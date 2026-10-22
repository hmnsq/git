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

स्थिर स्थिरांक अक्षर * स्थिरांक builtin_remote_usage[] = {
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

स्थिर स्थिरांक अक्षर * स्थिरांक builtin_remote_add_usage[] = {
	N_("git remote add [<options>] <name> <url>"),
	रिक्त
};

स्थिर स्थिरांक अक्षर * स्थिरांक builtin_remote_rename_usage[] = {
	N_("git remote rename [--[no-]progress] <old> <new>"),
	रिक्त
};

स्थिर स्थिरांक अक्षर * स्थिरांक builtin_remote_rm_usage[] = {
	N_("git remote remove <name>"),
	रिक्त
};

स्थिर स्थिरांक अक्षर * स्थिरांक builtin_remote_sethead_usage[] = {
	N_("git remote set-head <name> (-a | --auto | -d | --delete | <branch>)"),
	रिक्त
};

स्थिर स्थिरांक अक्षर * स्थिरांक builtin_remote_setbranches_usage[] = {
	N_("git remote set-branches <name> <branch>..."),
	N_("git remote set-branches --add <name> <branch>..."),
	रिक्त
};

स्थिर स्थिरांक अक्षर * स्थिरांक builtin_remote_show_usage[] = {
	N_("git remote show [<options>] <name>"),
	रिक्त
};

स्थिर स्थिरांक अक्षर * स्थिरांक builtin_remote_prune_usage[] = {
	N_("git remote prune [<options>] <name>"),
	रिक्त
};

स्थिर स्थिरांक अक्षर * स्थिरांक builtin_remote_update_usage[] = {
	N_("git remote update [<options>] [<group> | <remote>]..."),
	रिक्त
};

स्थिर स्थिरांक अक्षर * स्थिरांक builtin_remote_geturl_usage[] = {
	N_("git remote get-url [--push] [--all] <name>"),
	रिक्त
};

स्थिर स्थिरांक अक्षर * स्थिरांक builtin_remote_seturl_usage[] = {
	N_("git remote set-url [--push] <name> <newurl> [<oldurl>]"),
	N_("git remote set-url --add <name> <newurl>"),
	N_("git remote set-url --delete <name> <url>"),
	रिक्त
};

#define GET_REF_STATES (1<<0)
#define GET_HEAD_NAMES (1<<1)
#define GET_ढकला_REF_STATES (1<<2)

स्थिर पूर्णांक verbose;

स्थिर पूर्णांक fetch_remote(स्थिरांक अक्षर *name)
{
	संरचना child_process cmd = CHILD_PROCESS_INIT;

	strvec_ढकला(&cmd.args, "fetch");
	जर (verbose)
		strvec_ढकला(&cmd.args, "-v");
	strvec_ढकला(&cmd.args, name);
	cmd.git_cmd = 1;
	printf_ln(_("Updating %s"), name);
	जर (run_command(&cmd))
		परत error(_("Could not fetch %s"), name);
	परत 0;
}

गणना {
	TAGS_UNSET = 0,
	TAGS_DEFAULT = 1,
	TAGS_SET = 2
};

#define MIRROR_NONE 0
#define MIRROR_FETCH 1
#define MIRROR_ढकला 2
#define MIRROR_BOTH (MIRROR_FETCH|MIRROR_ढकला)

स्थिर शून्य add_branch(स्थिरांक अक्षर *किल्ली, स्थिरांक अक्षर *branchname,
		       स्थिरांक अक्षर *remotename, पूर्णांक mirror, संरचना strbuf *tmp)
{
	strbuf_reset(tmp);
	strbuf_addch(tmp, '+');
	जर (mirror)
		strbuf_addf(tmp, "refs/%s:refs/%s",
				branchname, branchname);
	अन्यथा
		strbuf_addf(tmp, "refs/heads/%s:refs/remotes/%s/%s",
				branchname, remotename, branchname);
	repo_config_set_multivar(the_repository, किल्ली, tmp->buf, "^$", 0);
}

स्थिर स्थिरांक अक्षर mirror_advice[] =
N_("--mirror is dangerous and deprecated; please\n"
   "\t use --mirror=fetch or --mirror=push instead");

स्थिर पूर्णांक parse_mirror_opt(स्थिरांक संरचना option *opt, स्थिरांक अक्षर *arg, पूर्णांक not)
{
	अचिन्हित *mirror = opt->मूल्य;
	जर (not)
		*mirror = MIRROR_NONE;
	अन्यथा जर (!arg) {
		warning("%s", _(mirror_advice));
		*mirror = MIRROR_BOTH;
	}
	अन्यथा जर (!म_भेद(arg, "fetch"))
		*mirror = MIRROR_FETCH;
	अन्यथा जर (!म_भेद(arg, "push"))
		*mirror = MIRROR_ढकला;
	अन्यथा
		परत error(_("unknown --mirror argument: %s"), arg);
	परत 0;
}

स्थिर पूर्णांक check_remote_collision(संरचना remote *remote, शून्य *data)
{
	स्थिरांक अक्षर *name = data;
	स्थिरांक अक्षर *p;

	जर (skip_prefix(name, remote->name, &p) && *p == '/')
		die(_("remote name '%s' is a subset of existing remote '%s'"),
		    name, remote->name);
	जर (skip_prefix(remote->name, name, &p) && *p == '/')
		die(_("remote name '%s' is a superset of existing remote '%s'"),
		    name, remote->name);

	परत 0;
}

स्थिर पूर्णांक add(पूर्णांक argc, स्थिरांक अक्षर **argv, स्थिरांक अक्षर *prefix,
	       संरचना repository *repo UNUSED)
{
	पूर्णांक fetch = 0, fetch_tags = TAGS_DEFAULT;
	अचिन्हित mirror = MIRROR_NONE;
	संरचना string_list track = STRING_LIST_INIT_NODUP;
	स्थिरांक अक्षर *master = रिक्त;
	संरचना remote *remote;
	संरचना strbuf buf = STRBUF_INIT, buf2 = STRBUF_INIT;
	स्थिरांक अक्षर *name, *url;
	पूर्णांक result = 0;

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

	जर (argc != 2)
		usage_with_options(builtin_remote_add_usage, options);

	जर (mirror && master)
		die(_("specifying a master branch makes no sense with --mirror"));
	जर (mirror && !(mirror & MIRROR_FETCH) && track.nr)
		die(_("specifying branches to track makes sense only with fetch mirrors"));

	name = argv[0];
	url = argv[1];

	remote = remote_get(name);
	जर (remote_is_configured(remote, 1)) {
		error(_("remote %s already exists."), name);
		निकास(3);
	}

	जर (!valid_remote_name(name))
		die(_("'%s' is not a valid remote name"), name);

	for_each_remote(check_remote_collision, (शून्य *)name);

	strbuf_addf(&buf, "remote.%s.url", name);
	repo_config_set(the_repository, buf.buf, url);

	जर (!mirror || mirror & MIRROR_FETCH) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.fetch", name);
		जर (track.nr == 0)
			string_list_append(&track, "*");
		साठी (माप_प्रकार i = 0; i < track.nr; i++) {
			add_branch(buf.buf, track.वस्तुसंच[i].string,
				   name, mirror, &buf2);
		}
	}

	जर (mirror & MIRROR_ढकला) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.mirror", name);
		repo_config_set(the_repository, buf.buf, "true");
	}

	जर (fetch_tags != TAGS_DEFAULT) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.tagOpt", name);
		repo_config_set(the_repository, buf.buf,
				fetch_tags == TAGS_SET ? "--tags" : "--no-tags");
	}

	जर (fetch && fetch_remote(name)) {
		result = 1;
		जा out;
	}

	जर (master) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "refs/remotes/%s/HEAD", name);

		strbuf_reset(&buf2);
		strbuf_addf(&buf2, "refs/remotes/%s/%s", name, master);

		जर (refs_update_symref(get_main_ref_store(the_repository), buf.buf, buf2.buf, "remote add"))
			result = error(_("Could not setup master '%s'"), master);
	}

out:
	strbuf_release(&buf);
	strbuf_release(&buf2);
	string_list_clear(&track, 0);

	परत result;
}

संरचना branch_info {
	अक्षर *remote_name;
	संरचना string_list merge;
	गणना rebase_type rebase;
	अक्षर *ढकला_remote_name;
};

स्थिर संरचना string_list branch_list = STRING_LIST_INIT_DUP;

स्थिर स्थिरांक अक्षर *abbrev_ref(स्थिरांक अक्षर *name, स्थिरांक अक्षर *prefix)
{
	skip_prefix(name, prefix, &name);
	परत name;
}
#define abbrev_branch(name) abbrev_ref((name), "refs/heads/")

