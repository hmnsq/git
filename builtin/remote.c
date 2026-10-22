#define USE_THE_REPOSITORY_VARIABLE

#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "advice.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "date.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "ident.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "path.h"
#அந்தர்பாவ "transport.h"
#அந்தர்பாவ "remote.h"
#அந்தர்பாவ "string-list.h"
#அந்தர்பாவ "strbuf.h"
#அந்தர்பாவ "run-command.h"
#அந்தர்பாவ "rebase.h"
#அந்தர்பாவ "refs.h"
#அந்தர்பாவ "refspec.h"
#அந்தர்பாவ "odb.h"
#அந்தர்பாவ "strvec.h"
#அந்தர்பாவ "commit-reach.h"
#அந்தர்பாவ "progress.h"

நிலையான மாறிலி எழுத்து * மாறிலி builtin_remote_usage[] = {
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
	வெற்று
};

நிலையான மாறிலி எழுத்து * மாறிலி builtin_remote_add_usage[] = {
	N_("git remote add [<options>] <name> <url>"),
	வெற்று
};

நிலையான மாறிலி எழுத்து * மாறிலி builtin_remote_rename_usage[] = {
	N_("git remote rename [--[no-]progress] <old> <new>"),
	வெற்று
};

நிலையான மாறிலி எழுத்து * மாறிலி builtin_remote_rm_usage[] = {
	N_("git remote remove <name>"),
	வெற்று
};

நிலையான மாறிலி எழுத்து * மாறிலி builtin_remote_sethead_usage[] = {
	N_("git remote set-head <name> (-a | --auto | -d | --delete | <branch>)"),
	வெற்று
};

நிலையான மாறிலி எழுத்து * மாறிலி builtin_remote_setbranches_usage[] = {
	N_("git remote set-branches <name> <branch>..."),
	N_("git remote set-branches --add <name> <branch>..."),
	வெற்று
};

நிலையான மாறிலி எழுத்து * மாறிலி builtin_remote_show_usage[] = {
	N_("git remote show [<options>] <name>"),
	வெற்று
};

நிலையான மாறிலி எழுத்து * மாறிலி builtin_remote_prune_usage[] = {
	N_("git remote prune [<options>] <name>"),
	வெற்று
};

நிலையான மாறிலி எழுத்து * மாறிலி builtin_remote_update_usage[] = {
	N_("git remote update [<options>] [<group> | <remote>]..."),
	வெற்று
};

நிலையான மாறிலி எழுத்து * மாறிலி builtin_remote_geturl_usage[] = {
	N_("git remote get-url [--push] [--all] <name>"),
	வெற்று
};

நிலையான மாறிலி எழுத்து * மாறிலி builtin_remote_seturl_usage[] = {
	N_("git remote set-url [--push] <name> <newurl> [<oldurl>]"),
	N_("git remote set-url --add <name> <newurl>"),
	N_("git remote set-url --delete <name> <url>"),
	வெற்று
};

#define GET_REF_STATES (1<<0)
#define GET_HEAD_NAMES (1<<1)
#define GET_தள்ளு_REF_STATES (1<<2)

நிலையான முழு verbose;

நிலையான முழு fetch_remote(மாறிலி எழுத்து *name)
{
	கட்டமைப்பு child_process cmd = CHILD_PROCESS_INIT;

	strvec_தள்ளு(&cmd.args, "fetch");
	எனில் (verbose)
		strvec_தள்ளு(&cmd.args, "-v");
	strvec_தள்ளு(&cmd.args, name);
	cmd.git_cmd = 1;
	printf_ln(_("Updating %s"), name);
	எனில் (run_command(&cmd))
		திரும்பு error(_("Could not fetch %s"), name);
	திரும்பு 0;
}

எண்ணிக்கை {
	TAGS_UNSET = 0,
	TAGS_DEFAULT = 1,
	TAGS_SET = 2
};

#define MIRROR_NONE 0
#define MIRROR_FETCH 1
#define MIRROR_தள்ளு 2
#define MIRROR_BOTH (MIRROR_FETCH|MIRROR_தள்ளு)

நிலையான வெறுமை add_branch(மாறிலி எழுத்து *சாவி, மாறிலி எழுத்து *branchname,
		       மாறிலி எழுத்து *remotename, முழு mirror, கட்டமைப்பு strbuf *tmp)
{
	strbuf_reset(tmp);
	strbuf_addch(tmp, '+');
	எனில் (mirror)
		strbuf_addf(tmp, "refs/%s:refs/%s",
				branchname, branchname);
	இல்லையெனில்
		strbuf_addf(tmp, "refs/heads/%s:refs/remotes/%s/%s",
				branchname, remotename, branchname);
	repo_config_set_multivar(the_repository, சாவி, tmp->buf, "^$", 0);
}

நிலையான மாறிலி எழுத்து mirror_advice[] =
N_("--mirror is dangerous and deprecated; please\n"
   "\t use --mirror=fetch or --mirror=push instead");

நிலையான முழு parse_mirror_opt(மாறிலி கட்டமைப்பு option *opt, மாறிலி எழுத்து *arg, முழு not)
{
	unsigned *mirror = opt->மதிப்பு;
	எனில் (not)
		*mirror = MIRROR_NONE;
	இல்லையெனில் எனில் (!arg) {
		warning("%s", _(mirror_advice));
		*mirror = MIRROR_BOTH;
	}
	இல்லையெனில் எனில் (!strcmp(arg, "fetch"))
		*mirror = MIRROR_FETCH;
	இல்லையெனில் எனில் (!strcmp(arg, "push"))
		*mirror = MIRROR_தள்ளு;
	இல்லையெனில்
		திரும்பு error(_("unknown --mirror argument: %s"), arg);
	திரும்பு 0;
}

நிலையான முழு check_remote_collision(கட்டமைப்பு remote *remote, வெறுமை *data)
{
	மாறிலி எழுத்து *name = data;
	மாறிலி எழுத்து *p;

	எனில் (skip_prefix(name, remote->name, &p) && *p == '/')
		die(_("remote name '%s' is a subset of existing remote '%s'"),
		    name, remote->name);
	எனில் (skip_prefix(remote->name, name, &p) && *p == '/')
		die(_("remote name '%s' is a superset of existing remote '%s'"),
		    name, remote->name);

	திரும்பு 0;
}

நிலையான முழு add(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
	       கட்டமைப்பு repository *repo UNUSED)
{
	முழு fetch = 0, fetch_tags = TAGS_DEFAULT;
	unsigned mirror = MIRROR_NONE;
	கட்டமைப்பு string_list track = STRING_LIST_INIT_NODUP;
	மாறிலி எழுத்து *master = வெற்று;
	கட்டமைப்பு remote *remote;
	கட்டமைப்பு strbuf buf = STRBUF_INIT, buf2 = STRBUF_INIT;
	மாறிலி எழுத்து *name, *url;
	முழு result = 0;

	கட்டமைப்பு option options[] = {
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

	எனில் (argc != 2)
		usage_with_options(builtin_remote_add_usage, options);

	எனில் (mirror && master)
		die(_("specifying a master branch makes no sense with --mirror"));
	எனில் (mirror && !(mirror & MIRROR_FETCH) && track.nr)
		die(_("specifying branches to track makes sense only with fetch mirrors"));

	name = argv[0];
	url = argv[1];

	remote = remote_get(name);
	எனில் (remote_is_configured(remote, 1)) {
		error(_("remote %s already exists."), name);
		exit(3);
	}

	எனில் (!valid_remote_name(name))
		die(_("'%s' is not a valid remote name"), name);

	for_each_remote(check_remote_collision, (வெறுமை *)name);

	strbuf_addf(&buf, "remote.%s.url", name);
	repo_config_set(the_repository, buf.buf, url);

	எனில் (!mirror || mirror & MIRROR_FETCH) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.fetch", name);
		எனில் (track.nr == 0)
			string_list_append(&track, "*");
		க்கு (size_t i = 0; i < track.nr; i++) {
			add_branch(buf.buf, track.பொருட்கள்[i].string,
				   name, mirror, &buf2);
		}
	}

	எனில் (mirror & MIRROR_தள்ளு) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.mirror", name);
		repo_config_set(the_repository, buf.buf, "true");
	}

	எனில் (fetch_tags != TAGS_DEFAULT) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.tagOpt", name);
		repo_config_set(the_repository, buf.buf,
				fetch_tags == TAGS_SET ? "--tags" : "--no-tags");
	}

	எனில் (fetch && fetch_remote(name)) {
		result = 1;
		செல் out;
	}

	எனில் (master) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "refs/remotes/%s/HEAD", name);

		strbuf_reset(&buf2);
		strbuf_addf(&buf2, "refs/remotes/%s/%s", name, master);

		எனில் (refs_update_symref(get_main_ref_store(the_repository), buf.buf, buf2.buf, "remote add"))
			result = error(_("Could not setup master '%s'"), master);
	}

out:
	strbuf_release(&buf);
	strbuf_release(&buf2);
	string_list_clear(&track, 0);

	திரும்பு result;
}

கட்டமைப்பு branch_info {
	எழுத்து *remote_name;
	கட்டமைப்பு string_list merge;
	எண்ணிக்கை rebase_type rebase;
	எழுத்து *தள்ளு_remote_name;
};

நிலையான கட்டமைப்பு string_list branch_list = STRING_LIST_INIT_DUP;

