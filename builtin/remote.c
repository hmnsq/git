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

static const char * const builtin_remote_usage[] = {
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

static const char * const builtin_remote_add_usage[] = {
	N_("git remote add [<options>] <name> <url>"),
	NULL
};

static const char * const builtin_remote_rename_usage[] = {
	N_("git remote rename [--[no-]progress] <old> <new>"),
	NULL
};

static const char * const builtin_remote_rm_usage[] = {
	N_("git remote remove <name>"),
	NULL
};

static const char * const builtin_remote_sethead_usage[] = {
	N_("git remote set-head <name> (-a | --auto | -d | --delete | <branch>)"),
	NULL
};

static const char * const builtin_remote_setbranches_usage[] = {
	N_("git remote set-branches <name> <branch>..."),
	N_("git remote set-branches --add <name> <branch>..."),
	NULL
};

static const char * const builtin_remote_show_usage[] = {
	N_("git remote show [<options>] <name>"),
	NULL
};

static const char * const builtin_remote_prune_usage[] = {
	N_("git remote prune [<options>] <name>"),
	NULL
};

static const char * const builtin_remote_update_usage[] = {
	N_("git remote update [<options>] [<group> | <remote>]..."),
	NULL
};

static const char * const builtin_remote_geturl_usage[] = {
	N_("git remote get-url [--push] [--all] <name>"),
	NULL
};

static const char * const builtin_remote_seturl_usage[] = {
	N_("git remote set-url [--push] <name> <newurl> [<oldurl>]"),
	N_("git remote set-url --add <name> <newurl>"),
	N_("git remote set-url --delete <name> <url>"),
	NULL
};

#define GET_REF_STATES (1<<0)
#define GET_HEAD_NAMES (1<<1)
#define GET_PUSH_REF_STATES (1<<2)

static ціле verbose;

static ціле fetch_remote(const char *name)
{
	struct child_process cmd = CHILD_PROCESS_INIT;

	strvec_push(&cmd.args, "fetch");
	якщо (verbose)
		strvec_push(&cmd.args, "-v");
	strvec_push(&cmd.args, name);
	cmd.git_cmd = 1;
	printf_ln(_("Updating %s"), name);
	якщо (run_command(&cmd))
		повернути error(_("Could not fetch %s"), name);
	повернути 0;
}

enum {
	TAGS_UNSET = 0,
	TAGS_DEFAULT = 1,
	TAGS_SET = 2
};

#define MIRROR_NONE 0
#define MIRROR_FETCH 1
#define MIRROR_PUSH 2
#define MIRROR_BOTH (MIRROR_FETCH|MIRROR_PUSH)

static void add_branch(const char *key, const char *branchname,
		       const char *remotename, ціле mirror, struct strbuf *tmp)
{
	strbuf_reset(tmp);
	strbuf_addch(tmp, '+');
	якщо (mirror)
		strbuf_addf(tmp, "refs/%s:refs/%s",
				branchname, branchname);
	інакше
		strbuf_addf(tmp, "refs/heads/%s:refs/remotes/%s/%s",
				branchname, remotename, branchname);
	repo_config_set_multivar(the_repository, key, tmp->buf, "^$", 0);
}

static const char mirror_advice[] =
N_("--mirror is dangerous and deprecated; please\n"
   "\t use --mirror=fetch or --mirror=push instead");

static ціле parse_mirror_opt(const struct option *opt, const char *arg, ціле not)
{
	unsigned *mirror = opt->value;
	якщо (not)
		*mirror = MIRROR_NONE;
	інакше якщо (!arg) {
		warning("%s", _(mirror_advice));
		*mirror = MIRROR_BOTH;
	}
	інакше якщо (!strcmp(arg, "fetch"))
		*mirror = MIRROR_FETCH;
	інакше якщо (!strcmp(arg, "push"))
		*mirror = MIRROR_PUSH;
	інакше
		повернути error(_("unknown --mirror argument: %s"), arg);
	повернути 0;
}

static ціле check_remote_collision(struct remote *remote, void *data)
{
	const char *name = data;
	const char *p;

	якщо (skip_prefix(name, remote->name, &p) && *p == '/')
		die(_("remote name '%s' is a subset of existing remote '%s'"),
		    name, remote->name);
	якщо (skip_prefix(remote->name, name, &p) && *p == '/')
		die(_("remote name '%s' is a superset of existing remote '%s'"),
		    name, remote->name);

	повернути 0;
}

static ціле add(ціле argc, const char **argv, const char *prefix,
	       struct repository *repo UNUSED)
{
	ціле fetch = 0, fetch_tags = TAGS_DEFAULT;
	unsigned mirror = MIRROR_NONE;
	struct string_list track = STRING_LIST_INIT_NODUP;
	const char *master = NULL;
	struct remote *remote;
	struct strbuf buf = STRBUF_INIT, buf2 = STRBUF_INIT;
	const char *name, *url;
	ціле result = 0;

	struct option options[] = {
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

	якщо (argc != 2)
		usage_with_options(builtin_remote_add_usage, options);

	якщо (mirror && master)
		die(_("specifying a master branch makes no sense with --mirror"));
	якщо (mirror && !(mirror & MIRROR_FETCH) && track.nr)
		die(_("specifying branches to track makes sense only with fetch mirrors"));

	name = argv[0];
	url = argv[1];

	remote = remote_get(name);
	якщо (remote_is_configured(remote, 1)) {
		error(_("remote %s already exists."), name);
		exit(3);
	}

	якщо (!valid_remote_name(name))
		die(_("'%s' is not a valid remote name"), name);

	for_each_remote(check_remote_collision, (void *)name);

	strbuf_addf(&buf, "remote.%s.url", name);
	repo_config_set(the_repository, buf.buf, url);

	якщо (!mirror || mirror & MIRROR_FETCH) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.fetch", name);
		якщо (track.nr == 0)
			string_list_append(&track, "*");
		для (size_t i = 0; i < track.nr; i++) {
			add_branch(buf.buf, track.items[i].string,
				   name, mirror, &buf2);
		}
	}

	якщо (mirror & MIRROR_PUSH) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.mirror", name);
		repo_config_set(the_repository, buf.buf, "true");
	}

	якщо (fetch_tags != TAGS_DEFAULT) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.tagOpt", name);
		repo_config_set(the_repository, buf.buf,
				fetch_tags == TAGS_SET ? "--tags" : "--no-tags");
	}

	якщо (fetch && fetch_remote(name)) {
		result = 1;
		goto out;
	}

	якщо (master) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "refs/remotes/%s/HEAD", name);

		strbuf_reset(&buf2);
		strbuf_addf(&buf2, "refs/remotes/%s/%s", name, master);

		якщо (refs_update_symref(get_main_ref_store(the_repository), buf.buf, buf2.buf, "remote add"))
			result = error(_("Could not setup master '%s'"), master);
	}

out:
	strbuf_release(&buf);
	strbuf_release(&buf2);
	string_list_clear(&track, 0);

	повернути result;
}

struct branch_info {
	char *remote_name;
	struct string_list merge;
	enum rebase_type rebase;
	char *push_remote_name;
};

static struct string_list branch_list = STRING_LIST_INIT_DUP;

static const char *abbrev_ref(const char *name, const char *prefix)
{
	skip_prefix(name, prefix, &name);
	повернути name;
}
#define abbrev_branch(name) abbrev_ref((name), "refs/heads/")

