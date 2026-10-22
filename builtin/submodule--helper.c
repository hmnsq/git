#تعریف_کن USE_THE_REPOSITORY_VARIABLE

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

#تعریف_کن OPT_QUIET (1 << 0)
#تعریف_کن OPT_CACHED (1 << 1)
#تعریف_کن OPT_RECURSIVE (1 << 2)
#تعریف_کن OPT_FORCE (1 << 3)

تعريفنوع فراغ (*each_submodule_fn)(ثابت بنية cache_مدخل *list_قلم,
				  فراغ *cb_داده);

ساكن حرف *get_default_remote(فراغ)
{
	أرجع xstrdup(repo_default_remote(the_repository));
}

ساكن حرف *resolve_relative_url(ثابت حرف *rel_url, ثابت حرف *به_بالا_مسیر, صحيح quiet)
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
	resolved_url = relative_url(remoteurl, rel_url, به_بالا_مسیر);

	حرر(remote);
	حرر(remoteurl);
	strbuf_release(&remotesb);

	أرجع resolved_url;
}

ساكن صحيح get_default_remote_submodule(ثابت حرف *module_مسیر, حرف **default_remote)
{
	ثابت بنية submodule *sub;
	بنية repository subrepo;
	ثابت حرف *remote_name = عدم;
	حرف *url = عدم;
	صحيح ret = 0;

	sub = submodule_from_مسیر(the_repository, null_oid(the_hash_algo), module_مسیر);
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

	إذا (repo_submodule_init(&subrepo, the_repository, module_مسیر,
				null_oid(the_hash_algo)) < 0) {
		ret = die_پیام(_("could not get a repository handle for submodule '%s'"),
				   module_مسیر);
		اذهب بیرون;
	}

	/* Look up by URL first */
	إذا (url)
		remote_name = repo_remote_from_url(&subrepo, url);
	إذا (!remote_name)
		remote_name = repo_default_remote(&subrepo);

	*default_remote = xstrdup(remote_name);

بیرون:
	repo_clear(&subrepo);
	حرر(url);

	أرجع ret;
}

ساكن صحيح module_get_default_remote(صحيح argc, ثابت حرف **argv, ثابت حرف *پیشوند,
				     بنية repository *repo استفادهنشده)
{
	ثابت حرف *مسیر;
	حرف *resolved_مسیر = عدم;
	حرف *default_remote = عدم;
	صحيح کد;
	بنية option options[] = {
		OPT_انتها()
	};
	ثابت حرف *ثابت usage[] = {
		N_("git submodule--helper get-default-remote <path>"),
		عدم
	};

	argc = parse_options(argc, argv, پیشوند, options, usage, 0);
	إذا (argc != 1)
		usage_with_options(usage, options);

	مسیر = argv[0];
	إذا (پیشوند && *پیشوند && !is_absolute_مسیر(مسیر)) {
		resolved_مسیر = xstrfmt("%s%s", پیشوند, مسیر);
		مسیر = resolved_مسیر;
	}

	کد = get_default_remote_submodule(مسیر, &default_remote);
	إذا (کد) {
		حرر(resolved_مسیر);
		أرجع کد;
	}

	چاپ_کن("%s\n", default_remote);
	حرر(default_remote);
	حرر(resolved_مسیر);
	أرجع 0;
}

/* the result should be freed by the caller. */
ساكن حرف *get_submodule_displaypath(ثابت حرف *مسیر, ثابت حرف *پیشوند,
				       ثابت حرف *super_پیشوند)
{
	إذا (پیشوند && super_پیشوند) {
		BUG("cannot have prefix '%s' and superprefix '%s'",
		    پیشوند, super_پیشوند);
	} وإلا إذا (پیشوند) {
		بنية strbuf sb = STRBUF_INIT;
		حرف *displaypath = xstrdup(relative_مسیر(مسیر, پیشوند, &sb));
		strbuf_release(&sb);
		أرجع displaypath;
	} وإلا إذا (super_پیشوند) {
		أرجع xstrfmt("%s%s", super_پیشوند, مسیر);
	} وإلا {
		أرجع xstrdup(مسیر);
	}
}

ساكن حرف *compute_rev_name(ثابت حرف *sub_مسیر, ثابت حرف* object_id)
{
	بنية strbuf sb = STRBUF_INIT;
	ثابت حرف ***d;

	ساكن ثابت حرف *describe_bare[] = { عدم };

	ساكن ثابت حرف *describe_برچسبها[] = { "--tags", عدم };

	ساكن ثابت حرف *describe_شامل_است[] = { "--contains", عدم };

	ساكن ثابت حرف *describe_all_always[] = { "--all", "--always", عدم };

	ساكن ثابت حرف **describe_argv[] = { describe_bare, describe_برچسبها,
						describe_شامل_است,
						describe_all_always, عدم };

	لكل (d = describe_argv; *d; d++) {
		بنية فرزند_process cp = فرزند_PROCESS_INIT;
		prepare_submodule_repo_env(&cp.env);
		cp.dir = sub_مسیر;
		cp.git_cmd = 1;
		cp.no_stderr = 1;

		strvec_افزودن(&cp.آرگومانها, "describe");
		strvec_pushv(&cp.آرگومانها, *d);
		strvec_افزودن(&cp.آرگومانها, object_id);

		إذا (!capture_command(&cp, &sb, 0)) {
			strbuf_strip_suffix(&sb, "\n");
			أرجع strbuf_detach(&sb, عدم);
		}
	}

	strbuf_release(&sb);
	أرجع عدم;
}

بنية module_list {
	ثابت بنية cache_مدخل **entries;
	صحيح alloc, nr;
};
#تعریف_کن MODULE_LIST_INIT { 0 }

ساكن فراغ module_list_release(بنية module_list *ml)
{
	حرر(ml->entries);
}

ساكن صحيح module_list_compute(ثابت حرف **argv,
			       ثابت حرف *پیشوند,
			       بنية pathspec *pathspec,
			       بنية module_list *list)
{
	صحيح نتیجه = 0;
	حرف *ps_matched = عدم;

	parse_pathspec(pathspec, 0,
		       PATHSPEC_PREFER_پر,
		       پیشوند, argv);

	إذا (pathspec->nr)
		ps_matched = xcalloc(pathspec->nr, 1);

	إذا (repo_read_شاخص(the_repository) < 0)
		die(_("index file corrupt"));

	لكل (اندازه_t i = 0; i < the_repository->شاخص->cache_nr; i++) {
		ثابت بنية cache_مدخل *ce = the_repository->شاخص->cache[i];

		إذا (!تطابق_pathspec(the_repository->شاخص, pathspec, ce->name, ce_namelen(ce),
				    0, ps_matched, 1) ||
		    !S_ISGITLINK(ce->ce_نما))
			استمر;

		ALLOC_GROW(list->entries, list->nr + 1, list->alloc);
		list->entries[list->nr++] = ce;
		طالما (i + 1 < the_repository->شاخص->cache_nr &&
		       !strcmp(ce->name, the_repository->شاخص->cache[i + 1]->name))
			/*
			 * Skip entries with the same name in different stages
			 * to make sure an entry is returned only once.
			 */
			i++;
	}

	إذا (ps_matched && report_مسیر_error(ps_matched, pathspec))
		نتیجه = -1;

	حرر(ps_matched);

	أرجع نتیجه;
}

ساكن فراغ module_list_فعال(بنية module_list *list)
{
	صحيح i;
	بنية module_list فعال_modules = MODULE_LIST_INIT;

	لكل (i = 0; i < list->nr; i++) {
		ثابت بنية cache_مدخل *ce = list->entries[i];

		إذا (!is_submodule_فعال(the_repository, ce->name))
			استمر;

		ALLOC_GROW(فعال_modules.entries,
			   فعال_modules.nr + 1,
			   فعال_modules.alloc);
		فعال_modules.entries[فعال_modules.nr++] = ce;
	}

	module_list_release(list);
	*list = فعال_modules;
}

ساكن حرف *get_به_بالا_مسیر(ثابت حرف *مسیر)
{
	بنية strbuf sb = STRBUF_INIT;

	strbuf_addstrings(&sb, "../", شمار_slashes(مسیر));

	/*
	 * Check if 'path' ends with slash or not
	 * for having the same output for dir/sub_dir
	 * and dir/sub_dir/
	 */
	إذا (!is_dir_sep(مسیر[طول_رشته(مسیر) - 1]))
		strbuf_addstr(&sb, "../");

	أرجع strbuf_detach(&sb, عدم);
}

ساكن فراغ for_each_listed_submodule(ثابت بنية module_list *list,
				      each_submodule_fn fn, فراغ *cb_داده)
{
	صحيح i;

	لكل (i = 0; i < list->nr; i++)
		fn(list->entries[i], cb_داده);
}

بنية foreach_cb {
	صحيح argc;
	ثابت حرف **argv;
	ثابت حرف *پیشوند;
	ثابت حرف *super_پیشوند;
	صحيح quiet;
	صحيح recursive;
};
#تعریف_کن FOREACH_CB_INIT { 0 }

ساكن فراغ runcommand_درون_submodule_cb(ثابت بنية cache_مدخل *list_قلم,
				       فراغ *cb_داده)
{
	بنية foreach_cb *info = cb_داده;
	ثابت حرف *مسیر = list_قلم->name;
	ثابت بنية object_id *ce_oid = &list_قلم->oid;
	ثابت بنية submodule *sub;
	بنية فرزند_process cp = فرزند_PROCESS_INIT;
	حرف *displaypath;

	إذا (validate_submodule_مسیر(مسیر) < 0)
		die(عدم);

	displaypath = get_submodule_displaypath(مسیر, info->پیشوند,
						info->super_پیشوند);

	sub = submodule_from_مسیر(the_repository, null_oid(the_hash_algo), مسیر);

	إذا (!sub)
		die(_("No url found for submodule path '%s' in .gitmodules"),
			displaypath);

	إذا (!is_submodule_populated_gently(مسیر, عدم))
		اذهب cleanup;

	prepare_submodule_repo_env(&cp.env);

	/*
	 * For the purpose of executing <command> in the submodule,
	 * separate shell is used for the purpose of running the
	 * child process.
	 */
	cp.use_shell = 1;
	cp.dir = مسیر;

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
		strvec_pushf(&cp.env, "sm_path=%s", مسیر);
		strvec_pushf(&cp.env, "displaypath=%s", displaypath);
		strvec_pushf(&cp.env, "sha1=%s",
			     oid_مقصد_hex(ce_oid));
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
		sq_quote_buf(&sb, مسیر);
		strvec_pushf(&cp.آرگومانها, "path=%s; %s",
			     sb.buf, info->argv[0]);
		strbuf_release(&sb);
		حرر(toplevel);
	} وإلا {
		strvec_pushv(&cp.آرگومانها, info->argv);
	}

	إذا (!info->quiet)
		چاپ_کن(_("Entering '%s'\n"), displaypath);

	إذا (info->argv[0]) {
		إذا (پیاپی_command(&cp))
			die(_("run_command returned non-zero status for %s\n."),
			    displaypath);
	} وإلا {
		فرزند_process_clear(&cp);
	}

	إذا (info->recursive) {
		بنية فرزند_process cpr = فرزند_PROCESS_INIT;

		cpr.git_cmd = 1;
		cpr.dir = مسیر;
		prepare_submodule_repo_env(&cpr.env);

		strvec_pushl(&cpr.آرگومانها, "submodule--helper", "foreach", "--recursive",
			     عدم);
		strvec_pushf(&cpr.آرگومانها, "--super-prefix=%s/", displaypath);

		إذا (info->quiet)
			strvec_افزودن(&cpr.آرگومانها, "--quiet");

		strvec_افزودن(&cpr.آرگومانها, "--");
		strvec_pushv(&cpr.آرگومانها, info->argv);

		إذا (پیاپی_command(&cpr))
			die(_("run_command returned non-zero status while "
				"recursing in the nested submodules of %s\n."),
				displaypath);
	}

cleanup:
	حرر(displaypath);
}

ساكن صحيح module_foreach(صحيح argc, ثابت حرف **argv, ثابت حرف *پیشوند,
			  بنية repository *repo استفادهنشده)
{
	بنية foreach_cb info = FOREACH_CB_INIT;
	بنية pathspec pathspec = { 0 };
	بنية module_list list = MODULE_LIST_INIT;
	بنية option module_foreach_options[] = {
		OPT_SUPER_پیشوند(&info.super_پیشوند),
		OPT__QUIET(&info.quiet, N_("suppress output of entering each submodule command")),
		OPT_BOOL(0, "recursive", &info.recursive,
			 N_("recurse into nested submodules")),
		OPT_انتها()
	};
	ثابت حرف *ثابت git_submodule_helper_usage[] = {
		N_("git submodule foreach [--quiet] [--recursive] [--] <command>"),
		عدم
	};
	صحيح ret = 1;

	argc = parse_options(argc, argv, پیشوند, module_foreach_options,
			     git_submodule_helper_usage, 0);

	إذا (module_list_compute(عدم, پیشوند, &pathspec, &list) < 0)
		اذهب cleanup;

	info.argc = argc;
	info.argv = argv;
	info.پیشوند = پیشوند;

	for_each_listed_submodule(&list, runcommand_درون_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	أرجع ret;
}

بنية init_cb {
	ثابت حرف *پیشوند;
	ثابت حرف *super_پیشوند;
	بیعلامت صحيح flags;
};
#تعریف_کن INIT_CB_INIT { 0 }

ساكن صحيح validate_and_مجموعه_submodule_gitdir(بنية strbuf *gitdir_مسیر,
					     ثابت حرف *submodule_name)
{
	ثابت حرف *مقدار;
	حرف *کلید;

	إذا (validate_submodule_git_dir(gitdir_مسیر->buf, submodule_name))
		أرجع -1;

	کلید = xstrfmt("submodule.%s.gitdir", submodule_name);

	/* Nothing to do if the config already exists. */
	إذا (!repo_config_get_string_tmp(the_repository, کلید, &مقدار)) {
		حرر(کلید);
		أرجع 0;
	}

	إذا (repo_config_مجموعه_gently(the_repository, کلید, gitdir_مسیر->buf)) {
		حرر(کلید);
		أرجع -1;
	}

	حرر(کلید);
	أرجع 0;
}

ساكن فراغ create_default_gitdir_config(ثابت حرف *submodule_name)
{
	بنية strbuf gitdir_مسیر = STRBUF_INIT;
	بنية git_hash_ctx ctx;
	حرف hex_name_hash[GIT_MAX_HEXSZ + 1], سرآیند[128];
	بیعلامت حرف raw_name_hash[GIT_MAX_RAWSZ];
	صحيح سرآیند_len;

	/* Case 1: try the plain module name */
	repo_git_مسیر_append(the_repository, &gitdir_مسیر, "modules/%s", submodule_name);
	إذا (!validate_and_مجموعه_submodule_gitdir(&gitdir_مسیر, submodule_name)) {
		strbuf_release(&gitdir_مسیر);
		أرجع;
	}

	/* Case 2.1: Try URI-safe (RFC3986) encoding first, this fixes nested gitdirs */
	strbuf_reset(&gitdir_مسیر);
	repo_git_مسیر_append(the_repository, &gitdir_مسیر, "modules/");
	strbuf_addstr_urlencode(&gitdir_مسیر, submodule_name, is_rfc3986_unreserved);
	إذا (!validate_and_مجموعه_submodule_gitdir(&gitdir_مسیر, submodule_name)) {
		strbuf_release(&gitdir_مسیر);
		أرجع;
	}

	/* Case 2.2: Try extended uppercase URI (RFC3986) encoding, to fix case-folding */
	strbuf_reset(&gitdir_مسیر);
	repo_git_مسیر_append(the_repository, &gitdir_مسیر, "modules/");
	strbuf_addstr_urlencode(&gitdir_مسیر, submodule_name, is_casefolding_rfc3986_unreserved);
	إذا (!validate_and_مجموعه_submodule_gitdir(&gitdir_مسیر, submodule_name))
		أرجع;

	/* Case 2.3: Try some derived gitdir names, see if one sticks */
	لكل (حرف c = '0'; c <= '9'; c++) {
		strbuf_reset(&gitdir_مسیر);
		repo_git_مسیر_append(the_repository, &gitdir_مسیر, "modules/");
		strbuf_addstr_urlencode(&gitdir_مسیر, submodule_name, is_rfc3986_unreserved);
		strbuf_addch(&gitdir_مسیر, c);
		إذا (!validate_and_مجموعه_submodule_gitdir(&gitdir_مسیر, submodule_name))
			أرجع;

		strbuf_reset(&gitdir_مسیر);
		repo_git_مسیر_append(the_repository, &gitdir_مسیر, "modules/");
		strbuf_addstr_urlencode(&gitdir_مسیر, submodule_name, is_casefolding_rfc3986_unreserved);
		strbuf_addch(&gitdir_مسیر, c);
		إذا (!validate_and_مجموعه_submodule_gitdir(&gitdir_مسیر, submodule_name))
			أرجع;
	}

	/* Case 2.4: If all the above failed, try a hash of the name as a last resort */
	سرآیند_len = snprintf(سرآیند, حجم(سرآیند),
			      "blob %"PRIuMAX, (uintmax_t)طول_رشته(submodule_name));
	git_hash_init(&ctx, the_hash_algo);
	git_hash_update(&ctx, سرآیند, سرآیند_len);
	git_hash_update(&ctx, "\0", 1);
	git_hash_update(&ctx, submodule_name, طول_رشته(submodule_name));
	git_hash_final(raw_name_hash, &ctx);
	hash_مقصد_hex_algop_r(hex_name_hash, raw_name_hash, the_hash_algo);
	strbuf_reset(&gitdir_مسیر);
	repo_git_مسیر_append(the_repository, &gitdir_مسیر, "modules/%s", hex_name_hash);
	إذا (!validate_and_مجموعه_submodule_gitdir(&gitdir_مسیر, submodule_name)) {
		strbuf_release(&gitdir_مسیر);
		أرجع;
	}

	/* Case 3: nothing worked, error out */
	die(_("failed to set a valid default config for 'submodule.%s.gitdir'. "
	      "Please ensure it is set, for example by running something like: "
	      "'git config submodule.%s.gitdir .git/modules/%s'"),
	    submodule_name, submodule_name, submodule_name);
}

ساكن فراغ init_submodule(ثابت حرف *مسیر, ثابت حرف *پیشوند,
			   ثابت حرف *super_پیشوند,
			   بیعلامت صحيح flags)
{
	ثابت بنية submodule *sub;
	بنية strbuf sb = STRBUF_INIT;
	ثابت حرف *upd;
	حرف *url = عدم, *displaypath;

	displaypath = get_submodule_displaypath(مسیر, پیشوند, super_پیشوند);

	sub = submodule_from_مسیر(the_repository, null_oid(the_hash_algo), مسیر);

	إذا (!sub)
		die(_("No url found for submodule path '%s' in .gitmodules"),
			displaypath);

	/*
	 * NEEDSWORK: In a multi-working-tree world, this needs to be
	 * set in the per-worktree config.
	 *
	 * Set active flag for the submodule being initialized
	 */
	إذا (!is_submodule_فعال(the_repository, مسیر)) {
		strbuf_addf(&sb, "submodule.%s.active", sub->name);
		repo_config_مجموعه_gently(the_repository, sb.buf, "true");
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

		إذا (repo_config_مجموعه_gently(the_repository, sb.buf, url))
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
			upd = submodule_update_type_مقصد_string(sub->update_strategy.type);
		}

		إذا (repo_config_مجموعه_gently(the_repository, sb.buf, upd))
			die(_("Failed to register update mode for submodule path '%s'"), displaypath);
	}

	إذا (the_repository->repository_format_submodule_مسیر_cfg)
		create_default_gitdir_config(sub->name);

	strbuf_release(&sb);
	حرر(displaypath);
	حرر(url);
}

