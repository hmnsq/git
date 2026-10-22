#define USE_THE_REPOSITORY_VARIABLE

#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "advice.h"
#অন্তর্ভাব "config.h"
#অন্তর্ভাব "date.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "ident.h"
#অন্তর্ভাব "parse-options.h"
#অন্তর্ভাব "path.h"
#অন্তর্ভাব "transport.h"
#অন্তর্ভাব "remote.h"
#অন্তর্ভাব "string-list.h"
#অন্তর্ভাব "strbuf.h"
#অন্তর্ভাব "run-command.h"
#অন্তর্ভাব "rebase.h"
#অন্তর্ভাব "refs.h"
#অন্তর্ভাব "refspec.h"
#অন্তর্ভাব "odb.h"
#অন্তর্ভাব "strvec.h"
#অন্তর্ভাব "commit-reach.h"
#অন্তর্ভাব "progress.h"

স্থির ধ্রুব বর্ণ * ধ্রুব builtin_remote_usage[] = {
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
	রিক্ত
};

স্থির ধ্রুব বর্ণ * ধ্রুব builtin_remote_add_usage[] = {
	N_("git remote add [<options>] <name> <url>"),
	রিক্ত
};

স্থির ধ্রুব বর্ণ * ধ্রুব builtin_remote_rename_usage[] = {
	N_("git remote rename [--[no-]progress] <old> <new>"),
	রিক্ত
};

স্থির ধ্রুব বর্ণ * ধ্রুব builtin_remote_rm_usage[] = {
	N_("git remote remove <name>"),
	রিক্ত
};

স্থির ধ্রুব বর্ণ * ধ্রুব builtin_remote_sethead_usage[] = {
	N_("git remote set-head <name> (-a | --auto | -d | --delete | <branch>)"),
	রিক্ত
};

স্থির ধ্রুব বর্ণ * ধ্রুব builtin_remote_setbranches_usage[] = {
	N_("git remote set-branches <name> <branch>..."),
	N_("git remote set-branches --add <name> <branch>..."),
	রিক্ত
};

স্থির ধ্রুব বর্ণ * ধ্রুব builtin_remote_show_usage[] = {
	N_("git remote show [<options>] <name>"),
	রিক্ত
};

স্থির ধ্রুব বর্ণ * ধ্রুব builtin_remote_prune_usage[] = {
	N_("git remote prune [<options>] <name>"),
	রিক্ত
};

স্থির ধ্রুব বর্ণ * ধ্রুব builtin_remote_update_usage[] = {
	N_("git remote update [<options>] [<group> | <remote>]..."),
	রিক্ত
};

স্থির ধ্রুব বর্ণ * ধ্রুব builtin_remote_geturl_usage[] = {
	N_("git remote get-url [--push] [--all] <name>"),
	রিক্ত
};

স্থির ধ্রুব বর্ণ * ধ্রুব builtin_remote_seturl_usage[] = {
	N_("git remote set-url [--push] <name> <newurl> [<oldurl>]"),
	N_("git remote set-url --add <name> <newurl>"),
	N_("git remote set-url --delete <name> <url>"),
	রিক্ত
};

#define GET_REF_STATES (1<<0)
#define GET_HEAD_NAMES (1<<1)
#define GET_ঠেলো_REF_STATES (1<<2)

স্থির পূর্ণাঙ্ক verbose;

স্থির পূর্ণাঙ্ক fetch_remote(ধ্রুব বর্ণ *name)
{
	সংরচনা child_process cmd = CHILD_PROCESS_INIT;

	strvec_ঠেলো(&cmd.args, "fetch");
	যদি (verbose)
		strvec_ঠেলো(&cmd.args, "-v");
	strvec_ঠেলো(&cmd.args, name);
	cmd.git_cmd = 1;
	printf_ln(_("Updating %s"), name);
	যদি (run_command(&cmd))
		প্রত্যাগম error(_("Could not fetch %s"), name);
	প্রত্যাগম 0;
}

গণনা {
	TAGS_UNSET = 0,
	TAGS_DEFAULT = 1,
	TAGS_SET = 2
};

#define MIRROR_NONE 0
#define MIRROR_FETCH 1
#define MIRROR_ঠেলো 2
#define MIRROR_BOTH (MIRROR_FETCH|MIRROR_ঠেলো)

স্থির শূন্য add_branch(ধ্রুব বর্ণ *চাবি, ধ্রুব বর্ণ *branchname,
		       ধ্রুব বর্ণ *remotename, পূর্ণাঙ্ক mirror, সংরচনা strbuf *tmp)
{
	strbuf_reset(tmp);
	strbuf_addch(tmp, '+');
	যদি (mirror)
		strbuf_addf(tmp, "refs/%s:refs/%s",
				branchname, branchname);
	অন্যথা
		strbuf_addf(tmp, "refs/heads/%s:refs/remotes/%s/%s",
				branchname, remotename, branchname);
	repo_config_set_multivar(the_repository, চাবি, tmp->buf, "^$", 0);
}

স্থির ধ্রুব বর্ণ mirror_advice[] =
N_("--mirror is dangerous and deprecated; please\n"
   "\t use --mirror=fetch or --mirror=push instead");

স্থির পূর্ণাঙ্ক parse_mirror_opt(ধ্রুব সংরচনা option *opt, ধ্রুব বর্ণ *arg, পূর্ণাঙ্ক not)
{
	unsigned *mirror = opt->মান;
	যদি (not)
		*mirror = MIRROR_NONE;
	অন্যথা যদি (!arg) {
		warning("%s", _(mirror_advice));
		*mirror = MIRROR_BOTH;
	}
	অন্যথা যদি (!strcmp(arg, "fetch"))
		*mirror = MIRROR_FETCH;
	অন্যথা যদি (!strcmp(arg, "push"))
		*mirror = MIRROR_ঠেলো;
	অন্যথা
		প্রত্যাগম error(_("unknown --mirror argument: %s"), arg);
	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক check_remote_collision(সংরচনা remote *remote, শূন্য *data)
{
	ধ্রুব বর্ণ *name = data;
	ধ্রুব বর্ণ *p;

	যদি (skip_prefix(name, remote->name, &p) && *p == '/')
		die(_("remote name '%s' is a subset of existing remote '%s'"),
		    name, remote->name);
	যদি (skip_prefix(remote->name, name, &p) && *p == '/')
		die(_("remote name '%s' is a superset of existing remote '%s'"),
		    name, remote->name);

	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক add(পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv, ধ্রুব বর্ণ *prefix,
	       সংরচনা repository *repo UNUSED)
{
	পূর্ণাঙ্ক fetch = 0, fetch_tags = TAGS_DEFAULT;
	unsigned mirror = MIRROR_NONE;
	সংরচনা string_list track = STRING_LIST_INIT_NODUP;
	ধ্রুব বর্ণ *master = রিক্ত;
	সংরচনা remote *remote;
	সংরচনা strbuf buf = STRBUF_INIT, buf2 = STRBUF_INIT;
	ধ্রুব বর্ণ *name, *url;
	পূর্ণাঙ্ক result = 0;

	সংরচনা option options[] = {
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

	যদি (argc != 2)
		usage_with_options(builtin_remote_add_usage, options);

	যদি (mirror && master)
		die(_("specifying a master branch makes no sense with --mirror"));
	যদি (mirror && !(mirror & MIRROR_FETCH) && track.nr)
		die(_("specifying branches to track makes sense only with fetch mirrors"));

	name = argv[0];
	url = argv[1];

	remote = remote_get(name);
	যদি (remote_is_configured(remote, 1)) {
		error(_("remote %s already exists."), name);
		exit(3);
	}

	যদি (!valid_remote_name(name))
		die(_("'%s' is not a valid remote name"), name);

	for_each_remote(check_remote_collision, (শূন্য *)name);

	strbuf_addf(&buf, "remote.%s.url", name);
	repo_config_set(the_repository, buf.buf, url);

	যদি (!mirror || mirror & MIRROR_FETCH) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.fetch", name);
		যদি (track.nr == 0)
			string_list_append(&track, "*");
		প্রতি (size_t i = 0; i < track.nr; i++) {
			add_branch(buf.buf, track.বস্তুসমূহ[i].string,
				   name, mirror, &buf2);
		}
	}

	যদি (mirror & MIRROR_ঠেলো) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.mirror", name);
		repo_config_set(the_repository, buf.buf, "true");
	}

	যদি (fetch_tags != TAGS_DEFAULT) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.tagOpt", name);
		repo_config_set(the_repository, buf.buf,
				fetch_tags == TAGS_SET ? "--tags" : "--no-tags");
	}

	যদি (fetch && fetch_remote(name)) {
		result = 1;
		গচ্ছ out;
	}

	যদি (master) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "refs/remotes/%s/HEAD", name);

		strbuf_reset(&buf2);
		strbuf_addf(&buf2, "refs/remotes/%s/%s", name, master);

		যদি (refs_update_symref(get_main_ref_store(the_repository), buf.buf, buf2.buf, "remote add"))
			result = error(_("Could not setup master '%s'"), master);
	}

out:
	strbuf_release(&buf);
	strbuf_release(&buf2);
	string_list_clear(&track, 0);

	প্রত্যাগম result;
}

সংরচনা branch_info {
	বর্ণ *remote_name;
	সংরচনা string_list merge;
	গণনা rebase_type rebase;
	বর্ণ *ঠেলো_remote_name;
};

