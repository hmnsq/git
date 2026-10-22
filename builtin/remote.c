#define USE_THE_REPOSITORY_VARIABLE

#अन्तर्भाव "builtin.h"
#अन्तर्भाव "advice.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "date.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "ident.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "path.h"
#अन्तर्भाव "transport.h"
#अन्तर्भाव "remote.h"
#अन्तर्भाव "string-list.h"
#अन्तर्भाव "strbuf.h"
#अन्तर्भाव "run-command.h"
#अन्तर्भाव "rebase.h"
#अन्तर्भाव "refs.h"
#अन्तर्भाव "refspec.h"
#अन्तर्भाव "odb.h"
#अन्तर्भाव "strvec.h"
#अन्तर्भाव "commit-reach.h"
#अन्तर्भाव "progress.h"

स्थिर ध्रुव वर्ण * ध्रुव builtin_remote_usage[] = {
	"git remote [-v | --verbose]",
	N_("git remote add [-t <branch>] [-m <master>] [-f] [--tags | --no-tags] [--mirror=<fetch|push>] <name> <url>"),
	N_("git remote rename [--[no-]progress] <old> <new>"),
	N_("git remote remove <name>"),
	N_("git remote set-head <name> (-a | --auto | -d | --delete | <branch>)"),
	N_("git remote [-v | --verbose] show [-n] <name>"),
	N_("git remote prune [-n | --dry-run] <name>"),
	N_("git remote [-v | --verbose] update [-p | --prune] [(<group> | <remote>)...]"),
	N_("git remote set-branches [--add] <name> <branch>..."),
	N_("git remote get-url [--push] [--all] <name>"),
	N_("git remote set-url [--push] <name> <newurl> [<oldurl>]"),
	N_("git remote set-url --add <name> <newurl>"),
	N_("git remote set-url --delete <name> <url>"),
	रिक्त
};

स्थिर ध्रुव वर्ण * ध्रुव builtin_remote_add_usage[] = {
	N_("git remote add [<options>] <name> <url>"),
	रिक्त
};

स्थिर ध्रुव वर्ण * ध्रुव builtin_remote_rename_usage[] = {
	N_("git remote rename [--[no-]progress] <old> <new>"),
	रिक्त
};

स्थिर ध्रुव वर्ण * ध्रुव builtin_remote_rm_usage[] = {
	N_("git remote remove <name>"),
	रिक्त
};

स्थिर ध्रुव वर्ण * ध्रुव builtin_remote_sethead_usage[] = {
	N_("git remote set-head <name> (-a | --auto | -d | --delete | <branch>)"),
	रिक्त
};

स्थिर ध्रुव वर्ण * ध्रुव builtin_remote_setbranches_usage[] = {
	N_("git remote set-branches <name> <branch>..."),
	N_("git remote set-branches --add <name> <branch>..."),
	रिक्त
};

स्थिर ध्रुव वर्ण * ध्रुव builtin_remote_show_usage[] = {
	N_("git remote show [<options>] <name>"),
	रिक्त
};

स्थिर ध्रुव वर्ण * ध्रुव builtin_remote_prune_usage[] = {
	N_("git remote prune [<options>] <name>"),
	रिक्त
};

स्थिर ध्रुव वर्ण * ध्रुव builtin_remote_update_usage[] = {
	N_("git remote update [<options>] [<group> | <remote>]..."),
	रिक्त
};

स्थिर ध्रुव वर्ण * ध्रुव builtin_remote_geturl_usage[] = {
	N_("git remote get-url [--push] [--all] <name>"),
	रिक्त
};

स्थिर ध्रुव वर्ण * ध्रुव builtin_remote_seturl_usage[] = {
	N_("git remote set-url [--push] <name> <newurl> [<oldurl>]"),
	N_("git remote set-url --add <name> <newurl>"),
	N_("git remote set-url --delete <name> <url>"),
	रिक्त
};

#define GET_REF_STATES (1<<0)
#define GET_HEAD_NAMES (1<<1)
#define GET_प्रक्षिप_REF_STATES (1<<2)

स्थिर पूर्णाङ्क verbose;

स्थिर पूर्णाङ्क fetch_remote(ध्रुव वर्ण *name)
{
	संरचना child_process cmd = CHILD_PROCESS_INIT;

	strvec_प्रक्षिप(&cmd.args, "fetch");
	जइ (verbose)
		strvec_प्रक्षिप(&cmd.args, "-v");
	strvec_प्रक्षिप(&cmd.args, name);
	cmd.git_cmd = 1;
	printf_ln(_("Updating %s"), name);
	जइ (run_command(&cmd))
		प्रत्यागम error(_("Could not fetch %s"), name);
	प्रत्यागम 0;
}

गणना {
	TAGS_UNSET = 0,
	TAGS_DEFAULT = 1,
	TAGS_SET = 2
};

#define MIRROR_NONE 0
#define MIRROR_FETCH 1
#define MIRROR_प्रक्षिप 2
#define MIRROR_BOTH (MIRROR_FETCH|MIRROR_प्रक्षिप)

स्थिर शून्य add_branch(ध्रुव वर्ण *कुञ्चिका, ध्रुव वर्ण *branchname,
		       ध्रुव वर्ण *remotename, पूर्णाङ्क mirror, संरचना strbuf *tmp)
{
	strbuf_reset(tmp);
	strbuf_addch(tmp, '+');
	जइ (mirror)
		strbuf_addf(tmp, "refs/%s:refs/%s",
				branchname, branchname);
	अण्णहा
		strbuf_addf(tmp, "refs/heads/%s:refs/remotes/%s/%s",
				branchname, remotename, branchname);
	repo_config_set_multivar(the_repository, कुञ्चिका, tmp->buf, "^$", 0);
}

स्थिर ध्रुव वर्ण mirror_advice[] =
N_("--mirror is dangerous and deprecated; please\n"
   "\t use --mirror=fetch or --mirror=push instead");

स्थिर पूर्णाङ्क parse_mirror_opt(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg, पूर्णाङ्क not)
{
	unsigned *mirror = opt->मानम्;
	जइ (not)
		*mirror = MIRROR_NONE;
	अण्णहा जइ (!arg) {
		warning("%s", _(mirror_advice));
		*mirror = MIRROR_BOTH;
	}
	अण्णहा जइ (!strcmp(arg, "fetch"))
		*mirror = MIRROR_FETCH;
	अण्णहा जइ (!strcmp(arg, "push"))
		*mirror = MIRROR_प्रक्षिप;
	अण्णहा
		प्रत्यागम error(_("unknown --mirror argument: %s"), arg);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क check_remote_collision(संरचना remote *remote, शून्य *data)
{
	ध्रुव वर्ण *name = data;
	ध्रुव वर्ण *p;

	जइ (skip_prefix(name, remote->name, &p) && *p == '/')
		die(_("remote name '%s' is a subset of existing remote '%s'"),
		    name, remote->name);
	जइ (skip_prefix(remote->name, name, &p) && *p == '/')
		die(_("remote name '%s' is a superset of existing remote '%s'"),
		    name, remote->name);

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क add(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *prefix,
	       संरचना repository *repo UNUSED)
{
	पूर्णाङ्क fetch = 0, fetch_tags = TAGS_DEFAULT;
	unsigned mirror = MIRROR_NONE;
	संरचना string_list track = STRING_LIST_INIT_NODUP;
	ध्रुव वर्ण *master = रिक्त;
	संरचना remote *remote;
	संरचना strbuf buf = STRBUF_INIT, buf2 = STRBUF_INIT;
	ध्रुव वर्ण *name, *url;
	पूर्णाङ्क result = 0;

	संरचना option options[] = {
		OPT_BOOL('f', "fetch", &fetch, N_("fetch the remote branches")),
		OPT_SET_INT(0, "tags", &fetch_tags,
			    N_("import all tags and associated objects when fetching\n"
			       "or do not fetch any tag at all (--no-tags)"),
			    TAGS_SET),
		OPT_STRING_LIST('t', "track", &track, N_("branch"),
				N_("branch(es) to track")),
		OPT_STRING('m', "master", &master, N_("branch"), N_("master branch")),
		OPT_CALLBACK_F(0, "mirror", &mirror, "(push|fetch)",
			N_("set up remote as a mirror to push to or fetch from"),
			PARSE_OPT_OPTARG | PARSE_OPT_COMP_ARG, parse_mirror_opt),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_add_usage, 0);

	जइ (argc != 2)
		usage_with_options(builtin_remote_add_usage, options);

	जइ (mirror && master)
		die(_("specifying a master branch makes no sense with --mirror"));
	जइ (mirror && !(mirror & MIRROR_FETCH) && track.nr)
		die(_("specifying branches to track makes sense only with fetch mirrors"));

	name = argv[0];
	url = argv[1];

	remote = remote_get(name);
	जइ (remote_is_configured(remote, 1)) {
		error(_("remote %s already exists."), name);
		exit(3);
	}

	जइ (!valid_remote_name(name))
		die(_("'%s' is not a valid remote name"), name);

	for_each_remote(check_remote_collision, (शून्य *)name);

	strbuf_addf(&buf, "remote.%s.url", name);
	repo_config_set(the_repository, buf.buf, url);

	जइ (!mirror || mirror & MIRROR_FETCH) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.fetch", name);
		जइ (track.nr == 0)
			string_list_append(&track, "*");
		प्रति (size_t i = 0; i < track.nr; i++) {
			add_branch(buf.buf, track.वस्तूनि[i].string,
				   name, mirror, &buf2);
		}
	}

	जइ (mirror & MIRROR_प्रक्षिप) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.mirror", name);
		repo_config_set(the_repository, buf.buf, "true");
	}

	जइ (fetch_tags != TAGS_DEFAULT) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.tagOpt", name);
		repo_config_set(the_repository, buf.buf,
				fetch_tags == TAGS_SET ? "--tags" : "--no-tags");
	}

	जइ (fetch && fetch_remote(name)) {
		result = 1;
		गच्छ out;
	}

	जइ (master) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "refs/remotes/%s/HEAD", name);

		strbuf_reset(&buf2);
		strbuf_addf(&buf2, "refs/remotes/%s/%s", name, master);

		जइ (refs_update_symref(get_main_ref_store(the_repository), buf.buf, buf2.buf, "remote add"))
			result = error(_("Could not setup master '%s'"), master);
	}