நிலையான மாறிலி எழுத்து *abbrev_ref(மாறிலி எழுத்து *name, மாறிலி எழுத்து *prefix)
{
	skip_prefix(name, prefix, &name);
	திரும்பு name;
}
#define abbrev_branch(name) abbrev_ref((name), "refs/heads/")

நிலையான முழு config_read_branches(மாறிலி எழுத்து *சாவி, மாறிலி எழுத்து *மதிப்பு,
				மாறிலி கட்டமைப்பு config_context *ctx UNUSED,
				வெறுமை *data UNUSED)
{
	மாறிலி எழுத்து *orig_சாவி = சாவி;
	எழுத்து *name;
	கட்டமைப்பு string_list_item *item;
	கட்டமைப்பு branch_info *info;
	எண்ணிக்கை { REMOTE, MERGE, REBASE, தள்ளு_REMOTE } type;
	size_t சாவி_len;

	எனில் (!starts_with(சாவி, "branch."))
		திரும்பு 0;

	சாவி += strlen("branch.");
	எனில் (strip_suffix(சாவி, ".remote", &சாவி_len))
		type = REMOTE;
	இல்லையெனில் எனில் (strip_suffix(சாவி, ".merge", &சாவி_len))
		type = MERGE;
	இல்லையெனில் எனில் (strip_suffix(சாவி, ".rebase", &சாவி_len))
		type = REBASE;
	இல்லையெனில் எனில் (strip_suffix(சாவி, ".pushremote", &சாவி_len))
		type = தள்ளு_REMOTE;
	இல்லையெனில்
		திரும்பு 0;

	name = xmemdupz(சாவி, சாவி_len);
	item = string_list_செருகு(&branch_list, name);

	எனில் (!item->util)
		item->util = xcalloc(1, அளவு(கட்டமைப்பு branch_info));
	info = item->util;
	தேர்வு (type) {
	நிலை REMOTE:
		எனில் (info->remote_name)
			warning(_("more than one %s"), orig_சாவி);
		info->remote_name = xstrdup(மதிப்பு);
		நிறுத்து;
	நிலை MERGE: {
		மாறிலி எழுத்து *space = strchr(மதிப்பு, ' ');
		மதிப்பு = abbrev_branch(மதிப்பு);
		போது (space) {
			எழுத்து *merge;
			merge = xstrndup(மதிப்பு, space - மதிப்பு);
			string_list_append(&info->merge, merge);
			மதிப்பு = abbrev_branch(space + 1);
			space = strchr(மதிப்பு, ' ');
		}
		string_list_append(&info->merge, xstrdup(மதிப்பு));
		நிறுத்து;
	}
	நிலை REBASE:
		/*
		 * Consider invalid values as false and check the
		 * truth value with >= REBASE_TRUE.
		 */
		info->rebase = rebase_parse_மதிப்பு(மதிப்பு);
		எனில் (info->rebase == REBASE_INVALID)
			warning(_("unhandled branch.%s.rebase=%s; assuming "
				  "'true'"), name, மதிப்பு);
		நிறுத்து;
	நிலை தள்ளு_REMOTE:
		எனில் (info->தள்ளு_remote_name)
			warning(_("more than one %s"), orig_சாவி);
		info->தள்ளு_remote_name = xstrdup(மதிப்பு);
		நிறுத்து;
	இயல்பு:
		BUG("unexpected type=%d", type);
	}

	விடுவி(name);
	திரும்பு 0;
}

நிலையான வெறுமை read_branches(வெறுமை)
{
	எனில் (branch_list.nr)
		திரும்பு;
	repo_config(the_repository, config_read_branches, வெற்று);
}

கட்டமைப்பு ref_states {
	கட்டமைப்பு remote *remote;
	கட்டமைப்பு string_list new_refs, skipped, stale, tracked, heads, தள்ளு;
	முழு queried;
};

#define REF_STATES_INIT { \
	.new_refs = STRING_LIST_INIT_DUP, \
	.skipped = STRING_LIST_INIT_DUP, \
	.stale = STRING_LIST_INIT_DUP, \
	.tracked = STRING_LIST_INIT_DUP, \
	.heads = STRING_LIST_INIT_DUP, \
	.தள்ளு = STRING_LIST_INIT_DUP, \
}

நிலையான முழு get_ref_states(மாறிலி கட்டமைப்பு ref *remote_refs, கட்டமைப்பு ref_states *states)
{
	கட்டமைப்பு ref *fetch_map = வெற்று, **tail = &fetch_map;
	கட்டமைப்பு ref *ref, *stale_refs;
	முழு i;

	க்கு (i = 0; i < states->remote->fetch.nr; i++)
		எனில் (get_fetch_map(remote_refs, &states->remote->fetch.பொருட்கள்[i], &tail, 1))
			die(_("Could not get fetch map for refspec %s"),
				states->remote->fetch.பொருட்கள்[i].raw);

	க்கு (ref = fetch_map; ref; ref = ref->next) {
		எனில் (refname_matches_negative_refspec_item(ref->name, &states->remote->fetch))
			string_list_append(&states->skipped, abbrev_branch(ref->name));
		இல்லையெனில் எனில் (!ref->peer_ref || !refs_ref_exists(get_main_ref_store(the_repository), ref->peer_ref->name))
			string_list_append(&states->new_refs, abbrev_branch(ref->name));
		இல்லையெனில்
			string_list_append(&states->tracked, abbrev_branch(ref->name));
	}
	stale_refs = get_stale_heads(&states->remote->fetch, fetch_map);
	க்கு (ref = stale_refs; ref; ref = ref->next) {
		கட்டமைப்பு string_list_item *item =
			string_list_append(&states->stale, abbrev_branch(ref->name));
		item->util = xstrdup(ref->name);
	}
	free_refs(stale_refs);
	free_refs(fetch_map);

	string_list_sort(&states->new_refs);
	string_list_sort(&states->skipped);
	string_list_sort(&states->tracked);
	string_list_sort(&states->stale);

	திரும்பு 0;
}

கட்டமைப்பு தள்ளு_info {
	எழுத்து *dest;
	முழு forced;
	எண்ணிக்கை {
		தள்ளு_STATUS_CREATE = 0,
		தள்ளு_STATUS_DELETE,
		தள்ளு_STATUS_UPTODATE,
		தள்ளு_STATUS_FASTFORWARD,
		தள்ளு_STATUS_OUTOFDATE,
		தள்ளு_STATUS_NOTQUERIED
	} status;
};

நிலையான முழு get_தள்ளு_ref_states(மாறிலி கட்டமைப்பு ref *remote_refs,
	கட்டமைப்பு ref_states *states)
{
	கட்டமைப்பு remote *remote = states->remote;
	கட்டமைப்பு ref *ref, *local_refs, *தள்ளு_map;
	எனில் (remote->mirror)
		திரும்பு 0;

	local_refs = get_local_heads();
	தள்ளு_map = copy_ref_list(remote_refs);

	match_தள்ளு_refs(local_refs, &தள்ளு_map, &remote->தள்ளு, MATCH_REFS_NONE);

	க்கு (ref = தள்ளு_map; ref; ref = ref->next) {
		கட்டமைப்பு string_list_item *item;
		கட்டமைப்பு தள்ளு_info *info;

		எனில் (!ref->peer_ref)
			தொடர்;
		oidcpy(&ref->new_oid, &ref->peer_ref->new_oid);

		item = string_list_append(&states->தள்ளு,
					  abbrev_branch(ref->peer_ref->name));
		item->util = xcalloc(1, அளவு(கட்டமைப்பு தள்ளு_info));
		info = item->util;
		info->forced = ref->force;
		info->dest = xstrdup(abbrev_branch(ref->name));

		எனில் (is_null_oid(&ref->new_oid)) {
			info->status = தள்ளு_STATUS_DELETE;
		} இல்லையெனில் எனில் (oideq(&ref->old_oid, &ref->new_oid))
			info->status = தள்ளு_STATUS_UPTODATE;
		இல்லையெனில் எனில் (is_null_oid(&ref->old_oid))
			info->status = தள்ளு_STATUS_CREATE;
		இல்லையெனில் எனில் (odb_has_object(the_repository->objects, &ref->old_oid,
					ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR) &&
			 ref_newer(&ref->new_oid, &ref->old_oid))
			info->status = தள்ளு_STATUS_FASTFORWARD;
		இல்லையெனில்
			info->status = தள்ளு_STATUS_OUTOFDATE;
	}
	free_refs(local_refs);
	free_refs(தள்ளு_map);
	திரும்பு 0;
}

நிலையான முழு get_தள்ளு_ref_states_noquery(கட்டமைப்பு ref_states *states)
{
	முழு i;
	கட்டமைப்பு remote *remote = states->remote;
	கட்டமைப்பு string_list_item *item;
	கட்டமைப்பு தள்ளு_info *info;

	எனில் (remote->mirror)
		திரும்பு 0;

	எனில் (!remote->தள்ளு.nr) {
		item = string_list_append(&states->தள்ளு, _("(matching)"));
		info = item->util = xcalloc(1, அளவு(கட்டமைப்பு தள்ளு_info));
		info->status = தள்ளு_STATUS_NOTQUERIED;
		info->dest = xstrdup(item->string);
	}
	க்கு (i = 0; i < remote->தள்ளு.nr; i++) {
		மாறிலி கட்டமைப்பு refspec_item *spec = &remote->தள்ளு.பொருட்கள்[i];
		எனில் (spec->matching)
			item = string_list_append(&states->தள்ளு, _("(matching)"));
		இல்லையெனில் எனில் (strlen(spec->src))
			item = string_list_append(&states->தள்ளு, spec->src);
		இல்லையெனில்
			item = string_list_append(&states->தள்ளு, _("(delete)"));

		info = item->util = xcalloc(1, அளவு(கட்டமைப்பு தள்ளு_info));
		info->forced = spec->force;
		info->status = தள்ளு_STATUS_NOTQUERIED;
		info->dest = xstrdup(spec->dst ? spec->dst : item->string);
	}
	திரும்பு 0;
}

