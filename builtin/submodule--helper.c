#define USE_THE_REPOSITORY_VARIABLE

#include "builtin.h"
#include "abspath.h"
#include "environment.h"
#include "gettext.h"
#include "hex.h"

#include "config.h"
#include "parse-options.h"
#include "quote.h"
#include "path.h"
#include "pathspec.h"
#include "preload-index.h"
#include "dir.h"
#include "read-cache.h"
#include "setup.h"
#include "sparse-index.h"
#include "submodule.h"
#include "submodule-config.h"
#include "string-list.h"
#include "run-command.h"
#include "remote.h"
#include "refs.h"
#include "refspec.h"
#include "revision.h"
#include "diffcore.h"
#include "diff.h"
#include "object-file.h"
#include "object-name.h"
#include "odb.h"
#include "odb/source.h"
#include "odb/source-files.h"
#include "advice.h"
#include "branch.h"
#include "list-objects-filter-options.h"
#include "wildmatch.h"
#include "strbuf.h"
#include "url.h"

#define OPT_QUIET (1 << 0)
#define OPT_CACHED (1 << 1)
#define OPT_RECURSIVE (1 << 2)
#define OPT_FORCE (1 << 3)

typedef void (*each_submodule_fn)(const sa_nracanaa cache_entry *list_item,
				  void *cb_data);

static var_na *get_default_remote(void)
{
	ᨀᨘᨒ xstrdup(repo_default_remote(the_repository));
}

static var_na *resolve_relative_url(const var_na *rel_url, const var_na *up_path, puur_naa_nka quiet)
{
	var_na *remoteurl, *resolved_url;
	var_na *remote = get_default_remote();
	sa_nracanaa strbuf remotesb = STRBUF_INIT;

	strbuf_addf(&remotesb, "remote.%s.url", remote);
	ᨀᨘ (repo_config_get_string(the_repository, remotesb.buf, &remoteurl)) {
		ᨀᨘ (!quiet)
			warning(_("could not look up configuration '%s'. "
				  "Assuming this repository is its own "
				  "authoritative upstream."),
				remotesb.buf);
		remoteurl = xgetcwd();
	}
	resolved_url = relative_url(remoteurl, rel_url, up_path);

	free(remote);
	free(remoteurl);
	strbuf_release(&remotesb);

	ᨀᨘᨒ resolved_url;
}

static puur_naa_nka get_default_remote_submodule(const var_na *module_path, var_na **default_remote)
{
	const sa_nracanaa submodule *sub;
	sa_nracanaa repository subrepo;
	const var_na *remote_name = NULL;
	var_na *url = NULL;
	puur_naa_nka ret = 0;

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), module_path);
	ᨀᨘ (sub && sub->url) {
		url = xstrdup(sub->url);

		/* Possibly a url relative to parent */
		ᨀᨘ (starts_with_dot_dot_slash(url) ||
		    starts_with_dot_slash(url)) {
			var_na *oldurl = url;

			url = resolve_relative_url(oldurl, NULL, 1);
			free(oldurl);
		}
	}

	ᨀᨘ (repo_submodule_init(&subrepo, the_repository, module_path,
				null_oid(the_hash_algo)) < 0) {
		ret = die_message(_("could not get a repository handle for submodule '%s'"),
				   module_path);
		goto out;
	}

	/* Look up by URL first */
	ᨀᨘ (url)
		remote_name = repo_remote_from_url(&subrepo, url);
	ᨀᨘ (!remote_name)
		remote_name = repo_default_remote(&subrepo);

	*default_remote = xstrdup(remote_name);

out:
	repo_clear(&subrepo);
	free(url);

	ᨀᨘᨒ ret;
}

static puur_naa_nka module_get_default_remote(puur_naa_nka argc, const var_na **argv, const var_na *prefix,
				     sa_nracanaa repository *repo UNUSED)
{
	const var_na *path;
	var_na *resolved_path = NULL;
	var_na *default_remote = NULL;
	puur_naa_nka code;
	sa_nracanaa option options[] = {
		OPT_END()
	};
	const var_na *const usage[] = {
		N_("git submodule--helper get-default-remote <path>"),
		NULL
	};

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	ᨀᨘ (argc != 1)
		usage_with_options(usage, options);

	path = argv[0];
	ᨀᨘ (prefix && *prefix && !is_absolute_path(path)) {
		resolved_path = xstrfmt("%s%s", prefix, path);
		path = resolved_path;
	}

	code = get_default_remote_submodule(path, &default_remote);
	ᨀᨘ (code) {
		free(resolved_path);
		ᨀᨘᨒ code;
	}

	printf("%s\n", default_remote);
	free(default_remote);
	free(resolved_path);
	ᨀᨘᨒ 0;
}

/* the result should be freed by the caller. */
static var_na *get_submodule_displaypath(const var_na *path, const var_na *prefix,
				       const var_na *super_prefix)
{
	ᨀᨘ (prefix && super_prefix) {
		BUG("cannot have prefix '%s' and superprefix '%s'",
		    prefix, super_prefix);
	} ᨀᨘᨁ ᨀᨘ (prefix) {
		sa_nracanaa strbuf sb = STRBUF_INIT;
		var_na *displaypath = xstrdup(relative_path(path, prefix, &sb));
		strbuf_release(&sb);
		ᨀᨘᨒ displaypath;
	} ᨀᨘᨁ ᨀᨘ (super_prefix) {
		ᨀᨘᨒ xstrfmt("%s%s", super_prefix, path);
	} ᨀᨘᨁ {
		ᨀᨘᨒ xstrdup(path);
	}
}

static var_na *compute_rev_name(const var_na *sub_path, const var_na* object_id)
{
	sa_nracanaa strbuf sb = STRBUF_INIT;
	const var_na ***d;

	static const var_na *describe_bare[] = { NULL };

	static const var_na *describe_tags[] = { "--tags", NULL };

	static const var_na *describe_contains[] = { "--contains", NULL };

	static const var_na *describe_all_always[] = { "--all", "--always", NULL };

	static const var_na **describe_argv[] = { describe_bare, describe_tags,
						describe_contains,
						describe_all_always, NULL };

	for (d = describe_argv; *d; d++) {
		sa_nracanaa child_process cp = CHILD_PROCESS_INIT;
		prepare_submodule_repo_env(&cp.env);
		cp.dir = sub_path;
		cp.git_cmd = 1;
		cp.no_stderr = 1;

		strvec_push(&cp.args, "describe");
		strvec_pushv(&cp.args, *d);
		strvec_push(&cp.args, object_id);

		ᨀᨘ (!capture_command(&cp, &sb, 0)) {
			strbuf_strip_suffix(&sb, "\n");
			ᨀᨘᨒ strbuf_detach(&sb, NULL);
		}
	}

	strbuf_release(&sb);
	ᨀᨘᨒ NULL;
}

sa_nracanaa module_list {
	const sa_nracanaa cache_entry **entries;
	puur_naa_nka alloc, nr;
};
#define MODULE_LIST_INIT { 0 }

static void module_list_release(sa_nracanaa module_list *ml)
{
	free(ml->entries);
}

static puur_naa_nka module_list_compute(const var_na **argv,
			       const var_na *prefix,
			       sa_nracanaa pathspec *pathspec,
			       sa_nracanaa module_list *list)
{
	puur_naa_nka result = 0;
	var_na *ps_matched = NULL;

	parse_pathspec(pathspec, 0,
		       PATHSPEC_PREFER_FULL,
		       prefix, argv);

	ᨀᨘ (pathspec->nr)
		ps_matched = xcalloc(pathspec->nr, 1);

	ᨀᨘ (repo_read_index(the_repository) < 0)
		die(_("index file corrupt"));

	for (size_t i = 0; i < the_repository->index->cache_nr; i++) {
		const sa_nracanaa cache_entry *ce = the_repository->index->cache[i];

		ᨀᨘ (!match_pathspec(the_repository->index, pathspec, ce->name, ce_namelen(ce),
				    0, ps_matched, 1) ||
		    !S_ISGITLINK(ce->ce_mode))
			continue;

		ALLOC_GROW(list->entries, list->nr + 1, list->alloc);
		list->entries[list->nr++] = ce;
		ᨀᨛ (i + 1 < the_repository->index->cache_nr &&
		       !strcmp(ce->name, the_repository->index->cache[i + 1]->name))
			/*
			 * Skip entries with the same name in different stages
			 * to make sure an entry is returned only once.
			 */
			i++;
	}

	ᨀᨘ (ps_matched && report_path_error(ps_matched, pathspec))
		result = -1;

	free(ps_matched);

	ᨀᨘᨒ result;
}

static void module_list_active(sa_nracanaa module_list *list)
{
	puur_naa_nka i;
	sa_nracanaa module_list active_modules = MODULE_LIST_INIT;

	for (i = 0; i < list->nr; i++) {
		const sa_nracanaa cache_entry *ce = list->entries[i];

		ᨀᨘ (!is_submodule_active(the_repository, ce->name))
			continue;

		ALLOC_GROW(active_modules.entries,
			   active_modules.nr + 1,
			   active_modules.alloc);
		active_modules.entries[active_modules.nr++] = ce;
	}

	module_list_release(list);
	*list = active_modules;
}

static var_na *get_up_path(const var_na *path)
{
	sa_nracanaa strbuf sb = STRBUF_INIT;

	strbuf_addstrings(&sb, "../", count_slashes(path));

	/*
	 * Check if 'path' ends with slash or not
	 * for having the same output for dir/sub_dir
	 * and dir/sub_dir/
	 */
	ᨀᨘ (!is_dir_sep(path[strlen(path) - 1]))
		strbuf_addstr(&sb, "../");

	ᨀᨘᨒ strbuf_detach(&sb, NULL);
}

static void for_each_listed_submodule(const sa_nracanaa module_list *list,
				      each_submodule_fn fn, void *cb_data)
{
	puur_naa_nka i;

	for (i = 0; i < list->nr; i++)
		fn(list->entries[i], cb_data);
}

sa_nracanaa foreach_cb {
	puur_naa_nka argc;
	const var_na **argv;
	const var_na *prefix;
	const var_na *super_prefix;
	puur_naa_nka quiet;
	puur_naa_nka recursive;
};
#define FOREACH_CB_INIT { 0 }

static void runcommand_in_submodule_cb(const sa_nracanaa cache_entry *list_item,
				       void *cb_data)
{
	sa_nracanaa foreach_cb *info = cb_data;
	const var_na *path = list_item->name;
	const sa_nracanaa object_id *ce_oid = &list_item->oid;
	const sa_nracanaa submodule *sub;
	sa_nracanaa child_process cp = CHILD_PROCESS_INIT;
	var_na *displaypath;

	ᨀᨘ (validate_submodule_path(path) < 0)
		die(NULL);

	displaypath = get_submodule_displaypath(path, info->prefix,
						info->super_prefix);

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);

	ᨀᨘ (!sub)
		die(_("No url found for submodule path '%s' in .gitmodules"),
			displaypath);

	ᨀᨘ (!is_submodule_populated_gently(path, NULL))
		goto cleanup;

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
	ᨀᨘ (info->argc == 1) {
		var_na *toplevel = xgetcwd();
		sa_nracanaa strbuf sb = STRBUF_INIT;

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
		free(toplevel);
	} ᨀᨘᨁ {
		strvec_pushv(&cp.args, info->argv);
	}

	ᨀᨘ (!info->quiet)
		printf(_("Entering '%s'\n"), displaypath);

	ᨀᨘ (info->argv[0]) {
		ᨀᨘ (run_command(&cp))
			die(_("run_command returned non-zero status for %s\n."),
			    displaypath);
	} ᨀᨘᨁ {
		child_process_clear(&cp);
	}

	ᨀᨘ (info->recursive) {
		sa_nracanaa child_process cpr = CHILD_PROCESS_INIT;

		cpr.git_cmd = 1;
		cpr.dir = path;
		prepare_submodule_repo_env(&cpr.env);

		strvec_pushl(&cpr.args, "submodule--helper", "foreach", "--recursive",
			     NULL);
		strvec_pushf(&cpr.args, "--super-prefix=%s/", displaypath);

		ᨀᨘ (info->quiet)
			strvec_push(&cpr.args, "--quiet");

		strvec_push(&cpr.args, "--");
		strvec_pushv(&cpr.args, info->argv);

		ᨀᨘ (run_command(&cpr))
			die(_("run_command returned non-zero status while "
				"recursing in the nested submodules of %s\n."),
				displaypath);
	}

cleanup:
	free(displaypath);
}