out:
	strbuf_release(&buf);
	strbuf_release(&buf2);
	string_list_clear(&track, 0);

	प्रत्यागम result;
}

संरचना branch_info {
	वर्ण *remote_name;
	संरचना string_list merge;
	गणना rebase_type rebase;
	वर्ण *प्रक्षिप_remote_name;
};

स्थिर संरचना string_list branch_list = STRING_LIST_INIT_DUP;

स्थिर ध्रुव वर्ण *abbrev_ref(ध्रुव वर्ण *name, ध्रुव वर्ण *prefix)
{
	skip_prefix(name, prefix, &name);
	प्रत्यागम name;
}
#define abbrev_branch(name) abbrev_ref((name), "refs/heads/")

स्थिर पूर्णाङ्क config_read_branches(ध्रुव वर्ण *कुञ्चिका, ध्रुव वर्ण *मानम्,
				ध्रुव संरचना config_context *ctx UNUSED,
				शून्य *data UNUSED)
{
	ध्रुव वर्ण *orig_कुञ्चिका = कुञ्चिका;
	वर्ण *name;
	संरचना string_list_item *item;
	संरचना branch_info *info;
	गणना { REMOTE, MERGE, REBASE, प्रक्षिप_REMOTE } type;
	size_t कुञ्चिका_len;

	जइ (!starts_with(कुञ्चिका, "branch."))
		प्रत्यागम 0;

	कुञ्चिका += strlen("branch.");
	जइ (strip_suffix(कुञ्चिका, ".remote", &कुञ्चिका_len))
		type = REMOTE;
	अण्णहा जइ (strip_suffix(कुञ्चिका, ".merge", &कुञ्चिका_len))
		type = MERGE;
	अण्णहा जइ (strip_suffix(कुञ्चिका, ".rebase", &कुञ्चिका_len))
		type = REBASE;
	अण्णहा जइ (strip_suffix(कुञ्चिका, ".pushremote", &कुञ्चिका_len))
		type = प्रक्षिप_REMOTE;
	अण्णहा
		प्रत्यागम 0;

	name = xmemdupz(कुञ्चिका, कुञ्चिका_len);
	item = string_list_निवेशय(&branch_list, name);

	जइ (!item->util)
		item->util = xcalloc(1, परिमाण(संरचना branch_info));
	info = item->util;
	विकल्प (type) {
	पक्ष REMOTE:
		जइ (info->remote_name)
			warning(_("more than one %s"), orig_कुञ्चिका);
		info->remote_name = xstrdup(मानम्);
		भङ्ग;
	पक्ष MERGE: {
		ध्रुव वर्ण *space = strchr(मानम्, ' ');
		मानम् = abbrev_branch(मानम्);
		जाव (space) {
			वर्ण *merge;
			merge = xstrndup(मानम्, space - मानम्);
			string_list_append(&info->merge, merge);
			मानम् = abbrev_branch(space + 1);
			space = strchr(मानम्, ' ');
		}
		string_list_append(&info->merge, xstrdup(मानम्));
		भङ्ग;
	}
	पक्ष REBASE:
		/*
		 * Consider invalid values as false and check the
		 * truth value with >= REBASE_TRUE.
		 */
		info->rebase = rebase_parse_मानम्(मानम्);
		जइ (info->rebase == REBASE_INVALID)
			warning(_("unhandled branch.%s.rebase=%s; assuming "
				  "'true'"), name, मानम्);
		भङ्ग;
	पक्ष प्रक्षिप_REMOTE:
		जइ (info->प्रक्षिप_remote_name)
			warning(_("more than one %s"), orig_कुञ्चिका);
		info->प्रक्षिप_remote_name = xstrdup(मानम्);
		भङ्ग;
	स्वभाव:
		BUG("unexpected type=%d", type);
	}

	मुक्त(name);
	प्रत्यागम 0;
}

स्थिर शून्य read_branches(शून्य)
{
	जइ (branch_list.nr)
		प्रत्यागम;
	repo_config(the_repository, config_read_branches, रिक्त);
}

संरचना ref_states {
	संरचना remote *remote;
	संरचना string_list new_refs, skipped, stale, tracked, heads, प्रक्षिप;
	पूर्णाङ्क queried;
};

#define REF_STATES_INIT { \
	.new_refs = STRING_LIST_INIT_DUP, \
	.skipped = STRING_LIST_INIT_DUP, \
	.stale = STRING_LIST_INIT_DUP, \
	.tracked = STRING_LIST_INIT_DUP, \
	.heads = STRING_LIST_INIT_DUP, \
	.प्रक्षिप = STRING_LIST_INIT_DUP, \
}

स्थिर पूर्णाङ्क get_ref_states(ध्रुव संरचना ref *remote_refs, संरचना ref_states *states)
{
	संरचना ref *fetch_map = रिक्त, **tail = &fetch_map;
	संरचना ref *ref, *stale_refs;
	पूर्णाङ्क i;

	प्रति (i = 0; i < states->remote->fetch.nr; i++)
		जइ (get_fetch_map(remote_refs, &states->remote->fetch.वस्तूनि[i], &tail, 1))
			die(_("Could not get fetch map for refspec %s"),
				states->remote->fetch.वस्तूनि[i].raw);

	प्रति (ref = fetch_map; ref; ref = ref->next) {
		जइ (refname_matches_negative_refspec_item(ref->name, &states->remote->fetch))
			string_list_append(&states->skipped, abbrev_branch(ref->name));
		अण्णहा जइ (!ref->peer_ref || !refs_ref_exists(get_main_ref_store(the_repository), ref->peer_ref->name))
			string_list_append(&states->new_refs, abbrev_branch(ref->name));
		अण्णहा
			string_list_append(&states->tracked, abbrev_branch(ref->name));
	}
	stale_refs = get_stale_heads(&states->remote->fetch, fetch_map);
	प्रति (ref = stale_refs; ref; ref = ref->next) {
		संरचना string_list_item *item =
			string_list_append(&states->stale, abbrev_branch(ref->name));
		item->util = xstrdup(ref->name);
	}
	free_refs(stale_refs);
	free_refs(fetch_map);

	string_list_sort(&states->new_refs);
	string_list_sort(&states->skipped);
	string_list_sort(&states->tracked);
	string_list_sort(&states->stale);

	प्रत्यागम 0;
}

संरचना प्रक्षिप_info {
	वर्ण *dest;
	पूर्णाङ्क forced;
	गणना {
		प्रक्षिप_STATUS_CREATE = 0,
		प्रक्षिप_STATUS_DELETE,
		प्रक्षिप_STATUS_UPTODATE,
		प्रक्षिप_STATUS_FASTFORWARD,
		प्रक्षिप_STATUS_OUTOFDATE,
		प्रक्षिप_STATUS_NOTQUERIED
	} status;
};

