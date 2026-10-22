#నిర్వచించు USE_THE_REPOSITORY_VARIABLE

#అన్తర్భావ "builtin.h"
#అన్తర్భావ "advice.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "date.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "ident.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "path.h"
#అన్తర్భావ "transport.h"
#అన్తర్భావ "remote.h"
#అన్తర్భావ "string-list.h"
#అన్తర్భావ "strbuf.h"
#అన్తర్భావ "run-command.h"
#అన్తర్భావ "rebase.h"
#అన్తర్భావ "refs.h"
#అన్తర్భావ "refspec.h"
#అన్తర్భావ "odb.h"
#అన్తర్భావ "strvec.h"
#అన్తర్భావ "commit-reach.h"
#అన్తర్భావ "progress.h"

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి builtin_remote_usage[] = {
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
	ఖాళీ
};

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి builtin_remote_కూడు_usage[] = {
	N_("git remote add [<options>] <name> <url>"),
	ఖాళీ
};

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి builtin_remote_rename_usage[] = {
	N_("git remote rename [--[no-]progress] <old> <new>"),
	ఖాళీ
};

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి builtin_remote_rm_usage[] = {
	N_("git remote remove <name>"),
	ఖాళీ
};

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి builtin_remote_sethead_usage[] = {
	N_("git remote set-head <name> (-a | --auto | -d | --delete | <branch>)"),
	ఖాళీ
};

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి builtin_remote_setbranches_usage[] = {
	N_("git remote set-branches <name> <branch>..."),
	N_("git remote set-branches --add <name> <branch>..."),
	ఖాళీ
};

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి builtin_remote_show_usage[] = {
	N_("git remote show [<options>] <name>"),
	ఖాళీ
};

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి builtin_remote_prune_usage[] = {
	N_("git remote prune [<options>] <name>"),
	ఖాళీ
};

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి builtin_remote_update_usage[] = {
	N_("git remote update [<options>] [<group> | <remote>]..."),
	ఖాళీ
};

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి builtin_remote_geturl_usage[] = {
	N_("git remote get-url [--push] [--all] <name>"),
	ఖాళీ
};

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి builtin_remote_seturl_usage[] = {
	N_("git remote set-url [--push] <name> <newurl> [<oldurl>]"),
	N_("git remote set-url --add <name> <newurl>"),
	N_("git remote set-url --delete <name> <url>"),
	ఖాళీ
};

#నిర్వచించు GET_REF_స్థితులు (1<<0)
#నిర్వచించు GET_తల_పేర్లు (1<<1)
#నిర్వచించు GET_నెట్టు_REF_స్థితులు (1<<2)

స్థిర పూర్ణాంకం verbose;

స్థిర పూర్ణాంకం fetch_remote(స్థిరరాశి అక్షరం *name)
{
	నిర్మాణం బిడ్డ_process cmd = బిడ్డ_PROCESS_INIT;

	strvec_నెట్టు(&cmd.వాదనలు, "fetch");
	ఒకవేళ (verbose)
		strvec_నెట్టు(&cmd.వాదనలు, "-v");
	strvec_నెట్టు(&cmd.వాదనలు, name);
	cmd.git_cmd = 1;
	printf_ln(_("Updating %s"), name);
	ఒకవేళ (నిరంతరం_command(&cmd))
		తిరిగి error(_("Could not fetch %s"), name);
	తిరిగి 0;
}

గణన {
	ట్యాగులు_UNSET = 0,
	ట్యాగులు_DEFAULT = 1,
	ట్యాగులు_సమితి = 2
};

#నిర్వచించు MIRROR_NONE 0
#నిర్వచించు MIRROR_FETCH 1
#నిర్వచించు MIRROR_నెట్టు 2
#నిర్వచించు MIRROR_BOTH (MIRROR_FETCH|MIRROR_నెట్టు)

స్థిర శూన్య కూడు_branch(స్థిరరాశి అక్షరం *కీలకం, స్థిరరాశి అక్షరం *branchname,
		       స్థిరరాశి అక్షరం *remotename, పూర్ణాంకం mirror, నిర్మాణం strbuf *tmp)
{
	strbuf_reset(tmp);
	strbuf_addch(tmp, '+');
	ఒకవేళ (mirror)
		strbuf_addf(tmp, "refs/%s:refs/%s",
				branchname, branchname);
	లేకపోతే
		strbuf_addf(tmp, "refs/heads/%s:refs/remotes/%s/%s",
				branchname, remotename, branchname);
	repo_config_సమితి_multivar(the_repository, కీలకం, tmp->buf, "^$", 0);
}

స్థిర స్థిరరాశి అక్షరం mirror_advice[] =
N_("--mirror is dangerous and deprecated; please\n"
   "\t use --mirror=fetch or --mirror=push instead");

స్థిర పూర్ణాంకం parse_mirror_opt(స్థిరరాశి నిర్మాణం option *opt, స్థిరరాశి అక్షరం *arg, పూర్ణాంకం not)
{
	గుర్తులేని *mirror = opt->విలువ;
	ఒకవేళ (not)
		*mirror = MIRROR_NONE;
	లేకపోతే ఒకవేళ (!arg) {
		warning("%s", _(mirror_advice));
		*mirror = MIRROR_BOTH;
	}
	లేకపోతే ఒకవేళ (!strcmp(arg, "fetch"))
		*mirror = MIRROR_FETCH;
	లేకపోతే ఒకవేళ (!strcmp(arg, "push"))
		*mirror = MIRROR_నెట్టు;
	లేకపోతే
		తిరిగి error(_("unknown --mirror argument: %s"), arg);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం check_remote_collision(నిర్మాణం remote *remote, శూన్య *దత్తాంశం)
{
	స్థిరరాశి అక్షరం *name = దత్తాంశం;
	స్థిరరాశి అక్షరం *p;

	ఒకవేళ (skip_ఉపసర్గ(name, remote->name, &p) && *p == '/')
		die(_("remote name '%s' is a subset of existing remote '%s'"),
		    name, remote->name);
	ఒకవేళ (skip_ఉపసర్గ(remote->name, name, &p) && *p == '/')
		die(_("remote name '%s' is a superset of existing remote '%s'"),
		    name, remote->name);

	తిరిగి 0;
}

స్థిర పూర్ణాంకం కూడు(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
	       నిర్మాణం repository *repo వాడని)
{
	పూర్ణాంకం fetch = 0, fetch_ట్యాగులు = ట్యాగులు_DEFAULT;
	గుర్తులేని mirror = MIRROR_NONE;
	నిర్మాణం string_list track = STRING_LIST_INIT_NODUP;
	స్థిరరాశి అక్షరం *master = ఖాళీ;
	నిర్మాణం remote *remote;
	నిర్మాణం strbuf buf = STRBUF_INIT, buf2 = STRBUF_INIT;
	స్థిరరాశి అక్షరం *name, *url;
	పూర్ణాంకం ఫలితం = 0;

	నిర్మాణం option options[] = {
		OPT_BOOL('f', "fetch", &fetch, N_("fetch the remote branches")),
		OPT_సమితి_INT(0, "tags", &fetch_ట్యాగులు,
			    N_("import all tags and associated objects when fetching\n"
			       "or do not fetch any tag at all (--no-tags)"),
			    ట్యాగులు_సమితి),
		OPT_STRING_LIST('t', "track", &track, N_("branch"),
				N_("branch(es) to track")),
		OPT_STRING('m', "master", &master, N_("branch"), N_("master branch")),
		OPT_CALLBACK_F(0, "mirror", &mirror, "(push|fetch)",
			N_("set up remote as a mirror to push to or fetch from"),
			PARSE_OPT_OPTARG | PARSE_OPT_COMP_ARG, parse_mirror_opt),
		OPT_చివర()
	};

	argc = parse_options(argc, argv, ఉపసర్గ, options,
			     builtin_remote_కూడు_usage, 0);

	ఒకవేళ (argc != 2)
		usage_with_options(builtin_remote_కూడు_usage, options);

	ఒకవేళ (mirror && master)
		die(_("specifying a master branch makes no sense with --mirror"));
	ఒకవేళ (mirror && !(mirror & MIRROR_FETCH) && track.nr)
		die(_("specifying branches to track makes sense only with fetch mirrors"));

	name = argv[0];
	url = argv[1];

	remote = remote_get(name);
	ఒకవేళ (remote_is_configured(remote, 1)) {
		error(_("remote %s already exists."), name);
		నిష్క్రమించు(3);
	}

	ఒకవేళ (!valid_remote_name(name))
		die(_("'%s' is not a valid remote name"), name);

	for_each_remote(check_remote_collision, (శూన్య *)name);

	strbuf_addf(&buf, "remote.%s.url", name);
	repo_config_సమితి(the_repository, buf.buf, url);

	ఒకవేళ (!mirror || mirror & MIRROR_FETCH) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.fetch", name);
		ఒకవేళ (track.nr == 0)
			string_list_append(&track, "*");
		కొరకు (కొలత_t i = 0; i < track.nr; i++) {
			కూడు_branch(buf.buf, track.వస్తువులు[i].string,
				   name, mirror, &buf2);
		}
	}

	ఒకవేళ (mirror & MIRROR_నెట్టు) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.mirror", name);
		repo_config_సమితి(the_repository, buf.buf, "true");
	}

	ఒకవేళ (fetch_ట్యాగులు != ట్యాగులు_DEFAULT) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.tagOpt", name);
		repo_config_సమితి(the_repository, buf.buf,
				fetch_ట్యాగులు == ట్యాగులు_సమితి ? "--tags" : "--no-tags");
	}

	ఒకవేళ (fetch && fetch_remote(name)) {
		ఫలితం = 1;
		వెళ్ళు బహిర్గతం;
	}

	ఒకవేళ (master) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "refs/remotes/%s/HEAD", name);

		strbuf_reset(&buf2);
		strbuf_addf(&buf2, "refs/remotes/%s/%s", name, master);

		ఒకవేళ (refs_update_symref(get_main_ref_store(the_repository), buf.buf, buf2.buf, "remote add"))
			ఫలితం = error(_("Could not setup master '%s'"), master);
	}

బహిర్గతం:
	strbuf_release(&buf);
	strbuf_release(&buf2);
	string_list_clear(&track, 0);

	తిరిగి ఫలితం;
}

నిర్మాణం branch_info {
	అక్షరం *remote_name;
	నిర్మాణం string_list విలీనం;
	గణన rebase_type rebase;
	అక్షరం *నెట్టు_remote_name;
};

స్థిర నిర్మాణం string_list branch_list = STRING_LIST_INIT_DUP;

స్థిర స్థిరరాశి అక్షరం *abbrev_ref(స్థిరరాశి అక్షరం *name, స్థిరరాశి అక్షరం *ఉపసర్గ)
{
	skip_ఉపసర్గ(name, ఉపసర్గ, &name);
	తిరిగి name;
}
#నిర్వచించు abbrev_branch(name) abbrev_ref((name), "refs/heads/")

