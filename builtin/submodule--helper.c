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

প্রকারনির্দেশ শূন্য (*each_submodule_fn)(ধ্রুব সংরচনা cache_entry *list_item,
				  শূন্য *cb_data);

স্থির বর্ণ *get_default_remote(শূন্য)
{
	প্রত্যাগম xstrdup(repo_default_remote(the_repository));
}

স্থির বর্ণ *resolve_relative_url(ধ্রুব বর্ণ *rel_url, ধ্রুব বর্ণ *up_path, পূর্ণাঙ্ক quiet)
{
	বর্ণ *remoteurl, *resolved_url;
	বর্ণ *remote = get_default_remote();
	সংরচনা strbuf remotesb = STRBUF_INIT;

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

	প্রত্যাগম resolved_url;
}

স্থির পূর্ণাঙ্ক get_default_remote_submodule(ধ্রুব বর্ণ *module_path, বর্ণ **default_remote)
{
	ধ্রুব সংরচনা submodule *sub;
	সংরচনা repository subrepo;
	ধ্রুব বর্ণ *remote_name = রিক্ত;
	বর্ণ *url = রিক্ত;
	পূর্ণাঙ্ক ret = 0;

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), module_path);
	যদি (sub && sub->url) {
		url = xstrdup(sub->url);

		/* Possibly a url relative to parent */
		যদি (starts_with_dot_dot_slash(url) ||
		    starts_with_dot_slash(url)) {
			বর্ণ *oldurl = url;

			url = resolve_relative_url(oldurl, রিক্ত, 1);
			মুক্ত(oldurl);
		}
	}

	যদি (repo_submodule_init(&subrepo, the_repository, module_path,
				null_oid(the_hash_algo)) < 0) {
		ret = die_message(_("could not get a repository handle for submodule '%s'"),
				   module_path);
		গচ্ছ out;
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

	প্রত্যাগম ret;
}

স্থির পূর্ণাঙ্ক module_get_default_remote(পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv, ধ্রুব বর্ণ *prefix,
				     সংরচনা repository *repo UNUSED)
{
	ধ্রুব বর্ণ *path;
	বর্ণ *resolved_path = রিক্ত;
	বর্ণ *default_remote = রিক্ত;
	পূর্ণাঙ্ক code;
	সংরচনা option options[] = {
		OPT_END()
	};
	ধ্রুব বর্ণ *ধ্রুব usage[] = {
		N_("git submodule--helper get-default-remote <path>"),
		রিক্ত
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
		প্রত্যাগম code;
	}

	printf("%s\n", default_remote);
	মুক্ত(default_remote);
	মুক্ত(resolved_path);
	প্রত্যাগম 0;
}

/* the result should be freed by the caller. */
স্থির বর্ণ *get_submodule_displaypath(ধ্রুব বর্ণ *path, ধ্রুব বর্ণ *prefix,
				       ধ্রুব বর্ণ *super_prefix)
{
	যদি (prefix && super_prefix) {
		BUG("cannot have prefix '%s' and superprefix '%s'",
		    prefix, super_prefix);
	} অন্যথা যদি (prefix) {
		সংরচনা strbuf sb = STRBUF_INIT;
		বর্ণ *displaypath = xstrdup(relative_path(path, prefix, &sb));
		strbuf_release(&sb);
		প্রত্যাগম displaypath;
	} অন্যথা যদি (super_prefix) {
		প্রত্যাগম xstrfmt("%s%s", super_prefix, path);
	} অন্যথা {
		প্রত্যাগম xstrdup(path);
	}
}

স্থির বর্ণ *compute_rev_name(ধ্রুব বর্ণ *sub_path, ধ্রুব বর্ণ* object_id)
{
	সংরচনা strbuf sb = STRBUF_INIT;
	ধ্রুব বর্ণ ***d;

	স্থির ধ্রুব বর্ণ *describe_bare[] = { রিক্ত };

	স্থির ধ্রুব বর্ণ *describe_tags[] = { "--tags", রিক্ত };

	স্থির ধ্রুব বর্ণ *describe_contains[] = { "--contains", রিক্ত };

	স্থির ধ্রুব বর্ণ *describe_all_always[] = { "--all", "--always", রিক্ত };

	স্থির ধ্রুব বর্ণ **describe_argv[] = { describe_bare, describe_tags,
						describe_contains,
						describe_all_always, রিক্ত };

	প্রতি (d = describe_argv; *d; d++) {
		সংরচনা child_process cp = CHILD_PROCESS_INIT;
		prepare_submodule_repo_env(&cp.env);
		cp.dir = sub_path;
		cp.git_cmd = 1;
		cp.no_stderr = 1;

		strvec_ঠেলো(&cp.args, "describe");
		strvec_pushv(&cp.args, *d);
		strvec_ঠেলো(&cp.args, object_id);

		যদি (!capture_command(&cp, &sb, 0)) {
			strbuf_strip_suffix(&sb, "\n");
			প্রত্যাগম strbuf_detach(&sb, রিক্ত);
		}
	}

	strbuf_release(&sb);
	প্রত্যাগম রিক্ত;
}

সংরচনা module_list {
	ধ্রুব সংরচনা cache_entry **entries;
	পূর্ণাঙ্ক alloc, nr;
};
#define MODULE_LIST_INIT { 0 }

স্থির শূন্য module_list_release(সংরচনা module_list *ml)
{
	মুক্ত(ml->entries);
}

স্থির পূর্ণাঙ্ক module_list_compute(ধ্রুব বর্ণ **argv,
			       ধ্রুব বর্ণ *prefix,
			       সংরচনা pathspec *pathspec,
			       সংরচনা module_list *list)
{
	পূর্ণাঙ্ক result = 0;
	বর্ণ *ps_matched = রিক্ত;

	parse_pathspec(pathspec, 0,
		       PATHSPEC_PREFER_FULL,
		       prefix, argv);

	যদি (pathspec->nr)
		ps_matched = xcalloc(pathspec->nr, 1);

	যদি (repo_read_index(the_repository) < 0)
		die(_("index file corrupt"));

	প্রতি (size_t i = 0; i < the_repository->index->cache_nr; i++) {
		ধ্রুব সংরচনা cache_entry *ce = the_repository->index->cache[i];

		যদি (!match_pathspec(the_repository->index, pathspec, ce->name, ce_namelen(ce),
				    0, ps_matched, 1) ||
		    !S_ISGITLINK(ce->ce_mode))
			অনুবর্ত;

		ALLOC_GROW(list->entries, list->nr + 1, list->alloc);
		list->entries[list->nr++] = ce;
		যাবত্ (i + 1 < the_repository->index->cache_nr &&
		       !strcmp(ce->name, the_repository->index->cache[i + 1]->name))
			/*
			 * Skip entries with the same name in different stages
			 * to make sure an entry is returned only once.
			 */
			i++;
	}

	যদি (ps_matched && report_path_error(ps_matched, pathspec))
		result = -1;

	মুক্ত(ps_matched);

	প্রত্যাগম result;
}

স্থির শূন্য module_list_active(সংরচনা module_list *list)
{
	পূর্ণাঙ্ক i;
	সংরচনা module_list active_modules = MODULE_LIST_INIT;

	প্রতি (i = 0; i < list->nr; i++) {
		ধ্রুব সংরচনা cache_entry *ce = list->entries[i];

		যদি (!is_submodule_active(the_repository, ce->name))
			অনুবর্ত;

		ALLOC_GROW(active_modules.entries,
			   active_modules.nr + 1,
			   active_modules.alloc);
		active_modules.entries[active_modules.nr++] = ce;
	}

	module_list_release(list);
	*list = active_modules;
}

স্থির বর্ণ *get_up_path(ধ্রুব বর্ণ *path)
{
	সংরচনা strbuf sb = STRBUF_INIT;

	strbuf_addstrings(&sb, "../", গুনতি_slashes(path));

	/*
	 * Check if 'path' ends with slash or not
	 * for having the same output for dir/sub_dir
	 * and dir/sub_dir/
	 */
	যদি (!is_dir_sep(path[strlen(path) - 1]))
		strbuf_addstr(&sb, "../");

	প্রত্যাগম strbuf_detach(&sb, রিক্ত);
}

স্থির শূন্য for_each_listed_submodule(ধ্রুব সংরচনা module_list *list,
				      each_submodule_fn fn, শূন্য *cb_data)
{
	পূর্ণাঙ্ক i;

	প্রতি (i = 0; i < list->nr; i++)
		fn(list->entries[i], cb_data);
}

সংরচনা foreach_cb {
	পূর্ণাঙ্ক argc;
	ধ্রুব বর্ণ **argv;
	ধ্রুব বর্ণ *prefix;
	ধ্রুব বর্ণ *super_prefix;
	পূর্ণাঙ্ক quiet;
	পূর্ণাঙ্ক recursive;
};
#define FOREACH_CB_INIT { 0 }

স্থির শূন্য runcommand_in_submodule_cb(ধ্রুব সংরচনা cache_entry *list_item,
				       শূন্য *cb_data)
{
	সংরচনা foreach_cb *info = cb_data;
	ধ্রুব বর্ণ *path = list_item->name;
	ধ্রুব সংরচনা object_id *ce_oid = &list_item->oid;
	ধ্রুব সংরচনা submodule *sub;
	সংরচনা child_process cp = CHILD_PROCESS_INIT;
	বর্ণ *displaypath;

	যদি (validate_submodule_path(path) < 0)
		die(রিক্ত);

	displaypath = get_submodule_displaypath(path, info->prefix,
						info->super_prefix);

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);

	যদি (!sub)
		die(_("No url found for submodule path '%s' in .gitmodules"),
			displaypath);

	যদি (!is_submodule_populated_gently(path, রিক্ত))
		গচ্ছ cleanup;

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
		বর্ণ *toplevel = xgetcwd();
		সংরচনা strbuf sb = STRBUF_INIT;

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
		সংরচনা child_process cpr = CHILD_PROCESS_INIT;

		cpr.git_cmd = 1;
		cpr.dir = path;
		prepare_submodule_repo_env(&cpr.env);

		strvec_pushl(&cpr.args, "submodule--helper", "foreach", "--recursive",
			     রিক্ত);
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

