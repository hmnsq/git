#घोषणा USE_THE_REPOSITORY_VARIABLE

#समावेश "builtin.h"
#समावेश "advice.h"
#समावेश "config.h"
#समावेश "date.h"
#समावेश "gettext.h"
#समावेश "ident.h"
#समावेश "parse-options.h"
#समावेश "path.h"
#समावेश "transport.h"
#समावेश "remote.h"
#समावेश "string-list.h"
#समावेश "strbuf.h"
#समावेश "run-command.h"
#समावेश "rebase.h"
#समावेश "refs.h"
#समावेश "refspec.h"
#समावेश "odb.h"
#समावेश "strvec.h"
#समावेश "commit-reach.h"
#समावेश "progress.h"

अटल स्थिर अक्षर * स्थिर builtin_remote_usage[] = {
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
	शून्य
};

अटल स्थिर अक्षर * स्थिर builtin_remote_add_usage[] = {
	N_("git remote add [<options>] <name> <url>"),
	शून्य
};

अटल स्थिर अक्षर * स्थिर builtin_remote_rename_usage[] = {
	N_("git remote rename [--[no-]progress] <old> <new>"),
	शून्य
};

अटल स्थिर अक्षर * स्थिर builtin_remote_rm_usage[] = {
	N_("git remote remove <name>"),
	शून्य
};

अटल स्थिर अक्षर * स्थिर builtin_remote_sethead_usage[] = {
	N_("git remote set-head <name> (-a | --auto | -d | --delete | <branch>)"),
	शून्य
};

अटल स्थिर अक्षर * स्थिर builtin_remote_setbranches_usage[] = {
	N_("git remote set-branches <name> <branch>..."),
	N_("git remote set-branches --add <name> <branch>..."),
	शून्य
};

अटल स्थिर अक्षर * स्थिर builtin_remote_show_usage[] = {
	N_("git remote show [<options>] <name>"),
	शून्य
};

अटल स्थिर अक्षर * स्थिर builtin_remote_prune_usage[] = {
	N_("git remote prune [<options>] <name>"),
	शून्य
};

अटल स्थिर अक्षर * स्थिर builtin_remote_update_usage[] = {
	N_("git remote update [<options>] [<group> | <remote>]..."),
	शून्य
};

अटल स्थिर अक्षर * स्थिर builtin_remote_geturl_usage[] = {
	N_("git remote get-url [--push] [--all] <name>"),
	शून्य
};

अटल स्थिर अक्षर * स्थिर builtin_remote_seturl_usage[] = {
	N_("git remote set-url [--push] <name> <newurl> [<oldurl>]"),
	N_("git remote set-url --add <name> <newurl>"),
	N_("git remote set-url --delete <name> <url>"),
	शून्य
};

#घोषणा GET_REF_STATES (1<<0)
#घोषणा GET_शीर्ष_NAMES (1<<1)
#घोषणा GET_धकेलो_REF_STATES (1<<2)

अटल पूर्णांक verbose;

अटल पूर्णांक fetch_remote(स्थिर अक्षर *name)
{
	काष्ठा child_process cmd = CHILD_PROCESS_INIT;

	strvec_धकेलो(&cmd.args, "fetch");
	अगर (verbose)
		strvec_धकेलो(&cmd.args, "-v");
	strvec_धकेलो(&cmd.args, name);
	cmd.git_cmd = 1;
	printf_ln(_("Updating %s"), name);
	अगर (run_command(&cmd))
		वापस error(_("Could not fetch %s"), name);
	वापस 0;
}

क्रमागत {
	TAGS_UNSET = 0,
	TAGS_DEFAULT = 1,
	TAGS_SET = 2
};

#घोषणा MIRROR_NONE 0
#घोषणा MIRROR_FETCH 1
#घोषणा MIRROR_धकेलो 2
#घोषणा MIRROR_BOTH (MIRROR_FETCH|MIRROR_धकेलो)

अटल व्योम add_branch(स्थिर अक्षर *कुंजी, स्थिर अक्षर *branchname,
		       स्थिर अक्षर *remotename, पूर्णांक mirror, काष्ठा strbuf *tmp)
{
	strbuf_reset(tmp);
	strbuf_addch(tmp, '+');
	अगर (mirror)
		strbuf_addf(tmp, "refs/%s:refs/%s",
				branchname, branchname);
	अन्यथा
		strbuf_addf(tmp, "refs/heads/%s:refs/remotes/%s/%s",
				branchname, remotename, branchname);
	repo_config_set_multivar(the_repository, कुंजी, tmp->buf, "^$", 0);
}

अटल स्थिर अक्षर mirror_advice[] =
N_("--mirror is dangerous and deprecated; please\n"
   "\t use --mirror=fetch or --mirror=push instead");

अटल पूर्णांक parse_mirror_opt(स्थिर काष्ठा option *opt, स्थिर अक्षर *arg, पूर्णांक not)
{
	अचिन्हित *mirror = opt->मूल्य;
	अगर (not)
		*mirror = MIRROR_NONE;
	अन्यथा अगर (!arg) {
		warning("%s", _(mirror_advice));
		*mirror = MIRROR_BOTH;
	}
	अन्यथा अगर (!म_भेद(arg, "fetch"))
		*mirror = MIRROR_FETCH;
	अन्यथा अगर (!म_भेद(arg, "push"))
		*mirror = MIRROR_धकेलो;
	अन्यथा
		वापस error(_("unknown --mirror argument: %s"), arg);
	वापस 0;
}

अटल पूर्णांक check_remote_collision(काष्ठा remote *remote, व्योम *तथ्य)
{
	स्थिर अक्षर *name = तथ्य;
	स्थिर अक्षर *p;

	अगर (skip_prefix(name, remote->name, &p) && *p == '/')
		die(_("remote name '%s' is a subset of existing remote '%s'"),
		    name, remote->name);
	अगर (skip_prefix(remote->name, name, &p) && *p == '/')
		die(_("remote name '%s' is a superset of existing remote '%s'"),
		    name, remote->name);

	वापस 0;
}

अटल पूर्णांक add(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
	       काष्ठा repository *repo UNUSED)
{
	पूर्णांक fetch = 0, fetch_tags = TAGS_DEFAULT;
	अचिन्हित mirror = MIRROR_NONE;
	काष्ठा string_list track = STRING_LIST_INIT_NODUP;
	स्थिर अक्षर *master = शून्य;
	काष्ठा remote *remote;
	काष्ठा strbuf buf = STRBUF_INIT, buf2 = STRBUF_INIT;
	स्थिर अक्षर *name, *url;
	पूर्णांक परिणाम = 0;

	काष्ठा option options[] = {
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

	अगर (argc != 2)
		usage_with_options(builtin_remote_add_usage, options);

	अगर (mirror && master)
		die(_("specifying a master branch makes no sense with --mirror"));
	अगर (mirror && !(mirror & MIRROR_FETCH) && track.nr)
		die(_("specifying branches to track makes sense only with fetch mirrors"));

	name = argv[0];
	url = argv[1];

	remote = remote_get(name);
	अगर (remote_is_configured(remote, 1)) {
		error(_("remote %s already exists."), name);
		निकास(3);
	}

	अगर (!valid_remote_name(name))
		die(_("'%s' is not a valid remote name"), name);

	for_each_remote(check_remote_collision, (व्योम *)name);

	strbuf_addf(&buf, "remote.%s.url", name);
	repo_config_set(the_repository, buf.buf, url);

	अगर (!mirror || mirror & MIRROR_FETCH) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.fetch", name);
		अगर (track.nr == 0)
			string_list_append(&track, "*");
		क्रम (माप_प्रकार i = 0; i < track.nr; i++) {
			add_branch(buf.buf, track.वस्तुएँ[i].string,
				   name, mirror, &buf2);
		}
	}

	अगर (mirror & MIRROR_धकेलो) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.mirror", name);
		repo_config_set(the_repository, buf.buf, "true");
	}

	अगर (fetch_tags != TAGS_DEFAULT) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.tagOpt", name);
		repo_config_set(the_repository, buf.buf,
				fetch_tags == TAGS_SET ? "--tags" : "--no-tags");
	}

	अगर (fetch && fetch_remote(name)) {
		परिणाम = 1;
		जाओ out;
	}

	अगर (master) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "refs/remotes/%s/HEAD", name);

		strbuf_reset(&buf2);
		strbuf_addf(&buf2, "refs/remotes/%s/%s", name, master);

		अगर (refs_update_symref(get_main_ref_store(the_repository), buf.buf, buf2.buf, "remote add"))
			परिणाम = error(_("Could not setup master '%s'"), master);
	}

out:
	strbuf_release(&buf);
	strbuf_release(&buf2);
	string_list_clear(&track, 0);

	वापस परिणाम;
}

काष्ठा branch_info {
	अक्षर *remote_name;
	काष्ठा string_list merge;
	क्रमागत rebase_type rebase;
	अक्षर *धकेलो_remote_name;
};

अटल काष्ठा string_list branch_list = STRING_LIST_INIT_DUP;