স্থির সংরচনা string_list branch_list = STRING_LIST_INIT_DUP;

স্থির ধ্রুব বর্ণ *abbrev_ref(ধ্রুব বর্ণ *name, ধ্রুব বর্ণ *prefix)
{
	skip_prefix(name, prefix, &name);
	প্রত্যাগম name;
}
#define abbrev_branch(name) abbrev_ref((name), "refs/heads/")

স্থির পূর্ণাঙ্ক config_read_branches(ধ্রুব বর্ণ *চাবি, ধ্রুব বর্ণ *মান,
				ধ্রুব সংরচনা config_context *ctx UNUSED,
				শূন্য *data UNUSED)
{
	ধ্রুব বর্ণ *orig_চাবি = চাবি;
	বর্ণ *name;
	সংরচনা string_list_item *item;
	সংরচনা branch_info *info;
	গণনা { REMOTE, MERGE, REBASE, ঠেলো_REMOTE } type;
	size_t চাবি_len;

	যদি (!starts_with(চাবি, "branch."))
		প্রত্যাগম 0;

	চাবি += strlen("branch.");
	যদি (strip_suffix(চাবি, ".remote", &চাবি_len))
		type = REMOTE;
	অন্যথা যদি (strip_suffix(চাবি, ".merge", &চাবি_len))
		type = MERGE;
	অন্যথা যদি (strip_suffix(চাবি, ".rebase", &চাবি_len))
		type = REBASE;
	অন্যথা যদি (strip_suffix(চাবি, ".pushremote", &চাবি_len))
		type = ঠেলো_REMOTE;
	অন্যথা
		প্রত্যাগম 0;

	name = xmemdupz(চাবি, চাবি_len);
	item = string_list_ঢোকাও(&branch_list, name);

	যদি (!item->util)
		item->util = xcalloc(1, পরিমাণ(সংরচনা branch_info));
	info = item->util;
	বিকল্প (type) {
	পক্ষ REMOTE:
		যদি (info->remote_name)
			warning(_("more than one %s"), orig_চাবি);
		info->remote_name = xstrdup(মান);
		ভঙ্গ;
	পক্ষ MERGE: {
		ধ্রুব বর্ণ *space = strchr(মান, ' ');
		মান = abbrev_branch(মান);
		যাবত্ (space) {
			বর্ণ *merge;
			merge = xstrndup(মান, space - মান);
			string_list_append(&info->merge, merge);
			মান = abbrev_branch(space + 1);
			space = strchr(মান, ' ');
		}
		string_list_append(&info->merge, xstrdup(মান));
		ভঙ্গ;
	}
	পক্ষ REBASE:
		/*
		 * Consider invalid values as false and check the
		 * truth value with >= REBASE_TRUE.
		 */
		info->rebase = rebase_parse_মান(মান);
		যদি (info->rebase == REBASE_INVALID)
			warning(_("unhandled branch.%s.rebase=%s; assuming "
				  "'true'"), name, মান);
		ভঙ্গ;
	পক্ষ ঠেলো_REMOTE:
		যদি (info->ঠেলো_remote_name)
			warning(_("more than one %s"), orig_চাবি);
		info->ঠেলো_remote_name = xstrdup(মান);
		ভঙ্গ;
	স্বভাব:
		BUG("unexpected type=%d", type);
	}

	মুক্ত(name);
	প্রত্যাগম 0;
}

স্থির শূন্য read_branches(শূন্য)
{
	যদি (branch_list.nr)
		প্রত্যাগম;
	repo_config(the_repository, config_read_branches, রিক্ত);
}

সংরচনা ref_states {
	সংরচনা remote *remote;
	সংরচনা string_list new_refs, skipped, stale, tracked, heads, ঠেলো;
	পূর্ণাঙ্ক queried;
};

#define REF_STATES_INIT { \
	.new_refs = STRING_LIST_INIT_DUP, \
	.skipped = STRING_LIST_INIT_DUP, \
	.stale = STRING_LIST_INIT_DUP, \
	.tracked = STRING_LIST_INIT_DUP, \
	.heads = STRING_LIST_INIT_DUP, \
	.ঠেলো = STRING_LIST_INIT_DUP, \
}

স্থির পূর্ণাঙ্ক get_ref_states(ধ্রুব সংরচনা ref *remote_refs, সংরচনা ref_states *states)
{
	সংরচনা ref *fetch_map = রিক্ত, **tail = &fetch_map;
	সংরচনা ref *ref, *stale_refs;
	পূর্ণাঙ্ক i;

	প্রতি (i = 0; i < states->remote->fetch.nr; i++)
		যদি (get_fetch_map(remote_refs, &states->remote->fetch.বস্তুসমূহ[i], &tail, 1))
			die(_("Could not get fetch map for refspec %s"),
				states->remote->fetch.বস্তুসমূহ[i].raw);

	প্রতি (ref = fetch_map; ref; ref = ref->next) {
		যদি (refname_matches_negative_refspec_item(ref->name, &states->remote->fetch))
			string_list_append(&states->skipped, abbrev_branch(ref->name));
		অন্যথা যদি (!ref->peer_ref || !refs_ref_exists(get_main_ref_store(the_repository), ref->peer_ref->name))
			string_list_append(&states->new_refs, abbrev_branch(ref->name));
		অন্যথা
			string_list_append(&states->tracked, abbrev_branch(ref->name));
	}
	stale_refs = get_stale_heads(&states->remote->fetch, fetch_map);
	প্রতি (ref = stale_refs; ref; ref = ref->next) {
		সংরচনা string_list_item *item =
			string_list_append(&states->stale, abbrev_branch(ref->name));
		item->util = xstrdup(ref->name);
	}
	free_refs(stale_refs);
	free_refs(fetch_map);

	string_list_sort(&states->new_refs);
	string_list_sort(&states->skipped);
	string_list_sort(&states->tracked);
	string_list_sort(&states->stale);

	প্রত্যাগম 0;
}

সংরচনা ঠেলো_info {
	বর্ণ *dest;
	পূর্ণাঙ্ক forced;
	গণনা {
		ঠেলো_STATUS_CREATE = 0,
		ঠেলো_STATUS_DELETE,
		ঠেলো_STATUS_UPTODATE,
		ঠেলো_STATUS_FASTFORWARD,
		ঠেলো_STATUS_OUTOFDATE,
		ঠেলো_STATUS_NOTQUERIED
	} status;
};