static ціле config_read_branches(const char *key, const char *value,
				const struct config_context *ctx UNUSED,
				void *data UNUSED)
{
	const char *orig_key = key;
	char *name;
	struct string_list_item *item;
	struct branch_info *info;
	enum { REMOTE, MERGE, REBASE, PUSH_REMOTE } type;
	size_t key_len;

	якщо (!starts_with(key, "branch."))
		повернути 0;

	key += strlen("branch.");
	якщо (strip_suffix(key, ".remote", &key_len))
		type = REMOTE;
	інакше якщо (strip_suffix(key, ".merge", &key_len))
		type = MERGE;
	інакше якщо (strip_suffix(key, ".rebase", &key_len))
		type = REBASE;
	інакше якщо (strip_suffix(key, ".pushremote", &key_len))
		type = PUSH_REMOTE;
	інакше
		повернути 0;

	name = xmemdupz(key, key_len);
	item = string_list_insert(&branch_list, name);

	якщо (!item->util)
		item->util = xcalloc(1, sizeof(struct branch_info));
	info = item->util;
	switch (type) {
	case REMOTE:
		якщо (info->remote_name)
			warning(_("more than one %s"), orig_key);
		info->remote_name = xstrdup(value);
		break;
	case MERGE: {
		const char *space = strchr(value, ' ');
		value = abbrev_branch(value);
		поки (space) {
			char *merge;
			merge = xstrndup(value, space - value);
			string_list_append(&info->merge, merge);
			value = abbrev_branch(space + 1);
			space = strchr(value, ' ');
		}
		string_list_append(&info->merge, xstrdup(value));
		break;
	}
	case REBASE:
		/*
		 * Consider invalid values as false and check the
		 * truth value with >= REBASE_TRUE.
		 */
		info->rebase = rebase_parse_value(value);
		якщо (info->rebase == REBASE_INVALID)
			warning(_("unhandled branch.%s.rebase=%s; assuming "
				  "'true'"), name, value);
		break;
	case PUSH_REMOTE:
		якщо (info->push_remote_name)
			warning(_("more than one %s"), orig_key);
		info->push_remote_name = xstrdup(value);
		break;
	default:
		BUG("unexpected type=%d", type);
	}

	free(name);
	повернути 0;
}

static void read_branches(void)
{
	якщо (branch_list.nr)
		повернути;
	repo_config(the_repository, config_read_branches, NULL);
}

struct ref_states {
	struct remote *remote;
	struct string_list new_refs, skipped, stale, tracked, heads, push;
	ціле queried;
};

#define REF_STATES_INIT { \
	.new_refs = STRING_LIST_INIT_DUP, \
	.skipped = STRING_LIST_INIT_DUP, \
	.stale = STRING_LIST_INIT_DUP, \
	.tracked = STRING_LIST_INIT_DUP, \
	.heads = STRING_LIST_INIT_DUP, \
	.push = STRING_LIST_INIT_DUP, \
}

static ціле get_ref_states(const struct ref *remote_refs, struct ref_states *states)
{
	struct ref *fetch_map = NULL, **tail = &fetch_map;
	struct ref *ref, *stale_refs;
	ціле i;

	для (i = 0; i < states->remote->fetch.nr; i++)
		якщо (get_fetch_map(remote_refs, &states->remote->fetch.items[i], &tail, 1))
			die(_("Could not get fetch map for refspec %s"),
				states->remote->fetch.items[i].raw);

	для (ref = fetch_map; ref; ref = ref->next) {
		якщо (refname_matches_negative_refspec_item(ref->name, &states->remote->fetch))
			string_list_append(&states->skipped, abbrev_branch(ref->name));
		інакше якщо (!ref->peer_ref || !refs_ref_exists(get_main_ref_store(the_repository), ref->peer_ref->name))
			string_list_append(&states->new_refs, abbrev_branch(ref->name));
		інакше
			string_list_append(&states->tracked, abbrev_branch(ref->name));
	}
	stale_refs = get_stale_heads(&states->remote->fetch, fetch_map);
	для (ref = stale_refs; ref; ref = ref->next) {
		struct string_list_item *item =
			string_list_append(&states->stale, abbrev_branch(ref->name));
		item->util = xstrdup(ref->name);
	}
	free_refs(stale_refs);
	free_refs(fetch_map);

	string_list_sort(&states->new_refs);
	string_list_sort(&states->skipped);
	string_list_sort(&states->tracked);
	string_list_sort(&states->stale);

	повернути 0;
}

struct push_info {
	char *dest;
	ціле forced;
	enum {
		PUSH_STATUS_CREATE = 0,
		PUSH_STATUS_DELETE,
		PUSH_STATUS_UPTODATE,
		PUSH_STATUS_FASTFORWARD,
		PUSH_STATUS_OUTOFDATE,
		PUSH_STATUS_NOTQUERIED
	} status;
};

static ціле get_push_ref_states(const struct ref *remote_refs,
	struct ref_states *states)
{
	struct remote *remote = states->remote;
	struct ref *ref, *local_refs, *push_map;
	якщо (remote->mirror)
		повернути 0;

	local_refs = get_local_heads();
	push_map = copy_ref_list(remote_refs);

	match_push_refs(local_refs, &push_map, &remote->push, MATCH_REFS_NONE);

	для (ref = push_map; ref; ref = ref->next) {
		struct string_list_item *item;
		struct push_info *info;

		якщо (!ref->peer_ref)
			continue;
		oidcpy(&ref->new_oid, &ref->peer_ref->new_oid);

		item = string_list_append(&states->push,
					  abbrev_branch(ref->peer_ref->name));
		item->util = xcalloc(1, sizeof(struct push_info));
		info = item->util;
		info->forced = ref->force;
		info->dest = xstrdup(abbrev_branch(ref->name));

		якщо (is_null_oid(&ref->new_oid)) {
			info->status = PUSH_STATUS_DELETE;
		} інакше якщо (oideq(&ref->old_oid, &ref->new_oid))
			info->status = PUSH_STATUS_UPTODATE;
		інакше якщо (is_null_oid(&ref->old_oid))
			info->status = PUSH_STATUS_CREATE;
		інакше якщо (odb_has_object(the_repository->objects, &ref->old_oid,
					ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR) &&
			 ref_newer(&ref->new_oid, &ref->old_oid))
			info->status = PUSH_STATUS_FASTFORWARD;
		інакше
			info->status = PUSH_STATUS_OUTOFDATE;
	}
	free_refs(local_refs);
	free_refs(push_map);
	повернути 0;
}

static ціле get_push_ref_states_noquery(struct ref_states *states)
{
	ціле i;
	struct remote *remote = states->remote;
	struct string_list_item *item;
	struct push_info *info;

	якщо (remote->mirror)
		повернути 0;

	якщо (!remote->push.nr) {
		item = string_list_append(&states->push, _("(matching)"));
		info = item->util = xcalloc(1, sizeof(struct push_info));
		info->status = PUSH_STATUS_NOTQUERIED;
		info->dest = xstrdup(item->string);
	}
	для (i = 0; i < remote->push.nr; i++) {
		const struct refspec_item *spec = &remote->push.items[i];
		якщо (spec->matching)
			item = string_list_append(&states->push, _("(matching)"));
		інакше якщо (strlen(spec->src))
			item = string_list_append(&states->push, spec->src);
		інакше
			item = string_list_append(&states->push, _("(delete)"));

		info = item->util = xcalloc(1, sizeof(struct push_info));
		info->forced = spec->force;
		info->status = PUSH_STATUS_NOTQUERIED;
		info->dest = xstrdup(spec->dst ? spec->dst : item->string);
	}
	повернути 0;
}

