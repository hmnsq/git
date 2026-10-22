#నిర్వచించు USE_THE_REPOSITORY_VARIABLE

#అన్తర్భావ "builtin.h"
#అన్తర్భావ "abspath.h"
#అన్తర్భావ "environment.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "hex.h"

#అన్తర్భావ "config.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "quote.h"
#అన్తర్భావ "path.h"
#అన్తర్భావ "pathspec.h"
#అన్తర్భావ "preload-index.h"
#అన్తర్భావ "dir.h"
#అన్తర్భావ "read-cache.h"
#అన్తర్భావ "setup.h"
#అన్తర్భావ "sparse-index.h"
#అన్తర్భావ "submodule.h"
#అన్తర్భావ "submodule-config.h"
#అన్తర్భావ "string-list.h"
#అన్తర్భావ "run-command.h"
#అన్తర్భావ "remote.h"
#అన్తర్భావ "refs.h"
#అన్తర్భావ "refspec.h"
#అన్తర్భావ "revision.h"
#అన్తర్భావ "diffcore.h"
#అన్తర్భావ "diff.h"
#అన్తర్భావ "object-file.h"
#అన్తర్భావ "object-name.h"
#అన్తర్భావ "odb.h"
#అన్తర్భావ "odb/source.h"
#అన్తర్భావ "odb/source-files.h"
#అన్తర్భావ "advice.h"
#అన్తర్భావ "branch.h"
#అన్తర్భావ "list-objects-filter-options.h"
#అన్తర్భావ "wildmatch.h"
#అన్తర్భావ "strbuf.h"
#అన్తర్భావ "url.h"

#నిర్వచించు OPT_QUIET (1 << 0)
#నిర్వచించు OPT_CACHED (1 << 1)
#నిర్వచించు OPT_RECURSIVE (1 << 2)
#నిర్వచించు OPT_FORCE (1 << 3)

రకనిర్వచనం శూన్య (*each_submodule_fn)(స్థిరరాశి నిర్మాణం cache_నమోదు *list_అంశం,
				  శూన్య *cb_దత్తాంశం);

స్థిర అక్షరం *get_default_remote(శూన్య)
{
	తిరిగి xstrdup(repo_default_remote(the_repository));
}

స్థిర అక్షరం *resolve_relative_url(స్థిరరాశి అక్షరం *rel_url, స్థిరరాశి అక్షరం *పైకి_మార్గం, పూర్ణాంకం quiet)
{
	అక్షరం *remoteurl, *resolved_url;
	అక్షరం *remote = get_default_remote();
	నిర్మాణం strbuf remotesb = STRBUF_INIT;

	strbuf_addf(&remotesb, "remote.%s.url", remote);
	ఒకవేళ (repo_config_get_string(the_repository, remotesb.buf, &remoteurl)) {
		ఒకవేళ (!quiet)
			warning(_("could not look up configuration '%s'. "
				  "Assuming this repository is its own "
				  "authoritative upstream."),
				remotesb.buf);
		remoteurl = xgetcwd();
	}
	resolved_url = relative_url(remoteurl, rel_url, పైకి_మార్గం);

	విడుదల(remote);
	విడుదల(remoteurl);
	strbuf_release(&remotesb);

	తిరిగి resolved_url;
}

స్థిర పూర్ణాంకం get_default_remote_submodule(స్థిరరాశి అక్షరం *module_మార్గం, అక్షరం **default_remote)
{
	స్థిరరాశి నిర్మాణం submodule *sub;
	నిర్మాణం repository subrepo;
	స్థిరరాశి అక్షరం *remote_name = ఖాళీ;
	అక్షరం *url = ఖాళీ;
	పూర్ణాంకం ret = 0;

	sub = submodule_from_మార్గం(the_repository, null_oid(the_hash_algo), module_మార్గం);
	ఒకవేళ (sub && sub->url) {
		url = xstrdup(sub->url);

		/* Possibly a url relative to parent */
		ఒకవేళ (starts_with_dot_dot_slash(url) ||
		    starts_with_dot_slash(url)) {
			అక్షరం *oldurl = url;

			url = resolve_relative_url(oldurl, ఖాళీ, 1);
			విడుదల(oldurl);
		}
	}

	ఒకవేళ (repo_submodule_init(&subrepo, the_repository, module_మార్గం,
				null_oid(the_hash_algo)) < 0) {
		ret = die_సందేశం(_("could not get a repository handle for submodule '%s'"),
				   module_మార్గం);
		వెళ్ళు బహిర్గతం;
	}

	/* Look up by URL first */
	ఒకవేళ (url)
		remote_name = repo_remote_from_url(&subrepo, url);
	ఒకవేళ (!remote_name)
		remote_name = repo_default_remote(&subrepo);

	*default_remote = xstrdup(remote_name);

బహిర్గతం:
	repo_clear(&subrepo);
	విడుదల(url);

	తిరిగి ret;
}

స్థిర పూర్ణాంకం module_get_default_remote(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
				     నిర్మాణం repository *repo వాడని)
{
	స్థిరరాశి అక్షరం *మార్గం;
	అక్షరం *resolved_మార్గం = ఖాళీ;
	అక్షరం *default_remote = ఖాళీ;
	పూర్ణాంకం సంకేతం_2;
	నిర్మాణం option options[] = {
		OPT_చివర()
	};
	స్థిరరాశి అక్షరం *స్థిరరాశి usage[] = {
		N_("git submodule--helper get-default-remote <path>"),
		ఖాళీ
	};

	argc = parse_options(argc, argv, ఉపసర్గ, options, usage, 0);
	ఒకవేళ (argc != 1)
		usage_with_options(usage, options);

	మార్గం = argv[0];
	ఒకవేళ (ఉపసర్గ && *ఉపసర్గ && !is_absolute_మార్గం(మార్గం)) {
		resolved_మార్గం = xstrfmt("%s%s", ఉపసర్గ, మార్గం);
		మార్గం = resolved_మార్గం;
	}

	సంకేతం_2 = get_default_remote_submodule(మార్గం, &default_remote);
	ఒకవేళ (సంకేతం_2) {
		విడుదల(resolved_మార్గం);
		తిరిగి సంకేతం_2;
	}

	రాయి("%s\n", default_remote);
	విడుదల(default_remote);
	విడుదల(resolved_మార్గం);
	తిరిగి 0;
}

/* the result should be freed by the caller. */
స్థిర అక్షరం *get_submodule_displaypath(స్థిరరాశి అక్షరం *మార్గం, స్థిరరాశి అక్షరం *ఉపసర్గ,
				       స్థిరరాశి అక్షరం *super_ఉపసర్గ)
{
	ఒకవేళ (ఉపసర్గ && super_ఉపసర్గ) {
		BUG("cannot have prefix '%s' and superprefix '%s'",
		    ఉపసర్గ, super_ఉపసర్గ);
	} లేకపోతే ఒకవేళ (ఉపసర్గ) {
		నిర్మాణం strbuf sb = STRBUF_INIT;
		అక్షరం *displaypath = xstrdup(relative_మార్గం(మార్గం, ఉపసర్గ, &sb));
		strbuf_release(&sb);
		తిరిగి displaypath;
	} లేకపోతే ఒకవేళ (super_ఉపసర్గ) {
		తిరిగి xstrfmt("%s%s", super_ఉపసర్గ, మార్గం);
	} లేకపోతే {
		తిరిగి xstrdup(మార్గం);
	}
}

స్థిర అక్షరం *compute_rev_name(స్థిరరాశి అక్షరం *sub_మార్గం, స్థిరరాశి అక్షరం* object_id)
{
	నిర్మాణం strbuf sb = STRBUF_INIT;
	స్థిరరాశి అక్షరం ***d;

	స్థిర స్థిరరాశి అక్షరం *describe_bare[] = { ఖాళీ };

	స్థిర స్థిరరాశి అక్షరం *describe_ట్యాగులు[] = { "--tags", ఖాళీ };

	స్థిర స్థిరరాశి అక్షరం *describe_కలిగియుంది[] = { "--contains", ఖాళీ };

	స్థిర స్థిరరాశి అక్షరం *describe_all_always[] = { "--all", "--always", ఖాళీ };

	స్థిర స్థిరరాశి అక్షరం **describe_argv[] = { describe_bare, describe_ట్యాగులు,
						describe_కలిగియుంది,
						describe_all_always, ఖాళీ };

	కొరకు (d = describe_argv; *d; d++) {
		నిర్మాణం బిడ్డ_process cp = బిడ్డ_PROCESS_INIT;
		prepare_submodule_repo_env(&cp.env);
		cp.dir = sub_మార్గం;
		cp.git_cmd = 1;
		cp.no_stderr = 1;

		strvec_నెట్టు(&cp.వాదనలు, "describe");
		strvec_pushv(&cp.వాదనలు, *d);
		strvec_నెట్టు(&cp.వాదనలు, object_id);

		ఒకవేళ (!capture_command(&cp, &sb, 0)) {
			strbuf_strip_suffix(&sb, "\n");
			తిరిగి strbuf_detach(&sb, ఖాళీ);
		}
	}

	strbuf_release(&sb);
	తిరిగి ఖాళీ;
}

నిర్మాణం module_list {
	స్థిరరాశి నిర్మాణం cache_నమోదు **entries;
	పూర్ణాంకం alloc, nr;
};
#నిర్వచించు MODULE_LIST_INIT { 0 }

స్థిర శూన్య module_list_release(నిర్మాణం module_list *ml)
{
	విడుదల(ml->entries);
}

స్థిర పూర్ణాంకం module_list_compute(స్థిరరాశి అక్షరం **argv,
			       స్థిరరాశి అక్షరం *ఉపసర్గ,
			       నిర్మాణం pathspec *pathspec,
			       నిర్మాణం module_list *list)
{
	పూర్ణాంకం ఫలితం = 0;
	అక్షరం *ps_matched = ఖాళీ;

	parse_pathspec(pathspec, 0,
		       PATHSPEC_PREFER_నిండిన,
		       ఉపసర్గ, argv);

	ఒకవేళ (pathspec->nr)
		ps_matched = xcalloc(pathspec->nr, 1);

	ఒకవేళ (repo_read_సూచీ(the_repository) < 0)
		die(_("index file corrupt"));

	కొరకు (కొలత_t i = 0; i < the_repository->సూచీ->cache_nr; i++) {
		స్థిరరాశి నిర్మాణం cache_నమోదు *ce = the_repository->సూచీ->cache[i];

		ఒకవేళ (!సరిపోలిక_pathspec(the_repository->సూచీ, pathspec, ce->name, ce_namelen(ce),
				    0, ps_matched, 1) ||
		    !S_ISGITLINK(ce->ce_బహుళకం))
			కొనసాగు;

		ALLOC_GROW(list->entries, list->nr + 1, list->alloc);
		list->entries[list->nr++] = ce;
		అయితే (i + 1 < the_repository->సూచీ->cache_nr &&
		       !strcmp(ce->name, the_repository->సూచీ->cache[i + 1]->name))
			/*
			 * Skip entries with the same name in different stages
			 * to make sure an entry is returned only once.
			 */
			i++;
	}

	ఒకవేళ (ps_matched && report_మార్గం_error(ps_matched, pathspec))
		ఫలితం = -1;

	విడుదల(ps_matched);

	తిరిగి ఫలితం;
}

స్థిర శూన్య module_list_చురుకైన(నిర్మాణం module_list *list)
{
	పూర్ణాంకం i;
	నిర్మాణం module_list చురుకైన_modules = MODULE_LIST_INIT;

	కొరకు (i = 0; i < list->nr; i++) {
		స్థిరరాశి నిర్మాణం cache_నమోదు *ce = list->entries[i];

		ఒకవేళ (!is_submodule_చురుకైన(the_repository, ce->name))
			కొనసాగు;

		ALLOC_GROW(చురుకైన_modules.entries,
			   చురుకైన_modules.nr + 1,
			   చురుకైన_modules.alloc);
		చురుకైన_modules.entries[చురుకైన_modules.nr++] = ce;
	}

	module_list_release(list);
	*list = చురుకైన_modules;
}

స్థిర అక్షరం *get_పైకి_మార్గం(స్థిరరాశి అక్షరం *మార్గం)
{
	నిర్మాణం strbuf sb = STRBUF_INIT;

	strbuf_addstrings(&sb, "../", లెక్క_slashes(మార్గం));

	/*
	 * Check if 'path' ends with slash or not
	 * for having the same output for dir/sub_dir
	 * and dir/sub_dir/
	 */
	ఒకవేళ (!is_dir_sep(మార్గం[అక్షరశ్రేణి_పొడవు(మార్గం) - 1]))
		strbuf_addstr(&sb, "../");

	తిరిగి strbuf_detach(&sb, ఖాళీ);
}

స్థిర శూన్య for_each_listed_submodule(స్థిరరాశి నిర్మాణం module_list *list,
				      each_submodule_fn fn, శూన్య *cb_దత్తాంశం)
{
	పూర్ణాంకం i;

	కొరకు (i = 0; i < list->nr; i++)
		fn(list->entries[i], cb_దత్తాంశం);
}

నిర్మాణం foreach_cb {
	పూర్ణాంకం argc;
	స్థిరరాశి అక్షరం **argv;
	స్థిరరాశి అక్షరం *ఉపసర్గ;
	స్థిరరాశి అక్షరం *super_ఉపసర్గ;
	పూర్ణాంకం quiet;
	పూర్ణాంకం recursive;
};
#నిర్వచించు FOREACH_CB_INIT { 0 }

స్థిర శూన్య runcommand_లోపల_submodule_cb(స్థిరరాశి నిర్మాణం cache_నమోదు *list_అంశం,
				       శూన్య *cb_దత్తాంశం)
{
	నిర్మాణం foreach_cb *info = cb_దత్తాంశం;
	స్థిరరాశి అక్షరం *మార్గం = list_అంశం->name;
	స్థిరరాశి నిర్మాణం object_id *ce_oid = &list_అంశం->oid;
	స్థిరరాశి నిర్మాణం submodule *sub;
	నిర్మాణం బిడ్డ_process cp = బిడ్డ_PROCESS_INIT;
	అక్షరం *displaypath;

	ఒకవేళ (validate_submodule_మార్గం(మార్గం) < 0)
		die(ఖాళీ);

	displaypath = get_submodule_displaypath(మార్గం, info->ఉపసర్గ,
						info->super_ఉపసర్గ);

	sub = submodule_from_మార్గం(the_repository, null_oid(the_hash_algo), మార్గం);

	ఒకవేళ (!sub)
		die(_("No url found for submodule path '%s' in .gitmodules"),
			displaypath);

	ఒకవేళ (!is_submodule_populated_gently(మార్గం, ఖాళీ))
		వెళ్ళు cleanup;

	prepare_submodule_repo_env(&cp.env);

	/*
	 * For the purpose of executing <command> in the submodule,
	 * separate shell is used for the purpose of running the
	 * child process.
	 */
	cp.use_shell = 1;
	cp.dir = మార్గం;

	/*
	 * NEEDSWORK: the command currently has access to the variables $name,
	 * $sm_path, $displaypath, $sha1 and $toplevel only when the command
	 * contains a single argument. This is done for maintaining a faithful
	 * translation from shell script.
	 */
	ఒకవేళ (info->argc == 1) {
		అక్షరం *toplevel = xgetcwd();
		నిర్మాణం strbuf sb = STRBUF_INIT;

		strvec_pushf(&cp.env, "name=%s", sub->name);
		strvec_pushf(&cp.env, "sm_path=%s", మార్గం);
		strvec_pushf(&cp.env, "displaypath=%s", displaypath);
		strvec_pushf(&cp.env, "sha1=%s",
			     oid_గమ్యం_hex(ce_oid));
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
		sq_quote_buf(&sb, మార్గం);
		strvec_pushf(&cp.వాదనలు, "path=%s; %s",
			     sb.buf, info->argv[0]);
		strbuf_release(&sb);
		విడుదల(toplevel);
	} లేకపోతే {
		strvec_pushv(&cp.వాదనలు, info->argv);
	}

	ఒకవేళ (!info->quiet)
		రాయి(_("Entering '%s'\n"), displaypath);

	ఒకవేళ (info->argv[0]) {
		ఒకవేళ (నిరంతరం_command(&cp))
			die(_("run_command returned non-zero status for %s\n."),
			    displaypath);
	} లేకపోతే {
		బిడ్డ_process_clear(&cp);
	}

	ఒకవేళ (info->recursive) {
		నిర్మాణం బిడ్డ_process cpr = బిడ్డ_PROCESS_INIT;

		cpr.git_cmd = 1;
		cpr.dir = మార్గం;
		prepare_submodule_repo_env(&cpr.env);

		strvec_pushl(&cpr.వాదనలు, "submodule--helper", "foreach", "--recursive",
			     ఖాళీ);
		strvec_pushf(&cpr.వాదనలు, "--super-prefix=%s/", displaypath);

		ఒకవేళ (info->quiet)
			strvec_నెట్టు(&cpr.వాదనలు, "--quiet");

		strvec_నెట్టు(&cpr.వాదనలు, "--");
		strvec_pushv(&cpr.వాదనలు, info->argv);

		ఒకవేళ (నిరంతరం_command(&cpr))
			die(_("run_command returned non-zero status while "
				"recursing in the nested submodules of %s\n."),
				displaypath);
	}

cleanup:
	విడుదల(displaypath);
}