স্থির পূর্ণাঙ্ক get_ঠেলো_ref_states(ধ্রুব সংরচনা ref *remote_refs,
	সংরচনা ref_states *states)
{
	সংরচনা remote *remote = states->remote;
	সংরচনা ref *ref, *local_refs, *ঠেলো_map;
	যদি (remote->mirror)
		প্রত্যাগম 0;

	local_refs = get_local_heads();
	ঠেলো_map = copy_ref_list(remote_refs);

	match_ঠেলো_refs(local_refs, &ঠেলো_map, &remote->ঠেলো, MATCH_REFS_NONE);

	প্রতি (ref = ঠেলো_map; ref; ref = ref->next) {
		সংরচনা string_list_item *item;
		সংরচনা ঠেলো_info *info;

		যদি (!ref->peer_ref)
			অনুবর্ত;
		oidcpy(&ref->new_oid, &ref->peer_ref->new_oid);

		item = string_list_append(&states->ঠেলো,
					  abbrev_branch(ref->peer_ref->name));
		item->util = xcalloc(1, পরিমাণ(সংরচনা ঠেলো_info));
		info = item->util;
		info->forced = ref->force;
		info->dest = xstrdup(abbrev_branch(ref->name));

		যদি (is_null_oid(&ref->new_oid)) {
			info->status = ঠেলো_STATUS_DELETE;
		} অন্যথা যদি (oideq(&ref->old_oid, &ref->new_oid))
			info->status = ঠেলো_STATUS_UPTODATE;
		অন্যথা যদি (is_null_oid(&ref->old_oid))
			info->status = ঠেলো_STATUS_CREATE;
		অন্যথা যদি (odb_has_object(the_repository->objects, &ref->old_oid,
					ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR) &&
			 ref_newer(&ref->new_oid, &ref->old_oid))
			info->status = ঠেলো_STATUS_FASTFORWARD;
		অন্যথা
			info->status = ঠেলো_STATUS_OUTOFDATE;
	}
	free_refs(local_refs);
	free_refs(ঠেলো_map);
	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক get_ঠেলো_ref_states_noquery(সংরচনা ref_states *states)
{
	পূর্ণাঙ্ক i;
	সংরচনা remote *remote = states->remote;
	সংরচনা string_list_item *item;
	সংরচনা ঠেলো_info *info;

	যদি (remote->mirror)
		প্রত্যাগম 0;

	যদি (!remote->ঠেলো.nr) {
		item = string_list_append(&states->ঠেলো, _("(matching)"));
		info = item->util = xcalloc(1, পরিমাণ(সংরচনা ঠেলো_info));
		info->status = ঠেলো_STATUS_NOTQUERIED;
		info->dest = xstrdup(item->string);
	}
	প্রতি (i = 0; i < remote->ঠেলো.nr; i++) {
		ধ্রুব সংরচনা refspec_item *spec = &remote->ঠেলো.বস্তুসমূহ[i];
		যদি (spec->matching)
			item = string_list_append(&states->ঠেলো, _("(matching)"));
		অন্যথা যদি (strlen(spec->src))
			item = string_list_append(&states->ঠেলো, spec->src);
		অন্যথা
			item = string_list_append(&states->ঠেলো, _("(delete)"));

		info = item->util = xcalloc(1, পরিমাণ(সংরচনা ঠেলো_info));
		info->forced = spec->force;
		info->status = ঠেলো_STATUS_NOTQUERIED;
		info->dest = xstrdup(spec->dst ? spec->dst : item->string);
	}
	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক get_head_names(ধ্রুব সংরচনা ref *remote_refs, সংরচনা ref_states *states)
{
	সংরচনা ref *ref, *matches;
	সংরচনা ref *fetch_map = রিক্ত, **fetch_map_tail = &fetch_map;
	সংরচনা refspec_item refspec = {
		.force = 0,
		.pattern = 1,
		.src = (বর্ণ *) "refs/heads/*",
		.dst = (বর্ণ *) "refs/heads/*",
	};

	get_fetch_map(remote_refs, &refspec, &fetch_map_tail, 0);
	matches = guess_remote_head(find_ref_by_name(remote_refs, "HEAD"),
				    fetch_map, REMOTE_GUESS_HEAD_ALL);
	প্রতি (ref = matches; ref; ref = ref->next)
		string_list_append(&states->heads, abbrev_branch(ref->name));

	free_refs(fetch_map);
	free_refs(matches);
	প্রত্যাগম 0;
}

সংরচনা known_remote {
	সংরচনা known_remote *next;
	সংরচনা remote *remote;
};

সংরচনা known_remotes {
	সংরচনা remote *to_delete;
	সংরচনা known_remote *list;
};

স্থির পূর্ণাঙ্ক add_known_remote(সংরচনা remote *remote, শূন্য *cb_data)
{
	সংরচনা known_remotes *all = cb_data;
	সংরচনা known_remote *r;

	যদি (!strcmp(all->to_delete->name, remote->name))
		প্রত্যাগম 0;

	r = xmalloc(পরিমাণ(*r));
	r->remote = remote;
	r->next = all->list;
	all->list = r;
	প্রত্যাগম 0;
}

সংরচনা branches_for_remote {
	সংরচনা remote *remote;
	সংরচনা string_list *branches, *skipped;
	সংরচনা known_remotes *keep;
};

স্থির পূর্ণাঙ্ক add_branch_for_removal(ধ্রুব সংরচনা reference *ref, শূন্য *cb_data)
{
	সংরচনা branches_for_remote *branches = cb_data;
	সংরচনা refspec_item refspec;
	সংরচনা known_remote *kr;

	memset(&refspec, 0, পরিমাণ(refspec));
	refspec.dst = (বর্ণ *)ref->name;
	যদি (remote_find_tracking(branches->remote, &refspec))
		প্রত্যাগম 0;
	মুক্ত(refspec.src);

	/* don't delete a branch if another remote also uses it */
	প্রতি (kr = branches->keep->list; kr; kr = kr->next) {
		memset(&refspec, 0, পরিমাণ(refspec));
		refspec.dst = (বর্ণ *)ref->name;
		যদি (!remote_find_tracking(kr->remote, &refspec)) {
			মুক্ত(refspec.src);
			প্রত্যাগম 0;
		}
	}

	/* don't delete non-remote-tracking refs */
	যদি (!starts_with(ref->name, "refs/remotes/")) {
		/* advise user how to delete local branches */
		যদি (starts_with(ref->name, "refs/heads/"))
			string_list_append(branches->skipped,
					   abbrev_branch(ref->name));
		/* silently skip over other non-remote refs */
		প্রত্যাগম 0;
	}

	string_list_append(branches->branches, ref->name);

	প্রত্যাগম 0;
}

সংরচনা rename_info {
	ধ্রুব বর্ণ *old_name;
	ধ্রুব বর্ণ *new_name;
	সংরচনা ref_transaction *transaction;
	সংরচনা progress *progress;
	সংরচনা strbuf *err;
	uint32_t progress_nr;
	uint64_t index;
};

স্থির শূন্য compute_renamed_ref(সংরচনা rename_info *rename,
				ধ্রুব বর্ণ *refname,
				সংরচনা strbuf *out)
{
	strbuf_reset(out);
	strbuf_addstr(out, refname);
	strbuf_splice(out, strlen("refs/remotes/"), strlen(rename->old_name),
		      rename->new_name, strlen(rename->new_name));
}

স্থির পূর্ণাঙ্ক rename_one_reflog_entry(ধ্রুব বর্ণ *old_refname,
				   সংরচনা object_id *old_oid,
				   সংরচনা object_id *new_oid,
				   ধ্রুব বর্ণ *committer,
				   timestamp_t timestamp, পূর্ণাঙ্ক tz,
				   ধ্রুব বর্ণ *বার্তা, শূন্য *cb_data)
{
	সংরচনা rename_info *rename = cb_data;
	সংরচনা strbuf new_refname = STRBUF_INIT;
	সংরচনা strbuf identity = STRBUF_INIT;
	সংরচনা strbuf name = STRBUF_INIT;
	সংরচনা strbuf mail = STRBUF_INIT;
	সংরচনা ident_split ident;
	ধ্রুব বর্ণ *date;
	পূর্ণাঙ্ক error;

	compute_renamed_ref(rename, old_refname, &new_refname);

	যদি (split_ident_সারি(&ident, committer, strlen(committer)) < 0) {
		error = -1;
		গচ্ছ out;
	}

	strbuf_add(&name, ident.name_begin, ident.name_end - ident.name_begin);
	strbuf_add(&mail, ident.mail_begin, ident.mail_end - ident.mail_begin);

	date = show_date(timestamp, tz, DATE_MODE(NORMAL));
	strbuf_addstr(&identity, fmt_ident(name.buf, mail.buf,
					  WANT_BLANK_IDENT, date, 0));

	error = ref_transaction_update_reflog(rename->transaction, new_refname.buf,
					      new_oid, old_oid, identity.buf, বার্তা,
					      rename->index++, rename->err);

out:
	strbuf_release(&new_refname);
	strbuf_release(&identity);
	strbuf_release(&name);
	strbuf_release(&mail);
	প্রত্যাগম error;
}

স্থির পূর্ণাঙ্ক rename_one_reflog(ধ্রুব বর্ণ *old_refname,
			     ধ্রুব সংরচনা object_id *old_oid,
			     সংরচনা rename_info *rename)
{
	সংরচনা strbuf new_refname = STRBUF_INIT;
	সংরচনা strbuf message = STRBUF_INIT;
	পূর্ণাঙ্ক error;

	যদি (!refs_reflog_exists(get_main_ref_store(the_repository), old_refname))
		প্রত্যাগম 0;

	error = refs_for_each_reflog_ent(get_main_ref_store(the_repository),
					 old_refname, rename_one_reflog_entry, rename);
	যদি (error < 0)
		গচ্ছ out;

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
	যদি (error < 0)
		প্রত্যাগম error;

out:
	strbuf_release(&new_refname);
	strbuf_release(&message);
	প্রত্যাগম error;
}

স্থির পূর্ণাঙ্ক rename_one_ref(ধ্রুব সংরচনা reference *ref, শূন্য *cb_data)
{
	সংরচনা strbuf new_referent = STRBUF_INIT;
	সংরচনা strbuf new_refname = STRBUF_INIT;
	সংরচনা rename_info *rename = cb_data;
	ধ্রুব সংরচনা object_id *oid = ref->oid;
	ধ্রুব বর্ণ *referent = ref->target;
	পূর্ণাঙ্ক error;

	compute_renamed_ref(rename, ref->name, &new_refname);

	যদি (ref->flags & REF_ISSYMREF) {
		/*
		 * Stupidly enough `referent` is not pointing to the immediate
		 * target of a symref, but it's the recursively resolved value.
		 * So symrefs pointing to symrefs would be misresolved, and
		 * unborn symrefs don't have any value for the `referent` at all.
		 */
		referent = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
						   ref->name, RESOLVE_REF_NO_RECURSE,
						   রিক্ত, রিক্ত);
		compute_renamed_ref(rename, referent, &new_referent);
		oid = রিক্ত;
	}

	error = ref_transaction_delete(rename->transaction, ref->name,
				       oid, referent, REF_NO_DEREF, রিক্ত, rename->err);
	যদি (error < 0)
		গচ্ছ out;

	error = ref_transaction_update(rename->transaction, new_refname.buf, oid, null_oid(the_hash_algo),
				       (ref->flags & REF_ISSYMREF) ? new_referent.buf : রিক্ত, রিক্ত,
				       REF_SKIP_CREATE_REFLOG | REF_NO_DEREF | REF_SKIP_OID_VERIFICATION,
				       রিক্ত, rename->err);
	যদি (error < 0)
		গচ্ছ out;

	error = rename_one_reflog(ref->name, oid, rename);
	যদি (error < 0)
		গচ্ছ out;

	display_progress(rename->progress, ++rename->progress_nr);

out:
	strbuf_release(&new_referent);
	strbuf_release(&new_refname);
	প্রত্যাগম error;
}

স্থির পূর্ণাঙ্ক migrate_file(সংরচনা remote *remote)
{
	সংরচনা strbuf buf = STRBUF_INIT;

	strbuf_addf(&buf, "remote.%s.url", remote->name);
	প্রতি (size_t i = 0; i < remote->url.nr; i++)
		repo_config_set_multivar(the_repository, buf.buf, remote->url.v[i], "^$", 0);
	strbuf_reset(&buf);
	strbuf_addf(&buf, "remote.%s.push", remote->name);
	প্রতি (পূর্ণাঙ্ক i = 0; i < remote->ঠেলো.nr; i++)
		repo_config_set_multivar(the_repository, buf.buf, remote->ঠেলো.বস্তুসমূহ[i].raw, "^$", 0);
	strbuf_reset(&buf);
	strbuf_addf(&buf, "remote.%s.fetch", remote->name);
	প্রতি (পূর্ণাঙ্ক i = 0; i < remote->fetch.nr; i++)
		repo_config_set_multivar(the_repository, buf.buf, remote->fetch.বস্তুসমূহ[i].raw, "^$", 0);
#ifndef WITH_BREAKING_CHANGES
	যদি (remote->origin == REMOTE_REMOTES)
		unlink_or_warn(repo_git_path_replace(the_repository, &buf,
						     "remotes/%s", remote->name));
	অন্যথা যদি (remote->origin == REMOTE_BRANCHES)
		unlink_or_warn(repo_git_path_replace(the_repository, &buf,
						     "branches/%s", remote->name));
#endif /* WITH_BREAKING_CHANGES */
	strbuf_release(&buf);

	প্রত্যাগম 0;
}

সংরচনা ঠেলো_default_info
{
	ধ্রুব বর্ণ *old_name;
	গণনা config_scope scope;
	সংরচনা strbuf origin;
	পূর্ণাঙ্ক linenr;
};

স্থির পূর্ণাঙ্ক config_read_ঠেলো_default(ধ্রুব বর্ণ *চাবি, ধ্রুব বর্ণ *মান,
	ধ্রুব সংরচনা config_context *ctx, শূন্য *cb)
{
	ধ্রুব সংরচনা চাবি_মান_info *kvi = ctx->kvi;

	সংরচনা ঠেলো_default_info* info = cb;
	যদি (strcmp(চাবি, "remote.pushdefault") ||
	    !মান || strcmp(মান, info->old_name))
		প্রত্যাগম 0;

	info->scope = kvi->scope;
	strbuf_reset(&info->origin);
	strbuf_addstr(&info->origin, config_origin_type_name(kvi->origin_type));
	info->linenr = kvi->linenr;

	প্রত্যাগম 0;
}

স্থির শূন্য handle_ঠেলো_default(ধ্রুব বর্ণ* old_name, ধ্রুব বর্ণ* new_name)
{
	সংরচনা ঠেলো_default_info ঠেলো_default = {
		.old_name = old_name,
		.scope = CONFIG_SCOPE_UNKNOWN,
		.origin = STRBUF_INIT,
		.linenr = -1,
	};
	repo_config(the_repository, config_read_ঠেলো_default, &ঠেলো_default);
	যদি (ঠেলো_default.scope >= CONFIG_SCOPE_COMMAND)
		; /* pass */
	অন্যথা যদি (ঠেলো_default.scope >= CONFIG_SCOPE_LOCAL) {
		পূর্ণাঙ্ক result = repo_config_set_gently(the_repository, "remote.pushDefault",
						    new_name);
		যদি (new_name && result && result != CONFIG_NOTHING_SET)
			die(_("could not set '%s'"), "remote.pushDefault");
		অন্যথা যদি (!new_name && result && result != CONFIG_NOTHING_SET)
			die(_("could not unset '%s'"), "remote.pushDefault");
	} অন্যথা যদি (ঠেলো_default.scope >= CONFIG_SCOPE_SYSTEM) {
		/* warn */
		warning(_("The %s configuration remote.pushDefault in:\n"
			  "\t%s:%d\n"
			  "now names the non-existent remote '%s'"),
			config_scope_name(ঠেলো_default.scope),
			ঠেলো_default.origin.buf, ঠেলো_default.linenr,
			old_name);
	}

	strbuf_release(&ঠেলো_default.origin);
}

স্থির ধ্রুব বর্ণ conflicting_remote_refs_advice[] = N_(
	"The remote you are trying to rename has conflicting references in the\n"
	"new target refspec. This is most likely caused by you trying to nest\n"
	"a remote into itself, e.g. by renaming 'parent' into 'parent/child'\n"
	"or by unnesting a remote, e.g. the other way round.\n"
	"\n"
	"If that is the case, you can address this by first renaming the\n"
	"remote to a different name.\n");

স্থির পূর্ণাঙ্ক mv(পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv, ধ্রুব বর্ণ *prefix,
	      সংরচনা repository *repo UNUSED)
{
	পূর্ণাঙ্ক show_progress = isatty(2);
	সংরচনা option options[] = {
		OPT_BOOL(0, "progress", &show_progress, N_("force progress reporting")),
		OPT_END()
	};
	সংরচনা remote *oldremote, *newremote;
	সংরচনা strbuf buf = STRBUF_INIT, buf2 = STRBUF_INIT, buf3 = STRBUF_INIT,
		old_remote_context = STRBUF_INIT, err = STRBUF_INIT;
	সংরচনা rename_info rename = {
		.err = &err,
	};
	পূর্ণাঙ্ক refspecs_need_update = 0;
	পূর্ণাঙ্ক result = 0;

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_rename_usage, 0);

	যদি (argc != 2)
		usage_with_options(builtin_remote_rename_usage, options);

	rename.old_name = argv[0];
	rename.new_name = argv[1];

	oldremote = remote_get(rename.old_name);
	যদি (!remote_is_configured(oldremote, 1)) {
		error(_("No such remote: '%s'"), rename.old_name);
		exit(2);
	}

	যদি (!strcmp(rename.old_name, rename.new_name) && oldremote->origin != REMOTE_CONFIG)
		প্রত্যাগম migrate_file(oldremote);

	newremote = remote_get(rename.new_name);
	যদি (remote_is_configured(newremote, 1)) {
		error(_("remote %s already exists."), rename.new_name);
		exit(3);
	}

	যদি (!valid_remote_name(rename.new_name))
		die(_("'%s' is not a valid remote name"), rename.new_name);

	strbuf_addf(&buf, "remote.%s", rename.old_name);
	strbuf_addf(&buf2, "remote.%s", rename.new_name);
	যদি (repo_config_rename_section(the_repository, buf.buf, buf2.buf) < 1) {
		result = error(_("Could not rename config section '%s' to '%s'"),
			       buf.buf, buf2.buf);
		গচ্ছ out;
	}

	strbuf_addf(&old_remote_context, ":refs/remotes/%s/", rename.old_name);

	প্রতি (পূর্ণাঙ্ক i = 0; i < oldremote->fetch.nr && !refspecs_need_update; i++)
		refspecs_need_update = !!strstr(oldremote->fetch.বস্তুসমূহ[i].raw,
						old_remote_context.buf);

	যদি (refspecs_need_update) {
		সংরচনা refs_for_each_ref_options opts = {
			.flags = REFS_FOR_EACH_INCLUDE_BROKEN,
		};
		rename.transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
							       0, &err);
		যদি (!rename.transaction)
			গচ্ছ out;

		যদি (show_progress)
			rename.progress = start_delayed_progress(the_repository,
								 _("Renaming remote references"), 0);

		strbuf_reset(&buf);
		strbuf_addf(&buf, "refs/remotes/%s/", rename.old_name);
		opts.prefix = buf.buf;

		result = refs_for_each_ref_ext(get_main_ref_store(the_repository),
					       rename_one_ref, &rename, &opts);
		যদি (result < 0)
			die(_("queueing remote ref renames failed: %s"), rename.err->buf);

		result = ref_transaction_prepare(rename.transaction, &err);
		যদি (result < 0) {
			error("renaming remote references failed: %s", err.buf);
			যদি (result == REF_TRANSACTION_ERROR_NAME_CONFLICT)
				advise(conflicting_remote_refs_advice);
			die(রিক্ত);
		}
	}

	যদি (oldremote->fetch.nr) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.fetch", rename.new_name);
		repo_config_set_multivar(the_repository, buf.buf, রিক্ত, রিক্ত, CONFIG_FLAGS_MULTI_REPLACE);
		প্রতি (পূর্ণাঙ্ক i = 0; i < oldremote->fetch.nr; i++) {
			বর্ণ *ptr;

			strbuf_reset(&buf2);
			strbuf_addstr(&buf2, oldremote->fetch.বস্তুসমূহ[i].raw);
			ptr = strstr(buf2.buf, old_remote_context.buf);
			যদি (ptr) {
				strbuf_splice(&buf2,
					      ptr-buf2.buf + strlen(":refs/remotes/"),
					      strlen(rename.old_name), rename.new_name,
					      strlen(rename.new_name));
			} অন্যথা
				warning(_("Not updating non-default fetch refspec\n"
					  "\t%s\n"
					  "\tPlease update the configuration manually if necessary."),
					buf2.buf);

			repo_config_set_multivar(the_repository, buf.buf, buf2.buf, "^$", 0);
		}
	}

	read_branches();
	প্রতি (size_t i = 0; i < branch_list.nr; i++) {
		সংরচনা string_list_item *item = branch_list.বস্তুসমূহ + i;
		সংরচনা branch_info *info = item->util;
		যদি (info->remote_name && !strcmp(info->remote_name, rename.old_name)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "branch.%s.remote", item->string);
			repo_config_set(the_repository, buf.buf, rename.new_name);
		}
		যদি (info->ঠেলো_remote_name && !strcmp(info->ঠেলো_remote_name, rename.old_name)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "branch.%s.pushRemote", item->string);
			repo_config_set(the_repository, buf.buf, rename.new_name);
		}
	}

	যদি (refspecs_need_update) {
		result = ref_transaction_commit(rename.transaction, &err);
		যদি (result < 0)
			die(_("renaming remote refs failed: %s"), rename.err->buf);

		stop_progress(&rename.progress);

		handle_ঠেলো_default(rename.old_name, rename.new_name);
	}

