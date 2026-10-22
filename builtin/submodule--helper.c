#define USE_THE_REPOSITORY_VARIABLE

#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "abspath.h"
#ଅନ୍ତର୍ଭାଵ "environment.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "hex.h"

#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"
#ଅନ୍ତର୍ଭାଵ "quote.h"
#ଅନ୍ତର୍ଭାଵ "path.h"
#ଅନ୍ତର୍ଭାଵ "pathspec.h"
#ଅନ୍ତର୍ଭାଵ "preload-index.h"
#ଅନ୍ତର୍ଭାଵ "dir.h"
#ଅନ୍ତର୍ଭାଵ "read-cache.h"
#ଅନ୍ତର୍ଭାଵ "setup.h"
#ଅନ୍ତର୍ଭାଵ "sparse-index.h"
#ଅନ୍ତର୍ଭାଵ "submodule.h"
#ଅନ୍ତର୍ଭାଵ "submodule-config.h"
#ଅନ୍ତର୍ଭାଵ "string-list.h"
#ଅନ୍ତର୍ଭାଵ "run-command.h"
#ଅନ୍ତର୍ଭାଵ "remote.h"
#ଅନ୍ତର୍ଭାଵ "refs.h"
#ଅନ୍ତର୍ଭାଵ "refspec.h"
#ଅନ୍ତର୍ଭାଵ "revision.h"
#ଅନ୍ତର୍ଭାଵ "diffcore.h"
#ଅନ୍ତର୍ଭାଵ "diff.h"
#ଅନ୍ତର୍ଭାଵ "object-file.h"
#ଅନ୍ତର୍ଭାଵ "object-name.h"
#ଅନ୍ତର୍ଭାଵ "odb.h"
#ଅନ୍ତର୍ଭାଵ "odb/source.h"
#ଅନ୍ତର୍ଭାଵ "odb/source-files.h"
#ଅନ୍ତର୍ଭାଵ "advice.h"
#ଅନ୍ତର୍ଭାଵ "branch.h"
#ଅନ୍ତର୍ଭାଵ "list-objects-filter-options.h"
#ଅନ୍ତର୍ଭାଵ "wildmatch.h"
#ଅନ୍ତର୍ଭାଵ "strbuf.h"
#ଅନ୍ତର୍ଭାଵ "url.h"

#define OPT_QUIET (1 << 0)
#define OPT_CACHED (1 << 1)
#define OPT_RECURSIVE (1 << 2)
#define OPT_FORCE (1 << 3)

ପ୍ରକାରବ୍ୟାଖ୍ୟା ଶୂନ୍ୟ (*each_submodule_fn)(ଅଚଳ ସଂରଚନା cache_entry *list_item,
				  ଶୂନ୍ୟ *cb_data);

ସ୍ଥିର ଅକ୍ଷର *get_default_remote(ଶୂନ୍ୟ)
{
	ଫେରନ୍ତୁ xstrdup(repo_default_remote(the_repository));
}

ସ୍ଥିର ଅକ୍ଷର *resolve_relative_url(ଅଚଳ ଅକ୍ଷର *rel_url, ଅଚଳ ଅକ୍ଷର *up_path, ପୂର୍ଣ୍ଣସଂଖ୍ୟା quiet)
{
	ଅକ୍ଷର *remoteurl, *resolved_url;
	ଅକ୍ଷର *remote = get_default_remote();
	ସଂରଚନା strbuf remotesb = STRBUF_INIT;

	strbuf_addf(&remotesb, "remote.%s.url", remote);
	ଯଦି (repo_config_get_string(the_repository, remotesb.buf, &remoteurl)) {
		ଯଦି (!quiet)
			warning(_("could not look up configuration '%s'. "
				  "Assuming this repository is its own "
				  "authoritative upstream."),
				remotesb.buf);
		remoteurl = xgetcwd();
	}
	resolved_url = relative_url(remoteurl, rel_url, up_path);

	ମୁକ୍ତ(remote);
	ମୁକ୍ତ(remoteurl);
	strbuf_release(&remotesb);

	ଫେରନ୍ତୁ resolved_url;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_default_remote_submodule(ଅଚଳ ଅକ୍ଷର *module_path, ଅକ୍ଷର **default_remote)
{
	ଅଚଳ ସଂରଚନା submodule *sub;
	ସଂରଚନା repository subrepo;
	ଅଚଳ ଅକ୍ଷର *remote_name = ଖାଲି;
	ଅକ୍ଷର *url = ଖାଲି;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), module_path);
	ଯଦି (sub && sub->url) {
		url = xstrdup(sub->url);

		/* Possibly a url relative to parent */
		ଯଦି (starts_with_dot_dot_slash(url) ||
		    starts_with_dot_slash(url)) {
			ଅକ୍ଷର *oldurl = url;

			url = resolve_relative_url(oldurl, ଖାଲି, 1);
			ମୁକ୍ତ(oldurl);
		}
	}

	ଯଦି (repo_submodule_init(&subrepo, the_repository, module_path,
				null_oid(the_hash_algo)) < 0) {
		ret = die_message(_("could not get a repository handle for submodule '%s'"),
				   module_path);
		ଯାଅ out;
	}

	/* Look up by URL first */
	ଯଦି (url)
		remote_name = repo_remote_from_url(&subrepo, url);
	ଯଦି (!remote_name)
		remote_name = repo_default_remote(&subrepo);

	*default_remote = xstrdup(remote_name);

out:
	repo_clear(&subrepo);
	ମୁକ୍ତ(url);

	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା module_get_default_remote(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
				     ସଂରଚନା repository *repo UNUSED)
{
	ଅଚଳ ଅକ୍ଷର *path;
	ଅକ୍ଷର *resolved_path = ଖାଲି;
	ଅକ୍ଷର *default_remote = ଖାଲି;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା code;
	ସଂରଚନା option options[] = {
		OPT_END()
	};
	ଅଚଳ ଅକ୍ଷର *ଅଚଳ usage[] = {
		N_("git submodule--helper get-default-remote <path>"),
		ଖାଲି
	};

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	ଯଦି (argc != 1)
		usage_with_options(usage, options);

	path = argv[0];
	ଯଦି (prefix && *prefix && !is_absolute_path(path)) {
		resolved_path = xstrfmt("%s%s", prefix, path);
		path = resolved_path;
	}

	code = get_default_remote_submodule(path, &default_remote);
	ଯଦି (code) {
		ମୁକ୍ତ(resolved_path);
		ଫେରନ୍ତୁ code;
	}

	printf("%s\n", default_remote);
	ମୁକ୍ତ(default_remote);
	ମୁକ୍ତ(resolved_path);
	ଫେରନ୍ତୁ 0;
}

/* the result should be freed by the caller. */
ସ୍ଥିର ଅକ୍ଷର *get_submodule_displaypath(ଅଚଳ ଅକ୍ଷର *path, ଅଚଳ ଅକ୍ଷର *prefix,
				       ଅଚଳ ଅକ୍ଷର *super_prefix)
{
	ଯଦି (prefix && super_prefix) {
		BUG("cannot have prefix '%s' and superprefix '%s'",
		    prefix, super_prefix);
	} ଅନ୍ୟଥା ଯଦି (prefix) {
		ସଂରଚନା strbuf sb = STRBUF_INIT;
		ଅକ୍ଷର *displaypath = xstrdup(relative_path(path, prefix, &sb));
		strbuf_release(&sb);
		ଫେରନ୍ତୁ displaypath;
	} ଅନ୍ୟଥା ଯଦି (super_prefix) {
		ଫେରନ୍ତୁ xstrfmt("%s%s", super_prefix, path);
	} ଅନ୍ୟଥା {
		ଫେରନ୍ତୁ xstrdup(path);
	}
}

ସ୍ଥିର ଅକ୍ଷର *compute_rev_name(ଅଚଳ ଅକ୍ଷର *sub_path, ଅଚଳ ଅକ୍ଷର* object_id)
{
	ସଂରଚନା strbuf sb = STRBUF_INIT;
	ଅଚଳ ଅକ୍ଷର ***d;

	ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *describe_bare[] = { ଖାଲି };

	ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *describe_tags[] = { "--tags", ଖାଲି };

	ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *describe_contains[] = { "--contains", ଖାଲି };

	ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *describe_all_always[] = { "--all", "--always", ଖାଲି };

	ସ୍ଥିର ଅଚଳ ଅକ୍ଷର **describe_argv[] = { describe_bare, describe_tags,
						describe_contains,
						describe_all_always, ଖାଲି };

	ପାଇଁ (d = describe_argv; *d; d++) {
		ସଂରଚନା child_process cp = CHILD_PROCESS_INIT;
		prepare_submodule_repo_env(&cp.env);
		cp.dir = sub_path;
		cp.git_cmd = 1;
		cp.no_stderr = 1;

		strvec_ଠେଲ(&cp.args, "describe");
		strvec_pushv(&cp.args, *d);
		strvec_ଠେଲ(&cp.args, object_id);

		ଯଦି (!capture_command(&cp, &sb, 0)) {
			strbuf_strip_suffix(&sb, "\n");
			ଫେରନ୍ତୁ strbuf_detach(&sb, ଖାଲି);
		}
	}

	strbuf_release(&sb);
	ଫେରନ୍ତୁ ଖାଲି;
}

ସଂରଚନା module_list {
	ଅଚଳ ସଂରଚନା cache_entry **entries;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା alloc, nr;
};
#define MODULE_LIST_INIT { 0 }

ସ୍ଥିର ଶୂନ୍ୟ module_list_release(ସଂରଚନା module_list *ml)
{
	ମୁକ୍ତ(ml->entries);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା module_list_compute(ଅଚଳ ଅକ୍ଷର **argv,
			       ଅଚଳ ଅକ୍ଷର *prefix,
			       ସଂରଚନା pathspec *pathspec,
			       ସଂରଚନା module_list *list)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା result = 0;
	ଅକ୍ଷର *ps_matched = ଖାଲି;

	parse_pathspec(pathspec, 0,
		       PATHSPEC_PREFER_FULL,
		       prefix, argv);

	ଯଦି (pathspec->nr)
		ps_matched = xcalloc(pathspec->nr, 1);

	ଯଦି (repo_read_index(the_repository) < 0)
		die(_("index file corrupt"));

	ପାଇଁ (size_t i = 0; i < the_repository->index->cache_nr; i++) {
		ଅଚଳ ସଂରଚନା cache_entry *ce = the_repository->index->cache[i];

		ଯଦି (!match_pathspec(the_repository->index, pathspec, ce->name, ce_namelen(ce),
				    0, ps_matched, 1) ||
		    !S_ISGITLINK(ce->ce_mode))
			ଜାରି;

		ALLOC_GROW(list->entries, list->nr + 1, list->alloc);
		list->entries[list->nr++] = ce;
		ଯେତେବେଳେ (i + 1 < the_repository->index->cache_nr &&
		       !strcmp(ce->name, the_repository->index->cache[i + 1]->name))
			/*
			 * Skip entries with the same name in different stages
			 * to make sure an entry is returned only once.
			 */
			i++;
	}

	ଯଦି (ps_matched && report_path_error(ps_matched, pathspec))
		result = -1;

	ମୁକ୍ତ(ps_matched);

	ଫେରନ୍ତୁ result;
}

ସ୍ଥିର ଶୂନ୍ୟ module_list_active(ସଂରଚନା module_list *list)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
	ସଂରଚନା module_list active_modules = MODULE_LIST_INIT;

	ପାଇଁ (i = 0; i < list->nr; i++) {
		ଅଚଳ ସଂରଚନା cache_entry *ce = list->entries[i];

		ଯଦି (!is_submodule_active(the_repository, ce->name))
			ଜାରି;

		ALLOC_GROW(active_modules.entries,
			   active_modules.nr + 1,
			   active_modules.alloc);
		active_modules.entries[active_modules.nr++] = ce;
	}

	module_list_release(list);
	*list = active_modules;
}

ସ୍ଥିର ଅକ୍ଷର *get_up_path(ଅଚଳ ଅକ୍ଷର *path)
{
	ସଂରଚନା strbuf sb = STRBUF_INIT;

	strbuf_addstrings(&sb, "../", ଗଣନା_slashes(path));

	/*
	 * Check if 'path' ends with slash or not
	 * for having the same output for dir/sub_dir
	 * and dir/sub_dir/
	 */
	ଯଦି (!is_dir_sep(path[strlen(path) - 1]))
		strbuf_addstr(&sb, "../");

	ଫେରନ୍ତୁ strbuf_detach(&sb, ଖାଲି);
}

ସ୍ଥିର ଶୂନ୍ୟ for_each_listed_submodule(ଅଚଳ ସଂରଚନା module_list *list,
				      each_submodule_fn fn, ଶୂନ୍ୟ *cb_data)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

	ପାଇଁ (i = 0; i < list->nr; i++)
		fn(list->entries[i], cb_data);
}

ସଂରଚନା foreach_cb {
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc;
	ଅଚଳ ଅକ୍ଷର **argv;
	ଅଚଳ ଅକ୍ଷର *prefix;
	ଅଚଳ ଅକ୍ଷର *super_prefix;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା quiet;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା recursive;
};
#define FOREACH_CB_INIT { 0 }

ସ୍ଥିର ଶୂନ୍ୟ runcommand_in_submodule_cb(ଅଚଳ ସଂରଚନା cache_entry *list_item,
				       ଶୂନ୍ୟ *cb_data)
{
	ସଂରଚନା foreach_cb *info = cb_data;
	ଅଚଳ ଅକ୍ଷର *path = list_item->name;
	ଅଚଳ ସଂରଚନା object_id *ce_oid = &list_item->oid;
	ଅଚଳ ସଂରଚନା submodule *sub;
	ସଂରଚନା child_process cp = CHILD_PROCESS_INIT;
	ଅକ୍ଷର *displaypath;

	ଯଦି (validate_submodule_path(path) < 0)
		die(ଖାଲି);

	displaypath = get_submodule_displaypath(path, info->prefix,
						info->super_prefix);

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);

	ଯଦି (!sub)
		die(_("No url found for submodule path '%s' in .gitmodules"),
			displaypath);

	ଯଦି (!is_submodule_populated_gently(path, ଖାଲି))
		ଯାଅ cleanup;

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
	ଯଦି (info->argc == 1) {
		ଅକ୍ଷର *toplevel = xgetcwd();
		ସଂରଚନା strbuf sb = STRBUF_INIT;

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
		ମୁକ୍ତ(toplevel);
	} ଅନ୍ୟଥା {
		strvec_pushv(&cp.args, info->argv);
	}

	ଯଦି (!info->quiet)
		printf(_("Entering '%s'\n"), displaypath);

	ଯଦି (info->argv[0]) {
		ଯଦି (run_command(&cp))
			die(_("run_command returned non-zero status for %s\n."),
			    displaypath);
	} ଅନ୍ୟଥା {
		child_process_clear(&cp);
	}

	ଯଦି (info->recursive) {
		ସଂରଚନା child_process cpr = CHILD_PROCESS_INIT;

		cpr.git_cmd = 1;
		cpr.dir = path;
		prepare_submodule_repo_env(&cpr.env);

		strvec_pushl(&cpr.args, "submodule--helper", "foreach", "--recursive",
			     ଖାଲି);
		strvec_pushf(&cpr.args, "--super-prefix=%s/", displaypath);

		ଯଦି (info->quiet)
			strvec_ଠେଲ(&cpr.args, "--quiet");

		strvec_ଠେଲ(&cpr.args, "--");
		strvec_pushv(&cpr.args, info->argv);

		ଯଦି (run_command(&cpr))
			die(_("run_command returned non-zero status while "
				"recursing in the nested submodules of %s\n."),
				displaypath);
	}

