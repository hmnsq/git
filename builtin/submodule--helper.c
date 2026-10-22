#define USE_THE_REPOSITORY_VARIABLE

#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "abspath.h"
#അന്തര്ഭാവ "environment.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "hex.h"

#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ "parse-options.h"
#അന്തര്ഭാവ "quote.h"
#അന്തര്ഭാവ "path.h"
#അന്തര്ഭാവ "pathspec.h"
#അന്തര്ഭാവ "preload-index.h"
#അന്തര്ഭാവ "dir.h"
#അന്തര്ഭാവ "read-cache.h"
#അന്തര്ഭാവ "setup.h"
#അന്തര്ഭാവ "sparse-index.h"
#അന്തര്ഭാവ "submodule.h"
#അന്തര്ഭാവ "submodule-config.h"
#അന്തര്ഭാവ "string-list.h"
#അന്തര്ഭാവ "run-command.h"
#അന്തര്ഭാവ "remote.h"
#അന്തര്ഭാവ "refs.h"
#അന്തര്ഭാവ "refspec.h"
#അന്തര്ഭാവ "revision.h"
#അന്തര്ഭാവ "diffcore.h"
#അന്തര്ഭാവ "diff.h"
#അന്തര്ഭാവ "object-file.h"
#അന്തര്ഭാവ "object-name.h"
#അന്തര്ഭാവ "odb.h"
#അന്തര്ഭാവ "odb/source.h"
#അന്തര്ഭാവ "odb/source-files.h"
#അന്തര്ഭാവ "advice.h"
#അന്തര്ഭാവ "branch.h"
#അന്തര്ഭാവ "list-objects-filter-options.h"
#അന്തര്ഭാവ "wildmatch.h"
#അന്തര്ഭാവ "strbuf.h"
#അന്തര്ഭാവ "url.h"

#define OPT_QUIET (1 << 0)
#define OPT_CACHED (1 << 1)
#define OPT_RECURSIVE (1 << 2)
#define OPT_FORCE (1 << 3)

തരനിർവചനം ശൂന്യം (*each_submodule_fn)(സ്ഥിരം ഘടന cache_entry *list_item,
				  ശൂന്യം *cb_data);

സ്ഥിതം അക്ഷരം *get_default_remote(ശൂന്യം)
{
	തിരികെ xstrdup(repo_default_remote(the_repository));
}

സ്ഥിതം അക്ഷരം *resolve_relative_url(സ്ഥിരം അക്ഷരം *rel_url, സ്ഥിരം അക്ഷരം *up_path, പൂർണ്ണസംഖ്യ quiet)
{
	അക്ഷരം *remoteurl, *resolved_url;
	അക്ഷരം *remote = get_default_remote();
	ഘടന strbuf remotesb = STRBUF_INIT;

	strbuf_addf(&remotesb, "remote.%s.url", remote);
	എങ്കിൽ (repo_config_get_string(the_repository, remotesb.buf, &remoteurl)) {
		എങ്കിൽ (!quiet)
			warning(_("could not look up configuration '%s'. "
				  "Assuming this repository is its own "
				  "authoritative upstream."),
				remotesb.buf);
		remoteurl = xgetcwd();
	}
	resolved_url = relative_url(remoteurl, rel_url, up_path);

	സ്വതന്ത്രം(remote);
	സ്വതന്ത്രം(remoteurl);
	strbuf_release(&remotesb);

	തിരികെ resolved_url;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ get_default_remote_submodule(സ്ഥിരം അക്ഷരം *module_path, അക്ഷരം **default_remote)
{
	സ്ഥിരം ഘടന submodule *sub;
	ഘടന repository subrepo;
	സ്ഥിരം അക്ഷരം *remote_name = ശൂന്യ;
	അക്ഷരം *url = ശൂന്യ;
	പൂർണ്ണസംഖ്യ ret = 0;

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), module_path);
	എങ്കിൽ (sub && sub->url) {
		url = xstrdup(sub->url);

		/* Possibly a url relative to parent */
		എങ്കിൽ (starts_with_dot_dot_slash(url) ||
		    starts_with_dot_slash(url)) {
			അക്ഷരം *oldurl = url;

			url = resolve_relative_url(oldurl, ശൂന്യ, 1);
			സ്വതന്ത്രം(oldurl);
		}
	}

	എങ്കിൽ (repo_submodule_init(&subrepo, the_repository, module_path,
				null_oid(the_hash_algo)) < 0) {
		ret = die_message(_("could not get a repository handle for submodule '%s'"),
				   module_path);
		പോകുക out;
	}

	/* Look up by URL first */
	എങ്കിൽ (url)
		remote_name = repo_remote_from_url(&subrepo, url);
	എങ്കിൽ (!remote_name)
		remote_name = repo_default_remote(&subrepo);

	*default_remote = xstrdup(remote_name);

out:
	repo_clear(&subrepo);
	സ്വതന്ത്രം(url);

	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ module_get_default_remote(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
				     ഘടന repository *repo UNUSED)
{
	സ്ഥിരം അക്ഷരം *path;
	അക്ഷരം *resolved_path = ശൂന്യ;
	അക്ഷരം *default_remote = ശൂന്യ;
	പൂർണ്ണസംഖ്യ code;
	ഘടന option options[] = {
		OPT_END()
	};
	സ്ഥിരം അക്ഷരം *സ്ഥിരം usage[] = {
		N_("git submodule--helper get-default-remote <path>"),
		ശൂന്യ
	};

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	എങ്കിൽ (argc != 1)
		usage_with_options(usage, options);

	path = argv[0];
	എങ്കിൽ (prefix && *prefix && !is_absolute_path(path)) {
		resolved_path = xstrfmt("%s%s", prefix, path);
		path = resolved_path;
	}

	code = get_default_remote_submodule(path, &default_remote);
	എങ്കിൽ (code) {
		സ്വതന്ത്രം(resolved_path);
		തിരികെ code;
	}

	printf("%s\n", default_remote);
	സ്വതന്ത്രം(default_remote);
	സ്വതന്ത്രം(resolved_path);
	തിരികെ 0;
}

/* the result should be freed by the caller. */
സ്ഥിതം അക്ഷരം *get_submodule_displaypath(സ്ഥിരം അക്ഷരം *path, സ്ഥിരം അക്ഷരം *prefix,
				       സ്ഥിരം അക്ഷരം *super_prefix)
{
	എങ്കിൽ (prefix && super_prefix) {
		BUG("cannot have prefix '%s' and superprefix '%s'",
		    prefix, super_prefix);
	} അല്ലെങ്കിൽ എങ്കിൽ (prefix) {
		ഘടന strbuf sb = STRBUF_INIT;
		അക്ഷരം *displaypath = xstrdup(relative_path(path, prefix, &sb));
		strbuf_release(&sb);
		തിരികെ displaypath;
	} അല്ലെങ്കിൽ എങ്കിൽ (super_prefix) {
		തിരികെ xstrfmt("%s%s", super_prefix, path);
	} അല്ലെങ്കിൽ {
		തിരികെ xstrdup(path);
	}
}

സ്ഥിതം അക്ഷരം *compute_rev_name(സ്ഥിരം അക്ഷരം *sub_path, സ്ഥിരം അക്ഷരം* object_id)
{
	ഘടന strbuf sb = STRBUF_INIT;
	സ്ഥിരം അക്ഷരം ***d;

	സ്ഥിതം സ്ഥിരം അക്ഷരം *describe_bare[] = { ശൂന്യ };

	സ്ഥിതം സ്ഥിരം അക്ഷരം *describe_tags[] = { "--tags", ശൂന്യ };

	സ്ഥിതം സ്ഥിരം അക്ഷരം *describe_contains[] = { "--contains", ശൂന്യ };

	സ്ഥിതം സ്ഥിരം അക്ഷരം *describe_all_always[] = { "--all", "--always", ശൂന്യ };

	സ്ഥിതം സ്ഥിരം അക്ഷരം **describe_argv[] = { describe_bare, describe_tags,
						describe_contains,
						describe_all_always, ശൂന്യ };

	വേണ്ടി (d = describe_argv; *d; d++) {
		ഘടന child_process cp = CHILD_PROCESS_INIT;
		prepare_submodule_repo_env(&cp.env);
		cp.dir = sub_path;
		cp.git_cmd = 1;
		cp.no_stderr = 1;

		strvec_തള്ളുക(&cp.args, "describe");
		strvec_pushv(&cp.args, *d);
		strvec_തള്ളുക(&cp.args, object_id);

		എങ്കിൽ (!capture_command(&cp, &sb, 0)) {
			strbuf_strip_suffix(&sb, "\n");
			തിരികെ strbuf_detach(&sb, ശൂന്യ);
		}
	}

	strbuf_release(&sb);
	തിരികെ ശൂന്യ;
}

ഘടന module_list {
	സ്ഥിരം ഘടന cache_entry **entries;
	പൂർണ്ണസംഖ്യ alloc, nr;
};
#define MODULE_LIST_INIT { 0 }