స్థిర పూర్ణాంకం config_read_branches(స్థిరరాశి అక్షరం *కీలకం, స్థిరరాశి అక్షరం *విలువ,
				స్థిరరాశి నిర్మాణం config_context *ctx వాడని,
				శూన్య *దత్తాంశం వాడని)
{
	స్థిరరాశి అక్షరం *orig_కీలకం = కీలకం;
	అక్షరం *name;
	నిర్మాణం string_list_అంశం *అంశం;
	నిర్మాణం branch_info *info;
	గణన { REMOTE, విలీనం_2, REBASE, నెట్టు_REMOTE } type;
	కొలత_t కీలకం_len;

	ఒకవేళ (!starts_with(కీలకం, "branch."))
		తిరిగి 0;

	కీలకం += అక్షరశ్రేణి_పొడవు("branch.");
	ఒకవేళ (strip_suffix(కీలకం, ".remote", &కీలకం_len))
		type = REMOTE;
	లేకపోతే ఒకవేళ (strip_suffix(కీలకం, ".merge", &కీలకం_len))
		type = విలీనం_2;
	లేకపోతే ఒకవేళ (strip_suffix(కీలకం, ".rebase", &కీలకం_len))
		type = REBASE;
	లేకపోతే ఒకవేళ (strip_suffix(కీలకం, ".pushremote", &కీలకం_len))
		type = నెట్టు_REMOTE;
	లేకపోతే
		తిరిగి 0;

	name = xmemdupz(కీలకం, కీలకం_len);
	అంశం = string_list_చొప్పించు(&branch_list, name);

	ఒకవేళ (!అంశం->util)
		అంశం->util = xcalloc(1, పరిమాణం(నిర్మాణం branch_info));
	info = అంశం->util;
	ఎంపిక (type) {
	స్థితి REMOTE:
		ఒకవేళ (info->remote_name)
			warning(_("more than one %s"), orig_కీలకం);
		info->remote_name = xstrdup(విలువ);
		ఆపు;
	స్థితి విలీనం_2: {
		స్థిరరాశి అక్షరం *space = strchr(విలువ, ' ');
		విలువ = abbrev_branch(విలువ);
		అయితే (space) {
			అక్షరం *విలీనం;
			విలీనం = xstrndup(విలువ, space - విలువ);
			string_list_append(&info->విలీనం, విలీనం);
			విలువ = abbrev_branch(space + 1);
			space = strchr(విలువ, ' ');
		}
		string_list_append(&info->విలీనం, xstrdup(విలువ));
		ఆపు;
	}
	స్థితి REBASE:
		/*
		 * Consider invalid values as false and check the
		 * truth value with >= REBASE_TRUE.
		 */
		info->rebase = rebase_parse_విలువ(విలువ);
		ఒకవేళ (info->rebase == REBASE_INVALID)
			warning(_("unhandled branch.%s.rebase=%s; assuming "
				  "'true'"), name, విలువ);
		ఆపు;
	స్థితి నెట్టు_REMOTE:
		ఒకవేళ (info->నెట్టు_remote_name)
			warning(_("more than one %s"), orig_కీలకం);
		info->నెట్టు_remote_name = xstrdup(విలువ);
		ఆపు;
	అప్రమేయ:
		BUG("unexpected type=%d", type);
	}

	విడుదల(name);
	తిరిగి 0;
}

స్థిర శూన్య read_branches(శూన్య)
{
	ఒకవేళ (branch_list.nr)
		తిరిగి;
	repo_config(the_repository, config_read_branches, ఖాళీ);
}

నిర్మాణం ref_స్థితులు {
	నిర్మాణం remote *remote;
	నిర్మాణం string_list new_refs, skipped, stale, tracked, heads, నెట్టు;
	పూర్ణాంకం queried;
};

#నిర్వచించు REF_స్థితులు_INIT { \
	.new_refs = STRING_LIST_INIT_DUP, \
	.skipped = STRING_LIST_INIT_DUP, \
	.stale = STRING_LIST_INIT_DUP, \
	.tracked = STRING_LIST_INIT_DUP, \
	.heads = STRING_LIST_INIT_DUP, \
	.నెట్టు = STRING_LIST_INIT_DUP, \
}

స్థిర పూర్ణాంకం get_ref_స్థితులు(స్థిరరాశి నిర్మాణం ref *remote_refs, నిర్మాణం ref_స్థితులు *స్థితులు)
{
	నిర్మాణం ref *fetch_map = ఖాళీ, **తోక = &fetch_map;
	నిర్మాణం ref *ref, *stale_refs;
	పూర్ణాంకం i;

	కొరకు (i = 0; i < స్థితులు->remote->fetch.nr; i++)
		ఒకవేళ (get_fetch_map(remote_refs, &స్థితులు->remote->fetch.వస్తువులు[i], &తోక, 1))
			die(_("Could not get fetch map for refspec %s"),
				స్థితులు->remote->fetch.వస్తువులు[i].raw);

	కొరకు (ref = fetch_map; ref; ref = ref->తదుపరి) {
		ఒకవేళ (refname_matches_negative_refspec_అంశం(ref->name, &స్థితులు->remote->fetch))
			string_list_append(&స్థితులు->skipped, abbrev_branch(ref->name));
		లేకపోతే ఒకవేళ (!ref->peer_ref || !refs_ref_exists(get_main_ref_store(the_repository), ref->peer_ref->name))
			string_list_append(&స్థితులు->new_refs, abbrev_branch(ref->name));
		లేకపోతే
			string_list_append(&స్థితులు->tracked, abbrev_branch(ref->name));
	}
	stale_refs = get_stale_heads(&స్థితులు->remote->fetch, fetch_map);
	కొరకు (ref = stale_refs; ref; ref = ref->తదుపరి) {
		నిర్మాణం string_list_అంశం *అంశం =
			string_list_append(&స్థితులు->stale, abbrev_branch(ref->name));
		అంశం->util = xstrdup(ref->name);
	}
	free_refs(stale_refs);
	free_refs(fetch_map);

	string_list_క్రమీకరించు(&స్థితులు->new_refs);
	string_list_క్రమీకరించు(&స్థితులు->skipped);
	string_list_క్రమీకరించు(&స్థితులు->tracked);
	string_list_క్రమీకరించు(&స్థితులు->stale);

	తిరిగి 0;
}

నిర్మాణం నెట్టు_info {
	అక్షరం *dest;
	పూర్ణాంకం forced;
	గణన {
		నెట్టు_స్థితి_CREATE = 0,
		నెట్టు_స్థితి_DELETE,
		నెట్టు_స్థితి_UPTODATE,
		నెట్టు_స్థితి_FASTFORWARD,
		నెట్టు_స్థితి_OUTOFDATE,
		నెట్టు_స్థితి_NOTQUERIED
	} స్థితి_2;
};

