#परिभाषय USE_THE_REPOSITORY_VARIABLE

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

स्थिर ध्रुव वर्ण * ध्रुव builtin_remote_सङ्कलय_usage[] = {
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

#परिभाषय GET_REF_अवस्थाः (1<<0)
#परिभाषय GET_शीर्ष_नामानि (1<<1)
#परिभाषय GET_प्रक्षिप_REF_अवस्थाः (1<<2)

स्थिर पूर्णाङ्क verbose;

स्थिर पूर्णाङ्क fetch_remote(ध्रुव वर्ण *name)
{
	संरचना child_process cmd = CHILD_PROCESS_INIT;

	strvec_प्रक्षिप(&cmd.तर्काः, "fetch");
	यदि (verbose)
		strvec_प्रक्षिप(&cmd.तर्काः, "-v");
	strvec_प्रक्षिप(&cmd.तर्काः, name);
	cmd.git_cmd = 1;
	printf_ln(_("Updating %s"), name);
	यदि (सततम्_command(&cmd))
		प्रत्यागम error(_("Could not fetch %s"), name);
	प्रत्यागम 0;
}

गणना {
	चिह्नकानि_UNSET = 0,
	चिह्नकानि_DEFAULT = 1,
	चिह्नकानि_समुच्चयः = 2
};

#परिभाषय MIRROR_NONE 0
#परिभाषय MIRROR_FETCH 1
#परिभाषय MIRROR_प्रक्षिप 2
#परिभाषय MIRROR_BOTH (MIRROR_FETCH|MIRROR_प्रक्षिप)

स्थिर शून्य सङ्कलय_branch(ध्रुव वर्ण *कुञ्चिका, ध्रुव वर्ण *branchname,
		       ध्रुव वर्ण *remotename, पूर्णाङ्क mirror, संरचना strbuf *tmp)
{
	strbuf_reset(tmp);
	strbuf_addch(tmp, '+');
	यदि (mirror)
		strbuf_addf(tmp, "refs/%s:refs/%s",
				branchname, branchname);
	अन्यथा
		strbuf_addf(tmp, "refs/heads/%s:refs/remotes/%s/%s",
				branchname, remotename, branchname);
	repo_config_समुच्चयः_multivar(the_repository, कुञ्चिका, tmp->buf, "^$", 0);
}

स्थिर ध्रुव वर्ण mirror_advice[] =
N_("--mirror is dangerous and deprecated; please\n"
   "\t use --mirror=fetch or --mirror=push instead");

स्थिर पूर्णाङ्क parse_mirror_opt(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg, पूर्णाङ्क not)
{
	अचिह्नित *mirror = opt->मानम्;
	यदि (not)
		*mirror = MIRROR_NONE;
	अन्यथा यदि (!arg) {
		warning("%s", _(mirror_advice));
		*mirror = MIRROR_BOTH;
	}
	अन्यथा यदि (!वाक्यं_तुलय(arg, "fetch"))
		*mirror = MIRROR_FETCH;
	अन्यथा यदि (!वाक्यं_तुलय(arg, "push"))
		*mirror = MIRROR_प्रक्षिप;
	अन्यथा
		प्रत्यागम error(_("unknown --mirror argument: %s"), arg);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क check_remote_collision(संरचना remote *remote, शून्य *दत्त)
{
	ध्रुव वर्ण *name = दत्त;
	ध्रुव वर्ण *p;

	यदि (skip_उपसर्गः(name, remote->name, &p) && *p == '/')
		die(_("remote name '%s' is a subset of existing remote '%s'"),
		    name, remote->name);
	यदि (skip_उपसर्गः(remote->name, name, &p) && *p == '/')
		die(_("remote name '%s' is a superset of existing remote '%s'"),
		    name, remote->name);

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क सङ्कलय(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
	       संरचना repository *repo अप्रयुक्तम्)
{
	पूर्णाङ्क fetch = 0, fetch_चिह्नकानि = चिह्नकानि_DEFAULT;
	अचिह्नित mirror = MIRROR_NONE;
	संरचना string_list track = STRING_LIST_INIT_NODUP;
	ध्रुव वर्ण *master = रिक्त;
	संरचना remote *remote;
	संरचना strbuf buf = STRBUF_INIT, buf2 = STRBUF_INIT;
	ध्रुव वर्ण *name, *url;
	पूर्णाङ्क परिणाम = 0;

	संरचना option options[] = {
		OPT_BOOL('f', "fetch", &fetch, N_("fetch the remote branches")),
		OPT_समुच्चयः_INT(0, "tags", &fetch_चिह्नकानि,
			    N_("import all tags and associated objects when fetching\n"
			       "or do not fetch any tag at all (--no-tags)"),
			    चिह्नकानि_समुच्चयः),
		OPT_STRING_LIST('t', "track", &track, N_("branch"),
				N_("branch(es) to track")),
		OPT_STRING('m', "master", &master, N_("branch"), N_("master branch")),
		OPT_CALLBACK_F(0, "mirror", &mirror, "(push|fetch)",
			N_("set up remote as a mirror to push to or fetch from"),
			PARSE_OPT_OPTARG | PARSE_OPT_COMP_ARG, parse_mirror_opt),
		OPT_अन्तः()
	};

	argc = parse_options(argc, argv, उपसर्गः, options,
			     builtin_remote_सङ्कलय_usage, 0);

	यदि (argc != 2)
		usage_with_options(builtin_remote_सङ्कलय_usage, options);

	यदि (mirror && master)
		die(_("specifying a master branch makes no sense with --mirror"));
	यदि (mirror && !(mirror & MIRROR_FETCH) && track.nr)
		die(_("specifying branches to track makes sense only with fetch mirrors"));

	name = argv[0];
	url = argv[1];

	remote = remote_get(name);
	यदि (remote_is_configured(remote, 1)) {
		error(_("remote %s already exists."), name);
		निर्गच्छ(3);
	}

	यदि (!valid_remote_name(name))
		die(_("'%s' is not a valid remote name"), name);

	for_each_remote(check_remote_collision, (शून्य *)name);

	strbuf_addf(&buf, "remote.%s.url", name);
	repo_config_समुच्चयः(the_repository, buf.buf, url);

	यदि (!mirror || mirror & MIRROR_FETCH) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.fetch", name);
		यदि (track.nr == 0)
			string_list_append(&track, "*");
		प्रति (आकार_t i = 0; i < track.nr; i++) {
			सङ्कलय_branch(buf.buf, track.वस्तूनि[i].string,
				   name, mirror, &buf2);
		}
	}

	यदि (mirror & MIRROR_प्रक्षिप) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.mirror", name);
		repo_config_समुच्चयः(the_repository, buf.buf, "true");
	}

	यदि (fetch_चिह्नकानि != चिह्नकानि_DEFAULT) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.tagOpt", name);
		repo_config_समुच्चयः(the_repository, buf.buf,
				fetch_चिह्नकानि == चिह्नकानि_समुच्चयः ? "--tags" : "--no-tags");
	}

	यदि (fetch && fetch_remote(name)) {
		परिणाम = 1;
		गच्छ बहिर्गमनम्;
	}

	यदि (master) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "refs/remotes/%s/HEAD", name);

		strbuf_reset(&buf2);
		strbuf_addf(&buf2, "refs/remotes/%s/%s", name, master);

		यदि (refs_update_symref(get_main_ref_store(the_repository), buf.buf, buf2.buf, "remote add"))
			परिणाम = error(_("Could not setup master '%s'"), master);
	}

बहिर्गमनम्:
	strbuf_release(&buf);
	strbuf_release(&buf2);
	string_list_clear(&track, 0);

	प्रत्यागम परिणाम;
}

संरचना branch_info {
	वर्ण *remote_name;
	संरचना string_list merge;
	गणना rebase_type rebase;
	वर्ण *प्रक्षिप_remote_name;
};

स्थिर संरचना string_list branch_list = STRING_LIST_INIT_DUP;

स्थिर ध्रुव वर्ण *abbrev_ref(ध्रुव वर्ण *name, ध्रुव वर्ण *उपसर्गः)
{
	skip_उपसर्गः(name, उपसर्गः, &name);
	प्रत्यागम name;
}
#परिभाषय abbrev_branch(name) abbrev_ref((name), "refs/heads/")

