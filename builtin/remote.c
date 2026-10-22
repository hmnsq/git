#define USE_THE_REPOSITORY_VARIABLE

#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "advice.h"
#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ "date.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "ident.h"
#અન્તર્ભાવ "parse-options.h"
#અન્તર્ભાવ "path.h"
#અન્તર્ભાવ "transport.h"
#અન્તર્ભાવ "remote.h"
#અન્તર્ભાવ "string-list.h"
#અન્તર્ભાવ "strbuf.h"
#અન્તર્ભાવ "run-command.h"
#અન્તર્ભાવ "rebase.h"
#અન્તર્ભાવ "refs.h"
#અન્તર્ભાવ "refspec.h"
#અન્તર્ભાવ "odb.h"
#અન્તર્ભાવ "strvec.h"
#અન્તર્ભાવ "commit-reach.h"
#અન્તર્ભાવ "progress.h"

સ્થિર અચલ અક્ષર * અચલ builtin_remote_usage[] = {
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
	ખાલી
};

સ્થિર અચલ અક્ષર * અચલ builtin_remote_add_usage[] = {
	N_("git remote add [<options>] <name> <url>"),
	ખાલી
};

સ્થિર અચલ અક્ષર * અચલ builtin_remote_rename_usage[] = {
	N_("git remote rename [--[no-]progress] <old> <new>"),
	ખાલી
};

સ્થિર અચલ અક્ષર * અચલ builtin_remote_rm_usage[] = {
	N_("git remote remove <name>"),
	ખાલી
};

સ્થિર અચલ અક્ષર * અચલ builtin_remote_sethead_usage[] = {
	N_("git remote set-head <name> (-a | --auto | -d | --delete | <branch>)"),
	ખાલી
};

સ્થિર અચલ અક્ષર * અચલ builtin_remote_setbranches_usage[] = {
	N_("git remote set-branches <name> <branch>..."),
	N_("git remote set-branches --add <name> <branch>..."),
	ખાલી
};

સ્થિર અચલ અક્ષર * અચલ builtin_remote_show_usage[] = {
	N_("git remote show [<options>] <name>"),
	ખાલી
};

સ્થિર અચલ અક્ષર * અચલ builtin_remote_prune_usage[] = {
	N_("git remote prune [<options>] <name>"),
	ખાલી
};

સ્થિર અચલ અક્ષર * અચલ builtin_remote_update_usage[] = {
	N_("git remote update [<options>] [<group> | <remote>]..."),
	ખાલી
};

સ્થિર અચલ અક્ષર * અચલ builtin_remote_geturl_usage[] = {
	N_("git remote get-url [--push] [--all] <name>"),
	ખાલી
};

સ્થિર અચલ અક્ષર * અચલ builtin_remote_seturl_usage[] = {
	N_("git remote set-url [--push] <name> <newurl> [<oldurl>]"),
	N_("git remote set-url --add <name> <newurl>"),
	N_("git remote set-url --delete <name> <url>"),
	ખાલી
};

#define GET_REF_STATES (1<<0)
#define GET_HEAD_NAMES (1<<1)
#define GET_ધકેલો_REF_STATES (1<<2)

સ્થિર પૂર્ણાંક verbose;

સ્થિર પૂર્ણાંક fetch_remote(અચલ અક્ષર *name)
{
	સંરચના child_process cmd = CHILD_PROCESS_INIT;

	strvec_ધકેલો(&cmd.args, "fetch");
	જો (verbose)
		strvec_ધકેલો(&cmd.args, "-v");
	strvec_ધકેલો(&cmd.args, name);
	cmd.git_cmd = 1;
	printf_ln(_("Updating %s"), name);
	જો (run_command(&cmd))
		પાછા error(_("Could not fetch %s"), name);
	પાછા 0;
}

ગણના {
	TAGS_UNSET = 0,
	TAGS_DEFAULT = 1,
	TAGS_SET = 2
};

#define MIRROR_NONE 0
#define MIRROR_FETCH 1
#define MIRROR_ધકેલો 2
#define MIRROR_BOTH (MIRROR_FETCH|MIRROR_ધકેલો)

સ્થિર શૂન્ય add_branch(અચલ અક્ષર *ચાવી, અચલ અક્ષર *branchname,
		       અચલ અક્ષર *remotename, પૂર્ણાંક mirror, સંરચના strbuf *tmp)
{
	strbuf_reset(tmp);
	strbuf_addch(tmp, '+');
	જો (mirror)
		strbuf_addf(tmp, "refs/%s:refs/%s",
				branchname, branchname);
	નહીંતર
		strbuf_addf(tmp, "refs/heads/%s:refs/remotes/%s/%s",
				branchname, remotename, branchname);
	repo_config_set_multivar(the_repository, ચાવી, tmp->buf, "^$", 0);
}

સ્થિર અચલ અક્ષર mirror_advice[] =
N_("--mirror is dangerous and deprecated; please\n"
   "\t use --mirror=fetch or --mirror=push instead");

સ્થિર પૂર્ણાંક parse_mirror_opt(અચલ સંરચના option *opt, અચલ અક્ષર *arg, પૂર્ણાંક not)
{
	unsigned *mirror = opt->કિંમત;
	જો (not)
		*mirror = MIRROR_NONE;
	નહીંતર જો (!arg) {
		warning("%s", _(mirror_advice));
		*mirror = MIRROR_BOTH;
	}
	નહીંતર જો (!strcmp(arg, "fetch"))
		*mirror = MIRROR_FETCH;
	નહીંતર જો (!strcmp(arg, "push"))
		*mirror = MIRROR_ધકેલો;
	નહીંતર
		પાછા error(_("unknown --mirror argument: %s"), arg);
	પાછા 0;
}

સ્થિર પૂર્ણાંક check_remote_collision(સંરચના remote *remote, શૂન્ય *data)
{
	અચલ અક્ષર *name = data;
	અચલ અક્ષર *p;

	જો (skip_prefix(name, remote->name, &p) && *p == '/')
		die(_("remote name '%s' is a subset of existing remote '%s'"),
		    name, remote->name);
	જો (skip_prefix(remote->name, name, &p) && *p == '/')
		die(_("remote name '%s' is a superset of existing remote '%s'"),
		    name, remote->name);

	પાછા 0;
}

સ્થિર પૂર્ણાંક add(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
	       સંરચના repository *repo UNUSED)
{
	પૂર્ણાંક fetch = 0, fetch_tags = TAGS_DEFAULT;
	unsigned mirror = MIRROR_NONE;
	સંરચના string_list track = STRING_LIST_INIT_NODUP;
	અચલ અક્ષર *master = ખાલી;
	સંરચના remote *remote;
	સંરચના strbuf buf = STRBUF_INIT, buf2 = STRBUF_INIT;
	અચલ અક્ષર *name, *url;
	પૂર્ણાંક result = 0;

	સંરચના option options[] = {
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

	જો (argc != 2)
		usage_with_options(builtin_remote_add_usage, options);

	જો (mirror && master)
		die(_("specifying a master branch makes no sense with --mirror"));
	જો (mirror && !(mirror & MIRROR_FETCH) && track.nr)
		die(_("specifying branches to track makes sense only with fetch mirrors"));

	name = argv[0];
	url = argv[1];

	remote = remote_get(name);
	જો (remote_is_configured(remote, 1)) {
		error(_("remote %s already exists."), name);
		exit(3);
	}

	જો (!valid_remote_name(name))
		die(_("'%s' is not a valid remote name"), name);

	for_each_remote(check_remote_collision, (શૂન્ય *)name);

	strbuf_addf(&buf, "remote.%s.url", name);
	repo_config_set(the_repository, buf.buf, url);

	જો (!mirror || mirror & MIRROR_FETCH) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.fetch", name);
		જો (track.nr == 0)
			string_list_append(&track, "*");
		માટે (size_t i = 0; i < track.nr; i++) {
			add_branch(buf.buf, track.વસ્તુઓ[i].string,
				   name, mirror, &buf2);
		}
	}

	જો (mirror & MIRROR_ધકેલો) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.mirror", name);
		repo_config_set(the_repository, buf.buf, "true");
	}

	જો (fetch_tags != TAGS_DEFAULT) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.tagOpt", name);
		repo_config_set(the_repository, buf.buf,
				fetch_tags == TAGS_SET ? "--tags" : "--no-tags");
	}

	જો (fetch && fetch_remote(name)) {
		result = 1;
		જાઓ out;
	}

	જો (master) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "refs/remotes/%s/HEAD", name);

		strbuf_reset(&buf2);
		strbuf_addf(&buf2, "refs/remotes/%s/%s", name, master);

		જો (refs_update_symref(get_main_ref_store(the_repository), buf.buf, buf2.buf, "remote add"))
			result = error(_("Could not setup master '%s'"), master);
	}

out:
	strbuf_release(&buf);
	strbuf_release(&buf2);
	string_list_clear(&track, 0);

	પાછા result;
}

સંરચના branch_info {
	અક્ષર *remote_name;
	સંરચના string_list merge;
	ગણના rebase_type rebase;
	અક્ષર *ધકેલો_remote_name;
};

