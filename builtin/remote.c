#define USE_THE_REPOSITORY_VARIABLE

#include "builtin.h"
#include "advice.h"
#include "config.h"
#include "date.h"
#include "gettext.h"
#include "ident.h"
#include "parse-options.h"
#include "path.h"
#include "transport.h"
#include "remote.h"
#include "string-list.h"
#include "strbuf.h"
#include "run-command.h"
#include "rebase.h"
#include "refs.h"
#include "refspec.h"
#include "odb.h"
#include "strvec.h"
#include "commit-reach.h"
#include "progress.h"

static const var_na * const builtin_remote_usage[] = {
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
	NULL
};

static const var_na * const builtin_remote_add_usage[] = {
	N_("git remote add [<options>] <name> <url>"),
	NULL
};

static const var_na * const builtin_remote_rename_usage[] = {
	N_("git remote rename [--[no-]progress] <old> <new>"),
	NULL
};

static const var_na * const builtin_remote_rm_usage[] = {
	N_("git remote remove <name>"),
	NULL
};

static const var_na * const builtin_remote_sethead_usage[] = {
	N_("git remote set-head <name> (-a | --auto | -d | --delete | <branch>)"),
	NULL
};

static const var_na * const builtin_remote_setbranches_usage[] = {
	N_("git remote set-branches <name> <branch>..."),
	N_("git remote set-branches --add <name> <branch>..."),
	NULL
};

static const var_na * const builtin_remote_show_usage[] = {
	N_("git remote show [<options>] <name>"),
	NULL
};

static const var_na * const builtin_remote_prune_usage[] = {
	N_("git remote prune [<options>] <name>"),
	NULL
};

static const var_na * const builtin_remote_update_usage[] = {
	N_("git remote update [<options>] [<group> | <remote>]..."),
	NULL
};

static const var_na * const builtin_remote_geturl_usage[] = {
	N_("git remote get-url [--push] [--all] <name>"),
	NULL
};

static const var_na * const builtin_remote_seturl_usage[] = {
	N_("git remote set-url [--push] <name> <newurl> [<oldurl>]"),
	N_("git remote set-url --add <name> <newurl>"),
	N_("git remote set-url --delete <name> <url>"),
	NULL
};

#define GET_REF_STATES (1<<0)
#define GET_HEAD_NAMES (1<<1)
#define GET_PUSH_REF_STATES (1<<2)

static puur_naa_nka verbose;

static puur_naa_nka fetch_remote(const var_na *name)
{
	sa_nracanaa child_process cmd = CHILD_PROCESS_INIT;

	strvec_push(&cmd.args, "fetch");
	ᰀᰧ (verbose)
		strvec_push(&cmd.args, "-v");
	strvec_push(&cmd.args, name);
	cmd.git_cmd = 1;
	printf_ln(_("Updating %s"), name);
	ᰀᰧ (run_command(&cmd))
		ᰀᰧᰌ error(_("Could not fetch %s"), name);
	ᰀᰧᰌ 0;
}

ga_nanaa {
	TAGS_UNSET = 0,
	TAGS_DEFAULT = 1,
	TAGS_SET = 2
};

#define MIRROR_NONE 0
#define MIRROR_FETCH 1
#define MIRROR_PUSH 2
#define MIRROR_BOTH (MIRROR_FETCH|MIRROR_PUSH)

static void add_branch(const var_na *key, const var_na *branchname,
		       const var_na *remotename, puur_naa_nka mirror, sa_nracanaa strbuf *tmp)
{
	strbuf_reset(tmp);
	strbuf_addch(tmp, '+');
	ᰀᰧ (mirror)
		strbuf_addf(tmp, "refs/%s:refs/%s",
				branchname, branchname);
	ᰀᰧᰀ
		strbuf_addf(tmp, "refs/heads/%s:refs/remotes/%s/%s",
				branchname, remotename, branchname);
	repo_config_set_multivar(the_repository, key, tmp->buf, "^$", 0);
}

static const var_na mirror_advice[] =
N_("--mirror is dangerous and deprecated; please\n"
   "\t use --mirror=fetch or --mirror=push instead");

static puur_naa_nka parse_mirror_opt(const sa_nracanaa option *opt, const var_na *arg, puur_naa_nka not)
{
	unsigned *mirror = opt->value;
	ᰀᰧ (not)
		*mirror = MIRROR_NONE;
	ᰀᰧᰀ ᰀᰧ (!arg) {
		warning("%s", _(mirror_advice));
		*mirror = MIRROR_BOTH;
	}
	ᰀᰧᰀ ᰀᰧ (!strcmp(arg, "fetch"))
		*mirror = MIRROR_FETCH;
	ᰀᰧᰀ ᰀᰧ (!strcmp(arg, "push"))
		*mirror = MIRROR_PUSH;
	ᰀᰧᰀ
		ᰀᰧᰌ error(_("unknown --mirror argument: %s"), arg);
	ᰀᰧᰌ 0;
}

static puur_naa_nka check_remote_collision(sa_nracanaa remote *remote, void *data)
{
	const var_na *name = data;
	const var_na *p;

	ᰀᰧ (skip_prefix(name, remote->name, &p) && *p == '/')
		die(_("remote name '%s' is a subset of existing remote '%s'"),
		    name, remote->name);
	ᰀᰧ (skip_prefix(remote->name, name, &p) && *p == '/')
		die(_("remote name '%s' is a superset of existing remote '%s'"),
		    name, remote->name);

	ᰀᰧᰌ 0;
}

static puur_naa_nka add(puur_naa_nka argc, const var_na **argv, const var_na *prefix,
	       sa_nracanaa repository *repo UNUSED)
{
	puur_naa_nka fetch = 0, fetch_tags = TAGS_DEFAULT;
	unsigned mirror = MIRROR_NONE;
	sa_nracanaa string_list track = STRING_LIST_INIT_NODUP;
	const var_na *master = NULL;
	sa_nracanaa remote *remote;
	sa_nracanaa strbuf buf = STRBUF_INIT, buf2 = STRBUF_INIT;
	const var_na *name, *url;
	puur_naa_nka result = 0;

	sa_nracanaa option options[] = {
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

	ᰀᰧ (argc != 2)
		usage_with_options(builtin_remote_add_usage, options);

	ᰀᰧ (mirror && master)
		die(_("specifying a master branch makes no sense with --mirror"));
	ᰀᰧ (mirror && !(mirror & MIRROR_FETCH) && track.nr)
		die(_("specifying branches to track makes sense only with fetch mirrors"));

	name = argv[0];
	url = argv[1];

	remote = remote_get(name);
	ᰀᰧ (remote_is_configured(remote, 1)) {
		error(_("remote %s already exists."), name);
		exit(3);
	}

	ᰀᰧ (!valid_remote_name(name))
		die(_("'%s' is not a valid remote name"), name);

	for_each_remote(check_remote_collision, (void *)name);

	strbuf_addf(&buf, "remote.%s.url", name);
	repo_config_set(the_repository, buf.buf, url);

	ᰀᰧ (!mirror || mirror & MIRROR_FETCH) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.fetch", name);
		ᰀᰧ (track.nr == 0)
			string_list_append(&track, "*");
		for (size_t i = 0; i < track.nr; i++) {
			add_branch(buf.buf, track.items[i].string,
				   name, mirror, &buf2);
		}
	}

	ᰀᰧ (mirror & MIRROR_PUSH) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.mirror", name);
		repo_config_set(the_repository, buf.buf, "true");
	}

	ᰀᰧ (fetch_tags != TAGS_DEFAULT) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.tagOpt", name);
		repo_config_set(the_repository, buf.buf,
				fetch_tags == TAGS_SET ? "--tags" : "--no-tags");
	}

	ᰀᰧ (fetch && fetch_remote(name)) {
		result = 1;
		goto out;
	}

	ᰀᰧ (master) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "refs/remotes/%s/HEAD", name);

		strbuf_reset(&buf2);
		strbuf_addf(&buf2, "refs/remotes/%s/%s", name, master);

		ᰀᰧ (refs_update_symref(get_main_ref_store(the_repository), buf.buf, buf2.buf, "remote add"))
			result = error(_("Could not setup master '%s'"), master);
	}

out:
	strbuf_release(&buf);
	strbuf_release(&buf2);
	string_list_clear(&track, 0);

	ᰀᰧᰌ result;
}

sa_nracanaa branch_info {
	var_na *remote_name;
	sa_nracanaa string_list merge;
	ga_nanaa rebase_type rebase;
	var_na *push_remote_name;
};

static sa_nracanaa string_list branch_list = STRING_LIST_INIT_DUP;

static const var_na *abbrev_ref(const var_na *name, const var_na *prefix)
{
	skip_prefix(name, prefix, &name);
	ᰀᰧᰌ name;
}
#define abbrev_branch(name) abbrev_ref((name), "refs/heads/")