static ціле get_head_names(const struct ref *remote_refs, struct ref_states *states)
{
	struct ref *ref, *matches;
	struct ref *fetch_map = NULL, **fetch_map_tail = &fetch_map;
	struct refspec_item refspec = {
		.force = 0,
		.pattern = 1,
		.src = (char *) "refs/heads/*",
		.dst = (char *) "refs/heads/*",
	};

	get_fetch_map(remote_refs, &refspec, &fetch_map_tail, 0);
	matches = guess_remote_head(find_ref_by_name(remote_refs, "HEAD"),
				    fetch_map, REMOTE_GUESS_HEAD_ALL);
	для (ref = matches; ref; ref = ref->next)
		string_list_append(&states->heads, abbrev_branch(ref->name));

	free_refs(fetch_map);
	free_refs(matches);
	повернути 0;
}

struct known_remote {
	struct known_remote *next;
	struct remote *remote;
};

struct known_remotes {
	struct remote *to_delete;
	struct known_remote *list;
};

static ціле add_known_remote(struct remote *remote, void *cb_data)
{
	struct known_remotes *all = cb_data;
	struct known_remote *r;

	якщо (!strcmp(all->to_delete->name, remote->name))
		повернути 0;

	r = xmalloc(sizeof(*r));
	r->remote = remote;
	r->next = all->list;
	all->list = r;
	повернути 0;
}

struct branches_for_remote {
	struct remote *remote;
	struct string_list *branches, *skipped;
	struct known_remotes *keep;
};

static ціле add_branch_for_removal(const struct reference *ref, void *cb_data)
{
	struct branches_for_remote *branches = cb_data;
	struct refspec_item refspec;
	struct known_remote *kr;

	memset(&refspec, 0, sizeof(refspec));
	refspec.dst = (char *)ref->name;
	якщо (remote_find_tracking(branches->remote, &refspec))
		повернути 0;
	free(refspec.src);

	/* don't delete a branch if another remote also uses it */
	для (kr = branches->keep->list; kr; kr = kr->next) {
		memset(&refspec, 0, sizeof(refspec));
		refspec.dst = (char *)ref->name;
		якщо (!remote_find_tracking(kr->remote, &refspec)) {
			free(refspec.src);
			повернути 0;
		}
	}

	/* don't delete non-remote-tracking refs */
	якщо (!starts_with(ref->name, "refs/remotes/")) {
		/* advise user how to delete local branches */
		якщо (starts_with(ref->name, "refs/heads/"))
			string_list_append(branches->skipped,
					   abbrev_branch(ref->name));
		/* silently skip over other non-remote refs */
		повернути 0;
	}

	string_list_append(branches->branches, ref->name);

	повернути 0;
}

struct rename_info {
	const char *old_name;
	const char *new_name;
	struct ref_transaction *transaction;
	struct progress *progress;
	struct strbuf *err;
	uint32_t progress_nr;
	uint64_t index;
};

static void compute_renamed_ref(struct rename_info *rename,
				const char *refname,
				struct strbuf *out)
{
	strbuf_reset(out);
	strbuf_addstr(out, refname);
	strbuf_splice(out, strlen("refs/remotes/"), strlen(rename->old_name),
		      rename->new_name, strlen(rename->new_name));
}

