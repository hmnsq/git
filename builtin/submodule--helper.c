#define USE_THE_REPOSITORY_VARIABLE

#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "abspath.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "hex.h"

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "path.h"
#ಅನ್ತರ್ಭಾವ "pathspec.h"
#ಅನ್ತರ್ಭಾವ "preload-index.h"
#ಅನ್ತರ್ಭಾವ "dir.h"
#ಅನ್ತರ್ಭಾವ "read-cache.h"
#ಅನ್ತರ್ಭಾವ "setup.h"
#ಅನ್ತರ್ಭಾವ "sparse-index.h"
#ಅನ್ತರ್ಭಾವ "submodule.h"
#ಅನ್ತರ್ಭಾವ "submodule-config.h"
#ಅನ್ತರ್ಭಾವ "string-list.h"
#ಅನ್ತರ್ಭಾವ "run-command.h"
#ಅನ್ತರ್ಭಾವ "remote.h"
#ಅನ್ತರ್ಭಾವ "refs.h"
#ಅನ್ತರ್ಭಾವ "refspec.h"
#ಅನ್ತರ್ಭಾವ "revision.h"
#ಅನ್ತರ್ಭಾವ "diffcore.h"
#ಅನ್ತರ್ಭಾವ "diff.h"
#ಅನ್ತರ್ಭಾವ "object-file.h"
#ಅನ್ತರ್ಭಾವ "object-name.h"
#ಅನ್ತರ್ಭಾವ "odb.h"
#ಅನ್ತರ್ಭಾವ "odb/source.h"
#ಅನ್ತರ್ಭಾವ "odb/source-files.h"
#ಅನ್ತರ್ಭಾವ "advice.h"
#ಅನ್ತರ್ಭಾವ "branch.h"
#ಅನ್ತರ್ಭಾವ "list-objects-filter-options.h"
#ಅನ್ತರ್ಭಾವ "wildmatch.h"
#ಅನ್ತರ್ಭಾವ "strbuf.h"
#ಅನ್ತರ್ಭಾವ "url.h"

#define OPT_QUIET (1 << 0)
#define OPT_CACHED (1 << 1)
#define OPT_RECURSIVE (1 << 2)
#define OPT_FORCE (1 << 3)

ಪ್ರಕಾರನಿರ್ದೇಶ ಶೂನ್ಯ (*each_submodule_fn)(ಧ್ರುವ ಸಂರಚನಾ cache_entry *list_item,
				  ಶೂನ್ಯ *cb_data);

ಸ್ಥಿರ ವರ್ಣ *get_default_remote(ಶೂನ್ಯ)
{
	ಪ್ರತ್ಯಾಗಮ xstrdup(repo_default_remote(the_repository));
}

ಸ್ಥಿರ ವರ್ಣ *resolve_relative_url(ಧ್ರುವ ವರ್ಣ *rel_url, ಧ್ರುವ ವರ್ಣ *up_path, ಪೂರ್ಣಾಙ್ಕ quiet)
{
	ವರ್ಣ *remoteurl, *resolved_url;
	ವರ್ಣ *remote = get_default_remote();
	ಸಂರಚನಾ strbuf remotesb = STRBUF_INIT;

	strbuf_addf(&remotesb, "remote.%s.url", remote);
	ಯದಿ (repo_config_get_string(the_repository, remotesb.buf, &remoteurl)) {
		ಯದಿ (!quiet)
			warning(_("could not look up configuration '%s'. "
				  "Assuming this repository is its own "
				  "authoritative upstream."),
				remotesb.buf);
		remoteurl = xgetcwd();
	}
	resolved_url = relative_url(remoteurl, rel_url, up_path);

	ಮುಕ್ತ(remote);
	ಮುಕ್ತ(remoteurl);
	strbuf_release(&remotesb);

	ಪ್ರತ್ಯಾಗಮ resolved_url;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ get_default_remote_submodule(ಧ್ರುವ ವರ್ಣ *module_path, ವರ್ಣ **default_remote)
{
	ಧ್ರುವ ಸಂರಚನಾ submodule *sub;
	ಸಂರಚನಾ repository subrepo;
	ಧ್ರುವ ವರ್ಣ *remote_name = ರಿಕ್ತ;
	ವರ್ಣ *url = ರಿಕ್ತ;
	ಪೂರ್ಣಾಙ್ಕ ret = 0;

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), module_path);
	ಯದಿ (sub && sub->url) {
		url = xstrdup(sub->url);

		/* Possibly a url relative to parent */
		ಯದಿ (starts_with_dot_dot_slash(url) ||
		    starts_with_dot_slash(url)) {
			ವರ್ಣ *oldurl = url;

			url = resolve_relative_url(oldurl, ರಿಕ್ತ, 1);
			ಮುಕ್ತ(oldurl);
		}
	}

	ಯದಿ (repo_submodule_init(&subrepo, the_repository, module_path,
				null_oid(the_hash_algo)) < 0) {
		ret = die_message(_("could not get a repository handle for submodule '%s'"),
				   module_path);
		ಗಚ್ಛ out;
	}

	/* Look up by URL first */
	ಯದಿ (url)
		remote_name = repo_remote_from_url(&subrepo, url);
	ಯದಿ (!remote_name)
		remote_name = repo_default_remote(&subrepo);

	*default_remote = xstrdup(remote_name);

out:
	repo_clear(&subrepo);
	ಮುಕ್ತ(url);

	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ module_get_default_remote(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
				     ಸಂರಚನಾ repository *repo UNUSED)
{
	ಧ್ರುವ ವರ್ಣ *path;
	ವರ್ಣ *resolved_path = ರಿಕ್ತ;
	ವರ್ಣ *default_remote = ರಿಕ್ತ;
	ಪೂರ್ಣಾಙ್ಕ code;
	ಸಂರಚನಾ option options[] = {
		OPT_END()
	};
	ಧ್ರುವ ವರ್ಣ *ಧ್ರುವ usage[] = {
		N_("git submodule--helper get-default-remote <path>"),
		ರಿಕ್ತ
	};

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	ಯದಿ (argc != 1)
		usage_with_options(usage, options);

	path = argv[0];
	ಯದಿ (prefix && *prefix && !is_absolute_path(path)) {
		resolved_path = xstrfmt("%s%s", prefix, path);
		path = resolved_path;
	}

	code = get_default_remote_submodule(path, &default_remote);
	ಯದಿ (code) {
		ಮುಕ್ತ(resolved_path);
		ಪ್ರತ್ಯಾಗಮ code;
	}

	printf("%s\n", default_remote);
	ಮುಕ್ತ(default_remote);
	ಮುಕ್ತ(resolved_path);
	ಪ್ರತ್ಯಾಗಮ 0;
}

/* the result should be freed by the caller. */
ಸ್ಥಿರ ವರ್ಣ *get_submodule_displaypath(ಧ್ರುವ ವರ್ಣ *path, ಧ್ರುವ ವರ್ಣ *prefix,
				       ಧ್ರುವ ವರ್ಣ *super_prefix)
{
	ಯದಿ (prefix && super_prefix) {
		BUG("cannot have prefix '%s' and superprefix '%s'",
		    prefix, super_prefix);
	} ಅನ್ಯಥಾ ಯದಿ (prefix) {
		ಸಂರಚನಾ strbuf sb = STRBUF_INIT;
		ವರ್ಣ *displaypath = xstrdup(relative_path(path, prefix, &sb));
		strbuf_release(&sb);
		ಪ್ರತ್ಯಾಗಮ displaypath;
	} ಅನ್ಯಥಾ ಯದಿ (super_prefix) {
		ಪ್ರತ್ಯಾಗಮ xstrfmt("%s%s", super_prefix, path);
	} ಅನ್ಯಥಾ {
		ಪ್ರತ್ಯಾಗಮ xstrdup(path);
	}
}