अटल स्थिर अक्षर *abbrev_ref(स्थिर अक्षर *name, स्थिर अक्षर *prefix)
{
	skip_prefix(name, prefix, &name);
	वापस name;
}
#घोषणा abbrev_branch(name) abbrev_ref((name), "refs/heads/")

अटल पूर्णांक config_read_branches(स्थिर अक्षर *कुंजी, स्थिर अक्षर *मूल्य,
				स्थिर काष्ठा config_context *ctx UNUSED,
				व्योम *तथ्य UNUSED)
{
	स्थिर अक्षर *orig_कुंजी = कुंजी;
	अक्षर *name;
	काष्ठा string_list_item *item;
	काष्ठा branch_info *info;
	क्रमागत { REMOTE, MERGE, REBASE, धकेलो_REMOTE } type;
	माप_प्रकार कुंजी_len;

	अगर (!starts_with(कुंजी, "branch."))
		वापस 0;

	कुंजी += म_माप("branch.");
	अगर (strip_suffix(कुंजी, ".remote", &कुंजी_len))
		type = REMOTE;
	अन्यथा अगर (strip_suffix(कुंजी, ".merge", &कुंजी_len))
		type = MERGE;
	अन्यथा अगर (strip_suffix(कुंजी, ".rebase", &कुंजी_len))
		type = REBASE;
	अन्यथा अगर (strip_suffix(कुंजी, ".pushremote", &कुंजी_len))
		type = धकेलो_REMOTE;
	अन्यथा
		वापस 0;

	name = xmemdupz(कुंजी, कुंजी_len);
	item = string_list_जोड़ो(&branch_list, name);

	अगर (!item->util)
		item->util = xcalloc(1, माप(काष्ठा branch_info));
	info = item->util;
	चयन (type) {
	हाल REMOTE:
		अगर (info->remote_name)
			warning(_("more than one %s"), orig_कुंजी);
		info->remote_name = xstrdup(मूल्य);
		अवरोध;
	हाल MERGE: {
		स्थिर अक्षर *space = म_अक्षर(मूल्य, ' ');
		मूल्य = abbrev_branch(मूल्य);
		जबतक (space) {
			अक्षर *merge;
			merge = xstrndup(मूल्य, space - मूल्य);
			string_list_append(&info->merge, merge);
			मूल्य = abbrev_branch(space + 1);
			space = म_अक्षर(मूल्य, ' ');
		}
		string_list_append(&info->merge, xstrdup(मूल्य));
		अवरोध;
	}
	हाल REBASE:
		/*
		 * Consider invalid values as false and check the
		 * truth value with >= REBASE_TRUE.
		 */
		info->rebase = rebase_parse_मूल्य(मूल्य);
		अगर (info->rebase == REBASE_INVALID)
			warning(_("unhandled branch.%s.rebase=%s; assuming "
				  "'true'"), name, मूल्य);
		अवरोध;
	हाल धकेलो_REMOTE:
		अगर (info->धकेलो_remote_name)
			warning(_("more than one %s"), orig_कुंजी);
		info->धकेलो_remote_name = xstrdup(मूल्य);
		अवरोध;
	शेष:
		BUG("unexpected type=%d", type);
	}

	मुक्त(name);
	वापस 0;
}

अटल व्योम read_branches(व्योम)
{
	अगर (branch_list.nr)
		वापस;
	repo_config(the_repository, config_read_branches, शून्य);
}

काष्ठा ref_states {
	काष्ठा remote *remote;
	काष्ठा string_list new_refs, skipped, stale, tracked, heads, धकेलो;
	पूर्णांक queried;
};

#घोषणा REF_STATES_INIT { \
	.new_refs = STRING_LIST_INIT_DUP, \
	.skipped = STRING_LIST_INIT_DUP, \
	.stale = STRING_LIST_INIT_DUP, \
	.tracked = STRING_LIST_INIT_DUP, \
	.heads = STRING_LIST_INIT_DUP, \
	.धकेलो = STRING_LIST_INIT_DUP, \
}

अटल पूर्णांक get_ref_states(स्थिर काष्ठा ref *remote_refs, काष्ठा ref_states *states)
{
	काष्ठा ref *fetch_map = शून्य, **पुच्छ = &fetch_map;
	काष्ठा ref *ref, *stale_refs;
	पूर्णांक i;

	क्रम (i = 0; i < states->remote->fetch.nr; i++)
		अगर (get_fetch_map(remote_refs, &states->remote->fetch.वस्तुएँ[i], &पुच्छ, 1))
			die(_("Could not get fetch map for refspec %s"),
				states->remote->fetch.वस्तुएँ[i].raw);

	क्रम (ref = fetch_map; ref; ref = ref->अग्र) {
		अगर (refname_matches_negative_refspec_item(ref->name, &states->remote->fetch))
			string_list_append(&states->skipped, abbrev_branch(ref->name));
		अन्यथा अगर (!ref->peer_ref || !refs_ref_exists(get_main_ref_store(the_repository), ref->peer_ref->name))
			string_list_append(&states->new_refs, abbrev_branch(ref->name));
		अन्यथा
			string_list_append(&states->tracked, abbrev_branch(ref->name));
	}
	stale_refs = get_stale_heads(&states->remote->fetch, fetch_map);
	क्रम (ref = stale_refs; ref; ref = ref->अग्र) {
		काष्ठा string_list_item *item =
			string_list_append(&states->stale, abbrev_branch(ref->name));
		item->util = xstrdup(ref->name);
	}
	free_refs(stale_refs);
	free_refs(fetch_map);

	string_list_sort(&states->new_refs);
	string_list_sort(&states->skipped);
	string_list_sort(&states->tracked);
	string_list_sort(&states->stale);

	वापस 0;
}

काष्ठा धकेलो_info {
	अक्षर *dest;
	पूर्णांक forced;
	क्रमागत {
		धकेलो_STATUS_CREATE = 0,
		धकेलो_STATUS_DELETE,
		धकेलो_STATUS_UPTODATE,
		धकेलो_STATUS_FASTFORWARD,
		धकेलो_STATUS_OUTOFDATE,
		धकेलो_STATUS_NOTQUERIED
	} status;
};

अटल पूर्णांक get_धकेलो_ref_states(स्थिर काष्ठा ref *remote_refs,
	काष्ठा ref_states *states)
{
	काष्ठा remote *remote = states->remote;
	काष्ठा ref *ref, *local_refs, *धकेलो_map;
	अगर (remote->mirror)
		वापस 0;

	local_refs = get_local_heads();
	धकेलो_map = copy_ref_list(remote_refs);

	match_धकेलो_refs(local_refs, &धकेलो_map, &remote->धकेलो, MATCH_REFS_NONE);

	क्रम (ref = धकेलो_map; ref; ref = ref->अग्र) {
		काष्ठा string_list_item *item;
		काष्ठा धकेलो_info *info;

		अगर (!ref->peer_ref)
			जारी;
		oidcpy(&ref->new_oid, &ref->peer_ref->new_oid);

		item = string_list_append(&states->धकेलो,
					  abbrev_branch(ref->peer_ref->name));
		item->util = xcalloc(1, माप(काष्ठा धकेलो_info));
		info = item->util;
		info->forced = ref->force;
		info->dest = xstrdup(abbrev_branch(ref->name));

		अगर (is_null_oid(&ref->new_oid)) {
			info->status = धकेलो_STATUS_DELETE;
		} अन्यथा अगर (oideq(&ref->old_oid, &ref->new_oid))
			info->status = धकेलो_STATUS_UPTODATE;
		अन्यथा अगर (is_null_oid(&ref->old_oid))
			info->status = धकेलो_STATUS_CREATE;
		अन्यथा अगर (odb_has_object(the_repository->objects, &ref->old_oid,
					ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR) &&
			 ref_newer(&ref->new_oid, &ref->old_oid))
			info->status = धकेलो_STATUS_FASTFORWARD;
		अन्यथा
			info->status = धकेलो_STATUS_OUTOFDATE;
	}
	free_refs(local_refs);
	free_refs(धकेलो_map);
	वापस 0;
}

अटल पूर्णांक get_धकेलो_ref_states_noquery(काष्ठा ref_states *states)
{
	पूर्णांक i;
	काष्ठा remote *remote = states->remote;
	काष्ठा string_list_item *item;
	काष्ठा धकेलो_info *info;

	अगर (remote->mirror)
		वापस 0;

	अगर (!remote->धकेलो.nr) {
		item = string_list_append(&states->धकेलो, _("(matching)"));
		info = item->util = xcalloc(1, माप(काष्ठा धकेलो_info));
		info->status = धकेलो_STATUS_NOTQUERIED;
		info->dest = xstrdup(item->string);
	}
	क्रम (i = 0; i < remote->धकेलो.nr; i++) {
		स्थिर काष्ठा refspec_item *spec = &remote->धकेलो.वस्तुएँ[i];
		अगर (spec->matching)
			item = string_list_append(&states->धकेलो, _("(matching)"));
		अन्यथा अगर (म_माप(spec->src))
			item = string_list_append(&states->धकेलो, spec->src);
		अन्यथा
			item = string_list_append(&states->धकेलो, _("(delete)"));

		info = item->util = xcalloc(1, माप(काष्ठा धकेलो_info));
		info->forced = spec->force;
		info->status = धकेलो_STATUS_NOTQUERIED;
		info->dest = xstrdup(spec->dst ? spec->dst : item->string);
	}
	वापस 0;
}

