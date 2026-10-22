#define USE_THE_REPOSITORY_VARIABLE

#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "advice.h"
#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ "date.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "ident.h"
#അന്തര്ഭാവ "parse-options.h"
#അന്തര്ഭാവ "path.h"
#അന്തര്ഭാവ "transport.h"
#അന്തര്ഭാവ "remote.h"
#അന്തര്ഭാവ "string-list.h"
#അന്തര്ഭാവ "strbuf.h"
#അന്തര്ഭാവ "run-command.h"
#അന്തര്ഭാവ "rebase.h"
#അന്തര്ഭാവ "refs.h"
#അന്തര്ഭാവ "refspec.h"
#അന്തര്ഭാവ "odb.h"
#അന്തര്ഭാവ "strvec.h"
#അന്തര്ഭാവ "commit-reach.h"
#അന്തര്ഭാവ "progress.h"

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം builtin_remote_usage[] = {
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
	ശൂന്യ
};

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം builtin_remote_add_usage[] = {
	N_("git remote add [<options>] <name> <url>"),
	ശൂന്യ
};

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം builtin_remote_rename_usage[] = {
	N_("git remote rename [--[no-]progress] <old> <new>"),
	ശൂന്യ
};

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം builtin_remote_rm_usage[] = {
	N_("git remote remove <name>"),
	ശൂന്യ
};

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം builtin_remote_sethead_usage[] = {
	N_("git remote set-head <name> (-a | --auto | -d | --delete | <branch>)"),
	ശൂന്യ
};

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം builtin_remote_setbranches_usage[] = {
	N_("git remote set-branches <name> <branch>..."),
	N_("git remote set-branches --add <name> <branch>..."),
	ശൂന്യ
};

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം builtin_remote_show_usage[] = {
	N_("git remote show [<options>] <name>"),
	ശൂന്യ
};

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം builtin_remote_prune_usage[] = {
	N_("git remote prune [<options>] <name>"),
	ശൂന്യ
};

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം builtin_remote_update_usage[] = {
	N_("git remote update [<options>] [<group> | <remote>]..."),
	ശൂന്യ
};

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം builtin_remote_geturl_usage[] = {
	N_("git remote get-url [--push] [--all] <name>"),
	ശൂന്യ
};

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം builtin_remote_seturl_usage[] = {
	N_("git remote set-url [--push] <name> <newurl> [<oldurl>]"),
	N_("git remote set-url --add <name> <newurl>"),
	N_("git remote set-url --delete <name> <url>"),
	ശൂന്യ
};

#define GET_REF_STATES (1<<0)
#define GET_HEAD_NAMES (1<<1)
#define GET_തള്ളുക_REF_STATES (1<<2)

സ്ഥിതം പൂർണ്ണസംഖ്യ verbose;

സ്ഥിതം പൂർണ്ണസംഖ്യ fetch_remote(സ്ഥിരം അക്ഷരം *name)
{
	ഘടന child_process cmd = CHILD_PROCESS_INIT;

	strvec_തള്ളുക(&cmd.args, "fetch");
	എങ്കിൽ (verbose)
		strvec_തള്ളുക(&cmd.args, "-v");
	strvec_തള്ളുക(&cmd.args, name);
	cmd.git_cmd = 1;
	printf_ln(_("Updating %s"), name);
	എങ്കിൽ (run_command(&cmd))
		തിരികെ error(_("Could not fetch %s"), name);
	തിരികെ 0;
}

എണ്ണൽ {
	TAGS_UNSET = 0,
	TAGS_DEFAULT = 1,
	TAGS_SET = 2
};

#define MIRROR_NONE 0
#define MIRROR_FETCH 1
#define MIRROR_തള്ളുക 2
#define MIRROR_BOTH (MIRROR_FETCH|MIRROR_തള്ളുക)

സ്ഥിതം ശൂന്യം add_branch(സ്ഥിരം അക്ഷരം *താക്കോൽ, സ്ഥിരം അക്ഷരം *branchname,
		       സ്ഥിരം അക്ഷരം *remotename, പൂർണ്ണസംഖ്യ mirror, ഘടന strbuf *tmp)
{
	strbuf_reset(tmp);
	strbuf_addch(tmp, '+');
	എങ്കിൽ (mirror)
		strbuf_addf(tmp, "refs/%s:refs/%s",
				branchname, branchname);
	അല്ലെങ്കിൽ
		strbuf_addf(tmp, "refs/heads/%s:refs/remotes/%s/%s",
				branchname, remotename, branchname);
	repo_config_set_multivar(the_repository, താക്കോൽ, tmp->buf, "^$", 0);
}

സ്ഥിതം സ്ഥിരം അക്ഷരം mirror_advice[] =
N_("--mirror is dangerous and deprecated; please\n"
   "\t use --mirror=fetch or --mirror=push instead");

സ്ഥിതം പൂർണ്ണസംഖ്യ parse_mirror_opt(സ്ഥിരം ഘടന option *opt, സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ not)
{
	unsigned *mirror = opt->മൂല്യം;
	എങ്കിൽ (not)
		*mirror = MIRROR_NONE;
	അല്ലെങ്കിൽ എങ്കിൽ (!arg) {
		warning("%s", _(mirror_advice));
		*mirror = MIRROR_BOTH;
	}
	അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(arg, "fetch"))
		*mirror = MIRROR_FETCH;
	അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(arg, "push"))
		*mirror = MIRROR_തള്ളുക;
	അല്ലെങ്കിൽ
		തിരികെ error(_("unknown --mirror argument: %s"), arg);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ check_remote_collision(ഘടന remote *remote, ശൂന്യം *data)
{
	സ്ഥിരം അക്ഷരം *name = data;
	സ്ഥിരം അക്ഷരം *p;

	എങ്കിൽ (skip_prefix(name, remote->name, &p) && *p == '/')
		die(_("remote name '%s' is a subset of existing remote '%s'"),
		    name, remote->name);
	എങ്കിൽ (skip_prefix(remote->name, name, &p) && *p == '/')
		die(_("remote name '%s' is a superset of existing remote '%s'"),
		    name, remote->name);

	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ add(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
	       ഘടന repository *repo UNUSED)
{
	പൂർണ്ണസംഖ്യ fetch = 0, fetch_tags = TAGS_DEFAULT;
	unsigned mirror = MIRROR_NONE;
	ഘടന string_list track = STRING_LIST_INIT_NODUP;
	സ്ഥിരം അക്ഷരം *master = ശൂന്യ;
	ഘടന remote *remote;
	ഘടന strbuf buf = STRBUF_INIT, buf2 = STRBUF_INIT;
	സ്ഥിരം അക്ഷരം *name, *url;
	പൂർണ്ണസംഖ്യ result = 0;

	ഘടന option options[] = {
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

	എങ്കിൽ (argc != 2)
		usage_with_options(builtin_remote_add_usage, options);

	എങ്കിൽ (mirror && master)
		die(_("specifying a master branch makes no sense with --mirror"));
	എങ്കിൽ (mirror && !(mirror & MIRROR_FETCH) && track.nr)
		die(_("specifying branches to track makes sense only with fetch mirrors"));

	name = argv[0];
	url = argv[1];

	remote = remote_get(name);
	എങ്കിൽ (remote_is_configured(remote, 1)) {
		error(_("remote %s already exists."), name);
		exit(3);
	}

	എങ്കിൽ (!valid_remote_name(name))
		die(_("'%s' is not a valid remote name"), name);

	for_each_remote(check_remote_collision, (ശൂന്യം *)name);

	strbuf_addf(&buf, "remote.%s.url", name);
	repo_config_set(the_repository, buf.buf, url);

	എങ്കിൽ (!mirror || mirror & MIRROR_FETCH) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.fetch", name);
		എങ്കിൽ (track.nr == 0)
			string_list_append(&track, "*");
		വേണ്ടി (size_t i = 0; i < track.nr; i++) {
			add_branch(buf.buf, track.ഇനങ്ങൾ[i].string,
				   name, mirror, &buf2);
		}
	}

	എങ്കിൽ (mirror & MIRROR_തള്ളുക) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.mirror", name);
		repo_config_set(the_repository, buf.buf, "true");
	}

	എങ്കിൽ (fetch_tags != TAGS_DEFAULT) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.tagOpt", name);
		repo_config_set(the_repository, buf.buf,
				fetch_tags == TAGS_SET ? "--tags" : "--no-tags");
	}

	എങ്കിൽ (fetch && fetch_remote(name)) {
		result = 1;
		പോകുക out;
	}

	എങ്കിൽ (master) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "refs/remotes/%s/HEAD", name);

		strbuf_reset(&buf2);
		strbuf_addf(&buf2, "refs/remotes/%s/%s", name, master);

		എങ്കിൽ (refs_update_symref(get_main_ref_store(the_repository), buf.buf, buf2.buf, "remote add"))
			result = error(_("Could not setup master '%s'"), master);
	}

out:
	strbuf_release(&buf);
	strbuf_release(&buf2);
	string_list_clear(&track, 0);

	തിരികെ result;
}

ഘടന branch_info {
	അക്ഷരം *remote_name;
	ഘടന string_list merge;
	എണ്ണൽ rebase_type rebase;
	അക്ഷരം *തള്ളുക_remote_name;
};

സ്ഥിതം ഘടന string_list branch_list = STRING_LIST_INIT_DUP;

സ്ഥിതം സ്ഥിരം അക്ഷരം *abbrev_ref(സ്ഥിരം അക്ഷരം *name, സ്ഥിരം അക്ഷരം *prefix)
{
	skip_prefix(name, prefix, &name);
	തിരികെ name;
}
#define abbrev_branch(name) abbrev_ref((name), "refs/heads/")