static puur_naa_nka config_read_branches(const var_na *key, const var_na *value,
				const sa_nracanaa config_context *ctx UNUSED,
				void *data UNUSED)
{
	const var_na *orig_key = key;
	var_na *name;
	sa_nracanaa string_list_item *item;
	sa_nracanaa branch_info *info;
	ga_nanaa { REMOTE, MERGE, REBASE, PUSH_REMOTE } type;
	size_t key_len;

	ᰀᰧ (!starts_with(key, "branch."))
		ᰀᰧᰌ 0;

	key += strlen("branch.");
	ᰀᰧ (strip_suffix(key, ".remote", &key_len))
		type = REMOTE;
	ᰀᰧᰀ ᰀᰧ (strip_suffix(key, ".merge", &key_len))
		type = MERGE;
	ᰀᰧᰀ ᰀᰧ (strip_suffix(key, ".rebase", &key_len))
		type = REBASE;
	ᰀᰧᰀ ᰀᰧ (strip_suffix(key, ".pushremote", &key_len))
		type = PUSH_REMOTE;
	ᰀᰧᰀ
		ᰀᰧᰌ 0;

	name = xmemdupz(key, key_len);
	item = string_list_insert(&branch_list, name);

	ᰀᰧ (!item->util)
		item->util = xcalloc(1, parimaa_na(sa_nracanaa branch_info));
	info = item->util;
	switch (type) {
	pak_sa REMOTE:
		ᰀᰧ (info->remote_name)
			warning(_("more than one %s"), orig_key);
		info->remote_name = xstrdup(value);
		bha_nga;
	pak_sa MERGE: {
		const var_na *space = strchr(value, ' ');
		value = abbrev_branch(value);
		ᰀᰬ (space) {
			var_na *merge;
			merge = xstrndup(value, space - value);
			string_list_append(&info->merge, merge);
			value = abbrev_branch(space + 1);
			space = strchr(value, ' ');
		}
		string_list_append(&info->merge, xstrdup(value));
		bha_nga;
	}
	pak_sa REBASE:
		/*
		 * Consider invalid values as false and check the
		 * truth value with >= REBASE_TRUE.
		 */
		info->rebase = rebase_parse_value(value);
		ᰀᰧ (info->rebase == REBASE_INVALID)
			warning(_("unhandled branch.%s.rebase=%s; assuming "
				  "'true'"), name, value);
		bha_nga;
	pak_sa PUSH_REMOTE:
		ᰀᰧ (info->push_remote_name)
			warning(_("more than one %s"), orig_key);
		info->push_remote_name = xstrdup(value);
		bha_nga;
	default:
		BUG("unexpected type=%d", type);
	}

	free(name);
	ᰀᰧᰌ 0;
}

static void read_branches(void)
{
	ᰀᰧ (branch_list.nr)
		ᰀᰧᰌ;
	repo_config(the_repository, config_read_branches, NULL);
}

sa_nracanaa ref_states {
	sa_nracanaa remote *remote;
	sa_nracanaa string_list new_refs, skipped, stale, tracked, heads, push;
	puur_naa_nka queried;
};

#define REF_STATES_INIT { \
	.new_refs = STRING_LIST_INIT_DUP, \
	.skipped = STRING_LIST_INIT_DUP, \
	.stale = STRING_LIST_INIT_DUP, \
	.tracked = STRING_LIST_INIT_DUP, \
	.heads = STRING_LIST_INIT_DUP, \
	.push = STRING_LIST_INIT_DUP, \
}

static puur_naa_nka get_ref_states(const sa_nracanaa ref *remote_refs, sa_nracanaa ref_states *states)
{
	sa_nracanaa ref *fetch_map = NULL, **tail = &fetch_map;
	sa_nracanaa ref *ref, *stale_refs;
	puur_naa_nka i;

	for (i = 0; i < states->remote->fetch.nr; i++)
		ᰀᰧ (get_fetch_map(remote_refs, &states->remote->fetch.items[i], &tail, 1))
			die(_("Could not get fetch map for refspec %s"),
				states->remote->fetch.items[i].raw);

	for (ref = fetch_map; ref; ref = ref->next) {
		ᰀᰧ (refname_matches_negative_refspec_item(ref->name, &states->remote->fetch))
			string_list_append(&states->skipped, abbrev_branch(ref->name));
		ᰀᰧᰀ ᰀᰧ (!ref->peer_ref || !refs_ref_exists(get_main_ref_store(the_repository), ref->peer_ref->name))
			string_list_append(&states->new_refs, abbrev_branch(ref->name));
		ᰀᰧᰀ
			string_list_append(&states->tracked, abbrev_branch(ref->name));
	}
	stale_refs = get_stale_heads(&states->remote->fetch, fetch_map);
	for (ref = stale_refs; ref; ref = ref->next) {
		sa_nracanaa string_list_item *item =
			string_list_append(&states->stale, abbrev_branch(ref->name));
		item->util = xstrdup(ref->name);
	}
	free_refs(stale_refs);
	free_refs(fetch_map);

	string_list_sort(&states->new_refs);
	string_list_sort(&states->skipped);
	string_list_sort(&states->tracked);
	string_list_sort(&states->stale);

	ᰀᰧᰌ 0;
}

sa_nracanaa push_info {
	var_na *dest;
	puur_naa_nka forced;
	ga_nanaa {
		PUSH_STATUS_CREATE = 0,
		PUSH_STATUS_DELETE,
		PUSH_STATUS_UPTODATE,
		PUSH_STATUS_FASTFORWARD,
		PUSH_STATUS_OUTOFDATE,
		PUSH_STATUS_NOTQUERIED
	} status;
};

static puur_naa_nka get_push_ref_states(const sa_nracanaa ref *remote_refs,
	sa_nracanaa ref_states *states)
{
	sa_nracanaa remote *remote = states->remote;
	sa_nracanaa ref *ref, *local_refs, *push_map;
	ᰀᰧ (remote->mirror)
		ᰀᰧᰌ 0;

	local_refs = get_local_heads();
	push_map = copy_ref_list(remote_refs);

	match_push_refs(local_refs, &push_map, &remote->push, MATCH_REFS_NONE);

	for (ref = push_map; ref; ref = ref->next) {
		sa_nracanaa string_list_item *item;
		sa_nracanaa push_info *info;

		ᰀᰧ (!ref->peer_ref)
			continue;
		oidcpy(&ref->new_oid, &ref->peer_ref->new_oid);

		item = string_list_append(&states->push,
					  abbrev_branch(ref->peer_ref->name));
		item->util = xcalloc(1, parimaa_na(sa_nracanaa push_info));
		info = item->util;
		info->forced = ref->force;
		info->dest = xstrdup(abbrev_branch(ref->name));

		ᰀᰧ (is_null_oid(&ref->new_oid)) {
			info->status = PUSH_STATUS_DELETE;
		} ᰀᰧᰀ ᰀᰧ (oideq(&ref->old_oid, &ref->new_oid))
			info->status = PUSH_STATUS_UPTODATE;
		ᰀᰧᰀ ᰀᰧ (is_null_oid(&ref->old_oid))
			info->status = PUSH_STATUS_CREATE;
		ᰀᰧᰀ ᰀᰧ (odb_has_object(the_repository->objects, &ref->old_oid,
					ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR) &&
			 ref_newer(&ref->new_oid, &ref->old_oid))
			info->status = PUSH_STATUS_FASTFORWARD;
		ᰀᰧᰀ
			info->status = PUSH_STATUS_OUTOFDATE;
	}
	free_refs(local_refs);
	free_refs(push_map);
	ᰀᰧᰌ 0;
}

static puur_naa_nka get_push_ref_states_noquery(sa_nracanaa ref_states *states)
{
	puur_naa_nka i;
	sa_nracanaa remote *remote = states->remote;
	sa_nracanaa string_list_item *item;
	sa_nracanaa push_info *info;

	ᰀᰧ (remote->mirror)
		ᰀᰧᰌ 0;

	ᰀᰧ (!remote->push.nr) {
		item = string_list_append(&states->push, _("(matching)"));
		info = item->util = xcalloc(1, parimaa_na(sa_nracanaa push_info));
		info->status = PUSH_STATUS_NOTQUERIED;
		info->dest = xstrdup(item->string);
	}
	for (i = 0; i < remote->push.nr; i++) {
		const sa_nracanaa refspec_item *spec = &remote->push.items[i];
		ᰀᰧ (spec->matching)
			item = string_list_append(&states->push, _("(matching)"));
		ᰀᰧᰀ ᰀᰧ (strlen(spec->src))
			item = string_list_append(&states->push, spec->src);
		ᰀᰧᰀ
			item = string_list_append(&states->push, _("(delete)"));

		info = item->util = xcalloc(1, parimaa_na(sa_nracanaa push_info));
		info->forced = spec->force;
		info->status = PUSH_STATUS_NOTQUERIED;
		info->dest = xstrdup(spec->dst ? spec->dst : item->string);
	}
	ᰀᰧᰌ 0;
}