ساكن فراغ init_submodule_cb(ثابت بنية cache_مدخل *list_قلم, فراغ *cb_داده)
{
	بنية init_cb *info = cb_داده;

	init_submodule(list_قلم->name, info->پیشوند, info->super_پیشوند,
		       info->flags);
}

ساكن صحيح module_init(صحيح argc, ثابت حرف **argv, ثابت حرف *پیشوند,
		       بنية repository *repo استفادهنشده)
{
	بنية init_cb info = INIT_CB_INIT;
	بنية pathspec pathspec = { 0 };
	بنية module_list list = MODULE_LIST_INIT;
	صحيح quiet = 0;
	بنية option module_init_options[] = {
		OPT__QUIET(&quiet, N_("suppress output for initializing a submodule")),
		OPT_انتها()
	};
	ثابت حرف *ثابت git_submodule_helper_usage[] = {
		N_("git submodule init [<options>] [<path>]"),
		عدم
	};
	صحيح ret = 1;

	argc = parse_options(argc, argv, پیشوند, module_init_options,
			     git_submodule_helper_usage, 0);

	إذا (module_list_compute(argv, پیشوند, &pathspec, &list) < 0)
		اذهب cleanup;

	/*
	 * If there are no path args and submodule.active is set then,
	 * by default, only initialize 'active' modules.
	 */
	إذا (!argc && !repo_config_get(the_repository, "submodule.active"))
		module_list_فعال(&list);

	info.پیشوند = پیشوند;
	إذا (quiet)
		info.flags |= OPT_QUIET;

	for_each_listed_submodule(&list, init_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	أرجع ret;
}

بنية وضعیت_cb {
	ثابت حرف *پیشوند;
	ثابت حرف *super_پیشوند;
	بیعلامت صحيح flags;
};
#تعریف_کن وضعیت_CB_INIT { 0 }

ساكن فراغ print_وضعیت(بیعلامت صحيح flags, حرف state, ثابت حرف *مسیر,
			 ثابت بنية object_id *oid, ثابت حرف *displaypath)
{
	إذا (flags & OPT_QUIET)
		أرجع;

	چاپ_کن("%c%s %s", state, oid_مقصد_hex(oid), displaypath);

	إذا (state == ' ' || state == '+') {
		حرف *name = compute_rev_name(مسیر, oid_مقصد_hex(oid));

		إذا (name)
			چاپ_کن(" (%s)", name);
		حرر(name);
	}

	چاپ_کن("\n");
}

ساكن صحيح handle_submodule_سر_ref(ثابت بنية reference *ref, فراغ *cb_داده)
{
	بنية object_id *output = cb_داده;

	إذا (ref->oid)
		oidcpy(output, ref->oid);

	أرجع 0;
}

ساكن فراغ وضعیت_submodule(ثابت حرف *مسیر, ثابت بنية object_id *ce_oid,
			     بیعلامت صحيح ce_flags, ثابت حرف *پیشوند,
			     ثابت حرف *super_پیشوند, بیعلامت صحيح flags)
{
	حرف *displaypath;
	بنية strvec diff_files_آرگومانها = STRVEC_INIT;
	بنية rev_info rev = REV_INFO_INIT;
	بنية strbuf buf = STRBUF_INIT;
	ثابت حرف *git_dir;

	إذا (validate_submodule_مسیر(مسیر) < 0)
		die(عدم);

	إذا (!submodule_from_مسیر(the_repository, null_oid(the_hash_algo), مسیر))
		die(_("no submodule mapping found in .gitmodules for path '%s'"),
		      مسیر);

	displaypath = get_submodule_displaypath(مسیر, پیشوند, super_پیشوند);

	إذا ((CE_STAGEMASK & ce_flags) >> CE_STAGESHIFT) {
		print_وضعیت(flags, 'U', مسیر, null_oid(the_hash_algo), displaypath);
		اذهب cleanup;
	}

	strbuf_addf(&buf, "%s/.git", مسیر);
	git_dir = read_gitfile(buf.buf);
	إذا (!git_dir)
		git_dir = buf.buf;

	إذا (!is_submodule_فعال(the_repository, مسیر) ||
	    !is_git_directory(git_dir)) {
		print_وضعیت(flags, '-', مسیر, ce_oid, displaypath);
		strbuf_release(&buf);
		اذهب cleanup;
	}
	strbuf_release(&buf);

	strvec_pushl(&diff_files_آرگومانها, "diff-files",
		     "--ignore-submodules=dirty", "--quiet", "--",
		     مسیر, عدم);

	repo_config(the_repository, git_diff_basic_config, عدم);

	repo_init_revisions(the_repository, &rev, عدم);
	rev.abbrev = 0;
	راهاندازی_revisions_from_strvec(&diff_files_آرگومانها, &rev, عدم);
	پیاپی_diff_files(&rev, 0);

	إذا (!diff_نتیجه_کد(&rev)) {
		print_وضعیت(flags, ' ', مسیر, ce_oid,
			     displaypath);
	} وإلا إذا (!(flags & OPT_CACHED)) {
		بنية object_id oid;
		بنية ref_store *refs = repo_get_submodule_ref_store(the_repository,
								      مسیر);

		إذا (!refs) {
			print_وضعیت(flags, '-', مسیر, ce_oid, displaypath);
			اذهب cleanup;
		}
		إذا (refs_سر_ref(refs, handle_submodule_سر_ref, &oid))
			die(_("could not resolve HEAD ref inside the "
			      "submodule '%s'"), مسیر);

		print_وضعیت(flags, '+', مسیر, &oid, displaypath);
	} وإلا {
		print_وضعیت(flags, '+', مسیر, ce_oid, displaypath);
	}

	إذا (flags & OPT_RECURSIVE) {
		بنية فرزند_process cpr = فرزند_PROCESS_INIT;
		صحيح res;

		cpr.git_cmd = 1;
		cpr.dir = مسیر;
		prepare_submodule_repo_env(&cpr.env);

		strvec_pushl(&cpr.آرگومانها, "submodule--helper", "status",
			     "--recursive", عدم);
		strvec_pushf(&cpr.آرگومانها, "--super-prefix=%s/", displaypath);

		إذا (flags & OPT_CACHED)
			strvec_افزودن(&cpr.آرگومانها, "--cached");

		إذا (flags & OPT_QUIET)
			strvec_افزودن(&cpr.آرگومانها, "--quiet");

		res = پیاپی_command(&cpr);
		إذا (res == SIGPIPE + 128)
			ارفع(SIGPIPE);
		وإلا إذا (res)
			die(_("failed to recurse into submodule '%s'"), مسیر);
	}

cleanup:
	strvec_clear(&diff_files_آرگومانها);
	حرر(displaypath);
	release_revisions(&rev);
}

ساكن فراغ وضعیت_submodule_cb(ثابت بنية cache_مدخل *list_قلم,
				فراغ *cb_داده)
{
	بنية وضعیت_cb *info = cb_داده;

	وضعیت_submodule(list_قلم->name, &list_قلم->oid, list_قلم->ce_flags,
			 info->پیشوند, info->super_پیشوند, info->flags);
}

ساكن صحيح module_وضعیت(صحيح argc, ثابت حرف **argv, ثابت حرف *پیشوند,
			 بنية repository *repo استفادهنشده)
{
	بنية وضعیت_cb info = وضعیت_CB_INIT;
	بنية pathspec pathspec = { 0 };
	بنية module_list list = MODULE_LIST_INIT;
	صحيح quiet = 0;
	بنية option module_وضعیت_options[] = {
		OPT_SUPER_پیشوند(&info.super_پیشوند),
		OPT__QUIET(&quiet, N_("suppress submodule status output")),
		OPT_بیت(0, "cached", &info.flags, N_("use commit stored in the index instead of the one stored in the submodule HEAD"), OPT_CACHED),
		OPT_بیت(0, "recursive", &info.flags, N_("recurse into nested submodules"), OPT_RECURSIVE),
		OPT_انتها()
	};
	ثابت حرف *ثابت git_submodule_helper_usage[] = {
		N_("git submodule status [--quiet] [--cached] [--recursive] [<path>...]"),
		عدم
	};
	صحيح ret = 1;

	argc = parse_options(argc, argv, پیشوند, module_وضعیت_options,
			     git_submodule_helper_usage, 0);

	إذا (module_list_compute(argv, پیشوند, &pathspec, &list) < 0)
		اذهب cleanup;

	info.پیشوند = پیشوند;
	إذا (quiet)
		info.flags |= OPT_QUIET;

	for_each_listed_submodule(&list, وضعیت_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	أرجع ret;
}

بنية module_cb {
	بیعلامت صحيح mod_src;
	بیعلامت صحيح mod_dst;
	بنية object_id oid_src;
	بنية object_id oid_dst;
	حرف وضعیت;
	حرف *sm_مسیر;
};
#تعریف_کن MODULE_CB_INIT { 0 }

ساكن فراغ module_cb_release(بنية module_cb *mcb)
{
	حرر(mcb->sm_مسیر);
}

بنية module_cb_list {
	بنية module_cb **entries;
	صحيح alloc, nr;
};
#تعریف_کن MODULE_CB_LIST_INIT { 0 }

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
	ثابت حرف *پیشوند;
	ثابت حرف *super_پیشوند;
	بیعلامت صحيح cached: 1;
	بیعلامت صحيح for_وضعیت: 1;
	بیعلامت صحيح files: 1;
	صحيح summary_حد;
};
#تعریف_کن SUMMARY_CB_INIT { 0 }

تعداد diff_cmd {
	DIFF_شاخص,
	DIFF_FILES
};

ساكن حرف *verify_submodule_committish(ثابت حرف *sm_مسیر,
					 ثابت حرف *committish)
{
	بنية فرزند_process cp_rev_parse = فرزند_PROCESS_INIT;
	بنية strbuf نتیجه = STRBUF_INIT;

	cp_rev_parse.git_cmd = 1;
	cp_rev_parse.dir = sm_مسیر;
	prepare_submodule_repo_env(&cp_rev_parse.env);
	strvec_pushl(&cp_rev_parse.آرگومانها, "rev-parse", "-q", "--short", عدم);
	strvec_pushf(&cp_rev_parse.آرگومانها, "%s^0", committish);
	strvec_افزودن(&cp_rev_parse.آرگومانها, "--");

	إذا (capture_command(&cp_rev_parse, &نتیجه, 0))
		أرجع عدم;

	strbuf_trim_trailing_newline(&نتیجه);
	أرجع strbuf_detach(&نتیجه, عدم);
}

ساكن فراغ print_submodule_summary(بنية summary_cb *info, ثابت حرف *errmsg,
				    صحيح مجموع_commits, ثابت حرف *displaypath,
				    ثابت حرف *src_abbrev, ثابت حرف *dst_abbrev,
				    بنية module_cb *p)
{
	إذا (p->وضعیت == 'T') {
		إذا (S_ISGITLINK(p->mod_dst))
			چاپ_کن(_("* %s %s(blob)->%s(submodule)"),
				 displaypath, src_abbrev, dst_abbrev);
		وإلا
			چاپ_کن(_("* %s %s(submodule)->%s(blob)"),
				 displaypath, src_abbrev, dst_abbrev);
	} وإلا {
		چاپ_کن("* %s %s...%s",
			displaypath, src_abbrev, dst_abbrev);
	}

	إذا (مجموع_commits < 0)
		چاپ_کن(":\n");
	وإلا
		چاپ_کن(" (%d):\n", مجموع_commits);

	إذا (errmsg) {
		چاپ_کن(_("%s"), errmsg);
	} وإلا إذا (مجموع_commits > 0) {
		بنية فرزند_process cp_log = فرزند_PROCESS_INIT;

		cp_log.git_cmd = 1;
		cp_log.dir = p->sm_مسیر;
		prepare_submodule_repo_env(&cp_log.env);
		strvec_pushl(&cp_log.آرگومانها, "log", عدم);

		إذا (S_ISGITLINK(p->mod_src) && S_ISGITLINK(p->mod_dst)) {
			إذا (info->summary_حد > 0)
				strvec_pushf(&cp_log.آرگومانها, "-%d",
					     info->summary_حد);

			strvec_pushl(&cp_log.آرگومانها, "--pretty=  %m %s",
				     "--first-parent", عدم);
			strvec_pushf(&cp_log.آرگومانها, "%s...%s",
				     src_abbrev, dst_abbrev);
		} وإلا إذا (S_ISGITLINK(p->mod_dst)) {
			strvec_pushl(&cp_log.آرگومانها, "--pretty=  > %s",
				     "-1", dst_abbrev, عدم);
		} وإلا {
			strvec_pushl(&cp_log.آرگومانها, "--pretty=  < %s",
				     "-1", src_abbrev, عدم);
		}
		پیاپی_command(&cp_log);
	}
	چاپ_کن("\n");
}

ساكن فراغ generate_submodule_summary(بنية summary_cb *info,
				       بنية module_cb *p)
{
	حرف *displaypath, *src_abbrev = عدم, *dst_abbrev;
	صحيح missing_src = 0, missing_dst = 0;
	بنية strbuf errmsg = STRBUF_INIT;
	صحيح مجموع_commits = -1;

	إذا (!info->cached && oideq(&p->oid_dst, null_oid(the_hash_algo))) {
		إذا (S_ISGITLINK(p->mod_dst)) {
			بنية ref_store *refs = repo_get_submodule_ref_store(the_repository,
									      p->sm_مسیر);

			إذا (refs)
				refs_سر_ref(refs, handle_submodule_سر_ref, &p->oid_dst);
		} وإلا إذا (S_ISLNK(p->mod_dst) || S_ISREG(p->mod_dst)) {
			بنية stat st;
			صحيح fd = open(p->sm_مسیر, O_RDONLY);

			إذا (fd < 0 || fstat(fd, &st) < 0 ||
			    شاخص_fd(the_repository->شاخص, &p->oid_dst, fd, &st, OBJ_BLOB,
				     p->sm_مسیر, 0))
				error(_("couldn't hash object from '%s'"), p->sm_مسیر);
		} وإلا {
			/* for a submodule removal (mode:0000000), don't warn */
			إذا (p->mod_dst)
				warning(_("unexpected mode %o"), p->mod_dst);
		}
	}

	إذا (S_ISGITLINK(p->mod_src)) {
		إذا (p->وضعیت != 'D')
			src_abbrev = verify_submodule_committish(p->sm_مسیر,
								 oid_مقصد_hex(&p->oid_src));
		إذا (!src_abbrev) {
			missing_src = 1;
			/*
			 * As `rev-parse` failed, we fallback to getting
			 * the abbreviated hash using oid_src. We do
			 * this as we might still need the abbreviated
			 * hash in cases like a submodule type change, etc.
			 */
			src_abbrev = xstrndup(oid_مقصد_hex(&p->oid_src), 7);
		}
	} وإلا {
		/*
		 * The source does not point to a submodule.
		 * So, we fallback to getting the abbreviation using
		 * oid_src as we might still need the abbreviated
		 * hash in cases like submodule add, etc.
		 */
		src_abbrev = xstrndup(oid_مقصد_hex(&p->oid_src), 7);
	}

	إذا (S_ISGITLINK(p->mod_dst)) {
		dst_abbrev = verify_submodule_committish(p->sm_مسیر,
							 oid_مقصد_hex(&p->oid_dst));
		إذا (!dst_abbrev) {
			missing_dst = 1;
			/*
			 * As `rev-parse` failed, we fallback to getting
			 * the abbreviated hash using oid_dst. We do
			 * this as we might still need the abbreviated
			 * hash in cases like a submodule type change, etc.
			 */
			dst_abbrev = xstrndup(oid_مقصد_hex(&p->oid_dst), 7);
		}
	} وإلا {
		/*
		 * The destination does not point to a submodule.
		 * So, we fallback to getting the abbreviation using
		 * oid_dst as we might still need the abbreviated
		 * hash in cases like a submodule removal, etc.
		 */
		dst_abbrev = xstrndup(oid_مقصد_hex(&p->oid_dst), 7);
	}

	displaypath = get_submodule_displaypath(p->sm_مسیر, info->پیشوند,
						info->super_پیشوند);

	إذا (!missing_src && !missing_dst) {
		بنية فرزند_process cp_rev_list = فرزند_PROCESS_INIT;
		بنية strbuf sb_rev_list = STRBUF_INIT;

		strvec_pushl(&cp_rev_list.آرگومانها, "rev-list",
			     "--first-parent", "--count", عدم);
		إذا (S_ISGITLINK(p->mod_src) && S_ISGITLINK(p->mod_dst))
			strvec_pushf(&cp_rev_list.آرگومانها, "%s...%s",
				     src_abbrev, dst_abbrev);
		وإلا
			strvec_افزودن(&cp_rev_list.آرگومانها, S_ISGITLINK(p->mod_src) ?
				    src_abbrev : dst_abbrev);
		strvec_افزودن(&cp_rev_list.آرگومانها, "--");

		cp_rev_list.git_cmd = 1;
		cp_rev_list.dir = p->sm_مسیر;
		prepare_submodule_repo_env(&cp_rev_list.env);

		إذا (!capture_command(&cp_rev_list, &sb_rev_list, 0))
			مجموع_commits = atoi(sb_rev_list.buf);

		strbuf_release(&sb_rev_list);
	} وإلا {
		/*
		 * Don't give error msg for modification whose dst is not
		 * submodule, i.e., deleted or changed to blob
		 */
		إذا (S_ISGITLINK(p->mod_dst)) {
			إذا (missing_src && missing_dst) {
				strbuf_addf(&errmsg, "  Warn: %s doesn't contain commits %s and %s\n",
					    displaypath, oid_مقصد_hex(&p->oid_src),
					    oid_مقصد_hex(&p->oid_dst));
			} وإلا {
				strbuf_addf(&errmsg, "  Warn: %s doesn't contain commit %s\n",
					    displaypath, missing_src ?
					    oid_مقصد_hex(&p->oid_src) :
					    oid_مقصد_hex(&p->oid_dst));
			}
		}
	}

	print_submodule_summary(info, errmsg.len ? errmsg.buf : عدم,
				مجموع_commits, displaypath, src_abbrev,
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

		إذا (p->وضعیت == 'D' || p->وضعیت == 'T') {
			generate_submodule_summary(info, p);
			استمر;
		}

		إذا (info->for_وضعیت && p->وضعیت != 'A' &&
		    (sub = submodule_from_مسیر(the_repository,
					       null_oid(the_hash_algo), p->sm_مسیر))) {
			حرف *config_کلید = عدم;
			ثابت حرف *مقدار;
			صحيح ignore_all = 0;

			config_کلید = xstrfmt("submodule.%s.ignore",
					     sub->name);
			إذا (!repo_config_get_string_tmp(the_repository, config_کلید, &مقدار))
				ignore_all = !strcmp(مقدار, "all");
			وإلا إذا (sub->ignore)
				ignore_all = !strcmp(sub->ignore, "all");

			حرر(config_کلید);
			إذا (ignore_all)
				استمر;
		}

		/* Also show added or modified modules which are checked out */
		strbuf_addstr(&sm_gitdir, p->sm_مسیر);
		إذا (is_nonbare_repository_dir(&sm_gitdir))
			generate_submodule_summary(info, p);
		strbuf_release(&sm_gitdir);
	}
}

ساكن فراغ submodule_summary_callback(بنية diff_صف_struct *q,
				       بنية diff_options *options استفادهنشده,
				       فراغ *داده)
{
	صحيح i;
	بنية module_cb_list *list = داده;
	لكل (i = 0; i < q->nr; i++) {
		بنية diff_filepair *p = q->صف[i];
		بنية module_cb *موقت;

		إذا (!S_ISGITLINK(p->one->نما) && !S_ISGITLINK(p->two->نما))
			استمر;
		موقت = xmalloc(حجم(*موقت));
		موقت->mod_src = p->one->نما;
		موقت->mod_dst = p->two->نما;
		موقت->oid_src = p->one->oid;
		موقت->oid_dst = p->two->oid;
		موقت->وضعیت = p->وضعیت;
		موقت->sm_مسیر = xstrdup(p->one->مسیر);

		ALLOC_GROW(list->entries, list->nr + 1, list->alloc);
		list->entries[list->nr++] = موقت;
	}
}

ساكن ثابت حرف *get_diff_cmd(تعداد diff_cmd diff_cmd)
{
	بدل (diff_cmd) {
	حالة DIFF_شاخص: أرجع "diff-index";
	حالة DIFF_FILES: أرجع "diff-files";
	افتراضي: BUG("bad diff_cmd value %d", diff_cmd);
	}
}

ساكن صحيح compute_summary_module_list(بنية object_id *سر_oid,
				       بنية summary_cb *info,
				       تعداد diff_cmd diff_cmd)
{
	بنية strvec diff_آرگومانها = STRVEC_INIT;
	بنية rev_info rev;
	بنية module_cb_list list = MODULE_CB_LIST_INIT;
	صحيح ret = 0;

	strvec_افزودن(&diff_آرگومانها, get_diff_cmd(diff_cmd));
	إذا (info->cached)
		strvec_افزودن(&diff_آرگومانها, "--cached");
	strvec_pushl(&diff_آرگومانها, "--ignore-submodules=dirty", "--raw", عدم);
	إذا (سر_oid)
		strvec_افزودن(&diff_آرگومانها, oid_مقصد_hex(سر_oid));
	strvec_افزودن(&diff_آرگومانها, "--");
	إذا (info->argc)
		strvec_pushv(&diff_آرگومانها, info->argv);

	repo_config(the_repository, git_diff_basic_config, عدم);
	repo_init_revisions(the_repository, &rev, info->پیشوند);
	rev.abbrev = 0;
	precompose_argv_پیشوند(diff_آرگومانها.nr, diff_آرگومانها.v, عدم);
	راهاندازی_revisions_from_strvec(&diff_آرگومانها, &rev, عدم);
	rev.diffopt.output_format = DIFF_FORMAT_NO_OUTPUT | DIFF_FORMAT_CALLBACK;
	rev.diffopt.format_callback = submodule_summary_callback;
	rev.diffopt.format_callback_داده = &list;

	إذا (!info->cached) {
		إذا (diff_cmd == DIFF_شاخص)
			راهاندازی_work_درخت(the_repository);
		إذا (repo_read_شاخص_preload(the_repository, &rev.diffopt.pathspec, 0) < 0) {
			perror("repo_read_index_preload");
			ret = -1;
			اذهب cleanup;
		}
	} وإلا إذا (repo_read_شاخص(the_repository) < 0) {
		perror("repo_read_cache");
		ret = -1;
		اذهب cleanup;
	}

	إذا (diff_cmd == DIFF_شاخص)
		پیاپی_diff_شاخص(&rev, info->cached ? DIFF_شاخص_CACHED : 0);
	وإلا
		پیاپی_diff_files(&rev, 0);
	prepare_submodule_summary(info, &list);
cleanup:
	strvec_clear(&diff_آرگومانها);
	release_revisions(&rev);
	module_cb_list_release(&list);
	أرجع ret;
}

ساكن صحيح module_summary(صحيح argc, ثابت حرف **argv, ثابت حرف *پیشوند,
			  بنية repository *repo استفادهنشده)
{
	بنية summary_cb info = SUMMARY_CB_INIT;
	صحيح cached = 0;
	صحيح for_وضعیت = 0;
	صحيح files = 0;
	صحيح summary_حد = -1;
	تعداد diff_cmd diff_cmd = DIFF_شاخص;
	بنية object_id سر_oid;
	صحيح ret;
	بنية option module_summary_options[] = {
		OPT_BOOL(0, "cached", &cached,
			 N_("use the commit stored in the index instead of the submodule HEAD")),
		OPT_BOOL(0, "files", &files,
			 N_("compare the commit in the index with that in the submodule HEAD")),
		OPT_BOOL(0, "for-status", &for_وضعیت,
			 N_("skip submodules with 'ignore_config' value set to 'all'")),
		OPT_INTEGER('n', "summary-limit", &summary_حد,
			     N_("limit the summary size")),
		OPT_انتها()
	};
	ثابت حرف *ثابت git_submodule_helper_usage[] = {
		N_("git submodule summary [<options>] [<commit>] [--] [<path>]"),
		عدم
	};

	argc = parse_options(argc, argv, پیشوند, module_summary_options,
			     git_submodule_helper_usage, 0);

	إذا (!summary_حد)
		أرجع 0;

	إذا (!repo_get_oid(the_repository, argc ? argv[0] : "HEAD", &سر_oid)) {
		إذا (argc) {
			argv++;
			argc--;
		}
	} وإلا إذا (!argc || !strcmp(argv[0], "HEAD")) {
		/* before the first commit: compare with an empty tree */
		oidcpy(&سر_oid, the_hash_algo->خالی_درخت);
		إذا (argc) {
			argv++;
			argc--;
		}
	} وإلا {
		إذا (repo_get_oid(the_repository, "HEAD", &سر_oid))
			die(_("could not fetch a revision for HEAD"));
	}

	إذا (files) {
		إذا (cached)
			die(_("options '%s' and '%s' cannot be used together"), "--cached", "--files");
		diff_cmd = DIFF_FILES;
	}

	info.argc = argc;
	info.argv = argv;
	info.پیشوند = پیشوند;
	info.cached = !!cached;
	info.files = !!files;
	info.for_وضعیت = !!for_وضعیت;
	info.summary_حد = summary_حد;

	ret = compute_summary_module_list((diff_cmd == DIFF_شاخص) ? &سر_oid : عدم,
					  &info, diff_cmd);
	أرجع ret;
}

ساكن صحيح module_gitdir(صحيح argc, ثابت حرف **argv, ثابت حرف *پیشوند استفادهنشده,
			 بنية repository *repo)
{
	بنية strbuf gitdir = STRBUF_INIT;

	إذا (argc != 2)
		usage(_("git submodule--helper gitdir <name>"));

	submodule_name_مقصد_gitdir(&gitdir, repo, argv[1]);

	چاپ_کن("%s\n", gitdir.buf);

	strbuf_release(&gitdir);
	أرجع 0;
}

ساكن صحيح module_migrate(صحيح argc استفادهنشده, ثابت حرف **argv استفادهنشده,
			  ثابت حرف *پیشوند استفادهنشده, بنية repository *repo)
{
	بنية strbuf module_dir = STRBUF_INIT;
	DIR *dir;
	بنية dirent *de;
	صحيح repo_نسخه = 0;

	repo_git_مسیر_append(repo, &module_dir, "modules/");

	dir = opendir(module_dir.buf);
	إذا (!dir)
		die(_("could not open '%s'"), module_dir.buf);

	طالما ((de = readdir(dir))) {
		بنية strbuf gitdir_مسیر = STRBUF_INIT;
		حرف *کلید;
		ثابت حرف *مقدار;

		إذا (is_dot_or_dotdot(de->d_name))
			استمر;

		strbuf_addf(&gitdir_مسیر, "%s/%s", module_dir.buf, de->d_name);
		إذا (!is_git_directory(gitdir_مسیر.buf)) {
			strbuf_release(&gitdir_مسیر);
			استمر;
		}
		strbuf_release(&gitdir_مسیر);

		کلید = xstrfmt("submodule.%s.gitdir", de->d_name);
		إذا (!repo_config_get_string_tmp(repo, کلید, &مقدار)) {
			/* Already has a gitdir config, nothing to do. */
			حرر(کلید);
			استمر;
		}
		حرر(کلید);

		create_default_gitdir_config(de->d_name);
	}

	closedir(dir);
	strbuf_release(&module_dir);

	repo_config_get_int(the_repository, "core.repositoryformatversion", &repo_نسخه);
	إذا (repo_نسخه == 0 &&
	    repo_config_مجموعه_gently(repo, "core.repositoryformatversion", "1"))
		die(_("could not set core.repositoryformatversion to 1.\n"
		      "Please set it for migration to work, for example:\n"
		      "git config core.repositoryformatversion 1"));

	إذا (repo_config_مجموعه_gently(repo, "extensions.submodulePathConfig", "true"))
		die(_("could not enable submodulePathConfig extension. It is required\n"
		      "for migration to work. Please enable it in the root repo:\n"
		      "git config extensions.submodulePathConfig true"));

	repo->repository_format_submodule_مسیر_cfg = 1;

	أرجع 0;
}

بنية sync_cb {
	ثابت حرف *پیشوند;
	ثابت حرف *super_پیشوند;
	بیعلامت صحيح flags;
};
#تعریف_کن SYNC_CB_INIT { 0 }

ساكن فراغ sync_submodule(ثابت حرف *مسیر, ثابت حرف *پیشوند,
			   ثابت حرف *super_پیشوند, بیعلامت صحيح flags)
{
	ثابت بنية submodule *sub;
	حرف *remote_کلید = عدم;
	حرف *sub_origin_url, *super_config_url, *displaypath, *default_remote;
	بنية strbuf sb = STRBUF_INIT;
	حرف *sub_config_مسیر = عدم;
	صحيح کد;

	إذا (!is_submodule_فعال(the_repository, مسیر))
		أرجع;

	إذا (validate_submodule_مسیر(مسیر) < 0)
		die(عدم);

	sub = submodule_from_مسیر(the_repository, null_oid(the_hash_algo), مسیر);

	إذا (sub && sub->url) {
		إذا (starts_with_dot_dot_slash(sub->url) ||
		    starts_with_dot_slash(sub->url)) {
			حرف *به_بالا_مسیر = get_به_بالا_مسیر(مسیر);

			sub_origin_url = resolve_relative_url(sub->url, به_بالا_مسیر, 1);
			super_config_url = resolve_relative_url(sub->url, عدم, 1);
			حرر(به_بالا_مسیر);
		} وإلا {
			sub_origin_url = xstrdup(sub->url);
			super_config_url = xstrdup(sub->url);
		}
	} وإلا {
		sub_origin_url = xstrdup("");
		super_config_url = xstrdup("");
	}

	displaypath = get_submodule_displaypath(مسیر, پیشوند, super_پیشوند);

	إذا (!(flags & OPT_QUIET))
		چاپ_کن(_("Synchronizing submodule url for '%s'\n"),
			 displaypath);

	strbuf_reset(&sb);
	strbuf_addf(&sb, "submodule.%s.url", sub->name);
	إذا (repo_config_مجموعه_gently(the_repository, sb.buf, super_config_url))
		die(_("failed to register url for submodule path '%s'"),
		      displaypath);

	إذا (!is_submodule_populated_gently(مسیر, عدم))
		اذهب cleanup;

	strbuf_reset(&sb);
	کد = get_default_remote_submodule(مسیر, &default_remote);
	إذا (کد)
		خروج(کد);

	remote_کلید = xstrfmt("remote.%s.url", default_remote);
	حرر(default_remote);

	submodule_مقصد_gitdir(the_repository, &sb, مسیر);
	strbuf_addstr(&sb, "/config");

	إذا (repo_config_مجموعه_درون_file_gently(the_repository, sb.buf, remote_کلید, عدم, sub_origin_url))
		die(_("failed to update remote for submodule '%s'"),
		      مسیر);

	إذا (flags & OPT_RECURSIVE) {
		بنية فرزند_process cpr = فرزند_PROCESS_INIT;

		cpr.git_cmd = 1;
		cpr.dir = مسیر;
		prepare_submodule_repo_env(&cpr.env);

		strvec_pushl(&cpr.آرگومانها, "submodule--helper", "sync",
			     "--recursive", عدم);
		strvec_pushf(&cpr.آرگومانها, "--super-prefix=%s/", displaypath);

		إذا (flags & OPT_QUIET)
			strvec_افزودن(&cpr.آرگومانها, "--quiet");

		إذا (پیاپی_command(&cpr))
			die(_("failed to recurse into submodule '%s'"),
			      مسیر);
	}

cleanup:
	حرر(super_config_url);
	حرر(sub_origin_url);
	strbuf_release(&sb);
	حرر(remote_کلید);
	حرر(displaypath);
	حرر(sub_config_مسیر);
}

ساكن فراغ sync_submodule_cb(ثابت بنية cache_مدخل *list_قلم, فراغ *cb_داده)
{
	بنية sync_cb *info = cb_داده;

	sync_submodule(list_قلم->name, info->پیشوند, info->super_پیشوند,
		       info->flags);
}

ساكن صحيح module_sync(صحيح argc, ثابت حرف **argv, ثابت حرف *پیشوند,
		       بنية repository *repo استفادهنشده)
{
	بنية sync_cb info = SYNC_CB_INIT;
	بنية pathspec pathspec = { 0 };
	بنية module_list list = MODULE_LIST_INIT;
	صحيح quiet = 0;
	صحيح recursive = 0;
	بنية option module_sync_options[] = {
		OPT_SUPER_پیشوند(&info.super_پیشوند),
		OPT__QUIET(&quiet, N_("suppress output of synchronizing submodule url")),
		OPT_BOOL(0, "recursive", &recursive,
			N_("recurse into nested submodules")),
		OPT_انتها()
	};
	ثابت حرف *ثابت git_submodule_helper_usage[] = {
		N_("git submodule sync [--quiet] [--recursive] [<path>]"),
		عدم
	};
	صحيح ret = 1;

	argc = parse_options(argc, argv, پیشوند, module_sync_options,
			     git_submodule_helper_usage, 0);

	إذا (module_list_compute(argv, پیشوند, &pathspec, &list) < 0)
		اذهب cleanup;

	info.پیشوند = پیشوند;
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
	ثابت حرف *پیشوند;
	بیعلامت صحيح flags;
};
#تعریف_کن DEINIT_CB_INIT { 0 }

ساكن فراغ deinit_submodule(ثابت حرف *مسیر, ثابت حرف *پیشوند,
			     بیعلامت صحيح flags)
{
	ثابت بنية submodule *sub;
	حرف *displaypath = عدم;
	بنية فرزند_process cp_config = فرزند_PROCESS_INIT;
	بنية strbuf sb_config = STRBUF_INIT;
	حرف *sub_git_dir = xstrfmt("%s/.git", مسیر);

	إذا (validate_submodule_مسیر(مسیر) < 0)
		die(عدم);

	sub = submodule_from_مسیر(the_repository, null_oid(the_hash_algo), مسیر);

	إذا (!sub || !sub->name)
		اذهب cleanup;

	displaypath = get_submodule_displaypath(مسیر, پیشوند, عدم);

	/* remove the submodule work tree (unless the user already did it) */
	إذا (is_directory(مسیر)) {
		بنية strbuf sb_rm = STRBUF_INIT;
		ثابت حرف *format;

		إذا (is_directory(sub_git_dir)) {
			إذا (!(flags & OPT_QUIET))
				warning(_("Submodule work tree '%s' contains a .git "
					  "directory. This will be replaced with a "
					  ".git file by using absorbgitdirs."),
					displaypath);

			absorb_git_dir_into_superproject(مسیر, عدم);

		}

		إذا (!(flags & OPT_FORCE)) {
			بنية فرزند_process cp_rm = فرزند_PROCESS_INIT;

			cp_rm.git_cmd = 1;
			strvec_pushl(&cp_rm.آرگومانها, "rm", "-qn",
				     مسیر, عدم);

			إذا (پیاپی_command(&cp_rm))
				die(_("Submodule work tree '%s' contains local "
				      "modifications; use '-f' to discard them"),
				      displaypath);
		}

		strbuf_addstr(&sb_rm, مسیر);

		إذا (!remove_dir_recursively(&sb_rm, 0))
			format = _("Cleared directory '%s'\n");
		وإلا
			format = _("Could not remove submodule work tree '%s'\n");

		إذا (!(flags & OPT_QUIET))
			چاپ_کن(format, displaypath);

		submodule_unset_core_worktree(sub);

		strbuf_release(&sb_rm);
	}

	إذا (mkdir(مسیر, 0777))
		چاپ_کن(_("could not create empty submodule directory %s"),
		      displaypath);

	cp_config.git_cmd = 1;
	strvec_pushl(&cp_config.آرگومانها, "config", "--get-regexp", عدم);
	strvec_pushf(&cp_config.آرگومانها, "submodule.%s\\.", sub->name);

	/* remove the .git/config entries (unless the user already did it) */
	إذا (!capture_command(&cp_config, &sb_config, 0) && sb_config.len) {
		حرف *sub_کلید = xstrfmt("submodule.%s", sub->name);

		/*
		 * remove the whole section so we have a clean state when
		 * the user later decides to init this submodule again
		 */
		repo_config_rename_section_درون_file(the_repository, عدم, sub_کلید, عدم);
		إذا (!(flags & OPT_QUIET))
			چاپ_کن(_("Submodule '%s' (%s) unregistered for path '%s'\n"),
				 sub->name, sub->url, displaypath);
		حرر(sub_کلید);
	}

cleanup:
	حرر(displaypath);
	حرر(sub_git_dir);
	strbuf_release(&sb_config);
}

ساكن فراغ deinit_submodule_cb(ثابت بنية cache_مدخل *list_قلم,
				فراغ *cb_داده)
{
	بنية deinit_cb *info = cb_داده;
	deinit_submodule(list_قلم->name, info->پیشوند, info->flags);
}

ساكن صحيح module_deinit(صحيح argc, ثابت حرف **argv, ثابت حرف *پیشوند,
			 بنية repository *repo استفادهنشده)
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
		OPT_انتها()
	};
	ثابت حرف *ثابت git_submodule_helper_usage[] = {
		N_("git submodule deinit [--quiet] [-f | --force] [--all | [--] [<path>...]]"),
		عدم
	};
	صحيح ret = 1;

	argc = parse_options(argc, argv, پیشوند, module_deinit_options,
			     git_submodule_helper_usage, 0);

	إذا (all && argc) {
		error("pathspec and --all are incompatible");
		usage_with_options(git_submodule_helper_usage,
				   module_deinit_options);
	}

	إذا (!argc && !all)
		die(_("Use '--all' if you really want to deinitialize all submodules"));

	إذا (module_list_compute(argv, پیشوند, &pathspec, &list) < 0)
		اذهب cleanup;

	info.پیشوند = پیشوند;
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