நிலையான முழு get_head_names(மாறிலி கட்டமைப்பு ref *remote_refs, கட்டமைப்பு ref_states *states)
{
	கட்டமைப்பு ref *ref, *matches;
	கட்டமைப்பு ref *fetch_map = வெற்று, **fetch_map_tail = &fetch_map;
	கட்டமைப்பு refspec_item refspec = {
		.force = 0,
		.pattern = 1,
		.src = (எழுத்து *) "refs/heads/*",
		.dst = (எழுத்து *) "refs/heads/*",
	};

	get_fetch_map(remote_refs, &refspec, &fetch_map_tail, 0);
	matches = guess_remote_head(find_ref_by_name(remote_refs, "HEAD"),
				    fetch_map, REMOTE_GUESS_HEAD_ALL);
	க்கு (ref = matches; ref; ref = ref->next)
		string_list_append(&states->heads, abbrev_branch(ref->name));

	free_refs(fetch_map);
	free_refs(matches);
	திரும்பு 0;
}

கட்டமைப்பு known_remote {
	கட்டமைப்பு known_remote *next;
	கட்டமைப்பு remote *remote;
};

கட்டமைப்பு known_remotes {
	கட்டமைப்பு remote *to_delete;
	கட்டமைப்பு known_remote *list;
};

நிலையான முழு add_known_remote(கட்டமைப்பு remote *remote, வெறுமை *cb_data)
{
	கட்டமைப்பு known_remotes *all = cb_data;
	கட்டமைப்பு known_remote *r;

	எனில் (!strcmp(all->to_delete->name, remote->name))
		திரும்பு 0;

	r = xmalloc(அளவு(*r));
	r->remote = remote;
	r->next = all->list;
	all->list = r;
	திரும்பு 0;
}

கட்டமைப்பு branches_for_remote {
	கட்டமைப்பு remote *remote;
	கட்டமைப்பு string_list *branches, *skipped;
	கட்டமைப்பு known_remotes *keep;
};

நிலையான முழு add_branch_for_removal(மாறிலி கட்டமைப்பு reference *ref, வெறுமை *cb_data)
{
	கட்டமைப்பு branches_for_remote *branches = cb_data;
	கட்டமைப்பு refspec_item refspec;
	கட்டமைப்பு known_remote *kr;

	memset(&refspec, 0, அளவு(refspec));
	refspec.dst = (எழுத்து *)ref->name;
	எனில் (remote_find_tracking(branches->remote, &refspec))
		திரும்பு 0;
	விடுவி(refspec.src);

	/* don't delete a branch if another remote also uses it */
	க்கு (kr = branches->keep->list; kr; kr = kr->next) {
		memset(&refspec, 0, அளவு(refspec));
		refspec.dst = (எழுத்து *)ref->name;
		எனில் (!remote_find_tracking(kr->remote, &refspec)) {
			விடுவி(refspec.src);
			திரும்பு 0;
		}
	}

	/* don't delete non-remote-tracking refs */
	எனில் (!starts_with(ref->name, "refs/remotes/")) {
		/* advise user how to delete local branches */
		எனில் (starts_with(ref->name, "refs/heads/"))
			string_list_append(branches->skipped,
					   abbrev_branch(ref->name));
		/* silently skip over other non-remote refs */
		திரும்பு 0;
	}

	string_list_append(branches->branches, ref->name);

	திரும்பு 0;
}

கட்டமைப்பு rename_info {
	மாறிலி எழுத்து *old_name;
	மாறிலி எழுத்து *new_name;
	கட்டமைப்பு ref_transaction *transaction;
	கட்டமைப்பு progress *progress;
	கட்டமைப்பு strbuf *err;
	uint32_t progress_nr;
	uint64_t index;
};

நிலையான வெறுமை compute_renamed_ref(கட்டமைப்பு rename_info *rename,
				மாறிலி எழுத்து *refname,
				கட்டமைப்பு strbuf *out)
{
	strbuf_reset(out);
	strbuf_addstr(out, refname);
	strbuf_splice(out, strlen("refs/remotes/"), strlen(rename->old_name),
		      rename->new_name, strlen(rename->new_name));
}

நிலையான முழு rename_one_reflog_entry(மாறிலி எழுத்து *old_refname,
				   கட்டமைப்பு object_id *old_oid,
				   கட்டமைப்பு object_id *new_oid,
				   மாறிலி எழுத்து *committer,
				   timestamp_t timestamp, முழு tz,
				   மாறிலி எழுத்து *செய்தி, வெறுமை *cb_data)
{
	கட்டமைப்பு rename_info *rename = cb_data;
	கட்டமைப்பு strbuf new_refname = STRBUF_INIT;
	கட்டமைப்பு strbuf identity = STRBUF_INIT;
	கட்டமைப்பு strbuf name = STRBUF_INIT;
	கட்டமைப்பு strbuf mail = STRBUF_INIT;
	கட்டமைப்பு ident_split ident;
	மாறிலி எழுத்து *date;
	முழு error;

	compute_renamed_ref(rename, old_refname, &new_refname);

	எனில் (split_ident_வரி(&ident, committer, strlen(committer)) < 0) {
		error = -1;
		செல் out;
	}

	strbuf_add(&name, ident.name_begin, ident.name_end - ident.name_begin);
	strbuf_add(&mail, ident.mail_begin, ident.mail_end - ident.mail_begin);

	date = show_date(timestamp, tz, DATE_MODE(NORMAL));
	strbuf_addstr(&identity, fmt_ident(name.buf, mail.buf,
					  WANT_BLANK_IDENT, date, 0));

	error = ref_transaction_update_reflog(rename->transaction, new_refname.buf,
					      new_oid, old_oid, identity.buf, செய்தி,
					      rename->index++, rename->err);

out:
	strbuf_release(&new_refname);
	strbuf_release(&identity);
	strbuf_release(&name);
	strbuf_release(&mail);
	திரும்பு error;
}

நிலையான முழு rename_one_reflog(மாறிலி எழுத்து *old_refname,
			     மாறிலி கட்டமைப்பு object_id *old_oid,
			     கட்டமைப்பு rename_info *rename)
{
	கட்டமைப்பு strbuf new_refname = STRBUF_INIT;
	கட்டமைப்பு strbuf message = STRBUF_INIT;
	முழு error;

	எனில் (!refs_reflog_exists(get_main_ref_store(the_repository), old_refname))
		திரும்பு 0;

	error = refs_for_each_reflog_ent(get_main_ref_store(the_repository),
					 old_refname, rename_one_reflog_entry, rename);
	எனில் (error < 0)
		செல் out;

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
	எனில் (error < 0)
		திரும்பு error;

out:
	strbuf_release(&new_refname);
	strbuf_release(&message);
	திரும்பு error;
}

நிலையான முழு rename_one_ref(மாறிலி கட்டமைப்பு reference *ref, வெறுமை *cb_data)
{
	கட்டமைப்பு strbuf new_referent = STRBUF_INIT;
	கட்டமைப்பு strbuf new_refname = STRBUF_INIT;
	கட்டமைப்பு rename_info *rename = cb_data;
	மாறிலி கட்டமைப்பு object_id *oid = ref->oid;
	மாறிலி எழுத்து *referent = ref->target;
	முழு error;

	compute_renamed_ref(rename, ref->name, &new_refname);

	எனில் (ref->flags & REF_ISSYMREF) {
		/*
		 * Stupidly enough `referent` is not pointing to the immediate
		 * target of a symref, but it's the recursively resolved value.
		 * So symrefs pointing to symrefs would be misresolved, and
		 * unborn symrefs don't have any value for the `referent` at all.
		 */
		referent = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
						   ref->name, RESOLVE_REF_NO_RECURSE,
						   வெற்று, வெற்று);
		compute_renamed_ref(rename, referent, &new_referent);
		oid = வெற்று;
	}

	error = ref_transaction_delete(rename->transaction, ref->name,
				       oid, referent, REF_NO_DEREF, வெற்று, rename->err);
	எனில் (error < 0)
		செல் out;

	error = ref_transaction_update(rename->transaction, new_refname.buf, oid, null_oid(the_hash_algo),
				       (ref->flags & REF_ISSYMREF) ? new_referent.buf : வெற்று, வெற்று,
				       REF_SKIP_CREATE_REFLOG | REF_NO_DEREF | REF_SKIP_OID_VERIFICATION,
				       வெற்று, rename->err);
	எனில் (error < 0)
		செல் out;

	error = rename_one_reflog(ref->name, oid, rename);
	எனில் (error < 0)
		செல் out;

	display_progress(rename->progress, ++rename->progress_nr);

out:
	strbuf_release(&new_referent);
	strbuf_release(&new_refname);
	திரும்பு error;
}

