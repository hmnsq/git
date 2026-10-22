/*
 * "git fetch"
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#include "builtin.h"
#include "advice.h"
#include "config.h"
#include "gettext.h"
#include "environment.h"
#include "hex.h"
#include "refs.h"
#include "refspec.h"
#include "object-name.h"
#include "odb.h"
#include "odb/source.h"
#include "oidset.h"
#include "oid-array.h"
#include "commit.h"
#include "string-list.h"
#include "remote.h"
#include "transport.h"
#include "run-command.h"
#include "parse-options.h"
#include "sigchain.h"
#include "submodule-config.h"
#include "submodule.h"
#include "connected.h"
#include "strvec.h"
#include "utf8.h"
#include "pager.h"
#include "path.h"
#include "pkt-line.h"
#include "list-objects-filter-options.h"
#include "commit-reach.h"
#include "branch.h"
#include "promisor-remote.h"
#include "commit-graph.h"
#include "shallow.h"
#include "trace.h"
#include "trace2.h"
#include "bundle-uri.h"

#define FORCED_UPDATES_DELAY_WARNING_IN_MS (10 * 1000)

static const char * const builtin_fetch_usage[] = {
	N_("git fetch [<options>] [<repository> [<refspec>...]]"),
	N_("git fetch [<options>] <group>"),
	N_("git fetch --multiple [<options>] [(<repository>|<group>)...]"),
	N_("git fetch --all [<options>]"),
	NULL
};

enum {
	TAGS_UNSET = 0,
	TAGS_DEFAULT = 1,
	TAGS_SET = 2
};

enum display_format {
	DISPLAY_FORMAT_FULL,
	DISPLAY_FORMAT_COMPACT,
	DISPLAY_FORMAT_PORCELAIN,
};

struct display_state {
	struct strbuf buf;

	целое refcol_width;
	enum display_format format;

	char *url;
	целое url_len, shown_url;
};

static uint64_t forced_updates_ms = 0;
static целое prefetch = 0;
static целое prune = -1; /* unspecified */
#define PRUNE_BY_DEFAULT 0 /* do we prune by default? */

static целое prune_tags = -1; /* unspecified */
#define PRUNE_TAGS_BY_DEFAULT 0 /* do we prune tags by default? */

static целое append, dry_run, force, keep, update_head_ok;
static целое write_fetch_head = 1;
static целое verbosity, deepen_relative, set_upstream, refetch;
static целое progress = -1;
static целое tags = TAGS_DEFAULT, update_shallow, deepen;
static целое atomic_fetch;
static enum transport_family family;
static const char *depth;
static const char *deepen_since;
static const char *upload_pack;
static struct string_list deepen_not = STRING_LIST_INIT_NODUP;
static struct strbuf default_rla = STRBUF_INIT;
static struct transport *gtransport;
static struct transport *gsecondary;
static struct refspec refmap;
static struct string_list server_options = STRING_LIST_INIT_DUP;
static struct string_list negotiation_restrict = STRING_LIST_INIT_NODUP;
static struct string_list negotiation_include = STRING_LIST_INIT_NODUP;

struct fetch_config {
	enum display_format display_format;
	char *follow_remote_head_raw;
	целое follow_remote_head_seen;
	целое all;
	целое prune;
	целое prune_tags;
	целое show_forced_updates;
	целое recurse_submodules;
	целое parallel;
	целое submodule_fetch_jobs;
};

static целое git_fetch_config(const char *k, const char *v,
			    const struct config_context *ctx, void *cb)
{
	struct fetch_config *fetch_config = cb;

	если (!strcmp(k, "fetch.all")) {
		fetch_config->all = git_config_bool(k, v);
		вернуть 0;
	}

	если (!strcmp(k, "fetch.prune")) {
		fetch_config->prune = git_config_bool(k, v);
		вернуть 0;
	}

	если (!strcmp(k, "fetch.prunetags")) {
		fetch_config->prune_tags = git_config_bool(k, v);
		вернуть 0;
	}

	если (!strcmp(k, "fetch.showforcedupdates")) {
		fetch_config->show_forced_updates = git_config_bool(k, v);
		вернуть 0;
	}

	если (!strcmp(k, "submodule.recurse")) {
		целое r = git_config_bool(k, v) ?
			RECURSE_SUBMODULES_ON : RECURSE_SUBMODULES_OFF;
		fetch_config->recurse_submodules = r;
		вернуть 0;
	}

	если (!strcmp(k, "submodule.fetchjobs")) {
		fetch_config->submodule_fetch_jobs = parse_submodule_fetchjobs(k, v, ctx->kvi);
		вернуть 0;
	} иначе если (!strcmp(k, "fetch.recursesubmodules")) {
		fetch_config->recurse_submodules = parse_fetch_recurse_submodules_arg(k, v);
		вернуть 0;
	}

	если (!strcmp(k, "fetch.parallel")) {
		fetch_config->parallel = git_config_int(k, v, ctx->kvi);
		если (fetch_config->parallel < 0)
			die(_("fetch.parallel cannot be negative"));
		если (!fetch_config->parallel)
			fetch_config->parallel = online_cpus();
		вернуть 0;
	}

	если (!strcmp(k, "fetch.output")) {
		если (!v)
			вернуть config_error_nonbool(k);
		иначе если (!strcasecmp(v, "full"))
			fetch_config->display_format = DISPLAY_FORMAT_FULL;
		иначе если (!strcasecmp(v, "compact"))
			fetch_config->display_format = DISPLAY_FORMAT_COMPACT;
		иначе
			die(_("invalid value for '%s': '%s'"),
			    "fetch.output", v);
		вернуть 0;
	}

	если (!strcmp(k, "fetch.followremotehead")) {
		free(fetch_config->follow_remote_head_raw);
		fetch_config->follow_remote_head_raw = xstrdup_or_null(v);
		fetch_config->follow_remote_head_seen = 1;
		вернуть 0;
	}

	вернуть git_default_config(k, v, ctx, cb);
}

static enum follow_remote_head_settings get_follow_remote_head(const char *setting)
{
	если (!setting)
		die(_("missing value for 'fetch.followRemoteHEAD'"));
	иначе если (!strcmp(setting, "never"))
		вернуть FOLLOW_REMOTE_NEVER;
	иначе если (!strcmp(setting, "create"))
		вернуть FOLLOW_REMOTE_CREATE;
	иначе если (!strcmp(setting, "warn"))
		вернуть FOLLOW_REMOTE_WARN;
	иначе если (!strcmp(setting, "always"))
		вернуть FOLLOW_REMOTE_ALWAYS;
	warning(_("unrecognized fetch.followRemoteHEAD value '%s' ignored"), setting);
	вернуть BUILTIN_FOLLOW_REMOTE_HEAD_DFLT;
}

static целое parse_refmap_arg(const struct option *opt, const char *arg, целое unset)
{
	BUG_ON_OPT_NEG(unset);

	/*
	 * "git fetch --refmap='' origin foo"
	 * can be used to tell the command not to store anywhere
	 */
	refspec_append(opt->value, arg);

	вернуть 0;
}

static void unlock_pack(unsigned целое flags)
{
	если (gtransport)
		transport_unlock_pack(gtransport, flags);
	если (gsecondary)
		transport_unlock_pack(gsecondary, flags);
}

static void unlock_pack_atexit(void)
{
	unlock_pack(0);
}

static void unlock_pack_on_signal(целое signo)
{
	unlock_pack(TRANSPORT_UNLOCK_PACK_IN_SIGNAL_HANDLER);
	sigchain_pop(signo);
	raise(signo);
}

static void add_merge_config(struct ref **head,
			   const struct ref *remote_refs,
		           struct branch *branch,
		           struct ref ***tail)
{
	целое i;

	для (i = 0; i < branch->merge_nr; i++) {
		struct ref *rm, **old_tail = *tail;
		struct refspec_item refspec;

		для (rm = *head; rm; rm = rm->next) {
			если (branch_merge_matches(branch, i, rm->name)) {
				rm->fetch_head_status = FETCH_HEAD_MERGE;
				break;
			}
		}
		если (rm)
			continue;

		/*
		 * Not fetched to a remote-tracking branch?  We need to fetch
		 * it anyway to allow this branch's "branch.$name.merge"
		 * to be honored by 'git pull', but we do not have to
		 * fail if branch.$name.merge is misconfigured to point
		 * at a nonexisting branch.  If we were indeed called by
		 * 'git pull', it will notice the misconfiguration because
		 * there is no entry in the resulting FETCH_HEAD marked
		 * for merging.
		 */
		memset(&refspec, 0, sizeof(refspec));
		refspec.src = branch->merge[i]->src;
		get_fetch_map(remote_refs, &refspec, tail, 1);
		для (rm = *old_tail; rm; rm = rm->next)
			rm->fetch_head_status = FETCH_HEAD_MERGE;
	}
}

static void create_fetch_oidset(struct ref **head, struct oidset *out)
{
	struct ref *rm = *head;
	пока (rm) {
		oidset_insert(out, &rm->old_oid);
		rm = rm->next;
	}
}

struct refname_hash_entry {
	struct hashmap_entry ent;
	struct object_id oid;
	целое ignore;
	char refname[FLEX_ARRAY];
};

static целое refname_hash_entry_cmp(const void *hashmap_cmp_fn_data UNUSED,
				  const struct hashmap_entry *eptr,
				  const struct hashmap_entry *entry_or_key,
				  const void *keydata)
{
	const struct refname_hash_entry *e1, *e2;

	e1 = container_of(eptr, const struct refname_hash_entry, ent);
	e2 = container_of(entry_or_key, const struct refname_hash_entry, ent);
	вернуть strcmp(e1->refname, keydata ? keydata : e2->refname);
}

static struct refname_hash_entry *refname_hash_add(struct hashmap *map,
						   const char *refname,
						   const struct object_id *oid)
{
	struct refname_hash_entry *ent;
	size_t len = strlen(refname);

	FLEX_ALLOC_MEM(ent, refname, refname, len);
	hashmap_entry_init(&ent->ent, strhash(refname));
	oidcpy(&ent->oid, oid);
	hashmap_add(map, &ent->ent);
	вернуть ent;
}

static целое add_one_refname(const struct reference *ref, void *cbdata)
{
	struct hashmap *refname_map = cbdata;

	(void) refname_hash_add(refname_map, ref->name, ref->oid);
	вернуть 0;
}

static void refname_hash_init(struct hashmap *map)
{
	hashmap_init(map, refname_hash_entry_cmp, NULL, 0);
}

static целое refname_hash_exists(struct hashmap *map, const char *refname)
{
	вернуть !!hashmap_get_from_hash(map, strhash(refname), refname);
}

static void clear_item(struct refname_hash_entry *item)
{
	item->ignore = 1;
}


static void add_already_queued_tags(const char *refname,
				    const struct object_id *old_oid UNUSED,
				    const struct object_id *new_oid,
				    void *cb_data)
{
	struct hashmap *queued_tags = cb_data;
	если (starts_with(refname, "refs/tags/") && new_oid)
		(void) refname_hash_add(queued_tags, refname, new_oid);
}

static void find_non_local_tags(const struct ref *refs,
				struct ref_transaction *transaction,
				struct ref **head,
				struct ref ***tail)
{
	struct hashmap existing_refs;
	struct hashmap remote_refs;
	struct oidset fetch_oids = OIDSET_INIT;
	struct string_list remote_refs_list = STRING_LIST_INIT_NODUP;
	struct string_list_item *remote_ref_item;
	const struct ref *ref;
	struct refname_hash_entry *item = NULL;

	refname_hash_init(&existing_refs);
	refname_hash_init(&remote_refs);
	create_fetch_oidset(head, &fetch_oids);

	refs_for_each_ref(get_main_ref_store(the_repository), add_one_refname,
			  &existing_refs);

	/*
	 * If we already have a transaction, then we need to filter out all
	 * tags which have already been queued up.
	 */
	если (transaction)
		ref_transaction_for_each_queued_update(transaction,
						       add_already_queued_tags,
						       &existing_refs);

