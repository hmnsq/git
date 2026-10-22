#define USE_THE_REPOSITORY_VARIABLE

#अन्तर्भाव "builtin.h"
#अन्तर्भाव "abspath.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "hex.h"

#अन्तर्भाव "config.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "path.h"
#अन्तर्भाव "pathspec.h"
#अन्तर्भाव "preload-index.h"
#अन्तर्भाव "dir.h"
#अन्तर्भाव "read-cache.h"
#अन्तर्भाव "setup.h"
#अन्तर्भाव "sparse-index.h"
#अन्तर्भाव "submodule.h"
#अन्तर्भाव "submodule-config.h"
#अन्तर्भाव "string-list.h"
#अन्तर्भाव "run-command.h"
#अन्तर्भाव "remote.h"
#अन्तर्भाव "refs.h"
#अन्तर्भाव "refspec.h"
#अन्तर्भाव "revision.h"
#अन्तर्भाव "diffcore.h"
#अन्तर्भाव "diff.h"
#अन्तर्भाव "object-file.h"
#अन्तर्भाव "object-name.h"
#अन्तर्भाव "odb.h"
#अन्तर्भाव "odb/source.h"
#अन्तर्भाव "odb/source-files.h"
#अन्तर्भाव "advice.h"
#अन्तर्भाव "branch.h"
#अन्तर्भाव "list-objects-filter-options.h"
#अन्तर्भाव "wildmatch.h"
#अन्तर्भाव "strbuf.h"
#अन्तर्भाव "url.h"

#define OPT_QUIET (1 << 0)
#define OPT_CACHED (1 << 1)
#define OPT_RECURSIVE (1 << 2)
#define OPT_FORCE (1 << 3)

प्रकारनिर्देश शून्य (*each_submodule_fn)(अचर संरचना cache_entry *list_item,
				  शून्य *cb_data);

स्थिर वर्ण *get_default_remote(शून्य)
{
	फर्काऊ xstrdup(repo_default_remote(the_repository));
}

स्थिर वर्ण *resolve_relative_url(अचर वर्ण *rel_url, अचर वर्ण *up_path, पूर्णाङ्क quiet)
{
	वर्ण *remoteurl, *resolved_url;
	वर्ण *remote = get_default_remote();
	संरचना strbuf remotesb = STRBUF_INIT;

	strbuf_addf(&remotesb, "remote.%s.url", remote);
	यदि (repo_config_get_string(the_repository, remotesb.buf, &remoteurl)) {
		यदि (!quiet)
			warning(_("could not look up configuration '%s'. "
				  "Assuming this repository is its own "
				  "authoritative upstream."),
				remotesb.buf);
		remoteurl = xgetcwd();
	}
	resolved_url = relative_url(remoteurl, rel_url, up_path);

	मुक्त(remote);
	मुक्त(remoteurl);
	strbuf_release(&remotesb);

	फर्काऊ resolved_url;
}

स्थिर पूर्णाङ्क get_default_remote_submodule(अचर वर्ण *module_path, वर्ण **default_remote)
{
	अचर संरचना submodule *sub;
	संरचना repository subrepo;
	अचर वर्ण *remote_name = रिक्त;
	वर्ण *url = रिक्त;
	पूर्णाङ्क ret = 0;

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), module_path);
	यदि (sub && sub->url) {
		url = xstrdup(sub->url);

		/* Possibly a url relative to parent */
		यदि (starts_with_dot_dot_slash(url) ||
		    starts_with_dot_slash(url)) {
			वर्ण *oldurl = url;

			url = resolve_relative_url(oldurl, रिक्त, 1);
			मुक्त(oldurl);
		}
	}

	यदि (repo_submodule_init(&subrepo, the_repository, module_path,
				null_oid(the_hash_algo)) < 0) {
		ret = die_message(_("could not get a repository handle for submodule '%s'"),
				   module_path);
		गच्छ out;
	}

	/* Look up by URL first */
	यदि (url)
		remote_name = repo_remote_from_url(&subrepo, url);
	यदि (!remote_name)
		remote_name = repo_default_remote(&subrepo);

	*default_remote = xstrdup(remote_name);

out:
	repo_clear(&subrepo);
	मुक्त(url);

	फर्काऊ ret;
}

स्थिर पूर्णाङ्क module_get_default_remote(पूर्णाङ्क argc, अचर वर्ण **argv, अचर वर्ण *prefix,
				     संरचना repository *repo UNUSED)
{
	अचर वर्ण *path;
	वर्ण *resolved_path = रिक्त;
	वर्ण *default_remote = रिक्त;
	पूर्णाङ्क code;
	संरचना option options[] = {
		OPT_END()
	};
	अचर वर्ण *अचर usage[] = {
		N_("git submodule--helper get-default-remote <path>"),
		रिक्त
	};

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	यदि (argc != 1)
		usage_with_options(usage, options);

	path = argv[0];
	यदि (prefix && *prefix && !is_absolute_path(path)) {
		resolved_path = xstrfmt("%s%s", prefix, path);
		path = resolved_path;
	}

	code = get_default_remote_submodule(path, &default_remote);
	यदि (code) {
		मुक्त(resolved_path);
		फर्काऊ code;
	}

	म_लिखो("%s\n", default_remote);
	मुक्त(default_remote);
	मुक्त(resolved_path);
	फर्काऊ 0;
}

/* the result should be freed by the caller. */
स्थिर वर्ण *get_submodule_displaypath(अचर वर्ण *path, अचर वर्ण *prefix,
				       अचर वर्ण *super_prefix)
{
	यदि (prefix && super_prefix) {
		BUG("cannot have prefix '%s' and superprefix '%s'",
		    prefix, super_prefix);
	} अन्यथा यदि (prefix) {
		संरचना strbuf sb = STRBUF_INIT;
		वर्ण *displaypath = xstrdup(relative_path(path, prefix, &sb));
		strbuf_release(&sb);
		फर्काऊ displaypath;
	} अन्यथा यदि (super_prefix) {
		फर्काऊ xstrfmt("%s%s", super_prefix, path);
	} अन्यथा {
		फर्काऊ xstrdup(path);
	}
}

स्थिर वर्ण *compute_rev_name(अचर वर्ण *sub_path, अचर वर्ण* object_id)
{
	संरचना strbuf sb = STRBUF_INIT;
	अचर वर्ण ***d;

	स्थिर अचर वर्ण *describe_bare[] = { रिक्त };

	स्थिर अचर वर्ण *describe_tags[] = { "--tags", रिक्त };

	स्थिर अचर वर्ण *describe_contains[] = { "--contains", रिक्त };

	स्थिर अचर वर्ण *describe_all_always[] = { "--all", "--always", रिक्त };

	स्थिर अचर वर्ण **describe_argv[] = { describe_bare, describe_tags,
						describe_contains,
						describe_all_always, रिक्त };

	लागि (d = describe_argv; *d; d++) {
		संरचना child_process cp = CHILD_PROCESS_INIT;
		prepare_submodule_repo_env(&cp.env);
		cp.dir = sub_path;
		cp.git_cmd = 1;
		cp.no_stderr = 1;

		strvec_थप(&cp.args, "describe");
		strvec_pushv(&cp.args, *d);
		strvec_थप(&cp.args, object_id);

		यदि (!capture_command(&cp, &sb, 0)) {
			strbuf_strip_suffix(&sb, "\n");
			फर्काऊ strbuf_detach(&sb, रिक्त);
		}
	}

	strbuf_release(&sb);
	फर्काऊ रिक्त;
}

संरचना module_list {
	अचर संरचना cache_entry **entries;
	पूर्णाङ्क alloc, nr;
};
#define MODULE_LIST_INIT { 0 }

स्थिर शून्य module_list_release(संरचना module_list *ml)
{
	मुक्त(ml->entries);
}

स्थिर पूर्णाङ्क module_list_compute(अचर वर्ण **argv,
			       अचर वर्ण *prefix,
			       संरचना pathspec *pathspec,
			       संरचना module_list *list)
{
	पूर्णाङ्क result = 0;
	वर्ण *ps_matched = रिक्त;

	parse_pathspec(pathspec, 0,
		       PATHSPEC_PREFER_FULL,
		       prefix, argv);

	यदि (pathspec->nr)
		ps_matched = xcalloc(pathspec->nr, 1);

	यदि (repo_read_index(the_repository) < 0)
		die(_("index file corrupt"));

	लागि (माप_प्रकार i = 0; i < the_repository->index->cache_nr; i++) {
		अचर संरचना cache_entry *ce = the_repository->index->cache[i];

		यदि (!match_pathspec(the_repository->index, pathspec, ce->name, ce_namelen(ce),
				    0, ps_matched, 1) ||
		    !S_ISGITLINK(ce->ce_mode))
			जारी;

		ALLOC_GROW(list->entries, list->nr + 1, list->alloc);
		list->entries[list->nr++] = ce;
		जबसम्म (i + 1 < the_repository->index->cache_nr &&
		       !म_भेद(ce->name, the_repository->index->cache[i + 1]->name))
			/*
			 * Skip entries with the same name in different stages
			 * to make sure an entry is returned only once.
			 */
			i++;
	}

	यदि (ps_matched && report_path_error(ps_matched, pathspec))
		result = -1;

	मुक्त(ps_matched);

	फर्काऊ result;
}

स्थिर शून्य module_list_active(संरचना module_list *list)
{
	पूर्णाङ्क i;
	संरचना module_list active_modules = MODULE_LIST_INIT;

	लागि (i = 0; i < list->nr; i++) {
		अचर संरचना cache_entry *ce = list->entries[i];

		यदि (!is_submodule_active(the_repository, ce->name))
			जारी;

		ALLOC_GROW(active_modules.entries,
			   active_modules.nr + 1,
			   active_modules.alloc);
		active_modules.entries[active_modules.nr++] = ce;
	}

	module_list_release(list);
	*list = active_modules;
}

स्थिर वर्ण *get_up_path(अचर वर्ण *path)
{
	संरचना strbuf sb = STRBUF_INIT;

	strbuf_addstrings(&sb, "../", गन्ती_slashes(path));

	/*
	 * Check if 'path' ends with slash or not
	 * for having the same output for dir/sub_dir
	 * and dir/sub_dir/
	 */
	यदि (!is_dir_sep(path[म_माप(path) - 1]))
		strbuf_addstr(&sb, "../");

	फर्काऊ strbuf_detach(&sb, रिक्त);
}

स्थिर शून्य for_each_listed_submodule(अचर संरचना module_list *list,
				      each_submodule_fn fn, शून्य *cb_data)
{
	पूर्णाङ्क i;

	लागि (i = 0; i < list->nr; i++)
		fn(list->entries[i], cb_data);
}

संरचना foreach_cb {
	पूर्णाङ्क argc;
	अचर वर्ण **argv;
	अचर वर्ण *prefix;
	अचर वर्ण *super_prefix;
	पूर्णाङ्क quiet;
	पूर्णाङ्क recursive;
};
#define FOREACH_CB_INIT { 0 }

स्थिर शून्य runcommand_in_submodule_cb(अचर संरचना cache_entry *list_item,
				       शून्य *cb_data)
{
	संरचना foreach_cb *info = cb_data;
	अचर वर्ण *path = list_item->name;
	अचर संरचना object_id *ce_oid = &list_item->oid;
	अचर संरचना submodule *sub;
	संरचना child_process cp = CHILD_PROCESS_INIT;
	वर्ण *displaypath;

	यदि (validate_submodule_path(path) < 0)
		die(रिक्त);

	displaypath = get_submodule_displaypath(path, info->prefix,
						info->super_prefix);

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);

	यदि (!sub)
		die(_("No url found for submodule path '%s' in .gitmodules"),
			displaypath);

	यदि (!is_submodule_populated_gently(path, रिक्त))
		गच्छ cleanup;

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
	यदि (info->argc == 1) {
		वर्ण *toplevel = xgetcwd();
		संरचना strbuf sb = STRBUF_INIT;

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
		मुक्त(toplevel);
	} अन्यथा {
		strvec_pushv(&cp.args, info->argv);
	}

	यदि (!info->quiet)
		म_लिखो(_("Entering '%s'\n"), displaypath);

	यदि (info->argv[0]) {
		यदि (run_command(&cp))
			die(_("run_command returned non-zero status for %s\n."),
			    displaypath);
	} अन्यथा {
		child_process_clear(&cp);
	}

	यदि (info->recursive) {
		संरचना child_process cpr = CHILD_PROCESS_INIT;

		cpr.git_cmd = 1;
		cpr.dir = path;
		prepare_submodule_repo_env(&cpr.env);

		strvec_pushl(&cpr.args, "submodule--helper", "foreach", "--recursive",
			     रिक्त);
		strvec_pushf(&cpr.args, "--super-prefix=%s/", displaypath);

		यदि (info->quiet)
			strvec_थप(&cpr.args, "--quiet");

		strvec_थप(&cpr.args, "--");
		strvec_pushv(&cpr.args, info->argv);

		यदि (run_command(&cpr))
			die(_("run_command returned non-zero status while "
				"recursing in the nested submodules of %s\n."),
				displaypath);
	}

cleanup:
	मुक्त(displaypath);
}