நிலையான முழு migrate_file(கட்டமைப்பு remote *remote)
{
	கட்டமைப்பு strbuf buf = STRBUF_INIT;

	strbuf_addf(&buf, "remote.%s.url", remote->name);
	க்கு (size_t i = 0; i < remote->url.nr; i++)
		repo_config_set_multivar(the_repository, buf.buf, remote->url.v[i], "^$", 0);
	strbuf_reset(&buf);
	strbuf_addf(&buf, "remote.%s.push", remote->name);
	க்கு (முழு i = 0; i < remote->தள்ளு.nr; i++)
		repo_config_set_multivar(the_repository, buf.buf, remote->தள்ளு.பொருட்கள்[i].raw, "^$", 0);
	strbuf_reset(&buf);
	strbuf_addf(&buf, "remote.%s.fetch", remote->name);
	க்கு (முழு i = 0; i < remote->fetch.nr; i++)
		repo_config_set_multivar(the_repository, buf.buf, remote->fetch.பொருட்கள்[i].raw, "^$", 0);
#ifndef WITH_BREAKING_CHANGES
	எனில் (remote->origin == REMOTE_REMOTES)
		unlink_or_warn(repo_git_path_replace(the_repository, &buf,
						     "remotes/%s", remote->name));
	இல்லையெனில் எனில் (remote->origin == REMOTE_BRANCHES)
		unlink_or_warn(repo_git_path_replace(the_repository, &buf,
						     "branches/%s", remote->name));
#endif /* WITH_BREAKING_CHANGES */
	strbuf_release(&buf);

	திரும்பு 0;
}

கட்டமைப்பு தள்ளு_default_info
{
	மாறிலி எழுத்து *old_name;
	எண்ணிக்கை config_scope scope;
	கட்டமைப்பு strbuf origin;
	முழு linenr;
};

நிலையான முழு config_read_தள்ளு_default(மாறிலி எழுத்து *சாவி, மாறிலி எழுத்து *மதிப்பு,
	மாறிலி கட்டமைப்பு config_context *ctx, வெறுமை *cb)
{
	மாறிலி கட்டமைப்பு சாவி_மதிப்பு_info *kvi = ctx->kvi;

	கட்டமைப்பு தள்ளு_default_info* info = cb;
	எனில் (strcmp(சாவி, "remote.pushdefault") ||
	    !மதிப்பு || strcmp(மதிப்பு, info->old_name))
		திரும்பு 0;

	info->scope = kvi->scope;
	strbuf_reset(&info->origin);
	strbuf_addstr(&info->origin, config_origin_type_name(kvi->origin_type));
	info->linenr = kvi->linenr;

	திரும்பு 0;
}

நிலையான வெறுமை handle_தள்ளு_default(மாறிலி எழுத்து* old_name, மாறிலி எழுத்து* new_name)
{
	கட்டமைப்பு தள்ளு_default_info தள்ளு_default = {
		.old_name = old_name,
		.scope = CONFIG_SCOPE_UNKNOWN,
		.origin = STRBUF_INIT,
		.linenr = -1,
	};
	repo_config(the_repository, config_read_தள்ளு_default, &தள்ளு_default);
	எனில் (தள்ளு_default.scope >= CONFIG_SCOPE_COMMAND)
		; /* pass */
	இல்லையெனில் எனில் (தள்ளு_default.scope >= CONFIG_SCOPE_LOCAL) {
		முழு result = repo_config_set_gently(the_repository, "remote.pushDefault",
						    new_name);
		எனில் (new_name && result && result != CONFIG_NOTHING_SET)
			die(_("could not set '%s'"), "remote.pushDefault");
		இல்லையெனில் எனில் (!new_name && result && result != CONFIG_NOTHING_SET)
			die(_("could not unset '%s'"), "remote.pushDefault");
	} இல்லையெனில் எனில் (தள்ளு_default.scope >= CONFIG_SCOPE_SYSTEM) {
		/* warn */
		warning(_("The %s configuration remote.pushDefault in:\n"
			  "\t%s:%d\n"
			  "now names the non-existent remote '%s'"),
			config_scope_name(தள்ளு_default.scope),
			தள்ளு_default.origin.buf, தள்ளு_default.linenr,
			old_name);
	}

	strbuf_release(&தள்ளு_default.origin);
}

நிலையான மாறிலி எழுத்து conflicting_remote_refs_advice[] = N_(
	"The remote you are trying to rename has conflicting references in the\n"
	"new target refspec. This is most likely caused by you trying to nest\n"
	"a remote into itself, e.g. by renaming 'parent' into 'parent/child'\n"
	"or by unnesting a remote, e.g. the other way round.\n"
	"\n"
	"If that is the case, you can address this by first renaming the\n"
	"remote to a different name.\n");

நிலையான முழு mv(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
	      கட்டமைப்பு repository *repo UNUSED)
{
	முழு show_progress = isatty(2);
	கட்டமைப்பு option options[] = {
		OPT_BOOL(0, "progress", &show_progress, N_("force progress reporting")),
		OPT_END()
	};
	கட்டமைப்பு remote *oldremote, *newremote;
	கட்டமைப்பு strbuf buf = STRBUF_INIT, buf2 = STRBUF_INIT, buf3 = STRBUF_INIT,
		old_remote_context = STRBUF_INIT, err = STRBUF_INIT;
	கட்டமைப்பு rename_info rename = {
		.err = &err,
	};
	முழு refspecs_need_update = 0;
	முழு result = 0;

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_rename_usage, 0);

	எனில் (argc != 2)
		usage_with_options(builtin_remote_rename_usage, options);

	rename.old_name = argv[0];
	rename.new_name = argv[1];

	oldremote = remote_get(rename.old_name);
	எனில் (!remote_is_configured(oldremote, 1)) {
		error(_("No such remote: '%s'"), rename.old_name);
		exit(2);
	}

	எனில் (!strcmp(rename.old_name, rename.new_name) && oldremote->origin != REMOTE_CONFIG)
		திரும்பு migrate_file(oldremote);

	newremote = remote_get(rename.new_name);
	எனில் (remote_is_configured(newremote, 1)) {
		error(_("remote %s already exists."), rename.new_name);
		exit(3);
	}

	எனில் (!valid_remote_name(rename.new_name))
		die(_("'%s' is not a valid remote name"), rename.new_name);

	strbuf_addf(&buf, "remote.%s", rename.old_name);
	strbuf_addf(&buf2, "remote.%s", rename.new_name);
	எனில் (repo_config_rename_section(the_repository, buf.buf, buf2.buf) < 1) {
		result = error(_("Could not rename config section '%s' to '%s'"),
			       buf.buf, buf2.buf);
		செல் out;
	}

	strbuf_addf(&old_remote_context, ":refs/remotes/%s/", rename.old_name);

	க்கு (முழு i = 0; i < oldremote->fetch.nr && !refspecs_need_update; i++)
		refspecs_need_update = !!strstr(oldremote->fetch.பொருட்கள்[i].raw,
						old_remote_context.buf);

	எனில் (refspecs_need_update) {
		கட்டமைப்பு refs_for_each_ref_options opts = {
			.flags = REFS_FOR_EACH_INCLUDE_BROKEN,
		};
		rename.transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
							       0, &err);
		எனில் (!rename.transaction)
			செல் out;

		எனில் (show_progress)
			rename.progress = start_delayed_progress(the_repository,
								 _("Renaming remote references"), 0);

		strbuf_reset(&buf);
		strbuf_addf(&buf, "refs/remotes/%s/", rename.old_name);
		opts.prefix = buf.buf;

		result = refs_for_each_ref_ext(get_main_ref_store(the_repository),
					       rename_one_ref, &rename, &opts);
		எனில் (result < 0)
			die(_("queueing remote ref renames failed: %s"), rename.err->buf);

		result = ref_transaction_prepare(rename.transaction, &err);
		எனில் (result < 0) {
			error("renaming remote references failed: %s", err.buf);
			எனில் (result == REF_TRANSACTION_ERROR_NAME_CONFLICT)
				advise(conflicting_remote_refs_advice);
			die(வெற்று);
		}
	}

	எனில் (oldremote->fetch.nr) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.fetch", rename.new_name);
		repo_config_set_multivar(the_repository, buf.buf, வெற்று, வெற்று, CONFIG_FLAGS_MULTI_REPLACE);
		க்கு (முழு i = 0; i < oldremote->fetch.nr; i++) {
			எழுத்து *ptr;

			strbuf_reset(&buf2);
			strbuf_addstr(&buf2, oldremote->fetch.பொருட்கள்[i].raw);
			ptr = strstr(buf2.buf, old_remote_context.buf);
			எனில் (ptr) {
				strbuf_splice(&buf2,
					      ptr-buf2.buf + strlen(":refs/remotes/"),
					      strlen(rename.old_name), rename.new_name,
					      strlen(rename.new_name));
			} இல்லையெனில்
				warning(_("Not updating non-default fetch refspec\n"
					  "\t%s\n"
					  "\tPlease update the configuration manually if necessary."),
					buf2.buf);

			repo_config_set_multivar(the_repository, buf.buf, buf2.buf, "^$", 0);
		}
	}

	read_branches();
	க்கு (size_t i = 0; i < branch_list.nr; i++) {
		கட்டமைப்பு string_list_item *item = branch_list.பொருட்கள் + i;
		கட்டமைப்பு branch_info *info = item->util;
		எனில் (info->remote_name && !strcmp(info->remote_name, rename.old_name)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "branch.%s.remote", item->string);
			repo_config_set(the_repository, buf.buf, rename.new_name);
		}
		எனில் (info->தள்ளு_remote_name && !strcmp(info->தள்ளு_remote_name, rename.old_name)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "branch.%s.pushRemote", item->string);
			repo_config_set(the_repository, buf.buf, rename.new_name);
		}
	}

	எனில் (refspecs_need_update) {
		result = ref_transaction_commit(rename.transaction, &err);
		எனில் (result < 0)
			die(_("renaming remote refs failed: %s"), rename.err->buf);

		stop_progress(&rename.progress);

		handle_தள்ளு_default(rename.old_name, rename.new_name);
	}

