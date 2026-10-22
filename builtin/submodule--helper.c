#تعریف_کرو USE_THE_REPOSITORY_VARIABLE

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

#تعریف_کرو OPT_QUIET (1 << 0)
#تعریف_کرو OPT_CACHED (1 << 1)
#تعریف_کرو OPT_RECURSIVE (1 << 2)
#تعریف_کرو OPT_FORCE (1 << 3)

قسمتعریف خالی (*each_submodule_fn)(ثابت ساخت cache_اندراج *list_شے,
				  خالی *cb_ڈیٹا);

ساکن حرف *get_default_remote(خالی)
{
	واپس xstrdup(repo_default_remote(the_repository));
}

ساکن حرف *resolve_relative_url(ثابت حرف *rel_url, ثابت حرف *اوپر_راستہ, صحیح quiet)
{
	حرف *remoteurl, *resolved_url;
	حرف *remote = get_default_remote();
	ساخت strbuf remotesb = STRBUF_INIT;

	strbuf_addf(&remotesb, "remote.%s.url", remote);
	اگر (repo_config_get_string(the_repository, remotesb.buf, &remoteurl)) {
		اگر (!quiet)
			warning(_("could not look up configuration '%s'. "
				  "Assuming this repository is its own "
				  "authoritative upstream."),
				remotesb.buf);
		remoteurl = xgetcwd();
	}
	resolved_url = relative_url(remoteurl, rel_url, اوپر_راستہ);

	آزاد(remote);
	آزاد(remoteurl);
	strbuf_release(&remotesb);

	واپس resolved_url;
}

ساکن صحیح get_default_remote_submodule(ثابت حرف *module_راستہ, حرف **default_remote)
{
	ثابت ساخت submodule *sub;
	ساخت repository subrepo;
	ثابت حرف *remote_name = تہی;
	حرف *url = تہی;
	صحیح ret = 0;

	sub = submodule_from_راستہ(the_repository, null_oid(the_hash_algo), module_راستہ);
	اگر (sub && sub->url) {
		url = xstrdup(sub->url);

		/* Possibly a url relative to parent */
		اگر (starts_with_dot_dot_slash(url) ||
		    starts_with_dot_slash(url)) {
			حرف *oldurl = url;

			url = resolve_relative_url(oldurl, تہی, 1);
			آزاد(oldurl);
		}
	}

	اگر (repo_submodule_init(&subrepo, the_repository, module_راستہ,
				null_oid(the_hash_algo)) < 0) {
		ret = die_پیغام(_("could not get a repository handle for submodule '%s'"),
				   module_راستہ);
		جاؤ برآمد;
	}

	/* Look up by URL first */
	اگر (url)
		remote_name = repo_remote_from_url(&subrepo, url);
	اگر (!remote_name)
		remote_name = repo_default_remote(&subrepo);

	*default_remote = xstrdup(remote_name);

برآمد:
	repo_clear(&subrepo);
	آزاد(url);

	واپس ret;
}

ساکن صحیح module_get_default_remote(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
				     ساخت repository *repo غیر_مستعمل)
{
	ثابت حرف *راستہ;
	حرف *resolved_راستہ = تہی;
	حرف *default_remote = تہی;
	صحیح رمز;
	ساخت option options[] = {
		OPT_انتہا()
	};
	ثابت حرف *ثابت usage[] = {
		N_("git submodule--helper get-default-remote <path>"),
		تہی
	};

	argc = parse_options(argc, argv, سابقہ, options, usage, 0);
	اگر (argc != 1)
		usage_with_options(usage, options);

	راستہ = argv[0];
	اگر (سابقہ && *سابقہ && !is_absolute_راستہ(راستہ)) {
		resolved_راستہ = xstrfmt("%s%s", سابقہ, راستہ);
		راستہ = resolved_راستہ;
	}

	رمز = get_default_remote_submodule(راستہ, &default_remote);
	اگر (رمز) {
		آزاد(resolved_راستہ);
		واپس رمز;
	}

	لکھو("%s\n", default_remote);
	آزاد(default_remote);
	آزاد(resolved_راستہ);
	واپس 0;
}

/* the result should be freed by the caller. */
ساکن حرف *get_submodule_displaypath(ثابت حرف *راستہ, ثابت حرف *سابقہ,
				       ثابت حرف *super_سابقہ)
{
	اگر (سابقہ && super_سابقہ) {
		BUG("cannot have prefix '%s' and superprefix '%s'",
		    سابقہ, super_سابقہ);
	} ورنہ اگر (سابقہ) {
		ساخت strbuf sb = STRBUF_INIT;
		حرف *displaypath = xstrdup(relative_راستہ(راستہ, سابقہ, &sb));
		strbuf_release(&sb);
		واپس displaypath;
	} ورنہ اگر (super_سابقہ) {
		واپس xstrfmt("%s%s", super_سابقہ, راستہ);
	} ورنہ {
		واپس xstrdup(راستہ);
	}
}

ساکن حرف *compute_rev_name(ثابت حرف *sub_راستہ, ثابت حرف* object_id)
{
	ساخت strbuf sb = STRBUF_INIT;
	ثابت حرف ***d;

	ساکن ثابت حرف *describe_bare[] = { تہی };

	ساکن ثابت حرف *describe_لیبل_فہرست[] = { "--tags", تہی };

	ساکن ثابت حرف *describe_شامل_ہے[] = { "--contains", تہی };

	ساکن ثابت حرف *describe_all_always[] = { "--all", "--always", تہی };

	ساکن ثابت حرف **describe_argv[] = { describe_bare, describe_لیبل_فہرست,
						describe_شامل_ہے,
						describe_all_always, تہی };

	برائے (d = describe_argv; *d; d++) {
		ساخت بچہ_process cp = بچہ_PROCESS_INIT;
		prepare_submodule_repo_env(&cp.env);
		cp.dir = sub_راستہ;
		cp.git_cmd = 1;
		cp.no_stderr = 1;

		strvec_ڈالو(&cp.دلائل, "describe");
		strvec_pushv(&cp.دلائل, *d);
		strvec_ڈالو(&cp.دلائل, object_id);

		اگر (!capture_command(&cp, &sb, 0)) {
			strbuf_strip_suffix(&sb, "\n");
			واپس strbuf_detach(&sb, تہی);
		}
	}

	strbuf_release(&sb);
	واپس تہی;
}

ساخت module_list {
	ثابت ساخت cache_اندراج **entries;
	صحیح alloc, nr;
};
#تعریف_کرو MODULE_LIST_INIT { 0 }

ساکن خالی module_list_release(ساخت module_list *ml)
{
	آزاد(ml->entries);
}

ساکن صحیح module_list_compute(ثابت حرف **argv,
			       ثابت حرف *سابقہ,
			       ساخت pathspec *pathspec,
			       ساخت module_list *list)
{
	صحیح نتیجہ = 0;
	حرف *ps_matched = تہی;

	parse_pathspec(pathspec, 0,
		       PATHSPEC_PREFER_بھرا,
		       سابقہ, argv);

	اگر (pathspec->nr)
		ps_matched = xcalloc(pathspec->nr, 1);

	اگر (repo_read_اشاریہ(the_repository) < 0)
		die(_("index file corrupt"));

	برائے (سائز_t i = 0; i < the_repository->اشاریہ->cache_nr; i++) {
		ثابت ساخت cache_اندراج *ce = the_repository->اشاریہ->cache[i];

		اگر (!مطابقت_pathspec(the_repository->اشاریہ, pathspec, ce->name, ce_namelen(ce),
				    0, ps_matched, 1) ||
		    !S_ISGITLINK(ce->ce_منوال))
			جاری;

		ALLOC_GROW(list->entries, list->nr + 1, list->alloc);
		list->entries[list->nr++] = ce;
		جب (i + 1 < the_repository->اشاریہ->cache_nr &&
		       !strcmp(ce->name, the_repository->اشاریہ->cache[i + 1]->name))
			/*
			 * Skip entries with the same name in different stages
			 * to make sure an entry is returned only once.
			 */
			i++;
	}

	اگر (ps_matched && report_راستہ_error(ps_matched, pathspec))
		نتیجہ = -1;

	آزاد(ps_matched);

	واپس نتیجہ;
}

ساکن خالی module_list_فعال(ساخت module_list *list)
{
	صحیح i;
	ساخت module_list فعال_modules = MODULE_LIST_INIT;

	برائے (i = 0; i < list->nr; i++) {
		ثابت ساخت cache_اندراج *ce = list->entries[i];

		اگر (!is_submodule_فعال(the_repository, ce->name))
			جاری;

		ALLOC_GROW(فعال_modules.entries,
			   فعال_modules.nr + 1,
			   فعال_modules.alloc);
		فعال_modules.entries[فعال_modules.nr++] = ce;
	}

	module_list_release(list);
	*list = فعال_modules;
}

ساکن حرف *get_اوپر_راستہ(ثابت حرف *راستہ)
{
	ساخت strbuf sb = STRBUF_INIT;

	strbuf_addstrings(&sb, "../", گنتی_slashes(راستہ));

	/*
	 * Check if 'path' ends with slash or not
	 * for having the same output for dir/sub_dir
	 * and dir/sub_dir/
	 */
	اگر (!is_dir_sep(راستہ[لڑی_لمبائی(راستہ) - 1]))
		strbuf_addstr(&sb, "../");

	واپس strbuf_detach(&sb, تہی);
}

ساکن خالی for_each_listed_submodule(ثابت ساخت module_list *list,
				      each_submodule_fn fn, خالی *cb_ڈیٹا)
{
	صحیح i;

	برائے (i = 0; i < list->nr; i++)
		fn(list->entries[i], cb_ڈیٹا);
}

ساخت foreach_cb {
	صحیح argc;
	ثابت حرف **argv;
	ثابت حرف *سابقہ;
	ثابت حرف *super_سابقہ;
	صحیح quiet;
	صحیح recursive;
};
#تعریف_کرو FOREACH_CB_INIT { 0 }

ساکن خالی runcommand_اندر_submodule_cb(ثابت ساخت cache_اندراج *list_شے,
				       خالی *cb_ڈیٹا)
{
	ساخت foreach_cb *info = cb_ڈیٹا;
	ثابت حرف *راستہ = list_شے->name;
	ثابت ساخت object_id *ce_oid = &list_شے->oid;
	ثابت ساخت submodule *sub;
	ساخت بچہ_process cp = بچہ_PROCESS_INIT;
	حرف *displaypath;

	اگر (validate_submodule_راستہ(راستہ) < 0)
		die(تہی);

	displaypath = get_submodule_displaypath(راستہ, info->سابقہ,
						info->super_سابقہ);

	sub = submodule_from_راستہ(the_repository, null_oid(the_hash_algo), راستہ);

	اگر (!sub)
		die(_("No url found for submodule path '%s' in .gitmodules"),
			displaypath);

	اگر (!is_submodule_populated_gently(راستہ, تہی))
		جاؤ cleanup;

	prepare_submodule_repo_env(&cp.env);

	/*
	 * For the purpose of executing <command> in the submodule,
	 * separate shell is used for the purpose of running the
	 * child process.
	 */
	cp.use_shell = 1;
	cp.dir = راستہ;

	/*
	 * NEEDSWORK: the command currently has access to the variables $name,
	 * $sm_path, $displaypath, $sha1 and $toplevel only when the command
	 * contains a single argument. This is done for maintaining a faithful
	 * translation from shell script.
	 */
	اگر (info->argc == 1) {
		حرف *toplevel = xgetcwd();
		ساخت strbuf sb = STRBUF_INIT;

		strvec_pushf(&cp.env, "name=%s", sub->name);
		strvec_pushf(&cp.env, "sm_path=%s", راستہ);
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
		sq_quote_buf(&sb, راستہ);
		strvec_pushf(&cp.دلائل, "path=%s; %s",
			     sb.buf, info->argv[0]);
		strbuf_release(&sb);
		آزاد(toplevel);
	} ورنہ {
		strvec_pushv(&cp.دلائل, info->argv);
	}

	اگر (!info->quiet)
		لکھو(_("Entering '%s'\n"), displaypath);

	اگر (info->argv[0]) {
		اگر (تسلسل_command(&cp))
			die(_("run_command returned non-zero status for %s\n."),
			    displaypath);
	} ورنہ {
		بچہ_process_clear(&cp);
	}

	اگر (info->recursive) {
		ساخت بچہ_process cpr = بچہ_PROCESS_INIT;

		cpr.git_cmd = 1;
		cpr.dir = راستہ;
		prepare_submodule_repo_env(&cpr.env);

		strvec_pushl(&cpr.دلائل, "submodule--helper", "foreach", "--recursive",
			     تہی);
		strvec_pushf(&cpr.دلائل, "--super-prefix=%s/", displaypath);

		اگر (info->quiet)
			strvec_ڈالو(&cpr.دلائل, "--quiet");

		strvec_ڈالو(&cpr.دلائل, "--");
		strvec_pushv(&cpr.دلائل, info->argv);

		اگر (تسلسل_command(&cpr))
			die(_("run_command returned non-zero status while "
				"recursing in the nested submodules of %s\n."),
				displaypath);
	}

cleanup:
	آزاد(displaypath);
}

ساکن صحیح module_foreach(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
			  ساخت repository *repo غیر_مستعمل)
{
	ساخت foreach_cb info = FOREACH_CB_INIT;
	ساخت pathspec pathspec = { 0 };
	ساخت module_list list = MODULE_LIST_INIT;
	ساخت option module_foreach_options[] = {
		OPT_SUPER_سابقہ(&info.super_سابقہ),
		OPT__QUIET(&info.quiet, N_("suppress output of entering each submodule command")),
		OPT_BOOL(0, "recursive", &info.recursive,
			 N_("recurse into nested submodules")),
		OPT_انتہا()
	};
	ثابت حرف *ثابت git_submodule_helper_usage[] = {
		N_("git submodule foreach [--quiet] [--recursive] [--] <command>"),
		تہی
	};
	صحیح ret = 1;

	argc = parse_options(argc, argv, سابقہ, module_foreach_options,
			     git_submodule_helper_usage, 0);

	اگر (module_list_compute(تہی, سابقہ, &pathspec, &list) < 0)
		جاؤ cleanup;

	info.argc = argc;
	info.argv = argv;
	info.سابقہ = سابقہ;

	for_each_listed_submodule(&list, runcommand_اندر_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	واپس ret;
}

ساخت init_cb {
	ثابت حرف *سابقہ;
	ثابت حرف *super_سابقہ;
	غیرعلامتی صحیح flags;
};
#تعریف_کرو INIT_CB_INIT { 0 }

ساکن صحیح validate_and_سیٹ_submodule_gitdir(ساخت strbuf *gitdir_راستہ,
					     ثابت حرف *submodule_name)
{
	ثابت حرف *قدر;
	حرف *کلید;

	اگر (validate_submodule_git_dir(gitdir_راستہ->buf, submodule_name))
		واپس -1;

	کلید = xstrfmt("submodule.%s.gitdir", submodule_name);

	/* Nothing to do if the config already exists. */
	اگر (!repo_config_get_string_tmp(the_repository, کلید, &قدر)) {
		آزاد(کلید);
		واپس 0;
	}

	اگر (repo_config_سیٹ_gently(the_repository, کلید, gitdir_راستہ->buf)) {
		آزاد(کلید);
		واپس -1;
	}

	آزاد(کلید);
	واپس 0;
}

ساکن خالی create_default_gitdir_config(ثابت حرف *submodule_name)
{
	ساخت strbuf gitdir_راستہ = STRBUF_INIT;
	ساخت git_hash_ctx ctx;
	حرف hex_name_hash[GIT_MAX_HEXSZ + 1], سرخی[128];
	غیرعلامتی حرف raw_name_hash[GIT_MAX_RAWSZ];
	صحیح سرخی_len;

	/* Case 1: try the plain module name */
	repo_git_راستہ_append(the_repository, &gitdir_راستہ, "modules/%s", submodule_name);
	اگر (!validate_and_سیٹ_submodule_gitdir(&gitdir_راستہ, submodule_name)) {
		strbuf_release(&gitdir_راستہ);
		واپس;
	}

	/* Case 2.1: Try URI-safe (RFC3986) encoding first, this fixes nested gitdirs */
	strbuf_reset(&gitdir_راستہ);
	repo_git_راستہ_append(the_repository, &gitdir_راستہ, "modules/");
	strbuf_addstr_urlencode(&gitdir_راستہ, submodule_name, is_rfc3986_unreserved);
	اگر (!validate_and_سیٹ_submodule_gitdir(&gitdir_راستہ, submodule_name)) {
		strbuf_release(&gitdir_راستہ);
		واپس;
	}

	/* Case 2.2: Try extended uppercase URI (RFC3986) encoding, to fix case-folding */
	strbuf_reset(&gitdir_راستہ);
	repo_git_راستہ_append(the_repository, &gitdir_راستہ, "modules/");
	strbuf_addstr_urlencode(&gitdir_راستہ, submodule_name, is_casefolding_rfc3986_unreserved);
	اگر (!validate_and_سیٹ_submodule_gitdir(&gitdir_راستہ, submodule_name))
		واپس;

	/* Case 2.3: Try some derived gitdir names, see if one sticks */
	برائے (حرف c = '0'; c <= '9'; c++) {
		strbuf_reset(&gitdir_راستہ);
		repo_git_راستہ_append(the_repository, &gitdir_راستہ, "modules/");
		strbuf_addstr_urlencode(&gitdir_راستہ, submodule_name, is_rfc3986_unreserved);
		strbuf_addch(&gitdir_راستہ, c);
		اگر (!validate_and_سیٹ_submodule_gitdir(&gitdir_راستہ, submodule_name))
			واپس;

		strbuf_reset(&gitdir_راستہ);
		repo_git_راستہ_append(the_repository, &gitdir_راستہ, "modules/");
		strbuf_addstr_urlencode(&gitdir_راستہ, submodule_name, is_casefolding_rfc3986_unreserved);
		strbuf_addch(&gitdir_راستہ, c);
		اگر (!validate_and_سیٹ_submodule_gitdir(&gitdir_راستہ, submodule_name))
			واپس;
	}

	/* Case 2.4: If all the above failed, try a hash of the name as a last resort */
	سرخی_len = snprintf(سرخی, حجم(سرخی),
			      "blob %"PRIuMAX, (uintmax_t)لڑی_لمبائی(submodule_name));
	git_hash_init(&ctx, the_hash_algo);
	git_hash_update(&ctx, سرخی, سرخی_len);
	git_hash_update(&ctx, "\0", 1);
	git_hash_update(&ctx, submodule_name, لڑی_لمبائی(submodule_name));
	git_hash_final(raw_name_hash, &ctx);
	hash_مقصد_hex_algop_r(hex_name_hash, raw_name_hash, the_hash_algo);
	strbuf_reset(&gitdir_راستہ);
	repo_git_راستہ_append(the_repository, &gitdir_راستہ, "modules/%s", hex_name_hash);
	اگر (!validate_and_سیٹ_submodule_gitdir(&gitdir_راستہ, submodule_name)) {
		strbuf_release(&gitdir_راستہ);
		واپس;
	}

	/* Case 3: nothing worked, error out */
	die(_("failed to set a valid default config for 'submodule.%s.gitdir'. "
	      "Please ensure it is set, for example by running something like: "
	      "'git config submodule.%s.gitdir .git/modules/%s'"),
	    submodule_name, submodule_name, submodule_name);
}

ساکن خالی init_submodule(ثابت حرف *راستہ, ثابت حرف *سابقہ,
			   ثابت حرف *super_سابقہ,
			   غیرعلامتی صحیح flags)
{
	ثابت ساخت submodule *sub;
	ساخت strbuf sb = STRBUF_INIT;
	ثابت حرف *upd;
	حرف *url = تہی, *displaypath;

	displaypath = get_submodule_displaypath(راستہ, سابقہ, super_سابقہ);

	sub = submodule_from_راستہ(the_repository, null_oid(the_hash_algo), راستہ);

	اگر (!sub)
		die(_("No url found for submodule path '%s' in .gitmodules"),
			displaypath);

	/*
	 * NEEDSWORK: In a multi-working-tree world, this needs to be
	 * set in the per-worktree config.
	 *
	 * Set active flag for the submodule being initialized
	 */
	اگر (!is_submodule_فعال(the_repository, راستہ)) {
		strbuf_addf(&sb, "submodule.%s.active", sub->name);
		repo_config_سیٹ_gently(the_repository, sb.buf, "true");
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
			حرف *oldurl = url;

			url = resolve_relative_url(oldurl, تہی, 0);
			آزاد(oldurl);
		}

		اگر (repo_config_سیٹ_gently(the_repository, sb.buf, url))
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
		} ورنہ {
			upd = submodule_update_type_مقصد_string(sub->update_strategy.type);
		}

		اگر (repo_config_سیٹ_gently(the_repository, sb.buf, upd))
			die(_("Failed to register update mode for submodule path '%s'"), displaypath);
	}

	اگر (the_repository->repository_format_submodule_راستہ_cfg)
		create_default_gitdir_config(sub->name);

	strbuf_release(&sb);
	آزاد(displaypath);
	آزاد(url);
}