static ціле rename_one_reflog_entry(const char *old_refname,
				   struct object_id *old_oid,
				   struct object_id *new_oid,
				   const char *committer,
				   timestamp_t timestamp, ціле tz,
				   const char *msg, void *cb_data)
{
	struct rename_info *rename = cb_data;
	struct strbuf new_refname = STRBUF_INIT;
	struct strbuf identity = STRBUF_INIT;
	struct strbuf name = STRBUF_INIT;
	struct strbuf mail = STRBUF_INIT;
	struct ident_split ident;
	const char *date;
	ціле error;

	compute_renamed_ref(rename, old_refname, &new_refname);

	якщо (split_ident_line(&ident, committer, strlen(committer)) < 0) {
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
	повернути error;
}

static ціле rename_one_reflog(const char *old_refname,
			     const struct object_id *old_oid,
			     struct rename_info *rename)
{
	struct strbuf new_refname = STRBUF_INIT;
	struct strbuf message = STRBUF_INIT;
	ціле error;

	якщо (!refs_reflog_exists(get_main_ref_store(the_repository), old_refname))
		повернути 0;

	error = refs_for_each_reflog_ent(get_main_ref_store(the_repository),
					 old_refname, rename_one_reflog_entry, rename);
	якщо (error < 0)
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
	якщо (error < 0)
		повернути error;

out:
	strbuf_release(&new_refname);
	strbuf_release(&message);
	повернути error;
}

static ціле rename_one_ref(const struct reference *ref, void *cb_data)
{
	struct strbuf new_referent = STRBUF_INIT;
	struct strbuf new_refname = STRBUF_INIT;
	struct rename_info *rename = cb_data;
	const struct object_id *oid = ref->oid;
	const char *referent = ref->target;
	ціле error;

	compute_renamed_ref(rename, ref->name, &new_refname);

	якщо (ref->flags & REF_ISSYMREF) {
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
	якщо (error < 0)
		goto out;

	error = ref_transaction_update(rename->transaction, new_refname.buf, oid, null_oid(the_hash_algo),
				       (ref->flags & REF_ISSYMREF) ? new_referent.buf : NULL, NULL,
				       REF_SKIP_CREATE_REFLOG | REF_NO_DEREF | REF_SKIP_OID_VERIFICATION,
				       NULL, rename->err);
	якщо (error < 0)
		goto out;

	error = rename_one_reflog(ref->name, oid, rename);
	якщо (error < 0)
		goto out;

	display_progress(rename->progress, ++rename->progress_nr);

out:
	strbuf_release(&new_referent);
	strbuf_release(&new_refname);
	повернути error;
}

static ціле migrate_file(struct remote *remote)
{
	struct strbuf buf = STRBUF_INIT;

	strbuf_addf(&buf, "remote.%s.url", remote->name);
	для (size_t i = 0; i < remote->url.nr; i++)
		repo_config_set_multivar(the_repository, buf.buf, remote->url.v[i], "^$", 0);
	strbuf_reset(&buf);
	strbuf_addf(&buf, "remote.%s.push", remote->name);
	для (ціле i = 0; i < remote->push.nr; i++)
		repo_config_set_multivar(the_repository, buf.buf, remote->push.items[i].raw, "^$", 0);
	strbuf_reset(&buf);
	strbuf_addf(&buf, "remote.%s.fetch", remote->name);
	для (ціле i = 0; i < remote->fetch.nr; i++)
		repo_config_set_multivar(the_repository, buf.buf, remote->fetch.items[i].raw, "^$", 0);
#ifndef WITH_BREAKING_CHANGES
	якщо (remote->origin == REMOTE_REMOTES)
		unlink_or_warn(repo_git_path_replace(the_repository, &buf,
						     "remotes/%s", remote->name));
	інакше якщо (remote->origin == REMOTE_BRANCHES)
		unlink_or_warn(repo_git_path_replace(the_repository, &buf,
						     "branches/%s", remote->name));
#endif /* WITH_BREAKING_CHANGES */
	strbuf_release(&buf);

	повернути 0;
}

struct push_default_info
{
	const char *old_name;
	enum config_scope scope;
	struct strbuf origin;
	ціле linenr;
};

static ціле config_read_push_default(const char *key, const char *value,
	const struct config_context *ctx, void *cb)
{
	const struct key_value_info *kvi = ctx->kvi;

	struct push_default_info* info = cb;
	якщо (strcmp(key, "remote.pushdefault") ||
	    !value || strcmp(value, info->old_name))
		повернути 0;

	info->scope = kvi->scope;
	strbuf_reset(&info->origin);
	strbuf_addstr(&info->origin, config_origin_type_name(kvi->origin_type));
	info->linenr = kvi->linenr;

	повернути 0;
}

static void handle_push_default(const char* old_name, const char* new_name)
{
	struct push_default_info push_default = {
		.old_name = old_name,
		.scope = CONFIG_SCOPE_UNKNOWN,
		.origin = STRBUF_INIT,
		.linenr = -1,
	};
	repo_config(the_repository, config_read_push_default, &push_default);
	якщо (push_default.scope >= CONFIG_SCOPE_COMMAND)
		; /* pass */
	інакше якщо (push_default.scope >= CONFIG_SCOPE_LOCAL) {
		ціле result = repo_config_set_gently(the_repository, "remote.pushDefault",
						    new_name);
		якщо (new_name && result && result != CONFIG_NOTHING_SET)
			die(_("could not set '%s'"), "remote.pushDefault");
		інакше якщо (!new_name && result && result != CONFIG_NOTHING_SET)
			die(_("could not unset '%s'"), "remote.pushDefault");
	} інакше якщо (push_default.scope >= CONFIG_SCOPE_SYSTEM) {
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

static const char conflicting_remote_refs_advice[] = N_(
	"The remote you are trying to rename has conflicting references in the\n"
	"new target refspec. This is most likely caused by you trying to nest\n"
	"a remote into itself, e.g. by renaming 'parent' into 'parent/child'\n"
	"or by unnesting a remote, e.g. the other way round.\n"
	"\n"
	"If that is the case, you can address this by first renaming the\n"
	"remote to a different name.\n");

static ціле mv(ціле argc, const char **argv, const char *prefix,
	      struct repository *repo UNUSED)
{
	ціле show_progress = isatty(2);
	struct option options[] = {
		OPT_BOOL(0, "progress", &show_progress, N_("force progress reporting")),
		OPT_END()
	};
	struct remote *oldremote, *newremote;
	struct strbuf buf = STRBUF_INIT, buf2 = STRBUF_INIT, buf3 = STRBUF_INIT,
		old_remote_context = STRBUF_INIT, err = STRBUF_INIT;
	struct rename_info rename = {
		.err = &err,
	};
	ціле refspecs_need_update = 0;
	ціле result = 0;

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_rename_usage, 0);

	якщо (argc != 2)
		usage_with_options(builtin_remote_rename_usage, options);

	rename.old_name = argv[0];
	rename.new_name = argv[1];

	oldremote = remote_get(rename.old_name);
	якщо (!remote_is_configured(oldremote, 1)) {
		error(_("No such remote: '%s'"), rename.old_name);
		exit(2);
	}

	якщо (!strcmp(rename.old_name, rename.new_name) && oldremote->origin != REMOTE_CONFIG)
		повернути migrate_file(oldremote);

	newremote = remote_get(rename.new_name);
	якщо (remote_is_configured(newremote, 1)) {
		error(_("remote %s already exists."), rename.new_name);
		exit(3);
	}

	якщо (!valid_remote_name(rename.new_name))
		die(_("'%s' is not a valid remote name"), rename.new_name);

	strbuf_addf(&buf, "remote.%s", rename.old_name);
	strbuf_addf(&buf2, "remote.%s", rename.new_name);
	якщо (repo_config_rename_section(the_repository, buf.buf, buf2.buf) < 1) {
		result = error(_("Could not rename config section '%s' to '%s'"),
			       buf.buf, buf2.buf);
		goto out;
	}

	strbuf_addf(&old_remote_context, ":refs/remotes/%s/", rename.old_name);

	для (ціле i = 0; i < oldremote->fetch.nr && !refspecs_need_update; i++)
		refspecs_need_update = !!strstr(oldremote->fetch.items[i].raw,
						old_remote_context.buf);

	якщо (refspecs_need_update) {
		struct refs_for_each_ref_options opts = {
			.flags = REFS_FOR_EACH_INCLUDE_BROKEN,
		};
		rename.transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
							       0, &err);
		якщо (!rename.transaction)
			goto out;

		якщо (show_progress)
			rename.progress = start_delayed_progress(the_repository,
								 _("Renaming remote references"), 0);

		strbuf_reset(&buf);
		strbuf_addf(&buf, "refs/remotes/%s/", rename.old_name);
		opts.prefix = buf.buf;

		result = refs_for_each_ref_ext(get_main_ref_store(the_repository),
					       rename_one_ref, &rename, &opts);
		якщо (result < 0)
			die(_("queueing remote ref renames failed: %s"), rename.err->buf);

		result = ref_transaction_prepare(rename.transaction, &err);
		якщо (result < 0) {
			error("renaming remote references failed: %s", err.buf);
			якщо (result == REF_TRANSACTION_ERROR_NAME_CONFLICT)
				advise(conflicting_remote_refs_advice);
			die(NULL);
		}
	}

	якщо (oldremote->fetch.nr) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.fetch", rename.new_name);
		repo_config_set_multivar(the_repository, buf.buf, NULL, NULL, CONFIG_FLAGS_MULTI_REPLACE);
		для (ціле i = 0; i < oldremote->fetch.nr; i++) {
			char *ptr;

			strbuf_reset(&buf2);
			strbuf_addstr(&buf2, oldremote->fetch.items[i].raw);
			ptr = strstr(buf2.buf, old_remote_context.buf);
			якщо (ptr) {
				strbuf_splice(&buf2,
					      ptr-buf2.buf + strlen(":refs/remotes/"),
					      strlen(rename.old_name), rename.new_name,
					      strlen(rename.new_name));
			} інакше
				warning(_("Not updating non-default fetch refspec\n"
					  "\t%s\n"
					  "\tPlease update the configuration manually if necessary."),
					buf2.buf);

			repo_config_set_multivar(the_repository, buf.buf, buf2.buf, "^$", 0);
		}
	}

	read_branches();
	для (size_t i = 0; i < branch_list.nr; i++) {
		struct string_list_item *item = branch_list.items + i;
		struct branch_info *info = item->util;
		якщо (info->remote_name && !strcmp(info->remote_name, rename.old_name)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "branch.%s.remote", item->string);
			repo_config_set(the_repository, buf.buf, rename.new_name);
		}
		якщо (info->push_remote_name && !strcmp(info->push_remote_name, rename.old_name)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "branch.%s.pushRemote", item->string);
			repo_config_set(the_repository, buf.buf, rename.new_name);
		}
	}

	якщо (refspecs_need_update) {
		result = ref_transaction_commit(rename.transaction, &err);
		якщо (result < 0)
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
	повернути result;
}

static ціле rm(ціле argc, const char **argv, const char *prefix,
	      struct repository *repo UNUSED)
{
	struct option options[] = {
		OPT_END()
	};
	struct remote *remote;
	struct strbuf buf = STRBUF_INIT;
	struct known_remotes known_remotes = { NULL, NULL };
	struct string_list branches = STRING_LIST_INIT_DUP;
	struct string_list skipped = STRING_LIST_INIT_DUP;
	struct branches_for_remote cb_data;
	ціле result;