स्थिर पूर्णाङ्क config_read_branches(ध्रुव वर्ण *कुञ्चिका, ध्रुव वर्ण *मानम्,
				ध्रुव संरचना config_context *ctx अप्रयुक्तम्,
				शून्य *दत्त अप्रयुक्तम्)
{
	ध्रुव वर्ण *orig_कुञ्चिका = कुञ्चिका;
	वर्ण *name;
	संरचना string_list_item *item;
	संरचना branch_info *info;
	गणना { REMOTE, MERGE, REBASE, प्रक्षिप_REMOTE } type;
	आकार_t कुञ्चिका_len;

	यदि (!starts_with(कुञ्चिका, "branch."))
		प्रत्यागम 0;

	कुञ्चिका += वाक्यदैर्घ्यम्("branch.");
	यदि (strip_suffix(कुञ्चिका, ".remote", &कुञ्चिका_len))
		type = REMOTE;
	अन्यथा यदि (strip_suffix(कुञ्चिका, ".merge", &कुञ्चिका_len))
		type = MERGE;
	अन्यथा यदि (strip_suffix(कुञ्चिका, ".rebase", &कुञ्चिका_len))
		type = REBASE;
	अन्यथा यदि (strip_suffix(कुञ्चिका, ".pushremote", &कुञ्चिका_len))
		type = प्रक्षिप_REMOTE;
	अन्यथा
		प्रत्यागम 0;

	name = xmemdupz(कुञ्चिका, कुञ्चिका_len);
	item = string_list_निवेशय(&branch_list, name);

	यदि (!item->util)
		item->util = xcalloc(1, परिमाण(संरचना branch_info));
	info = item->util;
	विकल्प (type) {
	पक्ष REMOTE:
		यदि (info->remote_name)
			warning(_("more than one %s"), orig_कुञ्चिका);
		info->remote_name = xstrdup(मानम्);
		भङ्ग;
	पक्ष MERGE: {
		ध्रुव वर्ण *space = strchr(मानम्, ' ');
		मानम् = abbrev_branch(मानम्);
		यावत् (space) {
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
		यदि (info->rebase == REBASE_INVALID)
			warning(_("unhandled branch.%s.rebase=%s; assuming "
				  "'true'"), name, मानम्);
		भङ्ग;
	पक्ष प्रक्षिप_REMOTE:
		यदि (info->प्रक्षिप_remote_name)
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
	यदि (branch_list.nr)
		प्रत्यागम;
	repo_config(the_repository, config_read_branches, रिक्त);
}

संरचना ref_अवस्थाः {
	संरचना remote *remote;
	संरचना string_list new_refs, skipped, stale, tracked, heads, प्रक्षिप;
	पूर्णाङ्क queried;
};

#परिभाषय REF_अवस्थाः_INIT { \
	.new_refs = STRING_LIST_INIT_DUP, \
	.skipped = STRING_LIST_INIT_DUP, \
	.stale = STRING_LIST_INIT_DUP, \
	.tracked = STRING_LIST_INIT_DUP, \
	.heads = STRING_LIST_INIT_DUP, \
	.प्रक्षिप = STRING_LIST_INIT_DUP, \
}

स्थिर पूर्णाङ्क get_ref_अवस्थाः(ध्रुव संरचना ref *remote_refs, संरचना ref_अवस्थाः *अवस्थाः)
{
	संरचना ref *fetch_map = रिक्त, **पुच्छ = &fetch_map;
	संरचना ref *ref, *stale_refs;
	पूर्णाङ्क i;

	प्रति (i = 0; i < अवस्थाः->remote->fetch.nr; i++)
		यदि (get_fetch_map(remote_refs, &अवस्थाः->remote->fetch.वस्तूनि[i], &पुच्छ, 1))
			die(_("Could not get fetch map for refspec %s"),
				अवस्थाः->remote->fetch.वस्तूनि[i].raw);

	प्रति (ref = fetch_map; ref; ref = ref->अग्र) {
		यदि (refname_matches_negative_refspec_item(ref->name, &अवस्थाः->remote->fetch))
			string_list_append(&अवस्थाः->skipped, abbrev_branch(ref->name));
		अन्यथा यदि (!ref->peer_ref || !refs_ref_exists(get_main_ref_store(the_repository), ref->peer_ref->name))
			string_list_append(&अवस्थाः->new_refs, abbrev_branch(ref->name));
		अन्यथा
			string_list_append(&अवस्थाः->tracked, abbrev_branch(ref->name));
	}
	stale_refs = get_stale_heads(&अवस्थाः->remote->fetch, fetch_map);
	प्रति (ref = stale_refs; ref; ref = ref->अग्र) {
		संरचना string_list_item *item =
			string_list_append(&अवस्थाः->stale, abbrev_branch(ref->name));
		item->util = xstrdup(ref->name);
	}
	free_refs(stale_refs);
	free_refs(fetch_map);

	string_list_क्रमय(&अवस्थाः->new_refs);
	string_list_क्रमय(&अवस्थाः->skipped);
	string_list_क्रमय(&अवस्थाः->tracked);
	string_list_क्रमय(&अवस्थाः->stale);

	प्रत्यागम 0;
}

संरचना प्रक्षिप_info {
	वर्ण *dest;
	पूर्णाङ्क forced;
	गणना {
		प्रक्षिप_स्थितिः_CREATE = 0,
		प्रक्षिप_स्थितिः_DELETE,
		प्रक्षिप_स्थितिः_UPTODATE,
		प्रक्षिप_स्थितिः_FASTFORWARD,
		प्रक्षिप_स्थितिः_OUTOFDATE,
		प्रक्षिप_स्थितिः_NOTQUERIED
	} स्थितिः;
};

स्थिर पूर्णाङ्क get_प्रक्षिप_ref_अवस्थाः(ध्रुव संरचना ref *remote_refs,
	संरचना ref_अवस्थाः *अवस्थाः)
{
	संरचना remote *remote = अवस्थाः->remote;
	संरचना ref *ref, *local_refs, *प्रक्षिप_map;
	यदि (remote->mirror)
		प्रत्यागम 0;

	local_refs = get_local_heads();
	प्रक्षिप_map = copy_ref_list(remote_refs);

	मेलनम्_प्रक्षिप_refs(local_refs, &प्रक्षिप_map, &remote->प्रक्षिप, मेलनम्_REFS_NONE);

	प्रति (ref = प्रक्षिप_map; ref; ref = ref->अग्र) {
		संरचना string_list_item *item;
		संरचना प्रक्षिप_info *info;

		यदि (!ref->peer_ref)
			अनुवर्त;
		oidcpy(&ref->new_oid, &ref->peer_ref->new_oid);

		item = string_list_append(&अवस्थाः->प्रक्षिप,
					  abbrev_branch(ref->peer_ref->name));
		item->util = xcalloc(1, परिमाण(संरचना प्रक्षिप_info));
		info = item->util;
		info->forced = ref->force;
		info->dest = xstrdup(abbrev_branch(ref->name));

		यदि (is_null_oid(&ref->new_oid)) {
			info->स्थितिः = प्रक्षिप_स्थितिः_DELETE;
		} अन्यथा यदि (oideq(&ref->old_oid, &ref->new_oid))
			info->स्थितिः = प्रक्षिप_स्थितिः_UPTODATE;
		अन्यथा यदि (is_null_oid(&ref->old_oid))
			info->स्थितिः = प्रक्षिप_स्थितिः_CREATE;
		अन्यथा यदि (odb_has_object(the_repository->objects, &ref->old_oid,
					ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR) &&
			 ref_newer(&ref->new_oid, &ref->old_oid))
			info->स्थितिः = प्रक्षिप_स्थितिः_FASTFORWARD;
		अन्यथा
			info->स्थितिः = प्रक्षिप_स्थितिः_OUTOFDATE;
	}
	free_refs(local_refs);
	free_refs(प्रक्षिप_map);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क get_प्रक्षिप_ref_अवस्थाः_noquery(संरचना ref_अवस्थाः *अवस्थाः)
{
	पूर्णाङ्क i;
	संरचना remote *remote = अवस्थाः->remote;
	संरचना string_list_item *item;
	संरचना प्रक्षिप_info *info;

	यदि (remote->mirror)
		प्रत्यागम 0;

	यदि (!remote->प्रक्षिप.nr) {
		item = string_list_append(&अवस्थाः->प्रक्षिप, _("(matching)"));
		info = item->util = xcalloc(1, परिमाण(संरचना प्रक्षिप_info));
		info->स्थितिः = प्रक्षिप_स्थितिः_NOTQUERIED;
		info->dest = xstrdup(item->string);
	}
	प्रति (i = 0; i < remote->प्रक्षिप.nr; i++) {
		ध्रुव संरचना refspec_item *spec = &remote->प्रक्षिप.वस्तूनि[i];
		यदि (spec->matching)
			item = string_list_append(&अवस्थाः->प्रक्षिप, _("(matching)"));
		अन्यथा यदि (वाक्यदैर्घ्यम्(spec->src))
			item = string_list_append(&अवस्थाः->प्रक्षिप, spec->src);
		अन्यथा
			item = string_list_append(&अवस्थाः->प्रक्षिप, _("(delete)"));

		info = item->util = xcalloc(1, परिमाण(संरचना प्रक्षिप_info));
		info->forced = spec->force;
		info->स्थितिः = प्रक्षिप_स्थितिः_NOTQUERIED;
		info->dest = xstrdup(spec->dst ? spec->dst : item->string);
	}
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क get_शीर्ष_नामानि(ध्रुव संरचना ref *remote_refs, संरचना ref_अवस्थाः *अवस्थाः)
{
	संरचना ref *ref, *matches;
	संरचना ref *fetch_map = रिक्त, **fetch_map_पुच्छ = &fetch_map;
	संरचना refspec_item refspec = {
		.force = 0,
		.आदर्शः = 1,
		.src = (वर्ण *) "refs/heads/*",
		.dst = (वर्ण *) "refs/heads/*",
	};

	get_fetch_map(remote_refs, &refspec, &fetch_map_पुच्छ, 0);
	matches = guess_remote_शीर्ष(find_ref_by_name(remote_refs, "HEAD"),
				    fetch_map, REMOTE_GUESS_शीर्ष_ALL);
	प्रति (ref = matches; ref; ref = ref->अग्र)
		string_list_append(&अवस्थाः->heads, abbrev_branch(ref->name));

	free_refs(fetch_map);
	free_refs(matches);
	प्रत्यागम 0;
}

संरचना ज्ञातम्_remote {
	संरचना ज्ञातम्_remote *अग्र;
	संरचना remote *remote;
};

संरचना ज्ञातम्_remotes {
	संरचना remote *गन्तव्यम्_delete;
	संरचना ज्ञातम्_remote *list;
};

स्थिर पूर्णाङ्क सङ्कलय_ज्ञातम्_remote(संरचना remote *remote, शून्य *cb_दत्त)
{
	संरचना ज्ञातम्_remotes *all = cb_दत्त;
	संरचना ज्ञातम्_remote *r;

	यदि (!वाक्यं_तुलय(all->गन्तव्यम्_delete->name, remote->name))
		प्रत्यागम 0;

	r = xmalloc(परिमाण(*r));
	r->remote = remote;
	r->अग्र = all->list;
	all->list = r;
	प्रत्यागम 0;
}

संरचना branches_for_remote {
	संरचना remote *remote;
	संरचना string_list *branches, *skipped;
	संरचना ज्ञातम्_remotes *keep;
};

स्थिर पूर्णाङ्क सङ्कलय_branch_for_removal(ध्रुव संरचना reference *ref, शून्य *cb_दत्त)
{
	संरचना branches_for_remote *branches = cb_दत्त;
	संरचना refspec_item refspec;
	संरचना ज्ञातम्_remote *kr;

	स्मृतिं_पूरय(&refspec, 0, परिमाण(refspec));
	refspec.dst = (वर्ण *)ref->name;
	यदि (remote_find_tracking(branches->remote, &refspec))
		प्रत्यागम 0;
	मुक्त(refspec.src);

	/* don't delete a branch if another remote also uses it */
	प्रति (kr = branches->keep->list; kr; kr = kr->अग्र) {
		स्मृतिं_पूरय(&refspec, 0, परिमाण(refspec));
		refspec.dst = (वर्ण *)ref->name;
		यदि (!remote_find_tracking(kr->remote, &refspec)) {
			मुक्त(refspec.src);
			प्रत्यागम 0;
		}
	}

	/* don't delete non-remote-tracking refs */
	यदि (!starts_with(ref->name, "refs/remotes/")) {
		/* advise user how to delete local branches */
		यदि (starts_with(ref->name, "refs/heads/"))
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
	संरचना प्रगतिः *प्रगतिः;
	संरचना strbuf *err;
	uint32_t प्रगतिः_nr;
	uint64_t सूचकांक;
};

स्थिर शून्य compute_renamed_ref(संरचना rename_info *rename,
				ध्रुव वर्ण *refname,
				संरचना strbuf *बहिर्गमनम्)
{
	strbuf_reset(बहिर्गमनम्);
	strbuf_addstr(बहिर्गमनम्, refname);
	strbuf_splice(बहिर्गमनम्, वाक्यदैर्घ्यम्("refs/remotes/"), वाक्यदैर्घ्यम्(rename->old_name),
		      rename->new_name, वाक्यदैर्घ्यम्(rename->new_name));
}

स्थिर पूर्णाङ्क rename_one_reflog_प्रविष्टिः(ध्रुव वर्ण *old_refname,
				   संरचना object_id *old_oid,
				   संरचना object_id *new_oid,
				   ध्रुव वर्ण *committer,
				   timestamp_t timestamp, पूर्णाङ्क tz,
				   ध्रुव वर्ण *सन्देशः, शून्य *cb_दत्त)
{
	संरचना rename_info *rename = cb_दत्त;
	संरचना strbuf new_refname = STRBUF_INIT;
	संरचना strbuf तटस्थः = STRBUF_INIT;
	संरचना strbuf name = STRBUF_INIT;
	संरचना strbuf mail = STRBUF_INIT;
	संरचना ident_विभज ident;
	ध्रुव वर्ण *date;
	पूर्णाङ्क error;

	compute_renamed_ref(rename, old_refname, &new_refname);

	यदि (विभज_ident_पङ्क्तिः(&ident, committer, वाक्यदैर्घ्यम्(committer)) < 0) {
		error = -1;
		गच्छ बहिर्गमनम्;
	}

	strbuf_सङ्कलय(&name, ident.name_begin, ident.name_अन्तः - ident.name_begin);
	strbuf_सङ्कलय(&mail, ident.mail_begin, ident.mail_अन्तः - ident.mail_begin);

	date = show_date(timestamp, tz, DATE_बहुलकम्(प्रसामान्यम्));
	strbuf_addstr(&तटस्थः, fmt_ident(name.buf, mail.buf,
					  WANT_BLANK_IDENT, date, 0));

	error = ref_transaction_update_reflog(rename->transaction, new_refname.buf,
					      new_oid, old_oid, तटस्थः.buf, सन्देशः,
					      rename->सूचकांक++, rename->err);

बहिर्गमनम्:
	strbuf_release(&new_refname);
	strbuf_release(&तटस्थः);
	strbuf_release(&name);
	strbuf_release(&mail);
	प्रत्यागम error;
}

स्थिर पूर्णाङ्क rename_one_reflog(ध्रुव वर्ण *old_refname,
			     ध्रुव संरचना object_id *old_oid,
			     संरचना rename_info *rename)
{
	संरचना strbuf new_refname = STRBUF_INIT;
	संरचना strbuf सन्देशः_2 = STRBUF_INIT;
	पूर्णाङ्क error;

	यदि (!refs_reflog_exists(get_main_ref_store(the_repository), old_refname))
		प्रत्यागम 0;

	error = refs_for_each_reflog_ent(get_main_ref_store(the_repository),
					 old_refname, rename_one_reflog_प्रविष्टिः, rename);
	यदि (error < 0)
		गच्छ बहिर्गमनम्;

	compute_renamed_ref(rename, old_refname, &new_refname);

	/*
	 * Manually write the reflog entry for the now-renamed ref. We cannot
	 * rely on `rename_one_ref()` to do this for us as that would screw
	 * over order in which reflog entries are being written.
	 *
	 * Furthermore, we only append the entry in case the reference
	 * resolves. Missing references shouldn't have reflogs anyway.
	 */
	strbuf_addf(&सन्देशः_2, "remote: renamed %s to %s", old_refname,
		    new_refname.buf);

	error = ref_transaction_update_reflog(rename->transaction, new_refname.buf,
					      old_oid, old_oid, git_committer_info(0),
					      सन्देशः_2.buf, rename->सूचकांक++, rename->err);
	यदि (error < 0)
		प्रत्यागम error;

बहिर्गमनम्:
	strbuf_release(&new_refname);
	strbuf_release(&सन्देशः_2);
	प्रत्यागम error;
}

स्थिर पूर्णाङ्क rename_one_ref(ध्रुव संरचना reference *ref, शून्य *cb_दत्त)
{
	संरचना strbuf new_referent = STRBUF_INIT;
	संरचना strbuf new_refname = STRBUF_INIT;
	संरचना rename_info *rename = cb_दत्त;
	ध्रुव संरचना object_id *oid = ref->oid;
	ध्रुव वर्ण *referent = ref->target;
	पूर्णाङ्क error;

	compute_renamed_ref(rename, ref->name, &new_refname);

	यदि (ref->flags & REF_ISSYMREF) {
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
	यदि (error < 0)
		गच्छ बहिर्गमनम्;

	error = ref_transaction_update(rename->transaction, new_refname.buf, oid, null_oid(the_hash_algo),
				       (ref->flags & REF_ISSYMREF) ? new_referent.buf : रिक्त, रिक्त,
				       REF_SKIP_CREATE_REFLOG | REF_NO_DEREF | REF_SKIP_OID_VERIFICATION,
				       रिक्त, rename->err);
	यदि (error < 0)
		गच्छ बहिर्गमनम्;

	error = rename_one_reflog(ref->name, oid, rename);
	यदि (error < 0)
		गच्छ बहिर्गमनम्;

	display_प्रगतिः(rename->प्रगतिः, ++rename->प्रगतिः_nr);

बहिर्गमनम्:
	strbuf_release(&new_referent);
	strbuf_release(&new_refname);
	प्रत्यागम error;
}

स्थिर पूर्णाङ्क migrate_file(संरचना remote *remote)
{
	संरचना strbuf buf = STRBUF_INIT;

	strbuf_addf(&buf, "remote.%s.url", remote->name);
	प्रति (आकार_t i = 0; i < remote->url.nr; i++)
		repo_config_समुच्चयः_multivar(the_repository, buf.buf, remote->url.v[i], "^$", 0);
	strbuf_reset(&buf);
	strbuf_addf(&buf, "remote.%s.push", remote->name);
	प्रति (पूर्णाङ्क i = 0; i < remote->प्रक्षिप.nr; i++)
		repo_config_समुच्चयः_multivar(the_repository, buf.buf, remote->प्रक्षिप.वस्तूनि[i].raw, "^$", 0);
	strbuf_reset(&buf);
	strbuf_addf(&buf, "remote.%s.fetch", remote->name);
	प्रति (पूर्णाङ्क i = 0; i < remote->fetch.nr; i++)
		repo_config_समुच्चयः_multivar(the_repository, buf.buf, remote->fetch.वस्तूनि[i].raw, "^$", 0);
#यदि_अपरिभाषितम् WITH_BREAKING_CHANGES
	यदि (remote->origin == REMOTE_REMOTES)
		unlink_or_warn(repo_git_मार्गः_replace(the_repository, &buf,
						     "remotes/%s", remote->name));
	अन्यथा यदि (remote->origin == REMOTE_BRANCHES)
		unlink_or_warn(repo_git_मार्गः_replace(the_repository, &buf,
						     "branches/%s", remote->name));
#यदि_अन्तः /* WITH_BREAKING_CHANGES */
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
	यदि (वाक्यं_तुलय(कुञ्चिका, "remote.pushdefault") ||
	    !मानम् || वाक्यं_तुलय(मानम्, info->old_name))
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
	यदि (प्रक्षिप_default.scope >= CONFIG_SCOPE_COMMAND)
		; /* pass */
	अन्यथा यदि (प्रक्षिप_default.scope >= CONFIG_SCOPE_LOCAL) {
		पूर्णाङ्क परिणाम = repo_config_समुच्चयः_gently(the_repository, "remote.pushDefault",
						    new_name);
		यदि (new_name && परिणाम && परिणाम != CONFIG_NOTHING_समुच्चयः)
			die(_("could not set '%s'"), "remote.pushDefault");
		अन्यथा यदि (!new_name && परिणाम && परिणाम != CONFIG_NOTHING_समुच्चयः)
			die(_("could not unset '%s'"), "remote.pushDefault");
	} अन्यथा यदि (प्रक्षिप_default.scope >= CONFIG_SCOPE_SYSTEM) {
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

स्थिर पूर्णाङ्क mv(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
	      संरचना repository *repo अप्रयुक्तम्)
{
	पूर्णाङ्क show_प्रगतिः = isatty(2);
	संरचना option options[] = {
		OPT_BOOL(0, "progress", &show_प्रगतिः, N_("force progress reporting")),
		OPT_अन्तः()
	};
	संरचना remote *oldremote, *newremote;
	संरचना strbuf buf = STRBUF_INIT, buf2 = STRBUF_INIT, buf3 = STRBUF_INIT,
		old_remote_context = STRBUF_INIT, err = STRBUF_INIT;
	संरचना rename_info rename = {
		.err = &err,
	};
	पूर्णाङ्क refspecs_need_update = 0;
	पूर्णाङ्क परिणाम = 0;

	argc = parse_options(argc, argv, उपसर्गः, options,
			     builtin_remote_rename_usage, 0);

	यदि (argc != 2)
		usage_with_options(builtin_remote_rename_usage, options);

	rename.old_name = argv[0];
	rename.new_name = argv[1];

	oldremote = remote_get(rename.old_name);
	यदि (!remote_is_configured(oldremote, 1)) {
		error(_("No such remote: '%s'"), rename.old_name);
		निर्गच्छ(2);
	}

	यदि (!वाक्यं_तुलय(rename.old_name, rename.new_name) && oldremote->origin != REMOTE_CONFIG)
		प्रत्यागम migrate_file(oldremote);

	newremote = remote_get(rename.new_name);
	यदि (remote_is_configured(newremote, 1)) {
		error(_("remote %s already exists."), rename.new_name);
		निर्गच्छ(3);
	}

	यदि (!valid_remote_name(rename.new_name))
		die(_("'%s' is not a valid remote name"), rename.new_name);

	strbuf_addf(&buf, "remote.%s", rename.old_name);
	strbuf_addf(&buf2, "remote.%s", rename.new_name);
	यदि (repo_config_rename_section(the_repository, buf.buf, buf2.buf) < 1) {
		परिणाम = error(_("Could not rename config section '%s' to '%s'"),
			       buf.buf, buf2.buf);
		गच्छ बहिर्गमनम्;
	}

	strbuf_addf(&old_remote_context, ":refs/remotes/%s/", rename.old_name);

	प्रति (पूर्णाङ्क i = 0; i < oldremote->fetch.nr && !refspecs_need_update; i++)
		refspecs_need_update = !!वाक्ये_अन्विष(oldremote->fetch.वस्तूनि[i].raw,
						old_remote_context.buf);

	यदि (refspecs_need_update) {
		संरचना refs_for_each_ref_options opts = {
			.flags = REFS_FOR_EACH_INCLUDE_BROKEN,
		};
		rename.transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
							       0, &err);
		यदि (!rename.transaction)
			गच्छ बहिर्गमनम्;

		यदि (show_प्रगतिः)
			rename.प्रगतिः = start_delayed_प्रगतिः(the_repository,
								 _("Renaming remote references"), 0);

		strbuf_reset(&buf);
		strbuf_addf(&buf, "refs/remotes/%s/", rename.old_name);
		opts.उपसर्गः = buf.buf;

		परिणाम = refs_for_each_ref_ext(get_main_ref_store(the_repository),
					       rename_one_ref, &rename, &opts);
		यदि (परिणाम < 0)
			die(_("queueing remote ref renames failed: %s"), rename.err->buf);

		परिणाम = ref_transaction_prepare(rename.transaction, &err);
		यदि (परिणाम < 0) {
			error("renaming remote references failed: %s", err.buf);
			यदि (परिणाम == REF_TRANSACTION_ERROR_NAME_CONFLICT)
				advise(conflicting_remote_refs_advice);
			die(रिक्त);
		}
	}

	यदि (oldremote->fetch.nr) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.fetch", rename.new_name);
		repo_config_समुच्चयः_multivar(the_repository, buf.buf, रिक्त, रिक्त, CONFIG_FLAGS_MULTI_REPLACE);
		प्रति (पूर्णाङ्क i = 0; i < oldremote->fetch.nr; i++) {
			वर्ण *ptr;

			strbuf_reset(&buf2);
			strbuf_addstr(&buf2, oldremote->fetch.वस्तूनि[i].raw);
			ptr = वाक्ये_अन्विष(buf2.buf, old_remote_context.buf);
			यदि (ptr) {
				strbuf_splice(&buf2,
					      ptr-buf2.buf + वाक्यदैर्घ्यम्(":refs/remotes/"),
					      वाक्यदैर्घ्यम्(rename.old_name), rename.new_name,
					      वाक्यदैर्घ्यम्(rename.new_name));
			} अन्यथा
				warning(_("Not updating non-default fetch refspec\n"
					  "\t%s\n"
					  "\tPlease update the configuration manually if necessary."),
					buf2.buf);

			repo_config_समुच्चयः_multivar(the_repository, buf.buf, buf2.buf, "^$", 0);
		}
	}

	read_branches();
	प्रति (आकार_t i = 0; i < branch_list.nr; i++) {
		संरचना string_list_item *item = branch_list.वस्तूनि + i;
		संरचना branch_info *info = item->util;
		यदि (info->remote_name && !वाक्यं_तुलय(info->remote_name, rename.old_name)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "branch.%s.remote", item->string);
			repo_config_समुच्चयः(the_repository, buf.buf, rename.new_name);
		}
		यदि (info->प्रक्षिप_remote_name && !वाक्यं_तुलय(info->प्रक्षिप_remote_name, rename.old_name)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "branch.%s.pushRemote", item->string);
			repo_config_समुच्चयः(the_repository, buf.buf, rename.new_name);
		}
	}

	यदि (refspecs_need_update) {
		परिणाम = ref_transaction_commit(rename.transaction, &err);
		यदि (परिणाम < 0)
			die(_("renaming remote refs failed: %s"), rename.err->buf);

		stop_प्रगतिः(&rename.प्रगतिः);

		handle_प्रक्षिप_default(rename.old_name, rename.new_name);
	}

बहिर्गमनम्:
	ref_transaction_free(rename.transaction);
	strbuf_release(&old_remote_context);
	strbuf_release(&buf);
	strbuf_release(&buf2);
	strbuf_release(&buf3);
	strbuf_release(&err);
	प्रत्यागम परिणाम;
}

स्थिर पूर्णाङ्क rm(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
	      संरचना repository *repo अप्रयुक्तम्)
{
	संरचना option options[] = {
		OPT_अन्तः()
	};
	संरचना remote *remote;
	संरचना strbuf buf = STRBUF_INIT;
	संरचना ज्ञातम्_remotes ज्ञातम्_remotes = { रिक्त, रिक्त };
	संरचना string_list branches = STRING_LIST_INIT_DUP;
	संरचना string_list skipped = STRING_LIST_INIT_DUP;
	संरचना branches_for_remote cb_दत्त;
	पूर्णाङ्क परिणाम;

	स्मृतिं_पूरय(&cb_दत्त, 0, परिमाण(cb_दत्त));
	cb_दत्त.branches = &branches;
	cb_दत्त.skipped = &skipped;
	cb_दत्त.keep = &ज्ञातम्_remotes;

	argc = parse_options(argc, argv, उपसर्गः, options,
			     builtin_remote_rm_usage, 0);
	यदि (argc != 1)
		usage_with_options(builtin_remote_rm_usage, options);

	remote = remote_get(argv[0]);
	यदि (!remote_is_configured(remote, 1)) {
		error(_("No such remote: '%s'"), argv[0]);
		निर्गच्छ(2);
	}

	ज्ञातम्_remotes.गन्तव्यम्_delete = remote;
	for_each_remote(सङ्कलय_ज्ञातम्_remote, &ज्ञातम्_remotes);

	read_branches();
	प्रति (आकार_t i = 0; i < branch_list.nr; i++) {
		संरचना string_list_item *item = branch_list.वस्तूनि + i;
		संरचना branch_info *info = item->util;
		यदि (info->remote_name && !वाक्यं_तुलय(info->remote_name, remote->name)) {
			ध्रुव वर्ण *कुञ्चिकाः[] = { "remote", "merge", रिक्त }, **k;
			प्रति (k = कुञ्चिकाः; *k; k++) {
				strbuf_reset(&buf);
				strbuf_addf(&buf, "branch.%s.%s",
						item->string, *k);
				परिणाम = repo_config_समुच्चयः_gently(the_repository, buf.buf, रिक्त);
				यदि (परिणाम && परिणाम != CONFIG_NOTHING_समुच्चयः)
					die(_("could not unset '%s'"), buf.buf);
			}
		}
		यदि (info->प्रक्षिप_remote_name && !वाक्यं_तुलय(info->प्रक्षिप_remote_name, remote->name)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "branch.%s.pushremote", item->string);
			परिणाम = repo_config_समुच्चयः_gently(the_repository, buf.buf, रिक्त);
			यदि (परिणाम && परिणाम != CONFIG_NOTHING_समुच्चयः)
				die(_("could not unset '%s'"), buf.buf);
		}
	}

	/*
	 * We cannot just pass a function to for_each_ref() which deletes
	 * the branches one by one, since for_each_ref() relies on cached
	 * refs, which are invalidated when deleting a branch.
	 */
	cb_दत्त.remote = remote;
	परिणाम = refs_for_each_ref(get_main_ref_store(the_repository),
				   सङ्कलय_branch_for_removal, &cb_दत्त);
	strbuf_release(&buf);

	यदि (!परिणाम)
		परिणाम = refs_delete_refs(get_main_ref_store(the_repository),
					  "remote: remove", &branches,
					  REF_NO_DEREF);
	string_list_clear(&branches, 0);

	यदि (skipped.nr) {
		fprintf_ln(stderr,
			   Q_("Note: A branch outside the refs/remotes/ hierarchy was not removed;\n"
			      "to delete it, use:",
			      "Note: Some branches outside the refs/remotes/ hierarchy were not removed;\n"
			      "to delete them, use:",
			      skipped.nr));
		प्रति (आकार_t i = 0; i < skipped.nr; i++)
			fprintf(stderr, "  git branch -d %s\n",
				skipped.वस्तूनि[i].string);
	}
	string_list_clear(&skipped, 0);

	यदि (!परिणाम) {
		strbuf_addf(&buf, "remote.%s", remote->name);
		यदि (repo_config_rename_section(the_repository, buf.buf, रिक्त) < 1) {
			परिणाम = error(_("Could not remove config section '%s'"), buf.buf);
			गच्छ बहिर्गमनम्;
		}

		handle_प्रक्षिप_default(remote->name, रिक्त);
	}

बहिर्गमनम्:
	प्रति (संरचना ज्ञातम्_remote *r = ज्ञातम्_remotes.list; r;) {
		संरचना ज्ञातम्_remote *अग्र = r->अग्र;
		मुक्त(r);
		r = अग्र;
	}
	strbuf_release(&buf);
	प्रत्यागम परिणाम;
}

स्थिर शून्य clear_प्रक्षिप_info(शून्य *util, ध्रुव वर्ण *string अप्रयुक्तम्)
{
	संरचना प्रक्षिप_info *info = util;
	मुक्त(info->dest);
	मुक्त(info);
}

स्थिर शून्य free_remote_ref_अवस्थाः(संरचना ref_अवस्थाः *अवस्थाः)
{
	string_list_clear(&अवस्थाः->new_refs, 0);
	string_list_clear(&अवस्थाः->skipped, 0);
	string_list_clear(&अवस्थाः->stale, 1);
	string_list_clear(&अवस्थाः->tracked, 0);
	string_list_clear(&अवस्थाः->heads, 0);
	string_list_clear_func(&अवस्थाः->प्रक्षिप, clear_प्रक्षिप_info);
}

स्थिर पूर्णाङ्क append_ref_गन्तव्यम्_tracked_list(ध्रुव संरचना reference *ref, शून्य *cb_दत्त)
{
	संरचना ref_अवस्थाः *अवस्थाः = cb_दत्त;
	संरचना refspec_item refspec;

	यदि (ref->flags & REF_ISSYMREF)
		प्रत्यागम 0;

	स्मृतिं_पूरय(&refspec, 0, परिमाण(refspec));
	refspec.dst = (वर्ण *)ref->name;
	यदि (!remote_find_tracking(अवस्थाः->remote, &refspec)) {
		string_list_append(&अवस्थाः->tracked, abbrev_branch(refspec.src));
		मुक्त(refspec.src);
	}

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क get_remote_ref_अवस्थाः(ध्रुव वर्ण *name,
				 संरचना ref_अवस्थाः *अवस्थाः,
				 पूर्णाङ्क query)
{
	अवस्थाः->remote = remote_get(name);
	यदि (!अवस्थाः->remote)
		प्रत्यागम error(_("No such remote: '%s'"), name);

	read_branches();

	यदि (query) {
		संरचना transport *transport;
		ध्रुव संरचना ref *remote_refs;

		transport = transport_get(अवस्थाः->remote, अवस्थाः->remote->url.v[0]);
		remote_refs = transport_get_remote_refs(transport, रिक्त);

		अवस्थाः->queried = 1;
		यदि (query & GET_REF_अवस्थाः)
			get_ref_अवस्थाः(remote_refs, अवस्थाः);
		यदि (query & GET_शीर्ष_नामानि)
			get_शीर्ष_नामानि(remote_refs, अवस्थाः);
		यदि (query & GET_प्रक्षिप_REF_अवस्थाः)
			get_प्रक्षिप_ref_अवस्थाः(remote_refs, अवस्थाः);
		transport_disconnect(transport);
	} अन्यथा {
		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_ref_गन्तव्यम्_tracked_list, अवस्थाः);
		string_list_क्रमय(&अवस्थाः->tracked);
		get_प्रक्षिप_ref_अवस्थाः_noquery(अवस्थाः);
	}

	प्रत्यागम 0;
}

संरचना show_info {
	संरचना string_list list;
	संरचना ref_अवस्थाः अवस्थाः;
	पूर्णाङ्क विस्तारः, width2;
	पूर्णाङ्क any_rebase;
};

#परिभाषय SHOW_INFO_INIT { \
	.list = STRING_LIST_INIT_DUP, \
	.अवस्थाः = REF_अवस्थाः_INIT, \
}

स्थिर पूर्णाङ्क सङ्कलय_remote_गन्तव्यम्_show_info(संरचना string_list_item *item, शून्य *cb_दत्त)
{
	संरचना show_info *info = cb_दत्त;
	पूर्णाङ्क n = वाक्यदैर्घ्यम्(item->string);
	यदि (n > info->विस्तारः)
		info->विस्तारः = n;
	string_list_निवेशय(&info->list, item->string);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क show_remote_info_item(संरचना string_list_item *item, शून्य *cb_दत्त)
{
	संरचना show_info *info = cb_दत्त;
	संरचना ref_अवस्थाः *अवस्थाः = &info->अवस्थाः;
	ध्रुव वर्ण *name = item->string;

	यदि (अवस्थाः->queried) {
		ध्रुव वर्ण *fmt = "%s";
		ध्रुव वर्ण *arg = "";
		यदि (string_list_has_string(&अवस्थाः->new_refs, name)) {
			fmt = _(" new (next fetch will store in remotes/%s)");
			arg = अवस्थाः->remote->name;
		} अन्यथा यदि (string_list_has_string(&अवस्थाः->tracked, name))
			arg = _(" tracked");
		अन्यथा यदि (string_list_has_string(&अवस्थाः->skipped, name))
			arg = _(" skipped");
		अन्यथा यदि (string_list_has_string(&अवस्थाः->stale, name))
			arg = _(" stale (use 'git remote prune' to remove)");
		अन्यथा
			arg = _(" ???");
		प्रारूप_लिख("    %-*s", info->विस्तारः, name);
		प्रारूप_लिख(fmt, arg);
		प्रारूप_लिख("\n");
	} अन्यथा
		प्रारूप_लिख("    %s\n", name);

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क सङ्कलय_local_गन्तव्यम्_show_info(संरचना string_list_item *branch_item, शून्य *cb_दत्त)
{
	संरचना show_info *show_info = cb_दत्त;
	संरचना ref_अवस्थाः *अवस्थाः = &show_info->अवस्थाः;
	संरचना branch_info *branch_info = branch_item->util;
	संरचना string_list_item *item;
	पूर्णाङ्क n;

	यदि (!branch_info->merge.nr || !branch_info->remote_name ||
	    वाक्यं_तुलय(अवस्थाः->remote->name, branch_info->remote_name))
		प्रत्यागम 0;
	यदि ((n = वाक्यदैर्घ्यम्(branch_item->string)) > show_info->विस्तारः)
		show_info->विस्तारः = n;
	यदि (branch_info->rebase >= REBASE_TRUE)
		show_info->any_rebase = 1;

	item = string_list_निवेशय(&show_info->list, branch_item->string);
	item->util = branch_info;

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क show_local_info_item(संरचना string_list_item *item, शून्य *cb_दत्त)
{
	संरचना show_info *show_info = cb_दत्त;
	संरचना branch_info *branch_info = item->util;
	संरचना string_list *merge = &branch_info->merge;
	पूर्णाङ्क विस्तारः = show_info->विस्तारः + 4;

	यदि (branch_info->rebase >= REBASE_TRUE && branch_info->merge.nr > 1) {
		error(_("invalid branch.%s.merge; cannot rebase onto > 1 branch"),
			item->string);
		प्रत्यागम 0;
	}

	प्रारूप_लिख("    %-*s ", show_info->विस्तारः, item->string);
	यदि (branch_info->rebase >= REBASE_TRUE) {
		ध्रुव वर्ण *सन्देशः;
		यदि (branch_info->rebase == REBASE_INTERACTIVE)
			सन्देशः = _("rebases interactively onto remote %s");
		अन्यथा यदि (branch_info->rebase == REBASE_MERGES)
			सन्देशः = _("rebases interactively (with merges) onto "
				"remote %s");
		अन्यथा
			सन्देशः = _("rebases onto remote %s");
		printf_ln(सन्देशः, merge->वस्तूनि[0].string);
		प्रत्यागम 0;
	} अन्यथा यदि (show_info->any_rebase) {
		printf_ln(_(" merges with remote %s"), merge->वस्तूनि[0].string);
		विस्तारः++;
	} अन्यथा {
		printf_ln(_("merges with remote %s"), merge->वस्तूनि[0].string);
	}
	प्रति (आकार_t i = 1; i < merge->nr; i++)
		प्रारूप_लिख(_("%-*s    and with remote %s\n"), विस्तारः, "",
		       merge->वस्तूनि[i].string);

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क सङ्कलय_प्रक्षिप_गन्तव्यम्_show_info(संरचना string_list_item *प्रक्षिप_item, शून्य *cb_दत्त)
{
	संरचना show_info *show_info = cb_दत्त;
	संरचना प्रक्षिप_info *प्रक्षिप_info = प्रक्षिप_item->util;
	संरचना string_list_item *item;
	पूर्णाङ्क n;
	यदि ((n = वाक्यदैर्घ्यम्(प्रक्षिप_item->string)) > show_info->विस्तारः)
		show_info->विस्तारः = n;
	यदि ((n = वाक्यदैर्घ्यम्(प्रक्षिप_info->dest)) > show_info->width2)
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
	पूर्णाङ्क cmp = वाक्यं_तुलय(a->string, b->string);
	प्रत्यागम cmp ? cmp : वाक्यं_तुलय(a_प्रक्षिप->dest, b_प्रक्षिप->dest);
}

स्थिर पूर्णाङ्क show_प्रक्षिप_info_item(संरचना string_list_item *item, शून्य *cb_दत्त)
{
	संरचना show_info *show_info = cb_दत्त;
	संरचना प्रक्षिप_info *प्रक्षिप_info = item->util;
	ध्रुव वर्ण *src = item->string, *स्थितिः = रिक्त;

	विकल्प (प्रक्षिप_info->स्थितिः) {
	पक्ष प्रक्षिप_स्थितिः_CREATE:
		स्थितिः = _("create");
		भङ्ग;
	पक्ष प्रक्षिप_स्थितिः_DELETE:
		स्थितिः = _("delete");
		src = _("(none)");
		भङ्ग;
	पक्ष प्रक्षिप_स्थितिः_UPTODATE:
		स्थितिः = _("up to date");
		भङ्ग;
	पक्ष प्रक्षिप_स्थितिः_FASTFORWARD:
		स्थितिः = _("fast-forwardable");
		भङ्ग;
	पक्ष प्रक्षिप_स्थितिः_OUTOFDATE:
		स्थितिः = _("local out of date");
		भङ्ग;
	पक्ष प्रक्षिप_स्थितिः_NOTQUERIED:
		भङ्ग;
	}
	यदि (स्थितिः) {
		यदि (प्रक्षिप_info->forced)
			printf_ln(_("    %-*s forces to %-*s (%s)"), show_info->विस्तारः, src,
			       show_info->width2, प्रक्षिप_info->dest, स्थितिः);
		अन्यथा
			printf_ln(_("    %-*s pushes to %-*s (%s)"), show_info->विस्तारः, src,
			       show_info->width2, प्रक्षिप_info->dest, स्थितिः);
	} अन्यथा {
		यदि (प्रक्षिप_info->forced)
			printf_ln(_("    %-*s forces to %s"), show_info->विस्तारः, src,
			       प्रक्षिप_info->dest);
		अन्यथा
			printf_ln(_("    %-*s pushes to %s"), show_info->विस्तारः, src,
			       प्रक्षिप_info->dest);
	}
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क get_one_प्रविष्टिः(संरचना remote *remote, शून्य *priv)
{
	संरचना string_list *list = priv;
	संरचना strbuf remote_info_buf = STRBUF_INIT;
	संरचना strvec *url;

	यदि (remote->url.nr > 0) {
		संरचना strbuf promisor_config = STRBUF_INIT;
		ध्रुव वर्ण *partial_clone_filter = रिक्त;

		strbuf_addf(&promisor_config, "remote.%s.partialclonefilter", remote->name);
		strbuf_addf(&remote_info_buf, "%s (fetch)", remote->url.v[0]);
		यदि (!repo_config_get_string_tmp(the_repository, promisor_config.buf, &partial_clone_filter))
			strbuf_addf(&remote_info_buf, " [%s]", partial_clone_filter);

		strbuf_release(&promisor_config);
		string_list_append(list, remote->name)->util =
				strbuf_detach(&remote_info_buf, रिक्त);
	} अन्यथा
		string_list_append(list, remote->name)->util = रिक्त;
	url = प्रक्षिप_url_सम्बन्धि_remote(remote);
	प्रति (आकार_t i = 0; i < url->nr; i++) {
		strbuf_addf(&remote_info_buf, "%s (push)", url->v[i]);
		string_list_append(list, remote->name)->util =
				strbuf_detach(&remote_info_buf, रिक्त);
	}

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क show_all(शून्य)
{
	संरचना string_list list = STRING_LIST_INIT_DUP;
	पूर्णाङ्क परिणाम;

	परिणाम = for_each_remote(get_one_प्रविष्टिः, &list);

	यदि (!परिणाम) {
		string_list_क्रमय(&list);
		प्रति (आकार_t i = 0; i < list.nr; i++) {
			संरचना string_list_item *item = list.वस्तूनि + i;
			यदि (verbose)
				प्रारूप_लिख("%s\t%s\n", item->string,
					item->util ? (ध्रुव वर्ण *)item->util : "");
			अन्यथा {
				यदि (i && !वाक्यं_तुलय((item - 1)->string, item->string))
					अनुवर्त;
				प्रारूप_लिख("%s\n", item->string);
			}
		}
	}
	string_list_clear(&list, 1);
	प्रत्यागम परिणाम;
}

स्थिर पूर्णाङ्क show(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
		संरचना repository *repo अप्रयुक्तम्)
{
	पूर्णाङ्क no_query = 0, परिणाम = 0, query_flag = 0;
	संरचना option options[] = {
		OPT_BOOL('n', रिक्त, &no_query, N_("do not query remotes")),
		OPT_अन्तः()
	};
	संरचना show_info info = SHOW_INFO_INIT;

	argc = parse_options(argc, argv, उपसर्गः, options,
			     builtin_remote_show_usage,
			     0);

	यदि (argc < 1)
		प्रत्यागम show_all();

	यदि (!no_query)
		query_flag = (GET_REF_अवस्थाः | GET_शीर्ष_नामानि | GET_प्रक्षिप_REF_अवस्थाः);

	प्रति (; argc; argc--, argv++) {
		आकार_t i;
		संरचना strvec *url;

		get_remote_ref_अवस्थाः(*argv, &info.अवस्थाः, query_flag);

		printf_ln(_("* remote %s"), *argv);
		printf_ln(_("  Fetch URL: %s"), info.अवस्थाः.remote->url.v[0]);
		url = प्रक्षिप_url_सम्बन्धि_remote(info.अवस्थाः.remote);
		प्रति (i = 0; i < url->nr; i++)
			/*
			 * TRANSLATORS: the colon ':' should align
			 * with the one in " Fetch URL: %s"
			 * translation.
			 */
			printf_ln(_("  Push  URL: %s"), url->v[i]);
		यदि (!i)
			printf_ln(_("  Push  URL: %s"), _("(no URL)"));
		यदि (no_query)
			printf_ln(_("  HEAD branch: %s"), _("(not queried)"));
		अन्यथा यदि (!info.अवस्थाः.heads.nr)
			printf_ln(_("  HEAD branch: %s"), _("(unknown)"));
		अन्यथा यदि (info.अवस्थाः.heads.nr == 1)
			printf_ln(_("  HEAD branch: %s"), info.अवस्थाः.heads.वस्तूनि[0].string);
		अन्यथा {
			प्रारूप_लिख(_("  HEAD branch (remote HEAD is ambiguous,"
				 " may be one of the following):\n"));
			प्रति (i = 0; i < info.अवस्थाः.heads.nr; i++)
				प्रारूप_लिख("    %s\n", info.अवस्थाः.heads.वस्तूनि[i].string);
		}

		/* remote branch info */
		info.विस्तारः = 0;
		for_each_string_list(&info.अवस्थाः.new_refs, सङ्कलय_remote_गन्तव्यम्_show_info, &info);
		for_each_string_list(&info.अवस्थाः.skipped, सङ्कलय_remote_गन्तव्यम्_show_info, &info);
		for_each_string_list(&info.अवस्थाः.tracked, सङ्कलय_remote_गन्तव्यम्_show_info, &info);
		for_each_string_list(&info.अवस्थाः.stale, सङ्कलय_remote_गन्तव्यम्_show_info, &info);
		यदि (info.list.nr)
			printf_ln(Q_("  Remote branch:%s",
				     "  Remote branches:%s",
				     info.list.nr),
				  no_query ? _(" (status not queried)") : "");
		for_each_string_list(&info.list, show_remote_info_item, &info);
		string_list_clear(&info.list, 0);

		/* git pull info */
		info.विस्तारः = 0;
		info.any_rebase = 0;
		for_each_string_list(&branch_list, सङ्कलय_local_गन्तव्यम्_show_info, &info);
		यदि (info.list.nr)
			printf_ln(Q_("  Local branch configured for 'git pull':",
				     "  Local branches configured for 'git pull':",
				     info.list.nr));
		for_each_string_list(&info.list, show_local_info_item, &info);
		string_list_clear(&info.list, 0);

		/* git push info */
		यदि (info.अवस्थाः.remote->mirror)
			printf_ln(_("  Local refs will be mirrored by 'git push'"));

		info.विस्तारः = info.width2 = 0;
		for_each_string_list(&info.अवस्थाः.प्रक्षिप, सङ्कलय_प्रक्षिप_गन्तव्यम्_show_info, &info);
		QSORT(info.list.वस्तूनि, info.list.nr, cmp_string_with_प्रक्षिप);
		यदि (info.list.nr)
			printf_ln(Q_("  Local ref configured for 'git push'%s:",
				     "  Local refs configured for 'git push'%s:",
				     info.list.nr),
				  no_query ? _(" (status not queried)") : "");
		for_each_string_list(&info.list, show_प्रक्षिप_info_item, &info);
		string_list_clear(&info.list, 0);

		free_remote_ref_अवस्थाः(&info.अवस्थाः);
	}

	प्रत्यागम परिणाम;
}

स्थिर शून्य report_समुच्चयः_शीर्ष_auto(ध्रुव वर्ण *remote, ध्रुव वर्ण *शीर्ष_name,
			संरचना strbuf *b_local_शीर्ष, पूर्णाङ्क was_detached) {
	संरचना strbuf buf_उपसर्गः = STRBUF_INIT;
	ध्रुव वर्ण *पूर्व_शीर्ष = रिक्त;

	strbuf_addf(&buf_उपसर्गः, "refs/remotes/%s/", remote);
	skip_उपसर्गः(b_local_शीर्ष->buf, buf_उपसर्गः.buf, &पूर्व_शीर्ष);

	यदि (पूर्व_शीर्ष && !वाक्यं_तुलय(पूर्व_शीर्ष, शीर्ष_name))
		प्रारूप_लिख(_("'%s/HEAD' is unchanged and points to '%s'\n"),
			remote, शीर्ष_name);
	अन्यथा यदि (पूर्व_शीर्ष)
		प्रारूप_लिख(_("'%s/HEAD' has changed from '%s' and now points to '%s'\n"),
			remote, पूर्व_शीर्ष, शीर्ष_name);
	अन्यथा यदि (!b_local_शीर्ष->len)
		प्रारूप_लिख(_("'%s/HEAD' is now created and points to '%s'\n"),
			remote, शीर्ष_name);
	अन्यथा यदि (was_detached && b_local_शीर्ष->len)
		प्रारूप_लिख(_("'%s/HEAD' was detached at '%s' and now points to '%s'\n"),
			remote, b_local_शीर्ष->buf, शीर्ष_name);
	अन्यथा
		प्रारूप_लिख(_("'%s/HEAD' used to point to '%s' "
			"(which is not a remote branch), but now points to '%s'\n"),
			remote, b_local_शीर्ष->buf, शीर्ष_name);
	strbuf_release(&buf_उपसर्गः);
}

स्थिर पूर्णाङ्क समुच्चयः_शीर्ष(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
		    संरचना repository *repo अप्रयुक्तम्)
{
	पूर्णाङ्क opt_a = 0, opt_d = 0, परिणाम = 0, was_detached;
	संरचना strbuf b_शीर्ष = STRBUF_INIT, b_remote_शीर्ष = STRBUF_INIT,
		b_local_शीर्ष = STRBUF_INIT;
	वर्ण *शीर्ष_name = रिक्त;
	संरचना ref_store *refs = get_main_ref_store(the_repository);
	संरचना remote *remote;

	संरचना option options[] = {
		OPT_BOOL('a', "auto", &opt_a,
			 N_("set refs/remotes/<name>/HEAD according to remote")),
		OPT_BOOL('d', "delete", &opt_d,
			 N_("delete refs/remotes/<name>/HEAD")),
		OPT_अन्तः()
	};
	argc = parse_options(argc, argv, उपसर्गः, options,
			     builtin_remote_sethead_usage, 0);

	/* All modes require at least a remote name. */
	यदि (!argc)
		usage_with_options(builtin_remote_sethead_usage, options);

	strbuf_addf(&b_शीर्ष, "refs/remotes/%s/HEAD", argv[0]);
	remote = remote_get(argv[0]);

	यदि (!opt_a && !opt_d && argc == 2) {
		शीर्ष_name = xstrdup(argv[1]);
	} अन्यथा यदि (opt_a && !opt_d && argc == 1) {
		संरचना ref_अवस्थाः अवस्थाः = REF_अवस्थाः_INIT;
		get_remote_ref_अवस्थाः(argv[0], &अवस्थाः, GET_शीर्ष_नामानि);
		यदि (!अवस्थाः.heads.nr)
			परिणाम |= error(_("Cannot determine remote HEAD"));
		अन्यथा यदि (अवस्थाः.heads.nr > 1) {
			परिणाम |= error(_("Multiple remote HEAD branches. "
					  "Please choose one explicitly with:"));
			प्रति (आकार_t i = 0; i < अवस्थाः.heads.nr; i++)
				fprintf(stderr, "  git remote set-head %s %s\n",
					argv[0], अवस्थाः.heads.वस्तूनि[i].string);
		} अन्यथा
			शीर्ष_name = xstrdup(अवस्थाः.heads.वस्तूनि[0].string);
		free_remote_ref_अवस्थाः(&अवस्थाः);
	} अन्यथा यदि (opt_d && !opt_a && argc == 1) {
		यदि (refs_delete_ref(refs, रिक्त, b_शीर्ष.buf, रिक्त, REF_NO_DEREF))
			परिणाम |= error(_("Could not delete %s"), b_शीर्ष.buf);
	} अन्यथा
		usage_with_options(builtin_remote_sethead_usage, options);

	यदि (!शीर्ष_name)
		गच्छ cleanup;
	strbuf_addf(&b_remote_शीर्ष, "refs/remotes/%s/%s", argv[0], शीर्ष_name);
	यदि (!refs_ref_exists(refs, b_remote_शीर्ष.buf)) {
		परिणाम |= error(_("Not a valid ref: %s"), b_remote_शीर्ष.buf);
		गच्छ cleanup;
	}
	was_detached = refs_update_symref_extended(refs, b_शीर्ष.buf, b_remote_शीर्ष.buf,
			"remote set-head", &b_local_शीर्ष, 0);
	यदि (was_detached == -1) {
		परिणाम |= error(_("Could not set up %s"), b_शीर्ष.buf);
		गच्छ cleanup;
	}
	यदि (opt_a)
		report_समुच्चयः_शीर्ष_auto(argv[0], शीर्ष_name, &b_local_शीर्ष, was_detached);
	यदि (remote->follow_remote_शीर्ष == FOLLOW_REMOTE_ALWAYS) {
		संरचना strbuf config_name = STRBUF_INIT;
		strbuf_addf(&config_name,
			"remote.%s.followremotehead", remote->name);
		repo_config_समुच्चयः(the_repository, config_name.buf, "warn");
		strbuf_release(&config_name);
	}

cleanup:
	मुक्त(शीर्ष_name);
	strbuf_release(&b_शीर्ष);
	strbuf_release(&b_remote_शीर्ष);
	strbuf_release(&b_local_शीर्ष);
	प्रत्यागम परिणाम;
}

स्थिर पूर्णाङ्क prune_remote(ध्रुव वर्ण *remote, पूर्णाङ्क dry_सततम्)
{
	पूर्णाङ्क परिणाम = 0;
	संरचना ref_अवस्थाः अवस्थाः = REF_अवस्थाः_INIT;
	संरचना string_list refs_गन्तव्यम्_prune = STRING_LIST_INIT_NODUP;
	संरचना string_list_item *item;

	get_remote_ref_अवस्थाः(remote, &अवस्थाः, GET_REF_अवस्थाः);

	यदि (!अवस्थाः.stale.nr) {
		free_remote_ref_अवस्थाः(&अवस्थाः);
		प्रत्यागम 0;
	}

	printf_ln(_("Pruning %s"), remote);
	printf_ln(_("URL: %s"), अवस्थाः.remote->url.v[0]);

	for_each_string_list_item(item, &अवस्थाः.stale)
		string_list_append(&refs_गन्तव्यम्_prune, item->util);
	string_list_क्रमय(&refs_गन्तव्यम्_prune);

	यदि (!dry_सततम्)
		परिणाम |= refs_delete_refs(get_main_ref_store(the_repository),
					   "remote: prune", &refs_गन्तव्यम्_prune, 0);

	for_each_string_list_item(item, &अवस्थाः.stale) {
		ध्रुव वर्ण *refname = item->util;

		यदि (dry_सततम्)
			printf_ln(_(" * [would prune] %s"),
			       abbrev_ref(refname, "refs/remotes/"));
		अन्यथा
			printf_ln(_(" * [pruned] %s"),
			       abbrev_ref(refname, "refs/remotes/"));
	}

	refs_warn_dangling_symrefs(get_main_ref_store(the_repository),
				   stdout, " ", dry_सततम्, &refs_गन्तव्यम्_prune);

	string_list_clear(&refs_गन्तव्यम्_prune, 0);
	free_remote_ref_अवस्थाः(&अवस्थाः);
	प्रत्यागम परिणाम;
}

स्थिर पूर्णाङ्क prune(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
		 संरचना repository *repo अप्रयुक्तम्)
{
	पूर्णाङ्क dry_सततम् = 0, परिणाम = 0;
	संरचना option options[] = {
		OPT_DRY_सततम्(&dry_सततम्, N_("dry run")),
		OPT_अन्तः()
	};

	argc = parse_options(argc, argv, उपसर्गः, options,
			     builtin_remote_prune_usage, 0);

	यदि (argc < 1)
		usage_with_options(builtin_remote_prune_usage, options);

	प्रति (; argc; argc--, argv++)
		परिणाम |= prune_remote(*argv, dry_सततम्);

	प्रत्यागम परिणाम;
}

स्थिर पूर्णाङ्क get_remote_default(ध्रुव वर्ण *कुञ्चिका, ध्रुव वर्ण *मानम् अप्रयुक्तम्,
			      ध्रुव संरचना config_context *ctx अप्रयुक्तम्,
			      शून्य *priv)
{
	यदि (वाक्यं_तुलय(कुञ्चिका, "remotes.default") == 0) {
		पूर्णाङ्क *प्राप्तम्_2 = priv;
		*प्राप्तम्_2 = 1;
	}
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क update(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
		  संरचना repository *repo अप्रयुक्तम्)
{
	पूर्णाङ्क i, prune = -1;
	संरचना option options[] = {
		OPT_BOOL('p', "prune", &prune,
			 N_("prune remotes after fetching")),
		OPT_अन्तः()
	};
	संरचना child_process cmd = CHILD_PROCESS_INIT;
	पूर्णाङ्क default_defined = 0;

	argc = parse_options(argc, argv, उपसर्गः, options,
			     builtin_remote_update_usage,
			     PARSE_OPT_KEEP_ARGV0);

	strvec_प्रक्षिप(&cmd.तर्काः, "fetch");

	यदि (prune != -1)
		strvec_प्रक्षिप(&cmd.तर्काः, prune ? "--prune" : "--no-prune");
	यदि (verbose)
		strvec_प्रक्षिप(&cmd.तर्काः, "-v");
	strvec_प्रक्षिप(&cmd.तर्काः, "--multiple");
	यदि (argc < 2)
		strvec_प्रक्षिप(&cmd.तर्काः, "default");
	प्रति (i = 1; i < argc; i++)
		strvec_प्रक्षिप(&cmd.तर्काः, argv[i]);

	यदि (वाक्यं_तुलय(cmd.तर्काः.v[cmd.तर्काः.nr-1], "default") == 0) {
		repo_config(the_repository, get_remote_default, &default_defined);
		यदि (!default_defined) {
			strvec_निष्कासय(&cmd.तर्काः);
			strvec_प्रक्षिप(&cmd.तर्काः, "--all");
		}
	}

	cmd.git_cmd = 1;
	प्रत्यागम सततम्_command(&cmd);
}

स्थिर पूर्णाङ्क remove_all_fetch_refspecs(ध्रुव वर्ण *कुञ्चिका)
{
	प्रत्यागम repo_config_समुच्चयः_multivar_gently(the_repository, कुञ्चिका, रिक्त, रिक्त,
					       CONFIG_FLAGS_MULTI_REPLACE);
}

स्थिर शून्य सङ्कलय_branches(संरचना remote *remote, ध्रुव वर्ण **branches,
			 ध्रुव वर्ण *कुञ्चिका)
{
	ध्रुव वर्ण *remotename = remote->name;
	पूर्णाङ्क mirror = remote->mirror;
	संरचना strbuf refspec = STRBUF_INIT;

	प्रति (; *branches; branches++)
		सङ्कलय_branch(कुञ्चिका, *branches, remotename, mirror, &refspec);

	strbuf_release(&refspec);
}

स्थिर पूर्णाङ्क समुच्चयः_remote_branches(ध्रुव वर्ण *remotename, ध्रुव वर्ण **branches,
				पूर्णाङ्क सङ्कलय_बहुलकम्)
{
	संरचना strbuf कुञ्चिका = STRBUF_INIT;
	संरचना remote *remote;

	strbuf_addf(&कुञ्चिका, "remote.%s.fetch", remotename);

	remote = remote_get(remotename);
	यदि (!remote_is_configured(remote, 1)) {
		error(_("No such remote '%s'"), remotename);
		निर्गच्छ(2);
	}

	यदि (!सङ्कलय_बहुलकम् && remove_all_fetch_refspecs(कुञ्चिका.buf)) {
		strbuf_release(&कुञ्चिका);
		प्रत्यागम 1;
	}
	सङ्कलय_branches(remote, branches, कुञ्चिका.buf);

	strbuf_release(&कुञ्चिका);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क समुच्चयः_branches(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
			संरचना repository *repo अप्रयुक्तम्)
{
	पूर्णाङ्क सङ्कलय_बहुलकम् = 0;
	संरचना option options[] = {
		OPT_BOOL('\0', "add", &सङ्कलय_बहुलकम्, N_("add branch")),
		OPT_अन्तः()
	};

	argc = parse_options(argc, argv, उपसर्गः, options,
			     builtin_remote_setbranches_usage, 0);
	यदि (argc == 0) {
		error(_("no remote specified"));
		usage_with_options(builtin_remote_setbranches_usage, options);
	}
	argv[argc] = रिक्त;

	प्रत्यागम समुच्चयः_remote_branches(argv[0], argv + 1, सङ्कलय_बहुलकम्);
}

स्थिर पूर्णाङ्क get_url(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
		   संरचना repository *repo अप्रयुक्तम्)
{
	पूर्णाङ्क प्रक्षिप_बहुलकम् = 0, all_बहुलकम् = 0;
	ध्रुव वर्ण *remotename = रिक्त;
	संरचना remote *remote;
	संरचना strvec *url;
	संरचना option options[] = {
		OPT_BOOL('\0', "push", &प्रक्षिप_बहुलकम्,
			 N_("query push URLs rather than fetch URLs")),
		OPT_BOOL('\0', "all", &all_बहुलकम्,
			 N_("return all URLs")),
		OPT_अन्तः()
	};
	argc = parse_options(argc, argv, उपसर्गः, options,
			     builtin_remote_geturl_usage, 0);

	यदि (argc != 1)
		usage_with_options(builtin_remote_geturl_usage, options);

	remotename = argv[0];

	remote = remote_get(remotename);
	यदि (!remote_is_configured(remote, 1)) {
		error(_("No such remote '%s'"), remotename);
		निर्गच्छ(2);
	}

	url = प्रक्षिप_बहुलकम् ? प्रक्षिप_url_सम्बन्धि_remote(remote) : &remote->url;

	यदि (all_बहुलकम्) {
		प्रति (आकार_t i = 0; i < url->nr; i++)
			printf_ln("%s", url->v[i]);
	} अन्यथा {
		printf_ln("%s", url->v[0]);
	}

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क समुच्चयः_url(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
		   संरचना repository *repo अप्रयुक्तम्)
{
	पूर्णाङ्क प्रक्षिप_बहुलकम् = 0, सङ्कलय_बहुलकम् = 0, delete_बहुलकम् = 0;
	पूर्णाङ्क matches = 0, negative_matches = 0;
	ध्रुव वर्ण *remotename = रिक्त;
	ध्रुव वर्ण *newurl = रिक्त;
	ध्रुव वर्ण *oldurl = रिक्त;
	संरचना remote *remote;
	regex_t old_regex;
	संरचना strvec *urlset;
	संरचना strbuf name_buf = STRBUF_INIT;
	संरचना option options[] = {
		OPT_BOOL('\0', "push", &प्रक्षिप_बहुलकम्,
			 N_("manipulate push URLs")),
		OPT_BOOL('\0', "add", &सङ्कलय_बहुलकम्,
			 N_("add URL")),
		OPT_BOOL('\0', "delete", &delete_बहुलकम्,
			    N_("delete URLs")),
		OPT_अन्तः()
	};
	argc = parse_options(argc, argv, उपसर्गः, options,
			     builtin_remote_seturl_usage,
			     PARSE_OPT_KEEP_ARGV0);

	यदि (सङ्कलय_बहुलकम् && delete_बहुलकम्)
		die(_("--add --delete doesn't make sense"));

	यदि (argc < 3 || argc > 4 || ((सङ्कलय_बहुलकम् || delete_बहुलकम्) && argc != 3))
		usage_with_options(builtin_remote_seturl_usage, options);

	remotename = argv[1];
	newurl = argv[2];
	यदि (argc > 3)
		oldurl = argv[3];

	यदि (delete_बहुलकम्)
		oldurl = newurl;

	remote = remote_get(remotename);
	यदि (!remote_is_configured(remote, 1)) {
		error(_("No such remote '%s'"), remotename);
		निर्गच्छ(2);
	}

	यदि (प्रक्षिप_बहुलकम्) {
		strbuf_addf(&name_buf, "remote.%s.pushurl", remotename);
		urlset = &remote->pushurl;
	} अन्यथा {
		strbuf_addf(&name_buf, "remote.%s.url", remotename);
		urlset = &remote->url;
	}

	/* Special cases that add new entry. */
	यदि ((!oldurl && !delete_बहुलकम्) || सङ्कलय_बहुलकम्) {
		यदि (सङ्कलय_बहुलकम्)
			repo_config_समुच्चयः_multivar(the_repository, name_buf.buf, newurl,
						       "^$", 0);
		अन्यथा
			repo_config_समुच्चयः(the_repository, name_buf.buf, newurl);
		गच्छ बहिर्गमनम्;
	}

	/* Old URL specified. Demand that one matches. */
	यदि (regcomp(&old_regex, oldurl, REG_EXTENDED))
		die(_("Invalid old URL pattern: %s"), oldurl);

	प्रति (आकार_t i = 0; i < urlset->nr; i++)
		यदि (!regexec(&old_regex, urlset->v[i], 0, रिक्त, 0))
			matches++;
		अन्यथा
			negative_matches++;
	यदि (!delete_बहुलकम् && !matches)
		die(_("No such URL found: %s"), oldurl);
	यदि (delete_बहुलकम् && !negative_matches && !प्रक्षिप_बहुलकम्)
		die(_("Will not delete all non-push URLs"));

	regfree(&old_regex);

	यदि (!delete_बहुलकम्)
		repo_config_समुच्चयः_multivar(the_repository, name_buf.buf, newurl, oldurl, 0);
	अन्यथा
		repo_config_समुच्चयः_multivar(the_repository, name_buf.buf, रिक्त, oldurl,
					 CONFIG_FLAGS_MULTI_REPLACE);
बहिर्गमनम्:
	strbuf_release(&name_buf);
	प्रत्यागम 0;
}

पूर्णाङ्क cmd_remote(पूर्णाङ्क argc,
	       ध्रुव वर्ण **argv,
	       ध्रुव वर्ण *उपसर्गः,
	       संरचना repository *repo)
{
	parse_opt_subcommand_fn *fn = रिक्त;
	संरचना option options[] = {
		OPT__VERBOSE(&verbose, N_("be verbose; must be placed before a subcommand")),
		OPT_SUBCOMMAND("add", &fn, सङ्कलय),
		OPT_SUBCOMMAND("rename", &fn, mv),
		OPT_SUBCOMMAND_F("rm", &fn, rm, रिक्त, PARSE_OPT_NOCOMPLETE),
		OPT_SUBCOMMAND("remove", &fn, rm),
		OPT_SUBCOMMAND("set-head", &fn, समुच्चयः_शीर्ष),
		OPT_SUBCOMMAND("set-branches", &fn, समुच्चयः_branches),
		OPT_SUBCOMMAND("get-url", &fn, get_url),
		OPT_SUBCOMMAND("set-url", &fn, समुच्चयः_url),
		OPT_SUBCOMMAND("show", &fn, show),
		OPT_SUBCOMMAND("prune", &fn, prune),
		OPT_SUBCOMMAND("update", &fn, update),
		OPT_अन्तः()
	};

	argc = parse_options(argc, argv, उपसर्गः, options, builtin_remote_usage,
			     PARSE_OPT_SUBCOMMAND_OPTIONAL);

	यदि (fn) {
		प्रत्यागम !!fn(argc, argv, उपसर्गः, repo);
	} अन्यथा {
		यदि (argc) {
			error(_("unknown subcommand: `%s'"), argv[0]);
			usage_with_options(builtin_remote_usage, options);
		}
		प्रत्यागम !!show_all();
	}
}
