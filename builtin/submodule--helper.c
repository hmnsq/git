#define USE_THE_REPOSITORY_VARIABLE

#شامل "builtin.h"
#شامل "abspath.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "hex.h"

#شامل "config.h"
#شامل "parse-options.h"
#شامل "quote.h"
#شامل "path.h"
#شامل "pathspec.h"
#شامل "preload-index.h"
#شامل "dir.h"
#شامل "read-cache.h"
#شامل "setup.h"
#شامل "sparse-index.h"
#شامل "submodule.h"
#شامل "submodule-config.h"
#شامل "string-list.h"
#شامل "run-command.h"
#شامل "remote.h"
#شامل "refs.h"
#شامل "refspec.h"
#شامل "revision.h"
#شامل "diffcore.h"
#شامل "diff.h"
#شامل "object-file.h"
#شامل "object-name.h"
#شامل "odb.h"
#شامل "odb/source.h"
#شامل "odb/source-files.h"
#شامل "advice.h"
#شامل "branch.h"
#شامل "list-objects-filter-options.h"
#شامل "wildmatch.h"
#شامل "strbuf.h"
#شامل "url.h"

#define OPT_QUIET (1 << 0)
#define OPT_CACHED (1 << 1)
#define OPT_RECURSIVE (1 << 2)
#define OPT_FORCE (1 << 3)

تعریفنوع تهی (*each_submodule_fn)(ثابت ساختار cache_entry *list_item,
				  تهی *cb_data);

ایستا نویسه *get_default_remote(تهی)
{
	بازگشت xstrdup(repo_default_remote(the_repository));
}

ایستا نویسه *resolve_relative_url(ثابت نویسه *rel_url, ثابت نویسه *up_path, صحیح quiet)
{
	نویسه *remoteurl, *resolved_url;
	نویسه *remote = get_default_remote();
	ساختار strbuf remotesb = STRBUF_INIT;

	strbuf_addf(&remotesb, "remote.%s.url", remote);
	اگر (repo_config_get_string(the_repository, remotesb.buf, &remoteurl)) {
		اگر (!quiet)
			warning(_("could not look up configuration '%s'. "
				  "Assuming this repository is its own "
				  "authoritative upstream."),
				remotesb.buf);
		remoteurl = xgetcwd();
	}
	resolved_url = relative_url(remoteurl, rel_url, up_path);

	آزاد(remote);
	آزاد(remoteurl);
	strbuf_release(&remotesb);

	بازگشت resolved_url;
}

ایستا صحیح get_default_remote_submodule(ثابت نویسه *module_path, نویسه **default_remote)
{
	ثابت ساختار submodule *sub;
	ساختار repository subrepo;
	ثابت نویسه *remote_name = پوچ;
	نویسه *url = پوچ;
	صحیح ret = 0;

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), module_path);
	اگر (sub && sub->url) {
		url = xstrdup(sub->url);

		/* Possibly a url relative to parent */
		اگر (starts_with_dot_dot_slash(url) ||
		    starts_with_dot_slash(url)) {
			نویسه *oldurl = url;

			url = resolve_relative_url(oldurl, پوچ, 1);
			آزاد(oldurl);
		}
	}

	اگر (repo_submodule_init(&subrepo, the_repository, module_path,
				null_oid(the_hash_algo)) < 0) {
		ret = die_message(_("could not get a repository handle for submodule '%s'"),
				   module_path);
		برو out;
	}

	/* Look up by URL first */
	اگر (url)
		remote_name = repo_remote_from_url(&subrepo, url);
	اگر (!remote_name)
		remote_name = repo_default_remote(&subrepo);

	*default_remote = xstrdup(remote_name);

out:
	repo_clear(&subrepo);
	آزاد(url);

	بازگشت ret;
}

ایستا صحیح module_get_default_remote(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix,
				     ساختار repository *repo UNUSED)
{
	ثابت نویسه *path;
	نویسه *resolved_path = پوچ;
	نویسه *default_remote = پوچ;
	صحیح code;
	ساختار option options[] = {
		OPT_END()
	};
	ثابت نویسه *ثابت usage[] = {
		N_("git submodule--helper get-default-remote <path>"),
		پوچ
	};

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	اگر (argc != 1)
		usage_with_options(usage, options);

	path = argv[0];
	اگر (prefix && *prefix && !is_absolute_path(path)) {
		resolved_path = xstrfmt("%s%s", prefix, path);
		path = resolved_path;
	}

	code = get_default_remote_submodule(path, &default_remote);
	اگر (code) {
		آزاد(resolved_path);
		بازگشت code;
	}

	printf("%s\n", default_remote);
	آزاد(default_remote);
	آزاد(resolved_path);
	بازگشت 0;
}

/* the result should be freed by the caller. */
ایستا نویسه *get_submodule_displaypath(ثابت نویسه *path, ثابت نویسه *prefix,
				       ثابت نویسه *super_prefix)
{
	اگر (prefix && super_prefix) {
		BUG("cannot have prefix '%s' and superprefix '%s'",
		    prefix, super_prefix);
	} وگرنه اگر (prefix) {
		ساختار strbuf sb = STRBUF_INIT;
		نویسه *displaypath = xstrdup(relative_path(path, prefix, &sb));
		strbuf_release(&sb);
		بازگشت displaypath;
	} وگرنه اگر (super_prefix) {
		بازگشت xstrfmt("%s%s", super_prefix, path);
	} وگرنه {
		بازگشت xstrdup(path);
	}
}

ایستا نویسه *compute_rev_name(ثابت نویسه *sub_path, ثابت نویسه* object_id)
{
	ساختار strbuf sb = STRBUF_INIT;
	ثابت نویسه ***d;

	ایستا ثابت نویسه *describe_bare[] = { پوچ };

	ایستا ثابت نویسه *describe_tags[] = { "--tags", پوچ };

	ایستا ثابت نویسه *describe_contains[] = { "--contains", پوچ };

	ایستا ثابت نویسه *describe_all_always[] = { "--all", "--always", پوچ };

	ایستا ثابت نویسه **describe_argv[] = { describe_bare, describe_tags,
						describe_contains,
						describe_all_always, پوچ };

	برای (d = describe_argv; *d; d++) {
		ساختار child_process cp = CHILD_PROCESS_INIT;
		prepare_submodule_repo_env(&cp.env);
		cp.dir = sub_path;
		cp.git_cmd = 1;
		cp.no_stderr = 1;

		strvec_افزودن(&cp.args, "describe");
		strvec_pushv(&cp.args, *d);
		strvec_افزودن(&cp.args, object_id);

		اگر (!capture_command(&cp, &sb, 0)) {
			strbuf_strip_suffix(&sb, "\n");
			بازگشت strbuf_detach(&sb, پوچ);
		}
	}

	strbuf_release(&sb);
	بازگشت پوچ;
}

ساختار module_list {
	ثابت ساختار cache_entry **entries;
	صحیح alloc, nr;
};
#define MODULE_LIST_INIT { 0 }

ایستا تهی module_list_release(ساختار module_list *ml)
{
	آزاد(ml->entries);
}

ایستا صحیح module_list_compute(ثابت نویسه **argv,
			       ثابت نویسه *prefix,
			       ساختار pathspec *pathspec,
			       ساختار module_list *list)
{
	صحیح result = 0;
	نویسه *ps_matched = پوچ;

	parse_pathspec(pathspec, 0,
		       PATHSPEC_PREFER_FULL,
		       prefix, argv);

	اگر (pathspec->nr)
		ps_matched = xcalloc(pathspec->nr, 1);

	اگر (repo_read_index(the_repository) < 0)
		die(_("index file corrupt"));

	برای (size_t i = 0; i < the_repository->index->cache_nr; i++) {
		ثابت ساختار cache_entry *ce = the_repository->index->cache[i];

		اگر (!match_pathspec(the_repository->index, pathspec, ce->name, ce_namelen(ce),
				    0, ps_matched, 1) ||
		    !S_ISGITLINK(ce->ce_mode))
			ادامه;

		ALLOC_GROW(list->entries, list->nr + 1, list->alloc);
		list->entries[list->nr++] = ce;
		وقتی (i + 1 < the_repository->index->cache_nr &&
		       !strcmp(ce->name, the_repository->index->cache[i + 1]->name))
			/*
			 * Skip entries with the same name in different stages
			 * to make sure an entry is returned only once.
			 */
			i++;
	}

	اگر (ps_matched && report_path_error(ps_matched, pathspec))
		result = -1;

	آزاد(ps_matched);

	بازگشت result;
}

ایستا تهی module_list_active(ساختار module_list *list)
{
	صحیح i;
	ساختار module_list active_modules = MODULE_LIST_INIT;

	برای (i = 0; i < list->nr; i++) {
		ثابت ساختار cache_entry *ce = list->entries[i];

		اگر (!is_submodule_active(the_repository, ce->name))
			ادامه;

		ALLOC_GROW(active_modules.entries,
			   active_modules.nr + 1,
			   active_modules.alloc);
		active_modules.entries[active_modules.nr++] = ce;
	}

	module_list_release(list);
	*list = active_modules;
}

ایستا نویسه *get_up_path(ثابت نویسه *path)
{
	ساختار strbuf sb = STRBUF_INIT;

	strbuf_addstrings(&sb, "../", شمار_slashes(path));

	/*
	 * Check if 'path' ends with slash or not
	 * for having the same output for dir/sub_dir
	 * and dir/sub_dir/
	 */
	اگر (!is_dir_sep(path[strlen(path) - 1]))
		strbuf_addstr(&sb, "../");

	بازگشت strbuf_detach(&sb, پوچ);
}

ایستا تهی for_each_listed_submodule(ثابت ساختار module_list *list,
				      each_submodule_fn fn, تهی *cb_data)
{
	صحیح i;

	برای (i = 0; i < list->nr; i++)
		fn(list->entries[i], cb_data);
}

ساختار foreach_cb {
	صحیح argc;
	ثابت نویسه **argv;
	ثابت نویسه *prefix;
	ثابت نویسه *super_prefix;
	صحیح quiet;
	صحیح recursive;
};
#define FOREACH_CB_INIT { 0 }

ایستا تهی runcommand_in_submodule_cb(ثابت ساختار cache_entry *list_item,
				       تهی *cb_data)
{
	ساختار foreach_cb *info = cb_data;
	ثابت نویسه *path = list_item->name;
	ثابت ساختار object_id *ce_oid = &list_item->oid;
	ثابت ساختار submodule *sub;
	ساختار child_process cp = CHILD_PROCESS_INIT;
	نویسه *displaypath;

	اگر (validate_submodule_path(path) < 0)
		die(پوچ);

	displaypath = get_submodule_displaypath(path, info->prefix,
						info->super_prefix);

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);

	اگر (!sub)
		die(_("No url found for submodule path '%s' in .gitmodules"),
			displaypath);

	اگر (!is_submodule_populated_gently(path, پوچ))
		برو cleanup;

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
	اگر (info->argc == 1) {
		نویسه *toplevel = xgetcwd();
		ساختار strbuf sb = STRBUF_INIT;

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
		آزاد(toplevel);
	} وگرنه {
		strvec_pushv(&cp.args, info->argv);
	}

	اگر (!info->quiet)
		printf(_("Entering '%s'\n"), displaypath);

	اگر (info->argv[0]) {
		اگر (run_command(&cp))
			die(_("run_command returned non-zero status for %s\n."),
			    displaypath);
	} وگرنه {
		child_process_clear(&cp);
	}

	اگر (info->recursive) {
		ساختار child_process cpr = CHILD_PROCESS_INIT;

		cpr.git_cmd = 1;
		cpr.dir = path;
		prepare_submodule_repo_env(&cpr.env);

		strvec_pushl(&cpr.args, "submodule--helper", "foreach", "--recursive",
			     پوچ);
		strvec_pushf(&cpr.args, "--super-prefix=%s/", displaypath);

		اگر (info->quiet)
			strvec_افزودن(&cpr.args, "--quiet");

		strvec_افزودن(&cpr.args, "--");
		strvec_pushv(&cpr.args, info->argv);

		اگر (run_command(&cpr))
			die(_("run_command returned non-zero status while "
				"recursing in the nested submodules of %s\n."),
				displaypath);
	}

cleanup:
	آزاد(displaypath);
}