स्थिर पूर्णाङ्क module_foreach(पूर्णाङ्क argc, अचर वर्ण **argv, अचर वर्ण *prefix,
			  संरचना repository *repo UNUSED)
{
	संरचना foreach_cb info = FOREACH_CB_INIT;
	संरचना pathspec pathspec = { 0 };
	संरचना module_list list = MODULE_LIST_INIT;
	संरचना option module_foreach_options[] = {
		OPT__SUPER_PREFIX(&info.super_prefix),
		OPT__QUIET(&info.quiet, N_("suppress output of entering each submodule command")),
		OPT_BOOL(0, "recursive", &info.recursive,
			 N_("recurse into nested submodules")),
		OPT_END()
	};
	अचर वर्ण *अचर git_submodule_helper_usage[] = {
		N_("git submodule foreach [--quiet] [--recursive] [--] <command>"),
		रिक्त
	};
	पूर्णाङ्क ret = 1;

	argc = parse_options(argc, argv, prefix, module_foreach_options,
			     git_submodule_helper_usage, 0);

	यदि (module_list_compute(रिक्त, prefix, &pathspec, &list) < 0)
		गच्छ cleanup;

	info.argc = argc;
	info.argv = argv;
	info.prefix = prefix;

	for_each_listed_submodule(&list, runcommand_in_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	फर्काऊ ret;
}

संरचना init_cb {
	अचर वर्ण *prefix;
	अचर वर्ण *super_prefix;
	अचिन्हित पूर्णाङ्क flags;
};
#define INIT_CB_INIT { 0 }

स्थिर पूर्णाङ्क validate_and_set_submodule_gitdir(संरचना strbuf *gitdir_path,
					     अचर वर्ण *submodule_name)
{
	अचर वर्ण *मान;
	वर्ण *साँचो;

	यदि (validate_submodule_git_dir(gitdir_path->buf, submodule_name))
		फर्काऊ -1;

	साँचो = xstrfmt("submodule.%s.gitdir", submodule_name);

	/* Nothing to do if the config already exists. */
	यदि (!repo_config_get_string_tmp(the_repository, साँचो, &मान)) {
		मुक्त(साँचो);
		फर्काऊ 0;
	}

	यदि (repo_config_set_gently(the_repository, साँचो, gitdir_path->buf)) {
		मुक्त(साँचो);
		फर्काऊ -1;
	}

	मुक्त(साँचो);
	फर्काऊ 0;
}

स्थिर शून्य create_default_gitdir_config(अचर वर्ण *submodule_name)
{
	संरचना strbuf gitdir_path = STRBUF_INIT;
	संरचना git_hash_ctx ctx;
	वर्ण hex_name_hash[GIT_MAX_HEXSZ + 1], header[128];
	अचिन्हित वर्ण raw_name_hash[GIT_MAX_RAWSZ];
	पूर्णाङ्क header_len;

	/* Case 1: try the plain module name */
	repo_git_path_append(the_repository, &gitdir_path, "modules/%s", submodule_name);
	यदि (!validate_and_set_submodule_gitdir(&gitdir_path, submodule_name)) {
		strbuf_release(&gitdir_path);
		फर्काऊ;
	}

	/* Case 2.1: Try URI-safe (RFC3986) encoding first, this fixes nested gitdirs */
	strbuf_reset(&gitdir_path);
	repo_git_path_append(the_repository, &gitdir_path, "modules/");
	strbuf_addstr_urlencode(&gitdir_path, submodule_name, is_rfc3986_unreserved);
	यदि (!validate_and_set_submodule_gitdir(&gitdir_path, submodule_name)) {
		strbuf_release(&gitdir_path);
		फर्काऊ;
	}

	/* Case 2.2: Try extended uppercase URI (RFC3986) encoding, to fix case-folding */
	strbuf_reset(&gitdir_path);
	repo_git_path_append(the_repository, &gitdir_path, "modules/");
	strbuf_addstr_urlencode(&gitdir_path, submodule_name, is_casefolding_rfc3986_unreserved);
	यदि (!validate_and_set_submodule_gitdir(&gitdir_path, submodule_name))
		फर्काऊ;

	/* Case 2.3: Try some derived gitdir names, see if one sticks */
	लागि (वर्ण c = '0'; c <= '9'; c++) {
		strbuf_reset(&gitdir_path);
		repo_git_path_append(the_repository, &gitdir_path, "modules/");
		strbuf_addstr_urlencode(&gitdir_path, submodule_name, is_rfc3986_unreserved);
		strbuf_addch(&gitdir_path, c);
		यदि (!validate_and_set_submodule_gitdir(&gitdir_path, submodule_name))
			फर्काऊ;

		strbuf_reset(&gitdir_path);
		repo_git_path_append(the_repository, &gitdir_path, "modules/");
		strbuf_addstr_urlencode(&gitdir_path, submodule_name, is_casefolding_rfc3986_unreserved);
		strbuf_addch(&gitdir_path, c);
		यदि (!validate_and_set_submodule_gitdir(&gitdir_path, submodule_name))
			फर्काऊ;
	}

	/* Case 2.4: If all the above failed, try a hash of the name as a last resort */
	header_len = snprintf(header, परिमाण(header),
			      "blob %"PRIuMAX, (uintmax_t)म_माप(submodule_name));
	git_hash_init(&ctx, the_hash_algo);
	git_hash_update(&ctx, header, header_len);
	git_hash_update(&ctx, "\0", 1);
	git_hash_update(&ctx, submodule_name, म_माप(submodule_name));
	git_hash_final(raw_name_hash, &ctx);
	hash_to_hex_algop_r(hex_name_hash, raw_name_hash, the_hash_algo);
	strbuf_reset(&gitdir_path);
	repo_git_path_append(the_repository, &gitdir_path, "modules/%s", hex_name_hash);
	यदि (!validate_and_set_submodule_gitdir(&gitdir_path, submodule_name)) {
		strbuf_release(&gitdir_path);
		फर्काऊ;
	}

	/* Case 3: nothing worked, error out */
	die(_("failed to set a valid default config for 'submodule.%s.gitdir'. "
	      "Please ensure it is set, for example by running something like: "
	      "'git config submodule.%s.gitdir .git/modules/%s'"),
	    submodule_name, submodule_name, submodule_name);
}

स्थिर शून्य init_submodule(अचर वर्ण *path, अचर वर्ण *prefix,
			   अचर वर्ण *super_prefix,
			   अचिन्हित पूर्णाङ्क flags)
{
	अचर संरचना submodule *sub;
	संरचना strbuf sb = STRBUF_INIT;
	अचर वर्ण *upd;
	वर्ण *url = रिक्त, *displaypath;

	displaypath = get_submodule_displaypath(path, prefix, super_prefix);

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);

	यदि (!sub)
		die(_("No url found for submodule path '%s' in .gitmodules"),
			displaypath);

	/*
	 * NEEDSWORK: In a multi-working-tree world, this needs to be
	 * set in the per-worktree config.
	 *
	 * Set active flag for the submodule being initialized
	 */
	यदि (!is_submodule_active(the_repository, path)) {
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
	यदि (repo_config_get_string(the_repository, sb.buf, &url)) {
		यदि (!sub->url)
			die(_("No url found for submodule path '%s' in .gitmodules"),
				displaypath);

		url = xstrdup(sub->url);

		/* Possibly a url relative to parent */
		यदि (starts_with_dot_dot_slash(url) ||
		    starts_with_dot_slash(url)) {
			वर्ण *oldurl = url;

			url = resolve_relative_url(oldurl, रिक्त, 0);
			मुक्त(oldurl);
		}

		यदि (repo_config_set_gently(the_repository, sb.buf, url))
			die(_("Failed to register url for submodule path '%s'"),
			    displaypath);
		यदि (!(flags & OPT_QUIET))
			ख_लिखो(मानक_त्रुटि,
				_("Submodule '%s' (%s) registered for path '%s'\n"),
				sub->name, url, displaypath);
	}
	strbuf_reset(&sb);

	/* Copy "update" setting when it is not set yet */
	strbuf_addf(&sb, "submodule.%s.update", sub->name);
	यदि (repo_config_get_string_tmp(the_repository, sb.buf, &upd) &&
	    sub->update_strategy.type != SM_UPDATE_UNSPECIFIED) {
		यदि (sub->update_strategy.type == SM_UPDATE_COMMAND) {
			ख_लिखो(मानक_त्रुटि, _("warning: command update mode suggested for submodule '%s'\n"),
				sub->name);
			upd = "none";
		} अन्यथा {
			upd = submodule_update_type_to_string(sub->update_strategy.type);
		}

		यदि (repo_config_set_gently(the_repository, sb.buf, upd))
			die(_("Failed to register update mode for submodule path '%s'"), displaypath);
	}

	यदि (the_repository->repository_format_submodule_path_cfg)
		create_default_gitdir_config(sub->name);

	strbuf_release(&sb);
	मुक्त(displaypath);
	मुक्त(url);
}

स्थिर शून्य init_submodule_cb(अचर संरचना cache_entry *list_item, शून्य *cb_data)
{
	संरचना init_cb *info = cb_data;

	init_submodule(list_item->name, info->prefix, info->super_prefix,
		       info->flags);
}

स्थिर पूर्णाङ्क module_init(पूर्णाङ्क argc, अचर वर्ण **argv, अचर वर्ण *prefix,
		       संरचना repository *repo UNUSED)
{
	संरचना init_cb info = INIT_CB_INIT;
	संरचना pathspec pathspec = { 0 };
	संरचना module_list list = MODULE_LIST_INIT;
	पूर्णाङ्क quiet = 0;
	संरचना option module_init_options[] = {
		OPT__QUIET(&quiet, N_("suppress output for initializing a submodule")),
		OPT_END()
	};
	अचर वर्ण *अचर git_submodule_helper_usage[] = {
		N_("git submodule init [<options>] [<path>]"),
		रिक्त
	};
	पूर्णाङ्क ret = 1;

	argc = parse_options(argc, argv, prefix, module_init_options,
			     git_submodule_helper_usage, 0);

	यदि (module_list_compute(argv, prefix, &pathspec, &list) < 0)
		गच्छ cleanup;

	/*
	 * If there are no path args and submodule.active is set then,
	 * by default, only initialize 'active' modules.
	 */
	यदि (!argc && !repo_config_get(the_repository, "submodule.active"))
		module_list_active(&list);

	info.prefix = prefix;
	यदि (quiet)
		info.flags |= OPT_QUIET;

	for_each_listed_submodule(&list, init_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	फर्काऊ ret;
}

संरचना status_cb {
	अचर वर्ण *prefix;
	अचर वर्ण *super_prefix;
	अचिन्हित पूर्णाङ्क flags;
};
#define STATUS_CB_INIT { 0 }

स्थिर शून्य print_status(अचिन्हित पूर्णाङ्क flags, वर्ण state, अचर वर्ण *path,
			 अचर संरचना object_id *oid, अचर वर्ण *displaypath)
{
	यदि (flags & OPT_QUIET)
		फर्काऊ;

	म_लिखो("%c%s %s", state, oid_to_hex(oid), displaypath);

	यदि (state == ' ' || state == '+') {
		वर्ण *name = compute_rev_name(path, oid_to_hex(oid));

		यदि (name)
			म_लिखो(" (%s)", name);
		मुक्त(name);
	}

	म_लिखो("\n");
}

स्थिर पूर्णाङ्क handle_submodule_head_ref(अचर संरचना reference *ref, शून्य *cb_data)
{
	संरचना object_id *output = cb_data;

	यदि (ref->oid)
		oidcpy(output, ref->oid);

	फर्काऊ 0;
}

स्थिर शून्य status_submodule(अचर वर्ण *path, अचर संरचना object_id *ce_oid,
			     अचिन्हित पूर्णाङ्क ce_flags, अचर वर्ण *prefix,
			     अचर वर्ण *super_prefix, अचिन्हित पूर्णाङ्क flags)
{
	वर्ण *displaypath;
	संरचना strvec diff_files_args = STRVEC_INIT;
	संरचना rev_info rev = REV_INFO_INIT;
	संरचना strbuf buf = STRBUF_INIT;
	अचर वर्ण *git_dir;

	यदि (validate_submodule_path(path) < 0)
		die(रिक्त);

	यदि (!submodule_from_path(the_repository, null_oid(the_hash_algo), path))
		die(_("no submodule mapping found in .gitmodules for path '%s'"),
		      path);

	displaypath = get_submodule_displaypath(path, prefix, super_prefix);

	यदि ((CE_STAGEMASK & ce_flags) >> CE_STAGESHIFT) {
		print_status(flags, 'U', path, null_oid(the_hash_algo), displaypath);
		गच्छ cleanup;
	}

	strbuf_addf(&buf, "%s/.git", path);
	git_dir = read_gitfile(buf.buf);
	यदि (!git_dir)
		git_dir = buf.buf;

	यदि (!is_submodule_active(the_repository, path) ||
	    !is_git_directory(git_dir)) {
		print_status(flags, '-', path, ce_oid, displaypath);
		strbuf_release(&buf);
		गच्छ cleanup;
	}
	strbuf_release(&buf);

	strvec_pushl(&diff_files_args, "diff-files",
		     "--ignore-submodules=dirty", "--quiet", "--",
		     path, रिक्त);

	repo_config(the_repository, git_diff_basic_config, रिक्त);

	repo_init_revisions(the_repository, &rev, रिक्त);
	rev.abbrev = 0;
	setup_revisions_from_strvec(&diff_files_args, &rev, रिक्त);
	run_diff_files(&rev, 0);

	यदि (!diff_result_code(&rev)) {
		print_status(flags, ' ', path, ce_oid,
			     displaypath);
	} अन्यथा यदि (!(flags & OPT_CACHED)) {
		संरचना object_id oid;
		संरचना ref_store *refs = repo_get_submodule_ref_store(the_repository,
								      path);

		यदि (!refs) {
			print_status(flags, '-', path, ce_oid, displaypath);
			गच्छ cleanup;
		}
		यदि (refs_head_ref(refs, handle_submodule_head_ref, &oid))
			die(_("could not resolve HEAD ref inside the "
			      "submodule '%s'"), path);

		print_status(flags, '+', path, &oid, displaypath);
	} अन्यथा {
		print_status(flags, '+', path, ce_oid, displaypath);
	}

	यदि (flags & OPT_RECURSIVE) {
		संरचना child_process cpr = CHILD_PROCESS_INIT;
		पूर्णाङ्क res;

		cpr.git_cmd = 1;
		cpr.dir = path;
		prepare_submodule_repo_env(&cpr.env);

		strvec_pushl(&cpr.args, "submodule--helper", "status",
			     "--recursive", रिक्त);
		strvec_pushf(&cpr.args, "--super-prefix=%s/", displaypath);

		यदि (flags & OPT_CACHED)
			strvec_थप(&cpr.args, "--cached");

		यदि (flags & OPT_QUIET)
			strvec_थप(&cpr.args, "--quiet");

		res = run_command(&cpr);
		यदि (res == SIGPIPE + 128)
			उत्थापय(SIGPIPE);
		अन्यथा यदि (res)
			die(_("failed to recurse into submodule '%s'"), path);
	}

cleanup:
	strvec_clear(&diff_files_args);
	मुक्त(displaypath);
	release_revisions(&rev);
}

स्थिर शून्य status_submodule_cb(अचर संरचना cache_entry *list_item,
				शून्य *cb_data)
{
	संरचना status_cb *info = cb_data;

	status_submodule(list_item->name, &list_item->oid, list_item->ce_flags,
			 info->prefix, info->super_prefix, info->flags);
}

स्थिर पूर्णाङ्क module_status(पूर्णाङ्क argc, अचर वर्ण **argv, अचर वर्ण *prefix,
			 संरचना repository *repo UNUSED)
{
	संरचना status_cb info = STATUS_CB_INIT;
	संरचना pathspec pathspec = { 0 };
	संरचना module_list list = MODULE_LIST_INIT;
	पूर्णाङ्क quiet = 0;
	संरचना option module_status_options[] = {
		OPT__SUPER_PREFIX(&info.super_prefix),
		OPT__QUIET(&quiet, N_("suppress submodule status output")),
		OPT_BIT(0, "cached", &info.flags, N_("use commit stored in the index instead of the one stored in the submodule HEAD"), OPT_CACHED),
		OPT_BIT(0, "recursive", &info.flags, N_("recurse into nested submodules"), OPT_RECURSIVE),
		OPT_END()
	};
	अचर वर्ण *अचर git_submodule_helper_usage[] = {
		N_("git submodule status [--quiet] [--cached] [--recursive] [<path>...]"),
		रिक्त
	};
	पूर्णाङ्क ret = 1;

	argc = parse_options(argc, argv, prefix, module_status_options,
			     git_submodule_helper_usage, 0);

	यदि (module_list_compute(argv, prefix, &pathspec, &list) < 0)
		गच्छ cleanup;

	info.prefix = prefix;
	यदि (quiet)
		info.flags |= OPT_QUIET;

	for_each_listed_submodule(&list, status_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	फर्काऊ ret;
}

संरचना module_cb {
	अचिन्हित पूर्णाङ्क mod_src;
	अचिन्हित पूर्णाङ्क mod_dst;
	संरचना object_id oid_src;
	संरचना object_id oid_dst;
	वर्ण status;
	वर्ण *sm_path;
};
#define MODULE_CB_INIT { 0 }

स्थिर शून्य module_cb_release(संरचना module_cb *mcb)
{
	मुक्त(mcb->sm_path);
}

संरचना module_cb_list {
	संरचना module_cb **entries;
	पूर्णाङ्क alloc, nr;
};
#define MODULE_CB_LIST_INIT { 0 }

स्थिर शून्य module_cb_list_release(संरचना module_cb_list *mcbl)
{
	पूर्णाङ्क i;

	लागि (i = 0; i < mcbl->nr; i++) {
		संरचना module_cb *mcb = mcbl->entries[i];

		module_cb_release(mcb);
		मुक्त(mcb);
	}
	मुक्त(mcbl->entries);
}

संरचना summary_cb {
	पूर्णाङ्क argc;
	अचर वर्ण **argv;
	अचर वर्ण *prefix;
	अचर वर्ण *super_prefix;
	अचिन्हित पूर्णाङ्क cached: 1;
	अचिन्हित पूर्णाङ्क for_status: 1;
	अचिन्हित पूर्णाङ्क files: 1;
	पूर्णाङ्क summary_limit;
};
#define SUMMARY_CB_INIT { 0 }

गणना diff_cmd {
	DIFF_INDEX,
	DIFF_FILES
};

स्थिर वर्ण *verify_submodule_committish(अचर वर्ण *sm_path,
					 अचर वर्ण *committish)
{
	संरचना child_process cp_rev_parse = CHILD_PROCESS_INIT;
	संरचना strbuf result = STRBUF_INIT;

	cp_rev_parse.git_cmd = 1;
	cp_rev_parse.dir = sm_path;
	prepare_submodule_repo_env(&cp_rev_parse.env);
	strvec_pushl(&cp_rev_parse.args, "rev-parse", "-q", "--short", रिक्त);
	strvec_pushf(&cp_rev_parse.args, "%s^0", committish);
	strvec_थप(&cp_rev_parse.args, "--");

	यदि (capture_command(&cp_rev_parse, &result, 0))
		फर्काऊ रिक्त;

	strbuf_trim_trailing_newline(&result);
	फर्काऊ strbuf_detach(&result, रिक्त);
}

स्थिर शून्य print_submodule_summary(संरचना summary_cb *info, अचर वर्ण *errmsg,
				    पूर्णाङ्क total_commits, अचर वर्ण *displaypath,
				    अचर वर्ण *src_abbrev, अचर वर्ण *dst_abbrev,
				    संरचना module_cb *p)
{
	यदि (p->status == 'T') {
		यदि (S_ISGITLINK(p->mod_dst))
			म_लिखो(_("* %s %s(blob)->%s(submodule)"),
				 displaypath, src_abbrev, dst_abbrev);
		अन्यथा
			म_लिखो(_("* %s %s(submodule)->%s(blob)"),
				 displaypath, src_abbrev, dst_abbrev);
	} अन्यथा {
		म_लिखो("* %s %s...%s",
			displaypath, src_abbrev, dst_abbrev);
	}

	यदि (total_commits < 0)
		म_लिखो(":\n");
	अन्यथा
		म_लिखो(" (%d):\n", total_commits);

	यदि (errmsg) {
		म_लिखो(_("%s"), errmsg);
	} अन्यथा यदि (total_commits > 0) {
		संरचना child_process cp_log = CHILD_PROCESS_INIT;

		cp_log.git_cmd = 1;
		cp_log.dir = p->sm_path;
		prepare_submodule_repo_env(&cp_log.env);
		strvec_pushl(&cp_log.args, "log", रिक्त);

		यदि (S_ISGITLINK(p->mod_src) && S_ISGITLINK(p->mod_dst)) {
			यदि (info->summary_limit > 0)
				strvec_pushf(&cp_log.args, "-%d",
					     info->summary_limit);

			strvec_pushl(&cp_log.args, "--pretty=  %m %s",
				     "--first-parent", रिक्त);
			strvec_pushf(&cp_log.args, "%s...%s",
				     src_abbrev, dst_abbrev);
		} अन्यथा यदि (S_ISGITLINK(p->mod_dst)) {
			strvec_pushl(&cp_log.args, "--pretty=  > %s",
				     "-1", dst_abbrev, रिक्त);
		} अन्यथा {
			strvec_pushl(&cp_log.args, "--pretty=  < %s",
				     "-1", src_abbrev, रिक्त);
		}
		run_command(&cp_log);
	}
	म_लिखो("\n");
}

स्थिर शून्य generate_submodule_summary(संरचना summary_cb *info,
				       संरचना module_cb *p)
{
	वर्ण *displaypath, *src_abbrev = रिक्त, *dst_abbrev;
	पूर्णाङ्क missing_src = 0, missing_dst = 0;
	संरचना strbuf errmsg = STRBUF_INIT;
	पूर्णाङ्क total_commits = -1;

	यदि (!info->cached && oideq(&p->oid_dst, null_oid(the_hash_algo))) {
		यदि (S_ISGITLINK(p->mod_dst)) {
			संरचना ref_store *refs = repo_get_submodule_ref_store(the_repository,
									      p->sm_path);

			यदि (refs)
				refs_head_ref(refs, handle_submodule_head_ref, &p->oid_dst);
		} अन्यथा यदि (S_ISLNK(p->mod_dst) || S_ISREG(p->mod_dst)) {
			संरचना stat st;
			पूर्णाङ्क fd = खोलो(p->sm_path, O_RDONLY);

			यदि (fd < 0 || ख_स्थिति(fd, &st) < 0 ||
			    index_fd(the_repository->index, &p->oid_dst, fd, &st, OBJ_BLOB,
				     p->sm_path, 0))
				error(_("couldn't hash object from '%s'"), p->sm_path);
		} अन्यथा {
			/* for a submodule removal (mode:0000000), don't warn */
			यदि (p->mod_dst)
				warning(_("unexpected mode %o"), p->mod_dst);
		}
	}

	यदि (S_ISGITLINK(p->mod_src)) {
		यदि (p->status != 'D')
			src_abbrev = verify_submodule_committish(p->sm_path,
								 oid_to_hex(&p->oid_src));
		यदि (!src_abbrev) {
			missing_src = 1;
			/*
			 * As `rev-parse` failed, we fallback to getting
			 * the abbreviated hash using oid_src. We do
			 * this as we might still need the abbreviated
			 * hash in cases like a submodule type change, etc.
			 */
			src_abbrev = xstrndup(oid_to_hex(&p->oid_src), 7);
		}
	} अन्यथा {
		/*
		 * The source does not point to a submodule.
		 * So, we fallback to getting the abbreviation using
		 * oid_src as we might still need the abbreviated
		 * hash in cases like submodule add, etc.
		 */
		src_abbrev = xstrndup(oid_to_hex(&p->oid_src), 7);
	}

	यदि (S_ISGITLINK(p->mod_dst)) {
		dst_abbrev = verify_submodule_committish(p->sm_path,
							 oid_to_hex(&p->oid_dst));
		यदि (!dst_abbrev) {
			missing_dst = 1;
			/*
			 * As `rev-parse` failed, we fallback to getting
			 * the abbreviated hash using oid_dst. We do
			 * this as we might still need the abbreviated
			 * hash in cases like a submodule type change, etc.
			 */
			dst_abbrev = xstrndup(oid_to_hex(&p->oid_dst), 7);
		}
	} अन्यथा {
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

	यदि (!missing_src && !missing_dst) {
		संरचना child_process cp_rev_list = CHILD_PROCESS_INIT;
		संरचना strbuf sb_rev_list = STRBUF_INIT;

		strvec_pushl(&cp_rev_list.args, "rev-list",
			     "--first-parent", "--count", रिक्त);
		यदि (S_ISGITLINK(p->mod_src) && S_ISGITLINK(p->mod_dst))
			strvec_pushf(&cp_rev_list.args, "%s...%s",
				     src_abbrev, dst_abbrev);
		अन्यथा
			strvec_थप(&cp_rev_list.args, S_ISGITLINK(p->mod_src) ?
				    src_abbrev : dst_abbrev);
		strvec_थप(&cp_rev_list.args, "--");

		cp_rev_list.git_cmd = 1;
		cp_rev_list.dir = p->sm_path;
		prepare_submodule_repo_env(&cp_rev_list.env);

		यदि (!capture_command(&cp_rev_list, &sb_rev_list, 0))
			total_commits = म_से_प(sb_rev_list.buf);

		strbuf_release(&sb_rev_list);
	} अन्यथा {
		/*
		 * Don't give error msg for modification whose dst is not
		 * submodule, i.e., deleted or changed to blob
		 */
		यदि (S_ISGITLINK(p->mod_dst)) {
			यदि (missing_src && missing_dst) {
				strbuf_addf(&errmsg, "  Warn: %s doesn't contain commits %s and %s\n",
					    displaypath, oid_to_hex(&p->oid_src),
					    oid_to_hex(&p->oid_dst));
			} अन्यथा {
				strbuf_addf(&errmsg, "  Warn: %s doesn't contain commit %s\n",
					    displaypath, missing_src ?
					    oid_to_hex(&p->oid_src) :
					    oid_to_hex(&p->oid_dst));
			}
		}
	}

	print_submodule_summary(info, errmsg.len ? errmsg.buf : रिक्त,
				total_commits, displaypath, src_abbrev,
				dst_abbrev, p);

	मुक्त(displaypath);
	मुक्त(src_abbrev);
	मुक्त(dst_abbrev);
	strbuf_release(&errmsg);
}

स्थिर शून्य prepare_submodule_summary(संरचना summary_cb *info,
				      संरचना module_cb_list *list)
{
	पूर्णाङ्क i;
	लागि (i = 0; i < list->nr; i++) {
		अचर संरचना submodule *sub;
		संरचना module_cb *p = list->entries[i];
		संरचना strbuf sm_gitdir = STRBUF_INIT;

		यदि (p->status == 'D' || p->status == 'T') {
			generate_submodule_summary(info, p);
			जारी;
		}

		यदि (info->for_status && p->status != 'A' &&
		    (sub = submodule_from_path(the_repository,
					       null_oid(the_hash_algo), p->sm_path))) {
			वर्ण *config_साँचो = रिक्त;
			अचर वर्ण *मान;
			पूर्णाङ्क ignore_all = 0;

			config_साँचो = xstrfmt("submodule.%s.ignore",
					     sub->name);
			यदि (!repo_config_get_string_tmp(the_repository, config_साँचो, &मान))
				ignore_all = !म_भेद(मान, "all");
			अन्यथा यदि (sub->ignore)
				ignore_all = !म_भेद(sub->ignore, "all");

			मुक्त(config_साँचो);
			यदि (ignore_all)
				जारी;
		}

		/* Also show added or modified modules which are checked out */
		strbuf_addstr(&sm_gitdir, p->sm_path);
		यदि (is_nonbare_repository_dir(&sm_gitdir))
			generate_submodule_summary(info, p);
		strbuf_release(&sm_gitdir);
	}
}

स्थिर शून्य submodule_summary_callback(संरचना diff_queue_struct *q,
				       संरचना diff_options *options UNUSED,
				       शून्य *data)
{
	पूर्णाङ्क i;
	संरचना module_cb_list *list = data;
	लागि (i = 0; i < q->nr; i++) {
		संरचना diff_filepair *p = q->queue[i];
		संरचना module_cb *temp;

		यदि (!S_ISGITLINK(p->one->mode) && !S_ISGITLINK(p->two->mode))
			जारी;
		temp = xmalloc(परिमाण(*temp));
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

स्थिर अचर वर्ण *get_diff_cmd(गणना diff_cmd diff_cmd)
{
	विकल्प (diff_cmd) {
	पक्ष DIFF_INDEX: फर्काऊ "diff-index";
	पक्ष DIFF_FILES: फर्काऊ "diff-files";
	स्वभाव: BUG("bad diff_cmd value %d", diff_cmd);
	}
}

स्थिर पूर्णाङ्क compute_summary_module_list(संरचना object_id *head_oid,
				       संरचना summary_cb *info,
				       गणना diff_cmd diff_cmd)
{
	संरचना strvec diff_args = STRVEC_INIT;
	संरचना rev_info rev;
	संरचना module_cb_list list = MODULE_CB_LIST_INIT;
	पूर्णाङ्क ret = 0;

	strvec_थप(&diff_args, get_diff_cmd(diff_cmd));
	यदि (info->cached)
		strvec_थप(&diff_args, "--cached");
	strvec_pushl(&diff_args, "--ignore-submodules=dirty", "--raw", रिक्त);
	यदि (head_oid)
		strvec_थप(&diff_args, oid_to_hex(head_oid));
	strvec_थप(&diff_args, "--");
	यदि (info->argc)
		strvec_pushv(&diff_args, info->argv);

	repo_config(the_repository, git_diff_basic_config, रिक्त);
	repo_init_revisions(the_repository, &rev, info->prefix);
	rev.abbrev = 0;
	precompose_argv_prefix(diff_args.nr, diff_args.v, रिक्त);
	setup_revisions_from_strvec(&diff_args, &rev, रिक्त);
	rev.diffopt.output_format = DIFF_FORMAT_NO_OUTPUT | DIFF_FORMAT_CALLBACK;
	rev.diffopt.format_callback = submodule_summary_callback;
	rev.diffopt.format_callback_data = &list;

	यदि (!info->cached) {
		यदि (diff_cmd == DIFF_INDEX)
			setup_work_रूख(the_repository);
		यदि (repo_read_index_preload(the_repository, &rev.diffopt.pathspec, 0) < 0) {
			लिखो_त्रुटि("repo_read_index_preload");
			ret = -1;
			गच्छ cleanup;
		}
	} अन्यथा यदि (repo_read_index(the_repository) < 0) {
		लिखो_त्रुटि("repo_read_cache");
		ret = -1;
		गच्छ cleanup;
	}

	यदि (diff_cmd == DIFF_INDEX)
		run_diff_index(&rev, info->cached ? DIFF_INDEX_CACHED : 0);
	अन्यथा
		run_diff_files(&rev, 0);
	prepare_submodule_summary(info, &list);
cleanup:
	strvec_clear(&diff_args);
	release_revisions(&rev);
	module_cb_list_release(&list);
	फर्काऊ ret;
}

स्थिर पूर्णाङ्क module_summary(पूर्णाङ्क argc, अचर वर्ण **argv, अचर वर्ण *prefix,
			  संरचना repository *repo UNUSED)
{
	संरचना summary_cb info = SUMMARY_CB_INIT;
	पूर्णाङ्क cached = 0;
	पूर्णाङ्क for_status = 0;
	पूर्णाङ्क files = 0;
	पूर्णाङ्क summary_limit = -1;
	गणना diff_cmd diff_cmd = DIFF_INDEX;
	संरचना object_id head_oid;
	पूर्णाङ्क ret;
	संरचना option module_summary_options[] = {
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
	अचर वर्ण *अचर git_submodule_helper_usage[] = {
		N_("git submodule summary [<options>] [<commit>] [--] [<path>]"),
		रिक्त
	};

	argc = parse_options(argc, argv, prefix, module_summary_options,
			     git_submodule_helper_usage, 0);

	यदि (!summary_limit)
		फर्काऊ 0;

	यदि (!repo_get_oid(the_repository, argc ? argv[0] : "HEAD", &head_oid)) {
		यदि (argc) {
			argv++;
			argc--;
		}
	} अन्यथा यदि (!argc || !म_भेद(argv[0], "HEAD")) {
		/* before the first commit: compare with an empty tree */
		oidcpy(&head_oid, the_hash_algo->empty_रूख);
		यदि (argc) {
			argv++;
			argc--;
		}
	} अन्यथा {
		यदि (repo_get_oid(the_repository, "HEAD", &head_oid))
			die(_("could not fetch a revision for HEAD"));
	}

	यदि (files) {
		यदि (cached)
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

	ret = compute_summary_module_list((diff_cmd == DIFF_INDEX) ? &head_oid : रिक्त,
					  &info, diff_cmd);
	फर्काऊ ret;
}

स्थिर पूर्णाङ्क module_gitdir(पूर्णाङ्क argc, अचर वर्ण **argv, अचर वर्ण *prefix UNUSED,
			 संरचना repository *repo)
{
	संरचना strbuf gitdir = STRBUF_INIT;

	यदि (argc != 2)
		usage(_("git submodule--helper gitdir <name>"));

	submodule_name_to_gitdir(&gitdir, repo, argv[1]);

	म_लिखो("%s\n", gitdir.buf);

	strbuf_release(&gitdir);
	फर्काऊ 0;
}

स्थिर पूर्णाङ्क module_migrate(पूर्णाङ्क argc UNUSED, अचर वर्ण **argv UNUSED,
			  अचर वर्ण *prefix UNUSED, संरचना repository *repo)
{
	संरचना strbuf module_dir = STRBUF_INIT;
	सूची *dir;
	संरचना dirent *de;
	पूर्णाङ्क repo_version = 0;

	repo_git_path_append(repo, &module_dir, "modules/");

	dir = सूची_खोलो(module_dir.buf);
	यदि (!dir)
		die(_("could not open '%s'"), module_dir.buf);

	जबसम्म ((de = सूची_पढ़ो(dir))) {
		संरचना strbuf gitdir_path = STRBUF_INIT;
		वर्ण *साँचो;
		अचर वर्ण *मान;

		यदि (is_dot_or_dotdot(de->d_name))
			जारी;

		strbuf_addf(&gitdir_path, "%s/%s", module_dir.buf, de->d_name);
		यदि (!is_git_directory(gitdir_path.buf)) {
			strbuf_release(&gitdir_path);
			जारी;
		}
		strbuf_release(&gitdir_path);

		साँचो = xstrfmt("submodule.%s.gitdir", de->d_name);
		यदि (!repo_config_get_string_tmp(repo, साँचो, &मान)) {
			/* Already has a gitdir config, nothing to do. */
			मुक्त(साँचो);
			जारी;
		}
		मुक्त(साँचो);

		create_default_gitdir_config(de->d_name);
	}

	बंद_सूची(dir);
	strbuf_release(&module_dir);

	repo_config_get_int(the_repository, "core.repositoryformatversion", &repo_version);
	यदि (repo_version == 0 &&
	    repo_config_set_gently(repo, "core.repositoryformatversion", "1"))
		die(_("could not set core.repositoryformatversion to 1.\n"
		      "Please set it for migration to work, for example:\n"
		      "git config core.repositoryformatversion 1"));

	यदि (repo_config_set_gently(repo, "extensions.submodulePathConfig", "true"))
		die(_("could not enable submodulePathConfig extension. It is required\n"
		      "for migration to work. Please enable it in the root repo:\n"
		      "git config extensions.submodulePathConfig true"));

	repo->repository_format_submodule_path_cfg = 1;

	फर्काऊ 0;
}

संरचना sync_cb {
	अचर वर्ण *prefix;
	अचर वर्ण *super_prefix;
	अचिन्हित पूर्णाङ्क flags;
};
#define SYNC_CB_INIT { 0 }

स्थिर शून्य sync_submodule(अचर वर्ण *path, अचर वर्ण *prefix,
			   अचर वर्ण *super_prefix, अचिन्हित पूर्णाङ्क flags)
{
	अचर संरचना submodule *sub;
	वर्ण *remote_साँचो = रिक्त;
	वर्ण *sub_origin_url, *super_config_url, *displaypath, *default_remote;
	संरचना strbuf sb = STRBUF_INIT;
	वर्ण *sub_config_path = रिक्त;
	पूर्णाङ्क code;

	यदि (!is_submodule_active(the_repository, path))
		फर्काऊ;

	यदि (validate_submodule_path(path) < 0)
		die(रिक्त);

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);

	यदि (sub && sub->url) {
		यदि (starts_with_dot_dot_slash(sub->url) ||
		    starts_with_dot_slash(sub->url)) {
			वर्ण *up_path = get_up_path(path);

			sub_origin_url = resolve_relative_url(sub->url, up_path, 1);
			super_config_url = resolve_relative_url(sub->url, रिक्त, 1);
			मुक्त(up_path);
		} अन्यथा {
			sub_origin_url = xstrdup(sub->url);
			super_config_url = xstrdup(sub->url);
		}
	} अन्यथा {
		sub_origin_url = xstrdup("");
		super_config_url = xstrdup("");
	}

	displaypath = get_submodule_displaypath(path, prefix, super_prefix);

	यदि (!(flags & OPT_QUIET))
		म_लिखो(_("Synchronizing submodule url for '%s'\n"),
			 displaypath);

	strbuf_reset(&sb);
	strbuf_addf(&sb, "submodule.%s.url", sub->name);
	यदि (repo_config_set_gently(the_repository, sb.buf, super_config_url))
		die(_("failed to register url for submodule path '%s'"),
		      displaypath);

	यदि (!is_submodule_populated_gently(path, रिक्त))
		गच्छ cleanup;

	strbuf_reset(&sb);
	code = get_default_remote_submodule(path, &default_remote);
	यदि (code)
		निकास(code);

	remote_साँचो = xstrfmt("remote.%s.url", default_remote);
	मुक्त(default_remote);

	submodule_to_gitdir(the_repository, &sb, path);
	strbuf_addstr(&sb, "/config");

	यदि (repo_config_set_in_file_gently(the_repository, sb.buf, remote_साँचो, रिक्त, sub_origin_url))
		die(_("failed to update remote for submodule '%s'"),
		      path);

	यदि (flags & OPT_RECURSIVE) {
		संरचना child_process cpr = CHILD_PROCESS_INIT;

		cpr.git_cmd = 1;
		cpr.dir = path;
		prepare_submodule_repo_env(&cpr.env);

		strvec_pushl(&cpr.args, "submodule--helper", "sync",
			     "--recursive", रिक्त);
		strvec_pushf(&cpr.args, "--super-prefix=%s/", displaypath);

		यदि (flags & OPT_QUIET)
			strvec_थप(&cpr.args, "--quiet");

		यदि (run_command(&cpr))
			die(_("failed to recurse into submodule '%s'"),
			      path);
	}

cleanup:
	मुक्त(super_config_url);
	मुक्त(sub_origin_url);
	strbuf_release(&sb);
	मुक्त(remote_साँचो);
	मुक्त(displaypath);
	मुक्त(sub_config_path);
}

स्थिर शून्य sync_submodule_cb(अचर संरचना cache_entry *list_item, शून्य *cb_data)
{
	संरचना sync_cb *info = cb_data;

	sync_submodule(list_item->name, info->prefix, info->super_prefix,
		       info->flags);
}

स्थिर पूर्णाङ्क module_sync(पूर्णाङ्क argc, अचर वर्ण **argv, अचर वर्ण *prefix,
		       संरचना repository *repo UNUSED)
{
	संरचना sync_cb info = SYNC_CB_INIT;
	संरचना pathspec pathspec = { 0 };
	संरचना module_list list = MODULE_LIST_INIT;
	पूर्णाङ्क quiet = 0;
	पूर्णाङ्क recursive = 0;
	संरचना option module_sync_options[] = {
		OPT__SUPER_PREFIX(&info.super_prefix),
		OPT__QUIET(&quiet, N_("suppress output of synchronizing submodule url")),
		OPT_BOOL(0, "recursive", &recursive,
			N_("recurse into nested submodules")),
		OPT_END()
	};
	अचर वर्ण *अचर git_submodule_helper_usage[] = {
		N_("git submodule sync [--quiet] [--recursive] [<path>]"),
		रिक्त
	};
	पूर्णाङ्क ret = 1;

	argc = parse_options(argc, argv, prefix, module_sync_options,
			     git_submodule_helper_usage, 0);

	यदि (module_list_compute(argv, prefix, &pathspec, &list) < 0)
		गच्छ cleanup;

	info.prefix = prefix;
	यदि (quiet)
		info.flags |= OPT_QUIET;
	यदि (recursive)
		info.flags |= OPT_RECURSIVE;

	for_each_listed_submodule(&list, sync_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	फर्काऊ ret;
}

संरचना deinit_cb {
	अचर वर्ण *prefix;
	अचिन्हित पूर्णाङ्क flags;
};
#define DEINIT_CB_INIT { 0 }

स्थिर शून्य deinit_submodule(अचर वर्ण *path, अचर वर्ण *prefix,
			     अचिन्हित पूर्णाङ्क flags)
{
	अचर संरचना submodule *sub;
	वर्ण *displaypath = रिक्त;
	संरचना child_process cp_config = CHILD_PROCESS_INIT;
	संरचना strbuf sb_config = STRBUF_INIT;
	वर्ण *sub_git_dir = xstrfmt("%s/.git", path);

	यदि (validate_submodule_path(path) < 0)
		die(रिक्त);

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);

	यदि (!sub || !sub->name)
		गच्छ cleanup;

	displaypath = get_submodule_displaypath(path, prefix, रिक्त);

	/* remove the submodule work tree (unless the user already did it) */
	यदि (is_directory(path)) {
		संरचना strbuf sb_rm = STRBUF_INIT;
		अचर वर्ण *format;

		यदि (is_directory(sub_git_dir)) {
			यदि (!(flags & OPT_QUIET))
				warning(_("Submodule work tree '%s' contains a .git "
					  "directory. This will be replaced with a "
					  ".git file by using absorbgitdirs."),
					displaypath);

			absorb_git_dir_into_superproject(path, रिक्त);

		}

		यदि (!(flags & OPT_FORCE)) {
			संरचना child_process cp_rm = CHILD_PROCESS_INIT;

			cp_rm.git_cmd = 1;
			strvec_pushl(&cp_rm.args, "rm", "-qn",
				     path, रिक्त);

			यदि (run_command(&cp_rm))
				die(_("Submodule work tree '%s' contains local "
				      "modifications; use '-f' to discard them"),
				      displaypath);
		}

		strbuf_addstr(&sb_rm, path);

		यदि (!remove_dir_recursively(&sb_rm, 0))
			format = _("Cleared directory '%s'\n");
		अन्यथा
			format = _("Could not remove submodule work tree '%s'\n");

		यदि (!(flags & OPT_QUIET))
			म_लिखो(format, displaypath);

		submodule_unset_core_worktree(sub);

		strbuf_release(&sb_rm);
	}

	यदि (सूची_गढ़ो(path, 0777))
		म_लिखो(_("could not create empty submodule directory %s"),
		      displaypath);

	cp_config.git_cmd = 1;
	strvec_pushl(&cp_config.args, "config", "--get-regexp", रिक्त);
	strvec_pushf(&cp_config.args, "submodule.%s\\.", sub->name);

	/* remove the .git/config entries (unless the user already did it) */
	यदि (!capture_command(&cp_config, &sb_config, 0) && sb_config.len) {
		वर्ण *sub_साँचो = xstrfmt("submodule.%s", sub->name);

		/*
		 * remove the whole section so we have a clean state when
		 * the user later decides to init this submodule again
		 */
		repo_config_rename_section_in_file(the_repository, रिक्त, sub_साँचो, रिक्त);
		यदि (!(flags & OPT_QUIET))
			म_लिखो(_("Submodule '%s' (%s) unregistered for path '%s'\n"),
				 sub->name, sub->url, displaypath);
		मुक्त(sub_साँचो);
	}

cleanup:
	मुक्त(displaypath);
	मुक्त(sub_git_dir);
	strbuf_release(&sb_config);
}

स्थिर शून्य deinit_submodule_cb(अचर संरचना cache_entry *list_item,
				शून्य *cb_data)
{
	संरचना deinit_cb *info = cb_data;
	deinit_submodule(list_item->name, info->prefix, info->flags);
}

स्थिर पूर्णाङ्क module_deinit(पूर्णाङ्क argc, अचर वर्ण **argv, अचर वर्ण *prefix,
			 संरचना repository *repo UNUSED)
{
	संरचना deinit_cb info = DEINIT_CB_INIT;
	संरचना pathspec pathspec = { 0 };
	संरचना module_list list = MODULE_LIST_INIT;
	पूर्णाङ्क quiet = 0;
	पूर्णाङ्क force = 0;
	पूर्णाङ्क all = 0;
	संरचना option module_deinit_options[] = {
		OPT__QUIET(&quiet, N_("suppress submodule status output")),
		OPT__FORCE(&force, N_("remove submodule working trees even if they contain local changes"), 0),
		OPT_BOOL(0, "all", &all, N_("unregister all submodules")),
		OPT_END()
	};
	अचर वर्ण *अचर git_submodule_helper_usage[] = {
		N_("git submodule deinit [--quiet] [-f | --force] [--all | [--] [<path>...]]"),
		रिक्त
	};
	पूर्णाङ्क ret = 1;

	argc = parse_options(argc, argv, prefix, module_deinit_options,
			     git_submodule_helper_usage, 0);

	यदि (all && argc) {
		error("pathspec and --all are incompatible");
		usage_with_options(git_submodule_helper_usage,
				   module_deinit_options);
	}

	यदि (!argc && !all)
		die(_("Use '--all' if you really want to deinitialize all submodules"));

	यदि (module_list_compute(argv, prefix, &pathspec, &list) < 0)
		गच्छ cleanup;

	info.prefix = prefix;
	यदि (quiet)
		info.flags |= OPT_QUIET;
	यदि (force)
		info.flags |= OPT_FORCE;

	for_each_listed_submodule(&list, deinit_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	फर्काऊ ret;
}

संरचना module_clone_data {
	अचर वर्ण *prefix;
	अचर वर्ण *path;
	अचर वर्ण *name;
	अचर वर्ण *url;
	पूर्णाङ्क depth;
	संरचना list_objects_filter_options *filter_options;
	गणना ref_storage_format ref_storage_format;
	अचिन्हित पूर्णाङ्क quiet: 1;
	अचिन्हित पूर्णाङ्क progress: 1;
	अचिन्हित पूर्णाङ्क dissociate: 1;
	अचिन्हित पूर्णाङ्क require_init: 1;
	पूर्णाङ्क single_branch;
};
#define MODULE_CLONE_DATA_INIT { \
	.single_branch = -1, \
	.ref_storage_format = REF_STORAGE_FORMAT_UNKNOWN, \
}

संरचना submodule_alternate_setup {
	अचर वर्ण *submodule_name;
	गणना SUBMODULE_ALTERNATE_ERROR_MODE {
		SUBMODULE_ALTERNATE_ERROR_DIE,
		SUBMODULE_ALTERNATE_ERROR_INFO,
		SUBMODULE_ALTERNATE_ERROR_IGNORE
	} error_mode;
	संरचना string_list *reference;
};
#define SUBMODULE_ALTERNATE_SETUP_INIT { \
	.error_mode = SUBMODULE_ALTERNATE_ERROR_IGNORE, \
}

स्थिर अचर वर्ण alternate_error_advice[] = N_(
"An alternate computed from a superproject's alternate is invalid.\n"
"To allow Git to clone without an alternate in such a case, set\n"
"submodule.alternateErrorStrategy to 'info' or, equivalently, clone with\n"
"'--reference-if-able' instead of '--reference'."
);

स्थिर पूर्णाङ्क add_possible_reference_from_superproject(
		संरचना odb_files_dir *alt_odb, शून्य *sas_cb)
{
	संरचना submodule_alternate_setup *sas = sas_cb;
	माप_प्रकार len;

	/*
	 * If the alternate object store is another repository, try the
	 * standard layout with .git/(modules/<name>)+/objects
	 */
	यदि (strip_suffix(alt_odb->abspath, "/objects", &len)) {
		संरचना repository alternate;
		वर्ण *sm_alternate;
		संरचना strbuf sb = STRBUF_INIT;
		संरचना strbuf err = STRBUF_INIT;
		strbuf_add(&sb, alt_odb->abspath, len);

		यदि (repo_init(&alternate, sb.buf, रिक्त) < 0)
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
		यदि (sm_alternate) {
			वर्ण *p = strbuf_detach(&sb, रिक्त);

			string_list_append(sas->reference, p)->util = p;
			मुक्त(sm_alternate);
		} अन्यथा {
			विकल्प (sas->error_mode) {
			पक्ष SUBMODULE_ALTERNATE_ERROR_DIE:
				यदि (advice_enabled(ADVICE_SUBMODULE_ALTERNATE_ERROR_STRATEGY_DIE))
					advise(_(alternate_error_advice));
				die(_("submodule '%s' cannot add alternate: %s"),
				    sas->submodule_name, err.buf);
			पक्ष SUBMODULE_ALTERNATE_ERROR_INFO:
				fprintf_ln(मानक_त्रुटि, _("submodule '%s' cannot add alternate: %s"),
					sas->submodule_name, err.buf);
			पक्ष SUBMODULE_ALTERNATE_ERROR_IGNORE:
				; /* nothing */
			}
		}

		strbuf_release(&err);
		strbuf_release(&sb);
	}

	फर्काऊ 0;
}

स्थिर शून्य prepare_possible_alternates(अचर वर्ण *sm_name,
		संरचना string_list *reference)
{
	वर्ण *sm_alternate = रिक्त, *error_strategy = रिक्त;
	संरचना submodule_alternate_setup sas = SUBMODULE_ALTERNATE_SETUP_INIT;

	repo_config_get_string(the_repository, "submodule.alternateLocation", &sm_alternate);
	यदि (!sm_alternate)
		फर्काऊ;

	repo_config_get_string(the_repository, "submodule.alternateErrorStrategy", &error_strategy);

	यदि (!error_strategy)
		error_strategy = xstrdup("die");

	sas.submodule_name = sm_name;
	sas.reference = reference;
	यदि (!म_भेद(error_strategy, "die"))
		sas.error_mode = SUBMODULE_ALTERNATE_ERROR_DIE;
	अन्यथा यदि (!म_भेद(error_strategy, "info"))
		sas.error_mode = SUBMODULE_ALTERNATE_ERROR_INFO;
	अन्यथा यदि (!म_भेद(error_strategy, "ignore"))
		sas.error_mode = SUBMODULE_ALTERNATE_ERROR_IGNORE;
	अन्यथा
		die(_("Value '%s' for submodule.alternateErrorStrategy is not recognized"), error_strategy);

	यदि (!म_भेद(sm_alternate, "superproject"))
		odb_for_each_alternate(the_repository->objects,
				       add_possible_reference_from_superproject, &sas);
	अन्यथा यदि (!म_भेद(sm_alternate, "no"))
		; /* do nothing */
	अन्यथा
		die(_("Value '%s' for submodule.alternateLocation is not recognized"), sm_alternate);

	मुक्त(sm_alternate);
	मुक्त(error_strategy);
}

स्थिर वर्ण *clone_submodule_sm_gitdir(अचर वर्ण *name)
{
	संरचना strbuf sb = STRBUF_INIT;
	वर्ण *sm_gitdir;

	submodule_name_to_gitdir(&sb, the_repository, name);
	sm_gitdir = absolute_pathdup(sb.buf);
	strbuf_release(&sb);

	फर्काऊ sm_gitdir;
}

स्थिर पूर्णाङ्क dir_contains_only_dotgit(अचर वर्ण *path)
{
	सूची *dir = सूची_खोलो(path);
	संरचना dirent *e;
	पूर्णाङ्क ret = 1;

	यदि (!dir)
		फर्काऊ 0;

	e = readdir_skip_dot_and_dotdot(dir);
	यदि (!e)
		ret = 0;
	अन्यथा यदि (म_भेद(DEFAULT_GIT_DIR_ENVIRONMENT, e->d_name) ||
		 (e = readdir_skip_dot_and_dotdot(dir))) {
		error("unexpected item '%s' in '%s'", e->d_name, path);
		ret = 0;
	}

	बंद_सूची(dir);
	फर्काऊ ret;
}

स्थिर पूर्णाङ्क clone_submodule(अचर संरचना module_clone_data *clone_data,
			   संरचना string_list *reference)
{
	वर्ण *p;
	वर्ण *sm_gitdir = clone_submodule_sm_gitdir(clone_data->name);
	वर्ण *sm_alternate = रिक्त, *error_strategy = रिक्त;
	संरचना stat st;
	संरचना child_process cp = CHILD_PROCESS_INIT;
	अचर वर्ण *clone_data_path = clone_data->path;
	वर्ण *to_free = रिक्त;

	यदि (validate_submodule_path(clone_data_path) < 0)
		die(रिक्त);

	यदि (!is_absolute_path(clone_data->path))
		clone_data_path = to_free = xstrfmt("%s/%s", repo_get_work_रूख(the_repository),
						    clone_data->path);

	यदि (!file_exists(sm_gitdir)) {
		यदि (clone_data->require_init && !stat(clone_data_path, &st) &&
		    !is_empty_dir(clone_data_path))
			die(_("directory not empty: '%s'"), clone_data_path);

		यदि (safe_create_leading_directories_const(the_repository, sm_gitdir) < 0)
			die(_("could not create directory '%s'"), sm_gitdir);

		prepare_possible_alternates(clone_data->name, reference);

		strvec_थप(&cp.args, "clone");
		strvec_थप(&cp.args, "--no-checkout");
		यदि (clone_data->quiet)
			strvec_थप(&cp.args, "--quiet");
		यदि (clone_data->progress)
			strvec_थप(&cp.args, "--progress");
		यदि (clone_data->depth > 0)
			strvec_pushf(&cp.args, "--depth=%d", clone_data->depth);
		यदि (reference->nr) {
			संरचना string_list_item *item;

			for_each_string_list_item(item, reference)
				strvec_pushl(&cp.args, "--reference",
					     item->string, रिक्त);
		}
		यदि (clone_data->ref_storage_format != REF_STORAGE_FORMAT_UNKNOWN)
			strvec_pushf(&cp.args, "--ref-storage-format=%s",
				     ref_storage_format_to_name(clone_data->ref_storage_format));
		यदि (clone_data->dissociate)
			strvec_थप(&cp.args, "--dissociate");
		यदि (sm_gitdir && *sm_gitdir)
			strvec_pushl(&cp.args, "--separate-git-dir", sm_gitdir, रिक्त);
		यदि (clone_data->filter_options && clone_data->filter_options->choice)
			strvec_pushf(&cp.args, "--filter=%s",
				     expand_list_objects_filter_spec(
					     clone_data->filter_options));
		यदि (clone_data->single_branch >= 0)
			strvec_थप(&cp.args, clone_data->single_branch ?
				    "--single-branch" :
				    "--no-single-branch");

		strvec_थप(&cp.args, "--");
		strvec_थप(&cp.args, clone_data->url);
		strvec_थप(&cp.args, clone_data_path);

		cp.git_cmd = 1;
		prepare_submodule_repo_env(&cp.env);
		cp.no_stdin = 1;

		यदि(run_command(&cp))
			die(_("clone of '%s' into submodule path '%s' failed"),
			    clone_data->url, clone_data_path);

		यदि (clone_data->require_init && !stat(clone_data_path, &st) &&
		    !dir_contains_only_dotgit(clone_data_path)) {
			वर्ण *dot_git = xstrfmt("%s/.git", clone_data_path);
			unlink(dot_git);
			मुक्त(dot_git);
			die(_("directory not empty: '%s'"), clone_data_path);
		}
	} अन्यथा {
		वर्ण *path;

		यदि (clone_data->require_init && !stat(clone_data_path, &st) &&
		    !is_empty_dir(clone_data_path))
			die(_("directory not empty: '%s'"), clone_data_path);
		यदि (safe_create_leading_directories_const(the_repository, clone_data_path) < 0)
			die(_("could not create directory '%s'"), clone_data_path);
		path = xstrfmt("%s/index", sm_gitdir);
		unlink_or_warn(path);
		मुक्त(path);
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
	यदि (validate_submodule_git_dir(sm_gitdir, clone_data->name) < 0) {
		वर्ण *head = xstrfmt("%s/HEAD", sm_gitdir);
		unlink(head);
		मुक्त(head);
		die(_("refusing to create/use '%s' in another submodule's git dir. "
		      "Enabling extensions.submodulePathConfig should fix this."),
		    sm_gitdir);
	}

	connect_work_रूख_and_git_dir(clone_data_path, sm_gitdir, 0);

	p = repo_submodule_path(the_repository, clone_data_path, "config");
	यदि (!p)
		die(_("could not get submodule directory for '%s'"), clone_data_path);

	/* setup alternateLocation and alternateErrorStrategy in the cloned submodule if needed */
	repo_config_get_string(the_repository, "submodule.alternateLocation", &sm_alternate);
	यदि (sm_alternate)
		repo_config_set_in_file(the_repository, p, "submodule.alternateLocation",
					sm_alternate);
	repo_config_get_string(the_repository, "submodule.alternateErrorStrategy", &error_strategy);
	यदि (error_strategy)
		repo_config_set_in_file(the_repository, p, "submodule.alternateErrorStrategy",
					error_strategy);

	मुक्त(sm_alternate);
	मुक्त(error_strategy);

	मुक्त(sm_gitdir);
	मुक्त(p);
	मुक्त(to_free);
	फर्काऊ 0;
}

स्थिर पूर्णाङ्क module_clone(पूर्णाङ्क argc, अचर वर्ण **argv, अचर वर्ण *prefix,
			संरचना repository *repo UNUSED)
{
	पूर्णाङ्क dissociate = 0, quiet = 0, progress = 0, require_init = 0;
	संरचना module_clone_data clone_data = MODULE_CLONE_DATA_INIT;
	संरचना string_list reference = STRING_LIST_INIT_NODUP;
	संरचना list_objects_filter_options filter_options =
		LIST_OBJECTS_FILTER_INIT;
	अचर वर्ण *ref_storage_format = रिक्त;

	संरचना option module_clone_options[] = {
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
	अचर वर्ण *अचर git_submodule_helper_usage[] = {
		N_("git submodule--helper clone [--prefix=<path>] [--quiet] "
		   "[--reference <repository>] [--name <name>] [--depth <depth>] "
		   "[--single-branch] [--filter <filter-spec>] "
		   "--url <url> --path <path>"),
		रिक्त
	};

	argc = parse_options(argc, argv, prefix, module_clone_options,
			     git_submodule_helper_usage, 0);

	यदि (ref_storage_format) {
		clone_data.ref_storage_format = ref_storage_format_by_name(ref_storage_format);
		यदि (clone_data.ref_storage_format == REF_STORAGE_FORMAT_UNKNOWN)
			die(_("unknown ref storage format '%s'"), ref_storage_format);
	}
	clone_data.dissociate = !!dissociate;
	clone_data.quiet = !!quiet;
	clone_data.progress = !!progress;
	clone_data.require_init = !!require_init;
	clone_data.filter_options = &filter_options;

	यदि (argc || !clone_data.url || !clone_data.path || !*(clone_data.path))
		usage_with_options(git_submodule_helper_usage,
				   module_clone_options);

	clone_submodule(&clone_data, &reference);
	list_objects_filter_release(&filter_options);
	string_list_clear(&reference, 1);
	फर्काऊ 0;
}

स्थिर पूर्णाङ्क determine_submodule_update_strategy(संरचना repository *r,
					       पूर्णाङ्क just_cloned,
					       अचर वर्ण *path,
					       गणना submodule_update_type update,
					       संरचना submodule_update_strategy *out)
{
	अचर संरचना submodule *sub = submodule_from_path(r, null_oid(the_hash_algo), path);
	वर्ण *साँचो;
	अचर वर्ण *val;
	पूर्णाङ्क ret;

	/*
	 * NEEDSWORK: audit and ensure that update_submodule() has right
	 * to assume that submodule_from_path() above will always succeed.
	 */
	यदि (!sub)
		BUG("update_submodule assumes a submodule exists at path (%s)",
		    path);
	साँचो = xstrfmt("submodule.%s.update", sub->name);

	यदि (update) {
		out->type = update;
	} अन्यथा यदि (!repo_config_get_string_tmp(r, साँचो, &val)) {
		यदि (parse_submodule_update_strategy(val, out) < 0) {
			ret = die_message(_("Invalid update mode '%s' configured for submodule path '%s'"),
					  val, path);
			गच्छ cleanup;
		}
	} अन्यथा यदि (sub->update_strategy.type != SM_UPDATE_UNSPECIFIED) {
		यदि (sub->update_strategy.type == SM_UPDATE_COMMAND)
			BUG("how did we read update = !command from .gitmodules?");
		out->type = sub->update_strategy.type;
		out->command = sub->update_strategy.command;
	} अन्यथा
		out->type = SM_UPDATE_CHECKOUT;

	यदि (just_cloned &&
	    (out->type == SM_UPDATE_MERGE ||
	     out->type == SM_UPDATE_REBASE ||
	     out->type == SM_UPDATE_NONE))
		out->type = SM_UPDATE_CHECKOUT;

	ret = 0;
cleanup:
	मुक्त(साँचो);
	फर्काऊ ret;
}

संरचना update_clone_data {
	अचर संरचना submodule *sub;
	संरचना object_id oid;
	अचिन्हित just_cloned;
};

संरचना submodule_update_clone {
	/* index into 'update_data.list', the list of submodules to look into for cloning */
	पूर्णाङ्क current;

	/* configuration parameters which are passed on to the children */
	अचर संरचना update_data *update_data;

	/* to be consumed by update_submodule() */
	संरचना update_clone_data *update_clone;
	पूर्णाङ्क update_clone_nr; पूर्णाङ्क update_clone_alloc;

	/* If we want to stop as fast as possible and return an error */
	अचिन्हित quickstop : 1;

	/* failed clones to be retried again */
	अचर संरचना cache_entry **failed_clones;
	पूर्णाङ्क failed_clones_nr, failed_clones_alloc;
};
#define SUBMODULE_UPDATE_CLONE_INIT { 0 }

स्थिर शून्य submodule_update_clone_release(संरचना submodule_update_clone *suc)
{
	मुक्त(suc->update_clone);
	मुक्त(suc->failed_clones);
}

संरचना update_data {
	अचर वर्ण *prefix;
	अचर वर्ण *super_prefix;
	वर्ण *displaypath;
	गणना submodule_update_type update_default;
	संरचना object_id suboid;
	संरचना string_list references;
	संरचना submodule_update_strategy update_strategy;
	संरचना list_objects_filter_options *filter_options;
	संरचना module_list list;
	गणना ref_storage_format ref_storage_format;
	पूर्णाङ्क depth;
	पूर्णाङ्क max_jobs;
	पूर्णाङ्क single_branch;
	पूर्णाङ्क recommend_shallow;
	अचिन्हित पूर्णाङ्क require_init;
	अचिन्हित पूर्णाङ्क force;
	अचिन्हित पूर्णाङ्क quiet;
	अचिन्हित पूर्णाङ्क nofetch;
	अचिन्हित पूर्णाङ्क remote;
	अचिन्हित पूर्णाङ्क progress;
	अचिन्हित पूर्णाङ्क dissociate;
	अचिन्हित पूर्णाङ्क init;
	अचिन्हित पूर्णाङ्क warn_if_uninitialized;
	अचिन्हित पूर्णाङ्क recursive;

	/* copied over from update_clone_data */
	संरचना object_id oid;
	अचिन्हित पूर्णाङ्क just_cloned;
	अचर वर्ण *sm_path;
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

स्थिर शून्य update_data_release(संरचना update_data *ud)
{
	मुक्त(ud->displaypath);
	submodule_update_strategy_release(&ud->update_strategy);
	module_list_release(&ud->list);
}

स्थिर शून्य next_submodule_warn_missing(संरचना submodule_update_clone *suc,
		संरचना strbuf *out, अचर वर्ण *displaypath)
{
	/*
	 * Only mention uninitialized submodules when their
	 * paths have been specified.
	 */
	यदि (suc->update_data->warn_if_uninitialized) {
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
स्थिर पूर्णाङ्क prepare_to_clone_next_submodule(अचर संरचना cache_entry *ce,
					   संरचना child_process *child,
					   संरचना submodule_update_clone *suc,
					   संरचना strbuf *out)
{
	अचर संरचना submodule *sub = रिक्त;
	अचर वर्ण *url = रिक्त;
	अचर वर्ण *update_string;
	गणना submodule_update_type update_type;
	वर्ण *साँचो;
	अचर संरचना update_data *ud = suc->update_data;
	वर्ण *displaypath = get_submodule_displaypath(ce->name, ud->prefix,
						      ud->super_prefix);
	संरचना strbuf sb = STRBUF_INIT;
	पूर्णाङ्क needs_cloning = 0;
	पूर्णाङ्क need_free_url = 0;

	यदि (ce_stage(ce)) {
		strbuf_addf(out, _("Skipping unmerged submodule %s"), displaypath);
		strbuf_addch(out, '\n');
		गच्छ cleanup;
	}

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), ce->name);

	यदि (!sub) {
		next_submodule_warn_missing(suc, out, displaypath);
		गच्छ cleanup;
	}

	साँचो = xstrfmt("submodule.%s.update", sub->name);
	यदि (!repo_config_get_string_tmp(the_repository, साँचो, &update_string)) {
		update_type = parse_submodule_update_type(update_string);
	} अन्यथा {
		update_type = sub->update_strategy.type;
	}
	मुक्त(साँचो);

	यदि (suc->update_data->update_strategy.type == SM_UPDATE_NONE
	    || (suc->update_data->update_strategy.type == SM_UPDATE_UNSPECIFIED
		&& update_type == SM_UPDATE_NONE)) {
		strbuf_addf(out, _("Skipping submodule '%s'"), displaypath);
		strbuf_addch(out, '\n');
		गच्छ cleanup;
	}

	/* Check if the submodule has been initialized. */
	यदि (!is_submodule_active(the_repository, ce->name)) {
		next_submodule_warn_missing(suc, out, displaypath);
		गच्छ cleanup;
	}

	strbuf_reset(&sb);
	strbuf_addf(&sb, "submodule.%s.url", sub->name);
	यदि (repo_config_get_string_tmp(the_repository, sb.buf, &url)) {
		यदि (sub->url && (starts_with_dot_slash(sub->url) ||
				 starts_with_dot_dot_slash(sub->url))) {
			url = resolve_relative_url(sub->url, रिक्त, 0);
			need_free_url = 1;
		} अन्यथा
			url = sub->url;
	}

	यदि (!url)
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

	यदि (!needs_cloning)
		गच्छ cleanup;

	child->git_cmd = 1;
	child->no_stdin = 1;
	child->stdout_to_stderr = 1;
	child->err = -1;
	strvec_थप(&child->args, "submodule--helper");
	strvec_थप(&child->args, "clone");
	यदि (suc->update_data->progress)
		strvec_थप(&child->args, "--progress");
	यदि (suc->update_data->quiet)
		strvec_थप(&child->args, "--quiet");
	यदि (suc->update_data->prefix)
		strvec_pushl(&child->args, "--prefix", suc->update_data->prefix, रिक्त);
	यदि (suc->update_data->recommend_shallow && sub->recommend_shallow == 1)
		strvec_थप(&child->args, "--depth=1");
	अन्यथा यदि (suc->update_data->depth)
		strvec_pushf(&child->args, "--depth=%d", suc->update_data->depth);
	यदि (suc->update_data->filter_options && suc->update_data->filter_options->choice)
		strvec_pushf(&child->args, "--filter=%s",
			     expand_list_objects_filter_spec(suc->update_data->filter_options));
	यदि (suc->update_data->require_init)
		strvec_थप(&child->args, "--require-init");
	यदि (suc->update_data->ref_storage_format != REF_STORAGE_FORMAT_UNKNOWN)
		strvec_pushf(&child->args, "--ref-storage-format=%s",
			     ref_storage_format_to_name(suc->update_data->ref_storage_format));
	strvec_pushl(&child->args, "--path", sub->path, रिक्त);
	strvec_pushl(&child->args, "--name", sub->name, रिक्त);
	strvec_pushl(&child->args, "--url", url, रिक्त);
	यदि (suc->update_data->references.nr) {
		संरचना string_list_item *item;

		for_each_string_list_item(item, &suc->update_data->references)
			strvec_pushl(&child->args, "--reference", item->string, रिक्त);
	}
	यदि (suc->update_data->dissociate)
		strvec_थप(&child->args, "--dissociate");
	यदि (suc->update_data->single_branch >= 0)
		strvec_थप(&child->args, suc->update_data->single_branch ?
					      "--single-branch" :
					      "--no-single-branch");

cleanup:
	मुक्त(displaypath);
	strbuf_release(&sb);
	यदि (need_free_url)
		मुक्त((शून्य*)url);

	फर्काऊ needs_cloning;
}

स्थिर पूर्णाङ्क update_clone_get_next_task(संरचना child_process *child,
				      संरचना strbuf *err,
				      शून्य *suc_cb,
				      शून्य **idx_task_cb)
{
	संरचना submodule_update_clone *suc = suc_cb;
	अचर संरचना cache_entry *ce;
	पूर्णाङ्क index;

	लागि (; suc->current < suc->update_data->list.nr; suc->current++) {
		ce = suc->update_data->list.entries[suc->current];
		यदि (prepare_to_clone_next_submodule(ce, child, suc, err)) {
			पूर्णाङ्क *p = xmalloc(परिमाण(*p));

			*p = suc->current;
			*idx_task_cb = p;
			suc->current++;
			फर्काऊ 1;
		}
	}

	/*
	 * The loop above tried cloning each submodule once, now try the
	 * stragglers again, which we can imagine as an extension of the
	 * entry list.
	 */
	index = suc->current - suc->update_data->list.nr;
	यदि (index < suc->failed_clones_nr) {
		पूर्णाङ्क *p;

		ce = suc->failed_clones[index];
		यदि (!prepare_to_clone_next_submodule(ce, child, suc, err)) {
			suc->current ++;
			strbuf_addstr(err, "BUG: submodule considered for "
					   "cloning, doesn't need cloning "
					   "any more?\n");
			फर्काऊ 0;
		}
		p = xmalloc(परिमाण(*p));
		*p = suc->current;
		*idx_task_cb = p;
		suc->current ++;
		फर्काऊ 1;
	}

	फर्काऊ 0;
}

स्थिर पूर्णाङ्क update_clone_start_failure(संरचना strbuf *err UNUSED,
				      शून्य *suc_cb,
				      शून्य *idx_task_cb UNUSED)
{
	संरचना submodule_update_clone *suc = suc_cb;

	suc->quickstop = 1;
	फर्काऊ 1;
}

स्थिर पूर्णाङ्क update_clone_task_finished(पूर्णाङ्क result,
				      संरचना strbuf *err,
				      शून्य *suc_cb,
				      शून्य *idx_task_cb)
{
	अचर संरचना cache_entry *ce;
	संरचना submodule_update_clone *suc = suc_cb;
	पूर्णाङ्क *idxP = idx_task_cb;
	पूर्णाङ्क idx = *idxP;

	मुक्त(idxP);

	यदि (!result)
		फर्काऊ 0;

	यदि (idx < suc->update_data->list.nr) {
		ce  = suc->update_data->list.entries[idx];
		strbuf_addf(err, _("Failed to clone '%s'. Retry scheduled"),
			    ce->name);
		strbuf_addch(err, '\n');
		ALLOC_GROW(suc->failed_clones,
			   suc->failed_clones_nr + 1,
			   suc->failed_clones_alloc);
		suc->failed_clones[suc->failed_clones_nr++] = ce;
		फर्काऊ 0;
	} अन्यथा {
		idx -= suc->update_data->list.nr;
		ce  = suc->failed_clones[idx];
		strbuf_addf(err, _("Failed to clone '%s' a second time, aborting"),
			    ce->name);
		strbuf_addch(err, '\n');
		suc->quickstop = 1;
		फर्काऊ 1;
	}

	फर्काऊ 0;
}

स्थिर पूर्णाङ्क git_update_clone_config(अचर वर्ण *var, अचर वर्ण *मान,
				   अचर संरचना config_context *ctx,
				   शून्य *cb)
{
	पूर्णाङ्क *max_jobs = cb;

	यदि (!म_भेद(var, "submodule.fetchjobs"))
		*max_jobs = parse_submodule_fetchjobs(var, मान, ctx->kvi);
	फर्काऊ 0;
}

स्थिर पूर्णाङ्क is_tip_reachable(अचर वर्ण *path, अचर संरचना object_id *oid)
{
	संरचना child_process cp = CHILD_PROCESS_INIT;
	संरचना strbuf rev = STRBUF_INIT;
	वर्ण *hex = oid_to_hex(oid);
	पूर्णाङ्क reachable;

	cp.git_cmd = 1;
	cp.dir = path;
	cp.no_stderr = 1;
	strvec_pushl(&cp.args, "rev-list", "-n", "1", hex, "--not", "--all", रिक्त);

	prepare_submodule_repo_env(&cp.env);

	यदि (capture_command(&cp, &rev, GIT_MAX_HEXSZ + 1) || rev.len)
		reachable = 0;
	अन्यथा
		reachable = 1;

	strbuf_release(&rev);
	फर्काऊ reachable;
}

स्थिर पूर्णाङ्क fetch_in_submodule(अचर वर्ण *module_path, पूर्णाङ्क depth, पूर्णाङ्क quiet,
			      अचर संरचना object_id *oid)
{
	संरचना child_process cp = CHILD_PROCESS_INIT;

	prepare_submodule_repo_env(&cp.env);
	cp.git_cmd = 1;
	cp.dir = module_path;

	strvec_थप(&cp.args, "fetch");
	यदि (quiet)
		strvec_थप(&cp.args, "--quiet");
	यदि (depth)
		strvec_pushf(&cp.args, "--depth=%d", depth);
	यदि (oid) {
		वर्ण *hex = oid_to_hex(oid);
		वर्ण *remote;
		पूर्णाङ्क code;

		code = get_default_remote_submodule(module_path, &remote);
		यदि (code) {
			child_process_clear(&cp);
			फर्काऊ code;
		}

		strvec_pushl(&cp.args, remote, hex, रिक्त);
		मुक्त(remote);
	}

	फर्काऊ run_command(&cp);
}

स्थिर पूर्णाङ्क run_update_command(अचर संरचना update_data *ud, पूर्णाङ्क subforce)
{
	संरचना child_process cp = CHILD_PROCESS_INIT;
	वर्ण *oid = oid_to_hex(&ud->oid);
	पूर्णाङ्क ret;

	विकल्प (ud->update_strategy.type) {
	पक्ष SM_UPDATE_CHECKOUT:
		cp.git_cmd = 1;
		strvec_pushl(&cp.args, "checkout", "-q", रिक्त);
		यदि (subforce)
			strvec_थप(&cp.args, "-f");
		रोक;
	पक्ष SM_UPDATE_REBASE:
		cp.git_cmd = 1;
		strvec_थप(&cp.args, "rebase");
		यदि (ud->quiet)
			strvec_थप(&cp.args, "--quiet");
		रोक;
	पक्ष SM_UPDATE_MERGE:
		cp.git_cmd = 1;
		strvec_थप(&cp.args, "merge");
		यदि (ud->quiet)
			strvec_थप(&cp.args, "--quiet");
		रोक;
	पक्ष SM_UPDATE_COMMAND:
		cp.use_shell = 1;
		strvec_थप(&cp.args, ud->update_strategy.command);
		रोक;
	स्वभाव:
		BUG("unexpected update strategy type: %d",
		    ud->update_strategy.type);
	}
	strvec_थप(&cp.args, oid);

	cp.dir = ud->sm_path;
	prepare_submodule_repo_env(&cp.env);
	यदि ((ret = run_command(&cp))) {
		विकल्प (ud->update_strategy.type) {
		पक्ष SM_UPDATE_CHECKOUT:
			die_message(_("Unable to checkout '%s' in submodule path '%s'"),
				    oid, ud->displaypath);
			/* No "ret" assignment, use "git checkout"'s */
			रोक;
		पक्ष SM_UPDATE_REBASE:
			ret = die_message(_("Unable to rebase '%s' in submodule path '%s'"),
					  oid, ud->displaypath);
			रोक;
		पक्ष SM_UPDATE_MERGE:
			ret = die_message(_("Unable to merge '%s' in submodule path '%s'"),
					  oid, ud->displaypath);
			रोक;
		पक्ष SM_UPDATE_COMMAND:
			ret = die_message(_("Execution of '%s %s' failed in submodule path '%s'"),
					  ud->update_strategy.command, oid, ud->displaypath);
			रोक;
		स्वभाव:
			BUG("unexpected update strategy type: %d",
			    ud->update_strategy.type);
		}

		फर्काऊ ret;
	}

	यदि (ud->quiet)
		फर्काऊ 0;

	विकल्प (ud->update_strategy.type) {
	पक्ष SM_UPDATE_CHECKOUT:
		म_लिखो(_("Submodule path '%s': checked out '%s'\n"),
		       ud->displaypath, oid);
		रोक;
	पक्ष SM_UPDATE_REBASE:
		म_लिखो(_("Submodule path '%s': rebased into '%s'\n"),
		       ud->displaypath, oid);
		रोक;
	पक्ष SM_UPDATE_MERGE:
		म_लिखो(_("Submodule path '%s': merged in '%s'\n"),
		       ud->displaypath, oid);
		रोक;
	पक्ष SM_UPDATE_COMMAND:
		म_लिखो(_("Submodule path '%s': '%s %s'\n"),
		       ud->displaypath, ud->update_strategy.command, oid);
		रोक;
	स्वभाव:
		BUG("unexpected update strategy type: %d",
		    ud->update_strategy.type);
	}

	फर्काऊ 0;
}

स्थिर पूर्णाङ्क run_update_procedure(अचर संरचना update_data *ud)
{
	पूर्णाङ्क subforce = is_null_oid(&ud->suboid) || ud->force;

	यदि (!ud->nofetch) {
		/*
		 * Run fetch only if `oid` isn't present or it
		 * is not reachable from a ref.
		 */
		यदि (!is_tip_reachable(ud->sm_path, &ud->oid) &&
		    fetch_in_submodule(ud->sm_path, ud->depth, ud->quiet, रिक्त) &&
		    !ud->quiet)
			fprintf_ln(मानक_त्रुटि,
				   _("Unable to fetch in submodule path '%s'; "
				     "trying to directly fetch %s:"),
				   ud->displaypath, oid_to_hex(&ud->oid));
		/*
		 * Now we tried the usual fetch, but `oid` may
		 * not be reachable from any of the refs.
		 */
		यदि (!is_tip_reachable(ud->sm_path, &ud->oid) &&
		    fetch_in_submodule(ud->sm_path, ud->depth, ud->quiet, &ud->oid))
			फर्काऊ die_message(_("Fetched in submodule path '%s', but it did not "
					     "contain %s. Direct fetching of that commit failed."),
					   ud->displaypath, oid_to_hex(&ud->oid));
	}

	फर्काऊ run_update_command(ud, subforce);
}

स्थिर पूर्णाङ्क remote_submodule_branch(अचर वर्ण *path, अचर वर्ण **branch)
{
	अचर संरचना submodule *sub;
	वर्ण *साँचो;
	*branch = रिक्त;

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);
	यदि (!sub)
		फर्काऊ die_message(_("could not initialize submodule at path '%s'"),
				   path);

	साँचो = xstrfmt("submodule.%s.branch", sub->name);
	यदि (repo_config_get_string_tmp(the_repository, साँचो, branch))
		*branch = sub->branch;
	मुक्त(साँचो);

	यदि (!*branch) {
		*branch = "HEAD";
		फर्काऊ 0;
	}

	यदि (!म_भेद(*branch, ".")) {
		अचर वर्ण *refname = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
							      "HEAD", 0, रिक्त,
							      रिक्त);

		यदि (!refname)
			फर्काऊ die_message(_("No such ref: %s"), "HEAD");

		/* detached HEAD */
		यदि (!म_भेद(refname, "HEAD"))
			फर्काऊ die_message(_("Submodule (%s) branch configured to inherit "
					     "branch from superproject, but the superproject "
					     "is not on any branch"), sub->name);

		यदि (!skip_prefix(refname, "refs/heads/", &refname))
			फर्काऊ die_message(_("Expecting a full ref name, got %s"),
					   refname);

		*branch = refname;
		फर्काऊ 0;
	}

	/* Our "branch" is coming from repo_config_get_string_tmp() */
	फर्काऊ 0;
}

स्थिर पूर्णाङ्क ensure_core_worktree(अचर वर्ण *path)
{
	अचर वर्ण *cw;
	संरचना repository subrepo;

	यदि (repo_submodule_init(&subrepo, the_repository, path, null_oid(the_hash_algo)))
		फर्काऊ die_message(_("could not get a repository handle for submodule '%s'"),
				   path);

	यदि (!repo_config_get_string_tmp(&subrepo, "core.worktree", &cw)) {
		वर्ण *cfg_file, *abs_path;
		अचर वर्ण *rel_path;
		संरचना strbuf sb = STRBUF_INIT;

		cfg_file = repo_git_path(&subrepo, "config");

		abs_path = absolute_pathdup(path);
		rel_path = relative_path(abs_path, subrepo.gitdir, &sb);

		repo_config_set_in_file(the_repository, cfg_file, "core.worktree", rel_path);

		मुक्त(cfg_file);
		मुक्त(abs_path);
		strbuf_release(&sb);
	}

	repo_clear(&subrepo);
	फर्काऊ 0;
}

स्थिर अचर वर्ण *submodule_update_type_to_label(गणना submodule_update_type type)
{
	विकल्प (type) {
	पक्ष SM_UPDATE_CHECKOUT:
		फर्काऊ "checkout";
	पक्ष SM_UPDATE_MERGE:
		फर्काऊ "merge";
	पक्ष SM_UPDATE_REBASE:
		फर्काऊ "rebase";
	पक्ष SM_UPDATE_UNSPECIFIED:
	पक्ष SM_UPDATE_NONE:
	पक्ष SM_UPDATE_COMMAND:
		रोक;
	}
	BUG("unreachable with type %d", type);
}

स्थिर शून्य update_data_to_args(अचर संरचना update_data *update_data,
				संरचना strvec *args)
{
	गणना submodule_update_type update_type = update_data->update_default;

	strvec_pushl(args, "submodule--helper", "update", "--recursive", रिक्त);
	यदि (update_data->displaypath)
		strvec_pushf(args, "--super-prefix=%s/",
			     update_data->displaypath);
	strvec_pushf(args, "--jobs=%d", update_data->max_jobs);
	यदि (update_data->quiet)
		strvec_थप(args, "--quiet");
	यदि (update_data->force)
		strvec_थप(args, "--force");
	यदि (update_data->init)
		strvec_थप(args, "--init");
	यदि (update_data->remote)
		strvec_थप(args, "--remote");
	यदि (update_data->nofetch)
		strvec_थप(args, "--no-fetch");
	यदि (update_data->dissociate)
		strvec_थप(args, "--dissociate");
	यदि (update_data->progress)
		strvec_थप(args, "--progress");
	यदि (update_data->require_init)
		strvec_थप(args, "--require-init");
	यदि (update_data->depth)
		strvec_pushf(args, "--depth=%d", update_data->depth);
	यदि (update_type != SM_UPDATE_UNSPECIFIED)
		strvec_pushf(args, "--%s",
			     submodule_update_type_to_label(update_type));

	यदि (update_data->references.nr) {
		संरचना string_list_item *item;

		for_each_string_list_item(item, &update_data->references)
			strvec_pushl(args, "--reference", item->string, रिक्त);
	}
	यदि (update_data->ref_storage_format != REF_STORAGE_FORMAT_UNKNOWN)
		strvec_pushf(args, "--ref-storage-format=%s",
			     ref_storage_format_to_name(update_data->ref_storage_format));
	यदि (update_data->filter_options && update_data->filter_options->choice)
		strvec_pushf(args, "--filter=%s",
				expand_list_objects_filter_spec(
					update_data->filter_options));
	यदि (update_data->recommend_shallow == 0)
		strvec_थप(args, "--no-recommend-shallow");
	अन्यथा यदि (update_data->recommend_shallow == 1)
		strvec_थप(args, "--recommend-shallow");
	यदि (update_data->single_branch >= 0)
		strvec_थप(args, update_data->single_branch ?
				    "--single-branch" :
				    "--no-single-branch");
}

स्थिर पूर्णाङ्क update_submodule(संरचना update_data *update_data)
{
	पूर्णाङ्क ret;

	यदि (validate_submodule_path(update_data->sm_path) < 0)
		फर्काऊ -1;

	ret = determine_submodule_update_strategy(the_repository,
						  update_data->just_cloned,
						  update_data->sm_path,
						  update_data->update_default,
						  &update_data->update_strategy);
	यदि (ret)
		फर्काऊ ret;

	यदि (update_data->just_cloned)
		oidcpy(&update_data->suboid, null_oid(the_hash_algo));
	अन्यथा यदि (repo_resolve_gitlink_ref(the_repository, update_data->sm_path,
					  "HEAD", &update_data->suboid))
		फर्काऊ die_message(_("Unable to find current revision in submodule path '%s'"),
				   update_data->displaypath);

	यदि (update_data->remote) {
		वर्ण *remote_name;
		अचर वर्ण *branch;
		वर्ण *remote_ref;
		पूर्णाङ्क code;

		code = get_default_remote_submodule(update_data->sm_path, &remote_name);
		यदि (code)
			फर्काऊ code;
		code = remote_submodule_branch(update_data->sm_path, &branch);
		यदि (code) {
			मुक्त(remote_name);
			फर्काऊ code;
		}
		remote_ref = xstrfmt("refs/remotes/%s/%s", remote_name, branch);

		मुक्त(remote_name);

		यदि (!update_data->nofetch) {
			यदि (fetch_in_submodule(update_data->sm_path, update_data->depth,
					      0, रिक्त)) {
				मुक्त(remote_ref);
				फर्काऊ die_message(_("Unable to fetch in submodule path '%s'"),
						   update_data->sm_path);
			}
		}

		यदि (repo_resolve_gitlink_ref(the_repository, update_data->sm_path,
					     remote_ref, &update_data->oid)) {
			ret = die_message(_("Unable to find %s revision in submodule path '%s'"),
					  remote_ref, update_data->sm_path);
			मुक्त(remote_ref);
			फर्काऊ ret;
		}

		मुक्त(remote_ref);
	}

	यदि (!oideq(&update_data->oid, &update_data->suboid) || update_data->force) {
		ret = run_update_procedure(update_data);
		यदि (ret)
			फर्काऊ ret;
	}

	यदि (update_data->recursive) {
		संरचना child_process cp = CHILD_PROCESS_INIT;
		संरचना update_data next = *update_data;

		next.prefix = रिक्त;
		oidcpy(&next.oid, null_oid(the_hash_algo));
		oidcpy(&next.suboid, null_oid(the_hash_algo));

		cp.dir = update_data->sm_path;
		cp.git_cmd = 1;
		prepare_submodule_repo_env(&cp.env);
		update_data_to_args(&next, &cp.args);

		ret = run_command(&cp);
		यदि (ret)
			die_message(_("Failed to recurse into submodule path '%s'"),
				    update_data->displaypath);
		फर्काऊ ret;
	}

	फर्काऊ 0;
}

स्थिर पूर्णाङ्क update_submodules(संरचना update_data *update_data)
{
	पूर्णाङ्क i, ret = 0;
	संरचना submodule_update_clone suc = SUBMODULE_UPDATE_CLONE_INIT;
	अचर संरचना run_process_parallel_opts opts = {
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
	यदि (suc.quickstop) {
		ret = 1;
		गच्छ cleanup;
	}

	लागि (i = 0; i < suc.update_clone_nr; i++) {
		संरचना update_clone_data ucd = suc.update_clone[i];
		पूर्णाङ्क code = 128;

		oidcpy(&update_data->oid, &ucd.oid);
		update_data->just_cloned = ucd.just_cloned;
		update_data->sm_path = ucd.sub->path;

		/*
		 * Verify that the submodule path does not contain any
		 * symlinks; if it does, it might have been tampered with.
		 * TODO: allow exempting it via
		 * `safe.submodule.path` or something
		 */
		यदि (validate_submodule_path(update_data->sm_path) < 0)
			गच्छ fail;

		code = ensure_core_worktree(update_data->sm_path);
		यदि (code)
			गच्छ fail;

		update_data->displaypath = get_submodule_displaypath(
			update_data->sm_path, update_data->prefix,
			update_data->super_prefix);
		code = update_submodule(update_data);
		FREE_AND_NULL(update_data->displaypath);
fail:
		यदि (!code)
			जारी;
		ret = code;
		यदि (ret == 128)
			गच्छ cleanup;
	}

cleanup:
	submodule_update_clone_release(&suc);
	string_list_clear(&update_data->references, 0);
	फर्काऊ ret;
}

स्थिर पूर्णाङ्क module_update(पूर्णाङ्क argc, अचर वर्ण **argv, अचर वर्ण *prefix,
			 संरचना repository *repo UNUSED)
{
	संरचना pathspec pathspec = { 0 };
	संरचना pathspec pathspec2 = { 0 };
	संरचना update_data opt = UPDATE_DATA_INIT;
	संरचना list_objects_filter_options filter_options =
		LIST_OBJECTS_FILTER_INIT;
	अचर वर्ण *ref_storage_format = रिक्त;
	पूर्णाङ्क ret;
	संरचना option module_update_options[] = {
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
	अचर वर्ण *अचर git_submodule_helper_usage[] = {
		N_("git submodule [--quiet] update"
		" [--init [--filter=<filter-spec>]] [--remote]"
		" [-N|--no-fetch] [-f|--force]"
		" [--checkout|--merge|--rebase]"
		" [--[no-]recommend-shallow] [--reference <repository>]"
		" [--recursive] [--[no-]single-branch] [--] [<path>...]"),
		रिक्त
	};

	update_clone_config_from_gitmodules(the_repository, &opt.max_jobs);
	repo_config(the_repository, git_update_clone_config, &opt.max_jobs);

	argc = parse_options(argc, argv, prefix, module_update_options,
			     git_submodule_helper_usage, 0);

	यदि (opt.require_init)
		opt.init = 1;

	यदि (filter_options.choice && !opt.init) {
		usage_with_options(git_submodule_helper_usage,
				   module_update_options);
	}

	यदि (ref_storage_format) {
		opt.ref_storage_format = ref_storage_format_by_name(ref_storage_format);
		यदि (opt.ref_storage_format == REF_STORAGE_FORMAT_UNKNOWN)
			die(_("unknown ref storage format '%s'"), ref_storage_format);
	}

	opt.filter_options = &filter_options;
	opt.prefix = prefix;

	यदि (opt.update_default)
		opt.update_strategy.type = opt.update_default;

	यदि (module_list_compute(argv, prefix, &pathspec, &opt.list) < 0) {
		ret = 1;
		गच्छ cleanup;
	}

	यदि (pathspec.nr)
		opt.warn_if_uninitialized = 1;

	यदि (opt.init) {
		संरचना module_list list = MODULE_LIST_INIT;
		संरचना init_cb info = INIT_CB_INIT;

		यदि (module_list_compute(argv, opt.prefix,
					&pathspec2, &list) < 0) {
			module_list_release(&list);
			ret = 1;
			गच्छ cleanup;
		}

		/*
		 * If there are no path args and submodule.active is set then,
		 * by default, only initialize 'active' modules.
		 */
		यदि (!argc && !repo_config_get(the_repository, "submodule.active"))
			module_list_active(&list);

		info.prefix = opt.prefix;
		info.super_prefix = opt.super_prefix;
		यदि (opt.quiet)
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
	फर्काऊ ret;
}

स्थिर पूर्णाङ्क थप_check(पूर्णाङ्क argc, अचर वर्ण **argv, अचर वर्ण *prefix UNUSED,
		      संरचना repository *repo UNUSED)
{
	संरचना remote *remote;
	अचर वर्ण *superproject_head;
	वर्ण *head;
	पूर्णाङ्क detached_head = 0;
	संरचना object_id head_oid;

	यदि (argc < 3)
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
				   0, &head_oid, रिक्त);
	यदि (!head)
		die(_("Failed to resolve HEAD as a valid ref."));
	यदि (!म_भेद(head, "HEAD"))
		detached_head = 1;

	/*
	 * The remote must be configured.
	 * This is to avoid pushing to the exact same URL as the parent.
	 */
	remote = pushremote_get(argv[1]);
	यदि (!remote || remote->origin == REMOTE_UNCONFIGURED)
		die("remote '%s' not configured", argv[1]);

	/* Check the refspec */
	यदि (argc > 2) {
		पूर्णाङ्क i;
		संरचना ref *local_refs = get_local_heads();
		संरचना refspec refspec = REFSPEC_INIT_थप(the_hash_algo);

		refspec_appendn(&refspec, argv + 2, argc - 2);

		लागि (i = 0; i < refspec.nr; i++) {
			अचर संरचना refspec_item *rs = &refspec.वस्तुहरू[i];

			यदि (rs->pattern || rs->matching)
				जारी;

			/* LHS must match a single ref */
			विकल्प (गन्ती_refspec_match(rs->src, local_refs, रिक्त)) {
			पक्ष 1:
				रोक;
			पक्ष 0:
				/*
				 * If LHS matches 'HEAD' then we need to ensure
				 * that it matches the same named branch
				 * checked out in the superproject.
				 */
				यदि (!म_भेद(rs->src, "HEAD")) {
					यदि (!detached_head &&
					    !म_भेद(head, superproject_head))
						रोक;
					die("HEAD does not match the named branch in the superproject");
				}
				/* fallthrough */
			स्वभाव:
				die("src refspec '%s' must name a ref",
				    rs->src);
			}
		}

		refspec_clear(&refspec);
		free_refs(local_refs);
	}
	मुक्त(head);

	फर्काऊ 0;
}

स्थिर पूर्णाङ्क absorb_git_dirs(पूर्णाङ्क argc, अचर वर्ण **argv, अचर वर्ण *prefix,
			   संरचना repository *repo UNUSED)
{
	पूर्णाङ्क i;
	संरचना pathspec pathspec = { 0 };
	संरचना module_list list = MODULE_LIST_INIT;
	अचर वर्ण *super_prefix = रिक्त;
	संरचना option embed_gitdir_options[] = {
		OPT__SUPER_PREFIX(&super_prefix),
		OPT_END()
	};
	अचर वर्ण *अचर git_submodule_helper_usage[] = {
		N_("git submodule absorbgitdirs [<options>] [<path>...]"),
		रिक्त
	};
	पूर्णाङ्क ret = 1;

	argc = parse_options(argc, argv, prefix, embed_gitdir_options,
			     git_submodule_helper_usage, 0);

	यदि (module_list_compute(argv, prefix, &pathspec, &list) < 0)
		गच्छ cleanup;

	लागि (i = 0; i < list.nr; i++)
		absorb_git_dir_into_superproject(list.entries[i]->name,
						 super_prefix);

	ret = 0;
cleanup:
	clear_pathspec(&pathspec);
	module_list_release(&list);
	फर्काऊ ret;
}

स्थिर पूर्णाङ्क module_set_url(पूर्णाङ्क argc, अचर वर्ण **argv, अचर वर्ण *prefix,
			  संरचना repository *repo UNUSED)
{
	पूर्णाङ्क quiet = 0, ret;
	अचर वर्ण *newurl;
	अचर वर्ण *path;
	वर्ण *config_name;
	संरचना option options[] = {
		OPT__QUIET(&quiet, N_("suppress output for setting url of a submodule")),
		OPT_END()
	};
	अचर वर्ण *अचर usage[] = {
		N_("git submodule set-url [--quiet] <path> <newurl>"),
		रिक्त
	};
	अचर संरचना submodule *sub;

	argc = parse_options(argc, argv, prefix, options, usage, 0);

	यदि (argc != 2 || !(path = argv[0]) || !(newurl = argv[1]))
		usage_with_options(usage, options);

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);

	यदि (!sub)
		die(_("no submodule mapping found in .gitmodules for path '%s'"),
		    path);

	config_name = xstrfmt("submodule.%s.url", sub->name);
	ret = config_set_in_gitmodules_file_gently(the_repository, config_name, newurl);

	यदि (!ret) {
		repo_read_gitmodules(the_repository, 0);
		sync_submodule(sub->path, prefix, रिक्त, quiet ? OPT_QUIET : 0);
	}

	मुक्त(config_name);
	फर्काऊ !!ret;
}

स्थिर पूर्णाङ्क module_set_branch(पूर्णाङ्क argc, अचर वर्ण **argv, अचर वर्ण *prefix,
			     संरचना repository *repo UNUSED)
{
	पूर्णाङ्क opt_default = 0, ret;
	अचर वर्ण *opt_branch = रिक्त;
	अचर वर्ण *path;
	वर्ण *config_name;
	संरचना option options[] = {
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
	अचर वर्ण *अचर usage[] = {
		N_("git submodule set-branch [-q|--quiet] (-d|--default) <path>"),
		N_("git submodule set-branch [-q|--quiet] (-b|--branch) <branch> <path>"),
		रिक्त
	};
	अचर संरचना submodule *sub;

	argc = parse_options(argc, argv, prefix, options, usage, 0);

	यदि (!opt_branch && !opt_default)
		die(_("--branch or --default required"));

	यदि (opt_branch && opt_default)
		die(_("options '%s' and '%s' cannot be used together"), "--branch", "--default");

	यदि (argc != 1 || !(path = argv[0]))
		usage_with_options(usage, options);

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);

	यदि (!sub)
		die(_("no submodule mapping found in .gitmodules for path '%s'"),
		    path);

	config_name = xstrfmt("submodule.%s.branch", sub->name);
	ret = config_set_in_gitmodules_file_gently(the_repository, config_name, opt_branch);

	मुक्त(config_name);
	फर्काऊ !!ret;
}

स्थिर पूर्णाङ्क module_create_branch(पूर्णाङ्क argc, अचर वर्ण **argv, अचर वर्ण *prefix,
				संरचना repository *repo UNUSED)
{
	गणना branch_track track;
	पूर्णाङ्क quiet = 0, force = 0, reflog = 0, dry_run = 0;
	संरचना option options[] = {
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
	अचर वर्ण *अचर usage[] = {
		N_("git submodule--helper create-branch [-f|--force] [--create-reflog] [-q|--quiet] [-t|--track] [-n|--dry-run] <name> <start-oid> <start-name>"),
		रिक्त
	};
	संरचना repo_config_मानहरू *cfg = repo_config_मानहरू(the_repository);

	repo_config(the_repository, git_default_config, रिक्त);
	track = cfg->branch_track;
	argc = parse_options(argc, argv, prefix, options, usage, 0);

	यदि (argc != 3)
		usage_with_options(usage, options);

	यदि (!quiet && !dry_run)
		printf_ln(_("creating branch '%s'"), argv[0]);

	create_branches_recursively(the_repository, argv[0], argv[1], argv[2],
				    force, reflog, quiet, track, dry_run);
	फर्काऊ 0;
}

संरचना add_data {
	अचर वर्ण *prefix;
	अचर वर्ण *branch;
	अचर वर्ण *reference_path;
	वर्ण *sm_path;
	अचर वर्ण *sm_name;
	अचर वर्ण *repo;
	अचर वर्ण *realrepo;
	गणना ref_storage_format ref_storage_format;
	पूर्णाङ्क depth;
	अचिन्हित पूर्णाङ्क force: 1;
	अचिन्हित पूर्णाङ्क quiet: 1;
	अचिन्हित पूर्णाङ्क progress: 1;
	अचिन्हित पूर्णाङ्क dissociate: 1;
};
#define ADD_DATA_INIT { \
	.depth = -1, \
	.ref_storage_format = REF_STORAGE_FORMAT_UNKNOWN, \
}

स्थिर शून्य append_fetch_remotes(संरचना strbuf *सन्देश, अचर वर्ण *git_dir_path)
{
	संरचना child_process cp_remote = CHILD_PROCESS_INIT;
	संरचना strbuf sb_remote_out = STRBUF_INIT;

	cp_remote.git_cmd = 1;
	strvec_pushf(&cp_remote.env,
		     "GIT_DIR=%s", git_dir_path);
	strvec_थप(&cp_remote.env, "GIT_WORK_TREE=.");
	strvec_pushl(&cp_remote.args, "remote", "-v", रिक्त);
	यदि (!capture_command(&cp_remote, &sb_remote_out, 0)) {
		वर्ण *next_पङ्क्ति;
		वर्ण *पङ्क्ति = sb_remote_out.buf;

		जबसम्म ((next_पङ्क्ति = म_अक्षर(पङ्क्ति, '\n')) != रिक्त) {
			माप_प्रकार len = next_पङ्क्ति - पङ्क्ति;

			यदि (strip_suffix_mem(पङ्क्ति, &len, " (fetch)"))
				strbuf_addf(सन्देश, "  %.*s\n", (पूर्णाङ्क)len, पङ्क्ति);
			पङ्क्ति = next_पङ्क्ति + 1;
		}
	}

	strbuf_release(&sb_remote_out);
}

स्थिर पूर्णाङ्क add_submodule(अचर संरचना add_data *add_data)
{
	संरचना module_clone_data clone_data = MODULE_CLONE_DATA_INIT;
	संरचना string_list reference = STRING_LIST_INIT_NODUP;
	पूर्णाङ्क ret = -1;

	/* perhaps the path already exists and is already a git repo, else clone it */
	यदि (is_directory(add_data->sm_path)) {
		वर्ण *submod_gitdir_path;
		संरचना strbuf sm_path = STRBUF_INIT;
		strbuf_addstr(&sm_path, add_data->sm_path);
		submod_gitdir_path = xstrfmt("%s/.git", add_data->sm_path);
		यदि (is_nonbare_repository_dir(&sm_path))
			म_लिखो(_("Adding existing repo at '%s' to the index\n"),
			       add_data->sm_path);
		अन्यथा
			die(_("'%s' already exists and is not a valid git repo"),
			    add_data->sm_path);
		strbuf_release(&sm_path);
		मुक्त(submod_gitdir_path);
	} अन्यथा {
		संरचना child_process cp = CHILD_PROCESS_INIT;
		संरचना strbuf submod_gitdir = STRBUF_INIT;

		submodule_name_to_gitdir(&submod_gitdir, the_repository, add_data->sm_name);

		यदि (is_directory(submod_gitdir.buf)) {
			यदि (!add_data->force) {
				संरचना strbuf सन्देश = STRBUF_INIT;
				वर्ण *die_सन्देश;

				strbuf_addf(&सन्देश, _("A git directory for '%s' is found "
						    "locally with remote(s):\n"),
					    add_data->sm_name);

				append_fetch_remotes(&सन्देश, submod_gitdir.buf);
				strbuf_release(&submod_gitdir);

				strbuf_addf(&सन्देश, _("If you want to reuse this local git "
						    "directory instead of cloning again from\n"
						    "  %s\n"
						    "use the '--force' option. If the local git "
						    "directory is not the correct repo\n"
						    "or you are unsure what this means choose "
						    "another name with the '--name' option."),
					    add_data->realrepo);

				die_सन्देश = strbuf_detach(&सन्देश, रिक्त);
				die("%s", die_सन्देश);
			} अन्यथा {
				म_लिखो(_("Reactivating local git directory for "
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
		यदि (add_data->reference_path) {
			वर्ण *p = xstrdup(add_data->reference_path);

			string_list_append(&reference, p)->util = p;
		}
		clone_data.ref_storage_format = add_data->ref_storage_format;
		clone_data.dissociate = add_data->dissociate;
		यदि (add_data->depth >= 0)
			clone_data.depth = add_data->depth;

		यदि (clone_submodule(&clone_data, &reference))
			गच्छ cleanup;

		prepare_submodule_repo_env(&cp.env);
		cp.git_cmd = 1;
		cp.dir = add_data->sm_path;
		/*
		 * NOTE: we only get here if add_data->force is true, so
		 * passing --force to checkout is reasonable.
		 */
		strvec_pushl(&cp.args, "checkout", "-f", "-q", रिक्त);

		यदि (add_data->branch) {
			strvec_pushl(&cp.args, "-B", add_data->branch, रिक्त);
			strvec_pushf(&cp.args, "origin/%s", add_data->branch);
		}

		यदि (run_command(&cp))
			die(_("unable to checkout submodule '%s'"), add_data->sm_path);
	}
	ret = 0;

cleanup:
	string_list_clear(&reference, 1);
	फर्काऊ ret;
}

स्थिर पूर्णाङ्क config_submodule_in_gitmodules(अचर वर्ण *name, अचर वर्ण *var, अचर वर्ण *मान)
{
	वर्ण *साँचो;
	पूर्णाङ्क ret;

	यदि (!is_writing_gitmodules_ok())
		die(_("please make sure that the .gitmodules file is in the working tree"));

	साँचो = xstrfmt("submodule.%s.%s", name, var);
	ret = config_set_in_gitmodules_file_gently(the_repository, साँचो, मान);
	मुक्त(साँचो);

	फर्काऊ ret;
}

स्थिर शून्य configure_added_submodule(संरचना add_data *add_data)
{
	वर्ण *साँचो;
	संरचना child_process add_submod = CHILD_PROCESS_INIT;
	संरचना child_process add_gitmodules = CHILD_PROCESS_INIT;
	अचर संरचना string_list *मानहरू;
	पूर्णाङ्क matched = 0;

	साँचो = xstrfmt("submodule.%s.url", add_data->sm_name);
	repo_config_set_gently(the_repository, साँचो, add_data->realrepo);
	मुक्त(साँचो);

	add_submod.git_cmd = 1;
	strvec_pushl(&add_submod.args, "add",
		     "--no-warn-embedded-repo", रिक्त);
	यदि (add_data->force)
		strvec_थप(&add_submod.args, "--force");
	strvec_pushl(&add_submod.args, "--", add_data->sm_path, रिक्त);

	यदि (run_command(&add_submod))
		die(_("Failed to add submodule '%s'"), add_data->sm_path);

	यदि (config_submodule_in_gitmodules(add_data->sm_name, "path", add_data->sm_path) ||
	    config_submodule_in_gitmodules(add_data->sm_name, "url", add_data->repo))
		die(_("Failed to register submodule '%s'"), add_data->sm_path);

	यदि (add_data->branch) {
		यदि (config_submodule_in_gitmodules(add_data->sm_name,
						   "branch", add_data->branch))
			die(_("Failed to register submodule '%s'"), add_data->sm_path);
	}

	add_gitmodules.git_cmd = 1;
	strvec_pushl(&add_gitmodules.args,
		     "add", "--force", "--", ".gitmodules", रिक्त);

	यदि (run_command(&add_gitmodules))
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
	यदि (repo_config_get(the_repository, "submodule.active") || /* key absent */
	    repo_config_get_string_multi(the_repository, "submodule.active", &मानहरू)) {
		/*
		 * If the submodule being added isn't already covered by the
		 * current configured pathspec, set the submodule's active flag
		 */
		साँचो = xstrfmt("submodule.%s.active", add_data->sm_name);
		repo_config_set_gently(the_repository, साँचो, "true");
		मुक्त(साँचो);
	} अन्यथा {
		लागि (माप_प्रकार i = 0; i < मानहरू->nr; i++) {
			अचर वर्ण *pat = मानहरू->वस्तुहरू[i].string;
			यदि (!wildmatch(pat, add_data->sm_path, 0)) { /* match found */
				matched = 1;
				रोक;
			}
		}
		यदि (!matched) { /* no pattern matched -> force-enable */
			साँचो = xstrfmt("submodule.%s.active", add_data->sm_name);
			repo_config_set_gently(the_repository, साँचो, "true");
			मुक्त(साँचो);
		}
	}
}

स्थिर शून्य die_on_index_match(अचर वर्ण *path, पूर्णाङ्क force)
{
	संरचना pathspec ps;
	अचर वर्ण *args[] = { path, रिक्त };
	parse_pathspec(&ps, 0, PATHSPEC_PREFER_CWD, रिक्त, args);

	यदि (repo_read_index_preload(the_repository, रिक्त, 0) < 0)
		die(_("index file corrupt"));

	यदि (ps.nr) {
		वर्ण *ps_matched = xcalloc(ps.nr, 1);

		/* TODO: audit for interaction with sparse-index. */
		ensure_full_index(the_repository->index);

		/*
		 * Since there is only one pathspec, we just need to
		 * check ps_matched[0] to know if a cache entry matched.
		 */
		लागि (माप_प्रकार i = 0; i < the_repository->index->cache_nr; i++) {
			ce_path_match(the_repository->index, the_repository->index->cache[i], &ps,
				      ps_matched);

			यदि (ps_matched[0]) {
				यदि (!force)
					die(_("'%s' already exists in the index"),
					    path);
				यदि (!S_ISGITLINK(the_repository->index->cache[i]->ce_mode))
					die(_("'%s' already exists in the index "
					      "and is not a submodule"), path);
				रोक;
			}
		}
		मुक्त(ps_matched);
	}
	clear_pathspec(&ps);
}

स्थिर शून्य die_on_repo_without_commits(अचर वर्ण *path)
{
	संरचना strbuf sb = STRBUF_INIT;
	strbuf_addstr(&sb, path);
	यदि (is_nonbare_repository_dir(&sb)) {
		संरचना object_id oid;
		यदि (repo_resolve_gitlink_ref(the_repository, path, "HEAD", &oid) < 0)
			die(_("'%s' does not have a commit checked out"), path);
	}
	strbuf_release(&sb);
}

स्थिर पूर्णाङ्क module_add(पूर्णाङ्क argc, अचर वर्ण **argv, अचर वर्ण *prefix,
		      संरचना repository *repo UNUSED)
{
	पूर्णाङ्क force = 0, quiet = 0, progress = 0, dissociate = 0;
	संरचना add_data add_data = ADD_DATA_INIT;
	अचर वर्ण *ref_storage_format = रिक्त;
	वर्ण *to_free = रिक्त;
	अचर संरचना submodule *existing;
	संरचना strbuf buf = STRBUF_INIT;
	वर्ण *sm_name_to_free = रिक्त;
	संरचना option options[] = {
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
	अचर वर्ण *अचर usage[] = {
		N_("git submodule add [<options>] [--] <repository> [<path>]"),
		रिक्त
	};
	संरचना strbuf sb = STRBUF_INIT;
	पूर्णाङ्क ret = 1;

	argc = parse_options(argc, argv, prefix, options, usage, 0);

	यदि (!is_writing_gitmodules_ok())
		die(_("please make sure that the .gitmodules file is in the working tree"));

	यदि (prefix && *prefix &&
	    add_data.reference_path && !is_absolute_path(add_data.reference_path))
		add_data.reference_path = xstrfmt("%s%s", prefix, add_data.reference_path);

	यदि (argc == 0 || argc > 2)
		usage_with_options(usage, options);

	यदि (ref_storage_format) {
		add_data.ref_storage_format = ref_storage_format_by_name(ref_storage_format);
		यदि (add_data.ref_storage_format == REF_STORAGE_FORMAT_UNKNOWN)
			die(_("unknown ref storage format '%s'"), ref_storage_format);
	}

	add_data.repo = argv[0];
	यदि (argc == 1)
		add_data.sm_path = git_url_basename(add_data.repo, 0, 0);
	अन्यथा
		add_data.sm_path = xstrdup(argv[1]);

	यदि (prefix && *prefix && !is_absolute_path(add_data.sm_path)) {
		वर्ण *sm_path = add_data.sm_path;

		add_data.sm_path = xstrfmt("%s%s", prefix, sm_path);
		मुक्त(sm_path);
	}

	यदि (starts_with_dot_dot_slash(add_data.repo) ||
	    starts_with_dot_slash(add_data.repo)) {
		यदि (prefix)
			die(_("Relative path can only be used from the toplevel "
			      "of the working tree"));

		/* dereference source url relative to parent's url */
		to_free = resolve_relative_url(add_data.repo, रिक्त, 1);
		add_data.realrepo = to_free;
	} अन्यथा यदि (is_dir_sep(add_data.repo[0]) || म_अक्षर(add_data.repo, ':')) {
		add_data.realrepo = add_data.repo;
	} अन्यथा {
		die(_("repo URL: '%s' must be absolute or begin with ./|../"),
		    add_data.repo);
	}

	/*
	 * normalize path:
	 * multiple //; leading ./; /./; /../;
	 */
	normalize_path_copy(add_data.sm_path, add_data.sm_path);
	strip_dir_trailing_slashes(add_data.sm_path);

	यदि (validate_submodule_path(add_data.sm_path) < 0)
		die(रिक्त);

	die_on_index_match(add_data.sm_path, force);
	die_on_repo_without_commits(add_data.sm_path);

	यदि (!force) {
		संरचना child_process cp = CHILD_PROCESS_INIT;

		cp.git_cmd = 1;
		cp.no_stdout = 1;
		strvec_pushl(&cp.args, "add", "--dry-run", "--ignore-missing",
			     "--no-warn-embedded-repo", add_data.sm_path, रिक्त);
		यदि ((ret = pipe_command(&cp, रिक्त, 0, रिक्त, 0, &sb, 0))) {
			strbuf_complete_पङ्क्ति(&sb);
			ख_माला_दो(sb.buf, मानक_त्रुटि);
			गच्छ cleanup;
		}
	}

	यदि (!add_data.sm_name)
		add_data.sm_name = add_data.sm_path;

	existing = submodule_from_name(the_repository,
					null_oid(the_hash_algo),
					add_data.sm_name);

	यदि (existing && existing->path &&
	    म_भेद(existing->path, add_data.sm_path)) {
		यदि (!force) {
			die(_("submodule name '%s' already used for path '%s'"),
			    add_data.sm_name, existing->path);
		}
		/* --force: build <name><n> until unique */
		लागि (पूर्णाङ्क i = 1; ; i++) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "%s%d", add_data.sm_name, i);
			यदि (!submodule_from_name(the_repository,
						null_oid(the_hash_algo),
						buf.buf)) {
				रोक;
			}
		}
		add_data.sm_name = sm_name_to_free = strbuf_detach(&buf, रिक्त);
	}

	यदि (check_submodule_name(add_data.sm_name))
		die(_("'%s' is not a valid submodule name"), add_data.sm_name);

	add_data.prefix = prefix;
	add_data.force = !!force;
	add_data.quiet = !!quiet;
	add_data.progress = !!progress;
	add_data.dissociate = !!dissociate;

	यदि (the_repository->repository_format_submodule_path_cfg)
		create_default_gitdir_config(add_data.sm_name);

	यदि (add_submodule(&add_data))
		गच्छ cleanup;
	configure_added_submodule(&add_data);

	ret = 0;
cleanup:
	मुक्त(sm_name_to_free);
	मुक्त(add_data.sm_path);
	मुक्त(to_free);
	strbuf_release(&sb);

	फर्काऊ ret;
}

पूर्णाङ्क cmd_submodule__helper(पूर्णाङ्क argc,
			  अचर वर्ण **argv,
			  अचर वर्ण *prefix,
			  संरचना repository *repo)
{
	parse_opt_subcommand_fn *fn = रिक्त;
	अचर वर्ण *अचर usage[] = {
		N_("git submodule--helper <command>"),
		रिक्त
	};
	संरचना option options[] = {
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
		OPT_SUBCOMMAND("push-check", &fn, थप_check),
		OPT_SUBCOMMAND("absorbgitdirs", &fn, absorb_git_dirs),
		OPT_SUBCOMMAND("set-url", &fn, module_set_url),
		OPT_SUBCOMMAND("set-branch", &fn, module_set_branch),
		OPT_SUBCOMMAND("create-branch", &fn, module_create_branch),
		OPT_SUBCOMMAND("get-default-remote", &fn, module_get_default_remote),
		OPT_END()
	};
	argc = parse_options(argc, argv, prefix, options, usage, 0);

	फर्काऊ fn(argc, argv, prefix, repo);
}