अटल पूर्णांक get_शीर्ष_names(स्थिर काष्ठा ref *remote_refs, काष्ठा ref_states *states)
{
	काष्ठा ref *ref, *matches;
	काष्ठा ref *fetch_map = शून्य, **fetch_map_पुच्छ = &fetch_map;
	काष्ठा refspec_item refspec = {
		.force = 0,
		.pattern = 1,
		.src = (अक्षर *) "refs/heads/*",
		.dst = (अक्षर *) "refs/heads/*",
	};

	get_fetch_map(remote_refs, &refspec, &fetch_map_पुच्छ, 0);
	matches = guess_remote_शीर्ष(find_ref_by_name(remote_refs, "HEAD"),
				    fetch_map, REMOTE_GUESS_शीर्ष_ALL);
	क्रम (ref = matches; ref; ref = ref->अग्र)
		string_list_append(&states->heads, abbrev_branch(ref->name));

	free_refs(fetch_map);
	free_refs(matches);
	वापस 0;
}

काष्ठा known_remote {
	काष्ठा known_remote *अग्र;
	काष्ठा remote *remote;
};

काष्ठा known_remotes {
	काष्ठा remote *to_delete;
	काष्ठा known_remote *list;
};

अटल पूर्णांक add_known_remote(काष्ठा remote *remote, व्योम *cb_तथ्य)
{
	काष्ठा known_remotes *all = cb_तथ्य;
	काष्ठा known_remote *r;

	अगर (!म_भेद(all->to_delete->name, remote->name))
		वापस 0;

	r = xmalloc(माप(*r));
	r->remote = remote;
	r->अग्र = all->list;
	all->list = r;
	वापस 0;
}

काष्ठा branches_for_remote {
	काष्ठा remote *remote;
	काष्ठा string_list *branches, *skipped;
	काष्ठा known_remotes *keep;
};

अटल पूर्णांक add_branch_for_removal(स्थिर काष्ठा reference *ref, व्योम *cb_तथ्य)
{
	काष्ठा branches_for_remote *branches = cb_तथ्य;
	काष्ठा refspec_item refspec;
	काष्ठा known_remote *kr;

	स_रखो(&refspec, 0, माप(refspec));
	refspec.dst = (अक्षर *)ref->name;
	अगर (remote_find_tracking(branches->remote, &refspec))
		वापस 0;
	मुक्त(refspec.src);

	/* don't delete a branch if another remote also uses it */
	क्रम (kr = branches->keep->list; kr; kr = kr->अग्र) {
		स_रखो(&refspec, 0, माप(refspec));
		refspec.dst = (अक्षर *)ref->name;
		अगर (!remote_find_tracking(kr->remote, &refspec)) {
			मुक्त(refspec.src);
			वापस 0;
		}
	}

	/* don't delete non-remote-tracking refs */
	अगर (!starts_with(ref->name, "refs/remotes/")) {
		/* advise user how to delete local branches */
		अगर (starts_with(ref->name, "refs/heads/"))
			string_list_append(branches->skipped,
					   abbrev_branch(ref->name));
		/* silently skip over other non-remote refs */
		वापस 0;
	}

	string_list_append(branches->branches, ref->name);

	वापस 0;
}

काष्ठा rename_info {
	स्थिर अक्षर *old_name;
	स्थिर अक्षर *new_name;
	काष्ठा ref_transaction *transaction;
	काष्ठा progress *progress;
	काष्ठा strbuf *err;
	uint32_t progress_nr;
	uint64_t सूचकांक;
};

अटल व्योम compute_renamed_ref(काष्ठा rename_info *नाम,
				स्थिर अक्षर *refname,
				काष्ठा strbuf *out)
{
	strbuf_reset(out);
	strbuf_addstr(out, refname);
	strbuf_splice(out, म_माप("refs/remotes/"), म_माप(नाम->old_name),
		      नाम->new_name, म_माप(नाम->new_name));
}

अटल पूर्णांक rename_one_reflog_entry(स्थिर अक्षर *old_refname,
				   काष्ठा object_id *old_oid,
				   काष्ठा object_id *new_oid,
				   स्थिर अक्षर *committer,
				   timestamp_t timestamp, पूर्णांक tz,
				   स्थिर अक्षर *संदेश, व्योम *cb_तथ्य)
{
	काष्ठा rename_info *नाम = cb_तथ्य;
	काष्ठा strbuf new_refname = STRBUF_INIT;
	काष्ठा strbuf identity = STRBUF_INIT;
	काष्ठा strbuf name = STRBUF_INIT;
	काष्ठा strbuf mail = STRBUF_INIT;
	काष्ठा ident_split ident;
	स्थिर अक्षर *date;
	पूर्णांक error;

	compute_renamed_ref(नाम, old_refname, &new_refname);

	अगर (split_ident_पंक्ति(&ident, committer, म_माप(committer)) < 0) {
		error = -1;
		जाओ out;
	}

	strbuf_add(&name, ident.name_begin, ident.name_end - ident.name_begin);
	strbuf_add(&mail, ident.mail_begin, ident.mail_end - ident.mail_begin);

	date = show_date(timestamp, tz, DATE_MODE(NORMAL));
	strbuf_addstr(&identity, fmt_ident(name.buf, mail.buf,
					  WANT_BLANK_IDENT, date, 0));

	error = ref_transaction_update_reflog(नाम->transaction, new_refname.buf,
					      new_oid, old_oid, identity.buf, संदेश,
					      नाम->सूचकांक++, नाम->err);

out:
	strbuf_release(&new_refname);
	strbuf_release(&identity);
	strbuf_release(&name);
	strbuf_release(&mail);
	वापस error;
}

अटल पूर्णांक rename_one_reflog(स्थिर अक्षर *old_refname,
			     स्थिर काष्ठा object_id *old_oid,
			     काष्ठा rename_info *नाम)
{
	काष्ठा strbuf new_refname = STRBUF_INIT;
	काष्ठा strbuf message = STRBUF_INIT;
	पूर्णांक error;

	अगर (!refs_reflog_exists(get_main_ref_store(the_repository), old_refname))
		वापस 0;

	error = refs_for_each_reflog_ent(get_main_ref_store(the_repository),
					 old_refname, rename_one_reflog_entry, नाम);
	अगर (error < 0)
		जाओ out;

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
					      message.buf, नाम->सूचकांक++, नाम->err);
	अगर (error < 0)
		वापस error;

out:
	strbuf_release(&new_refname);
	strbuf_release(&message);
	वापस error;
}

अटल पूर्णांक rename_one_ref(स्थिर काष्ठा reference *ref, व्योम *cb_तथ्य)
{
	काष्ठा strbuf new_referent = STRBUF_INIT;
	काष्ठा strbuf new_refname = STRBUF_INIT;
	काष्ठा rename_info *नाम = cb_तथ्य;
	स्थिर काष्ठा object_id *oid = ref->oid;
	स्थिर अक्षर *referent = ref->target;
	पूर्णांक error;

	compute_renamed_ref(नाम, ref->name, &new_refname);

	अगर (ref->flags & REF_ISSYMREF) {
		/*
		 * Stupidly enough `referent` is not pointing to the immediate
		 * target of a symref, but it's the recursively resolved value.
		 * So symrefs pointing to symrefs would be misresolved, and
		 * unborn symrefs don't have any value for the `referent` at all.
		 */
		referent = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
						   ref->name, RESOLVE_REF_NO_RECURSE,
						   शून्य, शून्य);
		compute_renamed_ref(नाम, referent, &new_referent);
		oid = शून्य;
	}

	error = ref_transaction_delete(नाम->transaction, ref->name,
				       oid, referent, REF_NO_DEREF, शून्य, नाम->err);
	अगर (error < 0)
		जाओ out;

	error = ref_transaction_update(नाम->transaction, new_refname.buf, oid, null_oid(the_hash_algo),
				       (ref->flags & REF_ISSYMREF) ? new_referent.buf : शून्य, शून्य,
				       REF_SKIP_CREATE_REFLOG | REF_NO_DEREF | REF_SKIP_OID_VERIFICATION,
				       शून्य, नाम->err);
	अगर (error < 0)
		जाओ out;

	error = rename_one_reflog(ref->name, oid, नाम);
	अगर (error < 0)
		जाओ out;

	display_progress(नाम->progress, ++नाम->progress_nr);

out:
	strbuf_release(&new_referent);
	strbuf_release(&new_refname);
	वापस error;
}