സ്ഥിതം പൂർണ്ണസംഖ്യ config_read_branches(സ്ഥിരം അക്ഷരം *താക്കോൽ, സ്ഥിരം അക്ഷരം *മൂല്യം,
				സ്ഥിരം ഘടന config_context *ctx UNUSED,
				ശൂന്യം *data UNUSED)
{
	സ്ഥിരം അക്ഷരം *orig_താക്കോൽ = താക്കോൽ;
	അക്ഷരം *name;
	ഘടന string_list_item *item;
	ഘടന branch_info *info;
	എണ്ണൽ { REMOTE, MERGE, REBASE, തള്ളുക_REMOTE } type;
	size_t താക്കോൽ_len;

	എങ്കിൽ (!starts_with(താക്കോൽ, "branch."))
		തിരികെ 0;

	താക്കോൽ += strlen("branch.");
	എങ്കിൽ (strip_suffix(താക്കോൽ, ".remote", &താക്കോൽ_len))
		type = REMOTE;
	അല്ലെങ്കിൽ എങ്കിൽ (strip_suffix(താക്കോൽ, ".merge", &താക്കോൽ_len))
		type = MERGE;
	അല്ലെങ്കിൽ എങ്കിൽ (strip_suffix(താക്കോൽ, ".rebase", &താക്കോൽ_len))
		type = REBASE;
	അല്ലെങ്കിൽ എങ്കിൽ (strip_suffix(താക്കോൽ, ".pushremote", &താക്കോൽ_len))
		type = തള്ളുക_REMOTE;
	അല്ലെങ്കിൽ
		തിരികെ 0;

	name = xmemdupz(താക്കോൽ, താക്കോൽ_len);
	item = string_list_തിരുകുക(&branch_list, name);

	എങ്കിൽ (!item->util)
		item->util = xcalloc(1, വലുപ്പം(ഘടന branch_info));
	info = item->util;
	തിരഞ്ഞെടുപ്പ് (type) {
	അവസ്ഥ REMOTE:
		എങ്കിൽ (info->remote_name)
			warning(_("more than one %s"), orig_താക്കോൽ);
		info->remote_name = xstrdup(മൂല്യം);
		നിർത്തുക;
	അവസ്ഥ MERGE: {
		സ്ഥിരം അക്ഷരം *space = strchr(മൂല്യം, ' ');
		മൂല്യം = abbrev_branch(മൂല്യം);
		ആകുമ്പോൾ (space) {
			അക്ഷരം *merge;
			merge = xstrndup(മൂല്യം, space - മൂല്യം);
			string_list_append(&info->merge, merge);
			മൂല്യം = abbrev_branch(space + 1);
			space = strchr(മൂല്യം, ' ');
		}
		string_list_append(&info->merge, xstrdup(മൂല്യം));
		നിർത്തുക;
	}
	അവസ്ഥ REBASE:
		/*
		 * Consider invalid values as false and check the
		 * truth value with >= REBASE_TRUE.
		 */
		info->rebase = rebase_parse_മൂല്യം(മൂല്യം);
		എങ്കിൽ (info->rebase == REBASE_INVALID)
			warning(_("unhandled branch.%s.rebase=%s; assuming "
				  "'true'"), name, മൂല്യം);
		നിർത്തുക;
	അവസ്ഥ തള്ളുക_REMOTE:
		എങ്കിൽ (info->തള്ളുക_remote_name)
			warning(_("more than one %s"), orig_താക്കോൽ);
		info->തള്ളുക_remote_name = xstrdup(മൂല്യം);
		നിർത്തുക;
	സ്ഥിരസ്ഥിതി:
		BUG("unexpected type=%d", type);
	}

	സ്വതന്ത്രം(name);
	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം read_branches(ശൂന്യം)
{
	എങ്കിൽ (branch_list.nr)
		തിരികെ;
	repo_config(the_repository, config_read_branches, ശൂന്യ);
}

ഘടന ref_states {
	ഘടന remote *remote;
	ഘടന string_list new_refs, skipped, stale, tracked, heads, തള്ളുക;
	പൂർണ്ണസംഖ്യ queried;
};

#define REF_STATES_INIT { \
	.new_refs = STRING_LIST_INIT_DUP, \
	.skipped = STRING_LIST_INIT_DUP, \
	.stale = STRING_LIST_INIT_DUP, \
	.tracked = STRING_LIST_INIT_DUP, \
	.heads = STRING_LIST_INIT_DUP, \
	.തള്ളുക = STRING_LIST_INIT_DUP, \
}

സ്ഥിതം പൂർണ്ണസംഖ്യ get_ref_states(സ്ഥിരം ഘടന ref *remote_refs, ഘടന ref_states *states)
{
	ഘടന ref *fetch_map = ശൂന്യ, **tail = &fetch_map;
	ഘടന ref *ref, *stale_refs;
	പൂർണ്ണസംഖ്യ i;

	വേണ്ടി (i = 0; i < states->remote->fetch.nr; i++)
		എങ്കിൽ (get_fetch_map(remote_refs, &states->remote->fetch.ഇനങ്ങൾ[i], &tail, 1))
			die(_("Could not get fetch map for refspec %s"),
				states->remote->fetch.ഇനങ്ങൾ[i].raw);

	വേണ്ടി (ref = fetch_map; ref; ref = ref->next) {
		എങ്കിൽ (refname_matches_negative_refspec_item(ref->name, &states->remote->fetch))
			string_list_append(&states->skipped, abbrev_branch(ref->name));
		അല്ലെങ്കിൽ എങ്കിൽ (!ref->peer_ref || !refs_ref_exists(get_main_ref_store(the_repository), ref->peer_ref->name))
			string_list_append(&states->new_refs, abbrev_branch(ref->name));
		അല്ലെങ്കിൽ
			string_list_append(&states->tracked, abbrev_branch(ref->name));
	}
	stale_refs = get_stale_heads(&states->remote->fetch, fetch_map);
	വേണ്ടി (ref = stale_refs; ref; ref = ref->next) {
		ഘടന string_list_item *item =
			string_list_append(&states->stale, abbrev_branch(ref->name));
		item->util = xstrdup(ref->name);
	}
	free_refs(stale_refs);
	free_refs(fetch_map);

	string_list_sort(&states->new_refs);
	string_list_sort(&states->skipped);
	string_list_sort(&states->tracked);
	string_list_sort(&states->stale);

	തിരികെ 0;
}

ഘടന തള്ളുക_info {
	അക്ഷരം *dest;
	പൂർണ്ണസംഖ്യ forced;
	എണ്ണൽ {
		തള്ളുക_STATUS_CREATE = 0,
		തള്ളുക_STATUS_DELETE,
		തള്ളുക_STATUS_UPTODATE,
		തള്ളുക_STATUS_FASTFORWARD,
		തള്ളുക_STATUS_OUTOFDATE,
		തള്ളുക_STATUS_NOTQUERIED
	} status;
};

