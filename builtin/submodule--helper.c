#define USE_THE_REPOSITORY_VARIABLE

#ضمن "builtin.h"
#ضمن "abspath.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "hex.h"

#ضمن "config.h"
#ضمن "parse-options.h"
#ضمن "quote.h"
#ضمن "path.h"
#ضمن "pathspec.h"
#ضمن "preload-index.h"
#ضمن "dir.h"
#ضمن "read-cache.h"
#ضمن "setup.h"
#ضمن "sparse-index.h"
#ضمن "submodule.h"
#ضمن "submodule-config.h"
#ضمن "string-list.h"
#ضمن "run-command.h"
#ضمن "remote.h"
#ضمن "refs.h"
#ضمن "refspec.h"
#ضمن "revision.h"
#ضمن "diffcore.h"
#ضمن "diff.h"
#ضمن "object-file.h"
#ضمن "object-name.h"
#ضمن "odb.h"
#ضمن "odb/source.h"
#ضمن "odb/source-files.h"
#ضمن "advice.h"
#ضمن "branch.h"
#ضمن "list-objects-filter-options.h"
#ضمن "wildmatch.h"
#ضمن "strbuf.h"
#ضمن "url.h"

#define OPT_QUIET (1 << 0)
#define OPT_CACHED (1 << 1)
#define OPT_RECURSIVE (1 << 2)
#define OPT_FORCE (1 << 3)

تعريفنوع فراغ (*each_submodule_fn)(ثابت بنية cache_entry *list_item,
				  فراغ *cb_data);

ساكن حرف *get_default_remote(فراغ)
{
	החזר xstrdup(repo_default_remote(the_repository));
}

ساكن حرف *resolve_relative_url(ثابت حرف *rel_url, ثابت حرف *up_path, صحيح quiet)
{
	حرف *remoteurl, *resolved_url;
	حرف *remote = get_default_remote();
	بنية strbuf remotesb = STRBUF_INIT;

	strbuf_addf(&remotesb, "remote.%s.url", remote);
	אם (repo_config_get_string(the_repository, remotesb.buf, &remoteurl)) {
		אם (!quiet)
			warning(_("could not look up configuration '%s'. "
				  "Assuming this repository is its own "
				  "authoritative upstream."),
				remotesb.buf);
		remoteurl = xgetcwd();
	}
	resolved_url = relative_url(remoteurl, rel_url, up_path);

	حرر(remote);
	حرر(remoteurl);
	strbuf_release(&remotesb);

	החזר resolved_url;
}

ساكن صحيح get_default_remote_submodule(ثابت حرف *module_path, حرف **default_remote)
{
	ثابت بنية submodule *sub;
	بنية repository subrepo;
	ثابت حرف *remote_name = عدم;
	حرف *url = عدم;
	صحيح ret = 0;

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), module_path);
	אם (sub && sub->url) {
		url = xstrdup(sub->url);

		/* Possibly a url relative to parent */
		אם (starts_with_dot_dot_slash(url) ||
		    starts_with_dot_slash(url)) {
			حرف *oldurl = url;

			url = resolve_relative_url(oldurl, عدم, 1);
			حرر(oldurl);
		}
	}

	אם (repo_submodule_init(&subrepo, the_repository, module_path,
				null_oid(the_hash_algo)) < 0) {
		ret = die_message(_("could not get a repository handle for submodule '%s'"),
				   module_path);
		اذهب out;
	}

	/* Look up by URL first */
	אם (url)
		remote_name = repo_remote_from_url(&subrepo, url);
	אם (!remote_name)
		remote_name = repo_default_remote(&subrepo);

	*default_remote = xstrdup(remote_name);

out:
	repo_clear(&subrepo);
	حرر(url);

	החזר ret;
}

ساكن صحيح module_get_default_remote(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix,
				     بنية repository *repo UNUSED)
{
	ثابت حرف *path;
	حرف *resolved_path = عدم;
	حرف *default_remote = عدم;
	صحيح code;
	بنية option options[] = {
		OPT_END()
	};
	ثابت حرف *ثابت usage[] = {
		N_("git submodule--helper get-default-remote <path>"),
		عدم
	};

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	אם (argc != 1)
		usage_with_options(usage, options);

	path = argv[0];
	אם (prefix && *prefix && !is_absolute_path(path)) {
		resolved_path = xstrfmt("%s%s", prefix, path);
		path = resolved_path;
	}

	code = get_default_remote_submodule(path, &default_remote);
	אם (code) {
		حرر(resolved_path);
		החזר code;
	}

	printf("%s\n", default_remote);
	حرر(default_remote);
	حرر(resolved_path);
	החזר 0;
}

/* the result should be freed by the caller. */
ساكن حرف *get_submodule_displaypath(ثابت حرف *path, ثابت حرف *prefix,
				       ثابت حرف *super_prefix)
{
	אם (prefix && super_prefix) {
		BUG("cannot have prefix '%s' and superprefix '%s'",
		    prefix, super_prefix);
	} אחרת אם (prefix) {
		بنية strbuf sb = STRBUF_INIT;
		حرف *displaypath = xstrdup(relative_path(path, prefix, &sb));
		strbuf_release(&sb);
		החזר displaypath;
	} אחרת אם (super_prefix) {
		החזר xstrfmt("%s%s", super_prefix, path);
	} אחרת {
		החזר xstrdup(path);
	}
}

ساكن حرف *compute_rev_name(ثابت حرف *sub_path, ثابت حرف* object_id)
{
	بنية strbuf sb = STRBUF_INIT;
	ثابت حرف ***d;

	ساكن ثابت حرف *describe_bare[] = { عدم };

	ساكن ثابت حرف *describe_tags[] = { "--tags", عدم };

	ساكن ثابت حرف *describe_contains[] = { "--contains", عدم };

	ساكن ثابت حرف *describe_all_always[] = { "--all", "--always", عدم };

	ساكن ثابت حرف **describe_argv[] = { describe_bare, describe_tags,
						describe_contains,
						describe_all_always, عدم };

	עבור (d = describe_argv; *d; d++) {
		بنية child_process cp = CHILD_PROCESS_INIT;
		prepare_submodule_repo_env(&cp.env);
		cp.dir = sub_path;
		cp.git_cmd = 1;
		cp.no_stderr = 1;

		strvec_push(&cp.args, "describe");
		strvec_pushv(&cp.args, *d);
		strvec_push(&cp.args, object_id);

		אם (!capture_command(&cp, &sb, 0)) {
			strbuf_strip_suffix(&sb, "\n");
			החזר strbuf_detach(&sb, عدم);
		}
	}

	strbuf_release(&sb);
	החזר عدم;
}

بنية module_list {
	ثابت بنية cache_entry **entries;
	صحيح alloc, nr;
};
#define MODULE_LIST_INIT { 0 }

ساكن فراغ module_list_release(بنية module_list *ml)
{
	حرر(ml->entries);
}

ساكن صحيح module_list_compute(ثابت حرف **argv,
			       ثابت حرف *prefix,
			       بنية pathspec *pathspec,
			       بنية module_list *list)
{
	صحيح result = 0;
	حرف *ps_matched = عدم;

	parse_pathspec(pathspec, 0,
		       PATHSPEC_PREFER_FULL,
		       prefix, argv);

	אם (pathspec->nr)
		ps_matched = xcalloc(pathspec->nr, 1);

	אם (repo_read_index(the_repository) < 0)
		die(_("index file corrupt"));

	עבור (size_t i = 0; i < the_repository->index->cache_nr; i++) {
		ثابت بنية cache_entry *ce = the_repository->index->cache[i];

		אם (!match_pathspec(the_repository->index, pathspec, ce->name, ce_namelen(ce),
				    0, ps_matched, 1) ||
		    !S_ISGITLINK(ce->ce_mode))
			استمر;

		ALLOC_GROW(list->entries, list->nr + 1, list->alloc);
		list->entries[list->nr++] = ce;
		כלעוד (i + 1 < the_repository->index->cache_nr &&
		       !strcmp(ce->name, the_repository->index->cache[i + 1]->name))
			/*
			 * Skip entries with the same name in different stages
			 * to make sure an entry is returned only once.
			 */
			i++;
	}

	אם (ps_matched && report_path_error(ps_matched, pathspec))
		result = -1;

	حرر(ps_matched);

	החזר result;
}

ساكن فراغ module_list_active(بنية module_list *list)
{
	صحيح i;
	بنية module_list active_modules = MODULE_LIST_INIT;

	עבור (i = 0; i < list->nr; i++) {
		ثابت بنية cache_entry *ce = list->entries[i];

		אם (!is_submodule_active(the_repository, ce->name))
			استمر;

		ALLOC_GROW(active_modules.entries,
			   active_modules.nr + 1,
			   active_modules.alloc);
		active_modules.entries[active_modules.nr++] = ce;
	}

	module_list_release(list);
	*list = active_modules;
}

ساكن حرف *get_up_path(ثابت حرف *path)
{
	بنية strbuf sb = STRBUF_INIT;

	strbuf_addstrings(&sb, "../", count_slashes(path));

	/*
	 * Check if 'path' ends with slash or not
	 * for having the same output for dir/sub_dir
	 * and dir/sub_dir/
	 */
	אם (!is_dir_sep(path[strlen(path) - 1]))
		strbuf_addstr(&sb, "../");

	החזר strbuf_detach(&sb, عدم);
}

ساكن فراغ for_each_listed_submodule(ثابت بنية module_list *list,
				      each_submodule_fn fn, فراغ *cb_data)
{
	صحيح i;

	עבור (i = 0; i < list->nr; i++)
		fn(list->entries[i], cb_data);
}

بنية foreach_cb {
	صحيح argc;
	ثابت حرف **argv;
	ثابت حرف *prefix;
	ثابت حرف *super_prefix;
	صحيح quiet;
	صحيح recursive;
};
#define FOREACH_CB_INIT { 0 }

ساكن فراغ runcommand_in_submodule_cb(ثابت بنية cache_entry *list_item,
				       فراغ *cb_data)
{
	بنية foreach_cb *info = cb_data;
	ثابت حرف *path = list_item->name;
	ثابت بنية object_id *ce_oid = &list_item->oid;
	ثابت بنية submodule *sub;
	بنية child_process cp = CHILD_PROCESS_INIT;
	حرف *displaypath;

	אם (validate_submodule_path(path) < 0)
		die(عدم);

	displaypath = get_submodule_displaypath(path, info->prefix,
						info->super_prefix);

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);

	אם (!sub)
		die(_("No url found for submodule path '%s' in .gitmodules"),
			displaypath);

	אם (!is_submodule_populated_gently(path, عدم))
		اذهب cleanup;

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
	אם (info->argc == 1) {
		حرف *toplevel = xgetcwd();
		بنية strbuf sb = STRBUF_INIT;

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
		حرر(toplevel);
	} אחרת {
		strvec_pushv(&cp.args, info->argv);
	}

	אם (!info->quiet)
		printf(_("Entering '%s'\n"), displaypath);

	אם (info->argv[0]) {
		אם (run_command(&cp))
			die(_("run_command returned non-zero status for %s\n."),
			    displaypath);
	} אחרת {
		child_process_clear(&cp);
	}

	אם (info->recursive) {
		بنية child_process cpr = CHILD_PROCESS_INIT;

		cpr.git_cmd = 1;
		cpr.dir = path;
		prepare_submodule_repo_env(&cpr.env);

		strvec_pushl(&cpr.args, "submodule--helper", "foreach", "--recursive",
			     عدم);
		strvec_pushf(&cpr.args, "--super-prefix=%s/", displaypath);

		אם (info->quiet)
			strvec_push(&cpr.args, "--quiet");

		strvec_push(&cpr.args, "--");
		strvec_pushv(&cpr.args, info->argv);

		אם (run_command(&cpr))
			die(_("run_command returned non-zero status while "
				"recursing in the nested submodules of %s\n."),
				displaypath);
	}

cleanup:
	حرر(displaypath);
}