స్థిర పూర్ణాంకం get_నెట్టు_ref_స్థితులు(స్థిరరాశి నిర్మాణం ref *remote_refs,
	నిర్మాణం ref_స్థితులు *స్థితులు)
{
	నిర్మాణం remote *remote = స్థితులు->remote;
	నిర్మాణం ref *ref, *local_refs, *నెట్టు_map;
	ఒకవేళ (remote->mirror)
		తిరిగి 0;

	local_refs = get_local_heads();
	నెట్టు_map = copy_ref_list(remote_refs);

	సరిపోలిక_నెట్టు_refs(local_refs, &నెట్టు_map, &remote->నెట్టు, సరిపోలిక_REFS_NONE);

	కొరకు (ref = నెట్టు_map; ref; ref = ref->తదుపరి) {
		నిర్మాణం string_list_అంశం *అంశం;
		నిర్మాణం నెట్టు_info *info;

		ఒకవేళ (!ref->peer_ref)
			కొనసాగు;
		oidcpy(&ref->new_oid, &ref->peer_ref->new_oid);

		అంశం = string_list_append(&స్థితులు->నెట్టు,
					  abbrev_branch(ref->peer_ref->name));
		అంశం->util = xcalloc(1, పరిమాణం(నిర్మాణం నెట్టు_info));
		info = అంశం->util;
		info->forced = ref->force;
		info->dest = xstrdup(abbrev_branch(ref->name));

		ఒకవేళ (is_null_oid(&ref->new_oid)) {
			info->స్థితి_2 = నెట్టు_స్థితి_DELETE;
		} లేకపోతే ఒకవేళ (oideq(&ref->old_oid, &ref->new_oid))
			info->స్థితి_2 = నెట్టు_స్థితి_UPTODATE;
		లేకపోతే ఒకవేళ (is_null_oid(&ref->old_oid))
			info->స్థితి_2 = నెట్టు_స్థితి_CREATE;
		లేకపోతే ఒకవేళ (odb_has_object(the_repository->objects, &ref->old_oid,
					ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR) &&
			 ref_newer(&ref->new_oid, &ref->old_oid))
			info->స్థితి_2 = నెట్టు_స్థితి_FASTFORWARD;
		లేకపోతే
			info->స్థితి_2 = నెట్టు_స్థితి_OUTOFDATE;
	}
	free_refs(local_refs);
	free_refs(నెట్టు_map);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం get_నెట్టు_ref_స్థితులు_noquery(నిర్మాణం ref_స్థితులు *స్థితులు)
{
	పూర్ణాంకం i;
	నిర్మాణం remote *remote = స్థితులు->remote;
	నిర్మాణం string_list_అంశం *అంశం;
	నిర్మాణం నెట్టు_info *info;

	ఒకవేళ (remote->mirror)
		తిరిగి 0;

	ఒకవేళ (!remote->నెట్టు.nr) {
		అంశం = string_list_append(&స్థితులు->నెట్టు, _("(matching)"));
		info = అంశం->util = xcalloc(1, పరిమాణం(నిర్మాణం నెట్టు_info));
		info->స్థితి_2 = నెట్టు_స్థితి_NOTQUERIED;
		info->dest = xstrdup(అంశం->string);
	}
	కొరకు (i = 0; i < remote->నెట్టు.nr; i++) {
		స్థిరరాశి నిర్మాణం refspec_అంశం *spec = &remote->నెట్టు.వస్తువులు[i];
		ఒకవేళ (spec->matching)
			అంశం = string_list_append(&స్థితులు->నెట్టు, _("(matching)"));
		లేకపోతే ఒకవేళ (అక్షరశ్రేణి_పొడవు(spec->src))
			అంశం = string_list_append(&స్థితులు->నెట్టు, spec->src);
		లేకపోతే
			అంశం = string_list_append(&స్థితులు->నెట్టు, _("(delete)"));

		info = అంశం->util = xcalloc(1, పరిమాణం(నిర్మాణం నెట్టు_info));
		info->forced = spec->force;
		info->స్థితి_2 = నెట్టు_స్థితి_NOTQUERIED;
		info->dest = xstrdup(spec->dst ? spec->dst : అంశం->string);
	}
	తిరిగి 0;
}

స్థిర పూర్ణాంకం get_తల_పేర్లు(స్థిరరాశి నిర్మాణం ref *remote_refs, నిర్మాణం ref_స్థితులు *స్థితులు)
{
	నిర్మాణం ref *ref, *matches;
	నిర్మాణం ref *fetch_map = ఖాళీ, **fetch_map_తోక = &fetch_map;
	నిర్మాణం refspec_అంశం refspec = {
		.force = 0,
		.నమూనా = 1,
		.src = (అక్షరం *) "refs/heads/*",
		.dst = (అక్షరం *) "refs/heads/*",
	};

	get_fetch_map(remote_refs, &refspec, &fetch_map_తోక, 0);
	matches = guess_remote_తల(కనుగొను_ref_by_name(remote_refs, "HEAD"),
				    fetch_map, REMOTE_GUESS_తల_ALL);
	కొరకు (ref = matches; ref; ref = ref->తదుపరి)
		string_list_append(&స్థితులు->heads, abbrev_branch(ref->name));

	free_refs(fetch_map);
	free_refs(matches);
	తిరిగి 0;
}

నిర్మాణం తెలిసిన_remote {
	నిర్మాణం తెలిసిన_remote *తదుపరి;
	నిర్మాణం remote *remote;
};

నిర్మాణం తెలిసిన_remotes {
	నిర్మాణం remote *గమ్యం_delete;
	నిర్మాణం తెలిసిన_remote *list;
};

స్థిర పూర్ణాంకం కూడు_తెలిసిన_remote(నిర్మాణం remote *remote, శూన్య *cb_దత్తాంశం)
{
	నిర్మాణం తెలిసిన_remotes *all = cb_దత్తాంశం;
	నిర్మాణం తెలిసిన_remote *r;

	ఒకవేళ (!strcmp(all->గమ్యం_delete->name, remote->name))
		తిరిగి 0;

	r = xmalloc(పరిమాణం(*r));
	r->remote = remote;
	r->తదుపరి = all->list;
	all->list = r;
	తిరిగి 0;
}

నిర్మాణం branches_for_remote {
	నిర్మాణం remote *remote;
	నిర్మాణం string_list *branches, *skipped;
	నిర్మాణం తెలిసిన_remotes *keep;
};

స్థిర పూర్ణాంకం కూడు_branch_for_removal(స్థిరరాశి నిర్మాణం reference *ref, శూన్య *cb_దత్తాంశం)
{
	నిర్మాణం branches_for_remote *branches = cb_దత్తాంశం;
	నిర్మాణం refspec_అంశం refspec;
	నిర్మాణం తెలిసిన_remote *kr;

	memset(&refspec, 0, పరిమాణం(refspec));
	refspec.dst = (అక్షరం *)ref->name;
	ఒకవేళ (remote_కనుగొను_tracking(branches->remote, &refspec))
		తిరిగి 0;
	విడుదల(refspec.src);

	/* don't delete a branch if another remote also uses it */
	కొరకు (kr = branches->keep->list; kr; kr = kr->తదుపరి) {
		memset(&refspec, 0, పరిమాణం(refspec));
		refspec.dst = (అక్షరం *)ref->name;
		ఒకవేళ (!remote_కనుగొను_tracking(kr->remote, &refspec)) {
			విడుదల(refspec.src);
			తిరిగి 0;
		}
	}

	/* don't delete non-remote-tracking refs */
	ఒకవేళ (!starts_with(ref->name, "refs/remotes/")) {
		/* advise user how to delete local branches */
		ఒకవేళ (starts_with(ref->name, "refs/heads/"))
			string_list_append(branches->skipped,
					   abbrev_branch(ref->name));
		/* silently skip over other non-remote refs */
		తిరిగి 0;
	}

	string_list_append(branches->branches, ref->name);

	తిరిగి 0;
}

నిర్మాణం rename_info {
	స్థిరరాశి అక్షరం *old_name;
	స్థిరరాశి అక్షరం *new_name;
	నిర్మాణం ref_transaction *transaction;
	నిర్మాణం పురోగతి *పురోగతి;
	నిర్మాణం strbuf *err;
	uint32_t పురోగతి_nr;
	uint64_t సూచీ;
};

స్థిర శూన్య compute_renamed_ref(నిర్మాణం rename_info *rename,
				స్థిరరాశి అక్షరం *refname,
				నిర్మాణం strbuf *బహిర్గతం)
{
	strbuf_reset(బహిర్గతం);
	strbuf_addstr(బహిర్గతం, refname);
	strbuf_splice(బహిర్గతం, అక్షరశ్రేణి_పొడవు("refs/remotes/"), అక్షరశ్రేణి_పొడవు(rename->old_name),
		      rename->new_name, అక్షరశ్రేణి_పొడవు(rename->new_name));
}

స్థిర పూర్ణాంకం rename_one_reflog_నమోదు(స్థిరరాశి అక్షరం *old_refname,
				   నిర్మాణం object_id *old_oid,
				   నిర్మాణం object_id *new_oid,
				   స్థిరరాశి అక్షరం *committer,
				   timestamp_t timestamp, పూర్ణాంకం tz,
				   స్థిరరాశి అక్షరం *సందేశం, శూన్య *cb_దత్తాంశం)
{
	నిర్మాణం rename_info *rename = cb_దత్తాంశం;
	నిర్మాణం strbuf new_refname = STRBUF_INIT;
	నిర్మాణం strbuf తటస్థం = STRBUF_INIT;
	నిర్మాణం strbuf name = STRBUF_INIT;
	నిర్మాణం strbuf mail = STRBUF_INIT;
	నిర్మాణం ident_విభజించు ident;
	స్థిరరాశి అక్షరం *date;
	పూర్ణాంకం error;

	compute_renamed_ref(rename, old_refname, &new_refname);

	ఒకవేళ (విభజించు_ident_పంక్తి(&ident, committer, అక్షరశ్రేణి_పొడవు(committer)) < 0) {
		error = -1;
		వెళ్ళు బహిర్గతం;
	}

	strbuf_కూడు(&name, ident.name_begin, ident.name_చివర - ident.name_begin);
	strbuf_కూడు(&mail, ident.mail_begin, ident.mail_చివర - ident.mail_begin);

	date = show_date(timestamp, tz, DATE_బహుళకం(సాధారణ));
	strbuf_addstr(&తటస్థం, fmt_ident(name.buf, mail.buf,
					  WANT_BLANK_IDENT, date, 0));

	error = ref_transaction_update_reflog(rename->transaction, new_refname.buf,
					      new_oid, old_oid, తటస్థం.buf, సందేశం,
					      rename->సూచీ++, rename->err);

బహిర్గతం:
	strbuf_release(&new_refname);
	strbuf_release(&తటస్థం);
	strbuf_release(&name);
	strbuf_release(&mail);
	తిరిగి error;
}

స్థిర పూర్ణాంకం rename_one_reflog(స్థిరరాశి అక్షరం *old_refname,
			     స్థిరరాశి నిర్మాణం object_id *old_oid,
			     నిర్మాణం rename_info *rename)
{
	నిర్మాణం strbuf new_refname = STRBUF_INIT;
	నిర్మాణం strbuf సందేశం_2 = STRBUF_INIT;
	పూర్ణాంకం error;

	ఒకవేళ (!refs_reflog_exists(get_main_ref_store(the_repository), old_refname))
		తిరిగి 0;

	error = refs_for_each_reflog_ent(get_main_ref_store(the_repository),
					 old_refname, rename_one_reflog_నమోదు, rename);
	ఒకవేళ (error < 0)
		వెళ్ళు బహిర్గతం;

	compute_renamed_ref(rename, old_refname, &new_refname);

	/*
	 * Manually write the reflog entry for the now-renamed ref. We cannot
	 * rely on `rename_one_ref()` to do this for us as that would screw
	 * over order in which reflog entries are being written.
	 *
	 * Furthermore, we only append the entry in case the reference
	 * resolves. Missing references shouldn't have reflogs anyway.
	 */
	strbuf_addf(&సందేశం_2, "remote: renamed %s to %s", old_refname,
		    new_refname.buf);

	error = ref_transaction_update_reflog(rename->transaction, new_refname.buf,
					      old_oid, old_oid, git_committer_info(0),
					      సందేశం_2.buf, rename->సూచీ++, rename->err);
	ఒకవేళ (error < 0)
		తిరిగి error;

బహిర్గతం:
	strbuf_release(&new_refname);
	strbuf_release(&సందేశం_2);
	తిరిగి error;
}

స్థిర పూర్ణాంకం rename_one_ref(స్థిరరాశి నిర్మాణం reference *ref, శూన్య *cb_దత్తాంశం)
{
	నిర్మాణం strbuf new_referent = STRBUF_INIT;
	నిర్మాణం strbuf new_refname = STRBUF_INIT;
	నిర్మాణం rename_info *rename = cb_దత్తాంశం;
	స్థిరరాశి నిర్మాణం object_id *oid = ref->oid;
	స్థిరరాశి అక్షరం *referent = ref->లక్ష్యం;
	పూర్ణాంకం error;

	compute_renamed_ref(rename, ref->name, &new_refname);

	ఒకవేళ (ref->flags & REF_ISSYMREF) {
		/*
		 * Stupidly enough `referent` is not pointing to the immediate
		 * target of a symref, but it's the recursively resolved value.
		 * So symrefs pointing to symrefs would be misresolved, and
		 * unborn symrefs don't have any value for the `referent` at all.
		 */
		referent = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
						   ref->name, RESOLVE_REF_NO_RECURSE,
						   ఖాళీ, ఖాళీ);
		compute_renamed_ref(rename, referent, &new_referent);
		oid = ఖాళీ;
	}

	error = ref_transaction_delete(rename->transaction, ref->name,
				       oid, referent, REF_NO_DEREF, ఖాళీ, rename->err);
	ఒకవేళ (error < 0)
		వెళ్ళు బహిర్గతం;

	error = ref_transaction_update(rename->transaction, new_refname.buf, oid, null_oid(the_hash_algo),
				       (ref->flags & REF_ISSYMREF) ? new_referent.buf : ఖాళీ, ఖాళీ,
				       REF_SKIP_CREATE_REFLOG | REF_NO_DEREF | REF_SKIP_OID_VERIFICATION,
				       ఖాళీ, rename->err);
	ఒకవేళ (error < 0)
		వెళ్ళు బహిర్గతం;

	error = rename_one_reflog(ref->name, oid, rename);
	ఒకవేళ (error < 0)
		వెళ్ళు బహిర్గతం;

	display_పురోగతి(rename->పురోగతి, ++rename->పురోగతి_nr);

బహిర్గతం:
	strbuf_release(&new_referent);
	strbuf_release(&new_refname);
	తిరిగి error;
}

స్థిర పూర్ణాంకం migrate_file(నిర్మాణం remote *remote)
{
	నిర్మాణం strbuf buf = STRBUF_INIT;

	strbuf_addf(&buf, "remote.%s.url", remote->name);
	కొరకు (కొలత_t i = 0; i < remote->url.nr; i++)
		repo_config_సమితి_multivar(the_repository, buf.buf, remote->url.v[i], "^$", 0);
	strbuf_reset(&buf);
	strbuf_addf(&buf, "remote.%s.push", remote->name);
	కొరకు (పూర్ణాంకం i = 0; i < remote->నెట్టు.nr; i++)
		repo_config_సమితి_multivar(the_repository, buf.buf, remote->నెట్టు.వస్తువులు[i].raw, "^$", 0);
	strbuf_reset(&buf);
	strbuf_addf(&buf, "remote.%s.fetch", remote->name);
	కొరకు (పూర్ణాంకం i = 0; i < remote->fetch.nr; i++)
		repo_config_సమితి_multivar(the_repository, buf.buf, remote->fetch.వస్తువులు[i].raw, "^$", 0);
#నిర్వచితం_కాకపోతే WITH_BREAKING_CHANGES
	ఒకవేళ (remote->origin == REMOTE_REMOTES)
		unlink_or_warn(repo_git_మార్గం_replace(the_repository, &buf,
						     "remotes/%s", remote->name));
	లేకపోతే ఒకవేళ (remote->origin == REMOTE_BRANCHES)
		unlink_or_warn(repo_git_మార్గం_replace(the_repository, &buf,
						     "branches/%s", remote->name));
#అయితే_ముగింపు /* WITH_BREAKING_CHANGES */
	strbuf_release(&buf);

	తిరిగి 0;
}