स्थिर पूर्णांक config_read_branches(स्थिरांक अक्षर *किल्ली, स्थिरांक अक्षर *मूल्य,
				स्थिरांक संरचना config_context *ctx UNUSED,
				शून्य *data UNUSED)
{
	स्थिरांक अक्षर *orig_किल्ली = किल्ली;
	अक्षर *name;
	संरचना string_list_item *item;
	संरचना branch_info *info;
	गणना { REMOTE, MERGE, REBASE, ढकला_REMOTE } type;
	माप_प्रकार किल्ली_len;

	जर (!starts_with(किल्ली, "branch."))
		परत 0;

	किल्ली += म_माप("branch.");
	जर (strip_suffix(किल्ली, ".remote", &किल्ली_len))
		type = REMOTE;
	अन्यथा जर (strip_suffix(किल्ली, ".merge", &किल्ली_len))
		type = MERGE;
	अन्यथा जर (strip_suffix(किल्ली, ".rebase", &किल्ली_len))
		type = REBASE;
	अन्यथा जर (strip_suffix(किल्ली, ".pushremote", &किल्ली_len))
		type = ढकला_REMOTE;
	अन्यथा
		परत 0;

	name = xmemdupz(किल्ली, किल्ली_len);
	item = string_list_घाला(&branch_list, name);

	जर (!item->util)
		item->util = xcalloc(1, आकार(संरचना branch_info));
	info = item->util;
	निवड (type) {
	स्थिती REMOTE:
		जर (info->remote_name)
			warning(_("more than one %s"), orig_किल्ली);
		info->remote_name = xstrdup(मूल्य);
		थांबा;
	स्थिती MERGE: {
		स्थिरांक अक्षर *space = म_अक्षर(मूल्य, ' ');
		मूल्य = abbrev_branch(मूल्य);
		पर्यंत (space) {
			अक्षर *merge;
			merge = xstrndup(मूल्य, space - मूल्य);
			string_list_append(&info->merge, merge);
			मूल्य = abbrev_branch(space + 1);
			space = म_अक्षर(मूल्य, ' ');
		}
		string_list_append(&info->merge, xstrdup(मूल्य));
		थांबा;
	}
	स्थिती REBASE:
		/*
		 * Consider invalid values as false and check the
		 * truth value with >= REBASE_TRUE.
		 */
		info->rebase = rebase_parse_मूल्य(मूल्य);
		जर (info->rebase == REBASE_INVALID)
			warning(_("unhandled branch.%s.rebase=%s; assuming "
				  "'true'"), name, मूल्य);
		थांबा;
	स्थिती ढकला_REMOTE:
		जर (info->ढकला_remote_name)
			warning(_("more than one %s"), orig_किल्ली);
		info->ढकला_remote_name = xstrdup(मूल्य);
		थांबा;
	पूर्वनिर्धारित:
		BUG("unexpected type=%d", type);
	}

	मुक्त(name);
	परत 0;
}

स्थिर शून्य read_branches(शून्य)
{
	जर (branch_list.nr)
		परत;
	repo_config(the_repository, config_read_branches, रिक्त);
}

संरचना ref_states {
	संरचना remote *remote;
	संरचना string_list new_refs, skipped, stale, tracked, heads, ढकला;
	पूर्णांक queried;
};

#define REF_STATES_INIT { \
	.new_refs = STRING_LIST_INIT_DUP, \
	.skipped = STRING_LIST_INIT_DUP, \
	.stale = STRING_LIST_INIT_DUP, \
	.tracked = STRING_LIST_INIT_DUP, \
	.heads = STRING_LIST_INIT_DUP, \
	.ढकला = STRING_LIST_INIT_DUP, \
}