अटल पूर्णांक migrate_file(काष्ठा remote *remote)
{
	काष्ठा strbuf buf = STRBUF_INIT;

	strbuf_addf(&buf, "remote.%s.url", remote->name);
	क्रम (माप_प्रकार i = 0; i < remote->url.nr; i++)
		repo_config_set_multivar(the_repository, buf.buf, remote->url.v[i], "^$", 0);
	strbuf_reset(&buf);
	strbuf_addf(&buf, "remote.%s.push", remote->name);
	क्रम (पूर्णांक i = 0; i < remote->धकेलो.nr; i++)
		repo_config_set_multivar(the_repository, buf.buf, remote->धकेलो.वस्तुएँ[i].raw, "^$", 0);
	strbuf_reset(&buf);
	strbuf_addf(&buf, "remote.%s.fetch", remote->name);
	क्रम (पूर्णांक i = 0; i < remote->fetch.nr; i++)
		repo_config_set_multivar(the_repository, buf.buf, remote->fetch.वस्तुएँ[i].raw, "^$", 0);
#अगर_अघोषित WITH_BREAKING_CHANGES
	अगर (remote->origin == REMOTE_REMOTES)
		unlink_or_warn(repo_git_path_replace(the_repository, &buf,
						     "remotes/%s", remote->name));
	अन्यथा अगर (remote->origin == REMOTE_BRANCHES)
		unlink_or_warn(repo_git_path_replace(the_repository, &buf,
						     "branches/%s", remote->name));
#पूर्ण_अगर /* WITH_BREAKING_CHANGES */
	strbuf_release(&buf);

	वापस 0;
}

काष्ठा धकेलो_default_info
{
	स्थिर अक्षर *old_name;
	क्रमागत config_scope scope;
	काष्ठा strbuf origin;
	पूर्णांक linenr;
};

अटल पूर्णांक config_read_धकेलो_default(स्थिर अक्षर *कुंजी, स्थिर अक्षर *मूल्य,
	स्थिर काष्ठा config_context *ctx, व्योम *cb)
{
	स्थिर काष्ठा कुंजी_मूल्य_info *kvi = ctx->kvi;

	काष्ठा धकेलो_default_info* info = cb;
	अगर (म_भेद(कुंजी, "remote.pushdefault") ||
	    !मूल्य || म_भेद(मूल्य, info->old_name))
		वापस 0;

	info->scope = kvi->scope;
	strbuf_reset(&info->origin);
	strbuf_addstr(&info->origin, config_origin_type_name(kvi->origin_type));
	info->linenr = kvi->linenr;

	वापस 0;
}

अटल व्योम handle_धकेलो_default(स्थिर अक्षर* old_name, स्थिर अक्षर* new_name)
{
	काष्ठा धकेलो_default_info धकेलो_default = {
		.old_name = old_name,
		.scope = CONFIG_SCOPE_UNKNOWN,
		.origin = STRBUF_INIT,
		.linenr = -1,
	};
	repo_config(the_repository, config_read_धकेलो_default, &धकेलो_default);
	अगर (धकेलो_default.scope >= CONFIG_SCOPE_COMMAND)
		; /* pass */
	अन्यथा अगर (धकेलो_default.scope >= CONFIG_SCOPE_LOCAL) {
		पूर्णांक परिणाम = repo_config_set_gently(the_repository, "remote.pushDefault",
						    new_name);
		अगर (new_name && परिणाम && परिणाम != CONFIG_NOTHING_SET)
			die(_("could not set '%s'"), "remote.pushDefault");
		अन्यथा अगर (!new_name && परिणाम && परिणाम != CONFIG_NOTHING_SET)
			die(_("could not unset '%s'"), "remote.pushDefault");
	} अन्यथा अगर (धकेलो_default.scope >= CONFIG_SCOPE_SYSTEM) {
		/* warn */
		warning(_("The %s configuration remote.pushDefault in:\n"
			  "\t%s:%d\n"
			  "now names the non-existent remote '%s'"),
			config_scope_name(धकेलो_default.scope),
			धकेलो_default.origin.buf, धकेलो_default.linenr,
			old_name);
	}

	strbuf_release(&धकेलो_default.origin);
}

अटल स्थिर अक्षर conflicting_remote_refs_advice[] = N_(
	"The remote you are trying to rename has conflicting references in the\n"
	"new target refspec. This is most likely caused by you trying to nest\n"
	"a remote into itself, e.g. by renaming 'parent' into 'parent/child'\n"
	"or by unnesting a remote, e.g. the other way round.\n"
	"\n"
	"If that is the case, you can address this by first renaming the\n"
	"remote to a different name.\n");

अटल पूर्णांक mv(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
	      काष्ठा repository *repo UNUSED)
{
	पूर्णांक show_progress = isatty(2);
	काष्ठा option options[] = {
		OPT_BOOL(0, "progress", &show_progress, N_("force progress reporting")),
		OPT_END()
	};
	काष्ठा remote *oldremote, *newremote;
	काष्ठा strbuf buf = STRBUF_INIT, buf2 = STRBUF_INIT, buf3 = STRBUF_INIT,
		old_remote_context = STRBUF_INIT, err = STRBUF_INIT;
	काष्ठा rename_info नाम = {
		.err = &err,
	};
	पूर्णांक refspecs_need_update = 0;
	पूर्णांक परिणाम = 0;

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_rename_usage, 0);

	अगर (argc != 2)
		usage_with_options(builtin_remote_rename_usage, options);

	नाम.old_name = argv[0];
	नाम.new_name = argv[1];

	oldremote = remote_get(नाम.old_name);
	अगर (!remote_is_configured(oldremote, 1)) {
		error(_("No such remote: '%s'"), नाम.old_name);
		निकास(2);
	}

	अगर (!म_भेद(नाम.old_name, नाम.new_name) && oldremote->origin != REMOTE_CONFIG)
		वापस migrate_file(oldremote);

	newremote = remote_get(नाम.new_name);
	अगर (remote_is_configured(newremote, 1)) {
		error(_("remote %s already exists."), नाम.new_name);
		निकास(3);
	}

	अगर (!valid_remote_name(नाम.new_name))
		die(_("'%s' is not a valid remote name"), नाम.new_name);

	strbuf_addf(&buf, "remote.%s", नाम.old_name);
	strbuf_addf(&buf2, "remote.%s", नाम.new_name);
	अगर (repo_config_rename_section(the_repository, buf.buf, buf2.buf) < 1) {
		परिणाम = error(_("Could not rename config section '%s' to '%s'"),
			       buf.buf, buf2.buf);
		जाओ out;
	}

	strbuf_addf(&old_remote_context, ":refs/remotes/%s/", नाम.old_name);

	क्रम (पूर्णांक i = 0; i < oldremote->fetch.nr && !refspecs_need_update; i++)
		refspecs_need_update = !!म_माला(oldremote->fetch.वस्तुएँ[i].raw,
						old_remote_context.buf);

	अगर (refspecs_need_update) {
		काष्ठा refs_for_each_ref_options opts = {
			.flags = REFS_FOR_EACH_INCLUDE_BROKEN,
		};
		नाम.transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
							       0, &err);
		अगर (!नाम.transaction)
			जाओ out;

		अगर (show_progress)
			नाम.progress = start_delayed_progress(the_repository,
								 _("Renaming remote references"), 0);

		strbuf_reset(&buf);
		strbuf_addf(&buf, "refs/remotes/%s/", नाम.old_name);
		opts.prefix = buf.buf;

		परिणाम = refs_for_each_ref_ext(get_main_ref_store(the_repository),
					       rename_one_ref, &नाम, &opts);
		अगर (परिणाम < 0)
			die(_("queueing remote ref renames failed: %s"), नाम.err->buf);

		परिणाम = ref_transaction_prepare(नाम.transaction, &err);
		अगर (परिणाम < 0) {
			error("renaming remote references failed: %s", err.buf);
			अगर (परिणाम == REF_TRANSACTION_ERROR_NAME_CONFLICT)
				advise(conflicting_remote_refs_advice);
			die(शून्य);
		}
	}

	अगर (oldremote->fetch.nr) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.fetch", नाम.new_name);
		repo_config_set_multivar(the_repository, buf.buf, शून्य, शून्य, CONFIG_FLAGS_MULTI_REPLACE);
		क्रम (पूर्णांक i = 0; i < oldremote->fetch.nr; i++) {
			अक्षर *ptr;

			strbuf_reset(&buf2);
			strbuf_addstr(&buf2, oldremote->fetch.वस्तुएँ[i].raw);
			ptr = म_माला(buf2.buf, old_remote_context.buf);
			अगर (ptr) {
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
	क्रम (माप_प्रकार i = 0; i < branch_list.nr; i++) {
		काष्ठा string_list_item *item = branch_list.वस्तुएँ + i;
		काष्ठा branch_info *info = item->util;
		अगर (info->remote_name && !म_भेद(info->remote_name, नाम.old_name)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "branch.%s.remote", item->string);
			repo_config_set(the_repository, buf.buf, नाम.new_name);
		}
		अगर (info->धकेलो_remote_name && !म_भेद(info->धकेलो_remote_name, नाम.old_name)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "branch.%s.pushRemote", item->string);
			repo_config_set(the_repository, buf.buf, नाम.new_name);
		}
	}

	अगर (refspecs_need_update) {
		परिणाम = ref_transaction_commit(नाम.transaction, &err);
		अगर (परिणाम < 0)
			die(_("renaming remote refs failed: %s"), नाम.err->buf);

		stop_progress(&नाम.progress);

		handle_धकेलो_default(नाम.old_name, नाम.new_name);
	}