ساكن صحيح module_foreach(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix,
			  بنية repository *repo UNUSED)
{
	بنية foreach_cb info = FOREACH_CB_INIT;
	بنية pathspec pathspec = { 0 };
	بنية module_list list = MODULE_LIST_INIT;
	بنية option module_foreach_options[] = {
		OPT__SUPER_PREFIX(&info.super_prefix),
		OPT__QUIET(&info.quiet, N_("suppress output of entering each submodule command")),
		OPT_BOOL(0, "recursive", &info.recursive,
			 N_("recurse into nested submodules")),
		OPT_END()
	};
	ثابت حرف *ثابت git_submodule_helper_usage[] = {
		N_("git submodule foreach [--quiet] [--recursive] [--] <command>"),
		عدم
	};
	صحيح ret = 1;

	argc = parse_options(argc, argv, prefix, module_foreach_options,
			     git_submodule_helper_usage, 0);

	אם (module_list_compute(عدم, prefix, &pathspec, &list) < 0)
		اذهب cleanup;

	info.argc = argc;
	info.argv = argv;
	info.prefix = prefix;

	for_each_listed_submodule(&list, runcommand_in_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	החזר ret;
}

بنية init_cb {
	ثابت حرف *prefix;
	ثابت حرف *super_prefix;
	unsigned صحيح flags;
};
#define INIT_CB_INIT { 0 }

ساكن صحيح validate_and_set_submodule_gitdir(بنية strbuf *gitdir_path,
					     ثابت حرف *submodule_name)
{
	ثابت حرف *value;
	حرف *key;

	אם (validate_submodule_git_dir(gitdir_path->buf, submodule_name))
		החזר -1;

	key = xstrfmt("submodule.%s.gitdir", submodule_name);

	/* Nothing to do if the config already exists. */
	אם (!repo_config_get_string_tmp(the_repository, key, &value)) {
		حرر(key);
		החזר 0;
	}

	אם (repo_config_set_gently(the_repository, key, gitdir_path->buf)) {
		حرر(key);
		החזר -1;
	}

	حرر(key);
	החזר 0;
}

ساكن فراغ create_default_gitdir_config(ثابت حرف *submodule_name)
{
	بنية strbuf gitdir_path = STRBUF_INIT;
	بنية git_hash_ctx ctx;
	حرف hex_name_hash[GIT_MAX_HEXSZ + 1], header[128];
	unsigned حرف raw_name_hash[GIT_MAX_RAWSZ];
	صحيح header_len;

	/* Case 1: try the plain module name */
	repo_git_path_append(the_repository, &gitdir_path, "modules/%s", submodule_name);
	אם (!validate_and_set_submodule_gitdir(&gitdir_path, submodule_name)) {
		strbuf_release(&gitdir_path);
		החזר;
	}

	/* Case 2.1: Try URI-safe (RFC3986) encoding first, this fixes nested gitdirs */
	strbuf_reset(&gitdir_path);
	repo_git_path_append(the_repository, &gitdir_path, "modules/");
	strbuf_addstr_urlencode(&gitdir_path, submodule_name, is_rfc3986_unreserved);
	אם (!validate_and_set_submodule_gitdir(&gitdir_path, submodule_name)) {
		strbuf_release(&gitdir_path);
		החזר;
	}

	/* Case 2.2: Try extended uppercase URI (RFC3986) encoding, to fix case-folding */
	strbuf_reset(&gitdir_path);
	repo_git_path_append(the_repository, &gitdir_path, "modules/");
	strbuf_addstr_urlencode(&gitdir_path, submodule_name, is_casefolding_rfc3986_unreserved);
	אם (!validate_and_set_submodule_gitdir(&gitdir_path, submodule_name))
		החזר;

	/* Case 2.3: Try some derived gitdir names, see if one sticks */
	עבור (حرف c = '0'; c <= '9'; c++) {
		strbuf_reset(&gitdir_path);
		repo_git_path_append(the_repository, &gitdir_path, "modules/");
		strbuf_addstr_urlencode(&gitdir_path, submodule_name, is_rfc3986_unreserved);
		strbuf_addch(&gitdir_path, c);
		אם (!validate_and_set_submodule_gitdir(&gitdir_path, submodule_name))
			החזר;

		strbuf_reset(&gitdir_path);
		repo_git_path_append(the_repository, &gitdir_path, "modules/");
		strbuf_addstr_urlencode(&gitdir_path, submodule_name, is_casefolding_rfc3986_unreserved);
		strbuf_addch(&gitdir_path, c);
		אם (!validate_and_set_submodule_gitdir(&gitdir_path, submodule_name))
			החזר;
	}

	/* Case 2.4: If all the above failed, try a hash of the name as a last resort */
	header_len = snprintf(header, حجم(header),
			      "blob %"PRIuMAX, (uintmax_t)strlen(submodule_name));
	git_hash_init(&ctx, the_hash_algo);
	git_hash_update(&ctx, header, header_len);
	git_hash_update(&ctx, "\0", 1);
	git_hash_update(&ctx, submodule_name, strlen(submodule_name));
	git_hash_final(raw_name_hash, &ctx);
	hash_to_hex_algop_r(hex_name_hash, raw_name_hash, the_hash_algo);
	strbuf_reset(&gitdir_path);
	repo_git_path_append(the_repository, &gitdir_path, "modules/%s", hex_name_hash);
	אם (!validate_and_set_submodule_gitdir(&gitdir_path, submodule_name)) {
		strbuf_release(&gitdir_path);
		החזר;
	}

	/* Case 3: nothing worked, error out */
	die(_("failed to set a valid default config for 'submodule.%s.gitdir'. "
	      "Please ensure it is set, for example by running something like: "
	      "'git config submodule.%s.gitdir .git/modules/%s'"),
	    submodule_name, submodule_name, submodule_name);
}

ساكن فراغ init_submodule(ثابت حرف *path, ثابت حرف *prefix,
			   ثابت حرف *super_prefix,
			   unsigned صحيح flags)
{
	ثابت بنية submodule *sub;
	بنية strbuf sb = STRBUF_INIT;
	ثابت حرف *upd;
	حرف *url = عدم, *displaypath;

	displaypath = get_submodule_displaypath(path, prefix, super_prefix);

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);

	אם (!sub)
		die(_("No url found for submodule path '%s' in .gitmodules"),
			displaypath);

	/*
	 * NEEDSWORK: In a multi-working-tree world, this needs to be
	 * set in the per-worktree config.
	 *
	 * Set active flag for the submodule being initialized
	 */
	אם (!is_submodule_active(the_repository, path)) {
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
	אם (repo_config_get_string(the_repository, sb.buf, &url)) {
		אם (!sub->url)
			die(_("No url found for submodule path '%s' in .gitmodules"),
				displaypath);

		url = xstrdup(sub->url);

		/* Possibly a url relative to parent */
		אם (starts_with_dot_dot_slash(url) ||
		    starts_with_dot_slash(url)) {
			حرف *oldurl = url;

			url = resolve_relative_url(oldurl, عدم, 0);
			حرر(oldurl);
		}

		אם (repo_config_set_gently(the_repository, sb.buf, url))
			die(_("Failed to register url for submodule path '%s'"),
			    displaypath);
		אם (!(flags & OPT_QUIET))
			fprintf(stderr,
				_("Submodule '%s' (%s) registered for path '%s'\n"),
				sub->name, url, displaypath);
	}
	strbuf_reset(&sb);

	/* Copy "update" setting when it is not set yet */
	strbuf_addf(&sb, "submodule.%s.update", sub->name);
	אם (repo_config_get_string_tmp(the_repository, sb.buf, &upd) &&
	    sub->update_strategy.type != SM_UPDATE_UNSPECIFIED) {
		אם (sub->update_strategy.type == SM_UPDATE_COMMAND) {
			fprintf(stderr, _("warning: command update mode suggested for submodule '%s'\n"),
				sub->name);
			upd = "none";
		} אחרת {
			upd = submodule_update_type_to_string(sub->update_strategy.type);
		}

		אם (repo_config_set_gently(the_repository, sb.buf, upd))
			die(_("Failed to register update mode for submodule path '%s'"), displaypath);
	}

	אם (the_repository->repository_format_submodule_path_cfg)
		create_default_gitdir_config(sub->name);

	strbuf_release(&sb);
	حرر(displaypath);
	حرر(url);
}

ساكن فراغ init_submodule_cb(ثابت بنية cache_entry *list_item, فراغ *cb_data)
{
	بنية init_cb *info = cb_data;

	init_submodule(list_item->name, info->prefix, info->super_prefix,
		       info->flags);
}

ساكن صحيح module_init(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix,
		       بنية repository *repo UNUSED)
{
	بنية init_cb info = INIT_CB_INIT;
	بنية pathspec pathspec = { 0 };
	بنية module_list list = MODULE_LIST_INIT;
	صحيح quiet = 0;
	بنية option module_init_options[] = {
		OPT__QUIET(&quiet, N_("suppress output for initializing a submodule")),
		OPT_END()
	};
	ثابت حرف *ثابت git_submodule_helper_usage[] = {
		N_("git submodule init [<options>] [<path>]"),
		عدم
	};
	صحيح ret = 1;

	argc = parse_options(argc, argv, prefix, module_init_options,
			     git_submodule_helper_usage, 0);

	אם (module_list_compute(argv, prefix, &pathspec, &list) < 0)
		اذهب cleanup;

	/*
	 * If there are no path args and submodule.active is set then,
	 * by default, only initialize 'active' modules.
	 */
	אם (!argc && !repo_config_get(the_repository, "submodule.active"))
		module_list_active(&list);

	info.prefix = prefix;
	אם (quiet)
		info.flags |= OPT_QUIET;

	for_each_listed_submodule(&list, init_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	החזר ret;
}

بنية status_cb {
	ثابت حرف *prefix;
	ثابت حرف *super_prefix;
	unsigned صحيح flags;
};
#define STATUS_CB_INIT { 0 }

ساكن فراغ print_status(unsigned صحيح flags, حرف state, ثابت حرف *path,
			 ثابت بنية object_id *oid, ثابت حرف *displaypath)
{
	אם (flags & OPT_QUIET)
		החזר;

	printf("%c%s %s", state, oid_to_hex(oid), displaypath);

	אם (state == ' ' || state == '+') {
		حرف *name = compute_rev_name(path, oid_to_hex(oid));

		אם (name)
			printf(" (%s)", name);
		حرر(name);
	}

	printf("\n");
}

ساكن صحيح handle_submodule_head_ref(ثابت بنية reference *ref, فراغ *cb_data)
{
	بنية object_id *output = cb_data;

	אם (ref->oid)
		oidcpy(output, ref->oid);

	החזר 0;
}

ساكن فراغ status_submodule(ثابت حرف *path, ثابت بنية object_id *ce_oid,
			     unsigned صحيح ce_flags, ثابت حرف *prefix,
			     ثابت حرف *super_prefix, unsigned صحيح flags)
{
	حرف *displaypath;
	بنية strvec diff_files_args = STRVEC_INIT;
	بنية rev_info rev = REV_INFO_INIT;
	بنية strbuf buf = STRBUF_INIT;
	ثابت حرف *git_dir;

	אם (validate_submodule_path(path) < 0)
		die(عدم);

	אם (!submodule_from_path(the_repository, null_oid(the_hash_algo), path))
		die(_("no submodule mapping found in .gitmodules for path '%s'"),
		      path);

	displaypath = get_submodule_displaypath(path, prefix, super_prefix);

	אם ((CE_STAGEMASK & ce_flags) >> CE_STAGESHIFT) {
		print_status(flags, 'U', path, null_oid(the_hash_algo), displaypath);
		اذهب cleanup;
	}

	strbuf_addf(&buf, "%s/.git", path);
	git_dir = read_gitfile(buf.buf);
	אם (!git_dir)
		git_dir = buf.buf;

	אם (!is_submodule_active(the_repository, path) ||
	    !is_git_directory(git_dir)) {
		print_status(flags, '-', path, ce_oid, displaypath);
		strbuf_release(&buf);
		اذهب cleanup;
	}
	strbuf_release(&buf);

	strvec_pushl(&diff_files_args, "diff-files",
		     "--ignore-submodules=dirty", "--quiet", "--",
		     path, عدم);

	repo_config(the_repository, git_diff_basic_config, عدم);

	repo_init_revisions(the_repository, &rev, عدم);
	rev.abbrev = 0;
	setup_revisions_from_strvec(&diff_files_args, &rev, عدم);
	run_diff_files(&rev, 0);

	אם (!diff_result_code(&rev)) {
		print_status(flags, ' ', path, ce_oid,
			     displaypath);
	} אחרת אם (!(flags & OPT_CACHED)) {
		بنية object_id oid;
		بنية ref_store *refs = repo_get_submodule_ref_store(the_repository,
								      path);

		אם (!refs) {
			print_status(flags, '-', path, ce_oid, displaypath);
			اذهب cleanup;
		}
		אם (refs_head_ref(refs, handle_submodule_head_ref, &oid))
			die(_("could not resolve HEAD ref inside the "
			      "submodule '%s'"), path);

		print_status(flags, '+', path, &oid, displaypath);
	} אחרת {
		print_status(flags, '+', path, ce_oid, displaypath);
	}

	אם (flags & OPT_RECURSIVE) {
		بنية child_process cpr = CHILD_PROCESS_INIT;
		صحيح res;

		cpr.git_cmd = 1;
		cpr.dir = path;
		prepare_submodule_repo_env(&cpr.env);

		strvec_pushl(&cpr.args, "submodule--helper", "status",
			     "--recursive", عدم);
		strvec_pushf(&cpr.args, "--super-prefix=%s/", displaypath);

		אם (flags & OPT_CACHED)
			strvec_push(&cpr.args, "--cached");

		אם (flags & OPT_QUIET)
			strvec_push(&cpr.args, "--quiet");

		res = run_command(&cpr);
		אם (res == SIGPIPE + 128)
			ارفع(SIGPIPE);
		אחרת אם (res)
			die(_("failed to recurse into submodule '%s'"), path);
	}

cleanup:
	strvec_clear(&diff_files_args);
	حرر(displaypath);
	release_revisions(&rev);
}

ساكن فراغ status_submodule_cb(ثابت بنية cache_entry *list_item,
				فراغ *cb_data)
{
	بنية status_cb *info = cb_data;

	status_submodule(list_item->name, &list_item->oid, list_item->ce_flags,
			 info->prefix, info->super_prefix, info->flags);
}

ساكن صحيح module_status(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix,
			 بنية repository *repo UNUSED)
{
	بنية status_cb info = STATUS_CB_INIT;
	بنية pathspec pathspec = { 0 };
	بنية module_list list = MODULE_LIST_INIT;
	صحيح quiet = 0;
	بنية option module_status_options[] = {
		OPT__SUPER_PREFIX(&info.super_prefix),
		OPT__QUIET(&quiet, N_("suppress submodule status output")),
		OPT_BIT(0, "cached", &info.flags, N_("use commit stored in the index instead of the one stored in the submodule HEAD"), OPT_CACHED),
		OPT_BIT(0, "recursive", &info.flags, N_("recurse into nested submodules"), OPT_RECURSIVE),
		OPT_END()
	};
	ثابت حرف *ثابت git_submodule_helper_usage[] = {
		N_("git submodule status [--quiet] [--cached] [--recursive] [<path>...]"),
		عدم
	};
	صحيح ret = 1;

	argc = parse_options(argc, argv, prefix, module_status_options,
			     git_submodule_helper_usage, 0);

	אם (module_list_compute(argv, prefix, &pathspec, &list) < 0)
		اذهب cleanup;

	info.prefix = prefix;
	אם (quiet)
		info.flags |= OPT_QUIET;

	for_each_listed_submodule(&list, status_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	החזר ret;
}

بنية module_cb {
	unsigned صحيح mod_src;
	unsigned صحيح mod_dst;
	بنية object_id oid_src;
	بنية object_id oid_dst;
	حرف status;
	حرف *sm_path;
};
#define MODULE_CB_INIT { 0 }

ساكن فراغ module_cb_release(بنية module_cb *mcb)
{
	حرر(mcb->sm_path);
}

بنية module_cb_list {
	بنية module_cb **entries;
	صحيح alloc, nr;
};
#define MODULE_CB_LIST_INIT { 0 }

ساكن فراغ module_cb_list_release(بنية module_cb_list *mcbl)
{
	صحيح i;

	עבור (i = 0; i < mcbl->nr; i++) {
		بنية module_cb *mcb = mcbl->entries[i];

		module_cb_release(mcb);
		حرر(mcb);
	}
	حرر(mcbl->entries);
}

بنية summary_cb {
	صحيح argc;
	ثابت حرف **argv;
	ثابت حرف *prefix;
	ثابت حرف *super_prefix;
	unsigned صحيح cached: 1;
	unsigned صحيح for_status: 1;
	unsigned صحيح files: 1;
	صحيح summary_limit;
};
#define SUMMARY_CB_INIT { 0 }

تعداد diff_cmd {
	DIFF_INDEX,
	DIFF_FILES
};

ساكن حرف *verify_submodule_committish(ثابت حرف *sm_path,
					 ثابت حرف *committish)
{
	بنية child_process cp_rev_parse = CHILD_PROCESS_INIT;
	بنية strbuf result = STRBUF_INIT;

	cp_rev_parse.git_cmd = 1;
	cp_rev_parse.dir = sm_path;
	prepare_submodule_repo_env(&cp_rev_parse.env);
	strvec_pushl(&cp_rev_parse.args, "rev-parse", "-q", "--short", عدم);
	strvec_pushf(&cp_rev_parse.args, "%s^0", committish);
	strvec_push(&cp_rev_parse.args, "--");

	אם (capture_command(&cp_rev_parse, &result, 0))
		החזר عدم;

	strbuf_trim_trailing_newline(&result);
	החזר strbuf_detach(&result, عدم);
}

ساكن فراغ print_submodule_summary(بنية summary_cb *info, ثابت حرف *errmsg,
				    صحيح total_commits, ثابت حرف *displaypath,
				    ثابت حرف *src_abbrev, ثابت حرف *dst_abbrev,
				    بنية module_cb *p)
{
	אם (p->status == 'T') {
		אם (S_ISGITLINK(p->mod_dst))
			printf(_("* %s %s(blob)->%s(submodule)"),
				 displaypath, src_abbrev, dst_abbrev);
		אחרת
			printf(_("* %s %s(submodule)->%s(blob)"),
				 displaypath, src_abbrev, dst_abbrev);
	} אחרת {
		printf("* %s %s...%s",
			displaypath, src_abbrev, dst_abbrev);
	}

	אם (total_commits < 0)
		printf(":\n");
	אחרת
		printf(" (%d):\n", total_commits);

	אם (errmsg) {
		printf(_("%s"), errmsg);
	} אחרת אם (total_commits > 0) {
		بنية child_process cp_log = CHILD_PROCESS_INIT;

		cp_log.git_cmd = 1;
		cp_log.dir = p->sm_path;
		prepare_submodule_repo_env(&cp_log.env);
		strvec_pushl(&cp_log.args, "log", عدم);

		אם (S_ISGITLINK(p->mod_src) && S_ISGITLINK(p->mod_dst)) {
			אם (info->summary_limit > 0)
				strvec_pushf(&cp_log.args, "-%d",
					     info->summary_limit);

			strvec_pushl(&cp_log.args, "--pretty=  %m %s",
				     "--first-parent", عدم);
			strvec_pushf(&cp_log.args, "%s...%s",
				     src_abbrev, dst_abbrev);
		} אחרת אם (S_ISGITLINK(p->mod_dst)) {
			strvec_pushl(&cp_log.args, "--pretty=  > %s",
				     "-1", dst_abbrev, عدم);
		} אחרת {
			strvec_pushl(&cp_log.args, "--pretty=  < %s",
				     "-1", src_abbrev, عدم);
		}
		run_command(&cp_log);
	}
	printf("\n");
}

ساكن فراغ generate_submodule_summary(بنية summary_cb *info,
				       بنية module_cb *p)
{
	حرف *displaypath, *src_abbrev = عدم, *dst_abbrev;
	صحيح missing_src = 0, missing_dst = 0;
	بنية strbuf errmsg = STRBUF_INIT;
	صحيح total_commits = -1;

	אם (!info->cached && oideq(&p->oid_dst, null_oid(the_hash_algo))) {
		אם (S_ISGITLINK(p->mod_dst)) {
			بنية ref_store *refs = repo_get_submodule_ref_store(the_repository,
									      p->sm_path);

			אם (refs)
				refs_head_ref(refs, handle_submodule_head_ref, &p->oid_dst);
		} אחרת אם (S_ISLNK(p->mod_dst) || S_ISREG(p->mod_dst)) {
			بنية stat st;
			صحيح fd = open(p->sm_path, O_RDONLY);

			אם (fd < 0 || fstat(fd, &st) < 0 ||
			    index_fd(the_repository->index, &p->oid_dst, fd, &st, OBJ_BLOB,
				     p->sm_path, 0))
				error(_("couldn't hash object from '%s'"), p->sm_path);
		} אחרת {
			/* for a submodule removal (mode:0000000), don't warn */
			אם (p->mod_dst)
				warning(_("unexpected mode %o"), p->mod_dst);
		}
	}

	אם (S_ISGITLINK(p->mod_src)) {
		אם (p->status != 'D')
			src_abbrev = verify_submodule_committish(p->sm_path,
								 oid_to_hex(&p->oid_src));
		אם (!src_abbrev) {
			missing_src = 1;
			/*
			 * As `rev-parse` failed, we fallback to getting
			 * the abbreviated hash using oid_src. We do
			 * this as we might still need the abbreviated
			 * hash in cases like a submodule type change, etc.
			 */
			src_abbrev = xstrndup(oid_to_hex(&p->oid_src), 7);
		}
	} אחרת {
		/*
		 * The source does not point to a submodule.
		 * So, we fallback to getting the abbreviation using
		 * oid_src as we might still need the abbreviated
		 * hash in cases like submodule add, etc.
		 */
		src_abbrev = xstrndup(oid_to_hex(&p->oid_src), 7);
	}

	אם (S_ISGITLINK(p->mod_dst)) {
		dst_abbrev = verify_submodule_committish(p->sm_path,
							 oid_to_hex(&p->oid_dst));
		אם (!dst_abbrev) {
			missing_dst = 1;
			/*
			 * As `rev-parse` failed, we fallback to getting
			 * the abbreviated hash using oid_dst. We do
			 * this as we might still need the abbreviated
			 * hash in cases like a submodule type change, etc.
			 */
			dst_abbrev = xstrndup(oid_to_hex(&p->oid_dst), 7);
		}
	} אחרת {
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

	אם (!missing_src && !missing_dst) {
		بنية child_process cp_rev_list = CHILD_PROCESS_INIT;
		بنية strbuf sb_rev_list = STRBUF_INIT;

		strvec_pushl(&cp_rev_list.args, "rev-list",
			     "--first-parent", "--count", عدم);
		אם (S_ISGITLINK(p->mod_src) && S_ISGITLINK(p->mod_dst))
			strvec_pushf(&cp_rev_list.args, "%s...%s",
				     src_abbrev, dst_abbrev);
		אחרת
			strvec_push(&cp_rev_list.args, S_ISGITLINK(p->mod_src) ?
				    src_abbrev : dst_abbrev);
		strvec_push(&cp_rev_list.args, "--");

		cp_rev_list.git_cmd = 1;
		cp_rev_list.dir = p->sm_path;
		prepare_submodule_repo_env(&cp_rev_list.env);

		אם (!capture_command(&cp_rev_list, &sb_rev_list, 0))
			total_commits = atoi(sb_rev_list.buf);

		strbuf_release(&sb_rev_list);
	} אחרת {
		/*
		 * Don't give error msg for modification whose dst is not
		 * submodule, i.e., deleted or changed to blob
		 */
		אם (S_ISGITLINK(p->mod_dst)) {
			אם (missing_src && missing_dst) {
				strbuf_addf(&errmsg, "  Warn: %s doesn't contain commits %s and %s\n",
					    displaypath, oid_to_hex(&p->oid_src),
					    oid_to_hex(&p->oid_dst));
			} אחרת {
				strbuf_addf(&errmsg, "  Warn: %s doesn't contain commit %s\n",
					    displaypath, missing_src ?
					    oid_to_hex(&p->oid_src) :
					    oid_to_hex(&p->oid_dst));
			}
		}
	}

	print_submodule_summary(info, errmsg.len ? errmsg.buf : عدم,
				total_commits, displaypath, src_abbrev,
				dst_abbrev, p);

	حرر(displaypath);
	حرر(src_abbrev);
	حرر(dst_abbrev);
	strbuf_release(&errmsg);
}

ساكن فراغ prepare_submodule_summary(بنية summary_cb *info,
				      بنية module_cb_list *list)
{
	صحيح i;
	עבור (i = 0; i < list->nr; i++) {
		ثابت بنية submodule *sub;
		بنية module_cb *p = list->entries[i];
		بنية strbuf sm_gitdir = STRBUF_INIT;

		אם (p->status == 'D' || p->status == 'T') {
			generate_submodule_summary(info, p);
			استمر;
		}

		אם (info->for_status && p->status != 'A' &&
		    (sub = submodule_from_path(the_repository,
					       null_oid(the_hash_algo), p->sm_path))) {
			حرف *config_key = عدم;
			ثابت حرف *value;
			صحيح ignore_all = 0;

			config_key = xstrfmt("submodule.%s.ignore",
					     sub->name);
			אם (!repo_config_get_string_tmp(the_repository, config_key, &value))
				ignore_all = !strcmp(value, "all");
			אחרת אם (sub->ignore)
				ignore_all = !strcmp(sub->ignore, "all");

			حرر(config_key);
			אם (ignore_all)
				استمر;
		}

		/* Also show added or modified modules which are checked out */
		strbuf_addstr(&sm_gitdir, p->sm_path);
		אם (is_nonbare_repository_dir(&sm_gitdir))
			generate_submodule_summary(info, p);
		strbuf_release(&sm_gitdir);
	}
}

ساكن فراغ submodule_summary_callback(بنية diff_queue_struct *q,
				       بنية diff_options *options UNUSED,
				       فراغ *data)
{
	صحيح i;
	بنية module_cb_list *list = data;
	עבור (i = 0; i < q->nr; i++) {
		بنية diff_filepair *p = q->queue[i];
		بنية module_cb *temp;

		אם (!S_ISGITLINK(p->one->mode) && !S_ISGITLINK(p->two->mode))
			استمر;
		temp = xmalloc(حجم(*temp));
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

ساكن ثابت حرف *get_diff_cmd(تعداد diff_cmd diff_cmd)
{
	بدل (diff_cmd) {
	حالة DIFF_INDEX: החזר "diff-index";
	حالة DIFF_FILES: החזר "diff-files";
	افتراضي: BUG("bad diff_cmd value %d", diff_cmd);
	}
}

ساكن صحيح compute_summary_module_list(بنية object_id *head_oid,
				       بنية summary_cb *info,
				       تعداد diff_cmd diff_cmd)
{
	بنية strvec diff_args = STRVEC_INIT;
	بنية rev_info rev;
	بنية module_cb_list list = MODULE_CB_LIST_INIT;
	صحيح ret = 0;

	strvec_push(&diff_args, get_diff_cmd(diff_cmd));
	אם (info->cached)
		strvec_push(&diff_args, "--cached");
	strvec_pushl(&diff_args, "--ignore-submodules=dirty", "--raw", عدم);
	אם (head_oid)
		strvec_push(&diff_args, oid_to_hex(head_oid));
	strvec_push(&diff_args, "--");
	אם (info->argc)
		strvec_pushv(&diff_args, info->argv);

	repo_config(the_repository, git_diff_basic_config, عدم);
	repo_init_revisions(the_repository, &rev, info->prefix);
	rev.abbrev = 0;
	precompose_argv_prefix(diff_args.nr, diff_args.v, عدم);
	setup_revisions_from_strvec(&diff_args, &rev, عدم);
	rev.diffopt.output_format = DIFF_FORMAT_NO_OUTPUT | DIFF_FORMAT_CALLBACK;
	rev.diffopt.format_callback = submodule_summary_callback;
	rev.diffopt.format_callback_data = &list;

	אם (!info->cached) {
		אם (diff_cmd == DIFF_INDEX)
			setup_work_tree(the_repository);
		אם (repo_read_index_preload(the_repository, &rev.diffopt.pathspec, 0) < 0) {
			perror("repo_read_index_preload");
			ret = -1;
			اذهب cleanup;
		}
	} אחרת אם (repo_read_index(the_repository) < 0) {
		perror("repo_read_cache");
		ret = -1;
		اذهب cleanup;
	}

	אם (diff_cmd == DIFF_INDEX)
		run_diff_index(&rev, info->cached ? DIFF_INDEX_CACHED : 0);
	אחרת
		run_diff_files(&rev, 0);
	prepare_submodule_summary(info, &list);
cleanup:
	strvec_clear(&diff_args);
	release_revisions(&rev);
	module_cb_list_release(&list);
	החזר ret;
}

ساكن صحيح module_summary(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix,
			  بنية repository *repo UNUSED)
{
	بنية summary_cb info = SUMMARY_CB_INIT;
	صحيح cached = 0;
	صحيح for_status = 0;
	صحيح files = 0;
	صحيح summary_limit = -1;
	تعداد diff_cmd diff_cmd = DIFF_INDEX;
	بنية object_id head_oid;
	صحيح ret;
	بنية option module_summary_options[] = {
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
	ثابت حرف *ثابت git_submodule_helper_usage[] = {
		N_("git submodule summary [<options>] [<commit>] [--] [<path>]"),
		عدم
	};

	argc = parse_options(argc, argv, prefix, module_summary_options,
			     git_submodule_helper_usage, 0);

	אם (!summary_limit)
		החזר 0;

	אם (!repo_get_oid(the_repository, argc ? argv[0] : "HEAD", &head_oid)) {
		אם (argc) {
			argv++;
			argc--;
		}
	} אחרת אם (!argc || !strcmp(argv[0], "HEAD")) {
		/* before the first commit: compare with an empty tree */
		oidcpy(&head_oid, the_hash_algo->empty_tree);
		אם (argc) {
			argv++;
			argc--;
		}
	} אחרת {
		אם (repo_get_oid(the_repository, "HEAD", &head_oid))
			die(_("could not fetch a revision for HEAD"));
	}

	אם (files) {
		אם (cached)
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

	ret = compute_summary_module_list((diff_cmd == DIFF_INDEX) ? &head_oid : عدم,
					  &info, diff_cmd);
	החזר ret;
}

ساكن صحيح module_gitdir(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix UNUSED,
			 بنية repository *repo)
{
	بنية strbuf gitdir = STRBUF_INIT;

	אם (argc != 2)
		usage(_("git submodule--helper gitdir <name>"));

	submodule_name_to_gitdir(&gitdir, repo, argv[1]);

	printf("%s\n", gitdir.buf);

	strbuf_release(&gitdir);
	החזר 0;
}

ساكن صحيح module_migrate(صحيح argc UNUSED, ثابت حرف **argv UNUSED,
			  ثابت حرف *prefix UNUSED, بنية repository *repo)
{
	بنية strbuf module_dir = STRBUF_INIT;
	DIR *dir;
	بنية dirent *de;
	صحيح repo_version = 0;

	repo_git_path_append(repo, &module_dir, "modules/");

	dir = opendir(module_dir.buf);
	אם (!dir)
		die(_("could not open '%s'"), module_dir.buf);

	כלעוד ((de = readdir(dir))) {
		بنية strbuf gitdir_path = STRBUF_INIT;
		حرف *key;
		ثابت حرف *value;

		אם (is_dot_or_dotdot(de->d_name))
			استمر;

		strbuf_addf(&gitdir_path, "%s/%s", module_dir.buf, de->d_name);
		אם (!is_git_directory(gitdir_path.buf)) {
			strbuf_release(&gitdir_path);
			استمر;
		}
		strbuf_release(&gitdir_path);

		key = xstrfmt("submodule.%s.gitdir", de->d_name);
		אם (!repo_config_get_string_tmp(repo, key, &value)) {
			/* Already has a gitdir config, nothing to do. */
			حرر(key);
			استمر;
		}
		حرر(key);

		create_default_gitdir_config(de->d_name);
	}

	closedir(dir);
	strbuf_release(&module_dir);

	repo_config_get_int(the_repository, "core.repositoryformatversion", &repo_version);
	אם (repo_version == 0 &&
	    repo_config_set_gently(repo, "core.repositoryformatversion", "1"))
		die(_("could not set core.repositoryformatversion to 1.\n"
		      "Please set it for migration to work, for example:\n"
		      "git config core.repositoryformatversion 1"));

	אם (repo_config_set_gently(repo, "extensions.submodulePathConfig", "true"))
		die(_("could not enable submodulePathConfig extension. It is required\n"
		      "for migration to work. Please enable it in the root repo:\n"
		      "git config extensions.submodulePathConfig true"));

	repo->repository_format_submodule_path_cfg = 1;

	החזר 0;
}

بنية sync_cb {
	ثابت حرف *prefix;
	ثابت حرف *super_prefix;
	unsigned صحيح flags;
};
#define SYNC_CB_INIT { 0 }

ساكن فراغ sync_submodule(ثابت حرف *path, ثابت حرف *prefix,
			   ثابت حرف *super_prefix, unsigned صحيح flags)
{
	ثابت بنية submodule *sub;
	حرف *remote_key = عدم;
	حرف *sub_origin_url, *super_config_url, *displaypath, *default_remote;
	بنية strbuf sb = STRBUF_INIT;
	حرف *sub_config_path = عدم;
	صحيح code;

	אם (!is_submodule_active(the_repository, path))
		החזר;

	אם (validate_submodule_path(path) < 0)
		die(عدم);

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);

	אם (sub && sub->url) {
		אם (starts_with_dot_dot_slash(sub->url) ||
		    starts_with_dot_slash(sub->url)) {
			حرف *up_path = get_up_path(path);

			sub_origin_url = resolve_relative_url(sub->url, up_path, 1);
			super_config_url = resolve_relative_url(sub->url, عدم, 1);
			حرر(up_path);
		} אחרת {
			sub_origin_url = xstrdup(sub->url);
			super_config_url = xstrdup(sub->url);
		}
	} אחרת {
		sub_origin_url = xstrdup("");
		super_config_url = xstrdup("");
	}

	displaypath = get_submodule_displaypath(path, prefix, super_prefix);

	אם (!(flags & OPT_QUIET))
		printf(_("Synchronizing submodule url for '%s'\n"),
			 displaypath);

	strbuf_reset(&sb);
	strbuf_addf(&sb, "submodule.%s.url", sub->name);
	אם (repo_config_set_gently(the_repository, sb.buf, super_config_url))
		die(_("failed to register url for submodule path '%s'"),
		      displaypath);

	אם (!is_submodule_populated_gently(path, عدم))
		اذهب cleanup;

	strbuf_reset(&sb);
	code = get_default_remote_submodule(path, &default_remote);
	אם (code)
		exit(code);

	remote_key = xstrfmt("remote.%s.url", default_remote);
	حرر(default_remote);

	submodule_to_gitdir(the_repository, &sb, path);
	strbuf_addstr(&sb, "/config");

	אם (repo_config_set_in_file_gently(the_repository, sb.buf, remote_key, عدم, sub_origin_url))
		die(_("failed to update remote for submodule '%s'"),
		      path);

	אם (flags & OPT_RECURSIVE) {
		بنية child_process cpr = CHILD_PROCESS_INIT;

		cpr.git_cmd = 1;
		cpr.dir = path;
		prepare_submodule_repo_env(&cpr.env);

		strvec_pushl(&cpr.args, "submodule--helper", "sync",
			     "--recursive", عدم);
		strvec_pushf(&cpr.args, "--super-prefix=%s/", displaypath);

		אם (flags & OPT_QUIET)
			strvec_push(&cpr.args, "--quiet");

		אם (run_command(&cpr))
			die(_("failed to recurse into submodule '%s'"),
			      path);
	}

cleanup:
	حرر(super_config_url);
	حرر(sub_origin_url);
	strbuf_release(&sb);
	حرر(remote_key);
	حرر(displaypath);
	حرر(sub_config_path);
}

ساكن فراغ sync_submodule_cb(ثابت بنية cache_entry *list_item, فراغ *cb_data)
{
	بنية sync_cb *info = cb_data;

	sync_submodule(list_item->name, info->prefix, info->super_prefix,
		       info->flags);
}

ساكن صحيح module_sync(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix,
		       بنية repository *repo UNUSED)
{
	بنية sync_cb info = SYNC_CB_INIT;
	بنية pathspec pathspec = { 0 };
	بنية module_list list = MODULE_LIST_INIT;
	صحيح quiet = 0;
	صحيح recursive = 0;
	بنية option module_sync_options[] = {
		OPT__SUPER_PREFIX(&info.super_prefix),
		OPT__QUIET(&quiet, N_("suppress output of synchronizing submodule url")),
		OPT_BOOL(0, "recursive", &recursive,
			N_("recurse into nested submodules")),
		OPT_END()
	};
	ثابت حرف *ثابت git_submodule_helper_usage[] = {
		N_("git submodule sync [--quiet] [--recursive] [<path>]"),
		عدم
	};
	صحيح ret = 1;

	argc = parse_options(argc, argv, prefix, module_sync_options,
			     git_submodule_helper_usage, 0);

	אם (module_list_compute(argv, prefix, &pathspec, &list) < 0)
		اذهب cleanup;

	info.prefix = prefix;
	אם (quiet)
		info.flags |= OPT_QUIET;
	אם (recursive)
		info.flags |= OPT_RECURSIVE;

	for_each_listed_submodule(&list, sync_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	החזר ret;
}

بنية deinit_cb {
	ثابت حرف *prefix;
	unsigned صحيح flags;
};
#define DEINIT_CB_INIT { 0 }

ساكن فراغ deinit_submodule(ثابت حرف *path, ثابت حرف *prefix,
			     unsigned صحيح flags)
{
	ثابت بنية submodule *sub;
	حرف *displaypath = عدم;
	بنية child_process cp_config = CHILD_PROCESS_INIT;
	بنية strbuf sb_config = STRBUF_INIT;
	حرف *sub_git_dir = xstrfmt("%s/.git", path);

	אם (validate_submodule_path(path) < 0)
		die(عدم);

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);

	אם (!sub || !sub->name)
		اذهب cleanup;

	displaypath = get_submodule_displaypath(path, prefix, عدم);

	/* remove the submodule work tree (unless the user already did it) */
	אם (is_directory(path)) {
		بنية strbuf sb_rm = STRBUF_INIT;
		ثابت حرف *format;

		אם (is_directory(sub_git_dir)) {
			אם (!(flags & OPT_QUIET))
				warning(_("Submodule work tree '%s' contains a .git "
					  "directory. This will be replaced with a "
					  ".git file by using absorbgitdirs."),
					displaypath);

			absorb_git_dir_into_superproject(path, عدم);

		}

		אם (!(flags & OPT_FORCE)) {
			بنية child_process cp_rm = CHILD_PROCESS_INIT;

			cp_rm.git_cmd = 1;
			strvec_pushl(&cp_rm.args, "rm", "-qn",
				     path, عدم);

			אם (run_command(&cp_rm))
				die(_("Submodule work tree '%s' contains local "
				      "modifications; use '-f' to discard them"),
				      displaypath);
		}

		strbuf_addstr(&sb_rm, path);

		אם (!remove_dir_recursively(&sb_rm, 0))
			format = _("Cleared directory '%s'\n");
		אחרת
			format = _("Could not remove submodule work tree '%s'\n");

		אם (!(flags & OPT_QUIET))
			printf(format, displaypath);

		submodule_unset_core_worktree(sub);

		strbuf_release(&sb_rm);
	}

	אם (mkdir(path, 0777))
		printf(_("could not create empty submodule directory %s"),
		      displaypath);

	cp_config.git_cmd = 1;
	strvec_pushl(&cp_config.args, "config", "--get-regexp", عدم);
	strvec_pushf(&cp_config.args, "submodule.%s\\.", sub->name);

	/* remove the .git/config entries (unless the user already did it) */
	אם (!capture_command(&cp_config, &sb_config, 0) && sb_config.len) {
		حرف *sub_key = xstrfmt("submodule.%s", sub->name);

		/*
		 * remove the whole section so we have a clean state when
		 * the user later decides to init this submodule again
		 */
		repo_config_rename_section_in_file(the_repository, عدم, sub_key, عدم);
		אם (!(flags & OPT_QUIET))
			printf(_("Submodule '%s' (%s) unregistered for path '%s'\n"),
				 sub->name, sub->url, displaypath);
		حرر(sub_key);
	}

cleanup:
	حرر(displaypath);
	حرر(sub_git_dir);
	strbuf_release(&sb_config);
}

ساكن فراغ deinit_submodule_cb(ثابت بنية cache_entry *list_item,
				فراغ *cb_data)
{
	بنية deinit_cb *info = cb_data;
	deinit_submodule(list_item->name, info->prefix, info->flags);
}

ساكن صحيح module_deinit(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix,
			 بنية repository *repo UNUSED)
{
	بنية deinit_cb info = DEINIT_CB_INIT;
	بنية pathspec pathspec = { 0 };
	بنية module_list list = MODULE_LIST_INIT;
	صحيح quiet = 0;
	صحيح force = 0;
	صحيح all = 0;
	بنية option module_deinit_options[] = {
		OPT__QUIET(&quiet, N_("suppress submodule status output")),
		OPT__FORCE(&force, N_("remove submodule working trees even if they contain local changes"), 0),
		OPT_BOOL(0, "all", &all, N_("unregister all submodules")),
		OPT_END()
	};
	ثابت حرف *ثابت git_submodule_helper_usage[] = {
		N_("git submodule deinit [--quiet] [-f | --force] [--all | [--] [<path>...]]"),
		عدم
	};
	صحيح ret = 1;

	argc = parse_options(argc, argv, prefix, module_deinit_options,
			     git_submodule_helper_usage, 0);

	אם (all && argc) {
		error("pathspec and --all are incompatible");
		usage_with_options(git_submodule_helper_usage,
				   module_deinit_options);
	}

	אם (!argc && !all)
		die(_("Use '--all' if you really want to deinitialize all submodules"));

	אם (module_list_compute(argv, prefix, &pathspec, &list) < 0)
		اذهب cleanup;

	info.prefix = prefix;
	אם (quiet)
		info.flags |= OPT_QUIET;
	אם (force)
		info.flags |= OPT_FORCE;

	for_each_listed_submodule(&list, deinit_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	החזר ret;
}

بنية module_clone_data {
	ثابت حرف *prefix;
	ثابت حرف *path;
	ثابت حرف *name;
	ثابت حرف *url;
	صحيح depth;
	بنية list_objects_filter_options *filter_options;
	تعداد ref_storage_format ref_storage_format;
	unsigned صحيح quiet: 1;
	unsigned صحيح progress: 1;
	unsigned صحيح dissociate: 1;
	unsigned صحيح require_init: 1;
	صحيح single_branch;
};
#define MODULE_CLONE_DATA_INIT { \
	.single_branch = -1, \
	.ref_storage_format = REF_STORAGE_FORMAT_UNKNOWN, \
}

بنية submodule_alternate_setup {
	ثابت حرف *submodule_name;
	تعداد SUBMODULE_ALTERNATE_ERROR_MODE {
		SUBMODULE_ALTERNATE_ERROR_DIE,
		SUBMODULE_ALTERNATE_ERROR_INFO,
		SUBMODULE_ALTERNATE_ERROR_IGNORE
	} error_mode;
	بنية string_list *reference;
};
#define SUBMODULE_ALTERNATE_SETUP_INIT { \
	.error_mode = SUBMODULE_ALTERNATE_ERROR_IGNORE, \
}

ساكن ثابت حرف alternate_error_advice[] = N_(
"An alternate computed from a superproject's alternate is invalid.\n"
"To allow Git to clone without an alternate in such a case, set\n"
"submodule.alternateErrorStrategy to 'info' or, equivalently, clone with\n"
"'--reference-if-able' instead of '--reference'."
);

ساكن صحيح add_possible_reference_from_superproject(
		بنية odb_files_dir *alt_odb, فراغ *sas_cb)
{
	بنية submodule_alternate_setup *sas = sas_cb;
	size_t len;

	/*
	 * If the alternate object store is another repository, try the
	 * standard layout with .git/(modules/<name>)+/objects
	 */
	אם (strip_suffix(alt_odb->abspath, "/objects", &len)) {
		بنية repository alternate;
		حرف *sm_alternate;
		بنية strbuf sb = STRBUF_INIT;
		بنية strbuf err = STRBUF_INIT;
		strbuf_add(&sb, alt_odb->abspath, len);

		אם (repo_init(&alternate, sb.buf, عدم) < 0)
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
		אם (sm_alternate) {
			حرف *p = strbuf_detach(&sb, عدم);

			string_list_append(sas->reference, p)->util = p;
			حرر(sm_alternate);
		} אחרת {
			بدل (sas->error_mode) {
			حالة SUBMODULE_ALTERNATE_ERROR_DIE:
				אם (advice_enabled(ADVICE_SUBMODULE_ALTERNATE_ERROR_STRATEGY_DIE))
					advise(_(alternate_error_advice));
				die(_("submodule '%s' cannot add alternate: %s"),
				    sas->submodule_name, err.buf);
			حالة SUBMODULE_ALTERNATE_ERROR_INFO:
				fprintf_ln(stderr, _("submodule '%s' cannot add alternate: %s"),
					sas->submodule_name, err.buf);
			حالة SUBMODULE_ALTERNATE_ERROR_IGNORE:
				; /* nothing */
			}
		}

		strbuf_release(&err);
		strbuf_release(&sb);
	}

	החזר 0;
}

ساكن فراغ prepare_possible_alternates(ثابت حرف *sm_name,
		بنية string_list *reference)
{
	حرف *sm_alternate = عدم, *error_strategy = عدم;
	بنية submodule_alternate_setup sas = SUBMODULE_ALTERNATE_SETUP_INIT;

	repo_config_get_string(the_repository, "submodule.alternateLocation", &sm_alternate);
	אם (!sm_alternate)
		החזר;

	repo_config_get_string(the_repository, "submodule.alternateErrorStrategy", &error_strategy);

	אם (!error_strategy)
		error_strategy = xstrdup("die");

	sas.submodule_name = sm_name;
	sas.reference = reference;
	אם (!strcmp(error_strategy, "die"))
		sas.error_mode = SUBMODULE_ALTERNATE_ERROR_DIE;
	אחרת אם (!strcmp(error_strategy, "info"))
		sas.error_mode = SUBMODULE_ALTERNATE_ERROR_INFO;
	אחרת אם (!strcmp(error_strategy, "ignore"))
		sas.error_mode = SUBMODULE_ALTERNATE_ERROR_IGNORE;
	אחרת
		die(_("Value '%s' for submodule.alternateErrorStrategy is not recognized"), error_strategy);

	אם (!strcmp(sm_alternate, "superproject"))
		odb_for_each_alternate(the_repository->objects,
				       add_possible_reference_from_superproject, &sas);
	אחרת אם (!strcmp(sm_alternate, "no"))
		; /* do nothing */
	אחרת
		die(_("Value '%s' for submodule.alternateLocation is not recognized"), sm_alternate);

	حرر(sm_alternate);
	حرر(error_strategy);
}

ساكن حرف *clone_submodule_sm_gitdir(ثابت حرف *name)
{
	بنية strbuf sb = STRBUF_INIT;
	حرف *sm_gitdir;

	submodule_name_to_gitdir(&sb, the_repository, name);
	sm_gitdir = absolute_pathdup(sb.buf);
	strbuf_release(&sb);

	החזר sm_gitdir;
}

ساكن صحيح dir_contains_only_dotgit(ثابت حرف *path)
{
	DIR *dir = opendir(path);
	بنية dirent *e;
	صحيح ret = 1;

	אם (!dir)
		החזר 0;

	e = readdir_skip_dot_and_dotdot(dir);
	אם (!e)
		ret = 0;
	אחרת אם (strcmp(DEFAULT_GIT_DIR_ENVIRONMENT, e->d_name) ||
		 (e = readdir_skip_dot_and_dotdot(dir))) {
		error("unexpected item '%s' in '%s'", e->d_name, path);
		ret = 0;
	}

	closedir(dir);
	החזר ret;
}

ساكن صحيح clone_submodule(ثابت بنية module_clone_data *clone_data,
			   بنية string_list *reference)
{
	حرف *p;
	حرف *sm_gitdir = clone_submodule_sm_gitdir(clone_data->name);
	حرف *sm_alternate = عدم, *error_strategy = عدم;
	بنية stat st;
	بنية child_process cp = CHILD_PROCESS_INIT;
	ثابت حرف *clone_data_path = clone_data->path;
	حرف *to_free = عدم;

	אם (validate_submodule_path(clone_data_path) < 0)
		die(عدم);

	אם (!is_absolute_path(clone_data->path))
		clone_data_path = to_free = xstrfmt("%s/%s", repo_get_work_tree(the_repository),
						    clone_data->path);

	אם (!file_exists(sm_gitdir)) {
		אם (clone_data->require_init && !stat(clone_data_path, &st) &&
		    !is_empty_dir(clone_data_path))
			die(_("directory not empty: '%s'"), clone_data_path);

		אם (safe_create_leading_directories_const(the_repository, sm_gitdir) < 0)
			die(_("could not create directory '%s'"), sm_gitdir);

		prepare_possible_alternates(clone_data->name, reference);

		strvec_push(&cp.args, "clone");
		strvec_push(&cp.args, "--no-checkout");
		אם (clone_data->quiet)
			strvec_push(&cp.args, "--quiet");
		אם (clone_data->progress)
			strvec_push(&cp.args, "--progress");
		אם (clone_data->depth > 0)
			strvec_pushf(&cp.args, "--depth=%d", clone_data->depth);
		אם (reference->nr) {
			بنية string_list_item *item;

			for_each_string_list_item(item, reference)
				strvec_pushl(&cp.args, "--reference",
					     item->string, عدم);
		}
		אם (clone_data->ref_storage_format != REF_STORAGE_FORMAT_UNKNOWN)
			strvec_pushf(&cp.args, "--ref-storage-format=%s",
				     ref_storage_format_to_name(clone_data->ref_storage_format));
		אם (clone_data->dissociate)
			strvec_push(&cp.args, "--dissociate");
		אם (sm_gitdir && *sm_gitdir)
			strvec_pushl(&cp.args, "--separate-git-dir", sm_gitdir, عدم);
		אם (clone_data->filter_options && clone_data->filter_options->choice)
			strvec_pushf(&cp.args, "--filter=%s",
				     expand_list_objects_filter_spec(
					     clone_data->filter_options));
		אם (clone_data->single_branch >= 0)
			strvec_push(&cp.args, clone_data->single_branch ?
				    "--single-branch" :
				    "--no-single-branch");

		strvec_push(&cp.args, "--");
		strvec_push(&cp.args, clone_data->url);
		strvec_push(&cp.args, clone_data_path);

		cp.git_cmd = 1;
		prepare_submodule_repo_env(&cp.env);
		cp.no_stdin = 1;

		אם(run_command(&cp))
			die(_("clone of '%s' into submodule path '%s' failed"),
			    clone_data->url, clone_data_path);

		אם (clone_data->require_init && !stat(clone_data_path, &st) &&
		    !dir_contains_only_dotgit(clone_data_path)) {
			حرف *dot_git = xstrfmt("%s/.git", clone_data_path);
			unlink(dot_git);
			حرر(dot_git);
			die(_("directory not empty: '%s'"), clone_data_path);
		}
	} אחרת {
		حرف *path;

		אם (clone_data->require_init && !stat(clone_data_path, &st) &&
		    !is_empty_dir(clone_data_path))
			die(_("directory not empty: '%s'"), clone_data_path);
		אם (safe_create_leading_directories_const(the_repository, clone_data_path) < 0)
			die(_("could not create directory '%s'"), clone_data_path);
		path = xstrfmt("%s/index", sm_gitdir);
		unlink_or_warn(path);
		حرر(path);
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
	אם (validate_submodule_git_dir(sm_gitdir, clone_data->name) < 0) {
		حرف *head = xstrfmt("%s/HEAD", sm_gitdir);
		unlink(head);
		حرر(head);
		die(_("refusing to create/use '%s' in another submodule's git dir. "
		      "Enabling extensions.submodulePathConfig should fix this."),
		    sm_gitdir);
	}

	connect_work_tree_and_git_dir(clone_data_path, sm_gitdir, 0);

	p = repo_submodule_path(the_repository, clone_data_path, "config");
	אם (!p)
		die(_("could not get submodule directory for '%s'"), clone_data_path);

	/* setup alternateLocation and alternateErrorStrategy in the cloned submodule if needed */
	repo_config_get_string(the_repository, "submodule.alternateLocation", &sm_alternate);
	אם (sm_alternate)
		repo_config_set_in_file(the_repository, p, "submodule.alternateLocation",
					sm_alternate);
	repo_config_get_string(the_repository, "submodule.alternateErrorStrategy", &error_strategy);
	אם (error_strategy)
		repo_config_set_in_file(the_repository, p, "submodule.alternateErrorStrategy",
					error_strategy);

	حرر(sm_alternate);
	حرر(error_strategy);

	حرر(sm_gitdir);
	حرر(p);
	حرر(to_free);
	החזר 0;
}

ساكن صحيح module_clone(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix,
			بنية repository *repo UNUSED)
{
	صحيح dissociate = 0, quiet = 0, progress = 0, require_init = 0;
	بنية module_clone_data clone_data = MODULE_CLONE_DATA_INIT;
	بنية string_list reference = STRING_LIST_INIT_NODUP;
	بنية list_objects_filter_options filter_options =
		LIST_OBJECTS_FILTER_INIT;
	ثابت حرف *ref_storage_format = عدم;

	بنية option module_clone_options[] = {
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
	ثابت حرف *ثابت git_submodule_helper_usage[] = {
		N_("git submodule--helper clone [--prefix=<path>] [--quiet] "
		   "[--reference <repository>] [--name <name>] [--depth <depth>] "
		   "[--single-branch] [--filter <filter-spec>] "
		   "--url <url> --path <path>"),
		عدم
	};

	argc = parse_options(argc, argv, prefix, module_clone_options,
			     git_submodule_helper_usage, 0);

	אם (ref_storage_format) {
		clone_data.ref_storage_format = ref_storage_format_by_name(ref_storage_format);
		אם (clone_data.ref_storage_format == REF_STORAGE_FORMAT_UNKNOWN)
			die(_("unknown ref storage format '%s'"), ref_storage_format);
	}
	clone_data.dissociate = !!dissociate;
	clone_data.quiet = !!quiet;
	clone_data.progress = !!progress;
	clone_data.require_init = !!require_init;
	clone_data.filter_options = &filter_options;

	אם (argc || !clone_data.url || !clone_data.path || !*(clone_data.path))
		usage_with_options(git_submodule_helper_usage,
				   module_clone_options);

	clone_submodule(&clone_data, &reference);
	list_objects_filter_release(&filter_options);
	string_list_clear(&reference, 1);
	החזר 0;
}

ساكن صحيح determine_submodule_update_strategy(بنية repository *r,
					       صحيح just_cloned,
					       ثابت حرف *path,
					       تعداد submodule_update_type update,
					       بنية submodule_update_strategy *out)
{
	ثابت بنية submodule *sub = submodule_from_path(r, null_oid(the_hash_algo), path);
	حرف *key;
	ثابت حرف *val;
	صحيح ret;

	/*
	 * NEEDSWORK: audit and ensure that update_submodule() has right
	 * to assume that submodule_from_path() above will always succeed.
	 */
	אם (!sub)
		BUG("update_submodule assumes a submodule exists at path (%s)",
		    path);
	key = xstrfmt("submodule.%s.update", sub->name);

	אם (update) {
		out->type = update;
	} אחרת אם (!repo_config_get_string_tmp(r, key, &val)) {
		אם (parse_submodule_update_strategy(val, out) < 0) {
			ret = die_message(_("Invalid update mode '%s' configured for submodule path '%s'"),
					  val, path);
			اذهب cleanup;
		}
	} אחרת אם (sub->update_strategy.type != SM_UPDATE_UNSPECIFIED) {
		אם (sub->update_strategy.type == SM_UPDATE_COMMAND)
			BUG("how did we read update = !command from .gitmodules?");
		out->type = sub->update_strategy.type;
		out->command = sub->update_strategy.command;
	} אחרת
		out->type = SM_UPDATE_CHECKOUT;

	אם (just_cloned &&
	    (out->type == SM_UPDATE_MERGE ||
	     out->type == SM_UPDATE_REBASE ||
	     out->type == SM_UPDATE_NONE))
		out->type = SM_UPDATE_CHECKOUT;

	ret = 0;
cleanup:
	حرر(key);
	החזר ret;
}

بنية update_clone_data {
	ثابت بنية submodule *sub;
	بنية object_id oid;
	unsigned just_cloned;
};

بنية submodule_update_clone {
	/* index into 'update_data.list', the list of submodules to look into for cloning */
	صحيح current;

	/* configuration parameters which are passed on to the children */
	ثابت بنية update_data *update_data;

	/* to be consumed by update_submodule() */
	بنية update_clone_data *update_clone;
	صحيح update_clone_nr; صحيح update_clone_alloc;

	/* If we want to stop as fast as possible and return an error */
	unsigned quickstop : 1;

	/* failed clones to be retried again */
	ثابت بنية cache_entry **failed_clones;
	صحيح failed_clones_nr, failed_clones_alloc;
};
#define SUBMODULE_UPDATE_CLONE_INIT { 0 }

ساكن فراغ submodule_update_clone_release(بنية submodule_update_clone *suc)
{
	حرر(suc->update_clone);
	حرر(suc->failed_clones);
}

بنية update_data {
	ثابت حرف *prefix;
	ثابت حرف *super_prefix;
	حرف *displaypath;
	تعداد submodule_update_type update_default;
	بنية object_id suboid;
	بنية string_list references;
	بنية submodule_update_strategy update_strategy;
	بنية list_objects_filter_options *filter_options;
	بنية module_list list;
	تعداد ref_storage_format ref_storage_format;
	صحيح depth;
	صحيح max_jobs;
	صحيح single_branch;
	صحيح recommend_shallow;
	unsigned صحيح require_init;
	unsigned صحيح force;
	unsigned صحيح quiet;
	unsigned صحيح nofetch;
	unsigned صحيح remote;
	unsigned صحيح progress;
	unsigned صحيح dissociate;
	unsigned صحيح init;
	unsigned صحيح warn_if_uninitialized;
	unsigned صحيح recursive;

	/* copied over from update_clone_data */
	بنية object_id oid;
	unsigned صحيح just_cloned;
	ثابت حرف *sm_path;
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

ساكن فراغ update_data_release(بنية update_data *ud)
{
	حرر(ud->displaypath);
	submodule_update_strategy_release(&ud->update_strategy);
	module_list_release(&ud->list);
}

ساكن فراغ next_submodule_warn_missing(بنية submodule_update_clone *suc,
		بنية strbuf *out, ثابت حرف *displaypath)
{
	/*
	 * Only mention uninitialized submodules when their
	 * paths have been specified.
	 */
	אם (suc->update_data->warn_if_uninitialized) {
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
ساكن صحيح prepare_to_clone_next_submodule(ثابت بنية cache_entry *ce,
					   بنية child_process *child,
					   بنية submodule_update_clone *suc,
					   بنية strbuf *out)
{
	ثابت بنية submodule *sub = عدم;
	ثابت حرف *url = عدم;
	ثابت حرف *update_string;
	تعداد submodule_update_type update_type;
	حرف *key;
	ثابت بنية update_data *ud = suc->update_data;
	حرف *displaypath = get_submodule_displaypath(ce->name, ud->prefix,
						      ud->super_prefix);
	بنية strbuf sb = STRBUF_INIT;
	صحيح needs_cloning = 0;
	صحيح need_free_url = 0;

	אם (ce_stage(ce)) {
		strbuf_addf(out, _("Skipping unmerged submodule %s"), displaypath);
		strbuf_addch(out, '\n');
		اذهب cleanup;
	}

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), ce->name);

	אם (!sub) {
		next_submodule_warn_missing(suc, out, displaypath);
		اذهب cleanup;
	}

	key = xstrfmt("submodule.%s.update", sub->name);
	אם (!repo_config_get_string_tmp(the_repository, key, &update_string)) {
		update_type = parse_submodule_update_type(update_string);
	} אחרת {
		update_type = sub->update_strategy.type;
	}
	حرر(key);

	אם (suc->update_data->update_strategy.type == SM_UPDATE_NONE
	    || (suc->update_data->update_strategy.type == SM_UPDATE_UNSPECIFIED
		&& update_type == SM_UPDATE_NONE)) {
		strbuf_addf(out, _("Skipping submodule '%s'"), displaypath);
		strbuf_addch(out, '\n');
		اذهب cleanup;
	}

	/* Check if the submodule has been initialized. */
	אם (!is_submodule_active(the_repository, ce->name)) {
		next_submodule_warn_missing(suc, out, displaypath);
		اذهب cleanup;
	}

	strbuf_reset(&sb);
	strbuf_addf(&sb, "submodule.%s.url", sub->name);
	אם (repo_config_get_string_tmp(the_repository, sb.buf, &url)) {
		אם (sub->url && (starts_with_dot_slash(sub->url) ||
				 starts_with_dot_dot_slash(sub->url))) {
			url = resolve_relative_url(sub->url, عدم, 0);
			need_free_url = 1;
		} אחרת
			url = sub->url;
	}

	אם (!url)
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

	אם (!needs_cloning)
		اذهب cleanup;

	child->git_cmd = 1;
	child->no_stdin = 1;
	child->stdout_to_stderr = 1;
	child->err = -1;
	strvec_push(&child->args, "submodule--helper");
	strvec_push(&child->args, "clone");
	אם (suc->update_data->progress)
		strvec_push(&child->args, "--progress");
	אם (suc->update_data->quiet)
		strvec_push(&child->args, "--quiet");
	אם (suc->update_data->prefix)
		strvec_pushl(&child->args, "--prefix", suc->update_data->prefix, عدم);
	אם (suc->update_data->recommend_shallow && sub->recommend_shallow == 1)
		strvec_push(&child->args, "--depth=1");
	אחרת אם (suc->update_data->depth)
		strvec_pushf(&child->args, "--depth=%d", suc->update_data->depth);
	אם (suc->update_data->filter_options && suc->update_data->filter_options->choice)
		strvec_pushf(&child->args, "--filter=%s",
			     expand_list_objects_filter_spec(suc->update_data->filter_options));
	אם (suc->update_data->require_init)
		strvec_push(&child->args, "--require-init");
	אם (suc->update_data->ref_storage_format != REF_STORAGE_FORMAT_UNKNOWN)
		strvec_pushf(&child->args, "--ref-storage-format=%s",
			     ref_storage_format_to_name(suc->update_data->ref_storage_format));
	strvec_pushl(&child->args, "--path", sub->path, عدم);
	strvec_pushl(&child->args, "--name", sub->name, عدم);
	strvec_pushl(&child->args, "--url", url, عدم);
	אם (suc->update_data->references.nr) {
		بنية string_list_item *item;

		for_each_string_list_item(item, &suc->update_data->references)
			strvec_pushl(&child->args, "--reference", item->string, عدم);
	}
	אם (suc->update_data->dissociate)
		strvec_push(&child->args, "--dissociate");
	אם (suc->update_data->single_branch >= 0)
		strvec_push(&child->args, suc->update_data->single_branch ?
					      "--single-branch" :
					      "--no-single-branch");

cleanup:
	حرر(displaypath);
	strbuf_release(&sb);
	אם (need_free_url)
		حرر((فراغ*)url);

	החזר needs_cloning;
}

ساكن صحيح update_clone_get_next_task(بنية child_process *child,
				      بنية strbuf *err,
				      فراغ *suc_cb,
				      فراغ **idx_task_cb)
{
	بنية submodule_update_clone *suc = suc_cb;
	ثابت بنية cache_entry *ce;
	صحيح index;

	עבור (; suc->current < suc->update_data->list.nr; suc->current++) {
		ce = suc->update_data->list.entries[suc->current];
		אם (prepare_to_clone_next_submodule(ce, child, suc, err)) {
			صحيح *p = xmalloc(حجم(*p));

			*p = suc->current;
			*idx_task_cb = p;
			suc->current++;
			החזר 1;
		}
	}

	/*
	 * The loop above tried cloning each submodule once, now try the
	 * stragglers again, which we can imagine as an extension of the
	 * entry list.
	 */
	index = suc->current - suc->update_data->list.nr;
	אם (index < suc->failed_clones_nr) {
		صحيح *p;

		ce = suc->failed_clones[index];
		אם (!prepare_to_clone_next_submodule(ce, child, suc, err)) {
			suc->current ++;
			strbuf_addstr(err, "BUG: submodule considered for "
					   "cloning, doesn't need cloning "
					   "any more?\n");
			החזר 0;
		}
		p = xmalloc(حجم(*p));
		*p = suc->current;
		*idx_task_cb = p;
		suc->current ++;
		החזר 1;
	}

	החזר 0;
}

ساكن صحيح update_clone_start_failure(بنية strbuf *err UNUSED,
				      فراغ *suc_cb,
				      فراغ *idx_task_cb UNUSED)
{
	بنية submodule_update_clone *suc = suc_cb;

	suc->quickstop = 1;
	החזר 1;
}

ساكن صحيح update_clone_task_finished(صحيح result,
				      بنية strbuf *err,
				      فراغ *suc_cb,
				      فراغ *idx_task_cb)
{
	ثابت بنية cache_entry *ce;
	بنية submodule_update_clone *suc = suc_cb;
	صحيح *idxP = idx_task_cb;
	صحيح idx = *idxP;

	حرر(idxP);

	אם (!result)
		החזר 0;

	אם (idx < suc->update_data->list.nr) {
		ce  = suc->update_data->list.entries[idx];
		strbuf_addf(err, _("Failed to clone '%s'. Retry scheduled"),
			    ce->name);
		strbuf_addch(err, '\n');
		ALLOC_GROW(suc->failed_clones,
			   suc->failed_clones_nr + 1,
			   suc->failed_clones_alloc);
		suc->failed_clones[suc->failed_clones_nr++] = ce;
		החזר 0;
	} אחרת {
		idx -= suc->update_data->list.nr;
		ce  = suc->failed_clones[idx];
		strbuf_addf(err, _("Failed to clone '%s' a second time, aborting"),
			    ce->name);
		strbuf_addch(err, '\n');
		suc->quickstop = 1;
		החזר 1;
	}

	החזר 0;
}

ساكن صحيح git_update_clone_config(ثابت حرف *var, ثابت حرف *value,
				   ثابت بنية config_context *ctx,
				   فراغ *cb)
{
	صحيح *max_jobs = cb;

	אם (!strcmp(var, "submodule.fetchjobs"))
		*max_jobs = parse_submodule_fetchjobs(var, value, ctx->kvi);
	החזר 0;
}

ساكن صحيح is_tip_reachable(ثابت حرف *path, ثابت بنية object_id *oid)
{
	بنية child_process cp = CHILD_PROCESS_INIT;
	بنية strbuf rev = STRBUF_INIT;
	حرف *hex = oid_to_hex(oid);
	صحيح reachable;

	cp.git_cmd = 1;
	cp.dir = path;
	cp.no_stderr = 1;
	strvec_pushl(&cp.args, "rev-list", "-n", "1", hex, "--not", "--all", عدم);

	prepare_submodule_repo_env(&cp.env);

	אם (capture_command(&cp, &rev, GIT_MAX_HEXSZ + 1) || rev.len)
		reachable = 0;
	אחרת
		reachable = 1;

	strbuf_release(&rev);
	החזר reachable;
}

ساكن صحيح fetch_in_submodule(ثابت حرف *module_path, صحيح depth, صحيح quiet,
			      ثابت بنية object_id *oid)
{
	بنية child_process cp = CHILD_PROCESS_INIT;

	prepare_submodule_repo_env(&cp.env);
	cp.git_cmd = 1;
	cp.dir = module_path;

	strvec_push(&cp.args, "fetch");
	אם (quiet)
		strvec_push(&cp.args, "--quiet");
	אם (depth)
		strvec_pushf(&cp.args, "--depth=%d", depth);
	אם (oid) {
		حرف *hex = oid_to_hex(oid);
		حرف *remote;
		صحيح code;

		code = get_default_remote_submodule(module_path, &remote);
		אם (code) {
			child_process_clear(&cp);
			החזר code;
		}

		strvec_pushl(&cp.args, remote, hex, عدم);
		حرر(remote);
	}

	החזר run_command(&cp);
}

ساكن صحيح run_update_command(ثابت بنية update_data *ud, صحيح subforce)
{
	بنية child_process cp = CHILD_PROCESS_INIT;
	حرف *oid = oid_to_hex(&ud->oid);
	صحيح ret;

	بدل (ud->update_strategy.type) {
	حالة SM_UPDATE_CHECKOUT:
		cp.git_cmd = 1;
		strvec_pushl(&cp.args, "checkout", "-q", عدم);
		אם (subforce)
			strvec_push(&cp.args, "-f");
		اكسر;
	حالة SM_UPDATE_REBASE:
		cp.git_cmd = 1;
		strvec_push(&cp.args, "rebase");
		אם (ud->quiet)
			strvec_push(&cp.args, "--quiet");
		اكسر;
	حالة SM_UPDATE_MERGE:
		cp.git_cmd = 1;
		strvec_push(&cp.args, "merge");
		אם (ud->quiet)
			strvec_push(&cp.args, "--quiet");
		اكسر;
	حالة SM_UPDATE_COMMAND:
		cp.use_shell = 1;
		strvec_push(&cp.args, ud->update_strategy.command);
		اكسر;
	افتراضي:
		BUG("unexpected update strategy type: %d",
		    ud->update_strategy.type);
	}
	strvec_push(&cp.args, oid);

	cp.dir = ud->sm_path;
	prepare_submodule_repo_env(&cp.env);
	אם ((ret = run_command(&cp))) {
		بدل (ud->update_strategy.type) {
		حالة SM_UPDATE_CHECKOUT:
			die_message(_("Unable to checkout '%s' in submodule path '%s'"),
				    oid, ud->displaypath);
			/* No "ret" assignment, use "git checkout"'s */
			اكسر;
		حالة SM_UPDATE_REBASE:
			ret = die_message(_("Unable to rebase '%s' in submodule path '%s'"),
					  oid, ud->displaypath);
			اكسر;
		حالة SM_UPDATE_MERGE:
			ret = die_message(_("Unable to merge '%s' in submodule path '%s'"),
					  oid, ud->displaypath);
			اكسر;
		حالة SM_UPDATE_COMMAND:
			ret = die_message(_("Execution of '%s %s' failed in submodule path '%s'"),
					  ud->update_strategy.command, oid, ud->displaypath);
			اكسر;
		افتراضي:
			BUG("unexpected update strategy type: %d",
			    ud->update_strategy.type);
		}

		החזר ret;
	}

	אם (ud->quiet)
		החזר 0;

	بدل (ud->update_strategy.type) {
	حالة SM_UPDATE_CHECKOUT:
		printf(_("Submodule path '%s': checked out '%s'\n"),
		       ud->displaypath, oid);
		اكسر;
	حالة SM_UPDATE_REBASE:
		printf(_("Submodule path '%s': rebased into '%s'\n"),
		       ud->displaypath, oid);
		اكسر;
	حالة SM_UPDATE_MERGE:
		printf(_("Submodule path '%s': merged in '%s'\n"),
		       ud->displaypath, oid);
		اكسر;
	حالة SM_UPDATE_COMMAND:
		printf(_("Submodule path '%s': '%s %s'\n"),
		       ud->displaypath, ud->update_strategy.command, oid);
		اكسر;
	افتراضي:
		BUG("unexpected update strategy type: %d",
		    ud->update_strategy.type);
	}

	החזר 0;
}

ساكن صحيح run_update_procedure(ثابت بنية update_data *ud)
{
	صحيح subforce = is_null_oid(&ud->suboid) || ud->force;

	אם (!ud->nofetch) {
		/*
		 * Run fetch only if `oid` isn't present or it
		 * is not reachable from a ref.
		 */
		אם (!is_tip_reachable(ud->sm_path, &ud->oid) &&
		    fetch_in_submodule(ud->sm_path, ud->depth, ud->quiet, عدم) &&
		    !ud->quiet)
			fprintf_ln(stderr,
				   _("Unable to fetch in submodule path '%s'; "
				     "trying to directly fetch %s:"),
				   ud->displaypath, oid_to_hex(&ud->oid));
		/*
		 * Now we tried the usual fetch, but `oid` may
		 * not be reachable from any of the refs.
		 */
		אם (!is_tip_reachable(ud->sm_path, &ud->oid) &&
		    fetch_in_submodule(ud->sm_path, ud->depth, ud->quiet, &ud->oid))
			החזר die_message(_("Fetched in submodule path '%s', but it did not "
					     "contain %s. Direct fetching of that commit failed."),
					   ud->displaypath, oid_to_hex(&ud->oid));
	}

	החזר run_update_command(ud, subforce);
}

ساكن صحيح remote_submodule_branch(ثابت حرف *path, ثابت حرف **branch)
{
	ثابت بنية submodule *sub;
	حرف *key;
	*branch = عدم;

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);
	אם (!sub)
		החזר die_message(_("could not initialize submodule at path '%s'"),
				   path);

	key = xstrfmt("submodule.%s.branch", sub->name);
	אם (repo_config_get_string_tmp(the_repository, key, branch))
		*branch = sub->branch;
	حرر(key);

	אם (!*branch) {
		*branch = "HEAD";
		החזר 0;
	}

	אם (!strcmp(*branch, ".")) {
		ثابت حرف *refname = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
							      "HEAD", 0, عدم,
							      عدم);

		אם (!refname)
			החזר die_message(_("No such ref: %s"), "HEAD");

		/* detached HEAD */
		אם (!strcmp(refname, "HEAD"))
			החזר die_message(_("Submodule (%s) branch configured to inherit "
					     "branch from superproject, but the superproject "
					     "is not on any branch"), sub->name);

		אם (!skip_prefix(refname, "refs/heads/", &refname))
			החזר die_message(_("Expecting a full ref name, got %s"),
					   refname);

		*branch = refname;
		החזר 0;
	}

	/* Our "branch" is coming from repo_config_get_string_tmp() */
	החזר 0;
}

ساكن صحيح ensure_core_worktree(ثابت حرف *path)
{
	ثابت حرف *cw;
	بنية repository subrepo;

	אם (repo_submodule_init(&subrepo, the_repository, path, null_oid(the_hash_algo)))
		החזר die_message(_("could not get a repository handle for submodule '%s'"),
				   path);

	אם (!repo_config_get_string_tmp(&subrepo, "core.worktree", &cw)) {
		حرف *cfg_file, *abs_path;
		ثابت حرف *rel_path;
		بنية strbuf sb = STRBUF_INIT;

		cfg_file = repo_git_path(&subrepo, "config");

		abs_path = absolute_pathdup(path);
		rel_path = relative_path(abs_path, subrepo.gitdir, &sb);

		repo_config_set_in_file(the_repository, cfg_file, "core.worktree", rel_path);

		حرر(cfg_file);
		حرر(abs_path);
		strbuf_release(&sb);
	}

	repo_clear(&subrepo);
	החזר 0;
}

ساكن ثابت حرف *submodule_update_type_to_label(تعداد submodule_update_type type)
{
	بدل (type) {
	حالة SM_UPDATE_CHECKOUT:
		החזר "checkout";
	حالة SM_UPDATE_MERGE:
		החזר "merge";
	حالة SM_UPDATE_REBASE:
		החזר "rebase";
	حالة SM_UPDATE_UNSPECIFIED:
	حالة SM_UPDATE_NONE:
	حالة SM_UPDATE_COMMAND:
		اكسر;
	}
	BUG("unreachable with type %d", type);
}

ساكن فراغ update_data_to_args(ثابت بنية update_data *update_data,
				بنية strvec *args)
{
	تعداد submodule_update_type update_type = update_data->update_default;

	strvec_pushl(args, "submodule--helper", "update", "--recursive", عدم);
	אם (update_data->displaypath)
		strvec_pushf(args, "--super-prefix=%s/",
			     update_data->displaypath);
	strvec_pushf(args, "--jobs=%d", update_data->max_jobs);
	אם (update_data->quiet)
		strvec_push(args, "--quiet");
	אם (update_data->force)
		strvec_push(args, "--force");
	אם (update_data->init)
		strvec_push(args, "--init");
	אם (update_data->remote)
		strvec_push(args, "--remote");
	אם (update_data->nofetch)
		strvec_push(args, "--no-fetch");
	אם (update_data->dissociate)
		strvec_push(args, "--dissociate");
	אם (update_data->progress)
		strvec_push(args, "--progress");
	אם (update_data->require_init)
		strvec_push(args, "--require-init");
	אם (update_data->depth)
		strvec_pushf(args, "--depth=%d", update_data->depth);
	אם (update_type != SM_UPDATE_UNSPECIFIED)
		strvec_pushf(args, "--%s",
			     submodule_update_type_to_label(update_type));

	אם (update_data->references.nr) {
		بنية string_list_item *item;

		for_each_string_list_item(item, &update_data->references)
			strvec_pushl(args, "--reference", item->string, عدم);
	}
	אם (update_data->ref_storage_format != REF_STORAGE_FORMAT_UNKNOWN)
		strvec_pushf(args, "--ref-storage-format=%s",
			     ref_storage_format_to_name(update_data->ref_storage_format));
	אם (update_data->filter_options && update_data->filter_options->choice)
		strvec_pushf(args, "--filter=%s",
				expand_list_objects_filter_spec(
					update_data->filter_options));
	אם (update_data->recommend_shallow == 0)
		strvec_push(args, "--no-recommend-shallow");
	אחרת אם (update_data->recommend_shallow == 1)
		strvec_push(args, "--recommend-shallow");
	אם (update_data->single_branch >= 0)
		strvec_push(args, update_data->single_branch ?
				    "--single-branch" :
				    "--no-single-branch");
}

ساكن صحيح update_submodule(بنية update_data *update_data)
{
	صحيح ret;

	אם (validate_submodule_path(update_data->sm_path) < 0)
		החזר -1;

	ret = determine_submodule_update_strategy(the_repository,
						  update_data->just_cloned,
						  update_data->sm_path,
						  update_data->update_default,
						  &update_data->update_strategy);
	אם (ret)
		החזר ret;

	אם (update_data->just_cloned)
		oidcpy(&update_data->suboid, null_oid(the_hash_algo));
	אחרת אם (repo_resolve_gitlink_ref(the_repository, update_data->sm_path,
					  "HEAD", &update_data->suboid))
		החזר die_message(_("Unable to find current revision in submodule path '%s'"),
				   update_data->displaypath);

	אם (update_data->remote) {
		حرف *remote_name;
		ثابت حرف *branch;
		حرف *remote_ref;
		صحيح code;

		code = get_default_remote_submodule(update_data->sm_path, &remote_name);
		אם (code)
			החזר code;
		code = remote_submodule_branch(update_data->sm_path, &branch);
		אם (code) {
			حرر(remote_name);
			החזר code;
		}
		remote_ref = xstrfmt("refs/remotes/%s/%s", remote_name, branch);

		حرر(remote_name);

		אם (!update_data->nofetch) {
			אם (fetch_in_submodule(update_data->sm_path, update_data->depth,
					      0, عدم)) {
				حرر(remote_ref);
				החזר die_message(_("Unable to fetch in submodule path '%s'"),
						   update_data->sm_path);
			}
		}

		אם (repo_resolve_gitlink_ref(the_repository, update_data->sm_path,
					     remote_ref, &update_data->oid)) {
			ret = die_message(_("Unable to find %s revision in submodule path '%s'"),
					  remote_ref, update_data->sm_path);
			حرر(remote_ref);
			החזר ret;
		}

		حرر(remote_ref);
	}

	אם (!oideq(&update_data->oid, &update_data->suboid) || update_data->force) {
		ret = run_update_procedure(update_data);
		אם (ret)
			החזר ret;
	}

	אם (update_data->recursive) {
		بنية child_process cp = CHILD_PROCESS_INIT;
		بنية update_data next = *update_data;

		next.prefix = عدم;
		oidcpy(&next.oid, null_oid(the_hash_algo));
		oidcpy(&next.suboid, null_oid(the_hash_algo));

		cp.dir = update_data->sm_path;
		cp.git_cmd = 1;
		prepare_submodule_repo_env(&cp.env);
		update_data_to_args(&next, &cp.args);

		ret = run_command(&cp);
		אם (ret)
			die_message(_("Failed to recurse into submodule path '%s'"),
				    update_data->displaypath);
		החזר ret;
	}

	החזר 0;
}

ساكن صحيح update_submodules(بنية update_data *update_data)
{
	صحيح i, ret = 0;
	بنية submodule_update_clone suc = SUBMODULE_UPDATE_CLONE_INIT;
	ثابت بنية run_process_parallel_opts opts = {
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
	אם (suc.quickstop) {
		ret = 1;
		اذهب cleanup;
	}

	עבור (i = 0; i < suc.update_clone_nr; i++) {
		بنية update_clone_data ucd = suc.update_clone[i];
		صحيح code = 128;

		oidcpy(&update_data->oid, &ucd.oid);
		update_data->just_cloned = ucd.just_cloned;
		update_data->sm_path = ucd.sub->path;

		/*
		 * Verify that the submodule path does not contain any
		 * symlinks; if it does, it might have been tampered with.
		 * TODO: allow exempting it via
		 * `safe.submodule.path` or something
		 */
		אם (validate_submodule_path(update_data->sm_path) < 0)
			اذهب fail;

		code = ensure_core_worktree(update_data->sm_path);
		אם (code)
			اذهب fail;

		update_data->displaypath = get_submodule_displaypath(
			update_data->sm_path, update_data->prefix,
			update_data->super_prefix);
		code = update_submodule(update_data);
		FREE_AND_NULL(update_data->displaypath);
fail:
		אם (!code)
			استمر;
		ret = code;
		אם (ret == 128)
			اذهب cleanup;
	}

cleanup:
	submodule_update_clone_release(&suc);
	string_list_clear(&update_data->references, 0);
	החזר ret;
}

ساكن صحيح module_update(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix,
			 بنية repository *repo UNUSED)
{
	بنية pathspec pathspec = { 0 };
	بنية pathspec pathspec2 = { 0 };
	بنية update_data opt = UPDATE_DATA_INIT;
	بنية list_objects_filter_options filter_options =
		LIST_OBJECTS_FILTER_INIT;
	ثابت حرف *ref_storage_format = عدم;
	صحيح ret;
	بنية option module_update_options[] = {
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
	ثابت حرف *ثابت git_submodule_helper_usage[] = {
		N_("git submodule [--quiet] update"
		" [--init [--filter=<filter-spec>]] [--remote]"
		" [-N|--no-fetch] [-f|--force]"
		" [--checkout|--merge|--rebase]"
		" [--[no-]recommend-shallow] [--reference <repository>]"
		" [--recursive] [--[no-]single-branch] [--] [<path>...]"),
		عدم
	};

	update_clone_config_from_gitmodules(the_repository, &opt.max_jobs);
	repo_config(the_repository, git_update_clone_config, &opt.max_jobs);

	argc = parse_options(argc, argv, prefix, module_update_options,
			     git_submodule_helper_usage, 0);

	אם (opt.require_init)
		opt.init = 1;

	אם (filter_options.choice && !opt.init) {
		usage_with_options(git_submodule_helper_usage,
				   module_update_options);
	}

	אם (ref_storage_format) {
		opt.ref_storage_format = ref_storage_format_by_name(ref_storage_format);
		אם (opt.ref_storage_format == REF_STORAGE_FORMAT_UNKNOWN)
			die(_("unknown ref storage format '%s'"), ref_storage_format);
	}

	opt.filter_options = &filter_options;
	opt.prefix = prefix;

	אם (opt.update_default)
		opt.update_strategy.type = opt.update_default;

	אם (module_list_compute(argv, prefix, &pathspec, &opt.list) < 0) {
		ret = 1;
		اذهب cleanup;
	}

	אם (pathspec.nr)
		opt.warn_if_uninitialized = 1;

	אם (opt.init) {
		بنية module_list list = MODULE_LIST_INIT;
		بنية init_cb info = INIT_CB_INIT;

		אם (module_list_compute(argv, opt.prefix,
					&pathspec2, &list) < 0) {
			module_list_release(&list);
			ret = 1;
			اذهب cleanup;
		}

		/*
		 * If there are no path args and submodule.active is set then,
		 * by default, only initialize 'active' modules.
		 */
		אם (!argc && !repo_config_get(the_repository, "submodule.active"))
			module_list_active(&list);

		info.prefix = opt.prefix;
		info.super_prefix = opt.super_prefix;
		אם (opt.quiet)
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
	החזר ret;
}

ساكن صحيح push_check(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix UNUSED,
		      بنية repository *repo UNUSED)
{
	بنية remote *remote;
	ثابت حرف *superproject_head;
	حرف *head;
	صحيح detached_head = 0;
	بنية object_id head_oid;

	אם (argc < 3)
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
				   0, &head_oid, عدم);
	אם (!head)
		die(_("Failed to resolve HEAD as a valid ref."));
	אם (!strcmp(head, "HEAD"))
		detached_head = 1;

	/*
	 * The remote must be configured.
	 * This is to avoid pushing to the exact same URL as the parent.
	 */
	remote = pushremote_get(argv[1]);
	אם (!remote || remote->origin == REMOTE_UNCONFIGURED)
		die("remote '%s' not configured", argv[1]);

	/* Check the refspec */
	אם (argc > 2) {
		صحيح i;
		بنية ref *local_refs = get_local_heads();
		بنية refspec refspec = REFSPEC_INIT_PUSH(the_hash_algo);

		refspec_appendn(&refspec, argv + 2, argc - 2);

		עבור (i = 0; i < refspec.nr; i++) {
			ثابت بنية refspec_item *rs = &refspec.items[i];

			אם (rs->pattern || rs->matching)
				استمر;

			/* LHS must match a single ref */
			بدل (count_refspec_match(rs->src, local_refs, عدم)) {
			حالة 1:
				اكسر;
			حالة 0:
				/*
				 * If LHS matches 'HEAD' then we need to ensure
				 * that it matches the same named branch
				 * checked out in the superproject.
				 */
				אם (!strcmp(rs->src, "HEAD")) {
					אם (!detached_head &&
					    !strcmp(head, superproject_head))
						اكسر;
					die("HEAD does not match the named branch in the superproject");
				}
				/* fallthrough */
			افتراضي:
				die("src refspec '%s' must name a ref",
				    rs->src);
			}
		}

		refspec_clear(&refspec);
		free_refs(local_refs);
	}
	حرر(head);

	החזר 0;
}

ساكن صحيح absorb_git_dirs(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix,
			   بنية repository *repo UNUSED)
{
	صحيح i;
	بنية pathspec pathspec = { 0 };
	بنية module_list list = MODULE_LIST_INIT;
	ثابت حرف *super_prefix = عدم;
	بنية option embed_gitdir_options[] = {
		OPT__SUPER_PREFIX(&super_prefix),
		OPT_END()
	};
	ثابت حرف *ثابت git_submodule_helper_usage[] = {
		N_("git submodule absorbgitdirs [<options>] [<path>...]"),
		عدم
	};
	صحيح ret = 1;

	argc = parse_options(argc, argv, prefix, embed_gitdir_options,
			     git_submodule_helper_usage, 0);

	אם (module_list_compute(argv, prefix, &pathspec, &list) < 0)
		اذهب cleanup;

	עבור (i = 0; i < list.nr; i++)
		absorb_git_dir_into_superproject(list.entries[i]->name,
						 super_prefix);

	ret = 0;
cleanup:
	clear_pathspec(&pathspec);
	module_list_release(&list);
	החזר ret;
}

ساكن صحيح module_set_url(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix,
			  بنية repository *repo UNUSED)
{
	صحيح quiet = 0, ret;
	ثابت حرف *newurl;
	ثابت حرف *path;
	حرف *config_name;
	بنية option options[] = {
		OPT__QUIET(&quiet, N_("suppress output for setting url of a submodule")),
		OPT_END()
	};
	ثابت حرف *ثابت usage[] = {
		N_("git submodule set-url [--quiet] <path> <newurl>"),
		عدم
	};
	ثابت بنية submodule *sub;

	argc = parse_options(argc, argv, prefix, options, usage, 0);

	אם (argc != 2 || !(path = argv[0]) || !(newurl = argv[1]))
		usage_with_options(usage, options);

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);

	אם (!sub)
		die(_("no submodule mapping found in .gitmodules for path '%s'"),
		    path);

	config_name = xstrfmt("submodule.%s.url", sub->name);
	ret = config_set_in_gitmodules_file_gently(the_repository, config_name, newurl);

	אם (!ret) {
		repo_read_gitmodules(the_repository, 0);
		sync_submodule(sub->path, prefix, عدم, quiet ? OPT_QUIET : 0);
	}

	حرر(config_name);
	החזר !!ret;
}

ساكن صحيح module_set_branch(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix,
			     بنية repository *repo UNUSED)
{
	صحيح opt_default = 0, ret;
	ثابت حرف *opt_branch = عدم;
	ثابت حرف *path;
	حرف *config_name;
	بنية option options[] = {
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
	ثابت حرف *ثابت usage[] = {
		N_("git submodule set-branch [-q|--quiet] (-d|--default) <path>"),
		N_("git submodule set-branch [-q|--quiet] (-b|--branch) <branch> <path>"),
		عدم
	};
	ثابت بنية submodule *sub;

	argc = parse_options(argc, argv, prefix, options, usage, 0);

	אם (!opt_branch && !opt_default)
		die(_("--branch or --default required"));

	אם (opt_branch && opt_default)
		die(_("options '%s' and '%s' cannot be used together"), "--branch", "--default");

	אם (argc != 1 || !(path = argv[0]))
		usage_with_options(usage, options);

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);

	אם (!sub)
		die(_("no submodule mapping found in .gitmodules for path '%s'"),
		    path);

	config_name = xstrfmt("submodule.%s.branch", sub->name);
	ret = config_set_in_gitmodules_file_gently(the_repository, config_name, opt_branch);

	حرر(config_name);
	החזר !!ret;
}

ساكن صحيح module_create_branch(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix,
				بنية repository *repo UNUSED)
{
	تعداد branch_track track;
	صحيح quiet = 0, force = 0, reflog = 0, dry_run = 0;
	بنية option options[] = {
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
	ثابت حرف *ثابت usage[] = {
		N_("git submodule--helper create-branch [-f|--force] [--create-reflog] [-q|--quiet] [-t|--track] [-n|--dry-run] <name> <start-oid> <start-name>"),
		عدم
	};
	بنية repo_config_values *cfg = repo_config_values(the_repository);

	repo_config(the_repository, git_default_config, عدم);
	track = cfg->branch_track;
	argc = parse_options(argc, argv, prefix, options, usage, 0);

	אם (argc != 3)
		usage_with_options(usage, options);

	אם (!quiet && !dry_run)
		printf_ln(_("creating branch '%s'"), argv[0]);

	create_branches_recursively(the_repository, argv[0], argv[1], argv[2],
				    force, reflog, quiet, track, dry_run);
	החזר 0;
}

بنية add_data {
	ثابت حرف *prefix;
	ثابت حرف *branch;
	ثابت حرف *reference_path;
	حرف *sm_path;
	ثابت حرف *sm_name;
	ثابت حرف *repo;
	ثابت حرف *realrepo;
	تعداد ref_storage_format ref_storage_format;
	صحيح depth;
	unsigned صحيح force: 1;
	unsigned صحيح quiet: 1;
	unsigned صحيح progress: 1;
	unsigned صحيح dissociate: 1;
};
#define ADD_DATA_INIT { \
	.depth = -1, \
	.ref_storage_format = REF_STORAGE_FORMAT_UNKNOWN, \
}

ساكن فراغ append_fetch_remotes(بنية strbuf *msg, ثابت حرف *git_dir_path)
{
	بنية child_process cp_remote = CHILD_PROCESS_INIT;
	بنية strbuf sb_remote_out = STRBUF_INIT;

	cp_remote.git_cmd = 1;
	strvec_pushf(&cp_remote.env,
		     "GIT_DIR=%s", git_dir_path);
	strvec_push(&cp_remote.env, "GIT_WORK_TREE=.");
	strvec_pushl(&cp_remote.args, "remote", "-v", عدم);
	אם (!capture_command(&cp_remote, &sb_remote_out, 0)) {
		حرف *next_line;
		حرف *line = sb_remote_out.buf;

		כלעוד ((next_line = strchr(line, '\n')) != عدم) {
			size_t len = next_line - line;

			אם (strip_suffix_mem(line, &len, " (fetch)"))
				strbuf_addf(msg, "  %.*s\n", (صحيح)len, line);
			line = next_line + 1;
		}
	}

	strbuf_release(&sb_remote_out);
}

ساكن صحيح add_submodule(ثابت بنية add_data *add_data)
{
	بنية module_clone_data clone_data = MODULE_CLONE_DATA_INIT;
	بنية string_list reference = STRING_LIST_INIT_NODUP;
	صحيح ret = -1;

	/* perhaps the path already exists and is already a git repo, else clone it */
	אם (is_directory(add_data->sm_path)) {
		حرف *submod_gitdir_path;
		بنية strbuf sm_path = STRBUF_INIT;
		strbuf_addstr(&sm_path, add_data->sm_path);
		submod_gitdir_path = xstrfmt("%s/.git", add_data->sm_path);
		אם (is_nonbare_repository_dir(&sm_path))
			printf(_("Adding existing repo at '%s' to the index\n"),
			       add_data->sm_path);
		אחרת
			die(_("'%s' already exists and is not a valid git repo"),
			    add_data->sm_path);
		strbuf_release(&sm_path);
		حرر(submod_gitdir_path);
	} אחרת {
		بنية child_process cp = CHILD_PROCESS_INIT;
		بنية strbuf submod_gitdir = STRBUF_INIT;

		submodule_name_to_gitdir(&submod_gitdir, the_repository, add_data->sm_name);

		אם (is_directory(submod_gitdir.buf)) {
			אם (!add_data->force) {
				بنية strbuf msg = STRBUF_INIT;
				حرف *die_msg;

				strbuf_addf(&msg, _("A git directory for '%s' is found "
						    "locally with remote(s):\n"),
					    add_data->sm_name);

				append_fetch_remotes(&msg, submod_gitdir.buf);
				strbuf_release(&submod_gitdir);

				strbuf_addf(&msg, _("If you want to reuse this local git "
						    "directory instead of cloning again from\n"
						    "  %s\n"
						    "use the '--force' option. If the local git "
						    "directory is not the correct repo\n"
						    "or you are unsure what this means choose "
						    "another name with the '--name' option."),
					    add_data->realrepo);

				die_msg = strbuf_detach(&msg, عدم);
				die("%s", die_msg);
			} אחרת {
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
		אם (add_data->reference_path) {
			حرف *p = xstrdup(add_data->reference_path);

			string_list_append(&reference, p)->util = p;
		}
		clone_data.ref_storage_format = add_data->ref_storage_format;
		clone_data.dissociate = add_data->dissociate;
		אם (add_data->depth >= 0)
			clone_data.depth = add_data->depth;

		אם (clone_submodule(&clone_data, &reference))
			اذهب cleanup;

		prepare_submodule_repo_env(&cp.env);
		cp.git_cmd = 1;
		cp.dir = add_data->sm_path;
		/*
		 * NOTE: we only get here if add_data->force is true, so
		 * passing --force to checkout is reasonable.
		 */
		strvec_pushl(&cp.args, "checkout", "-f", "-q", عدم);

		אם (add_data->branch) {
			strvec_pushl(&cp.args, "-B", add_data->branch, عدم);
			strvec_pushf(&cp.args, "origin/%s", add_data->branch);
		}

		אם (run_command(&cp))
			die(_("unable to checkout submodule '%s'"), add_data->sm_path);
	}
	ret = 0;

cleanup:
	string_list_clear(&reference, 1);
	החזר ret;
}

ساكن صحيح config_submodule_in_gitmodules(ثابت حرف *name, ثابت حرف *var, ثابت حرف *value)
{
	حرف *key;
	صحيح ret;

	אם (!is_writing_gitmodules_ok())
		die(_("please make sure that the .gitmodules file is in the working tree"));

	key = xstrfmt("submodule.%s.%s", name, var);
	ret = config_set_in_gitmodules_file_gently(the_repository, key, value);
	حرر(key);

	החזר ret;
}

ساكن فراغ configure_added_submodule(بنية add_data *add_data)
{
	حرف *key;
	بنية child_process add_submod = CHILD_PROCESS_INIT;
	بنية child_process add_gitmodules = CHILD_PROCESS_INIT;
	ثابت بنية string_list *values;
	صحيح matched = 0;

	key = xstrfmt("submodule.%s.url", add_data->sm_name);
	repo_config_set_gently(the_repository, key, add_data->realrepo);
	حرر(key);

	add_submod.git_cmd = 1;
	strvec_pushl(&add_submod.args, "add",
		     "--no-warn-embedded-repo", عدم);
	אם (add_data->force)
		strvec_push(&add_submod.args, "--force");
	strvec_pushl(&add_submod.args, "--", add_data->sm_path, عدم);

	אם (run_command(&add_submod))
		die(_("Failed to add submodule '%s'"), add_data->sm_path);

	אם (config_submodule_in_gitmodules(add_data->sm_name, "path", add_data->sm_path) ||
	    config_submodule_in_gitmodules(add_data->sm_name, "url", add_data->repo))
		die(_("Failed to register submodule '%s'"), add_data->sm_path);

	אם (add_data->branch) {
		אם (config_submodule_in_gitmodules(add_data->sm_name,
						   "branch", add_data->branch))
			die(_("Failed to register submodule '%s'"), add_data->sm_path);
	}

	add_gitmodules.git_cmd = 1;
	strvec_pushl(&add_gitmodules.args,
		     "add", "--force", "--", ".gitmodules", عدم);

	אם (run_command(&add_gitmodules))
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
	אם (repo_config_get(the_repository, "submodule.active") || /* key absent */
	    repo_config_get_string_multi(the_repository, "submodule.active", &values)) {
		/*
		 * If the submodule being added isn't already covered by the
		 * current configured pathspec, set the submodule's active flag
		 */
		key = xstrfmt("submodule.%s.active", add_data->sm_name);
		repo_config_set_gently(the_repository, key, "true");
		حرر(key);
	} אחרת {
		עבור (size_t i = 0; i < values->nr; i++) {
			ثابت حرف *pat = values->items[i].string;
			אם (!wildmatch(pat, add_data->sm_path, 0)) { /* match found */
				matched = 1;
				اكسر;
			}
		}
		אם (!matched) { /* no pattern matched -> force-enable */
			key = xstrfmt("submodule.%s.active", add_data->sm_name);
			repo_config_set_gently(the_repository, key, "true");
			حرر(key);
		}
	}
}

ساكن فراغ die_on_index_match(ثابت حرف *path, صحيح force)
{
	بنية pathspec ps;
	ثابت حرف *args[] = { path, عدم };
	parse_pathspec(&ps, 0, PATHSPEC_PREFER_CWD, عدم, args);

	אם (repo_read_index_preload(the_repository, عدم, 0) < 0)
		die(_("index file corrupt"));

	אם (ps.nr) {
		حرف *ps_matched = xcalloc(ps.nr, 1);

		/* TODO: audit for interaction with sparse-index. */
		ensure_full_index(the_repository->index);

		/*
		 * Since there is only one pathspec, we just need to
		 * check ps_matched[0] to know if a cache entry matched.
		 */
		עבור (size_t i = 0; i < the_repository->index->cache_nr; i++) {
			ce_path_match(the_repository->index, the_repository->index->cache[i], &ps,
				      ps_matched);

			אם (ps_matched[0]) {
				אם (!force)
					die(_("'%s' already exists in the index"),
					    path);
				אם (!S_ISGITLINK(the_repository->index->cache[i]->ce_mode))
					die(_("'%s' already exists in the index "
					      "and is not a submodule"), path);
				اكسر;
			}
		}
		حرر(ps_matched);
	}
	clear_pathspec(&ps);
}

ساكن فراغ die_on_repo_without_commits(ثابت حرف *path)
{
	بنية strbuf sb = STRBUF_INIT;
	strbuf_addstr(&sb, path);
	אם (is_nonbare_repository_dir(&sb)) {
		بنية object_id oid;
		אם (repo_resolve_gitlink_ref(the_repository, path, "HEAD", &oid) < 0)
			die(_("'%s' does not have a commit checked out"), path);
	}
	strbuf_release(&sb);
}

ساكن صحيح module_add(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix,
		      بنية repository *repo UNUSED)
{
	صحيح force = 0, quiet = 0, progress = 0, dissociate = 0;
	بنية add_data add_data = ADD_DATA_INIT;
	ثابت حرف *ref_storage_format = عدم;
	حرف *to_free = عدم;
	ثابت بنية submodule *existing;
	بنية strbuf buf = STRBUF_INIT;
	حرف *sm_name_to_free = عدم;
	بنية option options[] = {
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
	ثابت حرف *ثابت usage[] = {
		N_("git submodule add [<options>] [--] <repository> [<path>]"),
		عدم
	};
	بنية strbuf sb = STRBUF_INIT;
	صحيح ret = 1;

	argc = parse_options(argc, argv, prefix, options, usage, 0);

	אם (!is_writing_gitmodules_ok())
		die(_("please make sure that the .gitmodules file is in the working tree"));

	אם (prefix && *prefix &&
	    add_data.reference_path && !is_absolute_path(add_data.reference_path))
		add_data.reference_path = xstrfmt("%s%s", prefix, add_data.reference_path);

	אם (argc == 0 || argc > 2)
		usage_with_options(usage, options);

	אם (ref_storage_format) {
		add_data.ref_storage_format = ref_storage_format_by_name(ref_storage_format);
		אם (add_data.ref_storage_format == REF_STORAGE_FORMAT_UNKNOWN)
			die(_("unknown ref storage format '%s'"), ref_storage_format);
	}

	add_data.repo = argv[0];
	אם (argc == 1)
		add_data.sm_path = git_url_basename(add_data.repo, 0, 0);
	אחרת
		add_data.sm_path = xstrdup(argv[1]);

	אם (prefix && *prefix && !is_absolute_path(add_data.sm_path)) {
		حرف *sm_path = add_data.sm_path;

		add_data.sm_path = xstrfmt("%s%s", prefix, sm_path);
		حرر(sm_path);
	}

	אם (starts_with_dot_dot_slash(add_data.repo) ||
	    starts_with_dot_slash(add_data.repo)) {
		אם (prefix)
			die(_("Relative path can only be used from the toplevel "
			      "of the working tree"));

		/* dereference source url relative to parent's url */
		to_free = resolve_relative_url(add_data.repo, عدم, 1);
		add_data.realrepo = to_free;
	} אחרת אם (is_dir_sep(add_data.repo[0]) || strchr(add_data.repo, ':')) {
		add_data.realrepo = add_data.repo;
	} אחרת {
		die(_("repo URL: '%s' must be absolute or begin with ./|../"),
		    add_data.repo);
	}

	/*
	 * normalize path:
	 * multiple //; leading ./; /./; /../;
	 */
	normalize_path_copy(add_data.sm_path, add_data.sm_path);
	strip_dir_trailing_slashes(add_data.sm_path);

	אם (validate_submodule_path(add_data.sm_path) < 0)
		die(عدم);

	die_on_index_match(add_data.sm_path, force);
	die_on_repo_without_commits(add_data.sm_path);

	אם (!force) {
		بنية child_process cp = CHILD_PROCESS_INIT;

		cp.git_cmd = 1;
		cp.no_stdout = 1;
		strvec_pushl(&cp.args, "add", "--dry-run", "--ignore-missing",
			     "--no-warn-embedded-repo", add_data.sm_path, عدم);
		אם ((ret = pipe_command(&cp, عدم, 0, عدم, 0, &sb, 0))) {
			strbuf_complete_line(&sb);
			fputs(sb.buf, stderr);
			اذهب cleanup;
		}
	}

	אם (!add_data.sm_name)
		add_data.sm_name = add_data.sm_path;

	existing = submodule_from_name(the_repository,
					null_oid(the_hash_algo),
					add_data.sm_name);

	אם (existing && existing->path &&
	    strcmp(existing->path, add_data.sm_path)) {
		אם (!force) {
			die(_("submodule name '%s' already used for path '%s'"),
			    add_data.sm_name, existing->path);
		}
		/* --force: build <name><n> until unique */
		עבור (صحيح i = 1; ; i++) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "%s%d", add_data.sm_name, i);
			אם (!submodule_from_name(the_repository,
						null_oid(the_hash_algo),
						buf.buf)) {
				اكسر;
			}
		}
		add_data.sm_name = sm_name_to_free = strbuf_detach(&buf, عدم);
	}

	אם (check_submodule_name(add_data.sm_name))
		die(_("'%s' is not a valid submodule name"), add_data.sm_name);

	add_data.prefix = prefix;
	add_data.force = !!force;
	add_data.quiet = !!quiet;
	add_data.progress = !!progress;
	add_data.dissociate = !!dissociate;

	אם (the_repository->repository_format_submodule_path_cfg)
		create_default_gitdir_config(add_data.sm_name);

	אם (add_submodule(&add_data))
		اذهب cleanup;
	configure_added_submodule(&add_data);

	ret = 0;
cleanup:
	حرر(sm_name_to_free);
	حرر(add_data.sm_path);
	حرر(to_free);
	strbuf_release(&sb);

	החזר ret;
}

صحيح cmd_submodule__helper(صحيح argc,
			  ثابت حرف **argv,
			  ثابت حرف *prefix,
			  بنية repository *repo)
{
	parse_opt_subcommand_fn *fn = عدم;
	ثابت حرف *ثابت usage[] = {
		N_("git submodule--helper <command>"),
		عدم
	};
	بنية option options[] = {
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
		OPT_SUBCOMMAND("push-check", &fn, push_check),
		OPT_SUBCOMMAND("absorbgitdirs", &fn, absorb_git_dirs),
		OPT_SUBCOMMAND("set-url", &fn, module_set_url),
		OPT_SUBCOMMAND("set-branch", &fn, module_set_branch),
		OPT_SUBCOMMAND("create-branch", &fn, module_create_branch),
		OPT_SUBCOMMAND("get-default-remote", &fn, module_get_default_remote),
		OPT_END()
	};
	argc = parse_options(argc, argv, prefix, options, usage, 0);

	החזר fn(argc, argv, prefix, repo);
}