स्थिर पूर्णांक get_ref_states(स्थिरांक संरचना ref *remote_refs, संरचना ref_states *states)
{
	संरचना ref *fetch_map = रिक्त, **tail = &fetch_map;
	संरचना ref *ref, *stale_refs;
	पूर्णांक i;

	साठी (i = 0; i < states->remote->fetch.nr; i++)
		जर (get_fetch_map(remote_refs, &states->remote->fetch.वस्तुसंच[i], &tail, 1))
			die(_("Could not get fetch map for refspec %s"),
				states->remote->fetch.वस्तुसंच[i].raw);

	साठी (ref = fetch_map; ref; ref = ref->next) {
		जर (refname_matches_negative_refspec_item(ref->name, &states->remote->fetch))
			string_list_append(&states->skipped, abbrev_branch(ref->name));
		अन्यथा जर (!ref->peer_ref || !refs_ref_exists(get_main_ref_store(the_repository), ref->peer_ref->name))
			string_list_append(&states->new_refs, abbrev_branch(ref->name));
		अन्यथा
			string_list_append(&states->tracked, abbrev_branch(ref->name));
	}
	stale_refs = get_stale_heads(&states->remote->fetch, fetch_map);
	साठी (ref = stale_refs; ref; ref = ref->next) {
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

	परत 0;
}

संरचना ढकला_info {
	अक्षर *dest;
	पूर्णांक forced;
	गणना {
		ढकला_STATUS_CREATE = 0,
		ढकला_STATUS_DELETE,
		ढकला_STATUS_UPTODATE,
		ढकला_STATUS_FASTFORWARD,
		ढकला_STATUS_OUTOFDATE,
		ढकला_STATUS_NOTQUERIED
	} status;
};

स्थिर पूर्णांक get_ढकला_ref_states(स्थिरांक संरचना ref *remote_refs,
	संरचना ref_states *states)
{
	संरचना remote *remote = states->remote;
	संरचना ref *ref, *local_refs, *ढकला_map;
	जर (remote->mirror)
		परत 0;

	local_refs = get_local_heads();
	ढकला_map = copy_ref_list(remote_refs);

	match_ढकला_refs(local_refs, &ढकला_map, &remote->ढकला, MATCH_REFS_NONE);

	साठी (ref = ढकला_map; ref; ref = ref->next) {
		संरचना string_list_item *item;
		संरचना ढकला_info *info;

		जर (!ref->peer_ref)
			सुरूठेवा;
		oidcpy(&ref->new_oid, &ref->peer_ref->new_oid);

		item = string_list_append(&states->ढकला,
					  abbrev_branch(ref->peer_ref->name));
		item->util = xcalloc(1, आकार(संरचना ढकला_info));
		info = item->util;
		info->forced = ref->force;
		info->dest = xstrdup(abbrev_branch(ref->name));

		जर (is_null_oid(&ref->new_oid)) {
			info->status = ढकला_STATUS_DELETE;
		} अन्यथा जर (oideq(&ref->old_oid, &ref->new_oid))
			info->status = ढकला_STATUS_UPTODATE;
		अन्यथा जर (is_null_oid(&ref->old_oid))
			info->status = ढकला_STATUS_CREATE;
		अन्यथा जर (odb_has_object(the_repository->objects, &ref->old_oid,
					ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR) &&
			 ref_newer(&ref->new_oid, &ref->old_oid))
			info->status = ढकला_STATUS_FASTFORWARD;
		अन्यथा
			info->status = ढकला_STATUS_OUTOFDATE;
	}
	free_refs(local_refs);
	free_refs(ढकला_map);
	परत 0;
}

स्थिर पूर्णांक get_ढकला_ref_states_noquery(संरचना ref_states *states)
{
	पूर्णांक i;
	संरचना remote *remote = states->remote;
	संरचना string_list_item *item;
	संरचना ढकला_info *info;

	जर (remote->mirror)
		परत 0;

	जर (!remote->ढकला.nr) {
		item = string_list_append(&states->ढकला, _("(matching)"));
		info = item->util = xcalloc(1, आकार(संरचना ढकला_info));
		info->status = ढकला_STATUS_NOTQUERIED;
		info->dest = xstrdup(item->string);
	}
	साठी (i = 0; i < remote->ढकला.nr; i++) {
		स्थिरांक संरचना refspec_item *spec = &remote->ढकला.वस्तुसंच[i];
		जर (spec->matching)
			item = string_list_append(&states->ढकला, _("(matching)"));
		अन्यथा जर (म_माप(spec->src))
			item = string_list_append(&states->ढकला, spec->src);
		अन्यथा
			item = string_list_append(&states->ढकला, _("(delete)"));

		info = item->util = xcalloc(1, आकार(संरचना ढकला_info));
		info->forced = spec->force;
		info->status = ढकला_STATUS_NOTQUERIED;
		info->dest = xstrdup(spec->dst ? spec->dst : item->string);
	}
	परत 0;
}

स्थिर पूर्णांक get_head_names(स्थिरांक संरचना ref *remote_refs, संरचना ref_states *states)
{
	संरचना ref *ref, *matches;
	संरचना ref *fetch_map = रिक्त, **fetch_map_tail = &fetch_map;
	संरचना refspec_item refspec = {
		.force = 0,
		.pattern = 1,
		.src = (अक्षर *) "refs/heads/*",
		.dst = (अक्षर *) "refs/heads/*",
	};

	get_fetch_map(remote_refs, &refspec, &fetch_map_tail, 0);
	matches = guess_remote_head(find_ref_by_name(remote_refs, "HEAD"),
				    fetch_map, REMOTE_GUESS_HEAD_ALL);
	साठी (ref = matches; ref; ref = ref->next)
		string_list_append(&states->heads, abbrev_branch(ref->name));

	free_refs(fetch_map);
	free_refs(matches);
	परत 0;
}

संरचना known_remote {
	संरचना known_remote *next;
	संरचना remote *remote;
};

संरचना known_remotes {
	संरचना remote *to_delete;
	संरचना known_remote *list;
};

स्थिर पूर्णांक add_known_remote(संरचना remote *remote, शून्य *cb_data)
{
	संरचना known_remotes *all = cb_data;
	संरचना known_remote *r;

	जर (!म_भेद(all->to_delete->name, remote->name))
		परत 0;

	r = xmalloc(आकार(*r));
	r->remote = remote;
	r->next = all->list;
	all->list = r;
	परत 0;
}

संरचना branches_for_remote {
	संरचना remote *remote;
	संरचना string_list *branches, *skipped;
	संरचना known_remotes *keep;
};

स्थिर पूर्णांक add_branch_for_removal(स्थिरांक संरचना reference *ref, शून्य *cb_data)
{
	संरचना branches_for_remote *branches = cb_data;
	संरचना refspec_item refspec;
	संरचना known_remote *kr;

	स_रखो(&refspec, 0, आकार(refspec));
	refspec.dst = (अक्षर *)ref->name;
	जर (remote_find_tracking(branches->remote, &refspec))
		परत 0;
	मुक्त(refspec.src);

	/* don't delete a branch if another remote also uses it */
	साठी (kr = branches->keep->list; kr; kr = kr->next) {
		स_रखो(&refspec, 0, आकार(refspec));
		refspec.dst = (अक्षर *)ref->name;
		जर (!remote_find_tracking(kr->remote, &refspec)) {
			मुक्त(refspec.src);
			परत 0;
		}
	}

	/* don't delete non-remote-tracking refs */
	जर (!starts_with(ref->name, "refs/remotes/")) {
		/* advise user how to delete local branches */
		जर (starts_with(ref->name, "refs/heads/"))
			string_list_append(branches->skipped,
					   abbrev_branch(ref->name));
		/* silently skip over other non-remote refs */
		परत 0;
	}

	string_list_append(branches->branches, ref->name);

	परत 0;
}

संरचना rename_info {
	स्थिरांक अक्षर *old_name;
	स्थिरांक अक्षर *new_name;
	संरचना ref_transaction *transaction;
	संरचना progress *progress;
	संरचना strbuf *err;
	uint32_t progress_nr;
	uint64_t index;
};

स्थिर शून्य compute_renamed_ref(संरचना rename_info *नाम,
				स्थिरांक अक्षर *refname,
				संरचना strbuf *out)
{
	strbuf_reset(out);
	strbuf_addstr(out, refname);
	strbuf_splice(out, म_माप("refs/remotes/"), म_माप(नाम->old_name),
		      नाम->new_name, म_माप(नाम->new_name));
}

स्थिर पूर्णांक rename_one_reflog_entry(स्थिरांक अक्षर *old_refname,
				   संरचना object_id *old_oid,
				   संरचना object_id *new_oid,
				   स्थिरांक अक्षर *committer,
				   timestamp_t timestamp, पूर्णांक tz,
				   स्थिरांक अक्षर *संदेश, शून्य *cb_data)
{
	संरचना rename_info *नाम = cb_data;
	संरचना strbuf new_refname = STRBUF_INIT;
	संरचना strbuf identity = STRBUF_INIT;
	संरचना strbuf name = STRBUF_INIT;
	संरचना strbuf mail = STRBUF_INIT;
	संरचना ident_split ident;
	स्थिरांक अक्षर *date;
	पूर्णांक error;

	compute_renamed_ref(नाम, old_refname, &new_refname);

	जर (split_ident_ओळ(&ident, committer, म_माप(committer)) < 0) {
		error = -1;
		जा out;
	}

	strbuf_add(&name, ident.name_begin, ident.name_end - ident.name_begin);
	strbuf_add(&mail, ident.mail_begin, ident.mail_end - ident.mail_begin);

	date = show_date(timestamp, tz, DATE_MODE(NORMAL));
	strbuf_addstr(&identity, fmt_ident(name.buf, mail.buf,
					  WANT_BLANK_IDENT, date, 0));

	error = ref_transaction_update_reflog(नाम->transaction, new_refname.buf,
					      new_oid, old_oid, identity.buf, संदेश,
					      नाम->index++, नाम->err);

out:
	strbuf_release(&new_refname);
	strbuf_release(&identity);
	strbuf_release(&name);
	strbuf_release(&mail);
	परत error;
}

स्थिर पूर्णांक rename_one_reflog(स्थिरांक अक्षर *old_refname,
			     स्थिरांक संरचना object_id *old_oid,
			     संरचना rename_info *नाम)
{
	संरचना strbuf new_refname = STRBUF_INIT;
	संरचना strbuf message = STRBUF_INIT;
	पूर्णांक error;

	जर (!refs_reflog_exists(get_main_ref_store(the_repository), old_refname))
		परत 0;

	error = refs_for_each_reflog_ent(get_main_ref_store(the_repository),
					 old_refname, rename_one_reflog_entry, नाम);
	जर (error < 0)
		जा out;

	compute_renamed_ref(नाम, old_refname, &new_refname);

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

	error = ref_transaction_update_reflog(नाम->transaction, new_refname.buf,
					      old_oid, old_oid, git_committer_info(0),
					      message.buf, नाम->index++, नाम->err);
	जर (error < 0)
		परत error;

out:
	strbuf_release(&new_refname);
	strbuf_release(&message);
	परत error;
}

स्थिर पूर्णांक rename_one_ref(स्थिरांक संरचना reference *ref, शून्य *cb_data)
{
	संरचना strbuf new_referent = STRBUF_INIT;
	संरचना strbuf new_refname = STRBUF_INIT;
	संरचना rename_info *नाम = cb_data;
	स्थिरांक संरचना object_id *oid = ref->oid;
	स्थिरांक अक्षर *referent = ref->target;
	पूर्णांक error;

	compute_renamed_ref(नाम, ref->name, &new_refname);

	जर (ref->flags & REF_ISSYMREF) {
		/*
		 * Stupidly enough `referent` is not pointing to the immediate
		 * target of a symref, but it's the recursively resolved value.
		 * So symrefs pointing to symrefs would be misresolved, and
		 * unborn symrefs don't have any value for the `referent` at all.
		 */
		referent = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
						   ref->name, RESOLVE_REF_NO_RECURSE,
						   रिक्त, रिक्त);
		compute_renamed_ref(नाम, referent, &new_referent);
		oid = रिक्त;
	}

	error = ref_transaction_delete(नाम->transaction, ref->name,
				       oid, referent, REF_NO_DEREF, रिक्त, नाम->err);
	जर (error < 0)
		जा out;

	error = ref_transaction_update(नाम->transaction, new_refname.buf, oid, null_oid(the_hash_algo),
				       (ref->flags & REF_ISSYMREF) ? new_referent.buf : रिक्त, रिक्त,
				       REF_SKIP_CREATE_REFLOG | REF_NO_DEREF | REF_SKIP_OID_VERIFICATION,
				       रिक्त, नाम->err);
	जर (error < 0)
		जा out;

	error = rename_one_reflog(ref->name, oid, नाम);
	जर (error < 0)
		जा out;

	display_progress(नाम->progress, ++नाम->progress_nr);

out:
	strbuf_release(&new_referent);
	strbuf_release(&new_refname);
	परत error;
}