out:
	ref_transaction_free(rename.transaction);
	strbuf_release(&old_remote_context);
	strbuf_release(&buf);
	strbuf_release(&buf2);
	strbuf_release(&buf3);
	strbuf_release(&err);
	திரும்பு result;
}

நிலையான முழு rm(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
	      கட்டமைப்பு repository *repo UNUSED)
{
	கட்டமைப்பு option options[] = {
		OPT_END()
	};
	கட்டமைப்பு remote *remote;
	கட்டமைப்பு strbuf buf = STRBUF_INIT;
	கட்டமைப்பு known_remotes known_remotes = { வெற்று, வெற்று };
	கட்டமைப்பு string_list branches = STRING_LIST_INIT_DUP;
	கட்டமைப்பு string_list skipped = STRING_LIST_INIT_DUP;
	கட்டமைப்பு branches_for_remote cb_data;
	முழு result;

	memset(&cb_data, 0, அளவு(cb_data));
	cb_data.branches = &branches;
	cb_data.skipped = &skipped;
	cb_data.keep = &known_remotes;

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_rm_usage, 0);
	எனில் (argc != 1)
		usage_with_options(builtin_remote_rm_usage, options);

	remote = remote_get(argv[0]);
	எனில் (!remote_is_configured(remote, 1)) {
		error(_("No such remote: '%s'"), argv[0]);
		exit(2);
	}

	known_remotes.to_delete = remote;
	for_each_remote(add_known_remote, &known_remotes);

	read_branches();
	க்கு (size_t i = 0; i < branch_list.nr; i++) {
		கட்டமைப்பு string_list_item *item = branch_list.பொருட்கள் + i;
		கட்டமைப்பு branch_info *info = item->util;
		எனில் (info->remote_name && !strcmp(info->remote_name, remote->name)) {
			மாறிலி எழுத்து *சாவிகள்[] = { "remote", "merge", வெற்று }, **k;
			க்கு (k = சாவிகள்; *k; k++) {
				strbuf_reset(&buf);
				strbuf_addf(&buf, "branch.%s.%s",
						item->string, *k);
				result = repo_config_set_gently(the_repository, buf.buf, வெற்று);
				எனில் (result && result != CONFIG_NOTHING_SET)
					die(_("could not unset '%s'"), buf.buf);
			}
		}
		எனில் (info->தள்ளு_remote_name && !strcmp(info->தள்ளு_remote_name, remote->name)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "branch.%s.pushremote", item->string);
			result = repo_config_set_gently(the_repository, buf.buf, வெற்று);
			எனில் (result && result != CONFIG_NOTHING_SET)
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

	எனில் (!result)
		result = refs_delete_refs(get_main_ref_store(the_repository),
					  "remote: remove", &branches,
					  REF_NO_DEREF);
	string_list_clear(&branches, 0);

	எனில் (skipped.nr) {
		fprintf_ln(stderr,
			   Q_("Note: A branch outside the refs/remotes/ hierarchy was not removed;\n"
			      "to delete it, use:",
			      "Note: Some branches outside the refs/remotes/ hierarchy were not removed;\n"
			      "to delete them, use:",
			      skipped.nr));
		க்கு (size_t i = 0; i < skipped.nr; i++)
			fprintf(stderr, "  git branch -d %s\n",
				skipped.பொருட்கள்[i].string);
	}
	string_list_clear(&skipped, 0);

	எனில் (!result) {
		strbuf_addf(&buf, "remote.%s", remote->name);
		எனில் (repo_config_rename_section(the_repository, buf.buf, வெற்று) < 1) {
			result = error(_("Could not remove config section '%s'"), buf.buf);
			செல் out;
		}

		handle_தள்ளு_default(remote->name, வெற்று);
	}

out:
	க்கு (கட்டமைப்பு known_remote *r = known_remotes.list; r;) {
		கட்டமைப்பு known_remote *next = r->next;
		விடுவி(r);
		r = next;
	}
	strbuf_release(&buf);
	திரும்பு result;
}

நிலையான வெறுமை clear_தள்ளு_info(வெறுமை *util, மாறிலி எழுத்து *string UNUSED)
{
	கட்டமைப்பு தள்ளு_info *info = util;
	விடுவி(info->dest);
	விடுவி(info);
}

நிலையான வெறுமை free_remote_ref_states(கட்டமைப்பு ref_states *states)
{
	string_list_clear(&states->new_refs, 0);
	string_list_clear(&states->skipped, 0);
	string_list_clear(&states->stale, 1);
	string_list_clear(&states->tracked, 0);
	string_list_clear(&states->heads, 0);
	string_list_clear_func(&states->தள்ளு, clear_தள்ளு_info);
}

நிலையான முழு append_ref_to_tracked_list(மாறிலி கட்டமைப்பு reference *ref, வெறுமை *cb_data)
{
	கட்டமைப்பு ref_states *states = cb_data;
	கட்டமைப்பு refspec_item refspec;

	எனில் (ref->flags & REF_ISSYMREF)
		திரும்பு 0;

	memset(&refspec, 0, அளவு(refspec));
	refspec.dst = (எழுத்து *)ref->name;
	எனில் (!remote_find_tracking(states->remote, &refspec)) {
		string_list_append(&states->tracked, abbrev_branch(refspec.src));
		விடுவி(refspec.src);
	}

	திரும்பு 0;
}

நிலையான முழு get_remote_ref_states(மாறிலி எழுத்து *name,
				 கட்டமைப்பு ref_states *states,
				 முழு query)
{
	states->remote = remote_get(name);
	எனில் (!states->remote)
		திரும்பு error(_("No such remote: '%s'"), name);

	read_branches();

	எனில் (query) {
		கட்டமைப்பு transport *transport;
		மாறிலி கட்டமைப்பு ref *remote_refs;

		transport = transport_get(states->remote, states->remote->url.v[0]);
		remote_refs = transport_get_remote_refs(transport, வெற்று);

		states->queried = 1;
		எனில் (query & GET_REF_STATES)
			get_ref_states(remote_refs, states);
		எனில் (query & GET_HEAD_NAMES)
			get_head_names(remote_refs, states);
		எனில் (query & GET_தள்ளு_REF_STATES)
			get_தள்ளு_ref_states(remote_refs, states);
		transport_disconnect(transport);
	} இல்லையெனில் {
		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_ref_to_tracked_list, states);
		string_list_sort(&states->tracked);
		get_தள்ளு_ref_states_noquery(states);
	}

	திரும்பு 0;
}

கட்டமைப்பு show_info {
	கட்டமைப்பு string_list list;
	கட்டமைப்பு ref_states states;
	முழு width, width2;
	முழு any_rebase;
};

#define SHOW_INFO_INIT { \
	.list = STRING_LIST_INIT_DUP, \
	.states = REF_STATES_INIT, \
}

நிலையான முழு add_remote_to_show_info(கட்டமைப்பு string_list_item *item, வெறுமை *cb_data)
{
	கட்டமைப்பு show_info *info = cb_data;
	முழு n = strlen(item->string);
	எனில் (n > info->width)
		info->width = n;
	string_list_செருகு(&info->list, item->string);
	திரும்பு 0;
}

நிலையான முழு show_remote_info_item(கட்டமைப்பு string_list_item *item, வெறுமை *cb_data)
{
	கட்டமைப்பு show_info *info = cb_data;
	கட்டமைப்பு ref_states *states = &info->states;
	மாறிலி எழுத்து *name = item->string;

	எனில் (states->queried) {
		மாறிலி எழுத்து *fmt = "%s";
		மாறிலி எழுத்து *arg = "";
		எனில் (string_list_has_string(&states->new_refs, name)) {
			fmt = _(" new (next fetch will store in remotes/%s)");
			arg = states->remote->name;
		} இல்லையெனில் எனில் (string_list_has_string(&states->tracked, name))
			arg = _(" tracked");
		இல்லையெனில் எனில் (string_list_has_string(&states->skipped, name))
			arg = _(" skipped");
		இல்லையெனில் எனில் (string_list_has_string(&states->stale, name))
			arg = _(" stale (use 'git remote prune' to remove)");
		இல்லையெனில்
			arg = _(" ???");
		printf("    %-*s", info->width, name);
		printf(fmt, arg);
		printf("\n");
	} இல்லையெனில்
		printf("    %s\n", name);

	திரும்பு 0;
}