	для (ref = refs; ref; ref = ref->next) {
		если (!starts_with(ref->name, "refs/tags/"))
			continue;

		/*
		 * The peeled ref always follows the matching base
		 * ref, so if we see a peeled ref that we don't want
		 * to fetch then we can mark the ref entry in the list
		 * as one to ignore by setting util to NULL.
		 */
		если (ends_with(ref->name, "^{}")) {
			если (item &&
			    !odb_has_object(the_repository->objects, &ref->old_oid, 0) &&
			    !oidset_contains(&fetch_oids, &ref->old_oid) &&
			    !odb_has_object(the_repository->objects, &item->oid, 0) &&
			    !oidset_contains(&fetch_oids, &item->oid))
				clear_item(item);
			item = NULL;
			continue;
		}

		/*
		 * If item is non-NULL here, then we previously saw a
		 * ref not followed by a peeled reference, so we need
		 * to check if it is a lightweight tag that we want to
		 * fetch.
		 */
		если (item &&
		    !odb_has_object(the_repository->objects, &item->oid, 0) &&
		    !oidset_contains(&fetch_oids, &item->oid))
			clear_item(item);

		item = NULL;

		/* skip duplicates and refs that we already have */
		если (refname_hash_exists(&remote_refs, ref->name) ||
		    refname_hash_exists(&existing_refs, ref->name))
			continue;

		item = refname_hash_add(&remote_refs, ref->name, &ref->old_oid);
		string_list_insert(&remote_refs_list, ref->name);
	}
	hashmap_clear_and_free(&existing_refs, struct refname_hash_entry, ent);

	/*
	 * We may have a final lightweight tag that needs to be
	 * checked to see if it needs fetching.
	 */
	если (item &&
	    !odb_has_object(the_repository->objects, &item->oid, 0) &&
	    !oidset_contains(&fetch_oids, &item->oid))
		clear_item(item);

	/*
	 * For all the tags in the remote_refs_list,
	 * add them to the list of refs to be fetched
	 */
	for_each_string_list_item(remote_ref_item, &remote_refs_list) {
		const char *refname = remote_ref_item->string;
		struct ref *rm;
		unsigned целое hash = strhash(refname);

		item = hashmap_get_entry_from_hash(&remote_refs, hash, refname,
					struct refname_hash_entry, ent);
		если (!item)
			BUG("unseen remote ref?");

		/* Unless we have already decided to ignore this item... */
		если (item->ignore)
			continue;

		rm = alloc_ref(item->refname);
		rm->peer_ref = alloc_ref(item->refname);
		oidcpy(&rm->old_oid, &item->oid);
		**tail = rm;
		*tail = &rm->next;
	}
	hashmap_clear_and_free(&remote_refs, struct refname_hash_entry, ent);
	string_list_clear(&remote_refs_list, 0);
	oidset_clear(&fetch_oids);
}

static void filter_prefetch_refspec(struct refspec *rs)
{
	целое i;

	если (!prefetch)
		вернуть;

	для (i = 0; i < rs->nr; i++) {
		struct strbuf new_dst = STRBUF_INIT;
		char *old_dst;
		const char *sub = NULL;

		если (rs->items[i].negative)
			continue;
		если (!rs->items[i].dst ||
		    (rs->items[i].src &&
		     starts_with(rs->items[i].src,
				 ref_namespace[NAMESPACE_TAGS].ref))) {
			целое j;

			refspec_item_clear(&rs->items[i]);

			для (j = i + 1; j < rs->nr; j++)
				rs->items[j - 1] = rs->items[j];
			rs->nr--;
			i--;
			continue;
		}

		old_dst = rs->items[i].dst;
		strbuf_addstr(&new_dst, ref_namespace[NAMESPACE_PREFETCH].ref);

		/*
		 * If old_dst starts with "refs/", then place
		 * sub after that prefix. Otherwise, start at
		 * the beginning of the string.
		 */
		если (!skip_prefix(old_dst, "refs/", &sub))
			sub = old_dst;
		strbuf_addstr(&new_dst, sub);

		rs->items[i].dst = strbuf_detach(&new_dst, NULL);
		rs->items[i].force = 1;

		free(old_dst);
	}
}

static struct ref *get_ref_map(struct remote *remote,
			       const struct ref *remote_refs,
			       struct refspec *rs,
			       целое tags, целое *autotags)
{
	целое i;
	struct ref *rm;
	struct ref *ref_map = NULL;
	struct ref **tail = &ref_map;

	/* opportunistically-updated references: */
	struct ref *orefs = NULL, **oref_tail = &orefs;

	struct hashmap existing_refs;
	целое existing_refs_populated = 0;

	filter_prefetch_refspec(rs);
	если (remote)
		filter_prefetch_refspec(&remote->fetch);

	если (rs->nr) {
		struct refspec *fetch_refspec;

		для (i = 0; i < rs->nr; i++) {
			get_fetch_map(remote_refs, &rs->items[i], &tail, 0);
			если (rs->items[i].dst && rs->items[i].dst[0])
				*autotags = 1;
		}
		/* Merge everything on the command line (but not --tags) */
		для (rm = ref_map; rm; rm = rm->next)
			rm->fetch_head_status = FETCH_HEAD_MERGE;

		/*
		 * For any refs that we happen to be fetching via
		 * command-line arguments, the destination ref might
		 * have been missing or have been different than the
		 * remote-tracking ref that would be derived from the
		 * configured refspec.  In these cases, we want to
		 * take the opportunity to update their configured
		 * remote-tracking reference.  However, we do not want
		 * to mention these entries in FETCH_HEAD at all, as
		 * they would simply be duplicates of existing
		 * entries, so we set them FETCH_HEAD_IGNORE below.
		 *
		 * We compute these entries now, based only on the
		 * refspecs specified on the command line.  But we add
		 * them to the list following the refspecs resulting
		 * from the tags option so that one of the latter,
		 * which has FETCH_HEAD_NOT_FOR_MERGE, is not removed
		 * by ref_remove_duplicates() in favor of one of these
		 * opportunistic entries with FETCH_HEAD_IGNORE.
		 */
		если (refmap.nr)
			fetch_refspec = &refmap;
		иначе
			fetch_refspec = &remote->fetch;

		для (i = 0; i < fetch_refspec->nr; i++)
			get_fetch_map(ref_map, &fetch_refspec->items[i], &oref_tail, 1);
	} иначе если (refmap.nr) {
		die("--refmap option is only meaningful with command-line refspec(s)");
	} иначе {
		/* Use the defaults */
		struct branch *branch = branch_get(NULL);
		целое has_merge = branch_has_merge_config(branch);
		если (remote &&
		    (remote->fetch.nr ||
		     /* Note: has_merge implies non-NULL branch->remote_name */
		     (has_merge && !strcmp(branch->remote_name, remote->name)))) {
			для (i = 0; i < remote->fetch.nr; i++) {
				get_fetch_map(remote_refs, &remote->fetch.items[i], &tail, 0);
				если (remote->fetch.items[i].dst &&
				    remote->fetch.items[i].dst[0])
					*autotags = 1;
				если (!i && !has_merge && ref_map &&
				    !remote->fetch.items[0].pattern)
					ref_map->fetch_head_status = FETCH_HEAD_MERGE;
			}
			/*
			 * if the remote we're fetching from is the same
			 * as given in branch.<name>.remote, we add the
			 * ref given in branch.<name>.merge, too.
			 *
			 * Note: has_merge implies non-NULL branch->remote_name
			 */
			если (has_merge &&
			    !strcmp(branch->remote_name, remote->name))
				add_merge_config(&ref_map, remote_refs, branch, &tail);
		} иначе если (!prefetch) {
			ref_map = get_remote_ref(remote_refs, "HEAD");
			если (!ref_map)
				die(_("couldn't find remote ref HEAD"));
			ref_map->fetch_head_status = FETCH_HEAD_MERGE;
			tail = &ref_map->next;
		}
	}

	если (tags == TAGS_SET) {
		struct refspec_item tag_refspec;

		/* also fetch all tags */
		refspec_item_init_push(&tag_refspec, TAG_REFSPEC,
				       the_hash_algo);
		get_fetch_map(remote_refs, &tag_refspec, &tail, 0);
		refspec_item_clear(&tag_refspec);
	} иначе если (tags == TAGS_DEFAULT && *autotags) {
		find_non_local_tags(remote_refs, NULL, &ref_map, &tail);
	}

	/* Now append any refs to be updated opportunistically: */
	*tail = orefs;
	для (rm = orefs; rm; rm = rm->next) {
		rm->fetch_head_status = FETCH_HEAD_IGNORE;
		tail = &rm->next;
	}

	/*
	 * apply negative refspecs first, before we remove duplicates. This is
	 * necessary as negative refspecs might remove an otherwise conflicting
	 * duplicate.
	 */
	если (rs->nr)
		ref_map = apply_negative_refspecs(ref_map, rs);
	иначе
		ref_map = apply_negative_refspecs(ref_map, &remote->fetch);

	ref_map = ref_remove_duplicates(ref_map);

	для (rm = ref_map; rm; rm = rm->next) {
		если (rm->peer_ref) {
			const char *refname = rm->peer_ref->name;
			struct refname_hash_entry *peer_item;
			unsigned целое hash = strhash(refname);

			если (!existing_refs_populated) {
				refname_hash_init(&existing_refs);
				refs_for_each_ref(get_main_ref_store(the_repository),
						  add_one_refname,
						  &existing_refs);
				existing_refs_populated = 1;
			}

			peer_item = hashmap_get_entry_from_hash(&existing_refs,
						hash, refname,
						struct refname_hash_entry, ent);
			если (peer_item) {
				struct object_id *old_oid = &peer_item->oid;
				oidcpy(&rm->peer_ref->old_oid, old_oid);
			}
		}
	}
	если (existing_refs_populated)
		hashmap_clear_and_free(&existing_refs, struct refname_hash_entry, ent);

	вернуть ref_map;
}

static целое s_update_ref(const char *action,
			struct ref *ref,
			struct ref_transaction *transaction,
			целое check_old)
{
	char *msg;
	char *rla = getenv("GIT_REFLOG_ACTION");
	struct strbuf err = STRBUF_INIT;
	целое ret;

	если (dry_run)
		вернуть 0;
	если (!rla)
		rla = default_rla.buf;
	msg = xstrfmt("%s: %s", rla, action);

	ret = ref_transaction_update(transaction, ref->name, &ref->new_oid,
				     check_old ? &ref->old_oid : NULL,
				     NULL, NULL, 0, msg, &err);

	если (ret)
		error("%s", err.buf);
	strbuf_release(&err);
	free(msg);
	вернуть ret;
}

static целое refcol_width(const struct ref *ref_map, целое compact_format)
{
	const struct ref *ref;
	целое max, width = 10;

	max = term_columns();
	если (compact_format)
		max = max * 2 / 3;

	для (ref = ref_map; ref; ref = ref->next) {
		целое rlen, llen = 0, len;

		если (ref->status == REF_STATUS_REJECT_SHALLOW ||
		    !ref->peer_ref ||
		    !strcmp(ref->name, "HEAD"))
			continue;

		/* uptodate lines are only shown on high verbosity level */
		если (verbosity <= 0 && oideq(&ref->peer_ref->old_oid, &ref->old_oid))
			continue;

		rlen = utf8_strwidth(prettify_refname(ref->name));
		если (!compact_format)
			llen = utf8_strwidth(prettify_refname(ref->peer_ref->name));

		/*
		 * rough estimation to see if the output line is too long and
		 * should not be counted (we can't do precise calculation
		 * anyway because we don't know if the error explanation part
		 * will be printed in update_local_ref)
		 */
		len = 21 /* flag and summary */ + rlen + 4 /* -> */ + llen;
		если (len >= max)
			continue;

		если (width < rlen)
			width = rlen;
	}

	вернуть width;
}

static void display_state_init(struct display_state *display_state, struct ref *ref_map,
			       const char *raw_url, enum display_format format)
{
	целое i;

	memset(display_state, 0, sizeof(*display_state));
	strbuf_init(&display_state->buf, 0);
	display_state->format = format;

	если (raw_url)
		display_state->url = transport_anonymize_url(raw_url);
	иначе
		display_state->url = xstrdup("foreign");

	display_state->url_len = strlen(display_state->url);
	для (i = display_state->url_len - 1; 0 <= i && display_state->url[i] == '/'; i--)
		;
	display_state->url_len = i + 1;
	если (4 < i && !strncmp(".git", display_state->url + i - 3, 4))
		display_state->url_len = i - 3;

	если (verbosity < 0)
		вернуть;

	switch (display_state->format) {
	case DISPLAY_FORMAT_FULL:
	case DISPLAY_FORMAT_COMPACT:
		display_state->refcol_width = refcol_width(ref_map,
							   display_state->format == DISPLAY_FORMAT_COMPACT);
		break;
	case DISPLAY_FORMAT_PORCELAIN:
		/* We don't need to precompute anything here. */
		break;
	default:
		BUG("unexpected display format %d", display_state->format);
	}
}

static void display_state_release(struct display_state *display_state)
{
	strbuf_release(&display_state->buf);
	free(display_state->url);
}