स्थिर पूर्णांक migrate_file(संरचना remote *remote)
{
	संरचना strbuf buf = STRBUF_INIT;

	strbuf_addf(&buf, "remote.%s.url", remote->name);
	साठी (माप_प्रकार i = 0; i < remote->url.nr; i++)
		repo_config_set_multivar(the_repository, buf.buf, remote->url.v[i], "^$", 0);
	strbuf_reset(&buf);
	strbuf_addf(&buf, "remote.%s.push", remote->name);
	साठी (पूर्णांक i = 0; i < remote->ढकला.nr; i++)
		repo_config_set_multivar(the_repository, buf.buf, remote->ढकला.वस्तुसंच[i].raw, "^$", 0);
	strbuf_reset(&buf);
	strbuf_addf(&buf, "remote.%s.fetch", remote->name);
	साठी (पूर्णांक i = 0; i < remote->fetch.nr; i++)
		repo_config_set_multivar(the_repository, buf.buf, remote->fetch.वस्तुसंच[i].raw, "^$", 0);
#ifndef WITH_BREAKING_CHANGES
	जर (remote->origin == REMOTE_REMOTES)
		unlink_or_warn(repo_git_path_replace(the_repository, &buf,
						     "remotes/%s", remote->name));
	अन्यथा जर (remote->origin == REMOTE_BRANCHES)
		unlink_or_warn(repo_git_path_replace(the_repository, &buf,
						     "branches/%s", remote->name));
#endif /* WITH_BREAKING_CHANGES */
	strbuf_release(&buf);

	परत 0;
}

संरचना ढकला_default_info
{
	स्थिरांक अक्षर *old_name;
	गणना config_scope scope;
	संरचना strbuf origin;
	पूर्णांक linenr;
};

स्थिर पूर्णांक config_read_ढकला_default(स्थिरांक अक्षर *किल्ली, स्थिरांक अक्षर *मूल्य,
	स्थिरांक संरचना config_context *ctx, शून्य *cb)
{
	स्थिरांक संरचना किल्ली_मूल्य_info *kvi = ctx->kvi;

	संरचना ढकला_default_info* info = cb;
	जर (म_भेद(किल्ली, "remote.pushdefault") ||
	    !मूल्य || म_भेद(मूल्य, info->old_name))
		परत 0;

	info->scope = kvi->scope;
	strbuf_reset(&info->origin);
	strbuf_addstr(&info->origin, config_origin_type_name(kvi->origin_type));
	info->linenr = kvi->linenr;

	परत 0;
}

स्थिर शून्य handle_ढकला_default(स्थिरांक अक्षर* old_name, स्थिरांक अक्षर* new_name)
{
	संरचना ढकला_default_info ढकला_default = {
		.old_name = old_name,
		.scope = CONFIG_SCOPE_UNKNOWN,
		.origin = STRBUF_INIT,
		.linenr = -1,
	};
	repo_config(the_repository, config_read_ढकला_default, &ढकला_default);
	जर (ढकला_default.scope >= CONFIG_SCOPE_COMMAND)
		; /* pass */
	अन्यथा जर (ढकला_default.scope >= CONFIG_SCOPE_LOCAL) {
		पूर्णांक result = repo_config_set_gently(the_repository, "remote.pushDefault",
						    new_name);
		जर (new_name && result && result != CONFIG_NOTHING_SET)
			die(_("could not set '%s'"), "remote.pushDefault");
		अन्यथा जर (!new_name && result && result != CONFIG_NOTHING_SET)
			die(_("could not unset '%s'"), "remote.pushDefault");
	} अन्यथा जर (ढकला_default.scope >= CONFIG_SCOPE_SYSTEM) {
		/* warn */
		warning(_("The %s configuration remote.pushDefault in:\n"
			  "\t%s:%d\n"
			  "now names the non-existent remote '%s'"),
			config_scope_name(ढकला_default.scope),
			ढकला_default.origin.buf, ढकला_default.linenr,
			old_name);
	}

	strbuf_release(&ढकला_default.origin);
}

स्थिर स्थिरांक अक्षर conflicting_remote_refs_advice[] = N_(
	"The remote you are trying to rename has conflicting references in the\n"
	"new target refspec. This is most likely caused by you trying to nest\n"
	"a remote into itself, e.g. by renaming 'parent' into 'parent/child'\n"
	"or by unnesting a remote, e.g. the other way round.\n"
	"\n"
	"If that is the case, you can address this by first renaming the\n"
	"remote to a different name.\n");

स्थिर पूर्णांक mv(पूर्णांक argc, स्थिरांक अक्षर **argv, स्थिरांक अक्षर *prefix,
	      संरचना repository *repo UNUSED)
{
	पूर्णांक show_progress = isatty(2);
	संरचना option options[] = {
		OPT_BOOL(0, "progress", &show_progress, N_("force progress reporting")),
		OPT_END()
	};
	संरचना remote *oldremote, *newremote;
	संरचना strbuf buf = STRBUF_INIT, buf2 = STRBUF_INIT, buf3 = STRBUF_INIT,
		old_remote_context = STRBUF_INIT, err = STRBUF_INIT;
	संरचना rename_info नाम = {
		.err = &err,
	};
	पूर्णांक refspecs_need_update = 0;
	पूर्णांक result = 0;

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_rename_usage, 0);

	जर (argc != 2)
		usage_with_options(builtin_remote_rename_usage, options);

	नाम.old_name = argv[0];
	नाम.new_name = argv[1];

	oldremote = remote_get(नाम.old_name);
	जर (!remote_is_configured(oldremote, 1)) {
		error(_("No such remote: '%s'"), नाम.old_name);
		निकास(2);
	}

	जर (!म_भेद(नाम.old_name, नाम.new_name) && oldremote->origin != REMOTE_CONFIG)
		परत migrate_file(oldremote);

	newremote = remote_get(नाम.new_name);
	जर (remote_is_configured(newremote, 1)) {
		error(_("remote %s already exists."), नाम.new_name);
		निकास(3);
	}

	जर (!valid_remote_name(नाम.new_name))
		die(_("'%s' is not a valid remote name"), नाम.new_name);

	strbuf_addf(&buf, "remote.%s", नाम.old_name);
	strbuf_addf(&buf2, "remote.%s", नाम.new_name);
	जर (repo_config_rename_section(the_repository, buf.buf, buf2.buf) < 1) {
		result = error(_("Could not rename config section '%s' to '%s'"),
			       buf.buf, buf2.buf);
		जा out;
	}

	strbuf_addf(&old_remote_context, ":refs/remotes/%s/", नाम.old_name);

	साठी (पूर्णांक i = 0; i < oldremote->fetch.nr && !refspecs_need_update; i++)
		refspecs_need_update = !!म_माला(oldremote->fetch.वस्तुसंच[i].raw,
						old_remote_context.buf);

	जर (refspecs_need_update) {
		संरचना refs_for_each_ref_options opts = {
			.flags = REFS_FOR_EACH_INCLUDE_BROKEN,
		};
		नाम.transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
							       0, &err);
		जर (!नाम.transaction)
			जा out;

		जर (show_progress)
			नाम.progress = start_delayed_progress(the_repository,
								 _("Renaming remote references"), 0);

		strbuf_reset(&buf);
		strbuf_addf(&buf, "refs/remotes/%s/", नाम.old_name);
		opts.prefix = buf.buf;

		result = refs_for_each_ref_ext(get_main_ref_store(the_repository),
					       rename_one_ref, &नाम, &opts);
		जर (result < 0)
			die(_("queueing remote ref renames failed: %s"), नाम.err->buf);

		result = ref_transaction_prepare(नाम.transaction, &err);
		जर (result < 0) {
			error("renaming remote references failed: %s", err.buf);
			जर (result == REF_TRANSACTION_ERROR_NAME_CONFLICT)
				advise(conflicting_remote_refs_advice);
			die(रिक्त);
		}
	}

	जर (oldremote->fetch.nr) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.fetch", नाम.new_name);
		repo_config_set_multivar(the_repository, buf.buf, रिक्त, रिक्त, CONFIG_FLAGS_MULTI_REPLACE);
		साठी (पूर्णांक i = 0; i < oldremote->fetch.nr; i++) {
			अक्षर *ptr;

			strbuf_reset(&buf2);
			strbuf_addstr(&buf2, oldremote->fetch.वस्तुसंच[i].raw);
			ptr = म_माला(buf2.buf, old_remote_context.buf);
			जर (ptr) {
				strbuf_splice(&buf2,
					      ptr-buf2.buf + म_माप(":refs/remotes/"),
					      म_माप(नाम.old_name), नाम.new_name,
					      म_माप(नाम.new_name));
			} अन्यथा
				warning(_("Not updating non-default fetch refspec\n"
					  "\t%s\n"
					  "\tPlease update the configuration manually if necessary."),
					buf2.buf);

			repo_config_set_multivar(the_repository, buf.buf, buf2.buf, "^$", 0);
		}
	}

	read_branches();
	साठी (माप_प्रकार i = 0; i < branch_list.nr; i++) {
		संरचना string_list_item *item = branch_list.वस्तुसंच + i;
		संरचना branch_info *info = item->util;
		जर (info->remote_name && !म_भेद(info->remote_name, नाम.old_name)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "branch.%s.remote", item->string);
			repo_config_set(the_repository, buf.buf, नाम.new_name);
		}
		जर (info->ढकला_remote_name && !म_भेद(info->ढकला_remote_name, नाम.old_name)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "branch.%s.pushRemote", item->string);
			repo_config_set(the_repository, buf.buf, नाम.new_name);
		}
	}

	जर (refspecs_need_update) {
		result = ref_transaction_commit(नाम.transaction, &err);
		जर (result < 0)
			die(_("renaming remote refs failed: %s"), नाम.err->buf);

		stop_progress(&नाम.progress);

		handle_ढकला_default(नाम.old_name, नाम.new_name);
	}

