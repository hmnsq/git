#परिभाषय USE_THE_REPOSITORY_VARIABLE

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

#परिभाषय OPT_QUIET (1 << 0)
#परिभाषय OPT_CACHED (1 << 1)
#परिभाषय OPT_RECURSIVE (1 << 2)
#परिभाषय OPT_FORCE (1 << 3)

प्रकारनिर्देश शून्य (*each_submodule_fn)(ध्रुव संरचना cache_प्रविष्टिः *list_item,
				  शून्य *cb_दत्त);

स्थिर वर्ण *get_default_remote(शून्य)
{
	प्रत्यागम xstrdup(repo_default_remote(the_repository));
}

स्थिर वर्ण *resolve_relative_url(ध्रुव वर्ण *rel_url, ध्रुव वर्ण *ऊर्ध्वम्_मार्गः, पूर्णाङ्क quiet)
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
	resolved_url = relative_url(remoteurl, rel_url, ऊर्ध्वम्_मार्गः);

	मुक्त(remote);
	मुक्त(remoteurl);
	strbuf_release(&remotesb);

	प्रत्यागम resolved_url;
}

स्थिर पूर्णाङ्क get_default_remote_submodule(ध्रुव वर्ण *module_मार्गः, वर्ण **default_remote)
{
	ध्रुव संरचना submodule *sub;
	संरचना repository subrepo;
	ध्रुव वर्ण *remote_name = रिक्त;
	वर्ण *url = रिक्त;
	पूर्णाङ्क ret = 0;

	sub = submodule_from_मार्गः(the_repository, null_oid(the_hash_algo), module_मार्गः);
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

	यदि (repo_submodule_init(&subrepo, the_repository, module_मार्गः,
				null_oid(the_hash_algo)) < 0) {
		ret = die_सन्देशः(_("could not get a repository handle for submodule '%s'"),
				   module_मार्गः);
		गच्छ बहिर्गमनम्;
	}

	/* Look up by URL first */
	यदि (url)
		remote_name = repo_remote_from_url(&subrepo, url);
	यदि (!remote_name)
		remote_name = repo_default_remote(&subrepo);

	*default_remote = xstrdup(remote_name);

बहिर्गमनम्:
	repo_clear(&subrepo);
	मुक्त(url);

	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क module_get_default_remote(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
				     संरचना repository *repo अप्रयुक्तम्)
{
	ध्रुव वर्ण *मार्गः;
	वर्ण *resolved_मार्गः = रिक्त;
	वर्ण *default_remote = रिक्त;
	पूर्णाङ्क संहिता;
	संरचना option options[] = {
		OPT_अन्तः()
	};
	ध्रुव वर्ण *ध्रुव usage[] = {
		N_("git submodule--helper get-default-remote <path>"),
		रिक्त
	};

	argc = parse_options(argc, argv, उपसर्गः, options, usage, 0);
	यदि (argc != 1)
		usage_with_options(usage, options);

	मार्गः = argv[0];
	यदि (उपसर्गः && *उपसर्गः && !is_absolute_मार्गः(मार्गः)) {
		resolved_मार्गः = xstrfmt("%s%s", उपसर्गः, मार्गः);
		मार्गः = resolved_मार्गः;
	}

	संहिता = get_default_remote_submodule(मार्गः, &default_remote);
	यदि (संहिता) {
		मुक्त(resolved_मार्गः);
		प्रत्यागम संहिता;
	}

	प्रारूप_लिख("%s\n", default_remote);
	मुक्त(default_remote);
	मुक्त(resolved_मार्गः);
	प्रत्यागम 0;
}

/* the result should be freed by the caller. */
स्थिर वर्ण *get_submodule_displaypath(ध्रुव वर्ण *मार्गः, ध्रुव वर्ण *उपसर्गः,
				       ध्रुव वर्ण *super_उपसर्गः)
{
	यदि (उपसर्गः && super_उपसर्गः) {
		BUG("cannot have prefix '%s' and superprefix '%s'",
		    उपसर्गः, super_उपसर्गः);
	} अन्यथा यदि (उपसर्गः) {
		संरचना strbuf sb = STRBUF_INIT;
		वर्ण *displaypath = xstrdup(relative_मार्गः(मार्गः, उपसर्गः, &sb));
		strbuf_release(&sb);
		प्रत्यागम displaypath;
	} अन्यथा यदि (super_उपसर्गः) {
		प्रत्यागम xstrfmt("%s%s", super_उपसर्गः, मार्गः);
	} अन्यथा {
		प्रत्यागम xstrdup(मार्गः);
	}
}

स्थिर वर्ण *compute_rev_name(ध्रुव वर्ण *sub_मार्गः, ध्रुव वर्ण* object_id)
{
	संरचना strbuf sb = STRBUF_INIT;
	ध्रुव वर्ण ***d;

	स्थिर ध्रुव वर्ण *describe_bare[] = { रिक्त };

	स्थिर ध्रुव वर्ण *describe_चिह्नकानि[] = { "--tags", रिक्त };

	स्थिर ध्रुव वर्ण *describe_अन्तर्भवति[] = { "--contains", रिक्त };

	स्थिर ध्रुव वर्ण *describe_all_always[] = { "--all", "--always", रिक्त };

	स्थिर ध्रुव वर्ण **describe_argv[] = { describe_bare, describe_चिह्नकानि,
						describe_अन्तर्भवति,
						describe_all_always, रिक्त };

	प्रति (d = describe_argv; *d; d++) {
		संरचना child_process cp = CHILD_PROCESS_INIT;
		prepare_submodule_repo_env(&cp.env);
		cp.dir = sub_मार्गः;
		cp.git_cmd = 1;
		cp.no_stderr = 1;

		strvec_प्रक्षिप(&cp.तर्काः, "describe");
		strvec_pushv(&cp.तर्काः, *d);
		strvec_प्रक्षिप(&cp.तर्काः, object_id);

		यदि (!capture_command(&cp, &sb, 0)) {
			strbuf_strip_suffix(&sb, "\n");
			प्रत्यागम strbuf_detach(&sb, रिक्त);
		}
	}

	strbuf_release(&sb);
	प्रत्यागम रिक्त;
}

संरचना module_list {
	ध्रुव संरचना cache_प्रविष्टिः **entries;
	पूर्णाङ्क alloc, nr;
};
#परिभाषय MODULE_LIST_INIT { 0 }

स्थिर शून्य module_list_release(संरचना module_list *ml)
{
	मुक्त(ml->entries);
}

स्थिर पूर्णाङ्क module_list_compute(ध्रुव वर्ण **argv,
			       ध्रुव वर्ण *उपसर्गः,
			       संरचना pathspec *pathspec,
			       संरचना module_list *list)
{
	पूर्णाङ्क परिणाम = 0;
	वर्ण *ps_matched = रिक्त;

	parse_pathspec(pathspec, 0,
		       PATHSPEC_PREFER_पूर्णम्,
		       उपसर्गः, argv);

	यदि (pathspec->nr)
		ps_matched = xcalloc(pathspec->nr, 1);

	यदि (repo_read_सूचकांक(the_repository) < 0)
		die(_("index file corrupt"));

	प्रति (आकार_t i = 0; i < the_repository->सूचकांक->cache_nr; i++) {
		ध्रुव संरचना cache_प्रविष्टिः *ce = the_repository->सूचकांक->cache[i];

		यदि (!मेलनम्_pathspec(the_repository->सूचकांक, pathspec, ce->name, ce_namelen(ce),
				    0, ps_matched, 1) ||
		    !S_ISGITLINK(ce->ce_बहुलकम्))
			अनुवर्त;

		ALLOC_GROW(list->entries, list->nr + 1, list->alloc);
		list->entries[list->nr++] = ce;
		यावत् (i + 1 < the_repository->सूचकांक->cache_nr &&
		       !वाक्यं_तुलय(ce->name, the_repository->सूचकांक->cache[i + 1]->name))
			/*
			 * Skip entries with the same name in different stages
			 * to make sure an entry is returned only once.
			 */
			i++;
	}

	यदि (ps_matched && report_मार्गः_error(ps_matched, pathspec))
		परिणाम = -1;

	मुक्त(ps_matched);

	प्रत्यागम परिणाम;
}

स्थिर शून्य module_list_सक्रिय(संरचना module_list *list)
{
	पूर्णाङ्क i;
	संरचना module_list सक्रिय_modules = MODULE_LIST_INIT;

	प्रति (i = 0; i < list->nr; i++) {
		ध्रुव संरचना cache_प्रविष्टिः *ce = list->entries[i];

		यदि (!is_submodule_सक्रिय(the_repository, ce->name))
			अनुवर्त;

		ALLOC_GROW(सक्रिय_modules.entries,
			   सक्रिय_modules.nr + 1,
			   सक्रिय_modules.alloc);
		सक्रिय_modules.entries[सक्रिय_modules.nr++] = ce;
	}

	module_list_release(list);
	*list = सक्रिय_modules;
}

स्थिर वर्ण *get_ऊर्ध्वम्_मार्गः(ध्रुव वर्ण *मार्गः)
{
	संरचना strbuf sb = STRBUF_INIT;

	strbuf_addstrings(&sb, "../", गणनम्_slashes(मार्गः));

	/*
	 * Check if 'path' ends with slash or not
	 * for having the same output for dir/sub_dir
	 * and dir/sub_dir/
	 */
	यदि (!is_dir_sep(मार्गः[वाक्यदैर्घ्यम्(मार्गः) - 1]))
		strbuf_addstr(&sb, "../");

	प्रत्यागम strbuf_detach(&sb, रिक्त);
}

स्थिर शून्य for_each_listed_submodule(ध्रुव संरचना module_list *list,
				      each_submodule_fn fn, शून्य *cb_दत्त)
{
	पूर्णाङ्क i;

	प्रति (i = 0; i < list->nr; i++)
		fn(list->entries[i], cb_दत्त);
}

संरचना foreach_cb {
	पूर्णाङ्क argc;
	ध्रुव वर्ण **argv;
	ध्रुव वर्ण *उपसर्गः;
	ध्रुव वर्ण *super_उपसर्गः;
	पूर्णाङ्क quiet;
	पूर्णाङ्क recursive;
};
#परिभाषय FOREACH_CB_INIT { 0 }

स्थिर शून्य runcommand_अन्तर्_submodule_cb(ध्रुव संरचना cache_प्रविष्टिः *list_item,
				       शून्य *cb_दत्त)
{
	संरचना foreach_cb *info = cb_दत्त;
	ध्रुव वर्ण *मार्गः = list_item->name;
	ध्रुव संरचना object_id *ce_oid = &list_item->oid;
	ध्रुव संरचना submodule *sub;
	संरचना child_process cp = CHILD_PROCESS_INIT;
	वर्ण *displaypath;

	यदि (validate_submodule_मार्गः(मार्गः) < 0)
		die(रिक्त);

	displaypath = get_submodule_displaypath(मार्गः, info->उपसर्गः,
						info->super_उपसर्गः);

	sub = submodule_from_मार्गः(the_repository, null_oid(the_hash_algo), मार्गः);

	यदि (!sub)
		die(_("No url found for submodule path '%s' in .gitmodules"),
			displaypath);

	यदि (!is_submodule_populated_gently(मार्गः, रिक्त))
		गच्छ cleanup;

	prepare_submodule_repo_env(&cp.env);

	/*
	 * For the purpose of executing <command> in the submodule,
	 * separate shell is used for the purpose of running the
	 * child process.
	 */
	cp.use_shell = 1;
	cp.dir = मार्गः;

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
		strvec_pushf(&cp.env, "sm_path=%s", मार्गः);
		strvec_pushf(&cp.env, "displaypath=%s", displaypath);
		strvec_pushf(&cp.env, "sha1=%s",
			     oid_गन्तव्यम्_hex(ce_oid));
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
		sq_quote_buf(&sb, मार्गः);
		strvec_pushf(&cp.तर्काः, "path=%s; %s",
			     sb.buf, info->argv[0]);
		strbuf_release(&sb);
		मुक्त(toplevel);
	} अन्यथा {
		strvec_pushv(&cp.तर्काः, info->argv);
	}

	यदि (!info->quiet)
		प्रारूप_लिख(_("Entering '%s'\n"), displaypath);

	यदि (info->argv[0]) {
		यदि (सततम्_command(&cp))
			die(_("run_command returned non-zero status for %s\n."),
			    displaypath);
	} अन्यथा {
		child_process_clear(&cp);
	}

	यदि (info->recursive) {
		संरचना child_process cpr = CHILD_PROCESS_INIT;

		cpr.git_cmd = 1;
		cpr.dir = मार्गः;
		prepare_submodule_repo_env(&cpr.env);

		strvec_pushl(&cpr.तर्काः, "submodule--helper", "foreach", "--recursive",
			     रिक्त);
		strvec_pushf(&cpr.तर्काः, "--super-prefix=%s/", displaypath);

		यदि (info->quiet)
			strvec_प्रक्षिप(&cpr.तर्काः, "--quiet");

		strvec_प्रक्षिप(&cpr.तर्काः, "--");
		strvec_pushv(&cpr.तर्काः, info->argv);

		यदि (सततम्_command(&cpr))
			die(_("run_command returned non-zero status while "
				"recursing in the nested submodules of %s\n."),
				displaypath);
	}

cleanup:
	मुक्त(displaypath);
}

