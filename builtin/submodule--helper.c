#define USE_THE_REPOSITORY_VARIABLE

#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "abspath.h"
#অন্তর্ভাব "environment.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "hex.h"

#অন্তর্ভাব "config.h"
#অন্তর্ভাব "parse-options.h"
#অন্তর্ভাব "quote.h"
#অন্তর্ভাব "path.h"
#অন্তর্ভাব "pathspec.h"
#অন্তর্ভাব "preload-index.h"
#অন্তর্ভাব "dir.h"
#অন্তর্ভাব "read-cache.h"
#অন্তর্ভাব "setup.h"
#অন্তর্ভাব "sparse-index.h"
#অন্তর্ভাব "submodule.h"
#অন্তর্ভাব "submodule-config.h"
#অন্তর্ভাব "string-list.h"
#অন্তর্ভাব "run-command.h"
#অন্তর্ভাব "remote.h"
#অন্তর্ভাব "refs.h"
#অন্তর্ভাব "refspec.h"
#অন্তর্ভাব "revision.h"
#অন্তর্ভাব "diffcore.h"
#অন্তর্ভাব "diff.h"
#অন্তর্ভাব "object-file.h"
#অন্তর্ভাব "object-name.h"
#অন্তর্ভাব "odb.h"
#অন্তর্ভাব "odb/source.h"
#অন্তর্ভাব "odb/source-files.h"
#অন্তর্ভাব "advice.h"
#অন্তর্ভাব "branch.h"
#অন্তর্ভাব "list-objects-filter-options.h"
#অন্তর্ভাব "wildmatch.h"
#অন্তর্ভাব "strbuf.h"
#অন্তর্ভাব "url.h"

#define OPT_QUIET (1 << 0)
#define OPT_CACHED (1 << 1)
#define OPT_RECURSIVE (1 << 2)
#define OPT_FORCE (1 << 3)

প্রকারলেখ শূন্য (*each_submodule_fn)(ধ্রুবক কাঠামো cache_entry *list_item,
				  শূন্য *cb_তথ্য);

স্থির অক্ষর *get_default_remote(শূন্য)
{
	ফেরত xstrdup(repo_default_remote(the_repository));
}

স্থির অক্ষর *resolve_relative_url(ধ্রুবক অক্ষর *rel_url, ধ্রুবক অক্ষর *up_path, পূর্ণসংখ্যা quiet)
{
	অক্ষর *remoteurl, *resolved_url;
	অক্ষর *remote = get_default_remote();
	কাঠামো strbuf remotesb = STRBUF_INIT;

	strbuf_addf(&remotesb, "remote.%s.url", remote);
	যদি (repo_config_get_string(the_repository, remotesb.buf, &remoteurl)) {
		যদি (!quiet)
			warning(_("could not look up configuration '%s'. "
				  "Assuming this repository is its own "
				  "authoritative upstream."),
				remotesb.buf);
		remoteurl = xgetcwd();
	}
	resolved_url = relative_url(remoteurl, rel_url, up_path);

	মুক্ত(remote);
	মুক্ত(remoteurl);
	strbuf_release(&remotesb);

	ফেরত resolved_url;
}

স্থির পূর্ণসংখ্যা get_default_remote_submodule(ধ্রুবক অক্ষর *module_path, অক্ষর **default_remote)
{
	ধ্রুবক কাঠামো submodule *sub;
	কাঠামো repository subrepo;
	ধ্রুবক অক্ষর *remote_name = ফাঁকা;
	অক্ষর *url = ফাঁকা;
	পূর্ণসংখ্যা ret = 0;

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), module_path);
	যদি (sub && sub->url) {
		url = xstrdup(sub->url);

		/* Possibly a url relative to parent */
		যদি (starts_with_dot_dot_slash(url) ||
		    starts_with_dot_slash(url)) {
			অক্ষর *oldurl = url;

			url = resolve_relative_url(oldurl, ফাঁকা, 1);
			মুক্ত(oldurl);
		}
	}

	যদি (repo_submodule_init(&subrepo, the_repository, module_path,
				null_oid(the_hash_algo)) < 0) {
		ret = die_message(_("could not get a repository handle for submodule '%s'"),
				   module_path);
		যাও out;
	}

	/* Look up by URL first */
	যদি (url)
		remote_name = repo_remote_from_url(&subrepo, url);
	যদি (!remote_name)
		remote_name = repo_default_remote(&subrepo);

	*default_remote = xstrdup(remote_name);

out:
	repo_clear(&subrepo);
	মুক্ত(url);

	ফেরত ret;
}

স্থির পূর্ণসংখ্যা module_get_default_remote(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv, ধ্রুবক অক্ষর *prefix,
				     কাঠামো repository *repo UNUSED)
{
	ধ্রুবক অক্ষর *path;
	অক্ষর *resolved_path = ফাঁকা;
	অক্ষর *default_remote = ফাঁকা;
	পূর্ণসংখ্যা code;
	কাঠামো option options[] = {
		OPT_END()
	};
	ধ্রুবক অক্ষর *ধ্রুবক usage[] = {
		N_("git submodule--helper get-default-remote <path>"),
		ফাঁকা
	};

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	যদি (argc != 1)
		usage_with_options(usage, options);

	path = argv[0];
	যদি (prefix && *prefix && !is_absolute_path(path)) {
		resolved_path = xstrfmt("%s%s", prefix, path);
		path = resolved_path;
	}

	code = get_default_remote_submodule(path, &default_remote);
	যদি (code) {
		মুক্ত(resolved_path);
		ফেরত code;
	}

	printf("%s\n", default_remote);
	মুক্ত(default_remote);
	মুক্ত(resolved_path);
	ফেরত 0;
}

/* the result should be freed by the caller. */
স্থির অক্ষর *get_submodule_displaypath(ধ্রুবক অক্ষর *path, ধ্রুবক অক্ষর *prefix,
				       ধ্রুবক অক্ষর *super_prefix)
{
	যদি (prefix && super_prefix) {
		BUG("cannot have prefix '%s' and superprefix '%s'",
		    prefix, super_prefix);
	} অন্যথা যদি (prefix) {
		কাঠামো strbuf sb = STRBUF_INIT;
		অক্ষর *displaypath = xstrdup(relative_path(path, prefix, &sb));
		strbuf_release(&sb);
		ফেরত displaypath;
	} অন্যথা যদি (super_prefix) {
		ফেরত xstrfmt("%s%s", super_prefix, path);
	} অন্যথা {
		ফেরত xstrdup(path);
	}
}

স্থির অক্ষর *compute_rev_name(ধ্রুবক অক্ষর *sub_path, ধ্রুবক অক্ষর* object_id)
{
	কাঠামো strbuf sb = STRBUF_INIT;
	ধ্রুবক অক্ষর ***d;

	স্থির ধ্রুবক অক্ষর *describe_bare[] = { ফাঁকা };

	স্থির ধ্রুবক অক্ষর *describe_tags[] = { "--tags", ফাঁকা };

	স্থির ধ্রুবক অক্ষর *describe_contains[] = { "--contains", ফাঁকা };

	স্থির ধ্রুবক অক্ষর *describe_all_always[] = { "--all", "--always", ফাঁকা };

	স্থির ধ্রুবক অক্ষর **describe_argv[] = { describe_bare, describe_tags,
						describe_contains,
						describe_all_always, ফাঁকা };

	জন্য (d = describe_argv; *d; d++) {
		কাঠামো child_process cp = CHILD_PROCESS_INIT;
		prepare_submodule_repo_env(&cp.env);
		cp.dir = sub_path;
		cp.git_cmd = 1;
		cp.no_stderr = 1;

		strvec_ঠেলো(&cp.args, "describe");
		strvec_pushv(&cp.args, *d);
		strvec_ঠেলো(&cp.args, object_id);

		যদি (!capture_command(&cp, &sb, 0)) {
			strbuf_strip_suffix(&sb, "\n");
			ফেরত strbuf_detach(&sb, ফাঁকা);
		}
	}

	strbuf_release(&sb);
	ফেরত ফাঁকা;
}

কাঠামো module_list {
	ধ্রুবক কাঠামো cache_entry **entries;
	পূর্ণসংখ্যা alloc, nr;
};
#define MODULE_LIST_INIT { 0 }

স্থির শূন্য module_list_release(কাঠামো module_list *ml)
{
	মুক্ত(ml->entries);
}

স্থির পূর্ণসংখ্যা module_list_compute(ধ্রুবক অক্ষর **argv,
			       ধ্রুবক অক্ষর *prefix,
			       কাঠামো pathspec *pathspec,
			       কাঠামো module_list *list)
{
	পূর্ণসংখ্যা ফলাফল = 0;
	অক্ষর *ps_matched = ফাঁকা;

	parse_pathspec(pathspec, 0,
		       PATHSPEC_PREFER_FULL,
		       prefix, argv);

	যদি (pathspec->nr)
		ps_matched = xcalloc(pathspec->nr, 1);

	যদি (repo_read_সূচক(the_repository) < 0)
		die(_("index file corrupt"));

	জন্য (আকার_t i = 0; i < the_repository->সূচক->cache_nr; i++) {
		ধ্রুবক কাঠামো cache_entry *ce = the_repository->সূচক->cache[i];

		যদি (!match_pathspec(the_repository->সূচক, pathspec, ce->name, ce_namelen(ce),
				    0, ps_matched, 1) ||
		    !S_ISGITLINK(ce->ce_mode))
			চালু;

		ALLOC_GROW(list->entries, list->nr + 1, list->alloc);
		list->entries[list->nr++] = ce;
		যখন (i + 1 < the_repository->সূচক->cache_nr &&
		       !strcmp(ce->name, the_repository->সূচক->cache[i + 1]->name))
			/*
			 * Skip entries with the same name in different stages
			 * to make sure an entry is returned only once.
			 */
			i++;
	}

	যদি (ps_matched && report_path_error(ps_matched, pathspec))
		ফলাফল = -1;

	মুক্ত(ps_matched);

	ফেরত ফলাফল;
}

স্থির শূন্য module_list_active(কাঠামো module_list *list)
{
	পূর্ণসংখ্যা i;
	কাঠামো module_list active_modules = MODULE_LIST_INIT;

	জন্য (i = 0; i < list->nr; i++) {
		ধ্রুবক কাঠামো cache_entry *ce = list->entries[i];

		যদি (!is_submodule_active(the_repository, ce->name))
			চালু;

		ALLOC_GROW(active_modules.entries,
			   active_modules.nr + 1,
			   active_modules.alloc);
		active_modules.entries[active_modules.nr++] = ce;
	}

	module_list_release(list);
	*list = active_modules;
}

স্থির অক্ষর *get_up_path(ধ্রুবক অক্ষর *path)
{
	কাঠামো strbuf sb = STRBUF_INIT;

	strbuf_addstrings(&sb, "../", গুনতি_slashes(path));

	/*
	 * Check if 'path' ends with slash or not
	 * for having the same output for dir/sub_dir
	 * and dir/sub_dir/
	 */
	যদি (!is_dir_sep(path[strlen(path) - 1]))
		strbuf_addstr(&sb, "../");

	ফেরত strbuf_detach(&sb, ফাঁকা);
}

স্থির শূন্য for_each_listed_submodule(ধ্রুবক কাঠামো module_list *list,
				      each_submodule_fn fn, শূন্য *cb_তথ্য)
{
	পূর্ণসংখ্যা i;

	জন্য (i = 0; i < list->nr; i++)
		fn(list->entries[i], cb_তথ্য);
}

কাঠামো foreach_cb {
	পূর্ণসংখ্যা argc;
	ধ্রুবক অক্ষর **argv;
	ধ্রুবক অক্ষর *prefix;
	ধ্রুবক অক্ষর *super_prefix;
	পূর্ণসংখ্যা quiet;
	পূর্ণসংখ্যা recursive;
};
#define FOREACH_CB_INIT { 0 }

স্থির শূন্য runcommand_in_submodule_cb(ধ্রুবক কাঠামো cache_entry *list_item,
				       শূন্য *cb_তথ্য)
{
	কাঠামো foreach_cb *info = cb_তথ্য;
	ধ্রুবক অক্ষর *path = list_item->name;
	ধ্রুবক কাঠামো object_id *ce_oid = &list_item->oid;
	ধ্রুবক কাঠামো submodule *sub;
	কাঠামো child_process cp = CHILD_PROCESS_INIT;
	অক্ষর *displaypath;

	যদি (validate_submodule_path(path) < 0)
		die(ফাঁকা);

	displaypath = get_submodule_displaypath(path, info->prefix,
						info->super_prefix);

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);

	যদি (!sub)
		die(_("No url found for submodule path '%s' in .gitmodules"),
			displaypath);

	যদি (!is_submodule_populated_gently(path, ফাঁকা))
		যাও cleanup;

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
	যদি (info->argc == 1) {
		অক্ষর *toplevel = xgetcwd();
		কাঠামো strbuf sb = STRBUF_INIT;

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
		মুক্ত(toplevel);
	} অন্যথা {
		strvec_pushv(&cp.args, info->argv);
	}

	যদি (!info->quiet)
		printf(_("Entering '%s'\n"), displaypath);

	যদি (info->argv[0]) {
		যদি (run_command(&cp))
			die(_("run_command returned non-zero status for %s\n."),
			    displaypath);
	} অন্যথা {
		child_process_clear(&cp);
	}

	যদি (info->recursive) {
		কাঠামো child_process cpr = CHILD_PROCESS_INIT;

		cpr.git_cmd = 1;
		cpr.dir = path;
		prepare_submodule_repo_env(&cpr.env);

		strvec_pushl(&cpr.args, "submodule--helper", "foreach", "--recursive",
			     ফাঁকা);
		strvec_pushf(&cpr.args, "--super-prefix=%s/", displaypath);

		যদি (info->quiet)
			strvec_ঠেলো(&cpr.args, "--quiet");

		strvec_ঠেলো(&cpr.args, "--");
		strvec_pushv(&cpr.args, info->argv);

		যদি (run_command(&cpr))
			die(_("run_command returned non-zero status while "
				"recursing in the nested submodules of %s\n."),
				displaypath);
	}

cleanup:
	মুক্ত(displaypath);
}

