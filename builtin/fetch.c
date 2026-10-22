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

	số_nguyên refcol_width;
	enum display_format format;

	char *url;
	số_nguyên url_len, shown_url;
};

static uint64_t forced_updates_ms = 0;
static số_nguyên prefetch = 0;
static số_nguyên prune = -1; /* unspecified */
#define PRUNE_BY_DEFAULT 0 /* do we prune by default? */

static số_nguyên prune_tags = -1; /* unspecified */
#define PRUNE_TAGS_BY_DEFAULT 0 /* do we prune tags by default? */

static số_nguyên append, dry_run, force, keep, update_head_ok;
static số_nguyên write_fetch_head = 1;
static số_nguyên verbosity, deepen_relative, set_upstream, refetch;
static số_nguyên progress = -1;
static số_nguyên tags = TAGS_DEFAULT, update_shallow, deepen;
static số_nguyên atomic_fetch;
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
	số_nguyên follow_remote_head_seen;
	số_nguyên all;
	số_nguyên prune;
	số_nguyên prune_tags;
	số_nguyên show_forced_updates;
	số_nguyên recurse_submodules;
	số_nguyên parallel;
	số_nguyên submodule_fetch_jobs;
};

static số_nguyên git_fetch_config(const char *k, const char *v,
			    const struct config_context *ctx, void *cb)
{
	struct fetch_config *fetch_config = cb;

	nếu (!strcmp(k, "fetch.all")) {
		fetch_config->all = git_config_bool(k, v);
		trả_về 0;
	}

	nếu (!strcmp(k, "fetch.prune")) {
		fetch_config->prune = git_config_bool(k, v);
		trả_về 0;
	}

	nếu (!strcmp(k, "fetch.prunetags")) {
		fetch_config->prune_tags = git_config_bool(k, v);
		trả_về 0;
	}

	nếu (!strcmp(k, "fetch.showforcedupdates")) {
		fetch_config->show_forced_updates = git_config_bool(k, v);
		trả_về 0;
	}

	nếu (!strcmp(k, "submodule.recurse")) {
		số_nguyên r = git_config_bool(k, v) ?
			RECURSE_SUBMODULES_ON : RECURSE_SUBMODULES_OFF;
		fetch_config->recurse_submodules = r;
		trả_về 0;
	}

	nếu (!strcmp(k, "submodule.fetchjobs")) {
		fetch_config->submodule_fetch_jobs = parse_submodule_fetchjobs(k, v, ctx->kvi);
		trả_về 0;
	} không_thì nếu (!strcmp(k, "fetch.recursesubmodules")) {
		fetch_config->recurse_submodules = parse_fetch_recurse_submodules_arg(k, v);
		trả_về 0;
	}

	nếu (!strcmp(k, "fetch.parallel")) {
		fetch_config->parallel = git_config_int(k, v, ctx->kvi);
		nếu (fetch_config->parallel < 0)
			die(_("fetch.parallel cannot be negative"));
		nếu (!fetch_config->parallel)
			fetch_config->parallel = online_cpus();
		trả_về 0;
	}

	nếu (!strcmp(k, "fetch.output")) {
		nếu (!v)
			trả_về config_error_nonbool(k);
		không_thì nếu (!strcasecmp(v, "full"))
			fetch_config->display_format = DISPLAY_FORMAT_FULL;
		không_thì nếu (!strcasecmp(v, "compact"))
			fetch_config->display_format = DISPLAY_FORMAT_COMPACT;
		không_thì
			die(_("invalid value for '%s': '%s'"),
			    "fetch.output", v);
		trả_về 0;
	}

	nếu (!strcmp(k, "fetch.followremotehead")) {
		free(fetch_config->follow_remote_head_raw);
		fetch_config->follow_remote_head_raw = xstrdup_or_null(v);
		fetch_config->follow_remote_head_seen = 1;
		trả_về 0;
	}

	trả_về git_default_config(k, v, ctx, cb);
}

static enum follow_remote_head_settings get_follow_remote_head(const char *setting)
{
	nếu (!setting)
		die(_("missing value for 'fetch.followRemoteHEAD'"));
	không_thì nếu (!strcmp(setting, "never"))
		trả_về FOLLOW_REMOTE_NEVER;
	không_thì nếu (!strcmp(setting, "create"))
		trả_về FOLLOW_REMOTE_CREATE;
	không_thì nếu (!strcmp(setting, "warn"))
		trả_về FOLLOW_REMOTE_WARN;
	không_thì nếu (!strcmp(setting, "always"))
		trả_về FOLLOW_REMOTE_ALWAYS;
	warning(_("unrecognized fetch.followRemoteHEAD value '%s' ignored"), setting);
	trả_về BUILTIN_FOLLOW_REMOTE_HEAD_DFLT;
}

static số_nguyên parse_refmap_arg(const struct option *opt, const char *arg, số_nguyên unset)
{
	BUG_ON_OPT_NEG(unset);

	/*
	 * "git fetch --refmap='' origin foo"
	 * can be used to tell the command not to store anywhere
	 */
	refspec_append(opt->value, arg);

	trả_về 0;
}

static void unlock_pack(unsigned số_nguyên flags)
{
	nếu (gtransport)
		transport_unlock_pack(gtransport, flags);
	nếu (gsecondary)
		transport_unlock_pack(gsecondary, flags);
}

static void unlock_pack_atexit(void)
{
	unlock_pack(0);
}

static void unlock_pack_on_signal(số_nguyên signo)
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
	số_nguyên i;

	cho (i = 0; i < branch->merge_nr; i++) {
		struct ref *rm, **old_tail = *tail;
		struct refspec_item refspec;

		cho (rm = *head; rm; rm = rm->next) {
			nếu (branch_merge_matches(branch, i, rm->name)) {
				rm->fetch_head_status = FETCH_HEAD_MERGE;
				break;
			}
		}
		nếu (rm)
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
		cho (rm = *old_tail; rm; rm = rm->next)
			rm->fetch_head_status = FETCH_HEAD_MERGE;
	}
}

static void create_fetch_oidset(struct ref **head, struct oidset *out)
{
	struct ref *rm = *head;
	trong_khi (rm) {
		oidset_insert(out, &rm->old_oid);
		rm = rm->next;
	}
}

struct refname_hash_entry {
	struct hashmap_entry ent;
	struct object_id oid;
	số_nguyên ignore;
	char refname[FLEX_ARRAY];
};

static số_nguyên refname_hash_entry_cmp(const void *hashmap_cmp_fn_data UNUSED,
				  const struct hashmap_entry *eptr,
				  const struct hashmap_entry *entry_or_key,
				  const void *keydata)
{
	const struct refname_hash_entry *e1, *e2;

	e1 = container_of(eptr, const struct refname_hash_entry, ent);
	e2 = container_of(entry_or_key, const struct refname_hash_entry, ent);
	trả_về strcmp(e1->refname, keydata ? keydata : e2->refname);
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
	trả_về ent;
}

static số_nguyên add_one_refname(const struct reference *ref, void *cbdata)
{
	struct hashmap *refname_map = cbdata;

	(void) refname_hash_add(refname_map, ref->name, ref->oid);
	trả_về 0;
}

static void refname_hash_init(struct hashmap *map)
{
	hashmap_init(map, refname_hash_entry_cmp, NULL, 0);
}