out:
	ref_transaction_free(नाम.transaction);
	strbuf_release(&old_remote_context);
	strbuf_release(&buf);
	strbuf_release(&buf2);
	strbuf_release(&buf3);
	strbuf_release(&err);
	परत result;
}

स्थिर पूर्णांक rm(पूर्णांक argc, स्थिरांक अक्षर **argv, स्थिरांक अक्षर *prefix,
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
	पूर्णांक result;

	स_रखो(&cb_data, 0, आकार(cb_data));
	cb_data.branches = &branches;
	cb_data.skipped = &skipped;
	cb_data.keep = &known_remotes;

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_rm_usage, 0);
	जर (argc != 1)
		usage_with_options(builtin_remote_rm_usage, options);

	remote = remote_get(argv[0]);
	जर (!remote_is_configured(remote, 1)) {
		error(_("No such remote: '%s'"), argv[0]);
		निकास(2);
	}

	known_remotes.to_delete = remote;
	for_each_remote(add_known_remote, &known_remotes);

	read_branches();
	साठी (माप_प्रकार i = 0; i < branch_list.nr; i++) {
		संरचना string_list_item *item = branch_list.वस्तुसंच + i;
		संरचना branch_info *info = item->util;
		जर (info->remote_name && !म_भेद(info->remote_name, remote->name)) {
			स्थिरांक अक्षर *किल्ल्या[] = { "remote", "merge", रिक्त }, **k;
			साठी (k = किल्ल्या; *k; k++) {
				strbuf_reset(&buf);
				strbuf_addf(&buf, "branch.%s.%s",
						item->string, *k);
				result = repo_config_set_gently(the_repository, buf.buf, रिक्त);
				जर (result && result != CONFIG_NOTHING_SET)
					die(_("could not unset '%s'"), buf.buf);
			}
		}
		जर (info->ढकला_remote_name && !म_भेद(info->ढकला_remote_name, remote->name)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "branch.%s.pushremote", item->string);
			result = repo_config_set_gently(the_repository, buf.buf, रिक्त);
			जर (result && result != CONFIG_NOTHING_SET)
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

	जर (!result)
		result = refs_delete_refs(get_main_ref_store(the_repository),
					  "remote: remove", &branches,
					  REF_NO_DEREF);
	string_list_clear(&branches, 0);

	जर (skipped.nr) {
		fprintf_ln(मानक_त्रुटि,
			   Q_("Note: A branch outside the refs/remotes/ hierarchy was not removed;\n"
			      "to delete it, use:",
			      "Note: Some branches outside the refs/remotes/ hierarchy were not removed;\n"
			      "to delete them, use:",
			      skipped.nr));
		साठी (माप_प्रकार i = 0; i < skipped.nr; i++)
			ख_लिखो(मानक_त्रुटि, "  git branch -d %s\n",
				skipped.वस्तुसंच[i].string);
	}
	string_list_clear(&skipped, 0);

	जर (!result) {
		strbuf_addf(&buf, "remote.%s", remote->name);
		जर (repo_config_rename_section(the_repository, buf.buf, रिक्त) < 1) {
			result = error(_("Could not remove config section '%s'"), buf.buf);
			जा out;
		}

		handle_ढकला_default(remote->name, रिक्त);
	}

out:
	साठी (संरचना known_remote *r = known_remotes.list; r;) {
		संरचना known_remote *next = r->next;
		मुक्त(r);
		r = next;
	}
	strbuf_release(&buf);
	परत result;
}

स्थिर शून्य clear_ढकला_info(शून्य *util, स्थिरांक अक्षर *string UNUSED)
{
	संरचना ढकला_info *info = util;
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
	string_list_clear_func(&states->ढकला, clear_ढकला_info);
}

स्थिर पूर्णांक append_ref_to_tracked_list(स्थिरांक संरचना reference *ref, शून्य *cb_data)
{
	संरचना ref_states *states = cb_data;
	संरचना refspec_item refspec;

	जर (ref->flags & REF_ISSYMREF)
		परत 0;

	स_रखो(&refspec, 0, आकार(refspec));
	refspec.dst = (अक्षर *)ref->name;
	जर (!remote_find_tracking(states->remote, &refspec)) {
		string_list_append(&states->tracked, abbrev_branch(refspec.src));
		मुक्त(refspec.src);
	}

	परत 0;
}

स्थिर पूर्णांक get_remote_ref_states(स्थिरांक अक्षर *name,
				 संरचना ref_states *states,
				 पूर्णांक query)
{
	states->remote = remote_get(name);
	जर (!states->remote)
		परत error(_("No such remote: '%s'"), name);

	read_branches();

	जर (query) {
		संरचना transport *transport;
		स्थिरांक संरचना ref *remote_refs;

		transport = transport_get(states->remote, states->remote->url.v[0]);
		remote_refs = transport_get_remote_refs(transport, रिक्त);

		states->queried = 1;
		जर (query & GET_REF_STATES)
			get_ref_states(remote_refs, states);
		जर (query & GET_HEAD_NAMES)
			get_head_names(remote_refs, states);
		जर (query & GET_ढकला_REF_STATES)
			get_ढकला_ref_states(remote_refs, states);
		transport_disconnect(transport);
	} अन्यथा {
		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_ref_to_tracked_list, states);
		string_list_sort(&states->tracked);
		get_ढकला_ref_states_noquery(states);
	}

	परत 0;
}

संरचना show_info {
	संरचना string_list list;
	संरचना ref_states states;
	पूर्णांक width, width2;
	पूर्णांक any_rebase;
};

#define SHOW_INFO_INIT { \
	.list = STRING_LIST_INIT_DUP, \
	.states = REF_STATES_INIT, \
}

स्थिर पूर्णांक add_remote_to_show_info(संरचना string_list_item *item, शून्य *cb_data)
{
	संरचना show_info *info = cb_data;
	पूर्णांक n = म_माप(item->string);
	जर (n > info->width)
		info->width = n;
	string_list_घाला(&info->list, item->string);
	परत 0;
}

स्थिर पूर्णांक show_remote_info_item(संरचना string_list_item *item, शून्य *cb_data)
{
	संरचना show_info *info = cb_data;
	संरचना ref_states *states = &info->states;
	स्थिरांक अक्षर *name = item->string;

	जर (states->queried) {
		स्थिरांक अक्षर *fmt = "%s";
		स्थिरांक अक्षर *arg = "";
		जर (string_list_has_string(&states->new_refs, name)) {
			fmt = _(" new (next fetch will store in remotes/%s)");
			arg = states->remote->name;
		} अन्यथा जर (string_list_has_string(&states->tracked, name))
			arg = _(" tracked");
		अन्यथा जर (string_list_has_string(&states->skipped, name))
			arg = _(" skipped");
		अन्यथा जर (string_list_has_string(&states->stale, name))
			arg = _(" stale (use 'git remote prune' to remove)");
		अन्यथा
			arg = _(" ???");
		म_लिखो("    %-*s", info->width, name);
		म_लिखो(fmt, arg);
		म_लिखो("\n");
	} अन्यथा
		म_लिखो("    %s\n", name);

	परत 0;
}

स्थिर पूर्णांक add_local_to_show_info(संरचना string_list_item *branch_item, शून्य *cb_data)
{
	संरचना show_info *show_info = cb_data;
	संरचना ref_states *states = &show_info->states;
	संरचना branch_info *branch_info = branch_item->util;
	संरचना string_list_item *item;
	पूर्णांक n;

	जर (!branch_info->merge.nr || !branch_info->remote_name ||
	    म_भेद(states->remote->name, branch_info->remote_name))
		परत 0;
	जर ((n = म_माप(branch_item->string)) > show_info->width)
		show_info->width = n;
	जर (branch_info->rebase >= REBASE_TRUE)
		show_info->any_rebase = 1;

	item = string_list_घाला(&show_info->list, branch_item->string);
	item->util = branch_info;

	परत 0;
}