நிலையான முழு add_local_to_show_info(கட்டமைப்பு string_list_item *branch_item, வெறுமை *cb_data)
{
	கட்டமைப்பு show_info *show_info = cb_data;
	கட்டமைப்பு ref_states *states = &show_info->states;
	கட்டமைப்பு branch_info *branch_info = branch_item->util;
	கட்டமைப்பு string_list_item *item;
	முழு n;

	எனில் (!branch_info->merge.nr || !branch_info->remote_name ||
	    strcmp(states->remote->name, branch_info->remote_name))
		திரும்பு 0;
	எனில் ((n = strlen(branch_item->string)) > show_info->width)
		show_info->width = n;
	எனில் (branch_info->rebase >= REBASE_TRUE)
		show_info->any_rebase = 1;

	item = string_list_செருகு(&show_info->list, branch_item->string);
	item->util = branch_info;

	திரும்பு 0;
}

நிலையான முழு show_local_info_item(கட்டமைப்பு string_list_item *item, வெறுமை *cb_data)
{
	கட்டமைப்பு show_info *show_info = cb_data;
	கட்டமைப்பு branch_info *branch_info = item->util;
	கட்டமைப்பு string_list *merge = &branch_info->merge;
	முழு width = show_info->width + 4;

	எனில் (branch_info->rebase >= REBASE_TRUE && branch_info->merge.nr > 1) {
		error(_("invalid branch.%s.merge; cannot rebase onto > 1 branch"),
			item->string);
		திரும்பு 0;
	}

	printf("    %-*s ", show_info->width, item->string);
	எனில் (branch_info->rebase >= REBASE_TRUE) {
		மாறிலி எழுத்து *செய்தி;
		எனில் (branch_info->rebase == REBASE_INTERACTIVE)
			செய்தி = _("rebases interactively onto remote %s");
		இல்லையெனில் எனில் (branch_info->rebase == REBASE_MERGES)
			செய்தி = _("rebases interactively (with merges) onto "
				"remote %s");
		இல்லையெனில்
			செய்தி = _("rebases onto remote %s");
		printf_ln(செய்தி, merge->பொருட்கள்[0].string);
		திரும்பு 0;
	} இல்லையெனில் எனில் (show_info->any_rebase) {
		printf_ln(_(" merges with remote %s"), merge->பொருட்கள்[0].string);
		width++;
	} இல்லையெனில் {
		printf_ln(_("merges with remote %s"), merge->பொருட்கள்[0].string);
	}
	க்கு (size_t i = 1; i < merge->nr; i++)
		printf(_("%-*s    and with remote %s\n"), width, "",
		       merge->பொருட்கள்[i].string);

	திரும்பு 0;
}

நிலையான முழு add_தள்ளு_to_show_info(கட்டமைப்பு string_list_item *தள்ளு_item, வெறுமை *cb_data)
{
	கட்டமைப்பு show_info *show_info = cb_data;
	கட்டமைப்பு தள்ளு_info *தள்ளு_info = தள்ளு_item->util;
	கட்டமைப்பு string_list_item *item;
	முழு n;
	எனில் ((n = strlen(தள்ளு_item->string)) > show_info->width)
		show_info->width = n;
	எனில் ((n = strlen(தள்ளு_info->dest)) > show_info->width2)
		show_info->width2 = n;
	item = string_list_append(&show_info->list, தள்ளு_item->string);
	item->util = தள்ளு_item->util;
	திரும்பு 0;
}

/*
 * Sorting comparison for a string list that has push_info
 * structs in its util field
 */
நிலையான முழு cmp_string_with_தள்ளு(மாறிலி வெறுமை *va, மாறிலி வெறுமை *vb)
{
	மாறிலி கட்டமைப்பு string_list_item *a = va;
	மாறிலி கட்டமைப்பு string_list_item *b = vb;
	மாறிலி கட்டமைப்பு தள்ளு_info *a_தள்ளு = a->util;
	மாறிலி கட்டமைப்பு தள்ளு_info *b_தள்ளு = b->util;
	முழு cmp = strcmp(a->string, b->string);
	திரும்பு cmp ? cmp : strcmp(a_தள்ளு->dest, b_தள்ளு->dest);
}

நிலையான முழு show_தள்ளு_info_item(கட்டமைப்பு string_list_item *item, வெறுமை *cb_data)
{
	கட்டமைப்பு show_info *show_info = cb_data;
	கட்டமைப்பு தள்ளு_info *தள்ளு_info = item->util;
	மாறிலி எழுத்து *src = item->string, *status = வெற்று;

	தேர்வு (தள்ளு_info->status) {
	நிலை தள்ளு_STATUS_CREATE:
		status = _("create");
		நிறுத்து;
	நிலை தள்ளு_STATUS_DELETE:
		status = _("delete");
		src = _("(none)");
		நிறுத்து;
	நிலை தள்ளு_STATUS_UPTODATE:
		status = _("up to date");
		நிறுத்து;
	நிலை தள்ளு_STATUS_FASTFORWARD:
		status = _("fast-forwardable");
		நிறுத்து;
	நிலை தள்ளு_STATUS_OUTOFDATE:
		status = _("local out of date");
		நிறுத்து;
	நிலை தள்ளு_STATUS_NOTQUERIED:
		நிறுத்து;
	}
	எனில் (status) {
		எனில் (தள்ளு_info->forced)
			printf_ln(_("    %-*s forces to %-*s (%s)"), show_info->width, src,
			       show_info->width2, தள்ளு_info->dest, status);
		இல்லையெனில்
			printf_ln(_("    %-*s pushes to %-*s (%s)"), show_info->width, src,
			       show_info->width2, தள்ளு_info->dest, status);
	} இல்லையெனில் {
		எனில் (தள்ளு_info->forced)
			printf_ln(_("    %-*s forces to %s"), show_info->width, src,
			       தள்ளு_info->dest);
		இல்லையெனில்
			printf_ln(_("    %-*s pushes to %s"), show_info->width, src,
			       தள்ளு_info->dest);
	}
	திரும்பு 0;
}

நிலையான முழு get_one_entry(கட்டமைப்பு remote *remote, வெறுமை *priv)
{
	கட்டமைப்பு string_list *list = priv;
	கட்டமைப்பு strbuf remote_info_buf = STRBUF_INIT;
	கட்டமைப்பு strvec *url;

	எனில் (remote->url.nr > 0) {
		கட்டமைப்பு strbuf promisor_config = STRBUF_INIT;
		மாறிலி எழுத்து *partial_clone_filter = வெற்று;

		strbuf_addf(&promisor_config, "remote.%s.partialclonefilter", remote->name);
		strbuf_addf(&remote_info_buf, "%s (fetch)", remote->url.v[0]);
		எனில் (!repo_config_get_string_tmp(the_repository, promisor_config.buf, &partial_clone_filter))
			strbuf_addf(&remote_info_buf, " [%s]", partial_clone_filter);

		strbuf_release(&promisor_config);
		string_list_append(list, remote->name)->util =
				strbuf_detach(&remote_info_buf, வெற்று);
	} இல்லையெனில்
		string_list_append(list, remote->name)->util = வெற்று;
	url = தள்ளு_url_of_remote(remote);
	க்கு (size_t i = 0; i < url->nr; i++) {
		strbuf_addf(&remote_info_buf, "%s (push)", url->v[i]);
		string_list_append(list, remote->name)->util =
				strbuf_detach(&remote_info_buf, வெற்று);
	}

	திரும்பு 0;
}

நிலையான முழு show_all(வெறுமை)
{
	கட்டமைப்பு string_list list = STRING_LIST_INIT_DUP;
	முழு result;

	result = for_each_remote(get_one_entry, &list);

	எனில் (!result) {
		string_list_sort(&list);
		க்கு (size_t i = 0; i < list.nr; i++) {
			கட்டமைப்பு string_list_item *item = list.பொருட்கள் + i;
			எனில் (verbose)
				printf("%s\t%s\n", item->string,
					item->util ? (மாறிலி எழுத்து *)item->util : "");
			இல்லையெனில் {
				எனில் (i && !strcmp((item - 1)->string, item->string))
					தொடர்;
				printf("%s\n", item->string);
			}
		}
	}
	string_list_clear(&list, 1);
	திரும்பு result;
}

