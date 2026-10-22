#define USE_THE_REPOSITORY_VARIABLE

#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "abspath.h"
#અન્તર્ભાવ "environment.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "hex.h"

#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ "parse-options.h"
#અન્તર્ભાવ "quote.h"
#અન્તર્ભાવ "path.h"
#અન્તર્ભાવ "pathspec.h"
#અન્તર્ભાવ "preload-index.h"
#અન્તર્ભાવ "dir.h"
#અન્તર્ભાવ "read-cache.h"
#અન્તર્ભાવ "setup.h"
#અન્તર્ભાવ "sparse-index.h"
#અન્તર્ભાવ "submodule.h"
#અન્તર્ભાવ "submodule-config.h"
#અન્તર્ભાવ "string-list.h"
#અન્તર્ભાવ "run-command.h"
#અન્તર્ભાવ "remote.h"
#અન્તર્ભાવ "refs.h"
#અન્તર્ભાવ "refspec.h"
#અન્તર્ભાવ "revision.h"
#અન્તર્ભાવ "diffcore.h"
#અન્તર્ભાવ "diff.h"
#અન્તર્ભાવ "object-file.h"
#અન્તર્ભાવ "object-name.h"
#અન્તર્ભાવ "odb.h"
#અન્તર્ભાવ "odb/source.h"
#અન્તર્ભાવ "odb/source-files.h"
#અન્તર્ભાવ "advice.h"
#અન્તર્ભાવ "branch.h"
#અન્તર્ભાવ "list-objects-filter-options.h"
#અન્તર્ભાવ "wildmatch.h"
#અન્તર્ભાવ "strbuf.h"
#અન્તર્ભાવ "url.h"

#define OPT_QUIET (1 << 0)
#define OPT_CACHED (1 << 1)
#define OPT_RECURSIVE (1 << 2)
#define OPT_FORCE (1 << 3)

પ્રકારવ્યાખ્યા શૂન્ય (*each_submodule_fn)(અચલ સંરચના cache_entry *list_item,
				  શૂન્ય *cb_data);

સ્થિર અક્ષર *get_default_remote(શૂન્ય)
{
	પાછા xstrdup(repo_default_remote(the_repository));
}

સ્થિર અક્ષર *resolve_relative_url(અચલ અક્ષર *rel_url, અચલ અક્ષર *up_path, પૂર્ણાંક quiet)
{
	અક્ષર *remoteurl, *resolved_url;
	અક્ષર *remote = get_default_remote();
	સંરચના strbuf remotesb = STRBUF_INIT;

	strbuf_addf(&remotesb, "remote.%s.url", remote);
	જો (repo_config_get_string(the_repository, remotesb.buf, &remoteurl)) {
		જો (!quiet)
			warning(_("could not look up configuration '%s'. "
				  "Assuming this repository is its own "
				  "authoritative upstream."),
				remotesb.buf);
		remoteurl = xgetcwd();
	}
	resolved_url = relative_url(remoteurl, rel_url, up_path);

	મુક્ત(remote);
	મુક્ત(remoteurl);
	strbuf_release(&remotesb);

	પાછા resolved_url;
}

સ્થિર પૂર્ણાંક get_default_remote_submodule(અચલ અક્ષર *module_path, અક્ષર **default_remote)
{
	અચલ સંરચના submodule *sub;
	સંરચના repository subrepo;
	અચલ અક્ષર *remote_name = ખાલી;
	અક્ષર *url = ખાલી;
	પૂર્ણાંક ret = 0;

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), module_path);
	જો (sub && sub->url) {
		url = xstrdup(sub->url);

		/* Possibly a url relative to parent */
		જો (starts_with_dot_dot_slash(url) ||
		    starts_with_dot_slash(url)) {
			અક્ષર *oldurl = url;

			url = resolve_relative_url(oldurl, ખાલી, 1);
			મુક્ત(oldurl);
		}
	}

	જો (repo_submodule_init(&subrepo, the_repository, module_path,
				null_oid(the_hash_algo)) < 0) {
		ret = die_message(_("could not get a repository handle for submodule '%s'"),
				   module_path);
		જાઓ out;
	}

	/* Look up by URL first */
	જો (url)
		remote_name = repo_remote_from_url(&subrepo, url);
	જો (!remote_name)
		remote_name = repo_default_remote(&subrepo);

	*default_remote = xstrdup(remote_name);

out:
	repo_clear(&subrepo);
	મુક્ત(url);

	પાછા ret;
}

સ્થિર પૂર્ણાંક module_get_default_remote(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
				     સંરચના repository *repo UNUSED)
{
	અચલ અક્ષર *path;
	અક્ષર *resolved_path = ખાલી;
	અક્ષર *default_remote = ખાલી;
	પૂર્ણાંક code;
	સંરચના option options[] = {
		OPT_END()
	};
	અચલ અક્ષર *અચલ usage[] = {
		N_("git submodule--helper get-default-remote <path>"),
		ખાલી
	};

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	જો (argc != 1)
		usage_with_options(usage, options);

	path = argv[0];
	જો (prefix && *prefix && !is_absolute_path(path)) {
		resolved_path = xstrfmt("%s%s", prefix, path);
		path = resolved_path;
	}

	code = get_default_remote_submodule(path, &default_remote);
	જો (code) {
		મુક્ત(resolved_path);
		પાછા code;
	}

	printf("%s\n", default_remote);
	મુક્ત(default_remote);
	મુક્ત(resolved_path);
	પાછા 0;
}

/* the result should be freed by the caller. */
સ્થિર અક્ષર *get_submodule_displaypath(અચલ અક્ષર *path, અચલ અક્ષર *prefix,
				       અચલ અક્ષર *super_prefix)
{
	જો (prefix && super_prefix) {
		BUG("cannot have prefix '%s' and superprefix '%s'",
		    prefix, super_prefix);
	} નહીંતર જો (prefix) {
		સંરચના strbuf sb = STRBUF_INIT;
		અક્ષર *displaypath = xstrdup(relative_path(path, prefix, &sb));
		strbuf_release(&sb);
		પાછા displaypath;
	} નહીંતર જો (super_prefix) {
		પાછા xstrfmt("%s%s", super_prefix, path);
	} નહીંતર {
		પાછા xstrdup(path);
	}
}

સ્થિર અક્ષર *compute_rev_name(અચલ અક્ષર *sub_path, અચલ અક્ષર* object_id)
{
	સંરચના strbuf sb = STRBUF_INIT;
	અચલ અક્ષર ***d;

	સ્થિર અચલ અક્ષર *describe_bare[] = { ખાલી };

	સ્થિર અચલ અક્ષર *describe_tags[] = { "--tags", ખાલી };

	સ્થિર અચલ અક્ષર *describe_contains[] = { "--contains", ખાલી };

	સ્થિર અચલ અક્ષર *describe_all_always[] = { "--all", "--always", ખાલી };

	સ્થિર અચલ અક્ષર **describe_argv[] = { describe_bare, describe_tags,
						describe_contains,
						describe_all_always, ખાલી };

	માટે (d = describe_argv; *d; d++) {
		સંરચના child_process cp = CHILD_PROCESS_INIT;
		prepare_submodule_repo_env(&cp.env);
		cp.dir = sub_path;
		cp.git_cmd = 1;
		cp.no_stderr = 1;

		strvec_ધકેલો(&cp.args, "describe");
		strvec_pushv(&cp.args, *d);
		strvec_ધકેલો(&cp.args, object_id);

		જો (!capture_command(&cp, &sb, 0)) {
			strbuf_strip_suffix(&sb, "\n");
			પાછા strbuf_detach(&sb, ખાલી);
		}
	}

	strbuf_release(&sb);
	પાછા ખાલી;
}

સંરચના module_list {
	અચલ સંરચના cache_entry **entries;
	પૂર્ણાંક alloc, nr;
};
#define MODULE_LIST_INIT { 0 }

સ્થિર શૂન્ય module_list_release(સંરચના module_list *ml)
{
	મુક્ત(ml->entries);
}

સ્થિર પૂર્ણાંક module_list_compute(અચલ અક્ષર **argv,
			       અચલ અક્ષર *prefix,
			       સંરચના pathspec *pathspec,
			       સંરચના module_list *list)
{
	પૂર્ણાંક result = 0;
	અક્ષર *ps_matched = ખાલી;

	parse_pathspec(pathspec, 0,
		       PATHSPEC_PREFER_FULL,
		       prefix, argv);

	જો (pathspec->nr)
		ps_matched = xcalloc(pathspec->nr, 1);

	જો (repo_read_index(the_repository) < 0)
		die(_("index file corrupt"));

	માટે (size_t i = 0; i < the_repository->index->cache_nr; i++) {
		અચલ સંરચના cache_entry *ce = the_repository->index->cache[i];

		જો (!match_pathspec(the_repository->index, pathspec, ce->name, ce_namelen(ce),
				    0, ps_matched, 1) ||
		    !S_ISGITLINK(ce->ce_mode))
			ચાલુ;

		ALLOC_GROW(list->entries, list->nr + 1, list->alloc);
		list->entries[list->nr++] = ce;
		જ્યારે (i + 1 < the_repository->index->cache_nr &&
		       !strcmp(ce->name, the_repository->index->cache[i + 1]->name))
			/*
			 * Skip entries with the same name in different stages
			 * to make sure an entry is returned only once.
			 */
			i++;
	}

	જો (ps_matched && report_path_error(ps_matched, pathspec))
		result = -1;

	મુક્ત(ps_matched);

	પાછા result;
}

સ્થિર શૂન્ય module_list_active(સંરચના module_list *list)
{
	પૂર્ણાંક i;
	સંરચના module_list active_modules = MODULE_LIST_INIT;

	માટે (i = 0; i < list->nr; i++) {
		અચલ સંરચના cache_entry *ce = list->entries[i];

		જો (!is_submodule_active(the_repository, ce->name))
			ચાલુ;

		ALLOC_GROW(active_modules.entries,
			   active_modules.nr + 1,
			   active_modules.alloc);
		active_modules.entries[active_modules.nr++] = ce;
	}

	module_list_release(list);
	*list = active_modules;
}

સ્થિર અક્ષર *get_up_path(અચલ અક્ષર *path)
{
	સંરચના strbuf sb = STRBUF_INIT;

	strbuf_addstrings(&sb, "../", ગણતરી_slashes(path));

	/*
	 * Check if 'path' ends with slash or not
	 * for having the same output for dir/sub_dir
	 * and dir/sub_dir/
	 */
	જો (!is_dir_sep(path[strlen(path) - 1]))
		strbuf_addstr(&sb, "../");

	પાછા strbuf_detach(&sb, ખાલી);
}

સ્થિર શૂન્ય for_each_listed_submodule(અચલ સંરચના module_list *list,
				      each_submodule_fn fn, શૂન્ય *cb_data)
{
	પૂર્ણાંક i;

	માટે (i = 0; i < list->nr; i++)
		fn(list->entries[i], cb_data);
}

સંરચના foreach_cb {
	પૂર્ણાંક argc;
	અચલ અક્ષર **argv;
	અચલ અક્ષર *prefix;
	અચલ અક્ષર *super_prefix;
	પૂર્ણાંક quiet;
	પૂર્ણાંક recursive;
};
#define FOREACH_CB_INIT { 0 }

સ્થિર શૂન્ય runcommand_in_submodule_cb(અચલ સંરચના cache_entry *list_item,
				       શૂન્ય *cb_data)
{
	સંરચના foreach_cb *info = cb_data;
	અચલ અક્ષર *path = list_item->name;
	અચલ સંરચના object_id *ce_oid = &list_item->oid;
	અચલ સંરચના submodule *sub;
	સંરચના child_process cp = CHILD_PROCESS_INIT;
	અક્ષર *displaypath;

	જો (validate_submodule_path(path) < 0)
		die(ખાલી);

	displaypath = get_submodule_displaypath(path, info->prefix,
						info->super_prefix);

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);

	જો (!sub)
		die(_("No url found for submodule path '%s' in .gitmodules"),
			displaypath);

	જો (!is_submodule_populated_gently(path, ખાલી))
		જાઓ cleanup;

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
	જો (info->argc == 1) {
		અક્ષર *toplevel = xgetcwd();
		સંરચના strbuf sb = STRBUF_INIT;

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
		મુક્ત(toplevel);
	} નહીંતર {
		strvec_pushv(&cp.args, info->argv);
	}

	જો (!info->quiet)
		printf(_("Entering '%s'\n"), displaypath);

	જો (info->argv[0]) {
		જો (run_command(&cp))
			die(_("run_command returned non-zero status for %s\n."),
			    displaypath);
	} નહીંતર {
		child_process_clear(&cp);
	}

	જો (info->recursive) {
		સંરચના child_process cpr = CHILD_PROCESS_INIT;

		cpr.git_cmd = 1;
		cpr.dir = path;
		prepare_submodule_repo_env(&cpr.env);

		strvec_pushl(&cpr.args, "submodule--helper", "foreach", "--recursive",
			     ખાલી);
		strvec_pushf(&cpr.args, "--super-prefix=%s/", displaypath);

		જો (info->quiet)
			strvec_ધકેલો(&cpr.args, "--quiet");

		strvec_ધકેલો(&cpr.args, "--");
		strvec_pushv(&cpr.args, info->argv);

		જો (run_command(&cpr))
			die(_("run_command returned non-zero status while "
				"recursing in the nested submodules of %s\n."),
				displaypath);
	}

cleanup:
	મુક્ત(displaypath);
}

