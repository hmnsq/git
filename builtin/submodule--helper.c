#define USE_THE_REPOSITORY_VARIABLE

#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "abspath.h"
#அந்தர்பாவ "environment.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "hex.h"

#அந்தர்பாவ "config.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "quote.h"
#அந்தர்பாவ "path.h"
#அந்தர்பாவ "pathspec.h"
#அந்தர்பாவ "preload-index.h"
#அந்தர்பாவ "dir.h"
#அந்தர்பாவ "read-cache.h"
#அந்தர்பாவ "setup.h"
#அந்தர்பாவ "sparse-index.h"
#அந்தர்பாவ "submodule.h"
#அந்தர்பாவ "submodule-config.h"
#அந்தர்பாவ "string-list.h"
#அந்தர்பாவ "run-command.h"
#அந்தர்பாவ "remote.h"
#அந்தர்பாவ "refs.h"
#அந்தர்பாவ "refspec.h"
#அந்தர்பாவ "revision.h"
#அந்தர்பாவ "diffcore.h"
#அந்தர்பாவ "diff.h"
#அந்தர்பாவ "object-file.h"
#அந்தர்பாவ "object-name.h"
#அந்தர்பாவ "odb.h"
#அந்தர்பாவ "odb/source.h"
#அந்தர்பாவ "odb/source-files.h"
#அந்தர்பாவ "advice.h"
#அந்தர்பாவ "branch.h"
#அந்தர்பாவ "list-objects-filter-options.h"
#அந்தர்பாவ "wildmatch.h"
#அந்தர்பாவ "strbuf.h"
#அந்தர்பாவ "url.h"

#define OPT_QUIET (1 << 0)
#define OPT_CACHED (1 << 1)
#define OPT_RECURSIVE (1 << 2)
#define OPT_FORCE (1 << 3)

வகைவரை வெறுமை (*each_submodule_fn)(மாறிலி கட்டமைப்பு cache_entry *list_item,
				  வெறுமை *cb_data);

நிலையான எழுத்து *get_default_remote(வெறுமை)
{
	திரும்பு xstrdup(repo_default_remote(the_repository));
}

நிலையான எழுத்து *resolve_relative_url(மாறிலி எழுத்து *rel_url, மாறிலி எழுத்து *up_path, முழு quiet)
{
	எழுத்து *remoteurl, *resolved_url;
	எழுத்து *remote = get_default_remote();
	கட்டமைப்பு strbuf remotesb = STRBUF_INIT;

	strbuf_addf(&remotesb, "remote.%s.url", remote);
	எனில் (repo_config_get_string(the_repository, remotesb.buf, &remoteurl)) {
		எனில் (!quiet)
			warning(_("could not look up configuration '%s'. "
				  "Assuming this repository is its own "
				  "authoritative upstream."),
				remotesb.buf);
		remoteurl = xgetcwd();
	}
	resolved_url = relative_url(remoteurl, rel_url, up_path);

	விடுவி(remote);
	விடுவி(remoteurl);
	strbuf_release(&remotesb);

	திரும்பு resolved_url;
}

நிலையான முழு get_default_remote_submodule(மாறிலி எழுத்து *module_path, எழுத்து **default_remote)
{
	மாறிலி கட்டமைப்பு submodule *sub;
	கட்டமைப்பு repository subrepo;
	மாறிலி எழுத்து *remote_name = வெற்று;
	எழுத்து *url = வெற்று;
	முழு ret = 0;

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), module_path);
	எனில் (sub && sub->url) {
		url = xstrdup(sub->url);

		/* Possibly a url relative to parent */
		எனில் (starts_with_dot_dot_slash(url) ||
		    starts_with_dot_slash(url)) {
			எழுத்து *oldurl = url;

			url = resolve_relative_url(oldurl, வெற்று, 1);
			விடுவி(oldurl);
		}
	}

	எனில் (repo_submodule_init(&subrepo, the_repository, module_path,
				null_oid(the_hash_algo)) < 0) {
		ret = die_message(_("could not get a repository handle for submodule '%s'"),
				   module_path);
		செல் out;
	}

	/* Look up by URL first */
	எனில் (url)
		remote_name = repo_remote_from_url(&subrepo, url);
	எனில் (!remote_name)
		remote_name = repo_default_remote(&subrepo);

	*default_remote = xstrdup(remote_name);

out:
	repo_clear(&subrepo);
	விடுவி(url);

	திரும்பு ret;
}

நிலையான முழு module_get_default_remote(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
				     கட்டமைப்பு repository *repo UNUSED)
{
	மாறிலி எழுத்து *path;
	எழுத்து *resolved_path = வெற்று;
	எழுத்து *default_remote = வெற்று;
	முழு code;
	கட்டமைப்பு option options[] = {
		OPT_END()
	};
	மாறிலி எழுத்து *மாறிலி usage[] = {
		N_("git submodule--helper get-default-remote <path>"),
		வெற்று
	};

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	எனில் (argc != 1)
		usage_with_options(usage, options);

	path = argv[0];
	எனில் (prefix && *prefix && !is_absolute_path(path)) {
		resolved_path = xstrfmt("%s%s", prefix, path);
		path = resolved_path;
	}

	code = get_default_remote_submodule(path, &default_remote);
	எனில் (code) {
		விடுவி(resolved_path);
		திரும்பு code;
	}

	printf("%s\n", default_remote);
	விடுவி(default_remote);
	விடுவி(resolved_path);
	திரும்பு 0;
}

/* the result should be freed by the caller. */
நிலையான எழுத்து *get_submodule_displaypath(மாறிலி எழுத்து *path, மாறிலி எழுத்து *prefix,
				       மாறிலி எழுத்து *super_prefix)
{
	எனில் (prefix && super_prefix) {
		BUG("cannot have prefix '%s' and superprefix '%s'",
		    prefix, super_prefix);
	} இல்லையெனில் எனில் (prefix) {
		கட்டமைப்பு strbuf sb = STRBUF_INIT;
		எழுத்து *displaypath = xstrdup(relative_path(path, prefix, &sb));
		strbuf_release(&sb);
		திரும்பு displaypath;
	} இல்லையெனில் எனில் (super_prefix) {
		திரும்பு xstrfmt("%s%s", super_prefix, path);
	} இல்லையெனில் {
		திரும்பு xstrdup(path);
	}
}

நிலையான எழுத்து *compute_rev_name(மாறிலி எழுத்து *sub_path, மாறிலி எழுத்து* object_id)
{
	கட்டமைப்பு strbuf sb = STRBUF_INIT;
	மாறிலி எழுத்து ***d;

	நிலையான மாறிலி எழுத்து *describe_bare[] = { வெற்று };

	நிலையான மாறிலி எழுத்து *describe_tags[] = { "--tags", வெற்று };

	நிலையான மாறிலி எழுத்து *describe_contains[] = { "--contains", வெற்று };

	நிலையான மாறிலி எழுத்து *describe_all_always[] = { "--all", "--always", வெற்று };

	நிலையான மாறிலி எழுத்து **describe_argv[] = { describe_bare, describe_tags,
						describe_contains,
						describe_all_always, வெற்று };

	க்கு (d = describe_argv; *d; d++) {
		கட்டமைப்பு child_process cp = CHILD_PROCESS_INIT;
		prepare_submodule_repo_env(&cp.env);
		cp.dir = sub_path;
		cp.git_cmd = 1;
		cp.no_stderr = 1;

		strvec_தள்ளு(&cp.args, "describe");
		strvec_pushv(&cp.args, *d);
		strvec_தள்ளு(&cp.args, object_id);

		எனில் (!capture_command(&cp, &sb, 0)) {
			strbuf_strip_suffix(&sb, "\n");
			திரும்பு strbuf_detach(&sb, வெற்று);
		}
	}

	strbuf_release(&sb);
	திரும்பு வெற்று;
}

கட்டமைப்பு module_list {
	மாறிலி கட்டமைப்பு cache_entry **entries;
	முழு alloc, nr;
};
#define MODULE_LIST_INIT { 0 }

நிலையான வெறுமை module_list_release(கட்டமைப்பு module_list *ml)
{
	விடுவி(ml->entries);
}

நிலையான முழு module_list_compute(மாறிலி எழுத்து **argv,
			       மாறிலி எழுத்து *prefix,
			       கட்டமைப்பு pathspec *pathspec,
			       கட்டமைப்பு module_list *list)
{
	முழு result = 0;
	எழுத்து *ps_matched = வெற்று;

	parse_pathspec(pathspec, 0,
		       PATHSPEC_PREFER_FULL,
		       prefix, argv);

	எனில் (pathspec->nr)
		ps_matched = xcalloc(pathspec->nr, 1);

	எனில் (repo_read_index(the_repository) < 0)
		die(_("index file corrupt"));

	க்கு (size_t i = 0; i < the_repository->index->cache_nr; i++) {
		மாறிலி கட்டமைப்பு cache_entry *ce = the_repository->index->cache[i];

		எனில் (!match_pathspec(the_repository->index, pathspec, ce->name, ce_namelen(ce),
				    0, ps_matched, 1) ||
		    !S_ISGITLINK(ce->ce_mode))
			தொடர்;

		ALLOC_GROW(list->entries, list->nr + 1, list->alloc);
		list->entries[list->nr++] = ce;
		போது (i + 1 < the_repository->index->cache_nr &&
		       !strcmp(ce->name, the_repository->index->cache[i + 1]->name))
			/*
			 * Skip entries with the same name in different stages
			 * to make sure an entry is returned only once.
			 */
			i++;
	}

	எனில் (ps_matched && report_path_error(ps_matched, pathspec))
		result = -1;

	விடுவி(ps_matched);

	திரும்பு result;
}

நிலையான வெறுமை module_list_active(கட்டமைப்பு module_list *list)
{
	முழு i;
	கட்டமைப்பு module_list active_modules = MODULE_LIST_INIT;

	க்கு (i = 0; i < list->nr; i++) {
		மாறிலி கட்டமைப்பு cache_entry *ce = list->entries[i];

		எனில் (!is_submodule_active(the_repository, ce->name))
			தொடர்;

		ALLOC_GROW(active_modules.entries,
			   active_modules.nr + 1,
			   active_modules.alloc);
		active_modules.entries[active_modules.nr++] = ce;
	}

	module_list_release(list);
	*list = active_modules;
}

நிலையான எழுத்து *get_up_path(மாறிலி எழுத்து *path)
{
	கட்டமைப்பு strbuf sb = STRBUF_INIT;

	strbuf_addstrings(&sb, "../", மொத்தம்_slashes(path));

	/*
	 * Check if 'path' ends with slash or not
	 * for having the same output for dir/sub_dir
	 * and dir/sub_dir/
	 */
	எனில் (!is_dir_sep(path[strlen(path) - 1]))
		strbuf_addstr(&sb, "../");

	திரும்பு strbuf_detach(&sb, வெற்று);
}

நிலையான வெறுமை for_each_listed_submodule(மாறிலி கட்டமைப்பு module_list *list,
				      each_submodule_fn fn, வெறுமை *cb_data)
{
	முழு i;

	க்கு (i = 0; i < list->nr; i++)
		fn(list->entries[i], cb_data);
}

கட்டமைப்பு foreach_cb {
	முழு argc;
	மாறிலி எழுத்து **argv;
	மாறிலி எழுத்து *prefix;
	மாறிலி எழுத்து *super_prefix;
	முழு quiet;
	முழு recursive;
};
#define FOREACH_CB_INIT { 0 }

நிலையான வெறுமை runcommand_in_submodule_cb(மாறிலி கட்டமைப்பு cache_entry *list_item,
				       வெறுமை *cb_data)
{
	கட்டமைப்பு foreach_cb *info = cb_data;
	மாறிலி எழுத்து *path = list_item->name;
	மாறிலி கட்டமைப்பு object_id *ce_oid = &list_item->oid;
	மாறிலி கட்டமைப்பு submodule *sub;
	கட்டமைப்பு child_process cp = CHILD_PROCESS_INIT;
	எழுத்து *displaypath;

	எனில் (validate_submodule_path(path) < 0)
		die(வெற்று);

	displaypath = get_submodule_displaypath(path, info->prefix,
						info->super_prefix);

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);

	எனில் (!sub)
		die(_("No url found for submodule path '%s' in .gitmodules"),
			displaypath);

	எனில் (!is_submodule_populated_gently(path, வெற்று))
		செல் cleanup;

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
	எனில் (info->argc == 1) {
		எழுத்து *toplevel = xgetcwd();
		கட்டமைப்பு strbuf sb = STRBUF_INIT;

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
		விடுவி(toplevel);
	} இல்லையெனில் {
		strvec_pushv(&cp.args, info->argv);
	}

	எனில் (!info->quiet)
		printf(_("Entering '%s'\n"), displaypath);

	எனில் (info->argv[0]) {
		எனில் (run_command(&cp))
			die(_("run_command returned non-zero status for %s\n."),
			    displaypath);
	} இல்லையெனில் {
		child_process_clear(&cp);
	}

	எனில் (info->recursive) {
		கட்டமைப்பு child_process cpr = CHILD_PROCESS_INIT;

		cpr.git_cmd = 1;
		cpr.dir = path;
		prepare_submodule_repo_env(&cpr.env);

		strvec_pushl(&cpr.args, "submodule--helper", "foreach", "--recursive",
			     வெற்று);
		strvec_pushf(&cpr.args, "--super-prefix=%s/", displaypath);

		எனில் (info->quiet)
			strvec_தள்ளு(&cpr.args, "--quiet");

		strvec_தள்ளு(&cpr.args, "--");
		strvec_pushv(&cpr.args, info->argv);

		எனில் (run_command(&cpr))
			die(_("run_command returned non-zero status while "
				"recursing in the nested submodules of %s\n."),
				displaypath);
	}

cleanup:
	விடுவி(displaypath);
}