static số_nguyên refname_hash_exists(struct hashmap *map, const char *refname)
{
	trả_về !!hashmap_get_from_hash(map, strhash(refname), refname);
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
	nếu (starts_with(refname, "refs/tags/") && new_oid)
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
	nếu (transaction)
		ref_transaction_for_each_queued_update(transaction,
						       add_already_queued_tags,
						       &existing_refs);

	cho (ref = refs; ref; ref = ref->next) {
		nếu (!starts_with(ref->name, "refs/tags/"))
			continue;

		/*
		 * The peeled ref always follows the matching base
		 * ref, so if we see a peeled ref that we don't want
		 * to fetch then we can mark the ref entry in the list
		 * as one to ignore by setting util to NULL.
		 */
		nếu (ends_with(ref->name, "^{}")) {
			nếu (item &&
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
		nếu (item &&
		    !odb_has_object(the_repository->objects, &item->oid, 0) &&
		    !oidset_contains(&fetch_oids, &item->oid))
			clear_item(item);

		item = NULL;

		/* skip duplicates and refs that we already have */
		nếu (refname_hash_exists(&remote_refs, ref->name) ||
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
	nếu (item &&
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
		unsigned số_nguyên hash = strhash(refname);

		item = hashmap_get_entry_from_hash(&remote_refs, hash, refname,
					struct refname_hash_entry, ent);
		nếu (!item)
			BUG("unseen remote ref?");

		/* Unless we have already decided to ignore this item... */
		nếu (item->ignore)
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
	số_nguyên i;

	nếu (!prefetch)
		trả_về;

	cho (i = 0; i < rs->nr; i++) {
		struct strbuf new_dst = STRBUF_INIT;
		char *old_dst;
		const char *sub = NULL;

		nếu (rs->items[i].negative)
			continue;
		nếu (!rs->items[i].dst ||
		    (rs->items[i].src &&
		     starts_with(rs->items[i].src,
				 ref_namespace[NAMESPACE_TAGS].ref))) {
			số_nguyên j;

			refspec_item_clear(&rs->items[i]);

			cho (j = i + 1; j < rs->nr; j++)
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
		nếu (!skip_prefix(old_dst, "refs/", &sub))
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
			       số_nguyên tags, số_nguyên *autotags)
{
	số_nguyên i;
	struct ref *rm;
	struct ref *ref_map = NULL;
	struct ref **tail = &ref_map;

	/* opportunistically-updated references: */
	struct ref *orefs = NULL, **oref_tail = &orefs;

	struct hashmap existing_refs;
	số_nguyên existing_refs_populated = 0;

	filter_prefetch_refspec(rs);
	nếu (remote)
		filter_prefetch_refspec(&remote->fetch);

	nếu (rs->nr) {
		struct refspec *fetch_refspec;

		cho (i = 0; i < rs->nr; i++) {
			get_fetch_map(remote_refs, &rs->items[i], &tail, 0);
			nếu (rs->items[i].dst && rs->items[i].dst[0])
				*autotags = 1;
		}
		/* Merge everything on the command line (but not --tags) */
		cho (rm = ref_map; rm; rm = rm->next)
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
		nếu (refmap.nr)
			fetch_refspec = &refmap;
		không_thì
			fetch_refspec = &remote->fetch;

		cho (i = 0; i < fetch_refspec->nr; i++)
			get_fetch_map(ref_map, &fetch_refspec->items[i], &oref_tail, 1);
	} không_thì nếu (refmap.nr) {
		die("--refmap option is only meaningful with command-line refspec(s)");
	} không_thì {
		/* Use the defaults */
		struct branch *branch = branch_get(NULL);
		số_nguyên has_merge = branch_has_merge_config(branch);
		nếu (remote &&
		    (remote->fetch.nr ||
		     /* Note: has_merge implies non-NULL branch->remote_name */
		     (has_merge && !strcmp(branch->remote_name, remote->name)))) {
			cho (i = 0; i < remote->fetch.nr; i++) {
				get_fetch_map(remote_refs, &remote->fetch.items[i], &tail, 0);
				nếu (remote->fetch.items[i].dst &&
				    remote->fetch.items[i].dst[0])
					*autotags = 1;
				nếu (!i && !has_merge && ref_map &&
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
			nếu (has_merge &&
			    !strcmp(branch->remote_name, remote->name))
				add_merge_config(&ref_map, remote_refs, branch, &tail);
		} không_thì nếu (!prefetch) {
			ref_map = get_remote_ref(remote_refs, "HEAD");
			nếu (!ref_map)
				die(_("couldn't find remote ref HEAD"));
			ref_map->fetch_head_status = FETCH_HEAD_MERGE;
			tail = &ref_map->next;
		}
	}

	nếu (tags == TAGS_SET) {
		struct refspec_item tag_refspec;

		/* also fetch all tags */
		refspec_item_init_push(&tag_refspec, TAG_REFSPEC,
				       the_hash_algo);
		get_fetch_map(remote_refs, &tag_refspec, &tail, 0);
		refspec_item_clear(&tag_refspec);
	} không_thì nếu (tags == TAGS_DEFAULT && *autotags) {
		find_non_local_tags(remote_refs, NULL, &ref_map, &tail);
	}

	/* Now append any refs to be updated opportunistically: */
	*tail = orefs;
	cho (rm = orefs; rm; rm = rm->next) {
		rm->fetch_head_status = FETCH_HEAD_IGNORE;
		tail = &rm->next;
	}

	/*
	 * apply negative refspecs first, before we remove duplicates. This is
	 * necessary as negative refspecs might remove an otherwise conflicting
	 * duplicate.
	 */
	nếu (rs->nr)
		ref_map = apply_negative_refspecs(ref_map, rs);
	không_thì
		ref_map = apply_negative_refspecs(ref_map, &remote->fetch);

	ref_map = ref_remove_duplicates(ref_map);

	cho (rm = ref_map; rm; rm = rm->next) {
		nếu (rm->peer_ref) {
			const char *refname = rm->peer_ref->name;
			struct refname_hash_entry *peer_item;
			unsigned số_nguyên hash = strhash(refname);

			nếu (!existing_refs_populated) {
				refname_hash_init(&existing_refs);
				refs_for_each_ref(get_main_ref_store(the_repository),
						  add_one_refname,
						  &existing_refs);
				existing_refs_populated = 1;
			}

			peer_item = hashmap_get_entry_from_hash(&existing_refs,
						hash, refname,
						struct refname_hash_entry, ent);
			nếu (peer_item) {
				struct object_id *old_oid = &peer_item->oid;
				oidcpy(&rm->peer_ref->old_oid, old_oid);
			}
		}
	}
	nếu (existing_refs_populated)
		hashmap_clear_and_free(&existing_refs, struct refname_hash_entry, ent);

	trả_về ref_map;
}

static số_nguyên s_update_ref(const char *action,
			struct ref *ref,
			struct ref_transaction *transaction,
			số_nguyên check_old)
{
	char *msg;
	char *rla = getenv("GIT_REFLOG_ACTION");
	struct strbuf err = STRBUF_INIT;
	số_nguyên ret;

	nếu (dry_run)
		trả_về 0;
	nếu (!rla)
		rla = default_rla.buf;
	msg = xstrfmt("%s: %s", rla, action);

	ret = ref_transaction_update(transaction, ref->name, &ref->new_oid,
				     check_old ? &ref->old_oid : NULL,
				     NULL, NULL, 0, msg, &err);

	nếu (ret)
		error("%s", err.buf);
	strbuf_release(&err);
	free(msg);
	trả_về ret;
}

static số_nguyên refcol_width(const struct ref *ref_map, số_nguyên compact_format)
{
	const struct ref *ref;
	số_nguyên max, width = 10;

	max = term_columns();
	nếu (compact_format)
		max = max * 2 / 3;

	cho (ref = ref_map; ref; ref = ref->next) {
		số_nguyên rlen, llen = 0, len;

		nếu (ref->status == REF_STATUS_REJECT_SHALLOW ||
		    !ref->peer_ref ||
		    !strcmp(ref->name, "HEAD"))
			continue;

		/* uptodate lines are only shown on high verbosity level */
		nếu (verbosity <= 0 && oideq(&ref->peer_ref->old_oid, &ref->old_oid))
			continue;

		rlen = utf8_strwidth(prettify_refname(ref->name));
		nếu (!compact_format)
			llen = utf8_strwidth(prettify_refname(ref->peer_ref->name));

		/*
		 * rough estimation to see if the output line is too long and
		 * should not be counted (we can't do precise calculation
		 * anyway because we don't know if the error explanation part
		 * will be printed in update_local_ref)
		 */
		len = 21 /* flag and summary */ + rlen + 4 /* -> */ + llen;
		nếu (len >= max)
			continue;

		nếu (width < rlen)
			width = rlen;
	}

	trả_về width;
}

static void display_state_init(struct display_state *display_state, struct ref *ref_map,
			       const char *raw_url, enum display_format format)
{
	số_nguyên i;

	memset(display_state, 0, sizeof(*display_state));
	strbuf_init(&display_state->buf, 0);
	display_state->format = format;

	nếu (raw_url)
		display_state->url = transport_anonymize_url(raw_url);
	không_thì
		display_state->url = xstrdup("foreign");

	display_state->url_len = strlen(display_state->url);
	cho (i = display_state->url_len - 1; 0 <= i && display_state->url[i] == '/'; i--)
		;
	display_state->url_len = i + 1;
	nếu (4 < i && !strncmp(".git", display_state->url + i - 3, 4))
		display_state->url_len = i - 3;

	nếu (verbosity < 0)
		trả_về;

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

static số_nguyên find_and_replace(struct strbuf *haystack,
			    const char *needle,
			    const char *placeholder)
{
	const char *p = NULL;
	số_nguyên plen, nlen;

	nlen = strlen(needle);
	nếu (ends_with(haystack->buf, needle))
		p = haystack->buf + haystack->len - nlen;
	không_thì
		p = strstr(haystack->buf, needle);
	nếu (!p)
		trả_về 0;

	nếu (p > haystack->buf && p[-1] != '/')
		trả_về 0;

	plen = strlen(p);
	nếu (plen > nlen && p[nlen] != '/')
		trả_về 0;

	strbuf_splice(haystack, p - haystack->buf, nlen,
		      placeholder, strlen(placeholder));
	trả_về 1;
}

static void print_compact(struct display_state *display_state,
			  const char *remote, const char *local)
{
	struct strbuf r = STRBUF_INIT;
	struct strbuf l = STRBUF_INIT;

	nếu (!strcmp(remote, local)) {
		strbuf_addf(&display_state->buf, "%-*s -> *", display_state->refcol_width, remote);
		trả_về;
	}

	strbuf_addstr(&r, remote);
	strbuf_addstr(&l, local);

	nếu (!find_and_replace(&r, local, "*"))
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
			       số_nguyên summary_width)
{
	FILE *f = stderr;

	nếu (verbosity < 0)
		trả_về;

	strbuf_reset(&display_state->buf);

	switch (display_state->format) {
	case DISPLAY_FORMAT_FULL:
	case DISPLAY_FORMAT_COMPACT: {
		số_nguyên width;

		nếu (!display_state->shown_url) {
			strbuf_addf(&display_state->buf, _("From %.*s\n"),
				    display_state->url_len, display_state->url);
			display_state->shown_url = 1;
		}

		width = (summary_width + strlen(summary) - gettext_width(summary));
		remote = prettify_refname(remote);
		local = prettify_refname(local);

		strbuf_addf(&display_state->buf, " %c %-*s ", code, width, summary);

		nếu (display_state->format != DISPLAY_FORMAT_COMPACT)
			print_remote_to_local(display_state, remote, local);
		không_thì
			print_compact(display_state, remote, local);

		nếu (error)
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

	trả_về info;
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
					    số_nguyên summary_width)
{
	display_ref_update(display_state,
			   info->failed ? info->fail_code : info->success_code,
			   info->summary,
			   info->failed ? info->fail_detail : info->success_detail,
			   info->remote, info->ref, &info->old_oid,
			   &info->new_oid, summary_width);
}

static số_nguyên update_local_ref(struct ref *ref,
			    struct ref_transaction *transaction,
			    const struct ref *remote_ref,
			    const struct fetch_config *config,
			    struct ref_update_display_info_array *display_array)
{
	struct commit *current = NULL, *updated;
	số_nguyên fast_forward = 0;

	nếu (!odb_has_object(the_repository->objects, &ref->new_oid,
			    ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR))
		die(_("object %s not found"), oid_to_hex(&ref->new_oid));

	nếu (oideq(&ref->old_oid, &ref->new_oid)) {
		nếu (verbosity > 0)
			ref_update_display_info_append(display_array, '=', '=',
						       _("[up to date]"), NULL,
						       NULL, ref->name,
						       remote_ref->name, &ref->old_oid,
						       &ref->new_oid);
		trả_về 0;
	}

	nếu (!update_head_ok &&
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
		trả_về 1;
	}

	nếu (!is_null_oid(&ref->old_oid) &&
	    starts_with(ref->name, "refs/tags/")) {
		struct ref_update_display_info *info;

		nếu (force || ref->force) {
			số_nguyên r;

			r = s_update_ref("updating tag", ref, transaction, 0);

			info = ref_update_display_info_append(display_array, 't', '!',
							      _("[tag update]"), NULL,
							      _("unable to update local ref"),
							      ref->name, remote_ref->name,
							      &ref->old_oid, &ref->new_oid);
			nếu (r)
				ref_update_display_info_set_failed(info);

			trả_về r;
		} không_thì {
			info = ref_update_display_info_append(display_array, '!', '!',
							      _("[rejected]"), NULL,
							      _("would clobber existing tag"),
							      ref->name, remote_ref->name,
							      &ref->old_oid, &ref->new_oid);
			ref_update_display_info_set_failed(info);
			trả_về 1;
		}
	}

	current = lookup_commit_reference_gently(the_repository,
						 &ref->old_oid, 1);
	updated = lookup_commit_reference_gently(the_repository,
						 &ref->new_oid, 1);
	nếu (!current || !updated) {
		struct ref_update_display_info *info;
		const char *msg;
		const char *what;
		số_nguyên r;
		/*
		 * Nicely describe the new ref we're fetching.
		 * Base this on the remote's ref name, as it's
		 * more likely to follow a standard layout.
		 */
		nếu (starts_with(remote_ref->name, "refs/tags/")) {
			msg = "storing tag";
			what = _("[new tag]");
		} không_thì nếu (starts_with(remote_ref->name, "refs/heads/")) {
			msg = "storing head";
			what = _("[new branch]");
		} không_thì {
			msg = "storing ref";
			what = _("[new ref]");
		}

		r = s_update_ref(msg, ref, transaction, 0);

		info = ref_update_display_info_append(display_array, '*', '!',
						      what, NULL,
						      _("unable to update local ref"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);
		nếu (r)
			ref_update_display_info_set_failed(info);

		trả_về r;
	}

	nếu (config->show_forced_updates) {
		uint64_t t_before = getnanotime();
		fast_forward = repo_in_merge_bases(the_repository, current,
						   updated);
		nếu (fast_forward < 0)
			die(NULL);
		forced_updates_ms += (getnanotime() - t_before) / 1000000;
	} không_thì {
		fast_forward = 1;
	}

	nếu (fast_forward) {
		struct ref_update_display_info *info;
		struct strbuf quickref = STRBUF_INIT;
		số_nguyên r;

		strbuf_add_unique_abbrev(&quickref, &current->object.oid, DEFAULT_ABBREV);
		strbuf_addstr(&quickref, "..");
		strbuf_add_unique_abbrev(&quickref, &ref->new_oid, DEFAULT_ABBREV);
		r = s_update_ref("fast-forward", ref, transaction, 1);

		info = ref_update_display_info_append(display_array, ' ', '!',
						      quickref.buf, NULL,
						      _("unable to update local ref"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);
		nếu (r)
			ref_update_display_info_set_failed(info);

		strbuf_release(&quickref);
		trả_về r;
	} không_thì nếu (force || ref->force) {
		struct ref_update_display_info *info;
		struct strbuf quickref = STRBUF_INIT;
		số_nguyên r;

		strbuf_add_unique_abbrev(&quickref, &current->object.oid, DEFAULT_ABBREV);
		strbuf_addstr(&quickref, "...");
		strbuf_add_unique_abbrev(&quickref, &ref->new_oid, DEFAULT_ABBREV);
		r = s_update_ref("forced-update", ref, transaction, 1);

		info = ref_update_display_info_append(display_array, '+', '!',
						      quickref.buf, _("forced update"),
						      _("unable to update local ref"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);

		nếu (r)
			ref_update_display_info_set_failed(info);

		strbuf_release(&quickref);
		trả_về r;
	} không_thì {
		struct ref_update_display_info *info;
		info = ref_update_display_info_append(display_array, '!', '!',
						      _("[rejected]"), NULL,
						      _("non-fast-forward"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);
		ref_update_display_info_set_failed(info);
		trả_về 1;
	}
}

static const struct object_id *iterate_ref_map(void *cb_data)
{
	struct ref **rm = cb_data;
	struct ref *ref = *rm;

	trong_khi (ref && ref->status == REF_STATUS_REJECT_SHALLOW)
		ref = ref->next;
	nếu (!ref)
		trả_về NULL;
	*rm = ref->next;
	trả_về &ref->old_oid;
}

struct fetch_head {
	FILE *fp;
	struct strbuf buf;
};

static số_nguyên open_fetch_head(struct fetch_head *fetch_head)
{
	const char *filename = git_path_fetch_head(the_repository);

	nếu (write_fetch_head) {
		fetch_head->fp = fopen(filename, "a");
		nếu (!fetch_head->fp)
			trả_về error_errno(_("cannot open '%s'"), filename);
		strbuf_init(&fetch_head->buf, 0);
	} không_thì {
		fetch_head->fp = NULL;
	}

	trả_về 0;
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

	nếu (!fetch_head->fp)
		trả_về;

	switch (fetch_head_status) {
	case FETCH_HEAD_NOT_FOR_MERGE:
		merge_status_marker = "not-for-merge";
		break;
	case FETCH_HEAD_MERGE:
		merge_status_marker = "";
		break;
	default:
		/* do not write anything to FETCH_HEAD */
		trả_về;
	}

	strbuf_addf(&fetch_head->buf, "%s\t%s\t%s",
		    oid_to_hex_r(old_oid_hex, old_oid), merge_status_marker, note);
	cho (i = 0; i < url_len; ++i)
		nếu ('\n' == url[i])
			strbuf_addstr(&fetch_head->buf, "\\n");
		không_thì
			strbuf_addch(&fetch_head->buf, url[i]);
	strbuf_addch(&fetch_head->buf, '\n');

	/*
	 * When using an atomic fetch, we do not want to update FETCH_HEAD if
	 * any of the reference updates fails. We thus have to write all
	 * updates to a buffer first and only commit it as soon as all
	 * references have been successfully updated.
	 */
	nếu (!atomic_fetch) {
		strbuf_write(&fetch_head->buf, fetch_head->fp);
		strbuf_reset(&fetch_head->buf);
	}
}

static void commit_fetch_head(struct fetch_head *fetch_head)
{
	nếu (!fetch_head->fp || !atomic_fetch)
		trả_về;
	strbuf_write(&fetch_head->buf, fetch_head->fp);
}

static void close_fetch_head(struct fetch_head *fetch_head)
{
	nếu (!fetch_head->fp)
		trả_về;

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

static số_nguyên store_updated_refs(struct display_state *display_state,
			      số_nguyên connectivity_checked,
			      struct ref_transaction *transaction, struct ref *ref_map,
			      struct fetch_head *fetch_head,
			      const struct fetch_config *config,
			      struct ref_update_display_info_array *display_array)
{
	số_nguyên rc = 0;
	struct strbuf note = STRBUF_INIT;
	const char *what, *kind;
	struct ref *rm;
	số_nguyên want_status;

	nếu (!connectivity_checked) {
		struct check_connected_options opt = CHECK_CONNECTED_INIT;

		opt.exclude_hidden_refs_section = "fetch";
		rm = ref_map;
		nếu (check_connected(iterate_ref_map, &rm, &opt)) {
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
	cho (want_status = FETCH_HEAD_MERGE;
	     want_status <= FETCH_HEAD_IGNORE;
	     want_status++) {
		cho (rm = ref_map; rm; rm = rm->next) {
			struct ref *ref = NULL;

			nếu (rm->status == REF_STATUS_REJECT_SHALLOW) {
				nếu (want_status == FETCH_HEAD_MERGE)
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
			nếu (fetch_head->fp) {
				struct commit *commit = NULL;

				/*
				 * References in "refs/tags/" are often going to point
				 * to annotated tags, which are not part of the
				 * commit-graph. We thus only try to look up refs in
				 * the graph which are not in that namespace to not
				 * regress performance in repositories with many
				 * annotated tags.
				 */
				nếu (!starts_with(rm->name, "refs/tags/"))
					commit = lookup_commit_in_graph(the_repository, &rm->old_oid);
				nếu (!commit) {
					commit = lookup_commit_reference_gently(the_repository,
										&rm->old_oid,
										1);
					nếu (!commit)
						rm->fetch_head_status = FETCH_HEAD_NOT_FOR_MERGE;
				}
			}

			nếu (rm->fetch_head_status != want_status)
				continue;

			nếu (rm->peer_ref) {
				ref = alloc_ref(rm->peer_ref->name);
				oidcpy(&ref->old_oid, &rm->peer_ref->old_oid);
				oidcpy(&ref->new_oid, &rm->old_oid);
				ref->force = rm->peer_ref->force;
			}

			nếu (config->recurse_submodules != RECURSE_SUBMODULES_OFF &&
			    (!rm->peer_ref || !oideq(&ref->old_oid, &ref->new_oid))) {
				check_for_new_submodule_commits(&rm->old_oid);
			}

			nếu (!strcmp(rm->name, "HEAD")) {
				kind = "";
				what = "";
			} không_thì nếu (skip_prefix(rm->name, "refs/heads/", &what)) {
				kind = "branch";
			} không_thì nếu (skip_prefix(rm->name, "refs/tags/", &what)) {
				kind = "tag";
			} không_thì nếu (skip_prefix(rm->name, "refs/remotes/", &what)) {
				kind = "remote-tracking branch";
			} không_thì {
				kind = "";
				what = rm->name;
			}

			strbuf_reset(&note);
			nếu (*what) {
				nếu (*kind)
					strbuf_addf(&note, "%s ", kind);
				strbuf_addf(&note, "'%s' of ", what);
			}

			append_fetch_head(fetch_head, &rm->old_oid,
					  rm->fetch_head_status,
					  note.buf, display_state->url,
					  display_state->url_len);

			nếu (ref) {
				rc |= update_local_ref(ref, transaction, rm,
						       config, display_array);
				free(ref);
			} không_thì nếu (write_fetch_head || dry_run) {
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

	nếu (advice_enabled(ADVICE_FETCH_SHOW_FORCED_UPDATES)) {
		nếu (!config->show_forced_updates) {
			warning(_(warn_show_forced_updates));
		} không_thì nếu (forced_updates_ms > FORCED_UPDATES_DELAY_WARNING_IN_MS) {
			warning(_(warn_time_show_forced_updates),
				forced_updates_ms / 1000.0);
		}
	}

 abort:
	strbuf_release(&note);
	trả_về rc;
}

/*
 * We would want to bypass the object transfer altogether if
 * everything we are going to fetch already exists and is connected
 * locally.
 */
static số_nguyên check_exist_and_connected(struct ref *ref_map)
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
	nếu (deepen)
		trả_về -1;

	/*
	 * Similarly, if we need to refetch, we always want to perform a full
	 * fetch ignoring existing objects.
	 */
	nếu (refetch)
		trả_về -1;


	/*
	 * check_connected() allows objects to merely be promised, but
	 * we need all direct targets to exist.
	 */
	cho (r = rm; r; r = r->next) {
		nếu (!odb_has_object(the_repository->objects, &r->old_oid,
				    ODB_HAS_OBJECT_RECHECK_PACKED))
			trả_về -1;
	}

	opt.quiet = 1;
	opt.exclude_hidden_refs_section = "fetch";
	trả_về check_connected(iterate_ref_map, &rm, &opt);
}

static số_nguyên fetch_and_consume_refs(struct display_state *display_state,
				  struct transport *transport,
				  struct ref_transaction *transaction,
				  struct ref *ref_map,
				  struct fetch_head *fetch_head,
				  const struct fetch_config *config,
				  struct ref_update_display_info_array *display_array)
{
	số_nguyên connectivity_checked = 1;
	số_nguyên ret;

	/*
	 * We don't need to perform a fetch in case we can already satisfy all
	 * refs.
	 */
	ret = check_exist_and_connected(ref_map);
	nếu (ret) {
		trace2_region_enter("fetch", "fetch_refs", the_repository);
		ret = transport_fetch_refs(transport, ref_map);
		trace2_region_leave("fetch", "fetch_refs", the_repository);
		nếu (ret)
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
	trả_về ret;
}

static số_nguyên prune_refs(struct display_state *display_state,
		      struct refspec *rs,
		      struct ref_transaction *transaction,
		      struct ref *ref_map)
{
	số_nguyên result = 0;
	struct ref *ref, *stale_refs = get_stale_heads(rs, ref_map);
	struct strbuf err = STRBUF_INIT;
	struct string_list refnames = STRING_LIST_INIT_NODUP;

	cho (ref = stale_refs; ref; ref = ref->next)
		string_list_append(&refnames, ref->name);

	nếu (!dry_run) {
		nếu (transaction) {
			cho (ref = stale_refs; ref; ref = ref->next) {
				result = ref_transaction_delete(transaction, ref->name, NULL,
								NULL, 0, "fetch: prune", &err);
				nếu (result)
					goto cleanup;
			}
		} không_thì {
			result = refs_delete_refs(get_main_ref_store(the_repository),
						  "fetch: prune", &refnames,
						  0);
		}
	}

	nếu (verbosity >= 0) {
		số_nguyên summary_width = transport_summary_width(stale_refs);

		cho (ref = stale_refs; ref; ref = ref->next) {
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
	trả_về result;
}

static void check_not_current_branch(struct ref *ref_map)
{
	const char *path;
	cho (; ref_map; ref_map = ref_map->next)
		nếu (ref_map->peer_ref &&
		    starts_with(ref_map->peer_ref->name, "refs/heads/") &&
		    (path = branch_checked_out(ref_map->peer_ref->name)))
			die(_("refusing to fetch into branch '%s' "
			      "checked out at '%s'"),
			    ref_map->peer_ref->name, path);
}

static số_nguyên truncate_fetch_head(void)
{
	const char *filename = git_path_fetch_head(the_repository);
	FILE *fp = fopen_for_writing(filename);

	nếu (!fp)
		trả_về error_errno(_("cannot open '%s'"), filename);
	fclose(fp);
	trả_về 0;
}

static void set_option(struct transport *transport, const char *name, const char *value)
{
	số_nguyên r = transport_set_option(transport, name, value);
	nếu (r < 0)
		die(_("option \"%s\" value \"%s\" is not valid for %s"),
		    name, value, transport->url);
	nếu (r > 0)
		warning(_("option \"%s\" is ignored for %s"),
			name, transport->url);
}


static số_nguyên add_oid(const struct reference *ref, void *cb_data)
{
	struct oid_array *oids = cb_data;

	oid_array_append(oids, ref->oid);
	trả_về 0;
}

static void add_negotiation_tips(struct string_list *input_list,
				 struct oid_array **output_list,
				 const char *argname)
{
	struct oid_array *oids = xcalloc(1, sizeof(*oids));
	số_nguyên i;

	cho (i = 0; i < input_list->nr; i++) {
		const char *s = input_list->items[i].string;
		struct refs_for_each_ref_options opts = {
			.pattern = s,
		};
		số_nguyên old_nr;
		nếu (!has_glob_specials(s)) {
			struct object_id oid;

			/* Ignore missing reference. */
			nếu (repo_get_oid(the_repository, s, &oid))
				continue;
			/* Fail on missing object pointed by ref. */
			nếu (!odb_has_object(the_repository->objects, &oid, 0))
				die(_("the object %s does not exist"), s);

			oid_array_append(oids, &oid);
			continue;
		}
		old_nr = oids->nr;
		refs_for_each_ref_ext(get_main_ref_store(the_repository),
				      add_oid, oids, &opts);
		nếu (old_nr == oids->nr)
			warning(_("ignoring %s=%s because it does not match any refs"),
				argname, s);
	}
	*output_list = oids;
}

static struct transport *prepare_transport(struct remote *remote, số_nguyên deepen,
					   struct list_objects_filter_options *filter_options)
{
	struct transport *transport;

	transport = transport_get(remote, NULL);
	transport_set_verbosity(transport, verbosity, progress);
	transport->family = family;
	nếu (upload_pack)
		set_option(transport, TRANS_OPT_UPLOADPACK, upload_pack);
	nếu (keep)
		set_option(transport, TRANS_OPT_KEEP, "yes");
	nếu (depth)
		set_option(transport, TRANS_OPT_DEPTH, depth);
	nếu (deepen && deepen_since)
		set_option(transport, TRANS_OPT_DEEPEN_SINCE, deepen_since);
	nếu (deepen && deepen_not.nr)
		set_option(transport, TRANS_OPT_DEEPEN_NOT,
			   (const char *)&deepen_not);
	nếu (deepen_relative)
		set_option(transport, TRANS_OPT_DEEPEN_RELATIVE, "yes");
	nếu (update_shallow)
		set_option(transport, TRANS_OPT_UPDATE_SHALLOW, "yes");
	nếu (refetch)
		set_option(transport, TRANS_OPT_REFETCH, "yes");
	nếu (filter_options->choice) {
		const char *spec =
			expand_list_objects_filter_spec(filter_options);
		set_option(transport, TRANS_OPT_LIST_OBJECTS_FILTER, spec);
		set_option(transport, TRANS_OPT_FROM_PROMISOR, "1");
	}
	nếu (negotiation_restrict.nr) {
		nếu (transport->smart_options)
			add_negotiation_tips(&negotiation_restrict,
					     &transport->smart_options->negotiation_restrict_tips,
					     "--negotiation-restrict");
		không_thì
			warning(_("ignoring %s because the protocol does not support it"),
				"--negotiation-restrict");
	} không_thì nếu (remote->negotiation_restrict.nr) {
		struct string_list_item *item;
		for_each_string_list_item(item, &remote->negotiation_restrict)
			string_list_append(&negotiation_restrict, item->string);
		nếu (transport->smart_options)
			add_negotiation_tips(&negotiation_restrict,
					     &transport->smart_options->negotiation_restrict_tips,
					     "--negotiation-restrict");
		không_thì {
			struct strbuf config_name = STRBUF_INIT;
			strbuf_addf(&config_name, "remote.%s.negotiationRestrict", remote->name);
			warning(_("ignoring %s because the protocol does not support it"),
				config_name.buf);
			strbuf_release(&config_name);
		}
	}
	nếu (negotiation_include.nr) {
		nếu (transport->smart_options)
			add_negotiation_tips(&negotiation_include,
					     &transport->smart_options->negotiation_include_tips,
					     "--negotiation-include");
		không_thì
			warning(_("ignoring %s because the protocol does not support it"),
				"--negotiation-include");
	} không_thì nếu (remote->negotiation_include.nr) {
		nếu (transport->smart_options) {
			add_negotiation_tips(&remote->negotiation_include,
					     &transport->smart_options->negotiation_include_tips,
					     "--negotiation-include");
		} không_thì {
			struct strbuf config_name = STRBUF_INIT;
			strbuf_addf(&config_name, "remote.%s.negotiationInclude", remote->name);
			warning(_("ignoring %s because the protocol does not support it"),
				config_name.buf);
			strbuf_release(&config_name);
		}
	}
	trả_về transport;
}

static số_nguyên backfill_tags(struct display_state *display_state,
			 struct transport *transport,
			 struct ref_transaction *transaction,
			 struct ref *ref_map,
			 struct fetch_head *fetch_head,
			 const struct fetch_config *config,
			 struct ref_update_display_info_array *display_array,
			 struct list_objects_filter_options *filter_options)
{
	số_nguyên retcode, cannot_reuse;

	/*
	 * Once we have set TRANS_OPT_DEEPEN_SINCE, we can't unset it
	 * when remote helper is used (setting it to an empty string
	 * is not unsetting). We could extend the remote helper
	 * protocol for that, but for now, just force a new connection
	 * without deepen-since. Similar story for deepen-not.
	 */
	cannot_reuse = transport->cannot_reuse ||
		deepen_since || deepen_not.nr;
	nếu (cannot_reuse) {
		gsecondary = prepare_transport(transport->remote, 0, filter_options);
		transport = gsecondary;
	}

	transport_set_option(transport, TRANS_OPT_FOLLOWTAGS, NULL);
	transport_set_option(transport, TRANS_OPT_DEPTH, "0");
	transport_set_option(transport, TRANS_OPT_DEEPEN_RELATIVE, NULL);
	retcode = fetch_and_consume_refs(display_state, transport, transaction, ref_map,
					 fetch_head, config, display_array);

	nếu (gsecondary) {
		transport_disconnect(gsecondary);
		gsecondary = NULL;
	}

	trả_về retcode;
}

static const char *strip_refshead(const char *name){
	skip_prefix(name, "refs/heads/", &name);
	trả_về name;
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
			struct strbuf *buf_prev, số_nguyên updateres) {
	struct strbuf buf_prefix = STRBUF_INIT;
	const char *prev_head = NULL;

	strbuf_addf(&buf_prefix, "refs/remotes/%s/", remote);
	skip_prefix(buf_prev->buf, buf_prefix.buf, &prev_head);

	nếu (prev_head && strcmp(prev_head, head_name)) {
		printf("'HEAD' at '%s' is '%s', but we have '%s' locally.\n",
			remote, head_name, prev_head);
		set_head_advice_msg(remote, head_name);
	}
	không_thì nếu (updateres && buf_prev->len) {
		printf("'HEAD' at '%s' is '%s', "
			"but we have a detached HEAD pointing to '%s' locally.\n",
			remote, head_name, buf_prev->buf);
		set_head_advice_msg(remote, head_name);
	}
	strbuf_release(&buf_prefix);
}

static số_nguyên set_head(const struct ref *remote_refs, struct remote *remote,
			số_nguyên follow_remote_head)
{
	số_nguyên result = 0, create_only, baremirror, was_detached;
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
	cho (ref = matches; ref; ref = ref->next) {
		string_list_append(&heads, strip_refshead(ref->name));
	}

	nếu (!heads.nr)
		result = 1;
	không_thì nếu (heads.nr > 1)
		result = 1;
	không_thì
		head_name = xstrdup(heads.items[0].string);

	nếu (!head_name)
		goto cleanup;
	baremirror = is_bare_repository(the_repository) && remote->mirror;
	create_only = follow_remote_head == FOLLOW_REMOTE_ALWAYS ? 0 : !baremirror;
	nếu (baremirror) {
		strbuf_addstr(&b_head, "HEAD");
		strbuf_addf(&b_remote_head, "refs/heads/%s", head_name);
	} không_thì {
		strbuf_addf(&b_head, "refs/remotes/%s/HEAD", remote->name);
		strbuf_addf(&b_remote_head, "refs/remotes/%s/%s", remote->name, head_name);
	}
	/* make sure it's valid */
	nếu (!baremirror && !refs_ref_exists(refs, b_remote_head.buf)) {
		result = 1;
		goto cleanup;
	}
	was_detached = refs_update_symref_extended(refs, b_head.buf, b_remote_head.buf,
					"fetch", &b_local_head, create_only);
	nếu (was_detached == -1) {
		result = 1;
		goto cleanup;
	}
	nếu (verbosity >= 0 &&
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
	trả_về result;
}

struct ref_rejection_data {
	số_nguyên *retcode;
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

	nếu (err == REF_TRANSACTION_ERROR_CASE_CONFLICT && repo_ignore_case(the_repository) &&
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
	} không_thì nếu (err == REF_TRANSACTION_ERROR_NAME_CONFLICT &&
		   !data->conflict_msg_shown) {
		error(_("some local refs could not be updated; try running\n"
			" 'git remote prune %s' to remove any old, conflicting "
			"branches"), data->remote_name);
		data->conflict_msg_shown = true;
	} không_thì {
		nếu (details)
			error("%s", details);
		không_thì
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
static số_nguyên commit_ref_transaction(struct ref_transaction **transaction,
				  bool is_atomic, const char *remote_name,
				  struct strmap *rejected_refs,
				  struct strbuf *err)
{
	số_nguyên retcode = ref_transaction_commit(*transaction, err);
	nếu (retcode)
		goto out;

	nếu (!is_atomic) {
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
	trả_về retcode;
}

static void collect_updated_tips(struct oidset *tips, struct ref *ref_map)
{
	struct ref *rm;
	cho (rm = ref_map; rm; rm = rm->next) {
		struct commit *commit;
		/*
		 * Shallow-rejected refs are not stored and their history
		 * is incomplete, so skip them.
		 */
		nếu (rm->status == REF_STATUS_REJECT_SHALLOW)
			continue;
		nếu (is_null_oid(&rm->old_oid))
			continue;
		nếu (rm->peer_ref &&
		    oideq(&rm->old_oid, &rm->peer_ref->old_oid))
			continue;
		commit = lookup_commit_reference_gently(the_repository,
							&rm->old_oid, 1);
		nếu (commit)
			oidset_insert(tips, &commit->object.oid);
	}
}

static số_nguyên do_fetch(struct transport *transport,
		    struct refspec *rs,
		    const struct fetch_config *config,
		    struct list_objects_filter_options *filter_options,
		    struct oidset *updated_tips)
{
	struct ref_transaction *transaction = NULL;
	struct ref *ref_map = NULL;
	struct display_state display_state = { 0 };
	số_nguyên autotags = (transport->remote->fetch_tags == 1);
	số_nguyên retcode = 0;
	const struct ref *remote_refs;
	struct transport_ls_refs_options transport_ls_refs_options =
		TRANSPORT_LS_REFS_OPTIONS_INIT;
	struct fetch_head fetch_head = { 0 };
	struct strbuf err = STRBUF_INIT;
	struct ref_update_display_info_array display_array = { 0 };
	struct strmap rejected_refs = STRMAP_INIT;
	số_nguyên summary_width = 0;
	số_nguyên follow_remote_head = FOLLOW_REMOTE_NEVER;

	nếu (tags == TAGS_DEFAULT) {
		nếu (transport->remote->fetch_tags == 2)
			tags = TAGS_SET;
		nếu (transport->remote->fetch_tags == -1)
			tags = TAGS_UNSET;
	}

	/* if not appending, truncate FETCH_HEAD */
	nếu (!append && write_fetch_head) {
		retcode = truncate_fetch_head();
		nếu (retcode)
			goto cleanup;
	}

	nếu (rs->nr) {
		refspec_ref_prefixes(rs, &transport_ls_refs_options.ref_prefixes);
	} không_thì {
		struct branch *branch = branch_get(NULL);

		nếu (transport->remote->fetch.nr) {
			refspec_ref_prefixes(&transport->remote->fetch,
					     &transport_ls_refs_options.ref_prefixes);
			/*
			 * See remote.c's handling of remote.<name>.followRemoteHEAD
			 * for the analogous, still-unresolved case.
			 */
			nếu (transport->remote->follow_remote_head)
				follow_remote_head = transport->remote->follow_remote_head;
			không_thì nếu (config->follow_remote_head_seen)
				follow_remote_head = get_follow_remote_head(config->follow_remote_head_raw);
			không_thì
				follow_remote_head = BUILTIN_FOLLOW_REMOTE_HEAD_DFLT;
		}
		nếu (branch && branch_has_merge_config(branch) &&
		    !strcmp(branch->remote_name, transport->remote->name)) {
			số_nguyên i;
			cho (i = 0; i < branch->merge_nr; i++) {
				strvec_push(&transport_ls_refs_options.ref_prefixes,
					    branch->merge[i]->src);
			}
		}

		/*
		 * If there are no refs specified to fetch, then we just
		 * fetch HEAD; mention that to narrow the advertisement.
		 */
		nếu (!transport_ls_refs_options.ref_prefixes.nr)
			strvec_push(&transport_ls_refs_options.ref_prefixes,
				    "HEAD");
	}

	nếu (tags == TAGS_SET || tags == TAGS_DEFAULT)
		strvec_push(&transport_ls_refs_options.ref_prefixes,
			    "refs/tags/");

	nếu (follow_remote_head != FOLLOW_REMOTE_NEVER)
		strvec_push(&transport_ls_refs_options.ref_prefixes,
			    "HEAD");

	/*
	 * Only initiate ref listing if we have at least one ref we want to
	 * know about.
	 */
	nếu (transport_ls_refs_options.ref_prefixes.nr) {
		trace2_region_enter("fetch", "remote_refs", the_repository);
		remote_refs = transport_get_remote_refs(transport,
							&transport_ls_refs_options);
		trace2_region_leave("fetch", "remote_refs", the_repository);
	} không_thì
		remote_refs = NULL;

	transport_ls_refs_options_release(&transport_ls_refs_options);

	ref_map = get_ref_map(transport->remote, remote_refs, rs,
			      tags, &autotags);
	nếu (!update_head_ok)
		check_not_current_branch(ref_map);

	retcode = open_fetch_head(&fetch_head);
	nếu (retcode)
		goto cleanup;

	display_state_init(&display_state, ref_map, transport->url,
			   config->display_format);

	nếu (atomic_fetch) {
		transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
							  0, &err);
		nếu (!transaction) {
			retcode = -1;
			goto cleanup;
		}
	}

	nếu (tags == TAGS_DEFAULT && autotags)
		transport_set_option(transport, TRANS_OPT_FOLLOWTAGS, "1");
	nếu (prune) {
		/*
		 * We only prune based on refspecs specified
		 * explicitly (via command line or configuration); we
		 * don't care whether --tags was specified.
		 */
		nếu (rs->nr) {
			retcode = prune_refs(&display_state, rs, transaction, ref_map);
		} không_thì {
			retcode = prune_refs(&display_state, &transport->remote->fetch,
					     transaction, ref_map);
		}
		nếu (retcode != 0)
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
	nếu (!transaction) {
		transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
							  REF_TRANSACTION_ALLOW_FAILURE, &err);
		nếu (!transaction) {
			retcode = -1;
			goto cleanup;
		}
	}

	nếu (fetch_and_consume_refs(&display_state, transport, transaction, ref_map,
				   &fetch_head, config, &display_array)) {
		retcode = 1;
		goto cleanup;
	}

	/*
	 * If neither --no-tags nor --tags was specified, do automated tag
	 * following.
	 */
	nếu (tags == TAGS_DEFAULT && autotags) {
		struct ref *tags_ref_map = NULL, **tail = &tags_ref_map;

		find_non_local_tags(remote_refs, transaction, &tags_ref_map, &tail);
		nếu (tags_ref_map) {
			/*
			 * If backfilling of tags fails then we want to tell
			 * the user so, but we have to continue regardless to
			 * populate upstream information of the references we
			 * have already fetched above. The exception though is
			 * when `--atomic` is passed: in that case we'll abort
			 * the transaction and don't commit anything.
			 */
			nếu (backfill_tags(&display_state, transport, transaction, tags_ref_map,
					  &fetch_head, config, &display_array, filter_options))
				retcode = 1;
		}

		free_refs(tags_ref_map);
	}

	nếu (retcode)
		goto cleanup;

	nếu (verbosity >= 0)
		summary_width = transport_summary_width(ref_map);

	retcode = commit_ref_transaction(&transaction, atomic_fetch,
					 transport->remote->name,
					 &rejected_refs, &err);
	/*
	 * With '--atomic', bail out if the transaction fails. Without '--atomic',
	 * continue to fetch head and perform other post-fetch operations.
	 */
	nếu (retcode && atomic_fetch)
		goto cleanup;

	commit_fetch_head(&fetch_head);

	collect_updated_tips(updated_tips, ref_map);

	nếu (set_upstream) {
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
		cho (rm = ref_map; rm; rm = rm->next) {
			nếu (!rm->peer_ref) {
				nếu (source_ref) {
					warning(_("multiple branches detected, incompatible with --set-upstream"));
					goto cleanup;
				} không_thì {
					source_ref = rm;
				}
			}
		}
		nếu (source_ref) {
			nếu (!branch) {
				const char *shortname = source_ref->name;
				skip_prefix(shortname, "refs/heads/", &shortname);

				warning(_("could not set upstream of HEAD to '%s' from '%s' when "
					  "it does not point to any branch."),
					shortname, transport->remote->name);
				goto cleanup;
			}

			nếu (!strcmp(source_ref->name, "HEAD") ||
			    starts_with(source_ref->name, "refs/heads/"))
				install_branch_config(0,
						      branch->name,
						      transport->remote->name,
						      source_ref->name);
			không_thì nếu (starts_with(source_ref->name, "refs/remotes/"))
				warning(_("not setting upstream for a remote remote-tracking branch"));
			không_thì nếu (starts_with(source_ref->name, "refs/tags/"))
				warning(_("not setting upstream for a remote tag"));
			không_thì
				warning(_("unknown branch type"));
		} không_thì {
			warning(_("no source branch found;\n"
				  "you need to specify exactly one branch with the --set-upstream option"));
		}
	}
	nếu (follow_remote_head != FOLLOW_REMOTE_NEVER) {
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
	nếu (retcode && !atomic_fetch && transaction)
		commit_ref_transaction(&transaction, false,
				       transport->remote->name,
				       &rejected_refs, &err);

	cho (size_t i = 0; i < display_array.nr; i++) {
		struct ref_update_display_info *info = &display_array.info[i];

		nếu (!info->failed && strmap_contains(&rejected_refs, info->ref))
			ref_update_display_info_set_failed(info);
		ref_update_display_info_display(info, &display_state, summary_width);
		ref_update_display_info_free(info);
	}

	nếu (retcode) {
		nếu (err.len) {
			error("%s", err.buf);
			strbuf_reset(&err);
		}
		nếu (transaction && ref_transaction_abort(transaction, &err) &&
		    err.len)
			error("%s", err.buf);
		transaction = NULL;
	}

	nếu (transaction)
		ref_transaction_free(transaction);

	free(display_array.info);
	strmap_clear(&rejected_refs, 0);
	display_state_release(&display_state);
	close_fetch_head(&fetch_head);
	strbuf_release(&err);
	free_refs(ref_map);
	trả_về retcode;
}

static số_nguyên get_one_remote_for_fetch(struct remote *remote, void *priv)
{
	struct string_list *list = priv;
	nếu (!remote->skip_default_update)
		string_list_append(list, remote->name);
	trả_về 0;
}

static void add_options_to_argv(struct strvec *argv,
				const struct fetch_config *config)
{
	nếu (dry_run)
		strvec_push(argv, "--dry-run");
	nếu (prune != -1)
		strvec_push(argv, prune ? "--prune" : "--no-prune");
	nếu (prune_tags != -1)
		strvec_push(argv, prune_tags ? "--prune-tags" : "--no-prune-tags");
	nếu (update_head_ok)
		strvec_push(argv, "--update-head-ok");
	nếu (force)
		strvec_push(argv, "--force");
	nếu (keep)
		strvec_push(argv, "--keep");
	nếu (config->recurse_submodules == RECURSE_SUBMODULES_ON)
		strvec_push(argv, "--recurse-submodules");
	không_thì nếu (config->recurse_submodules == RECURSE_SUBMODULES_OFF)
		strvec_push(argv, "--no-recurse-submodules");
	không_thì nếu (config->recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND)
		strvec_push(argv, "--recurse-submodules=on-demand");
	nếu (tags == TAGS_SET)
		strvec_push(argv, "--tags");
	không_thì nếu (tags == TAGS_UNSET)
		strvec_push(argv, "--no-tags");
	nếu (verbosity >= 2)
		strvec_push(argv, "-v");
	nếu (verbosity >= 1)
		strvec_push(argv, "-v");
	không_thì nếu (verbosity < 0)
		strvec_push(argv, "-q");
	nếu (family == TRANSPORT_FAMILY_IPV4)
		strvec_push(argv, "--ipv4");
	không_thì nếu (family == TRANSPORT_FAMILY_IPV6)
		strvec_push(argv, "--ipv6");
	nếu (!write_fetch_head)
		strvec_push(argv, "--no-write-fetch-head");
	nếu (config->display_format == DISPLAY_FORMAT_PORCELAIN)
		strvec_pushf(argv, "--porcelain");
}

/* Fetch multiple remotes in parallel */

struct parallel_fetch_state {
	const char **argv;
	struct string_list *remotes;
	số_nguyên next, result;
	const struct fetch_config *config;
};

static số_nguyên fetch_next_remote(struct child_process *cp,
			     struct strbuf *out UNUSED,
			     void *cb, void **task_cb)
{
	struct parallel_fetch_state *state = cb;
	char *remote;

	nếu (state->next < 0 || state->next >= state->remotes->nr)
		trả_về 0;

	remote = state->remotes->items[state->next++].string;
	*task_cb = remote;

	strvec_pushv(&cp->args, state->argv);
	strvec_push(&cp->args, remote);
	cp->git_cmd = 1;

	nếu (verbosity >= 0 && state->config->display_format != DISPLAY_FORMAT_PORCELAIN)
		printf(_("Fetching %s\n"), remote);

	trả_về 1;
}

static số_nguyên fetch_failed_to_start(struct strbuf *out UNUSED,
				 void *cb, void *task_cb)
{
	struct parallel_fetch_state *state = cb;
	const char *remote = task_cb;

	state->result = error(_("could not fetch %s"), remote);

	trả_về 0;
}

static số_nguyên fetch_finished(số_nguyên result, struct strbuf *out,
			  void *cb, void *task_cb)
{
	struct parallel_fetch_state *state = cb;
	const char *remote = task_cb;

	nếu (result) {
		strbuf_addf(out, _("could not fetch '%s' (exit code: %d)\n"),
			    remote, result);
		state->result = -1;
	}

	trả_về 0;
}

static số_nguyên fetch_multiple(struct string_list *list, số_nguyên max_children,
			  const struct fetch_config *config)
{
	số_nguyên i, result = 0;
	struct strvec argv = STRVEC_INIT;

	nếu (!append && write_fetch_head) {
		số_nguyên errcode = truncate_fetch_head();
		nếu (errcode)
			trả_về errcode;
	}

	/*
	 * Cancel out the fetch.bundleURI config when running subprocesses,
	 * to avoid fetching from the same bundle list multiple times.
	 */
	strvec_pushl(&argv, "-c", "fetch.bundleURI=",
		     "fetch", "--append", "--no-auto-gc",
		     "--no-write-commit-graph", NULL);
	cho (i = 0; i < server_options.nr; i++)
		strvec_pushf(&argv, "--server-option=%s", server_options.items[i].string);
	add_options_to_argv(&argv, config);

	nếu (max_children != 1 && list->nr != 1) {
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
	} không_thì
		cho (i = 0; i < list->nr; i++) {
			const char *name = list->items[i].string;
			struct child_process cmd = CHILD_PROCESS_INIT;

			strvec_pushv(&cmd.args, argv.v);
			strvec_push(&cmd.args, name);
			nếu (verbosity >= 0 && config->display_format != DISPLAY_FORMAT_PORCELAIN)
				printf(_("Fetching %s\n"), name);
			cmd.git_cmd = 1;
			nếu (run_command(&cmd)) {
				error(_("could not fetch %s"), name);
				result = 1;
			}
		}

	strvec_clear(&argv);
	trả_về !!result;
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
	nếu (filter_options->no_filter)
		trả_về;

	/*
	 * If no prior partial clone/fetch and the current fetch DID NOT
	 * request a partial-fetch, do a normal fetch.
	 */
	nếu (!repo_has_promisor_remote(the_repository) && !filter_options->choice)
		trả_về;

	/*
	 * If this is a partial-fetch request, we enable partial on
	 * this repo if not already enabled and remember the given
	 * filter-spec as the default for subsequent fetches to this
	 * remote if there is currently no default filter-spec.
	 */
	nếu (filter_options->choice) {
		partial_clone_register(remote->name, filter_options);
		trả_về;
	}

	/*
	 * Do a partial-fetch from the promisor remote using either the
	 * explicitly given filter-spec or inherit the filter-spec from
	 * the config.
	 */
	nếu (!filter_options->choice)
		partial_clone_get_default_filter_spec(filter_options, remote->name);
	trả_về;
}

static số_nguyên fetch_one(struct remote *remote, số_nguyên argc, const char **argv,
		     số_nguyên prune_tags_ok, số_nguyên use_stdin_refspecs,
		     const struct fetch_config *config,
		     struct list_objects_filter_options *filter_options,
		     struct oidset *updated_tips)
{
	struct refspec rs = REFSPEC_INIT_FETCH(the_hash_algo);
	số_nguyên i;
	số_nguyên exit_code;
	số_nguyên maybe_prune_tags;
	số_nguyên remote_via_config = remote_is_configured(remote, 0);

	nếu (!remote)
		die(_("no remote repository specified; please specify either a URL or a\n"
		      "remote name from which new revisions should be fetched"));

	gtransport = prepare_transport(remote, 1, filter_options);

	nếu (prune < 0) {
		/* no command line request */
		nếu (0 <= remote->prune)
			prune = remote->prune;
		không_thì nếu (0 <= config->prune)
			prune = config->prune;
		không_thì
			prune = PRUNE_BY_DEFAULT;
	}

	nếu (prune_tags < 0) {
		/* no command line request */
		nếu (0 <= remote->prune_tags)
			prune_tags = remote->prune_tags;
		không_thì nếu (0 <= config->prune_tags)
			prune_tags = config->prune_tags;
		không_thì
			prune_tags = PRUNE_TAGS_BY_DEFAULT;
	}

	maybe_prune_tags = prune_tags_ok && prune_tags;
	nếu (maybe_prune_tags && remote_via_config)
		refspec_append(&remote->fetch, TAG_REFSPEC);

	nếu (maybe_prune_tags && (argc || !remote_via_config))
		refspec_append(&rs, TAG_REFSPEC);

	cho (i = 0; i < argc; i++) {
		nếu (!strcmp(argv[i], "tag")) {
			i++;
			nếu (i >= argc)
				die(_("you need to specify a tag name"));

			refspec_appendf(&rs, "refs/tags/%s:refs/tags/%s",
					argv[i], argv[i]);
		} không_thì {
			refspec_append(&rs, argv[i]);
		}
	}

	nếu (use_stdin_refspecs) {
		struct strbuf line = STRBUF_INIT;
		trong_khi (strbuf_getline_lf(&line, stdin) != EOF)
			refspec_append(&rs, line.buf);
		strbuf_release(&line);
	}

	nếu (server_options.nr)
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
	trả_về exit_code;
}

số_nguyên cmd_fetch(số_nguyên argc,
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
	số_nguyên all = -1, multiple = 0;
	số_nguyên result = 0;
	số_nguyên prune_tags_ok = 1;
	số_nguyên enable_auto_gc = 1;
	số_nguyên unshallow = 0;
	số_nguyên max_jobs = -1;
	số_nguyên recurse_submodules_cli = RECURSE_SUBMODULES_DEFAULT;
	số_nguyên recurse_submodules_default = RECURSE_SUBMODULES_ON_DEMAND;
	số_nguyên fetch_write_commit_graph = -1;
	số_nguyên stdin_refspecs = 0;
	số_nguyên negotiate_only = 0;
	số_nguyên porcelain = 0;
	số_nguyên i;
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
	cho (i = 1; i < argc; i++) {
		/* This handles non-URLs gracefully */
		char *anon = transport_anonymize_url(argv[i]);

		strbuf_addf(&default_rla, " %s", anon);
		free(anon);
	}

	repo_config(the_repository, git_fetch_config, &config);
	nếu (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_full_index = 0;
	}

	argc = parse_options(argc, argv, prefix,
			     builtin_fetch_options, builtin_fetch_usage, 0);

	nếu (recurse_submodules_cli != RECURSE_SUBMODULES_DEFAULT)
		config.recurse_submodules = recurse_submodules_cli;

	nếu (negotiate_only) {
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

	nếu (config.recurse_submodules != RECURSE_SUBMODULES_OFF) {
		số_nguyên *sfjc = config.submodule_fetch_jobs == -1
			    ? &config.submodule_fetch_jobs : NULL;
		số_nguyên *rs = config.recurse_submodules == RECURSE_SUBMODULES_DEFAULT
			  ? &config.recurse_submodules : NULL;

		fetch_config_from_gitmodules(the_repository, sfjc, rs);
	}


	nếu (porcelain) {
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

	nếu (deepen_relative) {
		nếu (deepen_relative < 0)
			die(_("negative depth in --deepen is not supported"));
		nếu (depth)
			die(_("options '%s' and '%s' cannot be used together"), "--deepen", "--depth");
		depth = xstrfmt("%d", deepen_relative);
	}
	nếu (unshallow) {
		nếu (depth)
			die(_("options '%s' and '%s' cannot be used together"), "--depth", "--unshallow");
		không_thì nếu (!is_repository_shallow(the_repository))
			die(_("--unshallow on a complete repository does not make sense"));
		không_thì
			depth = xstrfmt("%d", INFINITE_DEPTH);
	}

	/* no need to be strict, transport_set_option() will validate it again */
	nếu (depth && atoi(depth) < 1)
		die(_("depth %s is not a positive number"), depth);
	nếu (depth || deepen_since || deepen_not.nr)
		deepen = 1;

	/* FETCH_HEAD never gets updated in --dry-run mode */
	nếu (dry_run)
		write_fetch_head = 0;

	nếu (!max_jobs)
		max_jobs = online_cpus();

	nếu (!repo_config_get_string_tmp(the_repository, "fetch.bundleuri", &bundle_uri) &&
	    fetch_bundle_uri(the_repository, bundle_uri, NULL))
		warning(_("failed to fetch bundles from '%s'"), bundle_uri);

	nếu (all < 0) {
		/*
		 * no --[no-]all given;
		 * only use config option if no remote was explicitly specified
		 */
		all = (!argc) ? config.all : 0;
	}

	nếu (all) {
		nếu (argc == 1)
			die(_("fetch --all does not take a repository argument"));
		không_thì nếu (argc > 1)
			die(_("fetch --all does not make sense with refspecs"));

		(void) for_each_remote(get_one_remote_for_fetch, &list);

		/* do not do fetch_multiple() of one */
		nếu (list.nr == 1)
			remote = remote_get(list.items[0].string);
	} không_thì nếu (argc == 0) {
		/* No arguments -- use default remote */
		remote = remote_get(NULL);
	} không_thì nếu (multiple) {
		/* All arguments are assumed to be remotes or groups */
		cho (i = 0; i < argc; i++)
			nếu (!add_remote_or_group(argv[i], &list))
				die(_("no such remote or remote group: %s"),
				    argv[i]);
	} không_thì {
		/* Single remote or group */
		(void) add_remote_or_group(argv[0], &list);
		nếu (list.nr > 1) {
			/* More than one remote */
			nếu (argc > 1)
				die(_("fetching a group and specifying refspecs does not make sense"));
		} không_thì {
			/* Zero or one remotes */
			remote = remote_get(argv[0]);
			prune_tags_ok = (argc == 1);
			argc--;
			argv++;
		}
	}
	string_list_remove_duplicates(&list, 0);

	nếu (negotiate_only) {
		struct oidset acked_commits = OIDSET_INIT;
		struct oidset_iter iter;
		const struct object_id *oid;

		trace2_region_enter("fetch", "negotiate-only", the_repository);
		nếu (!remote)
			die(_("must supply remote when using --negotiate-only"));
		gtransport = prepare_transport(remote, 1, &filter_options);

		nếu (!gtransport->smart_options) {
			warning(_("protocol does not support --negotiate-only, exiting"));
			result = 1;
			trace2_region_leave("fetch", "negotiate-only", the_repository);
			goto cleanup;
		}
		nếu (!gtransport->smart_options->negotiation_restrict_tips)
			die(_("%s needs one or more %s"), "--negotiate-only",
			    "--negotiation-restrict=*");

		gtransport->smart_options->acked_commits = &acked_commits;

		nếu (server_options.nr)
			gtransport->server_options = &server_options;
		result = transport_fetch_refs(gtransport, NULL);
		gtransport->smart_options->acked_commits = NULL;

		oidset_iter_init(&acked_commits, &iter);
		trong_khi ((oid = oidset_iter_next(&iter)))
			printf("%s\n", oid_to_hex(oid));
		oidset_clear(&acked_commits);
		trace2_region_leave("fetch", "negotiate-only", the_repository);
	} không_thì nếu (remote) {
		nếu (filter_options.choice || repo_has_promisor_remote(the_repository)) {
			trace2_region_enter("fetch", "setup-partial", the_repository);
			fetch_one_setup_partial(remote, &filter_options);
			trace2_region_leave("fetch", "setup-partial", the_repository);
		}
		trace2_region_enter("fetch", "fetch-one", the_repository);
		result = fetch_one(remote, argc, argv, prune_tags_ok, stdin_refspecs,
				   &config, &filter_options, &updated_tips);
		nếu (prepare_commit_graph(the_repository)) {
			nếu (oidset_size(&updated_tips))
				graph_write_mode = GRAPH_WRITE_TIPS;
			không_thì
				graph_write_mode = GRAPH_WRITE_SKIP;
		}
		trace2_region_leave("fetch", "fetch-one", the_repository);
	} không_thì {
		số_nguyên max_children = max_jobs;

		nếu (filter_options.choice)
			die(_("--filter can only be used with the remote "
			      "configured in extensions.partialclone"));

		nếu (atomic_fetch)
			die(_("--atomic can only be used when fetching "
			      "from one remote"));

		nếu (stdin_refspecs)
			die(_("--stdin can only be used when fetching "
			      "from one remote"));

		nếu (max_children < 0)
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
	nếu (!result && remote && (config.recurse_submodules != RECURSE_SUBMODULES_OFF)) {
		struct strvec options = STRVEC_INIT;
		số_nguyên max_children = max_jobs;

		nếu (max_children < 0)
			max_children = config.submodule_fetch_jobs;
		nếu (max_children < 0)
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
	nếu (negotiate_only)
		goto cleanup;

	prepare_repo_settings(the_repository);
	nếu (fetch_write_commit_graph > 0 ||
	    (fetch_write_commit_graph < 0 &&
	     the_repository->settings.fetch_write_commit_graph)) {
		số_nguyên commit_graph_flags = COMMIT_GRAPH_WRITE_SPLIT;

		nếu (progress)
			commit_graph_flags |= COMMIT_GRAPH_WRITE_PROGRESS;

		nếu (graph_write_mode != GRAPH_WRITE_SKIP) {
			trace2_region_enter("fetch", "write-commit-graph",
					    the_repository);
			nếu (graph_write_mode == GRAPH_WRITE_TIPS)
				write_commit_graph(
					the_repository,
					the_repository->objects->source->path,
					NULL, &updated_tips,
					commit_graph_flags, NULL);
			không_thì
				write_commit_graph_reachable(
					the_repository,
					the_repository->objects->source->path,
					commit_graph_flags, NULL);
			trace2_region_leave("fetch", "write-commit-graph",
					    the_repository);
		}
	}

	nếu (enable_auto_gc) {
		nếu (refetch) {
			/*
			 * Hint auto-maintenance strongly to encourage repacking,
			 * but respect config settings disabling it.
			 */
			số_nguyên opt_val;
			nếu (repo_config_get_int(the_repository, "gc.autopacklimit", &opt_val))
				opt_val = -1;
			nếu (opt_val != 0)
				git_config_push_parameter("gc.autoPackLimit=1");

			nếu (repo_config_get_int(the_repository, "maintenance.incremental-repack.auto", &opt_val))
				opt_val = -1;
			nếu (opt_val != 0)
				git_config_push_parameter("maintenance.incremental-repack.auto=-1");
		}
		run_auto_maintenance(the_repository, verbosity < 0);
	}

 cleanup:
	oidset_clear(&updated_tips);
	string_list_clear(&list, 0);
	list_objects_filter_release(&filter_options);
	free(config.follow_remote_head_raw);
	trả_về result;
}