સ્થિર પૂર્ણાંક module_foreach(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
			  સંરચના repository *repo UNUSED)
{
	સંરચના foreach_cb info = FOREACH_CB_INIT;
	સંરચના pathspec pathspec = { 0 };
	સંરચના module_list list = MODULE_LIST_INIT;
	સંરચના option module_foreach_options[] = {
		OPT__SUPER_PREFIX(&info.super_prefix),
		OPT__QUIET(&info.quiet, N_("suppress output of entering each submodule command")),
		OPT_BOOL(0, "recursive", &info.recursive,
			 N_("recurse into nested submodules")),
		OPT_END()
	};
	અચલ અક્ષર *અચલ git_submodule_helper_usage[] = {
		N_("git submodule foreach [--quiet] [--recursive] [--] <command>"),
		ખાલી
	};
	પૂર્ણાંક ret = 1;

	argc = parse_options(argc, argv, prefix, module_foreach_options,
			     git_submodule_helper_usage, 0);

	જો (module_list_compute(ખાલી, prefix, &pathspec, &list) < 0)
		જાઓ cleanup;

	info.argc = argc;
	info.argv = argv;
	info.prefix = prefix;

	for_each_listed_submodule(&list, runcommand_in_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	પાછા ret;
}

સંરચના init_cb {
	અચલ અક્ષર *prefix;
	અચલ અક્ષર *super_prefix;
	unsigned પૂર્ણાંક flags;
};
#define INIT_CB_INIT { 0 }

સ્થિર પૂર્ણાંક validate_and_set_submodule_gitdir(સંરચના strbuf *gitdir_path,
					     અચલ અક્ષર *submodule_name)
{
	અચલ અક્ષર *કિંમત;
	અક્ષર *ચાવી;

	જો (validate_submodule_git_dir(gitdir_path->buf, submodule_name))
		પાછા -1;

	ચાવી = xstrfmt("submodule.%s.gitdir", submodule_name);

	/* Nothing to do if the config already exists. */
	જો (!repo_config_get_string_tmp(the_repository, ચાવી, &કિંમત)) {
		મુક્ત(ચાવી);
		પાછા 0;
	}

	જો (repo_config_set_gently(the_repository, ચાવી, gitdir_path->buf)) {
		મુક્ત(ચાવી);
		પાછા -1;
	}

	મુક્ત(ચાવી);
	પાછા 0;
}

સ્થિર શૂન્ય create_default_gitdir_config(અચલ અક્ષર *submodule_name)
{
	સંરચના strbuf gitdir_path = STRBUF_INIT;
	સંરચના git_hash_ctx ctx;
	અક્ષર hex_name_hash[GIT_MAX_HEXSZ + 1], header[128];
	unsigned અક્ષર raw_name_hash[GIT_MAX_RAWSZ];
	પૂર્ણાંક header_len;

	/* Case 1: try the plain module name */
	repo_git_path_append(the_repository, &gitdir_path, "modules/%s", submodule_name);
	જો (!validate_and_set_submodule_gitdir(&gitdir_path, submodule_name)) {
		strbuf_release(&gitdir_path);
		પાછા;
	}

	/* Case 2.1: Try URI-safe (RFC3986) encoding first, this fixes nested gitdirs */
	strbuf_reset(&gitdir_path);
	repo_git_path_append(the_repository, &gitdir_path, "modules/");
	strbuf_addstr_urlencode(&gitdir_path, submodule_name, is_rfc3986_unreserved);
	જો (!validate_and_set_submodule_gitdir(&gitdir_path, submodule_name)) {
		strbuf_release(&gitdir_path);
		પાછા;
	}

	/* Case 2.2: Try extended uppercase URI (RFC3986) encoding, to fix case-folding */
	strbuf_reset(&gitdir_path);
	repo_git_path_append(the_repository, &gitdir_path, "modules/");
	strbuf_addstr_urlencode(&gitdir_path, submodule_name, is_casefolding_rfc3986_unreserved);
	જો (!validate_and_set_submodule_gitdir(&gitdir_path, submodule_name))
		પાછા;

	/* Case 2.3: Try some derived gitdir names, see if one sticks */
	માટે (અક્ષર c = '0'; c <= '9'; c++) {
		strbuf_reset(&gitdir_path);
		repo_git_path_append(the_repository, &gitdir_path, "modules/");
		strbuf_addstr_urlencode(&gitdir_path, submodule_name, is_rfc3986_unreserved);
		strbuf_addch(&gitdir_path, c);
		જો (!validate_and_set_submodule_gitdir(&gitdir_path, submodule_name))
			પાછા;

		strbuf_reset(&gitdir_path);
		repo_git_path_append(the_repository, &gitdir_path, "modules/");
		strbuf_addstr_urlencode(&gitdir_path, submodule_name, is_casefolding_rfc3986_unreserved);
		strbuf_addch(&gitdir_path, c);
		જો (!validate_and_set_submodule_gitdir(&gitdir_path, submodule_name))
			પાછા;
	}

	/* Case 2.4: If all the above failed, try a hash of the name as a last resort */
	header_len = snprintf(header, કદ(header),
			      "blob %"PRIuMAX, (uintmax_t)strlen(submodule_name));
	git_hash_init(&ctx, the_hash_algo);
	git_hash_update(&ctx, header, header_len);
	git_hash_update(&ctx, "\0", 1);
	git_hash_update(&ctx, submodule_name, strlen(submodule_name));
	git_hash_final(raw_name_hash, &ctx);
	hash_to_hex_algop_r(hex_name_hash, raw_name_hash, the_hash_algo);
	strbuf_reset(&gitdir_path);
	repo_git_path_append(the_repository, &gitdir_path, "modules/%s", hex_name_hash);
	જો (!validate_and_set_submodule_gitdir(&gitdir_path, submodule_name)) {
		strbuf_release(&gitdir_path);
		પાછા;
	}

	/* Case 3: nothing worked, error out */
	die(_("failed to set a valid default config for 'submodule.%s.gitdir'. "
	      "Please ensure it is set, for example by running something like: "
	      "'git config submodule.%s.gitdir .git/modules/%s'"),
	    submodule_name, submodule_name, submodule_name);
}

સ્થિર શૂન્ય init_submodule(અચલ અક્ષર *path, અચલ અક્ષર *prefix,
			   અચલ અક્ષર *super_prefix,
			   unsigned પૂર્ણાંક flags)
{
	અચલ સંરચના submodule *sub;
	સંરચના strbuf sb = STRBUF_INIT;
	અચલ અક્ષર *upd;
	અક્ષર *url = ખાલી, *displaypath;

	displaypath = get_submodule_displaypath(path, prefix, super_prefix);

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);

	જો (!sub)
		die(_("No url found for submodule path '%s' in .gitmodules"),
			displaypath);

	/*
	 * NEEDSWORK: In a multi-working-tree world, this needs to be
	 * set in the per-worktree config.
	 *
	 * Set active flag for the submodule being initialized
	 */
	જો (!is_submodule_active(the_repository, path)) {
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
	જો (repo_config_get_string(the_repository, sb.buf, &url)) {
		જો (!sub->url)
			die(_("No url found for submodule path '%s' in .gitmodules"),
				displaypath);

		url = xstrdup(sub->url);

		/* Possibly a url relative to parent */
		જો (starts_with_dot_dot_slash(url) ||
		    starts_with_dot_slash(url)) {
			અક્ષર *oldurl = url;

			url = resolve_relative_url(oldurl, ખાલી, 0);
			મુક્ત(oldurl);
		}

		જો (repo_config_set_gently(the_repository, sb.buf, url))
			die(_("Failed to register url for submodule path '%s'"),
			    displaypath);
		જો (!(flags & OPT_QUIET))
			fprintf(stderr,
				_("Submodule '%s' (%s) registered for path '%s'\n"),
				sub->name, url, displaypath);
	}
	strbuf_reset(&sb);

	/* Copy "update" setting when it is not set yet */
	strbuf_addf(&sb, "submodule.%s.update", sub->name);
	જો (repo_config_get_string_tmp(the_repository, sb.buf, &upd) &&
	    sub->update_strategy.type != SM_UPDATE_UNSPECIFIED) {
		જો (sub->update_strategy.type == SM_UPDATE_COMMAND) {
			fprintf(stderr, _("warning: command update mode suggested for submodule '%s'\n"),
				sub->name);
			upd = "none";
		} નહીંતર {
			upd = submodule_update_type_to_string(sub->update_strategy.type);
		}

		જો (repo_config_set_gently(the_repository, sb.buf, upd))
			die(_("Failed to register update mode for submodule path '%s'"), displaypath);
	}

	જો (the_repository->repository_format_submodule_path_cfg)
		create_default_gitdir_config(sub->name);

	strbuf_release(&sb);
	મુક્ત(displaypath);
	મુક્ત(url);
}

સ્થિર શૂન્ય init_submodule_cb(અચલ સંરચના cache_entry *list_item, શૂન્ય *cb_data)
{
	સંરચના init_cb *info = cb_data;

	init_submodule(list_item->name, info->prefix, info->super_prefix,
		       info->flags);
}

સ્થિર પૂર્ણાંક module_init(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
		       સંરચના repository *repo UNUSED)
{
	સંરચના init_cb info = INIT_CB_INIT;
	સંરચના pathspec pathspec = { 0 };
	સંરચના module_list list = MODULE_LIST_INIT;
	પૂર્ણાંક quiet = 0;
	સંરચના option module_init_options[] = {
		OPT__QUIET(&quiet, N_("suppress output for initializing a submodule")),
		OPT_END()
	};
	અચલ અક્ષર *અચલ git_submodule_helper_usage[] = {
		N_("git submodule init [<options>] [<path>]"),
		ખાલી
	};
	પૂર્ણાંક ret = 1;

	argc = parse_options(argc, argv, prefix, module_init_options,
			     git_submodule_helper_usage, 0);

	જો (module_list_compute(argv, prefix, &pathspec, &list) < 0)
		જાઓ cleanup;

	/*
	 * If there are no path args and submodule.active is set then,
	 * by default, only initialize 'active' modules.
	 */
	જો (!argc && !repo_config_get(the_repository, "submodule.active"))
		module_list_active(&list);

	info.prefix = prefix;
	જો (quiet)
		info.flags |= OPT_QUIET;

	for_each_listed_submodule(&list, init_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	પાછા ret;
}

સંરચના status_cb {
	અચલ અક્ષર *prefix;
	અચલ અક્ષર *super_prefix;
	unsigned પૂર્ણાંક flags;
};
#define STATUS_CB_INIT { 0 }

સ્થિર શૂન્ય print_status(unsigned પૂર્ણાંક flags, અક્ષર state, અચલ અક્ષર *path,
			 અચલ સંરચના object_id *oid, અચલ અક્ષર *displaypath)
{
	જો (flags & OPT_QUIET)
		પાછા;

	printf("%c%s %s", state, oid_to_hex(oid), displaypath);

	જો (state == ' ' || state == '+') {
		અક્ષર *name = compute_rev_name(path, oid_to_hex(oid));

		જો (name)
			printf(" (%s)", name);
		મુક્ત(name);
	}

	printf("\n");
}

સ્થિર પૂર્ણાંક handle_submodule_head_ref(અચલ સંરચના reference *ref, શૂન્ય *cb_data)
{
	સંરચના object_id *output = cb_data;

	જો (ref->oid)
		oidcpy(output, ref->oid);

	પાછા 0;
}

સ્થિર શૂન્ય status_submodule(અચલ અક્ષર *path, અચલ સંરચના object_id *ce_oid,
			     unsigned પૂર્ણાંક ce_flags, અચલ અક્ષર *prefix,
			     અચલ અક્ષર *super_prefix, unsigned પૂર્ણાંક flags)
{
	અક્ષર *displaypath;
	સંરચના strvec diff_files_args = STRVEC_INIT;
	સંરચના rev_info rev = REV_INFO_INIT;
	સંરચના strbuf buf = STRBUF_INIT;
	અચલ અક્ષર *git_dir;

	જો (validate_submodule_path(path) < 0)
		die(ખાલી);

	જો (!submodule_from_path(the_repository, null_oid(the_hash_algo), path))
		die(_("no submodule mapping found in .gitmodules for path '%s'"),
		      path);

	displaypath = get_submodule_displaypath(path, prefix, super_prefix);

	જો ((CE_STAGEMASK & ce_flags) >> CE_STAGESHIFT) {
		print_status(flags, 'U', path, null_oid(the_hash_algo), displaypath);
		જાઓ cleanup;
	}

	strbuf_addf(&buf, "%s/.git", path);
	git_dir = read_gitfile(buf.buf);
	જો (!git_dir)
		git_dir = buf.buf;

	જો (!is_submodule_active(the_repository, path) ||
	    !is_git_directory(git_dir)) {
		print_status(flags, '-', path, ce_oid, displaypath);
		strbuf_release(&buf);
		જાઓ cleanup;
	}
	strbuf_release(&buf);

	strvec_pushl(&diff_files_args, "diff-files",
		     "--ignore-submodules=dirty", "--quiet", "--",
		     path, ખાલી);

	repo_config(the_repository, git_diff_basic_config, ખાલી);

	repo_init_revisions(the_repository, &rev, ખાલી);
	rev.abbrev = 0;
	setup_revisions_from_strvec(&diff_files_args, &rev, ખાલી);
	run_diff_files(&rev, 0);

	જો (!diff_result_code(&rev)) {
		print_status(flags, ' ', path, ce_oid,
			     displaypath);
	} નહીંતર જો (!(flags & OPT_CACHED)) {
		સંરચના object_id oid;
		સંરચના ref_store *refs = repo_get_submodule_ref_store(the_repository,
								      path);

		જો (!refs) {
			print_status(flags, '-', path, ce_oid, displaypath);
			જાઓ cleanup;
		}
		જો (refs_head_ref(refs, handle_submodule_head_ref, &oid))
			die(_("could not resolve HEAD ref inside the "
			      "submodule '%s'"), path);

		print_status(flags, '+', path, &oid, displaypath);
	} નહીંતર {
		print_status(flags, '+', path, ce_oid, displaypath);
	}

	જો (flags & OPT_RECURSIVE) {
		સંરચના child_process cpr = CHILD_PROCESS_INIT;
		પૂર્ણાંક res;

		cpr.git_cmd = 1;
		cpr.dir = path;
		prepare_submodule_repo_env(&cpr.env);

		strvec_pushl(&cpr.args, "submodule--helper", "status",
			     "--recursive", ખાલી);
		strvec_pushf(&cpr.args, "--super-prefix=%s/", displaypath);

		જો (flags & OPT_CACHED)
			strvec_ધકેલો(&cpr.args, "--cached");

		જો (flags & OPT_QUIET)
			strvec_ધકેલો(&cpr.args, "--quiet");

		res = run_command(&cpr);
		જો (res == SIGPIPE + 128)
			ઉઠાવો(SIGPIPE);
		નહીંતર જો (res)
			die(_("failed to recurse into submodule '%s'"), path);
	}

cleanup:
	strvec_clear(&diff_files_args);
	મુક્ત(displaypath);
	release_revisions(&rev);
}

સ્થિર શૂન્ય status_submodule_cb(અચલ સંરચના cache_entry *list_item,
				શૂન્ય *cb_data)
{
	સંરચના status_cb *info = cb_data;

	status_submodule(list_item->name, &list_item->oid, list_item->ce_flags,
			 info->prefix, info->super_prefix, info->flags);
}

સ્થિર પૂર્ણાંક module_status(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
			 સંરચના repository *repo UNUSED)
{
	સંરચના status_cb info = STATUS_CB_INIT;
	સંરચના pathspec pathspec = { 0 };
	સંરચના module_list list = MODULE_LIST_INIT;
	પૂર્ણાંક quiet = 0;
	સંરચના option module_status_options[] = {
		OPT__SUPER_PREFIX(&info.super_prefix),
		OPT__QUIET(&quiet, N_("suppress submodule status output")),
		OPT_BIT(0, "cached", &info.flags, N_("use commit stored in the index instead of the one stored in the submodule HEAD"), OPT_CACHED),
		OPT_BIT(0, "recursive", &info.flags, N_("recurse into nested submodules"), OPT_RECURSIVE),
		OPT_END()
	};
	અચલ અક્ષર *અચલ git_submodule_helper_usage[] = {
		N_("git submodule status [--quiet] [--cached] [--recursive] [<path>...]"),
		ખાલી
	};
	પૂર્ણાંક ret = 1;

	argc = parse_options(argc, argv, prefix, module_status_options,
			     git_submodule_helper_usage, 0);

	જો (module_list_compute(argv, prefix, &pathspec, &list) < 0)
		જાઓ cleanup;

	info.prefix = prefix;
	જો (quiet)
		info.flags |= OPT_QUIET;

	for_each_listed_submodule(&list, status_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	પાછા ret;
}

સંરચના module_cb {
	unsigned પૂર્ણાંક mod_src;
	unsigned પૂર્ણાંક mod_dst;
	સંરચના object_id oid_src;
	સંરચના object_id oid_dst;
	અક્ષર status;
	અક્ષર *sm_path;
};
#define MODULE_CB_INIT { 0 }

સ્થિર શૂન્ય module_cb_release(સંરચના module_cb *mcb)
{
	મુક્ત(mcb->sm_path);
}

સંરચના module_cb_list {
	સંરચના module_cb **entries;
	પૂર્ણાંક alloc, nr;
};
#define MODULE_CB_LIST_INIT { 0 }

સ્થિર શૂન્ય module_cb_list_release(સંરચના module_cb_list *mcbl)
{
	પૂર્ણાંક i;

	માટે (i = 0; i < mcbl->nr; i++) {
		સંરચના module_cb *mcb = mcbl->entries[i];

		module_cb_release(mcb);
		મુક્ત(mcb);
	}
	મુક્ત(mcbl->entries);
}

સંરચના summary_cb {
	પૂર્ણાંક argc;
	અચલ અક્ષર **argv;
	અચલ અક્ષર *prefix;
	અચલ અક્ષર *super_prefix;
	unsigned પૂર્ણાંક cached: 1;
	unsigned પૂર્ણાંક for_status: 1;
	unsigned પૂર્ણાંક files: 1;
	પૂર્ણાંક summary_limit;
};
#define SUMMARY_CB_INIT { 0 }

ગણના diff_cmd {
	DIFF_INDEX,
	DIFF_FILES
};

સ્થિર અક્ષર *verify_submodule_committish(અચલ અક્ષર *sm_path,
					 અચલ અક્ષર *committish)
{
	સંરચના child_process cp_rev_parse = CHILD_PROCESS_INIT;
	સંરચના strbuf result = STRBUF_INIT;

	cp_rev_parse.git_cmd = 1;
	cp_rev_parse.dir = sm_path;
	prepare_submodule_repo_env(&cp_rev_parse.env);
	strvec_pushl(&cp_rev_parse.args, "rev-parse", "-q", "--short", ખાલી);
	strvec_pushf(&cp_rev_parse.args, "%s^0", committish);
	strvec_ધકેલો(&cp_rev_parse.args, "--");

	જો (capture_command(&cp_rev_parse, &result, 0))
		પાછા ખાલી;

	strbuf_trim_trailing_newline(&result);
	પાછા strbuf_detach(&result, ખાલી);
}

સ્થિર શૂન્ય print_submodule_summary(સંરચના summary_cb *info, અચલ અક્ષર *errmsg,
				    પૂર્ણાંક total_commits, અચલ અક્ષર *displaypath,
				    અચલ અક્ષર *src_abbrev, અચલ અક્ષર *dst_abbrev,
				    સંરચના module_cb *p)
{
	જો (p->status == 'T') {
		જો (S_ISGITLINK(p->mod_dst))
			printf(_("* %s %s(blob)->%s(submodule)"),
				 displaypath, src_abbrev, dst_abbrev);
		નહીંતર
			printf(_("* %s %s(submodule)->%s(blob)"),
				 displaypath, src_abbrev, dst_abbrev);
	} નહીંતર {
		printf("* %s %s...%s",
			displaypath, src_abbrev, dst_abbrev);
	}

	જો (total_commits < 0)
		printf(":\n");
	નહીંતર
		printf(" (%d):\n", total_commits);

	જો (errmsg) {
		printf(_("%s"), errmsg);
	} નહીંતર જો (total_commits > 0) {
		સંરચના child_process cp_log = CHILD_PROCESS_INIT;

		cp_log.git_cmd = 1;
		cp_log.dir = p->sm_path;
		prepare_submodule_repo_env(&cp_log.env);
		strvec_pushl(&cp_log.args, "log", ખાલી);

		જો (S_ISGITLINK(p->mod_src) && S_ISGITLINK(p->mod_dst)) {
			જો (info->summary_limit > 0)
				strvec_pushf(&cp_log.args, "-%d",
					     info->summary_limit);

			strvec_pushl(&cp_log.args, "--pretty=  %m %s",
				     "--first-parent", ખાલી);
			strvec_pushf(&cp_log.args, "%s...%s",
				     src_abbrev, dst_abbrev);
		} નહીંતર જો (S_ISGITLINK(p->mod_dst)) {
			strvec_pushl(&cp_log.args, "--pretty=  > %s",
				     "-1", dst_abbrev, ખાલી);
		} નહીંતર {
			strvec_pushl(&cp_log.args, "--pretty=  < %s",
				     "-1", src_abbrev, ખાલી);
		}
		run_command(&cp_log);
	}
	printf("\n");
}

સ્થિર શૂન્ય generate_submodule_summary(સંરચના summary_cb *info,
				       સંરચના module_cb *p)
{
	અક્ષર *displaypath, *src_abbrev = ખાલી, *dst_abbrev;
	પૂર્ણાંક missing_src = 0, missing_dst = 0;
	સંરચના strbuf errmsg = STRBUF_INIT;
	પૂર્ણાંક total_commits = -1;

	જો (!info->cached && oideq(&p->oid_dst, null_oid(the_hash_algo))) {
		જો (S_ISGITLINK(p->mod_dst)) {
			સંરચના ref_store *refs = repo_get_submodule_ref_store(the_repository,
									      p->sm_path);

			જો (refs)
				refs_head_ref(refs, handle_submodule_head_ref, &p->oid_dst);
		} નહીંતર જો (S_ISLNK(p->mod_dst) || S_ISREG(p->mod_dst)) {
			સંરચના stat st;
			પૂર્ણાંક fd = open(p->sm_path, O_RDONLY);

			જો (fd < 0 || fstat(fd, &st) < 0 ||
			    index_fd(the_repository->index, &p->oid_dst, fd, &st, OBJ_BLOB,
				     p->sm_path, 0))
				error(_("couldn't hash object from '%s'"), p->sm_path);
		} નહીંતર {
			/* for a submodule removal (mode:0000000), don't warn */
			જો (p->mod_dst)
				warning(_("unexpected mode %o"), p->mod_dst);
		}
	}

	જો (S_ISGITLINK(p->mod_src)) {
		જો (p->status != 'D')
			src_abbrev = verify_submodule_committish(p->sm_path,
								 oid_to_hex(&p->oid_src));
		જો (!src_abbrev) {
			missing_src = 1;
			/*
			 * As `rev-parse` failed, we fallback to getting
			 * the abbreviated hash using oid_src. We do
			 * this as we might still need the abbreviated
			 * hash in cases like a submodule type change, etc.
			 */
			src_abbrev = xstrndup(oid_to_hex(&p->oid_src), 7);
		}
	} નહીંતર {
		/*
		 * The source does not point to a submodule.
		 * So, we fallback to getting the abbreviation using
		 * oid_src as we might still need the abbreviated
		 * hash in cases like submodule add, etc.
		 */
		src_abbrev = xstrndup(oid_to_hex(&p->oid_src), 7);
	}

	જો (S_ISGITLINK(p->mod_dst)) {
		dst_abbrev = verify_submodule_committish(p->sm_path,
							 oid_to_hex(&p->oid_dst));
		જો (!dst_abbrev) {
			missing_dst = 1;
			/*
			 * As `rev-parse` failed, we fallback to getting
			 * the abbreviated hash using oid_dst. We do
			 * this as we might still need the abbreviated
			 * hash in cases like a submodule type change, etc.
			 */
			dst_abbrev = xstrndup(oid_to_hex(&p->oid_dst), 7);
		}
	} નહીંતર {
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

	જો (!missing_src && !missing_dst) {
		સંરચના child_process cp_rev_list = CHILD_PROCESS_INIT;
		સંરચના strbuf sb_rev_list = STRBUF_INIT;

		strvec_pushl(&cp_rev_list.args, "rev-list",
			     "--first-parent", "--count", ખાલી);
		જો (S_ISGITLINK(p->mod_src) && S_ISGITLINK(p->mod_dst))
			strvec_pushf(&cp_rev_list.args, "%s...%s",
				     src_abbrev, dst_abbrev);
		નહીંતર
			strvec_ધકેલો(&cp_rev_list.args, S_ISGITLINK(p->mod_src) ?
				    src_abbrev : dst_abbrev);
		strvec_ધકેલો(&cp_rev_list.args, "--");

		cp_rev_list.git_cmd = 1;
		cp_rev_list.dir = p->sm_path;
		prepare_submodule_repo_env(&cp_rev_list.env);

		જો (!capture_command(&cp_rev_list, &sb_rev_list, 0))
			total_commits = atoi(sb_rev_list.buf);

		strbuf_release(&sb_rev_list);
	} નહીંતર {
		/*
		 * Don't give error msg for modification whose dst is not
		 * submodule, i.e., deleted or changed to blob
		 */
		જો (S_ISGITLINK(p->mod_dst)) {
			જો (missing_src && missing_dst) {
				strbuf_addf(&errmsg, "  Warn: %s doesn't contain commits %s and %s\n",
					    displaypath, oid_to_hex(&p->oid_src),
					    oid_to_hex(&p->oid_dst));
			} નહીંતર {
				strbuf_addf(&errmsg, "  Warn: %s doesn't contain commit %s\n",
					    displaypath, missing_src ?
					    oid_to_hex(&p->oid_src) :
					    oid_to_hex(&p->oid_dst));
			}
		}
	}

	print_submodule_summary(info, errmsg.len ? errmsg.buf : ખાલી,
				total_commits, displaypath, src_abbrev,
				dst_abbrev, p);

	મુક્ત(displaypath);
	મુક્ત(src_abbrev);
	મુક્ત(dst_abbrev);
	strbuf_release(&errmsg);
}

સ્થિર શૂન્ય prepare_submodule_summary(સંરચના summary_cb *info,
				      સંરચના module_cb_list *list)
{
	પૂર્ણાંક i;
	માટે (i = 0; i < list->nr; i++) {
		અચલ સંરચના submodule *sub;
		સંરચના module_cb *p = list->entries[i];
		સંરચના strbuf sm_gitdir = STRBUF_INIT;

		જો (p->status == 'D' || p->status == 'T') {
			generate_submodule_summary(info, p);
			ચાલુ;
		}

		જો (info->for_status && p->status != 'A' &&
		    (sub = submodule_from_path(the_repository,
					       null_oid(the_hash_algo), p->sm_path))) {
			અક્ષર *config_ચાવી = ખાલી;
			અચલ અક્ષર *કિંમત;
			પૂર્ણાંક ignore_all = 0;

			config_ચાવી = xstrfmt("submodule.%s.ignore",
					     sub->name);
			જો (!repo_config_get_string_tmp(the_repository, config_ચાવી, &કિંમત))
				ignore_all = !strcmp(કિંમત, "all");
			નહીંતર જો (sub->ignore)
				ignore_all = !strcmp(sub->ignore, "all");

			મુક્ત(config_ચાવી);
			જો (ignore_all)
				ચાલુ;
		}

		/* Also show added or modified modules which are checked out */
		strbuf_addstr(&sm_gitdir, p->sm_path);
		જો (is_nonbare_repository_dir(&sm_gitdir))
			generate_submodule_summary(info, p);
		strbuf_release(&sm_gitdir);
	}
}

સ્થિર શૂન્ય submodule_summary_callback(સંરચના diff_queue_struct *q,
				       સંરચના diff_options *options UNUSED,
				       શૂન્ય *data)
{
	પૂર્ણાંક i;
	સંરચના module_cb_list *list = data;
	માટે (i = 0; i < q->nr; i++) {
		સંરચના diff_filepair *p = q->queue[i];
		સંરચના module_cb *temp;

		જો (!S_ISGITLINK(p->one->mode) && !S_ISGITLINK(p->two->mode))
			ચાલુ;
		temp = xmalloc(કદ(*temp));
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

સ્થિર અચલ અક્ષર *get_diff_cmd(ગણના diff_cmd diff_cmd)
{
	પસંદગી (diff_cmd) {
	સ્થિતિ DIFF_INDEX: પાછા "diff-index";
	સ્થિતિ DIFF_FILES: પાછા "diff-files";
	મૂળભૂત: BUG("bad diff_cmd value %d", diff_cmd);
	}
}

સ્થિર પૂર્ણાંક compute_summary_module_list(સંરચના object_id *head_oid,
				       સંરચના summary_cb *info,
				       ગણના diff_cmd diff_cmd)
{
	સંરચના strvec diff_args = STRVEC_INIT;
	સંરચના rev_info rev;
	સંરચના module_cb_list list = MODULE_CB_LIST_INIT;
	પૂર્ણાંક ret = 0;

	strvec_ધકેલો(&diff_args, get_diff_cmd(diff_cmd));
	જો (info->cached)
		strvec_ધકેલો(&diff_args, "--cached");
	strvec_pushl(&diff_args, "--ignore-submodules=dirty", "--raw", ખાલી);
	જો (head_oid)
		strvec_ધકેલો(&diff_args, oid_to_hex(head_oid));
	strvec_ધકેલો(&diff_args, "--");
	જો (info->argc)
		strvec_pushv(&diff_args, info->argv);

	repo_config(the_repository, git_diff_basic_config, ખાલી);
	repo_init_revisions(the_repository, &rev, info->prefix);
	rev.abbrev = 0;
	precompose_argv_prefix(diff_args.nr, diff_args.v, ખાલી);
	setup_revisions_from_strvec(&diff_args, &rev, ખાલી);
	rev.diffopt.output_format = DIFF_FORMAT_NO_OUTPUT | DIFF_FORMAT_CALLBACK;
	rev.diffopt.format_callback = submodule_summary_callback;
	rev.diffopt.format_callback_data = &list;

	જો (!info->cached) {
		જો (diff_cmd == DIFF_INDEX)
			setup_work_વૃક્ષ(the_repository);
		જો (repo_read_index_preload(the_repository, &rev.diffopt.pathspec, 0) < 0) {
			perror("repo_read_index_preload");
			ret = -1;
			જાઓ cleanup;
		}
	} નહીંતર જો (repo_read_index(the_repository) < 0) {
		perror("repo_read_cache");
		ret = -1;
		જાઓ cleanup;
	}

	જો (diff_cmd == DIFF_INDEX)
		run_diff_index(&rev, info->cached ? DIFF_INDEX_CACHED : 0);
	નહીંતર
		run_diff_files(&rev, 0);
	prepare_submodule_summary(info, &list);
cleanup:
	strvec_clear(&diff_args);
	release_revisions(&rev);
	module_cb_list_release(&list);
	પાછા ret;
}

સ્થિર પૂર્ણાંક module_summary(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
			  સંરચના repository *repo UNUSED)
{
	સંરચના summary_cb info = SUMMARY_CB_INIT;
	પૂર્ણાંક cached = 0;
	પૂર્ણાંક for_status = 0;
	પૂર્ણાંક files = 0;
	પૂર્ણાંક summary_limit = -1;
	ગણના diff_cmd diff_cmd = DIFF_INDEX;
	સંરચના object_id head_oid;
	પૂર્ણાંક ret;
	સંરચના option module_summary_options[] = {
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
	અચલ અક્ષર *અચલ git_submodule_helper_usage[] = {
		N_("git submodule summary [<options>] [<commit>] [--] [<path>]"),
		ખાલી
	};

	argc = parse_options(argc, argv, prefix, module_summary_options,
			     git_submodule_helper_usage, 0);

	જો (!summary_limit)
		પાછા 0;

	જો (!repo_get_oid(the_repository, argc ? argv[0] : "HEAD", &head_oid)) {
		જો (argc) {
			argv++;
			argc--;
		}
	} નહીંતર જો (!argc || !strcmp(argv[0], "HEAD")) {
		/* before the first commit: compare with an empty tree */
		oidcpy(&head_oid, the_hash_algo->empty_વૃક્ષ);
		જો (argc) {
			argv++;
			argc--;
		}
	} નહીંતર {
		જો (repo_get_oid(the_repository, "HEAD", &head_oid))
			die(_("could not fetch a revision for HEAD"));
	}

	જો (files) {
		જો (cached)
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

	ret = compute_summary_module_list((diff_cmd == DIFF_INDEX) ? &head_oid : ખાલી,
					  &info, diff_cmd);
	પાછા ret;
}

સ્થિર પૂર્ણાંક module_gitdir(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix UNUSED,
			 સંરચના repository *repo)
{
	સંરચના strbuf gitdir = STRBUF_INIT;

	જો (argc != 2)
		usage(_("git submodule--helper gitdir <name>"));

	submodule_name_to_gitdir(&gitdir, repo, argv[1]);

	printf("%s\n", gitdir.buf);

	strbuf_release(&gitdir);
	પાછા 0;
}

સ્થિર પૂર્ણાંક module_migrate(પૂર્ણાંક argc UNUSED, અચલ અક્ષર **argv UNUSED,
			  અચલ અક્ષર *prefix UNUSED, સંરચના repository *repo)
{
	સંરચના strbuf module_dir = STRBUF_INIT;
	DIR *dir;
	સંરચના dirent *de;
	પૂર્ણાંક repo_version = 0;

	repo_git_path_append(repo, &module_dir, "modules/");

	dir = opendir(module_dir.buf);
	જો (!dir)
		die(_("could not open '%s'"), module_dir.buf);

	જ્યારે ((de = readdir(dir))) {
		સંરચના strbuf gitdir_path = STRBUF_INIT;
		અક્ષર *ચાવી;
		અચલ અક્ષર *કિંમત;

		જો (is_dot_or_dotdot(de->d_name))
			ચાલુ;

		strbuf_addf(&gitdir_path, "%s/%s", module_dir.buf, de->d_name);
		જો (!is_git_directory(gitdir_path.buf)) {
			strbuf_release(&gitdir_path);
			ચાલુ;
		}
		strbuf_release(&gitdir_path);

		ચાવી = xstrfmt("submodule.%s.gitdir", de->d_name);
		જો (!repo_config_get_string_tmp(repo, ચાવી, &કિંમત)) {
			/* Already has a gitdir config, nothing to do. */
			મુક્ત(ચાવી);
			ચાલુ;
		}
		મુક્ત(ચાવી);

		create_default_gitdir_config(de->d_name);
	}

	closedir(dir);
	strbuf_release(&module_dir);

	repo_config_get_int(the_repository, "core.repositoryformatversion", &repo_version);
	જો (repo_version == 0 &&
	    repo_config_set_gently(repo, "core.repositoryformatversion", "1"))
		die(_("could not set core.repositoryformatversion to 1.\n"
		      "Please set it for migration to work, for example:\n"
		      "git config core.repositoryformatversion 1"));

	જો (repo_config_set_gently(repo, "extensions.submodulePathConfig", "true"))
		die(_("could not enable submodulePathConfig extension. It is required\n"
		      "for migration to work. Please enable it in the root repo:\n"
		      "git config extensions.submodulePathConfig true"));

	repo->repository_format_submodule_path_cfg = 1;

	પાછા 0;
}

સંરચના sync_cb {
	અચલ અક્ષર *prefix;
	અચલ અક્ષર *super_prefix;
	unsigned પૂર્ણાંક flags;
};
#define SYNC_CB_INIT { 0 }

સ્થિર શૂન્ય sync_submodule(અચલ અક્ષર *path, અચલ અક્ષર *prefix,
			   અચલ અક્ષર *super_prefix, unsigned પૂર્ણાંક flags)
{
	અચલ સંરચના submodule *sub;
	અક્ષર *remote_ચાવી = ખાલી;
	અક્ષર *sub_origin_url, *super_config_url, *displaypath, *default_remote;
	સંરચના strbuf sb = STRBUF_INIT;
	અક્ષર *sub_config_path = ખાલી;
	પૂર્ણાંક code;

	જો (!is_submodule_active(the_repository, path))
		પાછા;

	જો (validate_submodule_path(path) < 0)
		die(ખાલી);

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);

	જો (sub && sub->url) {
		જો (starts_with_dot_dot_slash(sub->url) ||
		    starts_with_dot_slash(sub->url)) {
			અક્ષર *up_path = get_up_path(path);

			sub_origin_url = resolve_relative_url(sub->url, up_path, 1);
			super_config_url = resolve_relative_url(sub->url, ખાલી, 1);
			મુક્ત(up_path);
		} નહીંતર {
			sub_origin_url = xstrdup(sub->url);
			super_config_url = xstrdup(sub->url);
		}
	} નહીંતર {
		sub_origin_url = xstrdup("");
		super_config_url = xstrdup("");
	}

	displaypath = get_submodule_displaypath(path, prefix, super_prefix);

	જો (!(flags & OPT_QUIET))
		printf(_("Synchronizing submodule url for '%s'\n"),
			 displaypath);

	strbuf_reset(&sb);
	strbuf_addf(&sb, "submodule.%s.url", sub->name);
	જો (repo_config_set_gently(the_repository, sb.buf, super_config_url))
		die(_("failed to register url for submodule path '%s'"),
		      displaypath);

	જો (!is_submodule_populated_gently(path, ખાલી))
		જાઓ cleanup;

	strbuf_reset(&sb);
	code = get_default_remote_submodule(path, &default_remote);
	જો (code)
		exit(code);

	remote_ચાવી = xstrfmt("remote.%s.url", default_remote);
	મુક્ત(default_remote);

	submodule_to_gitdir(the_repository, &sb, path);
	strbuf_addstr(&sb, "/config");

	જો (repo_config_set_in_file_gently(the_repository, sb.buf, remote_ચાવી, ખાલી, sub_origin_url))
		die(_("failed to update remote for submodule '%s'"),
		      path);

	જો (flags & OPT_RECURSIVE) {
		સંરચના child_process cpr = CHILD_PROCESS_INIT;

		cpr.git_cmd = 1;
		cpr.dir = path;
		prepare_submodule_repo_env(&cpr.env);

		strvec_pushl(&cpr.args, "submodule--helper", "sync",
			     "--recursive", ખાલી);
		strvec_pushf(&cpr.args, "--super-prefix=%s/", displaypath);

		જો (flags & OPT_QUIET)
			strvec_ધકેલો(&cpr.args, "--quiet");

		જો (run_command(&cpr))
			die(_("failed to recurse into submodule '%s'"),
			      path);
	}

cleanup:
	મુક્ત(super_config_url);
	મુક્ત(sub_origin_url);
	strbuf_release(&sb);
	મુક્ત(remote_ચાવી);
	મુક્ત(displaypath);
	મુક્ત(sub_config_path);
}

સ્થિર શૂન્ય sync_submodule_cb(અચલ સંરચના cache_entry *list_item, શૂન્ય *cb_data)
{
	સંરચના sync_cb *info = cb_data;

	sync_submodule(list_item->name, info->prefix, info->super_prefix,
		       info->flags);
}

સ્થિર પૂર્ણાંક module_sync(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
		       સંરચના repository *repo UNUSED)
{
	સંરચના sync_cb info = SYNC_CB_INIT;
	સંરચના pathspec pathspec = { 0 };
	સંરચના module_list list = MODULE_LIST_INIT;
	પૂર્ણાંક quiet = 0;
	પૂર્ણાંક recursive = 0;
	સંરચના option module_sync_options[] = {
		OPT__SUPER_PREFIX(&info.super_prefix),
		OPT__QUIET(&quiet, N_("suppress output of synchronizing submodule url")),
		OPT_BOOL(0, "recursive", &recursive,
			N_("recurse into nested submodules")),
		OPT_END()
	};
	અચલ અક્ષર *અચલ git_submodule_helper_usage[] = {
		N_("git submodule sync [--quiet] [--recursive] [<path>]"),
		ખાલી
	};
	પૂર્ણાંક ret = 1;

	argc = parse_options(argc, argv, prefix, module_sync_options,
			     git_submodule_helper_usage, 0);

	જો (module_list_compute(argv, prefix, &pathspec, &list) < 0)
		જાઓ cleanup;

	info.prefix = prefix;
	જો (quiet)
		info.flags |= OPT_QUIET;
	જો (recursive)
		info.flags |= OPT_RECURSIVE;

	for_each_listed_submodule(&list, sync_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	પાછા ret;
}

સંરચના deinit_cb {
	અચલ અક્ષર *prefix;
	unsigned પૂર્ણાંક flags;
};
#define DEINIT_CB_INIT { 0 }

સ્થિર શૂન્ય deinit_submodule(અચલ અક્ષર *path, અચલ અક્ષર *prefix,
			     unsigned પૂર્ણાંક flags)
{
	અચલ સંરચના submodule *sub;
	અક્ષર *displaypath = ખાલી;
	સંરચના child_process cp_config = CHILD_PROCESS_INIT;
	સંરચના strbuf sb_config = STRBUF_INIT;
	અક્ષર *sub_git_dir = xstrfmt("%s/.git", path);

	જો (validate_submodule_path(path) < 0)
		die(ખાલી);

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);

	જો (!sub || !sub->name)
		જાઓ cleanup;

	displaypath = get_submodule_displaypath(path, prefix, ખાલી);

	/* remove the submodule work tree (unless the user already did it) */
	જો (is_directory(path)) {
		સંરચના strbuf sb_rm = STRBUF_INIT;
		અચલ અક્ષર *format;

		જો (is_directory(sub_git_dir)) {
			જો (!(flags & OPT_QUIET))
				warning(_("Submodule work tree '%s' contains a .git "
					  "directory. This will be replaced with a "
					  ".git file by using absorbgitdirs."),
					displaypath);

			absorb_git_dir_into_superproject(path, ખાલી);

		}

		જો (!(flags & OPT_FORCE)) {
			સંરચના child_process cp_rm = CHILD_PROCESS_INIT;

			cp_rm.git_cmd = 1;
			strvec_pushl(&cp_rm.args, "rm", "-qn",
				     path, ખાલી);

			જો (run_command(&cp_rm))
				die(_("Submodule work tree '%s' contains local "
				      "modifications; use '-f' to discard them"),
				      displaypath);
		}

		strbuf_addstr(&sb_rm, path);

		જો (!remove_dir_recursively(&sb_rm, 0))
			format = _("Cleared directory '%s'\n");
		નહીંતર
			format = _("Could not remove submodule work tree '%s'\n");

		જો (!(flags & OPT_QUIET))
			printf(format, displaypath);

		submodule_unset_core_worktree(sub);

		strbuf_release(&sb_rm);
	}

	જો (mkdir(path, 0777))
		printf(_("could not create empty submodule directory %s"),
		      displaypath);

	cp_config.git_cmd = 1;
	strvec_pushl(&cp_config.args, "config", "--get-regexp", ખાલી);
	strvec_pushf(&cp_config.args, "submodule.%s\\.", sub->name);

	/* remove the .git/config entries (unless the user already did it) */
	જો (!capture_command(&cp_config, &sb_config, 0) && sb_config.len) {
		અક્ષર *sub_ચાવી = xstrfmt("submodule.%s", sub->name);

		/*
		 * remove the whole section so we have a clean state when
		 * the user later decides to init this submodule again
		 */
		repo_config_rename_section_in_file(the_repository, ખાલી, sub_ચાવી, ખાલી);
		જો (!(flags & OPT_QUIET))
			printf(_("Submodule '%s' (%s) unregistered for path '%s'\n"),
				 sub->name, sub->url, displaypath);
		મુક્ત(sub_ચાવી);
	}

cleanup:
	મુક્ત(displaypath);
	મુક્ત(sub_git_dir);
	strbuf_release(&sb_config);
}

સ્થિર શૂન્ય deinit_submodule_cb(અચલ સંરચના cache_entry *list_item,
				શૂન્ય *cb_data)
{
	સંરચના deinit_cb *info = cb_data;
	deinit_submodule(list_item->name, info->prefix, info->flags);
}

સ્થિર પૂર્ણાંક module_deinit(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
			 સંરચના repository *repo UNUSED)
{
	સંરચના deinit_cb info = DEINIT_CB_INIT;
	સંરચના pathspec pathspec = { 0 };
	સંરચના module_list list = MODULE_LIST_INIT;
	પૂર્ણાંક quiet = 0;
	પૂર્ણાંક force = 0;
	પૂર્ણાંક all = 0;
	સંરચના option module_deinit_options[] = {
		OPT__QUIET(&quiet, N_("suppress submodule status output")),
		OPT__FORCE(&force, N_("remove submodule working trees even if they contain local changes"), 0),
		OPT_BOOL(0, "all", &all, N_("unregister all submodules")),
		OPT_END()
	};
	અચલ અક્ષર *અચલ git_submodule_helper_usage[] = {
		N_("git submodule deinit [--quiet] [-f | --force] [--all | [--] [<path>...]]"),
		ખાલી
	};
	પૂર્ણાંક ret = 1;

	argc = parse_options(argc, argv, prefix, module_deinit_options,
			     git_submodule_helper_usage, 0);

	જો (all && argc) {
		error("pathspec and --all are incompatible");
		usage_with_options(git_submodule_helper_usage,
				   module_deinit_options);
	}

	જો (!argc && !all)
		die(_("Use '--all' if you really want to deinitialize all submodules"));

	જો (module_list_compute(argv, prefix, &pathspec, &list) < 0)
		જાઓ cleanup;

	info.prefix = prefix;
	જો (quiet)
		info.flags |= OPT_QUIET;
	જો (force)
		info.flags |= OPT_FORCE;

	for_each_listed_submodule(&list, deinit_submodule_cb, &info);

	ret = 0;
cleanup:
	module_list_release(&list);
	clear_pathspec(&pathspec);
	પાછા ret;
}

સંરચના module_clone_data {
	અચલ અક્ષર *prefix;
	અચલ અક્ષર *path;
	અચલ અક્ષર *name;
	અચલ અક્ષર *url;
	પૂર્ણાંક depth;
	સંરચના list_objects_filter_options *filter_options;
	ગણના ref_storage_format ref_storage_format;
	unsigned પૂર્ણાંક quiet: 1;
	unsigned પૂર્ણાંક progress: 1;
	unsigned પૂર્ણાંક dissociate: 1;
	unsigned પૂર્ણાંક require_init: 1;
	પૂર્ણાંક single_branch;
};
#define MODULE_CLONE_DATA_INIT { \
	.single_branch = -1, \
	.ref_storage_format = REF_STORAGE_FORMAT_UNKNOWN, \
}

સંરચના submodule_alternate_setup {
	અચલ અક્ષર *submodule_name;
	ગણના SUBMODULE_ALTERNATE_ERROR_MODE {
		SUBMODULE_ALTERNATE_ERROR_DIE,
		SUBMODULE_ALTERNATE_ERROR_INFO,
		SUBMODULE_ALTERNATE_ERROR_IGNORE
	} error_mode;
	સંરચના string_list *reference;
};
#define SUBMODULE_ALTERNATE_SETUP_INIT { \
	.error_mode = SUBMODULE_ALTERNATE_ERROR_IGNORE, \
}

સ્થિર અચલ અક્ષર alternate_error_advice[] = N_(
"An alternate computed from a superproject's alternate is invalid.\n"
"To allow Git to clone without an alternate in such a case, set\n"
"submodule.alternateErrorStrategy to 'info' or, equivalently, clone with\n"
"'--reference-if-able' instead of '--reference'."
);

સ્થિર પૂર્ણાંક add_possible_reference_from_superproject(
		સંરચના odb_files_dir *alt_odb, શૂન્ય *sas_cb)
{
	સંરચના submodule_alternate_setup *sas = sas_cb;
	size_t len;

	/*
	 * If the alternate object store is another repository, try the
	 * standard layout with .git/(modules/<name>)+/objects
	 */
	જો (strip_suffix(alt_odb->abspath, "/objects", &len)) {
		સંરચના repository alternate;
		અક્ષર *sm_alternate;
		સંરચના strbuf sb = STRBUF_INIT;
		સંરચના strbuf err = STRBUF_INIT;
		strbuf_add(&sb, alt_odb->abspath, len);

		જો (repo_init(&alternate, sb.buf, ખાલી) < 0)
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
		જો (sm_alternate) {
			અક્ષર *p = strbuf_detach(&sb, ખાલી);

			string_list_append(sas->reference, p)->util = p;
			મુક્ત(sm_alternate);
		} નહીંતર {
			પસંદગી (sas->error_mode) {
			સ્થિતિ SUBMODULE_ALTERNATE_ERROR_DIE:
				જો (advice_enabled(ADVICE_SUBMODULE_ALTERNATE_ERROR_STRATEGY_DIE))
					advise(_(alternate_error_advice));
				die(_("submodule '%s' cannot add alternate: %s"),
				    sas->submodule_name, err.buf);
			સ્થિતિ SUBMODULE_ALTERNATE_ERROR_INFO:
				fprintf_ln(stderr, _("submodule '%s' cannot add alternate: %s"),
					sas->submodule_name, err.buf);
			સ્થિતિ SUBMODULE_ALTERNATE_ERROR_IGNORE:
				; /* nothing */
			}
		}

		strbuf_release(&err);
		strbuf_release(&sb);
	}

	પાછા 0;
}

સ્થિર શૂન્ય prepare_possible_alternates(અચલ અક્ષર *sm_name,
		સંરચના string_list *reference)
{
	અક્ષર *sm_alternate = ખાલી, *error_strategy = ખાલી;
	સંરચના submodule_alternate_setup sas = SUBMODULE_ALTERNATE_SETUP_INIT;

	repo_config_get_string(the_repository, "submodule.alternateLocation", &sm_alternate);
	જો (!sm_alternate)
		પાછા;

	repo_config_get_string(the_repository, "submodule.alternateErrorStrategy", &error_strategy);

	જો (!error_strategy)
		error_strategy = xstrdup("die");

	sas.submodule_name = sm_name;
	sas.reference = reference;
	જો (!strcmp(error_strategy, "die"))
		sas.error_mode = SUBMODULE_ALTERNATE_ERROR_DIE;
	નહીંતર જો (!strcmp(error_strategy, "info"))
		sas.error_mode = SUBMODULE_ALTERNATE_ERROR_INFO;
	નહીંતર જો (!strcmp(error_strategy, "ignore"))
		sas.error_mode = SUBMODULE_ALTERNATE_ERROR_IGNORE;
	નહીંતર
		die(_("Value '%s' for submodule.alternateErrorStrategy is not recognized"), error_strategy);

	જો (!strcmp(sm_alternate, "superproject"))
		odb_for_each_alternate(the_repository->objects,
				       add_possible_reference_from_superproject, &sas);
	નહીંતર જો (!strcmp(sm_alternate, "no"))
		; /* do nothing */
	નહીંતર
		die(_("Value '%s' for submodule.alternateLocation is not recognized"), sm_alternate);

	મુક્ત(sm_alternate);
	મુક્ત(error_strategy);
}

સ્થિર અક્ષર *clone_submodule_sm_gitdir(અચલ અક્ષર *name)
{
	સંરચના strbuf sb = STRBUF_INIT;
	અક્ષર *sm_gitdir;

	submodule_name_to_gitdir(&sb, the_repository, name);
	sm_gitdir = absolute_pathdup(sb.buf);
	strbuf_release(&sb);

	પાછા sm_gitdir;
}

સ્થિર પૂર્ણાંક dir_contains_only_dotgit(અચલ અક્ષર *path)
{
	DIR *dir = opendir(path);
	સંરચના dirent *e;
	પૂર્ણાંક ret = 1;

	જો (!dir)
		પાછા 0;

	e = readdir_skip_dot_and_dotdot(dir);
	જો (!e)
		ret = 0;
	નહીંતર જો (strcmp(DEFAULT_GIT_DIR_ENVIRONMENT, e->d_name) ||
		 (e = readdir_skip_dot_and_dotdot(dir))) {
		error("unexpected item '%s' in '%s'", e->d_name, path);
		ret = 0;
	}

	closedir(dir);
	પાછા ret;
}

સ્થિર પૂર્ણાંક clone_submodule(અચલ સંરચના module_clone_data *clone_data,
			   સંરચના string_list *reference)
{
	અક્ષર *p;
	અક્ષર *sm_gitdir = clone_submodule_sm_gitdir(clone_data->name);
	અક્ષર *sm_alternate = ખાલી, *error_strategy = ખાલી;
	સંરચના stat st;
	સંરચના child_process cp = CHILD_PROCESS_INIT;
	અચલ અક્ષર *clone_data_path = clone_data->path;
	અક્ષર *to_free = ખાલી;

	જો (validate_submodule_path(clone_data_path) < 0)
		die(ખાલી);

	જો (!is_absolute_path(clone_data->path))
		clone_data_path = to_free = xstrfmt("%s/%s", repo_get_work_વૃક્ષ(the_repository),
						    clone_data->path);

	જો (!file_exists(sm_gitdir)) {
		જો (clone_data->require_init && !stat(clone_data_path, &st) &&
		    !is_empty_dir(clone_data_path))
			die(_("directory not empty: '%s'"), clone_data_path);

		જો (safe_create_leading_directories_const(the_repository, sm_gitdir) < 0)
			die(_("could not create directory '%s'"), sm_gitdir);

		prepare_possible_alternates(clone_data->name, reference);

		strvec_ધકેલો(&cp.args, "clone");
		strvec_ધકેલો(&cp.args, "--no-checkout");
		જો (clone_data->quiet)
			strvec_ધકેલો(&cp.args, "--quiet");
		જો (clone_data->progress)
			strvec_ધકેલો(&cp.args, "--progress");
		જો (clone_data->depth > 0)
			strvec_pushf(&cp.args, "--depth=%d", clone_data->depth);
		જો (reference->nr) {
			સંરચના string_list_item *item;

			for_each_string_list_item(item, reference)
				strvec_pushl(&cp.args, "--reference",
					     item->string, ખાલી);
		}
		જો (clone_data->ref_storage_format != REF_STORAGE_FORMAT_UNKNOWN)
			strvec_pushf(&cp.args, "--ref-storage-format=%s",
				     ref_storage_format_to_name(clone_data->ref_storage_format));
		જો (clone_data->dissociate)
			strvec_ધકેલો(&cp.args, "--dissociate");
		જો (sm_gitdir && *sm_gitdir)
			strvec_pushl(&cp.args, "--separate-git-dir", sm_gitdir, ખાલી);
		જો (clone_data->filter_options && clone_data->filter_options->choice)
			strvec_pushf(&cp.args, "--filter=%s",
				     expand_list_objects_filter_spec(
					     clone_data->filter_options));
		જો (clone_data->single_branch >= 0)
			strvec_ધકેલો(&cp.args, clone_data->single_branch ?
				    "--single-branch" :
				    "--no-single-branch");

		strvec_ધકેલો(&cp.args, "--");
		strvec_ધકેલો(&cp.args, clone_data->url);
		strvec_ધકેલો(&cp.args, clone_data_path);

		cp.git_cmd = 1;
		prepare_submodule_repo_env(&cp.env);
		cp.no_stdin = 1;

		જો(run_command(&cp))
			die(_("clone of '%s' into submodule path '%s' failed"),
			    clone_data->url, clone_data_path);

		જો (clone_data->require_init && !stat(clone_data_path, &st) &&
		    !dir_contains_only_dotgit(clone_data_path)) {
			અક્ષર *dot_git = xstrfmt("%s/.git", clone_data_path);
			unlink(dot_git);
			મુક્ત(dot_git);
			die(_("directory not empty: '%s'"), clone_data_path);
		}
	} નહીંતર {
		અક્ષર *path;

		જો (clone_data->require_init && !stat(clone_data_path, &st) &&
		    !is_empty_dir(clone_data_path))
			die(_("directory not empty: '%s'"), clone_data_path);
		જો (safe_create_leading_directories_const(the_repository, clone_data_path) < 0)
			die(_("could not create directory '%s'"), clone_data_path);
		path = xstrfmt("%s/index", sm_gitdir);
		unlink_or_warn(path);
		મુક્ત(path);
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
	જો (validate_submodule_git_dir(sm_gitdir, clone_data->name) < 0) {
		અક્ષર *head = xstrfmt("%s/HEAD", sm_gitdir);
		unlink(head);
		મુક્ત(head);
		die(_("refusing to create/use '%s' in another submodule's git dir. "
		      "Enabling extensions.submodulePathConfig should fix this."),
		    sm_gitdir);
	}

	connect_work_વૃક્ષ_and_git_dir(clone_data_path, sm_gitdir, 0);

	p = repo_submodule_path(the_repository, clone_data_path, "config");
	જો (!p)
		die(_("could not get submodule directory for '%s'"), clone_data_path);

	/* setup alternateLocation and alternateErrorStrategy in the cloned submodule if needed */
	repo_config_get_string(the_repository, "submodule.alternateLocation", &sm_alternate);
	જો (sm_alternate)
		repo_config_set_in_file(the_repository, p, "submodule.alternateLocation",
					sm_alternate);
	repo_config_get_string(the_repository, "submodule.alternateErrorStrategy", &error_strategy);
	જો (error_strategy)
		repo_config_set_in_file(the_repository, p, "submodule.alternateErrorStrategy",
					error_strategy);

	મુક્ત(sm_alternate);
	મુક્ત(error_strategy);

	મુક્ત(sm_gitdir);
	મુક્ત(p);
	મુક્ત(to_free);
	પાછા 0;
}

સ્થિર પૂર્ણાંક module_clone(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
			સંરચના repository *repo UNUSED)
{
	પૂર્ણાંક dissociate = 0, quiet = 0, progress = 0, require_init = 0;
	સંરચના module_clone_data clone_data = MODULE_CLONE_DATA_INIT;
	સંરચના string_list reference = STRING_LIST_INIT_NODUP;
	સંરચના list_objects_filter_options filter_options =
		LIST_OBJECTS_FILTER_INIT;
	અચલ અક્ષર *ref_storage_format = ખાલી;

	સંરચના option module_clone_options[] = {
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
	અચલ અક્ષર *અચલ git_submodule_helper_usage[] = {
		N_("git submodule--helper clone [--prefix=<path>] [--quiet] "
		   "[--reference <repository>] [--name <name>] [--depth <depth>] "
		   "[--single-branch] [--filter <filter-spec>] "
		   "--url <url> --path <path>"),
		ખાલી
	};

	argc = parse_options(argc, argv, prefix, module_clone_options,
			     git_submodule_helper_usage, 0);

	જો (ref_storage_format) {
		clone_data.ref_storage_format = ref_storage_format_by_name(ref_storage_format);
		જો (clone_data.ref_storage_format == REF_STORAGE_FORMAT_UNKNOWN)
			die(_("unknown ref storage format '%s'"), ref_storage_format);
	}
	clone_data.dissociate = !!dissociate;
	clone_data.quiet = !!quiet;
	clone_data.progress = !!progress;
	clone_data.require_init = !!require_init;
	clone_data.filter_options = &filter_options;

	જો (argc || !clone_data.url || !clone_data.path || !*(clone_data.path))
		usage_with_options(git_submodule_helper_usage,
				   module_clone_options);

	clone_submodule(&clone_data, &reference);
	list_objects_filter_release(&filter_options);
	string_list_clear(&reference, 1);
	પાછા 0;
}

સ્થિર પૂર્ણાંક determine_submodule_update_strategy(સંરચના repository *r,
					       પૂર્ણાંક just_cloned,
					       અચલ અક્ષર *path,
					       ગણના submodule_update_type update,
					       સંરચના submodule_update_strategy *out)
{
	અચલ સંરચના submodule *sub = submodule_from_path(r, null_oid(the_hash_algo), path);
	અક્ષર *ચાવી;
	અચલ અક્ષર *val;
	પૂર્ણાંક ret;

	/*
	 * NEEDSWORK: audit and ensure that update_submodule() has right
	 * to assume that submodule_from_path() above will always succeed.
	 */
	જો (!sub)
		BUG("update_submodule assumes a submodule exists at path (%s)",
		    path);
	ચાવી = xstrfmt("submodule.%s.update", sub->name);

	જો (update) {
		out->type = update;
	} નહીંતર જો (!repo_config_get_string_tmp(r, ચાવી, &val)) {
		જો (parse_submodule_update_strategy(val, out) < 0) {
			ret = die_message(_("Invalid update mode '%s' configured for submodule path '%s'"),
					  val, path);
			જાઓ cleanup;
		}
	} નહીંતર જો (sub->update_strategy.type != SM_UPDATE_UNSPECIFIED) {
		જો (sub->update_strategy.type == SM_UPDATE_COMMAND)
			BUG("how did we read update = !command from .gitmodules?");
		out->type = sub->update_strategy.type;
		out->command = sub->update_strategy.command;
	} નહીંતર
		out->type = SM_UPDATE_CHECKOUT;

	જો (just_cloned &&
	    (out->type == SM_UPDATE_MERGE ||
	     out->type == SM_UPDATE_REBASE ||
	     out->type == SM_UPDATE_NONE))
		out->type = SM_UPDATE_CHECKOUT;

	ret = 0;
cleanup:
	મુક્ત(ચાવી);
	પાછા ret;
}

સંરચના update_clone_data {
	અચલ સંરચના submodule *sub;
	સંરચના object_id oid;
	unsigned just_cloned;
};

સંરચના submodule_update_clone {
	/* index into 'update_data.list', the list of submodules to look into for cloning */
	પૂર્ણાંક current;

	/* configuration parameters which are passed on to the children */
	અચલ સંરચના update_data *update_data;

	/* to be consumed by update_submodule() */
	સંરચના update_clone_data *update_clone;
	પૂર્ણાંક update_clone_nr; પૂર્ણાંક update_clone_alloc;

	/* If we want to stop as fast as possible and return an error */
	unsigned quickstop : 1;

	/* failed clones to be retried again */
	અચલ સંરચના cache_entry **failed_clones;
	પૂર્ણાંક failed_clones_nr, failed_clones_alloc;
};
#define SUBMODULE_UPDATE_CLONE_INIT { 0 }

સ્થિર શૂન્ય submodule_update_clone_release(સંરચના submodule_update_clone *suc)
{
	મુક્ત(suc->update_clone);
	મુક્ત(suc->failed_clones);
}

સંરચના update_data {
	અચલ અક્ષર *prefix;
	અચલ અક્ષર *super_prefix;
	અક્ષર *displaypath;
	ગણના submodule_update_type update_default;
	સંરચના object_id suboid;
	સંરચના string_list references;
	સંરચના submodule_update_strategy update_strategy;
	સંરચના list_objects_filter_options *filter_options;
	સંરચના module_list list;
	ગણના ref_storage_format ref_storage_format;
	પૂર્ણાંક depth;
	પૂર્ણાંક max_jobs;
	પૂર્ણાંક single_branch;
	પૂર્ણાંક recommend_shallow;
	unsigned પૂર્ણાંક require_init;
	unsigned પૂર્ણાંક force;
	unsigned પૂર્ણાંક quiet;
	unsigned પૂર્ણાંક nofetch;
	unsigned પૂર્ણાંક remote;
	unsigned પૂર્ણાંક progress;
	unsigned પૂર્ણાંક dissociate;
	unsigned પૂર્ણાંક init;
	unsigned પૂર્ણાંક warn_if_uninitialized;
	unsigned પૂર્ણાંક recursive;

	/* copied over from update_clone_data */
	સંરચના object_id oid;
	unsigned પૂર્ણાંક just_cloned;
	અચલ અક્ષર *sm_path;
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

સ્થિર શૂન્ય update_data_release(સંરચના update_data *ud)
{
	મુક્ત(ud->displaypath);
	submodule_update_strategy_release(&ud->update_strategy);
	module_list_release(&ud->list);
}

સ્થિર શૂન્ય next_submodule_warn_missing(સંરચના submodule_update_clone *suc,
		સંરચના strbuf *out, અચલ અક્ષર *displaypath)
{
	/*
	 * Only mention uninitialized submodules when their
	 * paths have been specified.
	 */
	જો (suc->update_data->warn_if_uninitialized) {
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
સ્થિર પૂર્ણાંક prepare_to_clone_next_submodule(અચલ સંરચના cache_entry *ce,
					   સંરચના child_process *child,
					   સંરચના submodule_update_clone *suc,
					   સંરચના strbuf *out)
{
	અચલ સંરચના submodule *sub = ખાલી;
	અચલ અક્ષર *url = ખાલી;
	અચલ અક્ષર *update_string;
	ગણના submodule_update_type update_type;
	અક્ષર *ચાવી;
	અચલ સંરચના update_data *ud = suc->update_data;
	અક્ષર *displaypath = get_submodule_displaypath(ce->name, ud->prefix,
						      ud->super_prefix);
	સંરચના strbuf sb = STRBUF_INIT;
	પૂર્ણાંક needs_cloning = 0;
	પૂર્ણાંક need_free_url = 0;

	જો (ce_stage(ce)) {
		strbuf_addf(out, _("Skipping unmerged submodule %s"), displaypath);
		strbuf_addch(out, '\n');
		જાઓ cleanup;
	}

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), ce->name);

	જો (!sub) {
		next_submodule_warn_missing(suc, out, displaypath);
		જાઓ cleanup;
	}

	ચાવી = xstrfmt("submodule.%s.update", sub->name);
	જો (!repo_config_get_string_tmp(the_repository, ચાવી, &update_string)) {
		update_type = parse_submodule_update_type(update_string);
	} નહીંતર {
		update_type = sub->update_strategy.type;
	}
	મુક્ત(ચાવી);

	જો (suc->update_data->update_strategy.type == SM_UPDATE_NONE
	    || (suc->update_data->update_strategy.type == SM_UPDATE_UNSPECIFIED
		&& update_type == SM_UPDATE_NONE)) {
		strbuf_addf(out, _("Skipping submodule '%s'"), displaypath);
		strbuf_addch(out, '\n');
		જાઓ cleanup;
	}

	/* Check if the submodule has been initialized. */
	જો (!is_submodule_active(the_repository, ce->name)) {
		next_submodule_warn_missing(suc, out, displaypath);
		જાઓ cleanup;
	}

	strbuf_reset(&sb);
	strbuf_addf(&sb, "submodule.%s.url", sub->name);
	જો (repo_config_get_string_tmp(the_repository, sb.buf, &url)) {
		જો (sub->url && (starts_with_dot_slash(sub->url) ||
				 starts_with_dot_dot_slash(sub->url))) {
			url = resolve_relative_url(sub->url, ખાલી, 0);
			need_free_url = 1;
		} નહીંતર
			url = sub->url;
	}

	જો (!url)
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

	જો (!needs_cloning)
		જાઓ cleanup;

	child->git_cmd = 1;
	child->no_stdin = 1;
	child->stdout_to_stderr = 1;
	child->err = -1;
	strvec_ધકેલો(&child->args, "submodule--helper");
	strvec_ધકેલો(&child->args, "clone");
	જો (suc->update_data->progress)
		strvec_ધકેલો(&child->args, "--progress");
	જો (suc->update_data->quiet)
		strvec_ધકેલો(&child->args, "--quiet");
	જો (suc->update_data->prefix)
		strvec_pushl(&child->args, "--prefix", suc->update_data->prefix, ખાલી);
	જો (suc->update_data->recommend_shallow && sub->recommend_shallow == 1)
		strvec_ધકેલો(&child->args, "--depth=1");
	નહીંતર જો (suc->update_data->depth)
		strvec_pushf(&child->args, "--depth=%d", suc->update_data->depth);
	જો (suc->update_data->filter_options && suc->update_data->filter_options->choice)
		strvec_pushf(&child->args, "--filter=%s",
			     expand_list_objects_filter_spec(suc->update_data->filter_options));
	જો (suc->update_data->require_init)
		strvec_ધકેલો(&child->args, "--require-init");
	જો (suc->update_data->ref_storage_format != REF_STORAGE_FORMAT_UNKNOWN)
		strvec_pushf(&child->args, "--ref-storage-format=%s",
			     ref_storage_format_to_name(suc->update_data->ref_storage_format));
	strvec_pushl(&child->args, "--path", sub->path, ખાલી);
	strvec_pushl(&child->args, "--name", sub->name, ખાલી);
	strvec_pushl(&child->args, "--url", url, ખાલી);
	જો (suc->update_data->references.nr) {
		સંરચના string_list_item *item;

		for_each_string_list_item(item, &suc->update_data->references)
			strvec_pushl(&child->args, "--reference", item->string, ખાલી);
	}
	જો (suc->update_data->dissociate)
		strvec_ધકેલો(&child->args, "--dissociate");
	જો (suc->update_data->single_branch >= 0)
		strvec_ધકેલો(&child->args, suc->update_data->single_branch ?
					      "--single-branch" :
					      "--no-single-branch");

cleanup:
	મુક્ત(displaypath);
	strbuf_release(&sb);
	જો (need_free_url)
		મુક્ત((શૂન્ય*)url);

	પાછા needs_cloning;
}

સ્થિર પૂર્ણાંક update_clone_get_next_task(સંરચના child_process *child,
				      સંરચના strbuf *err,
				      શૂન્ય *suc_cb,
				      શૂન્ય **idx_task_cb)
{
	સંરચના submodule_update_clone *suc = suc_cb;
	અચલ સંરચના cache_entry *ce;
	પૂર્ણાંક index;

	માટે (; suc->current < suc->update_data->list.nr; suc->current++) {
		ce = suc->update_data->list.entries[suc->current];
		જો (prepare_to_clone_next_submodule(ce, child, suc, err)) {
			પૂર્ણાંક *p = xmalloc(કદ(*p));

			*p = suc->current;
			*idx_task_cb = p;
			suc->current++;
			પાછા 1;
		}
	}

	/*
	 * The loop above tried cloning each submodule once, now try the
	 * stragglers again, which we can imagine as an extension of the
	 * entry list.
	 */
	index = suc->current - suc->update_data->list.nr;
	જો (index < suc->failed_clones_nr) {
		પૂર્ણાંક *p;

		ce = suc->failed_clones[index];
		જો (!prepare_to_clone_next_submodule(ce, child, suc, err)) {
			suc->current ++;
			strbuf_addstr(err, "BUG: submodule considered for "
					   "cloning, doesn't need cloning "
					   "any more?\n");
			પાછા 0;
		}
		p = xmalloc(કદ(*p));
		*p = suc->current;
		*idx_task_cb = p;
		suc->current ++;
		પાછા 1;
	}

	પાછા 0;
}

સ્થિર પૂર્ણાંક update_clone_start_failure(સંરચના strbuf *err UNUSED,
				      શૂન્ય *suc_cb,
				      શૂન્ય *idx_task_cb UNUSED)
{
	સંરચના submodule_update_clone *suc = suc_cb;

	suc->quickstop = 1;
	પાછા 1;
}

સ્થિર પૂર્ણાંક update_clone_task_finished(પૂર્ણાંક result,
				      સંરચના strbuf *err,
				      શૂન્ય *suc_cb,
				      શૂન્ય *idx_task_cb)
{
	અચલ સંરચના cache_entry *ce;
	સંરચના submodule_update_clone *suc = suc_cb;
	પૂર્ણાંક *idxP = idx_task_cb;
	પૂર્ણાંક idx = *idxP;

	મુક્ત(idxP);

	જો (!result)
		પાછા 0;

	જો (idx < suc->update_data->list.nr) {
		ce  = suc->update_data->list.entries[idx];
		strbuf_addf(err, _("Failed to clone '%s'. Retry scheduled"),
			    ce->name);
		strbuf_addch(err, '\n');
		ALLOC_GROW(suc->failed_clones,
			   suc->failed_clones_nr + 1,
			   suc->failed_clones_alloc);
		suc->failed_clones[suc->failed_clones_nr++] = ce;
		પાછા 0;
	} નહીંતર {
		idx -= suc->update_data->list.nr;
		ce  = suc->failed_clones[idx];
		strbuf_addf(err, _("Failed to clone '%s' a second time, aborting"),
			    ce->name);
		strbuf_addch(err, '\n');
		suc->quickstop = 1;
		પાછા 1;
	}

	પાછા 0;
}

સ્થિર પૂર્ણાંક git_update_clone_config(અચલ અક્ષર *var, અચલ અક્ષર *કિંમત,
				   અચલ સંરચના config_context *ctx,
				   શૂન્ય *cb)
{
	પૂર્ણાંક *max_jobs = cb;

	જો (!strcmp(var, "submodule.fetchjobs"))
		*max_jobs = parse_submodule_fetchjobs(var, કિંમત, ctx->kvi);
	પાછા 0;
}

સ્થિર પૂર્ણાંક is_tip_reachable(અચલ અક્ષર *path, અચલ સંરચના object_id *oid)
{
	સંરચના child_process cp = CHILD_PROCESS_INIT;
	સંરચના strbuf rev = STRBUF_INIT;
	અક્ષર *hex = oid_to_hex(oid);
	પૂર્ણાંક reachable;

	cp.git_cmd = 1;
	cp.dir = path;
	cp.no_stderr = 1;
	strvec_pushl(&cp.args, "rev-list", "-n", "1", hex, "--not", "--all", ખાલી);

	prepare_submodule_repo_env(&cp.env);

	જો (capture_command(&cp, &rev, GIT_MAX_HEXSZ + 1) || rev.len)
		reachable = 0;
	નહીંતર
		reachable = 1;

	strbuf_release(&rev);
	પાછા reachable;
}

સ્થિર પૂર્ણાંક fetch_in_submodule(અચલ અક્ષર *module_path, પૂર્ણાંક depth, પૂર્ણાંક quiet,
			      અચલ સંરચના object_id *oid)
{
	સંરચના child_process cp = CHILD_PROCESS_INIT;

	prepare_submodule_repo_env(&cp.env);
	cp.git_cmd = 1;
	cp.dir = module_path;

	strvec_ધકેલો(&cp.args, "fetch");
	જો (quiet)
		strvec_ધકેલો(&cp.args, "--quiet");
	જો (depth)
		strvec_pushf(&cp.args, "--depth=%d", depth);
	જો (oid) {
		અક્ષર *hex = oid_to_hex(oid);
		અક્ષર *remote;
		પૂર્ણાંક code;

		code = get_default_remote_submodule(module_path, &remote);
		જો (code) {
			child_process_clear(&cp);
			પાછા code;
		}

		strvec_pushl(&cp.args, remote, hex, ખાલી);
		મુક્ત(remote);
	}

	પાછા run_command(&cp);
}

સ્થિર પૂર્ણાંક run_update_command(અચલ સંરચના update_data *ud, પૂર્ણાંક subforce)
{
	સંરચના child_process cp = CHILD_PROCESS_INIT;
	અક્ષર *oid = oid_to_hex(&ud->oid);
	પૂર્ણાંક ret;

	પસંદગી (ud->update_strategy.type) {
	સ્થિતિ SM_UPDATE_CHECKOUT:
		cp.git_cmd = 1;
		strvec_pushl(&cp.args, "checkout", "-q", ખાલી);
		જો (subforce)
			strvec_ધકેલો(&cp.args, "-f");
		તોડો;
	સ્થિતિ SM_UPDATE_REBASE:
		cp.git_cmd = 1;
		strvec_ધકેલો(&cp.args, "rebase");
		જો (ud->quiet)
			strvec_ધકેલો(&cp.args, "--quiet");
		તોડો;
	સ્થિતિ SM_UPDATE_MERGE:
		cp.git_cmd = 1;
		strvec_ધકેલો(&cp.args, "merge");
		જો (ud->quiet)
			strvec_ધકેલો(&cp.args, "--quiet");
		તોડો;
	સ્થિતિ SM_UPDATE_COMMAND:
		cp.use_shell = 1;
		strvec_ધકેલો(&cp.args, ud->update_strategy.command);
		તોડો;
	મૂળભૂત:
		BUG("unexpected update strategy type: %d",
		    ud->update_strategy.type);
	}
	strvec_ધકેલો(&cp.args, oid);

	cp.dir = ud->sm_path;
	prepare_submodule_repo_env(&cp.env);
	જો ((ret = run_command(&cp))) {
		પસંદગી (ud->update_strategy.type) {
		સ્થિતિ SM_UPDATE_CHECKOUT:
			die_message(_("Unable to checkout '%s' in submodule path '%s'"),
				    oid, ud->displaypath);
			/* No "ret" assignment, use "git checkout"'s */
			તોડો;
		સ્થિતિ SM_UPDATE_REBASE:
			ret = die_message(_("Unable to rebase '%s' in submodule path '%s'"),
					  oid, ud->displaypath);
			તોડો;
		સ્થિતિ SM_UPDATE_MERGE:
			ret = die_message(_("Unable to merge '%s' in submodule path '%s'"),
					  oid, ud->displaypath);
			તોડો;
		સ્થિતિ SM_UPDATE_COMMAND:
			ret = die_message(_("Execution of '%s %s' failed in submodule path '%s'"),
					  ud->update_strategy.command, oid, ud->displaypath);
			તોડો;
		મૂળભૂત:
			BUG("unexpected update strategy type: %d",
			    ud->update_strategy.type);
		}

		પાછા ret;
	}

	જો (ud->quiet)
		પાછા 0;

	પસંદગી (ud->update_strategy.type) {
	સ્થિતિ SM_UPDATE_CHECKOUT:
		printf(_("Submodule path '%s': checked out '%s'\n"),
		       ud->displaypath, oid);
		તોડો;
	સ્થિતિ SM_UPDATE_REBASE:
		printf(_("Submodule path '%s': rebased into '%s'\n"),
		       ud->displaypath, oid);
		તોડો;
	સ્થિતિ SM_UPDATE_MERGE:
		printf(_("Submodule path '%s': merged in '%s'\n"),
		       ud->displaypath, oid);
		તોડો;
	સ્થિતિ SM_UPDATE_COMMAND:
		printf(_("Submodule path '%s': '%s %s'\n"),
		       ud->displaypath, ud->update_strategy.command, oid);
		તોડો;
	મૂળભૂત:
		BUG("unexpected update strategy type: %d",
		    ud->update_strategy.type);
	}

	પાછા 0;
}

સ્થિર પૂર્ણાંક run_update_procedure(અચલ સંરચના update_data *ud)
{
	પૂર્ણાંક subforce = is_null_oid(&ud->suboid) || ud->force;

	જો (!ud->nofetch) {
		/*
		 * Run fetch only if `oid` isn't present or it
		 * is not reachable from a ref.
		 */
		જો (!is_tip_reachable(ud->sm_path, &ud->oid) &&
		    fetch_in_submodule(ud->sm_path, ud->depth, ud->quiet, ખાલી) &&
		    !ud->quiet)
			fprintf_ln(stderr,
				   _("Unable to fetch in submodule path '%s'; "
				     "trying to directly fetch %s:"),
				   ud->displaypath, oid_to_hex(&ud->oid));
		/*
		 * Now we tried the usual fetch, but `oid` may
		 * not be reachable from any of the refs.
		 */
		જો (!is_tip_reachable(ud->sm_path, &ud->oid) &&
		    fetch_in_submodule(ud->sm_path, ud->depth, ud->quiet, &ud->oid))
			પાછા die_message(_("Fetched in submodule path '%s', but it did not "
					     "contain %s. Direct fetching of that commit failed."),
					   ud->displaypath, oid_to_hex(&ud->oid));
	}

	પાછા run_update_command(ud, subforce);
}

સ્થિર પૂર્ણાંક remote_submodule_branch(અચલ અક્ષર *path, અચલ અક્ષર **branch)
{
	અચલ સંરચના submodule *sub;
	અક્ષર *ચાવી;
	*branch = ખાલી;

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);
	જો (!sub)
		પાછા die_message(_("could not initialize submodule at path '%s'"),
				   path);

	ચાવી = xstrfmt("submodule.%s.branch", sub->name);
	જો (repo_config_get_string_tmp(the_repository, ચાવી, branch))
		*branch = sub->branch;
	મુક્ત(ચાવી);

	જો (!*branch) {
		*branch = "HEAD";
		પાછા 0;
	}

	જો (!strcmp(*branch, ".")) {
		અચલ અક્ષર *refname = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
							      "HEAD", 0, ખાલી,
							      ખાલી);

		જો (!refname)
			પાછા die_message(_("No such ref: %s"), "HEAD");

		/* detached HEAD */
		જો (!strcmp(refname, "HEAD"))
			પાછા die_message(_("Submodule (%s) branch configured to inherit "
					     "branch from superproject, but the superproject "
					     "is not on any branch"), sub->name);

		જો (!skip_prefix(refname, "refs/heads/", &refname))
			પાછા die_message(_("Expecting a full ref name, got %s"),
					   refname);

		*branch = refname;
		પાછા 0;
	}

	/* Our "branch" is coming from repo_config_get_string_tmp() */
	પાછા 0;
}

સ્થિર પૂર્ણાંક ensure_core_worktree(અચલ અક્ષર *path)
{
	અચલ અક્ષર *cw;
	સંરચના repository subrepo;

	જો (repo_submodule_init(&subrepo, the_repository, path, null_oid(the_hash_algo)))
		પાછા die_message(_("could not get a repository handle for submodule '%s'"),
				   path);

	જો (!repo_config_get_string_tmp(&subrepo, "core.worktree", &cw)) {
		અક્ષર *cfg_file, *abs_path;
		અચલ અક્ષર *rel_path;
		સંરચના strbuf sb = STRBUF_INIT;

		cfg_file = repo_git_path(&subrepo, "config");

		abs_path = absolute_pathdup(path);
		rel_path = relative_path(abs_path, subrepo.gitdir, &sb);

		repo_config_set_in_file(the_repository, cfg_file, "core.worktree", rel_path);

		મુક્ત(cfg_file);
		મુક્ત(abs_path);
		strbuf_release(&sb);
	}

	repo_clear(&subrepo);
	પાછા 0;
}

સ્થિર અચલ અક્ષર *submodule_update_type_to_label(ગણના submodule_update_type type)
{
	પસંદગી (type) {
	સ્થિતિ SM_UPDATE_CHECKOUT:
		પાછા "checkout";
	સ્થિતિ SM_UPDATE_MERGE:
		પાછા "merge";
	સ્થિતિ SM_UPDATE_REBASE:
		પાછા "rebase";
	સ્થિતિ SM_UPDATE_UNSPECIFIED:
	સ્થિતિ SM_UPDATE_NONE:
	સ્થિતિ SM_UPDATE_COMMAND:
		તોડો;
	}
	BUG("unreachable with type %d", type);
}

સ્થિર શૂન્ય update_data_to_args(અચલ સંરચના update_data *update_data,
				સંરચના strvec *args)
{
	ગણના submodule_update_type update_type = update_data->update_default;

	strvec_pushl(args, "submodule--helper", "update", "--recursive", ખાલી);
	જો (update_data->displaypath)
		strvec_pushf(args, "--super-prefix=%s/",
			     update_data->displaypath);
	strvec_pushf(args, "--jobs=%d", update_data->max_jobs);
	જો (update_data->quiet)
		strvec_ધકેલો(args, "--quiet");
	જો (update_data->force)
		strvec_ધકેલો(args, "--force");
	જો (update_data->init)
		strvec_ધકેલો(args, "--init");
	જો (update_data->remote)
		strvec_ધકેલો(args, "--remote");
	જો (update_data->nofetch)
		strvec_ધકેલો(args, "--no-fetch");
	જો (update_data->dissociate)
		strvec_ધકેલો(args, "--dissociate");
	જો (update_data->progress)
		strvec_ધકેલો(args, "--progress");
	જો (update_data->require_init)
		strvec_ધકેલો(args, "--require-init");
	જો (update_data->depth)
		strvec_pushf(args, "--depth=%d", update_data->depth);
	જો (update_type != SM_UPDATE_UNSPECIFIED)
		strvec_pushf(args, "--%s",
			     submodule_update_type_to_label(update_type));

	જો (update_data->references.nr) {
		સંરચના string_list_item *item;

		for_each_string_list_item(item, &update_data->references)
			strvec_pushl(args, "--reference", item->string, ખાલી);
	}
	જો (update_data->ref_storage_format != REF_STORAGE_FORMAT_UNKNOWN)
		strvec_pushf(args, "--ref-storage-format=%s",
			     ref_storage_format_to_name(update_data->ref_storage_format));
	જો (update_data->filter_options && update_data->filter_options->choice)
		strvec_pushf(args, "--filter=%s",
				expand_list_objects_filter_spec(
					update_data->filter_options));
	જો (update_data->recommend_shallow == 0)
		strvec_ધકેલો(args, "--no-recommend-shallow");
	નહીંતર જો (update_data->recommend_shallow == 1)
		strvec_ધકેલો(args, "--recommend-shallow");
	જો (update_data->single_branch >= 0)
		strvec_ધકેલો(args, update_data->single_branch ?
				    "--single-branch" :
				    "--no-single-branch");
}

સ્થિર પૂર્ણાંક update_submodule(સંરચના update_data *update_data)
{
	પૂર્ણાંક ret;

	જો (validate_submodule_path(update_data->sm_path) < 0)
		પાછા -1;

	ret = determine_submodule_update_strategy(the_repository,
						  update_data->just_cloned,
						  update_data->sm_path,
						  update_data->update_default,
						  &update_data->update_strategy);
	જો (ret)
		પાછા ret;

	જો (update_data->just_cloned)
		oidcpy(&update_data->suboid, null_oid(the_hash_algo));
	નહીંતર જો (repo_resolve_gitlink_ref(the_repository, update_data->sm_path,
					  "HEAD", &update_data->suboid))
		પાછા die_message(_("Unable to find current revision in submodule path '%s'"),
				   update_data->displaypath);

	જો (update_data->remote) {
		અક્ષર *remote_name;
		અચલ અક્ષર *branch;
		અક્ષર *remote_ref;
		પૂર્ણાંક code;

		code = get_default_remote_submodule(update_data->sm_path, &remote_name);
		જો (code)
			પાછા code;
		code = remote_submodule_branch(update_data->sm_path, &branch);
		જો (code) {
			મુક્ત(remote_name);
			પાછા code;
		}
		remote_ref = xstrfmt("refs/remotes/%s/%s", remote_name, branch);

		મુક્ત(remote_name);

		જો (!update_data->nofetch) {
			જો (fetch_in_submodule(update_data->sm_path, update_data->depth,
					      0, ખાલી)) {
				મુક્ત(remote_ref);
				પાછા die_message(_("Unable to fetch in submodule path '%s'"),
						   update_data->sm_path);
			}
		}

		જો (repo_resolve_gitlink_ref(the_repository, update_data->sm_path,
					     remote_ref, &update_data->oid)) {
			ret = die_message(_("Unable to find %s revision in submodule path '%s'"),
					  remote_ref, update_data->sm_path);
			મુક્ત(remote_ref);
			પાછા ret;
		}

		મુક્ત(remote_ref);
	}

	જો (!oideq(&update_data->oid, &update_data->suboid) || update_data->force) {
		ret = run_update_procedure(update_data);
		જો (ret)
			પાછા ret;
	}

	જો (update_data->recursive) {
		સંરચના child_process cp = CHILD_PROCESS_INIT;
		સંરચના update_data next = *update_data;

		next.prefix = ખાલી;
		oidcpy(&next.oid, null_oid(the_hash_algo));
		oidcpy(&next.suboid, null_oid(the_hash_algo));

		cp.dir = update_data->sm_path;
		cp.git_cmd = 1;
		prepare_submodule_repo_env(&cp.env);
		update_data_to_args(&next, &cp.args);

		ret = run_command(&cp);
		જો (ret)
			die_message(_("Failed to recurse into submodule path '%s'"),
				    update_data->displaypath);
		પાછા ret;
	}

	પાછા 0;
}

સ્થિર પૂર્ણાંક update_submodules(સંરચના update_data *update_data)
{
	પૂર્ણાંક i, ret = 0;
	સંરચના submodule_update_clone suc = SUBMODULE_UPDATE_CLONE_INIT;
	અચલ સંરચના run_process_parallel_opts opts = {
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
	જો (suc.quickstop) {
		ret = 1;
		જાઓ cleanup;
	}

	માટે (i = 0; i < suc.update_clone_nr; i++) {
		સંરચના update_clone_data ucd = suc.update_clone[i];
		પૂર્ણાંક code = 128;

		oidcpy(&update_data->oid, &ucd.oid);
		update_data->just_cloned = ucd.just_cloned;
		update_data->sm_path = ucd.sub->path;

		/*
		 * Verify that the submodule path does not contain any
		 * symlinks; if it does, it might have been tampered with.
		 * TODO: allow exempting it via
		 * `safe.submodule.path` or something
		 */
		જો (validate_submodule_path(update_data->sm_path) < 0)
			જાઓ fail;

		code = ensure_core_worktree(update_data->sm_path);
		જો (code)
			જાઓ fail;

		update_data->displaypath = get_submodule_displaypath(
			update_data->sm_path, update_data->prefix,
			update_data->super_prefix);
		code = update_submodule(update_data);
		FREE_AND_NULL(update_data->displaypath);
fail:
		જો (!code)
			ચાલુ;
		ret = code;
		જો (ret == 128)
			જાઓ cleanup;
	}

cleanup:
	submodule_update_clone_release(&suc);
	string_list_clear(&update_data->references, 0);
	પાછા ret;
}

સ્થિર પૂર્ણાંક module_update(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
			 સંરચના repository *repo UNUSED)
{
	સંરચના pathspec pathspec = { 0 };
	સંરચના pathspec pathspec2 = { 0 };
	સંરચના update_data opt = UPDATE_DATA_INIT;
	સંરચના list_objects_filter_options filter_options =
		LIST_OBJECTS_FILTER_INIT;
	અચલ અક્ષર *ref_storage_format = ખાલી;
	પૂર્ણાંક ret;
	સંરચના option module_update_options[] = {
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
	અચલ અક્ષર *અચલ git_submodule_helper_usage[] = {
		N_("git submodule [--quiet] update"
		" [--init [--filter=<filter-spec>]] [--remote]"
		" [-N|--no-fetch] [-f|--force]"
		" [--checkout|--merge|--rebase]"
		" [--[no-]recommend-shallow] [--reference <repository>]"
		" [--recursive] [--[no-]single-branch] [--] [<path>...]"),
		ખાલી
	};

	update_clone_config_from_gitmodules(the_repository, &opt.max_jobs);
	repo_config(the_repository, git_update_clone_config, &opt.max_jobs);

	argc = parse_options(argc, argv, prefix, module_update_options,
			     git_submodule_helper_usage, 0);

	જો (opt.require_init)
		opt.init = 1;

	જો (filter_options.choice && !opt.init) {
		usage_with_options(git_submodule_helper_usage,
				   module_update_options);
	}

	જો (ref_storage_format) {
		opt.ref_storage_format = ref_storage_format_by_name(ref_storage_format);
		જો (opt.ref_storage_format == REF_STORAGE_FORMAT_UNKNOWN)
			die(_("unknown ref storage format '%s'"), ref_storage_format);
	}

	opt.filter_options = &filter_options;
	opt.prefix = prefix;

	જો (opt.update_default)
		opt.update_strategy.type = opt.update_default;

	જો (module_list_compute(argv, prefix, &pathspec, &opt.list) < 0) {
		ret = 1;
		જાઓ cleanup;
	}

	જો (pathspec.nr)
		opt.warn_if_uninitialized = 1;

	જો (opt.init) {
		સંરચના module_list list = MODULE_LIST_INIT;
		સંરચના init_cb info = INIT_CB_INIT;

		જો (module_list_compute(argv, opt.prefix,
					&pathspec2, &list) < 0) {
			module_list_release(&list);
			ret = 1;
			જાઓ cleanup;
		}

		/*
		 * If there are no path args and submodule.active is set then,
		 * by default, only initialize 'active' modules.
		 */
		જો (!argc && !repo_config_get(the_repository, "submodule.active"))
			module_list_active(&list);

		info.prefix = opt.prefix;
		info.super_prefix = opt.super_prefix;
		જો (opt.quiet)
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
	પાછા ret;
}

સ્થિર પૂર્ણાંક ધકેલો_check(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix UNUSED,
		      સંરચના repository *repo UNUSED)
{
	સંરચના remote *remote;
	અચલ અક્ષર *superproject_head;
	અક્ષર *head;
	પૂર્ણાંક detached_head = 0;
	સંરચના object_id head_oid;

	જો (argc < 3)
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
				   0, &head_oid, ખાલી);
	જો (!head)
		die(_("Failed to resolve HEAD as a valid ref."));
	જો (!strcmp(head, "HEAD"))
		detached_head = 1;

	/*
	 * The remote must be configured.
	 * This is to avoid pushing to the exact same URL as the parent.
	 */
	remote = pushremote_get(argv[1]);
	જો (!remote || remote->origin == REMOTE_UNCONFIGURED)
		die("remote '%s' not configured", argv[1]);

	/* Check the refspec */
	જો (argc > 2) {
		પૂર્ણાંક i;
		સંરચના ref *local_refs = get_local_heads();
		સંરચના refspec refspec = REFSPEC_INIT_ધકેલો(the_hash_algo);

		refspec_appendn(&refspec, argv + 2, argc - 2);

		માટે (i = 0; i < refspec.nr; i++) {
			અચલ સંરચના refspec_item *rs = &refspec.વસ્તુઓ[i];

			જો (rs->pattern || rs->matching)
				ચાલુ;

			/* LHS must match a single ref */
			પસંદગી (ગણતરી_refspec_match(rs->src, local_refs, ખાલી)) {
			સ્થિતિ 1:
				તોડો;
			સ્થિતિ 0:
				/*
				 * If LHS matches 'HEAD' then we need to ensure
				 * that it matches the same named branch
				 * checked out in the superproject.
				 */
				જો (!strcmp(rs->src, "HEAD")) {
					જો (!detached_head &&
					    !strcmp(head, superproject_head))
						તોડો;
					die("HEAD does not match the named branch in the superproject");
				}
				/* fallthrough */
			મૂળભૂત:
				die("src refspec '%s' must name a ref",
				    rs->src);
			}
		}

		refspec_clear(&refspec);
		free_refs(local_refs);
	}
	મુક્ત(head);

	પાછા 0;
}

સ્થિર પૂર્ણાંક absorb_git_dirs(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
			   સંરચના repository *repo UNUSED)
{
	પૂર્ણાંક i;
	સંરચના pathspec pathspec = { 0 };
	સંરચના module_list list = MODULE_LIST_INIT;
	અચલ અક્ષર *super_prefix = ખાલી;
	સંરચના option embed_gitdir_options[] = {
		OPT__SUPER_PREFIX(&super_prefix),
		OPT_END()
	};
	અચલ અક્ષર *અચલ git_submodule_helper_usage[] = {
		N_("git submodule absorbgitdirs [<options>] [<path>...]"),
		ખાલી
	};
	પૂર્ણાંક ret = 1;

	argc = parse_options(argc, argv, prefix, embed_gitdir_options,
			     git_submodule_helper_usage, 0);

	જો (module_list_compute(argv, prefix, &pathspec, &list) < 0)
		જાઓ cleanup;

	માટે (i = 0; i < list.nr; i++)
		absorb_git_dir_into_superproject(list.entries[i]->name,
						 super_prefix);

	ret = 0;
cleanup:
	clear_pathspec(&pathspec);
	module_list_release(&list);
	પાછા ret;
}

સ્થિર પૂર્ણાંક module_set_url(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
			  સંરચના repository *repo UNUSED)
{
	પૂર્ણાંક quiet = 0, ret;
	અચલ અક્ષર *newurl;
	અચલ અક્ષર *path;
	અક્ષર *config_name;
	સંરચના option options[] = {
		OPT__QUIET(&quiet, N_("suppress output for setting url of a submodule")),
		OPT_END()
	};
	અચલ અક્ષર *અચલ usage[] = {
		N_("git submodule set-url [--quiet] <path> <newurl>"),
		ખાલી
	};
	અચલ સંરચના submodule *sub;

	argc = parse_options(argc, argv, prefix, options, usage, 0);

	જો (argc != 2 || !(path = argv[0]) || !(newurl = argv[1]))
		usage_with_options(usage, options);

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);

	જો (!sub)
		die(_("no submodule mapping found in .gitmodules for path '%s'"),
		    path);

	config_name = xstrfmt("submodule.%s.url", sub->name);
	ret = config_set_in_gitmodules_file_gently(the_repository, config_name, newurl);

	જો (!ret) {
		repo_read_gitmodules(the_repository, 0);
		sync_submodule(sub->path, prefix, ખાલી, quiet ? OPT_QUIET : 0);
	}

	મુક્ત(config_name);
	પાછા !!ret;
}

સ્થિર પૂર્ણાંક module_set_branch(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
			     સંરચના repository *repo UNUSED)
{
	પૂર્ણાંક opt_default = 0, ret;
	અચલ અક્ષર *opt_branch = ખાલી;
	અચલ અક્ષર *path;
	અક્ષર *config_name;
	સંરચના option options[] = {
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
	અચલ અક્ષર *અચલ usage[] = {
		N_("git submodule set-branch [-q|--quiet] (-d|--default) <path>"),
		N_("git submodule set-branch [-q|--quiet] (-b|--branch) <branch> <path>"),
		ખાલી
	};
	અચલ સંરચના submodule *sub;

	argc = parse_options(argc, argv, prefix, options, usage, 0);

	જો (!opt_branch && !opt_default)
		die(_("--branch or --default required"));

	જો (opt_branch && opt_default)
		die(_("options '%s' and '%s' cannot be used together"), "--branch", "--default");

	જો (argc != 1 || !(path = argv[0]))
		usage_with_options(usage, options);

	sub = submodule_from_path(the_repository, null_oid(the_hash_algo), path);

	જો (!sub)
		die(_("no submodule mapping found in .gitmodules for path '%s'"),
		    path);

	config_name = xstrfmt("submodule.%s.branch", sub->name);
	ret = config_set_in_gitmodules_file_gently(the_repository, config_name, opt_branch);

	મુક્ત(config_name);
	પાછા !!ret;
}

સ્થિર પૂર્ણાંક module_create_branch(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
				સંરચના repository *repo UNUSED)
{
	ગણના branch_track track;
	પૂર્ણાંક quiet = 0, force = 0, reflog = 0, dry_run = 0;
	સંરચના option options[] = {
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
	અચલ અક્ષર *અચલ usage[] = {
		N_("git submodule--helper create-branch [-f|--force] [--create-reflog] [-q|--quiet] [-t|--track] [-n|--dry-run] <name> <start-oid> <start-name>"),
		ખાલી
	};
	સંરચના repo_config_કિંમતો *cfg = repo_config_કિંમતો(the_repository);

	repo_config(the_repository, git_default_config, ખાલી);
	track = cfg->branch_track;
	argc = parse_options(argc, argv, prefix, options, usage, 0);

	જો (argc != 3)
		usage_with_options(usage, options);

	જો (!quiet && !dry_run)
		printf_ln(_("creating branch '%s'"), argv[0]);

	create_branches_recursively(the_repository, argv[0], argv[1], argv[2],
				    force, reflog, quiet, track, dry_run);
	પાછા 0;
}

સંરચના add_data {
	અચલ અક્ષર *prefix;
	અચલ અક્ષર *branch;
	અચલ અક્ષર *reference_path;
	અક્ષર *sm_path;
	અચલ અક્ષર *sm_name;
	અચલ અક્ષર *repo;
	અચલ અક્ષર *realrepo;
	ગણના ref_storage_format ref_storage_format;
	પૂર્ણાંક depth;
	unsigned પૂર્ણાંક force: 1;
	unsigned પૂર્ણાંક quiet: 1;
	unsigned પૂર્ણાંક progress: 1;
	unsigned પૂર્ણાંક dissociate: 1;
};
#define ADD_DATA_INIT { \
	.depth = -1, \
	.ref_storage_format = REF_STORAGE_FORMAT_UNKNOWN, \
}

સ્થિર શૂન્ય append_fetch_remotes(સંરચના strbuf *સંદેશ, અચલ અક્ષર *git_dir_path)
{
	સંરચના child_process cp_remote = CHILD_PROCESS_INIT;
	સંરચના strbuf sb_remote_out = STRBUF_INIT;

	cp_remote.git_cmd = 1;
	strvec_pushf(&cp_remote.env,
		     "GIT_DIR=%s", git_dir_path);
	strvec_ધકેલો(&cp_remote.env, "GIT_WORK_TREE=.");
	strvec_pushl(&cp_remote.args, "remote", "-v", ખાલી);
	જો (!capture_command(&cp_remote, &sb_remote_out, 0)) {
		અક્ષર *next_લીટી;
		અક્ષર *લીટી = sb_remote_out.buf;

		જ્યારે ((next_લીટી = strchr(લીટી, '\n')) != ખાલી) {
			size_t len = next_લીટી - લીટી;

			જો (strip_suffix_mem(લીટી, &len, " (fetch)"))
				strbuf_addf(સંદેશ, "  %.*s\n", (પૂર્ણાંક)len, લીટી);
			લીટી = next_લીટી + 1;
		}
	}

	strbuf_release(&sb_remote_out);
}

સ્થિર પૂર્ણાંક add_submodule(અચલ સંરચના add_data *add_data)
{
	સંરચના module_clone_data clone_data = MODULE_CLONE_DATA_INIT;
	સંરચના string_list reference = STRING_LIST_INIT_NODUP;
	પૂર્ણાંક ret = -1;

	/* perhaps the path already exists and is already a git repo, else clone it */
	જો (is_directory(add_data->sm_path)) {
		અક્ષર *submod_gitdir_path;
		સંરચના strbuf sm_path = STRBUF_INIT;
		strbuf_addstr(&sm_path, add_data->sm_path);
		submod_gitdir_path = xstrfmt("%s/.git", add_data->sm_path);
		જો (is_nonbare_repository_dir(&sm_path))
			printf(_("Adding existing repo at '%s' to the index\n"),
			       add_data->sm_path);
		નહીંતર
			die(_("'%s' already exists and is not a valid git repo"),
			    add_data->sm_path);
		strbuf_release(&sm_path);
		મુક્ત(submod_gitdir_path);
	} નહીંતર {
		સંરચના child_process cp = CHILD_PROCESS_INIT;
		સંરચના strbuf submod_gitdir = STRBUF_INIT;

		submodule_name_to_gitdir(&submod_gitdir, the_repository, add_data->sm_name);

		જો (is_directory(submod_gitdir.buf)) {
			જો (!add_data->force) {
				સંરચના strbuf સંદેશ = STRBUF_INIT;
				અક્ષર *die_સંદેશ;

				strbuf_addf(&સંદેશ, _("A git directory for '%s' is found "
						    "locally with remote(s):\n"),
					    add_data->sm_name);

				append_fetch_remotes(&સંદેશ, submod_gitdir.buf);
				strbuf_release(&submod_gitdir);

				strbuf_addf(&સંદેશ, _("If you want to reuse this local git "
						    "directory instead of cloning again from\n"
						    "  %s\n"
						    "use the '--force' option. If the local git "
						    "directory is not the correct repo\n"
						    "or you are unsure what this means choose "
						    "another name with the '--name' option."),
					    add_data->realrepo);

				die_સંદેશ = strbuf_detach(&સંદેશ, ખાલી);
				die("%s", die_સંદેશ);
			} નહીંતર {
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
		જો (add_data->reference_path) {
			અક્ષર *p = xstrdup(add_data->reference_path);

			string_list_append(&reference, p)->util = p;
		}
		clone_data.ref_storage_format = add_data->ref_storage_format;
		clone_data.dissociate = add_data->dissociate;
		જો (add_data->depth >= 0)
			clone_data.depth = add_data->depth;

		જો (clone_submodule(&clone_data, &reference))
			જાઓ cleanup;

		prepare_submodule_repo_env(&cp.env);
		cp.git_cmd = 1;
		cp.dir = add_data->sm_path;
		/*
		 * NOTE: we only get here if add_data->force is true, so
		 * passing --force to checkout is reasonable.
		 */
		strvec_pushl(&cp.args, "checkout", "-f", "-q", ખાલી);

		જો (add_data->branch) {
			strvec_pushl(&cp.args, "-B", add_data->branch, ખાલી);
			strvec_pushf(&cp.args, "origin/%s", add_data->branch);
		}

		જો (run_command(&cp))
			die(_("unable to checkout submodule '%s'"), add_data->sm_path);
	}
	ret = 0;

cleanup:
	string_list_clear(&reference, 1);
	પાછા ret;
}

સ્થિર પૂર્ણાંક config_submodule_in_gitmodules(અચલ અક્ષર *name, અચલ અક્ષર *var, અચલ અક્ષર *કિંમત)
{
	અક્ષર *ચાવી;
	પૂર્ણાંક ret;

	જો (!is_writing_gitmodules_ok())
		die(_("please make sure that the .gitmodules file is in the working tree"));

	ચાવી = xstrfmt("submodule.%s.%s", name, var);
	ret = config_set_in_gitmodules_file_gently(the_repository, ચાવી, કિંમત);
	મુક્ત(ચાવી);

	પાછા ret;
}

સ્થિર શૂન્ય configure_added_submodule(સંરચના add_data *add_data)
{
	અક્ષર *ચાવી;
	સંરચના child_process add_submod = CHILD_PROCESS_INIT;
	સંરચના child_process add_gitmodules = CHILD_PROCESS_INIT;
	અચલ સંરચના string_list *કિંમતો;
	પૂર્ણાંક matched = 0;

	ચાવી = xstrfmt("submodule.%s.url", add_data->sm_name);
	repo_config_set_gently(the_repository, ચાવી, add_data->realrepo);
	મુક્ત(ચાવી);

	add_submod.git_cmd = 1;
	strvec_pushl(&add_submod.args, "add",
		     "--no-warn-embedded-repo", ખાલી);
	જો (add_data->force)
		strvec_ધકેલો(&add_submod.args, "--force");
	strvec_pushl(&add_submod.args, "--", add_data->sm_path, ખાલી);

	જો (run_command(&add_submod))
		die(_("Failed to add submodule '%s'"), add_data->sm_path);

	જો (config_submodule_in_gitmodules(add_data->sm_name, "path", add_data->sm_path) ||
	    config_submodule_in_gitmodules(add_data->sm_name, "url", add_data->repo))
		die(_("Failed to register submodule '%s'"), add_data->sm_path);

	જો (add_data->branch) {
		જો (config_submodule_in_gitmodules(add_data->sm_name,
						   "branch", add_data->branch))
			die(_("Failed to register submodule '%s'"), add_data->sm_path);
	}

	add_gitmodules.git_cmd = 1;
	strvec_pushl(&add_gitmodules.args,
		     "add", "--force", "--", ".gitmodules", ખાલી);

	જો (run_command(&add_gitmodules))
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
	જો (repo_config_get(the_repository, "submodule.active") || /* key absent */
	    repo_config_get_string_multi(the_repository, "submodule.active", &કિંમતો)) {
		/*
		 * If the submodule being added isn't already covered by the
		 * current configured pathspec, set the submodule's active flag
		 */
		ચાવી = xstrfmt("submodule.%s.active", add_data->sm_name);
		repo_config_set_gently(the_repository, ચાવી, "true");
		મુક્ત(ચાવી);
	} નહીંતર {
		માટે (size_t i = 0; i < કિંમતો->nr; i++) {
			અચલ અક્ષર *pat = કિંમતો->વસ્તુઓ[i].string;
			જો (!wildmatch(pat, add_data->sm_path, 0)) { /* match found */
				matched = 1;
				તોડો;
			}
		}
		જો (!matched) { /* no pattern matched -> force-enable */
			ચાવી = xstrfmt("submodule.%s.active", add_data->sm_name);
			repo_config_set_gently(the_repository, ચાવી, "true");
			મુક્ત(ચાવી);
		}
	}
}

સ્થિર શૂન્ય die_on_index_match(અચલ અક્ષર *path, પૂર્ણાંક force)
{
	સંરચના pathspec ps;
	અચલ અક્ષર *args[] = { path, ખાલી };
	parse_pathspec(&ps, 0, PATHSPEC_PREFER_CWD, ખાલી, args);

	જો (repo_read_index_preload(the_repository, ખાલી, 0) < 0)
		die(_("index file corrupt"));

	જો (ps.nr) {
		અક્ષર *ps_matched = xcalloc(ps.nr, 1);

		/* TODO: audit for interaction with sparse-index. */
		ensure_full_index(the_repository->index);

		/*
		 * Since there is only one pathspec, we just need to
		 * check ps_matched[0] to know if a cache entry matched.
		 */
		માટે (size_t i = 0; i < the_repository->index->cache_nr; i++) {
			ce_path_match(the_repository->index, the_repository->index->cache[i], &ps,
				      ps_matched);

			જો (ps_matched[0]) {
				જો (!force)
					die(_("'%s' already exists in the index"),
					    path);
				જો (!S_ISGITLINK(the_repository->index->cache[i]->ce_mode))
					die(_("'%s' already exists in the index "
					      "and is not a submodule"), path);
				તોડો;
			}
		}
		મુક્ત(ps_matched);
	}
	clear_pathspec(&ps);
}

સ્થિર શૂન્ય die_on_repo_without_commits(અચલ અક્ષર *path)
{
	સંરચના strbuf sb = STRBUF_INIT;
	strbuf_addstr(&sb, path);
	જો (is_nonbare_repository_dir(&sb)) {
		સંરચના object_id oid;
		જો (repo_resolve_gitlink_ref(the_repository, path, "HEAD", &oid) < 0)
			die(_("'%s' does not have a commit checked out"), path);
	}
	strbuf_release(&sb);
}

સ્થિર પૂર્ણાંક module_add(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
		      સંરચના repository *repo UNUSED)
{
	પૂર્ણાંક force = 0, quiet = 0, progress = 0, dissociate = 0;
	સંરચના add_data add_data = ADD_DATA_INIT;
	અચલ અક્ષર *ref_storage_format = ખાલી;
	અક્ષર *to_free = ખાલી;
	અચલ સંરચના submodule *existing;
	સંરચના strbuf buf = STRBUF_INIT;
	અક્ષર *sm_name_to_free = ખાલી;
	સંરચના option options[] = {
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
	અચલ અક્ષર *અચલ usage[] = {
		N_("git submodule add [<options>] [--] <repository> [<path>]"),
		ખાલી
	};
	સંરચના strbuf sb = STRBUF_INIT;
	પૂર્ણાંક ret = 1;

	argc = parse_options(argc, argv, prefix, options, usage, 0);

	જો (!is_writing_gitmodules_ok())
		die(_("please make sure that the .gitmodules file is in the working tree"));

	જો (prefix && *prefix &&
	    add_data.reference_path && !is_absolute_path(add_data.reference_path))
		add_data.reference_path = xstrfmt("%s%s", prefix, add_data.reference_path);

	જો (argc == 0 || argc > 2)
		usage_with_options(usage, options);

	જો (ref_storage_format) {
		add_data.ref_storage_format = ref_storage_format_by_name(ref_storage_format);
		જો (add_data.ref_storage_format == REF_STORAGE_FORMAT_UNKNOWN)
			die(_("unknown ref storage format '%s'"), ref_storage_format);
	}

	add_data.repo = argv[0];
	જો (argc == 1)
		add_data.sm_path = git_url_basename(add_data.repo, 0, 0);
	નહીંતર
		add_data.sm_path = xstrdup(argv[1]);

	જો (prefix && *prefix && !is_absolute_path(add_data.sm_path)) {
		અક્ષર *sm_path = add_data.sm_path;

		add_data.sm_path = xstrfmt("%s%s", prefix, sm_path);
		મુક્ત(sm_path);
	}

	જો (starts_with_dot_dot_slash(add_data.repo) ||
	    starts_with_dot_slash(add_data.repo)) {
		જો (prefix)
			die(_("Relative path can only be used from the toplevel "
			      "of the working tree"));

		/* dereference source url relative to parent's url */
		to_free = resolve_relative_url(add_data.repo, ખાલી, 1);
		add_data.realrepo = to_free;
	} નહીંતર જો (is_dir_sep(add_data.repo[0]) || strchr(add_data.repo, ':')) {
		add_data.realrepo = add_data.repo;
	} નહીંતર {
		die(_("repo URL: '%s' must be absolute or begin with ./|../"),
		    add_data.repo);
	}

	/*
	 * normalize path:
	 * multiple //; leading ./; /./; /../;
	 */
	normalize_path_copy(add_data.sm_path, add_data.sm_path);
	strip_dir_trailing_slashes(add_data.sm_path);

	જો (validate_submodule_path(add_data.sm_path) < 0)
		die(ખાલી);

	die_on_index_match(add_data.sm_path, force);
	die_on_repo_without_commits(add_data.sm_path);

	જો (!force) {
		સંરચના child_process cp = CHILD_PROCESS_INIT;

		cp.git_cmd = 1;
		cp.no_stdout = 1;
		strvec_pushl(&cp.args, "add", "--dry-run", "--ignore-missing",
			     "--no-warn-embedded-repo", add_data.sm_path, ખાલી);
		જો ((ret = pipe_command(&cp, ખાલી, 0, ખાલી, 0, &sb, 0))) {
			strbuf_complete_લીટી(&sb);
			fputs(sb.buf, stderr);
			જાઓ cleanup;
		}
	}

	જો (!add_data.sm_name)
		add_data.sm_name = add_data.sm_path;

	existing = submodule_from_name(the_repository,
					null_oid(the_hash_algo),
					add_data.sm_name);

	જો (existing && existing->path &&
	    strcmp(existing->path, add_data.sm_path)) {
		જો (!force) {
			die(_("submodule name '%s' already used for path '%s'"),
			    add_data.sm_name, existing->path);
		}
		/* --force: build <name><n> until unique */
		માટે (પૂર્ણાંક i = 1; ; i++) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "%s%d", add_data.sm_name, i);
			જો (!submodule_from_name(the_repository,
						null_oid(the_hash_algo),
						buf.buf)) {
				તોડો;
			}
		}
		add_data.sm_name = sm_name_to_free = strbuf_detach(&buf, ખાલી);
	}

	જો (check_submodule_name(add_data.sm_name))
		die(_("'%s' is not a valid submodule name"), add_data.sm_name);

	add_data.prefix = prefix;
	add_data.force = !!force;
	add_data.quiet = !!quiet;
	add_data.progress = !!progress;
	add_data.dissociate = !!dissociate;

	જો (the_repository->repository_format_submodule_path_cfg)
		create_default_gitdir_config(add_data.sm_name);

	જો (add_submodule(&add_data))
		જાઓ cleanup;
	configure_added_submodule(&add_data);

	ret = 0;
cleanup:
	મુક્ત(sm_name_to_free);
	મુક્ત(add_data.sm_path);
	મુક્ત(to_free);
	strbuf_release(&sb);

	પાછા ret;
}

પૂર્ણાંક cmd_submodule__helper(પૂર્ણાંક argc,
			  અચલ અક્ષર **argv,
			  અચલ અક્ષર *prefix,
			  સંરચના repository *repo)
{
	parse_opt_subcommand_fn *fn = ખાલી;
	અચલ અક્ષર *અચલ usage[] = {
		N_("git submodule--helper <command>"),
		ખાલી
	};
	સંરચના option options[] = {
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
		OPT_SUBCOMMAND("push-check", &fn, ધકેલો_check),
		OPT_SUBCOMMAND("absorbgitdirs", &fn, absorb_git_dirs),
		OPT_SUBCOMMAND("set-url", &fn, module_set_url),
		OPT_SUBCOMMAND("set-branch", &fn, module_set_branch),
		OPT_SUBCOMMAND("create-branch", &fn, module_create_branch),
		OPT_SUBCOMMAND("get-default-remote", &fn, module_get_default_remote),
		OPT_END()
	};
	argc = parse_options(argc, argv, prefix, options, usage, 0);

	પાછા fn(argc, argv, prefix, repo);
}