സ്ഥിതം പൂർണ്ണസംഖ്യ get_തള്ളുക_ref_states(സ്ഥിരം ഘടന ref *remote_refs,
	ഘടന ref_states *states)
{
	ഘടന remote *remote = states->remote;
	ഘടന ref *ref, *local_refs, *തള്ളുക_map;
	എങ്കിൽ (remote->mirror)
		തിരികെ 0;

	local_refs = get_local_heads();
	തള്ളുക_map = copy_ref_list(remote_refs);

	match_തള്ളുക_refs(local_refs, &തള്ളുക_map, &remote->തള്ളുക, MATCH_REFS_NONE);

	വേണ്ടി (ref = തള്ളുക_map; ref; ref = ref->next) {
		ഘടന string_list_item *item;
		ഘടന തള്ളുക_info *info;

		എങ്കിൽ (!ref->peer_ref)
			തുടരുക;
		oidcpy(&ref->new_oid, &ref->peer_ref->new_oid);

		item = string_list_append(&states->തള്ളുക,
					  abbrev_branch(ref->peer_ref->name));
		item->util = xcalloc(1, വലുപ്പം(ഘടന തള്ളുക_info));
		info = item->util;
		info->forced = ref->force;
		info->dest = xstrdup(abbrev_branch(ref->name));

		എങ്കിൽ (is_null_oid(&ref->new_oid)) {
			info->status = തള്ളുക_STATUS_DELETE;
		} അല്ലെങ്കിൽ എങ്കിൽ (oideq(&ref->old_oid, &ref->new_oid))
			info->status = തള്ളുക_STATUS_UPTODATE;
		അല്ലെങ്കിൽ എങ്കിൽ (is_null_oid(&ref->old_oid))
			info->status = തള്ളുക_STATUS_CREATE;
		അല്ലെങ്കിൽ എങ്കിൽ (odb_has_object(the_repository->objects, &ref->old_oid,
					ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR) &&
			 ref_newer(&ref->new_oid, &ref->old_oid))
			info->status = തള്ളുക_STATUS_FASTFORWARD;
		അല്ലെങ്കിൽ
			info->status = തള്ളുക_STATUS_OUTOFDATE;
	}
	free_refs(local_refs);
	free_refs(തള്ളുക_map);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ get_തള്ളുക_ref_states_noquery(ഘടന ref_states *states)
{
	പൂർണ്ണസംഖ്യ i;
	ഘടന remote *remote = states->remote;
	ഘടന string_list_item *item;
	ഘടന തള്ളുക_info *info;

	എങ്കിൽ (remote->mirror)
		തിരികെ 0;

	എങ്കിൽ (!remote->തള്ളുക.nr) {
		item = string_list_append(&states->തള്ളുക, _("(matching)"));
		info = item->util = xcalloc(1, വലുപ്പം(ഘടന തള്ളുക_info));
		info->status = തള്ളുക_STATUS_NOTQUERIED;
		info->dest = xstrdup(item->string);
	}
	വേണ്ടി (i = 0; i < remote->തള്ളുക.nr; i++) {
		സ്ഥിരം ഘടന refspec_item *spec = &remote->തള്ളുക.ഇനങ്ങൾ[i];
		എങ്കിൽ (spec->matching)
			item = string_list_append(&states->തള്ളുക, _("(matching)"));
		അല്ലെങ്കിൽ എങ്കിൽ (strlen(spec->src))
			item = string_list_append(&states->തള്ളുക, spec->src);
		അല്ലെങ്കിൽ
			item = string_list_append(&states->തള്ളുക, _("(delete)"));

		info = item->util = xcalloc(1, വലുപ്പം(ഘടന തള്ളുക_info));
		info->forced = spec->force;
		info->status = തള്ളുക_STATUS_NOTQUERIED;
		info->dest = xstrdup(spec->dst ? spec->dst : item->string);
	}
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ get_head_names(സ്ഥിരം ഘടന ref *remote_refs, ഘടന ref_states *states)
{
	ഘടന ref *ref, *matches;
	ഘടന ref *fetch_map = ശൂന്യ, **fetch_map_tail = &fetch_map;
	ഘടന refspec_item refspec = {
		.force = 0,
		.pattern = 1,
		.src = (അക്ഷരം *) "refs/heads/*",
		.dst = (അക്ഷരം *) "refs/heads/*",
	};

	get_fetch_map(remote_refs, &refspec, &fetch_map_tail, 0);
	matches = guess_remote_head(find_ref_by_name(remote_refs, "HEAD"),
				    fetch_map, REMOTE_GUESS_HEAD_ALL);
	വേണ്ടി (ref = matches; ref; ref = ref->next)
		string_list_append(&states->heads, abbrev_branch(ref->name));

	free_refs(fetch_map);
	free_refs(matches);
	തിരികെ 0;
}

ഘടന known_remote {
	ഘടന known_remote *next;
	ഘടന remote *remote;
};

ഘടന known_remotes {
	ഘടന remote *to_delete;
	ഘടന known_remote *list;
};

സ്ഥിതം പൂർണ്ണസംഖ്യ add_known_remote(ഘടന remote *remote, ശൂന്യം *cb_data)
{
	ഘടന known_remotes *all = cb_data;
	ഘടന known_remote *r;

	എങ്കിൽ (!strcmp(all->to_delete->name, remote->name))
		തിരികെ 0;

	r = xmalloc(വലുപ്പം(*r));
	r->remote = remote;
	r->next = all->list;
	all->list = r;
	തിരികെ 0;
}

ഘടന branches_for_remote {
	ഘടന remote *remote;
	ഘടന string_list *branches, *skipped;
	ഘടന known_remotes *keep;
};

സ്ഥിതം പൂർണ്ണസംഖ്യ add_branch_for_removal(സ്ഥിരം ഘടന reference *ref, ശൂന്യം *cb_data)
{
	ഘടന branches_for_remote *branches = cb_data;
	ഘടന refspec_item refspec;
	ഘടന known_remote *kr;

	memset(&refspec, 0, വലുപ്പം(refspec));
	refspec.dst = (അക്ഷരം *)ref->name;
	എങ്കിൽ (remote_find_tracking(branches->remote, &refspec))
		തിരികെ 0;
	സ്വതന്ത്രം(refspec.src);

	/* don't delete a branch if another remote also uses it */
	വേണ്ടി (kr = branches->keep->list; kr; kr = kr->next) {
		memset(&refspec, 0, വലുപ്പം(refspec));
		refspec.dst = (അക്ഷരം *)ref->name;
		എങ്കിൽ (!remote_find_tracking(kr->remote, &refspec)) {
			സ്വതന്ത്രം(refspec.src);
			തിരികെ 0;
		}
	}

	/* don't delete non-remote-tracking refs */
	എങ്കിൽ (!starts_with(ref->name, "refs/remotes/")) {
		/* advise user how to delete local branches */
		എങ്കിൽ (starts_with(ref->name, "refs/heads/"))
			string_list_append(branches->skipped,
					   abbrev_branch(ref->name));
		/* silently skip over other non-remote refs */
		തിരികെ 0;
	}

	string_list_append(branches->branches, ref->name);

	തിരികെ 0;
}

ഘടന rename_info {
	സ്ഥിരം അക്ഷരം *old_name;
	സ്ഥിരം അക്ഷരം *new_name;
	ഘടന ref_transaction *transaction;
	ഘടന progress *progress;
	ഘടന strbuf *err;
	uint32_t progress_nr;
	uint64_t index;
};

സ്ഥിതം ശൂന്യം compute_renamed_ref(ഘടന rename_info *rename,
				സ്ഥിരം അക്ഷരം *refname,
				ഘടന strbuf *out)
{
	strbuf_reset(out);
	strbuf_addstr(out, refname);
	strbuf_splice(out, strlen("refs/remotes/"), strlen(rename->old_name),
		      rename->new_name, strlen(rename->new_name));
}

സ്ഥിതം പൂർണ്ണസംഖ്യ rename_one_reflog_entry(സ്ഥിരം അക്ഷരം *old_refname,
				   ഘടന object_id *old_oid,
				   ഘടന object_id *new_oid,
				   സ്ഥിരം അക്ഷരം *committer,
				   timestamp_t timestamp, പൂർണ്ണസംഖ്യ tz,
				   സ്ഥിരം അക്ഷരം *സന്ദേശം, ശൂന്യം *cb_data)
{
	ഘടന rename_info *rename = cb_data;
	ഘടന strbuf new_refname = STRBUF_INIT;
	ഘടന strbuf identity = STRBUF_INIT;
	ഘടന strbuf name = STRBUF_INIT;
	ഘടന strbuf mail = STRBUF_INIT;
	ഘടന ident_split ident;
	സ്ഥിരം അക്ഷരം *date;
	പൂർണ്ണസംഖ്യ error;

	compute_renamed_ref(rename, old_refname, &new_refname);

	എങ്കിൽ (split_ident_വരി(&ident, committer, strlen(committer)) < 0) {
		error = -1;
		പോകുക out;
	}

	strbuf_add(&name, ident.name_begin, ident.name_end - ident.name_begin);
	strbuf_add(&mail, ident.mail_begin, ident.mail_end - ident.mail_begin);

	date = show_date(timestamp, tz, DATE_MODE(NORMAL));
	strbuf_addstr(&identity, fmt_ident(name.buf, mail.buf,
					  WANT_BLANK_IDENT, date, 0));

	error = ref_transaction_update_reflog(rename->transaction, new_refname.buf,
					      new_oid, old_oid, identity.buf, സന്ദേശം,
					      rename->index++, rename->err);

out:
	strbuf_release(&new_refname);
	strbuf_release(&identity);
	strbuf_release(&name);
	strbuf_release(&mail);
	തിരികെ error;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ rename_one_reflog(സ്ഥിരം അക്ഷരം *old_refname,
			     സ്ഥിരം ഘടന object_id *old_oid,
			     ഘടന rename_info *rename)
{
	ഘടന strbuf new_refname = STRBUF_INIT;
	ഘടന strbuf message = STRBUF_INIT;
	പൂർണ്ണസംഖ്യ error;

	എങ്കിൽ (!refs_reflog_exists(get_main_ref_store(the_repository), old_refname))
		തിരികെ 0;

	error = refs_for_each_reflog_ent(get_main_ref_store(the_repository),
					 old_refname, rename_one_reflog_entry, rename);
	എങ്കിൽ (error < 0)
		പോകുക out;

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
	എങ്കിൽ (error < 0)
		തിരികെ error;

out:
	strbuf_release(&new_refname);
	strbuf_release(&message);
	തിരികെ error;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ rename_one_ref(സ്ഥിരം ഘടന reference *ref, ശൂന്യം *cb_data)
{
	ഘടന strbuf new_referent = STRBUF_INIT;
	ഘടന strbuf new_refname = STRBUF_INIT;
	ഘടന rename_info *rename = cb_data;
	സ്ഥിരം ഘടന object_id *oid = ref->oid;
	സ്ഥിരം അക്ഷരം *referent = ref->target;
	പൂർണ്ണസംഖ്യ error;

	compute_renamed_ref(rename, ref->name, &new_refname);

	എങ്കിൽ (ref->flags & REF_ISSYMREF) {
		/*
		 * Stupidly enough `referent` is not pointing to the immediate
		 * target of a symref, but it's the recursively resolved value.
		 * So symrefs pointing to symrefs would be misresolved, and
		 * unborn symrefs don't have any value for the `referent` at all.
		 */
		referent = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
						   ref->name, RESOLVE_REF_NO_RECURSE,
						   ശൂന്യ, ശൂന്യ);
		compute_renamed_ref(rename, referent, &new_referent);
		oid = ശൂന്യ;
	}

	error = ref_transaction_delete(rename->transaction, ref->name,
				       oid, referent, REF_NO_DEREF, ശൂന്യ, rename->err);
	എങ്കിൽ (error < 0)
		പോകുക out;

	error = ref_transaction_update(rename->transaction, new_refname.buf, oid, null_oid(the_hash_algo),
				       (ref->flags & REF_ISSYMREF) ? new_referent.buf : ശൂന്യ, ശൂന്യ,
				       REF_SKIP_CREATE_REFLOG | REF_NO_DEREF | REF_SKIP_OID_VERIFICATION,
				       ശൂന്യ, rename->err);
	എങ്കിൽ (error < 0)
		പോകുക out;

	error = rename_one_reflog(ref->name, oid, rename);
	എങ്കിൽ (error < 0)
		പോകുക out;

	display_progress(rename->progress, ++rename->progress_nr);

out:
	strbuf_release(&new_referent);
	strbuf_release(&new_refname);
	തിരികെ error;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ migrate_file(ഘടന remote *remote)
{
	ഘടന strbuf buf = STRBUF_INIT;

	strbuf_addf(&buf, "remote.%s.url", remote->name);
	വേണ്ടി (size_t i = 0; i < remote->url.nr; i++)
		repo_config_set_multivar(the_repository, buf.buf, remote->url.v[i], "^$", 0);
	strbuf_reset(&buf);
	strbuf_addf(&buf, "remote.%s.push", remote->name);
	വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < remote->തള്ളുക.nr; i++)
		repo_config_set_multivar(the_repository, buf.buf, remote->തള്ളുക.ഇനങ്ങൾ[i].raw, "^$", 0);
	strbuf_reset(&buf);
	strbuf_addf(&buf, "remote.%s.fetch", remote->name);
	വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < remote->fetch.nr; i++)
		repo_config_set_multivar(the_repository, buf.buf, remote->fetch.ഇനങ്ങൾ[i].raw, "^$", 0);
#ifndef WITH_BREAKING_CHANGES
	എങ്കിൽ (remote->origin == REMOTE_REMOTES)
		unlink_or_warn(repo_git_path_replace(the_repository, &buf,
						     "remotes/%s", remote->name));
	അല്ലെങ്കിൽ എങ്കിൽ (remote->origin == REMOTE_BRANCHES)
		unlink_or_warn(repo_git_path_replace(the_repository, &buf,
						     "branches/%s", remote->name));
#endif /* WITH_BREAKING_CHANGES */
	strbuf_release(&buf);

	തിരികെ 0;
}

ഘടന തള്ളുക_default_info
{
	സ്ഥിരം അക്ഷരം *old_name;
	എണ്ണൽ config_scope scope;
	ഘടന strbuf origin;
	പൂർണ്ണസംഖ്യ linenr;
};

സ്ഥിതം പൂർണ്ണസംഖ്യ config_read_തള്ളുക_default(സ്ഥിരം അക്ഷരം *താക്കോൽ, സ്ഥിരം അക്ഷരം *മൂല്യം,
	സ്ഥിരം ഘടന config_context *ctx, ശൂന്യം *cb)
{
	സ്ഥിരം ഘടന താക്കോൽ_മൂല്യം_info *kvi = ctx->kvi;

	ഘടന തള്ളുക_default_info* info = cb;
	എങ്കിൽ (strcmp(താക്കോൽ, "remote.pushdefault") ||
	    !മൂല്യം || strcmp(മൂല്യം, info->old_name))
		തിരികെ 0;

	info->scope = kvi->scope;
	strbuf_reset(&info->origin);
	strbuf_addstr(&info->origin, config_origin_type_name(kvi->origin_type));
	info->linenr = kvi->linenr;

	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം handle_തള്ളുക_default(സ്ഥിരം അക്ഷരം* old_name, സ്ഥിരം അക്ഷരം* new_name)
{
	ഘടന തള്ളുക_default_info തള്ളുക_default = {
		.old_name = old_name,
		.scope = CONFIG_SCOPE_UNKNOWN,
		.origin = STRBUF_INIT,
		.linenr = -1,
	};
	repo_config(the_repository, config_read_തള്ളുക_default, &തള്ളുക_default);
	എങ്കിൽ (തള്ളുക_default.scope >= CONFIG_SCOPE_COMMAND)
		; /* pass */
	അല്ലെങ്കിൽ എങ്കിൽ (തള്ളുക_default.scope >= CONFIG_SCOPE_LOCAL) {
		പൂർണ്ണസംഖ്യ result = repo_config_set_gently(the_repository, "remote.pushDefault",
						    new_name);
		എങ്കിൽ (new_name && result && result != CONFIG_NOTHING_SET)
			die(_("could not set '%s'"), "remote.pushDefault");
		അല്ലെങ്കിൽ എങ്കിൽ (!new_name && result && result != CONFIG_NOTHING_SET)
			die(_("could not unset '%s'"), "remote.pushDefault");
	} അല്ലെങ്കിൽ എങ്കിൽ (തള്ളുക_default.scope >= CONFIG_SCOPE_SYSTEM) {
		/* warn */
		warning(_("The %s configuration remote.pushDefault in:\n"
			  "\t%s:%d\n"
			  "now names the non-existent remote '%s'"),
			config_scope_name(തള്ളുക_default.scope),
			തള്ളുക_default.origin.buf, തള്ളുക_default.linenr,
			old_name);
	}

	strbuf_release(&തള്ളുക_default.origin);
}

സ്ഥിതം സ്ഥിരം അക്ഷരം conflicting_remote_refs_advice[] = N_(
	"The remote you are trying to rename has conflicting references in the\n"
	"new target refspec. This is most likely caused by you trying to nest\n"
	"a remote into itself, e.g. by renaming 'parent' into 'parent/child'\n"
	"or by unnesting a remote, e.g. the other way round.\n"
	"\n"
	"If that is the case, you can address this by first renaming the\n"
	"remote to a different name.\n");

സ്ഥിതം പൂർണ്ണസംഖ്യ mv(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
	      ഘടന repository *repo UNUSED)
{
	പൂർണ്ണസംഖ്യ show_progress = isatty(2);
	ഘടന option options[] = {
		OPT_BOOL(0, "progress", &show_progress, N_("force progress reporting")),
		OPT_END()
	};
	ഘടന remote *oldremote, *newremote;
	ഘടന strbuf buf = STRBUF_INIT, buf2 = STRBUF_INIT, buf3 = STRBUF_INIT,
		old_remote_context = STRBUF_INIT, err = STRBUF_INIT;
	ഘടന rename_info rename = {
		.err = &err,
	};
	പൂർണ്ണസംഖ്യ refspecs_need_update = 0;
	പൂർണ്ണസംഖ്യ result = 0;

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_rename_usage, 0);

	എങ്കിൽ (argc != 2)
		usage_with_options(builtin_remote_rename_usage, options);

	rename.old_name = argv[0];
	rename.new_name = argv[1];

	oldremote = remote_get(rename.old_name);
	എങ്കിൽ (!remote_is_configured(oldremote, 1)) {
		error(_("No such remote: '%s'"), rename.old_name);
		exit(2);
	}

	എങ്കിൽ (!strcmp(rename.old_name, rename.new_name) && oldremote->origin != REMOTE_CONFIG)
		തിരികെ migrate_file(oldremote);

	newremote = remote_get(rename.new_name);
	എങ്കിൽ (remote_is_configured(newremote, 1)) {
		error(_("remote %s already exists."), rename.new_name);
		exit(3);
	}

	എങ്കിൽ (!valid_remote_name(rename.new_name))
		die(_("'%s' is not a valid remote name"), rename.new_name);

	strbuf_addf(&buf, "remote.%s", rename.old_name);
	strbuf_addf(&buf2, "remote.%s", rename.new_name);
	എങ്കിൽ (repo_config_rename_section(the_repository, buf.buf, buf2.buf) < 1) {
		result = error(_("Could not rename config section '%s' to '%s'"),
			       buf.buf, buf2.buf);
		പോകുക out;
	}

	strbuf_addf(&old_remote_context, ":refs/remotes/%s/", rename.old_name);

	വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < oldremote->fetch.nr && !refspecs_need_update; i++)
		refspecs_need_update = !!strstr(oldremote->fetch.ഇനങ്ങൾ[i].raw,
						old_remote_context.buf);

	എങ്കിൽ (refspecs_need_update) {
		ഘടന refs_for_each_ref_options opts = {
			.flags = REFS_FOR_EACH_INCLUDE_BROKEN,
		};
		rename.transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
							       0, &err);
		എങ്കിൽ (!rename.transaction)
			പോകുക out;

		എങ്കിൽ (show_progress)
			rename.progress = start_delayed_progress(the_repository,
								 _("Renaming remote references"), 0);

		strbuf_reset(&buf);
		strbuf_addf(&buf, "refs/remotes/%s/", rename.old_name);
		opts.prefix = buf.buf;

		result = refs_for_each_ref_ext(get_main_ref_store(the_repository),
					       rename_one_ref, &rename, &opts);
		എങ്കിൽ (result < 0)
			die(_("queueing remote ref renames failed: %s"), rename.err->buf);

		result = ref_transaction_prepare(rename.transaction, &err);
		എങ്കിൽ (result < 0) {
			error("renaming remote references failed: %s", err.buf);
			എങ്കിൽ (result == REF_TRANSACTION_ERROR_NAME_CONFLICT)
				advise(conflicting_remote_refs_advice);
			die(ശൂന്യ);
		}
	}

	എങ്കിൽ (oldremote->fetch.nr) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.fetch", rename.new_name);
		repo_config_set_multivar(the_repository, buf.buf, ശൂന്യ, ശൂന്യ, CONFIG_FLAGS_MULTI_REPLACE);
		വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < oldremote->fetch.nr; i++) {
			അക്ഷരം *ptr;

			strbuf_reset(&buf2);
			strbuf_addstr(&buf2, oldremote->fetch.ഇനങ്ങൾ[i].raw);
			ptr = strstr(buf2.buf, old_remote_context.buf);
			എങ്കിൽ (ptr) {
				strbuf_splice(&buf2,
					      ptr-buf2.buf + strlen(":refs/remotes/"),
					      strlen(rename.old_name), rename.new_name,
					      strlen(rename.new_name));
			} അല്ലെങ്കിൽ
				warning(_("Not updating non-default fetch refspec\n"
					  "\t%s\n"
					  "\tPlease update the configuration manually if necessary."),
					buf2.buf);

			repo_config_set_multivar(the_repository, buf.buf, buf2.buf, "^$", 0);
		}
	}

	read_branches();
	വേണ്ടി (size_t i = 0; i < branch_list.nr; i++) {
		ഘടന string_list_item *item = branch_list.ഇനങ്ങൾ + i;
		ഘടന branch_info *info = item->util;
		എങ്കിൽ (info->remote_name && !strcmp(info->remote_name, rename.old_name)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "branch.%s.remote", item->string);
			repo_config_set(the_repository, buf.buf, rename.new_name);
		}
		എങ്കിൽ (info->തള്ളുക_remote_name && !strcmp(info->തള്ളുക_remote_name, rename.old_name)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "branch.%s.pushRemote", item->string);
			repo_config_set(the_repository, buf.buf, rename.new_name);
		}
	}

	എങ്കിൽ (refspecs_need_update) {
		result = ref_transaction_commit(rename.transaction, &err);
		എങ്കിൽ (result < 0)
			die(_("renaming remote refs failed: %s"), rename.err->buf);

		stop_progress(&rename.progress);

		handle_തള്ളുക_default(rename.old_name, rename.new_name);
	}