स्थिर पूर्णाङ्क get_प्रक्षिप_ref_states(ध्रुव संरचना ref *remote_refs,
	संरचना ref_states *states)
{
	संरचना remote *remote = states->remote;
	संरचना ref *ref, *local_refs, *प्रक्षिप_map;
	जइ (remote->mirror)
		प्रत्यागम 0;

	local_refs = get_local_heads();
	प्रक्षिप_map = copy_ref_list(remote_refs);

	match_प्रक्षिप_refs(local_refs, &प्रक्षिप_map, &remote->प्रक्षिप, MATCH_REFS_NONE);

	प्रति (ref = प्रक्षिप_map; ref; ref = ref->next) {
		संरचना string_list_item *item;
		संरचना प्रक्षिप_info *info;

		जइ (!ref->peer_ref)
			अनुवर्त;
		oidcpy(&ref->new_oid, &ref->peer_ref->new_oid);

		item = string_list_append(&states->प्रक्षिप,
					  abbrev_branch(ref->peer_ref->name));
		item->util = xcalloc(1, परिमाण(संरचना प्रक्षिप_info));
		info = item->util;
		info->forced = ref->force;
		info->dest = xstrdup(abbrev_branch(ref->name));

		जइ (is_null_oid(&ref->new_oid)) {
			info->status = प्रक्षिप_STATUS_DELETE;
		} अण्णहा जइ (oideq(&ref->old_oid, &ref->new_oid))
			info->status = प्रक्षिप_STATUS_UPTODATE;
		अण्णहा जइ (is_null_oid(&ref->old_oid))
			info->status = प्रक्षिप_STATUS_CREATE;
		अण्णहा जइ (odb_has_object(the_repository->objects, &ref->old_oid,
					ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR) &&
			 ref_newer(&ref->new_oid, &ref->old_oid))
			info->status = प्रक्षिप_STATUS_FASTFORWARD;
		अण्णहा
			info->status = प्रक्षिप_STATUS_OUTOFDATE;
	}
	free_refs(local_refs);
	free_refs(प्रक्षिप_map);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क get_प्रक्षिप_ref_states_noquery(संरचना ref_states *states)
{
	पूर्णाङ्क i;
	संरचना remote *remote = states->remote;
	संरचना string_list_item *item;
	संरचना प्रक्षिप_info *info;

	जइ (remote->mirror)
		प्रत्यागम 0;

	जइ (!remote->प्रक्षिप.nr) {
		item = string_list_append(&states->प्रक्षिप, _("(matching)"));
		info = item->util = xcalloc(1, परिमाण(संरचना प्रक्षिप_info));
		info->status = प्रक्षिप_STATUS_NOTQUERIED;
		info->dest = xstrdup(item->string);
	}
	प्रति (i = 0; i < remote->प्रक्षिप.nr; i++) {
		ध्रुव संरचना refspec_item *spec = &remote->प्रक्षिप.वस्तूनि[i];
		जइ (spec->matching)
			item = string_list_append(&states->प्रक्षिप, _("(matching)"));
		अण्णहा जइ (strlen(spec->src))
			item = string_list_append(&states->प्रक्षिप, spec->src);
		अण्णहा
			item = string_list_append(&states->प्रक्षिप, _("(delete)"));

		info = item->util = xcalloc(1, परिमाण(संरचना प्रक्षिप_info));
		info->forced = spec->force;
		info->status = प्रक्षिप_STATUS_NOTQUERIED;
		info->dest = xstrdup(spec->dst ? spec->dst : item->string);
	}
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क get_head_names(ध्रुव संरचना ref *remote_refs, संरचना ref_states *states)
{
	संरचना ref *ref, *matches;
	संरचना ref *fetch_map = रिक्त, **fetch_map_tail = &fetch_map;
	संरचना refspec_item refspec = {
		.force = 0,
		.pattern = 1,
		.src = (वर्ण *) "refs/heads/*",
		.dst = (वर्ण *) "refs/heads/*",
	};

	get_fetch_map(remote_refs, &refspec, &fetch_map_tail, 0);
	matches = guess_remote_head(find_ref_by_name(remote_refs, "HEAD"),
				    fetch_map, REMOTE_GUESS_HEAD_ALL);
	प्रति (ref = matches; ref; ref = ref->next)
		string_list_append(&states->heads, abbrev_branch(ref->name));

	free_refs(fetch_map);
	free_refs(matches);
	प्रत्यागम 0;
}

संरचना known_remote {
	संरचना known_remote *next;
	संरचना remote *remote;
};

संरचना known_remotes {
	संरचना remote *to_delete;
	संरचना known_remote *list;
};

स्थिर पूर्णाङ्क add_known_remote(संरचना remote *remote, शून्य *cb_data)
{
	संरचना known_remotes *all = cb_data;
	संरचना known_remote *r;

	जइ (!strcmp(all->to_delete->name, remote->name))
		प्रत्यागम 0;

	r = xmalloc(परिमाण(*r));
	r->remote = remote;
	r->next = all->list;
	all->list = r;
	प्रत्यागम 0;
}

संरचना branches_for_remote {
	संरचना remote *remote;
	संरचना string_list *branches, *skipped;
	संरचना known_remotes *keep;
};

स्थिर पूर्णाङ्क add_branch_for_removal(ध्रुव संरचना reference *ref, शून्य *cb_data)
{
	संरचना branches_for_remote *branches = cb_data;
	संरचना refspec_item refspec;
	संरचना known_remote *kr;

	memset(&refspec, 0, परिमाण(refspec));
	refspec.dst = (वर्ण *)ref->name;
	जइ (remote_find_tracking(branches->remote, &refspec))
		प्रत्यागम 0;
	मुक्त(refspec.src);

	/* don't delete a branch if another remote also uses it */
	प्रति (kr = branches->keep->list; kr; kr = kr->next) {
		memset(&refspec, 0, परिमाण(refspec));
		refspec.dst = (वर्ण *)ref->name;
		जइ (!remote_find_tracking(kr->remote, &refspec)) {
			मुक्त(refspec.src);
			प्रत्यागम 0;
		}
	}

	/* don't delete non-remote-tracking refs */
	जइ (!starts_with(ref->name, "refs/remotes/")) {
		/* advise user how to delete local branches */
		जइ (starts_with(ref->name, "refs/heads/"))
			string_list_append(branches->skipped,
					   abbrev_branch(ref->name));
		/* silently skip over other non-remote refs */
		प्रत्यागम 0;
	}

	string_list_append(branches->branches, ref->name);

	प्रत्यागम 0;
}

संरचना rename_info {
	ध्रुव वर्ण *old_name;
	ध्रुव वर्ण *new_name;
	संरचना ref_transaction *transaction;
	संरचना progress *progress;
	संरचना strbuf *err;
	uint32_t progress_nr;
	uint64_t index;
};

स्थिर शून्य compute_renamed_ref(संरचना rename_info *rename,
				ध्रुव वर्ण *refname,
				संरचना strbuf *out)
{
	strbuf_reset(out);
	strbuf_addstr(out, refname);
	strbuf_splice(out, strlen("refs/remotes/"), strlen(rename->old_name),
		      rename->new_name, strlen(rename->new_name));
}

स्थिर पूर्णाङ्क rename_one_reflog_entry(ध्रुव वर्ण *old_refname,
				   संरचना object_id *old_oid,
				   संरचना object_id *new_oid,
				   ध्रुव वर्ण *committer,
				   timestamp_t timestamp, पूर्णाङ्क tz,
				   ध्रुव वर्ण *सन्देशः, शून्य *cb_data)
{
	संरचना rename_info *rename = cb_data;
	संरचना strbuf new_refname = STRBUF_INIT;
	संरचना strbuf identity = STRBUF_INIT;
	संरचना strbuf name = STRBUF_INIT;
	संरचना strbuf mail = STRBUF_INIT;
	संरचना ident_split ident;
	ध्रुव वर्ण *date;
	पूर्णाङ्क error;

	compute_renamed_ref(rename, old_refname, &new_refname);

	जइ (split_ident_पङ्क्तिः(&ident, committer, strlen(committer)) < 0) {
		error = -1;
		गच्छ out;
	}

	strbuf_add(&name, ident.name_begin, ident.name_end - ident.name_begin);
	strbuf_add(&mail, ident.mail_begin, ident.mail_end - ident.mail_begin);

	date = show_date(timestamp, tz, DATE_MODE(NORMAL));
	strbuf_addstr(&identity, fmt_ident(name.buf, mail.buf,
					  WANT_BLANK_IDENT, date, 0));

	error = ref_transaction_update_reflog(rename->transaction, new_refname.buf,
					      new_oid, old_oid, identity.buf, सन्देशः,
					      rename->index++, rename->err);

out:
	strbuf_release(&new_refname);
	strbuf_release(&identity);
	strbuf_release(&name);
	strbuf_release(&mail);
	प्रत्यागम error;
}

स्थिर पूर्णाङ्क rename_one_reflog(ध्रुव वर्ण *old_refname,
			     ध्रुव संरचना object_id *old_oid,
			     संरचना rename_info *rename)
{
	संरचना strbuf new_refname = STRBUF_INIT;
	संरचना strbuf message = STRBUF_INIT;
	पूर्णाङ्क error;

	जइ (!refs_reflog_exists(get_main_ref_store(the_repository), old_refname))
		प्रत्यागम 0;

	error = refs_for_each_reflog_ent(get_main_ref_store(the_repository),
					 old_refname, rename_one_reflog_entry, rename);
	जइ (error < 0)
		गच्छ out;

	compute_renamed_ref(rename, old_refname, &new_refname);

	/*
	 * Manually write the reflog entry for the now-renamed ref. We cannot
	 * rely on `rename_one_ref()` to do this for us as that would screw
	 * over order in which reflog entries are being written.
	 *
	 * Furthermore, we only append the entry in case the reference
	 * resolves. Missing references shouldn't have reflogs anyway.
	 */
	strbuf_addf(&message, "remote: renamed %s to %s", old_refname,
		    new_refname.buf);

	error = ref_transaction_update_reflog(rename->transaction, new_refname.buf,
					      old_oid, old_oid, git_committer_info(0),
					      message.buf, rename->index++, rename->err);
	जइ (error < 0)
		प्रत्यागम error;

out:
	strbuf_release(&new_refname);
	strbuf_release(&message);
	प्रत्यागम error;
}

स्थिर पूर्णाङ्क rename_one_ref(ध्रुव संरचना reference *ref, शून्य *cb_data)
{
	संरचना strbuf new_referent = STRBUF_INIT;
	संरचना strbuf new_refname = STRBUF_INIT;
	संरचना rename_info *rename = cb_data;
	ध्रुव संरचना object_id *oid = ref->oid;
	ध्रुव वर्ण *referent = ref->target;
	पूर्णाङ्क error;

	compute_renamed_ref(rename, ref->name, &new_refname);

	जइ (ref->flags & REF_ISSYMREF) {
		/*
		 * Stupidly enough `referent` is not pointing to the immediate
		 * target of a symref, but it's the recursively resolved value.
		 * So symrefs pointing to symrefs would be misresolved, and
		 * unborn symrefs don't have any value for the `referent` at all.
		 */
		referent = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
						   ref->name, RESOLVE_REF_NO_RECURSE,
						   रिक्त, रिक्त);
		compute_renamed_ref(rename, referent, &new_referent);
		oid = रिक्त;
	}

	error = ref_transaction_delete(rename->transaction, ref->name,
				       oid, referent, REF_NO_DEREF, रिक्त, rename->err);
	जइ (error < 0)
		गच्छ out;

	error = ref_transaction_update(rename->transaction, new_refname.buf, oid, null_oid(the_hash_algo),
				       (ref->flags & REF_ISSYMREF) ? new_referent.buf : रिक्त, रिक्त,
				       REF_SKIP_CREATE_REFLOG | REF_NO_DEREF | REF_SKIP_OID_VERIFICATION,
				       रिक्त, rename->err);
	जइ (error < 0)
		गच्छ out;

	error = rename_one_reflog(ref->name, oid, rename);
	जइ (error < 0)
		गच्छ out;

	display_progress(rename->progress, ++rename->progress_nr);

out:
	strbuf_release(&new_referent);
	strbuf_release(&new_refname);
	प्रत्यागम error;
}

स्थिर पूर्णाङ्क migrate_file(संरचना remote *remote)
{
	संरचना strbuf buf = STRBUF_INIT;

	strbuf_addf(&buf, "remote.%s.url", remote->name);
	प्रति (size_t i = 0; i < remote->url.nr; i++)
		repo_config_set_multivar(the_repository, buf.buf, remote->url.v[i], "^$", 0);
	strbuf_reset(&buf);
	strbuf_addf(&buf, "remote.%s.push", remote->name);
	प्रति (पूर्णाङ्क i = 0; i < remote->प्रक्षिप.nr; i++)
		repo_config_set_multivar(the_repository, buf.buf, remote->प्रक्षिप.वस्तूनि[i].raw, "^$", 0);
	strbuf_reset(&buf);
	strbuf_addf(&buf, "remote.%s.fetch", remote->name);
	प्रति (पूर्णाङ्क i = 0; i < remote->fetch.nr; i++)
		repo_config_set_multivar(the_repository, buf.buf, remote->fetch.वस्तूनि[i].raw, "^$", 0);
#ifndef WITH_BREAKING_CHANGES
	जइ (remote->origin == REMOTE_REMOTES)
		unlink_or_warn(repo_git_path_replace(the_repository, &buf,
						     "remotes/%s", remote->name));
	अण्णहा जइ (remote->origin == REMOTE_BRANCHES)
		unlink_or_warn(repo_git_path_replace(the_repository, &buf,
						     "branches/%s", remote->name));
#endif /* WITH_BREAKING_CHANGES */
	strbuf_release(&buf);

	प्रत्यागम 0;
}

संरचना प्रक्षिप_default_info
{
	ध्रुव वर्ण *old_name;
	गणना config_scope scope;
	संरचना strbuf origin;
	पूर्णाङ्क linenr;
};

स्थिर पूर्णाङ्क config_read_प्रक्षिप_default(ध्रुव वर्ण *कुञ्चिका, ध्रुव वर्ण *मानम्,
	ध्रुव संरचना config_context *ctx, शून्य *cb)
{
	ध्रुव संरचना कुञ्चिका_मानम्_info *kvi = ctx->kvi;

	संरचना प्रक्षिप_default_info* info = cb;
	जइ (strcmp(कुञ्चिका, "remote.pushdefault") ||
	    !मानम् || strcmp(मानम्, info->old_name))
		प्रत्यागम 0;

	info->scope = kvi->scope;
	strbuf_reset(&info->origin);
	strbuf_addstr(&info->origin, config_origin_type_name(kvi->origin_type));
	info->linenr = kvi->linenr;

	प्रत्यागम 0;
}

स्थिर शून्य handle_प्रक्षिप_default(ध्रुव वर्ण* old_name, ध्रुव वर्ण* new_name)
{
	संरचना प्रक्षिप_default_info प्रक्षिप_default = {
		.old_name = old_name,
		.scope = CONFIG_SCOPE_UNKNOWN,
		.origin = STRBUF_INIT,
		.linenr = -1,
	};
	repo_config(the_repository, config_read_प्रक्षिप_default, &प्रक्षिप_default);
	जइ (प्रक्षिप_default.scope >= CONFIG_SCOPE_COMMAND)
		; /* pass */
	अण्णहा जइ (प्रक्षिप_default.scope >= CONFIG_SCOPE_LOCAL) {
		पूर्णाङ्क result = repo_config_set_gently(the_repository, "remote.pushDefault",
						    new_name);
		जइ (new_name && result && result != CONFIG_NOTHING_SET)
			die(_("could not set '%s'"), "remote.pushDefault");
		अण्णहा जइ (!new_name && result && result != CONFIG_NOTHING_SET)
			die(_("could not unset '%s'"), "remote.pushDefault");
	} अण्णहा जइ (प्रक्षिप_default.scope >= CONFIG_SCOPE_SYSTEM) {
		/* warn */
		warning(_("The %s configuration remote.pushDefault in:\n"
			  "\t%s:%d\n"
			  "now names the non-existent remote '%s'"),
			config_scope_name(प्रक्षिप_default.scope),
			प्रक्षिप_default.origin.buf, प्रक्षिप_default.linenr,
			old_name);
	}

	strbuf_release(&प्रक्षिप_default.origin);
}

स्थिर ध्रुव वर्ण conflicting_remote_refs_advice[] = N_(
	"The remote you are trying to rename has conflicting references in the\n"
	"new target refspec. This is most likely caused by you trying to nest\n"
	"a remote into itself, e.g. by renaming 'parent' into 'parent/child'\n"
	"or by unnesting a remote, e.g. the other way round.\n"
	"\n"
	"If that is the case, you can address this by first renaming the\n"
	"remote to a different name.\n");

स्थिर पूर्णाङ्क mv(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *prefix,
	      संरचना repository *repo UNUSED)
{
	पूर्णाङ्क show_progress = isatty(2);
	संरचना option options[] = {
		OPT_BOOL(0, "progress", &show_progress, N_("force progress reporting")),
		OPT_END()
	};
	संरचना remote *oldremote, *newremote;
	संरचना strbuf buf = STRBUF_INIT, buf2 = STRBUF_INIT, buf3 = STRBUF_INIT,
		old_remote_context = STRBUF_INIT, err = STRBUF_INIT;
	संरचना rename_info rename = {
		.err = &err,
	};
	पूर्णाङ्क refspecs_need_update = 0;
	पूर्णाङ्क result = 0;

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_rename_usage, 0);

	जइ (argc != 2)
		usage_with_options(builtin_remote_rename_usage, options);

	rename.old_name = argv[0];
	rename.new_name = argv[1];

	oldremote = remote_get(rename.old_name);
	जइ (!remote_is_configured(oldremote, 1)) {
		error(_("No such remote: '%s'"), rename.old_name);
		exit(2);
	}

	जइ (!strcmp(rename.old_name, rename.new_name) && oldremote->origin != REMOTE_CONFIG)
		प्रत्यागम migrate_file(oldremote);

	newremote = remote_get(rename.new_name);
	जइ (remote_is_configured(newremote, 1)) {
		error(_("remote %s already exists."), rename.new_name);
		exit(3);
	}

	जइ (!valid_remote_name(rename.new_name))
		die(_("'%s' is not a valid remote name"), rename.new_name);

	strbuf_addf(&buf, "remote.%s", rename.old_name);
	strbuf_addf(&buf2, "remote.%s", rename.new_name);
	जइ (repo_config_rename_section(the_repository, buf.buf, buf2.buf) < 1) {
		result = error(_("Could not rename config section '%s' to '%s'"),
			       buf.buf, buf2.buf);
		गच्छ out;
	}

	strbuf_addf(&old_remote_context, ":refs/remotes/%s/", rename.old_name);

	प्रति (पूर्णाङ्क i = 0; i < oldremote->fetch.nr && !refspecs_need_update; i++)
		refspecs_need_update = !!strstr(oldremote->fetch.वस्तूनि[i].raw,
						old_remote_context.buf);

	जइ (refspecs_need_update) {
		संरचना refs_for_each_ref_options opts = {
			.flags = REFS_FOR_EACH_INCLUDE_BROKEN,
		};
		rename.transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
							       0, &err);
		जइ (!rename.transaction)
			गच्छ out;

		जइ (show_progress)
			rename.progress = start_delayed_progress(the_repository,
								 _("Renaming remote references"), 0);

		strbuf_reset(&buf);
		strbuf_addf(&buf, "refs/remotes/%s/", rename.old_name);
		opts.prefix = buf.buf;

		result = refs_for_each_ref_ext(get_main_ref_store(the_repository),
					       rename_one_ref, &rename, &opts);
		जइ (result < 0)
			die(_("queueing remote ref renames failed: %s"), rename.err->buf);

		result = ref_transaction_prepare(rename.transaction, &err);
		जइ (result < 0) {
			error("renaming remote references failed: %s", err.buf);
			जइ (result == REF_TRANSACTION_ERROR_NAME_CONFLICT)
				advise(conflicting_remote_refs_advice);
			die(रिक्त);
		}
	}

	जइ (oldremote->fetch.nr) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.fetch", rename.new_name);
		repo_config_set_multivar(the_repository, buf.buf, रिक्त, रिक्त, CONFIG_FLAGS_MULTI_REPLACE);
		प्रति (पूर्णाङ्क i = 0; i < oldremote->fetch.nr; i++) {
			वर्ण *ptr;

			strbuf_reset(&buf2);
			strbuf_addstr(&buf2, oldremote->fetch.वस्तूनि[i].raw);
			ptr = strstr(buf2.buf, old_remote_context.buf);
			जइ (ptr) {
				strbuf_splice(&buf2,
					      ptr-buf2.buf + strlen(":refs/remotes/"),
					      strlen(rename.old_name), rename.new_name,
					      strlen(rename.new_name));
			} अण्णहा
				warning(_("Not updating non-default fetch refspec\n"
					  "\t%s\n"
					  "\tPlease update the configuration manually if necessary."),
					buf2.buf);

			repo_config_set_multivar(the_repository, buf.buf, buf2.buf, "^$", 0);
		}
	}

	read_branches();
	प्रति (size_t i = 0; i < branch_list.nr; i++) {
		संरचना string_list_item *item = branch_list.वस्तूनि + i;
		संरचना branch_info *info = item->util;
		जइ (info->remote_name && !strcmp(info->remote_name, rename.old_name)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "branch.%s.remote", item->string);
			repo_config_set(the_repository, buf.buf, rename.new_name);
		}
		जइ (info->प्रक्षिप_remote_name && !strcmp(info->प्रक्षिप_remote_name, rename.old_name)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "branch.%s.pushRemote", item->string);
			repo_config_set(the_repository, buf.buf, rename.new_name);
		}
	}

	जइ (refspecs_need_update) {
		result = ref_transaction_commit(rename.transaction, &err);
		जइ (result < 0)
			die(_("renaming remote refs failed: %s"), rename.err->buf);

		stop_progress(&rename.progress);

		handle_प्रक्षिप_default(rename.old_name, rename.new_name);
	}