સ્થિર સંરચના string_list branch_list = STRING_LIST_INIT_DUP;

સ્થિર અચલ અક્ષર *abbrev_ref(અચલ અક્ષર *name, અચલ અક્ષર *prefix)
{
	skip_prefix(name, prefix, &name);
	પાછા name;
}
#define abbrev_branch(name) abbrev_ref((name), "refs/heads/")

સ્થિર પૂર્ણાંક config_read_branches(અચલ અક્ષર *ચાવી, અચલ અક્ષર *કિંમત,
				અચલ સંરચના config_context *ctx UNUSED,
				શૂન્ય *data UNUSED)
{
	અચલ અક્ષર *orig_ચાવી = ચાવી;
	અક્ષર *name;
	સંરચના string_list_item *item;
	સંરચના branch_info *info;
	ગણના { REMOTE, MERGE, REBASE, ધકેલો_REMOTE } type;
	size_t ચાવી_len;

	જો (!starts_with(ચાવી, "branch."))
		પાછા 0;

	ચાવી += strlen("branch.");
	જો (strip_suffix(ચાવી, ".remote", &ચાવી_len))
		type = REMOTE;
	નહીંતર જો (strip_suffix(ચાવી, ".merge", &ચાવી_len))
		type = MERGE;
	નહીંતર જો (strip_suffix(ચાવી, ".rebase", &ચાવી_len))
		type = REBASE;
	નહીંતર જો (strip_suffix(ચાવી, ".pushremote", &ચાવી_len))
		type = ધકેલો_REMOTE;
	નહીંતર
		પાછા 0;

	name = xmemdupz(ચાવી, ચાવી_len);
	item = string_list_ઉમેરો(&branch_list, name);

	જો (!item->util)
		item->util = xcalloc(1, કદ(સંરચના branch_info));
	info = item->util;
	પસંદગી (type) {
	સ્થિતિ REMOTE:
		જો (info->remote_name)
			warning(_("more than one %s"), orig_ચાવી);
		info->remote_name = xstrdup(કિંમત);
		તોડો;
	સ્થિતિ MERGE: {
		અચલ અક્ષર *space = strchr(કિંમત, ' ');
		કિંમત = abbrev_branch(કિંમત);
		જ્યારે (space) {
			અક્ષર *merge;
			merge = xstrndup(કિંમત, space - કિંમત);
			string_list_append(&info->merge, merge);
			કિંમત = abbrev_branch(space + 1);
			space = strchr(કિંમત, ' ');
		}
		string_list_append(&info->merge, xstrdup(કિંમત));
		તોડો;
	}
	સ્થિતિ REBASE:
		/*
		 * Consider invalid values as false and check the
		 * truth value with >= REBASE_TRUE.
		 */
		info->rebase = rebase_parse_કિંમત(કિંમત);
		જો (info->rebase == REBASE_INVALID)
			warning(_("unhandled branch.%s.rebase=%s; assuming "
				  "'true'"), name, કિંમત);
		તોડો;
	સ્થિતિ ધકેલો_REMOTE:
		જો (info->ધકેલો_remote_name)
			warning(_("more than one %s"), orig_ચાવી);
		info->ધકેલો_remote_name = xstrdup(કિંમત);
		તોડો;
	મૂળભૂત:
		BUG("unexpected type=%d", type);
	}

	મુક્ત(name);
	પાછા 0;
}

સ્થિર શૂન્ય read_branches(શૂન્ય)
{
	જો (branch_list.nr)
		પાછા;
	repo_config(the_repository, config_read_branches, ખાલી);
}

સંરચના ref_states {
	સંરચના remote *remote;
	સંરચના string_list new_refs, skipped, stale, tracked, heads, ધકેલો;
	પૂર્ણાંક queried;
};

#define REF_STATES_INIT { \
	.new_refs = STRING_LIST_INIT_DUP, \
	.skipped = STRING_LIST_INIT_DUP, \
	.stale = STRING_LIST_INIT_DUP, \
	.tracked = STRING_LIST_INIT_DUP, \
	.heads = STRING_LIST_INIT_DUP, \
	.ધકેલો = STRING_LIST_INIT_DUP, \
}

સ્થિર પૂર્ણાંક get_ref_states(અચલ સંરચના ref *remote_refs, સંરચના ref_states *states)
{
	સંરચના ref *fetch_map = ખાલી, **tail = &fetch_map;
	સંરચના ref *ref, *stale_refs;
	પૂર્ણાંક i;

	માટે (i = 0; i < states->remote->fetch.nr; i++)
		જો (get_fetch_map(remote_refs, &states->remote->fetch.વસ્તુઓ[i], &tail, 1))
			die(_("Could not get fetch map for refspec %s"),
				states->remote->fetch.વસ્તુઓ[i].raw);

	માટે (ref = fetch_map; ref; ref = ref->next) {
		જો (refname_matches_negative_refspec_item(ref->name, &states->remote->fetch))
			string_list_append(&states->skipped, abbrev_branch(ref->name));
		નહીંતર જો (!ref->peer_ref || !refs_ref_exists(get_main_ref_store(the_repository), ref->peer_ref->name))
			string_list_append(&states->new_refs, abbrev_branch(ref->name));
		નહીંતર
			string_list_append(&states->tracked, abbrev_branch(ref->name));
	}
	stale_refs = get_stale_heads(&states->remote->fetch, fetch_map);
	માટે (ref = stale_refs; ref; ref = ref->next) {
		સંરચના string_list_item *item =
			string_list_append(&states->stale, abbrev_branch(ref->name));
		item->util = xstrdup(ref->name);
	}
	free_refs(stale_refs);
	free_refs(fetch_map);

	string_list_sort(&states->new_refs);
	string_list_sort(&states->skipped);
	string_list_sort(&states->tracked);
	string_list_sort(&states->stale);

	પાછા 0;
}

સંરચના ધકેલો_info {
	અક્ષર *dest;
	પૂર્ણાંક forced;
	ગણના {
		ધકેલો_STATUS_CREATE = 0,
		ધકેલો_STATUS_DELETE,
		ધકેલો_STATUS_UPTODATE,
		ધકેલો_STATUS_FASTFORWARD,
		ધકેલો_STATUS_OUTOFDATE,
		ધકેલો_STATUS_NOTQUERIED
	} status;
};

સ્થિર પૂર્ણાંક get_ધકેલો_ref_states(અચલ સંરચના ref *remote_refs,
	સંરચના ref_states *states)
{
	સંરચના remote *remote = states->remote;
	સંરચના ref *ref, *local_refs, *ધકેલો_map;
	જો (remote->mirror)
		પાછા 0;

	local_refs = get_local_heads();
	ધકેલો_map = copy_ref_list(remote_refs);

	match_ધકેલો_refs(local_refs, &ધકેલો_map, &remote->ધકેલો, MATCH_REFS_NONE);

	માટે (ref = ધકેલો_map; ref; ref = ref->next) {
		સંરચના string_list_item *item;
		સંરચના ધકેલો_info *info;

		જો (!ref->peer_ref)
			ચાલુ;
		oidcpy(&ref->new_oid, &ref->peer_ref->new_oid);

		item = string_list_append(&states->ધકેલો,
					  abbrev_branch(ref->peer_ref->name));
		item->util = xcalloc(1, કદ(સંરચના ધકેલો_info));
		info = item->util;
		info->forced = ref->force;
		info->dest = xstrdup(abbrev_branch(ref->name));

		જો (is_null_oid(&ref->new_oid)) {
			info->status = ધકેલો_STATUS_DELETE;
		} નહીંતર જો (oideq(&ref->old_oid, &ref->new_oid))
			info->status = ધકેલો_STATUS_UPTODATE;
		નહીંતર જો (is_null_oid(&ref->old_oid))
			info->status = ધકેલો_STATUS_CREATE;
		નહીંતર જો (odb_has_object(the_repository->objects, &ref->old_oid,
					ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR) &&
			 ref_newer(&ref->new_oid, &ref->old_oid))
			info->status = ધકેલો_STATUS_FASTFORWARD;
		નહીંતર
			info->status = ધકેલો_STATUS_OUTOFDATE;
	}
	free_refs(local_refs);
	free_refs(ધકેલો_map);
	પાછા 0;
}

સ્થિર પૂર્ણાંક get_ધકેલો_ref_states_noquery(સંરચના ref_states *states)
{
	પૂર્ણાંક i;
	સંરચના remote *remote = states->remote;
	સંરચના string_list_item *item;
	સંરચના ધકેલો_info *info;

	જો (remote->mirror)
		પાછા 0;

	જો (!remote->ધકેલો.nr) {
		item = string_list_append(&states->ધકેલો, _("(matching)"));
		info = item->util = xcalloc(1, કદ(સંરચના ધકેલો_info));
		info->status = ધકેલો_STATUS_NOTQUERIED;
		info->dest = xstrdup(item->string);
	}
	માટે (i = 0; i < remote->ધકેલો.nr; i++) {
		અચલ સંરચના refspec_item *spec = &remote->ધકેલો.વસ્તુઓ[i];
		જો (spec->matching)
			item = string_list_append(&states->ધકેલો, _("(matching)"));
		નહીંતર જો (strlen(spec->src))
			item = string_list_append(&states->ધકેલો, spec->src);
		નહીંતર
			item = string_list_append(&states->ધકેલો, _("(delete)"));

		info = item->util = xcalloc(1, કદ(સંરચના ધકેલો_info));
		info->forced = spec->force;
		info->status = ધકેલો_STATUS_NOTQUERIED;
		info->dest = xstrdup(spec->dst ? spec->dst : item->string);
	}
	પાછા 0;
}