ساکن خالی init_submodule_cb(ثابت ساخت cache_اندراج *list_شے, خالی *cb_ڈیٹا)
{
	ساخت init_cb *info = cb_ڈیٹا;

	init_submodule(list_شے->name, info->سابقہ, info->super_سابقہ,
		       info->flags);
}

ساکن صحیح module_init(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
		       ساخت repository *repo غیر_مستعمل)
{
	ساخت init_cb info = INIT_CB_INIT;
	ساخت pathspec pathspec = { 0 };
	ساخت module_list list = MODULE_LIST_INIT;
	صحیح quiet = 0;
	ساخت option module_init_options[] = {
		OPT__QUIET(&quiet, N_("suppress output for initializing a submodule")),
		OPT_انتہا()
	};
	ثابت حرف *ثابت git_submodule_helper_usage[] = {
		N_("git submodule init [<options>] [<path>]"),
		تہی
	};
	صحیح ret = 1;

	argc = parse_options(argc, argv, سابقہ, module_init_options,
			     git_submodule_helper_usage, 0);

	اگر (module_list_compute(argv, سابقہ, &pathspec, &list) < 0)
		جاؤ cleanup;

	/*
	 * If there are no path args and submodule.active is set then,
	 * by default, only initialize 'active' modules.
	 */
	اگر (!argc && !repo_config_get(the_repository, "submodule.active"))
		module_list_فعال(&list);

	info.سابقہ = سابقہ;
	اگر (quiet)
		info.flags |= OPT_QUIET;

	for_each_listed_submodule(&list, init_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	واپس ret;
}

ساخت حالت_cb {
	ثابت حرف *سابقہ;
	ثابت حرف *super_سابقہ;
	غیرعلامتی صحیح flags;
};
#تعریف_کرو حالت_CB_INIT { 0 }

ساکن خالی print_حالت(غیرعلامتی صحیح flags, حرف state, ثابت حرف *راستہ,
			 ثابت ساخت object_id *oid, ثابت حرف *displaypath)
{
	اگر (flags & OPT_QUIET)
		واپس;

	لکھو("%c%s %s", state, oid_مقصد_hex(oid), displaypath);

	اگر (state == ' ' || state == '+') {
		حرف *name = compute_rev_name(راستہ, oid_مقصد_hex(oid));

		اگر (name)
			لکھو(" (%s)", name);
		آزاد(name);
	}

	لکھو("\n");
}

ساکن صحیح handle_submodule_سر_ref(ثابت ساخت reference *ref, خالی *cb_ڈیٹا)
{
	ساخت object_id *output = cb_ڈیٹا;

	اگر (ref->oid)
		oidcpy(output, ref->oid);

	واپس 0;
}

ساکن خالی حالت_submodule(ثابت حرف *راستہ, ثابت ساخت object_id *ce_oid,
			     غیرعلامتی صحیح ce_flags, ثابت حرف *سابقہ,
			     ثابت حرف *super_سابقہ, غیرعلامتی صحیح flags)
{
	حرف *displaypath;
	ساخت strvec diff_files_دلائل = STRVEC_INIT;
	ساخت rev_info rev = REV_INFO_INIT;
	ساخت strbuf buf = STRBUF_INIT;
	ثابت حرف *git_dir;

	اگر (validate_submodule_راستہ(راستہ) < 0)
		die(تہی);

	اگر (!submodule_from_راستہ(the_repository, null_oid(the_hash_algo), راستہ))
		die(_("no submodule mapping found in .gitmodules for path '%s'"),
		      راستہ);

	displaypath = get_submodule_displaypath(راستہ, سابقہ, super_سابقہ);

	اگر ((CE_STAGEMASK & ce_flags) >> CE_STAGESHIFT) {
		print_حالت(flags, 'U', راستہ, null_oid(the_hash_algo), displaypath);
		جاؤ cleanup;
	}

	strbuf_addf(&buf, "%s/.git", راستہ);
	git_dir = read_gitfile(buf.buf);
	اگر (!git_dir)
		git_dir = buf.buf;

	اگر (!is_submodule_فعال(the_repository, راستہ) ||
	    !is_git_directory(git_dir)) {
		print_حالت(flags, '-', راستہ, ce_oid, displaypath);
		strbuf_release(&buf);
		جاؤ cleanup;
	}
	strbuf_release(&buf);

	strvec_pushl(&diff_files_دلائل, "diff-files",
		     "--ignore-submodules=dirty", "--quiet", "--",
		     راستہ, تہی);

	repo_config(the_repository, git_diff_basic_config, تہی);

	repo_init_revisions(the_repository, &rev, تہی);
	rev.abbrev = 0;
	تیاری_revisions_from_strvec(&diff_files_دلائل, &rev, تہی);
	تسلسل_diff_files(&rev, 0);

	اگر (!diff_نتیجہ_رمز(&rev)) {
		print_حالت(flags, ' ', راستہ, ce_oid,
			     displaypath);
	} ورنہ اگر (!(flags & OPT_CACHED)) {
		ساخت object_id oid;
		ساخت ref_store *refs = repo_get_submodule_ref_store(the_repository,
								      راستہ);

		اگر (!refs) {
			print_حالت(flags, '-', راستہ, ce_oid, displaypath);
			جاؤ cleanup;
		}
		اگر (refs_سر_ref(refs, handle_submodule_سر_ref, &oid))
			die(_("could not resolve HEAD ref inside the "
			      "submodule '%s'"), راستہ);

		print_حالت(flags, '+', راستہ, &oid, displaypath);
	} ورنہ {
		print_حالت(flags, '+', راستہ, ce_oid, displaypath);
	}

	اگر (flags & OPT_RECURSIVE) {
		ساخت بچہ_process cpr = بچہ_PROCESS_INIT;
		صحیح res;

		cpr.git_cmd = 1;
		cpr.dir = راستہ;
		prepare_submodule_repo_env(&cpr.env);

		strvec_pushl(&cpr.دلائل, "submodule--helper", "status",
			     "--recursive", تہی);
		strvec_pushf(&cpr.دلائل, "--super-prefix=%s/", displaypath);

		اگر (flags & OPT_CACHED)
			strvec_ڈالو(&cpr.دلائل, "--cached");

		اگر (flags & OPT_QUIET)
			strvec_ڈالو(&cpr.دلائل, "--quiet");

		res = تسلسل_command(&cpr);
		اگر (res == SIGPIPE + 128)
			اٹھاؤ(SIGPIPE);
		ورنہ اگر (res)
			die(_("failed to recurse into submodule '%s'"), راستہ);
	}

cleanup:
	strvec_clear(&diff_files_دلائل);
	آزاد(displaypath);
	release_revisions(&rev);
}

ساکن خالی حالت_submodule_cb(ثابت ساخت cache_اندراج *list_شے,
				خالی *cb_ڈیٹا)
{
	ساخت حالت_cb *info = cb_ڈیٹا;

	حالت_submodule(list_شے->name, &list_شے->oid, list_شے->ce_flags,
			 info->سابقہ, info->super_سابقہ, info->flags);
}

ساکن صحیح module_حالت(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
			 ساخت repository *repo غیر_مستعمل)
{
	ساخت حالت_cb info = حالت_CB_INIT;
	ساخت pathspec pathspec = { 0 };
	ساخت module_list list = MODULE_LIST_INIT;
	صحیح quiet = 0;
	ساخت option module_حالت_options[] = {
		OPT_SUPER_سابقہ(&info.super_سابقہ),
		OPT__QUIET(&quiet, N_("suppress submodule status output")),
		OPT_ثنائی_ہندسہ(0, "cached", &info.flags, N_("use commit stored in the index instead of the one stored in the submodule HEAD"), OPT_CACHED),
		OPT_ثنائی_ہندسہ(0, "recursive", &info.flags, N_("recurse into nested submodules"), OPT_RECURSIVE),
		OPT_انتہا()
	};
	ثابت حرف *ثابت git_submodule_helper_usage[] = {
		N_("git submodule status [--quiet] [--cached] [--recursive] [<path>...]"),
		تہی
	};
	صحیح ret = 1;

	argc = parse_options(argc, argv, سابقہ, module_حالت_options,
			     git_submodule_helper_usage, 0);

	اگر (module_list_compute(argv, سابقہ, &pathspec, &list) < 0)
		جاؤ cleanup;

	info.سابقہ = سابقہ;
	اگر (quiet)
		info.flags |= OPT_QUIET;

	for_each_listed_submodule(&list, حالت_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	واپس ret;
}

ساخت module_cb {
	غیرعلامتی صحیح mod_src;
	غیرعلامتی صحیح mod_dst;
	ساخت object_id oid_src;
	ساخت object_id oid_dst;
	حرف حالت_2;
	حرف *sm_راستہ;
};
#تعریف_کرو MODULE_CB_INIT { 0 }

ساکن خالی module_cb_release(ساخت module_cb *mcb)
{
	آزاد(mcb->sm_راستہ);
}

ساخت module_cb_list {
	ساخت module_cb **entries;
	صحیح alloc, nr;
};
#تعریف_کرو MODULE_CB_LIST_INIT { 0 }

ساکن خالی module_cb_list_release(ساخت module_cb_list *mcbl)
{
	صحیح i;

	برائے (i = 0; i < mcbl->nr; i++) {
		ساخت module_cb *mcb = mcbl->entries[i];

		module_cb_release(mcb);
		آزاد(mcb);
	}
	آزاد(mcbl->entries);
}

ساخت summary_cb {
	صحیح argc;
	ثابت حرف **argv;
	ثابت حرف *سابقہ;
	ثابت حرف *super_سابقہ;
	غیرعلامتی صحیح cached: 1;
	غیرعلامتی صحیح for_حالت: 1;
	غیرعلامتی صحیح files: 1;
	صحیح summary_حد_بندی;
};
#تعریف_کرو SUMMARY_CB_INIT { 0 }

شمار diff_cmd {
	DIFF_اشاریہ,
	DIFF_FILES
};

ساکن حرف *verify_submodule_committish(ثابت حرف *sm_راستہ,
					 ثابت حرف *committish)
{
	ساخت بچہ_process cp_rev_parse = بچہ_PROCESS_INIT;
	ساخت strbuf نتیجہ = STRBUF_INIT;

	cp_rev_parse.git_cmd = 1;
	cp_rev_parse.dir = sm_راستہ;
	prepare_submodule_repo_env(&cp_rev_parse.env);
	strvec_pushl(&cp_rev_parse.دلائل, "rev-parse", "-q", "--short", تہی);
	strvec_pushf(&cp_rev_parse.دلائل, "%s^0", committish);
	strvec_ڈالو(&cp_rev_parse.دلائل, "--");

	اگر (capture_command(&cp_rev_parse, &نتیجہ, 0))
		واپس تہی;

	strbuf_trim_trailing_newline(&نتیجہ);
	واپس strbuf_detach(&نتیجہ, تہی);
}

ساکن خالی print_submodule_summary(ساخت summary_cb *info, ثابت حرف *errmsg,
				    صحیح کل_commits, ثابت حرف *displaypath,
				    ثابت حرف *src_abbrev, ثابت حرف *dst_abbrev,
				    ساخت module_cb *p)
{
	اگر (p->حالت_2 == 'T') {
		اگر (S_ISGITLINK(p->mod_dst))
			لکھو(_("* %s %s(blob)->%s(submodule)"),
				 displaypath, src_abbrev, dst_abbrev);
		ورنہ
			لکھو(_("* %s %s(submodule)->%s(blob)"),
				 displaypath, src_abbrev, dst_abbrev);
	} ورنہ {
		لکھو("* %s %s...%s",
			displaypath, src_abbrev, dst_abbrev);
	}

	اگر (کل_commits < 0)
		لکھو(":\n");
	ورنہ
		لکھو(" (%d):\n", کل_commits);

	اگر (errmsg) {
		لکھو(_("%s"), errmsg);
	} ورنہ اگر (کل_commits > 0) {
		ساخت بچہ_process cp_log = بچہ_PROCESS_INIT;

		cp_log.git_cmd = 1;
		cp_log.dir = p->sm_راستہ;
		prepare_submodule_repo_env(&cp_log.env);
		strvec_pushl(&cp_log.دلائل, "log", تہی);

		اگر (S_ISGITLINK(p->mod_src) && S_ISGITLINK(p->mod_dst)) {
			اگر (info->summary_حد_بندی > 0)
				strvec_pushf(&cp_log.دلائل, "-%d",
					     info->summary_حد_بندی);

			strvec_pushl(&cp_log.دلائل, "--pretty=  %m %s",
				     "--first-parent", تہی);
			strvec_pushf(&cp_log.دلائل, "%s...%s",
				     src_abbrev, dst_abbrev);
		} ورنہ اگر (S_ISGITLINK(p->mod_dst)) {
			strvec_pushl(&cp_log.دلائل, "--pretty=  > %s",
				     "-1", dst_abbrev, تہی);
		} ورنہ {
			strvec_pushl(&cp_log.دلائل, "--pretty=  < %s",
				     "-1", src_abbrev, تہی);
		}
		تسلسل_command(&cp_log);
	}
	لکھو("\n");
}

ساکن خالی generate_submodule_summary(ساخت summary_cb *info,
				       ساخت module_cb *p)
{
	حرف *displaypath, *src_abbrev = تہی, *dst_abbrev;
	صحیح missing_src = 0, missing_dst = 0;
	ساخت strbuf errmsg = STRBUF_INIT;
	صحیح کل_commits = -1;

	اگر (!info->cached && oideq(&p->oid_dst, null_oid(the_hash_algo))) {
		اگر (S_ISGITLINK(p->mod_dst)) {
			ساخت ref_store *refs = repo_get_submodule_ref_store(the_repository,
									      p->sm_راستہ);

			اگر (refs)
				refs_سر_ref(refs, handle_submodule_سر_ref, &p->oid_dst);
		} ورنہ اگر (S_ISLNK(p->mod_dst) || S_ISREG(p->mod_dst)) {
			ساخت stat st;
			صحیح fd = open(p->sm_راستہ, O_RDONLY);

			اگر (fd < 0 || fstat(fd, &st) < 0 ||
			    اشاریہ_fd(the_repository->اشاریہ, &p->oid_dst, fd, &st, OBJ_BLOB,
				     p->sm_راستہ, 0))
				error(_("couldn't hash object from '%s'"), p->sm_راستہ);
		} ورنہ {
			/* for a submodule removal (mode:0000000), don't warn */
			اگر (p->mod_dst)
				warning(_("unexpected mode %o"), p->mod_dst);
		}
	}

	اگر (S_ISGITLINK(p->mod_src)) {
		اگر (p->حالت_2 != 'D')
			src_abbrev = verify_submodule_committish(p->sm_راستہ,
								 oid_مقصد_hex(&p->oid_src));
		اگر (!src_abbrev) {
			missing_src = 1;
			/*
			 * As `rev-parse` failed, we fallback to getting
			 * the abbreviated hash using oid_src. We do
			 * this as we might still need the abbreviated
			 * hash in cases like a submodule type change, etc.
			 */
			src_abbrev = xstrndup(oid_مقصد_hex(&p->oid_src), 7);
		}
	} ورنہ {
		/*
		 * The source does not point to a submodule.
		 * So, we fallback to getting the abbreviation using
		 * oid_src as we might still need the abbreviated
		 * hash in cases like submodule add, etc.
		 */
		src_abbrev = xstrndup(oid_مقصد_hex(&p->oid_src), 7);
	}

	اگر (S_ISGITLINK(p->mod_dst)) {
		dst_abbrev = verify_submodule_committish(p->sm_راستہ,
							 oid_مقصد_hex(&p->oid_dst));
		اگر (!dst_abbrev) {
			missing_dst = 1;
			/*
			 * As `rev-parse` failed, we fallback to getting
			 * the abbreviated hash using oid_dst. We do
			 * this as we might still need the abbreviated
			 * hash in cases like a submodule type change, etc.
			 */
			dst_abbrev = xstrndup(oid_مقصد_hex(&p->oid_dst), 7);
		}
	} ورنہ {
		/*
		 * The destination does not point to a submodule.
		 * So, we fallback to getting the abbreviation using
		 * oid_dst as we might still need the abbreviated
		 * hash in cases like a submodule removal, etc.
		 */
		dst_abbrev = xstrndup(oid_مقصد_hex(&p->oid_dst), 7);
	}

	displaypath = get_submodule_displaypath(p->sm_راستہ, info->سابقہ,
						info->super_سابقہ);

	اگر (!missing_src && !missing_dst) {
		ساخت بچہ_process cp_rev_list = بچہ_PROCESS_INIT;
		ساخت strbuf sb_rev_list = STRBUF_INIT;

		strvec_pushl(&cp_rev_list.دلائل, "rev-list",
			     "--first-parent", "--count", تہی);
		اگر (S_ISGITLINK(p->mod_src) && S_ISGITLINK(p->mod_dst))
			strvec_pushf(&cp_rev_list.دلائل, "%s...%s",
				     src_abbrev, dst_abbrev);
		ورنہ
			strvec_ڈالو(&cp_rev_list.دلائل, S_ISGITLINK(p->mod_src) ?
				    src_abbrev : dst_abbrev);
		strvec_ڈالو(&cp_rev_list.دلائل, "--");

		cp_rev_list.git_cmd = 1;
		cp_rev_list.dir = p->sm_راستہ;
		prepare_submodule_repo_env(&cp_rev_list.env);

		اگر (!capture_command(&cp_rev_list, &sb_rev_list, 0))
			کل_commits = atoi(sb_rev_list.buf);

		strbuf_release(&sb_rev_list);
	} ورنہ {
		/*
		 * Don't give error msg for modification whose dst is not
		 * submodule, i.e., deleted or changed to blob
		 */
		اگر (S_ISGITLINK(p->mod_dst)) {
			اگر (missing_src && missing_dst) {
				strbuf_addf(&errmsg, "  Warn: %s doesn't contain commits %s and %s\n",
					    displaypath, oid_مقصد_hex(&p->oid_src),
					    oid_مقصد_hex(&p->oid_dst));
			} ورنہ {
				strbuf_addf(&errmsg, "  Warn: %s doesn't contain commit %s\n",
					    displaypath, missing_src ?
					    oid_مقصد_hex(&p->oid_src) :
					    oid_مقصد_hex(&p->oid_dst));
			}
		}
	}

	print_submodule_summary(info, errmsg.len ? errmsg.buf : تہی,
				کل_commits, displaypath, src_abbrev,
				dst_abbrev, p);

	آزاد(displaypath);
	آزاد(src_abbrev);
	آزاد(dst_abbrev);
	strbuf_release(&errmsg);
}

ساکن خالی prepare_submodule_summary(ساخت summary_cb *info,
				      ساخت module_cb_list *list)
{
	صحیح i;
	برائے (i = 0; i < list->nr; i++) {
		ثابت ساخت submodule *sub;
		ساخت module_cb *p = list->entries[i];
		ساخت strbuf sm_gitdir = STRBUF_INIT;

		اگر (p->حالت_2 == 'D' || p->حالت_2 == 'T') {
			generate_submodule_summary(info, p);
			جاری;
		}

		اگر (info->for_حالت && p->حالت_2 != 'A' &&
		    (sub = submodule_from_راستہ(the_repository,
					       null_oid(the_hash_algo), p->sm_راستہ))) {
			حرف *config_کلید = تہی;
			ثابت حرف *قدر;
			صحیح ignore_all = 0;

			config_کلید = xstrfmt("submodule.%s.ignore",
					     sub->name);
			اگر (!repo_config_get_string_tmp(the_repository, config_کلید, &قدر))
				ignore_all = !strcmp(قدر, "all");
			ورنہ اگر (sub->ignore)
				ignore_all = !strcmp(sub->ignore, "all");

			آزاد(config_کلید);
			اگر (ignore_all)
				جاری;
		}

		/* Also show added or modified modules which are checked out */
		strbuf_addstr(&sm_gitdir, p->sm_راستہ);
		اگر (is_nonbare_repository_dir(&sm_gitdir))
			generate_submodule_summary(info, p);
		strbuf_release(&sm_gitdir);
	}
}

ساکن خالی submodule_summary_callback(ساخت diff_قطار_struct *q,
				       ساخت diff_options *options غیر_مستعمل,
				       خالی *ڈیٹا)
{
	صحیح i;
	ساخت module_cb_list *list = ڈیٹا;
	برائے (i = 0; i < q->nr; i++) {
		ساخت diff_filepair *p = q->قطار[i];
		ساخت module_cb *عارضی;

		اگر (!S_ISGITLINK(p->one->منوال) && !S_ISGITLINK(p->two->منوال))
			جاری;
		عارضی = xmalloc(حجم(*عارضی));
		عارضی->mod_src = p->one->منوال;
		عارضی->mod_dst = p->two->منوال;
		عارضی->oid_src = p->one->oid;
		عارضی->oid_dst = p->two->oid;
		عارضی->حالت_2 = p->حالت_2;
		عارضی->sm_راستہ = xstrdup(p->one->راستہ);

		ALLOC_GROW(list->entries, list->nr + 1, list->alloc);
		list->entries[list->nr++] = عارضی;
	}
}

ساکن ثابت حرف *get_diff_cmd(شمار diff_cmd diff_cmd)
{
	انتخاب (diff_cmd) {
	حالت DIFF_اشاریہ: واپس "diff-index";
	حالت DIFF_FILES: واپس "diff-files";
	طےشدہ: BUG("bad diff_cmd value %d", diff_cmd);
	}
}

ساکن صحیح compute_summary_module_list(ساخت object_id *سر_oid,
				       ساخت summary_cb *info,
				       شمار diff_cmd diff_cmd)
{
	ساخت strvec diff_دلائل = STRVEC_INIT;
	ساخت rev_info rev;
	ساخت module_cb_list list = MODULE_CB_LIST_INIT;
	صحیح ret = 0;

	strvec_ڈالو(&diff_دلائل, get_diff_cmd(diff_cmd));
	اگر (info->cached)
		strvec_ڈالو(&diff_دلائل, "--cached");
	strvec_pushl(&diff_دلائل, "--ignore-submodules=dirty", "--raw", تہی);
	اگر (سر_oid)
		strvec_ڈالو(&diff_دلائل, oid_مقصد_hex(سر_oid));
	strvec_ڈالو(&diff_دلائل, "--");
	اگر (info->argc)
		strvec_pushv(&diff_دلائل, info->argv);

	repo_config(the_repository, git_diff_basic_config, تہی);
	repo_init_revisions(the_repository, &rev, info->سابقہ);
	rev.abbrev = 0;
	precompose_argv_سابقہ(diff_دلائل.nr, diff_دلائل.v, تہی);
	تیاری_revisions_from_strvec(&diff_دلائل, &rev, تہی);
	rev.diffopt.output_format = DIFF_FORMAT_NO_OUTPUT | DIFF_FORMAT_CALLBACK;
	rev.diffopt.format_callback = submodule_summary_callback;
	rev.diffopt.format_callback_ڈیٹا = &list;

	اگر (!info->cached) {
		اگر (diff_cmd == DIFF_اشاریہ)
			تیاری_work_درخت(the_repository);
		اگر (repo_read_اشاریہ_preload(the_repository, &rev.diffopt.pathspec, 0) < 0) {
			perror("repo_read_index_preload");
			ret = -1;
			جاؤ cleanup;
		}
	} ورنہ اگر (repo_read_اشاریہ(the_repository) < 0) {
		perror("repo_read_cache");
		ret = -1;
		جاؤ cleanup;
	}

	اگر (diff_cmd == DIFF_اشاریہ)
		تسلسل_diff_اشاریہ(&rev, info->cached ? DIFF_اشاریہ_CACHED : 0);
	ورنہ
		تسلسل_diff_files(&rev, 0);
	prepare_submodule_summary(info, &list);
cleanup:
	strvec_clear(&diff_دلائل);
	release_revisions(&rev);
	module_cb_list_release(&list);
	واپس ret;
}

ساکن صحیح module_summary(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
			  ساخت repository *repo غیر_مستعمل)
{
	ساخت summary_cb info = SUMMARY_CB_INIT;
	صحیح cached = 0;
	صحیح for_حالت = 0;
	صحیح files = 0;
	صحیح summary_حد_بندی = -1;
	شمار diff_cmd diff_cmd = DIFF_اشاریہ;
	ساخت object_id سر_oid;
	صحیح ret;
	ساخت option module_summary_options[] = {
		OPT_BOOL(0, "cached", &cached,
			 N_("use the commit stored in the index instead of the submodule HEAD")),
		OPT_BOOL(0, "files", &files,
			 N_("compare the commit in the index with that in the submodule HEAD")),
		OPT_BOOL(0, "for-status", &for_حالت,
			 N_("skip submodules with 'ignore_config' value set to 'all'")),
		OPT_INTEGER('n', "summary-limit", &summary_حد_بندی,
			     N_("limit the summary size")),
		OPT_انتہا()
	};
	ثابت حرف *ثابت git_submodule_helper_usage[] = {
		N_("git submodule summary [<options>] [<commit>] [--] [<path>]"),
		تہی
	};

	argc = parse_options(argc, argv, سابقہ, module_summary_options,
			     git_submodule_helper_usage, 0);

	اگر (!summary_حد_بندی)
		واپس 0;

	اگر (!repo_get_oid(the_repository, argc ? argv[0] : "HEAD", &سر_oid)) {
		اگر (argc) {
			argv++;
			argc--;
		}
	} ورنہ اگر (!argc || !strcmp(argv[0], "HEAD")) {
		/* before the first commit: compare with an empty tree */
		oidcpy(&سر_oid, the_hash_algo->خالی_خانے_درخت);
		اگر (argc) {
			argv++;
			argc--;
		}
	} ورنہ {
		اگر (repo_get_oid(the_repository, "HEAD", &سر_oid))
			die(_("could not fetch a revision for HEAD"));
	}

	اگر (files) {
		اگر (cached)
			die(_("options '%s' and '%s' cannot be used together"), "--cached", "--files");
		diff_cmd = DIFF_FILES;
	}

	info.argc = argc;
	info.argv = argv;
	info.سابقہ = سابقہ;
	info.cached = !!cached;
	info.files = !!files;
	info.for_حالت = !!for_حالت;
	info.summary_حد_بندی = summary_حد_بندی;

	ret = compute_summary_module_list((diff_cmd == DIFF_اشاریہ) ? &سر_oid : تہی,
					  &info, diff_cmd);
	واپس ret;
}

ساکن صحیح module_gitdir(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ غیر_مستعمل,
			 ساخت repository *repo)
{
	ساخت strbuf gitdir = STRBUF_INIT;

	اگر (argc != 2)
		usage(_("git submodule--helper gitdir <name>"));

	submodule_name_مقصد_gitdir(&gitdir, repo, argv[1]);

	لکھو("%s\n", gitdir.buf);

	strbuf_release(&gitdir);
	واپس 0;
}

ساکن صحیح module_migrate(صحیح argc غیر_مستعمل, ثابت حرف **argv غیر_مستعمل,
			  ثابت حرف *سابقہ غیر_مستعمل, ساخت repository *repo)
{
	ساخت strbuf module_dir = STRBUF_INIT;
	DIR *dir;
	ساخت dirent *de;
	صحیح repo_نسخہ = 0;

	repo_git_راستہ_append(repo, &module_dir, "modules/");

	dir = opendir(module_dir.buf);
	اگر (!dir)
		die(_("could not open '%s'"), module_dir.buf);

	جب ((de = readdir(dir))) {
		ساخت strbuf gitdir_راستہ = STRBUF_INIT;
		حرف *کلید;
		ثابت حرف *قدر;

		اگر (is_dot_or_dotdot(de->d_name))
			جاری;

		strbuf_addf(&gitdir_راستہ, "%s/%s", module_dir.buf, de->d_name);
		اگر (!is_git_directory(gitdir_راستہ.buf)) {
			strbuf_release(&gitdir_راستہ);
			جاری;
		}
		strbuf_release(&gitdir_راستہ);

		کلید = xstrfmt("submodule.%s.gitdir", de->d_name);
		اگر (!repo_config_get_string_tmp(repo, کلید, &قدر)) {
			/* Already has a gitdir config, nothing to do. */
			آزاد(کلید);
			جاری;
		}
		آزاد(کلید);

		create_default_gitdir_config(de->d_name);
	}

	closedir(dir);
	strbuf_release(&module_dir);

	repo_config_get_int(the_repository, "core.repositoryformatversion", &repo_نسخہ);
	اگر (repo_نسخہ == 0 &&
	    repo_config_سیٹ_gently(repo, "core.repositoryformatversion", "1"))
		die(_("could not set core.repositoryformatversion to 1.\n"
		      "Please set it for migration to work, for example:\n"
		      "git config core.repositoryformatversion 1"));

	اگر (repo_config_سیٹ_gently(repo, "extensions.submodulePathConfig", "true"))
		die(_("could not enable submodulePathConfig extension. It is required\n"
		      "for migration to work. Please enable it in the root repo:\n"
		      "git config extensions.submodulePathConfig true"));

	repo->repository_format_submodule_راستہ_cfg = 1;

	واپس 0;
}

ساخت sync_cb {
	ثابت حرف *سابقہ;
	ثابت حرف *super_سابقہ;
	غیرعلامتی صحیح flags;
};
#تعریف_کرو SYNC_CB_INIT { 0 }

ساکن خالی sync_submodule(ثابت حرف *راستہ, ثابت حرف *سابقہ,
			   ثابت حرف *super_سابقہ, غیرعلامتی صحیح flags)
{
	ثابت ساخت submodule *sub;
	حرف *remote_کلید = تہی;
	حرف *sub_origin_url, *super_config_url, *displaypath, *default_remote;
	ساخت strbuf sb = STRBUF_INIT;
	حرف *sub_config_راستہ = تہی;
	صحیح رمز;

	اگر (!is_submodule_فعال(the_repository, راستہ))
		واپس;

	اگر (validate_submodule_راستہ(راستہ) < 0)
		die(تہی);

	sub = submodule_from_راستہ(the_repository, null_oid(the_hash_algo), راستہ);

	اگر (sub && sub->url) {
		اگر (starts_with_dot_dot_slash(sub->url) ||
		    starts_with_dot_slash(sub->url)) {
			حرف *اوپر_راستہ = get_اوپر_راستہ(راستہ);

			sub_origin_url = resolve_relative_url(sub->url, اوپر_راستہ, 1);
			super_config_url = resolve_relative_url(sub->url, تہی, 1);
			آزاد(اوپر_راستہ);
		} ورنہ {
			sub_origin_url = xstrdup(sub->url);
			super_config_url = xstrdup(sub->url);
		}
	} ورنہ {
		sub_origin_url = xstrdup("");
		super_config_url = xstrdup("");
	}

	displaypath = get_submodule_displaypath(راستہ, سابقہ, super_سابقہ);

	اگر (!(flags & OPT_QUIET))
		لکھو(_("Synchronizing submodule url for '%s'\n"),
			 displaypath);

	strbuf_reset(&sb);
	strbuf_addf(&sb, "submodule.%s.url", sub->name);
	اگر (repo_config_سیٹ_gently(the_repository, sb.buf, super_config_url))
		die(_("failed to register url for submodule path '%s'"),
		      displaypath);

	اگر (!is_submodule_populated_gently(راستہ, تہی))
		جاؤ cleanup;

	strbuf_reset(&sb);
	رمز = get_default_remote_submodule(راستہ, &default_remote);
	اگر (رمز)
		باہر_نکلو(رمز);

	remote_کلید = xstrfmt("remote.%s.url", default_remote);
	آزاد(default_remote);

	submodule_مقصد_gitdir(the_repository, &sb, راستہ);
	strbuf_addstr(&sb, "/config");

	اگر (repo_config_سیٹ_اندر_file_gently(the_repository, sb.buf, remote_کلید, تہی, sub_origin_url))
		die(_("failed to update remote for submodule '%s'"),
		      راستہ);

	اگر (flags & OPT_RECURSIVE) {
		ساخت بچہ_process cpr = بچہ_PROCESS_INIT;

		cpr.git_cmd = 1;
		cpr.dir = راستہ;
		prepare_submodule_repo_env(&cpr.env);

		strvec_pushl(&cpr.دلائل, "submodule--helper", "sync",
			     "--recursive", تہی);
		strvec_pushf(&cpr.دلائل, "--super-prefix=%s/", displaypath);

		اگر (flags & OPT_QUIET)
			strvec_ڈالو(&cpr.دلائل, "--quiet");

		اگر (تسلسل_command(&cpr))
			die(_("failed to recurse into submodule '%s'"),
			      راستہ);
	}

cleanup:
	آزاد(super_config_url);
	آزاد(sub_origin_url);
	strbuf_release(&sb);
	آزاد(remote_کلید);
	آزاد(displaypath);
	آزاد(sub_config_راستہ);
}

ساکن خالی sync_submodule_cb(ثابت ساخت cache_اندراج *list_شے, خالی *cb_ڈیٹا)
{
	ساخت sync_cb *info = cb_ڈیٹا;

	sync_submodule(list_شے->name, info->سابقہ, info->super_سابقہ,
		       info->flags);
}

ساکن صحیح module_sync(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
		       ساخت repository *repo غیر_مستعمل)
{
	ساخت sync_cb info = SYNC_CB_INIT;
	ساخت pathspec pathspec = { 0 };
	ساخت module_list list = MODULE_LIST_INIT;
	صحیح quiet = 0;
	صحیح recursive = 0;
	ساخت option module_sync_options[] = {
		OPT_SUPER_سابقہ(&info.super_سابقہ),
		OPT__QUIET(&quiet, N_("suppress output of synchronizing submodule url")),
		OPT_BOOL(0, "recursive", &recursive,
			N_("recurse into nested submodules")),
		OPT_انتہا()
	};
	ثابت حرف *ثابت git_submodule_helper_usage[] = {
		N_("git submodule sync [--quiet] [--recursive] [<path>]"),
		تہی
	};
	صحیح ret = 1;

	argc = parse_options(argc, argv, سابقہ, module_sync_options,
			     git_submodule_helper_usage, 0);

	اگر (module_list_compute(argv, سابقہ, &pathspec, &list) < 0)
		جاؤ cleanup;

	info.سابقہ = سابقہ;
	اگر (quiet)
		info.flags |= OPT_QUIET;
	اگر (recursive)
		info.flags |= OPT_RECURSIVE;

	for_each_listed_submodule(&list, sync_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	واپس ret;
}

ساخت deinit_cb {
	ثابت حرف *سابقہ;
	غیرعلامتی صحیح flags;
};
#تعریف_کرو DEINIT_CB_INIT { 0 }

ساکن خالی deinit_submodule(ثابت حرف *راستہ, ثابت حرف *سابقہ,
			     غیرعلامتی صحیح flags)
{
	ثابت ساخت submodule *sub;
	حرف *displaypath = تہی;
	ساخت بچہ_process cp_config = بچہ_PROCESS_INIT;
	ساخت strbuf sb_config = STRBUF_INIT;
	حرف *sub_git_dir = xstrfmt("%s/.git", راستہ);

	اگر (validate_submodule_راستہ(راستہ) < 0)
		die(تہی);

	sub = submodule_from_راستہ(the_repository, null_oid(the_hash_algo), راستہ);

	اگر (!sub || !sub->name)
		جاؤ cleanup;

	displaypath = get_submodule_displaypath(راستہ, سابقہ, تہی);

	/* remove the submodule work tree (unless the user already did it) */
	اگر (is_directory(راستہ)) {
		ساخت strbuf sb_rm = STRBUF_INIT;
		ثابت حرف *format;

		اگر (is_directory(sub_git_dir)) {
			اگر (!(flags & OPT_QUIET))
				warning(_("Submodule work tree '%s' contains a .git "
					  "directory. This will be replaced with a "
					  ".git file by using absorbgitdirs."),
					displaypath);

			absorb_git_dir_into_superproject(راستہ, تہی);

		}

		اگر (!(flags & OPT_FORCE)) {
			ساخت بچہ_process cp_rm = بچہ_PROCESS_INIT;

			cp_rm.git_cmd = 1;
			strvec_pushl(&cp_rm.دلائل, "rm", "-qn",
				     راستہ, تہی);

			اگر (تسلسل_command(&cp_rm))
				die(_("Submodule work tree '%s' contains local "
				      "modifications; use '-f' to discard them"),
				      displaypath);
		}

		strbuf_addstr(&sb_rm, راستہ);

		اگر (!remove_dir_recursively(&sb_rm, 0))
			format = _("Cleared directory '%s'\n");
		ورنہ
			format = _("Could not remove submodule work tree '%s'\n");

		اگر (!(flags & OPT_QUIET))
			لکھو(format, displaypath);

		submodule_unset_core_worktree(sub);

		strbuf_release(&sb_rm);
	}

	اگر (mkdir(راستہ, 0777))
		لکھو(_("could not create empty submodule directory %s"),
		      displaypath);

	cp_config.git_cmd = 1;
	strvec_pushl(&cp_config.دلائل, "config", "--get-regexp", تہی);
	strvec_pushf(&cp_config.دلائل, "submodule.%s\\.", sub->name);

	/* remove the .git/config entries (unless the user already did it) */
	اگر (!capture_command(&cp_config, &sb_config, 0) && sb_config.len) {
		حرف *sub_کلید = xstrfmt("submodule.%s", sub->name);

		/*
		 * remove the whole section so we have a clean state when
		 * the user later decides to init this submodule again
		 */
		repo_config_rename_section_اندر_file(the_repository, تہی, sub_کلید, تہی);
		اگر (!(flags & OPT_QUIET))
			لکھو(_("Submodule '%s' (%s) unregistered for path '%s'\n"),
				 sub->name, sub->url, displaypath);
		آزاد(sub_کلید);
	}

cleanup:
	آزاد(displaypath);
	آزاد(sub_git_dir);
	strbuf_release(&sb_config);
}

ساکن خالی deinit_submodule_cb(ثابت ساخت cache_اندراج *list_شے,
				خالی *cb_ڈیٹا)
{
	ساخت deinit_cb *info = cb_ڈیٹا;
	deinit_submodule(list_شے->name, info->سابقہ, info->flags);
}

ساکن صحیح module_deinit(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
			 ساخت repository *repo غیر_مستعمل)
{
	ساخت deinit_cb info = DEINIT_CB_INIT;
	ساخت pathspec pathspec = { 0 };
	ساخت module_list list = MODULE_LIST_INIT;
	صحیح quiet = 0;
	صحیح force = 0;
	صحیح all = 0;
	ساخت option module_deinit_options[] = {
		OPT__QUIET(&quiet, N_("suppress submodule status output")),
		OPT__FORCE(&force, N_("remove submodule working trees even if they contain local changes"), 0),
		OPT_BOOL(0, "all", &all, N_("unregister all submodules")),
		OPT_انتہا()
	};
	ثابت حرف *ثابت git_submodule_helper_usage[] = {
		N_("git submodule deinit [--quiet] [-f | --force] [--all | [--] [<path>...]]"),
		تہی
	};
	صحیح ret = 1;

	argc = parse_options(argc, argv, سابقہ, module_deinit_options,
			     git_submodule_helper_usage, 0);

	اگر (all && argc) {
		error("pathspec and --all are incompatible");
		usage_with_options(git_submodule_helper_usage,
				   module_deinit_options);
	}

	اگر (!argc && !all)
		die(_("Use '--all' if you really want to deinitialize all submodules"));

	اگر (module_list_compute(argv, سابقہ, &pathspec, &list) < 0)
		جاؤ cleanup;

	info.سابقہ = سابقہ;
	اگر (quiet)
		info.flags |= OPT_QUIET;
	اگر (force)
		info.flags |= OPT_FORCE;

	for_each_listed_submodule(&list, deinit_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	واپس ret;
}

ساخت module_clone_ڈیٹا {
	ثابت حرف *سابقہ;
	ثابت حرف *راستہ;
	ثابت حرف *name;
	ثابت حرف *url;
	صحیح گہرائی;
	ساخت list_objects_filter_options *filter_options;
	شمار ref_storage_format ref_storage_format;
	غیرعلامتی صحیح quiet: 1;
	غیرعلامتی صحیح پیش_رفت: 1;
	غیرعلامتی صحیح dissociate: 1;
	غیرعلامتی صحیح require_init: 1;
	صحیح single_branch;
};
#تعریف_کرو MODULE_CLONE_ڈیٹا_INIT { \
	.single_branch = -1, \
	.ref_storage_format = REF_STORAGE_FORMAT_UNKNOWN, \
}

ساخت submodule_alternate_تیاری {
	ثابت حرف *submodule_name;
	شمار SUBMODULE_ALTERNATE_ERROR_منوال {
		SUBMODULE_ALTERNATE_ERROR_DIE,
		SUBMODULE_ALTERNATE_ERROR_INFO,
		SUBMODULE_ALTERNATE_ERROR_IGNORE
	} error_منوال;
	ساخت string_list *reference;
};
#تعریف_کرو SUBMODULE_ALTERNATE_تیاری_INIT { \
	.error_منوال = SUBMODULE_ALTERNATE_ERROR_IGNORE, \
}

ساکن ثابت حرف alternate_error_advice[] = N_(
"An alternate computed from a superproject's alternate is invalid.\n"
"To allow Git to clone without an alternate in such a case, set\n"
"submodule.alternateErrorStrategy to 'info' or, equivalently, clone with\n"
"'--reference-if-able' instead of '--reference'."
);

ساکن صحیح جوڑو_possible_reference_from_superproject(
		ساخت odb_files_dir *alt_odb, خالی *sas_cb)
{
	ساخت submodule_alternate_تیاری *sas = sas_cb;
	سائز_t len;

	/*
	 * If the alternate object store is another repository, try the
	 * standard layout with .git/(modules/<name>)+/objects
	 */
	اگر (strip_suffix(alt_odb->abspath, "/objects", &len)) {
		ساخت repository alternate;
		حرف *sm_alternate;
		ساخت strbuf sb = STRBUF_INIT;
		ساخت strbuf err = STRBUF_INIT;
		strbuf_جوڑو(&sb, alt_odb->abspath, len);

		اگر (repo_init(&alternate, sb.buf, تہی) < 0)
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

		sm_alternate = compute_alternate_راستہ(sb.buf, &err);
		اگر (sm_alternate) {
			حرف *p = strbuf_detach(&sb, تہی);

			string_list_append(sas->reference, p)->util = p;
			آزاد(sm_alternate);
		} ورنہ {
			انتخاب (sas->error_منوال) {
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

	واپس 0;
}

ساکن خالی prepare_possible_alternates(ثابت حرف *sm_name,
		ساخت string_list *reference)
{
	حرف *sm_alternate = تہی, *error_strategy = تہی;
	ساخت submodule_alternate_تیاری sas = SUBMODULE_ALTERNATE_تیاری_INIT;

	repo_config_get_string(the_repository, "submodule.alternateLocation", &sm_alternate);
	اگر (!sm_alternate)
		واپس;

	repo_config_get_string(the_repository, "submodule.alternateErrorStrategy", &error_strategy);

	اگر (!error_strategy)
		error_strategy = xstrdup("die");

	sas.submodule_name = sm_name;
	sas.reference = reference;
	اگر (!strcmp(error_strategy, "die"))
		sas.error_منوال = SUBMODULE_ALTERNATE_ERROR_DIE;
	ورنہ اگر (!strcmp(error_strategy, "info"))
		sas.error_منوال = SUBMODULE_ALTERNATE_ERROR_INFO;
	ورنہ اگر (!strcmp(error_strategy, "ignore"))
		sas.error_منوال = SUBMODULE_ALTERNATE_ERROR_IGNORE;
	ورنہ
		die(_("Value '%s' for submodule.alternateErrorStrategy is not recognized"), error_strategy);

	اگر (!strcmp(sm_alternate, "superproject"))
		odb_for_each_alternate(the_repository->objects,
				       جوڑو_possible_reference_from_superproject, &sas);
	ورنہ اگر (!strcmp(sm_alternate, "no"))
		; /* do nothing */
	ورنہ
		die(_("Value '%s' for submodule.alternateLocation is not recognized"), sm_alternate);

	آزاد(sm_alternate);
	آزاد(error_strategy);
}

ساکن حرف *clone_submodule_sm_gitdir(ثابت حرف *name)
{
	ساخت strbuf sb = STRBUF_INIT;
	حرف *sm_gitdir;

	submodule_name_مقصد_gitdir(&sb, the_repository, name);
	sm_gitdir = absolute_pathdup(sb.buf);
	strbuf_release(&sb);

	واپس sm_gitdir;
}

ساکن صحیح dir_شامل_ہے_only_dotgit(ثابت حرف *راستہ)
{
	DIR *dir = opendir(راستہ);
	ساخت dirent *e;
	صحیح ret = 1;

	اگر (!dir)
		واپس 0;

	e = readdir_skip_dot_and_dotdot(dir);
	اگر (!e)
		ret = 0;
	ورنہ اگر (strcmp(DEFAULT_GIT_DIR_ENVIRONMENT, e->d_name) ||
		 (e = readdir_skip_dot_and_dotdot(dir))) {
		error("unexpected item '%s' in '%s'", e->d_name, راستہ);
		ret = 0;
	}

	closedir(dir);
	واپس ret;
}

ساکن صحیح clone_submodule(ثابت ساخت module_clone_ڈیٹا *clone_ڈیٹا,
			   ساخت string_list *reference)
{
	حرف *p;
	حرف *sm_gitdir = clone_submodule_sm_gitdir(clone_ڈیٹا->name);
	حرف *sm_alternate = تہی, *error_strategy = تہی;
	ساخت stat st;
	ساخت بچہ_process cp = بچہ_PROCESS_INIT;
	ثابت حرف *clone_ڈیٹا_راستہ = clone_ڈیٹا->راستہ;
	حرف *مقصد_free = تہی;

	اگر (validate_submodule_راستہ(clone_ڈیٹا_راستہ) < 0)
		die(تہی);

	اگر (!is_absolute_راستہ(clone_ڈیٹا->راستہ))
		clone_ڈیٹا_راستہ = مقصد_free = xstrfmt("%s/%s", repo_get_work_درخت(the_repository),
						    clone_ڈیٹا->راستہ);

	اگر (!file_exists(sm_gitdir)) {
		اگر (clone_ڈیٹا->require_init && !stat(clone_ڈیٹا_راستہ, &st) &&
		    !is_خالی_خانے_dir(clone_ڈیٹا_راستہ))
			die(_("directory not empty: '%s'"), clone_ڈیٹا_راستہ);

		اگر (محفوظ_create_leading_directories_const(the_repository, sm_gitdir) < 0)
			die(_("could not create directory '%s'"), sm_gitdir);

		prepare_possible_alternates(clone_ڈیٹا->name, reference);

		strvec_ڈالو(&cp.دلائل, "clone");
		strvec_ڈالو(&cp.دلائل, "--no-checkout");
		اگر (clone_ڈیٹا->quiet)
			strvec_ڈالو(&cp.دلائل, "--quiet");
		اگر (clone_ڈیٹا->پیش_رفت)
			strvec_ڈالو(&cp.دلائل, "--progress");
		اگر (clone_ڈیٹا->گہرائی > 0)
			strvec_pushf(&cp.دلائل, "--depth=%d", clone_ڈیٹا->گہرائی);
		اگر (reference->nr) {
			ساخت string_list_شے *شے;

			for_each_string_list_شے(شے, reference)
				strvec_pushl(&cp.دلائل, "--reference",
					     شے->string, تہی);
		}
		اگر (clone_ڈیٹا->ref_storage_format != REF_STORAGE_FORMAT_UNKNOWN)
			strvec_pushf(&cp.دلائل, "--ref-storage-format=%s",
				     ref_storage_format_مقصد_name(clone_ڈیٹا->ref_storage_format));
		اگر (clone_ڈیٹا->dissociate)
			strvec_ڈالو(&cp.دلائل, "--dissociate");
		اگر (sm_gitdir && *sm_gitdir)
			strvec_pushl(&cp.دلائل, "--separate-git-dir", sm_gitdir, تہی);
		اگر (clone_ڈیٹا->filter_options && clone_ڈیٹا->filter_options->choice)
			strvec_pushf(&cp.دلائل, "--filter=%s",
				     expand_list_objects_filter_spec(
					     clone_ڈیٹا->filter_options));
		اگر (clone_ڈیٹا->single_branch >= 0)
			strvec_ڈالو(&cp.دلائل, clone_ڈیٹا->single_branch ?
				    "--single-branch" :
				    "--no-single-branch");

		strvec_ڈالو(&cp.دلائل, "--");
		strvec_ڈالو(&cp.دلائل, clone_ڈیٹا->url);
		strvec_ڈالو(&cp.دلائل, clone_ڈیٹا_راستہ);

		cp.git_cmd = 1;
		prepare_submodule_repo_env(&cp.env);
		cp.no_stdin = 1;

		اگر(تسلسل_command(&cp))
			die(_("clone of '%s' into submodule path '%s' failed"),
			    clone_ڈیٹا->url, clone_ڈیٹا_راستہ);

		اگر (clone_ڈیٹا->require_init && !stat(clone_ڈیٹا_راستہ, &st) &&
		    !dir_شامل_ہے_only_dotgit(clone_ڈیٹا_راستہ)) {
			حرف *dot_git = xstrfmt("%s/.git", clone_ڈیٹا_راستہ);
			unlink(dot_git);
			آزاد(dot_git);
			die(_("directory not empty: '%s'"), clone_ڈیٹا_راستہ);
		}
	} ورنہ {
		حرف *راستہ;

		اگر (clone_ڈیٹا->require_init && !stat(clone_ڈیٹا_راستہ, &st) &&
		    !is_خالی_خانے_dir(clone_ڈیٹا_راستہ))
			die(_("directory not empty: '%s'"), clone_ڈیٹا_راستہ);
		اگر (محفوظ_create_leading_directories_const(the_repository, clone_ڈیٹا_راستہ) < 0)
			die(_("could not create directory '%s'"), clone_ڈیٹا_راستہ);
		راستہ = xstrfmt("%s/index", sm_gitdir);
		unlink_or_warn(راستہ);
		آزاد(راستہ);
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
	اگر (validate_submodule_git_dir(sm_gitdir, clone_ڈیٹا->name) < 0) {
		حرف *سر = xstrfmt("%s/HEAD", sm_gitdir);
		unlink(سر);
		آزاد(سر);
		die(_("refusing to create/use '%s' in another submodule's git dir. "
		      "Enabling extensions.submodulePathConfig should fix this."),
		    sm_gitdir);
	}

	connect_work_درخت_and_git_dir(clone_ڈیٹا_راستہ, sm_gitdir, 0);

	p = repo_submodule_راستہ(the_repository, clone_ڈیٹا_راستہ, "config");
	اگر (!p)
		die(_("could not get submodule directory for '%s'"), clone_ڈیٹا_راستہ);

	/* setup alternateLocation and alternateErrorStrategy in the cloned submodule if needed */
	repo_config_get_string(the_repository, "submodule.alternateLocation", &sm_alternate);
	اگر (sm_alternate)
		repo_config_سیٹ_اندر_file(the_repository, p, "submodule.alternateLocation",
					sm_alternate);
	repo_config_get_string(the_repository, "submodule.alternateErrorStrategy", &error_strategy);
	اگر (error_strategy)
		repo_config_سیٹ_اندر_file(the_repository, p, "submodule.alternateErrorStrategy",
					error_strategy);

	آزاد(sm_alternate);
	آزاد(error_strategy);

	آزاد(sm_gitdir);
	آزاد(p);
	آزاد(مقصد_free);
	واپس 0;
}

ساکن صحیح module_clone(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
			ساخت repository *repo غیر_مستعمل)
{
	صحیح dissociate = 0, quiet = 0, پیش_رفت = 0, require_init = 0;
	ساخت module_clone_ڈیٹا clone_ڈیٹا = MODULE_CLONE_ڈیٹا_INIT;
	ساخت string_list reference = STRING_LIST_INIT_NODUP;
	ساخت list_objects_filter_options filter_options =
		LIST_OBJECTS_FILTER_INIT;
	ثابت حرف *ref_storage_format = تہی;

	ساخت option module_clone_options[] = {
		OPT_STRING(0, "prefix", &clone_ڈیٹا.سابقہ,
			   N_("path"),
			   N_("alternative anchor for relative paths")),
		OPT_STRING(0, "path", &clone_ڈیٹا.راستہ,
			   N_("path"),
			   N_("where the new submodule will be cloned to")),
		OPT_STRING(0, "name", &clone_ڈیٹا.name,
			   N_("string"),
			   N_("name of the new submodule")),
		OPT_STRING(0, "url", &clone_ڈیٹا.url,
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
		OPT_INTEGER(0, "depth", &clone_ڈیٹا.گہرائی,
			   N_("depth for shallow clones")),
		OPT__QUIET(&quiet, "suppress output for cloning a submodule"),
		OPT_BOOL(0, "progress", &پیش_رفت,
			   N_("force cloning progress")),
		OPT_BOOL(0, "require-init", &require_init,
			   N_("disallow cloning into non-empty directory")),
		OPT_BOOL(0, "single-branch", &clone_ڈیٹا.single_branch,
			 N_("clone only one branch, HEAD or --branch")),
		OPT_PARSE_LIST_OBJECTS_FILTER(&filter_options),
		OPT_انتہا()
	};
	ثابت حرف *ثابت git_submodule_helper_usage[] = {
		N_("git submodule--helper clone [--prefix=<path>] [--quiet] "
		   "[--reference <repository>] [--name <name>] [--depth <depth>] "
		   "[--single-branch] [--filter <filter-spec>] "
		   "--url <url> --path <path>"),
		تہی
	};

	argc = parse_options(argc, argv, سابقہ, module_clone_options,
			     git_submodule_helper_usage, 0);

	اگر (ref_storage_format) {
		clone_ڈیٹا.ref_storage_format = ref_storage_format_by_name(ref_storage_format);
		اگر (clone_ڈیٹا.ref_storage_format == REF_STORAGE_FORMAT_UNKNOWN)
			die(_("unknown ref storage format '%s'"), ref_storage_format);
	}
	clone_ڈیٹا.dissociate = !!dissociate;
	clone_ڈیٹا.quiet = !!quiet;
	clone_ڈیٹا.پیش_رفت = !!پیش_رفت;
	clone_ڈیٹا.require_init = !!require_init;
	clone_ڈیٹا.filter_options = &filter_options;

	اگر (argc || !clone_ڈیٹا.url || !clone_ڈیٹا.راستہ || !*(clone_ڈیٹا.راستہ))
		usage_with_options(git_submodule_helper_usage,
				   module_clone_options);

	clone_submodule(&clone_ڈیٹا, &reference);
	list_objects_filter_release(&filter_options);
	string_list_clear(&reference, 1);
	واپس 0;
}

ساکن صحیح determine_submodule_update_strategy(ساخت repository *r,
					       صحیح just_cloned,
					       ثابت حرف *راستہ,
					       شمار submodule_update_type update,
					       ساخت submodule_update_strategy *برآمد)
{
	ثابت ساخت submodule *sub = submodule_from_راستہ(r, null_oid(the_hash_algo), راستہ);
	حرف *کلید;
	ثابت حرف *val;
	صحیح ret;

	/*
	 * NEEDSWORK: audit and ensure that update_submodule() has right
	 * to assume that submodule_from_path() above will always succeed.
	 */
	اگر (!sub)
		BUG("update_submodule assumes a submodule exists at path (%s)",
		    راستہ);
	کلید = xstrfmt("submodule.%s.update", sub->name);

	اگر (update) {
		برآمد->type = update;
	} ورنہ اگر (!repo_config_get_string_tmp(r, کلید, &val)) {
		اگر (parse_submodule_update_strategy(val, برآمد) < 0) {
			ret = die_پیغام(_("Invalid update mode '%s' configured for submodule path '%s'"),
					  val, راستہ);
			جاؤ cleanup;
		}
	} ورنہ اگر (sub->update_strategy.type != SM_UPDATE_UNSPECIFIED) {
		اگر (sub->update_strategy.type == SM_UPDATE_COMMAND)
			BUG("how did we read update = !command from .gitmodules?");
		برآمد->type = sub->update_strategy.type;
		برآمد->command = sub->update_strategy.command;
	} ورنہ
		برآمد->type = SM_UPDATE_CHECKOUT;

	اگر (just_cloned &&
	    (برآمد->type == SM_UPDATE_ملاؤ ||
	     برآمد->type == SM_UPDATE_REBASE ||
	     برآمد->type == SM_UPDATE_NONE))
		برآمد->type = SM_UPDATE_CHECKOUT;

	ret = 0;
cleanup:
	آزاد(کلید);
	واپس ret;
}

ساخت update_clone_ڈیٹا {
	ثابت ساخت submodule *sub;
	ساخت object_id oid;
	غیرعلامتی just_cloned;
};

ساخت submodule_update_clone {
	/* index into 'update_data.list', the list of submodules to look into for cloning */
	صحیح current;

	/* configuration parameters which are passed on to the children */
	ثابت ساخت update_ڈیٹا *update_ڈیٹا;

	/* to be consumed by update_submodule() */
	ساخت update_clone_ڈیٹا *update_clone;
	صحیح update_clone_nr; صحیح update_clone_alloc;

	/* If we want to stop as fast as possible and return an error */
	غیرعلامتی quickstop : 1;

	/* failed clones to be retried again */
	ثابت ساخت cache_اندراج **failed_clones;
	صحیح failed_clones_nr, failed_clones_alloc;
};
#تعریف_کرو SUBMODULE_UPDATE_CLONE_INIT { 0 }

ساکن خالی submodule_update_clone_release(ساخت submodule_update_clone *suc)
{
	آزاد(suc->update_clone);
	آزاد(suc->failed_clones);
}

ساخت update_ڈیٹا {
	ثابت حرف *سابقہ;
	ثابت حرف *super_سابقہ;
	حرف *displaypath;
	شمار submodule_update_type update_default;
	ساخت object_id suboid;
	ساخت string_list references;
	ساخت submodule_update_strategy update_strategy;
	ساخت list_objects_filter_options *filter_options;
	ساخت module_list list;
	شمار ref_storage_format ref_storage_format;
	صحیح گہرائی;
	صحیح max_jobs;
	صحیح single_branch;
	صحیح recommend_shallow;
	غیرعلامتی صحیح require_init;
	غیرعلامتی صحیح force;
	غیرعلامتی صحیح quiet;
	غیرعلامتی صحیح nofetch;
	غیرعلامتی صحیح remote;
	غیرعلامتی صحیح پیش_رفت;
	غیرعلامتی صحیح dissociate;
	غیرعلامتی صحیح init;
	غیرعلامتی صحیح warn_if_uninitialized;
	غیرعلامتی صحیح recursive;

	/* copied over from update_clone_data */
	ساخت object_id oid;
	غیرعلامتی صحیح just_cloned;
	ثابت حرف *sm_راستہ;
};
#تعریف_کرو UPDATE_ڈیٹا_INIT { \
	.update_strategy = SUBMODULE_UPDATE_STRATEGY_INIT, \
	.list = MODULE_LIST_INIT, \
	.ref_storage_format = REF_STORAGE_FORMAT_UNKNOWN, \
	.recommend_shallow = -1, \
	.references = STRING_LIST_INIT_DUP, \
	.single_branch = -1, \
	.max_jobs = 1, \
}

ساکن خالی update_ڈیٹا_release(ساخت update_ڈیٹا *ud)
{
	آزاد(ud->displaypath);
	submodule_update_strategy_release(&ud->update_strategy);
	module_list_release(&ud->list);
}

ساکن خالی اگلا_submodule_warn_missing(ساخت submodule_update_clone *suc,
		ساخت strbuf *برآمد, ثابت حرف *displaypath)
{
	/*
	 * Only mention uninitialized submodules when their
	 * paths have been specified.
	 */
	اگر (suc->update_ڈیٹا->warn_if_uninitialized) {
		strbuf_addf(برآمد,
			_("Submodule path '%s' not initialized"),
			displaypath);
		strbuf_addch(برآمد, '\n');
		strbuf_addstr(برآمد,
			_("Maybe you want to use 'update --init'?"));
		strbuf_addch(برآمد, '\n');
	}
}

/**
 * Determine whether 'ce' needs to be cloned. If so, prepare the 'child' to
 * run the clone. Returns 1 if 'ce' needs to be cloned, 0 otherwise.
 */
ساکن صحیح prepare_مقصد_clone_اگلا_submodule(ثابت ساخت cache_اندراج *ce,
					   ساخت بچہ_process *بچہ,
					   ساخت submodule_update_clone *suc,
					   ساخت strbuf *برآمد)
{
	ثابت ساخت submodule *sub = تہی;
	ثابت حرف *url = تہی;
	ثابت حرف *update_string;
	شمار submodule_update_type update_type;
	حرف *کلید;
	ثابت ساخت update_ڈیٹا *ud = suc->update_ڈیٹا;
	حرف *displaypath = get_submodule_displaypath(ce->name, ud->سابقہ,
						      ud->super_سابقہ);
	ساخت strbuf sb = STRBUF_INIT;
	صحیح needs_cloning = 0;
	صحیح need_free_url = 0;

	اگر (ce_stage(ce)) {
		strbuf_addf(برآمد, _("Skipping unmerged submodule %s"), displaypath);
		strbuf_addch(برآمد, '\n');
		جاؤ cleanup;
	}

	sub = submodule_from_راستہ(the_repository, null_oid(the_hash_algo), ce->name);

	اگر (!sub) {
		اگلا_submodule_warn_missing(suc, برآمد, displaypath);
		جاؤ cleanup;
	}

	کلید = xstrfmt("submodule.%s.update", sub->name);
	اگر (!repo_config_get_string_tmp(the_repository, کلید, &update_string)) {
		update_type = parse_submodule_update_type(update_string);
	} ورنہ {
		update_type = sub->update_strategy.type;
	}
	آزاد(کلید);

	اگر (suc->update_ڈیٹا->update_strategy.type == SM_UPDATE_NONE
	    || (suc->update_ڈیٹا->update_strategy.type == SM_UPDATE_UNSPECIFIED
		&& update_type == SM_UPDATE_NONE)) {
		strbuf_addf(برآمد, _("Skipping submodule '%s'"), displaypath);
		strbuf_addch(برآمد, '\n');
		جاؤ cleanup;
	}

	/* Check if the submodule has been initialized. */
	اگر (!is_submodule_فعال(the_repository, ce->name)) {
		اگلا_submodule_warn_missing(suc, برآمد, displaypath);
		جاؤ cleanup;
	}

	strbuf_reset(&sb);
	strbuf_addf(&sb, "submodule.%s.url", sub->name);
	اگر (repo_config_get_string_tmp(the_repository, sb.buf, &url)) {
		اگر (sub->url && (starts_with_dot_slash(sub->url) ||
				 starts_with_dot_dot_slash(sub->url))) {
			url = resolve_relative_url(sub->url, تہی, 0);
			need_free_url = 1;
		} ورنہ
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
		جاؤ cleanup;

	بچہ->git_cmd = 1;
	بچہ->no_stdin = 1;
	بچہ->stdout_مقصد_stderr = 1;
	بچہ->err = -1;
	strvec_ڈالو(&بچہ->دلائل, "submodule--helper");
	strvec_ڈالو(&بچہ->دلائل, "clone");
	اگر (suc->update_ڈیٹا->پیش_رفت)
		strvec_ڈالو(&بچہ->دلائل, "--progress");
	اگر (suc->update_ڈیٹا->quiet)
		strvec_ڈالو(&بچہ->دلائل, "--quiet");
	اگر (suc->update_ڈیٹا->سابقہ)
		strvec_pushl(&بچہ->دلائل, "--prefix", suc->update_ڈیٹا->سابقہ, تہی);
	اگر (suc->update_ڈیٹا->recommend_shallow && sub->recommend_shallow == 1)
		strvec_ڈالو(&بچہ->دلائل, "--depth=1");
	ورنہ اگر (suc->update_ڈیٹا->گہرائی)
		strvec_pushf(&بچہ->دلائل, "--depth=%d", suc->update_ڈیٹا->گہرائی);
	اگر (suc->update_ڈیٹا->filter_options && suc->update_ڈیٹا->filter_options->choice)
		strvec_pushf(&بچہ->دلائل, "--filter=%s",
			     expand_list_objects_filter_spec(suc->update_ڈیٹا->filter_options));
	اگر (suc->update_ڈیٹا->require_init)
		strvec_ڈالو(&بچہ->دلائل, "--require-init");
	اگر (suc->update_ڈیٹا->ref_storage_format != REF_STORAGE_FORMAT_UNKNOWN)
		strvec_pushf(&بچہ->دلائل, "--ref-storage-format=%s",
			     ref_storage_format_مقصد_name(suc->update_ڈیٹا->ref_storage_format));
	strvec_pushl(&بچہ->دلائل, "--path", sub->راستہ, تہی);
	strvec_pushl(&بچہ->دلائل, "--name", sub->name, تہی);
	strvec_pushl(&بچہ->دلائل, "--url", url, تہی);
	اگر (suc->update_ڈیٹا->references.nr) {
		ساخت string_list_شے *شے;

		for_each_string_list_شے(شے, &suc->update_ڈیٹا->references)
			strvec_pushl(&بچہ->دلائل, "--reference", شے->string, تہی);
	}
	اگر (suc->update_ڈیٹا->dissociate)
		strvec_ڈالو(&بچہ->دلائل, "--dissociate");
	اگر (suc->update_ڈیٹا->single_branch >= 0)
		strvec_ڈالو(&بچہ->دلائل, suc->update_ڈیٹا->single_branch ?
					      "--single-branch" :
					      "--no-single-branch");

cleanup:
	آزاد(displaypath);
	strbuf_release(&sb);
	اگر (need_free_url)
		آزاد((خالی*)url);

	واپس needs_cloning;
}

ساکن صحیح update_clone_get_اگلا_task(ساخت بچہ_process *بچہ,
				      ساخت strbuf *err,
				      خالی *suc_cb,
				      خالی **idx_task_cb)
{
	ساخت submodule_update_clone *suc = suc_cb;
	ثابت ساخت cache_اندراج *ce;
	صحیح اشاریہ;

	برائے (; suc->current < suc->update_ڈیٹا->list.nr; suc->current++) {
		ce = suc->update_ڈیٹا->list.entries[suc->current];
		اگر (prepare_مقصد_clone_اگلا_submodule(ce, بچہ, suc, err)) {
			صحیح *p = xmalloc(حجم(*p));

			*p = suc->current;
			*idx_task_cb = p;
			suc->current++;
			واپس 1;
		}
	}

	/*
	 * The loop above tried cloning each submodule once, now try the
	 * stragglers again, which we can imagine as an extension of the
	 * entry list.
	 */
	اشاریہ = suc->current - suc->update_ڈیٹا->list.nr;
	اگر (اشاریہ < suc->failed_clones_nr) {
		صحیح *p;

		ce = suc->failed_clones[اشاریہ];
		اگر (!prepare_مقصد_clone_اگلا_submodule(ce, بچہ, suc, err)) {
			suc->current ++;
			strbuf_addstr(err, "BUG: submodule considered for "
					   "cloning, doesn't need cloning "
					   "any more?\n");
			واپس 0;
		}
		p = xmalloc(حجم(*p));
		*p = suc->current;
		*idx_task_cb = p;
		suc->current ++;
		واپس 1;
	}

	واپس 0;
}

ساکن صحیح update_clone_آغاز_failure(ساخت strbuf *err غیر_مستعمل,
				      خالی *suc_cb,
				      خالی *idx_task_cb غیر_مستعمل)
{
	ساخت submodule_update_clone *suc = suc_cb;

	suc->quickstop = 1;
	واپس 1;
}

ساکن صحیح update_clone_task_ختم_شدہ(صحیح نتیجہ,
				      ساخت strbuf *err,
				      خالی *suc_cb,
				      خالی *idx_task_cb)
{
	ثابت ساخت cache_اندراج *ce;
	ساخت submodule_update_clone *suc = suc_cb;
	صحیح *idxP = idx_task_cb;
	صحیح idx = *idxP;

	آزاد(idxP);

	اگر (!نتیجہ)
		واپس 0;

	اگر (idx < suc->update_ڈیٹا->list.nr) {
		ce  = suc->update_ڈیٹا->list.entries[idx];
		strbuf_addf(err, _("Failed to clone '%s'. Retry scheduled"),
			    ce->name);
		strbuf_addch(err, '\n');
		ALLOC_GROW(suc->failed_clones,
			   suc->failed_clones_nr + 1,
			   suc->failed_clones_alloc);
		suc->failed_clones[suc->failed_clones_nr++] = ce;
		واپس 0;
	} ورنہ {
		idx -= suc->update_ڈیٹا->list.nr;
		ce  = suc->failed_clones[idx];
		strbuf_addf(err, _("Failed to clone '%s' a second time, aborting"),
			    ce->name);
		strbuf_addch(err, '\n');
		suc->quickstop = 1;
		واپس 1;
	}

	واپس 0;
}

ساکن صحیح git_update_clone_config(ثابت حرف *var, ثابت حرف *قدر,
				   ثابت ساخت config_context *ctx,
				   خالی *cb)
{
	صحیح *max_jobs = cb;

	اگر (!strcmp(var, "submodule.fetchjobs"))
		*max_jobs = parse_submodule_fetchjobs(var, قدر, ctx->kvi);
	واپس 0;
}

ساکن صحیح is_tip_reachable(ثابت حرف *راستہ, ثابت ساخت object_id *oid)
{
	ساخت بچہ_process cp = بچہ_PROCESS_INIT;
	ساخت strbuf rev = STRBUF_INIT;
	حرف *hex = oid_مقصد_hex(oid);
	صحیح reachable;

	cp.git_cmd = 1;
	cp.dir = راستہ;
	cp.no_stderr = 1;
	strvec_pushl(&cp.دلائل, "rev-list", "-n", "1", hex, "--not", "--all", تہی);

	prepare_submodule_repo_env(&cp.env);

	اگر (capture_command(&cp, &rev, GIT_MAX_HEXSZ + 1) || rev.len)
		reachable = 0;
	ورنہ
		reachable = 1;

	strbuf_release(&rev);
	واپس reachable;
}

ساکن صحیح fetch_اندر_submodule(ثابت حرف *module_راستہ, صحیح گہرائی, صحیح quiet,
			      ثابت ساخت object_id *oid)
{
	ساخت بچہ_process cp = بچہ_PROCESS_INIT;

	prepare_submodule_repo_env(&cp.env);
	cp.git_cmd = 1;
	cp.dir = module_راستہ;

	strvec_ڈالو(&cp.دلائل, "fetch");
	اگر (quiet)
		strvec_ڈالو(&cp.دلائل, "--quiet");
	اگر (گہرائی)
		strvec_pushf(&cp.دلائل, "--depth=%d", گہرائی);
	اگر (oid) {
		حرف *hex = oid_مقصد_hex(oid);
		حرف *remote;
		صحیح رمز;

		رمز = get_default_remote_submodule(module_راستہ, &remote);
		اگر (رمز) {
			بچہ_process_clear(&cp);
			واپس رمز;
		}

		strvec_pushl(&cp.دلائل, remote, hex, تہی);
		آزاد(remote);
	}

	واپس تسلسل_command(&cp);
}

ساکن صحیح تسلسل_update_command(ثابت ساخت update_ڈیٹا *ud, صحیح subforce)
{
	ساخت بچہ_process cp = بچہ_PROCESS_INIT;
	حرف *oid = oid_مقصد_hex(&ud->oid);
	صحیح ret;

	انتخاب (ud->update_strategy.type) {
	حالت SM_UPDATE_CHECKOUT:
		cp.git_cmd = 1;
		strvec_pushl(&cp.دلائل, "checkout", "-q", تہی);
		اگر (subforce)
			strvec_ڈالو(&cp.دلائل, "-f");
		توڑو;
	حالت SM_UPDATE_REBASE:
		cp.git_cmd = 1;
		strvec_ڈالو(&cp.دلائل, "rebase");
		اگر (ud->quiet)
			strvec_ڈالو(&cp.دلائل, "--quiet");
		توڑو;
	حالت SM_UPDATE_ملاؤ:
		cp.git_cmd = 1;
		strvec_ڈالو(&cp.دلائل, "merge");
		اگر (ud->quiet)
			strvec_ڈالو(&cp.دلائل, "--quiet");
		توڑو;
	حالت SM_UPDATE_COMMAND:
		cp.use_shell = 1;
		strvec_ڈالو(&cp.دلائل, ud->update_strategy.command);
		توڑو;
	طےشدہ:
		BUG("unexpected update strategy type: %d",
		    ud->update_strategy.type);
	}
	strvec_ڈالو(&cp.دلائل, oid);

	cp.dir = ud->sm_راستہ;
	prepare_submodule_repo_env(&cp.env);
	اگر ((ret = تسلسل_command(&cp))) {
		انتخاب (ud->update_strategy.type) {
		حالت SM_UPDATE_CHECKOUT:
			die_پیغام(_("Unable to checkout '%s' in submodule path '%s'"),
				    oid, ud->displaypath);
			/* No "ret" assignment, use "git checkout"'s */
			توڑو;
		حالت SM_UPDATE_REBASE:
			ret = die_پیغام(_("Unable to rebase '%s' in submodule path '%s'"),
					  oid, ud->displaypath);
			توڑو;
		حالت SM_UPDATE_ملاؤ:
			ret = die_پیغام(_("Unable to merge '%s' in submodule path '%s'"),
					  oid, ud->displaypath);
			توڑو;
		حالت SM_UPDATE_COMMAND:
			ret = die_پیغام(_("Execution of '%s %s' failed in submodule path '%s'"),
					  ud->update_strategy.command, oid, ud->displaypath);
			توڑو;
		طےشدہ:
			BUG("unexpected update strategy type: %d",
			    ud->update_strategy.type);
		}

		واپس ret;
	}

	اگر (ud->quiet)
		واپس 0;

	انتخاب (ud->update_strategy.type) {
	حالت SM_UPDATE_CHECKOUT:
		لکھو(_("Submodule path '%s': checked out '%s'\n"),
		       ud->displaypath, oid);
		توڑو;
	حالت SM_UPDATE_REBASE:
		لکھو(_("Submodule path '%s': rebased into '%s'\n"),
		       ud->displaypath, oid);
		توڑو;
	حالت SM_UPDATE_ملاؤ:
		لکھو(_("Submodule path '%s': merged in '%s'\n"),
		       ud->displaypath, oid);
		توڑو;
	حالت SM_UPDATE_COMMAND:
		لکھو(_("Submodule path '%s': '%s %s'\n"),
		       ud->displaypath, ud->update_strategy.command, oid);
		توڑو;
	طےشدہ:
		BUG("unexpected update strategy type: %d",
		    ud->update_strategy.type);
	}

	واپس 0;
}

ساکن صحیح تسلسل_update_procedure(ثابت ساخت update_ڈیٹا *ud)
{
	صحیح subforce = is_null_oid(&ud->suboid) || ud->force;

	اگر (!ud->nofetch) {
		/*
		 * Run fetch only if `oid` isn't present or it
		 * is not reachable from a ref.
		 */
		اگر (!is_tip_reachable(ud->sm_راستہ, &ud->oid) &&
		    fetch_اندر_submodule(ud->sm_راستہ, ud->گہرائی, ud->quiet, تہی) &&
		    !ud->quiet)
			fprintf_ln(stderr,
				   _("Unable to fetch in submodule path '%s'; "
				     "trying to directly fetch %s:"),
				   ud->displaypath, oid_مقصد_hex(&ud->oid));
		/*
		 * Now we tried the usual fetch, but `oid` may
		 * not be reachable from any of the refs.
		 */
		اگر (!is_tip_reachable(ud->sm_راستہ, &ud->oid) &&
		    fetch_اندر_submodule(ud->sm_راستہ, ud->گہرائی, ud->quiet, &ud->oid))
			واپس die_پیغام(_("Fetched in submodule path '%s', but it did not "
					     "contain %s. Direct fetching of that commit failed."),
					   ud->displaypath, oid_مقصد_hex(&ud->oid));
	}

	واپس تسلسل_update_command(ud, subforce);
}

ساکن صحیح remote_submodule_branch(ثابت حرف *راستہ, ثابت حرف **branch)
{
	ثابت ساخت submodule *sub;
	حرف *کلید;
	*branch = تہی;

	sub = submodule_from_راستہ(the_repository, null_oid(the_hash_algo), راستہ);
	اگر (!sub)
		واپس die_پیغام(_("could not initialize submodule at path '%s'"),
				   راستہ);

	کلید = xstrfmt("submodule.%s.branch", sub->name);
	اگر (repo_config_get_string_tmp(the_repository, کلید, branch))
		*branch = sub->branch;
	آزاد(کلید);

	اگر (!*branch) {
		*branch = "HEAD";
		واپس 0;
	}

	اگر (!strcmp(*branch, ".")) {
		ثابت حرف *refname = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
							      "HEAD", 0, تہی,
							      تہی);

		اگر (!refname)
			واپس die_پیغام(_("No such ref: %s"), "HEAD");

		/* detached HEAD */
		اگر (!strcmp(refname, "HEAD"))
			واپس die_پیغام(_("Submodule (%s) branch configured to inherit "
					     "branch from superproject, but the superproject "
					     "is not on any branch"), sub->name);

		اگر (!skip_سابقہ(refname, "refs/heads/", &refname))
			واپس die_پیغام(_("Expecting a full ref name, got %s"),
					   refname);

		*branch = refname;
		واپس 0;
	}

	/* Our "branch" is coming from repo_config_get_string_tmp() */
	واپس 0;
}

ساکن صحیح ensure_core_worktree(ثابت حرف *راستہ)
{
	ثابت حرف *cw;
	ساخت repository subrepo;

	اگر (repo_submodule_init(&subrepo, the_repository, راستہ, null_oid(the_hash_algo)))
		واپس die_پیغام(_("could not get a repository handle for submodule '%s'"),
				   راستہ);

	اگر (!repo_config_get_string_tmp(&subrepo, "core.worktree", &cw)) {
		حرف *cfg_file, *abs_راستہ;
		ثابت حرف *rel_راستہ;
		ساخت strbuf sb = STRBUF_INIT;

		cfg_file = repo_git_راستہ(&subrepo, "config");

		abs_راستہ = absolute_pathdup(راستہ);
		rel_راستہ = relative_راستہ(abs_راستہ, subrepo.gitdir, &sb);

		repo_config_سیٹ_اندر_file(the_repository, cfg_file, "core.worktree", rel_راستہ);

		آزاد(cfg_file);
		آزاد(abs_راستہ);
		strbuf_release(&sb);
	}

	repo_clear(&subrepo);
	واپس 0;
}

ساکن ثابت حرف *submodule_update_type_مقصد_label(شمار submodule_update_type type)
{
	انتخاب (type) {
	حالت SM_UPDATE_CHECKOUT:
		واپس "checkout";
	حالت SM_UPDATE_ملاؤ:
		واپس "merge";
	حالت SM_UPDATE_REBASE:
		واپس "rebase";
	حالت SM_UPDATE_UNSPECIFIED:
	حالت SM_UPDATE_NONE:
	حالت SM_UPDATE_COMMAND:
		توڑو;
	}
	BUG("unreachable with type %d", type);
}

ساکن خالی update_ڈیٹا_مقصد_دلائل(ثابت ساخت update_ڈیٹا *update_ڈیٹا,
				ساخت strvec *دلائل)
{
	شمار submodule_update_type update_type = update_ڈیٹا->update_default;

	strvec_pushl(دلائل, "submodule--helper", "update", "--recursive", تہی);
	اگر (update_ڈیٹا->displaypath)
		strvec_pushf(دلائل, "--super-prefix=%s/",
			     update_ڈیٹا->displaypath);
	strvec_pushf(دلائل, "--jobs=%d", update_ڈیٹا->max_jobs);
	اگر (update_ڈیٹا->quiet)
		strvec_ڈالو(دلائل, "--quiet");
	اگر (update_ڈیٹا->force)
		strvec_ڈالو(دلائل, "--force");
	اگر (update_ڈیٹا->init)
		strvec_ڈالو(دلائل, "--init");
	اگر (update_ڈیٹا->remote)
		strvec_ڈالو(دلائل, "--remote");
	اگر (update_ڈیٹا->nofetch)
		strvec_ڈالو(دلائل, "--no-fetch");
	اگر (update_ڈیٹا->dissociate)
		strvec_ڈالو(دلائل, "--dissociate");
	اگر (update_ڈیٹا->پیش_رفت)
		strvec_ڈالو(دلائل, "--progress");
	اگر (update_ڈیٹا->require_init)
		strvec_ڈالو(دلائل, "--require-init");
	اگر (update_ڈیٹا->گہرائی)
		strvec_pushf(دلائل, "--depth=%d", update_ڈیٹا->گہرائی);
	اگر (update_type != SM_UPDATE_UNSPECIFIED)
		strvec_pushf(دلائل, "--%s",
			     submodule_update_type_مقصد_label(update_type));

	اگر (update_ڈیٹا->references.nr) {
		ساخت string_list_شے *شے;

		for_each_string_list_شے(شے, &update_ڈیٹا->references)
			strvec_pushl(دلائل, "--reference", شے->string, تہی);
	}
	اگر (update_ڈیٹا->ref_storage_format != REF_STORAGE_FORMAT_UNKNOWN)
		strvec_pushf(دلائل, "--ref-storage-format=%s",
			     ref_storage_format_مقصد_name(update_ڈیٹا->ref_storage_format));
	اگر (update_ڈیٹا->filter_options && update_ڈیٹا->filter_options->choice)
		strvec_pushf(دلائل, "--filter=%s",
				expand_list_objects_filter_spec(
					update_ڈیٹا->filter_options));
	اگر (update_ڈیٹا->recommend_shallow == 0)
		strvec_ڈالو(دلائل, "--no-recommend-shallow");
	ورنہ اگر (update_ڈیٹا->recommend_shallow == 1)
		strvec_ڈالو(دلائل, "--recommend-shallow");
	اگر (update_ڈیٹا->single_branch >= 0)
		strvec_ڈالو(دلائل, update_ڈیٹا->single_branch ?
				    "--single-branch" :
				    "--no-single-branch");
}

ساکن صحیح update_submodule(ساخت update_ڈیٹا *update_ڈیٹا)
{
	صحیح ret;

	اگر (validate_submodule_راستہ(update_ڈیٹا->sm_راستہ) < 0)
		واپس -1;

	ret = determine_submodule_update_strategy(the_repository,
						  update_ڈیٹا->just_cloned,
						  update_ڈیٹا->sm_راستہ,
						  update_ڈیٹا->update_default,
						  &update_ڈیٹا->update_strategy);
	اگر (ret)
		واپس ret;

	اگر (update_ڈیٹا->just_cloned)
		oidcpy(&update_ڈیٹا->suboid, null_oid(the_hash_algo));
	ورنہ اگر (repo_resolve_gitlink_ref(the_repository, update_ڈیٹا->sm_راستہ,
					  "HEAD", &update_ڈیٹا->suboid))
		واپس die_پیغام(_("Unable to find current revision in submodule path '%s'"),
				   update_ڈیٹا->displaypath);

	اگر (update_ڈیٹا->remote) {
		حرف *remote_name;
		ثابت حرف *branch;
		حرف *remote_ref;
		صحیح رمز;

		رمز = get_default_remote_submodule(update_ڈیٹا->sm_راستہ, &remote_name);
		اگر (رمز)
			واپس رمز;
		رمز = remote_submodule_branch(update_ڈیٹا->sm_راستہ, &branch);
		اگر (رمز) {
			آزاد(remote_name);
			واپس رمز;
		}
		remote_ref = xstrfmt("refs/remotes/%s/%s", remote_name, branch);

		آزاد(remote_name);

		اگر (!update_ڈیٹا->nofetch) {
			اگر (fetch_اندر_submodule(update_ڈیٹا->sm_راستہ, update_ڈیٹا->گہرائی,
					      0, تہی)) {
				آزاد(remote_ref);
				واپس die_پیغام(_("Unable to fetch in submodule path '%s'"),
						   update_ڈیٹا->sm_راستہ);
			}
		}

		اگر (repo_resolve_gitlink_ref(the_repository, update_ڈیٹا->sm_راستہ,
					     remote_ref, &update_ڈیٹا->oid)) {
			ret = die_پیغام(_("Unable to find %s revision in submodule path '%s'"),
					  remote_ref, update_ڈیٹا->sm_راستہ);
			آزاد(remote_ref);
			واپس ret;
		}

		آزاد(remote_ref);
	}

	اگر (!oideq(&update_ڈیٹا->oid, &update_ڈیٹا->suboid) || update_ڈیٹا->force) {
		ret = تسلسل_update_procedure(update_ڈیٹا);
		اگر (ret)
			واپس ret;
	}

	اگر (update_ڈیٹا->recursive) {
		ساخت بچہ_process cp = بچہ_PROCESS_INIT;
		ساخت update_ڈیٹا اگلا = *update_ڈیٹا;

		اگلا.سابقہ = تہی;
		oidcpy(&اگلا.oid, null_oid(the_hash_algo));
		oidcpy(&اگلا.suboid, null_oid(the_hash_algo));

		cp.dir = update_ڈیٹا->sm_راستہ;
		cp.git_cmd = 1;
		prepare_submodule_repo_env(&cp.env);
		update_ڈیٹا_مقصد_دلائل(&اگلا, &cp.دلائل);

		ret = تسلسل_command(&cp);
		اگر (ret)
			die_پیغام(_("Failed to recurse into submodule path '%s'"),
				    update_ڈیٹا->displaypath);
		واپس ret;
	}

	واپس 0;
}

ساکن صحیح update_submodules(ساخت update_ڈیٹا *update_ڈیٹا)
{
	صحیح i, ret = 0;
	ساخت submodule_update_clone suc = SUBMODULE_UPDATE_CLONE_INIT;
	ثابت ساخت تسلسل_process_parallel_opts opts = {
		.tr2_category = "submodule",
		.tr2_label = "parallel/update",

		.processes = update_ڈیٹا->max_jobs,

		.get_اگلا_task = update_clone_get_اگلا_task,
		.آغاز_failure = update_clone_آغاز_failure,
		.task_ختم_شدہ = update_clone_task_ختم_شدہ,
		.ڈیٹا = &suc,
	};

	suc.update_ڈیٹا = update_ڈیٹا;
	تسلسل_processes_parallel(&opts);

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
		جاؤ cleanup;
	}

	برائے (i = 0; i < suc.update_clone_nr; i++) {
		ساخت update_clone_ڈیٹا ucd = suc.update_clone[i];
		صحیح رمز = 128;

		oidcpy(&update_ڈیٹا->oid, &ucd.oid);
		update_ڈیٹا->just_cloned = ucd.just_cloned;
		update_ڈیٹا->sm_راستہ = ucd.sub->راستہ;

		/*
		 * Verify that the submodule path does not contain any
		 * symlinks; if it does, it might have been tampered with.
		 * TODO: allow exempting it via
		 * `safe.submodule.path` or something
		 */
		اگر (validate_submodule_راستہ(update_ڈیٹا->sm_راستہ) < 0)
			جاؤ fail;

		رمز = ensure_core_worktree(update_ڈیٹا->sm_راستہ);
		اگر (رمز)
			جاؤ fail;

		update_ڈیٹا->displaypath = get_submodule_displaypath(
			update_ڈیٹا->sm_راستہ, update_ڈیٹا->سابقہ,
			update_ڈیٹا->super_سابقہ);
		رمز = update_submodule(update_ڈیٹا);
		FREE_AND_NULL(update_ڈیٹا->displaypath);
fail:
		اگر (!رمز)
			جاری;
		ret = رمز;
		اگر (ret == 128)
			جاؤ cleanup;
	}

cleanup:
	submodule_update_clone_release(&suc);
	string_list_clear(&update_ڈیٹا->references, 0);
	واپس ret;
}

ساکن صحیح module_update(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
			 ساخت repository *repo غیر_مستعمل)
{
	ساخت pathspec pathspec = { 0 };
	ساخت pathspec pathspec2 = { 0 };
	ساخت update_ڈیٹا opt = UPDATE_ڈیٹا_INIT;
	ساخت list_objects_filter_options filter_options =
		LIST_OBJECTS_FILTER_INIT;
	ثابت حرف *ref_storage_format = تہی;
	صحیح ret;
	ساخت option module_update_options[] = {
		OPT_SUPER_سابقہ(&opt.super_سابقہ),
		OPT__FORCE(&opt.force, N_("force checkout updates"), 0),
		OPT_BOOL('i', "init", &opt.init,
			 N_("initialize uninitialized submodules before update")),
		OPT_BOOL(0, "remote", &opt.remote,
			 N_("use SHA-1 of submodule's remote tracking branch")),
		OPT_BOOL(0, "recursive", &opt.recursive,
			 N_("traverse submodules recursively")),
		OPT_BOOL('N', "no-fetch", &opt.nofetch,
			 N_("don't fetch new objects from the remote site")),
		OPT_سیٹ_INT(0, "checkout", &opt.update_default,
			N_("use the 'checkout' update strategy (default)"),
			SM_UPDATE_CHECKOUT),
		OPT_سیٹ_INT('m', "merge", &opt.update_default,
			N_("use the 'merge' update strategy"),
			SM_UPDATE_ملاؤ),
		OPT_سیٹ_INT('r', "rebase", &opt.update_default,
			N_("use the 'rebase' update strategy"),
			SM_UPDATE_REBASE),
		OPT_STRING_LIST(0, "reference", &opt.references, N_("repo"),
			   N_("reference repository")),
		OPT_STRING(0, "ref-storage-format", &ref_storage_format, N_("format"),
			   N_("specify the reference storage format to use")),
		OPT_ALIAS_F(0, "ref-format", "ref-storage-format", PARSE_OPT_HIDDEN),
		OPT_BOOL(0, "dissociate", &opt.dissociate,
			   N_("use --reference only while cloning")),
		OPT_INTEGER(0, "depth", &opt.گہرائی,
			   N_("create a shallow clone truncated to the "
			      "specified number of revisions")),
		OPT_INTEGER('j', "jobs", &opt.max_jobs,
			    N_("parallel jobs")),
		OPT_BOOL(0, "recommend-shallow", &opt.recommend_shallow,
			    N_("whether the initial clone should follow the shallow recommendation")),
		OPT__QUIET(&opt.quiet, N_("don't print cloning progress")),
		OPT_BOOL(0, "progress", &opt.پیش_رفت,
			    N_("force cloning progress")),
		OPT_BOOL(0, "require-init", &opt.require_init,
			   N_("disallow cloning into non-empty directory, implies --init")),
		OPT_BOOL(0, "single-branch", &opt.single_branch,
			 N_("clone only one branch, HEAD or --branch")),
		OPT_PARSE_LIST_OBJECTS_FILTER(&filter_options),
		OPT_انتہا()
	};
	ثابت حرف *ثابت git_submodule_helper_usage[] = {
		N_("git submodule [--quiet] update"
		" [--init [--filter=<filter-spec>]] [--remote]"
		" [-N|--no-fetch] [-f|--force]"
		" [--checkout|--merge|--rebase]"
		" [--[no-]recommend-shallow] [--reference <repository>]"
		" [--recursive] [--[no-]single-branch] [--] [<path>...]"),
		تہی
	};

	update_clone_config_from_gitmodules(the_repository, &opt.max_jobs);
	repo_config(the_repository, git_update_clone_config, &opt.max_jobs);

	argc = parse_options(argc, argv, سابقہ, module_update_options,
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
	opt.سابقہ = سابقہ;

	اگر (opt.update_default)
		opt.update_strategy.type = opt.update_default;

	اگر (module_list_compute(argv, سابقہ, &pathspec, &opt.list) < 0) {
		ret = 1;
		جاؤ cleanup;
	}

	اگر (pathspec.nr)
		opt.warn_if_uninitialized = 1;

	اگر (opt.init) {
		ساخت module_list list = MODULE_LIST_INIT;
		ساخت init_cb info = INIT_CB_INIT;

		اگر (module_list_compute(argv, opt.سابقہ,
					&pathspec2, &list) < 0) {
			module_list_release(&list);
			ret = 1;
			جاؤ cleanup;
		}

		/*
		 * If there are no path args and submodule.active is set then,
		 * by default, only initialize 'active' modules.
		 */
		اگر (!argc && !repo_config_get(the_repository, "submodule.active"))
			module_list_فعال(&list);

		info.سابقہ = opt.سابقہ;
		info.super_سابقہ = opt.super_سابقہ;
		اگر (opt.quiet)
			info.flags |= OPT_QUIET;

		for_each_listed_submodule(&list, init_submodule_cb, &info);
		module_list_release(&list);
	}

	ret = update_submodules(&opt);
cleanup:
	update_ڈیٹا_release(&opt);
	list_objects_filter_release(&filter_options);
	clear_pathspec(&pathspec);
	clear_pathspec(&pathspec2);
	واپس ret;
}

ساکن صحیح ڈالو_check(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ غیر_مستعمل,
		      ساخت repository *repo غیر_مستعمل)
{
	ساخت remote *remote;
	ثابت حرف *superproject_سر;
	حرف *سر;
	صحیح detached_سر = 0;
	ساخت object_id سر_oid;

	اگر (argc < 3)
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
				   0, &سر_oid, تہی);
	اگر (!سر)
		die(_("Failed to resolve HEAD as a valid ref."));
	اگر (!strcmp(سر, "HEAD"))
		detached_سر = 1;

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
		ساخت ref *local_refs = get_local_heads();
		ساخت refspec refspec = REFSPEC_INIT_ڈالو(the_hash_algo);

		refspec_appendn(&refspec, argv + 2, argc - 2);

		برائے (i = 0; i < refspec.nr; i++) {
			ثابت ساخت refspec_شے *rs = &refspec.اشیاء[i];

			اگر (rs->نمونہ || rs->matching)
				جاری;

			/* LHS must match a single ref */
			انتخاب (گنتی_refspec_مطابقت(rs->src, local_refs, تہی)) {
			حالت 1:
				توڑو;
			حالت 0:
				/*
				 * If LHS matches 'HEAD' then we need to ensure
				 * that it matches the same named branch
				 * checked out in the superproject.
				 */
				اگر (!strcmp(rs->src, "HEAD")) {
					اگر (!detached_سر &&
					    !strcmp(سر, superproject_سر))
						توڑو;
					die("HEAD does not match the named branch in the superproject");
				}
				/* fallthrough */
			طےشدہ:
				die("src refspec '%s' must name a ref",
				    rs->src);
			}
		}

		refspec_clear(&refspec);
		free_refs(local_refs);
	}
	آزاد(سر);

	واپس 0;
}

ساکن صحیح absorb_git_dirs(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
			   ساخت repository *repo غیر_مستعمل)
{
	صحیح i;
	ساخت pathspec pathspec = { 0 };
	ساخت module_list list = MODULE_LIST_INIT;
	ثابت حرف *super_سابقہ = تہی;
	ساخت option embed_gitdir_options[] = {
		OPT_SUPER_سابقہ(&super_سابقہ),
		OPT_انتہا()
	};
	ثابت حرف *ثابت git_submodule_helper_usage[] = {
		N_("git submodule absorbgitdirs [<options>] [<path>...]"),
		تہی
	};
	صحیح ret = 1;

	argc = parse_options(argc, argv, سابقہ, embed_gitdir_options,
			     git_submodule_helper_usage, 0);

	اگر (module_list_compute(argv, سابقہ, &pathspec, &list) < 0)
		جاؤ cleanup;

	برائے (i = 0; i < list.nr; i++)
		absorb_git_dir_into_superproject(list.entries[i]->name,
						 super_سابقہ);

	ret = 0;
cleanup:
	clear_pathspec(&pathspec);
	module_list_release(&list);
	واپس ret;
}

ساکن صحیح module_سیٹ_url(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
			  ساخت repository *repo غیر_مستعمل)
{
	صحیح quiet = 0, ret;
	ثابت حرف *newurl;
	ثابت حرف *راستہ;
	حرف *config_name;
	ساخت option options[] = {
		OPT__QUIET(&quiet, N_("suppress output for setting url of a submodule")),
		OPT_انتہا()
	};
	ثابت حرف *ثابت usage[] = {
		N_("git submodule set-url [--quiet] <path> <newurl>"),
		تہی
	};
	ثابت ساخت submodule *sub;

	argc = parse_options(argc, argv, سابقہ, options, usage, 0);

	اگر (argc != 2 || !(راستہ = argv[0]) || !(newurl = argv[1]))
		usage_with_options(usage, options);

	sub = submodule_from_راستہ(the_repository, null_oid(the_hash_algo), راستہ);

	اگر (!sub)
		die(_("no submodule mapping found in .gitmodules for path '%s'"),
		    راستہ);

	config_name = xstrfmt("submodule.%s.url", sub->name);
	ret = config_سیٹ_اندر_gitmodules_file_gently(the_repository, config_name, newurl);

	اگر (!ret) {
		repo_read_gitmodules(the_repository, 0);
		sync_submodule(sub->راستہ, سابقہ, تہی, quiet ? OPT_QUIET : 0);
	}

	آزاد(config_name);
	واپس !!ret;
}

ساکن صحیح module_سیٹ_branch(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
			     ساخت repository *repo غیر_مستعمل)
{
	صحیح opt_default = 0, ret;
	ثابت حرف *opt_branch = تہی;
	ثابت حرف *راستہ;
	حرف *config_name;
	ساخت option options[] = {
		/*
		 * We accept the `quiet` option for uniformity across subcommands,
		 * though there is nothing to make less verbose in this subcommand.
		 */
		OPT_NOOP_NOARG('q', "quiet"),

		OPT_BOOL('d', "default", &opt_default,
			N_("set the default tracking branch to master")),
		OPT_STRING('b', "branch", &opt_branch, N_("branch"),
			N_("set the default tracking branch")),
		OPT_انتہا()
	};
	ثابت حرف *ثابت usage[] = {
		N_("git submodule set-branch [-q|--quiet] (-d|--default) <path>"),
		N_("git submodule set-branch [-q|--quiet] (-b|--branch) <branch> <path>"),
		تہی
	};
	ثابت ساخت submodule *sub;

	argc = parse_options(argc, argv, سابقہ, options, usage, 0);

	اگر (!opt_branch && !opt_default)
		die(_("--branch or --default required"));

	اگر (opt_branch && opt_default)
		die(_("options '%s' and '%s' cannot be used together"), "--branch", "--default");

	اگر (argc != 1 || !(راستہ = argv[0]))
		usage_with_options(usage, options);

	sub = submodule_from_راستہ(the_repository, null_oid(the_hash_algo), راستہ);

	اگر (!sub)
		die(_("no submodule mapping found in .gitmodules for path '%s'"),
		    راستہ);

	config_name = xstrfmt("submodule.%s.branch", sub->name);
	ret = config_سیٹ_اندر_gitmodules_file_gently(the_repository, config_name, opt_branch);

	آزاد(config_name);
	واپس !!ret;
}

ساکن صحیح module_create_branch(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
				ساخت repository *repo غیر_مستعمل)
{
	شمار branch_track track;
	صحیح quiet = 0, force = 0, reflog = 0, dry_تسلسل = 0;
	ساخت option options[] = {
		OPT__QUIET(&quiet, N_("print only error messages")),
		OPT__FORCE(&force, N_("force creation"), 0),
		OPT_BOOL(0, "create-reflog", &reflog,
			 N_("create the branch's reflog")),
		OPT_CALLBACK_F('t', "track",  &track, "(direct|inherit)",
			N_("set branch tracking configuration"),
			PARSE_OPT_OPTARG,
			parse_opt_tracking_منوال),
		OPT_DRY_تسلسل(&dry_تسلسل,
			     N_("show whether the branch would be created")),
		OPT_انتہا()
	};
	ثابت حرف *ثابت usage[] = {
		N_("git submodule--helper create-branch [-f|--force] [--create-reflog] [-q|--quiet] [-t|--track] [-n|--dry-run] <name> <start-oid> <start-name>"),
		تہی
	};
	ساخت repo_config_اقدار *cfg = repo_config_اقدار(the_repository);

	repo_config(the_repository, git_default_config, تہی);
	track = cfg->branch_track;
	argc = parse_options(argc, argv, سابقہ, options, usage, 0);

	اگر (argc != 3)
		usage_with_options(usage, options);

	اگر (!quiet && !dry_تسلسل)
		printf_ln(_("creating branch '%s'"), argv[0]);

	create_branches_recursively(the_repository, argv[0], argv[1], argv[2],
				    force, reflog, quiet, track, dry_تسلسل);
	واپس 0;
}

ساخت جوڑو_ڈیٹا {
	ثابت حرف *سابقہ;
	ثابت حرف *branch;
	ثابت حرف *reference_راستہ;
	حرف *sm_راستہ;
	ثابت حرف *sm_name;
	ثابت حرف *repo;
	ثابت حرف *realrepo;
	شمار ref_storage_format ref_storage_format;
	صحیح گہرائی;
	غیرعلامتی صحیح force: 1;
	غیرعلامتی صحیح quiet: 1;
	غیرعلامتی صحیح پیش_رفت: 1;
	غیرعلامتی صحیح dissociate: 1;
};
#تعریف_کرو جوڑو_ڈیٹا_INIT { \
	.گہرائی = -1, \
	.ref_storage_format = REF_STORAGE_FORMAT_UNKNOWN, \
}

ساکن خالی append_fetch_remotes(ساخت strbuf *پیغام, ثابت حرف *git_dir_راستہ)
{
	ساخت بچہ_process cp_remote = بچہ_PROCESS_INIT;
	ساخت strbuf sb_remote_برآمد = STRBUF_INIT;

	cp_remote.git_cmd = 1;
	strvec_pushf(&cp_remote.env,
		     "GIT_DIR=%s", git_dir_راستہ);
	strvec_ڈالو(&cp_remote.env, "GIT_WORK_TREE=.");
	strvec_pushl(&cp_remote.دلائل, "remote", "-v", تہی);
	اگر (!capture_command(&cp_remote, &sb_remote_برآمد, 0)) {
		حرف *اگلا_سطر;
		حرف *سطر = sb_remote_برآمد.buf;

		جب ((اگلا_سطر = strchr(سطر, '\n')) != تہی) {
			سائز_t len = اگلا_سطر - سطر;

			اگر (strip_suffix_mem(سطر, &len, " (fetch)"))
				strbuf_addf(پیغام, "  %.*s\n", (صحیح)len, سطر);
			سطر = اگلا_سطر + 1;
		}
	}

	strbuf_release(&sb_remote_برآمد);
}

ساکن صحیح جوڑو_submodule(ثابت ساخت جوڑو_ڈیٹا *جوڑو_ڈیٹا)
{
	ساخت module_clone_ڈیٹا clone_ڈیٹا = MODULE_CLONE_ڈیٹا_INIT;
	ساخت string_list reference = STRING_LIST_INIT_NODUP;
	صحیح ret = -1;

	/* perhaps the path already exists and is already a git repo, else clone it */
	اگر (is_directory(جوڑو_ڈیٹا->sm_راستہ)) {
		حرف *submod_gitdir_راستہ;
		ساخت strbuf sm_راستہ = STRBUF_INIT;
		strbuf_addstr(&sm_راستہ, جوڑو_ڈیٹا->sm_راستہ);
		submod_gitdir_راستہ = xstrfmt("%s/.git", جوڑو_ڈیٹا->sm_راستہ);
		اگر (is_nonbare_repository_dir(&sm_راستہ))
			لکھو(_("Adding existing repo at '%s' to the index\n"),
			       جوڑو_ڈیٹا->sm_راستہ);
		ورنہ
			die(_("'%s' already exists and is not a valid git repo"),
			    جوڑو_ڈیٹا->sm_راستہ);
		strbuf_release(&sm_راستہ);
		آزاد(submod_gitdir_راستہ);
	} ورنہ {
		ساخت بچہ_process cp = بچہ_PROCESS_INIT;
		ساخت strbuf submod_gitdir = STRBUF_INIT;

		submodule_name_مقصد_gitdir(&submod_gitdir, the_repository, جوڑو_ڈیٹا->sm_name);

		اگر (is_directory(submod_gitdir.buf)) {
			اگر (!جوڑو_ڈیٹا->force) {
				ساخت strbuf پیغام = STRBUF_INIT;
				حرف *die_پیغام_2;

				strbuf_addf(&پیغام, _("A git directory for '%s' is found "
						    "locally with remote(s):\n"),
					    جوڑو_ڈیٹا->sm_name);

				append_fetch_remotes(&پیغام, submod_gitdir.buf);
				strbuf_release(&submod_gitdir);

				strbuf_addf(&پیغام, _("If you want to reuse this local git "
						    "directory instead of cloning again from\n"
						    "  %s\n"
						    "use the '--force' option. If the local git "
						    "directory is not the correct repo\n"
						    "or you are unsure what this means choose "
						    "another name with the '--name' option."),
					    جوڑو_ڈیٹا->realrepo);

				die_پیغام_2 = strbuf_detach(&پیغام, تہی);
				die("%s", die_پیغام_2);
			} ورنہ {
				لکھو(_("Reactivating local git directory for "
					 "submodule '%s'\n"), جوڑو_ڈیٹا->sm_name);
			}
		}
		strbuf_release(&submod_gitdir);

		clone_ڈیٹا.سابقہ = جوڑو_ڈیٹا->سابقہ;
		clone_ڈیٹا.راستہ = جوڑو_ڈیٹا->sm_راستہ;
		clone_ڈیٹا.name = جوڑو_ڈیٹا->sm_name;
		clone_ڈیٹا.url = جوڑو_ڈیٹا->realrepo;
		clone_ڈیٹا.quiet = جوڑو_ڈیٹا->quiet;
		clone_ڈیٹا.پیش_رفت = جوڑو_ڈیٹا->پیش_رفت;
		اگر (جوڑو_ڈیٹا->reference_راستہ) {
			حرف *p = xstrdup(جوڑو_ڈیٹا->reference_راستہ);

			string_list_append(&reference, p)->util = p;
		}
		clone_ڈیٹا.ref_storage_format = جوڑو_ڈیٹا->ref_storage_format;
		clone_ڈیٹا.dissociate = جوڑو_ڈیٹا->dissociate;
		اگر (جوڑو_ڈیٹا->گہرائی >= 0)
			clone_ڈیٹا.گہرائی = جوڑو_ڈیٹا->گہرائی;

		اگر (clone_submodule(&clone_ڈیٹا, &reference))
			جاؤ cleanup;

		prepare_submodule_repo_env(&cp.env);
		cp.git_cmd = 1;
		cp.dir = جوڑو_ڈیٹا->sm_راستہ;
		/*
		 * NOTE: we only get here if add_data->force is true, so
		 * passing --force to checkout is reasonable.
		 */
		strvec_pushl(&cp.دلائل, "checkout", "-f", "-q", تہی);

		اگر (جوڑو_ڈیٹا->branch) {
			strvec_pushl(&cp.دلائل, "-B", جوڑو_ڈیٹا->branch, تہی);
			strvec_pushf(&cp.دلائل, "origin/%s", جوڑو_ڈیٹا->branch);
		}

		اگر (تسلسل_command(&cp))
			die(_("unable to checkout submodule '%s'"), جوڑو_ڈیٹا->sm_راستہ);
	}
	ret = 0;

cleanup:
	string_list_clear(&reference, 1);
	واپس ret;
}

ساکن صحیح config_submodule_اندر_gitmodules(ثابت حرف *name, ثابت حرف *var, ثابت حرف *قدر)
{
	حرف *کلید;
	صحیح ret;

	اگر (!is_writing_gitmodules_ok())
		die(_("please make sure that the .gitmodules file is in the working tree"));

	کلید = xstrfmt("submodule.%s.%s", name, var);
	ret = config_سیٹ_اندر_gitmodules_file_gently(the_repository, کلید, قدر);
	آزاد(کلید);

	واپس ret;
}

ساکن خالی configure_added_submodule(ساخت جوڑو_ڈیٹا *جوڑو_ڈیٹا)
{
	حرف *کلید;
	ساخت بچہ_process جوڑو_submod = بچہ_PROCESS_INIT;
	ساخت بچہ_process جوڑو_gitmodules = بچہ_PROCESS_INIT;
	ثابت ساخت string_list *اقدار;
	صحیح matched = 0;

	کلید = xstrfmt("submodule.%s.url", جوڑو_ڈیٹا->sm_name);
	repo_config_سیٹ_gently(the_repository, کلید, جوڑو_ڈیٹا->realrepo);
	آزاد(کلید);

	جوڑو_submod.git_cmd = 1;
	strvec_pushl(&جوڑو_submod.دلائل, "add",
		     "--no-warn-embedded-repo", تہی);
	اگر (جوڑو_ڈیٹا->force)
		strvec_ڈالو(&جوڑو_submod.دلائل, "--force");
	strvec_pushl(&جوڑو_submod.دلائل, "--", جوڑو_ڈیٹا->sm_راستہ, تہی);

	اگر (تسلسل_command(&جوڑو_submod))
		die(_("Failed to add submodule '%s'"), جوڑو_ڈیٹا->sm_راستہ);

	اگر (config_submodule_اندر_gitmodules(جوڑو_ڈیٹا->sm_name, "path", جوڑو_ڈیٹا->sm_راستہ) ||
	    config_submodule_اندر_gitmodules(جوڑو_ڈیٹا->sm_name, "url", جوڑو_ڈیٹا->repo))
		die(_("Failed to register submodule '%s'"), جوڑو_ڈیٹا->sm_راستہ);

	اگر (جوڑو_ڈیٹا->branch) {
		اگر (config_submodule_اندر_gitmodules(جوڑو_ڈیٹا->sm_name,
						   "branch", جوڑو_ڈیٹا->branch))
			die(_("Failed to register submodule '%s'"), جوڑو_ڈیٹا->sm_راستہ);
	}

	جوڑو_gitmodules.git_cmd = 1;
	strvec_pushl(&جوڑو_gitmodules.دلائل,
		     "add", "--force", "--", ".gitmodules", تہی);

	اگر (تسلسل_command(&جوڑو_gitmodules))
		die(_("Failed to register submodule '%s'"), جوڑو_ڈیٹا->sm_راستہ);

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
	    repo_config_get_string_multi(the_repository, "submodule.active", &اقدار)) {
		/*
		 * If the submodule being added isn't already covered by the
		 * current configured pathspec, set the submodule's active flag
		 */
		کلید = xstrfmt("submodule.%s.active", جوڑو_ڈیٹا->sm_name);
		repo_config_سیٹ_gently(the_repository, کلید, "true");
		آزاد(کلید);
	} ورنہ {
		برائے (سائز_t i = 0; i < اقدار->nr; i++) {
			ثابت حرف *pat = اقدار->اشیاء[i].string;
			اگر (!wildmatch(pat, جوڑو_ڈیٹا->sm_راستہ, 0)) { /* match found */
				matched = 1;
				توڑو;
			}
		}
		اگر (!matched) { /* no pattern matched -> force-enable */
			کلید = xstrfmt("submodule.%s.active", جوڑو_ڈیٹا->sm_name);
			repo_config_سیٹ_gently(the_repository, کلید, "true");
			آزاد(کلید);
		}
	}
}

ساکن خالی die_on_اشاریہ_مطابقت(ثابت حرف *راستہ, صحیح force)
{
	ساخت pathspec ps;
	ثابت حرف *دلائل[] = { راستہ, تہی };
	parse_pathspec(&ps, 0, PATHSPEC_PREFER_CWD, تہی, دلائل);

	اگر (repo_read_اشاریہ_preload(the_repository, تہی, 0) < 0)
		die(_("index file corrupt"));

	اگر (ps.nr) {
		حرف *ps_matched = xcalloc(ps.nr, 1);

		/* TODO: audit for interaction with sparse-index. */
		ensure_بھرا_اشاریہ(the_repository->اشاریہ);

		/*
		 * Since there is only one pathspec, we just need to
		 * check ps_matched[0] to know if a cache entry matched.
		 */
		برائے (سائز_t i = 0; i < the_repository->اشاریہ->cache_nr; i++) {
			ce_راستہ_مطابقت(the_repository->اشاریہ, the_repository->اشاریہ->cache[i], &ps,
				      ps_matched);

			اگر (ps_matched[0]) {
				اگر (!force)
					die(_("'%s' already exists in the index"),
					    راستہ);
				اگر (!S_ISGITLINK(the_repository->اشاریہ->cache[i]->ce_منوال))
					die(_("'%s' already exists in the index "
					      "and is not a submodule"), راستہ);
				توڑو;
			}
		}
		آزاد(ps_matched);
	}
	clear_pathspec(&ps);
}

ساکن خالی die_on_repo_without_commits(ثابت حرف *راستہ)
{
	ساخت strbuf sb = STRBUF_INIT;
	strbuf_addstr(&sb, راستہ);
	اگر (is_nonbare_repository_dir(&sb)) {
		ساخت object_id oid;
		اگر (repo_resolve_gitlink_ref(the_repository, راستہ, "HEAD", &oid) < 0)
			die(_("'%s' does not have a commit checked out"), راستہ);
	}
	strbuf_release(&sb);
}

ساکن صحیح module_جوڑو(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
		      ساخت repository *repo غیر_مستعمل)
{
	صحیح force = 0, quiet = 0, پیش_رفت = 0, dissociate = 0;
	ساخت جوڑو_ڈیٹا جوڑو_ڈیٹا = جوڑو_ڈیٹا_INIT;
	ثابت حرف *ref_storage_format = تہی;
	حرف *مقصد_free = تہی;
	ثابت ساخت submodule *existing;
	ساخت strbuf buf = STRBUF_INIT;
	حرف *sm_name_مقصد_free = تہی;
	ساخت option options[] = {
		OPT_STRING('b', "branch", &جوڑو_ڈیٹا.branch, N_("branch"),
			   N_("branch of repository to add as submodule")),
		OPT__FORCE(&force, N_("allow adding an otherwise ignored submodule path"),
			   PARSE_OPT_NOCOMPLETE),
		OPT__QUIET(&quiet, N_("print only error messages")),
		OPT_BOOL(0, "progress", &پیش_رفت, N_("force cloning progress")),
		OPT_STRING(0, "reference", &جوڑو_ڈیٹا.reference_راستہ, N_("repository"),
			   N_("reference repository")),
		OPT_STRING(0, "ref-storage-format", &ref_storage_format, N_("format"),
			   N_("specify the reference storage format to use")),
		OPT_ALIAS_F(0, "ref-format", "ref-storage-format", PARSE_OPT_HIDDEN),
		OPT_BOOL(0, "dissociate", &dissociate, N_("borrow the objects from reference repositories")),
		OPT_STRING(0, "name", &جوڑو_ڈیٹا.sm_name, N_("name"),
			   N_("sets the submodule's name to the given string "
			      "instead of defaulting to its path")),
		OPT_INTEGER(0, "depth", &جوڑو_ڈیٹا.گہرائی, N_("depth for shallow clones")),
		OPT_انتہا()
	};
	ثابت حرف *ثابت usage[] = {
		N_("git submodule add [<options>] [--] <repository> [<path>]"),
		تہی
	};
	ساخت strbuf sb = STRBUF_INIT;
	صحیح ret = 1;

	argc = parse_options(argc, argv, سابقہ, options, usage, 0);

	اگر (!is_writing_gitmodules_ok())
		die(_("please make sure that the .gitmodules file is in the working tree"));

	اگر (سابقہ && *سابقہ &&
	    جوڑو_ڈیٹا.reference_راستہ && !is_absolute_راستہ(جوڑو_ڈیٹا.reference_راستہ))
		جوڑو_ڈیٹا.reference_راستہ = xstrfmt("%s%s", سابقہ, جوڑو_ڈیٹا.reference_راستہ);

	اگر (argc == 0 || argc > 2)
		usage_with_options(usage, options);

	اگر (ref_storage_format) {
		جوڑو_ڈیٹا.ref_storage_format = ref_storage_format_by_name(ref_storage_format);
		اگر (جوڑو_ڈیٹا.ref_storage_format == REF_STORAGE_FORMAT_UNKNOWN)
			die(_("unknown ref storage format '%s'"), ref_storage_format);
	}

	جوڑو_ڈیٹا.repo = argv[0];
	اگر (argc == 1)
		جوڑو_ڈیٹا.sm_راستہ = git_url_basename(جوڑو_ڈیٹا.repo, 0, 0);
	ورنہ
		جوڑو_ڈیٹا.sm_راستہ = xstrdup(argv[1]);

	اگر (سابقہ && *سابقہ && !is_absolute_راستہ(جوڑو_ڈیٹا.sm_راستہ)) {
		حرف *sm_راستہ = جوڑو_ڈیٹا.sm_راستہ;

		جوڑو_ڈیٹا.sm_راستہ = xstrfmt("%s%s", سابقہ, sm_راستہ);
		آزاد(sm_راستہ);
	}

	اگر (starts_with_dot_dot_slash(جوڑو_ڈیٹا.repo) ||
	    starts_with_dot_slash(جوڑو_ڈیٹا.repo)) {
		اگر (سابقہ)
			die(_("Relative path can only be used from the toplevel "
			      "of the working tree"));

		/* dereference source url relative to parent's url */
		مقصد_free = resolve_relative_url(جوڑو_ڈیٹا.repo, تہی, 1);
		جوڑو_ڈیٹا.realrepo = مقصد_free;
	} ورنہ اگر (is_dir_sep(جوڑو_ڈیٹا.repo[0]) || strchr(جوڑو_ڈیٹا.repo, ':')) {
		جوڑو_ڈیٹا.realrepo = جوڑو_ڈیٹا.repo;
	} ورنہ {
		die(_("repo URL: '%s' must be absolute or begin with ./|../"),
		    جوڑو_ڈیٹا.repo);
	}

	/*
	 * normalize path:
	 * multiple //; leading ./; /./; /../;
	 */
	normalize_راستہ_copy(جوڑو_ڈیٹا.sm_راستہ, جوڑو_ڈیٹا.sm_راستہ);
	strip_dir_trailing_slashes(جوڑو_ڈیٹا.sm_راستہ);

	اگر (validate_submodule_راستہ(جوڑو_ڈیٹا.sm_راستہ) < 0)
		die(تہی);

	die_on_اشاریہ_مطابقت(جوڑو_ڈیٹا.sm_راستہ, force);
	die_on_repo_without_commits(جوڑو_ڈیٹا.sm_راستہ);

	اگر (!force) {
		ساخت بچہ_process cp = بچہ_PROCESS_INIT;

		cp.git_cmd = 1;
		cp.no_stdout = 1;
		strvec_pushl(&cp.دلائل, "add", "--dry-run", "--ignore-missing",
			     "--no-warn-embedded-repo", جوڑو_ڈیٹا.sm_راستہ, تہی);
		اگر ((ret = pipe_command(&cp, تہی, 0, تہی, 0, &sb, 0))) {
			strbuf_complete_سطر(&sb);
			fputs(sb.buf, stderr);
			جاؤ cleanup;
		}
	}

	اگر (!جوڑو_ڈیٹا.sm_name)
		جوڑو_ڈیٹا.sm_name = جوڑو_ڈیٹا.sm_راستہ;

	existing = submodule_from_name(the_repository,
					null_oid(the_hash_algo),
					جوڑو_ڈیٹا.sm_name);

	اگر (existing && existing->راستہ &&
	    strcmp(existing->راستہ, جوڑو_ڈیٹا.sm_راستہ)) {
		اگر (!force) {
			die(_("submodule name '%s' already used for path '%s'"),
			    جوڑو_ڈیٹا.sm_name, existing->راستہ);
		}
		/* --force: build <name><n> until unique */
		برائے (صحیح i = 1; ; i++) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "%s%d", جوڑو_ڈیٹا.sm_name, i);
			اگر (!submodule_from_name(the_repository,
						null_oid(the_hash_algo),
						buf.buf)) {
				توڑو;
			}
		}
		جوڑو_ڈیٹا.sm_name = sm_name_مقصد_free = strbuf_detach(&buf, تہی);
	}

	اگر (check_submodule_name(جوڑو_ڈیٹا.sm_name))
		die(_("'%s' is not a valid submodule name"), جوڑو_ڈیٹا.sm_name);

	جوڑو_ڈیٹا.سابقہ = سابقہ;
	جوڑو_ڈیٹا.force = !!force;
	جوڑو_ڈیٹا.quiet = !!quiet;
	جوڑو_ڈیٹا.پیش_رفت = !!پیش_رفت;
	جوڑو_ڈیٹا.dissociate = !!dissociate;

	اگر (the_repository->repository_format_submodule_راستہ_cfg)
		create_default_gitdir_config(جوڑو_ڈیٹا.sm_name);

	اگر (جوڑو_submodule(&جوڑو_ڈیٹا))
		جاؤ cleanup;
	configure_added_submodule(&جوڑو_ڈیٹا);

	ret = 0;
cleanup:
	آزاد(sm_name_مقصد_free);
	آزاد(جوڑو_ڈیٹا.sm_راستہ);
	آزاد(مقصد_free);
	strbuf_release(&sb);

	واپس ret;
}