out:
	ref_transaction_free(rename.transaction);
	strbuf_release(&old_remote_context);
	strbuf_release(&buf);
	strbuf_release(&buf2);
	strbuf_release(&buf3);
	strbuf_release(&err);
	প্রত্যাগম result;
}

স্থির পূর্ণাঙ্ক rm(পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv, ধ্রুব বর্ণ *prefix,
	      সংরচনা repository *repo UNUSED)
{
	সংরচনা option options[] = {
		OPT_END()
	};
	সংরচনা remote *remote;
	সংরচনা strbuf buf = STRBUF_INIT;
	সংরচনা known_remotes known_remotes = { রিক্ত, রিক্ত };
	সংরচনা string_list branches = STRING_LIST_INIT_DUP;
	সংরচনা string_list skipped = STRING_LIST_INIT_DUP;
	সংরচনা branches_for_remote cb_data;
	পূর্ণাঙ্ক result;

	memset(&cb_data, 0, পরিমাণ(cb_data));
	cb_data.branches = &branches;
	cb_data.skipped = &skipped;
	cb_data.keep = &known_remotes;

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_rm_usage, 0);
	যদি (argc != 1)
		usage_with_options(builtin_remote_rm_usage, options);

	remote = remote_get(argv[0]);
	যদি (!remote_is_configured(remote, 1)) {
		error(_("No such remote: '%s'"), argv[0]);
		exit(2);
	}

	known_remotes.to_delete = remote;
	for_each_remote(add_known_remote, &known_remotes);

	read_branches();
	প্রতি (size_t i = 0; i < branch_list.nr; i++) {
		সংরচনা string_list_item *item = branch_list.বস্তুসমূহ + i;
		সংরচনা branch_info *info = item->util;
		যদি (info->remote_name && !strcmp(info->remote_name, remote->name)) {
			ধ্রুব বর্ণ *চাবিগুলি[] = { "remote", "merge", রিক্ত }, **k;
			প্রতি (k = চাবিগুলি; *k; k++) {
				strbuf_reset(&buf);
				strbuf_addf(&buf, "branch.%s.%s",
						item->string, *k);
				result = repo_config_set_gently(the_repository, buf.buf, রিক্ত);
				যদি (result && result != CONFIG_NOTHING_SET)
					die(_("could not unset '%s'"), buf.buf);
			}
		}
		যদি (info->ঠেলো_remote_name && !strcmp(info->ঠেলো_remote_name, remote->name)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "branch.%s.pushremote", item->string);
			result = repo_config_set_gently(the_repository, buf.buf, রিক্ত);
			যদি (result && result != CONFIG_NOTHING_SET)
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

	যদি (!result)
		result = refs_delete_refs(get_main_ref_store(the_repository),
					  "remote: remove", &branches,
					  REF_NO_DEREF);
	string_list_clear(&branches, 0);

	যদি (skipped.nr) {
		fprintf_ln(stderr,
			   Q_("Note: A branch outside the refs/remotes/ hierarchy was not removed;\n"
			      "to delete it, use:",
			      "Note: Some branches outside the refs/remotes/ hierarchy were not removed;\n"
			      "to delete them, use:",
			      skipped.nr));
		প্রতি (size_t i = 0; i < skipped.nr; i++)
			fprintf(stderr, "  git branch -d %s\n",
				skipped.বস্তুসমূহ[i].string);
	}
	string_list_clear(&skipped, 0);

	যদি (!result) {
		strbuf_addf(&buf, "remote.%s", remote->name);
		যদি (repo_config_rename_section(the_repository, buf.buf, রিক্ত) < 1) {
			result = error(_("Could not remove config section '%s'"), buf.buf);
			গচ্ছ out;
		}

		handle_ঠেলো_default(remote->name, রিক্ত);
	}

out:
	প্রতি (সংরচনা known_remote *r = known_remotes.list; r;) {
		সংরচনা known_remote *next = r->next;
		মুক্ত(r);
		r = next;
	}
	strbuf_release(&buf);
	প্রত্যাগম result;
}

