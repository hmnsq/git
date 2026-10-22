#घोषणा USE_THE_REPOSITORY_VARIABLE

#समावेश "builtin.h"
#समावेश "abspath.h"
#समावेश "environment.h"
#समावेश "gettext.h"
#समावेश "hex.h"

#समावेश "config.h"
#समावेश "parse-options.h"
#समावेश "quote.h"
#समावेश "path.h"
#समावेश "pathspec.h"
#समावेश "preload-index.h"
#समावेश "dir.h"
#समावेश "read-cache.h"
#समावेश "setup.h"
#समावेश "sparse-index.h"
#समावेश "submodule.h"
#समावेश "submodule-config.h"
#समावेश "string-list.h"
#समावेश "run-command.h"
#समावेश "remote.h"
#समावेश "refs.h"
#समावेश "refspec.h"
#समावेश "revision.h"
#समावेश "diffcore.h"
#समावेश "diff.h"
#समावेश "object-file.h"
#समावेश "object-name.h"
#समावेश "odb.h"
#समावेश "odb/source.h"
#समावेश "odb/source-files.h"
#समावेश "advice.h"
#समावेश "branch.h"
#समावेश "list-objects-filter-options.h"
#समावेश "wildmatch.h"
#समावेश "strbuf.h"
#समावेश "url.h"

#घोषणा OPT_QUIET (1 << 0)
#घोषणा OPT_CACHED (1 << 1)
#घोषणा OPT_RECURSIVE (1 << 2)
#घोषणा OPT_FORCE (1 << 3)

प्रकार व्योम (*each_submodule_fn)(स्थिर काष्ठा cache_entry *list_item,
				  व्योम *cb_तथ्य);

अटल अक्षर *get_default_remote(व्योम)
{
	वापस xstrdup(repo_default_remote(the_repository));
}

अटल अक्षर *resolve_relative_url(स्थिर अक्षर *rel_url, स्थिर अक्षर *up_path, पूर्णांक quiet)
{
	अक्षर *remoteurl, *resolved_url;
	अक्षर *remote = get_default_remote();
	काष्ठा strbuf remotesb = STRBUF_INIT;

	strbuf_addf(&remotesb, "remote.%s.url", remote);
	अगर (repo_config_get_string(the_repository, remotesb.buf, &remoteurl)) {
		अगर (!quiet)
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

	वापस resolved_url;
}

अटल पूर्णांक get_default_remote_submodule(स्थिर अक्षर *module_path, अक्षर **default_remote)
{
	स्थिर काष्ठा submodule *sub;
	काष्ठा repository subrepo;
	स्थिर अक्षर *remote_name = शून्य;
	अक्षर *url = शून्य;
	पूर्णांक ret = 0;

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), module_path);
	अगर (sub && sub->url) {
		url = xstrdup(sub->url);

		/* Possibly a url relative to parent */
		अगर (starts_with_dot_dot_slash(url) ||
		    starts_with_dot_slash(url)) {
			अक्षर *oldurl = url;

			url = resolve_relative_url(oldurl, शून्य, 1);
			मुक्त(oldurl);
		}
	}

	अगर (repo_submodule_init(&subrepo, the_repository, module_path,
				null_oid(the_hash_algo)) < 0) {
		ret = die_message(_("could not get a repository handle for submodule '%s'"),
				   module_path);
		जाओ out;
	}

	/* Look up by URL first */
	अगर (url)
		remote_name = repo_remote_from_url(&subrepo, url);
	अगर (!remote_name)
		remote_name = repo_default_remote(&subrepo);

	*default_remote = xstrdup(remote_name);

out:
	repo_clear(&subrepo);
	मुक्त(url);

	वापस ret;
}

अटल पूर्णांक module_get_default_remote(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
				     काष्ठा repository *repo UNUSED)
{
	स्थिर अक्षर *path;
	अक्षर *resolved_path = शून्य;
	अक्षर *default_remote = शून्य;
	पूर्णांक code;
	काष्ठा option options[] = {
		OPT_END()
	};
	स्थिर अक्षर *स्थिर usage[] = {
		N_("git submodule--helper get-default-remote <path>"),
		शून्य
	};

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	अगर (argc != 1)
		usage_with_options(usage, options);

	path = argv[0];
	अगर (prefix && *prefix && !is_absolute_path(path)) {
		resolved_path = xstrfmt("%s%s", prefix, path);
		path = resolved_path;
	}

	code = get_default_remote_submodule(path, &default_remote);
	अगर (code) {
		मुक्त(resolved_path);
		वापस code;
	}

	म_लिखो("%s\n", default_remote);
	मुक्त(default_remote);
	मुक्त(resolved_path);
	वापस 0;
}

/* the result should be freed by the caller. */
अटल अक्षर *get_submodule_displaypath(स्थिर अक्षर *path, स्थिर अक्षर *prefix,
				       स्थिर अक्षर *super_prefix)
{
	अगर (prefix && super_prefix) {
		BUG("cannot have prefix '%s' and superprefix '%s'",
		    prefix, super_prefix);
	} अन्यथा अगर (prefix) {
		काष्ठा strbuf sb = STRBUF_INIT;
		अक्षर *displaypath = xstrdup(relative_path(path, prefix, &sb));
		strbuf_release(&sb);
		वापस displaypath;
	} अन्यथा अगर (super_prefix) {
		वापस xstrfmt("%s%s", super_prefix, path);
	} अन्यथा {
		वापस xstrdup(path);
	}
}

अटल अक्षर *compute_rev_name(स्थिर अक्षर *sub_path, स्थिर अक्षर* object_id)
{
	काष्ठा strbuf sb = STRBUF_INIT;
	स्थिर अक्षर ***d;

	अटल स्थिर अक्षर *describe_bare[] = { शून्य };

	अटल स्थिर अक्षर *describe_tags[] = { "--tags", शून्य };

	अटल स्थिर अक्षर *describe_contains[] = { "--contains", शून्य };

	अटल स्थिर अक्षर *describe_all_always[] = { "--all", "--always", शून्य };

	अटल स्थिर अक्षर **describe_argv[] = { describe_bare, describe_tags,
						describe_contains,
						describe_all_always, शून्य };

	क्रम (d = describe_argv; *d; d++) {
		काष्ठा child_process cp = CHILD_PROCESS_INIT;
		prepare_submodule_repo_env(&cp.env);
		cp.dir = sub_path;
		cp.git_cmd = 1;
		cp.no_stderr = 1;

		strvec_धकेलो(&cp.args, "describe");
		strvec_pushv(&cp.args, *d);
		strvec_धकेलो(&cp.args, object_id);

		अगर (!capture_command(&cp, &sb, 0)) {
			strbuf_strip_suffix(&sb, "\n");
			वापस strbuf_detach(&sb, शून्य);
		}
	}

	strbuf_release(&sb);
	वापस शून्य;
}

काष्ठा module_list {
	स्थिर काष्ठा cache_entry **entries;
	पूर्णांक alloc, nr;
};
#घोषणा MODULE_LIST_INIT { 0 }

अटल व्योम module_list_release(काष्ठा module_list *ml)
{
	मुक्त(ml->entries);
}

अटल पूर्णांक module_list_compute(स्थिर अक्षर **argv,
			       स्थिर अक्षर *prefix,
			       काष्ठा pathspec *pathspec,
			       काष्ठा module_list *list)
{
	पूर्णांक परिणाम = 0;
	अक्षर *ps_matched = शून्य;

	parse_pathspec(pathspec, 0,
		       PATHSPEC_PREFER_FULL,
		       prefix, argv);

	अगर (pathspec->nr)
		ps_matched = xcalloc(pathspec->nr, 1);

	अगर (repo_read_सूचकांक(the_repository) < 0)
		die(_("index file corrupt"));

	क्रम (माप_प्रकार i = 0; i < the_repository->सूचकांक->cache_nr; i++) {
		स्थिर काष्ठा cache_entry *ce = the_repository->सूचकांक->cache[i];

		अगर (!match_pathspec(the_repository->सूचकांक, pathspec, ce->name, ce_namelen(ce),
				    0, ps_matched, 1) ||
		    !S_ISGITLINK(ce->ce_mode))
			जारी;

		ALLOC_GROW(list->entries, list->nr + 1, list->alloc);
		list->entries[list->nr++] = ce;
		जबतक (i + 1 < the_repository->सूचकांक->cache_nr &&
		       !म_भेद(ce->name, the_repository->सूचकांक->cache[i + 1]->name))
			/*
			 * Skip entries with the same name in different stages
			 * to make sure an entry is returned only once.
			 */
			i++;
	}

	अगर (ps_matched && report_path_error(ps_matched, pathspec))
		परिणाम = -1;

	मुक्त(ps_matched);

	वापस परिणाम;
}

अटल व्योम module_list_active(काष्ठा module_list *list)
{
	पूर्णांक i;
	काष्ठा module_list active_modules = MODULE_LIST_INIT;

	क्रम (i = 0; i < list->nr; i++) {
		स्थिर काष्ठा cache_entry *ce = list->entries[i];

		अगर (!is_submodule_active(the_repository, ce->name))
			जारी;

		ALLOC_GROW(active_modules.entries,
			   active_modules.nr + 1,
			   active_modules.alloc);
		active_modules.entries[active_modules.nr++] = ce;
	}

	module_list_release(list);
	*list = active_modules;
}

अटल अक्षर *get_up_path(स्थिर अक्षर *path)
{
	काष्ठा strbuf sb = STRBUF_INIT;

	strbuf_addstrings(&sb, "../", गिनती_slashes(path));

	/*
	 * Check if 'path' ends with slash or not
	 * for having the same output for dir/sub_dir
	 * and dir/sub_dir/
	 */
	अगर (!is_dir_sep(path[म_माप(path) - 1]))
		strbuf_addstr(&sb, "../");

	वापस strbuf_detach(&sb, शून्य);
}

अटल व्योम for_each_listed_submodule(स्थिर काष्ठा module_list *list,
				      each_submodule_fn fn, व्योम *cb_तथ्य)
{
	पूर्णांक i;

	क्रम (i = 0; i < list->nr; i++)
		fn(list->entries[i], cb_तथ्य);
}

काष्ठा foreach_cb {
	पूर्णांक argc;
	स्थिर अक्षर **argv;
	स्थिर अक्षर *prefix;
	स्थिर अक्षर *super_prefix;
	पूर्णांक quiet;
	पूर्णांक recursive;
};
#घोषणा FOREACH_CB_INIT { 0 }

अटल व्योम runcommand_in_submodule_cb(स्थिर काष्ठा cache_entry *list_item,
				       व्योम *cb_तथ्य)
{
	काष्ठा foreach_cb *info = cb_तथ्य;
	स्थिर अक्षर *path = list_item->name;
	स्थिर काष्ठा object_id *ce_oid = &list_item->oid;
	स्थिर काष्ठा submodule *sub;
	काष्ठा child_process cp = CHILD_PROCESS_INIT;
	अक्षर *displaypath;

	अगर (validate_submodule_path(path) < 0)
		die(शून्य);

	displaypath = get_submodule_displaypath(path, info->prefix,
						info->super_prefix);

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);

	अगर (!sub)
		die(_("No url found for submodule path '%s' in .gitmodules"),
			displaypath);

	अगर (!is_submodule_populated_gently(path, शून्य))
		जाओ cleanup;

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
	अगर (info->argc == 1) {
		अक्षर *toplevel = xgetcwd();
		काष्ठा strbuf sb = STRBUF_INIT;

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

	अगर (!info->quiet)
		म_लिखो(_("Entering '%s'\n"), displaypath);

	अगर (info->argv[0]) {
		अगर (run_command(&cp))
			die(_("run_command returned non-zero status for %s\n."),
			    displaypath);
	} अन्यथा {
		child_process_clear(&cp);
	}

	अगर (info->recursive) {
		काष्ठा child_process cpr = CHILD_PROCESS_INIT;

		cpr.git_cmd = 1;
		cpr.dir = path;
		prepare_submodule_repo_env(&cpr.env);

		strvec_pushl(&cpr.args, "submodule--helper", "foreach", "--recursive",
			     शून्य);
		strvec_pushf(&cpr.args, "--super-prefix=%s/", displaypath);

		अगर (info->quiet)
			strvec_धकेलो(&cpr.args, "--quiet");

		strvec_धकेलो(&cpr.args, "--");
		strvec_pushv(&cpr.args, info->argv);

		अगर (run_command(&cpr))
			die(_("run_command returned non-zero status while "
				"recursing in the nested submodules of %s\n."),
				displaypath);
	}

cleanup:
	मुक्त(displaypath);
}