out:
	ref_transaction_free(नाम.transaction);
	strbuf_release(&old_remote_context);
	strbuf_release(&buf);
	strbuf_release(&buf2);
	strbuf_release(&buf3);
	strbuf_release(&err);
	वापस परिणाम;
}

अटल पूर्णांक rm(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
	      काष्ठा repository *repo UNUSED)
{
	काष्ठा option options[] = {
		OPT_END()
	};
	काष्ठा remote *remote;
	काष्ठा strbuf buf = STRBUF_INIT;
	काष्ठा known_remotes known_remotes = { शून्य, शून्य };
	काष्ठा string_list branches = STRING_LIST_INIT_DUP;
	काष्ठा string_list skipped = STRING_LIST_INIT_DUP;
	काष्ठा branches_for_remote cb_तथ्य;
	पूर्णांक परिणाम;

	स_रखो(&cb_तथ्य, 0, माप(cb_तथ्य));
	cb_तथ्य.branches = &branches;
	cb_तथ्य.skipped = &skipped;
	cb_तथ्य.keep = &known_remotes;

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_rm_usage, 0);
	अगर (argc != 1)
		usage_with_options(builtin_remote_rm_usage, options);

	remote = remote_get(argv[0]);
	अगर (!remote_is_configured(remote, 1)) {
		error(_("No such remote: '%s'"), argv[0]);
		निकास(2);
	}

	known_remotes.to_delete = remote;
	for_each_remote(add_known_remote, &known_remotes);

	read_branches();
	क्रम (माप_प्रकार i = 0; i < branch_list.nr; i++) {
		काष्ठा string_list_item *item = branch_list.वस्तुएँ + i;
		काष्ठा branch_info *info = item->util;
		अगर (info->remote_name && !म_भेद(info->remote_name, remote->name)) {
			स्थिर अक्षर *कुंजियाँ[] = { "remote", "merge", शून्य }, **k;
			क्रम (k = कुंजियाँ; *k; k++) {
				strbuf_reset(&buf);
				strbuf_addf(&buf, "branch.%s.%s",
						item->string, *k);
				परिणाम = repo_config_set_gently(the_repository, buf.buf, शून्य);
				अगर (परिणाम && परिणाम != CONFIG_NOTHING_SET)
					die(_("could not unset '%s'"), buf.buf);
			}
		}
		अगर (info->धकेलो_remote_name && !म_भेद(info->धकेलो_remote_name, remote->name)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "branch.%s.pushremote", item->string);
			परिणाम = repo_config_set_gently(the_repository, buf.buf, शून्य);
			अगर (परिणाम && परिणाम != CONFIG_NOTHING_SET)
				die(_("could not unset '%s'"), buf.buf);
		}
	}

	/*
	 * We cannot just pass a function to for_each_ref() which deletes
	 * the branches one by one, since for_each_ref() relies on cached
	 * refs, which are invalidated when deleting a branch.
	 */
	cb_तथ्य.remote = remote;
	परिणाम = refs_for_each_ref(get_main_ref_store(the_repository),
				   add_branch_for_removal, &cb_तथ्य);
	strbuf_release(&buf);

	अगर (!परिणाम)
		परिणाम = refs_delete_refs(get_main_ref_store(the_repository),
					  "remote: remove", &branches,
					  REF_NO_DEREF);
	string_list_clear(&branches, 0);

	अगर (skipped.nr) {
		fprintf_ln(मानक_त्रुटि,
			   Q_("Note: A branch outside the refs/remotes/ hierarchy was not removed;\n"
			      "to delete it, use:",
			      "Note: Some branches outside the refs/remotes/ hierarchy were not removed;\n"
			      "to delete them, use:",
			      skipped.nr));
		क्रम (माप_प्रकार i = 0; i < skipped.nr; i++)
			ख_लिखो(मानक_त्रुटि, "  git branch -d %s\n",
				skipped.वस्तुएँ[i].string);
	}
	string_list_clear(&skipped, 0);

	अगर (!परिणाम) {
		strbuf_addf(&buf, "remote.%s", remote->name);
		अगर (repo_config_rename_section(the_repository, buf.buf, शून्य) < 1) {
			परिणाम = error(_("Could not remove config section '%s'"), buf.buf);
			जाओ out;
		}

		handle_धकेलो_default(remote->name, शून्य);
	}

out:
	क्रम (काष्ठा known_remote *r = known_remotes.list; r;) {
		काष्ठा known_remote *अग्र = r->अग्र;
		मुक्त(r);
		r = अग्र;
	}
	strbuf_release(&buf);
	वापस परिणाम;
}

अटल व्योम clear_धकेलो_info(व्योम *util, स्थिर अक्षर *string UNUSED)
{
	काष्ठा धकेलो_info *info = util;
	मुक्त(info->dest);
	मुक्त(info);
}

अटल व्योम free_remote_ref_states(काष्ठा ref_states *states)
{
	string_list_clear(&states->new_refs, 0);
	string_list_clear(&states->skipped, 0);
	string_list_clear(&states->stale, 1);
	string_list_clear(&states->tracked, 0);
	string_list_clear(&states->heads, 0);
	string_list_clear_func(&states->धकेलो, clear_धकेलो_info);
}

अटल पूर्णांक append_ref_to_tracked_list(स्थिर काष्ठा reference *ref, व्योम *cb_तथ्य)
{
	काष्ठा ref_states *states = cb_तथ्य;
	काष्ठा refspec_item refspec;

	अगर (ref->flags & REF_ISSYMREF)
		वापस 0;

	स_रखो(&refspec, 0, माप(refspec));
	refspec.dst = (अक्षर *)ref->name;
	अगर (!remote_find_tracking(states->remote, &refspec)) {
		string_list_append(&states->tracked, abbrev_branch(refspec.src));
		मुक्त(refspec.src);
	}

	वापस 0;
}

अटल पूर्णांक get_remote_ref_states(स्थिर अक्षर *name,
				 काष्ठा ref_states *states,
				 पूर्णांक query)
{
	states->remote = remote_get(name);
	अगर (!states->remote)
		वापस error(_("No such remote: '%s'"), name);

	read_branches();

	अगर (query) {
		काष्ठा transport *transport;
		स्थिर काष्ठा ref *remote_refs;

		transport = transport_get(states->remote, states->remote->url.v[0]);
		remote_refs = transport_get_remote_refs(transport, शून्य);

		states->queried = 1;
		अगर (query & GET_REF_STATES)
			get_ref_states(remote_refs, states);
		अगर (query & GET_शीर्ष_NAMES)
			get_शीर्ष_names(remote_refs, states);
		अगर (query & GET_धकेलो_REF_STATES)
			get_धकेलो_ref_states(remote_refs, states);
		transport_disconnect(transport);
	} अन्यथा {
		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_ref_to_tracked_list, states);
		string_list_sort(&states->tracked);
		get_धकेलो_ref_states_noquery(states);
	}

	वापस 0;
}

काष्ठा show_info {
	काष्ठा string_list list;
	काष्ठा ref_states states;
	पूर्णांक width, width2;
	पूर्णांक any_rebase;
};

#घोषणा SHOW_INFO_INIT { \
	.list = STRING_LIST_INIT_DUP, \
	.states = REF_STATES_INIT, \
}

अटल पूर्णांक add_remote_to_show_info(काष्ठा string_list_item *item, व्योम *cb_तथ्य)
{
	काष्ठा show_info *info = cb_तथ्य;
	पूर्णांक n = म_माप(item->string);
	अगर (n > info->width)
		info->width = n;
	string_list_जोड़ो(&info->list, item->string);
	वापस 0;
}

अटल पूर्णांक show_remote_info_item(काष्ठा string_list_item *item, व्योम *cb_तथ्य)
{
	काष्ठा show_info *info = cb_तथ्य;
	काष्ठा ref_states *states = &info->states;
	स्थिर अक्षर *name = item->string;

	अगर (states->queried) {
		स्थिर अक्षर *fmt = "%s";
		स्थिर अक्षर *arg = "";
		अगर (string_list_has_string(&states->new_refs, name)) {
			fmt = _(" new (next fetch will store in remotes/%s)");
			arg = states->remote->name;
		} अन्यथा अगर (string_list_has_string(&states->tracked, name))
			arg = _(" tracked");
		अन्यथा अगर (string_list_has_string(&states->skipped, name))
			arg = _(" skipped");
		अन्यथा अगर (string_list_has_string(&states->stale, name))
			arg = _(" stale (use 'git remote prune' to remove)");
		अन्यथा
			arg = _(" ???");
		म_लिखो("    %-*s", info->width, name);
		म_लिखो(fmt, arg);
		म_लिखो("\n");
	} अन्यथा
		म_लिखो("    %s\n", name);

	वापस 0;
}

अटल पूर्णांक add_local_to_show_info(काष्ठा string_list_item *branch_item, व्योम *cb_तथ्य)
{
	काष्ठा show_info *show_info = cb_तथ्य;
	काष्ठा ref_states *states = &show_info->states;
	काष्ठा branch_info *branch_info = branch_item->util;
	काष्ठा string_list_item *item;
	पूर्णांक n;

	अगर (!branch_info->merge.nr || !branch_info->remote_name ||
	    म_भेद(states->remote->name, branch_info->remote_name))
		वापस 0;
	अगर ((n = म_माप(branch_item->string)) > show_info->width)
		show_info->width = n;
	अगर (branch_info->rebase >= REBASE_TRUE)
		show_info->any_rebase = 1;

	item = string_list_जोड़ो(&show_info->list, branch_item->string);
	item->util = branch_info;

	वापस 0;
}