স্থির শূন্য clear_ঠেলো_info(শূন্য *util, ধ্রুব বর্ণ *string UNUSED)
{
	সংরচনা ঠেলো_info *info = util;
	মুক্ত(info->dest);
	মুক্ত(info);
}

স্থির শূন্য free_remote_ref_states(সংরচনা ref_states *states)
{
	string_list_clear(&states->new_refs, 0);
	string_list_clear(&states->skipped, 0);
	string_list_clear(&states->stale, 1);
	string_list_clear(&states->tracked, 0);
	string_list_clear(&states->heads, 0);
	string_list_clear_func(&states->ঠেলো, clear_ঠেলো_info);
}

স্থির পূর্ণাঙ্ক append_ref_to_tracked_list(ধ্রুব সংরচনা reference *ref, শূন্য *cb_data)
{
	সংরচনা ref_states *states = cb_data;
	সংরচনা refspec_item refspec;

	যদি (ref->flags & REF_ISSYMREF)
		প্রত্যাগম 0;

	memset(&refspec, 0, পরিমাণ(refspec));
	refspec.dst = (বর্ণ *)ref->name;
	যদি (!remote_find_tracking(states->remote, &refspec)) {
		string_list_append(&states->tracked, abbrev_branch(refspec.src));
		মুক্ত(refspec.src);
	}

	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক get_remote_ref_states(ধ্রুব বর্ণ *name,
				 সংরচনা ref_states *states,
				 পূর্ণাঙ্ক query)
{
	states->remote = remote_get(name);
	যদি (!states->remote)
		প্রত্যাগম error(_("No such remote: '%s'"), name);

	read_branches();

	যদি (query) {
		সংরচনা transport *transport;
		ধ্রুব সংরচনা ref *remote_refs;

		transport = transport_get(states->remote, states->remote->url.v[0]);
		remote_refs = transport_get_remote_refs(transport, রিক্ত);

		states->queried = 1;
		যদি (query & GET_REF_STATES)
			get_ref_states(remote_refs, states);
		যদি (query & GET_HEAD_NAMES)
			get_head_names(remote_refs, states);
		যদি (query & GET_ঠেলো_REF_STATES)
			get_ঠেলো_ref_states(remote_refs, states);
		transport_disconnect(transport);
	} অন্যথা {
		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_ref_to_tracked_list, states);
		string_list_sort(&states->tracked);
		get_ঠেলো_ref_states_noquery(states);
	}

	প্রত্যাগম 0;
}

সংরচনা show_info {
	সংরচনা string_list list;
	সংরচনা ref_states states;
	পূর্ণাঙ্ক width, width2;
	পূর্ণাঙ্ক any_rebase;
};

#define SHOW_INFO_INIT { \
	.list = STRING_LIST_INIT_DUP, \
	.states = REF_STATES_INIT, \
}