स्थिर पूर्णांक show_local_info_item(संरचना string_list_item *item, शून्य *cb_data)
{
	संरचना show_info *show_info = cb_data;
	संरचना branch_info *branch_info = item->util;
	संरचना string_list *merge = &branch_info->merge;
	पूर्णांक width = show_info->width + 4;

	जर (branch_info->rebase >= REBASE_TRUE && branch_info->merge.nr > 1) {
		error(_("invalid branch.%s.merge; cannot rebase onto > 1 branch"),
			item->string);
		परत 0;
	}

	म_लिखो("    %-*s ", show_info->width, item->string);
	जर (branch_info->rebase >= REBASE_TRUE) {
		स्थिरांक अक्षर *संदेश;
		जर (branch_info->rebase == REBASE_INTERACTIVE)
			संदेश = _("rebases interactively onto remote %s");
		अन्यथा जर (branch_info->rebase == REBASE_MERGES)
			संदेश = _("rebases interactively (with merges) onto "
				"remote %s");
		अन्यथा
			संदेश = _("rebases onto remote %s");
		printf_ln(संदेश, merge->वस्तुसंच[0].string);
		परत 0;
	} अन्यथा जर (show_info->any_rebase) {
		printf_ln(_(" merges with remote %s"), merge->वस्तुसंच[0].string);
		width++;
	} अन्यथा {
		printf_ln(_("merges with remote %s"), merge->वस्तुसंच[0].string);
	}
	साठी (माप_प्रकार i = 1; i < merge->nr; i++)
		म_लिखो(_("%-*s    and with remote %s\n"), width, "",
		       merge->वस्तुसंच[i].string);

	परत 0;
}

स्थिर पूर्णांक add_ढकला_to_show_info(संरचना string_list_item *ढकला_item, शून्य *cb_data)
{
	संरचना show_info *show_info = cb_data;
	संरचना ढकला_info *ढकला_info = ढकला_item->util;
	संरचना string_list_item *item;
	पूर्णांक n;
	जर ((n = म_माप(ढकला_item->string)) > show_info->width)
		show_info->width = n;
	जर ((n = म_माप(ढकला_info->dest)) > show_info->width2)
		show_info->width2 = n;
	item = string_list_append(&show_info->list, ढकला_item->string);
	item->util = ढकला_item->util;
	परत 0;
}

/*
 * Sorting comparison for a string list that has push_info
 * structs in its util field
 */
स्थिर पूर्णांक cmp_string_with_ढकला(स्थिरांक शून्य *va, स्थिरांक शून्य *vb)
{
	स्थिरांक संरचना string_list_item *a = va;
	स्थिरांक संरचना string_list_item *b = vb;
	स्थिरांक संरचना ढकला_info *a_ढकला = a->util;
	स्थिरांक संरचना ढकला_info *b_ढकला = b->util;
	पूर्णांक cmp = म_भेद(a->string, b->string);
	परत cmp ? cmp : म_भेद(a_ढकला->dest, b_ढकला->dest);
}

स्थिर पूर्णांक show_ढकला_info_item(संरचना string_list_item *item, शून्य *cb_data)
{
	संरचना show_info *show_info = cb_data;
	संरचना ढकला_info *ढकला_info = item->util;
	स्थिरांक अक्षर *src = item->string, *status = रिक्त;

	निवड (ढकला_info->status) {
	स्थिती ढकला_STATUS_CREATE:
		status = _("create");
		थांबा;
	स्थिती ढकला_STATUS_DELETE:
		status = _("delete");
		src = _("(none)");
		थांबा;
	स्थिती ढकला_STATUS_UPTODATE:
		status = _("up to date");
		थांबा;
	स्थिती ढकला_STATUS_FASTFORWARD:
		status = _("fast-forwardable");
		थांबा;
	स्थिती ढकला_STATUS_OUTOFDATE:
		status = _("local out of date");
		थांबा;
	स्थिती ढकला_STATUS_NOTQUERIED:
		थांबा;
	}
	जर (status) {
		जर (ढकला_info->forced)
			printf_ln(_("    %-*s forces to %-*s (%s)"), show_info->width, src,
			       show_info->width2, ढकला_info->dest, status);
		अन्यथा
			printf_ln(_("    %-*s pushes to %-*s (%s)"), show_info->width, src,
			       show_info->width2, ढकला_info->dest, status);
	} अन्यथा {
		जर (ढकला_info->forced)
			printf_ln(_("    %-*s forces to %s"), show_info->width, src,
			       ढकला_info->dest);
		अन्यथा
			printf_ln(_("    %-*s pushes to %s"), show_info->width, src,
			       ढकला_info->dest);
	}
	परत 0;
}

स्थिर पूर्णांक get_one_entry(संरचना remote *remote, शून्य *priv)
{
	संरचना string_list *list = priv;
	संरचना strbuf remote_info_buf = STRBUF_INIT;
	संरचना strvec *url;

	जर (remote->url.nr > 0) {
		संरचना strbuf promisor_config = STRBUF_INIT;
		स्थिरांक अक्षर *partial_clone_filter = रिक्त;

		strbuf_addf(&promisor_config, "remote.%s.partialclonefilter", remote->name);
		strbuf_addf(&remote_info_buf, "%s (fetch)", remote->url.v[0]);
		जर (!repo_config_get_string_tmp(the_repository, promisor_config.buf, &partial_clone_filter))
			strbuf_addf(&remote_info_buf, " [%s]", partial_clone_filter);

		strbuf_release(&promisor_config);
		string_list_append(list, remote->name)->util =
				strbuf_detach(&remote_info_buf, रिक्त);
	} अन्यथा
		string_list_append(list, remote->name)->util = रिक्त;
	url = ढकला_url_of_remote(remote);
	साठी (माप_प्रकार i = 0; i < url->nr; i++) {
		strbuf_addf(&remote_info_buf, "%s (push)", url->v[i]);
		string_list_append(list, remote->name)->util =
				strbuf_detach(&remote_info_buf, रिक्त);
	}

	परत 0;
}

स्थिर पूर्णांक show_all(शून्य)
{
	संरचना string_list list = STRING_LIST_INIT_DUP;
	पूर्णांक result;

	result = for_each_remote(get_one_entry, &list);

	जर (!result) {
		string_list_sort(&list);
		साठी (माप_प्रकार i = 0; i < list.nr; i++) {
			संरचना string_list_item *item = list.वस्तुसंच + i;
			जर (verbose)
				म_लिखो("%s\t%s\n", item->string,
					item->util ? (स्थिरांक अक्षर *)item->util : "");
			अन्यथा {
				जर (i && !म_भेद((item - 1)->string, item->string))
					सुरूठेवा;
				म_लिखो("%s\n", item->string);
			}
		}
	}
	string_list_clear(&list, 1);
	परत result;
}