अटल पूर्णांक show_local_info_item(काष्ठा string_list_item *item, व्योम *cb_तथ्य)
{
	काष्ठा show_info *show_info = cb_तथ्य;
	काष्ठा branch_info *branch_info = item->util;
	काष्ठा string_list *merge = &branch_info->merge;
	पूर्णांक width = show_info->width + 4;

	अगर (branch_info->rebase >= REBASE_TRUE && branch_info->merge.nr > 1) {
		error(_("invalid branch.%s.merge; cannot rebase onto > 1 branch"),
			item->string);
		वापस 0;
	}

	म_लिखो("    %-*s ", show_info->width, item->string);
	अगर (branch_info->rebase >= REBASE_TRUE) {
		स्थिर अक्षर *संदेश;
		अगर (branch_info->rebase == REBASE_INTERACTIVE)
			संदेश = _("rebases interactively onto remote %s");
		अन्यथा अगर (branch_info->rebase == REBASE_MERGES)
			संदेश = _("rebases interactively (with merges) onto "
				"remote %s");
		अन्यथा
			संदेश = _("rebases onto remote %s");
		printf_ln(संदेश, merge->वस्तुएँ[0].string);
		वापस 0;
	} अन्यथा अगर (show_info->any_rebase) {
		printf_ln(_(" merges with remote %s"), merge->वस्तुएँ[0].string);
		width++;
	} अन्यथा {
		printf_ln(_("merges with remote %s"), merge->वस्तुएँ[0].string);
	}
	क्रम (माप_प्रकार i = 1; i < merge->nr; i++)
		म_लिखो(_("%-*s    and with remote %s\n"), width, "",
		       merge->वस्तुएँ[i].string);

	वापस 0;
}

अटल पूर्णांक add_धकेलो_to_show_info(काष्ठा string_list_item *धकेलो_item, व्योम *cb_तथ्य)
{
	काष्ठा show_info *show_info = cb_तथ्य;
	काष्ठा धकेलो_info *धकेलो_info = धकेलो_item->util;
	काष्ठा string_list_item *item;
	पूर्णांक n;
	अगर ((n = म_माप(धकेलो_item->string)) > show_info->width)
		show_info->width = n;
	अगर ((n = म_माप(धकेलो_info->dest)) > show_info->width2)
		show_info->width2 = n;
	item = string_list_append(&show_info->list, धकेलो_item->string);
	item->util = धकेलो_item->util;
	वापस 0;
}

/*
 * Sorting comparison for a string list that has push_info
 * structs in its util field
 */
अटल पूर्णांक cmp_string_with_धकेलो(स्थिर व्योम *va, स्थिर व्योम *vb)
{
	स्थिर काष्ठा string_list_item *a = va;
	स्थिर काष्ठा string_list_item *b = vb;
	स्थिर काष्ठा धकेलो_info *a_धकेलो = a->util;
	स्थिर काष्ठा धकेलो_info *b_धकेलो = b->util;
	पूर्णांक cmp = म_भेद(a->string, b->string);
	वापस cmp ? cmp : म_भेद(a_धकेलो->dest, b_धकेलो->dest);
}

अटल पूर्णांक show_धकेलो_info_item(काष्ठा string_list_item *item, व्योम *cb_तथ्य)
{
	काष्ठा show_info *show_info = cb_तथ्य;
	काष्ठा धकेलो_info *धकेलो_info = item->util;
	स्थिर अक्षर *src = item->string, *status = शून्य;

	चयन (धकेलो_info->status) {
	हाल धकेलो_STATUS_CREATE:
		status = _("create");
		अवरोध;
	हाल धकेलो_STATUS_DELETE:
		status = _("delete");
		src = _("(none)");
		अवरोध;
	हाल धकेलो_STATUS_UPTODATE:
		status = _("up to date");
		अवरोध;
	हाल धकेलो_STATUS_FASTFORWARD:
		status = _("fast-forwardable");
		अवरोध;
	हाल धकेलो_STATUS_OUTOFDATE:
		status = _("local out of date");
		अवरोध;
	हाल धकेलो_STATUS_NOTQUERIED:
		अवरोध;
	}
	अगर (status) {
		अगर (धकेलो_info->forced)
			printf_ln(_("    %-*s forces to %-*s (%s)"), show_info->width, src,
			       show_info->width2, धकेलो_info->dest, status);
		अन्यथा
			printf_ln(_("    %-*s pushes to %-*s (%s)"), show_info->width, src,
			       show_info->width2, धकेलो_info->dest, status);
	} अन्यथा {
		अगर (धकेलो_info->forced)
			printf_ln(_("    %-*s forces to %s"), show_info->width, src,
			       धकेलो_info->dest);
		अन्यथा
			printf_ln(_("    %-*s pushes to %s"), show_info->width, src,
			       धकेलो_info->dest);
	}
	वापस 0;
}

अटल पूर्णांक get_one_entry(काष्ठा remote *remote, व्योम *priv)
{
	काष्ठा string_list *list = priv;
	काष्ठा strbuf remote_info_buf = STRBUF_INIT;
	काष्ठा strvec *url;

	अगर (remote->url.nr > 0) {
		काष्ठा strbuf promisor_config = STRBUF_INIT;
		स्थिर अक्षर *partial_clone_filter = शून्य;

		strbuf_addf(&promisor_config, "remote.%s.partialclonefilter", remote->name);
		strbuf_addf(&remote_info_buf, "%s (fetch)", remote->url.v[0]);
		अगर (!repo_config_get_string_tmp(the_repository, promisor_config.buf, &partial_clone_filter))
			strbuf_addf(&remote_info_buf, " [%s]", partial_clone_filter);

		strbuf_release(&promisor_config);
		string_list_append(list, remote->name)->util =
				strbuf_detach(&remote_info_buf, शून्य);
	} अन्यथा
		string_list_append(list, remote->name)->util = शून्य;
	url = धकेलो_url_of_remote(remote);
	क्रम (माप_प्रकार i = 0; i < url->nr; i++) {
		strbuf_addf(&remote_info_buf, "%s (push)", url->v[i]);
		string_list_append(list, remote->name)->util =
				strbuf_detach(&remote_info_buf, शून्य);
	}

	वापस 0;
}

अटल पूर्णांक show_all(व्योम)
{
	काष्ठा string_list list = STRING_LIST_INIT_DUP;
	पूर्णांक परिणाम;

	परिणाम = for_each_remote(get_one_entry, &list);

	अगर (!परिणाम) {
		string_list_sort(&list);
		क्रम (माप_प्रकार i = 0; i < list.nr; i++) {
			काष्ठा string_list_item *item = list.वस्तुएँ + i;
			अगर (verbose)
				म_लिखो("%s\t%s\n", item->string,
					item->util ? (स्थिर अक्षर *)item->util : "");
			अन्यथा {
				अगर (i && !म_भेद((item - 1)->string, item->string))
					जारी;
				म_लिखो("%s\n", item->string);
			}
		}
	}
	string_list_clear(&list, 1);
	वापस परिणाम;
}