સ્થિર પૂર્ણાંક get_head_names(અચલ સંરચના ref *remote_refs, સંરચના ref_states *states)
{
	સંરચના ref *ref, *matches;
	સંરચના ref *fetch_map = ખાલી, **fetch_map_tail = &fetch_map;
	સંરચના refspec_item refspec = {
		.force = 0,
		.pattern = 1,
		.src = (અક્ષર *) "refs/heads/*",
		.dst = (અક્ષર *) "refs/heads/*",
	};

	get_fetch_map(remote_refs, &refspec, &fetch_map_tail, 0);
	matches = guess_remote_head(find_ref_by_name(remote_refs, "HEAD"),
				    fetch_map, REMOTE_GUESS_HEAD_ALL);
	માટે (ref = matches; ref; ref = ref->next)
		string_list_append(&states->heads, abbrev_branch(ref->name));

	free_refs(fetch_map);
	free_refs(matches);
	પાછા 0;
}

સંરચના known_remote {
	સંરચના known_remote *next;
	સંરચના remote *remote;
};

સંરચના known_remotes {
	સંરચના remote *to_delete;
	સંરચના known_remote *list;
};

સ્થિર પૂર્ણાંક add_known_remote(સંરચના remote *remote, શૂન્ય *cb_data)
{
	સંરચના known_remotes *all = cb_data;
	સંરચના known_remote *r;

	જો (!strcmp(all->to_delete->name, remote->name))
		પાછા 0;

	r = xmalloc(કદ(*r));
	r->remote = remote;
	r->next = all->list;
	all->list = r;
	પાછા 0;
}

સંરચના branches_for_remote {
	સંરચના remote *remote;
	સંરચના string_list *branches, *skipped;
	સંરચના known_remotes *keep;
};

સ્થિર પૂર્ણાંક add_branch_for_removal(અચલ સંરચના reference *ref, શૂન્ય *cb_data)
{
	સંરચના branches_for_remote *branches = cb_data;
	સંરચના refspec_item refspec;
	સંરચના known_remote *kr;

	memset(&refspec, 0, કદ(refspec));
	refspec.dst = (અક્ષર *)ref->name;
	જો (remote_find_tracking(branches->remote, &refspec))
		પાછા 0;
	મુક્ત(refspec.src);

	/* don't delete a branch if another remote also uses it */
	માટે (kr = branches->keep->list; kr; kr = kr->next) {
		memset(&refspec, 0, કદ(refspec));
		refspec.dst = (અક્ષર *)ref->name;
		જો (!remote_find_tracking(kr->remote, &refspec)) {
			મુક્ત(refspec.src);
			પાછા 0;
		}
	}

	/* don't delete non-remote-tracking refs */
	જો (!starts_with(ref->name, "refs/remotes/")) {
		/* advise user how to delete local branches */
		જો (starts_with(ref->name, "refs/heads/"))
			string_list_append(branches->skipped,
					   abbrev_branch(ref->name));
		/* silently skip over other non-remote refs */
		પાછા 0;
	}

	string_list_append(branches->branches, ref->name);

	પાછા 0;
}

સંરચના rename_info {
	અચલ અક્ષર *old_name;
	અચલ અક્ષર *new_name;
	સંરચના ref_transaction *transaction;
	સંરચના progress *progress;
	સંરચના strbuf *err;
	uint32_t progress_nr;
	uint64_t index;
};

સ્થિર શૂન્ય compute_renamed_ref(સંરચના rename_info *rename,
				અચલ અક્ષર *refname,
				સંરચના strbuf *out)
{
	strbuf_reset(out);
	strbuf_addstr(out, refname);
	strbuf_splice(out, strlen("refs/remotes/"), strlen(rename->old_name),
		      rename->new_name, strlen(rename->new_name));
}

સ્થિર પૂર્ણાંક rename_one_reflog_entry(અચલ અક્ષર *old_refname,
				   સંરચના object_id *old_oid,
				   સંરચના object_id *new_oid,
				   અચલ અક્ષર *committer,
				   timestamp_t timestamp, પૂર્ણાંક tz,
				   અચલ અક્ષર *સંદેશ, શૂન્ય *cb_data)
{
	સંરચના rename_info *rename = cb_data;
	સંરચના strbuf new_refname = STRBUF_INIT;
	સંરચના strbuf identity = STRBUF_INIT;
	સંરચના strbuf name = STRBUF_INIT;
	સંરચના strbuf mail = STRBUF_INIT;
	સંરચના ident_split ident;
	અચલ અક્ષર *date;
	પૂર્ણાંક error;

	compute_renamed_ref(rename, old_refname, &new_refname);

	જો (split_ident_લીટી(&ident, committer, strlen(committer)) < 0) {
		error = -1;
		જાઓ out;
	}

	strbuf_add(&name, ident.name_begin, ident.name_end - ident.name_begin);
	strbuf_add(&mail, ident.mail_begin, ident.mail_end - ident.mail_begin);

	date = show_date(timestamp, tz, DATE_MODE(NORMAL));
	strbuf_addstr(&identity, fmt_ident(name.buf, mail.buf,
					  WANT_BLANK_IDENT, date, 0));

	error = ref_transaction_update_reflog(rename->transaction, new_refname.buf,
					      new_oid, old_oid, identity.buf, સંદેશ,
					      rename->index++, rename->err);

out:
	strbuf_release(&new_refname);
	strbuf_release(&identity);
	strbuf_release(&name);
	strbuf_release(&mail);
	પાછા error;
}

સ્થિર પૂર્ણાંક rename_one_reflog(અચલ અક્ષર *old_refname,
			     અચલ સંરચના object_id *old_oid,
			     સંરચના rename_info *rename)
{
	સંરચના strbuf new_refname = STRBUF_INIT;
	સંરચના strbuf message = STRBUF_INIT;
	પૂર્ણાંક error;

	જો (!refs_reflog_exists(get_main_ref_store(the_repository), old_refname))
		પાછા 0;

	error = refs_for_each_reflog_ent(get_main_ref_store(the_repository),
					 old_refname, rename_one_reflog_entry, rename);
	જો (error < 0)
		જાઓ out;

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
	જો (error < 0)
		પાછા error;

out:
	strbuf_release(&new_refname);
	strbuf_release(&message);
	પાછા error;
}

સ્થિર પૂર્ણાંક rename_one_ref(અચલ સંરચના reference *ref, શૂન્ય *cb_data)
{
	સંરચના strbuf new_referent = STRBUF_INIT;
	સંરચના strbuf new_refname = STRBUF_INIT;
	સંરચના rename_info *rename = cb_data;
	અચલ સંરચના object_id *oid = ref->oid;
	અચલ અક્ષર *referent = ref->target;
	પૂર્ણાંક error;

	compute_renamed_ref(rename, ref->name, &new_refname);

	જો (ref->flags & REF_ISSYMREF) {
		/*
		 * Stupidly enough `referent` is not pointing to the immediate
		 * target of a symref, but it's the recursively resolved value.
		 * So symrefs pointing to symrefs would be misresolved, and
		 * unborn symrefs don't have any value for the `referent` at all.
		 */
		referent = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
						   ref->name, RESOLVE_REF_NO_RECURSE,
						   ખાલી, ખાલી);
		compute_renamed_ref(rename, referent, &new_referent);
		oid = ખાલી;
	}

	error = ref_transaction_delete(rename->transaction, ref->name,
				       oid, referent, REF_NO_DEREF, ખાલી, rename->err);
	જો (error < 0)
		જાઓ out;

	error = ref_transaction_update(rename->transaction, new_refname.buf, oid, null_oid(the_hash_algo),
				       (ref->flags & REF_ISSYMREF) ? new_referent.buf : ખાલી, ખાલી,
				       REF_SKIP_CREATE_REFLOG | REF_NO_DEREF | REF_SKIP_OID_VERIFICATION,
				       ખાલી, rename->err);
	જો (error < 0)
		જાઓ out;

	error = rename_one_reflog(ref->name, oid, rename);
	જો (error < 0)
		જાઓ out;

	display_progress(rename->progress, ++rename->progress_nr);

out:
	strbuf_release(&new_referent);
	strbuf_release(&new_refname);
	પાછા error;
}