स्थिर पूर्णांक show(पूर्णांक argc, स्थिरांक अक्षर **argv, स्थिरांक अक्षर *prefix,
		संरचना repository *repo UNUSED)
{
	पूर्णांक no_query = 0, result = 0, query_flag = 0;
	संरचना option options[] = {
		OPT_BOOL('n', रिक्त, &no_query, N_("do not query remotes")),
		OPT_END()
	};
	संरचना show_info info = SHOW_INFO_INIT;

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_show_usage,
			     0);

	जर (argc < 1)
		परत show_all();

	जर (!no_query)
		query_flag = (GET_REF_STATES | GET_HEAD_NAMES | GET_ढकला_REF_STATES);

	साठी (; argc; argc--, argv++) {
		माप_प्रकार i;
		संरचना strvec *url;

		get_remote_ref_states(*argv, &info.states, query_flag);

		printf_ln(_("* remote %s"), *argv);
		printf_ln(_("  Fetch URL: %s"), info.states.remote->url.v[0]);
		url = ढकला_url_of_remote(info.states.remote);
		साठी (i = 0; i < url->nr; i++)
			/*
			 * TRANSLATORS: the colon ':' should align
			 * with the one in " Fetch URL: %s"
			 * translation.
			 */
			printf_ln(_("  Push  URL: %s"), url->v[i]);
		जर (!i)
			printf_ln(_("  Push  URL: %s"), _("(no URL)"));
		जर (no_query)
			printf_ln(_("  HEAD branch: %s"), _("(not queried)"));
		अन्यथा जर (!info.states.heads.nr)
			printf_ln(_("  HEAD branch: %s"), _("(unknown)"));
		अन्यथा जर (info.states.heads.nr == 1)
			printf_ln(_("  HEAD branch: %s"), info.states.heads.वस्तुसंच[0].string);
		अन्यथा {
			म_लिखो(_("  HEAD branch (remote HEAD is ambiguous,"
				 " may be one of the following):\n"));
			साठी (i = 0; i < info.states.heads.nr; i++)
				म_लिखो("    %s\n", info.states.heads.वस्तुसंच[i].string);
		}

		/* remote branch info */
		info.width = 0;
		for_each_string_list(&info.states.new_refs, add_remote_to_show_info, &info);
		for_each_string_list(&info.states.skipped, add_remote_to_show_info, &info);
		for_each_string_list(&info.states.tracked, add_remote_to_show_info, &info);
		for_each_string_list(&info.states.stale, add_remote_to_show_info, &info);
		जर (info.list.nr)
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
		जर (info.list.nr)
			printf_ln(Q_("  Local branch configured for 'git pull':",
				     "  Local branches configured for 'git pull':",
				     info.list.nr));
		for_each_string_list(&info.list, show_local_info_item, &info);
		string_list_clear(&info.list, 0);

		/* git push info */
		जर (info.states.remote->mirror)
			printf_ln(_("  Local refs will be mirrored by 'git push'"));

		info.width = info.width2 = 0;
		for_each_string_list(&info.states.ढकला, add_ढकला_to_show_info, &info);
		QSORT(info.list.वस्तुसंच, info.list.nr, cmp_string_with_ढकला);
		जर (info.list.nr)
			printf_ln(Q_("  Local ref configured for 'git push'%s:",
				     "  Local refs configured for 'git push'%s:",
				     info.list.nr),
				  no_query ? _(" (status not queried)") : "");
		for_each_string_list(&info.list, show_ढकला_info_item, &info);
		string_list_clear(&info.list, 0);

		free_remote_ref_states(&info.states);
	}

	परत result;
}

स्थिर शून्य report_set_head_auto(स्थिरांक अक्षर *remote, स्थिरांक अक्षर *head_name,
			संरचना strbuf *b_local_head, पूर्णांक was_detached) {
	संरचना strbuf buf_prefix = STRBUF_INIT;
	स्थिरांक अक्षर *prev_head = रिक्त;

	strbuf_addf(&buf_prefix, "refs/remotes/%s/", remote);
	skip_prefix(b_local_head->buf, buf_prefix.buf, &prev_head);

	जर (prev_head && !म_भेद(prev_head, head_name))
		म_लिखो(_("'%s/HEAD' is unchanged and points to '%s'\n"),
			remote, head_name);
	अन्यथा जर (prev_head)
		म_लिखो(_("'%s/HEAD' has changed from '%s' and now points to '%s'\n"),
			remote, prev_head, head_name);
	अन्यथा जर (!b_local_head->len)
		म_लिखो(_("'%s/HEAD' is now created and points to '%s'\n"),
			remote, head_name);
	अन्यथा जर (was_detached && b_local_head->len)
		म_लिखो(_("'%s/HEAD' was detached at '%s' and now points to '%s'\n"),
			remote, b_local_head->buf, head_name);
	अन्यथा
		म_लिखो(_("'%s/HEAD' used to point to '%s' "
			"(which is not a remote branch), but now points to '%s'\n"),
			remote, b_local_head->buf, head_name);
	strbuf_release(&buf_prefix);
}