నిర్మాణం నెట్టు_default_info
{
	స్థిరరాశి అక్షరం *old_name;
	గణన config_scope scope;
	నిర్మాణం strbuf origin;
	పూర్ణాంకం linenr;
};

స్థిర పూర్ణాంకం config_read_నెట్టు_default(స్థిరరాశి అక్షరం *కీలకం, స్థిరరాశి అక్షరం *విలువ,
	స్థిరరాశి నిర్మాణం config_context *ctx, శూన్య *cb)
{
	స్థిరరాశి నిర్మాణం కీలకం_విలువ_info *kvi = ctx->kvi;

	నిర్మాణం నెట్టు_default_info* info = cb;
	ఒకవేళ (strcmp(కీలకం, "remote.pushdefault") ||
	    !విలువ || strcmp(విలువ, info->old_name))
		తిరిగి 0;

	info->scope = kvi->scope;
	strbuf_reset(&info->origin);
	strbuf_addstr(&info->origin, config_origin_type_name(kvi->origin_type));
	info->linenr = kvi->linenr;

	తిరిగి 0;
}

స్థిర శూన్య handle_నెట్టు_default(స్థిరరాశి అక్షరం* old_name, స్థిరరాశి అక్షరం* new_name)
{
	నిర్మాణం నెట్టు_default_info నెట్టు_default = {
		.old_name = old_name,
		.scope = CONFIG_SCOPE_UNKNOWN,
		.origin = STRBUF_INIT,
		.linenr = -1,
	};
	repo_config(the_repository, config_read_నెట్టు_default, &నెట్టు_default);
	ఒకవేళ (నెట్టు_default.scope >= CONFIG_SCOPE_COMMAND)
		; /* pass */
	లేకపోతే ఒకవేళ (నెట్టు_default.scope >= CONFIG_SCOPE_LOCAL) {
		పూర్ణాంకం ఫలితం = repo_config_సమితి_gently(the_repository, "remote.pushDefault",
						    new_name);
		ఒకవేళ (new_name && ఫలితం && ఫలితం != CONFIG_NOTHING_సమితి)
			die(_("could not set '%s'"), "remote.pushDefault");
		లేకపోతే ఒకవేళ (!new_name && ఫలితం && ఫలితం != CONFIG_NOTHING_సమితి)
			die(_("could not unset '%s'"), "remote.pushDefault");
	} లేకపోతే ఒకవేళ (నెట్టు_default.scope >= CONFIG_SCOPE_SYSTEM) {
		/* warn */
		warning(_("The %s configuration remote.pushDefault in:\n"
			  "\t%s:%d\n"
			  "now names the non-existent remote '%s'"),
			config_scope_name(నెట్టు_default.scope),
			నెట్టు_default.origin.buf, నెట్టు_default.linenr,
			old_name);
	}

	strbuf_release(&నెట్టు_default.origin);
}

స్థిర స్థిరరాశి అక్షరం conflicting_remote_refs_advice[] = N_(
	"The remote you are trying to rename has conflicting references in the\n"
	"new target refspec. This is most likely caused by you trying to nest\n"
	"a remote into itself, e.g. by renaming 'parent' into 'parent/child'\n"
	"or by unnesting a remote, e.g. the other way round.\n"
	"\n"
	"If that is the case, you can address this by first renaming the\n"
	"remote to a different name.\n");

స్థిర పూర్ణాంకం mv(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
	      నిర్మాణం repository *repo వాడని)
{
	పూర్ణాంకం show_పురోగతి = isatty(2);
	నిర్మాణం option options[] = {
		OPT_BOOL(0, "progress", &show_పురోగతి, N_("force progress reporting")),
		OPT_చివర()
	};
	నిర్మాణం remote *oldremote, *newremote;
	నిర్మాణం strbuf buf = STRBUF_INIT, buf2 = STRBUF_INIT, buf3 = STRBUF_INIT,
		old_remote_context = STRBUF_INIT, err = STRBUF_INIT;
	నిర్మాణం rename_info rename = {
		.err = &err,
	};
	పూర్ణాంకం refspecs_need_update = 0;
	పూర్ణాంకం ఫలితం = 0;

	argc = parse_options(argc, argv, ఉపసర్గ, options,
			     builtin_remote_rename_usage, 0);

	ఒకవేళ (argc != 2)
		usage_with_options(builtin_remote_rename_usage, options);

	rename.old_name = argv[0];
	rename.new_name = argv[1];

	oldremote = remote_get(rename.old_name);
	ఒకవేళ (!remote_is_configured(oldremote, 1)) {
		error(_("No such remote: '%s'"), rename.old_name);
		నిష్క్రమించు(2);
	}

	ఒకవేళ (!strcmp(rename.old_name, rename.new_name) && oldremote->origin != REMOTE_CONFIG)
		తిరిగి migrate_file(oldremote);

	newremote = remote_get(rename.new_name);
	ఒకవేళ (remote_is_configured(newremote, 1)) {
		error(_("remote %s already exists."), rename.new_name);
		నిష్క్రమించు(3);
	}

	ఒకవేళ (!valid_remote_name(rename.new_name))
		die(_("'%s' is not a valid remote name"), rename.new_name);

	strbuf_addf(&buf, "remote.%s", rename.old_name);
	strbuf_addf(&buf2, "remote.%s", rename.new_name);
	ఒకవేళ (repo_config_rename_section(the_repository, buf.buf, buf2.buf) < 1) {
		ఫలితం = error(_("Could not rename config section '%s' to '%s'"),
			       buf.buf, buf2.buf);
		వెళ్ళు బహిర్గతం;
	}

	strbuf_addf(&old_remote_context, ":refs/remotes/%s/", rename.old_name);

	కొరకు (పూర్ణాంకం i = 0; i < oldremote->fetch.nr && !refspecs_need_update; i++)
		refspecs_need_update = !!అక్షరశ్రేణి_శోధన(oldremote->fetch.వస్తువులు[i].raw,
						old_remote_context.buf);

	ఒకవేళ (refspecs_need_update) {
		నిర్మాణం refs_for_each_ref_options opts = {
			.flags = REFS_FOR_EACH_INCLUDE_BROKEN,
		};
		rename.transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
							       0, &err);
		ఒకవేళ (!rename.transaction)
			వెళ్ళు బహిర్గతం;

		ఒకవేళ (show_పురోగతి)
			rename.పురోగతి = ప్రారంభం_delayed_పురోగతి(the_repository,
								 _("Renaming remote references"), 0);

		strbuf_reset(&buf);
		strbuf_addf(&buf, "refs/remotes/%s/", rename.old_name);
		opts.ఉపసర్గ = buf.buf;

		ఫలితం = refs_for_each_ref_ext(get_main_ref_store(the_repository),
					       rename_one_ref, &rename, &opts);
		ఒకవేళ (ఫలితం < 0)
			die(_("queueing remote ref renames failed: %s"), rename.err->buf);

		ఫలితం = ref_transaction_prepare(rename.transaction, &err);
		ఒకవేళ (ఫలితం < 0) {
			error("renaming remote references failed: %s", err.buf);
			ఒకవేళ (ఫలితం == REF_TRANSACTION_ERROR_NAME_CONFLICT)
				advise(conflicting_remote_refs_advice);
			die(ఖాళీ);
		}
	}

	ఒకవేళ (oldremote->fetch.nr) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.fetch", rename.new_name);
		repo_config_సమితి_multivar(the_repository, buf.buf, ఖాళీ, ఖాళీ, CONFIG_FLAGS_MULTI_REPLACE);
		కొరకు (పూర్ణాంకం i = 0; i < oldremote->fetch.nr; i++) {
			అక్షరం *ptr;

			strbuf_reset(&buf2);
			strbuf_addstr(&buf2, oldremote->fetch.వస్తువులు[i].raw);
			ptr = అక్షరశ్రేణి_శోధన(buf2.buf, old_remote_context.buf);
			ఒకవేళ (ptr) {
				strbuf_splice(&buf2,
					      ptr-buf2.buf + అక్షరశ్రేణి_పొడవు(":refs/remotes/"),
					      అక్షరశ్రేణి_పొడవు(rename.old_name), rename.new_name,
					      అక్షరశ్రేణి_పొడవు(rename.new_name));
			} లేకపోతే
				warning(_("Not updating non-default fetch refspec\n"
					  "\t%s\n"
					  "\tPlease update the configuration manually if necessary."),
					buf2.buf);

			repo_config_సమితి_multivar(the_repository, buf.buf, buf2.buf, "^$", 0);
		}
	}

	read_branches();
	కొరకు (కొలత_t i = 0; i < branch_list.nr; i++) {
		నిర్మాణం string_list_అంశం *అంశం = branch_list.వస్తువులు + i;
		నిర్మాణం branch_info *info = అంశం->util;
		ఒకవేళ (info->remote_name && !strcmp(info->remote_name, rename.old_name)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "branch.%s.remote", అంశం->string);
			repo_config_సమితి(the_repository, buf.buf, rename.new_name);
		}
		ఒకవేళ (info->నెట్టు_remote_name && !strcmp(info->నెట్టు_remote_name, rename.old_name)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "branch.%s.pushRemote", అంశం->string);
			repo_config_సమితి(the_repository, buf.buf, rename.new_name);
		}
	}

	ఒకవేళ (refspecs_need_update) {
		ఫలితం = ref_transaction_commit(rename.transaction, &err);
		ఒకవేళ (ఫలితం < 0)
			die(_("renaming remote refs failed: %s"), rename.err->buf);

		stop_పురోగతి(&rename.పురోగతి);

		handle_నెట్టు_default(rename.old_name, rename.new_name);
	}

బహిర్గతం:
	ref_transaction_free(rename.transaction);
	strbuf_release(&old_remote_context);
	strbuf_release(&buf);
	strbuf_release(&buf2);
	strbuf_release(&buf3);
	strbuf_release(&err);
	తిరిగి ఫలితం;
}