સ્થિર પૂર્ણાંક migrate_file(સંરચના remote *remote)
{
	સંરચના strbuf buf = STRBUF_INIT;

	strbuf_addf(&buf, "remote.%s.url", remote->name);
	માટે (size_t i = 0; i < remote->url.nr; i++)
		repo_config_set_multivar(the_repository, buf.buf, remote->url.v[i], "^$", 0);
	strbuf_reset(&buf);
	strbuf_addf(&buf, "remote.%s.push", remote->name);
	માટે (પૂર્ણાંક i = 0; i < remote->ધકેલો.nr; i++)
		repo_config_set_multivar(the_repository, buf.buf, remote->ધકેલો.વસ્તુઓ[i].raw, "^$", 0);
	strbuf_reset(&buf);
	strbuf_addf(&buf, "remote.%s.fetch", remote->name);
	માટે (પૂર્ણાંક i = 0; i < remote->fetch.nr; i++)
		repo_config_set_multivar(the_repository, buf.buf, remote->fetch.વસ્તુઓ[i].raw, "^$", 0);
#ifndef WITH_BREAKING_CHANGES
	જો (remote->origin == REMOTE_REMOTES)
		unlink_or_warn(repo_git_path_replace(the_repository, &buf,
						     "remotes/%s", remote->name));
	નહીંતર જો (remote->origin == REMOTE_BRANCHES)
		unlink_or_warn(repo_git_path_replace(the_repository, &buf,
						     "branches/%s", remote->name));
#endif /* WITH_BREAKING_CHANGES */
	strbuf_release(&buf);

	પાછા 0;
}

સંરચના ધકેલો_default_info
{
	અચલ અક્ષર *old_name;
	ગણના config_scope scope;
	સંરચના strbuf origin;
	પૂર્ણાંક linenr;
};

સ્થિર પૂર્ણાંક config_read_ધકેલો_default(અચલ અક્ષર *ચાવી, અચલ અક્ષર *કિંમત,
	અચલ સંરચના config_context *ctx, શૂન્ય *cb)
{
	અચલ સંરચના ચાવી_કિંમત_info *kvi = ctx->kvi;

	સંરચના ધકેલો_default_info* info = cb;
	જો (strcmp(ચાવી, "remote.pushdefault") ||
	    !કિંમત || strcmp(કિંમત, info->old_name))
		પાછા 0;

	info->scope = kvi->scope;
	strbuf_reset(&info->origin);
	strbuf_addstr(&info->origin, config_origin_type_name(kvi->origin_type));
	info->linenr = kvi->linenr;

	પાછા 0;
}

સ્થિર શૂન્ય handle_ધકેલો_default(અચલ અક્ષર* old_name, અચલ અક્ષર* new_name)
{
	સંરચના ધકેલો_default_info ધકેલો_default = {
		.old_name = old_name,
		.scope = CONFIG_SCOPE_UNKNOWN,
		.origin = STRBUF_INIT,
		.linenr = -1,
	};
	repo_config(the_repository, config_read_ધકેલો_default, &ધકેલો_default);
	જો (ધકેલો_default.scope >= CONFIG_SCOPE_COMMAND)
		; /* pass */
	નહીંતર જો (ધકેલો_default.scope >= CONFIG_SCOPE_LOCAL) {
		પૂર્ણાંક result = repo_config_set_gently(the_repository, "remote.pushDefault",
						    new_name);
		જો (new_name && result && result != CONFIG_NOTHING_SET)
			die(_("could not set '%s'"), "remote.pushDefault");
		નહીંતર જો (!new_name && result && result != CONFIG_NOTHING_SET)
			die(_("could not unset '%s'"), "remote.pushDefault");
	} નહીંતર જો (ધકેલો_default.scope >= CONFIG_SCOPE_SYSTEM) {
		/* warn */
		warning(_("The %s configuration remote.pushDefault in:\n"
			  "\t%s:%d\n"
			  "now names the non-existent remote '%s'"),
			config_scope_name(ધકેલો_default.scope),
			ધકેલો_default.origin.buf, ધકેલો_default.linenr,
			old_name);
	}

	strbuf_release(&ધકેલો_default.origin);
}

સ્થિર અચલ અક્ષર conflicting_remote_refs_advice[] = N_(
	"The remote you are trying to rename has conflicting references in the\n"
	"new target refspec. This is most likely caused by you trying to nest\n"
	"a remote into itself, e.g. by renaming 'parent' into 'parent/child'\n"
	"or by unnesting a remote, e.g. the other way round.\n"
	"\n"
	"If that is the case, you can address this by first renaming the\n"
	"remote to a different name.\n");

સ્થિર પૂર્ણાંક mv(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
	      સંરચના repository *repo UNUSED)
{
	પૂર્ણાંક show_progress = isatty(2);
	સંરચના option options[] = {
		OPT_BOOL(0, "progress", &show_progress, N_("force progress reporting")),
		OPT_END()
	};
	સંરચના remote *oldremote, *newremote;
	સંરચના strbuf buf = STRBUF_INIT, buf2 = STRBUF_INIT, buf3 = STRBUF_INIT,
		old_remote_context = STRBUF_INIT, err = STRBUF_INIT;
	સંરચના rename_info rename = {
		.err = &err,
	};
	પૂર્ણાંક refspecs_need_update = 0;
	પૂર્ણાંક result = 0;

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_rename_usage, 0);

	જો (argc != 2)
		usage_with_options(builtin_remote_rename_usage, options);

	rename.old_name = argv[0];
	rename.new_name = argv[1];

	oldremote = remote_get(rename.old_name);
	જો (!remote_is_configured(oldremote, 1)) {
		error(_("No such remote: '%s'"), rename.old_name);
		exit(2);
	}

	જો (!strcmp(rename.old_name, rename.new_name) && oldremote->origin != REMOTE_CONFIG)
		પાછા migrate_file(oldremote);

	newremote = remote_get(rename.new_name);
	જો (remote_is_configured(newremote, 1)) {
		error(_("remote %s already exists."), rename.new_name);
		exit(3);
	}

	જો (!valid_remote_name(rename.new_name))
		die(_("'%s' is not a valid remote name"), rename.new_name);

	strbuf_addf(&buf, "remote.%s", rename.old_name);
	strbuf_addf(&buf2, "remote.%s", rename.new_name);
	જો (repo_config_rename_section(the_repository, buf.buf, buf2.buf) < 1) {
		result = error(_("Could not rename config section '%s' to '%s'"),
			       buf.buf, buf2.buf);
		જાઓ out;
	}

	strbuf_addf(&old_remote_context, ":refs/remotes/%s/", rename.old_name);

	માટે (પૂર્ણાંક i = 0; i < oldremote->fetch.nr && !refspecs_need_update; i++)
		refspecs_need_update = !!strstr(oldremote->fetch.વસ્તુઓ[i].raw,
						old_remote_context.buf);

	જો (refspecs_need_update) {
		સંરચના refs_for_each_ref_options opts = {
			.flags = REFS_FOR_EACH_INCLUDE_BROKEN,
		};
		rename.transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
							       0, &err);
		જો (!rename.transaction)
			જાઓ out;

		જો (show_progress)
			rename.progress = start_delayed_progress(the_repository,
								 _("Renaming remote references"), 0);

		strbuf_reset(&buf);
		strbuf_addf(&buf, "refs/remotes/%s/", rename.old_name);
		opts.prefix = buf.buf;

		result = refs_for_each_ref_ext(get_main_ref_store(the_repository),
					       rename_one_ref, &rename, &opts);
		જો (result < 0)
			die(_("queueing remote ref renames failed: %s"), rename.err->buf);

		result = ref_transaction_prepare(rename.transaction, &err);
		જો (result < 0) {
			error("renaming remote references failed: %s", err.buf);
			જો (result == REF_TRANSACTION_ERROR_NAME_CONFLICT)
				advise(conflicting_remote_refs_advice);
			die(ખાલી);
		}
	}

	જો (oldremote->fetch.nr) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.fetch", rename.new_name);
		repo_config_set_multivar(the_repository, buf.buf, ખાલી, ખાલી, CONFIG_FLAGS_MULTI_REPLACE);
		માટે (પૂર્ણાંક i = 0; i < oldremote->fetch.nr; i++) {
			અક્ષર *ptr;

			strbuf_reset(&buf2);
			strbuf_addstr(&buf2, oldremote->fetch.વસ્તુઓ[i].raw);
			ptr = strstr(buf2.buf, old_remote_context.buf);
			જો (ptr) {
				strbuf_splice(&buf2,
					      ptr-buf2.buf + strlen(":refs/remotes/"),
					      strlen(rename.old_name), rename.new_name,
					      strlen(rename.new_name));
			} નહીંતર
				warning(_("Not updating non-default fetch refspec\n"
					  "\t%s\n"
					  "\tPlease update the configuration manually if necessary."),
					buf2.buf);

			repo_config_set_multivar(the_repository, buf.buf, buf2.buf, "^$", 0);
		}
	}

	read_branches();
	માટે (size_t i = 0; i < branch_list.nr; i++) {
		સંરચના string_list_item *item = branch_list.વસ્તુઓ + i;
		સંરચના branch_info *info = item->util;
		જો (info->remote_name && !strcmp(info->remote_name, rename.old_name)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "branch.%s.remote", item->string);
			repo_config_set(the_repository, buf.buf, rename.new_name);
		}
		જો (info->ધકેલો_remote_name && !strcmp(info->ધકેલો_remote_name, rename.old_name)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "branch.%s.pushRemote", item->string);
			repo_config_set(the_repository, buf.buf, rename.new_name);
		}
	}

	જો (refspecs_need_update) {
		result = ref_transaction_commit(rename.transaction, &err);
		જો (result < 0)
			die(_("renaming remote refs failed: %s"), rename.err->buf);

		stop_progress(&rename.progress);

		handle_ધકેલો_default(rename.old_name, rename.new_name);
	}