out:
	ref_transaction_free(rename.transaction);
	strbuf_release(&old_remote_context);
	strbuf_release(&buf);
	strbuf_release(&buf2);
	strbuf_release(&buf3);
	strbuf_release(&err);
	प्रत्यागम result;
}

स्थिर पूर्णाङ्क rm(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *prefix,
	      संरचना repository *repo UNUSED)
{
	संरचना option options[] = {
		OPT_END()
	};
	संरचना remote *remote;
	संरचना strbuf buf = STRBUF_INIT;
	संरचना known_remotes known_remotes = { रिक्त, रिक्त };
	संरचना string_list branches = STRING_LIST_INIT_DUP;
	संरचना string_list skipped = STRING_LIST_INIT_DUP;
	संरचना branches_for_remote cb_data;
	पूर्णाङ्क result;

	memset(&cb_data, 0, परिमाण(cb_data));
	cb_data.branches = &branches;
	cb_data.skipped = &skipped;
	cb_data.keep = &known_remotes;

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_rm_usage, 0);
	जइ (argc != 1)
		usage_with_options(builtin_remote_rm_usage, options);

	remote = remote_get(argv[0]);
	जइ (!remote_is_configured(remote, 1)) {
		error(_("No such remote: '%s'"), argv[0]);
		exit(2);
	}

	known_remotes.to_delete = remote;
	for_each_remote(add_known_remote, &known_remotes);

	read_branches();
	प्रति (size_t i = 0; i < branch_list.nr; i++) {
		संरचना string_list_item *item = branch_list.वस्तूनि + i;
		संरचना branch_info *info = item->util;
		जइ (info->remote_name && !strcmp(info->remote_name, remote->name)) {
			ध्रुव वर्ण *कुञ्चिकाः[] = { "remote", "merge", रिक्त }, **k;
			प्रति (k = कुञ्चिकाः; *k; k++) {
				strbuf_reset(&buf);
				strbuf_addf(&buf, "branch.%s.%s",
						item->string, *k);
				result = repo_config_set_gently(the_repository, buf.buf, रिक्त);
				जइ (result && result != CONFIG_NOTHING_SET)
					die(_("could not unset '%s'"), buf.buf);
			}
		}
		जइ (info->प्रक्षिप_remote_name && !strcmp(info->प्रक्षिप_remote_name, remote->name)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "branch.%s.pushremote", item->string);
			result = repo_config_set_gently(the_repository, buf.buf, रिक्त);
			जइ (result && result != CONFIG_NOTHING_SET)
				die(_("could not unset '%s'"), buf.buf);
		}
	}

	/*
	 * We cannot just pass a function to for_each_ref() which deletes
	 * the branches one by one, since for_each_ref() relies on cached
	 * refs, which are invalidated when deleting a branch.
	 */
	cb_data.remote = remote;
	result = refs_for_each_ref(get_main_ref_store(the_repository),
				   add_branch_for_removal, &cb_data);
	strbuf_release(&buf);

	जइ (!result)
		result = refs_delete_refs(get_main_ref_store(the_repository),
					  "remote: remove", &branches,
					  REF_NO_DEREF);
	string_list_clear(&branches, 0);

	जइ (skipped.nr) {
		fprintf_ln(stderr,
			   Q_("Note: A branch outside the refs/remotes/ hierarchy was not removed;\n"
			      "to delete it, use:",
			      "Note: Some branches outside the refs/remotes/ hierarchy were not removed;\n"
			      "to delete them, use:",
			      skipped.nr));
		प्रति (size_t i = 0; i < skipped.nr; i++)
			fprintf(stderr, "  git branch -d %s\n",
				skipped.वस्तूनि[i].string);
	}
	string_list_clear(&skipped, 0);

	जइ (!result) {
		strbuf_addf(&buf, "remote.%s", remote->name);
		जइ (repo_config_rename_section(the_repository, buf.buf, रिक्त) < 1) {
			result = error(_("Could not remove config section '%s'"), buf.buf);
			गच्छ out;
		}

		handle_प्रक्षिप_default(remote->name, रिक्त);
	}