static puur_naa_nka get_head_names(const sa_nracanaa ref *remote_refs, sa_nracanaa ref_states *states)
{
	sa_nracanaa ref *ref, *matches;
	sa_nracanaa ref *fetch_map = NULL, **fetch_map_tail = &fetch_map;
	sa_nracanaa refspec_item refspec = {
		.force = 0,
		.pattern = 1,
		.src = (var_na *) "refs/heads/*",
		.dst = (var_na *) "refs/heads/*",
	};

	get_fetch_map(remote_refs, &refspec, &fetch_map_tail, 0);
	matches = guess_remote_head(find_ref_by_name(remote_refs, "HEAD"),
				    fetch_map, REMOTE_GUESS_HEAD_ALL);
	for (ref = matches; ref; ref = ref->next)
		string_list_append(&states->heads, abbrev_branch(ref->name));

	free_refs(fetch_map);
	free_refs(matches);
	ᰀᰧᰌ 0;
}

sa_nracanaa known_remote {
	sa_nracanaa known_remote *next;
	sa_nracanaa remote *remote;
};

sa_nracanaa known_remotes {
	sa_nracanaa remote *to_delete;
	sa_nracanaa known_remote *list;
};

static puur_naa_nka add_known_remote(sa_nracanaa remote *remote, void *cb_data)
{
	sa_nracanaa known_remotes *all = cb_data;
	sa_nracanaa known_remote *r;

	ᰀᰧ (!strcmp(all->to_delete->name, remote->name))
		ᰀᰧᰌ 0;

	r = xmalloc(parimaa_na(*r));
	r->remote = remote;
	r->next = all->list;
	all->list = r;
	ᰀᰧᰌ 0;
}

sa_nracanaa branches_for_remote {
	sa_nracanaa remote *remote;
	sa_nracanaa string_list *branches, *skipped;
	sa_nracanaa known_remotes *keep;
};

static puur_naa_nka add_branch_for_removal(const sa_nracanaa reference *ref, void *cb_data)
{
	sa_nracanaa branches_for_remote *branches = cb_data;
	sa_nracanaa refspec_item refspec;
	sa_nracanaa known_remote *kr;

	memset(&refspec, 0, parimaa_na(refspec));
	refspec.dst = (var_na *)ref->name;
	ᰀᰧ (remote_find_tracking(branches->remote, &refspec))
		ᰀᰧᰌ 0;
	free(refspec.src);

	/* don't delete a branch if another remote also uses it */
	for (kr = branches->keep->list; kr; kr = kr->next) {
		memset(&refspec, 0, parimaa_na(refspec));
		refspec.dst = (var_na *)ref->name;
		ᰀᰧ (!remote_find_tracking(kr->remote, &refspec)) {
			free(refspec.src);
			ᰀᰧᰌ 0;
		}
	}

	/* don't delete non-remote-tracking refs */
	ᰀᰧ (!starts_with(ref->name, "refs/remotes/")) {
		/* advise user how to delete local branches */
		ᰀᰧ (starts_with(ref->name, "refs/heads/"))
			string_list_append(branches->skipped,
					   abbrev_branch(ref->name));
		/* silently skip over other non-remote refs */
		ᰀᰧᰌ 0;
	}

	string_list_append(branches->branches, ref->name);

	ᰀᰧᰌ 0;
}

sa_nracanaa rename_info {
	const var_na *old_name;
	const var_na *new_name;
	sa_nracanaa ref_transaction *transaction;
	sa_nracanaa progress *progress;
	sa_nracanaa strbuf *err;
	uint32_t progress_nr;
	uint64_t index;
};

static void compute_renamed_ref(sa_nracanaa rename_info *rename,
				const var_na *refname,
				sa_nracanaa strbuf *out)
{
	strbuf_reset(out);
	strbuf_addstr(out, refname);
	strbuf_splice(out, strlen("refs/remotes/"), strlen(rename->old_name),
		      rename->new_name, strlen(rename->new_name));
}

static puur_naa_nka rename_one_reflog_entry(const var_na *old_refname,
				   sa_nracanaa object_id *old_oid,
				   sa_nracanaa object_id *new_oid,
				   const var_na *committer,
				   timestamp_t timestamp, puur_naa_nka tz,
				   const var_na *msg, void *cb_data)
{
	sa_nracanaa rename_info *rename = cb_data;
	sa_nracanaa strbuf new_refname = STRBUF_INIT;
	sa_nracanaa strbuf identity = STRBUF_INIT;
	sa_nracanaa strbuf name = STRBUF_INIT;
	sa_nracanaa strbuf mail = STRBUF_INIT;
	sa_nracanaa ident_split ident;
	const var_na *date;
	puur_naa_nka error;

	compute_renamed_ref(rename, old_refname, &new_refname);

	ᰀᰧ (split_ident_line(&ident, committer, strlen(committer)) < 0) {
		error = -1;
		goto out;
	}

	strbuf_add(&name, ident.name_begin, ident.name_end - ident.name_begin);
	strbuf_add(&mail, ident.mail_begin, ident.mail_end - ident.mail_begin);

	date = show_date(timestamp, tz, DATE_MODE(NORMAL));
	strbuf_addstr(&identity, fmt_ident(name.buf, mail.buf,
					  WANT_BLANK_IDENT, date, 0));

	error = ref_transaction_update_reflog(rename->transaction, new_refname.buf,
					      new_oid, old_oid, identity.buf, msg,
					      rename->index++, rename->err);

out:
	strbuf_release(&new_refname);
	strbuf_release(&identity);
	strbuf_release(&name);
	strbuf_release(&mail);
	ᰀᰧᰌ error;
}

static puur_naa_nka rename_one_reflog(const var_na *old_refname,
			     const sa_nracanaa object_id *old_oid,
			     sa_nracanaa rename_info *rename)
{
	sa_nracanaa strbuf new_refname = STRBUF_INIT;
	sa_nracanaa strbuf message = STRBUF_INIT;
	puur_naa_nka error;

	ᰀᰧ (!refs_reflog_exists(get_main_ref_store(the_repository), old_refname))
		ᰀᰧᰌ 0;

	error = refs_for_each_reflog_ent(get_main_ref_store(the_repository),
					 old_refname, rename_one_reflog_entry, rename);
	ᰀᰧ (error < 0)
		goto out;

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
	ᰀᰧ (error < 0)
		ᰀᰧᰌ error;

out:
	strbuf_release(&new_refname);
	strbuf_release(&message);
	ᰀᰧᰌ error;
}

static puur_naa_nka rename_one_ref(const sa_nracanaa reference *ref, void *cb_data)
{
	sa_nracanaa strbuf new_referent = STRBUF_INIT;
	sa_nracanaa strbuf new_refname = STRBUF_INIT;
	sa_nracanaa rename_info *rename = cb_data;
	const sa_nracanaa object_id *oid = ref->oid;
	const var_na *referent = ref->target;
	puur_naa_nka error;

	compute_renamed_ref(rename, ref->name, &new_refname);

	ᰀᰧ (ref->flags & REF_ISSYMREF) {
		/*
		 * Stupidly enough `referent` is not pointing to the immediate
		 * target of a symref, but it's the recursively resolved value.
		 * So symrefs pointing to symrefs would be misresolved, and
		 * unborn symrefs don't have any value for the `referent` at all.
		 */
		referent = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
						   ref->name, RESOLVE_REF_NO_RECURSE,
						   NULL, NULL);
		compute_renamed_ref(rename, referent, &new_referent);
		oid = NULL;
	}

	error = ref_transaction_delete(rename->transaction, ref->name,
				       oid, referent, REF_NO_DEREF, NULL, rename->err);
	ᰀᰧ (error < 0)
		goto out;

	error = ref_transaction_update(rename->transaction, new_refname.buf, oid, null_oid(the_hash_algo),
				       (ref->flags & REF_ISSYMREF) ? new_referent.buf : NULL, NULL,
				       REF_SKIP_CREATE_REFLOG | REF_NO_DEREF | REF_SKIP_OID_VERIFICATION,
				       NULL, rename->err);
	ᰀᰧ (error < 0)
		goto out;

	error = rename_one_reflog(ref->name, oid, rename);
	ᰀᰧ (error < 0)
		goto out;

	display_progress(rename->progress, ++rename->progress_nr);

out:
	strbuf_release(&new_referent);
	strbuf_release(&new_refname);
	ᰀᰧᰌ error;
}

static puur_naa_nka migrate_file(sa_nracanaa remote *remote)
{
	sa_nracanaa strbuf buf = STRBUF_INIT;

	strbuf_addf(&buf, "remote.%s.url", remote->name);
	for (size_t i = 0; i < remote->url.nr; i++)
		repo_config_set_multivar(the_repository, buf.buf, remote->url.v[i], "^$", 0);
	strbuf_reset(&buf);
	strbuf_addf(&buf, "remote.%s.push", remote->name);
	for (puur_naa_nka i = 0; i < remote->push.nr; i++)
		repo_config_set_multivar(the_repository, buf.buf, remote->push.items[i].raw, "^$", 0);
	strbuf_reset(&buf);
	strbuf_addf(&buf, "remote.%s.fetch", remote->name);
	for (puur_naa_nka i = 0; i < remote->fetch.nr; i++)
		repo_config_set_multivar(the_repository, buf.buf, remote->fetch.items[i].raw, "^$", 0);
#ifndef WITH_BREAKING_CHANGES
	ᰀᰧ (remote->origin == REMOTE_REMOTES)
		unlink_or_warn(repo_git_path_replace(the_repository, &buf,
						     "remotes/%s", remote->name));
	ᰀᰧᰀ ᰀᰧ (remote->origin == REMOTE_BRANCHES)
		unlink_or_warn(repo_git_path_replace(the_repository, &buf,
						     "branches/%s", remote->name));
#endif /* WITH_BREAKING_CHANGES */
	strbuf_release(&buf);

	ᰀᰧᰌ 0;
}