out:
	ref_transaction_free(rename.transaction);
	strbuf_release(&old_remote_context);
	strbuf_release(&buf);
	strbuf_release(&buf2);
	strbuf_release(&buf3);
	strbuf_release(&err);
	પાછા result;
}

સ્થિર પૂર્ણાંક rm(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
	      સંરચના repository *repo UNUSED)
{
	સંરચના option options[] = {
		OPT_END()
	};
	સંરચના remote *remote;
	સંરચના strbuf buf = STRBUF_INIT;
	સંરચના known_remotes known_remotes = { ખાલી, ખાલી };
	સંરચના string_list branches = STRING_LIST_INIT_DUP;
	સંરચના string_list skipped = STRING_LIST_INIT_DUP;
	સંરચના branches_for_remote cb_data;
	પૂર્ણાંક result;

	memset(&cb_data, 0, કદ(cb_data));
	cb_data.branches = &branches;
	cb_data.skipped = &skipped;
	cb_data.keep = &known_remotes;

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_rm_usage, 0);
	જો (argc != 1)
		usage_with_options(builtin_remote_rm_usage, options);

	remote = remote_get(argv[0]);
	જો (!remote_is_configured(remote, 1)) {
		error(_("No such remote: '%s'"), argv[0]);
		exit(2);
	}

	known_remotes.to_delete = remote;
	for_each_remote(add_known_remote, &known_remotes);

	read_branches();
	માટે (size_t i = 0; i < branch_list.nr; i++) {
		સંરચના string_list_item *item = branch_list.વસ્તુઓ + i;
		સંરચના branch_info *info = item->util;
		જો (info->remote_name && !strcmp(info->remote_name, remote->name)) {
			અચલ અક્ષર *ચાવીઓ[] = { "remote", "merge", ખાલી }, **k;
			માટે (k = ચાવીઓ; *k; k++) {
				strbuf_reset(&buf);
				strbuf_addf(&buf, "branch.%s.%s",
						item->string, *k);
				result = repo_config_set_gently(the_repository, buf.buf, ખાલી);
				જો (result && result != CONFIG_NOTHING_SET)
					die(_("could not unset '%s'"), buf.buf);
			}
		}
		જો (info->ધકેલો_remote_name && !strcmp(info->ધકેલો_remote_name, remote->name)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "branch.%s.pushremote", item->string);
			result = repo_config_set_gently(the_repository, buf.buf, ખાલી);
			જો (result && result != CONFIG_NOTHING_SET)
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

	જો (!result)
		result = refs_delete_refs(get_main_ref_store(the_repository),
					  "remote: remove", &branches,
					  REF_NO_DEREF);
	string_list_clear(&branches, 0);

	જો (skipped.nr) {
		fprintf_ln(stderr,
			   Q_("Note: A branch outside the refs/remotes/ hierarchy was not removed;\n"
			      "to delete it, use:",
			      "Note: Some branches outside the refs/remotes/ hierarchy were not removed;\n"
			      "to delete them, use:",
			      skipped.nr));
		માટે (size_t i = 0; i < skipped.nr; i++)
			fprintf(stderr, "  git branch -d %s\n",
				skipped.વસ્તુઓ[i].string);
	}
	string_list_clear(&skipped, 0);

	જો (!result) {
		strbuf_addf(&buf, "remote.%s", remote->name);
		જો (repo_config_rename_section(the_repository, buf.buf, ખાલી) < 1) {
			result = error(_("Could not remove config section '%s'"), buf.buf);
			જાઓ out;
		}

		handle_ધકેલો_default(remote->name, ખાલી);
	}

out:
	માટે (સંરચના known_remote *r = known_remotes.list; r;) {
		સંરચના known_remote *next = r->next;
		મુક્ત(r);
		r = next;
	}
	strbuf_release(&buf);
	પાછા result;
}

સ્થિર શૂન્ય clear_ધકેલો_info(શૂન્ય *util, અચલ અક્ષર *string UNUSED)
{
	સંરચના ધકેલો_info *info = util;
	મુક્ત(info->dest);
	મુક્ત(info);
}

સ્થિર શૂન્ય free_remote_ref_states(સંરચના ref_states *states)
{
	string_list_clear(&states->new_refs, 0);
	string_list_clear(&states->skipped, 0);
	string_list_clear(&states->stale, 1);
	string_list_clear(&states->tracked, 0);
	string_list_clear(&states->heads, 0);
	string_list_clear_func(&states->ધકેલો, clear_ધકેલો_info);
}

સ્થિર પૂર્ણાંક append_ref_to_tracked_list(અચલ સંરચના reference *ref, શૂન્ય *cb_data)
{
	સંરચના ref_states *states = cb_data;
	સંરચના refspec_item refspec;

	જો (ref->flags & REF_ISSYMREF)
		પાછા 0;

	memset(&refspec, 0, કદ(refspec));
	refspec.dst = (અક્ષર *)ref->name;
	જો (!remote_find_tracking(states->remote, &refspec)) {
		string_list_append(&states->tracked, abbrev_branch(refspec.src));
		મુક્ત(refspec.src);
	}

	પાછા 0;
}

સ્થિર પૂર્ણાંક get_remote_ref_states(અચલ અક્ષર *name,
				 સંરચના ref_states *states,
				 પૂર્ણાંક query)
{
	states->remote = remote_get(name);
	જો (!states->remote)
		પાછા error(_("No such remote: '%s'"), name);

	read_branches();

	જો (query) {
		સંરચના transport *transport;
		અચલ સંરચના ref *remote_refs;

		transport = transport_get(states->remote, states->remote->url.v[0]);
		remote_refs = transport_get_remote_refs(transport, ખાલી);

		states->queried = 1;
		જો (query & GET_REF_STATES)
			get_ref_states(remote_refs, states);
		જો (query & GET_HEAD_NAMES)
			get_head_names(remote_refs, states);
		જો (query & GET_ધકેલો_REF_STATES)
			get_ધકેલો_ref_states(remote_refs, states);
		transport_disconnect(transport);
	} નહીંતર {
		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_ref_to_tracked_list, states);
		string_list_sort(&states->tracked);
		get_ધકેલો_ref_states_noquery(states);
	}

	પાછા 0;
}

સંરચના show_info {
	સંરચના string_list list;
	સંરચના ref_states states;
	પૂર્ણાંક width, width2;
	પૂર્ણાંક any_rebase;
};

#define SHOW_INFO_INIT { \
	.list = STRING_LIST_INIT_DUP, \
	.states = REF_STATES_INIT, \
}

સ્થિર પૂર્ણાંક add_remote_to_show_info(સંરચના string_list_item *item, શૂન્ય *cb_data)
{
	સંરચના show_info *info = cb_data;
	પૂર્ણાંક n = strlen(item->string);
	જો (n > info->width)
		info->width = n;
	string_list_ઉમેરો(&info->list, item->string);
	પાછા 0;
}

સ્થિર પૂર્ણાંક show_remote_info_item(સંરચના string_list_item *item, શૂન્ય *cb_data)
{
	સંરચના show_info *info = cb_data;
	સંરચના ref_states *states = &info->states;
	અચલ અક્ષર *name = item->string;

	જો (states->queried) {
		અચલ અક્ષર *fmt = "%s";
		અચલ અક્ષર *arg = "";
		જો (string_list_has_string(&states->new_refs, name)) {
			fmt = _(" new (next fetch will store in remotes/%s)");
			arg = states->remote->name;
		} નહીંતર જો (string_list_has_string(&states->tracked, name))
			arg = _(" tracked");
		નહીંતર જો (string_list_has_string(&states->skipped, name))
			arg = _(" skipped");
		નહીંતર જો (string_list_has_string(&states->stale, name))
			arg = _(" stale (use 'git remote prune' to remove)");
		નહીંતર
			arg = _(" ???");
		printf("    %-*s", info->width, name);
		printf(fmt, arg);
		printf("\n");
	} નહીંતર
		printf("    %s\n", name);

	પાછા 0;
}

સ્થિર પૂર્ણાંક add_local_to_show_info(સંરચના string_list_item *branch_item, શૂન્ય *cb_data)
{
	સંરચના show_info *show_info = cb_data;
	સંરચના ref_states *states = &show_info->states;
	સંરચના branch_info *branch_info = branch_item->util;
	સંરચના string_list_item *item;
	પૂર્ણાંક n;

	જો (!branch_info->merge.nr || !branch_info->remote_name ||
	    strcmp(states->remote->name, branch_info->remote_name))
		પાછા 0;
	જો ((n = strlen(branch_item->string)) > show_info->width)
		show_info->width = n;
	જો (branch_info->rebase >= REBASE_TRUE)
		show_info->any_rebase = 1;

	item = string_list_ઉમેરો(&show_info->list, branch_item->string);
	item->util = branch_info;

	પાછા 0;
}