స్థిర పూర్ణాంకం module_foreach(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
			  నిర్మాణం repository *repo వాడని)
{
	నిర్మాణం foreach_cb info = FOREACH_CB_INIT;
	నిర్మాణం pathspec pathspec = { 0 };
	నిర్మాణం module_list list = MODULE_LIST_INIT;
	నిర్మాణం option module_foreach_options[] = {
		OPT_SUPER_ఉపసర్గ(&info.super_ఉపసర్గ),
		OPT__QUIET(&info.quiet, N_("suppress output of entering each submodule command")),
		OPT_BOOL(0, "recursive", &info.recursive,
			 N_("recurse into nested submodules")),
		OPT_చివర()
	};
	స్థిరరాశి అక్షరం *స్థిరరాశి git_submodule_helper_usage[] = {
		N_("git submodule foreach [--quiet] [--recursive] [--] <command>"),
		ఖాళీ
	};
	పూర్ణాంకం ret = 1;

	argc = parse_options(argc, argv, ఉపసర్గ, module_foreach_options,
			     git_submodule_helper_usage, 0);

	ఒకవేళ (module_list_compute(ఖాళీ, ఉపసర్గ, &pathspec, &list) < 0)
		వెళ్ళు cleanup;

	info.argc = argc;
	info.argv = argv;
	info.ఉపసర్గ = ఉపసర్గ;

	for_each_listed_submodule(&list, runcommand_లోపల_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	తిరిగి ret;
}

నిర్మాణం init_cb {
	స్థిరరాశి అక్షరం *ఉపసర్గ;
	స్థిరరాశి అక్షరం *super_ఉపసర్గ;
	గుర్తులేని పూర్ణాంకం flags;
};
#నిర్వచించు INIT_CB_INIT { 0 }

స్థిర పూర్ణాంకం validate_and_సమితి_submodule_gitdir(నిర్మాణం strbuf *gitdir_మార్గం,
					     స్థిరరాశి అక్షరం *submodule_name)
{
	స్థిరరాశి అక్షరం *విలువ;
	అక్షరం *కీలకం;

	ఒకవేళ (validate_submodule_git_dir(gitdir_మార్గం->buf, submodule_name))
		తిరిగి -1;

	కీలకం = xstrfmt("submodule.%s.gitdir", submodule_name);

	/* Nothing to do if the config already exists. */
	ఒకవేళ (!repo_config_get_string_tmp(the_repository, కీలకం, &విలువ)) {
		విడుదల(కీలకం);
		తిరిగి 0;
	}

	ఒకవేళ (repo_config_సమితి_gently(the_repository, కీలకం, gitdir_మార్గం->buf)) {
		విడుదల(కీలకం);
		తిరిగి -1;
	}

	విడుదల(కీలకం);
	తిరిగి 0;
}

స్థిర శూన్య create_default_gitdir_config(స్థిరరాశి అక్షరం *submodule_name)
{
	నిర్మాణం strbuf gitdir_మార్గం = STRBUF_INIT;
	నిర్మాణం git_hash_ctx ctx;
	అక్షరం hex_name_hash[GIT_MAX_HEXSZ + 1], శీర్షిక[128];
	గుర్తులేని అక్షరం raw_name_hash[GIT_MAX_RAWSZ];
	పూర్ణాంకం శీర్షిక_len;

	/* Case 1: try the plain module name */
	repo_git_మార్గం_append(the_repository, &gitdir_మార్గం, "modules/%s", submodule_name);
	ఒకవేళ (!validate_and_సమితి_submodule_gitdir(&gitdir_మార్గం, submodule_name)) {
		strbuf_release(&gitdir_మార్గం);
		తిరిగి;
	}

	/* Case 2.1: Try URI-safe (RFC3986) encoding first, this fixes nested gitdirs */
	strbuf_reset(&gitdir_మార్గం);
	repo_git_మార్గం_append(the_repository, &gitdir_మార్గం, "modules/");
	strbuf_addstr_urlencode(&gitdir_మార్గం, submodule_name, is_rfc3986_unreserved);
	ఒకవేళ (!validate_and_సమితి_submodule_gitdir(&gitdir_మార్గం, submodule_name)) {
		strbuf_release(&gitdir_మార్గం);
		తిరిగి;
	}

	/* Case 2.2: Try extended uppercase URI (RFC3986) encoding, to fix case-folding */
	strbuf_reset(&gitdir_మార్గం);
	repo_git_మార్గం_append(the_repository, &gitdir_మార్గం, "modules/");
	strbuf_addstr_urlencode(&gitdir_మార్గం, submodule_name, is_casefolding_rfc3986_unreserved);
	ఒకవేళ (!validate_and_సమితి_submodule_gitdir(&gitdir_మార్గం, submodule_name))
		తిరిగి;

	/* Case 2.3: Try some derived gitdir names, see if one sticks */
	కొరకు (అక్షరం c = '0'; c <= '9'; c++) {
		strbuf_reset(&gitdir_మార్గం);
		repo_git_మార్గం_append(the_repository, &gitdir_మార్గం, "modules/");
		strbuf_addstr_urlencode(&gitdir_మార్గం, submodule_name, is_rfc3986_unreserved);
		strbuf_addch(&gitdir_మార్గం, c);
		ఒకవేళ (!validate_and_సమితి_submodule_gitdir(&gitdir_మార్గం, submodule_name))
			తిరిగి;

		strbuf_reset(&gitdir_మార్గం);
		repo_git_మార్గం_append(the_repository, &gitdir_మార్గం, "modules/");
		strbuf_addstr_urlencode(&gitdir_మార్గం, submodule_name, is_casefolding_rfc3986_unreserved);
		strbuf_addch(&gitdir_మార్గం, c);
		ఒకవేళ (!validate_and_సమితి_submodule_gitdir(&gitdir_మార్గం, submodule_name))
			తిరిగి;
	}

	/* Case 2.4: If all the above failed, try a hash of the name as a last resort */
	శీర్షిక_len = snprintf(శీర్షిక, పరిమాణం(శీర్షిక),
			      "blob %"PRIuMAX, (uintmax_t)అక్షరశ్రేణి_పొడవు(submodule_name));
	git_hash_init(&ctx, the_hash_algo);
	git_hash_update(&ctx, శీర్షిక, శీర్షిక_len);
	git_hash_update(&ctx, "\0", 1);
	git_hash_update(&ctx, submodule_name, అక్షరశ్రేణి_పొడవు(submodule_name));
	git_hash_final(raw_name_hash, &ctx);
	hash_గమ్యం_hex_algop_r(hex_name_hash, raw_name_hash, the_hash_algo);
	strbuf_reset(&gitdir_మార్గం);
	repo_git_మార్గం_append(the_repository, &gitdir_మార్గం, "modules/%s", hex_name_hash);
	ఒకవేళ (!validate_and_సమితి_submodule_gitdir(&gitdir_మార్గం, submodule_name)) {
		strbuf_release(&gitdir_మార్గం);
		తిరిగి;
	}

	/* Case 3: nothing worked, error out */
	die(_("failed to set a valid default config for 'submodule.%s.gitdir'. "
	      "Please ensure it is set, for example by running something like: "
	      "'git config submodule.%s.gitdir .git/modules/%s'"),
	    submodule_name, submodule_name, submodule_name);
}

స్థిర శూన్య init_submodule(స్థిరరాశి అక్షరం *మార్గం, స్థిరరాశి అక్షరం *ఉపసర్గ,
			   స్థిరరాశి అక్షరం *super_ఉపసర్గ,
			   గుర్తులేని పూర్ణాంకం flags)
{
	స్థిరరాశి నిర్మాణం submodule *sub;
	నిర్మాణం strbuf sb = STRBUF_INIT;
	స్థిరరాశి అక్షరం *upd;
	అక్షరం *url = ఖాళీ, *displaypath;

	displaypath = get_submodule_displaypath(మార్గం, ఉపసర్గ, super_ఉపసర్గ);

	sub = submodule_from_మార్గం(the_repository, null_oid(the_hash_algo), మార్గం);

	ఒకవేళ (!sub)
		die(_("No url found for submodule path '%s' in .gitmodules"),
			displaypath);

	/*
	 * NEEDSWORK: In a multi-working-tree world, this needs to be
	 * set in the per-worktree config.
	 *
	 * Set active flag for the submodule being initialized
	 */
	ఒకవేళ (!is_submodule_చురుకైన(the_repository, మార్గం)) {
		strbuf_addf(&sb, "submodule.%s.active", sub->name);
		repo_config_సమితి_gently(the_repository, sb.buf, "true");
		strbuf_reset(&sb);
	}

	/*
	 * Copy url setting when it is not set yet.
	 * To look up the url in .git/config, we must not fall back to
	 * .gitmodules, so look it up directly.
	 */
	strbuf_addf(&sb, "submodule.%s.url", sub->name);
	ఒకవేళ (repo_config_get_string(the_repository, sb.buf, &url)) {
		ఒకవేళ (!sub->url)
			die(_("No url found for submodule path '%s' in .gitmodules"),
				displaypath);

		url = xstrdup(sub->url);

		/* Possibly a url relative to parent */
		ఒకవేళ (starts_with_dot_dot_slash(url) ||
		    starts_with_dot_slash(url)) {
			అక్షరం *oldurl = url;

			url = resolve_relative_url(oldurl, ఖాళీ, 0);
			విడుదల(oldurl);
		}

		ఒకవేళ (repo_config_సమితి_gently(the_repository, sb.buf, url))
			die(_("Failed to register url for submodule path '%s'"),
			    displaypath);
		ఒకవేళ (!(flags & OPT_QUIET))
			fprintf(stderr,
				_("Submodule '%s' (%s) registered for path '%s'\n"),
				sub->name, url, displaypath);
	}
	strbuf_reset(&sb);

	/* Copy "update" setting when it is not set yet */
	strbuf_addf(&sb, "submodule.%s.update", sub->name);
	ఒకవేళ (repo_config_get_string_tmp(the_repository, sb.buf, &upd) &&
	    sub->update_strategy.type != SM_UPDATE_UNSPECIFIED) {
		ఒకవేళ (sub->update_strategy.type == SM_UPDATE_COMMAND) {
			fprintf(stderr, _("warning: command update mode suggested for submodule '%s'\n"),
				sub->name);
			upd = "none";
		} లేకపోతే {
			upd = submodule_update_type_గమ్యం_string(sub->update_strategy.type);
		}

		ఒకవేళ (repo_config_సమితి_gently(the_repository, sb.buf, upd))
			die(_("Failed to register update mode for submodule path '%s'"), displaypath);
	}

	ఒకవేళ (the_repository->repository_format_submodule_మార్గం_cfg)
		create_default_gitdir_config(sub->name);

	strbuf_release(&sb);
	విడుదల(displaypath);
	విడుదల(url);
}

స్థిర శూన్య init_submodule_cb(స్థిరరాశి నిర్మాణం cache_నమోదు *list_అంశం, శూన్య *cb_దత్తాంశం)
{
	నిర్మాణం init_cb *info = cb_దత్తాంశం;

	init_submodule(list_అంశం->name, info->ఉపసర్గ, info->super_ఉపసర్గ,
		       info->flags);
}

స్థిర పూర్ణాంకం module_init(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
		       నిర్మాణం repository *repo వాడని)
{
	నిర్మాణం init_cb info = INIT_CB_INIT;
	నిర్మాణం pathspec pathspec = { 0 };
	నిర్మాణం module_list list = MODULE_LIST_INIT;
	పూర్ణాంకం quiet = 0;
	నిర్మాణం option module_init_options[] = {
		OPT__QUIET(&quiet, N_("suppress output for initializing a submodule")),
		OPT_చివర()
	};
	స్థిరరాశి అక్షరం *స్థిరరాశి git_submodule_helper_usage[] = {
		N_("git submodule init [<options>] [<path>]"),
		ఖాళీ
	};
	పూర్ణాంకం ret = 1;

	argc = parse_options(argc, argv, ఉపసర్గ, module_init_options,
			     git_submodule_helper_usage, 0);

	ఒకవేళ (module_list_compute(argv, ఉపసర్గ, &pathspec, &list) < 0)
		వెళ్ళు cleanup;

	/*
	 * If there are no path args and submodule.active is set then,
	 * by default, only initialize 'active' modules.
	 */
	ఒకవేళ (!argc && !repo_config_get(the_repository, "submodule.active"))
		module_list_చురుకైన(&list);

	info.ఉపసర్గ = ఉపసర్గ;
	ఒకవేళ (quiet)
		info.flags |= OPT_QUIET;

	for_each_listed_submodule(&list, init_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	తిరిగి ret;
}

నిర్మాణం స్థితి_cb {
	స్థిరరాశి అక్షరం *ఉపసర్గ;
	స్థిరరాశి అక్షరం *super_ఉపసర్గ;
	గుర్తులేని పూర్ణాంకం flags;
};
#నిర్వచించు స్థితి_CB_INIT { 0 }

స్థిర శూన్య print_స్థితి(గుర్తులేని పూర్ణాంకం flags, అక్షరం state, స్థిరరాశి అక్షరం *మార్గం,
			 స్థిరరాశి నిర్మాణం object_id *oid, స్థిరరాశి అక్షరం *displaypath)
{
	ఒకవేళ (flags & OPT_QUIET)
		తిరిగి;

	రాయి("%c%s %s", state, oid_గమ్యం_hex(oid), displaypath);

	ఒకవేళ (state == ' ' || state == '+') {
		అక్షరం *name = compute_rev_name(మార్గం, oid_గమ్యం_hex(oid));

		ఒకవేళ (name)
			రాయి(" (%s)", name);
		విడుదల(name);
	}

	రాయి("\n");
}

స్థిర పూర్ణాంకం handle_submodule_తల_ref(స్థిరరాశి నిర్మాణం reference *ref, శూన్య *cb_దత్తాంశం)
{
	నిర్మాణం object_id *output = cb_దత్తాంశం;

	ఒకవేళ (ref->oid)
		oidcpy(output, ref->oid);

	తిరిగి 0;
}

స్థిర శూన్య స్థితి_submodule(స్థిరరాశి అక్షరం *మార్గం, స్థిరరాశి నిర్మాణం object_id *ce_oid,
			     గుర్తులేని పూర్ణాంకం ce_flags, స్థిరరాశి అక్షరం *ఉపసర్గ,
			     స్థిరరాశి అక్షరం *super_ఉపసర్గ, గుర్తులేని పూర్ణాంకం flags)
{
	అక్షరం *displaypath;
	నిర్మాణం strvec diff_files_వాదనలు = STRVEC_INIT;
	నిర్మాణం rev_info rev = REV_INFO_INIT;
	నిర్మాణం strbuf buf = STRBUF_INIT;
	స్థిరరాశి అక్షరం *git_dir;

	ఒకవేళ (validate_submodule_మార్గం(మార్గం) < 0)
		die(ఖాళీ);

	ఒకవేళ (!submodule_from_మార్గం(the_repository, null_oid(the_hash_algo), మార్గం))
		die(_("no submodule mapping found in .gitmodules for path '%s'"),
		      మార్గం);

	displaypath = get_submodule_displaypath(మార్గం, ఉపసర్గ, super_ఉపసర్గ);

	ఒకవేళ ((CE_STAGEMASK & ce_flags) >> CE_STAGESHIFT) {
		print_స్థితి(flags, 'U', మార్గం, null_oid(the_hash_algo), displaypath);
		వెళ్ళు cleanup;
	}

	strbuf_addf(&buf, "%s/.git", మార్గం);
	git_dir = read_gitfile(buf.buf);
	ఒకవేళ (!git_dir)
		git_dir = buf.buf;

	ఒకవేళ (!is_submodule_చురుకైన(the_repository, మార్గం) ||
	    !is_git_directory(git_dir)) {
		print_స్థితి(flags, '-', మార్గం, ce_oid, displaypath);
		strbuf_release(&buf);
		వెళ్ళు cleanup;
	}
	strbuf_release(&buf);

	strvec_pushl(&diff_files_వాదనలు, "diff-files",
		     "--ignore-submodules=dirty", "--quiet", "--",
		     మార్గం, ఖాళీ);

	repo_config(the_repository, git_diff_basic_config, ఖాళీ);

	repo_init_revisions(the_repository, &rev, ఖాళీ);
	rev.abbrev = 0;
	అమరిక_revisions_from_strvec(&diff_files_వాదనలు, &rev, ఖాళీ);
	నిరంతరం_diff_files(&rev, 0);

	ఒకవేళ (!diff_ఫలితం_సంకేతం(&rev)) {
		print_స్థితి(flags, ' ', మార్గం, ce_oid,
			     displaypath);
	} లేకపోతే ఒకవేళ (!(flags & OPT_CACHED)) {
		నిర్మాణం object_id oid;
		నిర్మాణం ref_store *refs = repo_get_submodule_ref_store(the_repository,
								      మార్గం);

		ఒకవేళ (!refs) {
			print_స్థితి(flags, '-', మార్గం, ce_oid, displaypath);
			వెళ్ళు cleanup;
		}
		ఒకవేళ (refs_తల_ref(refs, handle_submodule_తల_ref, &oid))
			die(_("could not resolve HEAD ref inside the "
			      "submodule '%s'"), మార్గం);

		print_స్థితి(flags, '+', మార్గం, &oid, displaypath);
	} లేకపోతే {
		print_స్థితి(flags, '+', మార్గం, ce_oid, displaypath);
	}

	ఒకవేళ (flags & OPT_RECURSIVE) {
		నిర్మాణం బిడ్డ_process cpr = బిడ్డ_PROCESS_INIT;
		పూర్ణాంకం res;

		cpr.git_cmd = 1;
		cpr.dir = మార్గం;
		prepare_submodule_repo_env(&cpr.env);

		strvec_pushl(&cpr.వాదనలు, "submodule--helper", "status",
			     "--recursive", ఖాళీ);
		strvec_pushf(&cpr.వాదనలు, "--super-prefix=%s/", displaypath);

		ఒకవేళ (flags & OPT_CACHED)
			strvec_నెట్టు(&cpr.వాదనలు, "--cached");

		ఒకవేళ (flags & OPT_QUIET)
			strvec_నెట్టు(&cpr.వాదనలు, "--quiet");

		res = నిరంతరం_command(&cpr);
		ఒకవేళ (res == SIGPIPE + 128)
			లేపు(SIGPIPE);
		లేకపోతే ఒకవేళ (res)
			die(_("failed to recurse into submodule '%s'"), మార్గం);
	}

cleanup:
	strvec_clear(&diff_files_వాదనలు);
	విడుదల(displaypath);
	release_revisions(&rev);
}

స్థిర శూన్య స్థితి_submodule_cb(స్థిరరాశి నిర్మాణం cache_నమోదు *list_అంశం,
				శూన్య *cb_దత్తాంశం)
{
	నిర్మాణం స్థితి_cb *info = cb_దత్తాంశం;

	స్థితి_submodule(list_అంశం->name, &list_అంశం->oid, list_అంశం->ce_flags,
			 info->ఉపసర్గ, info->super_ఉపసర్గ, info->flags);
}

స్థిర పూర్ణాంకం module_స్థితి(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
			 నిర్మాణం repository *repo వాడని)
{
	నిర్మాణం స్థితి_cb info = స్థితి_CB_INIT;
	నిర్మాణం pathspec pathspec = { 0 };
	నిర్మాణం module_list list = MODULE_LIST_INIT;
	పూర్ణాంకం quiet = 0;
	నిర్మాణం option module_స్థితి_options[] = {
		OPT_SUPER_ఉపసర్గ(&info.super_ఉపసర్గ),
		OPT__QUIET(&quiet, N_("suppress submodule status output")),
		OPT_బిట్టు(0, "cached", &info.flags, N_("use commit stored in the index instead of the one stored in the submodule HEAD"), OPT_CACHED),
		OPT_బిట్టు(0, "recursive", &info.flags, N_("recurse into nested submodules"), OPT_RECURSIVE),
		OPT_చివర()
	};
	స్థిరరాశి అక్షరం *స్థిరరాశి git_submodule_helper_usage[] = {
		N_("git submodule status [--quiet] [--cached] [--recursive] [<path>...]"),
		ఖాళీ
	};
	పూర్ణాంకం ret = 1;

	argc = parse_options(argc, argv, ఉపసర్గ, module_స్థితి_options,
			     git_submodule_helper_usage, 0);

	ఒకవేళ (module_list_compute(argv, ఉపసర్గ, &pathspec, &list) < 0)
		వెళ్ళు cleanup;

	info.ఉపసర్గ = ఉపసర్గ;
	ఒకవేళ (quiet)
		info.flags |= OPT_QUIET;

	for_each_listed_submodule(&list, స్థితి_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	తిరిగి ret;
}

నిర్మాణం module_cb {
	గుర్తులేని పూర్ణాంకం mod_src;
	గుర్తులేని పూర్ణాంకం mod_dst;
	నిర్మాణం object_id oid_src;
	నిర్మాణం object_id oid_dst;
	అక్షరం స్థితి_2;
	అక్షరం *sm_మార్గం;
};
#నిర్వచించు MODULE_CB_INIT { 0 }

స్థిర శూన్య module_cb_release(నిర్మాణం module_cb *mcb)
{
	విడుదల(mcb->sm_మార్గం);
}

నిర్మాణం module_cb_list {
	నిర్మాణం module_cb **entries;
	పూర్ణాంకం alloc, nr;
};
#నిర్వచించు MODULE_CB_LIST_INIT { 0 }

స్థిర శూన్య module_cb_list_release(నిర్మాణం module_cb_list *mcbl)
{
	పూర్ణాంకం i;

	కొరకు (i = 0; i < mcbl->nr; i++) {
		నిర్మాణం module_cb *mcb = mcbl->entries[i];

		module_cb_release(mcb);
		విడుదల(mcb);
	}
	విడుదల(mcbl->entries);
}

నిర్మాణం summary_cb {
	పూర్ణాంకం argc;
	స్థిరరాశి అక్షరం **argv;
	స్థిరరాశి అక్షరం *ఉపసర్గ;
	స్థిరరాశి అక్షరం *super_ఉపసర్గ;
	గుర్తులేని పూర్ణాంకం cached: 1;
	గుర్తులేని పూర్ణాంకం for_స్థితి: 1;
	గుర్తులేని పూర్ణాంకం files: 1;
	పూర్ణాంకం summary_హద్దు;
};
#నిర్వచించు SUMMARY_CB_INIT { 0 }

గణన diff_cmd {
	DIFF_సూచీ,
	DIFF_FILES
};

స్థిర అక్షరం *verify_submodule_committish(స్థిరరాశి అక్షరం *sm_మార్గం,
					 స్థిరరాశి అక్షరం *committish)
{
	నిర్మాణం బిడ్డ_process cp_rev_parse = బిడ్డ_PROCESS_INIT;
	నిర్మాణం strbuf ఫలితం = STRBUF_INIT;

	cp_rev_parse.git_cmd = 1;
	cp_rev_parse.dir = sm_మార్గం;
	prepare_submodule_repo_env(&cp_rev_parse.env);
	strvec_pushl(&cp_rev_parse.వాదనలు, "rev-parse", "-q", "--short", ఖాళీ);
	strvec_pushf(&cp_rev_parse.వాదనలు, "%s^0", committish);
	strvec_నెట్టు(&cp_rev_parse.వాదనలు, "--");

	ఒకవేళ (capture_command(&cp_rev_parse, &ఫలితం, 0))
		తిరిగి ఖాళీ;

	strbuf_trim_trailing_newline(&ఫలితం);
	తిరిగి strbuf_detach(&ఫలితం, ఖాళీ);
}

స్థిర శూన్య print_submodule_summary(నిర్మాణం summary_cb *info, స్థిరరాశి అక్షరం *errmsg,
				    పూర్ణాంకం మొత్తం_commits, స్థిరరాశి అక్షరం *displaypath,
				    స్థిరరాశి అక్షరం *src_abbrev, స్థిరరాశి అక్షరం *dst_abbrev,
				    నిర్మాణం module_cb *p)
{
	ఒకవేళ (p->స్థితి_2 == 'T') {
		ఒకవేళ (S_ISGITLINK(p->mod_dst))
			రాయి(_("* %s %s(blob)->%s(submodule)"),
				 displaypath, src_abbrev, dst_abbrev);
		లేకపోతే
			రాయి(_("* %s %s(submodule)->%s(blob)"),
				 displaypath, src_abbrev, dst_abbrev);
	} లేకపోతే {
		రాయి("* %s %s...%s",
			displaypath, src_abbrev, dst_abbrev);
	}

	ఒకవేళ (మొత్తం_commits < 0)
		రాయి(":\n");
	లేకపోతే
		రాయి(" (%d):\n", మొత్తం_commits);

	ఒకవేళ (errmsg) {
		రాయి(_("%s"), errmsg);
	} లేకపోతే ఒకవేళ (మొత్తం_commits > 0) {
		నిర్మాణం బిడ్డ_process cp_log = బిడ్డ_PROCESS_INIT;

		cp_log.git_cmd = 1;
		cp_log.dir = p->sm_మార్గం;
		prepare_submodule_repo_env(&cp_log.env);
		strvec_pushl(&cp_log.వాదనలు, "log", ఖాళీ);

		ఒకవేళ (S_ISGITLINK(p->mod_src) && S_ISGITLINK(p->mod_dst)) {
			ఒకవేళ (info->summary_హద్దు > 0)
				strvec_pushf(&cp_log.వాదనలు, "-%d",
					     info->summary_హద్దు);

			strvec_pushl(&cp_log.వాదనలు, "--pretty=  %m %s",
				     "--first-parent", ఖాళీ);
			strvec_pushf(&cp_log.వాదనలు, "%s...%s",
				     src_abbrev, dst_abbrev);
		} లేకపోతే ఒకవేళ (S_ISGITLINK(p->mod_dst)) {
			strvec_pushl(&cp_log.వాదనలు, "--pretty=  > %s",
				     "-1", dst_abbrev, ఖాళీ);
		} లేకపోతే {
			strvec_pushl(&cp_log.వాదనలు, "--pretty=  < %s",
				     "-1", src_abbrev, ఖాళీ);
		}
		నిరంతరం_command(&cp_log);
	}
	రాయి("\n");
}

స్థిర శూన్య generate_submodule_summary(నిర్మాణం summary_cb *info,
				       నిర్మాణం module_cb *p)
{
	అక్షరం *displaypath, *src_abbrev = ఖాళీ, *dst_abbrev;
	పూర్ణాంకం missing_src = 0, missing_dst = 0;
	నిర్మాణం strbuf errmsg = STRBUF_INIT;
	పూర్ణాంకం మొత్తం_commits = -1;

	ఒకవేళ (!info->cached && oideq(&p->oid_dst, null_oid(the_hash_algo))) {
		ఒకవేళ (S_ISGITLINK(p->mod_dst)) {
			నిర్మాణం ref_store *refs = repo_get_submodule_ref_store(the_repository,
									      p->sm_మార్గం);

			ఒకవేళ (refs)
				refs_తల_ref(refs, handle_submodule_తల_ref, &p->oid_dst);
		} లేకపోతే ఒకవేళ (S_ISLNK(p->mod_dst) || S_ISREG(p->mod_dst)) {
			నిర్మాణం stat st;
			పూర్ణాంకం fd = open(p->sm_మార్గం, O_RDONLY);

			ఒకవేళ (fd < 0 || fstat(fd, &st) < 0 ||
			    సూచీ_fd(the_repository->సూచీ, &p->oid_dst, fd, &st, OBJ_BLOB,
				     p->sm_మార్గం, 0))
				error(_("couldn't hash object from '%s'"), p->sm_మార్గం);
		} లేకపోతే {
			/* for a submodule removal (mode:0000000), don't warn */
			ఒకవేళ (p->mod_dst)
				warning(_("unexpected mode %o"), p->mod_dst);
		}
	}

	ఒకవేళ (S_ISGITLINK(p->mod_src)) {
		ఒకవేళ (p->స్థితి_2 != 'D')
			src_abbrev = verify_submodule_committish(p->sm_మార్గం,
								 oid_గమ్యం_hex(&p->oid_src));
		ఒకవేళ (!src_abbrev) {
			missing_src = 1;
			/*
			 * As `rev-parse` failed, we fallback to getting
			 * the abbreviated hash using oid_src. We do
			 * this as we might still need the abbreviated
			 * hash in cases like a submodule type change, etc.
			 */
			src_abbrev = xstrndup(oid_గమ్యం_hex(&p->oid_src), 7);
		}
	} లేకపోతే {
		/*
		 * The source does not point to a submodule.
		 * So, we fallback to getting the abbreviation using
		 * oid_src as we might still need the abbreviated
		 * hash in cases like submodule add, etc.
		 */
		src_abbrev = xstrndup(oid_గమ్యం_hex(&p->oid_src), 7);
	}

	ఒకవేళ (S_ISGITLINK(p->mod_dst)) {
		dst_abbrev = verify_submodule_committish(p->sm_మార్గం,
							 oid_గమ్యం_hex(&p->oid_dst));
		ఒకవేళ (!dst_abbrev) {
			missing_dst = 1;
			/*
			 * As `rev-parse` failed, we fallback to getting
			 * the abbreviated hash using oid_dst. We do
			 * this as we might still need the abbreviated
			 * hash in cases like a submodule type change, etc.
			 */
			dst_abbrev = xstrndup(oid_గమ్యం_hex(&p->oid_dst), 7);
		}
	} లేకపోతే {
		/*
		 * The destination does not point to a submodule.
		 * So, we fallback to getting the abbreviation using
		 * oid_dst as we might still need the abbreviated
		 * hash in cases like a submodule removal, etc.
		 */
		dst_abbrev = xstrndup(oid_గమ్యం_hex(&p->oid_dst), 7);
	}

	displaypath = get_submodule_displaypath(p->sm_మార్గం, info->ఉపసర్గ,
						info->super_ఉపసర్గ);

	ఒకవేళ (!missing_src && !missing_dst) {
		నిర్మాణం బిడ్డ_process cp_rev_list = బిడ్డ_PROCESS_INIT;
		నిర్మాణం strbuf sb_rev_list = STRBUF_INIT;

		strvec_pushl(&cp_rev_list.వాదనలు, "rev-list",
			     "--first-parent", "--count", ఖాళీ);
		ఒకవేళ (S_ISGITLINK(p->mod_src) && S_ISGITLINK(p->mod_dst))
			strvec_pushf(&cp_rev_list.వాదనలు, "%s...%s",
				     src_abbrev, dst_abbrev);
		లేకపోతే
			strvec_నెట్టు(&cp_rev_list.వాదనలు, S_ISGITLINK(p->mod_src) ?
				    src_abbrev : dst_abbrev);
		strvec_నెట్టు(&cp_rev_list.వాదనలు, "--");

		cp_rev_list.git_cmd = 1;
		cp_rev_list.dir = p->sm_మార్గం;
		prepare_submodule_repo_env(&cp_rev_list.env);

		ఒకవేళ (!capture_command(&cp_rev_list, &sb_rev_list, 0))
			మొత్తం_commits = atoi(sb_rev_list.buf);

		strbuf_release(&sb_rev_list);
	} లేకపోతే {
		/*
		 * Don't give error msg for modification whose dst is not
		 * submodule, i.e., deleted or changed to blob
		 */
		ఒకవేళ (S_ISGITLINK(p->mod_dst)) {
			ఒకవేళ (missing_src && missing_dst) {
				strbuf_addf(&errmsg, "  Warn: %s doesn't contain commits %s and %s\n",
					    displaypath, oid_గమ్యం_hex(&p->oid_src),
					    oid_గమ్యం_hex(&p->oid_dst));
			} లేకపోతే {
				strbuf_addf(&errmsg, "  Warn: %s doesn't contain commit %s\n",
					    displaypath, missing_src ?
					    oid_గమ్యం_hex(&p->oid_src) :
					    oid_గమ్యం_hex(&p->oid_dst));
			}
		}
	}

	print_submodule_summary(info, errmsg.len ? errmsg.buf : ఖాళీ,
				మొత్తం_commits, displaypath, src_abbrev,
				dst_abbrev, p);

	విడుదల(displaypath);
	విడుదల(src_abbrev);
	విడుదల(dst_abbrev);
	strbuf_release(&errmsg);
}

స్థిర శూన్య prepare_submodule_summary(నిర్మాణం summary_cb *info,
				      నిర్మాణం module_cb_list *list)
{
	పూర్ణాంకం i;
	కొరకు (i = 0; i < list->nr; i++) {
		స్థిరరాశి నిర్మాణం submodule *sub;
		నిర్మాణం module_cb *p = list->entries[i];
		నిర్మాణం strbuf sm_gitdir = STRBUF_INIT;

		ఒకవేళ (p->స్థితి_2 == 'D' || p->స్థితి_2 == 'T') {
			generate_submodule_summary(info, p);
			కొనసాగు;
		}

		ఒకవేళ (info->for_స్థితి && p->స్థితి_2 != 'A' &&
		    (sub = submodule_from_మార్గం(the_repository,
					       null_oid(the_hash_algo), p->sm_మార్గం))) {
			అక్షరం *config_కీలకం = ఖాళీ;
			స్థిరరాశి అక్షరం *విలువ;
			పూర్ణాంకం ignore_all = 0;

			config_కీలకం = xstrfmt("submodule.%s.ignore",
					     sub->name);
			ఒకవేళ (!repo_config_get_string_tmp(the_repository, config_కీలకం, &విలువ))
				ignore_all = !strcmp(విలువ, "all");
			లేకపోతే ఒకవేళ (sub->ignore)
				ignore_all = !strcmp(sub->ignore, "all");

			విడుదల(config_కీలకం);
			ఒకవేళ (ignore_all)
				కొనసాగు;
		}

		/* Also show added or modified modules which are checked out */
		strbuf_addstr(&sm_gitdir, p->sm_మార్గం);
		ఒకవేళ (is_nonbare_repository_dir(&sm_gitdir))
			generate_submodule_summary(info, p);
		strbuf_release(&sm_gitdir);
	}
}

స్థిర శూన్య submodule_summary_callback(నిర్మాణం diff_వరుస_struct *q,
				       నిర్మాణం diff_options *options వాడని,
				       శూన్య *దత్తాంశం)
{
	పూర్ణాంకం i;
	నిర్మాణం module_cb_list *list = దత్తాంశం;
	కొరకు (i = 0; i < q->nr; i++) {
		నిర్మాణం diff_filepair *p = q->వరుస[i];
		నిర్మాణం module_cb *తాత్కాలిక;

		ఒకవేళ (!S_ISGITLINK(p->one->బహుళకం) && !S_ISGITLINK(p->two->బహుళకం))
			కొనసాగు;
		తాత్కాలిక = xmalloc(పరిమాణం(*తాత్కాలిక));
		తాత్కాలిక->mod_src = p->one->బహుళకం;
		తాత్కాలిక->mod_dst = p->two->బహుళకం;
		తాత్కాలిక->oid_src = p->one->oid;
		తాత్కాలిక->oid_dst = p->two->oid;
		తాత్కాలిక->స్థితి_2 = p->స్థితి_2;
		తాత్కాలిక->sm_మార్గం = xstrdup(p->one->మార్గం);

		ALLOC_GROW(list->entries, list->nr + 1, list->alloc);
		list->entries[list->nr++] = తాత్కాలిక;
	}
}

స్థిర స్థిరరాశి అక్షరం *get_diff_cmd(గణన diff_cmd diff_cmd)
{
	ఎంపిక (diff_cmd) {
	స్థితి DIFF_సూచీ: తిరిగి "diff-index";
	స్థితి DIFF_FILES: తిరిగి "diff-files";
	అప్రమేయ: BUG("bad diff_cmd value %d", diff_cmd);
	}
}

స్థిర పూర్ణాంకం compute_summary_module_list(నిర్మాణం object_id *తల_oid,
				       నిర్మాణం summary_cb *info,
				       గణన diff_cmd diff_cmd)
{
	నిర్మాణం strvec diff_వాదనలు = STRVEC_INIT;
	నిర్మాణం rev_info rev;
	నిర్మాణం module_cb_list list = MODULE_CB_LIST_INIT;
	పూర్ణాంకం ret = 0;

	strvec_నెట్టు(&diff_వాదనలు, get_diff_cmd(diff_cmd));
	ఒకవేళ (info->cached)
		strvec_నెట్టు(&diff_వాదనలు, "--cached");
	strvec_pushl(&diff_వాదనలు, "--ignore-submodules=dirty", "--raw", ఖాళీ);
	ఒకవేళ (తల_oid)
		strvec_నెట్టు(&diff_వాదనలు, oid_గమ్యం_hex(తల_oid));
	strvec_నెట్టు(&diff_వాదనలు, "--");
	ఒకవేళ (info->argc)
		strvec_pushv(&diff_వాదనలు, info->argv);

	repo_config(the_repository, git_diff_basic_config, ఖాళీ);
	repo_init_revisions(the_repository, &rev, info->ఉపసర్గ);
	rev.abbrev = 0;
	precompose_argv_ఉపసర్గ(diff_వాదనలు.nr, diff_వాదనలు.v, ఖాళీ);
	అమరిక_revisions_from_strvec(&diff_వాదనలు, &rev, ఖాళీ);
	rev.diffopt.output_format = DIFF_FORMAT_NO_OUTPUT | DIFF_FORMAT_CALLBACK;
	rev.diffopt.format_callback = submodule_summary_callback;
	rev.diffopt.format_callback_దత్తాంశం = &list;

	ఒకవేళ (!info->cached) {
		ఒకవేళ (diff_cmd == DIFF_సూచీ)
			అమరిక_work_వృక్షం(the_repository);
		ఒకవేళ (repo_read_సూచీ_preload(the_repository, &rev.diffopt.pathspec, 0) < 0) {
			perror("repo_read_index_preload");
			ret = -1;
			వెళ్ళు cleanup;
		}
	} లేకపోతే ఒకవేళ (repo_read_సూచీ(the_repository) < 0) {
		perror("repo_read_cache");
		ret = -1;
		వెళ్ళు cleanup;
	}

	ఒకవేళ (diff_cmd == DIFF_సూచీ)
		నిరంతరం_diff_సూచీ(&rev, info->cached ? DIFF_సూచీ_CACHED : 0);
	లేకపోతే
		నిరంతరం_diff_files(&rev, 0);
	prepare_submodule_summary(info, &list);
cleanup:
	strvec_clear(&diff_వాదనలు);
	release_revisions(&rev);
	module_cb_list_release(&list);
	తిరిగి ret;
}

స్థిర పూర్ణాంకం module_summary(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
			  నిర్మాణం repository *repo వాడని)
{
	నిర్మాణం summary_cb info = SUMMARY_CB_INIT;
	పూర్ణాంకం cached = 0;
	పూర్ణాంకం for_స్థితి = 0;
	పూర్ణాంకం files = 0;
	పూర్ణాంకం summary_హద్దు = -1;
	గణన diff_cmd diff_cmd = DIFF_సూచీ;
	నిర్మాణం object_id తల_oid;
	పూర్ణాంకం ret;
	నిర్మాణం option module_summary_options[] = {
		OPT_BOOL(0, "cached", &cached,
			 N_("use the commit stored in the index instead of the submodule HEAD")),
		OPT_BOOL(0, "files", &files,
			 N_("compare the commit in the index with that in the submodule HEAD")),
		OPT_BOOL(0, "for-status", &for_స్థితి,
			 N_("skip submodules with 'ignore_config' value set to 'all'")),
		OPT_INTEGER('n', "summary-limit", &summary_హద్దు,
			     N_("limit the summary size")),
		OPT_చివర()
	};
	స్థిరరాశి అక్షరం *స్థిరరాశి git_submodule_helper_usage[] = {
		N_("git submodule summary [<options>] [<commit>] [--] [<path>]"),
		ఖాళీ
	};

	argc = parse_options(argc, argv, ఉపసర్గ, module_summary_options,
			     git_submodule_helper_usage, 0);

	ఒకవేళ (!summary_హద్దు)
		తిరిగి 0;

	ఒకవేళ (!repo_get_oid(the_repository, argc ? argv[0] : "HEAD", &తల_oid)) {
		ఒకవేళ (argc) {
			argv++;
			argc--;
		}
	} లేకపోతే ఒకవేళ (!argc || !strcmp(argv[0], "HEAD")) {
		/* before the first commit: compare with an empty tree */
		oidcpy(&తల_oid, the_hash_algo->ఖాళీలు_వృక్షం);
		ఒకవేళ (argc) {
			argv++;
			argc--;
		}
	} లేకపోతే {
		ఒకవేళ (repo_get_oid(the_repository, "HEAD", &తల_oid))
			die(_("could not fetch a revision for HEAD"));
	}

	ఒకవేళ (files) {
		ఒకవేళ (cached)
			die(_("options '%s' and '%s' cannot be used together"), "--cached", "--files");
		diff_cmd = DIFF_FILES;
	}

	info.argc = argc;
	info.argv = argv;
	info.ఉపసర్గ = ఉపసర్గ;
	info.cached = !!cached;
	info.files = !!files;
	info.for_స్థితి = !!for_స్థితి;
	info.summary_హద్దు = summary_హద్దు;

	ret = compute_summary_module_list((diff_cmd == DIFF_సూచీ) ? &తల_oid : ఖాళీ,
					  &info, diff_cmd);
	తిరిగి ret;
}

స్థిర పూర్ణాంకం module_gitdir(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ వాడని,
			 నిర్మాణం repository *repo)
{
	నిర్మాణం strbuf gitdir = STRBUF_INIT;

	ఒకవేళ (argc != 2)
		usage(_("git submodule--helper gitdir <name>"));

	submodule_name_గమ్యం_gitdir(&gitdir, repo, argv[1]);

	రాయి("%s\n", gitdir.buf);

	strbuf_release(&gitdir);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం module_migrate(పూర్ణాంకం argc వాడని, స్థిరరాశి అక్షరం **argv వాడని,
			  స్థిరరాశి అక్షరం *ఉపసర్గ వాడని, నిర్మాణం repository *repo)
{
	నిర్మాణం strbuf module_dir = STRBUF_INIT;
	DIR *dir;
	నిర్మాణం dirent *de;
	పూర్ణాంకం repo_సంస్కరణ = 0;

	repo_git_మార్గం_append(repo, &module_dir, "modules/");

	dir = opendir(module_dir.buf);
	ఒకవేళ (!dir)
		die(_("could not open '%s'"), module_dir.buf);

	అయితే ((de = readdir(dir))) {
		నిర్మాణం strbuf gitdir_మార్గం = STRBUF_INIT;
		అక్షరం *కీలకం;
		స్థిరరాశి అక్షరం *విలువ;

		ఒకవేళ (is_dot_or_dotdot(de->d_name))
			కొనసాగు;

		strbuf_addf(&gitdir_మార్గం, "%s/%s", module_dir.buf, de->d_name);
		ఒకవేళ (!is_git_directory(gitdir_మార్గం.buf)) {
			strbuf_release(&gitdir_మార్గం);
			కొనసాగు;
		}
		strbuf_release(&gitdir_మార్గం);

		కీలకం = xstrfmt("submodule.%s.gitdir", de->d_name);
		ఒకవేళ (!repo_config_get_string_tmp(repo, కీలకం, &విలువ)) {
			/* Already has a gitdir config, nothing to do. */
			విడుదల(కీలకం);
			కొనసాగు;
		}
		విడుదల(కీలకం);

		create_default_gitdir_config(de->d_name);
	}

	closedir(dir);
	strbuf_release(&module_dir);

	repo_config_get_int(the_repository, "core.repositoryformatversion", &repo_సంస్కరణ);
	ఒకవేళ (repo_సంస్కరణ == 0 &&
	    repo_config_సమితి_gently(repo, "core.repositoryformatversion", "1"))
		die(_("could not set core.repositoryformatversion to 1.\n"
		      "Please set it for migration to work, for example:\n"
		      "git config core.repositoryformatversion 1"));

	ఒకవేళ (repo_config_సమితి_gently(repo, "extensions.submodulePathConfig", "true"))
		die(_("could not enable submodulePathConfig extension. It is required\n"
		      "for migration to work. Please enable it in the root repo:\n"
		      "git config extensions.submodulePathConfig true"));

	repo->repository_format_submodule_మార్గం_cfg = 1;

	తిరిగి 0;
}

నిర్మాణం sync_cb {
	స్థిరరాశి అక్షరం *ఉపసర్గ;
	స్థిరరాశి అక్షరం *super_ఉపసర్గ;
	గుర్తులేని పూర్ణాంకం flags;
};
#నిర్వచించు SYNC_CB_INIT { 0 }

స్థిర శూన్య sync_submodule(స్థిరరాశి అక్షరం *మార్గం, స్థిరరాశి అక్షరం *ఉపసర్గ,
			   స్థిరరాశి అక్షరం *super_ఉపసర్గ, గుర్తులేని పూర్ణాంకం flags)
{
	స్థిరరాశి నిర్మాణం submodule *sub;
	అక్షరం *remote_కీలకం = ఖాళీ;
	అక్షరం *sub_origin_url, *super_config_url, *displaypath, *default_remote;
	నిర్మాణం strbuf sb = STRBUF_INIT;
	అక్షరం *sub_config_మార్గం = ఖాళీ;
	పూర్ణాంకం సంకేతం_2;

	ఒకవేళ (!is_submodule_చురుకైన(the_repository, మార్గం))
		తిరిగి;

	ఒకవేళ (validate_submodule_మార్గం(మార్గం) < 0)
		die(ఖాళీ);

	sub = submodule_from_మార్గం(the_repository, null_oid(the_hash_algo), మార్గం);

	ఒకవేళ (sub && sub->url) {
		ఒకవేళ (starts_with_dot_dot_slash(sub->url) ||
		    starts_with_dot_slash(sub->url)) {
			అక్షరం *పైకి_మార్గం = get_పైకి_మార్గం(మార్గం);

			sub_origin_url = resolve_relative_url(sub->url, పైకి_మార్గం, 1);
			super_config_url = resolve_relative_url(sub->url, ఖాళీ, 1);
			విడుదల(పైకి_మార్గం);
		} లేకపోతే {
			sub_origin_url = xstrdup(sub->url);
			super_config_url = xstrdup(sub->url);
		}
	} లేకపోతే {
		sub_origin_url = xstrdup("");
		super_config_url = xstrdup("");
	}

	displaypath = get_submodule_displaypath(మార్గం, ఉపసర్గ, super_ఉపసర్గ);

	ఒకవేళ (!(flags & OPT_QUIET))
		రాయి(_("Synchronizing submodule url for '%s'\n"),
			 displaypath);

	strbuf_reset(&sb);
	strbuf_addf(&sb, "submodule.%s.url", sub->name);
	ఒకవేళ (repo_config_సమితి_gently(the_repository, sb.buf, super_config_url))
		die(_("failed to register url for submodule path '%s'"),
		      displaypath);

	ఒకవేళ (!is_submodule_populated_gently(మార్గం, ఖాళీ))
		వెళ్ళు cleanup;

	strbuf_reset(&sb);
	సంకేతం_2 = get_default_remote_submodule(మార్గం, &default_remote);
	ఒకవేళ (సంకేతం_2)
		నిష్క్రమించు(సంకేతం_2);

	remote_కీలకం = xstrfmt("remote.%s.url", default_remote);
	విడుదల(default_remote);

	submodule_గమ్యం_gitdir(the_repository, &sb, మార్గం);
	strbuf_addstr(&sb, "/config");

	ఒకవేళ (repo_config_సమితి_లోపల_file_gently(the_repository, sb.buf, remote_కీలకం, ఖాళీ, sub_origin_url))
		die(_("failed to update remote for submodule '%s'"),
		      మార్గం);

	ఒకవేళ (flags & OPT_RECURSIVE) {
		నిర్మాణం బిడ్డ_process cpr = బిడ్డ_PROCESS_INIT;

		cpr.git_cmd = 1;
		cpr.dir = మార్గం;
		prepare_submodule_repo_env(&cpr.env);

		strvec_pushl(&cpr.వాదనలు, "submodule--helper", "sync",
			     "--recursive", ఖాళీ);
		strvec_pushf(&cpr.వాదనలు, "--super-prefix=%s/", displaypath);

		ఒకవేళ (flags & OPT_QUIET)
			strvec_నెట్టు(&cpr.వాదనలు, "--quiet");

		ఒకవేళ (నిరంతరం_command(&cpr))
			die(_("failed to recurse into submodule '%s'"),
			      మార్గం);
	}

cleanup:
	విడుదల(super_config_url);
	విడుదల(sub_origin_url);
	strbuf_release(&sb);
	విడుదల(remote_కీలకం);
	విడుదల(displaypath);
	విడుదల(sub_config_మార్గం);
}

స్థిర శూన్య sync_submodule_cb(స్థిరరాశి నిర్మాణం cache_నమోదు *list_అంశం, శూన్య *cb_దత్తాంశం)
{
	నిర్మాణం sync_cb *info = cb_దత్తాంశం;

	sync_submodule(list_అంశం->name, info->ఉపసర్గ, info->super_ఉపసర్గ,
		       info->flags);
}

స్థిర పూర్ణాంకం module_sync(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
		       నిర్మాణం repository *repo వాడని)
{
	నిర్మాణం sync_cb info = SYNC_CB_INIT;
	నిర్మాణం pathspec pathspec = { 0 };
	నిర్మాణం module_list list = MODULE_LIST_INIT;
	పూర్ణాంకం quiet = 0;
	పూర్ణాంకం recursive = 0;
	నిర్మాణం option module_sync_options[] = {
		OPT_SUPER_ఉపసర్గ(&info.super_ఉపసర్గ),
		OPT__QUIET(&quiet, N_("suppress output of synchronizing submodule url")),
		OPT_BOOL(0, "recursive", &recursive,
			N_("recurse into nested submodules")),
		OPT_చివర()
	};
	స్థిరరాశి అక్షరం *స్థిరరాశి git_submodule_helper_usage[] = {
		N_("git submodule sync [--quiet] [--recursive] [<path>]"),
		ఖాళీ
	};
	పూర్ణాంకం ret = 1;

	argc = parse_options(argc, argv, ఉపసర్గ, module_sync_options,
			     git_submodule_helper_usage, 0);

	ఒకవేళ (module_list_compute(argv, ఉపసర్గ, &pathspec, &list) < 0)
		వెళ్ళు cleanup;

	info.ఉపసర్గ = ఉపసర్గ;
	ఒకవేళ (quiet)
		info.flags |= OPT_QUIET;
	ఒకవేళ (recursive)
		info.flags |= OPT_RECURSIVE;

	for_each_listed_submodule(&list, sync_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	తిరిగి ret;
}

నిర్మాణం deinit_cb {
	స్థిరరాశి అక్షరం *ఉపసర్గ;
	గుర్తులేని పూర్ణాంకం flags;
};
#నిర్వచించు DEINIT_CB_INIT { 0 }

స్థిర శూన్య deinit_submodule(స్థిరరాశి అక్షరం *మార్గం, స్థిరరాశి అక్షరం *ఉపసర్గ,
			     గుర్తులేని పూర్ణాంకం flags)
{
	స్థిరరాశి నిర్మాణం submodule *sub;
	అక్షరం *displaypath = ఖాళీ;
	నిర్మాణం బిడ్డ_process cp_config = బిడ్డ_PROCESS_INIT;
	నిర్మాణం strbuf sb_config = STRBUF_INIT;
	అక్షరం *sub_git_dir = xstrfmt("%s/.git", మార్గం);

	ఒకవేళ (validate_submodule_మార్గం(మార్గం) < 0)
		die(ఖాళీ);

	sub = submodule_from_మార్గం(the_repository, null_oid(the_hash_algo), మార్గం);

	ఒకవేళ (!sub || !sub->name)
		వెళ్ళు cleanup;

	displaypath = get_submodule_displaypath(మార్గం, ఉపసర్గ, ఖాళీ);

	/* remove the submodule work tree (unless the user already did it) */
	ఒకవేళ (is_directory(మార్గం)) {
		నిర్మాణం strbuf sb_rm = STRBUF_INIT;
		స్థిరరాశి అక్షరం *format;

		ఒకవేళ (is_directory(sub_git_dir)) {
			ఒకవేళ (!(flags & OPT_QUIET))
				warning(_("Submodule work tree '%s' contains a .git "
					  "directory. This will be replaced with a "
					  ".git file by using absorbgitdirs."),
					displaypath);

			absorb_git_dir_into_superproject(మార్గం, ఖాళీ);

		}

		ఒకవేళ (!(flags & OPT_FORCE)) {
			నిర్మాణం బిడ్డ_process cp_rm = బిడ్డ_PROCESS_INIT;

			cp_rm.git_cmd = 1;
			strvec_pushl(&cp_rm.వాదనలు, "rm", "-qn",
				     మార్గం, ఖాళీ);

			ఒకవేళ (నిరంతరం_command(&cp_rm))
				die(_("Submodule work tree '%s' contains local "
				      "modifications; use '-f' to discard them"),
				      displaypath);
		}

		strbuf_addstr(&sb_rm, మార్గం);

		ఒకవేళ (!remove_dir_recursively(&sb_rm, 0))
			format = _("Cleared directory '%s'\n");
		లేకపోతే
			format = _("Could not remove submodule work tree '%s'\n");

		ఒకవేళ (!(flags & OPT_QUIET))
			రాయి(format, displaypath);

		submodule_unset_core_worktree(sub);

		strbuf_release(&sb_rm);
	}

	ఒకవేళ (mkdir(మార్గం, 0777))
		రాయి(_("could not create empty submodule directory %s"),
		      displaypath);

	cp_config.git_cmd = 1;
	strvec_pushl(&cp_config.వాదనలు, "config", "--get-regexp", ఖాళీ);
	strvec_pushf(&cp_config.వాదనలు, "submodule.%s\\.", sub->name);

	/* remove the .git/config entries (unless the user already did it) */
	ఒకవేళ (!capture_command(&cp_config, &sb_config, 0) && sb_config.len) {
		అక్షరం *sub_కీలకం = xstrfmt("submodule.%s", sub->name);

		/*
		 * remove the whole section so we have a clean state when
		 * the user later decides to init this submodule again
		 */
		repo_config_rename_section_లోపల_file(the_repository, ఖాళీ, sub_కీలకం, ఖాళీ);
		ఒకవేళ (!(flags & OPT_QUIET))
			రాయి(_("Submodule '%s' (%s) unregistered for path '%s'\n"),
				 sub->name, sub->url, displaypath);
		విడుదల(sub_కీలకం);
	}

cleanup:
	విడుదల(displaypath);
	విడుదల(sub_git_dir);
	strbuf_release(&sb_config);
}

స్థిర శూన్య deinit_submodule_cb(స్థిరరాశి నిర్మాణం cache_నమోదు *list_అంశం,
				శూన్య *cb_దత్తాంశం)
{
	నిర్మాణం deinit_cb *info = cb_దత్తాంశం;
	deinit_submodule(list_అంశం->name, info->ఉపసర్గ, info->flags);
}

స్థిర పూర్ణాంకం module_deinit(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
			 నిర్మాణం repository *repo వాడని)
{
	నిర్మాణం deinit_cb info = DEINIT_CB_INIT;
	నిర్మాణం pathspec pathspec = { 0 };
	నిర్మాణం module_list list = MODULE_LIST_INIT;
	పూర్ణాంకం quiet = 0;
	పూర్ణాంకం force = 0;
	పూర్ణాంకం all = 0;
	నిర్మాణం option module_deinit_options[] = {
		OPT__QUIET(&quiet, N_("suppress submodule status output")),
		OPT__FORCE(&force, N_("remove submodule working trees even if they contain local changes"), 0),
		OPT_BOOL(0, "all", &all, N_("unregister all submodules")),
		OPT_చివర()
	};
	స్థిరరాశి అక్షరం *స్థిరరాశి git_submodule_helper_usage[] = {
		N_("git submodule deinit [--quiet] [-f | --force] [--all | [--] [<path>...]]"),
		ఖాళీ
	};
	పూర్ణాంకం ret = 1;

	argc = parse_options(argc, argv, ఉపసర్గ, module_deinit_options,
			     git_submodule_helper_usage, 0);

	ఒకవేళ (all && argc) {
		error("pathspec and --all are incompatible");
		usage_with_options(git_submodule_helper_usage,
				   module_deinit_options);
	}

	ఒకవేళ (!argc && !all)
		die(_("Use '--all' if you really want to deinitialize all submodules"));

	ఒకవేళ (module_list_compute(argv, ఉపసర్గ, &pathspec, &list) < 0)
		వెళ్ళు cleanup;

	info.ఉపసర్గ = ఉపసర్గ;
	ఒకవేళ (quiet)
		info.flags |= OPT_QUIET;
	ఒకవేళ (force)
		info.flags |= OPT_FORCE;

	for_each_listed_submodule(&list, deinit_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	తిరిగి ret;
}

నిర్మాణం module_clone_దత్తాంశం {
	స్థిరరాశి అక్షరం *ఉపసర్గ;
	స్థిరరాశి అక్షరం *మార్గం;
	స్థిరరాశి అక్షరం *name;
	స్థిరరాశి అక్షరం *url;
	పూర్ణాంకం లోతు;
	నిర్మాణం list_objects_filter_options *filter_options;
	గణన ref_storage_format ref_storage_format;
	గుర్తులేని పూర్ణాంకం quiet: 1;
	గుర్తులేని పూర్ణాంకం పురోగతి: 1;
	గుర్తులేని పూర్ణాంకం dissociate: 1;
	గుర్తులేని పూర్ణాంకం require_init: 1;
	పూర్ణాంకం single_branch;
};
#నిర్వచించు MODULE_CLONE_దత్తాంశం_INIT { \
	.single_branch = -1, \
	.ref_storage_format = REF_STORAGE_FORMAT_UNKNOWN, \
}

నిర్మాణం submodule_alternate_అమరిక {
	స్థిరరాశి అక్షరం *submodule_name;
	గణన SUBMODULE_ALTERNATE_ERROR_బహుళకం {
		SUBMODULE_ALTERNATE_ERROR_DIE,
		SUBMODULE_ALTERNATE_ERROR_INFO,
		SUBMODULE_ALTERNATE_ERROR_IGNORE
	} error_బహుళకం;
	నిర్మాణం string_list *reference;
};
#నిర్వచించు SUBMODULE_ALTERNATE_అమరిక_INIT { \
	.error_బహుళకం = SUBMODULE_ALTERNATE_ERROR_IGNORE, \
}

స్థిర స్థిరరాశి అక్షరం alternate_error_advice[] = N_(
"An alternate computed from a superproject's alternate is invalid.\n"
"To allow Git to clone without an alternate in such a case, set\n"
"submodule.alternateErrorStrategy to 'info' or, equivalently, clone with\n"
"'--reference-if-able' instead of '--reference'."
);

స్థిర పూర్ణాంకం కూడు_possible_reference_from_superproject(
		నిర్మాణం odb_files_dir *alt_odb, శూన్య *sas_cb)
{
	నిర్మాణం submodule_alternate_అమరిక *sas = sas_cb;
	కొలత_t len;

	/*
	 * If the alternate object store is another repository, try the
	 * standard layout with .git/(modules/<name>)+/objects
	 */
	ఒకవేళ (strip_suffix(alt_odb->abspath, "/objects", &len)) {
		నిర్మాణం repository alternate;
		అక్షరం *sm_alternate;
		నిర్మాణం strbuf sb = STRBUF_INIT;
		నిర్మాణం strbuf err = STRBUF_INIT;
		strbuf_కూడు(&sb, alt_odb->abspath, len);

		ఒకవేళ (repo_init(&alternate, sb.buf, ఖాళీ) < 0)
			die(_("could not get a repository handle for gitdir '%s'"),
			    sb.buf);

		/*
		 * We need to end the new path with '/' to mark it as a dir,
		 * otherwise a submodule name containing '/' will be broken
		 * as the last part of a missing submodule reference would
		 * be taken as a file name.
		 */
		strbuf_reset(&sb);
		submodule_name_గమ్యం_gitdir(&sb, &alternate, sas->submodule_name);
		strbuf_addch(&sb, '/');
		repo_clear(&alternate);

		sm_alternate = compute_alternate_మార్గం(sb.buf, &err);
		ఒకవేళ (sm_alternate) {
			అక్షరం *p = strbuf_detach(&sb, ఖాళీ);

			string_list_append(sas->reference, p)->util = p;
			విడుదల(sm_alternate);
		} లేకపోతే {
			ఎంపిక (sas->error_బహుళకం) {
			స్థితి SUBMODULE_ALTERNATE_ERROR_DIE:
				ఒకవేళ (advice_enabled(ADVICE_SUBMODULE_ALTERNATE_ERROR_STRATEGY_DIE))
					advise(_(alternate_error_advice));
				die(_("submodule '%s' cannot add alternate: %s"),
				    sas->submodule_name, err.buf);
			స్థితి SUBMODULE_ALTERNATE_ERROR_INFO:
				fprintf_ln(stderr, _("submodule '%s' cannot add alternate: %s"),
					sas->submodule_name, err.buf);
			స్థితి SUBMODULE_ALTERNATE_ERROR_IGNORE:
				; /* nothing */
			}
		}

		strbuf_release(&err);
		strbuf_release(&sb);
	}

	తిరిగి 0;
}

స్థిర శూన్య prepare_possible_alternates(స్థిరరాశి అక్షరం *sm_name,
		నిర్మాణం string_list *reference)
{
	అక్షరం *sm_alternate = ఖాళీ, *error_strategy = ఖాళీ;
	నిర్మాణం submodule_alternate_అమరిక sas = SUBMODULE_ALTERNATE_అమరిక_INIT;

	repo_config_get_string(the_repository, "submodule.alternateLocation", &sm_alternate);
	ఒకవేళ (!sm_alternate)
		తిరిగి;

	repo_config_get_string(the_repository, "submodule.alternateErrorStrategy", &error_strategy);

	ఒకవేళ (!error_strategy)
		error_strategy = xstrdup("die");

	sas.submodule_name = sm_name;
	sas.reference = reference;
	ఒకవేళ (!strcmp(error_strategy, "die"))
		sas.error_బహుళకం = SUBMODULE_ALTERNATE_ERROR_DIE;
	లేకపోతే ఒకవేళ (!strcmp(error_strategy, "info"))
		sas.error_బహుళకం = SUBMODULE_ALTERNATE_ERROR_INFO;
	లేకపోతే ఒకవేళ (!strcmp(error_strategy, "ignore"))
		sas.error_బహుళకం = SUBMODULE_ALTERNATE_ERROR_IGNORE;
	లేకపోతే
		die(_("Value '%s' for submodule.alternateErrorStrategy is not recognized"), error_strategy);

	ఒకవేళ (!strcmp(sm_alternate, "superproject"))
		odb_for_each_alternate(the_repository->objects,
				       కూడు_possible_reference_from_superproject, &sas);
	లేకపోతే ఒకవేళ (!strcmp(sm_alternate, "no"))
		; /* do nothing */
	లేకపోతే
		die(_("Value '%s' for submodule.alternateLocation is not recognized"), sm_alternate);

	విడుదల(sm_alternate);
	విడుదల(error_strategy);
}

స్థిర అక్షరం *clone_submodule_sm_gitdir(స్థిరరాశి అక్షరం *name)
{
	నిర్మాణం strbuf sb = STRBUF_INIT;
	అక్షరం *sm_gitdir;

	submodule_name_గమ్యం_gitdir(&sb, the_repository, name);
	sm_gitdir = absolute_pathdup(sb.buf);
	strbuf_release(&sb);

	తిరిగి sm_gitdir;
}

స్థిర పూర్ణాంకం dir_కలిగియుంది_only_dotgit(స్థిరరాశి అక్షరం *మార్గం)
{
	DIR *dir = opendir(మార్గం);
	నిర్మాణం dirent *e;
	పూర్ణాంకం ret = 1;

	ఒకవేళ (!dir)
		తిరిగి 0;

	e = readdir_skip_dot_and_dotdot(dir);
	ఒకవేళ (!e)
		ret = 0;
	లేకపోతే ఒకవేళ (strcmp(DEFAULT_GIT_DIR_ENVIRONMENT, e->d_name) ||
		 (e = readdir_skip_dot_and_dotdot(dir))) {
		error("unexpected item '%s' in '%s'", e->d_name, మార్గం);
		ret = 0;
	}

	closedir(dir);
	తిరిగి ret;
}

స్థిర పూర్ణాంకం clone_submodule(స్థిరరాశి నిర్మాణం module_clone_దత్తాంశం *clone_దత్తాంశం,
			   నిర్మాణం string_list *reference)
{
	అక్షరం *p;
	అక్షరం *sm_gitdir = clone_submodule_sm_gitdir(clone_దత్తాంశం->name);
	అక్షరం *sm_alternate = ఖాళీ, *error_strategy = ఖాళీ;
	నిర్మాణం stat st;
	నిర్మాణం బిడ్డ_process cp = బిడ్డ_PROCESS_INIT;
	స్థిరరాశి అక్షరం *clone_దత్తాంశం_మార్గం = clone_దత్తాంశం->మార్గం;
	అక్షరం *గమ్యం_free = ఖాళీ;

	ఒకవేళ (validate_submodule_మార్గం(clone_దత్తాంశం_మార్గం) < 0)
		die(ఖాళీ);

	ఒకవేళ (!is_absolute_మార్గం(clone_దత్తాంశం->మార్గం))
		clone_దత్తాంశం_మార్గం = గమ్యం_free = xstrfmt("%s/%s", repo_get_work_వృక్షం(the_repository),
						    clone_దత్తాంశం->మార్గం);

	ఒకవేళ (!file_exists(sm_gitdir)) {
		ఒకవేళ (clone_దత్తాంశం->require_init && !stat(clone_దత్తాంశం_మార్గం, &st) &&
		    !is_ఖాళీలు_dir(clone_దత్తాంశం_మార్గం))
			die(_("directory not empty: '%s'"), clone_దత్తాంశం_మార్గం);

		ఒకవేళ (సురక్షిత_create_leading_directories_const(the_repository, sm_gitdir) < 0)
			die(_("could not create directory '%s'"), sm_gitdir);

		prepare_possible_alternates(clone_దత్తాంశం->name, reference);

		strvec_నెట్టు(&cp.వాదనలు, "clone");
		strvec_నెట్టు(&cp.వాదనలు, "--no-checkout");
		ఒకవేళ (clone_దత్తాంశం->quiet)
			strvec_నెట్టు(&cp.వాదనలు, "--quiet");
		ఒకవేళ (clone_దత్తాంశం->పురోగతి)
			strvec_నెట్టు(&cp.వాదనలు, "--progress");
		ఒకవేళ (clone_దత్తాంశం->లోతు > 0)
			strvec_pushf(&cp.వాదనలు, "--depth=%d", clone_దత్తాంశం->లోతు);
		ఒకవేళ (reference->nr) {
			నిర్మాణం string_list_అంశం *అంశం;

			for_each_string_list_అంశం(అంశం, reference)
				strvec_pushl(&cp.వాదనలు, "--reference",
					     అంశం->string, ఖాళీ);
		}
		ఒకవేళ (clone_దత్తాంశం->ref_storage_format != REF_STORAGE_FORMAT_UNKNOWN)
			strvec_pushf(&cp.వాదనలు, "--ref-storage-format=%s",
				     ref_storage_format_గమ్యం_name(clone_దత్తాంశం->ref_storage_format));
		ఒకవేళ (clone_దత్తాంశం->dissociate)
			strvec_నెట్టు(&cp.వాదనలు, "--dissociate");
		ఒకవేళ (sm_gitdir && *sm_gitdir)
			strvec_pushl(&cp.వాదనలు, "--separate-git-dir", sm_gitdir, ఖాళీ);
		ఒకవేళ (clone_దత్తాంశం->filter_options && clone_దత్తాంశం->filter_options->choice)
			strvec_pushf(&cp.వాదనలు, "--filter=%s",
				     expand_list_objects_filter_spec(
					     clone_దత్తాంశం->filter_options));
		ఒకవేళ (clone_దత్తాంశం->single_branch >= 0)
			strvec_నెట్టు(&cp.వాదనలు, clone_దత్తాంశం->single_branch ?
				    "--single-branch" :
				    "--no-single-branch");

		strvec_నెట్టు(&cp.వాదనలు, "--");
		strvec_నెట్టు(&cp.వాదనలు, clone_దత్తాంశం->url);
		strvec_నెట్టు(&cp.వాదనలు, clone_దత్తాంశం_మార్గం);

		cp.git_cmd = 1;
		prepare_submodule_repo_env(&cp.env);
		cp.no_stdin = 1;

		ఒకవేళ(నిరంతరం_command(&cp))
			die(_("clone of '%s' into submodule path '%s' failed"),
			    clone_దత్తాంశం->url, clone_దత్తాంశం_మార్గం);

		ఒకవేళ (clone_దత్తాంశం->require_init && !stat(clone_దత్తాంశం_మార్గం, &st) &&
		    !dir_కలిగియుంది_only_dotgit(clone_దత్తాంశం_మార్గం)) {
			అక్షరం *dot_git = xstrfmt("%s/.git", clone_దత్తాంశం_మార్గం);
			unlink(dot_git);
			విడుదల(dot_git);
			die(_("directory not empty: '%s'"), clone_దత్తాంశం_మార్గం);
		}
	} లేకపోతే {
		అక్షరం *మార్గం;

		ఒకవేళ (clone_దత్తాంశం->require_init && !stat(clone_దత్తాంశం_మార్గం, &st) &&
		    !is_ఖాళీలు_dir(clone_దత్తాంశం_మార్గం))
			die(_("directory not empty: '%s'"), clone_దత్తాంశం_మార్గం);
		ఒకవేళ (సురక్షిత_create_leading_directories_const(the_repository, clone_దత్తాంశం_మార్గం) < 0)
			die(_("could not create directory '%s'"), clone_దత్తాంశం_మార్గం);
		మార్గం = xstrfmt("%s/index", sm_gitdir);
		unlink_or_warn(మార్గం);
		విడుదల(మార్గం);
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
	ఒకవేళ (validate_submodule_git_dir(sm_gitdir, clone_దత్తాంశం->name) < 0) {
		అక్షరం *తల = xstrfmt("%s/HEAD", sm_gitdir);
		unlink(తల);
		విడుదల(తల);
		die(_("refusing to create/use '%s' in another submodule's git dir. "
		      "Enabling extensions.submodulePathConfig should fix this."),
		    sm_gitdir);
	}

	connect_work_వృక్షం_and_git_dir(clone_దత్తాంశం_మార్గం, sm_gitdir, 0);

	p = repo_submodule_మార్గం(the_repository, clone_దత్తాంశం_మార్గం, "config");
	ఒకవేళ (!p)
		die(_("could not get submodule directory for '%s'"), clone_దత్తాంశం_మార్గం);

	/* setup alternateLocation and alternateErrorStrategy in the cloned submodule if needed */
	repo_config_get_string(the_repository, "submodule.alternateLocation", &sm_alternate);
	ఒకవేళ (sm_alternate)
		repo_config_సమితి_లోపల_file(the_repository, p, "submodule.alternateLocation",
					sm_alternate);
	repo_config_get_string(the_repository, "submodule.alternateErrorStrategy", &error_strategy);
	ఒకవేళ (error_strategy)
		repo_config_సమితి_లోపల_file(the_repository, p, "submodule.alternateErrorStrategy",
					error_strategy);

	విడుదల(sm_alternate);
	విడుదల(error_strategy);

	విడుదల(sm_gitdir);
	విడుదల(p);
	విడుదల(గమ్యం_free);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం module_clone(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
			నిర్మాణం repository *repo వాడని)
{
	పూర్ణాంకం dissociate = 0, quiet = 0, పురోగతి = 0, require_init = 0;
	నిర్మాణం module_clone_దత్తాంశం clone_దత్తాంశం = MODULE_CLONE_దత్తాంశం_INIT;
	నిర్మాణం string_list reference = STRING_LIST_INIT_NODUP;
	నిర్మాణం list_objects_filter_options filter_options =
		LIST_OBJECTS_FILTER_INIT;
	స్థిరరాశి అక్షరం *ref_storage_format = ఖాళీ;

	నిర్మాణం option module_clone_options[] = {
		OPT_STRING(0, "prefix", &clone_దత్తాంశం.ఉపసర్గ,
			   N_("path"),
			   N_("alternative anchor for relative paths")),
		OPT_STRING(0, "path", &clone_దత్తాంశం.మార్గం,
			   N_("path"),
			   N_("where the new submodule will be cloned to")),
		OPT_STRING(0, "name", &clone_దత్తాంశం.name,
			   N_("string"),
			   N_("name of the new submodule")),
		OPT_STRING(0, "url", &clone_దత్తాంశం.url,
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
		OPT_INTEGER(0, "depth", &clone_దత్తాంశం.లోతు,
			   N_("depth for shallow clones")),
		OPT__QUIET(&quiet, "suppress output for cloning a submodule"),
		OPT_BOOL(0, "progress", &పురోగతి,
			   N_("force cloning progress")),
		OPT_BOOL(0, "require-init", &require_init,
			   N_("disallow cloning into non-empty directory")),
		OPT_BOOL(0, "single-branch", &clone_దత్తాంశం.single_branch,
			 N_("clone only one branch, HEAD or --branch")),
		OPT_PARSE_LIST_OBJECTS_FILTER(&filter_options),
		OPT_చివర()
	};
	స్థిరరాశి అక్షరం *స్థిరరాశి git_submodule_helper_usage[] = {
		N_("git submodule--helper clone [--prefix=<path>] [--quiet] "
		   "[--reference <repository>] [--name <name>] [--depth <depth>] "
		   "[--single-branch] [--filter <filter-spec>] "
		   "--url <url> --path <path>"),
		ఖాళీ
	};

	argc = parse_options(argc, argv, ఉపసర్గ, module_clone_options,
			     git_submodule_helper_usage, 0);

	ఒకవేళ (ref_storage_format) {
		clone_దత్తాంశం.ref_storage_format = ref_storage_format_by_name(ref_storage_format);
		ఒకవేళ (clone_దత్తాంశం.ref_storage_format == REF_STORAGE_FORMAT_UNKNOWN)
			die(_("unknown ref storage format '%s'"), ref_storage_format);
	}
	clone_దత్తాంశం.dissociate = !!dissociate;
	clone_దత్తాంశం.quiet = !!quiet;
	clone_దత్తాంశం.పురోగతి = !!పురోగతి;
	clone_దత్తాంశం.require_init = !!require_init;
	clone_దత్తాంశం.filter_options = &filter_options;

	ఒకవేళ (argc || !clone_దత్తాంశం.url || !clone_దత్తాంశం.మార్గం || !*(clone_దత్తాంశం.మార్గం))
		usage_with_options(git_submodule_helper_usage,
				   module_clone_options);

	clone_submodule(&clone_దత్తాంశం, &reference);
	list_objects_filter_release(&filter_options);
	string_list_clear(&reference, 1);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం determine_submodule_update_strategy(నిర్మాణం repository *r,
					       పూర్ణాంకం just_cloned,
					       స్థిరరాశి అక్షరం *మార్గం,
					       గణన submodule_update_type update,
					       నిర్మాణం submodule_update_strategy *బహిర్గతం)
{
	స్థిరరాశి నిర్మాణం submodule *sub = submodule_from_మార్గం(r, null_oid(the_hash_algo), మార్గం);
	అక్షరం *కీలకం;
	స్థిరరాశి అక్షరం *val;
	పూర్ణాంకం ret;

	/*
	 * NEEDSWORK: audit and ensure that update_submodule() has right
	 * to assume that submodule_from_path() above will always succeed.
	 */
	ఒకవేళ (!sub)
		BUG("update_submodule assumes a submodule exists at path (%s)",
		    మార్గం);
	కీలకం = xstrfmt("submodule.%s.update", sub->name);

	ఒకవేళ (update) {
		బహిర్గతం->type = update;
	} లేకపోతే ఒకవేళ (!repo_config_get_string_tmp(r, కీలకం, &val)) {
		ఒకవేళ (parse_submodule_update_strategy(val, బహిర్గతం) < 0) {
			ret = die_సందేశం(_("Invalid update mode '%s' configured for submodule path '%s'"),
					  val, మార్గం);
			వెళ్ళు cleanup;
		}
	} లేకపోతే ఒకవేళ (sub->update_strategy.type != SM_UPDATE_UNSPECIFIED) {
		ఒకవేళ (sub->update_strategy.type == SM_UPDATE_COMMAND)
			BUG("how did we read update = !command from .gitmodules?");
		బహిర్గతం->type = sub->update_strategy.type;
		బహిర్గతం->command = sub->update_strategy.command;
	} లేకపోతే
		బహిర్గతం->type = SM_UPDATE_CHECKOUT;

	ఒకవేళ (just_cloned &&
	    (బహిర్గతం->type == SM_UPDATE_విలీనం ||
	     బహిర్గతం->type == SM_UPDATE_REBASE ||
	     బహిర్గతం->type == SM_UPDATE_NONE))
		బహిర్గతం->type = SM_UPDATE_CHECKOUT;

	ret = 0;
cleanup:
	విడుదల(కీలకం);
	తిరిగి ret;
}

నిర్మాణం update_clone_దత్తాంశం {
	స్థిరరాశి నిర్మాణం submodule *sub;
	నిర్మాణం object_id oid;
	గుర్తులేని just_cloned;
};

నిర్మాణం submodule_update_clone {
	/* index into 'update_data.list', the list of submodules to look into for cloning */
	పూర్ణాంకం current;

	/* configuration parameters which are passed on to the children */
	స్థిరరాశి నిర్మాణం update_దత్తాంశం *update_దత్తాంశం;

	/* to be consumed by update_submodule() */
	నిర్మాణం update_clone_దత్తాంశం *update_clone;
	పూర్ణాంకం update_clone_nr; పూర్ణాంకం update_clone_alloc;

	/* If we want to stop as fast as possible and return an error */
	గుర్తులేని quickstop : 1;

	/* failed clones to be retried again */
	స్థిరరాశి నిర్మాణం cache_నమోదు **failed_clones;
	పూర్ణాంకం failed_clones_nr, failed_clones_alloc;
};
#నిర్వచించు SUBMODULE_UPDATE_CLONE_INIT { 0 }

స్థిర శూన్య submodule_update_clone_release(నిర్మాణం submodule_update_clone *suc)
{
	విడుదల(suc->update_clone);
	విడుదల(suc->failed_clones);
}

నిర్మాణం update_దత్తాంశం {
	స్థిరరాశి అక్షరం *ఉపసర్గ;
	స్థిరరాశి అక్షరం *super_ఉపసర్గ;
	అక్షరం *displaypath;
	గణన submodule_update_type update_default;
	నిర్మాణం object_id suboid;
	నిర్మాణం string_list references;
	నిర్మాణం submodule_update_strategy update_strategy;
	నిర్మాణం list_objects_filter_options *filter_options;
	నిర్మాణం module_list list;
	గణన ref_storage_format ref_storage_format;
	పూర్ణాంకం లోతు;
	పూర్ణాంకం max_jobs;
	పూర్ణాంకం single_branch;
	పూర్ణాంకం recommend_shallow;
	గుర్తులేని పూర్ణాంకం require_init;
	గుర్తులేని పూర్ణాంకం force;
	గుర్తులేని పూర్ణాంకం quiet;
	గుర్తులేని పూర్ణాంకం nofetch;
	గుర్తులేని పూర్ణాంకం remote;
	గుర్తులేని పూర్ణాంకం పురోగతి;
	గుర్తులేని పూర్ణాంకం dissociate;
	గుర్తులేని పూర్ణాంకం init;
	గుర్తులేని పూర్ణాంకం warn_if_uninitialized;
	గుర్తులేని పూర్ణాంకం recursive;

	/* copied over from update_clone_data */
	నిర్మాణం object_id oid;
	గుర్తులేని పూర్ణాంకం just_cloned;
	స్థిరరాశి అక్షరం *sm_మార్గం;
};
#నిర్వచించు UPDATE_దత్తాంశం_INIT { \
	.update_strategy = SUBMODULE_UPDATE_STRATEGY_INIT, \
	.list = MODULE_LIST_INIT, \
	.ref_storage_format = REF_STORAGE_FORMAT_UNKNOWN, \
	.recommend_shallow = -1, \
	.references = STRING_LIST_INIT_DUP, \
	.single_branch = -1, \
	.max_jobs = 1, \
}

స్థిర శూన్య update_దత్తాంశం_release(నిర్మాణం update_దత్తాంశం *ud)
{
	విడుదల(ud->displaypath);
	submodule_update_strategy_release(&ud->update_strategy);
	module_list_release(&ud->list);
}

స్థిర శూన్య తదుపరి_submodule_warn_missing(నిర్మాణం submodule_update_clone *suc,
		నిర్మాణం strbuf *బహిర్గతం, స్థిరరాశి అక్షరం *displaypath)
{
	/*
	 * Only mention uninitialized submodules when their
	 * paths have been specified.
	 */
	ఒకవేళ (suc->update_దత్తాంశం->warn_if_uninitialized) {
		strbuf_addf(బహిర్గతం,
			_("Submodule path '%s' not initialized"),
			displaypath);
		strbuf_addch(బహిర్గతం, '\n');
		strbuf_addstr(బహిర్గతం,
			_("Maybe you want to use 'update --init'?"));
		strbuf_addch(బహిర్గతం, '\n');
	}
}

/**
 * Determine whether 'ce' needs to be cloned. If so, prepare the 'child' to
 * run the clone. Returns 1 if 'ce' needs to be cloned, 0 otherwise.
 */
స్థిర పూర్ణాంకం prepare_గమ్యం_clone_తదుపరి_submodule(స్థిరరాశి నిర్మాణం cache_నమోదు *ce,
					   నిర్మాణం బిడ్డ_process *బిడ్డ,
					   నిర్మాణం submodule_update_clone *suc,
					   నిర్మాణం strbuf *బహిర్గతం)
{
	స్థిరరాశి నిర్మాణం submodule *sub = ఖాళీ;
	స్థిరరాశి అక్షరం *url = ఖాళీ;
	స్థిరరాశి అక్షరం *update_string;
	గణన submodule_update_type update_type;
	అక్షరం *కీలకం;
	స్థిరరాశి నిర్మాణం update_దత్తాంశం *ud = suc->update_దత్తాంశం;
	అక్షరం *displaypath = get_submodule_displaypath(ce->name, ud->ఉపసర్గ,
						      ud->super_ఉపసర్గ);
	నిర్మాణం strbuf sb = STRBUF_INIT;
	పూర్ణాంకం needs_cloning = 0;
	పూర్ణాంకం need_free_url = 0;

	ఒకవేళ (ce_stage(ce)) {
		strbuf_addf(బహిర్గతం, _("Skipping unmerged submodule %s"), displaypath);
		strbuf_addch(బహిర్గతం, '\n');
		వెళ్ళు cleanup;
	}

	sub = submodule_from_మార్గం(the_repository, null_oid(the_hash_algo), ce->name);

	ఒకవేళ (!sub) {
		తదుపరి_submodule_warn_missing(suc, బహిర్గతం, displaypath);
		వెళ్ళు cleanup;
	}

	కీలకం = xstrfmt("submodule.%s.update", sub->name);
	ఒకవేళ (!repo_config_get_string_tmp(the_repository, కీలకం, &update_string)) {
		update_type = parse_submodule_update_type(update_string);
	} లేకపోతే {
		update_type = sub->update_strategy.type;
	}
	విడుదల(కీలకం);

	ఒకవేళ (suc->update_దత్తాంశం->update_strategy.type == SM_UPDATE_NONE
	    || (suc->update_దత్తాంశం->update_strategy.type == SM_UPDATE_UNSPECIFIED
		&& update_type == SM_UPDATE_NONE)) {
		strbuf_addf(బహిర్గతం, _("Skipping submodule '%s'"), displaypath);
		strbuf_addch(బహిర్గతం, '\n');
		వెళ్ళు cleanup;
	}

	/* Check if the submodule has been initialized. */
	ఒకవేళ (!is_submodule_చురుకైన(the_repository, ce->name)) {
		తదుపరి_submodule_warn_missing(suc, బహిర్గతం, displaypath);
		వెళ్ళు cleanup;
	}

	strbuf_reset(&sb);
	strbuf_addf(&sb, "submodule.%s.url", sub->name);
	ఒకవేళ (repo_config_get_string_tmp(the_repository, sb.buf, &url)) {
		ఒకవేళ (sub->url && (starts_with_dot_slash(sub->url) ||
				 starts_with_dot_dot_slash(sub->url))) {
			url = resolve_relative_url(sub->url, ఖాళీ, 0);
			need_free_url = 1;
		} లేకపోతే
			url = sub->url;
	}

	ఒకవేళ (!url)
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

	ఒకవేళ (!needs_cloning)
		వెళ్ళు cleanup;

	బిడ్డ->git_cmd = 1;
	బిడ్డ->no_stdin = 1;
	బిడ్డ->stdout_గమ్యం_stderr = 1;
	బిడ్డ->err = -1;
	strvec_నెట్టు(&బిడ్డ->వాదనలు, "submodule--helper");
	strvec_నెట్టు(&బిడ్డ->వాదనలు, "clone");
	ఒకవేళ (suc->update_దత్తాంశం->పురోగతి)
		strvec_నెట్టు(&బిడ్డ->వాదనలు, "--progress");
	ఒకవేళ (suc->update_దత్తాంశం->quiet)
		strvec_నెట్టు(&బిడ్డ->వాదనలు, "--quiet");
	ఒకవేళ (suc->update_దత్తాంశం->ఉపసర్గ)
		strvec_pushl(&బిడ్డ->వాదనలు, "--prefix", suc->update_దత్తాంశం->ఉపసర్గ, ఖాళీ);
	ఒకవేళ (suc->update_దత్తాంశం->recommend_shallow && sub->recommend_shallow == 1)
		strvec_నెట్టు(&బిడ్డ->వాదనలు, "--depth=1");
	లేకపోతే ఒకవేళ (suc->update_దత్తాంశం->లోతు)
		strvec_pushf(&బిడ్డ->వాదనలు, "--depth=%d", suc->update_దత్తాంశం->లోతు);
	ఒకవేళ (suc->update_దత్తాంశం->filter_options && suc->update_దత్తాంశం->filter_options->choice)
		strvec_pushf(&బిడ్డ->వాదనలు, "--filter=%s",
			     expand_list_objects_filter_spec(suc->update_దత్తాంశం->filter_options));
	ఒకవేళ (suc->update_దత్తాంశం->require_init)
		strvec_నెట్టు(&బిడ్డ->వాదనలు, "--require-init");
	ఒకవేళ (suc->update_దత్తాంశం->ref_storage_format != REF_STORAGE_FORMAT_UNKNOWN)
		strvec_pushf(&బిడ్డ->వాదనలు, "--ref-storage-format=%s",
			     ref_storage_format_గమ్యం_name(suc->update_దత్తాంశం->ref_storage_format));
	strvec_pushl(&బిడ్డ->వాదనలు, "--path", sub->మార్గం, ఖాళీ);
	strvec_pushl(&బిడ్డ->వాదనలు, "--name", sub->name, ఖాళీ);
	strvec_pushl(&బిడ్డ->వాదనలు, "--url", url, ఖాళీ);
	ఒకవేళ (suc->update_దత్తాంశం->references.nr) {
		నిర్మాణం string_list_అంశం *అంశం;

		for_each_string_list_అంశం(అంశం, &suc->update_దత్తాంశం->references)
			strvec_pushl(&బిడ్డ->వాదనలు, "--reference", అంశం->string, ఖాళీ);
	}
	ఒకవేళ (suc->update_దత్తాంశం->dissociate)
		strvec_నెట్టు(&బిడ్డ->వాదనలు, "--dissociate");
	ఒకవేళ (suc->update_దత్తాంశం->single_branch >= 0)
		strvec_నెట్టు(&బిడ్డ->వాదనలు, suc->update_దత్తాంశం->single_branch ?
					      "--single-branch" :
					      "--no-single-branch");

cleanup:
	విడుదల(displaypath);
	strbuf_release(&sb);
	ఒకవేళ (need_free_url)
		విడుదల((శూన్య*)url);

	తిరిగి needs_cloning;
}

స్థిర పూర్ణాంకం update_clone_get_తదుపరి_task(నిర్మాణం బిడ్డ_process *బిడ్డ,
				      నిర్మాణం strbuf *err,
				      శూన్య *suc_cb,
				      శూన్య **idx_task_cb)
{
	నిర్మాణం submodule_update_clone *suc = suc_cb;
	స్థిరరాశి నిర్మాణం cache_నమోదు *ce;
	పూర్ణాంకం సూచీ;

	కొరకు (; suc->current < suc->update_దత్తాంశం->list.nr; suc->current++) {
		ce = suc->update_దత్తాంశం->list.entries[suc->current];
		ఒకవేళ (prepare_గమ్యం_clone_తదుపరి_submodule(ce, బిడ్డ, suc, err)) {
			పూర్ణాంకం *p = xmalloc(పరిమాణం(*p));

			*p = suc->current;
			*idx_task_cb = p;
			suc->current++;
			తిరిగి 1;
		}
	}

	/*
	 * The loop above tried cloning each submodule once, now try the
	 * stragglers again, which we can imagine as an extension of the
	 * entry list.
	 */
	సూచీ = suc->current - suc->update_దత్తాంశం->list.nr;
	ఒకవేళ (సూచీ < suc->failed_clones_nr) {
		పూర్ణాంకం *p;

		ce = suc->failed_clones[సూచీ];
		ఒకవేళ (!prepare_గమ్యం_clone_తదుపరి_submodule(ce, బిడ్డ, suc, err)) {
			suc->current ++;
			strbuf_addstr(err, "BUG: submodule considered for "
					   "cloning, doesn't need cloning "
					   "any more?\n");
			తిరిగి 0;
		}
		p = xmalloc(పరిమాణం(*p));
		*p = suc->current;
		*idx_task_cb = p;
		suc->current ++;
		తిరిగి 1;
	}

	తిరిగి 0;
}

స్థిర పూర్ణాంకం update_clone_ప్రారంభం_failure(నిర్మాణం strbuf *err వాడని,
				      శూన్య *suc_cb,
				      శూన్య *idx_task_cb వాడని)
{
	నిర్మాణం submodule_update_clone *suc = suc_cb;

	suc->quickstop = 1;
	తిరిగి 1;
}

స్థిర పూర్ణాంకం update_clone_task_ముగిసిన(పూర్ణాంకం ఫలితం,
				      నిర్మాణం strbuf *err,
				      శూన్య *suc_cb,
				      శూన్య *idx_task_cb)
{
	స్థిరరాశి నిర్మాణం cache_నమోదు *ce;
	నిర్మాణం submodule_update_clone *suc = suc_cb;
	పూర్ణాంకం *idxP = idx_task_cb;
	పూర్ణాంకం idx = *idxP;

	విడుదల(idxP);

	ఒకవేళ (!ఫలితం)
		తిరిగి 0;

	ఒకవేళ (idx < suc->update_దత్తాంశం->list.nr) {
		ce  = suc->update_దత్తాంశం->list.entries[idx];
		strbuf_addf(err, _("Failed to clone '%s'. Retry scheduled"),
			    ce->name);
		strbuf_addch(err, '\n');
		ALLOC_GROW(suc->failed_clones,
			   suc->failed_clones_nr + 1,
			   suc->failed_clones_alloc);
		suc->failed_clones[suc->failed_clones_nr++] = ce;
		తిరిగి 0;
	} లేకపోతే {
		idx -= suc->update_దత్తాంశం->list.nr;
		ce  = suc->failed_clones[idx];
		strbuf_addf(err, _("Failed to clone '%s' a second time, aborting"),
			    ce->name);
		strbuf_addch(err, '\n');
		suc->quickstop = 1;
		తిరిగి 1;
	}

	తిరిగి 0;
}

స్థిర పూర్ణాంకం git_update_clone_config(స్థిరరాశి అక్షరం *var, స్థిరరాశి అక్షరం *విలువ,
				   స్థిరరాశి నిర్మాణం config_context *ctx,
				   శూన్య *cb)
{
	పూర్ణాంకం *max_jobs = cb;

	ఒకవేళ (!strcmp(var, "submodule.fetchjobs"))
		*max_jobs = parse_submodule_fetchjobs(var, విలువ, ctx->kvi);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం is_tip_reachable(స్థిరరాశి అక్షరం *మార్గం, స్థిరరాశి నిర్మాణం object_id *oid)
{
	నిర్మాణం బిడ్డ_process cp = బిడ్డ_PROCESS_INIT;
	నిర్మాణం strbuf rev = STRBUF_INIT;
	అక్షరం *hex = oid_గమ్యం_hex(oid);
	పూర్ణాంకం reachable;

	cp.git_cmd = 1;
	cp.dir = మార్గం;
	cp.no_stderr = 1;
	strvec_pushl(&cp.వాదనలు, "rev-list", "-n", "1", hex, "--not", "--all", ఖాళీ);

	prepare_submodule_repo_env(&cp.env);

	ఒకవేళ (capture_command(&cp, &rev, GIT_MAX_HEXSZ + 1) || rev.len)
		reachable = 0;
	లేకపోతే
		reachable = 1;

	strbuf_release(&rev);
	తిరిగి reachable;
}

స్థిర పూర్ణాంకం fetch_లోపల_submodule(స్థిరరాశి అక్షరం *module_మార్గం, పూర్ణాంకం లోతు, పూర్ణాంకం quiet,
			      స్థిరరాశి నిర్మాణం object_id *oid)
{
	నిర్మాణం బిడ్డ_process cp = బిడ్డ_PROCESS_INIT;

	prepare_submodule_repo_env(&cp.env);
	cp.git_cmd = 1;
	cp.dir = module_మార్గం;

	strvec_నెట్టు(&cp.వాదనలు, "fetch");
	ఒకవేళ (quiet)
		strvec_నెట్టు(&cp.వాదనలు, "--quiet");
	ఒకవేళ (లోతు)
		strvec_pushf(&cp.వాదనలు, "--depth=%d", లోతు);
	ఒకవేళ (oid) {
		అక్షరం *hex = oid_గమ్యం_hex(oid);
		అక్షరం *remote;
		పూర్ణాంకం సంకేతం_2;

		సంకేతం_2 = get_default_remote_submodule(module_మార్గం, &remote);
		ఒకవేళ (సంకేతం_2) {
			బిడ్డ_process_clear(&cp);
			తిరిగి సంకేతం_2;
		}

		strvec_pushl(&cp.వాదనలు, remote, hex, ఖాళీ);
		విడుదల(remote);
	}

	తిరిగి నిరంతరం_command(&cp);
}

స్థిర పూర్ణాంకం నిరంతరం_update_command(స్థిరరాశి నిర్మాణం update_దత్తాంశం *ud, పూర్ణాంకం subforce)
{
	నిర్మాణం బిడ్డ_process cp = బిడ్డ_PROCESS_INIT;
	అక్షరం *oid = oid_గమ్యం_hex(&ud->oid);
	పూర్ణాంకం ret;

	ఎంపిక (ud->update_strategy.type) {
	స్థితి SM_UPDATE_CHECKOUT:
		cp.git_cmd = 1;
		strvec_pushl(&cp.వాదనలు, "checkout", "-q", ఖాళీ);
		ఒకవేళ (subforce)
			strvec_నెట్టు(&cp.వాదనలు, "-f");
		ఆపు;
	స్థితి SM_UPDATE_REBASE:
		cp.git_cmd = 1;
		strvec_నెట్టు(&cp.వాదనలు, "rebase");
		ఒకవేళ (ud->quiet)
			strvec_నెట్టు(&cp.వాదనలు, "--quiet");
		ఆపు;
	స్థితి SM_UPDATE_విలీనం:
		cp.git_cmd = 1;
		strvec_నెట్టు(&cp.వాదనలు, "merge");
		ఒకవేళ (ud->quiet)
			strvec_నెట్టు(&cp.వాదనలు, "--quiet");
		ఆపు;
	స్థితి SM_UPDATE_COMMAND:
		cp.use_shell = 1;
		strvec_నెట్టు(&cp.వాదనలు, ud->update_strategy.command);
		ఆపు;
	అప్రమేయ:
		BUG("unexpected update strategy type: %d",
		    ud->update_strategy.type);
	}
	strvec_నెట్టు(&cp.వాదనలు, oid);

	cp.dir = ud->sm_మార్గం;
	prepare_submodule_repo_env(&cp.env);
	ఒకవేళ ((ret = నిరంతరం_command(&cp))) {
		ఎంపిక (ud->update_strategy.type) {
		స్థితి SM_UPDATE_CHECKOUT:
			die_సందేశం(_("Unable to checkout '%s' in submodule path '%s'"),
				    oid, ud->displaypath);
			/* No "ret" assignment, use "git checkout"'s */
			ఆపు;
		స్థితి SM_UPDATE_REBASE:
			ret = die_సందేశం(_("Unable to rebase '%s' in submodule path '%s'"),
					  oid, ud->displaypath);
			ఆపు;
		స్థితి SM_UPDATE_విలీనం:
			ret = die_సందేశం(_("Unable to merge '%s' in submodule path '%s'"),
					  oid, ud->displaypath);
			ఆపు;
		స్థితి SM_UPDATE_COMMAND:
			ret = die_సందేశం(_("Execution of '%s %s' failed in submodule path '%s'"),
					  ud->update_strategy.command, oid, ud->displaypath);
			ఆపు;
		అప్రమేయ:
			BUG("unexpected update strategy type: %d",
			    ud->update_strategy.type);
		}

		తిరిగి ret;
	}

	ఒకవేళ (ud->quiet)
		తిరిగి 0;

	ఎంపిక (ud->update_strategy.type) {
	స్థితి SM_UPDATE_CHECKOUT:
		రాయి(_("Submodule path '%s': checked out '%s'\n"),
		       ud->displaypath, oid);
		ఆపు;
	స్థితి SM_UPDATE_REBASE:
		రాయి(_("Submodule path '%s': rebased into '%s'\n"),
		       ud->displaypath, oid);
		ఆపు;
	స్థితి SM_UPDATE_విలీనం:
		రాయి(_("Submodule path '%s': merged in '%s'\n"),
		       ud->displaypath, oid);
		ఆపు;
	స్థితి SM_UPDATE_COMMAND:
		రాయి(_("Submodule path '%s': '%s %s'\n"),
		       ud->displaypath, ud->update_strategy.command, oid);
		ఆపు;
	అప్రమేయ:
		BUG("unexpected update strategy type: %d",
		    ud->update_strategy.type);
	}

	తిరిగి 0;
}

స్థిర పూర్ణాంకం నిరంతరం_update_procedure(స్థిరరాశి నిర్మాణం update_దత్తాంశం *ud)
{
	పూర్ణాంకం subforce = is_null_oid(&ud->suboid) || ud->force;

	ఒకవేళ (!ud->nofetch) {
		/*
		 * Run fetch only if `oid` isn't present or it
		 * is not reachable from a ref.
		 */
		ఒకవేళ (!is_tip_reachable(ud->sm_మార్గం, &ud->oid) &&
		    fetch_లోపల_submodule(ud->sm_మార్గం, ud->లోతు, ud->quiet, ఖాళీ) &&
		    !ud->quiet)
			fprintf_ln(stderr,
				   _("Unable to fetch in submodule path '%s'; "
				     "trying to directly fetch %s:"),
				   ud->displaypath, oid_గమ్యం_hex(&ud->oid));
		/*
		 * Now we tried the usual fetch, but `oid` may
		 * not be reachable from any of the refs.
		 */
		ఒకవేళ (!is_tip_reachable(ud->sm_మార్గం, &ud->oid) &&
		    fetch_లోపల_submodule(ud->sm_మార్గం, ud->లోతు, ud->quiet, &ud->oid))
			తిరిగి die_సందేశం(_("Fetched in submodule path '%s', but it did not "
					     "contain %s. Direct fetching of that commit failed."),
					   ud->displaypath, oid_గమ్యం_hex(&ud->oid));
	}

	తిరిగి నిరంతరం_update_command(ud, subforce);
}

స్థిర పూర్ణాంకం remote_submodule_branch(స్థిరరాశి అక్షరం *మార్గం, స్థిరరాశి అక్షరం **branch)
{
	స్థిరరాశి నిర్మాణం submodule *sub;
	అక్షరం *కీలకం;
	*branch = ఖాళీ;

	sub = submodule_from_మార్గం(the_repository, null_oid(the_hash_algo), మార్గం);
	ఒకవేళ (!sub)
		తిరిగి die_సందేశం(_("could not initialize submodule at path '%s'"),
				   మార్గం);

	కీలకం = xstrfmt("submodule.%s.branch", sub->name);
	ఒకవేళ (repo_config_get_string_tmp(the_repository, కీలకం, branch))
		*branch = sub->branch;
	విడుదల(కీలకం);

	ఒకవేళ (!*branch) {
		*branch = "HEAD";
		తిరిగి 0;
	}

	ఒకవేళ (!strcmp(*branch, ".")) {
		స్థిరరాశి అక్షరం *refname = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
							      "HEAD", 0, ఖాళీ,
							      ఖాళీ);

		ఒకవేళ (!refname)
			తిరిగి die_సందేశం(_("No such ref: %s"), "HEAD");

		/* detached HEAD */
		ఒకవేళ (!strcmp(refname, "HEAD"))
			తిరిగి die_సందేశం(_("Submodule (%s) branch configured to inherit "
					     "branch from superproject, but the superproject "
					     "is not on any branch"), sub->name);

		ఒకవేళ (!skip_ఉపసర్గ(refname, "refs/heads/", &refname))
			తిరిగి die_సందేశం(_("Expecting a full ref name, got %s"),
					   refname);

		*branch = refname;
		తిరిగి 0;
	}

	/* Our "branch" is coming from repo_config_get_string_tmp() */
	తిరిగి 0;
}

స్థిర పూర్ణాంకం ensure_core_worktree(స్థిరరాశి అక్షరం *మార్గం)
{
	స్థిరరాశి అక్షరం *cw;
	నిర్మాణం repository subrepo;

	ఒకవేళ (repo_submodule_init(&subrepo, the_repository, మార్గం, null_oid(the_hash_algo)))
		తిరిగి die_సందేశం(_("could not get a repository handle for submodule '%s'"),
				   మార్గం);

	ఒకవేళ (!repo_config_get_string_tmp(&subrepo, "core.worktree", &cw)) {
		అక్షరం *cfg_file, *abs_మార్గం;
		స్థిరరాశి అక్షరం *rel_మార్గం;
		నిర్మాణం strbuf sb = STRBUF_INIT;

		cfg_file = repo_git_మార్గం(&subrepo, "config");

		abs_మార్గం = absolute_pathdup(మార్గం);
		rel_మార్గం = relative_మార్గం(abs_మార్గం, subrepo.gitdir, &sb);

		repo_config_సమితి_లోపల_file(the_repository, cfg_file, "core.worktree", rel_మార్గం);

		విడుదల(cfg_file);
		విడుదల(abs_మార్గం);
		strbuf_release(&sb);
	}

	repo_clear(&subrepo);
	తిరిగి 0;
}

స్థిర స్థిరరాశి అక్షరం *submodule_update_type_గమ్యం_label(గణన submodule_update_type type)
{
	ఎంపిక (type) {
	స్థితి SM_UPDATE_CHECKOUT:
		తిరిగి "checkout";
	స్థితి SM_UPDATE_విలీనం:
		తిరిగి "merge";
	స్థితి SM_UPDATE_REBASE:
		తిరిగి "rebase";
	స్థితి SM_UPDATE_UNSPECIFIED:
	స్థితి SM_UPDATE_NONE:
	స్థితి SM_UPDATE_COMMAND:
		ఆపు;
	}
	BUG("unreachable with type %d", type);
}

స్థిర శూన్య update_దత్తాంశం_గమ్యం_వాదనలు(స్థిరరాశి నిర్మాణం update_దత్తాంశం *update_దత్తాంశం,
				నిర్మాణం strvec *వాదనలు)
{
	గణన submodule_update_type update_type = update_దత్తాంశం->update_default;

	strvec_pushl(వాదనలు, "submodule--helper", "update", "--recursive", ఖాళీ);
	ఒకవేళ (update_దత్తాంశం->displaypath)
		strvec_pushf(వాదనలు, "--super-prefix=%s/",
			     update_దత్తాంశం->displaypath);
	strvec_pushf(వాదనలు, "--jobs=%d", update_దత్తాంశం->max_jobs);
	ఒకవేళ (update_దత్తాంశం->quiet)
		strvec_నెట్టు(వాదనలు, "--quiet");
	ఒకవేళ (update_దత్తాంశం->force)
		strvec_నెట్టు(వాదనలు, "--force");
	ఒకవేళ (update_దత్తాంశం->init)
		strvec_నెట్టు(వాదనలు, "--init");
	ఒకవేళ (update_దత్తాంశం->remote)
		strvec_నెట్టు(వాదనలు, "--remote");
	ఒకవేళ (update_దత్తాంశం->nofetch)
		strvec_నెట్టు(వాదనలు, "--no-fetch");
	ఒకవేళ (update_దత్తాంశం->dissociate)
		strvec_నెట్టు(వాదనలు, "--dissociate");
	ఒకవేళ (update_దత్తాంశం->పురోగతి)
		strvec_నెట్టు(వాదనలు, "--progress");
	ఒకవేళ (update_దత్తాంశం->require_init)
		strvec_నెట్టు(వాదనలు, "--require-init");
	ఒకవేళ (update_దత్తాంశం->లోతు)
		strvec_pushf(వాదనలు, "--depth=%d", update_దత్తాంశం->లోతు);
	ఒకవేళ (update_type != SM_UPDATE_UNSPECIFIED)
		strvec_pushf(వాదనలు, "--%s",
			     submodule_update_type_గమ్యం_label(update_type));

	ఒకవేళ (update_దత్తాంశం->references.nr) {
		నిర్మాణం string_list_అంశం *అంశం;

		for_each_string_list_అంశం(అంశం, &update_దత్తాంశం->references)
			strvec_pushl(వాదనలు, "--reference", అంశం->string, ఖాళీ);
	}
	ఒకవేళ (update_దత్తాంశం->ref_storage_format != REF_STORAGE_FORMAT_UNKNOWN)
		strvec_pushf(వాదనలు, "--ref-storage-format=%s",
			     ref_storage_format_గమ్యం_name(update_దత్తాంశం->ref_storage_format));
	ఒకవేళ (update_దత్తాంశం->filter_options && update_దత్తాంశం->filter_options->choice)
		strvec_pushf(వాదనలు, "--filter=%s",
				expand_list_objects_filter_spec(
					update_దత్తాంశం->filter_options));
	ఒకవేళ (update_దత్తాంశం->recommend_shallow == 0)
		strvec_నెట్టు(వాదనలు, "--no-recommend-shallow");
	లేకపోతే ఒకవేళ (update_దత్తాంశం->recommend_shallow == 1)
		strvec_నెట్టు(వాదనలు, "--recommend-shallow");
	ఒకవేళ (update_దత్తాంశం->single_branch >= 0)
		strvec_నెట్టు(వాదనలు, update_దత్తాంశం->single_branch ?
				    "--single-branch" :
				    "--no-single-branch");
}

స్థిర పూర్ణాంకం update_submodule(నిర్మాణం update_దత్తాంశం *update_దత్తాంశం)
{
	పూర్ణాంకం ret;

	ఒకవేళ (validate_submodule_మార్గం(update_దత్తాంశం->sm_మార్గం) < 0)
		తిరిగి -1;

	ret = determine_submodule_update_strategy(the_repository,
						  update_దత్తాంశం->just_cloned,
						  update_దత్తాంశం->sm_మార్గం,
						  update_దత్తాంశం->update_default,
						  &update_దత్తాంశం->update_strategy);
	ఒకవేళ (ret)
		తిరిగి ret;

	ఒకవేళ (update_దత్తాంశం->just_cloned)
		oidcpy(&update_దత్తాంశం->suboid, null_oid(the_hash_algo));
	లేకపోతే ఒకవేళ (repo_resolve_gitlink_ref(the_repository, update_దత్తాంశం->sm_మార్గం,
					  "HEAD", &update_దత్తాంశం->suboid))
		తిరిగి die_సందేశం(_("Unable to find current revision in submodule path '%s'"),
				   update_దత్తాంశం->displaypath);

	ఒకవేళ (update_దత్తాంశం->remote) {
		అక్షరం *remote_name;
		స్థిరరాశి అక్షరం *branch;
		అక్షరం *remote_ref;
		పూర్ణాంకం సంకేతం_2;

		సంకేతం_2 = get_default_remote_submodule(update_దత్తాంశం->sm_మార్గం, &remote_name);
		ఒకవేళ (సంకేతం_2)
			తిరిగి సంకేతం_2;
		సంకేతం_2 = remote_submodule_branch(update_దత్తాంశం->sm_మార్గం, &branch);
		ఒకవేళ (సంకేతం_2) {
			విడుదల(remote_name);
			తిరిగి సంకేతం_2;
		}
		remote_ref = xstrfmt("refs/remotes/%s/%s", remote_name, branch);

		విడుదల(remote_name);

		ఒకవేళ (!update_దత్తాంశం->nofetch) {
			ఒకవేళ (fetch_లోపల_submodule(update_దత్తాంశం->sm_మార్గం, update_దత్తాంశం->లోతు,
					      0, ఖాళీ)) {
				విడుదల(remote_ref);
				తిరిగి die_సందేశం(_("Unable to fetch in submodule path '%s'"),
						   update_దత్తాంశం->sm_మార్గం);
			}
		}

		ఒకవేళ (repo_resolve_gitlink_ref(the_repository, update_దత్తాంశం->sm_మార్గం,
					     remote_ref, &update_దత్తాంశం->oid)) {
			ret = die_సందేశం(_("Unable to find %s revision in submodule path '%s'"),
					  remote_ref, update_దత్తాంశం->sm_మార్గం);
			విడుదల(remote_ref);
			తిరిగి ret;
		}

		విడుదల(remote_ref);
	}

	ఒకవేళ (!oideq(&update_దత్తాంశం->oid, &update_దత్తాంశం->suboid) || update_దత్తాంశం->force) {
		ret = నిరంతరం_update_procedure(update_దత్తాంశం);
		ఒకవేళ (ret)
			తిరిగి ret;
	}

	ఒకవేళ (update_దత్తాంశం->recursive) {
		నిర్మాణం బిడ్డ_process cp = బిడ్డ_PROCESS_INIT;
		నిర్మాణం update_దత్తాంశం తదుపరి = *update_దత్తాంశం;

		తదుపరి.ఉపసర్గ = ఖాళీ;
		oidcpy(&తదుపరి.oid, null_oid(the_hash_algo));
		oidcpy(&తదుపరి.suboid, null_oid(the_hash_algo));

		cp.dir = update_దత్తాంశం->sm_మార్గం;
		cp.git_cmd = 1;
		prepare_submodule_repo_env(&cp.env);
		update_దత్తాంశం_గమ్యం_వాదనలు(&తదుపరి, &cp.వాదనలు);

		ret = నిరంతరం_command(&cp);
		ఒకవేళ (ret)
			die_సందేశం(_("Failed to recurse into submodule path '%s'"),
				    update_దత్తాంశం->displaypath);
		తిరిగి ret;
	}

	తిరిగి 0;
}

స్థిర పూర్ణాంకం update_submodules(నిర్మాణం update_దత్తాంశం *update_దత్తాంశం)
{
	పూర్ణాంకం i, ret = 0;
	నిర్మాణం submodule_update_clone suc = SUBMODULE_UPDATE_CLONE_INIT;
	స్థిరరాశి నిర్మాణం నిరంతరం_process_parallel_opts opts = {
		.tr2_category = "submodule",
		.tr2_label = "parallel/update",

		.processes = update_దత్తాంశం->max_jobs,

		.get_తదుపరి_task = update_clone_get_తదుపరి_task,
		.ప్రారంభం_failure = update_clone_ప్రారంభం_failure,
		.task_ముగిసిన = update_clone_task_ముగిసిన,
		.దత్తాంశం = &suc,
	};

	suc.update_దత్తాంశం = update_దత్తాంశం;
	నిరంతరం_processes_parallel(&opts);

	/*
	 * We saved the output and put it out all at once now.
	 * That means:
	 * - the listener does not have to interleave their (checkout)
	 *   work with our fetching.  The writes involved in a
	 *   checkout involve more straightforward sequential I/O.
	 * - the listener can avoid doing any work if fetching failed.
	 */
	ఒకవేళ (suc.quickstop) {
		ret = 1;
		వెళ్ళు cleanup;
	}

	కొరకు (i = 0; i < suc.update_clone_nr; i++) {
		నిర్మాణం update_clone_దత్తాంశం ucd = suc.update_clone[i];
		పూర్ణాంకం సంకేతం_2 = 128;

		oidcpy(&update_దత్తాంశం->oid, &ucd.oid);
		update_దత్తాంశం->just_cloned = ucd.just_cloned;
		update_దత్తాంశం->sm_మార్గం = ucd.sub->మార్గం;

		/*
		 * Verify that the submodule path does not contain any
		 * symlinks; if it does, it might have been tampered with.
		 * TODO: allow exempting it via
		 * `safe.submodule.path` or something
		 */
		ఒకవేళ (validate_submodule_మార్గం(update_దత్తాంశం->sm_మార్గం) < 0)
			వెళ్ళు fail;

		సంకేతం_2 = ensure_core_worktree(update_దత్తాంశం->sm_మార్గం);
		ఒకవేళ (సంకేతం_2)
			వెళ్ళు fail;

		update_దత్తాంశం->displaypath = get_submodule_displaypath(
			update_దత్తాంశం->sm_మార్గం, update_దత్తాంశం->ఉపసర్గ,
			update_దత్తాంశం->super_ఉపసర్గ);
		సంకేతం_2 = update_submodule(update_దత్తాంశం);
		FREE_AND_NULL(update_దత్తాంశం->displaypath);
fail:
		ఒకవేళ (!సంకేతం_2)
			కొనసాగు;
		ret = సంకేతం_2;
		ఒకవేళ (ret == 128)
			వెళ్ళు cleanup;
	}

cleanup:
	submodule_update_clone_release(&suc);
	string_list_clear(&update_దత్తాంశం->references, 0);
	తిరిగి ret;
}

స్థిర పూర్ణాంకం module_update(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
			 నిర్మాణం repository *repo వాడని)
{
	నిర్మాణం pathspec pathspec = { 0 };
	నిర్మాణం pathspec pathspec2 = { 0 };
	నిర్మాణం update_దత్తాంశం opt = UPDATE_దత్తాంశం_INIT;
	నిర్మాణం list_objects_filter_options filter_options =
		LIST_OBJECTS_FILTER_INIT;
	స్థిరరాశి అక్షరం *ref_storage_format = ఖాళీ;
	పూర్ణాంకం ret;
	నిర్మాణం option module_update_options[] = {
		OPT_SUPER_ఉపసర్గ(&opt.super_ఉపసర్గ),
		OPT__FORCE(&opt.force, N_("force checkout updates"), 0),
		OPT_BOOL('i', "init", &opt.init,
			 N_("initialize uninitialized submodules before update")),
		OPT_BOOL(0, "remote", &opt.remote,
			 N_("use SHA-1 of submodule's remote tracking branch")),
		OPT_BOOL(0, "recursive", &opt.recursive,
			 N_("traverse submodules recursively")),
		OPT_BOOL('N', "no-fetch", &opt.nofetch,
			 N_("don't fetch new objects from the remote site")),
		OPT_సమితి_INT(0, "checkout", &opt.update_default,
			N_("use the 'checkout' update strategy (default)"),
			SM_UPDATE_CHECKOUT),
		OPT_సమితి_INT('m', "merge", &opt.update_default,
			N_("use the 'merge' update strategy"),
			SM_UPDATE_విలీనం),
		OPT_సమితి_INT('r', "rebase", &opt.update_default,
			N_("use the 'rebase' update strategy"),
			SM_UPDATE_REBASE),
		OPT_STRING_LIST(0, "reference", &opt.references, N_("repo"),
			   N_("reference repository")),
		OPT_STRING(0, "ref-storage-format", &ref_storage_format, N_("format"),
			   N_("specify the reference storage format to use")),
		OPT_ALIAS_F(0, "ref-format", "ref-storage-format", PARSE_OPT_HIDDEN),
		OPT_BOOL(0, "dissociate", &opt.dissociate,
			   N_("use --reference only while cloning")),
		OPT_INTEGER(0, "depth", &opt.లోతు,
			   N_("create a shallow clone truncated to the "
			      "specified number of revisions")),
		OPT_INTEGER('j', "jobs", &opt.max_jobs,
			    N_("parallel jobs")),
		OPT_BOOL(0, "recommend-shallow", &opt.recommend_shallow,
			    N_("whether the initial clone should follow the shallow recommendation")),
		OPT__QUIET(&opt.quiet, N_("don't print cloning progress")),
		OPT_BOOL(0, "progress", &opt.పురోగతి,
			    N_("force cloning progress")),
		OPT_BOOL(0, "require-init", &opt.require_init,
			   N_("disallow cloning into non-empty directory, implies --init")),
		OPT_BOOL(0, "single-branch", &opt.single_branch,
			 N_("clone only one branch, HEAD or --branch")),
		OPT_PARSE_LIST_OBJECTS_FILTER(&filter_options),
		OPT_చివర()
	};
	స్థిరరాశి అక్షరం *స్థిరరాశి git_submodule_helper_usage[] = {
		N_("git submodule [--quiet] update"
		" [--init [--filter=<filter-spec>]] [--remote]"
		" [-N|--no-fetch] [-f|--force]"
		" [--checkout|--merge|--rebase]"
		" [--[no-]recommend-shallow] [--reference <repository>]"
		" [--recursive] [--[no-]single-branch] [--] [<path>...]"),
		ఖాళీ
	};

	update_clone_config_from_gitmodules(the_repository, &opt.max_jobs);
	repo_config(the_repository, git_update_clone_config, &opt.max_jobs);

	argc = parse_options(argc, argv, ఉపసర్గ, module_update_options,
			     git_submodule_helper_usage, 0);

	ఒకవేళ (opt.require_init)
		opt.init = 1;

	ఒకవేళ (filter_options.choice && !opt.init) {
		usage_with_options(git_submodule_helper_usage,
				   module_update_options);
	}

	ఒకవేళ (ref_storage_format) {
		opt.ref_storage_format = ref_storage_format_by_name(ref_storage_format);
		ఒకవేళ (opt.ref_storage_format == REF_STORAGE_FORMAT_UNKNOWN)
			die(_("unknown ref storage format '%s'"), ref_storage_format);
	}

	opt.filter_options = &filter_options;
	opt.ఉపసర్గ = ఉపసర్గ;

	ఒకవేళ (opt.update_default)
		opt.update_strategy.type = opt.update_default;

	ఒకవేళ (module_list_compute(argv, ఉపసర్గ, &pathspec, &opt.list) < 0) {
		ret = 1;
		వెళ్ళు cleanup;
	}

	ఒకవేళ (pathspec.nr)
		opt.warn_if_uninitialized = 1;

	ఒకవేళ (opt.init) {
		నిర్మాణం module_list list = MODULE_LIST_INIT;
		నిర్మాణం init_cb info = INIT_CB_INIT;

		ఒకవేళ (module_list_compute(argv, opt.ఉపసర్గ,
					&pathspec2, &list) < 0) {
			module_list_release(&list);
			ret = 1;
			వెళ్ళు cleanup;
		}

		/*
		 * If there are no path args and submodule.active is set then,
		 * by default, only initialize 'active' modules.
		 */
		ఒకవేళ (!argc && !repo_config_get(the_repository, "submodule.active"))
			module_list_చురుకైన(&list);

		info.ఉపసర్గ = opt.ఉపసర్గ;
		info.super_ఉపసర్గ = opt.super_ఉపసర్గ;
		ఒకవేళ (opt.quiet)
			info.flags |= OPT_QUIET;

		for_each_listed_submodule(&list, init_submodule_cb, &info);
		module_list_release(&list);
	}

	ret = update_submodules(&opt);
cleanup:
	update_దత్తాంశం_release(&opt);
	list_objects_filter_release(&filter_options);
	clear_pathspec(&pathspec);
	clear_pathspec(&pathspec2);
	తిరిగి ret;
}

స్థిర పూర్ణాంకం నెట్టు_check(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ వాడని,
		      నిర్మాణం repository *repo వాడని)
{
	నిర్మాణం remote *remote;
	స్థిరరాశి అక్షరం *superproject_తల;
	అక్షరం *తల;
	పూర్ణాంకం detached_తల = 0;
	నిర్మాణం object_id తల_oid;

	ఒకవేళ (argc < 3)
		die("submodule--helper push-check requires at least 2 arguments");

	/*
	 * superproject's resolved head ref.
	 * if HEAD then the superproject is in a detached head state, otherwise
	 * it will be the resolved head ref.
	 */
	superproject_తల = argv[1];
	argv++;
	argc--;
	/* Get the submodule's head ref and determine if it is detached */
	తల = refs_resolve_refdup(get_main_ref_store(the_repository), "HEAD",
				   0, &తల_oid, ఖాళీ);
	ఒకవేళ (!తల)
		die(_("Failed to resolve HEAD as a valid ref."));
	ఒకవేళ (!strcmp(తల, "HEAD"))
		detached_తల = 1;

	/*
	 * The remote must be configured.
	 * This is to avoid pushing to the exact same URL as the parent.
	 */
	remote = pushremote_get(argv[1]);
	ఒకవేళ (!remote || remote->origin == REMOTE_UNCONFIGURED)
		die("remote '%s' not configured", argv[1]);

	/* Check the refspec */
	ఒకవేళ (argc > 2) {
		పూర్ణాంకం i;
		నిర్మాణం ref *local_refs = get_local_heads();
		నిర్మాణం refspec refspec = REFSPEC_INIT_నెట్టు(the_hash_algo);

		refspec_appendn(&refspec, argv + 2, argc - 2);

		కొరకు (i = 0; i < refspec.nr; i++) {
			స్థిరరాశి నిర్మాణం refspec_అంశం *rs = &refspec.వస్తువులు[i];

			ఒకవేళ (rs->నమూనా || rs->matching)
				కొనసాగు;

			/* LHS must match a single ref */
			ఎంపిక (లెక్క_refspec_సరిపోలిక(rs->src, local_refs, ఖాళీ)) {
			స్థితి 1:
				ఆపు;
			స్థితి 0:
				/*
				 * If LHS matches 'HEAD' then we need to ensure
				 * that it matches the same named branch
				 * checked out in the superproject.
				 */
				ఒకవేళ (!strcmp(rs->src, "HEAD")) {
					ఒకవేళ (!detached_తల &&
					    !strcmp(తల, superproject_తల))
						ఆపు;
					die("HEAD does not match the named branch in the superproject");
				}
				/* fallthrough */
			అప్రమేయ:
				die("src refspec '%s' must name a ref",
				    rs->src);
			}
		}

		refspec_clear(&refspec);
		free_refs(local_refs);
	}
	విడుదల(తల);

	తిరిగి 0;
}

స్థిర పూర్ణాంకం absorb_git_dirs(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
			   నిర్మాణం repository *repo వాడని)
{
	పూర్ణాంకం i;
	నిర్మాణం pathspec pathspec = { 0 };
	నిర్మాణం module_list list = MODULE_LIST_INIT;
	స్థిరరాశి అక్షరం *super_ఉపసర్గ = ఖాళీ;
	నిర్మాణం option embed_gitdir_options[] = {
		OPT_SUPER_ఉపసర్గ(&super_ఉపసర్గ),
		OPT_చివర()
	};
	స్థిరరాశి అక్షరం *స్థిరరాశి git_submodule_helper_usage[] = {
		N_("git submodule absorbgitdirs [<options>] [<path>...]"),
		ఖాళీ
	};
	పూర్ణాంకం ret = 1;

	argc = parse_options(argc, argv, ఉపసర్గ, embed_gitdir_options,
			     git_submodule_helper_usage, 0);

	ఒకవేళ (module_list_compute(argv, ఉపసర్గ, &pathspec, &list) < 0)
		వెళ్ళు cleanup;

	కొరకు (i = 0; i < list.nr; i++)
		absorb_git_dir_into_superproject(list.entries[i]->name,
						 super_ఉపసర్గ);

	ret = 0;
cleanup:
	clear_pathspec(&pathspec);
	module_list_release(&list);
	తిరిగి ret;
}

స్థిర పూర్ణాంకం module_సమితి_url(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
			  నిర్మాణం repository *repo వాడని)
{
	పూర్ణాంకం quiet = 0, ret;
	స్థిరరాశి అక్షరం *newurl;
	స్థిరరాశి అక్షరం *మార్గం;
	అక్షరం *config_name;
	నిర్మాణం option options[] = {
		OPT__QUIET(&quiet, N_("suppress output for setting url of a submodule")),
		OPT_చివర()
	};
	స్థిరరాశి అక్షరం *స్థిరరాశి usage[] = {
		N_("git submodule set-url [--quiet] <path> <newurl>"),
		ఖాళీ
	};
	స్థిరరాశి నిర్మాణం submodule *sub;

	argc = parse_options(argc, argv, ఉపసర్గ, options, usage, 0);

	ఒకవేళ (argc != 2 || !(మార్గం = argv[0]) || !(newurl = argv[1]))
		usage_with_options(usage, options);

	sub = submodule_from_మార్గం(the_repository, null_oid(the_hash_algo), మార్గం);

	ఒకవేళ (!sub)
		die(_("no submodule mapping found in .gitmodules for path '%s'"),
		    మార్గం);

	config_name = xstrfmt("submodule.%s.url", sub->name);
	ret = config_సమితి_లోపల_gitmodules_file_gently(the_repository, config_name, newurl);

	ఒకవేళ (!ret) {
		repo_read_gitmodules(the_repository, 0);
		sync_submodule(sub->మార్గం, ఉపసర్గ, ఖాళీ, quiet ? OPT_QUIET : 0);
	}

	విడుదల(config_name);
	తిరిగి !!ret;
}

స్థిర పూర్ణాంకం module_సమితి_branch(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
			     నిర్మాణం repository *repo వాడని)
{
	పూర్ణాంకం opt_default = 0, ret;
	స్థిరరాశి అక్షరం *opt_branch = ఖాళీ;
	స్థిరరాశి అక్షరం *మార్గం;
	అక్షరం *config_name;
	నిర్మాణం option options[] = {
		/*
		 * We accept the `quiet` option for uniformity across subcommands,
		 * though there is nothing to make less verbose in this subcommand.
		 */
		OPT_NOOP_NOARG('q', "quiet"),

		OPT_BOOL('d', "default", &opt_default,
			N_("set the default tracking branch to master")),
		OPT_STRING('b', "branch", &opt_branch, N_("branch"),
			N_("set the default tracking branch")),
		OPT_చివర()
	};
	స్థిరరాశి అక్షరం *స్థిరరాశి usage[] = {
		N_("git submodule set-branch [-q|--quiet] (-d|--default) <path>"),
		N_("git submodule set-branch [-q|--quiet] (-b|--branch) <branch> <path>"),
		ఖాళీ
	};
	స్థిరరాశి నిర్మాణం submodule *sub;

	argc = parse_options(argc, argv, ఉపసర్గ, options, usage, 0);

	ఒకవేళ (!opt_branch && !opt_default)
		die(_("--branch or --default required"));

	ఒకవేళ (opt_branch && opt_default)
		die(_("options '%s' and '%s' cannot be used together"), "--branch", "--default");

	ఒకవేళ (argc != 1 || !(మార్గం = argv[0]))
		usage_with_options(usage, options);

	sub = submodule_from_మార్గం(the_repository, null_oid(the_hash_algo), మార్గం);

	ఒకవేళ (!sub)
		die(_("no submodule mapping found in .gitmodules for path '%s'"),
		    మార్గం);

	config_name = xstrfmt("submodule.%s.branch", sub->name);
	ret = config_సమితి_లోపల_gitmodules_file_gently(the_repository, config_name, opt_branch);

	విడుదల(config_name);
	తిరిగి !!ret;
}

స్థిర పూర్ణాంకం module_create_branch(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
				నిర్మాణం repository *repo వాడని)
{
	గణన branch_track track;
	పూర్ణాంకం quiet = 0, force = 0, reflog = 0, dry_నిరంతరం = 0;
	నిర్మాణం option options[] = {
		OPT__QUIET(&quiet, N_("print only error messages")),
		OPT__FORCE(&force, N_("force creation"), 0),
		OPT_BOOL(0, "create-reflog", &reflog,
			 N_("create the branch's reflog")),
		OPT_CALLBACK_F('t', "track",  &track, "(direct|inherit)",
			N_("set branch tracking configuration"),
			PARSE_OPT_OPTARG,
			parse_opt_tracking_బహుళకం),
		OPT_DRY_నిరంతరం(&dry_నిరంతరం,
			     N_("show whether the branch would be created")),
		OPT_చివర()
	};
	స్థిరరాశి అక్షరం *స్థిరరాశి usage[] = {
		N_("git submodule--helper create-branch [-f|--force] [--create-reflog] [-q|--quiet] [-t|--track] [-n|--dry-run] <name> <start-oid> <start-name>"),
		ఖాళీ
	};
	నిర్మాణం repo_config_విలువలు *cfg = repo_config_విలువలు(the_repository);

	repo_config(the_repository, git_default_config, ఖాళీ);
	track = cfg->branch_track;
	argc = parse_options(argc, argv, ఉపసర్గ, options, usage, 0);

	ఒకవేళ (argc != 3)
		usage_with_options(usage, options);

	ఒకవేళ (!quiet && !dry_నిరంతరం)
		printf_ln(_("creating branch '%s'"), argv[0]);

	create_branches_recursively(the_repository, argv[0], argv[1], argv[2],
				    force, reflog, quiet, track, dry_నిరంతరం);
	తిరిగి 0;
}

నిర్మాణం కూడు_దత్తాంశం {
	స్థిరరాశి అక్షరం *ఉపసర్గ;
	స్థిరరాశి అక్షరం *branch;
	స్థిరరాశి అక్షరం *reference_మార్గం;
	అక్షరం *sm_మార్గం;
	స్థిరరాశి అక్షరం *sm_name;
	స్థిరరాశి అక్షరం *repo;
	స్థిరరాశి అక్షరం *realrepo;
	గణన ref_storage_format ref_storage_format;
	పూర్ణాంకం లోతు;
	గుర్తులేని పూర్ణాంకం force: 1;
	గుర్తులేని పూర్ణాంకం quiet: 1;
	గుర్తులేని పూర్ణాంకం పురోగతి: 1;
	గుర్తులేని పూర్ణాంకం dissociate: 1;
};
#నిర్వచించు కూడు_దత్తాంశం_INIT { \
	.లోతు = -1, \
	.ref_storage_format = REF_STORAGE_FORMAT_UNKNOWN, \
}

స్థిర శూన్య append_fetch_remotes(నిర్మాణం strbuf *సందేశం, స్థిరరాశి అక్షరం *git_dir_మార్గం)
{
	నిర్మాణం బిడ్డ_process cp_remote = బిడ్డ_PROCESS_INIT;
	నిర్మాణం strbuf sb_remote_బహిర్గతం = STRBUF_INIT;

	cp_remote.git_cmd = 1;
	strvec_pushf(&cp_remote.env,
		     "GIT_DIR=%s", git_dir_మార్గం);
	strvec_నెట్టు(&cp_remote.env, "GIT_WORK_TREE=.");
	strvec_pushl(&cp_remote.వాదనలు, "remote", "-v", ఖాళీ);
	ఒకవేళ (!capture_command(&cp_remote, &sb_remote_బహిర్గతం, 0)) {
		అక్షరం *తదుపరి_పంక్తి;
		అక్షరం *పంక్తి = sb_remote_బహిర్గతం.buf;

		అయితే ((తదుపరి_పంక్తి = strchr(పంక్తి, '\n')) != ఖాళీ) {
			కొలత_t len = తదుపరి_పంక్తి - పంక్తి;

			ఒకవేళ (strip_suffix_mem(పంక్తి, &len, " (fetch)"))
				strbuf_addf(సందేశం, "  %.*s\n", (పూర్ణాంకం)len, పంక్తి);
			పంక్తి = తదుపరి_పంక్తి + 1;
		}
	}

	strbuf_release(&sb_remote_బహిర్గతం);
}

స్థిర పూర్ణాంకం కూడు_submodule(స్థిరరాశి నిర్మాణం కూడు_దత్తాంశం *కూడు_దత్తాంశం)
{
	నిర్మాణం module_clone_దత్తాంశం clone_దత్తాంశం = MODULE_CLONE_దత్తాంశం_INIT;
	నిర్మాణం string_list reference = STRING_LIST_INIT_NODUP;
	పూర్ణాంకం ret = -1;

	/* perhaps the path already exists and is already a git repo, else clone it */
	ఒకవేళ (is_directory(కూడు_దత్తాంశం->sm_మార్గం)) {
		అక్షరం *submod_gitdir_మార్గం;
		నిర్మాణం strbuf sm_మార్గం = STRBUF_INIT;
		strbuf_addstr(&sm_మార్గం, కూడు_దత్తాంశం->sm_మార్గం);
		submod_gitdir_మార్గం = xstrfmt("%s/.git", కూడు_దత్తాంశం->sm_మార్గం);
		ఒకవేళ (is_nonbare_repository_dir(&sm_మార్గం))
			రాయి(_("Adding existing repo at '%s' to the index\n"),
			       కూడు_దత్తాంశం->sm_మార్గం);
		లేకపోతే
			die(_("'%s' already exists and is not a valid git repo"),
			    కూడు_దత్తాంశం->sm_మార్గం);
		strbuf_release(&sm_మార్గం);
		విడుదల(submod_gitdir_మార్గం);
	} లేకపోతే {
		నిర్మాణం బిడ్డ_process cp = బిడ్డ_PROCESS_INIT;
		నిర్మాణం strbuf submod_gitdir = STRBUF_INIT;

		submodule_name_గమ్యం_gitdir(&submod_gitdir, the_repository, కూడు_దత్తాంశం->sm_name);

		ఒకవేళ (is_directory(submod_gitdir.buf)) {
			ఒకవేళ (!కూడు_దత్తాంశం->force) {
				నిర్మాణం strbuf సందేశం = STRBUF_INIT;
				అక్షరం *die_సందేశం_2;

				strbuf_addf(&సందేశం, _("A git directory for '%s' is found "
						    "locally with remote(s):\n"),
					    కూడు_దత్తాంశం->sm_name);

				append_fetch_remotes(&సందేశం, submod_gitdir.buf);
				strbuf_release(&submod_gitdir);

				strbuf_addf(&సందేశం, _("If you want to reuse this local git "
						    "directory instead of cloning again from\n"
						    "  %s\n"
						    "use the '--force' option. If the local git "
						    "directory is not the correct repo\n"
						    "or you are unsure what this means choose "
						    "another name with the '--name' option."),
					    కూడు_దత్తాంశం->realrepo);

				die_సందేశం_2 = strbuf_detach(&సందేశం, ఖాళీ);
				die("%s", die_సందేశం_2);
			} లేకపోతే {
				రాయి(_("Reactivating local git directory for "
					 "submodule '%s'\n"), కూడు_దత్తాంశం->sm_name);
			}
		}
		strbuf_release(&submod_gitdir);

		clone_దత్తాంశం.ఉపసర్గ = కూడు_దత్తాంశం->ఉపసర్గ;
		clone_దత్తాంశం.మార్గం = కూడు_దత్తాంశం->sm_మార్గం;
		clone_దత్తాంశం.name = కూడు_దత్తాంశం->sm_name;
		clone_దత్తాంశం.url = కూడు_దత్తాంశం->realrepo;
		clone_దత్తాంశం.quiet = కూడు_దత్తాంశం->quiet;
		clone_దత్తాంశం.పురోగతి = కూడు_దత్తాంశం->పురోగతి;
		ఒకవేళ (కూడు_దత్తాంశం->reference_మార్గం) {
			అక్షరం *p = xstrdup(కూడు_దత్తాంశం->reference_మార్గం);

			string_list_append(&reference, p)->util = p;
		}
		clone_దత్తాంశం.ref_storage_format = కూడు_దత్తాంశం->ref_storage_format;
		clone_దత్తాంశం.dissociate = కూడు_దత్తాంశం->dissociate;
		ఒకవేళ (కూడు_దత్తాంశం->లోతు >= 0)
			clone_దత్తాంశం.లోతు = కూడు_దత్తాంశం->లోతు;

		ఒకవేళ (clone_submodule(&clone_దత్తాంశం, &reference))
			వెళ్ళు cleanup;

		prepare_submodule_repo_env(&cp.env);
		cp.git_cmd = 1;
		cp.dir = కూడు_దత్తాంశం->sm_మార్గం;
		/*
		 * NOTE: we only get here if add_data->force is true, so
		 * passing --force to checkout is reasonable.
		 */
		strvec_pushl(&cp.వాదనలు, "checkout", "-f", "-q", ఖాళీ);

		ఒకవేళ (కూడు_దత్తాంశం->branch) {
			strvec_pushl(&cp.వాదనలు, "-B", కూడు_దత్తాంశం->branch, ఖాళీ);
			strvec_pushf(&cp.వాదనలు, "origin/%s", కూడు_దత్తాంశం->branch);
		}

		ఒకవేళ (నిరంతరం_command(&cp))
			die(_("unable to checkout submodule '%s'"), కూడు_దత్తాంశం->sm_మార్గం);
	}
	ret = 0;

cleanup:
	string_list_clear(&reference, 1);
	తిరిగి ret;
}

స్థిర పూర్ణాంకం config_submodule_లోపల_gitmodules(స్థిరరాశి అక్షరం *name, స్థిరరాశి అక్షరం *var, స్థిరరాశి అక్షరం *విలువ)
{
	అక్షరం *కీలకం;
	పూర్ణాంకం ret;

	ఒకవేళ (!is_writing_gitmodules_ok())
		die(_("please make sure that the .gitmodules file is in the working tree"));

	కీలకం = xstrfmt("submodule.%s.%s", name, var);
	ret = config_సమితి_లోపల_gitmodules_file_gently(the_repository, కీలకం, విలువ);
	విడుదల(కీలకం);

	తిరిగి ret;
}

స్థిర శూన్య configure_added_submodule(నిర్మాణం కూడు_దత్తాంశం *కూడు_దత్తాంశం)
{
	అక్షరం *కీలకం;
	నిర్మాణం బిడ్డ_process కూడు_submod = బిడ్డ_PROCESS_INIT;
	నిర్మాణం బిడ్డ_process కూడు_gitmodules = బిడ్డ_PROCESS_INIT;
	స్థిరరాశి నిర్మాణం string_list *విలువలు;
	పూర్ణాంకం matched = 0;

	కీలకం = xstrfmt("submodule.%s.url", కూడు_దత్తాంశం->sm_name);
	repo_config_సమితి_gently(the_repository, కీలకం, కూడు_దత్తాంశం->realrepo);
	విడుదల(కీలకం);

	కూడు_submod.git_cmd = 1;
	strvec_pushl(&కూడు_submod.వాదనలు, "add",
		     "--no-warn-embedded-repo", ఖాళీ);
	ఒకవేళ (కూడు_దత్తాంశం->force)
		strvec_నెట్టు(&కూడు_submod.వాదనలు, "--force");
	strvec_pushl(&కూడు_submod.వాదనలు, "--", కూడు_దత్తాంశం->sm_మార్గం, ఖాళీ);

	ఒకవేళ (నిరంతరం_command(&కూడు_submod))
		die(_("Failed to add submodule '%s'"), కూడు_దత్తాంశం->sm_మార్గం);

	ఒకవేళ (config_submodule_లోపల_gitmodules(కూడు_దత్తాంశం->sm_name, "path", కూడు_దత్తాంశం->sm_మార్గం) ||
	    config_submodule_లోపల_gitmodules(కూడు_దత్తాంశం->sm_name, "url", కూడు_దత్తాంశం->repo))
		die(_("Failed to register submodule '%s'"), కూడు_దత్తాంశం->sm_మార్గం);

	ఒకవేళ (కూడు_దత్తాంశం->branch) {
		ఒకవేళ (config_submodule_లోపల_gitmodules(కూడు_దత్తాంశం->sm_name,
						   "branch", కూడు_దత్తాంశం->branch))
			die(_("Failed to register submodule '%s'"), కూడు_దత్తాంశం->sm_మార్గం);
	}

	కూడు_gitmodules.git_cmd = 1;
	strvec_pushl(&కూడు_gitmodules.వాదనలు,
		     "add", "--force", "--", ".gitmodules", ఖాళీ);

	ఒకవేళ (నిరంతరం_command(&కూడు_gitmodules))
		die(_("Failed to register submodule '%s'"), కూడు_దత్తాంశం->sm_మార్గం);

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
	ఒకవేళ (repo_config_get(the_repository, "submodule.active") || /* key absent */
	    repo_config_get_string_multi(the_repository, "submodule.active", &విలువలు)) {
		/*
		 * If the submodule being added isn't already covered by the
		 * current configured pathspec, set the submodule's active flag
		 */
		కీలకం = xstrfmt("submodule.%s.active", కూడు_దత్తాంశం->sm_name);
		repo_config_సమితి_gently(the_repository, కీలకం, "true");
		విడుదల(కీలకం);
	} లేకపోతే {
		కొరకు (కొలత_t i = 0; i < విలువలు->nr; i++) {
			స్థిరరాశి అక్షరం *pat = విలువలు->వస్తువులు[i].string;
			ఒకవేళ (!wildmatch(pat, కూడు_దత్తాంశం->sm_మార్గం, 0)) { /* match found */
				matched = 1;
				ఆపు;
			}
		}
		ఒకవేళ (!matched) { /* no pattern matched -> force-enable */
			కీలకం = xstrfmt("submodule.%s.active", కూడు_దత్తాంశం->sm_name);
			repo_config_సమితి_gently(the_repository, కీలకం, "true");
			విడుదల(కీలకం);
		}
	}
}

స్థిర శూన్య die_on_సూచీ_సరిపోలిక(స్థిరరాశి అక్షరం *మార్గం, పూర్ణాంకం force)
{
	నిర్మాణం pathspec ps;
	స్థిరరాశి అక్షరం *వాదనలు[] = { మార్గం, ఖాళీ };
	parse_pathspec(&ps, 0, PATHSPEC_PREFER_CWD, ఖాళీ, వాదనలు);

	ఒకవేళ (repo_read_సూచీ_preload(the_repository, ఖాళీ, 0) < 0)
		die(_("index file corrupt"));

	ఒకవేళ (ps.nr) {
		అక్షరం *ps_matched = xcalloc(ps.nr, 1);

		/* TODO: audit for interaction with sparse-index. */
		ensure_నిండిన_సూచీ(the_repository->సూచీ);

		/*
		 * Since there is only one pathspec, we just need to
		 * check ps_matched[0] to know if a cache entry matched.
		 */
		కొరకు (కొలత_t i = 0; i < the_repository->సూచీ->cache_nr; i++) {
			ce_మార్గం_సరిపోలిక(the_repository->సూచీ, the_repository->సూచీ->cache[i], &ps,
				      ps_matched);

			ఒకవేళ (ps_matched[0]) {
				ఒకవేళ (!force)
					die(_("'%s' already exists in the index"),
					    మార్గం);
				ఒకవేళ (!S_ISGITLINK(the_repository->సూచీ->cache[i]->ce_బహుళకం))
					die(_("'%s' already exists in the index "
					      "and is not a submodule"), మార్గం);
				ఆపు;
			}
		}
		విడుదల(ps_matched);
	}
	clear_pathspec(&ps);
}

స్థిర శూన్య die_on_repo_without_commits(స్థిరరాశి అక్షరం *మార్గం)
{
	నిర్మాణం strbuf sb = STRBUF_INIT;
	strbuf_addstr(&sb, మార్గం);
	ఒకవేళ (is_nonbare_repository_dir(&sb)) {
		నిర్మాణం object_id oid;
		ఒకవేళ (repo_resolve_gitlink_ref(the_repository, మార్గం, "HEAD", &oid) < 0)
			die(_("'%s' does not have a commit checked out"), మార్గం);
	}
	strbuf_release(&sb);
}

స్థిర పూర్ణాంకం module_కూడు(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
		      నిర్మాణం repository *repo వాడని)
{
	పూర్ణాంకం force = 0, quiet = 0, పురోగతి = 0, dissociate = 0;
	నిర్మాణం కూడు_దత్తాంశం కూడు_దత్తాంశం = కూడు_దత్తాంశం_INIT;
	స్థిరరాశి అక్షరం *ref_storage_format = ఖాళీ;
	అక్షరం *గమ్యం_free = ఖాళీ;
	స్థిరరాశి నిర్మాణం submodule *existing;
	నిర్మాణం strbuf buf = STRBUF_INIT;
	అక్షరం *sm_name_గమ్యం_free = ఖాళీ;
	నిర్మాణం option options[] = {
		OPT_STRING('b', "branch", &కూడు_దత్తాంశం.branch, N_("branch"),
			   N_("branch of repository to add as submodule")),
		OPT__FORCE(&force, N_("allow adding an otherwise ignored submodule path"),
			   PARSE_OPT_NOCOMPLETE),
		OPT__QUIET(&quiet, N_("print only error messages")),
		OPT_BOOL(0, "progress", &పురోగతి, N_("force cloning progress")),
		OPT_STRING(0, "reference", &కూడు_దత్తాంశం.reference_మార్గం, N_("repository"),
			   N_("reference repository")),
		OPT_STRING(0, "ref-storage-format", &ref_storage_format, N_("format"),
			   N_("specify the reference storage format to use")),
		OPT_ALIAS_F(0, "ref-format", "ref-storage-format", PARSE_OPT_HIDDEN),
		OPT_BOOL(0, "dissociate", &dissociate, N_("borrow the objects from reference repositories")),
		OPT_STRING(0, "name", &కూడు_దత్తాంశం.sm_name, N_("name"),
			   N_("sets the submodule's name to the given string "
			      "instead of defaulting to its path")),
		OPT_INTEGER(0, "depth", &కూడు_దత్తాంశం.లోతు, N_("depth for shallow clones")),
		OPT_చివర()
	};
	స్థిరరాశి అక్షరం *స్థిరరాశి usage[] = {
		N_("git submodule add [<options>] [--] <repository> [<path>]"),
		ఖాళీ
	};
	నిర్మాణం strbuf sb = STRBUF_INIT;
	పూర్ణాంకం ret = 1;

	argc = parse_options(argc, argv, ఉపసర్గ, options, usage, 0);

	ఒకవేళ (!is_writing_gitmodules_ok())
		die(_("please make sure that the .gitmodules file is in the working tree"));

	ఒకవేళ (ఉపసర్గ && *ఉపసర్గ &&
	    కూడు_దత్తాంశం.reference_మార్గం && !is_absolute_మార్గం(కూడు_దత్తాంశం.reference_మార్గం))
		కూడు_దత్తాంశం.reference_మార్గం = xstrfmt("%s%s", ఉపసర్గ, కూడు_దత్తాంశం.reference_మార్గం);

	ఒకవేళ (argc == 0 || argc > 2)
		usage_with_options(usage, options);

	ఒకవేళ (ref_storage_format) {
		కూడు_దత్తాంశం.ref_storage_format = ref_storage_format_by_name(ref_storage_format);
		ఒకవేళ (కూడు_దత్తాంశం.ref_storage_format == REF_STORAGE_FORMAT_UNKNOWN)
			die(_("unknown ref storage format '%s'"), ref_storage_format);
	}

	కూడు_దత్తాంశం.repo = argv[0];
	ఒకవేళ (argc == 1)
		కూడు_దత్తాంశం.sm_మార్గం = git_url_basename(కూడు_దత్తాంశం.repo, 0, 0);
	లేకపోతే
		కూడు_దత్తాంశం.sm_మార్గం = xstrdup(argv[1]);

	ఒకవేళ (ఉపసర్గ && *ఉపసర్గ && !is_absolute_మార్గం(కూడు_దత్తాంశం.sm_మార్గం)) {
		అక్షరం *sm_మార్గం = కూడు_దత్తాంశం.sm_మార్గం;

		కూడు_దత్తాంశం.sm_మార్గం = xstrfmt("%s%s", ఉపసర్గ, sm_మార్గం);
		విడుదల(sm_మార్గం);
	}

	ఒకవేళ (starts_with_dot_dot_slash(కూడు_దత్తాంశం.repo) ||
	    starts_with_dot_slash(కూడు_దత్తాంశం.repo)) {
		ఒకవేళ (ఉపసర్గ)
			die(_("Relative path can only be used from the toplevel "
			      "of the working tree"));

		/* dereference source url relative to parent's url */
		గమ్యం_free = resolve_relative_url(కూడు_దత్తాంశం.repo, ఖాళీ, 1);
		కూడు_దత్తాంశం.realrepo = గమ్యం_free;
	} లేకపోతే ఒకవేళ (is_dir_sep(కూడు_దత్తాంశం.repo[0]) || strchr(కూడు_దత్తాంశం.repo, ':')) {
		కూడు_దత్తాంశం.realrepo = కూడు_దత్తాంశం.repo;
	} లేకపోతే {
		die(_("repo URL: '%s' must be absolute or begin with ./|../"),
		    కూడు_దత్తాంశం.repo);
	}

	/*
	 * normalize path:
	 * multiple //; leading ./; /./; /../;
	 */
	normalize_మార్గం_copy(కూడు_దత్తాంశం.sm_మార్గం, కూడు_దత్తాంశం.sm_మార్గం);
	strip_dir_trailing_slashes(కూడు_దత్తాంశం.sm_మార్గం);

	ఒకవేళ (validate_submodule_మార్గం(కూడు_దత్తాంశం.sm_మార్గం) < 0)
		die(ఖాళీ);

	die_on_సూచీ_సరిపోలిక(కూడు_దత్తాంశం.sm_మార్గం, force);
	die_on_repo_without_commits(కూడు_దత్తాంశం.sm_మార్గం);

	ఒకవేళ (!force) {
		నిర్మాణం బిడ్డ_process cp = బిడ్డ_PROCESS_INIT;

		cp.git_cmd = 1;
		cp.no_stdout = 1;
		strvec_pushl(&cp.వాదనలు, "add", "--dry-run", "--ignore-missing",
			     "--no-warn-embedded-repo", కూడు_దత్తాంశం.sm_మార్గం, ఖాళీ);
		ఒకవేళ ((ret = pipe_command(&cp, ఖాళీ, 0, ఖాళీ, 0, &sb, 0))) {
			strbuf_complete_పంక్తి(&sb);
			fputs(sb.buf, stderr);
			వెళ్ళు cleanup;
		}
	}

	ఒకవేళ (!కూడు_దత్తాంశం.sm_name)
		కూడు_దత్తాంశం.sm_name = కూడు_దత్తాంశం.sm_మార్గం;

	existing = submodule_from_name(the_repository,
					null_oid(the_hash_algo),
					కూడు_దత్తాంశం.sm_name);

	ఒకవేళ (existing && existing->మార్గం &&
	    strcmp(existing->మార్గం, కూడు_దత్తాంశం.sm_మార్గం)) {
		ఒకవేళ (!force) {
			die(_("submodule name '%s' already used for path '%s'"),
			    కూడు_దత్తాంశం.sm_name, existing->మార్గం);
		}
		/* --force: build <name><n> until unique */
		కొరకు (పూర్ణాంకం i = 1; ; i++) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "%s%d", కూడు_దత్తాంశం.sm_name, i);
			ఒకవేళ (!submodule_from_name(the_repository,
						null_oid(the_hash_algo),
						buf.buf)) {
				ఆపు;
			}
		}
		కూడు_దత్తాంశం.sm_name = sm_name_గమ్యం_free = strbuf_detach(&buf, ఖాళీ);
	}

	ఒకవేళ (check_submodule_name(కూడు_దత్తాంశం.sm_name))
		die(_("'%s' is not a valid submodule name"), కూడు_దత్తాంశం.sm_name);

	కూడు_దత్తాంశం.ఉపసర్గ = ఉపసర్గ;
	కూడు_దత్తాంశం.force = !!force;
	కూడు_దత్తాంశం.quiet = !!quiet;
	కూడు_దత్తాంశం.పురోగతి = !!పురోగతి;
	కూడు_దత్తాంశం.dissociate = !!dissociate;

	ఒకవేళ (the_repository->repository_format_submodule_మార్గం_cfg)
		create_default_gitdir_config(కూడు_దత్తాంశం.sm_name);

	ఒకవేళ (కూడు_submodule(&కూడు_దత్తాంశం))
		వెళ్ళు cleanup;
	configure_added_submodule(&కూడు_దత్తాంశం);

	ret = 0;
cleanup:
	విడుదల(sm_name_గమ్యం_free);
	విడుదల(కూడు_దత్తాంశం.sm_మార్గం);
	విడుదల(గమ్యం_free);
	strbuf_release(&sb);

	తిరిగి ret;
}

పూర్ణాంకం cmd_submodule__helper(పూర్ణాంకం argc,
			  స్థిరరాశి అక్షరం **argv,
			  స్థిరరాశి అక్షరం *ఉపసర్గ,
			  నిర్మాణం repository *repo)
{
	parse_opt_subcommand_fn *fn = ఖాళీ;
	స్థిరరాశి అక్షరం *స్థిరరాశి usage[] = {
		N_("git submodule--helper <command>"),
		ఖాళీ
	};
	నిర్మాణం option options[] = {
		OPT_SUBCOMMAND("migrate-gitdir-configs", &fn, module_migrate),
		OPT_SUBCOMMAND("gitdir", &fn, module_gitdir),
		OPT_SUBCOMMAND("clone", &fn, module_clone),
		OPT_SUBCOMMAND("add", &fn, module_కూడు),
		OPT_SUBCOMMAND("update", &fn, module_update),
		OPT_SUBCOMMAND("foreach", &fn, module_foreach),
		OPT_SUBCOMMAND("init", &fn, module_init),
		OPT_SUBCOMMAND("status", &fn, module_స్థితి),
		OPT_SUBCOMMAND("sync", &fn, module_sync),
		OPT_SUBCOMMAND("deinit", &fn, module_deinit),
		OPT_SUBCOMMAND("summary", &fn, module_summary),
		OPT_SUBCOMMAND("push-check", &fn, నెట్టు_check),
		OPT_SUBCOMMAND("absorbgitdirs", &fn, absorb_git_dirs),
		OPT_SUBCOMMAND("set-url", &fn, module_సమితి_url),
		OPT_SUBCOMMAND("set-branch", &fn, module_సమితి_branch),
		OPT_SUBCOMMAND("create-branch", &fn, module_create_branch),
		OPT_SUBCOMMAND("get-default-remote", &fn, module_get_default_remote),
		OPT_చివర()
	};
	argc = parse_options(argc, argv, ఉపసర్గ, options, usage, 0);

	తిరిగి fn(argc, argv, ఉపసర్గ, repo);
}