out:
	ref_transaction_free(rename.transaction);
	strbuf_release(&old_remote_context);
	strbuf_release(&buf);
	strbuf_release(&buf2);
	strbuf_release(&buf3);
	strbuf_release(&err);
	തിരികെ result;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ rm(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
	      ഘടന repository *repo UNUSED)
{
	ഘടന option options[] = {
		OPT_END()
	};
	ഘടന remote *remote;
	ഘടന strbuf buf = STRBUF_INIT;
	ഘടന known_remotes known_remotes = { ശൂന്യ, ശൂന്യ };
	ഘടന string_list branches = STRING_LIST_INIT_DUP;
	ഘടന string_list skipped = STRING_LIST_INIT_DUP;
	ഘടന branches_for_remote cb_data;
	പൂർണ്ണസംഖ്യ result;

	memset(&cb_data, 0, വലുപ്പം(cb_data));
	cb_data.branches = &branches;
	cb_data.skipped = &skipped;
	cb_data.keep = &known_remotes;

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_rm_usage, 0);
	എങ്കിൽ (argc != 1)
		usage_with_options(builtin_remote_rm_usage, options);

	remote = remote_get(argv[0]);
	എങ്കിൽ (!remote_is_configured(remote, 1)) {
		error(_("No such remote: '%s'"), argv[0]);
		exit(2);
	}

	known_remotes.to_delete = remote;
	for_each_remote(add_known_remote, &known_remotes);

	read_branches();
	വേണ്ടി (size_t i = 0; i < branch_list.nr; i++) {
		ഘടന string_list_item *item = branch_list.ഇനങ്ങൾ + i;
		ഘടന branch_info *info = item->util;
		എങ്കിൽ (info->remote_name && !strcmp(info->remote_name, remote->name)) {
			സ്ഥിരം അക്ഷരം *താക്കോലുകൾ[] = { "remote", "merge", ശൂന്യ }, **k;
			വേണ്ടി (k = താക്കോലുകൾ; *k; k++) {
				strbuf_reset(&buf);
				strbuf_addf(&buf, "branch.%s.%s",
						item->string, *k);
				result = repo_config_set_gently(the_repository, buf.buf, ശൂന്യ);
				എങ്കിൽ (result && result != CONFIG_NOTHING_SET)
					die(_("could not unset '%s'"), buf.buf);
			}
		}
		എങ്കിൽ (info->തള്ളുക_remote_name && !strcmp(info->തള്ളുക_remote_name, remote->name)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "branch.%s.pushremote", item->string);
			result = repo_config_set_gently(the_repository, buf.buf, ശൂന്യ);
			എങ്കിൽ (result && result != CONFIG_NOTHING_SET)
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

	എങ്കിൽ (!result)
		result = refs_delete_refs(get_main_ref_store(the_repository),
					  "remote: remove", &branches,
					  REF_NO_DEREF);
	string_list_clear(&branches, 0);

	എങ്കിൽ (skipped.nr) {
		fprintf_ln(stderr,
			   Q_("Note: A branch outside the refs/remotes/ hierarchy was not removed;\n"
			      "to delete it, use:",
			      "Note: Some branches outside the refs/remotes/ hierarchy were not removed;\n"
			      "to delete them, use:",
			      skipped.nr));
		വേണ്ടി (size_t i = 0; i < skipped.nr; i++)
			fprintf(stderr, "  git branch -d %s\n",
				skipped.ഇനങ്ങൾ[i].string);
	}
	string_list_clear(&skipped, 0);

	എങ്കിൽ (!result) {
		strbuf_addf(&buf, "remote.%s", remote->name);
		എങ്കിൽ (repo_config_rename_section(the_repository, buf.buf, ശൂന്യ) < 1) {
			result = error(_("Could not remove config section '%s'"), buf.buf);
			പോകുക out;
		}

		handle_തള്ളുക_default(remote->name, ശൂന്യ);
	}

out:
	വേണ്ടി (ഘടന known_remote *r = known_remotes.list; r;) {
		ഘടന known_remote *next = r->next;
		സ്വതന്ത്രം(r);
		r = next;
	}
	strbuf_release(&buf);
	തിരികെ result;
}