স্থির পূর্ণাঙ্ক add_remote_to_show_info(সংরচনা string_list_item *item, শূন্য *cb_data)
{
	সংরচনা show_info *info = cb_data;
	পূর্ণাঙ্ক n = strlen(item->string);
	যদি (n > info->width)
		info->width = n;
	string_list_ঢোকাও(&info->list, item->string);
	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক show_remote_info_item(সংরচনা string_list_item *item, শূন্য *cb_data)
{
	সংরচনা show_info *info = cb_data;
	সংরচনা ref_states *states = &info->states;
	ধ্রুব বর্ণ *name = item->string;

	যদি (states->queried) {
		ধ্রুব বর্ণ *fmt = "%s";
		ধ্রুব বর্ণ *arg = "";
		যদি (string_list_has_string(&states->new_refs, name)) {
			fmt = _(" new (next fetch will store in remotes/%s)");
			arg = states->remote->name;
		} অন্যথা যদি (string_list_has_string(&states->tracked, name))
			arg = _(" tracked");
		অন্যথা যদি (string_list_has_string(&states->skipped, name))
			arg = _(" skipped");
		অন্যথা যদি (string_list_has_string(&states->stale, name))
			arg = _(" stale (use 'git remote prune' to remove)");
		অন্যথা
			arg = _(" ???");
		printf("    %-*s", info->width, name);
		printf(fmt, arg);
		printf("\n");
	} অন্যথা
		printf("    %s\n", name);

	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক add_local_to_show_info(সংরচনা string_list_item *branch_item, শূন্য *cb_data)
{
	সংরচনা show_info *show_info = cb_data;
	সংরচনা ref_states *states = &show_info->states;
	সংরচনা branch_info *branch_info = branch_item->util;
	সংরচনা string_list_item *item;
	পূর্ণাঙ্ক n;

	যদি (!branch_info->merge.nr || !branch_info->remote_name ||
	    strcmp(states->remote->name, branch_info->remote_name))
		প্রত্যাগম 0;
	যদি ((n = strlen(branch_item->string)) > show_info->width)
		show_info->width = n;
	যদি (branch_info->rebase >= REBASE_TRUE)
		show_info->any_rebase = 1;

	item = string_list_ঢোকাও(&show_info->list, branch_item->string);
	item->util = branch_info;

	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক show_local_info_item(সংরচনা string_list_item *item, শূন্য *cb_data)
{
	সংরচনা show_info *show_info = cb_data;
	সংরচনা branch_info *branch_info = item->util;
	সংরচনা string_list *merge = &branch_info->merge;
	পূর্ণাঙ্ক width = show_info->width + 4;

	যদি (branch_info->rebase >= REBASE_TRUE && branch_info->merge.nr > 1) {
		error(_("invalid branch.%s.merge; cannot rebase onto > 1 branch"),
			item->string);
		প্রত্যাগম 0;
	}

	printf("    %-*s ", show_info->width, item->string);
	যদি (branch_info->rebase >= REBASE_TRUE) {
		ধ্রুব বর্ণ *বার্তা;
		যদি (branch_info->rebase == REBASE_INTERACTIVE)
			বার্তা = _("rebases interactively onto remote %s");
		অন্যথা যদি (branch_info->rebase == REBASE_MERGES)
			বার্তা = _("rebases interactively (with merges) onto "
				"remote %s");
		অন্যথা
			বার্তা = _("rebases onto remote %s");
		printf_ln(বার্তা, merge->বস্তুসমূহ[0].string);
		প্রত্যাগম 0;
	} অন্যথা যদি (show_info->any_rebase) {
		printf_ln(_(" merges with remote %s"), merge->বস্তুসমূহ[0].string);
		width++;
	} অন্যথা {
		printf_ln(_("merges with remote %s"), merge->বস্তুসমূহ[0].string);
	}
	প্রতি (size_t i = 1; i < merge->nr; i++)
		printf(_("%-*s    and with remote %s\n"), width, "",
		       merge->বস্তুসমূহ[i].string);

	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক add_ঠেলো_to_show_info(সংরচনা string_list_item *ঠেলো_item, শূন্য *cb_data)
{
	সংরচনা show_info *show_info = cb_data;
	সংরচনা ঠেলো_info *ঠেলো_info = ঠেলো_item->util;
	সংরচনা string_list_item *item;
	পূর্ণাঙ্ক n;
	যদি ((n = strlen(ঠেলো_item->string)) > show_info->width)
		show_info->width = n;
	যদি ((n = strlen(ঠেলো_info->dest)) > show_info->width2)
		show_info->width2 = n;
	item = string_list_append(&show_info->list, ঠেলো_item->string);
	item->util = ঠেলো_item->util;
	প্রত্যাগম 0;
}

/*
 * Sorting comparison for a string list that has push_info
 * structs in its util field
 */
স্থির পূর্ণাঙ্ক cmp_string_with_ঠেলো(ধ্রুব শূন্য *va, ধ্রুব শূন্য *vb)
{
	ধ্রুব সংরচনা string_list_item *a = va;
	ধ্রুব সংরচনা string_list_item *b = vb;
	ধ্রুব সংরচনা ঠেলো_info *a_ঠেলো = a->util;
	ধ্রুব সংরচনা ঠেলো_info *b_ঠেলো = b->util;
	পূর্ণাঙ্ক cmp = strcmp(a->string, b->string);
	প্রত্যাগম cmp ? cmp : strcmp(a_ঠেলো->dest, b_ঠেলো->dest);
}

স্থির পূর্ণাঙ্ক show_ঠেলো_info_item(সংরচনা string_list_item *item, শূন্য *cb_data)
{
	সংরচনা show_info *show_info = cb_data;
	সংরচনা ঠেলো_info *ঠেলো_info = item->util;
	ধ্রুব বর্ণ *src = item->string, *status = রিক্ত;

	বিকল্প (ঠেলো_info->status) {
	পক্ষ ঠেলো_STATUS_CREATE:
		status = _("create");
		ভঙ্গ;
	পক্ষ ঠেলো_STATUS_DELETE:
		status = _("delete");
		src = _("(none)");
		ভঙ্গ;
	পক্ষ ঠেলো_STATUS_UPTODATE:
		status = _("up to date");
		ভঙ্গ;
	পক্ষ ঠেলো_STATUS_FASTFORWARD:
		status = _("fast-forwardable");
		ভঙ্গ;
	পক্ষ ঠেলো_STATUS_OUTOFDATE:
		status = _("local out of date");
		ভঙ্গ;
	পক্ষ ঠেলো_STATUS_NOTQUERIED:
		ভঙ্গ;
	}
	যদি (status) {
		যদি (ঠেলো_info->forced)
			printf_ln(_("    %-*s forces to %-*s (%s)"), show_info->width, src,
			       show_info->width2, ঠেলো_info->dest, status);
		অন্যথা
			printf_ln(_("    %-*s pushes to %-*s (%s)"), show_info->width, src,
			       show_info->width2, ঠেলো_info->dest, status);
	} অন্যথা {
		যদি (ঠেলো_info->forced)
			printf_ln(_("    %-*s forces to %s"), show_info->width, src,
			       ঠেলো_info->dest);
		অন্যথা
			printf_ln(_("    %-*s pushes to %s"), show_info->width, src,
			       ঠেলো_info->dest);
	}
	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক get_one_entry(সংরচনা remote *remote, শূন্য *priv)
{
	সংরচনা string_list *list = priv;
	সংরচনা strbuf remote_info_buf = STRBUF_INIT;
	সংরচনা strvec *url;

	যদি (remote->url.nr > 0) {
		সংরচনা strbuf promisor_config = STRBUF_INIT;
		ধ্রুব বর্ণ *partial_clone_filter = রিক্ত;

		strbuf_addf(&promisor_config, "remote.%s.partialclonefilter", remote->name);
		strbuf_addf(&remote_info_buf, "%s (fetch)", remote->url.v[0]);
		যদি (!repo_config_get_string_tmp(the_repository, promisor_config.buf, &partial_clone_filter))
			strbuf_addf(&remote_info_buf, " [%s]", partial_clone_filter);

		strbuf_release(&promisor_config);
		string_list_append(list, remote->name)->util =
				strbuf_detach(&remote_info_buf, রিক্ত);
	} অন্যথা
		string_list_append(list, remote->name)->util = রিক্ত;
	url = ঠেলো_url_of_remote(remote);
	প্রতি (size_t i = 0; i < url->nr; i++) {
		strbuf_addf(&remote_info_buf, "%s (push)", url->v[i]);
		string_list_append(list, remote->name)->util =
				strbuf_detach(&remote_info_buf, রিক্ত);
	}

	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক show_all(শূন্য)
{
	সংরচনা string_list list = STRING_LIST_INIT_DUP;
	পূর্ণাঙ্ক result;

	result = for_each_remote(get_one_entry, &list);

	যদি (!result) {
		string_list_sort(&list);
		প্রতি (size_t i = 0; i < list.nr; i++) {
			সংরচনা string_list_item *item = list.বস্তুসমূহ + i;
			যদি (verbose)
				printf("%s\t%s\n", item->string,
					item->util ? (ধ্রুব বর্ণ *)item->util : "");
			অন্যথা {
				যদি (i && !strcmp((item - 1)->string, item->string))
					অনুবর্ত;
				printf("%s\n", item->string);
			}
		}
	}
	string_list_clear(&list, 1);
	প্রত্যাগম result;
}

স্থির পূর্ণাঙ্ক show(পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv, ধ্রুব বর্ণ *prefix,
		সংরচনা repository *repo UNUSED)
{
	পূর্ণাঙ্ক no_query = 0, result = 0, query_flag = 0;
	সংরচনা option options[] = {
		OPT_BOOL('n', রিক্ত, &no_query, N_("do not query remotes")),
		OPT_END()
	};
	সংরচনা show_info info = SHOW_INFO_INIT;

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_show_usage,
			     0);

	যদি (argc < 1)
		প্রত্যাগম show_all();

	যদি (!no_query)
		query_flag = (GET_REF_STATES | GET_HEAD_NAMES | GET_ঠেলো_REF_STATES);

	প্রতি (; argc; argc--, argv++) {
		size_t i;
		সংরচনা strvec *url;

		get_remote_ref_states(*argv, &info.states, query_flag);

		printf_ln(_("* remote %s"), *argv);
		printf_ln(_("  Fetch URL: %s"), info.states.remote->url.v[0]);
		url = ঠেলো_url_of_remote(info.states.remote);
		প্রতি (i = 0; i < url->nr; i++)
			/*
			 * TRANSLATORS: the colon ':' should align
			 * with the one in " Fetch URL: %s"
			 * translation.
			 */
			printf_ln(_("  Push  URL: %s"), url->v[i]);
		যদি (!i)
			printf_ln(_("  Push  URL: %s"), _("(no URL)"));
		যদি (no_query)
			printf_ln(_("  HEAD branch: %s"), _("(not queried)"));
		অন্যথা যদি (!info.states.heads.nr)
			printf_ln(_("  HEAD branch: %s"), _("(unknown)"));
		অন্যথা যদি (info.states.heads.nr == 1)
			printf_ln(_("  HEAD branch: %s"), info.states.heads.বস্তুসমূহ[0].string);
		অন্যথা {
			printf(_("  HEAD branch (remote HEAD is ambiguous,"
				 " may be one of the following):\n"));
			প্রতি (i = 0; i < info.states.heads.nr; i++)
				printf("    %s\n", info.states.heads.বস্তুসমূহ[i].string);
		}

		/* remote branch info */
		info.width = 0;
		for_each_string_list(&info.states.new_refs, add_remote_to_show_info, &info);
		for_each_string_list(&info.states.skipped, add_remote_to_show_info, &info);
		for_each_string_list(&info.states.tracked, add_remote_to_show_info, &info);
		for_each_string_list(&info.states.stale, add_remote_to_show_info, &info);
		যদি (info.list.nr)
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
		যদি (info.list.nr)
			printf_ln(Q_("  Local branch configured for 'git pull':",
				     "  Local branches configured for 'git pull':",
				     info.list.nr));
		for_each_string_list(&info.list, show_local_info_item, &info);
		string_list_clear(&info.list, 0);

		/* git push info */
		যদি (info.states.remote->mirror)
			printf_ln(_("  Local refs will be mirrored by 'git push'"));

		info.width = info.width2 = 0;
		for_each_string_list(&info.states.ঠেলো, add_ঠেলো_to_show_info, &info);
		QSORT(info.list.বস্তুসমূহ, info.list.nr, cmp_string_with_ঠেলো);
		যদি (info.list.nr)
			printf_ln(Q_("  Local ref configured for 'git push'%s:",
				     "  Local refs configured for 'git push'%s:",
				     info.list.nr),
				  no_query ? _(" (status not queried)") : "");
		for_each_string_list(&info.list, show_ঠেলো_info_item, &info);
		string_list_clear(&info.list, 0);

		free_remote_ref_states(&info.states);
	}

	প্রত্যাগম result;
}

স্থির শূন্য report_set_head_auto(ধ্রুব বর্ণ *remote, ধ্রুব বর্ণ *head_name,
			সংরচনা strbuf *b_local_head, পূর্ণাঙ্ক was_detached) {
	সংরচনা strbuf buf_prefix = STRBUF_INIT;
	ধ্রুব বর্ণ *prev_head = রিক্ত;

	strbuf_addf(&buf_prefix, "refs/remotes/%s/", remote);
	skip_prefix(b_local_head->buf, buf_prefix.buf, &prev_head);

	যদি (prev_head && !strcmp(prev_head, head_name))
		printf(_("'%s/HEAD' is unchanged and points to '%s'\n"),
			remote, head_name);
	অন্যথা যদি (prev_head)
		printf(_("'%s/HEAD' has changed from '%s' and now points to '%s'\n"),
			remote, prev_head, head_name);
	অন্যথা যদি (!b_local_head->len)
		printf(_("'%s/HEAD' is now created and points to '%s'\n"),
			remote, head_name);
	অন্যথা যদি (was_detached && b_local_head->len)
		printf(_("'%s/HEAD' was detached at '%s' and now points to '%s'\n"),
			remote, b_local_head->buf, head_name);
	অন্যথা
		printf(_("'%s/HEAD' used to point to '%s' "
			"(which is not a remote branch), but now points to '%s'\n"),
			remote, b_local_head->buf, head_name);
	strbuf_release(&buf_prefix);
}

স্থির পূর্ণাঙ্ক set_head(পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv, ধ্রুব বর্ণ *prefix,
		    সংরচনা repository *repo UNUSED)
{
	পূর্ণাঙ্ক opt_a = 0, opt_d = 0, result = 0, was_detached;
	সংরচনা strbuf b_head = STRBUF_INIT, b_remote_head = STRBUF_INIT,
		b_local_head = STRBUF_INIT;
	বর্ণ *head_name = রিক্ত;
	সংরচনা ref_store *refs = get_main_ref_store(the_repository);
	সংরচনা remote *remote;

	সংরচনা option options[] = {
		OPT_BOOL('a', "auto", &opt_a,
			 N_("set refs/remotes/<name>/HEAD according to remote")),
		OPT_BOOL('d', "delete", &opt_d,
			 N_("delete refs/remotes/<name>/HEAD")),
		OPT_END()
	};
	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_sethead_usage, 0);

	/* All modes require at least a remote name. */
	যদি (!argc)
		usage_with_options(builtin_remote_sethead_usage, options);

	strbuf_addf(&b_head, "refs/remotes/%s/HEAD", argv[0]);
	remote = remote_get(argv[0]);

	যদি (!opt_a && !opt_d && argc == 2) {
		head_name = xstrdup(argv[1]);
	} অন্যথা যদি (opt_a && !opt_d && argc == 1) {
		সংরচনা ref_states states = REF_STATES_INIT;
		get_remote_ref_states(argv[0], &states, GET_HEAD_NAMES);
		যদি (!states.heads.nr)
			result |= error(_("Cannot determine remote HEAD"));
		অন্যথা যদি (states.heads.nr > 1) {
			result |= error(_("Multiple remote HEAD branches. "
					  "Please choose one explicitly with:"));
			প্রতি (size_t i = 0; i < states.heads.nr; i++)
				fprintf(stderr, "  git remote set-head %s %s\n",
					argv[0], states.heads.বস্তুসমূহ[i].string);
		} অন্যথা
			head_name = xstrdup(states.heads.বস্তুসমূহ[0].string);
		free_remote_ref_states(&states);
	} অন্যথা যদি (opt_d && !opt_a && argc == 1) {
		যদি (refs_delete_ref(refs, রিক্ত, b_head.buf, রিক্ত, REF_NO_DEREF))
			result |= error(_("Could not delete %s"), b_head.buf);
	} অন্যথা
		usage_with_options(builtin_remote_sethead_usage, options);

	যদি (!head_name)
		গচ্ছ cleanup;
	strbuf_addf(&b_remote_head, "refs/remotes/%s/%s", argv[0], head_name);
	যদি (!refs_ref_exists(refs, b_remote_head.buf)) {
		result |= error(_("Not a valid ref: %s"), b_remote_head.buf);
		গচ্ছ cleanup;
	}
	was_detached = refs_update_symref_extended(refs, b_head.buf, b_remote_head.buf,
			"remote set-head", &b_local_head, 0);
	যদি (was_detached == -1) {
		result |= error(_("Could not set up %s"), b_head.buf);
		গচ্ছ cleanup;
	}
	যদি (opt_a)
		report_set_head_auto(argv[0], head_name, &b_local_head, was_detached);
	যদি (remote->follow_remote_head == FOLLOW_REMOTE_ALWAYS) {
		সংরচনা strbuf config_name = STRBUF_INIT;
		strbuf_addf(&config_name,
			"remote.%s.followremotehead", remote->name);
		repo_config_set(the_repository, config_name.buf, "warn");
		strbuf_release(&config_name);
	}

cleanup:
	মুক্ত(head_name);
	strbuf_release(&b_head);
	strbuf_release(&b_remote_head);
	strbuf_release(&b_local_head);
	প্রত্যাগম result;
}

স্থির পূর্ণাঙ্ক prune_remote(ধ্রুব বর্ণ *remote, পূর্ণাঙ্ক dry_run)
{
	পূর্ণাঙ্ক result = 0;
	সংরচনা ref_states states = REF_STATES_INIT;
	সংরচনা string_list refs_to_prune = STRING_LIST_INIT_NODUP;
	সংরচনা string_list_item *item;

	get_remote_ref_states(remote, &states, GET_REF_STATES);

	যদি (!states.stale.nr) {
		free_remote_ref_states(&states);
		প্রত্যাগম 0;
	}

	printf_ln(_("Pruning %s"), remote);
	printf_ln(_("URL: %s"), states.remote->url.v[0]);

	for_each_string_list_item(item, &states.stale)
		string_list_append(&refs_to_prune, item->util);
	string_list_sort(&refs_to_prune);

	যদি (!dry_run)
		result |= refs_delete_refs(get_main_ref_store(the_repository),
					   "remote: prune", &refs_to_prune, 0);

	for_each_string_list_item(item, &states.stale) {
		ধ্রুব বর্ণ *refname = item->util;

		যদি (dry_run)
			printf_ln(_(" * [would prune] %s"),
			       abbrev_ref(refname, "refs/remotes/"));
		অন্যথা
			printf_ln(_(" * [pruned] %s"),
			       abbrev_ref(refname, "refs/remotes/"));
	}

	refs_warn_dangling_symrefs(get_main_ref_store(the_repository),
				   stdout, " ", dry_run, &refs_to_prune);

	string_list_clear(&refs_to_prune, 0);
	free_remote_ref_states(&states);
	প্রত্যাগম result;
}

স্থির পূর্ণাঙ্ক prune(পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv, ধ্রুব বর্ণ *prefix,
		 সংরচনা repository *repo UNUSED)
{
	পূর্ণাঙ্ক dry_run = 0, result = 0;
	সংরচনা option options[] = {
		OPT__DRY_RUN(&dry_run, N_("dry run")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_prune_usage, 0);

	যদি (argc < 1)
		usage_with_options(builtin_remote_prune_usage, options);

	প্রতি (; argc; argc--, argv++)
		result |= prune_remote(*argv, dry_run);

	প্রত্যাগম result;
}

স্থির পূর্ণাঙ্ক get_remote_default(ধ্রুব বর্ণ *চাবি, ধ্রুব বর্ণ *মান UNUSED,
			      ধ্রুব সংরচনা config_context *ctx UNUSED,
			      শূন্য *priv)
{
	যদি (strcmp(চাবি, "remotes.default") == 0) {
		পূর্ণাঙ্ক *found = priv;
		*found = 1;
	}
	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক update(পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv, ধ্রুব বর্ণ *prefix,
		  সংরচনা repository *repo UNUSED)
{
	পূর্ণাঙ্ক i, prune = -1;
	সংরচনা option options[] = {
		OPT_BOOL('p', "prune", &prune,
			 N_("prune remotes after fetching")),
		OPT_END()
	};
	সংরচনা child_process cmd = CHILD_PROCESS_INIT;
	পূর্ণাঙ্ক default_defined = 0;

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_update_usage,
			     PARSE_OPT_KEEP_ARGV0);

	strvec_ঠেলো(&cmd.args, "fetch");

	যদি (prune != -1)
		strvec_ঠেলো(&cmd.args, prune ? "--prune" : "--no-prune");
	যদি (verbose)
		strvec_ঠেলো(&cmd.args, "-v");
	strvec_ঠেলো(&cmd.args, "--multiple");
	যদি (argc < 2)
		strvec_ঠেলো(&cmd.args, "default");
	প্রতি (i = 1; i < argc; i++)
		strvec_ঠেলো(&cmd.args, argv[i]);

	যদি (strcmp(cmd.args.v[cmd.args.nr-1], "default") == 0) {
		repo_config(the_repository, get_remote_default, &default_defined);
		যদি (!default_defined) {
			strvec_বের_করো(&cmd.args);
			strvec_ঠেলো(&cmd.args, "--all");
		}
	}

	cmd.git_cmd = 1;
	প্রত্যাগম run_command(&cmd);
}

স্থির পূর্ণাঙ্ক remove_all_fetch_refspecs(ধ্রুব বর্ণ *চাবি)
{
	প্রত্যাগম repo_config_set_multivar_gently(the_repository, চাবি, রিক্ত, রিক্ত,
					       CONFIG_FLAGS_MULTI_REPLACE);
}

স্থির শূন্য add_branches(সংরচনা remote *remote, ধ্রুব বর্ণ **branches,
			 ধ্রুব বর্ণ *চাবি)
{
	ধ্রুব বর্ণ *remotename = remote->name;
	পূর্ণাঙ্ক mirror = remote->mirror;
	সংরচনা strbuf refspec = STRBUF_INIT;

	প্রতি (; *branches; branches++)
		add_branch(চাবি, *branches, remotename, mirror, &refspec);

	strbuf_release(&refspec);
}

স্থির পূর্ণাঙ্ক set_remote_branches(ধ্রুব বর্ণ *remotename, ধ্রুব বর্ণ **branches,
				পূর্ণাঙ্ক add_mode)
{
	সংরচনা strbuf চাবি = STRBUF_INIT;
	সংরচনা remote *remote;

	strbuf_addf(&চাবি, "remote.%s.fetch", remotename);

	remote = remote_get(remotename);
	যদি (!remote_is_configured(remote, 1)) {
		error(_("No such remote '%s'"), remotename);
		exit(2);
	}

	যদি (!add_mode && remove_all_fetch_refspecs(চাবি.buf)) {
		strbuf_release(&চাবি);
		প্রত্যাগম 1;
	}
	add_branches(remote, branches, চাবি.buf);

	strbuf_release(&চাবি);
	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক set_branches(পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv, ধ্রুব বর্ণ *prefix,
			সংরচনা repository *repo UNUSED)
{
	পূর্ণাঙ্ক add_mode = 0;
	সংরচনা option options[] = {
		OPT_BOOL('\0', "add", &add_mode, N_("add branch")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_setbranches_usage, 0);
	যদি (argc == 0) {
		error(_("no remote specified"));
		usage_with_options(builtin_remote_setbranches_usage, options);
	}
	argv[argc] = রিক্ত;

	প্রত্যাগম set_remote_branches(argv[0], argv + 1, add_mode);
}

স্থির পূর্ণাঙ্ক get_url(পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv, ধ্রুব বর্ণ *prefix,
		   সংরচনা repository *repo UNUSED)
{
	পূর্ণাঙ্ক ঠেলো_mode = 0, all_mode = 0;
	ধ্রুব বর্ণ *remotename = রিক্ত;
	সংরচনা remote *remote;
	সংরচনা strvec *url;
	সংরচনা option options[] = {
		OPT_BOOL('\0', "push", &ঠেলো_mode,
			 N_("query push URLs rather than fetch URLs")),
		OPT_BOOL('\0', "all", &all_mode,
			 N_("return all URLs")),
		OPT_END()
	};
	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_geturl_usage, 0);

	যদি (argc != 1)
		usage_with_options(builtin_remote_geturl_usage, options);

	remotename = argv[0];

	remote = remote_get(remotename);
	যদি (!remote_is_configured(remote, 1)) {
		error(_("No such remote '%s'"), remotename);
		exit(2);
	}

	url = ঠেলো_mode ? ঠেলো_url_of_remote(remote) : &remote->url;

	যদি (all_mode) {
		প্রতি (size_t i = 0; i < url->nr; i++)
			printf_ln("%s", url->v[i]);
	} অন্যথা {
		printf_ln("%s", url->v[0]);
	}

	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক set_url(পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv, ধ্রুব বর্ণ *prefix,
		   সংরচনা repository *repo UNUSED)
{
	পূর্ণাঙ্ক ঠেলো_mode = 0, add_mode = 0, delete_mode = 0;
	পূর্ণাঙ্ক matches = 0, negative_matches = 0;
	ধ্রুব বর্ণ *remotename = রিক্ত;
	ধ্রুব বর্ণ *newurl = রিক্ত;
	ধ্রুব বর্ণ *oldurl = রিক্ত;
	সংরচনা remote *remote;
	regex_t old_regex;
	সংরচনা strvec *urlset;
	সংরচনা strbuf name_buf = STRBUF_INIT;
	সংরচনা option options[] = {
		OPT_BOOL('\0', "push", &ঠেলো_mode,
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

	যদি (add_mode && delete_mode)
		die(_("--add --delete doesn't make sense"));

	যদি (argc < 3 || argc > 4 || ((add_mode || delete_mode) && argc != 3))
		usage_with_options(builtin_remote_seturl_usage, options);

	remotename = argv[1];
	newurl = argv[2];
	যদি (argc > 3)
		oldurl = argv[3];

	যদি (delete_mode)
		oldurl = newurl;

	remote = remote_get(remotename);
	যদি (!remote_is_configured(remote, 1)) {
		error(_("No such remote '%s'"), remotename);
		exit(2);
	}

	যদি (ঠেলো_mode) {
		strbuf_addf(&name_buf, "remote.%s.pushurl", remotename);
		urlset = &remote->pushurl;
	} অন্যথা {
		strbuf_addf(&name_buf, "remote.%s.url", remotename);
		urlset = &remote->url;
	}

	/* Special cases that add new entry. */
	যদি ((!oldurl && !delete_mode) || add_mode) {
		যদি (add_mode)
			repo_config_set_multivar(the_repository, name_buf.buf, newurl,
						       "^$", 0);
		অন্যথা
			repo_config_set(the_repository, name_buf.buf, newurl);
		গচ্ছ out;
	}

	/* Old URL specified. Demand that one matches. */
	যদি (regcomp(&old_regex, oldurl, REG_EXTENDED))
		die(_("Invalid old URL pattern: %s"), oldurl);

	প্রতি (size_t i = 0; i < urlset->nr; i++)
		যদি (!regexec(&old_regex, urlset->v[i], 0, রিক্ত, 0))
			matches++;
		অন্যথা
			negative_matches++;
	যদি (!delete_mode && !matches)
		die(_("No such URL found: %s"), oldurl);
	যদি (delete_mode && !negative_matches && !ঠেলো_mode)
		die(_("Will not delete all non-push URLs"));

	regfree(&old_regex);

	যদি (!delete_mode)
		repo_config_set_multivar(the_repository, name_buf.buf, newurl, oldurl, 0);
	অন্যথা
		repo_config_set_multivar(the_repository, name_buf.buf, রিক্ত, oldurl,
					 CONFIG_FLAGS_MULTI_REPLACE);
out:
	strbuf_release(&name_buf);
	প্রত্যাগম 0;
}

পূর্ণাঙ্ক cmd_remote(পূর্ণাঙ্ক argc,
	       ধ্রুব বর্ণ **argv,
	       ধ্রুব বর্ণ *prefix,
	       সংরচনা repository *repo)
{
	parse_opt_subcommand_fn *fn = রিক্ত;
	সংরচনা option options[] = {
		OPT__VERBOSE(&verbose, N_("be verbose; must be placed before a subcommand")),
		OPT_SUBCOMMAND("add", &fn, add),
		OPT_SUBCOMMAND("rename", &fn, mv),
		OPT_SUBCOMMAND_F("rm", &fn, rm, রিক্ত, PARSE_OPT_NOCOMPLETE),
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

	যদি (fn) {
		প্রত্যাগম !!fn(argc, argv, prefix, repo);
	} অন্যথা {
		যদি (argc) {
			error(_("unknown subcommand: `%s'"), argv[0]);
			usage_with_options(builtin_remote_usage, options);
		}
		প্রত্যাগম !!show_all();
	}
}