	memset(&cb_data, 0, sizeof(cb_data));
	cb_data.branches = &branches;
	cb_data.skipped = &skipped;
	cb_data.keep = &known_remotes;

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_rm_usage, 0);
	якщо (argc != 1)
		usage_with_options(builtin_remote_rm_usage, options);

	remote = remote_get(argv[0]);
	якщо (!remote_is_configured(remote, 1)) {
		error(_("No such remote: '%s'"), argv[0]);
		exit(2);
	}

	known_remotes.to_delete = remote;
	for_each_remote(add_known_remote, &known_remotes);

	read_branches();
	для (size_t i = 0; i < branch_list.nr; i++) {
		struct string_list_item *item = branch_list.items + i;
		struct branch_info *info = item->util;
		якщо (info->remote_name && !strcmp(info->remote_name, remote->name)) {
			const char *keys[] = { "remote", "merge", NULL }, **k;
			для (k = keys; *k; k++) {
				strbuf_reset(&buf);
				strbuf_addf(&buf, "branch.%s.%s",
						item->string, *k);
				result = repo_config_set_gently(the_repository, buf.buf, NULL);
				якщо (result && result != CONFIG_NOTHING_SET)
					die(_("could not unset '%s'"), buf.buf);
			}
		}
		якщо (info->push_remote_name && !strcmp(info->push_remote_name, remote->name)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "branch.%s.pushremote", item->string);
			result = repo_config_set_gently(the_repository, buf.buf, NULL);
			якщо (result && result != CONFIG_NOTHING_SET)
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

	якщо (!result)
		result = refs_delete_refs(get_main_ref_store(the_repository),
					  "remote: remove", &branches,
					  REF_NO_DEREF);
	string_list_clear(&branches, 0);

	якщо (skipped.nr) {
		fprintf_ln(stderr,
			   Q_("Note: A branch outside the refs/remotes/ hierarchy was not removed;\n"
			      "to delete it, use:",
			      "Note: Some branches outside the refs/remotes/ hierarchy were not removed;\n"
			      "to delete them, use:",
			      skipped.nr));
		для (size_t i = 0; i < skipped.nr; i++)
			fprintf(stderr, "  git branch -d %s\n",
				skipped.items[i].string);
	}
	string_list_clear(&skipped, 0);

	якщо (!result) {
		strbuf_addf(&buf, "remote.%s", remote->name);
		якщо (repo_config_rename_section(the_repository, buf.buf, NULL) < 1) {
			result = error(_("Could not remove config section '%s'"), buf.buf);
			goto out;
		}

		handle_push_default(remote->name, NULL);
	}

out:
	для (struct known_remote *r = known_remotes.list; r;) {
		struct known_remote *next = r->next;
		free(r);
		r = next;
	}
	strbuf_release(&buf);
	повернути result;
}

static void clear_push_info(void *util, const char *string UNUSED)
{
	struct push_info *info = util;
	free(info->dest);
	free(info);
}

static void free_remote_ref_states(struct ref_states *states)
{
	string_list_clear(&states->new_refs, 0);
	string_list_clear(&states->skipped, 0);
	string_list_clear(&states->stale, 1);
	string_list_clear(&states->tracked, 0);
	string_list_clear(&states->heads, 0);
	string_list_clear_func(&states->push, clear_push_info);
}

static ціле append_ref_to_tracked_list(const struct reference *ref, void *cb_data)
{
	struct ref_states *states = cb_data;
	struct refspec_item refspec;

	якщо (ref->flags & REF_ISSYMREF)
		повернути 0;

	memset(&refspec, 0, sizeof(refspec));
	refspec.dst = (char *)ref->name;
	якщо (!remote_find_tracking(states->remote, &refspec)) {
		string_list_append(&states->tracked, abbrev_branch(refspec.src));
		free(refspec.src);
	}

	повернути 0;
}

static ціле get_remote_ref_states(const char *name,
				 struct ref_states *states,
				 ціле query)
{
	states->remote = remote_get(name);
	якщо (!states->remote)
		повернути error(_("No such remote: '%s'"), name);

	read_branches();

	якщо (query) {
		struct transport *transport;
		const struct ref *remote_refs;

		transport = transport_get(states->remote, states->remote->url.v[0]);
		remote_refs = transport_get_remote_refs(transport, NULL);

		states->queried = 1;
		якщо (query & GET_REF_STATES)
			get_ref_states(remote_refs, states);
		якщо (query & GET_HEAD_NAMES)
			get_head_names(remote_refs, states);
		якщо (query & GET_PUSH_REF_STATES)
			get_push_ref_states(remote_refs, states);
		transport_disconnect(transport);
	} інакше {
		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_ref_to_tracked_list, states);
		string_list_sort(&states->tracked);
		get_push_ref_states_noquery(states);
	}

	повернути 0;
}

struct show_info {
	struct string_list list;
	struct ref_states states;
	ціле width, width2;
	ціле any_rebase;
};

#define SHOW_INFO_INIT { \
	.list = STRING_LIST_INIT_DUP, \
	.states = REF_STATES_INIT, \
}

static ціле add_remote_to_show_info(struct string_list_item *item, void *cb_data)
{
	struct show_info *info = cb_data;
	ціле n = strlen(item->string);
	якщо (n > info->width)
		info->width = n;
	string_list_insert(&info->list, item->string);
	повернути 0;
}

static ціле show_remote_info_item(struct string_list_item *item, void *cb_data)
{
	struct show_info *info = cb_data;
	struct ref_states *states = &info->states;
	const char *name = item->string;

	якщо (states->queried) {
		const char *fmt = "%s";
		const char *arg = "";
		якщо (string_list_has_string(&states->new_refs, name)) {
			fmt = _(" new (next fetch will store in remotes/%s)");
			arg = states->remote->name;
		} інакше якщо (string_list_has_string(&states->tracked, name))
			arg = _(" tracked");
		інакше якщо (string_list_has_string(&states->skipped, name))
			arg = _(" skipped");
		інакше якщо (string_list_has_string(&states->stale, name))
			arg = _(" stale (use 'git remote prune' to remove)");
		інакше
			arg = _(" ???");
		printf("    %-*s", info->width, name);
		printf(fmt, arg);
		printf("\n");
	} інакше
		printf("    %s\n", name);

	повернути 0;
}

static ціле add_local_to_show_info(struct string_list_item *branch_item, void *cb_data)
{
	struct show_info *show_info = cb_data;
	struct ref_states *states = &show_info->states;
	struct branch_info *branch_info = branch_item->util;
	struct string_list_item *item;
	ціле n;

	якщо (!branch_info->merge.nr || !branch_info->remote_name ||
	    strcmp(states->remote->name, branch_info->remote_name))
		повернути 0;
	якщо ((n = strlen(branch_item->string)) > show_info->width)
		show_info->width = n;
	якщо (branch_info->rebase >= REBASE_TRUE)
		show_info->any_rebase = 1;

	item = string_list_insert(&show_info->list, branch_item->string);
	item->util = branch_info;

	повернути 0;
}