अटल पूर्णांक module_foreach(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
			  काष्ठा repository *repo UNUSED)
{
	काष्ठा foreach_cb info = FOREACH_CB_INIT;
	काष्ठा pathspec pathspec = { 0 };
	काष्ठा module_list list = MODULE_LIST_INIT;
	काष्ठा option module_foreach_options[] = {
		OPT__SUPER_PREFIX(&info.super_prefix),
		OPT__QUIET(&info.quiet, N_("suppress output of entering each submodule command")),
		OPT_BOOL(0, "recursive", &info.recursive,
			 N_("recurse into nested submodules")),
		OPT_END()
	};
	स्थिर अक्षर *स्थिर git_submodule_helper_usage[] = {
		N_("git submodule foreach [--quiet] [--recursive] [--] <command>"),
		शून्य
	};
	पूर्णांक ret = 1;

	argc = parse_options(argc, argv, prefix, module_foreach_options,
			     git_submodule_helper_usage, 0);

	अगर (module_list_compute(शून्य, prefix, &pathspec, &list) < 0)
		जाओ cleanup;

	info.argc = argc;
	info.argv = argv;
	info.prefix = prefix;

	for_each_listed_submodule(&list, runcommand_in_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	वापस ret;
}

काष्ठा init_cb {
	स्थिर अक्षर *prefix;
	स्थिर अक्षर *super_prefix;
	अचिन्हित पूर्णांक flags;
};
#घोषणा INIT_CB_INIT { 0 }

अटल पूर्णांक validate_and_set_submodule_gitdir(काष्ठा strbuf *gitdir_path,
					     स्थिर अक्षर *submodule_name)
{
	स्थिर अक्षर *मूल्य;
	अक्षर *कुंजी;

	अगर (validate_submodule_git_dir(gitdir_path->buf, submodule_name))
		वापस -1;

	कुंजी = xstrfmt("submodule.%s.gitdir", submodule_name);

	/* Nothing to do if the config already exists. */
	अगर (!repo_config_get_string_tmp(the_repository, कुंजी, &मूल्य)) {
		मुक्त(कुंजी);
		वापस 0;
	}

	अगर (repo_config_set_gently(the_repository, कुंजी, gitdir_path->buf)) {
		मुक्त(कुंजी);
		वापस -1;
	}

	मुक्त(कुंजी);
	वापस 0;
}

अटल व्योम create_default_gitdir_config(स्थिर अक्षर *submodule_name)
{
	काष्ठा strbuf gitdir_path = STRBUF_INIT;
	काष्ठा git_hash_ctx ctx;
	अक्षर hex_name_hash[GIT_MAX_HEXSZ + 1], header[128];
	अचिन्हित अक्षर raw_name_hash[GIT_MAX_RAWSZ];
	पूर्णांक header_len;

	/* Case 1: try the plain module name */
	repo_git_path_append(the_repository, &gitdir_path, "modules/%s", submodule_name);
	अगर (!validate_and_set_submodule_gitdir(&gitdir_path, submodule_name)) {
		strbuf_release(&gitdir_path);
		वापस;
	}

	/* Case 2.1: Try URI-safe (RFC3986) encoding first, this fixes nested gitdirs */
	strbuf_reset(&gitdir_path);
	repo_git_path_append(the_repository, &gitdir_path, "modules/");
	strbuf_addstr_urlencode(&gitdir_path, submodule_name, is_rfc3986_unreserved);
	अगर (!validate_and_set_submodule_gitdir(&gitdir_path, submodule_name)) {
		strbuf_release(&gitdir_path);
		वापस;
	}

	/* Case 2.2: Try extended uppercase URI (RFC3986) encoding, to fix case-folding */
	strbuf_reset(&gitdir_path);
	repo_git_path_append(the_repository, &gitdir_path, "modules/");
	strbuf_addstr_urlencode(&gitdir_path, submodule_name, is_casefolding_rfc3986_unreserved);
	अगर (!validate_and_set_submodule_gitdir(&gitdir_path, submodule_name))
		वापस;

	/* Case 2.3: Try some derived gitdir names, see if one sticks */
	क्रम (अक्षर c = '0'; c <= '9'; c++) {
		strbuf_reset(&gitdir_path);
		repo_git_path_append(the_repository, &gitdir_path, "modules/");
		strbuf_addstr_urlencode(&gitdir_path, submodule_name, is_rfc3986_unreserved);
		strbuf_addch(&gitdir_path, c);
		अगर (!validate_and_set_submodule_gitdir(&gitdir_path, submodule_name))
			वापस;

		strbuf_reset(&gitdir_path);
		repo_git_path_append(the_repository, &gitdir_path, "modules/");
		strbuf_addstr_urlencode(&gitdir_path, submodule_name, is_casefolding_rfc3986_unreserved);
		strbuf_addch(&gitdir_path, c);
		अगर (!validate_and_set_submodule_gitdir(&gitdir_path, submodule_name))
			वापस;
	}

	/* Case 2.4: If all the above failed, try a hash of the name as a last resort */
	header_len = snprintf(header, माप(header),
			      "blob %"PRIuMAX, (uintmax_t)म_माप(submodule_name));
	git_hash_init(&ctx, the_hash_algo);
	git_hash_update(&ctx, header, header_len);
	git_hash_update(&ctx, "\0", 1);
	git_hash_update(&ctx, submodule_name, म_माप(submodule_name));
	git_hash_final(raw_name_hash, &ctx);
	hash_to_hex_algop_r(hex_name_hash, raw_name_hash, the_hash_algo);
	strbuf_reset(&gitdir_path);
	repo_git_path_append(the_repository, &gitdir_path, "modules/%s", hex_name_hash);
	अगर (!validate_and_set_submodule_gitdir(&gitdir_path, submodule_name)) {
		strbuf_release(&gitdir_path);
		वापस;
	}

	/* Case 3: nothing worked, error out */
	die(_("failed to set a valid default config for 'submodule.%s.gitdir'. "
	      "Please ensure it is set, for example by running something like: "
	      "'git config submodule.%s.gitdir .git/modules/%s'"),
	    submodule_name, submodule_name, submodule_name);
}

अटल व्योम init_submodule(स्थिर अक्षर *path, स्थिर अक्षर *prefix,
			   स्थिर अक्षर *super_prefix,
			   अचिन्हित पूर्णांक flags)
{
	स्थिर काष्ठा submodule *sub;
	काष्ठा strbuf sb = STRBUF_INIT;
	स्थिर अक्षर *upd;
	अक्षर *url = शून्य, *displaypath;

	displaypath = get_submodule_displaypath(path, prefix, super_prefix);

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);

	अगर (!sub)
		die(_("No url found for submodule path '%s' in .gitmodules"),
			displaypath);

	/*
	 * NEEDSWORK: In a multi-working-tree world, this needs to be
	 * set in the per-worktree config.
	 *
	 * Set active flag for the submodule being initialized
	 */
	अगर (!is_submodule_active(the_repository, path)) {
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
	अगर (repo_config_get_string(the_repository, sb.buf, &url)) {
		अगर (!sub->url)
			die(_("No url found for submodule path '%s' in .gitmodules"),
				displaypath);

		url = xstrdup(sub->url);

		/* Possibly a url relative to parent */
		अगर (starts_with_dot_dot_slash(url) ||
		    starts_with_dot_slash(url)) {
			अक्षर *oldurl = url;

			url = resolve_relative_url(oldurl, शून्य, 0);
			मुक्त(oldurl);
		}

		अगर (repo_config_set_gently(the_repository, sb.buf, url))
			die(_("Failed to register url for submodule path '%s'"),
			    displaypath);
		अगर (!(flags & OPT_QUIET))
			ख_लिखो(मानक_त्रुटि,
				_("Submodule '%s' (%s) registered for path '%s'\n"),
				sub->name, url, displaypath);
	}
	strbuf_reset(&sb);

	/* Copy "update" setting when it is not set yet */
	strbuf_addf(&sb, "submodule.%s.update", sub->name);
	अगर (repo_config_get_string_tmp(the_repository, sb.buf, &upd) &&
	    sub->update_strategy.type != SM_UPDATE_UNSPECIFIED) {
		अगर (sub->update_strategy.type == SM_UPDATE_COMMAND) {
			ख_लिखो(मानक_त्रुटि, _("warning: command update mode suggested for submodule '%s'\n"),
				sub->name);
			upd = "none";
		} अन्यथा {
			upd = submodule_update_type_to_string(sub->update_strategy.type);
		}

		अगर (repo_config_set_gently(the_repository, sb.buf, upd))
			die(_("Failed to register update mode for submodule path '%s'"), displaypath);
	}

	अगर (the_repository->repository_format_submodule_path_cfg)
		create_default_gitdir_config(sub->name);

	strbuf_release(&sb);
	मुक्त(displaypath);
	मुक्त(url);
}

अटल व्योम init_submodule_cb(स्थिर काष्ठा cache_entry *list_item, व्योम *cb_तथ्य)
{
	काष्ठा init_cb *info = cb_तथ्य;

	init_submodule(list_item->name, info->prefix, info->super_prefix,
		       info->flags);
}

अटल पूर्णांक module_init(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
		       काष्ठा repository *repo UNUSED)
{
	काष्ठा init_cb info = INIT_CB_INIT;
	काष्ठा pathspec pathspec = { 0 };
	काष्ठा module_list list = MODULE_LIST_INIT;
	पूर्णांक quiet = 0;
	काष्ठा option module_init_options[] = {
		OPT__QUIET(&quiet, N_("suppress output for initializing a submodule")),
		OPT_END()
	};
	स्थिर अक्षर *स्थिर git_submodule_helper_usage[] = {
		N_("git submodule init [<options>] [<path>]"),
		शून्य
	};
	पूर्णांक ret = 1;

	argc = parse_options(argc, argv, prefix, module_init_options,
			     git_submodule_helper_usage, 0);

	अगर (module_list_compute(argv, prefix, &pathspec, &list) < 0)
		जाओ cleanup;

	/*
	 * If there are no path args and submodule.active is set then,
	 * by default, only initialize 'active' modules.
	 */
	अगर (!argc && !repo_config_get(the_repository, "submodule.active"))
		module_list_active(&list);

	info.prefix = prefix;
	अगर (quiet)
		info.flags |= OPT_QUIET;

	for_each_listed_submodule(&list, init_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	वापस ret;
}

काष्ठा status_cb {
	स्थिर अक्षर *prefix;
	स्थिर अक्षर *super_prefix;
	अचिन्हित पूर्णांक flags;
};
#घोषणा STATUS_CB_INIT { 0 }

अटल व्योम print_status(अचिन्हित पूर्णांक flags, अक्षर state, स्थिर अक्षर *path,
			 स्थिर काष्ठा object_id *oid, स्थिर अक्षर *displaypath)
{
	अगर (flags & OPT_QUIET)
		वापस;

	म_लिखो("%c%s %s", state, oid_to_hex(oid), displaypath);

	अगर (state == ' ' || state == '+') {
		अक्षर *name = compute_rev_name(path, oid_to_hex(oid));

		अगर (name)
			म_लिखो(" (%s)", name);
		मुक्त(name);
	}

	म_लिखो("\n");
}

अटल पूर्णांक handle_submodule_शीर्ष_ref(स्थिर काष्ठा reference *ref, व्योम *cb_तथ्य)
{
	काष्ठा object_id *output = cb_तथ्य;

	अगर (ref->oid)
		oidcpy(output, ref->oid);

	वापस 0;
}

अटल व्योम status_submodule(स्थिर अक्षर *path, स्थिर काष्ठा object_id *ce_oid,
			     अचिन्हित पूर्णांक ce_flags, स्थिर अक्षर *prefix,
			     स्थिर अक्षर *super_prefix, अचिन्हित पूर्णांक flags)
{
	अक्षर *displaypath;
	काष्ठा strvec diff_files_args = STRVEC_INIT;
	काष्ठा rev_info rev = REV_INFO_INIT;
	काष्ठा strbuf buf = STRBUF_INIT;
	स्थिर अक्षर *git_dir;

	अगर (validate_submodule_path(path) < 0)
		die(शून्य);

	अगर (!submodule_from_path(the_repository, null_oid(the_hash_algo), path))
		die(_("no submodule mapping found in .gitmodules for path '%s'"),
		      path);

	displaypath = get_submodule_displaypath(path, prefix, super_prefix);

	अगर ((CE_STAGEMASK & ce_flags) >> CE_STAGESHIFT) {
		print_status(flags, 'U', path, null_oid(the_hash_algo), displaypath);
		जाओ cleanup;
	}

	strbuf_addf(&buf, "%s/.git", path);
	git_dir = read_gitfile(buf.buf);
	अगर (!git_dir)
		git_dir = buf.buf;

	अगर (!is_submodule_active(the_repository, path) ||
	    !is_git_directory(git_dir)) {
		print_status(flags, '-', path, ce_oid, displaypath);
		strbuf_release(&buf);
		जाओ cleanup;
	}
	strbuf_release(&buf);

	strvec_pushl(&diff_files_args, "diff-files",
		     "--ignore-submodules=dirty", "--quiet", "--",
		     path, शून्य);

	repo_config(the_repository, git_diff_basic_config, शून्य);

	repo_init_revisions(the_repository, &rev, शून्य);
	rev.abbrev = 0;
	setup_revisions_from_strvec(&diff_files_args, &rev, शून्य);
	run_diff_files(&rev, 0);

	अगर (!diff_परिणाम_code(&rev)) {
		print_status(flags, ' ', path, ce_oid,
			     displaypath);
	} अन्यथा अगर (!(flags & OPT_CACHED)) {
		काष्ठा object_id oid;
		काष्ठा ref_store *refs = repo_get_submodule_ref_store(the_repository,
								      path);

		अगर (!refs) {
			print_status(flags, '-', path, ce_oid, displaypath);
			जाओ cleanup;
		}
		अगर (refs_शीर्ष_ref(refs, handle_submodule_शीर्ष_ref, &oid))
			die(_("could not resolve HEAD ref inside the "
			      "submodule '%s'"), path);

		print_status(flags, '+', path, &oid, displaypath);
	} अन्यथा {
		print_status(flags, '+', path, ce_oid, displaypath);
	}

	अगर (flags & OPT_RECURSIVE) {
		काष्ठा child_process cpr = CHILD_PROCESS_INIT;
		पूर्णांक res;

		cpr.git_cmd = 1;
		cpr.dir = path;
		prepare_submodule_repo_env(&cpr.env);

		strvec_pushl(&cpr.args, "submodule--helper", "status",
			     "--recursive", शून्य);
		strvec_pushf(&cpr.args, "--super-prefix=%s/", displaypath);

		अगर (flags & OPT_CACHED)
			strvec_धकेलो(&cpr.args, "--cached");

		अगर (flags & OPT_QUIET)
			strvec_धकेलो(&cpr.args, "--quiet");

		res = run_command(&cpr);
		अगर (res == SIGPIPE + 128)
			उठाओ(SIGPIPE);
		अन्यथा अगर (res)
			die(_("failed to recurse into submodule '%s'"), path);
	}

cleanup:
	strvec_clear(&diff_files_args);
	मुक्त(displaypath);
	release_revisions(&rev);
}

अटल व्योम status_submodule_cb(स्थिर काष्ठा cache_entry *list_item,
				व्योम *cb_तथ्य)
{
	काष्ठा status_cb *info = cb_तथ्य;

	status_submodule(list_item->name, &list_item->oid, list_item->ce_flags,
			 info->prefix, info->super_prefix, info->flags);
}

अटल पूर्णांक module_status(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
			 काष्ठा repository *repo UNUSED)
{
	काष्ठा status_cb info = STATUS_CB_INIT;
	काष्ठा pathspec pathspec = { 0 };
	काष्ठा module_list list = MODULE_LIST_INIT;
	पूर्णांक quiet = 0;
	काष्ठा option module_status_options[] = {
		OPT__SUPER_PREFIX(&info.super_prefix),
		OPT__QUIET(&quiet, N_("suppress submodule status output")),
		OPT_BIT(0, "cached", &info.flags, N_("use commit stored in the index instead of the one stored in the submodule HEAD"), OPT_CACHED),
		OPT_BIT(0, "recursive", &info.flags, N_("recurse into nested submodules"), OPT_RECURSIVE),
		OPT_END()
	};
	स्थिर अक्षर *स्थिर git_submodule_helper_usage[] = {
		N_("git submodule status [--quiet] [--cached] [--recursive] [<path>...]"),
		शून्य
	};
	पूर्णांक ret = 1;

	argc = parse_options(argc, argv, prefix, module_status_options,
			     git_submodule_helper_usage, 0);

	अगर (module_list_compute(argv, prefix, &pathspec, &list) < 0)
		जाओ cleanup;

	info.prefix = prefix;
	अगर (quiet)
		info.flags |= OPT_QUIET;

	for_each_listed_submodule(&list, status_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	वापस ret;
}

काष्ठा module_cb {
	अचिन्हित पूर्णांक mod_src;
	अचिन्हित पूर्णांक mod_dst;
	काष्ठा object_id oid_src;
	काष्ठा object_id oid_dst;
	अक्षर status;
	अक्षर *sm_path;
};
#घोषणा MODULE_CB_INIT { 0 }

अटल व्योम module_cb_release(काष्ठा module_cb *mcb)
{
	मुक्त(mcb->sm_path);
}

काष्ठा module_cb_list {
	काष्ठा module_cb **entries;
	पूर्णांक alloc, nr;
};
#घोषणा MODULE_CB_LIST_INIT { 0 }

अटल व्योम module_cb_list_release(काष्ठा module_cb_list *mcbl)
{
	पूर्णांक i;

	क्रम (i = 0; i < mcbl->nr; i++) {
		काष्ठा module_cb *mcb = mcbl->entries[i];

		module_cb_release(mcb);
		मुक्त(mcb);
	}
	मुक्त(mcbl->entries);
}

काष्ठा summary_cb {
	पूर्णांक argc;
	स्थिर अक्षर **argv;
	स्थिर अक्षर *prefix;
	स्थिर अक्षर *super_prefix;
	अचिन्हित पूर्णांक cached: 1;
	अचिन्हित पूर्णांक for_status: 1;
	अचिन्हित पूर्णांक files: 1;
	पूर्णांक summary_limit;
};
#घोषणा SUMMARY_CB_INIT { 0 }

क्रमागत diff_cmd {
	DIFF_सूचकांक,
	DIFF_FILES
};

अटल अक्षर *verify_submodule_committish(स्थिर अक्षर *sm_path,
					 स्थिर अक्षर *committish)
{
	काष्ठा child_process cp_rev_parse = CHILD_PROCESS_INIT;
	काष्ठा strbuf परिणाम = STRBUF_INIT;

	cp_rev_parse.git_cmd = 1;
	cp_rev_parse.dir = sm_path;
	prepare_submodule_repo_env(&cp_rev_parse.env);
	strvec_pushl(&cp_rev_parse.args, "rev-parse", "-q", "--short", शून्य);
	strvec_pushf(&cp_rev_parse.args, "%s^0", committish);
	strvec_धकेलो(&cp_rev_parse.args, "--");

	अगर (capture_command(&cp_rev_parse, &परिणाम, 0))
		वापस शून्य;

	strbuf_trim_trailing_newline(&परिणाम);
	वापस strbuf_detach(&परिणाम, शून्य);
}

अटल व्योम print_submodule_summary(काष्ठा summary_cb *info, स्थिर अक्षर *errmsg,
				    पूर्णांक योग_commits, स्थिर अक्षर *displaypath,
				    स्थिर अक्षर *src_abbrev, स्थिर अक्षर *dst_abbrev,
				    काष्ठा module_cb *p)
{
	अगर (p->status == 'T') {
		अगर (S_ISGITLINK(p->mod_dst))
			म_लिखो(_("* %s %s(blob)->%s(submodule)"),
				 displaypath, src_abbrev, dst_abbrev);
		अन्यथा
			म_लिखो(_("* %s %s(submodule)->%s(blob)"),
				 displaypath, src_abbrev, dst_abbrev);
	} अन्यथा {
		म_लिखो("* %s %s...%s",
			displaypath, src_abbrev, dst_abbrev);
	}

	अगर (योग_commits < 0)
		म_लिखो(":\n");
	अन्यथा
		म_लिखो(" (%d):\n", योग_commits);

	अगर (errmsg) {
		म_लिखो(_("%s"), errmsg);
	} अन्यथा अगर (योग_commits > 0) {
		काष्ठा child_process cp_log = CHILD_PROCESS_INIT;

		cp_log.git_cmd = 1;
		cp_log.dir = p->sm_path;
		prepare_submodule_repo_env(&cp_log.env);
		strvec_pushl(&cp_log.args, "log", शून्य);

		अगर (S_ISGITLINK(p->mod_src) && S_ISGITLINK(p->mod_dst)) {
			अगर (info->summary_limit > 0)
				strvec_pushf(&cp_log.args, "-%d",
					     info->summary_limit);

			strvec_pushl(&cp_log.args, "--pretty=  %m %s",
				     "--first-parent", शून्य);
			strvec_pushf(&cp_log.args, "%s...%s",
				     src_abbrev, dst_abbrev);
		} अन्यथा अगर (S_ISGITLINK(p->mod_dst)) {
			strvec_pushl(&cp_log.args, "--pretty=  > %s",
				     "-1", dst_abbrev, शून्य);
		} अन्यथा {
			strvec_pushl(&cp_log.args, "--pretty=  < %s",
				     "-1", src_abbrev, शून्य);
		}
		run_command(&cp_log);
	}
	म_लिखो("\n");
}

अटल व्योम generate_submodule_summary(काष्ठा summary_cb *info,
				       काष्ठा module_cb *p)
{
	अक्षर *displaypath, *src_abbrev = शून्य, *dst_abbrev;
	पूर्णांक missing_src = 0, missing_dst = 0;
	काष्ठा strbuf errmsg = STRBUF_INIT;
	पूर्णांक योग_commits = -1;

	अगर (!info->cached && oideq(&p->oid_dst, null_oid(the_hash_algo))) {
		अगर (S_ISGITLINK(p->mod_dst)) {
			काष्ठा ref_store *refs = repo_get_submodule_ref_store(the_repository,
									      p->sm_path);

			अगर (refs)
				refs_शीर्ष_ref(refs, handle_submodule_शीर्ष_ref, &p->oid_dst);
		} अन्यथा अगर (S_ISLNK(p->mod_dst) || S_ISREG(p->mod_dst)) {
			काष्ठा stat st;
			पूर्णांक fd = खोलो(p->sm_path, O_RDONLY);

			अगर (fd < 0 || ख_स्थिति(fd, &st) < 0 ||
			    सूचकांक_fd(the_repository->सूचकांक, &p->oid_dst, fd, &st, OBJ_BLOB,
				     p->sm_path, 0))
				error(_("couldn't hash object from '%s'"), p->sm_path);
		} अन्यथा {
			/* for a submodule removal (mode:0000000), don't warn */
			अगर (p->mod_dst)
				warning(_("unexpected mode %o"), p->mod_dst);
		}
	}

	अगर (S_ISGITLINK(p->mod_src)) {
		अगर (p->status != 'D')
			src_abbrev = verify_submodule_committish(p->sm_path,
								 oid_to_hex(&p->oid_src));
		अगर (!src_abbrev) {
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

	अगर (S_ISGITLINK(p->mod_dst)) {
		dst_abbrev = verify_submodule_committish(p->sm_path,
							 oid_to_hex(&p->oid_dst));
		अगर (!dst_abbrev) {
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

	अगर (!missing_src && !missing_dst) {
		काष्ठा child_process cp_rev_list = CHILD_PROCESS_INIT;
		काष्ठा strbuf sb_rev_list = STRBUF_INIT;

		strvec_pushl(&cp_rev_list.args, "rev-list",
			     "--first-parent", "--count", शून्य);
		अगर (S_ISGITLINK(p->mod_src) && S_ISGITLINK(p->mod_dst))
			strvec_pushf(&cp_rev_list.args, "%s...%s",
				     src_abbrev, dst_abbrev);
		अन्यथा
			strvec_धकेलो(&cp_rev_list.args, S_ISGITLINK(p->mod_src) ?
				    src_abbrev : dst_abbrev);
		strvec_धकेलो(&cp_rev_list.args, "--");

		cp_rev_list.git_cmd = 1;
		cp_rev_list.dir = p->sm_path;
		prepare_submodule_repo_env(&cp_rev_list.env);

		अगर (!capture_command(&cp_rev_list, &sb_rev_list, 0))
			योग_commits = म_से_प(sb_rev_list.buf);

		strbuf_release(&sb_rev_list);
	} अन्यथा {
		/*
		 * Don't give error msg for modification whose dst is not
		 * submodule, i.e., deleted or changed to blob
		 */
		अगर (S_ISGITLINK(p->mod_dst)) {
			अगर (missing_src && missing_dst) {
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

	print_submodule_summary(info, errmsg.len ? errmsg.buf : शून्य,
				योग_commits, displaypath, src_abbrev,
				dst_abbrev, p);

	मुक्त(displaypath);
	मुक्त(src_abbrev);
	मुक्त(dst_abbrev);
	strbuf_release(&errmsg);
}

अटल व्योम prepare_submodule_summary(काष्ठा summary_cb *info,
				      काष्ठा module_cb_list *list)
{
	पूर्णांक i;
	क्रम (i = 0; i < list->nr; i++) {
		स्थिर काष्ठा submodule *sub;
		काष्ठा module_cb *p = list->entries[i];
		काष्ठा strbuf sm_gitdir = STRBUF_INIT;

		अगर (p->status == 'D' || p->status == 'T') {
			generate_submodule_summary(info, p);
			जारी;
		}

		अगर (info->for_status && p->status != 'A' &&
		    (sub = submodule_from_path(the_repository,
					       null_oid(the_hash_algo), p->sm_path))) {
			अक्षर *config_कुंजी = शून्य;
			स्थिर अक्षर *मूल्य;
			पूर्णांक ignore_all = 0;

			config_कुंजी = xstrfmt("submodule.%s.ignore",
					     sub->name);
			अगर (!repo_config_get_string_tmp(the_repository, config_कुंजी, &मूल्य))
				ignore_all = !म_भेद(मूल्य, "all");
			अन्यथा अगर (sub->ignore)
				ignore_all = !म_भेद(sub->ignore, "all");

			मुक्त(config_कुंजी);
			अगर (ignore_all)
				जारी;
		}

		/* Also show added or modified modules which are checked out */
		strbuf_addstr(&sm_gitdir, p->sm_path);
		अगर (is_nonbare_repository_dir(&sm_gitdir))
			generate_submodule_summary(info, p);
		strbuf_release(&sm_gitdir);
	}
}

अटल व्योम submodule_summary_callback(काष्ठा diff_queue_struct *q,
				       काष्ठा diff_options *options UNUSED,
				       व्योम *तथ्य)
{
	पूर्णांक i;
	काष्ठा module_cb_list *list = तथ्य;
	क्रम (i = 0; i < q->nr; i++) {
		काष्ठा diff_filepair *p = q->queue[i];
		काष्ठा module_cb *अस्थायी;

		अगर (!S_ISGITLINK(p->one->mode) && !S_ISGITLINK(p->two->mode))
			जारी;
		अस्थायी = xmalloc(माप(*अस्थायी));
		अस्थायी->mod_src = p->one->mode;
		अस्थायी->mod_dst = p->two->mode;
		अस्थायी->oid_src = p->one->oid;
		अस्थायी->oid_dst = p->two->oid;
		अस्थायी->status = p->status;
		अस्थायी->sm_path = xstrdup(p->one->path);

		ALLOC_GROW(list->entries, list->nr + 1, list->alloc);
		list->entries[list->nr++] = अस्थायी;
	}
}

अटल स्थिर अक्षर *get_diff_cmd(क्रमागत diff_cmd diff_cmd)
{
	चयन (diff_cmd) {
	हाल DIFF_सूचकांक: वापस "diff-index";
	हाल DIFF_FILES: वापस "diff-files";
	शेष: BUG("bad diff_cmd value %d", diff_cmd);
	}
}

अटल पूर्णांक compute_summary_module_list(काष्ठा object_id *शीर्ष_oid,
				       काष्ठा summary_cb *info,
				       क्रमागत diff_cmd diff_cmd)
{
	काष्ठा strvec diff_args = STRVEC_INIT;
	काष्ठा rev_info rev;
	काष्ठा module_cb_list list = MODULE_CB_LIST_INIT;
	पूर्णांक ret = 0;

	strvec_धकेलो(&diff_args, get_diff_cmd(diff_cmd));
	अगर (info->cached)
		strvec_धकेलो(&diff_args, "--cached");
	strvec_pushl(&diff_args, "--ignore-submodules=dirty", "--raw", शून्य);
	अगर (शीर्ष_oid)
		strvec_धकेलो(&diff_args, oid_to_hex(शीर्ष_oid));
	strvec_धकेलो(&diff_args, "--");
	अगर (info->argc)
		strvec_pushv(&diff_args, info->argv);

	repo_config(the_repository, git_diff_basic_config, शून्य);
	repo_init_revisions(the_repository, &rev, info->prefix);
	rev.abbrev = 0;
	precompose_argv_prefix(diff_args.nr, diff_args.v, शून्य);
	setup_revisions_from_strvec(&diff_args, &rev, शून्य);
	rev.diffopt.output_format = DIFF_FORMAT_NO_OUTPUT | DIFF_FORMAT_CALLBACK;
	rev.diffopt.format_callback = submodule_summary_callback;
	rev.diffopt.format_callback_तथ्य = &list;

	अगर (!info->cached) {
		अगर (diff_cmd == DIFF_सूचकांक)
			setup_work_वृक्ष(the_repository);
		अगर (repo_read_सूचकांक_preload(the_repository, &rev.diffopt.pathspec, 0) < 0) {
			लिखो_त्रुटि("repo_read_index_preload");
			ret = -1;
			जाओ cleanup;
		}
	} अन्यथा अगर (repo_read_सूचकांक(the_repository) < 0) {
		लिखो_त्रुटि("repo_read_cache");
		ret = -1;
		जाओ cleanup;
	}

	अगर (diff_cmd == DIFF_सूचकांक)
		run_diff_सूचकांक(&rev, info->cached ? DIFF_सूचकांक_CACHED : 0);
	अन्यथा
		run_diff_files(&rev, 0);
	prepare_submodule_summary(info, &list);
cleanup:
	strvec_clear(&diff_args);
	release_revisions(&rev);
	module_cb_list_release(&list);
	वापस ret;
}

अटल पूर्णांक module_summary(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
			  काष्ठा repository *repo UNUSED)
{
	काष्ठा summary_cb info = SUMMARY_CB_INIT;
	पूर्णांक cached = 0;
	पूर्णांक for_status = 0;
	पूर्णांक files = 0;
	पूर्णांक summary_limit = -1;
	क्रमागत diff_cmd diff_cmd = DIFF_सूचकांक;
	काष्ठा object_id शीर्ष_oid;
	पूर्णांक ret;
	काष्ठा option module_summary_options[] = {
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
	स्थिर अक्षर *स्थिर git_submodule_helper_usage[] = {
		N_("git submodule summary [<options>] [<commit>] [--] [<path>]"),
		शून्य
	};

	argc = parse_options(argc, argv, prefix, module_summary_options,
			     git_submodule_helper_usage, 0);

	अगर (!summary_limit)
		वापस 0;

	अगर (!repo_get_oid(the_repository, argc ? argv[0] : "HEAD", &शीर्ष_oid)) {
		अगर (argc) {
			argv++;
			argc--;
		}
	} अन्यथा अगर (!argc || !म_भेद(argv[0], "HEAD")) {
		/* before the first commit: compare with an empty tree */
		oidcpy(&शीर्ष_oid, the_hash_algo->empty_वृक्ष);
		अगर (argc) {
			argv++;
			argc--;
		}
	} अन्यथा {
		अगर (repo_get_oid(the_repository, "HEAD", &शीर्ष_oid))
			die(_("could not fetch a revision for HEAD"));
	}

	अगर (files) {
		अगर (cached)
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

	ret = compute_summary_module_list((diff_cmd == DIFF_सूचकांक) ? &शीर्ष_oid : शून्य,
					  &info, diff_cmd);
	वापस ret;
}

अटल पूर्णांक module_gitdir(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix UNUSED,
			 काष्ठा repository *repo)
{
	काष्ठा strbuf gitdir = STRBUF_INIT;

	अगर (argc != 2)
		usage(_("git submodule--helper gitdir <name>"));

	submodule_name_to_gitdir(&gitdir, repo, argv[1]);

	म_लिखो("%s\n", gitdir.buf);

	strbuf_release(&gitdir);
	वापस 0;
}

अटल पूर्णांक module_migrate(पूर्णांक argc UNUSED, स्थिर अक्षर **argv UNUSED,
			  स्थिर अक्षर *prefix UNUSED, काष्ठा repository *repo)
{
	काष्ठा strbuf module_dir = STRBUF_INIT;
	सूची *dir;
	काष्ठा dirent *de;
	पूर्णांक repo_version = 0;

	repo_git_path_append(repo, &module_dir, "modules/");

	dir = सूची_खोलो(module_dir.buf);
	अगर (!dir)
		die(_("could not open '%s'"), module_dir.buf);

	जबतक ((de = सूची_पढ़ो(dir))) {
		काष्ठा strbuf gitdir_path = STRBUF_INIT;
		अक्षर *कुंजी;
		स्थिर अक्षर *मूल्य;

		अगर (is_dot_or_dotdot(de->d_name))
			जारी;

		strbuf_addf(&gitdir_path, "%s/%s", module_dir.buf, de->d_name);
		अगर (!is_git_directory(gitdir_path.buf)) {
			strbuf_release(&gitdir_path);
			जारी;
		}
		strbuf_release(&gitdir_path);

		कुंजी = xstrfmt("submodule.%s.gitdir", de->d_name);
		अगर (!repo_config_get_string_tmp(repo, कुंजी, &मूल्य)) {
			/* Already has a gitdir config, nothing to do. */
			मुक्त(कुंजी);
			जारी;
		}
		मुक्त(कुंजी);

		create_default_gitdir_config(de->d_name);
	}

	बंद_सूची(dir);
	strbuf_release(&module_dir);

	repo_config_get_int(the_repository, "core.repositoryformatversion", &repo_version);
	अगर (repo_version == 0 &&
	    repo_config_set_gently(repo, "core.repositoryformatversion", "1"))
		die(_("could not set core.repositoryformatversion to 1.\n"
		      "Please set it for migration to work, for example:\n"
		      "git config core.repositoryformatversion 1"));

	अगर (repo_config_set_gently(repo, "extensions.submodulePathConfig", "true"))
		die(_("could not enable submodulePathConfig extension. It is required\n"
		      "for migration to work. Please enable it in the root repo:\n"
		      "git config extensions.submodulePathConfig true"));

	repo->repository_format_submodule_path_cfg = 1;

	वापस 0;
}

काष्ठा sync_cb {
	स्थिर अक्षर *prefix;
	स्थिर अक्षर *super_prefix;
	अचिन्हित पूर्णांक flags;
};
#घोषणा SYNC_CB_INIT { 0 }

अटल व्योम sync_submodule(स्थिर अक्षर *path, स्थिर अक्षर *prefix,
			   स्थिर अक्षर *super_prefix, अचिन्हित पूर्णांक flags)
{
	स्थिर काष्ठा submodule *sub;
	अक्षर *remote_कुंजी = शून्य;
	अक्षर *sub_origin_url, *super_config_url, *displaypath, *default_remote;
	काष्ठा strbuf sb = STRBUF_INIT;
	अक्षर *sub_config_path = शून्य;
	पूर्णांक code;

	अगर (!is_submodule_active(the_repository, path))
		वापस;

	अगर (validate_submodule_path(path) < 0)
		die(शून्य);

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);

	अगर (sub && sub->url) {
		अगर (starts_with_dot_dot_slash(sub->url) ||
		    starts_with_dot_slash(sub->url)) {
			अक्षर *up_path = get_up_path(path);

			sub_origin_url = resolve_relative_url(sub->url, up_path, 1);
			super_config_url = resolve_relative_url(sub->url, शून्य, 1);
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

	अगर (!(flags & OPT_QUIET))
		म_लिखो(_("Synchronizing submodule url for '%s'\n"),
			 displaypath);

	strbuf_reset(&sb);
	strbuf_addf(&sb, "submodule.%s.url", sub->name);
	अगर (repo_config_set_gently(the_repository, sb.buf, super_config_url))
		die(_("failed to register url for submodule path '%s'"),
		      displaypath);

	अगर (!is_submodule_populated_gently(path, शून्य))
		जाओ cleanup;

	strbuf_reset(&sb);
	code = get_default_remote_submodule(path, &default_remote);
	अगर (code)
		निकास(code);

	remote_कुंजी = xstrfmt("remote.%s.url", default_remote);
	मुक्त(default_remote);

	submodule_to_gitdir(the_repository, &sb, path);
	strbuf_addstr(&sb, "/config");

	अगर (repo_config_set_in_file_gently(the_repository, sb.buf, remote_कुंजी, शून्य, sub_origin_url))
		die(_("failed to update remote for submodule '%s'"),
		      path);

	अगर (flags & OPT_RECURSIVE) {
		काष्ठा child_process cpr = CHILD_PROCESS_INIT;

		cpr.git_cmd = 1;
		cpr.dir = path;
		prepare_submodule_repo_env(&cpr.env);

		strvec_pushl(&cpr.args, "submodule--helper", "sync",
			     "--recursive", शून्य);
		strvec_pushf(&cpr.args, "--super-prefix=%s/", displaypath);

		अगर (flags & OPT_QUIET)
			strvec_धकेलो(&cpr.args, "--quiet");

		अगर (run_command(&cpr))
			die(_("failed to recurse into submodule '%s'"),
			      path);
	}

cleanup:
	मुक्त(super_config_url);
	मुक्त(sub_origin_url);
	strbuf_release(&sb);
	मुक्त(remote_कुंजी);
	मुक्त(displaypath);
	मुक्त(sub_config_path);
}

अटल व्योम sync_submodule_cb(स्थिर काष्ठा cache_entry *list_item, व्योम *cb_तथ्य)
{
	काष्ठा sync_cb *info = cb_तथ्य;

	sync_submodule(list_item->name, info->prefix, info->super_prefix,
		       info->flags);
}

अटल पूर्णांक module_sync(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
		       काष्ठा repository *repo UNUSED)
{
	काष्ठा sync_cb info = SYNC_CB_INIT;
	काष्ठा pathspec pathspec = { 0 };
	काष्ठा module_list list = MODULE_LIST_INIT;
	पूर्णांक quiet = 0;
	पूर्णांक recursive = 0;
	काष्ठा option module_sync_options[] = {
		OPT__SUPER_PREFIX(&info.super_prefix),
		OPT__QUIET(&quiet, N_("suppress output of synchronizing submodule url")),
		OPT_BOOL(0, "recursive", &recursive,
			N_("recurse into nested submodules")),
		OPT_END()
	};
	स्थिर अक्षर *स्थिर git_submodule_helper_usage[] = {
		N_("git submodule sync [--quiet] [--recursive] [<path>]"),
		शून्य
	};
	पूर्णांक ret = 1;

	argc = parse_options(argc, argv, prefix, module_sync_options,
			     git_submodule_helper_usage, 0);

	अगर (module_list_compute(argv, prefix, &pathspec, &list) < 0)
		जाओ cleanup;

	info.prefix = prefix;
	अगर (quiet)
		info.flags |= OPT_QUIET;
	अगर (recursive)
		info.flags |= OPT_RECURSIVE;

	for_each_listed_submodule(&list, sync_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	वापस ret;
}

काष्ठा deinit_cb {
	स्थिर अक्षर *prefix;
	अचिन्हित पूर्णांक flags;
};
#घोषणा DEINIT_CB_INIT { 0 }

अटल व्योम deinit_submodule(स्थिर अक्षर *path, स्थिर अक्षर *prefix,
			     अचिन्हित पूर्णांक flags)
{
	स्थिर काष्ठा submodule *sub;
	अक्षर *displaypath = शून्य;
	काष्ठा child_process cp_config = CHILD_PROCESS_INIT;
	काष्ठा strbuf sb_config = STRBUF_INIT;
	अक्षर *sub_git_dir = xstrfmt("%s/.git", path);

	अगर (validate_submodule_path(path) < 0)
		die(शून्य);

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);

	अगर (!sub || !sub->name)
		जाओ cleanup;

	displaypath = get_submodule_displaypath(path, prefix, शून्य);

	/* remove the submodule work tree (unless the user already did it) */
	अगर (is_directory(path)) {
		काष्ठा strbuf sb_rm = STRBUF_INIT;
		स्थिर अक्षर *format;

		अगर (is_directory(sub_git_dir)) {
			अगर (!(flags & OPT_QUIET))
				warning(_("Submodule work tree '%s' contains a .git "
					  "directory. This will be replaced with a "
					  ".git file by using absorbgitdirs."),
					displaypath);

			absorb_git_dir_into_superproject(path, शून्य);

		}

		अगर (!(flags & OPT_FORCE)) {
			काष्ठा child_process cp_rm = CHILD_PROCESS_INIT;

			cp_rm.git_cmd = 1;
			strvec_pushl(&cp_rm.args, "rm", "-qn",
				     path, शून्य);

			अगर (run_command(&cp_rm))
				die(_("Submodule work tree '%s' contains local "
				      "modifications; use '-f' to discard them"),
				      displaypath);
		}

		strbuf_addstr(&sb_rm, path);

		अगर (!remove_dir_recursively(&sb_rm, 0))
			format = _("Cleared directory '%s'\n");
		अन्यथा
			format = _("Could not remove submodule work tree '%s'\n");

		अगर (!(flags & OPT_QUIET))
			म_लिखो(format, displaypath);

		submodule_unset_core_worktree(sub);

		strbuf_release(&sb_rm);
	}

	अगर (सूची_गढ़ो(path, 0777))
		म_लिखो(_("could not create empty submodule directory %s"),
		      displaypath);

	cp_config.git_cmd = 1;
	strvec_pushl(&cp_config.args, "config", "--get-regexp", शून्य);
	strvec_pushf(&cp_config.args, "submodule.%s\\.", sub->name);

	/* remove the .git/config entries (unless the user already did it) */
	अगर (!capture_command(&cp_config, &sb_config, 0) && sb_config.len) {
		अक्षर *sub_कुंजी = xstrfmt("submodule.%s", sub->name);

		/*
		 * remove the whole section so we have a clean state when
		 * the user later decides to init this submodule again
		 */
		repo_config_rename_section_in_file(the_repository, शून्य, sub_कुंजी, शून्य);
		अगर (!(flags & OPT_QUIET))
			म_लिखो(_("Submodule '%s' (%s) unregistered for path '%s'\n"),
				 sub->name, sub->url, displaypath);
		मुक्त(sub_कुंजी);
	}

cleanup:
	मुक्त(displaypath);
	मुक्त(sub_git_dir);
	strbuf_release(&sb_config);
}

अटल व्योम deinit_submodule_cb(स्थिर काष्ठा cache_entry *list_item,
				व्योम *cb_तथ्य)
{
	काष्ठा deinit_cb *info = cb_तथ्य;
	deinit_submodule(list_item->name, info->prefix, info->flags);
}

अटल पूर्णांक module_deinit(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
			 काष्ठा repository *repo UNUSED)
{
	काष्ठा deinit_cb info = DEINIT_CB_INIT;
	काष्ठा pathspec pathspec = { 0 };
	काष्ठा module_list list = MODULE_LIST_INIT;
	पूर्णांक quiet = 0;
	पूर्णांक force = 0;
	पूर्णांक all = 0;
	काष्ठा option module_deinit_options[] = {
		OPT__QUIET(&quiet, N_("suppress submodule status output")),
		OPT__FORCE(&force, N_("remove submodule working trees even if they contain local changes"), 0),
		OPT_BOOL(0, "all", &all, N_("unregister all submodules")),
		OPT_END()
	};
	स्थिर अक्षर *स्थिर git_submodule_helper_usage[] = {
		N_("git submodule deinit [--quiet] [-f | --force] [--all | [--] [<path>...]]"),
		शून्य
	};
	पूर्णांक ret = 1;

	argc = parse_options(argc, argv, prefix, module_deinit_options,
			     git_submodule_helper_usage, 0);

	अगर (all && argc) {
		error("pathspec and --all are incompatible");
		usage_with_options(git_submodule_helper_usage,
				   module_deinit_options);
	}

	अगर (!argc && !all)
		die(_("Use '--all' if you really want to deinitialize all submodules"));

	अगर (module_list_compute(argv, prefix, &pathspec, &list) < 0)
		जाओ cleanup;

	info.prefix = prefix;
	अगर (quiet)
		info.flags |= OPT_QUIET;
	अगर (force)
		info.flags |= OPT_FORCE;

	for_each_listed_submodule(&list, deinit_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	वापस ret;
}

काष्ठा module_clone_तथ्य {
	स्थिर अक्षर *prefix;
	स्थिर अक्षर *path;
	स्थिर अक्षर *name;
	स्थिर अक्षर *url;
	पूर्णांक depth;
	काष्ठा list_objects_filter_options *filter_options;
	क्रमागत ref_storage_format ref_storage_format;
	अचिन्हित पूर्णांक quiet: 1;
	अचिन्हित पूर्णांक progress: 1;
	अचिन्हित पूर्णांक dissociate: 1;
	अचिन्हित पूर्णांक require_init: 1;
	पूर्णांक single_branch;
};
#घोषणा MODULE_CLONE_तथ्य_INIT { \
	.single_branch = -1, \
	.ref_storage_format = REF_STORAGE_FORMAT_UNKNOWN, \
}

काष्ठा submodule_alternate_setup {
	स्थिर अक्षर *submodule_name;
	क्रमागत SUBMODULE_ALTERNATE_ERROR_MODE {
		SUBMODULE_ALTERNATE_ERROR_DIE,
		SUBMODULE_ALTERNATE_ERROR_INFO,
		SUBMODULE_ALTERNATE_ERROR_IGNORE
	} error_mode;
	काष्ठा string_list *reference;
};
#घोषणा SUBMODULE_ALTERNATE_SETUP_INIT { \
	.error_mode = SUBMODULE_ALTERNATE_ERROR_IGNORE, \
}

अटल स्थिर अक्षर alternate_error_advice[] = N_(
"An alternate computed from a superproject's alternate is invalid.\n"
"To allow Git to clone without an alternate in such a case, set\n"
"submodule.alternateErrorStrategy to 'info' or, equivalently, clone with\n"
"'--reference-if-able' instead of '--reference'."
);

अटल पूर्णांक add_possible_reference_from_superproject(
		काष्ठा odb_files_dir *alt_odb, व्योम *sas_cb)
{
	काष्ठा submodule_alternate_setup *sas = sas_cb;
	माप_प्रकार len;

	/*
	 * If the alternate object store is another repository, try the
	 * standard layout with .git/(modules/<name>)+/objects
	 */
	अगर (strip_suffix(alt_odb->abspath, "/objects", &len)) {
		काष्ठा repository alternate;
		अक्षर *sm_alternate;
		काष्ठा strbuf sb = STRBUF_INIT;
		काष्ठा strbuf err = STRBUF_INIT;
		strbuf_add(&sb, alt_odb->abspath, len);

		अगर (repo_init(&alternate, sb.buf, शून्य) < 0)
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
		अगर (sm_alternate) {
			अक्षर *p = strbuf_detach(&sb, शून्य);

			string_list_append(sas->reference, p)->util = p;
			मुक्त(sm_alternate);
		} अन्यथा {
			चयन (sas->error_mode) {
			हाल SUBMODULE_ALTERNATE_ERROR_DIE:
				अगर (advice_enabled(ADVICE_SUBMODULE_ALTERNATE_ERROR_STRATEGY_DIE))
					advise(_(alternate_error_advice));
				die(_("submodule '%s' cannot add alternate: %s"),
				    sas->submodule_name, err.buf);
			हाल SUBMODULE_ALTERNATE_ERROR_INFO:
				fprintf_ln(मानक_त्रुटि, _("submodule '%s' cannot add alternate: %s"),
					sas->submodule_name, err.buf);
			हाल SUBMODULE_ALTERNATE_ERROR_IGNORE:
				; /* nothing */
			}
		}

		strbuf_release(&err);
		strbuf_release(&sb);
	}

	वापस 0;
}

अटल व्योम prepare_possible_alternates(स्थिर अक्षर *sm_name,
		काष्ठा string_list *reference)
{
	अक्षर *sm_alternate = शून्य, *error_strategy = शून्य;
	काष्ठा submodule_alternate_setup sas = SUBMODULE_ALTERNATE_SETUP_INIT;

	repo_config_get_string(the_repository, "submodule.alternateLocation", &sm_alternate);
	अगर (!sm_alternate)
		वापस;

	repo_config_get_string(the_repository, "submodule.alternateErrorStrategy", &error_strategy);

	अगर (!error_strategy)
		error_strategy = xstrdup("die");

	sas.submodule_name = sm_name;
	sas.reference = reference;
	अगर (!म_भेद(error_strategy, "die"))
		sas.error_mode = SUBMODULE_ALTERNATE_ERROR_DIE;
	अन्यथा अगर (!म_भेद(error_strategy, "info"))
		sas.error_mode = SUBMODULE_ALTERNATE_ERROR_INFO;
	अन्यथा अगर (!म_भेद(error_strategy, "ignore"))
		sas.error_mode = SUBMODULE_ALTERNATE_ERROR_IGNORE;
	अन्यथा
		die(_("Value '%s' for submodule.alternateErrorStrategy is not recognized"), error_strategy);

	अगर (!म_भेद(sm_alternate, "superproject"))
		odb_for_each_alternate(the_repository->objects,
				       add_possible_reference_from_superproject, &sas);
	अन्यथा अगर (!म_भेद(sm_alternate, "no"))
		; /* do nothing */
	अन्यथा
		die(_("Value '%s' for submodule.alternateLocation is not recognized"), sm_alternate);

	मुक्त(sm_alternate);
	मुक्त(error_strategy);
}

अटल अक्षर *clone_submodule_sm_gitdir(स्थिर अक्षर *name)
{
	काष्ठा strbuf sb = STRBUF_INIT;
	अक्षर *sm_gitdir;

	submodule_name_to_gitdir(&sb, the_repository, name);
	sm_gitdir = absolute_pathdup(sb.buf);
	strbuf_release(&sb);

	वापस sm_gitdir;
}

अटल पूर्णांक dir_contains_only_dotgit(स्थिर अक्षर *path)
{
	सूची *dir = सूची_खोलो(path);
	काष्ठा dirent *e;
	पूर्णांक ret = 1;

	अगर (!dir)
		वापस 0;

	e = readdir_skip_dot_and_dotdot(dir);
	अगर (!e)
		ret = 0;
	अन्यथा अगर (म_भेद(DEFAULT_GIT_DIR_ENVIRONMENT, e->d_name) ||
		 (e = readdir_skip_dot_and_dotdot(dir))) {
		error("unexpected item '%s' in '%s'", e->d_name, path);
		ret = 0;
	}

	बंद_सूची(dir);
	वापस ret;
}

अटल पूर्णांक clone_submodule(स्थिर काष्ठा module_clone_तथ्य *clone_तथ्य,
			   काष्ठा string_list *reference)
{
	अक्षर *p;
	अक्षर *sm_gitdir = clone_submodule_sm_gitdir(clone_तथ्य->name);
	अक्षर *sm_alternate = शून्य, *error_strategy = शून्य;
	काष्ठा stat st;
	काष्ठा child_process cp = CHILD_PROCESS_INIT;
	स्थिर अक्षर *clone_तथ्य_path = clone_तथ्य->path;
	अक्षर *to_free = शून्य;

	अगर (validate_submodule_path(clone_तथ्य_path) < 0)
		die(शून्य);

	अगर (!is_absolute_path(clone_तथ्य->path))
		clone_तथ्य_path = to_free = xstrfmt("%s/%s", repo_get_work_वृक्ष(the_repository),
						    clone_तथ्य->path);

	अगर (!file_exists(sm_gitdir)) {
		अगर (clone_तथ्य->require_init && !stat(clone_तथ्य_path, &st) &&
		    !is_empty_dir(clone_तथ्य_path))
			die(_("directory not empty: '%s'"), clone_तथ्य_path);

		अगर (safe_create_leading_directories_const(the_repository, sm_gitdir) < 0)
			die(_("could not create directory '%s'"), sm_gitdir);

		prepare_possible_alternates(clone_तथ्य->name, reference);

		strvec_धकेलो(&cp.args, "clone");
		strvec_धकेलो(&cp.args, "--no-checkout");
		अगर (clone_तथ्य->quiet)
			strvec_धकेलो(&cp.args, "--quiet");
		अगर (clone_तथ्य->progress)
			strvec_धकेलो(&cp.args, "--progress");
		अगर (clone_तथ्य->depth > 0)
			strvec_pushf(&cp.args, "--depth=%d", clone_तथ्य->depth);
		अगर (reference->nr) {
			काष्ठा string_list_item *item;

			for_each_string_list_item(item, reference)
				strvec_pushl(&cp.args, "--reference",
					     item->string, शून्य);
		}
		अगर (clone_तथ्य->ref_storage_format != REF_STORAGE_FORMAT_UNKNOWN)
			strvec_pushf(&cp.args, "--ref-storage-format=%s",
				     ref_storage_format_to_name(clone_तथ्य->ref_storage_format));
		अगर (clone_तथ्य->dissociate)
			strvec_धकेलो(&cp.args, "--dissociate");
		अगर (sm_gitdir && *sm_gitdir)
			strvec_pushl(&cp.args, "--separate-git-dir", sm_gitdir, शून्य);
		अगर (clone_तथ्य->filter_options && clone_तथ्य->filter_options->choice)
			strvec_pushf(&cp.args, "--filter=%s",
				     expand_list_objects_filter_spec(
					     clone_तथ्य->filter_options));
		अगर (clone_तथ्य->single_branch >= 0)
			strvec_धकेलो(&cp.args, clone_तथ्य->single_branch ?
				    "--single-branch" :
				    "--no-single-branch");

		strvec_धकेलो(&cp.args, "--");
		strvec_धकेलो(&cp.args, clone_तथ्य->url);
		strvec_धकेलो(&cp.args, clone_तथ्य_path);

		cp.git_cmd = 1;
		prepare_submodule_repo_env(&cp.env);
		cp.no_stdin = 1;

		अगर(run_command(&cp))
			die(_("clone of '%s' into submodule path '%s' failed"),
			    clone_तथ्य->url, clone_तथ्य_path);

		अगर (clone_तथ्य->require_init && !stat(clone_तथ्य_path, &st) &&
		    !dir_contains_only_dotgit(clone_तथ्य_path)) {
			अक्षर *dot_git = xstrfmt("%s/.git", clone_तथ्य_path);
			unlink(dot_git);
			मुक्त(dot_git);
			die(_("directory not empty: '%s'"), clone_तथ्य_path);
		}
	} अन्यथा {
		अक्षर *path;

		अगर (clone_तथ्य->require_init && !stat(clone_तथ्य_path, &st) &&
		    !is_empty_dir(clone_तथ्य_path))
			die(_("directory not empty: '%s'"), clone_तथ्य_path);
		अगर (safe_create_leading_directories_const(the_repository, clone_तथ्य_path) < 0)
			die(_("could not create directory '%s'"), clone_तथ्य_path);
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
	अगर (validate_submodule_git_dir(sm_gitdir, clone_तथ्य->name) < 0) {
		अक्षर *शीर्ष = xstrfmt("%s/HEAD", sm_gitdir);
		unlink(शीर्ष);
		मुक्त(शीर्ष);
		die(_("refusing to create/use '%s' in another submodule's git dir. "
		      "Enabling extensions.submodulePathConfig should fix this."),
		    sm_gitdir);
	}

	connect_work_वृक्ष_and_git_dir(clone_तथ्य_path, sm_gitdir, 0);

	p = repo_submodule_path(the_repository, clone_तथ्य_path, "config");
	अगर (!p)
		die(_("could not get submodule directory for '%s'"), clone_तथ्य_path);

	/* setup alternateLocation and alternateErrorStrategy in the cloned submodule if needed */
	repo_config_get_string(the_repository, "submodule.alternateLocation", &sm_alternate);
	अगर (sm_alternate)
		repo_config_set_in_file(the_repository, p, "submodule.alternateLocation",
					sm_alternate);
	repo_config_get_string(the_repository, "submodule.alternateErrorStrategy", &error_strategy);
	अगर (error_strategy)
		repo_config_set_in_file(the_repository, p, "submodule.alternateErrorStrategy",
					error_strategy);

	मुक्त(sm_alternate);
	मुक्त(error_strategy);

	मुक्त(sm_gitdir);
	मुक्त(p);
	मुक्त(to_free);
	वापस 0;
}

अटल पूर्णांक module_clone(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
			काष्ठा repository *repo UNUSED)
{
	पूर्णांक dissociate = 0, quiet = 0, progress = 0, require_init = 0;
	काष्ठा module_clone_तथ्य clone_तथ्य = MODULE_CLONE_तथ्य_INIT;
	काष्ठा string_list reference = STRING_LIST_INIT_NODUP;
	काष्ठा list_objects_filter_options filter_options =
		LIST_OBJECTS_FILTER_INIT;
	स्थिर अक्षर *ref_storage_format = शून्य;

	काष्ठा option module_clone_options[] = {
		OPT_STRING(0, "prefix", &clone_तथ्य.prefix,
			   N_("path"),
			   N_("alternative anchor for relative paths")),
		OPT_STRING(0, "path", &clone_तथ्य.path,
			   N_("path"),
			   N_("where the new submodule will be cloned to")),
		OPT_STRING(0, "name", &clone_तथ्य.name,
			   N_("string"),
			   N_("name of the new submodule")),
		OPT_STRING(0, "url", &clone_तथ्य.url,
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
		OPT_INTEGER(0, "depth", &clone_तथ्य.depth,
			   N_("depth for shallow clones")),
		OPT__QUIET(&quiet, "suppress output for cloning a submodule"),
		OPT_BOOL(0, "progress", &progress,
			   N_("force cloning progress")),
		OPT_BOOL(0, "require-init", &require_init,
			   N_("disallow cloning into non-empty directory")),
		OPT_BOOL(0, "single-branch", &clone_तथ्य.single_branch,
			 N_("clone only one branch, HEAD or --branch")),
		OPT_PARSE_LIST_OBJECTS_FILTER(&filter_options),
		OPT_END()
	};
	स्थिर अक्षर *स्थिर git_submodule_helper_usage[] = {
		N_("git submodule--helper clone [--prefix=<path>] [--quiet] "
		   "[--reference <repository>] [--name <name>] [--depth <depth>] "
		   "[--single-branch] [--filter <filter-spec>] "
		   "--url <url> --path <path>"),
		शून्य
	};

	argc = parse_options(argc, argv, prefix, module_clone_options,
			     git_submodule_helper_usage, 0);

	अगर (ref_storage_format) {
		clone_तथ्य.ref_storage_format = ref_storage_format_by_name(ref_storage_format);
		अगर (clone_तथ्य.ref_storage_format == REF_STORAGE_FORMAT_UNKNOWN)
			die(_("unknown ref storage format '%s'"), ref_storage_format);
	}
	clone_तथ्य.dissociate = !!dissociate;
	clone_तथ्य.quiet = !!quiet;
	clone_तथ्य.progress = !!progress;
	clone_तथ्य.require_init = !!require_init;
	clone_तथ्य.filter_options = &filter_options;

	अगर (argc || !clone_तथ्य.url || !clone_तथ्य.path || !*(clone_तथ्य.path))
		usage_with_options(git_submodule_helper_usage,
				   module_clone_options);

	clone_submodule(&clone_तथ्य, &reference);
	list_objects_filter_release(&filter_options);
	string_list_clear(&reference, 1);
	वापस 0;
}

अटल पूर्णांक determine_submodule_update_strategy(काष्ठा repository *r,
					       पूर्णांक just_cloned,
					       स्थिर अक्षर *path,
					       क्रमागत submodule_update_type update,
					       काष्ठा submodule_update_strategy *out)
{
	स्थिर काष्ठा submodule *sub = submodule_from_path(r, null_oid(the_hash_algo), path);
	अक्षर *कुंजी;
	स्थिर अक्षर *val;
	पूर्णांक ret;

	/*
	 * NEEDSWORK: audit and ensure that update_submodule() has right
	 * to assume that submodule_from_path() above will always succeed.
	 */
	अगर (!sub)
		BUG("update_submodule assumes a submodule exists at path (%s)",
		    path);
	कुंजी = xstrfmt("submodule.%s.update", sub->name);

	अगर (update) {
		out->type = update;
	} अन्यथा अगर (!repo_config_get_string_tmp(r, कुंजी, &val)) {
		अगर (parse_submodule_update_strategy(val, out) < 0) {
			ret = die_message(_("Invalid update mode '%s' configured for submodule path '%s'"),
					  val, path);
			जाओ cleanup;
		}
	} अन्यथा अगर (sub->update_strategy.type != SM_UPDATE_UNSPECIFIED) {
		अगर (sub->update_strategy.type == SM_UPDATE_COMMAND)
			BUG("how did we read update = !command from .gitmodules?");
		out->type = sub->update_strategy.type;
		out->command = sub->update_strategy.command;
	} अन्यथा
		out->type = SM_UPDATE_CHECKOUT;

	अगर (just_cloned &&
	    (out->type == SM_UPDATE_MERGE ||
	     out->type == SM_UPDATE_REBASE ||
	     out->type == SM_UPDATE_NONE))
		out->type = SM_UPDATE_CHECKOUT;

	ret = 0;
cleanup:
	मुक्त(कुंजी);
	वापस ret;
}

काष्ठा update_clone_तथ्य {
	स्थिर काष्ठा submodule *sub;
	काष्ठा object_id oid;
	अचिन्हित just_cloned;
};

काष्ठा submodule_update_clone {
	/* index into 'update_data.list', the list of submodules to look into for cloning */
	पूर्णांक current;

	/* configuration parameters which are passed on to the children */
	स्थिर काष्ठा update_तथ्य *update_तथ्य;

	/* to be consumed by update_submodule() */
	काष्ठा update_clone_तथ्य *update_clone;
	पूर्णांक update_clone_nr; पूर्णांक update_clone_alloc;

	/* If we want to stop as fast as possible and return an error */
	अचिन्हित quickstop : 1;

	/* failed clones to be retried again */
	स्थिर काष्ठा cache_entry **failed_clones;
	पूर्णांक failed_clones_nr, failed_clones_alloc;
};
#घोषणा SUBMODULE_UPDATE_CLONE_INIT { 0 }

अटल व्योम submodule_update_clone_release(काष्ठा submodule_update_clone *suc)
{
	मुक्त(suc->update_clone);
	मुक्त(suc->failed_clones);
}

काष्ठा update_तथ्य {
	स्थिर अक्षर *prefix;
	स्थिर अक्षर *super_prefix;
	अक्षर *displaypath;
	क्रमागत submodule_update_type update_default;
	काष्ठा object_id suboid;
	काष्ठा string_list references;
	काष्ठा submodule_update_strategy update_strategy;
	काष्ठा list_objects_filter_options *filter_options;
	काष्ठा module_list list;
	क्रमागत ref_storage_format ref_storage_format;
	पूर्णांक depth;
	पूर्णांक max_jobs;
	पूर्णांक single_branch;
	पूर्णांक recommend_shallow;
	अचिन्हित पूर्णांक require_init;
	अचिन्हित पूर्णांक force;
	अचिन्हित पूर्णांक quiet;
	अचिन्हित पूर्णांक nofetch;
	अचिन्हित पूर्णांक remote;
	अचिन्हित पूर्णांक progress;
	अचिन्हित पूर्णांक dissociate;
	अचिन्हित पूर्णांक init;
	अचिन्हित पूर्णांक warn_if_uninitialized;
	अचिन्हित पूर्णांक recursive;

	/* copied over from update_clone_data */
	काष्ठा object_id oid;
	अचिन्हित पूर्णांक just_cloned;
	स्थिर अक्षर *sm_path;
};
#घोषणा UPDATE_तथ्य_INIT { \
	.update_strategy = SUBMODULE_UPDATE_STRATEGY_INIT, \
	.list = MODULE_LIST_INIT, \
	.ref_storage_format = REF_STORAGE_FORMAT_UNKNOWN, \
	.recommend_shallow = -1, \
	.references = STRING_LIST_INIT_DUP, \
	.single_branch = -1, \
	.max_jobs = 1, \
}

अटल व्योम update_तथ्य_release(काष्ठा update_तथ्य *ud)
{
	मुक्त(ud->displaypath);
	submodule_update_strategy_release(&ud->update_strategy);
	module_list_release(&ud->list);
}

अटल व्योम अग्र_submodule_warn_missing(काष्ठा submodule_update_clone *suc,
		काष्ठा strbuf *out, स्थिर अक्षर *displaypath)
{
	/*
	 * Only mention uninitialized submodules when their
	 * paths have been specified.
	 */
	अगर (suc->update_तथ्य->warn_if_uninitialized) {
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
अटल पूर्णांक prepare_to_clone_अग्र_submodule(स्थिर काष्ठा cache_entry *ce,
					   काष्ठा child_process *child,
					   काष्ठा submodule_update_clone *suc,
					   काष्ठा strbuf *out)
{
	स्थिर काष्ठा submodule *sub = शून्य;
	स्थिर अक्षर *url = शून्य;
	स्थिर अक्षर *update_string;
	क्रमागत submodule_update_type update_type;
	अक्षर *कुंजी;
	स्थिर काष्ठा update_तथ्य *ud = suc->update_तथ्य;
	अक्षर *displaypath = get_submodule_displaypath(ce->name, ud->prefix,
						      ud->super_prefix);
	काष्ठा strbuf sb = STRBUF_INIT;
	पूर्णांक needs_cloning = 0;
	पूर्णांक need_free_url = 0;

	अगर (ce_stage(ce)) {
		strbuf_addf(out, _("Skipping unmerged submodule %s"), displaypath);
		strbuf_addch(out, '\n');
		जाओ cleanup;
	}

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), ce->name);

	अगर (!sub) {
		अग्र_submodule_warn_missing(suc, out, displaypath);
		जाओ cleanup;
	}

	कुंजी = xstrfmt("submodule.%s.update", sub->name);
	अगर (!repo_config_get_string_tmp(the_repository, कुंजी, &update_string)) {
		update_type = parse_submodule_update_type(update_string);
	} अन्यथा {
		update_type = sub->update_strategy.type;
	}
	मुक्त(कुंजी);

	अगर (suc->update_तथ्य->update_strategy.type == SM_UPDATE_NONE
	    || (suc->update_तथ्य->update_strategy.type == SM_UPDATE_UNSPECIFIED
		&& update_type == SM_UPDATE_NONE)) {
		strbuf_addf(out, _("Skipping submodule '%s'"), displaypath);
		strbuf_addch(out, '\n');
		जाओ cleanup;
	}

	/* Check if the submodule has been initialized. */
	अगर (!is_submodule_active(the_repository, ce->name)) {
		अग्र_submodule_warn_missing(suc, out, displaypath);
		जाओ cleanup;
	}

	strbuf_reset(&sb);
	strbuf_addf(&sb, "submodule.%s.url", sub->name);
	अगर (repo_config_get_string_tmp(the_repository, sb.buf, &url)) {
		अगर (sub->url && (starts_with_dot_slash(sub->url) ||
				 starts_with_dot_dot_slash(sub->url))) {
			url = resolve_relative_url(sub->url, शून्य, 0);
			need_free_url = 1;
		} अन्यथा
			url = sub->url;
	}

	अगर (!url)
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

	अगर (!needs_cloning)
		जाओ cleanup;

	child->git_cmd = 1;
	child->no_stdin = 1;
	child->stdout_to_stderr = 1;
	child->err = -1;
	strvec_धकेलो(&child->args, "submodule--helper");
	strvec_धकेलो(&child->args, "clone");
	अगर (suc->update_तथ्य->progress)
		strvec_धकेलो(&child->args, "--progress");
	अगर (suc->update_तथ्य->quiet)
		strvec_धकेलो(&child->args, "--quiet");
	अगर (suc->update_तथ्य->prefix)
		strvec_pushl(&child->args, "--prefix", suc->update_तथ्य->prefix, शून्य);
	अगर (suc->update_तथ्य->recommend_shallow && sub->recommend_shallow == 1)
		strvec_धकेलो(&child->args, "--depth=1");
	अन्यथा अगर (suc->update_तथ्य->depth)
		strvec_pushf(&child->args, "--depth=%d", suc->update_तथ्य->depth);
	अगर (suc->update_तथ्य->filter_options && suc->update_तथ्य->filter_options->choice)
		strvec_pushf(&child->args, "--filter=%s",
			     expand_list_objects_filter_spec(suc->update_तथ्य->filter_options));
	अगर (suc->update_तथ्य->require_init)
		strvec_धकेलो(&child->args, "--require-init");
	अगर (suc->update_तथ्य->ref_storage_format != REF_STORAGE_FORMAT_UNKNOWN)
		strvec_pushf(&child->args, "--ref-storage-format=%s",
			     ref_storage_format_to_name(suc->update_तथ्य->ref_storage_format));
	strvec_pushl(&child->args, "--path", sub->path, शून्य);
	strvec_pushl(&child->args, "--name", sub->name, शून्य);
	strvec_pushl(&child->args, "--url", url, शून्य);
	अगर (suc->update_तथ्य->references.nr) {
		काष्ठा string_list_item *item;

		for_each_string_list_item(item, &suc->update_तथ्य->references)
			strvec_pushl(&child->args, "--reference", item->string, शून्य);
	}
	अगर (suc->update_तथ्य->dissociate)
		strvec_धकेलो(&child->args, "--dissociate");
	अगर (suc->update_तथ्य->single_branch >= 0)
		strvec_धकेलो(&child->args, suc->update_तथ्य->single_branch ?
					      "--single-branch" :
					      "--no-single-branch");

cleanup:
	मुक्त(displaypath);
	strbuf_release(&sb);
	अगर (need_free_url)
		मुक्त((व्योम*)url);

	वापस needs_cloning;
}

अटल पूर्णांक update_clone_get_अग्र_task(काष्ठा child_process *child,
				      काष्ठा strbuf *err,
				      व्योम *suc_cb,
				      व्योम **idx_task_cb)
{
	काष्ठा submodule_update_clone *suc = suc_cb;
	स्थिर काष्ठा cache_entry *ce;
	पूर्णांक सूचकांक;

	क्रम (; suc->current < suc->update_तथ्य->list.nr; suc->current++) {
		ce = suc->update_तथ्य->list.entries[suc->current];
		अगर (prepare_to_clone_अग्र_submodule(ce, child, suc, err)) {
			पूर्णांक *p = xmalloc(माप(*p));

			*p = suc->current;
			*idx_task_cb = p;
			suc->current++;
			वापस 1;
		}
	}

	/*
	 * The loop above tried cloning each submodule once, now try the
	 * stragglers again, which we can imagine as an extension of the
	 * entry list.
	 */
	सूचकांक = suc->current - suc->update_तथ्य->list.nr;
	अगर (सूचकांक < suc->failed_clones_nr) {
		पूर्णांक *p;

		ce = suc->failed_clones[सूचकांक];
		अगर (!prepare_to_clone_अग्र_submodule(ce, child, suc, err)) {
			suc->current ++;
			strbuf_addstr(err, "BUG: submodule considered for "
					   "cloning, doesn't need cloning "
					   "any more?\n");
			वापस 0;
		}
		p = xmalloc(माप(*p));
		*p = suc->current;
		*idx_task_cb = p;
		suc->current ++;
		वापस 1;
	}

	वापस 0;
}

अटल पूर्णांक update_clone_start_failure(काष्ठा strbuf *err UNUSED,
				      व्योम *suc_cb,
				      व्योम *idx_task_cb UNUSED)
{
	काष्ठा submodule_update_clone *suc = suc_cb;

	suc->quickstop = 1;
	वापस 1;
}

अटल पूर्णांक update_clone_task_finished(पूर्णांक परिणाम,
				      काष्ठा strbuf *err,
				      व्योम *suc_cb,
				      व्योम *idx_task_cb)
{
	स्थिर काष्ठा cache_entry *ce;
	काष्ठा submodule_update_clone *suc = suc_cb;
	पूर्णांक *idxP = idx_task_cb;
	पूर्णांक idx = *idxP;

	मुक्त(idxP);

	अगर (!परिणाम)
		वापस 0;

	अगर (idx < suc->update_तथ्य->list.nr) {
		ce  = suc->update_तथ्य->list.entries[idx];
		strbuf_addf(err, _("Failed to clone '%s'. Retry scheduled"),
			    ce->name);
		strbuf_addch(err, '\n');
		ALLOC_GROW(suc->failed_clones,
			   suc->failed_clones_nr + 1,
			   suc->failed_clones_alloc);
		suc->failed_clones[suc->failed_clones_nr++] = ce;
		वापस 0;
	} अन्यथा {
		idx -= suc->update_तथ्य->list.nr;
		ce  = suc->failed_clones[idx];
		strbuf_addf(err, _("Failed to clone '%s' a second time, aborting"),
			    ce->name);
		strbuf_addch(err, '\n');
		suc->quickstop = 1;
		वापस 1;
	}

	वापस 0;
}

अटल पूर्णांक git_update_clone_config(स्थिर अक्षर *var, स्थिर अक्षर *मूल्य,
				   स्थिर काष्ठा config_context *ctx,
				   व्योम *cb)
{
	पूर्णांक *max_jobs = cb;

	अगर (!म_भेद(var, "submodule.fetchjobs"))
		*max_jobs = parse_submodule_fetchjobs(var, मूल्य, ctx->kvi);
	वापस 0;
}

अटल पूर्णांक is_tip_reachable(स्थिर अक्षर *path, स्थिर काष्ठा object_id *oid)
{
	काष्ठा child_process cp = CHILD_PROCESS_INIT;
	काष्ठा strbuf rev = STRBUF_INIT;
	अक्षर *hex = oid_to_hex(oid);
	पूर्णांक reachable;

	cp.git_cmd = 1;
	cp.dir = path;
	cp.no_stderr = 1;
	strvec_pushl(&cp.args, "rev-list", "-n", "1", hex, "--not", "--all", शून्य);

	prepare_submodule_repo_env(&cp.env);

	अगर (capture_command(&cp, &rev, GIT_MAX_HEXSZ + 1) || rev.len)
		reachable = 0;
	अन्यथा
		reachable = 1;

	strbuf_release(&rev);
	वापस reachable;
}

अटल पूर्णांक fetch_in_submodule(स्थिर अक्षर *module_path, पूर्णांक depth, पूर्णांक quiet,
			      स्थिर काष्ठा object_id *oid)
{
	काष्ठा child_process cp = CHILD_PROCESS_INIT;

	prepare_submodule_repo_env(&cp.env);
	cp.git_cmd = 1;
	cp.dir = module_path;

	strvec_धकेलो(&cp.args, "fetch");
	अगर (quiet)
		strvec_धकेलो(&cp.args, "--quiet");
	अगर (depth)
		strvec_pushf(&cp.args, "--depth=%d", depth);
	अगर (oid) {
		अक्षर *hex = oid_to_hex(oid);
		अक्षर *remote;
		पूर्णांक code;

		code = get_default_remote_submodule(module_path, &remote);
		अगर (code) {
			child_process_clear(&cp);
			वापस code;
		}

		strvec_pushl(&cp.args, remote, hex, शून्य);
		मुक्त(remote);
	}

	वापस run_command(&cp);
}

अटल पूर्णांक run_update_command(स्थिर काष्ठा update_तथ्य *ud, पूर्णांक subforce)
{
	काष्ठा child_process cp = CHILD_PROCESS_INIT;
	अक्षर *oid = oid_to_hex(&ud->oid);
	पूर्णांक ret;

	चयन (ud->update_strategy.type) {
	हाल SM_UPDATE_CHECKOUT:
		cp.git_cmd = 1;
		strvec_pushl(&cp.args, "checkout", "-q", शून्य);
		अगर (subforce)
			strvec_धकेलो(&cp.args, "-f");
		अवरोध;
	हाल SM_UPDATE_REBASE:
		cp.git_cmd = 1;
		strvec_धकेलो(&cp.args, "rebase");
		अगर (ud->quiet)
			strvec_धकेलो(&cp.args, "--quiet");
		अवरोध;
	हाल SM_UPDATE_MERGE:
		cp.git_cmd = 1;
		strvec_धकेलो(&cp.args, "merge");
		अगर (ud->quiet)
			strvec_धकेलो(&cp.args, "--quiet");
		अवरोध;
	हाल SM_UPDATE_COMMAND:
		cp.use_shell = 1;
		strvec_धकेलो(&cp.args, ud->update_strategy.command);
		अवरोध;
	शेष:
		BUG("unexpected update strategy type: %d",
		    ud->update_strategy.type);
	}
	strvec_धकेलो(&cp.args, oid);

	cp.dir = ud->sm_path;
	prepare_submodule_repo_env(&cp.env);
	अगर ((ret = run_command(&cp))) {
		चयन (ud->update_strategy.type) {
		हाल SM_UPDATE_CHECKOUT:
			die_message(_("Unable to checkout '%s' in submodule path '%s'"),
				    oid, ud->displaypath);
			/* No "ret" assignment, use "git checkout"'s */
			अवरोध;
		हाल SM_UPDATE_REBASE:
			ret = die_message(_("Unable to rebase '%s' in submodule path '%s'"),
					  oid, ud->displaypath);
			अवरोध;
		हाल SM_UPDATE_MERGE:
			ret = die_message(_("Unable to merge '%s' in submodule path '%s'"),
					  oid, ud->displaypath);
			अवरोध;
		हाल SM_UPDATE_COMMAND:
			ret = die_message(_("Execution of '%s %s' failed in submodule path '%s'"),
					  ud->update_strategy.command, oid, ud->displaypath);
			अवरोध;
		शेष:
			BUG("unexpected update strategy type: %d",
			    ud->update_strategy.type);
		}

		वापस ret;
	}

	अगर (ud->quiet)
		वापस 0;

	चयन (ud->update_strategy.type) {
	हाल SM_UPDATE_CHECKOUT:
		म_लिखो(_("Submodule path '%s': checked out '%s'\n"),
		       ud->displaypath, oid);
		अवरोध;
	हाल SM_UPDATE_REBASE:
		म_लिखो(_("Submodule path '%s': rebased into '%s'\n"),
		       ud->displaypath, oid);
		अवरोध;
	हाल SM_UPDATE_MERGE:
		म_लिखो(_("Submodule path '%s': merged in '%s'\n"),
		       ud->displaypath, oid);
		अवरोध;
	हाल SM_UPDATE_COMMAND:
		म_लिखो(_("Submodule path '%s': '%s %s'\n"),
		       ud->displaypath, ud->update_strategy.command, oid);
		अवरोध;
	शेष:
		BUG("unexpected update strategy type: %d",
		    ud->update_strategy.type);
	}

	वापस 0;
}

अटल पूर्णांक run_update_procedure(स्थिर काष्ठा update_तथ्य *ud)
{
	पूर्णांक subforce = is_null_oid(&ud->suboid) || ud->force;

	अगर (!ud->nofetch) {
		/*
		 * Run fetch only if `oid` isn't present or it
		 * is not reachable from a ref.
		 */
		अगर (!is_tip_reachable(ud->sm_path, &ud->oid) &&
		    fetch_in_submodule(ud->sm_path, ud->depth, ud->quiet, शून्य) &&
		    !ud->quiet)
			fprintf_ln(मानक_त्रुटि,
				   _("Unable to fetch in submodule path '%s'; "
				     "trying to directly fetch %s:"),
				   ud->displaypath, oid_to_hex(&ud->oid));
		/*
		 * Now we tried the usual fetch, but `oid` may
		 * not be reachable from any of the refs.
		 */
		अगर (!is_tip_reachable(ud->sm_path, &ud->oid) &&
		    fetch_in_submodule(ud->sm_path, ud->depth, ud->quiet, &ud->oid))
			वापस die_message(_("Fetched in submodule path '%s', but it did not "
					     "contain %s. Direct fetching of that commit failed."),
					   ud->displaypath, oid_to_hex(&ud->oid));
	}

	वापस run_update_command(ud, subforce);
}

अटल पूर्णांक remote_submodule_branch(स्थिर अक्षर *path, स्थिर अक्षर **branch)
{
	स्थिर काष्ठा submodule *sub;
	अक्षर *कुंजी;
	*branch = शून्य;

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);
	अगर (!sub)
		वापस die_message(_("could not initialize submodule at path '%s'"),
				   path);

	कुंजी = xstrfmt("submodule.%s.branch", sub->name);
	अगर (repo_config_get_string_tmp(the_repository, कुंजी, branch))
		*branch = sub->branch;
	मुक्त(कुंजी);

	अगर (!*branch) {
		*branch = "HEAD";
		वापस 0;
	}

	अगर (!म_भेद(*branch, ".")) {
		स्थिर अक्षर *refname = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
							      "HEAD", 0, शून्य,
							      शून्य);

		अगर (!refname)
			वापस die_message(_("No such ref: %s"), "HEAD");

		/* detached HEAD */
		अगर (!म_भेद(refname, "HEAD"))
			वापस die_message(_("Submodule (%s) branch configured to inherit "
					     "branch from superproject, but the superproject "
					     "is not on any branch"), sub->name);

		अगर (!skip_prefix(refname, "refs/heads/", &refname))
			वापस die_message(_("Expecting a full ref name, got %s"),
					   refname);

		*branch = refname;
		वापस 0;
	}

	/* Our "branch" is coming from repo_config_get_string_tmp() */
	वापस 0;
}

अटल पूर्णांक ensure_core_worktree(स्थिर अक्षर *path)
{
	स्थिर अक्षर *cw;
	काष्ठा repository subrepo;

	अगर (repo_submodule_init(&subrepo, the_repository, path, null_oid(the_hash_algo)))
		वापस die_message(_("could not get a repository handle for submodule '%s'"),
				   path);

	अगर (!repo_config_get_string_tmp(&subrepo, "core.worktree", &cw)) {
		अक्षर *cfg_file, *abs_path;
		स्थिर अक्षर *rel_path;
		काष्ठा strbuf sb = STRBUF_INIT;

		cfg_file = repo_git_path(&subrepo, "config");

		abs_path = absolute_pathdup(path);
		rel_path = relative_path(abs_path, subrepo.gitdir, &sb);

		repo_config_set_in_file(the_repository, cfg_file, "core.worktree", rel_path);

		मुक्त(cfg_file);
		मुक्त(abs_path);
		strbuf_release(&sb);
	}

	repo_clear(&subrepo);
	वापस 0;
}

अटल स्थिर अक्षर *submodule_update_type_to_label(क्रमागत submodule_update_type type)
{
	चयन (type) {
	हाल SM_UPDATE_CHECKOUT:
		वापस "checkout";
	हाल SM_UPDATE_MERGE:
		वापस "merge";
	हाल SM_UPDATE_REBASE:
		वापस "rebase";
	हाल SM_UPDATE_UNSPECIFIED:
	हाल SM_UPDATE_NONE:
	हाल SM_UPDATE_COMMAND:
		अवरोध;
	}
	BUG("unreachable with type %d", type);
}

अटल व्योम update_तथ्य_to_args(स्थिर काष्ठा update_तथ्य *update_तथ्य,
				काष्ठा strvec *args)
{
	क्रमागत submodule_update_type update_type = update_तथ्य->update_default;

	strvec_pushl(args, "submodule--helper", "update", "--recursive", शून्य);
	अगर (update_तथ्य->displaypath)
		strvec_pushf(args, "--super-prefix=%s/",
			     update_तथ्य->displaypath);
	strvec_pushf(args, "--jobs=%d", update_तथ्य->max_jobs);
	अगर (update_तथ्य->quiet)
		strvec_धकेलो(args, "--quiet");
	अगर (update_तथ्य->force)
		strvec_धकेलो(args, "--force");
	अगर (update_तथ्य->init)
		strvec_धकेलो(args, "--init");
	अगर (update_तथ्य->remote)
		strvec_धकेलो(args, "--remote");
	अगर (update_तथ्य->nofetch)
		strvec_धकेलो(args, "--no-fetch");
	अगर (update_तथ्य->dissociate)
		strvec_धकेलो(args, "--dissociate");
	अगर (update_तथ्य->progress)
		strvec_धकेलो(args, "--progress");
	अगर (update_तथ्य->require_init)
		strvec_धकेलो(args, "--require-init");
	अगर (update_तथ्य->depth)
		strvec_pushf(args, "--depth=%d", update_तथ्य->depth);
	अगर (update_type != SM_UPDATE_UNSPECIFIED)
		strvec_pushf(args, "--%s",
			     submodule_update_type_to_label(update_type));

	अगर (update_तथ्य->references.nr) {
		काष्ठा string_list_item *item;

		for_each_string_list_item(item, &update_तथ्य->references)
			strvec_pushl(args, "--reference", item->string, शून्य);
	}
	अगर (update_तथ्य->ref_storage_format != REF_STORAGE_FORMAT_UNKNOWN)
		strvec_pushf(args, "--ref-storage-format=%s",
			     ref_storage_format_to_name(update_तथ्य->ref_storage_format));
	अगर (update_तथ्य->filter_options && update_तथ्य->filter_options->choice)
		strvec_pushf(args, "--filter=%s",
				expand_list_objects_filter_spec(
					update_तथ्य->filter_options));
	अगर (update_तथ्य->recommend_shallow == 0)
		strvec_धकेलो(args, "--no-recommend-shallow");
	अन्यथा अगर (update_तथ्य->recommend_shallow == 1)
		strvec_धकेलो(args, "--recommend-shallow");
	अगर (update_तथ्य->single_branch >= 0)
		strvec_धकेलो(args, update_तथ्य->single_branch ?
				    "--single-branch" :
				    "--no-single-branch");
}

अटल पूर्णांक update_submodule(काष्ठा update_तथ्य *update_तथ्य)
{
	पूर्णांक ret;

	अगर (validate_submodule_path(update_तथ्य->sm_path) < 0)
		वापस -1;

	ret = determine_submodule_update_strategy(the_repository,
						  update_तथ्य->just_cloned,
						  update_तथ्य->sm_path,
						  update_तथ्य->update_default,
						  &update_तथ्य->update_strategy);
	अगर (ret)
		वापस ret;

	अगर (update_तथ्य->just_cloned)
		oidcpy(&update_तथ्य->suboid, null_oid(the_hash_algo));
	अन्यथा अगर (repo_resolve_gitlink_ref(the_repository, update_तथ्य->sm_path,
					  "HEAD", &update_तथ्य->suboid))
		वापस die_message(_("Unable to find current revision in submodule path '%s'"),
				   update_तथ्य->displaypath);

	अगर (update_तथ्य->remote) {
		अक्षर *remote_name;
		स्थिर अक्षर *branch;
		अक्षर *remote_ref;
		पूर्णांक code;

		code = get_default_remote_submodule(update_तथ्य->sm_path, &remote_name);
		अगर (code)
			वापस code;
		code = remote_submodule_branch(update_तथ्य->sm_path, &branch);
		अगर (code) {
			मुक्त(remote_name);
			वापस code;
		}
		remote_ref = xstrfmt("refs/remotes/%s/%s", remote_name, branch);

		मुक्त(remote_name);

		अगर (!update_तथ्य->nofetch) {
			अगर (fetch_in_submodule(update_तथ्य->sm_path, update_तथ्य->depth,
					      0, शून्य)) {
				मुक्त(remote_ref);
				वापस die_message(_("Unable to fetch in submodule path '%s'"),
						   update_तथ्य->sm_path);
			}
		}

		अगर (repo_resolve_gitlink_ref(the_repository, update_तथ्य->sm_path,
					     remote_ref, &update_तथ्य->oid)) {
			ret = die_message(_("Unable to find %s revision in submodule path '%s'"),
					  remote_ref, update_तथ्य->sm_path);
			मुक्त(remote_ref);
			वापस ret;
		}

		मुक्त(remote_ref);
	}

	अगर (!oideq(&update_तथ्य->oid, &update_तथ्य->suboid) || update_तथ्य->force) {
		ret = run_update_procedure(update_तथ्य);
		अगर (ret)
			वापस ret;
	}

	अगर (update_तथ्य->recursive) {
		काष्ठा child_process cp = CHILD_PROCESS_INIT;
		काष्ठा update_तथ्य अग्र = *update_तथ्य;

		अग्र.prefix = शून्य;
		oidcpy(&अग्र.oid, null_oid(the_hash_algo));
		oidcpy(&अग्र.suboid, null_oid(the_hash_algo));

		cp.dir = update_तथ्य->sm_path;
		cp.git_cmd = 1;
		prepare_submodule_repo_env(&cp.env);
		update_तथ्य_to_args(&अग्र, &cp.args);

		ret = run_command(&cp);
		अगर (ret)
			die_message(_("Failed to recurse into submodule path '%s'"),
				    update_तथ्य->displaypath);
		वापस ret;
	}

	वापस 0;
}

अटल पूर्णांक update_submodules(काष्ठा update_तथ्य *update_तथ्य)
{
	पूर्णांक i, ret = 0;
	काष्ठा submodule_update_clone suc = SUBMODULE_UPDATE_CLONE_INIT;
	स्थिर काष्ठा run_process_parallel_opts opts = {
		.tr2_category = "submodule",
		.tr2_label = "parallel/update",

		.processes = update_तथ्य->max_jobs,

		.get_अग्र_task = update_clone_get_अग्र_task,
		.start_failure = update_clone_start_failure,
		.task_finished = update_clone_task_finished,
		.तथ्य = &suc,
	};

	suc.update_तथ्य = update_तथ्य;
	run_processes_parallel(&opts);

	/*
	 * We saved the output and put it out all at once now.
	 * That means:
	 * - the listener does not have to interleave their (checkout)
	 *   work with our fetching.  The writes involved in a
	 *   checkout involve more straightforward sequential I/O.
	 * - the listener can avoid doing any work if fetching failed.
	 */
	अगर (suc.quickstop) {
		ret = 1;
		जाओ cleanup;
	}

	क्रम (i = 0; i < suc.update_clone_nr; i++) {
		काष्ठा update_clone_तथ्य ucd = suc.update_clone[i];
		पूर्णांक code = 128;

		oidcpy(&update_तथ्य->oid, &ucd.oid);
		update_तथ्य->just_cloned = ucd.just_cloned;
		update_तथ्य->sm_path = ucd.sub->path;

		/*
		 * Verify that the submodule path does not contain any
		 * symlinks; if it does, it might have been tampered with.
		 * TODO: allow exempting it via
		 * `safe.submodule.path` or something
		 */
		अगर (validate_submodule_path(update_तथ्य->sm_path) < 0)
			जाओ fail;

		code = ensure_core_worktree(update_तथ्य->sm_path);
		अगर (code)
			जाओ fail;

		update_तथ्य->displaypath = get_submodule_displaypath(
			update_तथ्य->sm_path, update_तथ्य->prefix,
			update_तथ्य->super_prefix);
		code = update_submodule(update_तथ्य);
		FREE_AND_NULL(update_तथ्य->displaypath);
fail:
		अगर (!code)
			जारी;
		ret = code;
		अगर (ret == 128)
			जाओ cleanup;
	}

cleanup:
	submodule_update_clone_release(&suc);
	string_list_clear(&update_तथ्य->references, 0);
	वापस ret;
}

अटल पूर्णांक module_update(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
			 काष्ठा repository *repo UNUSED)
{
	काष्ठा pathspec pathspec = { 0 };
	काष्ठा pathspec pathspec2 = { 0 };
	काष्ठा update_तथ्य opt = UPDATE_तथ्य_INIT;
	काष्ठा list_objects_filter_options filter_options =
		LIST_OBJECTS_FILTER_INIT;
	स्थिर अक्षर *ref_storage_format = शून्य;
	पूर्णांक ret;
	काष्ठा option module_update_options[] = {
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
	स्थिर अक्षर *स्थिर git_submodule_helper_usage[] = {
		N_("git submodule [--quiet] update"
		" [--init [--filter=<filter-spec>]] [--remote]"
		" [-N|--no-fetch] [-f|--force]"
		" [--checkout|--merge|--rebase]"
		" [--[no-]recommend-shallow] [--reference <repository>]"
		" [--recursive] [--[no-]single-branch] [--] [<path>...]"),
		शून्य
	};

	update_clone_config_from_gitmodules(the_repository, &opt.max_jobs);
	repo_config(the_repository, git_update_clone_config, &opt.max_jobs);

	argc = parse_options(argc, argv, prefix, module_update_options,
			     git_submodule_helper_usage, 0);

	अगर (opt.require_init)
		opt.init = 1;

	अगर (filter_options.choice && !opt.init) {
		usage_with_options(git_submodule_helper_usage,
				   module_update_options);
	}

	अगर (ref_storage_format) {
		opt.ref_storage_format = ref_storage_format_by_name(ref_storage_format);
		अगर (opt.ref_storage_format == REF_STORAGE_FORMAT_UNKNOWN)
			die(_("unknown ref storage format '%s'"), ref_storage_format);
	}

	opt.filter_options = &filter_options;
	opt.prefix = prefix;

	अगर (opt.update_default)
		opt.update_strategy.type = opt.update_default;

	अगर (module_list_compute(argv, prefix, &pathspec, &opt.list) < 0) {
		ret = 1;
		जाओ cleanup;
	}

	अगर (pathspec.nr)
		opt.warn_if_uninitialized = 1;

	अगर (opt.init) {
		काष्ठा module_list list = MODULE_LIST_INIT;
		काष्ठा init_cb info = INIT_CB_INIT;

		अगर (module_list_compute(argv, opt.prefix,
					&pathspec2, &list) < 0) {
			module_list_release(&list);
			ret = 1;
			जाओ cleanup;
		}

		/*
		 * If there are no path args and submodule.active is set then,
		 * by default, only initialize 'active' modules.
		 */
		अगर (!argc && !repo_config_get(the_repository, "submodule.active"))
			module_list_active(&list);

		info.prefix = opt.prefix;
		info.super_prefix = opt.super_prefix;
		अगर (opt.quiet)
			info.flags |= OPT_QUIET;

		for_each_listed_submodule(&list, init_submodule_cb, &info);
		module_list_release(&list);
	}

	ret = update_submodules(&opt);
cleanup:
	update_तथ्य_release(&opt);
	list_objects_filter_release(&filter_options);
	clear_pathspec(&pathspec);
	clear_pathspec(&pathspec2);
	वापस ret;
}

अटल पूर्णांक धकेलो_check(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix UNUSED,
		      काष्ठा repository *repo UNUSED)
{
	काष्ठा remote *remote;
	स्थिर अक्षर *superproject_शीर्ष;
	अक्षर *शीर्ष;
	पूर्णांक detached_शीर्ष = 0;
	काष्ठा object_id शीर्ष_oid;

	अगर (argc < 3)
		die("submodule--helper push-check requires at least 2 arguments");

	/*
	 * superproject's resolved head ref.
	 * if HEAD then the superproject is in a detached head state, otherwise
	 * it will be the resolved head ref.
	 */
	superproject_शीर्ष = argv[1];
	argv++;
	argc--;
	/* Get the submodule's head ref and determine if it is detached */
	शीर्ष = refs_resolve_refdup(get_main_ref_store(the_repository), "HEAD",
				   0, &शीर्ष_oid, शून्य);
	अगर (!शीर्ष)
		die(_("Failed to resolve HEAD as a valid ref."));
	अगर (!म_भेद(शीर्ष, "HEAD"))
		detached_शीर्ष = 1;

	/*
	 * The remote must be configured.
	 * This is to avoid pushing to the exact same URL as the parent.
	 */
	remote = pushremote_get(argv[1]);
	अगर (!remote || remote->origin == REMOTE_UNCONFIGURED)
		die("remote '%s' not configured", argv[1]);

	/* Check the refspec */
	अगर (argc > 2) {
		पूर्णांक i;
		काष्ठा ref *local_refs = get_local_heads();
		काष्ठा refspec refspec = REFSPEC_INIT_धकेलो(the_hash_algo);

		refspec_appendn(&refspec, argv + 2, argc - 2);

		क्रम (i = 0; i < refspec.nr; i++) {
			स्थिर काष्ठा refspec_item *rs = &refspec.वस्तुएँ[i];

			अगर (rs->pattern || rs->matching)
				जारी;

			/* LHS must match a single ref */
			चयन (गिनती_refspec_match(rs->src, local_refs, शून्य)) {
			हाल 1:
				अवरोध;
			हाल 0:
				/*
				 * If LHS matches 'HEAD' then we need to ensure
				 * that it matches the same named branch
				 * checked out in the superproject.
				 */
				अगर (!म_भेद(rs->src, "HEAD")) {
					अगर (!detached_शीर्ष &&
					    !म_भेद(शीर्ष, superproject_शीर्ष))
						अवरोध;
					die("HEAD does not match the named branch in the superproject");
				}
				/* fallthrough */
			शेष:
				die("src refspec '%s' must name a ref",
				    rs->src);
			}
		}

		refspec_clear(&refspec);
		free_refs(local_refs);
	}
	मुक्त(शीर्ष);

	वापस 0;
}

अटल पूर्णांक absorb_git_dirs(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
			   काष्ठा repository *repo UNUSED)
{
	पूर्णांक i;
	काष्ठा pathspec pathspec = { 0 };
	काष्ठा module_list list = MODULE_LIST_INIT;
	स्थिर अक्षर *super_prefix = शून्य;
	काष्ठा option embed_gitdir_options[] = {
		OPT__SUPER_PREFIX(&super_prefix),
		OPT_END()
	};
	स्थिर अक्षर *स्थिर git_submodule_helper_usage[] = {
		N_("git submodule absorbgitdirs [<options>] [<path>...]"),
		शून्य
	};
	पूर्णांक ret = 1;

	argc = parse_options(argc, argv, prefix, embed_gitdir_options,
			     git_submodule_helper_usage, 0);

	अगर (module_list_compute(argv, prefix, &pathspec, &list) < 0)
		जाओ cleanup;

	क्रम (i = 0; i < list.nr; i++)
		absorb_git_dir_into_superproject(list.entries[i]->name,
						 super_prefix);

	ret = 0;
cleanup:
	clear_pathspec(&pathspec);
	module_list_release(&list);
	वापस ret;
}

अटल पूर्णांक module_set_url(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
			  काष्ठा repository *repo UNUSED)
{
	पूर्णांक quiet = 0, ret;
	स्थिर अक्षर *newurl;
	स्थिर अक्षर *path;
	अक्षर *config_name;
	काष्ठा option options[] = {
		OPT__QUIET(&quiet, N_("suppress output for setting url of a submodule")),
		OPT_END()
	};
	स्थिर अक्षर *स्थिर usage[] = {
		N_("git submodule set-url [--quiet] <path> <newurl>"),
		शून्य
	};
	स्थिर काष्ठा submodule *sub;

	argc = parse_options(argc, argv, prefix, options, usage, 0);

	अगर (argc != 2 || !(path = argv[0]) || !(newurl = argv[1]))
		usage_with_options(usage, options);

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);

	अगर (!sub)
		die(_("no submodule mapping found in .gitmodules for path '%s'"),
		    path);

	config_name = xstrfmt("submodule.%s.url", sub->name);
	ret = config_set_in_gitmodules_file_gently(the_repository, config_name, newurl);

	अगर (!ret) {
		repo_read_gitmodules(the_repository, 0);
		sync_submodule(sub->path, prefix, शून्य, quiet ? OPT_QUIET : 0);
	}

	मुक्त(config_name);
	वापस !!ret;
}

अटल पूर्णांक module_set_branch(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
			     काष्ठा repository *repo UNUSED)
{
	पूर्णांक opt_default = 0, ret;
	स्थिर अक्षर *opt_branch = शून्य;
	स्थिर अक्षर *path;
	अक्षर *config_name;
	काष्ठा option options[] = {
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
	स्थिर अक्षर *स्थिर usage[] = {
		N_("git submodule set-branch [-q|--quiet] (-d|--default) <path>"),
		N_("git submodule set-branch [-q|--quiet] (-b|--branch) <branch> <path>"),
		शून्य
	};
	स्थिर काष्ठा submodule *sub;

	argc = parse_options(argc, argv, prefix, options, usage, 0);

	अगर (!opt_branch && !opt_default)
		die(_("--branch or --default required"));

	अगर (opt_branch && opt_default)
		die(_("options '%s' and '%s' cannot be used together"), "--branch", "--default");

	अगर (argc != 1 || !(path = argv[0]))
		usage_with_options(usage, options);

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);

	अगर (!sub)
		die(_("no submodule mapping found in .gitmodules for path '%s'"),
		    path);

	config_name = xstrfmt("submodule.%s.branch", sub->name);
	ret = config_set_in_gitmodules_file_gently(the_repository, config_name, opt_branch);

	मुक्त(config_name);
	वापस !!ret;
}

अटल पूर्णांक module_create_branch(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
				काष्ठा repository *repo UNUSED)
{
	क्रमागत branch_track track;
	पूर्णांक quiet = 0, force = 0, reflog = 0, dry_run = 0;
	काष्ठा option options[] = {
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
	स्थिर अक्षर *स्थिर usage[] = {
		N_("git submodule--helper create-branch [-f|--force] [--create-reflog] [-q|--quiet] [-t|--track] [-n|--dry-run] <name> <start-oid> <start-name>"),
		शून्य
	};
	काष्ठा repo_config_मूल्यावली *cfg = repo_config_मूल्यावली(the_repository);

	repo_config(the_repository, git_default_config, शून्य);
	track = cfg->branch_track;
	argc = parse_options(argc, argv, prefix, options, usage, 0);

	अगर (argc != 3)
		usage_with_options(usage, options);

	अगर (!quiet && !dry_run)
		printf_ln(_("creating branch '%s'"), argv[0]);

	create_branches_recursively(the_repository, argv[0], argv[1], argv[2],
				    force, reflog, quiet, track, dry_run);
	वापस 0;
}

काष्ठा add_तथ्य {
	स्थिर अक्षर *prefix;
	स्थिर अक्षर *branch;
	स्थिर अक्षर *reference_path;
	अक्षर *sm_path;
	स्थिर अक्षर *sm_name;
	स्थिर अक्षर *repo;
	स्थिर अक्षर *realrepo;
	क्रमागत ref_storage_format ref_storage_format;
	पूर्णांक depth;
	अचिन्हित पूर्णांक force: 1;
	अचिन्हित पूर्णांक quiet: 1;
	अचिन्हित पूर्णांक progress: 1;
	अचिन्हित पूर्णांक dissociate: 1;
};
#घोषणा ADD_तथ्य_INIT { \
	.depth = -1, \
	.ref_storage_format = REF_STORAGE_FORMAT_UNKNOWN, \
}

अटल व्योम append_fetch_remotes(काष्ठा strbuf *संदेश, स्थिर अक्षर *git_dir_path)
{
	काष्ठा child_process cp_remote = CHILD_PROCESS_INIT;
	काष्ठा strbuf sb_remote_out = STRBUF_INIT;

	cp_remote.git_cmd = 1;
	strvec_pushf(&cp_remote.env,
		     "GIT_DIR=%s", git_dir_path);
	strvec_धकेलो(&cp_remote.env, "GIT_WORK_TREE=.");
	strvec_pushl(&cp_remote.args, "remote", "-v", शून्य);
	अगर (!capture_command(&cp_remote, &sb_remote_out, 0)) {
		अक्षर *अग्र_पंक्ति;
		अक्षर *पंक्ति = sb_remote_out.buf;

		जबतक ((अग्र_पंक्ति = म_अक्षर(पंक्ति, '\n')) != शून्य) {
			माप_प्रकार len = अग्र_पंक्ति - पंक्ति;

			अगर (strip_suffix_mem(पंक्ति, &len, " (fetch)"))
				strbuf_addf(संदेश, "  %.*s\n", (पूर्णांक)len, पंक्ति);
			पंक्ति = अग्र_पंक्ति + 1;
		}
	}

	strbuf_release(&sb_remote_out);
}

अटल पूर्णांक add_submodule(स्थिर काष्ठा add_तथ्य *add_तथ्य)
{
	काष्ठा module_clone_तथ्य clone_तथ्य = MODULE_CLONE_तथ्य_INIT;
	काष्ठा string_list reference = STRING_LIST_INIT_NODUP;
	पूर्णांक ret = -1;

	/* perhaps the path already exists and is already a git repo, else clone it */
	अगर (is_directory(add_तथ्य->sm_path)) {
		अक्षर *submod_gitdir_path;
		काष्ठा strbuf sm_path = STRBUF_INIT;
		strbuf_addstr(&sm_path, add_तथ्य->sm_path);
		submod_gitdir_path = xstrfmt("%s/.git", add_तथ्य->sm_path);
		अगर (is_nonbare_repository_dir(&sm_path))
			म_लिखो(_("Adding existing repo at '%s' to the index\n"),
			       add_तथ्य->sm_path);
		अन्यथा
			die(_("'%s' already exists and is not a valid git repo"),
			    add_तथ्य->sm_path);
		strbuf_release(&sm_path);
		मुक्त(submod_gitdir_path);
	} अन्यथा {
		काष्ठा child_process cp = CHILD_PROCESS_INIT;
		काष्ठा strbuf submod_gitdir = STRBUF_INIT;

		submodule_name_to_gitdir(&submod_gitdir, the_repository, add_तथ्य->sm_name);

		अगर (is_directory(submod_gitdir.buf)) {
			अगर (!add_तथ्य->force) {
				काष्ठा strbuf संदेश = STRBUF_INIT;
				अक्षर *die_संदेश;

				strbuf_addf(&संदेश, _("A git directory for '%s' is found "
						    "locally with remote(s):\n"),
					    add_तथ्य->sm_name);

				append_fetch_remotes(&संदेश, submod_gitdir.buf);
				strbuf_release(&submod_gitdir);

				strbuf_addf(&संदेश, _("If you want to reuse this local git "
						    "directory instead of cloning again from\n"
						    "  %s\n"
						    "use the '--force' option. If the local git "
						    "directory is not the correct repo\n"
						    "or you are unsure what this means choose "
						    "another name with the '--name' option."),
					    add_तथ्य->realrepo);

				die_संदेश = strbuf_detach(&संदेश, शून्य);
				die("%s", die_संदेश);
			} अन्यथा {
				म_लिखो(_("Reactivating local git directory for "
					 "submodule '%s'\n"), add_तथ्य->sm_name);
			}
		}
		strbuf_release(&submod_gitdir);

		clone_तथ्य.prefix = add_तथ्य->prefix;
		clone_तथ्य.path = add_तथ्य->sm_path;
		clone_तथ्य.name = add_तथ्य->sm_name;
		clone_तथ्य.url = add_तथ्य->realrepo;
		clone_तथ्य.quiet = add_तथ्य->quiet;
		clone_तथ्य.progress = add_तथ्य->progress;
		अगर (add_तथ्य->reference_path) {
			अक्षर *p = xstrdup(add_तथ्य->reference_path);

			string_list_append(&reference, p)->util = p;
		}
		clone_तथ्य.ref_storage_format = add_तथ्य->ref_storage_format;
		clone_तथ्य.dissociate = add_तथ्य->dissociate;
		अगर (add_तथ्य->depth >= 0)
			clone_तथ्य.depth = add_तथ्य->depth;

		अगर (clone_submodule(&clone_तथ्य, &reference))
			जाओ cleanup;

		prepare_submodule_repo_env(&cp.env);
		cp.git_cmd = 1;
		cp.dir = add_तथ्य->sm_path;
		/*
		 * NOTE: we only get here if add_data->force is true, so
		 * passing --force to checkout is reasonable.
		 */
		strvec_pushl(&cp.args, "checkout", "-f", "-q", शून्य);

		अगर (add_तथ्य->branch) {
			strvec_pushl(&cp.args, "-B", add_तथ्य->branch, शून्य);
			strvec_pushf(&cp.args, "origin/%s", add_तथ्य->branch);
		}

		अगर (run_command(&cp))
			die(_("unable to checkout submodule '%s'"), add_तथ्य->sm_path);
	}
	ret = 0;

cleanup:
	string_list_clear(&reference, 1);
	वापस ret;
}

अटल पूर्णांक config_submodule_in_gitmodules(स्थिर अक्षर *name, स्थिर अक्षर *var, स्थिर अक्षर *मूल्य)
{
	अक्षर *कुंजी;
	पूर्णांक ret;

	अगर (!is_writing_gitmodules_ok())
		die(_("please make sure that the .gitmodules file is in the working tree"));

	कुंजी = xstrfmt("submodule.%s.%s", name, var);
	ret = config_set_in_gitmodules_file_gently(the_repository, कुंजी, मूल्य);
	मुक्त(कुंजी);

	वापस ret;
}

अटल व्योम configure_added_submodule(काष्ठा add_तथ्य *add_तथ्य)
{
	अक्षर *कुंजी;
	काष्ठा child_process add_submod = CHILD_PROCESS_INIT;
	काष्ठा child_process add_gitmodules = CHILD_PROCESS_INIT;
	स्थिर काष्ठा string_list *मूल्यावली;
	पूर्णांक matched = 0;

	कुंजी = xstrfmt("submodule.%s.url", add_तथ्य->sm_name);
	repo_config_set_gently(the_repository, कुंजी, add_तथ्य->realrepo);
	मुक्त(कुंजी);

	add_submod.git_cmd = 1;
	strvec_pushl(&add_submod.args, "add",
		     "--no-warn-embedded-repo", शून्य);
	अगर (add_तथ्य->force)
		strvec_धकेलो(&add_submod.args, "--force");
	strvec_pushl(&add_submod.args, "--", add_तथ्य->sm_path, शून्य);

	अगर (run_command(&add_submod))
		die(_("Failed to add submodule '%s'"), add_तथ्य->sm_path);

	अगर (config_submodule_in_gitmodules(add_तथ्य->sm_name, "path", add_तथ्य->sm_path) ||
	    config_submodule_in_gitmodules(add_तथ्य->sm_name, "url", add_तथ्य->repo))
		die(_("Failed to register submodule '%s'"), add_तथ्य->sm_path);

	अगर (add_तथ्य->branch) {
		अगर (config_submodule_in_gitmodules(add_तथ्य->sm_name,
						   "branch", add_तथ्य->branch))
			die(_("Failed to register submodule '%s'"), add_तथ्य->sm_path);
	}

	add_gitmodules.git_cmd = 1;
	strvec_pushl(&add_gitmodules.args,
		     "add", "--force", "--", ".gitmodules", शून्य);

	अगर (run_command(&add_gitmodules))
		die(_("Failed to register submodule '%s'"), add_तथ्य->sm_path);

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
	अगर (repo_config_get(the_repository, "submodule.active") || /* key absent */
	    repo_config_get_string_multi(the_repository, "submodule.active", &मूल्यावली)) {
		/*
		 * If the submodule being added isn't already covered by the
		 * current configured pathspec, set the submodule's active flag
		 */
		कुंजी = xstrfmt("submodule.%s.active", add_तथ्य->sm_name);
		repo_config_set_gently(the_repository, कुंजी, "true");
		मुक्त(कुंजी);
	} अन्यथा {
		क्रम (माप_प्रकार i = 0; i < मूल्यावली->nr; i++) {
			स्थिर अक्षर *pat = मूल्यावली->वस्तुएँ[i].string;
			अगर (!wildmatch(pat, add_तथ्य->sm_path, 0)) { /* match found */
				matched = 1;
				अवरोध;
			}
		}
		अगर (!matched) { /* no pattern matched -> force-enable */
			कुंजी = xstrfmt("submodule.%s.active", add_तथ्य->sm_name);
			repo_config_set_gently(the_repository, कुंजी, "true");
			मुक्त(कुंजी);
		}
	}
}

अटल व्योम die_on_सूचकांक_match(स्थिर अक्षर *path, पूर्णांक force)
{
	काष्ठा pathspec ps;
	स्थिर अक्षर *args[] = { path, शून्य };
	parse_pathspec(&ps, 0, PATHSPEC_PREFER_CWD, शून्य, args);

	अगर (repo_read_सूचकांक_preload(the_repository, शून्य, 0) < 0)
		die(_("index file corrupt"));

	अगर (ps.nr) {
		अक्षर *ps_matched = xcalloc(ps.nr, 1);

		/* TODO: audit for interaction with sparse-index. */
		ensure_full_सूचकांक(the_repository->सूचकांक);

		/*
		 * Since there is only one pathspec, we just need to
		 * check ps_matched[0] to know if a cache entry matched.
		 */
		क्रम (माप_प्रकार i = 0; i < the_repository->सूचकांक->cache_nr; i++) {
			ce_path_match(the_repository->सूचकांक, the_repository->सूचकांक->cache[i], &ps,
				      ps_matched);

			अगर (ps_matched[0]) {
				अगर (!force)
					die(_("'%s' already exists in the index"),
					    path);
				अगर (!S_ISGITLINK(the_repository->सूचकांक->cache[i]->ce_mode))
					die(_("'%s' already exists in the index "
					      "and is not a submodule"), path);
				अवरोध;
			}
		}
		मुक्त(ps_matched);
	}
	clear_pathspec(&ps);
}

अटल व्योम die_on_repo_without_commits(स्थिर अक्षर *path)
{
	काष्ठा strbuf sb = STRBUF_INIT;
	strbuf_addstr(&sb, path);
	अगर (is_nonbare_repository_dir(&sb)) {
		काष्ठा object_id oid;
		अगर (repo_resolve_gitlink_ref(the_repository, path, "HEAD", &oid) < 0)
			die(_("'%s' does not have a commit checked out"), path);
	}
	strbuf_release(&sb);
}

अटल पूर्णांक module_add(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
		      काष्ठा repository *repo UNUSED)
{
	पूर्णांक force = 0, quiet = 0, progress = 0, dissociate = 0;
	काष्ठा add_तथ्य add_तथ्य = ADD_तथ्य_INIT;
	स्थिर अक्षर *ref_storage_format = शून्य;
	अक्षर *to_free = शून्य;
	स्थिर काष्ठा submodule *existing;
	काष्ठा strbuf buf = STRBUF_INIT;
	अक्षर *sm_name_to_free = शून्य;
	काष्ठा option options[] = {
		OPT_STRING('b', "branch", &add_तथ्य.branch, N_("branch"),
			   N_("branch of repository to add as submodule")),
		OPT__FORCE(&force, N_("allow adding an otherwise ignored submodule path"),
			   PARSE_OPT_NOCOMPLETE),
		OPT__QUIET(&quiet, N_("print only error messages")),
		OPT_BOOL(0, "progress", &progress, N_("force cloning progress")),
		OPT_STRING(0, "reference", &add_तथ्य.reference_path, N_("repository"),
			   N_("reference repository")),
		OPT_STRING(0, "ref-storage-format", &ref_storage_format, N_("format"),
			   N_("specify the reference storage format to use")),
		OPT_ALIAS_F(0, "ref-format", "ref-storage-format", PARSE_OPT_HIDDEN),
		OPT_BOOL(0, "dissociate", &dissociate, N_("borrow the objects from reference repositories")),
		OPT_STRING(0, "name", &add_तथ्य.sm_name, N_("name"),
			   N_("sets the submodule's name to the given string "
			      "instead of defaulting to its path")),
		OPT_INTEGER(0, "depth", &add_तथ्य.depth, N_("depth for shallow clones")),
		OPT_END()
	};
	स्थिर अक्षर *स्थिर usage[] = {
		N_("git submodule add [<options>] [--] <repository> [<path>]"),
		शून्य
	};
	काष्ठा strbuf sb = STRBUF_INIT;
	पूर्णांक ret = 1;

	argc = parse_options(argc, argv, prefix, options, usage, 0);

	अगर (!is_writing_gitmodules_ok())
		die(_("please make sure that the .gitmodules file is in the working tree"));

	अगर (prefix && *prefix &&
	    add_तथ्य.reference_path && !is_absolute_path(add_तथ्य.reference_path))
		add_तथ्य.reference_path = xstrfmt("%s%s", prefix, add_तथ्य.reference_path);

	अगर (argc == 0 || argc > 2)
		usage_with_options(usage, options);

	अगर (ref_storage_format) {
		add_तथ्य.ref_storage_format = ref_storage_format_by_name(ref_storage_format);
		अगर (add_तथ्य.ref_storage_format == REF_STORAGE_FORMAT_UNKNOWN)
			die(_("unknown ref storage format '%s'"), ref_storage_format);
	}

	add_तथ्य.repo = argv[0];
	अगर (argc == 1)
		add_तथ्य.sm_path = git_url_basename(add_तथ्य.repo, 0, 0);
	अन्यथा
		add_तथ्य.sm_path = xstrdup(argv[1]);

	अगर (prefix && *prefix && !is_absolute_path(add_तथ्य.sm_path)) {
		अक्षर *sm_path = add_तथ्य.sm_path;

		add_तथ्य.sm_path = xstrfmt("%s%s", prefix, sm_path);
		मुक्त(sm_path);
	}

	अगर (starts_with_dot_dot_slash(add_तथ्य.repo) ||
	    starts_with_dot_slash(add_तथ्य.repo)) {
		अगर (prefix)
			die(_("Relative path can only be used from the toplevel "
			      "of the working tree"));

		/* dereference source url relative to parent's url */
		to_free = resolve_relative_url(add_तथ्य.repo, शून्य, 1);
		add_तथ्य.realrepo = to_free;
	} अन्यथा अगर (is_dir_sep(add_तथ्य.repo[0]) || म_अक्षर(add_तथ्य.repo, ':')) {
		add_तथ्य.realrepo = add_तथ्य.repo;
	} अन्यथा {
		die(_("repo URL: '%s' must be absolute or begin with ./|../"),
		    add_तथ्य.repo);
	}

	/*
	 * normalize path:
	 * multiple //; leading ./; /./; /../;
	 */
	normalize_path_copy(add_तथ्य.sm_path, add_तथ्य.sm_path);
	strip_dir_trailing_slashes(add_तथ्य.sm_path);

	अगर (validate_submodule_path(add_तथ्य.sm_path) < 0)
		die(शून्य);

	die_on_सूचकांक_match(add_तथ्य.sm_path, force);
	die_on_repo_without_commits(add_तथ्य.sm_path);

	अगर (!force) {
		काष्ठा child_process cp = CHILD_PROCESS_INIT;

		cp.git_cmd = 1;
		cp.no_stdout = 1;
		strvec_pushl(&cp.args, "add", "--dry-run", "--ignore-missing",
			     "--no-warn-embedded-repo", add_तथ्य.sm_path, शून्य);
		अगर ((ret = pipe_command(&cp, शून्य, 0, शून्य, 0, &sb, 0))) {
			strbuf_complete_पंक्ति(&sb);
			ख_माला_दो(sb.buf, मानक_त्रुटि);
			जाओ cleanup;
		}
	}

	अगर (!add_तथ्य.sm_name)
		add_तथ्य.sm_name = add_तथ्य.sm_path;

	existing = submodule_from_name(the_repository,
					null_oid(the_hash_algo),
					add_तथ्य.sm_name);

	अगर (existing && existing->path &&
	    म_भेद(existing->path, add_तथ्य.sm_path)) {
		अगर (!force) {
			die(_("submodule name '%s' already used for path '%s'"),
			    add_तथ्य.sm_name, existing->path);
		}
		/* --force: build <name><n> until unique */
		क्रम (पूर्णांक i = 1; ; i++) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "%s%d", add_तथ्य.sm_name, i);
			अगर (!submodule_from_name(the_repository,
						null_oid(the_hash_algo),
						buf.buf)) {
				अवरोध;
			}
		}
		add_तथ्य.sm_name = sm_name_to_free = strbuf_detach(&buf, शून्य);
	}

	अगर (check_submodule_name(add_तथ्य.sm_name))
		die(_("'%s' is not a valid submodule name"), add_तथ्य.sm_name);

	add_तथ्य.prefix = prefix;
	add_तथ्य.force = !!force;
	add_तथ्य.quiet = !!quiet;
	add_तथ्य.progress = !!progress;
	add_तथ्य.dissociate = !!dissociate;

	अगर (the_repository->repository_format_submodule_path_cfg)
		create_default_gitdir_config(add_तथ्य.sm_name);

	अगर (add_submodule(&add_तथ्य))
		जाओ cleanup;
	configure_added_submodule(&add_तथ्य);

	ret = 0;
cleanup:
	मुक्त(sm_name_to_free);
	मुक्त(add_तथ्य.sm_path);
	मुक्त(to_free);
	strbuf_release(&sb);

	वापस ret;
}

पूर्णांक cmd_submodule__helper(पूर्णांक argc,
			  स्थिर अक्षर **argv,
			  स्थिर अक्षर *prefix,
			  काष्ठा repository *repo)
{
	parse_opt_subcommand_fn *fn = शून्य;
	स्थिर अक्षर *स्थिर usage[] = {
		N_("git submodule--helper <command>"),
		शून्य
	};
	काष्ठा option options[] = {
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
		OPT_SUBCOMMAND("push-check", &fn, धकेलो_check),
		OPT_SUBCOMMAND("absorbgitdirs", &fn, absorb_git_dirs),
		OPT_SUBCOMMAND("set-url", &fn, module_set_url),
		OPT_SUBCOMMAND("set-branch", &fn, module_set_branch),
		OPT_SUBCOMMAND("create-branch", &fn, module_create_branch),
		OPT_SUBCOMMAND("get-default-remote", &fn, module_get_default_remote),
		OPT_END()
	};
	argc = parse_options(argc, argv, prefix, options, usage, 0);

	वापस fn(argc, argv, prefix, repo);
}