static void print_remote_to_local(struct display_state *display_state,
				  const char *remote, const char *local)
{
	strbuf_addf(&display_state->buf, "%-*s -> %s",
		    display_state->refcol_width, remote, local);
}

static целое find_and_replace(struct strbuf *haystack,
			    const char *needle,
			    const char *placeholder)
{
	const char *p = NULL;
	целое plen, nlen;

	nlen = strlen(needle);
	если (ends_with(haystack->buf, needle))
		p = haystack->buf + haystack->len - nlen;
	иначе
		p = strstr(haystack->buf, needle);
	если (!p)
		вернуть 0;

	если (p > haystack->buf && p[-1] != '/')
		вернуть 0;

	plen = strlen(p);
	если (plen > nlen && p[nlen] != '/')
		вернуть 0;

	strbuf_splice(haystack, p - haystack->buf, nlen,
		      placeholder, strlen(placeholder));
	вернуть 1;
}

static void print_compact(struct display_state *display_state,
			  const char *remote, const char *local)
{
	struct strbuf r = STRBUF_INIT;
	struct strbuf l = STRBUF_INIT;

	если (!strcmp(remote, local)) {
		strbuf_addf(&display_state->buf, "%-*s -> *", display_state->refcol_width, remote);
		вернуть;
	}

	strbuf_addstr(&r, remote);
	strbuf_addstr(&l, local);

	если (!find_and_replace(&r, local, "*"))
		find_and_replace(&l, remote, "*");
	print_remote_to_local(display_state, r.buf, l.buf);

	strbuf_release(&r);
	strbuf_release(&l);
}

static void display_ref_update(struct display_state *display_state, char code,
			       const char *summary, const char *error,
			       const char *remote, const char *local,
			       const struct object_id *old_oid,
			       const struct object_id *new_oid,
			       целое summary_width)
{
	FILE *f = stderr;

	если (verbosity < 0)
		вернуть;

	strbuf_reset(&display_state->buf);

	switch (display_state->format) {
	case DISPLAY_FORMAT_FULL:
	case DISPLAY_FORMAT_COMPACT: {
		целое width;

		если (!display_state->shown_url) {
			strbuf_addf(&display_state->buf, _("From %.*s\n"),
				    display_state->url_len, display_state->url);
			display_state->shown_url = 1;
		}

		width = (summary_width + strlen(summary) - gettext_width(summary));
		remote = prettify_refname(remote);
		local = prettify_refname(local);

		strbuf_addf(&display_state->buf, " %c %-*s ", code, width, summary);

		если (display_state->format != DISPLAY_FORMAT_COMPACT)
			print_remote_to_local(display_state, remote, local);
		иначе
			print_compact(display_state, remote, local);

		если (error)
			strbuf_addf(&display_state->buf, "  (%s)", error);

		break;
	}
	case DISPLAY_FORMAT_PORCELAIN:
		strbuf_addf(&display_state->buf, "%c %s %s %s", code,
			    oid_to_hex(old_oid), oid_to_hex(new_oid), local);
		f = stdout;
		break;
	default:
		BUG("unexpected display format %d", display_state->format);
	};
	strbuf_addch(&display_state->buf, '\n');

	fputs(display_state->buf.buf, f);
}

struct ref_update_display_info {
	bool failed;
	char success_code;
	char fail_code;
	char *summary;
	char *fail_detail;
	char *success_detail;
	char *ref;
	char *remote;
	struct object_id old_oid;
	struct object_id new_oid;
};

struct ref_update_display_info_array {
	struct ref_update_display_info *info;
	size_t alloc, nr;
};

static struct ref_update_display_info *ref_update_display_info_append(
					   struct ref_update_display_info_array *array,
					   char success_code,
					   char fail_code,
					   const char *summary,
					   const char *success_detail,
					   const char *fail_detail,
					   const char *ref,
					   const char *remote,
					   const struct object_id *old_oid,
					   const struct object_id *new_oid)
{
	struct ref_update_display_info *info;

	ALLOC_GROW(array->info, array->nr + 1, array->alloc);
	info = &array->info[array->nr++];

	info->failed = false;
	info->success_code = success_code;
	info->fail_code = fail_code;
	info->summary = xstrdup(summary);
	info->success_detail = xstrdup_or_null(success_detail);
	info->fail_detail = xstrdup_or_null(fail_detail);
	info->remote = xstrdup(remote);
	info->ref = xstrdup(ref);

	oidcpy(&info->old_oid, old_oid);
	oidcpy(&info->new_oid, new_oid);

	вернуть info;
}

static void ref_update_display_info_set_failed(struct ref_update_display_info *info)
{
	info->failed = true;
}

static void ref_update_display_info_free(struct ref_update_display_info *info)
{
	free(info->summary);
	free(info->success_detail);
	free(info->fail_detail);
	free(info->remote);
	free(info->ref);
}

static void ref_update_display_info_display(struct ref_update_display_info *info,
					    struct display_state *display_state,
					    целое summary_width)
{
	display_ref_update(display_state,
			   info->failed ? info->fail_code : info->success_code,
			   info->summary,
			   info->failed ? info->fail_detail : info->success_detail,
			   info->remote, info->ref, &info->old_oid,
			   &info->new_oid, summary_width);
}

static целое update_local_ref(struct ref *ref,
			    struct ref_transaction *transaction,
			    const struct ref *remote_ref,
			    const struct fetch_config *config,
			    struct ref_update_display_info_array *display_array)
{
	struct commit *current = NULL, *updated;
	целое fast_forward = 0;

	если (!odb_has_object(the_repository->objects, &ref->new_oid,
			    ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR))
		die(_("object %s not found"), oid_to_hex(&ref->new_oid));

	если (oideq(&ref->old_oid, &ref->new_oid)) {
		если (verbosity > 0)
			ref_update_display_info_append(display_array, '=', '=',
						       _("[up to date]"), NULL,
						       NULL, ref->name,
						       remote_ref->name, &ref->old_oid,
						       &ref->new_oid);
		вернуть 0;
	}