સ્થિર પૂર્ણાંક show_local_info_item(સંરચના string_list_item *item, શૂન્ય *cb_data)
{
	સંરચના show_info *show_info = cb_data;
	સંરચના branch_info *branch_info = item->util;
	સંરચના string_list *merge = &branch_info->merge;
	પૂર્ણાંક width = show_info->width + 4;

	જો (branch_info->rebase >= REBASE_TRUE && branch_info->merge.nr > 1) {
		error(_("invalid branch.%s.merge; cannot rebase onto > 1 branch"),
			item->string);
		પાછા 0;
	}

	printf("    %-*s ", show_info->width, item->string);
	જો (branch_info->rebase >= REBASE_TRUE) {
		અચલ અક્ષર *સંદેશ;
		જો (branch_info->rebase == REBASE_INTERACTIVE)
			સંદેશ = _("rebases interactively onto remote %s");
		નહીંતર જો (branch_info->rebase == REBASE_MERGES)
			સંદેશ = _("rebases interactively (with merges) onto "
				"remote %s");
		નહીંતર
			સંદેશ = _("rebases onto remote %s");
		printf_ln(સંદેશ, merge->વસ્તુઓ[0].string);
		પાછા 0;
	} નહીંતર જો (show_info->any_rebase) {
		printf_ln(_(" merges with remote %s"), merge->વસ્તુઓ[0].string);
		width++;
	} નહીંતર {
		printf_ln(_("merges with remote %s"), merge->વસ્તુઓ[0].string);
	}
	માટે (size_t i = 1; i < merge->nr; i++)
		printf(_("%-*s    and with remote %s\n"), width, "",
		       merge->વસ્તુઓ[i].string);

	પાછા 0;
}

સ્થિર પૂર્ણાંક add_ધકેલો_to_show_info(સંરચના string_list_item *ધકેલો_item, શૂન્ય *cb_data)
{
	સંરચના show_info *show_info = cb_data;
	સંરચના ધકેલો_info *ધકેલો_info = ધકેલો_item->util;
	સંરચના string_list_item *item;
	પૂર્ણાંક n;
	જો ((n = strlen(ધકેલો_item->string)) > show_info->width)
		show_info->width = n;
	જો ((n = strlen(ધકેલો_info->dest)) > show_info->width2)
		show_info->width2 = n;
	item = string_list_append(&show_info->list, ધકેલો_item->string);
	item->util = ધકેલો_item->util;
	પાછા 0;
}

/*
 * Sorting comparison for a string list that has push_info
 * structs in its util field
 */
સ્થિર પૂર્ણાંક cmp_string_with_ધકેલો(અચલ શૂન્ય *va, અચલ શૂન્ય *vb)
{
	અચલ સંરચના string_list_item *a = va;
	અચલ સંરચના string_list_item *b = vb;
	અચલ સંરચના ધકેલો_info *a_ધકેલો = a->util;
	અચલ સંરચના ધકેલો_info *b_ધકેલો = b->util;
	પૂર્ણાંક cmp = strcmp(a->string, b->string);
	પાછા cmp ? cmp : strcmp(a_ધકેલો->dest, b_ધકેલો->dest);
}

સ્થિર પૂર્ણાંક show_ધકેલો_info_item(સંરચના string_list_item *item, શૂન્ય *cb_data)
{
	સંરચના show_info *show_info = cb_data;
	સંરચના ધકેલો_info *ધકેલો_info = item->util;
	અચલ અક્ષર *src = item->string, *status = ખાલી;

	પસંદગી (ધકેલો_info->status) {
	સ્થિતિ ધકેલો_STATUS_CREATE:
		status = _("create");
		તોડો;
	સ્થિતિ ધકેલો_STATUS_DELETE:
		status = _("delete");
		src = _("(none)");
		તોડો;
	સ્થિતિ ધકેલો_STATUS_UPTODATE:
		status = _("up to date");
		તોડો;
	સ્થિતિ ધકેલો_STATUS_FASTFORWARD:
		status = _("fast-forwardable");
		તોડો;
	સ્થિતિ ધકેલો_STATUS_OUTOFDATE:
		status = _("local out of date");
		તોડો;
	સ્થિતિ ધકેલો_STATUS_NOTQUERIED:
		તોડો;
	}
	જો (status) {
		જો (ધકેલો_info->forced)
			printf_ln(_("    %-*s forces to %-*s (%s)"), show_info->width, src,
			       show_info->width2, ધકેલો_info->dest, status);
		નહીંતર
			printf_ln(_("    %-*s pushes to %-*s (%s)"), show_info->width, src,
			       show_info->width2, ધકેલો_info->dest, status);
	} નહીંતર {
		જો (ધકેલો_info->forced)
			printf_ln(_("    %-*s forces to %s"), show_info->width, src,
			       ધકેલો_info->dest);
		નહીંતર
			printf_ln(_("    %-*s pushes to %s"), show_info->width, src,
			       ધકેલો_info->dest);
	}
	પાછા 0;
}

સ્થિર પૂર્ણાંક get_one_entry(સંરચના remote *remote, શૂન્ય *priv)
{
	સંરચના string_list *list = priv;
	સંરચના strbuf remote_info_buf = STRBUF_INIT;
	સંરચના strvec *url;

	જો (remote->url.nr > 0) {
		સંરચના strbuf promisor_config = STRBUF_INIT;
		અચલ અક્ષર *partial_clone_filter = ખાલી;

		strbuf_addf(&promisor_config, "remote.%s.partialclonefilter", remote->name);
		strbuf_addf(&remote_info_buf, "%s (fetch)", remote->url.v[0]);
		જો (!repo_config_get_string_tmp(the_repository, promisor_config.buf, &partial_clone_filter))
			strbuf_addf(&remote_info_buf, " [%s]", partial_clone_filter);

		strbuf_release(&promisor_config);
		string_list_append(list, remote->name)->util =
				strbuf_detach(&remote_info_buf, ખાલી);
	} નહીંતર
		string_list_append(list, remote->name)->util = ખાલી;
	url = ધકેલો_url_of_remote(remote);
	માટે (size_t i = 0; i < url->nr; i++) {
		strbuf_addf(&remote_info_buf, "%s (push)", url->v[i]);
		string_list_append(list, remote->name)->util =
				strbuf_detach(&remote_info_buf, ખાલી);
	}

	પાછા 0;
}

સ્થિર પૂર્ણાંક show_all(શૂન્ય)
{
	સંરચના string_list list = STRING_LIST_INIT_DUP;
	પૂર્ણાંક result;

	result = for_each_remote(get_one_entry, &list);

	જો (!result) {
		string_list_sort(&list);
		માટે (size_t i = 0; i < list.nr; i++) {
			સંરચના string_list_item *item = list.વસ્તુઓ + i;
			જો (verbose)
				printf("%s\t%s\n", item->string,
					item->util ? (અચલ અક્ષર *)item->util : "");
			નહીંતર {
				જો (i && !strcmp((item - 1)->string, item->string))
					ચાલુ;
				printf("%s\n", item->string);
			}
		}
	}
	string_list_clear(&list, 1);
	પાછા result;
}