صحیح cmd_submodule__helper(صحیح argc,
			  ثابت حرف **argv,
			  ثابت حرف *سابقہ,
			  ساخت repository *repo)
{
	parse_opt_subcommand_fn *fn = تہی;
	ثابت حرف *ثابت usage[] = {
		N_("git submodule--helper <command>"),
		تہی
	};
	ساخت option options[] = {
		OPT_SUBCOMMAND("migrate-gitdir-configs", &fn, module_migrate),
		OPT_SUBCOMMAND("gitdir", &fn, module_gitdir),
		OPT_SUBCOMMAND("clone", &fn, module_clone),
		OPT_SUBCOMMAND("add", &fn, module_جوڑو),
		OPT_SUBCOMMAND("update", &fn, module_update),
		OPT_SUBCOMMAND("foreach", &fn, module_foreach),
		OPT_SUBCOMMAND("init", &fn, module_init),
		OPT_SUBCOMMAND("status", &fn, module_حالت),
		OPT_SUBCOMMAND("sync", &fn, module_sync),
		OPT_SUBCOMMAND("deinit", &fn, module_deinit),
		OPT_SUBCOMMAND("summary", &fn, module_summary),
		OPT_SUBCOMMAND("push-check", &fn, ڈالو_check),
		OPT_SUBCOMMAND("absorbgitdirs", &fn, absorb_git_dirs),
		OPT_SUBCOMMAND("set-url", &fn, module_سیٹ_url),
		OPT_SUBCOMMAND("set-branch", &fn, module_سیٹ_branch),
		OPT_SUBCOMMAND("create-branch", &fn, module_create_branch),
		OPT_SUBCOMMAND("get-default-remote", &fn, module_get_default_remote),
		OPT_انتہا()
	};
	argc = parse_options(argc, argv, سابقہ, options, usage, 0);

	واپس fn(argc, argv, سابقہ, repo);
}