static ціле show_local_info_item(struct string_list_item *item, void *cb_data)
{
	struct show_info *show_info = cb_data;
	struct branch_info *branch_info = item->util;
	struct string_list *merge = &branch_info->merge;
	ціле width = show_info->width + 4;

	якщо (branch_info->rebase >= REBASE_TRUE && branch_info->merge.nr > 1) {
		error(_("invalid branch.%s.merge; cannot rebase onto > 1 branch"),
			item->string);
		повернути 0;
	}

	printf("    %-*s ", show_info->width, item->string);
	якщо (branch_info->rebase >= REBASE_TRUE) {
		const char *msg;
		якщо (branch_info->rebase == REBASE_INTERACTIVE)
			msg = _("rebases interactively onto remote %s");
		інакше якщо (branch_info->rebase == REBASE_MERGES)
			msg = _("rebases interactively (with merges) onto "
				"remote %s");
		інакше
			msg = _("rebases onto remote %s");
		printf_ln(msg, merge->items[0].string);
		повернути 0;
	} інакше якщо (show_info->any_rebase) {
		printf_ln(_(" merges with remote %s"), merge->items[0].string);
		width++;
	} інакше {
		printf_ln(_("merges with remote %s"), merge->items[0].string);
	}
	для (size_t i = 1; i < merge->nr; i++)
		printf(_("%-*s    and with remote %s\n"), width, "",
		       merge->items[i].string);

	повернути 0;
}

static ціле add_push_to_show_info(struct string_list_item *push_item, void *cb_data)
{
	struct show_info *show_info = cb_data;
	struct push_info *push_info = push_item->util;
	struct string_list_item *item;
	ціле n;
	якщо ((n = strlen(push_item->string)) > show_info->width)
		show_info->width = n;
	якщо ((n = strlen(push_info->dest)) > show_info->width2)
		show_info->width2 = n;
	item = string_list_append(&show_info->list, push_item->string);
	item->util = push_item->util;
	повернути 0;
}

/*
 * Sorting comparison for a string list that has push_info
 * structs in its util field
 */
static ціле cmp_string_with_push(const void *va, const void *vb)
{
	const struct string_list_item *a = va;
	const struct string_list_item *b = vb;
	const struct push_info *a_push = a->util;
	const struct push_info *b_push = b->util;
	ціле cmp = strcmp(a->string, b->string);
	повернути cmp ? cmp : strcmp(a_push->dest, b_push->dest);
}

static ціле show_push_info_item(struct string_list_item *item, void *cb_data)
{
	struct show_info *show_info = cb_data;
	struct push_info *push_info = item->util;
	const char *src = item->string, *status = NULL;

	switch (push_info->status) {
	case PUSH_STATUS_CREATE:
		status = _("create");
		break;
	case PUSH_STATUS_DELETE:
		status = _("delete");
		src = _("(none)");
		break;
	case PUSH_STATUS_UPTODATE:
		status = _("up to date");
		break;
	case PUSH_STATUS_FASTFORWARD:
		status = _("fast-forwardable");
		break;
	case PUSH_STATUS_OUTOFDATE:
		status = _("local out of date");
		break;
	case PUSH_STATUS_NOTQUERIED:
		break;
	}
	якщо (status) {
		якщо (push_info->forced)
			printf_ln(_("    %-*s forces to %-*s (%s)"), show_info->width, src,
			       show_info->width2, push_info->dest, status);
		інакше
			printf_ln(_("    %-*s pushes to %-*s (%s)"), show_info->width, src,
			       show_info->width2, push_info->dest, status);
	} інакше {
		якщо (push_info->forced)
			printf_ln(_("    %-*s forces to %s"), show_info->width, src,
			       push_info->dest);
		інакше
			printf_ln(_("    %-*s pushes to %s"), show_info->width, src,
			       push_info->dest);
	}
	повернути 0;
}

static ціле get_one_entry(struct remote *remote, void *priv)
{
	struct string_list *list = priv;
	struct strbuf remote_info_buf = STRBUF_INIT;
	struct strvec *url;

	якщо (remote->url.nr > 0) {
		struct strbuf promisor_config = STRBUF_INIT;
		const char *partial_clone_filter = NULL;

		strbuf_addf(&promisor_config, "remote.%s.partialclonefilter", remote->name);
		strbuf_addf(&remote_info_buf, "%s (fetch)", remote->url.v[0]);
		якщо (!repo_config_get_string_tmp(the_repository, promisor_config.buf, &partial_clone_filter))
			strbuf_addf(&remote_info_buf, " [%s]", partial_clone_filter);

		strbuf_release(&promisor_config);
		string_list_append(list, remote->name)->util =
				strbuf_detach(&remote_info_buf, NULL);
	} інакше
		string_list_append(list, remote->name)->util = NULL;
	url = push_url_of_remote(remote);
	для (size_t i = 0; i < url->nr; i++) {
		strbuf_addf(&remote_info_buf, "%s (push)", url->v[i]);
		string_list_append(list, remote->name)->util =
				strbuf_detach(&remote_info_buf, NULL);
	}

	повернути 0;
}

static ціле show_all(void)
{
	struct string_list list = STRING_LIST_INIT_DUP;
	ціле result;

	result = for_each_remote(get_one_entry, &list);

	якщо (!result) {
		string_list_sort(&list);
		для (size_t i = 0; i < list.nr; i++) {
			struct string_list_item *item = list.items + i;
			якщо (verbose)
				printf("%s\t%s\n", item->string,
					item->util ? (const char *)item->util : "");
			інакше {
				якщо (i && !strcmp((item - 1)->string, item->string))
					continue;
				printf("%s\n", item->string);
			}
		}
	}
	string_list_clear(&list, 1);
	повернути result;
}

static ціле show(ціле argc, const char **argv, const char *prefix,
		struct repository *repo UNUSED)
{
	ціле no_query = 0, result = 0, query_flag = 0;
	struct option options[] = {
		OPT_BOOL('n', NULL, &no_query, N_("do not query remotes")),
		OPT_END()
	};
	struct show_info info = SHOW_INFO_INIT;

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_show_usage,
			     0);

	якщо (argc < 1)
		повернути show_all();

	якщо (!no_query)
		query_flag = (GET_REF_STATES | GET_HEAD_NAMES | GET_PUSH_REF_STATES);

	для (; argc; argc--, argv++) {
		size_t i;
		struct strvec *url;

		get_remote_ref_states(*argv, &info.states, query_flag);

		printf_ln(_("* remote %s"), *argv);
		printf_ln(_("  Fetch URL: %s"), info.states.remote->url.v[0]);
		url = push_url_of_remote(info.states.remote);
		для (i = 0; i < url->nr; i++)
			/*
			 * TRANSLATORS: the colon ':' should align
			 * with the one in " Fetch URL: %s"
			 * translation.
			 */
			printf_ln(_("  Push  URL: %s"), url->v[i]);
		якщо (!i)
			printf_ln(_("  Push  URL: %s"), _("(no URL)"));
		якщо (no_query)
			printf_ln(_("  HEAD branch: %s"), _("(not queried)"));
		інакше якщо (!info.states.heads.nr)
			printf_ln(_("  HEAD branch: %s"), _("(unknown)"));
		інакше якщо (info.states.heads.nr == 1)
			printf_ln(_("  HEAD branch: %s"), info.states.heads.items[0].string);
		інакше {
			printf(_("  HEAD branch (remote HEAD is ambiguous,"
				 " may be one of the following):\n"));
			для (i = 0; i < info.states.heads.nr; i++)
				printf("    %s\n", info.states.heads.items[i].string);
		}

		/* remote branch info */
		info.width = 0;
		for_each_string_list(&info.states.new_refs, add_remote_to_show_info, &info);
		for_each_string_list(&info.states.skipped, add_remote_to_show_info, &info);
		for_each_string_list(&info.states.tracked, add_remote_to_show_info, &info);
		for_each_string_list(&info.states.stale, add_remote_to_show_info, &info);
		якщо (info.list.nr)
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
		якщо (info.list.nr)
			printf_ln(Q_("  Local branch configured for 'git pull':",
				     "  Local branches configured for 'git pull':",
				     info.list.nr));
		for_each_string_list(&info.list, show_local_info_item, &info);
		string_list_clear(&info.list, 0);

		/* git push info */
		якщо (info.states.remote->mirror)
			printf_ln(_("  Local refs will be mirrored by 'git push'"));

		info.width = info.width2 = 0;
		for_each_string_list(&info.states.push, add_push_to_show_info, &info);
		QSORT(info.list.items, info.list.nr, cmp_string_with_push);
		якщо (info.list.nr)
			printf_ln(Q_("  Local ref configured for 'git push'%s:",
				     "  Local refs configured for 'git push'%s:",
				     info.list.nr),
				  no_query ? _(" (status not queried)") : "");
		for_each_string_list(&info.list, show_push_info_item, &info);
		string_list_clear(&info.list, 0);

		free_remote_ref_states(&info.states);
	}

	повернути result;
}