સ્થિર પૂર્ણાંક show(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
		સંરચના repository *repo UNUSED)
{
	પૂર્ણાંક no_query = 0, result = 0, query_flag = 0;
	સંરચના option options[] = {
		OPT_BOOL('n', ખાલી, &no_query, N_("do not query remotes")),
		OPT_END()
	};
	સંરચના show_info info = SHOW_INFO_INIT;

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_show_usage,
			     0);

	જો (argc < 1)
		પાછા show_all();

	જો (!no_query)
		query_flag = (GET_REF_STATES | GET_HEAD_NAMES | GET_ધકેલો_REF_STATES);

	માટે (; argc; argc--, argv++) {
		size_t i;
		સંરચના strvec *url;

		get_remote_ref_states(*argv, &info.states, query_flag);

		printf_ln(_("* remote %s"), *argv);
		printf_ln(_("  Fetch URL: %s"), info.states.remote->url.v[0]);
		url = ધકેલો_url_of_remote(info.states.remote);
		માટે (i = 0; i < url->nr; i++)
			/*
			 * TRANSLATORS: the colon ':' should align
			 * with the one in " Fetch URL: %s"
			 * translation.
			 */
			printf_ln(_("  Push  URL: %s"), url->v[i]);
		જો (!i)
			printf_ln(_("  Push  URL: %s"), _("(no URL)"));
		જો (no_query)
			printf_ln(_("  HEAD branch: %s"), _("(not queried)"));
		નહીંતર જો (!info.states.heads.nr)
			printf_ln(_("  HEAD branch: %s"), _("(unknown)"));
		નહીંતર જો (info.states.heads.nr == 1)
			printf_ln(_("  HEAD branch: %s"), info.states.heads.વસ્તુઓ[0].string);
		નહીંતર {
			printf(_("  HEAD branch (remote HEAD is ambiguous,"
				 " may be one of the following):\n"));
			માટે (i = 0; i < info.states.heads.nr; i++)
				printf("    %s\n", info.states.heads.વસ્તુઓ[i].string);
		}

		/* remote branch info */
		info.width = 0;
		for_each_string_list(&info.states.new_refs, add_remote_to_show_info, &info);
		for_each_string_list(&info.states.skipped, add_remote_to_show_info, &info);
		for_each_string_list(&info.states.tracked, add_remote_to_show_info, &info);
		for_each_string_list(&info.states.stale, add_remote_to_show_info, &info);
		જો (info.list.nr)
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
		જો (info.list.nr)
			printf_ln(Q_("  Local branch configured for 'git pull':",
				     "  Local branches configured for 'git pull':",
				     info.list.nr));
		for_each_string_list(&info.list, show_local_info_item, &info);
		string_list_clear(&info.list, 0);

		/* git push info */
		જો (info.states.remote->mirror)
			printf_ln(_("  Local refs will be mirrored by 'git push'"));

		info.width = info.width2 = 0;
		for_each_string_list(&info.states.ધકેલો, add_ધકેલો_to_show_info, &info);
		QSORT(info.list.વસ્તુઓ, info.list.nr, cmp_string_with_ધકેલો);
		જો (info.list.nr)
			printf_ln(Q_("  Local ref configured for 'git push'%s:",
				     "  Local refs configured for 'git push'%s:",
				     info.list.nr),
				  no_query ? _(" (status not queried)") : "");
		for_each_string_list(&info.list, show_ધકેલો_info_item, &info);
		string_list_clear(&info.list, 0);

		free_remote_ref_states(&info.states);
	}

	પાછા result;
}

સ્થિર શૂન્ય report_set_head_auto(અચલ અક્ષર *remote, અચલ અક્ષર *head_name,
			સંરચના strbuf *b_local_head, પૂર્ણાંક was_detached) {
	સંરચના strbuf buf_prefix = STRBUF_INIT;
	અચલ અક્ષર *prev_head = ખાલી;

	strbuf_addf(&buf_prefix, "refs/remotes/%s/", remote);
	skip_prefix(b_local_head->buf, buf_prefix.buf, &prev_head);

	જો (prev_head && !strcmp(prev_head, head_name))
		printf(_("'%s/HEAD' is unchanged and points to '%s'\n"),
			remote, head_name);
	નહીંતર જો (prev_head)
		printf(_("'%s/HEAD' has changed from '%s' and now points to '%s'\n"),
			remote, prev_head, head_name);
	નહીંતર જો (!b_local_head->len)
		printf(_("'%s/HEAD' is now created and points to '%s'\n"),
			remote, head_name);
	નહીંતર જો (was_detached && b_local_head->len)
		printf(_("'%s/HEAD' was detached at '%s' and now points to '%s'\n"),
			remote, b_local_head->buf, head_name);
	નહીંતર
		printf(_("'%s/HEAD' used to point to '%s' "
			"(which is not a remote branch), but now points to '%s'\n"),
			remote, b_local_head->buf, head_name);
	strbuf_release(&buf_prefix);
}

સ્થિર પૂર્ણાંક set_head(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
		    સંરચના repository *repo UNUSED)
{
	પૂર્ણાંક opt_a = 0, opt_d = 0, result = 0, was_detached;
	સંરચના strbuf b_head = STRBUF_INIT, b_remote_head = STRBUF_INIT,
		b_local_head = STRBUF_INIT;
	અક્ષર *head_name = ખાલી;
	સંરચના ref_store *refs = get_main_ref_store(the_repository);
	સંરચના remote *remote;

	સંરચના option options[] = {
		OPT_BOOL('a', "auto", &opt_a,
			 N_("set refs/remotes/<name>/HEAD according to remote")),
		OPT_BOOL('d', "delete", &opt_d,
			 N_("delete refs/remotes/<name>/HEAD")),
		OPT_END()
	};
	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_sethead_usage, 0);

	/* All modes require at least a remote name. */
	જો (!argc)
		usage_with_options(builtin_remote_sethead_usage, options);

	strbuf_addf(&b_head, "refs/remotes/%s/HEAD", argv[0]);
	remote = remote_get(argv[0]);

	જો (!opt_a && !opt_d && argc == 2) {
		head_name = xstrdup(argv[1]);
	} નહીંતર જો (opt_a && !opt_d && argc == 1) {
		સંરચના ref_states states = REF_STATES_INIT;
		get_remote_ref_states(argv[0], &states, GET_HEAD_NAMES);
		જો (!states.heads.nr)
			result |= error(_("Cannot determine remote HEAD"));
		નહીંતર જો (states.heads.nr > 1) {
			result |= error(_("Multiple remote HEAD branches. "
					  "Please choose one explicitly with:"));
			માટે (size_t i = 0; i < states.heads.nr; i++)
				fprintf(stderr, "  git remote set-head %s %s\n",
					argv[0], states.heads.વસ્તુઓ[i].string);
		} નહીંતર
			head_name = xstrdup(states.heads.વસ્તુઓ[0].string);
		free_remote_ref_states(&states);
	} નહીંતર જો (opt_d && !opt_a && argc == 1) {
		જો (refs_delete_ref(refs, ખાલી, b_head.buf, ખાલી, REF_NO_DEREF))
			result |= error(_("Could not delete %s"), b_head.buf);
	} નહીંતર
		usage_with_options(builtin_remote_sethead_usage, options);

	જો (!head_name)
		જાઓ cleanup;
	strbuf_addf(&b_remote_head, "refs/remotes/%s/%s", argv[0], head_name);
	જો (!refs_ref_exists(refs, b_remote_head.buf)) {
		result |= error(_("Not a valid ref: %s"), b_remote_head.buf);
		જાઓ cleanup;
	}
	was_detached = refs_update_symref_extended(refs, b_head.buf, b_remote_head.buf,
			"remote set-head", &b_local_head, 0);
	જો (was_detached == -1) {
		result |= error(_("Could not set up %s"), b_head.buf);
		જાઓ cleanup;
	}
	જો (opt_a)
		report_set_head_auto(argv[0], head_name, &b_local_head, was_detached);
	જો (remote->follow_remote_head == FOLLOW_REMOTE_ALWAYS) {
		સંરચના strbuf config_name = STRBUF_INIT;
		strbuf_addf(&config_name,
			"remote.%s.followremotehead", remote->name);
		repo_config_set(the_repository, config_name.buf, "warn");
		strbuf_release(&config_name);
	}

cleanup:
	મુક્ત(head_name);
	strbuf_release(&b_head);
	strbuf_release(&b_remote_head);
	strbuf_release(&b_local_head);
	પાછા result;
}

સ્થિર પૂર્ણાંક prune_remote(અચલ અક્ષર *remote, પૂર્ણાંક dry_run)
{
	પૂર્ણાંક result = 0;
	સંરચના ref_states states = REF_STATES_INIT;
	સંરચના string_list refs_to_prune = STRING_LIST_INIT_NODUP;
	સંરચના string_list_item *item;

	get_remote_ref_states(remote, &states, GET_REF_STATES);

	જો (!states.stale.nr) {
		free_remote_ref_states(&states);
		પાછા 0;
	}

	printf_ln(_("Pruning %s"), remote);
	printf_ln(_("URL: %s"), states.remote->url.v[0]);

	for_each_string_list_item(item, &states.stale)
		string_list_append(&refs_to_prune, item->util);
	string_list_sort(&refs_to_prune);

	જો (!dry_run)
		result |= refs_delete_refs(get_main_ref_store(the_repository),
					   "remote: prune", &refs_to_prune, 0);

	for_each_string_list_item(item, &states.stale) {
		અચલ અક્ષર *refname = item->util;

		જો (dry_run)
			printf_ln(_(" * [would prune] %s"),
			       abbrev_ref(refname, "refs/remotes/"));
		નહીંતર
			printf_ln(_(" * [pruned] %s"),
			       abbrev_ref(refname, "refs/remotes/"));
	}

	refs_warn_dangling_symrefs(get_main_ref_store(the_repository),
				   stdout, " ", dry_run, &refs_to_prune);

	string_list_clear(&refs_to_prune, 0);
	free_remote_ref_states(&states);
	પાછા result;
}