out:
	प्रति (संरचना known_remote *r = known_remotes.list; r;) {
		संरचना known_remote *next = r->next;
		मुक्त(r);
		r = next;
	}
	strbuf_release(&buf);
	प्रत्यागम result;
}

स्थिर शून्य clear_प्रक्षिप_info(शून्य *util, ध्रुव वर्ण *string UNUSED)
{
	संरचना प्रक्षिप_info *info = util;
	मुक्त(info->dest);
	मुक्त(info);
}

स्थिर शून्य free_remote_ref_states(संरचना ref_states *states)
{
	string_list_clear(&states->new_refs, 0);
	string_list_clear(&states->skipped, 0);
	string_list_clear(&states->stale, 1);
	string_list_clear(&states->tracked, 0);
	string_list_clear(&states->heads, 0);
	string_list_clear_func(&states->प्रक्षिप, clear_प्रक्षिप_info);
}

स्थिर पूर्णाङ्क append_ref_to_tracked_list(ध्रुव संरचना reference *ref, शून्य *cb_data)
{
	संरचना ref_states *states = cb_data;
	संरचना refspec_item refspec;

	जइ (ref->flags & REF_ISSYMREF)
		प्रत्यागम 0;

	memset(&refspec, 0, परिमाण(refspec));
	refspec.dst = (वर्ण *)ref->name;
	जइ (!remote_find_tracking(states->remote, &refspec)) {
		string_list_append(&states->tracked, abbrev_branch(refspec.src));
		मुक्त(refspec.src);
	}

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क get_remote_ref_states(ध्रुव वर्ण *name,
				 संरचना ref_states *states,
				 पूर्णाङ्क query)
{
	states->remote = remote_get(name);
	जइ (!states->remote)
		प्रत्यागम error(_("No such remote: '%s'"), name);

	read_branches();

	जइ (query) {
		संरचना transport *transport;
		ध्रुव संरचना ref *remote_refs;

		transport = transport_get(states->remote, states->remote->url.v[0]);
		remote_refs = transport_get_remote_refs(transport, रिक्त);

		states->queried = 1;
		जइ (query & GET_REF_STATES)
			get_ref_states(remote_refs, states);
		जइ (query & GET_HEAD_NAMES)
			get_head_names(remote_refs, states);
		जइ (query & GET_प्रक्षिप_REF_STATES)
			get_प्रक्षिप_ref_states(remote_refs, states);
		transport_disconnect(transport);
	} अण्णहा {
		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_ref_to_tracked_list, states);
		string_list_sort(&states->tracked);
		get_प्रक्षिप_ref_states_noquery(states);
	}

	प्रत्यागम 0;
}

संरचना show_info {
	संरचना string_list list;
	संरचना ref_states states;
	पूर्णाङ्क width, width2;
	पूर्णाङ्क any_rebase;
};

#define SHOW_INFO_INIT { \
	.list = STRING_LIST_INIT_DUP, \
	.states = REF_STATES_INIT, \
}