സ്ഥിതം ശൂന്യം clear_തള്ളുക_info(ശൂന്യം *util, സ്ഥിരം അക്ഷരം *string UNUSED)
{
	ഘടന തള്ളുക_info *info = util;
	സ്വതന്ത്രം(info->dest);
	സ്വതന്ത്രം(info);
}

സ്ഥിതം ശൂന്യം free_remote_ref_states(ഘടന ref_states *states)
{
	string_list_clear(&states->new_refs, 0);
	string_list_clear(&states->skipped, 0);
	string_list_clear(&states->stale, 1);
	string_list_clear(&states->tracked, 0);
	string_list_clear(&states->heads, 0);
	string_list_clear_func(&states->തള്ളുക, clear_തള്ളുക_info);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ append_ref_to_tracked_list(സ്ഥിരം ഘടന reference *ref, ശൂന്യം *cb_data)
{
	ഘടന ref_states *states = cb_data;
	ഘടന refspec_item refspec;

	എങ്കിൽ (ref->flags & REF_ISSYMREF)
		തിരികെ 0;

	memset(&refspec, 0, വലുപ്പം(refspec));
	refspec.dst = (അക്ഷരം *)ref->name;
	എങ്കിൽ (!remote_find_tracking(states->remote, &refspec)) {
		string_list_append(&states->tracked, abbrev_branch(refspec.src));
		സ്വതന്ത്രം(refspec.src);
	}

	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ get_remote_ref_states(സ്ഥിരം അക്ഷരം *name,
				 ഘടന ref_states *states,
				 പൂർണ്ണസംഖ്യ query)
{
	states->remote = remote_get(name);
	എങ്കിൽ (!states->remote)
		തിരികെ error(_("No such remote: '%s'"), name);

	read_branches();

	എങ്കിൽ (query) {
		ഘടന transport *transport;
		സ്ഥിരം ഘടന ref *remote_refs;

		transport = transport_get(states->remote, states->remote->url.v[0]);
		remote_refs = transport_get_remote_refs(transport, ശൂന്യ);

		states->queried = 1;
		എങ്കിൽ (query & GET_REF_STATES)
			get_ref_states(remote_refs, states);
		എങ്കിൽ (query & GET_HEAD_NAMES)
			get_head_names(remote_refs, states);
		എങ്കിൽ (query & GET_തള്ളുക_REF_STATES)
			get_തള്ളുക_ref_states(remote_refs, states);
		transport_disconnect(transport);
	} അല്ലെങ്കിൽ {
		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_ref_to_tracked_list, states);
		string_list_sort(&states->tracked);
		get_തള്ളുക_ref_states_noquery(states);
	}

	തിരികെ 0;
}

ഘടന show_info {
	ഘടന string_list list;
	ഘടന ref_states states;
	പൂർണ്ണസംഖ്യ width, width2;
	പൂർണ്ണസംഖ്യ any_rebase;
};

#define SHOW_INFO_INIT { \
	.list = STRING_LIST_INIT_DUP, \
	.states = REF_STATES_INIT, \
}