સ્થિર પૂર્ણાંક prune(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
		 સંરચના repository *repo UNUSED)
{
	પૂર્ણાંક dry_run = 0, result = 0;
	સંરચના option options[] = {
		OPT__DRY_RUN(&dry_run, N_("dry run")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_prune_usage, 0);

	જો (argc < 1)
		usage_with_options(builtin_remote_prune_usage, options);

	માટે (; argc; argc--, argv++)
		result |= prune_remote(*argv, dry_run);

	પાછા result;
}

સ્થિર પૂર્ણાંક get_remote_default(અચલ અક્ષર *ચાવી, અચલ અક્ષર *કિંમત UNUSED,
			      અચલ સંરચના config_context *ctx UNUSED,
			      શૂન્ય *priv)
{
	જો (strcmp(ચાવી, "remotes.default") == 0) {
		પૂર્ણાંક *found = priv;
		*found = 1;
	}
	પાછા 0;
}

સ્થિર પૂર્ણાંક update(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
		  સંરચના repository *repo UNUSED)
{
	પૂર્ણાંક i, prune = -1;
	સંરચના option options[] = {
		OPT_BOOL('p', "prune", &prune,
			 N_("prune remotes after fetching")),
		OPT_END()
	};
	સંરચના child_process cmd = CHILD_PROCESS_INIT;
	પૂર્ણાંક default_defined = 0;

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_update_usage,
			     PARSE_OPT_KEEP_ARGV0);

	strvec_ધકેલો(&cmd.args, "fetch");

	જો (prune != -1)
		strvec_ધકેલો(&cmd.args, prune ? "--prune" : "--no-prune");
	જો (verbose)
		strvec_ધકેલો(&cmd.args, "-v");
	strvec_ધકેલો(&cmd.args, "--multiple");
	જો (argc < 2)
		strvec_ધકેલો(&cmd.args, "default");
	માટે (i = 1; i < argc; i++)
		strvec_ધકેલો(&cmd.args, argv[i]);

	જો (strcmp(cmd.args.v[cmd.args.nr-1], "default") == 0) {
		repo_config(the_repository, get_remote_default, &default_defined);
		જો (!default_defined) {
			strvec_કાઢો(&cmd.args);
			strvec_ધકેલો(&cmd.args, "--all");
		}
	}

	cmd.git_cmd = 1;
	પાછા run_command(&cmd);
}

સ્થિર પૂર્ણાંક remove_all_fetch_refspecs(અચલ અક્ષર *ચાવી)
{
	પાછા repo_config_set_multivar_gently(the_repository, ચાવી, ખાલી, ખાલી,
					       CONFIG_FLAGS_MULTI_REPLACE);
}

સ્થિર શૂન્ય add_branches(સંરચના remote *remote, અચલ અક્ષર **branches,
			 અચલ અક્ષર *ચાવી)
{
	અચલ અક્ષર *remotename = remote->name;
	પૂર્ણાંક mirror = remote->mirror;
	સંરચના strbuf refspec = STRBUF_INIT;

	માટે (; *branches; branches++)
		add_branch(ચાવી, *branches, remotename, mirror, &refspec);

	strbuf_release(&refspec);
}

સ્થિર પૂર્ણાંક set_remote_branches(અચલ અક્ષર *remotename, અચલ અક્ષર **branches,
				પૂર્ણાંક add_mode)
{
	સંરચના strbuf ચાવી = STRBUF_INIT;
	સંરચના remote *remote;

	strbuf_addf(&ચાવી, "remote.%s.fetch", remotename);

	remote = remote_get(remotename);
	જો (!remote_is_configured(remote, 1)) {
		error(_("No such remote '%s'"), remotename);
		exit(2);
	}

	જો (!add_mode && remove_all_fetch_refspecs(ચાવી.buf)) {
		strbuf_release(&ચાવી);
		પાછા 1;
	}
	add_branches(remote, branches, ચાવી.buf);

	strbuf_release(&ચાવી);
	પાછા 0;
}

સ્થિર પૂર્ણાંક set_branches(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
			સંરચના repository *repo UNUSED)
{
	પૂર્ણાંક add_mode = 0;
	સંરચના option options[] = {
		OPT_BOOL('\0', "add", &add_mode, N_("add branch")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_setbranches_usage, 0);
	જો (argc == 0) {
		error(_("no remote specified"));
		usage_with_options(builtin_remote_setbranches_usage, options);
	}
	argv[argc] = ખાલી;

	પાછા set_remote_branches(argv[0], argv + 1, add_mode);
}

સ્થિર પૂર્ણાંક get_url(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
		   સંરચના repository *repo UNUSED)
{
	પૂર્ણાંક ધકેલો_mode = 0, all_mode = 0;
	અચલ અક્ષર *remotename = ખાલી;
	સંરચના remote *remote;
	સંરચના strvec *url;
	સંરચના option options[] = {
		OPT_BOOL('\0', "push", &ધકેલો_mode,
			 N_("query push URLs rather than fetch URLs")),
		OPT_BOOL('\0', "all", &all_mode,
			 N_("return all URLs")),
		OPT_END()
	};
	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_geturl_usage, 0);

	જો (argc != 1)
		usage_with_options(builtin_remote_geturl_usage, options);

	remotename = argv[0];

	remote = remote_get(remotename);
	જો (!remote_is_configured(remote, 1)) {
		error(_("No such remote '%s'"), remotename);
		exit(2);
	}

	url = ધકેલો_mode ? ધકેલો_url_of_remote(remote) : &remote->url;

	જો (all_mode) {
		માટે (size_t i = 0; i < url->nr; i++)
			printf_ln("%s", url->v[i]);
	} નહીંતર {
		printf_ln("%s", url->v[0]);
	}

	પાછા 0;
}

સ્થિર પૂર્ણાંક set_url(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
		   સંરચના repository *repo UNUSED)
{
	પૂર્ણાંક ધકેલો_mode = 0, add_mode = 0, delete_mode = 0;
	પૂર્ણાંક matches = 0, negative_matches = 0;
	અચલ અક્ષર *remotename = ખાલી;
	અચલ અક્ષર *newurl = ખાલી;
	અચલ અક્ષર *oldurl = ખાલી;
	સંરચના remote *remote;
	regex_t old_regex;
	સંરચના strvec *urlset;
	સંરચના strbuf name_buf = STRBUF_INIT;
	સંરચના option options[] = {
		OPT_BOOL('\0', "push", &ધકેલો_mode,
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

	જો (add_mode && delete_mode)
		die(_("--add --delete doesn't make sense"));

	જો (argc < 3 || argc > 4 || ((add_mode || delete_mode) && argc != 3))
		usage_with_options(builtin_remote_seturl_usage, options);

	remotename = argv[1];
	newurl = argv[2];
	જો (argc > 3)
		oldurl = argv[3];

	જો (delete_mode)
		oldurl = newurl;

	remote = remote_get(remotename);
	જો (!remote_is_configured(remote, 1)) {
		error(_("No such remote '%s'"), remotename);
		exit(2);
	}

	જો (ધકેલો_mode) {
		strbuf_addf(&name_buf, "remote.%s.pushurl", remotename);
		urlset = &remote->pushurl;
	} નહીંતર {
		strbuf_addf(&name_buf, "remote.%s.url", remotename);
		urlset = &remote->url;
	}

	/* Special cases that add new entry. */
	જો ((!oldurl && !delete_mode) || add_mode) {
		જો (add_mode)
			repo_config_set_multivar(the_repository, name_buf.buf, newurl,
						       "^$", 0);
		નહીંતર
			repo_config_set(the_repository, name_buf.buf, newurl);
		જાઓ out;
	}

	/* Old URL specified. Demand that one matches. */
	જો (regcomp(&old_regex, oldurl, REG_EXTENDED))
		die(_("Invalid old URL pattern: %s"), oldurl);

	માટે (size_t i = 0; i < urlset->nr; i++)
		જો (!regexec(&old_regex, urlset->v[i], 0, ખાલી, 0))
			matches++;
		નહીંતર
			negative_matches++;
	જો (!delete_mode && !matches)
		die(_("No such URL found: %s"), oldurl);
	જો (delete_mode && !negative_matches && !ધકેલો_mode)
		die(_("Will not delete all non-push URLs"));

	regfree(&old_regex);

	જો (!delete_mode)
		repo_config_set_multivar(the_repository, name_buf.buf, newurl, oldurl, 0);
	નહીંતર
		repo_config_set_multivar(the_repository, name_buf.buf, ખાલી, oldurl,
					 CONFIG_FLAGS_MULTI_REPLACE);
out:
	strbuf_release(&name_buf);
	પાછા 0;
}

પૂર્ણાંક cmd_remote(પૂર્ણાંક argc,
	       અચલ અક્ષર **argv,
	       અચલ અક્ષર *prefix,
	       સંરચના repository *repo)
{
	parse_opt_subcommand_fn *fn = ખાલી;
	સંરચના option options[] = {
		OPT__VERBOSE(&verbose, N_("be verbose; must be placed before a subcommand")),
		OPT_SUBCOMMAND("add", &fn, add),
		OPT_SUBCOMMAND("rename", &fn, mv),
		OPT_SUBCOMMAND_F("rm", &fn, rm, ખાલી, PARSE_OPT_NOCOMPLETE),
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

	જો (fn) {
		પાછા !!fn(argc, argv, prefix, repo);
	} નહીંતર {
		જો (argc) {
			error(_("unknown subcommand: `%s'"), argv[0]);
			usage_with_options(builtin_remote_usage, options);
		}
		પાછા !!show_all();
	}
}