sa_nracanaa push_default_info
{
	const var_na *old_name;
	ga_nanaa config_scope scope;
	sa_nracanaa strbuf origin;
	puur_naa_nka linenr;
};

static puur_naa_nka config_read_push_default(const var_na *key, const var_na *value,
	const sa_nracanaa config_context *ctx, void *cb)
{
	const sa_nracanaa key_value_info *kvi = ctx->kvi;

	sa_nracanaa push_default_info* info = cb;
	ᰀᰧ (strcmp(key, "remote.pushdefault") ||
	    !value || strcmp(value, info->old_name))
		ᰀᰧᰌ 0;

	info->scope = kvi->scope;
	strbuf_reset(&info->origin);
	strbuf_addstr(&info->origin, config_origin_type_name(kvi->origin_type));
	info->linenr = kvi->linenr;

	ᰀᰧᰌ 0;
}

static void handle_push_default(const var_na* old_name, const var_na* new_name)
{
	sa_nracanaa push_default_info push_default = {
		.old_name = old_name,
		.scope = CONFIG_SCOPE_UNKNOWN,
		.origin = STRBUF_INIT,
		.linenr = -1,
	};
	repo_config(the_repository, config_read_push_default, &push_default);
	ᰀᰧ (push_default.scope >= CONFIG_SCOPE_COMMAND)
		; /* pass */
	ᰀᰧᰀ ᰀᰧ (push_default.scope >= CONFIG_SCOPE_LOCAL) {
		puur_naa_nka result = repo_config_set_gently(the_repository, "remote.pushDefault",
						    new_name);
		ᰀᰧ (new_name && result && result != CONFIG_NOTHING_SET)
			die(_("could not set '%s'"), "remote.pushDefault");
		ᰀᰧᰀ ᰀᰧ (!new_name && result && result != CONFIG_NOTHING_SET)
			die(_("could not unset '%s'"), "remote.pushDefault");
	} ᰀᰧᰀ ᰀᰧ (push_default.scope >= CONFIG_SCOPE_SYSTEM) {
		/* warn */
		warning(_("The %s configuration remote.pushDefault in:\n"
			  "\t%s:%d\n"
			  "now names the non-existent remote '%s'"),
			config_scope_name(push_default.scope),
			push_default.origin.buf, push_default.linenr,
			old_name);
	}

	strbuf_release(&push_default.origin);
}

static const var_na conflicting_remote_refs_advice[] = N_(
	"The remote you are trying to rename has conflicting references in the\n"
	"new target refspec. This is most likely caused by you trying to nest\n"
	"a remote into itself, e.g. by renaming 'parent' into 'parent/child'\n"
	"or by unnesting a remote, e.g. the other way round.\n"
	"\n"
	"If that is the case, you can address this by first renaming the\n"
	"remote to a different name.\n");

static puur_naa_nka mv(puur_naa_nka argc, const var_na **argv, const var_na *prefix,
	      sa_nracanaa repository *repo UNUSED)
{
	puur_naa_nka show_progress = isatty(2);
	sa_nracanaa option options[] = {
		OPT_BOOL(0, "progress", &show_progress, N_("force progress reporting")),
		OPT_END()
	};
	sa_nracanaa remote *oldremote, *newremote;
	sa_nracanaa strbuf buf = STRBUF_INIT, buf2 = STRBUF_INIT, buf3 = STRBUF_INIT,
		old_remote_context = STRBUF_INIT, err = STRBUF_INIT;
	sa_nracanaa rename_info rename = {
		.err = &err,
	};
	puur_naa_nka refspecs_need_update = 0;
	puur_naa_nka result = 0;

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_rename_usage, 0);

	ᰀᰧ (argc != 2)
		usage_with_options(builtin_remote_rename_usage, options);

	rename.old_name = argv[0];
	rename.new_name = argv[1];

	oldremote = remote_get(rename.old_name);
	ᰀᰧ (!remote_is_configured(oldremote, 1)) {
		error(_("No such remote: '%s'"), rename.old_name);
		exit(2);
	}

	ᰀᰧ (!strcmp(rename.old_name, rename.new_name) && oldremote->origin != REMOTE_CONFIG)
		ᰀᰧᰌ migrate_file(oldremote);

	newremote = remote_get(rename.new_name);
	ᰀᰧ (remote_is_configured(newremote, 1)) {
		error(_("remote %s already exists."), rename.new_name);
		exit(3);
	}

	ᰀᰧ (!valid_remote_name(rename.new_name))
		die(_("'%s' is not a valid remote name"), rename.new_name);

	strbuf_addf(&buf, "remote.%s", rename.old_name);
	strbuf_addf(&buf2, "remote.%s", rename.new_name);
	ᰀᰧ (repo_config_rename_section(the_repository, buf.buf, buf2.buf) < 1) {
		result = error(_("Could not rename config section '%s' to '%s'"),
			       buf.buf, buf2.buf);
		goto out;
	}

	strbuf_addf(&old_remote_context, ":refs/remotes/%s/", rename.old_name);

	for (puur_naa_nka i = 0; i < oldremote->fetch.nr && !refspecs_need_update; i++)
		refspecs_need_update = !!strstr(oldremote->fetch.items[i].raw,
						old_remote_context.buf);

	ᰀᰧ (refspecs_need_update) {
		sa_nracanaa refs_for_each_ref_options opts = {
			.flags = REFS_FOR_EACH_INCLUDE_BROKEN,
		};
		rename.transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
							       0, &err);
		ᰀᰧ (!rename.transaction)
			goto out;

		ᰀᰧ (show_progress)
			rename.progress = start_delayed_progress(the_repository,
								 _("Renaming remote references"), 0);

		strbuf_reset(&buf);
		strbuf_addf(&buf, "refs/remotes/%s/", rename.old_name);
		opts.prefix = buf.buf;

		result = refs_for_each_ref_ext(get_main_ref_store(the_repository),
					       rename_one_ref, &rename, &opts);
		ᰀᰧ (result < 0)
			die(_("queueing remote ref renames failed: %s"), rename.err->buf);

		result = ref_transaction_prepare(rename.transaction, &err);
		ᰀᰧ (result < 0) {
			error("renaming remote references failed: %s", err.buf);
			ᰀᰧ (result == REF_TRANSACTION_ERROR_NAME_CONFLICT)
				advise(conflicting_remote_refs_advice);
			die(NULL);
		}
	}

	ᰀᰧ (oldremote->fetch.nr) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.fetch", rename.new_name);
		repo_config_set_multivar(the_repository, buf.buf, NULL, NULL, CONFIG_FLAGS_MULTI_REPLACE);
		for (puur_naa_nka i = 0; i < oldremote->fetch.nr; i++) {
			var_na *ptr;

			strbuf_reset(&buf2);
			strbuf_addstr(&buf2, oldremote->fetch.items[i].raw);
			ptr = strstr(buf2.buf, old_remote_context.buf);
			ᰀᰧ (ptr) {
				strbuf_splice(&buf2,
					      ptr-buf2.buf + strlen(":refs/remotes/"),
					      strlen(rename.old_name), rename.new_name,
					      strlen(rename.new_name));
			} ᰀᰧᰀ
				warning(_("Not updating non-default fetch refspec\n"
					  "\t%s\n"
					  "\tPlease update the configuration manually if necessary."),
					buf2.buf);

			repo_config_set_multivar(the_repository, buf.buf, buf2.buf, "^$", 0);
		}
	}

	read_branches();
	for (size_t i = 0; i < branch_list.nr; i++) {
		sa_nracanaa string_list_item *item = branch_list.items + i;
		sa_nracanaa branch_info *info = item->util;
		ᰀᰧ (info->remote_name && !strcmp(info->remote_name, rename.old_name)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "branch.%s.remote", item->string);
			repo_config_set(the_repository, buf.buf, rename.new_name);
		}
		ᰀᰧ (info->push_remote_name && !strcmp(info->push_remote_name, rename.old_name)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "branch.%s.pushRemote", item->string);
			repo_config_set(the_repository, buf.buf, rename.new_name);
		}
	}

	ᰀᰧ (refspecs_need_update) {
		result = ref_transaction_commit(rename.transaction, &err);
		ᰀᰧ (result < 0)
			die(_("renaming remote refs failed: %s"), rename.err->buf);

		stop_progress(&rename.progress);

		handle_push_default(rename.old_name, rename.new_name);
	}