സ്ഥിതം പൂർണ്ണസംഖ്യ add_remote_to_show_info(ഘടന string_list_item *item, ശൂന്യം *cb_data)
{
	ഘടന show_info *info = cb_data;
	പൂർണ്ണസംഖ്യ n = strlen(item->string);
	എങ്കിൽ (n > info->width)
		info->width = n;
	string_list_തിരുകുക(&info->list, item->string);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ show_remote_info_item(ഘടന string_list_item *item, ശൂന്യം *cb_data)
{
	ഘടന show_info *info = cb_data;
	ഘടന ref_states *states = &info->states;
	സ്ഥിരം അക്ഷരം *name = item->string;

	എങ്കിൽ (states->queried) {
		സ്ഥിരം അക്ഷരം *fmt = "%s";
		സ്ഥിരം അക്ഷരം *arg = "";
		എങ്കിൽ (string_list_has_string(&states->new_refs, name)) {
			fmt = _(" new (next fetch will store in remotes/%s)");
			arg = states->remote->name;
		} അല്ലെങ്കിൽ എങ്കിൽ (string_list_has_string(&states->tracked, name))
			arg = _(" tracked");
		അല്ലെങ്കിൽ എങ്കിൽ (string_list_has_string(&states->skipped, name))
			arg = _(" skipped");
		അല്ലെങ്കിൽ എങ്കിൽ (string_list_has_string(&states->stale, name))
			arg = _(" stale (use 'git remote prune' to remove)");
		അല്ലെങ്കിൽ
			arg = _(" ???");
		printf("    %-*s", info->width, name);
		printf(fmt, arg);
		printf("\n");
	} അല്ലെങ്കിൽ
		printf("    %s\n", name);

	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ add_local_to_show_info(ഘടന string_list_item *branch_item, ശൂന്യം *cb_data)
{
	ഘടന show_info *show_info = cb_data;
	ഘടന ref_states *states = &show_info->states;
	ഘടന branch_info *branch_info = branch_item->util;
	ഘടന string_list_item *item;
	പൂർണ്ണസംഖ്യ n;

	എങ്കിൽ (!branch_info->merge.nr || !branch_info->remote_name ||
	    strcmp(states->remote->name, branch_info->remote_name))
		തിരികെ 0;
	എങ്കിൽ ((n = strlen(branch_item->string)) > show_info->width)
		show_info->width = n;
	എങ്കിൽ (branch_info->rebase >= REBASE_TRUE)
		show_info->any_rebase = 1;

	item = string_list_തിരുകുക(&show_info->list, branch_item->string);
	item->util = branch_info;

	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ show_local_info_item(ഘടന string_list_item *item, ശൂന്യം *cb_data)
{
	ഘടന show_info *show_info = cb_data;
	ഘടന branch_info *branch_info = item->util;
	ഘടന string_list *merge = &branch_info->merge;
	പൂർണ്ണസംഖ്യ width = show_info->width + 4;

	എങ്കിൽ (branch_info->rebase >= REBASE_TRUE && branch_info->merge.nr > 1) {
		error(_("invalid branch.%s.merge; cannot rebase onto > 1 branch"),
			item->string);
		തിരികെ 0;
	}

	printf("    %-*s ", show_info->width, item->string);
	എങ്കിൽ (branch_info->rebase >= REBASE_TRUE) {
		സ്ഥിരം അക്ഷരം *സന്ദേശം;
		എങ്കിൽ (branch_info->rebase == REBASE_INTERACTIVE)
			സന്ദേശം = _("rebases interactively onto remote %s");
		അല്ലെങ്കിൽ എങ്കിൽ (branch_info->rebase == REBASE_MERGES)
			സന്ദേശം = _("rebases interactively (with merges) onto "
				"remote %s");
		അല്ലെങ്കിൽ
			സന്ദേശം = _("rebases onto remote %s");
		printf_ln(സന്ദേശം, merge->ഇനങ്ങൾ[0].string);
		തിരികെ 0;
	} അല്ലെങ്കിൽ എങ്കിൽ (show_info->any_rebase) {
		printf_ln(_(" merges with remote %s"), merge->ഇനങ്ങൾ[0].string);
		width++;
	} അല്ലെങ്കിൽ {
		printf_ln(_("merges with remote %s"), merge->ഇനങ്ങൾ[0].string);
	}
	വേണ്ടി (size_t i = 1; i < merge->nr; i++)
		printf(_("%-*s    and with remote %s\n"), width, "",
		       merge->ഇനങ്ങൾ[i].string);

	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ add_തള്ളുക_to_show_info(ഘടന string_list_item *തള്ളുക_item, ശൂന്യം *cb_data)
{
	ഘടന show_info *show_info = cb_data;
	ഘടന തള്ളുക_info *തള്ളുക_info = തള്ളുക_item->util;
	ഘടന string_list_item *item;
	പൂർണ്ണസംഖ്യ n;
	എങ്കിൽ ((n = strlen(തള്ളുക_item->string)) > show_info->width)
		show_info->width = n;
	എങ്കിൽ ((n = strlen(തള്ളുക_info->dest)) > show_info->width2)
		show_info->width2 = n;
	item = string_list_append(&show_info->list, തള്ളുക_item->string);
	item->util = തള്ളുക_item->util;
	തിരികെ 0;
}

/*
 * Sorting comparison for a string list that has push_info
 * structs in its util field
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ cmp_string_with_തള്ളുക(സ്ഥിരം ശൂന്യം *va, സ്ഥിരം ശൂന്യം *vb)
{
	സ്ഥിരം ഘടന string_list_item *a = va;
	സ്ഥിരം ഘടന string_list_item *b = vb;
	സ്ഥിരം ഘടന തള്ളുക_info *a_തള്ളുക = a->util;
	സ്ഥിരം ഘടന തള്ളുക_info *b_തള്ളുക = b->util;
	പൂർണ്ണസംഖ്യ cmp = strcmp(a->string, b->string);
	തിരികെ cmp ? cmp : strcmp(a_തള്ളുക->dest, b_തള്ളുക->dest);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ show_തള്ളുക_info_item(ഘടന string_list_item *item, ശൂന്യം *cb_data)
{
	ഘടന show_info *show_info = cb_data;
	ഘടന തള്ളുക_info *തള്ളുക_info = item->util;
	സ്ഥിരം അക്ഷരം *src = item->string, *status = ശൂന്യ;

	തിരഞ്ഞെടുപ്പ് (തള്ളുക_info->status) {
	അവസ്ഥ തള്ളുക_STATUS_CREATE:
		status = _("create");
		നിർത്തുക;
	അവസ്ഥ തള്ളുക_STATUS_DELETE:
		status = _("delete");
		src = _("(none)");
		നിർത്തുക;
	അവസ്ഥ തള്ളുക_STATUS_UPTODATE:
		status = _("up to date");
		നിർത്തുക;
	അവസ്ഥ തള്ളുക_STATUS_FASTFORWARD:
		status = _("fast-forwardable");
		നിർത്തുക;
	അവസ്ഥ തള്ളുക_STATUS_OUTOFDATE:
		status = _("local out of date");
		നിർത്തുക;
	അവസ്ഥ തള്ളുക_STATUS_NOTQUERIED:
		നിർത്തുക;
	}
	എങ്കിൽ (status) {
		എങ്കിൽ (തള്ളുക_info->forced)
			printf_ln(_("    %-*s forces to %-*s (%s)"), show_info->width, src,
			       show_info->width2, തള്ളുക_info->dest, status);
		അല്ലെങ്കിൽ
			printf_ln(_("    %-*s pushes to %-*s (%s)"), show_info->width, src,
			       show_info->width2, തള്ളുക_info->dest, status);
	} അല്ലെങ്കിൽ {
		എങ്കിൽ (തള്ളുക_info->forced)
			printf_ln(_("    %-*s forces to %s"), show_info->width, src,
			       തള്ളുക_info->dest);
		അല്ലെങ്കിൽ
			printf_ln(_("    %-*s pushes to %s"), show_info->width, src,
			       തള്ളുക_info->dest);
	}
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ get_one_entry(ഘടന remote *remote, ശൂന്യം *priv)
{
	ഘടന string_list *list = priv;
	ഘടന strbuf remote_info_buf = STRBUF_INIT;
	ഘടന strvec *url;

	എങ്കിൽ (remote->url.nr > 0) {
		ഘടന strbuf promisor_config = STRBUF_INIT;
		സ്ഥിരം അക്ഷരം *partial_clone_filter = ശൂന്യ;

		strbuf_addf(&promisor_config, "remote.%s.partialclonefilter", remote->name);
		strbuf_addf(&remote_info_buf, "%s (fetch)", remote->url.v[0]);
		എങ്കിൽ (!repo_config_get_string_tmp(the_repository, promisor_config.buf, &partial_clone_filter))
			strbuf_addf(&remote_info_buf, " [%s]", partial_clone_filter);

		strbuf_release(&promisor_config);
		string_list_append(list, remote->name)->util =
				strbuf_detach(&remote_info_buf, ശൂന്യ);
	} അല്ലെങ്കിൽ
		string_list_append(list, remote->name)->util = ശൂന്യ;
	url = തള്ളുക_url_of_remote(remote);
	വേണ്ടി (size_t i = 0; i < url->nr; i++) {
		strbuf_addf(&remote_info_buf, "%s (push)", url->v[i]);
		string_list_append(list, remote->name)->util =
				strbuf_detach(&remote_info_buf, ശൂന്യ);
	}

	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ show_all(ശൂന്യം)
{
	ഘടന string_list list = STRING_LIST_INIT_DUP;
	പൂർണ്ണസംഖ്യ result;

	result = for_each_remote(get_one_entry, &list);

	എങ്കിൽ (!result) {
		string_list_sort(&list);
		വേണ്ടി (size_t i = 0; i < list.nr; i++) {
			ഘടന string_list_item *item = list.ഇനങ്ങൾ + i;
			എങ്കിൽ (verbose)
				printf("%s\t%s\n", item->string,
					item->util ? (സ്ഥിരം അക്ഷരം *)item->util : "");
			അല്ലെങ്കിൽ {
				എങ്കിൽ (i && !strcmp((item - 1)->string, item->string))
					തുടരുക;
				printf("%s\n", item->string);
			}
		}
	}
	string_list_clear(&list, 1);
	തിരികെ result;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ show(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
		ഘടന repository *repo UNUSED)
{
	പൂർണ്ണസംഖ്യ no_query = 0, result = 0, query_flag = 0;
	ഘടന option options[] = {
		OPT_BOOL('n', ശൂന്യ, &no_query, N_("do not query remotes")),
		OPT_END()
	};
	ഘടന show_info info = SHOW_INFO_INIT;

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_show_usage,
			     0);

	എങ്കിൽ (argc < 1)
		തിരികെ show_all();

	എങ്കിൽ (!no_query)
		query_flag = (GET_REF_STATES | GET_HEAD_NAMES | GET_തള്ളുക_REF_STATES);

	വേണ്ടി (; argc; argc--, argv++) {
		size_t i;
		ഘടന strvec *url;

		get_remote_ref_states(*argv, &info.states, query_flag);

		printf_ln(_("* remote %s"), *argv);
		printf_ln(_("  Fetch URL: %s"), info.states.remote->url.v[0]);
		url = തള്ളുക_url_of_remote(info.states.remote);
		വേണ്ടി (i = 0; i < url->nr; i++)
			/*
			 * TRANSLATORS: the colon ':' should align
			 * with the one in " Fetch URL: %s"
			 * translation.
			 */
			printf_ln(_("  Push  URL: %s"), url->v[i]);
		എങ്കിൽ (!i)
			printf_ln(_("  Push  URL: %s"), _("(no URL)"));
		എങ്കിൽ (no_query)
			printf_ln(_("  HEAD branch: %s"), _("(not queried)"));
		അല്ലെങ്കിൽ എങ്കിൽ (!info.states.heads.nr)
			printf_ln(_("  HEAD branch: %s"), _("(unknown)"));
		അല്ലെങ്കിൽ എങ്കിൽ (info.states.heads.nr == 1)
			printf_ln(_("  HEAD branch: %s"), info.states.heads.ഇനങ്ങൾ[0].string);
		അല്ലെങ്കിൽ {
			printf(_("  HEAD branch (remote HEAD is ambiguous,"
				 " may be one of the following):\n"));
			വേണ്ടി (i = 0; i < info.states.heads.nr; i++)
				printf("    %s\n", info.states.heads.ഇനങ്ങൾ[i].string);
		}

		/* remote branch info */
		info.width = 0;
		for_each_string_list(&info.states.new_refs, add_remote_to_show_info, &info);
		for_each_string_list(&info.states.skipped, add_remote_to_show_info, &info);
		for_each_string_list(&info.states.tracked, add_remote_to_show_info, &info);
		for_each_string_list(&info.states.stale, add_remote_to_show_info, &info);
		എങ്കിൽ (info.list.nr)
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
		എങ്കിൽ (info.list.nr)
			printf_ln(Q_("  Local branch configured for 'git pull':",
				     "  Local branches configured for 'git pull':",
				     info.list.nr));
		for_each_string_list(&info.list, show_local_info_item, &info);
		string_list_clear(&info.list, 0);

		/* git push info */
		എങ്കിൽ (info.states.remote->mirror)
			printf_ln(_("  Local refs will be mirrored by 'git push'"));

		info.width = info.width2 = 0;
		for_each_string_list(&info.states.തള്ളുക, add_തള്ളുക_to_show_info, &info);
		QSORT(info.list.ഇനങ്ങൾ, info.list.nr, cmp_string_with_തള്ളുക);
		എങ്കിൽ (info.list.nr)
			printf_ln(Q_("  Local ref configured for 'git push'%s:",
				     "  Local refs configured for 'git push'%s:",
				     info.list.nr),
				  no_query ? _(" (status not queried)") : "");
		for_each_string_list(&info.list, show_തള്ളുക_info_item, &info);
		string_list_clear(&info.list, 0);

		free_remote_ref_states(&info.states);
	}

	തിരികെ result;
}

സ്ഥിതം ശൂന്യം report_set_head_auto(സ്ഥിരം അക്ഷരം *remote, സ്ഥിരം അക്ഷരം *head_name,
			ഘടന strbuf *b_local_head, പൂർണ്ണസംഖ്യ was_detached) {
	ഘടന strbuf buf_prefix = STRBUF_INIT;
	സ്ഥിരം അക്ഷരം *prev_head = ശൂന്യ;

	strbuf_addf(&buf_prefix, "refs/remotes/%s/", remote);
	skip_prefix(b_local_head->buf, buf_prefix.buf, &prev_head);

	എങ്കിൽ (prev_head && !strcmp(prev_head, head_name))
		printf(_("'%s/HEAD' is unchanged and points to '%s'\n"),
			remote, head_name);
	അല്ലെങ്കിൽ എങ്കിൽ (prev_head)
		printf(_("'%s/HEAD' has changed from '%s' and now points to '%s'\n"),
			remote, prev_head, head_name);
	അല്ലെങ്കിൽ എങ്കിൽ (!b_local_head->len)
		printf(_("'%s/HEAD' is now created and points to '%s'\n"),
			remote, head_name);
	അല്ലെങ്കിൽ എങ്കിൽ (was_detached && b_local_head->len)
		printf(_("'%s/HEAD' was detached at '%s' and now points to '%s'\n"),
			remote, b_local_head->buf, head_name);
	അല്ലെങ്കിൽ
		printf(_("'%s/HEAD' used to point to '%s' "
			"(which is not a remote branch), but now points to '%s'\n"),
			remote, b_local_head->buf, head_name);
	strbuf_release(&buf_prefix);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ set_head(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
		    ഘടന repository *repo UNUSED)
{
	പൂർണ്ണസംഖ്യ opt_a = 0, opt_d = 0, result = 0, was_detached;
	ഘടന strbuf b_head = STRBUF_INIT, b_remote_head = STRBUF_INIT,
		b_local_head = STRBUF_INIT;
	അക്ഷരം *head_name = ശൂന്യ;
	ഘടന ref_store *refs = get_main_ref_store(the_repository);
	ഘടന remote *remote;

	ഘടന option options[] = {
		OPT_BOOL('a', "auto", &opt_a,
			 N_("set refs/remotes/<name>/HEAD according to remote")),
		OPT_BOOL('d', "delete", &opt_d,
			 N_("delete refs/remotes/<name>/HEAD")),
		OPT_END()
	};
	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_sethead_usage, 0);

	/* All modes require at least a remote name. */
	എങ്കിൽ (!argc)
		usage_with_options(builtin_remote_sethead_usage, options);

	strbuf_addf(&b_head, "refs/remotes/%s/HEAD", argv[0]);
	remote = remote_get(argv[0]);

	എങ്കിൽ (!opt_a && !opt_d && argc == 2) {
		head_name = xstrdup(argv[1]);
	} അല്ലെങ്കിൽ എങ്കിൽ (opt_a && !opt_d && argc == 1) {
		ഘടന ref_states states = REF_STATES_INIT;
		get_remote_ref_states(argv[0], &states, GET_HEAD_NAMES);
		എങ്കിൽ (!states.heads.nr)
			result |= error(_("Cannot determine remote HEAD"));
		അല്ലെങ്കിൽ എങ്കിൽ (states.heads.nr > 1) {
			result |= error(_("Multiple remote HEAD branches. "
					  "Please choose one explicitly with:"));
			വേണ്ടി (size_t i = 0; i < states.heads.nr; i++)
				fprintf(stderr, "  git remote set-head %s %s\n",
					argv[0], states.heads.ഇനങ്ങൾ[i].string);
		} അല്ലെങ്കിൽ
			head_name = xstrdup(states.heads.ഇനങ്ങൾ[0].string);
		free_remote_ref_states(&states);
	} അല്ലെങ്കിൽ എങ്കിൽ (opt_d && !opt_a && argc == 1) {
		എങ്കിൽ (refs_delete_ref(refs, ശൂന്യ, b_head.buf, ശൂന്യ, REF_NO_DEREF))
			result |= error(_("Could not delete %s"), b_head.buf);
	} അല്ലെങ്കിൽ
		usage_with_options(builtin_remote_sethead_usage, options);

	എങ്കിൽ (!head_name)
		പോകുക cleanup;
	strbuf_addf(&b_remote_head, "refs/remotes/%s/%s", argv[0], head_name);
	എങ്കിൽ (!refs_ref_exists(refs, b_remote_head.buf)) {
		result |= error(_("Not a valid ref: %s"), b_remote_head.buf);
		പോകുക cleanup;
	}
	was_detached = refs_update_symref_extended(refs, b_head.buf, b_remote_head.buf,
			"remote set-head", &b_local_head, 0);
	എങ്കിൽ (was_detached == -1) {
		result |= error(_("Could not set up %s"), b_head.buf);
		പോകുക cleanup;
	}
	എങ്കിൽ (opt_a)
		report_set_head_auto(argv[0], head_name, &b_local_head, was_detached);
	എങ്കിൽ (remote->follow_remote_head == FOLLOW_REMOTE_ALWAYS) {
		ഘടന strbuf config_name = STRBUF_INIT;
		strbuf_addf(&config_name,
			"remote.%s.followremotehead", remote->name);
		repo_config_set(the_repository, config_name.buf, "warn");
		strbuf_release(&config_name);
	}

cleanup:
	സ്വതന്ത്രം(head_name);
	strbuf_release(&b_head);
	strbuf_release(&b_remote_head);
	strbuf_release(&b_local_head);
	തിരികെ result;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ prune_remote(സ്ഥിരം അക്ഷരം *remote, പൂർണ്ണസംഖ്യ dry_run)
{
	പൂർണ്ണസംഖ്യ result = 0;
	ഘടന ref_states states = REF_STATES_INIT;
	ഘടന string_list refs_to_prune = STRING_LIST_INIT_NODUP;
	ഘടന string_list_item *item;

	get_remote_ref_states(remote, &states, GET_REF_STATES);

	എങ്കിൽ (!states.stale.nr) {
		free_remote_ref_states(&states);
		തിരികെ 0;
	}

	printf_ln(_("Pruning %s"), remote);
	printf_ln(_("URL: %s"), states.remote->url.v[0]);

	for_each_string_list_item(item, &states.stale)
		string_list_append(&refs_to_prune, item->util);
	string_list_sort(&refs_to_prune);

	എങ്കിൽ (!dry_run)
		result |= refs_delete_refs(get_main_ref_store(the_repository),
					   "remote: prune", &refs_to_prune, 0);

	for_each_string_list_item(item, &states.stale) {
		സ്ഥിരം അക്ഷരം *refname = item->util;

		എങ്കിൽ (dry_run)
			printf_ln(_(" * [would prune] %s"),
			       abbrev_ref(refname, "refs/remotes/"));
		അല്ലെങ്കിൽ
			printf_ln(_(" * [pruned] %s"),
			       abbrev_ref(refname, "refs/remotes/"));
	}

	refs_warn_dangling_symrefs(get_main_ref_store(the_repository),
				   stdout, " ", dry_run, &refs_to_prune);

	string_list_clear(&refs_to_prune, 0);
	free_remote_ref_states(&states);
	തിരികെ result;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ prune(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
		 ഘടന repository *repo UNUSED)
{
	പൂർണ്ണസംഖ്യ dry_run = 0, result = 0;
	ഘടന option options[] = {
		OPT__DRY_RUN(&dry_run, N_("dry run")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_prune_usage, 0);

	എങ്കിൽ (argc < 1)
		usage_with_options(builtin_remote_prune_usage, options);

	വേണ്ടി (; argc; argc--, argv++)
		result |= prune_remote(*argv, dry_run);

	തിരികെ result;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ get_remote_default(സ്ഥിരം അക്ഷരം *താക്കോൽ, സ്ഥിരം അക്ഷരം *മൂല്യം UNUSED,
			      സ്ഥിരം ഘടന config_context *ctx UNUSED,
			      ശൂന്യം *priv)
{
	എങ്കിൽ (strcmp(താക്കോൽ, "remotes.default") == 0) {
		പൂർണ്ണസംഖ്യ *found = priv;
		*found = 1;
	}
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ update(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
		  ഘടന repository *repo UNUSED)
{
	പൂർണ്ണസംഖ്യ i, prune = -1;
	ഘടന option options[] = {
		OPT_BOOL('p', "prune", &prune,
			 N_("prune remotes after fetching")),
		OPT_END()
	};
	ഘടന child_process cmd = CHILD_PROCESS_INIT;
	പൂർണ്ണസംഖ്യ default_defined = 0;

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_update_usage,
			     PARSE_OPT_KEEP_ARGV0);

	strvec_തള്ളുക(&cmd.args, "fetch");

	എങ്കിൽ (prune != -1)
		strvec_തള്ളുക(&cmd.args, prune ? "--prune" : "--no-prune");
	എങ്കിൽ (verbose)
		strvec_തള്ളുക(&cmd.args, "-v");
	strvec_തള്ളുക(&cmd.args, "--multiple");
	എങ്കിൽ (argc < 2)
		strvec_തള്ളുക(&cmd.args, "default");
	വേണ്ടി (i = 1; i < argc; i++)
		strvec_തള്ളുക(&cmd.args, argv[i]);

	എങ്കിൽ (strcmp(cmd.args.v[cmd.args.nr-1], "default") == 0) {
		repo_config(the_repository, get_remote_default, &default_defined);
		എങ്കിൽ (!default_defined) {
			strvec_എടുക്കുക(&cmd.args);
			strvec_തള്ളുക(&cmd.args, "--all");
		}
	}

	cmd.git_cmd = 1;
	തിരികെ run_command(&cmd);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ remove_all_fetch_refspecs(സ്ഥിരം അക്ഷരം *താക്കോൽ)
{
	തിരികെ repo_config_set_multivar_gently(the_repository, താക്കോൽ, ശൂന്യ, ശൂന്യ,
					       CONFIG_FLAGS_MULTI_REPLACE);
}

സ്ഥിതം ശൂന്യം add_branches(ഘടന remote *remote, സ്ഥിരം അക്ഷരം **branches,
			 സ്ഥിരം അക്ഷരം *താക്കോൽ)
{
	സ്ഥിരം അക്ഷരം *remotename = remote->name;
	പൂർണ്ണസംഖ്യ mirror = remote->mirror;
	ഘടന strbuf refspec = STRBUF_INIT;

	വേണ്ടി (; *branches; branches++)
		add_branch(താക്കോൽ, *branches, remotename, mirror, &refspec);

	strbuf_release(&refspec);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ set_remote_branches(സ്ഥിരം അക്ഷരം *remotename, സ്ഥിരം അക്ഷരം **branches,
				പൂർണ്ണസംഖ്യ add_mode)
{
	ഘടന strbuf താക്കോൽ = STRBUF_INIT;
	ഘടന remote *remote;

	strbuf_addf(&താക്കോൽ, "remote.%s.fetch", remotename);

	remote = remote_get(remotename);
	എങ്കിൽ (!remote_is_configured(remote, 1)) {
		error(_("No such remote '%s'"), remotename);
		exit(2);
	}

	എങ്കിൽ (!add_mode && remove_all_fetch_refspecs(താക്കോൽ.buf)) {
		strbuf_release(&താക്കോൽ);
		തിരികെ 1;
	}
	add_branches(remote, branches, താക്കോൽ.buf);

	strbuf_release(&താക്കോൽ);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ set_branches(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
			ഘടന repository *repo UNUSED)
{
	പൂർണ്ണസംഖ്യ add_mode = 0;
	ഘടന option options[] = {
		OPT_BOOL('\0', "add", &add_mode, N_("add branch")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_setbranches_usage, 0);
	എങ്കിൽ (argc == 0) {
		error(_("no remote specified"));
		usage_with_options(builtin_remote_setbranches_usage, options);
	}
	argv[argc] = ശൂന്യ;

	തിരികെ set_remote_branches(argv[0], argv + 1, add_mode);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ get_url(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
		   ഘടന repository *repo UNUSED)
{
	പൂർണ്ണസംഖ്യ തള്ളുക_mode = 0, all_mode = 0;
	സ്ഥിരം അക്ഷരം *remotename = ശൂന്യ;
	ഘടന remote *remote;
	ഘടന strvec *url;
	ഘടന option options[] = {
		OPT_BOOL('\0', "push", &തള്ളുക_mode,
			 N_("query push URLs rather than fetch URLs")),
		OPT_BOOL('\0', "all", &all_mode,
			 N_("return all URLs")),
		OPT_END()
	};
	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_geturl_usage, 0);

	എങ്കിൽ (argc != 1)
		usage_with_options(builtin_remote_geturl_usage, options);

	remotename = argv[0];

	remote = remote_get(remotename);
	എങ്കിൽ (!remote_is_configured(remote, 1)) {
		error(_("No such remote '%s'"), remotename);
		exit(2);
	}

	url = തള്ളുക_mode ? തള്ളുക_url_of_remote(remote) : &remote->url;

	എങ്കിൽ (all_mode) {
		വേണ്ടി (size_t i = 0; i < url->nr; i++)
			printf_ln("%s", url->v[i]);
	} അല്ലെങ്കിൽ {
		printf_ln("%s", url->v[0]);
	}

	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ set_url(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
		   ഘടന repository *repo UNUSED)
{
	പൂർണ്ണസംഖ്യ തള്ളുക_mode = 0, add_mode = 0, delete_mode = 0;
	പൂർണ്ണസംഖ്യ matches = 0, negative_matches = 0;
	സ്ഥിരം അക്ഷരം *remotename = ശൂന്യ;
	സ്ഥിരം അക്ഷരം *newurl = ശൂന്യ;
	സ്ഥിരം അക്ഷരം *oldurl = ശൂന്യ;
	ഘടന remote *remote;
	regex_t old_regex;
	ഘടന strvec *urlset;
	ഘടന strbuf name_buf = STRBUF_INIT;
	ഘടന option options[] = {
		OPT_BOOL('\0', "push", &തള്ളുക_mode,
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

	എങ്കിൽ (add_mode && delete_mode)
		die(_("--add --delete doesn't make sense"));

	എങ്കിൽ (argc < 3 || argc > 4 || ((add_mode || delete_mode) && argc != 3))
		usage_with_options(builtin_remote_seturl_usage, options);

	remotename = argv[1];
	newurl = argv[2];
	എങ്കിൽ (argc > 3)
		oldurl = argv[3];

	എങ്കിൽ (delete_mode)
		oldurl = newurl;

	remote = remote_get(remotename);
	എങ്കിൽ (!remote_is_configured(remote, 1)) {
		error(_("No such remote '%s'"), remotename);
		exit(2);
	}

	എങ്കിൽ (തള്ളുക_mode) {
		strbuf_addf(&name_buf, "remote.%s.pushurl", remotename);
		urlset = &remote->pushurl;
	} അല്ലെങ്കിൽ {
		strbuf_addf(&name_buf, "remote.%s.url", remotename);
		urlset = &remote->url;
	}

	/* Special cases that add new entry. */
	എങ്കിൽ ((!oldurl && !delete_mode) || add_mode) {
		എങ്കിൽ (add_mode)
			repo_config_set_multivar(the_repository, name_buf.buf, newurl,
						       "^$", 0);
		അല്ലെങ്കിൽ
			repo_config_set(the_repository, name_buf.buf, newurl);
		പോകുക out;
	}

	/* Old URL specified. Demand that one matches. */
	എങ്കിൽ (regcomp(&old_regex, oldurl, REG_EXTENDED))
		die(_("Invalid old URL pattern: %s"), oldurl);

	വേണ്ടി (size_t i = 0; i < urlset->nr; i++)
		എങ്കിൽ (!regexec(&old_regex, urlset->v[i], 0, ശൂന്യ, 0))
			matches++;
		അല്ലെങ്കിൽ
			negative_matches++;
	എങ്കിൽ (!delete_mode && !matches)
		die(_("No such URL found: %s"), oldurl);
	എങ്കിൽ (delete_mode && !negative_matches && !തള്ളുക_mode)
		die(_("Will not delete all non-push URLs"));

	regfree(&old_regex);

	എങ്കിൽ (!delete_mode)
		repo_config_set_multivar(the_repository, name_buf.buf, newurl, oldurl, 0);
	അല്ലെങ്കിൽ
		repo_config_set_multivar(the_repository, name_buf.buf, ശൂന്യ, oldurl,
					 CONFIG_FLAGS_MULTI_REPLACE);
out:
	strbuf_release(&name_buf);
	തിരികെ 0;
}

പൂർണ്ണസംഖ്യ cmd_remote(പൂർണ്ണസംഖ്യ argc,
	       സ്ഥിരം അക്ഷരം **argv,
	       സ്ഥിരം അക്ഷരം *prefix,
	       ഘടന repository *repo)
{
	parse_opt_subcommand_fn *fn = ശൂന്യ;
	ഘടന option options[] = {
		OPT__VERBOSE(&verbose, N_("be verbose; must be placed before a subcommand")),
		OPT_SUBCOMMAND("add", &fn, add),
		OPT_SUBCOMMAND("rename", &fn, mv),
		OPT_SUBCOMMAND_F("rm", &fn, rm, ശൂന്യ, PARSE_OPT_NOCOMPLETE),
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

	എങ്കിൽ (fn) {
		തിരികെ !!fn(argc, argv, prefix, repo);
	} അല്ലെങ്കിൽ {
		എങ്കിൽ (argc) {
			error(_("unknown subcommand: `%s'"), argv[0]);
			usage_with_options(builtin_remote_usage, options);
		}
		തിരികെ !!show_all();
	}
}