स्थिर पूर्णाङ्क add_remote_to_show_info(संरचना string_list_item *item, शून्य *cb_data)
{
	संरचना show_info *info = cb_data;
	पूर्णाङ्क n = strlen(item->string);
	जइ (n > info->width)
		info->width = n;
	string_list_निवेशय(&info->list, item->string);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क show_remote_info_item(संरचना string_list_item *item, शून्य *cb_data)
{
	संरचना show_info *info = cb_data;
	संरचना ref_states *states = &info->states;
	ध्रुव वर्ण *name = item->string;

	जइ (states->queried) {
		ध्रुव वर्ण *fmt = "%s";
		ध्रुव वर्ण *arg = "";
		जइ (string_list_has_string(&states->new_refs, name)) {
			fmt = _(" new (next fetch will store in remotes/%s)");
			arg = states->remote->name;
		} अण्णहा जइ (string_list_has_string(&states->tracked, name))
			arg = _(" tracked");
		अण्णहा जइ (string_list_has_string(&states->skipped, name))
			arg = _(" skipped");
		अण्णहा जइ (string_list_has_string(&states->stale, name))
			arg = _(" stale (use 'git remote prune' to remove)");
		अण्णहा
			arg = _(" ???");
		printf("    %-*s", info->width, name);
		printf(fmt, arg);
		printf("\n");
	} अण्णहा
		printf("    %s\n", name);

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क add_local_to_show_info(संरचना string_list_item *branch_item, शून्य *cb_data)
{
	संरचना show_info *show_info = cb_data;
	संरचना ref_states *states = &show_info->states;
	संरचना branch_info *branch_info = branch_item->util;
	संरचना string_list_item *item;
	पूर्णाङ्क n;

	जइ (!branch_info->merge.nr || !branch_info->remote_name ||
	    strcmp(states->remote->name, branch_info->remote_name))
		प्रत्यागम 0;
	जइ ((n = strlen(branch_item->string)) > show_info->width)
		show_info->width = n;
	जइ (branch_info->rebase >= REBASE_TRUE)
		show_info->any_rebase = 1;

	item = string_list_निवेशय(&show_info->list, branch_item->string);
	item->util = branch_info;

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क show_local_info_item(संरचना string_list_item *item, शून्य *cb_data)
{
	संरचना show_info *show_info = cb_data;
	संरचना branch_info *branch_info = item->util;
	संरचना string_list *merge = &branch_info->merge;
	पूर्णाङ्क width = show_info->width + 4;

	जइ (branch_info->rebase >= REBASE_TRUE && branch_info->merge.nr > 1) {
		error(_("invalid branch.%s.merge; cannot rebase onto > 1 branch"),
			item->string);
		प्रत्यागम 0;
	}

	printf("    %-*s ", show_info->width, item->string);
	जइ (branch_info->rebase >= REBASE_TRUE) {
		ध्रुव वर्ण *सन्देशः;
		जइ (branch_info->rebase == REBASE_INTERACTIVE)
			सन्देशः = _("rebases interactively onto remote %s");
		अण्णहा जइ (branch_info->rebase == REBASE_MERGES)
			सन्देशः = _("rebases interactively (with merges) onto "
				"remote %s");
		अण्णहा
			सन्देशः = _("rebases onto remote %s");
		printf_ln(सन्देशः, merge->वस्तूनि[0].string);
		प्रत्यागम 0;
	} अण्णहा जइ (show_info->any_rebase) {
		printf_ln(_(" merges with remote %s"), merge->वस्तूनि[0].string);
		width++;
	} अण्णहा {
		printf_ln(_("merges with remote %s"), merge->वस्तूनि[0].string);
	}
	प्रति (size_t i = 1; i < merge->nr; i++)
		printf(_("%-*s    and with remote %s\n"), width, "",
		       merge->वस्तूनि[i].string);

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क add_प्रक्षिप_to_show_info(संरचना string_list_item *प्रक्षिप_item, शून्य *cb_data)
{
	संरचना show_info *show_info = cb_data;
	संरचना प्रक्षिप_info *प्रक्षिप_info = प्रक्षिप_item->util;
	संरचना string_list_item *item;
	पूर्णाङ्क n;
	जइ ((n = strlen(प्रक्षिप_item->string)) > show_info->width)
		show_info->width = n;
	जइ ((n = strlen(प्रक्षिप_info->dest)) > show_info->width2)
		show_info->width2 = n;
	item = string_list_append(&show_info->list, प्रक्षिप_item->string);
	item->util = प्रक्षिप_item->util;
	प्रत्यागम 0;
}

/*
 * Sorting comparison for a string list that has push_info
 * structs in its util field
 */
स्थिर पूर्णाङ्क cmp_string_with_प्रक्षिप(ध्रुव शून्य *va, ध्रुव शून्य *vb)
{
	ध्रुव संरचना string_list_item *a = va;
	ध्रुव संरचना string_list_item *b = vb;
	ध्रुव संरचना प्रक्षिप_info *a_प्रक्षिप = a->util;
	ध्रुव संरचना प्रक्षिप_info *b_प्रक्षिप = b->util;
	पूर्णाङ्क cmp = strcmp(a->string, b->string);
	प्रत्यागम cmp ? cmp : strcmp(a_प्रक्षिप->dest, b_प्रक्षिप->dest);
}

स्थिर पूर्णाङ्क show_प्रक्षिप_info_item(संरचना string_list_item *item, शून्य *cb_data)
{
	संरचना show_info *show_info = cb_data;
	संरचना प्रक्षिप_info *प्रक्षिप_info = item->util;
	ध्रुव वर्ण *src = item->string, *status = रिक्त;

	विकल्प (प्रक्षिप_info->status) {
	पक्ष प्रक्षिप_STATUS_CREATE:
		status = _("create");
		भङ्ग;
	पक्ष प्रक्षिप_STATUS_DELETE:
		status = _("delete");
		src = _("(none)");
		भङ्ग;
	पक्ष प्रक्षिप_STATUS_UPTODATE:
		status = _("up to date");
		भङ्ग;
	पक्ष प्रक्षिप_STATUS_FASTFORWARD:
		status = _("fast-forwardable");
		भङ्ग;
	पक्ष प्रक्षिप_STATUS_OUTOFDATE:
		status = _("local out of date");
		भङ्ग;
	पक्ष प्रक्षिप_STATUS_NOTQUERIED:
		भङ्ग;
	}
	जइ (status) {
		जइ (प्रक्षिप_info->forced)
			printf_ln(_("    %-*s forces to %-*s (%s)"), show_info->width, src,
			       show_info->width2, प्रक्षिप_info->dest, status);
		अण्णहा
			printf_ln(_("    %-*s pushes to %-*s (%s)"), show_info->width, src,
			       show_info->width2, प्रक्षिप_info->dest, status);
	} अण्णहा {
		जइ (प्रक्षिप_info->forced)
			printf_ln(_("    %-*s forces to %s"), show_info->width, src,
			       प्रक्षिप_info->dest);
		अण्णहा
			printf_ln(_("    %-*s pushes to %s"), show_info->width, src,
			       प्रक्षिप_info->dest);
	}
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क get_one_entry(संरचना remote *remote, शून्य *priv)
{
	संरचना string_list *list = priv;
	संरचना strbuf remote_info_buf = STRBUF_INIT;
	संरचना strvec *url;

	जइ (remote->url.nr > 0) {
		संरचना strbuf promisor_config = STRBUF_INIT;
		ध्रुव वर्ण *partial_clone_filter = रिक्त;

		strbuf_addf(&promisor_config, "remote.%s.partialclonefilter", remote->name);
		strbuf_addf(&remote_info_buf, "%s (fetch)", remote->url.v[0]);
		जइ (!repo_config_get_string_tmp(the_repository, promisor_config.buf, &partial_clone_filter))
			strbuf_addf(&remote_info_buf, " [%s]", partial_clone_filter);

		strbuf_release(&promisor_config);
		string_list_append(list, remote->name)->util =
				strbuf_detach(&remote_info_buf, रिक्त);
	} अण्णहा
		string_list_append(list, remote->name)->util = रिक्त;
	url = प्रक्षिप_url_of_remote(remote);
	प्रति (size_t i = 0; i < url->nr; i++) {
		strbuf_addf(&remote_info_buf, "%s (push)", url->v[i]);
		string_list_append(list, remote->name)->util =
				strbuf_detach(&remote_info_buf, रिक्त);
	}

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क show_all(शून्य)
{
	संरचना string_list list = STRING_LIST_INIT_DUP;
	पूर्णाङ्क result;

	result = for_each_remote(get_one_entry, &list);

	जइ (!result) {
		string_list_sort(&list);
		प्रति (size_t i = 0; i < list.nr; i++) {
			संरचना string_list_item *item = list.वस्तूनि + i;
			जइ (verbose)
				printf("%s\t%s\n", item->string,
					item->util ? (ध्रुव वर्ण *)item->util : "");
			अण्णहा {
				जइ (i && !strcmp((item - 1)->string, item->string))
					अनुवर्त;
				printf("%s\n", item->string);
			}
		}
	}
	string_list_clear(&list, 1);
	प्रत्यागम result;
}

स्थिर पूर्णाङ्क show(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *prefix,
		संरचना repository *repo UNUSED)
{
	पूर्णाङ्क no_query = 0, result = 0, query_flag = 0;
	संरचना option options[] = {
		OPT_BOOL('n', रिक्त, &no_query, N_("do not query remotes")),
		OPT_END()
	};
	संरचना show_info info = SHOW_INFO_INIT;

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_show_usage,
			     0);

	जइ (argc < 1)
		प्रत्यागम show_all();

	जइ (!no_query)
		query_flag = (GET_REF_STATES | GET_HEAD_NAMES | GET_प्रक्षिप_REF_STATES);

	प्रति (; argc; argc--, argv++) {
		size_t i;
		संरचना strvec *url;

		get_remote_ref_states(*argv, &info.states, query_flag);

		printf_ln(_("* remote %s"), *argv);
		printf_ln(_("  Fetch URL: %s"), info.states.remote->url.v[0]);
		url = प्रक्षिप_url_of_remote(info.states.remote);
		प्रति (i = 0; i < url->nr; i++)
			/*
			 * TRANSLATORS: the colon ':' should align
			 * with the one in " Fetch URL: %s"
			 * translation.
			 */
			printf_ln(_("  Push  URL: %s"), url->v[i]);
		जइ (!i)
			printf_ln(_("  Push  URL: %s"), _("(no URL)"));
		जइ (no_query)
			printf_ln(_("  HEAD branch: %s"), _("(not queried)"));
		अण्णहा जइ (!info.states.heads.nr)
			printf_ln(_("  HEAD branch: %s"), _("(unknown)"));
		अण्णहा जइ (info.states.heads.nr == 1)
			printf_ln(_("  HEAD branch: %s"), info.states.heads.वस्तूनि[0].string);
		अण्णहा {
			printf(_("  HEAD branch (remote HEAD is ambiguous,"
				 " may be one of the following):\n"));
			प्रति (i = 0; i < info.states.heads.nr; i++)
				printf("    %s\n", info.states.heads.वस्तूनि[i].string);
		}

		/* remote branch info */
		info.width = 0;
		for_each_string_list(&info.states.new_refs, add_remote_to_show_info, &info);
		for_each_string_list(&info.states.skipped, add_remote_to_show_info, &info);
		for_each_string_list(&info.states.tracked, add_remote_to_show_info, &info);
		for_each_string_list(&info.states.stale, add_remote_to_show_info, &info);
		जइ (info.list.nr)
			printf_ln(Q_("  Remote branch:%s",
				     "  Remote branches:%s",
				     info.list.nr),
				  no_query ? _(" (status not queried)") : "");
		for_each_string_list(&info.list, show_remote_info_item, &info);
		string_list_clear(&info.list, 0);

		/* git pull info */
		info.width = 0;
		info.any_rebase = 0;
		for_each_string_list(&branch_list, add_local_to_show_info, &info);
		जइ (info.list.nr)
			printf_ln(Q_("  Local branch configured for 'git pull':",
				     "  Local branches configured for 'git pull':",
				     info.list.nr));
		for_each_string_list(&info.list, show_local_info_item, &info);
		string_list_clear(&info.list, 0);

		/* git push info */
		जइ (info.states.remote->mirror)
			printf_ln(_("  Local refs will be mirrored by 'git push'"));

		info.width = info.width2 = 0;
		for_each_string_list(&info.states.प्रक्षिप, add_प्रक्षिप_to_show_info, &info);
		QSORT(info.list.वस्तूनि, info.list.nr, cmp_string_with_प्रक्षिप);
		जइ (info.list.nr)
			printf_ln(Q_("  Local ref configured for 'git push'%s:",
				     "  Local refs configured for 'git push'%s:",
				     info.list.nr),
				  no_query ? _(" (status not queried)") : "");
		for_each_string_list(&info.list, show_प्रक्षिप_info_item, &info);
		string_list_clear(&info.list, 0);

		free_remote_ref_states(&info.states);
	}

	प्रत्यागम result;
}

स्थिर शून्य report_set_head_auto(ध्रुव वर्ण *remote, ध्रुव वर्ण *head_name,
			संरचना strbuf *b_local_head, पूर्णाङ्क was_detached) {
	संरचना strbuf buf_prefix = STRBUF_INIT;
	ध्रुव वर्ण *prev_head = रिक्त;

	strbuf_addf(&buf_prefix, "refs/remotes/%s/", remote);
	skip_prefix(b_local_head->buf, buf_prefix.buf, &prev_head);

	जइ (prev_head && !strcmp(prev_head, head_name))
		printf(_("'%s/HEAD' is unchanged and points to '%s'\n"),
			remote, head_name);
	अण्णहा जइ (prev_head)
		printf(_("'%s/HEAD' has changed from '%s' and now points to '%s'\n"),
			remote, prev_head, head_name);
	अण्णहा जइ (!b_local_head->len)
		printf(_("'%s/HEAD' is now created and points to '%s'\n"),
			remote, head_name);
	अण्णहा जइ (was_detached && b_local_head->len)
		printf(_("'%s/HEAD' was detached at '%s' and now points to '%s'\n"),
			remote, b_local_head->buf, head_name);
	अण्णहा
		printf(_("'%s/HEAD' used to point to '%s' "
			"(which is not a remote branch), but now points to '%s'\n"),
			remote, b_local_head->buf, head_name);
	strbuf_release(&buf_prefix);
}

स्थिर पूर्णाङ्क set_head(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *prefix,
		    संरचना repository *repo UNUSED)
{
	पूर्णाङ्क opt_a = 0, opt_d = 0, result = 0, was_detached;
	संरचना strbuf b_head = STRBUF_INIT, b_remote_head = STRBUF_INIT,
		b_local_head = STRBUF_INIT;
	वर्ण *head_name = रिक्त;
	संरचना ref_store *refs = get_main_ref_store(the_repository);
	संरचना remote *remote;

	संरचना option options[] = {
		OPT_BOOL('a', "auto", &opt_a,
			 N_("set refs/remotes/<name>/HEAD according to remote")),
		OPT_BOOL('d', "delete", &opt_d,
			 N_("delete refs/remotes/<name>/HEAD")),
		OPT_END()
	};
	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_sethead_usage, 0);

	/* All modes require at least a remote name. */
	जइ (!argc)
		usage_with_options(builtin_remote_sethead_usage, options);

	strbuf_addf(&b_head, "refs/remotes/%s/HEAD", argv[0]);
	remote = remote_get(argv[0]);

	जइ (!opt_a && !opt_d && argc == 2) {
		head_name = xstrdup(argv[1]);
	} अण्णहा जइ (opt_a && !opt_d && argc == 1) {
		संरचना ref_states states = REF_STATES_INIT;
		get_remote_ref_states(argv[0], &states, GET_HEAD_NAMES);
		जइ (!states.heads.nr)
			result |= error(_("Cannot determine remote HEAD"));
		अण्णहा जइ (states.heads.nr > 1) {
			result |= error(_("Multiple remote HEAD branches. "
					  "Please choose one explicitly with:"));
			प्रति (size_t i = 0; i < states.heads.nr; i++)
				fprintf(stderr, "  git remote set-head %s %s\n",
					argv[0], states.heads.वस्तूनि[i].string);
		} अण्णहा
			head_name = xstrdup(states.heads.वस्तूनि[0].string);
		free_remote_ref_states(&states);
	} अण्णहा जइ (opt_d && !opt_a && argc == 1) {
		जइ (refs_delete_ref(refs, रिक्त, b_head.buf, रिक्त, REF_NO_DEREF))
			result |= error(_("Could not delete %s"), b_head.buf);
	} अण्णहा
		usage_with_options(builtin_remote_sethead_usage, options);

	जइ (!head_name)
		गच्छ cleanup;
	strbuf_addf(&b_remote_head, "refs/remotes/%s/%s", argv[0], head_name);
	जइ (!refs_ref_exists(refs, b_remote_head.buf)) {
		result |= error(_("Not a valid ref: %s"), b_remote_head.buf);
		गच्छ cleanup;
	}
	was_detached = refs_update_symref_extended(refs, b_head.buf, b_remote_head.buf,
			"remote set-head", &b_local_head, 0);
	जइ (was_detached == -1) {
		result |= error(_("Could not set up %s"), b_head.buf);
		गच्छ cleanup;
	}
	जइ (opt_a)
		report_set_head_auto(argv[0], head_name, &b_local_head, was_detached);
	जइ (remote->follow_remote_head == FOLLOW_REMOTE_ALWAYS) {
		संरचना strbuf config_name = STRBUF_INIT;
		strbuf_addf(&config_name,
			"remote.%s.followremotehead", remote->name);
		repo_config_set(the_repository, config_name.buf, "warn");
		strbuf_release(&config_name);
	}

cleanup:
	मुक्त(head_name);
	strbuf_release(&b_head);
	strbuf_release(&b_remote_head);
	strbuf_release(&b_local_head);
	प्रत्यागम result;
}

स्थिर पूर्णाङ्क prune_remote(ध्रुव वर्ण *remote, पूर्णाङ्क dry_run)
{
	पूर्णाङ्क result = 0;
	संरचना ref_states states = REF_STATES_INIT;
	संरचना string_list refs_to_prune = STRING_LIST_INIT_NODUP;
	संरचना string_list_item *item;

	get_remote_ref_states(remote, &states, GET_REF_STATES);

	जइ (!states.stale.nr) {
		free_remote_ref_states(&states);
		प्रत्यागम 0;
	}

	printf_ln(_("Pruning %s"), remote);
	printf_ln(_("URL: %s"), states.remote->url.v[0]);

	for_each_string_list_item(item, &states.stale)
		string_list_append(&refs_to_prune, item->util);
	string_list_sort(&refs_to_prune);

	जइ (!dry_run)
		result |= refs_delete_refs(get_main_ref_store(the_repository),
					   "remote: prune", &refs_to_prune, 0);

	for_each_string_list_item(item, &states.stale) {
		ध्रुव वर्ण *refname = item->util;

		जइ (dry_run)
			printf_ln(_(" * [would prune] %s"),
			       abbrev_ref(refname, "refs/remotes/"));
		अण्णहा
			printf_ln(_(" * [pruned] %s"),
			       abbrev_ref(refname, "refs/remotes/"));
	}

	refs_warn_dangling_symrefs(get_main_ref_store(the_repository),
				   stdout, " ", dry_run, &refs_to_prune);

	string_list_clear(&refs_to_prune, 0);
	free_remote_ref_states(&states);
	प्रत्यागम result;
}

स्थिर पूर्णाङ्क prune(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *prefix,
		 संरचना repository *repo UNUSED)
{
	पूर्णाङ्क dry_run = 0, result = 0;
	संरचना option options[] = {
		OPT__DRY_RUN(&dry_run, N_("dry run")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_prune_usage, 0);

	जइ (argc < 1)
		usage_with_options(builtin_remote_prune_usage, options);

	प्रति (; argc; argc--, argv++)
		result |= prune_remote(*argv, dry_run);

	प्रत्यागम result;
}

स्थिर पूर्णाङ्क get_remote_default(ध्रुव वर्ण *कुञ्चिका, ध्रुव वर्ण *मानम् UNUSED,
			      ध्रुव संरचना config_context *ctx UNUSED,
			      शून्य *priv)
{
	जइ (strcmp(कुञ्चिका, "remotes.default") == 0) {
		पूर्णाङ्क *found = priv;
		*found = 1;
	}
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क update(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *prefix,
		  संरचना repository *repo UNUSED)
{
	पूर्णाङ्क i, prune = -1;
	संरचना option options[] = {
		OPT_BOOL('p', "prune", &prune,
			 N_("prune remotes after fetching")),
		OPT_END()
	};
	संरचना child_process cmd = CHILD_PROCESS_INIT;
	पूर्णाङ्क default_defined = 0;

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_update_usage,
			     PARSE_OPT_KEEP_ARGV0);

	strvec_प्रक्षिप(&cmd.args, "fetch");

	जइ (prune != -1)
		strvec_प्रक्षिप(&cmd.args, prune ? "--prune" : "--no-prune");
	जइ (verbose)
		strvec_प्रक्षिप(&cmd.args, "-v");
	strvec_प्रक्षिप(&cmd.args, "--multiple");
	जइ (argc < 2)
		strvec_प्रक्षिप(&cmd.args, "default");
	प्रति (i = 1; i < argc; i++)
		strvec_प्रक्षिप(&cmd.args, argv[i]);

	जइ (strcmp(cmd.args.v[cmd.args.nr-1], "default") == 0) {
		repo_config(the_repository, get_remote_default, &default_defined);
		जइ (!default_defined) {
			strvec_निष्कासय(&cmd.args);
			strvec_प्रक्षिप(&cmd.args, "--all");
		}
	}

	cmd.git_cmd = 1;
	प्रत्यागम run_command(&cmd);
}

स्थिर पूर्णाङ्क remove_all_fetch_refspecs(ध्रुव वर्ण *कुञ्चिका)
{
	प्रत्यागम repo_config_set_multivar_gently(the_repository, कुञ्चिका, रिक्त, रिक्त,
					       CONFIG_FLAGS_MULTI_REPLACE);
}

स्थिर शून्य add_branches(संरचना remote *remote, ध्रुव वर्ण **branches,
			 ध्रुव वर्ण *कुञ्चिका)
{
	ध्रुव वर्ण *remotename = remote->name;
	पूर्णाङ्क mirror = remote->mirror;
	संरचना strbuf refspec = STRBUF_INIT;

	प्रति (; *branches; branches++)
		add_branch(कुञ्चिका, *branches, remotename, mirror, &refspec);

	strbuf_release(&refspec);
}

स्थिर पूर्णाङ्क set_remote_branches(ध्रुव वर्ण *remotename, ध्रुव वर्ण **branches,
				पूर्णाङ्क add_mode)
{
	संरचना strbuf कुञ्चिका = STRBUF_INIT;
	संरचना remote *remote;

	strbuf_addf(&कुञ्चिका, "remote.%s.fetch", remotename);

	remote = remote_get(remotename);
	जइ (!remote_is_configured(remote, 1)) {
		error(_("No such remote '%s'"), remotename);
		exit(2);
	}

	जइ (!add_mode && remove_all_fetch_refspecs(कुञ्चिका.buf)) {
		strbuf_release(&कुञ्चिका);
		प्रत्यागम 1;
	}
	add_branches(remote, branches, कुञ्चिका.buf);

	strbuf_release(&कुञ्चिका);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क set_branches(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *prefix,
			संरचना repository *repo UNUSED)
{
	पूर्णाङ्क add_mode = 0;
	संरचना option options[] = {
		OPT_BOOL('\0', "add", &add_mode, N_("add branch")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_setbranches_usage, 0);
	जइ (argc == 0) {
		error(_("no remote specified"));
		usage_with_options(builtin_remote_setbranches_usage, options);
	}
	argv[argc] = रिक्त;

	प्रत्यागम set_remote_branches(argv[0], argv + 1, add_mode);
}

स्थिर पूर्णाङ्क get_url(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *prefix,
		   संरचना repository *repo UNUSED)
{
	पूर्णाङ्क प्रक्षिप_mode = 0, all_mode = 0;
	ध्रुव वर्ण *remotename = रिक्त;
	संरचना remote *remote;
	संरचना strvec *url;
	संरचना option options[] = {
		OPT_BOOL('\0', "push", &प्रक्षिप_mode,
			 N_("query push URLs rather than fetch URLs")),
		OPT_BOOL('\0', "all", &all_mode,
			 N_("return all URLs")),
		OPT_END()
	};
	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_geturl_usage, 0);

	जइ (argc != 1)
		usage_with_options(builtin_remote_geturl_usage, options);

	remotename = argv[0];

	remote = remote_get(remotename);
	जइ (!remote_is_configured(remote, 1)) {
		error(_("No such remote '%s'"), remotename);
		exit(2);
	}

	url = प्रक्षिप_mode ? प्रक्षिप_url_of_remote(remote) : &remote->url;

	जइ (all_mode) {
		प्रति (size_t i = 0; i < url->nr; i++)
			printf_ln("%s", url->v[i]);
	} अण्णहा {
		printf_ln("%s", url->v[0]);
	}

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क set_url(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *prefix,
		   संरचना repository *repo UNUSED)
{
	पूर्णाङ्क प्रक्षिप_mode = 0, add_mode = 0, delete_mode = 0;
	पूर्णाङ्क matches = 0, negative_matches = 0;
	ध्रुव वर्ण *remotename = रिक्त;
	ध्रुव वर्ण *newurl = रिक्त;
	ध्रुव वर्ण *oldurl = रिक्त;
	संरचना remote *remote;
	regex_t old_regex;
	संरचना strvec *urlset;
	संरचना strbuf name_buf = STRBUF_INIT;
	संरचना option options[] = {
		OPT_BOOL('\0', "push", &प्रक्षिप_mode,
			 N_("manipulate push URLs")),
		OPT_BOOL('\0', "add", &add_mode,
			 N_("add URL")),
		OPT_BOOL('\0', "delete", &delete_mode,
			    N_("delete URLs")),
		OPT_END()
	};
	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_seturl_usage,
			     PARSE_OPT_KEEP_ARGV0);

	जइ (add_mode && delete_mode)
		die(_("--add --delete doesn't make sense"));

	जइ (argc < 3 || argc > 4 || ((add_mode || delete_mode) && argc != 3))
		usage_with_options(builtin_remote_seturl_usage, options);

	remotename = argv[1];
	newurl = argv[2];
	जइ (argc > 3)
		oldurl = argv[3];

	जइ (delete_mode)
		oldurl = newurl;

	remote = remote_get(remotename);
	जइ (!remote_is_configured(remote, 1)) {
		error(_("No such remote '%s'"), remotename);
		exit(2);
	}

	जइ (प्रक्षिप_mode) {
		strbuf_addf(&name_buf, "remote.%s.pushurl", remotename);
		urlset = &remote->pushurl;
	} अण्णहा {
		strbuf_addf(&name_buf, "remote.%s.url", remotename);
		urlset = &remote->url;
	}

	/* Special cases that add new entry. */
	जइ ((!oldurl && !delete_mode) || add_mode) {
		जइ (add_mode)
			repo_config_set_multivar(the_repository, name_buf.buf, newurl,
						       "^$", 0);
		अण्णहा
			repo_config_set(the_repository, name_buf.buf, newurl);
		गच्छ out;
	}

	/* Old URL specified. Demand that one matches. */
	जइ (regcomp(&old_regex, oldurl, REG_EXTENDED))
		die(_("Invalid old URL pattern: %s"), oldurl);

	प्रति (size_t i = 0; i < urlset->nr; i++)
		जइ (!regexec(&old_regex, urlset->v[i], 0, रिक्त, 0))
			matches++;
		अण्णहा
			negative_matches++;
	जइ (!delete_mode && !matches)
		die(_("No such URL found: %s"), oldurl);
	जइ (delete_mode && !negative_matches && !प्रक्षिप_mode)
		die(_("Will not delete all non-push URLs"));

	regfree(&old_regex);

	जइ (!delete_mode)
		repo_config_set_multivar(the_repository, name_buf.buf, newurl, oldurl, 0);
	अण्णहा
		repo_config_set_multivar(the_repository, name_buf.buf, रिक्त, oldurl,
					 CONFIG_FLAGS_MULTI_REPLACE);
out:
	strbuf_release(&name_buf);
	प्रत्यागम 0;
}

पूर्णाङ्क cmd_remote(पूर्णाङ्क argc,
	       ध्रुव वर्ण **argv,
	       ध्रुव वर्ण *prefix,
	       संरचना repository *repo)
{
	parse_opt_subcommand_fn *fn = रिक्त;
	संरचना option options[] = {
		OPT__VERBOSE(&verbose, N_("be verbose; must be placed before a subcommand")),
		OPT_SUBCOMMAND("add", &fn, add),
		OPT_SUBCOMMAND("rename", &fn, mv),
		OPT_SUBCOMMAND_F("rm", &fn, rm, रिक्त, PARSE_OPT_NOCOMPLETE),
		OPT_SUBCOMMAND("remove", &fn, rm),
		OPT_SUBCOMMAND("set-head", &fn, set_head),
		OPT_SUBCOMMAND("set-branches", &fn, set_branches),
		OPT_SUBCOMMAND("get-url", &fn, get_url),
		OPT_SUBCOMMAND("set-url", &fn, set_url),
		OPT_SUBCOMMAND("show", &fn, show),
		OPT_SUBCOMMAND("prune", &fn, prune),
		OPT_SUBCOMMAND("update", &fn, update),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, builtin_remote_usage,
			     PARSE_OPT_SUBCOMMAND_OPTIONAL);

	जइ (fn) {
		प्रत्यागम !!fn(argc, argv, prefix, repo);
	} अण्णहा {
		जइ (argc) {
			error(_("unknown subcommand: `%s'"), argv[0]);
			usage_with_options(builtin_remote_usage, options);
		}
		प्रत्यागम !!show_all();
	}
}
