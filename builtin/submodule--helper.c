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
	أرجع xstrdup(repo_default_remote(the_repository));
}

ساكن حرف *resolve_relative_url(ثابت حرف *rel_url, ثابت حرف *up_path, صحيح quiet)
{
	حرف *remoteurl, *resolved_url;
	حرف *remote = get_default_remote();
	بنية strbuf remotesb = STRBUF_INIT;

	strbuf_addf(&remotesb, "remote.%s.url", remote);
	إذا (repo_config_get_string(the_repository, remotesb.buf, &remoteurl)) {
		إذا (!quiet)
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

	أرجع resolved_url;
}

ساكن صحيح get_default_remote_submodule(ثابت حرف *module_path, حرف **default_remote)
{
	ثابت بنية submodule *sub;
	بنية repository subrepo;
	ثابت حرف *remote_name = عدم;
	حرف *url = عدم;
	صحيح ret = 0;

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), module_path);
	إذا (sub && sub->url) {
		url = xstrdup(sub->url);

		/* Possibly a url relative to parent */
		إذا (starts_with_dot_dot_slash(url) ||
		    starts_with_dot_slash(url)) {
			حرف *oldurl = url;

			url = resolve_relative_url(oldurl, عدم, 1);
			حرر(oldurl);
		}
	}

	إذا (repo_submodule_init(&subrepo, the_repository, module_path,
				null_oid(the_hash_algo)) < 0) {
		ret = die_message(_("could not get a repository handle for submodule '%s'"),
				   module_path);
		اذهب out;
	}

	/* Look up by URL first */
	إذا (url)
		remote_name = repo_remote_from_url(&subrepo, url);
	إذا (!remote_name)
		remote_name = repo_default_remote(&subrepo);

	*default_remote = xstrdup(remote_name);

out:
	repo_clear(&subrepo);
	حرر(url);

	أرجع ret;
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
	إذا (argc != 1)
		usage_with_options(usage, options);

	path = argv[0];
	إذا (prefix && *prefix && !is_absolute_path(path)) {
		resolved_path = xstrfmt("%s%s", prefix, path);
		path = resolved_path;
	}

	code = get_default_remote_submodule(path, &default_remote);
	إذا (code) {
		حرر(resolved_path);
		أرجع code;
	}

	printf("%s\n", default_remote);
	حرر(default_remote);
	حرر(resolved_path);
	أرجع 0;
}