अटल पूर्णांक show(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
		काष्ठा repository *repo UNUSED)
{
	पूर्णांक no_query = 0, परिणाम = 0, query_flag = 0;
	काष्ठा option options[] = {
		OPT_BOOL('n', शून्य, &no_query, N_("do not query remotes")),
		OPT_END()
	};
	काष्ठा show_info info = SHOW_INFO_INIT;

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_show_usage,
			     0);

	अगर (argc < 1)
		वापस show_all();

	अगर (!no_query)
		query_flag = (GET_REF_STATES | GET_शीर्ष_NAMES | GET_धकेलो_REF_STATES);

	क्रम (; argc; argc--, argv++) {
		माप_प्रकार i;
		काष्ठा strvec *url;

		get_remote_ref_states(*argv, &info.states, query_flag);

		printf_ln(_("* remote %s"), *argv);
		printf_ln(_("  Fetch URL: %s"), info.states.remote->url.v[0]);
		url = धकेलो_url_of_remote(info.states.remote);
		क्रम (i = 0; i < url->nr; i++)
			/*
			 * TRANSLATORS: the colon ':' should align
			 * with the one in " Fetch URL: %s"
			 * translation.
			 */
			printf_ln(_("  Push  URL: %s"), url->v[i]);
		अगर (!i)
			printf_ln(_("  Push  URL: %s"), _("(no URL)"));
		अगर (no_query)
			printf_ln(_("  HEAD branch: %s"), _("(not queried)"));
		अन्यथा अगर (!info.states.heads.nr)
			printf_ln(_("  HEAD branch: %s"), _("(unknown)"));
		अन्यथा अगर (info.states.heads.nr == 1)
			printf_ln(_("  HEAD branch: %s"), info.states.heads.वस्तुएँ[0].string);
		अन्यथा {
			म_लिखो(_("  HEAD branch (remote HEAD is ambiguous,"
				 " may be one of the following):\n"));
			क्रम (i = 0; i < info.states.heads.nr; i++)
				म_लिखो("    %s\n", info.states.heads.वस्तुएँ[i].string);
		}

		/* remote branch info */
		info.width = 0;
		for_each_string_list(&info.states.new_refs, add_remote_to_show_info, &info);
		for_each_string_list(&info.states.skipped, add_remote_to_show_info, &info);
		for_each_string_list(&info.states.tracked, add_remote_to_show_info, &info);
		for_each_string_list(&info.states.stale, add_remote_to_show_info, &info);
		अगर (info.list.nr)
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
		अगर (info.list.nr)
			printf_ln(Q_("  Local branch configured for 'git pull':",
				     "  Local branches configured for 'git pull':",
				     info.list.nr));
		for_each_string_list(&info.list, show_local_info_item, &info);
		string_list_clear(&info.list, 0);

		/* git push info */
		अगर (info.states.remote->mirror)
			printf_ln(_("  Local refs will be mirrored by 'git push'"));

		info.width = info.width2 = 0;
		for_each_string_list(&info.states.धकेलो, add_धकेलो_to_show_info, &info);
		QSORT(info.list.वस्तुएँ, info.list.nr, cmp_string_with_धकेलो);
		अगर (info.list.nr)
			printf_ln(Q_("  Local ref configured for 'git push'%s:",
				     "  Local refs configured for 'git push'%s:",
				     info.list.nr),
				  no_query ? _(" (status not queried)") : "");
		for_each_string_list(&info.list, show_धकेलो_info_item, &info);
		string_list_clear(&info.list, 0);

		free_remote_ref_states(&info.states);
	}

	वापस परिणाम;
}

अटल व्योम report_set_शीर्ष_auto(स्थिर अक्षर *remote, स्थिर अक्षर *शीर्ष_name,
			काष्ठा strbuf *b_local_शीर्ष, पूर्णांक was_detached) {
	काष्ठा strbuf buf_prefix = STRBUF_INIT;
	स्थिर अक्षर *पूर्व_शीर्ष = शून्य;

	strbuf_addf(&buf_prefix, "refs/remotes/%s/", remote);
	skip_prefix(b_local_शीर्ष->buf, buf_prefix.buf, &पूर्व_शीर्ष);

	अगर (पूर्व_शीर्ष && !म_भेद(पूर्व_शीर्ष, शीर्ष_name))
		म_लिखो(_("'%s/HEAD' is unchanged and points to '%s'\n"),
			remote, शीर्ष_name);
	अन्यथा अगर (पूर्व_शीर्ष)
		म_लिखो(_("'%s/HEAD' has changed from '%s' and now points to '%s'\n"),
			remote, पूर्व_शीर्ष, शीर्ष_name);
	अन्यथा अगर (!b_local_शीर्ष->len)
		म_लिखो(_("'%s/HEAD' is now created and points to '%s'\n"),
			remote, शीर्ष_name);
	अन्यथा अगर (was_detached && b_local_शीर्ष->len)
		म_लिखो(_("'%s/HEAD' was detached at '%s' and now points to '%s'\n"),
			remote, b_local_शीर्ष->buf, शीर्ष_name);
	अन्यथा
		म_लिखो(_("'%s/HEAD' used to point to '%s' "
			"(which is not a remote branch), but now points to '%s'\n"),
			remote, b_local_शीर्ष->buf, शीर्ष_name);
	strbuf_release(&buf_prefix);
}

अटल पूर्णांक set_शीर्ष(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
		    काष्ठा repository *repo UNUSED)
{
	पूर्णांक opt_a = 0, opt_d = 0, परिणाम = 0, was_detached;
	काष्ठा strbuf b_शीर्ष = STRBUF_INIT, b_remote_शीर्ष = STRBUF_INIT,
		b_local_शीर्ष = STRBUF_INIT;
	अक्षर *शीर्ष_name = शून्य;
	काष्ठा ref_store *refs = get_main_ref_store(the_repository);
	काष्ठा remote *remote;

	काष्ठा option options[] = {
		OPT_BOOL('a', "auto", &opt_a,
			 N_("set refs/remotes/<name>/HEAD according to remote")),
		OPT_BOOL('d', "delete", &opt_d,
			 N_("delete refs/remotes/<name>/HEAD")),
		OPT_END()
	};
	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_sethead_usage, 0);

	/* All modes require at least a remote name. */
	अगर (!argc)
		usage_with_options(builtin_remote_sethead_usage, options);

	strbuf_addf(&b_शीर्ष, "refs/remotes/%s/HEAD", argv[0]);
	remote = remote_get(argv[0]);

	अगर (!opt_a && !opt_d && argc == 2) {
		शीर्ष_name = xstrdup(argv[1]);
	} अन्यथा अगर (opt_a && !opt_d && argc == 1) {
		काष्ठा ref_states states = REF_STATES_INIT;
		get_remote_ref_states(argv[0], &states, GET_शीर्ष_NAMES);
		अगर (!states.heads.nr)
			परिणाम |= error(_("Cannot determine remote HEAD"));
		अन्यथा अगर (states.heads.nr > 1) {
			परिणाम |= error(_("Multiple remote HEAD branches. "
					  "Please choose one explicitly with:"));
			क्रम (माप_प्रकार i = 0; i < states.heads.nr; i++)
				ख_लिखो(मानक_त्रुटि, "  git remote set-head %s %s\n",
					argv[0], states.heads.वस्तुएँ[i].string);
		} अन्यथा
			शीर्ष_name = xstrdup(states.heads.वस्तुएँ[0].string);
		free_remote_ref_states(&states);
	} अन्यथा अगर (opt_d && !opt_a && argc == 1) {
		अगर (refs_delete_ref(refs, शून्य, b_शीर्ष.buf, शून्य, REF_NO_DEREF))
			परिणाम |= error(_("Could not delete %s"), b_शीर्ष.buf);
	} अन्यथा
		usage_with_options(builtin_remote_sethead_usage, options);

	अगर (!शीर्ष_name)
		जाओ cleanup;
	strbuf_addf(&b_remote_शीर्ष, "refs/remotes/%s/%s", argv[0], शीर्ष_name);
	अगर (!refs_ref_exists(refs, b_remote_शीर्ष.buf)) {
		परिणाम |= error(_("Not a valid ref: %s"), b_remote_शीर्ष.buf);
		जाओ cleanup;
	}
	was_detached = refs_update_symref_extended(refs, b_शीर्ष.buf, b_remote_शीर्ष.buf,
			"remote set-head", &b_local_शीर्ष, 0);
	अगर (was_detached == -1) {
		परिणाम |= error(_("Could not set up %s"), b_शीर्ष.buf);
		जाओ cleanup;
	}
	अगर (opt_a)
		report_set_शीर्ष_auto(argv[0], शीर्ष_name, &b_local_शीर्ष, was_detached);
	अगर (remote->follow_remote_शीर्ष == FOLLOW_REMOTE_ALWAYS) {
		काष्ठा strbuf config_name = STRBUF_INIT;
		strbuf_addf(&config_name,
			"remote.%s.followremotehead", remote->name);
		repo_config_set(the_repository, config_name.buf, "warn");
		strbuf_release(&config_name);
	}

cleanup:
	मुक्त(शीर्ष_name);
	strbuf_release(&b_शीर्ष);
	strbuf_release(&b_remote_शीर्ष);
	strbuf_release(&b_local_शीर्ष);
	वापस परिणाम;
}