स्थिर पूर्णांक set_head(पूर्णांक argc, स्थिरांक अक्षर **argv, स्थिरांक अक्षर *prefix,
		    संरचना repository *repo UNUSED)
{
	पूर्णांक opt_a = 0, opt_d = 0, result = 0, was_detached;
	संरचना strbuf b_head = STRBUF_INIT, b_remote_head = STRBUF_INIT,
		b_local_head = STRBUF_INIT;
	अक्षर *head_name = रिक्त;
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
	जर (!argc)
		usage_with_options(builtin_remote_sethead_usage, options);

	strbuf_addf(&b_head, "refs/remotes/%s/HEAD", argv[0]);
	remote = remote_get(argv[0]);

	जर (!opt_a && !opt_d && argc == 2) {
		head_name = xstrdup(argv[1]);
	} अन्यथा जर (opt_a && !opt_d && argc == 1) {
		संरचना ref_states states = REF_STATES_INIT;
		get_remote_ref_states(argv[0], &states, GET_HEAD_NAMES);
		जर (!states.heads.nr)
			result |= error(_("Cannot determine remote HEAD"));
		अन्यथा जर (states.heads.nr > 1) {
			result |= error(_("Multiple remote HEAD branches. "
					  "Please choose one explicitly with:"));
			साठी (माप_प्रकार i = 0; i < states.heads.nr; i++)
				ख_लिखो(मानक_त्रुटि, "  git remote set-head %s %s\n",
					argv[0], states.heads.वस्तुसंच[i].string);
		} अन्यथा
			head_name = xstrdup(states.heads.वस्तुसंच[0].string);
		free_remote_ref_states(&states);
	} अन्यथा जर (opt_d && !opt_a && argc == 1) {
		जर (refs_delete_ref(refs, रिक्त, b_head.buf, रिक्त, REF_NO_DEREF))
			result |= error(_("Could not delete %s"), b_head.buf);
	} अन्यथा
		usage_with_options(builtin_remote_sethead_usage, options);

	जर (!head_name)
		जा cleanup;
	strbuf_addf(&b_remote_head, "refs/remotes/%s/%s", argv[0], head_name);
	जर (!refs_ref_exists(refs, b_remote_head.buf)) {
		result |= error(_("Not a valid ref: %s"), b_remote_head.buf);
		जा cleanup;
	}
	was_detached = refs_update_symref_extended(refs, b_head.buf, b_remote_head.buf,
			"remote set-head", &b_local_head, 0);
	जर (was_detached == -1) {
		result |= error(_("Could not set up %s"), b_head.buf);
		जा cleanup;
	}
	जर (opt_a)
		report_set_head_auto(argv[0], head_name, &b_local_head, was_detached);
	जर (remote->follow_remote_head == FOLLOW_REMOTE_ALWAYS) {
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
	परत result;
}

स्थिर पूर्णांक prune_remote(स्थिरांक अक्षर *remote, पूर्णांक dry_run)
{
	पूर्णांक result = 0;
	संरचना ref_states states = REF_STATES_INIT;
	संरचना string_list refs_to_prune = STRING_LIST_INIT_NODUP;
	संरचना string_list_item *item;

	get_remote_ref_states(remote, &states, GET_REF_STATES);

	जर (!states.stale.nr) {
		free_remote_ref_states(&states);
		परत 0;
	}

	printf_ln(_("Pruning %s"), remote);
	printf_ln(_("URL: %s"), states.remote->url.v[0]);

	for_each_string_list_item(item, &states.stale)
		string_list_append(&refs_to_prune, item->util);
	string_list_sort(&refs_to_prune);

	जर (!dry_run)
		result |= refs_delete_refs(get_main_ref_store(the_repository),
					   "remote: prune", &refs_to_prune, 0);

	for_each_string_list_item(item, &states.stale) {
		स्थिरांक अक्षर *refname = item->util;

		जर (dry_run)
			printf_ln(_(" * [would prune] %s"),
			       abbrev_ref(refname, "refs/remotes/"));
		अन्यथा
			printf_ln(_(" * [pruned] %s"),
			       abbrev_ref(refname, "refs/remotes/"));
	}

	refs_warn_dangling_symrefs(get_main_ref_store(the_repository),
				   मानक_निकास, " ", dry_run, &refs_to_prune);

	string_list_clear(&refs_to_prune, 0);
	free_remote_ref_states(&states);
	परत result;
}

स्थिर पूर्णांक prune(पूर्णांक argc, स्थिरांक अक्षर **argv, स्थिरांक अक्षर *prefix,
		 संरचना repository *repo UNUSED)
{
	पूर्णांक dry_run = 0, result = 0;
	संरचना option options[] = {
		OPT__DRY_RUN(&dry_run, N_("dry run")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_prune_usage, 0);

	जर (argc < 1)
		usage_with_options(builtin_remote_prune_usage, options);

	साठी (; argc; argc--, argv++)
		result |= prune_remote(*argv, dry_run);

	परत result;
}

स्थिर पूर्णांक get_remote_default(स्थिरांक अक्षर *किल्ली, स्थिरांक अक्षर *मूल्य UNUSED,
			      स्थिरांक संरचना config_context *ctx UNUSED,
			      शून्य *priv)
{
	जर (म_भेद(किल्ली, "remotes.default") == 0) {
		पूर्णांक *found = priv;
		*found = 1;
	}
	परत 0;
}

स्थिर पूर्णांक update(पूर्णांक argc, स्थिरांक अक्षर **argv, स्थिरांक अक्षर *prefix,
		  संरचना repository *repo UNUSED)
{
	पूर्णांक i, prune = -1;
	संरचना option options[] = {
		OPT_BOOL('p', "prune", &prune,
			 N_("prune remotes after fetching")),
		OPT_END()
	};
	संरचना child_process cmd = CHILD_PROCESS_INIT;
	पूर्णांक default_defined = 0;

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_update_usage,
			     PARSE_OPT_KEEP_ARGV0);

	strvec_ढकला(&cmd.args, "fetch");

	जर (prune != -1)
		strvec_ढकला(&cmd.args, prune ? "--prune" : "--no-prune");
	जर (verbose)
		strvec_ढकला(&cmd.args, "-v");
	strvec_ढकला(&cmd.args, "--multiple");
	जर (argc < 2)
		strvec_ढकला(&cmd.args, "default");
	साठी (i = 1; i < argc; i++)
		strvec_ढकला(&cmd.args, argv[i]);

	जर (म_भेद(cmd.args.v[cmd.args.nr-1], "default") == 0) {
		repo_config(the_repository, get_remote_default, &default_defined);
		जर (!default_defined) {
			strvec_काढा(&cmd.args);
			strvec_ढकला(&cmd.args, "--all");
		}
	}

	cmd.git_cmd = 1;
	परत run_command(&cmd);
}

स्थिर पूर्णांक remove_all_fetch_refspecs(स्थिरांक अक्षर *किल्ली)
{
	परत repo_config_set_multivar_gently(the_repository, किल्ली, रिक्त, रिक्त,
					       CONFIG_FLAGS_MULTI_REPLACE);
}

स्थिर शून्य add_branches(संरचना remote *remote, स्थिरांक अक्षर **branches,
			 स्थिरांक अक्षर *किल्ली)
{
	स्थिरांक अक्षर *remotename = remote->name;
	पूर्णांक mirror = remote->mirror;
	संरचना strbuf refspec = STRBUF_INIT;

	साठी (; *branches; branches++)
		add_branch(किल्ली, *branches, remotename, mirror, &refspec);

	strbuf_release(&refspec);
}

स्थिर पूर्णांक set_remote_branches(स्थिरांक अक्षर *remotename, स्थिरांक अक्षर **branches,
				पूर्णांक add_mode)
{
	संरचना strbuf किल्ली = STRBUF_INIT;
	संरचना remote *remote;

	strbuf_addf(&किल्ली, "remote.%s.fetch", remotename);

	remote = remote_get(remotename);
	जर (!remote_is_configured(remote, 1)) {
		error(_("No such remote '%s'"), remotename);
		निकास(2);
	}

	जर (!add_mode && remove_all_fetch_refspecs(किल्ली.buf)) {
		strbuf_release(&किल्ली);
		परत 1;
	}
	add_branches(remote, branches, किल्ली.buf);

	strbuf_release(&किल्ली);
	परत 0;
}

स्थिर पूर्णांक set_branches(पूर्णांक argc, स्थिरांक अक्षर **argv, स्थिरांक अक्षर *prefix,
			संरचना repository *repo UNUSED)
{
	पूर्णांक add_mode = 0;
	संरचना option options[] = {
		OPT_BOOL('\0', "add", &add_mode, N_("add branch")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_setbranches_usage, 0);
	जर (argc == 0) {
		error(_("no remote specified"));
		usage_with_options(builtin_remote_setbranches_usage, options);
	}
	argv[argc] = रिक्त;

	परत set_remote_branches(argv[0], argv + 1, add_mode);
}

स्थिर पूर्णांक get_url(पूर्णांक argc, स्थिरांक अक्षर **argv, स्थिरांक अक्षर *prefix,
		   संरचना repository *repo UNUSED)
{
	पूर्णांक ढकला_mode = 0, all_mode = 0;
	स्थिरांक अक्षर *remotename = रिक्त;
	संरचना remote *remote;
	संरचना strvec *url;
	संरचना option options[] = {
		OPT_BOOL('\0', "push", &ढकला_mode,
			 N_("query push URLs rather than fetch URLs")),
		OPT_BOOL('\0', "all", &all_mode,
			 N_("return all URLs")),
		OPT_END()
	};
	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_geturl_usage, 0);

	जर (argc != 1)
		usage_with_options(builtin_remote_geturl_usage, options);

	remotename = argv[0];

	remote = remote_get(remotename);
	जर (!remote_is_configured(remote, 1)) {
		error(_("No such remote '%s'"), remotename);
		निकास(2);
	}

	url = ढकला_mode ? ढकला_url_of_remote(remote) : &remote->url;

	जर (all_mode) {
		साठी (माप_प्रकार i = 0; i < url->nr; i++)
			printf_ln("%s", url->v[i]);
	} अन्यथा {
		printf_ln("%s", url->v[0]);
	}

	परत 0;
}

स्थिर पूर्णांक set_url(पूर्णांक argc, स्थिरांक अक्षर **argv, स्थिरांक अक्षर *prefix,
		   संरचना repository *repo UNUSED)
{
	पूर्णांक ढकला_mode = 0, add_mode = 0, delete_mode = 0;
	पूर्णांक matches = 0, negative_matches = 0;
	स्थिरांक अक्षर *remotename = रिक्त;
	स्थिरांक अक्षर *newurl = रिक्त;
	स्थिरांक अक्षर *oldurl = रिक्त;
	संरचना remote *remote;
	regex_t old_regex;
	संरचना strvec *urlset;
	संरचना strbuf name_buf = STRBUF_INIT;
	संरचना option options[] = {
		OPT_BOOL('\0', "push", &ढकला_mode,
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

	जर (add_mode && delete_mode)
		die(_("--add --delete doesn't make sense"));

	जर (argc < 3 || argc > 4 || ((add_mode || delete_mode) && argc != 3))
		usage_with_options(builtin_remote_seturl_usage, options);

	remotename = argv[1];
	newurl = argv[2];
	जर (argc > 3)
		oldurl = argv[3];

	जर (delete_mode)
		oldurl = newurl;

	remote = remote_get(remotename);
	जर (!remote_is_configured(remote, 1)) {
		error(_("No such remote '%s'"), remotename);
		निकास(2);
	}

	जर (ढकला_mode) {
		strbuf_addf(&name_buf, "remote.%s.pushurl", remotename);
		urlset = &remote->pushurl;
	} अन्यथा {
		strbuf_addf(&name_buf, "remote.%s.url", remotename);
		urlset = &remote->url;
	}

	/* Special cases that add new entry. */
	जर ((!oldurl && !delete_mode) || add_mode) {
		जर (add_mode)
			repo_config_set_multivar(the_repository, name_buf.buf, newurl,
						       "^$", 0);
		अन्यथा
			repo_config_set(the_repository, name_buf.buf, newurl);
		जा out;
	}

	/* Old URL specified. Demand that one matches. */
	जर (regcomp(&old_regex, oldurl, REG_EXTENDED))
		die(_("Invalid old URL pattern: %s"), oldurl);

	साठी (माप_प्रकार i = 0; i < urlset->nr; i++)
		जर (!regexec(&old_regex, urlset->v[i], 0, रिक्त, 0))
			matches++;
		अन्यथा
			negative_matches++;
	जर (!delete_mode && !matches)
		die(_("No such URL found: %s"), oldurl);
	जर (delete_mode && !negative_matches && !ढकला_mode)
		die(_("Will not delete all non-push URLs"));

	regfree(&old_regex);

	जर (!delete_mode)
		repo_config_set_multivar(the_repository, name_buf.buf, newurl, oldurl, 0);
	अन्यथा
		repo_config_set_multivar(the_repository, name_buf.buf, रिक्त, oldurl,
					 CONFIG_FLAGS_MULTI_REPLACE);
out:
	strbuf_release(&name_buf);
	परत 0;
}

पूर्णांक cmd_remote(पूर्णांक argc,
	       स्थिरांक अक्षर **argv,
	       स्थिरांक अक्षर *prefix,
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

	जर (fn) {
		परत !!fn(argc, argv, prefix, repo);
	} अन्यथा {
		जर (argc) {
			error(_("unknown subcommand: `%s'"), argv[0]);
			usage_with_options(builtin_remote_usage, options);
		}
		परत !!show_all();
	}
}