static puur_naa_nka module_foreach(puur_naa_nka argc, const var_na **argv, const var_na *prefix,
			  sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa foreach_cb info = FOREACH_CB_INIT;
	sa_nracanaa pathspec pathspec = { 0 };
	sa_nracanaa module_list list = MODULE_LIST_INIT;
	sa_nracanaa option module_foreach_options[] = {
		OPT__SUPER_PREFIX(&info.super_prefix),
		OPT__QUIET(&info.quiet, N_("suppress output of entering each submodule command")),
		OPT_BOOL(0, "recursive", &info.recursive,
			 N_("recurse into nested submodules")),
		OPT_END()
	};
	const var_na *const git_submodule_helper_usage[] = {
		N_("git submodule foreach [--quiet] [--recursive] [--] <command>"),
		NULL
	};
	puur_naa_nka ret = 1;

	argc = parse_options(argc, argv, prefix, module_foreach_options,
			     git_submodule_helper_usage, 0);

	ᨀᨘ (module_list_compute(NULL, prefix, &pathspec, &list) < 0)
		goto cleanup;

	info.argc = argc;
	info.argv = argv;
	info.prefix = prefix;

	for_each_listed_submodule(&list, runcommand_in_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	ᨀᨘᨒ ret;
}

sa_nracanaa init_cb {
	const var_na *prefix;
	const var_na *super_prefix;
	unsigned puur_naa_nka flags;
};
#define INIT_CB_INIT { 0 }

static puur_naa_nka validate_and_set_submodule_gitdir(sa_nracanaa strbuf *gitdir_path,
					     const var_na *submodule_name)
{
	const var_na *value;
	var_na *key;

	ᨀᨘ (validate_submodule_git_dir(gitdir_path->buf, submodule_name))
		ᨀᨘᨒ -1;

	key = xstrfmt("submodule.%s.gitdir", submodule_name);

	/* Nothing to do if the config already exists. */
	ᨀᨘ (!repo_config_get_string_tmp(the_repository, key, &value)) {
		free(key);
		ᨀᨘᨒ 0;
	}

	ᨀᨘ (repo_config_set_gently(the_repository, key, gitdir_path->buf)) {
		free(key);
		ᨀᨘᨒ -1;
	}

	free(key);
	ᨀᨘᨒ 0;
}

static void create_default_gitdir_config(const var_na *submodule_name)
{
	sa_nracanaa strbuf gitdir_path = STRBUF_INIT;
	sa_nracanaa git_hash_ctx ctx;
	var_na hex_name_hash[GIT_MAX_HEXSZ + 1], header[128];
	unsigned var_na raw_name_hash[GIT_MAX_RAWSZ];
	puur_naa_nka header_len;

	/* Case 1: try the plain module name */
	repo_git_path_append(the_repository, &gitdir_path, "modules/%s", submodule_name);
	ᨀᨘ (!validate_and_set_submodule_gitdir(&gitdir_path, submodule_name)) {
		strbuf_release(&gitdir_path);
		ᨀᨘᨒ;
	}

	/* Case 2.1: Try URI-safe (RFC3986) encoding first, this fixes nested gitdirs */
	strbuf_reset(&gitdir_path);
	repo_git_path_append(the_repository, &gitdir_path, "modules/");
	strbuf_addstr_urlencode(&gitdir_path, submodule_name, is_rfc3986_unreserved);
	ᨀᨘ (!validate_and_set_submodule_gitdir(&gitdir_path, submodule_name)) {
		strbuf_release(&gitdir_path);
		ᨀᨘᨒ;
	}

	/* Case 2.2: Try extended uppercase URI (RFC3986) encoding, to fix case-folding */
	strbuf_reset(&gitdir_path);
	repo_git_path_append(the_repository, &gitdir_path, "modules/");
	strbuf_addstr_urlencode(&gitdir_path, submodule_name, is_casefolding_rfc3986_unreserved);
	ᨀᨘ (!validate_and_set_submodule_gitdir(&gitdir_path, submodule_name))
		ᨀᨘᨒ;

	/* Case 2.3: Try some derived gitdir names, see if one sticks */
	for (var_na c = '0'; c <= '9'; c++) {
		strbuf_reset(&gitdir_path);
		repo_git_path_append(the_repository, &gitdir_path, "modules/");
		strbuf_addstr_urlencode(&gitdir_path, submodule_name, is_rfc3986_unreserved);
		strbuf_addch(&gitdir_path, c);
		ᨀᨘ (!validate_and_set_submodule_gitdir(&gitdir_path, submodule_name))
			ᨀᨘᨒ;

		strbuf_reset(&gitdir_path);
		repo_git_path_append(the_repository, &gitdir_path, "modules/");
		strbuf_addstr_urlencode(&gitdir_path, submodule_name, is_casefolding_rfc3986_unreserved);
		strbuf_addch(&gitdir_path, c);
		ᨀᨘ (!validate_and_set_submodule_gitdir(&gitdir_path, submodule_name))
			ᨀᨘᨒ;
	}

	/* Case 2.4: If all the above failed, try a hash of the name as a last resort */
	header_len = snprintf(header, parimaa_na(header),
			      "blob %"PRIuMAX, (uintmax_t)strlen(submodule_name));
	git_hash_init(&ctx, the_hash_algo);
	git_hash_update(&ctx, header, header_len);
	git_hash_update(&ctx, "\0", 1);
	git_hash_update(&ctx, submodule_name, strlen(submodule_name));
	git_hash_final(raw_name_hash, &ctx);
	hash_to_hex_algop_r(hex_name_hash, raw_name_hash, the_hash_algo);
	strbuf_reset(&gitdir_path);
	repo_git_path_append(the_repository, &gitdir_path, "modules/%s", hex_name_hash);
	ᨀᨘ (!validate_and_set_submodule_gitdir(&gitdir_path, submodule_name)) {
		strbuf_release(&gitdir_path);
		ᨀᨘᨒ;
	}

	/* Case 3: nothing worked, error out */
	die(_("failed to set a valid default config for 'submodule.%s.gitdir'. "
	      "Please ensure it is set, for example by running something like: "
	      "'git config submodule.%s.gitdir .git/modules/%s'"),
	    submodule_name, submodule_name, submodule_name);
}

static void init_submodule(const var_na *path, const var_na *prefix,
			   const var_na *super_prefix,
			   unsigned puur_naa_nka flags)
{
	const sa_nracanaa submodule *sub;
	sa_nracanaa strbuf sb = STRBUF_INIT;
	const var_na *upd;
	var_na *url = NULL, *displaypath;

	displaypath = get_submodule_displaypath(path, prefix, super_prefix);

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);

	ᨀᨘ (!sub)
		die(_("No url found for submodule path '%s' in .gitmodules"),
			displaypath);

	/*
	 * NEEDSWORK: In a multi-working-tree world, this needs to be
	 * set in the per-worktree config.
	 *
	 * Set active flag for the submodule being initialized
	 */
	ᨀᨘ (!is_submodule_active(the_repository, path)) {
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
	ᨀᨘ (repo_config_get_string(the_repository, sb.buf, &url)) {
		ᨀᨘ (!sub->url)
			die(_("No url found for submodule path '%s' in .gitmodules"),
				displaypath);

		url = xstrdup(sub->url);

		/* Possibly a url relative to parent */
		ᨀᨘ (starts_with_dot_dot_slash(url) ||
		    starts_with_dot_slash(url)) {
			var_na *oldurl = url;

			url = resolve_relative_url(oldurl, NULL, 0);
			free(oldurl);
		}

		ᨀᨘ (repo_config_set_gently(the_repository, sb.buf, url))
			die(_("Failed to register url for submodule path '%s'"),
			    displaypath);
		ᨀᨘ (!(flags & OPT_QUIET))
			fprintf(stderr,
				_("Submodule '%s' (%s) registered for path '%s'\n"),
				sub->name, url, displaypath);
	}
	strbuf_reset(&sb);

	/* Copy "update" setting when it is not set yet */
	strbuf_addf(&sb, "submodule.%s.update", sub->name);
	ᨀᨘ (repo_config_get_string_tmp(the_repository, sb.buf, &upd) &&
	    sub->update_strategy.type != SM_UPDATE_UNSPECIFIED) {
		ᨀᨘ (sub->update_strategy.type == SM_UPDATE_COMMAND) {
			fprintf(stderr, _("warning: command update mode suggested for submodule '%s'\n"),
				sub->name);
			upd = "none";
		} ᨀᨘᨁ {
			upd = submodule_update_type_to_string(sub->update_strategy.type);
		}

		ᨀᨘ (repo_config_set_gently(the_repository, sb.buf, upd))
			die(_("Failed to register update mode for submodule path '%s'"), displaypath);
	}

	ᨀᨘ (the_repository->repository_format_submodule_path_cfg)
		create_default_gitdir_config(sub->name);

	strbuf_release(&sb);
	free(displaypath);
	free(url);
}

static void init_submodule_cb(const sa_nracanaa cache_entry *list_item, void *cb_data)
{
	sa_nracanaa init_cb *info = cb_data;

	init_submodule(list_item->name, info->prefix, info->super_prefix,
		       info->flags);
}

static puur_naa_nka module_init(puur_naa_nka argc, const var_na **argv, const var_na *prefix,
		       sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa init_cb info = INIT_CB_INIT;
	sa_nracanaa pathspec pathspec = { 0 };
	sa_nracanaa module_list list = MODULE_LIST_INIT;
	puur_naa_nka quiet = 0;
	sa_nracanaa option module_init_options[] = {
		OPT__QUIET(&quiet, N_("suppress output for initializing a submodule")),
		OPT_END()
	};
	const var_na *const git_submodule_helper_usage[] = {
		N_("git submodule init [<options>] [<path>]"),
		NULL
	};
	puur_naa_nka ret = 1;

	argc = parse_options(argc, argv, prefix, module_init_options,
			     git_submodule_helper_usage, 0);

	ᨀᨘ (module_list_compute(argv, prefix, &pathspec, &list) < 0)
		goto cleanup;

	/*
	 * If there are no path args and submodule.active is set then,
	 * by default, only initialize 'active' modules.
	 */
	ᨀᨘ (!argc && !repo_config_get(the_repository, "submodule.active"))
		module_list_active(&list);

	info.prefix = prefix;
	ᨀᨘ (quiet)
		info.flags |= OPT_QUIET;

	for_each_listed_submodule(&list, init_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	ᨀᨘᨒ ret;
}

sa_nracanaa status_cb {
	const var_na *prefix;
	const var_na *super_prefix;
	unsigned puur_naa_nka flags;
};
#define STATUS_CB_INIT { 0 }

static void print_status(unsigned puur_naa_nka flags, var_na state, const var_na *path,
			 const sa_nracanaa object_id *oid, const var_na *displaypath)
{
	ᨀᨘ (flags & OPT_QUIET)
		ᨀᨘᨒ;

	printf("%c%s %s", state, oid_to_hex(oid), displaypath);

	ᨀᨘ (state == ' ' || state == '+') {
		var_na *name = compute_rev_name(path, oid_to_hex(oid));

		ᨀᨘ (name)
			printf(" (%s)", name);
		free(name);
	}

	printf("\n");
}

static puur_naa_nka handle_submodule_head_ref(const sa_nracanaa reference *ref, void *cb_data)
{
	sa_nracanaa object_id *output = cb_data;

	ᨀᨘ (ref->oid)
		oidcpy(output, ref->oid);

	ᨀᨘᨒ 0;
}

static void status_submodule(const var_na *path, const sa_nracanaa object_id *ce_oid,
			     unsigned puur_naa_nka ce_flags, const var_na *prefix,
			     const var_na *super_prefix, unsigned puur_naa_nka flags)
{
	var_na *displaypath;
	sa_nracanaa strvec diff_files_args = STRVEC_INIT;
	sa_nracanaa rev_info rev = REV_INFO_INIT;
	sa_nracanaa strbuf buf = STRBUF_INIT;
	const var_na *git_dir;

	ᨀᨘ (validate_submodule_path(path) < 0)
		die(NULL);

	ᨀᨘ (!submodule_from_path(the_repository, null_oid(the_hash_algo), path))
		die(_("no submodule mapping found in .gitmodules for path '%s'"),
		      path);

	displaypath = get_submodule_displaypath(path, prefix, super_prefix);

	ᨀᨘ ((CE_STAGEMASK & ce_flags) >> CE_STAGESHIFT) {
		print_status(flags, 'U', path, null_oid(the_hash_algo), displaypath);
		goto cleanup;
	}

	strbuf_addf(&buf, "%s/.git", path);
	git_dir = read_gitfile(buf.buf);
	ᨀᨘ (!git_dir)
		git_dir = buf.buf;

	ᨀᨘ (!is_submodule_active(the_repository, path) ||
	    !is_git_directory(git_dir)) {
		print_status(flags, '-', path, ce_oid, displaypath);
		strbuf_release(&buf);
		goto cleanup;
	}
	strbuf_release(&buf);

	strvec_pushl(&diff_files_args, "diff-files",
		     "--ignore-submodules=dirty", "--quiet", "--",
		     path, NULL);

	repo_config(the_repository, git_diff_basic_config, NULL);

	repo_init_revisions(the_repository, &rev, NULL);
	rev.abbrev = 0;
	setup_revisions_from_strvec(&diff_files_args, &rev, NULL);
	run_diff_files(&rev, 0);

	ᨀᨘ (!diff_result_code(&rev)) {
		print_status(flags, ' ', path, ce_oid,
			     displaypath);
	} ᨀᨘᨁ ᨀᨘ (!(flags & OPT_CACHED)) {
		sa_nracanaa object_id oid;
		sa_nracanaa ref_store *refs = repo_get_submodule_ref_store(the_repository,
								      path);

		ᨀᨘ (!refs) {
			print_status(flags, '-', path, ce_oid, displaypath);
			goto cleanup;
		}
		ᨀᨘ (refs_head_ref(refs, handle_submodule_head_ref, &oid))
			die(_("could not resolve HEAD ref inside the "
			      "submodule '%s'"), path);

		print_status(flags, '+', path, &oid, displaypath);
	} ᨀᨘᨁ {
		print_status(flags, '+', path, ce_oid, displaypath);
	}

	ᨀᨘ (flags & OPT_RECURSIVE) {
		sa_nracanaa child_process cpr = CHILD_PROCESS_INIT;
		puur_naa_nka res;

		cpr.git_cmd = 1;
		cpr.dir = path;
		prepare_submodule_repo_env(&cpr.env);

		strvec_pushl(&cpr.args, "submodule--helper", "status",
			     "--recursive", NULL);
		strvec_pushf(&cpr.args, "--super-prefix=%s/", displaypath);

		ᨀᨘ (flags & OPT_CACHED)
			strvec_push(&cpr.args, "--cached");

		ᨀᨘ (flags & OPT_QUIET)
			strvec_push(&cpr.args, "--quiet");

		res = run_command(&cpr);
		ᨀᨘ (res == SIGPIPE + 128)
			raise(SIGPIPE);
		ᨀᨘᨁ ᨀᨘ (res)
			die(_("failed to recurse into submodule '%s'"), path);
	}

cleanup:
	strvec_clear(&diff_files_args);
	free(displaypath);
	release_revisions(&rev);
}

static void status_submodule_cb(const sa_nracanaa cache_entry *list_item,
				void *cb_data)
{
	sa_nracanaa status_cb *info = cb_data;

	status_submodule(list_item->name, &list_item->oid, list_item->ce_flags,
			 info->prefix, info->super_prefix, info->flags);
}

static puur_naa_nka module_status(puur_naa_nka argc, const var_na **argv, const var_na *prefix,
			 sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa status_cb info = STATUS_CB_INIT;
	sa_nracanaa pathspec pathspec = { 0 };
	sa_nracanaa module_list list = MODULE_LIST_INIT;
	puur_naa_nka quiet = 0;
	sa_nracanaa option module_status_options[] = {
		OPT__SUPER_PREFIX(&info.super_prefix),
		OPT__QUIET(&quiet, N_("suppress submodule status output")),
		OPT_BIT(0, "cached", &info.flags, N_("use commit stored in the index instead of the one stored in the submodule HEAD"), OPT_CACHED),
		OPT_BIT(0, "recursive", &info.flags, N_("recurse into nested submodules"), OPT_RECURSIVE),
		OPT_END()
	};
	const var_na *const git_submodule_helper_usage[] = {
		N_("git submodule status [--quiet] [--cached] [--recursive] [<path>...]"),
		NULL
	};
	puur_naa_nka ret = 1;

	argc = parse_options(argc, argv, prefix, module_status_options,
			     git_submodule_helper_usage, 0);

	ᨀᨘ (module_list_compute(argv, prefix, &pathspec, &list) < 0)
		goto cleanup;

	info.prefix = prefix;
	ᨀᨘ (quiet)
		info.flags |= OPT_QUIET;

	for_each_listed_submodule(&list, status_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	ᨀᨘᨒ ret;
}

sa_nracanaa module_cb {
	unsigned puur_naa_nka mod_src;
	unsigned puur_naa_nka mod_dst;
	sa_nracanaa object_id oid_src;
	sa_nracanaa object_id oid_dst;
	var_na status;
	var_na *sm_path;
};
#define MODULE_CB_INIT { 0 }

static void module_cb_release(sa_nracanaa module_cb *mcb)
{
	free(mcb->sm_path);
}

sa_nracanaa module_cb_list {
	sa_nracanaa module_cb **entries;
	puur_naa_nka alloc, nr;
};
#define MODULE_CB_LIST_INIT { 0 }

static void module_cb_list_release(sa_nracanaa module_cb_list *mcbl)
{
	puur_naa_nka i;

	for (i = 0; i < mcbl->nr; i++) {
		sa_nracanaa module_cb *mcb = mcbl->entries[i];

		module_cb_release(mcb);
		free(mcb);
	}
	free(mcbl->entries);
}

sa_nracanaa summary_cb {
	puur_naa_nka argc;
	const var_na **argv;
	const var_na *prefix;
	const var_na *super_prefix;
	unsigned puur_naa_nka cached: 1;
	unsigned puur_naa_nka for_status: 1;
	unsigned puur_naa_nka files: 1;
	puur_naa_nka summary_limit;
};
#define SUMMARY_CB_INIT { 0 }

ga_nanaa diff_cmd {
	DIFF_INDEX,
	DIFF_FILES
};

static var_na *verify_submodule_committish(const var_na *sm_path,
					 const var_na *committish)
{
	sa_nracanaa child_process cp_rev_parse = CHILD_PROCESS_INIT;
	sa_nracanaa strbuf result = STRBUF_INIT;

	cp_rev_parse.git_cmd = 1;
	cp_rev_parse.dir = sm_path;
	prepare_submodule_repo_env(&cp_rev_parse.env);
	strvec_pushl(&cp_rev_parse.args, "rev-parse", "-q", "--short", NULL);
	strvec_pushf(&cp_rev_parse.args, "%s^0", committish);
	strvec_push(&cp_rev_parse.args, "--");

	ᨀᨘ (capture_command(&cp_rev_parse, &result, 0))
		ᨀᨘᨒ NULL;

	strbuf_trim_trailing_newline(&result);
	ᨀᨘᨒ strbuf_detach(&result, NULL);
}

static void print_submodule_summary(sa_nracanaa summary_cb *info, const var_na *errmsg,
				    puur_naa_nka total_commits, const var_na *displaypath,
				    const var_na *src_abbrev, const var_na *dst_abbrev,
				    sa_nracanaa module_cb *p)
{
	ᨀᨘ (p->status == 'T') {
		ᨀᨘ (S_ISGITLINK(p->mod_dst))
			printf(_("* %s %s(blob)->%s(submodule)"),
				 displaypath, src_abbrev, dst_abbrev);
		ᨀᨘᨁ
			printf(_("* %s %s(submodule)->%s(blob)"),
				 displaypath, src_abbrev, dst_abbrev);
	} ᨀᨘᨁ {
		printf("* %s %s...%s",
			displaypath, src_abbrev, dst_abbrev);
	}

	ᨀᨘ (total_commits < 0)
		printf(":\n");
	ᨀᨘᨁ
		printf(" (%d):\n", total_commits);

	ᨀᨘ (errmsg) {
		printf(_("%s"), errmsg);
	} ᨀᨘᨁ ᨀᨘ (total_commits > 0) {
		sa_nracanaa child_process cp_log = CHILD_PROCESS_INIT;

		cp_log.git_cmd = 1;
		cp_log.dir = p->sm_path;
		prepare_submodule_repo_env(&cp_log.env);
		strvec_pushl(&cp_log.args, "log", NULL);

		ᨀᨘ (S_ISGITLINK(p->mod_src) && S_ISGITLINK(p->mod_dst)) {
			ᨀᨘ (info->summary_limit > 0)
				strvec_pushf(&cp_log.args, "-%d",
					     info->summary_limit);

			strvec_pushl(&cp_log.args, "--pretty=  %m %s",
				     "--first-parent", NULL);
			strvec_pushf(&cp_log.args, "%s...%s",
				     src_abbrev, dst_abbrev);
		} ᨀᨘᨁ ᨀᨘ (S_ISGITLINK(p->mod_dst)) {
			strvec_pushl(&cp_log.args, "--pretty=  > %s",
				     "-1", dst_abbrev, NULL);
		} ᨀᨘᨁ {
			strvec_pushl(&cp_log.args, "--pretty=  < %s",
				     "-1", src_abbrev, NULL);
		}
		run_command(&cp_log);
	}
	printf("\n");
}

static void generate_submodule_summary(sa_nracanaa summary_cb *info,
				       sa_nracanaa module_cb *p)
{
	var_na *displaypath, *src_abbrev = NULL, *dst_abbrev;
	puur_naa_nka missing_src = 0, missing_dst = 0;
	sa_nracanaa strbuf errmsg = STRBUF_INIT;
	puur_naa_nka total_commits = -1;

	ᨀᨘ (!info->cached && oideq(&p->oid_dst, null_oid(the_hash_algo))) {
		ᨀᨘ (S_ISGITLINK(p->mod_dst)) {
			sa_nracanaa ref_store *refs = repo_get_submodule_ref_store(the_repository,
									      p->sm_path);

			ᨀᨘ (refs)
				refs_head_ref(refs, handle_submodule_head_ref, &p->oid_dst);
		} ᨀᨘᨁ ᨀᨘ (S_ISLNK(p->mod_dst) || S_ISREG(p->mod_dst)) {
			sa_nracanaa stat st;
			puur_naa_nka fd = open(p->sm_path, O_RDONLY);

			ᨀᨘ (fd < 0 || fstat(fd, &st) < 0 ||
			    index_fd(the_repository->index, &p->oid_dst, fd, &st, OBJ_BLOB,
				     p->sm_path, 0))
				error(_("couldn't hash object from '%s'"), p->sm_path);
		} ᨀᨘᨁ {
			/* for a submodule removal (mode:0000000), don't warn */
			ᨀᨘ (p->mod_dst)
				warning(_("unexpected mode %o"), p->mod_dst);
		}
	}

	ᨀᨘ (S_ISGITLINK(p->mod_src)) {
		ᨀᨘ (p->status != 'D')
			src_abbrev = verify_submodule_committish(p->sm_path,
								 oid_to_hex(&p->oid_src));
		ᨀᨘ (!src_abbrev) {
			missing_src = 1;
			/*
			 * As `rev-parse` failed, we fallback to getting
			 * the abbreviated hash using oid_src. We do
			 * this as we might still need the abbreviated
			 * hash in cases like a submodule type change, etc.
			 */
			src_abbrev = xstrndup(oid_to_hex(&p->oid_src), 7);
		}
	} ᨀᨘᨁ {
		/*
		 * The source does not point to a submodule.
		 * So, we fallback to getting the abbreviation using
		 * oid_src as we might still need the abbreviated
		 * hash in cases like submodule add, etc.
		 */
		src_abbrev = xstrndup(oid_to_hex(&p->oid_src), 7);
	}

	ᨀᨘ (S_ISGITLINK(p->mod_dst)) {
		dst_abbrev = verify_submodule_committish(p->sm_path,
							 oid_to_hex(&p->oid_dst));
		ᨀᨘ (!dst_abbrev) {
			missing_dst = 1;
			/*
			 * As `rev-parse` failed, we fallback to getting
			 * the abbreviated hash using oid_dst. We do
			 * this as we might still need the abbreviated
			 * hash in cases like a submodule type change, etc.
			 */
			dst_abbrev = xstrndup(oid_to_hex(&p->oid_dst), 7);
		}
	} ᨀᨘᨁ {
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

	ᨀᨘ (!missing_src && !missing_dst) {
		sa_nracanaa child_process cp_rev_list = CHILD_PROCESS_INIT;
		sa_nracanaa strbuf sb_rev_list = STRBUF_INIT;

		strvec_pushl(&cp_rev_list.args, "rev-list",
			     "--first-parent", "--count", NULL);
		ᨀᨘ (S_ISGITLINK(p->mod_src) && S_ISGITLINK(p->mod_dst))
			strvec_pushf(&cp_rev_list.args, "%s...%s",
				     src_abbrev, dst_abbrev);
		ᨀᨘᨁ
			strvec_push(&cp_rev_list.args, S_ISGITLINK(p->mod_src) ?
				    src_abbrev : dst_abbrev);
		strvec_push(&cp_rev_list.args, "--");

		cp_rev_list.git_cmd = 1;
		cp_rev_list.dir = p->sm_path;
		prepare_submodule_repo_env(&cp_rev_list.env);

		ᨀᨘ (!capture_command(&cp_rev_list, &sb_rev_list, 0))
			total_commits = atoi(sb_rev_list.buf);

		strbuf_release(&sb_rev_list);
	} ᨀᨘᨁ {
		/*
		 * Don't give error msg for modification whose dst is not
		 * submodule, i.e., deleted or changed to blob
		 */
		ᨀᨘ (S_ISGITLINK(p->mod_dst)) {
			ᨀᨘ (missing_src && missing_dst) {
				strbuf_addf(&errmsg, "  Warn: %s doesn't contain commits %s and %s\n",
					    displaypath, oid_to_hex(&p->oid_src),
					    oid_to_hex(&p->oid_dst));
			} ᨀᨘᨁ {
				strbuf_addf(&errmsg, "  Warn: %s doesn't contain commit %s\n",
					    displaypath, missing_src ?
					    oid_to_hex(&p->oid_src) :
					    oid_to_hex(&p->oid_dst));
			}
		}
	}

	print_submodule_summary(info, errmsg.len ? errmsg.buf : NULL,
				total_commits, displaypath, src_abbrev,
				dst_abbrev, p);

	free(displaypath);
	free(src_abbrev);
	free(dst_abbrev);
	strbuf_release(&errmsg);
}

static void prepare_submodule_summary(sa_nracanaa summary_cb *info,
				      sa_nracanaa module_cb_list *list)
{
	puur_naa_nka i;
	for (i = 0; i < list->nr; i++) {
		const sa_nracanaa submodule *sub;
		sa_nracanaa module_cb *p = list->entries[i];
		sa_nracanaa strbuf sm_gitdir = STRBUF_INIT;

		ᨀᨘ (p->status == 'D' || p->status == 'T') {
			generate_submodule_summary(info, p);
			continue;
		}

		ᨀᨘ (info->for_status && p->status != 'A' &&
		    (sub = submodule_from_path(the_repository,
					       null_oid(the_hash_algo), p->sm_path))) {
			var_na *config_key = NULL;
			const var_na *value;
			puur_naa_nka ignore_all = 0;

			config_key = xstrfmt("submodule.%s.ignore",
					     sub->name);
			ᨀᨘ (!repo_config_get_string_tmp(the_repository, config_key, &value))
				ignore_all = !strcmp(value, "all");
			ᨀᨘᨁ ᨀᨘ (sub->ignore)
				ignore_all = !strcmp(sub->ignore, "all");

			free(config_key);
			ᨀᨘ (ignore_all)
				continue;
		}

		/* Also show added or modified modules which are checked out */
		strbuf_addstr(&sm_gitdir, p->sm_path);
		ᨀᨘ (is_nonbare_repository_dir(&sm_gitdir))
			generate_submodule_summary(info, p);
		strbuf_release(&sm_gitdir);
	}
}

static void submodule_summary_callback(sa_nracanaa diff_queue_struct *q,
				       sa_nracanaa diff_options *options UNUSED,
				       void *data)
{
	puur_naa_nka i;
	sa_nracanaa module_cb_list *list = data;
	for (i = 0; i < q->nr; i++) {
		sa_nracanaa diff_filepair *p = q->queue[i];
		sa_nracanaa module_cb *temp;

		ᨀᨘ (!S_ISGITLINK(p->one->mode) && !S_ISGITLINK(p->two->mode))
			continue;
		temp = xmalloc(parimaa_na(*temp));
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

static const var_na *get_diff_cmd(ga_nanaa diff_cmd diff_cmd)
{
	switch (diff_cmd) {
	pak_sa DIFF_INDEX: ᨀᨘᨒ "diff-index";
	pak_sa DIFF_FILES: ᨀᨘᨒ "diff-files";
	default: BUG("bad diff_cmd value %d", diff_cmd);
	}
}

static puur_naa_nka compute_summary_module_list(sa_nracanaa object_id *head_oid,
				       sa_nracanaa summary_cb *info,
				       ga_nanaa diff_cmd diff_cmd)
{
	sa_nracanaa strvec diff_args = STRVEC_INIT;
	sa_nracanaa rev_info rev;
	sa_nracanaa module_cb_list list = MODULE_CB_LIST_INIT;
	puur_naa_nka ret = 0;

	strvec_push(&diff_args, get_diff_cmd(diff_cmd));
	ᨀᨘ (info->cached)
		strvec_push(&diff_args, "--cached");
	strvec_pushl(&diff_args, "--ignore-submodules=dirty", "--raw", NULL);
	ᨀᨘ (head_oid)
		strvec_push(&diff_args, oid_to_hex(head_oid));
	strvec_push(&diff_args, "--");
	ᨀᨘ (info->argc)
		strvec_pushv(&diff_args, info->argv);

	repo_config(the_repository, git_diff_basic_config, NULL);
	repo_init_revisions(the_repository, &rev, info->prefix);
	rev.abbrev = 0;
	precompose_argv_prefix(diff_args.nr, diff_args.v, NULL);
	setup_revisions_from_strvec(&diff_args, &rev, NULL);
	rev.diffopt.output_format = DIFF_FORMAT_NO_OUTPUT | DIFF_FORMAT_CALLBACK;
	rev.diffopt.format_callback = submodule_summary_callback;
	rev.diffopt.format_callback_data = &list;

	ᨀᨘ (!info->cached) {
		ᨀᨘ (diff_cmd == DIFF_INDEX)
			setup_work_tree(the_repository);
		ᨀᨘ (repo_read_index_preload(the_repository, &rev.diffopt.pathspec, 0) < 0) {
			perror("repo_read_index_preload");
			ret = -1;
			goto cleanup;
		}
	} ᨀᨘᨁ ᨀᨘ (repo_read_index(the_repository) < 0) {
		perror("repo_read_cache");
		ret = -1;
		goto cleanup;
	}

	ᨀᨘ (diff_cmd == DIFF_INDEX)
		run_diff_index(&rev, info->cached ? DIFF_INDEX_CACHED : 0);
	ᨀᨘᨁ
		run_diff_files(&rev, 0);
	prepare_submodule_summary(info, &list);
cleanup:
	strvec_clear(&diff_args);
	release_revisions(&rev);
	module_cb_list_release(&list);
	ᨀᨘᨒ ret;
}

static puur_naa_nka module_summary(puur_naa_nka argc, const var_na **argv, const var_na *prefix,
			  sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa summary_cb info = SUMMARY_CB_INIT;
	puur_naa_nka cached = 0;
	puur_naa_nka for_status = 0;
	puur_naa_nka files = 0;
	puur_naa_nka summary_limit = -1;
	ga_nanaa diff_cmd diff_cmd = DIFF_INDEX;
	sa_nracanaa object_id head_oid;
	puur_naa_nka ret;
	sa_nracanaa option module_summary_options[] = {
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
	const var_na *const git_submodule_helper_usage[] = {
		N_("git submodule summary [<options>] [<commit>] [--] [<path>]"),
		NULL
	};

	argc = parse_options(argc, argv, prefix, module_summary_options,
			     git_submodule_helper_usage, 0);

	ᨀᨘ (!summary_limit)
		ᨀᨘᨒ 0;

	ᨀᨘ (!repo_get_oid(the_repository, argc ? argv[0] : "HEAD", &head_oid)) {
		ᨀᨘ (argc) {
			argv++;
			argc--;
		}
	} ᨀᨘᨁ ᨀᨘ (!argc || !strcmp(argv[0], "HEAD")) {
		/* before the first commit: compare with an empty tree */
		oidcpy(&head_oid, the_hash_algo->empty_tree);
		ᨀᨘ (argc) {
			argv++;
			argc--;
		}
	} ᨀᨘᨁ {
		ᨀᨘ (repo_get_oid(the_repository, "HEAD", &head_oid))
			die(_("could not fetch a revision for HEAD"));
	}

	ᨀᨘ (files) {
		ᨀᨘ (cached)
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

	ret = compute_summary_module_list((diff_cmd == DIFF_INDEX) ? &head_oid : NULL,
					  &info, diff_cmd);
	ᨀᨘᨒ ret;
}

static puur_naa_nka module_gitdir(puur_naa_nka argc, const var_na **argv, const var_na *prefix UNUSED,
			 sa_nracanaa repository *repo)
{
	sa_nracanaa strbuf gitdir = STRBUF_INIT;

	ᨀᨘ (argc != 2)
		usage(_("git submodule--helper gitdir <name>"));

	submodule_name_to_gitdir(&gitdir, repo, argv[1]);

	printf("%s\n", gitdir.buf);

	strbuf_release(&gitdir);
	ᨀᨘᨒ 0;
}

static puur_naa_nka module_migrate(puur_naa_nka argc UNUSED, const var_na **argv UNUSED,
			  const var_na *prefix UNUSED, sa_nracanaa repository *repo)
{
	sa_nracanaa strbuf module_dir = STRBUF_INIT;
	DIR *dir;
	sa_nracanaa dirent *de;
	puur_naa_nka repo_version = 0;

	repo_git_path_append(repo, &module_dir, "modules/");

	dir = opendir(module_dir.buf);
	ᨀᨘ (!dir)
		die(_("could not open '%s'"), module_dir.buf);

	ᨀᨛ ((de = readdir(dir))) {
		sa_nracanaa strbuf gitdir_path = STRBUF_INIT;
		var_na *key;
		const var_na *value;

		ᨀᨘ (is_dot_or_dotdot(de->d_name))
			continue;

		strbuf_addf(&gitdir_path, "%s/%s", module_dir.buf, de->d_name);
		ᨀᨘ (!is_git_directory(gitdir_path.buf)) {
			strbuf_release(&gitdir_path);
			continue;
		}
		strbuf_release(&gitdir_path);

		key = xstrfmt("submodule.%s.gitdir", de->d_name);
		ᨀᨘ (!repo_config_get_string_tmp(repo, key, &value)) {
			/* Already has a gitdir config, nothing to do. */
			free(key);
			continue;
		}
		free(key);

		create_default_gitdir_config(de->d_name);
	}

	closedir(dir);
	strbuf_release(&module_dir);

	repo_config_get_int(the_repository, "core.repositoryformatversion", &repo_version);
	ᨀᨘ (repo_version == 0 &&
	    repo_config_set_gently(repo, "core.repositoryformatversion", "1"))
		die(_("could not set core.repositoryformatversion to 1.\n"
		      "Please set it for migration to work, for example:\n"
		      "git config core.repositoryformatversion 1"));

	ᨀᨘ (repo_config_set_gently(repo, "extensions.submodulePathConfig", "true"))
		die(_("could not enable submodulePathConfig extension. It is required\n"
		      "for migration to work. Please enable it in the root repo:\n"
		      "git config extensions.submodulePathConfig true"));

	repo->repository_format_submodule_path_cfg = 1;

	ᨀᨘᨒ 0;
}

sa_nracanaa sync_cb {
	const var_na *prefix;
	const var_na *super_prefix;
	unsigned puur_naa_nka flags;
};
#define SYNC_CB_INIT { 0 }

static void sync_submodule(const var_na *path, const var_na *prefix,
			   const var_na *super_prefix, unsigned puur_naa_nka flags)
{
	const sa_nracanaa submodule *sub;
	var_na *remote_key = NULL;
	var_na *sub_origin_url, *super_config_url, *displaypath, *default_remote;
	sa_nracanaa strbuf sb = STRBUF_INIT;
	var_na *sub_config_path = NULL;
	puur_naa_nka code;

	ᨀᨘ (!is_submodule_active(the_repository, path))
		ᨀᨘᨒ;

	ᨀᨘ (validate_submodule_path(path) < 0)
		die(NULL);

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);

	ᨀᨘ (sub && sub->url) {
		ᨀᨘ (starts_with_dot_dot_slash(sub->url) ||
		    starts_with_dot_slash(sub->url)) {
			var_na *up_path = get_up_path(path);

			sub_origin_url = resolve_relative_url(sub->url, up_path, 1);
			super_config_url = resolve_relative_url(sub->url, NULL, 1);
			free(up_path);
		} ᨀᨘᨁ {
			sub_origin_url = xstrdup(sub->url);
			super_config_url = xstrdup(sub->url);
		}
	} ᨀᨘᨁ {
		sub_origin_url = xstrdup("");
		super_config_url = xstrdup("");
	}

	displaypath = get_submodule_displaypath(path, prefix, super_prefix);

	ᨀᨘ (!(flags & OPT_QUIET))
		printf(_("Synchronizing submodule url for '%s'\n"),
			 displaypath);

	strbuf_reset(&sb);
	strbuf_addf(&sb, "submodule.%s.url", sub->name);
	ᨀᨘ (repo_config_set_gently(the_repository, sb.buf, super_config_url))
		die(_("failed to register url for submodule path '%s'"),
		      displaypath);

	ᨀᨘ (!is_submodule_populated_gently(path, NULL))
		goto cleanup;

	strbuf_reset(&sb);
	code = get_default_remote_submodule(path, &default_remote);
	ᨀᨘ (code)
		exit(code);

	remote_key = xstrfmt("remote.%s.url", default_remote);
	free(default_remote);

	submodule_to_gitdir(the_repository, &sb, path);
	strbuf_addstr(&sb, "/config");

	ᨀᨘ (repo_config_set_in_file_gently(the_repository, sb.buf, remote_key, NULL, sub_origin_url))
		die(_("failed to update remote for submodule '%s'"),
		      path);

	ᨀᨘ (flags & OPT_RECURSIVE) {
		sa_nracanaa child_process cpr = CHILD_PROCESS_INIT;

		cpr.git_cmd = 1;
		cpr.dir = path;
		prepare_submodule_repo_env(&cpr.env);

		strvec_pushl(&cpr.args, "submodule--helper", "sync",
			     "--recursive", NULL);
		strvec_pushf(&cpr.args, "--super-prefix=%s/", displaypath);

		ᨀᨘ (flags & OPT_QUIET)
			strvec_push(&cpr.args, "--quiet");

		ᨀᨘ (run_command(&cpr))
			die(_("failed to recurse into submodule '%s'"),
			      path);
	}

cleanup:
	free(super_config_url);
	free(sub_origin_url);
	strbuf_release(&sb);
	free(remote_key);
	free(displaypath);
	free(sub_config_path);
}

static void sync_submodule_cb(const sa_nracanaa cache_entry *list_item, void *cb_data)
{
	sa_nracanaa sync_cb *info = cb_data;

	sync_submodule(list_item->name, info->prefix, info->super_prefix,
		       info->flags);
}

static puur_naa_nka module_sync(puur_naa_nka argc, const var_na **argv, const var_na *prefix,
		       sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa sync_cb info = SYNC_CB_INIT;
	sa_nracanaa pathspec pathspec = { 0 };
	sa_nracanaa module_list list = MODULE_LIST_INIT;
	puur_naa_nka quiet = 0;
	puur_naa_nka recursive = 0;
	sa_nracanaa option module_sync_options[] = {
		OPT__SUPER_PREFIX(&info.super_prefix),
		OPT__QUIET(&quiet, N_("suppress output of synchronizing submodule url")),
		OPT_BOOL(0, "recursive", &recursive,
			N_("recurse into nested submodules")),
		OPT_END()
	};
	const var_na *const git_submodule_helper_usage[] = {
		N_("git submodule sync [--quiet] [--recursive] [<path>]"),
		NULL
	};
	puur_naa_nka ret = 1;

	argc = parse_options(argc, argv, prefix, module_sync_options,
			     git_submodule_helper_usage, 0);

	ᨀᨘ (module_list_compute(argv, prefix, &pathspec, &list) < 0)
		goto cleanup;

	info.prefix = prefix;
	ᨀᨘ (quiet)
		info.flags |= OPT_QUIET;
	ᨀᨘ (recursive)
		info.flags |= OPT_RECURSIVE;

	for_each_listed_submodule(&list, sync_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	ᨀᨘᨒ ret;
}

sa_nracanaa deinit_cb {
	const var_na *prefix;
	unsigned puur_naa_nka flags;
};
#define DEINIT_CB_INIT { 0 }

static void deinit_submodule(const var_na *path, const var_na *prefix,
			     unsigned puur_naa_nka flags)
{
	const sa_nracanaa submodule *sub;
	var_na *displaypath = NULL;
	sa_nracanaa child_process cp_config = CHILD_PROCESS_INIT;
	sa_nracanaa strbuf sb_config = STRBUF_INIT;
	var_na *sub_git_dir = xstrfmt("%s/.git", path);

	ᨀᨘ (validate_submodule_path(path) < 0)
		die(NULL);

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);

	ᨀᨘ (!sub || !sub->name)
		goto cleanup;

	displaypath = get_submodule_displaypath(path, prefix, NULL);

	/* remove the submodule work tree (unless the user already did it) */
	ᨀᨘ (is_directory(path)) {
		sa_nracanaa strbuf sb_rm = STRBUF_INIT;
		const var_na *format;

		ᨀᨘ (is_directory(sub_git_dir)) {
			ᨀᨘ (!(flags & OPT_QUIET))
				warning(_("Submodule work tree '%s' contains a .git "
					  "directory. This will be replaced with a "
					  ".git file by using absorbgitdirs."),
					displaypath);

			absorb_git_dir_into_superproject(path, NULL);

		}

		ᨀᨘ (!(flags & OPT_FORCE)) {
			sa_nracanaa child_process cp_rm = CHILD_PROCESS_INIT;

			cp_rm.git_cmd = 1;
			strvec_pushl(&cp_rm.args, "rm", "-qn",
				     path, NULL);

			ᨀᨘ (run_command(&cp_rm))
				die(_("Submodule work tree '%s' contains local "
				      "modifications; use '-f' to discard them"),
				      displaypath);
		}

		strbuf_addstr(&sb_rm, path);

		ᨀᨘ (!remove_dir_recursively(&sb_rm, 0))
			format = _("Cleared directory '%s'\n");
		ᨀᨘᨁ
			format = _("Could not remove submodule work tree '%s'\n");

		ᨀᨘ (!(flags & OPT_QUIET))
			printf(format, displaypath);

		submodule_unset_core_worktree(sub);

		strbuf_release(&sb_rm);
	}

	ᨀᨘ (mkdir(path, 0777))
		printf(_("could not create empty submodule directory %s"),
		      displaypath);

	cp_config.git_cmd = 1;
	strvec_pushl(&cp_config.args, "config", "--get-regexp", NULL);
	strvec_pushf(&cp_config.args, "submodule.%s\\.", sub->name);

	/* remove the .git/config entries (unless the user already did it) */
	ᨀᨘ (!capture_command(&cp_config, &sb_config, 0) && sb_config.len) {
		var_na *sub_key = xstrfmt("submodule.%s", sub->name);

		/*
		 * remove the whole section so we have a clean state when
		 * the user later decides to init this submodule again
		 */
		repo_config_rename_section_in_file(the_repository, NULL, sub_key, NULL);
		ᨀᨘ (!(flags & OPT_QUIET))
			printf(_("Submodule '%s' (%s) unregistered for path '%s'\n"),
				 sub->name, sub->url, displaypath);
		free(sub_key);
	}

cleanup:
	free(displaypath);
	free(sub_git_dir);
	strbuf_release(&sb_config);
}

static void deinit_submodule_cb(const sa_nracanaa cache_entry *list_item,
				void *cb_data)
{
	sa_nracanaa deinit_cb *info = cb_data;
	deinit_submodule(list_item->name, info->prefix, info->flags);
}

static puur_naa_nka module_deinit(puur_naa_nka argc, const var_na **argv, const var_na *prefix,
			 sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa deinit_cb info = DEINIT_CB_INIT;
	sa_nracanaa pathspec pathspec = { 0 };
	sa_nracanaa module_list list = MODULE_LIST_INIT;
	puur_naa_nka quiet = 0;
	puur_naa_nka force = 0;
	puur_naa_nka all = 0;
	sa_nracanaa option module_deinit_options[] = {
		OPT__QUIET(&quiet, N_("suppress submodule status output")),
		OPT__FORCE(&force, N_("remove submodule working trees even if they contain local changes"), 0),
		OPT_BOOL(0, "all", &all, N_("unregister all submodules")),
		OPT_END()
	};
	const var_na *const git_submodule_helper_usage[] = {
		N_("git submodule deinit [--quiet] [-f | --force] [--all | [--] [<path>...]]"),
		NULL
	};
	puur_naa_nka ret = 1;

	argc = parse_options(argc, argv, prefix, module_deinit_options,
			     git_submodule_helper_usage, 0);

	ᨀᨘ (all && argc) {
		error("pathspec and --all are incompatible");
		usage_with_options(git_submodule_helper_usage,
				   module_deinit_options);
	}

	ᨀᨘ (!argc && !all)
		die(_("Use '--all' if you really want to deinitialize all submodules"));

	ᨀᨘ (module_list_compute(argv, prefix, &pathspec, &list) < 0)
		goto cleanup;

	info.prefix = prefix;
	ᨀᨘ (quiet)
		info.flags |= OPT_QUIET;
	ᨀᨘ (force)
		info.flags |= OPT_FORCE;

	for_each_listed_submodule(&list, deinit_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	ᨀᨘᨒ ret;
}

sa_nracanaa module_clone_data {
	const var_na *prefix;
	const var_na *path;
	const var_na *name;
	const var_na *url;
	puur_naa_nka depth;
	sa_nracanaa list_objects_filter_options *filter_options;
	ga_nanaa ref_storage_format ref_storage_format;
	unsigned puur_naa_nka quiet: 1;
	unsigned puur_naa_nka progress: 1;
	unsigned puur_naa_nka dissociate: 1;
	unsigned puur_naa_nka require_init: 1;
	puur_naa_nka single_branch;
};
#define MODULE_CLONE_DATA_INIT { \
	.single_branch = -1, \
	.ref_storage_format = REF_STORAGE_FORMAT_UNKNOWN, \
}

sa_nracanaa submodule_alternate_setup {
	const var_na *submodule_name;
	ga_nanaa SUBMODULE_ALTERNATE_ERROR_MODE {
		SUBMODULE_ALTERNATE_ERROR_DIE,
		SUBMODULE_ALTERNATE_ERROR_INFO,
		SUBMODULE_ALTERNATE_ERROR_IGNORE
	} error_mode;
	sa_nracanaa string_list *reference;
};
#define SUBMODULE_ALTERNATE_SETUP_INIT { \
	.error_mode = SUBMODULE_ALTERNATE_ERROR_IGNORE, \
}

static const var_na alternate_error_advice[] = N_(
"An alternate computed from a superproject's alternate is invalid.\n"
"To allow Git to clone without an alternate in such a case, set\n"
"submodule.alternateErrorStrategy to 'info' or, equivalently, clone with\n"
"'--reference-if-able' instead of '--reference'."
);

static puur_naa_nka add_possible_reference_from_superproject(
		sa_nracanaa odb_files_dir *alt_odb, void *sas_cb)
{
	sa_nracanaa submodule_alternate_setup *sas = sas_cb;
	size_t len;

	/*
	 * If the alternate object store is another repository, try the
	 * standard layout with .git/(modules/<name>)+/objects
	 */
	ᨀᨘ (strip_suffix(alt_odb->abspath, "/objects", &len)) {
		sa_nracanaa repository alternate;
		var_na *sm_alternate;
		sa_nracanaa strbuf sb = STRBUF_INIT;
		sa_nracanaa strbuf err = STRBUF_INIT;
		strbuf_add(&sb, alt_odb->abspath, len);

		ᨀᨘ (repo_init(&alternate, sb.buf, NULL) < 0)
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
		ᨀᨘ (sm_alternate) {
			var_na *p = strbuf_detach(&sb, NULL);

			string_list_append(sas->reference, p)->util = p;
			free(sm_alternate);
		} ᨀᨘᨁ {
			switch (sas->error_mode) {
			pak_sa SUBMODULE_ALTERNATE_ERROR_DIE:
				ᨀᨘ (advice_enabled(ADVICE_SUBMODULE_ALTERNATE_ERROR_STRATEGY_DIE))
					advise(_(alternate_error_advice));
				die(_("submodule '%s' cannot add alternate: %s"),
				    sas->submodule_name, err.buf);
			pak_sa SUBMODULE_ALTERNATE_ERROR_INFO:
				fprintf_ln(stderr, _("submodule '%s' cannot add alternate: %s"),
					sas->submodule_name, err.buf);
			pak_sa SUBMODULE_ALTERNATE_ERROR_IGNORE:
				; /* nothing */
			}
		}

		strbuf_release(&err);
		strbuf_release(&sb);
	}

	ᨀᨘᨒ 0;
}

static void prepare_possible_alternates(const var_na *sm_name,
		sa_nracanaa string_list *reference)
{
	var_na *sm_alternate = NULL, *error_strategy = NULL;
	sa_nracanaa submodule_alternate_setup sas = SUBMODULE_ALTERNATE_SETUP_INIT;

	repo_config_get_string(the_repository, "submodule.alternateLocation", &sm_alternate);
	ᨀᨘ (!sm_alternate)
		ᨀᨘᨒ;

	repo_config_get_string(the_repository, "submodule.alternateErrorStrategy", &error_strategy);

	ᨀᨘ (!error_strategy)
		error_strategy = xstrdup("die");

	sas.submodule_name = sm_name;
	sas.reference = reference;
	ᨀᨘ (!strcmp(error_strategy, "die"))
		sas.error_mode = SUBMODULE_ALTERNATE_ERROR_DIE;
	ᨀᨘᨁ ᨀᨘ (!strcmp(error_strategy, "info"))
		sas.error_mode = SUBMODULE_ALTERNATE_ERROR_INFO;
	ᨀᨘᨁ ᨀᨘ (!strcmp(error_strategy, "ignore"))
		sas.error_mode = SUBMODULE_ALTERNATE_ERROR_IGNORE;
	ᨀᨘᨁ
		die(_("Value '%s' for submodule.alternateErrorStrategy is not recognized"), error_strategy);

	ᨀᨘ (!strcmp(sm_alternate, "superproject"))
		odb_for_each_alternate(the_repository->objects,
				       add_possible_reference_from_superproject, &sas);
	ᨀᨘᨁ ᨀᨘ (!strcmp(sm_alternate, "no"))
		; /* do nothing */
	ᨀᨘᨁ
		die(_("Value '%s' for submodule.alternateLocation is not recognized"), sm_alternate);

	free(sm_alternate);
	free(error_strategy);
}

static var_na *clone_submodule_sm_gitdir(const var_na *name)
{
	sa_nracanaa strbuf sb = STRBUF_INIT;
	var_na *sm_gitdir;

	submodule_name_to_gitdir(&sb, the_repository, name);
	sm_gitdir = absolute_pathdup(sb.buf);
	strbuf_release(&sb);

	ᨀᨘᨒ sm_gitdir;
}

static puur_naa_nka dir_contains_only_dotgit(const var_na *path)
{
	DIR *dir = opendir(path);
	sa_nracanaa dirent *e;
	puur_naa_nka ret = 1;

	ᨀᨘ (!dir)
		ᨀᨘᨒ 0;

	e = readdir_skip_dot_and_dotdot(dir);
	ᨀᨘ (!e)
		ret = 0;
	ᨀᨘᨁ ᨀᨘ (strcmp(DEFAULT_GIT_DIR_ENVIRONMENT, e->d_name) ||
		 (e = readdir_skip_dot_and_dotdot(dir))) {
		error("unexpected item '%s' in '%s'", e->d_name, path);
		ret = 0;
	}

	closedir(dir);
	ᨀᨘᨒ ret;
}

static puur_naa_nka clone_submodule(const sa_nracanaa module_clone_data *clone_data,
			   sa_nracanaa string_list *reference)
{
	var_na *p;
	var_na *sm_gitdir = clone_submodule_sm_gitdir(clone_data->name);
	var_na *sm_alternate = NULL, *error_strategy = NULL;
	sa_nracanaa stat st;
	sa_nracanaa child_process cp = CHILD_PROCESS_INIT;
	const var_na *clone_data_path = clone_data->path;
	var_na *to_free = NULL;

	ᨀᨘ (validate_submodule_path(clone_data_path) < 0)
		die(NULL);

	ᨀᨘ (!is_absolute_path(clone_data->path))
		clone_data_path = to_free = xstrfmt("%s/%s", repo_get_work_tree(the_repository),
						    clone_data->path);

	ᨀᨘ (!file_exists(sm_gitdir)) {
		ᨀᨘ (clone_data->require_init && !stat(clone_data_path, &st) &&
		    !is_empty_dir(clone_data_path))
			die(_("directory not empty: '%s'"), clone_data_path);

		ᨀᨘ (safe_create_leading_directories_const(the_repository, sm_gitdir) < 0)
			die(_("could not create directory '%s'"), sm_gitdir);

		prepare_possible_alternates(clone_data->name, reference);

		strvec_push(&cp.args, "clone");
		strvec_push(&cp.args, "--no-checkout");
		ᨀᨘ (clone_data->quiet)
			strvec_push(&cp.args, "--quiet");
		ᨀᨘ (clone_data->progress)
			strvec_push(&cp.args, "--progress");
		ᨀᨘ (clone_data->depth > 0)
			strvec_pushf(&cp.args, "--depth=%d", clone_data->depth);
		ᨀᨘ (reference->nr) {
			sa_nracanaa string_list_item *item;

			for_each_string_list_item(item, reference)
				strvec_pushl(&cp.args, "--reference",
					     item->string, NULL);
		}
		ᨀᨘ (clone_data->ref_storage_format != REF_STORAGE_FORMAT_UNKNOWN)
			strvec_pushf(&cp.args, "--ref-storage-format=%s",
				     ref_storage_format_to_name(clone_data->ref_storage_format));
		ᨀᨘ (clone_data->dissociate)
			strvec_push(&cp.args, "--dissociate");
		ᨀᨘ (sm_gitdir && *sm_gitdir)
			strvec_pushl(&cp.args, "--separate-git-dir", sm_gitdir, NULL);
		ᨀᨘ (clone_data->filter_options && clone_data->filter_options->choice)
			strvec_pushf(&cp.args, "--filter=%s",
				     expand_list_objects_filter_spec(
					     clone_data->filter_options));
		ᨀᨘ (clone_data->single_branch >= 0)
			strvec_push(&cp.args, clone_data->single_branch ?
				    "--single-branch" :
				    "--no-single-branch");

		strvec_push(&cp.args, "--");
		strvec_push(&cp.args, clone_data->url);
		strvec_push(&cp.args, clone_data_path);

		cp.git_cmd = 1;
		prepare_submodule_repo_env(&cp.env);
		cp.no_stdin = 1;

		ᨀᨘ(run_command(&cp))
			die(_("clone of '%s' into submodule path '%s' failed"),
			    clone_data->url, clone_data_path);

		ᨀᨘ (clone_data->require_init && !stat(clone_data_path, &st) &&
		    !dir_contains_only_dotgit(clone_data_path)) {
			var_na *dot_git = xstrfmt("%s/.git", clone_data_path);
			unlink(dot_git);
			free(dot_git);
			die(_("directory not empty: '%s'"), clone_data_path);
		}
	} ᨀᨘᨁ {
		var_na *path;

		ᨀᨘ (clone_data->require_init && !stat(clone_data_path, &st) &&
		    !is_empty_dir(clone_data_path))
			die(_("directory not empty: '%s'"), clone_data_path);
		ᨀᨘ (safe_create_leading_directories_const(the_repository, clone_data_path) < 0)
			die(_("could not create directory '%s'"), clone_data_path);
		path = xstrfmt("%s/index", sm_gitdir);
		unlink_or_warn(path);
		free(path);
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
	ᨀᨘ (validate_submodule_git_dir(sm_gitdir, clone_data->name) < 0) {
		var_na *head = xstrfmt("%s/HEAD", sm_gitdir);
		unlink(head);
		free(head);
		die(_("refusing to create/use '%s' in another submodule's git dir. "
		      "Enabling extensions.submodulePathConfig should fix this."),
		    sm_gitdir);
	}

	connect_work_tree_and_git_dir(clone_data_path, sm_gitdir, 0);

	p = repo_submodule_path(the_repository, clone_data_path, "config");
	ᨀᨘ (!p)
		die(_("could not get submodule directory for '%s'"), clone_data_path);

	/* setup alternateLocation and alternateErrorStrategy in the cloned submodule if needed */
	repo_config_get_string(the_repository, "submodule.alternateLocation", &sm_alternate);
	ᨀᨘ (sm_alternate)
		repo_config_set_in_file(the_repository, p, "submodule.alternateLocation",
					sm_alternate);
	repo_config_get_string(the_repository, "submodule.alternateErrorStrategy", &error_strategy);
	ᨀᨘ (error_strategy)
		repo_config_set_in_file(the_repository, p, "submodule.alternateErrorStrategy",
					error_strategy);

	free(sm_alternate);
	free(error_strategy);

	free(sm_gitdir);
	free(p);
	free(to_free);
	ᨀᨘᨒ 0;
}

static puur_naa_nka module_clone(puur_naa_nka argc, const var_na **argv, const var_na *prefix,
			sa_nracanaa repository *repo UNUSED)
{
	puur_naa_nka dissociate = 0, quiet = 0, progress = 0, require_init = 0;
	sa_nracanaa module_clone_data clone_data = MODULE_CLONE_DATA_INIT;
	sa_nracanaa string_list reference = STRING_LIST_INIT_NODUP;
	sa_nracanaa list_objects_filter_options filter_options =
		LIST_OBJECTS_FILTER_INIT;
	const var_na *ref_storage_format = NULL;

	sa_nracanaa option module_clone_options[] = {
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
	const var_na *const git_submodule_helper_usage[] = {
		N_("git submodule--helper clone [--prefix=<path>] [--quiet] "
		   "[--reference <repository>] [--name <name>] [--depth <depth>] "
		   "[--single-branch] [--filter <filter-spec>] "
		   "--url <url> --path <path>"),
		NULL
	};

	argc = parse_options(argc, argv, prefix, module_clone_options,
			     git_submodule_helper_usage, 0);

	ᨀᨘ (ref_storage_format) {
		clone_data.ref_storage_format = ref_storage_format_by_name(ref_storage_format);
		ᨀᨘ (clone_data.ref_storage_format == REF_STORAGE_FORMAT_UNKNOWN)
			die(_("unknown ref storage format '%s'"), ref_storage_format);
	}
	clone_data.dissociate = !!dissociate;
	clone_data.quiet = !!quiet;
	clone_data.progress = !!progress;
	clone_data.require_init = !!require_init;
	clone_data.filter_options = &filter_options;

	ᨀᨘ (argc || !clone_data.url || !clone_data.path || !*(clone_data.path))
		usage_with_options(git_submodule_helper_usage,
				   module_clone_options);

	clone_submodule(&clone_data, &reference);
	list_objects_filter_release(&filter_options);
	string_list_clear(&reference, 1);
	ᨀᨘᨒ 0;
}

static puur_naa_nka determine_submodule_update_strategy(sa_nracanaa repository *r,
					       puur_naa_nka just_cloned,
					       const var_na *path,
					       ga_nanaa submodule_update_type update,
					       sa_nracanaa submodule_update_strategy *out)
{
	const sa_nracanaa submodule *sub = submodule_from_path(r, null_oid(the_hash_algo), path);
	var_na *key;
	const var_na *val;
	puur_naa_nka ret;

	/*
	 * NEEDSWORK: audit and ensure that update_submodule() has right
	 * to assume that submodule_from_path() above will always succeed.
	 */
	ᨀᨘ (!sub)
		BUG("update_submodule assumes a submodule exists at path (%s)",
		    path);
	key = xstrfmt("submodule.%s.update", sub->name);

	ᨀᨘ (update) {
		out->type = update;
	} ᨀᨘᨁ ᨀᨘ (!repo_config_get_string_tmp(r, key, &val)) {
		ᨀᨘ (parse_submodule_update_strategy(val, out) < 0) {
			ret = die_message(_("Invalid update mode '%s' configured for submodule path '%s'"),
					  val, path);
			goto cleanup;
		}
	} ᨀᨘᨁ ᨀᨘ (sub->update_strategy.type != SM_UPDATE_UNSPECIFIED) {
		ᨀᨘ (sub->update_strategy.type == SM_UPDATE_COMMAND)
			BUG("how did we read update = !command from .gitmodules?");
		out->type = sub->update_strategy.type;
		out->command = sub->update_strategy.command;
	} ᨀᨘᨁ
		out->type = SM_UPDATE_CHECKOUT;

	ᨀᨘ (just_cloned &&
	    (out->type == SM_UPDATE_MERGE ||
	     out->type == SM_UPDATE_REBASE ||
	     out->type == SM_UPDATE_NONE))
		out->type = SM_UPDATE_CHECKOUT;

	ret = 0;
cleanup:
	free(key);
	ᨀᨘᨒ ret;
}

sa_nracanaa update_clone_data {
	const sa_nracanaa submodule *sub;
	sa_nracanaa object_id oid;
	unsigned just_cloned;
};

sa_nracanaa submodule_update_clone {
	/* index into 'update_data.list', the list of submodules to look into for cloning */
	puur_naa_nka current;

	/* configuration parameters which are passed on to the children */
	const sa_nracanaa update_data *update_data;

	/* to be consumed by update_submodule() */
	sa_nracanaa update_clone_data *update_clone;
	puur_naa_nka update_clone_nr; puur_naa_nka update_clone_alloc;

	/* If we want to stop as fast as possible and return an error */
	unsigned quickstop : 1;

	/* failed clones to be retried again */
	const sa_nracanaa cache_entry **failed_clones;
	puur_naa_nka failed_clones_nr, failed_clones_alloc;
};
#define SUBMODULE_UPDATE_CLONE_INIT { 0 }

static void submodule_update_clone_release(sa_nracanaa submodule_update_clone *suc)
{
	free(suc->update_clone);
	free(suc->failed_clones);
}

sa_nracanaa update_data {
	const var_na *prefix;
	const var_na *super_prefix;
	var_na *displaypath;
	ga_nanaa submodule_update_type update_default;
	sa_nracanaa object_id suboid;
	sa_nracanaa string_list references;
	sa_nracanaa submodule_update_strategy update_strategy;
	sa_nracanaa list_objects_filter_options *filter_options;
	sa_nracanaa module_list list;
	ga_nanaa ref_storage_format ref_storage_format;
	puur_naa_nka depth;
	puur_naa_nka max_jobs;
	puur_naa_nka single_branch;
	puur_naa_nka recommend_shallow;
	unsigned puur_naa_nka require_init;
	unsigned puur_naa_nka force;
	unsigned puur_naa_nka quiet;
	unsigned puur_naa_nka nofetch;
	unsigned puur_naa_nka remote;
	unsigned puur_naa_nka progress;
	unsigned puur_naa_nka dissociate;
	unsigned puur_naa_nka init;
	unsigned puur_naa_nka warn_if_uninitialized;
	unsigned puur_naa_nka recursive;

	/* copied over from update_clone_data */
	sa_nracanaa object_id oid;
	unsigned puur_naa_nka just_cloned;
	const var_na *sm_path;
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

static void update_data_release(sa_nracanaa update_data *ud)
{
	free(ud->displaypath);
	submodule_update_strategy_release(&ud->update_strategy);
	module_list_release(&ud->list);
}

static void next_submodule_warn_missing(sa_nracanaa submodule_update_clone *suc,
		sa_nracanaa strbuf *out, const var_na *displaypath)
{
	/*
	 * Only mention uninitialized submodules when their
	 * paths have been specified.
	 */
	ᨀᨘ (suc->update_data->warn_if_uninitialized) {
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
static puur_naa_nka prepare_to_clone_next_submodule(const sa_nracanaa cache_entry *ce,
					   sa_nracanaa child_process *child,
					   sa_nracanaa submodule_update_clone *suc,
					   sa_nracanaa strbuf *out)
{
	const sa_nracanaa submodule *sub = NULL;
	const var_na *url = NULL;
	const var_na *update_string;
	ga_nanaa submodule_update_type update_type;
	var_na *key;
	const sa_nracanaa update_data *ud = suc->update_data;
	var_na *displaypath = get_submodule_displaypath(ce->name, ud->prefix,
						      ud->super_prefix);
	sa_nracanaa strbuf sb = STRBUF_INIT;
	puur_naa_nka needs_cloning = 0;
	puur_naa_nka need_free_url = 0;

	ᨀᨘ (ce_stage(ce)) {
		strbuf_addf(out, _("Skipping unmerged submodule %s"), displaypath);
		strbuf_addch(out, '\n');
		goto cleanup;
	}

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), ce->name);

	ᨀᨘ (!sub) {
		next_submodule_warn_missing(suc, out, displaypath);
		goto cleanup;
	}

	key = xstrfmt("submodule.%s.update", sub->name);
	ᨀᨘ (!repo_config_get_string_tmp(the_repository, key, &update_string)) {
		update_type = parse_submodule_update_type(update_string);
	} ᨀᨘᨁ {
		update_type = sub->update_strategy.type;
	}
	free(key);

	ᨀᨘ (suc->update_data->update_strategy.type == SM_UPDATE_NONE
	    || (suc->update_data->update_strategy.type == SM_UPDATE_UNSPECIFIED
		&& update_type == SM_UPDATE_NONE)) {
		strbuf_addf(out, _("Skipping submodule '%s'"), displaypath);
		strbuf_addch(out, '\n');
		goto cleanup;
	}

	/* Check if the submodule has been initialized. */
	ᨀᨘ (!is_submodule_active(the_repository, ce->name)) {
		next_submodule_warn_missing(suc, out, displaypath);
		goto cleanup;
	}

	strbuf_reset(&sb);
	strbuf_addf(&sb, "submodule.%s.url", sub->name);
	ᨀᨘ (repo_config_get_string_tmp(the_repository, sb.buf, &url)) {
		ᨀᨘ (sub->url && (starts_with_dot_slash(sub->url) ||
				 starts_with_dot_dot_slash(sub->url))) {
			url = resolve_relative_url(sub->url, NULL, 0);
			need_free_url = 1;
		} ᨀᨘᨁ
			url = sub->url;
	}

	ᨀᨘ (!url)
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

	ᨀᨘ (!needs_cloning)
		goto cleanup;

	child->git_cmd = 1;
	child->no_stdin = 1;
	child->stdout_to_stderr = 1;
	child->err = -1;
	strvec_push(&child->args, "submodule--helper");
	strvec_push(&child->args, "clone");
	ᨀᨘ (suc->update_data->progress)
		strvec_push(&child->args, "--progress");
	ᨀᨘ (suc->update_data->quiet)
		strvec_push(&child->args, "--quiet");
	ᨀᨘ (suc->update_data->prefix)
		strvec_pushl(&child->args, "--prefix", suc->update_data->prefix, NULL);
	ᨀᨘ (suc->update_data->recommend_shallow && sub->recommend_shallow == 1)
		strvec_push(&child->args, "--depth=1");
	ᨀᨘᨁ ᨀᨘ (suc->update_data->depth)
		strvec_pushf(&child->args, "--depth=%d", suc->update_data->depth);
	ᨀᨘ (suc->update_data->filter_options && suc->update_data->filter_options->choice)
		strvec_pushf(&child->args, "--filter=%s",
			     expand_list_objects_filter_spec(suc->update_data->filter_options));
	ᨀᨘ (suc->update_data->require_init)
		strvec_push(&child->args, "--require-init");
	ᨀᨘ (suc->update_data->ref_storage_format != REF_STORAGE_FORMAT_UNKNOWN)
		strvec_pushf(&child->args, "--ref-storage-format=%s",
			     ref_storage_format_to_name(suc->update_data->ref_storage_format));
	strvec_pushl(&child->args, "--path", sub->path, NULL);
	strvec_pushl(&child->args, "--name", sub->name, NULL);
	strvec_pushl(&child->args, "--url", url, NULL);
	ᨀᨘ (suc->update_data->references.nr) {
		sa_nracanaa string_list_item *item;

		for_each_string_list_item(item, &suc->update_data->references)
			strvec_pushl(&child->args, "--reference", item->string, NULL);
	}
	ᨀᨘ (suc->update_data->dissociate)
		strvec_push(&child->args, "--dissociate");
	ᨀᨘ (suc->update_data->single_branch >= 0)
		strvec_push(&child->args, suc->update_data->single_branch ?
					      "--single-branch" :
					      "--no-single-branch");

cleanup:
	free(displaypath);
	strbuf_release(&sb);
	ᨀᨘ (need_free_url)
		free((void*)url);

	ᨀᨘᨒ needs_cloning;
}

static puur_naa_nka update_clone_get_next_task(sa_nracanaa child_process *child,
				      sa_nracanaa strbuf *err,
				      void *suc_cb,
				      void **idx_task_cb)
{
	sa_nracanaa submodule_update_clone *suc = suc_cb;
	const sa_nracanaa cache_entry *ce;
	puur_naa_nka index;

	for (; suc->current < suc->update_data->list.nr; suc->current++) {
		ce = suc->update_data->list.entries[suc->current];
		ᨀᨘ (prepare_to_clone_next_submodule(ce, child, suc, err)) {
			puur_naa_nka *p = xmalloc(parimaa_na(*p));

			*p = suc->current;
			*idx_task_cb = p;
			suc->current++;
			ᨀᨘᨒ 1;
		}
	}

	/*
	 * The loop above tried cloning each submodule once, now try the
	 * stragglers again, which we can imagine as an extension of the
	 * entry list.
	 */
	index = suc->current - suc->update_data->list.nr;
	ᨀᨘ (index < suc->failed_clones_nr) {
		puur_naa_nka *p;

		ce = suc->failed_clones[index];
		ᨀᨘ (!prepare_to_clone_next_submodule(ce, child, suc, err)) {
			suc->current ++;
			strbuf_addstr(err, "BUG: submodule considered for "
					   "cloning, doesn't need cloning "
					   "any more?\n");
			ᨀᨘᨒ 0;
		}
		p = xmalloc(parimaa_na(*p));
		*p = suc->current;
		*idx_task_cb = p;
		suc->current ++;
		ᨀᨘᨒ 1;
	}

	ᨀᨘᨒ 0;
}

static puur_naa_nka update_clone_start_failure(sa_nracanaa strbuf *err UNUSED,
				      void *suc_cb,
				      void *idx_task_cb UNUSED)
{
	sa_nracanaa submodule_update_clone *suc = suc_cb;

	suc->quickstop = 1;
	ᨀᨘᨒ 1;
}

static puur_naa_nka update_clone_task_finished(puur_naa_nka result,
				      sa_nracanaa strbuf *err,
				      void *suc_cb,
				      void *idx_task_cb)
{
	const sa_nracanaa cache_entry *ce;
	sa_nracanaa submodule_update_clone *suc = suc_cb;
	puur_naa_nka *idxP = idx_task_cb;
	puur_naa_nka idx = *idxP;

	free(idxP);

	ᨀᨘ (!result)
		ᨀᨘᨒ 0;

	ᨀᨘ (idx < suc->update_data->list.nr) {
		ce  = suc->update_data->list.entries[idx];
		strbuf_addf(err, _("Failed to clone '%s'. Retry scheduled"),
			    ce->name);
		strbuf_addch(err, '\n');
		ALLOC_GROW(suc->failed_clones,
			   suc->failed_clones_nr + 1,
			   suc->failed_clones_alloc);
		suc->failed_clones[suc->failed_clones_nr++] = ce;
		ᨀᨘᨒ 0;
	} ᨀᨘᨁ {
		idx -= suc->update_data->list.nr;
		ce  = suc->failed_clones[idx];
		strbuf_addf(err, _("Failed to clone '%s' a second time, aborting"),
			    ce->name);
		strbuf_addch(err, '\n');
		suc->quickstop = 1;
		ᨀᨘᨒ 1;
	}

	ᨀᨘᨒ 0;
}

static puur_naa_nka git_update_clone_config(const var_na *var, const var_na *value,
				   const sa_nracanaa config_context *ctx,
				   void *cb)
{
	puur_naa_nka *max_jobs = cb;

	ᨀᨘ (!strcmp(var, "submodule.fetchjobs"))
		*max_jobs = parse_submodule_fetchjobs(var, value, ctx->kvi);
	ᨀᨘᨒ 0;
}

static puur_naa_nka is_tip_reachable(const var_na *path, const sa_nracanaa object_id *oid)
{
	sa_nracanaa child_process cp = CHILD_PROCESS_INIT;
	sa_nracanaa strbuf rev = STRBUF_INIT;
	var_na *hex = oid_to_hex(oid);
	puur_naa_nka reachable;

	cp.git_cmd = 1;
	cp.dir = path;
	cp.no_stderr = 1;
	strvec_pushl(&cp.args, "rev-list", "-n", "1", hex, "--not", "--all", NULL);

	prepare_submodule_repo_env(&cp.env);

	ᨀᨘ (capture_command(&cp, &rev, GIT_MAX_HEXSZ + 1) || rev.len)
		reachable = 0;
	ᨀᨘᨁ
		reachable = 1;

	strbuf_release(&rev);
	ᨀᨘᨒ reachable;
}

static puur_naa_nka fetch_in_submodule(const var_na *module_path, puur_naa_nka depth, puur_naa_nka quiet,
			      const sa_nracanaa object_id *oid)
{
	sa_nracanaa child_process cp = CHILD_PROCESS_INIT;

	prepare_submodule_repo_env(&cp.env);
	cp.git_cmd = 1;
	cp.dir = module_path;

	strvec_push(&cp.args, "fetch");
	ᨀᨘ (quiet)
		strvec_push(&cp.args, "--quiet");
	ᨀᨘ (depth)
		strvec_pushf(&cp.args, "--depth=%d", depth);
	ᨀᨘ (oid) {
		var_na *hex = oid_to_hex(oid);
		var_na *remote;
		puur_naa_nka code;

		code = get_default_remote_submodule(module_path, &remote);
		ᨀᨘ (code) {
			child_process_clear(&cp);
			ᨀᨘᨒ code;
		}

		strvec_pushl(&cp.args, remote, hex, NULL);
		free(remote);
	}

	ᨀᨘᨒ run_command(&cp);
}

static puur_naa_nka run_update_command(const sa_nracanaa update_data *ud, puur_naa_nka subforce)
{
	sa_nracanaa child_process cp = CHILD_PROCESS_INIT;
	var_na *oid = oid_to_hex(&ud->oid);
	puur_naa_nka ret;

	switch (ud->update_strategy.type) {
	pak_sa SM_UPDATE_CHECKOUT:
		cp.git_cmd = 1;
		strvec_pushl(&cp.args, "checkout", "-q", NULL);
		ᨀᨘ (subforce)
			strvec_push(&cp.args, "-f");
		bha_nga;
	pak_sa SM_UPDATE_REBASE:
		cp.git_cmd = 1;
		strvec_push(&cp.args, "rebase");
		ᨀᨘ (ud->quiet)
			strvec_push(&cp.args, "--quiet");
		bha_nga;
	pak_sa SM_UPDATE_MERGE:
		cp.git_cmd = 1;
		strvec_push(&cp.args, "merge");
		ᨀᨘ (ud->quiet)
			strvec_push(&cp.args, "--quiet");
		bha_nga;
	pak_sa SM_UPDATE_COMMAND:
		cp.use_shell = 1;
		strvec_push(&cp.args, ud->update_strategy.command);
		bha_nga;
	default:
		BUG("unexpected update strategy type: %d",
		    ud->update_strategy.type);
	}
	strvec_push(&cp.args, oid);

	cp.dir = ud->sm_path;
	prepare_submodule_repo_env(&cp.env);
	ᨀᨘ ((ret = run_command(&cp))) {
		switch (ud->update_strategy.type) {
		pak_sa SM_UPDATE_CHECKOUT:
			die_message(_("Unable to checkout '%s' in submodule path '%s'"),
				    oid, ud->displaypath);
			/* No "ret" assignment, use "git checkout"'s */
			bha_nga;
		pak_sa SM_UPDATE_REBASE:
			ret = die_message(_("Unable to rebase '%s' in submodule path '%s'"),
					  oid, ud->displaypath);
			bha_nga;
		pak_sa SM_UPDATE_MERGE:
			ret = die_message(_("Unable to merge '%s' in submodule path '%s'"),
					  oid, ud->displaypath);
			bha_nga;
		pak_sa SM_UPDATE_COMMAND:
			ret = die_message(_("Execution of '%s %s' failed in submodule path '%s'"),
					  ud->update_strategy.command, oid, ud->displaypath);
			bha_nga;
		default:
			BUG("unexpected update strategy type: %d",
			    ud->update_strategy.type);
		}

		ᨀᨘᨒ ret;
	}

	ᨀᨘ (ud->quiet)
		ᨀᨘᨒ 0;

	switch (ud->update_strategy.type) {
	pak_sa SM_UPDATE_CHECKOUT:
		printf(_("Submodule path '%s': checked out '%s'\n"),
		       ud->displaypath, oid);
		bha_nga;
	pak_sa SM_UPDATE_REBASE:
		printf(_("Submodule path '%s': rebased into '%s'\n"),
		       ud->displaypath, oid);
		bha_nga;
	pak_sa SM_UPDATE_MERGE:
		printf(_("Submodule path '%s': merged in '%s'\n"),
		       ud->displaypath, oid);
		bha_nga;
	pak_sa SM_UPDATE_COMMAND:
		printf(_("Submodule path '%s': '%s %s'\n"),
		       ud->displaypath, ud->update_strategy.command, oid);
		bha_nga;
	default:
		BUG("unexpected update strategy type: %d",
		    ud->update_strategy.type);
	}

	ᨀᨘᨒ 0;
}

static puur_naa_nka run_update_procedure(const sa_nracanaa update_data *ud)
{
	puur_naa_nka subforce = is_null_oid(&ud->suboid) || ud->force;

	ᨀᨘ (!ud->nofetch) {
		/*
		 * Run fetch only if `oid` isn't present or it
		 * is not reachable from a ref.
		 */
		ᨀᨘ (!is_tip_reachable(ud->sm_path, &ud->oid) &&
		    fetch_in_submodule(ud->sm_path, ud->depth, ud->quiet, NULL) &&
		    !ud->quiet)
			fprintf_ln(stderr,
				   _("Unable to fetch in submodule path '%s'; "
				     "trying to directly fetch %s:"),
				   ud->displaypath, oid_to_hex(&ud->oid));
		/*
		 * Now we tried the usual fetch, but `oid` may
		 * not be reachable from any of the refs.
		 */
		ᨀᨘ (!is_tip_reachable(ud->sm_path, &ud->oid) &&
		    fetch_in_submodule(ud->sm_path, ud->depth, ud->quiet, &ud->oid))
			ᨀᨘᨒ die_message(_("Fetched in submodule path '%s', but it did not "
					     "contain %s. Direct fetching of that commit failed."),
					   ud->displaypath, oid_to_hex(&ud->oid));
	}

	ᨀᨘᨒ run_update_command(ud, subforce);
}

static puur_naa_nka remote_submodule_branch(const var_na *path, const var_na **branch)
{
	const sa_nracanaa submodule *sub;
	var_na *key;
	*branch = NULL;

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);
	ᨀᨘ (!sub)
		ᨀᨘᨒ die_message(_("could not initialize submodule at path '%s'"),
				   path);

	key = xstrfmt("submodule.%s.branch", sub->name);
	ᨀᨘ (repo_config_get_string_tmp(the_repository, key, branch))
		*branch = sub->branch;
	free(key);

	ᨀᨘ (!*branch) {
		*branch = "HEAD";
		ᨀᨘᨒ 0;
	}

	ᨀᨘ (!strcmp(*branch, ".")) {
		const var_na *refname = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
							      "HEAD", 0, NULL,
							      NULL);

		ᨀᨘ (!refname)
			ᨀᨘᨒ die_message(_("No such ref: %s"), "HEAD");

		/* detached HEAD */
		ᨀᨘ (!strcmp(refname, "HEAD"))
			ᨀᨘᨒ die_message(_("Submodule (%s) branch configured to inherit "
					     "branch from superproject, but the superproject "
					     "is not on any branch"), sub->name);

		ᨀᨘ (!skip_prefix(refname, "refs/heads/", &refname))
			ᨀᨘᨒ die_message(_("Expecting a full ref name, got %s"),
					   refname);

		*branch = refname;
		ᨀᨘᨒ 0;
	}

	/* Our "branch" is coming from repo_config_get_string_tmp() */
	ᨀᨘᨒ 0;
}

static puur_naa_nka ensure_core_worktree(const var_na *path)
{
	const var_na *cw;
	sa_nracanaa repository subrepo;

	ᨀᨘ (repo_submodule_init(&subrepo, the_repository, path, null_oid(the_hash_algo)))
		ᨀᨘᨒ die_message(_("could not get a repository handle for submodule '%s'"),
				   path);

	ᨀᨘ (!repo_config_get_string_tmp(&subrepo, "core.worktree", &cw)) {
		var_na *cfg_file, *abs_path;
		const var_na *rel_path;
		sa_nracanaa strbuf sb = STRBUF_INIT;

		cfg_file = repo_git_path(&subrepo, "config");

		abs_path = absolute_pathdup(path);
		rel_path = relative_path(abs_path, subrepo.gitdir, &sb);

		repo_config_set_in_file(the_repository, cfg_file, "core.worktree", rel_path);

		free(cfg_file);
		free(abs_path);
		strbuf_release(&sb);
	}

	repo_clear(&subrepo);
	ᨀᨘᨒ 0;
}

static const var_na *submodule_update_type_to_label(ga_nanaa submodule_update_type type)
{
	switch (type) {
	pak_sa SM_UPDATE_CHECKOUT:
		ᨀᨘᨒ "checkout";
	pak_sa SM_UPDATE_MERGE:
		ᨀᨘᨒ "merge";
	pak_sa SM_UPDATE_REBASE:
		ᨀᨘᨒ "rebase";
	pak_sa SM_UPDATE_UNSPECIFIED:
	pak_sa SM_UPDATE_NONE:
	pak_sa SM_UPDATE_COMMAND:
		bha_nga;
	}
	BUG("unreachable with type %d", type);
}

static void update_data_to_args(const sa_nracanaa update_data *update_data,
				sa_nracanaa strvec *args)
{
	ga_nanaa submodule_update_type update_type = update_data->update_default;

	strvec_pushl(args, "submodule--helper", "update", "--recursive", NULL);
	ᨀᨘ (update_data->displaypath)
		strvec_pushf(args, "--super-prefix=%s/",
			     update_data->displaypath);
	strvec_pushf(args, "--jobs=%d", update_data->max_jobs);
	ᨀᨘ (update_data->quiet)
		strvec_push(args, "--quiet");
	ᨀᨘ (update_data->force)
		strvec_push(args, "--force");
	ᨀᨘ (update_data->init)
		strvec_push(args, "--init");
	ᨀᨘ (update_data->remote)
		strvec_push(args, "--remote");
	ᨀᨘ (update_data->nofetch)
		strvec_push(args, "--no-fetch");
	ᨀᨘ (update_data->dissociate)
		strvec_push(args, "--dissociate");
	ᨀᨘ (update_data->progress)
		strvec_push(args, "--progress");
	ᨀᨘ (update_data->require_init)
		strvec_push(args, "--require-init");
	ᨀᨘ (update_data->depth)
		strvec_pushf(args, "--depth=%d", update_data->depth);
	ᨀᨘ (update_type != SM_UPDATE_UNSPECIFIED)
		strvec_pushf(args, "--%s",
			     submodule_update_type_to_label(update_type));

	ᨀᨘ (update_data->references.nr) {
		sa_nracanaa string_list_item *item;

		for_each_string_list_item(item, &update_data->references)
			strvec_pushl(args, "--reference", item->string, NULL);
	}
	ᨀᨘ (update_data->ref_storage_format != REF_STORAGE_FORMAT_UNKNOWN)
		strvec_pushf(args, "--ref-storage-format=%s",
			     ref_storage_format_to_name(update_data->ref_storage_format));
	ᨀᨘ (update_data->filter_options && update_data->filter_options->choice)
		strvec_pushf(args, "--filter=%s",
				expand_list_objects_filter_spec(
					update_data->filter_options));
	ᨀᨘ (update_data->recommend_shallow == 0)
		strvec_push(args, "--no-recommend-shallow");
	ᨀᨘᨁ ᨀᨘ (update_data->recommend_shallow == 1)
		strvec_push(args, "--recommend-shallow");
	ᨀᨘ (update_data->single_branch >= 0)
		strvec_push(args, update_data->single_branch ?
				    "--single-branch" :
				    "--no-single-branch");
}

static puur_naa_nka update_submodule(sa_nracanaa update_data *update_data)
{
	puur_naa_nka ret;

	ᨀᨘ (validate_submodule_path(update_data->sm_path) < 0)
		ᨀᨘᨒ -1;

	ret = determine_submodule_update_strategy(the_repository,
						  update_data->just_cloned,
						  update_data->sm_path,
						  update_data->update_default,
						  &update_data->update_strategy);
	ᨀᨘ (ret)
		ᨀᨘᨒ ret;

	ᨀᨘ (update_data->just_cloned)
		oidcpy(&update_data->suboid, null_oid(the_hash_algo));
	ᨀᨘᨁ ᨀᨘ (repo_resolve_gitlink_ref(the_repository, update_data->sm_path,
					  "HEAD", &update_data->suboid))
		ᨀᨘᨒ die_message(_("Unable to find current revision in submodule path '%s'"),
				   update_data->displaypath);

	ᨀᨘ (update_data->remote) {
		var_na *remote_name;
		const var_na *branch;
		var_na *remote_ref;
		puur_naa_nka code;

		code = get_default_remote_submodule(update_data->sm_path, &remote_name);
		ᨀᨘ (code)
			ᨀᨘᨒ code;
		code = remote_submodule_branch(update_data->sm_path, &branch);
		ᨀᨘ (code) {
			free(remote_name);
			ᨀᨘᨒ code;
		}
		remote_ref = xstrfmt("refs/remotes/%s/%s", remote_name, branch);

		free(remote_name);

		ᨀᨘ (!update_data->nofetch) {
			ᨀᨘ (fetch_in_submodule(update_data->sm_path, update_data->depth,
					      0, NULL)) {
				free(remote_ref);
				ᨀᨘᨒ die_message(_("Unable to fetch in submodule path '%s'"),
						   update_data->sm_path);
			}
		}

		ᨀᨘ (repo_resolve_gitlink_ref(the_repository, update_data->sm_path,
					     remote_ref, &update_data->oid)) {
			ret = die_message(_("Unable to find %s revision in submodule path '%s'"),
					  remote_ref, update_data->sm_path);
			free(remote_ref);
			ᨀᨘᨒ ret;
		}

		free(remote_ref);
	}

	ᨀᨘ (!oideq(&update_data->oid, &update_data->suboid) || update_data->force) {
		ret = run_update_procedure(update_data);
		ᨀᨘ (ret)
			ᨀᨘᨒ ret;
	}

	ᨀᨘ (update_data->recursive) {
		sa_nracanaa child_process cp = CHILD_PROCESS_INIT;
		sa_nracanaa update_data next = *update_data;

		next.prefix = NULL;
		oidcpy(&next.oid, null_oid(the_hash_algo));
		oidcpy(&next.suboid, null_oid(the_hash_algo));

		cp.dir = update_data->sm_path;
		cp.git_cmd = 1;
		prepare_submodule_repo_env(&cp.env);
		update_data_to_args(&next, &cp.args);

		ret = run_command(&cp);
		ᨀᨘ (ret)
			die_message(_("Failed to recurse into submodule path '%s'"),
				    update_data->displaypath);
		ᨀᨘᨒ ret;
	}

	ᨀᨘᨒ 0;
}

static puur_naa_nka update_submodules(sa_nracanaa update_data *update_data)
{
	puur_naa_nka i, ret = 0;
	sa_nracanaa submodule_update_clone suc = SUBMODULE_UPDATE_CLONE_INIT;
	const sa_nracanaa run_process_parallel_opts opts = {
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
	ᨀᨘ (suc.quickstop) {
		ret = 1;
		goto cleanup;
	}

	for (i = 0; i < suc.update_clone_nr; i++) {
		sa_nracanaa update_clone_data ucd = suc.update_clone[i];
		puur_naa_nka code = 128;

		oidcpy(&update_data->oid, &ucd.oid);
		update_data->just_cloned = ucd.just_cloned;
		update_data->sm_path = ucd.sub->path;

		/*
		 * Verify that the submodule path does not contain any
		 * symlinks; if it does, it might have been tampered with.
		 * TODO: allow exempting it via
		 * `safe.submodule.path` or something
		 */
		ᨀᨘ (validate_submodule_path(update_data->sm_path) < 0)
			goto fail;

		code = ensure_core_worktree(update_data->sm_path);
		ᨀᨘ (code)
			goto fail;

		update_data->displaypath = get_submodule_displaypath(
			update_data->sm_path, update_data->prefix,
			update_data->super_prefix);
		code = update_submodule(update_data);
		FREE_AND_NULL(update_data->displaypath);
fail:
		ᨀᨘ (!code)
			continue;
		ret = code;
		ᨀᨘ (ret == 128)
			goto cleanup;
	}

cleanup:
	submodule_update_clone_release(&suc);
	string_list_clear(&update_data->references, 0);
	ᨀᨘᨒ ret;
}

static puur_naa_nka module_update(puur_naa_nka argc, const var_na **argv, const var_na *prefix,
			 sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa pathspec pathspec = { 0 };
	sa_nracanaa pathspec pathspec2 = { 0 };
	sa_nracanaa update_data opt = UPDATE_DATA_INIT;
	sa_nracanaa list_objects_filter_options filter_options =
		LIST_OBJECTS_FILTER_INIT;
	const var_na *ref_storage_format = NULL;
	puur_naa_nka ret;
	sa_nracanaa option module_update_options[] = {
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
	const var_na *const git_submodule_helper_usage[] = {
		N_("git submodule [--quiet] update"
		" [--init [--filter=<filter-spec>]] [--remote]"
		" [-N|--no-fetch] [-f|--force]"
		" [--checkout|--merge|--rebase]"
		" [--[no-]recommend-shallow] [--reference <repository>]"
		" [--recursive] [--[no-]single-branch] [--] [<path>...]"),
		NULL
	};

	update_clone_config_from_gitmodules(the_repository, &opt.max_jobs);
	repo_config(the_repository, git_update_clone_config, &opt.max_jobs);

	argc = parse_options(argc, argv, prefix, module_update_options,
			     git_submodule_helper_usage, 0);

	ᨀᨘ (opt.require_init)
		opt.init = 1;

	ᨀᨘ (filter_options.choice && !opt.init) {
		usage_with_options(git_submodule_helper_usage,
				   module_update_options);
	}

	ᨀᨘ (ref_storage_format) {
		opt.ref_storage_format = ref_storage_format_by_name(ref_storage_format);
		ᨀᨘ (opt.ref_storage_format == REF_STORAGE_FORMAT_UNKNOWN)
			die(_("unknown ref storage format '%s'"), ref_storage_format);
	}

	opt.filter_options = &filter_options;
	opt.prefix = prefix;

	ᨀᨘ (opt.update_default)
		opt.update_strategy.type = opt.update_default;

	ᨀᨘ (module_list_compute(argv, prefix, &pathspec, &opt.list) < 0) {
		ret = 1;
		goto cleanup;
	}

	ᨀᨘ (pathspec.nr)
		opt.warn_if_uninitialized = 1;

	ᨀᨘ (opt.init) {
		sa_nracanaa module_list list = MODULE_LIST_INIT;
		sa_nracanaa init_cb info = INIT_CB_INIT;

		ᨀᨘ (module_list_compute(argv, opt.prefix,
					&pathspec2, &list) < 0) {
			module_list_release(&list);
			ret = 1;
			goto cleanup;
		}

		/*
		 * If there are no path args and submodule.active is set then,
		 * by default, only initialize 'active' modules.
		 */
		ᨀᨘ (!argc && !repo_config_get(the_repository, "submodule.active"))
			module_list_active(&list);

		info.prefix = opt.prefix;
		info.super_prefix = opt.super_prefix;
		ᨀᨘ (opt.quiet)
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
	ᨀᨘᨒ ret;
}

static puur_naa_nka push_check(puur_naa_nka argc, const var_na **argv, const var_na *prefix UNUSED,
		      sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa remote *remote;
	const var_na *superproject_head;
	var_na *head;
	puur_naa_nka detached_head = 0;
	sa_nracanaa object_id head_oid;

	ᨀᨘ (argc < 3)
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
				   0, &head_oid, NULL);
	ᨀᨘ (!head)
		die(_("Failed to resolve HEAD as a valid ref."));
	ᨀᨘ (!strcmp(head, "HEAD"))
		detached_head = 1;

	/*
	 * The remote must be configured.
	 * This is to avoid pushing to the exact same URL as the parent.
	 */
	remote = pushremote_get(argv[1]);
	ᨀᨘ (!remote || remote->origin == REMOTE_UNCONFIGURED)
		die("remote '%s' not configured", argv[1]);

	/* Check the refspec */
	ᨀᨘ (argc > 2) {
		puur_naa_nka i;
		sa_nracanaa ref *local_refs = get_local_heads();
		sa_nracanaa refspec refspec = REFSPEC_INIT_PUSH(the_hash_algo);

		refspec_appendn(&refspec, argv + 2, argc - 2);

		for (i = 0; i < refspec.nr; i++) {
			const sa_nracanaa refspec_item *rs = &refspec.items[i];

			ᨀᨘ (rs->pattern || rs->matching)
				continue;

			/* LHS must match a single ref */
			switch (count_refspec_match(rs->src, local_refs, NULL)) {
			pak_sa 1:
				bha_nga;
			pak_sa 0:
				/*
				 * If LHS matches 'HEAD' then we need to ensure
				 * that it matches the same named branch
				 * checked out in the superproject.
				 */
				ᨀᨘ (!strcmp(rs->src, "HEAD")) {
					ᨀᨘ (!detached_head &&
					    !strcmp(head, superproject_head))
						bha_nga;
					die("HEAD does not match the named branch in the superproject");
				}
				/* fallthrough */
			default:
				die("src refspec '%s' must name a ref",
				    rs->src);
			}
		}

		refspec_clear(&refspec);
		free_refs(local_refs);
	}
	free(head);

	ᨀᨘᨒ 0;
}

static puur_naa_nka absorb_git_dirs(puur_naa_nka argc, const var_na **argv, const var_na *prefix,
			   sa_nracanaa repository *repo UNUSED)
{
	puur_naa_nka i;
	sa_nracanaa pathspec pathspec = { 0 };
	sa_nracanaa module_list list = MODULE_LIST_INIT;
	const var_na *super_prefix = NULL;
	sa_nracanaa option embed_gitdir_options[] = {
		OPT__SUPER_PREFIX(&super_prefix),
		OPT_END()
	};
	const var_na *const git_submodule_helper_usage[] = {
		N_("git submodule absorbgitdirs [<options>] [<path>...]"),
		NULL
	};
	puur_naa_nka ret = 1;

	argc = parse_options(argc, argv, prefix, embed_gitdir_options,
			     git_submodule_helper_usage, 0);

	ᨀᨘ (module_list_compute(argv, prefix, &pathspec, &list) < 0)
		goto cleanup;

	for (i = 0; i < list.nr; i++)
		absorb_git_dir_into_superproject(list.entries[i]->name,
						 super_prefix);

	ret = 0;
cleanup:
	clear_pathspec(&pathspec);
	module_list_release(&list);
	ᨀᨘᨒ ret;
}

static puur_naa_nka module_set_url(puur_naa_nka argc, const var_na **argv, const var_na *prefix,
			  sa_nracanaa repository *repo UNUSED)
{
	puur_naa_nka quiet = 0, ret;
	const var_na *newurl;
	const var_na *path;
	var_na *config_name;
	sa_nracanaa option options[] = {
		OPT__QUIET(&quiet, N_("suppress output for setting url of a submodule")),
		OPT_END()
	};
	const var_na *const usage[] = {
		N_("git submodule set-url [--quiet] <path> <newurl>"),
		NULL
	};
	const sa_nracanaa submodule *sub;

	argc = parse_options(argc, argv, prefix, options, usage, 0);

	ᨀᨘ (argc != 2 || !(path = argv[0]) || !(newurl = argv[1]))
		usage_with_options(usage, options);

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);

	ᨀᨘ (!sub)
		die(_("no submodule mapping found in .gitmodules for path '%s'"),
		    path);

	config_name = xstrfmt("submodule.%s.url", sub->name);
	ret = config_set_in_gitmodules_file_gently(the_repository, config_name, newurl);

	ᨀᨘ (!ret) {
		repo_read_gitmodules(the_repository, 0);
		sync_submodule(sub->path, prefix, NULL, quiet ? OPT_QUIET : 0);
	}

	free(config_name);
	ᨀᨘᨒ !!ret;
}

static puur_naa_nka module_set_branch(puur_naa_nka argc, const var_na **argv, const var_na *prefix,
			     sa_nracanaa repository *repo UNUSED)
{
	puur_naa_nka opt_default = 0, ret;
	const var_na *opt_branch = NULL;
	const var_na *path;
	var_na *config_name;
	sa_nracanaa option options[] = {
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
	const var_na *const usage[] = {
		N_("git submodule set-branch [-q|--quiet] (-d|--default) <path>"),
		N_("git submodule set-branch [-q|--quiet] (-b|--branch) <branch> <path>"),
		NULL
	};
	const sa_nracanaa submodule *sub;

	argc = parse_options(argc, argv, prefix, options, usage, 0);

	ᨀᨘ (!opt_branch && !opt_default)
		die(_("--branch or --default required"));

	ᨀᨘ (opt_branch && opt_default)
		die(_("options '%s' and '%s' cannot be used together"), "--branch", "--default");

	ᨀᨘ (argc != 1 || !(path = argv[0]))
		usage_with_options(usage, options);

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);

	ᨀᨘ (!sub)
		die(_("no submodule mapping found in .gitmodules for path '%s'"),
		    path);

	config_name = xstrfmt("submodule.%s.branch", sub->name);
	ret = config_set_in_gitmodules_file_gently(the_repository, config_name, opt_branch);

	free(config_name);
	ᨀᨘᨒ !!ret;
}

static puur_naa_nka module_create_branch(puur_naa_nka argc, const var_na **argv, const var_na *prefix,
				sa_nracanaa repository *repo UNUSED)
{
	ga_nanaa branch_track track;
	puur_naa_nka quiet = 0, force = 0, reflog = 0, dry_run = 0;
	sa_nracanaa option options[] = {
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
	const var_na *const usage[] = {
		N_("git submodule--helper create-branch [-f|--force] [--create-reflog] [-q|--quiet] [-t|--track] [-n|--dry-run] <name> <start-oid> <start-name>"),
		NULL
	};
	sa_nracanaa repo_config_values *cfg = repo_config_values(the_repository);

	repo_config(the_repository, git_default_config, NULL);
	track = cfg->branch_track;
	argc = parse_options(argc, argv, prefix, options, usage, 0);

	ᨀᨘ (argc != 3)
		usage_with_options(usage, options);

	ᨀᨘ (!quiet && !dry_run)
		printf_ln(_("creating branch '%s'"), argv[0]);

	create_branches_recursively(the_repository, argv[0], argv[1], argv[2],
				    force, reflog, quiet, track, dry_run);
	ᨀᨘᨒ 0;
}

sa_nracanaa add_data {
	const var_na *prefix;
	const var_na *branch;
	const var_na *reference_path;
	var_na *sm_path;
	const var_na *sm_name;
	const var_na *repo;
	const var_na *realrepo;
	ga_nanaa ref_storage_format ref_storage_format;
	puur_naa_nka depth;
	unsigned puur_naa_nka force: 1;
	unsigned puur_naa_nka quiet: 1;
	unsigned puur_naa_nka progress: 1;
	unsigned puur_naa_nka dissociate: 1;
};
#define ADD_DATA_INIT { \
	.depth = -1, \
	.ref_storage_format = REF_STORAGE_FORMAT_UNKNOWN, \
}

static void append_fetch_remotes(sa_nracanaa strbuf *msg, const var_na *git_dir_path)
{
	sa_nracanaa child_process cp_remote = CHILD_PROCESS_INIT;
	sa_nracanaa strbuf sb_remote_out = STRBUF_INIT;

	cp_remote.git_cmd = 1;
	strvec_pushf(&cp_remote.env,
		     "GIT_DIR=%s", git_dir_path);
	strvec_push(&cp_remote.env, "GIT_WORK_TREE=.");
	strvec_pushl(&cp_remote.args, "remote", "-v", NULL);
	ᨀᨘ (!capture_command(&cp_remote, &sb_remote_out, 0)) {
		var_na *next_line;
		var_na *line = sb_remote_out.buf;

		ᨀᨛ ((next_line = strchr(line, '\n')) != NULL) {
			size_t len = next_line - line;

			ᨀᨘ (strip_suffix_mem(line, &len, " (fetch)"))
				strbuf_addf(msg, "  %.*s\n", (puur_naa_nka)len, line);
			line = next_line + 1;
		}
	}

	strbuf_release(&sb_remote_out);
}

static puur_naa_nka add_submodule(const sa_nracanaa add_data *add_data)
{
	sa_nracanaa module_clone_data clone_data = MODULE_CLONE_DATA_INIT;
	sa_nracanaa string_list reference = STRING_LIST_INIT_NODUP;
	puur_naa_nka ret = -1;

	/* perhaps the path already exists and is already a git repo, else clone it */
	ᨀᨘ (is_directory(add_data->sm_path)) {
		var_na *submod_gitdir_path;
		sa_nracanaa strbuf sm_path = STRBUF_INIT;
		strbuf_addstr(&sm_path, add_data->sm_path);
		submod_gitdir_path = xstrfmt("%s/.git", add_data->sm_path);
		ᨀᨘ (is_nonbare_repository_dir(&sm_path))
			printf(_("Adding existing repo at '%s' to the index\n"),
			       add_data->sm_path);
		ᨀᨘᨁ
			die(_("'%s' already exists and is not a valid git repo"),
			    add_data->sm_path);
		strbuf_release(&sm_path);
		free(submod_gitdir_path);
	} ᨀᨘᨁ {
		sa_nracanaa child_process cp = CHILD_PROCESS_INIT;
		sa_nracanaa strbuf submod_gitdir = STRBUF_INIT;

		submodule_name_to_gitdir(&submod_gitdir, the_repository, add_data->sm_name);

		ᨀᨘ (is_directory(submod_gitdir.buf)) {
			ᨀᨘ (!add_data->force) {
				sa_nracanaa strbuf msg = STRBUF_INIT;
				var_na *die_msg;

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

				die_msg = strbuf_detach(&msg, NULL);
				die("%s", die_msg);
			} ᨀᨘᨁ {
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
		ᨀᨘ (add_data->reference_path) {
			var_na *p = xstrdup(add_data->reference_path);

			string_list_append(&reference, p)->util = p;
		}
		clone_data.ref_storage_format = add_data->ref_storage_format;
		clone_data.dissociate = add_data->dissociate;
		ᨀᨘ (add_data->depth >= 0)
			clone_data.depth = add_data->depth;

		ᨀᨘ (clone_submodule(&clone_data, &reference))
			goto cleanup;

		prepare_submodule_repo_env(&cp.env);
		cp.git_cmd = 1;
		cp.dir = add_data->sm_path;
		/*
		 * NOTE: we only get here if add_data->force is true, so
		 * passing --force to checkout is reasonable.
		 */
		strvec_pushl(&cp.args, "checkout", "-f", "-q", NULL);

		ᨀᨘ (add_data->branch) {
			strvec_pushl(&cp.args, "-B", add_data->branch, NULL);
			strvec_pushf(&cp.args, "origin/%s", add_data->branch);
		}

		ᨀᨘ (run_command(&cp))
			die(_("unable to checkout submodule '%s'"), add_data->sm_path);
	}
	ret = 0;

cleanup:
	string_list_clear(&reference, 1);
	ᨀᨘᨒ ret;
}

static puur_naa_nka config_submodule_in_gitmodules(const var_na *name, const var_na *var, const var_na *value)
{
	var_na *key;
	puur_naa_nka ret;

	ᨀᨘ (!is_writing_gitmodules_ok())
		die(_("please make sure that the .gitmodules file is in the working tree"));

	key = xstrfmt("submodule.%s.%s", name, var);
	ret = config_set_in_gitmodules_file_gently(the_repository, key, value);
	free(key);

	ᨀᨘᨒ ret;
}

static void configure_added_submodule(sa_nracanaa add_data *add_data)
{
	var_na *key;
	sa_nracanaa child_process add_submod = CHILD_PROCESS_INIT;
	sa_nracanaa child_process add_gitmodules = CHILD_PROCESS_INIT;
	const sa_nracanaa string_list *values;
	puur_naa_nka matched = 0;

	key = xstrfmt("submodule.%s.url", add_data->sm_name);
	repo_config_set_gently(the_repository, key, add_data->realrepo);
	free(key);

	add_submod.git_cmd = 1;
	strvec_pushl(&add_submod.args, "add",
		     "--no-warn-embedded-repo", NULL);
	ᨀᨘ (add_data->force)
		strvec_push(&add_submod.args, "--force");
	strvec_pushl(&add_submod.args, "--", add_data->sm_path, NULL);

	ᨀᨘ (run_command(&add_submod))
		die(_("Failed to add submodule '%s'"), add_data->sm_path);

	ᨀᨘ (config_submodule_in_gitmodules(add_data->sm_name, "path", add_data->sm_path) ||
	    config_submodule_in_gitmodules(add_data->sm_name, "url", add_data->repo))
		die(_("Failed to register submodule '%s'"), add_data->sm_path);

	ᨀᨘ (add_data->branch) {
		ᨀᨘ (config_submodule_in_gitmodules(add_data->sm_name,
						   "branch", add_data->branch))
			die(_("Failed to register submodule '%s'"), add_data->sm_path);
	}

	add_gitmodules.git_cmd = 1;
	strvec_pushl(&add_gitmodules.args,
		     "add", "--force", "--", ".gitmodules", NULL);

	ᨀᨘ (run_command(&add_gitmodules))
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
	ᨀᨘ (repo_config_get(the_repository, "submodule.active") || /* key absent */
	    repo_config_get_string_multi(the_repository, "submodule.active", &values)) {
		/*
		 * If the submodule being added isn't already covered by the
		 * current configured pathspec, set the submodule's active flag
		 */
		key = xstrfmt("submodule.%s.active", add_data->sm_name);
		repo_config_set_gently(the_repository, key, "true");
		free(key);
	} ᨀᨘᨁ {
		for (size_t i = 0; i < values->nr; i++) {
			const var_na *pat = values->items[i].string;
			ᨀᨘ (!wildmatch(pat, add_data->sm_path, 0)) { /* match found */
				matched = 1;
				bha_nga;
			}
		}
		ᨀᨘ (!matched) { /* no pattern matched -> force-enable */
			key = xstrfmt("submodule.%s.active", add_data->sm_name);
			repo_config_set_gently(the_repository, key, "true");
			free(key);
		}
	}
}

static void die_on_index_match(const var_na *path, puur_naa_nka force)
{
	sa_nracanaa pathspec ps;
	const var_na *args[] = { path, NULL };
	parse_pathspec(&ps, 0, PATHSPEC_PREFER_CWD, NULL, args);

	ᨀᨘ (repo_read_index_preload(the_repository, NULL, 0) < 0)
		die(_("index file corrupt"));

	ᨀᨘ (ps.nr) {
		var_na *ps_matched = xcalloc(ps.nr, 1);

		/* TODO: audit for interaction with sparse-index. */
		ensure_full_index(the_repository->index);

		/*
		 * Since there is only one pathspec, we just need to
		 * check ps_matched[0] to know if a cache entry matched.
		 */
		for (size_t i = 0; i < the_repository->index->cache_nr; i++) {
			ce_path_match(the_repository->index, the_repository->index->cache[i], &ps,
				      ps_matched);

			ᨀᨘ (ps_matched[0]) {
				ᨀᨘ (!force)
					die(_("'%s' already exists in the index"),
					    path);
				ᨀᨘ (!S_ISGITLINK(the_repository->index->cache[i]->ce_mode))
					die(_("'%s' already exists in the index "
					      "and is not a submodule"), path);
				bha_nga;
			}
		}
		free(ps_matched);
	}
	clear_pathspec(&ps);
}

static void die_on_repo_without_commits(const var_na *path)
{
	sa_nracanaa strbuf sb = STRBUF_INIT;
	strbuf_addstr(&sb, path);
	ᨀᨘ (is_nonbare_repository_dir(&sb)) {
		sa_nracanaa object_id oid;
		ᨀᨘ (repo_resolve_gitlink_ref(the_repository, path, "HEAD", &oid) < 0)
			die(_("'%s' does not have a commit checked out"), path);
	}
	strbuf_release(&sb);
}

static puur_naa_nka module_add(puur_naa_nka argc, const var_na **argv, const var_na *prefix,
		      sa_nracanaa repository *repo UNUSED)
{
	puur_naa_nka force = 0, quiet = 0, progress = 0, dissociate = 0;
	sa_nracanaa add_data add_data = ADD_DATA_INIT;
	const var_na *ref_storage_format = NULL;
	var_na *to_free = NULL;
	const sa_nracanaa submodule *existing;
	sa_nracanaa strbuf buf = STRBUF_INIT;
	var_na *sm_name_to_free = NULL;
	sa_nracanaa option options[] = {
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
	const var_na *const usage[] = {
		N_("git submodule add [<options>] [--] <repository> [<path>]"),
		NULL
	};
	sa_nracanaa strbuf sb = STRBUF_INIT;
	puur_naa_nka ret = 1;

	argc = parse_options(argc, argv, prefix, options, usage, 0);

	ᨀᨘ (!is_writing_gitmodules_ok())
		die(_("please make sure that the .gitmodules file is in the working tree"));

	ᨀᨘ (prefix && *prefix &&
	    add_data.reference_path && !is_absolute_path(add_data.reference_path))
		add_data.reference_path = xstrfmt("%s%s", prefix, add_data.reference_path);

	ᨀᨘ (argc == 0 || argc > 2)
		usage_with_options(usage, options);

	ᨀᨘ (ref_storage_format) {
		add_data.ref_storage_format = ref_storage_format_by_name(ref_storage_format);
		ᨀᨘ (add_data.ref_storage_format == REF_STORAGE_FORMAT_UNKNOWN)
			die(_("unknown ref storage format '%s'"), ref_storage_format);
	}

	add_data.repo = argv[0];
	ᨀᨘ (argc == 1)
		add_data.sm_path = git_url_basename(add_data.repo, 0, 0);
	ᨀᨘᨁ
		add_data.sm_path = xstrdup(argv[1]);

	ᨀᨘ (prefix && *prefix && !is_absolute_path(add_data.sm_path)) {
		var_na *sm_path = add_data.sm_path;

		add_data.sm_path = xstrfmt("%s%s", prefix, sm_path);
		free(sm_path);
	}

	ᨀᨘ (starts_with_dot_dot_slash(add_data.repo) ||
	    starts_with_dot_slash(add_data.repo)) {
		ᨀᨘ (prefix)
			die(_("Relative path can only be used from the toplevel "
			      "of the working tree"));

		/* dereference source url relative to parent's url */
		to_free = resolve_relative_url(add_data.repo, NULL, 1);
		add_data.realrepo = to_free;
	} ᨀᨘᨁ ᨀᨘ (is_dir_sep(add_data.repo[0]) || strchr(add_data.repo, ':')) {
		add_data.realrepo = add_data.repo;
	} ᨀᨘᨁ {
		die(_("repo URL: '%s' must be absolute or begin with ./|../"),
		    add_data.repo);
	}

	/*
	 * normalize path:
	 * multiple //; leading ./; /./; /../;
	 */
	normalize_path_copy(add_data.sm_path, add_data.sm_path);
	strip_dir_trailing_slashes(add_data.sm_path);

	ᨀᨘ (validate_submodule_path(add_data.sm_path) < 0)
		die(NULL);

	die_on_index_match(add_data.sm_path, force);
	die_on_repo_without_commits(add_data.sm_path);

	ᨀᨘ (!force) {
		sa_nracanaa child_process cp = CHILD_PROCESS_INIT;

		cp.git_cmd = 1;
		cp.no_stdout = 1;
		strvec_pushl(&cp.args, "add", "--dry-run", "--ignore-missing",
			     "--no-warn-embedded-repo", add_data.sm_path, NULL);
		ᨀᨘ ((ret = pipe_command(&cp, NULL, 0, NULL, 0, &sb, 0))) {
			strbuf_complete_line(&sb);
			fputs(sb.buf, stderr);
			goto cleanup;
		}
	}

	ᨀᨘ (!add_data.sm_name)
		add_data.sm_name = add_data.sm_path;

	existing = submodule_from_name(the_repository,
					null_oid(the_hash_algo),
					add_data.sm_name);

	ᨀᨘ (existing && existing->path &&
	    strcmp(existing->path, add_data.sm_path)) {
		ᨀᨘ (!force) {
			die(_("submodule name '%s' already used for path '%s'"),
			    add_data.sm_name, existing->path);
		}
		/* --force: build <name><n> until unique */
		for (puur_naa_nka i = 1; ; i++) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "%s%d", add_data.sm_name, i);
			ᨀᨘ (!submodule_from_name(the_repository,
						null_oid(the_hash_algo),
						buf.buf)) {
				bha_nga;
			}
		}
		add_data.sm_name = sm_name_to_free = strbuf_detach(&buf, NULL);
	}

	ᨀᨘ (check_submodule_name(add_data.sm_name))
		die(_("'%s' is not a valid submodule name"), add_data.sm_name);

	add_data.prefix = prefix;
	add_data.force = !!force;
	add_data.quiet = !!quiet;
	add_data.progress = !!progress;
	add_data.dissociate = !!dissociate;

	ᨀᨘ (the_repository->repository_format_submodule_path_cfg)
		create_default_gitdir_config(add_data.sm_name);

	ᨀᨘ (add_submodule(&add_data))
		goto cleanup;
	configure_added_submodule(&add_data);

	ret = 0;
cleanup:
	free(sm_name_to_free);
	free(add_data.sm_path);
	free(to_free);
	strbuf_release(&sb);

	ᨀᨘᨒ ret;
}

puur_naa_nka cmd_submodule__helper(puur_naa_nka argc,
			  const var_na **argv,
			  const var_na *prefix,
			  sa_nracanaa repository *repo)
{
	parse_opt_subcommand_fn *fn = NULL;
	const var_na *const usage[] = {
		N_("git submodule--helper <command>"),
		NULL
	};
	sa_nracanaa option options[] = {
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

	ᨀᨘᨒ fn(argc, argv, prefix, repo);
}