स्थिर पूर्णाङ्क module_foreach(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
			  संरचना repository *repo अप्रयुक्तम्)
{
	संरचना foreach_cb info = FOREACH_CB_INIT;
	संरचना pathspec pathspec = { 0 };
	संरचना module_list list = MODULE_LIST_INIT;
	संरचना option module_foreach_options[] = {
		OPT_SUPER_उपसर्गः(&info.super_उपसर्गः),
		OPT__QUIET(&info.quiet, N_("suppress output of entering each submodule command")),
		OPT_BOOL(0, "recursive", &info.recursive,
			 N_("recurse into nested submodules")),
		OPT_अन्तः()
	};
	ध्रुव वर्ण *ध्रुव git_submodule_helper_usage[] = {
		N_("git submodule foreach [--quiet] [--recursive] [--] <command>"),
		रिक्त
	};
	पूर्णाङ्क ret = 1;

	argc = parse_options(argc, argv, उपसर्गः, module_foreach_options,
			     git_submodule_helper_usage, 0);

	यदि (module_list_compute(रिक्त, उपसर्गः, &pathspec, &list) < 0)
		गच्छ cleanup;

	info.argc = argc;
	info.argv = argv;
	info.उपसर्गः = उपसर्गः;

	for_each_listed_submodule(&list, runcommand_अन्तर्_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	प्रत्यागम ret;
}

संरचना init_cb {
	ध्रुव वर्ण *उपसर्गः;
	ध्रुव वर्ण *super_उपसर्गः;
	अचिह्नित पूर्णाङ्क flags;
};
#परिभाषय INIT_CB_INIT { 0 }

स्थिर पूर्णाङ्क validate_and_समुच्चयः_submodule_gitdir(संरचना strbuf *gitdir_मार्गः,
					     ध्रुव वर्ण *submodule_name)
{
	ध्रुव वर्ण *मानम्;
	वर्ण *कुञ्चिका;

	यदि (validate_submodule_git_dir(gitdir_मार्गः->buf, submodule_name))
		प्रत्यागम -1;

	कुञ्चिका = xstrfmt("submodule.%s.gitdir", submodule_name);

	/* Nothing to do if the config already exists. */
	यदि (!repo_config_get_string_tmp(the_repository, कुञ्चिका, &मानम्)) {
		मुक्त(कुञ्चिका);
		प्रत्यागम 0;
	}

	यदि (repo_config_समुच्चयः_gently(the_repository, कुञ्चिका, gitdir_मार्गः->buf)) {
		मुक्त(कुञ्चिका);
		प्रत्यागम -1;
	}

	मुक्त(कुञ्चिका);
	प्रत्यागम 0;
}

स्थिर शून्य create_default_gitdir_config(ध्रुव वर्ण *submodule_name)
{
	संरचना strbuf gitdir_मार्गः = STRBUF_INIT;
	संरचना git_hash_ctx ctx;
	वर्ण hex_name_hash[GIT_MAX_HEXSZ + 1], शीर्षकम्[128];
	अचिह्नित वर्ण raw_name_hash[GIT_MAX_RAWSZ];
	पूर्णाङ्क शीर्षकम्_len;

	/* Case 1: try the plain module name */
	repo_git_मार्गः_append(the_repository, &gitdir_मार्गः, "modules/%s", submodule_name);
	यदि (!validate_and_समुच्चयः_submodule_gitdir(&gitdir_मार्गः, submodule_name)) {
		strbuf_release(&gitdir_मार्गः);
		प्रत्यागम;
	}

	/* Case 2.1: Try URI-safe (RFC3986) encoding first, this fixes nested gitdirs */
	strbuf_reset(&gitdir_मार्गः);
	repo_git_मार्गः_append(the_repository, &gitdir_मार्गः, "modules/");
	strbuf_addstr_urlencode(&gitdir_मार्गः, submodule_name, is_rfc3986_unreserved);
	यदि (!validate_and_समुच्चयः_submodule_gitdir(&gitdir_मार्गः, submodule_name)) {
		strbuf_release(&gitdir_मार्गः);
		प्रत्यागम;
	}

	/* Case 2.2: Try extended uppercase URI (RFC3986) encoding, to fix case-folding */
	strbuf_reset(&gitdir_मार्गः);
	repo_git_मार्गः_append(the_repository, &gitdir_मार्गः, "modules/");
	strbuf_addstr_urlencode(&gitdir_मार्गः, submodule_name, is_casefolding_rfc3986_unreserved);
	यदि (!validate_and_समुच्चयः_submodule_gitdir(&gitdir_मार्गः, submodule_name))
		प्रत्यागम;

	/* Case 2.3: Try some derived gitdir names, see if one sticks */
	प्रति (वर्ण c = '0'; c <= '9'; c++) {
		strbuf_reset(&gitdir_मार्गः);
		repo_git_मार्गः_append(the_repository, &gitdir_मार्गः, "modules/");
		strbuf_addstr_urlencode(&gitdir_मार्गः, submodule_name, is_rfc3986_unreserved);
		strbuf_addch(&gitdir_मार्गः, c);
		यदि (!validate_and_समुच्चयः_submodule_gitdir(&gitdir_मार्गः, submodule_name))
			प्रत्यागम;

		strbuf_reset(&gitdir_मार्गः);
		repo_git_मार्गः_append(the_repository, &gitdir_मार्गः, "modules/");
		strbuf_addstr_urlencode(&gitdir_मार्गः, submodule_name, is_casefolding_rfc3986_unreserved);
		strbuf_addch(&gitdir_मार्गः, c);
		यदि (!validate_and_समुच्चयः_submodule_gitdir(&gitdir_मार्गः, submodule_name))
			प्रत्यागम;
	}

	/* Case 2.4: If all the above failed, try a hash of the name as a last resort */
	शीर्षकम्_len = snprintf(शीर्षकम्, परिमाण(शीर्षकम्),
			      "blob %"PRIuMAX, (uintmax_t)वाक्यदैर्घ्यम्(submodule_name));
	git_hash_init(&ctx, the_hash_algo);
	git_hash_update(&ctx, शीर्षकम्, शीर्षकम्_len);
	git_hash_update(&ctx, "\0", 1);
	git_hash_update(&ctx, submodule_name, वाक्यदैर्घ्यम्(submodule_name));
	git_hash_final(raw_name_hash, &ctx);
	hash_गन्तव्यम्_hex_algop_r(hex_name_hash, raw_name_hash, the_hash_algo);
	strbuf_reset(&gitdir_मार्गः);
	repo_git_मार्गः_append(the_repository, &gitdir_मार्गः, "modules/%s", hex_name_hash);
	यदि (!validate_and_समुच्चयः_submodule_gitdir(&gitdir_मार्गः, submodule_name)) {
		strbuf_release(&gitdir_मार्गः);
		प्रत्यागम;
	}

	/* Case 3: nothing worked, error out */
	die(_("failed to set a valid default config for 'submodule.%s.gitdir'. "
	      "Please ensure it is set, for example by running something like: "
	      "'git config submodule.%s.gitdir .git/modules/%s'"),
	    submodule_name, submodule_name, submodule_name);
}

स्थिर शून्य init_submodule(ध्रुव वर्ण *मार्गः, ध्रुव वर्ण *उपसर्गः,
			   ध्रुव वर्ण *super_उपसर्गः,
			   अचिह्नित पूर्णाङ्क flags)
{
	ध्रुव संरचना submodule *sub;
	संरचना strbuf sb = STRBUF_INIT;
	ध्रुव वर्ण *upd;
	वर्ण *url = रिक्त, *displaypath;

	displaypath = get_submodule_displaypath(मार्गः, उपसर्गः, super_उपसर्गः);

	sub = submodule_from_मार्गः(the_repository, null_oid(the_hash_algo), मार्गः);

	यदि (!sub)
		die(_("No url found for submodule path '%s' in .gitmodules"),
			displaypath);

	/*
	 * NEEDSWORK: In a multi-working-tree world, this needs to be
	 * set in the per-worktree config.
	 *
	 * Set active flag for the submodule being initialized
	 */
	यदि (!is_submodule_सक्रिय(the_repository, मार्गः)) {
		strbuf_addf(&sb, "submodule.%s.active", sub->name);
		repo_config_समुच्चयः_gently(the_repository, sb.buf, "true");
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

		यदि (repo_config_समुच्चयः_gently(the_repository, sb.buf, url))
			die(_("Failed to register url for submodule path '%s'"),
			    displaypath);
		यदि (!(flags & OPT_QUIET))
			fprintf(stderr,
				_("Submodule '%s' (%s) registered for path '%s'\n"),
				sub->name, url, displaypath);
	}
	strbuf_reset(&sb);

	/* Copy "update" setting when it is not set yet */
	strbuf_addf(&sb, "submodule.%s.update", sub->name);
	यदि (repo_config_get_string_tmp(the_repository, sb.buf, &upd) &&
	    sub->update_strategy.type != SM_UPDATE_UNSPECIFIED) {
		यदि (sub->update_strategy.type == SM_UPDATE_COMMAND) {
			fprintf(stderr, _("warning: command update mode suggested for submodule '%s'\n"),
				sub->name);
			upd = "none";
		} अन्यथा {
			upd = submodule_update_type_गन्तव्यम्_string(sub->update_strategy.type);
		}

		यदि (repo_config_समुच्चयः_gently(the_repository, sb.buf, upd))
			die(_("Failed to register update mode for submodule path '%s'"), displaypath);
	}

	यदि (the_repository->repository_format_submodule_मार्गः_cfg)
		create_default_gitdir_config(sub->name);

	strbuf_release(&sb);
	मुक्त(displaypath);
	मुक्त(url);
}

स्थिर शून्य init_submodule_cb(ध्रुव संरचना cache_प्रविष्टिः *list_item, शून्य *cb_दत्त)
{
	संरचना init_cb *info = cb_दत्त;

	init_submodule(list_item->name, info->उपसर्गः, info->super_उपसर्गः,
		       info->flags);
}

स्थिर पूर्णाङ्क module_init(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
		       संरचना repository *repo अप्रयुक्तम्)
{
	संरचना init_cb info = INIT_CB_INIT;
	संरचना pathspec pathspec = { 0 };
	संरचना module_list list = MODULE_LIST_INIT;
	पूर्णाङ्क quiet = 0;
	संरचना option module_init_options[] = {
		OPT__QUIET(&quiet, N_("suppress output for initializing a submodule")),
		OPT_अन्तः()
	};
	ध्रुव वर्ण *ध्रुव git_submodule_helper_usage[] = {
		N_("git submodule init [<options>] [<path>]"),
		रिक्त
	};
	पूर्णाङ्क ret = 1;

	argc = parse_options(argc, argv, उपसर्गः, module_init_options,
			     git_submodule_helper_usage, 0);

	यदि (module_list_compute(argv, उपसर्गः, &pathspec, &list) < 0)
		गच्छ cleanup;

	/*
	 * If there are no path args and submodule.active is set then,
	 * by default, only initialize 'active' modules.
	 */
	यदि (!argc && !repo_config_get(the_repository, "submodule.active"))
		module_list_सक्रिय(&list);

	info.उपसर्गः = उपसर्गः;
	यदि (quiet)
		info.flags |= OPT_QUIET;

	for_each_listed_submodule(&list, init_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	प्रत्यागम ret;
}

संरचना स्थितिः_cb {
	ध्रुव वर्ण *उपसर्गः;
	ध्रुव वर्ण *super_उपसर्गः;
	अचिह्नित पूर्णाङ्क flags;
};
#परिभाषय स्थितिः_CB_INIT { 0 }

स्थिर शून्य print_स्थितिः(अचिह्नित पूर्णाङ्क flags, वर्ण state, ध्रुव वर्ण *मार्गः,
			 ध्रुव संरचना object_id *oid, ध्रुव वर्ण *displaypath)
{
	यदि (flags & OPT_QUIET)
		प्रत्यागम;

	प्रारूप_लिख("%c%s %s", state, oid_गन्तव्यम्_hex(oid), displaypath);

	यदि (state == ' ' || state == '+') {
		वर्ण *name = compute_rev_name(मार्गः, oid_गन्तव्यम्_hex(oid));

		यदि (name)
			प्रारूप_लिख(" (%s)", name);
		मुक्त(name);
	}

	प्रारूप_लिख("\n");
}

स्थिर पूर्णाङ्क handle_submodule_शीर्ष_ref(ध्रुव संरचना reference *ref, शून्य *cb_दत्त)
{
	संरचना object_id *output = cb_दत्त;

	यदि (ref->oid)
		oidcpy(output, ref->oid);

	प्रत्यागम 0;
}

स्थिर शून्य स्थितिः_submodule(ध्रुव वर्ण *मार्गः, ध्रुव संरचना object_id *ce_oid,
			     अचिह्नित पूर्णाङ्क ce_flags, ध्रुव वर्ण *उपसर्गः,
			     ध्रुव वर्ण *super_उपसर्गः, अचिह्नित पूर्णाङ्क flags)
{
	वर्ण *displaypath;
	संरचना strvec diff_files_तर्काः = STRVEC_INIT;
	संरचना rev_info rev = REV_INFO_INIT;
	संरचना strbuf buf = STRBUF_INIT;
	ध्रुव वर्ण *git_dir;

	यदि (validate_submodule_मार्गः(मार्गः) < 0)
		die(रिक्त);

	यदि (!submodule_from_मार्गः(the_repository, null_oid(the_hash_algo), मार्गः))
		die(_("no submodule mapping found in .gitmodules for path '%s'"),
		      मार्गः);

	displaypath = get_submodule_displaypath(मार्गः, उपसर्गः, super_उपसर्गः);

	यदि ((CE_STAGEMASK & ce_flags) >> CE_STAGESHIFT) {
		print_स्थितिः(flags, 'U', मार्गः, null_oid(the_hash_algo), displaypath);
		गच्छ cleanup;
	}

	strbuf_addf(&buf, "%s/.git", मार्गः);
	git_dir = read_gitfile(buf.buf);
	यदि (!git_dir)
		git_dir = buf.buf;

	यदि (!is_submodule_सक्रिय(the_repository, मार्गः) ||
	    !is_git_directory(git_dir)) {
		print_स्थितिः(flags, '-', मार्गः, ce_oid, displaypath);
		strbuf_release(&buf);
		गच्छ cleanup;
	}
	strbuf_release(&buf);

	strvec_pushl(&diff_files_तर्काः, "diff-files",
		     "--ignore-submodules=dirty", "--quiet", "--",
		     मार्गः, रिक्त);

	repo_config(the_repository, git_diff_basic_config, रिक्त);

	repo_init_revisions(the_repository, &rev, रिक्त);
	rev.abbrev = 0;
	सज्जता_revisions_from_strvec(&diff_files_तर्काः, &rev, रिक्त);
	सततम्_diff_files(&rev, 0);

	यदि (!diff_परिणाम_संहिता(&rev)) {
		print_स्थितिः(flags, ' ', मार्गः, ce_oid,
			     displaypath);
	} अन्यथा यदि (!(flags & OPT_CACHED)) {
		संरचना object_id oid;
		संरचना ref_store *refs = repo_get_submodule_ref_store(the_repository,
								      मार्गः);

		यदि (!refs) {
			print_स्थितिः(flags, '-', मार्गः, ce_oid, displaypath);
			गच्छ cleanup;
		}
		यदि (refs_शीर्ष_ref(refs, handle_submodule_शीर्ष_ref, &oid))
			die(_("could not resolve HEAD ref inside the "
			      "submodule '%s'"), मार्गः);

		print_स्थितिः(flags, '+', मार्गः, &oid, displaypath);
	} अन्यथा {
		print_स्थितिः(flags, '+', मार्गः, ce_oid, displaypath);
	}

	यदि (flags & OPT_RECURSIVE) {
		संरचना child_process cpr = CHILD_PROCESS_INIT;
		पूर्णाङ्क res;

		cpr.git_cmd = 1;
		cpr.dir = मार्गः;
		prepare_submodule_repo_env(&cpr.env);

		strvec_pushl(&cpr.तर्काः, "submodule--helper", "status",
			     "--recursive", रिक्त);
		strvec_pushf(&cpr.तर्काः, "--super-prefix=%s/", displaypath);

		यदि (flags & OPT_CACHED)
			strvec_प्रक्षिप(&cpr.तर्काः, "--cached");

		यदि (flags & OPT_QUIET)
			strvec_प्रक्षिप(&cpr.तर्काः, "--quiet");

		res = सततम्_command(&cpr);
		यदि (res == SIGPIPE + 128)
			उत्थापय(SIGPIPE);
		अन्यथा यदि (res)
			die(_("failed to recurse into submodule '%s'"), मार्गः);
	}

cleanup:
	strvec_clear(&diff_files_तर्काः);
	मुक्त(displaypath);
	release_revisions(&rev);
}

स्थिर शून्य स्थितिः_submodule_cb(ध्रुव संरचना cache_प्रविष्टिः *list_item,
				शून्य *cb_दत्त)
{
	संरचना स्थितिः_cb *info = cb_दत्त;

	स्थितिः_submodule(list_item->name, &list_item->oid, list_item->ce_flags,
			 info->उपसर्गः, info->super_उपसर्गः, info->flags);
}

स्थिर पूर्णाङ्क module_स्थितिः(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
			 संरचना repository *repo अप्रयुक्तम्)
{
	संरचना स्थितिः_cb info = स्थितिः_CB_INIT;
	संरचना pathspec pathspec = { 0 };
	संरचना module_list list = MODULE_LIST_INIT;
	पूर्णाङ्क quiet = 0;
	संरचना option module_स्थितिः_options[] = {
		OPT_SUPER_उपसर्गः(&info.super_उपसर्गः),
		OPT__QUIET(&quiet, N_("suppress submodule status output")),
		OPT_BIT(0, "cached", &info.flags, N_("use commit stored in the index instead of the one stored in the submodule HEAD"), OPT_CACHED),
		OPT_BIT(0, "recursive", &info.flags, N_("recurse into nested submodules"), OPT_RECURSIVE),
		OPT_अन्तः()
	};
	ध्रुव वर्ण *ध्रुव git_submodule_helper_usage[] = {
		N_("git submodule status [--quiet] [--cached] [--recursive] [<path>...]"),
		रिक्त
	};
	पूर्णाङ्क ret = 1;

	argc = parse_options(argc, argv, उपसर्गः, module_स्थितिः_options,
			     git_submodule_helper_usage, 0);

	यदि (module_list_compute(argv, उपसर्गः, &pathspec, &list) < 0)
		गच्छ cleanup;

	info.उपसर्गः = उपसर्गः;
	यदि (quiet)
		info.flags |= OPT_QUIET;

	for_each_listed_submodule(&list, स्थितिः_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	प्रत्यागम ret;
}

संरचना module_cb {
	अचिह्नित पूर्णाङ्क mod_src;
	अचिह्नित पूर्णाङ्क mod_dst;
	संरचना object_id oid_src;
	संरचना object_id oid_dst;
	वर्ण स्थितिः;
	वर्ण *sm_मार्गः;
};
#परिभाषय MODULE_CB_INIT { 0 }

स्थिर शून्य module_cb_release(संरचना module_cb *mcb)
{
	मुक्त(mcb->sm_मार्गः);
}

संरचना module_cb_list {
	संरचना module_cb **entries;
	पूर्णाङ्क alloc, nr;
};
#परिभाषय MODULE_CB_LIST_INIT { 0 }

स्थिर शून्य module_cb_list_release(संरचना module_cb_list *mcbl)
{
	पूर्णाङ्क i;

	प्रति (i = 0; i < mcbl->nr; i++) {
		संरचना module_cb *mcb = mcbl->entries[i];

		module_cb_release(mcb);
		मुक्त(mcb);
	}
	मुक्त(mcbl->entries);
}

संरचना summary_cb {
	पूर्णाङ्क argc;
	ध्रुव वर्ण **argv;
	ध्रुव वर्ण *उपसर्गः;
	ध्रुव वर्ण *super_उपसर्गः;
	अचिह्नित पूर्णाङ्क cached: 1;
	अचिह्नित पूर्णाङ्क for_स्थितिः: 1;
	अचिह्नित पूर्णाङ्क files: 1;
	पूर्णाङ्क summary_सीमा;
};
#परिभाषय SUMMARY_CB_INIT { 0 }

गणना diff_cmd {
	DIFF_सूचकांक,
	DIFF_FILES
};

स्थिर वर्ण *verify_submodule_committish(ध्रुव वर्ण *sm_मार्गः,
					 ध्रुव वर्ण *committish)
{
	संरचना child_process cp_rev_parse = CHILD_PROCESS_INIT;
	संरचना strbuf परिणाम = STRBUF_INIT;

	cp_rev_parse.git_cmd = 1;
	cp_rev_parse.dir = sm_मार्गः;
	prepare_submodule_repo_env(&cp_rev_parse.env);
	strvec_pushl(&cp_rev_parse.तर्काः, "rev-parse", "-q", "--short", रिक्त);
	strvec_pushf(&cp_rev_parse.तर्काः, "%s^0", committish);
	strvec_प्रक्षिप(&cp_rev_parse.तर्काः, "--");

	यदि (capture_command(&cp_rev_parse, &परिणाम, 0))
		प्रत्यागम रिक्त;

	strbuf_trim_trailing_newline(&परिणाम);
	प्रत्यागम strbuf_detach(&परिणाम, रिक्त);
}

स्थिर शून्य print_submodule_summary(संरचना summary_cb *info, ध्रुव वर्ण *errmsg,
				    पूर्णाङ्क योग_commits, ध्रुव वर्ण *displaypath,
				    ध्रुव वर्ण *src_abbrev, ध्रुव वर्ण *dst_abbrev,
				    संरचना module_cb *p)
{
	यदि (p->स्थितिः == 'T') {
		यदि (S_ISGITLINK(p->mod_dst))
			प्रारूप_लिख(_("* %s %s(blob)->%s(submodule)"),
				 displaypath, src_abbrev, dst_abbrev);
		अन्यथा
			प्रारूप_लिख(_("* %s %s(submodule)->%s(blob)"),
				 displaypath, src_abbrev, dst_abbrev);
	} अन्यथा {
		प्रारूप_लिख("* %s %s...%s",
			displaypath, src_abbrev, dst_abbrev);
	}

	यदि (योग_commits < 0)
		प्रारूप_लिख(":\n");
	अन्यथा
		प्रारूप_लिख(" (%d):\n", योग_commits);

	यदि (errmsg) {
		प्रारूप_लिख(_("%s"), errmsg);
	} अन्यथा यदि (योग_commits > 0) {
		संरचना child_process cp_log = CHILD_PROCESS_INIT;

		cp_log.git_cmd = 1;
		cp_log.dir = p->sm_मार्गः;
		prepare_submodule_repo_env(&cp_log.env);
		strvec_pushl(&cp_log.तर्काः, "log", रिक्त);

		यदि (S_ISGITLINK(p->mod_src) && S_ISGITLINK(p->mod_dst)) {
			यदि (info->summary_सीमा > 0)
				strvec_pushf(&cp_log.तर्काः, "-%d",
					     info->summary_सीमा);

			strvec_pushl(&cp_log.तर्काः, "--pretty=  %m %s",
				     "--first-parent", रिक्त);
			strvec_pushf(&cp_log.तर्काः, "%s...%s",
				     src_abbrev, dst_abbrev);
		} अन्यथा यदि (S_ISGITLINK(p->mod_dst)) {
			strvec_pushl(&cp_log.तर्काः, "--pretty=  > %s",
				     "-1", dst_abbrev, रिक्त);
		} अन्यथा {
			strvec_pushl(&cp_log.तर्काः, "--pretty=  < %s",
				     "-1", src_abbrev, रिक्त);
		}
		सततम्_command(&cp_log);
	}
	प्रारूप_लिख("\n");
}

स्थिर शून्य generate_submodule_summary(संरचना summary_cb *info,
				       संरचना module_cb *p)
{
	वर्ण *displaypath, *src_abbrev = रिक्त, *dst_abbrev;
	पूर्णाङ्क missing_src = 0, missing_dst = 0;
	संरचना strbuf errmsg = STRBUF_INIT;
	पूर्णाङ्क योग_commits = -1;

	यदि (!info->cached && oideq(&p->oid_dst, null_oid(the_hash_algo))) {
		यदि (S_ISGITLINK(p->mod_dst)) {
			संरचना ref_store *refs = repo_get_submodule_ref_store(the_repository,
									      p->sm_मार्गः);

			यदि (refs)
				refs_शीर्ष_ref(refs, handle_submodule_शीर्ष_ref, &p->oid_dst);
		} अन्यथा यदि (S_ISLNK(p->mod_dst) || S_ISREG(p->mod_dst)) {
			संरचना stat st;
			पूर्णाङ्क fd = उद्घाटय(p->sm_मार्गः, O_RDONLY);

			यदि (fd < 0 || fstat(fd, &st) < 0 ||
			    सूचकांक_fd(the_repository->सूचकांक, &p->oid_dst, fd, &st, OBJ_BLOB,
				     p->sm_मार्गः, 0))
				error(_("couldn't hash object from '%s'"), p->sm_मार्गः);
		} अन्यथा {
			/* for a submodule removal (mode:0000000), don't warn */
			यदि (p->mod_dst)
				warning(_("unexpected mode %o"), p->mod_dst);
		}
	}

	यदि (S_ISGITLINK(p->mod_src)) {
		यदि (p->स्थितिः != 'D')
			src_abbrev = verify_submodule_committish(p->sm_मार्गः,
								 oid_गन्तव्यम्_hex(&p->oid_src));
		यदि (!src_abbrev) {
			missing_src = 1;
			/*
			 * As `rev-parse` failed, we fallback to getting
			 * the abbreviated hash using oid_src. We do
			 * this as we might still need the abbreviated
			 * hash in cases like a submodule type change, etc.
			 */
			src_abbrev = xstrndup(oid_गन्तव्यम्_hex(&p->oid_src), 7);
		}
	} अन्यथा {
		/*
		 * The source does not point to a submodule.
		 * So, we fallback to getting the abbreviation using
		 * oid_src as we might still need the abbreviated
		 * hash in cases like submodule add, etc.
		 */
		src_abbrev = xstrndup(oid_गन्तव्यम्_hex(&p->oid_src), 7);
	}

	यदि (S_ISGITLINK(p->mod_dst)) {
		dst_abbrev = verify_submodule_committish(p->sm_मार्गः,
							 oid_गन्तव्यम्_hex(&p->oid_dst));
		यदि (!dst_abbrev) {
			missing_dst = 1;
			/*
			 * As `rev-parse` failed, we fallback to getting
			 * the abbreviated hash using oid_dst. We do
			 * this as we might still need the abbreviated
			 * hash in cases like a submodule type change, etc.
			 */
			dst_abbrev = xstrndup(oid_गन्तव्यम्_hex(&p->oid_dst), 7);
		}
	} अन्यथा {
		/*
		 * The destination does not point to a submodule.
		 * So, we fallback to getting the abbreviation using
		 * oid_dst as we might still need the abbreviated
		 * hash in cases like a submodule removal, etc.
		 */
		dst_abbrev = xstrndup(oid_गन्तव्यम्_hex(&p->oid_dst), 7);
	}

	displaypath = get_submodule_displaypath(p->sm_मार्गः, info->उपसर्गः,
						info->super_उपसर्गः);

	यदि (!missing_src && !missing_dst) {
		संरचना child_process cp_rev_list = CHILD_PROCESS_INIT;
		संरचना strbuf sb_rev_list = STRBUF_INIT;

		strvec_pushl(&cp_rev_list.तर्काः, "rev-list",
			     "--first-parent", "--count", रिक्त);
		यदि (S_ISGITLINK(p->mod_src) && S_ISGITLINK(p->mod_dst))
			strvec_pushf(&cp_rev_list.तर्काः, "%s...%s",
				     src_abbrev, dst_abbrev);
		अन्यथा
			strvec_प्रक्षिप(&cp_rev_list.तर्काः, S_ISGITLINK(p->mod_src) ?
				    src_abbrev : dst_abbrev);
		strvec_प्रक्षिप(&cp_rev_list.तर्काः, "--");

		cp_rev_list.git_cmd = 1;
		cp_rev_list.dir = p->sm_मार्गः;
		prepare_submodule_repo_env(&cp_rev_list.env);

		यदि (!capture_command(&cp_rev_list, &sb_rev_list, 0))
			योग_commits = atoi(sb_rev_list.buf);

		strbuf_release(&sb_rev_list);
	} अन्यथा {
		/*
		 * Don't give error msg for modification whose dst is not
		 * submodule, i.e., deleted or changed to blob
		 */
		यदि (S_ISGITLINK(p->mod_dst)) {
			यदि (missing_src && missing_dst) {
				strbuf_addf(&errmsg, "  Warn: %s doesn't contain commits %s and %s\n",
					    displaypath, oid_गन्तव्यम्_hex(&p->oid_src),
					    oid_गन्तव्यम्_hex(&p->oid_dst));
			} अन्यथा {
				strbuf_addf(&errmsg, "  Warn: %s doesn't contain commit %s\n",
					    displaypath, missing_src ?
					    oid_गन्तव्यम्_hex(&p->oid_src) :
					    oid_गन्तव्यम्_hex(&p->oid_dst));
			}
		}
	}

	print_submodule_summary(info, errmsg.len ? errmsg.buf : रिक्त,
				योग_commits, displaypath, src_abbrev,
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
	प्रति (i = 0; i < list->nr; i++) {
		ध्रुव संरचना submodule *sub;
		संरचना module_cb *p = list->entries[i];
		संरचना strbuf sm_gitdir = STRBUF_INIT;

		यदि (p->स्थितिः == 'D' || p->स्थितिः == 'T') {
			generate_submodule_summary(info, p);
			अनुवर्त;
		}

		यदि (info->for_स्थितिः && p->स्थितिः != 'A' &&
		    (sub = submodule_from_मार्गः(the_repository,
					       null_oid(the_hash_algo), p->sm_मार्गः))) {
			वर्ण *config_कुञ्चिका = रिक्त;
			ध्रुव वर्ण *मानम्;
			पूर्णाङ्क ignore_all = 0;

			config_कुञ्चिका = xstrfmt("submodule.%s.ignore",
					     sub->name);
			यदि (!repo_config_get_string_tmp(the_repository, config_कुञ्चिका, &मानम्))
				ignore_all = !वाक्यं_तुलय(मानम्, "all");
			अन्यथा यदि (sub->ignore)
				ignore_all = !वाक्यं_तुलय(sub->ignore, "all");

			मुक्त(config_कुञ्चिका);
			यदि (ignore_all)
				अनुवर्त;
		}

		/* Also show added or modified modules which are checked out */
		strbuf_addstr(&sm_gitdir, p->sm_मार्गः);
		यदि (is_nonbare_repository_dir(&sm_gitdir))
			generate_submodule_summary(info, p);
		strbuf_release(&sm_gitdir);
	}
}

स्थिर शून्य submodule_summary_callback(संरचना diff_queue_struct *q,
				       संरचना diff_options *options अप्रयुक्तम्,
				       शून्य *दत्त)
{
	पूर्णाङ्क i;
	संरचना module_cb_list *list = दत्त;
	प्रति (i = 0; i < q->nr; i++) {
		संरचना diff_filepair *p = q->queue[i];
		संरचना module_cb *अस्थायी;

		यदि (!S_ISGITLINK(p->one->बहुलकम्) && !S_ISGITLINK(p->two->बहुलकम्))
			अनुवर्त;
		अस्थायी = xmalloc(परिमाण(*अस्थायी));
		अस्थायी->mod_src = p->one->बहुलकम्;
		अस्थायी->mod_dst = p->two->बहुलकम्;
		अस्थायी->oid_src = p->one->oid;
		अस्थायी->oid_dst = p->two->oid;
		अस्थायी->स्थितिः = p->स्थितिः;
		अस्थायी->sm_मार्गः = xstrdup(p->one->मार्गः);

		ALLOC_GROW(list->entries, list->nr + 1, list->alloc);
		list->entries[list->nr++] = अस्थायी;
	}
}

स्थिर ध्रुव वर्ण *get_diff_cmd(गणना diff_cmd diff_cmd)
{
	विकल्प (diff_cmd) {
	पक्ष DIFF_सूचकांक: प्रत्यागम "diff-index";
	पक्ष DIFF_FILES: प्रत्यागम "diff-files";
	स्वभाव: BUG("bad diff_cmd value %d", diff_cmd);
	}
}

स्थिर पूर्णाङ्क compute_summary_module_list(संरचना object_id *शीर्ष_oid,
				       संरचना summary_cb *info,
				       गणना diff_cmd diff_cmd)
{
	संरचना strvec diff_तर्काः = STRVEC_INIT;
	संरचना rev_info rev;
	संरचना module_cb_list list = MODULE_CB_LIST_INIT;
	पूर्णाङ्क ret = 0;

	strvec_प्रक्षिप(&diff_तर्काः, get_diff_cmd(diff_cmd));
	यदि (info->cached)
		strvec_प्रक्षिप(&diff_तर्काः, "--cached");
	strvec_pushl(&diff_तर्काः, "--ignore-submodules=dirty", "--raw", रिक्त);
	यदि (शीर्ष_oid)
		strvec_प्रक्षिप(&diff_तर्काः, oid_गन्तव्यम्_hex(शीर्ष_oid));
	strvec_प्रक्षिप(&diff_तर्काः, "--");
	यदि (info->argc)
		strvec_pushv(&diff_तर्काः, info->argv);

	repo_config(the_repository, git_diff_basic_config, रिक्त);
	repo_init_revisions(the_repository, &rev, info->उपसर्गः);
	rev.abbrev = 0;
	precompose_argv_उपसर्गः(diff_तर्काः.nr, diff_तर्काः.v, रिक्त);
	सज्जता_revisions_from_strvec(&diff_तर्काः, &rev, रिक्त);
	rev.diffopt.output_format = DIFF_FORMAT_NO_OUTPUT | DIFF_FORMAT_CALLBACK;
	rev.diffopt.format_callback = submodule_summary_callback;
	rev.diffopt.format_callback_दत्त = &list;

	यदि (!info->cached) {
		यदि (diff_cmd == DIFF_सूचकांक)
			सज्जता_work_वृक्षः(the_repository);
		यदि (repo_read_सूचकांक_preload(the_repository, &rev.diffopt.pathspec, 0) < 0) {
			perror("repo_read_index_preload");
			ret = -1;
			गच्छ cleanup;
		}
	} अन्यथा यदि (repo_read_सूचकांक(the_repository) < 0) {
		perror("repo_read_cache");
		ret = -1;
		गच्छ cleanup;
	}

	यदि (diff_cmd == DIFF_सूचकांक)
		सततम्_diff_सूचकांक(&rev, info->cached ? DIFF_सूचकांक_CACHED : 0);
	अन्यथा
		सततम्_diff_files(&rev, 0);
	prepare_submodule_summary(info, &list);
cleanup:
	strvec_clear(&diff_तर्काः);
	release_revisions(&rev);
	module_cb_list_release(&list);
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क module_summary(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
			  संरचना repository *repo अप्रयुक्तम्)
{
	संरचना summary_cb info = SUMMARY_CB_INIT;
	पूर्णाङ्क cached = 0;
	पूर्णाङ्क for_स्थितिः = 0;
	पूर्णाङ्क files = 0;
	पूर्णाङ्क summary_सीमा = -1;
	गणना diff_cmd diff_cmd = DIFF_सूचकांक;
	संरचना object_id शीर्ष_oid;
	पूर्णाङ्क ret;
	संरचना option module_summary_options[] = {
		OPT_BOOL(0, "cached", &cached,
			 N_("use the commit stored in the index instead of the submodule HEAD")),
		OPT_BOOL(0, "files", &files,
			 N_("compare the commit in the index with that in the submodule HEAD")),
		OPT_BOOL(0, "for-status", &for_स्थितिः,
			 N_("skip submodules with 'ignore_config' value set to 'all'")),
		OPT_INTEGER('n', "summary-limit", &summary_सीमा,
			     N_("limit the summary size")),
		OPT_अन्तः()
	};
	ध्रुव वर्ण *ध्रुव git_submodule_helper_usage[] = {
		N_("git submodule summary [<options>] [<commit>] [--] [<path>]"),
		रिक्त
	};

	argc = parse_options(argc, argv, उपसर्गः, module_summary_options,
			     git_submodule_helper_usage, 0);

	यदि (!summary_सीमा)
		प्रत्यागम 0;

	यदि (!repo_get_oid(the_repository, argc ? argv[0] : "HEAD", &शीर्ष_oid)) {
		यदि (argc) {
			argv++;
			argc--;
		}
	} अन्यथा यदि (!argc || !वाक्यं_तुलय(argv[0], "HEAD")) {
		/* before the first commit: compare with an empty tree */
		oidcpy(&शीर्ष_oid, the_hash_algo->रिक्तानि_वृक्षः);
		यदि (argc) {
			argv++;
			argc--;
		}
	} अन्यथा {
		यदि (repo_get_oid(the_repository, "HEAD", &शीर्ष_oid))
			die(_("could not fetch a revision for HEAD"));
	}

	यदि (files) {
		यदि (cached)
			die(_("options '%s' and '%s' cannot be used together"), "--cached", "--files");
		diff_cmd = DIFF_FILES;
	}

	info.argc = argc;
	info.argv = argv;
	info.उपसर्गः = उपसर्गः;
	info.cached = !!cached;
	info.files = !!files;
	info.for_स्थितिः = !!for_स्थितिः;
	info.summary_सीमा = summary_सीमा;

	ret = compute_summary_module_list((diff_cmd == DIFF_सूचकांक) ? &शीर्ष_oid : रिक्त,
					  &info, diff_cmd);
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क module_gitdir(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः अप्रयुक्तम्,
			 संरचना repository *repo)
{
	संरचना strbuf gitdir = STRBUF_INIT;

	यदि (argc != 2)
		usage(_("git submodule--helper gitdir <name>"));

	submodule_name_गन्तव्यम्_gitdir(&gitdir, repo, argv[1]);

	प्रारूप_लिख("%s\n", gitdir.buf);

	strbuf_release(&gitdir);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क module_migrate(पूर्णाङ्क argc अप्रयुक्तम्, ध्रुव वर्ण **argv अप्रयुक्तम्,
			  ध्रुव वर्ण *उपसर्गः अप्रयुक्तम्, संरचना repository *repo)
{
	संरचना strbuf module_dir = STRBUF_INIT;
	DIR *dir;
	संरचना dirent *de;
	पूर्णाङ्क repo_संस्करणम् = 0;

	repo_git_मार्गः_append(repo, &module_dir, "modules/");

	dir = opendir(module_dir.buf);
	यदि (!dir)
		die(_("could not open '%s'"), module_dir.buf);

	यावत् ((de = readdir(dir))) {
		संरचना strbuf gitdir_मार्गः = STRBUF_INIT;
		वर्ण *कुञ्चिका;
		ध्रुव वर्ण *मानम्;

		यदि (is_dot_or_dotdot(de->d_name))
			अनुवर्त;

		strbuf_addf(&gitdir_मार्गः, "%s/%s", module_dir.buf, de->d_name);
		यदि (!is_git_directory(gitdir_मार्गः.buf)) {
			strbuf_release(&gitdir_मार्गः);
			अनुवर्त;
		}
		strbuf_release(&gitdir_मार्गः);

		कुञ्चिका = xstrfmt("submodule.%s.gitdir", de->d_name);
		यदि (!repo_config_get_string_tmp(repo, कुञ्चिका, &मानम्)) {
			/* Already has a gitdir config, nothing to do. */
			मुक्त(कुञ्चिका);
			अनुवर्त;
		}
		मुक्त(कुञ्चिका);

		create_default_gitdir_config(de->d_name);
	}

	closedir(dir);
	strbuf_release(&module_dir);

	repo_config_get_int(the_repository, "core.repositoryformatversion", &repo_संस्करणम्);
	यदि (repo_संस्करणम् == 0 &&
	    repo_config_समुच्चयः_gently(repo, "core.repositoryformatversion", "1"))
		die(_("could not set core.repositoryformatversion to 1.\n"
		      "Please set it for migration to work, for example:\n"
		      "git config core.repositoryformatversion 1"));

	यदि (repo_config_समुच्चयः_gently(repo, "extensions.submodulePathConfig", "true"))
		die(_("could not enable submodulePathConfig extension. It is required\n"
		      "for migration to work. Please enable it in the root repo:\n"
		      "git config extensions.submodulePathConfig true"));

	repo->repository_format_submodule_मार्गः_cfg = 1;

	प्रत्यागम 0;
}

संरचना sync_cb {
	ध्रुव वर्ण *उपसर्गः;
	ध्रुव वर्ण *super_उपसर्गः;
	अचिह्नित पूर्णाङ्क flags;
};
#परिभाषय SYNC_CB_INIT { 0 }

स्थिर शून्य sync_submodule(ध्रुव वर्ण *मार्गः, ध्रुव वर्ण *उपसर्गः,
			   ध्रुव वर्ण *super_उपसर्गः, अचिह्नित पूर्णाङ्क flags)
{
	ध्रुव संरचना submodule *sub;
	वर्ण *remote_कुञ्चिका = रिक्त;
	वर्ण *sub_origin_url, *super_config_url, *displaypath, *default_remote;
	संरचना strbuf sb = STRBUF_INIT;
	वर्ण *sub_config_मार्गः = रिक्त;
	पूर्णाङ्क संहिता;

	यदि (!is_submodule_सक्रिय(the_repository, मार्गः))
		प्रत्यागम;

	यदि (validate_submodule_मार्गः(मार्गः) < 0)
		die(रिक्त);

	sub = submodule_from_मार्गः(the_repository, null_oid(the_hash_algo), मार्गः);

	यदि (sub && sub->url) {
		यदि (starts_with_dot_dot_slash(sub->url) ||
		    starts_with_dot_slash(sub->url)) {
			वर्ण *ऊर्ध्वम्_मार्गः = get_ऊर्ध्वम्_मार्गः(मार्गः);

			sub_origin_url = resolve_relative_url(sub->url, ऊर्ध्वम्_मार्गः, 1);
			super_config_url = resolve_relative_url(sub->url, रिक्त, 1);
			मुक्त(ऊर्ध्वम्_मार्गः);
		} अन्यथा {
			sub_origin_url = xstrdup(sub->url);
			super_config_url = xstrdup(sub->url);
		}
	} अन्यथा {
		sub_origin_url = xstrdup("");
		super_config_url = xstrdup("");
	}

	displaypath = get_submodule_displaypath(मार्गः, उपसर्गः, super_उपसर्गः);

	यदि (!(flags & OPT_QUIET))
		प्रारूप_लिख(_("Synchronizing submodule url for '%s'\n"),
			 displaypath);

	strbuf_reset(&sb);
	strbuf_addf(&sb, "submodule.%s.url", sub->name);
	यदि (repo_config_समुच्चयः_gently(the_repository, sb.buf, super_config_url))
		die(_("failed to register url for submodule path '%s'"),
		      displaypath);

	यदि (!is_submodule_populated_gently(मार्गः, रिक्त))
		गच्छ cleanup;

	strbuf_reset(&sb);
	संहिता = get_default_remote_submodule(मार्गः, &default_remote);
	यदि (संहिता)
		निर्गच्छ(संहिता);

	remote_कुञ्चिका = xstrfmt("remote.%s.url", default_remote);
	मुक्त(default_remote);

	submodule_गन्तव्यम्_gitdir(the_repository, &sb, मार्गः);
	strbuf_addstr(&sb, "/config");

	यदि (repo_config_समुच्चयः_अन्तर्_file_gently(the_repository, sb.buf, remote_कुञ्चिका, रिक्त, sub_origin_url))
		die(_("failed to update remote for submodule '%s'"),
		      मार्गः);

	यदि (flags & OPT_RECURSIVE) {
		संरचना child_process cpr = CHILD_PROCESS_INIT;

		cpr.git_cmd = 1;
		cpr.dir = मार्गः;
		prepare_submodule_repo_env(&cpr.env);

		strvec_pushl(&cpr.तर्काः, "submodule--helper", "sync",
			     "--recursive", रिक्त);
		strvec_pushf(&cpr.तर्काः, "--super-prefix=%s/", displaypath);

		यदि (flags & OPT_QUIET)
			strvec_प्रक्षिप(&cpr.तर्काः, "--quiet");

		यदि (सततम्_command(&cpr))
			die(_("failed to recurse into submodule '%s'"),
			      मार्गः);
	}

cleanup:
	मुक्त(super_config_url);
	मुक्त(sub_origin_url);
	strbuf_release(&sb);
	मुक्त(remote_कुञ्चिका);
	मुक्त(displaypath);
	मुक्त(sub_config_मार्गः);
}

स्थिर शून्य sync_submodule_cb(ध्रुव संरचना cache_प्रविष्टिः *list_item, शून्य *cb_दत्त)
{
	संरचना sync_cb *info = cb_दत्त;

	sync_submodule(list_item->name, info->उपसर्गः, info->super_उपसर्गः,
		       info->flags);
}

स्थिर पूर्णाङ्क module_sync(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
		       संरचना repository *repo अप्रयुक्तम्)
{
	संरचना sync_cb info = SYNC_CB_INIT;
	संरचना pathspec pathspec = { 0 };
	संरचना module_list list = MODULE_LIST_INIT;
	पूर्णाङ्क quiet = 0;
	पूर्णाङ्क recursive = 0;
	संरचना option module_sync_options[] = {
		OPT_SUPER_उपसर्गः(&info.super_उपसर्गः),
		OPT__QUIET(&quiet, N_("suppress output of synchronizing submodule url")),
		OPT_BOOL(0, "recursive", &recursive,
			N_("recurse into nested submodules")),
		OPT_अन्तः()
	};
	ध्रुव वर्ण *ध्रुव git_submodule_helper_usage[] = {
		N_("git submodule sync [--quiet] [--recursive] [<path>]"),
		रिक्त
	};
	पूर्णाङ्क ret = 1;

	argc = parse_options(argc, argv, उपसर्गः, module_sync_options,
			     git_submodule_helper_usage, 0);

	यदि (module_list_compute(argv, उपसर्गः, &pathspec, &list) < 0)
		गच्छ cleanup;

	info.उपसर्गः = उपसर्गः;
	यदि (quiet)
		info.flags |= OPT_QUIET;
	यदि (recursive)
		info.flags |= OPT_RECURSIVE;

	for_each_listed_submodule(&list, sync_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	प्रत्यागम ret;
}

संरचना deinit_cb {
	ध्रुव वर्ण *उपसर्गः;
	अचिह्नित पूर्णाङ्क flags;
};
#परिभाषय DEINIT_CB_INIT { 0 }

स्थिर शून्य deinit_submodule(ध्रुव वर्ण *मार्गः, ध्रुव वर्ण *उपसर्गः,
			     अचिह्नित पूर्णाङ्क flags)
{
	ध्रुव संरचना submodule *sub;
	वर्ण *displaypath = रिक्त;
	संरचना child_process cp_config = CHILD_PROCESS_INIT;
	संरचना strbuf sb_config = STRBUF_INIT;
	वर्ण *sub_git_dir = xstrfmt("%s/.git", मार्गः);

	यदि (validate_submodule_मार्गः(मार्गः) < 0)
		die(रिक्त);

	sub = submodule_from_मार्गः(the_repository, null_oid(the_hash_algo), मार्गः);

	यदि (!sub || !sub->name)
		गच्छ cleanup;

	displaypath = get_submodule_displaypath(मार्गः, उपसर्गः, रिक्त);

	/* remove the submodule work tree (unless the user already did it) */
	यदि (is_directory(मार्गः)) {
		संरचना strbuf sb_rm = STRBUF_INIT;
		ध्रुव वर्ण *format;

		यदि (is_directory(sub_git_dir)) {
			यदि (!(flags & OPT_QUIET))
				warning(_("Submodule work tree '%s' contains a .git "
					  "directory. This will be replaced with a "
					  ".git file by using absorbgitdirs."),
					displaypath);

			absorb_git_dir_into_superproject(मार्गः, रिक्त);

		}

		यदि (!(flags & OPT_FORCE)) {
			संरचना child_process cp_rm = CHILD_PROCESS_INIT;

			cp_rm.git_cmd = 1;
			strvec_pushl(&cp_rm.तर्काः, "rm", "-qn",
				     मार्गः, रिक्त);

			यदि (सततम्_command(&cp_rm))
				die(_("Submodule work tree '%s' contains local "
				      "modifications; use '-f' to discard them"),
				      displaypath);
		}

		strbuf_addstr(&sb_rm, मार्गः);

		यदि (!remove_dir_recursively(&sb_rm, 0))
			format = _("Cleared directory '%s'\n");
		अन्यथा
			format = _("Could not remove submodule work tree '%s'\n");

		यदि (!(flags & OPT_QUIET))
			प्रारूप_लिख(format, displaypath);

		submodule_unset_core_worktree(sub);

		strbuf_release(&sb_rm);
	}

	यदि (mkdir(मार्गः, 0777))
		प्रारूप_लिख(_("could not create empty submodule directory %s"),
		      displaypath);

	cp_config.git_cmd = 1;
	strvec_pushl(&cp_config.तर्काः, "config", "--get-regexp", रिक्त);
	strvec_pushf(&cp_config.तर्काः, "submodule.%s\\.", sub->name);

	/* remove the .git/config entries (unless the user already did it) */
	यदि (!capture_command(&cp_config, &sb_config, 0) && sb_config.len) {
		वर्ण *sub_कुञ्चिका = xstrfmt("submodule.%s", sub->name);

		/*
		 * remove the whole section so we have a clean state when
		 * the user later decides to init this submodule again
		 */
		repo_config_rename_section_अन्तर्_file(the_repository, रिक्त, sub_कुञ्चिका, रिक्त);
		यदि (!(flags & OPT_QUIET))
			प्रारूप_लिख(_("Submodule '%s' (%s) unregistered for path '%s'\n"),
				 sub->name, sub->url, displaypath);
		मुक्त(sub_कुञ्चिका);
	}

cleanup:
	मुक्त(displaypath);
	मुक्त(sub_git_dir);
	strbuf_release(&sb_config);
}

स्थिर शून्य deinit_submodule_cb(ध्रुव संरचना cache_प्रविष्टिः *list_item,
				शून्य *cb_दत्त)
{
	संरचना deinit_cb *info = cb_दत्त;
	deinit_submodule(list_item->name, info->उपसर्गः, info->flags);
}

स्थिर पूर्णाङ्क module_deinit(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
			 संरचना repository *repo अप्रयुक्तम्)
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
		OPT_अन्तः()
	};
	ध्रुव वर्ण *ध्रुव git_submodule_helper_usage[] = {
		N_("git submodule deinit [--quiet] [-f | --force] [--all | [--] [<path>...]]"),
		रिक्त
	};
	पूर्णाङ्क ret = 1;

	argc = parse_options(argc, argv, उपसर्गः, module_deinit_options,
			     git_submodule_helper_usage, 0);

	यदि (all && argc) {
		error("pathspec and --all are incompatible");
		usage_with_options(git_submodule_helper_usage,
				   module_deinit_options);
	}

	यदि (!argc && !all)
		die(_("Use '--all' if you really want to deinitialize all submodules"));

	यदि (module_list_compute(argv, उपसर्गः, &pathspec, &list) < 0)
		गच्छ cleanup;

	info.उपसर्गः = उपसर्गः;
	यदि (quiet)
		info.flags |= OPT_QUIET;
	यदि (force)
		info.flags |= OPT_FORCE;

	for_each_listed_submodule(&list, deinit_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	प्रत्यागम ret;
}

संरचना module_clone_दत्त {
	ध्रुव वर्ण *उपसर्गः;
	ध्रुव वर्ण *मार्गः;
	ध्रुव वर्ण *name;
	ध्रुव वर्ण *url;
	पूर्णाङ्क गभीरता;
	संरचना list_objects_filter_options *filter_options;
	गणना ref_storage_format ref_storage_format;
	अचिह्नित पूर्णाङ्क quiet: 1;
	अचिह्नित पूर्णाङ्क प्रगतिः: 1;
	अचिह्नित पूर्णाङ्क dissociate: 1;
	अचिह्नित पूर्णाङ्क require_init: 1;
	पूर्णाङ्क single_branch;
};
#परिभाषय MODULE_CLONE_दत्त_INIT { \
	.single_branch = -1, \
	.ref_storage_format = REF_STORAGE_FORMAT_UNKNOWN, \
}

संरचना submodule_alternate_सज्जता {
	ध्रुव वर्ण *submodule_name;
	गणना SUBMODULE_ALTERNATE_ERROR_बहुलकम् {
		SUBMODULE_ALTERNATE_ERROR_DIE,
		SUBMODULE_ALTERNATE_ERROR_INFO,
		SUBMODULE_ALTERNATE_ERROR_IGNORE
	} error_बहुलकम्;
	संरचना string_list *reference;
};
#परिभाषय SUBMODULE_ALTERNATE_सज्जता_INIT { \
	.error_बहुलकम् = SUBMODULE_ALTERNATE_ERROR_IGNORE, \
}

स्थिर ध्रुव वर्ण alternate_error_advice[] = N_(
"An alternate computed from a superproject's alternate is invalid.\n"
"To allow Git to clone without an alternate in such a case, set\n"
"submodule.alternateErrorStrategy to 'info' or, equivalently, clone with\n"
"'--reference-if-able' instead of '--reference'."
);

स्थिर पूर्णाङ्क सङ्कलय_possible_reference_from_superproject(
		संरचना odb_files_dir *alt_odb, शून्य *sas_cb)
{
	संरचना submodule_alternate_सज्जता *sas = sas_cb;
	आकार_t len;

	/*
	 * If the alternate object store is another repository, try the
	 * standard layout with .git/(modules/<name>)+/objects
	 */
	यदि (strip_suffix(alt_odb->abspath, "/objects", &len)) {
		संरचना repository alternate;
		वर्ण *sm_alternate;
		संरचना strbuf sb = STRBUF_INIT;
		संरचना strbuf err = STRBUF_INIT;
		strbuf_सङ्कलय(&sb, alt_odb->abspath, len);

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
		submodule_name_गन्तव्यम्_gitdir(&sb, &alternate, sas->submodule_name);
		strbuf_addch(&sb, '/');
		repo_clear(&alternate);

		sm_alternate = compute_alternate_मार्गः(sb.buf, &err);
		यदि (sm_alternate) {
			वर्ण *p = strbuf_detach(&sb, रिक्त);

			string_list_append(sas->reference, p)->util = p;
			मुक्त(sm_alternate);
		} अन्यथा {
			विकल्प (sas->error_बहुलकम्) {
			पक्ष SUBMODULE_ALTERNATE_ERROR_DIE:
				यदि (advice_enabled(ADVICE_SUBMODULE_ALTERNATE_ERROR_STRATEGY_DIE))
					advise(_(alternate_error_advice));
				die(_("submodule '%s' cannot add alternate: %s"),
				    sas->submodule_name, err.buf);
			पक्ष SUBMODULE_ALTERNATE_ERROR_INFO:
				fprintf_ln(stderr, _("submodule '%s' cannot add alternate: %s"),
					sas->submodule_name, err.buf);
			पक्ष SUBMODULE_ALTERNATE_ERROR_IGNORE:
				; /* nothing */
			}
		}

		strbuf_release(&err);
		strbuf_release(&sb);
	}

	प्रत्यागम 0;
}

स्थिर शून्य prepare_possible_alternates(ध्रुव वर्ण *sm_name,
		संरचना string_list *reference)
{
	वर्ण *sm_alternate = रिक्त, *error_strategy = रिक्त;
	संरचना submodule_alternate_सज्जता sas = SUBMODULE_ALTERNATE_सज्जता_INIT;

	repo_config_get_string(the_repository, "submodule.alternateLocation", &sm_alternate);
	यदि (!sm_alternate)
		प्रत्यागम;

	repo_config_get_string(the_repository, "submodule.alternateErrorStrategy", &error_strategy);

	यदि (!error_strategy)
		error_strategy = xstrdup("die");

	sas.submodule_name = sm_name;
	sas.reference = reference;
	यदि (!वाक्यं_तुलय(error_strategy, "die"))
		sas.error_बहुलकम् = SUBMODULE_ALTERNATE_ERROR_DIE;
	अन्यथा यदि (!वाक्यं_तुलय(error_strategy, "info"))
		sas.error_बहुलकम् = SUBMODULE_ALTERNATE_ERROR_INFO;
	अन्यथा यदि (!वाक्यं_तुलय(error_strategy, "ignore"))
		sas.error_बहुलकम् = SUBMODULE_ALTERNATE_ERROR_IGNORE;
	अन्यथा
		die(_("Value '%s' for submodule.alternateErrorStrategy is not recognized"), error_strategy);

	यदि (!वाक्यं_तुलय(sm_alternate, "superproject"))
		odb_for_each_alternate(the_repository->objects,
				       सङ्कलय_possible_reference_from_superproject, &sas);
	अन्यथा यदि (!वाक्यं_तुलय(sm_alternate, "no"))
		; /* do nothing */
	अन्यथा
		die(_("Value '%s' for submodule.alternateLocation is not recognized"), sm_alternate);

	मुक्त(sm_alternate);
	मुक्त(error_strategy);
}

स्थिर वर्ण *clone_submodule_sm_gitdir(ध्रुव वर्ण *name)
{
	संरचना strbuf sb = STRBUF_INIT;
	वर्ण *sm_gitdir;

	submodule_name_गन्तव्यम्_gitdir(&sb, the_repository, name);
	sm_gitdir = absolute_pathdup(sb.buf);
	strbuf_release(&sb);

	प्रत्यागम sm_gitdir;
}

स्थिर पूर्णाङ्क dir_अन्तर्भवति_only_dotgit(ध्रुव वर्ण *मार्गः)
{
	DIR *dir = opendir(मार्गः);
	संरचना dirent *e;
	पूर्णाङ्क ret = 1;

	यदि (!dir)
		प्रत्यागम 0;

	e = readdir_skip_dot_and_dotdot(dir);
	यदि (!e)
		ret = 0;
	अन्यथा यदि (वाक्यं_तुलय(DEFAULT_GIT_DIR_ENVIRONMENT, e->d_name) ||
		 (e = readdir_skip_dot_and_dotdot(dir))) {
		error("unexpected item '%s' in '%s'", e->d_name, मार्गः);
		ret = 0;
	}

	closedir(dir);
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क clone_submodule(ध्रुव संरचना module_clone_दत्त *clone_दत्त,
			   संरचना string_list *reference)
{
	वर्ण *p;
	वर्ण *sm_gitdir = clone_submodule_sm_gitdir(clone_दत्त->name);
	वर्ण *sm_alternate = रिक्त, *error_strategy = रिक्त;
	संरचना stat st;
	संरचना child_process cp = CHILD_PROCESS_INIT;
	ध्रुव वर्ण *clone_दत्त_मार्गः = clone_दत्त->मार्गः;
	वर्ण *गन्तव्यम्_free = रिक्त;

	यदि (validate_submodule_मार्गः(clone_दत्त_मार्गः) < 0)
		die(रिक्त);

	यदि (!is_absolute_मार्गः(clone_दत्त->मार्गः))
		clone_दत्त_मार्गः = गन्तव्यम्_free = xstrfmt("%s/%s", repo_get_work_वृक्षः(the_repository),
						    clone_दत्त->मार्गः);

	यदि (!file_exists(sm_gitdir)) {
		यदि (clone_दत्त->require_init && !stat(clone_दत्त_मार्गः, &st) &&
		    !is_रिक्तानि_dir(clone_दत्त_मार्गः))
			die(_("directory not empty: '%s'"), clone_दत्त_मार्गः);

		यदि (safe_create_leading_directories_const(the_repository, sm_gitdir) < 0)
			die(_("could not create directory '%s'"), sm_gitdir);

		prepare_possible_alternates(clone_दत्त->name, reference);

		strvec_प्रक्षिप(&cp.तर्काः, "clone");
		strvec_प्रक्षिप(&cp.तर्काः, "--no-checkout");
		यदि (clone_दत्त->quiet)
			strvec_प्रक्षिप(&cp.तर्काः, "--quiet");
		यदि (clone_दत्त->प्रगतिः)
			strvec_प्रक्षिप(&cp.तर्काः, "--progress");
		यदि (clone_दत्त->गभीरता > 0)
			strvec_pushf(&cp.तर्काः, "--depth=%d", clone_दत्त->गभीरता);
		यदि (reference->nr) {
			संरचना string_list_item *item;

			for_each_string_list_item(item, reference)
				strvec_pushl(&cp.तर्काः, "--reference",
					     item->string, रिक्त);
		}
		यदि (clone_दत्त->ref_storage_format != REF_STORAGE_FORMAT_UNKNOWN)
			strvec_pushf(&cp.तर्काः, "--ref-storage-format=%s",
				     ref_storage_format_गन्तव्यम्_name(clone_दत्त->ref_storage_format));
		यदि (clone_दत्त->dissociate)
			strvec_प्रक्षिप(&cp.तर्काः, "--dissociate");
		यदि (sm_gitdir && *sm_gitdir)
			strvec_pushl(&cp.तर्काः, "--separate-git-dir", sm_gitdir, रिक्त);
		यदि (clone_दत्त->filter_options && clone_दत्त->filter_options->choice)
			strvec_pushf(&cp.तर्काः, "--filter=%s",
				     expand_list_objects_filter_spec(
					     clone_दत्त->filter_options));
		यदि (clone_दत्त->single_branch >= 0)
			strvec_प्रक्षिप(&cp.तर्काः, clone_दत्त->single_branch ?
				    "--single-branch" :
				    "--no-single-branch");

		strvec_प्रक्षिप(&cp.तर्काः, "--");
		strvec_प्रक्षिप(&cp.तर्काः, clone_दत्त->url);
		strvec_प्रक्षिप(&cp.तर्काः, clone_दत्त_मार्गः);

		cp.git_cmd = 1;
		prepare_submodule_repo_env(&cp.env);
		cp.no_stdin = 1;

		यदि(सततम्_command(&cp))
			die(_("clone of '%s' into submodule path '%s' failed"),
			    clone_दत्त->url, clone_दत्त_मार्गः);

		यदि (clone_दत्त->require_init && !stat(clone_दत्त_मार्गः, &st) &&
		    !dir_अन्तर्भवति_only_dotgit(clone_दत्त_मार्गः)) {
			वर्ण *dot_git = xstrfmt("%s/.git", clone_दत्त_मार्गः);
			unlink(dot_git);
			मुक्त(dot_git);
			die(_("directory not empty: '%s'"), clone_दत्त_मार्गः);
		}
	} अन्यथा {
		वर्ण *मार्गः;

		यदि (clone_दत्त->require_init && !stat(clone_दत्त_मार्गः, &st) &&
		    !is_रिक्तानि_dir(clone_दत्त_मार्गः))
			die(_("directory not empty: '%s'"), clone_दत्त_मार्गः);
		यदि (safe_create_leading_directories_const(the_repository, clone_दत्त_मार्गः) < 0)
			die(_("could not create directory '%s'"), clone_दत्त_मार्गः);
		मार्गः = xstrfmt("%s/index", sm_gitdir);
		unlink_or_warn(मार्गः);
		मुक्त(मार्गः);
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
	यदि (validate_submodule_git_dir(sm_gitdir, clone_दत्त->name) < 0) {
		वर्ण *शीर्ष = xstrfmt("%s/HEAD", sm_gitdir);
		unlink(शीर्ष);
		मुक्त(शीर्ष);
		die(_("refusing to create/use '%s' in another submodule's git dir. "
		      "Enabling extensions.submodulePathConfig should fix this."),
		    sm_gitdir);
	}

	connect_work_वृक्षः_and_git_dir(clone_दत्त_मार्गः, sm_gitdir, 0);

	p = repo_submodule_मार्गः(the_repository, clone_दत्त_मार्गः, "config");
	यदि (!p)
		die(_("could not get submodule directory for '%s'"), clone_दत्त_मार्गः);

	/* setup alternateLocation and alternateErrorStrategy in the cloned submodule if needed */
	repo_config_get_string(the_repository, "submodule.alternateLocation", &sm_alternate);
	यदि (sm_alternate)
		repo_config_समुच्चयः_अन्तर्_file(the_repository, p, "submodule.alternateLocation",
					sm_alternate);
	repo_config_get_string(the_repository, "submodule.alternateErrorStrategy", &error_strategy);
	यदि (error_strategy)
		repo_config_समुच्चयः_अन्तर्_file(the_repository, p, "submodule.alternateErrorStrategy",
					error_strategy);

	मुक्त(sm_alternate);
	मुक्त(error_strategy);

	मुक्त(sm_gitdir);
	मुक्त(p);
	मुक्त(गन्तव्यम्_free);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क module_clone(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
			संरचना repository *repo अप्रयुक्तम्)
{
	पूर्णाङ्क dissociate = 0, quiet = 0, प्रगतिः = 0, require_init = 0;
	संरचना module_clone_दत्त clone_दत्त = MODULE_CLONE_दत्त_INIT;
	संरचना string_list reference = STRING_LIST_INIT_NODUP;
	संरचना list_objects_filter_options filter_options =
		LIST_OBJECTS_FILTER_INIT;
	ध्रुव वर्ण *ref_storage_format = रिक्त;

	संरचना option module_clone_options[] = {
		OPT_STRING(0, "prefix", &clone_दत्त.उपसर्गः,
			   N_("path"),
			   N_("alternative anchor for relative paths")),
		OPT_STRING(0, "path", &clone_दत्त.मार्गः,
			   N_("path"),
			   N_("where the new submodule will be cloned to")),
		OPT_STRING(0, "name", &clone_दत्त.name,
			   N_("string"),
			   N_("name of the new submodule")),
		OPT_STRING(0, "url", &clone_दत्त.url,
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
		OPT_INTEGER(0, "depth", &clone_दत्त.गभीरता,
			   N_("depth for shallow clones")),
		OPT__QUIET(&quiet, "suppress output for cloning a submodule"),
		OPT_BOOL(0, "progress", &प्रगतिः,
			   N_("force cloning progress")),
		OPT_BOOL(0, "require-init", &require_init,
			   N_("disallow cloning into non-empty directory")),
		OPT_BOOL(0, "single-branch", &clone_दत्त.single_branch,
			 N_("clone only one branch, HEAD or --branch")),
		OPT_PARSE_LIST_OBJECTS_FILTER(&filter_options),
		OPT_अन्तः()
	};
	ध्रुव वर्ण *ध्रुव git_submodule_helper_usage[] = {
		N_("git submodule--helper clone [--prefix=<path>] [--quiet] "
		   "[--reference <repository>] [--name <name>] [--depth <depth>] "
		   "[--single-branch] [--filter <filter-spec>] "
		   "--url <url> --path <path>"),
		रिक्त
	};

	argc = parse_options(argc, argv, उपसर्गः, module_clone_options,
			     git_submodule_helper_usage, 0);

	यदि (ref_storage_format) {
		clone_दत्त.ref_storage_format = ref_storage_format_by_name(ref_storage_format);
		यदि (clone_दत्त.ref_storage_format == REF_STORAGE_FORMAT_UNKNOWN)
			die(_("unknown ref storage format '%s'"), ref_storage_format);
	}
	clone_दत्त.dissociate = !!dissociate;
	clone_दत्त.quiet = !!quiet;
	clone_दत्त.प्रगतिः = !!प्रगतिः;
	clone_दत्त.require_init = !!require_init;
	clone_दत्त.filter_options = &filter_options;

	यदि (argc || !clone_दत्त.url || !clone_दत्त.मार्गः || !*(clone_दत्त.मार्गः))
		usage_with_options(git_submodule_helper_usage,
				   module_clone_options);

	clone_submodule(&clone_दत्त, &reference);
	list_objects_filter_release(&filter_options);
	string_list_clear(&reference, 1);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क determine_submodule_update_strategy(संरचना repository *r,
					       पूर्णाङ्क just_cloned,
					       ध्रुव वर्ण *मार्गः,
					       गणना submodule_update_type update,
					       संरचना submodule_update_strategy *बहिर्गमनम्)
{
	ध्रुव संरचना submodule *sub = submodule_from_मार्गः(r, null_oid(the_hash_algo), मार्गः);
	वर्ण *कुञ्चिका;
	ध्रुव वर्ण *val;
	पूर्णाङ्क ret;

	/*
	 * NEEDSWORK: audit and ensure that update_submodule() has right
	 * to assume that submodule_from_path() above will always succeed.
	 */
	यदि (!sub)
		BUG("update_submodule assumes a submodule exists at path (%s)",
		    मार्गः);
	कुञ्चिका = xstrfmt("submodule.%s.update", sub->name);

	यदि (update) {
		बहिर्गमनम्->type = update;
	} अन्यथा यदि (!repo_config_get_string_tmp(r, कुञ्चिका, &val)) {
		यदि (parse_submodule_update_strategy(val, बहिर्गमनम्) < 0) {
			ret = die_सन्देशः(_("Invalid update mode '%s' configured for submodule path '%s'"),
					  val, मार्गः);
			गच्छ cleanup;
		}
	} अन्यथा यदि (sub->update_strategy.type != SM_UPDATE_UNSPECIFIED) {
		यदि (sub->update_strategy.type == SM_UPDATE_COMMAND)
			BUG("how did we read update = !command from .gitmodules?");
		बहिर्गमनम्->type = sub->update_strategy.type;
		बहिर्गमनम्->command = sub->update_strategy.command;
	} अन्यथा
		बहिर्गमनम्->type = SM_UPDATE_CHECKOUT;

	यदि (just_cloned &&
	    (बहिर्गमनम्->type == SM_UPDATE_MERGE ||
	     बहिर्गमनम्->type == SM_UPDATE_REBASE ||
	     बहिर्गमनम्->type == SM_UPDATE_NONE))
		बहिर्गमनम्->type = SM_UPDATE_CHECKOUT;

	ret = 0;
cleanup:
	मुक्त(कुञ्चिका);
	प्रत्यागम ret;
}

संरचना update_clone_दत्त {
	ध्रुव संरचना submodule *sub;
	संरचना object_id oid;
	अचिह्नित just_cloned;
};

संरचना submodule_update_clone {
	/* index into 'update_data.list', the list of submodules to look into for cloning */
	पूर्णाङ्क current;

	/* configuration parameters which are passed on to the children */
	ध्रुव संरचना update_दत्त *update_दत्त;

	/* to be consumed by update_submodule() */
	संरचना update_clone_दत्त *update_clone;
	पूर्णाङ्क update_clone_nr; पूर्णाङ्क update_clone_alloc;

	/* If we want to stop as fast as possible and return an error */
	अचिह्नित quickstop : 1;

	/* failed clones to be retried again */
	ध्रुव संरचना cache_प्रविष्टिः **failed_clones;
	पूर्णाङ्क failed_clones_nr, failed_clones_alloc;
};
#परिभाषय SUBMODULE_UPDATE_CLONE_INIT { 0 }

स्थिर शून्य submodule_update_clone_release(संरचना submodule_update_clone *suc)
{
	मुक्त(suc->update_clone);
	मुक्त(suc->failed_clones);
}

संरचना update_दत्त {
	ध्रुव वर्ण *उपसर्गः;
	ध्रुव वर्ण *super_उपसर्गः;
	वर्ण *displaypath;
	गणना submodule_update_type update_default;
	संरचना object_id suboid;
	संरचना string_list references;
	संरचना submodule_update_strategy update_strategy;
	संरचना list_objects_filter_options *filter_options;
	संरचना module_list list;
	गणना ref_storage_format ref_storage_format;
	पूर्णाङ्क गभीरता;
	पूर्णाङ्क max_jobs;
	पूर्णाङ्क single_branch;
	पूर्णाङ्क recommend_shallow;
	अचिह्नित पूर्णाङ्क require_init;
	अचिह्नित पूर्णाङ्क force;
	अचिह्नित पूर्णाङ्क quiet;
	अचिह्नित पूर्णाङ्क nofetch;
	अचिह्नित पूर्णाङ्क remote;
	अचिह्नित पूर्णाङ्क प्रगतिः;
	अचिह्नित पूर्णाङ्क dissociate;
	अचिह्नित पूर्णाङ्क init;
	अचिह्नित पूर्णाङ्क warn_if_uninitialized;
	अचिह्नित पूर्णाङ्क recursive;

	/* copied over from update_clone_data */
	संरचना object_id oid;
	अचिह्नित पूर्णाङ्क just_cloned;
	ध्रुव वर्ण *sm_मार्गः;
};
#परिभाषय UPDATE_दत्त_INIT { \
	.update_strategy = SUBMODULE_UPDATE_STRATEGY_INIT, \
	.list = MODULE_LIST_INIT, \
	.ref_storage_format = REF_STORAGE_FORMAT_UNKNOWN, \
	.recommend_shallow = -1, \
	.references = STRING_LIST_INIT_DUP, \
	.single_branch = -1, \
	.max_jobs = 1, \
}

स्थिर शून्य update_दत्त_release(संरचना update_दत्त *ud)
{
	मुक्त(ud->displaypath);
	submodule_update_strategy_release(&ud->update_strategy);
	module_list_release(&ud->list);
}

स्थिर शून्य अग्र_submodule_warn_missing(संरचना submodule_update_clone *suc,
		संरचना strbuf *बहिर्गमनम्, ध्रुव वर्ण *displaypath)
{
	/*
	 * Only mention uninitialized submodules when their
	 * paths have been specified.
	 */
	यदि (suc->update_दत्त->warn_if_uninitialized) {
		strbuf_addf(बहिर्गमनम्,
			_("Submodule path '%s' not initialized"),
			displaypath);
		strbuf_addch(बहिर्गमनम्, '\n');
		strbuf_addstr(बहिर्गमनम्,
			_("Maybe you want to use 'update --init'?"));
		strbuf_addch(बहिर्गमनम्, '\n');
	}
}

/**
 * Determine whether 'ce' needs to be cloned. If so, prepare the 'child' to
 * run the clone. Returns 1 if 'ce' needs to be cloned, 0 otherwise.
 */
स्थिर पूर्णाङ्क prepare_गन्तव्यम्_clone_अग्र_submodule(ध्रुव संरचना cache_प्रविष्टिः *ce,
					   संरचना child_process *child,
					   संरचना submodule_update_clone *suc,
					   संरचना strbuf *बहिर्गमनम्)
{
	ध्रुव संरचना submodule *sub = रिक्त;
	ध्रुव वर्ण *url = रिक्त;
	ध्रुव वर्ण *update_string;
	गणना submodule_update_type update_type;
	वर्ण *कुञ्चिका;
	ध्रुव संरचना update_दत्त *ud = suc->update_दत्त;
	वर्ण *displaypath = get_submodule_displaypath(ce->name, ud->उपसर्गः,
						      ud->super_उपसर्गः);
	संरचना strbuf sb = STRBUF_INIT;
	पूर्णाङ्क needs_cloning = 0;
	पूर्णाङ्क need_free_url = 0;

	यदि (ce_stage(ce)) {
		strbuf_addf(बहिर्गमनम्, _("Skipping unmerged submodule %s"), displaypath);
		strbuf_addch(बहिर्गमनम्, '\n');
		गच्छ cleanup;
	}

	sub = submodule_from_मार्गः(the_repository, null_oid(the_hash_algo), ce->name);

	यदि (!sub) {
		अग्र_submodule_warn_missing(suc, बहिर्गमनम्, displaypath);
		गच्छ cleanup;
	}

	कुञ्चिका = xstrfmt("submodule.%s.update", sub->name);
	यदि (!repo_config_get_string_tmp(the_repository, कुञ्चिका, &update_string)) {
		update_type = parse_submodule_update_type(update_string);
	} अन्यथा {
		update_type = sub->update_strategy.type;
	}
	मुक्त(कुञ्चिका);

	यदि (suc->update_दत्त->update_strategy.type == SM_UPDATE_NONE
	    || (suc->update_दत्त->update_strategy.type == SM_UPDATE_UNSPECIFIED
		&& update_type == SM_UPDATE_NONE)) {
		strbuf_addf(बहिर्गमनम्, _("Skipping submodule '%s'"), displaypath);
		strbuf_addch(बहिर्गमनम्, '\n');
		गच्छ cleanup;
	}

	/* Check if the submodule has been initialized. */
	यदि (!is_submodule_सक्रिय(the_repository, ce->name)) {
		अग्र_submodule_warn_missing(suc, बहिर्गमनम्, displaypath);
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
	child->stdout_गन्तव्यम्_stderr = 1;
	child->err = -1;
	strvec_प्रक्षिप(&child->तर्काः, "submodule--helper");
	strvec_प्रक्षिप(&child->तर्काः, "clone");
	यदि (suc->update_दत्त->प्रगतिः)
		strvec_प्रक्षिप(&child->तर्काः, "--progress");
	यदि (suc->update_दत्त->quiet)
		strvec_प्रक्षिप(&child->तर्काः, "--quiet");
	यदि (suc->update_दत्त->उपसर्गः)
		strvec_pushl(&child->तर्काः, "--prefix", suc->update_दत्त->उपसर्गः, रिक्त);
	यदि (suc->update_दत्त->recommend_shallow && sub->recommend_shallow == 1)
		strvec_प्रक्षिप(&child->तर्काः, "--depth=1");
	अन्यथा यदि (suc->update_दत्त->गभीरता)
		strvec_pushf(&child->तर्काः, "--depth=%d", suc->update_दत्त->गभीरता);
	यदि (suc->update_दत्त->filter_options && suc->update_दत्त->filter_options->choice)
		strvec_pushf(&child->तर्काः, "--filter=%s",
			     expand_list_objects_filter_spec(suc->update_दत्त->filter_options));
	यदि (suc->update_दत्त->require_init)
		strvec_प्रक्षिप(&child->तर्काः, "--require-init");
	यदि (suc->update_दत्त->ref_storage_format != REF_STORAGE_FORMAT_UNKNOWN)
		strvec_pushf(&child->तर्काः, "--ref-storage-format=%s",
			     ref_storage_format_गन्तव्यम्_name(suc->update_दत्त->ref_storage_format));
	strvec_pushl(&child->तर्काः, "--path", sub->मार्गः, रिक्त);
	strvec_pushl(&child->तर्काः, "--name", sub->name, रिक्त);
	strvec_pushl(&child->तर्काः, "--url", url, रिक्त);
	यदि (suc->update_दत्त->references.nr) {
		संरचना string_list_item *item;

		for_each_string_list_item(item, &suc->update_दत्त->references)
			strvec_pushl(&child->तर्काः, "--reference", item->string, रिक्त);
	}
	यदि (suc->update_दत्त->dissociate)
		strvec_प्रक्षिप(&child->तर्काः, "--dissociate");
	यदि (suc->update_दत्त->single_branch >= 0)
		strvec_प्रक्षिप(&child->तर्काः, suc->update_दत्त->single_branch ?
					      "--single-branch" :
					      "--no-single-branch");

cleanup:
	मुक्त(displaypath);
	strbuf_release(&sb);
	यदि (need_free_url)
		मुक्त((शून्य*)url);

	प्रत्यागम needs_cloning;
}

स्थिर पूर्णाङ्क update_clone_get_अग्र_task(संरचना child_process *child,
				      संरचना strbuf *err,
				      शून्य *suc_cb,
				      शून्य **idx_task_cb)
{
	संरचना submodule_update_clone *suc = suc_cb;
	ध्रुव संरचना cache_प्रविष्टिः *ce;
	पूर्णाङ्क सूचकांक;

	प्रति (; suc->current < suc->update_दत्त->list.nr; suc->current++) {
		ce = suc->update_दत्त->list.entries[suc->current];
		यदि (prepare_गन्तव्यम्_clone_अग्र_submodule(ce, child, suc, err)) {
			पूर्णाङ्क *p = xmalloc(परिमाण(*p));

			*p = suc->current;
			*idx_task_cb = p;
			suc->current++;
			प्रत्यागम 1;
		}
	}

	/*
	 * The loop above tried cloning each submodule once, now try the
	 * stragglers again, which we can imagine as an extension of the
	 * entry list.
	 */
	सूचकांक = suc->current - suc->update_दत्त->list.nr;
	यदि (सूचकांक < suc->failed_clones_nr) {
		पूर्णाङ्क *p;

		ce = suc->failed_clones[सूचकांक];
		यदि (!prepare_गन्तव्यम्_clone_अग्र_submodule(ce, child, suc, err)) {
			suc->current ++;
			strbuf_addstr(err, "BUG: submodule considered for "
					   "cloning, doesn't need cloning "
					   "any more?\n");
			प्रत्यागम 0;
		}
		p = xmalloc(परिमाण(*p));
		*p = suc->current;
		*idx_task_cb = p;
		suc->current ++;
		प्रत्यागम 1;
	}

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क update_clone_start_failure(संरचना strbuf *err अप्रयुक्तम्,
				      शून्य *suc_cb,
				      शून्य *idx_task_cb अप्रयुक्तम्)
{
	संरचना submodule_update_clone *suc = suc_cb;

	suc->quickstop = 1;
	प्रत्यागम 1;
}

स्थिर पूर्णाङ्क update_clone_task_परिसमाप्तम्(पूर्णाङ्क परिणाम,
				      संरचना strbuf *err,
				      शून्य *suc_cb,
				      शून्य *idx_task_cb)
{
	ध्रुव संरचना cache_प्रविष्टिः *ce;
	संरचना submodule_update_clone *suc = suc_cb;
	पूर्णाङ्क *idxP = idx_task_cb;
	पूर्णाङ्क idx = *idxP;

	मुक्त(idxP);

	यदि (!परिणाम)
		प्रत्यागम 0;

	यदि (idx < suc->update_दत्त->list.nr) {
		ce  = suc->update_दत्त->list.entries[idx];
		strbuf_addf(err, _("Failed to clone '%s'. Retry scheduled"),
			    ce->name);
		strbuf_addch(err, '\n');
		ALLOC_GROW(suc->failed_clones,
			   suc->failed_clones_nr + 1,
			   suc->failed_clones_alloc);
		suc->failed_clones[suc->failed_clones_nr++] = ce;
		प्रत्यागम 0;
	} अन्यथा {
		idx -= suc->update_दत्त->list.nr;
		ce  = suc->failed_clones[idx];
		strbuf_addf(err, _("Failed to clone '%s' a second time, aborting"),
			    ce->name);
		strbuf_addch(err, '\n');
		suc->quickstop = 1;
		प्रत्यागम 1;
	}

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क git_update_clone_config(ध्रुव वर्ण *var, ध्रुव वर्ण *मानम्,
				   ध्रुव संरचना config_context *ctx,
				   शून्य *cb)
{
	पूर्णाङ्क *max_jobs = cb;

	यदि (!वाक्यं_तुलय(var, "submodule.fetchjobs"))
		*max_jobs = parse_submodule_fetchjobs(var, मानम्, ctx->kvi);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क is_tip_reachable(ध्रुव वर्ण *मार्गः, ध्रुव संरचना object_id *oid)
{
	संरचना child_process cp = CHILD_PROCESS_INIT;
	संरचना strbuf rev = STRBUF_INIT;
	वर्ण *hex = oid_गन्तव्यम्_hex(oid);
	पूर्णाङ्क reachable;

	cp.git_cmd = 1;
	cp.dir = मार्गः;
	cp.no_stderr = 1;
	strvec_pushl(&cp.तर्काः, "rev-list", "-n", "1", hex, "--not", "--all", रिक्त);

	prepare_submodule_repo_env(&cp.env);

	यदि (capture_command(&cp, &rev, GIT_MAX_HEXSZ + 1) || rev.len)
		reachable = 0;
	अन्यथा
		reachable = 1;

	strbuf_release(&rev);
	प्रत्यागम reachable;
}

स्थिर पूर्णाङ्क fetch_अन्तर्_submodule(ध्रुव वर्ण *module_मार्गः, पूर्णाङ्क गभीरता, पूर्णाङ्क quiet,
			      ध्रुव संरचना object_id *oid)
{
	संरचना child_process cp = CHILD_PROCESS_INIT;

	prepare_submodule_repo_env(&cp.env);
	cp.git_cmd = 1;
	cp.dir = module_मार्गः;

	strvec_प्रक्षिप(&cp.तर्काः, "fetch");
	यदि (quiet)
		strvec_प्रक्षिप(&cp.तर्काः, "--quiet");
	यदि (गभीरता)
		strvec_pushf(&cp.तर्काः, "--depth=%d", गभीरता);
	यदि (oid) {
		वर्ण *hex = oid_गन्तव्यम्_hex(oid);
		वर्ण *remote;
		पूर्णाङ्क संहिता;

		संहिता = get_default_remote_submodule(module_मार्गः, &remote);
		यदि (संहिता) {
			child_process_clear(&cp);
			प्रत्यागम संहिता;
		}

		strvec_pushl(&cp.तर्काः, remote, hex, रिक्त);
		मुक्त(remote);
	}

	प्रत्यागम सततम्_command(&cp);
}

स्थिर पूर्णाङ्क सततम्_update_command(ध्रुव संरचना update_दत्त *ud, पूर्णाङ्क subforce)
{
	संरचना child_process cp = CHILD_PROCESS_INIT;
	वर्ण *oid = oid_गन्तव्यम्_hex(&ud->oid);
	पूर्णाङ्क ret;

	विकल्प (ud->update_strategy.type) {
	पक्ष SM_UPDATE_CHECKOUT:
		cp.git_cmd = 1;
		strvec_pushl(&cp.तर्काः, "checkout", "-q", रिक्त);
		यदि (subforce)
			strvec_प्रक्षिप(&cp.तर्काः, "-f");
		भङ्ग;
	पक्ष SM_UPDATE_REBASE:
		cp.git_cmd = 1;
		strvec_प्रक्षिप(&cp.तर्काः, "rebase");
		यदि (ud->quiet)
			strvec_प्रक्षिप(&cp.तर्काः, "--quiet");
		भङ्ग;
	पक्ष SM_UPDATE_MERGE:
		cp.git_cmd = 1;
		strvec_प्रक्षिप(&cp.तर्काः, "merge");
		यदि (ud->quiet)
			strvec_प्रक्षिप(&cp.तर्काः, "--quiet");
		भङ्ग;
	पक्ष SM_UPDATE_COMMAND:
		cp.use_shell = 1;
		strvec_प्रक्षिप(&cp.तर्काः, ud->update_strategy.command);
		भङ्ग;
	स्वभाव:
		BUG("unexpected update strategy type: %d",
		    ud->update_strategy.type);
	}
	strvec_प्रक्षिप(&cp.तर्काः, oid);

	cp.dir = ud->sm_मार्गः;
	prepare_submodule_repo_env(&cp.env);
	यदि ((ret = सततम्_command(&cp))) {
		विकल्प (ud->update_strategy.type) {
		पक्ष SM_UPDATE_CHECKOUT:
			die_सन्देशः(_("Unable to checkout '%s' in submodule path '%s'"),
				    oid, ud->displaypath);
			/* No "ret" assignment, use "git checkout"'s */
			भङ्ग;
		पक्ष SM_UPDATE_REBASE:
			ret = die_सन्देशः(_("Unable to rebase '%s' in submodule path '%s'"),
					  oid, ud->displaypath);
			भङ्ग;
		पक्ष SM_UPDATE_MERGE:
			ret = die_सन्देशः(_("Unable to merge '%s' in submodule path '%s'"),
					  oid, ud->displaypath);
			भङ्ग;
		पक्ष SM_UPDATE_COMMAND:
			ret = die_सन्देशः(_("Execution of '%s %s' failed in submodule path '%s'"),
					  ud->update_strategy.command, oid, ud->displaypath);
			भङ्ग;
		स्वभाव:
			BUG("unexpected update strategy type: %d",
			    ud->update_strategy.type);
		}

		प्रत्यागम ret;
	}

	यदि (ud->quiet)
		प्रत्यागम 0;

	विकल्प (ud->update_strategy.type) {
	पक्ष SM_UPDATE_CHECKOUT:
		प्रारूप_लिख(_("Submodule path '%s': checked out '%s'\n"),
		       ud->displaypath, oid);
		भङ्ग;
	पक्ष SM_UPDATE_REBASE:
		प्रारूप_लिख(_("Submodule path '%s': rebased into '%s'\n"),
		       ud->displaypath, oid);
		भङ्ग;
	पक्ष SM_UPDATE_MERGE:
		प्रारूप_लिख(_("Submodule path '%s': merged in '%s'\n"),
		       ud->displaypath, oid);
		भङ्ग;
	पक्ष SM_UPDATE_COMMAND:
		प्रारूप_लिख(_("Submodule path '%s': '%s %s'\n"),
		       ud->displaypath, ud->update_strategy.command, oid);
		भङ्ग;
	स्वभाव:
		BUG("unexpected update strategy type: %d",
		    ud->update_strategy.type);
	}

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क सततम्_update_procedure(ध्रुव संरचना update_दत्त *ud)
{
	पूर्णाङ्क subforce = is_null_oid(&ud->suboid) || ud->force;

	यदि (!ud->nofetch) {
		/*
		 * Run fetch only if `oid` isn't present or it
		 * is not reachable from a ref.
		 */
		यदि (!is_tip_reachable(ud->sm_मार्गः, &ud->oid) &&
		    fetch_अन्तर्_submodule(ud->sm_मार्गः, ud->गभीरता, ud->quiet, रिक्त) &&
		    !ud->quiet)
			fprintf_ln(stderr,
				   _("Unable to fetch in submodule path '%s'; "
				     "trying to directly fetch %s:"),
				   ud->displaypath, oid_गन्तव्यम्_hex(&ud->oid));
		/*
		 * Now we tried the usual fetch, but `oid` may
		 * not be reachable from any of the refs.
		 */
		यदि (!is_tip_reachable(ud->sm_मार्गः, &ud->oid) &&
		    fetch_अन्तर्_submodule(ud->sm_मार्गः, ud->गभीरता, ud->quiet, &ud->oid))
			प्रत्यागम die_सन्देशः(_("Fetched in submodule path '%s', but it did not "
					     "contain %s. Direct fetching of that commit failed."),
					   ud->displaypath, oid_गन्तव्यम्_hex(&ud->oid));
	}

	प्रत्यागम सततम्_update_command(ud, subforce);
}

स्थिर पूर्णाङ्क remote_submodule_branch(ध्रुव वर्ण *मार्गः, ध्रुव वर्ण **branch)
{
	ध्रुव संरचना submodule *sub;
	वर्ण *कुञ्चिका;
	*branch = रिक्त;

	sub = submodule_from_मार्गः(the_repository, null_oid(the_hash_algo), मार्गः);
	यदि (!sub)
		प्रत्यागम die_सन्देशः(_("could not initialize submodule at path '%s'"),
				   मार्गः);

	कुञ्चिका = xstrfmt("submodule.%s.branch", sub->name);
	यदि (repo_config_get_string_tmp(the_repository, कुञ्चिका, branch))
		*branch = sub->branch;
	मुक्त(कुञ्चिका);

	यदि (!*branch) {
		*branch = "HEAD";
		प्रत्यागम 0;
	}

	यदि (!वाक्यं_तुलय(*branch, ".")) {
		ध्रुव वर्ण *refname = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
							      "HEAD", 0, रिक्त,
							      रिक्त);

		यदि (!refname)
			प्रत्यागम die_सन्देशः(_("No such ref: %s"), "HEAD");

		/* detached HEAD */
		यदि (!वाक्यं_तुलय(refname, "HEAD"))
			प्रत्यागम die_सन्देशः(_("Submodule (%s) branch configured to inherit "
					     "branch from superproject, but the superproject "
					     "is not on any branch"), sub->name);

		यदि (!skip_उपसर्गः(refname, "refs/heads/", &refname))
			प्रत्यागम die_सन्देशः(_("Expecting a full ref name, got %s"),
					   refname);

		*branch = refname;
		प्रत्यागम 0;
	}

	/* Our "branch" is coming from repo_config_get_string_tmp() */
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क ensure_core_worktree(ध्रुव वर्ण *मार्गः)
{
	ध्रुव वर्ण *cw;
	संरचना repository subrepo;

	यदि (repo_submodule_init(&subrepo, the_repository, मार्गः, null_oid(the_hash_algo)))
		प्रत्यागम die_सन्देशः(_("could not get a repository handle for submodule '%s'"),
				   मार्गः);

	यदि (!repo_config_get_string_tmp(&subrepo, "core.worktree", &cw)) {
		वर्ण *cfg_file, *abs_मार्गः;
		ध्रुव वर्ण *rel_मार्गः;
		संरचना strbuf sb = STRBUF_INIT;

		cfg_file = repo_git_मार्गः(&subrepo, "config");

		abs_मार्गः = absolute_pathdup(मार्गः);
		rel_मार्गः = relative_मार्गः(abs_मार्गः, subrepo.gitdir, &sb);

		repo_config_समुच्चयः_अन्तर्_file(the_repository, cfg_file, "core.worktree", rel_मार्गः);

		मुक्त(cfg_file);
		मुक्त(abs_मार्गः);
		strbuf_release(&sb);
	}

	repo_clear(&subrepo);
	प्रत्यागम 0;
}

स्थिर ध्रुव वर्ण *submodule_update_type_गन्तव्यम्_label(गणना submodule_update_type type)
{
	विकल्प (type) {
	पक्ष SM_UPDATE_CHECKOUT:
		प्रत्यागम "checkout";
	पक्ष SM_UPDATE_MERGE:
		प्रत्यागम "merge";
	पक्ष SM_UPDATE_REBASE:
		प्रत्यागम "rebase";
	पक्ष SM_UPDATE_UNSPECIFIED:
	पक्ष SM_UPDATE_NONE:
	पक्ष SM_UPDATE_COMMAND:
		भङ्ग;
	}
	BUG("unreachable with type %d", type);
}

स्थिर शून्य update_दत्त_गन्तव्यम्_तर्काः(ध्रुव संरचना update_दत्त *update_दत्त,
				संरचना strvec *तर्काः)
{
	गणना submodule_update_type update_type = update_दत्त->update_default;

	strvec_pushl(तर्काः, "submodule--helper", "update", "--recursive", रिक्त);
	यदि (update_दत्त->displaypath)
		strvec_pushf(तर्काः, "--super-prefix=%s/",
			     update_दत्त->displaypath);
	strvec_pushf(तर्काः, "--jobs=%d", update_दत्त->max_jobs);
	यदि (update_दत्त->quiet)
		strvec_प्रक्षिप(तर्काः, "--quiet");
	यदि (update_दत्त->force)
		strvec_प्रक्षिप(तर्काः, "--force");
	यदि (update_दत्त->init)
		strvec_प्रक्षिप(तर्काः, "--init");
	यदि (update_दत्त->remote)
		strvec_प्रक्षिप(तर्काः, "--remote");
	यदि (update_दत्त->nofetch)
		strvec_प्रक्षिप(तर्काः, "--no-fetch");
	यदि (update_दत्त->dissociate)
		strvec_प्रक्षिप(तर्काः, "--dissociate");
	यदि (update_दत्त->प्रगतिः)
		strvec_प्रक्षिप(तर्काः, "--progress");
	यदि (update_दत्त->require_init)
		strvec_प्रक्षिप(तर्काः, "--require-init");
	यदि (update_दत्त->गभीरता)
		strvec_pushf(तर्काः, "--depth=%d", update_दत्त->गभीरता);
	यदि (update_type != SM_UPDATE_UNSPECIFIED)
		strvec_pushf(तर्काः, "--%s",
			     submodule_update_type_गन्तव्यम्_label(update_type));

	यदि (update_दत्त->references.nr) {
		संरचना string_list_item *item;

		for_each_string_list_item(item, &update_दत्त->references)
			strvec_pushl(तर्काः, "--reference", item->string, रिक्त);
	}
	यदि (update_दत्त->ref_storage_format != REF_STORAGE_FORMAT_UNKNOWN)
		strvec_pushf(तर्काः, "--ref-storage-format=%s",
			     ref_storage_format_गन्तव्यम्_name(update_दत्त->ref_storage_format));
	यदि (update_दत्त->filter_options && update_दत्त->filter_options->choice)
		strvec_pushf(तर्काः, "--filter=%s",
				expand_list_objects_filter_spec(
					update_दत्त->filter_options));
	यदि (update_दत्त->recommend_shallow == 0)
		strvec_प्रक्षिप(तर्काः, "--no-recommend-shallow");
	अन्यथा यदि (update_दत्त->recommend_shallow == 1)
		strvec_प्रक्षिप(तर्काः, "--recommend-shallow");
	यदि (update_दत्त->single_branch >= 0)
		strvec_प्रक्षिप(तर्काः, update_दत्त->single_branch ?
				    "--single-branch" :
				    "--no-single-branch");
}

स्थिर पूर्णाङ्क update_submodule(संरचना update_दत्त *update_दत्त)
{
	पूर्णाङ्क ret;

	यदि (validate_submodule_मार्गः(update_दत्त->sm_मार्गः) < 0)
		प्रत्यागम -1;

	ret = determine_submodule_update_strategy(the_repository,
						  update_दत्त->just_cloned,
						  update_दत्त->sm_मार्गः,
						  update_दत्त->update_default,
						  &update_दत्त->update_strategy);
	यदि (ret)
		प्रत्यागम ret;

	यदि (update_दत्त->just_cloned)
		oidcpy(&update_दत्त->suboid, null_oid(the_hash_algo));
	अन्यथा यदि (repo_resolve_gitlink_ref(the_repository, update_दत्त->sm_मार्गः,
					  "HEAD", &update_दत्त->suboid))
		प्रत्यागम die_सन्देशः(_("Unable to find current revision in submodule path '%s'"),
				   update_दत्त->displaypath);

	यदि (update_दत्त->remote) {
		वर्ण *remote_name;
		ध्रुव वर्ण *branch;
		वर्ण *remote_ref;
		पूर्णाङ्क संहिता;

		संहिता = get_default_remote_submodule(update_दत्त->sm_मार्गः, &remote_name);
		यदि (संहिता)
			प्रत्यागम संहिता;
		संहिता = remote_submodule_branch(update_दत्त->sm_मार्गः, &branch);
		यदि (संहिता) {
			मुक्त(remote_name);
			प्रत्यागम संहिता;
		}
		remote_ref = xstrfmt("refs/remotes/%s/%s", remote_name, branch);

		मुक्त(remote_name);

		यदि (!update_दत्त->nofetch) {
			यदि (fetch_अन्तर्_submodule(update_दत्त->sm_मार्गः, update_दत्त->गभीरता,
					      0, रिक्त)) {
				मुक्त(remote_ref);
				प्रत्यागम die_सन्देशः(_("Unable to fetch in submodule path '%s'"),
						   update_दत्त->sm_मार्गः);
			}
		}

		यदि (repo_resolve_gitlink_ref(the_repository, update_दत्त->sm_मार्गः,
					     remote_ref, &update_दत्त->oid)) {
			ret = die_सन्देशः(_("Unable to find %s revision in submodule path '%s'"),
					  remote_ref, update_दत्त->sm_मार्गः);
			मुक्त(remote_ref);
			प्रत्यागम ret;
		}

		मुक्त(remote_ref);
	}

	यदि (!oideq(&update_दत्त->oid, &update_दत्त->suboid) || update_दत्त->force) {
		ret = सततम्_update_procedure(update_दत्त);
		यदि (ret)
			प्रत्यागम ret;
	}

	यदि (update_दत्त->recursive) {
		संरचना child_process cp = CHILD_PROCESS_INIT;
		संरचना update_दत्त अग्र = *update_दत्त;

		अग्र.उपसर्गः = रिक्त;
		oidcpy(&अग्र.oid, null_oid(the_hash_algo));
		oidcpy(&अग्र.suboid, null_oid(the_hash_algo));

		cp.dir = update_दत्त->sm_मार्गः;
		cp.git_cmd = 1;
		prepare_submodule_repo_env(&cp.env);
		update_दत्त_गन्तव्यम्_तर्काः(&अग्र, &cp.तर्काः);

		ret = सततम्_command(&cp);
		यदि (ret)
			die_सन्देशः(_("Failed to recurse into submodule path '%s'"),
				    update_दत्त->displaypath);
		प्रत्यागम ret;
	}

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क update_submodules(संरचना update_दत्त *update_दत्त)
{
	पूर्णाङ्क i, ret = 0;
	संरचना submodule_update_clone suc = SUBMODULE_UPDATE_CLONE_INIT;
	ध्रुव संरचना सततम्_process_parallel_opts opts = {
		.tr2_category = "submodule",
		.tr2_label = "parallel/update",

		.processes = update_दत्त->max_jobs,

		.get_अग्र_task = update_clone_get_अग्र_task,
		.start_failure = update_clone_start_failure,
		.task_परिसमाप्तम् = update_clone_task_परिसमाप्तम्,
		.दत्त = &suc,
	};

	suc.update_दत्त = update_दत्त;
	सततम्_processes_parallel(&opts);

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

	प्रति (i = 0; i < suc.update_clone_nr; i++) {
		संरचना update_clone_दत्त ucd = suc.update_clone[i];
		पूर्णाङ्क संहिता = 128;

		oidcpy(&update_दत्त->oid, &ucd.oid);
		update_दत्त->just_cloned = ucd.just_cloned;
		update_दत्त->sm_मार्गः = ucd.sub->मार्गः;

		/*
		 * Verify that the submodule path does not contain any
		 * symlinks; if it does, it might have been tampered with.
		 * TODO: allow exempting it via
		 * `safe.submodule.path` or something
		 */
		यदि (validate_submodule_मार्गः(update_दत्त->sm_मार्गः) < 0)
			गच्छ fail;

		संहिता = ensure_core_worktree(update_दत्त->sm_मार्गः);
		यदि (संहिता)
			गच्छ fail;

		update_दत्त->displaypath = get_submodule_displaypath(
			update_दत्त->sm_मार्गः, update_दत्त->उपसर्गः,
			update_दत्त->super_उपसर्गः);
		संहिता = update_submodule(update_दत्त);
		FREE_AND_NULL(update_दत्त->displaypath);
fail:
		यदि (!संहिता)
			अनुवर्त;
		ret = संहिता;
		यदि (ret == 128)
			गच्छ cleanup;
	}

cleanup:
	submodule_update_clone_release(&suc);
	string_list_clear(&update_दत्त->references, 0);
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क module_update(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
			 संरचना repository *repo अप्रयुक्तम्)
{
	संरचना pathspec pathspec = { 0 };
	संरचना pathspec pathspec2 = { 0 };
	संरचना update_दत्त opt = UPDATE_दत्त_INIT;
	संरचना list_objects_filter_options filter_options =
		LIST_OBJECTS_FILTER_INIT;
	ध्रुव वर्ण *ref_storage_format = रिक्त;
	पूर्णाङ्क ret;
	संरचना option module_update_options[] = {
		OPT_SUPER_उपसर्गः(&opt.super_उपसर्गः),
		OPT__FORCE(&opt.force, N_("force checkout updates"), 0),
		OPT_BOOL('i', "init", &opt.init,
			 N_("initialize uninitialized submodules before update")),
		OPT_BOOL(0, "remote", &opt.remote,
			 N_("use SHA-1 of submodule's remote tracking branch")),
		OPT_BOOL(0, "recursive", &opt.recursive,
			 N_("traverse submodules recursively")),
		OPT_BOOL('N', "no-fetch", &opt.nofetch,
			 N_("don't fetch new objects from the remote site")),
		OPT_समुच्चयः_INT(0, "checkout", &opt.update_default,
			N_("use the 'checkout' update strategy (default)"),
			SM_UPDATE_CHECKOUT),
		OPT_समुच्चयः_INT('m', "merge", &opt.update_default,
			N_("use the 'merge' update strategy"),
			SM_UPDATE_MERGE),
		OPT_समुच्चयः_INT('r', "rebase", &opt.update_default,
			N_("use the 'rebase' update strategy"),
			SM_UPDATE_REBASE),
		OPT_STRING_LIST(0, "reference", &opt.references, N_("repo"),
			   N_("reference repository")),
		OPT_STRING(0, "ref-storage-format", &ref_storage_format, N_("format"),
			   N_("specify the reference storage format to use")),
		OPT_ALIAS_F(0, "ref-format", "ref-storage-format", PARSE_OPT_HIDDEN),
		OPT_BOOL(0, "dissociate", &opt.dissociate,
			   N_("use --reference only while cloning")),
		OPT_INTEGER(0, "depth", &opt.गभीरता,
			   N_("create a shallow clone truncated to the "
			      "specified number of revisions")),
		OPT_INTEGER('j', "jobs", &opt.max_jobs,
			    N_("parallel jobs")),
		OPT_BOOL(0, "recommend-shallow", &opt.recommend_shallow,
			    N_("whether the initial clone should follow the shallow recommendation")),
		OPT__QUIET(&opt.quiet, N_("don't print cloning progress")),
		OPT_BOOL(0, "progress", &opt.प्रगतिः,
			    N_("force cloning progress")),
		OPT_BOOL(0, "require-init", &opt.require_init,
			   N_("disallow cloning into non-empty directory, implies --init")),
		OPT_BOOL(0, "single-branch", &opt.single_branch,
			 N_("clone only one branch, HEAD or --branch")),
		OPT_PARSE_LIST_OBJECTS_FILTER(&filter_options),
		OPT_अन्तः()
	};
	ध्रुव वर्ण *ध्रुव git_submodule_helper_usage[] = {
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

	argc = parse_options(argc, argv, उपसर्गः, module_update_options,
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
	opt.उपसर्गः = उपसर्गः;

	यदि (opt.update_default)
		opt.update_strategy.type = opt.update_default;

	यदि (module_list_compute(argv, उपसर्गः, &pathspec, &opt.list) < 0) {
		ret = 1;
		गच्छ cleanup;
	}

	यदि (pathspec.nr)
		opt.warn_if_uninitialized = 1;

	यदि (opt.init) {
		संरचना module_list list = MODULE_LIST_INIT;
		संरचना init_cb info = INIT_CB_INIT;

		यदि (module_list_compute(argv, opt.उपसर्गः,
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
			module_list_सक्रिय(&list);

		info.उपसर्गः = opt.उपसर्गः;
		info.super_उपसर्गः = opt.super_उपसर्गः;
		यदि (opt.quiet)
			info.flags |= OPT_QUIET;

		for_each_listed_submodule(&list, init_submodule_cb, &info);
		module_list_release(&list);
	}

	ret = update_submodules(&opt);
cleanup:
	update_दत्त_release(&opt);
	list_objects_filter_release(&filter_options);
	clear_pathspec(&pathspec);
	clear_pathspec(&pathspec2);
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क प्रक्षिप_check(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः अप्रयुक्तम्,
		      संरचना repository *repo अप्रयुक्तम्)
{
	संरचना remote *remote;
	ध्रुव वर्ण *superproject_शीर्ष;
	वर्ण *शीर्ष;
	पूर्णाङ्क detached_शीर्ष = 0;
	संरचना object_id शीर्ष_oid;

	यदि (argc < 3)
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
				   0, &शीर्ष_oid, रिक्त);
	यदि (!शीर्ष)
		die(_("Failed to resolve HEAD as a valid ref."));
	यदि (!वाक्यं_तुलय(शीर्ष, "HEAD"))
		detached_शीर्ष = 1;

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
		संरचना refspec refspec = REFSPEC_INIT_प्रक्षिप(the_hash_algo);

		refspec_appendn(&refspec, argv + 2, argc - 2);

		प्रति (i = 0; i < refspec.nr; i++) {
			ध्रुव संरचना refspec_item *rs = &refspec.वस्तूनि[i];

			यदि (rs->आदर्शः || rs->matching)
				अनुवर्त;

			/* LHS must match a single ref */
			विकल्प (गणनम्_refspec_मेलनम्(rs->src, local_refs, रिक्त)) {
			पक्ष 1:
				भङ्ग;
			पक्ष 0:
				/*
				 * If LHS matches 'HEAD' then we need to ensure
				 * that it matches the same named branch
				 * checked out in the superproject.
				 */
				यदि (!वाक्यं_तुलय(rs->src, "HEAD")) {
					यदि (!detached_शीर्ष &&
					    !वाक्यं_तुलय(शीर्ष, superproject_शीर्ष))
						भङ्ग;
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
	मुक्त(शीर्ष);

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क absorb_git_dirs(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
			   संरचना repository *repo अप्रयुक्तम्)
{
	पूर्णाङ्क i;
	संरचना pathspec pathspec = { 0 };
	संरचना module_list list = MODULE_LIST_INIT;
	ध्रुव वर्ण *super_उपसर्गः = रिक्त;
	संरचना option embed_gitdir_options[] = {
		OPT_SUPER_उपसर्गः(&super_उपसर्गः),
		OPT_अन्तः()
	};
	ध्रुव वर्ण *ध्रुव git_submodule_helper_usage[] = {
		N_("git submodule absorbgitdirs [<options>] [<path>...]"),
		रिक्त
	};
	पूर्णाङ्क ret = 1;

	argc = parse_options(argc, argv, उपसर्गः, embed_gitdir_options,
			     git_submodule_helper_usage, 0);

	यदि (module_list_compute(argv, उपसर्गः, &pathspec, &list) < 0)
		गच्छ cleanup;

	प्रति (i = 0; i < list.nr; i++)
		absorb_git_dir_into_superproject(list.entries[i]->name,
						 super_उपसर्गः);

	ret = 0;
cleanup:
	clear_pathspec(&pathspec);
	module_list_release(&list);
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क module_समुच्चयः_url(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
			  संरचना repository *repo अप्रयुक्तम्)
{
	पूर्णाङ्क quiet = 0, ret;
	ध्रुव वर्ण *newurl;
	ध्रुव वर्ण *मार्गः;
	वर्ण *config_name;
	संरचना option options[] = {
		OPT__QUIET(&quiet, N_("suppress output for setting url of a submodule")),
		OPT_अन्तः()
	};
	ध्रुव वर्ण *ध्रुव usage[] = {
		N_("git submodule set-url [--quiet] <path> <newurl>"),
		रिक्त
	};
	ध्रुव संरचना submodule *sub;

	argc = parse_options(argc, argv, उपसर्गः, options, usage, 0);

	यदि (argc != 2 || !(मार्गः = argv[0]) || !(newurl = argv[1]))
		usage_with_options(usage, options);

	sub = submodule_from_मार्गः(the_repository, null_oid(the_hash_algo), मार्गः);

	यदि (!sub)
		die(_("no submodule mapping found in .gitmodules for path '%s'"),
		    मार्गः);

	config_name = xstrfmt("submodule.%s.url", sub->name);
	ret = config_समुच्चयः_अन्तर्_gitmodules_file_gently(the_repository, config_name, newurl);

	यदि (!ret) {
		repo_read_gitmodules(the_repository, 0);
		sync_submodule(sub->मार्गः, उपसर्गः, रिक्त, quiet ? OPT_QUIET : 0);
	}

	मुक्त(config_name);
	प्रत्यागम !!ret;
}

स्थिर पूर्णाङ्क module_समुच्चयः_branch(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
			     संरचना repository *repo अप्रयुक्तम्)
{
	पूर्णाङ्क opt_default = 0, ret;
	ध्रुव वर्ण *opt_branch = रिक्त;
	ध्रुव वर्ण *मार्गः;
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
		OPT_अन्तः()
	};
	ध्रुव वर्ण *ध्रुव usage[] = {
		N_("git submodule set-branch [-q|--quiet] (-d|--default) <path>"),
		N_("git submodule set-branch [-q|--quiet] (-b|--branch) <branch> <path>"),
		रिक्त
	};
	ध्रुव संरचना submodule *sub;

	argc = parse_options(argc, argv, उपसर्गः, options, usage, 0);

	यदि (!opt_branch && !opt_default)
		die(_("--branch or --default required"));

	यदि (opt_branch && opt_default)
		die(_("options '%s' and '%s' cannot be used together"), "--branch", "--default");

	यदि (argc != 1 || !(मार्गः = argv[0]))
		usage_with_options(usage, options);

	sub = submodule_from_मार्गः(the_repository, null_oid(the_hash_algo), मार्गः);

	यदि (!sub)
		die(_("no submodule mapping found in .gitmodules for path '%s'"),
		    मार्गः);

	config_name = xstrfmt("submodule.%s.branch", sub->name);
	ret = config_समुच्चयः_अन्तर्_gitmodules_file_gently(the_repository, config_name, opt_branch);

	मुक्त(config_name);
	प्रत्यागम !!ret;
}

स्थिर पूर्णाङ्क module_create_branch(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
				संरचना repository *repo अप्रयुक्तम्)
{
	गणना branch_track track;
	पूर्णाङ्क quiet = 0, force = 0, reflog = 0, dry_सततम् = 0;
	संरचना option options[] = {
		OPT__QUIET(&quiet, N_("print only error messages")),
		OPT__FORCE(&force, N_("force creation"), 0),
		OPT_BOOL(0, "create-reflog", &reflog,
			 N_("create the branch's reflog")),
		OPT_CALLBACK_F('t', "track",  &track, "(direct|inherit)",
			N_("set branch tracking configuration"),
			PARSE_OPT_OPTARG,
			parse_opt_tracking_बहुलकम्),
		OPT_DRY_सततम्(&dry_सततम्,
			     N_("show whether the branch would be created")),
		OPT_अन्तः()
	};
	ध्रुव वर्ण *ध्रुव usage[] = {
		N_("git submodule--helper create-branch [-f|--force] [--create-reflog] [-q|--quiet] [-t|--track] [-n|--dry-run] <name> <start-oid> <start-name>"),
		रिक्त
	};
	संरचना repo_config_मानानि *cfg = repo_config_मानानि(the_repository);

	repo_config(the_repository, git_default_config, रिक्त);
	track = cfg->branch_track;
	argc = parse_options(argc, argv, उपसर्गः, options, usage, 0);

	यदि (argc != 3)
		usage_with_options(usage, options);

	यदि (!quiet && !dry_सततम्)
		printf_ln(_("creating branch '%s'"), argv[0]);

	create_branches_recursively(the_repository, argv[0], argv[1], argv[2],
				    force, reflog, quiet, track, dry_सततम्);
	प्रत्यागम 0;
}

संरचना सङ्कलय_दत्त {
	ध्रुव वर्ण *उपसर्गः;
	ध्रुव वर्ण *branch;
	ध्रुव वर्ण *reference_मार्गः;
	वर्ण *sm_मार्गः;
	ध्रुव वर्ण *sm_name;
	ध्रुव वर्ण *repo;
	ध्रुव वर्ण *realrepo;
	गणना ref_storage_format ref_storage_format;
	पूर्णाङ्क गभीरता;
	अचिह्नित पूर्णाङ्क force: 1;
	अचिह्नित पूर्णाङ्क quiet: 1;
	अचिह्नित पूर्णाङ्क प्रगतिः: 1;
	अचिह्नित पूर्णाङ्क dissociate: 1;
};
#परिभाषय सङ्कलय_दत्त_INIT { \
	.गभीरता = -1, \
	.ref_storage_format = REF_STORAGE_FORMAT_UNKNOWN, \
}

स्थिर शून्य append_fetch_remotes(संरचना strbuf *सन्देशः, ध्रुव वर्ण *git_dir_मार्गः)
{
	संरचना child_process cp_remote = CHILD_PROCESS_INIT;
	संरचना strbuf sb_remote_बहिर्गमनम् = STRBUF_INIT;

	cp_remote.git_cmd = 1;
	strvec_pushf(&cp_remote.env,
		     "GIT_DIR=%s", git_dir_मार्गः);
	strvec_प्रक्षिप(&cp_remote.env, "GIT_WORK_TREE=.");
	strvec_pushl(&cp_remote.तर्काः, "remote", "-v", रिक्त);
	यदि (!capture_command(&cp_remote, &sb_remote_बहिर्गमनम्, 0)) {
		वर्ण *अग्र_पङ्क्तिः;
		वर्ण *पङ्क्तिः = sb_remote_बहिर्गमनम्.buf;

		यावत् ((अग्र_पङ्क्तिः = strchr(पङ्क्तिः, '\n')) != रिक्त) {
			आकार_t len = अग्र_पङ्क्तिः - पङ्क्तिः;

			यदि (strip_suffix_mem(पङ्क्तिः, &len, " (fetch)"))
				strbuf_addf(सन्देशः, "  %.*s\n", (पूर्णाङ्क)len, पङ्क्तिः);
			पङ्क्तिः = अग्र_पङ्क्तिः + 1;
		}
	}

	strbuf_release(&sb_remote_बहिर्गमनम्);
}

स्थिर पूर्णाङ्क सङ्कलय_submodule(ध्रुव संरचना सङ्कलय_दत्त *सङ्कलय_दत्त)
{
	संरचना module_clone_दत्त clone_दत्त = MODULE_CLONE_दत्त_INIT;
	संरचना string_list reference = STRING_LIST_INIT_NODUP;
	पूर्णाङ्क ret = -1;

	/* perhaps the path already exists and is already a git repo, else clone it */
	यदि (is_directory(सङ्कलय_दत्त->sm_मार्गः)) {
		वर्ण *submod_gitdir_मार्गः;
		संरचना strbuf sm_मार्गः = STRBUF_INIT;
		strbuf_addstr(&sm_मार्गः, सङ्कलय_दत्त->sm_मार्गः);
		submod_gitdir_मार्गः = xstrfmt("%s/.git", सङ्कलय_दत्त->sm_मार्गः);
		यदि (is_nonbare_repository_dir(&sm_मार्गः))
			प्रारूप_लिख(_("Adding existing repo at '%s' to the index\n"),
			       सङ्कलय_दत्त->sm_मार्गः);
		अन्यथा
			die(_("'%s' already exists and is not a valid git repo"),
			    सङ्कलय_दत्त->sm_मार्गः);
		strbuf_release(&sm_मार्गः);
		मुक्त(submod_gitdir_मार्गः);
	} अन्यथा {
		संरचना child_process cp = CHILD_PROCESS_INIT;
		संरचना strbuf submod_gitdir = STRBUF_INIT;

		submodule_name_गन्तव्यम्_gitdir(&submod_gitdir, the_repository, सङ्कलय_दत्त->sm_name);

		यदि (is_directory(submod_gitdir.buf)) {
			यदि (!सङ्कलय_दत्त->force) {
				संरचना strbuf सन्देशः = STRBUF_INIT;
				वर्ण *die_सन्देशः_2;

				strbuf_addf(&सन्देशः, _("A git directory for '%s' is found "
						    "locally with remote(s):\n"),
					    सङ्कलय_दत्त->sm_name);

				append_fetch_remotes(&सन्देशः, submod_gitdir.buf);
				strbuf_release(&submod_gitdir);

				strbuf_addf(&सन्देशः, _("If you want to reuse this local git "
						    "directory instead of cloning again from\n"
						    "  %s\n"
						    "use the '--force' option. If the local git "
						    "directory is not the correct repo\n"
						    "or you are unsure what this means choose "
						    "another name with the '--name' option."),
					    सङ्कलय_दत्त->realrepo);

				die_सन्देशः_2 = strbuf_detach(&सन्देशः, रिक्त);
				die("%s", die_सन्देशः_2);
			} अन्यथा {
				प्रारूप_लिख(_("Reactivating local git directory for "
					 "submodule '%s'\n"), सङ्कलय_दत्त->sm_name);
			}
		}
		strbuf_release(&submod_gitdir);

		clone_दत्त.उपसर्गः = सङ्कलय_दत्त->उपसर्गः;
		clone_दत्त.मार्गः = सङ्कलय_दत्त->sm_मार्गः;
		clone_दत्त.name = सङ्कलय_दत्त->sm_name;
		clone_दत्त.url = सङ्कलय_दत्त->realrepo;
		clone_दत्त.quiet = सङ्कलय_दत्त->quiet;
		clone_दत्त.प्रगतिः = सङ्कलय_दत्त->प्रगतिः;
		यदि (सङ्कलय_दत्त->reference_मार्गः) {
			वर्ण *p = xstrdup(सङ्कलय_दत्त->reference_मार्गः);

			string_list_append(&reference, p)->util = p;
		}
		clone_दत्त.ref_storage_format = सङ्कलय_दत्त->ref_storage_format;
		clone_दत्त.dissociate = सङ्कलय_दत्त->dissociate;
		यदि (सङ्कलय_दत्त->गभीरता >= 0)
			clone_दत्त.गभीरता = सङ्कलय_दत्त->गभीरता;

		यदि (clone_submodule(&clone_दत्त, &reference))
			गच्छ cleanup;

		prepare_submodule_repo_env(&cp.env);
		cp.git_cmd = 1;
		cp.dir = सङ्कलय_दत्त->sm_मार्गः;
		/*
		 * NOTE: we only get here if add_data->force is true, so
		 * passing --force to checkout is reasonable.
		 */
		strvec_pushl(&cp.तर्काः, "checkout", "-f", "-q", रिक्त);

		यदि (सङ्कलय_दत्त->branch) {
			strvec_pushl(&cp.तर्काः, "-B", सङ्कलय_दत्त->branch, रिक्त);
			strvec_pushf(&cp.तर्काः, "origin/%s", सङ्कलय_दत्त->branch);
		}

		यदि (सततम्_command(&cp))
			die(_("unable to checkout submodule '%s'"), सङ्कलय_दत्त->sm_मार्गः);
	}
	ret = 0;

cleanup:
	string_list_clear(&reference, 1);
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क config_submodule_अन्तर्_gitmodules(ध्रुव वर्ण *name, ध्रुव वर्ण *var, ध्रुव वर्ण *मानम्)
{
	वर्ण *कुञ्चिका;
	पूर्णाङ्क ret;

	यदि (!is_writing_gitmodules_ok())
		die(_("please make sure that the .gitmodules file is in the working tree"));

	कुञ्चिका = xstrfmt("submodule.%s.%s", name, var);
	ret = config_समुच्चयः_अन्तर्_gitmodules_file_gently(the_repository, कुञ्चिका, मानम्);
	मुक्त(कुञ्चिका);

	प्रत्यागम ret;
}

स्थिर शून्य configure_added_submodule(संरचना सङ्कलय_दत्त *सङ्कलय_दत्त)
{
	वर्ण *कुञ्चिका;
	संरचना child_process सङ्कलय_submod = CHILD_PROCESS_INIT;
	संरचना child_process सङ्कलय_gitmodules = CHILD_PROCESS_INIT;
	ध्रुव संरचना string_list *मानानि;
	पूर्णाङ्क matched = 0;

	कुञ्चिका = xstrfmt("submodule.%s.url", सङ्कलय_दत्त->sm_name);
	repo_config_समुच्चयः_gently(the_repository, कुञ्चिका, सङ्कलय_दत्त->realrepo);
	मुक्त(कुञ्चिका);

	सङ्कलय_submod.git_cmd = 1;
	strvec_pushl(&सङ्कलय_submod.तर्काः, "add",
		     "--no-warn-embedded-repo", रिक्त);
	यदि (सङ्कलय_दत्त->force)
		strvec_प्रक्षिप(&सङ्कलय_submod.तर्काः, "--force");
	strvec_pushl(&सङ्कलय_submod.तर्काः, "--", सङ्कलय_दत्त->sm_मार्गः, रिक्त);

	यदि (सततम्_command(&सङ्कलय_submod))
		die(_("Failed to add submodule '%s'"), सङ्कलय_दत्त->sm_मार्गः);

	यदि (config_submodule_अन्तर्_gitmodules(सङ्कलय_दत्त->sm_name, "path", सङ्कलय_दत्त->sm_मार्गः) ||
	    config_submodule_अन्तर्_gitmodules(सङ्कलय_दत्त->sm_name, "url", सङ्कलय_दत्त->repo))
		die(_("Failed to register submodule '%s'"), सङ्कलय_दत्त->sm_मार्गः);

	यदि (सङ्कलय_दत्त->branch) {
		यदि (config_submodule_अन्तर्_gitmodules(सङ्कलय_दत्त->sm_name,
						   "branch", सङ्कलय_दत्त->branch))
			die(_("Failed to register submodule '%s'"), सङ्कलय_दत्त->sm_मार्गः);
	}

	सङ्कलय_gitmodules.git_cmd = 1;
	strvec_pushl(&सङ्कलय_gitmodules.तर्काः,
		     "add", "--force", "--", ".gitmodules", रिक्त);

	यदि (सततम्_command(&सङ्कलय_gitmodules))
		die(_("Failed to register submodule '%s'"), सङ्कलय_दत्त->sm_मार्गः);

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
	    repo_config_get_string_multi(the_repository, "submodule.active", &मानानि)) {
		/*
		 * If the submodule being added isn't already covered by the
		 * current configured pathspec, set the submodule's active flag
		 */
		कुञ्चिका = xstrfmt("submodule.%s.active", सङ्कलय_दत्त->sm_name);
		repo_config_समुच्चयः_gently(the_repository, कुञ्चिका, "true");
		मुक्त(कुञ्चिका);
	} अन्यथा {
		प्रति (आकार_t i = 0; i < मानानि->nr; i++) {
			ध्रुव वर्ण *pat = मानानि->वस्तूनि[i].string;
			यदि (!wildmatch(pat, सङ्कलय_दत्त->sm_मार्गः, 0)) { /* match found */
				matched = 1;
				भङ्ग;
			}
		}
		यदि (!matched) { /* no pattern matched -> force-enable */
			कुञ्चिका = xstrfmt("submodule.%s.active", सङ्कलय_दत्त->sm_name);
			repo_config_समुच्चयः_gently(the_repository, कुञ्चिका, "true");
			मुक्त(कुञ्चिका);
		}
	}
}

स्थिर शून्य die_on_सूचकांक_मेलनम्(ध्रुव वर्ण *मार्गः, पूर्णाङ्क force)
{
	संरचना pathspec ps;
	ध्रुव वर्ण *तर्काः[] = { मार्गः, रिक्त };
	parse_pathspec(&ps, 0, PATHSPEC_PREFER_CWD, रिक्त, तर्काः);

	यदि (repo_read_सूचकांक_preload(the_repository, रिक्त, 0) < 0)
		die(_("index file corrupt"));

	यदि (ps.nr) {
		वर्ण *ps_matched = xcalloc(ps.nr, 1);

		/* TODO: audit for interaction with sparse-index. */
		ensure_पूर्णम्_सूचकांक(the_repository->सूचकांक);

		/*
		 * Since there is only one pathspec, we just need to
		 * check ps_matched[0] to know if a cache entry matched.
		 */
		प्रति (आकार_t i = 0; i < the_repository->सूचकांक->cache_nr; i++) {
			ce_मार्गः_मेलनम्(the_repository->सूचकांक, the_repository->सूचकांक->cache[i], &ps,
				      ps_matched);

			यदि (ps_matched[0]) {
				यदि (!force)
					die(_("'%s' already exists in the index"),
					    मार्गः);
				यदि (!S_ISGITLINK(the_repository->सूचकांक->cache[i]->ce_बहुलकम्))
					die(_("'%s' already exists in the index "
					      "and is not a submodule"), मार्गः);
				भङ्ग;
			}
		}
		मुक्त(ps_matched);
	}
	clear_pathspec(&ps);
}

स्थिर शून्य die_on_repo_without_commits(ध्रुव वर्ण *मार्गः)
{
	संरचना strbuf sb = STRBUF_INIT;
	strbuf_addstr(&sb, मार्गः);
	यदि (is_nonbare_repository_dir(&sb)) {
		संरचना object_id oid;
		यदि (repo_resolve_gitlink_ref(the_repository, मार्गः, "HEAD", &oid) < 0)
			die(_("'%s' does not have a commit checked out"), मार्गः);
	}
	strbuf_release(&sb);
}

स्थिर पूर्णाङ्क module_सङ्कलय(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
		      संरचना repository *repo अप्रयुक्तम्)
{
	पूर्णाङ्क force = 0, quiet = 0, प्रगतिः = 0, dissociate = 0;
	संरचना सङ्कलय_दत्त सङ्कलय_दत्त = सङ्कलय_दत्त_INIT;
	ध्रुव वर्ण *ref_storage_format = रिक्त;
	वर्ण *गन्तव्यम्_free = रिक्त;
	ध्रुव संरचना submodule *existing;
	संरचना strbuf buf = STRBUF_INIT;
	वर्ण *sm_name_गन्तव्यम्_free = रिक्त;
	संरचना option options[] = {
		OPT_STRING('b', "branch", &सङ्कलय_दत्त.branch, N_("branch"),
			   N_("branch of repository to add as submodule")),
		OPT__FORCE(&force, N_("allow adding an otherwise ignored submodule path"),
			   PARSE_OPT_NOCOMPLETE),
		OPT__QUIET(&quiet, N_("print only error messages")),
		OPT_BOOL(0, "progress", &प्रगतिः, N_("force cloning progress")),
		OPT_STRING(0, "reference", &सङ्कलय_दत्त.reference_मार्गः, N_("repository"),
			   N_("reference repository")),
		OPT_STRING(0, "ref-storage-format", &ref_storage_format, N_("format"),
			   N_("specify the reference storage format to use")),
		OPT_ALIAS_F(0, "ref-format", "ref-storage-format", PARSE_OPT_HIDDEN),
		OPT_BOOL(0, "dissociate", &dissociate, N_("borrow the objects from reference repositories")),
		OPT_STRING(0, "name", &सङ्कलय_दत्त.sm_name, N_("name"),
			   N_("sets the submodule's name to the given string "
			      "instead of defaulting to its path")),
		OPT_INTEGER(0, "depth", &सङ्कलय_दत्त.गभीरता, N_("depth for shallow clones")),
		OPT_अन्तः()
	};
	ध्रुव वर्ण *ध्रुव usage[] = {
		N_("git submodule add [<options>] [--] <repository> [<path>]"),
		रिक्त
	};
	संरचना strbuf sb = STRBUF_INIT;
	पूर्णाङ्क ret = 1;

	argc = parse_options(argc, argv, उपसर्गः, options, usage, 0);

	यदि (!is_writing_gitmodules_ok())
		die(_("please make sure that the .gitmodules file is in the working tree"));

	यदि (उपसर्गः && *उपसर्गः &&
	    सङ्कलय_दत्त.reference_मार्गः && !is_absolute_मार्गः(सङ्कलय_दत्त.reference_मार्गः))
		सङ्कलय_दत्त.reference_मार्गः = xstrfmt("%s%s", उपसर्गः, सङ्कलय_दत्त.reference_मार्गः);

	यदि (argc == 0 || argc > 2)
		usage_with_options(usage, options);

	यदि (ref_storage_format) {
		सङ्कलय_दत्त.ref_storage_format = ref_storage_format_by_name(ref_storage_format);
		यदि (सङ्कलय_दत्त.ref_storage_format == REF_STORAGE_FORMAT_UNKNOWN)
			die(_("unknown ref storage format '%s'"), ref_storage_format);
	}

	सङ्कलय_दत्त.repo = argv[0];
	यदि (argc == 1)
		सङ्कलय_दत्त.sm_मार्गः = git_url_basename(सङ्कलय_दत्त.repo, 0, 0);
	अन्यथा
		सङ्कलय_दत्त.sm_मार्गः = xstrdup(argv[1]);

	यदि (उपसर्गः && *उपसर्गः && !is_absolute_मार्गः(सङ्कलय_दत्त.sm_मार्गः)) {
		वर्ण *sm_मार्गः = सङ्कलय_दत्त.sm_मार्गः;

		सङ्कलय_दत्त.sm_मार्गः = xstrfmt("%s%s", उपसर्गः, sm_मार्गः);
		मुक्त(sm_मार्गः);
	}

	यदि (starts_with_dot_dot_slash(सङ्कलय_दत्त.repo) ||
	    starts_with_dot_slash(सङ्कलय_दत्त.repo)) {
		यदि (उपसर्गः)
			die(_("Relative path can only be used from the toplevel "
			      "of the working tree"));

		/* dereference source url relative to parent's url */
		गन्तव्यम्_free = resolve_relative_url(सङ्कलय_दत्त.repo, रिक्त, 1);
		सङ्कलय_दत्त.realrepo = गन्तव्यम्_free;
	} अन्यथा यदि (is_dir_sep(सङ्कलय_दत्त.repo[0]) || strchr(सङ्कलय_दत्त.repo, ':')) {
		सङ्कलय_दत्त.realrepo = सङ्कलय_दत्त.repo;
	} अन्यथा {
		die(_("repo URL: '%s' must be absolute or begin with ./|../"),
		    सङ्कलय_दत्त.repo);
	}

	/*
	 * normalize path:
	 * multiple //; leading ./; /./; /../;
	 */
	normalize_मार्गः_copy(सङ्कलय_दत्त.sm_मार्गः, सङ्कलय_दत्त.sm_मार्गः);
	strip_dir_trailing_slashes(सङ्कलय_दत्त.sm_मार्गः);

	यदि (validate_submodule_मार्गः(सङ्कलय_दत्त.sm_मार्गः) < 0)
		die(रिक्त);

	die_on_सूचकांक_मेलनम्(सङ्कलय_दत्त.sm_मार्गः, force);
	die_on_repo_without_commits(सङ्कलय_दत्त.sm_मार्गः);

	यदि (!force) {
		संरचना child_process cp = CHILD_PROCESS_INIT;

		cp.git_cmd = 1;
		cp.no_stdout = 1;
		strvec_pushl(&cp.तर्काः, "add", "--dry-run", "--ignore-missing",
			     "--no-warn-embedded-repo", सङ्कलय_दत्त.sm_मार्गः, रिक्त);
		यदि ((ret = pipe_command(&cp, रिक्त, 0, रिक्त, 0, &sb, 0))) {
			strbuf_complete_पङ्क्तिः(&sb);
			fputs(sb.buf, stderr);
			गच्छ cleanup;
		}
	}

	यदि (!सङ्कलय_दत्त.sm_name)
		सङ्कलय_दत्त.sm_name = सङ्कलय_दत्त.sm_मार्गः;

	existing = submodule_from_name(the_repository,
					null_oid(the_hash_algo),
					सङ्कलय_दत्त.sm_name);

	यदि (existing && existing->मार्गः &&
	    वाक्यं_तुलय(existing->मार्गः, सङ्कलय_दत्त.sm_मार्गः)) {
		यदि (!force) {
			die(_("submodule name '%s' already used for path '%s'"),
			    सङ्कलय_दत्त.sm_name, existing->मार्गः);
		}
		/* --force: build <name><n> until unique */
		प्रति (पूर्णाङ्क i = 1; ; i++) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "%s%d", सङ्कलय_दत्त.sm_name, i);
			यदि (!submodule_from_name(the_repository,
						null_oid(the_hash_algo),
						buf.buf)) {
				भङ्ग;
			}
		}
		सङ्कलय_दत्त.sm_name = sm_name_गन्तव्यम्_free = strbuf_detach(&buf, रिक्त);
	}

	यदि (check_submodule_name(सङ्कलय_दत्त.sm_name))
		die(_("'%s' is not a valid submodule name"), सङ्कलय_दत्त.sm_name);

	सङ्कलय_दत्त.उपसर्गः = उपसर्गः;
	सङ्कलय_दत्त.force = !!force;
	सङ्कलय_दत्त.quiet = !!quiet;
	सङ्कलय_दत्त.प्रगतिः = !!प्रगतिः;
	सङ्कलय_दत्त.dissociate = !!dissociate;

	यदि (the_repository->repository_format_submodule_मार्गः_cfg)
		create_default_gitdir_config(सङ्कलय_दत्त.sm_name);

	यदि (सङ्कलय_submodule(&सङ्कलय_दत्त))
		गच्छ cleanup;
	configure_added_submodule(&सङ्कलय_दत्त);

	ret = 0;
cleanup:
	मुक्त(sm_name_गन्तव्यम्_free);
	मुक्त(सङ्कलय_दत्त.sm_मार्गः);
	मुक्त(गन्तव्यम्_free);
	strbuf_release(&sb);

	प्रत्यागम ret;
}

पूर्णाङ्क cmd_submodule__helper(पूर्णाङ्क argc,
			  ध्रुव वर्ण **argv,
			  ध्रुव वर्ण *उपसर्गः,
			  संरचना repository *repo)
{
	parse_opt_subcommand_fn *fn = रिक्त;
	ध्रुव वर्ण *ध्रुव usage[] = {
		N_("git submodule--helper <command>"),
		रिक्त
	};
	संरचना option options[] = {
		OPT_SUBCOMMAND("migrate-gitdir-configs", &fn, module_migrate),
		OPT_SUBCOMMAND("gitdir", &fn, module_gitdir),
		OPT_SUBCOMMAND("clone", &fn, module_clone),
		OPT_SUBCOMMAND("add", &fn, module_सङ्कलय),
		OPT_SUBCOMMAND("update", &fn, module_update),
		OPT_SUBCOMMAND("foreach", &fn, module_foreach),
		OPT_SUBCOMMAND("init", &fn, module_init),
		OPT_SUBCOMMAND("status", &fn, module_स्थितिः),
		OPT_SUBCOMMAND("sync", &fn, module_sync),
		OPT_SUBCOMMAND("deinit", &fn, module_deinit),
		OPT_SUBCOMMAND("summary", &fn, module_summary),
		OPT_SUBCOMMAND("push-check", &fn, प्रक्षिप_check),
		OPT_SUBCOMMAND("absorbgitdirs", &fn, absorb_git_dirs),
		OPT_SUBCOMMAND("set-url", &fn, module_समुच्चयः_url),
		OPT_SUBCOMMAND("set-branch", &fn, module_समुच्चयः_branch),
		OPT_SUBCOMMAND("create-branch", &fn, module_create_branch),
		OPT_SUBCOMMAND("get-default-remote", &fn, module_get_default_remote),
		OPT_अन्तः()
	};
	argc = parse_options(argc, argv, उपसर्गः, options, usage, 0);

	प्रत्यागम fn(argc, argv, उपसर्गः, repo);
}
