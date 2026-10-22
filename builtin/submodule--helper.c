#تعریف_کړه USE_THE_REPOSITORY_VARIABLE

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

#تعریف_کړه OPT_QUIET (1 << 0)
#تعریف_کړه OPT_CACHED (1 << 1)
#تعریف_کړه OPT_RECURSIVE (1 << 2)
#تعریف_کړه OPT_FORCE (1 << 3)

تعريفنوع فراغ (*each_submodule_fn)(ثابت بنية cache_ثبت *list_توکی,
				  فراغ *cb_معلومات);

ساكن حرف *get_default_remote(فراغ)
{
	أرجع xstrdup(repo_default_remote(the_repository));
}

ساكن حرف *resolve_relative_url(ثابت حرف *rel_url, ثابت حرف *پورته_لار, صحيح quiet)
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
	resolved_url = relative_url(remoteurl, rel_url, پورته_لار);

	حرر(remote);
	حرر(remoteurl);
	strbuf_release(&remotesb);

	أرجع resolved_url;
}

ساكن صحيح get_default_remote_submodule(ثابت حرف *module_لار, حرف **default_remote)
{
	ثابت بنية submodule *sub;
	بنية repository subrepo;
	ثابت حرف *remote_name = عدم;
	حرف *url = عدم;
	صحيح ret = 0;

	sub = submodule_from_لار(the_repository, null_oid(the_hash_algo), module_لار);
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

	إذا (repo_submodule_init(&subrepo, the_repository, module_لار,
				null_oid(the_hash_algo)) < 0) {
		ret = die_پیغام(_("could not get a repository handle for submodule '%s'"),
				   module_لار);
		اذهب بهر;
	}

	/* Look up by URL first */
	إذا (url)
		remote_name = repo_remote_from_url(&subrepo, url);
	إذا (!remote_name)
		remote_name = repo_default_remote(&subrepo);

	*default_remote = xstrdup(remote_name);

بهر:
	repo_clear(&subrepo);
	حرر(url);

	أرجع ret;
}

ساكن صحيح module_get_default_remote(صحيح argc, ثابت حرف **argv, ثابت حرف *مختاړی,
				     بنية repository *repo ناکارول_شوی)
{
	ثابت حرف *لار;
	حرف *resolved_لار = عدم;
	حرف *default_remote = عدم;
	صحيح کوډ;
	بنية option options[] = {
		OPT_پای_ټکی()
	};
	ثابت حرف *ثابت usage[] = {
		N_("git submodule--helper get-default-remote <path>"),
		عدم
	};

	argc = parse_options(argc, argv, مختاړی, options, usage, 0);
	إذا (argc != 1)
		usage_with_options(usage, options);

	لار = argv[0];
	إذا (مختاړی && *مختاړی && !is_absolute_لار(لار)) {
		resolved_لار = xstrfmt("%s%s", مختاړی, لار);
		لار = resolved_لار;
	}

	کوډ = get_default_remote_submodule(لار, &default_remote);
	إذا (کوډ) {
		حرر(resolved_لار);
		أرجع کوډ;
	}

	چاپ_کړه("%s\n", default_remote);
	حرر(default_remote);
	حرر(resolved_لار);
	أرجع 0;
}

/* the result should be freed by the caller. */
ساكن حرف *get_submodule_displaypath(ثابت حرف *لار, ثابت حرف *مختاړی,
				       ثابت حرف *super_مختاړی)
{
	إذا (مختاړی && super_مختاړی) {
		BUG("cannot have prefix '%s' and superprefix '%s'",
		    مختاړی, super_مختاړی);
	} وإلا إذا (مختاړی) {
		بنية strbuf sb = STRBUF_INIT;
		حرف *displaypath = xstrdup(relative_لار(لار, مختاړی, &sb));
		strbuf_release(&sb);
		أرجع displaypath;
	} وإلا إذا (super_مختاړی) {
		أرجع xstrfmt("%s%s", super_مختاړی, لار);
	} وإلا {
		أرجع xstrdup(لار);
	}
}

ساكن حرف *compute_rev_name(ثابت حرف *sub_لار, ثابت حرف* object_id)
{
	بنية strbuf sb = STRBUF_INIT;
	ثابت حرف ***d;

	ساكن ثابت حرف *describe_bare[] = { عدم };

	ساكن ثابت حرف *describe_نښانونه[] = { "--tags", عدم };

	ساكن ثابت حرف *describe_لري[] = { "--contains", عدم };

	ساكن ثابت حرف *describe_all_always[] = { "--all", "--always", عدم };

	ساكن ثابت حرف **describe_argv[] = { describe_bare, describe_نښانونه,
						describe_لري,
						describe_all_always, عدم };

	لكل (d = describe_argv; *d; d++) {
		بنية ماشوم_process cp = ماشوم_PROCESS_INIT;
		prepare_submodule_repo_env(&cp.env);
		cp.dir = sub_لار;
		cp.git_cmd = 1;
		cp.no_stderr = 1;

		strvec_ټېل(&cp.دلیلونه, "describe");
		strvec_pushv(&cp.دلیلونه, *d);
		strvec_ټېل(&cp.دلیلونه, object_id);

		إذا (!capture_command(&cp, &sb, 0)) {
			strbuf_strip_suffix(&sb, "\n");
			أرجع strbuf_detach(&sb, عدم);
		}
	}

	strbuf_release(&sb);
	أرجع عدم;
}

بنية module_list {
	ثابت بنية cache_ثبت **entries;
	صحيح alloc, nr;
};
#تعریف_کړه MODULE_LIST_INIT { 0 }

ساكن فراغ module_list_release(بنية module_list *ml)
{
	حرر(ml->entries);
}