static void report_set_head_auto(const char *remote, const char *head_name,
			struct strbuf *b_local_head, ціле was_detached) {
	struct strbuf buf_prefix = STRBUF_INIT;
	const char *prev_head = NULL;

	strbuf_addf(&buf_prefix, "refs/remotes/%s/", remote);
	skip_prefix(b_local_head->buf, buf_prefix.buf, &prev_head);

	якщо (prev_head && !strcmp(prev_head, head_name))
		printf(_("'%s/HEAD' is unchanged and points to '%s'\n"),
			remote, head_name);
	інакше якщо (prev_head)
		printf(_("'%s/HEAD' has changed from '%s' and now points to '%s'\n"),
			remote, prev_head, head_name);
	інакше якщо (!b_local_head->len)
		printf(_("'%s/HEAD' is now created and points to '%s'\n"),
			remote, head_name);
	інакше якщо (was_detached && b_local_head->len)
		printf(_("'%s/HEAD' was detached at '%s' and now points to '%s'\n"),
			remote, b_local_head->buf, head_name);
	інакше
		printf(_("'%s/HEAD' used to point to '%s' "
			"(which is not a remote branch), but now points to '%s'\n"),
			remote, b_local_head->buf, head_name);
	strbuf_release(&buf_prefix);
}

static ціле set_head(ціле argc, const char **argv, const char *prefix,
		    struct repository *repo UNUSED)
{
	ціле opt_a = 0, opt_d = 0, result = 0, was_detached;
	struct strbuf b_head = STRBUF_INIT, b_remote_head = STRBUF_INIT,
		b_local_head = STRBUF_INIT;
	char *head_name = NULL;
	struct ref_store *refs = get_main_ref_store(the_repository);
	struct remote *remote;

	struct option options[] = {
		OPT_BOOL('a', "auto", &opt_a,
			 N_("set refs/remotes/<name>/HEAD according to remote")),
		OPT_BOOL('d', "delete", &opt_d,
			 N_("delete refs/remotes/<name>/HEAD")),
		OPT_END()
	};
	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_sethead_usage, 0);

	/* All modes require at least a remote name. */
	якщо (!argc)
		usage_with_options(builtin_remote_sethead_usage, options);

	strbuf_addf(&b_head, "refs/remotes/%s/HEAD", argv[0]);
	remote = remote_get(argv[0]);

	якщо (!opt_a && !opt_d && argc == 2) {
		head_name = xstrdup(argv[1]);
	} інакше якщо (opt_a && !opt_d && argc == 1) {
		struct ref_states states = REF_STATES_INIT;
		get_remote_ref_states(argv[0], &states, GET_HEAD_NAMES);
		якщо (!states.heads.nr)
			result |= error(_("Cannot determine remote HEAD"));
		інакше якщо (states.heads.nr > 1) {
			result |= error(_("Multiple remote HEAD branches. "
					  "Please choose one explicitly with:"));
			для (size_t i = 0; i < states.heads.nr; i++)
				fprintf(stderr, "  git remote set-head %s %s\n",
					argv[0], states.heads.items[i].string);
		} інакше
			head_name = xstrdup(states.heads.items[0].string);
		free_remote_ref_states(&states);
	} інакше якщо (opt_d && !opt_a && argc == 1) {
		якщо (refs_delete_ref(refs, NULL, b_head.buf, NULL, REF_NO_DEREF))
			result |= error(_("Could not delete %s"), b_head.buf);
	} інакше
		usage_with_options(builtin_remote_sethead_usage, options);

	якщо (!head_name)
		goto cleanup;
	strbuf_addf(&b_remote_head, "refs/remotes/%s/%s", argv[0], head_name);
	якщо (!refs_ref_exists(refs, b_remote_head.buf)) {
		result |= error(_("Not a valid ref: %s"), b_remote_head.buf);
		goto cleanup;
	}
	was_detached = refs_update_symref_extended(refs, b_head.buf, b_remote_head.buf,
			"remote set-head", &b_local_head, 0);
	якщо (was_detached == -1) {
		result |= error(_("Could not set up %s"), b_head.buf);
		goto cleanup;
	}
	якщо (opt_a)
		report_set_head_auto(argv[0], head_name, &b_local_head, was_detached);
	якщо (remote->follow_remote_head == FOLLOW_REMOTE_ALWAYS) {
		struct strbuf config_name = STRBUF_INIT;
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
	повернути result;
}

static ціле prune_remote(const char *remote, ціле dry_run)
{
	ціле result = 0;
	struct ref_states states = REF_STATES_INIT;
	struct string_list refs_to_prune = STRING_LIST_INIT_NODUP;
	struct string_list_item *item;

	get_remote_ref_states(remote, &states, GET_REF_STATES);

	якщо (!states.stale.nr) {
		free_remote_ref_states(&states);
		повернути 0;
	}

	printf_ln(_("Pruning %s"), remote);
	printf_ln(_("URL: %s"), states.remote->url.v[0]);

	for_each_string_list_item(item, &states.stale)
		string_list_append(&refs_to_prune, item->util);
	string_list_sort(&refs_to_prune);

	якщо (!dry_run)
		result |= refs_delete_refs(get_main_ref_store(the_repository),
					   "remote: prune", &refs_to_prune, 0);

	for_each_string_list_item(item, &states.stale) {
		const char *refname = item->util;

		якщо (dry_run)
			printf_ln(_(" * [would prune] %s"),
			       abbrev_ref(refname, "refs/remotes/"));
		інакше
			printf_ln(_(" * [pruned] %s"),
			       abbrev_ref(refname, "refs/remotes/"));
	}

	refs_warn_dangling_symrefs(get_main_ref_store(the_repository),
				   stdout, " ", dry_run, &refs_to_prune);

	string_list_clear(&refs_to_prune, 0);
	free_remote_ref_states(&states);
	повернути result;
}

static ціле prune(ціле argc, const char **argv, const char *prefix,
		 struct repository *repo UNUSED)
{
	ціле dry_run = 0, result = 0;
	struct option options[] = {
		OPT__DRY_RUN(&dry_run, N_("dry run")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_prune_usage, 0);

	якщо (argc < 1)
		usage_with_options(builtin_remote_prune_usage, options);

	для (; argc; argc--, argv++)
		result |= prune_remote(*argv, dry_run);