நிலையான முழு module_foreach(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
			  கட்டமைப்பு repository *repo UNUSED)
{
	கட்டமைப்பு foreach_cb info = FOREACH_CB_INIT;
	கட்டமைப்பு pathspec pathspec = { 0 };
	கட்டமைப்பு module_list list = MODULE_LIST_INIT;
	கட்டமைப்பு option module_foreach_options[] = {
		OPT__SUPER_PREFIX(&info.super_prefix),
		OPT__QUIET(&info.quiet, N_("suppress output of entering each submodule command")),
		OPT_BOOL(0, "recursive", &info.recursive,
			 N_("recurse into nested submodules")),
		OPT_END()
	};
	மாறிலி எழுத்து *மாறிலி git_submodule_helper_usage[] = {
		N_("git submodule foreach [--quiet] [--recursive] [--] <command>"),
		வெற்று
	};
	முழு ret = 1;

	argc = parse_options(argc, argv, prefix, module_foreach_options,
			     git_submodule_helper_usage, 0);

	எனில் (module_list_compute(வெற்று, prefix, &pathspec, &list) < 0)
		செல் cleanup;

	info.argc = argc;
	info.argv = argv;
	info.prefix = prefix;

	for_each_listed_submodule(&list, runcommand_in_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	திரும்பு ret;
}

கட்டமைப்பு init_cb {
	மாறிலி எழுத்து *prefix;
	மாறிலி எழுத்து *super_prefix;
	unsigned முழு flags;
};
#define INIT_CB_INIT { 0 }

நிலையான முழு validate_and_set_submodule_gitdir(கட்டமைப்பு strbuf *gitdir_path,
					     மாறிலி எழுத்து *submodule_name)
{
	மாறிலி எழுத்து *மதிப்பு;
	எழுத்து *சாவி;

	எனில் (validate_submodule_git_dir(gitdir_path->buf, submodule_name))
		திரும்பு -1;

	சாவி = xstrfmt("submodule.%s.gitdir", submodule_name);

	/* Nothing to do if the config already exists. */
	எனில் (!repo_config_get_string_tmp(the_repository, சாவி, &மதிப்பு)) {
		விடுவி(சாவி);
		திரும்பு 0;
	}

	எனில் (repo_config_set_gently(the_repository, சாவி, gitdir_path->buf)) {
		விடுவி(சாவி);
		திரும்பு -1;
	}

	விடுவி(சாவி);
	திரும்பு 0;
}

நிலையான வெறுமை create_default_gitdir_config(மாறிலி எழுத்து *submodule_name)
{
	கட்டமைப்பு strbuf gitdir_path = STRBUF_INIT;
	கட்டமைப்பு git_hash_ctx ctx;
	எழுத்து hex_name_hash[GIT_MAX_HEXSZ + 1], header[128];
	unsigned எழுத்து raw_name_hash[GIT_MAX_RAWSZ];
	முழு header_len;

	/* Case 1: try the plain module name */
	repo_git_path_append(the_repository, &gitdir_path, "modules/%s", submodule_name);
	எனில் (!validate_and_set_submodule_gitdir(&gitdir_path, submodule_name)) {
		strbuf_release(&gitdir_path);
		திரும்பு;
	}

	/* Case 2.1: Try URI-safe (RFC3986) encoding first, this fixes nested gitdirs */
	strbuf_reset(&gitdir_path);
	repo_git_path_append(the_repository, &gitdir_path, "modules/");
	strbuf_addstr_urlencode(&gitdir_path, submodule_name, is_rfc3986_unreserved);
	எனில் (!validate_and_set_submodule_gitdir(&gitdir_path, submodule_name)) {
		strbuf_release(&gitdir_path);
		திரும்பு;
	}

	/* Case 2.2: Try extended uppercase URI (RFC3986) encoding, to fix case-folding */
	strbuf_reset(&gitdir_path);
	repo_git_path_append(the_repository, &gitdir_path, "modules/");
	strbuf_addstr_urlencode(&gitdir_path, submodule_name, is_casefolding_rfc3986_unreserved);
	எனில் (!validate_and_set_submodule_gitdir(&gitdir_path, submodule_name))
		திரும்பு;

	/* Case 2.3: Try some derived gitdir names, see if one sticks */
	க்கு (எழுத்து c = '0'; c <= '9'; c++) {
		strbuf_reset(&gitdir_path);
		repo_git_path_append(the_repository, &gitdir_path, "modules/");
		strbuf_addstr_urlencode(&gitdir_path, submodule_name, is_rfc3986_unreserved);
		strbuf_addch(&gitdir_path, c);
		எனில் (!validate_and_set_submodule_gitdir(&gitdir_path, submodule_name))
			திரும்பு;

		strbuf_reset(&gitdir_path);
		repo_git_path_append(the_repository, &gitdir_path, "modules/");
		strbuf_addstr_urlencode(&gitdir_path, submodule_name, is_casefolding_rfc3986_unreserved);
		strbuf_addch(&gitdir_path, c);
		எனில் (!validate_and_set_submodule_gitdir(&gitdir_path, submodule_name))
			திரும்பு;
	}

	/* Case 2.4: If all the above failed, try a hash of the name as a last resort */
	header_len = snprintf(header, அளவு(header),
			      "blob %"PRIuMAX, (uintmax_t)strlen(submodule_name));
	git_hash_init(&ctx, the_hash_algo);
	git_hash_update(&ctx, header, header_len);
	git_hash_update(&ctx, "\0", 1);
	git_hash_update(&ctx, submodule_name, strlen(submodule_name));
	git_hash_final(raw_name_hash, &ctx);
	hash_to_hex_algop_r(hex_name_hash, raw_name_hash, the_hash_algo);
	strbuf_reset(&gitdir_path);
	repo_git_path_append(the_repository, &gitdir_path, "modules/%s", hex_name_hash);
	எனில் (!validate_and_set_submodule_gitdir(&gitdir_path, submodule_name)) {
		strbuf_release(&gitdir_path);
		திரும்பு;
	}

	/* Case 3: nothing worked, error out */
	die(_("failed to set a valid default config for 'submodule.%s.gitdir'. "
	      "Please ensure it is set, for example by running something like: "
	      "'git config submodule.%s.gitdir .git/modules/%s'"),
	    submodule_name, submodule_name, submodule_name);
}

நிலையான வெறுமை init_submodule(மாறிலி எழுத்து *path, மாறிலி எழுத்து *prefix,
			   மாறிலி எழுத்து *super_prefix,
			   unsigned முழு flags)
{
	மாறிலி கட்டமைப்பு submodule *sub;
	கட்டமைப்பு strbuf sb = STRBUF_INIT;
	மாறிலி எழுத்து *upd;
	எழுத்து *url = வெற்று, *displaypath;

	displaypath = get_submodule_displaypath(path, prefix, super_prefix);

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);

	எனில் (!sub)
		die(_("No url found for submodule path '%s' in .gitmodules"),
			displaypath);

	/*
	 * NEEDSWORK: In a multi-working-tree world, this needs to be
	 * set in the per-worktree config.
	 *
	 * Set active flag for the submodule being initialized
	 */
	எனில் (!is_submodule_active(the_repository, path)) {
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
	எனில் (repo_config_get_string(the_repository, sb.buf, &url)) {
		எனில் (!sub->url)
			die(_("No url found for submodule path '%s' in .gitmodules"),
				displaypath);

		url = xstrdup(sub->url);

		/* Possibly a url relative to parent */
		எனில் (starts_with_dot_dot_slash(url) ||
		    starts_with_dot_slash(url)) {
			எழுத்து *oldurl = url;

			url = resolve_relative_url(oldurl, வெற்று, 0);
			விடுவி(oldurl);
		}

		எனில் (repo_config_set_gently(the_repository, sb.buf, url))
			die(_("Failed to register url for submodule path '%s'"),
			    displaypath);
		எனில் (!(flags & OPT_QUIET))
			fprintf(stderr,
				_("Submodule '%s' (%s) registered for path '%s'\n"),
				sub->name, url, displaypath);
	}
	strbuf_reset(&sb);

	/* Copy "update" setting when it is not set yet */
	strbuf_addf(&sb, "submodule.%s.update", sub->name);
	எனில் (repo_config_get_string_tmp(the_repository, sb.buf, &upd) &&
	    sub->update_strategy.type != SM_UPDATE_UNSPECIFIED) {
		எனில் (sub->update_strategy.type == SM_UPDATE_COMMAND) {
			fprintf(stderr, _("warning: command update mode suggested for submodule '%s'\n"),
				sub->name);
			upd = "none";
		} இல்லையெனில் {
			upd = submodule_update_type_to_string(sub->update_strategy.type);
		}

		எனில் (repo_config_set_gently(the_repository, sb.buf, upd))
			die(_("Failed to register update mode for submodule path '%s'"), displaypath);
	}

	எனில் (the_repository->repository_format_submodule_path_cfg)
		create_default_gitdir_config(sub->name);

	strbuf_release(&sb);
	விடுவி(displaypath);
	விடுவி(url);
}

நிலையான வெறுமை init_submodule_cb(மாறிலி கட்டமைப்பு cache_entry *list_item, வெறுமை *cb_data)
{
	கட்டமைப்பு init_cb *info = cb_data;

	init_submodule(list_item->name, info->prefix, info->super_prefix,
		       info->flags);
}

நிலையான முழு module_init(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
		       கட்டமைப்பு repository *repo UNUSED)
{
	கட்டமைப்பு init_cb info = INIT_CB_INIT;
	கட்டமைப்பு pathspec pathspec = { 0 };
	கட்டமைப்பு module_list list = MODULE_LIST_INIT;
	முழு quiet = 0;
	கட்டமைப்பு option module_init_options[] = {
		OPT__QUIET(&quiet, N_("suppress output for initializing a submodule")),
		OPT_END()
	};
	மாறிலி எழுத்து *மாறிலி git_submodule_helper_usage[] = {
		N_("git submodule init [<options>] [<path>]"),
		வெற்று
	};
	முழு ret = 1;

	argc = parse_options(argc, argv, prefix, module_init_options,
			     git_submodule_helper_usage, 0);

	எனில் (module_list_compute(argv, prefix, &pathspec, &list) < 0)
		செல் cleanup;

	/*
	 * If there are no path args and submodule.active is set then,
	 * by default, only initialize 'active' modules.
	 */
	எனில் (!argc && !repo_config_get(the_repository, "submodule.active"))
		module_list_active(&list);

	info.prefix = prefix;
	எனில் (quiet)
		info.flags |= OPT_QUIET;

	for_each_listed_submodule(&list, init_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	திரும்பு ret;
}

கட்டமைப்பு status_cb {
	மாறிலி எழுத்து *prefix;
	மாறிலி எழுத்து *super_prefix;
	unsigned முழு flags;
};
#define STATUS_CB_INIT { 0 }

நிலையான வெறுமை print_status(unsigned முழு flags, எழுத்து state, மாறிலி எழுத்து *path,
			 மாறிலி கட்டமைப்பு object_id *oid, மாறிலி எழுத்து *displaypath)
{
	எனில் (flags & OPT_QUIET)
		திரும்பு;

	printf("%c%s %s", state, oid_to_hex(oid), displaypath);

	எனில் (state == ' ' || state == '+') {
		எழுத்து *name = compute_rev_name(path, oid_to_hex(oid));

		எனில் (name)
			printf(" (%s)", name);
		விடுவி(name);
	}

	printf("\n");
}

நிலையான முழு handle_submodule_head_ref(மாறிலி கட்டமைப்பு reference *ref, வெறுமை *cb_data)
{
	கட்டமைப்பு object_id *output = cb_data;

	எனில் (ref->oid)
		oidcpy(output, ref->oid);

	திரும்பு 0;
}

நிலையான வெறுமை status_submodule(மாறிலி எழுத்து *path, மாறிலி கட்டமைப்பு object_id *ce_oid,
			     unsigned முழு ce_flags, மாறிலி எழுத்து *prefix,
			     மாறிலி எழுத்து *super_prefix, unsigned முழு flags)
{
	எழுத்து *displaypath;
	கட்டமைப்பு strvec diff_files_args = STRVEC_INIT;
	கட்டமைப்பு rev_info rev = REV_INFO_INIT;
	கட்டமைப்பு strbuf buf = STRBUF_INIT;
	மாறிலி எழுத்து *git_dir;

	எனில் (validate_submodule_path(path) < 0)
		die(வெற்று);

	எனில் (!submodule_from_path(the_repository, null_oid(the_hash_algo), path))
		die(_("no submodule mapping found in .gitmodules for path '%s'"),
		      path);

	displaypath = get_submodule_displaypath(path, prefix, super_prefix);

	எனில் ((CE_STAGEMASK & ce_flags) >> CE_STAGESHIFT) {
		print_status(flags, 'U', path, null_oid(the_hash_algo), displaypath);
		செல் cleanup;
	}

	strbuf_addf(&buf, "%s/.git", path);
	git_dir = read_gitfile(buf.buf);
	எனில் (!git_dir)
		git_dir = buf.buf;

	எனில் (!is_submodule_active(the_repository, path) ||
	    !is_git_directory(git_dir)) {
		print_status(flags, '-', path, ce_oid, displaypath);
		strbuf_release(&buf);
		செல் cleanup;
	}
	strbuf_release(&buf);

	strvec_pushl(&diff_files_args, "diff-files",
		     "--ignore-submodules=dirty", "--quiet", "--",
		     path, வெற்று);

	repo_config(the_repository, git_diff_basic_config, வெற்று);

	repo_init_revisions(the_repository, &rev, வெற்று);
	rev.abbrev = 0;
	setup_revisions_from_strvec(&diff_files_args, &rev, வெற்று);
	run_diff_files(&rev, 0);

	எனில் (!diff_result_code(&rev)) {
		print_status(flags, ' ', path, ce_oid,
			     displaypath);
	} இல்லையெனில் எனில் (!(flags & OPT_CACHED)) {
		கட்டமைப்பு object_id oid;
		கட்டமைப்பு ref_store *refs = repo_get_submodule_ref_store(the_repository,
								      path);

		எனில் (!refs) {
			print_status(flags, '-', path, ce_oid, displaypath);
			செல் cleanup;
		}
		எனில் (refs_head_ref(refs, handle_submodule_head_ref, &oid))
			die(_("could not resolve HEAD ref inside the "
			      "submodule '%s'"), path);

		print_status(flags, '+', path, &oid, displaypath);
	} இல்லையெனில் {
		print_status(flags, '+', path, ce_oid, displaypath);
	}

	எனில் (flags & OPT_RECURSIVE) {
		கட்டமைப்பு child_process cpr = CHILD_PROCESS_INIT;
		முழு res;

		cpr.git_cmd = 1;
		cpr.dir = path;
		prepare_submodule_repo_env(&cpr.env);

		strvec_pushl(&cpr.args, "submodule--helper", "status",
			     "--recursive", வெற்று);
		strvec_pushf(&cpr.args, "--super-prefix=%s/", displaypath);

		எனில் (flags & OPT_CACHED)
			strvec_தள்ளு(&cpr.args, "--cached");

		எனில் (flags & OPT_QUIET)
			strvec_தள்ளு(&cpr.args, "--quiet");

		res = run_command(&cpr);
		எனில் (res == SIGPIPE + 128)
			எழுப்பு(SIGPIPE);
		இல்லையெனில் எனில் (res)
			die(_("failed to recurse into submodule '%s'"), path);
	}

cleanup:
	strvec_clear(&diff_files_args);
	விடுவி(displaypath);
	release_revisions(&rev);
}

நிலையான வெறுமை status_submodule_cb(மாறிலி கட்டமைப்பு cache_entry *list_item,
				வெறுமை *cb_data)
{
	கட்டமைப்பு status_cb *info = cb_data;

	status_submodule(list_item->name, &list_item->oid, list_item->ce_flags,
			 info->prefix, info->super_prefix, info->flags);
}

நிலையான முழு module_status(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
			 கட்டமைப்பு repository *repo UNUSED)
{
	கட்டமைப்பு status_cb info = STATUS_CB_INIT;
	கட்டமைப்பு pathspec pathspec = { 0 };
	கட்டமைப்பு module_list list = MODULE_LIST_INIT;
	முழு quiet = 0;
	கட்டமைப்பு option module_status_options[] = {
		OPT__SUPER_PREFIX(&info.super_prefix),
		OPT__QUIET(&quiet, N_("suppress submodule status output")),
		OPT_BIT(0, "cached", &info.flags, N_("use commit stored in the index instead of the one stored in the submodule HEAD"), OPT_CACHED),
		OPT_BIT(0, "recursive", &info.flags, N_("recurse into nested submodules"), OPT_RECURSIVE),
		OPT_END()
	};
	மாறிலி எழுத்து *மாறிலி git_submodule_helper_usage[] = {
		N_("git submodule status [--quiet] [--cached] [--recursive] [<path>...]"),
		வெற்று
	};
	முழு ret = 1;

	argc = parse_options(argc, argv, prefix, module_status_options,
			     git_submodule_helper_usage, 0);

	எனில் (module_list_compute(argv, prefix, &pathspec, &list) < 0)
		செல் cleanup;

	info.prefix = prefix;
	எனில் (quiet)
		info.flags |= OPT_QUIET;

	for_each_listed_submodule(&list, status_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	திரும்பு ret;
}

கட்டமைப்பு module_cb {
	unsigned முழு mod_src;
	unsigned முழு mod_dst;
	கட்டமைப்பு object_id oid_src;
	கட்டமைப்பு object_id oid_dst;
	எழுத்து status;
	எழுத்து *sm_path;
};
#define MODULE_CB_INIT { 0 }

நிலையான வெறுமை module_cb_release(கட்டமைப்பு module_cb *mcb)
{
	விடுவி(mcb->sm_path);
}

கட்டமைப்பு module_cb_list {
	கட்டமைப்பு module_cb **entries;
	முழு alloc, nr;
};
#define MODULE_CB_LIST_INIT { 0 }

நிலையான வெறுமை module_cb_list_release(கட்டமைப்பு module_cb_list *mcbl)
{
	முழு i;

	க்கு (i = 0; i < mcbl->nr; i++) {
		கட்டமைப்பு module_cb *mcb = mcbl->entries[i];

		module_cb_release(mcb);
		விடுவி(mcb);
	}
	விடுவி(mcbl->entries);
}

கட்டமைப்பு summary_cb {
	முழு argc;
	மாறிலி எழுத்து **argv;
	மாறிலி எழுத்து *prefix;
	மாறிலி எழுத்து *super_prefix;
	unsigned முழு cached: 1;
	unsigned முழு for_status: 1;
	unsigned முழு files: 1;
	முழு summary_limit;
};
#define SUMMARY_CB_INIT { 0 }

எண்ணிக்கை diff_cmd {
	DIFF_INDEX,
	DIFF_FILES
};

நிலையான எழுத்து *verify_submodule_committish(மாறிலி எழுத்து *sm_path,
					 மாறிலி எழுத்து *committish)
{
	கட்டமைப்பு child_process cp_rev_parse = CHILD_PROCESS_INIT;
	கட்டமைப்பு strbuf result = STRBUF_INIT;

	cp_rev_parse.git_cmd = 1;
	cp_rev_parse.dir = sm_path;
	prepare_submodule_repo_env(&cp_rev_parse.env);
	strvec_pushl(&cp_rev_parse.args, "rev-parse", "-q", "--short", வெற்று);
	strvec_pushf(&cp_rev_parse.args, "%s^0", committish);
	strvec_தள்ளு(&cp_rev_parse.args, "--");

	எனில் (capture_command(&cp_rev_parse, &result, 0))
		திரும்பு வெற்று;

	strbuf_trim_trailing_newline(&result);
	திரும்பு strbuf_detach(&result, வெற்று);
}

நிலையான வெறுமை print_submodule_summary(கட்டமைப்பு summary_cb *info, மாறிலி எழுத்து *errmsg,
				    முழு total_commits, மாறிலி எழுத்து *displaypath,
				    மாறிலி எழுத்து *src_abbrev, மாறிலி எழுத்து *dst_abbrev,
				    கட்டமைப்பு module_cb *p)
{
	எனில் (p->status == 'T') {
		எனில் (S_ISGITLINK(p->mod_dst))
			printf(_("* %s %s(blob)->%s(submodule)"),
				 displaypath, src_abbrev, dst_abbrev);
		இல்லையெனில்
			printf(_("* %s %s(submodule)->%s(blob)"),
				 displaypath, src_abbrev, dst_abbrev);
	} இல்லையெனில் {
		printf("* %s %s...%s",
			displaypath, src_abbrev, dst_abbrev);
	}

	எனில் (total_commits < 0)
		printf(":\n");
	இல்லையெனில்
		printf(" (%d):\n", total_commits);

	எனில் (errmsg) {
		printf(_("%s"), errmsg);
	} இல்லையெனில் எனில் (total_commits > 0) {
		கட்டமைப்பு child_process cp_log = CHILD_PROCESS_INIT;

		cp_log.git_cmd = 1;
		cp_log.dir = p->sm_path;
		prepare_submodule_repo_env(&cp_log.env);
		strvec_pushl(&cp_log.args, "log", வெற்று);

		எனில் (S_ISGITLINK(p->mod_src) && S_ISGITLINK(p->mod_dst)) {
			எனில் (info->summary_limit > 0)
				strvec_pushf(&cp_log.args, "-%d",
					     info->summary_limit);

			strvec_pushl(&cp_log.args, "--pretty=  %m %s",
				     "--first-parent", வெற்று);
			strvec_pushf(&cp_log.args, "%s...%s",
				     src_abbrev, dst_abbrev);
		} இல்லையெனில் எனில் (S_ISGITLINK(p->mod_dst)) {
			strvec_pushl(&cp_log.args, "--pretty=  > %s",
				     "-1", dst_abbrev, வெற்று);
		} இல்லையெனில் {
			strvec_pushl(&cp_log.args, "--pretty=  < %s",
				     "-1", src_abbrev, வெற்று);
		}
		run_command(&cp_log);
	}
	printf("\n");
}

நிலையான வெறுமை generate_submodule_summary(கட்டமைப்பு summary_cb *info,
				       கட்டமைப்பு module_cb *p)
{
	எழுத்து *displaypath, *src_abbrev = வெற்று, *dst_abbrev;
	முழு missing_src = 0, missing_dst = 0;
	கட்டமைப்பு strbuf errmsg = STRBUF_INIT;
	முழு total_commits = -1;

	எனில் (!info->cached && oideq(&p->oid_dst, null_oid(the_hash_algo))) {
		எனில் (S_ISGITLINK(p->mod_dst)) {
			கட்டமைப்பு ref_store *refs = repo_get_submodule_ref_store(the_repository,
									      p->sm_path);

			எனில் (refs)
				refs_head_ref(refs, handle_submodule_head_ref, &p->oid_dst);
		} இல்லையெனில் எனில் (S_ISLNK(p->mod_dst) || S_ISREG(p->mod_dst)) {
			கட்டமைப்பு stat st;
			முழு fd = open(p->sm_path, O_RDONLY);

			எனில் (fd < 0 || fstat(fd, &st) < 0 ||
			    index_fd(the_repository->index, &p->oid_dst, fd, &st, OBJ_BLOB,
				     p->sm_path, 0))
				error(_("couldn't hash object from '%s'"), p->sm_path);
		} இல்லையெனில் {
			/* for a submodule removal (mode:0000000), don't warn */
			எனில் (p->mod_dst)
				warning(_("unexpected mode %o"), p->mod_dst);
		}
	}

	எனில் (S_ISGITLINK(p->mod_src)) {
		எனில் (p->status != 'D')
			src_abbrev = verify_submodule_committish(p->sm_path,
								 oid_to_hex(&p->oid_src));
		எனில் (!src_abbrev) {
			missing_src = 1;
			/*
			 * As `rev-parse` failed, we fallback to getting
			 * the abbreviated hash using oid_src. We do
			 * this as we might still need the abbreviated
			 * hash in cases like a submodule type change, etc.
			 */
			src_abbrev = xstrndup(oid_to_hex(&p->oid_src), 7);
		}
	} இல்லையெனில் {
		/*
		 * The source does not point to a submodule.
		 * So, we fallback to getting the abbreviation using
		 * oid_src as we might still need the abbreviated
		 * hash in cases like submodule add, etc.
		 */
		src_abbrev = xstrndup(oid_to_hex(&p->oid_src), 7);
	}

	எனில் (S_ISGITLINK(p->mod_dst)) {
		dst_abbrev = verify_submodule_committish(p->sm_path,
							 oid_to_hex(&p->oid_dst));
		எனில் (!dst_abbrev) {
			missing_dst = 1;
			/*
			 * As `rev-parse` failed, we fallback to getting
			 * the abbreviated hash using oid_dst. We do
			 * this as we might still need the abbreviated
			 * hash in cases like a submodule type change, etc.
			 */
			dst_abbrev = xstrndup(oid_to_hex(&p->oid_dst), 7);
		}
	} இல்லையெனில் {
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

	எனில் (!missing_src && !missing_dst) {
		கட்டமைப்பு child_process cp_rev_list = CHILD_PROCESS_INIT;
		கட்டமைப்பு strbuf sb_rev_list = STRBUF_INIT;

		strvec_pushl(&cp_rev_list.args, "rev-list",
			     "--first-parent", "--count", வெற்று);
		எனில் (S_ISGITLINK(p->mod_src) && S_ISGITLINK(p->mod_dst))
			strvec_pushf(&cp_rev_list.args, "%s...%s",
				     src_abbrev, dst_abbrev);
		இல்லையெனில்
			strvec_தள்ளு(&cp_rev_list.args, S_ISGITLINK(p->mod_src) ?
				    src_abbrev : dst_abbrev);
		strvec_தள்ளு(&cp_rev_list.args, "--");

		cp_rev_list.git_cmd = 1;
		cp_rev_list.dir = p->sm_path;
		prepare_submodule_repo_env(&cp_rev_list.env);

		எனில் (!capture_command(&cp_rev_list, &sb_rev_list, 0))
			total_commits = atoi(sb_rev_list.buf);

		strbuf_release(&sb_rev_list);
	} இல்லையெனில் {
		/*
		 * Don't give error msg for modification whose dst is not
		 * submodule, i.e., deleted or changed to blob
		 */
		எனில் (S_ISGITLINK(p->mod_dst)) {
			எனில் (missing_src && missing_dst) {
				strbuf_addf(&errmsg, "  Warn: %s doesn't contain commits %s and %s\n",
					    displaypath, oid_to_hex(&p->oid_src),
					    oid_to_hex(&p->oid_dst));
			} இல்லையெனில் {
				strbuf_addf(&errmsg, "  Warn: %s doesn't contain commit %s\n",
					    displaypath, missing_src ?
					    oid_to_hex(&p->oid_src) :
					    oid_to_hex(&p->oid_dst));
			}
		}
	}

	print_submodule_summary(info, errmsg.len ? errmsg.buf : வெற்று,
				total_commits, displaypath, src_abbrev,
				dst_abbrev, p);

	விடுவி(displaypath);
	விடுவி(src_abbrev);
	விடுவி(dst_abbrev);
	strbuf_release(&errmsg);
}

நிலையான வெறுமை prepare_submodule_summary(கட்டமைப்பு summary_cb *info,
				      கட்டமைப்பு module_cb_list *list)
{
	முழு i;
	க்கு (i = 0; i < list->nr; i++) {
		மாறிலி கட்டமைப்பு submodule *sub;
		கட்டமைப்பு module_cb *p = list->entries[i];
		கட்டமைப்பு strbuf sm_gitdir = STRBUF_INIT;

		எனில் (p->status == 'D' || p->status == 'T') {
			generate_submodule_summary(info, p);
			தொடர்;
		}

		எனில் (info->for_status && p->status != 'A' &&
		    (sub = submodule_from_path(the_repository,
					       null_oid(the_hash_algo), p->sm_path))) {
			எழுத்து *config_சாவி = வெற்று;
			மாறிலி எழுத்து *மதிப்பு;
			முழு ignore_all = 0;

			config_சாவி = xstrfmt("submodule.%s.ignore",
					     sub->name);
			எனில் (!repo_config_get_string_tmp(the_repository, config_சாவி, &மதிப்பு))
				ignore_all = !strcmp(மதிப்பு, "all");
			இல்லையெனில் எனில் (sub->ignore)
				ignore_all = !strcmp(sub->ignore, "all");

			விடுவி(config_சாவி);
			எனில் (ignore_all)
				தொடர்;
		}

		/* Also show added or modified modules which are checked out */
		strbuf_addstr(&sm_gitdir, p->sm_path);
		எனில் (is_nonbare_repository_dir(&sm_gitdir))
			generate_submodule_summary(info, p);
		strbuf_release(&sm_gitdir);
	}
}

நிலையான வெறுமை submodule_summary_callback(கட்டமைப்பு diff_queue_struct *q,
				       கட்டமைப்பு diff_options *options UNUSED,
				       வெறுமை *data)
{
	முழு i;
	கட்டமைப்பு module_cb_list *list = data;
	க்கு (i = 0; i < q->nr; i++) {
		கட்டமைப்பு diff_filepair *p = q->queue[i];
		கட்டமைப்பு module_cb *temp;

		எனில் (!S_ISGITLINK(p->one->mode) && !S_ISGITLINK(p->two->mode))
			தொடர்;
		temp = xmalloc(அளவு(*temp));
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

நிலையான மாறிலி எழுத்து *get_diff_cmd(எண்ணிக்கை diff_cmd diff_cmd)
{
	தேர்வு (diff_cmd) {
	நிலை DIFF_INDEX: திரும்பு "diff-index";
	நிலை DIFF_FILES: திரும்பு "diff-files";
	இயல்பு: BUG("bad diff_cmd value %d", diff_cmd);
	}
}

நிலையான முழு compute_summary_module_list(கட்டமைப்பு object_id *head_oid,
				       கட்டமைப்பு summary_cb *info,
				       எண்ணிக்கை diff_cmd diff_cmd)
{
	கட்டமைப்பு strvec diff_args = STRVEC_INIT;
	கட்டமைப்பு rev_info rev;
	கட்டமைப்பு module_cb_list list = MODULE_CB_LIST_INIT;
	முழு ret = 0;

	strvec_தள்ளு(&diff_args, get_diff_cmd(diff_cmd));
	எனில் (info->cached)
		strvec_தள்ளு(&diff_args, "--cached");
	strvec_pushl(&diff_args, "--ignore-submodules=dirty", "--raw", வெற்று);
	எனில் (head_oid)
		strvec_தள்ளு(&diff_args, oid_to_hex(head_oid));
	strvec_தள்ளு(&diff_args, "--");
	எனில் (info->argc)
		strvec_pushv(&diff_args, info->argv);

	repo_config(the_repository, git_diff_basic_config, வெற்று);
	repo_init_revisions(the_repository, &rev, info->prefix);
	rev.abbrev = 0;
	precompose_argv_prefix(diff_args.nr, diff_args.v, வெற்று);
	setup_revisions_from_strvec(&diff_args, &rev, வெற்று);
	rev.diffopt.output_format = DIFF_FORMAT_NO_OUTPUT | DIFF_FORMAT_CALLBACK;
	rev.diffopt.format_callback = submodule_summary_callback;
	rev.diffopt.format_callback_data = &list;

	எனில் (!info->cached) {
		எனில் (diff_cmd == DIFF_INDEX)
			setup_work_மரம்(the_repository);
		எனில் (repo_read_index_preload(the_repository, &rev.diffopt.pathspec, 0) < 0) {
			perror("repo_read_index_preload");
			ret = -1;
			செல் cleanup;
		}
	} இல்லையெனில் எனில் (repo_read_index(the_repository) < 0) {
		perror("repo_read_cache");
		ret = -1;
		செல் cleanup;
	}

	எனில் (diff_cmd == DIFF_INDEX)
		run_diff_index(&rev, info->cached ? DIFF_INDEX_CACHED : 0);
	இல்லையெனில்
		run_diff_files(&rev, 0);
	prepare_submodule_summary(info, &list);
cleanup:
	strvec_clear(&diff_args);
	release_revisions(&rev);
	module_cb_list_release(&list);
	திரும்பு ret;
}

நிலையான முழு module_summary(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
			  கட்டமைப்பு repository *repo UNUSED)
{
	கட்டமைப்பு summary_cb info = SUMMARY_CB_INIT;
	முழு cached = 0;
	முழு for_status = 0;
	முழு files = 0;
	முழு summary_limit = -1;
	எண்ணிக்கை diff_cmd diff_cmd = DIFF_INDEX;
	கட்டமைப்பு object_id head_oid;
	முழு ret;
	கட்டமைப்பு option module_summary_options[] = {
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
	மாறிலி எழுத்து *மாறிலி git_submodule_helper_usage[] = {
		N_("git submodule summary [<options>] [<commit>] [--] [<path>]"),
		வெற்று
	};

	argc = parse_options(argc, argv, prefix, module_summary_options,
			     git_submodule_helper_usage, 0);

	எனில் (!summary_limit)
		திரும்பு 0;

	எனில் (!repo_get_oid(the_repository, argc ? argv[0] : "HEAD", &head_oid)) {
		எனில் (argc) {
			argv++;
			argc--;
		}
	} இல்லையெனில் எனில் (!argc || !strcmp(argv[0], "HEAD")) {
		/* before the first commit: compare with an empty tree */
		oidcpy(&head_oid, the_hash_algo->empty_மரம்);
		எனில் (argc) {
			argv++;
			argc--;
		}
	} இல்லையெனில் {
		எனில் (repo_get_oid(the_repository, "HEAD", &head_oid))
			die(_("could not fetch a revision for HEAD"));
	}

	எனில் (files) {
		எனில் (cached)
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

	ret = compute_summary_module_list((diff_cmd == DIFF_INDEX) ? &head_oid : வெற்று,
					  &info, diff_cmd);
	திரும்பு ret;
}

நிலையான முழு module_gitdir(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix UNUSED,
			 கட்டமைப்பு repository *repo)
{
	கட்டமைப்பு strbuf gitdir = STRBUF_INIT;

	எனில் (argc != 2)
		usage(_("git submodule--helper gitdir <name>"));

	submodule_name_to_gitdir(&gitdir, repo, argv[1]);

	printf("%s\n", gitdir.buf);

	strbuf_release(&gitdir);
	திரும்பு 0;
}

நிலையான முழு module_migrate(முழு argc UNUSED, மாறிலி எழுத்து **argv UNUSED,
			  மாறிலி எழுத்து *prefix UNUSED, கட்டமைப்பு repository *repo)
{
	கட்டமைப்பு strbuf module_dir = STRBUF_INIT;
	DIR *dir;
	கட்டமைப்பு dirent *de;
	முழு repo_version = 0;

	repo_git_path_append(repo, &module_dir, "modules/");

	dir = opendir(module_dir.buf);
	எனில் (!dir)
		die(_("could not open '%s'"), module_dir.buf);

	போது ((de = readdir(dir))) {
		கட்டமைப்பு strbuf gitdir_path = STRBUF_INIT;
		எழுத்து *சாவி;
		மாறிலி எழுத்து *மதிப்பு;

		எனில் (is_dot_or_dotdot(de->d_name))
			தொடர்;

		strbuf_addf(&gitdir_path, "%s/%s", module_dir.buf, de->d_name);
		எனில் (!is_git_directory(gitdir_path.buf)) {
			strbuf_release(&gitdir_path);
			தொடர்;
		}
		strbuf_release(&gitdir_path);

		சாவி = xstrfmt("submodule.%s.gitdir", de->d_name);
		எனில் (!repo_config_get_string_tmp(repo, சாவி, &மதிப்பு)) {
			/* Already has a gitdir config, nothing to do. */
			விடுவி(சாவி);
			தொடர்;
		}
		விடுவி(சாவி);

		create_default_gitdir_config(de->d_name);
	}

	closedir(dir);
	strbuf_release(&module_dir);

	repo_config_get_int(the_repository, "core.repositoryformatversion", &repo_version);
	எனில் (repo_version == 0 &&
	    repo_config_set_gently(repo, "core.repositoryformatversion", "1"))
		die(_("could not set core.repositoryformatversion to 1.\n"
		      "Please set it for migration to work, for example:\n"
		      "git config core.repositoryformatversion 1"));

	எனில் (repo_config_set_gently(repo, "extensions.submodulePathConfig", "true"))
		die(_("could not enable submodulePathConfig extension. It is required\n"
		      "for migration to work. Please enable it in the root repo:\n"
		      "git config extensions.submodulePathConfig true"));

	repo->repository_format_submodule_path_cfg = 1;

	திரும்பு 0;
}

கட்டமைப்பு sync_cb {
	மாறிலி எழுத்து *prefix;
	மாறிலி எழுத்து *super_prefix;
	unsigned முழு flags;
};
#define SYNC_CB_INIT { 0 }

நிலையான வெறுமை sync_submodule(மாறிலி எழுத்து *path, மாறிலி எழுத்து *prefix,
			   மாறிலி எழுத்து *super_prefix, unsigned முழு flags)
{
	மாறிலி கட்டமைப்பு submodule *sub;
	எழுத்து *remote_சாவி = வெற்று;
	எழுத்து *sub_origin_url, *super_config_url, *displaypath, *default_remote;
	கட்டமைப்பு strbuf sb = STRBUF_INIT;
	எழுத்து *sub_config_path = வெற்று;
	முழு code;

	எனில் (!is_submodule_active(the_repository, path))
		திரும்பு;

	எனில் (validate_submodule_path(path) < 0)
		die(வெற்று);

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);

	எனில் (sub && sub->url) {
		எனில் (starts_with_dot_dot_slash(sub->url) ||
		    starts_with_dot_slash(sub->url)) {
			எழுத்து *up_path = get_up_path(path);

			sub_origin_url = resolve_relative_url(sub->url, up_path, 1);
			super_config_url = resolve_relative_url(sub->url, வெற்று, 1);
			விடுவி(up_path);
		} இல்லையெனில் {
			sub_origin_url = xstrdup(sub->url);
			super_config_url = xstrdup(sub->url);
		}
	} இல்லையெனில் {
		sub_origin_url = xstrdup("");
		super_config_url = xstrdup("");
	}

	displaypath = get_submodule_displaypath(path, prefix, super_prefix);

	எனில் (!(flags & OPT_QUIET))
		printf(_("Synchronizing submodule url for '%s'\n"),
			 displaypath);

	strbuf_reset(&sb);
	strbuf_addf(&sb, "submodule.%s.url", sub->name);
	எனில் (repo_config_set_gently(the_repository, sb.buf, super_config_url))
		die(_("failed to register url for submodule path '%s'"),
		      displaypath);

	எனில் (!is_submodule_populated_gently(path, வெற்று))
		செல் cleanup;

	strbuf_reset(&sb);
	code = get_default_remote_submodule(path, &default_remote);
	எனில் (code)
		exit(code);

	remote_சாவி = xstrfmt("remote.%s.url", default_remote);
	விடுவி(default_remote);

	submodule_to_gitdir(the_repository, &sb, path);
	strbuf_addstr(&sb, "/config");

	எனில் (repo_config_set_in_file_gently(the_repository, sb.buf, remote_சாவி, வெற்று, sub_origin_url))
		die(_("failed to update remote for submodule '%s'"),
		      path);

	எனில் (flags & OPT_RECURSIVE) {
		கட்டமைப்பு child_process cpr = CHILD_PROCESS_INIT;

		cpr.git_cmd = 1;
		cpr.dir = path;
		prepare_submodule_repo_env(&cpr.env);

		strvec_pushl(&cpr.args, "submodule--helper", "sync",
			     "--recursive", வெற்று);
		strvec_pushf(&cpr.args, "--super-prefix=%s/", displaypath);

		எனில் (flags & OPT_QUIET)
			strvec_தள்ளு(&cpr.args, "--quiet");

		எனில் (run_command(&cpr))
			die(_("failed to recurse into submodule '%s'"),
			      path);
	}

cleanup:
	விடுவி(super_config_url);
	விடுவி(sub_origin_url);
	strbuf_release(&sb);
	விடுவி(remote_சாவி);
	விடுவி(displaypath);
	விடுவி(sub_config_path);
}

நிலையான வெறுமை sync_submodule_cb(மாறிலி கட்டமைப்பு cache_entry *list_item, வெறுமை *cb_data)
{
	கட்டமைப்பு sync_cb *info = cb_data;

	sync_submodule(list_item->name, info->prefix, info->super_prefix,
		       info->flags);
}

நிலையான முழு module_sync(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
		       கட்டமைப்பு repository *repo UNUSED)
{
	கட்டமைப்பு sync_cb info = SYNC_CB_INIT;
	கட்டமைப்பு pathspec pathspec = { 0 };
	கட்டமைப்பு module_list list = MODULE_LIST_INIT;
	முழு quiet = 0;
	முழு recursive = 0;
	கட்டமைப்பு option module_sync_options[] = {
		OPT__SUPER_PREFIX(&info.super_prefix),
		OPT__QUIET(&quiet, N_("suppress output of synchronizing submodule url")),
		OPT_BOOL(0, "recursive", &recursive,
			N_("recurse into nested submodules")),
		OPT_END()
	};
	மாறிலி எழுத்து *மாறிலி git_submodule_helper_usage[] = {
		N_("git submodule sync [--quiet] [--recursive] [<path>]"),
		வெற்று
	};
	முழு ret = 1;

	argc = parse_options(argc, argv, prefix, module_sync_options,
			     git_submodule_helper_usage, 0);

	எனில் (module_list_compute(argv, prefix, &pathspec, &list) < 0)
		செல் cleanup;

	info.prefix = prefix;
	எனில் (quiet)
		info.flags |= OPT_QUIET;
	எனில் (recursive)
		info.flags |= OPT_RECURSIVE;

	for_each_listed_submodule(&list, sync_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	திரும்பு ret;
}

கட்டமைப்பு deinit_cb {
	மாறிலி எழுத்து *prefix;
	unsigned முழு flags;
};
#define DEINIT_CB_INIT { 0 }

நிலையான வெறுமை deinit_submodule(மாறிலி எழுத்து *path, மாறிலி எழுத்து *prefix,
			     unsigned முழு flags)
{
	மாறிலி கட்டமைப்பு submodule *sub;
	எழுத்து *displaypath = வெற்று;
	கட்டமைப்பு child_process cp_config = CHILD_PROCESS_INIT;
	கட்டமைப்பு strbuf sb_config = STRBUF_INIT;
	எழுத்து *sub_git_dir = xstrfmt("%s/.git", path);

	எனில் (validate_submodule_path(path) < 0)
		die(வெற்று);

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);

	எனில் (!sub || !sub->name)
		செல் cleanup;

	displaypath = get_submodule_displaypath(path, prefix, வெற்று);

	/* remove the submodule work tree (unless the user already did it) */
	எனில் (is_directory(path)) {
		கட்டமைப்பு strbuf sb_rm = STRBUF_INIT;
		மாறிலி எழுத்து *format;

		எனில் (is_directory(sub_git_dir)) {
			எனில் (!(flags & OPT_QUIET))
				warning(_("Submodule work tree '%s' contains a .git "
					  "directory. This will be replaced with a "
					  ".git file by using absorbgitdirs."),
					displaypath);

			absorb_git_dir_into_superproject(path, வெற்று);

		}

		எனில் (!(flags & OPT_FORCE)) {
			கட்டமைப்பு child_process cp_rm = CHILD_PROCESS_INIT;

			cp_rm.git_cmd = 1;
			strvec_pushl(&cp_rm.args, "rm", "-qn",
				     path, வெற்று);

			எனில் (run_command(&cp_rm))
				die(_("Submodule work tree '%s' contains local "
				      "modifications; use '-f' to discard them"),
				      displaypath);
		}

		strbuf_addstr(&sb_rm, path);

		எனில் (!remove_dir_recursively(&sb_rm, 0))
			format = _("Cleared directory '%s'\n");
		இல்லையெனில்
			format = _("Could not remove submodule work tree '%s'\n");

		எனில் (!(flags & OPT_QUIET))
			printf(format, displaypath);

		submodule_unset_core_worktree(sub);

		strbuf_release(&sb_rm);
	}

	எனில் (mkdir(path, 0777))
		printf(_("could not create empty submodule directory %s"),
		      displaypath);

	cp_config.git_cmd = 1;
	strvec_pushl(&cp_config.args, "config", "--get-regexp", வெற்று);
	strvec_pushf(&cp_config.args, "submodule.%s\\.", sub->name);

	/* remove the .git/config entries (unless the user already did it) */
	எனில் (!capture_command(&cp_config, &sb_config, 0) && sb_config.len) {
		எழுத்து *sub_சாவி = xstrfmt("submodule.%s", sub->name);

		/*
		 * remove the whole section so we have a clean state when
		 * the user later decides to init this submodule again
		 */
		repo_config_rename_section_in_file(the_repository, வெற்று, sub_சாவி, வெற்று);
		எனில் (!(flags & OPT_QUIET))
			printf(_("Submodule '%s' (%s) unregistered for path '%s'\n"),
				 sub->name, sub->url, displaypath);
		விடுவி(sub_சாவி);
	}

cleanup:
	விடுவி(displaypath);
	விடுவி(sub_git_dir);
	strbuf_release(&sb_config);
}

நிலையான வெறுமை deinit_submodule_cb(மாறிலி கட்டமைப்பு cache_entry *list_item,
				வெறுமை *cb_data)
{
	கட்டமைப்பு deinit_cb *info = cb_data;
	deinit_submodule(list_item->name, info->prefix, info->flags);
}

நிலையான முழு module_deinit(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
			 கட்டமைப்பு repository *repo UNUSED)
{
	கட்டமைப்பு deinit_cb info = DEINIT_CB_INIT;
	கட்டமைப்பு pathspec pathspec = { 0 };
	கட்டமைப்பு module_list list = MODULE_LIST_INIT;
	முழு quiet = 0;
	முழு force = 0;
	முழு all = 0;
	கட்டமைப்பு option module_deinit_options[] = {
		OPT__QUIET(&quiet, N_("suppress submodule status output")),
		OPT__FORCE(&force, N_("remove submodule working trees even if they contain local changes"), 0),
		OPT_BOOL(0, "all", &all, N_("unregister all submodules")),
		OPT_END()
	};
	மாறிலி எழுத்து *மாறிலி git_submodule_helper_usage[] = {
		N_("git submodule deinit [--quiet] [-f | --force] [--all | [--] [<path>...]]"),
		வெற்று
	};
	முழு ret = 1;

	argc = parse_options(argc, argv, prefix, module_deinit_options,
			     git_submodule_helper_usage, 0);

	எனில் (all && argc) {
		error("pathspec and --all are incompatible");
		usage_with_options(git_submodule_helper_usage,
				   module_deinit_options);
	}

	எனில் (!argc && !all)
		die(_("Use '--all' if you really want to deinitialize all submodules"));

	எனில் (module_list_compute(argv, prefix, &pathspec, &list) < 0)
		செல் cleanup;

	info.prefix = prefix;
	எனில் (quiet)
		info.flags |= OPT_QUIET;
	எனில் (force)
		info.flags |= OPT_FORCE;

	for_each_listed_submodule(&list, deinit_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	திரும்பு ret;
}

கட்டமைப்பு module_clone_data {
	மாறிலி எழுத்து *prefix;
	மாறிலி எழுத்து *path;
	மாறிலி எழுத்து *name;
	மாறிலி எழுத்து *url;
	முழு depth;
	கட்டமைப்பு list_objects_filter_options *filter_options;
	எண்ணிக்கை ref_storage_format ref_storage_format;
	unsigned முழு quiet: 1;
	unsigned முழு progress: 1;
	unsigned முழு dissociate: 1;
	unsigned முழு require_init: 1;
	முழு single_branch;
};
#define MODULE_CLONE_DATA_INIT { \
	.single_branch = -1, \
	.ref_storage_format = REF_STORAGE_FORMAT_UNKNOWN, \
}

கட்டமைப்பு submodule_alternate_setup {
	மாறிலி எழுத்து *submodule_name;
	எண்ணிக்கை SUBMODULE_ALTERNATE_ERROR_MODE {
		SUBMODULE_ALTERNATE_ERROR_DIE,
		SUBMODULE_ALTERNATE_ERROR_INFO,
		SUBMODULE_ALTERNATE_ERROR_IGNORE
	} error_mode;
	கட்டமைப்பு string_list *reference;
};
#define SUBMODULE_ALTERNATE_SETUP_INIT { \
	.error_mode = SUBMODULE_ALTERNATE_ERROR_IGNORE, \
}

நிலையான மாறிலி எழுத்து alternate_error_advice[] = N_(
"An alternate computed from a superproject's alternate is invalid.\n"
"To allow Git to clone without an alternate in such a case, set\n"
"submodule.alternateErrorStrategy to 'info' or, equivalently, clone with\n"
"'--reference-if-able' instead of '--reference'."
);

நிலையான முழு add_possible_reference_from_superproject(
		கட்டமைப்பு odb_files_dir *alt_odb, வெறுமை *sas_cb)
{
	கட்டமைப்பு submodule_alternate_setup *sas = sas_cb;
	size_t len;

	/*
	 * If the alternate object store is another repository, try the
	 * standard layout with .git/(modules/<name>)+/objects
	 */
	எனில் (strip_suffix(alt_odb->abspath, "/objects", &len)) {
		கட்டமைப்பு repository alternate;
		எழுத்து *sm_alternate;
		கட்டமைப்பு strbuf sb = STRBUF_INIT;
		கட்டமைப்பு strbuf err = STRBUF_INIT;
		strbuf_add(&sb, alt_odb->abspath, len);

		எனில் (repo_init(&alternate, sb.buf, வெற்று) < 0)
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
		எனில் (sm_alternate) {
			எழுத்து *p = strbuf_detach(&sb, வெற்று);

			string_list_append(sas->reference, p)->util = p;
			விடுவி(sm_alternate);
		} இல்லையெனில் {
			தேர்வு (sas->error_mode) {
			நிலை SUBMODULE_ALTERNATE_ERROR_DIE:
				எனில் (advice_enabled(ADVICE_SUBMODULE_ALTERNATE_ERROR_STRATEGY_DIE))
					advise(_(alternate_error_advice));
				die(_("submodule '%s' cannot add alternate: %s"),
				    sas->submodule_name, err.buf);
			நிலை SUBMODULE_ALTERNATE_ERROR_INFO:
				fprintf_ln(stderr, _("submodule '%s' cannot add alternate: %s"),
					sas->submodule_name, err.buf);
			நிலை SUBMODULE_ALTERNATE_ERROR_IGNORE:
				; /* nothing */
			}
		}

		strbuf_release(&err);
		strbuf_release(&sb);
	}

	திரும்பு 0;
}

நிலையான வெறுமை prepare_possible_alternates(மாறிலி எழுத்து *sm_name,
		கட்டமைப்பு string_list *reference)
{
	எழுத்து *sm_alternate = வெற்று, *error_strategy = வெற்று;
	கட்டமைப்பு submodule_alternate_setup sas = SUBMODULE_ALTERNATE_SETUP_INIT;

	repo_config_get_string(the_repository, "submodule.alternateLocation", &sm_alternate);
	எனில் (!sm_alternate)
		திரும்பு;

	repo_config_get_string(the_repository, "submodule.alternateErrorStrategy", &error_strategy);

	எனில் (!error_strategy)
		error_strategy = xstrdup("die");

	sas.submodule_name = sm_name;
	sas.reference = reference;
	எனில் (!strcmp(error_strategy, "die"))
		sas.error_mode = SUBMODULE_ALTERNATE_ERROR_DIE;
	இல்லையெனில் எனில் (!strcmp(error_strategy, "info"))
		sas.error_mode = SUBMODULE_ALTERNATE_ERROR_INFO;
	இல்லையெனில் எனில் (!strcmp(error_strategy, "ignore"))
		sas.error_mode = SUBMODULE_ALTERNATE_ERROR_IGNORE;
	இல்லையெனில்
		die(_("Value '%s' for submodule.alternateErrorStrategy is not recognized"), error_strategy);

	எனில் (!strcmp(sm_alternate, "superproject"))
		odb_for_each_alternate(the_repository->objects,
				       add_possible_reference_from_superproject, &sas);
	இல்லையெனில் எனில் (!strcmp(sm_alternate, "no"))
		; /* do nothing */
	இல்லையெனில்
		die(_("Value '%s' for submodule.alternateLocation is not recognized"), sm_alternate);

	விடுவி(sm_alternate);
	விடுவி(error_strategy);
}

நிலையான எழுத்து *clone_submodule_sm_gitdir(மாறிலி எழுத்து *name)
{
	கட்டமைப்பு strbuf sb = STRBUF_INIT;
	எழுத்து *sm_gitdir;

	submodule_name_to_gitdir(&sb, the_repository, name);
	sm_gitdir = absolute_pathdup(sb.buf);
	strbuf_release(&sb);

	திரும்பு sm_gitdir;
}

நிலையான முழு dir_contains_only_dotgit(மாறிலி எழுத்து *path)
{
	DIR *dir = opendir(path);
	கட்டமைப்பு dirent *e;
	முழு ret = 1;

	எனில் (!dir)
		திரும்பு 0;

	e = readdir_skip_dot_and_dotdot(dir);
	எனில் (!e)
		ret = 0;
	இல்லையெனில் எனில் (strcmp(DEFAULT_GIT_DIR_ENVIRONMENT, e->d_name) ||
		 (e = readdir_skip_dot_and_dotdot(dir))) {
		error("unexpected item '%s' in '%s'", e->d_name, path);
		ret = 0;
	}

	closedir(dir);
	திரும்பு ret;
}

நிலையான முழு clone_submodule(மாறிலி கட்டமைப்பு module_clone_data *clone_data,
			   கட்டமைப்பு string_list *reference)
{
	எழுத்து *p;
	எழுத்து *sm_gitdir = clone_submodule_sm_gitdir(clone_data->name);
	எழுத்து *sm_alternate = வெற்று, *error_strategy = வெற்று;
	கட்டமைப்பு stat st;
	கட்டமைப்பு child_process cp = CHILD_PROCESS_INIT;
	மாறிலி எழுத்து *clone_data_path = clone_data->path;
	எழுத்து *to_free = வெற்று;

	எனில் (validate_submodule_path(clone_data_path) < 0)
		die(வெற்று);

	எனில் (!is_absolute_path(clone_data->path))
		clone_data_path = to_free = xstrfmt("%s/%s", repo_get_work_மரம்(the_repository),
						    clone_data->path);

	எனில் (!file_exists(sm_gitdir)) {
		எனில் (clone_data->require_init && !stat(clone_data_path, &st) &&
		    !is_empty_dir(clone_data_path))
			die(_("directory not empty: '%s'"), clone_data_path);

		எனில் (safe_create_leading_directories_const(the_repository, sm_gitdir) < 0)
			die(_("could not create directory '%s'"), sm_gitdir);

		prepare_possible_alternates(clone_data->name, reference);

		strvec_தள்ளு(&cp.args, "clone");
		strvec_தள்ளு(&cp.args, "--no-checkout");
		எனில் (clone_data->quiet)
			strvec_தள்ளு(&cp.args, "--quiet");
		எனில் (clone_data->progress)
			strvec_தள்ளு(&cp.args, "--progress");
		எனில் (clone_data->depth > 0)
			strvec_pushf(&cp.args, "--depth=%d", clone_data->depth);
		எனில் (reference->nr) {
			கட்டமைப்பு string_list_item *item;

			for_each_string_list_item(item, reference)
				strvec_pushl(&cp.args, "--reference",
					     item->string, வெற்று);
		}
		எனில் (clone_data->ref_storage_format != REF_STORAGE_FORMAT_UNKNOWN)
			strvec_pushf(&cp.args, "--ref-storage-format=%s",
				     ref_storage_format_to_name(clone_data->ref_storage_format));
		எனில் (clone_data->dissociate)
			strvec_தள்ளு(&cp.args, "--dissociate");
		எனில் (sm_gitdir && *sm_gitdir)
			strvec_pushl(&cp.args, "--separate-git-dir", sm_gitdir, வெற்று);
		எனில் (clone_data->filter_options && clone_data->filter_options->choice)
			strvec_pushf(&cp.args, "--filter=%s",
				     expand_list_objects_filter_spec(
					     clone_data->filter_options));
		எனில் (clone_data->single_branch >= 0)
			strvec_தள்ளு(&cp.args, clone_data->single_branch ?
				    "--single-branch" :
				    "--no-single-branch");

		strvec_தள்ளு(&cp.args, "--");
		strvec_தள்ளு(&cp.args, clone_data->url);
		strvec_தள்ளு(&cp.args, clone_data_path);

		cp.git_cmd = 1;
		prepare_submodule_repo_env(&cp.env);
		cp.no_stdin = 1;

		எனில்(run_command(&cp))
			die(_("clone of '%s' into submodule path '%s' failed"),
			    clone_data->url, clone_data_path);

		எனில் (clone_data->require_init && !stat(clone_data_path, &st) &&
		    !dir_contains_only_dotgit(clone_data_path)) {
			எழுத்து *dot_git = xstrfmt("%s/.git", clone_data_path);
			unlink(dot_git);
			விடுவி(dot_git);
			die(_("directory not empty: '%s'"), clone_data_path);
		}
	} இல்லையெனில் {
		எழுத்து *path;

		எனில் (clone_data->require_init && !stat(clone_data_path, &st) &&
		    !is_empty_dir(clone_data_path))
			die(_("directory not empty: '%s'"), clone_data_path);
		எனில் (safe_create_leading_directories_const(the_repository, clone_data_path) < 0)
			die(_("could not create directory '%s'"), clone_data_path);
		path = xstrfmt("%s/index", sm_gitdir);
		unlink_or_warn(path);
		விடுவி(path);
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
	எனில் (validate_submodule_git_dir(sm_gitdir, clone_data->name) < 0) {
		எழுத்து *head = xstrfmt("%s/HEAD", sm_gitdir);
		unlink(head);
		விடுவி(head);
		die(_("refusing to create/use '%s' in another submodule's git dir. "
		      "Enabling extensions.submodulePathConfig should fix this."),
		    sm_gitdir);
	}

	connect_work_மரம்_and_git_dir(clone_data_path, sm_gitdir, 0);

	p = repo_submodule_path(the_repository, clone_data_path, "config");
	எனில் (!p)
		die(_("could not get submodule directory for '%s'"), clone_data_path);

	/* setup alternateLocation and alternateErrorStrategy in the cloned submodule if needed */
	repo_config_get_string(the_repository, "submodule.alternateLocation", &sm_alternate);
	எனில் (sm_alternate)
		repo_config_set_in_file(the_repository, p, "submodule.alternateLocation",
					sm_alternate);
	repo_config_get_string(the_repository, "submodule.alternateErrorStrategy", &error_strategy);
	எனில் (error_strategy)
		repo_config_set_in_file(the_repository, p, "submodule.alternateErrorStrategy",
					error_strategy);

	விடுவி(sm_alternate);
	விடுவி(error_strategy);

	விடுவி(sm_gitdir);
	விடுவி(p);
	விடுவி(to_free);
	திரும்பு 0;
}

நிலையான முழு module_clone(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
			கட்டமைப்பு repository *repo UNUSED)
{
	முழு dissociate = 0, quiet = 0, progress = 0, require_init = 0;
	கட்டமைப்பு module_clone_data clone_data = MODULE_CLONE_DATA_INIT;
	கட்டமைப்பு string_list reference = STRING_LIST_INIT_NODUP;
	கட்டமைப்பு list_objects_filter_options filter_options =
		LIST_OBJECTS_FILTER_INIT;
	மாறிலி எழுத்து *ref_storage_format = வெற்று;

	கட்டமைப்பு option module_clone_options[] = {
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
	மாறிலி எழுத்து *மாறிலி git_submodule_helper_usage[] = {
		N_("git submodule--helper clone [--prefix=<path>] [--quiet] "
		   "[--reference <repository>] [--name <name>] [--depth <depth>] "
		   "[--single-branch] [--filter <filter-spec>] "
		   "--url <url> --path <path>"),
		வெற்று
	};

	argc = parse_options(argc, argv, prefix, module_clone_options,
			     git_submodule_helper_usage, 0);

	எனில் (ref_storage_format) {
		clone_data.ref_storage_format = ref_storage_format_by_name(ref_storage_format);
		எனில் (clone_data.ref_storage_format == REF_STORAGE_FORMAT_UNKNOWN)
			die(_("unknown ref storage format '%s'"), ref_storage_format);
	}
	clone_data.dissociate = !!dissociate;
	clone_data.quiet = !!quiet;
	clone_data.progress = !!progress;
	clone_data.require_init = !!require_init;
	clone_data.filter_options = &filter_options;

	எனில் (argc || !clone_data.url || !clone_data.path || !*(clone_data.path))
		usage_with_options(git_submodule_helper_usage,
				   module_clone_options);

	clone_submodule(&clone_data, &reference);
	list_objects_filter_release(&filter_options);
	string_list_clear(&reference, 1);
	திரும்பு 0;
}

நிலையான முழு determine_submodule_update_strategy(கட்டமைப்பு repository *r,
					       முழு just_cloned,
					       மாறிலி எழுத்து *path,
					       எண்ணிக்கை submodule_update_type update,
					       கட்டமைப்பு submodule_update_strategy *out)
{
	மாறிலி கட்டமைப்பு submodule *sub = submodule_from_path(r, null_oid(the_hash_algo), path);
	எழுத்து *சாவி;
	மாறிலி எழுத்து *val;
	முழு ret;

	/*
	 * NEEDSWORK: audit and ensure that update_submodule() has right
	 * to assume that submodule_from_path() above will always succeed.
	 */
	எனில் (!sub)
		BUG("update_submodule assumes a submodule exists at path (%s)",
		    path);
	சாவி = xstrfmt("submodule.%s.update", sub->name);

	எனில் (update) {
		out->type = update;
	} இல்லையெனில் எனில் (!repo_config_get_string_tmp(r, சாவி, &val)) {
		எனில் (parse_submodule_update_strategy(val, out) < 0) {
			ret = die_message(_("Invalid update mode '%s' configured for submodule path '%s'"),
					  val, path);
			செல் cleanup;
		}
	} இல்லையெனில் எனில் (sub->update_strategy.type != SM_UPDATE_UNSPECIFIED) {
		எனில் (sub->update_strategy.type == SM_UPDATE_COMMAND)
			BUG("how did we read update = !command from .gitmodules?");
		out->type = sub->update_strategy.type;
		out->command = sub->update_strategy.command;
	} இல்லையெனில்
		out->type = SM_UPDATE_CHECKOUT;

	எனில் (just_cloned &&
	    (out->type == SM_UPDATE_MERGE ||
	     out->type == SM_UPDATE_REBASE ||
	     out->type == SM_UPDATE_NONE))
		out->type = SM_UPDATE_CHECKOUT;

	ret = 0;
cleanup:
	விடுவி(சாவி);
	திரும்பு ret;
}

கட்டமைப்பு update_clone_data {
	மாறிலி கட்டமைப்பு submodule *sub;
	கட்டமைப்பு object_id oid;
	unsigned just_cloned;
};

கட்டமைப்பு submodule_update_clone {
	/* index into 'update_data.list', the list of submodules to look into for cloning */
	முழு current;

	/* configuration parameters which are passed on to the children */
	மாறிலி கட்டமைப்பு update_data *update_data;

	/* to be consumed by update_submodule() */
	கட்டமைப்பு update_clone_data *update_clone;
	முழு update_clone_nr; முழு update_clone_alloc;

	/* If we want to stop as fast as possible and return an error */
	unsigned quickstop : 1;

	/* failed clones to be retried again */
	மாறிலி கட்டமைப்பு cache_entry **failed_clones;
	முழு failed_clones_nr, failed_clones_alloc;
};
#define SUBMODULE_UPDATE_CLONE_INIT { 0 }

நிலையான வெறுமை submodule_update_clone_release(கட்டமைப்பு submodule_update_clone *suc)
{
	விடுவி(suc->update_clone);
	விடுவி(suc->failed_clones);
}

கட்டமைப்பு update_data {
	மாறிலி எழுத்து *prefix;
	மாறிலி எழுத்து *super_prefix;
	எழுத்து *displaypath;
	எண்ணிக்கை submodule_update_type update_default;
	கட்டமைப்பு object_id suboid;
	கட்டமைப்பு string_list references;
	கட்டமைப்பு submodule_update_strategy update_strategy;
	கட்டமைப்பு list_objects_filter_options *filter_options;
	கட்டமைப்பு module_list list;
	எண்ணிக்கை ref_storage_format ref_storage_format;
	முழு depth;
	முழு max_jobs;
	முழு single_branch;
	முழு recommend_shallow;
	unsigned முழு require_init;
	unsigned முழு force;
	unsigned முழு quiet;
	unsigned முழு nofetch;
	unsigned முழு remote;
	unsigned முழு progress;
	unsigned முழு dissociate;
	unsigned முழு init;
	unsigned முழு warn_if_uninitialized;
	unsigned முழு recursive;

	/* copied over from update_clone_data */
	கட்டமைப்பு object_id oid;
	unsigned முழு just_cloned;
	மாறிலி எழுத்து *sm_path;
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

நிலையான வெறுமை update_data_release(கட்டமைப்பு update_data *ud)
{
	விடுவி(ud->displaypath);
	submodule_update_strategy_release(&ud->update_strategy);
	module_list_release(&ud->list);
}

நிலையான வெறுமை next_submodule_warn_missing(கட்டமைப்பு submodule_update_clone *suc,
		கட்டமைப்பு strbuf *out, மாறிலி எழுத்து *displaypath)
{
	/*
	 * Only mention uninitialized submodules when their
	 * paths have been specified.
	 */
	எனில் (suc->update_data->warn_if_uninitialized) {
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
நிலையான முழு prepare_to_clone_next_submodule(மாறிலி கட்டமைப்பு cache_entry *ce,
					   கட்டமைப்பு child_process *child,
					   கட்டமைப்பு submodule_update_clone *suc,
					   கட்டமைப்பு strbuf *out)
{
	மாறிலி கட்டமைப்பு submodule *sub = வெற்று;
	மாறிலி எழுத்து *url = வெற்று;
	மாறிலி எழுத்து *update_string;
	எண்ணிக்கை submodule_update_type update_type;
	எழுத்து *சாவி;
	மாறிலி கட்டமைப்பு update_data *ud = suc->update_data;
	எழுத்து *displaypath = get_submodule_displaypath(ce->name, ud->prefix,
						      ud->super_prefix);
	கட்டமைப்பு strbuf sb = STRBUF_INIT;
	முழு needs_cloning = 0;
	முழு need_free_url = 0;

	எனில் (ce_stage(ce)) {
		strbuf_addf(out, _("Skipping unmerged submodule %s"), displaypath);
		strbuf_addch(out, '\n');
		செல் cleanup;
	}

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), ce->name);

	எனில் (!sub) {
		next_submodule_warn_missing(suc, out, displaypath);
		செல் cleanup;
	}

	சாவி = xstrfmt("submodule.%s.update", sub->name);
	எனில் (!repo_config_get_string_tmp(the_repository, சாவி, &update_string)) {
		update_type = parse_submodule_update_type(update_string);
	} இல்லையெனில் {
		update_type = sub->update_strategy.type;
	}
	விடுவி(சாவி);

	எனில் (suc->update_data->update_strategy.type == SM_UPDATE_NONE
	    || (suc->update_data->update_strategy.type == SM_UPDATE_UNSPECIFIED
		&& update_type == SM_UPDATE_NONE)) {
		strbuf_addf(out, _("Skipping submodule '%s'"), displaypath);
		strbuf_addch(out, '\n');
		செல் cleanup;
	}

	/* Check if the submodule has been initialized. */
	எனில் (!is_submodule_active(the_repository, ce->name)) {
		next_submodule_warn_missing(suc, out, displaypath);
		செல் cleanup;
	}

	strbuf_reset(&sb);
	strbuf_addf(&sb, "submodule.%s.url", sub->name);
	எனில் (repo_config_get_string_tmp(the_repository, sb.buf, &url)) {
		எனில் (sub->url && (starts_with_dot_slash(sub->url) ||
				 starts_with_dot_dot_slash(sub->url))) {
			url = resolve_relative_url(sub->url, வெற்று, 0);
			need_free_url = 1;
		} இல்லையெனில்
			url = sub->url;
	}

	எனில் (!url)
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

	எனில் (!needs_cloning)
		செல் cleanup;

	child->git_cmd = 1;
	child->no_stdin = 1;
	child->stdout_to_stderr = 1;
	child->err = -1;
	strvec_தள்ளு(&child->args, "submodule--helper");
	strvec_தள்ளு(&child->args, "clone");
	எனில் (suc->update_data->progress)
		strvec_தள்ளு(&child->args, "--progress");
	எனில் (suc->update_data->quiet)
		strvec_தள்ளு(&child->args, "--quiet");
	எனில் (suc->update_data->prefix)
		strvec_pushl(&child->args, "--prefix", suc->update_data->prefix, வெற்று);
	எனில் (suc->update_data->recommend_shallow && sub->recommend_shallow == 1)
		strvec_தள்ளு(&child->args, "--depth=1");
	இல்லையெனில் எனில் (suc->update_data->depth)
		strvec_pushf(&child->args, "--depth=%d", suc->update_data->depth);
	எனில் (suc->update_data->filter_options && suc->update_data->filter_options->choice)
		strvec_pushf(&child->args, "--filter=%s",
			     expand_list_objects_filter_spec(suc->update_data->filter_options));
	எனில் (suc->update_data->require_init)
		strvec_தள்ளு(&child->args, "--require-init");
	எனில் (suc->update_data->ref_storage_format != REF_STORAGE_FORMAT_UNKNOWN)
		strvec_pushf(&child->args, "--ref-storage-format=%s",
			     ref_storage_format_to_name(suc->update_data->ref_storage_format));
	strvec_pushl(&child->args, "--path", sub->path, வெற்று);
	strvec_pushl(&child->args, "--name", sub->name, வெற்று);
	strvec_pushl(&child->args, "--url", url, வெற்று);
	எனில் (suc->update_data->references.nr) {
		கட்டமைப்பு string_list_item *item;

		for_each_string_list_item(item, &suc->update_data->references)
			strvec_pushl(&child->args, "--reference", item->string, வெற்று);
	}
	எனில் (suc->update_data->dissociate)
		strvec_தள்ளு(&child->args, "--dissociate");
	எனில் (suc->update_data->single_branch >= 0)
		strvec_தள்ளு(&child->args, suc->update_data->single_branch ?
					      "--single-branch" :
					      "--no-single-branch");

cleanup:
	விடுவி(displaypath);
	strbuf_release(&sb);
	எனில் (need_free_url)
		விடுவி((வெறுமை*)url);

	திரும்பு needs_cloning;
}

நிலையான முழு update_clone_get_next_task(கட்டமைப்பு child_process *child,
				      கட்டமைப்பு strbuf *err,
				      வெறுமை *suc_cb,
				      வெறுமை **idx_task_cb)
{
	கட்டமைப்பு submodule_update_clone *suc = suc_cb;
	மாறிலி கட்டமைப்பு cache_entry *ce;
	முழு index;

	க்கு (; suc->current < suc->update_data->list.nr; suc->current++) {
		ce = suc->update_data->list.entries[suc->current];
		எனில் (prepare_to_clone_next_submodule(ce, child, suc, err)) {
			முழு *p = xmalloc(அளவு(*p));

			*p = suc->current;
			*idx_task_cb = p;
			suc->current++;
			திரும்பு 1;
		}
	}

	/*
	 * The loop above tried cloning each submodule once, now try the
	 * stragglers again, which we can imagine as an extension of the
	 * entry list.
	 */
	index = suc->current - suc->update_data->list.nr;
	எனில் (index < suc->failed_clones_nr) {
		முழு *p;

		ce = suc->failed_clones[index];
		எனில் (!prepare_to_clone_next_submodule(ce, child, suc, err)) {
			suc->current ++;
			strbuf_addstr(err, "BUG: submodule considered for "
					   "cloning, doesn't need cloning "
					   "any more?\n");
			திரும்பு 0;
		}
		p = xmalloc(அளவு(*p));
		*p = suc->current;
		*idx_task_cb = p;
		suc->current ++;
		திரும்பு 1;
	}

	திரும்பு 0;
}

நிலையான முழு update_clone_start_failure(கட்டமைப்பு strbuf *err UNUSED,
				      வெறுமை *suc_cb,
				      வெறுமை *idx_task_cb UNUSED)
{
	கட்டமைப்பு submodule_update_clone *suc = suc_cb;

	suc->quickstop = 1;
	திரும்பு 1;
}

நிலையான முழு update_clone_task_finished(முழு result,
				      கட்டமைப்பு strbuf *err,
				      வெறுமை *suc_cb,
				      வெறுமை *idx_task_cb)
{
	மாறிலி கட்டமைப்பு cache_entry *ce;
	கட்டமைப்பு submodule_update_clone *suc = suc_cb;
	முழு *idxP = idx_task_cb;
	முழு idx = *idxP;

	விடுவி(idxP);

	எனில் (!result)
		திரும்பு 0;

	எனில் (idx < suc->update_data->list.nr) {
		ce  = suc->update_data->list.entries[idx];
		strbuf_addf(err, _("Failed to clone '%s'. Retry scheduled"),
			    ce->name);
		strbuf_addch(err, '\n');
		ALLOC_GROW(suc->failed_clones,
			   suc->failed_clones_nr + 1,
			   suc->failed_clones_alloc);
		suc->failed_clones[suc->failed_clones_nr++] = ce;
		திரும்பு 0;
	} இல்லையெனில் {
		idx -= suc->update_data->list.nr;
		ce  = suc->failed_clones[idx];
		strbuf_addf(err, _("Failed to clone '%s' a second time, aborting"),
			    ce->name);
		strbuf_addch(err, '\n');
		suc->quickstop = 1;
		திரும்பு 1;
	}

	திரும்பு 0;
}

நிலையான முழு git_update_clone_config(மாறிலி எழுத்து *var, மாறிலி எழுத்து *மதிப்பு,
				   மாறிலி கட்டமைப்பு config_context *ctx,
				   வெறுமை *cb)
{
	முழு *max_jobs = cb;

	எனில் (!strcmp(var, "submodule.fetchjobs"))
		*max_jobs = parse_submodule_fetchjobs(var, மதிப்பு, ctx->kvi);
	திரும்பு 0;
}

நிலையான முழு is_tip_reachable(மாறிலி எழுத்து *path, மாறிலி கட்டமைப்பு object_id *oid)
{
	கட்டமைப்பு child_process cp = CHILD_PROCESS_INIT;
	கட்டமைப்பு strbuf rev = STRBUF_INIT;
	எழுத்து *hex = oid_to_hex(oid);
	முழு reachable;

	cp.git_cmd = 1;
	cp.dir = path;
	cp.no_stderr = 1;
	strvec_pushl(&cp.args, "rev-list", "-n", "1", hex, "--not", "--all", வெற்று);

	prepare_submodule_repo_env(&cp.env);

	எனில் (capture_command(&cp, &rev, GIT_MAX_HEXSZ + 1) || rev.len)
		reachable = 0;
	இல்லையெனில்
		reachable = 1;

	strbuf_release(&rev);
	திரும்பு reachable;
}

நிலையான முழு fetch_in_submodule(மாறிலி எழுத்து *module_path, முழு depth, முழு quiet,
			      மாறிலி கட்டமைப்பு object_id *oid)
{
	கட்டமைப்பு child_process cp = CHILD_PROCESS_INIT;

	prepare_submodule_repo_env(&cp.env);
	cp.git_cmd = 1;
	cp.dir = module_path;

	strvec_தள்ளு(&cp.args, "fetch");
	எனில் (quiet)
		strvec_தள்ளு(&cp.args, "--quiet");
	எனில் (depth)
		strvec_pushf(&cp.args, "--depth=%d", depth);
	எனில் (oid) {
		எழுத்து *hex = oid_to_hex(oid);
		எழுத்து *remote;
		முழு code;

		code = get_default_remote_submodule(module_path, &remote);
		எனில் (code) {
			child_process_clear(&cp);
			திரும்பு code;
		}

		strvec_pushl(&cp.args, remote, hex, வெற்று);
		விடுவி(remote);
	}

	திரும்பு run_command(&cp);
}

நிலையான முழு run_update_command(மாறிலி கட்டமைப்பு update_data *ud, முழு subforce)
{
	கட்டமைப்பு child_process cp = CHILD_PROCESS_INIT;
	எழுத்து *oid = oid_to_hex(&ud->oid);
	முழு ret;

	தேர்வு (ud->update_strategy.type) {
	நிலை SM_UPDATE_CHECKOUT:
		cp.git_cmd = 1;
		strvec_pushl(&cp.args, "checkout", "-q", வெற்று);
		எனில் (subforce)
			strvec_தள்ளு(&cp.args, "-f");
		நிறுத்து;
	நிலை SM_UPDATE_REBASE:
		cp.git_cmd = 1;
		strvec_தள்ளு(&cp.args, "rebase");
		எனில் (ud->quiet)
			strvec_தள்ளு(&cp.args, "--quiet");
		நிறுத்து;
	நிலை SM_UPDATE_MERGE:
		cp.git_cmd = 1;
		strvec_தள்ளு(&cp.args, "merge");
		எனில் (ud->quiet)
			strvec_தள்ளு(&cp.args, "--quiet");
		நிறுத்து;
	நிலை SM_UPDATE_COMMAND:
		cp.use_shell = 1;
		strvec_தள்ளு(&cp.args, ud->update_strategy.command);
		நிறுத்து;
	இயல்பு:
		BUG("unexpected update strategy type: %d",
		    ud->update_strategy.type);
	}
	strvec_தள்ளு(&cp.args, oid);

	cp.dir = ud->sm_path;
	prepare_submodule_repo_env(&cp.env);
	எனில் ((ret = run_command(&cp))) {
		தேர்வு (ud->update_strategy.type) {
		நிலை SM_UPDATE_CHECKOUT:
			die_message(_("Unable to checkout '%s' in submodule path '%s'"),
				    oid, ud->displaypath);
			/* No "ret" assignment, use "git checkout"'s */
			நிறுத்து;
		நிலை SM_UPDATE_REBASE:
			ret = die_message(_("Unable to rebase '%s' in submodule path '%s'"),
					  oid, ud->displaypath);
			நிறுத்து;
		நிலை SM_UPDATE_MERGE:
			ret = die_message(_("Unable to merge '%s' in submodule path '%s'"),
					  oid, ud->displaypath);
			நிறுத்து;
		நிலை SM_UPDATE_COMMAND:
			ret = die_message(_("Execution of '%s %s' failed in submodule path '%s'"),
					  ud->update_strategy.command, oid, ud->displaypath);
			நிறுத்து;
		இயல்பு:
			BUG("unexpected update strategy type: %d",
			    ud->update_strategy.type);
		}

		திரும்பு ret;
	}

	எனில் (ud->quiet)
		திரும்பு 0;

	தேர்வு (ud->update_strategy.type) {
	நிலை SM_UPDATE_CHECKOUT:
		printf(_("Submodule path '%s': checked out '%s'\n"),
		       ud->displaypath, oid);
		நிறுத்து;
	நிலை SM_UPDATE_REBASE:
		printf(_("Submodule path '%s': rebased into '%s'\n"),
		       ud->displaypath, oid);
		நிறுத்து;
	நிலை SM_UPDATE_MERGE:
		printf(_("Submodule path '%s': merged in '%s'\n"),
		       ud->displaypath, oid);
		நிறுத்து;
	நிலை SM_UPDATE_COMMAND:
		printf(_("Submodule path '%s': '%s %s'\n"),
		       ud->displaypath, ud->update_strategy.command, oid);
		நிறுத்து;
	இயல்பு:
		BUG("unexpected update strategy type: %d",
		    ud->update_strategy.type);
	}

	திரும்பு 0;
}

நிலையான முழு run_update_procedure(மாறிலி கட்டமைப்பு update_data *ud)
{
	முழு subforce = is_null_oid(&ud->suboid) || ud->force;

	எனில் (!ud->nofetch) {
		/*
		 * Run fetch only if `oid` isn't present or it
		 * is not reachable from a ref.
		 */
		எனில் (!is_tip_reachable(ud->sm_path, &ud->oid) &&
		    fetch_in_submodule(ud->sm_path, ud->depth, ud->quiet, வெற்று) &&
		    !ud->quiet)
			fprintf_ln(stderr,
				   _("Unable to fetch in submodule path '%s'; "
				     "trying to directly fetch %s:"),
				   ud->displaypath, oid_to_hex(&ud->oid));
		/*
		 * Now we tried the usual fetch, but `oid` may
		 * not be reachable from any of the refs.
		 */
		எனில் (!is_tip_reachable(ud->sm_path, &ud->oid) &&
		    fetch_in_submodule(ud->sm_path, ud->depth, ud->quiet, &ud->oid))
			திரும்பு die_message(_("Fetched in submodule path '%s', but it did not "
					     "contain %s. Direct fetching of that commit failed."),
					   ud->displaypath, oid_to_hex(&ud->oid));
	}

	திரும்பு run_update_command(ud, subforce);
}

நிலையான முழு remote_submodule_branch(மாறிலி எழுத்து *path, மாறிலி எழுத்து **branch)
{
	மாறிலி கட்டமைப்பு submodule *sub;
	எழுத்து *சாவி;
	*branch = வெற்று;

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);
	எனில் (!sub)
		திரும்பு die_message(_("could not initialize submodule at path '%s'"),
				   path);

	சாவி = xstrfmt("submodule.%s.branch", sub->name);
	எனில் (repo_config_get_string_tmp(the_repository, சாவி, branch))
		*branch = sub->branch;
	விடுவி(சாவி);

	எனில் (!*branch) {
		*branch = "HEAD";
		திரும்பு 0;
	}

	எனில் (!strcmp(*branch, ".")) {
		மாறிலி எழுத்து *refname = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
							      "HEAD", 0, வெற்று,
							      வெற்று);

		எனில் (!refname)
			திரும்பு die_message(_("No such ref: %s"), "HEAD");

		/* detached HEAD */
		எனில் (!strcmp(refname, "HEAD"))
			திரும்பு die_message(_("Submodule (%s) branch configured to inherit "
					     "branch from superproject, but the superproject "
					     "is not on any branch"), sub->name);

		எனில் (!skip_prefix(refname, "refs/heads/", &refname))
			திரும்பு die_message(_("Expecting a full ref name, got %s"),
					   refname);

		*branch = refname;
		திரும்பு 0;
	}

	/* Our "branch" is coming from repo_config_get_string_tmp() */
	திரும்பு 0;
}

நிலையான முழு ensure_core_worktree(மாறிலி எழுத்து *path)
{
	மாறிலி எழுத்து *cw;
	கட்டமைப்பு repository subrepo;

	எனில் (repo_submodule_init(&subrepo, the_repository, path, null_oid(the_hash_algo)))
		திரும்பு die_message(_("could not get a repository handle for submodule '%s'"),
				   path);

	எனில் (!repo_config_get_string_tmp(&subrepo, "core.worktree", &cw)) {
		எழுத்து *cfg_file, *abs_path;
		மாறிலி எழுத்து *rel_path;
		கட்டமைப்பு strbuf sb = STRBUF_INIT;

		cfg_file = repo_git_path(&subrepo, "config");

		abs_path = absolute_pathdup(path);
		rel_path = relative_path(abs_path, subrepo.gitdir, &sb);

		repo_config_set_in_file(the_repository, cfg_file, "core.worktree", rel_path);

		விடுவி(cfg_file);
		விடுவி(abs_path);
		strbuf_release(&sb);
	}

	repo_clear(&subrepo);
	திரும்பு 0;
}

நிலையான மாறிலி எழுத்து *submodule_update_type_to_label(எண்ணிக்கை submodule_update_type type)
{
	தேர்வு (type) {
	நிலை SM_UPDATE_CHECKOUT:
		திரும்பு "checkout";
	நிலை SM_UPDATE_MERGE:
		திரும்பு "merge";
	நிலை SM_UPDATE_REBASE:
		திரும்பு "rebase";
	நிலை SM_UPDATE_UNSPECIFIED:
	நிலை SM_UPDATE_NONE:
	நிலை SM_UPDATE_COMMAND:
		நிறுத்து;
	}
	BUG("unreachable with type %d", type);
}

நிலையான வெறுமை update_data_to_args(மாறிலி கட்டமைப்பு update_data *update_data,
				கட்டமைப்பு strvec *args)
{
	எண்ணிக்கை submodule_update_type update_type = update_data->update_default;

	strvec_pushl(args, "submodule--helper", "update", "--recursive", வெற்று);
	எனில் (update_data->displaypath)
		strvec_pushf(args, "--super-prefix=%s/",
			     update_data->displaypath);
	strvec_pushf(args, "--jobs=%d", update_data->max_jobs);
	எனில் (update_data->quiet)
		strvec_தள்ளு(args, "--quiet");
	எனில் (update_data->force)
		strvec_தள்ளு(args, "--force");
	எனில் (update_data->init)
		strvec_தள்ளு(args, "--init");
	எனில் (update_data->remote)
		strvec_தள்ளு(args, "--remote");
	எனில் (update_data->nofetch)
		strvec_தள்ளு(args, "--no-fetch");
	எனில் (update_data->dissociate)
		strvec_தள்ளு(args, "--dissociate");
	எனில் (update_data->progress)
		strvec_தள்ளு(args, "--progress");
	எனில் (update_data->require_init)
		strvec_தள்ளு(args, "--require-init");
	எனில் (update_data->depth)
		strvec_pushf(args, "--depth=%d", update_data->depth);
	எனில் (update_type != SM_UPDATE_UNSPECIFIED)
		strvec_pushf(args, "--%s",
			     submodule_update_type_to_label(update_type));

	எனில் (update_data->references.nr) {
		கட்டமைப்பு string_list_item *item;

		for_each_string_list_item(item, &update_data->references)
			strvec_pushl(args, "--reference", item->string, வெற்று);
	}
	எனில் (update_data->ref_storage_format != REF_STORAGE_FORMAT_UNKNOWN)
		strvec_pushf(args, "--ref-storage-format=%s",
			     ref_storage_format_to_name(update_data->ref_storage_format));
	எனில் (update_data->filter_options && update_data->filter_options->choice)
		strvec_pushf(args, "--filter=%s",
				expand_list_objects_filter_spec(
					update_data->filter_options));
	எனில் (update_data->recommend_shallow == 0)
		strvec_தள்ளு(args, "--no-recommend-shallow");
	இல்லையெனில் எனில் (update_data->recommend_shallow == 1)
		strvec_தள்ளு(args, "--recommend-shallow");
	எனில் (update_data->single_branch >= 0)
		strvec_தள்ளு(args, update_data->single_branch ?
				    "--single-branch" :
				    "--no-single-branch");
}

நிலையான முழு update_submodule(கட்டமைப்பு update_data *update_data)
{
	முழு ret;

	எனில் (validate_submodule_path(update_data->sm_path) < 0)
		திரும்பு -1;

	ret = determine_submodule_update_strategy(the_repository,
						  update_data->just_cloned,
						  update_data->sm_path,
						  update_data->update_default,
						  &update_data->update_strategy);
	எனில் (ret)
		திரும்பு ret;

	எனில் (update_data->just_cloned)
		oidcpy(&update_data->suboid, null_oid(the_hash_algo));
	இல்லையெனில் எனில் (repo_resolve_gitlink_ref(the_repository, update_data->sm_path,
					  "HEAD", &update_data->suboid))
		திரும்பு die_message(_("Unable to find current revision in submodule path '%s'"),
				   update_data->displaypath);

	எனில் (update_data->remote) {
		எழுத்து *remote_name;
		மாறிலி எழுத்து *branch;
		எழுத்து *remote_ref;
		முழு code;

		code = get_default_remote_submodule(update_data->sm_path, &remote_name);
		எனில் (code)
			திரும்பு code;
		code = remote_submodule_branch(update_data->sm_path, &branch);
		எனில் (code) {
			விடுவி(remote_name);
			திரும்பு code;
		}
		remote_ref = xstrfmt("refs/remotes/%s/%s", remote_name, branch);

		விடுவி(remote_name);

		எனில் (!update_data->nofetch) {
			எனில் (fetch_in_submodule(update_data->sm_path, update_data->depth,
					      0, வெற்று)) {
				விடுவி(remote_ref);
				திரும்பு die_message(_("Unable to fetch in submodule path '%s'"),
						   update_data->sm_path);
			}
		}

		எனில் (repo_resolve_gitlink_ref(the_repository, update_data->sm_path,
					     remote_ref, &update_data->oid)) {
			ret = die_message(_("Unable to find %s revision in submodule path '%s'"),
					  remote_ref, update_data->sm_path);
			விடுவி(remote_ref);
			திரும்பு ret;
		}

		விடுவி(remote_ref);
	}

	எனில் (!oideq(&update_data->oid, &update_data->suboid) || update_data->force) {
		ret = run_update_procedure(update_data);
		எனில் (ret)
			திரும்பு ret;
	}

	எனில் (update_data->recursive) {
		கட்டமைப்பு child_process cp = CHILD_PROCESS_INIT;
		கட்டமைப்பு update_data next = *update_data;

		next.prefix = வெற்று;
		oidcpy(&next.oid, null_oid(the_hash_algo));
		oidcpy(&next.suboid, null_oid(the_hash_algo));

		cp.dir = update_data->sm_path;
		cp.git_cmd = 1;
		prepare_submodule_repo_env(&cp.env);
		update_data_to_args(&next, &cp.args);

		ret = run_command(&cp);
		எனில் (ret)
			die_message(_("Failed to recurse into submodule path '%s'"),
				    update_data->displaypath);
		திரும்பு ret;
	}

	திரும்பு 0;
}

நிலையான முழு update_submodules(கட்டமைப்பு update_data *update_data)
{
	முழு i, ret = 0;
	கட்டமைப்பு submodule_update_clone suc = SUBMODULE_UPDATE_CLONE_INIT;
	மாறிலி கட்டமைப்பு run_process_parallel_opts opts = {
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
	எனில் (suc.quickstop) {
		ret = 1;
		செல் cleanup;
	}

	க்கு (i = 0; i < suc.update_clone_nr; i++) {
		கட்டமைப்பு update_clone_data ucd = suc.update_clone[i];
		முழு code = 128;

		oidcpy(&update_data->oid, &ucd.oid);
		update_data->just_cloned = ucd.just_cloned;
		update_data->sm_path = ucd.sub->path;

		/*
		 * Verify that the submodule path does not contain any
		 * symlinks; if it does, it might have been tampered with.
		 * TODO: allow exempting it via
		 * `safe.submodule.path` or something
		 */
		எனில் (validate_submodule_path(update_data->sm_path) < 0)
			செல் fail;

		code = ensure_core_worktree(update_data->sm_path);
		எனில் (code)
			செல் fail;

		update_data->displaypath = get_submodule_displaypath(
			update_data->sm_path, update_data->prefix,
			update_data->super_prefix);
		code = update_submodule(update_data);
		FREE_AND_NULL(update_data->displaypath);
fail:
		எனில் (!code)
			தொடர்;
		ret = code;
		எனில் (ret == 128)
			செல் cleanup;
	}

cleanup:
	submodule_update_clone_release(&suc);
	string_list_clear(&update_data->references, 0);
	திரும்பு ret;
}

நிலையான முழு module_update(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
			 கட்டமைப்பு repository *repo UNUSED)
{
	கட்டமைப்பு pathspec pathspec = { 0 };
	கட்டமைப்பு pathspec pathspec2 = { 0 };
	கட்டமைப்பு update_data opt = UPDATE_DATA_INIT;
	கட்டமைப்பு list_objects_filter_options filter_options =
		LIST_OBJECTS_FILTER_INIT;
	மாறிலி எழுத்து *ref_storage_format = வெற்று;
	முழு ret;
	கட்டமைப்பு option module_update_options[] = {
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
	மாறிலி எழுத்து *மாறிலி git_submodule_helper_usage[] = {
		N_("git submodule [--quiet] update"
		" [--init [--filter=<filter-spec>]] [--remote]"
		" [-N|--no-fetch] [-f|--force]"
		" [--checkout|--merge|--rebase]"
		" [--[no-]recommend-shallow] [--reference <repository>]"
		" [--recursive] [--[no-]single-branch] [--] [<path>...]"),
		வெற்று
	};

	update_clone_config_from_gitmodules(the_repository, &opt.max_jobs);
	repo_config(the_repository, git_update_clone_config, &opt.max_jobs);

	argc = parse_options(argc, argv, prefix, module_update_options,
			     git_submodule_helper_usage, 0);

	எனில் (opt.require_init)
		opt.init = 1;

	எனில் (filter_options.choice && !opt.init) {
		usage_with_options(git_submodule_helper_usage,
				   module_update_options);
	}

	எனில் (ref_storage_format) {
		opt.ref_storage_format = ref_storage_format_by_name(ref_storage_format);
		எனில் (opt.ref_storage_format == REF_STORAGE_FORMAT_UNKNOWN)
			die(_("unknown ref storage format '%s'"), ref_storage_format);
	}

	opt.filter_options = &filter_options;
	opt.prefix = prefix;

	எனில் (opt.update_default)
		opt.update_strategy.type = opt.update_default;

	எனில் (module_list_compute(argv, prefix, &pathspec, &opt.list) < 0) {
		ret = 1;
		செல் cleanup;
	}

	எனில் (pathspec.nr)
		opt.warn_if_uninitialized = 1;

	எனில் (opt.init) {
		கட்டமைப்பு module_list list = MODULE_LIST_INIT;
		கட்டமைப்பு init_cb info = INIT_CB_INIT;

		எனில் (module_list_compute(argv, opt.prefix,
					&pathspec2, &list) < 0) {
			module_list_release(&list);
			ret = 1;
			செல் cleanup;
		}

		/*
		 * If there are no path args and submodule.active is set then,
		 * by default, only initialize 'active' modules.
		 */
		எனில் (!argc && !repo_config_get(the_repository, "submodule.active"))
			module_list_active(&list);

		info.prefix = opt.prefix;
		info.super_prefix = opt.super_prefix;
		எனில் (opt.quiet)
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
	திரும்பு ret;
}

நிலையான முழு தள்ளு_check(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix UNUSED,
		      கட்டமைப்பு repository *repo UNUSED)
{
	கட்டமைப்பு remote *remote;
	மாறிலி எழுத்து *superproject_head;
	எழுத்து *head;
	முழு detached_head = 0;
	கட்டமைப்பு object_id head_oid;

	எனில் (argc < 3)
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
				   0, &head_oid, வெற்று);
	எனில் (!head)
		die(_("Failed to resolve HEAD as a valid ref."));
	எனில் (!strcmp(head, "HEAD"))
		detached_head = 1;

	/*
	 * The remote must be configured.
	 * This is to avoid pushing to the exact same URL as the parent.
	 */
	remote = pushremote_get(argv[1]);
	எனில் (!remote || remote->origin == REMOTE_UNCONFIGURED)
		die("remote '%s' not configured", argv[1]);

	/* Check the refspec */
	எனில் (argc > 2) {
		முழு i;
		கட்டமைப்பு ref *local_refs = get_local_heads();
		கட்டமைப்பு refspec refspec = REFSPEC_INIT_தள்ளு(the_hash_algo);

		refspec_appendn(&refspec, argv + 2, argc - 2);

		க்கு (i = 0; i < refspec.nr; i++) {
			மாறிலி கட்டமைப்பு refspec_item *rs = &refspec.பொருட்கள்[i];

			எனில் (rs->pattern || rs->matching)
				தொடர்;

			/* LHS must match a single ref */
			தேர்வு (மொத்தம்_refspec_match(rs->src, local_refs, வெற்று)) {
			நிலை 1:
				நிறுத்து;
			நிலை 0:
				/*
				 * If LHS matches 'HEAD' then we need to ensure
				 * that it matches the same named branch
				 * checked out in the superproject.
				 */
				எனில் (!strcmp(rs->src, "HEAD")) {
					எனில் (!detached_head &&
					    !strcmp(head, superproject_head))
						நிறுத்து;
					die("HEAD does not match the named branch in the superproject");
				}
				/* fallthrough */
			இயல்பு:
				die("src refspec '%s' must name a ref",
				    rs->src);
			}
		}

		refspec_clear(&refspec);
		free_refs(local_refs);
	}
	விடுவி(head);

	திரும்பு 0;
}

நிலையான முழு absorb_git_dirs(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
			   கட்டமைப்பு repository *repo UNUSED)
{
	முழு i;
	கட்டமைப்பு pathspec pathspec = { 0 };
	கட்டமைப்பு module_list list = MODULE_LIST_INIT;
	மாறிலி எழுத்து *super_prefix = வெற்று;
	கட்டமைப்பு option embed_gitdir_options[] = {
		OPT__SUPER_PREFIX(&super_prefix),
		OPT_END()
	};
	மாறிலி எழுத்து *மாறிலி git_submodule_helper_usage[] = {
		N_("git submodule absorbgitdirs [<options>] [<path>...]"),
		வெற்று
	};
	முழு ret = 1;

	argc = parse_options(argc, argv, prefix, embed_gitdir_options,
			     git_submodule_helper_usage, 0);

	எனில் (module_list_compute(argv, prefix, &pathspec, &list) < 0)
		செல் cleanup;

	க்கு (i = 0; i < list.nr; i++)
		absorb_git_dir_into_superproject(list.entries[i]->name,
						 super_prefix);

	ret = 0;
cleanup:
	clear_pathspec(&pathspec);
	module_list_release(&list);
	திரும்பு ret;
}

நிலையான முழு module_set_url(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
			  கட்டமைப்பு repository *repo UNUSED)
{
	முழு quiet = 0, ret;
	மாறிலி எழுத்து *newurl;
	மாறிலி எழுத்து *path;
	எழுத்து *config_name;
	கட்டமைப்பு option options[] = {
		OPT__QUIET(&quiet, N_("suppress output for setting url of a submodule")),
		OPT_END()
	};
	மாறிலி எழுத்து *மாறிலி usage[] = {
		N_("git submodule set-url [--quiet] <path> <newurl>"),
		வெற்று
	};
	மாறிலி கட்டமைப்பு submodule *sub;

	argc = parse_options(argc, argv, prefix, options, usage, 0);

	எனில் (argc != 2 || !(path = argv[0]) || !(newurl = argv[1]))
		usage_with_options(usage, options);

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);

	எனில் (!sub)
		die(_("no submodule mapping found in .gitmodules for path '%s'"),
		    path);

	config_name = xstrfmt("submodule.%s.url", sub->name);
	ret = config_set_in_gitmodules_file_gently(the_repository, config_name, newurl);

	எனில் (!ret) {
		repo_read_gitmodules(the_repository, 0);
		sync_submodule(sub->path, prefix, வெற்று, quiet ? OPT_QUIET : 0);
	}

	விடுவி(config_name);
	திரும்பு !!ret;
}

நிலையான முழு module_set_branch(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
			     கட்டமைப்பு repository *repo UNUSED)
{
	முழு opt_default = 0, ret;
	மாறிலி எழுத்து *opt_branch = வெற்று;
	மாறிலி எழுத்து *path;
	எழுத்து *config_name;
	கட்டமைப்பு option options[] = {
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
	மாறிலி எழுத்து *மாறிலி usage[] = {
		N_("git submodule set-branch [-q|--quiet] (-d|--default) <path>"),
		N_("git submodule set-branch [-q|--quiet] (-b|--branch) <branch> <path>"),
		வெற்று
	};
	மாறிலி கட்டமைப்பு submodule *sub;

	argc = parse_options(argc, argv, prefix, options, usage, 0);

	எனில் (!opt_branch && !opt_default)
		die(_("--branch or --default required"));

	எனில் (opt_branch && opt_default)
		die(_("options '%s' and '%s' cannot be used together"), "--branch", "--default");

	எனில் (argc != 1 || !(path = argv[0]))
		usage_with_options(usage, options);

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);

	எனில் (!sub)
		die(_("no submodule mapping found in .gitmodules for path '%s'"),
		    path);

	config_name = xstrfmt("submodule.%s.branch", sub->name);
	ret = config_set_in_gitmodules_file_gently(the_repository, config_name, opt_branch);

	விடுவி(config_name);
	திரும்பு !!ret;
}

நிலையான முழு module_create_branch(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
				கட்டமைப்பு repository *repo UNUSED)
{
	எண்ணிக்கை branch_track track;
	முழு quiet = 0, force = 0, reflog = 0, dry_run = 0;
	கட்டமைப்பு option options[] = {
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
	மாறிலி எழுத்து *மாறிலி usage[] = {
		N_("git submodule--helper create-branch [-f|--force] [--create-reflog] [-q|--quiet] [-t|--track] [-n|--dry-run] <name> <start-oid> <start-name>"),
		வெற்று
	};
	கட்டமைப்பு repo_config_மதிப்புகள் *cfg = repo_config_மதிப்புகள்(the_repository);

	repo_config(the_repository, git_default_config, வெற்று);
	track = cfg->branch_track;
	argc = parse_options(argc, argv, prefix, options, usage, 0);

	எனில் (argc != 3)
		usage_with_options(usage, options);

	எனில் (!quiet && !dry_run)
		printf_ln(_("creating branch '%s'"), argv[0]);

	create_branches_recursively(the_repository, argv[0], argv[1], argv[2],
				    force, reflog, quiet, track, dry_run);
	திரும்பு 0;
}

கட்டமைப்பு add_data {
	மாறிலி எழுத்து *prefix;
	மாறிலி எழுத்து *branch;
	மாறிலி எழுத்து *reference_path;
	எழுத்து *sm_path;
	மாறிலி எழுத்து *sm_name;
	மாறிலி எழுத்து *repo;
	மாறிலி எழுத்து *realrepo;
	எண்ணிக்கை ref_storage_format ref_storage_format;
	முழு depth;
	unsigned முழு force: 1;
	unsigned முழு quiet: 1;
	unsigned முழு progress: 1;
	unsigned முழு dissociate: 1;
};
#define ADD_DATA_INIT { \
	.depth = -1, \
	.ref_storage_format = REF_STORAGE_FORMAT_UNKNOWN, \
}

நிலையான வெறுமை append_fetch_remotes(கட்டமைப்பு strbuf *செய்தி, மாறிலி எழுத்து *git_dir_path)
{
	கட்டமைப்பு child_process cp_remote = CHILD_PROCESS_INIT;
	கட்டமைப்பு strbuf sb_remote_out = STRBUF_INIT;

	cp_remote.git_cmd = 1;
	strvec_pushf(&cp_remote.env,
		     "GIT_DIR=%s", git_dir_path);
	strvec_தள்ளு(&cp_remote.env, "GIT_WORK_TREE=.");
	strvec_pushl(&cp_remote.args, "remote", "-v", வெற்று);
	எனில் (!capture_command(&cp_remote, &sb_remote_out, 0)) {
		எழுத்து *next_வரி;
		எழுத்து *வரி = sb_remote_out.buf;

		போது ((next_வரி = strchr(வரி, '\n')) != வெற்று) {
			size_t len = next_வரி - வரி;

			எனில் (strip_suffix_mem(வரி, &len, " (fetch)"))
				strbuf_addf(செய்தி, "  %.*s\n", (முழு)len, வரி);
			வரி = next_வரி + 1;
		}
	}

	strbuf_release(&sb_remote_out);
}

நிலையான முழு add_submodule(மாறிலி கட்டமைப்பு add_data *add_data)
{
	கட்டமைப்பு module_clone_data clone_data = MODULE_CLONE_DATA_INIT;
	கட்டமைப்பு string_list reference = STRING_LIST_INIT_NODUP;
	முழு ret = -1;

	/* perhaps the path already exists and is already a git repo, else clone it */
	எனில் (is_directory(add_data->sm_path)) {
		எழுத்து *submod_gitdir_path;
		கட்டமைப்பு strbuf sm_path = STRBUF_INIT;
		strbuf_addstr(&sm_path, add_data->sm_path);
		submod_gitdir_path = xstrfmt("%s/.git", add_data->sm_path);
		எனில் (is_nonbare_repository_dir(&sm_path))
			printf(_("Adding existing repo at '%s' to the index\n"),
			       add_data->sm_path);
		இல்லையெனில்
			die(_("'%s' already exists and is not a valid git repo"),
			    add_data->sm_path);
		strbuf_release(&sm_path);
		விடுவி(submod_gitdir_path);
	} இல்லையெனில் {
		கட்டமைப்பு child_process cp = CHILD_PROCESS_INIT;
		கட்டமைப்பு strbuf submod_gitdir = STRBUF_INIT;

		submodule_name_to_gitdir(&submod_gitdir, the_repository, add_data->sm_name);

		எனில் (is_directory(submod_gitdir.buf)) {
			எனில் (!add_data->force) {
				கட்டமைப்பு strbuf செய்தி = STRBUF_INIT;
				எழுத்து *die_செய்தி;

				strbuf_addf(&செய்தி, _("A git directory for '%s' is found "
						    "locally with remote(s):\n"),
					    add_data->sm_name);

				append_fetch_remotes(&செய்தி, submod_gitdir.buf);
				strbuf_release(&submod_gitdir);

				strbuf_addf(&செய்தி, _("If you want to reuse this local git "
						    "directory instead of cloning again from\n"
						    "  %s\n"
						    "use the '--force' option. If the local git "
						    "directory is not the correct repo\n"
						    "or you are unsure what this means choose "
						    "another name with the '--name' option."),
					    add_data->realrepo);

				die_செய்தி = strbuf_detach(&செய்தி, வெற்று);
				die("%s", die_செய்தி);
			} இல்லையெனில் {
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
		எனில் (add_data->reference_path) {
			எழுத்து *p = xstrdup(add_data->reference_path);

			string_list_append(&reference, p)->util = p;
		}
		clone_data.ref_storage_format = add_data->ref_storage_format;
		clone_data.dissociate = add_data->dissociate;
		எனில் (add_data->depth >= 0)
			clone_data.depth = add_data->depth;

		எனில் (clone_submodule(&clone_data, &reference))
			செல் cleanup;

		prepare_submodule_repo_env(&cp.env);
		cp.git_cmd = 1;
		cp.dir = add_data->sm_path;
		/*
		 * NOTE: we only get here if add_data->force is true, so
		 * passing --force to checkout is reasonable.
		 */
		strvec_pushl(&cp.args, "checkout", "-f", "-q", வெற்று);

		எனில் (add_data->branch) {
			strvec_pushl(&cp.args, "-B", add_data->branch, வெற்று);
			strvec_pushf(&cp.args, "origin/%s", add_data->branch);
		}

		எனில் (run_command(&cp))
			die(_("unable to checkout submodule '%s'"), add_data->sm_path);
	}
	ret = 0;

cleanup:
	string_list_clear(&reference, 1);
	திரும்பு ret;
}

நிலையான முழு config_submodule_in_gitmodules(மாறிலி எழுத்து *name, மாறிலி எழுத்து *var, மாறிலி எழுத்து *மதிப்பு)
{
	எழுத்து *சாவி;
	முழு ret;

	எனில் (!is_writing_gitmodules_ok())
		die(_("please make sure that the .gitmodules file is in the working tree"));

	சாவி = xstrfmt("submodule.%s.%s", name, var);
	ret = config_set_in_gitmodules_file_gently(the_repository, சாவி, மதிப்பு);
	விடுவி(சாவி);

	திரும்பு ret;
}

நிலையான வெறுமை configure_added_submodule(கட்டமைப்பு add_data *add_data)
{
	எழுத்து *சாவி;
	கட்டமைப்பு child_process add_submod = CHILD_PROCESS_INIT;
	கட்டமைப்பு child_process add_gitmodules = CHILD_PROCESS_INIT;
	மாறிலி கட்டமைப்பு string_list *மதிப்புகள்;
	முழு matched = 0;

	சாவி = xstrfmt("submodule.%s.url", add_data->sm_name);
	repo_config_set_gently(the_repository, சாவி, add_data->realrepo);
	விடுவி(சாவி);

	add_submod.git_cmd = 1;
	strvec_pushl(&add_submod.args, "add",
		     "--no-warn-embedded-repo", வெற்று);
	எனில் (add_data->force)
		strvec_தள்ளு(&add_submod.args, "--force");
	strvec_pushl(&add_submod.args, "--", add_data->sm_path, வெற்று);

	எனில் (run_command(&add_submod))
		die(_("Failed to add submodule '%s'"), add_data->sm_path);

	எனில் (config_submodule_in_gitmodules(add_data->sm_name, "path", add_data->sm_path) ||
	    config_submodule_in_gitmodules(add_data->sm_name, "url", add_data->repo))
		die(_("Failed to register submodule '%s'"), add_data->sm_path);

	எனில் (add_data->branch) {
		எனில் (config_submodule_in_gitmodules(add_data->sm_name,
						   "branch", add_data->branch))
			die(_("Failed to register submodule '%s'"), add_data->sm_path);
	}

	add_gitmodules.git_cmd = 1;
	strvec_pushl(&add_gitmodules.args,
		     "add", "--force", "--", ".gitmodules", வெற்று);

	எனில் (run_command(&add_gitmodules))
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
	எனில் (repo_config_get(the_repository, "submodule.active") || /* key absent */
	    repo_config_get_string_multi(the_repository, "submodule.active", &மதிப்புகள்)) {
		/*
		 * If the submodule being added isn't already covered by the
		 * current configured pathspec, set the submodule's active flag
		 */
		சாவி = xstrfmt("submodule.%s.active", add_data->sm_name);
		repo_config_set_gently(the_repository, சாவி, "true");
		விடுவி(சாவி);
	} இல்லையெனில் {
		க்கு (size_t i = 0; i < மதிப்புகள்->nr; i++) {
			மாறிலி எழுத்து *pat = மதிப்புகள்->பொருட்கள்[i].string;
			எனில் (!wildmatch(pat, add_data->sm_path, 0)) { /* match found */
				matched = 1;
				நிறுத்து;
			}
		}
		எனில் (!matched) { /* no pattern matched -> force-enable */
			சாவி = xstrfmt("submodule.%s.active", add_data->sm_name);
			repo_config_set_gently(the_repository, சாவி, "true");
			விடுவி(சாவி);
		}
	}
}

நிலையான வெறுமை die_on_index_match(மாறிலி எழுத்து *path, முழு force)
{
	கட்டமைப்பு pathspec ps;
	மாறிலி எழுத்து *args[] = { path, வெற்று };
	parse_pathspec(&ps, 0, PATHSPEC_PREFER_CWD, வெற்று, args);

	எனில் (repo_read_index_preload(the_repository, வெற்று, 0) < 0)
		die(_("index file corrupt"));

	எனில் (ps.nr) {
		எழுத்து *ps_matched = xcalloc(ps.nr, 1);

		/* TODO: audit for interaction with sparse-index. */
		ensure_full_index(the_repository->index);

		/*
		 * Since there is only one pathspec, we just need to
		 * check ps_matched[0] to know if a cache entry matched.
		 */
		க்கு (size_t i = 0; i < the_repository->index->cache_nr; i++) {
			ce_path_match(the_repository->index, the_repository->index->cache[i], &ps,
				      ps_matched);

			எனில் (ps_matched[0]) {
				எனில் (!force)
					die(_("'%s' already exists in the index"),
					    path);
				எனில் (!S_ISGITLINK(the_repository->index->cache[i]->ce_mode))
					die(_("'%s' already exists in the index "
					      "and is not a submodule"), path);
				நிறுத்து;
			}
		}
		விடுவி(ps_matched);
	}
	clear_pathspec(&ps);
}

நிலையான வெறுமை die_on_repo_without_commits(மாறிலி எழுத்து *path)
{
	கட்டமைப்பு strbuf sb = STRBUF_INIT;
	strbuf_addstr(&sb, path);
	எனில் (is_nonbare_repository_dir(&sb)) {
		கட்டமைப்பு object_id oid;
		எனில் (repo_resolve_gitlink_ref(the_repository, path, "HEAD", &oid) < 0)
			die(_("'%s' does not have a commit checked out"), path);
	}
	strbuf_release(&sb);
}

நிலையான முழு module_add(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
		      கட்டமைப்பு repository *repo UNUSED)
{
	முழு force = 0, quiet = 0, progress = 0, dissociate = 0;
	கட்டமைப்பு add_data add_data = ADD_DATA_INIT;
	மாறிலி எழுத்து *ref_storage_format = வெற்று;
	எழுத்து *to_free = வெற்று;
	மாறிலி கட்டமைப்பு submodule *existing;
	கட்டமைப்பு strbuf buf = STRBUF_INIT;
	எழுத்து *sm_name_to_free = வெற்று;
	கட்டமைப்பு option options[] = {
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
	மாறிலி எழுத்து *மாறிலி usage[] = {
		N_("git submodule add [<options>] [--] <repository> [<path>]"),
		வெற்று
	};
	கட்டமைப்பு strbuf sb = STRBUF_INIT;
	முழு ret = 1;

	argc = parse_options(argc, argv, prefix, options, usage, 0);

	எனில் (!is_writing_gitmodules_ok())
		die(_("please make sure that the .gitmodules file is in the working tree"));

	எனில் (prefix && *prefix &&
	    add_data.reference_path && !is_absolute_path(add_data.reference_path))
		add_data.reference_path = xstrfmt("%s%s", prefix, add_data.reference_path);

	எனில் (argc == 0 || argc > 2)
		usage_with_options(usage, options);

	எனில் (ref_storage_format) {
		add_data.ref_storage_format = ref_storage_format_by_name(ref_storage_format);
		எனில் (add_data.ref_storage_format == REF_STORAGE_FORMAT_UNKNOWN)
			die(_("unknown ref storage format '%s'"), ref_storage_format);
	}

	add_data.repo = argv[0];
	எனில் (argc == 1)
		add_data.sm_path = git_url_basename(add_data.repo, 0, 0);
	இல்லையெனில்
		add_data.sm_path = xstrdup(argv[1]);

	எனில் (prefix && *prefix && !is_absolute_path(add_data.sm_path)) {
		எழுத்து *sm_path = add_data.sm_path;

		add_data.sm_path = xstrfmt("%s%s", prefix, sm_path);
		விடுவி(sm_path);
	}

	எனில் (starts_with_dot_dot_slash(add_data.repo) ||
	    starts_with_dot_slash(add_data.repo)) {
		எனில் (prefix)
			die(_("Relative path can only be used from the toplevel "
			      "of the working tree"));

		/* dereference source url relative to parent's url */
		to_free = resolve_relative_url(add_data.repo, வெற்று, 1);
		add_data.realrepo = to_free;
	} இல்லையெனில் எனில் (is_dir_sep(add_data.repo[0]) || strchr(add_data.repo, ':')) {
		add_data.realrepo = add_data.repo;
	} இல்லையெனில் {
		die(_("repo URL: '%s' must be absolute or begin with ./|../"),
		    add_data.repo);
	}

	/*
	 * normalize path:
	 * multiple //; leading ./; /./; /../;
	 */
	normalize_path_copy(add_data.sm_path, add_data.sm_path);
	strip_dir_trailing_slashes(add_data.sm_path);

	எனில் (validate_submodule_path(add_data.sm_path) < 0)
		die(வெற்று);

	die_on_index_match(add_data.sm_path, force);
	die_on_repo_without_commits(add_data.sm_path);

	எனில் (!force) {
		கட்டமைப்பு child_process cp = CHILD_PROCESS_INIT;

		cp.git_cmd = 1;
		cp.no_stdout = 1;
		strvec_pushl(&cp.args, "add", "--dry-run", "--ignore-missing",
			     "--no-warn-embedded-repo", add_data.sm_path, வெற்று);
		எனில் ((ret = pipe_command(&cp, வெற்று, 0, வெற்று, 0, &sb, 0))) {
			strbuf_complete_வரி(&sb);
			fputs(sb.buf, stderr);
			செல் cleanup;
		}
	}

	எனில் (!add_data.sm_name)
		add_data.sm_name = add_data.sm_path;

	existing = submodule_from_name(the_repository,
					null_oid(the_hash_algo),
					add_data.sm_name);

	எனில் (existing && existing->path &&
	    strcmp(existing->path, add_data.sm_path)) {
		எனில் (!force) {
			die(_("submodule name '%s' already used for path '%s'"),
			    add_data.sm_name, existing->path);
		}
		/* --force: build <name><n> until unique */
		க்கு (முழு i = 1; ; i++) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "%s%d", add_data.sm_name, i);
			எனில் (!submodule_from_name(the_repository,
						null_oid(the_hash_algo),
						buf.buf)) {
				நிறுத்து;
			}
		}
		add_data.sm_name = sm_name_to_free = strbuf_detach(&buf, வெற்று);
	}

	எனில் (check_submodule_name(add_data.sm_name))
		die(_("'%s' is not a valid submodule name"), add_data.sm_name);

	add_data.prefix = prefix;
	add_data.force = !!force;
	add_data.quiet = !!quiet;
	add_data.progress = !!progress;
	add_data.dissociate = !!dissociate;

	எனில் (the_repository->repository_format_submodule_path_cfg)
		create_default_gitdir_config(add_data.sm_name);

	எனில் (add_submodule(&add_data))
		செல் cleanup;
	configure_added_submodule(&add_data);

	ret = 0;
cleanup:
	விடுவி(sm_name_to_free);
	விடுவி(add_data.sm_path);
	விடுவி(to_free);
	strbuf_release(&sb);

	திரும்பு ret;
}

முழு cmd_submodule__helper(முழு argc,
			  மாறிலி எழுத்து **argv,
			  மாறிலி எழுத்து *prefix,
			  கட்டமைப்பு repository *repo)
{
	parse_opt_subcommand_fn *fn = வெற்று;
	மாறிலி எழுத்து *மாறிலி usage[] = {
		N_("git submodule--helper <command>"),
		வெற்று
	};
	கட்டமைப்பு option options[] = {
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
		OPT_SUBCOMMAND("push-check", &fn, தள்ளு_check),
		OPT_SUBCOMMAND("absorbgitdirs", &fn, absorb_git_dirs),
		OPT_SUBCOMMAND("set-url", &fn, module_set_url),
		OPT_SUBCOMMAND("set-branch", &fn, module_set_branch),
		OPT_SUBCOMMAND("create-branch", &fn, module_create_branch),
		OPT_SUBCOMMAND("get-default-remote", &fn, module_get_default_remote),
		OPT_END()
	};
	argc = parse_options(argc, argv, prefix, options, usage, 0);

	திரும்பு fn(argc, argv, prefix, repo);
}