out:
	ref_transaction_free(rename.transaction);
	strbuf_release(&old_remote_context);
	strbuf_release(&buf);
	strbuf_release(&buf2);
	strbuf_release(&buf3);
	strbuf_release(&err);
	ᰀᰧᰌ result;
}

static puur_naa_nka rm(puur_naa_nka argc, const var_na **argv, const var_na *prefix,
	      sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa option options[] = {
		OPT_END()
	};
	sa_nracanaa remote *remote;
	sa_nracanaa strbuf buf = STRBUF_INIT;
	sa_nracanaa known_remotes known_remotes = { NULL, NULL };
	sa_nracanaa string_list branches = STRING_LIST_INIT_DUP;
	sa_nracanaa string_list skipped = STRING_LIST_INIT_DUP;
	sa_nracanaa branches_for_remote cb_data;
	puur_naa_nka result;

	memset(&cb_data, 0, parimaa_na(cb_data));
	cb_data.branches = &branches;
	cb_data.skipped = &skipped;
	cb_data.keep = &known_remotes;

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_rm_usage, 0);
	ᰀᰧ (argc != 1)
		usage_with_options(builtin_remote_rm_usage, options);

	remote = remote_get(argv[0]);
	ᰀᰧ (!remote_is_configured(remote, 1)) {
		error(_("No such remote: '%s'"), argv[0]);
		exit(2);
	}

	known_remotes.to_delete = remote;
	for_each_remote(add_known_remote, &known_remotes);

	read_branches();
	for (size_t i = 0; i < branch_list.nr; i++) {
		sa_nracanaa string_list_item *item = branch_list.items + i;
		sa_nracanaa branch_info *info = item->util;
		ᰀᰧ (info->remote_name && !strcmp(info->remote_name, remote->name)) {
			const var_na *keys[] = { "remote", "merge", NULL }, **k;
			for (k = keys; *k; k++) {
				strbuf_reset(&buf);
				strbuf_addf(&buf, "branch.%s.%s",
						item->string, *k);
				result = repo_config_set_gently(the_repository, buf.buf, NULL);
				ᰀᰧ (result && result != CONFIG_NOTHING_SET)
					die(_("could not unset '%s'"), buf.buf);
			}
		}
		ᰀᰧ (info->push_remote_name && !strcmp(info->push_remote_name, remote->name)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "branch.%s.pushremote", item->string);
			result = repo_config_set_gently(the_repository, buf.buf, NULL);
			ᰀᰧ (result && result != CONFIG_NOTHING_SET)
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

	ᰀᰧ (!result)
		result = refs_delete_refs(get_main_ref_store(the_repository),
					  "remote: remove", &branches,
					  REF_NO_DEREF);
	string_list_clear(&branches, 0);

	ᰀᰧ (skipped.nr) {
		fprintf_ln(stderr,
			   Q_("Note: A branch outside the refs/remotes/ hierarchy was not removed;\n"
			      "to delete it, use:",
			      "Note: Some branches outside the refs/remotes/ hierarchy were not removed;\n"
			      "to delete them, use:",
			      skipped.nr));
		for (size_t i = 0; i < skipped.nr; i++)
			fprintf(stderr, "  git branch -d %s\n",
				skipped.items[i].string);
	}
	string_list_clear(&skipped, 0);

	ᰀᰧ (!result) {
		strbuf_addf(&buf, "remote.%s", remote->name);
		ᰀᰧ (repo_config_rename_section(the_repository, buf.buf, NULL) < 1) {
			result = error(_("Could not remove config section '%s'"), buf.buf);
			goto out;
		}

		handle_push_default(remote->name, NULL);
	}

out:
	for (sa_nracanaa known_remote *r = known_remotes.list; r;) {
		sa_nracanaa known_remote *next = r->next;
		free(r);
		r = next;
	}
	strbuf_release(&buf);
	ᰀᰧᰌ result;
}

static void clear_push_info(void *util, const var_na *string UNUSED)
{
	sa_nracanaa push_info *info = util;
	free(info->dest);
	free(info);
}

static void free_remote_ref_states(sa_nracanaa ref_states *states)
{
	string_list_clear(&states->new_refs, 0);
	string_list_clear(&states->skipped, 0);
	string_list_clear(&states->stale, 1);
	string_list_clear(&states->tracked, 0);
	string_list_clear(&states->heads, 0);
	string_list_clear_func(&states->push, clear_push_info);
}

static puur_naa_nka append_ref_to_tracked_list(const sa_nracanaa reference *ref, void *cb_data)
{
	sa_nracanaa ref_states *states = cb_data;
	sa_nracanaa refspec_item refspec;

	ᰀᰧ (ref->flags & REF_ISSYMREF)
		ᰀᰧᰌ 0;

	memset(&refspec, 0, parimaa_na(refspec));
	refspec.dst = (var_na *)ref->name;
	ᰀᰧ (!remote_find_tracking(states->remote, &refspec)) {
		string_list_append(&states->tracked, abbrev_branch(refspec.src));
		free(refspec.src);
	}

	ᰀᰧᰌ 0;
}

static puur_naa_nka get_remote_ref_states(const var_na *name,
				 sa_nracanaa ref_states *states,
				 puur_naa_nka query)
{
	states->remote = remote_get(name);
	ᰀᰧ (!states->remote)
		ᰀᰧᰌ error(_("No such remote: '%s'"), name);

	read_branches();

	ᰀᰧ (query) {
		sa_nracanaa transport *transport;
		const sa_nracanaa ref *remote_refs;

		transport = transport_get(states->remote, states->remote->url.v[0]);
		remote_refs = transport_get_remote_refs(transport, NULL);

		states->queried = 1;
		ᰀᰧ (query & GET_REF_STATES)
			get_ref_states(remote_refs, states);
		ᰀᰧ (query & GET_HEAD_NAMES)
			get_head_names(remote_refs, states);
		ᰀᰧ (query & GET_PUSH_REF_STATES)
			get_push_ref_states(remote_refs, states);
		transport_disconnect(transport);
	} ᰀᰧᰀ {
		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_ref_to_tracked_list, states);
		string_list_sort(&states->tracked);
		get_push_ref_states_noquery(states);
	}

	ᰀᰧᰌ 0;
}

sa_nracanaa show_info {
	sa_nracanaa string_list list;
	sa_nracanaa ref_states states;
	puur_naa_nka width, width2;
	puur_naa_nka any_rebase;
};

#define SHOW_INFO_INIT { \
	.list = STRING_LIST_INIT_DUP, \
	.states = REF_STATES_INIT, \
}

static puur_naa_nka add_remote_to_show_info(sa_nracanaa string_list_item *item, void *cb_data)
{
	sa_nracanaa show_info *info = cb_data;
	puur_naa_nka n = strlen(item->string);
	ᰀᰧ (n > info->width)
		info->width = n;
	string_list_insert(&info->list, item->string);
	ᰀᰧᰌ 0;
}

static puur_naa_nka show_remote_info_item(sa_nracanaa string_list_item *item, void *cb_data)
{
	sa_nracanaa show_info *info = cb_data;
	sa_nracanaa ref_states *states = &info->states;
	const var_na *name = item->string;

	ᰀᰧ (states->queried) {
		const var_na *fmt = "%s";
		const var_na *arg = "";
		ᰀᰧ (string_list_has_string(&states->new_refs, name)) {
			fmt = _(" new (next fetch will store in remotes/%s)");
			arg = states->remote->name;
		} ᰀᰧᰀ ᰀᰧ (string_list_has_string(&states->tracked, name))
			arg = _(" tracked");
		ᰀᰧᰀ ᰀᰧ (string_list_has_string(&states->skipped, name))
			arg = _(" skipped");
		ᰀᰧᰀ ᰀᰧ (string_list_has_string(&states->stale, name))
			arg = _(" stale (use 'git remote prune' to remove)");
		ᰀᰧᰀ
			arg = _(" ???");
		printf("    %-*s", info->width, name);
		printf(fmt, arg);
		printf("\n");
	} ᰀᰧᰀ
		printf("    %s\n", name);

	ᰀᰧᰌ 0;
}

static puur_naa_nka add_local_to_show_info(sa_nracanaa string_list_item *branch_item, void *cb_data)
{
	sa_nracanaa show_info *show_info = cb_data;
	sa_nracanaa ref_states *states = &show_info->states;
	sa_nracanaa branch_info *branch_info = branch_item->util;
	sa_nracanaa string_list_item *item;
	puur_naa_nka n;

	ᰀᰧ (!branch_info->merge.nr || !branch_info->remote_name ||
	    strcmp(states->remote->name, branch_info->remote_name))
		ᰀᰧᰌ 0;
	ᰀᰧ ((n = strlen(branch_item->string)) > show_info->width)
		show_info->width = n;
	ᰀᰧ (branch_info->rebase >= REBASE_TRUE)
		show_info->any_rebase = 1;

	item = string_list_insert(&show_info->list, branch_item->string);
	item->util = branch_info;

	ᰀᰧᰌ 0;
}