अटल पूर्णांक prune_remote(स्थिर अक्षर *remote, पूर्णांक dry_run)
{
	पूर्णांक परिणाम = 0;
	काष्ठा ref_states states = REF_STATES_INIT;
	काष्ठा string_list refs_to_prune = STRING_LIST_INIT_NODUP;
	काष्ठा string_list_item *item;

	get_remote_ref_states(remote, &states, GET_REF_STATES);

	अगर (!states.stale.nr) {
		free_remote_ref_states(&states);
		वापस 0;
	}

	printf_ln(_("Pruning %s"), remote);
	printf_ln(_("URL: %s"), states.remote->url.v[0]);

	for_each_string_list_item(item, &states.stale)
		string_list_append(&refs_to_prune, item->util);
	string_list_sort(&refs_to_prune);

	अगर (!dry_run)
		परिणाम |= refs_delete_refs(get_main_ref_store(the_repository),
					   "remote: prune", &refs_to_prune, 0);

	for_each_string_list_item(item, &states.stale) {
		स्थिर अक्षर *refname = item->util;

		अगर (dry_run)
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
	वापस परिणाम;
}

अटल पूर्णांक prune(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
		 काष्ठा repository *repo UNUSED)
{
	पूर्णांक dry_run = 0, परिणाम = 0;
	काष्ठा option options[] = {
		OPT__DRY_RUN(&dry_run, N_("dry run")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_prune_usage, 0);

	अगर (argc < 1)
		usage_with_options(builtin_remote_prune_usage, options);

	क्रम (; argc; argc--, argv++)
		परिणाम |= prune_remote(*argv, dry_run);

	वापस परिणाम;
}

अटल पूर्णांक get_remote_default(स्थिर अक्षर *कुंजी, स्थिर अक्षर *मूल्य UNUSED,
			      स्थिर काष्ठा config_context *ctx UNUSED,
			      व्योम *priv)
{
	अगर (म_भेद(कुंजी, "remotes.default") == 0) {
		पूर्णांक *found = priv;
		*found = 1;
	}
	वापस 0;
}

अटल पूर्णांक update(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
		  काष्ठा repository *repo UNUSED)
{
	पूर्णांक i, prune = -1;
	काष्ठा option options[] = {
		OPT_BOOL('p', "prune", &prune,
			 N_("prune remotes after fetching")),
		OPT_END()
	};
	काष्ठा child_process cmd = CHILD_PROCESS_INIT;
	पूर्णांक default_defined = 0;

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_update_usage,
			     PARSE_OPT_KEEP_ARGV0);

	strvec_धकेलो(&cmd.args, "fetch");

	अगर (prune != -1)
		strvec_धकेलो(&cmd.args, prune ? "--prune" : "--no-prune");
	अगर (verbose)
		strvec_धकेलो(&cmd.args, "-v");
	strvec_धकेलो(&cmd.args, "--multiple");
	अगर (argc < 2)
		strvec_धकेलो(&cmd.args, "default");
	क्रम (i = 1; i < argc; i++)
		strvec_धकेलो(&cmd.args, argv[i]);

	अगर (म_भेद(cmd.args.v[cmd.args.nr-1], "default") == 0) {
		repo_config(the_repository, get_remote_default, &default_defined);
		अगर (!default_defined) {
			strvec_निकालो(&cmd.args);
			strvec_धकेलो(&cmd.args, "--all");
		}
	}

	cmd.git_cmd = 1;
	वापस run_command(&cmd);
}

अटल पूर्णांक remove_all_fetch_refspecs(स्थिर अक्षर *कुंजी)
{
	वापस repo_config_set_multivar_gently(the_repository, कुंजी, शून्य, शून्य,
					       CONFIG_FLAGS_MULTI_REPLACE);
}

अटल व्योम add_branches(काष्ठा remote *remote, स्थिर अक्षर **branches,
			 स्थिर अक्षर *कुंजी)
{
	स्थिर अक्षर *remotename = remote->name;
	पूर्णांक mirror = remote->mirror;
	काष्ठा strbuf refspec = STRBUF_INIT;

	क्रम (; *branches; branches++)
		add_branch(कुंजी, *branches, remotename, mirror, &refspec);

	strbuf_release(&refspec);
}

अटल पूर्णांक set_remote_branches(स्थिर अक्षर *remotename, स्थिर अक्षर **branches,
				पूर्णांक add_mode)
{
	काष्ठा strbuf कुंजी = STRBUF_INIT;
	काष्ठा remote *remote;

	strbuf_addf(&कुंजी, "remote.%s.fetch", remotename);

	remote = remote_get(remotename);
	अगर (!remote_is_configured(remote, 1)) {
		error(_("No such remote '%s'"), remotename);
		निकास(2);
	}

	अगर (!add_mode && remove_all_fetch_refspecs(कुंजी.buf)) {
		strbuf_release(&कुंजी);
		वापस 1;
	}
	add_branches(remote, branches, कुंजी.buf);

	strbuf_release(&कुंजी);
	वापस 0;
}

अटल पूर्णांक set_branches(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
			काष्ठा repository *repo UNUSED)
{
	पूर्णांक add_mode = 0;
	काष्ठा option options[] = {
		OPT_BOOL('\0', "add", &add_mode, N_("add branch")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_setbranches_usage, 0);
	अगर (argc == 0) {
		error(_("no remote specified"));
		usage_with_options(builtin_remote_setbranches_usage, options);
	}
	argv[argc] = शून्य;

	वापस set_remote_branches(argv[0], argv + 1, add_mode);
}

अटल पूर्णांक get_url(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
		   काष्ठा repository *repo UNUSED)
{
	पूर्णांक धकेलो_mode = 0, all_mode = 0;
	स्थिर अक्षर *remotename = शून्य;
	काष्ठा remote *remote;
	काष्ठा strvec *url;
	काष्ठा option options[] = {
		OPT_BOOL('\0', "push", &धकेलो_mode,
			 N_("query push URLs rather than fetch URLs")),
		OPT_BOOL('\0', "all", &all_mode,
			 N_("return all URLs")),
		OPT_END()
	};
	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_geturl_usage, 0);

	अगर (argc != 1)
		usage_with_options(builtin_remote_geturl_usage, options);

	remotename = argv[0];

	remote = remote_get(remotename);
	अगर (!remote_is_configured(remote, 1)) {
		error(_("No such remote '%s'"), remotename);
		निकास(2);
	}

	url = धकेलो_mode ? धकेलो_url_of_remote(remote) : &remote->url;

	अगर (all_mode) {
		क्रम (माप_प्रकार i = 0; i < url->nr; i++)
			printf_ln("%s", url->v[i]);
	} अन्यथा {
		printf_ln("%s", url->v[0]);
	}

	वापस 0;
}

अटल पूर्णांक set_url(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
		   काष्ठा repository *repo UNUSED)
{
	पूर्णांक धकेलो_mode = 0, add_mode = 0, delete_mode = 0;
	पूर्णांक matches = 0, negative_matches = 0;
	स्थिर अक्षर *remotename = शून्य;
	स्थिर अक्षर *newurl = शून्य;
	स्थिर अक्षर *oldurl = शून्य;
	काष्ठा remote *remote;
	regex_t old_regex;
	काष्ठा strvec *urlset;
	काष्ठा strbuf name_buf = STRBUF_INIT;
	काष्ठा option options[] = {
		OPT_BOOL('\0', "push", &धकेलो_mode,
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

	अगर (add_mode && delete_mode)
		die(_("--add --delete doesn't make sense"));

	अगर (argc < 3 || argc > 4 || ((add_mode || delete_mode) && argc != 3))
		usage_with_options(builtin_remote_seturl_usage, options);

	remotename = argv[1];
	newurl = argv[2];
	अगर (argc > 3)
		oldurl = argv[3];

	अगर (delete_mode)
		oldurl = newurl;

	remote = remote_get(remotename);
	अगर (!remote_is_configured(remote, 1)) {
		error(_("No such remote '%s'"), remotename);
		निकास(2);
	}

	अगर (धकेलो_mode) {
		strbuf_addf(&name_buf, "remote.%s.pushurl", remotename);
		urlset = &remote->pushurl;
	} अन्यथा {
		strbuf_addf(&name_buf, "remote.%s.url", remotename);
		urlset = &remote->url;
	}

	/* Special cases that add new entry. */
	अगर ((!oldurl && !delete_mode) || add_mode) {
		अगर (add_mode)
			repo_config_set_multivar(the_repository, name_buf.buf, newurl,
						       "^$", 0);
		अन्यथा
			repo_config_set(the_repository, name_buf.buf, newurl);
		जाओ out;
	}

	/* Old URL specified. Demand that one matches. */
	अगर (regcomp(&old_regex, oldurl, REG_EXTENDED))
		die(_("Invalid old URL pattern: %s"), oldurl);

	क्रम (माप_प्रकार i = 0; i < urlset->nr; i++)
		अगर (!regexec(&old_regex, urlset->v[i], 0, शून्य, 0))
			matches++;
		अन्यथा
			negative_matches++;
	अगर (!delete_mode && !matches)
		die(_("No such URL found: %s"), oldurl);
	अगर (delete_mode && !negative_matches && !धकेलो_mode)
		die(_("Will not delete all non-push URLs"));

	regfree(&old_regex);

	अगर (!delete_mode)
		repo_config_set_multivar(the_repository, name_buf.buf, newurl, oldurl, 0);
	अन्यथा
		repo_config_set_multivar(the_repository, name_buf.buf, शून्य, oldurl,
					 CONFIG_FLAGS_MULTI_REPLACE);
out:
	strbuf_release(&name_buf);
	वापस 0;
}

पूर्णांक cmd_remote(पूर्णांक argc,
	       स्थिर अक्षर **argv,
	       स्थिर अक्षर *prefix,
	       काष्ठा repository *repo)
{
	parse_opt_subcommand_fn *fn = शून्य;
	काष्ठा option options[] = {
		OPT__VERBOSE(&verbose, N_("be verbose; must be placed before a subcommand")),
		OPT_SUBCOMMAND("add", &fn, add),
		OPT_SUBCOMMAND("rename", &fn, mv),
		OPT_SUBCOMMAND_F("rm", &fn, rm, शून्य, PARSE_OPT_NOCOMPLETE),
		OPT_SUBCOMMAND("remove", &fn, rm),
		OPT_SUBCOMMAND("set-head", &fn, set_शीर्ष),
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

	अगर (fn) {
		वापस !!fn(argc, argv, prefix, repo);
	} अन्यथा {
		अगर (argc) {
			error(_("unknown subcommand: `%s'"), argv[0]);
			usage_with_options(builtin_remote_usage, options);
		}
		वापस !!show_all();
	}
}