cleanup:
	ମୁକ୍ତ(displaypath);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା module_foreach(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
			  ସଂରଚନା repository *repo UNUSED)
{
	ସଂରଚନା foreach_cb info = FOREACH_CB_INIT;
	ସଂରଚନା pathspec pathspec = { 0 };
	ସଂରଚନା module_list list = MODULE_LIST_INIT;
	ସଂରଚନା option module_foreach_options[] = {
		OPT__SUPER_PREFIX(&info.super_prefix),
		OPT__QUIET(&info.quiet, N_("suppress output of entering each submodule command")),
		OPT_BOOL(0, "recursive", &info.recursive,
			 N_("recurse into nested submodules")),
		OPT_END()
	};
	ଅଚଳ ଅକ୍ଷର *ଅଚଳ git_submodule_helper_usage[] = {
		N_("git submodule foreach [--quiet] [--recursive] [--] <command>"),
		ଖାଲି
	};
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 1;

	argc = parse_options(argc, argv, prefix, module_foreach_options,
			     git_submodule_helper_usage, 0);

	ଯଦି (module_list_compute(ଖାଲି, prefix, &pathspec, &list) < 0)
		ଯାଅ cleanup;

	info.argc = argc;
	info.argv = argv;
	info.prefix = prefix;

	for_each_listed_submodule(&list, runcommand_in_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	ଫେରନ୍ତୁ ret;
}

ସଂରଚନା init_cb {
	ଅଚଳ ଅକ୍ଷର *prefix;
	ଅଚଳ ଅକ୍ଷର *super_prefix;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags;
};
#define INIT_CB_INIT { 0 }

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା validate_and_set_submodule_gitdir(ସଂରଚନା strbuf *gitdir_path,
					     ଅଚଳ ଅକ୍ଷର *submodule_name)
{
	ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ;
	ଅକ୍ଷର *ଚାବି;

	ଯଦି (validate_submodule_git_dir(gitdir_path->buf, submodule_name))
		ଫେରନ୍ତୁ -1;

	ଚାବି = xstrfmt("submodule.%s.gitdir", submodule_name);

	/* Nothing to do if the config already exists. */
	ଯଦି (!repo_config_get_string_tmp(the_repository, ଚାବି, &ମୂଲ୍ୟ)) {
		ମୁକ୍ତ(ଚାବି);
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (repo_config_set_gently(the_repository, ଚାବି, gitdir_path->buf)) {
		ମୁକ୍ତ(ଚାବି);
		ଫେରନ୍ତୁ -1;
	}

	ମୁକ୍ତ(ଚାବି);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ create_default_gitdir_config(ଅଚଳ ଅକ୍ଷର *submodule_name)
{
	ସଂରଚନା strbuf gitdir_path = STRBUF_INIT;
	ସଂରଚନା git_hash_ctx ctx;
	ଅକ୍ଷର hex_name_hash[GIT_MAX_HEXSZ + 1], header[128];
	unsigned ଅକ୍ଷର raw_name_hash[GIT_MAX_RAWSZ];
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା header_len;

	/* Case 1: try the plain module name */
	repo_git_path_append(the_repository, &gitdir_path, "modules/%s", submodule_name);
	ଯଦି (!validate_and_set_submodule_gitdir(&gitdir_path, submodule_name)) {
		strbuf_release(&gitdir_path);
		ଫେରନ୍ତୁ;
	}

	/* Case 2.1: Try URI-safe (RFC3986) encoding first, this fixes nested gitdirs */
	strbuf_reset(&gitdir_path);
	repo_git_path_append(the_repository, &gitdir_path, "modules/");
	strbuf_addstr_urlencode(&gitdir_path, submodule_name, is_rfc3986_unreserved);
	ଯଦି (!validate_and_set_submodule_gitdir(&gitdir_path, submodule_name)) {
		strbuf_release(&gitdir_path);
		ଫେରନ୍ତୁ;
	}

	/* Case 2.2: Try extended uppercase URI (RFC3986) encoding, to fix case-folding */
	strbuf_reset(&gitdir_path);
	repo_git_path_append(the_repository, &gitdir_path, "modules/");
	strbuf_addstr_urlencode(&gitdir_path, submodule_name, is_casefolding_rfc3986_unreserved);
	ଯଦି (!validate_and_set_submodule_gitdir(&gitdir_path, submodule_name))
		ଫେରନ୍ତୁ;

	/* Case 2.3: Try some derived gitdir names, see if one sticks */
	ପାଇଁ (ଅକ୍ଷର c = '0'; c <= '9'; c++) {
		strbuf_reset(&gitdir_path);
		repo_git_path_append(the_repository, &gitdir_path, "modules/");
		strbuf_addstr_urlencode(&gitdir_path, submodule_name, is_rfc3986_unreserved);
		strbuf_addch(&gitdir_path, c);
		ଯଦି (!validate_and_set_submodule_gitdir(&gitdir_path, submodule_name))
			ଫେରନ୍ତୁ;

		strbuf_reset(&gitdir_path);
		repo_git_path_append(the_repository, &gitdir_path, "modules/");
		strbuf_addstr_urlencode(&gitdir_path, submodule_name, is_casefolding_rfc3986_unreserved);
		strbuf_addch(&gitdir_path, c);
		ଯଦି (!validate_and_set_submodule_gitdir(&gitdir_path, submodule_name))
			ଫେରନ୍ତୁ;
	}

	/* Case 2.4: If all the above failed, try a hash of the name as a last resort */
	header_len = snprintf(header, ଆକାର(header),
			      "blob %"PRIuMAX, (uintmax_t)strlen(submodule_name));
	git_hash_init(&ctx, the_hash_algo);
	git_hash_update(&ctx, header, header_len);
	git_hash_update(&ctx, "\0", 1);
	git_hash_update(&ctx, submodule_name, strlen(submodule_name));
	git_hash_final(raw_name_hash, &ctx);
	hash_to_hex_algop_r(hex_name_hash, raw_name_hash, the_hash_algo);
	strbuf_reset(&gitdir_path);
	repo_git_path_append(the_repository, &gitdir_path, "modules/%s", hex_name_hash);
	ଯଦି (!validate_and_set_submodule_gitdir(&gitdir_path, submodule_name)) {
		strbuf_release(&gitdir_path);
		ଫେରନ୍ତୁ;
	}

	/* Case 3: nothing worked, error out */
	die(_("failed to set a valid default config for 'submodule.%s.gitdir'. "
	      "Please ensure it is set, for example by running something like: "
	      "'git config submodule.%s.gitdir .git/modules/%s'"),
	    submodule_name, submodule_name, submodule_name);
}

ସ୍ଥିର ଶୂନ୍ୟ init_submodule(ଅଚଳ ଅକ୍ଷର *path, ଅଚଳ ଅକ୍ଷର *prefix,
			   ଅଚଳ ଅକ୍ଷର *super_prefix,
			   unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
	ଅଚଳ ସଂରଚନା submodule *sub;
	ସଂରଚନା strbuf sb = STRBUF_INIT;
	ଅଚଳ ଅକ୍ଷର *upd;
	ଅକ୍ଷର *url = ଖାଲି, *displaypath;

	displaypath = get_submodule_displaypath(path, prefix, super_prefix);

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);

	ଯଦି (!sub)
		die(_("No url found for submodule path '%s' in .gitmodules"),
			displaypath);

	/*
	 * NEEDSWORK: In a multi-working-tree world, this needs to be
	 * set in the per-worktree config.
	 *
	 * Set active flag for the submodule being initialized
	 */
	ଯଦି (!is_submodule_active(the_repository, path)) {
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
	ଯଦି (repo_config_get_string(the_repository, sb.buf, &url)) {
		ଯଦି (!sub->url)
			die(_("No url found for submodule path '%s' in .gitmodules"),
				displaypath);

		url = xstrdup(sub->url);

		/* Possibly a url relative to parent */
		ଯଦି (starts_with_dot_dot_slash(url) ||
		    starts_with_dot_slash(url)) {
			ଅକ୍ଷର *oldurl = url;

			url = resolve_relative_url(oldurl, ଖାଲି, 0);
			ମୁକ୍ତ(oldurl);
		}

		ଯଦି (repo_config_set_gently(the_repository, sb.buf, url))
			die(_("Failed to register url for submodule path '%s'"),
			    displaypath);
		ଯଦି (!(flags & OPT_QUIET))
			fprintf(stderr,
				_("Submodule '%s' (%s) registered for path '%s'\n"),
				sub->name, url, displaypath);
	}
	strbuf_reset(&sb);

	/* Copy "update" setting when it is not set yet */
	strbuf_addf(&sb, "submodule.%s.update", sub->name);
	ଯଦି (repo_config_get_string_tmp(the_repository, sb.buf, &upd) &&
	    sub->update_strategy.type != SM_UPDATE_UNSPECIFIED) {
		ଯଦି (sub->update_strategy.type == SM_UPDATE_COMMAND) {
			fprintf(stderr, _("warning: command update mode suggested for submodule '%s'\n"),
				sub->name);
			upd = "none";
		} ଅନ୍ୟଥା {
			upd = submodule_update_type_to_string(sub->update_strategy.type);
		}

		ଯଦି (repo_config_set_gently(the_repository, sb.buf, upd))
			die(_("Failed to register update mode for submodule path '%s'"), displaypath);
	}

	ଯଦି (the_repository->repository_format_submodule_path_cfg)
		create_default_gitdir_config(sub->name);

	strbuf_release(&sb);
	ମୁକ୍ତ(displaypath);
	ମୁକ୍ତ(url);
}

ସ୍ଥିର ଶୂନ୍ୟ init_submodule_cb(ଅଚଳ ସଂରଚନା cache_entry *list_item, ଶୂନ୍ୟ *cb_data)
{
	ସଂରଚନା init_cb *info = cb_data;

	init_submodule(list_item->name, info->prefix, info->super_prefix,
		       info->flags);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା module_init(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
		       ସଂରଚନା repository *repo UNUSED)
{
	ସଂରଚନା init_cb info = INIT_CB_INIT;
	ସଂରଚନା pathspec pathspec = { 0 };
	ସଂରଚନା module_list list = MODULE_LIST_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା quiet = 0;
	ସଂରଚନା option module_init_options[] = {
		OPT__QUIET(&quiet, N_("suppress output for initializing a submodule")),
		OPT_END()
	};
	ଅଚଳ ଅକ୍ଷର *ଅଚଳ git_submodule_helper_usage[] = {
		N_("git submodule init [<options>] [<path>]"),
		ଖାଲି
	};
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 1;

	argc = parse_options(argc, argv, prefix, module_init_options,
			     git_submodule_helper_usage, 0);

	ଯଦି (module_list_compute(argv, prefix, &pathspec, &list) < 0)
		ଯାଅ cleanup;

	/*
	 * If there are no path args and submodule.active is set then,
	 * by default, only initialize 'active' modules.
	 */
	ଯଦି (!argc && !repo_config_get(the_repository, "submodule.active"))
		module_list_active(&list);

	info.prefix = prefix;
	ଯଦି (quiet)
		info.flags |= OPT_QUIET;

	for_each_listed_submodule(&list, init_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	ଫେରନ୍ତୁ ret;
}

ସଂରଚନା status_cb {
	ଅଚଳ ଅକ୍ଷର *prefix;
	ଅଚଳ ଅକ୍ଷର *super_prefix;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags;
};
#define STATUS_CB_INIT { 0 }

ସ୍ଥିର ଶୂନ୍ୟ print_status(unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags, ଅକ୍ଷର state, ଅଚଳ ଅକ୍ଷର *path,
			 ଅଚଳ ସଂରଚନା object_id *oid, ଅଚଳ ଅକ୍ଷର *displaypath)
{
	ଯଦି (flags & OPT_QUIET)
		ଫେରନ୍ତୁ;

	printf("%c%s %s", state, oid_to_hex(oid), displaypath);

	ଯଦି (state == ' ' || state == '+') {
		ଅକ୍ଷର *name = compute_rev_name(path, oid_to_hex(oid));

		ଯଦି (name)
			printf(" (%s)", name);
		ମୁକ୍ତ(name);
	}

	printf("\n");
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା handle_submodule_head_ref(ଅଚଳ ସଂରଚନା reference *ref, ଶୂନ୍ୟ *cb_data)
{
	ସଂରଚନା object_id *output = cb_data;

	ଯଦି (ref->oid)
		oidcpy(output, ref->oid);

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ status_submodule(ଅଚଳ ଅକ୍ଷର *path, ଅଚଳ ସଂରଚନା object_id *ce_oid,
			     unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ce_flags, ଅଚଳ ଅକ୍ଷର *prefix,
			     ଅଚଳ ଅକ୍ଷର *super_prefix, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
	ଅକ୍ଷର *displaypath;
	ସଂରଚନା strvec diff_files_args = STRVEC_INIT;
	ସଂରଚନା rev_info rev = REV_INFO_INIT;
	ସଂରଚନା strbuf buf = STRBUF_INIT;
	ଅଚଳ ଅକ୍ଷର *git_dir;

	ଯଦି (validate_submodule_path(path) < 0)
		die(ଖାଲି);

	ଯଦି (!submodule_from_path(the_repository, null_oid(the_hash_algo), path))
		die(_("no submodule mapping found in .gitmodules for path '%s'"),
		      path);

	displaypath = get_submodule_displaypath(path, prefix, super_prefix);

	ଯଦି ((CE_STAGEMASK & ce_flags) >> CE_STAGESHIFT) {
		print_status(flags, 'U', path, null_oid(the_hash_algo), displaypath);
		ଯାଅ cleanup;
	}

	strbuf_addf(&buf, "%s/.git", path);
	git_dir = read_gitfile(buf.buf);
	ଯଦି (!git_dir)
		git_dir = buf.buf;

	ଯଦି (!is_submodule_active(the_repository, path) ||
	    !is_git_directory(git_dir)) {
		print_status(flags, '-', path, ce_oid, displaypath);
		strbuf_release(&buf);
		ଯାଅ cleanup;
	}
	strbuf_release(&buf);

	strvec_pushl(&diff_files_args, "diff-files",
		     "--ignore-submodules=dirty", "--quiet", "--",
		     path, ଖାଲି);

	repo_config(the_repository, git_diff_basic_config, ଖାଲି);

	repo_init_revisions(the_repository, &rev, ଖାଲି);
	rev.abbrev = 0;
	setup_revisions_from_strvec(&diff_files_args, &rev, ଖାଲି);
	run_diff_files(&rev, 0);

	ଯଦି (!diff_result_code(&rev)) {
		print_status(flags, ' ', path, ce_oid,
			     displaypath);
	} ଅନ୍ୟଥା ଯଦି (!(flags & OPT_CACHED)) {
		ସଂରଚନା object_id oid;
		ସଂରଚନା ref_store *refs = repo_get_submodule_ref_store(the_repository,
								      path);

		ଯଦି (!refs) {
			print_status(flags, '-', path, ce_oid, displaypath);
			ଯାଅ cleanup;
		}
		ଯଦି (refs_head_ref(refs, handle_submodule_head_ref, &oid))
			die(_("could not resolve HEAD ref inside the "
			      "submodule '%s'"), path);

		print_status(flags, '+', path, &oid, displaypath);
	} ଅନ୍ୟଥା {
		print_status(flags, '+', path, ce_oid, displaypath);
	}

	ଯଦି (flags & OPT_RECURSIVE) {
		ସଂରଚନା child_process cpr = CHILD_PROCESS_INIT;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା res;

		cpr.git_cmd = 1;
		cpr.dir = path;
		prepare_submodule_repo_env(&cpr.env);

		strvec_pushl(&cpr.args, "submodule--helper", "status",
			     "--recursive", ଖାଲି);
		strvec_pushf(&cpr.args, "--super-prefix=%s/", displaypath);

		ଯଦି (flags & OPT_CACHED)
			strvec_ଠେଲ(&cpr.args, "--cached");

		ଯଦି (flags & OPT_QUIET)
			strvec_ଠେଲ(&cpr.args, "--quiet");

		res = run_command(&cpr);
		ଯଦି (res == SIGPIPE + 128)
			ଉଠାଅ(SIGPIPE);
		ଅନ୍ୟଥା ଯଦି (res)
			die(_("failed to recurse into submodule '%s'"), path);
	}

cleanup:
	strvec_clear(&diff_files_args);
	ମୁକ୍ତ(displaypath);
	release_revisions(&rev);
}

ସ୍ଥିର ଶୂନ୍ୟ status_submodule_cb(ଅଚଳ ସଂରଚନା cache_entry *list_item,
				ଶୂନ୍ୟ *cb_data)
{
	ସଂରଚନା status_cb *info = cb_data;

	status_submodule(list_item->name, &list_item->oid, list_item->ce_flags,
			 info->prefix, info->super_prefix, info->flags);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା module_status(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
			 ସଂରଚନା repository *repo UNUSED)
{
	ସଂରଚନା status_cb info = STATUS_CB_INIT;
	ସଂରଚନା pathspec pathspec = { 0 };
	ସଂରଚନା module_list list = MODULE_LIST_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା quiet = 0;
	ସଂରଚନା option module_status_options[] = {
		OPT__SUPER_PREFIX(&info.super_prefix),
		OPT__QUIET(&quiet, N_("suppress submodule status output")),
		OPT_BIT(0, "cached", &info.flags, N_("use commit stored in the index instead of the one stored in the submodule HEAD"), OPT_CACHED),
		OPT_BIT(0, "recursive", &info.flags, N_("recurse into nested submodules"), OPT_RECURSIVE),
		OPT_END()
	};
	ଅଚଳ ଅକ୍ଷର *ଅଚଳ git_submodule_helper_usage[] = {
		N_("git submodule status [--quiet] [--cached] [--recursive] [<path>...]"),
		ଖାଲି
	};
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 1;

	argc = parse_options(argc, argv, prefix, module_status_options,
			     git_submodule_helper_usage, 0);

	ଯଦି (module_list_compute(argv, prefix, &pathspec, &list) < 0)
		ଯାଅ cleanup;

	info.prefix = prefix;
	ଯଦି (quiet)
		info.flags |= OPT_QUIET;

	for_each_listed_submodule(&list, status_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	ଫେରନ୍ତୁ ret;
}

ସଂରଚନା module_cb {
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା mod_src;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା mod_dst;
	ସଂରଚନା object_id oid_src;
	ସଂରଚନା object_id oid_dst;
	ଅକ୍ଷର status;
	ଅକ୍ଷର *sm_path;
};
#define MODULE_CB_INIT { 0 }

ସ୍ଥିର ଶୂନ୍ୟ module_cb_release(ସଂରଚନା module_cb *mcb)
{
	ମୁକ୍ତ(mcb->sm_path);
}

ସଂରଚନା module_cb_list {
	ସଂରଚନା module_cb **entries;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା alloc, nr;
};
#define MODULE_CB_LIST_INIT { 0 }

ସ୍ଥିର ଶୂନ୍ୟ module_cb_list_release(ସଂରଚନା module_cb_list *mcbl)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

	ପାଇଁ (i = 0; i < mcbl->nr; i++) {
		ସଂରଚନା module_cb *mcb = mcbl->entries[i];

		module_cb_release(mcb);
		ମୁକ୍ତ(mcb);
	}
	ମୁକ୍ତ(mcbl->entries);
}

ସଂରଚନା summary_cb {
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc;
	ଅଚଳ ଅକ୍ଷର **argv;
	ଅଚଳ ଅକ୍ଷର *prefix;
	ଅଚଳ ଅକ୍ଷର *super_prefix;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cached: 1;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା for_status: 1;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା files: 1;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା summary_limit;
};
#define SUMMARY_CB_INIT { 0 }

ଗଣନା diff_cmd {
	DIFF_INDEX,
	DIFF_FILES
};

ସ୍ଥିର ଅକ୍ଷର *verify_submodule_committish(ଅଚଳ ଅକ୍ଷର *sm_path,
					 ଅଚଳ ଅକ୍ଷର *committish)
{
	ସଂରଚନା child_process cp_rev_parse = CHILD_PROCESS_INIT;
	ସଂରଚନା strbuf result = STRBUF_INIT;

	cp_rev_parse.git_cmd = 1;
	cp_rev_parse.dir = sm_path;
	prepare_submodule_repo_env(&cp_rev_parse.env);
	strvec_pushl(&cp_rev_parse.args, "rev-parse", "-q", "--short", ଖାଲି);
	strvec_pushf(&cp_rev_parse.args, "%s^0", committish);
	strvec_ଠେଲ(&cp_rev_parse.args, "--");

	ଯଦି (capture_command(&cp_rev_parse, &result, 0))
		ଫେରନ୍ତୁ ଖାଲି;

	strbuf_trim_trailing_newline(&result);
	ଫେରନ୍ତୁ strbuf_detach(&result, ଖାଲି);
}

ସ୍ଥିର ଶୂନ୍ୟ print_submodule_summary(ସଂରଚନା summary_cb *info, ଅଚଳ ଅକ୍ଷର *errmsg,
				    ପୂର୍ଣ୍ଣସଂଖ୍ୟା total_commits, ଅଚଳ ଅକ୍ଷର *displaypath,
				    ଅଚଳ ଅକ୍ଷର *src_abbrev, ଅଚଳ ଅକ୍ଷର *dst_abbrev,
				    ସଂରଚନା module_cb *p)
{
	ଯଦି (p->status == 'T') {
		ଯଦି (S_ISGITLINK(p->mod_dst))
			printf(_("* %s %s(blob)->%s(submodule)"),
				 displaypath, src_abbrev, dst_abbrev);
		ଅନ୍ୟଥା
			printf(_("* %s %s(submodule)->%s(blob)"),
				 displaypath, src_abbrev, dst_abbrev);
	} ଅନ୍ୟଥା {
		printf("* %s %s...%s",
			displaypath, src_abbrev, dst_abbrev);
	}

	ଯଦି (total_commits < 0)
		printf(":\n");
	ଅନ୍ୟଥା
		printf(" (%d):\n", total_commits);

	ଯଦି (errmsg) {
		printf(_("%s"), errmsg);
	} ଅନ୍ୟଥା ଯଦି (total_commits > 0) {
		ସଂରଚନା child_process cp_log = CHILD_PROCESS_INIT;

		cp_log.git_cmd = 1;
		cp_log.dir = p->sm_path;
		prepare_submodule_repo_env(&cp_log.env);
		strvec_pushl(&cp_log.args, "log", ଖାଲି);

		ଯଦି (S_ISGITLINK(p->mod_src) && S_ISGITLINK(p->mod_dst)) {
			ଯଦି (info->summary_limit > 0)
				strvec_pushf(&cp_log.args, "-%d",
					     info->summary_limit);

			strvec_pushl(&cp_log.args, "--pretty=  %m %s",
				     "--first-parent", ଖାଲି);
			strvec_pushf(&cp_log.args, "%s...%s",
				     src_abbrev, dst_abbrev);
		} ଅନ୍ୟଥା ଯଦି (S_ISGITLINK(p->mod_dst)) {
			strvec_pushl(&cp_log.args, "--pretty=  > %s",
				     "-1", dst_abbrev, ଖାଲି);
		} ଅନ୍ୟଥା {
			strvec_pushl(&cp_log.args, "--pretty=  < %s",
				     "-1", src_abbrev, ଖାଲି);
		}
		run_command(&cp_log);
	}
	printf("\n");
}

ସ୍ଥିର ଶୂନ୍ୟ generate_submodule_summary(ସଂରଚନା summary_cb *info,
				       ସଂରଚନା module_cb *p)
{
	ଅକ୍ଷର *displaypath, *src_abbrev = ଖାଲି, *dst_abbrev;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା missing_src = 0, missing_dst = 0;
	ସଂରଚନା strbuf errmsg = STRBUF_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା total_commits = -1;

	ଯଦି (!info->cached && oideq(&p->oid_dst, null_oid(the_hash_algo))) {
		ଯଦି (S_ISGITLINK(p->mod_dst)) {
			ସଂରଚନା ref_store *refs = repo_get_submodule_ref_store(the_repository,
									      p->sm_path);

			ଯଦି (refs)
				refs_head_ref(refs, handle_submodule_head_ref, &p->oid_dst);
		} ଅନ୍ୟଥା ଯଦି (S_ISLNK(p->mod_dst) || S_ISREG(p->mod_dst)) {
			ସଂରଚନା stat st;
			ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = open(p->sm_path, O_RDONLY);

			ଯଦି (fd < 0 || fstat(fd, &st) < 0 ||
			    index_fd(the_repository->index, &p->oid_dst, fd, &st, OBJ_BLOB,
				     p->sm_path, 0))
				error(_("couldn't hash object from '%s'"), p->sm_path);
		} ଅନ୍ୟଥା {
			/* for a submodule removal (mode:0000000), don't warn */
			ଯଦି (p->mod_dst)
				warning(_("unexpected mode %o"), p->mod_dst);
		}
	}

	ଯଦି (S_ISGITLINK(p->mod_src)) {
		ଯଦି (p->status != 'D')
			src_abbrev = verify_submodule_committish(p->sm_path,
								 oid_to_hex(&p->oid_src));
		ଯଦି (!src_abbrev) {
			missing_src = 1;
			/*
			 * As `rev-parse` failed, we fallback to getting
			 * the abbreviated hash using oid_src. We do
			 * this as we might still need the abbreviated
			 * hash in cases like a submodule type change, etc.
			 */
			src_abbrev = xstrndup(oid_to_hex(&p->oid_src), 7);
		}
	} ଅନ୍ୟଥା {
		/*
		 * The source does not point to a submodule.
		 * So, we fallback to getting the abbreviation using
		 * oid_src as we might still need the abbreviated
		 * hash in cases like submodule add, etc.
		 */
		src_abbrev = xstrndup(oid_to_hex(&p->oid_src), 7);
	}

	ଯଦି (S_ISGITLINK(p->mod_dst)) {
		dst_abbrev = verify_submodule_committish(p->sm_path,
							 oid_to_hex(&p->oid_dst));
		ଯଦି (!dst_abbrev) {
			missing_dst = 1;
			/*
			 * As `rev-parse` failed, we fallback to getting
			 * the abbreviated hash using oid_dst. We do
			 * this as we might still need the abbreviated
			 * hash in cases like a submodule type change, etc.
			 */
			dst_abbrev = xstrndup(oid_to_hex(&p->oid_dst), 7);
		}
	} ଅନ୍ୟଥା {
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

	ଯଦି (!missing_src && !missing_dst) {
		ସଂରଚନା child_process cp_rev_list = CHILD_PROCESS_INIT;
		ସଂରଚନା strbuf sb_rev_list = STRBUF_INIT;

		strvec_pushl(&cp_rev_list.args, "rev-list",
			     "--first-parent", "--count", ଖାଲି);
		ଯଦି (S_ISGITLINK(p->mod_src) && S_ISGITLINK(p->mod_dst))
			strvec_pushf(&cp_rev_list.args, "%s...%s",
				     src_abbrev, dst_abbrev);
		ଅନ୍ୟଥା
			strvec_ଠେଲ(&cp_rev_list.args, S_ISGITLINK(p->mod_src) ?
				    src_abbrev : dst_abbrev);
		strvec_ଠେଲ(&cp_rev_list.args, "--");

		cp_rev_list.git_cmd = 1;
		cp_rev_list.dir = p->sm_path;
		prepare_submodule_repo_env(&cp_rev_list.env);

		ଯଦି (!capture_command(&cp_rev_list, &sb_rev_list, 0))
			total_commits = atoi(sb_rev_list.buf);

		strbuf_release(&sb_rev_list);
	} ଅନ୍ୟଥା {
		/*
		 * Don't give error msg for modification whose dst is not
		 * submodule, i.e., deleted or changed to blob
		 */
		ଯଦି (S_ISGITLINK(p->mod_dst)) {
			ଯଦି (missing_src && missing_dst) {
				strbuf_addf(&errmsg, "  Warn: %s doesn't contain commits %s and %s\n",
					    displaypath, oid_to_hex(&p->oid_src),
					    oid_to_hex(&p->oid_dst));
			} ଅନ୍ୟଥା {
				strbuf_addf(&errmsg, "  Warn: %s doesn't contain commit %s\n",
					    displaypath, missing_src ?
					    oid_to_hex(&p->oid_src) :
					    oid_to_hex(&p->oid_dst));
			}
		}
	}

	print_submodule_summary(info, errmsg.len ? errmsg.buf : ଖାଲି,
				total_commits, displaypath, src_abbrev,
				dst_abbrev, p);

	ମୁକ୍ତ(displaypath);
	ମୁକ୍ତ(src_abbrev);
	ମୁକ୍ତ(dst_abbrev);
	strbuf_release(&errmsg);
}

ସ୍ଥିର ଶୂନ୍ୟ prepare_submodule_summary(ସଂରଚନା summary_cb *info,
				      ସଂରଚନା module_cb_list *list)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
	ପାଇଁ (i = 0; i < list->nr; i++) {
		ଅଚଳ ସଂରଚନା submodule *sub;
		ସଂରଚନା module_cb *p = list->entries[i];
		ସଂରଚନା strbuf sm_gitdir = STRBUF_INIT;

		ଯଦି (p->status == 'D' || p->status == 'T') {
			generate_submodule_summary(info, p);
			ଜାରି;
		}

		ଯଦି (info->for_status && p->status != 'A' &&
		    (sub = submodule_from_path(the_repository,
					       null_oid(the_hash_algo), p->sm_path))) {
			ଅକ୍ଷର *config_ଚାବି = ଖାଲି;
			ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ;
			ପୂର୍ଣ୍ଣସଂଖ୍ୟା ignore_all = 0;

			config_ଚାବି = xstrfmt("submodule.%s.ignore",
					     sub->name);
			ଯଦି (!repo_config_get_string_tmp(the_repository, config_ଚାବି, &ମୂଲ୍ୟ))
				ignore_all = !strcmp(ମୂଲ୍ୟ, "all");
			ଅନ୍ୟଥା ଯଦି (sub->ignore)
				ignore_all = !strcmp(sub->ignore, "all");

			ମୁକ୍ତ(config_ଚାବି);
			ଯଦି (ignore_all)
				ଜାରି;
		}

		/* Also show added or modified modules which are checked out */
		strbuf_addstr(&sm_gitdir, p->sm_path);
		ଯଦି (is_nonbare_repository_dir(&sm_gitdir))
			generate_submodule_summary(info, p);
		strbuf_release(&sm_gitdir);
	}
}

ସ୍ଥିର ଶୂନ୍ୟ submodule_summary_callback(ସଂରଚନା diff_queue_struct *q,
				       ସଂରଚନା diff_options *options UNUSED,
				       ଶୂନ୍ୟ *data)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
	ସଂରଚନା module_cb_list *list = data;
	ପାଇଁ (i = 0; i < q->nr; i++) {
		ସଂରଚନା diff_filepair *p = q->queue[i];
		ସଂରଚନା module_cb *temp;

		ଯଦି (!S_ISGITLINK(p->one->mode) && !S_ISGITLINK(p->two->mode))
			ଜାରି;
		temp = xmalloc(ଆକାର(*temp));
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

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *get_diff_cmd(ଗଣନା diff_cmd diff_cmd)
{
	ବାଛ (diff_cmd) {
	ସ୍ଥିତି DIFF_INDEX: ଫେରନ୍ତୁ "diff-index";
	ସ୍ଥିତି DIFF_FILES: ଫେରନ୍ତୁ "diff-files";
	ପୂର୍ବନିର୍ଦ୍ଧାରିତ: BUG("bad diff_cmd value %d", diff_cmd);
	}
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା compute_summary_module_list(ସଂରଚନା object_id *head_oid,
				       ସଂରଚନା summary_cb *info,
				       ଗଣନା diff_cmd diff_cmd)
{
	ସଂରଚନା strvec diff_args = STRVEC_INIT;
	ସଂରଚନା rev_info rev;
	ସଂରଚନା module_cb_list list = MODULE_CB_LIST_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;

	strvec_ଠେଲ(&diff_args, get_diff_cmd(diff_cmd));
	ଯଦି (info->cached)
		strvec_ଠେଲ(&diff_args, "--cached");
	strvec_pushl(&diff_args, "--ignore-submodules=dirty", "--raw", ଖାଲି);
	ଯଦି (head_oid)
		strvec_ଠେଲ(&diff_args, oid_to_hex(head_oid));
	strvec_ଠେଲ(&diff_args, "--");
	ଯଦି (info->argc)
		strvec_pushv(&diff_args, info->argv);

	repo_config(the_repository, git_diff_basic_config, ଖାଲି);
	repo_init_revisions(the_repository, &rev, info->prefix);
	rev.abbrev = 0;
	precompose_argv_prefix(diff_args.nr, diff_args.v, ଖାଲି);
	setup_revisions_from_strvec(&diff_args, &rev, ଖାଲି);
	rev.diffopt.output_format = DIFF_FORMAT_NO_OUTPUT | DIFF_FORMAT_CALLBACK;
	rev.diffopt.format_callback = submodule_summary_callback;
	rev.diffopt.format_callback_data = &list;

	ଯଦି (!info->cached) {
		ଯଦି (diff_cmd == DIFF_INDEX)
			setup_work_ବୃକ୍ଷ(the_repository);
		ଯଦି (repo_read_index_preload(the_repository, &rev.diffopt.pathspec, 0) < 0) {
			perror("repo_read_index_preload");
			ret = -1;
			ଯାଅ cleanup;
		}
	} ଅନ୍ୟଥା ଯଦି (repo_read_index(the_repository) < 0) {
		perror("repo_read_cache");
		ret = -1;
		ଯାଅ cleanup;
	}

	ଯଦି (diff_cmd == DIFF_INDEX)
		run_diff_index(&rev, info->cached ? DIFF_INDEX_CACHED : 0);
	ଅନ୍ୟଥା
		run_diff_files(&rev, 0);
	prepare_submodule_summary(info, &list);
cleanup:
	strvec_clear(&diff_args);
	release_revisions(&rev);
	module_cb_list_release(&list);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା module_summary(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
			  ସଂରଚନା repository *repo UNUSED)
{
	ସଂରଚନା summary_cb info = SUMMARY_CB_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା cached = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା for_status = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା files = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା summary_limit = -1;
	ଗଣନା diff_cmd diff_cmd = DIFF_INDEX;
	ସଂରଚନା object_id head_oid;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
	ସଂରଚନା option module_summary_options[] = {
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
	ଅଚଳ ଅକ୍ଷର *ଅଚଳ git_submodule_helper_usage[] = {
		N_("git submodule summary [<options>] [<commit>] [--] [<path>]"),
		ଖାଲି
	};

	argc = parse_options(argc, argv, prefix, module_summary_options,
			     git_submodule_helper_usage, 0);

	ଯଦି (!summary_limit)
		ଫେରନ୍ତୁ 0;

	ଯଦି (!repo_get_oid(the_repository, argc ? argv[0] : "HEAD", &head_oid)) {
		ଯଦି (argc) {
			argv++;
			argc--;
		}
	} ଅନ୍ୟଥା ଯଦି (!argc || !strcmp(argv[0], "HEAD")) {
		/* before the first commit: compare with an empty tree */
		oidcpy(&head_oid, the_hash_algo->empty_ବୃକ୍ଷ);
		ଯଦି (argc) {
			argv++;
			argc--;
		}
	} ଅନ୍ୟଥା {
		ଯଦି (repo_get_oid(the_repository, "HEAD", &head_oid))
			die(_("could not fetch a revision for HEAD"));
	}

	ଯଦି (files) {
		ଯଦି (cached)
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

	ret = compute_summary_module_list((diff_cmd == DIFF_INDEX) ? &head_oid : ଖାଲି,
					  &info, diff_cmd);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା module_gitdir(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix UNUSED,
			 ସଂରଚନା repository *repo)
{
	ସଂରଚନା strbuf gitdir = STRBUF_INIT;

	ଯଦି (argc != 2)
		usage(_("git submodule--helper gitdir <name>"));

	submodule_name_to_gitdir(&gitdir, repo, argv[1]);

	printf("%s\n", gitdir.buf);

	strbuf_release(&gitdir);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା module_migrate(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc UNUSED, ଅଚଳ ଅକ୍ଷର **argv UNUSED,
			  ଅଚଳ ଅକ୍ଷର *prefix UNUSED, ସଂରଚନା repository *repo)
{
	ସଂରଚନା strbuf module_dir = STRBUF_INIT;
	DIR *dir;
	ସଂରଚନା dirent *de;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା repo_version = 0;

	repo_git_path_append(repo, &module_dir, "modules/");

	dir = opendir(module_dir.buf);
	ଯଦି (!dir)
		die(_("could not open '%s'"), module_dir.buf);

	ଯେତେବେଳେ ((de = readdir(dir))) {
		ସଂରଚନା strbuf gitdir_path = STRBUF_INIT;
		ଅକ୍ଷର *ଚାବି;
		ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ;

		ଯଦି (is_dot_or_dotdot(de->d_name))
			ଜାରି;

		strbuf_addf(&gitdir_path, "%s/%s", module_dir.buf, de->d_name);
		ଯଦି (!is_git_directory(gitdir_path.buf)) {
			strbuf_release(&gitdir_path);
			ଜାରି;
		}
		strbuf_release(&gitdir_path);

		ଚାବି = xstrfmt("submodule.%s.gitdir", de->d_name);
		ଯଦି (!repo_config_get_string_tmp(repo, ଚାବି, &ମୂଲ୍ୟ)) {
			/* Already has a gitdir config, nothing to do. */
			ମୁକ୍ତ(ଚାବି);
			ଜାରି;
		}
		ମୁକ୍ତ(ଚାବି);

		create_default_gitdir_config(de->d_name);
	}

	closedir(dir);
	strbuf_release(&module_dir);

	repo_config_get_int(the_repository, "core.repositoryformatversion", &repo_version);
	ଯଦି (repo_version == 0 &&
	    repo_config_set_gently(repo, "core.repositoryformatversion", "1"))
		die(_("could not set core.repositoryformatversion to 1.\n"
		      "Please set it for migration to work, for example:\n"
		      "git config core.repositoryformatversion 1"));

	ଯଦି (repo_config_set_gently(repo, "extensions.submodulePathConfig", "true"))
		die(_("could not enable submodulePathConfig extension. It is required\n"
		      "for migration to work. Please enable it in the root repo:\n"
		      "git config extensions.submodulePathConfig true"));

	repo->repository_format_submodule_path_cfg = 1;

	ଫେରନ୍ତୁ 0;
}

ସଂରଚନା sync_cb {
	ଅଚଳ ଅକ୍ଷର *prefix;
	ଅଚଳ ଅକ୍ଷର *super_prefix;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags;
};
#define SYNC_CB_INIT { 0 }

ସ୍ଥିର ଶୂନ୍ୟ sync_submodule(ଅଚଳ ଅକ୍ଷର *path, ଅଚଳ ଅକ୍ଷର *prefix,
			   ଅଚଳ ଅକ୍ଷର *super_prefix, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
	ଅଚଳ ସଂରଚନା submodule *sub;
	ଅକ୍ଷର *remote_ଚାବି = ଖାଲି;
	ଅକ୍ଷର *sub_origin_url, *super_config_url, *displaypath, *default_remote;
	ସଂରଚନା strbuf sb = STRBUF_INIT;
	ଅକ୍ଷର *sub_config_path = ଖାଲି;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା code;

	ଯଦି (!is_submodule_active(the_repository, path))
		ଫେରନ୍ତୁ;

	ଯଦି (validate_submodule_path(path) < 0)
		die(ଖାଲି);

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);

	ଯଦି (sub && sub->url) {
		ଯଦି (starts_with_dot_dot_slash(sub->url) ||
		    starts_with_dot_slash(sub->url)) {
			ଅକ୍ଷର *up_path = get_up_path(path);

			sub_origin_url = resolve_relative_url(sub->url, up_path, 1);
			super_config_url = resolve_relative_url(sub->url, ଖାଲି, 1);
			ମୁକ୍ତ(up_path);
		} ଅନ୍ୟଥା {
			sub_origin_url = xstrdup(sub->url);
			super_config_url = xstrdup(sub->url);
		}
	} ଅନ୍ୟଥା {
		sub_origin_url = xstrdup("");
		super_config_url = xstrdup("");
	}

	displaypath = get_submodule_displaypath(path, prefix, super_prefix);

	ଯଦି (!(flags & OPT_QUIET))
		printf(_("Synchronizing submodule url for '%s'\n"),
			 displaypath);

	strbuf_reset(&sb);
	strbuf_addf(&sb, "submodule.%s.url", sub->name);
	ଯଦି (repo_config_set_gently(the_repository, sb.buf, super_config_url))
		die(_("failed to register url for submodule path '%s'"),
		      displaypath);

	ଯଦି (!is_submodule_populated_gently(path, ଖାଲି))
		ଯାଅ cleanup;

	strbuf_reset(&sb);
	code = get_default_remote_submodule(path, &default_remote);
	ଯଦି (code)
		exit(code);

	remote_ଚାବି = xstrfmt("remote.%s.url", default_remote);
	ମୁକ୍ତ(default_remote);

	submodule_to_gitdir(the_repository, &sb, path);
	strbuf_addstr(&sb, "/config");

	ଯଦି (repo_config_set_in_file_gently(the_repository, sb.buf, remote_ଚାବି, ଖାଲି, sub_origin_url))
		die(_("failed to update remote for submodule '%s'"),
		      path);

	ଯଦି (flags & OPT_RECURSIVE) {
		ସଂରଚନା child_process cpr = CHILD_PROCESS_INIT;

		cpr.git_cmd = 1;
		cpr.dir = path;
		prepare_submodule_repo_env(&cpr.env);

		strvec_pushl(&cpr.args, "submodule--helper", "sync",
			     "--recursive", ଖାଲି);
		strvec_pushf(&cpr.args, "--super-prefix=%s/", displaypath);

		ଯଦି (flags & OPT_QUIET)
			strvec_ଠେଲ(&cpr.args, "--quiet");

		ଯଦି (run_command(&cpr))
			die(_("failed to recurse into submodule '%s'"),
			      path);
	}

cleanup:
	ମୁକ୍ତ(super_config_url);
	ମୁକ୍ତ(sub_origin_url);
	strbuf_release(&sb);
	ମୁକ୍ତ(remote_ଚାବି);
	ମୁକ୍ତ(displaypath);
	ମୁକ୍ତ(sub_config_path);
}

ସ୍ଥିର ଶୂନ୍ୟ sync_submodule_cb(ଅଚଳ ସଂରଚନା cache_entry *list_item, ଶୂନ୍ୟ *cb_data)
{
	ସଂରଚନା sync_cb *info = cb_data;

	sync_submodule(list_item->name, info->prefix, info->super_prefix,
		       info->flags);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା module_sync(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
		       ସଂରଚନା repository *repo UNUSED)
{
	ସଂରଚନା sync_cb info = SYNC_CB_INIT;
	ସଂରଚନା pathspec pathspec = { 0 };
	ସଂରଚନା module_list list = MODULE_LIST_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା quiet = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା recursive = 0;
	ସଂରଚନା option module_sync_options[] = {
		OPT__SUPER_PREFIX(&info.super_prefix),
		OPT__QUIET(&quiet, N_("suppress output of synchronizing submodule url")),
		OPT_BOOL(0, "recursive", &recursive,
			N_("recurse into nested submodules")),
		OPT_END()
	};
	ଅଚଳ ଅକ୍ଷର *ଅଚଳ git_submodule_helper_usage[] = {
		N_("git submodule sync [--quiet] [--recursive] [<path>]"),
		ଖାଲି
	};
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 1;

	argc = parse_options(argc, argv, prefix, module_sync_options,
			     git_submodule_helper_usage, 0);

	ଯଦି (module_list_compute(argv, prefix, &pathspec, &list) < 0)
		ଯାଅ cleanup;

	info.prefix = prefix;
	ଯଦି (quiet)
		info.flags |= OPT_QUIET;
	ଯଦି (recursive)
		info.flags |= OPT_RECURSIVE;

	for_each_listed_submodule(&list, sync_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	ଫେରନ୍ତୁ ret;
}

ସଂରଚନା deinit_cb {
	ଅଚଳ ଅକ୍ଷର *prefix;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags;
};
#define DEINIT_CB_INIT { 0 }

ସ୍ଥିର ଶୂନ୍ୟ deinit_submodule(ଅଚଳ ଅକ୍ଷର *path, ଅଚଳ ଅକ୍ଷର *prefix,
			     unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
	ଅଚଳ ସଂରଚନା submodule *sub;
	ଅକ୍ଷର *displaypath = ଖାଲି;
	ସଂରଚନା child_process cp_config = CHILD_PROCESS_INIT;
	ସଂରଚନା strbuf sb_config = STRBUF_INIT;
	ଅକ୍ଷର *sub_git_dir = xstrfmt("%s/.git", path);

	ଯଦି (validate_submodule_path(path) < 0)
		die(ଖାଲି);

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);

	ଯଦି (!sub || !sub->name)
		ଯାଅ cleanup;

	displaypath = get_submodule_displaypath(path, prefix, ଖାଲି);

	/* remove the submodule work tree (unless the user already did it) */
	ଯଦି (is_directory(path)) {
		ସଂରଚନା strbuf sb_rm = STRBUF_INIT;
		ଅଚଳ ଅକ୍ଷର *format;

		ଯଦି (is_directory(sub_git_dir)) {
			ଯଦି (!(flags & OPT_QUIET))
				warning(_("Submodule work tree '%s' contains a .git "
					  "directory. This will be replaced with a "
					  ".git file by using absorbgitdirs."),
					displaypath);

			absorb_git_dir_into_superproject(path, ଖାଲି);

		}

		ଯଦି (!(flags & OPT_FORCE)) {
			ସଂରଚନା child_process cp_rm = CHILD_PROCESS_INIT;

			cp_rm.git_cmd = 1;
			strvec_pushl(&cp_rm.args, "rm", "-qn",
				     path, ଖାଲି);

			ଯଦି (run_command(&cp_rm))
				die(_("Submodule work tree '%s' contains local "
				      "modifications; use '-f' to discard them"),
				      displaypath);
		}

		strbuf_addstr(&sb_rm, path);

		ଯଦି (!remove_dir_recursively(&sb_rm, 0))
			format = _("Cleared directory '%s'\n");
		ଅନ୍ୟଥା
			format = _("Could not remove submodule work tree '%s'\n");

		ଯଦି (!(flags & OPT_QUIET))
			printf(format, displaypath);

		submodule_unset_core_worktree(sub);

		strbuf_release(&sb_rm);
	}

	ଯଦି (mkdir(path, 0777))
		printf(_("could not create empty submodule directory %s"),
		      displaypath);

	cp_config.git_cmd = 1;
	strvec_pushl(&cp_config.args, "config", "--get-regexp", ଖାଲି);
	strvec_pushf(&cp_config.args, "submodule.%s\\.", sub->name);

	/* remove the .git/config entries (unless the user already did it) */
	ଯଦି (!capture_command(&cp_config, &sb_config, 0) && sb_config.len) {
		ଅକ୍ଷର *sub_ଚାବି = xstrfmt("submodule.%s", sub->name);

		/*
		 * remove the whole section so we have a clean state when
		 * the user later decides to init this submodule again
		 */
		repo_config_rename_section_in_file(the_repository, ଖାଲି, sub_ଚାବି, ଖାଲି);
		ଯଦି (!(flags & OPT_QUIET))
			printf(_("Submodule '%s' (%s) unregistered for path '%s'\n"),
				 sub->name, sub->url, displaypath);
		ମୁକ୍ତ(sub_ଚାବି);
	}

cleanup:
	ମୁକ୍ତ(displaypath);
	ମୁକ୍ତ(sub_git_dir);
	strbuf_release(&sb_config);
}

ସ୍ଥିର ଶୂନ୍ୟ deinit_submodule_cb(ଅଚଳ ସଂରଚନା cache_entry *list_item,
				ଶୂନ୍ୟ *cb_data)
{
	ସଂରଚନା deinit_cb *info = cb_data;
	deinit_submodule(list_item->name, info->prefix, info->flags);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା module_deinit(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
			 ସଂରଚନା repository *repo UNUSED)
{
	ସଂରଚନା deinit_cb info = DEINIT_CB_INIT;
	ସଂରଚନା pathspec pathspec = { 0 };
	ସଂରଚନା module_list list = MODULE_LIST_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା quiet = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା force = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା all = 0;
	ସଂରଚନା option module_deinit_options[] = {
		OPT__QUIET(&quiet, N_("suppress submodule status output")),
		OPT__FORCE(&force, N_("remove submodule working trees even if they contain local changes"), 0),
		OPT_BOOL(0, "all", &all, N_("unregister all submodules")),
		OPT_END()
	};
	ଅଚଳ ଅକ୍ଷର *ଅଚଳ git_submodule_helper_usage[] = {
		N_("git submodule deinit [--quiet] [-f | --force] [--all | [--] [<path>...]]"),
		ଖାଲି
	};
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 1;

	argc = parse_options(argc, argv, prefix, module_deinit_options,
			     git_submodule_helper_usage, 0);

	ଯଦି (all && argc) {
		error("pathspec and --all are incompatible");
		usage_with_options(git_submodule_helper_usage,
				   module_deinit_options);
	}

	ଯଦି (!argc && !all)
		die(_("Use '--all' if you really want to deinitialize all submodules"));

	ଯଦି (module_list_compute(argv, prefix, &pathspec, &list) < 0)
		ଯାଅ cleanup;

	info.prefix = prefix;
	ଯଦି (quiet)
		info.flags |= OPT_QUIET;
	ଯଦି (force)
		info.flags |= OPT_FORCE;

	for_each_listed_submodule(&list, deinit_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	ଫେରନ୍ତୁ ret;
}

ସଂରଚନା module_clone_data {
	ଅଚଳ ଅକ୍ଷର *prefix;
	ଅଚଳ ଅକ୍ଷର *path;
	ଅଚଳ ଅକ୍ଷର *name;
	ଅଚଳ ଅକ୍ଷର *url;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth;
	ସଂରଚନା list_objects_filter_options *filter_options;
	ଗଣନା ref_storage_format ref_storage_format;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା quiet: 1;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା progress: 1;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା dissociate: 1;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା require_init: 1;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା single_branch;
};
#define MODULE_CLONE_DATA_INIT { \
	.single_branch = -1, \
	.ref_storage_format = REF_STORAGE_FORMAT_UNKNOWN, \
}

ସଂରଚନା submodule_alternate_setup {
	ଅଚଳ ଅକ୍ଷର *submodule_name;
	ଗଣନା SUBMODULE_ALTERNATE_ERROR_MODE {
		SUBMODULE_ALTERNATE_ERROR_DIE,
		SUBMODULE_ALTERNATE_ERROR_INFO,
		SUBMODULE_ALTERNATE_ERROR_IGNORE
	} error_mode;
	ସଂରଚନା string_list *reference;
};
#define SUBMODULE_ALTERNATE_SETUP_INIT { \
	.error_mode = SUBMODULE_ALTERNATE_ERROR_IGNORE, \
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର alternate_error_advice[] = N_(
"An alternate computed from a superproject's alternate is invalid.\n"
"To allow Git to clone without an alternate in such a case, set\n"
"submodule.alternateErrorStrategy to 'info' or, equivalently, clone with\n"
"'--reference-if-able' instead of '--reference'."
);

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା add_possible_reference_from_superproject(
		ସଂରଚନା odb_files_dir *alt_odb, ଶୂନ୍ୟ *sas_cb)
{
	ସଂରଚନା submodule_alternate_setup *sas = sas_cb;
	size_t len;

	/*
	 * If the alternate object store is another repository, try the
	 * standard layout with .git/(modules/<name>)+/objects
	 */
	ଯଦି (strip_suffix(alt_odb->abspath, "/objects", &len)) {
		ସଂରଚନା repository alternate;
		ଅକ୍ଷର *sm_alternate;
		ସଂରଚନା strbuf sb = STRBUF_INIT;
		ସଂରଚନା strbuf err = STRBUF_INIT;
		strbuf_add(&sb, alt_odb->abspath, len);

		ଯଦି (repo_init(&alternate, sb.buf, ଖାଲି) < 0)
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
		ଯଦି (sm_alternate) {
			ଅକ୍ଷର *p = strbuf_detach(&sb, ଖାଲି);

			string_list_append(sas->reference, p)->util = p;
			ମୁକ୍ତ(sm_alternate);
		} ଅନ୍ୟଥା {
			ବାଛ (sas->error_mode) {
			ସ୍ଥିତି SUBMODULE_ALTERNATE_ERROR_DIE:
				ଯଦି (advice_enabled(ADVICE_SUBMODULE_ALTERNATE_ERROR_STRATEGY_DIE))
					advise(_(alternate_error_advice));
				die(_("submodule '%s' cannot add alternate: %s"),
				    sas->submodule_name, err.buf);
			ସ୍ଥିତି SUBMODULE_ALTERNATE_ERROR_INFO:
				fprintf_ln(stderr, _("submodule '%s' cannot add alternate: %s"),
					sas->submodule_name, err.buf);
			ସ୍ଥିତି SUBMODULE_ALTERNATE_ERROR_IGNORE:
				; /* nothing */
			}
		}

		strbuf_release(&err);
		strbuf_release(&sb);
	}

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ prepare_possible_alternates(ଅଚଳ ଅକ୍ଷର *sm_name,
		ସଂରଚନା string_list *reference)
{
	ଅକ୍ଷର *sm_alternate = ଖାଲି, *error_strategy = ଖାଲି;
	ସଂରଚନା submodule_alternate_setup sas = SUBMODULE_ALTERNATE_SETUP_INIT;

	repo_config_get_string(the_repository, "submodule.alternateLocation", &sm_alternate);
	ଯଦି (!sm_alternate)
		ଫେରନ୍ତୁ;

	repo_config_get_string(the_repository, "submodule.alternateErrorStrategy", &error_strategy);

	ଯଦି (!error_strategy)
		error_strategy = xstrdup("die");

	sas.submodule_name = sm_name;
	sas.reference = reference;
	ଯଦି (!strcmp(error_strategy, "die"))
		sas.error_mode = SUBMODULE_ALTERNATE_ERROR_DIE;
	ଅନ୍ୟଥା ଯଦି (!strcmp(error_strategy, "info"))
		sas.error_mode = SUBMODULE_ALTERNATE_ERROR_INFO;
	ଅନ୍ୟଥା ଯଦି (!strcmp(error_strategy, "ignore"))
		sas.error_mode = SUBMODULE_ALTERNATE_ERROR_IGNORE;
	ଅନ୍ୟଥା
		die(_("Value '%s' for submodule.alternateErrorStrategy is not recognized"), error_strategy);

	ଯଦି (!strcmp(sm_alternate, "superproject"))
		odb_for_each_alternate(the_repository->objects,
				       add_possible_reference_from_superproject, &sas);
	ଅନ୍ୟଥା ଯଦି (!strcmp(sm_alternate, "no"))
		; /* do nothing */
	ଅନ୍ୟଥା
		die(_("Value '%s' for submodule.alternateLocation is not recognized"), sm_alternate);

	ମୁକ୍ତ(sm_alternate);
	ମୁକ୍ତ(error_strategy);
}

ସ୍ଥିର ଅକ୍ଷର *clone_submodule_sm_gitdir(ଅଚଳ ଅକ୍ଷର *name)
{
	ସଂରଚନା strbuf sb = STRBUF_INIT;
	ଅକ୍ଷର *sm_gitdir;

	submodule_name_to_gitdir(&sb, the_repository, name);
	sm_gitdir = absolute_pathdup(sb.buf);
	strbuf_release(&sb);

	ଫେରନ୍ତୁ sm_gitdir;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା dir_contains_only_dotgit(ଅଚଳ ଅକ୍ଷର *path)
{
	DIR *dir = opendir(path);
	ସଂରଚନା dirent *e;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 1;

	ଯଦି (!dir)
		ଫେରନ୍ତୁ 0;

	e = readdir_skip_dot_and_dotdot(dir);
	ଯଦି (!e)
		ret = 0;
	ଅନ୍ୟଥା ଯଦି (strcmp(DEFAULT_GIT_DIR_ENVIRONMENT, e->d_name) ||
		 (e = readdir_skip_dot_and_dotdot(dir))) {
		error("unexpected item '%s' in '%s'", e->d_name, path);
		ret = 0;
	}

	closedir(dir);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା clone_submodule(ଅଚଳ ସଂରଚନା module_clone_data *clone_data,
			   ସଂରଚନା string_list *reference)
{
	ଅକ୍ଷର *p;
	ଅକ୍ଷର *sm_gitdir = clone_submodule_sm_gitdir(clone_data->name);
	ଅକ୍ଷର *sm_alternate = ଖାଲି, *error_strategy = ଖାଲି;
	ସଂରଚନା stat st;
	ସଂରଚନା child_process cp = CHILD_PROCESS_INIT;
	ଅଚଳ ଅକ୍ଷର *clone_data_path = clone_data->path;
	ଅକ୍ଷର *to_free = ଖାଲି;

	ଯଦି (validate_submodule_path(clone_data_path) < 0)
		die(ଖାଲି);

	ଯଦି (!is_absolute_path(clone_data->path))
		clone_data_path = to_free = xstrfmt("%s/%s", repo_get_work_ବୃକ୍ଷ(the_repository),
						    clone_data->path);

	ଯଦି (!file_exists(sm_gitdir)) {
		ଯଦି (clone_data->require_init && !stat(clone_data_path, &st) &&
		    !is_empty_dir(clone_data_path))
			die(_("directory not empty: '%s'"), clone_data_path);

		ଯଦି (safe_create_leading_directories_const(the_repository, sm_gitdir) < 0)
			die(_("could not create directory '%s'"), sm_gitdir);

		prepare_possible_alternates(clone_data->name, reference);

		strvec_ଠେଲ(&cp.args, "clone");
		strvec_ଠେଲ(&cp.args, "--no-checkout");
		ଯଦି (clone_data->quiet)
			strvec_ଠେଲ(&cp.args, "--quiet");
		ଯଦି (clone_data->progress)
			strvec_ଠେଲ(&cp.args, "--progress");
		ଯଦି (clone_data->depth > 0)
			strvec_pushf(&cp.args, "--depth=%d", clone_data->depth);
		ଯଦି (reference->nr) {
			ସଂରଚନା string_list_item *item;

			for_each_string_list_item(item, reference)
				strvec_pushl(&cp.args, "--reference",
					     item->string, ଖାଲି);
		}
		ଯଦି (clone_data->ref_storage_format != REF_STORAGE_FORMAT_UNKNOWN)
			strvec_pushf(&cp.args, "--ref-storage-format=%s",
				     ref_storage_format_to_name(clone_data->ref_storage_format));
		ଯଦି (clone_data->dissociate)
			strvec_ଠେଲ(&cp.args, "--dissociate");
		ଯଦି (sm_gitdir && *sm_gitdir)
			strvec_pushl(&cp.args, "--separate-git-dir", sm_gitdir, ଖାଲି);
		ଯଦି (clone_data->filter_options && clone_data->filter_options->choice)
			strvec_pushf(&cp.args, "--filter=%s",
				     expand_list_objects_filter_spec(
					     clone_data->filter_options));
		ଯଦି (clone_data->single_branch >= 0)
			strvec_ଠେଲ(&cp.args, clone_data->single_branch ?
				    "--single-branch" :
				    "--no-single-branch");

		strvec_ଠେଲ(&cp.args, "--");
		strvec_ଠେଲ(&cp.args, clone_data->url);
		strvec_ଠେଲ(&cp.args, clone_data_path);

		cp.git_cmd = 1;
		prepare_submodule_repo_env(&cp.env);
		cp.no_stdin = 1;

		ଯଦି(run_command(&cp))
			die(_("clone of '%s' into submodule path '%s' failed"),
			    clone_data->url, clone_data_path);

		ଯଦି (clone_data->require_init && !stat(clone_data_path, &st) &&
		    !dir_contains_only_dotgit(clone_data_path)) {
			ଅକ୍ଷର *dot_git = xstrfmt("%s/.git", clone_data_path);
			unlink(dot_git);
			ମୁକ୍ତ(dot_git);
			die(_("directory not empty: '%s'"), clone_data_path);
		}
	} ଅନ୍ୟଥା {
		ଅକ୍ଷର *path;

		ଯଦି (clone_data->require_init && !stat(clone_data_path, &st) &&
		    !is_empty_dir(clone_data_path))
			die(_("directory not empty: '%s'"), clone_data_path);
		ଯଦି (safe_create_leading_directories_const(the_repository, clone_data_path) < 0)
			die(_("could not create directory '%s'"), clone_data_path);
		path = xstrfmt("%s/index", sm_gitdir);
		unlink_or_warn(path);
		ମୁକ୍ତ(path);
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
	ଯଦି (validate_submodule_git_dir(sm_gitdir, clone_data->name) < 0) {
		ଅକ୍ଷର *head = xstrfmt("%s/HEAD", sm_gitdir);
		unlink(head);
		ମୁକ୍ତ(head);
		die(_("refusing to create/use '%s' in another submodule's git dir. "
		      "Enabling extensions.submodulePathConfig should fix this."),
		    sm_gitdir);
	}

	connect_work_ବୃକ୍ଷ_and_git_dir(clone_data_path, sm_gitdir, 0);

	p = repo_submodule_path(the_repository, clone_data_path, "config");
	ଯଦି (!p)
		die(_("could not get submodule directory for '%s'"), clone_data_path);

	/* setup alternateLocation and alternateErrorStrategy in the cloned submodule if needed */
	repo_config_get_string(the_repository, "submodule.alternateLocation", &sm_alternate);
	ଯଦି (sm_alternate)
		repo_config_set_in_file(the_repository, p, "submodule.alternateLocation",
					sm_alternate);
	repo_config_get_string(the_repository, "submodule.alternateErrorStrategy", &error_strategy);
	ଯଦି (error_strategy)
		repo_config_set_in_file(the_repository, p, "submodule.alternateErrorStrategy",
					error_strategy);

	ମୁକ୍ତ(sm_alternate);
	ମୁକ୍ତ(error_strategy);

	ମୁକ୍ତ(sm_gitdir);
	ମୁକ୍ତ(p);
	ମୁକ୍ତ(to_free);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା module_clone(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
			ସଂରଚନା repository *repo UNUSED)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା dissociate = 0, quiet = 0, progress = 0, require_init = 0;
	ସଂରଚନା module_clone_data clone_data = MODULE_CLONE_DATA_INIT;
	ସଂରଚନା string_list reference = STRING_LIST_INIT_NODUP;
	ସଂରଚନା list_objects_filter_options filter_options =
		LIST_OBJECTS_FILTER_INIT;
	ଅଚଳ ଅକ୍ଷର *ref_storage_format = ଖାଲି;

	ସଂରଚନା option module_clone_options[] = {
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
	ଅଚଳ ଅକ୍ଷର *ଅଚଳ git_submodule_helper_usage[] = {
		N_("git submodule--helper clone [--prefix=<path>] [--quiet] "
		   "[--reference <repository>] [--name <name>] [--depth <depth>] "
		   "[--single-branch] [--filter <filter-spec>] "
		   "--url <url> --path <path>"),
		ଖାଲି
	};

	argc = parse_options(argc, argv, prefix, module_clone_options,
			     git_submodule_helper_usage, 0);

	ଯଦି (ref_storage_format) {
		clone_data.ref_storage_format = ref_storage_format_by_name(ref_storage_format);
		ଯଦି (clone_data.ref_storage_format == REF_STORAGE_FORMAT_UNKNOWN)
			die(_("unknown ref storage format '%s'"), ref_storage_format);
	}
	clone_data.dissociate = !!dissociate;
	clone_data.quiet = !!quiet;
	clone_data.progress = !!progress;
	clone_data.require_init = !!require_init;
	clone_data.filter_options = &filter_options;

	ଯଦି (argc || !clone_data.url || !clone_data.path || !*(clone_data.path))
		usage_with_options(git_submodule_helper_usage,
				   module_clone_options);

	clone_submodule(&clone_data, &reference);
	list_objects_filter_release(&filter_options);
	string_list_clear(&reference, 1);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା determine_submodule_update_strategy(ସଂରଚନା repository *r,
					       ପୂର୍ଣ୍ଣସଂଖ୍ୟା just_cloned,
					       ଅଚଳ ଅକ୍ଷର *path,
					       ଗଣନା submodule_update_type update,
					       ସଂରଚନା submodule_update_strategy *out)
{
	ଅଚଳ ସଂରଚନା submodule *sub = submodule_from_path(r, null_oid(the_hash_algo), path);
	ଅକ୍ଷର *ଚାବି;
	ଅଚଳ ଅକ୍ଷର *val;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	/*
	 * NEEDSWORK: audit and ensure that update_submodule() has right
	 * to assume that submodule_from_path() above will always succeed.
	 */
	ଯଦି (!sub)
		BUG("update_submodule assumes a submodule exists at path (%s)",
		    path);
	ଚାବି = xstrfmt("submodule.%s.update", sub->name);

	ଯଦି (update) {
		out->type = update;
	} ଅନ୍ୟଥା ଯଦି (!repo_config_get_string_tmp(r, ଚାବି, &val)) {
		ଯଦି (parse_submodule_update_strategy(val, out) < 0) {
			ret = die_message(_("Invalid update mode '%s' configured for submodule path '%s'"),
					  val, path);
			ଯାଅ cleanup;
		}
	} ଅନ୍ୟଥା ଯଦି (sub->update_strategy.type != SM_UPDATE_UNSPECIFIED) {
		ଯଦି (sub->update_strategy.type == SM_UPDATE_COMMAND)
			BUG("how did we read update = !command from .gitmodules?");
		out->type = sub->update_strategy.type;
		out->command = sub->update_strategy.command;
	} ଅନ୍ୟଥା
		out->type = SM_UPDATE_CHECKOUT;

	ଯଦି (just_cloned &&
	    (out->type == SM_UPDATE_MERGE ||
	     out->type == SM_UPDATE_REBASE ||
	     out->type == SM_UPDATE_NONE))
		out->type = SM_UPDATE_CHECKOUT;

	ret = 0;
cleanup:
	ମୁକ୍ତ(ଚାବି);
	ଫେରନ୍ତୁ ret;
}

ସଂରଚନା update_clone_data {
	ଅଚଳ ସଂରଚନା submodule *sub;
	ସଂରଚନା object_id oid;
	unsigned just_cloned;
};

ସଂରଚନା submodule_update_clone {
	/* index into 'update_data.list', the list of submodules to look into for cloning */
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା current;

	/* configuration parameters which are passed on to the children */
	ଅଚଳ ସଂରଚନା update_data *update_data;

	/* to be consumed by update_submodule() */
	ସଂରଚନା update_clone_data *update_clone;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା update_clone_nr; ପୂର୍ଣ୍ଣସଂଖ୍ୟା update_clone_alloc;

	/* If we want to stop as fast as possible and return an error */
	unsigned quickstop : 1;

	/* failed clones to be retried again */
	ଅଚଳ ସଂରଚନା cache_entry **failed_clones;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା failed_clones_nr, failed_clones_alloc;
};
#define SUBMODULE_UPDATE_CLONE_INIT { 0 }

ସ୍ଥିର ଶୂନ୍ୟ submodule_update_clone_release(ସଂରଚନା submodule_update_clone *suc)
{
	ମୁକ୍ତ(suc->update_clone);
	ମୁକ୍ତ(suc->failed_clones);
}

ସଂରଚନା update_data {
	ଅଚଳ ଅକ୍ଷର *prefix;
	ଅଚଳ ଅକ୍ଷର *super_prefix;
	ଅକ୍ଷର *displaypath;
	ଗଣନା submodule_update_type update_default;
	ସଂରଚନା object_id suboid;
	ସଂରଚନା string_list references;
	ସଂରଚନା submodule_update_strategy update_strategy;
	ସଂରଚନା list_objects_filter_options *filter_options;
	ସଂରଚନା module_list list;
	ଗଣନା ref_storage_format ref_storage_format;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା max_jobs;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା single_branch;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା recommend_shallow;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା require_init;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା force;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା quiet;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା nofetch;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା remote;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା progress;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା dissociate;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା init;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା warn_if_uninitialized;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା recursive;

	/* copied over from update_clone_data */
	ସଂରଚନା object_id oid;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା just_cloned;
	ଅଚଳ ଅକ୍ଷର *sm_path;
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

ସ୍ଥିର ଶୂନ୍ୟ update_data_release(ସଂରଚନା update_data *ud)
{
	ମୁକ୍ତ(ud->displaypath);
	submodule_update_strategy_release(&ud->update_strategy);
	module_list_release(&ud->list);
}

ସ୍ଥିର ଶୂନ୍ୟ next_submodule_warn_missing(ସଂରଚନା submodule_update_clone *suc,
		ସଂରଚନା strbuf *out, ଅଚଳ ଅକ୍ଷର *displaypath)
{
	/*
	 * Only mention uninitialized submodules when their
	 * paths have been specified.
	 */
	ଯଦି (suc->update_data->warn_if_uninitialized) {
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
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା prepare_to_clone_next_submodule(ଅଚଳ ସଂରଚନା cache_entry *ce,
					   ସଂରଚନା child_process *child,
					   ସଂରଚନା submodule_update_clone *suc,
					   ସଂରଚନା strbuf *out)
{
	ଅଚଳ ସଂରଚନା submodule *sub = ଖାଲି;
	ଅଚଳ ଅକ୍ଷର *url = ଖାଲି;
	ଅଚଳ ଅକ୍ଷର *update_string;
	ଗଣନା submodule_update_type update_type;
	ଅକ୍ଷର *ଚାବି;
	ଅଚଳ ସଂରଚନା update_data *ud = suc->update_data;
	ଅକ୍ଷର *displaypath = get_submodule_displaypath(ce->name, ud->prefix,
						      ud->super_prefix);
	ସଂରଚନା strbuf sb = STRBUF_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା needs_cloning = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା need_free_url = 0;

	ଯଦି (ce_stage(ce)) {
		strbuf_addf(out, _("Skipping unmerged submodule %s"), displaypath);
		strbuf_addch(out, '\n');
		ଯାଅ cleanup;
	}

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), ce->name);

	ଯଦି (!sub) {
		next_submodule_warn_missing(suc, out, displaypath);
		ଯାଅ cleanup;
	}

	ଚାବି = xstrfmt("submodule.%s.update", sub->name);
	ଯଦି (!repo_config_get_string_tmp(the_repository, ଚାବି, &update_string)) {
		update_type = parse_submodule_update_type(update_string);
	} ଅନ୍ୟଥା {
		update_type = sub->update_strategy.type;
	}
	ମୁକ୍ତ(ଚାବି);

	ଯଦି (suc->update_data->update_strategy.type == SM_UPDATE_NONE
	    || (suc->update_data->update_strategy.type == SM_UPDATE_UNSPECIFIED
		&& update_type == SM_UPDATE_NONE)) {
		strbuf_addf(out, _("Skipping submodule '%s'"), displaypath);
		strbuf_addch(out, '\n');
		ଯାଅ cleanup;
	}

	/* Check if the submodule has been initialized. */
	ଯଦି (!is_submodule_active(the_repository, ce->name)) {
		next_submodule_warn_missing(suc, out, displaypath);
		ଯାଅ cleanup;
	}

	strbuf_reset(&sb);
	strbuf_addf(&sb, "submodule.%s.url", sub->name);
	ଯଦି (repo_config_get_string_tmp(the_repository, sb.buf, &url)) {
		ଯଦି (sub->url && (starts_with_dot_slash(sub->url) ||
				 starts_with_dot_dot_slash(sub->url))) {
			url = resolve_relative_url(sub->url, ଖାଲି, 0);
			need_free_url = 1;
		} ଅନ୍ୟଥା
			url = sub->url;
	}

	ଯଦି (!url)
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

	ଯଦି (!needs_cloning)
		ଯାଅ cleanup;

	child->git_cmd = 1;
	child->no_stdin = 1;
	child->stdout_to_stderr = 1;
	child->err = -1;
	strvec_ଠେଲ(&child->args, "submodule--helper");
	strvec_ଠେଲ(&child->args, "clone");
	ଯଦି (suc->update_data->progress)
		strvec_ଠେଲ(&child->args, "--progress");
	ଯଦି (suc->update_data->quiet)
		strvec_ଠେଲ(&child->args, "--quiet");
	ଯଦି (suc->update_data->prefix)
		strvec_pushl(&child->args, "--prefix", suc->update_data->prefix, ଖାଲି);
	ଯଦି (suc->update_data->recommend_shallow && sub->recommend_shallow == 1)
		strvec_ଠେଲ(&child->args, "--depth=1");
	ଅନ୍ୟଥା ଯଦି (suc->update_data->depth)
		strvec_pushf(&child->args, "--depth=%d", suc->update_data->depth);
	ଯଦି (suc->update_data->filter_options && suc->update_data->filter_options->choice)
		strvec_pushf(&child->args, "--filter=%s",
			     expand_list_objects_filter_spec(suc->update_data->filter_options));
	ଯଦି (suc->update_data->require_init)
		strvec_ଠେଲ(&child->args, "--require-init");
	ଯଦି (suc->update_data->ref_storage_format != REF_STORAGE_FORMAT_UNKNOWN)
		strvec_pushf(&child->args, "--ref-storage-format=%s",
			     ref_storage_format_to_name(suc->update_data->ref_storage_format));
	strvec_pushl(&child->args, "--path", sub->path, ଖାଲି);
	strvec_pushl(&child->args, "--name", sub->name, ଖାଲି);
	strvec_pushl(&child->args, "--url", url, ଖାଲି);
	ଯଦି (suc->update_data->references.nr) {
		ସଂରଚନା string_list_item *item;

		for_each_string_list_item(item, &suc->update_data->references)
			strvec_pushl(&child->args, "--reference", item->string, ଖାଲି);
	}
	ଯଦି (suc->update_data->dissociate)
		strvec_ଠେଲ(&child->args, "--dissociate");
	ଯଦି (suc->update_data->single_branch >= 0)
		strvec_ଠେଲ(&child->args, suc->update_data->single_branch ?
					      "--single-branch" :
					      "--no-single-branch");

cleanup:
	ମୁକ୍ତ(displaypath);
	strbuf_release(&sb);
	ଯଦି (need_free_url)
		ମୁକ୍ତ((ଶୂନ୍ୟ*)url);

	ଫେରନ୍ତୁ needs_cloning;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା update_clone_get_next_task(ସଂରଚନା child_process *child,
				      ସଂରଚନା strbuf *err,
				      ଶୂନ୍ୟ *suc_cb,
				      ଶୂନ୍ୟ **idx_task_cb)
{
	ସଂରଚନା submodule_update_clone *suc = suc_cb;
	ଅଚଳ ସଂରଚନା cache_entry *ce;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା index;

	ପାଇଁ (; suc->current < suc->update_data->list.nr; suc->current++) {
		ce = suc->update_data->list.entries[suc->current];
		ଯଦି (prepare_to_clone_next_submodule(ce, child, suc, err)) {
			ପୂର୍ଣ୍ଣସଂଖ୍ୟା *p = xmalloc(ଆକାର(*p));

			*p = suc->current;
			*idx_task_cb = p;
			suc->current++;
			ଫେରନ୍ତୁ 1;
		}
	}

	/*
	 * The loop above tried cloning each submodule once, now try the
	 * stragglers again, which we can imagine as an extension of the
	 * entry list.
	 */
	index = suc->current - suc->update_data->list.nr;
	ଯଦି (index < suc->failed_clones_nr) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା *p;

		ce = suc->failed_clones[index];
		ଯଦି (!prepare_to_clone_next_submodule(ce, child, suc, err)) {
			suc->current ++;
			strbuf_addstr(err, "BUG: submodule considered for "
					   "cloning, doesn't need cloning "
					   "any more?\n");
			ଫେରନ୍ତୁ 0;
		}
		p = xmalloc(ଆକାର(*p));
		*p = suc->current;
		*idx_task_cb = p;
		suc->current ++;
		ଫେରନ୍ତୁ 1;
	}

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା update_clone_start_failure(ସଂରଚନା strbuf *err UNUSED,
				      ଶୂନ୍ୟ *suc_cb,
				      ଶୂନ୍ୟ *idx_task_cb UNUSED)
{
	ସଂରଚନା submodule_update_clone *suc = suc_cb;

	suc->quickstop = 1;
	ଫେରନ୍ତୁ 1;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା update_clone_task_finished(ପୂର୍ଣ୍ଣସଂଖ୍ୟା result,
				      ସଂରଚନା strbuf *err,
				      ଶୂନ୍ୟ *suc_cb,
				      ଶୂନ୍ୟ *idx_task_cb)
{
	ଅଚଳ ସଂରଚନା cache_entry *ce;
	ସଂରଚନା submodule_update_clone *suc = suc_cb;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା *idxP = idx_task_cb;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା idx = *idxP;

	ମୁକ୍ତ(idxP);

	ଯଦି (!result)
		ଫେରନ୍ତୁ 0;

	ଯଦି (idx < suc->update_data->list.nr) {
		ce  = suc->update_data->list.entries[idx];
		strbuf_addf(err, _("Failed to clone '%s'. Retry scheduled"),
			    ce->name);
		strbuf_addch(err, '\n');
		ALLOC_GROW(suc->failed_clones,
			   suc->failed_clones_nr + 1,
			   suc->failed_clones_alloc);
		suc->failed_clones[suc->failed_clones_nr++] = ce;
		ଫେରନ୍ତୁ 0;
	} ଅନ୍ୟଥା {
		idx -= suc->update_data->list.nr;
		ce  = suc->failed_clones[idx];
		strbuf_addf(err, _("Failed to clone '%s' a second time, aborting"),
			    ce->name);
		strbuf_addch(err, '\n');
		suc->quickstop = 1;
		ଫେରନ୍ତୁ 1;
	}

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା git_update_clone_config(ଅଚଳ ଅକ୍ଷର *var, ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ,
				   ଅଚଳ ସଂରଚନା config_context *ctx,
				   ଶୂନ୍ୟ *cb)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା *max_jobs = cb;

	ଯଦି (!strcmp(var, "submodule.fetchjobs"))
		*max_jobs = parse_submodule_fetchjobs(var, ମୂଲ୍ୟ, ctx->kvi);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା is_tip_reachable(ଅଚଳ ଅକ୍ଷର *path, ଅଚଳ ସଂରଚନା object_id *oid)
{
	ସଂରଚନା child_process cp = CHILD_PROCESS_INIT;
	ସଂରଚନା strbuf rev = STRBUF_INIT;
	ଅକ୍ଷର *hex = oid_to_hex(oid);
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା reachable;

	cp.git_cmd = 1;
	cp.dir = path;
	cp.no_stderr = 1;
	strvec_pushl(&cp.args, "rev-list", "-n", "1", hex, "--not", "--all", ଖାଲି);

	prepare_submodule_repo_env(&cp.env);

	ଯଦି (capture_command(&cp, &rev, GIT_MAX_HEXSZ + 1) || rev.len)
		reachable = 0;
	ଅନ୍ୟଥା
		reachable = 1;

	strbuf_release(&rev);
	ଫେରନ୍ତୁ reachable;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା fetch_in_submodule(ଅଚଳ ଅକ୍ଷର *module_path, ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth, ପୂର୍ଣ୍ଣସଂଖ୍ୟା quiet,
			      ଅଚଳ ସଂରଚନା object_id *oid)
{
	ସଂରଚନା child_process cp = CHILD_PROCESS_INIT;

	prepare_submodule_repo_env(&cp.env);
	cp.git_cmd = 1;
	cp.dir = module_path;

	strvec_ଠେଲ(&cp.args, "fetch");
	ଯଦି (quiet)
		strvec_ଠେଲ(&cp.args, "--quiet");
	ଯଦି (depth)
		strvec_pushf(&cp.args, "--depth=%d", depth);
	ଯଦି (oid) {
		ଅକ୍ଷର *hex = oid_to_hex(oid);
		ଅକ୍ଷର *remote;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା code;

		code = get_default_remote_submodule(module_path, &remote);
		ଯଦି (code) {
			child_process_clear(&cp);
			ଫେରନ୍ତୁ code;
		}

		strvec_pushl(&cp.args, remote, hex, ଖାଲି);
		ମୁକ୍ତ(remote);
	}

	ଫେରନ୍ତୁ run_command(&cp);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା run_update_command(ଅଚଳ ସଂରଚନା update_data *ud, ପୂର୍ଣ୍ଣସଂଖ୍ୟା subforce)
{
	ସଂରଚନା child_process cp = CHILD_PROCESS_INIT;
	ଅକ୍ଷର *oid = oid_to_hex(&ud->oid);
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	ବାଛ (ud->update_strategy.type) {
	ସ୍ଥିତି SM_UPDATE_CHECKOUT:
		cp.git_cmd = 1;
		strvec_pushl(&cp.args, "checkout", "-q", ଖାଲି);
		ଯଦି (subforce)
			strvec_ଠେଲ(&cp.args, "-f");
		ଭାଙ୍ଗ;
	ସ୍ଥିତି SM_UPDATE_REBASE:
		cp.git_cmd = 1;
		strvec_ଠେଲ(&cp.args, "rebase");
		ଯଦି (ud->quiet)
			strvec_ଠେଲ(&cp.args, "--quiet");
		ଭାଙ୍ଗ;
	ସ୍ଥିତି SM_UPDATE_MERGE:
		cp.git_cmd = 1;
		strvec_ଠେଲ(&cp.args, "merge");
		ଯଦି (ud->quiet)
			strvec_ଠେଲ(&cp.args, "--quiet");
		ଭାଙ୍ଗ;
	ସ୍ଥିତି SM_UPDATE_COMMAND:
		cp.use_shell = 1;
		strvec_ଠେଲ(&cp.args, ud->update_strategy.command);
		ଭାଙ୍ଗ;
	ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
		BUG("unexpected update strategy type: %d",
		    ud->update_strategy.type);
	}
	strvec_ଠେଲ(&cp.args, oid);

	cp.dir = ud->sm_path;
	prepare_submodule_repo_env(&cp.env);
	ଯଦି ((ret = run_command(&cp))) {
		ବାଛ (ud->update_strategy.type) {
		ସ୍ଥିତି SM_UPDATE_CHECKOUT:
			die_message(_("Unable to checkout '%s' in submodule path '%s'"),
				    oid, ud->displaypath);
			/* No "ret" assignment, use "git checkout"'s */
			ଭାଙ୍ଗ;
		ସ୍ଥିତି SM_UPDATE_REBASE:
			ret = die_message(_("Unable to rebase '%s' in submodule path '%s'"),
					  oid, ud->displaypath);
			ଭାଙ୍ଗ;
		ସ୍ଥିତି SM_UPDATE_MERGE:
			ret = die_message(_("Unable to merge '%s' in submodule path '%s'"),
					  oid, ud->displaypath);
			ଭାଙ୍ଗ;
		ସ୍ଥିତି SM_UPDATE_COMMAND:
			ret = die_message(_("Execution of '%s %s' failed in submodule path '%s'"),
					  ud->update_strategy.command, oid, ud->displaypath);
			ଭାଙ୍ଗ;
		ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
			BUG("unexpected update strategy type: %d",
			    ud->update_strategy.type);
		}

		ଫେରନ୍ତୁ ret;
	}

	ଯଦି (ud->quiet)
		ଫେରନ୍ତୁ 0;

	ବାଛ (ud->update_strategy.type) {
	ସ୍ଥିତି SM_UPDATE_CHECKOUT:
		printf(_("Submodule path '%s': checked out '%s'\n"),
		       ud->displaypath, oid);
		ଭାଙ୍ଗ;
	ସ୍ଥିତି SM_UPDATE_REBASE:
		printf(_("Submodule path '%s': rebased into '%s'\n"),
		       ud->displaypath, oid);
		ଭାଙ୍ଗ;
	ସ୍ଥିତି SM_UPDATE_MERGE:
		printf(_("Submodule path '%s': merged in '%s'\n"),
		       ud->displaypath, oid);
		ଭାଙ୍ଗ;
	ସ୍ଥିତି SM_UPDATE_COMMAND:
		printf(_("Submodule path '%s': '%s %s'\n"),
		       ud->displaypath, ud->update_strategy.command, oid);
		ଭାଙ୍ଗ;
	ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
		BUG("unexpected update strategy type: %d",
		    ud->update_strategy.type);
	}

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା run_update_procedure(ଅଚଳ ସଂରଚନା update_data *ud)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା subforce = is_null_oid(&ud->suboid) || ud->force;

	ଯଦି (!ud->nofetch) {
		/*
		 * Run fetch only if `oid` isn't present or it
		 * is not reachable from a ref.
		 */
		ଯଦି (!is_tip_reachable(ud->sm_path, &ud->oid) &&
		    fetch_in_submodule(ud->sm_path, ud->depth, ud->quiet, ଖାଲି) &&
		    !ud->quiet)
			fprintf_ln(stderr,
				   _("Unable to fetch in submodule path '%s'; "
				     "trying to directly fetch %s:"),
				   ud->displaypath, oid_to_hex(&ud->oid));
		/*
		 * Now we tried the usual fetch, but `oid` may
		 * not be reachable from any of the refs.
		 */
		ଯଦି (!is_tip_reachable(ud->sm_path, &ud->oid) &&
		    fetch_in_submodule(ud->sm_path, ud->depth, ud->quiet, &ud->oid))
			ଫେରନ୍ତୁ die_message(_("Fetched in submodule path '%s', but it did not "
					     "contain %s. Direct fetching of that commit failed."),
					   ud->displaypath, oid_to_hex(&ud->oid));
	}

	ଫେରନ୍ତୁ run_update_command(ud, subforce);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା remote_submodule_branch(ଅଚଳ ଅକ୍ଷର *path, ଅଚଳ ଅକ୍ଷର **branch)
{
	ଅଚଳ ସଂରଚନା submodule *sub;
	ଅକ୍ଷର *ଚାବି;
	*branch = ଖାଲି;

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);
	ଯଦି (!sub)
		ଫେରନ୍ତୁ die_message(_("could not initialize submodule at path '%s'"),
				   path);

	ଚାବି = xstrfmt("submodule.%s.branch", sub->name);
	ଯଦି (repo_config_get_string_tmp(the_repository, ଚାବି, branch))
		*branch = sub->branch;
	ମୁକ୍ତ(ଚାବି);

	ଯଦି (!*branch) {
		*branch = "HEAD";
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (!strcmp(*branch, ".")) {
		ଅଚଳ ଅକ୍ଷର *refname = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
							      "HEAD", 0, ଖାଲି,
							      ଖାଲି);

		ଯଦି (!refname)
			ଫେରନ୍ତୁ die_message(_("No such ref: %s"), "HEAD");

		/* detached HEAD */
		ଯଦି (!strcmp(refname, "HEAD"))
			ଫେରନ୍ତୁ die_message(_("Submodule (%s) branch configured to inherit "
					     "branch from superproject, but the superproject "
					     "is not on any branch"), sub->name);

		ଯଦି (!skip_prefix(refname, "refs/heads/", &refname))
			ଫେରନ୍ତୁ die_message(_("Expecting a full ref name, got %s"),
					   refname);

		*branch = refname;
		ଫେରନ୍ତୁ 0;
	}

	/* Our "branch" is coming from repo_config_get_string_tmp() */
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା ensure_core_worktree(ଅଚଳ ଅକ୍ଷର *path)
{
	ଅଚଳ ଅକ୍ଷର *cw;
	ସଂରଚନା repository subrepo;

	ଯଦି (repo_submodule_init(&subrepo, the_repository, path, null_oid(the_hash_algo)))
		ଫେରନ୍ତୁ die_message(_("could not get a repository handle for submodule '%s'"),
				   path);

	ଯଦି (!repo_config_get_string_tmp(&subrepo, "core.worktree", &cw)) {
		ଅକ୍ଷର *cfg_file, *abs_path;
		ଅଚଳ ଅକ୍ଷର *rel_path;
		ସଂରଚନା strbuf sb = STRBUF_INIT;

		cfg_file = repo_git_path(&subrepo, "config");

		abs_path = absolute_pathdup(path);
		rel_path = relative_path(abs_path, subrepo.gitdir, &sb);

		repo_config_set_in_file(the_repository, cfg_file, "core.worktree", rel_path);

		ମୁକ୍ତ(cfg_file);
		ମୁକ୍ତ(abs_path);
		strbuf_release(&sb);
	}

	repo_clear(&subrepo);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *submodule_update_type_to_label(ଗଣନା submodule_update_type type)
{
	ବାଛ (type) {
	ସ୍ଥିତି SM_UPDATE_CHECKOUT:
		ଫେରନ୍ତୁ "checkout";
	ସ୍ଥିତି SM_UPDATE_MERGE:
		ଫେରନ୍ତୁ "merge";
	ସ୍ଥିତି SM_UPDATE_REBASE:
		ଫେରନ୍ତୁ "rebase";
	ସ୍ଥିତି SM_UPDATE_UNSPECIFIED:
	ସ୍ଥିତି SM_UPDATE_NONE:
	ସ୍ଥିତି SM_UPDATE_COMMAND:
		ଭାଙ୍ଗ;
	}
	BUG("unreachable with type %d", type);
}

ସ୍ଥିର ଶୂନ୍ୟ update_data_to_args(ଅଚଳ ସଂରଚନା update_data *update_data,
				ସଂରଚନା strvec *args)
{
	ଗଣନା submodule_update_type update_type = update_data->update_default;

	strvec_pushl(args, "submodule--helper", "update", "--recursive", ଖାଲି);
	ଯଦି (update_data->displaypath)
		strvec_pushf(args, "--super-prefix=%s/",
			     update_data->displaypath);
	strvec_pushf(args, "--jobs=%d", update_data->max_jobs);
	ଯଦି (update_data->quiet)
		strvec_ଠେଲ(args, "--quiet");
	ଯଦି (update_data->force)
		strvec_ଠେଲ(args, "--force");
	ଯଦି (update_data->init)
		strvec_ଠେଲ(args, "--init");
	ଯଦି (update_data->remote)
		strvec_ଠେଲ(args, "--remote");
	ଯଦି (update_data->nofetch)
		strvec_ଠେଲ(args, "--no-fetch");
	ଯଦି (update_data->dissociate)
		strvec_ଠେଲ(args, "--dissociate");
	ଯଦି (update_data->progress)
		strvec_ଠେଲ(args, "--progress");
	ଯଦି (update_data->require_init)
		strvec_ଠେଲ(args, "--require-init");
	ଯଦି (update_data->depth)
		strvec_pushf(args, "--depth=%d", update_data->depth);
	ଯଦି (update_type != SM_UPDATE_UNSPECIFIED)
		strvec_pushf(args, "--%s",
			     submodule_update_type_to_label(update_type));

	ଯଦି (update_data->references.nr) {
		ସଂରଚନା string_list_item *item;

		for_each_string_list_item(item, &update_data->references)
			strvec_pushl(args, "--reference", item->string, ଖାଲି);
	}
	ଯଦି (update_data->ref_storage_format != REF_STORAGE_FORMAT_UNKNOWN)
		strvec_pushf(args, "--ref-storage-format=%s",
			     ref_storage_format_to_name(update_data->ref_storage_format));
	ଯଦି (update_data->filter_options && update_data->filter_options->choice)
		strvec_pushf(args, "--filter=%s",
				expand_list_objects_filter_spec(
					update_data->filter_options));
	ଯଦି (update_data->recommend_shallow == 0)
		strvec_ଠେଲ(args, "--no-recommend-shallow");
	ଅନ୍ୟଥା ଯଦି (update_data->recommend_shallow == 1)
		strvec_ଠେଲ(args, "--recommend-shallow");
	ଯଦି (update_data->single_branch >= 0)
		strvec_ଠେଲ(args, update_data->single_branch ?
				    "--single-branch" :
				    "--no-single-branch");
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା update_submodule(ସଂରଚନା update_data *update_data)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	ଯଦି (validate_submodule_path(update_data->sm_path) < 0)
		ଫେରନ୍ତୁ -1;

	ret = determine_submodule_update_strategy(the_repository,
						  update_data->just_cloned,
						  update_data->sm_path,
						  update_data->update_default,
						  &update_data->update_strategy);
	ଯଦି (ret)
		ଫେରନ୍ତୁ ret;

	ଯଦି (update_data->just_cloned)
		oidcpy(&update_data->suboid, null_oid(the_hash_algo));
	ଅନ୍ୟଥା ଯଦି (repo_resolve_gitlink_ref(the_repository, update_data->sm_path,
					  "HEAD", &update_data->suboid))
		ଫେରନ୍ତୁ die_message(_("Unable to find current revision in submodule path '%s'"),
				   update_data->displaypath);

	ଯଦି (update_data->remote) {
		ଅକ୍ଷର *remote_name;
		ଅଚଳ ଅକ୍ଷର *branch;
		ଅକ୍ଷର *remote_ref;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା code;

		code = get_default_remote_submodule(update_data->sm_path, &remote_name);
		ଯଦି (code)
			ଫେରନ୍ତୁ code;
		code = remote_submodule_branch(update_data->sm_path, &branch);
		ଯଦି (code) {
			ମୁକ୍ତ(remote_name);
			ଫେରନ୍ତୁ code;
		}
		remote_ref = xstrfmt("refs/remotes/%s/%s", remote_name, branch);

		ମୁକ୍ତ(remote_name);

		ଯଦି (!update_data->nofetch) {
			ଯଦି (fetch_in_submodule(update_data->sm_path, update_data->depth,
					      0, ଖାଲି)) {
				ମୁକ୍ତ(remote_ref);
				ଫେରନ୍ତୁ die_message(_("Unable to fetch in submodule path '%s'"),
						   update_data->sm_path);
			}
		}

		ଯଦି (repo_resolve_gitlink_ref(the_repository, update_data->sm_path,
					     remote_ref, &update_data->oid)) {
			ret = die_message(_("Unable to find %s revision in submodule path '%s'"),
					  remote_ref, update_data->sm_path);
			ମୁକ୍ତ(remote_ref);
			ଫେରନ୍ତୁ ret;
		}

		ମୁକ୍ତ(remote_ref);
	}

	ଯଦି (!oideq(&update_data->oid, &update_data->suboid) || update_data->force) {
		ret = run_update_procedure(update_data);
		ଯଦି (ret)
			ଫେରନ୍ତୁ ret;
	}

	ଯଦି (update_data->recursive) {
		ସଂରଚନା child_process cp = CHILD_PROCESS_INIT;
		ସଂରଚନା update_data next = *update_data;

		next.prefix = ଖାଲି;
		oidcpy(&next.oid, null_oid(the_hash_algo));
		oidcpy(&next.suboid, null_oid(the_hash_algo));

		cp.dir = update_data->sm_path;
		cp.git_cmd = 1;
		prepare_submodule_repo_env(&cp.env);
		update_data_to_args(&next, &cp.args);

		ret = run_command(&cp);
		ଯଦି (ret)
			die_message(_("Failed to recurse into submodule path '%s'"),
				    update_data->displaypath);
		ଫେରନ୍ତୁ ret;
	}

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା update_submodules(ସଂରଚନା update_data *update_data)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, ret = 0;
	ସଂରଚନା submodule_update_clone suc = SUBMODULE_UPDATE_CLONE_INIT;
	ଅଚଳ ସଂରଚନା run_process_parallel_opts opts = {
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
	ଯଦି (suc.quickstop) {
		ret = 1;
		ଯାଅ cleanup;
	}

	ପାଇଁ (i = 0; i < suc.update_clone_nr; i++) {
		ସଂରଚନା update_clone_data ucd = suc.update_clone[i];
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା code = 128;

		oidcpy(&update_data->oid, &ucd.oid);
		update_data->just_cloned = ucd.just_cloned;
		update_data->sm_path = ucd.sub->path;

		/*
		 * Verify that the submodule path does not contain any
		 * symlinks; if it does, it might have been tampered with.
		 * TODO: allow exempting it via
		 * `safe.submodule.path` or something
		 */
		ଯଦି (validate_submodule_path(update_data->sm_path) < 0)
			ଯାଅ fail;

		code = ensure_core_worktree(update_data->sm_path);
		ଯଦି (code)
			ଯାଅ fail;

		update_data->displaypath = get_submodule_displaypath(
			update_data->sm_path, update_data->prefix,
			update_data->super_prefix);
		code = update_submodule(update_data);
		FREE_AND_NULL(update_data->displaypath);
fail:
		ଯଦି (!code)
			ଜାରି;
		ret = code;
		ଯଦି (ret == 128)
			ଯାଅ cleanup;
	}

cleanup:
	submodule_update_clone_release(&suc);
	string_list_clear(&update_data->references, 0);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା module_update(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
			 ସଂରଚନା repository *repo UNUSED)
{
	ସଂରଚନା pathspec pathspec = { 0 };
	ସଂରଚନା pathspec pathspec2 = { 0 };
	ସଂରଚନା update_data opt = UPDATE_DATA_INIT;
	ସଂରଚନା list_objects_filter_options filter_options =
		LIST_OBJECTS_FILTER_INIT;
	ଅଚଳ ଅକ୍ଷର *ref_storage_format = ଖାଲି;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
	ସଂରଚନା option module_update_options[] = {
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
	ଅଚଳ ଅକ୍ଷର *ଅଚଳ git_submodule_helper_usage[] = {
		N_("git submodule [--quiet] update"
		" [--init [--filter=<filter-spec>]] [--remote]"
		" [-N|--no-fetch] [-f|--force]"
		" [--checkout|--merge|--rebase]"
		" [--[no-]recommend-shallow] [--reference <repository>]"
		" [--recursive] [--[no-]single-branch] [--] [<path>...]"),
		ଖାଲି
	};

	update_clone_config_from_gitmodules(the_repository, &opt.max_jobs);
	repo_config(the_repository, git_update_clone_config, &opt.max_jobs);

	argc = parse_options(argc, argv, prefix, module_update_options,
			     git_submodule_helper_usage, 0);

	ଯଦି (opt.require_init)
		opt.init = 1;

	ଯଦି (filter_options.choice && !opt.init) {
		usage_with_options(git_submodule_helper_usage,
				   module_update_options);
	}

	ଯଦି (ref_storage_format) {
		opt.ref_storage_format = ref_storage_format_by_name(ref_storage_format);
		ଯଦି (opt.ref_storage_format == REF_STORAGE_FORMAT_UNKNOWN)
			die(_("unknown ref storage format '%s'"), ref_storage_format);
	}

	opt.filter_options = &filter_options;
	opt.prefix = prefix;

	ଯଦି (opt.update_default)
		opt.update_strategy.type = opt.update_default;

	ଯଦି (module_list_compute(argv, prefix, &pathspec, &opt.list) < 0) {
		ret = 1;
		ଯାଅ cleanup;
	}

	ଯଦି (pathspec.nr)
		opt.warn_if_uninitialized = 1;

	ଯଦି (opt.init) {
		ସଂରଚନା module_list list = MODULE_LIST_INIT;
		ସଂରଚନା init_cb info = INIT_CB_INIT;

		ଯଦି (module_list_compute(argv, opt.prefix,
					&pathspec2, &list) < 0) {
			module_list_release(&list);
			ret = 1;
			ଯାଅ cleanup;
		}

		/*
		 * If there are no path args and submodule.active is set then,
		 * by default, only initialize 'active' modules.
		 */
		ଯଦି (!argc && !repo_config_get(the_repository, "submodule.active"))
			module_list_active(&list);

		info.prefix = opt.prefix;
		info.super_prefix = opt.super_prefix;
		ଯଦି (opt.quiet)
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
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଠେଲ_check(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix UNUSED,
		      ସଂରଚନା repository *repo UNUSED)
{
	ସଂରଚନା remote *remote;
	ଅଚଳ ଅକ୍ଷର *superproject_head;
	ଅକ୍ଷର *head;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା detached_head = 0;
	ସଂରଚନା object_id head_oid;

	ଯଦି (argc < 3)
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
				   0, &head_oid, ଖାଲି);
	ଯଦି (!head)
		die(_("Failed to resolve HEAD as a valid ref."));
	ଯଦି (!strcmp(head, "HEAD"))
		detached_head = 1;

	/*
	 * The remote must be configured.
	 * This is to avoid pushing to the exact same URL as the parent.
	 */
	remote = pushremote_get(argv[1]);
	ଯଦି (!remote || remote->origin == REMOTE_UNCONFIGURED)
		die("remote '%s' not configured", argv[1]);

	/* Check the refspec */
	ଯଦି (argc > 2) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
		ସଂରଚନା ref *local_refs = get_local_heads();
		ସଂରଚନା refspec refspec = REFSPEC_INIT_ଠେଲ(the_hash_algo);

		refspec_appendn(&refspec, argv + 2, argc - 2);

		ପାଇଁ (i = 0; i < refspec.nr; i++) {
			ଅଚଳ ସଂରଚନା refspec_item *rs = &refspec.ବସ୍ତୁଗୁଡ଼ିକ[i];

			ଯଦି (rs->pattern || rs->matching)
				ଜାରି;

			/* LHS must match a single ref */
			ବାଛ (ଗଣନା_refspec_match(rs->src, local_refs, ଖାଲି)) {
			ସ୍ଥିତି 1:
				ଭାଙ୍ଗ;
			ସ୍ଥିତି 0:
				/*
				 * If LHS matches 'HEAD' then we need to ensure
				 * that it matches the same named branch
				 * checked out in the superproject.
				 */
				ଯଦି (!strcmp(rs->src, "HEAD")) {
					ଯଦି (!detached_head &&
					    !strcmp(head, superproject_head))
						ଭାଙ୍ଗ;
					die("HEAD does not match the named branch in the superproject");
				}
				/* fallthrough */
			ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
				die("src refspec '%s' must name a ref",
				    rs->src);
			}
		}

		refspec_clear(&refspec);
		free_refs(local_refs);
	}
	ମୁକ୍ତ(head);

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା absorb_git_dirs(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
			   ସଂରଚନା repository *repo UNUSED)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
	ସଂରଚନା pathspec pathspec = { 0 };
	ସଂରଚନା module_list list = MODULE_LIST_INIT;
	ଅଚଳ ଅକ୍ଷର *super_prefix = ଖାଲି;
	ସଂରଚନା option embed_gitdir_options[] = {
		OPT__SUPER_PREFIX(&super_prefix),
		OPT_END()
	};
	ଅଚଳ ଅକ୍ଷର *ଅଚଳ git_submodule_helper_usage[] = {
		N_("git submodule absorbgitdirs [<options>] [<path>...]"),
		ଖାଲି
	};
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 1;

	argc = parse_options(argc, argv, prefix, embed_gitdir_options,
			     git_submodule_helper_usage, 0);

	ଯଦି (module_list_compute(argv, prefix, &pathspec, &list) < 0)
		ଯାଅ cleanup;

	ପାଇଁ (i = 0; i < list.nr; i++)
		absorb_git_dir_into_superproject(list.entries[i]->name,
						 super_prefix);

	ret = 0;
cleanup:
	clear_pathspec(&pathspec);
	module_list_release(&list);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା module_set_url(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
			  ସଂରଚନା repository *repo UNUSED)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା quiet = 0, ret;
	ଅଚଳ ଅକ୍ଷର *newurl;
	ଅଚଳ ଅକ୍ଷର *path;
	ଅକ୍ଷର *config_name;
	ସଂରଚନା option options[] = {
		OPT__QUIET(&quiet, N_("suppress output for setting url of a submodule")),
		OPT_END()
	};
	ଅଚଳ ଅକ୍ଷର *ଅଚଳ usage[] = {
		N_("git submodule set-url [--quiet] <path> <newurl>"),
		ଖାଲି
	};
	ଅଚଳ ସଂରଚନା submodule *sub;

	argc = parse_options(argc, argv, prefix, options, usage, 0);

	ଯଦି (argc != 2 || !(path = argv[0]) || !(newurl = argv[1]))
		usage_with_options(usage, options);

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);

	ଯଦି (!sub)
		die(_("no submodule mapping found in .gitmodules for path '%s'"),
		    path);

	config_name = xstrfmt("submodule.%s.url", sub->name);
	ret = config_set_in_gitmodules_file_gently(the_repository, config_name, newurl);

	ଯଦି (!ret) {
		repo_read_gitmodules(the_repository, 0);
		sync_submodule(sub->path, prefix, ଖାଲି, quiet ? OPT_QUIET : 0);
	}

	ମୁକ୍ତ(config_name);
	ଫେରନ୍ତୁ !!ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା module_set_branch(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
			     ସଂରଚନା repository *repo UNUSED)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା opt_default = 0, ret;
	ଅଚଳ ଅକ୍ଷର *opt_branch = ଖାଲି;
	ଅଚଳ ଅକ୍ଷର *path;
	ଅକ୍ଷର *config_name;
	ସଂରଚନା option options[] = {
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
	ଅଚଳ ଅକ୍ଷର *ଅଚଳ usage[] = {
		N_("git submodule set-branch [-q|--quiet] (-d|--default) <path>"),
		N_("git submodule set-branch [-q|--quiet] (-b|--branch) <branch> <path>"),
		ଖାଲି
	};
	ଅଚଳ ସଂରଚନା submodule *sub;

	argc = parse_options(argc, argv, prefix, options, usage, 0);

	ଯଦି (!opt_branch && !opt_default)
		die(_("--branch or --default required"));

	ଯଦି (opt_branch && opt_default)
		die(_("options '%s' and '%s' cannot be used together"), "--branch", "--default");

	ଯଦି (argc != 1 || !(path = argv[0]))
		usage_with_options(usage, options);

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);

	ଯଦି (!sub)
		die(_("no submodule mapping found in .gitmodules for path '%s'"),
		    path);

	config_name = xstrfmt("submodule.%s.branch", sub->name);
	ret = config_set_in_gitmodules_file_gently(the_repository, config_name, opt_branch);

	ମୁକ୍ତ(config_name);
	ଫେରନ୍ତୁ !!ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା module_create_branch(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
				ସଂରଚନା repository *repo UNUSED)
{
	ଗଣନା branch_track track;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା quiet = 0, force = 0, reflog = 0, dry_run = 0;
	ସଂରଚନା option options[] = {
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
	ଅଚଳ ଅକ୍ଷର *ଅଚଳ usage[] = {
		N_("git submodule--helper create-branch [-f|--force] [--create-reflog] [-q|--quiet] [-t|--track] [-n|--dry-run] <name> <start-oid> <start-name>"),
		ଖାଲି
	};
	ସଂରଚନା repo_config_ମୂଲ୍ୟଗୁଡ଼ିକ *cfg = repo_config_ମୂଲ୍ୟଗୁଡ଼ିକ(the_repository);

	repo_config(the_repository, git_default_config, ଖାଲି);
	track = cfg->branch_track;
	argc = parse_options(argc, argv, prefix, options, usage, 0);

	ଯଦି (argc != 3)
		usage_with_options(usage, options);

	ଯଦି (!quiet && !dry_run)
		printf_ln(_("creating branch '%s'"), argv[0]);

	create_branches_recursively(the_repository, argv[0], argv[1], argv[2],
				    force, reflog, quiet, track, dry_run);
	ଫେରନ୍ତୁ 0;
}

ସଂରଚନା add_data {
	ଅଚଳ ଅକ୍ଷର *prefix;
	ଅଚଳ ଅକ୍ଷର *branch;
	ଅଚଳ ଅକ୍ଷର *reference_path;
	ଅକ୍ଷର *sm_path;
	ଅଚଳ ଅକ୍ଷର *sm_name;
	ଅଚଳ ଅକ୍ଷର *repo;
	ଅଚଳ ଅକ୍ଷର *realrepo;
	ଗଣନା ref_storage_format ref_storage_format;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା force: 1;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା quiet: 1;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା progress: 1;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା dissociate: 1;
};
#define ADD_DATA_INIT { \
	.depth = -1, \
	.ref_storage_format = REF_STORAGE_FORMAT_UNKNOWN, \
}

ସ୍ଥିର ଶୂନ୍ୟ append_fetch_remotes(ସଂରଚନା strbuf *ସନ୍ଦେଶ, ଅଚଳ ଅକ୍ଷର *git_dir_path)
{
	ସଂରଚନା child_process cp_remote = CHILD_PROCESS_INIT;
	ସଂରଚନା strbuf sb_remote_out = STRBUF_INIT;

	cp_remote.git_cmd = 1;
	strvec_pushf(&cp_remote.env,
		     "GIT_DIR=%s", git_dir_path);
	strvec_ଠେଲ(&cp_remote.env, "GIT_WORK_TREE=.");
	strvec_pushl(&cp_remote.args, "remote", "-v", ଖାଲି);
	ଯଦି (!capture_command(&cp_remote, &sb_remote_out, 0)) {
		ଅକ୍ଷର *next_ଧାଡ଼ି;
		ଅକ୍ଷର *ଧାଡ଼ି = sb_remote_out.buf;

		ଯେତେବେଳେ ((next_ଧାଡ଼ି = strchr(ଧାଡ଼ି, '\n')) != ଖାଲି) {
			size_t len = next_ଧାଡ଼ି - ଧାଡ଼ି;

			ଯଦି (strip_suffix_mem(ଧାଡ଼ି, &len, " (fetch)"))
				strbuf_addf(ସନ୍ଦେଶ, "  %.*s\n", (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)len, ଧାଡ଼ି);
			ଧାଡ଼ି = next_ଧାଡ଼ି + 1;
		}
	}

	strbuf_release(&sb_remote_out);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା add_submodule(ଅଚଳ ସଂରଚନା add_data *add_data)
{
	ସଂରଚନା module_clone_data clone_data = MODULE_CLONE_DATA_INIT;
	ସଂରଚନା string_list reference = STRING_LIST_INIT_NODUP;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = -1;

	/* perhaps the path already exists and is already a git repo, else clone it */
	ଯଦି (is_directory(add_data->sm_path)) {
		ଅକ୍ଷର *submod_gitdir_path;
		ସଂରଚନା strbuf sm_path = STRBUF_INIT;
		strbuf_addstr(&sm_path, add_data->sm_path);
		submod_gitdir_path = xstrfmt("%s/.git", add_data->sm_path);
		ଯଦି (is_nonbare_repository_dir(&sm_path))
			printf(_("Adding existing repo at '%s' to the index\n"),
			       add_data->sm_path);
		ଅନ୍ୟଥା
			die(_("'%s' already exists and is not a valid git repo"),
			    add_data->sm_path);
		strbuf_release(&sm_path);
		ମୁକ୍ତ(submod_gitdir_path);
	} ଅନ୍ୟଥା {
		ସଂରଚନା child_process cp = CHILD_PROCESS_INIT;
		ସଂରଚନା strbuf submod_gitdir = STRBUF_INIT;

		submodule_name_to_gitdir(&submod_gitdir, the_repository, add_data->sm_name);

		ଯଦି (is_directory(submod_gitdir.buf)) {
			ଯଦି (!add_data->force) {
				ସଂରଚନା strbuf ସନ୍ଦେଶ = STRBUF_INIT;
				ଅକ୍ଷର *die_ସନ୍ଦେଶ;

				strbuf_addf(&ସନ୍ଦେଶ, _("A git directory for '%s' is found "
						    "locally with remote(s):\n"),
					    add_data->sm_name);

				append_fetch_remotes(&ସନ୍ଦେଶ, submod_gitdir.buf);
				strbuf_release(&submod_gitdir);

				strbuf_addf(&ସନ୍ଦେଶ, _("If you want to reuse this local git "
						    "directory instead of cloning again from\n"
						    "  %s\n"
						    "use the '--force' option. If the local git "
						    "directory is not the correct repo\n"
						    "or you are unsure what this means choose "
						    "another name with the '--name' option."),
					    add_data->realrepo);

				die_ସନ୍ଦେଶ = strbuf_detach(&ସନ୍ଦେଶ, ଖାଲି);
				die("%s", die_ସନ୍ଦେଶ);
			} ଅନ୍ୟଥା {
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
		ଯଦି (add_data->reference_path) {
			ଅକ୍ଷର *p = xstrdup(add_data->reference_path);

			string_list_append(&reference, p)->util = p;
		}
		clone_data.ref_storage_format = add_data->ref_storage_format;
		clone_data.dissociate = add_data->dissociate;
		ଯଦି (add_data->depth >= 0)
			clone_data.depth = add_data->depth;

		ଯଦି (clone_submodule(&clone_data, &reference))
			ଯାଅ cleanup;

		prepare_submodule_repo_env(&cp.env);
		cp.git_cmd = 1;
		cp.dir = add_data->sm_path;
		/*
		 * NOTE: we only get here if add_data->force is true, so
		 * passing --force to checkout is reasonable.
		 */
		strvec_pushl(&cp.args, "checkout", "-f", "-q", ଖାଲି);

		ଯଦି (add_data->branch) {
			strvec_pushl(&cp.args, "-B", add_data->branch, ଖାଲି);
			strvec_pushf(&cp.args, "origin/%s", add_data->branch);
		}

		ଯଦି (run_command(&cp))
			die(_("unable to checkout submodule '%s'"), add_data->sm_path);
	}
	ret = 0;

cleanup:
	string_list_clear(&reference, 1);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା config_submodule_in_gitmodules(ଅଚଳ ଅକ୍ଷର *name, ଅଚଳ ଅକ୍ଷର *var, ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ)
{
	ଅକ୍ଷର *ଚାବି;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	ଯଦି (!is_writing_gitmodules_ok())
		die(_("please make sure that the .gitmodules file is in the working tree"));

	ଚାବି = xstrfmt("submodule.%s.%s", name, var);
	ret = config_set_in_gitmodules_file_gently(the_repository, ଚାବି, ମୂଲ୍ୟ);
	ମୁକ୍ତ(ଚାବି);

	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ଶୂନ୍ୟ configure_added_submodule(ସଂରଚନା add_data *add_data)
{
	ଅକ୍ଷର *ଚାବି;
	ସଂରଚନା child_process add_submod = CHILD_PROCESS_INIT;
	ସଂରଚନା child_process add_gitmodules = CHILD_PROCESS_INIT;
	ଅଚଳ ସଂରଚନା string_list *ମୂଲ୍ୟଗୁଡ଼ିକ;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା matched = 0;

	ଚାବି = xstrfmt("submodule.%s.url", add_data->sm_name);
	repo_config_set_gently(the_repository, ଚାବି, add_data->realrepo);
	ମୁକ୍ତ(ଚାବି);

	add_submod.git_cmd = 1;
	strvec_pushl(&add_submod.args, "add",
		     "--no-warn-embedded-repo", ଖାଲି);
	ଯଦି (add_data->force)
		strvec_ଠେଲ(&add_submod.args, "--force");
	strvec_pushl(&add_submod.args, "--", add_data->sm_path, ଖାଲି);

	ଯଦି (run_command(&add_submod))
		die(_("Failed to add submodule '%s'"), add_data->sm_path);

	ଯଦି (config_submodule_in_gitmodules(add_data->sm_name, "path", add_data->sm_path) ||
	    config_submodule_in_gitmodules(add_data->sm_name, "url", add_data->repo))
		die(_("Failed to register submodule '%s'"), add_data->sm_path);

	ଯଦି (add_data->branch) {
		ଯଦି (config_submodule_in_gitmodules(add_data->sm_name,
						   "branch", add_data->branch))
			die(_("Failed to register submodule '%s'"), add_data->sm_path);
	}

	add_gitmodules.git_cmd = 1;
	strvec_pushl(&add_gitmodules.args,
		     "add", "--force", "--", ".gitmodules", ଖାଲି);

	ଯଦି (run_command(&add_gitmodules))
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
	ଯଦି (repo_config_get(the_repository, "submodule.active") || /* key absent */
	    repo_config_get_string_multi(the_repository, "submodule.active", &ମୂଲ୍ୟଗୁଡ଼ିକ)) {
		/*
		 * If the submodule being added isn't already covered by the
		 * current configured pathspec, set the submodule's active flag
		 */
		ଚାବି = xstrfmt("submodule.%s.active", add_data->sm_name);
		repo_config_set_gently(the_repository, ଚାବି, "true");
		ମୁକ୍ତ(ଚାବି);
	} ଅନ୍ୟଥା {
		ପାଇଁ (size_t i = 0; i < ମୂଲ୍ୟଗୁଡ଼ିକ->nr; i++) {
			ଅଚଳ ଅକ୍ଷର *pat = ମୂଲ୍ୟଗୁଡ଼ିକ->ବସ୍ତୁଗୁଡ଼ିକ[i].string;
			ଯଦି (!wildmatch(pat, add_data->sm_path, 0)) { /* match found */
				matched = 1;
				ଭାଙ୍ଗ;
			}
		}
		ଯଦି (!matched) { /* no pattern matched -> force-enable */
			ଚାବି = xstrfmt("submodule.%s.active", add_data->sm_name);
			repo_config_set_gently(the_repository, ଚାବି, "true");
			ମୁକ୍ତ(ଚାବି);
		}
	}
}

ସ୍ଥିର ଶୂନ୍ୟ die_on_index_match(ଅଚଳ ଅକ୍ଷର *path, ପୂର୍ଣ୍ଣସଂଖ୍ୟା force)
{
	ସଂରଚନା pathspec ps;
	ଅଚଳ ଅକ୍ଷର *args[] = { path, ଖାଲି };
	parse_pathspec(&ps, 0, PATHSPEC_PREFER_CWD, ଖାଲି, args);

	ଯଦି (repo_read_index_preload(the_repository, ଖାଲି, 0) < 0)
		die(_("index file corrupt"));

	ଯଦି (ps.nr) {
		ଅକ୍ଷର *ps_matched = xcalloc(ps.nr, 1);

		/* TODO: audit for interaction with sparse-index. */
		ensure_full_index(the_repository->index);

		/*
		 * Since there is only one pathspec, we just need to
		 * check ps_matched[0] to know if a cache entry matched.
		 */
		ପାଇଁ (size_t i = 0; i < the_repository->index->cache_nr; i++) {
			ce_path_match(the_repository->index, the_repository->index->cache[i], &ps,
				      ps_matched);

			ଯଦି (ps_matched[0]) {
				ଯଦି (!force)
					die(_("'%s' already exists in the index"),
					    path);
				ଯଦି (!S_ISGITLINK(the_repository->index->cache[i]->ce_mode))
					die(_("'%s' already exists in the index "
					      "and is not a submodule"), path);
				ଭାଙ୍ଗ;
			}
		}
		ମୁକ୍ତ(ps_matched);
	}
	clear_pathspec(&ps);
}

ସ୍ଥିର ଶୂନ୍ୟ die_on_repo_without_commits(ଅଚଳ ଅକ୍ଷର *path)
{
	ସଂରଚନା strbuf sb = STRBUF_INIT;
	strbuf_addstr(&sb, path);
	ଯଦି (is_nonbare_repository_dir(&sb)) {
		ସଂରଚନା object_id oid;
		ଯଦି (repo_resolve_gitlink_ref(the_repository, path, "HEAD", &oid) < 0)
			die(_("'%s' does not have a commit checked out"), path);
	}
	strbuf_release(&sb);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା module_add(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
		      ସଂରଚନା repository *repo UNUSED)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା force = 0, quiet = 0, progress = 0, dissociate = 0;
	ସଂରଚନା add_data add_data = ADD_DATA_INIT;
	ଅଚଳ ଅକ୍ଷର *ref_storage_format = ଖାଲି;
	ଅକ୍ଷର *to_free = ଖାଲି;
	ଅଚଳ ସଂରଚନା submodule *existing;
	ସଂରଚନା strbuf buf = STRBUF_INIT;
	ଅକ୍ଷର *sm_name_to_free = ଖାଲି;
	ସଂରଚନା option options[] = {
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
	ଅଚଳ ଅକ୍ଷର *ଅଚଳ usage[] = {
		N_("git submodule add [<options>] [--] <repository> [<path>]"),
		ଖାଲି
	};
	ସଂରଚନା strbuf sb = STRBUF_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 1;

	argc = parse_options(argc, argv, prefix, options, usage, 0);

	ଯଦି (!is_writing_gitmodules_ok())
		die(_("please make sure that the .gitmodules file is in the working tree"));

	ଯଦି (prefix && *prefix &&
	    add_data.reference_path && !is_absolute_path(add_data.reference_path))
		add_data.reference_path = xstrfmt("%s%s", prefix, add_data.reference_path);

	ଯଦି (argc == 0 || argc > 2)
		usage_with_options(usage, options);

	ଯଦି (ref_storage_format) {
		add_data.ref_storage_format = ref_storage_format_by_name(ref_storage_format);
		ଯଦି (add_data.ref_storage_format == REF_STORAGE_FORMAT_UNKNOWN)
			die(_("unknown ref storage format '%s'"), ref_storage_format);
	}

	add_data.repo = argv[0];
	ଯଦି (argc == 1)
		add_data.sm_path = git_url_basename(add_data.repo, 0, 0);
	ଅନ୍ୟଥା
		add_data.sm_path = xstrdup(argv[1]);

	ଯଦି (prefix && *prefix && !is_absolute_path(add_data.sm_path)) {
		ଅକ୍ଷର *sm_path = add_data.sm_path;

		add_data.sm_path = xstrfmt("%s%s", prefix, sm_path);
		ମୁକ୍ତ(sm_path);
	}

	ଯଦି (starts_with_dot_dot_slash(add_data.repo) ||
	    starts_with_dot_slash(add_data.repo)) {
		ଯଦି (prefix)
			die(_("Relative path can only be used from the toplevel "
			      "of the working tree"));

		/* dereference source url relative to parent's url */
		to_free = resolve_relative_url(add_data.repo, ଖାଲି, 1);
		add_data.realrepo = to_free;
	} ଅନ୍ୟଥା ଯଦି (is_dir_sep(add_data.repo[0]) || strchr(add_data.repo, ':')) {
		add_data.realrepo = add_data.repo;
	} ଅନ୍ୟଥା {
		die(_("repo URL: '%s' must be absolute or begin with ./|../"),
		    add_data.repo);
	}

	/*
	 * normalize path:
	 * multiple //; leading ./; /./; /../;
	 */
	normalize_path_copy(add_data.sm_path, add_data.sm_path);
	strip_dir_trailing_slashes(add_data.sm_path);

	ଯଦି (validate_submodule_path(add_data.sm_path) < 0)
		die(ଖାଲି);

	die_on_index_match(add_data.sm_path, force);
	die_on_repo_without_commits(add_data.sm_path);

	ଯଦି (!force) {
		ସଂରଚନା child_process cp = CHILD_PROCESS_INIT;

		cp.git_cmd = 1;
		cp.no_stdout = 1;
		strvec_pushl(&cp.args, "add", "--dry-run", "--ignore-missing",
			     "--no-warn-embedded-repo", add_data.sm_path, ଖାଲି);
		ଯଦି ((ret = pipe_command(&cp, ଖାଲି, 0, ଖାଲି, 0, &sb, 0))) {
			strbuf_complete_ଧାଡ଼ି(&sb);
			fputs(sb.buf, stderr);
			ଯାଅ cleanup;
		}
	}

	ଯଦି (!add_data.sm_name)
		add_data.sm_name = add_data.sm_path;

	existing = submodule_from_name(the_repository,
					null_oid(the_hash_algo),
					add_data.sm_name);

	ଯଦି (existing && existing->path &&
	    strcmp(existing->path, add_data.sm_path)) {
		ଯଦି (!force) {
			die(_("submodule name '%s' already used for path '%s'"),
			    add_data.sm_name, existing->path);
		}
		/* --force: build <name><n> until unique */
		ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 1; ; i++) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "%s%d", add_data.sm_name, i);
			ଯଦି (!submodule_from_name(the_repository,
						null_oid(the_hash_algo),
						buf.buf)) {
				ଭାଙ୍ଗ;
			}
		}
		add_data.sm_name = sm_name_to_free = strbuf_detach(&buf, ଖାଲି);
	}

	ଯଦି (check_submodule_name(add_data.sm_name))
		die(_("'%s' is not a valid submodule name"), add_data.sm_name);

	add_data.prefix = prefix;
	add_data.force = !!force;
	add_data.quiet = !!quiet;
	add_data.progress = !!progress;
	add_data.dissociate = !!dissociate;

	ଯଦି (the_repository->repository_format_submodule_path_cfg)
		create_default_gitdir_config(add_data.sm_name);

	ଯଦି (add_submodule(&add_data))
		ଯାଅ cleanup;
	configure_added_submodule(&add_data);

	ret = 0;
cleanup:
	ମୁକ୍ତ(sm_name_to_free);
	ମୁକ୍ତ(add_data.sm_path);
	ମୁକ୍ତ(to_free);
	strbuf_release(&sb);

	ଫେରନ୍ତୁ ret;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_submodule__helper(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
			  ଅଚଳ ଅକ୍ଷର **argv,
			  ଅଚଳ ଅକ୍ଷର *prefix,
			  ସଂରଚନା repository *repo)
{
	parse_opt_subcommand_fn *fn = ଖାଲି;
	ଅଚଳ ଅକ୍ଷର *ଅଚଳ usage[] = {
		N_("git submodule--helper <command>"),
		ଖାଲି
	};
	ସଂରଚନା option options[] = {
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
		OPT_SUBCOMMAND("push-check", &fn, ଠେଲ_check),
		OPT_SUBCOMMAND("absorbgitdirs", &fn, absorb_git_dirs),
		OPT_SUBCOMMAND("set-url", &fn, module_set_url),
		OPT_SUBCOMMAND("set-branch", &fn, module_set_branch),
		OPT_SUBCOMMAND("create-branch", &fn, module_create_branch),
		OPT_SUBCOMMAND("get-default-remote", &fn, module_get_default_remote),
		OPT_END()
	};
	argc = parse_options(argc, argv, prefix, options, usage, 0);

	ଫେରନ୍ତୁ fn(argc, argv, prefix, repo);
}