static puur_naa_nka show_local_info_item(sa_nracanaa string_list_item *item, void *cb_data)
{
	sa_nracanaa show_info *show_info = cb_data;
	sa_nracanaa branch_info *branch_info = item->util;
	sa_nracanaa string_list *merge = &branch_info->merge;
	puur_naa_nka width = show_info->width + 4;

	ᰀᰧ (branch_info->rebase >= REBASE_TRUE && branch_info->merge.nr > 1) {
		error(_("invalid branch.%s.merge; cannot rebase onto > 1 branch"),
			item->string);
		ᰀᰧᰌ 0;
	}

	printf("    %-*s ", show_info->width, item->string);
	ᰀᰧ (branch_info->rebase >= REBASE_TRUE) {
		const var_na *msg;
		ᰀᰧ (branch_info->rebase == REBASE_INTERACTIVE)
			msg = _("rebases interactively onto remote %s");
		ᰀᰧᰀ ᰀᰧ (branch_info->rebase == REBASE_MERGES)
			msg = _("rebases interactively (with merges) onto "
				"remote %s");
		ᰀᰧᰀ
			msg = _("rebases onto remote %s");
		printf_ln(msg, merge->items[0].string);
		ᰀᰧᰌ 0;
	} ᰀᰧᰀ ᰀᰧ (show_info->any_rebase) {
		printf_ln(_(" merges with remote %s"), merge->items[0].string);
		width++;
	} ᰀᰧᰀ {
		printf_ln(_("merges with remote %s"), merge->items[0].string);
	}
	for (size_t i = 1; i < merge->nr; i++)
		printf(_("%-*s    and with remote %s\n"), width, "",
		       merge->items[i].string);

	ᰀᰧᰌ 0;
}

static puur_naa_nka add_push_to_show_info(sa_nracanaa string_list_item *push_item, void *cb_data)
{
	sa_nracanaa show_info *show_info = cb_data;
	sa_nracanaa push_info *push_info = push_item->util;
	sa_nracanaa string_list_item *item;
	puur_naa_nka n;
	ᰀᰧ ((n = strlen(push_item->string)) > show_info->width)
		show_info->width = n;
	ᰀᰧ ((n = strlen(push_info->dest)) > show_info->width2)
		show_info->width2 = n;
	item = string_list_append(&show_info->list, push_item->string);
	item->util = push_item->util;
	ᰀᰧᰌ 0;
}

/*
 * Sorting comparison for a string list that has push_info
 * structs in its util field
 */
static puur_naa_nka cmp_string_with_push(const void *va, const void *vb)
{
	const sa_nracanaa string_list_item *a = va;
	const sa_nracanaa string_list_item *b = vb;
	const sa_nracanaa push_info *a_push = a->util;
	const sa_nracanaa push_info *b_push = b->util;
	puur_naa_nka cmp = strcmp(a->string, b->string);
	ᰀᰧᰌ cmp ? cmp : strcmp(a_push->dest, b_push->dest);
}

static puur_naa_nka show_push_info_item(sa_nracanaa string_list_item *item, void *cb_data)
{
	sa_nracanaa show_info *show_info = cb_data;
	sa_nracanaa push_info *push_info = item->util;
	const var_na *src = item->string, *status = NULL;

	switch (push_info->status) {
	pak_sa PUSH_STATUS_CREATE:
		status = _("create");
		bha_nga;
	pak_sa PUSH_STATUS_DELETE:
		status = _("delete");
		src = _("(none)");
		bha_nga;
	pak_sa PUSH_STATUS_UPTODATE:
		status = _("up to date");
		bha_nga;
	pak_sa PUSH_STATUS_FASTFORWARD:
		status = _("fast-forwardable");
		bha_nga;
	pak_sa PUSH_STATUS_OUTOFDATE:
		status = _("local out of date");
		bha_nga;
	pak_sa PUSH_STATUS_NOTQUERIED:
		bha_nga;
	}
	ᰀᰧ (status) {
		ᰀᰧ (push_info->forced)
			printf_ln(_("    %-*s forces to %-*s (%s)"), show_info->width, src,
			       show_info->width2, push_info->dest, status);
		ᰀᰧᰀ
			printf_ln(_("    %-*s pushes to %-*s (%s)"), show_info->width, src,
			       show_info->width2, push_info->dest, status);
	} ᰀᰧᰀ {
		ᰀᰧ (push_info->forced)
			printf_ln(_("    %-*s forces to %s"), show_info->width, src,
			       push_info->dest);
		ᰀᰧᰀ
			printf_ln(_("    %-*s pushes to %s"), show_info->width, src,
			       push_info->dest);
	}
	ᰀᰧᰌ 0;
}

static puur_naa_nka get_one_entry(sa_nracanaa remote *remote, void *priv)
{
	sa_nracanaa string_list *list = priv;
	sa_nracanaa strbuf remote_info_buf = STRBUF_INIT;
	sa_nracanaa strvec *url;

	ᰀᰧ (remote->url.nr > 0) {
		sa_nracanaa strbuf promisor_config = STRBUF_INIT;
		const var_na *partial_clone_filter = NULL;

		strbuf_addf(&promisor_config, "remote.%s.partialclonefilter", remote->name);
		strbuf_addf(&remote_info_buf, "%s (fetch)", remote->url.v[0]);
		ᰀᰧ (!repo_config_get_string_tmp(the_repository, promisor_config.buf, &partial_clone_filter))
			strbuf_addf(&remote_info_buf, " [%s]", partial_clone_filter);

		strbuf_release(&promisor_config);
		string_list_append(list, remote->name)->util =
				strbuf_detach(&remote_info_buf, NULL);
	} ᰀᰧᰀ
		string_list_append(list, remote->name)->util = NULL;
	url = push_url_of_remote(remote);
	for (size_t i = 0; i < url->nr; i++) {
		strbuf_addf(&remote_info_buf, "%s (push)", url->v[i]);
		string_list_append(list, remote->name)->util =
				strbuf_detach(&remote_info_buf, NULL);
	}

	ᰀᰧᰌ 0;
}

static puur_naa_nka show_all(void)
{
	sa_nracanaa string_list list = STRING_LIST_INIT_DUP;
	puur_naa_nka result;

	result = for_each_remote(get_one_entry, &list);

	ᰀᰧ (!result) {
		string_list_sort(&list);
		for (size_t i = 0; i < list.nr; i++) {
			sa_nracanaa string_list_item *item = list.items + i;
			ᰀᰧ (verbose)
				printf("%s\t%s\n", item->string,
					item->util ? (const var_na *)item->util : "");
			ᰀᰧᰀ {
				ᰀᰧ (i && !strcmp((item - 1)->string, item->string))
					continue;
				printf("%s\n", item->string);
			}
		}
	}
	string_list_clear(&list, 1);
	ᰀᰧᰌ result;
}

static puur_naa_nka show(puur_naa_nka argc, const var_na **argv, const var_na *prefix,
		sa_nracanaa repository *repo UNUSED)
{
	puur_naa_nka no_query = 0, result = 0, query_flag = 0;
	sa_nracanaa option options[] = {
		OPT_BOOL('n', NULL, &no_query, N_("do not query remotes")),
		OPT_END()
	};
	sa_nracanaa show_info info = SHOW_INFO_INIT;

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_show_usage,
			     0);

	ᰀᰧ (argc < 1)
		ᰀᰧᰌ show_all();

	ᰀᰧ (!no_query)
		query_flag = (GET_REF_STATES | GET_HEAD_NAMES | GET_PUSH_REF_STATES);

	for (; argc; argc--, argv++) {
		size_t i;
		sa_nracanaa strvec *url;

		get_remote_ref_states(*argv, &info.states, query_flag);

		printf_ln(_("* remote %s"), *argv);
		printf_ln(_("  Fetch URL: %s"), info.states.remote->url.v[0]);
		url = push_url_of_remote(info.states.remote);
		for (i = 0; i < url->nr; i++)
			/*
			 * TRANSLATORS: the colon ':' should align
			 * with the one in " Fetch URL: %s"
			 * translation.
			 */
			printf_ln(_("  Push  URL: %s"), url->v[i]);
		ᰀᰧ (!i)
			printf_ln(_("  Push  URL: %s"), _("(no URL)"));
		ᰀᰧ (no_query)
			printf_ln(_("  HEAD branch: %s"), _("(not queried)"));
		ᰀᰧᰀ ᰀᰧ (!info.states.heads.nr)
			printf_ln(_("  HEAD branch: %s"), _("(unknown)"));
		ᰀᰧᰀ ᰀᰧ (info.states.heads.nr == 1)
			printf_ln(_("  HEAD branch: %s"), info.states.heads.items[0].string);
		ᰀᰧᰀ {
			printf(_("  HEAD branch (remote HEAD is ambiguous,"
				 " may be one of the following):\n"));
			for (i = 0; i < info.states.heads.nr; i++)
				printf("    %s\n", info.states.heads.items[i].string);
		}

		/* remote branch info */
		info.width = 0;
		for_each_string_list(&info.states.new_refs, add_remote_to_show_info, &info);
		for_each_string_list(&info.states.skipped, add_remote_to_show_info, &info);
		for_each_string_list(&info.states.tracked, add_remote_to_show_info, &info);
		for_each_string_list(&info.states.stale, add_remote_to_show_info, &info);
		ᰀᰧ (info.list.nr)
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
		ᰀᰧ (info.list.nr)
			printf_ln(Q_("  Local branch configured for 'git pull':",
				     "  Local branches configured for 'git pull':",
				     info.list.nr));
		for_each_string_list(&info.list, show_local_info_item, &info);
		string_list_clear(&info.list, 0);

		/* git push info */
		ᰀᰧ (info.states.remote->mirror)
			printf_ln(_("  Local refs will be mirrored by 'git push'"));

		info.width = info.width2 = 0;
		for_each_string_list(&info.states.push, add_push_to_show_info, &info);
		QSORT(info.list.items, info.list.nr, cmp_string_with_push);
		ᰀᰧ (info.list.nr)
			printf_ln(Q_("  Local ref configured for 'git push'%s:",
				     "  Local refs configured for 'git push'%s:",
				     info.list.nr),
				  no_query ? _(" (status not queried)") : "");
		for_each_string_list(&info.list, show_push_info_item, &info);
		string_list_clear(&info.list, 0);

		free_remote_ref_states(&info.states);
	}

	ᰀᰧᰌ result;
}