بنية module_clone_داده {
	ثابت حرف *پیشوند;
	ثابت حرف *مسیر;
	ثابت حرف *name;
	ثابت حرف *url;
	صحيح عمق;
	بنية list_objects_filter_options *filter_options;
	تعداد ref_storage_format ref_storage_format;
	بیعلامت صحيح quiet: 1;
	بیعلامت صحيح پیشرفت: 1;
	بیعلامت صحيح dissociate: 1;
	بیعلامت صحيح require_init: 1;
	صحيح single_branch;
};
#تعریف_کن MODULE_CLONE_داده_INIT { \
	.single_branch = -1, \
	.ref_storage_format = REF_STORAGE_FORMAT_UNKNOWN, \
}

بنية submodule_alternate_راهاندازی {
	ثابت حرف *submodule_name;
	تعداد SUBMODULE_ALTERNATE_ERROR_نما {
		SUBMODULE_ALTERNATE_ERROR_DIE,
		SUBMODULE_ALTERNATE_ERROR_INFO,
		SUBMODULE_ALTERNATE_ERROR_IGNORE
	} error_نما;
	بنية string_list *reference;
};
#تعریف_کن SUBMODULE_ALTERNATE_راهاندازی_INIT { \
	.error_نما = SUBMODULE_ALTERNATE_ERROR_IGNORE, \
}