স্থির পূর্ণসংখ্যা module_foreach(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv, ধ্রুবক অক্ষর *prefix,
			  কাঠামো repository *repo UNUSED)
{
	কাঠামো foreach_cb info = FOREACH_CB_INIT;
	কাঠামো pathspec pathspec = { 0 };
	কাঠামো module_list list = MODULE_LIST_INIT;
	কাঠামো option module_foreach_options[] = {
		OPT__SUPER_PREFIX(&info.super_prefix),
		OPT__QUIET(&info.quiet, N_("suppress output of entering each submodule command")),
		OPT_BOOL(0, "recursive", &info.recursive,
			 N_("recurse into nested submodules")),
		OPT_END()
	};
	ধ্রুবক অক্ষর *ধ্রুবক git_submodule_helper_usage[] = {
		N_("git submodule foreach [--quiet] [--recursive] [--] <command>"),
		ফাঁকা
	};
	পূর্ণসংখ্যা ret = 1;

	argc = parse_options(argc, argv, prefix, module_foreach_options,
			     git_submodule_helper_usage, 0);

	যদি (module_list_compute(ফাঁকা, prefix, &pathspec, &list) < 0)
		যাও cleanup;

	info.argc = argc;
	info.argv = argv;
	info.prefix = prefix;

	for_each_listed_submodule(&list, runcommand_in_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	ফেরত ret;
}

কাঠামো init_cb {
	ধ্রুবক অক্ষর *prefix;
	ধ্রুবক অক্ষর *super_prefix;
	unsigned পূর্ণসংখ্যা flags;
};
#define INIT_CB_INIT { 0 }

স্থির পূর্ণসংখ্যা validate_and_set_submodule_gitdir(কাঠামো strbuf *gitdir_path,
					     ধ্রুবক অক্ষর *submodule_name)
{
	ধ্রুবক অক্ষর *মান;
	অক্ষর *চাবি;

	যদি (validate_submodule_git_dir(gitdir_path->buf, submodule_name))
		ফেরত -1;

	চাবি = xstrfmt("submodule.%s.gitdir", submodule_name);

	/* Nothing to do if the config already exists. */
	যদি (!repo_config_get_string_tmp(the_repository, চাবি, &মান)) {
		মুক্ত(চাবি);
		ফেরত 0;
	}

	যদি (repo_config_set_gently(the_repository, চাবি, gitdir_path->buf)) {
		মুক্ত(চাবি);
		ফেরত -1;
	}

	মুক্ত(চাবি);
	ফেরত 0;
}

স্থির শূন্য create_default_gitdir_config(ধ্রুবক অক্ষর *submodule_name)
{
	কাঠামো strbuf gitdir_path = STRBUF_INIT;
	কাঠামো git_hash_ctx ctx;
	অক্ষর hex_name_hash[GIT_MAX_HEXSZ + 1], header[128];
	unsigned অক্ষর raw_name_hash[GIT_MAX_RAWSZ];
	পূর্ণসংখ্যা header_len;

	/* Case 1: try the plain module name */
	repo_git_path_append(the_repository, &gitdir_path, "modules/%s", submodule_name);
	যদি (!validate_and_set_submodule_gitdir(&gitdir_path, submodule_name)) {
		strbuf_release(&gitdir_path);
		ফেরত;
	}

	/* Case 2.1: Try URI-safe (RFC3986) encoding first, this fixes nested gitdirs */
	strbuf_reset(&gitdir_path);
	repo_git_path_append(the_repository, &gitdir_path, "modules/");
	strbuf_addstr_urlencode(&gitdir_path, submodule_name, is_rfc3986_unreserved);
	যদি (!validate_and_set_submodule_gitdir(&gitdir_path, submodule_name)) {
		strbuf_release(&gitdir_path);
		ফেরত;
	}

	/* Case 2.2: Try extended uppercase URI (RFC3986) encoding, to fix case-folding */
	strbuf_reset(&gitdir_path);
	repo_git_path_append(the_repository, &gitdir_path, "modules/");
	strbuf_addstr_urlencode(&gitdir_path, submodule_name, is_casefolding_rfc3986_unreserved);
	যদি (!validate_and_set_submodule_gitdir(&gitdir_path, submodule_name))
		ফেরত;

	/* Case 2.3: Try some derived gitdir names, see if one sticks */
	জন্য (অক্ষর c = '0'; c <= '9'; c++) {
		strbuf_reset(&gitdir_path);
		repo_git_path_append(the_repository, &gitdir_path, "modules/");
		strbuf_addstr_urlencode(&gitdir_path, submodule_name, is_rfc3986_unreserved);
		strbuf_addch(&gitdir_path, c);
		যদি (!validate_and_set_submodule_gitdir(&gitdir_path, submodule_name))
			ফেরত;

		strbuf_reset(&gitdir_path);
		repo_git_path_append(the_repository, &gitdir_path, "modules/");
		strbuf_addstr_urlencode(&gitdir_path, submodule_name, is_casefolding_rfc3986_unreserved);
		strbuf_addch(&gitdir_path, c);
		যদি (!validate_and_set_submodule_gitdir(&gitdir_path, submodule_name))
			ফেরত;
	}

	/* Case 2.4: If all the above failed, try a hash of the name as a last resort */
	header_len = snprintf(header, আকার(header),
			      "blob %"PRIuMAX, (uintmax_t)strlen(submodule_name));
	git_hash_init(&ctx, the_hash_algo);
	git_hash_update(&ctx, header, header_len);
	git_hash_update(&ctx, "\0", 1);
	git_hash_update(&ctx, submodule_name, strlen(submodule_name));
	git_hash_final(raw_name_hash, &ctx);
	hash_to_hex_algop_r(hex_name_hash, raw_name_hash, the_hash_algo);
	strbuf_reset(&gitdir_path);
	repo_git_path_append(the_repository, &gitdir_path, "modules/%s", hex_name_hash);
	যদি (!validate_and_set_submodule_gitdir(&gitdir_path, submodule_name)) {
		strbuf_release(&gitdir_path);
		ফেরত;
	}

	/* Case 3: nothing worked, error out */
	die(_("failed to set a valid default config for 'submodule.%s.gitdir'. "
	      "Please ensure it is set, for example by running something like: "
	      "'git config submodule.%s.gitdir .git/modules/%s'"),
	    submodule_name, submodule_name, submodule_name);
}

স্থির শূন্য init_submodule(ধ্রুবক অক্ষর *path, ধ্রুবক অক্ষর *prefix,
			   ধ্রুবক অক্ষর *super_prefix,
			   unsigned পূর্ণসংখ্যা flags)
{
	ধ্রুবক কাঠামো submodule *sub;
	কাঠামো strbuf sb = STRBUF_INIT;
	ধ্রুবক অক্ষর *upd;
	অক্ষর *url = ফাঁকা, *displaypath;

	displaypath = get_submodule_displaypath(path, prefix, super_prefix);

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);

	যদি (!sub)
		die(_("No url found for submodule path '%s' in .gitmodules"),
			displaypath);

	/*
	 * NEEDSWORK: In a multi-working-tree world, this needs to be
	 * set in the per-worktree config.
	 *
	 * Set active flag for the submodule being initialized
	 */
	যদি (!is_submodule_active(the_repository, path)) {
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
	যদি (repo_config_get_string(the_repository, sb.buf, &url)) {
		যদি (!sub->url)
			die(_("No url found for submodule path '%s' in .gitmodules"),
				displaypath);

		url = xstrdup(sub->url);

		/* Possibly a url relative to parent */
		যদি (starts_with_dot_dot_slash(url) ||
		    starts_with_dot_slash(url)) {
			অক্ষর *oldurl = url;

			url = resolve_relative_url(oldurl, ফাঁকা, 0);
			মুক্ত(oldurl);
		}

		যদি (repo_config_set_gently(the_repository, sb.buf, url))
			die(_("Failed to register url for submodule path '%s'"),
			    displaypath);
		যদি (!(flags & OPT_QUIET))
			fprintf(stderr,
				_("Submodule '%s' (%s) registered for path '%s'\n"),
				sub->name, url, displaypath);
	}
	strbuf_reset(&sb);

	/* Copy "update" setting when it is not set yet */
	strbuf_addf(&sb, "submodule.%s.update", sub->name);
	যদি (repo_config_get_string_tmp(the_repository, sb.buf, &upd) &&
	    sub->update_strategy.type != SM_UPDATE_UNSPECIFIED) {
		যদি (sub->update_strategy.type == SM_UPDATE_COMMAND) {
			fprintf(stderr, _("warning: command update mode suggested for submodule '%s'\n"),
				sub->name);
			upd = "none";
		} অন্যথা {
			upd = submodule_update_type_to_string(sub->update_strategy.type);
		}

		যদি (repo_config_set_gently(the_repository, sb.buf, upd))
			die(_("Failed to register update mode for submodule path '%s'"), displaypath);
	}

	যদি (the_repository->repository_format_submodule_path_cfg)
		create_default_gitdir_config(sub->name);

	strbuf_release(&sb);
	মুক্ত(displaypath);
	মুক্ত(url);
}

স্থির শূন্য init_submodule_cb(ধ্রুবক কাঠামো cache_entry *list_item, শূন্য *cb_তথ্য)
{
	কাঠামো init_cb *info = cb_তথ্য;

	init_submodule(list_item->name, info->prefix, info->super_prefix,
		       info->flags);
}

স্থির পূর্ণসংখ্যা module_init(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv, ধ্রুবক অক্ষর *prefix,
		       কাঠামো repository *repo UNUSED)
{
	কাঠামো init_cb info = INIT_CB_INIT;
	কাঠামো pathspec pathspec = { 0 };
	কাঠামো module_list list = MODULE_LIST_INIT;
	পূর্ণসংখ্যা quiet = 0;
	কাঠামো option module_init_options[] = {
		OPT__QUIET(&quiet, N_("suppress output for initializing a submodule")),
		OPT_END()
	};
	ধ্রুবক অক্ষর *ধ্রুবক git_submodule_helper_usage[] = {
		N_("git submodule init [<options>] [<path>]"),
		ফাঁকা
	};
	পূর্ণসংখ্যা ret = 1;

	argc = parse_options(argc, argv, prefix, module_init_options,
			     git_submodule_helper_usage, 0);

	যদি (module_list_compute(argv, prefix, &pathspec, &list) < 0)
		যাও cleanup;

	/*
	 * If there are no path args and submodule.active is set then,
	 * by default, only initialize 'active' modules.
	 */
	যদি (!argc && !repo_config_get(the_repository, "submodule.active"))
		module_list_active(&list);

	info.prefix = prefix;
	যদি (quiet)
		info.flags |= OPT_QUIET;

	for_each_listed_submodule(&list, init_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	ফেরত ret;
}

কাঠামো status_cb {
	ধ্রুবক অক্ষর *prefix;
	ধ্রুবক অক্ষর *super_prefix;
	unsigned পূর্ণসংখ্যা flags;
};
#define STATUS_CB_INIT { 0 }

স্থির শূন্য print_status(unsigned পূর্ণসংখ্যা flags, অক্ষর state, ধ্রুবক অক্ষর *path,
			 ধ্রুবক কাঠামো object_id *oid, ধ্রুবক অক্ষর *displaypath)
{
	যদি (flags & OPT_QUIET)
		ফেরত;

	printf("%c%s %s", state, oid_to_hex(oid), displaypath);

	যদি (state == ' ' || state == '+') {
		অক্ষর *name = compute_rev_name(path, oid_to_hex(oid));

		যদি (name)
			printf(" (%s)", name);
		মুক্ত(name);
	}

	printf("\n");
}

স্থির পূর্ণসংখ্যা handle_submodule_শীর্ষ_ref(ধ্রুবক কাঠামো reference *ref, শূন্য *cb_তথ্য)
{
	কাঠামো object_id *output = cb_তথ্য;

	যদি (ref->oid)
		oidcpy(output, ref->oid);

	ফেরত 0;
}

স্থির শূন্য status_submodule(ধ্রুবক অক্ষর *path, ধ্রুবক কাঠামো object_id *ce_oid,
			     unsigned পূর্ণসংখ্যা ce_flags, ধ্রুবক অক্ষর *prefix,
			     ধ্রুবক অক্ষর *super_prefix, unsigned পূর্ণসংখ্যা flags)
{
	অক্ষর *displaypath;
	কাঠামো strvec diff_files_args = STRVEC_INIT;
	কাঠামো rev_info rev = REV_INFO_INIT;
	কাঠামো strbuf buf = STRBUF_INIT;
	ধ্রুবক অক্ষর *git_dir;

	যদি (validate_submodule_path(path) < 0)
		die(ফাঁকা);

	যদি (!submodule_from_path(the_repository, null_oid(the_hash_algo), path))
		die(_("no submodule mapping found in .gitmodules for path '%s'"),
		      path);

	displaypath = get_submodule_displaypath(path, prefix, super_prefix);

	যদি ((CE_STAGEMASK & ce_flags) >> CE_STAGESHIFT) {
		print_status(flags, 'U', path, null_oid(the_hash_algo), displaypath);
		যাও cleanup;
	}

	strbuf_addf(&buf, "%s/.git", path);
	git_dir = read_gitfile(buf.buf);
	যদি (!git_dir)
		git_dir = buf.buf;

	যদি (!is_submodule_active(the_repository, path) ||
	    !is_git_directory(git_dir)) {
		print_status(flags, '-', path, ce_oid, displaypath);
		strbuf_release(&buf);
		যাও cleanup;
	}
	strbuf_release(&buf);

	strvec_pushl(&diff_files_args, "diff-files",
		     "--ignore-submodules=dirty", "--quiet", "--",
		     path, ফাঁকা);

	repo_config(the_repository, git_diff_basic_config, ফাঁকা);

	repo_init_revisions(the_repository, &rev, ফাঁকা);
	rev.abbrev = 0;
	setup_revisions_from_strvec(&diff_files_args, &rev, ফাঁকা);
	run_diff_files(&rev, 0);

	যদি (!diff_ফলাফল_code(&rev)) {
		print_status(flags, ' ', path, ce_oid,
			     displaypath);
	} অন্যথা যদি (!(flags & OPT_CACHED)) {
		কাঠামো object_id oid;
		কাঠামো ref_store *refs = repo_get_submodule_ref_store(the_repository,
								      path);

		যদি (!refs) {
			print_status(flags, '-', path, ce_oid, displaypath);
			যাও cleanup;
		}
		যদি (refs_শীর্ষ_ref(refs, handle_submodule_শীর্ষ_ref, &oid))
			die(_("could not resolve HEAD ref inside the "
			      "submodule '%s'"), path);

		print_status(flags, '+', path, &oid, displaypath);
	} অন্যথা {
		print_status(flags, '+', path, ce_oid, displaypath);
	}

	যদি (flags & OPT_RECURSIVE) {
		কাঠামো child_process cpr = CHILD_PROCESS_INIT;
		পূর্ণসংখ্যা res;

		cpr.git_cmd = 1;
		cpr.dir = path;
		prepare_submodule_repo_env(&cpr.env);

		strvec_pushl(&cpr.args, "submodule--helper", "status",
			     "--recursive", ফাঁকা);
		strvec_pushf(&cpr.args, "--super-prefix=%s/", displaypath);

		যদি (flags & OPT_CACHED)
			strvec_ঠেলো(&cpr.args, "--cached");

		যদি (flags & OPT_QUIET)
			strvec_ঠেলো(&cpr.args, "--quiet");

		res = run_command(&cpr);
		যদি (res == SIGPIPE + 128)
			তোলো(SIGPIPE);
		অন্যথা যদি (res)
			die(_("failed to recurse into submodule '%s'"), path);
	}

cleanup:
	strvec_clear(&diff_files_args);
	মুক্ত(displaypath);
	release_revisions(&rev);
}

স্থির শূন্য status_submodule_cb(ধ্রুবক কাঠামো cache_entry *list_item,
				শূন্য *cb_তথ্য)
{
	কাঠামো status_cb *info = cb_তথ্য;

	status_submodule(list_item->name, &list_item->oid, list_item->ce_flags,
			 info->prefix, info->super_prefix, info->flags);
}

স্থির পূর্ণসংখ্যা module_status(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv, ধ্রুবক অক্ষর *prefix,
			 কাঠামো repository *repo UNUSED)
{
	কাঠামো status_cb info = STATUS_CB_INIT;
	কাঠামো pathspec pathspec = { 0 };
	কাঠামো module_list list = MODULE_LIST_INIT;
	পূর্ণসংখ্যা quiet = 0;
	কাঠামো option module_status_options[] = {
		OPT__SUPER_PREFIX(&info.super_prefix),
		OPT__QUIET(&quiet, N_("suppress submodule status output")),
		OPT_BIT(0, "cached", &info.flags, N_("use commit stored in the index instead of the one stored in the submodule HEAD"), OPT_CACHED),
		OPT_BIT(0, "recursive", &info.flags, N_("recurse into nested submodules"), OPT_RECURSIVE),
		OPT_END()
	};
	ধ্রুবক অক্ষর *ধ্রুবক git_submodule_helper_usage[] = {
		N_("git submodule status [--quiet] [--cached] [--recursive] [<path>...]"),
		ফাঁকা
	};
	পূর্ণসংখ্যা ret = 1;

	argc = parse_options(argc, argv, prefix, module_status_options,
			     git_submodule_helper_usage, 0);

	যদি (module_list_compute(argv, prefix, &pathspec, &list) < 0)
		যাও cleanup;

	info.prefix = prefix;
	যদি (quiet)
		info.flags |= OPT_QUIET;

	for_each_listed_submodule(&list, status_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	ফেরত ret;
}

কাঠামো module_cb {
	unsigned পূর্ণসংখ্যা mod_src;
	unsigned পূর্ণসংখ্যা mod_dst;
	কাঠামো object_id oid_src;
	কাঠামো object_id oid_dst;
	অক্ষর status;
	অক্ষর *sm_path;
};
#define MODULE_CB_INIT { 0 }

স্থির শূন্য module_cb_release(কাঠামো module_cb *mcb)
{
	মুক্ত(mcb->sm_path);
}

কাঠামো module_cb_list {
	কাঠামো module_cb **entries;
	পূর্ণসংখ্যা alloc, nr;
};
#define MODULE_CB_LIST_INIT { 0 }

স্থির শূন্য module_cb_list_release(কাঠামো module_cb_list *mcbl)
{
	পূর্ণসংখ্যা i;

	জন্য (i = 0; i < mcbl->nr; i++) {
		কাঠামো module_cb *mcb = mcbl->entries[i];

		module_cb_release(mcb);
		মুক্ত(mcb);
	}
	মুক্ত(mcbl->entries);
}

কাঠামো summary_cb {
	পূর্ণসংখ্যা argc;
	ধ্রুবক অক্ষর **argv;
	ধ্রুবক অক্ষর *prefix;
	ধ্রুবক অক্ষর *super_prefix;
	unsigned পূর্ণসংখ্যা cached: 1;
	unsigned পূর্ণসংখ্যা for_status: 1;
	unsigned পূর্ণসংখ্যা files: 1;
	পূর্ণসংখ্যা summary_limit;
};
#define SUMMARY_CB_INIT { 0 }

গণনা diff_cmd {
	DIFF_সূচক,
	DIFF_FILES
};

স্থির অক্ষর *verify_submodule_committish(ধ্রুবক অক্ষর *sm_path,
					 ধ্রুবক অক্ষর *committish)
{
	কাঠামো child_process cp_rev_parse = CHILD_PROCESS_INIT;
	কাঠামো strbuf ফলাফল = STRBUF_INIT;

	cp_rev_parse.git_cmd = 1;
	cp_rev_parse.dir = sm_path;
	prepare_submodule_repo_env(&cp_rev_parse.env);
	strvec_pushl(&cp_rev_parse.args, "rev-parse", "-q", "--short", ফাঁকা);
	strvec_pushf(&cp_rev_parse.args, "%s^0", committish);
	strvec_ঠেলো(&cp_rev_parse.args, "--");

	যদি (capture_command(&cp_rev_parse, &ফলাফল, 0))
		ফেরত ফাঁকা;

	strbuf_trim_trailing_newline(&ফলাফল);
	ফেরত strbuf_detach(&ফলাফল, ফাঁকা);
}

স্থির শূন্য print_submodule_summary(কাঠামো summary_cb *info, ধ্রুবক অক্ষর *errmsg,
				    পূর্ণসংখ্যা যোগ_commits, ধ্রুবক অক্ষর *displaypath,
				    ধ্রুবক অক্ষর *src_abbrev, ধ্রুবক অক্ষর *dst_abbrev,
				    কাঠামো module_cb *p)
{
	যদি (p->status == 'T') {
		যদি (S_ISGITLINK(p->mod_dst))
			printf(_("* %s %s(blob)->%s(submodule)"),
				 displaypath, src_abbrev, dst_abbrev);
		অন্যথা
			printf(_("* %s %s(submodule)->%s(blob)"),
				 displaypath, src_abbrev, dst_abbrev);
	} অন্যথা {
		printf("* %s %s...%s",
			displaypath, src_abbrev, dst_abbrev);
	}

	যদি (যোগ_commits < 0)
		printf(":\n");
	অন্যথা
		printf(" (%d):\n", যোগ_commits);

	যদি (errmsg) {
		printf(_("%s"), errmsg);
	} অন্যথা যদি (যোগ_commits > 0) {
		কাঠামো child_process cp_log = CHILD_PROCESS_INIT;

		cp_log.git_cmd = 1;
		cp_log.dir = p->sm_path;
		prepare_submodule_repo_env(&cp_log.env);
		strvec_pushl(&cp_log.args, "log", ফাঁকা);

		যদি (S_ISGITLINK(p->mod_src) && S_ISGITLINK(p->mod_dst)) {
			যদি (info->summary_limit > 0)
				strvec_pushf(&cp_log.args, "-%d",
					     info->summary_limit);

			strvec_pushl(&cp_log.args, "--pretty=  %m %s",
				     "--first-parent", ফাঁকা);
			strvec_pushf(&cp_log.args, "%s...%s",
				     src_abbrev, dst_abbrev);
		} অন্যথা যদি (S_ISGITLINK(p->mod_dst)) {
			strvec_pushl(&cp_log.args, "--pretty=  > %s",
				     "-1", dst_abbrev, ফাঁকা);
		} অন্যথা {
			strvec_pushl(&cp_log.args, "--pretty=  < %s",
				     "-1", src_abbrev, ফাঁকা);
		}
		run_command(&cp_log);
	}
	printf("\n");
}

স্থির শূন্য generate_submodule_summary(কাঠামো summary_cb *info,
				       কাঠামো module_cb *p)
{
	অক্ষর *displaypath, *src_abbrev = ফাঁকা, *dst_abbrev;
	পূর্ণসংখ্যা missing_src = 0, missing_dst = 0;
	কাঠামো strbuf errmsg = STRBUF_INIT;
	পূর্ণসংখ্যা যোগ_commits = -1;

	যদি (!info->cached && oideq(&p->oid_dst, null_oid(the_hash_algo))) {
		যদি (S_ISGITLINK(p->mod_dst)) {
			কাঠামো ref_store *refs = repo_get_submodule_ref_store(the_repository,
									      p->sm_path);

			যদি (refs)
				refs_শীর্ষ_ref(refs, handle_submodule_শীর্ষ_ref, &p->oid_dst);
		} অন্যথা যদি (S_ISLNK(p->mod_dst) || S_ISREG(p->mod_dst)) {
			কাঠামো stat st;
			পূর্ণসংখ্যা fd = open(p->sm_path, O_RDONLY);

			যদি (fd < 0 || fstat(fd, &st) < 0 ||
			    সূচক_fd(the_repository->সূচক, &p->oid_dst, fd, &st, OBJ_BLOB,
				     p->sm_path, 0))
				error(_("couldn't hash object from '%s'"), p->sm_path);
		} অন্যথা {
			/* for a submodule removal (mode:0000000), don't warn */
			যদি (p->mod_dst)
				warning(_("unexpected mode %o"), p->mod_dst);
		}
	}

	যদি (S_ISGITLINK(p->mod_src)) {
		যদি (p->status != 'D')
			src_abbrev = verify_submodule_committish(p->sm_path,
								 oid_to_hex(&p->oid_src));
		যদি (!src_abbrev) {
			missing_src = 1;
			/*
			 * As `rev-parse` failed, we fallback to getting
			 * the abbreviated hash using oid_src. We do
			 * this as we might still need the abbreviated
			 * hash in cases like a submodule type change, etc.
			 */
			src_abbrev = xstrndup(oid_to_hex(&p->oid_src), 7);
		}
	} অন্যথা {
		/*
		 * The source does not point to a submodule.
		 * So, we fallback to getting the abbreviation using
		 * oid_src as we might still need the abbreviated
		 * hash in cases like submodule add, etc.
		 */
		src_abbrev = xstrndup(oid_to_hex(&p->oid_src), 7);
	}

	যদি (S_ISGITLINK(p->mod_dst)) {
		dst_abbrev = verify_submodule_committish(p->sm_path,
							 oid_to_hex(&p->oid_dst));
		যদি (!dst_abbrev) {
			missing_dst = 1;
			/*
			 * As `rev-parse` failed, we fallback to getting
			 * the abbreviated hash using oid_dst. We do
			 * this as we might still need the abbreviated
			 * hash in cases like a submodule type change, etc.
			 */
			dst_abbrev = xstrndup(oid_to_hex(&p->oid_dst), 7);
		}
	} অন্যথা {
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

	যদি (!missing_src && !missing_dst) {
		কাঠামো child_process cp_rev_list = CHILD_PROCESS_INIT;
		কাঠামো strbuf sb_rev_list = STRBUF_INIT;

		strvec_pushl(&cp_rev_list.args, "rev-list",
			     "--first-parent", "--count", ফাঁকা);
		যদি (S_ISGITLINK(p->mod_src) && S_ISGITLINK(p->mod_dst))
			strvec_pushf(&cp_rev_list.args, "%s...%s",
				     src_abbrev, dst_abbrev);
		অন্যথা
			strvec_ঠেলো(&cp_rev_list.args, S_ISGITLINK(p->mod_src) ?
				    src_abbrev : dst_abbrev);
		strvec_ঠেলো(&cp_rev_list.args, "--");

		cp_rev_list.git_cmd = 1;
		cp_rev_list.dir = p->sm_path;
		prepare_submodule_repo_env(&cp_rev_list.env);

		যদি (!capture_command(&cp_rev_list, &sb_rev_list, 0))
			যোগ_commits = atoi(sb_rev_list.buf);

		strbuf_release(&sb_rev_list);
	} অন্যথা {
		/*
		 * Don't give error msg for modification whose dst is not
		 * submodule, i.e., deleted or changed to blob
		 */
		যদি (S_ISGITLINK(p->mod_dst)) {
			যদি (missing_src && missing_dst) {
				strbuf_addf(&errmsg, "  Warn: %s doesn't contain commits %s and %s\n",
					    displaypath, oid_to_hex(&p->oid_src),
					    oid_to_hex(&p->oid_dst));
			} অন্যথা {
				strbuf_addf(&errmsg, "  Warn: %s doesn't contain commit %s\n",
					    displaypath, missing_src ?
					    oid_to_hex(&p->oid_src) :
					    oid_to_hex(&p->oid_dst));
			}
		}
	}

	print_submodule_summary(info, errmsg.len ? errmsg.buf : ফাঁকা,
				যোগ_commits, displaypath, src_abbrev,
				dst_abbrev, p);

	মুক্ত(displaypath);
	মুক্ত(src_abbrev);
	মুক্ত(dst_abbrev);
	strbuf_release(&errmsg);
}

স্থির শূন্য prepare_submodule_summary(কাঠামো summary_cb *info,
				      কাঠামো module_cb_list *list)
{
	পূর্ণসংখ্যা i;
	জন্য (i = 0; i < list->nr; i++) {
		ধ্রুবক কাঠামো submodule *sub;
		কাঠামো module_cb *p = list->entries[i];
		কাঠামো strbuf sm_gitdir = STRBUF_INIT;

		যদি (p->status == 'D' || p->status == 'T') {
			generate_submodule_summary(info, p);
			চালু;
		}

		যদি (info->for_status && p->status != 'A' &&
		    (sub = submodule_from_path(the_repository,
					       null_oid(the_hash_algo), p->sm_path))) {
			অক্ষর *config_চাবি = ফাঁকা;
			ধ্রুবক অক্ষর *মান;
			পূর্ণসংখ্যা ignore_all = 0;

			config_চাবি = xstrfmt("submodule.%s.ignore",
					     sub->name);
			যদি (!repo_config_get_string_tmp(the_repository, config_চাবি, &মান))
				ignore_all = !strcmp(মান, "all");
			অন্যথা যদি (sub->ignore)
				ignore_all = !strcmp(sub->ignore, "all");

			মুক্ত(config_চাবি);
			যদি (ignore_all)
				চালু;
		}

		/* Also show added or modified modules which are checked out */
		strbuf_addstr(&sm_gitdir, p->sm_path);
		যদি (is_nonbare_repository_dir(&sm_gitdir))
			generate_submodule_summary(info, p);
		strbuf_release(&sm_gitdir);
	}
}

স্থির শূন্য submodule_summary_callback(কাঠামো diff_queue_struct *q,
				       কাঠামো diff_options *options UNUSED,
				       শূন্য *তথ্য)
{
	পূর্ণসংখ্যা i;
	কাঠামো module_cb_list *list = তথ্য;
	জন্য (i = 0; i < q->nr; i++) {
		কাঠামো diff_filepair *p = q->queue[i];
		কাঠামো module_cb *অস্থায়ী;

		যদি (!S_ISGITLINK(p->one->mode) && !S_ISGITLINK(p->two->mode))
			চালু;
		অস্থায়ী = xmalloc(আকার(*অস্থায়ী));
		অস্থায়ী->mod_src = p->one->mode;
		অস্থায়ী->mod_dst = p->two->mode;
		অস্থায়ী->oid_src = p->one->oid;
		অস্থায়ী->oid_dst = p->two->oid;
		অস্থায়ী->status = p->status;
		অস্থায়ী->sm_path = xstrdup(p->one->path);

		ALLOC_GROW(list->entries, list->nr + 1, list->alloc);
		list->entries[list->nr++] = অস্থায়ী;
	}
}

স্থির ধ্রুবক অক্ষর *get_diff_cmd(গণনা diff_cmd diff_cmd)
{
	নির্বাচন (diff_cmd) {
	ক্ষেত্র DIFF_সূচক: ফেরত "diff-index";
	ক্ষেত্র DIFF_FILES: ফেরত "diff-files";
	পূর্বনির্ধারিত: BUG("bad diff_cmd value %d", diff_cmd);
	}
}

স্থির পূর্ণসংখ্যা compute_summary_module_list(কাঠামো object_id *শীর্ষ_oid,
				       কাঠামো summary_cb *info,
				       গণনা diff_cmd diff_cmd)
{
	কাঠামো strvec diff_args = STRVEC_INIT;
	কাঠামো rev_info rev;
	কাঠামো module_cb_list list = MODULE_CB_LIST_INIT;
	পূর্ণসংখ্যা ret = 0;

	strvec_ঠেলো(&diff_args, get_diff_cmd(diff_cmd));
	যদি (info->cached)
		strvec_ঠেলো(&diff_args, "--cached");
	strvec_pushl(&diff_args, "--ignore-submodules=dirty", "--raw", ফাঁকা);
	যদি (শীর্ষ_oid)
		strvec_ঠেলো(&diff_args, oid_to_hex(শীর্ষ_oid));
	strvec_ঠেলো(&diff_args, "--");
	যদি (info->argc)
		strvec_pushv(&diff_args, info->argv);

	repo_config(the_repository, git_diff_basic_config, ফাঁকা);
	repo_init_revisions(the_repository, &rev, info->prefix);
	rev.abbrev = 0;
	precompose_argv_prefix(diff_args.nr, diff_args.v, ফাঁকা);
	setup_revisions_from_strvec(&diff_args, &rev, ফাঁকা);
	rev.diffopt.output_format = DIFF_FORMAT_NO_OUTPUT | DIFF_FORMAT_CALLBACK;
	rev.diffopt.format_callback = submodule_summary_callback;
	rev.diffopt.format_callback_তথ্য = &list;

	যদি (!info->cached) {
		যদি (diff_cmd == DIFF_সূচক)
			setup_work_বৃক্ষ(the_repository);
		যদি (repo_read_সূচক_preload(the_repository, &rev.diffopt.pathspec, 0) < 0) {
			perror("repo_read_index_preload");
			ret = -1;
			যাও cleanup;
		}
	} অন্যথা যদি (repo_read_সূচক(the_repository) < 0) {
		perror("repo_read_cache");
		ret = -1;
		যাও cleanup;
	}

	যদি (diff_cmd == DIFF_সূচক)
		run_diff_সূচক(&rev, info->cached ? DIFF_সূচক_CACHED : 0);
	অন্যথা
		run_diff_files(&rev, 0);
	prepare_submodule_summary(info, &list);
cleanup:
	strvec_clear(&diff_args);
	release_revisions(&rev);
	module_cb_list_release(&list);
	ফেরত ret;
}

স্থির পূর্ণসংখ্যা module_summary(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv, ধ্রুবক অক্ষর *prefix,
			  কাঠামো repository *repo UNUSED)
{
	কাঠামো summary_cb info = SUMMARY_CB_INIT;
	পূর্ণসংখ্যা cached = 0;
	পূর্ণসংখ্যা for_status = 0;
	পূর্ণসংখ্যা files = 0;
	পূর্ণসংখ্যা summary_limit = -1;
	গণনা diff_cmd diff_cmd = DIFF_সূচক;
	কাঠামো object_id শীর্ষ_oid;
	পূর্ণসংখ্যা ret;
	কাঠামো option module_summary_options[] = {
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
	ধ্রুবক অক্ষর *ধ্রুবক git_submodule_helper_usage[] = {
		N_("git submodule summary [<options>] [<commit>] [--] [<path>]"),
		ফাঁকা
	};

	argc = parse_options(argc, argv, prefix, module_summary_options,
			     git_submodule_helper_usage, 0);

	যদি (!summary_limit)
		ফেরত 0;

	যদি (!repo_get_oid(the_repository, argc ? argv[0] : "HEAD", &শীর্ষ_oid)) {
		যদি (argc) {
			argv++;
			argc--;
		}
	} অন্যথা যদি (!argc || !strcmp(argv[0], "HEAD")) {
		/* before the first commit: compare with an empty tree */
		oidcpy(&শীর্ষ_oid, the_hash_algo->empty_বৃক্ষ);
		যদি (argc) {
			argv++;
			argc--;
		}
	} অন্যথা {
		যদি (repo_get_oid(the_repository, "HEAD", &শীর্ষ_oid))
			die(_("could not fetch a revision for HEAD"));
	}

	যদি (files) {
		যদি (cached)
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

	ret = compute_summary_module_list((diff_cmd == DIFF_সূচক) ? &শীর্ষ_oid : ফাঁকা,
					  &info, diff_cmd);
	ফেরত ret;
}

স্থির পূর্ণসংখ্যা module_gitdir(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv, ধ্রুবক অক্ষর *prefix UNUSED,
			 কাঠামো repository *repo)
{
	কাঠামো strbuf gitdir = STRBUF_INIT;

	যদি (argc != 2)
		usage(_("git submodule--helper gitdir <name>"));

	submodule_name_to_gitdir(&gitdir, repo, argv[1]);

	printf("%s\n", gitdir.buf);

	strbuf_release(&gitdir);
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা module_migrate(পূর্ণসংখ্যা argc UNUSED, ধ্রুবক অক্ষর **argv UNUSED,
			  ধ্রুবক অক্ষর *prefix UNUSED, কাঠামো repository *repo)
{
	কাঠামো strbuf module_dir = STRBUF_INIT;
	DIR *dir;
	কাঠামো dirent *de;
	পূর্ণসংখ্যা repo_version = 0;

	repo_git_path_append(repo, &module_dir, "modules/");

	dir = opendir(module_dir.buf);
	যদি (!dir)
		die(_("could not open '%s'"), module_dir.buf);

	যখন ((de = readdir(dir))) {
		কাঠামো strbuf gitdir_path = STRBUF_INIT;
		অক্ষর *চাবি;
		ধ্রুবক অক্ষর *মান;

		যদি (is_dot_or_dotdot(de->d_name))
			চালু;

		strbuf_addf(&gitdir_path, "%s/%s", module_dir.buf, de->d_name);
		যদি (!is_git_directory(gitdir_path.buf)) {
			strbuf_release(&gitdir_path);
			চালু;
		}
		strbuf_release(&gitdir_path);

		চাবি = xstrfmt("submodule.%s.gitdir", de->d_name);
		যদি (!repo_config_get_string_tmp(repo, চাবি, &মান)) {
			/* Already has a gitdir config, nothing to do. */
			মুক্ত(চাবি);
			চালু;
		}
		মুক্ত(চাবি);

		create_default_gitdir_config(de->d_name);
	}

	closedir(dir);
	strbuf_release(&module_dir);

	repo_config_get_int(the_repository, "core.repositoryformatversion", &repo_version);
	যদি (repo_version == 0 &&
	    repo_config_set_gently(repo, "core.repositoryformatversion", "1"))
		die(_("could not set core.repositoryformatversion to 1.\n"
		      "Please set it for migration to work, for example:\n"
		      "git config core.repositoryformatversion 1"));

	যদি (repo_config_set_gently(repo, "extensions.submodulePathConfig", "true"))
		die(_("could not enable submodulePathConfig extension. It is required\n"
		      "for migration to work. Please enable it in the root repo:\n"
		      "git config extensions.submodulePathConfig true"));

	repo->repository_format_submodule_path_cfg = 1;

	ফেরত 0;
}

কাঠামো sync_cb {
	ধ্রুবক অক্ষর *prefix;
	ধ্রুবক অক্ষর *super_prefix;
	unsigned পূর্ণসংখ্যা flags;
};
#define SYNC_CB_INIT { 0 }

স্থির শূন্য sync_submodule(ধ্রুবক অক্ষর *path, ধ্রুবক অক্ষর *prefix,
			   ধ্রুবক অক্ষর *super_prefix, unsigned পূর্ণসংখ্যা flags)
{
	ধ্রুবক কাঠামো submodule *sub;
	অক্ষর *remote_চাবি = ফাঁকা;
	অক্ষর *sub_origin_url, *super_config_url, *displaypath, *default_remote;
	কাঠামো strbuf sb = STRBUF_INIT;
	অক্ষর *sub_config_path = ফাঁকা;
	পূর্ণসংখ্যা code;

	যদি (!is_submodule_active(the_repository, path))
		ফেরত;

	যদি (validate_submodule_path(path) < 0)
		die(ফাঁকা);

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);

	যদি (sub && sub->url) {
		যদি (starts_with_dot_dot_slash(sub->url) ||
		    starts_with_dot_slash(sub->url)) {
			অক্ষর *up_path = get_up_path(path);

			sub_origin_url = resolve_relative_url(sub->url, up_path, 1);
			super_config_url = resolve_relative_url(sub->url, ফাঁকা, 1);
			মুক্ত(up_path);
		} অন্যথা {
			sub_origin_url = xstrdup(sub->url);
			super_config_url = xstrdup(sub->url);
		}
	} অন্যথা {
		sub_origin_url = xstrdup("");
		super_config_url = xstrdup("");
	}

	displaypath = get_submodule_displaypath(path, prefix, super_prefix);

	যদি (!(flags & OPT_QUIET))
		printf(_("Synchronizing submodule url for '%s'\n"),
			 displaypath);

	strbuf_reset(&sb);
	strbuf_addf(&sb, "submodule.%s.url", sub->name);
	যদি (repo_config_set_gently(the_repository, sb.buf, super_config_url))
		die(_("failed to register url for submodule path '%s'"),
		      displaypath);

	যদি (!is_submodule_populated_gently(path, ফাঁকা))
		যাও cleanup;

	strbuf_reset(&sb);
	code = get_default_remote_submodule(path, &default_remote);
	যদি (code)
		exit(code);

	remote_চাবি = xstrfmt("remote.%s.url", default_remote);
	মুক্ত(default_remote);

	submodule_to_gitdir(the_repository, &sb, path);
	strbuf_addstr(&sb, "/config");

	যদি (repo_config_set_in_file_gently(the_repository, sb.buf, remote_চাবি, ফাঁকা, sub_origin_url))
		die(_("failed to update remote for submodule '%s'"),
		      path);

	যদি (flags & OPT_RECURSIVE) {
		কাঠামো child_process cpr = CHILD_PROCESS_INIT;

		cpr.git_cmd = 1;
		cpr.dir = path;
		prepare_submodule_repo_env(&cpr.env);

		strvec_pushl(&cpr.args, "submodule--helper", "sync",
			     "--recursive", ফাঁকা);
		strvec_pushf(&cpr.args, "--super-prefix=%s/", displaypath);

		যদি (flags & OPT_QUIET)
			strvec_ঠেলো(&cpr.args, "--quiet");

		যদি (run_command(&cpr))
			die(_("failed to recurse into submodule '%s'"),
			      path);
	}

cleanup:
	মুক্ত(super_config_url);
	মুক্ত(sub_origin_url);
	strbuf_release(&sb);
	মুক্ত(remote_চাবি);
	মুক্ত(displaypath);
	মুক্ত(sub_config_path);
}

স্থির শূন্য sync_submodule_cb(ধ্রুবক কাঠামো cache_entry *list_item, শূন্য *cb_তথ্য)
{
	কাঠামো sync_cb *info = cb_তথ্য;

	sync_submodule(list_item->name, info->prefix, info->super_prefix,
		       info->flags);
}

স্থির পূর্ণসংখ্যা module_sync(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv, ধ্রুবক অক্ষর *prefix,
		       কাঠামো repository *repo UNUSED)
{
	কাঠামো sync_cb info = SYNC_CB_INIT;
	কাঠামো pathspec pathspec = { 0 };
	কাঠামো module_list list = MODULE_LIST_INIT;
	পূর্ণসংখ্যা quiet = 0;
	পূর্ণসংখ্যা recursive = 0;
	কাঠামো option module_sync_options[] = {
		OPT__SUPER_PREFIX(&info.super_prefix),
		OPT__QUIET(&quiet, N_("suppress output of synchronizing submodule url")),
		OPT_BOOL(0, "recursive", &recursive,
			N_("recurse into nested submodules")),
		OPT_END()
	};
	ধ্রুবক অক্ষর *ধ্রুবক git_submodule_helper_usage[] = {
		N_("git submodule sync [--quiet] [--recursive] [<path>]"),
		ফাঁকা
	};
	পূর্ণসংখ্যা ret = 1;

	argc = parse_options(argc, argv, prefix, module_sync_options,
			     git_submodule_helper_usage, 0);

	যদি (module_list_compute(argv, prefix, &pathspec, &list) < 0)
		যাও cleanup;

	info.prefix = prefix;
	যদি (quiet)
		info.flags |= OPT_QUIET;
	যদি (recursive)
		info.flags |= OPT_RECURSIVE;

	for_each_listed_submodule(&list, sync_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	ফেরত ret;
}

কাঠামো deinit_cb {
	ধ্রুবক অক্ষর *prefix;
	unsigned পূর্ণসংখ্যা flags;
};
#define DEINIT_CB_INIT { 0 }

স্থির শূন্য deinit_submodule(ধ্রুবক অক্ষর *path, ধ্রুবক অক্ষর *prefix,
			     unsigned পূর্ণসংখ্যা flags)
{
	ধ্রুবক কাঠামো submodule *sub;
	অক্ষর *displaypath = ফাঁকা;
	কাঠামো child_process cp_config = CHILD_PROCESS_INIT;
	কাঠামো strbuf sb_config = STRBUF_INIT;
	অক্ষর *sub_git_dir = xstrfmt("%s/.git", path);

	যদি (validate_submodule_path(path) < 0)
		die(ফাঁকা);

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);

	যদি (!sub || !sub->name)
		যাও cleanup;

	displaypath = get_submodule_displaypath(path, prefix, ফাঁকা);

	/* remove the submodule work tree (unless the user already did it) */
	যদি (is_directory(path)) {
		কাঠামো strbuf sb_rm = STRBUF_INIT;
		ধ্রুবক অক্ষর *format;

		যদি (is_directory(sub_git_dir)) {
			যদি (!(flags & OPT_QUIET))
				warning(_("Submodule work tree '%s' contains a .git "
					  "directory. This will be replaced with a "
					  ".git file by using absorbgitdirs."),
					displaypath);

			absorb_git_dir_into_superproject(path, ফাঁকা);

		}

		যদি (!(flags & OPT_FORCE)) {
			কাঠামো child_process cp_rm = CHILD_PROCESS_INIT;

			cp_rm.git_cmd = 1;
			strvec_pushl(&cp_rm.args, "rm", "-qn",
				     path, ফাঁকা);

			যদি (run_command(&cp_rm))
				die(_("Submodule work tree '%s' contains local "
				      "modifications; use '-f' to discard them"),
				      displaypath);
		}

		strbuf_addstr(&sb_rm, path);

		যদি (!remove_dir_recursively(&sb_rm, 0))
			format = _("Cleared directory '%s'\n");
		অন্যথা
			format = _("Could not remove submodule work tree '%s'\n");

		যদি (!(flags & OPT_QUIET))
			printf(format, displaypath);

		submodule_unset_core_worktree(sub);

		strbuf_release(&sb_rm);
	}

	যদি (mkdir(path, 0777))
		printf(_("could not create empty submodule directory %s"),
		      displaypath);

	cp_config.git_cmd = 1;
	strvec_pushl(&cp_config.args, "config", "--get-regexp", ফাঁকা);
	strvec_pushf(&cp_config.args, "submodule.%s\\.", sub->name);

	/* remove the .git/config entries (unless the user already did it) */
	যদি (!capture_command(&cp_config, &sb_config, 0) && sb_config.len) {
		অক্ষর *sub_চাবি = xstrfmt("submodule.%s", sub->name);

		/*
		 * remove the whole section so we have a clean state when
		 * the user later decides to init this submodule again
		 */
		repo_config_rename_section_in_file(the_repository, ফাঁকা, sub_চাবি, ফাঁকা);
		যদি (!(flags & OPT_QUIET))
			printf(_("Submodule '%s' (%s) unregistered for path '%s'\n"),
				 sub->name, sub->url, displaypath);
		মুক্ত(sub_চাবি);
	}

cleanup:
	মুক্ত(displaypath);
	মুক্ত(sub_git_dir);
	strbuf_release(&sb_config);
}

স্থির শূন্য deinit_submodule_cb(ধ্রুবক কাঠামো cache_entry *list_item,
				শূন্য *cb_তথ্য)
{
	কাঠামো deinit_cb *info = cb_তথ্য;
	deinit_submodule(list_item->name, info->prefix, info->flags);
}

স্থির পূর্ণসংখ্যা module_deinit(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv, ধ্রুবক অক্ষর *prefix,
			 কাঠামো repository *repo UNUSED)
{
	কাঠামো deinit_cb info = DEINIT_CB_INIT;
	কাঠামো pathspec pathspec = { 0 };
	কাঠামো module_list list = MODULE_LIST_INIT;
	পূর্ণসংখ্যা quiet = 0;
	পূর্ণসংখ্যা force = 0;
	পূর্ণসংখ্যা all = 0;
	কাঠামো option module_deinit_options[] = {
		OPT__QUIET(&quiet, N_("suppress submodule status output")),
		OPT__FORCE(&force, N_("remove submodule working trees even if they contain local changes"), 0),
		OPT_BOOL(0, "all", &all, N_("unregister all submodules")),
		OPT_END()
	};
	ধ্রুবক অক্ষর *ধ্রুবক git_submodule_helper_usage[] = {
		N_("git submodule deinit [--quiet] [-f | --force] [--all | [--] [<path>...]]"),
		ফাঁকা
	};
	পূর্ণসংখ্যা ret = 1;

	argc = parse_options(argc, argv, prefix, module_deinit_options,
			     git_submodule_helper_usage, 0);

	যদি (all && argc) {
		error("pathspec and --all are incompatible");
		usage_with_options(git_submodule_helper_usage,
				   module_deinit_options);
	}

	যদি (!argc && !all)
		die(_("Use '--all' if you really want to deinitialize all submodules"));

	যদি (module_list_compute(argv, prefix, &pathspec, &list) < 0)
		যাও cleanup;

	info.prefix = prefix;
	যদি (quiet)
		info.flags |= OPT_QUIET;
	যদি (force)
		info.flags |= OPT_FORCE;

	for_each_listed_submodule(&list, deinit_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	ফেরত ret;
}

কাঠামো module_clone_তথ্য {
	ধ্রুবক অক্ষর *prefix;
	ধ্রুবক অক্ষর *path;
	ধ্রুবক অক্ষর *name;
	ধ্রুবক অক্ষর *url;
	পূর্ণসংখ্যা depth;
	কাঠামো list_objects_filter_options *filter_options;
	গণনা ref_storage_format ref_storage_format;
	unsigned পূর্ণসংখ্যা quiet: 1;
	unsigned পূর্ণসংখ্যা progress: 1;
	unsigned পূর্ণসংখ্যা dissociate: 1;
	unsigned পূর্ণসংখ্যা require_init: 1;
	পূর্ণসংখ্যা single_branch;
};
#define MODULE_CLONE_তথ্য_INIT { \
	.single_branch = -1, \
	.ref_storage_format = REF_STORAGE_FORMAT_UNKNOWN, \
}

কাঠামো submodule_alternate_setup {
	ধ্রুবক অক্ষর *submodule_name;
	গণনা SUBMODULE_ALTERNATE_ERROR_MODE {
		SUBMODULE_ALTERNATE_ERROR_DIE,
		SUBMODULE_ALTERNATE_ERROR_INFO,
		SUBMODULE_ALTERNATE_ERROR_IGNORE
	} error_mode;
	কাঠামো string_list *reference;
};
#define SUBMODULE_ALTERNATE_SETUP_INIT { \
	.error_mode = SUBMODULE_ALTERNATE_ERROR_IGNORE, \
}

স্থির ধ্রুবক অক্ষর alternate_error_advice[] = N_(
"An alternate computed from a superproject's alternate is invalid.\n"
"To allow Git to clone without an alternate in such a case, set\n"
"submodule.alternateErrorStrategy to 'info' or, equivalently, clone with\n"
"'--reference-if-able' instead of '--reference'."
);

স্থির পূর্ণসংখ্যা add_possible_reference_from_superproject(
		কাঠামো odb_files_dir *alt_odb, শূন্য *sas_cb)
{
	কাঠামো submodule_alternate_setup *sas = sas_cb;
	আকার_t len;

	/*
	 * If the alternate object store is another repository, try the
	 * standard layout with .git/(modules/<name>)+/objects
	 */
	যদি (strip_suffix(alt_odb->abspath, "/objects", &len)) {
		কাঠামো repository alternate;
		অক্ষর *sm_alternate;
		কাঠামো strbuf sb = STRBUF_INIT;
		কাঠামো strbuf err = STRBUF_INIT;
		strbuf_add(&sb, alt_odb->abspath, len);

		যদি (repo_init(&alternate, sb.buf, ফাঁকা) < 0)
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
		যদি (sm_alternate) {
			অক্ষর *p = strbuf_detach(&sb, ফাঁকা);

			string_list_append(sas->reference, p)->util = p;
			মুক্ত(sm_alternate);
		} অন্যথা {
			নির্বাচন (sas->error_mode) {
			ক্ষেত্র SUBMODULE_ALTERNATE_ERROR_DIE:
				যদি (advice_enabled(ADVICE_SUBMODULE_ALTERNATE_ERROR_STRATEGY_DIE))
					advise(_(alternate_error_advice));
				die(_("submodule '%s' cannot add alternate: %s"),
				    sas->submodule_name, err.buf);
			ক্ষেত্র SUBMODULE_ALTERNATE_ERROR_INFO:
				fprintf_ln(stderr, _("submodule '%s' cannot add alternate: %s"),
					sas->submodule_name, err.buf);
			ক্ষেত্র SUBMODULE_ALTERNATE_ERROR_IGNORE:
				; /* nothing */
			}
		}

		strbuf_release(&err);
		strbuf_release(&sb);
	}

	ফেরত 0;
}

স্থির শূন্য prepare_possible_alternates(ধ্রুবক অক্ষর *sm_name,
		কাঠামো string_list *reference)
{
	অক্ষর *sm_alternate = ফাঁকা, *error_strategy = ফাঁকা;
	কাঠামো submodule_alternate_setup sas = SUBMODULE_ALTERNATE_SETUP_INIT;

	repo_config_get_string(the_repository, "submodule.alternateLocation", &sm_alternate);
	যদি (!sm_alternate)
		ফেরত;

	repo_config_get_string(the_repository, "submodule.alternateErrorStrategy", &error_strategy);

	যদি (!error_strategy)
		error_strategy = xstrdup("die");

	sas.submodule_name = sm_name;
	sas.reference = reference;
	যদি (!strcmp(error_strategy, "die"))
		sas.error_mode = SUBMODULE_ALTERNATE_ERROR_DIE;
	অন্যথা যদি (!strcmp(error_strategy, "info"))
		sas.error_mode = SUBMODULE_ALTERNATE_ERROR_INFO;
	অন্যথা যদি (!strcmp(error_strategy, "ignore"))
		sas.error_mode = SUBMODULE_ALTERNATE_ERROR_IGNORE;
	অন্যথা
		die(_("Value '%s' for submodule.alternateErrorStrategy is not recognized"), error_strategy);

	যদি (!strcmp(sm_alternate, "superproject"))
		odb_for_each_alternate(the_repository->objects,
				       add_possible_reference_from_superproject, &sas);
	অন্যথা যদি (!strcmp(sm_alternate, "no"))
		; /* do nothing */
	অন্যথা
		die(_("Value '%s' for submodule.alternateLocation is not recognized"), sm_alternate);

	মুক্ত(sm_alternate);
	মুক্ত(error_strategy);
}

স্থির অক্ষর *clone_submodule_sm_gitdir(ধ্রুবক অক্ষর *name)
{
	কাঠামো strbuf sb = STRBUF_INIT;
	অক্ষর *sm_gitdir;

	submodule_name_to_gitdir(&sb, the_repository, name);
	sm_gitdir = absolute_pathdup(sb.buf);
	strbuf_release(&sb);

	ফেরত sm_gitdir;
}

স্থির পূর্ণসংখ্যা dir_contains_only_dotgit(ধ্রুবক অক্ষর *path)
{
	DIR *dir = opendir(path);
	কাঠামো dirent *e;
	পূর্ণসংখ্যা ret = 1;

	যদি (!dir)
		ফেরত 0;

	e = readdir_skip_dot_and_dotdot(dir);
	যদি (!e)
		ret = 0;
	অন্যথা যদি (strcmp(DEFAULT_GIT_DIR_ENVIRONMENT, e->d_name) ||
		 (e = readdir_skip_dot_and_dotdot(dir))) {
		error("unexpected item '%s' in '%s'", e->d_name, path);
		ret = 0;
	}

	closedir(dir);
	ফেরত ret;
}

স্থির পূর্ণসংখ্যা clone_submodule(ধ্রুবক কাঠামো module_clone_তথ্য *clone_তথ্য,
			   কাঠামো string_list *reference)
{
	অক্ষর *p;
	অক্ষর *sm_gitdir = clone_submodule_sm_gitdir(clone_তথ্য->name);
	অক্ষর *sm_alternate = ফাঁকা, *error_strategy = ফাঁকা;
	কাঠামো stat st;
	কাঠামো child_process cp = CHILD_PROCESS_INIT;
	ধ্রুবক অক্ষর *clone_তথ্য_path = clone_তথ্য->path;
	অক্ষর *to_free = ফাঁকা;

	যদি (validate_submodule_path(clone_তথ্য_path) < 0)
		die(ফাঁকা);

	যদি (!is_absolute_path(clone_তথ্য->path))
		clone_তথ্য_path = to_free = xstrfmt("%s/%s", repo_get_work_বৃক্ষ(the_repository),
						    clone_তথ্য->path);

	যদি (!file_exists(sm_gitdir)) {
		যদি (clone_তথ্য->require_init && !stat(clone_তথ্য_path, &st) &&
		    !is_empty_dir(clone_তথ্য_path))
			die(_("directory not empty: '%s'"), clone_তথ্য_path);

		যদি (safe_create_leading_directories_const(the_repository, sm_gitdir) < 0)
			die(_("could not create directory '%s'"), sm_gitdir);

		prepare_possible_alternates(clone_তথ্য->name, reference);

		strvec_ঠেলো(&cp.args, "clone");
		strvec_ঠেলো(&cp.args, "--no-checkout");
		যদি (clone_তথ্য->quiet)
			strvec_ঠেলো(&cp.args, "--quiet");
		যদি (clone_তথ্য->progress)
			strvec_ঠেলো(&cp.args, "--progress");
		যদি (clone_তথ্য->depth > 0)
			strvec_pushf(&cp.args, "--depth=%d", clone_তথ্য->depth);
		যদি (reference->nr) {
			কাঠামো string_list_item *item;

			for_each_string_list_item(item, reference)
				strvec_pushl(&cp.args, "--reference",
					     item->string, ফাঁকা);
		}
		যদি (clone_তথ্য->ref_storage_format != REF_STORAGE_FORMAT_UNKNOWN)
			strvec_pushf(&cp.args, "--ref-storage-format=%s",
				     ref_storage_format_to_name(clone_তথ্য->ref_storage_format));
		যদি (clone_তথ্য->dissociate)
			strvec_ঠেলো(&cp.args, "--dissociate");
		যদি (sm_gitdir && *sm_gitdir)
			strvec_pushl(&cp.args, "--separate-git-dir", sm_gitdir, ফাঁকা);
		যদি (clone_তথ্য->filter_options && clone_তথ্য->filter_options->choice)
			strvec_pushf(&cp.args, "--filter=%s",
				     expand_list_objects_filter_spec(
					     clone_তথ্য->filter_options));
		যদি (clone_তথ্য->single_branch >= 0)
			strvec_ঠেলো(&cp.args, clone_তথ্য->single_branch ?
				    "--single-branch" :
				    "--no-single-branch");

		strvec_ঠেলো(&cp.args, "--");
		strvec_ঠেলো(&cp.args, clone_তথ্য->url);
		strvec_ঠেলো(&cp.args, clone_তথ্য_path);

		cp.git_cmd = 1;
		prepare_submodule_repo_env(&cp.env);
		cp.no_stdin = 1;

		যদি(run_command(&cp))
			die(_("clone of '%s' into submodule path '%s' failed"),
			    clone_তথ্য->url, clone_তথ্য_path);

		যদি (clone_তথ্য->require_init && !stat(clone_তথ্য_path, &st) &&
		    !dir_contains_only_dotgit(clone_তথ্য_path)) {
			অক্ষর *dot_git = xstrfmt("%s/.git", clone_তথ্য_path);
			unlink(dot_git);
			মুক্ত(dot_git);
			die(_("directory not empty: '%s'"), clone_তথ্য_path);
		}
	} অন্যথা {
		অক্ষর *path;

		যদি (clone_তথ্য->require_init && !stat(clone_তথ্য_path, &st) &&
		    !is_empty_dir(clone_তথ্য_path))
			die(_("directory not empty: '%s'"), clone_তথ্য_path);
		যদি (safe_create_leading_directories_const(the_repository, clone_তথ্য_path) < 0)
			die(_("could not create directory '%s'"), clone_তথ্য_path);
		path = xstrfmt("%s/index", sm_gitdir);
		unlink_or_warn(path);
		মুক্ত(path);
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
	যদি (validate_submodule_git_dir(sm_gitdir, clone_তথ্য->name) < 0) {
		অক্ষর *শীর্ষ = xstrfmt("%s/HEAD", sm_gitdir);
		unlink(শীর্ষ);
		মুক্ত(শীর্ষ);
		die(_("refusing to create/use '%s' in another submodule's git dir. "
		      "Enabling extensions.submodulePathConfig should fix this."),
		    sm_gitdir);
	}

	connect_work_বৃক্ষ_and_git_dir(clone_তথ্য_path, sm_gitdir, 0);

	p = repo_submodule_path(the_repository, clone_তথ্য_path, "config");
	যদি (!p)
		die(_("could not get submodule directory for '%s'"), clone_তথ্য_path);

	/* setup alternateLocation and alternateErrorStrategy in the cloned submodule if needed */
	repo_config_get_string(the_repository, "submodule.alternateLocation", &sm_alternate);
	যদি (sm_alternate)
		repo_config_set_in_file(the_repository, p, "submodule.alternateLocation",
					sm_alternate);
	repo_config_get_string(the_repository, "submodule.alternateErrorStrategy", &error_strategy);
	যদি (error_strategy)
		repo_config_set_in_file(the_repository, p, "submodule.alternateErrorStrategy",
					error_strategy);

	মুক্ত(sm_alternate);
	মুক্ত(error_strategy);

	মুক্ত(sm_gitdir);
	মুক্ত(p);
	মুক্ত(to_free);
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা module_clone(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv, ধ্রুবক অক্ষর *prefix,
			কাঠামো repository *repo UNUSED)
{
	পূর্ণসংখ্যা dissociate = 0, quiet = 0, progress = 0, require_init = 0;
	কাঠামো module_clone_তথ্য clone_তথ্য = MODULE_CLONE_তথ্য_INIT;
	কাঠামো string_list reference = STRING_LIST_INIT_NODUP;
	কাঠামো list_objects_filter_options filter_options =
		LIST_OBJECTS_FILTER_INIT;
	ধ্রুবক অক্ষর *ref_storage_format = ফাঁকা;

	কাঠামো option module_clone_options[] = {
		OPT_STRING(0, "prefix", &clone_তথ্য.prefix,
			   N_("path"),
			   N_("alternative anchor for relative paths")),
		OPT_STRING(0, "path", &clone_তথ্য.path,
			   N_("path"),
			   N_("where the new submodule will be cloned to")),
		OPT_STRING(0, "name", &clone_তথ্য.name,
			   N_("string"),
			   N_("name of the new submodule")),
		OPT_STRING(0, "url", &clone_তথ্য.url,
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
		OPT_INTEGER(0, "depth", &clone_তথ্য.depth,
			   N_("depth for shallow clones")),
		OPT__QUIET(&quiet, "suppress output for cloning a submodule"),
		OPT_BOOL(0, "progress", &progress,
			   N_("force cloning progress")),
		OPT_BOOL(0, "require-init", &require_init,
			   N_("disallow cloning into non-empty directory")),
		OPT_BOOL(0, "single-branch", &clone_তথ্য.single_branch,
			 N_("clone only one branch, HEAD or --branch")),
		OPT_PARSE_LIST_OBJECTS_FILTER(&filter_options),
		OPT_END()
	};
	ধ্রুবক অক্ষর *ধ্রুবক git_submodule_helper_usage[] = {
		N_("git submodule--helper clone [--prefix=<path>] [--quiet] "
		   "[--reference <repository>] [--name <name>] [--depth <depth>] "
		   "[--single-branch] [--filter <filter-spec>] "
		   "--url <url> --path <path>"),
		ফাঁকা
	};

	argc = parse_options(argc, argv, prefix, module_clone_options,
			     git_submodule_helper_usage, 0);

	যদি (ref_storage_format) {
		clone_তথ্য.ref_storage_format = ref_storage_format_by_name(ref_storage_format);
		যদি (clone_তথ্য.ref_storage_format == REF_STORAGE_FORMAT_UNKNOWN)
			die(_("unknown ref storage format '%s'"), ref_storage_format);
	}
	clone_তথ্য.dissociate = !!dissociate;
	clone_তথ্য.quiet = !!quiet;
	clone_তথ্য.progress = !!progress;
	clone_তথ্য.require_init = !!require_init;
	clone_তথ্য.filter_options = &filter_options;

	যদি (argc || !clone_তথ্য.url || !clone_তথ্য.path || !*(clone_তথ্য.path))
		usage_with_options(git_submodule_helper_usage,
				   module_clone_options);

	clone_submodule(&clone_তথ্য, &reference);
	list_objects_filter_release(&filter_options);
	string_list_clear(&reference, 1);
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা determine_submodule_update_strategy(কাঠামো repository *r,
					       পূর্ণসংখ্যা just_cloned,
					       ধ্রুবক অক্ষর *path,
					       গণনা submodule_update_type update,
					       কাঠামো submodule_update_strategy *out)
{
	ধ্রুবক কাঠামো submodule *sub = submodule_from_path(r, null_oid(the_hash_algo), path);
	অক্ষর *চাবি;
	ধ্রুবক অক্ষর *val;
	পূর্ণসংখ্যা ret;

	/*
	 * NEEDSWORK: audit and ensure that update_submodule() has right
	 * to assume that submodule_from_path() above will always succeed.
	 */
	যদি (!sub)
		BUG("update_submodule assumes a submodule exists at path (%s)",
		    path);
	চাবি = xstrfmt("submodule.%s.update", sub->name);

	যদি (update) {
		out->type = update;
	} অন্যথা যদি (!repo_config_get_string_tmp(r, চাবি, &val)) {
		যদি (parse_submodule_update_strategy(val, out) < 0) {
			ret = die_message(_("Invalid update mode '%s' configured for submodule path '%s'"),
					  val, path);
			যাও cleanup;
		}
	} অন্যথা যদি (sub->update_strategy.type != SM_UPDATE_UNSPECIFIED) {
		যদি (sub->update_strategy.type == SM_UPDATE_COMMAND)
			BUG("how did we read update = !command from .gitmodules?");
		out->type = sub->update_strategy.type;
		out->command = sub->update_strategy.command;
	} অন্যথা
		out->type = SM_UPDATE_CHECKOUT;

	যদি (just_cloned &&
	    (out->type == SM_UPDATE_MERGE ||
	     out->type == SM_UPDATE_REBASE ||
	     out->type == SM_UPDATE_NONE))
		out->type = SM_UPDATE_CHECKOUT;

	ret = 0;
cleanup:
	মুক্ত(চাবি);
	ফেরত ret;
}

কাঠামো update_clone_তথ্য {
	ধ্রুবক কাঠামো submodule *sub;
	কাঠামো object_id oid;
	unsigned just_cloned;
};

কাঠামো submodule_update_clone {
	/* index into 'update_data.list', the list of submodules to look into for cloning */
	পূর্ণসংখ্যা current;

	/* configuration parameters which are passed on to the children */
	ধ্রুবক কাঠামো update_তথ্য *update_তথ্য;

	/* to be consumed by update_submodule() */
	কাঠামো update_clone_তথ্য *update_clone;
	পূর্ণসংখ্যা update_clone_nr; পূর্ণসংখ্যা update_clone_alloc;

	/* If we want to stop as fast as possible and return an error */
	unsigned quickstop : 1;

	/* failed clones to be retried again */
	ধ্রুবক কাঠামো cache_entry **failed_clones;
	পূর্ণসংখ্যা failed_clones_nr, failed_clones_alloc;
};
#define SUBMODULE_UPDATE_CLONE_INIT { 0 }

স্থির শূন্য submodule_update_clone_release(কাঠামো submodule_update_clone *suc)
{
	মুক্ত(suc->update_clone);
	মুক্ত(suc->failed_clones);
}

কাঠামো update_তথ্য {
	ধ্রুবক অক্ষর *prefix;
	ধ্রুবক অক্ষর *super_prefix;
	অক্ষর *displaypath;
	গণনা submodule_update_type update_default;
	কাঠামো object_id suboid;
	কাঠামো string_list references;
	কাঠামো submodule_update_strategy update_strategy;
	কাঠামো list_objects_filter_options *filter_options;
	কাঠামো module_list list;
	গণনা ref_storage_format ref_storage_format;
	পূর্ণসংখ্যা depth;
	পূর্ণসংখ্যা max_jobs;
	পূর্ণসংখ্যা single_branch;
	পূর্ণসংখ্যা recommend_shallow;
	unsigned পূর্ণসংখ্যা require_init;
	unsigned পূর্ণসংখ্যা force;
	unsigned পূর্ণসংখ্যা quiet;
	unsigned পূর্ণসংখ্যা nofetch;
	unsigned পূর্ণসংখ্যা remote;
	unsigned পূর্ণসংখ্যা progress;
	unsigned পূর্ণসংখ্যা dissociate;
	unsigned পূর্ণসংখ্যা init;
	unsigned পূর্ণসংখ্যা warn_if_uninitialized;
	unsigned পূর্ণসংখ্যা recursive;

	/* copied over from update_clone_data */
	কাঠামো object_id oid;
	unsigned পূর্ণসংখ্যা just_cloned;
	ধ্রুবক অক্ষর *sm_path;
};
#define UPDATE_তথ্য_INIT { \
	.update_strategy = SUBMODULE_UPDATE_STRATEGY_INIT, \
	.list = MODULE_LIST_INIT, \
	.ref_storage_format = REF_STORAGE_FORMAT_UNKNOWN, \
	.recommend_shallow = -1, \
	.references = STRING_LIST_INIT_DUP, \
	.single_branch = -1, \
	.max_jobs = 1, \
}

স্থির শূন্য update_তথ্য_release(কাঠামো update_তথ্য *ud)
{
	মুক্ত(ud->displaypath);
	submodule_update_strategy_release(&ud->update_strategy);
	module_list_release(&ud->list);
}

স্থির শূন্য পরবর্তী_submodule_warn_missing(কাঠামো submodule_update_clone *suc,
		কাঠামো strbuf *out, ধ্রুবক অক্ষর *displaypath)
{
	/*
	 * Only mention uninitialized submodules when their
	 * paths have been specified.
	 */
	যদি (suc->update_তথ্য->warn_if_uninitialized) {
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
স্থির পূর্ণসংখ্যা prepare_to_clone_পরবর্তী_submodule(ধ্রুবক কাঠামো cache_entry *ce,
					   কাঠামো child_process *child,
					   কাঠামো submodule_update_clone *suc,
					   কাঠামো strbuf *out)
{
	ধ্রুবক কাঠামো submodule *sub = ফাঁকা;
	ধ্রুবক অক্ষর *url = ফাঁকা;
	ধ্রুবক অক্ষর *update_string;
	গণনা submodule_update_type update_type;
	অক্ষর *চাবি;
	ধ্রুবক কাঠামো update_তথ্য *ud = suc->update_তথ্য;
	অক্ষর *displaypath = get_submodule_displaypath(ce->name, ud->prefix,
						      ud->super_prefix);
	কাঠামো strbuf sb = STRBUF_INIT;
	পূর্ণসংখ্যা needs_cloning = 0;
	পূর্ণসংখ্যা need_free_url = 0;

	যদি (ce_stage(ce)) {
		strbuf_addf(out, _("Skipping unmerged submodule %s"), displaypath);
		strbuf_addch(out, '\n');
		যাও cleanup;
	}

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), ce->name);

	যদি (!sub) {
		পরবর্তী_submodule_warn_missing(suc, out, displaypath);
		যাও cleanup;
	}

	চাবি = xstrfmt("submodule.%s.update", sub->name);
	যদি (!repo_config_get_string_tmp(the_repository, চাবি, &update_string)) {
		update_type = parse_submodule_update_type(update_string);
	} অন্যথা {
		update_type = sub->update_strategy.type;
	}
	মুক্ত(চাবি);

	যদি (suc->update_তথ্য->update_strategy.type == SM_UPDATE_NONE
	    || (suc->update_তথ্য->update_strategy.type == SM_UPDATE_UNSPECIFIED
		&& update_type == SM_UPDATE_NONE)) {
		strbuf_addf(out, _("Skipping submodule '%s'"), displaypath);
		strbuf_addch(out, '\n');
		যাও cleanup;
	}

	/* Check if the submodule has been initialized. */
	যদি (!is_submodule_active(the_repository, ce->name)) {
		পরবর্তী_submodule_warn_missing(suc, out, displaypath);
		যাও cleanup;
	}

	strbuf_reset(&sb);
	strbuf_addf(&sb, "submodule.%s.url", sub->name);
	যদি (repo_config_get_string_tmp(the_repository, sb.buf, &url)) {
		যদি (sub->url && (starts_with_dot_slash(sub->url) ||
				 starts_with_dot_dot_slash(sub->url))) {
			url = resolve_relative_url(sub->url, ফাঁকা, 0);
			need_free_url = 1;
		} অন্যথা
			url = sub->url;
	}

	যদি (!url)
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

	যদি (!needs_cloning)
		যাও cleanup;

	child->git_cmd = 1;
	child->no_stdin = 1;
	child->stdout_to_stderr = 1;
	child->err = -1;
	strvec_ঠেলো(&child->args, "submodule--helper");
	strvec_ঠেলো(&child->args, "clone");
	যদি (suc->update_তথ্য->progress)
		strvec_ঠেলো(&child->args, "--progress");
	যদি (suc->update_তথ্য->quiet)
		strvec_ঠেলো(&child->args, "--quiet");
	যদি (suc->update_তথ্য->prefix)
		strvec_pushl(&child->args, "--prefix", suc->update_তথ্য->prefix, ফাঁকা);
	যদি (suc->update_তথ্য->recommend_shallow && sub->recommend_shallow == 1)
		strvec_ঠেলো(&child->args, "--depth=1");
	অন্যথা যদি (suc->update_তথ্য->depth)
		strvec_pushf(&child->args, "--depth=%d", suc->update_তথ্য->depth);
	যদি (suc->update_তথ্য->filter_options && suc->update_তথ্য->filter_options->choice)
		strvec_pushf(&child->args, "--filter=%s",
			     expand_list_objects_filter_spec(suc->update_তথ্য->filter_options));
	যদি (suc->update_তথ্য->require_init)
		strvec_ঠেলো(&child->args, "--require-init");
	যদি (suc->update_তথ্য->ref_storage_format != REF_STORAGE_FORMAT_UNKNOWN)
		strvec_pushf(&child->args, "--ref-storage-format=%s",
			     ref_storage_format_to_name(suc->update_তথ্য->ref_storage_format));
	strvec_pushl(&child->args, "--path", sub->path, ফাঁকা);
	strvec_pushl(&child->args, "--name", sub->name, ফাঁকা);
	strvec_pushl(&child->args, "--url", url, ফাঁকা);
	যদি (suc->update_তথ্য->references.nr) {
		কাঠামো string_list_item *item;

		for_each_string_list_item(item, &suc->update_তথ্য->references)
			strvec_pushl(&child->args, "--reference", item->string, ফাঁকা);
	}
	যদি (suc->update_তথ্য->dissociate)
		strvec_ঠেলো(&child->args, "--dissociate");
	যদি (suc->update_তথ্য->single_branch >= 0)
		strvec_ঠেলো(&child->args, suc->update_তথ্য->single_branch ?
					      "--single-branch" :
					      "--no-single-branch");

cleanup:
	মুক্ত(displaypath);
	strbuf_release(&sb);
	যদি (need_free_url)
		মুক্ত((শূন্য*)url);

	ফেরত needs_cloning;
}

স্থির পূর্ণসংখ্যা update_clone_get_পরবর্তী_task(কাঠামো child_process *child,
				      কাঠামো strbuf *err,
				      শূন্য *suc_cb,
				      শূন্য **idx_task_cb)
{
	কাঠামো submodule_update_clone *suc = suc_cb;
	ধ্রুবক কাঠামো cache_entry *ce;
	পূর্ণসংখ্যা সূচক;

	জন্য (; suc->current < suc->update_তথ্য->list.nr; suc->current++) {
		ce = suc->update_তথ্য->list.entries[suc->current];
		যদি (prepare_to_clone_পরবর্তী_submodule(ce, child, suc, err)) {
			পূর্ণসংখ্যা *p = xmalloc(আকার(*p));

			*p = suc->current;
			*idx_task_cb = p;
			suc->current++;
			ফেরত 1;
		}
	}

	/*
	 * The loop above tried cloning each submodule once, now try the
	 * stragglers again, which we can imagine as an extension of the
	 * entry list.
	 */
	সূচক = suc->current - suc->update_তথ্য->list.nr;
	যদি (সূচক < suc->failed_clones_nr) {
		পূর্ণসংখ্যা *p;

		ce = suc->failed_clones[সূচক];
		যদি (!prepare_to_clone_পরবর্তী_submodule(ce, child, suc, err)) {
			suc->current ++;
			strbuf_addstr(err, "BUG: submodule considered for "
					   "cloning, doesn't need cloning "
					   "any more?\n");
			ফেরত 0;
		}
		p = xmalloc(আকার(*p));
		*p = suc->current;
		*idx_task_cb = p;
		suc->current ++;
		ফেরত 1;
	}

	ফেরত 0;
}

স্থির পূর্ণসংখ্যা update_clone_start_failure(কাঠামো strbuf *err UNUSED,
				      শূন্য *suc_cb,
				      শূন্য *idx_task_cb UNUSED)
{
	কাঠামো submodule_update_clone *suc = suc_cb;

	suc->quickstop = 1;
	ফেরত 1;
}

স্থির পূর্ণসংখ্যা update_clone_task_finished(পূর্ণসংখ্যা ফলাফল,
				      কাঠামো strbuf *err,
				      শূন্য *suc_cb,
				      শূন্য *idx_task_cb)
{
	ধ্রুবক কাঠামো cache_entry *ce;
	কাঠামো submodule_update_clone *suc = suc_cb;
	পূর্ণসংখ্যা *idxP = idx_task_cb;
	পূর্ণসংখ্যা idx = *idxP;

	মুক্ত(idxP);

	যদি (!ফলাফল)
		ফেরত 0;

	যদি (idx < suc->update_তথ্য->list.nr) {
		ce  = suc->update_তথ্য->list.entries[idx];
		strbuf_addf(err, _("Failed to clone '%s'. Retry scheduled"),
			    ce->name);
		strbuf_addch(err, '\n');
		ALLOC_GROW(suc->failed_clones,
			   suc->failed_clones_nr + 1,
			   suc->failed_clones_alloc);
		suc->failed_clones[suc->failed_clones_nr++] = ce;
		ফেরত 0;
	} অন্যথা {
		idx -= suc->update_তথ্য->list.nr;
		ce  = suc->failed_clones[idx];
		strbuf_addf(err, _("Failed to clone '%s' a second time, aborting"),
			    ce->name);
		strbuf_addch(err, '\n');
		suc->quickstop = 1;
		ফেরত 1;
	}

	ফেরত 0;
}

স্থির পূর্ণসংখ্যা git_update_clone_config(ধ্রুবক অক্ষর *var, ধ্রুবক অক্ষর *মান,
				   ধ্রুবক কাঠামো config_context *ctx,
				   শূন্য *cb)
{
	পূর্ণসংখ্যা *max_jobs = cb;

	যদি (!strcmp(var, "submodule.fetchjobs"))
		*max_jobs = parse_submodule_fetchjobs(var, মান, ctx->kvi);
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা is_tip_reachable(ধ্রুবক অক্ষর *path, ধ্রুবক কাঠামো object_id *oid)
{
	কাঠামো child_process cp = CHILD_PROCESS_INIT;
	কাঠামো strbuf rev = STRBUF_INIT;
	অক্ষর *hex = oid_to_hex(oid);
	পূর্ণসংখ্যা reachable;

	cp.git_cmd = 1;
	cp.dir = path;
	cp.no_stderr = 1;
	strvec_pushl(&cp.args, "rev-list", "-n", "1", hex, "--not", "--all", ফাঁকা);

	prepare_submodule_repo_env(&cp.env);

	যদি (capture_command(&cp, &rev, GIT_MAX_HEXSZ + 1) || rev.len)
		reachable = 0;
	অন্যথা
		reachable = 1;

	strbuf_release(&rev);
	ফেরত reachable;
}

স্থির পূর্ণসংখ্যা fetch_in_submodule(ধ্রুবক অক্ষর *module_path, পূর্ণসংখ্যা depth, পূর্ণসংখ্যা quiet,
			      ধ্রুবক কাঠামো object_id *oid)
{
	কাঠামো child_process cp = CHILD_PROCESS_INIT;

	prepare_submodule_repo_env(&cp.env);
	cp.git_cmd = 1;
	cp.dir = module_path;

	strvec_ঠেলো(&cp.args, "fetch");
	যদি (quiet)
		strvec_ঠেলো(&cp.args, "--quiet");
	যদি (depth)
		strvec_pushf(&cp.args, "--depth=%d", depth);
	যদি (oid) {
		অক্ষর *hex = oid_to_hex(oid);
		অক্ষর *remote;
		পূর্ণসংখ্যা code;

		code = get_default_remote_submodule(module_path, &remote);
		যদি (code) {
			child_process_clear(&cp);
			ফেরত code;
		}

		strvec_pushl(&cp.args, remote, hex, ফাঁকা);
		মুক্ত(remote);
	}

	ফেরত run_command(&cp);
}

স্থির পূর্ণসংখ্যা run_update_command(ধ্রুবক কাঠামো update_তথ্য *ud, পূর্ণসংখ্যা subforce)
{
	কাঠামো child_process cp = CHILD_PROCESS_INIT;
	অক্ষর *oid = oid_to_hex(&ud->oid);
	পূর্ণসংখ্যা ret;

	নির্বাচন (ud->update_strategy.type) {
	ক্ষেত্র SM_UPDATE_CHECKOUT:
		cp.git_cmd = 1;
		strvec_pushl(&cp.args, "checkout", "-q", ফাঁকা);
		যদি (subforce)
			strvec_ঠেলো(&cp.args, "-f");
		বিরতি;
	ক্ষেত্র SM_UPDATE_REBASE:
		cp.git_cmd = 1;
		strvec_ঠেলো(&cp.args, "rebase");
		যদি (ud->quiet)
			strvec_ঠেলো(&cp.args, "--quiet");
		বিরতি;
	ক্ষেত্র SM_UPDATE_MERGE:
		cp.git_cmd = 1;
		strvec_ঠেলো(&cp.args, "merge");
		যদি (ud->quiet)
			strvec_ঠেলো(&cp.args, "--quiet");
		বিরতি;
	ক্ষেত্র SM_UPDATE_COMMAND:
		cp.use_shell = 1;
		strvec_ঠেলো(&cp.args, ud->update_strategy.command);
		বিরতি;
	পূর্বনির্ধারিত:
		BUG("unexpected update strategy type: %d",
		    ud->update_strategy.type);
	}
	strvec_ঠেলো(&cp.args, oid);

	cp.dir = ud->sm_path;
	prepare_submodule_repo_env(&cp.env);
	যদি ((ret = run_command(&cp))) {
		নির্বাচন (ud->update_strategy.type) {
		ক্ষেত্র SM_UPDATE_CHECKOUT:
			die_message(_("Unable to checkout '%s' in submodule path '%s'"),
				    oid, ud->displaypath);
			/* No "ret" assignment, use "git checkout"'s */
			বিরতি;
		ক্ষেত্র SM_UPDATE_REBASE:
			ret = die_message(_("Unable to rebase '%s' in submodule path '%s'"),
					  oid, ud->displaypath);
			বিরতি;
		ক্ষেত্র SM_UPDATE_MERGE:
			ret = die_message(_("Unable to merge '%s' in submodule path '%s'"),
					  oid, ud->displaypath);
			বিরতি;
		ক্ষেত্র SM_UPDATE_COMMAND:
			ret = die_message(_("Execution of '%s %s' failed in submodule path '%s'"),
					  ud->update_strategy.command, oid, ud->displaypath);
			বিরতি;
		পূর্বনির্ধারিত:
			BUG("unexpected update strategy type: %d",
			    ud->update_strategy.type);
		}

		ফেরত ret;
	}

	যদি (ud->quiet)
		ফেরত 0;

	নির্বাচন (ud->update_strategy.type) {
	ক্ষেত্র SM_UPDATE_CHECKOUT:
		printf(_("Submodule path '%s': checked out '%s'\n"),
		       ud->displaypath, oid);
		বিরতি;
	ক্ষেত্র SM_UPDATE_REBASE:
		printf(_("Submodule path '%s': rebased into '%s'\n"),
		       ud->displaypath, oid);
		বিরতি;
	ক্ষেত্র SM_UPDATE_MERGE:
		printf(_("Submodule path '%s': merged in '%s'\n"),
		       ud->displaypath, oid);
		বিরতি;
	ক্ষেত্র SM_UPDATE_COMMAND:
		printf(_("Submodule path '%s': '%s %s'\n"),
		       ud->displaypath, ud->update_strategy.command, oid);
		বিরতি;
	পূর্বনির্ধারিত:
		BUG("unexpected update strategy type: %d",
		    ud->update_strategy.type);
	}

	ফেরত 0;
}

স্থির পূর্ণসংখ্যা run_update_procedure(ধ্রুবক কাঠামো update_তথ্য *ud)
{
	পূর্ণসংখ্যা subforce = is_null_oid(&ud->suboid) || ud->force;

	যদি (!ud->nofetch) {
		/*
		 * Run fetch only if `oid` isn't present or it
		 * is not reachable from a ref.
		 */
		যদি (!is_tip_reachable(ud->sm_path, &ud->oid) &&
		    fetch_in_submodule(ud->sm_path, ud->depth, ud->quiet, ফাঁকা) &&
		    !ud->quiet)
			fprintf_ln(stderr,
				   _("Unable to fetch in submodule path '%s'; "
				     "trying to directly fetch %s:"),
				   ud->displaypath, oid_to_hex(&ud->oid));
		/*
		 * Now we tried the usual fetch, but `oid` may
		 * not be reachable from any of the refs.
		 */
		যদি (!is_tip_reachable(ud->sm_path, &ud->oid) &&
		    fetch_in_submodule(ud->sm_path, ud->depth, ud->quiet, &ud->oid))
			ফেরত die_message(_("Fetched in submodule path '%s', but it did not "
					     "contain %s. Direct fetching of that commit failed."),
					   ud->displaypath, oid_to_hex(&ud->oid));
	}

	ফেরত run_update_command(ud, subforce);
}

স্থির পূর্ণসংখ্যা remote_submodule_branch(ধ্রুবক অক্ষর *path, ধ্রুবক অক্ষর **branch)
{
	ধ্রুবক কাঠামো submodule *sub;
	অক্ষর *চাবি;
	*branch = ফাঁকা;

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);
	যদি (!sub)
		ফেরত die_message(_("could not initialize submodule at path '%s'"),
				   path);

	চাবি = xstrfmt("submodule.%s.branch", sub->name);
	যদি (repo_config_get_string_tmp(the_repository, চাবি, branch))
		*branch = sub->branch;
	মুক্ত(চাবি);

	যদি (!*branch) {
		*branch = "HEAD";
		ফেরত 0;
	}

	যদি (!strcmp(*branch, ".")) {
		ধ্রুবক অক্ষর *refname = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
							      "HEAD", 0, ফাঁকা,
							      ফাঁকা);

		যদি (!refname)
			ফেরত die_message(_("No such ref: %s"), "HEAD");

		/* detached HEAD */
		যদি (!strcmp(refname, "HEAD"))
			ফেরত die_message(_("Submodule (%s) branch configured to inherit "
					     "branch from superproject, but the superproject "
					     "is not on any branch"), sub->name);

		যদি (!skip_prefix(refname, "refs/heads/", &refname))
			ফেরত die_message(_("Expecting a full ref name, got %s"),
					   refname);

		*branch = refname;
		ফেরত 0;
	}

	/* Our "branch" is coming from repo_config_get_string_tmp() */
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা ensure_core_worktree(ধ্রুবক অক্ষর *path)
{
	ধ্রুবক অক্ষর *cw;
	কাঠামো repository subrepo;

	যদি (repo_submodule_init(&subrepo, the_repository, path, null_oid(the_hash_algo)))
		ফেরত die_message(_("could not get a repository handle for submodule '%s'"),
				   path);

	যদি (!repo_config_get_string_tmp(&subrepo, "core.worktree", &cw)) {
		অক্ষর *cfg_file, *abs_path;
		ধ্রুবক অক্ষর *rel_path;
		কাঠামো strbuf sb = STRBUF_INIT;

		cfg_file = repo_git_path(&subrepo, "config");

		abs_path = absolute_pathdup(path);
		rel_path = relative_path(abs_path, subrepo.gitdir, &sb);

		repo_config_set_in_file(the_repository, cfg_file, "core.worktree", rel_path);

		মুক্ত(cfg_file);
		মুক্ত(abs_path);
		strbuf_release(&sb);
	}

	repo_clear(&subrepo);
	ফেরত 0;
}

স্থির ধ্রুবক অক্ষর *submodule_update_type_to_label(গণনা submodule_update_type type)
{
	নির্বাচন (type) {
	ক্ষেত্র SM_UPDATE_CHECKOUT:
		ফেরত "checkout";
	ক্ষেত্র SM_UPDATE_MERGE:
		ফেরত "merge";
	ক্ষেত্র SM_UPDATE_REBASE:
		ফেরত "rebase";
	ক্ষেত্র SM_UPDATE_UNSPECIFIED:
	ক্ষেত্র SM_UPDATE_NONE:
	ক্ষেত্র SM_UPDATE_COMMAND:
		বিরতি;
	}
	BUG("unreachable with type %d", type);
}

স্থির শূন্য update_তথ্য_to_args(ধ্রুবক কাঠামো update_তথ্য *update_তথ্য,
				কাঠামো strvec *args)
{
	গণনা submodule_update_type update_type = update_তথ্য->update_default;

	strvec_pushl(args, "submodule--helper", "update", "--recursive", ফাঁকা);
	যদি (update_তথ্য->displaypath)
		strvec_pushf(args, "--super-prefix=%s/",
			     update_তথ্য->displaypath);
	strvec_pushf(args, "--jobs=%d", update_তথ্য->max_jobs);
	যদি (update_তথ্য->quiet)
		strvec_ঠেলো(args, "--quiet");
	যদি (update_তথ্য->force)
		strvec_ঠেলো(args, "--force");
	যদি (update_তথ্য->init)
		strvec_ঠেলো(args, "--init");
	যদি (update_তথ্য->remote)
		strvec_ঠেলো(args, "--remote");
	যদি (update_তথ্য->nofetch)
		strvec_ঠেলো(args, "--no-fetch");
	যদি (update_তথ্য->dissociate)
		strvec_ঠেলো(args, "--dissociate");
	যদি (update_তথ্য->progress)
		strvec_ঠেলো(args, "--progress");
	যদি (update_তথ্য->require_init)
		strvec_ঠেলো(args, "--require-init");
	যদি (update_তথ্য->depth)
		strvec_pushf(args, "--depth=%d", update_তথ্য->depth);
	যদি (update_type != SM_UPDATE_UNSPECIFIED)
		strvec_pushf(args, "--%s",
			     submodule_update_type_to_label(update_type));

	যদি (update_তথ্য->references.nr) {
		কাঠামো string_list_item *item;

		for_each_string_list_item(item, &update_তথ্য->references)
			strvec_pushl(args, "--reference", item->string, ফাঁকা);
	}
	যদি (update_তথ্য->ref_storage_format != REF_STORAGE_FORMAT_UNKNOWN)
		strvec_pushf(args, "--ref-storage-format=%s",
			     ref_storage_format_to_name(update_তথ্য->ref_storage_format));
	যদি (update_তথ্য->filter_options && update_তথ্য->filter_options->choice)
		strvec_pushf(args, "--filter=%s",
				expand_list_objects_filter_spec(
					update_তথ্য->filter_options));
	যদি (update_তথ্য->recommend_shallow == 0)
		strvec_ঠেলো(args, "--no-recommend-shallow");
	অন্যথা যদি (update_তথ্য->recommend_shallow == 1)
		strvec_ঠেলো(args, "--recommend-shallow");
	যদি (update_তথ্য->single_branch >= 0)
		strvec_ঠেলো(args, update_তথ্য->single_branch ?
				    "--single-branch" :
				    "--no-single-branch");
}

স্থির পূর্ণসংখ্যা update_submodule(কাঠামো update_তথ্য *update_তথ্য)
{
	পূর্ণসংখ্যা ret;

	যদি (validate_submodule_path(update_তথ্য->sm_path) < 0)
		ফেরত -1;

	ret = determine_submodule_update_strategy(the_repository,
						  update_তথ্য->just_cloned,
						  update_তথ্য->sm_path,
						  update_তথ্য->update_default,
						  &update_তথ্য->update_strategy);
	যদি (ret)
		ফেরত ret;

	যদি (update_তথ্য->just_cloned)
		oidcpy(&update_তথ্য->suboid, null_oid(the_hash_algo));
	অন্যথা যদি (repo_resolve_gitlink_ref(the_repository, update_তথ্য->sm_path,
					  "HEAD", &update_তথ্য->suboid))
		ফেরত die_message(_("Unable to find current revision in submodule path '%s'"),
				   update_তথ্য->displaypath);

	যদি (update_তথ্য->remote) {
		অক্ষর *remote_name;
		ধ্রুবক অক্ষর *branch;
		অক্ষর *remote_ref;
		পূর্ণসংখ্যা code;

		code = get_default_remote_submodule(update_তথ্য->sm_path, &remote_name);
		যদি (code)
			ফেরত code;
		code = remote_submodule_branch(update_তথ্য->sm_path, &branch);
		যদি (code) {
			মুক্ত(remote_name);
			ফেরত code;
		}
		remote_ref = xstrfmt("refs/remotes/%s/%s", remote_name, branch);

		মুক্ত(remote_name);

		যদি (!update_তথ্য->nofetch) {
			যদি (fetch_in_submodule(update_তথ্য->sm_path, update_তথ্য->depth,
					      0, ফাঁকা)) {
				মুক্ত(remote_ref);
				ফেরত die_message(_("Unable to fetch in submodule path '%s'"),
						   update_তথ্য->sm_path);
			}
		}

		যদি (repo_resolve_gitlink_ref(the_repository, update_তথ্য->sm_path,
					     remote_ref, &update_তথ্য->oid)) {
			ret = die_message(_("Unable to find %s revision in submodule path '%s'"),
					  remote_ref, update_তথ্য->sm_path);
			মুক্ত(remote_ref);
			ফেরত ret;
		}

		মুক্ত(remote_ref);
	}

	যদি (!oideq(&update_তথ্য->oid, &update_তথ্য->suboid) || update_তথ্য->force) {
		ret = run_update_procedure(update_তথ্য);
		যদি (ret)
			ফেরত ret;
	}

	যদি (update_তথ্য->recursive) {
		কাঠামো child_process cp = CHILD_PROCESS_INIT;
		কাঠামো update_তথ্য পরবর্তী = *update_তথ্য;

		পরবর্তী.prefix = ফাঁকা;
		oidcpy(&পরবর্তী.oid, null_oid(the_hash_algo));
		oidcpy(&পরবর্তী.suboid, null_oid(the_hash_algo));

		cp.dir = update_তথ্য->sm_path;
		cp.git_cmd = 1;
		prepare_submodule_repo_env(&cp.env);
		update_তথ্য_to_args(&পরবর্তী, &cp.args);

		ret = run_command(&cp);
		যদি (ret)
			die_message(_("Failed to recurse into submodule path '%s'"),
				    update_তথ্য->displaypath);
		ফেরত ret;
	}

	ফেরত 0;
}

স্থির পূর্ণসংখ্যা update_submodules(কাঠামো update_তথ্য *update_তথ্য)
{
	পূর্ণসংখ্যা i, ret = 0;
	কাঠামো submodule_update_clone suc = SUBMODULE_UPDATE_CLONE_INIT;
	ধ্রুবক কাঠামো run_process_parallel_opts opts = {
		.tr2_category = "submodule",
		.tr2_label = "parallel/update",

		.processes = update_তথ্য->max_jobs,

		.get_পরবর্তী_task = update_clone_get_পরবর্তী_task,
		.start_failure = update_clone_start_failure,
		.task_finished = update_clone_task_finished,
		.তথ্য = &suc,
	};

	suc.update_তথ্য = update_তথ্য;
	run_processes_parallel(&opts);

	/*
	 * We saved the output and put it out all at once now.
	 * That means:
	 * - the listener does not have to interleave their (checkout)
	 *   work with our fetching.  The writes involved in a
	 *   checkout involve more straightforward sequential I/O.
	 * - the listener can avoid doing any work if fetching failed.
	 */
	যদি (suc.quickstop) {
		ret = 1;
		যাও cleanup;
	}

	জন্য (i = 0; i < suc.update_clone_nr; i++) {
		কাঠামো update_clone_তথ্য ucd = suc.update_clone[i];
		পূর্ণসংখ্যা code = 128;

		oidcpy(&update_তথ্য->oid, &ucd.oid);
		update_তথ্য->just_cloned = ucd.just_cloned;
		update_তথ্য->sm_path = ucd.sub->path;

		/*
		 * Verify that the submodule path does not contain any
		 * symlinks; if it does, it might have been tampered with.
		 * TODO: allow exempting it via
		 * `safe.submodule.path` or something
		 */
		যদি (validate_submodule_path(update_তথ্য->sm_path) < 0)
			যাও fail;

		code = ensure_core_worktree(update_তথ্য->sm_path);
		যদি (code)
			যাও fail;

		update_তথ্য->displaypath = get_submodule_displaypath(
			update_তথ্য->sm_path, update_তথ্য->prefix,
			update_তথ্য->super_prefix);
		code = update_submodule(update_তথ্য);
		FREE_AND_NULL(update_তথ্য->displaypath);
fail:
		যদি (!code)
			চালু;
		ret = code;
		যদি (ret == 128)
			যাও cleanup;
	}

cleanup:
	submodule_update_clone_release(&suc);
	string_list_clear(&update_তথ্য->references, 0);
	ফেরত ret;
}

স্থির পূর্ণসংখ্যা module_update(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv, ধ্রুবক অক্ষর *prefix,
			 কাঠামো repository *repo UNUSED)
{
	কাঠামো pathspec pathspec = { 0 };
	কাঠামো pathspec pathspec2 = { 0 };
	কাঠামো update_তথ্য opt = UPDATE_তথ্য_INIT;
	কাঠামো list_objects_filter_options filter_options =
		LIST_OBJECTS_FILTER_INIT;
	ধ্রুবক অক্ষর *ref_storage_format = ফাঁকা;
	পূর্ণসংখ্যা ret;
	কাঠামো option module_update_options[] = {
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
	ধ্রুবক অক্ষর *ধ্রুবক git_submodule_helper_usage[] = {
		N_("git submodule [--quiet] update"
		" [--init [--filter=<filter-spec>]] [--remote]"
		" [-N|--no-fetch] [-f|--force]"
		" [--checkout|--merge|--rebase]"
		" [--[no-]recommend-shallow] [--reference <repository>]"
		" [--recursive] [--[no-]single-branch] [--] [<path>...]"),
		ফাঁকা
	};

	update_clone_config_from_gitmodules(the_repository, &opt.max_jobs);
	repo_config(the_repository, git_update_clone_config, &opt.max_jobs);

	argc = parse_options(argc, argv, prefix, module_update_options,
			     git_submodule_helper_usage, 0);

	যদি (opt.require_init)
		opt.init = 1;

	যদি (filter_options.choice && !opt.init) {
		usage_with_options(git_submodule_helper_usage,
				   module_update_options);
	}

	যদি (ref_storage_format) {
		opt.ref_storage_format = ref_storage_format_by_name(ref_storage_format);
		যদি (opt.ref_storage_format == REF_STORAGE_FORMAT_UNKNOWN)
			die(_("unknown ref storage format '%s'"), ref_storage_format);
	}

	opt.filter_options = &filter_options;
	opt.prefix = prefix;

	যদি (opt.update_default)
		opt.update_strategy.type = opt.update_default;

	যদি (module_list_compute(argv, prefix, &pathspec, &opt.list) < 0) {
		ret = 1;
		যাও cleanup;
	}

	যদি (pathspec.nr)
		opt.warn_if_uninitialized = 1;

	যদি (opt.init) {
		কাঠামো module_list list = MODULE_LIST_INIT;
		কাঠামো init_cb info = INIT_CB_INIT;

		যদি (module_list_compute(argv, opt.prefix,
					&pathspec2, &list) < 0) {
			module_list_release(&list);
			ret = 1;
			যাও cleanup;
		}

		/*
		 * If there are no path args and submodule.active is set then,
		 * by default, only initialize 'active' modules.
		 */
		যদি (!argc && !repo_config_get(the_repository, "submodule.active"))
			module_list_active(&list);

		info.prefix = opt.prefix;
		info.super_prefix = opt.super_prefix;
		যদি (opt.quiet)
			info.flags |= OPT_QUIET;

		for_each_listed_submodule(&list, init_submodule_cb, &info);
		module_list_release(&list);
	}

	ret = update_submodules(&opt);
cleanup:
	update_তথ্য_release(&opt);
	list_objects_filter_release(&filter_options);
	clear_pathspec(&pathspec);
	clear_pathspec(&pathspec2);
	ফেরত ret;
}

স্থির পূর্ণসংখ্যা ঠেলো_check(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv, ধ্রুবক অক্ষর *prefix UNUSED,
		      কাঠামো repository *repo UNUSED)
{
	কাঠামো remote *remote;
	ধ্রুবক অক্ষর *superproject_শীর্ষ;
	অক্ষর *শীর্ষ;
	পূর্ণসংখ্যা detached_শীর্ষ = 0;
	কাঠামো object_id শীর্ষ_oid;

	যদি (argc < 3)
		die("submodule--helper push-check requires at least 2 arguments");

	/*
	 * superproject's resolved head ref.
	 * if HEAD then the superproject is in a detached head state, otherwise
	 * it will be the resolved head ref.
	 */
	superproject_শীর্ষ = argv[1];
	argv++;
	argc--;
	/* Get the submodule's head ref and determine if it is detached */
	শীর্ষ = refs_resolve_refdup(get_main_ref_store(the_repository), "HEAD",
				   0, &শীর্ষ_oid, ফাঁকা);
	যদি (!শীর্ষ)
		die(_("Failed to resolve HEAD as a valid ref."));
	যদি (!strcmp(শীর্ষ, "HEAD"))
		detached_শীর্ষ = 1;

	/*
	 * The remote must be configured.
	 * This is to avoid pushing to the exact same URL as the parent.
	 */
	remote = pushremote_get(argv[1]);
	যদি (!remote || remote->origin == REMOTE_UNCONFIGURED)
		die("remote '%s' not configured", argv[1]);

	/* Check the refspec */
	যদি (argc > 2) {
		পূর্ণসংখ্যা i;
		কাঠামো ref *local_refs = get_local_heads();
		কাঠামো refspec refspec = REFSPEC_INIT_ঠেলো(the_hash_algo);

		refspec_appendn(&refspec, argv + 2, argc - 2);

		জন্য (i = 0; i < refspec.nr; i++) {
			ধ্রুবক কাঠামো refspec_item *rs = &refspec.বস্তুসমূহ[i];

			যদি (rs->pattern || rs->matching)
				চালু;

			/* LHS must match a single ref */
			নির্বাচন (গুনতি_refspec_match(rs->src, local_refs, ফাঁকা)) {
			ক্ষেত্র 1:
				বিরতি;
			ক্ষেত্র 0:
				/*
				 * If LHS matches 'HEAD' then we need to ensure
				 * that it matches the same named branch
				 * checked out in the superproject.
				 */
				যদি (!strcmp(rs->src, "HEAD")) {
					যদি (!detached_শীর্ষ &&
					    !strcmp(শীর্ষ, superproject_শীর্ষ))
						বিরতি;
					die("HEAD does not match the named branch in the superproject");
				}
				/* fallthrough */
			পূর্বনির্ধারিত:
				die("src refspec '%s' must name a ref",
				    rs->src);
			}
		}

		refspec_clear(&refspec);
		free_refs(local_refs);
	}
	মুক্ত(শীর্ষ);

	ফেরত 0;
}

স্থির পূর্ণসংখ্যা absorb_git_dirs(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv, ধ্রুবক অক্ষর *prefix,
			   কাঠামো repository *repo UNUSED)
{
	পূর্ণসংখ্যা i;
	কাঠামো pathspec pathspec = { 0 };
	কাঠামো module_list list = MODULE_LIST_INIT;
	ধ্রুবক অক্ষর *super_prefix = ফাঁকা;
	কাঠামো option embed_gitdir_options[] = {
		OPT__SUPER_PREFIX(&super_prefix),
		OPT_END()
	};
	ধ্রুবক অক্ষর *ধ্রুবক git_submodule_helper_usage[] = {
		N_("git submodule absorbgitdirs [<options>] [<path>...]"),
		ফাঁকা
	};
	পূর্ণসংখ্যা ret = 1;

	argc = parse_options(argc, argv, prefix, embed_gitdir_options,
			     git_submodule_helper_usage, 0);

	যদি (module_list_compute(argv, prefix, &pathspec, &list) < 0)
		যাও cleanup;

	জন্য (i = 0; i < list.nr; i++)
		absorb_git_dir_into_superproject(list.entries[i]->name,
						 super_prefix);

	ret = 0;
cleanup:
	clear_pathspec(&pathspec);
	module_list_release(&list);
	ফেরত ret;
}

স্থির পূর্ণসংখ্যা module_set_url(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv, ধ্রুবক অক্ষর *prefix,
			  কাঠামো repository *repo UNUSED)
{
	পূর্ণসংখ্যা quiet = 0, ret;
	ধ্রুবক অক্ষর *newurl;
	ধ্রুবক অক্ষর *path;
	অক্ষর *config_name;
	কাঠামো option options[] = {
		OPT__QUIET(&quiet, N_("suppress output for setting url of a submodule")),
		OPT_END()
	};
	ধ্রুবক অক্ষর *ধ্রুবক usage[] = {
		N_("git submodule set-url [--quiet] <path> <newurl>"),
		ফাঁকা
	};
	ধ্রুবক কাঠামো submodule *sub;

	argc = parse_options(argc, argv, prefix, options, usage, 0);

	যদি (argc != 2 || !(path = argv[0]) || !(newurl = argv[1]))
		usage_with_options(usage, options);

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);

	যদি (!sub)
		die(_("no submodule mapping found in .gitmodules for path '%s'"),
		    path);

	config_name = xstrfmt("submodule.%s.url", sub->name);
	ret = config_set_in_gitmodules_file_gently(the_repository, config_name, newurl);

	যদি (!ret) {
		repo_read_gitmodules(the_repository, 0);
		sync_submodule(sub->path, prefix, ফাঁকা, quiet ? OPT_QUIET : 0);
	}

	মুক্ত(config_name);
	ফেরত !!ret;
}

স্থির পূর্ণসংখ্যা module_set_branch(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv, ধ্রুবক অক্ষর *prefix,
			     কাঠামো repository *repo UNUSED)
{
	পূর্ণসংখ্যা opt_default = 0, ret;
	ধ্রুবক অক্ষর *opt_branch = ফাঁকা;
	ধ্রুবক অক্ষর *path;
	অক্ষর *config_name;
	কাঠামো option options[] = {
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
	ধ্রুবক অক্ষর *ধ্রুবক usage[] = {
		N_("git submodule set-branch [-q|--quiet] (-d|--default) <path>"),
		N_("git submodule set-branch [-q|--quiet] (-b|--branch) <branch> <path>"),
		ফাঁকা
	};
	ধ্রুবক কাঠামো submodule *sub;

	argc = parse_options(argc, argv, prefix, options, usage, 0);

	যদি (!opt_branch && !opt_default)
		die(_("--branch or --default required"));

	যদি (opt_branch && opt_default)
		die(_("options '%s' and '%s' cannot be used together"), "--branch", "--default");

	যদি (argc != 1 || !(path = argv[0]))
		usage_with_options(usage, options);

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);

	যদি (!sub)
		die(_("no submodule mapping found in .gitmodules for path '%s'"),
		    path);

	config_name = xstrfmt("submodule.%s.branch", sub->name);
	ret = config_set_in_gitmodules_file_gently(the_repository, config_name, opt_branch);

	মুক্ত(config_name);
	ফেরত !!ret;
}

স্থির পূর্ণসংখ্যা module_create_branch(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv, ধ্রুবক অক্ষর *prefix,
				কাঠামো repository *repo UNUSED)
{
	গণনা branch_track track;
	পূর্ণসংখ্যা quiet = 0, force = 0, reflog = 0, dry_run = 0;
	কাঠামো option options[] = {
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
	ধ্রুবক অক্ষর *ধ্রুবক usage[] = {
		N_("git submodule--helper create-branch [-f|--force] [--create-reflog] [-q|--quiet] [-t|--track] [-n|--dry-run] <name> <start-oid> <start-name>"),
		ফাঁকা
	};
	কাঠামো repo_config_মানসমূহ *cfg = repo_config_মানসমূহ(the_repository);

	repo_config(the_repository, git_default_config, ফাঁকা);
	track = cfg->branch_track;
	argc = parse_options(argc, argv, prefix, options, usage, 0);

	যদি (argc != 3)
		usage_with_options(usage, options);

	যদি (!quiet && !dry_run)
		printf_ln(_("creating branch '%s'"), argv[0]);

	create_branches_recursively(the_repository, argv[0], argv[1], argv[2],
				    force, reflog, quiet, track, dry_run);
	ফেরত 0;
}

কাঠামো add_তথ্য {
	ধ্রুবক অক্ষর *prefix;
	ধ্রুবক অক্ষর *branch;
	ধ্রুবক অক্ষর *reference_path;
	অক্ষর *sm_path;
	ধ্রুবক অক্ষর *sm_name;
	ধ্রুবক অক্ষর *repo;
	ধ্রুবক অক্ষর *realrepo;
	গণনা ref_storage_format ref_storage_format;
	পূর্ণসংখ্যা depth;
	unsigned পূর্ণসংখ্যা force: 1;
	unsigned পূর্ণসংখ্যা quiet: 1;
	unsigned পূর্ণসংখ্যা progress: 1;
	unsigned পূর্ণসংখ্যা dissociate: 1;
};
#define ADD_তথ্য_INIT { \
	.depth = -1, \
	.ref_storage_format = REF_STORAGE_FORMAT_UNKNOWN, \
}

স্থির শূন্য append_fetch_remotes(কাঠামো strbuf *বার্তা, ধ্রুবক অক্ষর *git_dir_path)
{
	কাঠামো child_process cp_remote = CHILD_PROCESS_INIT;
	কাঠামো strbuf sb_remote_out = STRBUF_INIT;

	cp_remote.git_cmd = 1;
	strvec_pushf(&cp_remote.env,
		     "GIT_DIR=%s", git_dir_path);
	strvec_ঠেলো(&cp_remote.env, "GIT_WORK_TREE=.");
	strvec_pushl(&cp_remote.args, "remote", "-v", ফাঁকা);
	যদি (!capture_command(&cp_remote, &sb_remote_out, 0)) {
		অক্ষর *পরবর্তী_সারি;
		অক্ষর *সারি = sb_remote_out.buf;

		যখন ((পরবর্তী_সারি = strchr(সারি, '\n')) != ফাঁকা) {
			আকার_t len = পরবর্তী_সারি - সারি;

			যদি (strip_suffix_mem(সারি, &len, " (fetch)"))
				strbuf_addf(বার্তা, "  %.*s\n", (পূর্ণসংখ্যা)len, সারি);
			সারি = পরবর্তী_সারি + 1;
		}
	}

	strbuf_release(&sb_remote_out);
}

স্থির পূর্ণসংখ্যা add_submodule(ধ্রুবক কাঠামো add_তথ্য *add_তথ্য)
{
	কাঠামো module_clone_তথ্য clone_তথ্য = MODULE_CLONE_তথ্য_INIT;
	কাঠামো string_list reference = STRING_LIST_INIT_NODUP;
	পূর্ণসংখ্যা ret = -1;

	/* perhaps the path already exists and is already a git repo, else clone it */
	যদি (is_directory(add_তথ্য->sm_path)) {
		অক্ষর *submod_gitdir_path;
		কাঠামো strbuf sm_path = STRBUF_INIT;
		strbuf_addstr(&sm_path, add_তথ্য->sm_path);
		submod_gitdir_path = xstrfmt("%s/.git", add_তথ্য->sm_path);
		যদি (is_nonbare_repository_dir(&sm_path))
			printf(_("Adding existing repo at '%s' to the index\n"),
			       add_তথ্য->sm_path);
		অন্যথা
			die(_("'%s' already exists and is not a valid git repo"),
			    add_তথ্য->sm_path);
		strbuf_release(&sm_path);
		মুক্ত(submod_gitdir_path);
	} অন্যথা {
		কাঠামো child_process cp = CHILD_PROCESS_INIT;
		কাঠামো strbuf submod_gitdir = STRBUF_INIT;

		submodule_name_to_gitdir(&submod_gitdir, the_repository, add_তথ্য->sm_name);

		যদি (is_directory(submod_gitdir.buf)) {
			যদি (!add_তথ্য->force) {
				কাঠামো strbuf বার্তা = STRBUF_INIT;
				অক্ষর *die_বার্তা;

				strbuf_addf(&বার্তা, _("A git directory for '%s' is found "
						    "locally with remote(s):\n"),
					    add_তথ্য->sm_name);

				append_fetch_remotes(&বার্তা, submod_gitdir.buf);
				strbuf_release(&submod_gitdir);

				strbuf_addf(&বার্তা, _("If you want to reuse this local git "
						    "directory instead of cloning again from\n"
						    "  %s\n"
						    "use the '--force' option. If the local git "
						    "directory is not the correct repo\n"
						    "or you are unsure what this means choose "
						    "another name with the '--name' option."),
					    add_তথ্য->realrepo);

				die_বার্তা = strbuf_detach(&বার্তা, ফাঁকা);
				die("%s", die_বার্তা);
			} অন্যথা {
				printf(_("Reactivating local git directory for "
					 "submodule '%s'\n"), add_তথ্য->sm_name);
			}
		}
		strbuf_release(&submod_gitdir);

		clone_তথ্য.prefix = add_তথ্য->prefix;
		clone_তথ্য.path = add_তথ্য->sm_path;
		clone_তথ্য.name = add_তথ্য->sm_name;
		clone_তথ্য.url = add_তথ্য->realrepo;
		clone_তথ্য.quiet = add_তথ্য->quiet;
		clone_তথ্য.progress = add_তথ্য->progress;
		যদি (add_তথ্য->reference_path) {
			অক্ষর *p = xstrdup(add_তথ্য->reference_path);

			string_list_append(&reference, p)->util = p;
		}
		clone_তথ্য.ref_storage_format = add_তথ্য->ref_storage_format;
		clone_তথ্য.dissociate = add_তথ্য->dissociate;
		যদি (add_তথ্য->depth >= 0)
			clone_তথ্য.depth = add_তথ্য->depth;

		যদি (clone_submodule(&clone_তথ্য, &reference))
			যাও cleanup;

		prepare_submodule_repo_env(&cp.env);
		cp.git_cmd = 1;
		cp.dir = add_তথ্য->sm_path;
		/*
		 * NOTE: we only get here if add_data->force is true, so
		 * passing --force to checkout is reasonable.
		 */
		strvec_pushl(&cp.args, "checkout", "-f", "-q", ফাঁকা);

		যদি (add_তথ্য->branch) {
			strvec_pushl(&cp.args, "-B", add_তথ্য->branch, ফাঁকা);
			strvec_pushf(&cp.args, "origin/%s", add_তথ্য->branch);
		}

		যদি (run_command(&cp))
			die(_("unable to checkout submodule '%s'"), add_তথ্য->sm_path);
	}
	ret = 0;

cleanup:
	string_list_clear(&reference, 1);
	ফেরত ret;
}

স্থির পূর্ণসংখ্যা config_submodule_in_gitmodules(ধ্রুবক অক্ষর *name, ধ্রুবক অক্ষর *var, ধ্রুবক অক্ষর *মান)
{
	অক্ষর *চাবি;
	পূর্ণসংখ্যা ret;

	যদি (!is_writing_gitmodules_ok())
		die(_("please make sure that the .gitmodules file is in the working tree"));

	চাবি = xstrfmt("submodule.%s.%s", name, var);
	ret = config_set_in_gitmodules_file_gently(the_repository, চাবি, মান);
	মুক্ত(চাবি);

	ফেরত ret;
}

স্থির শূন্য configure_added_submodule(কাঠামো add_তথ্য *add_তথ্য)
{
	অক্ষর *চাবি;
	কাঠামো child_process add_submod = CHILD_PROCESS_INIT;
	কাঠামো child_process add_gitmodules = CHILD_PROCESS_INIT;
	ধ্রুবক কাঠামো string_list *মানসমূহ;
	পূর্ণসংখ্যা matched = 0;

	চাবি = xstrfmt("submodule.%s.url", add_তথ্য->sm_name);
	repo_config_set_gently(the_repository, চাবি, add_তথ্য->realrepo);
	মুক্ত(চাবি);

	add_submod.git_cmd = 1;
	strvec_pushl(&add_submod.args, "add",
		     "--no-warn-embedded-repo", ফাঁকা);
	যদি (add_তথ্য->force)
		strvec_ঠেলো(&add_submod.args, "--force");
	strvec_pushl(&add_submod.args, "--", add_তথ্য->sm_path, ফাঁকা);

	যদি (run_command(&add_submod))
		die(_("Failed to add submodule '%s'"), add_তথ্য->sm_path);

	যদি (config_submodule_in_gitmodules(add_তথ্য->sm_name, "path", add_তথ্য->sm_path) ||
	    config_submodule_in_gitmodules(add_তথ্য->sm_name, "url", add_তথ্য->repo))
		die(_("Failed to register submodule '%s'"), add_তথ্য->sm_path);

	যদি (add_তথ্য->branch) {
		যদি (config_submodule_in_gitmodules(add_তথ্য->sm_name,
						   "branch", add_তথ্য->branch))
			die(_("Failed to register submodule '%s'"), add_তথ্য->sm_path);
	}

	add_gitmodules.git_cmd = 1;
	strvec_pushl(&add_gitmodules.args,
		     "add", "--force", "--", ".gitmodules", ফাঁকা);

	যদি (run_command(&add_gitmodules))
		die(_("Failed to register submodule '%s'"), add_তথ্য->sm_path);

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
	যদি (repo_config_get(the_repository, "submodule.active") || /* key absent */
	    repo_config_get_string_multi(the_repository, "submodule.active", &মানসমূহ)) {
		/*
		 * If the submodule being added isn't already covered by the
		 * current configured pathspec, set the submodule's active flag
		 */
		চাবি = xstrfmt("submodule.%s.active", add_তথ্য->sm_name);
		repo_config_set_gently(the_repository, চাবি, "true");
		মুক্ত(চাবি);
	} অন্যথা {
		জন্য (আকার_t i = 0; i < মানসমূহ->nr; i++) {
			ধ্রুবক অক্ষর *pat = মানসমূহ->বস্তুসমূহ[i].string;
			যদি (!wildmatch(pat, add_তথ্য->sm_path, 0)) { /* match found */
				matched = 1;
				বিরতি;
			}
		}
		যদি (!matched) { /* no pattern matched -> force-enable */
			চাবি = xstrfmt("submodule.%s.active", add_তথ্য->sm_name);
			repo_config_set_gently(the_repository, চাবি, "true");
			মুক্ত(চাবি);
		}
	}
}

স্থির শূন্য die_on_সূচক_match(ধ্রুবক অক্ষর *path, পূর্ণসংখ্যা force)
{
	কাঠামো pathspec ps;
	ধ্রুবক অক্ষর *args[] = { path, ফাঁকা };
	parse_pathspec(&ps, 0, PATHSPEC_PREFER_CWD, ফাঁকা, args);

	যদি (repo_read_সূচক_preload(the_repository, ফাঁকা, 0) < 0)
		die(_("index file corrupt"));

	যদি (ps.nr) {
		অক্ষর *ps_matched = xcalloc(ps.nr, 1);

		/* TODO: audit for interaction with sparse-index. */
		ensure_full_সূচক(the_repository->সূচক);

		/*
		 * Since there is only one pathspec, we just need to
		 * check ps_matched[0] to know if a cache entry matched.
		 */
		জন্য (আকার_t i = 0; i < the_repository->সূচক->cache_nr; i++) {
			ce_path_match(the_repository->সূচক, the_repository->সূচক->cache[i], &ps,
				      ps_matched);

			যদি (ps_matched[0]) {
				যদি (!force)
					die(_("'%s' already exists in the index"),
					    path);
				যদি (!S_ISGITLINK(the_repository->সূচক->cache[i]->ce_mode))
					die(_("'%s' already exists in the index "
					      "and is not a submodule"), path);
				বিরতি;
			}
		}
		মুক্ত(ps_matched);
	}
	clear_pathspec(&ps);
}

স্থির শূন্য die_on_repo_without_commits(ধ্রুবক অক্ষর *path)
{
	কাঠামো strbuf sb = STRBUF_INIT;
	strbuf_addstr(&sb, path);
	যদি (is_nonbare_repository_dir(&sb)) {
		কাঠামো object_id oid;
		যদি (repo_resolve_gitlink_ref(the_repository, path, "HEAD", &oid) < 0)
			die(_("'%s' does not have a commit checked out"), path);
	}
	strbuf_release(&sb);
}

স্থির পূর্ণসংখ্যা module_add(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv, ধ্রুবক অক্ষর *prefix,
		      কাঠামো repository *repo UNUSED)
{
	পূর্ণসংখ্যা force = 0, quiet = 0, progress = 0, dissociate = 0;
	কাঠামো add_তথ্য add_তথ্য = ADD_তথ্য_INIT;
	ধ্রুবক অক্ষর *ref_storage_format = ফাঁকা;
	অক্ষর *to_free = ফাঁকা;
	ধ্রুবক কাঠামো submodule *existing;
	কাঠামো strbuf buf = STRBUF_INIT;
	অক্ষর *sm_name_to_free = ফাঁকা;
	কাঠামো option options[] = {
		OPT_STRING('b', "branch", &add_তথ্য.branch, N_("branch"),
			   N_("branch of repository to add as submodule")),
		OPT__FORCE(&force, N_("allow adding an otherwise ignored submodule path"),
			   PARSE_OPT_NOCOMPLETE),
		OPT__QUIET(&quiet, N_("print only error messages")),
		OPT_BOOL(0, "progress", &progress, N_("force cloning progress")),
		OPT_STRING(0, "reference", &add_তথ্য.reference_path, N_("repository"),
			   N_("reference repository")),
		OPT_STRING(0, "ref-storage-format", &ref_storage_format, N_("format"),
			   N_("specify the reference storage format to use")),
		OPT_ALIAS_F(0, "ref-format", "ref-storage-format", PARSE_OPT_HIDDEN),
		OPT_BOOL(0, "dissociate", &dissociate, N_("borrow the objects from reference repositories")),
		OPT_STRING(0, "name", &add_তথ্য.sm_name, N_("name"),
			   N_("sets the submodule's name to the given string "
			      "instead of defaulting to its path")),
		OPT_INTEGER(0, "depth", &add_তথ্য.depth, N_("depth for shallow clones")),
		OPT_END()
	};
	ধ্রুবক অক্ষর *ধ্রুবক usage[] = {
		N_("git submodule add [<options>] [--] <repository> [<path>]"),
		ফাঁকা
	};
	কাঠামো strbuf sb = STRBUF_INIT;
	পূর্ণসংখ্যা ret = 1;

	argc = parse_options(argc, argv, prefix, options, usage, 0);

	যদি (!is_writing_gitmodules_ok())
		die(_("please make sure that the .gitmodules file is in the working tree"));

	যদি (prefix && *prefix &&
	    add_তথ্য.reference_path && !is_absolute_path(add_তথ্য.reference_path))
		add_তথ্য.reference_path = xstrfmt("%s%s", prefix, add_তথ্য.reference_path);

	যদি (argc == 0 || argc > 2)
		usage_with_options(usage, options);

	যদি (ref_storage_format) {
		add_তথ্য.ref_storage_format = ref_storage_format_by_name(ref_storage_format);
		যদি (add_তথ্য.ref_storage_format == REF_STORAGE_FORMAT_UNKNOWN)
			die(_("unknown ref storage format '%s'"), ref_storage_format);
	}

	add_তথ্য.repo = argv[0];
	যদি (argc == 1)
		add_তথ্য.sm_path = git_url_basename(add_তথ্য.repo, 0, 0);
	অন্যথা
		add_তথ্য.sm_path = xstrdup(argv[1]);

	যদি (prefix && *prefix && !is_absolute_path(add_তথ্য.sm_path)) {
		অক্ষর *sm_path = add_তথ্য.sm_path;

		add_তথ্য.sm_path = xstrfmt("%s%s", prefix, sm_path);
		মুক্ত(sm_path);
	}

	যদি (starts_with_dot_dot_slash(add_তথ্য.repo) ||
	    starts_with_dot_slash(add_তথ্য.repo)) {
		যদি (prefix)
			die(_("Relative path can only be used from the toplevel "
			      "of the working tree"));

		/* dereference source url relative to parent's url */
		to_free = resolve_relative_url(add_তথ্য.repo, ফাঁকা, 1);
		add_তথ্য.realrepo = to_free;
	} অন্যথা যদি (is_dir_sep(add_তথ্য.repo[0]) || strchr(add_তথ্য.repo, ':')) {
		add_তথ্য.realrepo = add_তথ্য.repo;
	} অন্যথা {
		die(_("repo URL: '%s' must be absolute or begin with ./|../"),
		    add_তথ্য.repo);
	}

	/*
	 * normalize path:
	 * multiple //; leading ./; /./; /../;
	 */
	normalize_path_copy(add_তথ্য.sm_path, add_তথ্য.sm_path);
	strip_dir_trailing_slashes(add_তথ্য.sm_path);

	যদি (validate_submodule_path(add_তথ্য.sm_path) < 0)
		die(ফাঁকা);

	die_on_সূচক_match(add_তথ্য.sm_path, force);
	die_on_repo_without_commits(add_তথ্য.sm_path);

	যদি (!force) {
		কাঠামো child_process cp = CHILD_PROCESS_INIT;

		cp.git_cmd = 1;
		cp.no_stdout = 1;
		strvec_pushl(&cp.args, "add", "--dry-run", "--ignore-missing",
			     "--no-warn-embedded-repo", add_তথ্য.sm_path, ফাঁকা);
		যদি ((ret = pipe_command(&cp, ফাঁকা, 0, ফাঁকা, 0, &sb, 0))) {
			strbuf_complete_সারি(&sb);
			fputs(sb.buf, stderr);
			যাও cleanup;
		}
	}

	যদি (!add_তথ্য.sm_name)
		add_তথ্য.sm_name = add_তথ্য.sm_path;

	existing = submodule_from_name(the_repository,
					null_oid(the_hash_algo),
					add_তথ্য.sm_name);

	যদি (existing && existing->path &&
	    strcmp(existing->path, add_তথ্য.sm_path)) {
		যদি (!force) {
			die(_("submodule name '%s' already used for path '%s'"),
			    add_তথ্য.sm_name, existing->path);
		}
		/* --force: build <name><n> until unique */
		জন্য (পূর্ণসংখ্যা i = 1; ; i++) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "%s%d", add_তথ্য.sm_name, i);
			যদি (!submodule_from_name(the_repository,
						null_oid(the_hash_algo),
						buf.buf)) {
				বিরতি;
			}
		}
		add_তথ্য.sm_name = sm_name_to_free = strbuf_detach(&buf, ফাঁকা);
	}

	যদি (check_submodule_name(add_তথ্য.sm_name))
		die(_("'%s' is not a valid submodule name"), add_তথ্য.sm_name);

	add_তথ্য.prefix = prefix;
	add_তথ্য.force = !!force;
	add_তথ্য.quiet = !!quiet;
	add_তথ্য.progress = !!progress;
	add_তথ্য.dissociate = !!dissociate;

	যদি (the_repository->repository_format_submodule_path_cfg)
		create_default_gitdir_config(add_তথ্য.sm_name);

	যদি (add_submodule(&add_তথ্য))
		যাও cleanup;
	configure_added_submodule(&add_তথ্য);

	ret = 0;
cleanup:
	মুক্ত(sm_name_to_free);
	মুক্ত(add_তথ্য.sm_path);
	মুক্ত(to_free);
	strbuf_release(&sb);

	ফেরত ret;
}

পূর্ণসংখ্যা cmd_submodule__helper(পূর্ণসংখ্যা argc,
			  ধ্রুবক অক্ষর **argv,
			  ধ্রুবক অক্ষর *prefix,
			  কাঠামো repository *repo)
{
	parse_opt_subcommand_fn *fn = ফাঁকা;
	ধ্রুবক অক্ষর *ধ্রুবক usage[] = {
		N_("git submodule--helper <command>"),
		ফাঁকা
	};
	কাঠামো option options[] = {
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
		OPT_SUBCOMMAND("push-check", &fn, ঠেলো_check),
		OPT_SUBCOMMAND("absorbgitdirs", &fn, absorb_git_dirs),
		OPT_SUBCOMMAND("set-url", &fn, module_set_url),
		OPT_SUBCOMMAND("set-branch", &fn, module_set_branch),
		OPT_SUBCOMMAND("create-branch", &fn, module_create_branch),
		OPT_SUBCOMMAND("get-default-remote", &fn, module_get_default_remote),
		OPT_END()
	};
	argc = parse_options(argc, argv, prefix, options, usage, 0);

	ফেরত fn(argc, argv, prefix, repo);
}