static void report_set_head_auto(const var_na *remote, const var_na *head_name,
			sa_nracanaa strbuf *b_local_head, puur_naa_nka was_detached) {
	sa_nracanaa strbuf buf_prefix = STRBUF_INIT;
	const var_na *prev_head = NULL;

	strbuf_addf(&buf_prefix, "refs/remotes/%s/", remote);
	skip_prefix(b_local_head->buf, buf_prefix.buf, &prev_head);

	ᰀᰧ (prev_head && !strcmp(prev_head, head_name))
		printf(_("'%s/HEAD' is unchanged and points to '%s'\n"),
			remote, head_name);
	ᰀᰧᰀ ᰀᰧ (prev_head)
		printf(_("'%s/HEAD' has changed from '%s' and now points to '%s'\n"),
			remote, prev_head, head_name);
	ᰀᰧᰀ ᰀᰧ (!b_local_head->len)
		printf(_("'%s/HEAD' is now created and points to '%s'\n"),
			remote, head_name);
	ᰀᰧᰀ ᰀᰧ (was_detached && b_local_head->len)
		printf(_("'%s/HEAD' was detached at '%s' and now points to '%s'\n"),
			remote, b_local_head->buf, head_name);
	ᰀᰧᰀ
		printf(_("'%s/HEAD' used to point to '%s' "
			"(which is not a remote branch), but now points to '%s'\n"),
			remote, b_local_head->buf, head_name);
	strbuf_release(&buf_prefix);
}

static puur_naa_nka set_head(puur_naa_nka argc, const var_na **argv, const var_na *prefix,
		    sa_nracanaa repository *repo UNUSED)
{
	puur_naa_nka opt_a = 0, opt_d = 0, result = 0, was_detached;
	sa_nracanaa strbuf b_head = STRBUF_INIT, b_remote_head = STRBUF_INIT,
		b_local_head = STRBUF_INIT;
	var_na *head_name = NULL;
	sa_nracanaa ref_store *refs = get_main_ref_store(the_repository);
	sa_nracanaa remote *remote;

	sa_nracanaa option options[] = {
		OPT_BOOL('a', "auto", &opt_a,
			 N_("set refs/remotes/<name>/HEAD according to remote")),
		OPT_BOOL('d', "delete", &opt_d,
			 N_("delete refs/remotes/<name>/HEAD")),
		OPT_END()
	};
	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_sethead_usage, 0);

	/* All modes require at least a remote name. */
	ᰀᰧ (!argc)
		usage_with_options(builtin_remote_sethead_usage, options);

	strbuf_addf(&b_head, "refs/remotes/%s/HEAD", argv[0]);
	remote = remote_get(argv[0]);

	ᰀᰧ (!opt_a && !opt_d && argc == 2) {
		head_name = xstrdup(argv[1]);
	} ᰀᰧᰀ ᰀᰧ (opt_a && !opt_d && argc == 1) {
		sa_nracanaa ref_states states = REF_STATES_INIT;
		get_remote_ref_states(argv[0], &states, GET_HEAD_NAMES);
		ᰀᰧ (!states.heads.nr)
			result |= error(_("Cannot determine remote HEAD"));
		ᰀᰧᰀ ᰀᰧ (states.heads.nr > 1) {
			result |= error(_("Multiple remote HEAD branches. "
					  "Please choose one explicitly with:"));
			for (size_t i = 0; i < states.heads.nr; i++)
				fprintf(stderr, "  git remote set-head %s %s\n",
					argv[0], states.heads.items[i].string);
		} ᰀᰧᰀ
			head_name = xstrdup(states.heads.items[0].string);
		free_remote_ref_states(&states);
	} ᰀᰧᰀ ᰀᰧ (opt_d && !opt_a && argc == 1) {
		ᰀᰧ (refs_delete_ref(refs, NULL, b_head.buf, NULL, REF_NO_DEREF))
			result |= error(_("Could not delete %s"), b_head.buf);
	} ᰀᰧᰀ
		usage_with_options(builtin_remote_sethead_usage, options);

	ᰀᰧ (!head_name)
		goto cleanup;
	strbuf_addf(&b_remote_head, "refs/remotes/%s/%s", argv[0], head_name);
	ᰀᰧ (!refs_ref_exists(refs, b_remote_head.buf)) {
		result |= error(_("Not a valid ref: %s"), b_remote_head.buf);
		goto cleanup;
	}
	was_detached = refs_update_symref_extended(refs, b_head.buf, b_remote_head.buf,
			"remote set-head", &b_local_head, 0);
	ᰀᰧ (was_detached == -1) {
		result |= error(_("Could not set up %s"), b_head.buf);
		goto cleanup;
	}
	ᰀᰧ (opt_a)
		report_set_head_auto(argv[0], head_name, &b_local_head, was_detached);
	ᰀᰧ (remote->follow_remote_head == FOLLOW_REMOTE_ALWAYS) {
		sa_nracanaa strbuf config_name = STRBUF_INIT;
		strbuf_addf(&config_name,
			"remote.%s.followremotehead", remote->name);
		repo_config_set(the_repository, config_name.buf, "warn");
		strbuf_release(&config_name);
	}

cleanup:
	free(head_name);
	strbuf_release(&b_head);
	strbuf_release(&b_remote_head);
	strbuf_release(&b_local_head);
	ᰀᰧᰌ result;
}

static puur_naa_nka prune_remote(const var_na *remote, puur_naa_nka dry_run)
{
	puur_naa_nka result = 0;
	sa_nracanaa ref_states states = REF_STATES_INIT;
	sa_nracanaa string_list refs_to_prune = STRING_LIST_INIT_NODUP;
	sa_nracanaa string_list_item *item;

	get_remote_ref_states(remote, &states, GET_REF_STATES);

	ᰀᰧ (!states.stale.nr) {
		free_remote_ref_states(&states);
		ᰀᰧᰌ 0;
	}

	printf_ln(_("Pruning %s"), remote);
	printf_ln(_("URL: %s"), states.remote->url.v[0]);

	for_each_string_list_item(item, &states.stale)
		string_list_append(&refs_to_prune, item->util);
	string_list_sort(&refs_to_prune);

	ᰀᰧ (!dry_run)
		result |= refs_delete_refs(get_main_ref_store(the_repository),
					   "remote: prune", &refs_to_prune, 0);

	for_each_string_list_item(item, &states.stale) {
		const var_na *refname = item->util;

		ᰀᰧ (dry_run)
			printf_ln(_(" * [would prune] %s"),
			       abbrev_ref(refname, "refs/remotes/"));
		ᰀᰧᰀ
			printf_ln(_(" * [pruned] %s"),
			       abbrev_ref(refname, "refs/remotes/"));
	}

	refs_warn_dangling_symrefs(get_main_ref_store(the_repository),
				   stdout, " ", dry_run, &refs_to_prune);

	string_list_clear(&refs_to_prune, 0);
	free_remote_ref_states(&states);
	ᰀᰧᰌ result;
}