സ്ഥിതം ശൂന്യം module_list_release(ഘടന module_list *ml)
{
	സ്വതന്ത്രം(ml->entries);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ module_list_compute(സ്ഥിരം അക്ഷരം **argv,
			       സ്ഥിരം അക്ഷരം *prefix,
			       ഘടന pathspec *pathspec,
			       ഘടന module_list *list)
{
	പൂർണ്ണസംഖ്യ result = 0;
	അക്ഷരം *ps_matched = ശൂന്യ;

	parse_pathspec(pathspec, 0,
		       PATHSPEC_PREFER_FULL,
		       prefix, argv);

	എങ്കിൽ (pathspec->nr)
		ps_matched = xcalloc(pathspec->nr, 1);

	എങ്കിൽ (repo_read_index(the_repository) < 0)
		die(_("index file corrupt"));

	വേണ്ടി (size_t i = 0; i < the_repository->index->cache_nr; i++) {
		സ്ഥിരം ഘടന cache_entry *ce = the_repository->index->cache[i];

		എങ്കിൽ (!match_pathspec(the_repository->index, pathspec, ce->name, ce_namelen(ce),
				    0, ps_matched, 1) ||
		    !S_ISGITLINK(ce->ce_mode))
			തുടരുക;

		ALLOC_GROW(list->entries, list->nr + 1, list->alloc);
		list->entries[list->nr++] = ce;
		ആകുമ്പോൾ (i + 1 < the_repository->index->cache_nr &&
		       !strcmp(ce->name, the_repository->index->cache[i + 1]->name))
			/*
			 * Skip entries with the same name in different stages
			 * to make sure an entry is returned only once.
			 */
			i++;
	}

	എങ്കിൽ (ps_matched && report_path_error(ps_matched, pathspec))
		result = -1;

	സ്വതന്ത്രം(ps_matched);

	തിരികെ result;
}

സ്ഥിതം ശൂന്യം module_list_active(ഘടന module_list *list)
{
	പൂർണ്ണസംഖ്യ i;
	ഘടന module_list active_modules = MODULE_LIST_INIT;

	വേണ്ടി (i = 0; i < list->nr; i++) {
		സ്ഥിരം ഘടന cache_entry *ce = list->entries[i];

		എങ്കിൽ (!is_submodule_active(the_repository, ce->name))
			തുടരുക;

		ALLOC_GROW(active_modules.entries,
			   active_modules.nr + 1,
			   active_modules.alloc);
		active_modules.entries[active_modules.nr++] = ce;
	}

	module_list_release(list);
	*list = active_modules;
}

സ്ഥിതം അക്ഷരം *get_up_path(സ്ഥിരം അക്ഷരം *path)
{
	ഘടന strbuf sb = STRBUF_INIT;

	strbuf_addstrings(&sb, "../", എണ്ണം_slashes(path));

	/*
	 * Check if 'path' ends with slash or not
	 * for having the same output for dir/sub_dir
	 * and dir/sub_dir/
	 */
	എങ്കിൽ (!is_dir_sep(path[strlen(path) - 1]))
		strbuf_addstr(&sb, "../");

	തിരികെ strbuf_detach(&sb, ശൂന്യ);
}

സ്ഥിതം ശൂന്യം for_each_listed_submodule(സ്ഥിരം ഘടന module_list *list,
				      each_submodule_fn fn, ശൂന്യം *cb_data)
{
	പൂർണ്ണസംഖ്യ i;

	വേണ്ടി (i = 0; i < list->nr; i++)
		fn(list->entries[i], cb_data);
}

ഘടന foreach_cb {
	പൂർണ്ണസംഖ്യ argc;
	സ്ഥിരം അക്ഷരം **argv;
	സ്ഥിരം അക്ഷരം *prefix;
	സ്ഥിരം അക്ഷരം *super_prefix;
	പൂർണ്ണസംഖ്യ quiet;
	പൂർണ്ണസംഖ്യ recursive;
};
#define FOREACH_CB_INIT { 0 }

സ്ഥിതം ശൂന്യം runcommand_in_submodule_cb(സ്ഥിരം ഘടന cache_entry *list_item,
				       ശൂന്യം *cb_data)
{
	ഘടന foreach_cb *info = cb_data;
	സ്ഥിരം അക്ഷരം *path = list_item->name;
	സ്ഥിരം ഘടന object_id *ce_oid = &list_item->oid;
	സ്ഥിരം ഘടന submodule *sub;
	ഘടന child_process cp = CHILD_PROCESS_INIT;
	അക്ഷരം *displaypath;

	എങ്കിൽ (validate_submodule_path(path) < 0)
		die(ശൂന്യ);

	displaypath = get_submodule_displaypath(path, info->prefix,
						info->super_prefix);

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);

	എങ്കിൽ (!sub)
		die(_("No url found for submodule path '%s' in .gitmodules"),
			displaypath);

	എങ്കിൽ (!is_submodule_populated_gently(path, ശൂന്യ))
		പോകുക cleanup;

	prepare_submodule_repo_env(&cp.env);

	/*
	 * For the purpose of executing <command> in the submodule,
	 * separate shell is used for the purpose of running the
	 * child process.
	 */
	cp.use_shell = 1;
	cp.dir = path;

	/*
	 * NEEDSWORK: the command currently has access to the variables $name,
	 * $sm_path, $displaypath, $sha1 and $toplevel only when the command
	 * contains a single argument. This is done for maintaining a faithful
	 * translation from shell script.
	 */
	എങ്കിൽ (info->argc == 1) {
		അക്ഷരം *toplevel = xgetcwd();
		ഘടന strbuf sb = STRBUF_INIT;

		strvec_pushf(&cp.env, "name=%s", sub->name);
		strvec_pushf(&cp.env, "sm_path=%s", path);
		strvec_pushf(&cp.env, "displaypath=%s", displaypath);
		strvec_pushf(&cp.env, "sha1=%s",
			     oid_to_hex(ce_oid));
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
		sq_quote_buf(&sb, path);
		strvec_pushf(&cp.args, "path=%s; %s",
			     sb.buf, info->argv[0]);
		strbuf_release(&sb);
		സ്വതന്ത്രം(toplevel);
	} അല്ലെങ്കിൽ {
		strvec_pushv(&cp.args, info->argv);
	}

	എങ്കിൽ (!info->quiet)
		printf(_("Entering '%s'\n"), displaypath);

	എങ്കിൽ (info->argv[0]) {
		എങ്കിൽ (run_command(&cp))
			die(_("run_command returned non-zero status for %s\n."),
			    displaypath);
	} അല്ലെങ്കിൽ {
		child_process_clear(&cp);
	}

	എങ്കിൽ (info->recursive) {
		ഘടന child_process cpr = CHILD_PROCESS_INIT;

		cpr.git_cmd = 1;
		cpr.dir = path;
		prepare_submodule_repo_env(&cpr.env);

		strvec_pushl(&cpr.args, "submodule--helper", "foreach", "--recursive",
			     ശൂന്യ);
		strvec_pushf(&cpr.args, "--super-prefix=%s/", displaypath);

		എങ്കിൽ (info->quiet)
			strvec_തള്ളുക(&cpr.args, "--quiet");

		strvec_തള്ളുക(&cpr.args, "--");
		strvec_pushv(&cpr.args, info->argv);

		എങ്കിൽ (run_command(&cpr))
			die(_("run_command returned non-zero status while "
				"recursing in the nested submodules of %s\n."),
				displaypath);
	}

cleanup:
	സ്വതന്ത്രം(displaypath);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ module_foreach(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
			  ഘടന repository *repo UNUSED)
{
	ഘടന foreach_cb info = FOREACH_CB_INIT;
	ഘടന pathspec pathspec = { 0 };
	ഘടന module_list list = MODULE_LIST_INIT;
	ഘടന option module_foreach_options[] = {
		OPT__SUPER_PREFIX(&info.super_prefix),
		OPT__QUIET(&info.quiet, N_("suppress output of entering each submodule command")),
		OPT_BOOL(0, "recursive", &info.recursive,
			 N_("recurse into nested submodules")),
		OPT_END()
	};
	സ്ഥിരം അക്ഷരം *സ്ഥിരം git_submodule_helper_usage[] = {
		N_("git submodule foreach [--quiet] [--recursive] [--] <command>"),
		ശൂന്യ
	};
	പൂർണ്ണസംഖ്യ ret = 1;

	argc = parse_options(argc, argv, prefix, module_foreach_options,
			     git_submodule_helper_usage, 0);

	എങ്കിൽ (module_list_compute(ശൂന്യ, prefix, &pathspec, &list) < 0)
		പോകുക cleanup;

	info.argc = argc;
	info.argv = argv;
	info.prefix = prefix;

	for_each_listed_submodule(&list, runcommand_in_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	തിരികെ ret;
}

ഘടന init_cb {
	സ്ഥിരം അക്ഷരം *prefix;
	സ്ഥിരം അക്ഷരം *super_prefix;
	unsigned പൂർണ്ണസംഖ്യ flags;
};
#define INIT_CB_INIT { 0 }

സ്ഥിതം പൂർണ്ണസംഖ്യ validate_and_set_submodule_gitdir(ഘടന strbuf *gitdir_path,
					     സ്ഥിരം അക്ഷരം *submodule_name)
{
	സ്ഥിരം അക്ഷരം *മൂല്യം;
	അക്ഷരം *താക്കോൽ;

	എങ്കിൽ (validate_submodule_git_dir(gitdir_path->buf, submodule_name))
		തിരികെ -1;

	താക്കോൽ = xstrfmt("submodule.%s.gitdir", submodule_name);

	/* Nothing to do if the config already exists. */
	എങ്കിൽ (!repo_config_get_string_tmp(the_repository, താക്കോൽ, &മൂല്യം)) {
		സ്വതന്ത്രം(താക്കോൽ);
		തിരികെ 0;
	}

	എങ്കിൽ (repo_config_set_gently(the_repository, താക്കോൽ, gitdir_path->buf)) {
		സ്വതന്ത്രം(താക്കോൽ);
		തിരികെ -1;
	}

	സ്വതന്ത്രം(താക്കോൽ);
	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം create_default_gitdir_config(സ്ഥിരം അക്ഷരം *submodule_name)
{
	ഘടന strbuf gitdir_path = STRBUF_INIT;
	ഘടന git_hash_ctx ctx;
	അക്ഷരം hex_name_hash[GIT_MAX_HEXSZ + 1], header[128];
	unsigned അക്ഷരം raw_name_hash[GIT_MAX_RAWSZ];
	പൂർണ്ണസംഖ്യ header_len;

	/* Case 1: try the plain module name */
	repo_git_path_append(the_repository, &gitdir_path, "modules/%s", submodule_name);
	എങ്കിൽ (!validate_and_set_submodule_gitdir(&gitdir_path, submodule_name)) {
		strbuf_release(&gitdir_path);
		തിരികെ;
	}

	/* Case 2.1: Try URI-safe (RFC3986) encoding first, this fixes nested gitdirs */
	strbuf_reset(&gitdir_path);
	repo_git_path_append(the_repository, &gitdir_path, "modules/");
	strbuf_addstr_urlencode(&gitdir_path, submodule_name, is_rfc3986_unreserved);
	എങ്കിൽ (!validate_and_set_submodule_gitdir(&gitdir_path, submodule_name)) {
		strbuf_release(&gitdir_path);
		തിരികെ;
	}

	/* Case 2.2: Try extended uppercase URI (RFC3986) encoding, to fix case-folding */
	strbuf_reset(&gitdir_path);
	repo_git_path_append(the_repository, &gitdir_path, "modules/");
	strbuf_addstr_urlencode(&gitdir_path, submodule_name, is_casefolding_rfc3986_unreserved);
	എങ്കിൽ (!validate_and_set_submodule_gitdir(&gitdir_path, submodule_name))
		തിരികെ;

	/* Case 2.3: Try some derived gitdir names, see if one sticks */
	വേണ്ടി (അക്ഷരം c = '0'; c <= '9'; c++) {
		strbuf_reset(&gitdir_path);
		repo_git_path_append(the_repository, &gitdir_path, "modules/");
		strbuf_addstr_urlencode(&gitdir_path, submodule_name, is_rfc3986_unreserved);
		strbuf_addch(&gitdir_path, c);
		എങ്കിൽ (!validate_and_set_submodule_gitdir(&gitdir_path, submodule_name))
			തിരികെ;

		strbuf_reset(&gitdir_path);
		repo_git_path_append(the_repository, &gitdir_path, "modules/");
		strbuf_addstr_urlencode(&gitdir_path, submodule_name, is_casefolding_rfc3986_unreserved);
		strbuf_addch(&gitdir_path, c);
		എങ്കിൽ (!validate_and_set_submodule_gitdir(&gitdir_path, submodule_name))
			തിരികെ;
	}

	/* Case 2.4: If all the above failed, try a hash of the name as a last resort */
	header_len = snprintf(header, വലുപ്പം(header),
			      "blob %"PRIuMAX, (uintmax_t)strlen(submodule_name));
	git_hash_init(&ctx, the_hash_algo);
	git_hash_update(&ctx, header, header_len);
	git_hash_update(&ctx, "\0", 1);
	git_hash_update(&ctx, submodule_name, strlen(submodule_name));
	git_hash_final(raw_name_hash, &ctx);
	hash_to_hex_algop_r(hex_name_hash, raw_name_hash, the_hash_algo);
	strbuf_reset(&gitdir_path);
	repo_git_path_append(the_repository, &gitdir_path, "modules/%s", hex_name_hash);
	എങ്കിൽ (!validate_and_set_submodule_gitdir(&gitdir_path, submodule_name)) {
		strbuf_release(&gitdir_path);
		തിരികെ;
	}

	/* Case 3: nothing worked, error out */
	die(_("failed to set a valid default config for 'submodule.%s.gitdir'. "
	      "Please ensure it is set, for example by running something like: "
	      "'git config submodule.%s.gitdir .git/modules/%s'"),
	    submodule_name, submodule_name, submodule_name);
}

സ്ഥിതം ശൂന്യം init_submodule(സ്ഥിരം അക്ഷരം *path, സ്ഥിരം അക്ഷരം *prefix,
			   സ്ഥിരം അക്ഷരം *super_prefix,
			   unsigned പൂർണ്ണസംഖ്യ flags)
{
	സ്ഥിരം ഘടന submodule *sub;
	ഘടന strbuf sb = STRBUF_INIT;
	സ്ഥിരം അക്ഷരം *upd;
	അക്ഷരം *url = ശൂന്യ, *displaypath;

	displaypath = get_submodule_displaypath(path, prefix, super_prefix);

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);

	എങ്കിൽ (!sub)
		die(_("No url found for submodule path '%s' in .gitmodules"),
			displaypath);

	/*
	 * NEEDSWORK: In a multi-working-tree world, this needs to be
	 * set in the per-worktree config.
	 *
	 * Set active flag for the submodule being initialized
	 */
	എങ്കിൽ (!is_submodule_active(the_repository, path)) {
		strbuf_addf(&sb, "submodule.%s.active", sub->name);
		repo_config_set_gently(the_repository, sb.buf, "true");
		strbuf_reset(&sb);
	}

	/*
	 * Copy url setting when it is not set yet.
	 * To look up the url in .git/config, we must not fall back to
	 * .gitmodules, so look it up directly.
	 */
	strbuf_addf(&sb, "submodule.%s.url", sub->name);
	എങ്കിൽ (repo_config_get_string(the_repository, sb.buf, &url)) {
		എങ്കിൽ (!sub->url)
			die(_("No url found for submodule path '%s' in .gitmodules"),
				displaypath);

		url = xstrdup(sub->url);

		/* Possibly a url relative to parent */
		എങ്കിൽ (starts_with_dot_dot_slash(url) ||
		    starts_with_dot_slash(url)) {
			അക്ഷരം *oldurl = url;

			url = resolve_relative_url(oldurl, ശൂന്യ, 0);
			സ്വതന്ത്രം(oldurl);
		}

		എങ്കിൽ (repo_config_set_gently(the_repository, sb.buf, url))
			die(_("Failed to register url for submodule path '%s'"),
			    displaypath);
		എങ്കിൽ (!(flags & OPT_QUIET))
			fprintf(stderr,
				_("Submodule '%s' (%s) registered for path '%s'\n"),
				sub->name, url, displaypath);
	}
	strbuf_reset(&sb);

	/* Copy "update" setting when it is not set yet */
	strbuf_addf(&sb, "submodule.%s.update", sub->name);
	എങ്കിൽ (repo_config_get_string_tmp(the_repository, sb.buf, &upd) &&
	    sub->update_strategy.type != SM_UPDATE_UNSPECIFIED) {
		എങ്കിൽ (sub->update_strategy.type == SM_UPDATE_COMMAND) {
			fprintf(stderr, _("warning: command update mode suggested for submodule '%s'\n"),
				sub->name);
			upd = "none";
		} അല്ലെങ്കിൽ {
			upd = submodule_update_type_to_string(sub->update_strategy.type);
		}

		എങ്കിൽ (repo_config_set_gently(the_repository, sb.buf, upd))
			die(_("Failed to register update mode for submodule path '%s'"), displaypath);
	}

	എങ്കിൽ (the_repository->repository_format_submodule_path_cfg)
		create_default_gitdir_config(sub->name);

	strbuf_release(&sb);
	സ്വതന്ത്രം(displaypath);
	സ്വതന്ത്രം(url);
}

സ്ഥിതം ശൂന്യം init_submodule_cb(സ്ഥിരം ഘടന cache_entry *list_item, ശൂന്യം *cb_data)
{
	ഘടന init_cb *info = cb_data;

	init_submodule(list_item->name, info->prefix, info->super_prefix,
		       info->flags);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ module_init(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
		       ഘടന repository *repo UNUSED)
{
	ഘടന init_cb info = INIT_CB_INIT;
	ഘടന pathspec pathspec = { 0 };
	ഘടന module_list list = MODULE_LIST_INIT;
	പൂർണ്ണസംഖ്യ quiet = 0;
	ഘടന option module_init_options[] = {
		OPT__QUIET(&quiet, N_("suppress output for initializing a submodule")),
		OPT_END()
	};
	സ്ഥിരം അക്ഷരം *സ്ഥിരം git_submodule_helper_usage[] = {
		N_("git submodule init [<options>] [<path>]"),
		ശൂന്യ
	};
	പൂർണ്ണസംഖ്യ ret = 1;

	argc = parse_options(argc, argv, prefix, module_init_options,
			     git_submodule_helper_usage, 0);

	എങ്കിൽ (module_list_compute(argv, prefix, &pathspec, &list) < 0)
		പോകുക cleanup;

	/*
	 * If there are no path args and submodule.active is set then,
	 * by default, only initialize 'active' modules.
	 */
	എങ്കിൽ (!argc && !repo_config_get(the_repository, "submodule.active"))
		module_list_active(&list);

	info.prefix = prefix;
	എങ്കിൽ (quiet)
		info.flags |= OPT_QUIET;

	for_each_listed_submodule(&list, init_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	തിരികെ ret;
}

ഘടന status_cb {
	സ്ഥിരം അക്ഷരം *prefix;
	സ്ഥിരം അക്ഷരം *super_prefix;
	unsigned പൂർണ്ണസംഖ്യ flags;
};
#define STATUS_CB_INIT { 0 }

സ്ഥിതം ശൂന്യം print_status(unsigned പൂർണ്ണസംഖ്യ flags, അക്ഷരം state, സ്ഥിരം അക്ഷരം *path,
			 സ്ഥിരം ഘടന object_id *oid, സ്ഥിരം അക്ഷരം *displaypath)
{
	എങ്കിൽ (flags & OPT_QUIET)
		തിരികെ;

	printf("%c%s %s", state, oid_to_hex(oid), displaypath);

	എങ്കിൽ (state == ' ' || state == '+') {
		അക്ഷരം *name = compute_rev_name(path, oid_to_hex(oid));

		എങ്കിൽ (name)
			printf(" (%s)", name);
		സ്വതന്ത്രം(name);
	}

	printf("\n");
}

സ്ഥിതം പൂർണ്ണസംഖ്യ handle_submodule_head_ref(സ്ഥിരം ഘടന reference *ref, ശൂന്യം *cb_data)
{
	ഘടന object_id *output = cb_data;

	എങ്കിൽ (ref->oid)
		oidcpy(output, ref->oid);

	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം status_submodule(സ്ഥിരം അക്ഷരം *path, സ്ഥിരം ഘടന object_id *ce_oid,
			     unsigned പൂർണ്ണസംഖ്യ ce_flags, സ്ഥിരം അക്ഷരം *prefix,
			     സ്ഥിരം അക്ഷരം *super_prefix, unsigned പൂർണ്ണസംഖ്യ flags)
{
	അക്ഷരം *displaypath;
	ഘടന strvec diff_files_args = STRVEC_INIT;
	ഘടന rev_info rev = REV_INFO_INIT;
	ഘടന strbuf buf = STRBUF_INIT;
	സ്ഥിരം അക്ഷരം *git_dir;

	എങ്കിൽ (validate_submodule_path(path) < 0)
		die(ശൂന്യ);

	എങ്കിൽ (!submodule_from_path(the_repository, null_oid(the_hash_algo), path))
		die(_("no submodule mapping found in .gitmodules for path '%s'"),
		      path);

	displaypath = get_submodule_displaypath(path, prefix, super_prefix);

	എങ്കിൽ ((CE_STAGEMASK & ce_flags) >> CE_STAGESHIFT) {
		print_status(flags, 'U', path, null_oid(the_hash_algo), displaypath);
		പോകുക cleanup;
	}

	strbuf_addf(&buf, "%s/.git", path);
	git_dir = read_gitfile(buf.buf);
	എങ്കിൽ (!git_dir)
		git_dir = buf.buf;

	എങ്കിൽ (!is_submodule_active(the_repository, path) ||
	    !is_git_directory(git_dir)) {
		print_status(flags, '-', path, ce_oid, displaypath);
		strbuf_release(&buf);
		പോകുക cleanup;
	}
	strbuf_release(&buf);

	strvec_pushl(&diff_files_args, "diff-files",
		     "--ignore-submodules=dirty", "--quiet", "--",
		     path, ശൂന്യ);

	repo_config(the_repository, git_diff_basic_config, ശൂന്യ);

	repo_init_revisions(the_repository, &rev, ശൂന്യ);
	rev.abbrev = 0;
	setup_revisions_from_strvec(&diff_files_args, &rev, ശൂന്യ);
	run_diff_files(&rev, 0);

	എങ്കിൽ (!diff_result_code(&rev)) {
		print_status(flags, ' ', path, ce_oid,
			     displaypath);
	} അല്ലെങ്കിൽ എങ്കിൽ (!(flags & OPT_CACHED)) {
		ഘടന object_id oid;
		ഘടന ref_store *refs = repo_get_submodule_ref_store(the_repository,
								      path);

		എങ്കിൽ (!refs) {
			print_status(flags, '-', path, ce_oid, displaypath);
			പോകുക cleanup;
		}
		എങ്കിൽ (refs_head_ref(refs, handle_submodule_head_ref, &oid))
			die(_("could not resolve HEAD ref inside the "
			      "submodule '%s'"), path);

		print_status(flags, '+', path, &oid, displaypath);
	} അല്ലെങ്കിൽ {
		print_status(flags, '+', path, ce_oid, displaypath);
	}

	എങ്കിൽ (flags & OPT_RECURSIVE) {
		ഘടന child_process cpr = CHILD_PROCESS_INIT;
		പൂർണ്ണസംഖ്യ res;

		cpr.git_cmd = 1;
		cpr.dir = path;
		prepare_submodule_repo_env(&cpr.env);

		strvec_pushl(&cpr.args, "submodule--helper", "status",
			     "--recursive", ശൂന്യ);
		strvec_pushf(&cpr.args, "--super-prefix=%s/", displaypath);

		എങ്കിൽ (flags & OPT_CACHED)
			strvec_തള്ളുക(&cpr.args, "--cached");

		എങ്കിൽ (flags & OPT_QUIET)
			strvec_തള്ളുക(&cpr.args, "--quiet");

		res = run_command(&cpr);
		എങ്കിൽ (res == SIGPIPE + 128)
			ഉയർത്തുക(SIGPIPE);
		അല്ലെങ്കിൽ എങ്കിൽ (res)
			die(_("failed to recurse into submodule '%s'"), path);
	}

cleanup:
	strvec_clear(&diff_files_args);
	സ്വതന്ത്രം(displaypath);
	release_revisions(&rev);
}

സ്ഥിതം ശൂന്യം status_submodule_cb(സ്ഥിരം ഘടന cache_entry *list_item,
				ശൂന്യം *cb_data)
{
	ഘടന status_cb *info = cb_data;

	status_submodule(list_item->name, &list_item->oid, list_item->ce_flags,
			 info->prefix, info->super_prefix, info->flags);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ module_status(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
			 ഘടന repository *repo UNUSED)
{
	ഘടന status_cb info = STATUS_CB_INIT;
	ഘടന pathspec pathspec = { 0 };
	ഘടന module_list list = MODULE_LIST_INIT;
	പൂർണ്ണസംഖ്യ quiet = 0;
	ഘടന option module_status_options[] = {
		OPT__SUPER_PREFIX(&info.super_prefix),
		OPT__QUIET(&quiet, N_("suppress submodule status output")),
		OPT_BIT(0, "cached", &info.flags, N_("use commit stored in the index instead of the one stored in the submodule HEAD"), OPT_CACHED),
		OPT_BIT(0, "recursive", &info.flags, N_("recurse into nested submodules"), OPT_RECURSIVE),
		OPT_END()
	};
	സ്ഥിരം അക്ഷരം *സ്ഥിരം git_submodule_helper_usage[] = {
		N_("git submodule status [--quiet] [--cached] [--recursive] [<path>...]"),
		ശൂന്യ
	};
	പൂർണ്ണസംഖ്യ ret = 1;

	argc = parse_options(argc, argv, prefix, module_status_options,
			     git_submodule_helper_usage, 0);

	എങ്കിൽ (module_list_compute(argv, prefix, &pathspec, &list) < 0)
		പോകുക cleanup;

	info.prefix = prefix;
	എങ്കിൽ (quiet)
		info.flags |= OPT_QUIET;

	for_each_listed_submodule(&list, status_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	തിരികെ ret;
}

ഘടന module_cb {
	unsigned പൂർണ്ണസംഖ്യ mod_src;
	unsigned പൂർണ്ണസംഖ്യ mod_dst;
	ഘടന object_id oid_src;
	ഘടന object_id oid_dst;
	അക്ഷരം status;
	അക്ഷരം *sm_path;
};
#define MODULE_CB_INIT { 0 }

സ്ഥിതം ശൂന്യം module_cb_release(ഘടന module_cb *mcb)
{
	സ്വതന്ത്രം(mcb->sm_path);
}

ഘടന module_cb_list {
	ഘടന module_cb **entries;
	പൂർണ്ണസംഖ്യ alloc, nr;
};
#define MODULE_CB_LIST_INIT { 0 }

സ്ഥിതം ശൂന്യം module_cb_list_release(ഘടന module_cb_list *mcbl)
{
	പൂർണ്ണസംഖ്യ i;

	വേണ്ടി (i = 0; i < mcbl->nr; i++) {
		ഘടന module_cb *mcb = mcbl->entries[i];

		module_cb_release(mcb);
		സ്വതന്ത്രം(mcb);
	}
	സ്വതന്ത്രം(mcbl->entries);
}

ഘടന summary_cb {
	പൂർണ്ണസംഖ്യ argc;
	സ്ഥിരം അക്ഷരം **argv;
	സ്ഥിരം അക്ഷരം *prefix;
	സ്ഥിരം അക്ഷരം *super_prefix;
	unsigned പൂർണ്ണസംഖ്യ cached: 1;
	unsigned പൂർണ്ണസംഖ്യ for_status: 1;
	unsigned പൂർണ്ണസംഖ്യ files: 1;
	പൂർണ്ണസംഖ്യ summary_limit;
};
#define SUMMARY_CB_INIT { 0 }

എണ്ണൽ diff_cmd {
	DIFF_INDEX,
	DIFF_FILES
};

സ്ഥിതം അക്ഷരം *verify_submodule_committish(സ്ഥിരം അക്ഷരം *sm_path,
					 സ്ഥിരം അക്ഷരം *committish)
{
	ഘടന child_process cp_rev_parse = CHILD_PROCESS_INIT;
	ഘടന strbuf result = STRBUF_INIT;

	cp_rev_parse.git_cmd = 1;
	cp_rev_parse.dir = sm_path;
	prepare_submodule_repo_env(&cp_rev_parse.env);
	strvec_pushl(&cp_rev_parse.args, "rev-parse", "-q", "--short", ശൂന്യ);
	strvec_pushf(&cp_rev_parse.args, "%s^0", committish);
	strvec_തള്ളുക(&cp_rev_parse.args, "--");

	എങ്കിൽ (capture_command(&cp_rev_parse, &result, 0))
		തിരികെ ശൂന്യ;

	strbuf_trim_trailing_newline(&result);
	തിരികെ strbuf_detach(&result, ശൂന്യ);
}

സ്ഥിതം ശൂന്യം print_submodule_summary(ഘടന summary_cb *info, സ്ഥിരം അക്ഷരം *errmsg,
				    പൂർണ്ണസംഖ്യ total_commits, സ്ഥിരം അക്ഷരം *displaypath,
				    സ്ഥിരം അക്ഷരം *src_abbrev, സ്ഥിരം അക്ഷരം *dst_abbrev,
				    ഘടന module_cb *p)
{
	എങ്കിൽ (p->status == 'T') {
		എങ്കിൽ (S_ISGITLINK(p->mod_dst))
			printf(_("* %s %s(blob)->%s(submodule)"),
				 displaypath, src_abbrev, dst_abbrev);
		അല്ലെങ്കിൽ
			printf(_("* %s %s(submodule)->%s(blob)"),
				 displaypath, src_abbrev, dst_abbrev);
	} അല്ലെങ്കിൽ {
		printf("* %s %s...%s",
			displaypath, src_abbrev, dst_abbrev);
	}

	എങ്കിൽ (total_commits < 0)
		printf(":\n");
	അല്ലെങ്കിൽ
		printf(" (%d):\n", total_commits);

	എങ്കിൽ (errmsg) {
		printf(_("%s"), errmsg);
	} അല്ലെങ്കിൽ എങ്കിൽ (total_commits > 0) {
		ഘടന child_process cp_log = CHILD_PROCESS_INIT;

		cp_log.git_cmd = 1;
		cp_log.dir = p->sm_path;
		prepare_submodule_repo_env(&cp_log.env);
		strvec_pushl(&cp_log.args, "log", ശൂന്യ);

		എങ്കിൽ (S_ISGITLINK(p->mod_src) && S_ISGITLINK(p->mod_dst)) {
			എങ്കിൽ (info->summary_limit > 0)
				strvec_pushf(&cp_log.args, "-%d",
					     info->summary_limit);

			strvec_pushl(&cp_log.args, "--pretty=  %m %s",
				     "--first-parent", ശൂന്യ);
			strvec_pushf(&cp_log.args, "%s...%s",
				     src_abbrev, dst_abbrev);
		} അല്ലെങ്കിൽ എങ്കിൽ (S_ISGITLINK(p->mod_dst)) {
			strvec_pushl(&cp_log.args, "--pretty=  > %s",
				     "-1", dst_abbrev, ശൂന്യ);
		} അല്ലെങ്കിൽ {
			strvec_pushl(&cp_log.args, "--pretty=  < %s",
				     "-1", src_abbrev, ശൂന്യ);
		}
		run_command(&cp_log);
	}
	printf("\n");
}

സ്ഥിതം ശൂന്യം generate_submodule_summary(ഘടന summary_cb *info,
				       ഘടന module_cb *p)
{
	അക്ഷരം *displaypath, *src_abbrev = ശൂന്യ, *dst_abbrev;
	പൂർണ്ണസംഖ്യ missing_src = 0, missing_dst = 0;
	ഘടന strbuf errmsg = STRBUF_INIT;
	പൂർണ്ണസംഖ്യ total_commits = -1;

	എങ്കിൽ (!info->cached && oideq(&p->oid_dst, null_oid(the_hash_algo))) {
		എങ്കിൽ (S_ISGITLINK(p->mod_dst)) {
			ഘടന ref_store *refs = repo_get_submodule_ref_store(the_repository,
									      p->sm_path);

			എങ്കിൽ (refs)
				refs_head_ref(refs, handle_submodule_head_ref, &p->oid_dst);
		} അല്ലെങ്കിൽ എങ്കിൽ (S_ISLNK(p->mod_dst) || S_ISREG(p->mod_dst)) {
			ഘടന stat st;
			പൂർണ്ണസംഖ്യ fd = open(p->sm_path, O_RDONLY);

			എങ്കിൽ (fd < 0 || fstat(fd, &st) < 0 ||
			    index_fd(the_repository->index, &p->oid_dst, fd, &st, OBJ_BLOB,
				     p->sm_path, 0))
				error(_("couldn't hash object from '%s'"), p->sm_path);
		} അല്ലെങ്കിൽ {
			/* for a submodule removal (mode:0000000), don't warn */
			എങ്കിൽ (p->mod_dst)
				warning(_("unexpected mode %o"), p->mod_dst);
		}
	}

	എങ്കിൽ (S_ISGITLINK(p->mod_src)) {
		എങ്കിൽ (p->status != 'D')
			src_abbrev = verify_submodule_committish(p->sm_path,
								 oid_to_hex(&p->oid_src));
		എങ്കിൽ (!src_abbrev) {
			missing_src = 1;
			/*
			 * As `rev-parse` failed, we fallback to getting
			 * the abbreviated hash using oid_src. We do
			 * this as we might still need the abbreviated
			 * hash in cases like a submodule type change, etc.
			 */
			src_abbrev = xstrndup(oid_to_hex(&p->oid_src), 7);
		}
	} അല്ലെങ്കിൽ {
		/*
		 * The source does not point to a submodule.
		 * So, we fallback to getting the abbreviation using
		 * oid_src as we might still need the abbreviated
		 * hash in cases like submodule add, etc.
		 */
		src_abbrev = xstrndup(oid_to_hex(&p->oid_src), 7);
	}

	എങ്കിൽ (S_ISGITLINK(p->mod_dst)) {
		dst_abbrev = verify_submodule_committish(p->sm_path,
							 oid_to_hex(&p->oid_dst));
		എങ്കിൽ (!dst_abbrev) {
			missing_dst = 1;
			/*
			 * As `rev-parse` failed, we fallback to getting
			 * the abbreviated hash using oid_dst. We do
			 * this as we might still need the abbreviated
			 * hash in cases like a submodule type change, etc.
			 */
			dst_abbrev = xstrndup(oid_to_hex(&p->oid_dst), 7);
		}
	} അല്ലെങ്കിൽ {
		/*
		 * The destination does not point to a submodule.
		 * So, we fallback to getting the abbreviation using
		 * oid_dst as we might still need the abbreviated
		 * hash in cases like a submodule removal, etc.
		 */
		dst_abbrev = xstrndup(oid_to_hex(&p->oid_dst), 7);
	}

	displaypath = get_submodule_displaypath(p->sm_path, info->prefix,
						info->super_prefix);

	എങ്കിൽ (!missing_src && !missing_dst) {
		ഘടന child_process cp_rev_list = CHILD_PROCESS_INIT;
		ഘടന strbuf sb_rev_list = STRBUF_INIT;

		strvec_pushl(&cp_rev_list.args, "rev-list",
			     "--first-parent", "--count", ശൂന്യ);
		എങ്കിൽ (S_ISGITLINK(p->mod_src) && S_ISGITLINK(p->mod_dst))
			strvec_pushf(&cp_rev_list.args, "%s...%s",
				     src_abbrev, dst_abbrev);
		അല്ലെങ്കിൽ
			strvec_തള്ളുക(&cp_rev_list.args, S_ISGITLINK(p->mod_src) ?
				    src_abbrev : dst_abbrev);
		strvec_തള്ളുക(&cp_rev_list.args, "--");

		cp_rev_list.git_cmd = 1;
		cp_rev_list.dir = p->sm_path;
		prepare_submodule_repo_env(&cp_rev_list.env);

		എങ്കിൽ (!capture_command(&cp_rev_list, &sb_rev_list, 0))
			total_commits = atoi(sb_rev_list.buf);

		strbuf_release(&sb_rev_list);
	} അല്ലെങ്കിൽ {
		/*
		 * Don't give error msg for modification whose dst is not
		 * submodule, i.e., deleted or changed to blob
		 */
		എങ്കിൽ (S_ISGITLINK(p->mod_dst)) {
			എങ്കിൽ (missing_src && missing_dst) {
				strbuf_addf(&errmsg, "  Warn: %s doesn't contain commits %s and %s\n",
					    displaypath, oid_to_hex(&p->oid_src),
					    oid_to_hex(&p->oid_dst));
			} അല്ലെങ്കിൽ {
				strbuf_addf(&errmsg, "  Warn: %s doesn't contain commit %s\n",
					    displaypath, missing_src ?
					    oid_to_hex(&p->oid_src) :
					    oid_to_hex(&p->oid_dst));
			}
		}
	}

	print_submodule_summary(info, errmsg.len ? errmsg.buf : ശൂന്യ,
				total_commits, displaypath, src_abbrev,
				dst_abbrev, p);

	സ്വതന്ത്രം(displaypath);
	സ്വതന്ത്രം(src_abbrev);
	സ്വതന്ത്രം(dst_abbrev);
	strbuf_release(&errmsg);
}

സ്ഥിതം ശൂന്യം prepare_submodule_summary(ഘടന summary_cb *info,
				      ഘടന module_cb_list *list)
{
	പൂർണ്ണസംഖ്യ i;
	വേണ്ടി (i = 0; i < list->nr; i++) {
		സ്ഥിരം ഘടന submodule *sub;
		ഘടന module_cb *p = list->entries[i];
		ഘടന strbuf sm_gitdir = STRBUF_INIT;

		എങ്കിൽ (p->status == 'D' || p->status == 'T') {
			generate_submodule_summary(info, p);
			തുടരുക;
		}

		എങ്കിൽ (info->for_status && p->status != 'A' &&
		    (sub = submodule_from_path(the_repository,
					       null_oid(the_hash_algo), p->sm_path))) {
			അക്ഷരം *config_താക്കോൽ = ശൂന്യ;
			സ്ഥിരം അക്ഷരം *മൂല്യം;
			പൂർണ്ണസംഖ്യ ignore_all = 0;

			config_താക്കോൽ = xstrfmt("submodule.%s.ignore",
					     sub->name);
			എങ്കിൽ (!repo_config_get_string_tmp(the_repository, config_താക്കോൽ, &മൂല്യം))
				ignore_all = !strcmp(മൂല്യം, "all");
			അല്ലെങ്കിൽ എങ്കിൽ (sub->ignore)
				ignore_all = !strcmp(sub->ignore, "all");

			സ്വതന്ത്രം(config_താക്കോൽ);
			എങ്കിൽ (ignore_all)
				തുടരുക;
		}

		/* Also show added or modified modules which are checked out */
		strbuf_addstr(&sm_gitdir, p->sm_path);
		എങ്കിൽ (is_nonbare_repository_dir(&sm_gitdir))
			generate_submodule_summary(info, p);
		strbuf_release(&sm_gitdir);
	}
}

സ്ഥിതം ശൂന്യം submodule_summary_callback(ഘടന diff_queue_struct *q,
				       ഘടന diff_options *options UNUSED,
				       ശൂന്യം *data)
{
	പൂർണ്ണസംഖ്യ i;
	ഘടന module_cb_list *list = data;
	വേണ്ടി (i = 0; i < q->nr; i++) {
		ഘടന diff_filepair *p = q->queue[i];
		ഘടന module_cb *temp;

		എങ്കിൽ (!S_ISGITLINK(p->one->mode) && !S_ISGITLINK(p->two->mode))
			തുടരുക;
		temp = xmalloc(വലുപ്പം(*temp));
		temp->mod_src = p->one->mode;
		temp->mod_dst = p->two->mode;
		temp->oid_src = p->one->oid;
		temp->oid_dst = p->two->oid;
		temp->status = p->status;
		temp->sm_path = xstrdup(p->one->path);

		ALLOC_GROW(list->entries, list->nr + 1, list->alloc);
		list->entries[list->nr++] = temp;
	}
}

സ്ഥിതം സ്ഥിരം അക്ഷരം *get_diff_cmd(എണ്ണൽ diff_cmd diff_cmd)
{
	തിരഞ്ഞെടുപ്പ് (diff_cmd) {
	അവസ്ഥ DIFF_INDEX: തിരികെ "diff-index";
	അവസ്ഥ DIFF_FILES: തിരികെ "diff-files";
	സ്ഥിരസ്ഥിതി: BUG("bad diff_cmd value %d", diff_cmd);
	}
}

സ്ഥിതം പൂർണ്ണസംഖ്യ compute_summary_module_list(ഘടന object_id *head_oid,
				       ഘടന summary_cb *info,
				       എണ്ണൽ diff_cmd diff_cmd)
{
	ഘടന strvec diff_args = STRVEC_INIT;
	ഘടന rev_info rev;
	ഘടന module_cb_list list = MODULE_CB_LIST_INIT;
	പൂർണ്ണസംഖ്യ ret = 0;

	strvec_തള്ളുക(&diff_args, get_diff_cmd(diff_cmd));
	എങ്കിൽ (info->cached)
		strvec_തള്ളുക(&diff_args, "--cached");
	strvec_pushl(&diff_args, "--ignore-submodules=dirty", "--raw", ശൂന്യ);
	എങ്കിൽ (head_oid)
		strvec_തള്ളുക(&diff_args, oid_to_hex(head_oid));
	strvec_തള്ളുക(&diff_args, "--");
	എങ്കിൽ (info->argc)
		strvec_pushv(&diff_args, info->argv);

	repo_config(the_repository, git_diff_basic_config, ശൂന്യ);
	repo_init_revisions(the_repository, &rev, info->prefix);
	rev.abbrev = 0;
	precompose_argv_prefix(diff_args.nr, diff_args.v, ശൂന്യ);
	setup_revisions_from_strvec(&diff_args, &rev, ശൂന്യ);
	rev.diffopt.output_format = DIFF_FORMAT_NO_OUTPUT | DIFF_FORMAT_CALLBACK;
	rev.diffopt.format_callback = submodule_summary_callback;
	rev.diffopt.format_callback_data = &list;

	എങ്കിൽ (!info->cached) {
		എങ്കിൽ (diff_cmd == DIFF_INDEX)
			setup_work_വൃക്ഷം(the_repository);
		എങ്കിൽ (repo_read_index_preload(the_repository, &rev.diffopt.pathspec, 0) < 0) {
			perror("repo_read_index_preload");
			ret = -1;
			പോകുക cleanup;
		}
	} അല്ലെങ്കിൽ എങ്കിൽ (repo_read_index(the_repository) < 0) {
		perror("repo_read_cache");
		ret = -1;
		പോകുക cleanup;
	}

	എങ്കിൽ (diff_cmd == DIFF_INDEX)
		run_diff_index(&rev, info->cached ? DIFF_INDEX_CACHED : 0);
	അല്ലെങ്കിൽ
		run_diff_files(&rev, 0);
	prepare_submodule_summary(info, &list);
cleanup:
	strvec_clear(&diff_args);
	release_revisions(&rev);
	module_cb_list_release(&list);
	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ module_summary(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
			  ഘടന repository *repo UNUSED)
{
	ഘടന summary_cb info = SUMMARY_CB_INIT;
	പൂർണ്ണസംഖ്യ cached = 0;
	പൂർണ്ണസംഖ്യ for_status = 0;
	പൂർണ്ണസംഖ്യ files = 0;
	പൂർണ്ണസംഖ്യ summary_limit = -1;
	എണ്ണൽ diff_cmd diff_cmd = DIFF_INDEX;
	ഘടന object_id head_oid;
	പൂർണ്ണസംഖ്യ ret;
	ഘടന option module_summary_options[] = {
		OPT_BOOL(0, "cached", &cached,
			 N_("use the commit stored in the index instead of the submodule HEAD")),
		OPT_BOOL(0, "files", &files,
			 N_("compare the commit in the index with that in the submodule HEAD")),
		OPT_BOOL(0, "for-status", &for_status,
			 N_("skip submodules with 'ignore_config' value set to 'all'")),
		OPT_INTEGER('n', "summary-limit", &summary_limit,
			     N_("limit the summary size")),
		OPT_END()
	};
	സ്ഥിരം അക്ഷരം *സ്ഥിരം git_submodule_helper_usage[] = {
		N_("git submodule summary [<options>] [<commit>] [--] [<path>]"),
		ശൂന്യ
	};

	argc = parse_options(argc, argv, prefix, module_summary_options,
			     git_submodule_helper_usage, 0);

	എങ്കിൽ (!summary_limit)
		തിരികെ 0;

	എങ്കിൽ (!repo_get_oid(the_repository, argc ? argv[0] : "HEAD", &head_oid)) {
		എങ്കിൽ (argc) {
			argv++;
			argc--;
		}
	} അല്ലെങ്കിൽ എങ്കിൽ (!argc || !strcmp(argv[0], "HEAD")) {
		/* before the first commit: compare with an empty tree */
		oidcpy(&head_oid, the_hash_algo->empty_വൃക്ഷം);
		എങ്കിൽ (argc) {
			argv++;
			argc--;
		}
	} അല്ലെങ്കിൽ {
		എങ്കിൽ (repo_get_oid(the_repository, "HEAD", &head_oid))
			die(_("could not fetch a revision for HEAD"));
	}

	എങ്കിൽ (files) {
		എങ്കിൽ (cached)
			die(_("options '%s' and '%s' cannot be used together"), "--cached", "--files");
		diff_cmd = DIFF_FILES;
	}

	info.argc = argc;
	info.argv = argv;
	info.prefix = prefix;
	info.cached = !!cached;
	info.files = !!files;
	info.for_status = !!for_status;
	info.summary_limit = summary_limit;

	ret = compute_summary_module_list((diff_cmd == DIFF_INDEX) ? &head_oid : ശൂന്യ,
					  &info, diff_cmd);
	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ module_gitdir(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix UNUSED,
			 ഘടന repository *repo)
{
	ഘടന strbuf gitdir = STRBUF_INIT;

	എങ്കിൽ (argc != 2)
		usage(_("git submodule--helper gitdir <name>"));

	submodule_name_to_gitdir(&gitdir, repo, argv[1]);

	printf("%s\n", gitdir.buf);

	strbuf_release(&gitdir);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ module_migrate(പൂർണ്ണസംഖ്യ argc UNUSED, സ്ഥിരം അക്ഷരം **argv UNUSED,
			  സ്ഥിരം അക്ഷരം *prefix UNUSED, ഘടന repository *repo)
{
	ഘടന strbuf module_dir = STRBUF_INIT;
	DIR *dir;
	ഘടന dirent *de;
	പൂർണ്ണസംഖ്യ repo_version = 0;

	repo_git_path_append(repo, &module_dir, "modules/");

	dir = opendir(module_dir.buf);
	എങ്കിൽ (!dir)
		die(_("could not open '%s'"), module_dir.buf);

	ആകുമ്പോൾ ((de = readdir(dir))) {
		ഘടന strbuf gitdir_path = STRBUF_INIT;
		അക്ഷരം *താക്കോൽ;
		സ്ഥിരം അക്ഷരം *മൂല്യം;

		എങ്കിൽ (is_dot_or_dotdot(de->d_name))
			തുടരുക;

		strbuf_addf(&gitdir_path, "%s/%s", module_dir.buf, de->d_name);
		എങ്കിൽ (!is_git_directory(gitdir_path.buf)) {
			strbuf_release(&gitdir_path);
			തുടരുക;
		}
		strbuf_release(&gitdir_path);

		താക്കോൽ = xstrfmt("submodule.%s.gitdir", de->d_name);
		എങ്കിൽ (!repo_config_get_string_tmp(repo, താക്കോൽ, &മൂല്യം)) {
			/* Already has a gitdir config, nothing to do. */
			സ്വതന്ത്രം(താക്കോൽ);
			തുടരുക;
		}
		സ്വതന്ത്രം(താക്കോൽ);

		create_default_gitdir_config(de->d_name);
	}

	closedir(dir);
	strbuf_release(&module_dir);

	repo_config_get_int(the_repository, "core.repositoryformatversion", &repo_version);
	എങ്കിൽ (repo_version == 0 &&
	    repo_config_set_gently(repo, "core.repositoryformatversion", "1"))
		die(_("could not set core.repositoryformatversion to 1.\n"
		      "Please set it for migration to work, for example:\n"
		      "git config core.repositoryformatversion 1"));

	എങ്കിൽ (repo_config_set_gently(repo, "extensions.submodulePathConfig", "true"))
		die(_("could not enable submodulePathConfig extension. It is required\n"
		      "for migration to work. Please enable it in the root repo:\n"
		      "git config extensions.submodulePathConfig true"));

	repo->repository_format_submodule_path_cfg = 1;

	തിരികെ 0;
}

ഘടന sync_cb {
	സ്ഥിരം അക്ഷരം *prefix;
	സ്ഥിരം അക്ഷരം *super_prefix;
	unsigned പൂർണ്ണസംഖ്യ flags;
};
#define SYNC_CB_INIT { 0 }

സ്ഥിതം ശൂന്യം sync_submodule(സ്ഥിരം അക്ഷരം *path, സ്ഥിരം അക്ഷരം *prefix,
			   സ്ഥിരം അക്ഷരം *super_prefix, unsigned പൂർണ്ണസംഖ്യ flags)
{
	സ്ഥിരം ഘടന submodule *sub;
	അക്ഷരം *remote_താക്കോൽ = ശൂന്യ;
	അക്ഷരം *sub_origin_url, *super_config_url, *displaypath, *default_remote;
	ഘടന strbuf sb = STRBUF_INIT;
	അക്ഷരം *sub_config_path = ശൂന്യ;
	പൂർണ്ണസംഖ്യ code;

	എങ്കിൽ (!is_submodule_active(the_repository, path))
		തിരികെ;

	എങ്കിൽ (validate_submodule_path(path) < 0)
		die(ശൂന്യ);

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);

	എങ്കിൽ (sub && sub->url) {
		എങ്കിൽ (starts_with_dot_dot_slash(sub->url) ||
		    starts_with_dot_slash(sub->url)) {
			അക്ഷരം *up_path = get_up_path(path);

			sub_origin_url = resolve_relative_url(sub->url, up_path, 1);
			super_config_url = resolve_relative_url(sub->url, ശൂന്യ, 1);
			സ്വതന്ത്രം(up_path);
		} അല്ലെങ്കിൽ {
			sub_origin_url = xstrdup(sub->url);
			super_config_url = xstrdup(sub->url);
		}
	} അല്ലെങ്കിൽ {
		sub_origin_url = xstrdup("");
		super_config_url = xstrdup("");
	}

	displaypath = get_submodule_displaypath(path, prefix, super_prefix);

	എങ്കിൽ (!(flags & OPT_QUIET))
		printf(_("Synchronizing submodule url for '%s'\n"),
			 displaypath);

	strbuf_reset(&sb);
	strbuf_addf(&sb, "submodule.%s.url", sub->name);
	എങ്കിൽ (repo_config_set_gently(the_repository, sb.buf, super_config_url))
		die(_("failed to register url for submodule path '%s'"),
		      displaypath);

	എങ്കിൽ (!is_submodule_populated_gently(path, ശൂന്യ))
		പോകുക cleanup;

	strbuf_reset(&sb);
	code = get_default_remote_submodule(path, &default_remote);
	എങ്കിൽ (code)
		exit(code);

	remote_താക്കോൽ = xstrfmt("remote.%s.url", default_remote);
	സ്വതന്ത്രം(default_remote);

	submodule_to_gitdir(the_repository, &sb, path);
	strbuf_addstr(&sb, "/config");

	എങ്കിൽ (repo_config_set_in_file_gently(the_repository, sb.buf, remote_താക്കോൽ, ശൂന്യ, sub_origin_url))
		die(_("failed to update remote for submodule '%s'"),
		      path);

	എങ്കിൽ (flags & OPT_RECURSIVE) {
		ഘടന child_process cpr = CHILD_PROCESS_INIT;

		cpr.git_cmd = 1;
		cpr.dir = path;
		prepare_submodule_repo_env(&cpr.env);

		strvec_pushl(&cpr.args, "submodule--helper", "sync",
			     "--recursive", ശൂന്യ);
		strvec_pushf(&cpr.args, "--super-prefix=%s/", displaypath);

		എങ്കിൽ (flags & OPT_QUIET)
			strvec_തള്ളുക(&cpr.args, "--quiet");

		എങ്കിൽ (run_command(&cpr))
			die(_("failed to recurse into submodule '%s'"),
			      path);
	}

cleanup:
	സ്വതന്ത്രം(super_config_url);
	സ്വതന്ത്രം(sub_origin_url);
	strbuf_release(&sb);
	സ്വതന്ത്രം(remote_താക്കോൽ);
	സ്വതന്ത്രം(displaypath);
	സ്വതന്ത്രം(sub_config_path);
}

സ്ഥിതം ശൂന്യം sync_submodule_cb(സ്ഥിരം ഘടന cache_entry *list_item, ശൂന്യം *cb_data)
{
	ഘടന sync_cb *info = cb_data;

	sync_submodule(list_item->name, info->prefix, info->super_prefix,
		       info->flags);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ module_sync(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
		       ഘടന repository *repo UNUSED)
{
	ഘടന sync_cb info = SYNC_CB_INIT;
	ഘടന pathspec pathspec = { 0 };
	ഘടന module_list list = MODULE_LIST_INIT;
	പൂർണ്ണസംഖ്യ quiet = 0;
	പൂർണ്ണസംഖ്യ recursive = 0;
	ഘടന option module_sync_options[] = {
		OPT__SUPER_PREFIX(&info.super_prefix),
		OPT__QUIET(&quiet, N_("suppress output of synchronizing submodule url")),
		OPT_BOOL(0, "recursive", &recursive,
			N_("recurse into nested submodules")),
		OPT_END()
	};
	സ്ഥിരം അക്ഷരം *സ്ഥിരം git_submodule_helper_usage[] = {
		N_("git submodule sync [--quiet] [--recursive] [<path>]"),
		ശൂന്യ
	};
	പൂർണ്ണസംഖ്യ ret = 1;

	argc = parse_options(argc, argv, prefix, module_sync_options,
			     git_submodule_helper_usage, 0);

	എങ്കിൽ (module_list_compute(argv, prefix, &pathspec, &list) < 0)
		പോകുക cleanup;

	info.prefix = prefix;
	എങ്കിൽ (quiet)
		info.flags |= OPT_QUIET;
	എങ്കിൽ (recursive)
		info.flags |= OPT_RECURSIVE;

	for_each_listed_submodule(&list, sync_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	തിരികെ ret;
}

ഘടന deinit_cb {
	സ്ഥിരം അക്ഷരം *prefix;
	unsigned പൂർണ്ണസംഖ്യ flags;
};
#define DEINIT_CB_INIT { 0 }

സ്ഥിതം ശൂന്യം deinit_submodule(സ്ഥിരം അക്ഷരം *path, സ്ഥിരം അക്ഷരം *prefix,
			     unsigned പൂർണ്ണസംഖ്യ flags)
{
	സ്ഥിരം ഘടന submodule *sub;
	അക്ഷരം *displaypath = ശൂന്യ;
	ഘടന child_process cp_config = CHILD_PROCESS_INIT;
	ഘടന strbuf sb_config = STRBUF_INIT;
	അക്ഷരം *sub_git_dir = xstrfmt("%s/.git", path);

	എങ്കിൽ (validate_submodule_path(path) < 0)
		die(ശൂന്യ);

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);

	എങ്കിൽ (!sub || !sub->name)
		പോകുക cleanup;

	displaypath = get_submodule_displaypath(path, prefix, ശൂന്യ);

	/* remove the submodule work tree (unless the user already did it) */
	എങ്കിൽ (is_directory(path)) {
		ഘടന strbuf sb_rm = STRBUF_INIT;
		സ്ഥിരം അക്ഷരം *format;

		എങ്കിൽ (is_directory(sub_git_dir)) {
			എങ്കിൽ (!(flags & OPT_QUIET))
				warning(_("Submodule work tree '%s' contains a .git "
					  "directory. This will be replaced with a "
					  ".git file by using absorbgitdirs."),
					displaypath);

			absorb_git_dir_into_superproject(path, ശൂന്യ);

		}

		എങ്കിൽ (!(flags & OPT_FORCE)) {
			ഘടന child_process cp_rm = CHILD_PROCESS_INIT;

			cp_rm.git_cmd = 1;
			strvec_pushl(&cp_rm.args, "rm", "-qn",
				     path, ശൂന്യ);

			എങ്കിൽ (run_command(&cp_rm))
				die(_("Submodule work tree '%s' contains local "
				      "modifications; use '-f' to discard them"),
				      displaypath);
		}

		strbuf_addstr(&sb_rm, path);

		എങ്കിൽ (!remove_dir_recursively(&sb_rm, 0))
			format = _("Cleared directory '%s'\n");
		അല്ലെങ്കിൽ
			format = _("Could not remove submodule work tree '%s'\n");

		എങ്കിൽ (!(flags & OPT_QUIET))
			printf(format, displaypath);

		submodule_unset_core_worktree(sub);

		strbuf_release(&sb_rm);
	}

	എങ്കിൽ (mkdir(path, 0777))
		printf(_("could not create empty submodule directory %s"),
		      displaypath);

	cp_config.git_cmd = 1;
	strvec_pushl(&cp_config.args, "config", "--get-regexp", ശൂന്യ);
	strvec_pushf(&cp_config.args, "submodule.%s\\.", sub->name);

	/* remove the .git/config entries (unless the user already did it) */
	എങ്കിൽ (!capture_command(&cp_config, &sb_config, 0) && sb_config.len) {
		അക്ഷരം *sub_താക്കോൽ = xstrfmt("submodule.%s", sub->name);

		/*
		 * remove the whole section so we have a clean state when
		 * the user later decides to init this submodule again
		 */
		repo_config_rename_section_in_file(the_repository, ശൂന്യ, sub_താക്കോൽ, ശൂന്യ);
		എങ്കിൽ (!(flags & OPT_QUIET))
			printf(_("Submodule '%s' (%s) unregistered for path '%s'\n"),
				 sub->name, sub->url, displaypath);
		സ്വതന്ത്രം(sub_താക്കോൽ);
	}

cleanup:
	സ്വതന്ത്രം(displaypath);
	സ്വതന്ത്രം(sub_git_dir);
	strbuf_release(&sb_config);
}

സ്ഥിതം ശൂന്യം deinit_submodule_cb(സ്ഥിരം ഘടന cache_entry *list_item,
				ശൂന്യം *cb_data)
{
	ഘടന deinit_cb *info = cb_data;
	deinit_submodule(list_item->name, info->prefix, info->flags);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ module_deinit(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
			 ഘടന repository *repo UNUSED)
{
	ഘടന deinit_cb info = DEINIT_CB_INIT;
	ഘടന pathspec pathspec = { 0 };
	ഘടന module_list list = MODULE_LIST_INIT;
	പൂർണ്ണസംഖ്യ quiet = 0;
	പൂർണ്ണസംഖ്യ force = 0;
	പൂർണ്ണസംഖ്യ all = 0;
	ഘടന option module_deinit_options[] = {
		OPT__QUIET(&quiet, N_("suppress submodule status output")),
		OPT__FORCE(&force, N_("remove submodule working trees even if they contain local changes"), 0),
		OPT_BOOL(0, "all", &all, N_("unregister all submodules")),
		OPT_END()
	};
	സ്ഥിരം അക്ഷരം *സ്ഥിരം git_submodule_helper_usage[] = {
		N_("git submodule deinit [--quiet] [-f | --force] [--all | [--] [<path>...]]"),
		ശൂന്യ
	};
	പൂർണ്ണസംഖ്യ ret = 1;

	argc = parse_options(argc, argv, prefix, module_deinit_options,
			     git_submodule_helper_usage, 0);

	എങ്കിൽ (all && argc) {
		error("pathspec and --all are incompatible");
		usage_with_options(git_submodule_helper_usage,
				   module_deinit_options);
	}

	എങ്കിൽ (!argc && !all)
		die(_("Use '--all' if you really want to deinitialize all submodules"));

	എങ്കിൽ (module_list_compute(argv, prefix, &pathspec, &list) < 0)
		പോകുക cleanup;

	info.prefix = prefix;
	എങ്കിൽ (quiet)
		info.flags |= OPT_QUIET;
	എങ്കിൽ (force)
		info.flags |= OPT_FORCE;

	for_each_listed_submodule(&list, deinit_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	തിരികെ ret;
}

ഘടന module_clone_data {
	സ്ഥിരം അക്ഷരം *prefix;
	സ്ഥിരം അക്ഷരം *path;
	സ്ഥിരം അക്ഷരം *name;
	സ്ഥിരം അക്ഷരം *url;
	പൂർണ്ണസംഖ്യ depth;
	ഘടന list_objects_filter_options *filter_options;
	എണ്ണൽ ref_storage_format ref_storage_format;
	unsigned പൂർണ്ണസംഖ്യ quiet: 1;
	unsigned പൂർണ്ണസംഖ്യ progress: 1;
	unsigned പൂർണ്ണസംഖ്യ dissociate: 1;
	unsigned പൂർണ്ണസംഖ്യ require_init: 1;
	പൂർണ്ണസംഖ്യ single_branch;
};
#define MODULE_CLONE_DATA_INIT { \
	.single_branch = -1, \
	.ref_storage_format = REF_STORAGE_FORMAT_UNKNOWN, \
}

ഘടന submodule_alternate_setup {
	സ്ഥിരം അക്ഷരം *submodule_name;
	എണ്ണൽ SUBMODULE_ALTERNATE_ERROR_MODE {
		SUBMODULE_ALTERNATE_ERROR_DIE,
		SUBMODULE_ALTERNATE_ERROR_INFO,
		SUBMODULE_ALTERNATE_ERROR_IGNORE
	} error_mode;
	ഘടന string_list *reference;
};
#define SUBMODULE_ALTERNATE_SETUP_INIT { \
	.error_mode = SUBMODULE_ALTERNATE_ERROR_IGNORE, \
}

സ്ഥിതം സ്ഥിരം അക്ഷരം alternate_error_advice[] = N_(
"An alternate computed from a superproject's alternate is invalid.\n"
"To allow Git to clone without an alternate in such a case, set\n"
"submodule.alternateErrorStrategy to 'info' or, equivalently, clone with\n"
"'--reference-if-able' instead of '--reference'."
);

സ്ഥിതം പൂർണ്ണസംഖ്യ add_possible_reference_from_superproject(
		ഘടന odb_files_dir *alt_odb, ശൂന്യം *sas_cb)
{
	ഘടന submodule_alternate_setup *sas = sas_cb;
	size_t len;

	/*
	 * If the alternate object store is another repository, try the
	 * standard layout with .git/(modules/<name>)+/objects
	 */
	എങ്കിൽ (strip_suffix(alt_odb->abspath, "/objects", &len)) {
		ഘടന repository alternate;
		അക്ഷരം *sm_alternate;
		ഘടന strbuf sb = STRBUF_INIT;
		ഘടന strbuf err = STRBUF_INIT;
		strbuf_add(&sb, alt_odb->abspath, len);

		എങ്കിൽ (repo_init(&alternate, sb.buf, ശൂന്യ) < 0)
			die(_("could not get a repository handle for gitdir '%s'"),
			    sb.buf);

		/*
		 * We need to end the new path with '/' to mark it as a dir,
		 * otherwise a submodule name containing '/' will be broken
		 * as the last part of a missing submodule reference would
		 * be taken as a file name.
		 */
		strbuf_reset(&sb);
		submodule_name_to_gitdir(&sb, &alternate, sas->submodule_name);
		strbuf_addch(&sb, '/');
		repo_clear(&alternate);

		sm_alternate = compute_alternate_path(sb.buf, &err);
		എങ്കിൽ (sm_alternate) {
			അക്ഷരം *p = strbuf_detach(&sb, ശൂന്യ);

			string_list_append(sas->reference, p)->util = p;
			സ്വതന്ത്രം(sm_alternate);
		} അല്ലെങ്കിൽ {
			തിരഞ്ഞെടുപ്പ് (sas->error_mode) {
			അവസ്ഥ SUBMODULE_ALTERNATE_ERROR_DIE:
				എങ്കിൽ (advice_enabled(ADVICE_SUBMODULE_ALTERNATE_ERROR_STRATEGY_DIE))
					advise(_(alternate_error_advice));
				die(_("submodule '%s' cannot add alternate: %s"),
				    sas->submodule_name, err.buf);
			അവസ്ഥ SUBMODULE_ALTERNATE_ERROR_INFO:
				fprintf_ln(stderr, _("submodule '%s' cannot add alternate: %s"),
					sas->submodule_name, err.buf);
			അവസ്ഥ SUBMODULE_ALTERNATE_ERROR_IGNORE:
				; /* nothing */
			}
		}

		strbuf_release(&err);
		strbuf_release(&sb);
	}

	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം prepare_possible_alternates(സ്ഥിരം അക്ഷരം *sm_name,
		ഘടന string_list *reference)
{
	അക്ഷരം *sm_alternate = ശൂന്യ, *error_strategy = ശൂന്യ;
	ഘടന submodule_alternate_setup sas = SUBMODULE_ALTERNATE_SETUP_INIT;

	repo_config_get_string(the_repository, "submodule.alternateLocation", &sm_alternate);
	എങ്കിൽ (!sm_alternate)
		തിരികെ;

	repo_config_get_string(the_repository, "submodule.alternateErrorStrategy", &error_strategy);

	എങ്കിൽ (!error_strategy)
		error_strategy = xstrdup("die");

	sas.submodule_name = sm_name;
	sas.reference = reference;
	എങ്കിൽ (!strcmp(error_strategy, "die"))
		sas.error_mode = SUBMODULE_ALTERNATE_ERROR_DIE;
	അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(error_strategy, "info"))
		sas.error_mode = SUBMODULE_ALTERNATE_ERROR_INFO;
	അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(error_strategy, "ignore"))
		sas.error_mode = SUBMODULE_ALTERNATE_ERROR_IGNORE;
	അല്ലെങ്കിൽ
		die(_("Value '%s' for submodule.alternateErrorStrategy is not recognized"), error_strategy);

	എങ്കിൽ (!strcmp(sm_alternate, "superproject"))
		odb_for_each_alternate(the_repository->objects,
				       add_possible_reference_from_superproject, &sas);
	അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(sm_alternate, "no"))
		; /* do nothing */
	അല്ലെങ്കിൽ
		die(_("Value '%s' for submodule.alternateLocation is not recognized"), sm_alternate);

	സ്വതന്ത്രം(sm_alternate);
	സ്വതന്ത്രം(error_strategy);
}

സ്ഥിതം അക്ഷരം *clone_submodule_sm_gitdir(സ്ഥിരം അക്ഷരം *name)
{
	ഘടന strbuf sb = STRBUF_INIT;
	അക്ഷരം *sm_gitdir;

	submodule_name_to_gitdir(&sb, the_repository, name);
	sm_gitdir = absolute_pathdup(sb.buf);
	strbuf_release(&sb);

	തിരികെ sm_gitdir;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ dir_contains_only_dotgit(സ്ഥിരം അക്ഷരം *path)
{
	DIR *dir = opendir(path);
	ഘടന dirent *e;
	പൂർണ്ണസംഖ്യ ret = 1;

	എങ്കിൽ (!dir)
		തിരികെ 0;

	e = readdir_skip_dot_and_dotdot(dir);
	എങ്കിൽ (!e)
		ret = 0;
	അല്ലെങ്കിൽ എങ്കിൽ (strcmp(DEFAULT_GIT_DIR_ENVIRONMENT, e->d_name) ||
		 (e = readdir_skip_dot_and_dotdot(dir))) {
		error("unexpected item '%s' in '%s'", e->d_name, path);
		ret = 0;
	}

	closedir(dir);
	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ clone_submodule(സ്ഥിരം ഘടന module_clone_data *clone_data,
			   ഘടന string_list *reference)
{
	അക്ഷരം *p;
	അക്ഷരം *sm_gitdir = clone_submodule_sm_gitdir(clone_data->name);
	അക്ഷരം *sm_alternate = ശൂന്യ, *error_strategy = ശൂന്യ;
	ഘടന stat st;
	ഘടന child_process cp = CHILD_PROCESS_INIT;
	സ്ഥിരം അക്ഷരം *clone_data_path = clone_data->path;
	അക്ഷരം *to_free = ശൂന്യ;

	എങ്കിൽ (validate_submodule_path(clone_data_path) < 0)
		die(ശൂന്യ);

	എങ്കിൽ (!is_absolute_path(clone_data->path))
		clone_data_path = to_free = xstrfmt("%s/%s", repo_get_work_വൃക്ഷം(the_repository),
						    clone_data->path);

	എങ്കിൽ (!file_exists(sm_gitdir)) {
		എങ്കിൽ (clone_data->require_init && !stat(clone_data_path, &st) &&
		    !is_empty_dir(clone_data_path))
			die(_("directory not empty: '%s'"), clone_data_path);

		എങ്കിൽ (safe_create_leading_directories_const(the_repository, sm_gitdir) < 0)
			die(_("could not create directory '%s'"), sm_gitdir);

		prepare_possible_alternates(clone_data->name, reference);

		strvec_തള്ളുക(&cp.args, "clone");
		strvec_തള്ളുക(&cp.args, "--no-checkout");
		എങ്കിൽ (clone_data->quiet)
			strvec_തള്ളുക(&cp.args, "--quiet");
		എങ്കിൽ (clone_data->progress)
			strvec_തള്ളുക(&cp.args, "--progress");
		എങ്കിൽ (clone_data->depth > 0)
			strvec_pushf(&cp.args, "--depth=%d", clone_data->depth);
		എങ്കിൽ (reference->nr) {
			ഘടന string_list_item *item;

			for_each_string_list_item(item, reference)
				strvec_pushl(&cp.args, "--reference",
					     item->string, ശൂന്യ);
		}
		എങ്കിൽ (clone_data->ref_storage_format != REF_STORAGE_FORMAT_UNKNOWN)
			strvec_pushf(&cp.args, "--ref-storage-format=%s",
				     ref_storage_format_to_name(clone_data->ref_storage_format));
		എങ്കിൽ (clone_data->dissociate)
			strvec_തള്ളുക(&cp.args, "--dissociate");
		എങ്കിൽ (sm_gitdir && *sm_gitdir)
			strvec_pushl(&cp.args, "--separate-git-dir", sm_gitdir, ശൂന്യ);
		എങ്കിൽ (clone_data->filter_options && clone_data->filter_options->choice)
			strvec_pushf(&cp.args, "--filter=%s",
				     expand_list_objects_filter_spec(
					     clone_data->filter_options));
		എങ്കിൽ (clone_data->single_branch >= 0)
			strvec_തള്ളുക(&cp.args, clone_data->single_branch ?
				    "--single-branch" :
				    "--no-single-branch");

		strvec_തള്ളുക(&cp.args, "--");
		strvec_തള്ളുക(&cp.args, clone_data->url);
		strvec_തള്ളുക(&cp.args, clone_data_path);

		cp.git_cmd = 1;
		prepare_submodule_repo_env(&cp.env);
		cp.no_stdin = 1;

		എങ്കിൽ(run_command(&cp))
			die(_("clone of '%s' into submodule path '%s' failed"),
			    clone_data->url, clone_data_path);

		എങ്കിൽ (clone_data->require_init && !stat(clone_data_path, &st) &&
		    !dir_contains_only_dotgit(clone_data_path)) {
			അക്ഷരം *dot_git = xstrfmt("%s/.git", clone_data_path);
			unlink(dot_git);
			സ്വതന്ത്രം(dot_git);
			die(_("directory not empty: '%s'"), clone_data_path);
		}
	} അല്ലെങ്കിൽ {
		അക്ഷരം *path;

		എങ്കിൽ (clone_data->require_init && !stat(clone_data_path, &st) &&
		    !is_empty_dir(clone_data_path))
			die(_("directory not empty: '%s'"), clone_data_path);
		എങ്കിൽ (safe_create_leading_directories_const(the_repository, clone_data_path) < 0)
			die(_("could not create directory '%s'"), clone_data_path);
		path = xstrfmt("%s/index", sm_gitdir);
		unlink_or_warn(path);
		സ്വതന്ത്രം(path);
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
	എങ്കിൽ (validate_submodule_git_dir(sm_gitdir, clone_data->name) < 0) {
		അക്ഷരം *head = xstrfmt("%s/HEAD", sm_gitdir);
		unlink(head);
		സ്വതന്ത്രം(head);
		die(_("refusing to create/use '%s' in another submodule's git dir. "
		      "Enabling extensions.submodulePathConfig should fix this."),
		    sm_gitdir);
	}

	connect_work_വൃക്ഷം_and_git_dir(clone_data_path, sm_gitdir, 0);

	p = repo_submodule_path(the_repository, clone_data_path, "config");
	എങ്കിൽ (!p)
		die(_("could not get submodule directory for '%s'"), clone_data_path);

	/* setup alternateLocation and alternateErrorStrategy in the cloned submodule if needed */
	repo_config_get_string(the_repository, "submodule.alternateLocation", &sm_alternate);
	എങ്കിൽ (sm_alternate)
		repo_config_set_in_file(the_repository, p, "submodule.alternateLocation",
					sm_alternate);
	repo_config_get_string(the_repository, "submodule.alternateErrorStrategy", &error_strategy);
	എങ്കിൽ (error_strategy)
		repo_config_set_in_file(the_repository, p, "submodule.alternateErrorStrategy",
					error_strategy);

	സ്വതന്ത്രം(sm_alternate);
	സ്വതന്ത്രം(error_strategy);

	സ്വതന്ത്രം(sm_gitdir);
	സ്വതന്ത്രം(p);
	സ്വതന്ത്രം(to_free);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ module_clone(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
			ഘടന repository *repo UNUSED)
{
	പൂർണ്ണസംഖ്യ dissociate = 0, quiet = 0, progress = 0, require_init = 0;
	ഘടന module_clone_data clone_data = MODULE_CLONE_DATA_INIT;
	ഘടന string_list reference = STRING_LIST_INIT_NODUP;
	ഘടന list_objects_filter_options filter_options =
		LIST_OBJECTS_FILTER_INIT;
	സ്ഥിരം അക്ഷരം *ref_storage_format = ശൂന്യ;

	ഘടന option module_clone_options[] = {
		OPT_STRING(0, "prefix", &clone_data.prefix,
			   N_("path"),
			   N_("alternative anchor for relative paths")),
		OPT_STRING(0, "path", &clone_data.path,
			   N_("path"),
			   N_("where the new submodule will be cloned to")),
		OPT_STRING(0, "name", &clone_data.name,
			   N_("string"),
			   N_("name of the new submodule")),
		OPT_STRING(0, "url", &clone_data.url,
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
		OPT_INTEGER(0, "depth", &clone_data.depth,
			   N_("depth for shallow clones")),
		OPT__QUIET(&quiet, "suppress output for cloning a submodule"),
		OPT_BOOL(0, "progress", &progress,
			   N_("force cloning progress")),
		OPT_BOOL(0, "require-init", &require_init,
			   N_("disallow cloning into non-empty directory")),
		OPT_BOOL(0, "single-branch", &clone_data.single_branch,
			 N_("clone only one branch, HEAD or --branch")),
		OPT_PARSE_LIST_OBJECTS_FILTER(&filter_options),
		OPT_END()
	};
	സ്ഥിരം അക്ഷരം *സ്ഥിരം git_submodule_helper_usage[] = {
		N_("git submodule--helper clone [--prefix=<path>] [--quiet] "
		   "[--reference <repository>] [--name <name>] [--depth <depth>] "
		   "[--single-branch] [--filter <filter-spec>] "
		   "--url <url> --path <path>"),
		ശൂന്യ
	};

	argc = parse_options(argc, argv, prefix, module_clone_options,
			     git_submodule_helper_usage, 0);

	എങ്കിൽ (ref_storage_format) {
		clone_data.ref_storage_format = ref_storage_format_by_name(ref_storage_format);
		എങ്കിൽ (clone_data.ref_storage_format == REF_STORAGE_FORMAT_UNKNOWN)
			die(_("unknown ref storage format '%s'"), ref_storage_format);
	}
	clone_data.dissociate = !!dissociate;
	clone_data.quiet = !!quiet;
	clone_data.progress = !!progress;
	clone_data.require_init = !!require_init;
	clone_data.filter_options = &filter_options;

	എങ്കിൽ (argc || !clone_data.url || !clone_data.path || !*(clone_data.path))
		usage_with_options(git_submodule_helper_usage,
				   module_clone_options);

	clone_submodule(&clone_data, &reference);
	list_objects_filter_release(&filter_options);
	string_list_clear(&reference, 1);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ determine_submodule_update_strategy(ഘടന repository *r,
					       പൂർണ്ണസംഖ്യ just_cloned,
					       സ്ഥിരം അക്ഷരം *path,
					       എണ്ണൽ submodule_update_type update,
					       ഘടന submodule_update_strategy *out)
{
	സ്ഥിരം ഘടന submodule *sub = submodule_from_path(r, null_oid(the_hash_algo), path);
	അക്ഷരം *താക്കോൽ;
	സ്ഥിരം അക്ഷരം *val;
	പൂർണ്ണസംഖ്യ ret;

	/*
	 * NEEDSWORK: audit and ensure that update_submodule() has right
	 * to assume that submodule_from_path() above will always succeed.
	 */
	എങ്കിൽ (!sub)
		BUG("update_submodule assumes a submodule exists at path (%s)",
		    path);
	താക്കോൽ = xstrfmt("submodule.%s.update", sub->name);

	എങ്കിൽ (update) {
		out->type = update;
	} അല്ലെങ്കിൽ എങ്കിൽ (!repo_config_get_string_tmp(r, താക്കോൽ, &val)) {
		എങ്കിൽ (parse_submodule_update_strategy(val, out) < 0) {
			ret = die_message(_("Invalid update mode '%s' configured for submodule path '%s'"),
					  val, path);
			പോകുക cleanup;
		}
	} അല്ലെങ്കിൽ എങ്കിൽ (sub->update_strategy.type != SM_UPDATE_UNSPECIFIED) {
		എങ്കിൽ (sub->update_strategy.type == SM_UPDATE_COMMAND)
			BUG("how did we read update = !command from .gitmodules?");
		out->type = sub->update_strategy.type;
		out->command = sub->update_strategy.command;
	} അല്ലെങ്കിൽ
		out->type = SM_UPDATE_CHECKOUT;

	എങ്കിൽ (just_cloned &&
	    (out->type == SM_UPDATE_MERGE ||
	     out->type == SM_UPDATE_REBASE ||
	     out->type == SM_UPDATE_NONE))
		out->type = SM_UPDATE_CHECKOUT;

	ret = 0;
cleanup:
	സ്വതന്ത്രം(താക്കോൽ);
	തിരികെ ret;
}

ഘടന update_clone_data {
	സ്ഥിരം ഘടന submodule *sub;
	ഘടന object_id oid;
	unsigned just_cloned;
};

ഘടന submodule_update_clone {
	/* index into 'update_data.list', the list of submodules to look into for cloning */
	പൂർണ്ണസംഖ്യ current;

	/* configuration parameters which are passed on to the children */
	സ്ഥിരം ഘടന update_data *update_data;

	/* to be consumed by update_submodule() */
	ഘടന update_clone_data *update_clone;
	പൂർണ്ണസംഖ്യ update_clone_nr; പൂർണ്ണസംഖ്യ update_clone_alloc;

	/* If we want to stop as fast as possible and return an error */
	unsigned quickstop : 1;

	/* failed clones to be retried again */
	സ്ഥിരം ഘടന cache_entry **failed_clones;
	പൂർണ്ണസംഖ്യ failed_clones_nr, failed_clones_alloc;
};
#define SUBMODULE_UPDATE_CLONE_INIT { 0 }

സ്ഥിതം ശൂന്യം submodule_update_clone_release(ഘടന submodule_update_clone *suc)
{
	സ്വതന്ത്രം(suc->update_clone);
	സ്വതന്ത്രം(suc->failed_clones);
}

ഘടന update_data {
	സ്ഥിരം അക്ഷരം *prefix;
	സ്ഥിരം അക്ഷരം *super_prefix;
	അക്ഷരം *displaypath;
	എണ്ണൽ submodule_update_type update_default;
	ഘടന object_id suboid;
	ഘടന string_list references;
	ഘടന submodule_update_strategy update_strategy;
	ഘടന list_objects_filter_options *filter_options;
	ഘടന module_list list;
	എണ്ണൽ ref_storage_format ref_storage_format;
	പൂർണ്ണസംഖ്യ depth;
	പൂർണ്ണസംഖ്യ max_jobs;
	പൂർണ്ണസംഖ്യ single_branch;
	പൂർണ്ണസംഖ്യ recommend_shallow;
	unsigned പൂർണ്ണസംഖ്യ require_init;
	unsigned പൂർണ്ണസംഖ്യ force;
	unsigned പൂർണ്ണസംഖ്യ quiet;
	unsigned പൂർണ്ണസംഖ്യ nofetch;
	unsigned പൂർണ്ണസംഖ്യ remote;
	unsigned പൂർണ്ണസംഖ്യ progress;
	unsigned പൂർണ്ണസംഖ്യ dissociate;
	unsigned പൂർണ്ണസംഖ്യ init;
	unsigned പൂർണ്ണസംഖ്യ warn_if_uninitialized;
	unsigned പൂർണ്ണസംഖ്യ recursive;

	/* copied over from update_clone_data */
	ഘടന object_id oid;
	unsigned പൂർണ്ണസംഖ്യ just_cloned;
	സ്ഥിരം അക്ഷരം *sm_path;
};
#define UPDATE_DATA_INIT { \
	.update_strategy = SUBMODULE_UPDATE_STRATEGY_INIT, \
	.list = MODULE_LIST_INIT, \
	.ref_storage_format = REF_STORAGE_FORMAT_UNKNOWN, \
	.recommend_shallow = -1, \
	.references = STRING_LIST_INIT_DUP, \
	.single_branch = -1, \
	.max_jobs = 1, \
}

സ്ഥിതം ശൂന്യം update_data_release(ഘടന update_data *ud)
{
	സ്വതന്ത്രം(ud->displaypath);
	submodule_update_strategy_release(&ud->update_strategy);
	module_list_release(&ud->list);
}

സ്ഥിതം ശൂന്യം next_submodule_warn_missing(ഘടന submodule_update_clone *suc,
		ഘടന strbuf *out, സ്ഥിരം അക്ഷരം *displaypath)
{
	/*
	 * Only mention uninitialized submodules when their
	 * paths have been specified.
	 */
	എങ്കിൽ (suc->update_data->warn_if_uninitialized) {
		strbuf_addf(out,
			_("Submodule path '%s' not initialized"),
			displaypath);
		strbuf_addch(out, '\n');
		strbuf_addstr(out,
			_("Maybe you want to use 'update --init'?"));
		strbuf_addch(out, '\n');
	}
}

/**
 * Determine whether 'ce' needs to be cloned. If so, prepare the 'child' to
 * run the clone. Returns 1 if 'ce' needs to be cloned, 0 otherwise.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ prepare_to_clone_next_submodule(സ്ഥിരം ഘടന cache_entry *ce,
					   ഘടന child_process *child,
					   ഘടന submodule_update_clone *suc,
					   ഘടന strbuf *out)
{
	സ്ഥിരം ഘടന submodule *sub = ശൂന്യ;
	സ്ഥിരം അക്ഷരം *url = ശൂന്യ;
	സ്ഥിരം അക്ഷരം *update_string;
	എണ്ണൽ submodule_update_type update_type;
	അക്ഷരം *താക്കോൽ;
	സ്ഥിരം ഘടന update_data *ud = suc->update_data;
	അക്ഷരം *displaypath = get_submodule_displaypath(ce->name, ud->prefix,
						      ud->super_prefix);
	ഘടന strbuf sb = STRBUF_INIT;
	പൂർണ്ണസംഖ്യ needs_cloning = 0;
	പൂർണ്ണസംഖ്യ need_free_url = 0;

	എങ്കിൽ (ce_stage(ce)) {
		strbuf_addf(out, _("Skipping unmerged submodule %s"), displaypath);
		strbuf_addch(out, '\n');
		പോകുക cleanup;
	}

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), ce->name);

	എങ്കിൽ (!sub) {
		next_submodule_warn_missing(suc, out, displaypath);
		പോകുക cleanup;
	}

	താക്കോൽ = xstrfmt("submodule.%s.update", sub->name);
	എങ്കിൽ (!repo_config_get_string_tmp(the_repository, താക്കോൽ, &update_string)) {
		update_type = parse_submodule_update_type(update_string);
	} അല്ലെങ്കിൽ {
		update_type = sub->update_strategy.type;
	}
	സ്വതന്ത്രം(താക്കോൽ);

	എങ്കിൽ (suc->update_data->update_strategy.type == SM_UPDATE_NONE
	    || (suc->update_data->update_strategy.type == SM_UPDATE_UNSPECIFIED
		&& update_type == SM_UPDATE_NONE)) {
		strbuf_addf(out, _("Skipping submodule '%s'"), displaypath);
		strbuf_addch(out, '\n');
		പോകുക cleanup;
	}

	/* Check if the submodule has been initialized. */
	എങ്കിൽ (!is_submodule_active(the_repository, ce->name)) {
		next_submodule_warn_missing(suc, out, displaypath);
		പോകുക cleanup;
	}

	strbuf_reset(&sb);
	strbuf_addf(&sb, "submodule.%s.url", sub->name);
	എങ്കിൽ (repo_config_get_string_tmp(the_repository, sb.buf, &url)) {
		എങ്കിൽ (sub->url && (starts_with_dot_slash(sub->url) ||
				 starts_with_dot_dot_slash(sub->url))) {
			url = resolve_relative_url(sub->url, ശൂന്യ, 0);
			need_free_url = 1;
		} അല്ലെങ്കിൽ
			url = sub->url;
	}

	എങ്കിൽ (!url)
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

	എങ്കിൽ (!needs_cloning)
		പോകുക cleanup;

	child->git_cmd = 1;
	child->no_stdin = 1;
	child->stdout_to_stderr = 1;
	child->err = -1;
	strvec_തള്ളുക(&child->args, "submodule--helper");
	strvec_തള്ളുക(&child->args, "clone");
	എങ്കിൽ (suc->update_data->progress)
		strvec_തള്ളുക(&child->args, "--progress");
	എങ്കിൽ (suc->update_data->quiet)
		strvec_തള്ളുക(&child->args, "--quiet");
	എങ്കിൽ (suc->update_data->prefix)
		strvec_pushl(&child->args, "--prefix", suc->update_data->prefix, ശൂന്യ);
	എങ്കിൽ (suc->update_data->recommend_shallow && sub->recommend_shallow == 1)
		strvec_തള്ളുക(&child->args, "--depth=1");
	അല്ലെങ്കിൽ എങ്കിൽ (suc->update_data->depth)
		strvec_pushf(&child->args, "--depth=%d", suc->update_data->depth);
	എങ്കിൽ (suc->update_data->filter_options && suc->update_data->filter_options->choice)
		strvec_pushf(&child->args, "--filter=%s",
			     expand_list_objects_filter_spec(suc->update_data->filter_options));
	എങ്കിൽ (suc->update_data->require_init)
		strvec_തള്ളുക(&child->args, "--require-init");
	എങ്കിൽ (suc->update_data->ref_storage_format != REF_STORAGE_FORMAT_UNKNOWN)
		strvec_pushf(&child->args, "--ref-storage-format=%s",
			     ref_storage_format_to_name(suc->update_data->ref_storage_format));
	strvec_pushl(&child->args, "--path", sub->path, ശൂന്യ);
	strvec_pushl(&child->args, "--name", sub->name, ശൂന്യ);
	strvec_pushl(&child->args, "--url", url, ശൂന്യ);
	എങ്കിൽ (suc->update_data->references.nr) {
		ഘടന string_list_item *item;

		for_each_string_list_item(item, &suc->update_data->references)
			strvec_pushl(&child->args, "--reference", item->string, ശൂന്യ);
	}
	എങ്കിൽ (suc->update_data->dissociate)
		strvec_തള്ളുക(&child->args, "--dissociate");
	എങ്കിൽ (suc->update_data->single_branch >= 0)
		strvec_തള്ളുക(&child->args, suc->update_data->single_branch ?
					      "--single-branch" :
					      "--no-single-branch");

cleanup:
	സ്വതന്ത്രം(displaypath);
	strbuf_release(&sb);
	എങ്കിൽ (need_free_url)
		സ്വതന്ത്രം((ശൂന്യം*)url);

	തിരികെ needs_cloning;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ update_clone_get_next_task(ഘടന child_process *child,
				      ഘടന strbuf *err,
				      ശൂന്യം *suc_cb,
				      ശൂന്യം **idx_task_cb)
{
	ഘടന submodule_update_clone *suc = suc_cb;
	സ്ഥിരം ഘടന cache_entry *ce;
	പൂർണ്ണസംഖ്യ index;

	വേണ്ടി (; suc->current < suc->update_data->list.nr; suc->current++) {
		ce = suc->update_data->list.entries[suc->current];
		എങ്കിൽ (prepare_to_clone_next_submodule(ce, child, suc, err)) {
			പൂർണ്ണസംഖ്യ *p = xmalloc(വലുപ്പം(*p));

			*p = suc->current;
			*idx_task_cb = p;
			suc->current++;
			തിരികെ 1;
		}
	}

	/*
	 * The loop above tried cloning each submodule once, now try the
	 * stragglers again, which we can imagine as an extension of the
	 * entry list.
	 */
	index = suc->current - suc->update_data->list.nr;
	എങ്കിൽ (index < suc->failed_clones_nr) {
		പൂർണ്ണസംഖ്യ *p;

		ce = suc->failed_clones[index];
		എങ്കിൽ (!prepare_to_clone_next_submodule(ce, child, suc, err)) {
			suc->current ++;
			strbuf_addstr(err, "BUG: submodule considered for "
					   "cloning, doesn't need cloning "
					   "any more?\n");
			തിരികെ 0;
		}
		p = xmalloc(വലുപ്പം(*p));
		*p = suc->current;
		*idx_task_cb = p;
		suc->current ++;
		തിരികെ 1;
	}

	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ update_clone_start_failure(ഘടന strbuf *err UNUSED,
				      ശൂന്യം *suc_cb,
				      ശൂന്യം *idx_task_cb UNUSED)
{
	ഘടന submodule_update_clone *suc = suc_cb;

	suc->quickstop = 1;
	തിരികെ 1;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ update_clone_task_finished(പൂർണ്ണസംഖ്യ result,
				      ഘടന strbuf *err,
				      ശൂന്യം *suc_cb,
				      ശൂന്യം *idx_task_cb)
{
	സ്ഥിരം ഘടന cache_entry *ce;
	ഘടന submodule_update_clone *suc = suc_cb;
	പൂർണ്ണസംഖ്യ *idxP = idx_task_cb;
	പൂർണ്ണസംഖ്യ idx = *idxP;

	സ്വതന്ത്രം(idxP);

	എങ്കിൽ (!result)
		തിരികെ 0;

	എങ്കിൽ (idx < suc->update_data->list.nr) {
		ce  = suc->update_data->list.entries[idx];
		strbuf_addf(err, _("Failed to clone '%s'. Retry scheduled"),
			    ce->name);
		strbuf_addch(err, '\n');
		ALLOC_GROW(suc->failed_clones,
			   suc->failed_clones_nr + 1,
			   suc->failed_clones_alloc);
		suc->failed_clones[suc->failed_clones_nr++] = ce;
		തിരികെ 0;
	} അല്ലെങ്കിൽ {
		idx -= suc->update_data->list.nr;
		ce  = suc->failed_clones[idx];
		strbuf_addf(err, _("Failed to clone '%s' a second time, aborting"),
			    ce->name);
		strbuf_addch(err, '\n');
		suc->quickstop = 1;
		തിരികെ 1;
	}

	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ git_update_clone_config(സ്ഥിരം അക്ഷരം *var, സ്ഥിരം അക്ഷരം *മൂല്യം,
				   സ്ഥിരം ഘടന config_context *ctx,
				   ശൂന്യം *cb)
{
	പൂർണ്ണസംഖ്യ *max_jobs = cb;

	എങ്കിൽ (!strcmp(var, "submodule.fetchjobs"))
		*max_jobs = parse_submodule_fetchjobs(var, മൂല്യം, ctx->kvi);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ is_tip_reachable(സ്ഥിരം അക്ഷരം *path, സ്ഥിരം ഘടന object_id *oid)
{
	ഘടന child_process cp = CHILD_PROCESS_INIT;
	ഘടന strbuf rev = STRBUF_INIT;
	അക്ഷരം *hex = oid_to_hex(oid);
	പൂർണ്ണസംഖ്യ reachable;

	cp.git_cmd = 1;
	cp.dir = path;
	cp.no_stderr = 1;
	strvec_pushl(&cp.args, "rev-list", "-n", "1", hex, "--not", "--all", ശൂന്യ);

	prepare_submodule_repo_env(&cp.env);

	എങ്കിൽ (capture_command(&cp, &rev, GIT_MAX_HEXSZ + 1) || rev.len)
		reachable = 0;
	അല്ലെങ്കിൽ
		reachable = 1;

	strbuf_release(&rev);
	തിരികെ reachable;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ fetch_in_submodule(സ്ഥിരം അക്ഷരം *module_path, പൂർണ്ണസംഖ്യ depth, പൂർണ്ണസംഖ്യ quiet,
			      സ്ഥിരം ഘടന object_id *oid)
{
	ഘടന child_process cp = CHILD_PROCESS_INIT;

	prepare_submodule_repo_env(&cp.env);
	cp.git_cmd = 1;
	cp.dir = module_path;

	strvec_തള്ളുക(&cp.args, "fetch");
	എങ്കിൽ (quiet)
		strvec_തള്ളുക(&cp.args, "--quiet");
	എങ്കിൽ (depth)
		strvec_pushf(&cp.args, "--depth=%d", depth);
	എങ്കിൽ (oid) {
		അക്ഷരം *hex = oid_to_hex(oid);
		അക്ഷരം *remote;
		പൂർണ്ണസംഖ്യ code;

		code = get_default_remote_submodule(module_path, &remote);
		എങ്കിൽ (code) {
			child_process_clear(&cp);
			തിരികെ code;
		}

		strvec_pushl(&cp.args, remote, hex, ശൂന്യ);
		സ്വതന്ത്രം(remote);
	}

	തിരികെ run_command(&cp);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ run_update_command(സ്ഥിരം ഘടന update_data *ud, പൂർണ്ണസംഖ്യ subforce)
{
	ഘടന child_process cp = CHILD_PROCESS_INIT;
	അക്ഷരം *oid = oid_to_hex(&ud->oid);
	പൂർണ്ണസംഖ്യ ret;

	തിരഞ്ഞെടുപ്പ് (ud->update_strategy.type) {
	അവസ്ഥ SM_UPDATE_CHECKOUT:
		cp.git_cmd = 1;
		strvec_pushl(&cp.args, "checkout", "-q", ശൂന്യ);
		എങ്കിൽ (subforce)
			strvec_തള്ളുക(&cp.args, "-f");
		നിർത്തുക;
	അവസ്ഥ SM_UPDATE_REBASE:
		cp.git_cmd = 1;
		strvec_തള്ളുക(&cp.args, "rebase");
		എങ്കിൽ (ud->quiet)
			strvec_തള്ളുക(&cp.args, "--quiet");
		നിർത്തുക;
	അവസ്ഥ SM_UPDATE_MERGE:
		cp.git_cmd = 1;
		strvec_തള്ളുക(&cp.args, "merge");
		എങ്കിൽ (ud->quiet)
			strvec_തള്ളുക(&cp.args, "--quiet");
		നിർത്തുക;
	അവസ്ഥ SM_UPDATE_COMMAND:
		cp.use_shell = 1;
		strvec_തള്ളുക(&cp.args, ud->update_strategy.command);
		നിർത്തുക;
	സ്ഥിരസ്ഥിതി:
		BUG("unexpected update strategy type: %d",
		    ud->update_strategy.type);
	}
	strvec_തള്ളുക(&cp.args, oid);

	cp.dir = ud->sm_path;
	prepare_submodule_repo_env(&cp.env);
	എങ്കിൽ ((ret = run_command(&cp))) {
		തിരഞ്ഞെടുപ്പ് (ud->update_strategy.type) {
		അവസ്ഥ SM_UPDATE_CHECKOUT:
			die_message(_("Unable to checkout '%s' in submodule path '%s'"),
				    oid, ud->displaypath);
			/* No "ret" assignment, use "git checkout"'s */
			നിർത്തുക;
		അവസ്ഥ SM_UPDATE_REBASE:
			ret = die_message(_("Unable to rebase '%s' in submodule path '%s'"),
					  oid, ud->displaypath);
			നിർത്തുക;
		അവസ്ഥ SM_UPDATE_MERGE:
			ret = die_message(_("Unable to merge '%s' in submodule path '%s'"),
					  oid, ud->displaypath);
			നിർത്തുക;
		അവസ്ഥ SM_UPDATE_COMMAND:
			ret = die_message(_("Execution of '%s %s' failed in submodule path '%s'"),
					  ud->update_strategy.command, oid, ud->displaypath);
			നിർത്തുക;
		സ്ഥിരസ്ഥിതി:
			BUG("unexpected update strategy type: %d",
			    ud->update_strategy.type);
		}

		തിരികെ ret;
	}

	എങ്കിൽ (ud->quiet)
		തിരികെ 0;

	തിരഞ്ഞെടുപ്പ് (ud->update_strategy.type) {
	അവസ്ഥ SM_UPDATE_CHECKOUT:
		printf(_("Submodule path '%s': checked out '%s'\n"),
		       ud->displaypath, oid);
		നിർത്തുക;
	അവസ്ഥ SM_UPDATE_REBASE:
		printf(_("Submodule path '%s': rebased into '%s'\n"),
		       ud->displaypath, oid);
		നിർത്തുക;
	അവസ്ഥ SM_UPDATE_MERGE:
		printf(_("Submodule path '%s': merged in '%s'\n"),
		       ud->displaypath, oid);
		നിർത്തുക;
	അവസ്ഥ SM_UPDATE_COMMAND:
		printf(_("Submodule path '%s': '%s %s'\n"),
		       ud->displaypath, ud->update_strategy.command, oid);
		നിർത്തുക;
	സ്ഥിരസ്ഥിതി:
		BUG("unexpected update strategy type: %d",
		    ud->update_strategy.type);
	}

	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ run_update_procedure(സ്ഥിരം ഘടന update_data *ud)
{
	പൂർണ്ണസംഖ്യ subforce = is_null_oid(&ud->suboid) || ud->force;

	എങ്കിൽ (!ud->nofetch) {
		/*
		 * Run fetch only if `oid` isn't present or it
		 * is not reachable from a ref.
		 */
		എങ്കിൽ (!is_tip_reachable(ud->sm_path, &ud->oid) &&
		    fetch_in_submodule(ud->sm_path, ud->depth, ud->quiet, ശൂന്യ) &&
		    !ud->quiet)
			fprintf_ln(stderr,
				   _("Unable to fetch in submodule path '%s'; "
				     "trying to directly fetch %s:"),
				   ud->displaypath, oid_to_hex(&ud->oid));
		/*
		 * Now we tried the usual fetch, but `oid` may
		 * not be reachable from any of the refs.
		 */
		എങ്കിൽ (!is_tip_reachable(ud->sm_path, &ud->oid) &&
		    fetch_in_submodule(ud->sm_path, ud->depth, ud->quiet, &ud->oid))
			തിരികെ die_message(_("Fetched in submodule path '%s', but it did not "
					     "contain %s. Direct fetching of that commit failed."),
					   ud->displaypath, oid_to_hex(&ud->oid));
	}

	തിരികെ run_update_command(ud, subforce);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ remote_submodule_branch(സ്ഥിരം അക്ഷരം *path, സ്ഥിരം അക്ഷരം **branch)
{
	സ്ഥിരം ഘടന submodule *sub;
	അക്ഷരം *താക്കോൽ;
	*branch = ശൂന്യ;

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);
	എങ്കിൽ (!sub)
		തിരികെ die_message(_("could not initialize submodule at path '%s'"),
				   path);

	താക്കോൽ = xstrfmt("submodule.%s.branch", sub->name);
	എങ്കിൽ (repo_config_get_string_tmp(the_repository, താക്കോൽ, branch))
		*branch = sub->branch;
	സ്വതന്ത്രം(താക്കോൽ);

	എങ്കിൽ (!*branch) {
		*branch = "HEAD";
		തിരികെ 0;
	}

	എങ്കിൽ (!strcmp(*branch, ".")) {
		സ്ഥിരം അക്ഷരം *refname = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
							      "HEAD", 0, ശൂന്യ,
							      ശൂന്യ);

		എങ്കിൽ (!refname)
			തിരികെ die_message(_("No such ref: %s"), "HEAD");

		/* detached HEAD */
		എങ്കിൽ (!strcmp(refname, "HEAD"))
			തിരികെ die_message(_("Submodule (%s) branch configured to inherit "
					     "branch from superproject, but the superproject "
					     "is not on any branch"), sub->name);

		എങ്കിൽ (!skip_prefix(refname, "refs/heads/", &refname))
			തിരികെ die_message(_("Expecting a full ref name, got %s"),
					   refname);

		*branch = refname;
		തിരികെ 0;
	}

	/* Our "branch" is coming from repo_config_get_string_tmp() */
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ ensure_core_worktree(സ്ഥിരം അക്ഷരം *path)
{
	സ്ഥിരം അക്ഷരം *cw;
	ഘടന repository subrepo;

	എങ്കിൽ (repo_submodule_init(&subrepo, the_repository, path, null_oid(the_hash_algo)))
		തിരികെ die_message(_("could not get a repository handle for submodule '%s'"),
				   path);

	എങ്കിൽ (!repo_config_get_string_tmp(&subrepo, "core.worktree", &cw)) {
		അക്ഷരം *cfg_file, *abs_path;
		സ്ഥിരം അക്ഷരം *rel_path;
		ഘടന strbuf sb = STRBUF_INIT;

		cfg_file = repo_git_path(&subrepo, "config");

		abs_path = absolute_pathdup(path);
		rel_path = relative_path(abs_path, subrepo.gitdir, &sb);

		repo_config_set_in_file(the_repository, cfg_file, "core.worktree", rel_path);

		സ്വതന്ത്രം(cfg_file);
		സ്വതന്ത്രം(abs_path);
		strbuf_release(&sb);
	}

	repo_clear(&subrepo);
	തിരികെ 0;
}

സ്ഥിതം സ്ഥിരം അക്ഷരം *submodule_update_type_to_label(എണ്ണൽ submodule_update_type type)
{
	തിരഞ്ഞെടുപ്പ് (type) {
	അവസ്ഥ SM_UPDATE_CHECKOUT:
		തിരികെ "checkout";
	അവസ്ഥ SM_UPDATE_MERGE:
		തിരികെ "merge";
	അവസ്ഥ SM_UPDATE_REBASE:
		തിരികെ "rebase";
	അവസ്ഥ SM_UPDATE_UNSPECIFIED:
	അവസ്ഥ SM_UPDATE_NONE:
	അവസ്ഥ SM_UPDATE_COMMAND:
		നിർത്തുക;
	}
	BUG("unreachable with type %d", type);
}

സ്ഥിതം ശൂന്യം update_data_to_args(സ്ഥിരം ഘടന update_data *update_data,
				ഘടന strvec *args)
{
	എണ്ണൽ submodule_update_type update_type = update_data->update_default;

	strvec_pushl(args, "submodule--helper", "update", "--recursive", ശൂന്യ);
	എങ്കിൽ (update_data->displaypath)
		strvec_pushf(args, "--super-prefix=%s/",
			     update_data->displaypath);
	strvec_pushf(args, "--jobs=%d", update_data->max_jobs);
	എങ്കിൽ (update_data->quiet)
		strvec_തള്ളുക(args, "--quiet");
	എങ്കിൽ (update_data->force)
		strvec_തള്ളുക(args, "--force");
	എങ്കിൽ (update_data->init)
		strvec_തള്ളുക(args, "--init");
	എങ്കിൽ (update_data->remote)
		strvec_തള്ളുക(args, "--remote");
	എങ്കിൽ (update_data->nofetch)
		strvec_തള്ളുക(args, "--no-fetch");
	എങ്കിൽ (update_data->dissociate)
		strvec_തള്ളുക(args, "--dissociate");
	എങ്കിൽ (update_data->progress)
		strvec_തള്ളുക(args, "--progress");
	എങ്കിൽ (update_data->require_init)
		strvec_തള്ളുക(args, "--require-init");
	എങ്കിൽ (update_data->depth)
		strvec_pushf(args, "--depth=%d", update_data->depth);
	എങ്കിൽ (update_type != SM_UPDATE_UNSPECIFIED)
		strvec_pushf(args, "--%s",
			     submodule_update_type_to_label(update_type));

	എങ്കിൽ (update_data->references.nr) {
		ഘടന string_list_item *item;

		for_each_string_list_item(item, &update_data->references)
			strvec_pushl(args, "--reference", item->string, ശൂന്യ);
	}
	എങ്കിൽ (update_data->ref_storage_format != REF_STORAGE_FORMAT_UNKNOWN)
		strvec_pushf(args, "--ref-storage-format=%s",
			     ref_storage_format_to_name(update_data->ref_storage_format));
	എങ്കിൽ (update_data->filter_options && update_data->filter_options->choice)
		strvec_pushf(args, "--filter=%s",
				expand_list_objects_filter_spec(
					update_data->filter_options));
	എങ്കിൽ (update_data->recommend_shallow == 0)
		strvec_തള്ളുക(args, "--no-recommend-shallow");
	അല്ലെങ്കിൽ എങ്കിൽ (update_data->recommend_shallow == 1)
		strvec_തള്ളുക(args, "--recommend-shallow");
	എങ്കിൽ (update_data->single_branch >= 0)
		strvec_തള്ളുക(args, update_data->single_branch ?
				    "--single-branch" :
				    "--no-single-branch");
}

സ്ഥിതം പൂർണ്ണസംഖ്യ update_submodule(ഘടന update_data *update_data)
{
	പൂർണ്ണസംഖ്യ ret;

	എങ്കിൽ (validate_submodule_path(update_data->sm_path) < 0)
		തിരികെ -1;

	ret = determine_submodule_update_strategy(the_repository,
						  update_data->just_cloned,
						  update_data->sm_path,
						  update_data->update_default,
						  &update_data->update_strategy);
	എങ്കിൽ (ret)
		തിരികെ ret;

	എങ്കിൽ (update_data->just_cloned)
		oidcpy(&update_data->suboid, null_oid(the_hash_algo));
	അല്ലെങ്കിൽ എങ്കിൽ (repo_resolve_gitlink_ref(the_repository, update_data->sm_path,
					  "HEAD", &update_data->suboid))
		തിരികെ die_message(_("Unable to find current revision in submodule path '%s'"),
				   update_data->displaypath);

	എങ്കിൽ (update_data->remote) {
		അക്ഷരം *remote_name;
		സ്ഥിരം അക്ഷരം *branch;
		അക്ഷരം *remote_ref;
		പൂർണ്ണസംഖ്യ code;

		code = get_default_remote_submodule(update_data->sm_path, &remote_name);
		എങ്കിൽ (code)
			തിരികെ code;
		code = remote_submodule_branch(update_data->sm_path, &branch);
		എങ്കിൽ (code) {
			സ്വതന്ത്രം(remote_name);
			തിരികെ code;
		}
		remote_ref = xstrfmt("refs/remotes/%s/%s", remote_name, branch);

		സ്വതന്ത്രം(remote_name);

		എങ്കിൽ (!update_data->nofetch) {
			എങ്കിൽ (fetch_in_submodule(update_data->sm_path, update_data->depth,
					      0, ശൂന്യ)) {
				സ്വതന്ത്രം(remote_ref);
				തിരികെ die_message(_("Unable to fetch in submodule path '%s'"),
						   update_data->sm_path);
			}
		}

		എങ്കിൽ (repo_resolve_gitlink_ref(the_repository, update_data->sm_path,
					     remote_ref, &update_data->oid)) {
			ret = die_message(_("Unable to find %s revision in submodule path '%s'"),
					  remote_ref, update_data->sm_path);
			സ്വതന്ത്രം(remote_ref);
			തിരികെ ret;
		}

		സ്വതന്ത്രം(remote_ref);
	}

	എങ്കിൽ (!oideq(&update_data->oid, &update_data->suboid) || update_data->force) {
		ret = run_update_procedure(update_data);
		എങ്കിൽ (ret)
			തിരികെ ret;
	}

	എങ്കിൽ (update_data->recursive) {
		ഘടന child_process cp = CHILD_PROCESS_INIT;
		ഘടന update_data next = *update_data;

		next.prefix = ശൂന്യ;
		oidcpy(&next.oid, null_oid(the_hash_algo));
		oidcpy(&next.suboid, null_oid(the_hash_algo));

		cp.dir = update_data->sm_path;
		cp.git_cmd = 1;
		prepare_submodule_repo_env(&cp.env);
		update_data_to_args(&next, &cp.args);

		ret = run_command(&cp);
		എങ്കിൽ (ret)
			die_message(_("Failed to recurse into submodule path '%s'"),
				    update_data->displaypath);
		തിരികെ ret;
	}

	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ update_submodules(ഘടന update_data *update_data)
{
	പൂർണ്ണസംഖ്യ i, ret = 0;
	ഘടന submodule_update_clone suc = SUBMODULE_UPDATE_CLONE_INIT;
	സ്ഥിരം ഘടന run_process_parallel_opts opts = {
		.tr2_category = "submodule",
		.tr2_label = "parallel/update",

		.processes = update_data->max_jobs,

		.get_next_task = update_clone_get_next_task,
		.start_failure = update_clone_start_failure,
		.task_finished = update_clone_task_finished,
		.data = &suc,
	};

	suc.update_data = update_data;
	run_processes_parallel(&opts);

	/*
	 * We saved the output and put it out all at once now.
	 * That means:
	 * - the listener does not have to interleave their (checkout)
	 *   work with our fetching.  The writes involved in a
	 *   checkout involve more straightforward sequential I/O.
	 * - the listener can avoid doing any work if fetching failed.
	 */
	എങ്കിൽ (suc.quickstop) {
		ret = 1;
		പോകുക cleanup;
	}

	വേണ്ടി (i = 0; i < suc.update_clone_nr; i++) {
		ഘടന update_clone_data ucd = suc.update_clone[i];
		പൂർണ്ണസംഖ്യ code = 128;

		oidcpy(&update_data->oid, &ucd.oid);
		update_data->just_cloned = ucd.just_cloned;
		update_data->sm_path = ucd.sub->path;

		/*
		 * Verify that the submodule path does not contain any
		 * symlinks; if it does, it might have been tampered with.
		 * TODO: allow exempting it via
		 * `safe.submodule.path` or something
		 */
		എങ്കിൽ (validate_submodule_path(update_data->sm_path) < 0)
			പോകുക fail;

		code = ensure_core_worktree(update_data->sm_path);
		എങ്കിൽ (code)
			പോകുക fail;

		update_data->displaypath = get_submodule_displaypath(
			update_data->sm_path, update_data->prefix,
			update_data->super_prefix);
		code = update_submodule(update_data);
		FREE_AND_NULL(update_data->displaypath);
fail:
		എങ്കിൽ (!code)
			തുടരുക;
		ret = code;
		എങ്കിൽ (ret == 128)
			പോകുക cleanup;
	}

cleanup:
	submodule_update_clone_release(&suc);
	string_list_clear(&update_data->references, 0);
	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ module_update(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
			 ഘടന repository *repo UNUSED)
{
	ഘടന pathspec pathspec = { 0 };
	ഘടന pathspec pathspec2 = { 0 };
	ഘടന update_data opt = UPDATE_DATA_INIT;
	ഘടന list_objects_filter_options filter_options =
		LIST_OBJECTS_FILTER_INIT;
	സ്ഥിരം അക്ഷരം *ref_storage_format = ശൂന്യ;
	പൂർണ്ണസംഖ്യ ret;
	ഘടന option module_update_options[] = {
		OPT__SUPER_PREFIX(&opt.super_prefix),
		OPT__FORCE(&opt.force, N_("force checkout updates"), 0),
		OPT_BOOL('i', "init", &opt.init,
			 N_("initialize uninitialized submodules before update")),
		OPT_BOOL(0, "remote", &opt.remote,
			 N_("use SHA-1 of submodule's remote tracking branch")),
		OPT_BOOL(0, "recursive", &opt.recursive,
			 N_("traverse submodules recursively")),
		OPT_BOOL('N', "no-fetch", &opt.nofetch,
			 N_("don't fetch new objects from the remote site")),
		OPT_SET_INT(0, "checkout", &opt.update_default,
			N_("use the 'checkout' update strategy (default)"),
			SM_UPDATE_CHECKOUT),
		OPT_SET_INT('m', "merge", &opt.update_default,
			N_("use the 'merge' update strategy"),
			SM_UPDATE_MERGE),
		OPT_SET_INT('r', "rebase", &opt.update_default,
			N_("use the 'rebase' update strategy"),
			SM_UPDATE_REBASE),
		OPT_STRING_LIST(0, "reference", &opt.references, N_("repo"),
			   N_("reference repository")),
		OPT_STRING(0, "ref-storage-format", &ref_storage_format, N_("format"),
			   N_("specify the reference storage format to use")),
		OPT_ALIAS_F(0, "ref-format", "ref-storage-format", PARSE_OPT_HIDDEN),
		OPT_BOOL(0, "dissociate", &opt.dissociate,
			   N_("use --reference only while cloning")),
		OPT_INTEGER(0, "depth", &opt.depth,
			   N_("create a shallow clone truncated to the "
			      "specified number of revisions")),
		OPT_INTEGER('j', "jobs", &opt.max_jobs,
			    N_("parallel jobs")),
		OPT_BOOL(0, "recommend-shallow", &opt.recommend_shallow,
			    N_("whether the initial clone should follow the shallow recommendation")),
		OPT__QUIET(&opt.quiet, N_("don't print cloning progress")),
		OPT_BOOL(0, "progress", &opt.progress,
			    N_("force cloning progress")),
		OPT_BOOL(0, "require-init", &opt.require_init,
			   N_("disallow cloning into non-empty directory, implies --init")),
		OPT_BOOL(0, "single-branch", &opt.single_branch,
			 N_("clone only one branch, HEAD or --branch")),
		OPT_PARSE_LIST_OBJECTS_FILTER(&filter_options),
		OPT_END()
	};
	സ്ഥിരം അക്ഷരം *സ്ഥിരം git_submodule_helper_usage[] = {
		N_("git submodule [--quiet] update"
		" [--init [--filter=<filter-spec>]] [--remote]"
		" [-N|--no-fetch] [-f|--force]"
		" [--checkout|--merge|--rebase]"
		" [--[no-]recommend-shallow] [--reference <repository>]"
		" [--recursive] [--[no-]single-branch] [--] [<path>...]"),
		ശൂന്യ
	};

	update_clone_config_from_gitmodules(the_repository, &opt.max_jobs);
	repo_config(the_repository, git_update_clone_config, &opt.max_jobs);

	argc = parse_options(argc, argv, prefix, module_update_options,
			     git_submodule_helper_usage, 0);

	എങ്കിൽ (opt.require_init)
		opt.init = 1;

	എങ്കിൽ (filter_options.choice && !opt.init) {
		usage_with_options(git_submodule_helper_usage,
				   module_update_options);
	}

	എങ്കിൽ (ref_storage_format) {
		opt.ref_storage_format = ref_storage_format_by_name(ref_storage_format);
		എങ്കിൽ (opt.ref_storage_format == REF_STORAGE_FORMAT_UNKNOWN)
			die(_("unknown ref storage format '%s'"), ref_storage_format);
	}

	opt.filter_options = &filter_options;
	opt.prefix = prefix;

	എങ്കിൽ (opt.update_default)
		opt.update_strategy.type = opt.update_default;

	എങ്കിൽ (module_list_compute(argv, prefix, &pathspec, &opt.list) < 0) {
		ret = 1;
		പോകുക cleanup;
	}

	എങ്കിൽ (pathspec.nr)
		opt.warn_if_uninitialized = 1;

	എങ്കിൽ (opt.init) {
		ഘടന module_list list = MODULE_LIST_INIT;
		ഘടന init_cb info = INIT_CB_INIT;

		എങ്കിൽ (module_list_compute(argv, opt.prefix,
					&pathspec2, &list) < 0) {
			module_list_release(&list);
			ret = 1;
			പോകുക cleanup;
		}

		/*
		 * If there are no path args and submodule.active is set then,
		 * by default, only initialize 'active' modules.
		 */
		എങ്കിൽ (!argc && !repo_config_get(the_repository, "submodule.active"))
			module_list_active(&list);

		info.prefix = opt.prefix;
		info.super_prefix = opt.super_prefix;
		എങ്കിൽ (opt.quiet)
			info.flags |= OPT_QUIET;

		for_each_listed_submodule(&list, init_submodule_cb, &info);
		module_list_release(&list);
	}

	ret = update_submodules(&opt);
cleanup:
	update_data_release(&opt);
	list_objects_filter_release(&filter_options);
	clear_pathspec(&pathspec);
	clear_pathspec(&pathspec2);
	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ തള്ളുക_check(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix UNUSED,
		      ഘടന repository *repo UNUSED)
{
	ഘടന remote *remote;
	സ്ഥിരം അക്ഷരം *superproject_head;
	അക്ഷരം *head;
	പൂർണ്ണസംഖ്യ detached_head = 0;
	ഘടന object_id head_oid;

	എങ്കിൽ (argc < 3)
		die("submodule--helper push-check requires at least 2 arguments");

	/*
	 * superproject's resolved head ref.
	 * if HEAD then the superproject is in a detached head state, otherwise
	 * it will be the resolved head ref.
	 */
	superproject_head = argv[1];
	argv++;
	argc--;
	/* Get the submodule's head ref and determine if it is detached */
	head = refs_resolve_refdup(get_main_ref_store(the_repository), "HEAD",
				   0, &head_oid, ശൂന്യ);
	എങ്കിൽ (!head)
		die(_("Failed to resolve HEAD as a valid ref."));
	എങ്കിൽ (!strcmp(head, "HEAD"))
		detached_head = 1;

	/*
	 * The remote must be configured.
	 * This is to avoid pushing to the exact same URL as the parent.
	 */
	remote = pushremote_get(argv[1]);
	എങ്കിൽ (!remote || remote->origin == REMOTE_UNCONFIGURED)
		die("remote '%s' not configured", argv[1]);

	/* Check the refspec */
	എങ്കിൽ (argc > 2) {
		പൂർണ്ണസംഖ്യ i;
		ഘടന ref *local_refs = get_local_heads();
		ഘടന refspec refspec = REFSPEC_INIT_തള്ളുക(the_hash_algo);

		refspec_appendn(&refspec, argv + 2, argc - 2);

		വേണ്ടി (i = 0; i < refspec.nr; i++) {
			സ്ഥിരം ഘടന refspec_item *rs = &refspec.ഇനങ്ങൾ[i];

			എങ്കിൽ (rs->pattern || rs->matching)
				തുടരുക;

			/* LHS must match a single ref */
			തിരഞ്ഞെടുപ്പ് (എണ്ണം_refspec_match(rs->src, local_refs, ശൂന്യ)) {
			അവസ്ഥ 1:
				നിർത്തുക;
			അവസ്ഥ 0:
				/*
				 * If LHS matches 'HEAD' then we need to ensure
				 * that it matches the same named branch
				 * checked out in the superproject.
				 */
				എങ്കിൽ (!strcmp(rs->src, "HEAD")) {
					എങ്കിൽ (!detached_head &&
					    !strcmp(head, superproject_head))
						നിർത്തുക;
					die("HEAD does not match the named branch in the superproject");
				}
				/* fallthrough */
			സ്ഥിരസ്ഥിതി:
				die("src refspec '%s' must name a ref",
				    rs->src);
			}
		}

		refspec_clear(&refspec);
		free_refs(local_refs);
	}
	സ്വതന്ത്രം(head);

	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ absorb_git_dirs(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
			   ഘടന repository *repo UNUSED)
{
	പൂർണ്ണസംഖ്യ i;
	ഘടന pathspec pathspec = { 0 };
	ഘടന module_list list = MODULE_LIST_INIT;
	സ്ഥിരം അക്ഷരം *super_prefix = ശൂന്യ;
	ഘടന option embed_gitdir_options[] = {
		OPT__SUPER_PREFIX(&super_prefix),
		OPT_END()
	};
	സ്ഥിരം അക്ഷരം *സ്ഥിരം git_submodule_helper_usage[] = {
		N_("git submodule absorbgitdirs [<options>] [<path>...]"),
		ശൂന്യ
	};
	പൂർണ്ണസംഖ്യ ret = 1;

	argc = parse_options(argc, argv, prefix, embed_gitdir_options,
			     git_submodule_helper_usage, 0);

	എങ്കിൽ (module_list_compute(argv, prefix, &pathspec, &list) < 0)
		പോകുക cleanup;

	വേണ്ടി (i = 0; i < list.nr; i++)
		absorb_git_dir_into_superproject(list.entries[i]->name,
						 super_prefix);

	ret = 0;
cleanup:
	clear_pathspec(&pathspec);
	module_list_release(&list);
	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ module_set_url(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
			  ഘടന repository *repo UNUSED)
{
	പൂർണ്ണസംഖ്യ quiet = 0, ret;
	സ്ഥിരം അക്ഷരം *newurl;
	സ്ഥിരം അക്ഷരം *path;
	അക്ഷരം *config_name;
	ഘടന option options[] = {
		OPT__QUIET(&quiet, N_("suppress output for setting url of a submodule")),
		OPT_END()
	};
	സ്ഥിരം അക്ഷരം *സ്ഥിരം usage[] = {
		N_("git submodule set-url [--quiet] <path> <newurl>"),
		ശൂന്യ
	};
	സ്ഥിരം ഘടന submodule *sub;

	argc = parse_options(argc, argv, prefix, options, usage, 0);

	എങ്കിൽ (argc != 2 || !(path = argv[0]) || !(newurl = argv[1]))
		usage_with_options(usage, options);

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);

	എങ്കിൽ (!sub)
		die(_("no submodule mapping found in .gitmodules for path '%s'"),
		    path);

	config_name = xstrfmt("submodule.%s.url", sub->name);
	ret = config_set_in_gitmodules_file_gently(the_repository, config_name, newurl);

	എങ്കിൽ (!ret) {
		repo_read_gitmodules(the_repository, 0);
		sync_submodule(sub->path, prefix, ശൂന്യ, quiet ? OPT_QUIET : 0);
	}

	സ്വതന്ത്രം(config_name);
	തിരികെ !!ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ module_set_branch(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
			     ഘടന repository *repo UNUSED)
{
	പൂർണ്ണസംഖ്യ opt_default = 0, ret;
	സ്ഥിരം അക്ഷരം *opt_branch = ശൂന്യ;
	സ്ഥിരം അക്ഷരം *path;
	അക്ഷരം *config_name;
	ഘടന option options[] = {
		/*
		 * We accept the `quiet` option for uniformity across subcommands,
		 * though there is nothing to make less verbose in this subcommand.
		 */
		OPT_NOOP_NOARG('q', "quiet"),

		OPT_BOOL('d', "default", &opt_default,
			N_("set the default tracking branch to master")),
		OPT_STRING('b', "branch", &opt_branch, N_("branch"),
			N_("set the default tracking branch")),
		OPT_END()
	};
	സ്ഥിരം അക്ഷരം *സ്ഥിരം usage[] = {
		N_("git submodule set-branch [-q|--quiet] (-d|--default) <path>"),
		N_("git submodule set-branch [-q|--quiet] (-b|--branch) <branch> <path>"),
		ശൂന്യ
	};
	സ്ഥിരം ഘടന submodule *sub;

	argc = parse_options(argc, argv, prefix, options, usage, 0);

	എങ്കിൽ (!opt_branch && !opt_default)
		die(_("--branch or --default required"));

	എങ്കിൽ (opt_branch && opt_default)
		die(_("options '%s' and '%s' cannot be used together"), "--branch", "--default");

	എങ്കിൽ (argc != 1 || !(path = argv[0]))
		usage_with_options(usage, options);

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);

	എങ്കിൽ (!sub)
		die(_("no submodule mapping found in .gitmodules for path '%s'"),
		    path);

	config_name = xstrfmt("submodule.%s.branch", sub->name);
	ret = config_set_in_gitmodules_file_gently(the_repository, config_name, opt_branch);

	സ്വതന്ത്രം(config_name);
	തിരികെ !!ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ module_create_branch(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
				ഘടന repository *repo UNUSED)
{
	എണ്ണൽ branch_track track;
	പൂർണ്ണസംഖ്യ quiet = 0, force = 0, reflog = 0, dry_run = 0;
	ഘടന option options[] = {
		OPT__QUIET(&quiet, N_("print only error messages")),
		OPT__FORCE(&force, N_("force creation"), 0),
		OPT_BOOL(0, "create-reflog", &reflog,
			 N_("create the branch's reflog")),
		OPT_CALLBACK_F('t', "track",  &track, "(direct|inherit)",
			N_("set branch tracking configuration"),
			PARSE_OPT_OPTARG,
			parse_opt_tracking_mode),
		OPT__DRY_RUN(&dry_run,
			     N_("show whether the branch would be created")),
		OPT_END()
	};
	സ്ഥിരം അക്ഷരം *സ്ഥിരം usage[] = {
		N_("git submodule--helper create-branch [-f|--force] [--create-reflog] [-q|--quiet] [-t|--track] [-n|--dry-run] <name> <start-oid> <start-name>"),
		ശൂന്യ
	};
	ഘടന repo_config_മൂല്യങ്ങൾ *cfg = repo_config_മൂല്യങ്ങൾ(the_repository);

	repo_config(the_repository, git_default_config, ശൂന്യ);
	track = cfg->branch_track;
	argc = parse_options(argc, argv, prefix, options, usage, 0);

	എങ്കിൽ (argc != 3)
		usage_with_options(usage, options);

	എങ്കിൽ (!quiet && !dry_run)
		printf_ln(_("creating branch '%s'"), argv[0]);

	create_branches_recursively(the_repository, argv[0], argv[1], argv[2],
				    force, reflog, quiet, track, dry_run);
	തിരികെ 0;
}

ഘടന add_data {
	സ്ഥിരം അക്ഷരം *prefix;
	സ്ഥിരം അക്ഷരം *branch;
	സ്ഥിരം അക്ഷരം *reference_path;
	അക്ഷരം *sm_path;
	സ്ഥിരം അക്ഷരം *sm_name;
	സ്ഥിരം അക്ഷരം *repo;
	സ്ഥിരം അക്ഷരം *realrepo;
	എണ്ണൽ ref_storage_format ref_storage_format;
	പൂർണ്ണസംഖ്യ depth;
	unsigned പൂർണ്ണസംഖ്യ force: 1;
	unsigned പൂർണ്ണസംഖ്യ quiet: 1;
	unsigned പൂർണ്ണസംഖ്യ progress: 1;
	unsigned പൂർണ്ണസംഖ്യ dissociate: 1;
};
#define ADD_DATA_INIT { \
	.depth = -1, \
	.ref_storage_format = REF_STORAGE_FORMAT_UNKNOWN, \
}

സ്ഥിതം ശൂന്യം append_fetch_remotes(ഘടന strbuf *സന്ദേശം, സ്ഥിരം അക്ഷരം *git_dir_path)
{
	ഘടന child_process cp_remote = CHILD_PROCESS_INIT;
	ഘടന strbuf sb_remote_out = STRBUF_INIT;

	cp_remote.git_cmd = 1;
	strvec_pushf(&cp_remote.env,
		     "GIT_DIR=%s", git_dir_path);
	strvec_തള്ളുക(&cp_remote.env, "GIT_WORK_TREE=.");
	strvec_pushl(&cp_remote.args, "remote", "-v", ശൂന്യ);
	എങ്കിൽ (!capture_command(&cp_remote, &sb_remote_out, 0)) {
		അക്ഷരം *next_വരി;
		അക്ഷരം *വരി = sb_remote_out.buf;

		ആകുമ്പോൾ ((next_വരി = strchr(വരി, '\n')) != ശൂന്യ) {
			size_t len = next_വരി - വരി;

			എങ്കിൽ (strip_suffix_mem(വരി, &len, " (fetch)"))
				strbuf_addf(സന്ദേശം, "  %.*s\n", (പൂർണ്ണസംഖ്യ)len, വരി);
			വരി = next_വരി + 1;
		}
	}

	strbuf_release(&sb_remote_out);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ add_submodule(സ്ഥിരം ഘടന add_data *add_data)
{
	ഘടന module_clone_data clone_data = MODULE_CLONE_DATA_INIT;
	ഘടന string_list reference = STRING_LIST_INIT_NODUP;
	പൂർണ്ണസംഖ്യ ret = -1;

	/* perhaps the path already exists and is already a git repo, else clone it */
	എങ്കിൽ (is_directory(add_data->sm_path)) {
		അക്ഷരം *submod_gitdir_path;
		ഘടന strbuf sm_path = STRBUF_INIT;
		strbuf_addstr(&sm_path, add_data->sm_path);
		submod_gitdir_path = xstrfmt("%s/.git", add_data->sm_path);
		എങ്കിൽ (is_nonbare_repository_dir(&sm_path))
			printf(_("Adding existing repo at '%s' to the index\n"),
			       add_data->sm_path);
		അല്ലെങ്കിൽ
			die(_("'%s' already exists and is not a valid git repo"),
			    add_data->sm_path);
		strbuf_release(&sm_path);
		സ്വതന്ത്രം(submod_gitdir_path);
	} അല്ലെങ്കിൽ {
		ഘടന child_process cp = CHILD_PROCESS_INIT;
		ഘടന strbuf submod_gitdir = STRBUF_INIT;

		submodule_name_to_gitdir(&submod_gitdir, the_repository, add_data->sm_name);

		എങ്കിൽ (is_directory(submod_gitdir.buf)) {
			എങ്കിൽ (!add_data->force) {
				ഘടന strbuf സന്ദേശം = STRBUF_INIT;
				അക്ഷരം *die_സന്ദേശം;

				strbuf_addf(&സന്ദേശം, _("A git directory for '%s' is found "
						    "locally with remote(s):\n"),
					    add_data->sm_name);

				append_fetch_remotes(&സന്ദേശം, submod_gitdir.buf);
				strbuf_release(&submod_gitdir);

				strbuf_addf(&സന്ദേശം, _("If you want to reuse this local git "
						    "directory instead of cloning again from\n"
						    "  %s\n"
						    "use the '--force' option. If the local git "
						    "directory is not the correct repo\n"
						    "or you are unsure what this means choose "
						    "another name with the '--name' option."),
					    add_data->realrepo);

				die_സന്ദേശം = strbuf_detach(&സന്ദേശം, ശൂന്യ);
				die("%s", die_സന്ദേശം);
			} അല്ലെങ്കിൽ {
				printf(_("Reactivating local git directory for "
					 "submodule '%s'\n"), add_data->sm_name);
			}
		}
		strbuf_release(&submod_gitdir);

		clone_data.prefix = add_data->prefix;
		clone_data.path = add_data->sm_path;
		clone_data.name = add_data->sm_name;
		clone_data.url = add_data->realrepo;
		clone_data.quiet = add_data->quiet;
		clone_data.progress = add_data->progress;
		എങ്കിൽ (add_data->reference_path) {
			അക്ഷരം *p = xstrdup(add_data->reference_path);

			string_list_append(&reference, p)->util = p;
		}
		clone_data.ref_storage_format = add_data->ref_storage_format;
		clone_data.dissociate = add_data->dissociate;
		എങ്കിൽ (add_data->depth >= 0)
			clone_data.depth = add_data->depth;

		എങ്കിൽ (clone_submodule(&clone_data, &reference))
			പോകുക cleanup;

		prepare_submodule_repo_env(&cp.env);
		cp.git_cmd = 1;
		cp.dir = add_data->sm_path;
		/*
		 * NOTE: we only get here if add_data->force is true, so
		 * passing --force to checkout is reasonable.
		 */
		strvec_pushl(&cp.args, "checkout", "-f", "-q", ശൂന്യ);

		എങ്കിൽ (add_data->branch) {
			strvec_pushl(&cp.args, "-B", add_data->branch, ശൂന്യ);
			strvec_pushf(&cp.args, "origin/%s", add_data->branch);
		}

		എങ്കിൽ (run_command(&cp))
			die(_("unable to checkout submodule '%s'"), add_data->sm_path);
	}
	ret = 0;

cleanup:
	string_list_clear(&reference, 1);
	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ config_submodule_in_gitmodules(സ്ഥിരം അക്ഷരം *name, സ്ഥിരം അക്ഷരം *var, സ്ഥിരം അക്ഷരം *മൂല്യം)
{
	അക്ഷരം *താക്കോൽ;
	പൂർണ്ണസംഖ്യ ret;

	എങ്കിൽ (!is_writing_gitmodules_ok())
		die(_("please make sure that the .gitmodules file is in the working tree"));

	താക്കോൽ = xstrfmt("submodule.%s.%s", name, var);
	ret = config_set_in_gitmodules_file_gently(the_repository, താക്കോൽ, മൂല്യം);
	സ്വതന്ത്രം(താക്കോൽ);

	തിരികെ ret;
}

സ്ഥിതം ശൂന്യം configure_added_submodule(ഘടന add_data *add_data)
{
	അക്ഷരം *താക്കോൽ;
	ഘടന child_process add_submod = CHILD_PROCESS_INIT;
	ഘടന child_process add_gitmodules = CHILD_PROCESS_INIT;
	സ്ഥിരം ഘടന string_list *മൂല്യങ്ങൾ;
	പൂർണ്ണസംഖ്യ matched = 0;

	താക്കോൽ = xstrfmt("submodule.%s.url", add_data->sm_name);
	repo_config_set_gently(the_repository, താക്കോൽ, add_data->realrepo);
	സ്വതന്ത്രം(താക്കോൽ);

	add_submod.git_cmd = 1;
	strvec_pushl(&add_submod.args, "add",
		     "--no-warn-embedded-repo", ശൂന്യ);
	എങ്കിൽ (add_data->force)
		strvec_തള്ളുക(&add_submod.args, "--force");
	strvec_pushl(&add_submod.args, "--", add_data->sm_path, ശൂന്യ);

	എങ്കിൽ (run_command(&add_submod))
		die(_("Failed to add submodule '%s'"), add_data->sm_path);

	എങ്കിൽ (config_submodule_in_gitmodules(add_data->sm_name, "path", add_data->sm_path) ||
	    config_submodule_in_gitmodules(add_data->sm_name, "url", add_data->repo))
		die(_("Failed to register submodule '%s'"), add_data->sm_path);

	എങ്കിൽ (add_data->branch) {
		എങ്കിൽ (config_submodule_in_gitmodules(add_data->sm_name,
						   "branch", add_data->branch))
			die(_("Failed to register submodule '%s'"), add_data->sm_path);
	}

	add_gitmodules.git_cmd = 1;
	strvec_pushl(&add_gitmodules.args,
		     "add", "--force", "--", ".gitmodules", ശൂന്യ);

	എങ്കിൽ (run_command(&add_gitmodules))
		die(_("Failed to register submodule '%s'"), add_data->sm_path);

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
	എങ്കിൽ (repo_config_get(the_repository, "submodule.active") || /* key absent */
	    repo_config_get_string_multi(the_repository, "submodule.active", &മൂല്യങ്ങൾ)) {
		/*
		 * If the submodule being added isn't already covered by the
		 * current configured pathspec, set the submodule's active flag
		 */
		താക്കോൽ = xstrfmt("submodule.%s.active", add_data->sm_name);
		repo_config_set_gently(the_repository, താക്കോൽ, "true");
		സ്വതന്ത്രം(താക്കോൽ);
	} അല്ലെങ്കിൽ {
		വേണ്ടി (size_t i = 0; i < മൂല്യങ്ങൾ->nr; i++) {
			സ്ഥിരം അക്ഷരം *pat = മൂല്യങ്ങൾ->ഇനങ്ങൾ[i].string;
			എങ്കിൽ (!wildmatch(pat, add_data->sm_path, 0)) { /* match found */
				matched = 1;
				നിർത്തുക;
			}
		}
		എങ്കിൽ (!matched) { /* no pattern matched -> force-enable */
			താക്കോൽ = xstrfmt("submodule.%s.active", add_data->sm_name);
			repo_config_set_gently(the_repository, താക്കോൽ, "true");
			സ്വതന്ത്രം(താക്കോൽ);
		}
	}
}

സ്ഥിതം ശൂന്യം die_on_index_match(സ്ഥിരം അക്ഷരം *path, പൂർണ്ണസംഖ്യ force)
{
	ഘടന pathspec ps;
	സ്ഥിരം അക്ഷരം *args[] = { path, ശൂന്യ };
	parse_pathspec(&ps, 0, PATHSPEC_PREFER_CWD, ശൂന്യ, args);

	എങ്കിൽ (repo_read_index_preload(the_repository, ശൂന്യ, 0) < 0)
		die(_("index file corrupt"));

	എങ്കിൽ (ps.nr) {
		അക്ഷരം *ps_matched = xcalloc(ps.nr, 1);

		/* TODO: audit for interaction with sparse-index. */
		ensure_full_index(the_repository->index);

		/*
		 * Since there is only one pathspec, we just need to
		 * check ps_matched[0] to know if a cache entry matched.
		 */
		വേണ്ടി (size_t i = 0; i < the_repository->index->cache_nr; i++) {
			ce_path_match(the_repository->index, the_repository->index->cache[i], &ps,
				      ps_matched);

			എങ്കിൽ (ps_matched[0]) {
				എങ്കിൽ (!force)
					die(_("'%s' already exists in the index"),
					    path);
				എങ്കിൽ (!S_ISGITLINK(the_repository->index->cache[i]->ce_mode))
					die(_("'%s' already exists in the index "
					      "and is not a submodule"), path);
				നിർത്തുക;
			}
		}
		സ്വതന്ത്രം(ps_matched);
	}
	clear_pathspec(&ps);
}

സ്ഥിതം ശൂന്യം die_on_repo_without_commits(സ്ഥിരം അക്ഷരം *path)
{
	ഘടന strbuf sb = STRBUF_INIT;
	strbuf_addstr(&sb, path);
	എങ്കിൽ (is_nonbare_repository_dir(&sb)) {
		ഘടന object_id oid;
		എങ്കിൽ (repo_resolve_gitlink_ref(the_repository, path, "HEAD", &oid) < 0)
			die(_("'%s' does not have a commit checked out"), path);
	}
	strbuf_release(&sb);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ module_add(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
		      ഘടന repository *repo UNUSED)
{
	പൂർണ്ണസംഖ്യ force = 0, quiet = 0, progress = 0, dissociate = 0;
	ഘടന add_data add_data = ADD_DATA_INIT;
	സ്ഥിരം അക്ഷരം *ref_storage_format = ശൂന്യ;
	അക്ഷരം *to_free = ശൂന്യ;
	സ്ഥിരം ഘടന submodule *existing;
	ഘടന strbuf buf = STRBUF_INIT;
	അക്ഷരം *sm_name_to_free = ശൂന്യ;
	ഘടന option options[] = {
		OPT_STRING('b', "branch", &add_data.branch, N_("branch"),
			   N_("branch of repository to add as submodule")),
		OPT__FORCE(&force, N_("allow adding an otherwise ignored submodule path"),
			   PARSE_OPT_NOCOMPLETE),
		OPT__QUIET(&quiet, N_("print only error messages")),
		OPT_BOOL(0, "progress", &progress, N_("force cloning progress")),
		OPT_STRING(0, "reference", &add_data.reference_path, N_("repository"),
			   N_("reference repository")),
		OPT_STRING(0, "ref-storage-format", &ref_storage_format, N_("format"),
			   N_("specify the reference storage format to use")),
		OPT_ALIAS_F(0, "ref-format", "ref-storage-format", PARSE_OPT_HIDDEN),
		OPT_BOOL(0, "dissociate", &dissociate, N_("borrow the objects from reference repositories")),
		OPT_STRING(0, "name", &add_data.sm_name, N_("name"),
			   N_("sets the submodule's name to the given string "
			      "instead of defaulting to its path")),
		OPT_INTEGER(0, "depth", &add_data.depth, N_("depth for shallow clones")),
		OPT_END()
	};
	സ്ഥിരം അക്ഷരം *സ്ഥിരം usage[] = {
		N_("git submodule add [<options>] [--] <repository> [<path>]"),
		ശൂന്യ
	};
	ഘടന strbuf sb = STRBUF_INIT;
	പൂർണ്ണസംഖ്യ ret = 1;

	argc = parse_options(argc, argv, prefix, options, usage, 0);

	എങ്കിൽ (!is_writing_gitmodules_ok())
		die(_("please make sure that the .gitmodules file is in the working tree"));

	എങ്കിൽ (prefix && *prefix &&
	    add_data.reference_path && !is_absolute_path(add_data.reference_path))
		add_data.reference_path = xstrfmt("%s%s", prefix, add_data.reference_path);

	എങ്കിൽ (argc == 0 || argc > 2)
		usage_with_options(usage, options);

	എങ്കിൽ (ref_storage_format) {
		add_data.ref_storage_format = ref_storage_format_by_name(ref_storage_format);
		എങ്കിൽ (add_data.ref_storage_format == REF_STORAGE_FORMAT_UNKNOWN)
			die(_("unknown ref storage format '%s'"), ref_storage_format);
	}

	add_data.repo = argv[0];
	എങ്കിൽ (argc == 1)
		add_data.sm_path = git_url_basename(add_data.repo, 0, 0);
	അല്ലെങ്കിൽ
		add_data.sm_path = xstrdup(argv[1]);

	എങ്കിൽ (prefix && *prefix && !is_absolute_path(add_data.sm_path)) {
		അക്ഷരം *sm_path = add_data.sm_path;

		add_data.sm_path = xstrfmt("%s%s", prefix, sm_path);
		സ്വതന്ത്രം(sm_path);
	}

	എങ്കിൽ (starts_with_dot_dot_slash(add_data.repo) ||
	    starts_with_dot_slash(add_data.repo)) {
		എങ്കിൽ (prefix)
			die(_("Relative path can only be used from the toplevel "
			      "of the working tree"));

		/* dereference source url relative to parent's url */
		to_free = resolve_relative_url(add_data.repo, ശൂന്യ, 1);
		add_data.realrepo = to_free;
	} അല്ലെങ്കിൽ എങ്കിൽ (is_dir_sep(add_data.repo[0]) || strchr(add_data.repo, ':')) {
		add_data.realrepo = add_data.repo;
	} അല്ലെങ്കിൽ {
		die(_("repo URL: '%s' must be absolute or begin with ./|../"),
		    add_data.repo);
	}

	/*
	 * normalize path:
	 * multiple //; leading ./; /./; /../;
	 */
	normalize_path_copy(add_data.sm_path, add_data.sm_path);
	strip_dir_trailing_slashes(add_data.sm_path);

	എങ്കിൽ (validate_submodule_path(add_data.sm_path) < 0)
		die(ശൂന്യ);

	die_on_index_match(add_data.sm_path, force);
	die_on_repo_without_commits(add_data.sm_path);

	എങ്കിൽ (!force) {
		ഘടന child_process cp = CHILD_PROCESS_INIT;

		cp.git_cmd = 1;
		cp.no_stdout = 1;
		strvec_pushl(&cp.args, "add", "--dry-run", "--ignore-missing",
			     "--no-warn-embedded-repo", add_data.sm_path, ശൂന്യ);
		എങ്കിൽ ((ret = pipe_command(&cp, ശൂന്യ, 0, ശൂന്യ, 0, &sb, 0))) {
			strbuf_complete_വരി(&sb);
			fputs(sb.buf, stderr);
			പോകുക cleanup;
		}
	}

	എങ്കിൽ (!add_data.sm_name)
		add_data.sm_name = add_data.sm_path;

	existing = submodule_from_name(the_repository,
					null_oid(the_hash_algo),
					add_data.sm_name);

	എങ്കിൽ (existing && existing->path &&
	    strcmp(existing->path, add_data.sm_path)) {
		എങ്കിൽ (!force) {
			die(_("submodule name '%s' already used for path '%s'"),
			    add_data.sm_name, existing->path);
		}
		/* --force: build <name><n> until unique */
		വേണ്ടി (പൂർണ്ണസംഖ്യ i = 1; ; i++) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "%s%d", add_data.sm_name, i);
			എങ്കിൽ (!submodule_from_name(the_repository,
						null_oid(the_hash_algo),
						buf.buf)) {
				നിർത്തുക;
			}
		}
		add_data.sm_name = sm_name_to_free = strbuf_detach(&buf, ശൂന്യ);
	}

	എങ്കിൽ (check_submodule_name(add_data.sm_name))
		die(_("'%s' is not a valid submodule name"), add_data.sm_name);

	add_data.prefix = prefix;
	add_data.force = !!force;
	add_data.quiet = !!quiet;
	add_data.progress = !!progress;
	add_data.dissociate = !!dissociate;

	എങ്കിൽ (the_repository->repository_format_submodule_path_cfg)
		create_default_gitdir_config(add_data.sm_name);

	എങ്കിൽ (add_submodule(&add_data))
		പോകുക cleanup;
	configure_added_submodule(&add_data);

	ret = 0;
cleanup:
	സ്വതന്ത്രം(sm_name_to_free);
	സ്വതന്ത്രം(add_data.sm_path);
	സ്വതന്ത്രം(to_free);
	strbuf_release(&sb);

	തിരികെ ret;
}

പൂർണ്ണസംഖ്യ cmd_submodule__helper(പൂർണ്ണസംഖ്യ argc,
			  സ്ഥിരം അക്ഷരം **argv,
			  സ്ഥിരം അക്ഷരം *prefix,
			  ഘടന repository *repo)
{
	parse_opt_subcommand_fn *fn = ശൂന്യ;
	സ്ഥിരം അക്ഷരം *സ്ഥിരം usage[] = {
		N_("git submodule--helper <command>"),
		ശൂന്യ
	};
	ഘടന option options[] = {
		OPT_SUBCOMMAND("migrate-gitdir-configs", &fn, module_migrate),
		OPT_SUBCOMMAND("gitdir", &fn, module_gitdir),
		OPT_SUBCOMMAND("clone", &fn, module_clone),
		OPT_SUBCOMMAND("add", &fn, module_add),
		OPT_SUBCOMMAND("update", &fn, module_update),
		OPT_SUBCOMMAND("foreach", &fn, module_foreach),
		OPT_SUBCOMMAND("init", &fn, module_init),
		OPT_SUBCOMMAND("status", &fn, module_status),
		OPT_SUBCOMMAND("sync", &fn, module_sync),
		OPT_SUBCOMMAND("deinit", &fn, module_deinit),
		OPT_SUBCOMMAND("summary", &fn, module_summary),
		OPT_SUBCOMMAND("push-check", &fn, തള്ളുക_check),
		OPT_SUBCOMMAND("absorbgitdirs", &fn, absorb_git_dirs),
		OPT_SUBCOMMAND("set-url", &fn, module_set_url),
		OPT_SUBCOMMAND("set-branch", &fn, module_set_branch),
		OPT_SUBCOMMAND("create-branch", &fn, module_create_branch),
		OPT_SUBCOMMAND("get-default-remote", &fn, module_get_default_remote),
		OPT_END()
	};
	argc = parse_options(argc, argv, prefix, options, usage, 0);

	തിരികെ fn(argc, argv, prefix, repo);
}