/* the result should be freed by the caller. */
ساكن حرف *get_submodule_displaypath(ثابت حرف *path, ثابت حرف *prefix,
				       ثابت حرف *super_prefix)
{
	إذا (prefix && super_prefix) {
		BUG("cannot have prefix '%s' and superprefix '%s'",
		    prefix, super_prefix);
	} وإلا إذا (prefix) {
		بنية strbuf sb = STRBUF_INIT;
		حرف *displaypath = xstrdup(relative_path(path, prefix, &sb));
		strbuf_release(&sb);
		أرجع displaypath;
	} وإلا إذا (super_prefix) {
		أرجع xstrfmt("%s%s", super_prefix, path);
	} وإلا {
		أرجع xstrdup(path);
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

	لكل (d = describe_argv; *d; d++) {
		بنية child_process cp = CHILD_PROCESS_INIT;
		prepare_submodule_repo_env(&cp.env);
		cp.dir = sub_path;
		cp.git_cmd = 1;
		cp.no_stderr = 1;

		strvec_ادفع(&cp.وسائط, "describe");
		strvec_pushv(&cp.وسائط, *d);
		strvec_ادفع(&cp.وسائط, object_id);

		إذا (!capture_command(&cp, &sb, 0)) {
			strbuf_strip_suffix(&sb, "\n");
			أرجع strbuf_detach(&sb, عدم);
		}
	}

	strbuf_release(&sb);
	أرجع عدم;
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

	إذا (pathspec->nr)
		ps_matched = xcalloc(pathspec->nr, 1);

	إذا (repo_read_index(the_repository) < 0)
		die(_("index file corrupt"));

	لكل (size_t i = 0; i < the_repository->index->cache_nr; i++) {
		ثابت بنية cache_entry *ce = the_repository->index->cache[i];

		إذا (!match_pathspec(the_repository->index, pathspec, ce->name, ce_namelen(ce),
				    0, ps_matched, 1) ||
		    !S_ISGITLINK(ce->ce_mode))
			استمر;

		ALLOC_GROW(list->entries, list->nr + 1, list->alloc);
		list->entries[list->nr++] = ce;
		طالما (i + 1 < the_repository->index->cache_nr &&
		       !strcmp(ce->name, the_repository->index->cache[i + 1]->name))
			/*
			 * Skip entries with the same name in different stages
			 * to make sure an entry is returned only once.
			 */
			i++;
	}

	إذا (ps_matched && report_path_error(ps_matched, pathspec))
		result = -1;

	حرر(ps_matched);

	أرجع result;
}

ساكن فراغ module_list_active(بنية module_list *list)
{
	صحيح i;
	بنية module_list active_modules = MODULE_LIST_INIT;

	لكل (i = 0; i < list->nr; i++) {
		ثابت بنية cache_entry *ce = list->entries[i];

		إذا (!is_submodule_active(the_repository, ce->name))
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

	strbuf_addstrings(&sb, "../", عدد_slashes(path));

	/*
	 * Check if 'path' ends with slash or not
	 * for having the same output for dir/sub_dir
	 * and dir/sub_dir/
	 */
	إذا (!is_dir_sep(path[strlen(path) - 1]))
		strbuf_addstr(&sb, "../");

	أرجع strbuf_detach(&sb, عدم);
}

ساكن فراغ for_each_listed_submodule(ثابت بنية module_list *list,
				      each_submodule_fn fn, فراغ *cb_data)
{
	صحيح i;

	لكل (i = 0; i < list->nr; i++)
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

	إذا (validate_submodule_path(path) < 0)
		die(عدم);

	displaypath = get_submodule_displaypath(path, info->prefix,
						info->super_prefix);

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);

	إذا (!sub)
		die(_("No url found for submodule path '%s' in .gitmodules"),
			displaypath);

	إذا (!is_submodule_populated_gently(path, عدم))
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
	إذا (info->argc == 1) {
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
		strvec_pushf(&cp.وسائط, "path=%s; %s",
			     sb.buf, info->argv[0]);
		strbuf_release(&sb);
		حرر(toplevel);
	} وإلا {
		strvec_pushv(&cp.وسائط, info->argv);
	}

	إذا (!info->quiet)
		printf(_("Entering '%s'\n"), displaypath);

	إذا (info->argv[0]) {
		إذا (run_command(&cp))
			die(_("run_command returned non-zero status for %s\n."),
			    displaypath);
	} وإلا {
		child_process_clear(&cp);
	}

	إذا (info->recursive) {
		بنية child_process cpr = CHILD_PROCESS_INIT;

		cpr.git_cmd = 1;
		cpr.dir = path;
		prepare_submodule_repo_env(&cpr.env);

		strvec_pushl(&cpr.وسائط, "submodule--helper", "foreach", "--recursive",
			     عدم);
		strvec_pushf(&cpr.وسائط, "--super-prefix=%s/", displaypath);

		إذا (info->quiet)
			strvec_ادفع(&cpr.وسائط, "--quiet");

		strvec_ادفع(&cpr.وسائط, "--");
		strvec_pushv(&cpr.وسائط, info->argv);

		إذا (run_command(&cpr))
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

	إذا (module_list_compute(عدم, prefix, &pathspec, &list) < 0)
		اذهب cleanup;

	info.argc = argc;
	info.argv = argv;
	info.prefix = prefix;

	for_each_listed_submodule(&list, runcommand_in_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	أرجع ret;
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
	ثابت حرف *قيمة;
	حرف *مفتاح;

	إذا (validate_submodule_git_dir(gitdir_path->buf, submodule_name))
		أرجع -1;

	مفتاح = xstrfmt("submodule.%s.gitdir", submodule_name);

	/* Nothing to do if the config already exists. */
	إذا (!repo_config_get_string_tmp(the_repository, مفتاح, &قيمة)) {
		حرر(مفتاح);
		أرجع 0;
	}

	إذا (repo_config_set_gently(the_repository, مفتاح, gitdir_path->buf)) {
		حرر(مفتاح);
		أرجع -1;
	}

	حرر(مفتاح);
	أرجع 0;
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
	إذا (!validate_and_set_submodule_gitdir(&gitdir_path, submodule_name)) {
		strbuf_release(&gitdir_path);
		أرجع;
	}

	/* Case 2.1: Try URI-safe (RFC3986) encoding first, this fixes nested gitdirs */
	strbuf_reset(&gitdir_path);
	repo_git_path_append(the_repository, &gitdir_path, "modules/");
	strbuf_addstr_urlencode(&gitdir_path, submodule_name, is_rfc3986_unreserved);
	إذا (!validate_and_set_submodule_gitdir(&gitdir_path, submodule_name)) {
		strbuf_release(&gitdir_path);
		أرجع;
	}

	/* Case 2.2: Try extended uppercase URI (RFC3986) encoding, to fix case-folding */
	strbuf_reset(&gitdir_path);
	repo_git_path_append(the_repository, &gitdir_path, "modules/");
	strbuf_addstr_urlencode(&gitdir_path, submodule_name, is_casefolding_rfc3986_unreserved);
	إذا (!validate_and_set_submodule_gitdir(&gitdir_path, submodule_name))
		أرجع;

	/* Case 2.3: Try some derived gitdir names, see if one sticks */
	لكل (حرف c = '0'; c <= '9'; c++) {
		strbuf_reset(&gitdir_path);
		repo_git_path_append(the_repository, &gitdir_path, "modules/");
		strbuf_addstr_urlencode(&gitdir_path, submodule_name, is_rfc3986_unreserved);
		strbuf_addch(&gitdir_path, c);
		إذا (!validate_and_set_submodule_gitdir(&gitdir_path, submodule_name))
			أرجع;

		strbuf_reset(&gitdir_path);
		repo_git_path_append(the_repository, &gitdir_path, "modules/");
		strbuf_addstr_urlencode(&gitdir_path, submodule_name, is_casefolding_rfc3986_unreserved);
		strbuf_addch(&gitdir_path, c);
		إذا (!validate_and_set_submodule_gitdir(&gitdir_path, submodule_name))
			أرجع;
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
	إذا (!validate_and_set_submodule_gitdir(&gitdir_path, submodule_name)) {
		strbuf_release(&gitdir_path);
		أرجع;
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

	إذا (!sub)
		die(_("No url found for submodule path '%s' in .gitmodules"),
			displaypath);

	/*
	 * NEEDSWORK: In a multi-working-tree world, this needs to be
	 * set in the per-worktree config.
	 *
	 * Set active flag for the submodule being initialized
	 */
	إذا (!is_submodule_active(the_repository, path)) {
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
	إذا (repo_config_get_string(the_repository, sb.buf, &url)) {
		إذا (!sub->url)
			die(_("No url found for submodule path '%s' in .gitmodules"),
				displaypath);

		url = xstrdup(sub->url);

		/* Possibly a url relative to parent */
		إذا (starts_with_dot_dot_slash(url) ||
		    starts_with_dot_slash(url)) {
			حرف *oldurl = url;

			url = resolve_relative_url(oldurl, عدم, 0);
			حرر(oldurl);
		}

		إذا (repo_config_set_gently(the_repository, sb.buf, url))
			die(_("Failed to register url for submodule path '%s'"),
			    displaypath);
		إذا (!(flags & OPT_QUIET))
			fprintf(stderr,
				_("Submodule '%s' (%s) registered for path '%s'\n"),
				sub->name, url, displaypath);
	}
	strbuf_reset(&sb);

	/* Copy "update" setting when it is not set yet */
	strbuf_addf(&sb, "submodule.%s.update", sub->name);
	إذا (repo_config_get_string_tmp(the_repository, sb.buf, &upd) &&
	    sub->update_strategy.type != SM_UPDATE_UNSPECIFIED) {
		إذا (sub->update_strategy.type == SM_UPDATE_COMMAND) {
			fprintf(stderr, _("warning: command update mode suggested for submodule '%s'\n"),
				sub->name);
			upd = "none";
		} وإلا {
			upd = submodule_update_type_to_string(sub->update_strategy.type);
		}

		إذا (repo_config_set_gently(the_repository, sb.buf, upd))
			die(_("Failed to register update mode for submodule path '%s'"), displaypath);
	}

	إذا (the_repository->repository_format_submodule_path_cfg)
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

	إذا (module_list_compute(argv, prefix, &pathspec, &list) < 0)
		اذهب cleanup;

	/*
	 * If there are no path args and submodule.active is set then,
	 * by default, only initialize 'active' modules.
	 */
	إذا (!argc && !repo_config_get(the_repository, "submodule.active"))
		module_list_active(&list);

	info.prefix = prefix;
	إذا (quiet)
		info.flags |= OPT_QUIET;

	for_each_listed_submodule(&list, init_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	أرجع ret;
}

بنية status_cb {
	ثابت حرف *prefix;
	ثابت حرف *super_prefix;
	unsigned صحيح flags;
};
#define STATUS_CB_INIT { 0 }

ساكن فراغ اطبع_status(unsigned صحيح flags, حرف state, ثابت حرف *path,
			 ثابت بنية object_id *oid, ثابت حرف *displaypath)
{
	إذا (flags & OPT_QUIET)
		أرجع;

	printf("%c%s %s", state, oid_to_hex(oid), displaypath);

	إذا (state == ' ' || state == '+') {
		حرف *name = compute_rev_name(path, oid_to_hex(oid));

		إذا (name)
			printf(" (%s)", name);
		حرر(name);
	}

	printf("\n");
}

ساكن صحيح handle_submodule_head_ref(ثابت بنية reference *ref, فراغ *cb_data)
{
	بنية object_id *output = cb_data;

	إذا (ref->oid)
		oidcpy(output, ref->oid);

	أرجع 0;
}

ساكن فراغ status_submodule(ثابت حرف *path, ثابت بنية object_id *ce_oid,
			     unsigned صحيح ce_flags, ثابت حرف *prefix,
			     ثابت حرف *super_prefix, unsigned صحيح flags)
{
	حرف *displaypath;
	بنية strvec diff_files_وسائط = STRVEC_INIT;
	بنية rev_info rev = REV_INFO_INIT;
	بنية strbuf buf = STRBUF_INIT;
	ثابت حرف *git_dir;

	إذا (validate_submodule_path(path) < 0)
		die(عدم);

	إذا (!submodule_from_path(the_repository, null_oid(the_hash_algo), path))
		die(_("no submodule mapping found in .gitmodules for path '%s'"),
		      path);

	displaypath = get_submodule_displaypath(path, prefix, super_prefix);

	إذا ((CE_STAGEMASK & ce_flags) >> CE_STAGESHIFT) {
		اطبع_status(flags, 'U', path, null_oid(the_hash_algo), displaypath);
		اذهب cleanup;
	}

	strbuf_addf(&buf, "%s/.git", path);
	git_dir = read_gitfile(buf.buf);
	إذا (!git_dir)
		git_dir = buf.buf;

	إذا (!is_submodule_active(the_repository, path) ||
	    !is_git_directory(git_dir)) {
		اطبع_status(flags, '-', path, ce_oid, displaypath);
		strbuf_release(&buf);
		اذهب cleanup;
	}
	strbuf_release(&buf);

	strvec_pushl(&diff_files_وسائط, "diff-files",
		     "--ignore-submodules=dirty", "--quiet", "--",
		     path, عدم);

	repo_config(the_repository, git_diff_basic_config, عدم);

	repo_init_revisions(the_repository, &rev, عدم);
	rev.abbrev = 0;
	setup_revisions_from_strvec(&diff_files_وسائط, &rev, عدم);
	run_diff_files(&rev, 0);

	إذا (!diff_result_code(&rev)) {
		اطبع_status(flags, ' ', path, ce_oid,
			     displaypath);
	} وإلا إذا (!(flags & OPT_CACHED)) {
		بنية object_id oid;
		بنية ref_store *refs = repo_get_submodule_ref_store(the_repository,
								      path);

		إذا (!refs) {
			اطبع_status(flags, '-', path, ce_oid, displaypath);
			اذهب cleanup;
		}
		إذا (refs_head_ref(refs, handle_submodule_head_ref, &oid))
			die(_("could not resolve HEAD ref inside the "
			      "submodule '%s'"), path);

		اطبع_status(flags, '+', path, &oid, displaypath);
	} وإلا {
		اطبع_status(flags, '+', path, ce_oid, displaypath);
	}

	إذا (flags & OPT_RECURSIVE) {
		بنية child_process cpr = CHILD_PROCESS_INIT;
		صحيح res;

		cpr.git_cmd = 1;
		cpr.dir = path;
		prepare_submodule_repo_env(&cpr.env);

		strvec_pushl(&cpr.وسائط, "submodule--helper", "status",
			     "--recursive", عدم);
		strvec_pushf(&cpr.وسائط, "--super-prefix=%s/", displaypath);

		إذا (flags & OPT_CACHED)
			strvec_ادفع(&cpr.وسائط, "--cached");

		إذا (flags & OPT_QUIET)
			strvec_ادفع(&cpr.وسائط, "--quiet");

		res = run_command(&cpr);
		إذا (res == SIGPIPE + 128)
			ارفع(SIGPIPE);
		وإلا إذا (res)
			die(_("failed to recurse into submodule '%s'"), path);
	}

cleanup:
	strvec_clear(&diff_files_وسائط);
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

	إذا (module_list_compute(argv, prefix, &pathspec, &list) < 0)
		اذهب cleanup;

	info.prefix = prefix;
	إذا (quiet)
		info.flags |= OPT_QUIET;

	for_each_listed_submodule(&list, status_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	أرجع ret;
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

	لكل (i = 0; i < mcbl->nr; i++) {
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
	strvec_pushl(&cp_rev_parse.وسائط, "rev-parse", "-q", "--short", عدم);
	strvec_pushf(&cp_rev_parse.وسائط, "%s^0", committish);
	strvec_ادفع(&cp_rev_parse.وسائط, "--");

	إذا (capture_command(&cp_rev_parse, &result, 0))
		أرجع عدم;

	strbuf_trim_trailing_newline(&result);
	أرجع strbuf_detach(&result, عدم);
}

ساكن فراغ اطبع_submodule_summary(بنية summary_cb *info, ثابت حرف *errmsg,
				    صحيح total_commits, ثابت حرف *displaypath,
				    ثابت حرف *src_abbrev, ثابت حرف *dst_abbrev,
				    بنية module_cb *p)
{
	إذا (p->status == 'T') {
		إذا (S_ISGITLINK(p->mod_dst))
			printf(_("* %s %s(blob)->%s(submodule)"),
				 displaypath, src_abbrev, dst_abbrev);
		وإلا
			printf(_("* %s %s(submodule)->%s(blob)"),
				 displaypath, src_abbrev, dst_abbrev);
	} وإلا {
		printf("* %s %s...%s",
			displaypath, src_abbrev, dst_abbrev);
	}

	إذا (total_commits < 0)
		printf(":\n");
	وإلا
		printf(" (%d):\n", total_commits);

	إذا (errmsg) {
		printf(_("%s"), errmsg);
	} وإلا إذا (total_commits > 0) {
		بنية child_process cp_log = CHILD_PROCESS_INIT;

		cp_log.git_cmd = 1;
		cp_log.dir = p->sm_path;
		prepare_submodule_repo_env(&cp_log.env);
		strvec_pushl(&cp_log.وسائط, "log", عدم);

		إذا (S_ISGITLINK(p->mod_src) && S_ISGITLINK(p->mod_dst)) {
			إذا (info->summary_limit > 0)
				strvec_pushf(&cp_log.وسائط, "-%d",
					     info->summary_limit);

			strvec_pushl(&cp_log.وسائط, "--pretty=  %m %s",
				     "--first-parent", عدم);
			strvec_pushf(&cp_log.وسائط, "%s...%s",
				     src_abbrev, dst_abbrev);
		} وإلا إذا (S_ISGITLINK(p->mod_dst)) {
			strvec_pushl(&cp_log.وسائط, "--pretty=  > %s",
				     "-1", dst_abbrev, عدم);
		} وإلا {
			strvec_pushl(&cp_log.وسائط, "--pretty=  < %s",
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

	إذا (!info->cached && oideq(&p->oid_dst, null_oid(the_hash_algo))) {
		إذا (S_ISGITLINK(p->mod_dst)) {
			بنية ref_store *refs = repo_get_submodule_ref_store(the_repository,
									      p->sm_path);

			إذا (refs)
				refs_head_ref(refs, handle_submodule_head_ref, &p->oid_dst);
		} وإلا إذا (S_ISLNK(p->mod_dst) || S_ISREG(p->mod_dst)) {
			بنية stat st;
			صحيح fd = open(p->sm_path, O_RDONLY);

			إذا (fd < 0 || fstat(fd, &st) < 0 ||
			    index_fd(the_repository->index, &p->oid_dst, fd, &st, OBJ_BLOB,
				     p->sm_path, 0))
				error(_("couldn't hash object from '%s'"), p->sm_path);
		} وإلا {
			/* for a submodule removal (mode:0000000), don't warn */
			إذا (p->mod_dst)
				warning(_("unexpected mode %o"), p->mod_dst);
		}
	}

	إذا (S_ISGITLINK(p->mod_src)) {
		إذا (p->status != 'D')
			src_abbrev = verify_submodule_committish(p->sm_path,
								 oid_to_hex(&p->oid_src));
		إذا (!src_abbrev) {
			missing_src = 1;
			/*
			 * As `rev-parse` failed, we fallback to getting
			 * the abbreviated hash using oid_src. We do
			 * this as we might still need the abbreviated
			 * hash in cases like a submodule type change, etc.
			 */
			src_abbrev = xstrndup(oid_to_hex(&p->oid_src), 7);
		}
	} وإلا {
		/*
		 * The source does not point to a submodule.
		 * So, we fallback to getting the abbreviation using
		 * oid_src as we might still need the abbreviated
		 * hash in cases like submodule add, etc.
		 */
		src_abbrev = xstrndup(oid_to_hex(&p->oid_src), 7);
	}

	إذا (S_ISGITLINK(p->mod_dst)) {
		dst_abbrev = verify_submodule_committish(p->sm_path,
							 oid_to_hex(&p->oid_dst));
		إذا (!dst_abbrev) {
			missing_dst = 1;
			/*
			 * As `rev-parse` failed, we fallback to getting
			 * the abbreviated hash using oid_dst. We do
			 * this as we might still need the abbreviated
			 * hash in cases like a submodule type change, etc.
			 */
			dst_abbrev = xstrndup(oid_to_hex(&p->oid_dst), 7);
		}
	} وإلا {
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

	إذا (!missing_src && !missing_dst) {
		بنية child_process cp_rev_list = CHILD_PROCESS_INIT;
		بنية strbuf sb_rev_list = STRBUF_INIT;

		strvec_pushl(&cp_rev_list.وسائط, "rev-list",
			     "--first-parent", "--count", عدم);
		إذا (S_ISGITLINK(p->mod_src) && S_ISGITLINK(p->mod_dst))
			strvec_pushf(&cp_rev_list.وسائط, "%s...%s",
				     src_abbrev, dst_abbrev);
		وإلا
			strvec_ادفع(&cp_rev_list.وسائط, S_ISGITLINK(p->mod_src) ?
				    src_abbrev : dst_abbrev);
		strvec_ادفع(&cp_rev_list.وسائط, "--");

		cp_rev_list.git_cmd = 1;
		cp_rev_list.dir = p->sm_path;
		prepare_submodule_repo_env(&cp_rev_list.env);

		إذا (!capture_command(&cp_rev_list, &sb_rev_list, 0))
			total_commits = atoi(sb_rev_list.buf);

		strbuf_release(&sb_rev_list);
	} وإلا {
		/*
		 * Don't give error msg for modification whose dst is not
		 * submodule, i.e., deleted or changed to blob
		 */
		إذا (S_ISGITLINK(p->mod_dst)) {
			إذا (missing_src && missing_dst) {
				strbuf_addf(&errmsg, "  Warn: %s doesn't contain commits %s and %s\n",
					    displaypath, oid_to_hex(&p->oid_src),
					    oid_to_hex(&p->oid_dst));
			} وإلا {
				strbuf_addf(&errmsg, "  Warn: %s doesn't contain commit %s\n",
					    displaypath, missing_src ?
					    oid_to_hex(&p->oid_src) :
					    oid_to_hex(&p->oid_dst));
			}
		}
	}

	اطبع_submodule_summary(info, errmsg.len ? errmsg.buf : عدم,
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
	لكل (i = 0; i < list->nr; i++) {
		ثابت بنية submodule *sub;
		بنية module_cb *p = list->entries[i];
		بنية strbuf sm_gitdir = STRBUF_INIT;

		إذا (p->status == 'D' || p->status == 'T') {
			generate_submodule_summary(info, p);
			استمر;
		}

		إذا (info->for_status && p->status != 'A' &&
		    (sub = submodule_from_path(the_repository,
					       null_oid(the_hash_algo), p->sm_path))) {
			حرف *config_مفتاح = عدم;
			ثابت حرف *قيمة;
			صحيح ignore_all = 0;

			config_مفتاح = xstrfmt("submodule.%s.ignore",
					     sub->name);
			إذا (!repo_config_get_string_tmp(the_repository, config_مفتاح, &قيمة))
				ignore_all = !strcmp(قيمة, "all");
			وإلا إذا (sub->ignore)
				ignore_all = !strcmp(sub->ignore, "all");

			حرر(config_مفتاح);
			إذا (ignore_all)
				استمر;
		}

		/* Also show added or modified modules which are checked out */
		strbuf_addstr(&sm_gitdir, p->sm_path);
		إذا (is_nonbare_repository_dir(&sm_gitdir))
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
	لكل (i = 0; i < q->nr; i++) {
		بنية diff_filepair *p = q->queue[i];
		بنية module_cb *temp;

		إذا (!S_ISGITLINK(p->one->mode) && !S_ISGITLINK(p->two->mode))
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
	حالة DIFF_INDEX: أرجع "diff-index";
	حالة DIFF_FILES: أرجع "diff-files";
	افتراضي: BUG("bad diff_cmd value %d", diff_cmd);
	}
}

ساكن صحيح compute_summary_module_list(بنية object_id *head_oid,
				       بنية summary_cb *info,
				       تعداد diff_cmd diff_cmd)
{
	بنية strvec diff_وسائط = STRVEC_INIT;
	بنية rev_info rev;
	بنية module_cb_list list = MODULE_CB_LIST_INIT;
	صحيح ret = 0;

	strvec_ادفع(&diff_وسائط, get_diff_cmd(diff_cmd));
	إذا (info->cached)
		strvec_ادفع(&diff_وسائط, "--cached");
	strvec_pushl(&diff_وسائط, "--ignore-submodules=dirty", "--raw", عدم);
	إذا (head_oid)
		strvec_ادفع(&diff_وسائط, oid_to_hex(head_oid));
	strvec_ادفع(&diff_وسائط, "--");
	إذا (info->argc)
		strvec_pushv(&diff_وسائط, info->argv);

	repo_config(the_repository, git_diff_basic_config, عدم);
	repo_init_revisions(the_repository, &rev, info->prefix);
	rev.abbrev = 0;
	precompose_argv_prefix(diff_وسائط.nr, diff_وسائط.v, عدم);
	setup_revisions_from_strvec(&diff_وسائط, &rev, عدم);
	rev.diffopt.output_format = DIFF_FORMAT_NO_OUTPUT | DIFF_FORMAT_CALLBACK;
	rev.diffopt.format_callback = submodule_summary_callback;
	rev.diffopt.format_callback_data = &list;

	إذا (!info->cached) {
		إذا (diff_cmd == DIFF_INDEX)
			setup_work_شجرة(the_repository);
		إذا (repo_read_index_preload(the_repository, &rev.diffopt.pathspec, 0) < 0) {
			perror("repo_read_index_preload");
			ret = -1;
			اذهب cleanup;
		}
	} وإلا إذا (repo_read_index(the_repository) < 0) {
		perror("repo_read_cache");
		ret = -1;
		اذهب cleanup;
	}

	إذا (diff_cmd == DIFF_INDEX)
		run_diff_index(&rev, info->cached ? DIFF_INDEX_CACHED : 0);
	وإلا
		run_diff_files(&rev, 0);
	prepare_submodule_summary(info, &list);
cleanup:
	strvec_clear(&diff_وسائط);
	release_revisions(&rev);
	module_cb_list_release(&list);
	أرجع ret;
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

	إذا (!summary_limit)
		أرجع 0;

	إذا (!repo_get_oid(the_repository, argc ? argv[0] : "HEAD", &head_oid)) {
		إذا (argc) {
			argv++;
			argc--;
		}
	} وإلا إذا (!argc || !strcmp(argv[0], "HEAD")) {
		/* before the first commit: compare with an empty tree */
		oidcpy(&head_oid, the_hash_algo->empty_شجرة);
		إذا (argc) {
			argv++;
			argc--;
		}
	} وإلا {
		إذا (repo_get_oid(the_repository, "HEAD", &head_oid))
			die(_("could not fetch a revision for HEAD"));
	}

	إذا (files) {
		إذا (cached)
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
	أرجع ret;
}

ساكن صحيح module_gitdir(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix UNUSED,
			 بنية repository *repo)
{
	بنية strbuf gitdir = STRBUF_INIT;

	إذا (argc != 2)
		usage(_("git submodule--helper gitdir <name>"));

	submodule_name_to_gitdir(&gitdir, repo, argv[1]);

	printf("%s\n", gitdir.buf);

	strbuf_release(&gitdir);
	أرجع 0;
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
	إذا (!dir)
		die(_("could not open '%s'"), module_dir.buf);

	طالما ((de = readdir(dir))) {
		بنية strbuf gitdir_path = STRBUF_INIT;
		حرف *مفتاح;
		ثابت حرف *قيمة;

		إذا (is_dot_or_dotdot(de->d_name))
			استمر;

		strbuf_addf(&gitdir_path, "%s/%s", module_dir.buf, de->d_name);
		إذا (!is_git_directory(gitdir_path.buf)) {
			strbuf_release(&gitdir_path);
			استمر;
		}
		strbuf_release(&gitdir_path);

		مفتاح = xstrfmt("submodule.%s.gitdir", de->d_name);
		إذا (!repo_config_get_string_tmp(repo, مفتاح, &قيمة)) {
			/* Already has a gitdir config, nothing to do. */
			حرر(مفتاح);
			استمر;
		}
		حرر(مفتاح);

		create_default_gitdir_config(de->d_name);
	}

	closedir(dir);
	strbuf_release(&module_dir);

	repo_config_get_int(the_repository, "core.repositoryformatversion", &repo_version);
	إذا (repo_version == 0 &&
	    repo_config_set_gently(repo, "core.repositoryformatversion", "1"))
		die(_("could not set core.repositoryformatversion to 1.\n"
		      "Please set it for migration to work, for example:\n"
		      "git config core.repositoryformatversion 1"));

	إذا (repo_config_set_gently(repo, "extensions.submodulePathConfig", "true"))
		die(_("could not enable submodulePathConfig extension. It is required\n"
		      "for migration to work. Please enable it in the root repo:\n"
		      "git config extensions.submodulePathConfig true"));

	repo->repository_format_submodule_path_cfg = 1;

	أرجع 0;
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
	حرف *remote_مفتاح = عدم;
	حرف *sub_origin_url, *super_config_url, *displaypath, *default_remote;
	بنية strbuf sb = STRBUF_INIT;
	حرف *sub_config_path = عدم;
	صحيح code;

	إذا (!is_submodule_active(the_repository, path))
		أرجع;

	إذا (validate_submodule_path(path) < 0)
		die(عدم);

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);

	إذا (sub && sub->url) {
		إذا (starts_with_dot_dot_slash(sub->url) ||
		    starts_with_dot_slash(sub->url)) {
			حرف *up_path = get_up_path(path);

			sub_origin_url = resolve_relative_url(sub->url, up_path, 1);
			super_config_url = resolve_relative_url(sub->url, عدم, 1);
			حرر(up_path);
		} وإلا {
			sub_origin_url = xstrdup(sub->url);
			super_config_url = xstrdup(sub->url);
		}
	} وإلا {
		sub_origin_url = xstrdup("");
		super_config_url = xstrdup("");
	}

	displaypath = get_submodule_displaypath(path, prefix, super_prefix);

	إذا (!(flags & OPT_QUIET))
		printf(_("Synchronizing submodule url for '%s'\n"),
			 displaypath);

	strbuf_reset(&sb);
	strbuf_addf(&sb, "submodule.%s.url", sub->name);
	إذا (repo_config_set_gently(the_repository, sb.buf, super_config_url))
		die(_("failed to register url for submodule path '%s'"),
		      displaypath);

	إذا (!is_submodule_populated_gently(path, عدم))
		اذهب cleanup;

	strbuf_reset(&sb);
	code = get_default_remote_submodule(path, &default_remote);
	إذا (code)
		exit(code);

	remote_مفتاح = xstrfmt("remote.%s.url", default_remote);
	حرر(default_remote);

	submodule_to_gitdir(the_repository, &sb, path);
	strbuf_addstr(&sb, "/config");

	إذا (repo_config_set_in_file_gently(the_repository, sb.buf, remote_مفتاح, عدم, sub_origin_url))
		die(_("failed to update remote for submodule '%s'"),
		      path);

	إذا (flags & OPT_RECURSIVE) {
		بنية child_process cpr = CHILD_PROCESS_INIT;

		cpr.git_cmd = 1;
		cpr.dir = path;
		prepare_submodule_repo_env(&cpr.env);

		strvec_pushl(&cpr.وسائط, "submodule--helper", "sync",
			     "--recursive", عدم);
		strvec_pushf(&cpr.وسائط, "--super-prefix=%s/", displaypath);

		إذا (flags & OPT_QUIET)
			strvec_ادفع(&cpr.وسائط, "--quiet");

		إذا (run_command(&cpr))
			die(_("failed to recurse into submodule '%s'"),
			      path);
	}

cleanup:
	حرر(super_config_url);
	حرر(sub_origin_url);
	strbuf_release(&sb);
	حرر(remote_مفتاح);
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

	إذا (module_list_compute(argv, prefix, &pathspec, &list) < 0)
		اذهب cleanup;

	info.prefix = prefix;
	إذا (quiet)
		info.flags |= OPT_QUIET;
	إذا (recursive)
		info.flags |= OPT_RECURSIVE;

	for_each_listed_submodule(&list, sync_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	أرجع ret;
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

	إذا (validate_submodule_path(path) < 0)
		die(عدم);

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);

	إذا (!sub || !sub->name)
		اذهب cleanup;

	displaypath = get_submodule_displaypath(path, prefix, عدم);

	/* remove the submodule work tree (unless the user already did it) */
	إذا (is_directory(path)) {
		بنية strbuf sb_rm = STRBUF_INIT;
		ثابت حرف *format;

		إذا (is_directory(sub_git_dir)) {
			إذا (!(flags & OPT_QUIET))
				warning(_("Submodule work tree '%s' contains a .git "
					  "directory. This will be replaced with a "
					  ".git file by using absorbgitdirs."),
					displaypath);

			absorb_git_dir_into_superproject(path, عدم);

		}

		إذا (!(flags & OPT_FORCE)) {
			بنية child_process cp_rm = CHILD_PROCESS_INIT;

			cp_rm.git_cmd = 1;
			strvec_pushl(&cp_rm.وسائط, "rm", "-qn",
				     path, عدم);

			إذا (run_command(&cp_rm))
				die(_("Submodule work tree '%s' contains local "
				      "modifications; use '-f' to discard them"),
				      displaypath);
		}

		strbuf_addstr(&sb_rm, path);

		إذا (!remove_dir_recursively(&sb_rm, 0))
			format = _("Cleared directory '%s'\n");
		وإلا
			format = _("Could not remove submodule work tree '%s'\n");

		إذا (!(flags & OPT_QUIET))
			printf(format, displaypath);

		submodule_unset_core_worktree(sub);

		strbuf_release(&sb_rm);
	}

	إذا (mkdir(path, 0777))
		printf(_("could not create empty submodule directory %s"),
		      displaypath);

	cp_config.git_cmd = 1;
	strvec_pushl(&cp_config.وسائط, "config", "--get-regexp", عدم);
	strvec_pushf(&cp_config.وسائط, "submodule.%s\\.", sub->name);

	/* remove the .git/config entries (unless the user already did it) */
	إذا (!capture_command(&cp_config, &sb_config, 0) && sb_config.len) {
		حرف *sub_مفتاح = xstrfmt("submodule.%s", sub->name);

		/*
		 * remove the whole section so we have a clean state when
		 * the user later decides to init this submodule again
		 */
		repo_config_rename_section_in_file(the_repository, عدم, sub_مفتاح, عدم);
		إذا (!(flags & OPT_QUIET))
			printf(_("Submodule '%s' (%s) unregistered for path '%s'\n"),
				 sub->name, sub->url, displaypath);
		حرر(sub_مفتاح);
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

	إذا (all && argc) {
		error("pathspec and --all are incompatible");
		usage_with_options(git_submodule_helper_usage,
				   module_deinit_options);
	}

	إذا (!argc && !all)
		die(_("Use '--all' if you really want to deinitialize all submodules"));

	إذا (module_list_compute(argv, prefix, &pathspec, &list) < 0)
		اذهب cleanup;

	info.prefix = prefix;
	إذا (quiet)
		info.flags |= OPT_QUIET;
	إذا (force)
		info.flags |= OPT_FORCE;

	for_each_listed_submodule(&list, deinit_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	أرجع ret;
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
	إذا (strip_suffix(alt_odb->abspath, "/objects", &len)) {
		بنية repository alternate;
		حرف *sm_alternate;
		بنية strbuf sb = STRBUF_INIT;
		بنية strbuf err = STRBUF_INIT;
		strbuf_add(&sb, alt_odb->abspath, len);

		إذا (repo_init(&alternate, sb.buf, عدم) < 0)
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
		إذا (sm_alternate) {
			حرف *p = strbuf_detach(&sb, عدم);

			string_list_append(sas->reference, p)->util = p;
			حرر(sm_alternate);
		} وإلا {
			بدل (sas->error_mode) {
			حالة SUBMODULE_ALTERNATE_ERROR_DIE:
				إذا (advice_enabled(ADVICE_SUBMODULE_ALTERNATE_ERROR_STRATEGY_DIE))
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

	أرجع 0;
}

ساكن فراغ prepare_possible_alternates(ثابت حرف *sm_name,
		بنية string_list *reference)
{
	حرف *sm_alternate = عدم, *error_strategy = عدم;
	بنية submodule_alternate_setup sas = SUBMODULE_ALTERNATE_SETUP_INIT;

	repo_config_get_string(the_repository, "submodule.alternateLocation", &sm_alternate);
	إذا (!sm_alternate)
		أرجع;

	repo_config_get_string(the_repository, "submodule.alternateErrorStrategy", &error_strategy);

	إذا (!error_strategy)
		error_strategy = xstrdup("die");

	sas.submodule_name = sm_name;
	sas.reference = reference;
	إذا (!strcmp(error_strategy, "die"))
		sas.error_mode = SUBMODULE_ALTERNATE_ERROR_DIE;
	وإلا إذا (!strcmp(error_strategy, "info"))
		sas.error_mode = SUBMODULE_ALTERNATE_ERROR_INFO;
	وإلا إذا (!strcmp(error_strategy, "ignore"))
		sas.error_mode = SUBMODULE_ALTERNATE_ERROR_IGNORE;
	وإلا
		die(_("Value '%s' for submodule.alternateErrorStrategy is not recognized"), error_strategy);

	إذا (!strcmp(sm_alternate, "superproject"))
		odb_for_each_alternate(the_repository->objects,
				       add_possible_reference_from_superproject, &sas);
	وإلا إذا (!strcmp(sm_alternate, "no"))
		; /* do nothing */
	وإلا
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

	أرجع sm_gitdir;
}

ساكن صحيح dir_contains_only_dotgit(ثابت حرف *path)
{
	DIR *dir = opendir(path);
	بنية dirent *e;
	صحيح ret = 1;

	إذا (!dir)
		أرجع 0;

	e = readdir_skip_dot_and_dotdot(dir);
	إذا (!e)
		ret = 0;
	وإلا إذا (strcmp(DEFAULT_GIT_DIR_ENVIRONMENT, e->d_name) ||
		 (e = readdir_skip_dot_and_dotdot(dir))) {
		error("unexpected item '%s' in '%s'", e->d_name, path);
		ret = 0;
	}

	closedir(dir);
	أرجع ret;
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

	إذا (validate_submodule_path(clone_data_path) < 0)
		die(عدم);

	إذا (!is_absolute_path(clone_data->path))
		clone_data_path = to_free = xstrfmt("%s/%s", repo_get_work_شجرة(the_repository),
						    clone_data->path);

	إذا (!file_exists(sm_gitdir)) {
		إذا (clone_data->require_init && !stat(clone_data_path, &st) &&
		    !is_empty_dir(clone_data_path))
			die(_("directory not empty: '%s'"), clone_data_path);

		إذا (safe_create_leading_directories_const(the_repository, sm_gitdir) < 0)
			die(_("could not create directory '%s'"), sm_gitdir);

		prepare_possible_alternates(clone_data->name, reference);

		strvec_ادفع(&cp.وسائط, "clone");
		strvec_ادفع(&cp.وسائط, "--no-checkout");
		إذا (clone_data->quiet)
			strvec_ادفع(&cp.وسائط, "--quiet");
		إذا (clone_data->progress)
			strvec_ادفع(&cp.وسائط, "--progress");
		إذا (clone_data->depth > 0)
			strvec_pushf(&cp.وسائط, "--depth=%d", clone_data->depth);
		إذا (reference->nr) {
			بنية string_list_item *item;

			for_each_string_list_item(item, reference)
				strvec_pushl(&cp.وسائط, "--reference",
					     item->string, عدم);
		}
		إذا (clone_data->ref_storage_format != REF_STORAGE_FORMAT_UNKNOWN)
			strvec_pushf(&cp.وسائط, "--ref-storage-format=%s",
				     ref_storage_format_to_name(clone_data->ref_storage_format));
		إذا (clone_data->dissociate)
			strvec_ادفع(&cp.وسائط, "--dissociate");
		إذا (sm_gitdir && *sm_gitdir)
			strvec_pushl(&cp.وسائط, "--separate-git-dir", sm_gitdir, عدم);
		إذا (clone_data->filter_options && clone_data->filter_options->choice)
			strvec_pushf(&cp.وسائط, "--filter=%s",
				     expand_list_objects_filter_spec(
					     clone_data->filter_options));
		إذا (clone_data->single_branch >= 0)
			strvec_ادفع(&cp.وسائط, clone_data->single_branch ?
				    "--single-branch" :
				    "--no-single-branch");

		strvec_ادفع(&cp.وسائط, "--");
		strvec_ادفع(&cp.وسائط, clone_data->url);
		strvec_ادفع(&cp.وسائط, clone_data_path);

		cp.git_cmd = 1;
		prepare_submodule_repo_env(&cp.env);
		cp.no_stdin = 1;

		إذا(run_command(&cp))
			die(_("clone of '%s' into submodule path '%s' failed"),
			    clone_data->url, clone_data_path);

		إذا (clone_data->require_init && !stat(clone_data_path, &st) &&
		    !dir_contains_only_dotgit(clone_data_path)) {
			حرف *dot_git = xstrfmt("%s/.git", clone_data_path);
			unlink(dot_git);
			حرر(dot_git);
			die(_("directory not empty: '%s'"), clone_data_path);
		}
	} وإلا {
		حرف *path;

		إذا (clone_data->require_init && !stat(clone_data_path, &st) &&
		    !is_empty_dir(clone_data_path))
			die(_("directory not empty: '%s'"), clone_data_path);
		إذا (safe_create_leading_directories_const(the_repository, clone_data_path) < 0)
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
	إذا (validate_submodule_git_dir(sm_gitdir, clone_data->name) < 0) {
		حرف *head = xstrfmt("%s/HEAD", sm_gitdir);
		unlink(head);
		حرر(head);
		die(_("refusing to create/use '%s' in another submodule's git dir. "
		      "Enabling extensions.submodulePathConfig should fix this."),
		    sm_gitdir);
	}

	connect_work_شجرة_and_git_dir(clone_data_path, sm_gitdir, 0);

	p = repo_submodule_path(the_repository, clone_data_path, "config");
	إذا (!p)
		die(_("could not get submodule directory for '%s'"), clone_data_path);

	/* setup alternateLocation and alternateErrorStrategy in the cloned submodule if needed */
	repo_config_get_string(the_repository, "submodule.alternateLocation", &sm_alternate);
	إذا (sm_alternate)
		repo_config_set_in_file(the_repository, p, "submodule.alternateLocation",
					sm_alternate);
	repo_config_get_string(the_repository, "submodule.alternateErrorStrategy", &error_strategy);
	إذا (error_strategy)
		repo_config_set_in_file(the_repository, p, "submodule.alternateErrorStrategy",
					error_strategy);

	حرر(sm_alternate);
	حرر(error_strategy);

	حرر(sm_gitdir);
	حرر(p);
	حرر(to_free);
	أرجع 0;
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

	إذا (ref_storage_format) {
		clone_data.ref_storage_format = ref_storage_format_by_name(ref_storage_format);
		إذا (clone_data.ref_storage_format == REF_STORAGE_FORMAT_UNKNOWN)
			die(_("unknown ref storage format '%s'"), ref_storage_format);
	}
	clone_data.dissociate = !!dissociate;
	clone_data.quiet = !!quiet;
	clone_data.progress = !!progress;
	clone_data.require_init = !!require_init;
	clone_data.filter_options = &filter_options;

	إذا (argc || !clone_data.url || !clone_data.path || !*(clone_data.path))
		usage_with_options(git_submodule_helper_usage,
				   module_clone_options);

	clone_submodule(&clone_data, &reference);
	list_objects_filter_release(&filter_options);
	string_list_clear(&reference, 1);
	أرجع 0;
}

ساكن صحيح determine_submodule_update_strategy(بنية repository *r,
					       صحيح just_cloned,
					       ثابت حرف *path,
					       تعداد submodule_update_type update,
					       بنية submodule_update_strategy *out)
{
	ثابت بنية submodule *sub = submodule_from_path(r, null_oid(the_hash_algo), path);
	حرف *مفتاح;
	ثابت حرف *val;
	صحيح ret;

	/*
	 * NEEDSWORK: audit and ensure that update_submodule() has right
	 * to assume that submodule_from_path() above will always succeed.
	 */
	إذا (!sub)
		BUG("update_submodule assumes a submodule exists at path (%s)",
		    path);
	مفتاح = xstrfmt("submodule.%s.update", sub->name);

	إذا (update) {
		out->type = update;
	} وإلا إذا (!repo_config_get_string_tmp(r, مفتاح, &val)) {
		إذا (parse_submodule_update_strategy(val, out) < 0) {
			ret = die_message(_("Invalid update mode '%s' configured for submodule path '%s'"),
					  val, path);
			اذهب cleanup;
		}
	} وإلا إذا (sub->update_strategy.type != SM_UPDATE_UNSPECIFIED) {
		إذا (sub->update_strategy.type == SM_UPDATE_COMMAND)
			BUG("how did we read update = !command from .gitmodules?");
		out->type = sub->update_strategy.type;
		out->command = sub->update_strategy.command;
	} وإلا
		out->type = SM_UPDATE_CHECKOUT;

	إذا (just_cloned &&
	    (out->type == SM_UPDATE_MERGE ||
	     out->type == SM_UPDATE_REBASE ||
	     out->type == SM_UPDATE_NONE))
		out->type = SM_UPDATE_CHECKOUT;

	ret = 0;
cleanup:
	حرر(مفتاح);
	أرجع ret;
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
	إذا (suc->update_data->warn_if_uninitialized) {
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
	حرف *مفتاح;
	ثابت بنية update_data *ud = suc->update_data;
	حرف *displaypath = get_submodule_displaypath(ce->name, ud->prefix,
						      ud->super_prefix);
	بنية strbuf sb = STRBUF_INIT;
	صحيح needs_cloning = 0;
	صحيح need_free_url = 0;

	إذا (ce_stage(ce)) {
		strbuf_addf(out, _("Skipping unmerged submodule %s"), displaypath);
		strbuf_addch(out, '\n');
		اذهب cleanup;
	}

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), ce->name);

	إذا (!sub) {
		next_submodule_warn_missing(suc, out, displaypath);
		اذهب cleanup;
	}

	مفتاح = xstrfmt("submodule.%s.update", sub->name);
	إذا (!repo_config_get_string_tmp(the_repository, مفتاح, &update_string)) {
		update_type = parse_submodule_update_type(update_string);
	} وإلا {
		update_type = sub->update_strategy.type;
	}
	حرر(مفتاح);

	إذا (suc->update_data->update_strategy.type == SM_UPDATE_NONE
	    || (suc->update_data->update_strategy.type == SM_UPDATE_UNSPECIFIED
		&& update_type == SM_UPDATE_NONE)) {
		strbuf_addf(out, _("Skipping submodule '%s'"), displaypath);
		strbuf_addch(out, '\n');
		اذهب cleanup;
	}

	/* Check if the submodule has been initialized. */
	إذا (!is_submodule_active(the_repository, ce->name)) {
		next_submodule_warn_missing(suc, out, displaypath);
		اذهب cleanup;
	}

	strbuf_reset(&sb);
	strbuf_addf(&sb, "submodule.%s.url", sub->name);
	إذا (repo_config_get_string_tmp(the_repository, sb.buf, &url)) {
		إذا (sub->url && (starts_with_dot_slash(sub->url) ||
				 starts_with_dot_dot_slash(sub->url))) {
			url = resolve_relative_url(sub->url, عدم, 0);
			need_free_url = 1;
		} وإلا
			url = sub->url;
	}

	إذا (!url)
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

	إذا (!needs_cloning)
		اذهب cleanup;

	child->git_cmd = 1;
	child->no_stdin = 1;
	child->stdout_to_stderr = 1;
	child->err = -1;
	strvec_ادفع(&child->وسائط, "submodule--helper");
	strvec_ادفع(&child->وسائط, "clone");
	إذا (suc->update_data->progress)
		strvec_ادفع(&child->وسائط, "--progress");
	إذا (suc->update_data->quiet)
		strvec_ادفع(&child->وسائط, "--quiet");
	إذا (suc->update_data->prefix)
		strvec_pushl(&child->وسائط, "--prefix", suc->update_data->prefix, عدم);
	إذا (suc->update_data->recommend_shallow && sub->recommend_shallow == 1)
		strvec_ادفع(&child->وسائط, "--depth=1");
	وإلا إذا (suc->update_data->depth)
		strvec_pushf(&child->وسائط, "--depth=%d", suc->update_data->depth);
	إذا (suc->update_data->filter_options && suc->update_data->filter_options->choice)
		strvec_pushf(&child->وسائط, "--filter=%s",
			     expand_list_objects_filter_spec(suc->update_data->filter_options));
	إذا (suc->update_data->require_init)
		strvec_ادفع(&child->وسائط, "--require-init");
	إذا (suc->update_data->ref_storage_format != REF_STORAGE_FORMAT_UNKNOWN)
		strvec_pushf(&child->وسائط, "--ref-storage-format=%s",
			     ref_storage_format_to_name(suc->update_data->ref_storage_format));
	strvec_pushl(&child->وسائط, "--path", sub->path, عدم);
	strvec_pushl(&child->وسائط, "--name", sub->name, عدم);
	strvec_pushl(&child->وسائط, "--url", url, عدم);
	إذا (suc->update_data->references.nr) {
		بنية string_list_item *item;

		for_each_string_list_item(item, &suc->update_data->references)
			strvec_pushl(&child->وسائط, "--reference", item->string, عدم);
	}
	إذا (suc->update_data->dissociate)
		strvec_ادفع(&child->وسائط, "--dissociate");
	إذا (suc->update_data->single_branch >= 0)
		strvec_ادفع(&child->وسائط, suc->update_data->single_branch ?
					      "--single-branch" :
					      "--no-single-branch");

cleanup:
	حرر(displaypath);
	strbuf_release(&sb);
	إذا (need_free_url)
		حرر((فراغ*)url);

	أرجع needs_cloning;
}

ساكن صحيح update_clone_get_next_task(بنية child_process *child,
				      بنية strbuf *err,
				      فراغ *suc_cb,
				      فراغ **idx_task_cb)
{
	بنية submodule_update_clone *suc = suc_cb;
	ثابت بنية cache_entry *ce;
	صحيح index;

	لكل (; suc->current < suc->update_data->list.nr; suc->current++) {
		ce = suc->update_data->list.entries[suc->current];
		إذا (prepare_to_clone_next_submodule(ce, child, suc, err)) {
			صحيح *p = xmalloc(حجم(*p));

			*p = suc->current;
			*idx_task_cb = p;
			suc->current++;
			أرجع 1;
		}
	}

	/*
	 * The loop above tried cloning each submodule once, now try the
	 * stragglers again, which we can imagine as an extension of the
	 * entry list.
	 */
	index = suc->current - suc->update_data->list.nr;
	إذا (index < suc->failed_clones_nr) {
		صحيح *p;

		ce = suc->failed_clones[index];
		إذا (!prepare_to_clone_next_submodule(ce, child, suc, err)) {
			suc->current ++;
			strbuf_addstr(err, "BUG: submodule considered for "
					   "cloning, doesn't need cloning "
					   "any more?\n");
			أرجع 0;
		}
		p = xmalloc(حجم(*p));
		*p = suc->current;
		*idx_task_cb = p;
		suc->current ++;
		أرجع 1;
	}

	أرجع 0;
}

ساكن صحيح update_clone_start_failure(بنية strbuf *err UNUSED,
				      فراغ *suc_cb,
				      فراغ *idx_task_cb UNUSED)
{
	بنية submodule_update_clone *suc = suc_cb;

	suc->quickstop = 1;
	أرجع 1;
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

	إذا (!result)
		أرجع 0;

	إذا (idx < suc->update_data->list.nr) {
		ce  = suc->update_data->list.entries[idx];
		strbuf_addf(err, _("Failed to clone '%s'. Retry scheduled"),
			    ce->name);
		strbuf_addch(err, '\n');
		ALLOC_GROW(suc->failed_clones,
			   suc->failed_clones_nr + 1,
			   suc->failed_clones_alloc);
		suc->failed_clones[suc->failed_clones_nr++] = ce;
		أرجع 0;
	} وإلا {
		idx -= suc->update_data->list.nr;
		ce  = suc->failed_clones[idx];
		strbuf_addf(err, _("Failed to clone '%s' a second time, aborting"),
			    ce->name);
		strbuf_addch(err, '\n');
		suc->quickstop = 1;
		أرجع 1;
	}

	أرجع 0;
}

ساكن صحيح git_update_clone_config(ثابت حرف *var, ثابت حرف *قيمة,
				   ثابت بنية config_context *ctx,
				   فراغ *cb)
{
	صحيح *max_jobs = cb;

	إذا (!strcmp(var, "submodule.fetchjobs"))
		*max_jobs = parse_submodule_fetchjobs(var, قيمة, ctx->kvi);
	أرجع 0;
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
	strvec_pushl(&cp.وسائط, "rev-list", "-n", "1", hex, "--not", "--all", عدم);

	prepare_submodule_repo_env(&cp.env);

	إذا (capture_command(&cp, &rev, GIT_MAX_HEXSZ + 1) || rev.len)
		reachable = 0;
	وإلا
		reachable = 1;

	strbuf_release(&rev);
	أرجع reachable;
}

ساكن صحيح fetch_in_submodule(ثابت حرف *module_path, صحيح depth, صحيح quiet,
			      ثابت بنية object_id *oid)
{
	بنية child_process cp = CHILD_PROCESS_INIT;

	prepare_submodule_repo_env(&cp.env);
	cp.git_cmd = 1;
	cp.dir = module_path;

	strvec_ادفع(&cp.وسائط, "fetch");
	إذا (quiet)
		strvec_ادفع(&cp.وسائط, "--quiet");
	إذا (depth)
		strvec_pushf(&cp.وسائط, "--depth=%d", depth);
	إذا (oid) {
		حرف *hex = oid_to_hex(oid);
		حرف *remote;
		صحيح code;

		code = get_default_remote_submodule(module_path, &remote);
		إذا (code) {
			child_process_clear(&cp);
			أرجع code;
		}

		strvec_pushl(&cp.وسائط, remote, hex, عدم);
		حرر(remote);
	}

	أرجع run_command(&cp);
}

ساكن صحيح run_update_command(ثابت بنية update_data *ud, صحيح subforce)
{
	بنية child_process cp = CHILD_PROCESS_INIT;
	حرف *oid = oid_to_hex(&ud->oid);
	صحيح ret;

	بدل (ud->update_strategy.type) {
	حالة SM_UPDATE_CHECKOUT:
		cp.git_cmd = 1;
		strvec_pushl(&cp.وسائط, "checkout", "-q", عدم);
		إذا (subforce)
			strvec_ادفع(&cp.وسائط, "-f");
		اكسر;
	حالة SM_UPDATE_REBASE:
		cp.git_cmd = 1;
		strvec_ادفع(&cp.وسائط, "rebase");
		إذا (ud->quiet)
			strvec_ادفع(&cp.وسائط, "--quiet");
		اكسر;
	حالة SM_UPDATE_MERGE:
		cp.git_cmd = 1;
		strvec_ادفع(&cp.وسائط, "merge");
		إذا (ud->quiet)
			strvec_ادفع(&cp.وسائط, "--quiet");
		اكسر;
	حالة SM_UPDATE_COMMAND:
		cp.use_shell = 1;
		strvec_ادفع(&cp.وسائط, ud->update_strategy.command);
		اكسر;
	افتراضي:
		BUG("unexpected update strategy type: %d",
		    ud->update_strategy.type);
	}
	strvec_ادفع(&cp.وسائط, oid);

	cp.dir = ud->sm_path;
	prepare_submodule_repo_env(&cp.env);
	إذا ((ret = run_command(&cp))) {
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

		أرجع ret;
	}

	إذا (ud->quiet)
		أرجع 0;

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

	أرجع 0;
}

ساكن صحيح run_update_procedure(ثابت بنية update_data *ud)
{
	صحيح subforce = is_null_oid(&ud->suboid) || ud->force;

	إذا (!ud->nofetch) {
		/*
		 * Run fetch only if `oid` isn't present or it
		 * is not reachable from a ref.
		 */
		إذا (!is_tip_reachable(ud->sm_path, &ud->oid) &&
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
		إذا (!is_tip_reachable(ud->sm_path, &ud->oid) &&
		    fetch_in_submodule(ud->sm_path, ud->depth, ud->quiet, &ud->oid))
			أرجع die_message(_("Fetched in submodule path '%s', but it did not "
					     "contain %s. Direct fetching of that commit failed."),
					   ud->displaypath, oid_to_hex(&ud->oid));
	}

	أرجع run_update_command(ud, subforce);
}

ساكن صحيح remote_submodule_branch(ثابت حرف *path, ثابت حرف **branch)
{
	ثابت بنية submodule *sub;
	حرف *مفتاح;
	*branch = عدم;

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);
	إذا (!sub)
		أرجع die_message(_("could not initialize submodule at path '%s'"),
				   path);

	مفتاح = xstrfmt("submodule.%s.branch", sub->name);
	إذا (repo_config_get_string_tmp(the_repository, مفتاح, branch))
		*branch = sub->branch;
	حرر(مفتاح);

	إذا (!*branch) {
		*branch = "HEAD";
		أرجع 0;
	}

	إذا (!strcmp(*branch, ".")) {
		ثابت حرف *refname = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
							      "HEAD", 0, عدم,
							      عدم);

		إذا (!refname)
			أرجع die_message(_("No such ref: %s"), "HEAD");

		/* detached HEAD */
		إذا (!strcmp(refname, "HEAD"))
			أرجع die_message(_("Submodule (%s) branch configured to inherit "
					     "branch from superproject, but the superproject "
					     "is not on any branch"), sub->name);

		إذا (!skip_prefix(refname, "refs/heads/", &refname))
			أرجع die_message(_("Expecting a full ref name, got %s"),
					   refname);

		*branch = refname;
		أرجع 0;
	}

	/* Our "branch" is coming from repo_config_get_string_tmp() */
	أرجع 0;
}

ساكن صحيح ensure_core_worktree(ثابت حرف *path)
{
	ثابت حرف *cw;
	بنية repository subrepo;

	إذا (repo_submodule_init(&subrepo, the_repository, path, null_oid(the_hash_algo)))
		أرجع die_message(_("could not get a repository handle for submodule '%s'"),
				   path);

	إذا (!repo_config_get_string_tmp(&subrepo, "core.worktree", &cw)) {
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
	أرجع 0;
}

ساكن ثابت حرف *submodule_update_type_to_label(تعداد submodule_update_type type)
{
	بدل (type) {
	حالة SM_UPDATE_CHECKOUT:
		أرجع "checkout";
	حالة SM_UPDATE_MERGE:
		أرجع "merge";
	حالة SM_UPDATE_REBASE:
		أرجع "rebase";
	حالة SM_UPDATE_UNSPECIFIED:
	حالة SM_UPDATE_NONE:
	حالة SM_UPDATE_COMMAND:
		اكسر;
	}
	BUG("unreachable with type %d", type);
}

ساكن فراغ update_data_to_وسائط(ثابت بنية update_data *update_data,
				بنية strvec *وسائط)
{
	تعداد submodule_update_type update_type = update_data->update_default;

	strvec_pushl(وسائط, "submodule--helper", "update", "--recursive", عدم);
	إذا (update_data->displaypath)
		strvec_pushf(وسائط, "--super-prefix=%s/",
			     update_data->displaypath);
	strvec_pushf(وسائط, "--jobs=%d", update_data->max_jobs);
	إذا (update_data->quiet)
		strvec_ادفع(وسائط, "--quiet");
	إذا (update_data->force)
		strvec_ادفع(وسائط, "--force");
	إذا (update_data->init)
		strvec_ادفع(وسائط, "--init");
	إذا (update_data->remote)
		strvec_ادفع(وسائط, "--remote");
	إذا (update_data->nofetch)
		strvec_ادفع(وسائط, "--no-fetch");
	إذا (update_data->dissociate)
		strvec_ادفع(وسائط, "--dissociate");
	إذا (update_data->progress)
		strvec_ادفع(وسائط, "--progress");
	إذا (update_data->require_init)
		strvec_ادفع(وسائط, "--require-init");
	إذا (update_data->depth)
		strvec_pushf(وسائط, "--depth=%d", update_data->depth);
	إذا (update_type != SM_UPDATE_UNSPECIFIED)
		strvec_pushf(وسائط, "--%s",
			     submodule_update_type_to_label(update_type));

	إذا (update_data->references.nr) {
		بنية string_list_item *item;

		for_each_string_list_item(item, &update_data->references)
			strvec_pushl(وسائط, "--reference", item->string, عدم);
	}
	إذا (update_data->ref_storage_format != REF_STORAGE_FORMAT_UNKNOWN)
		strvec_pushf(وسائط, "--ref-storage-format=%s",
			     ref_storage_format_to_name(update_data->ref_storage_format));
	إذا (update_data->filter_options && update_data->filter_options->choice)
		strvec_pushf(وسائط, "--filter=%s",
				expand_list_objects_filter_spec(
					update_data->filter_options));
	إذا (update_data->recommend_shallow == 0)
		strvec_ادفع(وسائط, "--no-recommend-shallow");
	وإلا إذا (update_data->recommend_shallow == 1)
		strvec_ادفع(وسائط, "--recommend-shallow");
	إذا (update_data->single_branch >= 0)
		strvec_ادفع(وسائط, update_data->single_branch ?
				    "--single-branch" :
				    "--no-single-branch");
}

ساكن صحيح update_submodule(بنية update_data *update_data)
{
	صحيح ret;

	إذا (validate_submodule_path(update_data->sm_path) < 0)
		أرجع -1;

	ret = determine_submodule_update_strategy(the_repository,
						  update_data->just_cloned,
						  update_data->sm_path,
						  update_data->update_default,
						  &update_data->update_strategy);
	إذا (ret)
		أرجع ret;

	إذا (update_data->just_cloned)
		oidcpy(&update_data->suboid, null_oid(the_hash_algo));
	وإلا إذا (repo_resolve_gitlink_ref(the_repository, update_data->sm_path,
					  "HEAD", &update_data->suboid))
		أرجع die_message(_("Unable to find current revision in submodule path '%s'"),
				   update_data->displaypath);

	إذا (update_data->remote) {
		حرف *remote_name;
		ثابت حرف *branch;
		حرف *remote_ref;
		صحيح code;

		code = get_default_remote_submodule(update_data->sm_path, &remote_name);
		إذا (code)
			أرجع code;
		code = remote_submodule_branch(update_data->sm_path, &branch);
		إذا (code) {
			حرر(remote_name);
			أرجع code;
		}
		remote_ref = xstrfmt("refs/remotes/%s/%s", remote_name, branch);

		حرر(remote_name);

		إذا (!update_data->nofetch) {
			إذا (fetch_in_submodule(update_data->sm_path, update_data->depth,
					      0, عدم)) {
				حرر(remote_ref);
				أرجع die_message(_("Unable to fetch in submodule path '%s'"),
						   update_data->sm_path);
			}
		}

		إذا (repo_resolve_gitlink_ref(the_repository, update_data->sm_path,
					     remote_ref, &update_data->oid)) {
			ret = die_message(_("Unable to find %s revision in submodule path '%s'"),
					  remote_ref, update_data->sm_path);
			حرر(remote_ref);
			أرجع ret;
		}

		حرر(remote_ref);
	}

	إذا (!oideq(&update_data->oid, &update_data->suboid) || update_data->force) {
		ret = run_update_procedure(update_data);
		إذا (ret)
			أرجع ret;
	}

	إذا (update_data->recursive) {
		بنية child_process cp = CHILD_PROCESS_INIT;
		بنية update_data next = *update_data;

		next.prefix = عدم;
		oidcpy(&next.oid, null_oid(the_hash_algo));
		oidcpy(&next.suboid, null_oid(the_hash_algo));

		cp.dir = update_data->sm_path;
		cp.git_cmd = 1;
		prepare_submodule_repo_env(&cp.env);
		update_data_to_وسائط(&next, &cp.وسائط);

		ret = run_command(&cp);
		إذا (ret)
			die_message(_("Failed to recurse into submodule path '%s'"),
				    update_data->displaypath);
		أرجع ret;
	}

	أرجع 0;
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
	إذا (suc.quickstop) {
		ret = 1;
		اذهب cleanup;
	}

	لكل (i = 0; i < suc.update_clone_nr; i++) {
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
		إذا (validate_submodule_path(update_data->sm_path) < 0)
			اذهب fail;

		code = ensure_core_worktree(update_data->sm_path);
		إذا (code)
			اذهب fail;

		update_data->displaypath = get_submodule_displaypath(
			update_data->sm_path, update_data->prefix,
			update_data->super_prefix);
		code = update_submodule(update_data);
		FREE_AND_NULL(update_data->displaypath);
fail:
		إذا (!code)
			استمر;
		ret = code;
		إذا (ret == 128)
			اذهب cleanup;
	}

cleanup:
	submodule_update_clone_release(&suc);
	string_list_clear(&update_data->references, 0);
	أرجع ret;
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

	إذا (opt.require_init)
		opt.init = 1;

	إذا (filter_options.choice && !opt.init) {
		usage_with_options(git_submodule_helper_usage,
				   module_update_options);
	}

	إذا (ref_storage_format) {
		opt.ref_storage_format = ref_storage_format_by_name(ref_storage_format);
		إذا (opt.ref_storage_format == REF_STORAGE_FORMAT_UNKNOWN)
			die(_("unknown ref storage format '%s'"), ref_storage_format);
	}

	opt.filter_options = &filter_options;
	opt.prefix = prefix;

	إذا (opt.update_default)
		opt.update_strategy.type = opt.update_default;

	إذا (module_list_compute(argv, prefix, &pathspec, &opt.list) < 0) {
		ret = 1;
		اذهب cleanup;
	}

	إذا (pathspec.nr)
		opt.warn_if_uninitialized = 1;

	إذا (opt.init) {
		بنية module_list list = MODULE_LIST_INIT;
		بنية init_cb info = INIT_CB_INIT;

		إذا (module_list_compute(argv, opt.prefix,
					&pathspec2, &list) < 0) {
			module_list_release(&list);
			ret = 1;
			اذهب cleanup;
		}

		/*
		 * If there are no path args and submodule.active is set then,
		 * by default, only initialize 'active' modules.
		 */
		إذا (!argc && !repo_config_get(the_repository, "submodule.active"))
			module_list_active(&list);

		info.prefix = opt.prefix;
		info.super_prefix = opt.super_prefix;
		إذا (opt.quiet)
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
	أرجع ret;
}

ساكن صحيح ادفع_check(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix UNUSED,
		      بنية repository *repo UNUSED)
{
	بنية remote *remote;
	ثابت حرف *superproject_head;
	حرف *head;
	صحيح detached_head = 0;
	بنية object_id head_oid;

	إذا (argc < 3)
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
	إذا (!head)
		die(_("Failed to resolve HEAD as a valid ref."));
	إذا (!strcmp(head, "HEAD"))
		detached_head = 1;

	/*
	 * The remote must be configured.
	 * This is to avoid pushing to the exact same URL as the parent.
	 */
	remote = pushremote_get(argv[1]);
	إذا (!remote || remote->origin == REMOTE_UNCONFIGURED)
		die("remote '%s' not configured", argv[1]);

	/* Check the refspec */
	إذا (argc > 2) {
		صحيح i;
		بنية ref *local_refs = get_local_heads();
		بنية refspec refspec = REFSPEC_INIT_ادفع(the_hash_algo);

		refspec_appendn(&refspec, argv + 2, argc - 2);

		لكل (i = 0; i < refspec.nr; i++) {
			ثابت بنية refspec_item *rs = &refspec.عناصر[i];

			إذا (rs->pattern || rs->matching)
				استمر;

			/* LHS must match a single ref */
			بدل (عدد_refspec_match(rs->src, local_refs, عدم)) {
			حالة 1:
				اكسر;
			حالة 0:
				/*
				 * If LHS matches 'HEAD' then we need to ensure
				 * that it matches the same named branch
				 * checked out in the superproject.
				 */
				إذا (!strcmp(rs->src, "HEAD")) {
					إذا (!detached_head &&
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

	أرجع 0;
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

	إذا (module_list_compute(argv, prefix, &pathspec, &list) < 0)
		اذهب cleanup;

	لكل (i = 0; i < list.nr; i++)
		absorb_git_dir_into_superproject(list.entries[i]->name,
						 super_prefix);

	ret = 0;
cleanup:
	clear_pathspec(&pathspec);
	module_list_release(&list);
	أرجع ret;
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

	إذا (argc != 2 || !(path = argv[0]) || !(newurl = argv[1]))
		usage_with_options(usage, options);

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);

	إذا (!sub)
		die(_("no submodule mapping found in .gitmodules for path '%s'"),
		    path);

	config_name = xstrfmt("submodule.%s.url", sub->name);
	ret = config_set_in_gitmodules_file_gently(the_repository, config_name, newurl);

	إذا (!ret) {
		repo_read_gitmodules(the_repository, 0);
		sync_submodule(sub->path, prefix, عدم, quiet ? OPT_QUIET : 0);
	}

	حرر(config_name);
	أرجع !!ret;
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

	إذا (!opt_branch && !opt_default)
		die(_("--branch or --default required"));

	إذا (opt_branch && opt_default)
		die(_("options '%s' and '%s' cannot be used together"), "--branch", "--default");

	إذا (argc != 1 || !(path = argv[0]))
		usage_with_options(usage, options);

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);

	إذا (!sub)
		die(_("no submodule mapping found in .gitmodules for path '%s'"),
		    path);

	config_name = xstrfmt("submodule.%s.branch", sub->name);
	ret = config_set_in_gitmodules_file_gently(the_repository, config_name, opt_branch);

	حرر(config_name);
	أرجع !!ret;
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
	بنية repo_config_قيم *cfg = repo_config_قيم(the_repository);

	repo_config(the_repository, git_default_config, عدم);
	track = cfg->branch_track;
	argc = parse_options(argc, argv, prefix, options, usage, 0);

	إذا (argc != 3)
		usage_with_options(usage, options);

	إذا (!quiet && !dry_run)
		printf_ln(_("creating branch '%s'"), argv[0]);

	create_branches_recursively(the_repository, argv[0], argv[1], argv[2],
				    force, reflog, quiet, track, dry_run);
	أرجع 0;
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

ساكن فراغ append_fetch_remotes(بنية strbuf *رسالة, ثابت حرف *git_dir_path)
{
	بنية child_process cp_remote = CHILD_PROCESS_INIT;
	بنية strbuf sb_remote_out = STRBUF_INIT;

	cp_remote.git_cmd = 1;
	strvec_pushf(&cp_remote.env,
		     "GIT_DIR=%s", git_dir_path);
	strvec_ادفع(&cp_remote.env, "GIT_WORK_TREE=.");
	strvec_pushl(&cp_remote.وسائط, "remote", "-v", عدم);
	إذا (!capture_command(&cp_remote, &sb_remote_out, 0)) {
		حرف *next_سطر;
		حرف *سطر = sb_remote_out.buf;

		طالما ((next_سطر = strchr(سطر, '\n')) != عدم) {
			size_t len = next_سطر - سطر;

			إذا (strip_suffix_mem(سطر, &len, " (fetch)"))
				strbuf_addf(رسالة, "  %.*s\n", (صحيح)len, سطر);
			سطر = next_سطر + 1;
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
	إذا (is_directory(add_data->sm_path)) {
		حرف *submod_gitdir_path;
		بنية strbuf sm_path = STRBUF_INIT;
		strbuf_addstr(&sm_path, add_data->sm_path);
		submod_gitdir_path = xstrfmt("%s/.git", add_data->sm_path);
		إذا (is_nonbare_repository_dir(&sm_path))
			printf(_("Adding existing repo at '%s' to the index\n"),
			       add_data->sm_path);
		وإلا
			die(_("'%s' already exists and is not a valid git repo"),
			    add_data->sm_path);
		strbuf_release(&sm_path);
		حرر(submod_gitdir_path);
	} وإلا {
		بنية child_process cp = CHILD_PROCESS_INIT;
		بنية strbuf submod_gitdir = STRBUF_INIT;

		submodule_name_to_gitdir(&submod_gitdir, the_repository, add_data->sm_name);

		إذا (is_directory(submod_gitdir.buf)) {
			إذا (!add_data->force) {
				بنية strbuf رسالة = STRBUF_INIT;
				حرف *die_رسالة;

				strbuf_addf(&رسالة, _("A git directory for '%s' is found "
						    "locally with remote(s):\n"),
					    add_data->sm_name);

				append_fetch_remotes(&رسالة, submod_gitdir.buf);
				strbuf_release(&submod_gitdir);

				strbuf_addf(&رسالة, _("If you want to reuse this local git "
						    "directory instead of cloning again from\n"
						    "  %s\n"
						    "use the '--force' option. If the local git "
						    "directory is not the correct repo\n"
						    "or you are unsure what this means choose "
						    "another name with the '--name' option."),
					    add_data->realrepo);

				die_رسالة = strbuf_detach(&رسالة, عدم);
				die("%s", die_رسالة);
			} وإلا {
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
		إذا (add_data->reference_path) {
			حرف *p = xstrdup(add_data->reference_path);

			string_list_append(&reference, p)->util = p;
		}
		clone_data.ref_storage_format = add_data->ref_storage_format;
		clone_data.dissociate = add_data->dissociate;
		إذا (add_data->depth >= 0)
			clone_data.depth = add_data->depth;

		إذا (clone_submodule(&clone_data, &reference))
			اذهب cleanup;

		prepare_submodule_repo_env(&cp.env);
		cp.git_cmd = 1;
		cp.dir = add_data->sm_path;
		/*
		 * NOTE: we only get here if add_data->force is true, so
		 * passing --force to checkout is reasonable.
		 */
		strvec_pushl(&cp.وسائط, "checkout", "-f", "-q", عدم);

		إذا (add_data->branch) {
			strvec_pushl(&cp.وسائط, "-B", add_data->branch, عدم);
			strvec_pushf(&cp.وسائط, "origin/%s", add_data->branch);
		}

		إذا (run_command(&cp))
			die(_("unable to checkout submodule '%s'"), add_data->sm_path);
	}
	ret = 0;

cleanup:
	string_list_clear(&reference, 1);
	أرجع ret;
}

ساكن صحيح config_submodule_in_gitmodules(ثابت حرف *name, ثابت حرف *var, ثابت حرف *قيمة)
{
	حرف *مفتاح;
	صحيح ret;

	إذا (!is_writing_gitmodules_ok())
		die(_("please make sure that the .gitmodules file is in the working tree"));

	مفتاح = xstrfmt("submodule.%s.%s", name, var);
	ret = config_set_in_gitmodules_file_gently(the_repository, مفتاح, قيمة);
	حرر(مفتاح);

	أرجع ret;
}

ساكن فراغ configure_added_submodule(بنية add_data *add_data)
{
	حرف *مفتاح;
	بنية child_process add_submod = CHILD_PROCESS_INIT;
	بنية child_process add_gitmodules = CHILD_PROCESS_INIT;
	ثابت بنية string_list *قيم;
	صحيح matched = 0;

	مفتاح = xstrfmt("submodule.%s.url", add_data->sm_name);
	repo_config_set_gently(the_repository, مفتاح, add_data->realrepo);
	حرر(مفتاح);

	add_submod.git_cmd = 1;
	strvec_pushl(&add_submod.وسائط, "add",
		     "--no-warn-embedded-repo", عدم);
	إذا (add_data->force)
		strvec_ادفع(&add_submod.وسائط, "--force");
	strvec_pushl(&add_submod.وسائط, "--", add_data->sm_path, عدم);

	إذا (run_command(&add_submod))
		die(_("Failed to add submodule '%s'"), add_data->sm_path);

	إذا (config_submodule_in_gitmodules(add_data->sm_name, "path", add_data->sm_path) ||
	    config_submodule_in_gitmodules(add_data->sm_name, "url", add_data->repo))
		die(_("Failed to register submodule '%s'"), add_data->sm_path);

	إذا (add_data->branch) {
		إذا (config_submodule_in_gitmodules(add_data->sm_name,
						   "branch", add_data->branch))
			die(_("Failed to register submodule '%s'"), add_data->sm_path);
	}

	add_gitmodules.git_cmd = 1;
	strvec_pushl(&add_gitmodules.وسائط,
		     "add", "--force", "--", ".gitmodules", عدم);

	إذا (run_command(&add_gitmodules))
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
	إذا (repo_config_get(the_repository, "submodule.active") || /* key absent */
	    repo_config_get_string_multi(the_repository, "submodule.active", &قيم)) {
		/*
		 * If the submodule being added isn't already covered by the
		 * current configured pathspec, set the submodule's active flag
		 */
		مفتاح = xstrfmt("submodule.%s.active", add_data->sm_name);
		repo_config_set_gently(the_repository, مفتاح, "true");
		حرر(مفتاح);
	} وإلا {
		لكل (size_t i = 0; i < قيم->nr; i++) {
			ثابت حرف *pat = قيم->عناصر[i].string;
			إذا (!wildmatch(pat, add_data->sm_path, 0)) { /* match found */
				matched = 1;
				اكسر;
			}
		}
		إذا (!matched) { /* no pattern matched -> force-enable */
			مفتاح = xstrfmt("submodule.%s.active", add_data->sm_name);
			repo_config_set_gently(the_repository, مفتاح, "true");
			حرر(مفتاح);
		}
	}
}

ساكن فراغ die_on_index_match(ثابت حرف *path, صحيح force)
{
	بنية pathspec ps;
	ثابت حرف *وسائط[] = { path, عدم };
	parse_pathspec(&ps, 0, PATHSPEC_PREFER_CWD, عدم, وسائط);

	إذا (repo_read_index_preload(the_repository, عدم, 0) < 0)
		die(_("index file corrupt"));

	إذا (ps.nr) {
		حرف *ps_matched = xcalloc(ps.nr, 1);

		/* TODO: audit for interaction with sparse-index. */
		ensure_full_index(the_repository->index);

		/*
		 * Since there is only one pathspec, we just need to
		 * check ps_matched[0] to know if a cache entry matched.
		 */
		لكل (size_t i = 0; i < the_repository->index->cache_nr; i++) {
			ce_path_match(the_repository->index, the_repository->index->cache[i], &ps,
				      ps_matched);

			إذا (ps_matched[0]) {
				إذا (!force)
					die(_("'%s' already exists in the index"),
					    path);
				إذا (!S_ISGITLINK(the_repository->index->cache[i]->ce_mode))
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
	إذا (is_nonbare_repository_dir(&sb)) {
		بنية object_id oid;
		إذا (repo_resolve_gitlink_ref(the_repository, path, "HEAD", &oid) < 0)
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

	إذا (!is_writing_gitmodules_ok())
		die(_("please make sure that the .gitmodules file is in the working tree"));

	إذا (prefix && *prefix &&
	    add_data.reference_path && !is_absolute_path(add_data.reference_path))
		add_data.reference_path = xstrfmt("%s%s", prefix, add_data.reference_path);

	إذا (argc == 0 || argc > 2)
		usage_with_options(usage, options);

	إذا (ref_storage_format) {
		add_data.ref_storage_format = ref_storage_format_by_name(ref_storage_format);
		إذا (add_data.ref_storage_format == REF_STORAGE_FORMAT_UNKNOWN)
			die(_("unknown ref storage format '%s'"), ref_storage_format);
	}

	add_data.repo = argv[0];
	إذا (argc == 1)
		add_data.sm_path = git_url_basename(add_data.repo, 0, 0);
	وإلا
		add_data.sm_path = xstrdup(argv[1]);

	إذا (prefix && *prefix && !is_absolute_path(add_data.sm_path)) {
		حرف *sm_path = add_data.sm_path;

		add_data.sm_path = xstrfmt("%s%s", prefix, sm_path);
		حرر(sm_path);
	}

	إذا (starts_with_dot_dot_slash(add_data.repo) ||
	    starts_with_dot_slash(add_data.repo)) {
		إذا (prefix)
			die(_("Relative path can only be used from the toplevel "
			      "of the working tree"));

		/* dereference source url relative to parent's url */
		to_free = resolve_relative_url(add_data.repo, عدم, 1);
		add_data.realrepo = to_free;
	} وإلا إذا (is_dir_sep(add_data.repo[0]) || strchr(add_data.repo, ':')) {
		add_data.realrepo = add_data.repo;
	} وإلا {
		die(_("repo URL: '%s' must be absolute or begin with ./|../"),
		    add_data.repo);
	}

	/*
	 * normalize path:
	 * multiple //; leading ./; /./; /../;
	 */
	normalize_path_copy(add_data.sm_path, add_data.sm_path);
	strip_dir_trailing_slashes(add_data.sm_path);

	إذا (validate_submodule_path(add_data.sm_path) < 0)
		die(عدم);

	die_on_index_match(add_data.sm_path, force);
	die_on_repo_without_commits(add_data.sm_path);

	إذا (!force) {
		بنية child_process cp = CHILD_PROCESS_INIT;

		cp.git_cmd = 1;
		cp.no_stdout = 1;
		strvec_pushl(&cp.وسائط, "add", "--dry-run", "--ignore-missing",
			     "--no-warn-embedded-repo", add_data.sm_path, عدم);
		إذا ((ret = pipe_command(&cp, عدم, 0, عدم, 0, &sb, 0))) {
			strbuf_complete_سطر(&sb);
			fputs(sb.buf, stderr);
			اذهب cleanup;
		}
	}

	إذا (!add_data.sm_name)
		add_data.sm_name = add_data.sm_path;

	existing = submodule_from_name(the_repository,
					null_oid(the_hash_algo),
					add_data.sm_name);

	إذا (existing && existing->path &&
	    strcmp(existing->path, add_data.sm_path)) {
		إذا (!force) {
			die(_("submodule name '%s' already used for path '%s'"),
			    add_data.sm_name, existing->path);
		}
		/* --force: build <name><n> until unique */
		لكل (صحيح i = 1; ; i++) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "%s%d", add_data.sm_name, i);
			إذا (!submodule_from_name(the_repository,
						null_oid(the_hash_algo),
						buf.buf)) {
				اكسر;
			}
		}
		add_data.sm_name = sm_name_to_free = strbuf_detach(&buf, عدم);
	}

	إذا (check_submodule_name(add_data.sm_name))
		die(_("'%s' is not a valid submodule name"), add_data.sm_name);

	add_data.prefix = prefix;
	add_data.force = !!force;
	add_data.quiet = !!quiet;
	add_data.progress = !!progress;
	add_data.dissociate = !!dissociate;

	إذا (the_repository->repository_format_submodule_path_cfg)
		create_default_gitdir_config(add_data.sm_name);

	إذا (add_submodule(&add_data))
		اذهب cleanup;
	configure_added_submodule(&add_data);

	ret = 0;
cleanup:
	حرر(sm_name_to_free);
	حرر(add_data.sm_path);
	حرر(to_free);
	strbuf_release(&sb);

	أرجع ret;
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
		OPT_SUBCOMMAND("push-check", &fn, ادفع_check),
		OPT_SUBCOMMAND("absorbgitdirs", &fn, absorb_git_dirs),
		OPT_SUBCOMMAND("set-url", &fn, module_set_url),
		OPT_SUBCOMMAND("set-branch", &fn, module_set_branch),
		OPT_SUBCOMMAND("create-branch", &fn, module_create_branch),
		OPT_SUBCOMMAND("get-default-remote", &fn, module_get_default_remote),
		OPT_END()
	};
	argc = parse_options(argc, argv, prefix, options, usage, 0);

	أرجع fn(argc, argv, prefix, repo);
}