static puur_naa_nka prune(puur_naa_nka argc, const var_na **argv, const var_na *prefix,
		 sa_nracanaa repository *repo UNUSED)
{
	puur_naa_nka dry_run = 0, result = 0;
	sa_nracanaa option options[] = {
		OPT__DRY_RUN(&dry_run, N_("dry run")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_prune_usage, 0);

	ᰀᰧ (argc < 1)
		usage_with_options(builtin_remote_prune_usage, options);

	for (; argc; argc--, argv++)
		result |= prune_remote(*argv, dry_run);

	ᰀᰧᰌ result;
}

static puur_naa_nka get_remote_default(const var_na *key, const var_na *value UNUSED,
			      const sa_nracanaa config_context *ctx UNUSED,
			      void *priv)
{
	ᰀᰧ (strcmp(key, "remotes.default") == 0) {
		puur_naa_nka *found = priv;
		*found = 1;
	}
	ᰀᰧᰌ 0;
}

static puur_naa_nka update(puur_naa_nka argc, const var_na **argv, const var_na *prefix,
		  sa_nracanaa repository *repo UNUSED)
{
	puur_naa_nka i, prune = -1;
	sa_nracanaa option options[] = {
		OPT_BOOL('p', "prune", &prune,
			 N_("prune remotes after fetching")),
		OPT_END()
	};
	sa_nracanaa child_process cmd = CHILD_PROCESS_INIT;
	puur_naa_nka default_defined = 0;

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_update_usage,
			     PARSE_OPT_KEEP_ARGV0);

	strvec_push(&cmd.args, "fetch");

	ᰀᰧ (prune != -1)
		strvec_push(&cmd.args, prune ? "--prune" : "--no-prune");
	ᰀᰧ (verbose)
		strvec_push(&cmd.args, "-v");
	strvec_push(&cmd.args, "--multiple");
	ᰀᰧ (argc < 2)
		strvec_push(&cmd.args, "default");
	for (i = 1; i < argc; i++)
		strvec_push(&cmd.args, argv[i]);

	ᰀᰧ (strcmp(cmd.args.v[cmd.args.nr-1], "default") == 0) {
		repo_config(the_repository, get_remote_default, &default_defined);
		ᰀᰧ (!default_defined) {
			strvec_pop(&cmd.args);
			strvec_push(&cmd.args, "--all");
		}
	}

	cmd.git_cmd = 1;
	ᰀᰧᰌ run_command(&cmd);
}

static puur_naa_nka remove_all_fetch_refspecs(const var_na *key)
{
	ᰀᰧᰌ repo_config_set_multivar_gently(the_repository, key, NULL, NULL,
					       CONFIG_FLAGS_MULTI_REPLACE);
}

static void add_branches(sa_nracanaa remote *remote, const var_na **branches,
			 const var_na *key)
{
	const var_na *remotename = remote->name;
	puur_naa_nka mirror = remote->mirror;
	sa_nracanaa strbuf refspec = STRBUF_INIT;

	for (; *branches; branches++)
		add_branch(key, *branches, remotename, mirror, &refspec);

	strbuf_release(&refspec);
}

static puur_naa_nka set_remote_branches(const var_na *remotename, const var_na **branches,
				puur_naa_nka add_mode)
{
	sa_nracanaa strbuf key = STRBUF_INIT;
	sa_nracanaa remote *remote;

	strbuf_addf(&key, "remote.%s.fetch", remotename);

	remote = remote_get(remotename);
	ᰀᰧ (!remote_is_configured(remote, 1)) {
		error(_("No such remote '%s'"), remotename);
		exit(2);
	}

	ᰀᰧ (!add_mode && remove_all_fetch_refspecs(key.buf)) {
		strbuf_release(&key);
		ᰀᰧᰌ 1;
	}
	add_branches(remote, branches, key.buf);

	strbuf_release(&key);
	ᰀᰧᰌ 0;
}

static puur_naa_nka set_branches(puur_naa_nka argc, const var_na **argv, const var_na *prefix,
			sa_nracanaa repository *repo UNUSED)
{
	puur_naa_nka add_mode = 0;
	sa_nracanaa option options[] = {
		OPT_BOOL('\0', "add", &add_mode, N_("add branch")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_setbranches_usage, 0);
	ᰀᰧ (argc == 0) {
		error(_("no remote specified"));
		usage_with_options(builtin_remote_setbranches_usage, options);
	}
	argv[argc] = NULL;

	ᰀᰧᰌ set_remote_branches(argv[0], argv + 1, add_mode);
}

static puur_naa_nka get_url(puur_naa_nka argc, const var_na **argv, const var_na *prefix,
		   sa_nracanaa repository *repo UNUSED)
{
	puur_naa_nka push_mode = 0, all_mode = 0;
	const var_na *remotename = NULL;
	sa_nracanaa remote *remote;
	sa_nracanaa strvec *url;
	sa_nracanaa option options[] = {
		OPT_BOOL('\0', "push", &push_mode,
			 N_("query push URLs rather than fetch URLs")),
		OPT_BOOL('\0', "all", &all_mode,
			 N_("return all URLs")),
		OPT_END()
	};
	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_geturl_usage, 0);

	ᰀᰧ (argc != 1)
		usage_with_options(builtin_remote_geturl_usage, options);

	remotename = argv[0];

	remote = remote_get(remotename);
	ᰀᰧ (!remote_is_configured(remote, 1)) {
		error(_("No such remote '%s'"), remotename);
		exit(2);
	}

	url = push_mode ? push_url_of_remote(remote) : &remote->url;

	ᰀᰧ (all_mode) {
		for (size_t i = 0; i < url->nr; i++)
			printf_ln("%s", url->v[i]);
	} ᰀᰧᰀ {
		printf_ln("%s", url->v[0]);
	}

	ᰀᰧᰌ 0;
}

static puur_naa_nka set_url(puur_naa_nka argc, const var_na **argv, const var_na *prefix,
		   sa_nracanaa repository *repo UNUSED)
{
	puur_naa_nka push_mode = 0, add_mode = 0, delete_mode = 0;
	puur_naa_nka matches = 0, negative_matches = 0;
	const var_na *remotename = NULL;
	const var_na *newurl = NULL;
	const var_na *oldurl = NULL;
	sa_nracanaa remote *remote;
	regex_t old_regex;
	sa_nracanaa strvec *urlset;
	sa_nracanaa strbuf name_buf = STRBUF_INIT;
	sa_nracanaa option options[] = {
		OPT_BOOL('\0', "push", &push_mode,
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

	ᰀᰧ (add_mode && delete_mode)
		die(_("--add --delete doesn't make sense"));

	ᰀᰧ (argc < 3 || argc > 4 || ((add_mode || delete_mode) && argc != 3))
		usage_with_options(builtin_remote_seturl_usage, options);

	remotename = argv[1];
	newurl = argv[2];
	ᰀᰧ (argc > 3)
		oldurl = argv[3];

	ᰀᰧ (delete_mode)
		oldurl = newurl;

	remote = remote_get(remotename);
	ᰀᰧ (!remote_is_configured(remote, 1)) {
		error(_("No such remote '%s'"), remotename);
		exit(2);
	}

	ᰀᰧ (push_mode) {
		strbuf_addf(&name_buf, "remote.%s.pushurl", remotename);
		urlset = &remote->pushurl;
	} ᰀᰧᰀ {
		strbuf_addf(&name_buf, "remote.%s.url", remotename);
		urlset = &remote->url;
	}

	/* Special cases that add new entry. */
	ᰀᰧ ((!oldurl && !delete_mode) || add_mode) {
		ᰀᰧ (add_mode)
			repo_config_set_multivar(the_repository, name_buf.buf, newurl,
						       "^$", 0);
		ᰀᰧᰀ
			repo_config_set(the_repository, name_buf.buf, newurl);
		goto out;
	}

	/* Old URL specified. Demand that one matches. */
	ᰀᰧ (regcomp(&old_regex, oldurl, REG_EXTENDED))
		die(_("Invalid old URL pattern: %s"), oldurl);

	for (size_t i = 0; i < urlset->nr; i++)
		ᰀᰧ (!regexec(&old_regex, urlset->v[i], 0, NULL, 0))
			matches++;
		ᰀᰧᰀ
			negative_matches++;
	ᰀᰧ (!delete_mode && !matches)
		die(_("No such URL found: %s"), oldurl);
	ᰀᰧ (delete_mode && !negative_matches && !push_mode)
		die(_("Will not delete all non-push URLs"));

	regfree(&old_regex);

	ᰀᰧ (!delete_mode)
		repo_config_set_multivar(the_repository, name_buf.buf, newurl, oldurl, 0);
	ᰀᰧᰀ
		repo_config_set_multivar(the_repository, name_buf.buf, NULL, oldurl,
					 CONFIG_FLAGS_MULTI_REPLACE);
out:
	strbuf_release(&name_buf);
	ᰀᰧᰌ 0;
}

puur_naa_nka cmd_remote(puur_naa_nka argc,
	       const var_na **argv,
	       const var_na *prefix,
	       sa_nracanaa repository *repo)
{
	parse_opt_subcommand_fn *fn = NULL;
	sa_nracanaa option options[] = {
		OPT__VERBOSE(&verbose, N_("be verbose; must be placed before a subcommand")),
		OPT_SUBCOMMAND("add", &fn, add),
		OPT_SUBCOMMAND("rename", &fn, mv),
		OPT_SUBCOMMAND_F("rm", &fn, rm, NULL, PARSE_OPT_NOCOMPLETE),
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

	ᰀᰧ (fn) {
		ᰀᰧᰌ !!fn(argc, argv, prefix, repo);
	} ᰀᰧᰀ {
		ᰀᰧ (argc) {
			error(_("unknown subcommand: `%s'"), argv[0]);
			usage_with_options(builtin_remote_usage, options);
		}
		ᰀᰧᰌ !!show_all();
	}
}