ساكن ثابت حرف alternate_error_advice[] = N_(
"An alternate computed from a superproject's alternate is invalid.\n"
"To allow Git to clone without an alternate in such a case, set\n"
"submodule.alternateErrorStrategy to 'info' or, equivalently, clone with\n"
"'--reference-if-able' instead of '--reference'."
);

ساكن صحيح جمع_کن_possible_reference_from_superproject(
		بنية odb_files_dir *alt_odb, فراغ *sas_cb)
{
	بنية submodule_alternate_راهاندازی *sas = sas_cb;
	اندازه_t len;

	/*
	 * If the alternate object store is another repository, try the
	 * standard layout with .git/(modules/<name>)+/objects
	 */
	إذا (strip_suffix(alt_odb->abspath, "/objects", &len)) {
		بنية repository alternate;
		حرف *sm_alternate;
		بنية strbuf sb = STRBUF_INIT;
		بنية strbuf err = STRBUF_INIT;
		strbuf_جمع_کن(&sb, alt_odb->abspath, len);

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
		submodule_name_مقصد_gitdir(&sb, &alternate, sas->submodule_name);
		strbuf_addch(&sb, '/');
		repo_clear(&alternate);

		sm_alternate = compute_alternate_مسیر(sb.buf, &err);
		إذا (sm_alternate) {
			حرف *p = strbuf_detach(&sb, عدم);

			string_list_append(sas->reference, p)->util = p;
			حرر(sm_alternate);
		} وإلا {
			بدل (sas->error_نما) {
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
	بنية submodule_alternate_راهاندازی sas = SUBMODULE_ALTERNATE_راهاندازی_INIT;

	repo_config_get_string(the_repository, "submodule.alternateLocation", &sm_alternate);
	إذا (!sm_alternate)
		أرجع;

	repo_config_get_string(the_repository, "submodule.alternateErrorStrategy", &error_strategy);

	إذا (!error_strategy)
		error_strategy = xstrdup("die");

	sas.submodule_name = sm_name;
	sas.reference = reference;
	إذا (!strcmp(error_strategy, "die"))
		sas.error_نما = SUBMODULE_ALTERNATE_ERROR_DIE;
	وإلا إذا (!strcmp(error_strategy, "info"))
		sas.error_نما = SUBMODULE_ALTERNATE_ERROR_INFO;
	وإلا إذا (!strcmp(error_strategy, "ignore"))
		sas.error_نما = SUBMODULE_ALTERNATE_ERROR_IGNORE;
	وإلا
		die(_("Value '%s' for submodule.alternateErrorStrategy is not recognized"), error_strategy);

	إذا (!strcmp(sm_alternate, "superproject"))
		odb_for_each_alternate(the_repository->objects,
				       جمع_کن_possible_reference_from_superproject, &sas);
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

	submodule_name_مقصد_gitdir(&sb, the_repository, name);
	sm_gitdir = absolute_pathdup(sb.buf);
	strbuf_release(&sb);

	أرجع sm_gitdir;
}

ساكن صحيح dir_شامل_است_only_dotgit(ثابت حرف *مسیر)
{
	DIR *dir = opendir(مسیر);
	بنية dirent *e;
	صحيح ret = 1;

	إذا (!dir)
		أرجع 0;

	e = readdir_skip_dot_and_dotdot(dir);
	إذا (!e)
		ret = 0;
	وإلا إذا (strcmp(DEFAULT_GIT_DIR_ENVIRONMENT, e->d_name) ||
		 (e = readdir_skip_dot_and_dotdot(dir))) {
		error("unexpected item '%s' in '%s'", e->d_name, مسیر);
		ret = 0;
	}

	closedir(dir);
	أرجع ret;
}

ساكن صحيح clone_submodule(ثابت بنية module_clone_داده *clone_داده,
			   بنية string_list *reference)
{
	حرف *p;
	حرف *sm_gitdir = clone_submodule_sm_gitdir(clone_داده->name);
	حرف *sm_alternate = عدم, *error_strategy = عدم;
	بنية stat st;
	بنية فرزند_process cp = فرزند_PROCESS_INIT;
	ثابت حرف *clone_داده_مسیر = clone_داده->مسیر;
	حرف *مقصد_free = عدم;

	إذا (validate_submodule_مسیر(clone_داده_مسیر) < 0)
		die(عدم);

	إذا (!is_absolute_مسیر(clone_داده->مسیر))
		clone_داده_مسیر = مقصد_free = xstrfmt("%s/%s", repo_get_work_درخت(the_repository),
						    clone_داده->مسیر);

	إذا (!file_exists(sm_gitdir)) {
		إذا (clone_داده->require_init && !stat(clone_داده_مسیر, &st) &&
		    !is_خالی_dir(clone_داده_مسیر))
			die(_("directory not empty: '%s'"), clone_داده_مسیر);

		إذا (امن_create_leading_directories_const(the_repository, sm_gitdir) < 0)
			die(_("could not create directory '%s'"), sm_gitdir);

		prepare_possible_alternates(clone_داده->name, reference);

		strvec_افزودن(&cp.آرگومانها, "clone");
		strvec_افزودن(&cp.آرگومانها, "--no-checkout");
		إذا (clone_داده->quiet)
			strvec_افزودن(&cp.آرگومانها, "--quiet");
		إذا (clone_داده->پیشرفت)
			strvec_افزودن(&cp.آرگومانها, "--progress");
		إذا (clone_داده->عمق > 0)
			strvec_pushf(&cp.آرگومانها, "--depth=%d", clone_داده->عمق);
		إذا (reference->nr) {
			بنية string_list_قلم *قلم;

			for_each_string_list_قلم(قلم, reference)
				strvec_pushl(&cp.آرگومانها, "--reference",
					     قلم->string, عدم);
		}
		إذا (clone_داده->ref_storage_format != REF_STORAGE_FORMAT_UNKNOWN)
			strvec_pushf(&cp.آرگومانها, "--ref-storage-format=%s",
				     ref_storage_format_مقصد_name(clone_داده->ref_storage_format));
		إذا (clone_داده->dissociate)
			strvec_افزودن(&cp.آرگومانها, "--dissociate");
		إذا (sm_gitdir && *sm_gitdir)
			strvec_pushl(&cp.آرگومانها, "--separate-git-dir", sm_gitdir, عدم);
		إذا (clone_داده->filter_options && clone_داده->filter_options->choice)
			strvec_pushf(&cp.آرگومانها, "--filter=%s",
				     expand_list_objects_filter_spec(
					     clone_داده->filter_options));
		إذا (clone_داده->single_branch >= 0)
			strvec_افزودن(&cp.آرگومانها, clone_داده->single_branch ?
				    "--single-branch" :
				    "--no-single-branch");

		strvec_افزودن(&cp.آرگومانها, "--");
		strvec_افزودن(&cp.آرگومانها, clone_داده->url);
		strvec_افزودن(&cp.آرگومانها, clone_داده_مسیر);

		cp.git_cmd = 1;
		prepare_submodule_repo_env(&cp.env);
		cp.no_stdin = 1;

		إذا(پیاپی_command(&cp))
			die(_("clone of '%s' into submodule path '%s' failed"),
			    clone_داده->url, clone_داده_مسیر);

		إذا (clone_داده->require_init && !stat(clone_داده_مسیر, &st) &&
		    !dir_شامل_است_only_dotgit(clone_داده_مسیر)) {
			حرف *dot_git = xstrfmt("%s/.git", clone_داده_مسیر);
			unlink(dot_git);
			حرر(dot_git);
			die(_("directory not empty: '%s'"), clone_داده_مسیر);
		}
	} وإلا {
		حرف *مسیر;

		إذا (clone_داده->require_init && !stat(clone_داده_مسیر, &st) &&
		    !is_خالی_dir(clone_داده_مسیر))
			die(_("directory not empty: '%s'"), clone_داده_مسیر);
		إذا (امن_create_leading_directories_const(the_repository, clone_داده_مسیر) < 0)
			die(_("could not create directory '%s'"), clone_داده_مسیر);
		مسیر = xstrfmt("%s/index", sm_gitdir);
		unlink_or_warn(مسیر);
		حرر(مسیر);
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
	إذا (validate_submodule_git_dir(sm_gitdir, clone_داده->name) < 0) {
		حرف *سر = xstrfmt("%s/HEAD", sm_gitdir);
		unlink(سر);
		حرر(سر);
		die(_("refusing to create/use '%s' in another submodule's git dir. "
		      "Enabling extensions.submodulePathConfig should fix this."),
		    sm_gitdir);
	}

	connect_work_درخت_and_git_dir(clone_داده_مسیر, sm_gitdir, 0);

	p = repo_submodule_مسیر(the_repository, clone_داده_مسیر, "config");
	إذا (!p)
		die(_("could not get submodule directory for '%s'"), clone_داده_مسیر);

	/* setup alternateLocation and alternateErrorStrategy in the cloned submodule if needed */
	repo_config_get_string(the_repository, "submodule.alternateLocation", &sm_alternate);
	إذا (sm_alternate)
		repo_config_مجموعه_درون_file(the_repository, p, "submodule.alternateLocation",
					sm_alternate);
	repo_config_get_string(the_repository, "submodule.alternateErrorStrategy", &error_strategy);
	إذا (error_strategy)
		repo_config_مجموعه_درون_file(the_repository, p, "submodule.alternateErrorStrategy",
					error_strategy);

	حرر(sm_alternate);
	حرر(error_strategy);

	حرر(sm_gitdir);
	حرر(p);
	حرر(مقصد_free);
	أرجع 0;
}

ساكن صحيح module_clone(صحيح argc, ثابت حرف **argv, ثابت حرف *پیشوند,
			بنية repository *repo استفادهنشده)
{
	صحيح dissociate = 0, quiet = 0, پیشرفت = 0, require_init = 0;
	بنية module_clone_داده clone_داده = MODULE_CLONE_داده_INIT;
	بنية string_list reference = STRING_LIST_INIT_NODUP;
	بنية list_objects_filter_options filter_options =
		LIST_OBJECTS_FILTER_INIT;
	ثابت حرف *ref_storage_format = عدم;

	بنية option module_clone_options[] = {
		OPT_STRING(0, "prefix", &clone_داده.پیشوند,
			   N_("path"),
			   N_("alternative anchor for relative paths")),
		OPT_STRING(0, "path", &clone_داده.مسیر,
			   N_("path"),
			   N_("where the new submodule will be cloned to")),
		OPT_STRING(0, "name", &clone_داده.name,
			   N_("string"),
			   N_("name of the new submodule")),
		OPT_STRING(0, "url", &clone_داده.url,
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
		OPT_INTEGER(0, "depth", &clone_داده.عمق,
			   N_("depth for shallow clones")),
		OPT__QUIET(&quiet, "suppress output for cloning a submodule"),
		OPT_BOOL(0, "progress", &پیشرفت,
			   N_("force cloning progress")),
		OPT_BOOL(0, "require-init", &require_init,
			   N_("disallow cloning into non-empty directory")),
		OPT_BOOL(0, "single-branch", &clone_داده.single_branch,
			 N_("clone only one branch, HEAD or --branch")),
		OPT_PARSE_LIST_OBJECTS_FILTER(&filter_options),
		OPT_انتها()
	};
	ثابت حرف *ثابت git_submodule_helper_usage[] = {
		N_("git submodule--helper clone [--prefix=<path>] [--quiet] "
		   "[--reference <repository>] [--name <name>] [--depth <depth>] "
		   "[--single-branch] [--filter <filter-spec>] "
		   "--url <url> --path <path>"),
		عدم
	};

	argc = parse_options(argc, argv, پیشوند, module_clone_options,
			     git_submodule_helper_usage, 0);

	إذا (ref_storage_format) {
		clone_داده.ref_storage_format = ref_storage_format_by_name(ref_storage_format);
		إذا (clone_داده.ref_storage_format == REF_STORAGE_FORMAT_UNKNOWN)
			die(_("unknown ref storage format '%s'"), ref_storage_format);
	}
	clone_داده.dissociate = !!dissociate;
	clone_داده.quiet = !!quiet;
	clone_داده.پیشرفت = !!پیشرفت;
	clone_داده.require_init = !!require_init;
	clone_داده.filter_options = &filter_options;

	إذا (argc || !clone_داده.url || !clone_داده.مسیر || !*(clone_داده.مسیر))
		usage_with_options(git_submodule_helper_usage,
				   module_clone_options);

	clone_submodule(&clone_داده, &reference);
	list_objects_filter_release(&filter_options);
	string_list_clear(&reference, 1);
	أرجع 0;
}

ساكن صحيح determine_submodule_update_strategy(بنية repository *r,
					       صحيح just_cloned,
					       ثابت حرف *مسیر,
					       تعداد submodule_update_type update,
					       بنية submodule_update_strategy *بیرون)
{
	ثابت بنية submodule *sub = submodule_from_مسیر(r, null_oid(the_hash_algo), مسیر);
	حرف *کلید;
	ثابت حرف *val;
	صحيح ret;

	/*
	 * NEEDSWORK: audit and ensure that update_submodule() has right
	 * to assume that submodule_from_path() above will always succeed.
	 */
	إذا (!sub)
		BUG("update_submodule assumes a submodule exists at path (%s)",
		    مسیر);
	کلید = xstrfmt("submodule.%s.update", sub->name);

	إذا (update) {
		بیرون->type = update;
	} وإلا إذا (!repo_config_get_string_tmp(r, کلید, &val)) {
		إذا (parse_submodule_update_strategy(val, بیرون) < 0) {
			ret = die_پیام(_("Invalid update mode '%s' configured for submodule path '%s'"),
					  val, مسیر);
			اذهب cleanup;
		}
	} وإلا إذا (sub->update_strategy.type != SM_UPDATE_UNSPECIFIED) {
		إذا (sub->update_strategy.type == SM_UPDATE_COMMAND)
			BUG("how did we read update = !command from .gitmodules?");
		بیرون->type = sub->update_strategy.type;
		بیرون->command = sub->update_strategy.command;
	} وإلا
		بیرون->type = SM_UPDATE_CHECKOUT;

	إذا (just_cloned &&
	    (بیرون->type == SM_UPDATE_ادغام ||
	     بیرون->type == SM_UPDATE_REBASE ||
	     بیرون->type == SM_UPDATE_NONE))
		بیرون->type = SM_UPDATE_CHECKOUT;

	ret = 0;
cleanup:
	حرر(کلید);
	أرجع ret;
}

بنية update_clone_داده {
	ثابت بنية submodule *sub;
	بنية object_id oid;
	بیعلامت just_cloned;
};

بنية submodule_update_clone {
	/* index into 'update_data.list', the list of submodules to look into for cloning */
	صحيح current;

	/* configuration parameters which are passed on to the children */
	ثابت بنية update_داده *update_داده;

	/* to be consumed by update_submodule() */
	بنية update_clone_داده *update_clone;
	صحيح update_clone_nr; صحيح update_clone_alloc;

	/* If we want to stop as fast as possible and return an error */
	بیعلامت quickstop : 1;

	/* failed clones to be retried again */
	ثابت بنية cache_مدخل **failed_clones;
	صحيح failed_clones_nr, failed_clones_alloc;
};
#تعریف_کن SUBMODULE_UPDATE_CLONE_INIT { 0 }

ساكن فراغ submodule_update_clone_release(بنية submodule_update_clone *suc)
{
	حرر(suc->update_clone);
	حرر(suc->failed_clones);
}

بنية update_داده {
	ثابت حرف *پیشوند;
	ثابت حرف *super_پیشوند;
	حرف *displaypath;
	تعداد submodule_update_type update_default;
	بنية object_id suboid;
	بنية string_list references;
	بنية submodule_update_strategy update_strategy;
	بنية list_objects_filter_options *filter_options;
	بنية module_list list;
	تعداد ref_storage_format ref_storage_format;
	صحيح عمق;
	صحيح max_jobs;
	صحيح single_branch;
	صحيح recommend_shallow;
	بیعلامت صحيح require_init;
	بیعلامت صحيح force;
	بیعلامت صحيح quiet;
	بیعلامت صحيح nofetch;
	بیعلامت صحيح remote;
	بیعلامت صحيح پیشرفت;
	بیعلامت صحيح dissociate;
	بیعلامت صحيح init;
	بیعلامت صحيح warn_if_uninitialized;
	بیعلامت صحيح recursive;

	/* copied over from update_clone_data */
	بنية object_id oid;
	بیعلامت صحيح just_cloned;
	ثابت حرف *sm_مسیر;
};
#تعریف_کن UPDATE_داده_INIT { \
	.update_strategy = SUBMODULE_UPDATE_STRATEGY_INIT, \
	.list = MODULE_LIST_INIT, \
	.ref_storage_format = REF_STORAGE_FORMAT_UNKNOWN, \
	.recommend_shallow = -1, \
	.references = STRING_LIST_INIT_DUP, \
	.single_branch = -1, \
	.max_jobs = 1, \
}

ساكن فراغ update_داده_release(بنية update_داده *ud)
{
	حرر(ud->displaypath);
	submodule_update_strategy_release(&ud->update_strategy);
	module_list_release(&ud->list);
}

ساكن فراغ بعدی_submodule_warn_missing(بنية submodule_update_clone *suc,
		بنية strbuf *بیرون, ثابت حرف *displaypath)
{
	/*
	 * Only mention uninitialized submodules when their
	 * paths have been specified.
	 */
	إذا (suc->update_داده->warn_if_uninitialized) {
		strbuf_addf(بیرون,
			_("Submodule path '%s' not initialized"),
			displaypath);
		strbuf_addch(بیرون, '\n');
		strbuf_addstr(بیرون,
			_("Maybe you want to use 'update --init'?"));
		strbuf_addch(بیرون, '\n');
	}
}

/**
 * Determine whether 'ce' needs to be cloned. If so, prepare the 'child' to
 * run the clone. Returns 1 if 'ce' needs to be cloned, 0 otherwise.
 */
ساكن صحيح prepare_مقصد_clone_بعدی_submodule(ثابت بنية cache_مدخل *ce,
					   بنية فرزند_process *فرزند,
					   بنية submodule_update_clone *suc,
					   بنية strbuf *بیرون)
{
	ثابت بنية submodule *sub = عدم;
	ثابت حرف *url = عدم;
	ثابت حرف *update_string;
	تعداد submodule_update_type update_type;
	حرف *کلید;
	ثابت بنية update_داده *ud = suc->update_داده;
	حرف *displaypath = get_submodule_displaypath(ce->name, ud->پیشوند,
						      ud->super_پیشوند);
	بنية strbuf sb = STRBUF_INIT;
	صحيح needs_cloning = 0;
	صحيح need_free_url = 0;

	إذا (ce_stage(ce)) {
		strbuf_addf(بیرون, _("Skipping unmerged submodule %s"), displaypath);
		strbuf_addch(بیرون, '\n');
		اذهب cleanup;
	}

	sub = submodule_from_مسیر(the_repository, null_oid(the_hash_algo), ce->name);

	إذا (!sub) {
		بعدی_submodule_warn_missing(suc, بیرون, displaypath);
		اذهب cleanup;
	}

	کلید = xstrfmt("submodule.%s.update", sub->name);
	إذا (!repo_config_get_string_tmp(the_repository, کلید, &update_string)) {
		update_type = parse_submodule_update_type(update_string);
	} وإلا {
		update_type = sub->update_strategy.type;
	}
	حرر(کلید);

	إذا (suc->update_داده->update_strategy.type == SM_UPDATE_NONE
	    || (suc->update_داده->update_strategy.type == SM_UPDATE_UNSPECIFIED
		&& update_type == SM_UPDATE_NONE)) {
		strbuf_addf(بیرون, _("Skipping submodule '%s'"), displaypath);
		strbuf_addch(بیرون, '\n');
		اذهب cleanup;
	}

	/* Check if the submodule has been initialized. */
	إذا (!is_submodule_فعال(the_repository, ce->name)) {
		بعدی_submodule_warn_missing(suc, بیرون, displaypath);
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

	فرزند->git_cmd = 1;
	فرزند->no_stdin = 1;
	فرزند->stdout_مقصد_stderr = 1;
	فرزند->err = -1;
	strvec_افزودن(&فرزند->آرگومانها, "submodule--helper");
	strvec_افزودن(&فرزند->آرگومانها, "clone");
	إذا (suc->update_داده->پیشرفت)
		strvec_افزودن(&فرزند->آرگومانها, "--progress");
	إذا (suc->update_داده->quiet)
		strvec_افزودن(&فرزند->آرگومانها, "--quiet");
	إذا (suc->update_داده->پیشوند)
		strvec_pushl(&فرزند->آرگومانها, "--prefix", suc->update_داده->پیشوند, عدم);
	إذا (suc->update_داده->recommend_shallow && sub->recommend_shallow == 1)
		strvec_افزودن(&فرزند->آرگومانها, "--depth=1");
	وإلا إذا (suc->update_داده->عمق)
		strvec_pushf(&فرزند->آرگومانها, "--depth=%d", suc->update_داده->عمق);
	إذا (suc->update_داده->filter_options && suc->update_داده->filter_options->choice)
		strvec_pushf(&فرزند->آرگومانها, "--filter=%s",
			     expand_list_objects_filter_spec(suc->update_داده->filter_options));
	إذا (suc->update_داده->require_init)
		strvec_افزودن(&فرزند->آرگومانها, "--require-init");
	إذا (suc->update_داده->ref_storage_format != REF_STORAGE_FORMAT_UNKNOWN)
		strvec_pushf(&فرزند->آرگومانها, "--ref-storage-format=%s",
			     ref_storage_format_مقصد_name(suc->update_داده->ref_storage_format));
	strvec_pushl(&فرزند->آرگومانها, "--path", sub->مسیر, عدم);
	strvec_pushl(&فرزند->آرگومانها, "--name", sub->name, عدم);
	strvec_pushl(&فرزند->آرگومانها, "--url", url, عدم);
	إذا (suc->update_داده->references.nr) {
		بنية string_list_قلم *قلم;

		for_each_string_list_قلم(قلم, &suc->update_داده->references)
			strvec_pushl(&فرزند->آرگومانها, "--reference", قلم->string, عدم);
	}
	إذا (suc->update_داده->dissociate)
		strvec_افزودن(&فرزند->آرگومانها, "--dissociate");
	إذا (suc->update_داده->single_branch >= 0)
		strvec_افزودن(&فرزند->آرگومانها, suc->update_داده->single_branch ?
					      "--single-branch" :
					      "--no-single-branch");

cleanup:
	حرر(displaypath);
	strbuf_release(&sb);
	إذا (need_free_url)
		حرر((فراغ*)url);

	أرجع needs_cloning;
}

ساكن صحيح update_clone_get_بعدی_task(بنية فرزند_process *فرزند,
				      بنية strbuf *err,
				      فراغ *suc_cb,
				      فراغ **idx_task_cb)
{
	بنية submodule_update_clone *suc = suc_cb;
	ثابت بنية cache_مدخل *ce;
	صحيح شاخص;

	لكل (; suc->current < suc->update_داده->list.nr; suc->current++) {
		ce = suc->update_داده->list.entries[suc->current];
		إذا (prepare_مقصد_clone_بعدی_submodule(ce, فرزند, suc, err)) {
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
	شاخص = suc->current - suc->update_داده->list.nr;
	إذا (شاخص < suc->failed_clones_nr) {
		صحيح *p;

		ce = suc->failed_clones[شاخص];
		إذا (!prepare_مقصد_clone_بعدی_submodule(ce, فرزند, suc, err)) {
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

ساكن صحيح update_clone_شروع_failure(بنية strbuf *err استفادهنشده,
				      فراغ *suc_cb,
				      فراغ *idx_task_cb استفادهنشده)
{
	بنية submodule_update_clone *suc = suc_cb;

	suc->quickstop = 1;
	أرجع 1;
}

ساكن صحيح update_clone_task_پایانیافته(صحيح نتیجه,
				      بنية strbuf *err,
				      فراغ *suc_cb,
				      فراغ *idx_task_cb)
{
	ثابت بنية cache_مدخل *ce;
	بنية submodule_update_clone *suc = suc_cb;
	صحيح *idxP = idx_task_cb;
	صحيح idx = *idxP;

	حرر(idxP);

	إذا (!نتیجه)
		أرجع 0;

	إذا (idx < suc->update_داده->list.nr) {
		ce  = suc->update_داده->list.entries[idx];
		strbuf_addf(err, _("Failed to clone '%s'. Retry scheduled"),
			    ce->name);
		strbuf_addch(err, '\n');
		ALLOC_GROW(suc->failed_clones,
			   suc->failed_clones_nr + 1,
			   suc->failed_clones_alloc);
		suc->failed_clones[suc->failed_clones_nr++] = ce;
		أرجع 0;
	} وإلا {
		idx -= suc->update_داده->list.nr;
		ce  = suc->failed_clones[idx];
		strbuf_addf(err, _("Failed to clone '%s' a second time, aborting"),
			    ce->name);
		strbuf_addch(err, '\n');
		suc->quickstop = 1;
		أرجع 1;
	}

	أرجع 0;
}

ساكن صحيح git_update_clone_config(ثابت حرف *var, ثابت حرف *مقدار,
				   ثابت بنية config_context *ctx,
				   فراغ *cb)
{
	صحيح *max_jobs = cb;

	إذا (!strcmp(var, "submodule.fetchjobs"))
		*max_jobs = parse_submodule_fetchjobs(var, مقدار, ctx->kvi);
	أرجع 0;
}

ساكن صحيح is_tip_reachable(ثابت حرف *مسیر, ثابت بنية object_id *oid)
{
	بنية فرزند_process cp = فرزند_PROCESS_INIT;
	بنية strbuf rev = STRBUF_INIT;
	حرف *hex = oid_مقصد_hex(oid);
	صحيح reachable;

	cp.git_cmd = 1;
	cp.dir = مسیر;
	cp.no_stderr = 1;
	strvec_pushl(&cp.آرگومانها, "rev-list", "-n", "1", hex, "--not", "--all", عدم);

	prepare_submodule_repo_env(&cp.env);

	إذا (capture_command(&cp, &rev, GIT_MAX_HEXSZ + 1) || rev.len)
		reachable = 0;
	وإلا
		reachable = 1;

	strbuf_release(&rev);
	أرجع reachable;
}

ساكن صحيح fetch_درون_submodule(ثابت حرف *module_مسیر, صحيح عمق, صحيح quiet,
			      ثابت بنية object_id *oid)
{
	بنية فرزند_process cp = فرزند_PROCESS_INIT;

	prepare_submodule_repo_env(&cp.env);
	cp.git_cmd = 1;
	cp.dir = module_مسیر;

	strvec_افزودن(&cp.آرگومانها, "fetch");
	إذا (quiet)
		strvec_افزودن(&cp.آرگومانها, "--quiet");
	إذا (عمق)
		strvec_pushf(&cp.آرگومانها, "--depth=%d", عمق);
	إذا (oid) {
		حرف *hex = oid_مقصد_hex(oid);
		حرف *remote;
		صحيح کد;

		کد = get_default_remote_submodule(module_مسیر, &remote);
		إذا (کد) {
			فرزند_process_clear(&cp);
			أرجع کد;
		}

		strvec_pushl(&cp.آرگومانها, remote, hex, عدم);
		حرر(remote);
	}

	أرجع پیاپی_command(&cp);
}

ساكن صحيح پیاپی_update_command(ثابت بنية update_داده *ud, صحيح subforce)
{
	بنية فرزند_process cp = فرزند_PROCESS_INIT;
	حرف *oid = oid_مقصد_hex(&ud->oid);
	صحيح ret;

	بدل (ud->update_strategy.type) {
	حالة SM_UPDATE_CHECKOUT:
		cp.git_cmd = 1;
		strvec_pushl(&cp.آرگومانها, "checkout", "-q", عدم);
		إذا (subforce)
			strvec_افزودن(&cp.آرگومانها, "-f");
		اكسر;
	حالة SM_UPDATE_REBASE:
		cp.git_cmd = 1;
		strvec_افزودن(&cp.آرگومانها, "rebase");
		إذا (ud->quiet)
			strvec_افزودن(&cp.آرگومانها, "--quiet");
		اكسر;
	حالة SM_UPDATE_ادغام:
		cp.git_cmd = 1;
		strvec_افزودن(&cp.آرگومانها, "merge");
		إذا (ud->quiet)
			strvec_افزودن(&cp.آرگومانها, "--quiet");
		اكسر;
	حالة SM_UPDATE_COMMAND:
		cp.use_shell = 1;
		strvec_افزودن(&cp.آرگومانها, ud->update_strategy.command);
		اكسر;
	افتراضي:
		BUG("unexpected update strategy type: %d",
		    ud->update_strategy.type);
	}
	strvec_افزودن(&cp.آرگومانها, oid);

	cp.dir = ud->sm_مسیر;
	prepare_submodule_repo_env(&cp.env);
	إذا ((ret = پیاپی_command(&cp))) {
		بدل (ud->update_strategy.type) {
		حالة SM_UPDATE_CHECKOUT:
			die_پیام(_("Unable to checkout '%s' in submodule path '%s'"),
				    oid, ud->displaypath);
			/* No "ret" assignment, use "git checkout"'s */
			اكسر;
		حالة SM_UPDATE_REBASE:
			ret = die_پیام(_("Unable to rebase '%s' in submodule path '%s'"),
					  oid, ud->displaypath);
			اكسر;
		حالة SM_UPDATE_ادغام:
			ret = die_پیام(_("Unable to merge '%s' in submodule path '%s'"),
					  oid, ud->displaypath);
			اكسر;
		حالة SM_UPDATE_COMMAND:
			ret = die_پیام(_("Execution of '%s %s' failed in submodule path '%s'"),
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
		چاپ_کن(_("Submodule path '%s': checked out '%s'\n"),
		       ud->displaypath, oid);
		اكسر;
	حالة SM_UPDATE_REBASE:
		چاپ_کن(_("Submodule path '%s': rebased into '%s'\n"),
		       ud->displaypath, oid);
		اكسر;
	حالة SM_UPDATE_ادغام:
		چاپ_کن(_("Submodule path '%s': merged in '%s'\n"),
		       ud->displaypath, oid);
		اكسر;
	حالة SM_UPDATE_COMMAND:
		چاپ_کن(_("Submodule path '%s': '%s %s'\n"),
		       ud->displaypath, ud->update_strategy.command, oid);
		اكسر;
	افتراضي:
		BUG("unexpected update strategy type: %d",
		    ud->update_strategy.type);
	}

	أرجع 0;
}

ساكن صحيح پیاپی_update_procedure(ثابت بنية update_داده *ud)
{
	صحيح subforce = is_null_oid(&ud->suboid) || ud->force;

	إذا (!ud->nofetch) {
		/*
		 * Run fetch only if `oid` isn't present or it
		 * is not reachable from a ref.
		 */
		إذا (!is_tip_reachable(ud->sm_مسیر, &ud->oid) &&
		    fetch_درون_submodule(ud->sm_مسیر, ud->عمق, ud->quiet, عدم) &&
		    !ud->quiet)
			fprintf_ln(stderr,
				   _("Unable to fetch in submodule path '%s'; "
				     "trying to directly fetch %s:"),
				   ud->displaypath, oid_مقصد_hex(&ud->oid));
		/*
		 * Now we tried the usual fetch, but `oid` may
		 * not be reachable from any of the refs.
		 */
		إذا (!is_tip_reachable(ud->sm_مسیر, &ud->oid) &&
		    fetch_درون_submodule(ud->sm_مسیر, ud->عمق, ud->quiet, &ud->oid))
			أرجع die_پیام(_("Fetched in submodule path '%s', but it did not "
					     "contain %s. Direct fetching of that commit failed."),
					   ud->displaypath, oid_مقصد_hex(&ud->oid));
	}

	أرجع پیاپی_update_command(ud, subforce);
}

ساكن صحيح remote_submodule_branch(ثابت حرف *مسیر, ثابت حرف **branch)
{
	ثابت بنية submodule *sub;
	حرف *کلید;
	*branch = عدم;

	sub = submodule_from_مسیر(the_repository, null_oid(the_hash_algo), مسیر);
	إذا (!sub)
		أرجع die_پیام(_("could not initialize submodule at path '%s'"),
				   مسیر);

	کلید = xstrfmt("submodule.%s.branch", sub->name);
	إذا (repo_config_get_string_tmp(the_repository, کلید, branch))
		*branch = sub->branch;
	حرر(کلید);

	إذا (!*branch) {
		*branch = "HEAD";
		أرجع 0;
	}

	إذا (!strcmp(*branch, ".")) {
		ثابت حرف *refname = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
							      "HEAD", 0, عدم,
							      عدم);

		إذا (!refname)
			أرجع die_پیام(_("No such ref: %s"), "HEAD");

		/* detached HEAD */
		إذا (!strcmp(refname, "HEAD"))
			أرجع die_پیام(_("Submodule (%s) branch configured to inherit "
					     "branch from superproject, but the superproject "
					     "is not on any branch"), sub->name);

		إذا (!skip_پیشوند(refname, "refs/heads/", &refname))
			أرجع die_پیام(_("Expecting a full ref name, got %s"),
					   refname);

		*branch = refname;
		أرجع 0;
	}

	/* Our "branch" is coming from repo_config_get_string_tmp() */
	أرجع 0;
}

ساكن صحيح ensure_core_worktree(ثابت حرف *مسیر)
{
	ثابت حرف *cw;
	بنية repository subrepo;

	إذا (repo_submodule_init(&subrepo, the_repository, مسیر, null_oid(the_hash_algo)))
		أرجع die_پیام(_("could not get a repository handle for submodule '%s'"),
				   مسیر);

	إذا (!repo_config_get_string_tmp(&subrepo, "core.worktree", &cw)) {
		حرف *cfg_file, *abs_مسیر;
		ثابت حرف *rel_مسیر;
		بنية strbuf sb = STRBUF_INIT;

		cfg_file = repo_git_مسیر(&subrepo, "config");

		abs_مسیر = absolute_pathdup(مسیر);
		rel_مسیر = relative_مسیر(abs_مسیر, subrepo.gitdir, &sb);

		repo_config_مجموعه_درون_file(the_repository, cfg_file, "core.worktree", rel_مسیر);

		حرر(cfg_file);
		حرر(abs_مسیر);
		strbuf_release(&sb);
	}

	repo_clear(&subrepo);
	أرجع 0;
}

ساكن ثابت حرف *submodule_update_type_مقصد_label(تعداد submodule_update_type type)
{
	بدل (type) {
	حالة SM_UPDATE_CHECKOUT:
		أرجع "checkout";
	حالة SM_UPDATE_ادغام:
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

ساكن فراغ update_داده_مقصد_آرگومانها(ثابت بنية update_داده *update_داده,
				بنية strvec *آرگومانها)
{
	تعداد submodule_update_type update_type = update_داده->update_default;

	strvec_pushl(آرگومانها, "submodule--helper", "update", "--recursive", عدم);
	إذا (update_داده->displaypath)
		strvec_pushf(آرگومانها, "--super-prefix=%s/",
			     update_داده->displaypath);
	strvec_pushf(آرگومانها, "--jobs=%d", update_داده->max_jobs);
	إذا (update_داده->quiet)
		strvec_افزودن(آرگومانها, "--quiet");
	إذا (update_داده->force)
		strvec_افزودن(آرگومانها, "--force");
	إذا (update_داده->init)
		strvec_افزودن(آرگومانها, "--init");
	إذا (update_داده->remote)
		strvec_افزودن(آرگومانها, "--remote");
	إذا (update_داده->nofetch)
		strvec_افزودن(آرگومانها, "--no-fetch");
	إذا (update_داده->dissociate)
		strvec_افزودن(آرگومانها, "--dissociate");
	إذا (update_داده->پیشرفت)
		strvec_افزودن(آرگومانها, "--progress");
	إذا (update_داده->require_init)
		strvec_افزودن(آرگومانها, "--require-init");
	إذا (update_داده->عمق)
		strvec_pushf(آرگومانها, "--depth=%d", update_داده->عمق);
	إذا (update_type != SM_UPDATE_UNSPECIFIED)
		strvec_pushf(آرگومانها, "--%s",
			     submodule_update_type_مقصد_label(update_type));

	إذا (update_داده->references.nr) {
		بنية string_list_قلم *قلم;

		for_each_string_list_قلم(قلم, &update_داده->references)
			strvec_pushl(آرگومانها, "--reference", قلم->string, عدم);
	}
	إذا (update_داده->ref_storage_format != REF_STORAGE_FORMAT_UNKNOWN)
		strvec_pushf(آرگومانها, "--ref-storage-format=%s",
			     ref_storage_format_مقصد_name(update_داده->ref_storage_format));
	إذا (update_داده->filter_options && update_داده->filter_options->choice)
		strvec_pushf(آرگومانها, "--filter=%s",
				expand_list_objects_filter_spec(
					update_داده->filter_options));
	إذا (update_داده->recommend_shallow == 0)
		strvec_افزودن(آرگومانها, "--no-recommend-shallow");
	وإلا إذا (update_داده->recommend_shallow == 1)
		strvec_افزودن(آرگومانها, "--recommend-shallow");
	إذا (update_داده->single_branch >= 0)
		strvec_افزودن(آرگومانها, update_داده->single_branch ?
				    "--single-branch" :
				    "--no-single-branch");
}

ساكن صحيح update_submodule(بنية update_داده *update_داده)
{
	صحيح ret;

	إذا (validate_submodule_مسیر(update_داده->sm_مسیر) < 0)
		أرجع -1;

	ret = determine_submodule_update_strategy(the_repository,
						  update_داده->just_cloned,
						  update_داده->sm_مسیر,
						  update_داده->update_default,
						  &update_داده->update_strategy);
	إذا (ret)
		أرجع ret;

	إذا (update_داده->just_cloned)
		oidcpy(&update_داده->suboid, null_oid(the_hash_algo));
	وإلا إذا (repo_resolve_gitlink_ref(the_repository, update_داده->sm_مسیر,
					  "HEAD", &update_داده->suboid))
		أرجع die_پیام(_("Unable to find current revision in submodule path '%s'"),
				   update_داده->displaypath);

	إذا (update_داده->remote) {
		حرف *remote_name;
		ثابت حرف *branch;
		حرف *remote_ref;
		صحيح کد;

		کد = get_default_remote_submodule(update_داده->sm_مسیر, &remote_name);
		إذا (کد)
			أرجع کد;
		کد = remote_submodule_branch(update_داده->sm_مسیر, &branch);
		إذا (کد) {
			حرر(remote_name);
			أرجع کد;
		}
		remote_ref = xstrfmt("refs/remotes/%s/%s", remote_name, branch);

		حرر(remote_name);

		إذا (!update_داده->nofetch) {
			إذا (fetch_درون_submodule(update_داده->sm_مسیر, update_داده->عمق,
					      0, عدم)) {
				حرر(remote_ref);
				أرجع die_پیام(_("Unable to fetch in submodule path '%s'"),
						   update_داده->sm_مسیر);
			}
		}

		إذا (repo_resolve_gitlink_ref(the_repository, update_داده->sm_مسیر,
					     remote_ref, &update_داده->oid)) {
			ret = die_پیام(_("Unable to find %s revision in submodule path '%s'"),
					  remote_ref, update_داده->sm_مسیر);
			حرر(remote_ref);
			أرجع ret;
		}

		حرر(remote_ref);
	}

	إذا (!oideq(&update_داده->oid, &update_داده->suboid) || update_داده->force) {
		ret = پیاپی_update_procedure(update_داده);
		إذا (ret)
			أرجع ret;
	}

	إذا (update_داده->recursive) {
		بنية فرزند_process cp = فرزند_PROCESS_INIT;
		بنية update_داده بعدی = *update_داده;

		بعدی.پیشوند = عدم;
		oidcpy(&بعدی.oid, null_oid(the_hash_algo));
		oidcpy(&بعدی.suboid, null_oid(the_hash_algo));

		cp.dir = update_داده->sm_مسیر;
		cp.git_cmd = 1;
		prepare_submodule_repo_env(&cp.env);
		update_داده_مقصد_آرگومانها(&بعدی, &cp.آرگومانها);

		ret = پیاپی_command(&cp);
		إذا (ret)
			die_پیام(_("Failed to recurse into submodule path '%s'"),
				    update_داده->displaypath);
		أرجع ret;
	}

	أرجع 0;
}

ساكن صحيح update_submodules(بنية update_داده *update_داده)
{
	صحيح i, ret = 0;
	بنية submodule_update_clone suc = SUBMODULE_UPDATE_CLONE_INIT;
	ثابت بنية پیاپی_process_parallel_opts opts = {
		.tr2_category = "submodule",
		.tr2_label = "parallel/update",

		.processes = update_داده->max_jobs,

		.get_بعدی_task = update_clone_get_بعدی_task,
		.شروع_failure = update_clone_شروع_failure,
		.task_پایانیافته = update_clone_task_پایانیافته,
		.داده = &suc,
	};

	suc.update_داده = update_داده;
	پیاپی_processes_parallel(&opts);

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
		بنية update_clone_داده ucd = suc.update_clone[i];
		صحيح کد = 128;

		oidcpy(&update_داده->oid, &ucd.oid);
		update_داده->just_cloned = ucd.just_cloned;
		update_داده->sm_مسیر = ucd.sub->مسیر;

		/*
		 * Verify that the submodule path does not contain any
		 * symlinks; if it does, it might have been tampered with.
		 * TODO: allow exempting it via
		 * `safe.submodule.path` or something
		 */
		إذا (validate_submodule_مسیر(update_داده->sm_مسیر) < 0)
			اذهب fail;

		کد = ensure_core_worktree(update_داده->sm_مسیر);
		إذا (کد)
			اذهب fail;

		update_داده->displaypath = get_submodule_displaypath(
			update_داده->sm_مسیر, update_داده->پیشوند,
			update_داده->super_پیشوند);
		کد = update_submodule(update_داده);
		FREE_AND_NULL(update_داده->displaypath);
fail:
		إذا (!کد)
			استمر;
		ret = کد;
		إذا (ret == 128)
			اذهب cleanup;
	}

cleanup:
	submodule_update_clone_release(&suc);
	string_list_clear(&update_داده->references, 0);
	أرجع ret;
}

ساكن صحيح module_update(صحيح argc, ثابت حرف **argv, ثابت حرف *پیشوند,
			 بنية repository *repo استفادهنشده)
{
	بنية pathspec pathspec = { 0 };
	بنية pathspec pathspec2 = { 0 };
	بنية update_داده opt = UPDATE_داده_INIT;
	بنية list_objects_filter_options filter_options =
		LIST_OBJECTS_FILTER_INIT;
	ثابت حرف *ref_storage_format = عدم;
	صحيح ret;
	بنية option module_update_options[] = {
		OPT_SUPER_پیشوند(&opt.super_پیشوند),
		OPT__FORCE(&opt.force, N_("force checkout updates"), 0),
		OPT_BOOL('i', "init", &opt.init,
			 N_("initialize uninitialized submodules before update")),
		OPT_BOOL(0, "remote", &opt.remote,
			 N_("use SHA-1 of submodule's remote tracking branch")),
		OPT_BOOL(0, "recursive", &opt.recursive,
			 N_("traverse submodules recursively")),
		OPT_BOOL('N', "no-fetch", &opt.nofetch,
			 N_("don't fetch new objects from the remote site")),
		OPT_مجموعه_INT(0, "checkout", &opt.update_default,
			N_("use the 'checkout' update strategy (default)"),
			SM_UPDATE_CHECKOUT),
		OPT_مجموعه_INT('m', "merge", &opt.update_default,
			N_("use the 'merge' update strategy"),
			SM_UPDATE_ادغام),
		OPT_مجموعه_INT('r', "rebase", &opt.update_default,
			N_("use the 'rebase' update strategy"),
			SM_UPDATE_REBASE),
		OPT_STRING_LIST(0, "reference", &opt.references, N_("repo"),
			   N_("reference repository")),
		OPT_STRING(0, "ref-storage-format", &ref_storage_format, N_("format"),
			   N_("specify the reference storage format to use")),
		OPT_ALIAS_F(0, "ref-format", "ref-storage-format", PARSE_OPT_HIDDEN),
		OPT_BOOL(0, "dissociate", &opt.dissociate,
			   N_("use --reference only while cloning")),
		OPT_INTEGER(0, "depth", &opt.عمق,
			   N_("create a shallow clone truncated to the "
			      "specified number of revisions")),
		OPT_INTEGER('j', "jobs", &opt.max_jobs,
			    N_("parallel jobs")),
		OPT_BOOL(0, "recommend-shallow", &opt.recommend_shallow,
			    N_("whether the initial clone should follow the shallow recommendation")),
		OPT__QUIET(&opt.quiet, N_("don't print cloning progress")),
		OPT_BOOL(0, "progress", &opt.پیشرفت,
			    N_("force cloning progress")),
		OPT_BOOL(0, "require-init", &opt.require_init,
			   N_("disallow cloning into non-empty directory, implies --init")),
		OPT_BOOL(0, "single-branch", &opt.single_branch,
			 N_("clone only one branch, HEAD or --branch")),
		OPT_PARSE_LIST_OBJECTS_FILTER(&filter_options),
		OPT_انتها()
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

	argc = parse_options(argc, argv, پیشوند, module_update_options,
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
	opt.پیشوند = پیشوند;

	إذا (opt.update_default)
		opt.update_strategy.type = opt.update_default;

	إذا (module_list_compute(argv, پیشوند, &pathspec, &opt.list) < 0) {
		ret = 1;
		اذهب cleanup;
	}

	إذا (pathspec.nr)
		opt.warn_if_uninitialized = 1;

	إذا (opt.init) {
		بنية module_list list = MODULE_LIST_INIT;
		بنية init_cb info = INIT_CB_INIT;

		إذا (module_list_compute(argv, opt.پیشوند,
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
			module_list_فعال(&list);

		info.پیشوند = opt.پیشوند;
		info.super_پیشوند = opt.super_پیشوند;
		إذا (opt.quiet)
			info.flags |= OPT_QUIET;

		for_each_listed_submodule(&list, init_submodule_cb, &info);
		module_list_release(&list);
	}

	ret = update_submodules(&opt);
cleanup:
	update_داده_release(&opt);
	list_objects_filter_release(&filter_options);
	clear_pathspec(&pathspec);
	clear_pathspec(&pathspec2);
	أرجع ret;
}

ساكن صحيح افزودن_check(صحيح argc, ثابت حرف **argv, ثابت حرف *پیشوند استفادهنشده,
		      بنية repository *repo استفادهنشده)
{
	بنية remote *remote;
	ثابت حرف *superproject_سر;
	حرف *سر;
	صحيح detached_سر = 0;
	بنية object_id سر_oid;

	إذا (argc < 3)
		die("submodule--helper push-check requires at least 2 arguments");

	/*
	 * superproject's resolved head ref.
	 * if HEAD then the superproject is in a detached head state, otherwise
	 * it will be the resolved head ref.
	 */
	superproject_سر = argv[1];
	argv++;
	argc--;
	/* Get the submodule's head ref and determine if it is detached */
	سر = refs_resolve_refdup(get_main_ref_store(the_repository), "HEAD",
				   0, &سر_oid, عدم);
	إذا (!سر)
		die(_("Failed to resolve HEAD as a valid ref."));
	إذا (!strcmp(سر, "HEAD"))
		detached_سر = 1;

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
		بنية refspec refspec = REFSPEC_INIT_افزودن(the_hash_algo);

		refspec_appendn(&refspec, argv + 2, argc - 2);

		لكل (i = 0; i < refspec.nr; i++) {
			ثابت بنية refspec_قلم *rs = &refspec.اقلام[i];

			إذا (rs->الگو || rs->matching)
				استمر;

			/* LHS must match a single ref */
			بدل (شمار_refspec_تطابق(rs->src, local_refs, عدم)) {
			حالة 1:
				اكسر;
			حالة 0:
				/*
				 * If LHS matches 'HEAD' then we need to ensure
				 * that it matches the same named branch
				 * checked out in the superproject.
				 */
				إذا (!strcmp(rs->src, "HEAD")) {
					إذا (!detached_سر &&
					    !strcmp(سر, superproject_سر))
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
	حرر(سر);

	أرجع 0;
}

ساكن صحيح absorb_git_dirs(صحيح argc, ثابت حرف **argv, ثابت حرف *پیشوند,
			   بنية repository *repo استفادهنشده)
{
	صحيح i;
	بنية pathspec pathspec = { 0 };
	بنية module_list list = MODULE_LIST_INIT;
	ثابت حرف *super_پیشوند = عدم;
	بنية option embed_gitdir_options[] = {
		OPT_SUPER_پیشوند(&super_پیشوند),
		OPT_انتها()
	};
	ثابت حرف *ثابت git_submodule_helper_usage[] = {
		N_("git submodule absorbgitdirs [<options>] [<path>...]"),
		عدم
	};
	صحيح ret = 1;

	argc = parse_options(argc, argv, پیشوند, embed_gitdir_options,
			     git_submodule_helper_usage, 0);

	إذا (module_list_compute(argv, پیشوند, &pathspec, &list) < 0)
		اذهب cleanup;

	لكل (i = 0; i < list.nr; i++)
		absorb_git_dir_into_superproject(list.entries[i]->name,
						 super_پیشوند);

	ret = 0;
cleanup:
	clear_pathspec(&pathspec);
	module_list_release(&list);
	أرجع ret;
}

ساكن صحيح module_مجموعه_url(صحيح argc, ثابت حرف **argv, ثابت حرف *پیشوند,
			  بنية repository *repo استفادهنشده)
{
	صحيح quiet = 0, ret;
	ثابت حرف *newurl;
	ثابت حرف *مسیر;
	حرف *config_name;
	بنية option options[] = {
		OPT__QUIET(&quiet, N_("suppress output for setting url of a submodule")),
		OPT_انتها()
	};
	ثابت حرف *ثابت usage[] = {
		N_("git submodule set-url [--quiet] <path> <newurl>"),
		عدم
	};
	ثابت بنية submodule *sub;

	argc = parse_options(argc, argv, پیشوند, options, usage, 0);

	إذا (argc != 2 || !(مسیر = argv[0]) || !(newurl = argv[1]))
		usage_with_options(usage, options);

	sub = submodule_from_مسیر(the_repository, null_oid(the_hash_algo), مسیر);

	إذا (!sub)
		die(_("no submodule mapping found in .gitmodules for path '%s'"),
		    مسیر);

	config_name = xstrfmt("submodule.%s.url", sub->name);
	ret = config_مجموعه_درون_gitmodules_file_gently(the_repository, config_name, newurl);

	إذا (!ret) {
		repo_read_gitmodules(the_repository, 0);
		sync_submodule(sub->مسیر, پیشوند, عدم, quiet ? OPT_QUIET : 0);
	}

	حرر(config_name);
	أرجع !!ret;
}

ساكن صحيح module_مجموعه_branch(صحيح argc, ثابت حرف **argv, ثابت حرف *پیشوند,
			     بنية repository *repo استفادهنشده)
{
	صحيح opt_default = 0, ret;
	ثابت حرف *opt_branch = عدم;
	ثابت حرف *مسیر;
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
		OPT_انتها()
	};
	ثابت حرف *ثابت usage[] = {
		N_("git submodule set-branch [-q|--quiet] (-d|--default) <path>"),
		N_("git submodule set-branch [-q|--quiet] (-b|--branch) <branch> <path>"),
		عدم
	};
	ثابت بنية submodule *sub;

	argc = parse_options(argc, argv, پیشوند, options, usage, 0);

	إذا (!opt_branch && !opt_default)
		die(_("--branch or --default required"));

	إذا (opt_branch && opt_default)
		die(_("options '%s' and '%s' cannot be used together"), "--branch", "--default");

	إذا (argc != 1 || !(مسیر = argv[0]))
		usage_with_options(usage, options);

	sub = submodule_from_مسیر(the_repository, null_oid(the_hash_algo), مسیر);

	إذا (!sub)
		die(_("no submodule mapping found in .gitmodules for path '%s'"),
		    مسیر);

	config_name = xstrfmt("submodule.%s.branch", sub->name);
	ret = config_مجموعه_درون_gitmodules_file_gently(the_repository, config_name, opt_branch);

	حرر(config_name);
	أرجع !!ret;
}

ساكن صحيح module_create_branch(صحيح argc, ثابت حرف **argv, ثابت حرف *پیشوند,
				بنية repository *repo استفادهنشده)
{
	تعداد branch_track track;
	صحيح quiet = 0, force = 0, reflog = 0, dry_پیاپی = 0;
	بنية option options[] = {
		OPT__QUIET(&quiet, N_("print only error messages")),
		OPT__FORCE(&force, N_("force creation"), 0),
		OPT_BOOL(0, "create-reflog", &reflog,
			 N_("create the branch's reflog")),
		OPT_CALLBACK_F('t', "track",  &track, "(direct|inherit)",
			N_("set branch tracking configuration"),
			PARSE_OPT_OPTARG,
			parse_opt_tracking_نما),
		OPT_DRY_پیاپی(&dry_پیاپی,
			     N_("show whether the branch would be created")),
		OPT_انتها()
	};
	ثابت حرف *ثابت usage[] = {
		N_("git submodule--helper create-branch [-f|--force] [--create-reflog] [-q|--quiet] [-t|--track] [-n|--dry-run] <name> <start-oid> <start-name>"),
		عدم
	};
	بنية repo_config_مقادیر *cfg = repo_config_مقادیر(the_repository);

	repo_config(the_repository, git_default_config, عدم);
	track = cfg->branch_track;
	argc = parse_options(argc, argv, پیشوند, options, usage, 0);

	إذا (argc != 3)
		usage_with_options(usage, options);

	إذا (!quiet && !dry_پیاپی)
		printf_ln(_("creating branch '%s'"), argv[0]);

	create_branches_recursively(the_repository, argv[0], argv[1], argv[2],
				    force, reflog, quiet, track, dry_پیاپی);
	أرجع 0;
}

بنية جمع_کن_داده {
	ثابت حرف *پیشوند;
	ثابت حرف *branch;
	ثابت حرف *reference_مسیر;
	حرف *sm_مسیر;
	ثابت حرف *sm_name;
	ثابت حرف *repo;
	ثابت حرف *realrepo;
	تعداد ref_storage_format ref_storage_format;
	صحيح عمق;
	بیعلامت صحيح force: 1;
	بیعلامت صحيح quiet: 1;
	بیعلامت صحيح پیشرفت: 1;
	بیعلامت صحيح dissociate: 1;
};
#تعریف_کن جمع_کن_داده_INIT { \
	.عمق = -1, \
	.ref_storage_format = REF_STORAGE_FORMAT_UNKNOWN, \
}

ساكن فراغ append_fetch_remotes(بنية strbuf *پیام, ثابت حرف *git_dir_مسیر)
{
	بنية فرزند_process cp_remote = فرزند_PROCESS_INIT;
	بنية strbuf sb_remote_بیرون = STRBUF_INIT;

	cp_remote.git_cmd = 1;
	strvec_pushf(&cp_remote.env,
		     "GIT_DIR=%s", git_dir_مسیر);
	strvec_افزودن(&cp_remote.env, "GIT_WORK_TREE=.");
	strvec_pushl(&cp_remote.آرگومانها, "remote", "-v", عدم);
	إذا (!capture_command(&cp_remote, &sb_remote_بیرون, 0)) {
		حرف *بعدی_سطر;
		حرف *سطر = sb_remote_بیرون.buf;

		طالما ((بعدی_سطر = strchr(سطر, '\n')) != عدم) {
			اندازه_t len = بعدی_سطر - سطر;

			إذا (strip_suffix_mem(سطر, &len, " (fetch)"))
				strbuf_addf(پیام, "  %.*s\n", (صحيح)len, سطر);
			سطر = بعدی_سطر + 1;
		}
	}

	strbuf_release(&sb_remote_بیرون);
}

ساكن صحيح جمع_کن_submodule(ثابت بنية جمع_کن_داده *جمع_کن_داده)
{
	بنية module_clone_داده clone_داده = MODULE_CLONE_داده_INIT;
	بنية string_list reference = STRING_LIST_INIT_NODUP;
	صحيح ret = -1;

	/* perhaps the path already exists and is already a git repo, else clone it */
	إذا (is_directory(جمع_کن_داده->sm_مسیر)) {
		حرف *submod_gitdir_مسیر;
		بنية strbuf sm_مسیر = STRBUF_INIT;
		strbuf_addstr(&sm_مسیر, جمع_کن_داده->sm_مسیر);
		submod_gitdir_مسیر = xstrfmt("%s/.git", جمع_کن_داده->sm_مسیر);
		إذا (is_nonbare_repository_dir(&sm_مسیر))
			چاپ_کن(_("Adding existing repo at '%s' to the index\n"),
			       جمع_کن_داده->sm_مسیر);
		وإلا
			die(_("'%s' already exists and is not a valid git repo"),
			    جمع_کن_داده->sm_مسیر);
		strbuf_release(&sm_مسیر);
		حرر(submod_gitdir_مسیر);
	} وإلا {
		بنية فرزند_process cp = فرزند_PROCESS_INIT;
		بنية strbuf submod_gitdir = STRBUF_INIT;

		submodule_name_مقصد_gitdir(&submod_gitdir, the_repository, جمع_کن_داده->sm_name);

		إذا (is_directory(submod_gitdir.buf)) {
			إذا (!جمع_کن_داده->force) {
				بنية strbuf پیام = STRBUF_INIT;
				حرف *die_پیام_2;

				strbuf_addf(&پیام, _("A git directory for '%s' is found "
						    "locally with remote(s):\n"),
					    جمع_کن_داده->sm_name);

				append_fetch_remotes(&پیام, submod_gitdir.buf);
				strbuf_release(&submod_gitdir);

				strbuf_addf(&پیام, _("If you want to reuse this local git "
						    "directory instead of cloning again from\n"
						    "  %s\n"
						    "use the '--force' option. If the local git "
						    "directory is not the correct repo\n"
						    "or you are unsure what this means choose "
						    "another name with the '--name' option."),
					    جمع_کن_داده->realrepo);

				die_پیام_2 = strbuf_detach(&پیام, عدم);
				die("%s", die_پیام_2);
			} وإلا {
				چاپ_کن(_("Reactivating local git directory for "
					 "submodule '%s'\n"), جمع_کن_داده->sm_name);
			}
		}
		strbuf_release(&submod_gitdir);

		clone_داده.پیشوند = جمع_کن_داده->پیشوند;
		clone_داده.مسیر = جمع_کن_داده->sm_مسیر;
		clone_داده.name = جمع_کن_داده->sm_name;
		clone_داده.url = جمع_کن_داده->realrepo;
		clone_داده.quiet = جمع_کن_داده->quiet;
		clone_داده.پیشرفت = جمع_کن_داده->پیشرفت;
		إذا (جمع_کن_داده->reference_مسیر) {
			حرف *p = xstrdup(جمع_کن_داده->reference_مسیر);

			string_list_append(&reference, p)->util = p;
		}
		clone_داده.ref_storage_format = جمع_کن_داده->ref_storage_format;
		clone_داده.dissociate = جمع_کن_داده->dissociate;
		إذا (جمع_کن_داده->عمق >= 0)
			clone_داده.عمق = جمع_کن_داده->عمق;

		إذا (clone_submodule(&clone_داده, &reference))
			اذهب cleanup;

		prepare_submodule_repo_env(&cp.env);
		cp.git_cmd = 1;
		cp.dir = جمع_کن_داده->sm_مسیر;
		/*
		 * NOTE: we only get here if add_data->force is true, so
		 * passing --force to checkout is reasonable.
		 */
		strvec_pushl(&cp.آرگومانها, "checkout", "-f", "-q", عدم);

		إذا (جمع_کن_داده->branch) {
			strvec_pushl(&cp.آرگومانها, "-B", جمع_کن_داده->branch, عدم);
			strvec_pushf(&cp.آرگومانها, "origin/%s", جمع_کن_داده->branch);
		}

		إذا (پیاپی_command(&cp))
			die(_("unable to checkout submodule '%s'"), جمع_کن_داده->sm_مسیر);
	}
	ret = 0;

cleanup:
	string_list_clear(&reference, 1);
	أرجع ret;
}

ساكن صحيح config_submodule_درون_gitmodules(ثابت حرف *name, ثابت حرف *var, ثابت حرف *مقدار)
{
	حرف *کلید;
	صحيح ret;

	إذا (!is_writing_gitmodules_ok())
		die(_("please make sure that the .gitmodules file is in the working tree"));

	کلید = xstrfmt("submodule.%s.%s", name, var);
	ret = config_مجموعه_درون_gitmodules_file_gently(the_repository, کلید, مقدار);
	حرر(کلید);

	أرجع ret;
}

ساكن فراغ configure_added_submodule(بنية جمع_کن_داده *جمع_کن_داده)
{
	حرف *کلید;
	بنية فرزند_process جمع_کن_submod = فرزند_PROCESS_INIT;
	بنية فرزند_process جمع_کن_gitmodules = فرزند_PROCESS_INIT;
	ثابت بنية string_list *مقادیر;
	صحيح matched = 0;

	کلید = xstrfmt("submodule.%s.url", جمع_کن_داده->sm_name);
	repo_config_مجموعه_gently(the_repository, کلید, جمع_کن_داده->realrepo);
	حرر(کلید);

	جمع_کن_submod.git_cmd = 1;
	strvec_pushl(&جمع_کن_submod.آرگومانها, "add",
		     "--no-warn-embedded-repo", عدم);
	إذا (جمع_کن_داده->force)
		strvec_افزودن(&جمع_کن_submod.آرگومانها, "--force");
	strvec_pushl(&جمع_کن_submod.آرگومانها, "--", جمع_کن_داده->sm_مسیر, عدم);

	إذا (پیاپی_command(&جمع_کن_submod))
		die(_("Failed to add submodule '%s'"), جمع_کن_داده->sm_مسیر);

	إذا (config_submodule_درون_gitmodules(جمع_کن_داده->sm_name, "path", جمع_کن_داده->sm_مسیر) ||
	    config_submodule_درون_gitmodules(جمع_کن_داده->sm_name, "url", جمع_کن_داده->repo))
		die(_("Failed to register submodule '%s'"), جمع_کن_داده->sm_مسیر);

	إذا (جمع_کن_داده->branch) {
		إذا (config_submodule_درون_gitmodules(جمع_کن_داده->sm_name,
						   "branch", جمع_کن_داده->branch))
			die(_("Failed to register submodule '%s'"), جمع_کن_داده->sm_مسیر);
	}

	جمع_کن_gitmodules.git_cmd = 1;
	strvec_pushl(&جمع_کن_gitmodules.آرگومانها,
		     "add", "--force", "--", ".gitmodules", عدم);

	إذا (پیاپی_command(&جمع_کن_gitmodules))
		die(_("Failed to register submodule '%s'"), جمع_کن_داده->sm_مسیر);

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
	    repo_config_get_string_multi(the_repository, "submodule.active", &مقادیر)) {
		/*
		 * If the submodule being added isn't already covered by the
		 * current configured pathspec, set the submodule's active flag
		 */
		کلید = xstrfmt("submodule.%s.active", جمع_کن_داده->sm_name);
		repo_config_مجموعه_gently(the_repository, کلید, "true");
		حرر(کلید);
	} وإلا {
		لكل (اندازه_t i = 0; i < مقادیر->nr; i++) {
			ثابت حرف *pat = مقادیر->اقلام[i].string;
			إذا (!wildmatch(pat, جمع_کن_داده->sm_مسیر, 0)) { /* match found */
				matched = 1;
				اكسر;
			}
		}
		إذا (!matched) { /* no pattern matched -> force-enable */
			کلید = xstrfmt("submodule.%s.active", جمع_کن_داده->sm_name);
			repo_config_مجموعه_gently(the_repository, کلید, "true");
			حرر(کلید);
		}
	}
}

ساكن فراغ die_on_شاخص_تطابق(ثابت حرف *مسیر, صحيح force)
{
	بنية pathspec ps;
	ثابت حرف *آرگومانها[] = { مسیر, عدم };
	parse_pathspec(&ps, 0, PATHSPEC_PREFER_CWD, عدم, آرگومانها);

	إذا (repo_read_شاخص_preload(the_repository, عدم, 0) < 0)
		die(_("index file corrupt"));

	إذا (ps.nr) {
		حرف *ps_matched = xcalloc(ps.nr, 1);

		/* TODO: audit for interaction with sparse-index. */
		ensure_پر_شاخص(the_repository->شاخص);

		/*
		 * Since there is only one pathspec, we just need to
		 * check ps_matched[0] to know if a cache entry matched.
		 */
		لكل (اندازه_t i = 0; i < the_repository->شاخص->cache_nr; i++) {
			ce_مسیر_تطابق(the_repository->شاخص, the_repository->شاخص->cache[i], &ps,
				      ps_matched);

			إذا (ps_matched[0]) {
				إذا (!force)
					die(_("'%s' already exists in the index"),
					    مسیر);
				إذا (!S_ISGITLINK(the_repository->شاخص->cache[i]->ce_نما))
					die(_("'%s' already exists in the index "
					      "and is not a submodule"), مسیر);
				اكسر;
			}
		}
		حرر(ps_matched);
	}
	clear_pathspec(&ps);
}

ساكن فراغ die_on_repo_without_commits(ثابت حرف *مسیر)
{
	بنية strbuf sb = STRBUF_INIT;
	strbuf_addstr(&sb, مسیر);
	إذا (is_nonbare_repository_dir(&sb)) {
		بنية object_id oid;
		إذا (repo_resolve_gitlink_ref(the_repository, مسیر, "HEAD", &oid) < 0)
			die(_("'%s' does not have a commit checked out"), مسیر);
	}
	strbuf_release(&sb);
}

ساكن صحيح module_جمع_کن(صحيح argc, ثابت حرف **argv, ثابت حرف *پیشوند,
		      بنية repository *repo استفادهنشده)
{
	صحيح force = 0, quiet = 0, پیشرفت = 0, dissociate = 0;
	بنية جمع_کن_داده جمع_کن_داده = جمع_کن_داده_INIT;
	ثابت حرف *ref_storage_format = عدم;
	حرف *مقصد_free = عدم;
	ثابت بنية submodule *existing;
	بنية strbuf buf = STRBUF_INIT;
	حرف *sm_name_مقصد_free = عدم;
	بنية option options[] = {
		OPT_STRING('b', "branch", &جمع_کن_داده.branch, N_("branch"),
			   N_("branch of repository to add as submodule")),
		OPT__FORCE(&force, N_("allow adding an otherwise ignored submodule path"),
			   PARSE_OPT_NOCOMPLETE),
		OPT__QUIET(&quiet, N_("print only error messages")),
		OPT_BOOL(0, "progress", &پیشرفت, N_("force cloning progress")),
		OPT_STRING(0, "reference", &جمع_کن_داده.reference_مسیر, N_("repository"),
			   N_("reference repository")),
		OPT_STRING(0, "ref-storage-format", &ref_storage_format, N_("format"),
			   N_("specify the reference storage format to use")),
		OPT_ALIAS_F(0, "ref-format", "ref-storage-format", PARSE_OPT_HIDDEN),
		OPT_BOOL(0, "dissociate", &dissociate, N_("borrow the objects from reference repositories")),
		OPT_STRING(0, "name", &جمع_کن_داده.sm_name, N_("name"),
			   N_("sets the submodule's name to the given string "
			      "instead of defaulting to its path")),
		OPT_INTEGER(0, "depth", &جمع_کن_داده.عمق, N_("depth for shallow clones")),
		OPT_انتها()
	};
	ثابت حرف *ثابت usage[] = {
		N_("git submodule add [<options>] [--] <repository> [<path>]"),
		عدم
	};
	بنية strbuf sb = STRBUF_INIT;
	صحيح ret = 1;

	argc = parse_options(argc, argv, پیشوند, options, usage, 0);

	إذا (!is_writing_gitmodules_ok())
		die(_("please make sure that the .gitmodules file is in the working tree"));

	إذا (پیشوند && *پیشوند &&
	    جمع_کن_داده.reference_مسیر && !is_absolute_مسیر(جمع_کن_داده.reference_مسیر))
		جمع_کن_داده.reference_مسیر = xstrfmt("%s%s", پیشوند, جمع_کن_داده.reference_مسیر);

	إذا (argc == 0 || argc > 2)
		usage_with_options(usage, options);

	إذا (ref_storage_format) {
		جمع_کن_داده.ref_storage_format = ref_storage_format_by_name(ref_storage_format);
		إذا (جمع_کن_داده.ref_storage_format == REF_STORAGE_FORMAT_UNKNOWN)
			die(_("unknown ref storage format '%s'"), ref_storage_format);
	}

	جمع_کن_داده.repo = argv[0];
	إذا (argc == 1)
		جمع_کن_داده.sm_مسیر = git_url_basename(جمع_کن_داده.repo, 0, 0);
	وإلا
		جمع_کن_داده.sm_مسیر = xstrdup(argv[1]);

	إذا (پیشوند && *پیشوند && !is_absolute_مسیر(جمع_کن_داده.sm_مسیر)) {
		حرف *sm_مسیر = جمع_کن_داده.sm_مسیر;

		جمع_کن_داده.sm_مسیر = xstrfmt("%s%s", پیشوند, sm_مسیر);
		حرر(sm_مسیر);
	}

	إذا (starts_with_dot_dot_slash(جمع_کن_داده.repo) ||
	    starts_with_dot_slash(جمع_کن_داده.repo)) {
		إذا (پیشوند)
			die(_("Relative path can only be used from the toplevel "
			      "of the working tree"));

		/* dereference source url relative to parent's url */
		مقصد_free = resolve_relative_url(جمع_کن_داده.repo, عدم, 1);
		جمع_کن_داده.realrepo = مقصد_free;
	} وإلا إذا (is_dir_sep(جمع_کن_داده.repo[0]) || strchr(جمع_کن_داده.repo, ':')) {
		جمع_کن_داده.realrepo = جمع_کن_داده.repo;
	} وإلا {
		die(_("repo URL: '%s' must be absolute or begin with ./|../"),
		    جمع_کن_داده.repo);
	}

	/*
	 * normalize path:
	 * multiple //; leading ./; /./; /../;
	 */
	normalize_مسیر_copy(جمع_کن_داده.sm_مسیر, جمع_کن_داده.sm_مسیر);
	strip_dir_trailing_slashes(جمع_کن_داده.sm_مسیر);

	إذا (validate_submodule_مسیر(جمع_کن_داده.sm_مسیر) < 0)
		die(عدم);

	die_on_شاخص_تطابق(جمع_کن_داده.sm_مسیر, force);
	die_on_repo_without_commits(جمع_کن_داده.sm_مسیر);

	إذا (!force) {
		بنية فرزند_process cp = فرزند_PROCESS_INIT;

		cp.git_cmd = 1;
		cp.no_stdout = 1;
		strvec_pushl(&cp.آرگومانها, "add", "--dry-run", "--ignore-missing",
			     "--no-warn-embedded-repo", جمع_کن_داده.sm_مسیر, عدم);
		إذا ((ret = pipe_command(&cp, عدم, 0, عدم, 0, &sb, 0))) {
			strbuf_complete_سطر(&sb);
			fputs(sb.buf, stderr);
			اذهب cleanup;
		}
	}

	إذا (!جمع_کن_داده.sm_name)
		جمع_کن_داده.sm_name = جمع_کن_داده.sm_مسیر;

	existing = submodule_from_name(the_repository,
					null_oid(the_hash_algo),
					جمع_کن_داده.sm_name);

	إذا (existing && existing->مسیر &&
	    strcmp(existing->مسیر, جمع_کن_داده.sm_مسیر)) {
		إذا (!force) {
			die(_("submodule name '%s' already used for path '%s'"),
			    جمع_کن_داده.sm_name, existing->مسیر);
		}
		/* --force: build <name><n> until unique */
		لكل (صحيح i = 1; ; i++) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "%s%d", جمع_کن_داده.sm_name, i);
			إذا (!submodule_from_name(the_repository,
						null_oid(the_hash_algo),
						buf.buf)) {
				اكسر;
			}
		}
		جمع_کن_داده.sm_name = sm_name_مقصد_free = strbuf_detach(&buf, عدم);
	}

	إذا (check_submodule_name(جمع_کن_داده.sm_name))
		die(_("'%s' is not a valid submodule name"), جمع_کن_داده.sm_name);

	جمع_کن_داده.پیشوند = پیشوند;
	جمع_کن_داده.force = !!force;
	جمع_کن_داده.quiet = !!quiet;
	جمع_کن_داده.پیشرفت = !!پیشرفت;
	جمع_کن_داده.dissociate = !!dissociate;

	إذا (the_repository->repository_format_submodule_مسیر_cfg)
		create_default_gitdir_config(جمع_کن_داده.sm_name);

	إذا (جمع_کن_submodule(&جمع_کن_داده))
		اذهب cleanup;
	configure_added_submodule(&جمع_کن_داده);

	ret = 0;
cleanup:
	حرر(sm_name_مقصد_free);
	حرر(جمع_کن_داده.sm_مسیر);
	حرر(مقصد_free);
	strbuf_release(&sb);

	أرجع ret;
}

صحيح cmd_submodule__helper(صحيح argc,
			  ثابت حرف **argv,
			  ثابت حرف *پیشوند,
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
		OPT_SUBCOMMAND("add", &fn, module_جمع_کن),
		OPT_SUBCOMMAND("update", &fn, module_update),
		OPT_SUBCOMMAND("foreach", &fn, module_foreach),
		OPT_SUBCOMMAND("init", &fn, module_init),
		OPT_SUBCOMMAND("status", &fn, module_وضعیت),
		OPT_SUBCOMMAND("sync", &fn, module_sync),
		OPT_SUBCOMMAND("deinit", &fn, module_deinit),
		OPT_SUBCOMMAND("summary", &fn, module_summary),
		OPT_SUBCOMMAND("push-check", &fn, افزودن_check),
		OPT_SUBCOMMAND("absorbgitdirs", &fn, absorb_git_dirs),
		OPT_SUBCOMMAND("set-url", &fn, module_مجموعه_url),
		OPT_SUBCOMMAND("set-branch", &fn, module_مجموعه_branch),
		OPT_SUBCOMMAND("create-branch", &fn, module_create_branch),
		OPT_SUBCOMMAND("get-default-remote", &fn, module_get_default_remote),
		OPT_انتها()
	};
	argc = parse_options(argc, argv, پیشوند, options, usage, 0);

	أرجع fn(argc, argv, پیشوند, repo);
}