நிலையான முழு show(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
		கட்டமைப்பு repository *repo UNUSED)
{
	முழு no_query = 0, result = 0, query_flag = 0;
	கட்டமைப்பு option options[] = {
		OPT_BOOL('n', வெற்று, &no_query, N_("do not query remotes")),
		OPT_END()
	};
	கட்டமைப்பு show_info info = SHOW_INFO_INIT;

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_show_usage,
			     0);

	எனில் (argc < 1)
		திரும்பு show_all();

	எனில் (!no_query)
		query_flag = (GET_REF_STATES | GET_HEAD_NAMES | GET_தள்ளு_REF_STATES);

	க்கு (; argc; argc--, argv++) {
		size_t i;
		கட்டமைப்பு strvec *url;

		get_remote_ref_states(*argv, &info.states, query_flag);

		printf_ln(_("* remote %s"), *argv);
		printf_ln(_("  Fetch URL: %s"), info.states.remote->url.v[0]);
		url = தள்ளு_url_of_remote(info.states.remote);
		க்கு (i = 0; i < url->nr; i++)
			/*
			 * TRANSLATORS: the colon ':' should align
			 * with the one in " Fetch URL: %s"
			 * translation.
			 */
			printf_ln(_("  Push  URL: %s"), url->v[i]);
		எனில் (!i)
			printf_ln(_("  Push  URL: %s"), _("(no URL)"));
		எனில் (no_query)
			printf_ln(_("  HEAD branch: %s"), _("(not queried)"));
		இல்லையெனில் எனில் (!info.states.heads.nr)
			printf_ln(_("  HEAD branch: %s"), _("(unknown)"));
		இல்லையெனில் எனில் (info.states.heads.nr == 1)
			printf_ln(_("  HEAD branch: %s"), info.states.heads.பொருட்கள்[0].string);
		இல்லையெனில் {
			printf(_("  HEAD branch (remote HEAD is ambiguous,"
				 " may be one of the following):\n"));
			க்கு (i = 0; i < info.states.heads.nr; i++)
				printf("    %s\n", info.states.heads.பொருட்கள்[i].string);
		}

		/* remote branch info */
		info.width = 0;
		for_each_string_list(&info.states.new_refs, add_remote_to_show_info, &info);
		for_each_string_list(&info.states.skipped, add_remote_to_show_info, &info);
		for_each_string_list(&info.states.tracked, add_remote_to_show_info, &info);
		for_each_string_list(&info.states.stale, add_remote_to_show_info, &info);
		எனில் (info.list.nr)
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
		எனில் (info.list.nr)
			printf_ln(Q_("  Local branch configured for 'git pull':",
				     "  Local branches configured for 'git pull':",
				     info.list.nr));
		for_each_string_list(&info.list, show_local_info_item, &info);
		string_list_clear(&info.list, 0);

		/* git push info */
		எனில் (info.states.remote->mirror)
			printf_ln(_("  Local refs will be mirrored by 'git push'"));

		info.width = info.width2 = 0;
		for_each_string_list(&info.states.தள்ளு, add_தள்ளு_to_show_info, &info);
		QSORT(info.list.பொருட்கள், info.list.nr, cmp_string_with_தள்ளு);
		எனில் (info.list.nr)
			printf_ln(Q_("  Local ref configured for 'git push'%s:",
				     "  Local refs configured for 'git push'%s:",
				     info.list.nr),
				  no_query ? _(" (status not queried)") : "");
		for_each_string_list(&info.list, show_தள்ளு_info_item, &info);
		string_list_clear(&info.list, 0);

		free_remote_ref_states(&info.states);
	}

	திரும்பு result;
}

நிலையான வெறுமை report_set_head_auto(மாறிலி எழுத்து *remote, மாறிலி எழுத்து *head_name,
			கட்டமைப்பு strbuf *b_local_head, முழு was_detached) {
	கட்டமைப்பு strbuf buf_prefix = STRBUF_INIT;
	மாறிலி எழுத்து *prev_head = வெற்று;

	strbuf_addf(&buf_prefix, "refs/remotes/%s/", remote);
	skip_prefix(b_local_head->buf, buf_prefix.buf, &prev_head);

	எனில் (prev_head && !strcmp(prev_head, head_name))
		printf(_("'%s/HEAD' is unchanged and points to '%s'\n"),
			remote, head_name);
	இல்லையெனில் எனில் (prev_head)
		printf(_("'%s/HEAD' has changed from '%s' and now points to '%s'\n"),
			remote, prev_head, head_name);
	இல்லையெனில் எனில் (!b_local_head->len)
		printf(_("'%s/HEAD' is now created and points to '%s'\n"),
			remote, head_name);
	இல்லையெனில் எனில் (was_detached && b_local_head->len)
		printf(_("'%s/HEAD' was detached at '%s' and now points to '%s'\n"),
			remote, b_local_head->buf, head_name);
	இல்லையெனில்
		printf(_("'%s/HEAD' used to point to '%s' "
			"(which is not a remote branch), but now points to '%s'\n"),
			remote, b_local_head->buf, head_name);
	strbuf_release(&buf_prefix);
}

நிலையான முழு set_head(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
		    கட்டமைப்பு repository *repo UNUSED)
{
	முழு opt_a = 0, opt_d = 0, result = 0, was_detached;
	கட்டமைப்பு strbuf b_head = STRBUF_INIT, b_remote_head = STRBUF_INIT,
		b_local_head = STRBUF_INIT;
	எழுத்து *head_name = வெற்று;
	கட்டமைப்பு ref_store *refs = get_main_ref_store(the_repository);
	கட்டமைப்பு remote *remote;

	கட்டமைப்பு option options[] = {
		OPT_BOOL('a', "auto", &opt_a,
			 N_("set refs/remotes/<name>/HEAD according to remote")),
		OPT_BOOL('d', "delete", &opt_d,
			 N_("delete refs/remotes/<name>/HEAD")),
		OPT_END()
	};
	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_sethead_usage, 0);

	/* All modes require at least a remote name. */
	எனில் (!argc)
		usage_with_options(builtin_remote_sethead_usage, options);

	strbuf_addf(&b_head, "refs/remotes/%s/HEAD", argv[0]);
	remote = remote_get(argv[0]);

	எனில் (!opt_a && !opt_d && argc == 2) {
		head_name = xstrdup(argv[1]);
	} இல்லையெனில் எனில் (opt_a && !opt_d && argc == 1) {
		கட்டமைப்பு ref_states states = REF_STATES_INIT;
		get_remote_ref_states(argv[0], &states, GET_HEAD_NAMES);
		எனில் (!states.heads.nr)
			result |= error(_("Cannot determine remote HEAD"));
		இல்லையெனில் எனில் (states.heads.nr > 1) {
			result |= error(_("Multiple remote HEAD branches. "
					  "Please choose one explicitly with:"));
			க்கு (size_t i = 0; i < states.heads.nr; i++)
				fprintf(stderr, "  git remote set-head %s %s\n",
					argv[0], states.heads.பொருட்கள்[i].string);
		} இல்லையெனில்
			head_name = xstrdup(states.heads.பொருட்கள்[0].string);
		free_remote_ref_states(&states);
	} இல்லையெனில் எனில் (opt_d && !opt_a && argc == 1) {
		எனில் (refs_delete_ref(refs, வெற்று, b_head.buf, வெற்று, REF_NO_DEREF))
			result |= error(_("Could not delete %s"), b_head.buf);
	} இல்லையெனில்
		usage_with_options(builtin_remote_sethead_usage, options);

	எனில் (!head_name)
		செல் cleanup;
	strbuf_addf(&b_remote_head, "refs/remotes/%s/%s", argv[0], head_name);
	எனில் (!refs_ref_exists(refs, b_remote_head.buf)) {
		result |= error(_("Not a valid ref: %s"), b_remote_head.buf);
		செல் cleanup;
	}
	was_detached = refs_update_symref_extended(refs, b_head.buf, b_remote_head.buf,
			"remote set-head", &b_local_head, 0);
	எனில் (was_detached == -1) {
		result |= error(_("Could not set up %s"), b_head.buf);
		செல் cleanup;
	}
	எனில் (opt_a)
		report_set_head_auto(argv[0], head_name, &b_local_head, was_detached);
	எனில் (remote->follow_remote_head == FOLLOW_REMOTE_ALWAYS) {
		கட்டமைப்பு strbuf config_name = STRBUF_INIT;
		strbuf_addf(&config_name,
			"remote.%s.followremotehead", remote->name);
		repo_config_set(the_repository, config_name.buf, "warn");
		strbuf_release(&config_name);
	}

cleanup:
	விடுவி(head_name);
	strbuf_release(&b_head);
	strbuf_release(&b_remote_head);
	strbuf_release(&b_local_head);
	திரும்பு result;
}

நிலையான முழு prune_remote(மாறிலி எழுத்து *remote, முழு dry_run)
{
	முழு result = 0;
	கட்டமைப்பு ref_states states = REF_STATES_INIT;
	கட்டமைப்பு string_list refs_to_prune = STRING_LIST_INIT_NODUP;
	கட்டமைப்பு string_list_item *item;

	get_remote_ref_states(remote, &states, GET_REF_STATES);

	எனில் (!states.stale.nr) {
		free_remote_ref_states(&states);
		திரும்பு 0;
	}

	printf_ln(_("Pruning %s"), remote);
	printf_ln(_("URL: %s"), states.remote->url.v[0]);

	for_each_string_list_item(item, &states.stale)
		string_list_append(&refs_to_prune, item->util);
	string_list_sort(&refs_to_prune);

	எனில் (!dry_run)
		result |= refs_delete_refs(get_main_ref_store(the_repository),
					   "remote: prune", &refs_to_prune, 0);

	for_each_string_list_item(item, &states.stale) {
		மாறிலி எழுத்து *refname = item->util;

		எனில் (dry_run)
			printf_ln(_(" * [would prune] %s"),
			       abbrev_ref(refname, "refs/remotes/"));
		இல்லையெனில்
			printf_ln(_(" * [pruned] %s"),
			       abbrev_ref(refname, "refs/remotes/"));
	}

	refs_warn_dangling_symrefs(get_main_ref_store(the_repository),
				   stdout, " ", dry_run, &refs_to_prune);

	string_list_clear(&refs_to_prune, 0);
	free_remote_ref_states(&states);
	திரும்பு result;
}