స్థిర పూర్ణాంకం rm(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
	      నిర్మాణం repository *repo వాడని)
{
	నిర్మాణం option options[] = {
		OPT_చివర()
	};
	నిర్మాణం remote *remote;
	నిర్మాణం strbuf buf = STRBUF_INIT;
	నిర్మాణం తెలిసిన_remotes తెలిసిన_remotes = { ఖాళీ, ఖాళీ };
	నిర్మాణం string_list branches = STRING_LIST_INIT_DUP;
	నిర్మాణం string_list skipped = STRING_LIST_INIT_DUP;
	నిర్మాణం branches_for_remote cb_దత్తాంశం;
	పూర్ణాంకం ఫలితం;

	memset(&cb_దత్తాంశం, 0, పరిమాణం(cb_దత్తాంశం));
	cb_దత్తాంశం.branches = &branches;
	cb_దత్తాంశం.skipped = &skipped;
	cb_దత్తాంశం.keep = &తెలిసిన_remotes;

	argc = parse_options(argc, argv, ఉపసర్గ, options,
			     builtin_remote_rm_usage, 0);
	ఒకవేళ (argc != 1)
		usage_with_options(builtin_remote_rm_usage, options);

	remote = remote_get(argv[0]);
	ఒకవేళ (!remote_is_configured(remote, 1)) {
		error(_("No such remote: '%s'"), argv[0]);
		నిష్క్రమించు(2);
	}

	తెలిసిన_remotes.గమ్యం_delete = remote;
	for_each_remote(కూడు_తెలిసిన_remote, &తెలిసిన_remotes);

	read_branches();
	కొరకు (కొలత_t i = 0; i < branch_list.nr; i++) {
		నిర్మాణం string_list_అంశం *అంశం = branch_list.వస్తువులు + i;
		నిర్మాణం branch_info *info = అంశం->util;
		ఒకవేళ (info->remote_name && !strcmp(info->remote_name, remote->name)) {
			స్థిరరాశి అక్షరం *కీలకాలు[] = { "remote", "merge", ఖాళీ }, **k;
			కొరకు (k = కీలకాలు; *k; k++) {
				strbuf_reset(&buf);
				strbuf_addf(&buf, "branch.%s.%s",
						అంశం->string, *k);
				ఫలితం = repo_config_సమితి_gently(the_repository, buf.buf, ఖాళీ);
				ఒకవేళ (ఫలితం && ఫలితం != CONFIG_NOTHING_సమితి)
					die(_("could not unset '%s'"), buf.buf);
			}
		}
		ఒకవేళ (info->నెట్టు_remote_name && !strcmp(info->నెట్టు_remote_name, remote->name)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "branch.%s.pushremote", అంశం->string);
			ఫలితం = repo_config_సమితి_gently(the_repository, buf.buf, ఖాళీ);
			ఒకవేళ (ఫలితం && ఫలితం != CONFIG_NOTHING_సమితి)
				die(_("could not unset '%s'"), buf.buf);
		}
	}

	/*
	 * We cannot just pass a function to for_each_ref() which deletes
	 * the branches one by one, since for_each_ref() relies on cached
	 * refs, which are invalidated when deleting a branch.
	 */
	cb_దత్తాంశం.remote = remote;
	ఫలితం = refs_for_each_ref(get_main_ref_store(the_repository),
				   కూడు_branch_for_removal, &cb_దత్తాంశం);
	strbuf_release(&buf);

	ఒకవేళ (!ఫలితం)
		ఫలితం = refs_delete_refs(get_main_ref_store(the_repository),
					  "remote: remove", &branches,
					  REF_NO_DEREF);
	string_list_clear(&branches, 0);

	ఒకవేళ (skipped.nr) {
		fprintf_ln(stderr,
			   Q_("Note: A branch outside the refs/remotes/ hierarchy was not removed;\n"
			      "to delete it, use:",
			      "Note: Some branches outside the refs/remotes/ hierarchy were not removed;\n"
			      "to delete them, use:",
			      skipped.nr));
		కొరకు (కొలత_t i = 0; i < skipped.nr; i++)
			fprintf(stderr, "  git branch -d %s\n",
				skipped.వస్తువులు[i].string);
	}
	string_list_clear(&skipped, 0);

	ఒకవేళ (!ఫలితం) {
		strbuf_addf(&buf, "remote.%s", remote->name);
		ఒకవేళ (repo_config_rename_section(the_repository, buf.buf, ఖాళీ) < 1) {
			ఫలితం = error(_("Could not remove config section '%s'"), buf.buf);
			వెళ్ళు బహిర్గతం;
		}

		handle_నెట్టు_default(remote->name, ఖాళీ);
	}

బహిర్గతం:
	కొరకు (నిర్మాణం తెలిసిన_remote *r = తెలిసిన_remotes.list; r;) {
		నిర్మాణం తెలిసిన_remote *తదుపరి = r->తదుపరి;
		విడుదల(r);
		r = తదుపరి;
	}
	strbuf_release(&buf);
	తిరిగి ఫలితం;
}

స్థిర శూన్య clear_నెట్టు_info(శూన్య *util, స్థిరరాశి అక్షరం *string వాడని)
{
	నిర్మాణం నెట్టు_info *info = util;
	విడుదల(info->dest);
	విడుదల(info);
}

స్థిర శూన్య free_remote_ref_స్థితులు(నిర్మాణం ref_స్థితులు *స్థితులు)
{
	string_list_clear(&స్థితులు->new_refs, 0);
	string_list_clear(&స్థితులు->skipped, 0);
	string_list_clear(&స్థితులు->stale, 1);
	string_list_clear(&స్థితులు->tracked, 0);
	string_list_clear(&స్థితులు->heads, 0);
	string_list_clear_func(&స్థితులు->నెట్టు, clear_నెట్టు_info);
}

స్థిర పూర్ణాంకం append_ref_గమ్యం_tracked_list(స్థిరరాశి నిర్మాణం reference *ref, శూన్య *cb_దత్తాంశం)
{
	నిర్మాణం ref_స్థితులు *స్థితులు = cb_దత్తాంశం;
	నిర్మాణం refspec_అంశం refspec;

	ఒకవేళ (ref->flags & REF_ISSYMREF)
		తిరిగి 0;

	memset(&refspec, 0, పరిమాణం(refspec));
	refspec.dst = (అక్షరం *)ref->name;
	ఒకవేళ (!remote_కనుగొను_tracking(స్థితులు->remote, &refspec)) {
		string_list_append(&స్థితులు->tracked, abbrev_branch(refspec.src));
		విడుదల(refspec.src);
	}

	తిరిగి 0;
}

స్థిర పూర్ణాంకం get_remote_ref_స్థితులు(స్థిరరాశి అక్షరం *name,
				 నిర్మాణం ref_స్థితులు *స్థితులు,
				 పూర్ణాంకం query)
{
	స్థితులు->remote = remote_get(name);
	ఒకవేళ (!స్థితులు->remote)
		తిరిగి error(_("No such remote: '%s'"), name);

	read_branches();

	ఒకవేళ (query) {
		నిర్మాణం transport *transport;
		స్థిరరాశి నిర్మాణం ref *remote_refs;

		transport = transport_get(స్థితులు->remote, స్థితులు->remote->url.v[0]);
		remote_refs = transport_get_remote_refs(transport, ఖాళీ);

		స్థితులు->queried = 1;
		ఒకవేళ (query & GET_REF_స్థితులు)
			get_ref_స్థితులు(remote_refs, స్థితులు);
		ఒకవేళ (query & GET_తల_పేర్లు)
			get_తల_పేర్లు(remote_refs, స్థితులు);
		ఒకవేళ (query & GET_నెట్టు_REF_స్థితులు)
			get_నెట్టు_ref_స్థితులు(remote_refs, స్థితులు);
		transport_disconnect(transport);
	} లేకపోతే {
		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_ref_గమ్యం_tracked_list, స్థితులు);
		string_list_క్రమీకరించు(&స్థితులు->tracked);
		get_నెట్టు_ref_స్థితులు_noquery(స్థితులు);
	}

	తిరిగి 0;
}

నిర్మాణం show_info {
	నిర్మాణం string_list list;
	నిర్మాణం ref_స్థితులు స్థితులు;
	పూర్ణాంకం వెడల్పు, width2;
	పూర్ణాంకం any_rebase;
};

#నిర్వచించు SHOW_INFO_INIT { \
	.list = STRING_LIST_INIT_DUP, \
	.స్థితులు = REF_స్థితులు_INIT, \
}