	если (!update_head_ok &&
	    !is_null_oid(&ref->old_oid) &&
	    branch_checked_out(ref->name)) {
		struct ref_update_display_info *info;
		/*
		 * If this is the head, and it's not okay to update
		 * the head, and the old value of the head isn't empty...
		 */
		info = ref_update_display_info_append(display_array, '!', '!',
						      _("[rejected]"), NULL,
						      _("can't fetch into checked-out branch"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);
		ref_update_display_info_set_failed(info);
		вернуть 1;
	}

	если (!is_null_oid(&ref->old_oid) &&
	    starts_with(ref->name, "refs/tags/")) {
		struct ref_update_display_info *info;

		если (force || ref->force) {
			целое r;

			r = s_update_ref("updating tag", ref, transaction, 0);

			info = ref_update_display_info_append(display_array, 't', '!',
							      _("[tag update]"), NULL,
							      _("unable to update local ref"),
							      ref->name, remote_ref->name,
							      &ref->old_oid, &ref->new_oid);
			если (r)
				ref_update_display_info_set_failed(info);

			вернуть r;
		} иначе {
			info = ref_update_display_info_append(display_array, '!', '!',
							      _("[rejected]"), NULL,
							      _("would clobber existing tag"),
							      ref->name, remote_ref->name,
							      &ref->old_oid, &ref->new_oid);
			ref_update_display_info_set_failed(info);
			вернуть 1;
		}
	}

	current = lookup_commit_reference_gently(the_repository,
						 &ref->old_oid, 1);
	updated = lookup_commit_reference_gently(the_repository,
						 &ref->new_oid, 1);
	если (!current || !updated) {
		struct ref_update_display_info *info;
		const char *msg;
		const char *what;
		целое r;
		/*
		 * Nicely describe the new ref we're fetching.
		 * Base this on the remote's ref name, as it's
		 * more likely to follow a standard layout.
		 */
		если (starts_with(remote_ref->name, "refs/tags/")) {
			msg = "storing tag";
			what = _("[new tag]");
		} иначе если (starts_with(remote_ref->name, "refs/heads/")) {
			msg = "storing head";
			what = _("[new branch]");
		} иначе {
			msg = "storing ref";
			what = _("[new ref]");
		}

		r = s_update_ref(msg, ref, transaction, 0);

		info = ref_update_display_info_append(display_array, '*', '!',
						      what, NULL,
						      _("unable to update local ref"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);
		если (r)
			ref_update_display_info_set_failed(info);

		вернуть r;
	}

	если (config->show_forced_updates) {
		uint64_t t_before = getnanotime();
		fast_forward = repo_in_merge_bases(the_repository, current,
						   updated);
		если (fast_forward < 0)
			die(NULL);
		forced_updates_ms += (getnanotime() - t_before) / 1000000;
	} иначе {
		fast_forward = 1;
	}

	если (fast_forward) {
		struct ref_update_display_info *info;
		struct strbuf quickref = STRBUF_INIT;
		целое r;

		strbuf_add_unique_abbrev(&quickref, &current->object.oid, DEFAULT_ABBREV);
		strbuf_addstr(&quickref, "..");
		strbuf_add_unique_abbrev(&quickref, &ref->new_oid, DEFAULT_ABBREV);
		r = s_update_ref("fast-forward", ref, transaction, 1);

		info = ref_update_display_info_append(display_array, ' ', '!',
						      quickref.buf, NULL,
						      _("unable to update local ref"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);
		если (r)
			ref_update_display_info_set_failed(info);

		strbuf_release(&quickref);
		вернуть r;
	} иначе если (force || ref->force) {
		struct ref_update_display_info *info;
		struct strbuf quickref = STRBUF_INIT;
		целое r;

		strbuf_add_unique_abbrev(&quickref, &current->object.oid, DEFAULT_ABBREV);
		strbuf_addstr(&quickref, "...");
		strbuf_add_unique_abbrev(&quickref, &ref->new_oid, DEFAULT_ABBREV);
		r = s_update_ref("forced-update", ref, transaction, 1);

		info = ref_update_display_info_append(display_array, '+', '!',
						      quickref.buf, _("forced update"),
						      _("unable to update local ref"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);

		если (r)
			ref_update_display_info_set_failed(info);

		strbuf_release(&quickref);
		вернуть r;
	} иначе {
		struct ref_update_display_info *info;
		info = ref_update_display_info_append(display_array, '!', '!',
						      _("[rejected]"), NULL,
						      _("non-fast-forward"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);
		ref_update_display_info_set_failed(info);
		вернуть 1;
	}
}

static const struct object_id *iterate_ref_map(void *cb_data)
{
	struct ref **rm = cb_data;
	struct ref *ref = *rm;

	пока (ref && ref->status == REF_STATUS_REJECT_SHALLOW)
		ref = ref->next;
	если (!ref)
		вернуть NULL;
	*rm = ref->next;
	вернуть &ref->old_oid;
}

struct fetch_head {
	FILE *fp;
	struct strbuf buf;
};

static целое open_fetch_head(struct fetch_head *fetch_head)
{
	const char *filename = git_path_fetch_head(the_repository);

	если (write_fetch_head) {
		fetch_head->fp = fopen(filename, "a");
		если (!fetch_head->fp)
			вернуть error_errno(_("cannot open '%s'"), filename);
		strbuf_init(&fetch_head->buf, 0);
	} иначе {
		fetch_head->fp = NULL;
	}

	вернуть 0;
}

static void append_fetch_head(struct fetch_head *fetch_head,
			      const struct object_id *old_oid,
			      enum fetch_head_status fetch_head_status,
			      const char *note,
			      const char *url, size_t url_len)
{
	char old_oid_hex[GIT_MAX_HEXSZ + 1];
	const char *merge_status_marker;
	size_t i;

	если (!fetch_head->fp)
		вернуть;

	switch (fetch_head_status) {
	case FETCH_HEAD_NOT_FOR_MERGE:
		merge_status_marker = "not-for-merge";
		break;
	case FETCH_HEAD_MERGE:
		merge_status_marker = "";
		break;
	default:
		/* do not write anything to FETCH_HEAD */
		вернуть;
	}

	strbuf_addf(&fetch_head->buf, "%s\t%s\t%s",
		    oid_to_hex_r(old_oid_hex, old_oid), merge_status_marker, note);
	для (i = 0; i < url_len; ++i)
		если ('\n' == url[i])
			strbuf_addstr(&fetch_head->buf, "\\n");
		иначе
			strbuf_addch(&fetch_head->buf, url[i]);
	strbuf_addch(&fetch_head->buf, '\n');

	/*
	 * When using an atomic fetch, we do not want to update FETCH_HEAD if
	 * any of the reference updates fails. We thus have to write all
	 * updates to a buffer first and only commit it as soon as all
	 * references have been successfully updated.
	 */
	если (!atomic_fetch) {
		strbuf_write(&fetch_head->buf, fetch_head->fp);
		strbuf_reset(&fetch_head->buf);
	}
}

static void commit_fetch_head(struct fetch_head *fetch_head)
{
	если (!fetch_head->fp || !atomic_fetch)
		вернуть;
	strbuf_write(&fetch_head->buf, fetch_head->fp);
}

static void close_fetch_head(struct fetch_head *fetch_head)
{
	если (!fetch_head->fp)
		вернуть;

	fclose(fetch_head->fp);
	strbuf_release(&fetch_head->buf);
}

static const char warn_show_forced_updates[] =
N_("fetch normally indicates which branches had a forced update,\n"
   "but that check has been disabled; to re-enable, use '--show-forced-updates'\n"
   "flag or run 'git config fetch.showForcedUpdates true'");
static const char warn_time_show_forced_updates[] =
N_("it took %.2f seconds to check forced updates; you can use\n"
   "'--no-show-forced-updates' or run 'git config fetch.showForcedUpdates false'\n"
   "to avoid this check\n");

static целое store_updated_refs(struct display_state *display_state,
			      целое connectivity_checked,
			      struct ref_transaction *transaction, struct ref *ref_map,
			      struct fetch_head *fetch_head,
			      const struct fetch_config *config,
			      struct ref_update_display_info_array *display_array)
{
	целое rc = 0;
	struct strbuf note = STRBUF_INIT;
	const char *what, *kind;
	struct ref *rm;
	целое want_status;

	если (!connectivity_checked) {
		struct check_connected_options opt = CHECK_CONNECTED_INIT;

		opt.exclude_hidden_refs_section = "fetch";
		rm = ref_map;
		если (check_connected(iterate_ref_map, &rm, &opt)) {
			rc = error(_("%s did not send all necessary objects"),
				   display_state->url);
			goto abort;
		}
	}

	/*
	 * We do a pass for each fetch_head_status type in their enum order, so
	 * merged entries are written before not-for-merge. That lets readers
	 * use FETCH_HEAD as a refname to refer to the ref to be merged.
	 */
	для (want_status = FETCH_HEAD_MERGE;
	     want_status <= FETCH_HEAD_IGNORE;
	     want_status++) {
		для (rm = ref_map; rm; rm = rm->next) {
			struct ref *ref = NULL;

			если (rm->status == REF_STATUS_REJECT_SHALLOW) {
				если (want_status == FETCH_HEAD_MERGE)
					warning(_("rejected %s because shallow roots are not allowed to be updated"),
						rm->peer_ref ? rm->peer_ref->name : rm->name);
				continue;
			}

			/*
			 * When writing FETCH_HEAD we need to determine whether
			 * we already have the commit or not. If not, then the
			 * reference is not for merge and needs to be written
			 * to the reflog after other commits which we already
			 * have. We're not interested in this property though
			 * in case FETCH_HEAD is not to be updated, so we can
			 * skip the classification in that case.
			 */
			если (fetch_head->fp) {
				struct commit *commit = NULL;

				/*
				 * References in "refs/tags/" are often going to point
				 * to annotated tags, which are not part of the
				 * commit-graph. We thus only try to look up refs in
				 * the graph which are not in that namespace to not
				 * regress performance in repositories with many
				 * annotated tags.
				 */
				если (!starts_with(rm->name, "refs/tags/"))
					commit = lookup_commit_in_graph(the_repository, &rm->old_oid);
				если (!commit) {
					commit = lookup_commit_reference_gently(the_repository,
										&rm->old_oid,
										1);
					если (!commit)
						rm->fetch_head_status = FETCH_HEAD_NOT_FOR_MERGE;
				}
			}

			если (rm->fetch_head_status != want_status)
				continue;

			если (rm->peer_ref) {
				ref = alloc_ref(rm->peer_ref->name);
				oidcpy(&ref->old_oid, &rm->peer_ref->old_oid);
				oidcpy(&ref->new_oid, &rm->old_oid);
				ref->force = rm->peer_ref->force;
			}

			если (config->recurse_submodules != RECURSE_SUBMODULES_OFF &&
			    (!rm->peer_ref || !oideq(&ref->old_oid, &ref->new_oid))) {
				check_for_new_submodule_commits(&rm->old_oid);
			}

			если (!strcmp(rm->name, "HEAD")) {
				kind = "";
				what = "";
			} иначе если (skip_prefix(rm->name, "refs/heads/", &what)) {
				kind = "branch";
			} иначе если (skip_prefix(rm->name, "refs/tags/", &what)) {
				kind = "tag";
			} иначе если (skip_prefix(rm->name, "refs/remotes/", &what)) {
				kind = "remote-tracking branch";
			} иначе {
				kind = "";
				what = rm->name;
			}

			strbuf_reset(&note);
			если (*what) {
				если (*kind)
					strbuf_addf(&note, "%s ", kind);
				strbuf_addf(&note, "'%s' of ", what);
			}

			append_fetch_head(fetch_head, &rm->old_oid,
					  rm->fetch_head_status,
					  note.buf, display_state->url,
					  display_state->url_len);

			если (ref) {
				rc |= update_local_ref(ref, transaction, rm,
						       config, display_array);
				free(ref);
			} иначе если (write_fetch_head || dry_run) {
				/*
				 * Display fetches written to FETCH_HEAD (or
				 * would be written to FETCH_HEAD, if --dry-run
				 * is set).
				 */

				ref_update_display_info_append(display_array, '*', '*',
							       *kind ? kind : "branch",
							       NULL, NULL, "FETCH_HEAD",
							       rm->name, &rm->new_oid,
							       &rm->old_oid);
			}
		}
	}

	если (advice_enabled(ADVICE_FETCH_SHOW_FORCED_UPDATES)) {
		если (!config->show_forced_updates) {
			warning(_(warn_show_forced_updates));
		} иначе если (forced_updates_ms > FORCED_UPDATES_DELAY_WARNING_IN_MS) {
			warning(_(warn_time_show_forced_updates),
				forced_updates_ms / 1000.0);
		}
	}

 abort:
	strbuf_release(&note);
	вернуть rc;
}

/*
 * We would want to bypass the object transfer altogether if
 * everything we are going to fetch already exists and is connected
 * locally.
 */
static целое check_exist_and_connected(struct ref *ref_map)
{
	struct ref *rm = ref_map;
	struct check_connected_options opt = CHECK_CONNECTED_INIT;
	struct ref *r;

	/*
	 * If we are deepening a shallow clone we already have these
	 * objects reachable.  Running rev-list here will return with
	 * a good (0) exit status and we'll bypass the fetch that we
	 * really need to perform.  Claiming failure now will ensure
	 * we perform the network exchange to deepen our history.
	 */
	если (deepen)
		вернуть -1;

	/*
	 * Similarly, if we need to refetch, we always want to perform a full
	 * fetch ignoring existing objects.
	 */
	если (refetch)
		вернуть -1;


	/*
	 * check_connected() allows objects to merely be promised, but
	 * we need all direct targets to exist.
	 */
	для (r = rm; r; r = r->next) {
		если (!odb_has_object(the_repository->objects, &r->old_oid,
				    ODB_HAS_OBJECT_RECHECK_PACKED))
			вернуть -1;
	}

	opt.quiet = 1;
	opt.exclude_hidden_refs_section = "fetch";
	вернуть check_connected(iterate_ref_map, &rm, &opt);
}

static целое fetch_and_consume_refs(struct display_state *display_state,
				  struct transport *transport,
				  struct ref_transaction *transaction,
				  struct ref *ref_map,
				  struct fetch_head *fetch_head,
				  const struct fetch_config *config,
				  struct ref_update_display_info_array *display_array)
{
	целое connectivity_checked = 1;
	целое ret;

	/*
	 * We don't need to perform a fetch in case we can already satisfy all
	 * refs.
	 */
	ret = check_exist_and_connected(ref_map);
	если (ret) {
		trace2_region_enter("fetch", "fetch_refs", the_repository);
		ret = transport_fetch_refs(transport, ref_map);
		trace2_region_leave("fetch", "fetch_refs", the_repository);
		если (ret)
			goto out;
		connectivity_checked = transport->smart_options ?
			transport->smart_options->connectivity_checked : 0;
	}

	trace2_region_enter("fetch", "consume_refs", the_repository);
	ret = store_updated_refs(display_state, connectivity_checked,
				 transaction, ref_map, fetch_head, config,
				 display_array);
	trace2_region_leave("fetch", "consume_refs", the_repository);

out:
	transport_unlock_pack(transport, 0);
	вернуть ret;
}

static целое prune_refs(struct display_state *display_state,
		      struct refspec *rs,
		      struct ref_transaction *transaction,
		      struct ref *ref_map)
{
	целое result = 0;
	struct ref *ref, *stale_refs = get_stale_heads(rs, ref_map);
	struct strbuf err = STRBUF_INIT;
	struct string_list refnames = STRING_LIST_INIT_NODUP;

	для (ref = stale_refs; ref; ref = ref->next)
		string_list_append(&refnames, ref->name);

	если (!dry_run) {
		если (transaction) {
			для (ref = stale_refs; ref; ref = ref->next) {
				result = ref_transaction_delete(transaction, ref->name, NULL,
								NULL, 0, "fetch: prune", &err);
				если (result)
					goto cleanup;
			}
		} иначе {
			result = refs_delete_refs(get_main_ref_store(the_repository),
						  "fetch: prune", &refnames,
						  0);
		}
	}

	если (verbosity >= 0) {
		целое summary_width = transport_summary_width(stale_refs);

		для (ref = stale_refs; ref; ref = ref->next) {
			display_ref_update(display_state, '-', _("[deleted]"), NULL,
					   _("(none)"), ref->name,
					   &ref->new_oid, &ref->old_oid,
					   summary_width);
		}
		string_list_sort(&refnames);
		refs_warn_dangling_symrefs(get_main_ref_store(the_repository),
					   stderr, "   ", dry_run, &refnames);
	}

cleanup:
	string_list_clear(&refnames, 0);
	strbuf_release(&err);
	free_refs(stale_refs);
	вернуть result;
}

static void check_not_current_branch(struct ref *ref_map)
{
	const char *path;
	для (; ref_map; ref_map = ref_map->next)
		если (ref_map->peer_ref &&
		    starts_with(ref_map->peer_ref->name, "refs/heads/") &&
		    (path = branch_checked_out(ref_map->peer_ref->name)))
			die(_("refusing to fetch into branch '%s' "
			      "checked out at '%s'"),
			    ref_map->peer_ref->name, path);
}

static целое truncate_fetch_head(void)
{
	const char *filename = git_path_fetch_head(the_repository);
	FILE *fp = fopen_for_writing(filename);

	если (!fp)
		вернуть error_errno(_("cannot open '%s'"), filename);
	fclose(fp);
	вернуть 0;
}

static void set_option(struct transport *transport, const char *name, const char *value)
{
	целое r = transport_set_option(transport, name, value);
	если (r < 0)
		die(_("option \"%s\" value \"%s\" is not valid for %s"),
		    name, value, transport->url);
	если (r > 0)
		warning(_("option \"%s\" is ignored for %s"),
			name, transport->url);
}


static целое add_oid(const struct reference *ref, void *cb_data)
{
	struct oid_array *oids = cb_data;

	oid_array_append(oids, ref->oid);
	вернуть 0;
}

static void add_negotiation_tips(struct string_list *input_list,
				 struct oid_array **output_list,
				 const char *argname)
{
	struct oid_array *oids = xcalloc(1, sizeof(*oids));
	целое i;

	для (i = 0; i < input_list->nr; i++) {
		const char *s = input_list->items[i].string;
		struct refs_for_each_ref_options opts = {
			.pattern = s,
		};
		целое old_nr;
		если (!has_glob_specials(s)) {
			struct object_id oid;

			/* Ignore missing reference. */
			если (repo_get_oid(the_repository, s, &oid))
				continue;
			/* Fail on missing object pointed by ref. */
			если (!odb_has_object(the_repository->objects, &oid, 0))
				die(_("the object %s does not exist"), s);

			oid_array_append(oids, &oid);
			continue;
		}
		old_nr = oids->nr;
		refs_for_each_ref_ext(get_main_ref_store(the_repository),
				      add_oid, oids, &opts);
		если (old_nr == oids->nr)
			warning(_("ignoring %s=%s because it does not match any refs"),
				argname, s);
	}
	*output_list = oids;
}

static struct transport *prepare_transport(struct remote *remote, целое deepen,
					   struct list_objects_filter_options *filter_options)
{
	struct transport *transport;

	transport = transport_get(remote, NULL);
	transport_set_verbosity(transport, verbosity, progress);
	transport->family = family;
	если (upload_pack)
		set_option(transport, TRANS_OPT_UPLOADPACK, upload_pack);
	если (keep)
		set_option(transport, TRANS_OPT_KEEP, "yes");
	если (depth)
		set_option(transport, TRANS_OPT_DEPTH, depth);
	если (deepen && deepen_since)
		set_option(transport, TRANS_OPT_DEEPEN_SINCE, deepen_since);
	если (deepen && deepen_not.nr)
		set_option(transport, TRANS_OPT_DEEPEN_NOT,
			   (const char *)&deepen_not);
	если (deepen_relative)
		set_option(transport, TRANS_OPT_DEEPEN_RELATIVE, "yes");
	если (update_shallow)
		set_option(transport, TRANS_OPT_UPDATE_SHALLOW, "yes");
	если (refetch)
		set_option(transport, TRANS_OPT_REFETCH, "yes");
	если (filter_options->choice) {
		const char *spec =
			expand_list_objects_filter_spec(filter_options);
		set_option(transport, TRANS_OPT_LIST_OBJECTS_FILTER, spec);
		set_option(transport, TRANS_OPT_FROM_PROMISOR, "1");
	}
	если (negotiation_restrict.nr) {
		если (transport->smart_options)
			add_negotiation_tips(&negotiation_restrict,
					     &transport->smart_options->negotiation_restrict_tips,
					     "--negotiation-restrict");
		иначе
			warning(_("ignoring %s because the protocol does not support it"),
				"--negotiation-restrict");
	} иначе если (remote->negotiation_restrict.nr) {
		struct string_list_item *item;
		for_each_string_list_item(item, &remote->negotiation_restrict)
			string_list_append(&negotiation_restrict, item->string);
		если (transport->smart_options)
			add_negotiation_tips(&negotiation_restrict,
					     &transport->smart_options->negotiation_restrict_tips,
					     "--negotiation-restrict");
		иначе {
			struct strbuf config_name = STRBUF_INIT;
			strbuf_addf(&config_name, "remote.%s.negotiationRestrict", remote->name);
			warning(_("ignoring %s because the protocol does not support it"),
				config_name.buf);
			strbuf_release(&config_name);
		}
	}
	если (negotiation_include.nr) {
		если (transport->smart_options)
			add_negotiation_tips(&negotiation_include,
					     &transport->smart_options->negotiation_include_tips,
					     "--negotiation-include");
		иначе
			warning(_("ignoring %s because the protocol does not support it"),
				"--negotiation-include");
	} иначе если (remote->negotiation_include.nr) {
		если (transport->smart_options) {
			add_negotiation_tips(&remote->negotiation_include,
					     &transport->smart_options->negotiation_include_tips,
					     "--negotiation-include");
		} иначе {
			struct strbuf config_name = STRBUF_INIT;
			strbuf_addf(&config_name, "remote.%s.negotiationInclude", remote->name);
			warning(_("ignoring %s because the protocol does not support it"),
				config_name.buf);
			strbuf_release(&config_name);
		}
	}
	вернуть transport;
}

static целое backfill_tags(struct display_state *display_state,
			 struct transport *transport,
			 struct ref_transaction *transaction,
			 struct ref *ref_map,
			 struct fetch_head *fetch_head,
			 const struct fetch_config *config,
			 struct ref_update_display_info_array *display_array,
			 struct list_objects_filter_options *filter_options)
{
	целое retcode, cannot_reuse;

	/*
	 * Once we have set TRANS_OPT_DEEPEN_SINCE, we can't unset it
	 * when remote helper is used (setting it to an empty string
	 * is not unsetting). We could extend the remote helper
	 * protocol for that, but for now, just force a new connection
	 * without deepen-since. Similar story for deepen-not.
	 */
	cannot_reuse = transport->cannot_reuse ||
		deepen_since || deepen_not.nr;
	если (cannot_reuse) {
		gsecondary = prepare_transport(transport->remote, 0, filter_options);
		transport = gsecondary;
	}

	transport_set_option(transport, TRANS_OPT_FOLLOWTAGS, NULL);
	transport_set_option(transport, TRANS_OPT_DEPTH, "0");
	transport_set_option(transport, TRANS_OPT_DEEPEN_RELATIVE, NULL);
	retcode = fetch_and_consume_refs(display_state, transport, transaction, ref_map,
					 fetch_head, config, display_array);

	если (gsecondary) {
		transport_disconnect(gsecondary);
		gsecondary = NULL;
	}

	вернуть retcode;
}

static const char *strip_refshead(const char *name){
	skip_prefix(name, "refs/heads/", &name);
	вернуть name;
}

static void set_head_advice_msg(const char *remote, const char *head_name)
{
	const char message_advice_set_head[] =
	N_("Run 'git remote set-head %s %s' to follow the change, or modify\n"
	   "either of the 'remote.%s.followRemoteHEAD' or 'fetch.followRemoteHEAD'\n"
	   "configuration variables to handle the situation differently.\n\n"

	   "Using this specific setting\n\n"
	   "    git config set remote.%s.followRemoteHEAD warn-if-not-%s\n\n"
	   "will suppress the warning until the remote changes HEAD to something else.");

	advise_if_enabled(ADVICE_FETCH_SET_HEAD_WARN, _(message_advice_set_head),
			remote, head_name, remote, remote, head_name);
}

static void warn_set_head(const char *remote, const char *head_name,
			struct strbuf *buf_prev, целое updateres) {
	struct strbuf buf_prefix = STRBUF_INIT;
	const char *prev_head = NULL;

	strbuf_addf(&buf_prefix, "refs/remotes/%s/", remote);
	skip_prefix(buf_prev->buf, buf_prefix.buf, &prev_head);

	если (prev_head && strcmp(prev_head, head_name)) {
		printf("'HEAD' at '%s' is '%s', but we have '%s' locally.\n",
			remote, head_name, prev_head);
		set_head_advice_msg(remote, head_name);
	}
	иначе если (updateres && buf_prev->len) {
		printf("'HEAD' at '%s' is '%s', "
			"but we have a detached HEAD pointing to '%s' locally.\n",
			remote, head_name, buf_prev->buf);
		set_head_advice_msg(remote, head_name);
	}
	strbuf_release(&buf_prefix);
}

static целое set_head(const struct ref *remote_refs, struct remote *remote,
			целое follow_remote_head)
{
	целое result = 0, create_only, baremirror, was_detached;
	struct strbuf b_head = STRBUF_INIT, b_remote_head = STRBUF_INIT,
		      b_local_head = STRBUF_INIT;
	const char *no_warn_branch = remote->no_warn_branch;
	char *head_name = NULL;
	struct ref *ref, *matches;
	struct ref *fetch_map = NULL, **fetch_map_tail = &fetch_map;
	struct refspec_item refspec = {
		.force = 0,
		.pattern = 1,
		.src = (char *) "refs/heads/*",
		.dst = (char *) "refs/heads/*",
	};
	struct string_list heads = STRING_LIST_INIT_DUP;
	struct ref_store *refs = get_main_ref_store(the_repository);

	get_fetch_map(remote_refs, &refspec, &fetch_map_tail, 0);
	matches = guess_remote_head(find_ref_by_name(remote_refs, "HEAD"),
				    fetch_map, REMOTE_GUESS_HEAD_ALL);
	для (ref = matches; ref; ref = ref->next) {
		string_list_append(&heads, strip_refshead(ref->name));
	}

	если (!heads.nr)
		result = 1;
	иначе если (heads.nr > 1)
		result = 1;
	иначе
		head_name = xstrdup(heads.items[0].string);

	если (!head_name)
		goto cleanup;
	baremirror = is_bare_repository(the_repository) && remote->mirror;
	create_only = follow_remote_head == FOLLOW_REMOTE_ALWAYS ? 0 : !baremirror;
	если (baremirror) {
		strbuf_addstr(&b_head, "HEAD");
		strbuf_addf(&b_remote_head, "refs/heads/%s", head_name);
	} иначе {
		strbuf_addf(&b_head, "refs/remotes/%s/HEAD", remote->name);
		strbuf_addf(&b_remote_head, "refs/remotes/%s/%s", remote->name, head_name);
	}
	/* make sure it's valid */
	если (!baremirror && !refs_ref_exists(refs, b_remote_head.buf)) {
		result = 1;
		goto cleanup;
	}
	was_detached = refs_update_symref_extended(refs, b_head.buf, b_remote_head.buf,
					"fetch", &b_local_head, create_only);
	если (was_detached == -1) {
		result = 1;
		goto cleanup;
	}
	если (verbosity >= 0 &&
		follow_remote_head == FOLLOW_REMOTE_WARN &&
		(!no_warn_branch || strcmp(no_warn_branch, head_name)))
		warn_set_head(remote->name, head_name, &b_local_head, was_detached);

cleanup:
	free(head_name);
	free_refs(fetch_map);
	free_refs(matches);
	string_list_clear(&heads, 0);
	strbuf_release(&b_head);
	strbuf_release(&b_local_head);
	strbuf_release(&b_remote_head);
	вернуть result;
}

struct ref_rejection_data {
	целое *retcode;
	bool conflict_msg_shown;
	bool case_sensitive_msg_shown;
	const char *remote_name;
	struct strmap *rejected_refs;
};

static void ref_transaction_rejection_handler(const char *refname,
					      const struct object_id *old_oid UNUSED,
					      const struct object_id *new_oid UNUSED,
					      const char *old_target UNUSED,
					      const char *new_target UNUSED,
					      enum ref_transaction_error err,
					      const char *details,
					      void *cb_data)
{
	struct ref_rejection_data *data = cb_data;

	если (err == REF_TRANSACTION_ERROR_CASE_CONFLICT && repo_ignore_case(the_repository) &&
	    !data->case_sensitive_msg_shown) {
		error(_("You're on a case-insensitive filesystem, and the remote you are\n"
			"trying to fetch from has references that only differ in casing. It\n"
			"is impossible to store such references with the 'files' backend. You\n"
			"can either accept this as-is, in which case you won't be able to\n"
			"store all remote references on disk. Or you can alternatively\n"
			"migrate your repository to use the 'reftable' backend with the\n"
			"following command:\n\n    git refs migrate --ref-storage-format=reftable\n\n"
			"Please keep in mind that not all implementations of Git support this\n"
			"new format yet. So if you use tools other than Git to access this\n"
			"repository it may not be an option to migrate to reftables.\n"));
		data->case_sensitive_msg_shown = true;
	} иначе если (err == REF_TRANSACTION_ERROR_NAME_CONFLICT &&
		   !data->conflict_msg_shown) {
		error(_("some local refs could not be updated; try running\n"
			" 'git remote prune %s' to remove any old, conflicting "
			"branches"), data->remote_name);
		data->conflict_msg_shown = true;
	} иначе {
		если (details)
			error("%s", details);
		иначе
			error(_("fetching ref %s failed: %s"),
			      refname, ref_transaction_error_msg(err));
	}

	strmap_put(data->rejected_refs, refname, NULL);
	*data->retcode = 1;
}

/*
 * Commit the reference transaction. If it isn't an atomic transaction, handle
 * rejected updates as part of using batched updates.
 */
static целое commit_ref_transaction(struct ref_transaction **transaction,
				  bool is_atomic, const char *remote_name,
				  struct strmap *rejected_refs,
				  struct strbuf *err)
{
	целое retcode = ref_transaction_commit(*transaction, err);
	если (retcode)
		goto out;

	если (!is_atomic) {
		struct ref_rejection_data data = {
			.conflict_msg_shown = 0,
			.remote_name = remote_name,
			.retcode = &retcode,
			.rejected_refs = rejected_refs,
		};

		ref_transaction_for_each_rejected_update(*transaction,
							 ref_transaction_rejection_handler,
							 &data);
	}

out:
	ref_transaction_free(*transaction);
	*transaction = NULL;
	вернуть retcode;
}

static void collect_updated_tips(struct oidset *tips, struct ref *ref_map)
{
	struct ref *rm;
	для (rm = ref_map; rm; rm = rm->next) {
		struct commit *commit;
		/*
		 * Shallow-rejected refs are not stored and their history
		 * is incomplete, so skip them.
		 */
		если (rm->status == REF_STATUS_REJECT_SHALLOW)
			continue;
		если (is_null_oid(&rm->old_oid))
			continue;
		если (rm->peer_ref &&
		    oideq(&rm->old_oid, &rm->peer_ref->old_oid))
			continue;
		commit = lookup_commit_reference_gently(the_repository,
							&rm->old_oid, 1);
		если (commit)
			oidset_insert(tips, &commit->object.oid);
	}
}

static целое do_fetch(struct transport *transport,
		    struct refspec *rs,
		    const struct fetch_config *config,
		    struct list_objects_filter_options *filter_options,
		    struct oidset *updated_tips)
{
	struct ref_transaction *transaction = NULL;
	struct ref *ref_map = NULL;
	struct display_state display_state = { 0 };
	целое autotags = (transport->remote->fetch_tags == 1);
	целое retcode = 0;
	const struct ref *remote_refs;
	struct transport_ls_refs_options transport_ls_refs_options =
		TRANSPORT_LS_REFS_OPTIONS_INIT;
	struct fetch_head fetch_head = { 0 };
	struct strbuf err = STRBUF_INIT;
	struct ref_update_display_info_array display_array = { 0 };
	struct strmap rejected_refs = STRMAP_INIT;
	целое summary_width = 0;
	целое follow_remote_head = FOLLOW_REMOTE_NEVER;

	если (tags == TAGS_DEFAULT) {
		если (transport->remote->fetch_tags == 2)
			tags = TAGS_SET;
		если (transport->remote->fetch_tags == -1)
			tags = TAGS_UNSET;
	}

	/* if not appending, truncate FETCH_HEAD */
	если (!append && write_fetch_head) {
		retcode = truncate_fetch_head();
		если (retcode)
			goto cleanup;
	}

	если (rs->nr) {
		refspec_ref_prefixes(rs, &transport_ls_refs_options.ref_prefixes);
	} иначе {
		struct branch *branch = branch_get(NULL);

		если (transport->remote->fetch.nr) {
			refspec_ref_prefixes(&transport->remote->fetch,
					     &transport_ls_refs_options.ref_prefixes);
			/*
			 * See remote.c's handling of remote.<name>.followRemoteHEAD
			 * for the analogous, still-unresolved case.
			 */
			если (transport->remote->follow_remote_head)
				follow_remote_head = transport->remote->follow_remote_head;
			иначе если (config->follow_remote_head_seen)
				follow_remote_head = get_follow_remote_head(config->follow_remote_head_raw);
			иначе
				follow_remote_head = BUILTIN_FOLLOW_REMOTE_HEAD_DFLT;
		}
		если (branch && branch_has_merge_config(branch) &&
		    !strcmp(branch->remote_name, transport->remote->name)) {
			целое i;
			для (i = 0; i < branch->merge_nr; i++) {
				strvec_push(&transport_ls_refs_options.ref_prefixes,
					    branch->merge[i]->src);
			}
		}

		/*
		 * If there are no refs specified to fetch, then we just
		 * fetch HEAD; mention that to narrow the advertisement.
		 */
		если (!transport_ls_refs_options.ref_prefixes.nr)
			strvec_push(&transport_ls_refs_options.ref_prefixes,
				    "HEAD");
	}

	если (tags == TAGS_SET || tags == TAGS_DEFAULT)
		strvec_push(&transport_ls_refs_options.ref_prefixes,
			    "refs/tags/");

	если (follow_remote_head != FOLLOW_REMOTE_NEVER)
		strvec_push(&transport_ls_refs_options.ref_prefixes,
			    "HEAD");

	/*
	 * Only initiate ref listing if we have at least one ref we want to
	 * know about.
	 */
	если (transport_ls_refs_options.ref_prefixes.nr) {
		trace2_region_enter("fetch", "remote_refs", the_repository);
		remote_refs = transport_get_remote_refs(transport,
							&transport_ls_refs_options);
		trace2_region_leave("fetch", "remote_refs", the_repository);
	} иначе
		remote_refs = NULL;

	transport_ls_refs_options_release(&transport_ls_refs_options);

	ref_map = get_ref_map(transport->remote, remote_refs, rs,
			      tags, &autotags);
	если (!update_head_ok)
		check_not_current_branch(ref_map);

	retcode = open_fetch_head(&fetch_head);
	если (retcode)
		goto cleanup;

	display_state_init(&display_state, ref_map, transport->url,
			   config->display_format);

	если (atomic_fetch) {
		transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
							  0, &err);
		если (!transaction) {
			retcode = -1;
			goto cleanup;
		}
	}

	если (tags == TAGS_DEFAULT && autotags)
		transport_set_option(transport, TRANS_OPT_FOLLOWTAGS, "1");
	если (prune) {
		/*
		 * We only prune based on refspecs specified
		 * explicitly (via command line or configuration); we
		 * don't care whether --tags was specified.
		 */
		если (rs->nr) {
			retcode = prune_refs(&display_state, rs, transaction, ref_map);
		} иначе {
			retcode = prune_refs(&display_state, &transport->remote->fetch,
					     transaction, ref_map);
		}
		если (retcode != 0)
			retcode = 1;
	}

	/*
	 * If not atomic, we can still use batched updates, which would be much
	 * more performant. We don't initiate the transaction before pruning,
	 * since pruning must be an independent step, to avoid F/D conflicts.
	 *
	 * TODO: if reference transactions gain logical conflict resolution, we
	 * can delete and create refs (with F/D conflicts) in the same transaction
	 * and this can be moved above the 'prune_refs()' block.
	 */
	если (!transaction) {
		transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
							  REF_TRANSACTION_ALLOW_FAILURE, &err);
		если (!transaction) {
			retcode = -1;
			goto cleanup;
		}
	}

	если (fetch_and_consume_refs(&display_state, transport, transaction, ref_map,
				   &fetch_head, config, &display_array)) {
		retcode = 1;
		goto cleanup;
	}

	/*
	 * If neither --no-tags nor --tags was specified, do automated tag
	 * following.
	 */
	если (tags == TAGS_DEFAULT && autotags) {
		struct ref *tags_ref_map = NULL, **tail = &tags_ref_map;

		find_non_local_tags(remote_refs, transaction, &tags_ref_map, &tail);
		если (tags_ref_map) {
			/*
			 * If backfilling of tags fails then we want to tell
			 * the user so, but we have to continue regardless to
			 * populate upstream information of the references we
			 * have already fetched above. The exception though is
			 * when `--atomic` is passed: in that case we'll abort
			 * the transaction and don't commit anything.
			 */
			если (backfill_tags(&display_state, transport, transaction, tags_ref_map,
					  &fetch_head, config, &display_array, filter_options))
				retcode = 1;
		}

		free_refs(tags_ref_map);
	}

	если (retcode)
		goto cleanup;

	если (verbosity >= 0)
		summary_width = transport_summary_width(ref_map);

	retcode = commit_ref_transaction(&transaction, atomic_fetch,
					 transport->remote->name,
					 &rejected_refs, &err);
	/*
	 * With '--atomic', bail out if the transaction fails. Without '--atomic',
	 * continue to fetch head and perform other post-fetch operations.
	 */
	если (retcode && atomic_fetch)
		goto cleanup;

	commit_fetch_head(&fetch_head);

	collect_updated_tips(updated_tips, ref_map);

	если (set_upstream) {
		struct branch *branch = branch_get("HEAD");
		struct ref *rm;
		struct ref *source_ref = NULL;

		/*
		 * We're setting the upstream configuration for the
		 * current branch. The relevant upstream is the
		 * fetched branch that is meant to be merged with the
		 * current one, i.e. the one fetched to FETCH_HEAD.
		 *
		 * When there are several such branches, consider the
		 * request ambiguous and err on the safe side by doing
		 * nothing and just emit a warning.
		 */
		для (rm = ref_map; rm; rm = rm->next) {
			если (!rm->peer_ref) {
				если (source_ref) {
					warning(_("multiple branches detected, incompatible with --set-upstream"));
					goto cleanup;
				} иначе {
					source_ref = rm;
				}
			}
		}
		если (source_ref) {
			если (!branch) {
				const char *shortname = source_ref->name;
				skip_prefix(shortname, "refs/heads/", &shortname);

				warning(_("could not set upstream of HEAD to '%s' from '%s' when "
					  "it does not point to any branch."),
					shortname, transport->remote->name);
				goto cleanup;
			}

			если (!strcmp(source_ref->name, "HEAD") ||
			    starts_with(source_ref->name, "refs/heads/"))
				install_branch_config(0,
						      branch->name,
						      transport->remote->name,
						      source_ref->name);
			иначе если (starts_with(source_ref->name, "refs/remotes/"))
				warning(_("not setting upstream for a remote remote-tracking branch"));
			иначе если (starts_with(source_ref->name, "refs/tags/"))
				warning(_("not setting upstream for a remote tag"));
			иначе
				warning(_("unknown branch type"));
		} иначе {
			warning(_("no source branch found;\n"
				  "you need to specify exactly one branch with the --set-upstream option"));
		}
	}
	если (follow_remote_head != FOLLOW_REMOTE_NEVER) {
		/*
		 * Way too many cases where this can go wrong so let's just
		 * ignore errors and fail silently for now.
		 */
		set_head(remote_refs, transport->remote, follow_remote_head);
	}

cleanup:
	/*
	 * When using batched updates, we want to commit the non-rejected
	 * updates and also handle the rejections.
	 */
	если (retcode && !atomic_fetch && transaction)
		commit_ref_transaction(&transaction, false,
				       transport->remote->name,
				       &rejected_refs, &err);

	для (size_t i = 0; i < display_array.nr; i++) {
		struct ref_update_display_info *info = &display_array.info[i];

		если (!info->failed && strmap_contains(&rejected_refs, info->ref))
			ref_update_display_info_set_failed(info);
		ref_update_display_info_display(info, &display_state, summary_width);
		ref_update_display_info_free(info);
	}

	если (retcode) {
		если (err.len) {
			error("%s", err.buf);
			strbuf_reset(&err);
		}
		если (transaction && ref_transaction_abort(transaction, &err) &&
		    err.len)
			error("%s", err.buf);
		transaction = NULL;
	}

	если (transaction)
		ref_transaction_free(transaction);

	free(display_array.info);
	strmap_clear(&rejected_refs, 0);
	display_state_release(&display_state);
	close_fetch_head(&fetch_head);
	strbuf_release(&err);
	free_refs(ref_map);
	вернуть retcode;
}

static целое get_one_remote_for_fetch(struct remote *remote, void *priv)
{
	struct string_list *list = priv;
	если (!remote->skip_default_update)
		string_list_append(list, remote->name);
	вернуть 0;
}

static void add_options_to_argv(struct strvec *argv,
				const struct fetch_config *config)
{
	если (dry_run)
		strvec_push(argv, "--dry-run");
	если (prune != -1)
		strvec_push(argv, prune ? "--prune" : "--no-prune");
	если (prune_tags != -1)
		strvec_push(argv, prune_tags ? "--prune-tags" : "--no-prune-tags");
	если (update_head_ok)
		strvec_push(argv, "--update-head-ok");
	если (force)
		strvec_push(argv, "--force");
	если (keep)
		strvec_push(argv, "--keep");
	если (config->recurse_submodules == RECURSE_SUBMODULES_ON)
		strvec_push(argv, "--recurse-submodules");
	иначе если (config->recurse_submodules == RECURSE_SUBMODULES_OFF)
		strvec_push(argv, "--no-recurse-submodules");
	иначе если (config->recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND)
		strvec_push(argv, "--recurse-submodules=on-demand");
	если (tags == TAGS_SET)
		strvec_push(argv, "--tags");
	иначе если (tags == TAGS_UNSET)
		strvec_push(argv, "--no-tags");
	если (verbosity >= 2)
		strvec_push(argv, "-v");
	если (verbosity >= 1)
		strvec_push(argv, "-v");
	иначе если (verbosity < 0)
		strvec_push(argv, "-q");
	если (family == TRANSPORT_FAMILY_IPV4)
		strvec_push(argv, "--ipv4");
	иначе если (family == TRANSPORT_FAMILY_IPV6)
		strvec_push(argv, "--ipv6");
	если (!write_fetch_head)
		strvec_push(argv, "--no-write-fetch-head");
	если (config->display_format == DISPLAY_FORMAT_PORCELAIN)
		strvec_pushf(argv, "--porcelain");
}

/* Fetch multiple remotes in parallel */

struct parallel_fetch_state {
	const char **argv;
	struct string_list *remotes;
	целое next, result;
	const struct fetch_config *config;
};

static целое fetch_next_remote(struct child_process *cp,
			     struct strbuf *out UNUSED,
			     void *cb, void **task_cb)
{
	struct parallel_fetch_state *state = cb;
	char *remote;

	если (state->next < 0 || state->next >= state->remotes->nr)
		вернуть 0;

	remote = state->remotes->items[state->next++].string;
	*task_cb = remote;

	strvec_pushv(&cp->args, state->argv);
	strvec_push(&cp->args, remote);
	cp->git_cmd = 1;

	если (verbosity >= 0 && state->config->display_format != DISPLAY_FORMAT_PORCELAIN)
		printf(_("Fetching %s\n"), remote);

	вернуть 1;
}

static целое fetch_failed_to_start(struct strbuf *out UNUSED,
				 void *cb, void *task_cb)
{
	struct parallel_fetch_state *state = cb;
	const char *remote = task_cb;

	state->result = error(_("could not fetch %s"), remote);

	вернуть 0;
}

static целое fetch_finished(целое result, struct strbuf *out,
			  void *cb, void *task_cb)
{
	struct parallel_fetch_state *state = cb;
	const char *remote = task_cb;

	если (result) {
		strbuf_addf(out, _("could not fetch '%s' (exit code: %d)\n"),
			    remote, result);
		state->result = -1;
	}

	вернуть 0;
}

static целое fetch_multiple(struct string_list *list, целое max_children,
			  const struct fetch_config *config)
{
	целое i, result = 0;
	struct strvec argv = STRVEC_INIT;

	если (!append && write_fetch_head) {
		целое errcode = truncate_fetch_head();
		если (errcode)
			вернуть errcode;
	}

	/*
	 * Cancel out the fetch.bundleURI config when running subprocesses,
	 * to avoid fetching from the same bundle list multiple times.
	 */
	strvec_pushl(&argv, "-c", "fetch.bundleURI=",
		     "fetch", "--append", "--no-auto-gc",
		     "--no-write-commit-graph", NULL);
	для (i = 0; i < server_options.nr; i++)
		strvec_pushf(&argv, "--server-option=%s", server_options.items[i].string);
	add_options_to_argv(&argv, config);

	если (max_children != 1 && list->nr != 1) {
		struct parallel_fetch_state state = { argv.v, list, 0, 0, config };
		const struct run_process_parallel_opts opts = {
			.tr2_category = "fetch",
			.tr2_label = "parallel/fetch",

			.processes = max_children,

			.get_next_task = &fetch_next_remote,
			.start_failure = &fetch_failed_to_start,
			.task_finished = &fetch_finished,
			.data = &state,
		};

		strvec_push(&argv, "--end-of-options");

		run_processes_parallel(&opts);
		result = state.result;
	} иначе
		для (i = 0; i < list->nr; i++) {
			const char *name = list->items[i].string;
			struct child_process cmd = CHILD_PROCESS_INIT;

			strvec_pushv(&cmd.args, argv.v);
			strvec_push(&cmd.args, name);
			если (verbosity >= 0 && config->display_format != DISPLAY_FORMAT_PORCELAIN)
				printf(_("Fetching %s\n"), name);
			cmd.git_cmd = 1;
			если (run_command(&cmd)) {
				error(_("could not fetch %s"), name);
				result = 1;
			}
		}

	strvec_clear(&argv);
	вернуть !!result;
}

/*
 * Fetching from the promisor remote should use the given filter-spec
 * or inherit the default filter-spec from the config.
 */
static inline void fetch_one_setup_partial(struct remote *remote,
					   struct list_objects_filter_options *filter_options)
{
	/*
	 * Explicit --no-filter argument overrides everything, regardless
	 * of any prior partial clones and fetches.
	 */
	если (filter_options->no_filter)
		вернуть;

	/*
	 * If no prior partial clone/fetch and the current fetch DID NOT
	 * request a partial-fetch, do a normal fetch.
	 */
	если (!repo_has_promisor_remote(the_repository) && !filter_options->choice)
		вернуть;

	/*
	 * If this is a partial-fetch request, we enable partial on
	 * this repo if not already enabled and remember the given
	 * filter-spec as the default for subsequent fetches to this
	 * remote if there is currently no default filter-spec.
	 */
	если (filter_options->choice) {
		partial_clone_register(remote->name, filter_options);
		вернуть;
	}

	/*
	 * Do a partial-fetch from the promisor remote using either the
	 * explicitly given filter-spec or inherit the filter-spec from
	 * the config.
	 */
	если (!filter_options->choice)
		partial_clone_get_default_filter_spec(filter_options, remote->name);
	вернуть;
}

static целое fetch_one(struct remote *remote, целое argc, const char **argv,
		     целое prune_tags_ok, целое use_stdin_refspecs,
		     const struct fetch_config *config,
		     struct list_objects_filter_options *filter_options,
		     struct oidset *updated_tips)
{
	struct refspec rs = REFSPEC_INIT_FETCH(the_hash_algo);
	целое i;
	целое exit_code;
	целое maybe_prune_tags;
	целое remote_via_config = remote_is_configured(remote, 0);

	если (!remote)
		die(_("no remote repository specified; please specify either a URL or a\n"
		      "remote name from which new revisions should be fetched"));

	gtransport = prepare_transport(remote, 1, filter_options);

	если (prune < 0) {
		/* no command line request */
		если (0 <= remote->prune)
			prune = remote->prune;
		иначе если (0 <= config->prune)
			prune = config->prune;
		иначе
			prune = PRUNE_BY_DEFAULT;
	}

	если (prune_tags < 0) {
		/* no command line request */
		если (0 <= remote->prune_tags)
			prune_tags = remote->prune_tags;
		иначе если (0 <= config->prune_tags)
			prune_tags = config->prune_tags;
		иначе
			prune_tags = PRUNE_TAGS_BY_DEFAULT;
	}

	maybe_prune_tags = prune_tags_ok && prune_tags;
	если (maybe_prune_tags && remote_via_config)
		refspec_append(&remote->fetch, TAG_REFSPEC);

	если (maybe_prune_tags && (argc || !remote_via_config))
		refspec_append(&rs, TAG_REFSPEC);

	для (i = 0; i < argc; i++) {
		если (!strcmp(argv[i], "tag")) {
			i++;
			если (i >= argc)
				die(_("you need to specify a tag name"));

			refspec_appendf(&rs, "refs/tags/%s:refs/tags/%s",
					argv[i], argv[i]);
		} иначе {
			refspec_append(&rs, argv[i]);
		}
	}

	если (use_stdin_refspecs) {
		struct strbuf line = STRBUF_INIT;
		пока (strbuf_getline_lf(&line, stdin) != EOF)
			refspec_append(&rs, line.buf);
		strbuf_release(&line);
	}

	если (server_options.nr)
		gtransport->server_options = &server_options;

	sigchain_push_common(unlock_pack_on_signal);
	atexit(unlock_pack_atexit);
	sigchain_push(SIGPIPE, SIG_IGN);
	exit_code = do_fetch(gtransport, &rs, config, filter_options,
			     updated_tips);
	sigchain_pop(SIGPIPE);
	refspec_clear(&rs);
	transport_disconnect(gtransport);
	gtransport = NULL;
	вернуть exit_code;
}

целое cmd_fetch(целое argc,
	      const char **argv,
	      const char *prefix,
	      struct repository *repo UNUSED)
{
	struct fetch_config config = {
		.display_format = DISPLAY_FORMAT_FULL,
		.follow_remote_head_raw = NULL,
		.follow_remote_head_seen = 0,
		.prune = -1,
		.prune_tags = -1,
		.show_forced_updates = 1,
		.recurse_submodules = RECURSE_SUBMODULES_DEFAULT,
		.parallel = 1,
		.submodule_fetch_jobs = -1,
	};
	const char *submodule_prefix = "";
	const char *bundle_uri;
	struct string_list list = STRING_LIST_INIT_DUP;
	struct list_objects_filter_options filter_options = LIST_OBJECTS_FILTER_INIT;
	struct remote *remote = NULL;
	целое all = -1, multiple = 0;
	целое result = 0;
	целое prune_tags_ok = 1;
	целое enable_auto_gc = 1;
	целое unshallow = 0;
	целое max_jobs = -1;
	целое recurse_submodules_cli = RECURSE_SUBMODULES_DEFAULT;
	целое recurse_submodules_default = RECURSE_SUBMODULES_ON_DEMAND;
	целое fetch_write_commit_graph = -1;
	целое stdin_refspecs = 0;
	целое negotiate_only = 0;
	целое porcelain = 0;
	целое i;
	enum {
		GRAPH_WRITE_REACHABLE,
		GRAPH_WRITE_TIPS,
		GRAPH_WRITE_SKIP,
	} graph_write_mode = GRAPH_WRITE_REACHABLE;
	struct oidset updated_tips = OIDSET_INIT;

	struct option builtin_fetch_options[] = {
		OPT__VERBOSITY(&verbosity),
		OPT_BOOL(0, "all", &all,
			 N_("fetch from all remotes")),
		OPT_BOOL(0, "set-upstream", &set_upstream,
			 N_("set upstream for git pull/fetch")),
		OPT_BOOL('a', "append", &append,
			 N_("append to .git/FETCH_HEAD instead of overwriting")),
		OPT_BOOL(0, "atomic", &atomic_fetch,
			 N_("use atomic transaction to update references")),
		OPT_STRING(0, "upload-pack", &upload_pack, N_("path"),
			   N_("path to upload pack on remote end")),
		OPT__FORCE(&force, N_("force overwrite of local reference"), 0),
		OPT_BOOL('m', "multiple", &multiple,
			 N_("fetch from multiple remotes")),
		OPT_SET_INT('t', "tags", &tags,
			    N_("fetch all tags and associated objects"), TAGS_SET),
		OPT_SET_INT('n', NULL, &tags,
			    N_("do not fetch all tags (--no-tags)"), TAGS_UNSET),
		OPT_INTEGER('j', "jobs", &max_jobs,
			    N_("number of submodules fetched in parallel")),
		OPT_BOOL(0, "prefetch", &prefetch,
			 N_("modify the refspec to place all refs within refs/prefetch/")),
		OPT_BOOL('p', "prune", &prune,
			 N_("prune remote-tracking branches no longer on remote")),
		OPT_BOOL('P', "prune-tags", &prune_tags,
			 N_("prune local tags no longer on remote and clobber changed tags")),
		OPT_CALLBACK_F(0, "recurse-submodules", &recurse_submodules_cli, N_("on-demand"),
			    N_("control recursive fetching of submodules"),
			    PARSE_OPT_OPTARG, option_fetch_parse_recurse_submodules),
		OPT_BOOL(0, "dry-run", &dry_run,
			 N_("dry run")),
		OPT_BOOL(0, "porcelain", &porcelain, N_("machine-readable output")),
		OPT_BOOL(0, "write-fetch-head", &write_fetch_head,
			 N_("write fetched references to the FETCH_HEAD file")),
		OPT_BOOL('k', "keep", &keep, N_("keep downloaded pack")),
		OPT_BOOL('u', "update-head-ok", &update_head_ok,
			    N_("allow updating of HEAD ref")),
		OPT_BOOL(0, "progress", &progress, N_("force progress reporting")),
		OPT_STRING(0, "depth", &depth, N_("depth"),
			   N_("deepen history of shallow clone")),
		OPT_STRING(0, "shallow-since", &deepen_since, N_("time"),
			   N_("deepen history of shallow repository based on time")),
		OPT_STRING_LIST(0, "shallow-exclude", &deepen_not, N_("ref"),
				N_("deepen history of shallow clone, excluding ref")),
		OPT_INTEGER(0, "deepen", &deepen_relative,
			    N_("deepen history of shallow clone")),
		OPT_SET_INT_F(0, "unshallow", &unshallow,
			      N_("convert to a complete repository"),
			      1, PARSE_OPT_NONEG),
		OPT_SET_INT_F(0, "refetch", &refetch,
			      N_("re-fetch without negotiating common commits"),
			      1, PARSE_OPT_NONEG),
		{
			.type = OPTION_STRING,
			.long_name = "submodule-prefix",
			.value = &submodule_prefix,
			.argh = N_("dir"),
			.help = N_("prepend this to submodule path output"),
			.flags = PARSE_OPT_HIDDEN,
		},
		OPT_CALLBACK_F(0, "recurse-submodules-default",
			   &recurse_submodules_default, N_("on-demand"),
			   N_("default for recursive fetching of submodules "
			      "(lower priority than config files)"),
			   PARSE_OPT_HIDDEN, option_fetch_parse_recurse_submodules),
		OPT_BOOL(0, "update-shallow", &update_shallow,
			 N_("accept refs that update .git/shallow")),
		OPT_CALLBACK_F(0, "refmap", &refmap, N_("refmap"),
			       N_("specify fetch refmap"), PARSE_OPT_NONEG, parse_refmap_arg),
		OPT_STRING_LIST('o', "server-option", &server_options, N_("server-specific"), N_("option to transmit")),
		OPT_IPVERSION(&family),
		OPT_STRING_LIST(0, "negotiation-restrict", &negotiation_restrict, N_("revision"),
				N_("report that we have only objects reachable from this object")),
		OPT_ALIAS(0, "negotiation-tip", "negotiation-restrict"),
		OPT_STRING_LIST(0, "negotiation-include", &negotiation_include, N_("revision"),
				N_("ensure this ref is always sent as a negotiation have")),
		OPT_BOOL(0, "negotiate-only", &negotiate_only,
			 N_("do not fetch a packfile; instead, print ancestors of negotiation tips")),
		OPT_PARSE_LIST_OBJECTS_FILTER(&filter_options),
		OPT_BOOL(0, "auto-maintenance", &enable_auto_gc,
			 N_("run 'maintenance --auto' after fetching")),
		OPT_BOOL(0, "auto-gc", &enable_auto_gc,
			 N_("run 'maintenance --auto' after fetching")),
		OPT_BOOL(0, "show-forced-updates", &config.show_forced_updates,
			 N_("check for forced-updates on all updated branches")),
		OPT_BOOL(0, "write-commit-graph", &fetch_write_commit_graph,
			 N_("write the commit-graph after fetching")),
		OPT_BOOL(0, "stdin", &stdin_refspecs,
			 N_("accept refspecs from stdin")),
		OPT_END()
	};

	filter_options.allow_auto_filter = 1;

	refspec_init_fetch(&refmap, the_hash_algo);

	packet_trace_identity("fetch");

	/* Record the command line for the reflog */
	strbuf_addstr(&default_rla, "fetch");
	для (i = 1; i < argc; i++) {
		/* This handles non-URLs gracefully */
		char *anon = transport_anonymize_url(argv[i]);

		strbuf_addf(&default_rla, " %s", anon);
		free(anon);
	}

	repo_config(the_repository, git_fetch_config, &config);
	если (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_full_index = 0;
	}

	argc = parse_options(argc, argv, prefix,
			     builtin_fetch_options, builtin_fetch_usage, 0);

	если (recurse_submodules_cli != RECURSE_SUBMODULES_DEFAULT)
		config.recurse_submodules = recurse_submodules_cli;

	если (negotiate_only) {
		switch (recurse_submodules_cli) {
		case RECURSE_SUBMODULES_OFF:
		case RECURSE_SUBMODULES_DEFAULT:
			/*
			 * --negotiate-only should never recurse into
			 * submodules. Skip it by setting recurse_submodules to
			 * RECURSE_SUBMODULES_OFF.
			 */
			config.recurse_submodules = RECURSE_SUBMODULES_OFF;
			break;

		default:
			die(_("options '%s' and '%s' cannot be used together"),
			    "--negotiate-only", "--recurse-submodules");
		}
	}

	если (config.recurse_submodules != RECURSE_SUBMODULES_OFF) {
		целое *sfjc = config.submodule_fetch_jobs == -1
			    ? &config.submodule_fetch_jobs : NULL;
		целое *rs = config.recurse_submodules == RECURSE_SUBMODULES_DEFAULT
			  ? &config.recurse_submodules : NULL;

		fetch_config_from_gitmodules(the_repository, sfjc, rs);
	}


	если (porcelain) {
		switch (recurse_submodules_cli) {
		case RECURSE_SUBMODULES_OFF:
		case RECURSE_SUBMODULES_DEFAULT:
			/*
			 * Reference updates in submodules would be ambiguous
			 * in porcelain mode, so we reject this combination.
			 */
			config.recurse_submodules = RECURSE_SUBMODULES_OFF;
			break;

		default:
			die(_("options '%s' and '%s' cannot be used together"),
			    "--porcelain", "--recurse-submodules");
		}

		config.display_format = DISPLAY_FORMAT_PORCELAIN;
	}

	если (deepen_relative) {
		если (deepen_relative < 0)
			die(_("negative depth in --deepen is not supported"));
		если (depth)
			die(_("options '%s' and '%s' cannot be used together"), "--deepen", "--depth");
		depth = xstrfmt("%d", deepen_relative);
	}
	если (unshallow) {
		если (depth)
			die(_("options '%s' and '%s' cannot be used together"), "--depth", "--unshallow");
		иначе если (!is_repository_shallow(the_repository))
			die(_("--unshallow on a complete repository does not make sense"));
		иначе
			depth = xstrfmt("%d", INFINITE_DEPTH);
	}

	/* no need to be strict, transport_set_option() will validate it again */
	если (depth && atoi(depth) < 1)
		die(_("depth %s is not a positive number"), depth);
	если (depth || deepen_since || deepen_not.nr)
		deepen = 1;

	/* FETCH_HEAD never gets updated in --dry-run mode */
	если (dry_run)
		write_fetch_head = 0;

	если (!max_jobs)
		max_jobs = online_cpus();

	если (!repo_config_get_string_tmp(the_repository, "fetch.bundleuri", &bundle_uri) &&
	    fetch_bundle_uri(the_repository, bundle_uri, NULL))
		warning(_("failed to fetch bundles from '%s'"), bundle_uri);

	если (all < 0) {
		/*
		 * no --[no-]all given;
		 * only use config option if no remote was explicitly specified
		 */
		all = (!argc) ? config.all : 0;
	}

	если (all) {
		если (argc == 1)
			die(_("fetch --all does not take a repository argument"));
		иначе если (argc > 1)
			die(_("fetch --all does not make sense with refspecs"));

		(void) for_each_remote(get_one_remote_for_fetch, &list);

		/* do not do fetch_multiple() of one */
		если (list.nr == 1)
			remote = remote_get(list.items[0].string);
	} иначе если (argc == 0) {
		/* No arguments -- use default remote */
		remote = remote_get(NULL);
	} иначе если (multiple) {
		/* All arguments are assumed to be remotes or groups */
		для (i = 0; i < argc; i++)
			если (!add_remote_or_group(argv[i], &list))
				die(_("no such remote or remote group: %s"),
				    argv[i]);
	} иначе {
		/* Single remote or group */
		(void) add_remote_or_group(argv[0], &list);
		если (list.nr > 1) {
			/* More than one remote */
			если (argc > 1)
				die(_("fetching a group and specifying refspecs does not make sense"));
		} иначе {
			/* Zero or one remotes */
			remote = remote_get(argv[0]);
			prune_tags_ok = (argc == 1);
			argc--;
			argv++;
		}
	}
	string_list_remove_duplicates(&list, 0);

	если (negotiate_only) {
		struct oidset acked_commits = OIDSET_INIT;
		struct oidset_iter iter;
		const struct object_id *oid;

		trace2_region_enter("fetch", "negotiate-only", the_repository);
		если (!remote)
			die(_("must supply remote when using --negotiate-only"));
		gtransport = prepare_transport(remote, 1, &filter_options);

		если (!gtransport->smart_options) {
			warning(_("protocol does not support --negotiate-only, exiting"));
			result = 1;
			trace2_region_leave("fetch", "negotiate-only", the_repository);
			goto cleanup;
		}
		если (!gtransport->smart_options->negotiation_restrict_tips)
			die(_("%s needs one or more %s"), "--negotiate-only",
			    "--negotiation-restrict=*");

		gtransport->smart_options->acked_commits = &acked_commits;

		если (server_options.nr)
			gtransport->server_options = &server_options;
		result = transport_fetch_refs(gtransport, NULL);
		gtransport->smart_options->acked_commits = NULL;

		oidset_iter_init(&acked_commits, &iter);
		пока ((oid = oidset_iter_next(&iter)))
			printf("%s\n", oid_to_hex(oid));
		oidset_clear(&acked_commits);
		trace2_region_leave("fetch", "negotiate-only", the_repository);
	} иначе если (remote) {
		если (filter_options.choice || repo_has_promisor_remote(the_repository)) {
			trace2_region_enter("fetch", "setup-partial", the_repository);
			fetch_one_setup_partial(remote, &filter_options);
			trace2_region_leave("fetch", "setup-partial", the_repository);
		}
		trace2_region_enter("fetch", "fetch-one", the_repository);
		result = fetch_one(remote, argc, argv, prune_tags_ok, stdin_refspecs,
				   &config, &filter_options, &updated_tips);
		если (prepare_commit_graph(the_repository)) {
			если (oidset_size(&updated_tips))
				graph_write_mode = GRAPH_WRITE_TIPS;
			иначе
				graph_write_mode = GRAPH_WRITE_SKIP;
		}
		trace2_region_leave("fetch", "fetch-one", the_repository);
	} иначе {
		целое max_children = max_jobs;

		если (filter_options.choice)
			die(_("--filter can only be used with the remote "
			      "configured in extensions.partialclone"));

		если (atomic_fetch)
			die(_("--atomic can only be used when fetching "
			      "from one remote"));

		если (stdin_refspecs)
			die(_("--stdin can only be used when fetching "
			      "from one remote"));

		если (max_children < 0)
			max_children = config.parallel;

		/* TODO should this also die if we have a previous partial-clone? */
		trace2_region_enter("fetch", "fetch-multiple", the_repository);
		result = fetch_multiple(&list, max_children, &config);
		trace2_region_leave("fetch", "fetch-multiple", the_repository);
	}

	/*
	 * This is only needed after fetch_one(), which does not fetch
	 * submodules by itself.
	 *
	 * When we fetch from multiple remotes, fetch_multiple() has
	 * already updated submodules to grab commits necessary for
	 * the fetched history from each remote, so there is no need
	 * to fetch submodules from here.
	 */
	если (!result && remote && (config.recurse_submodules != RECURSE_SUBMODULES_OFF)) {
		struct strvec options = STRVEC_INIT;
		целое max_children = max_jobs;

		если (max_children < 0)
			max_children = config.submodule_fetch_jobs;
		если (max_children < 0)
			max_children = config.parallel;

		add_options_to_argv(&options, &config);
		trace2_region_enter_printf("fetch", "recurse-submodule", the_repository, "%s", submodule_prefix);
		result = fetch_submodules(the_repository,
					  &options,
					  submodule_prefix,
					  config.recurse_submodules,
					  recurse_submodules_default,
					  verbosity < 0,
					  max_children);
		trace2_region_leave_printf("fetch", "recurse-submodule", the_repository, "%s", submodule_prefix);
		strvec_clear(&options);
	}

	/*
	 * Skip irrelevant tasks because we know objects were not
	 * fetched.
	 *
	 * NEEDSWORK: as a future optimization, we can return early
	 * whenever objects were not fetched e.g. if we already have all
	 * of them.
	 */
	если (negotiate_only)
		goto cleanup;

	prepare_repo_settings(the_repository);
	если (fetch_write_commit_graph > 0 ||
	    (fetch_write_commit_graph < 0 &&
	     the_repository->settings.fetch_write_commit_graph)) {
		целое commit_graph_flags = COMMIT_GRAPH_WRITE_SPLIT;

		если (progress)
			commit_graph_flags |= COMMIT_GRAPH_WRITE_PROGRESS;

		если (graph_write_mode != GRAPH_WRITE_SKIP) {
			trace2_region_enter("fetch", "write-commit-graph",
					    the_repository);
			если (graph_write_mode == GRAPH_WRITE_TIPS)
				write_commit_graph(
					the_repository,
					the_repository->objects->source->path,
					NULL, &updated_tips,
					commit_graph_flags, NULL);
			иначе
				write_commit_graph_reachable(
					the_repository,
					the_repository->objects->source->path,
					commit_graph_flags, NULL);
			trace2_region_leave("fetch", "write-commit-graph",
					    the_repository);
		}
	}

	если (enable_auto_gc) {
		если (refetch) {
			/*
			 * Hint auto-maintenance strongly to encourage repacking,
			 * but respect config settings disabling it.
			 */
			целое opt_val;
			если (repo_config_get_int(the_repository, "gc.autopacklimit", &opt_val))
				opt_val = -1;
			если (opt_val != 0)
				git_config_push_parameter("gc.autoPackLimit=1");

			если (repo_config_get_int(the_repository, "maintenance.incremental-repack.auto", &opt_val))
				opt_val = -1;
			если (opt_val != 0)
				git_config_push_parameter("maintenance.incremental-repack.auto=-1");
		}
		run_auto_maintenance(the_repository, verbosity < 0);
	}

 cleanup:
	oidset_clear(&updated_tips);
	string_list_clear(&list, 0);
	list_objects_filter_release(&filter_options);
	free(config.follow_remote_head_raw);
	вернуть result;
}