நிலையான முழு prune(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
		 கட்டமைப்பு repository *repo UNUSED)
{
	முழு dry_run = 0, result = 0;
	கட்டமைப்பு option options[] = {
		OPT__DRY_RUN(&dry_run, N_("dry run")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_prune_usage, 0);

	எனில் (argc < 1)
		usage_with_options(builtin_remote_prune_usage, options);

	க்கு (; argc; argc--, argv++)
		result |= prune_remote(*argv, dry_run);

	திரும்பு result;
}

நிலையான முழு get_remote_default(மாறிலி எழுத்து *சாவி, மாறிலி எழுத்து *மதிப்பு UNUSED,
			      மாறிலி கட்டமைப்பு config_context *ctx UNUSED,
			      வெறுமை *priv)
{
	எனில் (strcmp(சாவி, "remotes.default") == 0) {
		முழு *found = priv;
		*found = 1;
	}
	திரும்பு 0;
}

நிலையான முழு update(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
		  கட்டமைப்பு repository *repo UNUSED)
{
	முழு i, prune = -1;
	கட்டமைப்பு option options[] = {
		OPT_BOOL('p', "prune", &prune,
			 N_("prune remotes after fetching")),
		OPT_END()
	};
	கட்டமைப்பு child_process cmd = CHILD_PROCESS_INIT;
	முழு default_defined = 0;

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_update_usage,
			     PARSE_OPT_KEEP_ARGV0);

	strvec_தள்ளு(&cmd.args, "fetch");

	எனில் (prune != -1)
		strvec_தள்ளு(&cmd.args, prune ? "--prune" : "--no-prune");
	எனில் (verbose)
		strvec_தள்ளு(&cmd.args, "-v");
	strvec_தள்ளு(&cmd.args, "--multiple");
	எனில் (argc < 2)
		strvec_தள்ளு(&cmd.args, "default");
	க்கு (i = 1; i < argc; i++)
		strvec_தள்ளு(&cmd.args, argv[i]);

	எனில் (strcmp(cmd.args.v[cmd.args.nr-1], "default") == 0) {
		repo_config(the_repository, get_remote_default, &default_defined);
		எனில் (!default_defined) {
			strvec_எடு(&cmd.args);
			strvec_தள்ளு(&cmd.args, "--all");
		}
	}

	cmd.git_cmd = 1;
	திரும்பு run_command(&cmd);
}

நிலையான முழு remove_all_fetch_refspecs(மாறிலி எழுத்து *சாவி)
{
	திரும்பு repo_config_set_multivar_gently(the_repository, சாவி, வெற்று, வெற்று,
					       CONFIG_FLAGS_MULTI_REPLACE);
}

நிலையான வெறுமை add_branches(கட்டமைப்பு remote *remote, மாறிலி எழுத்து **branches,
			 மாறிலி எழுத்து *சாவி)
{
	மாறிலி எழுத்து *remotename = remote->name;
	முழு mirror = remote->mirror;
	கட்டமைப்பு strbuf refspec = STRBUF_INIT;

	க்கு (; *branches; branches++)
		add_branch(சாவி, *branches, remotename, mirror, &refspec);

	strbuf_release(&refspec);
}

நிலையான முழு set_remote_branches(மாறிலி எழுத்து *remotename, மாறிலி எழுத்து **branches,
				முழு add_mode)
{
	கட்டமைப்பு strbuf சாவி = STRBUF_INIT;
	கட்டமைப்பு remote *remote;

	strbuf_addf(&சாவி, "remote.%s.fetch", remotename);

	remote = remote_get(remotename);
	எனில் (!remote_is_configured(remote, 1)) {
		error(_("No such remote '%s'"), remotename);
		exit(2);
	}

	எனில் (!add_mode && remove_all_fetch_refspecs(சாவி.buf)) {
		strbuf_release(&சாவி);
		திரும்பு 1;
	}
	add_branches(remote, branches, சாவி.buf);

	strbuf_release(&சாவி);
	திரும்பு 0;
}

நிலையான முழு set_branches(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
			கட்டமைப்பு repository *repo UNUSED)
{
	முழு add_mode = 0;
	கட்டமைப்பு option options[] = {
		OPT_BOOL('\0', "add", &add_mode, N_("add branch")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_setbranches_usage, 0);
	எனில் (argc == 0) {
		error(_("no remote specified"));
		usage_with_options(builtin_remote_setbranches_usage, options);
	}
	argv[argc] = வெற்று;

	திரும்பு set_remote_branches(argv[0], argv + 1, add_mode);
}

நிலையான முழு get_url(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
		   கட்டமைப்பு repository *repo UNUSED)
{
	முழு தள்ளு_mode = 0, all_mode = 0;
	மாறிலி எழுத்து *remotename = வெற்று;
	கட்டமைப்பு remote *remote;
	கட்டமைப்பு strvec *url;
	கட்டமைப்பு option options[] = {
		OPT_BOOL('\0', "push", &தள்ளு_mode,
			 N_("query push URLs rather than fetch URLs")),
		OPT_BOOL('\0', "all", &all_mode,
			 N_("return all URLs")),
		OPT_END()
	};
	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_geturl_usage, 0);

	எனில் (argc != 1)
		usage_with_options(builtin_remote_geturl_usage, options);

	remotename = argv[0];

	remote = remote_get(remotename);
	எனில் (!remote_is_configured(remote, 1)) {
		error(_("No such remote '%s'"), remotename);
		exit(2);
	}

	url = தள்ளு_mode ? தள்ளு_url_of_remote(remote) : &remote->url;

	எனில் (all_mode) {
		க்கு (size_t i = 0; i < url->nr; i++)
			printf_ln("%s", url->v[i]);
	} இல்லையெனில் {
		printf_ln("%s", url->v[0]);
	}

	திரும்பு 0;
}

நிலையான முழு set_url(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
		   கட்டமைப்பு repository *repo UNUSED)
{
	முழு தள்ளு_mode = 0, add_mode = 0, delete_mode = 0;
	முழு matches = 0, negative_matches = 0;
	மாறிலி எழுத்து *remotename = வெற்று;
	மாறிலி எழுத்து *newurl = வெற்று;
	மாறிலி எழுத்து *oldurl = வெற்று;
	கட்டமைப்பு remote *remote;
	regex_t old_regex;
	கட்டமைப்பு strvec *urlset;
	கட்டமைப்பு strbuf name_buf = STRBUF_INIT;
	கட்டமைப்பு option options[] = {
		OPT_BOOL('\0', "push", &தள்ளு_mode,
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

	எனில் (add_mode && delete_mode)
		die(_("--add --delete doesn't make sense"));

	எனில் (argc < 3 || argc > 4 || ((add_mode || delete_mode) && argc != 3))
		usage_with_options(builtin_remote_seturl_usage, options);

	remotename = argv[1];
	newurl = argv[2];
	எனில் (argc > 3)
		oldurl = argv[3];

	எனில் (delete_mode)
		oldurl = newurl;

	remote = remote_get(remotename);
	எனில் (!remote_is_configured(remote, 1)) {
		error(_("No such remote '%s'"), remotename);
		exit(2);
	}

	எனில் (தள்ளு_mode) {
		strbuf_addf(&name_buf, "remote.%s.pushurl", remotename);
		urlset = &remote->pushurl;
	} இல்லையெனில் {
		strbuf_addf(&name_buf, "remote.%s.url", remotename);
		urlset = &remote->url;
	}

	/* Special cases that add new entry. */
	எனில் ((!oldurl && !delete_mode) || add_mode) {
		எனில் (add_mode)
			repo_config_set_multivar(the_repository, name_buf.buf, newurl,
						       "^$", 0);
		இல்லையெனில்
			repo_config_set(the_repository, name_buf.buf, newurl);
		செல் out;
	}

	/* Old URL specified. Demand that one matches. */
	எனில் (regcomp(&old_regex, oldurl, REG_EXTENDED))
		die(_("Invalid old URL pattern: %s"), oldurl);

	க்கு (size_t i = 0; i < urlset->nr; i++)
		எனில் (!regexec(&old_regex, urlset->v[i], 0, வெற்று, 0))
			matches++;
		இல்லையெனில்
			negative_matches++;
	எனில் (!delete_mode && !matches)
		die(_("No such URL found: %s"), oldurl);
	எனில் (delete_mode && !negative_matches && !தள்ளு_mode)
		die(_("Will not delete all non-push URLs"));

	regfree(&old_regex);

	எனில் (!delete_mode)
		repo_config_set_multivar(the_repository, name_buf.buf, newurl, oldurl, 0);
	இல்லையெனில்
		repo_config_set_multivar(the_repository, name_buf.buf, வெற்று, oldurl,
					 CONFIG_FLAGS_MULTI_REPLACE);
out:
	strbuf_release(&name_buf);
	திரும்பு 0;
}

முழு cmd_remote(முழு argc,
	       மாறிலி எழுத்து **argv,
	       மாறிலி எழுத்து *prefix,
	       கட்டமைப்பு repository *repo)
{
	parse_opt_subcommand_fn *fn = வெற்று;
	கட்டமைப்பு option options[] = {
		OPT__VERBOSE(&verbose, N_("be verbose; must be placed before a subcommand")),
		OPT_SUBCOMMAND("add", &fn, add),
		OPT_SUBCOMMAND("rename", &fn, mv),
		OPT_SUBCOMMAND_F("rm", &fn, rm, வெற்று, PARSE_OPT_NOCOMPLETE),
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

	எனில் (fn) {
		திரும்பு !!fn(argc, argv, prefix, repo);
	} இல்லையெனில் {
		எனில் (argc) {
			error(_("unknown subcommand: `%s'"), argv[0]);
			usage_with_options(builtin_remote_usage, options);
		}
		திரும்பு !!show_all();
	}
}