ساكن صحيح module_list_compute(ثابت حرف **argv,
			       ثابت حرف *مختاړی,
			       بنية pathspec *pathspec,
			       بنية module_list *list)
{
	صحيح پایله = 0;
	حرف *ps_matched = عدم;

	parse_pathspec(pathspec, 0,
		       PATHSPEC_PREFER_ډک,
		       مختاړی, argv);

	إذا (pathspec->nr)
		ps_matched = xcalloc(pathspec->nr, 1);

	إذا (repo_read_شاخص(the_repository) < 0)
		die(_("index file corrupt"));

	لكل (اندازه_t i = 0; i < the_repository->شاخص->cache_nr; i++) {
		ثابت بنية cache_ثبت *ce = the_repository->شاخص->cache[i];

		إذا (!سمون_pathspec(the_repository->شاخص, pathspec, ce->name, ce_namelen(ce),
				    0, ps_matched, 1) ||
		    !S_ISGITLINK(ce->ce_ډېرپېښ))
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

	إذا (ps_matched && report_لار_error(ps_matched, pathspec))
		پایله = -1;

	حرر(ps_matched);

	أرجع پایله;
}

ساكن فراغ module_list_فعال(بنية module_list *list)
{
	صحيح i;
	بنية module_list فعال_modules = MODULE_LIST_INIT;

	لكل (i = 0; i < list->nr; i++) {
		ثابت بنية cache_ثبت *ce = list->entries[i];

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

ساكن حرف *get_پورته_لار(ثابت حرف *لار)
{
	بنية strbuf sb = STRBUF_INIT;

	strbuf_addstrings(&sb, "../", شمېر_slashes(لار));

	/*
	 * Check if 'path' ends with slash or not
	 * for having the same output for dir/sub_dir
	 * and dir/sub_dir/
	 */
	إذا (!is_dir_sep(لار[مزي_اوږدوالی(لار) - 1]))
		strbuf_addstr(&sb, "../");

	أرجع strbuf_detach(&sb, عدم);
}

ساكن فراغ for_each_listed_submodule(ثابت بنية module_list *list,
				      each_submodule_fn fn, فراغ *cb_معلومات)
{
	صحيح i;

	لكل (i = 0; i < list->nr; i++)
		fn(list->entries[i], cb_معلومات);
}

بنية foreach_cb {
	صحيح argc;
	ثابت حرف **argv;
	ثابت حرف *مختاړی;
	ثابت حرف *super_مختاړی;
	صحيح quiet;
	صحيح recursive;
};
#تعریف_کړه FOREACH_CB_INIT { 0 }

ساكن فراغ runcommand_دننه_submodule_cb(ثابت بنية cache_ثبت *list_توکی,
				       فراغ *cb_معلومات)
{
	بنية foreach_cb *info = cb_معلومات;
	ثابت حرف *لار = list_توکی->name;
	ثابت بنية object_id *ce_oid = &list_توکی->oid;
	ثابت بنية submodule *sub;
	بنية ماشوم_process cp = ماشوم_PROCESS_INIT;
	حرف *displaypath;

	إذا (validate_submodule_لار(لار) < 0)
		die(عدم);

	displaypath = get_submodule_displaypath(لار, info->مختاړی,
						info->super_مختاړی);

	sub = submodule_from_لار(the_repository, null_oid(the_hash_algo), لار);

	إذا (!sub)
		die(_("No url found for submodule path '%s' in .gitmodules"),
			displaypath);

	إذا (!is_submodule_populated_gently(لار, عدم))
		اذهب cleanup;

	prepare_submodule_repo_env(&cp.env);

	/*
	 * For the purpose of executing <command> in the submodule,
	 * separate shell is used for the purpose of running the
	 * child process.
	 */
	cp.use_shell = 1;
	cp.dir = لار;

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
		strvec_pushf(&cp.env, "sm_path=%s", لار);
		strvec_pushf(&cp.env, "displaypath=%s", displaypath);
		strvec_pushf(&cp.env, "sha1=%s",
			     oid_منزل_hex(ce_oid));
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
		sq_quote_buf(&sb, لار);
		strvec_pushf(&cp.دلیلونه, "path=%s; %s",
			     sb.buf, info->argv[0]);
		strbuf_release(&sb);
		حرر(toplevel);
	} وإلا {
		strvec_pushv(&cp.دلیلونه, info->argv);
	}

	إذا (!info->quiet)
		چاپ_کړه(_("Entering '%s'\n"), displaypath);

	إذا (info->argv[0]) {
		إذا (پرلپسې_command(&cp))
			die(_("run_command returned non-zero status for %s\n."),
			    displaypath);
	} وإلا {
		ماشوم_process_clear(&cp);
	}

	إذا (info->recursive) {
		بنية ماشوم_process cpr = ماشوم_PROCESS_INIT;

		cpr.git_cmd = 1;
		cpr.dir = لار;
		prepare_submodule_repo_env(&cpr.env);

		strvec_pushl(&cpr.دلیلونه, "submodule--helper", "foreach", "--recursive",
			     عدم);
		strvec_pushf(&cpr.دلیلونه, "--super-prefix=%s/", displaypath);

		إذا (info->quiet)
			strvec_ټېل(&cpr.دلیلونه, "--quiet");

		strvec_ټېل(&cpr.دلیلونه, "--");
		strvec_pushv(&cpr.دلیلونه, info->argv);

		إذا (پرلپسې_command(&cpr))
			die(_("run_command returned non-zero status while "
				"recursing in the nested submodules of %s\n."),
				displaypath);
	}

cleanup:
	حرر(displaypath);
}

ساكن صحيح module_foreach(صحيح argc, ثابت حرف **argv, ثابت حرف *مختاړی,
			  بنية repository *repo ناکارول_شوی)
{
	بنية foreach_cb info = FOREACH_CB_INIT;
	بنية pathspec pathspec = { 0 };
	بنية module_list list = MODULE_LIST_INIT;
	بنية option module_foreach_options[] = {
		OPT_SUPER_مختاړی(&info.super_مختاړی),
		OPT__QUIET(&info.quiet, N_("suppress output of entering each submodule command")),
		OPT_BOOL(0, "recursive", &info.recursive,
			 N_("recurse into nested submodules")),
		OPT_پای_ټکی()
	};
	ثابت حرف *ثابت git_submodule_helper_usage[] = {
		N_("git submodule foreach [--quiet] [--recursive] [--] <command>"),
		عدم
	};
	صحيح ret = 1;

	argc = parse_options(argc, argv, مختاړی, module_foreach_options,
			     git_submodule_helper_usage, 0);

	إذا (module_list_compute(عدم, مختاړی, &pathspec, &list) < 0)
		اذهب cleanup;

	info.argc = argc;
	info.argv = argv;
	info.مختاړی = مختاړی;

	for_each_listed_submodule(&list, runcommand_دننه_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	أرجع ret;
}

بنية init_cb {
	ثابت حرف *مختاړی;
	ثابت حرف *super_مختاړی;
	بینښه صحيح flags;
};
#تعریف_کړه INIT_CB_INIT { 0 }

ساكن صحيح validate_and_سېټ_submodule_gitdir(بنية strbuf *gitdir_لار,
					     ثابت حرف *submodule_name)
{
	ثابت حرف *ارزښت;
	حرف *کلی;

	إذا (validate_submodule_git_dir(gitdir_لار->buf, submodule_name))
		أرجع -1;

	کلی = xstrfmt("submodule.%s.gitdir", submodule_name);

	/* Nothing to do if the config already exists. */
	إذا (!repo_config_get_string_tmp(the_repository, کلی, &ارزښت)) {
		حرر(کلی);
		أرجع 0;
	}

	إذا (repo_config_سېټ_gently(the_repository, کلی, gitdir_لار->buf)) {
		حرر(کلی);
		أرجع -1;
	}

	حرر(کلی);
	أرجع 0;
}

ساكن فراغ create_default_gitdir_config(ثابت حرف *submodule_name)
{
	بنية strbuf gitdir_لار = STRBUF_INIT;
	بنية git_hash_ctx ctx;
	حرف hex_name_hash[GIT_MAX_HEXSZ + 1], سرلیک[128];
	بینښه حرف raw_name_hash[GIT_MAX_RAWSZ];
	صحيح سرلیک_len;

	/* Case 1: try the plain module name */
	repo_git_لار_append(the_repository, &gitdir_لار, "modules/%s", submodule_name);
	إذا (!validate_and_سېټ_submodule_gitdir(&gitdir_لار, submodule_name)) {
		strbuf_release(&gitdir_لار);
		أرجع;
	}

	/* Case 2.1: Try URI-safe (RFC3986) encoding first, this fixes nested gitdirs */
	strbuf_reset(&gitdir_لار);
	repo_git_لار_append(the_repository, &gitdir_لار, "modules/");
	strbuf_addstr_urlencode(&gitdir_لار, submodule_name, is_rfc3986_unreserved);
	إذا (!validate_and_سېټ_submodule_gitdir(&gitdir_لار, submodule_name)) {
		strbuf_release(&gitdir_لار);
		أرجع;
	}

	/* Case 2.2: Try extended uppercase URI (RFC3986) encoding, to fix case-folding */
	strbuf_reset(&gitdir_لار);
	repo_git_لار_append(the_repository, &gitdir_لار, "modules/");
	strbuf_addstr_urlencode(&gitdir_لار, submodule_name, is_casefolding_rfc3986_unreserved);
	إذا (!validate_and_سېټ_submodule_gitdir(&gitdir_لار, submodule_name))
		أرجع;

	/* Case 2.3: Try some derived gitdir names, see if one sticks */
	لكل (حرف c = '0'; c <= '9'; c++) {
		strbuf_reset(&gitdir_لار);
		repo_git_لار_append(the_repository, &gitdir_لار, "modules/");
		strbuf_addstr_urlencode(&gitdir_لار, submodule_name, is_rfc3986_unreserved);
		strbuf_addch(&gitdir_لار, c);
		إذا (!validate_and_سېټ_submodule_gitdir(&gitdir_لار, submodule_name))
			أرجع;

		strbuf_reset(&gitdir_لار);
		repo_git_لار_append(the_repository, &gitdir_لار, "modules/");
		strbuf_addstr_urlencode(&gitdir_لار, submodule_name, is_casefolding_rfc3986_unreserved);
		strbuf_addch(&gitdir_لار, c);
		إذا (!validate_and_سېټ_submodule_gitdir(&gitdir_لار, submodule_name))
			أرجع;
	}

	/* Case 2.4: If all the above failed, try a hash of the name as a last resort */
	سرلیک_len = snprintf(سرلیک, حجم(سرلیک),
			      "blob %"PRIuMAX, (uintmax_t)مزي_اوږدوالی(submodule_name));
	git_hash_init(&ctx, the_hash_algo);
	git_hash_update(&ctx, سرلیک, سرلیک_len);
	git_hash_update(&ctx, "\0", 1);
	git_hash_update(&ctx, submodule_name, مزي_اوږدوالی(submodule_name));
	git_hash_final(raw_name_hash, &ctx);
	hash_منزل_hex_algop_r(hex_name_hash, raw_name_hash, the_hash_algo);
	strbuf_reset(&gitdir_لار);
	repo_git_لار_append(the_repository, &gitdir_لار, "modules/%s", hex_name_hash);
	إذا (!validate_and_سېټ_submodule_gitdir(&gitdir_لار, submodule_name)) {
		strbuf_release(&gitdir_لار);
		أرجع;
	}

	/* Case 3: nothing worked, error out */
	die(_("failed to set a valid default config for 'submodule.%s.gitdir'. "
	      "Please ensure it is set, for example by running something like: "
	      "'git config submodule.%s.gitdir .git/modules/%s'"),
	    submodule_name, submodule_name, submodule_name);
}

ساكن فراغ init_submodule(ثابت حرف *لار, ثابت حرف *مختاړی,
			   ثابت حرف *super_مختاړی,
			   بینښه صحيح flags)
{
	ثابت بنية submodule *sub;
	بنية strbuf sb = STRBUF_INIT;
	ثابت حرف *upd;
	حرف *url = عدم, *displaypath;

	displaypath = get_submodule_displaypath(لار, مختاړی, super_مختاړی);

	sub = submodule_from_لار(the_repository, null_oid(the_hash_algo), لار);

	إذا (!sub)
		die(_("No url found for submodule path '%s' in .gitmodules"),
			displaypath);

	/*
	 * NEEDSWORK: In a multi-working-tree world, this needs to be
	 * set in the per-worktree config.
	 *
	 * Set active flag for the submodule being initialized
	 */
	إذا (!is_submodule_فعال(the_repository, لار)) {
		strbuf_addf(&sb, "submodule.%s.active", sub->name);
		repo_config_سېټ_gently(the_repository, sb.buf, "true");
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

		إذا (repo_config_سېټ_gently(the_repository, sb.buf, url))
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
			upd = submodule_update_type_منزل_string(sub->update_strategy.type);
		}

		إذا (repo_config_سېټ_gently(the_repository, sb.buf, upd))
			die(_("Failed to register update mode for submodule path '%s'"), displaypath);
	}

	إذا (the_repository->repository_format_submodule_لار_cfg)
		create_default_gitdir_config(sub->name);

	strbuf_release(&sb);
	حرر(displaypath);
	حرر(url);
}

ساكن فراغ init_submodule_cb(ثابت بنية cache_ثبت *list_توکی, فراغ *cb_معلومات)
{
	بنية init_cb *info = cb_معلومات;

	init_submodule(list_توکی->name, info->مختاړی, info->super_مختاړی,
		       info->flags);
}

ساكن صحيح module_init(صحيح argc, ثابت حرف **argv, ثابت حرف *مختاړی,
		       بنية repository *repo ناکارول_شوی)
{
	بنية init_cb info = INIT_CB_INIT;
	بنية pathspec pathspec = { 0 };
	بنية module_list list = MODULE_LIST_INIT;
	صحيح quiet = 0;
	بنية option module_init_options[] = {
		OPT__QUIET(&quiet, N_("suppress output for initializing a submodule")),
		OPT_پای_ټکی()
	};
	ثابت حرف *ثابت git_submodule_helper_usage[] = {
		N_("git submodule init [<options>] [<path>]"),
		عدم
	};
	صحيح ret = 1;

	argc = parse_options(argc, argv, مختاړی, module_init_options,
			     git_submodule_helper_usage, 0);

	إذا (module_list_compute(argv, مختاړی, &pathspec, &list) < 0)
		اذهب cleanup;

	/*
	 * If there are no path args and submodule.active is set then,
	 * by default, only initialize 'active' modules.
	 */
	إذا (!argc && !repo_config_get(the_repository, "submodule.active"))
		module_list_فعال(&list);

	info.مختاړی = مختاړی;
	إذا (quiet)
		info.flags |= OPT_QUIET;

	for_each_listed_submodule(&list, init_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	أرجع ret;
}

بنية حالت_cb {
	ثابت حرف *مختاړی;
	ثابت حرف *super_مختاړی;
	بینښه صحيح flags;
};
#تعریف_کړه حالت_CB_INIT { 0 }

ساكن فراغ print_حالت(بینښه صحيح flags, حرف state, ثابت حرف *لار,
			 ثابت بنية object_id *oid, ثابت حرف *displaypath)
{
	إذا (flags & OPT_QUIET)
		أرجع;

	چاپ_کړه("%c%s %s", state, oid_منزل_hex(oid), displaypath);

	إذا (state == ' ' || state == '+') {
		حرف *name = compute_rev_name(لار, oid_منزل_hex(oid));

		إذا (name)
			چاپ_کړه(" (%s)", name);
		حرر(name);
	}

	چاپ_کړه("\n");
}

ساكن صحيح handle_submodule_سر_ref(ثابت بنية reference *ref, فراغ *cb_معلومات)
{
	بنية object_id *output = cb_معلومات;

	إذا (ref->oid)
		oidcpy(output, ref->oid);

	أرجع 0;
}

ساكن فراغ حالت_submodule(ثابت حرف *لار, ثابت بنية object_id *ce_oid,
			     بینښه صحيح ce_flags, ثابت حرف *مختاړی,
			     ثابت حرف *super_مختاړی, بینښه صحيح flags)
{
	حرف *displaypath;
	بنية strvec diff_files_دلیلونه = STRVEC_INIT;
	بنية rev_info rev = REV_INFO_INIT;
	بنية strbuf buf = STRBUF_INIT;
	ثابت حرف *git_dir;

	إذا (validate_submodule_لار(لار) < 0)
		die(عدم);

	إذا (!submodule_from_لار(the_repository, null_oid(the_hash_algo), لار))
		die(_("no submodule mapping found in .gitmodules for path '%s'"),
		      لار);

	displaypath = get_submodule_displaypath(لار, مختاړی, super_مختاړی);

	إذا ((CE_STAGEMASK & ce_flags) >> CE_STAGESHIFT) {
		print_حالت(flags, 'U', لار, null_oid(the_hash_algo), displaypath);
		اذهب cleanup;
	}

	strbuf_addf(&buf, "%s/.git", لار);
	git_dir = read_gitfile(buf.buf);
	إذا (!git_dir)
		git_dir = buf.buf;

	إذا (!is_submodule_فعال(the_repository, لار) ||
	    !is_git_directory(git_dir)) {
		print_حالت(flags, '-', لار, ce_oid, displaypath);
		strbuf_release(&buf);
		اذهب cleanup;
	}
	strbuf_release(&buf);

	strvec_pushl(&diff_files_دلیلونه, "diff-files",
		     "--ignore-submodules=dirty", "--quiet", "--",
		     لار, عدم);

	repo_config(the_repository, git_diff_basic_config, عدم);

	repo_init_revisions(the_repository, &rev, عدم);
	rev.abbrev = 0;
	تیاری_revisions_from_strvec(&diff_files_دلیلونه, &rev, عدم);
	پرلپسې_diff_files(&rev, 0);

	إذا (!diff_پایله_کوډ(&rev)) {
		print_حالت(flags, ' ', لار, ce_oid,
			     displaypath);
	} وإلا إذا (!(flags & OPT_CACHED)) {
		بنية object_id oid;
		بنية ref_store *refs = repo_get_submodule_ref_store(the_repository,
								      لار);

		إذا (!refs) {
			print_حالت(flags, '-', لار, ce_oid, displaypath);
			اذهب cleanup;
		}
		إذا (refs_سر_ref(refs, handle_submodule_سر_ref, &oid))
			die(_("could not resolve HEAD ref inside the "
			      "submodule '%s'"), لار);

		print_حالت(flags, '+', لار, &oid, displaypath);
	} وإلا {
		print_حالت(flags, '+', لار, ce_oid, displaypath);
	}

	إذا (flags & OPT_RECURSIVE) {
		بنية ماشوم_process cpr = ماشوم_PROCESS_INIT;
		صحيح res;

		cpr.git_cmd = 1;
		cpr.dir = لار;
		prepare_submodule_repo_env(&cpr.env);

		strvec_pushl(&cpr.دلیلونه, "submodule--helper", "status",
			     "--recursive", عدم);
		strvec_pushf(&cpr.دلیلونه, "--super-prefix=%s/", displaypath);

		إذا (flags & OPT_CACHED)
			strvec_ټېل(&cpr.دلیلونه, "--cached");

		إذا (flags & OPT_QUIET)
			strvec_ټېل(&cpr.دلیلونه, "--quiet");

		res = پرلپسې_command(&cpr);
		إذا (res == SIGPIPE + 128)
			ارفع(SIGPIPE);
		وإلا إذا (res)
			die(_("failed to recurse into submodule '%s'"), لار);
	}

cleanup:
	strvec_clear(&diff_files_دلیلونه);
	حرر(displaypath);
	release_revisions(&rev);
}

ساكن فراغ حالت_submodule_cb(ثابت بنية cache_ثبت *list_توکی,
				فراغ *cb_معلومات)
{
	بنية حالت_cb *info = cb_معلومات;

	حالت_submodule(list_توکی->name, &list_توکی->oid, list_توکی->ce_flags,
			 info->مختاړی, info->super_مختاړی, info->flags);
}

ساكن صحيح module_حالت(صحيح argc, ثابت حرف **argv, ثابت حرف *مختاړی,
			 بنية repository *repo ناکارول_شوی)
{
	بنية حالت_cb info = حالت_CB_INIT;
	بنية pathspec pathspec = { 0 };
	بنية module_list list = MODULE_LIST_INIT;
	صحيح quiet = 0;
	بنية option module_حالت_options[] = {
		OPT_SUPER_مختاړی(&info.super_مختاړی),
		OPT__QUIET(&quiet, N_("suppress submodule status output")),
		OPT_بټ(0, "cached", &info.flags, N_("use commit stored in the index instead of the one stored in the submodule HEAD"), OPT_CACHED),
		OPT_بټ(0, "recursive", &info.flags, N_("recurse into nested submodules"), OPT_RECURSIVE),
		OPT_پای_ټکی()
	};
	ثابت حرف *ثابت git_submodule_helper_usage[] = {
		N_("git submodule status [--quiet] [--cached] [--recursive] [<path>...]"),
		عدم
	};
	صحيح ret = 1;

	argc = parse_options(argc, argv, مختاړی, module_حالت_options,
			     git_submodule_helper_usage, 0);

	إذا (module_list_compute(argv, مختاړی, &pathspec, &list) < 0)
		اذهب cleanup;

	info.مختاړی = مختاړی;
	إذا (quiet)
		info.flags |= OPT_QUIET;

	for_each_listed_submodule(&list, حالت_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	أرجع ret;
}

بنية module_cb {
	بینښه صحيح mod_src;
	بینښه صحيح mod_dst;
	بنية object_id oid_src;
	بنية object_id oid_dst;
	حرف حالت;
	حرف *sm_لار;
};
#تعریف_کړه MODULE_CB_INIT { 0 }

ساكن فراغ module_cb_release(بنية module_cb *mcb)
{
	حرر(mcb->sm_لار);
}

بنية module_cb_list {
	بنية module_cb **entries;
	صحيح alloc, nr;
};
#تعریف_کړه MODULE_CB_LIST_INIT { 0 }

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
	ثابت حرف *مختاړی;
	ثابت حرف *super_مختاړی;
	بینښه صحيح cached: 1;
	بینښه صحيح for_حالت: 1;
	بینښه صحيح files: 1;
	صحيح summary_حد;
};
#تعریف_کړه SUMMARY_CB_INIT { 0 }

تعداد diff_cmd {
	DIFF_شاخص,
	DIFF_FILES
};

ساكن حرف *verify_submodule_committish(ثابت حرف *sm_لار,
					 ثابت حرف *committish)
{
	بنية ماشوم_process cp_rev_parse = ماشوم_PROCESS_INIT;
	بنية strbuf پایله = STRBUF_INIT;

	cp_rev_parse.git_cmd = 1;
	cp_rev_parse.dir = sm_لار;
	prepare_submodule_repo_env(&cp_rev_parse.env);
	strvec_pushl(&cp_rev_parse.دلیلونه, "rev-parse", "-q", "--short", عدم);
	strvec_pushf(&cp_rev_parse.دلیلونه, "%s^0", committish);
	strvec_ټېل(&cp_rev_parse.دلیلونه, "--");

	إذا (capture_command(&cp_rev_parse, &پایله, 0))
		أرجع عدم;

	strbuf_trim_trailing_newline(&پایله);
	أرجع strbuf_detach(&پایله, عدم);
}

ساكن فراغ print_submodule_summary(بنية summary_cb *info, ثابت حرف *errmsg,
				    صحيح ټول_commits, ثابت حرف *displaypath,
				    ثابت حرف *src_abbrev, ثابت حرف *dst_abbrev,
				    بنية module_cb *p)
{
	إذا (p->حالت == 'T') {
		إذا (S_ISGITLINK(p->mod_dst))
			چاپ_کړه(_("* %s %s(blob)->%s(submodule)"),
				 displaypath, src_abbrev, dst_abbrev);
		وإلا
			چاپ_کړه(_("* %s %s(submodule)->%s(blob)"),
				 displaypath, src_abbrev, dst_abbrev);
	} وإلا {
		چاپ_کړه("* %s %s...%s",
			displaypath, src_abbrev, dst_abbrev);
	}

	إذا (ټول_commits < 0)
		چاپ_کړه(":\n");
	وإلا
		چاپ_کړه(" (%d):\n", ټول_commits);

	إذا (errmsg) {
		چاپ_کړه(_("%s"), errmsg);
	} وإلا إذا (ټول_commits > 0) {
		بنية ماشوم_process cp_log = ماشوم_PROCESS_INIT;

		cp_log.git_cmd = 1;
		cp_log.dir = p->sm_لار;
		prepare_submodule_repo_env(&cp_log.env);
		strvec_pushl(&cp_log.دلیلونه, "log", عدم);

		إذا (S_ISGITLINK(p->mod_src) && S_ISGITLINK(p->mod_dst)) {
			إذا (info->summary_حد > 0)
				strvec_pushf(&cp_log.دلیلونه, "-%d",
					     info->summary_حد);

			strvec_pushl(&cp_log.دلیلونه, "--pretty=  %m %s",
				     "--first-parent", عدم);
			strvec_pushf(&cp_log.دلیلونه, "%s...%s",
				     src_abbrev, dst_abbrev);
		} وإلا إذا (S_ISGITLINK(p->mod_dst)) {
			strvec_pushl(&cp_log.دلیلونه, "--pretty=  > %s",
				     "-1", dst_abbrev, عدم);
		} وإلا {
			strvec_pushl(&cp_log.دلیلونه, "--pretty=  < %s",
				     "-1", src_abbrev, عدم);
		}
		پرلپسې_command(&cp_log);
	}
	چاپ_کړه("\n");
}

ساكن فراغ generate_submodule_summary(بنية summary_cb *info,
				       بنية module_cb *p)
{
	حرف *displaypath, *src_abbrev = عدم, *dst_abbrev;
	صحيح missing_src = 0, missing_dst = 0;
	بنية strbuf errmsg = STRBUF_INIT;
	صحيح ټول_commits = -1;

	إذا (!info->cached && oideq(&p->oid_dst, null_oid(the_hash_algo))) {
		إذا (S_ISGITLINK(p->mod_dst)) {
			بنية ref_store *refs = repo_get_submodule_ref_store(the_repository,
									      p->sm_لار);

			إذا (refs)
				refs_سر_ref(refs, handle_submodule_سر_ref, &p->oid_dst);
		} وإلا إذا (S_ISLNK(p->mod_dst) || S_ISREG(p->mod_dst)) {
			بنية stat st;
			صحيح fd = open(p->sm_لار, O_RDONLY);

			إذا (fd < 0 || fstat(fd, &st) < 0 ||
			    شاخص_fd(the_repository->شاخص, &p->oid_dst, fd, &st, OBJ_BLOB,
				     p->sm_لار, 0))
				error(_("couldn't hash object from '%s'"), p->sm_لار);
		} وإلا {
			/* for a submodule removal (mode:0000000), don't warn */
			إذا (p->mod_dst)
				warning(_("unexpected mode %o"), p->mod_dst);
		}
	}

	إذا (S_ISGITLINK(p->mod_src)) {
		إذا (p->حالت != 'D')
			src_abbrev = verify_submodule_committish(p->sm_لار,
								 oid_منزل_hex(&p->oid_src));
		إذا (!src_abbrev) {
			missing_src = 1;
			/*
			 * As `rev-parse` failed, we fallback to getting
			 * the abbreviated hash using oid_src. We do
			 * this as we might still need the abbreviated
			 * hash in cases like a submodule type change, etc.
			 */
			src_abbrev = xstrndup(oid_منزل_hex(&p->oid_src), 7);
		}
	} وإلا {
		/*
		 * The source does not point to a submodule.
		 * So, we fallback to getting the abbreviation using
		 * oid_src as we might still need the abbreviated
		 * hash in cases like submodule add, etc.
		 */
		src_abbrev = xstrndup(oid_منزل_hex(&p->oid_src), 7);
	}

	إذا (S_ISGITLINK(p->mod_dst)) {
		dst_abbrev = verify_submodule_committish(p->sm_لار,
							 oid_منزل_hex(&p->oid_dst));
		إذا (!dst_abbrev) {
			missing_dst = 1;
			/*
			 * As `rev-parse` failed, we fallback to getting
			 * the abbreviated hash using oid_dst. We do
			 * this as we might still need the abbreviated
			 * hash in cases like a submodule type change, etc.
			 */
			dst_abbrev = xstrndup(oid_منزل_hex(&p->oid_dst), 7);
		}
	} وإلا {
		/*
		 * The destination does not point to a submodule.
		 * So, we fallback to getting the abbreviation using
		 * oid_dst as we might still need the abbreviated
		 * hash in cases like a submodule removal, etc.
		 */
		dst_abbrev = xstrndup(oid_منزل_hex(&p->oid_dst), 7);
	}

	displaypath = get_submodule_displaypath(p->sm_لار, info->مختاړی,
						info->super_مختاړی);

	إذا (!missing_src && !missing_dst) {
		بنية ماشوم_process cp_rev_list = ماشوم_PROCESS_INIT;
		بنية strbuf sb_rev_list = STRBUF_INIT;

		strvec_pushl(&cp_rev_list.دلیلونه, "rev-list",
			     "--first-parent", "--count", عدم);
		إذا (S_ISGITLINK(p->mod_src) && S_ISGITLINK(p->mod_dst))
			strvec_pushf(&cp_rev_list.دلیلونه, "%s...%s",
				     src_abbrev, dst_abbrev);
		وإلا
			strvec_ټېل(&cp_rev_list.دلیلونه, S_ISGITLINK(p->mod_src) ?
				    src_abbrev : dst_abbrev);
		strvec_ټېل(&cp_rev_list.دلیلونه, "--");

		cp_rev_list.git_cmd = 1;
		cp_rev_list.dir = p->sm_لار;
		prepare_submodule_repo_env(&cp_rev_list.env);

		إذا (!capture_command(&cp_rev_list, &sb_rev_list, 0))
			ټول_commits = atoi(sb_rev_list.buf);

		strbuf_release(&sb_rev_list);
	} وإلا {
		/*
		 * Don't give error msg for modification whose dst is not
		 * submodule, i.e., deleted or changed to blob
		 */
		إذا (S_ISGITLINK(p->mod_dst)) {
			إذا (missing_src && missing_dst) {
				strbuf_addf(&errmsg, "  Warn: %s doesn't contain commits %s and %s\n",
					    displaypath, oid_منزل_hex(&p->oid_src),
					    oid_منزل_hex(&p->oid_dst));
			} وإلا {
				strbuf_addf(&errmsg, "  Warn: %s doesn't contain commit %s\n",
					    displaypath, missing_src ?
					    oid_منزل_hex(&p->oid_src) :
					    oid_منزل_hex(&p->oid_dst));
			}
		}
	}

	print_submodule_summary(info, errmsg.len ? errmsg.buf : عدم,
				ټول_commits, displaypath, src_abbrev,
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

		إذا (p->حالت == 'D' || p->حالت == 'T') {
			generate_submodule_summary(info, p);
			استمر;
		}

		إذا (info->for_حالت && p->حالت != 'A' &&
		    (sub = submodule_from_لار(the_repository,
					       null_oid(the_hash_algo), p->sm_لار))) {
			حرف *config_کلی = عدم;
			ثابت حرف *ارزښت;
			صحيح ignore_all = 0;

			config_کلی = xstrfmt("submodule.%s.ignore",
					     sub->name);
			إذا (!repo_config_get_string_tmp(the_repository, config_کلی, &ارزښت))
				ignore_all = !strcmp(ارزښت, "all");
			وإلا إذا (sub->ignore)
				ignore_all = !strcmp(sub->ignore, "all");

			حرر(config_کلی);
			إذا (ignore_all)
				استمر;
		}

		/* Also show added or modified modules which are checked out */
		strbuf_addstr(&sm_gitdir, p->sm_لار);
		إذا (is_nonbare_repository_dir(&sm_gitdir))
			generate_submodule_summary(info, p);
		strbuf_release(&sm_gitdir);
	}
}

ساكن فراغ submodule_summary_callback(بنية diff_کتار_struct *q,
				       بنية diff_options *options ناکارول_شوی,
				       فراغ *معلومات)
{
	صحيح i;
	بنية module_cb_list *list = معلومات;
	لكل (i = 0; i < q->nr; i++) {
		بنية diff_filepair *p = q->کتار[i];
		بنية module_cb *لنډمهاله;

		إذا (!S_ISGITLINK(p->one->ډېرپېښ) && !S_ISGITLINK(p->two->ډېرپېښ))
			استمر;
		لنډمهاله = xmalloc(حجم(*لنډمهاله));
		لنډمهاله->mod_src = p->one->ډېرپېښ;
		لنډمهاله->mod_dst = p->two->ډېرپېښ;
		لنډمهاله->oid_src = p->one->oid;
		لنډمهاله->oid_dst = p->two->oid;
		لنډمهاله->حالت = p->حالت;
		لنډمهاله->sm_لار = xstrdup(p->one->لار);

		ALLOC_GROW(list->entries, list->nr + 1, list->alloc);
		list->entries[list->nr++] = لنډمهاله;
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
	بنية strvec diff_دلیلونه = STRVEC_INIT;
	بنية rev_info rev;
	بنية module_cb_list list = MODULE_CB_LIST_INIT;
	صحيح ret = 0;

	strvec_ټېل(&diff_دلیلونه, get_diff_cmd(diff_cmd));
	إذا (info->cached)
		strvec_ټېل(&diff_دلیلونه, "--cached");
	strvec_pushl(&diff_دلیلونه, "--ignore-submodules=dirty", "--raw", عدم);
	إذا (سر_oid)
		strvec_ټېل(&diff_دلیلونه, oid_منزل_hex(سر_oid));
	strvec_ټېل(&diff_دلیلونه, "--");
	إذا (info->argc)
		strvec_pushv(&diff_دلیلونه, info->argv);

	repo_config(the_repository, git_diff_basic_config, عدم);
	repo_init_revisions(the_repository, &rev, info->مختاړی);
	rev.abbrev = 0;
	precompose_argv_مختاړی(diff_دلیلونه.nr, diff_دلیلونه.v, عدم);
	تیاری_revisions_from_strvec(&diff_دلیلونه, &rev, عدم);
	rev.diffopt.output_format = DIFF_FORMAT_NO_OUTPUT | DIFF_FORMAT_CALLBACK;
	rev.diffopt.format_callback = submodule_summary_callback;
	rev.diffopt.format_callback_معلومات = &list;

	إذا (!info->cached) {
		إذا (diff_cmd == DIFF_شاخص)
			تیاری_work_ونه(the_repository);
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
		پرلپسې_diff_شاخص(&rev, info->cached ? DIFF_شاخص_CACHED : 0);
	وإلا
		پرلپسې_diff_files(&rev, 0);
	prepare_submodule_summary(info, &list);
cleanup:
	strvec_clear(&diff_دلیلونه);
	release_revisions(&rev);
	module_cb_list_release(&list);
	أرجع ret;
}

ساكن صحيح module_summary(صحيح argc, ثابت حرف **argv, ثابت حرف *مختاړی,
			  بنية repository *repo ناکارول_شوی)
{
	بنية summary_cb info = SUMMARY_CB_INIT;
	صحيح cached = 0;
	صحيح for_حالت = 0;
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
		OPT_BOOL(0, "for-status", &for_حالت,
			 N_("skip submodules with 'ignore_config' value set to 'all'")),
		OPT_INTEGER('n', "summary-limit", &summary_حد,
			     N_("limit the summary size")),
		OPT_پای_ټکی()
	};
	ثابت حرف *ثابت git_submodule_helper_usage[] = {
		N_("git submodule summary [<options>] [<commit>] [--] [<path>]"),
		عدم
	};

	argc = parse_options(argc, argv, مختاړی, module_summary_options,
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
		oidcpy(&سر_oid, the_hash_algo->تشه_ونه);
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
	info.مختاړی = مختاړی;
	info.cached = !!cached;
	info.files = !!files;
	info.for_حالت = !!for_حالت;
	info.summary_حد = summary_حد;

	ret = compute_summary_module_list((diff_cmd == DIFF_شاخص) ? &سر_oid : عدم,
					  &info, diff_cmd);
	أرجع ret;
}

ساكن صحيح module_gitdir(صحيح argc, ثابت حرف **argv, ثابت حرف *مختاړی ناکارول_شوی,
			 بنية repository *repo)
{
	بنية strbuf gitdir = STRBUF_INIT;

	إذا (argc != 2)
		usage(_("git submodule--helper gitdir <name>"));

	submodule_name_منزل_gitdir(&gitdir, repo, argv[1]);

	چاپ_کړه("%s\n", gitdir.buf);

	strbuf_release(&gitdir);
	أرجع 0;
}

ساكن صحيح module_migrate(صحيح argc ناکارول_شوی, ثابت حرف **argv ناکارول_شوی,
			  ثابت حرف *مختاړی ناکارول_شوی, بنية repository *repo)
{
	بنية strbuf module_dir = STRBUF_INIT;
	DIR *dir;
	بنية dirent *de;
	صحيح repo_نسخه = 0;

	repo_git_لار_append(repo, &module_dir, "modules/");

	dir = opendir(module_dir.buf);
	إذا (!dir)
		die(_("could not open '%s'"), module_dir.buf);

	طالما ((de = readdir(dir))) {
		بنية strbuf gitdir_لار = STRBUF_INIT;
		حرف *کلی;
		ثابت حرف *ارزښت;

		إذا (is_dot_or_dotdot(de->d_name))
			استمر;

		strbuf_addf(&gitdir_لار, "%s/%s", module_dir.buf, de->d_name);
		إذا (!is_git_directory(gitdir_لار.buf)) {
			strbuf_release(&gitdir_لار);
			استمر;
		}
		strbuf_release(&gitdir_لار);

		کلی = xstrfmt("submodule.%s.gitdir", de->d_name);
		إذا (!repo_config_get_string_tmp(repo, کلی, &ارزښت)) {
			/* Already has a gitdir config, nothing to do. */
			حرر(کلی);
			استمر;
		}
		حرر(کلی);

		create_default_gitdir_config(de->d_name);
	}

	closedir(dir);
	strbuf_release(&module_dir);

	repo_config_get_int(the_repository, "core.repositoryformatversion", &repo_نسخه);
	إذا (repo_نسخه == 0 &&
	    repo_config_سېټ_gently(repo, "core.repositoryformatversion", "1"))
		die(_("could not set core.repositoryformatversion to 1.\n"
		      "Please set it for migration to work, for example:\n"
		      "git config core.repositoryformatversion 1"));

	إذا (repo_config_سېټ_gently(repo, "extensions.submodulePathConfig", "true"))
		die(_("could not enable submodulePathConfig extension. It is required\n"
		      "for migration to work. Please enable it in the root repo:\n"
		      "git config extensions.submodulePathConfig true"));

	repo->repository_format_submodule_لار_cfg = 1;

	أرجع 0;
}

بنية sync_cb {
	ثابت حرف *مختاړی;
	ثابت حرف *super_مختاړی;
	بینښه صحيح flags;
};
#تعریف_کړه SYNC_CB_INIT { 0 }

ساكن فراغ sync_submodule(ثابت حرف *لار, ثابت حرف *مختاړی,
			   ثابت حرف *super_مختاړی, بینښه صحيح flags)
{
	ثابت بنية submodule *sub;
	حرف *remote_کلی = عدم;
	حرف *sub_origin_url, *super_config_url, *displaypath, *default_remote;
	بنية strbuf sb = STRBUF_INIT;
	حرف *sub_config_لار = عدم;
	صحيح کوډ;

	إذا (!is_submodule_فعال(the_repository, لار))
		أرجع;

	إذا (validate_submodule_لار(لار) < 0)
		die(عدم);

	sub = submodule_from_لار(the_repository, null_oid(the_hash_algo), لار);

	إذا (sub && sub->url) {
		إذا (starts_with_dot_dot_slash(sub->url) ||
		    starts_with_dot_slash(sub->url)) {
			حرف *پورته_لار = get_پورته_لار(لار);

			sub_origin_url = resolve_relative_url(sub->url, پورته_لار, 1);
			super_config_url = resolve_relative_url(sub->url, عدم, 1);
			حرر(پورته_لار);
		} وإلا {
			sub_origin_url = xstrdup(sub->url);
			super_config_url = xstrdup(sub->url);
		}
	} وإلا {
		sub_origin_url = xstrdup("");
		super_config_url = xstrdup("");
	}

	displaypath = get_submodule_displaypath(لار, مختاړی, super_مختاړی);

	إذا (!(flags & OPT_QUIET))
		چاپ_کړه(_("Synchronizing submodule url for '%s'\n"),
			 displaypath);

	strbuf_reset(&sb);
	strbuf_addf(&sb, "submodule.%s.url", sub->name);
	إذا (repo_config_سېټ_gently(the_repository, sb.buf, super_config_url))
		die(_("failed to register url for submodule path '%s'"),
		      displaypath);

	إذا (!is_submodule_populated_gently(لار, عدم))
		اذهب cleanup;

	strbuf_reset(&sb);
	کوډ = get_default_remote_submodule(لار, &default_remote);
	إذا (کوډ)
		وتل(کوډ);

	remote_کلی = xstrfmt("remote.%s.url", default_remote);
	حرر(default_remote);

	submodule_منزل_gitdir(the_repository, &sb, لار);
	strbuf_addstr(&sb, "/config");

	إذا (repo_config_سېټ_دننه_file_gently(the_repository, sb.buf, remote_کلی, عدم, sub_origin_url))
		die(_("failed to update remote for submodule '%s'"),
		      لار);

	إذا (flags & OPT_RECURSIVE) {
		بنية ماشوم_process cpr = ماشوم_PROCESS_INIT;

		cpr.git_cmd = 1;
		cpr.dir = لار;
		prepare_submodule_repo_env(&cpr.env);

		strvec_pushl(&cpr.دلیلونه, "submodule--helper", "sync",
			     "--recursive", عدم);
		strvec_pushf(&cpr.دلیلونه, "--super-prefix=%s/", displaypath);

		إذا (flags & OPT_QUIET)
			strvec_ټېل(&cpr.دلیلونه, "--quiet");

		إذا (پرلپسې_command(&cpr))
			die(_("failed to recurse into submodule '%s'"),
			      لار);
	}

cleanup:
	حرر(super_config_url);
	حرر(sub_origin_url);
	strbuf_release(&sb);
	حرر(remote_کلی);
	حرر(displaypath);
	حرر(sub_config_لار);
}

ساكن فراغ sync_submodule_cb(ثابت بنية cache_ثبت *list_توکی, فراغ *cb_معلومات)
{
	بنية sync_cb *info = cb_معلومات;

	sync_submodule(list_توکی->name, info->مختاړی, info->super_مختاړی,
		       info->flags);
}

ساكن صحيح module_sync(صحيح argc, ثابت حرف **argv, ثابت حرف *مختاړی,
		       بنية repository *repo ناکارول_شوی)
{
	بنية sync_cb info = SYNC_CB_INIT;
	بنية pathspec pathspec = { 0 };
	بنية module_list list = MODULE_LIST_INIT;
	صحيح quiet = 0;
	صحيح recursive = 0;
	بنية option module_sync_options[] = {
		OPT_SUPER_مختاړی(&info.super_مختاړی),
		OPT__QUIET(&quiet, N_("suppress output of synchronizing submodule url")),
		OPT_BOOL(0, "recursive", &recursive,
			N_("recurse into nested submodules")),
		OPT_پای_ټکی()
	};
	ثابت حرف *ثابت git_submodule_helper_usage[] = {
		N_("git submodule sync [--quiet] [--recursive] [<path>]"),
		عدم
	};
	صحيح ret = 1;

	argc = parse_options(argc, argv, مختاړی, module_sync_options,
			     git_submodule_helper_usage, 0);

	إذا (module_list_compute(argv, مختاړی, &pathspec, &list) < 0)
		اذهب cleanup;

	info.مختاړی = مختاړی;
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
	ثابت حرف *مختاړی;
	بینښه صحيح flags;
};
#تعریف_کړه DEINIT_CB_INIT { 0 }

ساكن فراغ deinit_submodule(ثابت حرف *لار, ثابت حرف *مختاړی,
			     بینښه صحيح flags)
{
	ثابت بنية submodule *sub;
	حرف *displaypath = عدم;
	بنية ماشوم_process cp_config = ماشوم_PROCESS_INIT;
	بنية strbuf sb_config = STRBUF_INIT;
	حرف *sub_git_dir = xstrfmt("%s/.git", لار);

	إذا (validate_submodule_لار(لار) < 0)
		die(عدم);

	sub = submodule_from_لار(the_repository, null_oid(the_hash_algo), لار);

	إذا (!sub || !sub->name)
		اذهب cleanup;

	displaypath = get_submodule_displaypath(لار, مختاړی, عدم);

	/* remove the submodule work tree (unless the user already did it) */
	إذا (is_directory(لار)) {
		بنية strbuf sb_rm = STRBUF_INIT;
		ثابت حرف *format;

		إذا (is_directory(sub_git_dir)) {
			إذا (!(flags & OPT_QUIET))
				warning(_("Submodule work tree '%s' contains a .git "
					  "directory. This will be replaced with a "
					  ".git file by using absorbgitdirs."),
					displaypath);

			absorb_git_dir_into_superproject(لار, عدم);

		}

		إذا (!(flags & OPT_FORCE)) {
			بنية ماشوم_process cp_rm = ماشوم_PROCESS_INIT;

			cp_rm.git_cmd = 1;
			strvec_pushl(&cp_rm.دلیلونه, "rm", "-qn",
				     لار, عدم);

			إذا (پرلپسې_command(&cp_rm))
				die(_("Submodule work tree '%s' contains local "
				      "modifications; use '-f' to discard them"),
				      displaypath);
		}

		strbuf_addstr(&sb_rm, لار);

		إذا (!remove_dir_recursively(&sb_rm, 0))
			format = _("Cleared directory '%s'\n");
		وإلا
			format = _("Could not remove submodule work tree '%s'\n");

		إذا (!(flags & OPT_QUIET))
			چاپ_کړه(format, displaypath);

		submodule_unset_core_worktree(sub);

		strbuf_release(&sb_rm);
	}

	إذا (mkdir(لار, 0777))
		چاپ_کړه(_("could not create empty submodule directory %s"),
		      displaypath);

	cp_config.git_cmd = 1;
	strvec_pushl(&cp_config.دلیلونه, "config", "--get-regexp", عدم);
	strvec_pushf(&cp_config.دلیلونه, "submodule.%s\\.", sub->name);

	/* remove the .git/config entries (unless the user already did it) */
	إذا (!capture_command(&cp_config, &sb_config, 0) && sb_config.len) {
		حرف *sub_کلی = xstrfmt("submodule.%s", sub->name);

		/*
		 * remove the whole section so we have a clean state when
		 * the user later decides to init this submodule again
		 */
		repo_config_rename_section_دننه_file(the_repository, عدم, sub_کلی, عدم);
		إذا (!(flags & OPT_QUIET))
			چاپ_کړه(_("Submodule '%s' (%s) unregistered for path '%s'\n"),
				 sub->name, sub->url, displaypath);
		حرر(sub_کلی);
	}

cleanup:
	حرر(displaypath);
	حرر(sub_git_dir);
	strbuf_release(&sb_config);
}

ساكن فراغ deinit_submodule_cb(ثابت بنية cache_ثبت *list_توکی,
				فراغ *cb_معلومات)
{
	بنية deinit_cb *info = cb_معلومات;
	deinit_submodule(list_توکی->name, info->مختاړی, info->flags);
}

ساكن صحيح module_deinit(صحيح argc, ثابت حرف **argv, ثابت حرف *مختاړی,
			 بنية repository *repo ناکارول_شوی)
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
		OPT_پای_ټکی()
	};
	ثابت حرف *ثابت git_submodule_helper_usage[] = {
		N_("git submodule deinit [--quiet] [-f | --force] [--all | [--] [<path>...]]"),
		عدم
	};
	صحيح ret = 1;

	argc = parse_options(argc, argv, مختاړی, module_deinit_options,
			     git_submodule_helper_usage, 0);

	إذا (all && argc) {
		error("pathspec and --all are incompatible");
		usage_with_options(git_submodule_helper_usage,
				   module_deinit_options);
	}

	إذا (!argc && !all)
		die(_("Use '--all' if you really want to deinitialize all submodules"));

	إذا (module_list_compute(argv, مختاړی, &pathspec, &list) < 0)
		اذهب cleanup;

	info.مختاړی = مختاړی;
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

بنية module_clone_معلومات {
	ثابت حرف *مختاړی;
	ثابت حرف *لار;
	ثابت حرف *name;
	ثابت حرف *url;
	صحيح ژوروالی;
	بنية list_objects_filter_options *filter_options;
	تعداد ref_storage_format ref_storage_format;
	بینښه صحيح quiet: 1;
	بینښه صحيح پرمختګ: 1;
	بینښه صحيح dissociate: 1;
	بینښه صحيح require_init: 1;
	صحيح single_branch;
};
#تعریف_کړه MODULE_CLONE_معلومات_INIT { \
	.single_branch = -1, \
	.ref_storage_format = REF_STORAGE_FORMAT_UNKNOWN, \
}

بنية submodule_alternate_تیاری {
	ثابت حرف *submodule_name;
	تعداد SUBMODULE_ALTERNATE_ERROR_ډېرپېښ {
		SUBMODULE_ALTERNATE_ERROR_DIE,
		SUBMODULE_ALTERNATE_ERROR_INFO,
		SUBMODULE_ALTERNATE_ERROR_IGNORE
	} error_ډېرپېښ;
	بنية string_list *reference;
};
#تعریف_کړه SUBMODULE_ALTERNATE_تیاری_INIT { \
	.error_ډېرپېښ = SUBMODULE_ALTERNATE_ERROR_IGNORE, \
}

ساكن ثابت حرف alternate_error_advice[] = N_(
"An alternate computed from a superproject's alternate is invalid.\n"
"To allow Git to clone without an alternate in such a case, set\n"
"submodule.alternateErrorStrategy to 'info' or, equivalently, clone with\n"
"'--reference-if-able' instead of '--reference'."
);

ساكن صحيح جمع_کړه_possible_reference_from_superproject(
		بنية odb_files_dir *alt_odb, فراغ *sas_cb)
{
	بنية submodule_alternate_تیاری *sas = sas_cb;
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
		strbuf_جمع_کړه(&sb, alt_odb->abspath, len);

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
		submodule_name_منزل_gitdir(&sb, &alternate, sas->submodule_name);
		strbuf_addch(&sb, '/');
		repo_clear(&alternate);

		sm_alternate = compute_alternate_لار(sb.buf, &err);
		إذا (sm_alternate) {
			حرف *p = strbuf_detach(&sb, عدم);

			string_list_append(sas->reference, p)->util = p;
			حرر(sm_alternate);
		} وإلا {
			بدل (sas->error_ډېرپېښ) {
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
	بنية submodule_alternate_تیاری sas = SUBMODULE_ALTERNATE_تیاری_INIT;

	repo_config_get_string(the_repository, "submodule.alternateLocation", &sm_alternate);
	إذا (!sm_alternate)
		أرجع;

	repo_config_get_string(the_repository, "submodule.alternateErrorStrategy", &error_strategy);

	إذا (!error_strategy)
		error_strategy = xstrdup("die");

	sas.submodule_name = sm_name;
	sas.reference = reference;
	إذا (!strcmp(error_strategy, "die"))
		sas.error_ډېرپېښ = SUBMODULE_ALTERNATE_ERROR_DIE;
	وإلا إذا (!strcmp(error_strategy, "info"))
		sas.error_ډېرپېښ = SUBMODULE_ALTERNATE_ERROR_INFO;
	وإلا إذا (!strcmp(error_strategy, "ignore"))
		sas.error_ډېرپېښ = SUBMODULE_ALTERNATE_ERROR_IGNORE;
	وإلا
		die(_("Value '%s' for submodule.alternateErrorStrategy is not recognized"), error_strategy);

	إذا (!strcmp(sm_alternate, "superproject"))
		odb_for_each_alternate(the_repository->objects,
				       جمع_کړه_possible_reference_from_superproject, &sas);
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

	submodule_name_منزل_gitdir(&sb, the_repository, name);
	sm_gitdir = absolute_pathdup(sb.buf);
	strbuf_release(&sb);

	أرجع sm_gitdir;
}

ساكن صحيح dir_لري_only_dotgit(ثابت حرف *لار)
{
	DIR *dir = opendir(لار);
	بنية dirent *e;
	صحيح ret = 1;

	إذا (!dir)
		أرجع 0;

	e = readdir_skip_dot_and_dotdot(dir);
	إذا (!e)
		ret = 0;
	وإلا إذا (strcmp(DEFAULT_GIT_DIR_ENVIRONMENT, e->d_name) ||
		 (e = readdir_skip_dot_and_dotdot(dir))) {
		error("unexpected item '%s' in '%s'", e->d_name, لار);
		ret = 0;
	}

	closedir(dir);
	أرجع ret;
}

ساكن صحيح clone_submodule(ثابت بنية module_clone_معلومات *clone_معلومات,
			   بنية string_list *reference)
{
	حرف *p;
	حرف *sm_gitdir = clone_submodule_sm_gitdir(clone_معلومات->name);
	حرف *sm_alternate = عدم, *error_strategy = عدم;
	بنية stat st;
	بنية ماشوم_process cp = ماشوم_PROCESS_INIT;
	ثابت حرف *clone_معلومات_لار = clone_معلومات->لار;
	حرف *منزل_free = عدم;

	إذا (validate_submodule_لار(clone_معلومات_لار) < 0)
		die(عدم);

	إذا (!is_absolute_لار(clone_معلومات->لار))
		clone_معلومات_لار = منزل_free = xstrfmt("%s/%s", repo_get_work_ونه(the_repository),
						    clone_معلومات->لار);

	إذا (!file_exists(sm_gitdir)) {
		إذا (clone_معلومات->require_init && !stat(clone_معلومات_لار, &st) &&
		    !is_تشه_dir(clone_معلومات_لار))
			die(_("directory not empty: '%s'"), clone_معلومات_لار);

		إذا (خوندي_create_leading_directories_const(the_repository, sm_gitdir) < 0)
			die(_("could not create directory '%s'"), sm_gitdir);

		prepare_possible_alternates(clone_معلومات->name, reference);

		strvec_ټېل(&cp.دلیلونه, "clone");
		strvec_ټېل(&cp.دلیلونه, "--no-checkout");
		إذا (clone_معلومات->quiet)
			strvec_ټېل(&cp.دلیلونه, "--quiet");
		إذا (clone_معلومات->پرمختګ)
			strvec_ټېل(&cp.دلیلونه, "--progress");
		إذا (clone_معلومات->ژوروالی > 0)
			strvec_pushf(&cp.دلیلونه, "--depth=%d", clone_معلومات->ژوروالی);
		إذا (reference->nr) {
			بنية string_list_توکی *توکی;

			for_each_string_list_توکی(توکی, reference)
				strvec_pushl(&cp.دلیلونه, "--reference",
					     توکی->string, عدم);
		}
		إذا (clone_معلومات->ref_storage_format != REF_STORAGE_FORMAT_UNKNOWN)
			strvec_pushf(&cp.دلیلونه, "--ref-storage-format=%s",
				     ref_storage_format_منزل_name(clone_معلومات->ref_storage_format));
		إذا (clone_معلومات->dissociate)
			strvec_ټېل(&cp.دلیلونه, "--dissociate");
		إذا (sm_gitdir && *sm_gitdir)
			strvec_pushl(&cp.دلیلونه, "--separate-git-dir", sm_gitdir, عدم);
		إذا (clone_معلومات->filter_options && clone_معلومات->filter_options->choice)
			strvec_pushf(&cp.دلیلونه, "--filter=%s",
				     expand_list_objects_filter_spec(
					     clone_معلومات->filter_options));
		إذا (clone_معلومات->single_branch >= 0)
			strvec_ټېل(&cp.دلیلونه, clone_معلومات->single_branch ?
				    "--single-branch" :
				    "--no-single-branch");

		strvec_ټېل(&cp.دلیلونه, "--");
		strvec_ټېل(&cp.دلیلونه, clone_معلومات->url);
		strvec_ټېل(&cp.دلیلونه, clone_معلومات_لار);

		cp.git_cmd = 1;
		prepare_submodule_repo_env(&cp.env);
		cp.no_stdin = 1;

		إذا(پرلپسې_command(&cp))
			die(_("clone of '%s' into submodule path '%s' failed"),
			    clone_معلومات->url, clone_معلومات_لار);

		إذا (clone_معلومات->require_init && !stat(clone_معلومات_لار, &st) &&
		    !dir_لري_only_dotgit(clone_معلومات_لار)) {
			حرف *dot_git = xstrfmt("%s/.git", clone_معلومات_لار);
			unlink(dot_git);
			حرر(dot_git);
			die(_("directory not empty: '%s'"), clone_معلومات_لار);
		}
	} وإلا {
		حرف *لار;

		إذا (clone_معلومات->require_init && !stat(clone_معلومات_لار, &st) &&
		    !is_تشه_dir(clone_معلومات_لار))
			die(_("directory not empty: '%s'"), clone_معلومات_لار);
		إذا (خوندي_create_leading_directories_const(the_repository, clone_معلومات_لار) < 0)
			die(_("could not create directory '%s'"), clone_معلومات_لار);
		لار = xstrfmt("%s/index", sm_gitdir);
		unlink_or_warn(لار);
		حرر(لار);
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
	إذا (validate_submodule_git_dir(sm_gitdir, clone_معلومات->name) < 0) {
		حرف *سر = xstrfmt("%s/HEAD", sm_gitdir);
		unlink(سر);
		حرر(سر);
		die(_("refusing to create/use '%s' in another submodule's git dir. "
		      "Enabling extensions.submodulePathConfig should fix this."),
		    sm_gitdir);
	}

	connect_work_ونه_and_git_dir(clone_معلومات_لار, sm_gitdir, 0);

	p = repo_submodule_لار(the_repository, clone_معلومات_لار, "config");
	إذا (!p)
		die(_("could not get submodule directory for '%s'"), clone_معلومات_لار);

	/* setup alternateLocation and alternateErrorStrategy in the cloned submodule if needed */
	repo_config_get_string(the_repository, "submodule.alternateLocation", &sm_alternate);
	إذا (sm_alternate)
		repo_config_سېټ_دننه_file(the_repository, p, "submodule.alternateLocation",
					sm_alternate);
	repo_config_get_string(the_repository, "submodule.alternateErrorStrategy", &error_strategy);
	إذا (error_strategy)
		repo_config_سېټ_دننه_file(the_repository, p, "submodule.alternateErrorStrategy",
					error_strategy);

	حرر(sm_alternate);
	حرر(error_strategy);

	حرر(sm_gitdir);
	حرر(p);
	حرر(منزل_free);
	أرجع 0;
}

ساكن صحيح module_clone(صحيح argc, ثابت حرف **argv, ثابت حرف *مختاړی,
			بنية repository *repo ناکارول_شوی)
{
	صحيح dissociate = 0, quiet = 0, پرمختګ = 0, require_init = 0;
	بنية module_clone_معلومات clone_معلومات = MODULE_CLONE_معلومات_INIT;
	بنية string_list reference = STRING_LIST_INIT_NODUP;
	بنية list_objects_filter_options filter_options =
		LIST_OBJECTS_FILTER_INIT;
	ثابت حرف *ref_storage_format = عدم;

	بنية option module_clone_options[] = {
		OPT_STRING(0, "prefix", &clone_معلومات.مختاړی,
			   N_("path"),
			   N_("alternative anchor for relative paths")),
		OPT_STRING(0, "path", &clone_معلومات.لار,
			   N_("path"),
			   N_("where the new submodule will be cloned to")),
		OPT_STRING(0, "name", &clone_معلومات.name,
			   N_("string"),
			   N_("name of the new submodule")),
		OPT_STRING(0, "url", &clone_معلومات.url,
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
		OPT_INTEGER(0, "depth", &clone_معلومات.ژوروالی,
			   N_("depth for shallow clones")),
		OPT__QUIET(&quiet, "suppress output for cloning a submodule"),
		OPT_BOOL(0, "progress", &پرمختګ,
			   N_("force cloning progress")),
		OPT_BOOL(0, "require-init", &require_init,
			   N_("disallow cloning into non-empty directory")),
		OPT_BOOL(0, "single-branch", &clone_معلومات.single_branch,
			 N_("clone only one branch, HEAD or --branch")),
		OPT_PARSE_LIST_OBJECTS_FILTER(&filter_options),
		OPT_پای_ټکی()
	};
	ثابت حرف *ثابت git_submodule_helper_usage[] = {
		N_("git submodule--helper clone [--prefix=<path>] [--quiet] "
		   "[--reference <repository>] [--name <name>] [--depth <depth>] "
		   "[--single-branch] [--filter <filter-spec>] "
		   "--url <url> --path <path>"),
		عدم
	};

	argc = parse_options(argc, argv, مختاړی, module_clone_options,
			     git_submodule_helper_usage, 0);

	إذا (ref_storage_format) {
		clone_معلومات.ref_storage_format = ref_storage_format_by_name(ref_storage_format);
		إذا (clone_معلومات.ref_storage_format == REF_STORAGE_FORMAT_UNKNOWN)
			die(_("unknown ref storage format '%s'"), ref_storage_format);
	}
	clone_معلومات.dissociate = !!dissociate;
	clone_معلومات.quiet = !!quiet;
	clone_معلومات.پرمختګ = !!پرمختګ;
	clone_معلومات.require_init = !!require_init;
	clone_معلومات.filter_options = &filter_options;

	إذا (argc || !clone_معلومات.url || !clone_معلومات.لار || !*(clone_معلومات.لار))
		usage_with_options(git_submodule_helper_usage,
				   module_clone_options);

	clone_submodule(&clone_معلومات, &reference);
	list_objects_filter_release(&filter_options);
	string_list_clear(&reference, 1);
	أرجع 0;
}

ساكن صحيح determine_submodule_update_strategy(بنية repository *r,
					       صحيح just_cloned,
					       ثابت حرف *لار,
					       تعداد submodule_update_type update,
					       بنية submodule_update_strategy *بهر)
{
	ثابت بنية submodule *sub = submodule_from_لار(r, null_oid(the_hash_algo), لار);
	حرف *کلی;
	ثابت حرف *val;
	صحيح ret;

	/*
	 * NEEDSWORK: audit and ensure that update_submodule() has right
	 * to assume that submodule_from_path() above will always succeed.
	 */
	إذا (!sub)
		BUG("update_submodule assumes a submodule exists at path (%s)",
		    لار);
	کلی = xstrfmt("submodule.%s.update", sub->name);

	إذا (update) {
		بهر->type = update;
	} وإلا إذا (!repo_config_get_string_tmp(r, کلی, &val)) {
		إذا (parse_submodule_update_strategy(val, بهر) < 0) {
			ret = die_پیغام(_("Invalid update mode '%s' configured for submodule path '%s'"),
					  val, لار);
			اذهب cleanup;
		}
	} وإلا إذا (sub->update_strategy.type != SM_UPDATE_UNSPECIFIED) {
		إذا (sub->update_strategy.type == SM_UPDATE_COMMAND)
			BUG("how did we read update = !command from .gitmodules?");
		بهر->type = sub->update_strategy.type;
		بهر->command = sub->update_strategy.command;
	} وإلا
		بهر->type = SM_UPDATE_CHECKOUT;

	إذا (just_cloned &&
	    (بهر->type == SM_UPDATE_یوځای ||
	     بهر->type == SM_UPDATE_REBASE ||
	     بهر->type == SM_UPDATE_NONE))
		بهر->type = SM_UPDATE_CHECKOUT;

	ret = 0;
cleanup:
	حرر(کلی);
	أرجع ret;
}

بنية update_clone_معلومات {
	ثابت بنية submodule *sub;
	بنية object_id oid;
	بینښه just_cloned;
};

بنية submodule_update_clone {
	/* index into 'update_data.list', the list of submodules to look into for cloning */
	صحيح current;

	/* configuration parameters which are passed on to the children */
	ثابت بنية update_معلومات *update_معلومات;

	/* to be consumed by update_submodule() */
	بنية update_clone_معلومات *update_clone;
	صحيح update_clone_nr; صحيح update_clone_alloc;

	/* If we want to stop as fast as possible and return an error */
	بینښه quickstop : 1;

	/* failed clones to be retried again */
	ثابت بنية cache_ثبت **failed_clones;
	صحيح failed_clones_nr, failed_clones_alloc;
};
#تعریف_کړه SUBMODULE_UPDATE_CLONE_INIT { 0 }

ساكن فراغ submodule_update_clone_release(بنية submodule_update_clone *suc)
{
	حرر(suc->update_clone);
	حرر(suc->failed_clones);
}

بنية update_معلومات {
	ثابت حرف *مختاړی;
	ثابت حرف *super_مختاړی;
	حرف *displaypath;
	تعداد submodule_update_type update_default;
	بنية object_id suboid;
	بنية string_list references;
	بنية submodule_update_strategy update_strategy;
	بنية list_objects_filter_options *filter_options;
	بنية module_list list;
	تعداد ref_storage_format ref_storage_format;
	صحيح ژوروالی;
	صحيح max_jobs;
	صحيح single_branch;
	صحيح recommend_shallow;
	بینښه صحيح require_init;
	بینښه صحيح force;
	بینښه صحيح quiet;
	بینښه صحيح nofetch;
	بینښه صحيح remote;
	بینښه صحيح پرمختګ;
	بینښه صحيح dissociate;
	بینښه صحيح init;
	بینښه صحيح warn_if_uninitialized;
	بینښه صحيح recursive;

	/* copied over from update_clone_data */
	بنية object_id oid;
	بینښه صحيح just_cloned;
	ثابت حرف *sm_لار;
};
#تعریف_کړه UPDATE_معلومات_INIT { \
	.update_strategy = SUBMODULE_UPDATE_STRATEGY_INIT, \
	.list = MODULE_LIST_INIT, \
	.ref_storage_format = REF_STORAGE_FORMAT_UNKNOWN, \
	.recommend_shallow = -1, \
	.references = STRING_LIST_INIT_DUP, \
	.single_branch = -1, \
	.max_jobs = 1, \
}

ساكن فراغ update_معلومات_release(بنية update_معلومات *ud)
{
	حرر(ud->displaypath);
	submodule_update_strategy_release(&ud->update_strategy);
	module_list_release(&ud->list);
}

ساكن فراغ بل_submodule_warn_missing(بنية submodule_update_clone *suc,
		بنية strbuf *بهر, ثابت حرف *displaypath)
{
	/*
	 * Only mention uninitialized submodules when their
	 * paths have been specified.
	 */
	إذا (suc->update_معلومات->warn_if_uninitialized) {
		strbuf_addf(بهر,
			_("Submodule path '%s' not initialized"),
			displaypath);
		strbuf_addch(بهر, '\n');
		strbuf_addstr(بهر,
			_("Maybe you want to use 'update --init'?"));
		strbuf_addch(بهر, '\n');
	}
}

/**
 * Determine whether 'ce' needs to be cloned. If so, prepare the 'child' to
 * run the clone. Returns 1 if 'ce' needs to be cloned, 0 otherwise.
 */
ساكن صحيح prepare_منزل_clone_بل_submodule(ثابت بنية cache_ثبت *ce,
					   بنية ماشوم_process *ماشوم,
					   بنية submodule_update_clone *suc,
					   بنية strbuf *بهر)
{
	ثابت بنية submodule *sub = عدم;
	ثابت حرف *url = عدم;
	ثابت حرف *update_string;
	تعداد submodule_update_type update_type;
	حرف *کلی;
	ثابت بنية update_معلومات *ud = suc->update_معلومات;
	حرف *displaypath = get_submodule_displaypath(ce->name, ud->مختاړی,
						      ud->super_مختاړی);
	بنية strbuf sb = STRBUF_INIT;
	صحيح needs_cloning = 0;
	صحيح need_free_url = 0;

	إذا (ce_stage(ce)) {
		strbuf_addf(بهر, _("Skipping unmerged submodule %s"), displaypath);
		strbuf_addch(بهر, '\n');
		اذهب cleanup;
	}

	sub = submodule_from_لار(the_repository, null_oid(the_hash_algo), ce->name);

	إذا (!sub) {
		بل_submodule_warn_missing(suc, بهر, displaypath);
		اذهب cleanup;
	}

	کلی = xstrfmt("submodule.%s.update", sub->name);
	إذا (!repo_config_get_string_tmp(the_repository, کلی, &update_string)) {
		update_type = parse_submodule_update_type(update_string);
	} وإلا {
		update_type = sub->update_strategy.type;
	}
	حرر(کلی);

	إذا (suc->update_معلومات->update_strategy.type == SM_UPDATE_NONE
	    || (suc->update_معلومات->update_strategy.type == SM_UPDATE_UNSPECIFIED
		&& update_type == SM_UPDATE_NONE)) {
		strbuf_addf(بهر, _("Skipping submodule '%s'"), displaypath);
		strbuf_addch(بهر, '\n');
		اذهب cleanup;
	}

	/* Check if the submodule has been initialized. */
	إذا (!is_submodule_فعال(the_repository, ce->name)) {
		بل_submodule_warn_missing(suc, بهر, displaypath);
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

	ماشوم->git_cmd = 1;
	ماشوم->no_stdin = 1;
	ماشوم->stdout_منزل_stderr = 1;
	ماشوم->err = -1;
	strvec_ټېل(&ماشوم->دلیلونه, "submodule--helper");
	strvec_ټېل(&ماشوم->دلیلونه, "clone");
	إذا (suc->update_معلومات->پرمختګ)
		strvec_ټېل(&ماشوم->دلیلونه, "--progress");
	إذا (suc->update_معلومات->quiet)
		strvec_ټېل(&ماشوم->دلیلونه, "--quiet");
	إذا (suc->update_معلومات->مختاړی)
		strvec_pushl(&ماشوم->دلیلونه, "--prefix", suc->update_معلومات->مختاړی, عدم);
	إذا (suc->update_معلومات->recommend_shallow && sub->recommend_shallow == 1)
		strvec_ټېل(&ماشوم->دلیلونه, "--depth=1");
	وإلا إذا (suc->update_معلومات->ژوروالی)
		strvec_pushf(&ماشوم->دلیلونه, "--depth=%d", suc->update_معلومات->ژوروالی);
	إذا (suc->update_معلومات->filter_options && suc->update_معلومات->filter_options->choice)
		strvec_pushf(&ماشوم->دلیلونه, "--filter=%s",
			     expand_list_objects_filter_spec(suc->update_معلومات->filter_options));
	إذا (suc->update_معلومات->require_init)
		strvec_ټېل(&ماشوم->دلیلونه, "--require-init");
	إذا (suc->update_معلومات->ref_storage_format != REF_STORAGE_FORMAT_UNKNOWN)
		strvec_pushf(&ماشوم->دلیلونه, "--ref-storage-format=%s",
			     ref_storage_format_منزل_name(suc->update_معلومات->ref_storage_format));
	strvec_pushl(&ماشوم->دلیلونه, "--path", sub->لار, عدم);
	strvec_pushl(&ماشوم->دلیلونه, "--name", sub->name, عدم);
	strvec_pushl(&ماشوم->دلیلونه, "--url", url, عدم);
	إذا (suc->update_معلومات->references.nr) {
		بنية string_list_توکی *توکی;

		for_each_string_list_توکی(توکی, &suc->update_معلومات->references)
			strvec_pushl(&ماشوم->دلیلونه, "--reference", توکی->string, عدم);
	}
	إذا (suc->update_معلومات->dissociate)
		strvec_ټېل(&ماشوم->دلیلونه, "--dissociate");
	إذا (suc->update_معلومات->single_branch >= 0)
		strvec_ټېل(&ماشوم->دلیلونه, suc->update_معلومات->single_branch ?
					      "--single-branch" :
					      "--no-single-branch");

cleanup:
	حرر(displaypath);
	strbuf_release(&sb);
	إذا (need_free_url)
		حرر((فراغ*)url);

	أرجع needs_cloning;
}

ساكن صحيح update_clone_get_بل_task(بنية ماشوم_process *ماشوم,
				      بنية strbuf *err,
				      فراغ *suc_cb,
				      فراغ **idx_task_cb)
{
	بنية submodule_update_clone *suc = suc_cb;
	ثابت بنية cache_ثبت *ce;
	صحيح شاخص;

	لكل (; suc->current < suc->update_معلومات->list.nr; suc->current++) {
		ce = suc->update_معلومات->list.entries[suc->current];
		إذا (prepare_منزل_clone_بل_submodule(ce, ماشوم, suc, err)) {
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
	شاخص = suc->current - suc->update_معلومات->list.nr;
	إذا (شاخص < suc->failed_clones_nr) {
		صحيح *p;

		ce = suc->failed_clones[شاخص];
		إذا (!prepare_منزل_clone_بل_submodule(ce, ماشوم, suc, err)) {
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

ساكن صحيح update_clone_پیل_failure(بنية strbuf *err ناکارول_شوی,
				      فراغ *suc_cb,
				      فراغ *idx_task_cb ناکارول_شوی)
{
	بنية submodule_update_clone *suc = suc_cb;

	suc->quickstop = 1;
	أرجع 1;
}

ساكن صحيح update_clone_task_پای_ته_رسېدلی(صحيح پایله,
				      بنية strbuf *err,
				      فراغ *suc_cb,
				      فراغ *idx_task_cb)
{
	ثابت بنية cache_ثبت *ce;
	بنية submodule_update_clone *suc = suc_cb;
	صحيح *idxP = idx_task_cb;
	صحيح idx = *idxP;

	حرر(idxP);

	إذا (!پایله)
		أرجع 0;

	إذا (idx < suc->update_معلومات->list.nr) {
		ce  = suc->update_معلومات->list.entries[idx];
		strbuf_addf(err, _("Failed to clone '%s'. Retry scheduled"),
			    ce->name);
		strbuf_addch(err, '\n');
		ALLOC_GROW(suc->failed_clones,
			   suc->failed_clones_nr + 1,
			   suc->failed_clones_alloc);
		suc->failed_clones[suc->failed_clones_nr++] = ce;
		أرجع 0;
	} وإلا {
		idx -= suc->update_معلومات->list.nr;
		ce  = suc->failed_clones[idx];
		strbuf_addf(err, _("Failed to clone '%s' a second time, aborting"),
			    ce->name);
		strbuf_addch(err, '\n');
		suc->quickstop = 1;
		أرجع 1;
	}

	أرجع 0;
}

ساكن صحيح git_update_clone_config(ثابت حرف *var, ثابت حرف *ارزښت,
				   ثابت بنية config_context *ctx,
				   فراغ *cb)
{
	صحيح *max_jobs = cb;

	إذا (!strcmp(var, "submodule.fetchjobs"))
		*max_jobs = parse_submodule_fetchjobs(var, ارزښت, ctx->kvi);
	أرجع 0;
}

ساكن صحيح is_tip_reachable(ثابت حرف *لار, ثابت بنية object_id *oid)
{
	بنية ماشوم_process cp = ماشوم_PROCESS_INIT;
	بنية strbuf rev = STRBUF_INIT;
	حرف *hex = oid_منزل_hex(oid);
	صحيح reachable;

	cp.git_cmd = 1;
	cp.dir = لار;
	cp.no_stderr = 1;
	strvec_pushl(&cp.دلیلونه, "rev-list", "-n", "1", hex, "--not", "--all", عدم);

	prepare_submodule_repo_env(&cp.env);

	إذا (capture_command(&cp, &rev, GIT_MAX_HEXSZ + 1) || rev.len)
		reachable = 0;
	وإلا
		reachable = 1;

	strbuf_release(&rev);
	أرجع reachable;
}

ساكن صحيح fetch_دننه_submodule(ثابت حرف *module_لار, صحيح ژوروالی, صحيح quiet,
			      ثابت بنية object_id *oid)
{
	بنية ماشوم_process cp = ماشوم_PROCESS_INIT;

	prepare_submodule_repo_env(&cp.env);
	cp.git_cmd = 1;
	cp.dir = module_لار;

	strvec_ټېل(&cp.دلیلونه, "fetch");
	إذا (quiet)
		strvec_ټېل(&cp.دلیلونه, "--quiet");
	إذا (ژوروالی)
		strvec_pushf(&cp.دلیلونه, "--depth=%d", ژوروالی);
	إذا (oid) {
		حرف *hex = oid_منزل_hex(oid);
		حرف *remote;
		صحيح کوډ;

		کوډ = get_default_remote_submodule(module_لار, &remote);
		إذا (کوډ) {
			ماشوم_process_clear(&cp);
			أرجع کوډ;
		}

		strvec_pushl(&cp.دلیلونه, remote, hex, عدم);
		حرر(remote);
	}

	أرجع پرلپسې_command(&cp);
}

ساكن صحيح پرلپسې_update_command(ثابت بنية update_معلومات *ud, صحيح subforce)
{
	بنية ماشوم_process cp = ماشوم_PROCESS_INIT;
	حرف *oid = oid_منزل_hex(&ud->oid);
	صحيح ret;

	بدل (ud->update_strategy.type) {
	حالة SM_UPDATE_CHECKOUT:
		cp.git_cmd = 1;
		strvec_pushl(&cp.دلیلونه, "checkout", "-q", عدم);
		إذا (subforce)
			strvec_ټېل(&cp.دلیلونه, "-f");
		اكسر;
	حالة SM_UPDATE_REBASE:
		cp.git_cmd = 1;
		strvec_ټېل(&cp.دلیلونه, "rebase");
		إذا (ud->quiet)
			strvec_ټېل(&cp.دلیلونه, "--quiet");
		اكسر;
	حالة SM_UPDATE_یوځای:
		cp.git_cmd = 1;
		strvec_ټېل(&cp.دلیلونه, "merge");
		إذا (ud->quiet)
			strvec_ټېل(&cp.دلیلونه, "--quiet");
		اكسر;
	حالة SM_UPDATE_COMMAND:
		cp.use_shell = 1;
		strvec_ټېل(&cp.دلیلونه, ud->update_strategy.command);
		اكسر;
	افتراضي:
		BUG("unexpected update strategy type: %d",
		    ud->update_strategy.type);
	}
	strvec_ټېل(&cp.دلیلونه, oid);

	cp.dir = ud->sm_لار;
	prepare_submodule_repo_env(&cp.env);
	إذا ((ret = پرلپسې_command(&cp))) {
		بدل (ud->update_strategy.type) {
		حالة SM_UPDATE_CHECKOUT:
			die_پیغام(_("Unable to checkout '%s' in submodule path '%s'"),
				    oid, ud->displaypath);
			/* No "ret" assignment, use "git checkout"'s */
			اكسر;
		حالة SM_UPDATE_REBASE:
			ret = die_پیغام(_("Unable to rebase '%s' in submodule path '%s'"),
					  oid, ud->displaypath);
			اكسر;
		حالة SM_UPDATE_یوځای:
			ret = die_پیغام(_("Unable to merge '%s' in submodule path '%s'"),
					  oid, ud->displaypath);
			اكسر;
		حالة SM_UPDATE_COMMAND:
			ret = die_پیغام(_("Execution of '%s %s' failed in submodule path '%s'"),
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
		چاپ_کړه(_("Submodule path '%s': checked out '%s'\n"),
		       ud->displaypath, oid);
		اكسر;
	حالة SM_UPDATE_REBASE:
		چاپ_کړه(_("Submodule path '%s': rebased into '%s'\n"),
		       ud->displaypath, oid);
		اكسر;
	حالة SM_UPDATE_یوځای:
		چاپ_کړه(_("Submodule path '%s': merged in '%s'\n"),
		       ud->displaypath, oid);
		اكسر;
	حالة SM_UPDATE_COMMAND:
		چاپ_کړه(_("Submodule path '%s': '%s %s'\n"),
		       ud->displaypath, ud->update_strategy.command, oid);
		اكسر;
	افتراضي:
		BUG("unexpected update strategy type: %d",
		    ud->update_strategy.type);
	}

	أرجع 0;
}

ساكن صحيح پرلپسې_update_procedure(ثابت بنية update_معلومات *ud)
{
	صحيح subforce = is_null_oid(&ud->suboid) || ud->force;

	إذا (!ud->nofetch) {
		/*
		 * Run fetch only if `oid` isn't present or it
		 * is not reachable from a ref.
		 */
		إذا (!is_tip_reachable(ud->sm_لار, &ud->oid) &&
		    fetch_دننه_submodule(ud->sm_لار, ud->ژوروالی, ud->quiet, عدم) &&
		    !ud->quiet)
			fprintf_ln(stderr,
				   _("Unable to fetch in submodule path '%s'; "
				     "trying to directly fetch %s:"),
				   ud->displaypath, oid_منزل_hex(&ud->oid));
		/*
		 * Now we tried the usual fetch, but `oid` may
		 * not be reachable from any of the refs.
		 */
		إذا (!is_tip_reachable(ud->sm_لار, &ud->oid) &&
		    fetch_دننه_submodule(ud->sm_لار, ud->ژوروالی, ud->quiet, &ud->oid))
			أرجع die_پیغام(_("Fetched in submodule path '%s', but it did not "
					     "contain %s. Direct fetching of that commit failed."),
					   ud->displaypath, oid_منزل_hex(&ud->oid));
	}

	أرجع پرلپسې_update_command(ud, subforce);
}

ساكن صحيح remote_submodule_branch(ثابت حرف *لار, ثابت حرف **branch)
{
	ثابت بنية submodule *sub;
	حرف *کلی;
	*branch = عدم;

	sub = submodule_from_لار(the_repository, null_oid(the_hash_algo), لار);
	إذا (!sub)
		أرجع die_پیغام(_("could not initialize submodule at path '%s'"),
				   لار);

	کلی = xstrfmt("submodule.%s.branch", sub->name);
	إذا (repo_config_get_string_tmp(the_repository, کلی, branch))
		*branch = sub->branch;
	حرر(کلی);

	إذا (!*branch) {
		*branch = "HEAD";
		أرجع 0;
	}

	إذا (!strcmp(*branch, ".")) {
		ثابت حرف *refname = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
							      "HEAD", 0, عدم,
							      عدم);

		إذا (!refname)
			أرجع die_پیغام(_("No such ref: %s"), "HEAD");

		/* detached HEAD */
		إذا (!strcmp(refname, "HEAD"))
			أرجع die_پیغام(_("Submodule (%s) branch configured to inherit "
					     "branch from superproject, but the superproject "
					     "is not on any branch"), sub->name);

		إذا (!skip_مختاړی(refname, "refs/heads/", &refname))
			أرجع die_پیغام(_("Expecting a full ref name, got %s"),
					   refname);

		*branch = refname;
		أرجع 0;
	}

	/* Our "branch" is coming from repo_config_get_string_tmp() */
	أرجع 0;
}

ساكن صحيح ensure_core_worktree(ثابت حرف *لار)
{
	ثابت حرف *cw;
	بنية repository subrepo;

	إذا (repo_submodule_init(&subrepo, the_repository, لار, null_oid(the_hash_algo)))
		أرجع die_پیغام(_("could not get a repository handle for submodule '%s'"),
				   لار);

	إذا (!repo_config_get_string_tmp(&subrepo, "core.worktree", &cw)) {
		حرف *cfg_file, *abs_لار;
		ثابت حرف *rel_لار;
		بنية strbuf sb = STRBUF_INIT;

		cfg_file = repo_git_لار(&subrepo, "config");

		abs_لار = absolute_pathdup(لار);
		rel_لار = relative_لار(abs_لار, subrepo.gitdir, &sb);

		repo_config_سېټ_دننه_file(the_repository, cfg_file, "core.worktree", rel_لار);

		حرر(cfg_file);
		حرر(abs_لار);
		strbuf_release(&sb);
	}

	repo_clear(&subrepo);
	أرجع 0;
}

ساكن ثابت حرف *submodule_update_type_منزل_label(تعداد submodule_update_type type)
{
	بدل (type) {
	حالة SM_UPDATE_CHECKOUT:
		أرجع "checkout";
	حالة SM_UPDATE_یوځای:
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

ساكن فراغ update_معلومات_منزل_دلیلونه(ثابت بنية update_معلومات *update_معلومات,
				بنية strvec *دلیلونه)
{
	تعداد submodule_update_type update_type = update_معلومات->update_default;

	strvec_pushl(دلیلونه, "submodule--helper", "update", "--recursive", عدم);
	إذا (update_معلومات->displaypath)
		strvec_pushf(دلیلونه, "--super-prefix=%s/",
			     update_معلومات->displaypath);
	strvec_pushf(دلیلونه, "--jobs=%d", update_معلومات->max_jobs);
	إذا (update_معلومات->quiet)
		strvec_ټېل(دلیلونه, "--quiet");
	إذا (update_معلومات->force)
		strvec_ټېل(دلیلونه, "--force");
	إذا (update_معلومات->init)
		strvec_ټېل(دلیلونه, "--init");
	إذا (update_معلومات->remote)
		strvec_ټېل(دلیلونه, "--remote");
	إذا (update_معلومات->nofetch)
		strvec_ټېل(دلیلونه, "--no-fetch");
	إذا (update_معلومات->dissociate)
		strvec_ټېل(دلیلونه, "--dissociate");
	إذا (update_معلومات->پرمختګ)
		strvec_ټېل(دلیلونه, "--progress");
	إذا (update_معلومات->require_init)
		strvec_ټېل(دلیلونه, "--require-init");
	إذا (update_معلومات->ژوروالی)
		strvec_pushf(دلیلونه, "--depth=%d", update_معلومات->ژوروالی);
	إذا (update_type != SM_UPDATE_UNSPECIFIED)
		strvec_pushf(دلیلونه, "--%s",
			     submodule_update_type_منزل_label(update_type));

	إذا (update_معلومات->references.nr) {
		بنية string_list_توکی *توکی;

		for_each_string_list_توکی(توکی, &update_معلومات->references)
			strvec_pushl(دلیلونه, "--reference", توکی->string, عدم);
	}
	إذا (update_معلومات->ref_storage_format != REF_STORAGE_FORMAT_UNKNOWN)
		strvec_pushf(دلیلونه, "--ref-storage-format=%s",
			     ref_storage_format_منزل_name(update_معلومات->ref_storage_format));
	إذا (update_معلومات->filter_options && update_معلومات->filter_options->choice)
		strvec_pushf(دلیلونه, "--filter=%s",
				expand_list_objects_filter_spec(
					update_معلومات->filter_options));
	إذا (update_معلومات->recommend_shallow == 0)
		strvec_ټېل(دلیلونه, "--no-recommend-shallow");
	وإلا إذا (update_معلومات->recommend_shallow == 1)
		strvec_ټېل(دلیلونه, "--recommend-shallow");
	إذا (update_معلومات->single_branch >= 0)
		strvec_ټېل(دلیلونه, update_معلومات->single_branch ?
				    "--single-branch" :
				    "--no-single-branch");
}

ساكن صحيح update_submodule(بنية update_معلومات *update_معلومات)
{
	صحيح ret;

	إذا (validate_submodule_لار(update_معلومات->sm_لار) < 0)
		أرجع -1;

	ret = determine_submodule_update_strategy(the_repository,
						  update_معلومات->just_cloned,
						  update_معلومات->sm_لار,
						  update_معلومات->update_default,
						  &update_معلومات->update_strategy);
	إذا (ret)
		أرجع ret;

	إذا (update_معلومات->just_cloned)
		oidcpy(&update_معلومات->suboid, null_oid(the_hash_algo));
	وإلا إذا (repo_resolve_gitlink_ref(the_repository, update_معلومات->sm_لار,
					  "HEAD", &update_معلومات->suboid))
		أرجع die_پیغام(_("Unable to find current revision in submodule path '%s'"),
				   update_معلومات->displaypath);

	إذا (update_معلومات->remote) {
		حرف *remote_name;
		ثابت حرف *branch;
		حرف *remote_ref;
		صحيح کوډ;

		کوډ = get_default_remote_submodule(update_معلومات->sm_لار, &remote_name);
		إذا (کوډ)
			أرجع کوډ;
		کوډ = remote_submodule_branch(update_معلومات->sm_لار, &branch);
		إذا (کوډ) {
			حرر(remote_name);
			أرجع کوډ;
		}
		remote_ref = xstrfmt("refs/remotes/%s/%s", remote_name, branch);

		حرر(remote_name);

		إذا (!update_معلومات->nofetch) {
			إذا (fetch_دننه_submodule(update_معلومات->sm_لار, update_معلومات->ژوروالی,
					      0, عدم)) {
				حرر(remote_ref);
				أرجع die_پیغام(_("Unable to fetch in submodule path '%s'"),
						   update_معلومات->sm_لار);
			}
		}

		إذا (repo_resolve_gitlink_ref(the_repository, update_معلومات->sm_لار,
					     remote_ref, &update_معلومات->oid)) {
			ret = die_پیغام(_("Unable to find %s revision in submodule path '%s'"),
					  remote_ref, update_معلومات->sm_لار);
			حرر(remote_ref);
			أرجع ret;
		}

		حرر(remote_ref);
	}

	إذا (!oideq(&update_معلومات->oid, &update_معلومات->suboid) || update_معلومات->force) {
		ret = پرلپسې_update_procedure(update_معلومات);
		إذا (ret)
			أرجع ret;
	}

	إذا (update_معلومات->recursive) {
		بنية ماشوم_process cp = ماشوم_PROCESS_INIT;
		بنية update_معلومات بل = *update_معلومات;

		بل.مختاړی = عدم;
		oidcpy(&بل.oid, null_oid(the_hash_algo));
		oidcpy(&بل.suboid, null_oid(the_hash_algo));

		cp.dir = update_معلومات->sm_لار;
		cp.git_cmd = 1;
		prepare_submodule_repo_env(&cp.env);
		update_معلومات_منزل_دلیلونه(&بل, &cp.دلیلونه);

		ret = پرلپسې_command(&cp);
		إذا (ret)
			die_پیغام(_("Failed to recurse into submodule path '%s'"),
				    update_معلومات->displaypath);
		أرجع ret;
	}

	أرجع 0;
}

ساكن صحيح update_submodules(بنية update_معلومات *update_معلومات)
{
	صحيح i, ret = 0;
	بنية submodule_update_clone suc = SUBMODULE_UPDATE_CLONE_INIT;
	ثابت بنية پرلپسې_process_parallel_opts opts = {
		.tr2_category = "submodule",
		.tr2_label = "parallel/update",

		.processes = update_معلومات->max_jobs,

		.get_بل_task = update_clone_get_بل_task,
		.پیل_failure = update_clone_پیل_failure,
		.task_پای_ته_رسېدلی = update_clone_task_پای_ته_رسېدلی,
		.معلومات = &suc,
	};

	suc.update_معلومات = update_معلومات;
	پرلپسې_processes_parallel(&opts);

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
		بنية update_clone_معلومات ucd = suc.update_clone[i];
		صحيح کوډ = 128;

		oidcpy(&update_معلومات->oid, &ucd.oid);
		update_معلومات->just_cloned = ucd.just_cloned;
		update_معلومات->sm_لار = ucd.sub->لار;

		/*
		 * Verify that the submodule path does not contain any
		 * symlinks; if it does, it might have been tampered with.
		 * TODO: allow exempting it via
		 * `safe.submodule.path` or something
		 */
		إذا (validate_submodule_لار(update_معلومات->sm_لار) < 0)
			اذهب fail;

		کوډ = ensure_core_worktree(update_معلومات->sm_لار);
		إذا (کوډ)
			اذهب fail;

		update_معلومات->displaypath = get_submodule_displaypath(
			update_معلومات->sm_لار, update_معلومات->مختاړی,
			update_معلومات->super_مختاړی);
		کوډ = update_submodule(update_معلومات);
		FREE_AND_NULL(update_معلومات->displaypath);
fail:
		إذا (!کوډ)
			استمر;
		ret = کوډ;
		إذا (ret == 128)
			اذهب cleanup;
	}

cleanup:
	submodule_update_clone_release(&suc);
	string_list_clear(&update_معلومات->references, 0);
	أرجع ret;
}

ساكن صحيح module_update(صحيح argc, ثابت حرف **argv, ثابت حرف *مختاړی,
			 بنية repository *repo ناکارول_شوی)
{
	بنية pathspec pathspec = { 0 };
	بنية pathspec pathspec2 = { 0 };
	بنية update_معلومات opt = UPDATE_معلومات_INIT;
	بنية list_objects_filter_options filter_options =
		LIST_OBJECTS_FILTER_INIT;
	ثابت حرف *ref_storage_format = عدم;
	صحيح ret;
	بنية option module_update_options[] = {
		OPT_SUPER_مختاړی(&opt.super_مختاړی),
		OPT__FORCE(&opt.force, N_("force checkout updates"), 0),
		OPT_BOOL('i', "init", &opt.init,
			 N_("initialize uninitialized submodules before update")),
		OPT_BOOL(0, "remote", &opt.remote,
			 N_("use SHA-1 of submodule's remote tracking branch")),
		OPT_BOOL(0, "recursive", &opt.recursive,
			 N_("traverse submodules recursively")),
		OPT_BOOL('N', "no-fetch", &opt.nofetch,
			 N_("don't fetch new objects from the remote site")),
		OPT_سېټ_INT(0, "checkout", &opt.update_default,
			N_("use the 'checkout' update strategy (default)"),
			SM_UPDATE_CHECKOUT),
		OPT_سېټ_INT('m', "merge", &opt.update_default,
			N_("use the 'merge' update strategy"),
			SM_UPDATE_یوځای),
		OPT_سېټ_INT('r', "rebase", &opt.update_default,
			N_("use the 'rebase' update strategy"),
			SM_UPDATE_REBASE),
		OPT_STRING_LIST(0, "reference", &opt.references, N_("repo"),
			   N_("reference repository")),
		OPT_STRING(0, "ref-storage-format", &ref_storage_format, N_("format"),
			   N_("specify the reference storage format to use")),
		OPT_ALIAS_F(0, "ref-format", "ref-storage-format", PARSE_OPT_HIDDEN),
		OPT_BOOL(0, "dissociate", &opt.dissociate,
			   N_("use --reference only while cloning")),
		OPT_INTEGER(0, "depth", &opt.ژوروالی,
			   N_("create a shallow clone truncated to the "
			      "specified number of revisions")),
		OPT_INTEGER('j', "jobs", &opt.max_jobs,
			    N_("parallel jobs")),
		OPT_BOOL(0, "recommend-shallow", &opt.recommend_shallow,
			    N_("whether the initial clone should follow the shallow recommendation")),
		OPT__QUIET(&opt.quiet, N_("don't print cloning progress")),
		OPT_BOOL(0, "progress", &opt.پرمختګ,
			    N_("force cloning progress")),
		OPT_BOOL(0, "require-init", &opt.require_init,
			   N_("disallow cloning into non-empty directory, implies --init")),
		OPT_BOOL(0, "single-branch", &opt.single_branch,
			 N_("clone only one branch, HEAD or --branch")),
		OPT_PARSE_LIST_OBJECTS_FILTER(&filter_options),
		OPT_پای_ټکی()
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

	argc = parse_options(argc, argv, مختاړی, module_update_options,
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
	opt.مختاړی = مختاړی;

	إذا (opt.update_default)
		opt.update_strategy.type = opt.update_default;

	إذا (module_list_compute(argv, مختاړی, &pathspec, &opt.list) < 0) {
		ret = 1;
		اذهب cleanup;
	}

	إذا (pathspec.nr)
		opt.warn_if_uninitialized = 1;

	إذا (opt.init) {
		بنية module_list list = MODULE_LIST_INIT;
		بنية init_cb info = INIT_CB_INIT;

		إذا (module_list_compute(argv, opt.مختاړی,
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

		info.مختاړی = opt.مختاړی;
		info.super_مختاړی = opt.super_مختاړی;
		إذا (opt.quiet)
			info.flags |= OPT_QUIET;

		for_each_listed_submodule(&list, init_submodule_cb, &info);
		module_list_release(&list);
	}

	ret = update_submodules(&opt);
cleanup:
	update_معلومات_release(&opt);
	list_objects_filter_release(&filter_options);
	clear_pathspec(&pathspec);
	clear_pathspec(&pathspec2);
	أرجع ret;
}

ساكن صحيح ټېل_check(صحيح argc, ثابت حرف **argv, ثابت حرف *مختاړی ناکارول_شوی,
		      بنية repository *repo ناکارول_شوی)
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
		بنية refspec refspec = REFSPEC_INIT_ټېل(the_hash_algo);

		refspec_appendn(&refspec, argv + 2, argc - 2);

		لكل (i = 0; i < refspec.nr; i++) {
			ثابت بنية refspec_توکی *rs = &refspec.توکي[i];

			إذا (rs->نمونه || rs->matching)
				استمر;

			/* LHS must match a single ref */
			بدل (شمېر_refspec_سمون(rs->src, local_refs, عدم)) {
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

ساكن صحيح absorb_git_dirs(صحيح argc, ثابت حرف **argv, ثابت حرف *مختاړی,
			   بنية repository *repo ناکارول_شوی)
{
	صحيح i;
	بنية pathspec pathspec = { 0 };
	بنية module_list list = MODULE_LIST_INIT;
	ثابت حرف *super_مختاړی = عدم;
	بنية option embed_gitdir_options[] = {
		OPT_SUPER_مختاړی(&super_مختاړی),
		OPT_پای_ټکی()
	};
	ثابت حرف *ثابت git_submodule_helper_usage[] = {
		N_("git submodule absorbgitdirs [<options>] [<path>...]"),
		عدم
	};
	صحيح ret = 1;

	argc = parse_options(argc, argv, مختاړی, embed_gitdir_options,
			     git_submodule_helper_usage, 0);

	إذا (module_list_compute(argv, مختاړی, &pathspec, &list) < 0)
		اذهب cleanup;

	لكل (i = 0; i < list.nr; i++)
		absorb_git_dir_into_superproject(list.entries[i]->name,
						 super_مختاړی);

	ret = 0;
cleanup:
	clear_pathspec(&pathspec);
	module_list_release(&list);
	أرجع ret;
}

ساكن صحيح module_سېټ_url(صحيح argc, ثابت حرف **argv, ثابت حرف *مختاړی,
			  بنية repository *repo ناکارول_شوی)
{
	صحيح quiet = 0, ret;
	ثابت حرف *newurl;
	ثابت حرف *لار;
	حرف *config_name;
	بنية option options[] = {
		OPT__QUIET(&quiet, N_("suppress output for setting url of a submodule")),
		OPT_پای_ټکی()
	};
	ثابت حرف *ثابت usage[] = {
		N_("git submodule set-url [--quiet] <path> <newurl>"),
		عدم
	};
	ثابت بنية submodule *sub;

	argc = parse_options(argc, argv, مختاړی, options, usage, 0);

	إذا (argc != 2 || !(لار = argv[0]) || !(newurl = argv[1]))
		usage_with_options(usage, options);

	sub = submodule_from_لار(the_repository, null_oid(the_hash_algo), لار);

	إذا (!sub)
		die(_("no submodule mapping found in .gitmodules for path '%s'"),
		    لار);

	config_name = xstrfmt("submodule.%s.url", sub->name);
	ret = config_سېټ_دننه_gitmodules_file_gently(the_repository, config_name, newurl);

	إذا (!ret) {
		repo_read_gitmodules(the_repository, 0);
		sync_submodule(sub->لار, مختاړی, عدم, quiet ? OPT_QUIET : 0);
	}

	حرر(config_name);
	أرجع !!ret;
}

ساكن صحيح module_سېټ_branch(صحيح argc, ثابت حرف **argv, ثابت حرف *مختاړی,
			     بنية repository *repo ناکارول_شوی)
{
	صحيح opt_default = 0, ret;
	ثابت حرف *opt_branch = عدم;
	ثابت حرف *لار;
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
		OPT_پای_ټکی()
	};
	ثابت حرف *ثابت usage[] = {
		N_("git submodule set-branch [-q|--quiet] (-d|--default) <path>"),
		N_("git submodule set-branch [-q|--quiet] (-b|--branch) <branch> <path>"),
		عدم
	};
	ثابت بنية submodule *sub;

	argc = parse_options(argc, argv, مختاړی, options, usage, 0);

	إذا (!opt_branch && !opt_default)
		die(_("--branch or --default required"));

	إذا (opt_branch && opt_default)
		die(_("options '%s' and '%s' cannot be used together"), "--branch", "--default");

	إذا (argc != 1 || !(لار = argv[0]))
		usage_with_options(usage, options);

	sub = submodule_from_لار(the_repository, null_oid(the_hash_algo), لار);

	إذا (!sub)
		die(_("no submodule mapping found in .gitmodules for path '%s'"),
		    لار);

	config_name = xstrfmt("submodule.%s.branch", sub->name);
	ret = config_سېټ_دننه_gitmodules_file_gently(the_repository, config_name, opt_branch);

	حرر(config_name);
	أرجع !!ret;
}

ساكن صحيح module_create_branch(صحيح argc, ثابت حرف **argv, ثابت حرف *مختاړی,
				بنية repository *repo ناکارول_شوی)
{
	تعداد branch_track track;
	صحيح quiet = 0, force = 0, reflog = 0, dry_پرلپسې = 0;
	بنية option options[] = {
		OPT__QUIET(&quiet, N_("print only error messages")),
		OPT__FORCE(&force, N_("force creation"), 0),
		OPT_BOOL(0, "create-reflog", &reflog,
			 N_("create the branch's reflog")),
		OPT_CALLBACK_F('t', "track",  &track, "(direct|inherit)",
			N_("set branch tracking configuration"),
			PARSE_OPT_OPTARG,
			parse_opt_tracking_ډېرپېښ),
		OPT_DRY_پرلپسې(&dry_پرلپسې,
			     N_("show whether the branch would be created")),
		OPT_پای_ټکی()
	};
	ثابت حرف *ثابت usage[] = {
		N_("git submodule--helper create-branch [-f|--force] [--create-reflog] [-q|--quiet] [-t|--track] [-n|--dry-run] <name> <start-oid> <start-name>"),
		عدم
	};
	بنية repo_config_ارزښتونه *cfg = repo_config_ارزښتونه(the_repository);

	repo_config(the_repository, git_default_config, عدم);
	track = cfg->branch_track;
	argc = parse_options(argc, argv, مختاړی, options, usage, 0);

	إذا (argc != 3)
		usage_with_options(usage, options);

	إذا (!quiet && !dry_پرلپسې)
		printf_ln(_("creating branch '%s'"), argv[0]);

	create_branches_recursively(the_repository, argv[0], argv[1], argv[2],
				    force, reflog, quiet, track, dry_پرلپسې);
	أرجع 0;
}

بنية جمع_کړه_معلومات {
	ثابت حرف *مختاړی;
	ثابت حرف *branch;
	ثابت حرف *reference_لار;
	حرف *sm_لار;
	ثابت حرف *sm_name;
	ثابت حرف *repo;
	ثابت حرف *realrepo;
	تعداد ref_storage_format ref_storage_format;
	صحيح ژوروالی;
	بینښه صحيح force: 1;
	بینښه صحيح quiet: 1;
	بینښه صحيح پرمختګ: 1;
	بینښه صحيح dissociate: 1;
};
#تعریف_کړه جمع_کړه_معلومات_INIT { \
	.ژوروالی = -1, \
	.ref_storage_format = REF_STORAGE_FORMAT_UNKNOWN, \
}

ساكن فراغ append_fetch_remotes(بنية strbuf *پیغام, ثابت حرف *git_dir_لار)
{
	بنية ماشوم_process cp_remote = ماشوم_PROCESS_INIT;
	بنية strbuf sb_remote_بهر = STRBUF_INIT;

	cp_remote.git_cmd = 1;
	strvec_pushf(&cp_remote.env,
		     "GIT_DIR=%s", git_dir_لار);
	strvec_ټېل(&cp_remote.env, "GIT_WORK_TREE=.");
	strvec_pushl(&cp_remote.دلیلونه, "remote", "-v", عدم);
	إذا (!capture_command(&cp_remote, &sb_remote_بهر, 0)) {
		حرف *بل_کرښه;
		حرف *کرښه = sb_remote_بهر.buf;

		طالما ((بل_کرښه = strchr(کرښه, '\n')) != عدم) {
			اندازه_t len = بل_کرښه - کرښه;

			إذا (strip_suffix_mem(کرښه, &len, " (fetch)"))
				strbuf_addf(پیغام, "  %.*s\n", (صحيح)len, کرښه);
			کرښه = بل_کرښه + 1;
		}
	}

	strbuf_release(&sb_remote_بهر);
}

ساكن صحيح جمع_کړه_submodule(ثابت بنية جمع_کړه_معلومات *جمع_کړه_معلومات)
{
	بنية module_clone_معلومات clone_معلومات = MODULE_CLONE_معلومات_INIT;
	بنية string_list reference = STRING_LIST_INIT_NODUP;
	صحيح ret = -1;

	/* perhaps the path already exists and is already a git repo, else clone it */
	إذا (is_directory(جمع_کړه_معلومات->sm_لار)) {
		حرف *submod_gitdir_لار;
		بنية strbuf sm_لار = STRBUF_INIT;
		strbuf_addstr(&sm_لار, جمع_کړه_معلومات->sm_لار);
		submod_gitdir_لار = xstrfmt("%s/.git", جمع_کړه_معلومات->sm_لار);
		إذا (is_nonbare_repository_dir(&sm_لار))
			چاپ_کړه(_("Adding existing repo at '%s' to the index\n"),
			       جمع_کړه_معلومات->sm_لار);
		وإلا
			die(_("'%s' already exists and is not a valid git repo"),
			    جمع_کړه_معلومات->sm_لار);
		strbuf_release(&sm_لار);
		حرر(submod_gitdir_لار);
	} وإلا {
		بنية ماشوم_process cp = ماشوم_PROCESS_INIT;
		بنية strbuf submod_gitdir = STRBUF_INIT;

		submodule_name_منزل_gitdir(&submod_gitdir, the_repository, جمع_کړه_معلومات->sm_name);

		إذا (is_directory(submod_gitdir.buf)) {
			إذا (!جمع_کړه_معلومات->force) {
				بنية strbuf پیغام = STRBUF_INIT;
				حرف *die_پیغام_2;

				strbuf_addf(&پیغام, _("A git directory for '%s' is found "
						    "locally with remote(s):\n"),
					    جمع_کړه_معلومات->sm_name);

				append_fetch_remotes(&پیغام, submod_gitdir.buf);
				strbuf_release(&submod_gitdir);

				strbuf_addf(&پیغام, _("If you want to reuse this local git "
						    "directory instead of cloning again from\n"
						    "  %s\n"
						    "use the '--force' option. If the local git "
						    "directory is not the correct repo\n"
						    "or you are unsure what this means choose "
						    "another name with the '--name' option."),
					    جمع_کړه_معلومات->realrepo);

				die_پیغام_2 = strbuf_detach(&پیغام, عدم);
				die("%s", die_پیغام_2);
			} وإلا {
				چاپ_کړه(_("Reactivating local git directory for "
					 "submodule '%s'\n"), جمع_کړه_معلومات->sm_name);
			}
		}
		strbuf_release(&submod_gitdir);

		clone_معلومات.مختاړی = جمع_کړه_معلومات->مختاړی;
		clone_معلومات.لار = جمع_کړه_معلومات->sm_لار;
		clone_معلومات.name = جمع_کړه_معلومات->sm_name;
		clone_معلومات.url = جمع_کړه_معلومات->realrepo;
		clone_معلومات.quiet = جمع_کړه_معلومات->quiet;
		clone_معلومات.پرمختګ = جمع_کړه_معلومات->پرمختګ;
		إذا (جمع_کړه_معلومات->reference_لار) {
			حرف *p = xstrdup(جمع_کړه_معلومات->reference_لار);

			string_list_append(&reference, p)->util = p;
		}
		clone_معلومات.ref_storage_format = جمع_کړه_معلومات->ref_storage_format;
		clone_معلومات.dissociate = جمع_کړه_معلومات->dissociate;
		إذا (جمع_کړه_معلومات->ژوروالی >= 0)
			clone_معلومات.ژوروالی = جمع_کړه_معلومات->ژوروالی;

		إذا (clone_submodule(&clone_معلومات, &reference))
			اذهب cleanup;

		prepare_submodule_repo_env(&cp.env);
		cp.git_cmd = 1;
		cp.dir = جمع_کړه_معلومات->sm_لار;
		/*
		 * NOTE: we only get here if add_data->force is true, so
		 * passing --force to checkout is reasonable.
		 */
		strvec_pushl(&cp.دلیلونه, "checkout", "-f", "-q", عدم);

		إذا (جمع_کړه_معلومات->branch) {
			strvec_pushl(&cp.دلیلونه, "-B", جمع_کړه_معلومات->branch, عدم);
			strvec_pushf(&cp.دلیلونه, "origin/%s", جمع_کړه_معلومات->branch);
		}

		إذا (پرلپسې_command(&cp))
			die(_("unable to checkout submodule '%s'"), جمع_کړه_معلومات->sm_لار);
	}
	ret = 0;

cleanup:
	string_list_clear(&reference, 1);
	أرجع ret;
}

ساكن صحيح config_submodule_دننه_gitmodules(ثابت حرف *name, ثابت حرف *var, ثابت حرف *ارزښت)
{
	حرف *کلی;
	صحيح ret;

	إذا (!is_writing_gitmodules_ok())
		die(_("please make sure that the .gitmodules file is in the working tree"));

	کلی = xstrfmt("submodule.%s.%s", name, var);
	ret = config_سېټ_دننه_gitmodules_file_gently(the_repository, کلی, ارزښت);
	حرر(کلی);

	أرجع ret;
}

ساكن فراغ configure_added_submodule(بنية جمع_کړه_معلومات *جمع_کړه_معلومات)
{
	حرف *کلی;
	بنية ماشوم_process جمع_کړه_submod = ماشوم_PROCESS_INIT;
	بنية ماشوم_process جمع_کړه_gitmodules = ماشوم_PROCESS_INIT;
	ثابت بنية string_list *ارزښتونه;
	صحيح matched = 0;

	کلی = xstrfmt("submodule.%s.url", جمع_کړه_معلومات->sm_name);
	repo_config_سېټ_gently(the_repository, کلی, جمع_کړه_معلومات->realrepo);
	حرر(کلی);

	جمع_کړه_submod.git_cmd = 1;
	strvec_pushl(&جمع_کړه_submod.دلیلونه, "add",
		     "--no-warn-embedded-repo", عدم);
	إذا (جمع_کړه_معلومات->force)
		strvec_ټېل(&جمع_کړه_submod.دلیلونه, "--force");
	strvec_pushl(&جمع_کړه_submod.دلیلونه, "--", جمع_کړه_معلومات->sm_لار, عدم);

	إذا (پرلپسې_command(&جمع_کړه_submod))
		die(_("Failed to add submodule '%s'"), جمع_کړه_معلومات->sm_لار);

	إذا (config_submodule_دننه_gitmodules(جمع_کړه_معلومات->sm_name, "path", جمع_کړه_معلومات->sm_لار) ||
	    config_submodule_دننه_gitmodules(جمع_کړه_معلومات->sm_name, "url", جمع_کړه_معلومات->repo))
		die(_("Failed to register submodule '%s'"), جمع_کړه_معلومات->sm_لار);

	إذا (جمع_کړه_معلومات->branch) {
		إذا (config_submodule_دننه_gitmodules(جمع_کړه_معلومات->sm_name,
						   "branch", جمع_کړه_معلومات->branch))
			die(_("Failed to register submodule '%s'"), جمع_کړه_معلومات->sm_لار);
	}

	جمع_کړه_gitmodules.git_cmd = 1;
	strvec_pushl(&جمع_کړه_gitmodules.دلیلونه,
		     "add", "--force", "--", ".gitmodules", عدم);

	إذا (پرلپسې_command(&جمع_کړه_gitmodules))
		die(_("Failed to register submodule '%s'"), جمع_کړه_معلومات->sm_لار);

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
	    repo_config_get_string_multi(the_repository, "submodule.active", &ارزښتونه)) {
		/*
		 * If the submodule being added isn't already covered by the
		 * current configured pathspec, set the submodule's active flag
		 */
		کلی = xstrfmt("submodule.%s.active", جمع_کړه_معلومات->sm_name);
		repo_config_سېټ_gently(the_repository, کلی, "true");
		حرر(کلی);
	} وإلا {
		لكل (اندازه_t i = 0; i < ارزښتونه->nr; i++) {
			ثابت حرف *pat = ارزښتونه->توکي[i].string;
			إذا (!wildmatch(pat, جمع_کړه_معلومات->sm_لار, 0)) { /* match found */
				matched = 1;
				اكسر;
			}
		}
		إذا (!matched) { /* no pattern matched -> force-enable */
			کلی = xstrfmt("submodule.%s.active", جمع_کړه_معلومات->sm_name);
			repo_config_سېټ_gently(the_repository, کلی, "true");
			حرر(کلی);
		}
	}
}

ساكن فراغ die_on_شاخص_سمون(ثابت حرف *لار, صحيح force)
{
	بنية pathspec ps;
	ثابت حرف *دلیلونه[] = { لار, عدم };
	parse_pathspec(&ps, 0, PATHSPEC_PREFER_CWD, عدم, دلیلونه);

	إذا (repo_read_شاخص_preload(the_repository, عدم, 0) < 0)
		die(_("index file corrupt"));

	إذا (ps.nr) {
		حرف *ps_matched = xcalloc(ps.nr, 1);

		/* TODO: audit for interaction with sparse-index. */
		ensure_ډک_شاخص(the_repository->شاخص);

		/*
		 * Since there is only one pathspec, we just need to
		 * check ps_matched[0] to know if a cache entry matched.
		 */
		لكل (اندازه_t i = 0; i < the_repository->شاخص->cache_nr; i++) {
			ce_لار_سمون(the_repository->شاخص, the_repository->شاخص->cache[i], &ps,
				      ps_matched);

			إذا (ps_matched[0]) {
				إذا (!force)
					die(_("'%s' already exists in the index"),
					    لار);
				إذا (!S_ISGITLINK(the_repository->شاخص->cache[i]->ce_ډېرپېښ))
					die(_("'%s' already exists in the index "
					      "and is not a submodule"), لار);
				اكسر;
			}
		}
		حرر(ps_matched);
	}
	clear_pathspec(&ps);
}

ساكن فراغ die_on_repo_without_commits(ثابت حرف *لار)
{
	بنية strbuf sb = STRBUF_INIT;
	strbuf_addstr(&sb, لار);
	إذا (is_nonbare_repository_dir(&sb)) {
		بنية object_id oid;
		إذا (repo_resolve_gitlink_ref(the_repository, لار, "HEAD", &oid) < 0)
			die(_("'%s' does not have a commit checked out"), لار);
	}
	strbuf_release(&sb);
}

ساكن صحيح module_جمع_کړه(صحيح argc, ثابت حرف **argv, ثابت حرف *مختاړی,
		      بنية repository *repo ناکارول_شوی)
{
	صحيح force = 0, quiet = 0, پرمختګ = 0, dissociate = 0;
	بنية جمع_کړه_معلومات جمع_کړه_معلومات = جمع_کړه_معلومات_INIT;
	ثابت حرف *ref_storage_format = عدم;
	حرف *منزل_free = عدم;
	ثابت بنية submodule *existing;
	بنية strbuf buf = STRBUF_INIT;
	حرف *sm_name_منزل_free = عدم;
	بنية option options[] = {
		OPT_STRING('b', "branch", &جمع_کړه_معلومات.branch, N_("branch"),
			   N_("branch of repository to add as submodule")),
		OPT__FORCE(&force, N_("allow adding an otherwise ignored submodule path"),
			   PARSE_OPT_NOCOMPLETE),
		OPT__QUIET(&quiet, N_("print only error messages")),
		OPT_BOOL(0, "progress", &پرمختګ, N_("force cloning progress")),
		OPT_STRING(0, "reference", &جمع_کړه_معلومات.reference_لار, N_("repository"),
			   N_("reference repository")),
		OPT_STRING(0, "ref-storage-format", &ref_storage_format, N_("format"),
			   N_("specify the reference storage format to use")),
		OPT_ALIAS_F(0, "ref-format", "ref-storage-format", PARSE_OPT_HIDDEN),
		OPT_BOOL(0, "dissociate", &dissociate, N_("borrow the objects from reference repositories")),
		OPT_STRING(0, "name", &جمع_کړه_معلومات.sm_name, N_("name"),
			   N_("sets the submodule's name to the given string "
			      "instead of defaulting to its path")),
		OPT_INTEGER(0, "depth", &جمع_کړه_معلومات.ژوروالی, N_("depth for shallow clones")),
		OPT_پای_ټکی()
	};
	ثابت حرف *ثابت usage[] = {
		N_("git submodule add [<options>] [--] <repository> [<path>]"),
		عدم
	};
	بنية strbuf sb = STRBUF_INIT;
	صحيح ret = 1;

	argc = parse_options(argc, argv, مختاړی, options, usage, 0);

	إذا (!is_writing_gitmodules_ok())
		die(_("please make sure that the .gitmodules file is in the working tree"));

	إذا (مختاړی && *مختاړی &&
	    جمع_کړه_معلومات.reference_لار && !is_absolute_لار(جمع_کړه_معلومات.reference_لار))
		جمع_کړه_معلومات.reference_لار = xstrfmt("%s%s", مختاړی, جمع_کړه_معلومات.reference_لار);

	إذا (argc == 0 || argc > 2)
		usage_with_options(usage, options);

	إذا (ref_storage_format) {
		جمع_کړه_معلومات.ref_storage_format = ref_storage_format_by_name(ref_storage_format);
		إذا (جمع_کړه_معلومات.ref_storage_format == REF_STORAGE_FORMAT_UNKNOWN)
			die(_("unknown ref storage format '%s'"), ref_storage_format);
	}

	جمع_کړه_معلومات.repo = argv[0];
	إذا (argc == 1)
		جمع_کړه_معلومات.sm_لار = git_url_basename(جمع_کړه_معلومات.repo, 0, 0);
	وإلا
		جمع_کړه_معلومات.sm_لار = xstrdup(argv[1]);

	إذا (مختاړی && *مختاړی && !is_absolute_لار(جمع_کړه_معلومات.sm_لار)) {
		حرف *sm_لار = جمع_کړه_معلومات.sm_لار;

		جمع_کړه_معلومات.sm_لار = xstrfmt("%s%s", مختاړی, sm_لار);
		حرر(sm_لار);
	}

	إذا (starts_with_dot_dot_slash(جمع_کړه_معلومات.repo) ||
	    starts_with_dot_slash(جمع_کړه_معلومات.repo)) {
		إذا (مختاړی)
			die(_("Relative path can only be used from the toplevel "
			      "of the working tree"));

		/* dereference source url relative to parent's url */
		منزل_free = resolve_relative_url(جمع_کړه_معلومات.repo, عدم, 1);
		جمع_کړه_معلومات.realrepo = منزل_free;
	} وإلا إذا (is_dir_sep(جمع_کړه_معلومات.repo[0]) || strchr(جمع_کړه_معلومات.repo, ':')) {
		جمع_کړه_معلومات.realrepo = جمع_کړه_معلومات.repo;
	} وإلا {
		die(_("repo URL: '%s' must be absolute or begin with ./|../"),
		    جمع_کړه_معلومات.repo);
	}

	/*
	 * normalize path:
	 * multiple //; leading ./; /./; /../;
	 */
	normalize_لار_copy(جمع_کړه_معلومات.sm_لار, جمع_کړه_معلومات.sm_لار);
	strip_dir_trailing_slashes(جمع_کړه_معلومات.sm_لار);

	إذا (validate_submodule_لار(جمع_کړه_معلومات.sm_لار) < 0)
		die(عدم);

	die_on_شاخص_سمون(جمع_کړه_معلومات.sm_لار, force);
	die_on_repo_without_commits(جمع_کړه_معلومات.sm_لار);

	إذا (!force) {
		بنية ماشوم_process cp = ماشوم_PROCESS_INIT;

		cp.git_cmd = 1;
		cp.no_stdout = 1;
		strvec_pushl(&cp.دلیلونه, "add", "--dry-run", "--ignore-missing",
			     "--no-warn-embedded-repo", جمع_کړه_معلومات.sm_لار, عدم);
		إذا ((ret = pipe_command(&cp, عدم, 0, عدم, 0, &sb, 0))) {
			strbuf_complete_کرښه(&sb);
			fputs(sb.buf, stderr);
			اذهب cleanup;
		}
	}

	إذا (!جمع_کړه_معلومات.sm_name)
		جمع_کړه_معلومات.sm_name = جمع_کړه_معلومات.sm_لار;

	existing = submodule_from_name(the_repository,
					null_oid(the_hash_algo),
					جمع_کړه_معلومات.sm_name);

	إذا (existing && existing->لار &&
	    strcmp(existing->لار, جمع_کړه_معلومات.sm_لار)) {
		إذا (!force) {
			die(_("submodule name '%s' already used for path '%s'"),
			    جمع_کړه_معلومات.sm_name, existing->لار);
		}
		/* --force: build <name><n> until unique */
		لكل (صحيح i = 1; ; i++) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "%s%d", جمع_کړه_معلومات.sm_name, i);
			إذا (!submodule_from_name(the_repository,
						null_oid(the_hash_algo),
						buf.buf)) {
				اكسر;
			}
		}
		جمع_کړه_معلومات.sm_name = sm_name_منزل_free = strbuf_detach(&buf, عدم);
	}

	إذا (check_submodule_name(جمع_کړه_معلومات.sm_name))
		die(_("'%s' is not a valid submodule name"), جمع_کړه_معلومات.sm_name);

	جمع_کړه_معلومات.مختاړی = مختاړی;
	جمع_کړه_معلومات.force = !!force;
	جمع_کړه_معلومات.quiet = !!quiet;
	جمع_کړه_معلومات.پرمختګ = !!پرمختګ;
	جمع_کړه_معلومات.dissociate = !!dissociate;

	إذا (the_repository->repository_format_submodule_لار_cfg)
		create_default_gitdir_config(جمع_کړه_معلومات.sm_name);

	إذا (جمع_کړه_submodule(&جمع_کړه_معلومات))
		اذهب cleanup;
	configure_added_submodule(&جمع_کړه_معلومات);

	ret = 0;
cleanup:
	حرر(sm_name_منزل_free);
	حرر(جمع_کړه_معلومات.sm_لار);
	حرر(منزل_free);
	strbuf_release(&sb);

	أرجع ret;
}

صحيح cmd_submodule__helper(صحيح argc,
			  ثابت حرف **argv,
			  ثابت حرف *مختاړی,
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
		OPT_SUBCOMMAND("add", &fn, module_جمع_کړه),
		OPT_SUBCOMMAND("update", &fn, module_update),
		OPT_SUBCOMMAND("foreach", &fn, module_foreach),
		OPT_SUBCOMMAND("init", &fn, module_init),
		OPT_SUBCOMMAND("status", &fn, module_حالت),
		OPT_SUBCOMMAND("sync", &fn, module_sync),
		OPT_SUBCOMMAND("deinit", &fn, module_deinit),
		OPT_SUBCOMMAND("summary", &fn, module_summary),
		OPT_SUBCOMMAND("push-check", &fn, ټېل_check),
		OPT_SUBCOMMAND("absorbgitdirs", &fn, absorb_git_dirs),
		OPT_SUBCOMMAND("set-url", &fn, module_سېټ_url),
		OPT_SUBCOMMAND("set-branch", &fn, module_سېټ_branch),
		OPT_SUBCOMMAND("create-branch", &fn, module_create_branch),
		OPT_SUBCOMMAND("get-default-remote", &fn, module_get_default_remote),
		OPT_پای_ټکی()
	};
	argc = parse_options(argc, argv, مختاړی, options, usage, 0);

	أرجع fn(argc, argv, مختاړی, repo);
}