	повернути result;
}

static ціле get_remote_default(const char *key, const char *value UNUSED,
			      const struct config_context *ctx UNUSED,
			      void *priv)
{
	якщо (strcmp(key, "remotes.default") == 0) {
		ціле *found = priv;
		*found = 1;
	}
	повернути 0;
}

static ціле update(ціле argc, const char **argv, const char *prefix,
		  struct repository *repo UNUSED)
{
	ціле i, prune = -1;
	struct option options[] = {
		OPT_BOOL('p', "prune", &prune,
			 N_("prune remotes after fetching")),
		OPT_END()
	};
	struct child_process cmd = CHILD_PROCESS_INIT;
	ціле default_defined = 0;

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_update_usage,
			     PARSE_OPT_KEEP_ARGV0);

	strvec_push(&cmd.args, "fetch");

	якщо (prune != -1)
		strvec_push(&cmd.args, prune ? "--prune" : "--no-prune");
	якщо (verbose)
		strvec_push(&cmd.args, "-v");
	strvec_push(&cmd.args, "--multiple");
	якщо (argc < 2)
		strvec_push(&cmd.args, "default");
	для (i = 1; i < argc; i++)
		strvec_push(&cmd.args, argv[i]);

	якщо (strcmp(cmd.args.v[cmd.args.nr-1], "default") == 0) {
		repo_config(the_repository, get_remote_default, &default_defined);
		якщо (!default_defined) {
			strvec_pop(&cmd.args);
			strvec_push(&cmd.args, "--all");
		}
	}

	cmd.git_cmd = 1;
	повернути run_command(&cmd);
}

static ціле remove_all_fetch_refspecs(const char *key)
{
	повернути repo_config_set_multivar_gently(the_repository, key, NULL, NULL,
					       CONFIG_FLAGS_MULTI_REPLACE);
}

static void add_branches(struct remote *remote, const char **branches,
			 const char *key)
{
	const char *remotename = remote->name;
	ціле mirror = remote->mirror;
	struct strbuf refspec = STRBUF_INIT;

	для (; *branches; branches++)
		add_branch(key, *branches, remotename, mirror, &refspec);

	strbuf_release(&refspec);
}

static ціле set_remote_branches(const char *remotename, const char **branches,
				ціле add_mode)
{
	struct strbuf key = STRBUF_INIT;
	struct remote *remote;

	strbuf_addf(&key, "remote.%s.fetch", remotename);

	remote = remote_get(remotename);
	якщо (!remote_is_configured(remote, 1)) {
		error(_("No such remote '%s'"), remotename);
		exit(2);
	}

	якщо (!add_mode && remove_all_fetch_refspecs(key.buf)) {
		strbuf_release(&key);
		повернути 1;
	}
	add_branches(remote, branches, key.buf);

	strbuf_release(&key);
	повернути 0;
}

static ціле set_branches(ціле argc, const char **argv, const char *prefix,
			struct repository *repo UNUSED)
{
	ціле add_mode = 0;
	struct option options[] = {
		OPT_BOOL('\0', "add", &add_mode, N_("add branch")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_setbranches_usage, 0);
	якщо (argc == 0) {
		error(_("no remote specified"));
		usage_with_options(builtin_remote_setbranches_usage, options);
	}
	argv[argc] = NULL;

	повернути set_remote_branches(argv[0], argv + 1, add_mode);
}

static ціле get_url(ціле argc, const char **argv, const char *prefix,
		   struct repository *repo UNUSED)
{
	ціле push_mode = 0, all_mode = 0;
	const char *remotename = NULL;
	struct remote *remote;
	struct strvec *url;
	struct option options[] = {
		OPT_BOOL('\0', "push", &push_mode,
			 N_("query push URLs rather than fetch URLs")),
		OPT_BOOL('\0', "all", &all_mode,
			 N_("return all URLs")),
		OPT_END()
	};
	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_geturl_usage, 0);

	якщо (argc != 1)
		usage_with_options(builtin_remote_geturl_usage, options);

	remotename = argv[0];

	remote = remote_get(remotename);
	якщо (!remote_is_configured(remote, 1)) {
		error(_("No such remote '%s'"), remotename);
		exit(2);
	}

	url = push_mode ? push_url_of_remote(remote) : &remote->url;

	якщо (all_mode) {
		для (size_t i = 0; i < url->nr; i++)
			printf_ln("%s", url->v[i]);
	} інакше {
		printf_ln("%s", url->v[0]);
	}

	повернути 0;
}

static ціле set_url(ціле argc, const char **argv, const char *prefix,
		   struct repository *repo UNUSED)
{
	ціле push_mode = 0, add_mode = 0, delete_mode = 0;
	ціле matches = 0, negative_matches = 0;
	const char *remotename = NULL;
	const char *newurl = NULL;
	const char *oldurl = NULL;
	struct remote *remote;
	regex_t old_regex;
	struct strvec *urlset;
	struct strbuf name_buf = STRBUF_INIT;
	struct option options[] = {
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

	якщо (add_mode && delete_mode)
		die(_("--add --delete doesn't make sense"));

	якщо (argc < 3 || argc > 4 || ((add_mode || delete_mode) && argc != 3))
		usage_with_options(builtin_remote_seturl_usage, options);

	remotename = argv[1];
	newurl = argv[2];
	якщо (argc > 3)
		oldurl = argv[3];

	якщо (delete_mode)
		oldurl = newurl;

	remote = remote_get(remotename);
	якщо (!remote_is_configured(remote, 1)) {
		error(_("No such remote '%s'"), remotename);
		exit(2);
	}

	якщо (push_mode) {
		strbuf_addf(&name_buf, "remote.%s.pushurl", remotename);
		urlset = &remote->pushurl;
	} інакше {
		strbuf_addf(&name_buf, "remote.%s.url", remotename);
		urlset = &remote->url;
	}

	/* Special cases that add new entry. */
	якщо ((!oldurl && !delete_mode) || add_mode) {
		якщо (add_mode)
			repo_config_set_multivar(the_repository, name_buf.buf, newurl,
						       "^$", 0);
		інакше
			repo_config_set(the_repository, name_buf.buf, newurl);
		goto out;
	}

	/* Old URL specified. Demand that one matches. */
	якщо (regcomp(&old_regex, oldurl, REG_EXTENDED))
		die(_("Invalid old URL pattern: %s"), oldurl);

	для (size_t i = 0; i < urlset->nr; i++)
		якщо (!regexec(&old_regex, urlset->v[i], 0, NULL, 0))
			matches++;
		інакше
			negative_matches++;
	якщо (!delete_mode && !matches)
		die(_("No such URL found: %s"), oldurl);
	якщо (delete_mode && !negative_matches && !push_mode)
		die(_("Will not delete all non-push URLs"));

	regfree(&old_regex);

	якщо (!delete_mode)
		repo_config_set_multivar(the_repository, name_buf.buf, newurl, oldurl, 0);
	інакше
		repo_config_set_multivar(the_repository, name_buf.buf, NULL, oldurl,
					 CONFIG_FLAGS_MULTI_REPLACE);
out:
	strbuf_release(&name_buf);
	повернути 0;
}

ціле cmd_remote(ціле argc,
	       const char **argv,
	       const char *prefix,
	       struct repository *repo)
{
	parse_opt_subcommand_fn *fn = NULL;
	struct option options[] = {
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

	якщо (fn) {
		повернути !!fn(argc, argv, prefix, repo);
	} інакше {
		якщо (argc) {
			error(_("unknown subcommand: `%s'"), argv[0]);
			usage_with_options(builtin_remote_usage, options);
		}
		повернути !!show_all();
	}
}