স্থির পূর্ণাঙ্ক module_foreach(পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv, ধ্রুব বর্ণ *prefix,
			  সংরচনা repository *repo UNUSED)
{
	সংরচনা foreach_cb info = FOREACH_CB_INIT;
	সংরচনা pathspec pathspec = { 0 };
	সংরচনা module_list list = MODULE_LIST_INIT;
	সংরচনা option module_foreach_options[] = {
		OPT__SUPER_PREFIX(&info.super_prefix),
		OPT__QUIET(&info.quiet, N_("suppress output of entering each submodule command")),
		OPT_BOOL(0, "recursive", &info.recursive,
			 N_("recurse into nested submodules")),
		OPT_END()
	};
	ধ্রুব বর্ণ *ধ্রুব git_submodule_helper_usage[] = {
		N_("git submodule foreach [--quiet] [--recursive] [--] <command>"),
		রিক্ত
	};
	পূর্ণাঙ্ক ret = 1;

	argc = parse_options(argc, argv, prefix, module_foreach_options,
			     git_submodule_helper_usage, 0);

	যদি (module_list_compute(রিক্ত, prefix, &pathspec, &list) < 0)
		গচ্ছ cleanup;

	info.argc = argc;
	info.argv = argv;
	info.prefix = prefix;

	for_each_listed_submodule(&list, runcommand_in_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	প্রত্যাগম ret;
}

সংরচনা init_cb {
	ধ্রুব বর্ণ *prefix;
	ধ্রুব বর্ণ *super_prefix;
	unsigned পূর্ণাঙ্ক flags;
};
#define INIT_CB_INIT { 0 }

স্থির পূর্ণাঙ্ক validate_and_set_submodule_gitdir(সংরচনা strbuf *gitdir_path,
					     ধ্রুব বর্ণ *submodule_name)
{
	ধ্রুব বর্ণ *মান;
	বর্ণ *চাবি;

	যদি (validate_submodule_git_dir(gitdir_path->buf, submodule_name))
		প্রত্যাগম -1;

	চাবি = xstrfmt("submodule.%s.gitdir", submodule_name);

	/* Nothing to do if the config already exists. */
	যদি (!repo_config_get_string_tmp(the_repository, চাবি, &মান)) {
		মুক্ত(চাবি);
		প্রত্যাগম 0;
	}

	যদি (repo_config_set_gently(the_repository, চাবি, gitdir_path->buf)) {
		মুক্ত(চাবি);
		প্রত্যাগম -1;
	}

	মুক্ত(চাবি);
	প্রত্যাগম 0;
}

স্থির শূন্য create_default_gitdir_config(ধ্রুব বর্ণ *submodule_name)
{
	সংরচনা strbuf gitdir_path = STRBUF_INIT;
	সংরচনা git_hash_ctx ctx;
	বর্ণ hex_name_hash[GIT_MAX_HEXSZ + 1], header[128];
	unsigned বর্ণ raw_name_hash[GIT_MAX_RAWSZ];
	পূর্ণাঙ্ক header_len;

	/* Case 1: try the plain module name */
	repo_git_path_append(the_repository, &gitdir_path, "modules/%s", submodule_name);
	যদি (!validate_and_set_submodule_gitdir(&gitdir_path, submodule_name)) {
		strbuf_release(&gitdir_path);
		প্রত্যাগম;
	}

	/* Case 2.1: Try URI-safe (RFC3986) encoding first, this fixes nested gitdirs */
	strbuf_reset(&gitdir_path);
	repo_git_path_append(the_repository, &gitdir_path, "modules/");
	strbuf_addstr_urlencode(&gitdir_path, submodule_name, is_rfc3986_unreserved);
	যদি (!validate_and_set_submodule_gitdir(&gitdir_path, submodule_name)) {
		strbuf_release(&gitdir_path);
		প্রত্যাগম;
	}

	/* Case 2.2: Try extended uppercase URI (RFC3986) encoding, to fix case-folding */
	strbuf_reset(&gitdir_path);
	repo_git_path_append(the_repository, &gitdir_path, "modules/");
	strbuf_addstr_urlencode(&gitdir_path, submodule_name, is_casefolding_rfc3986_unreserved);
	যদি (!validate_and_set_submodule_gitdir(&gitdir_path, submodule_name))
		প্রত্যাগম;

	/* Case 2.3: Try some derived gitdir names, see if one sticks */
	প্রতি (বর্ণ c = '0'; c <= '9'; c++) {
		strbuf_reset(&gitdir_path);
		repo_git_path_append(the_repository, &gitdir_path, "modules/");
		strbuf_addstr_urlencode(&gitdir_path, submodule_name, is_rfc3986_unreserved);
		strbuf_addch(&gitdir_path, c);
		যদি (!validate_and_set_submodule_gitdir(&gitdir_path, submodule_name))
			প্রত্যাগম;

		strbuf_reset(&gitdir_path);
		repo_git_path_append(the_repository, &gitdir_path, "modules/");
		strbuf_addstr_urlencode(&gitdir_path, submodule_name, is_casefolding_rfc3986_unreserved);
		strbuf_addch(&gitdir_path, c);
		যদি (!validate_and_set_submodule_gitdir(&gitdir_path, submodule_name))
			প্রত্যাগম;
	}

	/* Case 2.4: If all the above failed, try a hash of the name as a last resort */
	header_len = snprintf(header, পরিমাণ(header),
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
		প্রত্যাগম;
	}

	/* Case 3: nothing worked, error out */
	die(_("failed to set a valid default config for 'submodule.%s.gitdir'. "
	      "Please ensure it is set, for example by running something like: "
	      "'git config submodule.%s.gitdir .git/modules/%s'"),
	    submodule_name, submodule_name, submodule_name);
}

স্থির শূন্য init_submodule(ধ্রুব বর্ণ *path, ধ্রুব বর্ণ *prefix,
			   ধ্রুব বর্ণ *super_prefix,
			   unsigned পূর্ণাঙ্ক flags)
{
	ধ্রুব সংরচনা submodule *sub;
	সংরচনা strbuf sb = STRBUF_INIT;
	ধ্রুব বর্ণ *upd;
	বর্ণ *url = রিক্ত, *displaypath;

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
			বর্ণ *oldurl = url;

			url = resolve_relative_url(oldurl, রিক্ত, 0);
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

স্থির শূন্য init_submodule_cb(ধ্রুব সংরচনা cache_entry *list_item, শূন্য *cb_data)
{
	সংরচনা init_cb *info = cb_data;

	init_submodule(list_item->name, info->prefix, info->super_prefix,
		       info->flags);
}

স্থির পূর্ণাঙ্ক module_init(পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv, ধ্রুব বর্ণ *prefix,
		       সংরচনা repository *repo UNUSED)
{
	সংরচনা init_cb info = INIT_CB_INIT;
	সংরচনা pathspec pathspec = { 0 };
	সংরচনা module_list list = MODULE_LIST_INIT;
	পূর্ণাঙ্ক quiet = 0;
	সংরচনা option module_init_options[] = {
		OPT__QUIET(&quiet, N_("suppress output for initializing a submodule")),
		OPT_END()
	};
	ধ্রুব বর্ণ *ধ্রুব git_submodule_helper_usage[] = {
		N_("git submodule init [<options>] [<path>]"),
		রিক্ত
	};
	পূর্ণাঙ্ক ret = 1;

	argc = parse_options(argc, argv, prefix, module_init_options,
			     git_submodule_helper_usage, 0);

	যদি (module_list_compute(argv, prefix, &pathspec, &list) < 0)
		গচ্ছ cleanup;

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
	প্রত্যাগম ret;
}

সংরচনা status_cb {
	ধ্রুব বর্ণ *prefix;
	ধ্রুব বর্ণ *super_prefix;
	unsigned পূর্ণাঙ্ক flags;
};
#define STATUS_CB_INIT { 0 }

স্থির শূন্য print_status(unsigned পূর্ণাঙ্ক flags, বর্ণ state, ধ্রুব বর্ণ *path,
			 ধ্রুব সংরচনা object_id *oid, ধ্রুব বর্ণ *displaypath)
{
	যদি (flags & OPT_QUIET)
		প্রত্যাগম;

	printf("%c%s %s", state, oid_to_hex(oid), displaypath);

	যদি (state == ' ' || state == '+') {
		বর্ণ *name = compute_rev_name(path, oid_to_hex(oid));

		যদি (name)
			printf(" (%s)", name);
		মুক্ত(name);
	}

	printf("\n");
}

স্থির পূর্ণাঙ্ক handle_submodule_head_ref(ধ্রুব সংরচনা reference *ref, শূন্য *cb_data)
{
	সংরচনা object_id *output = cb_data;

	যদি (ref->oid)
		oidcpy(output, ref->oid);

	প্রত্যাগম 0;
}

স্থির শূন্য status_submodule(ধ্রুব বর্ণ *path, ধ্রুব সংরচনা object_id *ce_oid,
			     unsigned পূর্ণাঙ্ক ce_flags, ধ্রুব বর্ণ *prefix,
			     ধ্রুব বর্ণ *super_prefix, unsigned পূর্ণাঙ্ক flags)
{
	বর্ণ *displaypath;
	সংরচনা strvec diff_files_args = STRVEC_INIT;
	সংরচনা rev_info rev = REV_INFO_INIT;
	সংরচনা strbuf buf = STRBUF_INIT;
	ধ্রুব বর্ণ *git_dir;

	যদি (validate_submodule_path(path) < 0)
		die(রিক্ত);

	যদি (!submodule_from_path(the_repository, null_oid(the_hash_algo), path))
		die(_("no submodule mapping found in .gitmodules for path '%s'"),
		      path);

	displaypath = get_submodule_displaypath(path, prefix, super_prefix);

	যদি ((CE_STAGEMASK & ce_flags) >> CE_STAGESHIFT) {
		print_status(flags, 'U', path, null_oid(the_hash_algo), displaypath);
		গচ্ছ cleanup;
	}

	strbuf_addf(&buf, "%s/.git", path);
	git_dir = read_gitfile(buf.buf);
	যদি (!git_dir)
		git_dir = buf.buf;

	যদি (!is_submodule_active(the_repository, path) ||
	    !is_git_directory(git_dir)) {
		print_status(flags, '-', path, ce_oid, displaypath);
		strbuf_release(&buf);
		গচ্ছ cleanup;
	}
	strbuf_release(&buf);

	strvec_pushl(&diff_files_args, "diff-files",
		     "--ignore-submodules=dirty", "--quiet", "--",
		     path, রিক্ত);

	repo_config(the_repository, git_diff_basic_config, রিক্ত);

	repo_init_revisions(the_repository, &rev, রিক্ত);
	rev.abbrev = 0;
	setup_revisions_from_strvec(&diff_files_args, &rev, রিক্ত);
	run_diff_files(&rev, 0);

	যদি (!diff_result_code(&rev)) {
		print_status(flags, ' ', path, ce_oid,
			     displaypath);
	} অন্যথা যদি (!(flags & OPT_CACHED)) {
		সংরচনা object_id oid;
		সংরচনা ref_store *refs = repo_get_submodule_ref_store(the_repository,
								      path);

		যদি (!refs) {
			print_status(flags, '-', path, ce_oid, displaypath);
			গচ্ছ cleanup;
		}
		যদি (refs_head_ref(refs, handle_submodule_head_ref, &oid))
			die(_("could not resolve HEAD ref inside the "
			      "submodule '%s'"), path);

		print_status(flags, '+', path, &oid, displaypath);
	} অন্যথা {
		print_status(flags, '+', path, ce_oid, displaypath);
	}

	যদি (flags & OPT_RECURSIVE) {
		সংরচনা child_process cpr = CHILD_PROCESS_INIT;
		পূর্ণাঙ্ক res;

		cpr.git_cmd = 1;
		cpr.dir = path;
		prepare_submodule_repo_env(&cpr.env);

		strvec_pushl(&cpr.args, "submodule--helper", "status",
			     "--recursive", রিক্ত);
		strvec_pushf(&cpr.args, "--super-prefix=%s/", displaypath);

		যদি (flags & OPT_CACHED)
			strvec_ঠেলো(&cpr.args, "--cached");

		যদি (flags & OPT_QUIET)
			strvec_ঠেলো(&cpr.args, "--quiet");

		res = run_command(&cpr);
		যদি (res == SIGPIPE + 128)
			উত্থাপয(SIGPIPE);
		অন্যথা যদি (res)
			die(_("failed to recurse into submodule '%s'"), path);
	}

cleanup:
	strvec_clear(&diff_files_args);
	মুক্ত(displaypath);
	release_revisions(&rev);
}

স্থির শূন্য status_submodule_cb(ধ্রুব সংরচনা cache_entry *list_item,
				শূন্য *cb_data)
{
	সংরচনা status_cb *info = cb_data;

	status_submodule(list_item->name, &list_item->oid, list_item->ce_flags,
			 info->prefix, info->super_prefix, info->flags);
}

স্থির পূর্ণাঙ্ক module_status(পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv, ধ্রুব বর্ণ *prefix,
			 সংরচনা repository *repo UNUSED)
{
	সংরচনা status_cb info = STATUS_CB_INIT;
	সংরচনা pathspec pathspec = { 0 };
	সংরচনা module_list list = MODULE_LIST_INIT;
	পূর্ণাঙ্ক quiet = 0;
	সংরচনা option module_status_options[] = {
		OPT__SUPER_PREFIX(&info.super_prefix),
		OPT__QUIET(&quiet, N_("suppress submodule status output")),
		OPT_BIT(0, "cached", &info.flags, N_("use commit stored in the index instead of the one stored in the submodule HEAD"), OPT_CACHED),
		OPT_BIT(0, "recursive", &info.flags, N_("recurse into nested submodules"), OPT_RECURSIVE),
		OPT_END()
	};
	ধ্রুব বর্ণ *ধ্রুব git_submodule_helper_usage[] = {
		N_("git submodule status [--quiet] [--cached] [--recursive] [<path>...]"),
		রিক্ত
	};
	পূর্ণাঙ্ক ret = 1;

	argc = parse_options(argc, argv, prefix, module_status_options,
			     git_submodule_helper_usage, 0);

	যদি (module_list_compute(argv, prefix, &pathspec, &list) < 0)
		গচ্ছ cleanup;

	info.prefix = prefix;
	যদি (quiet)
		info.flags |= OPT_QUIET;

	for_each_listed_submodule(&list, status_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	প্রত্যাগম ret;
}

সংরচনা module_cb {
	unsigned পূর্ণাঙ্ক mod_src;
	unsigned পূর্ণাঙ্ক mod_dst;
	সংরচনা object_id oid_src;
	সংরচনা object_id oid_dst;
	বর্ণ status;
	বর্ণ *sm_path;
};
#define MODULE_CB_INIT { 0 }

স্থির শূন্য module_cb_release(সংরচনা module_cb *mcb)
{
	মুক্ত(mcb->sm_path);
}

সংরচনা module_cb_list {
	সংরচনা module_cb **entries;
	পূর্ণাঙ্ক alloc, nr;
};
#define MODULE_CB_LIST_INIT { 0 }

স্থির শূন্য module_cb_list_release(সংরচনা module_cb_list *mcbl)
{
	পূর্ণাঙ্ক i;

	প্রতি (i = 0; i < mcbl->nr; i++) {
		সংরচনা module_cb *mcb = mcbl->entries[i];

		module_cb_release(mcb);
		মুক্ত(mcb);
	}
	মুক্ত(mcbl->entries);
}

সংরচনা summary_cb {
	পূর্ণাঙ্ক argc;
	ধ্রুব বর্ণ **argv;
	ধ্রুব বর্ণ *prefix;
	ধ্রুব বর্ণ *super_prefix;
	unsigned পূর্ণাঙ্ক cached: 1;
	unsigned পূর্ণাঙ্ক for_status: 1;
	unsigned পূর্ণাঙ্ক files: 1;
	পূর্ণাঙ্ক summary_limit;
};
#define SUMMARY_CB_INIT { 0 }

গণনা diff_cmd {
	DIFF_INDEX,
	DIFF_FILES
};

স্থির বর্ণ *verify_submodule_committish(ধ্রুব বর্ণ *sm_path,
					 ধ্রুব বর্ণ *committish)
{
	সংরচনা child_process cp_rev_parse = CHILD_PROCESS_INIT;
	সংরচনা strbuf result = STRBUF_INIT;

	cp_rev_parse.git_cmd = 1;
	cp_rev_parse.dir = sm_path;
	prepare_submodule_repo_env(&cp_rev_parse.env);
	strvec_pushl(&cp_rev_parse.args, "rev-parse", "-q", "--short", রিক্ত);
	strvec_pushf(&cp_rev_parse.args, "%s^0", committish);
	strvec_ঠেলো(&cp_rev_parse.args, "--");

	যদি (capture_command(&cp_rev_parse, &result, 0))
		প্রত্যাগম রিক্ত;

	strbuf_trim_trailing_newline(&result);
	প্রত্যাগম strbuf_detach(&result, রিক্ত);
}

স্থির শূন্য print_submodule_summary(সংরচনা summary_cb *info, ধ্রুব বর্ণ *errmsg,
				    পূর্ণাঙ্ক total_commits, ধ্রুব বর্ণ *displaypath,
				    ধ্রুব বর্ণ *src_abbrev, ধ্রুব বর্ণ *dst_abbrev,
				    সংরচনা module_cb *p)
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

	যদি (total_commits < 0)
		printf(":\n");
	অন্যথা
		printf(" (%d):\n", total_commits);

	যদি (errmsg) {
		printf(_("%s"), errmsg);
	} অন্যথা যদি (total_commits > 0) {
		সংরচনা child_process cp_log = CHILD_PROCESS_INIT;

		cp_log.git_cmd = 1;
		cp_log.dir = p->sm_path;
		prepare_submodule_repo_env(&cp_log.env);
		strvec_pushl(&cp_log.args, "log", রিক্ত);

		যদি (S_ISGITLINK(p->mod_src) && S_ISGITLINK(p->mod_dst)) {
			যদি (info->summary_limit > 0)
				strvec_pushf(&cp_log.args, "-%d",
					     info->summary_limit);

			strvec_pushl(&cp_log.args, "--pretty=  %m %s",
				     "--first-parent", রিক্ত);
			strvec_pushf(&cp_log.args, "%s...%s",
				     src_abbrev, dst_abbrev);
		} অন্যথা যদি (S_ISGITLINK(p->mod_dst)) {
			strvec_pushl(&cp_log.args, "--pretty=  > %s",
				     "-1", dst_abbrev, রিক্ত);
		} অন্যথা {
			strvec_pushl(&cp_log.args, "--pretty=  < %s",
				     "-1", src_abbrev, রিক্ত);
		}
		run_command(&cp_log);
	}
	printf("\n");
}

স্থির শূন্য generate_submodule_summary(সংরচনা summary_cb *info,
				       সংরচনা module_cb *p)
{
	বর্ণ *displaypath, *src_abbrev = রিক্ত, *dst_abbrev;
	পূর্ণাঙ্ক missing_src = 0, missing_dst = 0;
	সংরচনা strbuf errmsg = STRBUF_INIT;
	পূর্ণাঙ্ক total_commits = -1;

	যদি (!info->cached && oideq(&p->oid_dst, null_oid(the_hash_algo))) {
		যদি (S_ISGITLINK(p->mod_dst)) {
			সংরচনা ref_store *refs = repo_get_submodule_ref_store(the_repository,
									      p->sm_path);

			যদি (refs)
				refs_head_ref(refs, handle_submodule_head_ref, &p->oid_dst);
		} অন্যথা যদি (S_ISLNK(p->mod_dst) || S_ISREG(p->mod_dst)) {
			সংরচনা stat st;
			পূর্ণাঙ্ক fd = open(p->sm_path, O_RDONLY);

			যদি (fd < 0 || fstat(fd, &st) < 0 ||
			    index_fd(the_repository->index, &p->oid_dst, fd, &st, OBJ_BLOB,
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
		সংরচনা child_process cp_rev_list = CHILD_PROCESS_INIT;
		সংরচনা strbuf sb_rev_list = STRBUF_INIT;

		strvec_pushl(&cp_rev_list.args, "rev-list",
			     "--first-parent", "--count", রিক্ত);
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
			total_commits = atoi(sb_rev_list.buf);

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

	print_submodule_summary(info, errmsg.len ? errmsg.buf : রিক্ত,
				total_commits, displaypath, src_abbrev,
				dst_abbrev, p);

	মুক্ত(displaypath);
	মুক্ত(src_abbrev);
	মুক্ত(dst_abbrev);
	strbuf_release(&errmsg);
}

স্থির শূন্য prepare_submodule_summary(সংরচনা summary_cb *info,
				      সংরচনা module_cb_list *list)
{
	পূর্ণাঙ্ক i;
	প্রতি (i = 0; i < list->nr; i++) {
		ধ্রুব সংরচনা submodule *sub;
		সংরচনা module_cb *p = list->entries[i];
		সংরচনা strbuf sm_gitdir = STRBUF_INIT;

		যদি (p->status == 'D' || p->status == 'T') {
			generate_submodule_summary(info, p);
			অনুবর্ত;
		}

		যদি (info->for_status && p->status != 'A' &&
		    (sub = submodule_from_path(the_repository,
					       null_oid(the_hash_algo), p->sm_path))) {
			বর্ণ *config_চাবি = রিক্ত;
			ধ্রুব বর্ণ *মান;
			পূর্ণাঙ্ক ignore_all = 0;

			config_চাবি = xstrfmt("submodule.%s.ignore",
					     sub->name);
			যদি (!repo_config_get_string_tmp(the_repository, config_চাবি, &মান))
				ignore_all = !strcmp(মান, "all");
			অন্যথা যদি (sub->ignore)
				ignore_all = !strcmp(sub->ignore, "all");

			মুক্ত(config_চাবি);
			যদি (ignore_all)
				অনুবর্ত;
		}

		/* Also show added or modified modules which are checked out */
		strbuf_addstr(&sm_gitdir, p->sm_path);
		যদি (is_nonbare_repository_dir(&sm_gitdir))
			generate_submodule_summary(info, p);
		strbuf_release(&sm_gitdir);
	}
}

স্থির শূন্য submodule_summary_callback(সংরচনা diff_queue_struct *q,
				       সংরচনা diff_options *options UNUSED,
				       শূন্য *data)
{
	পূর্ণাঙ্ক i;
	সংরচনা module_cb_list *list = data;
	প্রতি (i = 0; i < q->nr; i++) {
		সংরচনা diff_filepair *p = q->queue[i];
		সংরচনা module_cb *temp;

		যদি (!S_ISGITLINK(p->one->mode) && !S_ISGITLINK(p->two->mode))
			অনুবর্ত;
		temp = xmalloc(পরিমাণ(*temp));
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

স্থির ধ্রুব বর্ণ *get_diff_cmd(গণনা diff_cmd diff_cmd)
{
	বিকল্প (diff_cmd) {
	পক্ষ DIFF_INDEX: প্রত্যাগম "diff-index";
	পক্ষ DIFF_FILES: প্রত্যাগম "diff-files";
	স্বভাব: BUG("bad diff_cmd value %d", diff_cmd);
	}
}

স্থির পূর্ণাঙ্ক compute_summary_module_list(সংরচনা object_id *head_oid,
				       সংরচনা summary_cb *info,
				       গণনা diff_cmd diff_cmd)
{
	সংরচনা strvec diff_args = STRVEC_INIT;
	সংরচনা rev_info rev;
	সংরচনা module_cb_list list = MODULE_CB_LIST_INIT;
	পূর্ণাঙ্ক ret = 0;

	strvec_ঠেলো(&diff_args, get_diff_cmd(diff_cmd));
	যদি (info->cached)
		strvec_ঠেলো(&diff_args, "--cached");
	strvec_pushl(&diff_args, "--ignore-submodules=dirty", "--raw", রিক্ত);
	যদি (head_oid)
		strvec_ঠেলো(&diff_args, oid_to_hex(head_oid));
	strvec_ঠেলো(&diff_args, "--");
	যদি (info->argc)
		strvec_pushv(&diff_args, info->argv);

	repo_config(the_repository, git_diff_basic_config, রিক্ত);
	repo_init_revisions(the_repository, &rev, info->prefix);
	rev.abbrev = 0;
	precompose_argv_prefix(diff_args.nr, diff_args.v, রিক্ত);
	setup_revisions_from_strvec(&diff_args, &rev, রিক্ত);
	rev.diffopt.output_format = DIFF_FORMAT_NO_OUTPUT | DIFF_FORMAT_CALLBACK;
	rev.diffopt.format_callback = submodule_summary_callback;
	rev.diffopt.format_callback_data = &list;

	যদি (!info->cached) {
		যদি (diff_cmd == DIFF_INDEX)
			setup_work_বৃক্ষ(the_repository);
		যদি (repo_read_index_preload(the_repository, &rev.diffopt.pathspec, 0) < 0) {
			perror("repo_read_index_preload");
			ret = -1;
			গচ্ছ cleanup;
		}
	} অন্যথা যদি (repo_read_index(the_repository) < 0) {
		perror("repo_read_cache");
		ret = -1;
		গচ্ছ cleanup;
	}

	যদি (diff_cmd == DIFF_INDEX)
		run_diff_index(&rev, info->cached ? DIFF_INDEX_CACHED : 0);
	অন্যথা
		run_diff_files(&rev, 0);
	prepare_submodule_summary(info, &list);
cleanup:
	strvec_clear(&diff_args);
	release_revisions(&rev);
	module_cb_list_release(&list);
	প্রত্যাগম ret;
}

স্থির পূর্ণাঙ্ক module_summary(পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv, ধ্রুব বর্ণ *prefix,
			  সংরচনা repository *repo UNUSED)
{
	সংরচনা summary_cb info = SUMMARY_CB_INIT;
	পূর্ণাঙ্ক cached = 0;
	পূর্ণাঙ্ক for_status = 0;
	পূর্ণাঙ্ক files = 0;
	পূর্ণাঙ্ক summary_limit = -1;
	গণনা diff_cmd diff_cmd = DIFF_INDEX;
	সংরচনা object_id head_oid;
	পূর্ণাঙ্ক ret;
	সংরচনা option module_summary_options[] = {
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
	ধ্রুব বর্ণ *ধ্রুব git_submodule_helper_usage[] = {
		N_("git submodule summary [<options>] [<commit>] [--] [<path>]"),
		রিক্ত
	};

	argc = parse_options(argc, argv, prefix, module_summary_options,
			     git_submodule_helper_usage, 0);

	যদি (!summary_limit)
		প্রত্যাগম 0;

	যদি (!repo_get_oid(the_repository, argc ? argv[0] : "HEAD", &head_oid)) {
		যদি (argc) {
			argv++;
			argc--;
		}
	} অন্যথা যদি (!argc || !strcmp(argv[0], "HEAD")) {
		/* before the first commit: compare with an empty tree */
		oidcpy(&head_oid, the_hash_algo->empty_বৃক্ষ);
		যদি (argc) {
			argv++;
			argc--;
		}
	} অন্যথা {
		যদি (repo_get_oid(the_repository, "HEAD", &head_oid))
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

	ret = compute_summary_module_list((diff_cmd == DIFF_INDEX) ? &head_oid : রিক্ত,
					  &info, diff_cmd);
	প্রত্যাগম ret;
}

স্থির পূর্ণাঙ্ক module_gitdir(পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv, ধ্রুব বর্ণ *prefix UNUSED,
			 সংরচনা repository *repo)
{
	সংরচনা strbuf gitdir = STRBUF_INIT;

	যদি (argc != 2)
		usage(_("git submodule--helper gitdir <name>"));

	submodule_name_to_gitdir(&gitdir, repo, argv[1]);

	printf("%s\n", gitdir.buf);

	strbuf_release(&gitdir);
	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক module_migrate(পূর্ণাঙ্ক argc UNUSED, ধ্রুব বর্ণ **argv UNUSED,
			  ধ্রুব বর্ণ *prefix UNUSED, সংরচনা repository *repo)
{
	সংরচনা strbuf module_dir = STRBUF_INIT;
	DIR *dir;
	সংরচনা dirent *de;
	পূর্ণাঙ্ক repo_version = 0;

	repo_git_path_append(repo, &module_dir, "modules/");

	dir = opendir(module_dir.buf);
	যদি (!dir)
		die(_("could not open '%s'"), module_dir.buf);

	যাবত্ ((de = readdir(dir))) {
		সংরচনা strbuf gitdir_path = STRBUF_INIT;
		বর্ণ *চাবি;
		ধ্রুব বর্ণ *মান;

		যদি (is_dot_or_dotdot(de->d_name))
			অনুবর্ত;

		strbuf_addf(&gitdir_path, "%s/%s", module_dir.buf, de->d_name);
		যদি (!is_git_directory(gitdir_path.buf)) {
			strbuf_release(&gitdir_path);
			অনুবর্ত;
		}
		strbuf_release(&gitdir_path);

		চাবি = xstrfmt("submodule.%s.gitdir", de->d_name);
		যদি (!repo_config_get_string_tmp(repo, চাবি, &মান)) {
			/* Already has a gitdir config, nothing to do. */
			মুক্ত(চাবি);
			অনুবর্ত;
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

	প্রত্যাগম 0;
}

সংরচনা sync_cb {
	ধ্রুব বর্ণ *prefix;
	ধ্রুব বর্ণ *super_prefix;
	unsigned পূর্ণাঙ্ক flags;
};
#define SYNC_CB_INIT { 0 }

স্থির শূন্য sync_submodule(ধ্রুব বর্ণ *path, ধ্রুব বর্ণ *prefix,
			   ধ্রুব বর্ণ *super_prefix, unsigned পূর্ণাঙ্ক flags)
{
	ধ্রুব সংরচনা submodule *sub;
	বর্ণ *remote_চাবি = রিক্ত;
	বর্ণ *sub_origin_url, *super_config_url, *displaypath, *default_remote;
	সংরচনা strbuf sb = STRBUF_INIT;
	বর্ণ *sub_config_path = রিক্ত;
	পূর্ণাঙ্ক code;

	যদি (!is_submodule_active(the_repository, path))
		প্রত্যাগম;

	যদি (validate_submodule_path(path) < 0)
		die(রিক্ত);

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);

	যদি (sub && sub->url) {
		যদি (starts_with_dot_dot_slash(sub->url) ||
		    starts_with_dot_slash(sub->url)) {
			বর্ণ *up_path = get_up_path(path);

			sub_origin_url = resolve_relative_url(sub->url, up_path, 1);
			super_config_url = resolve_relative_url(sub->url, রিক্ত, 1);
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

	যদি (!is_submodule_populated_gently(path, রিক্ত))
		গচ্ছ cleanup;

	strbuf_reset(&sb);
	code = get_default_remote_submodule(path, &default_remote);
	যদি (code)
		exit(code);

	remote_চাবি = xstrfmt("remote.%s.url", default_remote);
	মুক্ত(default_remote);

	submodule_to_gitdir(the_repository, &sb, path);
	strbuf_addstr(&sb, "/config");

	যদি (repo_config_set_in_file_gently(the_repository, sb.buf, remote_চাবি, রিক্ত, sub_origin_url))
		die(_("failed to update remote for submodule '%s'"),
		      path);

	যদি (flags & OPT_RECURSIVE) {
		সংরচনা child_process cpr = CHILD_PROCESS_INIT;

		cpr.git_cmd = 1;
		cpr.dir = path;
		prepare_submodule_repo_env(&cpr.env);

		strvec_pushl(&cpr.args, "submodule--helper", "sync",
			     "--recursive", রিক্ত);
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

স্থির শূন্য sync_submodule_cb(ধ্রুব সংরচনা cache_entry *list_item, শূন্য *cb_data)
{
	সংরচনা sync_cb *info = cb_data;

	sync_submodule(list_item->name, info->prefix, info->super_prefix,
		       info->flags);
}

স্থির পূর্ণাঙ্ক module_sync(পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv, ধ্রুব বর্ণ *prefix,
		       সংরচনা repository *repo UNUSED)
{
	সংরচনা sync_cb info = SYNC_CB_INIT;
	সংরচনা pathspec pathspec = { 0 };
	সংরচনা module_list list = MODULE_LIST_INIT;
	পূর্ণাঙ্ক quiet = 0;
	পূর্ণাঙ্ক recursive = 0;
	সংরচনা option module_sync_options[] = {
		OPT__SUPER_PREFIX(&info.super_prefix),
		OPT__QUIET(&quiet, N_("suppress output of synchronizing submodule url")),
		OPT_BOOL(0, "recursive", &recursive,
			N_("recurse into nested submodules")),
		OPT_END()
	};
	ধ্রুব বর্ণ *ধ্রুব git_submodule_helper_usage[] = {
		N_("git submodule sync [--quiet] [--recursive] [<path>]"),
		রিক্ত
	};
	পূর্ণাঙ্ক ret = 1;

	argc = parse_options(argc, argv, prefix, module_sync_options,
			     git_submodule_helper_usage, 0);

	যদি (module_list_compute(argv, prefix, &pathspec, &list) < 0)
		গচ্ছ cleanup;

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
	প্রত্যাগম ret;
}

সংরচনা deinit_cb {
	ধ্রুব বর্ণ *prefix;
	unsigned পূর্ণাঙ্ক flags;
};
#define DEINIT_CB_INIT { 0 }

স্থির শূন্য deinit_submodule(ধ্রুব বর্ণ *path, ধ্রুব বর্ণ *prefix,
			     unsigned পূর্ণাঙ্ক flags)
{
	ধ্রুব সংরচনা submodule *sub;
	বর্ণ *displaypath = রিক্ত;
	সংরচনা child_process cp_config = CHILD_PROCESS_INIT;
	সংরচনা strbuf sb_config = STRBUF_INIT;
	বর্ণ *sub_git_dir = xstrfmt("%s/.git", path);

	যদি (validate_submodule_path(path) < 0)
		die(রিক্ত);

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);

	যদি (!sub || !sub->name)
		গচ্ছ cleanup;

	displaypath = get_submodule_displaypath(path, prefix, রিক্ত);

	/* remove the submodule work tree (unless the user already did it) */
	যদি (is_directory(path)) {
		সংরচনা strbuf sb_rm = STRBUF_INIT;
		ধ্রুব বর্ণ *format;

		যদি (is_directory(sub_git_dir)) {
			যদি (!(flags & OPT_QUIET))
				warning(_("Submodule work tree '%s' contains a .git "
					  "directory. This will be replaced with a "
					  ".git file by using absorbgitdirs."),
					displaypath);

			absorb_git_dir_into_superproject(path, রিক্ত);

		}

		যদি (!(flags & OPT_FORCE)) {
			সংরচনা child_process cp_rm = CHILD_PROCESS_INIT;

			cp_rm.git_cmd = 1;
			strvec_pushl(&cp_rm.args, "rm", "-qn",
				     path, রিক্ত);

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
	strvec_pushl(&cp_config.args, "config", "--get-regexp", রিক্ত);
	strvec_pushf(&cp_config.args, "submodule.%s\\.", sub->name);

	/* remove the .git/config entries (unless the user already did it) */
	যদি (!capture_command(&cp_config, &sb_config, 0) && sb_config.len) {
		বর্ণ *sub_চাবি = xstrfmt("submodule.%s", sub->name);

		/*
		 * remove the whole section so we have a clean state when
		 * the user later decides to init this submodule again
		 */
		repo_config_rename_section_in_file(the_repository, রিক্ত, sub_চাবি, রিক্ত);
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

স্থির শূন্য deinit_submodule_cb(ধ্রুব সংরচনা cache_entry *list_item,
				শূন্য *cb_data)
{
	সংরচনা deinit_cb *info = cb_data;
	deinit_submodule(list_item->name, info->prefix, info->flags);
}

স্থির পূর্ণাঙ্ক module_deinit(পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv, ধ্রুব বর্ণ *prefix,
			 সংরচনা repository *repo UNUSED)
{
	সংরচনা deinit_cb info = DEINIT_CB_INIT;
	সংরচনা pathspec pathspec = { 0 };
	সংরচনা module_list list = MODULE_LIST_INIT;
	পূর্ণাঙ্ক quiet = 0;
	পূর্ণাঙ্ক force = 0;
	পূর্ণাঙ্ক all = 0;
	সংরচনা option module_deinit_options[] = {
		OPT__QUIET(&quiet, N_("suppress submodule status output")),
		OPT__FORCE(&force, N_("remove submodule working trees even if they contain local changes"), 0),
		OPT_BOOL(0, "all", &all, N_("unregister all submodules")),
		OPT_END()
	};
	ধ্রুব বর্ণ *ধ্রুব git_submodule_helper_usage[] = {
		N_("git submodule deinit [--quiet] [-f | --force] [--all | [--] [<path>...]]"),
		রিক্ত
	};
	পূর্ণাঙ্ক ret = 1;

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
		গচ্ছ cleanup;

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
	প্রত্যাগম ret;
}

সংরচনা module_clone_data {
	ধ্রুব বর্ণ *prefix;
	ধ্রুব বর্ণ *path;
	ধ্রুব বর্ণ *name;
	ধ্রুব বর্ণ *url;
	পূর্ণাঙ্ক depth;
	সংরচনা list_objects_filter_options *filter_options;
	গণনা ref_storage_format ref_storage_format;
	unsigned পূর্ণাঙ্ক quiet: 1;
	unsigned পূর্ণাঙ্ক progress: 1;
	unsigned পূর্ণাঙ্ক dissociate: 1;
	unsigned পূর্ণাঙ্ক require_init: 1;
	পূর্ণাঙ্ক single_branch;
};
#define MODULE_CLONE_DATA_INIT { \
	.single_branch = -1, \
	.ref_storage_format = REF_STORAGE_FORMAT_UNKNOWN, \
}

সংরচনা submodule_alternate_setup {
	ধ্রুব বর্ণ *submodule_name;
	গণনা SUBMODULE_ALTERNATE_ERROR_MODE {
		SUBMODULE_ALTERNATE_ERROR_DIE,
		SUBMODULE_ALTERNATE_ERROR_INFO,
		SUBMODULE_ALTERNATE_ERROR_IGNORE
	} error_mode;
	সংরচনা string_list *reference;
};
#define SUBMODULE_ALTERNATE_SETUP_INIT { \
	.error_mode = SUBMODULE_ALTERNATE_ERROR_IGNORE, \
}

স্থির ধ্রুব বর্ণ alternate_error_advice[] = N_(
"An alternate computed from a superproject's alternate is invalid.\n"
"To allow Git to clone without an alternate in such a case, set\n"
"submodule.alternateErrorStrategy to 'info' or, equivalently, clone with\n"
"'--reference-if-able' instead of '--reference'."
);

স্থির পূর্ণাঙ্ক add_possible_reference_from_superproject(
		সংরচনা odb_files_dir *alt_odb, শূন্য *sas_cb)
{
	সংরচনা submodule_alternate_setup *sas = sas_cb;
	size_t len;

	/*
	 * If the alternate object store is another repository, try the
	 * standard layout with .git/(modules/<name>)+/objects
	 */
	যদি (strip_suffix(alt_odb->abspath, "/objects", &len)) {
		সংরচনা repository alternate;
		বর্ণ *sm_alternate;
		সংরচনা strbuf sb = STRBUF_INIT;
		সংরচনা strbuf err = STRBUF_INIT;
		strbuf_add(&sb, alt_odb->abspath, len);

		যদি (repo_init(&alternate, sb.buf, রিক্ত) < 0)
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
			বর্ণ *p = strbuf_detach(&sb, রিক্ত);

			string_list_append(sas->reference, p)->util = p;
			মুক্ত(sm_alternate);
		} অন্যথা {
			বিকল্প (sas->error_mode) {
			পক্ষ SUBMODULE_ALTERNATE_ERROR_DIE:
				যদি (advice_enabled(ADVICE_SUBMODULE_ALTERNATE_ERROR_STRATEGY_DIE))
					advise(_(alternate_error_advice));
				die(_("submodule '%s' cannot add alternate: %s"),
				    sas->submodule_name, err.buf);
			পক্ষ SUBMODULE_ALTERNATE_ERROR_INFO:
				fprintf_ln(stderr, _("submodule '%s' cannot add alternate: %s"),
					sas->submodule_name, err.buf);
			পক্ষ SUBMODULE_ALTERNATE_ERROR_IGNORE:
				; /* nothing */
			}
		}

		strbuf_release(&err);
		strbuf_release(&sb);
	}

	প্রত্যাগম 0;
}

স্থির শূন্য prepare_possible_alternates(ধ্রুব বর্ণ *sm_name,
		সংরচনা string_list *reference)
{
	বর্ণ *sm_alternate = রিক্ত, *error_strategy = রিক্ত;
	সংরচনা submodule_alternate_setup sas = SUBMODULE_ALTERNATE_SETUP_INIT;

	repo_config_get_string(the_repository, "submodule.alternateLocation", &sm_alternate);
	যদি (!sm_alternate)
		প্রত্যাগম;

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

স্থির বর্ণ *clone_submodule_sm_gitdir(ধ্রুব বর্ণ *name)
{
	সংরচনা strbuf sb = STRBUF_INIT;
	বর্ণ *sm_gitdir;

	submodule_name_to_gitdir(&sb, the_repository, name);
	sm_gitdir = absolute_pathdup(sb.buf);
	strbuf_release(&sb);

	প্রত্যাগম sm_gitdir;
}

স্থির পূর্ণাঙ্ক dir_contains_only_dotgit(ধ্রুব বর্ণ *path)
{
	DIR *dir = opendir(path);
	সংরচনা dirent *e;
	পূর্ণাঙ্ক ret = 1;

	যদি (!dir)
		প্রত্যাগম 0;

	e = readdir_skip_dot_and_dotdot(dir);
	যদি (!e)
		ret = 0;
	অন্যথা যদি (strcmp(DEFAULT_GIT_DIR_ENVIRONMENT, e->d_name) ||
		 (e = readdir_skip_dot_and_dotdot(dir))) {
		error("unexpected item '%s' in '%s'", e->d_name, path);
		ret = 0;
	}

	closedir(dir);
	প্রত্যাগম ret;
}

স্থির পূর্ণাঙ্ক clone_submodule(ধ্রুব সংরচনা module_clone_data *clone_data,
			   সংরচনা string_list *reference)
{
	বর্ণ *p;
	বর্ণ *sm_gitdir = clone_submodule_sm_gitdir(clone_data->name);
	বর্ণ *sm_alternate = রিক্ত, *error_strategy = রিক্ত;
	সংরচনা stat st;
	সংরচনা child_process cp = CHILD_PROCESS_INIT;
	ধ্রুব বর্ণ *clone_data_path = clone_data->path;
	বর্ণ *to_free = রিক্ত;

	যদি (validate_submodule_path(clone_data_path) < 0)
		die(রিক্ত);

	যদি (!is_absolute_path(clone_data->path))
		clone_data_path = to_free = xstrfmt("%s/%s", repo_get_work_বৃক্ষ(the_repository),
						    clone_data->path);

	যদি (!file_exists(sm_gitdir)) {
		যদি (clone_data->require_init && !stat(clone_data_path, &st) &&
		    !is_empty_dir(clone_data_path))
			die(_("directory not empty: '%s'"), clone_data_path);

		যদি (safe_create_leading_directories_const(the_repository, sm_gitdir) < 0)
			die(_("could not create directory '%s'"), sm_gitdir);

		prepare_possible_alternates(clone_data->name, reference);

		strvec_ঠেলো(&cp.args, "clone");
		strvec_ঠেলো(&cp.args, "--no-checkout");
		যদি (clone_data->quiet)
			strvec_ঠেলো(&cp.args, "--quiet");
		যদি (clone_data->progress)
			strvec_ঠেলো(&cp.args, "--progress");
		যদি (clone_data->depth > 0)
			strvec_pushf(&cp.args, "--depth=%d", clone_data->depth);
		যদি (reference->nr) {
			সংরচনা string_list_item *item;

			for_each_string_list_item(item, reference)
				strvec_pushl(&cp.args, "--reference",
					     item->string, রিক্ত);
		}
		যদি (clone_data->ref_storage_format != REF_STORAGE_FORMAT_UNKNOWN)
			strvec_pushf(&cp.args, "--ref-storage-format=%s",
				     ref_storage_format_to_name(clone_data->ref_storage_format));
		যদি (clone_data->dissociate)
			strvec_ঠেলো(&cp.args, "--dissociate");
		যদি (sm_gitdir && *sm_gitdir)
			strvec_pushl(&cp.args, "--separate-git-dir", sm_gitdir, রিক্ত);
		যদি (clone_data->filter_options && clone_data->filter_options->choice)
			strvec_pushf(&cp.args, "--filter=%s",
				     expand_list_objects_filter_spec(
					     clone_data->filter_options));
		যদি (clone_data->single_branch >= 0)
			strvec_ঠেলো(&cp.args, clone_data->single_branch ?
				    "--single-branch" :
				    "--no-single-branch");

		strvec_ঠেলো(&cp.args, "--");
		strvec_ঠেলো(&cp.args, clone_data->url);
		strvec_ঠেলো(&cp.args, clone_data_path);

		cp.git_cmd = 1;
		prepare_submodule_repo_env(&cp.env);
		cp.no_stdin = 1;

		যদি(run_command(&cp))
			die(_("clone of '%s' into submodule path '%s' failed"),
			    clone_data->url, clone_data_path);

		যদি (clone_data->require_init && !stat(clone_data_path, &st) &&
		    !dir_contains_only_dotgit(clone_data_path)) {
			বর্ণ *dot_git = xstrfmt("%s/.git", clone_data_path);
			unlink(dot_git);
			মুক্ত(dot_git);
			die(_("directory not empty: '%s'"), clone_data_path);
		}
	} অন্যথা {
		বর্ণ *path;

		যদি (clone_data->require_init && !stat(clone_data_path, &st) &&
		    !is_empty_dir(clone_data_path))
			die(_("directory not empty: '%s'"), clone_data_path);
		যদি (safe_create_leading_directories_const(the_repository, clone_data_path) < 0)
			die(_("could not create directory '%s'"), clone_data_path);
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
	যদি (validate_submodule_git_dir(sm_gitdir, clone_data->name) < 0) {
		বর্ণ *head = xstrfmt("%s/HEAD", sm_gitdir);
		unlink(head);
		মুক্ত(head);
		die(_("refusing to create/use '%s' in another submodule's git dir. "
		      "Enabling extensions.submodulePathConfig should fix this."),
		    sm_gitdir);
	}

	connect_work_বৃক্ষ_and_git_dir(clone_data_path, sm_gitdir, 0);

	p = repo_submodule_path(the_repository, clone_data_path, "config");
	যদি (!p)
		die(_("could not get submodule directory for '%s'"), clone_data_path);

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
	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক module_clone(পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv, ধ্রুব বর্ণ *prefix,
			সংরচনা repository *repo UNUSED)
{
	পূর্ণাঙ্ক dissociate = 0, quiet = 0, progress = 0, require_init = 0;
	সংরচনা module_clone_data clone_data = MODULE_CLONE_DATA_INIT;
	সংরচনা string_list reference = STRING_LIST_INIT_NODUP;
	সংরচনা list_objects_filter_options filter_options =
		LIST_OBJECTS_FILTER_INIT;
	ধ্রুব বর্ণ *ref_storage_format = রিক্ত;

	সংরচনা option module_clone_options[] = {
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
	ধ্রুব বর্ণ *ধ্রুব git_submodule_helper_usage[] = {
		N_("git submodule--helper clone [--prefix=<path>] [--quiet] "
		   "[--reference <repository>] [--name <name>] [--depth <depth>] "
		   "[--single-branch] [--filter <filter-spec>] "
		   "--url <url> --path <path>"),
		রিক্ত
	};

	argc = parse_options(argc, argv, prefix, module_clone_options,
			     git_submodule_helper_usage, 0);

	যদি (ref_storage_format) {
		clone_data.ref_storage_format = ref_storage_format_by_name(ref_storage_format);
		যদি (clone_data.ref_storage_format == REF_STORAGE_FORMAT_UNKNOWN)
			die(_("unknown ref storage format '%s'"), ref_storage_format);
	}
	clone_data.dissociate = !!dissociate;
	clone_data.quiet = !!quiet;
	clone_data.progress = !!progress;
	clone_data.require_init = !!require_init;
	clone_data.filter_options = &filter_options;

	যদি (argc || !clone_data.url || !clone_data.path || !*(clone_data.path))
		usage_with_options(git_submodule_helper_usage,
				   module_clone_options);

	clone_submodule(&clone_data, &reference);
	list_objects_filter_release(&filter_options);
	string_list_clear(&reference, 1);
	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক determine_submodule_update_strategy(সংরচনা repository *r,
					       পূর্ণাঙ্ক just_cloned,
					       ধ্রুব বর্ণ *path,
					       গণনা submodule_update_type update,
					       সংরচনা submodule_update_strategy *out)
{
	ধ্রুব সংরচনা submodule *sub = submodule_from_path(r, null_oid(the_hash_algo), path);
	বর্ণ *চাবি;
	ধ্রুব বর্ণ *val;
	পূর্ণাঙ্ক ret;

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
			গচ্ছ cleanup;
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
	প্রত্যাগম ret;
}

সংরচনা update_clone_data {
	ধ্রুব সংরচনা submodule *sub;
	সংরচনা object_id oid;
	unsigned just_cloned;
};

সংরচনা submodule_update_clone {
	/* index into 'update_data.list', the list of submodules to look into for cloning */
	পূর্ণাঙ্ক current;

	/* configuration parameters which are passed on to the children */
	ধ্রুব সংরচনা update_data *update_data;

	/* to be consumed by update_submodule() */
	সংরচনা update_clone_data *update_clone;
	পূর্ণাঙ্ক update_clone_nr; পূর্ণাঙ্ক update_clone_alloc;

	/* If we want to stop as fast as possible and return an error */
	unsigned quickstop : 1;

	/* failed clones to be retried again */
	ধ্রুব সংরচনা cache_entry **failed_clones;
	পূর্ণাঙ্ক failed_clones_nr, failed_clones_alloc;
};
#define SUBMODULE_UPDATE_CLONE_INIT { 0 }

স্থির শূন্য submodule_update_clone_release(সংরচনা submodule_update_clone *suc)
{
	মুক্ত(suc->update_clone);
	মুক্ত(suc->failed_clones);
}

সংরচনা update_data {
	ধ্রুব বর্ণ *prefix;
	ধ্রুব বর্ণ *super_prefix;
	বর্ণ *displaypath;
	গণনা submodule_update_type update_default;
	সংরচনা object_id suboid;
	সংরচনা string_list references;
	সংরচনা submodule_update_strategy update_strategy;
	সংরচনা list_objects_filter_options *filter_options;
	সংরচনা module_list list;
	গণনা ref_storage_format ref_storage_format;
	পূর্ণাঙ্ক depth;
	পূর্ণাঙ্ক max_jobs;
	পূর্ণাঙ্ক single_branch;
	পূর্ণাঙ্ক recommend_shallow;
	unsigned পূর্ণাঙ্ক require_init;
	unsigned পূর্ণাঙ্ক force;
	unsigned পূর্ণাঙ্ক quiet;
	unsigned পূর্ণাঙ্ক nofetch;
	unsigned পূর্ণাঙ্ক remote;
	unsigned পূর্ণাঙ্ক progress;
	unsigned পূর্ণাঙ্ক dissociate;
	unsigned পূর্ণাঙ্ক init;
	unsigned পূর্ণাঙ্ক warn_if_uninitialized;
	unsigned পূর্ণাঙ্ক recursive;

	/* copied over from update_clone_data */
	সংরচনা object_id oid;
	unsigned পূর্ণাঙ্ক just_cloned;
	ধ্রুব বর্ণ *sm_path;
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

স্থির শূন্য update_data_release(সংরচনা update_data *ud)
{
	মুক্ত(ud->displaypath);
	submodule_update_strategy_release(&ud->update_strategy);
	module_list_release(&ud->list);
}

স্থির শূন্য next_submodule_warn_missing(সংরচনা submodule_update_clone *suc,
		সংরচনা strbuf *out, ধ্রুব বর্ণ *displaypath)
{
	/*
	 * Only mention uninitialized submodules when their
	 * paths have been specified.
	 */
	যদি (suc->update_data->warn_if_uninitialized) {
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
স্থির পূর্ণাঙ্ক prepare_to_clone_next_submodule(ধ্রুব সংরচনা cache_entry *ce,
					   সংরচনা child_process *child,
					   সংরচনা submodule_update_clone *suc,
					   সংরচনা strbuf *out)
{
	ধ্রুব সংরচনা submodule *sub = রিক্ত;
	ধ্রুব বর্ণ *url = রিক্ত;
	ধ্রুব বর্ণ *update_string;
	গণনা submodule_update_type update_type;
	বর্ণ *চাবি;
	ধ্রুব সংরচনা update_data *ud = suc->update_data;
	বর্ণ *displaypath = get_submodule_displaypath(ce->name, ud->prefix,
						      ud->super_prefix);
	সংরচনা strbuf sb = STRBUF_INIT;
	পূর্ণাঙ্ক needs_cloning = 0;
	পূর্ণাঙ্ক need_free_url = 0;

	যদি (ce_stage(ce)) {
		strbuf_addf(out, _("Skipping unmerged submodule %s"), displaypath);
		strbuf_addch(out, '\n');
		গচ্ছ cleanup;
	}

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), ce->name);

	যদি (!sub) {
		next_submodule_warn_missing(suc, out, displaypath);
		গচ্ছ cleanup;
	}

	চাবি = xstrfmt("submodule.%s.update", sub->name);
	যদি (!repo_config_get_string_tmp(the_repository, চাবি, &update_string)) {
		update_type = parse_submodule_update_type(update_string);
	} অন্যথা {
		update_type = sub->update_strategy.type;
	}
	মুক্ত(চাবি);

	যদি (suc->update_data->update_strategy.type == SM_UPDATE_NONE
	    || (suc->update_data->update_strategy.type == SM_UPDATE_UNSPECIFIED
		&& update_type == SM_UPDATE_NONE)) {
		strbuf_addf(out, _("Skipping submodule '%s'"), displaypath);
		strbuf_addch(out, '\n');
		গচ্ছ cleanup;
	}

	/* Check if the submodule has been initialized. */
	যদি (!is_submodule_active(the_repository, ce->name)) {
		next_submodule_warn_missing(suc, out, displaypath);
		গচ্ছ cleanup;
	}

	strbuf_reset(&sb);
	strbuf_addf(&sb, "submodule.%s.url", sub->name);
	যদি (repo_config_get_string_tmp(the_repository, sb.buf, &url)) {
		যদি (sub->url && (starts_with_dot_slash(sub->url) ||
				 starts_with_dot_dot_slash(sub->url))) {
			url = resolve_relative_url(sub->url, রিক্ত, 0);
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
		গচ্ছ cleanup;

	child->git_cmd = 1;
	child->no_stdin = 1;
	child->stdout_to_stderr = 1;
	child->err = -1;
	strvec_ঠেলো(&child->args, "submodule--helper");
	strvec_ঠেলো(&child->args, "clone");
	যদি (suc->update_data->progress)
		strvec_ঠেলো(&child->args, "--progress");
	যদি (suc->update_data->quiet)
		strvec_ঠেলো(&child->args, "--quiet");
	যদি (suc->update_data->prefix)
		strvec_pushl(&child->args, "--prefix", suc->update_data->prefix, রিক্ত);
	যদি (suc->update_data->recommend_shallow && sub->recommend_shallow == 1)
		strvec_ঠেলো(&child->args, "--depth=1");
	অন্যথা যদি (suc->update_data->depth)
		strvec_pushf(&child->args, "--depth=%d", suc->update_data->depth);
	যদি (suc->update_data->filter_options && suc->update_data->filter_options->choice)
		strvec_pushf(&child->args, "--filter=%s",
			     expand_list_objects_filter_spec(suc->update_data->filter_options));
	যদি (suc->update_data->require_init)
		strvec_ঠেলো(&child->args, "--require-init");
	যদি (suc->update_data->ref_storage_format != REF_STORAGE_FORMAT_UNKNOWN)
		strvec_pushf(&child->args, "--ref-storage-format=%s",
			     ref_storage_format_to_name(suc->update_data->ref_storage_format));
	strvec_pushl(&child->args, "--path", sub->path, রিক্ত);
	strvec_pushl(&child->args, "--name", sub->name, রিক্ত);
	strvec_pushl(&child->args, "--url", url, রিক্ত);
	যদি (suc->update_data->references.nr) {
		সংরচনা string_list_item *item;

		for_each_string_list_item(item, &suc->update_data->references)
			strvec_pushl(&child->args, "--reference", item->string, রিক্ত);
	}
	যদি (suc->update_data->dissociate)
		strvec_ঠেলো(&child->args, "--dissociate");
	যদি (suc->update_data->single_branch >= 0)
		strvec_ঠেলো(&child->args, suc->update_data->single_branch ?
					      "--single-branch" :
					      "--no-single-branch");

cleanup:
	মুক্ত(displaypath);
	strbuf_release(&sb);
	যদি (need_free_url)
		মুক্ত((শূন্য*)url);

	প্রত্যাগম needs_cloning;
}

স্থির পূর্ণাঙ্ক update_clone_get_next_task(সংরচনা child_process *child,
				      সংরচনা strbuf *err,
				      শূন্য *suc_cb,
				      শূন্য **idx_task_cb)
{
	সংরচনা submodule_update_clone *suc = suc_cb;
	ধ্রুব সংরচনা cache_entry *ce;
	পূর্ণাঙ্ক index;

	প্রতি (; suc->current < suc->update_data->list.nr; suc->current++) {
		ce = suc->update_data->list.entries[suc->current];
		যদি (prepare_to_clone_next_submodule(ce, child, suc, err)) {
			পূর্ণাঙ্ক *p = xmalloc(পরিমাণ(*p));

			*p = suc->current;
			*idx_task_cb = p;
			suc->current++;
			প্রত্যাগম 1;
		}
	}

	/*
	 * The loop above tried cloning each submodule once, now try the
	 * stragglers again, which we can imagine as an extension of the
	 * entry list.
	 */
	index = suc->current - suc->update_data->list.nr;
	যদি (index < suc->failed_clones_nr) {
		পূর্ণাঙ্ক *p;

		ce = suc->failed_clones[index];
		যদি (!prepare_to_clone_next_submodule(ce, child, suc, err)) {
			suc->current ++;
			strbuf_addstr(err, "BUG: submodule considered for "
					   "cloning, doesn't need cloning "
					   "any more?\n");
			প্রত্যাগম 0;
		}
		p = xmalloc(পরিমাণ(*p));
		*p = suc->current;
		*idx_task_cb = p;
		suc->current ++;
		প্রত্যাগম 1;
	}

	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক update_clone_start_failure(সংরচনা strbuf *err UNUSED,
				      শূন্য *suc_cb,
				      শূন্য *idx_task_cb UNUSED)
{
	সংরচনা submodule_update_clone *suc = suc_cb;

	suc->quickstop = 1;
	প্রত্যাগম 1;
}

স্থির পূর্ণাঙ্ক update_clone_task_finished(পূর্ণাঙ্ক result,
				      সংরচনা strbuf *err,
				      শূন্য *suc_cb,
				      শূন্য *idx_task_cb)
{
	ধ্রুব সংরচনা cache_entry *ce;
	সংরচনা submodule_update_clone *suc = suc_cb;
	পূর্ণাঙ্ক *idxP = idx_task_cb;
	পূর্ণাঙ্ক idx = *idxP;

	মুক্ত(idxP);

	যদি (!result)
		প্রত্যাগম 0;

	যদি (idx < suc->update_data->list.nr) {
		ce  = suc->update_data->list.entries[idx];
		strbuf_addf(err, _("Failed to clone '%s'. Retry scheduled"),
			    ce->name);
		strbuf_addch(err, '\n');
		ALLOC_GROW(suc->failed_clones,
			   suc->failed_clones_nr + 1,
			   suc->failed_clones_alloc);
		suc->failed_clones[suc->failed_clones_nr++] = ce;
		প্রত্যাগম 0;
	} অন্যথা {
		idx -= suc->update_data->list.nr;
		ce  = suc->failed_clones[idx];
		strbuf_addf(err, _("Failed to clone '%s' a second time, aborting"),
			    ce->name);
		strbuf_addch(err, '\n');
		suc->quickstop = 1;
		প্রত্যাগম 1;
	}

	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক git_update_clone_config(ধ্রুব বর্ণ *var, ধ্রুব বর্ণ *মান,
				   ধ্রুব সংরচনা config_context *ctx,
				   শূন্য *cb)
{
	পূর্ণাঙ্ক *max_jobs = cb;

	যদি (!strcmp(var, "submodule.fetchjobs"))
		*max_jobs = parse_submodule_fetchjobs(var, মান, ctx->kvi);
	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক is_tip_reachable(ধ্রুব বর্ণ *path, ধ্রুব সংরচনা object_id *oid)
{
	সংরচনা child_process cp = CHILD_PROCESS_INIT;
	সংরচনা strbuf rev = STRBUF_INIT;
	বর্ণ *hex = oid_to_hex(oid);
	পূর্ণাঙ্ক reachable;

	cp.git_cmd = 1;
	cp.dir = path;
	cp.no_stderr = 1;
	strvec_pushl(&cp.args, "rev-list", "-n", "1", hex, "--not", "--all", রিক্ত);

	prepare_submodule_repo_env(&cp.env);

	যদি (capture_command(&cp, &rev, GIT_MAX_HEXSZ + 1) || rev.len)
		reachable = 0;
	অন্যথা
		reachable = 1;

	strbuf_release(&rev);
	প্রত্যাগম reachable;
}

স্থির পূর্ণাঙ্ক fetch_in_submodule(ধ্রুব বর্ণ *module_path, পূর্ণাঙ্ক depth, পূর্ণাঙ্ক quiet,
			      ধ্রুব সংরচনা object_id *oid)
{
	সংরচনা child_process cp = CHILD_PROCESS_INIT;

	prepare_submodule_repo_env(&cp.env);
	cp.git_cmd = 1;
	cp.dir = module_path;

	strvec_ঠেলো(&cp.args, "fetch");
	যদি (quiet)
		strvec_ঠেলো(&cp.args, "--quiet");
	যদি (depth)
		strvec_pushf(&cp.args, "--depth=%d", depth);
	যদি (oid) {
		বর্ণ *hex = oid_to_hex(oid);
		বর্ণ *remote;
		পূর্ণাঙ্ক code;

		code = get_default_remote_submodule(module_path, &remote);
		যদি (code) {
			child_process_clear(&cp);
			প্রত্যাগম code;
		}

		strvec_pushl(&cp.args, remote, hex, রিক্ত);
		মুক্ত(remote);
	}

	প্রত্যাগম run_command(&cp);
}

স্থির পূর্ণাঙ্ক run_update_command(ধ্রুব সংরচনা update_data *ud, পূর্ণাঙ্ক subforce)
{
	সংরচনা child_process cp = CHILD_PROCESS_INIT;
	বর্ণ *oid = oid_to_hex(&ud->oid);
	পূর্ণাঙ্ক ret;

	বিকল্প (ud->update_strategy.type) {
	পক্ষ SM_UPDATE_CHECKOUT:
		cp.git_cmd = 1;
		strvec_pushl(&cp.args, "checkout", "-q", রিক্ত);
		যদি (subforce)
			strvec_ঠেলো(&cp.args, "-f");
		ভঙ্গ;
	পক্ষ SM_UPDATE_REBASE:
		cp.git_cmd = 1;
		strvec_ঠেলো(&cp.args, "rebase");
		যদি (ud->quiet)
			strvec_ঠেলো(&cp.args, "--quiet");
		ভঙ্গ;
	পক্ষ SM_UPDATE_MERGE:
		cp.git_cmd = 1;
		strvec_ঠেলো(&cp.args, "merge");
		যদি (ud->quiet)
			strvec_ঠেলো(&cp.args, "--quiet");
		ভঙ্গ;
	পক্ষ SM_UPDATE_COMMAND:
		cp.use_shell = 1;
		strvec_ঠেলো(&cp.args, ud->update_strategy.command);
		ভঙ্গ;
	স্বভাব:
		BUG("unexpected update strategy type: %d",
		    ud->update_strategy.type);
	}
	strvec_ঠেলো(&cp.args, oid);

	cp.dir = ud->sm_path;
	prepare_submodule_repo_env(&cp.env);
	যদি ((ret = run_command(&cp))) {
		বিকল্প (ud->update_strategy.type) {
		পক্ষ SM_UPDATE_CHECKOUT:
			die_message(_("Unable to checkout '%s' in submodule path '%s'"),
				    oid, ud->displaypath);
			/* No "ret" assignment, use "git checkout"'s */
			ভঙ্গ;
		পক্ষ SM_UPDATE_REBASE:
			ret = die_message(_("Unable to rebase '%s' in submodule path '%s'"),
					  oid, ud->displaypath);
			ভঙ্গ;
		পক্ষ SM_UPDATE_MERGE:
			ret = die_message(_("Unable to merge '%s' in submodule path '%s'"),
					  oid, ud->displaypath);
			ভঙ্গ;
		পক্ষ SM_UPDATE_COMMAND:
			ret = die_message(_("Execution of '%s %s' failed in submodule path '%s'"),
					  ud->update_strategy.command, oid, ud->displaypath);
			ভঙ্গ;
		স্বভাব:
			BUG("unexpected update strategy type: %d",
			    ud->update_strategy.type);
		}

		প্রত্যাগম ret;
	}

	যদি (ud->quiet)
		প্রত্যাগম 0;

	বিকল্প (ud->update_strategy.type) {
	পক্ষ SM_UPDATE_CHECKOUT:
		printf(_("Submodule path '%s': checked out '%s'\n"),
		       ud->displaypath, oid);
		ভঙ্গ;
	পক্ষ SM_UPDATE_REBASE:
		printf(_("Submodule path '%s': rebased into '%s'\n"),
		       ud->displaypath, oid);
		ভঙ্গ;
	পক্ষ SM_UPDATE_MERGE:
		printf(_("Submodule path '%s': merged in '%s'\n"),
		       ud->displaypath, oid);
		ভঙ্গ;
	পক্ষ SM_UPDATE_COMMAND:
		printf(_("Submodule path '%s': '%s %s'\n"),
		       ud->displaypath, ud->update_strategy.command, oid);
		ভঙ্গ;
	স্বভাব:
		BUG("unexpected update strategy type: %d",
		    ud->update_strategy.type);
	}

	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক run_update_procedure(ধ্রুব সংরচনা update_data *ud)
{
	পূর্ণাঙ্ক subforce = is_null_oid(&ud->suboid) || ud->force;

	যদি (!ud->nofetch) {
		/*
		 * Run fetch only if `oid` isn't present or it
		 * is not reachable from a ref.
		 */
		যদি (!is_tip_reachable(ud->sm_path, &ud->oid) &&
		    fetch_in_submodule(ud->sm_path, ud->depth, ud->quiet, রিক্ত) &&
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
			প্রত্যাগম die_message(_("Fetched in submodule path '%s', but it did not "
					     "contain %s. Direct fetching of that commit failed."),
					   ud->displaypath, oid_to_hex(&ud->oid));
	}

	প্রত্যাগম run_update_command(ud, subforce);
}

স্থির পূর্ণাঙ্ক remote_submodule_branch(ধ্রুব বর্ণ *path, ধ্রুব বর্ণ **branch)
{
	ধ্রুব সংরচনা submodule *sub;
	বর্ণ *চাবি;
	*branch = রিক্ত;

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);
	যদি (!sub)
		প্রত্যাগম die_message(_("could not initialize submodule at path '%s'"),
				   path);

	চাবি = xstrfmt("submodule.%s.branch", sub->name);
	যদি (repo_config_get_string_tmp(the_repository, চাবি, branch))
		*branch = sub->branch;
	মুক্ত(চাবি);

	যদি (!*branch) {
		*branch = "HEAD";
		প্রত্যাগম 0;
	}

	যদি (!strcmp(*branch, ".")) {
		ধ্রুব বর্ণ *refname = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
							      "HEAD", 0, রিক্ত,
							      রিক্ত);

		যদি (!refname)
			প্রত্যাগম die_message(_("No such ref: %s"), "HEAD");

		/* detached HEAD */
		যদি (!strcmp(refname, "HEAD"))
			প্রত্যাগম die_message(_("Submodule (%s) branch configured to inherit "
					     "branch from superproject, but the superproject "
					     "is not on any branch"), sub->name);

		যদি (!skip_prefix(refname, "refs/heads/", &refname))
			প্রত্যাগম die_message(_("Expecting a full ref name, got %s"),
					   refname);

		*branch = refname;
		প্রত্যাগম 0;
	}

	/* Our "branch" is coming from repo_config_get_string_tmp() */
	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক ensure_core_worktree(ধ্রুব বর্ণ *path)
{
	ধ্রুব বর্ণ *cw;
	সংরচনা repository subrepo;

	যদি (repo_submodule_init(&subrepo, the_repository, path, null_oid(the_hash_algo)))
		প্রত্যাগম die_message(_("could not get a repository handle for submodule '%s'"),
				   path);

	যদি (!repo_config_get_string_tmp(&subrepo, "core.worktree", &cw)) {
		বর্ণ *cfg_file, *abs_path;
		ধ্রুব বর্ণ *rel_path;
		সংরচনা strbuf sb = STRBUF_INIT;

		cfg_file = repo_git_path(&subrepo, "config");

		abs_path = absolute_pathdup(path);
		rel_path = relative_path(abs_path, subrepo.gitdir, &sb);

		repo_config_set_in_file(the_repository, cfg_file, "core.worktree", rel_path);

		মুক্ত(cfg_file);
		মুক্ত(abs_path);
		strbuf_release(&sb);
	}

	repo_clear(&subrepo);
	প্রত্যাগম 0;
}

স্থির ধ্রুব বর্ণ *submodule_update_type_to_label(গণনা submodule_update_type type)
{
	বিকল্প (type) {
	পক্ষ SM_UPDATE_CHECKOUT:
		প্রত্যাগম "checkout";
	পক্ষ SM_UPDATE_MERGE:
		প্রত্যাগম "merge";
	পক্ষ SM_UPDATE_REBASE:
		প্রত্যাগম "rebase";
	পক্ষ SM_UPDATE_UNSPECIFIED:
	পক্ষ SM_UPDATE_NONE:
	পক্ষ SM_UPDATE_COMMAND:
		ভঙ্গ;
	}
	BUG("unreachable with type %d", type);
}

স্থির শূন্য update_data_to_args(ধ্রুব সংরচনা update_data *update_data,
				সংরচনা strvec *args)
{
	গণনা submodule_update_type update_type = update_data->update_default;

	strvec_pushl(args, "submodule--helper", "update", "--recursive", রিক্ত);
	যদি (update_data->displaypath)
		strvec_pushf(args, "--super-prefix=%s/",
			     update_data->displaypath);
	strvec_pushf(args, "--jobs=%d", update_data->max_jobs);
	যদি (update_data->quiet)
		strvec_ঠেলো(args, "--quiet");
	যদি (update_data->force)
		strvec_ঠেলো(args, "--force");
	যদি (update_data->init)
		strvec_ঠেলো(args, "--init");
	যদি (update_data->remote)
		strvec_ঠেলো(args, "--remote");
	যদি (update_data->nofetch)
		strvec_ঠেলো(args, "--no-fetch");
	যদি (update_data->dissociate)
		strvec_ঠেলো(args, "--dissociate");
	যদি (update_data->progress)
		strvec_ঠেলো(args, "--progress");
	যদি (update_data->require_init)
		strvec_ঠেলো(args, "--require-init");
	যদি (update_data->depth)
		strvec_pushf(args, "--depth=%d", update_data->depth);
	যদি (update_type != SM_UPDATE_UNSPECIFIED)
		strvec_pushf(args, "--%s",
			     submodule_update_type_to_label(update_type));

	যদি (update_data->references.nr) {
		সংরচনা string_list_item *item;

		for_each_string_list_item(item, &update_data->references)
			strvec_pushl(args, "--reference", item->string, রিক্ত);
	}
	যদি (update_data->ref_storage_format != REF_STORAGE_FORMAT_UNKNOWN)
		strvec_pushf(args, "--ref-storage-format=%s",
			     ref_storage_format_to_name(update_data->ref_storage_format));
	যদি (update_data->filter_options && update_data->filter_options->choice)
		strvec_pushf(args, "--filter=%s",
				expand_list_objects_filter_spec(
					update_data->filter_options));
	যদি (update_data->recommend_shallow == 0)
		strvec_ঠেলো(args, "--no-recommend-shallow");
	অন্যথা যদি (update_data->recommend_shallow == 1)
		strvec_ঠেলো(args, "--recommend-shallow");
	যদি (update_data->single_branch >= 0)
		strvec_ঠেলো(args, update_data->single_branch ?
				    "--single-branch" :
				    "--no-single-branch");
}

স্থির পূর্ণাঙ্ক update_submodule(সংরচনা update_data *update_data)
{
	পূর্ণাঙ্ক ret;

	যদি (validate_submodule_path(update_data->sm_path) < 0)
		প্রত্যাগম -1;

	ret = determine_submodule_update_strategy(the_repository,
						  update_data->just_cloned,
						  update_data->sm_path,
						  update_data->update_default,
						  &update_data->update_strategy);
	যদি (ret)
		প্রত্যাগম ret;

	যদি (update_data->just_cloned)
		oidcpy(&update_data->suboid, null_oid(the_hash_algo));
	অন্যথা যদি (repo_resolve_gitlink_ref(the_repository, update_data->sm_path,
					  "HEAD", &update_data->suboid))
		প্রত্যাগম die_message(_("Unable to find current revision in submodule path '%s'"),
				   update_data->displaypath);

	যদি (update_data->remote) {
		বর্ণ *remote_name;
		ধ্রুব বর্ণ *branch;
		বর্ণ *remote_ref;
		পূর্ণাঙ্ক code;

		code = get_default_remote_submodule(update_data->sm_path, &remote_name);
		যদি (code)
			প্রত্যাগম code;
		code = remote_submodule_branch(update_data->sm_path, &branch);
		যদি (code) {
			মুক্ত(remote_name);
			প্রত্যাগম code;
		}
		remote_ref = xstrfmt("refs/remotes/%s/%s", remote_name, branch);

		মুক্ত(remote_name);

		যদি (!update_data->nofetch) {
			যদি (fetch_in_submodule(update_data->sm_path, update_data->depth,
					      0, রিক্ত)) {
				মুক্ত(remote_ref);
				প্রত্যাগম die_message(_("Unable to fetch in submodule path '%s'"),
						   update_data->sm_path);
			}
		}

		যদি (repo_resolve_gitlink_ref(the_repository, update_data->sm_path,
					     remote_ref, &update_data->oid)) {
			ret = die_message(_("Unable to find %s revision in submodule path '%s'"),
					  remote_ref, update_data->sm_path);
			মুক্ত(remote_ref);
			প্রত্যাগম ret;
		}

		মুক্ত(remote_ref);
	}

	যদি (!oideq(&update_data->oid, &update_data->suboid) || update_data->force) {
		ret = run_update_procedure(update_data);
		যদি (ret)
			প্রত্যাগম ret;
	}

	যদি (update_data->recursive) {
		সংরচনা child_process cp = CHILD_PROCESS_INIT;
		সংরচনা update_data next = *update_data;

		next.prefix = রিক্ত;
		oidcpy(&next.oid, null_oid(the_hash_algo));
		oidcpy(&next.suboid, null_oid(the_hash_algo));

		cp.dir = update_data->sm_path;
		cp.git_cmd = 1;
		prepare_submodule_repo_env(&cp.env);
		update_data_to_args(&next, &cp.args);

		ret = run_command(&cp);
		যদি (ret)
			die_message(_("Failed to recurse into submodule path '%s'"),
				    update_data->displaypath);
		প্রত্যাগম ret;
	}

	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক update_submodules(সংরচনা update_data *update_data)
{
	পূর্ণাঙ্ক i, ret = 0;
	সংরচনা submodule_update_clone suc = SUBMODULE_UPDATE_CLONE_INIT;
	ধ্রুব সংরচনা run_process_parallel_opts opts = {
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
	যদি (suc.quickstop) {
		ret = 1;
		গচ্ছ cleanup;
	}

	প্রতি (i = 0; i < suc.update_clone_nr; i++) {
		সংরচনা update_clone_data ucd = suc.update_clone[i];
		পূর্ণাঙ্ক code = 128;

		oidcpy(&update_data->oid, &ucd.oid);
		update_data->just_cloned = ucd.just_cloned;
		update_data->sm_path = ucd.sub->path;

		/*
		 * Verify that the submodule path does not contain any
		 * symlinks; if it does, it might have been tampered with.
		 * TODO: allow exempting it via
		 * `safe.submodule.path` or something
		 */
		যদি (validate_submodule_path(update_data->sm_path) < 0)
			গচ্ছ fail;

		code = ensure_core_worktree(update_data->sm_path);
		যদি (code)
			গচ্ছ fail;

		update_data->displaypath = get_submodule_displaypath(
			update_data->sm_path, update_data->prefix,
			update_data->super_prefix);
		code = update_submodule(update_data);
		FREE_AND_NULL(update_data->displaypath);
fail:
		যদি (!code)
			অনুবর্ত;
		ret = code;
		যদি (ret == 128)
			গচ্ছ cleanup;
	}

cleanup:
	submodule_update_clone_release(&suc);
	string_list_clear(&update_data->references, 0);
	প্রত্যাগম ret;
}

স্থির পূর্ণাঙ্ক module_update(পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv, ধ্রুব বর্ণ *prefix,
			 সংরচনা repository *repo UNUSED)
{
	সংরচনা pathspec pathspec = { 0 };
	সংরচনা pathspec pathspec2 = { 0 };
	সংরচনা update_data opt = UPDATE_DATA_INIT;
	সংরচনা list_objects_filter_options filter_options =
		LIST_OBJECTS_FILTER_INIT;
	ধ্রুব বর্ণ *ref_storage_format = রিক্ত;
	পূর্ণাঙ্ক ret;
	সংরচনা option module_update_options[] = {
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
	ধ্রুব বর্ণ *ধ্রুব git_submodule_helper_usage[] = {
		N_("git submodule [--quiet] update"
		" [--init [--filter=<filter-spec>]] [--remote]"
		" [-N|--no-fetch] [-f|--force]"
		" [--checkout|--merge|--rebase]"
		" [--[no-]recommend-shallow] [--reference <repository>]"
		" [--recursive] [--[no-]single-branch] [--] [<path>...]"),
		রিক্ত
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
		গচ্ছ cleanup;
	}

	যদি (pathspec.nr)
		opt.warn_if_uninitialized = 1;

	যদি (opt.init) {
		সংরচনা module_list list = MODULE_LIST_INIT;
		সংরচনা init_cb info = INIT_CB_INIT;

		যদি (module_list_compute(argv, opt.prefix,
					&pathspec2, &list) < 0) {
			module_list_release(&list);
			ret = 1;
			গচ্ছ cleanup;
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
	update_data_release(&opt);
	list_objects_filter_release(&filter_options);
	clear_pathspec(&pathspec);
	clear_pathspec(&pathspec2);
	প্রত্যাগম ret;
}

স্থির পূর্ণাঙ্ক ঠেলো_check(পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv, ধ্রুব বর্ণ *prefix UNUSED,
		      সংরচনা repository *repo UNUSED)
{
	সংরচনা remote *remote;
	ধ্রুব বর্ণ *superproject_head;
	বর্ণ *head;
	পূর্ণাঙ্ক detached_head = 0;
	সংরচনা object_id head_oid;

	যদি (argc < 3)
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
				   0, &head_oid, রিক্ত);
	যদি (!head)
		die(_("Failed to resolve HEAD as a valid ref."));
	যদি (!strcmp(head, "HEAD"))
		detached_head = 1;

	/*
	 * The remote must be configured.
	 * This is to avoid pushing to the exact same URL as the parent.
	 */
	remote = pushremote_get(argv[1]);
	যদি (!remote || remote->origin == REMOTE_UNCONFIGURED)
		die("remote '%s' not configured", argv[1]);

	/* Check the refspec */
	যদি (argc > 2) {
		পূর্ণাঙ্ক i;
		সংরচনা ref *local_refs = get_local_heads();
		সংরচনা refspec refspec = REFSPEC_INIT_ঠেলো(the_hash_algo);

		refspec_appendn(&refspec, argv + 2, argc - 2);

		প্রতি (i = 0; i < refspec.nr; i++) {
			ধ্রুব সংরচনা refspec_item *rs = &refspec.বস্তুসমূহ[i];

			যদি (rs->pattern || rs->matching)
				অনুবর্ত;

			/* LHS must match a single ref */
			বিকল্প (গুনতি_refspec_match(rs->src, local_refs, রিক্ত)) {
			পক্ষ 1:
				ভঙ্গ;
			পক্ষ 0:
				/*
				 * If LHS matches 'HEAD' then we need to ensure
				 * that it matches the same named branch
				 * checked out in the superproject.
				 */
				যদি (!strcmp(rs->src, "HEAD")) {
					যদি (!detached_head &&
					    !strcmp(head, superproject_head))
						ভঙ্গ;
					die("HEAD does not match the named branch in the superproject");
				}
				/* fallthrough */
			স্বভাব:
				die("src refspec '%s' must name a ref",
				    rs->src);
			}
		}

		refspec_clear(&refspec);
		free_refs(local_refs);
	}
	মুক্ত(head);

	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক absorb_git_dirs(পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv, ধ্রুব বর্ণ *prefix,
			   সংরচনা repository *repo UNUSED)
{
	পূর্ণাঙ্ক i;
	সংরচনা pathspec pathspec = { 0 };
	সংরচনা module_list list = MODULE_LIST_INIT;
	ধ্রুব বর্ণ *super_prefix = রিক্ত;
	সংরচনা option embed_gitdir_options[] = {
		OPT__SUPER_PREFIX(&super_prefix),
		OPT_END()
	};
	ধ্রুব বর্ণ *ধ্রুব git_submodule_helper_usage[] = {
		N_("git submodule absorbgitdirs [<options>] [<path>...]"),
		রিক্ত
	};
	পূর্ণাঙ্ক ret = 1;

	argc = parse_options(argc, argv, prefix, embed_gitdir_options,
			     git_submodule_helper_usage, 0);

	যদি (module_list_compute(argv, prefix, &pathspec, &list) < 0)
		গচ্ছ cleanup;

	প্রতি (i = 0; i < list.nr; i++)
		absorb_git_dir_into_superproject(list.entries[i]->name,
						 super_prefix);

	ret = 0;
cleanup:
	clear_pathspec(&pathspec);
	module_list_release(&list);
	প্রত্যাগম ret;
}

স্থির পূর্ণাঙ্ক module_set_url(পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv, ধ্রুব বর্ণ *prefix,
			  সংরচনা repository *repo UNUSED)
{
	পূর্ণাঙ্ক quiet = 0, ret;
	ধ্রুব বর্ণ *newurl;
	ধ্রুব বর্ণ *path;
	বর্ণ *config_name;
	সংরচনা option options[] = {
		OPT__QUIET(&quiet, N_("suppress output for setting url of a submodule")),
		OPT_END()
	};
	ধ্রুব বর্ণ *ধ্রুব usage[] = {
		N_("git submodule set-url [--quiet] <path> <newurl>"),
		রিক্ত
	};
	ধ্রুব সংরচনা submodule *sub;

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
		sync_submodule(sub->path, prefix, রিক্ত, quiet ? OPT_QUIET : 0);
	}

	মুক্ত(config_name);
	প্রত্যাগম !!ret;
}

স্থির পূর্ণাঙ্ক module_set_branch(পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv, ধ্রুব বর্ণ *prefix,
			     সংরচনা repository *repo UNUSED)
{
	পূর্ণাঙ্ক opt_default = 0, ret;
	ধ্রুব বর্ণ *opt_branch = রিক্ত;
	ধ্রুব বর্ণ *path;
	বর্ণ *config_name;
	সংরচনা option options[] = {
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
	ধ্রুব বর্ণ *ধ্রুব usage[] = {
		N_("git submodule set-branch [-q|--quiet] (-d|--default) <path>"),
		N_("git submodule set-branch [-q|--quiet] (-b|--branch) <branch> <path>"),
		রিক্ত
	};
	ধ্রুব সংরচনা submodule *sub;

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
	প্রত্যাগম !!ret;
}

স্থির পূর্ণাঙ্ক module_create_branch(পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv, ধ্রুব বর্ণ *prefix,
				সংরচনা repository *repo UNUSED)
{
	গণনা branch_track track;
	পূর্ণাঙ্ক quiet = 0, force = 0, reflog = 0, dry_run = 0;
	সংরচনা option options[] = {
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
	ধ্রুব বর্ণ *ধ্রুব usage[] = {
		N_("git submodule--helper create-branch [-f|--force] [--create-reflog] [-q|--quiet] [-t|--track] [-n|--dry-run] <name> <start-oid> <start-name>"),
		রিক্ত
	};
	সংরচনা repo_config_মানসমূহ *cfg = repo_config_মানসমূহ(the_repository);

	repo_config(the_repository, git_default_config, রিক্ত);
	track = cfg->branch_track;
	argc = parse_options(argc, argv, prefix, options, usage, 0);

	যদি (argc != 3)
		usage_with_options(usage, options);

	যদি (!quiet && !dry_run)
		printf_ln(_("creating branch '%s'"), argv[0]);

	create_branches_recursively(the_repository, argv[0], argv[1], argv[2],
				    force, reflog, quiet, track, dry_run);
	প্রত্যাগম 0;
}

সংরচনা add_data {
	ধ্রুব বর্ণ *prefix;
	ধ্রুব বর্ণ *branch;
	ধ্রুব বর্ণ *reference_path;
	বর্ণ *sm_path;
	ধ্রুব বর্ণ *sm_name;
	ধ্রুব বর্ণ *repo;
	ধ্রুব বর্ণ *realrepo;
	গণনা ref_storage_format ref_storage_format;
	পূর্ণাঙ্ক depth;
	unsigned পূর্ণাঙ্ক force: 1;
	unsigned পূর্ণাঙ্ক quiet: 1;
	unsigned পূর্ণাঙ্ক progress: 1;
	unsigned পূর্ণাঙ্ক dissociate: 1;
};
#define ADD_DATA_INIT { \
	.depth = -1, \
	.ref_storage_format = REF_STORAGE_FORMAT_UNKNOWN, \
}

স্থির শূন্য append_fetch_remotes(সংরচনা strbuf *বার্তা, ধ্রুব বর্ণ *git_dir_path)
{
	সংরচনা child_process cp_remote = CHILD_PROCESS_INIT;
	সংরচনা strbuf sb_remote_out = STRBUF_INIT;

	cp_remote.git_cmd = 1;
	strvec_pushf(&cp_remote.env,
		     "GIT_DIR=%s", git_dir_path);
	strvec_ঠেলো(&cp_remote.env, "GIT_WORK_TREE=.");
	strvec_pushl(&cp_remote.args, "remote", "-v", রিক্ত);
	যদি (!capture_command(&cp_remote, &sb_remote_out, 0)) {
		বর্ণ *next_সারি;
		বর্ণ *সারি = sb_remote_out.buf;

		যাবত্ ((next_সারি = strchr(সারি, '\n')) != রিক্ত) {
			size_t len = next_সারি - সারি;

			যদি (strip_suffix_mem(সারি, &len, " (fetch)"))
				strbuf_addf(বার্তা, "  %.*s\n", (পূর্ণাঙ্ক)len, সারি);
			সারি = next_সারি + 1;
		}
	}

	strbuf_release(&sb_remote_out);
}

স্থির পূর্ণাঙ্ক add_submodule(ধ্রুব সংরচনা add_data *add_data)
{
	সংরচনা module_clone_data clone_data = MODULE_CLONE_DATA_INIT;
	সংরচনা string_list reference = STRING_LIST_INIT_NODUP;
	পূর্ণাঙ্ক ret = -1;

	/* perhaps the path already exists and is already a git repo, else clone it */
	যদি (is_directory(add_data->sm_path)) {
		বর্ণ *submod_gitdir_path;
		সংরচনা strbuf sm_path = STRBUF_INIT;
		strbuf_addstr(&sm_path, add_data->sm_path);
		submod_gitdir_path = xstrfmt("%s/.git", add_data->sm_path);
		যদি (is_nonbare_repository_dir(&sm_path))
			printf(_("Adding existing repo at '%s' to the index\n"),
			       add_data->sm_path);
		অন্যথা
			die(_("'%s' already exists and is not a valid git repo"),
			    add_data->sm_path);
		strbuf_release(&sm_path);
		মুক্ত(submod_gitdir_path);
	} অন্যথা {
		সংরচনা child_process cp = CHILD_PROCESS_INIT;
		সংরচনা strbuf submod_gitdir = STRBUF_INIT;

		submodule_name_to_gitdir(&submod_gitdir, the_repository, add_data->sm_name);

		যদি (is_directory(submod_gitdir.buf)) {
			যদি (!add_data->force) {
				সংরচনা strbuf বার্তা = STRBUF_INIT;
				বর্ণ *die_বার্তা;

				strbuf_addf(&বার্তা, _("A git directory for '%s' is found "
						    "locally with remote(s):\n"),
					    add_data->sm_name);

				append_fetch_remotes(&বার্তা, submod_gitdir.buf);
				strbuf_release(&submod_gitdir);

				strbuf_addf(&বার্তা, _("If you want to reuse this local git "
						    "directory instead of cloning again from\n"
						    "  %s\n"
						    "use the '--force' option. If the local git "
						    "directory is not the correct repo\n"
						    "or you are unsure what this means choose "
						    "another name with the '--name' option."),
					    add_data->realrepo);

				die_বার্তা = strbuf_detach(&বার্তা, রিক্ত);
				die("%s", die_বার্তা);
			} অন্যথা {
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
		যদি (add_data->reference_path) {
			বর্ণ *p = xstrdup(add_data->reference_path);

			string_list_append(&reference, p)->util = p;
		}
		clone_data.ref_storage_format = add_data->ref_storage_format;
		clone_data.dissociate = add_data->dissociate;
		যদি (add_data->depth >= 0)
			clone_data.depth = add_data->depth;

		যদি (clone_submodule(&clone_data, &reference))
			গচ্ছ cleanup;

		prepare_submodule_repo_env(&cp.env);
		cp.git_cmd = 1;
		cp.dir = add_data->sm_path;
		/*
		 * NOTE: we only get here if add_data->force is true, so
		 * passing --force to checkout is reasonable.
		 */
		strvec_pushl(&cp.args, "checkout", "-f", "-q", রিক্ত);

		যদি (add_data->branch) {
			strvec_pushl(&cp.args, "-B", add_data->branch, রিক্ত);
			strvec_pushf(&cp.args, "origin/%s", add_data->branch);
		}

		যদি (run_command(&cp))
			die(_("unable to checkout submodule '%s'"), add_data->sm_path);
	}
	ret = 0;

cleanup:
	string_list_clear(&reference, 1);
	প্রত্যাগম ret;
}

স্থির পূর্ণাঙ্ক config_submodule_in_gitmodules(ধ্রুব বর্ণ *name, ধ্রুব বর্ণ *var, ধ্রুব বর্ণ *মান)
{
	বর্ণ *চাবি;
	পূর্ণাঙ্ক ret;

	যদি (!is_writing_gitmodules_ok())
		die(_("please make sure that the .gitmodules file is in the working tree"));

	চাবি = xstrfmt("submodule.%s.%s", name, var);
	ret = config_set_in_gitmodules_file_gently(the_repository, চাবি, মান);
	মুক্ত(চাবি);

	প্রত্যাগম ret;
}

স্থির শূন্য configure_added_submodule(সংরচনা add_data *add_data)
{
	বর্ণ *চাবি;
	সংরচনা child_process add_submod = CHILD_PROCESS_INIT;
	সংরচনা child_process add_gitmodules = CHILD_PROCESS_INIT;
	ধ্রুব সংরচনা string_list *মানসমূহ;
	পূর্ণাঙ্ক matched = 0;

	চাবি = xstrfmt("submodule.%s.url", add_data->sm_name);
	repo_config_set_gently(the_repository, চাবি, add_data->realrepo);
	মুক্ত(চাবি);

	add_submod.git_cmd = 1;
	strvec_pushl(&add_submod.args, "add",
		     "--no-warn-embedded-repo", রিক্ত);
	যদি (add_data->force)
		strvec_ঠেলো(&add_submod.args, "--force");
	strvec_pushl(&add_submod.args, "--", add_data->sm_path, রিক্ত);

	যদি (run_command(&add_submod))
		die(_("Failed to add submodule '%s'"), add_data->sm_path);

	যদি (config_submodule_in_gitmodules(add_data->sm_name, "path", add_data->sm_path) ||
	    config_submodule_in_gitmodules(add_data->sm_name, "url", add_data->repo))
		die(_("Failed to register submodule '%s'"), add_data->sm_path);

	যদি (add_data->branch) {
		যদি (config_submodule_in_gitmodules(add_data->sm_name,
						   "branch", add_data->branch))
			die(_("Failed to register submodule '%s'"), add_data->sm_path);
	}

	add_gitmodules.git_cmd = 1;
	strvec_pushl(&add_gitmodules.args,
		     "add", "--force", "--", ".gitmodules", রিক্ত);

	যদি (run_command(&add_gitmodules))
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
	যদি (repo_config_get(the_repository, "submodule.active") || /* key absent */
	    repo_config_get_string_multi(the_repository, "submodule.active", &মানসমূহ)) {
		/*
		 * If the submodule being added isn't already covered by the
		 * current configured pathspec, set the submodule's active flag
		 */
		চাবি = xstrfmt("submodule.%s.active", add_data->sm_name);
		repo_config_set_gently(the_repository, চাবি, "true");
		মুক্ত(চাবি);
	} অন্যথা {
		প্রতি (size_t i = 0; i < মানসমূহ->nr; i++) {
			ধ্রুব বর্ণ *pat = মানসমূহ->বস্তুসমূহ[i].string;
			যদি (!wildmatch(pat, add_data->sm_path, 0)) { /* match found */
				matched = 1;
				ভঙ্গ;
			}
		}
		যদি (!matched) { /* no pattern matched -> force-enable */
			চাবি = xstrfmt("submodule.%s.active", add_data->sm_name);
			repo_config_set_gently(the_repository, চাবি, "true");
			মুক্ত(চাবি);
		}
	}
}

স্থির শূন্য die_on_index_match(ধ্রুব বর্ণ *path, পূর্ণাঙ্ক force)
{
	সংরচনা pathspec ps;
	ধ্রুব বর্ণ *args[] = { path, রিক্ত };
	parse_pathspec(&ps, 0, PATHSPEC_PREFER_CWD, রিক্ত, args);

	যদি (repo_read_index_preload(the_repository, রিক্ত, 0) < 0)
		die(_("index file corrupt"));

	যদি (ps.nr) {
		বর্ণ *ps_matched = xcalloc(ps.nr, 1);

		/* TODO: audit for interaction with sparse-index. */
		ensure_full_index(the_repository->index);

		/*
		 * Since there is only one pathspec, we just need to
		 * check ps_matched[0] to know if a cache entry matched.
		 */
		প্রতি (size_t i = 0; i < the_repository->index->cache_nr; i++) {
			ce_path_match(the_repository->index, the_repository->index->cache[i], &ps,
				      ps_matched);

			যদি (ps_matched[0]) {
				যদি (!force)
					die(_("'%s' already exists in the index"),
					    path);
				যদি (!S_ISGITLINK(the_repository->index->cache[i]->ce_mode))
					die(_("'%s' already exists in the index "
					      "and is not a submodule"), path);
				ভঙ্গ;
			}
		}
		মুক্ত(ps_matched);
	}
	clear_pathspec(&ps);
}

স্থির শূন্য die_on_repo_without_commits(ধ্রুব বর্ণ *path)
{
	সংরচনা strbuf sb = STRBUF_INIT;
	strbuf_addstr(&sb, path);
	যদি (is_nonbare_repository_dir(&sb)) {
		সংরচনা object_id oid;
		যদি (repo_resolve_gitlink_ref(the_repository, path, "HEAD", &oid) < 0)
			die(_("'%s' does not have a commit checked out"), path);
	}
	strbuf_release(&sb);
}

স্থির পূর্ণাঙ্ক module_add(পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv, ধ্রুব বর্ণ *prefix,
		      সংরচনা repository *repo UNUSED)
{
	পূর্ণাঙ্ক force = 0, quiet = 0, progress = 0, dissociate = 0;
	সংরচনা add_data add_data = ADD_DATA_INIT;
	ধ্রুব বর্ণ *ref_storage_format = রিক্ত;
	বর্ণ *to_free = রিক্ত;
	ধ্রুব সংরচনা submodule *existing;
	সংরচনা strbuf buf = STRBUF_INIT;
	বর্ণ *sm_name_to_free = রিক্ত;
	সংরচনা option options[] = {
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
	ধ্রুব বর্ণ *ধ্রুব usage[] = {
		N_("git submodule add [<options>] [--] <repository> [<path>]"),
		রিক্ত
	};
	সংরচনা strbuf sb = STRBUF_INIT;
	পূর্ণাঙ্ক ret = 1;

	argc = parse_options(argc, argv, prefix, options, usage, 0);

	যদি (!is_writing_gitmodules_ok())
		die(_("please make sure that the .gitmodules file is in the working tree"));

	যদি (prefix && *prefix &&
	    add_data.reference_path && !is_absolute_path(add_data.reference_path))
		add_data.reference_path = xstrfmt("%s%s", prefix, add_data.reference_path);

	যদি (argc == 0 || argc > 2)
		usage_with_options(usage, options);

	যদি (ref_storage_format) {
		add_data.ref_storage_format = ref_storage_format_by_name(ref_storage_format);
		যদি (add_data.ref_storage_format == REF_STORAGE_FORMAT_UNKNOWN)
			die(_("unknown ref storage format '%s'"), ref_storage_format);
	}

	add_data.repo = argv[0];
	যদি (argc == 1)
		add_data.sm_path = git_url_basename(add_data.repo, 0, 0);
	অন্যথা
		add_data.sm_path = xstrdup(argv[1]);

	যদি (prefix && *prefix && !is_absolute_path(add_data.sm_path)) {
		বর্ণ *sm_path = add_data.sm_path;

		add_data.sm_path = xstrfmt("%s%s", prefix, sm_path);
		মুক্ত(sm_path);
	}

	যদি (starts_with_dot_dot_slash(add_data.repo) ||
	    starts_with_dot_slash(add_data.repo)) {
		যদি (prefix)
			die(_("Relative path can only be used from the toplevel "
			      "of the working tree"));

		/* dereference source url relative to parent's url */
		to_free = resolve_relative_url(add_data.repo, রিক্ত, 1);
		add_data.realrepo = to_free;
	} অন্যথা যদি (is_dir_sep(add_data.repo[0]) || strchr(add_data.repo, ':')) {
		add_data.realrepo = add_data.repo;
	} অন্যথা {
		die(_("repo URL: '%s' must be absolute or begin with ./|../"),
		    add_data.repo);
	}

	/*
	 * normalize path:
	 * multiple //; leading ./; /./; /../;
	 */
	normalize_path_copy(add_data.sm_path, add_data.sm_path);
	strip_dir_trailing_slashes(add_data.sm_path);

	যদি (validate_submodule_path(add_data.sm_path) < 0)
		die(রিক্ত);

	die_on_index_match(add_data.sm_path, force);
	die_on_repo_without_commits(add_data.sm_path);

	যদি (!force) {
		সংরচনা child_process cp = CHILD_PROCESS_INIT;

		cp.git_cmd = 1;
		cp.no_stdout = 1;
		strvec_pushl(&cp.args, "add", "--dry-run", "--ignore-missing",
			     "--no-warn-embedded-repo", add_data.sm_path, রিক্ত);
		যদি ((ret = pipe_command(&cp, রিক্ত, 0, রিক্ত, 0, &sb, 0))) {
			strbuf_complete_সারি(&sb);
			fputs(sb.buf, stderr);
			গচ্ছ cleanup;
		}
	}

	যদি (!add_data.sm_name)
		add_data.sm_name = add_data.sm_path;

	existing = submodule_from_name(the_repository,
					null_oid(the_hash_algo),
					add_data.sm_name);

	যদি (existing && existing->path &&
	    strcmp(existing->path, add_data.sm_path)) {
		যদি (!force) {
			die(_("submodule name '%s' already used for path '%s'"),
			    add_data.sm_name, existing->path);
		}
		/* --force: build <name><n> until unique */
		প্রতি (পূর্ণাঙ্ক i = 1; ; i++) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "%s%d", add_data.sm_name, i);
			যদি (!submodule_from_name(the_repository,
						null_oid(the_hash_algo),
						buf.buf)) {
				ভঙ্গ;
			}
		}
		add_data.sm_name = sm_name_to_free = strbuf_detach(&buf, রিক্ত);
	}

	যদি (check_submodule_name(add_data.sm_name))
		die(_("'%s' is not a valid submodule name"), add_data.sm_name);

	add_data.prefix = prefix;
	add_data.force = !!force;
	add_data.quiet = !!quiet;
	add_data.progress = !!progress;
	add_data.dissociate = !!dissociate;

	যদি (the_repository->repository_format_submodule_path_cfg)
		create_default_gitdir_config(add_data.sm_name);

	যদি (add_submodule(&add_data))
		গচ্ছ cleanup;
	configure_added_submodule(&add_data);

	ret = 0;
cleanup:
	মুক্ত(sm_name_to_free);
	মুক্ত(add_data.sm_path);
	মুক্ত(to_free);
	strbuf_release(&sb);

	প্রত্যাগম ret;
}

পূর্ণাঙ্ক cmd_submodule__helper(পূর্ণাঙ্ক argc,
			  ধ্রুব বর্ণ **argv,
			  ধ্রুব বর্ণ *prefix,
			  সংরচনা repository *repo)
{
	parse_opt_subcommand_fn *fn = রিক্ত;
	ধ্রুব বর্ণ *ধ্রুব usage[] = {
		N_("git submodule--helper <command>"),
		রিক্ত
	};
	সংরচনা option options[] = {
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

	প্রত্যাগম fn(argc, argv, prefix, repo);
}