ایستا صحیح module_foreach(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix,
			  ساختار repository *repo UNUSED)
{
	ساختار foreach_cb info = FOREACH_CB_INIT;
	ساختار pathspec pathspec = { 0 };
	ساختار module_list list = MODULE_LIST_INIT;
	ساختار option module_foreach_options[] = {
		OPT__SUPER_PREFIX(&info.super_prefix),
		OPT__QUIET(&info.quiet, N_("suppress output of entering each submodule command")),
		OPT_BOOL(0, "recursive", &info.recursive,
			 N_("recurse into nested submodules")),
		OPT_END()
	};
	ثابت نویسه *ثابت git_submodule_helper_usage[] = {
		N_("git submodule foreach [--quiet] [--recursive] [--] <command>"),
		پوچ
	};
	صحیح ret = 1;

	argc = parse_options(argc, argv, prefix, module_foreach_options,
			     git_submodule_helper_usage, 0);

	اگر (module_list_compute(پوچ, prefix, &pathspec, &list) < 0)
		برو cleanup;

	info.argc = argc;
	info.argv = argv;
	info.prefix = prefix;

	for_each_listed_submodule(&list, runcommand_in_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	بازگشت ret;
}

ساختار init_cb {
	ثابت نویسه *prefix;
	ثابت نویسه *super_prefix;
	unsigned صحیح flags;
};
#define INIT_CB_INIT { 0 }

ایستا صحیح validate_and_set_submodule_gitdir(ساختار strbuf *gitdir_path,
					     ثابت نویسه *submodule_name)
{
	ثابت نویسه *مقدار;
	نویسه *کلید;

	اگر (validate_submodule_git_dir(gitdir_path->buf, submodule_name))
		بازگشت -1;

	کلید = xstrfmt("submodule.%s.gitdir", submodule_name);

	/* Nothing to do if the config already exists. */
	اگر (!repo_config_get_string_tmp(the_repository, کلید, &مقدار)) {
		آزاد(کلید);
		بازگشت 0;
	}

	اگر (repo_config_set_gently(the_repository, کلید, gitdir_path->buf)) {
		آزاد(کلید);
		بازگشت -1;
	}

	آزاد(کلید);
	بازگشت 0;
}

ایستا تهی create_default_gitdir_config(ثابت نویسه *submodule_name)
{
	ساختار strbuf gitdir_path = STRBUF_INIT;
	ساختار git_hash_ctx ctx;
	نویسه hex_name_hash[GIT_MAX_HEXSZ + 1], header[128];
	unsigned نویسه raw_name_hash[GIT_MAX_RAWSZ];
	صحیح header_len;

	/* Case 1: try the plain module name */
	repo_git_path_append(the_repository, &gitdir_path, "modules/%s", submodule_name);
	اگر (!validate_and_set_submodule_gitdir(&gitdir_path, submodule_name)) {
		strbuf_release(&gitdir_path);
		بازگشت;
	}

	/* Case 2.1: Try URI-safe (RFC3986) encoding first, this fixes nested gitdirs */
	strbuf_reset(&gitdir_path);
	repo_git_path_append(the_repository, &gitdir_path, "modules/");
	strbuf_addstr_urlencode(&gitdir_path, submodule_name, is_rfc3986_unreserved);
	اگر (!validate_and_set_submodule_gitdir(&gitdir_path, submodule_name)) {
		strbuf_release(&gitdir_path);
		بازگشت;
	}

	/* Case 2.2: Try extended uppercase URI (RFC3986) encoding, to fix case-folding */
	strbuf_reset(&gitdir_path);
	repo_git_path_append(the_repository, &gitdir_path, "modules/");
	strbuf_addstr_urlencode(&gitdir_path, submodule_name, is_casefolding_rfc3986_unreserved);
	اگر (!validate_and_set_submodule_gitdir(&gitdir_path, submodule_name))
		بازگشت;

	/* Case 2.3: Try some derived gitdir names, see if one sticks */
	برای (نویسه c = '0'; c <= '9'; c++) {
		strbuf_reset(&gitdir_path);
		repo_git_path_append(the_repository, &gitdir_path, "modules/");
		strbuf_addstr_urlencode(&gitdir_path, submodule_name, is_rfc3986_unreserved);
		strbuf_addch(&gitdir_path, c);
		اگر (!validate_and_set_submodule_gitdir(&gitdir_path, submodule_name))
			بازگشت;

		strbuf_reset(&gitdir_path);
		repo_git_path_append(the_repository, &gitdir_path, "modules/");
		strbuf_addstr_urlencode(&gitdir_path, submodule_name, is_casefolding_rfc3986_unreserved);
		strbuf_addch(&gitdir_path, c);
		اگر (!validate_and_set_submodule_gitdir(&gitdir_path, submodule_name))
			بازگشت;
	}

	/* Case 2.4: If all the above failed, try a hash of the name as a last resort */
	header_len = snprintf(header, اندازه(header),
			      "blob %"PRIuMAX, (uintmax_t)strlen(submodule_name));
	git_hash_init(&ctx, the_hash_algo);
	git_hash_update(&ctx, header, header_len);
	git_hash_update(&ctx, "\0", 1);
	git_hash_update(&ctx, submodule_name, strlen(submodule_name));
	git_hash_final(raw_name_hash, &ctx);
	hash_to_hex_algop_r(hex_name_hash, raw_name_hash, the_hash_algo);
	strbuf_reset(&gitdir_path);
	repo_git_path_append(the_repository, &gitdir_path, "modules/%s", hex_name_hash);
	اگر (!validate_and_set_submodule_gitdir(&gitdir_path, submodule_name)) {
		strbuf_release(&gitdir_path);
		بازگشت;
	}

	/* Case 3: nothing worked, error out */
	die(_("failed to set a valid default config for 'submodule.%s.gitdir'. "
	      "Please ensure it is set, for example by running something like: "
	      "'git config submodule.%s.gitdir .git/modules/%s'"),
	    submodule_name, submodule_name, submodule_name);
}

ایستا تهی init_submodule(ثابت نویسه *path, ثابت نویسه *prefix,
			   ثابت نویسه *super_prefix,
			   unsigned صحیح flags)
{
	ثابت ساختار submodule *sub;
	ساختار strbuf sb = STRBUF_INIT;
	ثابت نویسه *upd;
	نویسه *url = پوچ, *displaypath;

	displaypath = get_submodule_displaypath(path, prefix, super_prefix);

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);

	اگر (!sub)
		die(_("No url found for submodule path '%s' in .gitmodules"),
			displaypath);

	/*
	 * NEEDSWORK: In a multi-working-tree world, this needs to be
	 * set in the per-worktree config.
	 *
	 * Set active flag for the submodule being initialized
	 */
	اگر (!is_submodule_active(the_repository, path)) {
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
	اگر (repo_config_get_string(the_repository, sb.buf, &url)) {
		اگر (!sub->url)
			die(_("No url found for submodule path '%s' in .gitmodules"),
				displaypath);

		url = xstrdup(sub->url);

		/* Possibly a url relative to parent */
		اگر (starts_with_dot_dot_slash(url) ||
		    starts_with_dot_slash(url)) {
			نویسه *oldurl = url;

			url = resolve_relative_url(oldurl, پوچ, 0);
			آزاد(oldurl);
		}

		اگر (repo_config_set_gently(the_repository, sb.buf, url))
			die(_("Failed to register url for submodule path '%s'"),
			    displaypath);
		اگر (!(flags & OPT_QUIET))
			fprintf(stderr,
				_("Submodule '%s' (%s) registered for path '%s'\n"),
				sub->name, url, displaypath);
	}
	strbuf_reset(&sb);

	/* Copy "update" setting when it is not set yet */
	strbuf_addf(&sb, "submodule.%s.update", sub->name);
	اگر (repo_config_get_string_tmp(the_repository, sb.buf, &upd) &&
	    sub->update_strategy.type != SM_UPDATE_UNSPECIFIED) {
		اگر (sub->update_strategy.type == SM_UPDATE_COMMAND) {
			fprintf(stderr, _("warning: command update mode suggested for submodule '%s'\n"),
				sub->name);
			upd = "none";
		} وگرنه {
			upd = submodule_update_type_to_string(sub->update_strategy.type);
		}

		اگر (repo_config_set_gently(the_repository, sb.buf, upd))
			die(_("Failed to register update mode for submodule path '%s'"), displaypath);
	}

	اگر (the_repository->repository_format_submodule_path_cfg)
		create_default_gitdir_config(sub->name);

	strbuf_release(&sb);
	آزاد(displaypath);
	آزاد(url);
}

ایستا تهی init_submodule_cb(ثابت ساختار cache_entry *list_item, تهی *cb_data)
{
	ساختار init_cb *info = cb_data;

	init_submodule(list_item->name, info->prefix, info->super_prefix,
		       info->flags);
}

ایستا صحیح module_init(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix,
		       ساختار repository *repo UNUSED)
{
	ساختار init_cb info = INIT_CB_INIT;
	ساختار pathspec pathspec = { 0 };
	ساختار module_list list = MODULE_LIST_INIT;
	صحیح quiet = 0;
	ساختار option module_init_options[] = {
		OPT__QUIET(&quiet, N_("suppress output for initializing a submodule")),
		OPT_END()
	};
	ثابت نویسه *ثابت git_submodule_helper_usage[] = {
		N_("git submodule init [<options>] [<path>]"),
		پوچ
	};
	صحیح ret = 1;

	argc = parse_options(argc, argv, prefix, module_init_options,
			     git_submodule_helper_usage, 0);

	اگر (module_list_compute(argv, prefix, &pathspec, &list) < 0)
		برو cleanup;

	/*
	 * If there are no path args and submodule.active is set then,
	 * by default, only initialize 'active' modules.
	 */
	اگر (!argc && !repo_config_get(the_repository, "submodule.active"))
		module_list_active(&list);

	info.prefix = prefix;
	اگر (quiet)
		info.flags |= OPT_QUIET;

	for_each_listed_submodule(&list, init_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	بازگشت ret;
}

ساختار status_cb {
	ثابت نویسه *prefix;
	ثابت نویسه *super_prefix;
	unsigned صحیح flags;
};
#define STATUS_CB_INIT { 0 }

ایستا تهی print_status(unsigned صحیح flags, نویسه state, ثابت نویسه *path,
			 ثابت ساختار object_id *oid, ثابت نویسه *displaypath)
{
	اگر (flags & OPT_QUIET)
		بازگشت;

	printf("%c%s %s", state, oid_to_hex(oid), displaypath);

	اگر (state == ' ' || state == '+') {
		نویسه *name = compute_rev_name(path, oid_to_hex(oid));

		اگر (name)
			printf(" (%s)", name);
		آزاد(name);
	}

	printf("\n");
}

ایستا صحیح handle_submodule_head_ref(ثابت ساختار reference *ref, تهی *cb_data)
{
	ساختار object_id *output = cb_data;

	اگر (ref->oid)
		oidcpy(output, ref->oid);

	بازگشت 0;
}

ایستا تهی status_submodule(ثابت نویسه *path, ثابت ساختار object_id *ce_oid,
			     unsigned صحیح ce_flags, ثابت نویسه *prefix,
			     ثابت نویسه *super_prefix, unsigned صحیح flags)
{
	نویسه *displaypath;
	ساختار strvec diff_files_args = STRVEC_INIT;
	ساختار rev_info rev = REV_INFO_INIT;
	ساختار strbuf buf = STRBUF_INIT;
	ثابت نویسه *git_dir;

	اگر (validate_submodule_path(path) < 0)
		die(پوچ);

	اگر (!submodule_from_path(the_repository, null_oid(the_hash_algo), path))
		die(_("no submodule mapping found in .gitmodules for path '%s'"),
		      path);

	displaypath = get_submodule_displaypath(path, prefix, super_prefix);

	اگر ((CE_STAGEMASK & ce_flags) >> CE_STAGESHIFT) {
		print_status(flags, 'U', path, null_oid(the_hash_algo), displaypath);
		برو cleanup;
	}

	strbuf_addf(&buf, "%s/.git", path);
	git_dir = read_gitfile(buf.buf);
	اگر (!git_dir)
		git_dir = buf.buf;

	اگر (!is_submodule_active(the_repository, path) ||
	    !is_git_directory(git_dir)) {
		print_status(flags, '-', path, ce_oid, displaypath);
		strbuf_release(&buf);
		برو cleanup;
	}
	strbuf_release(&buf);

	strvec_pushl(&diff_files_args, "diff-files",
		     "--ignore-submodules=dirty", "--quiet", "--",
		     path, پوچ);

	repo_config(the_repository, git_diff_basic_config, پوچ);

	repo_init_revisions(the_repository, &rev, پوچ);
	rev.abbrev = 0;
	setup_revisions_from_strvec(&diff_files_args, &rev, پوچ);
	run_diff_files(&rev, 0);

	اگر (!diff_result_code(&rev)) {
		print_status(flags, ' ', path, ce_oid,
			     displaypath);
	} وگرنه اگر (!(flags & OPT_CACHED)) {
		ساختار object_id oid;
		ساختار ref_store *refs = repo_get_submodule_ref_store(the_repository,
								      path);

		اگر (!refs) {
			print_status(flags, '-', path, ce_oid, displaypath);
			برو cleanup;
		}
		اگر (refs_head_ref(refs, handle_submodule_head_ref, &oid))
			die(_("could not resolve HEAD ref inside the "
			      "submodule '%s'"), path);

		print_status(flags, '+', path, &oid, displaypath);
	} وگرنه {
		print_status(flags, '+', path, ce_oid, displaypath);
	}

	اگر (flags & OPT_RECURSIVE) {
		ساختار child_process cpr = CHILD_PROCESS_INIT;
		صحیح res;

		cpr.git_cmd = 1;
		cpr.dir = path;
		prepare_submodule_repo_env(&cpr.env);

		strvec_pushl(&cpr.args, "submodule--helper", "status",
			     "--recursive", پوچ);
		strvec_pushf(&cpr.args, "--super-prefix=%s/", displaypath);

		اگر (flags & OPT_CACHED)
			strvec_افزودن(&cpr.args, "--cached");

		اگر (flags & OPT_QUIET)
			strvec_افزودن(&cpr.args, "--quiet");

		res = run_command(&cpr);
		اگر (res == SIGPIPE + 128)
			بالاببر(SIGPIPE);
		وگرنه اگر (res)
			die(_("failed to recurse into submodule '%s'"), path);
	}

cleanup:
	strvec_clear(&diff_files_args);
	آزاد(displaypath);
	release_revisions(&rev);
}

ایستا تهی status_submodule_cb(ثابت ساختار cache_entry *list_item,
				تهی *cb_data)
{
	ساختار status_cb *info = cb_data;

	status_submodule(list_item->name, &list_item->oid, list_item->ce_flags,
			 info->prefix, info->super_prefix, info->flags);
}

ایستا صحیح module_status(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix,
			 ساختار repository *repo UNUSED)
{
	ساختار status_cb info = STATUS_CB_INIT;
	ساختار pathspec pathspec = { 0 };
	ساختار module_list list = MODULE_LIST_INIT;
	صحیح quiet = 0;
	ساختار option module_status_options[] = {
		OPT__SUPER_PREFIX(&info.super_prefix),
		OPT__QUIET(&quiet, N_("suppress submodule status output")),
		OPT_BIT(0, "cached", &info.flags, N_("use commit stored in the index instead of the one stored in the submodule HEAD"), OPT_CACHED),
		OPT_BIT(0, "recursive", &info.flags, N_("recurse into nested submodules"), OPT_RECURSIVE),
		OPT_END()
	};
	ثابت نویسه *ثابت git_submodule_helper_usage[] = {
		N_("git submodule status [--quiet] [--cached] [--recursive] [<path>...]"),
		پوچ
	};
	صحیح ret = 1;

	argc = parse_options(argc, argv, prefix, module_status_options,
			     git_submodule_helper_usage, 0);

	اگر (module_list_compute(argv, prefix, &pathspec, &list) < 0)
		برو cleanup;

	info.prefix = prefix;
	اگر (quiet)
		info.flags |= OPT_QUIET;

	for_each_listed_submodule(&list, status_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	بازگشت ret;
}

ساختار module_cb {
	unsigned صحیح mod_src;
	unsigned صحیح mod_dst;
	ساختار object_id oid_src;
	ساختار object_id oid_dst;
	نویسه status;
	نویسه *sm_path;
};
#define MODULE_CB_INIT { 0 }

ایستا تهی module_cb_release(ساختار module_cb *mcb)
{
	آزاد(mcb->sm_path);
}

ساختار module_cb_list {
	ساختار module_cb **entries;
	صحیح alloc, nr;
};
#define MODULE_CB_LIST_INIT { 0 }

ایستا تهی module_cb_list_release(ساختار module_cb_list *mcbl)
{
	صحیح i;

	برای (i = 0; i < mcbl->nr; i++) {
		ساختار module_cb *mcb = mcbl->entries[i];

		module_cb_release(mcb);
		آزاد(mcb);
	}
	آزاد(mcbl->entries);
}

ساختار summary_cb {
	صحیح argc;
	ثابت نویسه **argv;
	ثابت نویسه *prefix;
	ثابت نویسه *super_prefix;
	unsigned صحیح cached: 1;
	unsigned صحیح for_status: 1;
	unsigned صحیح files: 1;
	صحیح summary_limit;
};
#define SUMMARY_CB_INIT { 0 }

شمارش diff_cmd {
	DIFF_INDEX,
	DIFF_FILES
};

ایستا نویسه *verify_submodule_committish(ثابت نویسه *sm_path,
					 ثابت نویسه *committish)
{
	ساختار child_process cp_rev_parse = CHILD_PROCESS_INIT;
	ساختار strbuf result = STRBUF_INIT;

	cp_rev_parse.git_cmd = 1;
	cp_rev_parse.dir = sm_path;
	prepare_submodule_repo_env(&cp_rev_parse.env);
	strvec_pushl(&cp_rev_parse.args, "rev-parse", "-q", "--short", پوچ);
	strvec_pushf(&cp_rev_parse.args, "%s^0", committish);
	strvec_افزودن(&cp_rev_parse.args, "--");

	اگر (capture_command(&cp_rev_parse, &result, 0))
		بازگشت پوچ;

	strbuf_trim_trailing_newline(&result);
	بازگشت strbuf_detach(&result, پوچ);
}

ایستا تهی print_submodule_summary(ساختار summary_cb *info, ثابت نویسه *errmsg,
				    صحیح total_commits, ثابت نویسه *displaypath,
				    ثابت نویسه *src_abbrev, ثابت نویسه *dst_abbrev,
				    ساختار module_cb *p)
{
	اگر (p->status == 'T') {
		اگر (S_ISGITLINK(p->mod_dst))
			printf(_("* %s %s(blob)->%s(submodule)"),
				 displaypath, src_abbrev, dst_abbrev);
		وگرنه
			printf(_("* %s %s(submodule)->%s(blob)"),
				 displaypath, src_abbrev, dst_abbrev);
	} وگرنه {
		printf("* %s %s...%s",
			displaypath, src_abbrev, dst_abbrev);
	}

	اگر (total_commits < 0)
		printf(":\n");
	وگرنه
		printf(" (%d):\n", total_commits);

	اگر (errmsg) {
		printf(_("%s"), errmsg);
	} وگرنه اگر (total_commits > 0) {
		ساختار child_process cp_log = CHILD_PROCESS_INIT;

		cp_log.git_cmd = 1;
		cp_log.dir = p->sm_path;
		prepare_submodule_repo_env(&cp_log.env);
		strvec_pushl(&cp_log.args, "log", پوچ);

		اگر (S_ISGITLINK(p->mod_src) && S_ISGITLINK(p->mod_dst)) {
			اگر (info->summary_limit > 0)
				strvec_pushf(&cp_log.args, "-%d",
					     info->summary_limit);

			strvec_pushl(&cp_log.args, "--pretty=  %m %s",
				     "--first-parent", پوچ);
			strvec_pushf(&cp_log.args, "%s...%s",
				     src_abbrev, dst_abbrev);
		} وگرنه اگر (S_ISGITLINK(p->mod_dst)) {
			strvec_pushl(&cp_log.args, "--pretty=  > %s",
				     "-1", dst_abbrev, پوچ);
		} وگرنه {
			strvec_pushl(&cp_log.args, "--pretty=  < %s",
				     "-1", src_abbrev, پوچ);
		}
		run_command(&cp_log);
	}
	printf("\n");
}

ایستا تهی generate_submodule_summary(ساختار summary_cb *info,
				       ساختار module_cb *p)
{
	نویسه *displaypath, *src_abbrev = پوچ, *dst_abbrev;
	صحیح missing_src = 0, missing_dst = 0;
	ساختار strbuf errmsg = STRBUF_INIT;
	صحیح total_commits = -1;

	اگر (!info->cached && oideq(&p->oid_dst, null_oid(the_hash_algo))) {
		اگر (S_ISGITLINK(p->mod_dst)) {
			ساختار ref_store *refs = repo_get_submodule_ref_store(the_repository,
									      p->sm_path);

			اگر (refs)
				refs_head_ref(refs, handle_submodule_head_ref, &p->oid_dst);
		} وگرنه اگر (S_ISLNK(p->mod_dst) || S_ISREG(p->mod_dst)) {
			ساختار stat st;
			صحیح fd = open(p->sm_path, O_RDONLY);

			اگر (fd < 0 || fstat(fd, &st) < 0 ||
			    index_fd(the_repository->index, &p->oid_dst, fd, &st, OBJ_BLOB,
				     p->sm_path, 0))
				error(_("couldn't hash object from '%s'"), p->sm_path);
		} وگرنه {
			/* for a submodule removal (mode:0000000), don't warn */
			اگر (p->mod_dst)
				warning(_("unexpected mode %o"), p->mod_dst);
		}
	}

	اگر (S_ISGITLINK(p->mod_src)) {
		اگر (p->status != 'D')
			src_abbrev = verify_submodule_committish(p->sm_path,
								 oid_to_hex(&p->oid_src));
		اگر (!src_abbrev) {
			missing_src = 1;
			/*
			 * As `rev-parse` failed, we fallback to getting
			 * the abbreviated hash using oid_src. We do
			 * this as we might still need the abbreviated
			 * hash in cases like a submodule type change, etc.
			 */
			src_abbrev = xstrndup(oid_to_hex(&p->oid_src), 7);
		}
	} وگرنه {
		/*
		 * The source does not point to a submodule.
		 * So, we fallback to getting the abbreviation using
		 * oid_src as we might still need the abbreviated
		 * hash in cases like submodule add, etc.
		 */
		src_abbrev = xstrndup(oid_to_hex(&p->oid_src), 7);
	}

	اگر (S_ISGITLINK(p->mod_dst)) {
		dst_abbrev = verify_submodule_committish(p->sm_path,
							 oid_to_hex(&p->oid_dst));
		اگر (!dst_abbrev) {
			missing_dst = 1;
			/*
			 * As `rev-parse` failed, we fallback to getting
			 * the abbreviated hash using oid_dst. We do
			 * this as we might still need the abbreviated
			 * hash in cases like a submodule type change, etc.
			 */
			dst_abbrev = xstrndup(oid_to_hex(&p->oid_dst), 7);
		}
	} وگرنه {
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

	اگر (!missing_src && !missing_dst) {
		ساختار child_process cp_rev_list = CHILD_PROCESS_INIT;
		ساختار strbuf sb_rev_list = STRBUF_INIT;

		strvec_pushl(&cp_rev_list.args, "rev-list",
			     "--first-parent", "--count", پوچ);
		اگر (S_ISGITLINK(p->mod_src) && S_ISGITLINK(p->mod_dst))
			strvec_pushf(&cp_rev_list.args, "%s...%s",
				     src_abbrev, dst_abbrev);
		وگرنه
			strvec_افزودن(&cp_rev_list.args, S_ISGITLINK(p->mod_src) ?
				    src_abbrev : dst_abbrev);
		strvec_افزودن(&cp_rev_list.args, "--");

		cp_rev_list.git_cmd = 1;
		cp_rev_list.dir = p->sm_path;
		prepare_submodule_repo_env(&cp_rev_list.env);

		اگر (!capture_command(&cp_rev_list, &sb_rev_list, 0))
			total_commits = atoi(sb_rev_list.buf);

		strbuf_release(&sb_rev_list);
	} وگرنه {
		/*
		 * Don't give error msg for modification whose dst is not
		 * submodule, i.e., deleted or changed to blob
		 */
		اگر (S_ISGITLINK(p->mod_dst)) {
			اگر (missing_src && missing_dst) {
				strbuf_addf(&errmsg, "  Warn: %s doesn't contain commits %s and %s\n",
					    displaypath, oid_to_hex(&p->oid_src),
					    oid_to_hex(&p->oid_dst));
			} وگرنه {
				strbuf_addf(&errmsg, "  Warn: %s doesn't contain commit %s\n",
					    displaypath, missing_src ?
					    oid_to_hex(&p->oid_src) :
					    oid_to_hex(&p->oid_dst));
			}
		}
	}

	print_submodule_summary(info, errmsg.len ? errmsg.buf : پوچ,
				total_commits, displaypath, src_abbrev,
				dst_abbrev, p);

	آزاد(displaypath);
	آزاد(src_abbrev);
	آزاد(dst_abbrev);
	strbuf_release(&errmsg);
}

ایستا تهی prepare_submodule_summary(ساختار summary_cb *info,
				      ساختار module_cb_list *list)
{
	صحیح i;
	برای (i = 0; i < list->nr; i++) {
		ثابت ساختار submodule *sub;
		ساختار module_cb *p = list->entries[i];
		ساختار strbuf sm_gitdir = STRBUF_INIT;

		اگر (p->status == 'D' || p->status == 'T') {
			generate_submodule_summary(info, p);
			ادامه;
		}

		اگر (info->for_status && p->status != 'A' &&
		    (sub = submodule_from_path(the_repository,
					       null_oid(the_hash_algo), p->sm_path))) {
			نویسه *config_کلید = پوچ;
			ثابت نویسه *مقدار;
			صحیح ignore_all = 0;

			config_کلید = xstrfmt("submodule.%s.ignore",
					     sub->name);
			اگر (!repo_config_get_string_tmp(the_repository, config_کلید, &مقدار))
				ignore_all = !strcmp(مقدار, "all");
			وگرنه اگر (sub->ignore)
				ignore_all = !strcmp(sub->ignore, "all");

			آزاد(config_کلید);
			اگر (ignore_all)
				ادامه;
		}

		/* Also show added or modified modules which are checked out */
		strbuf_addstr(&sm_gitdir, p->sm_path);
		اگر (is_nonbare_repository_dir(&sm_gitdir))
			generate_submodule_summary(info, p);
		strbuf_release(&sm_gitdir);
	}
}

ایستا تهی submodule_summary_callback(ساختار diff_queue_struct *q,
				       ساختار diff_options *options UNUSED,
				       تهی *data)
{
	صحیح i;
	ساختار module_cb_list *list = data;
	برای (i = 0; i < q->nr; i++) {
		ساختار diff_filepair *p = q->queue[i];
		ساختار module_cb *temp;

		اگر (!S_ISGITLINK(p->one->mode) && !S_ISGITLINK(p->two->mode))
			ادامه;
		temp = xmalloc(اندازه(*temp));
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

ایستا ثابت نویسه *get_diff_cmd(شمارش diff_cmd diff_cmd)
{
	انتخاب (diff_cmd) {
	حالت DIFF_INDEX: بازگشت "diff-index";
	حالت DIFF_FILES: بازگشت "diff-files";
	پیشفرض: BUG("bad diff_cmd value %d", diff_cmd);
	}
}

ایستا صحیح compute_summary_module_list(ساختار object_id *head_oid,
				       ساختار summary_cb *info,
				       شمارش diff_cmd diff_cmd)
{
	ساختار strvec diff_args = STRVEC_INIT;
	ساختار rev_info rev;
	ساختار module_cb_list list = MODULE_CB_LIST_INIT;
	صحیح ret = 0;

	strvec_افزودن(&diff_args, get_diff_cmd(diff_cmd));
	اگر (info->cached)
		strvec_افزودن(&diff_args, "--cached");
	strvec_pushl(&diff_args, "--ignore-submodules=dirty", "--raw", پوچ);
	اگر (head_oid)
		strvec_افزودن(&diff_args, oid_to_hex(head_oid));
	strvec_افزودن(&diff_args, "--");
	اگر (info->argc)
		strvec_pushv(&diff_args, info->argv);

	repo_config(the_repository, git_diff_basic_config, پوچ);
	repo_init_revisions(the_repository, &rev, info->prefix);
	rev.abbrev = 0;
	precompose_argv_prefix(diff_args.nr, diff_args.v, پوچ);
	setup_revisions_from_strvec(&diff_args, &rev, پوچ);
	rev.diffopt.output_format = DIFF_FORMAT_NO_OUTPUT | DIFF_FORMAT_CALLBACK;
	rev.diffopt.format_callback = submodule_summary_callback;
	rev.diffopt.format_callback_data = &list;

	اگر (!info->cached) {
		اگر (diff_cmd == DIFF_INDEX)
			setup_work_درخت(the_repository);
		اگر (repo_read_index_preload(the_repository, &rev.diffopt.pathspec, 0) < 0) {
			perror("repo_read_index_preload");
			ret = -1;
			برو cleanup;
		}
	} وگرنه اگر (repo_read_index(the_repository) < 0) {
		perror("repo_read_cache");
		ret = -1;
		برو cleanup;
	}

	اگر (diff_cmd == DIFF_INDEX)
		run_diff_index(&rev, info->cached ? DIFF_INDEX_CACHED : 0);
	وگرنه
		run_diff_files(&rev, 0);
	prepare_submodule_summary(info, &list);
cleanup:
	strvec_clear(&diff_args);
	release_revisions(&rev);
	module_cb_list_release(&list);
	بازگشت ret;
}

ایستا صحیح module_summary(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix,
			  ساختار repository *repo UNUSED)
{
	ساختار summary_cb info = SUMMARY_CB_INIT;
	صحیح cached = 0;
	صحیح for_status = 0;
	صحیح files = 0;
	صحیح summary_limit = -1;
	شمارش diff_cmd diff_cmd = DIFF_INDEX;
	ساختار object_id head_oid;
	صحیح ret;
	ساختار option module_summary_options[] = {
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
	ثابت نویسه *ثابت git_submodule_helper_usage[] = {
		N_("git submodule summary [<options>] [<commit>] [--] [<path>]"),
		پوچ
	};

	argc = parse_options(argc, argv, prefix, module_summary_options,
			     git_submodule_helper_usage, 0);

	اگر (!summary_limit)
		بازگشت 0;

	اگر (!repo_get_oid(the_repository, argc ? argv[0] : "HEAD", &head_oid)) {
		اگر (argc) {
			argv++;
			argc--;
		}
	} وگرنه اگر (!argc || !strcmp(argv[0], "HEAD")) {
		/* before the first commit: compare with an empty tree */
		oidcpy(&head_oid, the_hash_algo->empty_درخت);
		اگر (argc) {
			argv++;
			argc--;
		}
	} وگرنه {
		اگر (repo_get_oid(the_repository, "HEAD", &head_oid))
			die(_("could not fetch a revision for HEAD"));
	}

	اگر (files) {
		اگر (cached)
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

	ret = compute_summary_module_list((diff_cmd == DIFF_INDEX) ? &head_oid : پوچ,
					  &info, diff_cmd);
	بازگشت ret;
}

ایستا صحیح module_gitdir(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix UNUSED,
			 ساختار repository *repo)
{
	ساختار strbuf gitdir = STRBUF_INIT;

	اگر (argc != 2)
		usage(_("git submodule--helper gitdir <name>"));

	submodule_name_to_gitdir(&gitdir, repo, argv[1]);

	printf("%s\n", gitdir.buf);

	strbuf_release(&gitdir);
	بازگشت 0;
}

ایستا صحیح module_migrate(صحیح argc UNUSED, ثابت نویسه **argv UNUSED,
			  ثابت نویسه *prefix UNUSED, ساختار repository *repo)
{
	ساختار strbuf module_dir = STRBUF_INIT;
	DIR *dir;
	ساختار dirent *de;
	صحیح repo_version = 0;

	repo_git_path_append(repo, &module_dir, "modules/");

	dir = opendir(module_dir.buf);
	اگر (!dir)
		die(_("could not open '%s'"), module_dir.buf);

	وقتی ((de = readdir(dir))) {
		ساختار strbuf gitdir_path = STRBUF_INIT;
		نویسه *کلید;
		ثابت نویسه *مقدار;

		اگر (is_dot_or_dotdot(de->d_name))
			ادامه;

		strbuf_addf(&gitdir_path, "%s/%s", module_dir.buf, de->d_name);
		اگر (!is_git_directory(gitdir_path.buf)) {
			strbuf_release(&gitdir_path);
			ادامه;
		}
		strbuf_release(&gitdir_path);

		کلید = xstrfmt("submodule.%s.gitdir", de->d_name);
		اگر (!repo_config_get_string_tmp(repo, کلید, &مقدار)) {
			/* Already has a gitdir config, nothing to do. */
			آزاد(کلید);
			ادامه;
		}
		آزاد(کلید);

		create_default_gitdir_config(de->d_name);
	}

	closedir(dir);
	strbuf_release(&module_dir);

	repo_config_get_int(the_repository, "core.repositoryformatversion", &repo_version);
	اگر (repo_version == 0 &&
	    repo_config_set_gently(repo, "core.repositoryformatversion", "1"))
		die(_("could not set core.repositoryformatversion to 1.\n"
		      "Please set it for migration to work, for example:\n"
		      "git config core.repositoryformatversion 1"));

	اگر (repo_config_set_gently(repo, "extensions.submodulePathConfig", "true"))
		die(_("could not enable submodulePathConfig extension. It is required\n"
		      "for migration to work. Please enable it in the root repo:\n"
		      "git config extensions.submodulePathConfig true"));

	repo->repository_format_submodule_path_cfg = 1;

	بازگشت 0;
}

ساختار sync_cb {
	ثابت نویسه *prefix;
	ثابت نویسه *super_prefix;
	unsigned صحیح flags;
};
#define SYNC_CB_INIT { 0 }

ایستا تهی sync_submodule(ثابت نویسه *path, ثابت نویسه *prefix,
			   ثابت نویسه *super_prefix, unsigned صحیح flags)
{
	ثابت ساختار submodule *sub;
	نویسه *remote_کلید = پوچ;
	نویسه *sub_origin_url, *super_config_url, *displaypath, *default_remote;
	ساختار strbuf sb = STRBUF_INIT;
	نویسه *sub_config_path = پوچ;
	صحیح code;

	اگر (!is_submodule_active(the_repository, path))
		بازگشت;

	اگر (validate_submodule_path(path) < 0)
		die(پوچ);

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);

	اگر (sub && sub->url) {
		اگر (starts_with_dot_dot_slash(sub->url) ||
		    starts_with_dot_slash(sub->url)) {
			نویسه *up_path = get_up_path(path);

			sub_origin_url = resolve_relative_url(sub->url, up_path, 1);
			super_config_url = resolve_relative_url(sub->url, پوچ, 1);
			آزاد(up_path);
		} وگرنه {
			sub_origin_url = xstrdup(sub->url);
			super_config_url = xstrdup(sub->url);
		}
	} وگرنه {
		sub_origin_url = xstrdup("");
		super_config_url = xstrdup("");
	}

	displaypath = get_submodule_displaypath(path, prefix, super_prefix);

	اگر (!(flags & OPT_QUIET))
		printf(_("Synchronizing submodule url for '%s'\n"),
			 displaypath);

	strbuf_reset(&sb);
	strbuf_addf(&sb, "submodule.%s.url", sub->name);
	اگر (repo_config_set_gently(the_repository, sb.buf, super_config_url))
		die(_("failed to register url for submodule path '%s'"),
		      displaypath);

	اگر (!is_submodule_populated_gently(path, پوچ))
		برو cleanup;

	strbuf_reset(&sb);
	code = get_default_remote_submodule(path, &default_remote);
	اگر (code)
		exit(code);

	remote_کلید = xstrfmt("remote.%s.url", default_remote);
	آزاد(default_remote);

	submodule_to_gitdir(the_repository, &sb, path);
	strbuf_addstr(&sb, "/config");

	اگر (repo_config_set_in_file_gently(the_repository, sb.buf, remote_کلید, پوچ, sub_origin_url))
		die(_("failed to update remote for submodule '%s'"),
		      path);

	اگر (flags & OPT_RECURSIVE) {
		ساختار child_process cpr = CHILD_PROCESS_INIT;

		cpr.git_cmd = 1;
		cpr.dir = path;
		prepare_submodule_repo_env(&cpr.env);

		strvec_pushl(&cpr.args, "submodule--helper", "sync",
			     "--recursive", پوچ);
		strvec_pushf(&cpr.args, "--super-prefix=%s/", displaypath);

		اگر (flags & OPT_QUIET)
			strvec_افزودن(&cpr.args, "--quiet");

		اگر (run_command(&cpr))
			die(_("failed to recurse into submodule '%s'"),
			      path);
	}

cleanup:
	آزاد(super_config_url);
	آزاد(sub_origin_url);
	strbuf_release(&sb);
	آزاد(remote_کلید);
	آزاد(displaypath);
	آزاد(sub_config_path);
}

ایستا تهی sync_submodule_cb(ثابت ساختار cache_entry *list_item, تهی *cb_data)
{
	ساختار sync_cb *info = cb_data;

	sync_submodule(list_item->name, info->prefix, info->super_prefix,
		       info->flags);
}

ایستا صحیح module_sync(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix,
		       ساختار repository *repo UNUSED)
{
	ساختار sync_cb info = SYNC_CB_INIT;
	ساختار pathspec pathspec = { 0 };
	ساختار module_list list = MODULE_LIST_INIT;
	صحیح quiet = 0;
	صحیح recursive = 0;
	ساختار option module_sync_options[] = {
		OPT__SUPER_PREFIX(&info.super_prefix),
		OPT__QUIET(&quiet, N_("suppress output of synchronizing submodule url")),
		OPT_BOOL(0, "recursive", &recursive,
			N_("recurse into nested submodules")),
		OPT_END()
	};
	ثابت نویسه *ثابت git_submodule_helper_usage[] = {
		N_("git submodule sync [--quiet] [--recursive] [<path>]"),
		پوچ
	};
	صحیح ret = 1;

	argc = parse_options(argc, argv, prefix, module_sync_options,
			     git_submodule_helper_usage, 0);

	اگر (module_list_compute(argv, prefix, &pathspec, &list) < 0)
		برو cleanup;

	info.prefix = prefix;
	اگر (quiet)
		info.flags |= OPT_QUIET;
	اگر (recursive)
		info.flags |= OPT_RECURSIVE;

	for_each_listed_submodule(&list, sync_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	بازگشت ret;
}

ساختار deinit_cb {
	ثابت نویسه *prefix;
	unsigned صحیح flags;
};
#define DEINIT_CB_INIT { 0 }

ایستا تهی deinit_submodule(ثابت نویسه *path, ثابت نویسه *prefix,
			     unsigned صحیح flags)
{
	ثابت ساختار submodule *sub;
	نویسه *displaypath = پوچ;
	ساختار child_process cp_config = CHILD_PROCESS_INIT;
	ساختار strbuf sb_config = STRBUF_INIT;
	نویسه *sub_git_dir = xstrfmt("%s/.git", path);

	اگر (validate_submodule_path(path) < 0)
		die(پوچ);

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);

	اگر (!sub || !sub->name)
		برو cleanup;

	displaypath = get_submodule_displaypath(path, prefix, پوچ);

	/* remove the submodule work tree (unless the user already did it) */
	اگر (is_directory(path)) {
		ساختار strbuf sb_rm = STRBUF_INIT;
		ثابت نویسه *format;

		اگر (is_directory(sub_git_dir)) {
			اگر (!(flags & OPT_QUIET))
				warning(_("Submodule work tree '%s' contains a .git "
					  "directory. This will be replaced with a "
					  ".git file by using absorbgitdirs."),
					displaypath);

			absorb_git_dir_into_superproject(path, پوچ);

		}

		اگر (!(flags & OPT_FORCE)) {
			ساختار child_process cp_rm = CHILD_PROCESS_INIT;

			cp_rm.git_cmd = 1;
			strvec_pushl(&cp_rm.args, "rm", "-qn",
				     path, پوچ);

			اگر (run_command(&cp_rm))
				die(_("Submodule work tree '%s' contains local "
				      "modifications; use '-f' to discard them"),
				      displaypath);
		}

		strbuf_addstr(&sb_rm, path);

		اگر (!remove_dir_recursively(&sb_rm, 0))
			format = _("Cleared directory '%s'\n");
		وگرنه
			format = _("Could not remove submodule work tree '%s'\n");

		اگر (!(flags & OPT_QUIET))
			printf(format, displaypath);

		submodule_unset_core_worktree(sub);

		strbuf_release(&sb_rm);
	}

	اگر (mkdir(path, 0777))
		printf(_("could not create empty submodule directory %s"),
		      displaypath);

	cp_config.git_cmd = 1;
	strvec_pushl(&cp_config.args, "config", "--get-regexp", پوچ);
	strvec_pushf(&cp_config.args, "submodule.%s\\.", sub->name);

	/* remove the .git/config entries (unless the user already did it) */
	اگر (!capture_command(&cp_config, &sb_config, 0) && sb_config.len) {
		نویسه *sub_کلید = xstrfmt("submodule.%s", sub->name);

		/*
		 * remove the whole section so we have a clean state when
		 * the user later decides to init this submodule again
		 */
		repo_config_rename_section_in_file(the_repository, پوچ, sub_کلید, پوچ);
		اگر (!(flags & OPT_QUIET))
			printf(_("Submodule '%s' (%s) unregistered for path '%s'\n"),
				 sub->name, sub->url, displaypath);
		آزاد(sub_کلید);
	}

cleanup:
	آزاد(displaypath);
	آزاد(sub_git_dir);
	strbuf_release(&sb_config);
}

ایستا تهی deinit_submodule_cb(ثابت ساختار cache_entry *list_item,
				تهی *cb_data)
{
	ساختار deinit_cb *info = cb_data;
	deinit_submodule(list_item->name, info->prefix, info->flags);
}

ایستا صحیح module_deinit(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix,
			 ساختار repository *repo UNUSED)
{
	ساختار deinit_cb info = DEINIT_CB_INIT;
	ساختار pathspec pathspec = { 0 };
	ساختار module_list list = MODULE_LIST_INIT;
	صحیح quiet = 0;
	صحیح force = 0;
	صحیح all = 0;
	ساختار option module_deinit_options[] = {
		OPT__QUIET(&quiet, N_("suppress submodule status output")),
		OPT__FORCE(&force, N_("remove submodule working trees even if they contain local changes"), 0),
		OPT_BOOL(0, "all", &all, N_("unregister all submodules")),
		OPT_END()
	};
	ثابت نویسه *ثابت git_submodule_helper_usage[] = {
		N_("git submodule deinit [--quiet] [-f | --force] [--all | [--] [<path>...]]"),
		پوچ
	};
	صحیح ret = 1;

	argc = parse_options(argc, argv, prefix, module_deinit_options,
			     git_submodule_helper_usage, 0);

	اگر (all && argc) {
		error("pathspec and --all are incompatible");
		usage_with_options(git_submodule_helper_usage,
				   module_deinit_options);
	}

	اگر (!argc && !all)
		die(_("Use '--all' if you really want to deinitialize all submodules"));

	اگر (module_list_compute(argv, prefix, &pathspec, &list) < 0)
		برو cleanup;

	info.prefix = prefix;
	اگر (quiet)
		info.flags |= OPT_QUIET;
	اگر (force)
		info.flags |= OPT_FORCE;

	for_each_listed_submodule(&list, deinit_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	بازگشت ret;
}

ساختار module_clone_data {
	ثابت نویسه *prefix;
	ثابت نویسه *path;
	ثابت نویسه *name;
	ثابت نویسه *url;
	صحیح depth;
	ساختار list_objects_filter_options *filter_options;
	شمارش ref_storage_format ref_storage_format;
	unsigned صحیح quiet: 1;
	unsigned صحیح progress: 1;
	unsigned صحیح dissociate: 1;
	unsigned صحیح require_init: 1;
	صحیح single_branch;
};
#define MODULE_CLONE_DATA_INIT { \
	.single_branch = -1, \
	.ref_storage_format = REF_STORAGE_FORMAT_UNKNOWN, \
}

ساختار submodule_alternate_setup {
	ثابت نویسه *submodule_name;
	شمارش SUBMODULE_ALTERNATE_ERROR_MODE {
		SUBMODULE_ALTERNATE_ERROR_DIE,
		SUBMODULE_ALTERNATE_ERROR_INFO,
		SUBMODULE_ALTERNATE_ERROR_IGNORE
	} error_mode;
	ساختار string_list *reference;
};
#define SUBMODULE_ALTERNATE_SETUP_INIT { \
	.error_mode = SUBMODULE_ALTERNATE_ERROR_IGNORE, \
}

ایستا ثابت نویسه alternate_error_advice[] = N_(
"An alternate computed from a superproject's alternate is invalid.\n"
"To allow Git to clone without an alternate in such a case, set\n"
"submodule.alternateErrorStrategy to 'info' or, equivalently, clone with\n"
"'--reference-if-able' instead of '--reference'."
);

ایستا صحیح add_possible_reference_from_superproject(
		ساختار odb_files_dir *alt_odb, تهی *sas_cb)
{
	ساختار submodule_alternate_setup *sas = sas_cb;
	size_t len;

	/*
	 * If the alternate object store is another repository, try the
	 * standard layout with .git/(modules/<name>)+/objects
	 */
	اگر (strip_suffix(alt_odb->abspath, "/objects", &len)) {
		ساختار repository alternate;
		نویسه *sm_alternate;
		ساختار strbuf sb = STRBUF_INIT;
		ساختار strbuf err = STRBUF_INIT;
		strbuf_add(&sb, alt_odb->abspath, len);

		اگر (repo_init(&alternate, sb.buf, پوچ) < 0)
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
		اگر (sm_alternate) {
			نویسه *p = strbuf_detach(&sb, پوچ);

			string_list_append(sas->reference, p)->util = p;
			آزاد(sm_alternate);
		} وگرنه {
			انتخاب (sas->error_mode) {
			حالت SUBMODULE_ALTERNATE_ERROR_DIE:
				اگر (advice_enabled(ADVICE_SUBMODULE_ALTERNATE_ERROR_STRATEGY_DIE))
					advise(_(alternate_error_advice));
				die(_("submodule '%s' cannot add alternate: %s"),
				    sas->submodule_name, err.buf);
			حالت SUBMODULE_ALTERNATE_ERROR_INFO:
				fprintf_ln(stderr, _("submodule '%s' cannot add alternate: %s"),
					sas->submodule_name, err.buf);
			حالت SUBMODULE_ALTERNATE_ERROR_IGNORE:
				; /* nothing */
			}
		}

		strbuf_release(&err);
		strbuf_release(&sb);
	}

	بازگشت 0;
}

ایستا تهی prepare_possible_alternates(ثابت نویسه *sm_name,
		ساختار string_list *reference)
{
	نویسه *sm_alternate = پوچ, *error_strategy = پوچ;
	ساختار submodule_alternate_setup sas = SUBMODULE_ALTERNATE_SETUP_INIT;

	repo_config_get_string(the_repository, "submodule.alternateLocation", &sm_alternate);
	اگر (!sm_alternate)
		بازگشت;

	repo_config_get_string(the_repository, "submodule.alternateErrorStrategy", &error_strategy);

	اگر (!error_strategy)
		error_strategy = xstrdup("die");

	sas.submodule_name = sm_name;
	sas.reference = reference;
	اگر (!strcmp(error_strategy, "die"))
		sas.error_mode = SUBMODULE_ALTERNATE_ERROR_DIE;
	وگرنه اگر (!strcmp(error_strategy, "info"))
		sas.error_mode = SUBMODULE_ALTERNATE_ERROR_INFO;
	وگرنه اگر (!strcmp(error_strategy, "ignore"))
		sas.error_mode = SUBMODULE_ALTERNATE_ERROR_IGNORE;
	وگرنه
		die(_("Value '%s' for submodule.alternateErrorStrategy is not recognized"), error_strategy);

	اگر (!strcmp(sm_alternate, "superproject"))
		odb_for_each_alternate(the_repository->objects,
				       add_possible_reference_from_superproject, &sas);
	وگرنه اگر (!strcmp(sm_alternate, "no"))
		; /* do nothing */
	وگرنه
		die(_("Value '%s' for submodule.alternateLocation is not recognized"), sm_alternate);

	آزاد(sm_alternate);
	آزاد(error_strategy);
}

ایستا نویسه *clone_submodule_sm_gitdir(ثابت نویسه *name)
{
	ساختار strbuf sb = STRBUF_INIT;
	نویسه *sm_gitdir;

	submodule_name_to_gitdir(&sb, the_repository, name);
	sm_gitdir = absolute_pathdup(sb.buf);
	strbuf_release(&sb);

	بازگشت sm_gitdir;
}

ایستا صحیح dir_contains_only_dotgit(ثابت نویسه *path)
{
	DIR *dir = opendir(path);
	ساختار dirent *e;
	صحیح ret = 1;

	اگر (!dir)
		بازگشت 0;

	e = readdir_skip_dot_and_dotdot(dir);
	اگر (!e)
		ret = 0;
	وگرنه اگر (strcmp(DEFAULT_GIT_DIR_ENVIRONMENT, e->d_name) ||
		 (e = readdir_skip_dot_and_dotdot(dir))) {
		error("unexpected item '%s' in '%s'", e->d_name, path);
		ret = 0;
	}

	closedir(dir);
	بازگشت ret;
}

ایستا صحیح clone_submodule(ثابت ساختار module_clone_data *clone_data,
			   ساختار string_list *reference)
{
	نویسه *p;
	نویسه *sm_gitdir = clone_submodule_sm_gitdir(clone_data->name);
	نویسه *sm_alternate = پوچ, *error_strategy = پوچ;
	ساختار stat st;
	ساختار child_process cp = CHILD_PROCESS_INIT;
	ثابت نویسه *clone_data_path = clone_data->path;
	نویسه *to_free = پوچ;

	اگر (validate_submodule_path(clone_data_path) < 0)
		die(پوچ);

	اگر (!is_absolute_path(clone_data->path))
		clone_data_path = to_free = xstrfmt("%s/%s", repo_get_work_درخت(the_repository),
						    clone_data->path);

	اگر (!file_exists(sm_gitdir)) {
		اگر (clone_data->require_init && !stat(clone_data_path, &st) &&
		    !is_empty_dir(clone_data_path))
			die(_("directory not empty: '%s'"), clone_data_path);

		اگر (safe_create_leading_directories_const(the_repository, sm_gitdir) < 0)
			die(_("could not create directory '%s'"), sm_gitdir);

		prepare_possible_alternates(clone_data->name, reference);

		strvec_افزودن(&cp.args, "clone");
		strvec_افزودن(&cp.args, "--no-checkout");
		اگر (clone_data->quiet)
			strvec_افزودن(&cp.args, "--quiet");
		اگر (clone_data->progress)
			strvec_افزودن(&cp.args, "--progress");
		اگر (clone_data->depth > 0)
			strvec_pushf(&cp.args, "--depth=%d", clone_data->depth);
		اگر (reference->nr) {
			ساختار string_list_item *item;

			for_each_string_list_item(item, reference)
				strvec_pushl(&cp.args, "--reference",
					     item->string, پوچ);
		}
		اگر (clone_data->ref_storage_format != REF_STORAGE_FORMAT_UNKNOWN)
			strvec_pushf(&cp.args, "--ref-storage-format=%s",
				     ref_storage_format_to_name(clone_data->ref_storage_format));
		اگر (clone_data->dissociate)
			strvec_افزودن(&cp.args, "--dissociate");
		اگر (sm_gitdir && *sm_gitdir)
			strvec_pushl(&cp.args, "--separate-git-dir", sm_gitdir, پوچ);
		اگر (clone_data->filter_options && clone_data->filter_options->choice)
			strvec_pushf(&cp.args, "--filter=%s",
				     expand_list_objects_filter_spec(
					     clone_data->filter_options));
		اگر (clone_data->single_branch >= 0)
			strvec_افزودن(&cp.args, clone_data->single_branch ?
				    "--single-branch" :
				    "--no-single-branch");

		strvec_افزودن(&cp.args, "--");
		strvec_افزودن(&cp.args, clone_data->url);
		strvec_افزودن(&cp.args, clone_data_path);

		cp.git_cmd = 1;
		prepare_submodule_repo_env(&cp.env);
		cp.no_stdin = 1;

		اگر(run_command(&cp))
			die(_("clone of '%s' into submodule path '%s' failed"),
			    clone_data->url, clone_data_path);

		اگر (clone_data->require_init && !stat(clone_data_path, &st) &&
		    !dir_contains_only_dotgit(clone_data_path)) {
			نویسه *dot_git = xstrfmt("%s/.git", clone_data_path);
			unlink(dot_git);
			آزاد(dot_git);
			die(_("directory not empty: '%s'"), clone_data_path);
		}
	} وگرنه {
		نویسه *path;

		اگر (clone_data->require_init && !stat(clone_data_path, &st) &&
		    !is_empty_dir(clone_data_path))
			die(_("directory not empty: '%s'"), clone_data_path);
		اگر (safe_create_leading_directories_const(the_repository, clone_data_path) < 0)
			die(_("could not create directory '%s'"), clone_data_path);
		path = xstrfmt("%s/index", sm_gitdir);
		unlink_or_warn(path);
		آزاد(path);
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
	اگر (validate_submodule_git_dir(sm_gitdir, clone_data->name) < 0) {
		نویسه *head = xstrfmt("%s/HEAD", sm_gitdir);
		unlink(head);
		آزاد(head);
		die(_("refusing to create/use '%s' in another submodule's git dir. "
		      "Enabling extensions.submodulePathConfig should fix this."),
		    sm_gitdir);
	}

	connect_work_درخت_and_git_dir(clone_data_path, sm_gitdir, 0);

	p = repo_submodule_path(the_repository, clone_data_path, "config");
	اگر (!p)
		die(_("could not get submodule directory for '%s'"), clone_data_path);

	/* setup alternateLocation and alternateErrorStrategy in the cloned submodule if needed */
	repo_config_get_string(the_repository, "submodule.alternateLocation", &sm_alternate);
	اگر (sm_alternate)
		repo_config_set_in_file(the_repository, p, "submodule.alternateLocation",
					sm_alternate);
	repo_config_get_string(the_repository, "submodule.alternateErrorStrategy", &error_strategy);
	اگر (error_strategy)
		repo_config_set_in_file(the_repository, p, "submodule.alternateErrorStrategy",
					error_strategy);

	آزاد(sm_alternate);
	آزاد(error_strategy);

	آزاد(sm_gitdir);
	آزاد(p);
	آزاد(to_free);
	بازگشت 0;
}

ایستا صحیح module_clone(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix,
			ساختار repository *repo UNUSED)
{
	صحیح dissociate = 0, quiet = 0, progress = 0, require_init = 0;
	ساختار module_clone_data clone_data = MODULE_CLONE_DATA_INIT;
	ساختار string_list reference = STRING_LIST_INIT_NODUP;
	ساختار list_objects_filter_options filter_options =
		LIST_OBJECTS_FILTER_INIT;
	ثابت نویسه *ref_storage_format = پوچ;

	ساختار option module_clone_options[] = {
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
	ثابت نویسه *ثابت git_submodule_helper_usage[] = {
		N_("git submodule--helper clone [--prefix=<path>] [--quiet] "
		   "[--reference <repository>] [--name <name>] [--depth <depth>] "
		   "[--single-branch] [--filter <filter-spec>] "
		   "--url <url> --path <path>"),
		پوچ
	};

	argc = parse_options(argc, argv, prefix, module_clone_options,
			     git_submodule_helper_usage, 0);

	اگر (ref_storage_format) {
		clone_data.ref_storage_format = ref_storage_format_by_name(ref_storage_format);
		اگر (clone_data.ref_storage_format == REF_STORAGE_FORMAT_UNKNOWN)
			die(_("unknown ref storage format '%s'"), ref_storage_format);
	}
	clone_data.dissociate = !!dissociate;
	clone_data.quiet = !!quiet;
	clone_data.progress = !!progress;
	clone_data.require_init = !!require_init;
	clone_data.filter_options = &filter_options;

	اگر (argc || !clone_data.url || !clone_data.path || !*(clone_data.path))
		usage_with_options(git_submodule_helper_usage,
				   module_clone_options);

	clone_submodule(&clone_data, &reference);
	list_objects_filter_release(&filter_options);
	string_list_clear(&reference, 1);
	بازگشت 0;
}

ایستا صحیح determine_submodule_update_strategy(ساختار repository *r,
					       صحیح just_cloned,
					       ثابت نویسه *path,
					       شمارش submodule_update_type update,
					       ساختار submodule_update_strategy *out)
{
	ثابت ساختار submodule *sub = submodule_from_path(r, null_oid(the_hash_algo), path);
	نویسه *کلید;
	ثابت نویسه *val;
	صحیح ret;

	/*
	 * NEEDSWORK: audit and ensure that update_submodule() has right
	 * to assume that submodule_from_path() above will always succeed.
	 */
	اگر (!sub)
		BUG("update_submodule assumes a submodule exists at path (%s)",
		    path);
	کلید = xstrfmt("submodule.%s.update", sub->name);

	اگر (update) {
		out->type = update;
	} وگرنه اگر (!repo_config_get_string_tmp(r, کلید, &val)) {
		اگر (parse_submodule_update_strategy(val, out) < 0) {
			ret = die_message(_("Invalid update mode '%s' configured for submodule path '%s'"),
					  val, path);
			برو cleanup;
		}
	} وگرنه اگر (sub->update_strategy.type != SM_UPDATE_UNSPECIFIED) {
		اگر (sub->update_strategy.type == SM_UPDATE_COMMAND)
			BUG("how did we read update = !command from .gitmodules?");
		out->type = sub->update_strategy.type;
		out->command = sub->update_strategy.command;
	} وگرنه
		out->type = SM_UPDATE_CHECKOUT;

	اگر (just_cloned &&
	    (out->type == SM_UPDATE_MERGE ||
	     out->type == SM_UPDATE_REBASE ||
	     out->type == SM_UPDATE_NONE))
		out->type = SM_UPDATE_CHECKOUT;

	ret = 0;
cleanup:
	آزاد(کلید);
	بازگشت ret;
}

ساختار update_clone_data {
	ثابت ساختار submodule *sub;
	ساختار object_id oid;
	unsigned just_cloned;
};

ساختار submodule_update_clone {
	/* index into 'update_data.list', the list of submodules to look into for cloning */
	صحیح current;

	/* configuration parameters which are passed on to the children */
	ثابت ساختار update_data *update_data;

	/* to be consumed by update_submodule() */
	ساختار update_clone_data *update_clone;
	صحیح update_clone_nr; صحیح update_clone_alloc;

	/* If we want to stop as fast as possible and return an error */
	unsigned quickstop : 1;

	/* failed clones to be retried again */
	ثابت ساختار cache_entry **failed_clones;
	صحیح failed_clones_nr, failed_clones_alloc;
};
#define SUBMODULE_UPDATE_CLONE_INIT { 0 }

ایستا تهی submodule_update_clone_release(ساختار submodule_update_clone *suc)
{
	آزاد(suc->update_clone);
	آزاد(suc->failed_clones);
}

ساختار update_data {
	ثابت نویسه *prefix;
	ثابت نویسه *super_prefix;
	نویسه *displaypath;
	شمارش submodule_update_type update_default;
	ساختار object_id suboid;
	ساختار string_list references;
	ساختار submodule_update_strategy update_strategy;
	ساختار list_objects_filter_options *filter_options;
	ساختار module_list list;
	شمارش ref_storage_format ref_storage_format;
	صحیح depth;
	صحیح max_jobs;
	صحیح single_branch;
	صحیح recommend_shallow;
	unsigned صحیح require_init;
	unsigned صحیح force;
	unsigned صحیح quiet;
	unsigned صحیح nofetch;
	unsigned صحیح remote;
	unsigned صحیح progress;
	unsigned صحیح dissociate;
	unsigned صحیح init;
	unsigned صحیح warn_if_uninitialized;
	unsigned صحیح recursive;

	/* copied over from update_clone_data */
	ساختار object_id oid;
	unsigned صحیح just_cloned;
	ثابت نویسه *sm_path;
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

ایستا تهی update_data_release(ساختار update_data *ud)
{
	آزاد(ud->displaypath);
	submodule_update_strategy_release(&ud->update_strategy);
	module_list_release(&ud->list);
}

ایستا تهی next_submodule_warn_missing(ساختار submodule_update_clone *suc,
		ساختار strbuf *out, ثابت نویسه *displaypath)
{
	/*
	 * Only mention uninitialized submodules when their
	 * paths have been specified.
	 */
	اگر (suc->update_data->warn_if_uninitialized) {
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
ایستا صحیح prepare_to_clone_next_submodule(ثابت ساختار cache_entry *ce,
					   ساختار child_process *child,
					   ساختار submodule_update_clone *suc,
					   ساختار strbuf *out)
{
	ثابت ساختار submodule *sub = پوچ;
	ثابت نویسه *url = پوچ;
	ثابت نویسه *update_string;
	شمارش submodule_update_type update_type;
	نویسه *کلید;
	ثابت ساختار update_data *ud = suc->update_data;
	نویسه *displaypath = get_submodule_displaypath(ce->name, ud->prefix,
						      ud->super_prefix);
	ساختار strbuf sb = STRBUF_INIT;
	صحیح needs_cloning = 0;
	صحیح need_free_url = 0;

	اگر (ce_stage(ce)) {
		strbuf_addf(out, _("Skipping unmerged submodule %s"), displaypath);
		strbuf_addch(out, '\n');
		برو cleanup;
	}

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), ce->name);

	اگر (!sub) {
		next_submodule_warn_missing(suc, out, displaypath);
		برو cleanup;
	}

	کلید = xstrfmt("submodule.%s.update", sub->name);
	اگر (!repo_config_get_string_tmp(the_repository, کلید, &update_string)) {
		update_type = parse_submodule_update_type(update_string);
	} وگرنه {
		update_type = sub->update_strategy.type;
	}
	آزاد(کلید);

	اگر (suc->update_data->update_strategy.type == SM_UPDATE_NONE
	    || (suc->update_data->update_strategy.type == SM_UPDATE_UNSPECIFIED
		&& update_type == SM_UPDATE_NONE)) {
		strbuf_addf(out, _("Skipping submodule '%s'"), displaypath);
		strbuf_addch(out, '\n');
		برو cleanup;
	}

	/* Check if the submodule has been initialized. */
	اگر (!is_submodule_active(the_repository, ce->name)) {
		next_submodule_warn_missing(suc, out, displaypath);
		برو cleanup;
	}

	strbuf_reset(&sb);
	strbuf_addf(&sb, "submodule.%s.url", sub->name);
	اگر (repo_config_get_string_tmp(the_repository, sb.buf, &url)) {
		اگر (sub->url && (starts_with_dot_slash(sub->url) ||
				 starts_with_dot_dot_slash(sub->url))) {
			url = resolve_relative_url(sub->url, پوچ, 0);
			need_free_url = 1;
		} وگرنه
			url = sub->url;
	}

	اگر (!url)
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

	اگر (!needs_cloning)
		برو cleanup;

	child->git_cmd = 1;
	child->no_stdin = 1;
	child->stdout_to_stderr = 1;
	child->err = -1;
	strvec_افزودن(&child->args, "submodule--helper");
	strvec_افزودن(&child->args, "clone");
	اگر (suc->update_data->progress)
		strvec_افزودن(&child->args, "--progress");
	اگر (suc->update_data->quiet)
		strvec_افزودن(&child->args, "--quiet");
	اگر (suc->update_data->prefix)
		strvec_pushl(&child->args, "--prefix", suc->update_data->prefix, پوچ);
	اگر (suc->update_data->recommend_shallow && sub->recommend_shallow == 1)
		strvec_افزودن(&child->args, "--depth=1");
	وگرنه اگر (suc->update_data->depth)
		strvec_pushf(&child->args, "--depth=%d", suc->update_data->depth);
	اگر (suc->update_data->filter_options && suc->update_data->filter_options->choice)
		strvec_pushf(&child->args, "--filter=%s",
			     expand_list_objects_filter_spec(suc->update_data->filter_options));
	اگر (suc->update_data->require_init)
		strvec_افزودن(&child->args, "--require-init");
	اگر (suc->update_data->ref_storage_format != REF_STORAGE_FORMAT_UNKNOWN)
		strvec_pushf(&child->args, "--ref-storage-format=%s",
			     ref_storage_format_to_name(suc->update_data->ref_storage_format));
	strvec_pushl(&child->args, "--path", sub->path, پوچ);
	strvec_pushl(&child->args, "--name", sub->name, پوچ);
	strvec_pushl(&child->args, "--url", url, پوچ);
	اگر (suc->update_data->references.nr) {
		ساختار string_list_item *item;

		for_each_string_list_item(item, &suc->update_data->references)
			strvec_pushl(&child->args, "--reference", item->string, پوچ);
	}
	اگر (suc->update_data->dissociate)
		strvec_افزودن(&child->args, "--dissociate");
	اگر (suc->update_data->single_branch >= 0)
		strvec_افزودن(&child->args, suc->update_data->single_branch ?
					      "--single-branch" :
					      "--no-single-branch");

cleanup:
	آزاد(displaypath);
	strbuf_release(&sb);
	اگر (need_free_url)
		آزاد((تهی*)url);

	بازگشت needs_cloning;
}

ایستا صحیح update_clone_get_next_task(ساختار child_process *child,
				      ساختار strbuf *err,
				      تهی *suc_cb,
				      تهی **idx_task_cb)
{
	ساختار submodule_update_clone *suc = suc_cb;
	ثابت ساختار cache_entry *ce;
	صحیح index;

	برای (; suc->current < suc->update_data->list.nr; suc->current++) {
		ce = suc->update_data->list.entries[suc->current];
		اگر (prepare_to_clone_next_submodule(ce, child, suc, err)) {
			صحیح *p = xmalloc(اندازه(*p));

			*p = suc->current;
			*idx_task_cb = p;
			suc->current++;
			بازگشت 1;
		}
	}

	/*
	 * The loop above tried cloning each submodule once, now try the
	 * stragglers again, which we can imagine as an extension of the
	 * entry list.
	 */
	index = suc->current - suc->update_data->list.nr;
	اگر (index < suc->failed_clones_nr) {
		صحیح *p;

		ce = suc->failed_clones[index];
		اگر (!prepare_to_clone_next_submodule(ce, child, suc, err)) {
			suc->current ++;
			strbuf_addstr(err, "BUG: submodule considered for "
					   "cloning, doesn't need cloning "
					   "any more?\n");
			بازگشت 0;
		}
		p = xmalloc(اندازه(*p));
		*p = suc->current;
		*idx_task_cb = p;
		suc->current ++;
		بازگشت 1;
	}

	بازگشت 0;
}

ایستا صحیح update_clone_start_failure(ساختار strbuf *err UNUSED,
				      تهی *suc_cb,
				      تهی *idx_task_cb UNUSED)
{
	ساختار submodule_update_clone *suc = suc_cb;

	suc->quickstop = 1;
	بازگشت 1;
}

ایستا صحیح update_clone_task_finished(صحیح result,
				      ساختار strbuf *err,
				      تهی *suc_cb,
				      تهی *idx_task_cb)
{
	ثابت ساختار cache_entry *ce;
	ساختار submodule_update_clone *suc = suc_cb;
	صحیح *idxP = idx_task_cb;
	صحیح idx = *idxP;

	آزاد(idxP);

	اگر (!result)
		بازگشت 0;

	اگر (idx < suc->update_data->list.nr) {
		ce  = suc->update_data->list.entries[idx];
		strbuf_addf(err, _("Failed to clone '%s'. Retry scheduled"),
			    ce->name);
		strbuf_addch(err, '\n');
		ALLOC_GROW(suc->failed_clones,
			   suc->failed_clones_nr + 1,
			   suc->failed_clones_alloc);
		suc->failed_clones[suc->failed_clones_nr++] = ce;
		بازگشت 0;
	} وگرنه {
		idx -= suc->update_data->list.nr;
		ce  = suc->failed_clones[idx];
		strbuf_addf(err, _("Failed to clone '%s' a second time, aborting"),
			    ce->name);
		strbuf_addch(err, '\n');
		suc->quickstop = 1;
		بازگشت 1;
	}

	بازگشت 0;
}

ایستا صحیح git_update_clone_config(ثابت نویسه *var, ثابت نویسه *مقدار,
				   ثابت ساختار config_context *ctx,
				   تهی *cb)
{
	صحیح *max_jobs = cb;

	اگر (!strcmp(var, "submodule.fetchjobs"))
		*max_jobs = parse_submodule_fetchjobs(var, مقدار, ctx->kvi);
	بازگشت 0;
}

ایستا صحیح is_tip_reachable(ثابت نویسه *path, ثابت ساختار object_id *oid)
{
	ساختار child_process cp = CHILD_PROCESS_INIT;
	ساختار strbuf rev = STRBUF_INIT;
	نویسه *hex = oid_to_hex(oid);
	صحیح reachable;

	cp.git_cmd = 1;
	cp.dir = path;
	cp.no_stderr = 1;
	strvec_pushl(&cp.args, "rev-list", "-n", "1", hex, "--not", "--all", پوچ);

	prepare_submodule_repo_env(&cp.env);

	اگر (capture_command(&cp, &rev, GIT_MAX_HEXSZ + 1) || rev.len)
		reachable = 0;
	وگرنه
		reachable = 1;

	strbuf_release(&rev);
	بازگشت reachable;
}

ایستا صحیح fetch_in_submodule(ثابت نویسه *module_path, صحیح depth, صحیح quiet,
			      ثابت ساختار object_id *oid)
{
	ساختار child_process cp = CHILD_PROCESS_INIT;

	prepare_submodule_repo_env(&cp.env);
	cp.git_cmd = 1;
	cp.dir = module_path;

	strvec_افزودن(&cp.args, "fetch");
	اگر (quiet)
		strvec_افزودن(&cp.args, "--quiet");
	اگر (depth)
		strvec_pushf(&cp.args, "--depth=%d", depth);
	اگر (oid) {
		نویسه *hex = oid_to_hex(oid);
		نویسه *remote;
		صحیح code;

		code = get_default_remote_submodule(module_path, &remote);
		اگر (code) {
			child_process_clear(&cp);
			بازگشت code;
		}

		strvec_pushl(&cp.args, remote, hex, پوچ);
		آزاد(remote);
	}

	بازگشت run_command(&cp);
}

ایستا صحیح run_update_command(ثابت ساختار update_data *ud, صحیح subforce)
{
	ساختار child_process cp = CHILD_PROCESS_INIT;
	نویسه *oid = oid_to_hex(&ud->oid);
	صحیح ret;

	انتخاب (ud->update_strategy.type) {
	حالت SM_UPDATE_CHECKOUT:
		cp.git_cmd = 1;
		strvec_pushl(&cp.args, "checkout", "-q", پوچ);
		اگر (subforce)
			strvec_افزودن(&cp.args, "-f");
		بشکن;
	حالت SM_UPDATE_REBASE:
		cp.git_cmd = 1;
		strvec_افزودن(&cp.args, "rebase");
		اگر (ud->quiet)
			strvec_افزودن(&cp.args, "--quiet");
		بشکن;
	حالت SM_UPDATE_MERGE:
		cp.git_cmd = 1;
		strvec_افزودن(&cp.args, "merge");
		اگر (ud->quiet)
			strvec_افزودن(&cp.args, "--quiet");
		بشکن;
	حالت SM_UPDATE_COMMAND:
		cp.use_shell = 1;
		strvec_افزودن(&cp.args, ud->update_strategy.command);
		بشکن;
	پیشفرض:
		BUG("unexpected update strategy type: %d",
		    ud->update_strategy.type);
	}
	strvec_افزودن(&cp.args, oid);

	cp.dir = ud->sm_path;
	prepare_submodule_repo_env(&cp.env);
	اگر ((ret = run_command(&cp))) {
		انتخاب (ud->update_strategy.type) {
		حالت SM_UPDATE_CHECKOUT:
			die_message(_("Unable to checkout '%s' in submodule path '%s'"),
				    oid, ud->displaypath);
			/* No "ret" assignment, use "git checkout"'s */
			بشکن;
		حالت SM_UPDATE_REBASE:
			ret = die_message(_("Unable to rebase '%s' in submodule path '%s'"),
					  oid, ud->displaypath);
			بشکن;
		حالت SM_UPDATE_MERGE:
			ret = die_message(_("Unable to merge '%s' in submodule path '%s'"),
					  oid, ud->displaypath);
			بشکن;
		حالت SM_UPDATE_COMMAND:
			ret = die_message(_("Execution of '%s %s' failed in submodule path '%s'"),
					  ud->update_strategy.command, oid, ud->displaypath);
			بشکن;
		پیشفرض:
			BUG("unexpected update strategy type: %d",
			    ud->update_strategy.type);
		}

		بازگشت ret;
	}

	اگر (ud->quiet)
		بازگشت 0;

	انتخاب (ud->update_strategy.type) {
	حالت SM_UPDATE_CHECKOUT:
		printf(_("Submodule path '%s': checked out '%s'\n"),
		       ud->displaypath, oid);
		بشکن;
	حالت SM_UPDATE_REBASE:
		printf(_("Submodule path '%s': rebased into '%s'\n"),
		       ud->displaypath, oid);
		بشکن;
	حالت SM_UPDATE_MERGE:
		printf(_("Submodule path '%s': merged in '%s'\n"),
		       ud->displaypath, oid);
		بشکن;
	حالت SM_UPDATE_COMMAND:
		printf(_("Submodule path '%s': '%s %s'\n"),
		       ud->displaypath, ud->update_strategy.command, oid);
		بشکن;
	پیشفرض:
		BUG("unexpected update strategy type: %d",
		    ud->update_strategy.type);
	}

	بازگشت 0;
}

ایستا صحیح run_update_procedure(ثابت ساختار update_data *ud)
{
	صحیح subforce = is_null_oid(&ud->suboid) || ud->force;

	اگر (!ud->nofetch) {
		/*
		 * Run fetch only if `oid` isn't present or it
		 * is not reachable from a ref.
		 */
		اگر (!is_tip_reachable(ud->sm_path, &ud->oid) &&
		    fetch_in_submodule(ud->sm_path, ud->depth, ud->quiet, پوچ) &&
		    !ud->quiet)
			fprintf_ln(stderr,
				   _("Unable to fetch in submodule path '%s'; "
				     "trying to directly fetch %s:"),
				   ud->displaypath, oid_to_hex(&ud->oid));
		/*
		 * Now we tried the usual fetch, but `oid` may
		 * not be reachable from any of the refs.
		 */
		اگر (!is_tip_reachable(ud->sm_path, &ud->oid) &&
		    fetch_in_submodule(ud->sm_path, ud->depth, ud->quiet, &ud->oid))
			بازگشت die_message(_("Fetched in submodule path '%s', but it did not "
					     "contain %s. Direct fetching of that commit failed."),
					   ud->displaypath, oid_to_hex(&ud->oid));
	}

	بازگشت run_update_command(ud, subforce);
}

ایستا صحیح remote_submodule_branch(ثابت نویسه *path, ثابت نویسه **branch)
{
	ثابت ساختار submodule *sub;
	نویسه *کلید;
	*branch = پوچ;

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);
	اگر (!sub)
		بازگشت die_message(_("could not initialize submodule at path '%s'"),
				   path);

	کلید = xstrfmt("submodule.%s.branch", sub->name);
	اگر (repo_config_get_string_tmp(the_repository, کلید, branch))
		*branch = sub->branch;
	آزاد(کلید);

	اگر (!*branch) {
		*branch = "HEAD";
		بازگشت 0;
	}

	اگر (!strcmp(*branch, ".")) {
		ثابت نویسه *refname = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
							      "HEAD", 0, پوچ,
							      پوچ);

		اگر (!refname)
			بازگشت die_message(_("No such ref: %s"), "HEAD");

		/* detached HEAD */
		اگر (!strcmp(refname, "HEAD"))
			بازگشت die_message(_("Submodule (%s) branch configured to inherit "
					     "branch from superproject, but the superproject "
					     "is not on any branch"), sub->name);

		اگر (!skip_prefix(refname, "refs/heads/", &refname))
			بازگشت die_message(_("Expecting a full ref name, got %s"),
					   refname);

		*branch = refname;
		بازگشت 0;
	}

	/* Our "branch" is coming from repo_config_get_string_tmp() */
	بازگشت 0;
}

ایستا صحیح ensure_core_worktree(ثابت نویسه *path)
{
	ثابت نویسه *cw;
	ساختار repository subrepo;

	اگر (repo_submodule_init(&subrepo, the_repository, path, null_oid(the_hash_algo)))
		بازگشت die_message(_("could not get a repository handle for submodule '%s'"),
				   path);

	اگر (!repo_config_get_string_tmp(&subrepo, "core.worktree", &cw)) {
		نویسه *cfg_file, *abs_path;
		ثابت نویسه *rel_path;
		ساختار strbuf sb = STRBUF_INIT;

		cfg_file = repo_git_path(&subrepo, "config");

		abs_path = absolute_pathdup(path);
		rel_path = relative_path(abs_path, subrepo.gitdir, &sb);

		repo_config_set_in_file(the_repository, cfg_file, "core.worktree", rel_path);

		آزاد(cfg_file);
		آزاد(abs_path);
		strbuf_release(&sb);
	}

	repo_clear(&subrepo);
	بازگشت 0;
}

ایستا ثابت نویسه *submodule_update_type_to_label(شمارش submodule_update_type type)
{
	انتخاب (type) {
	حالت SM_UPDATE_CHECKOUT:
		بازگشت "checkout";
	حالت SM_UPDATE_MERGE:
		بازگشت "merge";
	حالت SM_UPDATE_REBASE:
		بازگشت "rebase";
	حالت SM_UPDATE_UNSPECIFIED:
	حالت SM_UPDATE_NONE:
	حالت SM_UPDATE_COMMAND:
		بشکن;
	}
	BUG("unreachable with type %d", type);
}

ایستا تهی update_data_to_args(ثابت ساختار update_data *update_data,
				ساختار strvec *args)
{
	شمارش submodule_update_type update_type = update_data->update_default;

	strvec_pushl(args, "submodule--helper", "update", "--recursive", پوچ);
	اگر (update_data->displaypath)
		strvec_pushf(args, "--super-prefix=%s/",
			     update_data->displaypath);
	strvec_pushf(args, "--jobs=%d", update_data->max_jobs);
	اگر (update_data->quiet)
		strvec_افزودن(args, "--quiet");
	اگر (update_data->force)
		strvec_افزودن(args, "--force");
	اگر (update_data->init)
		strvec_افزودن(args, "--init");
	اگر (update_data->remote)
		strvec_افزودن(args, "--remote");
	اگر (update_data->nofetch)
		strvec_افزودن(args, "--no-fetch");
	اگر (update_data->dissociate)
		strvec_افزودن(args, "--dissociate");
	اگر (update_data->progress)
		strvec_افزودن(args, "--progress");
	اگر (update_data->require_init)
		strvec_افزودن(args, "--require-init");
	اگر (update_data->depth)
		strvec_pushf(args, "--depth=%d", update_data->depth);
	اگر (update_type != SM_UPDATE_UNSPECIFIED)
		strvec_pushf(args, "--%s",
			     submodule_update_type_to_label(update_type));

	اگر (update_data->references.nr) {
		ساختار string_list_item *item;

		for_each_string_list_item(item, &update_data->references)
			strvec_pushl(args, "--reference", item->string, پوچ);
	}
	اگر (update_data->ref_storage_format != REF_STORAGE_FORMAT_UNKNOWN)
		strvec_pushf(args, "--ref-storage-format=%s",
			     ref_storage_format_to_name(update_data->ref_storage_format));
	اگر (update_data->filter_options && update_data->filter_options->choice)
		strvec_pushf(args, "--filter=%s",
				expand_list_objects_filter_spec(
					update_data->filter_options));
	اگر (update_data->recommend_shallow == 0)
		strvec_افزودن(args, "--no-recommend-shallow");
	وگرنه اگر (update_data->recommend_shallow == 1)
		strvec_افزودن(args, "--recommend-shallow");
	اگر (update_data->single_branch >= 0)
		strvec_افزودن(args, update_data->single_branch ?
				    "--single-branch" :
				    "--no-single-branch");
}

ایستا صحیح update_submodule(ساختار update_data *update_data)
{
	صحیح ret;

	اگر (validate_submodule_path(update_data->sm_path) < 0)
		بازگشت -1;

	ret = determine_submodule_update_strategy(the_repository,
						  update_data->just_cloned,
						  update_data->sm_path,
						  update_data->update_default,
						  &update_data->update_strategy);
	اگر (ret)
		بازگشت ret;

	اگر (update_data->just_cloned)
		oidcpy(&update_data->suboid, null_oid(the_hash_algo));
	وگرنه اگر (repo_resolve_gitlink_ref(the_repository, update_data->sm_path,
					  "HEAD", &update_data->suboid))
		بازگشت die_message(_("Unable to find current revision in submodule path '%s'"),
				   update_data->displaypath);

	اگر (update_data->remote) {
		نویسه *remote_name;
		ثابت نویسه *branch;
		نویسه *remote_ref;
		صحیح code;

		code = get_default_remote_submodule(update_data->sm_path, &remote_name);
		اگر (code)
			بازگشت code;
		code = remote_submodule_branch(update_data->sm_path, &branch);
		اگر (code) {
			آزاد(remote_name);
			بازگشت code;
		}
		remote_ref = xstrfmt("refs/remotes/%s/%s", remote_name, branch);

		آزاد(remote_name);

		اگر (!update_data->nofetch) {
			اگر (fetch_in_submodule(update_data->sm_path, update_data->depth,
					      0, پوچ)) {
				آزاد(remote_ref);
				بازگشت die_message(_("Unable to fetch in submodule path '%s'"),
						   update_data->sm_path);
			}
		}

		اگر (repo_resolve_gitlink_ref(the_repository, update_data->sm_path,
					     remote_ref, &update_data->oid)) {
			ret = die_message(_("Unable to find %s revision in submodule path '%s'"),
					  remote_ref, update_data->sm_path);
			آزاد(remote_ref);
			بازگشت ret;
		}

		آزاد(remote_ref);
	}

	اگر (!oideq(&update_data->oid, &update_data->suboid) || update_data->force) {
		ret = run_update_procedure(update_data);
		اگر (ret)
			بازگشت ret;
	}

	اگر (update_data->recursive) {
		ساختار child_process cp = CHILD_PROCESS_INIT;
		ساختار update_data next = *update_data;

		next.prefix = پوچ;
		oidcpy(&next.oid, null_oid(the_hash_algo));
		oidcpy(&next.suboid, null_oid(the_hash_algo));

		cp.dir = update_data->sm_path;
		cp.git_cmd = 1;
		prepare_submodule_repo_env(&cp.env);
		update_data_to_args(&next, &cp.args);

		ret = run_command(&cp);
		اگر (ret)
			die_message(_("Failed to recurse into submodule path '%s'"),
				    update_data->displaypath);
		بازگشت ret;
	}

	بازگشت 0;
}

ایستا صحیح update_submodules(ساختار update_data *update_data)
{
	صحیح i, ret = 0;
	ساختار submodule_update_clone suc = SUBMODULE_UPDATE_CLONE_INIT;
	ثابت ساختار run_process_parallel_opts opts = {
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
	اگر (suc.quickstop) {
		ret = 1;
		برو cleanup;
	}

	برای (i = 0; i < suc.update_clone_nr; i++) {
		ساختار update_clone_data ucd = suc.update_clone[i];
		صحیح code = 128;

		oidcpy(&update_data->oid, &ucd.oid);
		update_data->just_cloned = ucd.just_cloned;
		update_data->sm_path = ucd.sub->path;

		/*
		 * Verify that the submodule path does not contain any
		 * symlinks; if it does, it might have been tampered with.
		 * TODO: allow exempting it via
		 * `safe.submodule.path` or something
		 */
		اگر (validate_submodule_path(update_data->sm_path) < 0)
			برو fail;

		code = ensure_core_worktree(update_data->sm_path);
		اگر (code)
			برو fail;

		update_data->displaypath = get_submodule_displaypath(
			update_data->sm_path, update_data->prefix,
			update_data->super_prefix);
		code = update_submodule(update_data);
		FREE_AND_NULL(update_data->displaypath);
fail:
		اگر (!code)
			ادامه;
		ret = code;
		اگر (ret == 128)
			برو cleanup;
	}

cleanup:
	submodule_update_clone_release(&suc);
	string_list_clear(&update_data->references, 0);
	بازگشت ret;
}

ایستا صحیح module_update(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix,
			 ساختار repository *repo UNUSED)
{
	ساختار pathspec pathspec = { 0 };
	ساختار pathspec pathspec2 = { 0 };
	ساختار update_data opt = UPDATE_DATA_INIT;
	ساختار list_objects_filter_options filter_options =
		LIST_OBJECTS_FILTER_INIT;
	ثابت نویسه *ref_storage_format = پوچ;
	صحیح ret;
	ساختار option module_update_options[] = {
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
	ثابت نویسه *ثابت git_submodule_helper_usage[] = {
		N_("git submodule [--quiet] update"
		" [--init [--filter=<filter-spec>]] [--remote]"
		" [-N|--no-fetch] [-f|--force]"
		" [--checkout|--merge|--rebase]"
		" [--[no-]recommend-shallow] [--reference <repository>]"
		" [--recursive] [--[no-]single-branch] [--] [<path>...]"),
		پوچ
	};

	update_clone_config_from_gitmodules(the_repository, &opt.max_jobs);
	repo_config(the_repository, git_update_clone_config, &opt.max_jobs);

	argc = parse_options(argc, argv, prefix, module_update_options,
			     git_submodule_helper_usage, 0);

	اگر (opt.require_init)
		opt.init = 1;

	اگر (filter_options.choice && !opt.init) {
		usage_with_options(git_submodule_helper_usage,
				   module_update_options);
	}

	اگر (ref_storage_format) {
		opt.ref_storage_format = ref_storage_format_by_name(ref_storage_format);
		اگر (opt.ref_storage_format == REF_STORAGE_FORMAT_UNKNOWN)
			die(_("unknown ref storage format '%s'"), ref_storage_format);
	}

	opt.filter_options = &filter_options;
	opt.prefix = prefix;

	اگر (opt.update_default)
		opt.update_strategy.type = opt.update_default;

	اگر (module_list_compute(argv, prefix, &pathspec, &opt.list) < 0) {
		ret = 1;
		برو cleanup;
	}

	اگر (pathspec.nr)
		opt.warn_if_uninitialized = 1;

	اگر (opt.init) {
		ساختار module_list list = MODULE_LIST_INIT;
		ساختار init_cb info = INIT_CB_INIT;

		اگر (module_list_compute(argv, opt.prefix,
					&pathspec2, &list) < 0) {
			module_list_release(&list);
			ret = 1;
			برو cleanup;
		}

		/*
		 * If there are no path args and submodule.active is set then,
		 * by default, only initialize 'active' modules.
		 */
		اگر (!argc && !repo_config_get(the_repository, "submodule.active"))
			module_list_active(&list);

		info.prefix = opt.prefix;
		info.super_prefix = opt.super_prefix;
		اگر (opt.quiet)
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
	بازگشت ret;
}

ایستا صحیح افزودن_check(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix UNUSED,
		      ساختار repository *repo UNUSED)
{
	ساختار remote *remote;
	ثابت نویسه *superproject_head;
	نویسه *head;
	صحیح detached_head = 0;
	ساختار object_id head_oid;

	اگر (argc < 3)
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
				   0, &head_oid, پوچ);
	اگر (!head)
		die(_("Failed to resolve HEAD as a valid ref."));
	اگر (!strcmp(head, "HEAD"))
		detached_head = 1;

	/*
	 * The remote must be configured.
	 * This is to avoid pushing to the exact same URL as the parent.
	 */
	remote = pushremote_get(argv[1]);
	اگر (!remote || remote->origin == REMOTE_UNCONFIGURED)
		die("remote '%s' not configured", argv[1]);

	/* Check the refspec */
	اگر (argc > 2) {
		صحیح i;
		ساختار ref *local_refs = get_local_heads();
		ساختار refspec refspec = REFSPEC_INIT_افزودن(the_hash_algo);

		refspec_appendn(&refspec, argv + 2, argc - 2);

		برای (i = 0; i < refspec.nr; i++) {
			ثابت ساختار refspec_item *rs = &refspec.اقلام[i];

			اگر (rs->pattern || rs->matching)
				ادامه;

			/* LHS must match a single ref */
			انتخاب (شمار_refspec_match(rs->src, local_refs, پوچ)) {
			حالت 1:
				بشکن;
			حالت 0:
				/*
				 * If LHS matches 'HEAD' then we need to ensure
				 * that it matches the same named branch
				 * checked out in the superproject.
				 */
				اگر (!strcmp(rs->src, "HEAD")) {
					اگر (!detached_head &&
					    !strcmp(head, superproject_head))
						بشکن;
					die("HEAD does not match the named branch in the superproject");
				}
				/* fallthrough */
			پیشفرض:
				die("src refspec '%s' must name a ref",
				    rs->src);
			}
		}

		refspec_clear(&refspec);
		free_refs(local_refs);
	}
	آزاد(head);

	بازگشت 0;
}

ایستا صحیح absorb_git_dirs(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix,
			   ساختار repository *repo UNUSED)
{
	صحیح i;
	ساختار pathspec pathspec = { 0 };
	ساختار module_list list = MODULE_LIST_INIT;
	ثابت نویسه *super_prefix = پوچ;
	ساختار option embed_gitdir_options[] = {
		OPT__SUPER_PREFIX(&super_prefix),
		OPT_END()
	};
	ثابت نویسه *ثابت git_submodule_helper_usage[] = {
		N_("git submodule absorbgitdirs [<options>] [<path>...]"),
		پوچ
	};
	صحیح ret = 1;

	argc = parse_options(argc, argv, prefix, embed_gitdir_options,
			     git_submodule_helper_usage, 0);

	اگر (module_list_compute(argv, prefix, &pathspec, &list) < 0)
		برو cleanup;

	برای (i = 0; i < list.nr; i++)
		absorb_git_dir_into_superproject(list.entries[i]->name,
						 super_prefix);

	ret = 0;
cleanup:
	clear_pathspec(&pathspec);
	module_list_release(&list);
	بازگشت ret;
}

ایستا صحیح module_set_url(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix,
			  ساختار repository *repo UNUSED)
{
	صحیح quiet = 0, ret;
	ثابت نویسه *newurl;
	ثابت نویسه *path;
	نویسه *config_name;
	ساختار option options[] = {
		OPT__QUIET(&quiet, N_("suppress output for setting url of a submodule")),
		OPT_END()
	};
	ثابت نویسه *ثابت usage[] = {
		N_("git submodule set-url [--quiet] <path> <newurl>"),
		پوچ
	};
	ثابت ساختار submodule *sub;

	argc = parse_options(argc, argv, prefix, options, usage, 0);

	اگر (argc != 2 || !(path = argv[0]) || !(newurl = argv[1]))
		usage_with_options(usage, options);

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);

	اگر (!sub)
		die(_("no submodule mapping found in .gitmodules for path '%s'"),
		    path);

	config_name = xstrfmt("submodule.%s.url", sub->name);
	ret = config_set_in_gitmodules_file_gently(the_repository, config_name, newurl);

	اگر (!ret) {
		repo_read_gitmodules(the_repository, 0);
		sync_submodule(sub->path, prefix, پوچ, quiet ? OPT_QUIET : 0);
	}

	آزاد(config_name);
	بازگشت !!ret;
}

ایستا صحیح module_set_branch(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix,
			     ساختار repository *repo UNUSED)
{
	صحیح opt_default = 0, ret;
	ثابت نویسه *opt_branch = پوچ;
	ثابت نویسه *path;
	نویسه *config_name;
	ساختار option options[] = {
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
	ثابت نویسه *ثابت usage[] = {
		N_("git submodule set-branch [-q|--quiet] (-d|--default) <path>"),
		N_("git submodule set-branch [-q|--quiet] (-b|--branch) <branch> <path>"),
		پوچ
	};
	ثابت ساختار submodule *sub;

	argc = parse_options(argc, argv, prefix, options, usage, 0);

	اگر (!opt_branch && !opt_default)
		die(_("--branch or --default required"));

	اگر (opt_branch && opt_default)
		die(_("options '%s' and '%s' cannot be used together"), "--branch", "--default");

	اگر (argc != 1 || !(path = argv[0]))
		usage_with_options(usage, options);

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);

	اگر (!sub)
		die(_("no submodule mapping found in .gitmodules for path '%s'"),
		    path);

	config_name = xstrfmt("submodule.%s.branch", sub->name);
	ret = config_set_in_gitmodules_file_gently(the_repository, config_name, opt_branch);

	آزاد(config_name);
	بازگشت !!ret;
}

ایستا صحیح module_create_branch(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix,
				ساختار repository *repo UNUSED)
{
	شمارش branch_track track;
	صحیح quiet = 0, force = 0, reflog = 0, dry_run = 0;
	ساختار option options[] = {
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
	ثابت نویسه *ثابت usage[] = {
		N_("git submodule--helper create-branch [-f|--force] [--create-reflog] [-q|--quiet] [-t|--track] [-n|--dry-run] <name> <start-oid> <start-name>"),
		پوچ
	};
	ساختار repo_config_مقادیر *cfg = repo_config_مقادیر(the_repository);

	repo_config(the_repository, git_default_config, پوچ);
	track = cfg->branch_track;
	argc = parse_options(argc, argv, prefix, options, usage, 0);

	اگر (argc != 3)
		usage_with_options(usage, options);

	اگر (!quiet && !dry_run)
		printf_ln(_("creating branch '%s'"), argv[0]);

	create_branches_recursively(the_repository, argv[0], argv[1], argv[2],
				    force, reflog, quiet, track, dry_run);
	بازگشت 0;
}

ساختار add_data {
	ثابت نویسه *prefix;
	ثابت نویسه *branch;
	ثابت نویسه *reference_path;
	نویسه *sm_path;
	ثابت نویسه *sm_name;
	ثابت نویسه *repo;
	ثابت نویسه *realrepo;
	شمارش ref_storage_format ref_storage_format;
	صحیح depth;
	unsigned صحیح force: 1;
	unsigned صحیح quiet: 1;
	unsigned صحیح progress: 1;
	unsigned صحیح dissociate: 1;
};
#define ADD_DATA_INIT { \
	.depth = -1, \
	.ref_storage_format = REF_STORAGE_FORMAT_UNKNOWN, \
}

ایستا تهی append_fetch_remotes(ساختار strbuf *پیام, ثابت نویسه *git_dir_path)
{
	ساختار child_process cp_remote = CHILD_PROCESS_INIT;
	ساختار strbuf sb_remote_out = STRBUF_INIT;

	cp_remote.git_cmd = 1;
	strvec_pushf(&cp_remote.env,
		     "GIT_DIR=%s", git_dir_path);
	strvec_افزودن(&cp_remote.env, "GIT_WORK_TREE=.");
	strvec_pushl(&cp_remote.args, "remote", "-v", پوچ);
	اگر (!capture_command(&cp_remote, &sb_remote_out, 0)) {
		نویسه *next_سطر;
		نویسه *سطر = sb_remote_out.buf;

		وقتی ((next_سطر = strchr(سطر, '\n')) != پوچ) {
			size_t len = next_سطر - سطر;

			اگر (strip_suffix_mem(سطر, &len, " (fetch)"))
				strbuf_addf(پیام, "  %.*s\n", (صحیح)len, سطر);
			سطر = next_سطر + 1;
		}
	}

	strbuf_release(&sb_remote_out);
}

ایستا صحیح add_submodule(ثابت ساختار add_data *add_data)
{
	ساختار module_clone_data clone_data = MODULE_CLONE_DATA_INIT;
	ساختار string_list reference = STRING_LIST_INIT_NODUP;
	صحیح ret = -1;

	/* perhaps the path already exists and is already a git repo, else clone it */
	اگر (is_directory(add_data->sm_path)) {
		نویسه *submod_gitdir_path;
		ساختار strbuf sm_path = STRBUF_INIT;
		strbuf_addstr(&sm_path, add_data->sm_path);
		submod_gitdir_path = xstrfmt("%s/.git", add_data->sm_path);
		اگر (is_nonbare_repository_dir(&sm_path))
			printf(_("Adding existing repo at '%s' to the index\n"),
			       add_data->sm_path);
		وگرنه
			die(_("'%s' already exists and is not a valid git repo"),
			    add_data->sm_path);
		strbuf_release(&sm_path);
		آزاد(submod_gitdir_path);
	} وگرنه {
		ساختار child_process cp = CHILD_PROCESS_INIT;
		ساختار strbuf submod_gitdir = STRBUF_INIT;

		submodule_name_to_gitdir(&submod_gitdir, the_repository, add_data->sm_name);

		اگر (is_directory(submod_gitdir.buf)) {
			اگر (!add_data->force) {
				ساختار strbuf پیام = STRBUF_INIT;
				نویسه *die_پیام;

				strbuf_addf(&پیام, _("A git directory for '%s' is found "
						    "locally with remote(s):\n"),
					    add_data->sm_name);

				append_fetch_remotes(&پیام, submod_gitdir.buf);
				strbuf_release(&submod_gitdir);

				strbuf_addf(&پیام, _("If you want to reuse this local git "
						    "directory instead of cloning again from\n"
						    "  %s\n"
						    "use the '--force' option. If the local git "
						    "directory is not the correct repo\n"
						    "or you are unsure what this means choose "
						    "another name with the '--name' option."),
					    add_data->realrepo);

				die_پیام = strbuf_detach(&پیام, پوچ);
				die("%s", die_پیام);
			} وگرنه {
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
		اگر (add_data->reference_path) {
			نویسه *p = xstrdup(add_data->reference_path);

			string_list_append(&reference, p)->util = p;
		}
		clone_data.ref_storage_format = add_data->ref_storage_format;
		clone_data.dissociate = add_data->dissociate;
		اگر (add_data->depth >= 0)
			clone_data.depth = add_data->depth;

		اگر (clone_submodule(&clone_data, &reference))
			برو cleanup;

		prepare_submodule_repo_env(&cp.env);
		cp.git_cmd = 1;
		cp.dir = add_data->sm_path;
		/*
		 * NOTE: we only get here if add_data->force is true, so
		 * passing --force to checkout is reasonable.
		 */
		strvec_pushl(&cp.args, "checkout", "-f", "-q", پوچ);

		اگر (add_data->branch) {
			strvec_pushl(&cp.args, "-B", add_data->branch, پوچ);
			strvec_pushf(&cp.args, "origin/%s", add_data->branch);
		}

		اگر (run_command(&cp))
			die(_("unable to checkout submodule '%s'"), add_data->sm_path);
	}
	ret = 0;

cleanup:
	string_list_clear(&reference, 1);
	بازگشت ret;
}

ایستا صحیح config_submodule_in_gitmodules(ثابت نویسه *name, ثابت نویسه *var, ثابت نویسه *مقدار)
{
	نویسه *کلید;
	صحیح ret;

	اگر (!is_writing_gitmodules_ok())
		die(_("please make sure that the .gitmodules file is in the working tree"));

	کلید = xstrfmt("submodule.%s.%s", name, var);
	ret = config_set_in_gitmodules_file_gently(the_repository, کلید, مقدار);
	آزاد(کلید);

	بازگشت ret;
}

ایستا تهی configure_added_submodule(ساختار add_data *add_data)
{
	نویسه *کلید;
	ساختار child_process add_submod = CHILD_PROCESS_INIT;
	ساختار child_process add_gitmodules = CHILD_PROCESS_INIT;
	ثابت ساختار string_list *مقادیر;
	صحیح matched = 0;

	کلید = xstrfmt("submodule.%s.url", add_data->sm_name);
	repo_config_set_gently(the_repository, کلید, add_data->realrepo);
	آزاد(کلید);

	add_submod.git_cmd = 1;
	strvec_pushl(&add_submod.args, "add",
		     "--no-warn-embedded-repo", پوچ);
	اگر (add_data->force)
		strvec_افزودن(&add_submod.args, "--force");
	strvec_pushl(&add_submod.args, "--", add_data->sm_path, پوچ);

	اگر (run_command(&add_submod))
		die(_("Failed to add submodule '%s'"), add_data->sm_path);

	اگر (config_submodule_in_gitmodules(add_data->sm_name, "path", add_data->sm_path) ||
	    config_submodule_in_gitmodules(add_data->sm_name, "url", add_data->repo))
		die(_("Failed to register submodule '%s'"), add_data->sm_path);

	اگر (add_data->branch) {
		اگر (config_submodule_in_gitmodules(add_data->sm_name,
						   "branch", add_data->branch))
			die(_("Failed to register submodule '%s'"), add_data->sm_path);
	}

	add_gitmodules.git_cmd = 1;
	strvec_pushl(&add_gitmodules.args,
		     "add", "--force", "--", ".gitmodules", پوچ);

	اگر (run_command(&add_gitmodules))
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
	اگر (repo_config_get(the_repository, "submodule.active") || /* key absent */
	    repo_config_get_string_multi(the_repository, "submodule.active", &مقادیر)) {
		/*
		 * If the submodule being added isn't already covered by the
		 * current configured pathspec, set the submodule's active flag
		 */
		کلید = xstrfmt("submodule.%s.active", add_data->sm_name);
		repo_config_set_gently(the_repository, کلید, "true");
		آزاد(کلید);
	} وگرنه {
		برای (size_t i = 0; i < مقادیر->nr; i++) {
			ثابت نویسه *pat = مقادیر->اقلام[i].string;
			اگر (!wildmatch(pat, add_data->sm_path, 0)) { /* match found */
				matched = 1;
				بشکن;
			}
		}
		اگر (!matched) { /* no pattern matched -> force-enable */
			کلید = xstrfmt("submodule.%s.active", add_data->sm_name);
			repo_config_set_gently(the_repository, کلید, "true");
			آزاد(کلید);
		}
	}
}

ایستا تهی die_on_index_match(ثابت نویسه *path, صحیح force)
{
	ساختار pathspec ps;
	ثابت نویسه *args[] = { path, پوچ };
	parse_pathspec(&ps, 0, PATHSPEC_PREFER_CWD, پوچ, args);

	اگر (repo_read_index_preload(the_repository, پوچ, 0) < 0)
		die(_("index file corrupt"));

	اگر (ps.nr) {
		نویسه *ps_matched = xcalloc(ps.nr, 1);

		/* TODO: audit for interaction with sparse-index. */
		ensure_full_index(the_repository->index);

		/*
		 * Since there is only one pathspec, we just need to
		 * check ps_matched[0] to know if a cache entry matched.
		 */
		برای (size_t i = 0; i < the_repository->index->cache_nr; i++) {
			ce_path_match(the_repository->index, the_repository->index->cache[i], &ps,
				      ps_matched);

			اگر (ps_matched[0]) {
				اگر (!force)
					die(_("'%s' already exists in the index"),
					    path);
				اگر (!S_ISGITLINK(the_repository->index->cache[i]->ce_mode))
					die(_("'%s' already exists in the index "
					      "and is not a submodule"), path);
				بشکن;
			}
		}
		آزاد(ps_matched);
	}
	clear_pathspec(&ps);
}

ایستا تهی die_on_repo_without_commits(ثابت نویسه *path)
{
	ساختار strbuf sb = STRBUF_INIT;
	strbuf_addstr(&sb, path);
	اگر (is_nonbare_repository_dir(&sb)) {
		ساختار object_id oid;
		اگر (repo_resolve_gitlink_ref(the_repository, path, "HEAD", &oid) < 0)
			die(_("'%s' does not have a commit checked out"), path);
	}
	strbuf_release(&sb);
}

ایستا صحیح module_add(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix,
		      ساختار repository *repo UNUSED)
{
	صحیح force = 0, quiet = 0, progress = 0, dissociate = 0;
	ساختار add_data add_data = ADD_DATA_INIT;
	ثابت نویسه *ref_storage_format = پوچ;
	نویسه *to_free = پوچ;
	ثابت ساختار submodule *existing;
	ساختار strbuf buf = STRBUF_INIT;
	نویسه *sm_name_to_free = پوچ;
	ساختار option options[] = {
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
	ثابت نویسه *ثابت usage[] = {
		N_("git submodule add [<options>] [--] <repository> [<path>]"),
		پوچ
	};
	ساختار strbuf sb = STRBUF_INIT;
	صحیح ret = 1;

	argc = parse_options(argc, argv, prefix, options, usage, 0);

	اگر (!is_writing_gitmodules_ok())
		die(_("please make sure that the .gitmodules file is in the working tree"));

	اگر (prefix && *prefix &&
	    add_data.reference_path && !is_absolute_path(add_data.reference_path))
		add_data.reference_path = xstrfmt("%s%s", prefix, add_data.reference_path);

	اگر (argc == 0 || argc > 2)
		usage_with_options(usage, options);

	اگر (ref_storage_format) {
		add_data.ref_storage_format = ref_storage_format_by_name(ref_storage_format);
		اگر (add_data.ref_storage_format == REF_STORAGE_FORMAT_UNKNOWN)
			die(_("unknown ref storage format '%s'"), ref_storage_format);
	}

	add_data.repo = argv[0];
	اگر (argc == 1)
		add_data.sm_path = git_url_basename(add_data.repo, 0, 0);
	وگرنه
		add_data.sm_path = xstrdup(argv[1]);

	اگر (prefix && *prefix && !is_absolute_path(add_data.sm_path)) {
		نویسه *sm_path = add_data.sm_path;

		add_data.sm_path = xstrfmt("%s%s", prefix, sm_path);
		آزاد(sm_path);
	}

	اگر (starts_with_dot_dot_slash(add_data.repo) ||
	    starts_with_dot_slash(add_data.repo)) {
		اگر (prefix)
			die(_("Relative path can only be used from the toplevel "
			      "of the working tree"));

		/* dereference source url relative to parent's url */
		to_free = resolve_relative_url(add_data.repo, پوچ, 1);
		add_data.realrepo = to_free;
	} وگرنه اگر (is_dir_sep(add_data.repo[0]) || strchr(add_data.repo, ':')) {
		add_data.realrepo = add_data.repo;
	} وگرنه {
		die(_("repo URL: '%s' must be absolute or begin with ./|../"),
		    add_data.repo);
	}

	/*
	 * normalize path:
	 * multiple //; leading ./; /./; /../;
	 */
	normalize_path_copy(add_data.sm_path, add_data.sm_path);
	strip_dir_trailing_slashes(add_data.sm_path);

	اگر (validate_submodule_path(add_data.sm_path) < 0)
		die(پوچ);

	die_on_index_match(add_data.sm_path, force);
	die_on_repo_without_commits(add_data.sm_path);

	اگر (!force) {
		ساختار child_process cp = CHILD_PROCESS_INIT;

		cp.git_cmd = 1;
		cp.no_stdout = 1;
		strvec_pushl(&cp.args, "add", "--dry-run", "--ignore-missing",
			     "--no-warn-embedded-repo", add_data.sm_path, پوچ);
		اگر ((ret = pipe_command(&cp, پوچ, 0, پوچ, 0, &sb, 0))) {
			strbuf_complete_سطر(&sb);
			fputs(sb.buf, stderr);
			برو cleanup;
		}
	}

	اگر (!add_data.sm_name)
		add_data.sm_name = add_data.sm_path;

	existing = submodule_from_name(the_repository,
					null_oid(the_hash_algo),
					add_data.sm_name);

	اگر (existing && existing->path &&
	    strcmp(existing->path, add_data.sm_path)) {
		اگر (!force) {
			die(_("submodule name '%s' already used for path '%s'"),
			    add_data.sm_name, existing->path);
		}
		/* --force: build <name><n> until unique */
		برای (صحیح i = 1; ; i++) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "%s%d", add_data.sm_name, i);
			اگر (!submodule_from_name(the_repository,
						null_oid(the_hash_algo),
						buf.buf)) {
				بشکن;
			}
		}
		add_data.sm_name = sm_name_to_free = strbuf_detach(&buf, پوچ);
	}

	اگر (check_submodule_name(add_data.sm_name))
		die(_("'%s' is not a valid submodule name"), add_data.sm_name);

	add_data.prefix = prefix;
	add_data.force = !!force;
	add_data.quiet = !!quiet;
	add_data.progress = !!progress;
	add_data.dissociate = !!dissociate;

	اگر (the_repository->repository_format_submodule_path_cfg)
		create_default_gitdir_config(add_data.sm_name);

	اگر (add_submodule(&add_data))
		برو cleanup;
	configure_added_submodule(&add_data);

	ret = 0;
cleanup:
	آزاد(sm_name_to_free);
	آزاد(add_data.sm_path);
	آزاد(to_free);
	strbuf_release(&sb);

	بازگشت ret;
}

صحیح cmd_submodule__helper(صحیح argc,
			  ثابت نویسه **argv,
			  ثابت نویسه *prefix,
			  ساختار repository *repo)
{
	parse_opt_subcommand_fn *fn = پوچ;
	ثابت نویسه *ثابت usage[] = {
		N_("git submodule--helper <command>"),
		پوچ
	};
	ساختار option options[] = {
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
		OPT_SUBCOMMAND("push-check", &fn, افزودن_check),
		OPT_SUBCOMMAND("absorbgitdirs", &fn, absorb_git_dirs),
		OPT_SUBCOMMAND("set-url", &fn, module_set_url),
		OPT_SUBCOMMAND("set-branch", &fn, module_set_branch),
		OPT_SUBCOMMAND("create-branch", &fn, module_create_branch),
		OPT_SUBCOMMAND("get-default-remote", &fn, module_get_default_remote),
		OPT_END()
	};
	argc = parse_options(argc, argv, prefix, options, usage, 0);

	بازگشت fn(argc, argv, prefix, repo);
}