స్థిర పూర్ణాంకం కూడు_remote_గమ్యం_show_info(నిర్మాణం string_list_అంశం *అంశం, శూన్య *cb_దత్తాంశం)
{
	నిర్మాణం show_info *info = cb_దత్తాంశం;
	పూర్ణాంకం n = అక్షరశ్రేణి_పొడవు(అంశం->string);
	ఒకవేళ (n > info->వెడల్పు)
		info->వెడల్పు = n;
	string_list_చొప్పించు(&info->list, అంశం->string);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం show_remote_info_అంశం(నిర్మాణం string_list_అంశం *అంశం, శూన్య *cb_దత్తాంశం)
{
	నిర్మాణం show_info *info = cb_దత్తాంశం;
	నిర్మాణం ref_స్థితులు *స్థితులు = &info->స్థితులు;
	స్థిరరాశి అక్షరం *name = అంశం->string;

	ఒకవేళ (స్థితులు->queried) {
		స్థిరరాశి అక్షరం *fmt = "%s";
		స్థిరరాశి అక్షరం *arg = "";
		ఒకవేళ (string_list_has_string(&స్థితులు->new_refs, name)) {
			fmt = _(" new (next fetch will store in remotes/%s)");
			arg = స్థితులు->remote->name;
		} లేకపోతే ఒకవేళ (string_list_has_string(&స్థితులు->tracked, name))
			arg = _(" tracked");
		లేకపోతే ఒకవేళ (string_list_has_string(&స్థితులు->skipped, name))
			arg = _(" skipped");
		లేకపోతే ఒకవేళ (string_list_has_string(&స్థితులు->stale, name))
			arg = _(" stale (use 'git remote prune' to remove)");
		లేకపోతే
			arg = _(" ???");
		రాయి("    %-*s", info->వెడల్పు, name);
		రాయి(fmt, arg);
		రాయి("\n");
	} లేకపోతే
		రాయి("    %s\n", name);

	తిరిగి 0;
}

స్థిర పూర్ణాంకం కూడు_local_గమ్యం_show_info(నిర్మాణం string_list_అంశం *branch_అంశం, శూన్య *cb_దత్తాంశం)
{
	నిర్మాణం show_info *show_info = cb_దత్తాంశం;
	నిర్మాణం ref_స్థితులు *స్థితులు = &show_info->స్థితులు;
	నిర్మాణం branch_info *branch_info = branch_అంశం->util;
	నిర్మాణం string_list_అంశం *అంశం;
	పూర్ణాంకం n;

	ఒకవేళ (!branch_info->విలీనం.nr || !branch_info->remote_name ||
	    strcmp(స్థితులు->remote->name, branch_info->remote_name))
		తిరిగి 0;
	ఒకవేళ ((n = అక్షరశ్రేణి_పొడవు(branch_అంశం->string)) > show_info->వెడల్పు)
		show_info->వెడల్పు = n;
	ఒకవేళ (branch_info->rebase >= REBASE_TRUE)
		show_info->any_rebase = 1;

	అంశం = string_list_చొప్పించు(&show_info->list, branch_అంశం->string);
	అంశం->util = branch_info;

	తిరిగి 0;
}

స్థిర పూర్ణాంకం show_local_info_అంశం(నిర్మాణం string_list_అంశం *అంశం, శూన్య *cb_దత్తాంశం)
{
	నిర్మాణం show_info *show_info = cb_దత్తాంశం;
	నిర్మాణం branch_info *branch_info = అంశం->util;
	నిర్మాణం string_list *విలీనం = &branch_info->విలీనం;
	పూర్ణాంకం వెడల్పు = show_info->వెడల్పు + 4;

	ఒకవేళ (branch_info->rebase >= REBASE_TRUE && branch_info->విలీనం.nr > 1) {
		error(_("invalid branch.%s.merge; cannot rebase onto > 1 branch"),
			అంశం->string);
		తిరిగి 0;
	}

	రాయి("    %-*s ", show_info->వెడల్పు, అంశం->string);
	ఒకవేళ (branch_info->rebase >= REBASE_TRUE) {
		స్థిరరాశి అక్షరం *సందేశం;
		ఒకవేళ (branch_info->rebase == REBASE_INTERACTIVE)
			సందేశం = _("rebases interactively onto remote %s");
		లేకపోతే ఒకవేళ (branch_info->rebase == REBASE_MERGES)
			సందేశం = _("rebases interactively (with merges) onto "
				"remote %s");
		లేకపోతే
			సందేశం = _("rebases onto remote %s");
		printf_ln(సందేశం, విలీనం->వస్తువులు[0].string);
		తిరిగి 0;
	} లేకపోతే ఒకవేళ (show_info->any_rebase) {
		printf_ln(_(" merges with remote %s"), విలీనం->వస్తువులు[0].string);
		వెడల్పు++;
	} లేకపోతే {
		printf_ln(_("merges with remote %s"), విలీనం->వస్తువులు[0].string);
	}
	కొరకు (కొలత_t i = 1; i < విలీనం->nr; i++)
		రాయి(_("%-*s    and with remote %s\n"), వెడల్పు, "",
		       విలీనం->వస్తువులు[i].string);

	తిరిగి 0;
}

స్థిర పూర్ణాంకం కూడు_నెట్టు_గమ్యం_show_info(నిర్మాణం string_list_అంశం *నెట్టు_అంశం, శూన్య *cb_దత్తాంశం)
{
	నిర్మాణం show_info *show_info = cb_దత్తాంశం;
	నిర్మాణం నెట్టు_info *నెట్టు_info = నెట్టు_అంశం->util;
	నిర్మాణం string_list_అంశం *అంశం;
	పూర్ణాంకం n;
	ఒకవేళ ((n = అక్షరశ్రేణి_పొడవు(నెట్టు_అంశం->string)) > show_info->వెడల్పు)
		show_info->వెడల్పు = n;
	ఒకవేళ ((n = అక్షరశ్రేణి_పొడవు(నెట్టు_info->dest)) > show_info->width2)
		show_info->width2 = n;
	అంశం = string_list_append(&show_info->list, నెట్టు_అంశం->string);
	అంశం->util = నెట్టు_అంశం->util;
	తిరిగి 0;
}

/*
 * Sorting comparison for a string list that has push_info
 * structs in its util field
 */
స్థిర పూర్ణాంకం cmp_string_with_నెట్టు(స్థిరరాశి శూన్య *va, స్థిరరాశి శూన్య *vb)
{
	స్థిరరాశి నిర్మాణం string_list_అంశం *a = va;
	స్థిరరాశి నిర్మాణం string_list_అంశం *b = vb;
	స్థిరరాశి నిర్మాణం నెట్టు_info *a_నెట్టు = a->util;
	స్థిరరాశి నిర్మాణం నెట్టు_info *b_నెట్టు = b->util;
	పూర్ణాంకం cmp = strcmp(a->string, b->string);
	తిరిగి cmp ? cmp : strcmp(a_నెట్టు->dest, b_నెట్టు->dest);
}

స్థిర పూర్ణాంకం show_నెట్టు_info_అంశం(నిర్మాణం string_list_అంశం *అంశం, శూన్య *cb_దత్తాంశం)
{
	నిర్మాణం show_info *show_info = cb_దత్తాంశం;
	నిర్మాణం నెట్టు_info *నెట్టు_info = అంశం->util;
	స్థిరరాశి అక్షరం *src = అంశం->string, *స్థితి_2 = ఖాళీ;

	ఎంపిక (నెట్టు_info->స్థితి_2) {
	స్థితి నెట్టు_స్థితి_CREATE:
		స్థితి_2 = _("create");
		ఆపు;
	స్థితి నెట్టు_స్థితి_DELETE:
		స్థితి_2 = _("delete");
		src = _("(none)");
		ఆపు;
	స్థితి నెట్టు_స్థితి_UPTODATE:
		స్థితి_2 = _("up to date");
		ఆపు;
	స్థితి నెట్టు_స్థితి_FASTFORWARD:
		స్థితి_2 = _("fast-forwardable");
		ఆపు;
	స్థితి నెట్టు_స్థితి_OUTOFDATE:
		స్థితి_2 = _("local out of date");
		ఆపు;
	స్థితి నెట్టు_స్థితి_NOTQUERIED:
		ఆపు;
	}
	ఒకవేళ (స్థితి_2) {
		ఒకవేళ (నెట్టు_info->forced)
			printf_ln(_("    %-*s forces to %-*s (%s)"), show_info->వెడల్పు, src,
			       show_info->width2, నెట్టు_info->dest, స్థితి_2);
		లేకపోతే
			printf_ln(_("    %-*s pushes to %-*s (%s)"), show_info->వెడల్పు, src,
			       show_info->width2, నెట్టు_info->dest, స్థితి_2);
	} లేకపోతే {
		ఒకవేళ (నెట్టు_info->forced)
			printf_ln(_("    %-*s forces to %s"), show_info->వెడల్పు, src,
			       నెట్టు_info->dest);
		లేకపోతే
			printf_ln(_("    %-*s pushes to %s"), show_info->వెడల్పు, src,
			       నెట్టు_info->dest);
	}
	తిరిగి 0;
}

స్థిర పూర్ణాంకం get_one_నమోదు(నిర్మాణం remote *remote, శూన్య *priv)
{
	నిర్మాణం string_list *list = priv;
	నిర్మాణం strbuf remote_info_buf = STRBUF_INIT;
	నిర్మాణం strvec *url;

	ఒకవేళ (remote->url.nr > 0) {
		నిర్మాణం strbuf promisor_config = STRBUF_INIT;
		స్థిరరాశి అక్షరం *partial_clone_filter = ఖాళీ;

		strbuf_addf(&promisor_config, "remote.%s.partialclonefilter", remote->name);
		strbuf_addf(&remote_info_buf, "%s (fetch)", remote->url.v[0]);
		ఒకవేళ (!repo_config_get_string_tmp(the_repository, promisor_config.buf, &partial_clone_filter))
			strbuf_addf(&remote_info_buf, " [%s]", partial_clone_filter);

		strbuf_release(&promisor_config);
		string_list_append(list, remote->name)->util =
				strbuf_detach(&remote_info_buf, ఖాళీ);
	} లేకపోతే
		string_list_append(list, remote->name)->util = ఖాళీ;
	url = నెట్టు_url_యొక్క_remote(remote);
	కొరకు (కొలత_t i = 0; i < url->nr; i++) {
		strbuf_addf(&remote_info_buf, "%s (push)", url->v[i]);
		string_list_append(list, remote->name)->util =
				strbuf_detach(&remote_info_buf, ఖాళీ);
	}

	తిరిగి 0;
}

స్థిర పూర్ణాంకం show_all(శూన్య)
{
	నిర్మాణం string_list list = STRING_LIST_INIT_DUP;
	పూర్ణాంకం ఫలితం;

	ఫలితం = for_each_remote(get_one_నమోదు, &list);

	ఒకవేళ (!ఫలితం) {
		string_list_క్రమీకరించు(&list);
		కొరకు (కొలత_t i = 0; i < list.nr; i++) {
			నిర్మాణం string_list_అంశం *అంశం = list.వస్తువులు + i;
			ఒకవేళ (verbose)
				రాయి("%s\t%s\n", అంశం->string,
					అంశం->util ? (స్థిరరాశి అక్షరం *)అంశం->util : "");
			లేకపోతే {
				ఒకవేళ (i && !strcmp((అంశం - 1)->string, అంశం->string))
					కొనసాగు;
				రాయి("%s\n", అంశం->string);
			}
		}
	}
	string_list_clear(&list, 1);
	తిరిగి ఫలితం;
}

స్థిర పూర్ణాంకం show(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
		నిర్మాణం repository *repo వాడని)
{
	పూర్ణాంకం no_query = 0, ఫలితం = 0, query_flag = 0;
	నిర్మాణం option options[] = {
		OPT_BOOL('n', ఖాళీ, &no_query, N_("do not query remotes")),
		OPT_చివర()
	};
	నిర్మాణం show_info info = SHOW_INFO_INIT;

	argc = parse_options(argc, argv, ఉపసర్గ, options,
			     builtin_remote_show_usage,
			     0);

	ఒకవేళ (argc < 1)
		తిరిగి show_all();

	ఒకవేళ (!no_query)
		query_flag = (GET_REF_స్థితులు | GET_తల_పేర్లు | GET_నెట్టు_REF_స్థితులు);

	కొరకు (; argc; argc--, argv++) {
		కొలత_t i;
		నిర్మాణం strvec *url;

		get_remote_ref_స్థితులు(*argv, &info.స్థితులు, query_flag);

		printf_ln(_("* remote %s"), *argv);
		printf_ln(_("  Fetch URL: %s"), info.స్థితులు.remote->url.v[0]);
		url = నెట్టు_url_యొక్క_remote(info.స్థితులు.remote);
		కొరకు (i = 0; i < url->nr; i++)
			/*
			 * TRANSLATORS: the colon ':' should align
			 * with the one in " Fetch URL: %s"
			 * translation.
			 */
			printf_ln(_("  Push  URL: %s"), url->v[i]);
		ఒకవేళ (!i)
			printf_ln(_("  Push  URL: %s"), _("(no URL)"));
		ఒకవేళ (no_query)
			printf_ln(_("  HEAD branch: %s"), _("(not queried)"));
		లేకపోతే ఒకవేళ (!info.స్థితులు.heads.nr)
			printf_ln(_("  HEAD branch: %s"), _("(unknown)"));
		లేకపోతే ఒకవేళ (info.స్థితులు.heads.nr == 1)
			printf_ln(_("  HEAD branch: %s"), info.స్థితులు.heads.వస్తువులు[0].string);
		లేకపోతే {
			రాయి(_("  HEAD branch (remote HEAD is ambiguous,"
				 " may be one of the following):\n"));
			కొరకు (i = 0; i < info.స్థితులు.heads.nr; i++)
				రాయి("    %s\n", info.స్థితులు.heads.వస్తువులు[i].string);
		}

		/* remote branch info */
		info.వెడల్పు = 0;
		for_each_string_list(&info.స్థితులు.new_refs, కూడు_remote_గమ్యం_show_info, &info);
		for_each_string_list(&info.స్థితులు.skipped, కూడు_remote_గమ్యం_show_info, &info);
		for_each_string_list(&info.స్థితులు.tracked, కూడు_remote_గమ్యం_show_info, &info);
		for_each_string_list(&info.స్థితులు.stale, కూడు_remote_గమ్యం_show_info, &info);
		ఒకవేళ (info.list.nr)
			printf_ln(Q_("  Remote branch:%s",
				     "  Remote branches:%s",
				     info.list.nr),
				  no_query ? _(" (status not queried)") : "");
		for_each_string_list(&info.list, show_remote_info_అంశం, &info);
		string_list_clear(&info.list, 0);

		/* git pull info */
		info.వెడల్పు = 0;
		info.any_rebase = 0;
		for_each_string_list(&branch_list, కూడు_local_గమ్యం_show_info, &info);
		ఒకవేళ (info.list.nr)
			printf_ln(Q_("  Local branch configured for 'git pull':",
				     "  Local branches configured for 'git pull':",
				     info.list.nr));
		for_each_string_list(&info.list, show_local_info_అంశం, &info);
		string_list_clear(&info.list, 0);

		/* git push info */
		ఒకవేళ (info.స్థితులు.remote->mirror)
			printf_ln(_("  Local refs will be mirrored by 'git push'"));

		info.వెడల్పు = info.width2 = 0;
		for_each_string_list(&info.స్థితులు.నెట్టు, కూడు_నెట్టు_గమ్యం_show_info, &info);
		QSORT(info.list.వస్తువులు, info.list.nr, cmp_string_with_నెట్టు);
		ఒకవేళ (info.list.nr)
			printf_ln(Q_("  Local ref configured for 'git push'%s:",
				     "  Local refs configured for 'git push'%s:",
				     info.list.nr),
				  no_query ? _(" (status not queried)") : "");
		for_each_string_list(&info.list, show_నెట్టు_info_అంశం, &info);
		string_list_clear(&info.list, 0);

		free_remote_ref_స్థితులు(&info.స్థితులు);
	}

	తిరిగి ఫలితం;
}

స్థిర శూన్య report_సమితి_తల_auto(స్థిరరాశి అక్షరం *remote, స్థిరరాశి అక్షరం *తల_name,
			నిర్మాణం strbuf *b_local_తల, పూర్ణాంకం was_detached) {
	నిర్మాణం strbuf buf_ఉపసర్గ = STRBUF_INIT;
	స్థిరరాశి అక్షరం *prev_తల = ఖాళీ;

	strbuf_addf(&buf_ఉపసర్గ, "refs/remotes/%s/", remote);
	skip_ఉపసర్గ(b_local_తల->buf, buf_ఉపసర్గ.buf, &prev_తల);

	ఒకవేళ (prev_తల && !strcmp(prev_తల, తల_name))
		రాయి(_("'%s/HEAD' is unchanged and points to '%s'\n"),
			remote, తల_name);
	లేకపోతే ఒకవేళ (prev_తల)
		రాయి(_("'%s/HEAD' has changed from '%s' and now points to '%s'\n"),
			remote, prev_తల, తల_name);
	లేకపోతే ఒకవేళ (!b_local_తల->len)
		రాయి(_("'%s/HEAD' is now created and points to '%s'\n"),
			remote, తల_name);
	లేకపోతే ఒకవేళ (was_detached && b_local_తల->len)
		రాయి(_("'%s/HEAD' was detached at '%s' and now points to '%s'\n"),
			remote, b_local_తల->buf, తల_name);
	లేకపోతే
		రాయి(_("'%s/HEAD' used to point to '%s' "
			"(which is not a remote branch), but now points to '%s'\n"),
			remote, b_local_తల->buf, తల_name);
	strbuf_release(&buf_ఉపసర్గ);
}

స్థిర పూర్ణాంకం సమితి_తల(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
		    నిర్మాణం repository *repo వాడని)
{
	పూర్ణాంకం opt_a = 0, opt_d = 0, ఫలితం = 0, was_detached;
	నిర్మాణం strbuf b_తల = STRBUF_INIT, b_remote_తల = STRBUF_INIT,
		b_local_తల = STRBUF_INIT;
	అక్షరం *తల_name = ఖాళీ;
	నిర్మాణం ref_store *refs = get_main_ref_store(the_repository);
	నిర్మాణం remote *remote;

	నిర్మాణం option options[] = {
		OPT_BOOL('a', "auto", &opt_a,
			 N_("set refs/remotes/<name>/HEAD according to remote")),
		OPT_BOOL('d', "delete", &opt_d,
			 N_("delete refs/remotes/<name>/HEAD")),
		OPT_చివర()
	};
	argc = parse_options(argc, argv, ఉపసర్గ, options,
			     builtin_remote_sethead_usage, 0);

	/* All modes require at least a remote name. */
	ఒకవేళ (!argc)
		usage_with_options(builtin_remote_sethead_usage, options);

	strbuf_addf(&b_తల, "refs/remotes/%s/HEAD", argv[0]);
	remote = remote_get(argv[0]);

	ఒకవేళ (!opt_a && !opt_d && argc == 2) {
		తల_name = xstrdup(argv[1]);
	} లేకపోతే ఒకవేళ (opt_a && !opt_d && argc == 1) {
		నిర్మాణం ref_స్థితులు స్థితులు = REF_స్థితులు_INIT;
		get_remote_ref_స్థితులు(argv[0], &స్థితులు, GET_తల_పేర్లు);
		ఒకవేళ (!స్థితులు.heads.nr)
			ఫలితం |= error(_("Cannot determine remote HEAD"));
		లేకపోతే ఒకవేళ (స్థితులు.heads.nr > 1) {
			ఫలితం |= error(_("Multiple remote HEAD branches. "
					  "Please choose one explicitly with:"));
			కొరకు (కొలత_t i = 0; i < స్థితులు.heads.nr; i++)
				fprintf(stderr, "  git remote set-head %s %s\n",
					argv[0], స్థితులు.heads.వస్తువులు[i].string);
		} లేకపోతే
			తల_name = xstrdup(స్థితులు.heads.వస్తువులు[0].string);
		free_remote_ref_స్థితులు(&స్థితులు);
	} లేకపోతే ఒకవేళ (opt_d && !opt_a && argc == 1) {
		ఒకవేళ (refs_delete_ref(refs, ఖాళీ, b_తల.buf, ఖాళీ, REF_NO_DEREF))
			ఫలితం |= error(_("Could not delete %s"), b_తల.buf);
	} లేకపోతే
		usage_with_options(builtin_remote_sethead_usage, options);

	ఒకవేళ (!తల_name)
		వెళ్ళు cleanup;
	strbuf_addf(&b_remote_తల, "refs/remotes/%s/%s", argv[0], తల_name);
	ఒకవేళ (!refs_ref_exists(refs, b_remote_తల.buf)) {
		ఫలితం |= error(_("Not a valid ref: %s"), b_remote_తల.buf);
		వెళ్ళు cleanup;
	}
	was_detached = refs_update_symref_extended(refs, b_తల.buf, b_remote_తల.buf,
			"remote set-head", &b_local_తల, 0);
	ఒకవేళ (was_detached == -1) {
		ఫలితం |= error(_("Could not set up %s"), b_తల.buf);
		వెళ్ళు cleanup;
	}
	ఒకవేళ (opt_a)
		report_సమితి_తల_auto(argv[0], తల_name, &b_local_తల, was_detached);
	ఒకవేళ (remote->follow_remote_తల == FOLLOW_REMOTE_ALWAYS) {
		నిర్మాణం strbuf config_name = STRBUF_INIT;
		strbuf_addf(&config_name,
			"remote.%s.followremotehead", remote->name);
		repo_config_సమితి(the_repository, config_name.buf, "warn");
		strbuf_release(&config_name);
	}

cleanup:
	విడుదల(తల_name);
	strbuf_release(&b_తల);
	strbuf_release(&b_remote_తల);
	strbuf_release(&b_local_తల);
	తిరిగి ఫలితం;
}

స్థిర పూర్ణాంకం prune_remote(స్థిరరాశి అక్షరం *remote, పూర్ణాంకం dry_నిరంతరం)
{
	పూర్ణాంకం ఫలితం = 0;
	నిర్మాణం ref_స్థితులు స్థితులు = REF_స్థితులు_INIT;
	నిర్మాణం string_list refs_గమ్యం_prune = STRING_LIST_INIT_NODUP;
	నిర్మాణం string_list_అంశం *అంశం;

	get_remote_ref_స్థితులు(remote, &స్థితులు, GET_REF_స్థితులు);

	ఒకవేళ (!స్థితులు.stale.nr) {
		free_remote_ref_స్థితులు(&స్థితులు);
		తిరిగి 0;
	}

	printf_ln(_("Pruning %s"), remote);
	printf_ln(_("URL: %s"), స్థితులు.remote->url.v[0]);

	for_each_string_list_అంశం(అంశం, &స్థితులు.stale)
		string_list_append(&refs_గమ్యం_prune, అంశం->util);
	string_list_క్రమీకరించు(&refs_గమ్యం_prune);

	ఒకవేళ (!dry_నిరంతరం)
		ఫలితం |= refs_delete_refs(get_main_ref_store(the_repository),
					   "remote: prune", &refs_గమ్యం_prune, 0);

	for_each_string_list_అంశం(అంశం, &స్థితులు.stale) {
		స్థిరరాశి అక్షరం *refname = అంశం->util;

		ఒకవేళ (dry_నిరంతరం)
			printf_ln(_(" * [would prune] %s"),
			       abbrev_ref(refname, "refs/remotes/"));
		లేకపోతే
			printf_ln(_(" * [pruned] %s"),
			       abbrev_ref(refname, "refs/remotes/"));
	}

	refs_warn_dangling_symrefs(get_main_ref_store(the_repository),
				   stdout, " ", dry_నిరంతరం, &refs_గమ్యం_prune);

	string_list_clear(&refs_గమ్యం_prune, 0);
	free_remote_ref_స్థితులు(&స్థితులు);
	తిరిగి ఫలితం;
}

స్థిర పూర్ణాంకం prune(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
		 నిర్మాణం repository *repo వాడని)
{
	పూర్ణాంకం dry_నిరంతరం = 0, ఫలితం = 0;
	నిర్మాణం option options[] = {
		OPT_DRY_నిరంతరం(&dry_నిరంతరం, N_("dry run")),
		OPT_చివర()
	};

	argc = parse_options(argc, argv, ఉపసర్గ, options,
			     builtin_remote_prune_usage, 0);

	ఒకవేళ (argc < 1)
		usage_with_options(builtin_remote_prune_usage, options);

	కొరకు (; argc; argc--, argv++)
		ఫలితం |= prune_remote(*argv, dry_నిరంతరం);

	తిరిగి ఫలితం;
}

స్థిర పూర్ణాంకం get_remote_default(స్థిరరాశి అక్షరం *కీలకం, స్థిరరాశి అక్షరం *విలువ వాడని,
			      స్థిరరాశి నిర్మాణం config_context *ctx వాడని,
			      శూన్య *priv)
{
	ఒకవేళ (strcmp(కీలకం, "remotes.default") == 0) {
		పూర్ణాంకం *దొరికింది_2 = priv;
		*దొరికింది_2 = 1;
	}
	తిరిగి 0;
}

స్థిర పూర్ణాంకం update(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
		  నిర్మాణం repository *repo వాడని)
{
	పూర్ణాంకం i, prune = -1;
	నిర్మాణం option options[] = {
		OPT_BOOL('p', "prune", &prune,
			 N_("prune remotes after fetching")),
		OPT_చివర()
	};
	నిర్మాణం బిడ్డ_process cmd = బిడ్డ_PROCESS_INIT;
	పూర్ణాంకం default_defined = 0;

	argc = parse_options(argc, argv, ఉపసర్గ, options,
			     builtin_remote_update_usage,
			     PARSE_OPT_KEEP_ARGV0);

	strvec_నెట్టు(&cmd.వాదనలు, "fetch");

	ఒకవేళ (prune != -1)
		strvec_నెట్టు(&cmd.వాదనలు, prune ? "--prune" : "--no-prune");
	ఒకవేళ (verbose)
		strvec_నెట్టు(&cmd.వాదనలు, "-v");
	strvec_నెట్టు(&cmd.వాదనలు, "--multiple");
	ఒకవేళ (argc < 2)
		strvec_నెట్టు(&cmd.వాదనలు, "default");
	కొరకు (i = 1; i < argc; i++)
		strvec_నెట్టు(&cmd.వాదనలు, argv[i]);

	ఒకవేళ (strcmp(cmd.వాదనలు.v[cmd.వాదనలు.nr-1], "default") == 0) {
		repo_config(the_repository, get_remote_default, &default_defined);
		ఒకవేళ (!default_defined) {
			strvec_తీయి(&cmd.వాదనలు);
			strvec_నెట్టు(&cmd.వాదనలు, "--all");
		}
	}

	cmd.git_cmd = 1;
	తిరిగి నిరంతరం_command(&cmd);
}

స్థిర పూర్ణాంకం remove_all_fetch_refspecs(స్థిరరాశి అక్షరం *కీలకం)
{
	తిరిగి repo_config_సమితి_multivar_gently(the_repository, కీలకం, ఖాళీ, ఖాళీ,
					       CONFIG_FLAGS_MULTI_REPLACE);
}

స్థిర శూన్య కూడు_branches(నిర్మాణం remote *remote, స్థిరరాశి అక్షరం **branches,
			 స్థిరరాశి అక్షరం *కీలకం)
{
	స్థిరరాశి అక్షరం *remotename = remote->name;
	పూర్ణాంకం mirror = remote->mirror;
	నిర్మాణం strbuf refspec = STRBUF_INIT;

	కొరకు (; *branches; branches++)
		కూడు_branch(కీలకం, *branches, remotename, mirror, &refspec);

	strbuf_release(&refspec);
}

స్థిర పూర్ణాంకం సమితి_remote_branches(స్థిరరాశి అక్షరం *remotename, స్థిరరాశి అక్షరం **branches,
				పూర్ణాంకం కూడు_బహుళకం)
{
	నిర్మాణం strbuf కీలకం = STRBUF_INIT;
	నిర్మాణం remote *remote;

	strbuf_addf(&కీలకం, "remote.%s.fetch", remotename);

	remote = remote_get(remotename);
	ఒకవేళ (!remote_is_configured(remote, 1)) {
		error(_("No such remote '%s'"), remotename);
		నిష్క్రమించు(2);
	}

	ఒకవేళ (!కూడు_బహుళకం && remove_all_fetch_refspecs(కీలకం.buf)) {
		strbuf_release(&కీలకం);
		తిరిగి 1;
	}
	కూడు_branches(remote, branches, కీలకం.buf);

	strbuf_release(&కీలకం);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం సమితి_branches(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
			నిర్మాణం repository *repo వాడని)
{
	పూర్ణాంకం కూడు_బహుళకం = 0;
	నిర్మాణం option options[] = {
		OPT_BOOL('\0', "add", &కూడు_బహుళకం, N_("add branch")),
		OPT_చివర()
	};

	argc = parse_options(argc, argv, ఉపసర్గ, options,
			     builtin_remote_setbranches_usage, 0);
	ఒకవేళ (argc == 0) {
		error(_("no remote specified"));
		usage_with_options(builtin_remote_setbranches_usage, options);
	}
	argv[argc] = ఖాళీ;

	తిరిగి సమితి_remote_branches(argv[0], argv + 1, కూడు_బహుళకం);
}

స్థిర పూర్ణాంకం get_url(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
		   నిర్మాణం repository *repo వాడని)
{
	పూర్ణాంకం నెట్టు_బహుళకం = 0, all_బహుళకం = 0;
	స్థిరరాశి అక్షరం *remotename = ఖాళీ;
	నిర్మాణం remote *remote;
	నిర్మాణం strvec *url;
	నిర్మాణం option options[] = {
		OPT_BOOL('\0', "push", &నెట్టు_బహుళకం,
			 N_("query push URLs rather than fetch URLs")),
		OPT_BOOL('\0', "all", &all_బహుళకం,
			 N_("return all URLs")),
		OPT_చివర()
	};
	argc = parse_options(argc, argv, ఉపసర్గ, options,
			     builtin_remote_geturl_usage, 0);

	ఒకవేళ (argc != 1)
		usage_with_options(builtin_remote_geturl_usage, options);

	remotename = argv[0];

	remote = remote_get(remotename);
	ఒకవేళ (!remote_is_configured(remote, 1)) {
		error(_("No such remote '%s'"), remotename);
		నిష్క్రమించు(2);
	}

	url = నెట్టు_బహుళకం ? నెట్టు_url_యొక్క_remote(remote) : &remote->url;

	ఒకవేళ (all_బహుళకం) {
		కొరకు (కొలత_t i = 0; i < url->nr; i++)
			printf_ln("%s", url->v[i]);
	} లేకపోతే {
		printf_ln("%s", url->v[0]);
	}

	తిరిగి 0;
}

స్థిర పూర్ణాంకం సమితి_url(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
		   నిర్మాణం repository *repo వాడని)
{
	పూర్ణాంకం నెట్టు_బహుళకం = 0, కూడు_బహుళకం = 0, delete_బహుళకం = 0;
	పూర్ణాంకం matches = 0, negative_matches = 0;
	స్థిరరాశి అక్షరం *remotename = ఖాళీ;
	స్థిరరాశి అక్షరం *newurl = ఖాళీ;
	స్థిరరాశి అక్షరం *oldurl = ఖాళీ;
	నిర్మాణం remote *remote;
	regex_t old_regex;
	నిర్మాణం strvec *urlset;
	నిర్మాణం strbuf name_buf = STRBUF_INIT;
	నిర్మాణం option options[] = {
		OPT_BOOL('\0', "push", &నెట్టు_బహుళకం,
			 N_("manipulate push URLs")),
		OPT_BOOL('\0', "add", &కూడు_బహుళకం,
			 N_("add URL")),
		OPT_BOOL('\0', "delete", &delete_బహుళకం,
			    N_("delete URLs")),
		OPT_చివర()
	};
	argc = parse_options(argc, argv, ఉపసర్గ, options,
			     builtin_remote_seturl_usage,
			     PARSE_OPT_KEEP_ARGV0);

	ఒకవేళ (కూడు_బహుళకం && delete_బహుళకం)
		die(_("--add --delete doesn't make sense"));

	ఒకవేళ (argc < 3 || argc > 4 || ((కూడు_బహుళకం || delete_బహుళకం) && argc != 3))
		usage_with_options(builtin_remote_seturl_usage, options);

	remotename = argv[1];
	newurl = argv[2];
	ఒకవేళ (argc > 3)
		oldurl = argv[3];

	ఒకవేళ (delete_బహుళకం)
		oldurl = newurl;

	remote = remote_get(remotename);
	ఒకవేళ (!remote_is_configured(remote, 1)) {
		error(_("No such remote '%s'"), remotename);
		నిష్క్రమించు(2);
	}

	ఒకవేళ (నెట్టు_బహుళకం) {
		strbuf_addf(&name_buf, "remote.%s.pushurl", remotename);
		urlset = &remote->pushurl;
	} లేకపోతే {
		strbuf_addf(&name_buf, "remote.%s.url", remotename);
		urlset = &remote->url;
	}

	/* Special cases that add new entry. */
	ఒకవేళ ((!oldurl && !delete_బహుళకం) || కూడు_బహుళకం) {
		ఒకవేళ (కూడు_బహుళకం)
			repo_config_సమితి_multivar(the_repository, name_buf.buf, newurl,
						       "^$", 0);
		లేకపోతే
			repo_config_సమితి(the_repository, name_buf.buf, newurl);
		వెళ్ళు బహిర్గతం;
	}

	/* Old URL specified. Demand that one matches. */
	ఒకవేళ (regcomp(&old_regex, oldurl, REG_EXTENDED))
		die(_("Invalid old URL pattern: %s"), oldurl);

	కొరకు (కొలత_t i = 0; i < urlset->nr; i++)
		ఒకవేళ (!regexec(&old_regex, urlset->v[i], 0, ఖాళీ, 0))
			matches++;
		లేకపోతే
			negative_matches++;
	ఒకవేళ (!delete_బహుళకం && !matches)
		die(_("No such URL found: %s"), oldurl);
	ఒకవేళ (delete_బహుళకం && !negative_matches && !నెట్టు_బహుళకం)
		die(_("Will not delete all non-push URLs"));

	regfree(&old_regex);

	ఒకవేళ (!delete_బహుళకం)
		repo_config_సమితి_multivar(the_repository, name_buf.buf, newurl, oldurl, 0);
	లేకపోతే
		repo_config_సమితి_multivar(the_repository, name_buf.buf, ఖాళీ, oldurl,
					 CONFIG_FLAGS_MULTI_REPLACE);
బహిర్గతం:
	strbuf_release(&name_buf);
	తిరిగి 0;
}

పూర్ణాంకం cmd_remote(పూర్ణాంకం argc,
	       స్థిరరాశి అక్షరం **argv,
	       స్థిరరాశి అక్షరం *ఉపసర్గ,
	       నిర్మాణం repository *repo)
{
	parse_opt_subcommand_fn *fn = ఖాళీ;
	నిర్మాణం option options[] = {
		OPT__VERBOSE(&verbose, N_("be verbose; must be placed before a subcommand")),
		OPT_SUBCOMMAND("add", &fn, కూడు),
		OPT_SUBCOMMAND("rename", &fn, mv),
		OPT_SUBCOMMAND_F("rm", &fn, rm, ఖాళీ, PARSE_OPT_NOCOMPLETE),
		OPT_SUBCOMMAND("remove", &fn, rm),
		OPT_SUBCOMMAND("set-head", &fn, సమితి_తల),
		OPT_SUBCOMMAND("set-branches", &fn, సమితి_branches),
		OPT_SUBCOMMAND("get-url", &fn, get_url),
		OPT_SUBCOMMAND("set-url", &fn, సమితి_url),
		OPT_SUBCOMMAND("show", &fn, show),
		OPT_SUBCOMMAND("prune", &fn, prune),
		OPT_SUBCOMMAND("update", &fn, update),
		OPT_చివర()
	};

	argc = parse_options(argc, argv, ఉపసర్గ, options, builtin_remote_usage,
			     PARSE_OPT_SUBCOMMAND_OPTIONAL);

	ఒకవేళ (fn) {
		తిరిగి !!fn(argc, argv, ఉపసర్గ, repo);
	} లేకపోతే {
		ఒకవేళ (argc) {
			error(_("unknown subcommand: `%s'"), argv[0]);
			usage_with_options(builtin_remote_usage, options);
		}
		తిరిగి !!show_all();
	}
}