ಸ್ಥಿರ ವರ್ಣ *compute_rev_name(ಧ್ರುವ ವರ್ಣ *sub_path, ಧ್ರುವ ವರ್ಣ* object_id)
{
	ಸಂರಚನಾ strbuf sb = STRBUF_INIT;
	ಧ್ರುವ ವರ್ಣ ***d;

	ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *describe_bare[] = { ರಿಕ್ತ };

	ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *describe_tags[] = { "--tags", ರಿಕ್ತ };

	ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *describe_contains[] = { "--contains", ರಿಕ್ತ };

	ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *describe_all_always[] = { "--all", "--always", ರಿಕ್ತ };

	ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ **describe_argv[] = { describe_bare, describe_tags,
						describe_contains,
						describe_all_always, ರಿಕ್ತ };

	ಪ್ರತಿ (d = describe_argv; *d; d++) {
		ಸಂರಚನಾ child_process cp = CHILD_PROCESS_INIT;
		prepare_submodule_repo_env(&cp.env);
		cp.dir = sub_path;
		cp.git_cmd = 1;
		cp.no_stderr = 1;

		strvec_ತಳ್ಳು(&cp.args, "describe");
		strvec_pushv(&cp.args, *d);
		strvec_ತಳ್ಳು(&cp.args, object_id);

		ಯದಿ (!capture_command(&cp, &sb, 0)) {
			strbuf_strip_suffix(&sb, "\n");
			ಪ್ರತ್ಯಾಗಮ strbuf_detach(&sb, ರಿಕ್ತ);
		}
	}

	strbuf_release(&sb);
	ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

ಸಂರಚನಾ module_list {
	ಧ್ರುವ ಸಂರಚನಾ cache_entry **entries;
	ಪೂರ್ಣಾಙ್ಕ alloc, nr;
};
#define MODULE_LIST_INIT { 0 }

ಸ್ಥಿರ ಶೂನ್ಯ module_list_release(ಸಂರಚನಾ module_list *ml)
{
	ಮುಕ್ತ(ml->entries);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ module_list_compute(ಧ್ರುವ ವರ್ಣ **argv,
			       ಧ್ರುವ ವರ್ಣ *prefix,
			       ಸಂರಚನಾ pathspec *pathspec,
			       ಸಂರಚನಾ module_list *list)
{
	ಪೂರ್ಣಾಙ್ಕ result = 0;
	ವರ್ಣ *ps_matched = ರಿಕ್ತ;

	parse_pathspec(pathspec, 0,
		       PATHSPEC_PREFER_FULL,
		       prefix, argv);

	ಯದಿ (pathspec->nr)
		ps_matched = xcalloc(pathspec->nr, 1);

	ಯದಿ (repo_read_index(the_repository) < 0)
		die(_("index file corrupt"));

	ಪ್ರತಿ (size_t i = 0; i < the_repository->index->cache_nr; i++) {
		ಧ್ರುವ ಸಂರಚನಾ cache_entry *ce = the_repository->index->cache[i];

		ಯದಿ (!match_pathspec(the_repository->index, pathspec, ce->name, ce_namelen(ce),
				    0, ps_matched, 1) ||
		    !S_ISGITLINK(ce->ce_mode))
			ಅನುವರ್ತ;

		ALLOC_GROW(list->entries, list->nr + 1, list->alloc);
		list->entries[list->nr++] = ce;
		ಯಾವತ್ (i + 1 < the_repository->index->cache_nr &&
		       !strcmp(ce->name, the_repository->index->cache[i + 1]->name))
			/*
			 * Skip entries with the same name in different stages
			 * to make sure an entry is returned only once.
			 */
			i++;
	}

	ಯದಿ (ps_matched && report_path_error(ps_matched, pathspec))
		result = -1;

	ಮುಕ್ತ(ps_matched);

	ಪ್ರತ್ಯಾಗಮ result;
}

ಸ್ಥಿರ ಶೂನ್ಯ module_list_active(ಸಂರಚನಾ module_list *list)
{
	ಪೂರ್ಣಾಙ್ಕ i;
	ಸಂರಚನಾ module_list active_modules = MODULE_LIST_INIT;

	ಪ್ರತಿ (i = 0; i < list->nr; i++) {
		ಧ್ರುವ ಸಂರಚನಾ cache_entry *ce = list->entries[i];

		ಯದಿ (!is_submodule_active(the_repository, ce->name))
			ಅನುವರ್ತ;

		ALLOC_GROW(active_modules.entries,
			   active_modules.nr + 1,
			   active_modules.alloc);
		active_modules.entries[active_modules.nr++] = ce;
	}

	module_list_release(list);
	*list = active_modules;
}

ಸ್ಥಿರ ವರ್ಣ *get_up_path(ಧ್ರುವ ವರ್ಣ *path)
{
	ಸಂರಚನಾ strbuf sb = STRBUF_INIT;

	strbuf_addstrings(&sb, "../", ಎಣಿಕೆ_slashes(path));

	/*
	 * Check if 'path' ends with slash or not
	 * for having the same output for dir/sub_dir
	 * and dir/sub_dir/
	 */
	ಯದಿ (!is_dir_sep(path[strlen(path) - 1]))
		strbuf_addstr(&sb, "../");

	ಪ್ರತ್ಯಾಗಮ strbuf_detach(&sb, ರಿಕ್ತ);
}

ಸ್ಥಿರ ಶೂನ್ಯ for_each_listed_submodule(ಧ್ರುವ ಸಂರಚನಾ module_list *list,
				      each_submodule_fn fn, ಶೂನ್ಯ *cb_data)
{
	ಪೂರ್ಣಾಙ್ಕ i;

	ಪ್ರತಿ (i = 0; i < list->nr; i++)
		fn(list->entries[i], cb_data);
}

ಸಂರಚನಾ foreach_cb {
	ಪೂರ್ಣಾಙ್ಕ argc;
	ಧ್ರುವ ವರ್ಣ **argv;
	ಧ್ರುವ ವರ್ಣ *prefix;
	ಧ್ರುವ ವರ್ಣ *super_prefix;
	ಪೂರ್ಣಾಙ್ಕ quiet;
	ಪೂರ್ಣಾಙ್ಕ recursive;
};
#define FOREACH_CB_INIT { 0 }

ಸ್ಥಿರ ಶೂನ್ಯ runcommand_in_submodule_cb(ಧ್ರುವ ಸಂರಚನಾ cache_entry *list_item,
				       ಶೂನ್ಯ *cb_data)
{
	ಸಂರಚನಾ foreach_cb *info = cb_data;
	ಧ್ರುವ ವರ್ಣ *path = list_item->name;
	ಧ್ರುವ ಸಂರಚನಾ object_id *ce_oid = &list_item->oid;
	ಧ್ರುವ ಸಂರಚನಾ submodule *sub;
	ಸಂರಚನಾ child_process cp = CHILD_PROCESS_INIT;
	ವರ್ಣ *displaypath;

	ಯದಿ (validate_submodule_path(path) < 0)
		die(ರಿಕ್ತ);

	displaypath = get_submodule_displaypath(path, info->prefix,
						info->super_prefix);

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);

	ಯದಿ (!sub)
		die(_("No url found for submodule path '%s' in .gitmodules"),
			displaypath);

	ಯದಿ (!is_submodule_populated_gently(path, ರಿಕ್ತ))
		ಗಚ್ಛ cleanup;

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
	ಯದಿ (info->argc == 1) {
		ವರ್ಣ *toplevel = xgetcwd();
		ಸಂರಚನಾ strbuf sb = STRBUF_INIT;

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
		ಮುಕ್ತ(toplevel);
	} ಅನ್ಯಥಾ {
		strvec_pushv(&cp.args, info->argv);
	}

	ಯದಿ (!info->quiet)
		printf(_("Entering '%s'\n"), displaypath);

	ಯದಿ (info->argv[0]) {
		ಯದಿ (run_command(&cp))
			die(_("run_command returned non-zero status for %s\n."),
			    displaypath);
	} ಅನ್ಯಥಾ {
		child_process_clear(&cp);
	}

	ಯದಿ (info->recursive) {
		ಸಂರಚನಾ child_process cpr = CHILD_PROCESS_INIT;

		cpr.git_cmd = 1;
		cpr.dir = path;
		prepare_submodule_repo_env(&cpr.env);

		strvec_pushl(&cpr.args, "submodule--helper", "foreach", "--recursive",
			     ರಿಕ್ತ);
		strvec_pushf(&cpr.args, "--super-prefix=%s/", displaypath);

		ಯದಿ (info->quiet)
			strvec_ತಳ್ಳು(&cpr.args, "--quiet");

		strvec_ತಳ್ಳು(&cpr.args, "--");
		strvec_pushv(&cpr.args, info->argv);

		ಯದಿ (run_command(&cpr))
			die(_("run_command returned non-zero status while "
				"recursing in the nested submodules of %s\n."),
				displaypath);
	}

cleanup:
	ಮುಕ್ತ(displaypath);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ module_foreach(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
			  ಸಂರಚನಾ repository *repo UNUSED)
{
	ಸಂರಚನಾ foreach_cb info = FOREACH_CB_INIT;
	ಸಂರಚನಾ pathspec pathspec = { 0 };
	ಸಂರಚನಾ module_list list = MODULE_LIST_INIT;
	ಸಂರಚನಾ option module_foreach_options[] = {
		OPT__SUPER_PREFIX(&info.super_prefix),
		OPT__QUIET(&info.quiet, N_("suppress output of entering each submodule command")),
		OPT_BOOL(0, "recursive", &info.recursive,
			 N_("recurse into nested submodules")),
		OPT_END()
	};
	ಧ್ರುವ ವರ್ಣ *ಧ್ರುವ git_submodule_helper_usage[] = {
		N_("git submodule foreach [--quiet] [--recursive] [--] <command>"),
		ರಿಕ್ತ
	};
	ಪೂರ್ಣಾಙ್ಕ ret = 1;

	argc = parse_options(argc, argv, prefix, module_foreach_options,
			     git_submodule_helper_usage, 0);

	ಯದಿ (module_list_compute(ರಿಕ್ತ, prefix, &pathspec, &list) < 0)
		ಗಚ್ಛ cleanup;

	info.argc = argc;
	info.argv = argv;
	info.prefix = prefix;

	for_each_listed_submodule(&list, runcommand_in_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸಂರಚನಾ init_cb {
	ಧ್ರುವ ವರ್ಣ *prefix;
	ಧ್ರುವ ವರ್ಣ *super_prefix;
	unsigned ಪೂರ್ಣಾಙ್ಕ flags;
};
#define INIT_CB_INIT { 0 }

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ validate_and_set_submodule_gitdir(ಸಂರಚನಾ strbuf *gitdir_path,
					     ಧ್ರುವ ವರ್ಣ *submodule_name)
{
	ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ;
	ವರ್ಣ *ಕೀಲಿ;

	ಯದಿ (validate_submodule_git_dir(gitdir_path->buf, submodule_name))
		ಪ್ರತ್ಯಾಗಮ -1;

	ಕೀಲಿ = xstrfmt("submodule.%s.gitdir", submodule_name);

	/* Nothing to do if the config already exists. */
	ಯದಿ (!repo_config_get_string_tmp(the_repository, ಕೀಲಿ, &ಮೌಲ್ಯ)) {
		ಮುಕ್ತ(ಕೀಲಿ);
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (repo_config_set_gently(the_repository, ಕೀಲಿ, gitdir_path->buf)) {
		ಮುಕ್ತ(ಕೀಲಿ);
		ಪ್ರತ್ಯಾಗಮ -1;
	}

	ಮುಕ್ತ(ಕೀಲಿ);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ create_default_gitdir_config(ಧ್ರುವ ವರ್ಣ *submodule_name)
{
	ಸಂರಚನಾ strbuf gitdir_path = STRBUF_INIT;
	ಸಂರಚನಾ git_hash_ctx ctx;
	ವರ್ಣ hex_name_hash[GIT_MAX_HEXSZ + 1], header[128];
	unsigned ವರ್ಣ raw_name_hash[GIT_MAX_RAWSZ];
	ಪೂರ್ಣಾಙ್ಕ header_len;

	/* Case 1: try the plain module name */
	repo_git_path_append(the_repository, &gitdir_path, "modules/%s", submodule_name);
	ಯದಿ (!validate_and_set_submodule_gitdir(&gitdir_path, submodule_name)) {
		strbuf_release(&gitdir_path);
		ಪ್ರತ್ಯಾಗಮ;
	}

	/* Case 2.1: Try URI-safe (RFC3986) encoding first, this fixes nested gitdirs */
	strbuf_reset(&gitdir_path);
	repo_git_path_append(the_repository, &gitdir_path, "modules/");
	strbuf_addstr_urlencode(&gitdir_path, submodule_name, is_rfc3986_unreserved);
	ಯದಿ (!validate_and_set_submodule_gitdir(&gitdir_path, submodule_name)) {
		strbuf_release(&gitdir_path);
		ಪ್ರತ್ಯಾಗಮ;
	}

	/* Case 2.2: Try extended uppercase URI (RFC3986) encoding, to fix case-folding */
	strbuf_reset(&gitdir_path);
	repo_git_path_append(the_repository, &gitdir_path, "modules/");
	strbuf_addstr_urlencode(&gitdir_path, submodule_name, is_casefolding_rfc3986_unreserved);
	ಯದಿ (!validate_and_set_submodule_gitdir(&gitdir_path, submodule_name))
		ಪ್ರತ್ಯಾಗಮ;

	/* Case 2.3: Try some derived gitdir names, see if one sticks */
	ಪ್ರತಿ (ವರ್ಣ c = '0'; c <= '9'; c++) {
		strbuf_reset(&gitdir_path);
		repo_git_path_append(the_repository, &gitdir_path, "modules/");
		strbuf_addstr_urlencode(&gitdir_path, submodule_name, is_rfc3986_unreserved);
		strbuf_addch(&gitdir_path, c);
		ಯದಿ (!validate_and_set_submodule_gitdir(&gitdir_path, submodule_name))
			ಪ್ರತ್ಯಾಗಮ;

		strbuf_reset(&gitdir_path);
		repo_git_path_append(the_repository, &gitdir_path, "modules/");
		strbuf_addstr_urlencode(&gitdir_path, submodule_name, is_casefolding_rfc3986_unreserved);
		strbuf_addch(&gitdir_path, c);
		ಯದಿ (!validate_and_set_submodule_gitdir(&gitdir_path, submodule_name))
			ಪ್ರತ್ಯಾಗಮ;
	}

	/* Case 2.4: If all the above failed, try a hash of the name as a last resort */
	header_len = snprintf(header, ಪರಿಮಾಣ(header),
			      "blob %"PRIuMAX, (uintmax_t)strlen(submodule_name));
	git_hash_init(&ctx, the_hash_algo);
	git_hash_update(&ctx, header, header_len);
	git_hash_update(&ctx, "\0", 1);
	git_hash_update(&ctx, submodule_name, strlen(submodule_name));
	git_hash_final(raw_name_hash, &ctx);
	hash_to_hex_algop_r(hex_name_hash, raw_name_hash, the_hash_algo);
	strbuf_reset(&gitdir_path);
	repo_git_path_append(the_repository, &gitdir_path, "modules/%s", hex_name_hash);
	ಯದಿ (!validate_and_set_submodule_gitdir(&gitdir_path, submodule_name)) {
		strbuf_release(&gitdir_path);
		ಪ್ರತ್ಯಾಗಮ;
	}

	/* Case 3: nothing worked, error out */
	die(_("failed to set a valid default config for 'submodule.%s.gitdir'. "
	      "Please ensure it is set, for example by running something like: "
	      "'git config submodule.%s.gitdir .git/modules/%s'"),
	    submodule_name, submodule_name, submodule_name);
}

ಸ್ಥಿರ ಶೂನ್ಯ init_submodule(ಧ್ರುವ ವರ್ಣ *path, ಧ್ರುವ ವರ್ಣ *prefix,
			   ಧ್ರುವ ವರ್ಣ *super_prefix,
			   unsigned ಪೂರ್ಣಾಙ್ಕ flags)
{
	ಧ್ರುವ ಸಂರಚನಾ submodule *sub;
	ಸಂರಚನಾ strbuf sb = STRBUF_INIT;
	ಧ್ರುವ ವರ್ಣ *upd;
	ವರ್ಣ *url = ರಿಕ್ತ, *displaypath;

	displaypath = get_submodule_displaypath(path, prefix, super_prefix);

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);

	ಯದಿ (!sub)
		die(_("No url found for submodule path '%s' in .gitmodules"),
			displaypath);

	/*
	 * NEEDSWORK: In a multi-working-tree world, this needs to be
	 * set in the per-worktree config.
	 *
	 * Set active flag for the submodule being initialized
	 */
	ಯದಿ (!is_submodule_active(the_repository, path)) {
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
	ಯದಿ (repo_config_get_string(the_repository, sb.buf, &url)) {
		ಯದಿ (!sub->url)
			die(_("No url found for submodule path '%s' in .gitmodules"),
				displaypath);

		url = xstrdup(sub->url);

		/* Possibly a url relative to parent */
		ಯದಿ (starts_with_dot_dot_slash(url) ||
		    starts_with_dot_slash(url)) {
			ವರ್ಣ *oldurl = url;

			url = resolve_relative_url(oldurl, ರಿಕ್ತ, 0);
			ಮುಕ್ತ(oldurl);
		}

		ಯದಿ (repo_config_set_gently(the_repository, sb.buf, url))
			die(_("Failed to register url for submodule path '%s'"),
			    displaypath);
		ಯದಿ (!(flags & OPT_QUIET))
			fprintf(stderr,
				_("Submodule '%s' (%s) registered for path '%s'\n"),
				sub->name, url, displaypath);
	}
	strbuf_reset(&sb);

	/* Copy "update" setting when it is not set yet */
	strbuf_addf(&sb, "submodule.%s.update", sub->name);
	ಯದಿ (repo_config_get_string_tmp(the_repository, sb.buf, &upd) &&
	    sub->update_strategy.type != SM_UPDATE_UNSPECIFIED) {
		ಯದಿ (sub->update_strategy.type == SM_UPDATE_COMMAND) {
			fprintf(stderr, _("warning: command update mode suggested for submodule '%s'\n"),
				sub->name);
			upd = "none";
		} ಅನ್ಯಥಾ {
			upd = submodule_update_type_to_string(sub->update_strategy.type);
		}

		ಯದಿ (repo_config_set_gently(the_repository, sb.buf, upd))
			die(_("Failed to register update mode for submodule path '%s'"), displaypath);
	}

	ಯದಿ (the_repository->repository_format_submodule_path_cfg)
		create_default_gitdir_config(sub->name);

	strbuf_release(&sb);
	ಮುಕ್ತ(displaypath);
	ಮುಕ್ತ(url);
}

ಸ್ಥಿರ ಶೂನ್ಯ init_submodule_cb(ಧ್ರುವ ಸಂರಚನಾ cache_entry *list_item, ಶೂನ್ಯ *cb_data)
{
	ಸಂರಚನಾ init_cb *info = cb_data;

	init_submodule(list_item->name, info->prefix, info->super_prefix,
		       info->flags);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ module_init(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
		       ಸಂರಚನಾ repository *repo UNUSED)
{
	ಸಂರಚನಾ init_cb info = INIT_CB_INIT;
	ಸಂರಚನಾ pathspec pathspec = { 0 };
	ಸಂರಚನಾ module_list list = MODULE_LIST_INIT;
	ಪೂರ್ಣಾಙ್ಕ quiet = 0;
	ಸಂರಚನಾ option module_init_options[] = {
		OPT__QUIET(&quiet, N_("suppress output for initializing a submodule")),
		OPT_END()
	};
	ಧ್ರುವ ವರ್ಣ *ಧ್ರುವ git_submodule_helper_usage[] = {
		N_("git submodule init [<options>] [<path>]"),
		ರಿಕ್ತ
	};
	ಪೂರ್ಣಾಙ್ಕ ret = 1;

	argc = parse_options(argc, argv, prefix, module_init_options,
			     git_submodule_helper_usage, 0);

	ಯದಿ (module_list_compute(argv, prefix, &pathspec, &list) < 0)
		ಗಚ್ಛ cleanup;

	/*
	 * If there are no path args and submodule.active is set then,
	 * by default, only initialize 'active' modules.
	 */
	ಯದಿ (!argc && !repo_config_get(the_repository, "submodule.active"))
		module_list_active(&list);

	info.prefix = prefix;
	ಯದಿ (quiet)
		info.flags |= OPT_QUIET;

	for_each_listed_submodule(&list, init_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸಂರಚನಾ status_cb {
	ಧ್ರುವ ವರ್ಣ *prefix;
	ಧ್ರುವ ವರ್ಣ *super_prefix;
	unsigned ಪೂರ್ಣಾಙ್ಕ flags;
};
#define STATUS_CB_INIT { 0 }

ಸ್ಥಿರ ಶೂನ್ಯ print_status(unsigned ಪೂರ್ಣಾಙ್ಕ flags, ವರ್ಣ state, ಧ್ರುವ ವರ್ಣ *path,
			 ಧ್ರುವ ಸಂರಚನಾ object_id *oid, ಧ್ರುವ ವರ್ಣ *displaypath)
{
	ಯದಿ (flags & OPT_QUIET)
		ಪ್ರತ್ಯಾಗಮ;

	printf("%c%s %s", state, oid_to_hex(oid), displaypath);

	ಯದಿ (state == ' ' || state == '+') {
		ವರ್ಣ *name = compute_rev_name(path, oid_to_hex(oid));

		ಯದಿ (name)
			printf(" (%s)", name);
		ಮುಕ್ತ(name);
	}

	printf("\n");
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ handle_submodule_head_ref(ಧ್ರುವ ಸಂರಚನಾ reference *ref, ಶೂನ್ಯ *cb_data)
{
	ಸಂರಚನಾ object_id *output = cb_data;

	ಯದಿ (ref->oid)
		oidcpy(output, ref->oid);

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ status_submodule(ಧ್ರುವ ವರ್ಣ *path, ಧ್ರುವ ಸಂರಚನಾ object_id *ce_oid,
			     unsigned ಪೂರ್ಣಾಙ್ಕ ce_flags, ಧ್ರುವ ವರ್ಣ *prefix,
			     ಧ್ರುವ ವರ್ಣ *super_prefix, unsigned ಪೂರ್ಣಾಙ್ಕ flags)
{
	ವರ್ಣ *displaypath;
	ಸಂರಚನಾ strvec diff_files_args = STRVEC_INIT;
	ಸಂರಚನಾ rev_info rev = REV_INFO_INIT;
	ಸಂರಚನಾ strbuf buf = STRBUF_INIT;
	ಧ್ರುವ ವರ್ಣ *git_dir;

	ಯದಿ (validate_submodule_path(path) < 0)
		die(ರಿಕ್ತ);

	ಯದಿ (!submodule_from_path(the_repository, null_oid(the_hash_algo), path))
		die(_("no submodule mapping found in .gitmodules for path '%s'"),
		      path);

	displaypath = get_submodule_displaypath(path, prefix, super_prefix);

	ಯದಿ ((CE_STAGEMASK & ce_flags) >> CE_STAGESHIFT) {
		print_status(flags, 'U', path, null_oid(the_hash_algo), displaypath);
		ಗಚ್ಛ cleanup;
	}

	strbuf_addf(&buf, "%s/.git", path);
	git_dir = read_gitfile(buf.buf);
	ಯದಿ (!git_dir)
		git_dir = buf.buf;

	ಯದಿ (!is_submodule_active(the_repository, path) ||
	    !is_git_directory(git_dir)) {
		print_status(flags, '-', path, ce_oid, displaypath);
		strbuf_release(&buf);
		ಗಚ್ಛ cleanup;
	}
	strbuf_release(&buf);

	strvec_pushl(&diff_files_args, "diff-files",
		     "--ignore-submodules=dirty", "--quiet", "--",
		     path, ರಿಕ್ತ);

	repo_config(the_repository, git_diff_basic_config, ರಿಕ್ತ);

	repo_init_revisions(the_repository, &rev, ರಿಕ್ತ);
	rev.abbrev = 0;
	setup_revisions_from_strvec(&diff_files_args, &rev, ರಿಕ್ತ);
	run_diff_files(&rev, 0);

	ಯದಿ (!diff_result_code(&rev)) {
		print_status(flags, ' ', path, ce_oid,
			     displaypath);
	} ಅನ್ಯಥಾ ಯದಿ (!(flags & OPT_CACHED)) {
		ಸಂರಚನಾ object_id oid;
		ಸಂರಚನಾ ref_store *refs = repo_get_submodule_ref_store(the_repository,
								      path);

		ಯದಿ (!refs) {
			print_status(flags, '-', path, ce_oid, displaypath);
			ಗಚ್ಛ cleanup;
		}
		ಯದಿ (refs_head_ref(refs, handle_submodule_head_ref, &oid))
			die(_("could not resolve HEAD ref inside the "
			      "submodule '%s'"), path);

		print_status(flags, '+', path, &oid, displaypath);
	} ಅನ್ಯಥಾ {
		print_status(flags, '+', path, ce_oid, displaypath);
	}

	ಯದಿ (flags & OPT_RECURSIVE) {
		ಸಂರಚನಾ child_process cpr = CHILD_PROCESS_INIT;
		ಪೂರ್ಣಾಙ್ಕ res;

		cpr.git_cmd = 1;
		cpr.dir = path;
		prepare_submodule_repo_env(&cpr.env);

		strvec_pushl(&cpr.args, "submodule--helper", "status",
			     "--recursive", ರಿಕ್ತ);
		strvec_pushf(&cpr.args, "--super-prefix=%s/", displaypath);

		ಯದಿ (flags & OPT_CACHED)
			strvec_ತಳ್ಳು(&cpr.args, "--cached");

		ಯದಿ (flags & OPT_QUIET)
			strvec_ತಳ್ಳು(&cpr.args, "--quiet");

		res = run_command(&cpr);
		ಯದಿ (res == SIGPIPE + 128)
			ಉತ್ಥಾಪಯ(SIGPIPE);
		ಅನ್ಯಥಾ ಯದಿ (res)
			die(_("failed to recurse into submodule '%s'"), path);
	}

cleanup:
	strvec_clear(&diff_files_args);
	ಮುಕ್ತ(displaypath);
	release_revisions(&rev);
}

ಸ್ಥಿರ ಶೂನ್ಯ status_submodule_cb(ಧ್ರುವ ಸಂರಚನಾ cache_entry *list_item,
				ಶೂನ್ಯ *cb_data)
{
	ಸಂರಚನಾ status_cb *info = cb_data;

	status_submodule(list_item->name, &list_item->oid, list_item->ce_flags,
			 info->prefix, info->super_prefix, info->flags);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ module_status(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
			 ಸಂರಚನಾ repository *repo UNUSED)
{
	ಸಂರಚನಾ status_cb info = STATUS_CB_INIT;
	ಸಂರಚನಾ pathspec pathspec = { 0 };
	ಸಂರಚನಾ module_list list = MODULE_LIST_INIT;
	ಪೂರ್ಣಾಙ್ಕ quiet = 0;
	ಸಂರಚನಾ option module_status_options[] = {
		OPT__SUPER_PREFIX(&info.super_prefix),
		OPT__QUIET(&quiet, N_("suppress submodule status output")),
		OPT_BIT(0, "cached", &info.flags, N_("use commit stored in the index instead of the one stored in the submodule HEAD"), OPT_CACHED),
		OPT_BIT(0, "recursive", &info.flags, N_("recurse into nested submodules"), OPT_RECURSIVE),
		OPT_END()
	};
	ಧ್ರುವ ವರ್ಣ *ಧ್ರುವ git_submodule_helper_usage[] = {
		N_("git submodule status [--quiet] [--cached] [--recursive] [<path>...]"),
		ರಿಕ್ತ
	};
	ಪೂರ್ಣಾಙ್ಕ ret = 1;

	argc = parse_options(argc, argv, prefix, module_status_options,
			     git_submodule_helper_usage, 0);

	ಯದಿ (module_list_compute(argv, prefix, &pathspec, &list) < 0)
		ಗಚ್ಛ cleanup;

	info.prefix = prefix;
	ಯದಿ (quiet)
		info.flags |= OPT_QUIET;

	for_each_listed_submodule(&list, status_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸಂರಚನಾ module_cb {
	unsigned ಪೂರ್ಣಾಙ್ಕ mod_src;
	unsigned ಪೂರ್ಣಾಙ್ಕ mod_dst;
	ಸಂರಚನಾ object_id oid_src;
	ಸಂರಚನಾ object_id oid_dst;
	ವರ್ಣ status;
	ವರ್ಣ *sm_path;
};
#define MODULE_CB_INIT { 0 }

ಸ್ಥಿರ ಶೂನ್ಯ module_cb_release(ಸಂರಚನಾ module_cb *mcb)
{
	ಮುಕ್ತ(mcb->sm_path);
}

ಸಂರಚನಾ module_cb_list {
	ಸಂರಚನಾ module_cb **entries;
	ಪೂರ್ಣಾಙ್ಕ alloc, nr;
};
#define MODULE_CB_LIST_INIT { 0 }

ಸ್ಥಿರ ಶೂನ್ಯ module_cb_list_release(ಸಂರಚನಾ module_cb_list *mcbl)
{
	ಪೂರ್ಣಾಙ್ಕ i;

	ಪ್ರತಿ (i = 0; i < mcbl->nr; i++) {
		ಸಂರಚನಾ module_cb *mcb = mcbl->entries[i];

		module_cb_release(mcb);
		ಮುಕ್ತ(mcb);
	}
	ಮುಕ್ತ(mcbl->entries);
}

ಸಂರಚನಾ summary_cb {
	ಪೂರ್ಣಾಙ್ಕ argc;
	ಧ್ರುವ ವರ್ಣ **argv;
	ಧ್ರುವ ವರ್ಣ *prefix;
	ಧ್ರುವ ವರ್ಣ *super_prefix;
	unsigned ಪೂರ್ಣಾಙ್ಕ cached: 1;
	unsigned ಪೂರ್ಣಾಙ್ಕ for_status: 1;
	unsigned ಪೂರ್ಣಾಙ್ಕ files: 1;
	ಪೂರ್ಣಾಙ್ಕ summary_limit;
};
#define SUMMARY_CB_INIT { 0 }

ಗಣನಾ diff_cmd {
	DIFF_INDEX,
	DIFF_FILES
};

ಸ್ಥಿರ ವರ್ಣ *verify_submodule_committish(ಧ್ರುವ ವರ್ಣ *sm_path,
					 ಧ್ರುವ ವರ್ಣ *committish)
{
	ಸಂರಚನಾ child_process cp_rev_parse = CHILD_PROCESS_INIT;
	ಸಂರಚನಾ strbuf result = STRBUF_INIT;

	cp_rev_parse.git_cmd = 1;
	cp_rev_parse.dir = sm_path;
	prepare_submodule_repo_env(&cp_rev_parse.env);
	strvec_pushl(&cp_rev_parse.args, "rev-parse", "-q", "--short", ರಿಕ್ತ);
	strvec_pushf(&cp_rev_parse.args, "%s^0", committish);
	strvec_ತಳ್ಳು(&cp_rev_parse.args, "--");

	ಯದಿ (capture_command(&cp_rev_parse, &result, 0))
		ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

	strbuf_trim_trailing_newline(&result);
	ಪ್ರತ್ಯಾಗಮ strbuf_detach(&result, ರಿಕ್ತ);
}

ಸ್ಥಿರ ಶೂನ್ಯ print_submodule_summary(ಸಂರಚನಾ summary_cb *info, ಧ್ರುವ ವರ್ಣ *errmsg,
				    ಪೂರ್ಣಾಙ್ಕ total_commits, ಧ್ರುವ ವರ್ಣ *displaypath,
				    ಧ್ರುವ ವರ್ಣ *src_abbrev, ಧ್ರುವ ವರ್ಣ *dst_abbrev,
				    ಸಂರಚನಾ module_cb *p)
{
	ಯದಿ (p->status == 'T') {
		ಯದಿ (S_ISGITLINK(p->mod_dst))
			printf(_("* %s %s(blob)->%s(submodule)"),
				 displaypath, src_abbrev, dst_abbrev);
		ಅನ್ಯಥಾ
			printf(_("* %s %s(submodule)->%s(blob)"),
				 displaypath, src_abbrev, dst_abbrev);
	} ಅನ್ಯಥಾ {
		printf("* %s %s...%s",
			displaypath, src_abbrev, dst_abbrev);
	}

	ಯದಿ (total_commits < 0)
		printf(":\n");
	ಅನ್ಯಥಾ
		printf(" (%d):\n", total_commits);

	ಯದಿ (errmsg) {
		printf(_("%s"), errmsg);
	} ಅನ್ಯಥಾ ಯದಿ (total_commits > 0) {
		ಸಂರಚನಾ child_process cp_log = CHILD_PROCESS_INIT;

		cp_log.git_cmd = 1;
		cp_log.dir = p->sm_path;
		prepare_submodule_repo_env(&cp_log.env);
		strvec_pushl(&cp_log.args, "log", ರಿಕ್ತ);

		ಯದಿ (S_ISGITLINK(p->mod_src) && S_ISGITLINK(p->mod_dst)) {
			ಯದಿ (info->summary_limit > 0)
				strvec_pushf(&cp_log.args, "-%d",
					     info->summary_limit);

			strvec_pushl(&cp_log.args, "--pretty=  %m %s",
				     "--first-parent", ರಿಕ್ತ);
			strvec_pushf(&cp_log.args, "%s...%s",
				     src_abbrev, dst_abbrev);
		} ಅನ್ಯಥಾ ಯದಿ (S_ISGITLINK(p->mod_dst)) {
			strvec_pushl(&cp_log.args, "--pretty=  > %s",
				     "-1", dst_abbrev, ರಿಕ್ತ);
		} ಅನ್ಯಥಾ {
			strvec_pushl(&cp_log.args, "--pretty=  < %s",
				     "-1", src_abbrev, ರಿಕ್ತ);
		}
		run_command(&cp_log);
	}
	printf("\n");
}

ಸ್ಥಿರ ಶೂನ್ಯ generate_submodule_summary(ಸಂರಚನಾ summary_cb *info,
				       ಸಂರಚನಾ module_cb *p)
{
	ವರ್ಣ *displaypath, *src_abbrev = ರಿಕ್ತ, *dst_abbrev;
	ಪೂರ್ಣಾಙ್ಕ missing_src = 0, missing_dst = 0;
	ಸಂರಚನಾ strbuf errmsg = STRBUF_INIT;
	ಪೂರ್ಣಾಙ್ಕ total_commits = -1;

	ಯದಿ (!info->cached && oideq(&p->oid_dst, null_oid(the_hash_algo))) {
		ಯದಿ (S_ISGITLINK(p->mod_dst)) {
			ಸಂರಚನಾ ref_store *refs = repo_get_submodule_ref_store(the_repository,
									      p->sm_path);

			ಯದಿ (refs)
				refs_head_ref(refs, handle_submodule_head_ref, &p->oid_dst);
		} ಅನ್ಯಥಾ ಯದಿ (S_ISLNK(p->mod_dst) || S_ISREG(p->mod_dst)) {
			ಸಂರಚನಾ stat st;
			ಪೂರ್ಣಾಙ್ಕ fd = open(p->sm_path, O_RDONLY);

			ಯದಿ (fd < 0 || fstat(fd, &st) < 0 ||
			    index_fd(the_repository->index, &p->oid_dst, fd, &st, OBJ_BLOB,
				     p->sm_path, 0))
				error(_("couldn't hash object from '%s'"), p->sm_path);
		} ಅನ್ಯಥಾ {
			/* for a submodule removal (mode:0000000), don't warn */
			ಯದಿ (p->mod_dst)
				warning(_("unexpected mode %o"), p->mod_dst);
		}
	}

	ಯದಿ (S_ISGITLINK(p->mod_src)) {
		ಯದಿ (p->status != 'D')
			src_abbrev = verify_submodule_committish(p->sm_path,
								 oid_to_hex(&p->oid_src));
		ಯದಿ (!src_abbrev) {
			missing_src = 1;
			/*
			 * As `rev-parse` failed, we fallback to getting
			 * the abbreviated hash using oid_src. We do
			 * this as we might still need the abbreviated
			 * hash in cases like a submodule type change, etc.
			 */
			src_abbrev = xstrndup(oid_to_hex(&p->oid_src), 7);
		}
	} ಅನ್ಯಥಾ {
		/*
		 * The source does not point to a submodule.
		 * So, we fallback to getting the abbreviation using
		 * oid_src as we might still need the abbreviated
		 * hash in cases like submodule add, etc.
		 */
		src_abbrev = xstrndup(oid_to_hex(&p->oid_src), 7);
	}

	ಯದಿ (S_ISGITLINK(p->mod_dst)) {
		dst_abbrev = verify_submodule_committish(p->sm_path,
							 oid_to_hex(&p->oid_dst));
		ಯದಿ (!dst_abbrev) {
			missing_dst = 1;
			/*
			 * As `rev-parse` failed, we fallback to getting
			 * the abbreviated hash using oid_dst. We do
			 * this as we might still need the abbreviated
			 * hash in cases like a submodule type change, etc.
			 */
			dst_abbrev = xstrndup(oid_to_hex(&p->oid_dst), 7);
		}
	} ಅನ್ಯಥಾ {
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

	ಯದಿ (!missing_src && !missing_dst) {
		ಸಂರಚನಾ child_process cp_rev_list = CHILD_PROCESS_INIT;
		ಸಂರಚನಾ strbuf sb_rev_list = STRBUF_INIT;

		strvec_pushl(&cp_rev_list.args, "rev-list",
			     "--first-parent", "--count", ರಿಕ್ತ);
		ಯದಿ (S_ISGITLINK(p->mod_src) && S_ISGITLINK(p->mod_dst))
			strvec_pushf(&cp_rev_list.args, "%s...%s",
				     src_abbrev, dst_abbrev);
		ಅನ್ಯಥಾ
			strvec_ತಳ್ಳು(&cp_rev_list.args, S_ISGITLINK(p->mod_src) ?
				    src_abbrev : dst_abbrev);
		strvec_ತಳ್ಳು(&cp_rev_list.args, "--");

		cp_rev_list.git_cmd = 1;
		cp_rev_list.dir = p->sm_path;
		prepare_submodule_repo_env(&cp_rev_list.env);

		ಯದಿ (!capture_command(&cp_rev_list, &sb_rev_list, 0))
			total_commits = atoi(sb_rev_list.buf);

		strbuf_release(&sb_rev_list);
	} ಅನ್ಯಥಾ {
		/*
		 * Don't give error msg for modification whose dst is not
		 * submodule, i.e., deleted or changed to blob
		 */
		ಯದಿ (S_ISGITLINK(p->mod_dst)) {
			ಯದಿ (missing_src && missing_dst) {
				strbuf_addf(&errmsg, "  Warn: %s doesn't contain commits %s and %s\n",
					    displaypath, oid_to_hex(&p->oid_src),
					    oid_to_hex(&p->oid_dst));
			} ಅನ್ಯಥಾ {
				strbuf_addf(&errmsg, "  Warn: %s doesn't contain commit %s\n",
					    displaypath, missing_src ?
					    oid_to_hex(&p->oid_src) :
					    oid_to_hex(&p->oid_dst));
			}
		}
	}

	print_submodule_summary(info, errmsg.len ? errmsg.buf : ರಿಕ್ತ,
				total_commits, displaypath, src_abbrev,
				dst_abbrev, p);

	ಮುಕ್ತ(displaypath);
	ಮುಕ್ತ(src_abbrev);
	ಮುಕ್ತ(dst_abbrev);
	strbuf_release(&errmsg);
}

ಸ್ಥಿರ ಶೂನ್ಯ prepare_submodule_summary(ಸಂರಚನಾ summary_cb *info,
				      ಸಂರಚನಾ module_cb_list *list)
{
	ಪೂರ್ಣಾಙ್ಕ i;
	ಪ್ರತಿ (i = 0; i < list->nr; i++) {
		ಧ್ರುವ ಸಂರಚನಾ submodule *sub;
		ಸಂರಚನಾ module_cb *p = list->entries[i];
		ಸಂರಚನಾ strbuf sm_gitdir = STRBUF_INIT;

		ಯದಿ (p->status == 'D' || p->status == 'T') {
			generate_submodule_summary(info, p);
			ಅನುವರ್ತ;
		}

		ಯದಿ (info->for_status && p->status != 'A' &&
		    (sub = submodule_from_path(the_repository,
					       null_oid(the_hash_algo), p->sm_path))) {
			ವರ್ಣ *config_ಕೀಲಿ = ರಿಕ್ತ;
			ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ;
			ಪೂರ್ಣಾಙ್ಕ ignore_all = 0;

			config_ಕೀಲಿ = xstrfmt("submodule.%s.ignore",
					     sub->name);
			ಯದಿ (!repo_config_get_string_tmp(the_repository, config_ಕೀಲಿ, &ಮೌಲ್ಯ))
				ignore_all = !strcmp(ಮೌಲ್ಯ, "all");
			ಅನ್ಯಥಾ ಯದಿ (sub->ignore)
				ignore_all = !strcmp(sub->ignore, "all");

			ಮುಕ್ತ(config_ಕೀಲಿ);
			ಯದಿ (ignore_all)
				ಅನುವರ್ತ;
		}

		/* Also show added or modified modules which are checked out */
		strbuf_addstr(&sm_gitdir, p->sm_path);
		ಯದಿ (is_nonbare_repository_dir(&sm_gitdir))
			generate_submodule_summary(info, p);
		strbuf_release(&sm_gitdir);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ submodule_summary_callback(ಸಂರಚನಾ diff_queue_struct *q,
				       ಸಂರಚನಾ diff_options *options UNUSED,
				       ಶೂನ್ಯ *data)
{
	ಪೂರ್ಣಾಙ್ಕ i;
	ಸಂರಚನಾ module_cb_list *list = data;
	ಪ್ರತಿ (i = 0; i < q->nr; i++) {
		ಸಂರಚನಾ diff_filepair *p = q->queue[i];
		ಸಂರಚನಾ module_cb *temp;

		ಯದಿ (!S_ISGITLINK(p->one->mode) && !S_ISGITLINK(p->two->mode))
			ಅನುವರ್ತ;
		temp = xmalloc(ಪರಿಮಾಣ(*temp));
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

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *get_diff_cmd(ಗಣನಾ diff_cmd diff_cmd)
{
	ವಿಕಲ್ಪ (diff_cmd) {
	ಪಕ್ಷ DIFF_INDEX: ಪ್ರತ್ಯಾಗಮ "diff-index";
	ಪಕ್ಷ DIFF_FILES: ಪ್ರತ್ಯಾಗಮ "diff-files";
	ಸ್ವಭಾವ: BUG("bad diff_cmd value %d", diff_cmd);
	}
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ compute_summary_module_list(ಸಂರಚನಾ object_id *head_oid,
				       ಸಂರಚನಾ summary_cb *info,
				       ಗಣನಾ diff_cmd diff_cmd)
{
	ಸಂರಚನಾ strvec diff_args = STRVEC_INIT;
	ಸಂರಚನಾ rev_info rev;
	ಸಂರಚನಾ module_cb_list list = MODULE_CB_LIST_INIT;
	ಪೂರ್ಣಾಙ್ಕ ret = 0;

	strvec_ತಳ್ಳು(&diff_args, get_diff_cmd(diff_cmd));
	ಯದಿ (info->cached)
		strvec_ತಳ್ಳು(&diff_args, "--cached");
	strvec_pushl(&diff_args, "--ignore-submodules=dirty", "--raw", ರಿಕ್ತ);
	ಯದಿ (head_oid)
		strvec_ತಳ್ಳು(&diff_args, oid_to_hex(head_oid));
	strvec_ತಳ್ಳು(&diff_args, "--");
	ಯದಿ (info->argc)
		strvec_pushv(&diff_args, info->argv);

	repo_config(the_repository, git_diff_basic_config, ರಿಕ್ತ);
	repo_init_revisions(the_repository, &rev, info->prefix);
	rev.abbrev = 0;
	precompose_argv_prefix(diff_args.nr, diff_args.v, ರಿಕ್ತ);
	setup_revisions_from_strvec(&diff_args, &rev, ರಿಕ್ತ);
	rev.diffopt.output_format = DIFF_FORMAT_NO_OUTPUT | DIFF_FORMAT_CALLBACK;
	rev.diffopt.format_callback = submodule_summary_callback;
	rev.diffopt.format_callback_data = &list;

	ಯದಿ (!info->cached) {
		ಯದಿ (diff_cmd == DIFF_INDEX)
			setup_work_ವೃಕ್ಷ(the_repository);
		ಯದಿ (repo_read_index_preload(the_repository, &rev.diffopt.pathspec, 0) < 0) {
			perror("repo_read_index_preload");
			ret = -1;
			ಗಚ್ಛ cleanup;
		}
	} ಅನ್ಯಥಾ ಯದಿ (repo_read_index(the_repository) < 0) {
		perror("repo_read_cache");
		ret = -1;
		ಗಚ್ಛ cleanup;
	}

	ಯದಿ (diff_cmd == DIFF_INDEX)
		run_diff_index(&rev, info->cached ? DIFF_INDEX_CACHED : 0);
	ಅನ್ಯಥಾ
		run_diff_files(&rev, 0);
	prepare_submodule_summary(info, &list);
cleanup:
	strvec_clear(&diff_args);
	release_revisions(&rev);
	module_cb_list_release(&list);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ module_summary(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
			  ಸಂರಚನಾ repository *repo UNUSED)
{
	ಸಂರಚನಾ summary_cb info = SUMMARY_CB_INIT;
	ಪೂರ್ಣಾಙ್ಕ cached = 0;
	ಪೂರ್ಣಾಙ್ಕ for_status = 0;
	ಪೂರ್ಣಾಙ್ಕ files = 0;
	ಪೂರ್ಣಾಙ್ಕ summary_limit = -1;
	ಗಣನಾ diff_cmd diff_cmd = DIFF_INDEX;
	ಸಂರಚನಾ object_id head_oid;
	ಪೂರ್ಣಾಙ್ಕ ret;
	ಸಂರಚನಾ option module_summary_options[] = {
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
	ಧ್ರುವ ವರ್ಣ *ಧ್ರುವ git_submodule_helper_usage[] = {
		N_("git submodule summary [<options>] [<commit>] [--] [<path>]"),
		ರಿಕ್ತ
	};

	argc = parse_options(argc, argv, prefix, module_summary_options,
			     git_submodule_helper_usage, 0);

	ಯದಿ (!summary_limit)
		ಪ್ರತ್ಯಾಗಮ 0;

	ಯದಿ (!repo_get_oid(the_repository, argc ? argv[0] : "HEAD", &head_oid)) {
		ಯದಿ (argc) {
			argv++;
			argc--;
		}
	} ಅನ್ಯಥಾ ಯದಿ (!argc || !strcmp(argv[0], "HEAD")) {
		/* before the first commit: compare with an empty tree */
		oidcpy(&head_oid, the_hash_algo->empty_ವೃಕ್ಷ);
		ಯದಿ (argc) {
			argv++;
			argc--;
		}
	} ಅನ್ಯಥಾ {
		ಯದಿ (repo_get_oid(the_repository, "HEAD", &head_oid))
			die(_("could not fetch a revision for HEAD"));
	}

	ಯದಿ (files) {
		ಯದಿ (cached)
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

	ret = compute_summary_module_list((diff_cmd == DIFF_INDEX) ? &head_oid : ರಿಕ್ತ,
					  &info, diff_cmd);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ module_gitdir(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix UNUSED,
			 ಸಂರಚನಾ repository *repo)
{
	ಸಂರಚನಾ strbuf gitdir = STRBUF_INIT;

	ಯದಿ (argc != 2)
		usage(_("git submodule--helper gitdir <name>"));

	submodule_name_to_gitdir(&gitdir, repo, argv[1]);

	printf("%s\n", gitdir.buf);

	strbuf_release(&gitdir);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ module_migrate(ಪೂರ್ಣಾಙ್ಕ argc UNUSED, ಧ್ರುವ ವರ್ಣ **argv UNUSED,
			  ಧ್ರುವ ವರ್ಣ *prefix UNUSED, ಸಂರಚನಾ repository *repo)
{
	ಸಂರಚನಾ strbuf module_dir = STRBUF_INIT;
	DIR *dir;
	ಸಂರಚನಾ dirent *de;
	ಪೂರ್ಣಾಙ್ಕ repo_version = 0;

	repo_git_path_append(repo, &module_dir, "modules/");

	dir = opendir(module_dir.buf);
	ಯದಿ (!dir)
		die(_("could not open '%s'"), module_dir.buf);

	ಯಾವತ್ ((de = readdir(dir))) {
		ಸಂರಚನಾ strbuf gitdir_path = STRBUF_INIT;
		ವರ್ಣ *ಕೀಲಿ;
		ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ;

		ಯದಿ (is_dot_or_dotdot(de->d_name))
			ಅನುವರ್ತ;

		strbuf_addf(&gitdir_path, "%s/%s", module_dir.buf, de->d_name);
		ಯದಿ (!is_git_directory(gitdir_path.buf)) {
			strbuf_release(&gitdir_path);
			ಅನುವರ್ತ;
		}
		strbuf_release(&gitdir_path);

		ಕೀಲಿ = xstrfmt("submodule.%s.gitdir", de->d_name);
		ಯದಿ (!repo_config_get_string_tmp(repo, ಕೀಲಿ, &ಮೌಲ್ಯ)) {
			/* Already has a gitdir config, nothing to do. */
			ಮುಕ್ತ(ಕೀಲಿ);
			ಅನುವರ್ತ;
		}
		ಮುಕ್ತ(ಕೀಲಿ);

		create_default_gitdir_config(de->d_name);
	}

	closedir(dir);
	strbuf_release(&module_dir);

	repo_config_get_int(the_repository, "core.repositoryformatversion", &repo_version);
	ಯದಿ (repo_version == 0 &&
	    repo_config_set_gently(repo, "core.repositoryformatversion", "1"))
		die(_("could not set core.repositoryformatversion to 1.\n"
		      "Please set it for migration to work, for example:\n"
		      "git config core.repositoryformatversion 1"));

	ಯದಿ (repo_config_set_gently(repo, "extensions.submodulePathConfig", "true"))
		die(_("could not enable submodulePathConfig extension. It is required\n"
		      "for migration to work. Please enable it in the root repo:\n"
		      "git config extensions.submodulePathConfig true"));

	repo->repository_format_submodule_path_cfg = 1;

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸಂರಚನಾ sync_cb {
	ಧ್ರುವ ವರ್ಣ *prefix;
	ಧ್ರುವ ವರ್ಣ *super_prefix;
	unsigned ಪೂರ್ಣಾಙ್ಕ flags;
};
#define SYNC_CB_INIT { 0 }

ಸ್ಥಿರ ಶೂನ್ಯ sync_submodule(ಧ್ರುವ ವರ್ಣ *path, ಧ್ರುವ ವರ್ಣ *prefix,
			   ಧ್ರುವ ವರ್ಣ *super_prefix, unsigned ಪೂರ್ಣಾಙ್ಕ flags)
{
	ಧ್ರುವ ಸಂರಚನಾ submodule *sub;
	ವರ್ಣ *remote_ಕೀಲಿ = ರಿಕ್ತ;
	ವರ್ಣ *sub_origin_url, *super_config_url, *displaypath, *default_remote;
	ಸಂರಚನಾ strbuf sb = STRBUF_INIT;
	ವರ್ಣ *sub_config_path = ರಿಕ್ತ;
	ಪೂರ್ಣಾಙ್ಕ code;

	ಯದಿ (!is_submodule_active(the_repository, path))
		ಪ್ರತ್ಯಾಗಮ;

	ಯದಿ (validate_submodule_path(path) < 0)
		die(ರಿಕ್ತ);

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);

	ಯದಿ (sub && sub->url) {
		ಯದಿ (starts_with_dot_dot_slash(sub->url) ||
		    starts_with_dot_slash(sub->url)) {
			ವರ್ಣ *up_path = get_up_path(path);

			sub_origin_url = resolve_relative_url(sub->url, up_path, 1);
			super_config_url = resolve_relative_url(sub->url, ರಿಕ್ತ, 1);
			ಮುಕ್ತ(up_path);
		} ಅನ್ಯಥಾ {
			sub_origin_url = xstrdup(sub->url);
			super_config_url = xstrdup(sub->url);
		}
	} ಅನ್ಯಥಾ {
		sub_origin_url = xstrdup("");
		super_config_url = xstrdup("");
	}

	displaypath = get_submodule_displaypath(path, prefix, super_prefix);

	ಯದಿ (!(flags & OPT_QUIET))
		printf(_("Synchronizing submodule url for '%s'\n"),
			 displaypath);

	strbuf_reset(&sb);
	strbuf_addf(&sb, "submodule.%s.url", sub->name);
	ಯದಿ (repo_config_set_gently(the_repository, sb.buf, super_config_url))
		die(_("failed to register url for submodule path '%s'"),
		      displaypath);

	ಯದಿ (!is_submodule_populated_gently(path, ರಿಕ್ತ))
		ಗಚ್ಛ cleanup;

	strbuf_reset(&sb);
	code = get_default_remote_submodule(path, &default_remote);
	ಯದಿ (code)
		exit(code);

	remote_ಕೀಲಿ = xstrfmt("remote.%s.url", default_remote);
	ಮುಕ್ತ(default_remote);

	submodule_to_gitdir(the_repository, &sb, path);
	strbuf_addstr(&sb, "/config");

	ಯದಿ (repo_config_set_in_file_gently(the_repository, sb.buf, remote_ಕೀಲಿ, ರಿಕ್ತ, sub_origin_url))
		die(_("failed to update remote for submodule '%s'"),
		      path);

	ಯದಿ (flags & OPT_RECURSIVE) {
		ಸಂರಚನಾ child_process cpr = CHILD_PROCESS_INIT;

		cpr.git_cmd = 1;
		cpr.dir = path;
		prepare_submodule_repo_env(&cpr.env);

		strvec_pushl(&cpr.args, "submodule--helper", "sync",
			     "--recursive", ರಿಕ್ತ);
		strvec_pushf(&cpr.args, "--super-prefix=%s/", displaypath);

		ಯದಿ (flags & OPT_QUIET)
			strvec_ತಳ್ಳು(&cpr.args, "--quiet");

		ಯದಿ (run_command(&cpr))
			die(_("failed to recurse into submodule '%s'"),
			      path);
	}

cleanup:
	ಮುಕ್ತ(super_config_url);
	ಮುಕ್ತ(sub_origin_url);
	strbuf_release(&sb);
	ಮುಕ್ತ(remote_ಕೀಲಿ);
	ಮುಕ್ತ(displaypath);
	ಮುಕ್ತ(sub_config_path);
}

ಸ್ಥಿರ ಶೂನ್ಯ sync_submodule_cb(ಧ್ರುವ ಸಂರಚನಾ cache_entry *list_item, ಶೂನ್ಯ *cb_data)
{
	ಸಂರಚನಾ sync_cb *info = cb_data;

	sync_submodule(list_item->name, info->prefix, info->super_prefix,
		       info->flags);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ module_sync(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
		       ಸಂರಚನಾ repository *repo UNUSED)
{
	ಸಂರಚನಾ sync_cb info = SYNC_CB_INIT;
	ಸಂರಚನಾ pathspec pathspec = { 0 };
	ಸಂರಚನಾ module_list list = MODULE_LIST_INIT;
	ಪೂರ್ಣಾಙ್ಕ quiet = 0;
	ಪೂರ್ಣಾಙ್ಕ recursive = 0;
	ಸಂರಚನಾ option module_sync_options[] = {
		OPT__SUPER_PREFIX(&info.super_prefix),
		OPT__QUIET(&quiet, N_("suppress output of synchronizing submodule url")),
		OPT_BOOL(0, "recursive", &recursive,
			N_("recurse into nested submodules")),
		OPT_END()
	};
	ಧ್ರುವ ವರ್ಣ *ಧ್ರುವ git_submodule_helper_usage[] = {
		N_("git submodule sync [--quiet] [--recursive] [<path>]"),
		ರಿಕ್ತ
	};
	ಪೂರ್ಣಾಙ್ಕ ret = 1;

	argc = parse_options(argc, argv, prefix, module_sync_options,
			     git_submodule_helper_usage, 0);

	ಯದಿ (module_list_compute(argv, prefix, &pathspec, &list) < 0)
		ಗಚ್ಛ cleanup;

	info.prefix = prefix;
	ಯದಿ (quiet)
		info.flags |= OPT_QUIET;
	ಯದಿ (recursive)
		info.flags |= OPT_RECURSIVE;

	for_each_listed_submodule(&list, sync_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸಂರಚನಾ deinit_cb {
	ಧ್ರುವ ವರ್ಣ *prefix;
	unsigned ಪೂರ್ಣಾಙ್ಕ flags;
};
#define DEINIT_CB_INIT { 0 }

ಸ್ಥಿರ ಶೂನ್ಯ deinit_submodule(ಧ್ರುವ ವರ್ಣ *path, ಧ್ರುವ ವರ್ಣ *prefix,
			     unsigned ಪೂರ್ಣಾಙ್ಕ flags)
{
	ಧ್ರುವ ಸಂರಚನಾ submodule *sub;
	ವರ್ಣ *displaypath = ರಿಕ್ತ;
	ಸಂರಚನಾ child_process cp_config = CHILD_PROCESS_INIT;
	ಸಂರಚನಾ strbuf sb_config = STRBUF_INIT;
	ವರ್ಣ *sub_git_dir = xstrfmt("%s/.git", path);

	ಯದಿ (validate_submodule_path(path) < 0)
		die(ರಿಕ್ತ);

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);

	ಯದಿ (!sub || !sub->name)
		ಗಚ್ಛ cleanup;

	displaypath = get_submodule_displaypath(path, prefix, ರಿಕ್ತ);

	/* remove the submodule work tree (unless the user already did it) */
	ಯದಿ (is_directory(path)) {
		ಸಂರಚನಾ strbuf sb_rm = STRBUF_INIT;
		ಧ್ರುವ ವರ್ಣ *format;

		ಯದಿ (is_directory(sub_git_dir)) {
			ಯದಿ (!(flags & OPT_QUIET))
				warning(_("Submodule work tree '%s' contains a .git "
					  "directory. This will be replaced with a "
					  ".git file by using absorbgitdirs."),
					displaypath);

			absorb_git_dir_into_superproject(path, ರಿಕ್ತ);

		}

		ಯದಿ (!(flags & OPT_FORCE)) {
			ಸಂರಚನಾ child_process cp_rm = CHILD_PROCESS_INIT;

			cp_rm.git_cmd = 1;
			strvec_pushl(&cp_rm.args, "rm", "-qn",
				     path, ರಿಕ್ತ);

			ಯದಿ (run_command(&cp_rm))
				die(_("Submodule work tree '%s' contains local "
				      "modifications; use '-f' to discard them"),
				      displaypath);
		}

		strbuf_addstr(&sb_rm, path);

		ಯದಿ (!remove_dir_recursively(&sb_rm, 0))
			format = _("Cleared directory '%s'\n");
		ಅನ್ಯಥಾ
			format = _("Could not remove submodule work tree '%s'\n");

		ಯದಿ (!(flags & OPT_QUIET))
			printf(format, displaypath);

		submodule_unset_core_worktree(sub);

		strbuf_release(&sb_rm);
	}

	ಯದಿ (mkdir(path, 0777))
		printf(_("could not create empty submodule directory %s"),
		      displaypath);

	cp_config.git_cmd = 1;
	strvec_pushl(&cp_config.args, "config", "--get-regexp", ರಿಕ್ತ);
	strvec_pushf(&cp_config.args, "submodule.%s\\.", sub->name);

	/* remove the .git/config entries (unless the user already did it) */
	ಯದಿ (!capture_command(&cp_config, &sb_config, 0) && sb_config.len) {
		ವರ್ಣ *sub_ಕೀಲಿ = xstrfmt("submodule.%s", sub->name);

		/*
		 * remove the whole section so we have a clean state when
		 * the user later decides to init this submodule again
		 */
		repo_config_rename_section_in_file(the_repository, ರಿಕ್ತ, sub_ಕೀಲಿ, ರಿಕ್ತ);
		ಯದಿ (!(flags & OPT_QUIET))
			printf(_("Submodule '%s' (%s) unregistered for path '%s'\n"),
				 sub->name, sub->url, displaypath);
		ಮುಕ್ತ(sub_ಕೀಲಿ);
	}

cleanup:
	ಮುಕ್ತ(displaypath);
	ಮುಕ್ತ(sub_git_dir);
	strbuf_release(&sb_config);
}

ಸ್ಥಿರ ಶೂನ್ಯ deinit_submodule_cb(ಧ್ರುವ ಸಂರಚನಾ cache_entry *list_item,
				ಶೂನ್ಯ *cb_data)
{
	ಸಂರಚನಾ deinit_cb *info = cb_data;
	deinit_submodule(list_item->name, info->prefix, info->flags);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ module_deinit(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
			 ಸಂರಚನಾ repository *repo UNUSED)
{
	ಸಂರಚನಾ deinit_cb info = DEINIT_CB_INIT;
	ಸಂರಚನಾ pathspec pathspec = { 0 };
	ಸಂರಚನಾ module_list list = MODULE_LIST_INIT;
	ಪೂರ್ಣಾಙ್ಕ quiet = 0;
	ಪೂರ್ಣಾಙ್ಕ force = 0;
	ಪೂರ್ಣಾಙ್ಕ all = 0;
	ಸಂರಚನಾ option module_deinit_options[] = {
		OPT__QUIET(&quiet, N_("suppress submodule status output")),
		OPT__FORCE(&force, N_("remove submodule working trees even if they contain local changes"), 0),
		OPT_BOOL(0, "all", &all, N_("unregister all submodules")),
		OPT_END()
	};
	ಧ್ರುವ ವರ್ಣ *ಧ್ರುವ git_submodule_helper_usage[] = {
		N_("git submodule deinit [--quiet] [-f | --force] [--all | [--] [<path>...]]"),
		ರಿಕ್ತ
	};
	ಪೂರ್ಣಾಙ್ಕ ret = 1;

	argc = parse_options(argc, argv, prefix, module_deinit_options,
			     git_submodule_helper_usage, 0);

	ಯದಿ (all && argc) {
		error("pathspec and --all are incompatible");
		usage_with_options(git_submodule_helper_usage,
				   module_deinit_options);
	}

	ಯದಿ (!argc && !all)
		die(_("Use '--all' if you really want to deinitialize all submodules"));

	ಯದಿ (module_list_compute(argv, prefix, &pathspec, &list) < 0)
		ಗಚ್ಛ cleanup;

	info.prefix = prefix;
	ಯದಿ (quiet)
		info.flags |= OPT_QUIET;
	ಯದಿ (force)
		info.flags |= OPT_FORCE;

	for_each_listed_submodule(&list, deinit_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸಂರಚನಾ module_clone_data {
	ಧ್ರುವ ವರ್ಣ *prefix;
	ಧ್ರುವ ವರ್ಣ *path;
	ಧ್ರುವ ವರ್ಣ *name;
	ಧ್ರುವ ವರ್ಣ *url;
	ಪೂರ್ಣಾಙ್ಕ depth;
	ಸಂರಚನಾ list_objects_filter_options *filter_options;
	ಗಣನಾ ref_storage_format ref_storage_format;
	unsigned ಪೂರ್ಣಾಙ್ಕ quiet: 1;
	unsigned ಪೂರ್ಣಾಙ್ಕ progress: 1;
	unsigned ಪೂರ್ಣಾಙ್ಕ dissociate: 1;
	unsigned ಪೂರ್ಣಾಙ್ಕ require_init: 1;
	ಪೂರ್ಣಾಙ್ಕ single_branch;
};
#define MODULE_CLONE_DATA_INIT { \
	.single_branch = -1, \
	.ref_storage_format = REF_STORAGE_FORMAT_UNKNOWN, \
}

ಸಂರಚನಾ submodule_alternate_setup {
	ಧ್ರುವ ವರ್ಣ *submodule_name;
	ಗಣನಾ SUBMODULE_ALTERNATE_ERROR_MODE {
		SUBMODULE_ALTERNATE_ERROR_DIE,
		SUBMODULE_ALTERNATE_ERROR_INFO,
		SUBMODULE_ALTERNATE_ERROR_IGNORE
	} error_mode;
	ಸಂರಚನಾ string_list *reference;
};
#define SUBMODULE_ALTERNATE_SETUP_INIT { \
	.error_mode = SUBMODULE_ALTERNATE_ERROR_IGNORE, \
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ alternate_error_advice[] = N_(
"An alternate computed from a superproject's alternate is invalid.\n"
"To allow Git to clone without an alternate in such a case, set\n"
"submodule.alternateErrorStrategy to 'info' or, equivalently, clone with\n"
"'--reference-if-able' instead of '--reference'."
);

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ add_possible_reference_from_superproject(
		ಸಂರಚನಾ odb_files_dir *alt_odb, ಶೂನ್ಯ *sas_cb)
{
	ಸಂರಚನಾ submodule_alternate_setup *sas = sas_cb;
	size_t len;

	/*
	 * If the alternate object store is another repository, try the
	 * standard layout with .git/(modules/<name>)+/objects
	 */
	ಯದಿ (strip_suffix(alt_odb->abspath, "/objects", &len)) {
		ಸಂರಚನಾ repository alternate;
		ವರ್ಣ *sm_alternate;
		ಸಂರಚನಾ strbuf sb = STRBUF_INIT;
		ಸಂರಚನಾ strbuf err = STRBUF_INIT;
		strbuf_add(&sb, alt_odb->abspath, len);

		ಯದಿ (repo_init(&alternate, sb.buf, ರಿಕ್ತ) < 0)
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
		ಯದಿ (sm_alternate) {
			ವರ್ಣ *p = strbuf_detach(&sb, ರಿಕ್ತ);

			string_list_append(sas->reference, p)->util = p;
			ಮುಕ್ತ(sm_alternate);
		} ಅನ್ಯಥಾ {
			ವಿಕಲ್ಪ (sas->error_mode) {
			ಪಕ್ಷ SUBMODULE_ALTERNATE_ERROR_DIE:
				ಯದಿ (advice_enabled(ADVICE_SUBMODULE_ALTERNATE_ERROR_STRATEGY_DIE))
					advise(_(alternate_error_advice));
				die(_("submodule '%s' cannot add alternate: %s"),
				    sas->submodule_name, err.buf);
			ಪಕ್ಷ SUBMODULE_ALTERNATE_ERROR_INFO:
				fprintf_ln(stderr, _("submodule '%s' cannot add alternate: %s"),
					sas->submodule_name, err.buf);
			ಪಕ್ಷ SUBMODULE_ALTERNATE_ERROR_IGNORE:
				; /* nothing */
			}
		}

		strbuf_release(&err);
		strbuf_release(&sb);
	}

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ prepare_possible_alternates(ಧ್ರುವ ವರ್ಣ *sm_name,
		ಸಂರಚನಾ string_list *reference)
{
	ವರ್ಣ *sm_alternate = ರಿಕ್ತ, *error_strategy = ರಿಕ್ತ;
	ಸಂರಚನಾ submodule_alternate_setup sas = SUBMODULE_ALTERNATE_SETUP_INIT;

	repo_config_get_string(the_repository, "submodule.alternateLocation", &sm_alternate);
	ಯದಿ (!sm_alternate)
		ಪ್ರತ್ಯಾಗಮ;

	repo_config_get_string(the_repository, "submodule.alternateErrorStrategy", &error_strategy);

	ಯದಿ (!error_strategy)
		error_strategy = xstrdup("die");

	sas.submodule_name = sm_name;
	sas.reference = reference;
	ಯದಿ (!strcmp(error_strategy, "die"))
		sas.error_mode = SUBMODULE_ALTERNATE_ERROR_DIE;
	ಅನ್ಯಥಾ ಯದಿ (!strcmp(error_strategy, "info"))
		sas.error_mode = SUBMODULE_ALTERNATE_ERROR_INFO;
	ಅನ್ಯಥಾ ಯದಿ (!strcmp(error_strategy, "ignore"))
		sas.error_mode = SUBMODULE_ALTERNATE_ERROR_IGNORE;
	ಅನ್ಯಥಾ
		die(_("Value '%s' for submodule.alternateErrorStrategy is not recognized"), error_strategy);

	ಯದಿ (!strcmp(sm_alternate, "superproject"))
		odb_for_each_alternate(the_repository->objects,
				       add_possible_reference_from_superproject, &sas);
	ಅನ್ಯಥಾ ಯದಿ (!strcmp(sm_alternate, "no"))
		; /* do nothing */
	ಅನ್ಯಥಾ
		die(_("Value '%s' for submodule.alternateLocation is not recognized"), sm_alternate);

	ಮುಕ್ತ(sm_alternate);
	ಮುಕ್ತ(error_strategy);
}

ಸ್ಥಿರ ವರ್ಣ *clone_submodule_sm_gitdir(ಧ್ರುವ ವರ್ಣ *name)
{
	ಸಂರಚನಾ strbuf sb = STRBUF_INIT;
	ವರ್ಣ *sm_gitdir;

	submodule_name_to_gitdir(&sb, the_repository, name);
	sm_gitdir = absolute_pathdup(sb.buf);
	strbuf_release(&sb);

	ಪ್ರತ್ಯಾಗಮ sm_gitdir;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ dir_contains_only_dotgit(ಧ್ರುವ ವರ್ಣ *path)
{
	DIR *dir = opendir(path);
	ಸಂರಚನಾ dirent *e;
	ಪೂರ್ಣಾಙ್ಕ ret = 1;

	ಯದಿ (!dir)
		ಪ್ರತ್ಯಾಗಮ 0;

	e = readdir_skip_dot_and_dotdot(dir);
	ಯದಿ (!e)
		ret = 0;
	ಅನ್ಯಥಾ ಯದಿ (strcmp(DEFAULT_GIT_DIR_ENVIRONMENT, e->d_name) ||
		 (e = readdir_skip_dot_and_dotdot(dir))) {
		error("unexpected item '%s' in '%s'", e->d_name, path);
		ret = 0;
	}

	closedir(dir);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ clone_submodule(ಧ್ರುವ ಸಂರಚನಾ module_clone_data *clone_data,
			   ಸಂರಚನಾ string_list *reference)
{
	ವರ್ಣ *p;
	ವರ್ಣ *sm_gitdir = clone_submodule_sm_gitdir(clone_data->name);
	ವರ್ಣ *sm_alternate = ರಿಕ್ತ, *error_strategy = ರಿಕ್ತ;
	ಸಂರಚನಾ stat st;
	ಸಂರಚನಾ child_process cp = CHILD_PROCESS_INIT;
	ಧ್ರುವ ವರ್ಣ *clone_data_path = clone_data->path;
	ವರ್ಣ *to_free = ರಿಕ್ತ;

	ಯದಿ (validate_submodule_path(clone_data_path) < 0)
		die(ರಿಕ್ತ);

	ಯದಿ (!is_absolute_path(clone_data->path))
		clone_data_path = to_free = xstrfmt("%s/%s", repo_get_work_ವೃಕ್ಷ(the_repository),
						    clone_data->path);

	ಯದಿ (!file_exists(sm_gitdir)) {
		ಯದಿ (clone_data->require_init && !stat(clone_data_path, &st) &&
		    !is_empty_dir(clone_data_path))
			die(_("directory not empty: '%s'"), clone_data_path);

		ಯದಿ (safe_create_leading_directories_const(the_repository, sm_gitdir) < 0)
			die(_("could not create directory '%s'"), sm_gitdir);

		prepare_possible_alternates(clone_data->name, reference);

		strvec_ತಳ್ಳು(&cp.args, "clone");
		strvec_ತಳ್ಳು(&cp.args, "--no-checkout");
		ಯದಿ (clone_data->quiet)
			strvec_ತಳ್ಳು(&cp.args, "--quiet");
		ಯದಿ (clone_data->progress)
			strvec_ತಳ್ಳು(&cp.args, "--progress");
		ಯದಿ (clone_data->depth > 0)
			strvec_pushf(&cp.args, "--depth=%d", clone_data->depth);
		ಯದಿ (reference->nr) {
			ಸಂರಚನಾ string_list_item *item;

			for_each_string_list_item(item, reference)
				strvec_pushl(&cp.args, "--reference",
					     item->string, ರಿಕ್ತ);
		}
		ಯದಿ (clone_data->ref_storage_format != REF_STORAGE_FORMAT_UNKNOWN)
			strvec_pushf(&cp.args, "--ref-storage-format=%s",
				     ref_storage_format_to_name(clone_data->ref_storage_format));
		ಯದಿ (clone_data->dissociate)
			strvec_ತಳ್ಳು(&cp.args, "--dissociate");
		ಯದಿ (sm_gitdir && *sm_gitdir)
			strvec_pushl(&cp.args, "--separate-git-dir", sm_gitdir, ರಿಕ್ತ);
		ಯದಿ (clone_data->filter_options && clone_data->filter_options->choice)
			strvec_pushf(&cp.args, "--filter=%s",
				     expand_list_objects_filter_spec(
					     clone_data->filter_options));
		ಯದಿ (clone_data->single_branch >= 0)
			strvec_ತಳ್ಳು(&cp.args, clone_data->single_branch ?
				    "--single-branch" :
				    "--no-single-branch");

		strvec_ತಳ್ಳು(&cp.args, "--");
		strvec_ತಳ್ಳು(&cp.args, clone_data->url);
		strvec_ತಳ್ಳು(&cp.args, clone_data_path);

		cp.git_cmd = 1;
		prepare_submodule_repo_env(&cp.env);
		cp.no_stdin = 1;

		ಯದಿ(run_command(&cp))
			die(_("clone of '%s' into submodule path '%s' failed"),
			    clone_data->url, clone_data_path);

		ಯದಿ (clone_data->require_init && !stat(clone_data_path, &st) &&
		    !dir_contains_only_dotgit(clone_data_path)) {
			ವರ್ಣ *dot_git = xstrfmt("%s/.git", clone_data_path);
			unlink(dot_git);
			ಮುಕ್ತ(dot_git);
			die(_("directory not empty: '%s'"), clone_data_path);
		}
	} ಅನ್ಯಥಾ {
		ವರ್ಣ *path;

		ಯದಿ (clone_data->require_init && !stat(clone_data_path, &st) &&
		    !is_empty_dir(clone_data_path))
			die(_("directory not empty: '%s'"), clone_data_path);
		ಯದಿ (safe_create_leading_directories_const(the_repository, clone_data_path) < 0)
			die(_("could not create directory '%s'"), clone_data_path);
		path = xstrfmt("%s/index", sm_gitdir);
		unlink_or_warn(path);
		ಮುಕ್ತ(path);
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
	ಯದಿ (validate_submodule_git_dir(sm_gitdir, clone_data->name) < 0) {
		ವರ್ಣ *head = xstrfmt("%s/HEAD", sm_gitdir);
		unlink(head);
		ಮುಕ್ತ(head);
		die(_("refusing to create/use '%s' in another submodule's git dir. "
		      "Enabling extensions.submodulePathConfig should fix this."),
		    sm_gitdir);
	}

	connect_work_ವೃಕ್ಷ_and_git_dir(clone_data_path, sm_gitdir, 0);

	p = repo_submodule_path(the_repository, clone_data_path, "config");
	ಯದಿ (!p)
		die(_("could not get submodule directory for '%s'"), clone_data_path);

	/* setup alternateLocation and alternateErrorStrategy in the cloned submodule if needed */
	repo_config_get_string(the_repository, "submodule.alternateLocation", &sm_alternate);
	ಯದಿ (sm_alternate)
		repo_config_set_in_file(the_repository, p, "submodule.alternateLocation",
					sm_alternate);
	repo_config_get_string(the_repository, "submodule.alternateErrorStrategy", &error_strategy);
	ಯದಿ (error_strategy)
		repo_config_set_in_file(the_repository, p, "submodule.alternateErrorStrategy",
					error_strategy);

	ಮುಕ್ತ(sm_alternate);
	ಮುಕ್ತ(error_strategy);

	ಮುಕ್ತ(sm_gitdir);
	ಮುಕ್ತ(p);
	ಮುಕ್ತ(to_free);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ module_clone(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
			ಸಂರಚನಾ repository *repo UNUSED)
{
	ಪೂರ್ಣಾಙ್ಕ dissociate = 0, quiet = 0, progress = 0, require_init = 0;
	ಸಂರಚನಾ module_clone_data clone_data = MODULE_CLONE_DATA_INIT;
	ಸಂರಚನಾ string_list reference = STRING_LIST_INIT_NODUP;
	ಸಂರಚನಾ list_objects_filter_options filter_options =
		LIST_OBJECTS_FILTER_INIT;
	ಧ್ರುವ ವರ್ಣ *ref_storage_format = ರಿಕ್ತ;

	ಸಂರಚನಾ option module_clone_options[] = {
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
	ಧ್ರುವ ವರ್ಣ *ಧ್ರುವ git_submodule_helper_usage[] = {
		N_("git submodule--helper clone [--prefix=<path>] [--quiet] "
		   "[--reference <repository>] [--name <name>] [--depth <depth>] "
		   "[--single-branch] [--filter <filter-spec>] "
		   "--url <url> --path <path>"),
		ರಿಕ್ತ
	};

	argc = parse_options(argc, argv, prefix, module_clone_options,
			     git_submodule_helper_usage, 0);

	ಯದಿ (ref_storage_format) {
		clone_data.ref_storage_format = ref_storage_format_by_name(ref_storage_format);
		ಯದಿ (clone_data.ref_storage_format == REF_STORAGE_FORMAT_UNKNOWN)
			die(_("unknown ref storage format '%s'"), ref_storage_format);
	}
	clone_data.dissociate = !!dissociate;
	clone_data.quiet = !!quiet;
	clone_data.progress = !!progress;
	clone_data.require_init = !!require_init;
	clone_data.filter_options = &filter_options;

	ಯದಿ (argc || !clone_data.url || !clone_data.path || !*(clone_data.path))
		usage_with_options(git_submodule_helper_usage,
				   module_clone_options);

	clone_submodule(&clone_data, &reference);
	list_objects_filter_release(&filter_options);
	string_list_clear(&reference, 1);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ determine_submodule_update_strategy(ಸಂರಚನಾ repository *r,
					       ಪೂರ್ಣಾಙ್ಕ just_cloned,
					       ಧ್ರುವ ವರ್ಣ *path,
					       ಗಣನಾ submodule_update_type update,
					       ಸಂರಚನಾ submodule_update_strategy *out)
{
	ಧ್ರುವ ಸಂರಚನಾ submodule *sub = submodule_from_path(r, null_oid(the_hash_algo), path);
	ವರ್ಣ *ಕೀಲಿ;
	ಧ್ರುವ ವರ್ಣ *val;
	ಪೂರ್ಣಾಙ್ಕ ret;

	/*
	 * NEEDSWORK: audit and ensure that update_submodule() has right
	 * to assume that submodule_from_path() above will always succeed.
	 */
	ಯದಿ (!sub)
		BUG("update_submodule assumes a submodule exists at path (%s)",
		    path);
	ಕೀಲಿ = xstrfmt("submodule.%s.update", sub->name);

	ಯದಿ (update) {
		out->type = update;
	} ಅನ್ಯಥಾ ಯದಿ (!repo_config_get_string_tmp(r, ಕೀಲಿ, &val)) {
		ಯದಿ (parse_submodule_update_strategy(val, out) < 0) {
			ret = die_message(_("Invalid update mode '%s' configured for submodule path '%s'"),
					  val, path);
			ಗಚ್ಛ cleanup;
		}
	} ಅನ್ಯಥಾ ಯದಿ (sub->update_strategy.type != SM_UPDATE_UNSPECIFIED) {
		ಯದಿ (sub->update_strategy.type == SM_UPDATE_COMMAND)
			BUG("how did we read update = !command from .gitmodules?");
		out->type = sub->update_strategy.type;
		out->command = sub->update_strategy.command;
	} ಅನ್ಯಥಾ
		out->type = SM_UPDATE_CHECKOUT;

	ಯದಿ (just_cloned &&
	    (out->type == SM_UPDATE_MERGE ||
	     out->type == SM_UPDATE_REBASE ||
	     out->type == SM_UPDATE_NONE))
		out->type = SM_UPDATE_CHECKOUT;

	ret = 0;
cleanup:
	ಮುಕ್ತ(ಕೀಲಿ);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸಂರಚನಾ update_clone_data {
	ಧ್ರುವ ಸಂರಚನಾ submodule *sub;
	ಸಂರಚನಾ object_id oid;
	unsigned just_cloned;
};

ಸಂರಚನಾ submodule_update_clone {
	/* index into 'update_data.list', the list of submodules to look into for cloning */
	ಪೂರ್ಣಾಙ್ಕ current;

	/* configuration parameters which are passed on to the children */
	ಧ್ರುವ ಸಂರಚನಾ update_data *update_data;

	/* to be consumed by update_submodule() */
	ಸಂರಚನಾ update_clone_data *update_clone;
	ಪೂರ್ಣಾಙ್ಕ update_clone_nr; ಪೂರ್ಣಾಙ್ಕ update_clone_alloc;

	/* If we want to stop as fast as possible and return an error */
	unsigned quickstop : 1;

	/* failed clones to be retried again */
	ಧ್ರುವ ಸಂರಚನಾ cache_entry **failed_clones;
	ಪೂರ್ಣಾಙ್ಕ failed_clones_nr, failed_clones_alloc;
};
#define SUBMODULE_UPDATE_CLONE_INIT { 0 }

ಸ್ಥಿರ ಶೂನ್ಯ submodule_update_clone_release(ಸಂರಚನಾ submodule_update_clone *suc)
{
	ಮುಕ್ತ(suc->update_clone);
	ಮುಕ್ತ(suc->failed_clones);
}

ಸಂರಚನಾ update_data {
	ಧ್ರುವ ವರ್ಣ *prefix;
	ಧ್ರುವ ವರ್ಣ *super_prefix;
	ವರ್ಣ *displaypath;
	ಗಣನಾ submodule_update_type update_default;
	ಸಂರಚನಾ object_id suboid;
	ಸಂರಚನಾ string_list references;
	ಸಂರಚನಾ submodule_update_strategy update_strategy;
	ಸಂರಚನಾ list_objects_filter_options *filter_options;
	ಸಂರಚನಾ module_list list;
	ಗಣನಾ ref_storage_format ref_storage_format;
	ಪೂರ್ಣಾಙ್ಕ depth;
	ಪೂರ್ಣಾಙ್ಕ max_jobs;
	ಪೂರ್ಣಾಙ್ಕ single_branch;
	ಪೂರ್ಣಾಙ್ಕ recommend_shallow;
	unsigned ಪೂರ್ಣಾಙ್ಕ require_init;
	unsigned ಪೂರ್ಣಾಙ್ಕ force;
	unsigned ಪೂರ್ಣಾಙ್ಕ quiet;
	unsigned ಪೂರ್ಣಾಙ್ಕ nofetch;
	unsigned ಪೂರ್ಣಾಙ್ಕ remote;
	unsigned ಪೂರ್ಣಾಙ್ಕ progress;
	unsigned ಪೂರ್ಣಾಙ್ಕ dissociate;
	unsigned ಪೂರ್ಣಾಙ್ಕ init;
	unsigned ಪೂರ್ಣಾಙ್ಕ warn_if_uninitialized;
	unsigned ಪೂರ್ಣಾಙ್ಕ recursive;

	/* copied over from update_clone_data */
	ಸಂರಚನಾ object_id oid;
	unsigned ಪೂರ್ಣಾಙ್ಕ just_cloned;
	ಧ್ರುವ ವರ್ಣ *sm_path;
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

ಸ್ಥಿರ ಶೂನ್ಯ update_data_release(ಸಂರಚನಾ update_data *ud)
{
	ಮುಕ್ತ(ud->displaypath);
	submodule_update_strategy_release(&ud->update_strategy);
	module_list_release(&ud->list);
}

ಸ್ಥಿರ ಶೂನ್ಯ next_submodule_warn_missing(ಸಂರಚನಾ submodule_update_clone *suc,
		ಸಂರಚನಾ strbuf *out, ಧ್ರುವ ವರ್ಣ *displaypath)
{
	/*
	 * Only mention uninitialized submodules when their
	 * paths have been specified.
	 */
	ಯದಿ (suc->update_data->warn_if_uninitialized) {
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
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ prepare_to_clone_next_submodule(ಧ್ರುವ ಸಂರಚನಾ cache_entry *ce,
					   ಸಂರಚನಾ child_process *child,
					   ಸಂರಚನಾ submodule_update_clone *suc,
					   ಸಂರಚನಾ strbuf *out)
{
	ಧ್ರುವ ಸಂರಚನಾ submodule *sub = ರಿಕ್ತ;
	ಧ್ರುವ ವರ್ಣ *url = ರಿಕ್ತ;
	ಧ್ರುವ ವರ್ಣ *update_string;
	ಗಣನಾ submodule_update_type update_type;
	ವರ್ಣ *ಕೀಲಿ;
	ಧ್ರುವ ಸಂರಚನಾ update_data *ud = suc->update_data;
	ವರ್ಣ *displaypath = get_submodule_displaypath(ce->name, ud->prefix,
						      ud->super_prefix);
	ಸಂರಚನಾ strbuf sb = STRBUF_INIT;
	ಪೂರ್ಣಾಙ್ಕ needs_cloning = 0;
	ಪೂರ್ಣಾಙ್ಕ need_free_url = 0;

	ಯದಿ (ce_stage(ce)) {
		strbuf_addf(out, _("Skipping unmerged submodule %s"), displaypath);
		strbuf_addch(out, '\n');
		ಗಚ್ಛ cleanup;
	}

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), ce->name);

	ಯದಿ (!sub) {
		next_submodule_warn_missing(suc, out, displaypath);
		ಗಚ್ಛ cleanup;
	}

	ಕೀಲಿ = xstrfmt("submodule.%s.update", sub->name);
	ಯದಿ (!repo_config_get_string_tmp(the_repository, ಕೀಲಿ, &update_string)) {
		update_type = parse_submodule_update_type(update_string);
	} ಅನ್ಯಥಾ {
		update_type = sub->update_strategy.type;
	}
	ಮುಕ್ತ(ಕೀಲಿ);

	ಯದಿ (suc->update_data->update_strategy.type == SM_UPDATE_NONE
	    || (suc->update_data->update_strategy.type == SM_UPDATE_UNSPECIFIED
		&& update_type == SM_UPDATE_NONE)) {
		strbuf_addf(out, _("Skipping submodule '%s'"), displaypath);
		strbuf_addch(out, '\n');
		ಗಚ್ಛ cleanup;
	}

	/* Check if the submodule has been initialized. */
	ಯದಿ (!is_submodule_active(the_repository, ce->name)) {
		next_submodule_warn_missing(suc, out, displaypath);
		ಗಚ್ಛ cleanup;
	}

	strbuf_reset(&sb);
	strbuf_addf(&sb, "submodule.%s.url", sub->name);
	ಯದಿ (repo_config_get_string_tmp(the_repository, sb.buf, &url)) {
		ಯದಿ (sub->url && (starts_with_dot_slash(sub->url) ||
				 starts_with_dot_dot_slash(sub->url))) {
			url = resolve_relative_url(sub->url, ರಿಕ್ತ, 0);
			need_free_url = 1;
		} ಅನ್ಯಥಾ
			url = sub->url;
	}

	ಯದಿ (!url)
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

	ಯದಿ (!needs_cloning)
		ಗಚ್ಛ cleanup;

	child->git_cmd = 1;
	child->no_stdin = 1;
	child->stdout_to_stderr = 1;
	child->err = -1;
	strvec_ತಳ್ಳು(&child->args, "submodule--helper");
	strvec_ತಳ್ಳು(&child->args, "clone");
	ಯದಿ (suc->update_data->progress)
		strvec_ತಳ್ಳು(&child->args, "--progress");
	ಯದಿ (suc->update_data->quiet)
		strvec_ತಳ್ಳು(&child->args, "--quiet");
	ಯದಿ (suc->update_data->prefix)
		strvec_pushl(&child->args, "--prefix", suc->update_data->prefix, ರಿಕ್ತ);
	ಯದಿ (suc->update_data->recommend_shallow && sub->recommend_shallow == 1)
		strvec_ತಳ್ಳು(&child->args, "--depth=1");
	ಅನ್ಯಥಾ ಯದಿ (suc->update_data->depth)
		strvec_pushf(&child->args, "--depth=%d", suc->update_data->depth);
	ಯದಿ (suc->update_data->filter_options && suc->update_data->filter_options->choice)
		strvec_pushf(&child->args, "--filter=%s",
			     expand_list_objects_filter_spec(suc->update_data->filter_options));
	ಯದಿ (suc->update_data->require_init)
		strvec_ತಳ್ಳು(&child->args, "--require-init");
	ಯದಿ (suc->update_data->ref_storage_format != REF_STORAGE_FORMAT_UNKNOWN)
		strvec_pushf(&child->args, "--ref-storage-format=%s",
			     ref_storage_format_to_name(suc->update_data->ref_storage_format));
	strvec_pushl(&child->args, "--path", sub->path, ರಿಕ್ತ);
	strvec_pushl(&child->args, "--name", sub->name, ರಿಕ್ತ);
	strvec_pushl(&child->args, "--url", url, ರಿಕ್ತ);
	ಯದಿ (suc->update_data->references.nr) {
		ಸಂರಚನಾ string_list_item *item;

		for_each_string_list_item(item, &suc->update_data->references)
			strvec_pushl(&child->args, "--reference", item->string, ರಿಕ್ತ);
	}
	ಯದಿ (suc->update_data->dissociate)
		strvec_ತಳ್ಳು(&child->args, "--dissociate");
	ಯದಿ (suc->update_data->single_branch >= 0)
		strvec_ತಳ್ಳು(&child->args, suc->update_data->single_branch ?
					      "--single-branch" :
					      "--no-single-branch");

cleanup:
	ಮುಕ್ತ(displaypath);
	strbuf_release(&sb);
	ಯದಿ (need_free_url)
		ಮುಕ್ತ((ಶೂನ್ಯ*)url);

	ಪ್ರತ್ಯಾಗಮ needs_cloning;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ update_clone_get_next_task(ಸಂರಚನಾ child_process *child,
				      ಸಂರಚನಾ strbuf *err,
				      ಶೂನ್ಯ *suc_cb,
				      ಶೂನ್ಯ **idx_task_cb)
{
	ಸಂರಚನಾ submodule_update_clone *suc = suc_cb;
	ಧ್ರುವ ಸಂರಚನಾ cache_entry *ce;
	ಪೂರ್ಣಾಙ್ಕ index;

	ಪ್ರತಿ (; suc->current < suc->update_data->list.nr; suc->current++) {
		ce = suc->update_data->list.entries[suc->current];
		ಯದಿ (prepare_to_clone_next_submodule(ce, child, suc, err)) {
			ಪೂರ್ಣಾಙ್ಕ *p = xmalloc(ಪರಿಮಾಣ(*p));

			*p = suc->current;
			*idx_task_cb = p;
			suc->current++;
			ಪ್ರತ್ಯಾಗಮ 1;
		}
	}

	/*
	 * The loop above tried cloning each submodule once, now try the
	 * stragglers again, which we can imagine as an extension of the
	 * entry list.
	 */
	index = suc->current - suc->update_data->list.nr;
	ಯದಿ (index < suc->failed_clones_nr) {
		ಪೂರ್ಣಾಙ್ಕ *p;

		ce = suc->failed_clones[index];
		ಯದಿ (!prepare_to_clone_next_submodule(ce, child, suc, err)) {
			suc->current ++;
			strbuf_addstr(err, "BUG: submodule considered for "
					   "cloning, doesn't need cloning "
					   "any more?\n");
			ಪ್ರತ್ಯಾಗಮ 0;
		}
		p = xmalloc(ಪರಿಮಾಣ(*p));
		*p = suc->current;
		*idx_task_cb = p;
		suc->current ++;
		ಪ್ರತ್ಯಾಗಮ 1;
	}

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ update_clone_start_failure(ಸಂರಚನಾ strbuf *err UNUSED,
				      ಶೂನ್ಯ *suc_cb,
				      ಶೂನ್ಯ *idx_task_cb UNUSED)
{
	ಸಂರಚನಾ submodule_update_clone *suc = suc_cb;

	suc->quickstop = 1;
	ಪ್ರತ್ಯಾಗಮ 1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ update_clone_task_finished(ಪೂರ್ಣಾಙ್ಕ result,
				      ಸಂರಚನಾ strbuf *err,
				      ಶೂನ್ಯ *suc_cb,
				      ಶೂನ್ಯ *idx_task_cb)
{
	ಧ್ರುವ ಸಂರಚನಾ cache_entry *ce;
	ಸಂರಚನಾ submodule_update_clone *suc = suc_cb;
	ಪೂರ್ಣಾಙ್ಕ *idxP = idx_task_cb;
	ಪೂರ್ಣಾಙ್ಕ idx = *idxP;

	ಮುಕ್ತ(idxP);

	ಯದಿ (!result)
		ಪ್ರತ್ಯಾಗಮ 0;

	ಯದಿ (idx < suc->update_data->list.nr) {
		ce  = suc->update_data->list.entries[idx];
		strbuf_addf(err, _("Failed to clone '%s'. Retry scheduled"),
			    ce->name);
		strbuf_addch(err, '\n');
		ALLOC_GROW(suc->failed_clones,
			   suc->failed_clones_nr + 1,
			   suc->failed_clones_alloc);
		suc->failed_clones[suc->failed_clones_nr++] = ce;
		ಪ್ರತ್ಯಾಗಮ 0;
	} ಅನ್ಯಥಾ {
		idx -= suc->update_data->list.nr;
		ce  = suc->failed_clones[idx];
		strbuf_addf(err, _("Failed to clone '%s' a second time, aborting"),
			    ce->name);
		strbuf_addch(err, '\n');
		suc->quickstop = 1;
		ಪ್ರತ್ಯಾಗಮ 1;
	}

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ git_update_clone_config(ಧ್ರುವ ವರ್ಣ *var, ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ,
				   ಧ್ರುವ ಸಂರಚನಾ config_context *ctx,
				   ಶೂನ್ಯ *cb)
{
	ಪೂರ್ಣಾಙ್ಕ *max_jobs = cb;

	ಯದಿ (!strcmp(var, "submodule.fetchjobs"))
		*max_jobs = parse_submodule_fetchjobs(var, ಮೌಲ್ಯ, ctx->kvi);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ is_tip_reachable(ಧ್ರುವ ವರ್ಣ *path, ಧ್ರುವ ಸಂರಚನಾ object_id *oid)
{
	ಸಂರಚನಾ child_process cp = CHILD_PROCESS_INIT;
	ಸಂರಚನಾ strbuf rev = STRBUF_INIT;
	ವರ್ಣ *hex = oid_to_hex(oid);
	ಪೂರ್ಣಾಙ್ಕ reachable;

	cp.git_cmd = 1;
	cp.dir = path;
	cp.no_stderr = 1;
	strvec_pushl(&cp.args, "rev-list", "-n", "1", hex, "--not", "--all", ರಿಕ್ತ);

	prepare_submodule_repo_env(&cp.env);

	ಯದಿ (capture_command(&cp, &rev, GIT_MAX_HEXSZ + 1) || rev.len)
		reachable = 0;
	ಅನ್ಯಥಾ
		reachable = 1;

	strbuf_release(&rev);
	ಪ್ರತ್ಯಾಗಮ reachable;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ fetch_in_submodule(ಧ್ರುವ ವರ್ಣ *module_path, ಪೂರ್ಣಾಙ್ಕ depth, ಪೂರ್ಣಾಙ್ಕ quiet,
			      ಧ್ರುವ ಸಂರಚನಾ object_id *oid)
{
	ಸಂರಚನಾ child_process cp = CHILD_PROCESS_INIT;

	prepare_submodule_repo_env(&cp.env);
	cp.git_cmd = 1;
	cp.dir = module_path;

	strvec_ತಳ್ಳು(&cp.args, "fetch");
	ಯದಿ (quiet)
		strvec_ತಳ್ಳು(&cp.args, "--quiet");
	ಯದಿ (depth)
		strvec_pushf(&cp.args, "--depth=%d", depth);
	ಯದಿ (oid) {
		ವರ್ಣ *hex = oid_to_hex(oid);
		ವರ್ಣ *remote;
		ಪೂರ್ಣಾಙ್ಕ code;

		code = get_default_remote_submodule(module_path, &remote);
		ಯದಿ (code) {
			child_process_clear(&cp);
			ಪ್ರತ್ಯಾಗಮ code;
		}

		strvec_pushl(&cp.args, remote, hex, ರಿಕ್ತ);
		ಮುಕ್ತ(remote);
	}

	ಪ್ರತ್ಯಾಗಮ run_command(&cp);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ run_update_command(ಧ್ರುವ ಸಂರಚನಾ update_data *ud, ಪೂರ್ಣಾಙ್ಕ subforce)
{
	ಸಂರಚನಾ child_process cp = CHILD_PROCESS_INIT;
	ವರ್ಣ *oid = oid_to_hex(&ud->oid);
	ಪೂರ್ಣಾಙ್ಕ ret;

	ವಿಕಲ್ಪ (ud->update_strategy.type) {
	ಪಕ್ಷ SM_UPDATE_CHECKOUT:
		cp.git_cmd = 1;
		strvec_pushl(&cp.args, "checkout", "-q", ರಿಕ್ತ);
		ಯದಿ (subforce)
			strvec_ತಳ್ಳು(&cp.args, "-f");
		ಭಙ್ಗ;
	ಪಕ್ಷ SM_UPDATE_REBASE:
		cp.git_cmd = 1;
		strvec_ತಳ್ಳು(&cp.args, "rebase");
		ಯದಿ (ud->quiet)
			strvec_ತಳ್ಳು(&cp.args, "--quiet");
		ಭಙ್ಗ;
	ಪಕ್ಷ SM_UPDATE_MERGE:
		cp.git_cmd = 1;
		strvec_ತಳ್ಳು(&cp.args, "merge");
		ಯದಿ (ud->quiet)
			strvec_ತಳ್ಳು(&cp.args, "--quiet");
		ಭಙ್ಗ;
	ಪಕ್ಷ SM_UPDATE_COMMAND:
		cp.use_shell = 1;
		strvec_ತಳ್ಳು(&cp.args, ud->update_strategy.command);
		ಭಙ್ಗ;
	ಸ್ವಭಾವ:
		BUG("unexpected update strategy type: %d",
		    ud->update_strategy.type);
	}
	strvec_ತಳ್ಳು(&cp.args, oid);

	cp.dir = ud->sm_path;
	prepare_submodule_repo_env(&cp.env);
	ಯದಿ ((ret = run_command(&cp))) {
		ವಿಕಲ್ಪ (ud->update_strategy.type) {
		ಪಕ್ಷ SM_UPDATE_CHECKOUT:
			die_message(_("Unable to checkout '%s' in submodule path '%s'"),
				    oid, ud->displaypath);
			/* No "ret" assignment, use "git checkout"'s */
			ಭಙ್ಗ;
		ಪಕ್ಷ SM_UPDATE_REBASE:
			ret = die_message(_("Unable to rebase '%s' in submodule path '%s'"),
					  oid, ud->displaypath);
			ಭಙ್ಗ;
		ಪಕ್ಷ SM_UPDATE_MERGE:
			ret = die_message(_("Unable to merge '%s' in submodule path '%s'"),
					  oid, ud->displaypath);
			ಭಙ್ಗ;
		ಪಕ್ಷ SM_UPDATE_COMMAND:
			ret = die_message(_("Execution of '%s %s' failed in submodule path '%s'"),
					  ud->update_strategy.command, oid, ud->displaypath);
			ಭಙ್ಗ;
		ಸ್ವಭಾವ:
			BUG("unexpected update strategy type: %d",
			    ud->update_strategy.type);
		}

		ಪ್ರತ್ಯಾಗಮ ret;
	}

	ಯದಿ (ud->quiet)
		ಪ್ರತ್ಯಾಗಮ 0;

	ವಿಕಲ್ಪ (ud->update_strategy.type) {
	ಪಕ್ಷ SM_UPDATE_CHECKOUT:
		printf(_("Submodule path '%s': checked out '%s'\n"),
		       ud->displaypath, oid);
		ಭಙ್ಗ;
	ಪಕ್ಷ SM_UPDATE_REBASE:
		printf(_("Submodule path '%s': rebased into '%s'\n"),
		       ud->displaypath, oid);
		ಭಙ್ಗ;
	ಪಕ್ಷ SM_UPDATE_MERGE:
		printf(_("Submodule path '%s': merged in '%s'\n"),
		       ud->displaypath, oid);
		ಭಙ್ಗ;
	ಪಕ್ಷ SM_UPDATE_COMMAND:
		printf(_("Submodule path '%s': '%s %s'\n"),
		       ud->displaypath, ud->update_strategy.command, oid);
		ಭಙ್ಗ;
	ಸ್ವಭಾವ:
		BUG("unexpected update strategy type: %d",
		    ud->update_strategy.type);
	}

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ run_update_procedure(ಧ್ರುವ ಸಂರಚನಾ update_data *ud)
{
	ಪೂರ್ಣಾಙ್ಕ subforce = is_null_oid(&ud->suboid) || ud->force;

	ಯದಿ (!ud->nofetch) {
		/*
		 * Run fetch only if `oid` isn't present or it
		 * is not reachable from a ref.
		 */
		ಯದಿ (!is_tip_reachable(ud->sm_path, &ud->oid) &&
		    fetch_in_submodule(ud->sm_path, ud->depth, ud->quiet, ರಿಕ್ತ) &&
		    !ud->quiet)
			fprintf_ln(stderr,
				   _("Unable to fetch in submodule path '%s'; "
				     "trying to directly fetch %s:"),
				   ud->displaypath, oid_to_hex(&ud->oid));
		/*
		 * Now we tried the usual fetch, but `oid` may
		 * not be reachable from any of the refs.
		 */
		ಯದಿ (!is_tip_reachable(ud->sm_path, &ud->oid) &&
		    fetch_in_submodule(ud->sm_path, ud->depth, ud->quiet, &ud->oid))
			ಪ್ರತ್ಯಾಗಮ die_message(_("Fetched in submodule path '%s', but it did not "
					     "contain %s. Direct fetching of that commit failed."),
					   ud->displaypath, oid_to_hex(&ud->oid));
	}

	ಪ್ರತ್ಯಾಗಮ run_update_command(ud, subforce);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ remote_submodule_branch(ಧ್ರುವ ವರ್ಣ *path, ಧ್ರುವ ವರ್ಣ **branch)
{
	ಧ್ರುವ ಸಂರಚನಾ submodule *sub;
	ವರ್ಣ *ಕೀಲಿ;
	*branch = ರಿಕ್ತ;

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);
	ಯದಿ (!sub)
		ಪ್ರತ್ಯಾಗಮ die_message(_("could not initialize submodule at path '%s'"),
				   path);

	ಕೀಲಿ = xstrfmt("submodule.%s.branch", sub->name);
	ಯದಿ (repo_config_get_string_tmp(the_repository, ಕೀಲಿ, branch))
		*branch = sub->branch;
	ಮುಕ್ತ(ಕೀಲಿ);

	ಯದಿ (!*branch) {
		*branch = "HEAD";
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (!strcmp(*branch, ".")) {
		ಧ್ರುವ ವರ್ಣ *refname = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
							      "HEAD", 0, ರಿಕ್ತ,
							      ರಿಕ್ತ);

		ಯದಿ (!refname)
			ಪ್ರತ್ಯಾಗಮ die_message(_("No such ref: %s"), "HEAD");

		/* detached HEAD */
		ಯದಿ (!strcmp(refname, "HEAD"))
			ಪ್ರತ್ಯಾಗಮ die_message(_("Submodule (%s) branch configured to inherit "
					     "branch from superproject, but the superproject "
					     "is not on any branch"), sub->name);

		ಯದಿ (!skip_prefix(refname, "refs/heads/", &refname))
			ಪ್ರತ್ಯಾಗಮ die_message(_("Expecting a full ref name, got %s"),
					   refname);

		*branch = refname;
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	/* Our "branch" is coming from repo_config_get_string_tmp() */
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ ensure_core_worktree(ಧ್ರುವ ವರ್ಣ *path)
{
	ಧ್ರುವ ವರ್ಣ *cw;
	ಸಂರಚನಾ repository subrepo;

	ಯದಿ (repo_submodule_init(&subrepo, the_repository, path, null_oid(the_hash_algo)))
		ಪ್ರತ್ಯಾಗಮ die_message(_("could not get a repository handle for submodule '%s'"),
				   path);

	ಯದಿ (!repo_config_get_string_tmp(&subrepo, "core.worktree", &cw)) {
		ವರ್ಣ *cfg_file, *abs_path;
		ಧ್ರುವ ವರ್ಣ *rel_path;
		ಸಂರಚನಾ strbuf sb = STRBUF_INIT;

		cfg_file = repo_git_path(&subrepo, "config");

		abs_path = absolute_pathdup(path);
		rel_path = relative_path(abs_path, subrepo.gitdir, &sb);

		repo_config_set_in_file(the_repository, cfg_file, "core.worktree", rel_path);

		ಮುಕ್ತ(cfg_file);
		ಮುಕ್ತ(abs_path);
		strbuf_release(&sb);
	}

	repo_clear(&subrepo);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *submodule_update_type_to_label(ಗಣನಾ submodule_update_type type)
{
	ವಿಕಲ್ಪ (type) {
	ಪಕ್ಷ SM_UPDATE_CHECKOUT:
		ಪ್ರತ್ಯಾಗಮ "checkout";
	ಪಕ್ಷ SM_UPDATE_MERGE:
		ಪ್ರತ್ಯಾಗಮ "merge";
	ಪಕ್ಷ SM_UPDATE_REBASE:
		ಪ್ರತ್ಯಾಗಮ "rebase";
	ಪಕ್ಷ SM_UPDATE_UNSPECIFIED:
	ಪಕ್ಷ SM_UPDATE_NONE:
	ಪಕ್ಷ SM_UPDATE_COMMAND:
		ಭಙ್ಗ;
	}
	BUG("unreachable with type %d", type);
}

ಸ್ಥಿರ ಶೂನ್ಯ update_data_to_args(ಧ್ರುವ ಸಂರಚನಾ update_data *update_data,
				ಸಂರಚನಾ strvec *args)
{
	ಗಣನಾ submodule_update_type update_type = update_data->update_default;

	strvec_pushl(args, "submodule--helper", "update", "--recursive", ರಿಕ್ತ);
	ಯದಿ (update_data->displaypath)
		strvec_pushf(args, "--super-prefix=%s/",
			     update_data->displaypath);
	strvec_pushf(args, "--jobs=%d", update_data->max_jobs);
	ಯದಿ (update_data->quiet)
		strvec_ತಳ್ಳು(args, "--quiet");
	ಯದಿ (update_data->force)
		strvec_ತಳ್ಳು(args, "--force");
	ಯದಿ (update_data->init)
		strvec_ತಳ್ಳು(args, "--init");
	ಯದಿ (update_data->remote)
		strvec_ತಳ್ಳು(args, "--remote");
	ಯದಿ (update_data->nofetch)
		strvec_ತಳ್ಳು(args, "--no-fetch");
	ಯದಿ (update_data->dissociate)
		strvec_ತಳ್ಳು(args, "--dissociate");
	ಯದಿ (update_data->progress)
		strvec_ತಳ್ಳು(args, "--progress");
	ಯದಿ (update_data->require_init)
		strvec_ತಳ್ಳು(args, "--require-init");
	ಯದಿ (update_data->depth)
		strvec_pushf(args, "--depth=%d", update_data->depth);
	ಯದಿ (update_type != SM_UPDATE_UNSPECIFIED)
		strvec_pushf(args, "--%s",
			     submodule_update_type_to_label(update_type));

	ಯದಿ (update_data->references.nr) {
		ಸಂರಚನಾ string_list_item *item;

		for_each_string_list_item(item, &update_data->references)
			strvec_pushl(args, "--reference", item->string, ರಿಕ್ತ);
	}
	ಯದಿ (update_data->ref_storage_format != REF_STORAGE_FORMAT_UNKNOWN)
		strvec_pushf(args, "--ref-storage-format=%s",
			     ref_storage_format_to_name(update_data->ref_storage_format));
	ಯದಿ (update_data->filter_options && update_data->filter_options->choice)
		strvec_pushf(args, "--filter=%s",
				expand_list_objects_filter_spec(
					update_data->filter_options));
	ಯದಿ (update_data->recommend_shallow == 0)
		strvec_ತಳ್ಳು(args, "--no-recommend-shallow");
	ಅನ್ಯಥಾ ಯದಿ (update_data->recommend_shallow == 1)
		strvec_ತಳ್ಳು(args, "--recommend-shallow");
	ಯದಿ (update_data->single_branch >= 0)
		strvec_ತಳ್ಳು(args, update_data->single_branch ?
				    "--single-branch" :
				    "--no-single-branch");
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ update_submodule(ಸಂರಚನಾ update_data *update_data)
{
	ಪೂರ್ಣಾಙ್ಕ ret;

	ಯದಿ (validate_submodule_path(update_data->sm_path) < 0)
		ಪ್ರತ್ಯಾಗಮ -1;

	ret = determine_submodule_update_strategy(the_repository,
						  update_data->just_cloned,
						  update_data->sm_path,
						  update_data->update_default,
						  &update_data->update_strategy);
	ಯದಿ (ret)
		ಪ್ರತ್ಯಾಗಮ ret;

	ಯದಿ (update_data->just_cloned)
		oidcpy(&update_data->suboid, null_oid(the_hash_algo));
	ಅನ್ಯಥಾ ಯದಿ (repo_resolve_gitlink_ref(the_repository, update_data->sm_path,
					  "HEAD", &update_data->suboid))
		ಪ್ರತ್ಯಾಗಮ die_message(_("Unable to find current revision in submodule path '%s'"),
				   update_data->displaypath);

	ಯದಿ (update_data->remote) {
		ವರ್ಣ *remote_name;
		ಧ್ರುವ ವರ್ಣ *branch;
		ವರ್ಣ *remote_ref;
		ಪೂರ್ಣಾಙ್ಕ code;

		code = get_default_remote_submodule(update_data->sm_path, &remote_name);
		ಯದಿ (code)
			ಪ್ರತ್ಯಾಗಮ code;
		code = remote_submodule_branch(update_data->sm_path, &branch);
		ಯದಿ (code) {
			ಮುಕ್ತ(remote_name);
			ಪ್ರತ್ಯಾಗಮ code;
		}
		remote_ref = xstrfmt("refs/remotes/%s/%s", remote_name, branch);

		ಮುಕ್ತ(remote_name);

		ಯದಿ (!update_data->nofetch) {
			ಯದಿ (fetch_in_submodule(update_data->sm_path, update_data->depth,
					      0, ರಿಕ್ತ)) {
				ಮುಕ್ತ(remote_ref);
				ಪ್ರತ್ಯಾಗಮ die_message(_("Unable to fetch in submodule path '%s'"),
						   update_data->sm_path);
			}
		}

		ಯದಿ (repo_resolve_gitlink_ref(the_repository, update_data->sm_path,
					     remote_ref, &update_data->oid)) {
			ret = die_message(_("Unable to find %s revision in submodule path '%s'"),
					  remote_ref, update_data->sm_path);
			ಮುಕ್ತ(remote_ref);
			ಪ್ರತ್ಯಾಗಮ ret;
		}

		ಮುಕ್ತ(remote_ref);
	}

	ಯದಿ (!oideq(&update_data->oid, &update_data->suboid) || update_data->force) {
		ret = run_update_procedure(update_data);
		ಯದಿ (ret)
			ಪ್ರತ್ಯಾಗಮ ret;
	}

	ಯದಿ (update_data->recursive) {
		ಸಂರಚನಾ child_process cp = CHILD_PROCESS_INIT;
		ಸಂರಚನಾ update_data next = *update_data;

		next.prefix = ರಿಕ್ತ;
		oidcpy(&next.oid, null_oid(the_hash_algo));
		oidcpy(&next.suboid, null_oid(the_hash_algo));

		cp.dir = update_data->sm_path;
		cp.git_cmd = 1;
		prepare_submodule_repo_env(&cp.env);
		update_data_to_args(&next, &cp.args);

		ret = run_command(&cp);
		ಯದಿ (ret)
			die_message(_("Failed to recurse into submodule path '%s'"),
				    update_data->displaypath);
		ಪ್ರತ್ಯಾಗಮ ret;
	}

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ update_submodules(ಸಂರಚನಾ update_data *update_data)
{
	ಪೂರ್ಣಾಙ್ಕ i, ret = 0;
	ಸಂರಚನಾ submodule_update_clone suc = SUBMODULE_UPDATE_CLONE_INIT;
	ಧ್ರುವ ಸಂರಚನಾ run_process_parallel_opts opts = {
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
	ಯದಿ (suc.quickstop) {
		ret = 1;
		ಗಚ್ಛ cleanup;
	}

	ಪ್ರತಿ (i = 0; i < suc.update_clone_nr; i++) {
		ಸಂರಚನಾ update_clone_data ucd = suc.update_clone[i];
		ಪೂರ್ಣಾಙ್ಕ code = 128;

		oidcpy(&update_data->oid, &ucd.oid);
		update_data->just_cloned = ucd.just_cloned;
		update_data->sm_path = ucd.sub->path;

		/*
		 * Verify that the submodule path does not contain any
		 * symlinks; if it does, it might have been tampered with.
		 * TODO: allow exempting it via
		 * `safe.submodule.path` or something
		 */
		ಯದಿ (validate_submodule_path(update_data->sm_path) < 0)
			ಗಚ್ಛ fail;

		code = ensure_core_worktree(update_data->sm_path);
		ಯದಿ (code)
			ಗಚ್ಛ fail;

		update_data->displaypath = get_submodule_displaypath(
			update_data->sm_path, update_data->prefix,
			update_data->super_prefix);
		code = update_submodule(update_data);
		FREE_AND_NULL(update_data->displaypath);
fail:
		ಯದಿ (!code)
			ಅನುವರ್ತ;
		ret = code;
		ಯದಿ (ret == 128)
			ಗಚ್ಛ cleanup;
	}

cleanup:
	submodule_update_clone_release(&suc);
	string_list_clear(&update_data->references, 0);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ module_update(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
			 ಸಂರಚನಾ repository *repo UNUSED)
{
	ಸಂರಚನಾ pathspec pathspec = { 0 };
	ಸಂರಚನಾ pathspec pathspec2 = { 0 };
	ಸಂರಚನಾ update_data opt = UPDATE_DATA_INIT;
	ಸಂರಚನಾ list_objects_filter_options filter_options =
		LIST_OBJECTS_FILTER_INIT;
	ಧ್ರುವ ವರ್ಣ *ref_storage_format = ರಿಕ್ತ;
	ಪೂರ್ಣಾಙ್ಕ ret;
	ಸಂರಚನಾ option module_update_options[] = {
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
	ಧ್ರುವ ವರ್ಣ *ಧ್ರುವ git_submodule_helper_usage[] = {
		N_("git submodule [--quiet] update"
		" [--init [--filter=<filter-spec>]] [--remote]"
		" [-N|--no-fetch] [-f|--force]"
		" [--checkout|--merge|--rebase]"
		" [--[no-]recommend-shallow] [--reference <repository>]"
		" [--recursive] [--[no-]single-branch] [--] [<path>...]"),
		ರಿಕ್ತ
	};

	update_clone_config_from_gitmodules(the_repository, &opt.max_jobs);
	repo_config(the_repository, git_update_clone_config, &opt.max_jobs);

	argc = parse_options(argc, argv, prefix, module_update_options,
			     git_submodule_helper_usage, 0);

	ಯದಿ (opt.require_init)
		opt.init = 1;

	ಯದಿ (filter_options.choice && !opt.init) {
		usage_with_options(git_submodule_helper_usage,
				   module_update_options);
	}

	ಯದಿ (ref_storage_format) {
		opt.ref_storage_format = ref_storage_format_by_name(ref_storage_format);
		ಯದಿ (opt.ref_storage_format == REF_STORAGE_FORMAT_UNKNOWN)
			die(_("unknown ref storage format '%s'"), ref_storage_format);
	}

	opt.filter_options = &filter_options;
	opt.prefix = prefix;

	ಯದಿ (opt.update_default)
		opt.update_strategy.type = opt.update_default;

	ಯದಿ (module_list_compute(argv, prefix, &pathspec, &opt.list) < 0) {
		ret = 1;
		ಗಚ್ಛ cleanup;
	}

	ಯದಿ (pathspec.nr)
		opt.warn_if_uninitialized = 1;

	ಯದಿ (opt.init) {
		ಸಂರಚನಾ module_list list = MODULE_LIST_INIT;
		ಸಂರಚನಾ init_cb info = INIT_CB_INIT;

		ಯದಿ (module_list_compute(argv, opt.prefix,
					&pathspec2, &list) < 0) {
			module_list_release(&list);
			ret = 1;
			ಗಚ್ಛ cleanup;
		}

		/*
		 * If there are no path args and submodule.active is set then,
		 * by default, only initialize 'active' modules.
		 */
		ಯದಿ (!argc && !repo_config_get(the_repository, "submodule.active"))
			module_list_active(&list);

		info.prefix = opt.prefix;
		info.super_prefix = opt.super_prefix;
		ಯದಿ (opt.quiet)
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
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ ತಳ್ಳು_check(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix UNUSED,
		      ಸಂರಚನಾ repository *repo UNUSED)
{
	ಸಂರಚನಾ remote *remote;
	ಧ್ರುವ ವರ್ಣ *superproject_head;
	ವರ್ಣ *head;
	ಪೂರ್ಣಾಙ್ಕ detached_head = 0;
	ಸಂರಚನಾ object_id head_oid;

	ಯದಿ (argc < 3)
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
				   0, &head_oid, ರಿಕ್ತ);
	ಯದಿ (!head)
		die(_("Failed to resolve HEAD as a valid ref."));
	ಯದಿ (!strcmp(head, "HEAD"))
		detached_head = 1;

	/*
	 * The remote must be configured.
	 * This is to avoid pushing to the exact same URL as the parent.
	 */
	remote = pushremote_get(argv[1]);
	ಯದಿ (!remote || remote->origin == REMOTE_UNCONFIGURED)
		die("remote '%s' not configured", argv[1]);

	/* Check the refspec */
	ಯದಿ (argc > 2) {
		ಪೂರ್ಣಾಙ್ಕ i;
		ಸಂರಚನಾ ref *local_refs = get_local_heads();
		ಸಂರಚನಾ refspec refspec = REFSPEC_INIT_ತಳ್ಳು(the_hash_algo);

		refspec_appendn(&refspec, argv + 2, argc - 2);

		ಪ್ರತಿ (i = 0; i < refspec.nr; i++) {
			ಧ್ರುವ ಸಂರಚನಾ refspec_item *rs = &refspec.ವಸ್ತುಗಳು[i];

			ಯದಿ (rs->pattern || rs->matching)
				ಅನುವರ್ತ;

			/* LHS must match a single ref */
			ವಿಕಲ್ಪ (ಎಣಿಕೆ_refspec_match(rs->src, local_refs, ರಿಕ್ತ)) {
			ಪಕ್ಷ 1:
				ಭಙ್ಗ;
			ಪಕ್ಷ 0:
				/*
				 * If LHS matches 'HEAD' then we need to ensure
				 * that it matches the same named branch
				 * checked out in the superproject.
				 */
				ಯದಿ (!strcmp(rs->src, "HEAD")) {
					ಯದಿ (!detached_head &&
					    !strcmp(head, superproject_head))
						ಭಙ್ಗ;
					die("HEAD does not match the named branch in the superproject");
				}
				/* fallthrough */
			ಸ್ವಭಾವ:
				die("src refspec '%s' must name a ref",
				    rs->src);
			}
		}

		refspec_clear(&refspec);
		free_refs(local_refs);
	}
	ಮುಕ್ತ(head);

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ absorb_git_dirs(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
			   ಸಂರಚನಾ repository *repo UNUSED)
{
	ಪೂರ್ಣಾಙ್ಕ i;
	ಸಂರಚನಾ pathspec pathspec = { 0 };
	ಸಂರಚನಾ module_list list = MODULE_LIST_INIT;
	ಧ್ರುವ ವರ್ಣ *super_prefix = ರಿಕ್ತ;
	ಸಂರಚನಾ option embed_gitdir_options[] = {
		OPT__SUPER_PREFIX(&super_prefix),
		OPT_END()
	};
	ಧ್ರುವ ವರ್ಣ *ಧ್ರುವ git_submodule_helper_usage[] = {
		N_("git submodule absorbgitdirs [<options>] [<path>...]"),
		ರಿಕ್ತ
	};
	ಪೂರ್ಣಾಙ್ಕ ret = 1;

	argc = parse_options(argc, argv, prefix, embed_gitdir_options,
			     git_submodule_helper_usage, 0);

	ಯದಿ (module_list_compute(argv, prefix, &pathspec, &list) < 0)
		ಗಚ್ಛ cleanup;

	ಪ್ರತಿ (i = 0; i < list.nr; i++)
		absorb_git_dir_into_superproject(list.entries[i]->name,
						 super_prefix);

	ret = 0;
cleanup:
	clear_pathspec(&pathspec);
	module_list_release(&list);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ module_set_url(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
			  ಸಂರಚನಾ repository *repo UNUSED)
{
	ಪೂರ್ಣಾಙ್ಕ quiet = 0, ret;
	ಧ್ರುವ ವರ್ಣ *newurl;
	ಧ್ರುವ ವರ್ಣ *path;
	ವರ್ಣ *config_name;
	ಸಂರಚನಾ option options[] = {
		OPT__QUIET(&quiet, N_("suppress output for setting url of a submodule")),
		OPT_END()
	};
	ಧ್ರುವ ವರ್ಣ *ಧ್ರುವ usage[] = {
		N_("git submodule set-url [--quiet] <path> <newurl>"),
		ರಿಕ್ತ
	};
	ಧ್ರುವ ಸಂರಚನಾ submodule *sub;

	argc = parse_options(argc, argv, prefix, options, usage, 0);

	ಯದಿ (argc != 2 || !(path = argv[0]) || !(newurl = argv[1]))
		usage_with_options(usage, options);

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);

	ಯದಿ (!sub)
		die(_("no submodule mapping found in .gitmodules for path '%s'"),
		    path);

	config_name = xstrfmt("submodule.%s.url", sub->name);
	ret = config_set_in_gitmodules_file_gently(the_repository, config_name, newurl);

	ಯದಿ (!ret) {
		repo_read_gitmodules(the_repository, 0);
		sync_submodule(sub->path, prefix, ರಿಕ್ತ, quiet ? OPT_QUIET : 0);
	}

	ಮುಕ್ತ(config_name);
	ಪ್ರತ್ಯಾಗಮ !!ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ module_set_branch(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
			     ಸಂರಚನಾ repository *repo UNUSED)
{
	ಪೂರ್ಣಾಙ್ಕ opt_default = 0, ret;
	ಧ್ರುವ ವರ್ಣ *opt_branch = ರಿಕ್ತ;
	ಧ್ರುವ ವರ್ಣ *path;
	ವರ್ಣ *config_name;
	ಸಂರಚನಾ option options[] = {
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
	ಧ್ರುವ ವರ್ಣ *ಧ್ರುವ usage[] = {
		N_("git submodule set-branch [-q|--quiet] (-d|--default) <path>"),
		N_("git submodule set-branch [-q|--quiet] (-b|--branch) <branch> <path>"),
		ರಿಕ್ತ
	};
	ಧ್ರುವ ಸಂರಚನಾ submodule *sub;

	argc = parse_options(argc, argv, prefix, options, usage, 0);

	ಯದಿ (!opt_branch && !opt_default)
		die(_("--branch or --default required"));

	ಯದಿ (opt_branch && opt_default)
		die(_("options '%s' and '%s' cannot be used together"), "--branch", "--default");

	ಯದಿ (argc != 1 || !(path = argv[0]))
		usage_with_options(usage, options);

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);

	ಯದಿ (!sub)
		die(_("no submodule mapping found in .gitmodules for path '%s'"),
		    path);

	config_name = xstrfmt("submodule.%s.branch", sub->name);
	ret = config_set_in_gitmodules_file_gently(the_repository, config_name, opt_branch);

	ಮುಕ್ತ(config_name);
	ಪ್ರತ್ಯಾಗಮ !!ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ module_create_branch(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
				ಸಂರಚನಾ repository *repo UNUSED)
{
	ಗಣನಾ branch_track track;
	ಪೂರ್ಣಾಙ್ಕ quiet = 0, force = 0, reflog = 0, dry_run = 0;
	ಸಂರಚನಾ option options[] = {
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
	ಧ್ರುವ ವರ್ಣ *ಧ್ರುವ usage[] = {
		N_("git submodule--helper create-branch [-f|--force] [--create-reflog] [-q|--quiet] [-t|--track] [-n|--dry-run] <name> <start-oid> <start-name>"),
		ರಿಕ್ತ
	};
	ಸಂರಚನಾ repo_config_ಮೌಲ್ಯಗಳು *cfg = repo_config_ಮೌಲ್ಯಗಳು(the_repository);

	repo_config(the_repository, git_default_config, ರಿಕ್ತ);
	track = cfg->branch_track;
	argc = parse_options(argc, argv, prefix, options, usage, 0);

	ಯದಿ (argc != 3)
		usage_with_options(usage, options);

	ಯದಿ (!quiet && !dry_run)
		printf_ln(_("creating branch '%s'"), argv[0]);

	create_branches_recursively(the_repository, argv[0], argv[1], argv[2],
				    force, reflog, quiet, track, dry_run);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸಂರಚನಾ add_data {
	ಧ್ರುವ ವರ್ಣ *prefix;
	ಧ್ರುವ ವರ್ಣ *branch;
	ಧ್ರುವ ವರ್ಣ *reference_path;
	ವರ್ಣ *sm_path;
	ಧ್ರುವ ವರ್ಣ *sm_name;
	ಧ್ರುವ ವರ್ಣ *repo;
	ಧ್ರುವ ವರ್ಣ *realrepo;
	ಗಣನಾ ref_storage_format ref_storage_format;
	ಪೂರ್ಣಾಙ್ಕ depth;
	unsigned ಪೂರ್ಣಾಙ್ಕ force: 1;
	unsigned ಪೂರ್ಣಾಙ್ಕ quiet: 1;
	unsigned ಪೂರ್ಣಾಙ್ಕ progress: 1;
	unsigned ಪೂರ್ಣಾಙ್ಕ dissociate: 1;
};
#define ADD_DATA_INIT { \
	.depth = -1, \
	.ref_storage_format = REF_STORAGE_FORMAT_UNKNOWN, \
}

ಸ್ಥಿರ ಶೂನ್ಯ append_fetch_remotes(ಸಂರಚನಾ strbuf *ಸಂದೇಶ, ಧ್ರುವ ವರ್ಣ *git_dir_path)
{
	ಸಂರಚನಾ child_process cp_remote = CHILD_PROCESS_INIT;
	ಸಂರಚನಾ strbuf sb_remote_out = STRBUF_INIT;

	cp_remote.git_cmd = 1;
	strvec_pushf(&cp_remote.env,
		     "GIT_DIR=%s", git_dir_path);
	strvec_ತಳ್ಳು(&cp_remote.env, "GIT_WORK_TREE=.");
	strvec_pushl(&cp_remote.args, "remote", "-v", ರಿಕ್ತ);
	ಯದಿ (!capture_command(&cp_remote, &sb_remote_out, 0)) {
		ವರ್ಣ *next_ಸಾಲು;
		ವರ್ಣ *ಸಾಲು = sb_remote_out.buf;

		ಯಾವತ್ ((next_ಸಾಲು = strchr(ಸಾಲು, '\n')) != ರಿಕ್ತ) {
			size_t len = next_ಸಾಲು - ಸಾಲು;

			ಯದಿ (strip_suffix_mem(ಸಾಲು, &len, " (fetch)"))
				strbuf_addf(ಸಂದೇಶ, "  %.*s\n", (ಪೂರ್ಣಾಙ್ಕ)len, ಸಾಲು);
			ಸಾಲು = next_ಸಾಲು + 1;
		}
	}

	strbuf_release(&sb_remote_out);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ add_submodule(ಧ್ರುವ ಸಂರಚನಾ add_data *add_data)
{
	ಸಂರಚನಾ module_clone_data clone_data = MODULE_CLONE_DATA_INIT;
	ಸಂರಚನಾ string_list reference = STRING_LIST_INIT_NODUP;
	ಪೂರ್ಣಾಙ್ಕ ret = -1;

	/* perhaps the path already exists and is already a git repo, else clone it */
	ಯದಿ (is_directory(add_data->sm_path)) {
		ವರ್ಣ *submod_gitdir_path;
		ಸಂರಚನಾ strbuf sm_path = STRBUF_INIT;
		strbuf_addstr(&sm_path, add_data->sm_path);
		submod_gitdir_path = xstrfmt("%s/.git", add_data->sm_path);
		ಯದಿ (is_nonbare_repository_dir(&sm_path))
			printf(_("Adding existing repo at '%s' to the index\n"),
			       add_data->sm_path);
		ಅನ್ಯಥಾ
			die(_("'%s' already exists and is not a valid git repo"),
			    add_data->sm_path);
		strbuf_release(&sm_path);
		ಮುಕ್ತ(submod_gitdir_path);
	} ಅನ್ಯಥಾ {
		ಸಂರಚನಾ child_process cp = CHILD_PROCESS_INIT;
		ಸಂರಚನಾ strbuf submod_gitdir = STRBUF_INIT;

		submodule_name_to_gitdir(&submod_gitdir, the_repository, add_data->sm_name);

		ಯದಿ (is_directory(submod_gitdir.buf)) {
			ಯದಿ (!add_data->force) {
				ಸಂರಚನಾ strbuf ಸಂದೇಶ = STRBUF_INIT;
				ವರ್ಣ *die_ಸಂದೇಶ;

				strbuf_addf(&ಸಂದೇಶ, _("A git directory for '%s' is found "
						    "locally with remote(s):\n"),
					    add_data->sm_name);

				append_fetch_remotes(&ಸಂದೇಶ, submod_gitdir.buf);
				strbuf_release(&submod_gitdir);

				strbuf_addf(&ಸಂದೇಶ, _("If you want to reuse this local git "
						    "directory instead of cloning again from\n"
						    "  %s\n"
						    "use the '--force' option. If the local git "
						    "directory is not the correct repo\n"
						    "or you are unsure what this means choose "
						    "another name with the '--name' option."),
					    add_data->realrepo);

				die_ಸಂದೇಶ = strbuf_detach(&ಸಂದೇಶ, ರಿಕ್ತ);
				die("%s", die_ಸಂದೇಶ);
			} ಅನ್ಯಥಾ {
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
		ಯದಿ (add_data->reference_path) {
			ವರ್ಣ *p = xstrdup(add_data->reference_path);

			string_list_append(&reference, p)->util = p;
		}
		clone_data.ref_storage_format = add_data->ref_storage_format;
		clone_data.dissociate = add_data->dissociate;
		ಯದಿ (add_data->depth >= 0)
			clone_data.depth = add_data->depth;

		ಯದಿ (clone_submodule(&clone_data, &reference))
			ಗಚ್ಛ cleanup;

		prepare_submodule_repo_env(&cp.env);
		cp.git_cmd = 1;
		cp.dir = add_data->sm_path;
		/*
		 * NOTE: we only get here if add_data->force is true, so
		 * passing --force to checkout is reasonable.
		 */
		strvec_pushl(&cp.args, "checkout", "-f", "-q", ರಿಕ್ತ);

		ಯದಿ (add_data->branch) {
			strvec_pushl(&cp.args, "-B", add_data->branch, ರಿಕ್ತ);
			strvec_pushf(&cp.args, "origin/%s", add_data->branch);
		}

		ಯದಿ (run_command(&cp))
			die(_("unable to checkout submodule '%s'"), add_data->sm_path);
	}
	ret = 0;

cleanup:
	string_list_clear(&reference, 1);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ config_submodule_in_gitmodules(ಧ್ರುವ ವರ್ಣ *name, ಧ್ರುವ ವರ್ಣ *var, ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ)
{
	ವರ್ಣ *ಕೀಲಿ;
	ಪೂರ್ಣಾಙ್ಕ ret;

	ಯದಿ (!is_writing_gitmodules_ok())
		die(_("please make sure that the .gitmodules file is in the working tree"));

	ಕೀಲಿ = xstrfmt("submodule.%s.%s", name, var);
	ret = config_set_in_gitmodules_file_gently(the_repository, ಕೀಲಿ, ಮೌಲ್ಯ);
	ಮುಕ್ತ(ಕೀಲಿ);

	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಶೂನ್ಯ configure_added_submodule(ಸಂರಚನಾ add_data *add_data)
{
	ವರ್ಣ *ಕೀಲಿ;
	ಸಂರಚನಾ child_process add_submod = CHILD_PROCESS_INIT;
	ಸಂರಚನಾ child_process add_gitmodules = CHILD_PROCESS_INIT;
	ಧ್ರುವ ಸಂರಚನಾ string_list *ಮೌಲ್ಯಗಳು;
	ಪೂರ್ಣಾಙ್ಕ matched = 0;

	ಕೀಲಿ = xstrfmt("submodule.%s.url", add_data->sm_name);
	repo_config_set_gently(the_repository, ಕೀಲಿ, add_data->realrepo);
	ಮುಕ್ತ(ಕೀಲಿ);

	add_submod.git_cmd = 1;
	strvec_pushl(&add_submod.args, "add",
		     "--no-warn-embedded-repo", ರಿಕ್ತ);
	ಯದಿ (add_data->force)
		strvec_ತಳ್ಳು(&add_submod.args, "--force");
	strvec_pushl(&add_submod.args, "--", add_data->sm_path, ರಿಕ್ತ);

	ಯದಿ (run_command(&add_submod))
		die(_("Failed to add submodule '%s'"), add_data->sm_path);

	ಯದಿ (config_submodule_in_gitmodules(add_data->sm_name, "path", add_data->sm_path) ||
	    config_submodule_in_gitmodules(add_data->sm_name, "url", add_data->repo))
		die(_("Failed to register submodule '%s'"), add_data->sm_path);

	ಯದಿ (add_data->branch) {
		ಯದಿ (config_submodule_in_gitmodules(add_data->sm_name,
						   "branch", add_data->branch))
			die(_("Failed to register submodule '%s'"), add_data->sm_path);
	}

	add_gitmodules.git_cmd = 1;
	strvec_pushl(&add_gitmodules.args,
		     "add", "--force", "--", ".gitmodules", ರಿಕ್ತ);

	ಯದಿ (run_command(&add_gitmodules))
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
	ಯದಿ (repo_config_get(the_repository, "submodule.active") || /* key absent */
	    repo_config_get_string_multi(the_repository, "submodule.active", &ಮೌಲ್ಯಗಳು)) {
		/*
		 * If the submodule being added isn't already covered by the
		 * current configured pathspec, set the submodule's active flag
		 */
		ಕೀಲಿ = xstrfmt("submodule.%s.active", add_data->sm_name);
		repo_config_set_gently(the_repository, ಕೀಲಿ, "true");
		ಮುಕ್ತ(ಕೀಲಿ);
	} ಅನ್ಯಥಾ {
		ಪ್ರತಿ (size_t i = 0; i < ಮೌಲ್ಯಗಳು->nr; i++) {
			ಧ್ರುವ ವರ್ಣ *pat = ಮೌಲ್ಯಗಳು->ವಸ್ತುಗಳು[i].string;
			ಯದಿ (!wildmatch(pat, add_data->sm_path, 0)) { /* match found */
				matched = 1;
				ಭಙ್ಗ;
			}
		}
		ಯದಿ (!matched) { /* no pattern matched -> force-enable */
			ಕೀಲಿ = xstrfmt("submodule.%s.active", add_data->sm_name);
			repo_config_set_gently(the_repository, ಕೀಲಿ, "true");
			ಮುಕ್ತ(ಕೀಲಿ);
		}
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ die_on_index_match(ಧ್ರುವ ವರ್ಣ *path, ಪೂರ್ಣಾಙ್ಕ force)
{
	ಸಂರಚನಾ pathspec ps;
	ಧ್ರುವ ವರ್ಣ *args[] = { path, ರಿಕ್ತ };
	parse_pathspec(&ps, 0, PATHSPEC_PREFER_CWD, ರಿಕ್ತ, args);

	ಯದಿ (repo_read_index_preload(the_repository, ರಿಕ್ತ, 0) < 0)
		die(_("index file corrupt"));

	ಯದಿ (ps.nr) {
		ವರ್ಣ *ps_matched = xcalloc(ps.nr, 1);

		/* TODO: audit for interaction with sparse-index. */
		ensure_full_index(the_repository->index);

		/*
		 * Since there is only one pathspec, we just need to
		 * check ps_matched[0] to know if a cache entry matched.
		 */
		ಪ್ರತಿ (size_t i = 0; i < the_repository->index->cache_nr; i++) {
			ce_path_match(the_repository->index, the_repository->index->cache[i], &ps,
				      ps_matched);

			ಯದಿ (ps_matched[0]) {
				ಯದಿ (!force)
					die(_("'%s' already exists in the index"),
					    path);
				ಯದಿ (!S_ISGITLINK(the_repository->index->cache[i]->ce_mode))
					die(_("'%s' already exists in the index "
					      "and is not a submodule"), path);
				ಭಙ್ಗ;
			}
		}
		ಮುಕ್ತ(ps_matched);
	}
	clear_pathspec(&ps);
}

ಸ್ಥಿರ ಶೂನ್ಯ die_on_repo_without_commits(ಧ್ರುವ ವರ್ಣ *path)
{
	ಸಂರಚನಾ strbuf sb = STRBUF_INIT;
	strbuf_addstr(&sb, path);
	ಯದಿ (is_nonbare_repository_dir(&sb)) {
		ಸಂರಚನಾ object_id oid;
		ಯದಿ (repo_resolve_gitlink_ref(the_repository, path, "HEAD", &oid) < 0)
			die(_("'%s' does not have a commit checked out"), path);
	}
	strbuf_release(&sb);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ module_add(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
		      ಸಂರಚನಾ repository *repo UNUSED)
{
	ಪೂರ್ಣಾಙ್ಕ force = 0, quiet = 0, progress = 0, dissociate = 0;
	ಸಂರಚನಾ add_data add_data = ADD_DATA_INIT;
	ಧ್ರುವ ವರ್ಣ *ref_storage_format = ರಿಕ್ತ;
	ವರ್ಣ *to_free = ರಿಕ್ತ;
	ಧ್ರುವ ಸಂರಚನಾ submodule *existing;
	ಸಂರಚನಾ strbuf buf = STRBUF_INIT;
	ವರ್ಣ *sm_name_to_free = ರಿಕ್ತ;
	ಸಂರಚನಾ option options[] = {
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
	ಧ್ರುವ ವರ್ಣ *ಧ್ರುವ usage[] = {
		N_("git submodule add [<options>] [--] <repository> [<path>]"),
		ರಿಕ್ತ
	};
	ಸಂರಚನಾ strbuf sb = STRBUF_INIT;
	ಪೂರ್ಣಾಙ್ಕ ret = 1;

	argc = parse_options(argc, argv, prefix, options, usage, 0);

	ಯದಿ (!is_writing_gitmodules_ok())
		die(_("please make sure that the .gitmodules file is in the working tree"));

	ಯದಿ (prefix && *prefix &&
	    add_data.reference_path && !is_absolute_path(add_data.reference_path))
		add_data.reference_path = xstrfmt("%s%s", prefix, add_data.reference_path);

	ಯದಿ (argc == 0 || argc > 2)
		usage_with_options(usage, options);

	ಯದಿ (ref_storage_format) {
		add_data.ref_storage_format = ref_storage_format_by_name(ref_storage_format);
		ಯದಿ (add_data.ref_storage_format == REF_STORAGE_FORMAT_UNKNOWN)
			die(_("unknown ref storage format '%s'"), ref_storage_format);
	}

	add_data.repo = argv[0];
	ಯದಿ (argc == 1)
		add_data.sm_path = git_url_basename(add_data.repo, 0, 0);
	ಅನ್ಯಥಾ
		add_data.sm_path = xstrdup(argv[1]);

	ಯದಿ (prefix && *prefix && !is_absolute_path(add_data.sm_path)) {
		ವರ್ಣ *sm_path = add_data.sm_path;

		add_data.sm_path = xstrfmt("%s%s", prefix, sm_path);
		ಮುಕ್ತ(sm_path);
	}

	ಯದಿ (starts_with_dot_dot_slash(add_data.repo) ||
	    starts_with_dot_slash(add_data.repo)) {
		ಯದಿ (prefix)
			die(_("Relative path can only be used from the toplevel "
			      "of the working tree"));

		/* dereference source url relative to parent's url */
		to_free = resolve_relative_url(add_data.repo, ರಿಕ್ತ, 1);
		add_data.realrepo = to_free;
	} ಅನ್ಯಥಾ ಯದಿ (is_dir_sep(add_data.repo[0]) || strchr(add_data.repo, ':')) {
		add_data.realrepo = add_data.repo;
	} ಅನ್ಯಥಾ {
		die(_("repo URL: '%s' must be absolute or begin with ./|../"),
		    add_data.repo);
	}

	/*
	 * normalize path:
	 * multiple //; leading ./; /./; /../;
	 */
	normalize_path_copy(add_data.sm_path, add_data.sm_path);
	strip_dir_trailing_slashes(add_data.sm_path);

	ಯದಿ (validate_submodule_path(add_data.sm_path) < 0)
		die(ರಿಕ್ತ);

	die_on_index_match(add_data.sm_path, force);
	die_on_repo_without_commits(add_data.sm_path);

	ಯದಿ (!force) {
		ಸಂರಚನಾ child_process cp = CHILD_PROCESS_INIT;

		cp.git_cmd = 1;
		cp.no_stdout = 1;
		strvec_pushl(&cp.args, "add", "--dry-run", "--ignore-missing",
			     "--no-warn-embedded-repo", add_data.sm_path, ರಿಕ್ತ);
		ಯದಿ ((ret = pipe_command(&cp, ರಿಕ್ತ, 0, ರಿಕ್ತ, 0, &sb, 0))) {
			strbuf_complete_ಸಾಲು(&sb);
			fputs(sb.buf, stderr);
			ಗಚ್ಛ cleanup;
		}
	}

	ಯದಿ (!add_data.sm_name)
		add_data.sm_name = add_data.sm_path;

	existing = submodule_from_name(the_repository,
					null_oid(the_hash_algo),
					add_data.sm_name);

	ಯದಿ (existing && existing->path &&
	    strcmp(existing->path, add_data.sm_path)) {
		ಯದಿ (!force) {
			die(_("submodule name '%s' already used for path '%s'"),
			    add_data.sm_name, existing->path);
		}
		/* --force: build <name><n> until unique */
		ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 1; ; i++) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "%s%d", add_data.sm_name, i);
			ಯದಿ (!submodule_from_name(the_repository,
						null_oid(the_hash_algo),
						buf.buf)) {
				ಭಙ್ಗ;
			}
		}
		add_data.sm_name = sm_name_to_free = strbuf_detach(&buf, ರಿಕ್ತ);
	}

	ಯದಿ (check_submodule_name(add_data.sm_name))
		die(_("'%s' is not a valid submodule name"), add_data.sm_name);

	add_data.prefix = prefix;
	add_data.force = !!force;
	add_data.quiet = !!quiet;
	add_data.progress = !!progress;
	add_data.dissociate = !!dissociate;

	ಯದಿ (the_repository->repository_format_submodule_path_cfg)
		create_default_gitdir_config(add_data.sm_name);

	ಯದಿ (add_submodule(&add_data))
		ಗಚ್ಛ cleanup;
	configure_added_submodule(&add_data);

	ret = 0;
cleanup:
	ಮುಕ್ತ(sm_name_to_free);
	ಮುಕ್ತ(add_data.sm_path);
	ಮುಕ್ತ(to_free);
	strbuf_release(&sb);

	ಪ್ರತ್ಯಾಗಮ ret;
}

ಪೂರ್ಣಾಙ್ಕ cmd_submodule__helper(ಪೂರ್ಣಾಙ್ಕ argc,
			  ಧ್ರುವ ವರ್ಣ **argv,
			  ಧ್ರುವ ವರ್ಣ *prefix,
			  ಸಂರಚನಾ repository *repo)
{
	parse_opt_subcommand_fn *fn = ರಿಕ್ತ;
	ಧ್ರುವ ವರ್ಣ *ಧ್ರುವ usage[] = {
		N_("git submodule--helper <command>"),
		ರಿಕ್ತ
	};
	ಸಂರಚನಾ option options[] = {
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
		OPT_SUBCOMMAND("push-check", &fn, ತಳ್ಳು_check),
		OPT_SUBCOMMAND("absorbgitdirs", &fn, absorb_git_dirs),
		OPT_SUBCOMMAND("set-url", &fn, module_set_url),
		OPT_SUBCOMMAND("set-branch", &fn, module_set_branch),
		OPT_SUBCOMMAND("create-branch", &fn, module_create_branch),
		OPT_SUBCOMMAND("get-default-remote", &fn, module_get_default_remote),
		OPT_END()
	};
	argc = parse_options(argc, argv, prefix, options, usage, 0);

	ಪ್ರತ್ಯಾಗಮ fn(argc, argv, prefix, repo);
}
