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

static const var_na * const builtin_fetch_usage[] = {
	N_("git fetch [<options>] [<repository> [<refspec>...]]"),
	N_("git fetch [<options>] <group>"),
	N_("git fetch --multiple [<options>] [(<repository>|<group>)...]"),
	N_("git fetch --all [<options>]"),
	NULL
};

ga_nanaa {
	TAGS_UNSET = 0,
	TAGS_DEFAULT = 1,
	TAGS_SET = 2
};

ga_nanaa display_format {
	DISPLAY_FORMAT_FULL,
	DISPLAY_FORMAT_COMPACT,
	DISPLAY_FORMAT_PORCELAIN,
};

sa_nracanaa display_state {
	sa_nracanaa strbuf buf;

	ཧྲིལ refcol_width;
	ga_nanaa display_format format;

	var_na *url;
	ཧྲིལ url_len, shown_url;
};

static uint64_t forced_updates_ms = 0;
static ཧྲིལ prefetch = 0;
static ཧྲིལ prune = -1; /* unspecified */
#define PRUNE_BY_DEFAULT 0 /* do we prune by default? */

static ཧྲིལ prune_tags = -1; /* unspecified */
#define PRUNE_TAGS_BY_DEFAULT 0 /* do we prune tags by default? */

static ཧྲིལ append, dry_run, force, keep, update_head_ok;
static ཧྲིལ write_fetch_head = 1;
static ཧྲིལ verbosity, deepen_relative, set_upstream, refetch;
static ཧྲིལ progress = -1;
static ཧྲིལ tags = TAGS_DEFAULT, update_shallow, deepen;
static ཧྲིལ atomic_fetch;
static ga_nanaa transport_family family;
static const var_na *depth;
static const var_na *deepen_since;
static const var_na *upload_pack;
static sa_nracanaa string_list deepen_not = STRING_LIST_INIT_NODUP;
static sa_nracanaa strbuf default_rla = STRBUF_INIT;
static sa_nracanaa transport *gtransport;
static sa_nracanaa transport *gsecondary;
static sa_nracanaa refspec refmap;
static sa_nracanaa string_list server_options = STRING_LIST_INIT_DUP;
static sa_nracanaa string_list negotiation_restrict = STRING_LIST_INIT_NODUP;
static sa_nracanaa string_list negotiation_include = STRING_LIST_INIT_NODUP;

sa_nracanaa fetch_config {
	ga_nanaa display_format display_format;
	var_na *follow_remote_head_raw;
	ཧྲིལ follow_remote_head_seen;
	ཧྲིལ all;
	ཧྲིལ prune;
	ཧྲིལ prune_tags;
	ཧྲིལ show_forced_updates;
	ཧྲིལ recurse_submodules;
	ཧྲིལ parallel;
	ཧྲིལ submodule_fetch_jobs;
};

static ཧྲིལ git_fetch_config(const var_na *k, const var_na *v,
			    const sa_nracanaa config_context *ctx, void *cb)
{
	sa_nracanaa fetch_config *fetch_config = cb;

	གལ་ཏེ (!strcmp(k, "fetch.all")) {
		fetch_config->all = git_config_bool(k, v);
		ཕྱིར 0;
	}

	གལ་ཏེ (!strcmp(k, "fetch.prune")) {
		fetch_config->prune = git_config_bool(k, v);
		ཕྱིར 0;
	}

	གལ་ཏེ (!strcmp(k, "fetch.prunetags")) {
		fetch_config->prune_tags = git_config_bool(k, v);
		ཕྱིར 0;
	}

	གལ་ཏེ (!strcmp(k, "fetch.showforcedupdates")) {
		fetch_config->show_forced_updates = git_config_bool(k, v);
		ཕྱིར 0;
	}

	གལ་ཏེ (!strcmp(k, "submodule.recurse")) {
		ཧྲིལ r = git_config_bool(k, v) ?
			RECURSE_SUBMODULES_ON : RECURSE_SUBMODULES_OFF;
		fetch_config->recurse_submodules = r;
		ཕྱིར 0;
	}

	གལ་ཏེ (!strcmp(k, "submodule.fetchjobs")) {
		fetch_config->submodule_fetch_jobs = parse_submodule_fetchjobs(k, v, ctx->kvi);
		ཕྱིར 0;
	} དེ་མིན གལ་ཏེ (!strcmp(k, "fetch.recursesubmodules")) {
		fetch_config->recurse_submodules = parse_fetch_recurse_submodules_arg(k, v);
		ཕྱིར 0;
	}

	གལ་ཏེ (!strcmp(k, "fetch.parallel")) {
		fetch_config->parallel = git_config_int(k, v, ctx->kvi);
		གལ་ཏེ (fetch_config->parallel < 0)
			die(_("fetch.parallel cannot be negative"));
		གལ་ཏེ (!fetch_config->parallel)
			fetch_config->parallel = online_cpus();
		ཕྱིར 0;
	}

	གལ་ཏེ (!strcmp(k, "fetch.output")) {
		གལ་ཏེ (!v)
			ཕྱིར config_error_nonbool(k);
		དེ་མིན གལ་ཏེ (!strcasecmp(v, "full"))
			fetch_config->display_format = DISPLAY_FORMAT_FULL;
		དེ་མིན གལ་ཏེ (!strcasecmp(v, "compact"))
			fetch_config->display_format = DISPLAY_FORMAT_COMPACT;
		དེ་མིན
			die(_("invalid value for '%s': '%s'"),
			    "fetch.output", v);
		ཕྱིར 0;
	}

	གལ་ཏེ (!strcmp(k, "fetch.followremotehead")) {
		free(fetch_config->follow_remote_head_raw);
		fetch_config->follow_remote_head_raw = xstrdup_or_null(v);
		fetch_config->follow_remote_head_seen = 1;
		ཕྱིར 0;
	}

	ཕྱིར git_default_config(k, v, ctx, cb);
}

static ga_nanaa follow_remote_head_settings get_follow_remote_head(const var_na *setting)
{
	གལ་ཏེ (!setting)
		die(_("missing value for 'fetch.followRemoteHEAD'"));
	དེ་མིན གལ་ཏེ (!strcmp(setting, "never"))
		ཕྱིར FOLLOW_REMOTE_NEVER;
	དེ་མིན གལ་ཏེ (!strcmp(setting, "create"))
		ཕྱིར FOLLOW_REMOTE_CREATE;
	དེ་མིན གལ་ཏེ (!strcmp(setting, "warn"))
		ཕྱིར FOLLOW_REMOTE_WARN;
	དེ་མིན གལ་ཏེ (!strcmp(setting, "always"))
		ཕྱིར FOLLOW_REMOTE_ALWAYS;
	warning(_("unrecognized fetch.followRemoteHEAD value '%s' ignored"), setting);
	ཕྱིར BUILTIN_FOLLOW_REMOTE_HEAD_DFLT;
}

static ཧྲིལ parse_refmap_arg(const sa_nracanaa option *opt, const var_na *arg, ཧྲིལ unset)
{
	BUG_ON_OPT_NEG(unset);

	/*
	 * "git fetch --refmap='' origin foo"
	 * can be used to tell the command not to store anywhere
	 */
	refspec_append(opt->value, arg);

	ཕྱིར 0;
}

static void unlock_pack(unsigned ཧྲིལ flags)
{
	གལ་ཏེ (gtransport)
		transport_unlock_pack(gtransport, flags);
	གལ་ཏེ (gsecondary)
		transport_unlock_pack(gsecondary, flags);
}

static void unlock_pack_atexit(void)
{
	unlock_pack(0);
}

static void unlock_pack_on_signal(ཧྲིལ signo)
{
	unlock_pack(TRANSPORT_UNLOCK_PACK_IN_SIGNAL_HANDLER);
	sigchain_pop(signo);
	raise(signo);
}

static void add_merge_config(sa_nracanaa ref **head,
			   const sa_nracanaa ref *remote_refs,
		           sa_nracanaa branch *branch,
		           sa_nracanaa ref ***tail)
{
	ཧྲིལ i;

	ཆེད་དུ (i = 0; i < branch->merge_nr; i++) {
		sa_nracanaa ref *rm, **old_tail = *tail;
		sa_nracanaa refspec_item refspec;

		ཆེད་དུ (rm = *head; rm; rm = rm->next) {
			གལ་ཏེ (branch_merge_matches(branch, i, rm->name)) {
				rm->fetch_head_status = FETCH_HEAD_MERGE;
				bha_nga;
			}
		}
		གལ་ཏེ (rm)
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
		memset(&refspec, 0, parimaa_na(refspec));
		refspec.src = branch->merge[i]->src;
		get_fetch_map(remote_refs, &refspec, tail, 1);
		ཆེད་དུ (rm = *old_tail; rm; rm = rm->next)
			rm->fetch_head_status = FETCH_HEAD_MERGE;
	}
}

static void create_fetch_oidset(sa_nracanaa ref **head, sa_nracanaa oidset *out)
{
	sa_nracanaa ref *rm = *head;
	སྐབས (rm) {
		oidset_insert(out, &rm->old_oid);
		rm = rm->next;
	}
}

sa_nracanaa refname_hash_entry {
	sa_nracanaa hashmap_entry ent;
	sa_nracanaa object_id oid;
	ཧྲིལ ignore;
	var_na refname[FLEX_ARRAY];
};

static ཧྲིལ refname_hash_entry_cmp(const void *hashmap_cmp_fn_data UNUSED,
				  const sa_nracanaa hashmap_entry *eptr,
				  const sa_nracanaa hashmap_entry *entry_or_key,
				  const void *keydata)
{
	const sa_nracanaa refname_hash_entry *e1, *e2;

	e1 = container_of(eptr, const sa_nracanaa refname_hash_entry, ent);
	e2 = container_of(entry_or_key, const sa_nracanaa refname_hash_entry, ent);
	ཕྱིར strcmp(e1->refname, keydata ? keydata : e2->refname);
}

static sa_nracanaa refname_hash_entry *refname_hash_add(sa_nracanaa hashmap *map,
						   const var_na *refname,
						   const sa_nracanaa object_id *oid)
{
	sa_nracanaa refname_hash_entry *ent;
	size_t len = strlen(refname);

	FLEX_ALLOC_MEM(ent, refname, refname, len);
	hashmap_entry_init(&ent->ent, strhash(refname));
	oidcpy(&ent->oid, oid);
	hashmap_add(map, &ent->ent);
	ཕྱིར ent;
}

static ཧྲིལ add_one_refname(const sa_nracanaa reference *ref, void *cbdata)
{
	sa_nracanaa hashmap *refname_map = cbdata;

	(void) refname_hash_add(refname_map, ref->name, ref->oid);
	ཕྱིར 0;
}

static void refname_hash_init(sa_nracanaa hashmap *map)
{
	hashmap_init(map, refname_hash_entry_cmp, NULL, 0);
}

static ཧྲིལ refname_hash_exists(sa_nracanaa hashmap *map, const var_na *refname)
{
	ཕྱིར !!hashmap_get_from_hash(map, strhash(refname), refname);
}

static void clear_item(sa_nracanaa refname_hash_entry *item)
{
	item->ignore = 1;
}


static void add_already_queued_tags(const var_na *refname,
				    const sa_nracanaa object_id *old_oid UNUSED,
				    const sa_nracanaa object_id *new_oid,
				    void *cb_data)
{
	sa_nracanaa hashmap *queued_tags = cb_data;
	གལ་ཏེ (starts_with(refname, "refs/tags/") && new_oid)
		(void) refname_hash_add(queued_tags, refname, new_oid);
}

static void find_non_local_tags(const sa_nracanaa ref *refs,
				sa_nracanaa ref_transaction *transaction,
				sa_nracanaa ref **head,
				sa_nracanaa ref ***tail)
{
	sa_nracanaa hashmap existing_refs;
	sa_nracanaa hashmap remote_refs;
	sa_nracanaa oidset fetch_oids = OIDSET_INIT;
	sa_nracanaa string_list remote_refs_list = STRING_LIST_INIT_NODUP;
	sa_nracanaa string_list_item *remote_ref_item;
	const sa_nracanaa ref *ref;
	sa_nracanaa refname_hash_entry *item = NULL;

	refname_hash_init(&existing_refs);
	refname_hash_init(&remote_refs);
	create_fetch_oidset(head, &fetch_oids);

	refs_for_each_ref(get_main_ref_store(the_repository), add_one_refname,
			  &existing_refs);

	/*
	 * If we already have a transaction, then we need to filter out all
	 * tags which have already been queued up.
	 */
	གལ་ཏེ (transaction)
		ref_transaction_for_each_queued_update(transaction,
						       add_already_queued_tags,
						       &existing_refs);

	ཆེད་དུ (ref = refs; ref; ref = ref->next) {
		གལ་ཏེ (!starts_with(ref->name, "refs/tags/"))
			continue;

		/*
		 * The peeled ref always follows the matching base
		 * ref, so if we see a peeled ref that we don't want
		 * to fetch then we can mark the ref entry in the list
		 * as one to ignore by setting util to NULL.
		 */
		གལ་ཏེ (ends_with(ref->name, "^{}")) {
			གལ་ཏེ (item &&
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
		གལ་ཏེ (item &&
		    !odb_has_object(the_repository->objects, &item->oid, 0) &&
		    !oidset_contains(&fetch_oids, &item->oid))
			clear_item(item);

		item = NULL;

		/* skip duplicates and refs that we already have */
		གལ་ཏེ (refname_hash_exists(&remote_refs, ref->name) ||
		    refname_hash_exists(&existing_refs, ref->name))
			continue;

		item = refname_hash_add(&remote_refs, ref->name, &ref->old_oid);
		string_list_insert(&remote_refs_list, ref->name);
	}
	hashmap_clear_and_free(&existing_refs, sa_nracanaa refname_hash_entry, ent);

	/*
	 * We may have a final lightweight tag that needs to be
	 * checked to see if it needs fetching.
	 */
	གལ་ཏེ (item &&
	    !odb_has_object(the_repository->objects, &item->oid, 0) &&
	    !oidset_contains(&fetch_oids, &item->oid))
		clear_item(item);

	/*
	 * For all the tags in the remote_refs_list,
	 * add them to the list of refs to be fetched
	 */
	for_each_string_list_item(remote_ref_item, &remote_refs_list) {
		const var_na *refname = remote_ref_item->string;
		sa_nracanaa ref *rm;
		unsigned ཧྲིལ hash = strhash(refname);

		item = hashmap_get_entry_from_hash(&remote_refs, hash, refname,
					sa_nracanaa refname_hash_entry, ent);
		གལ་ཏེ (!item)
			BUG("unseen remote ref?");

		/* Unless we have already decided to ignore this item... */
		གལ་ཏེ (item->ignore)
			continue;

		rm = alloc_ref(item->refname);
		rm->peer_ref = alloc_ref(item->refname);
		oidcpy(&rm->old_oid, &item->oid);
		**tail = rm;
		*tail = &rm->next;
	}
	hashmap_clear_and_free(&remote_refs, sa_nracanaa refname_hash_entry, ent);
	string_list_clear(&remote_refs_list, 0);
	oidset_clear(&fetch_oids);
}

static void filter_prefetch_refspec(sa_nracanaa refspec *rs)
{
	ཧྲིལ i;

	གལ་ཏེ (!prefetch)
		ཕྱིར;

	ཆེད་དུ (i = 0; i < rs->nr; i++) {
		sa_nracanaa strbuf new_dst = STRBUF_INIT;
		var_na *old_dst;
		const var_na *sub = NULL;

		གལ་ཏེ (rs->items[i].negative)
			continue;
		གལ་ཏེ (!rs->items[i].dst ||
		    (rs->items[i].src &&
		     starts_with(rs->items[i].src,
				 ref_namespace[NAMESPACE_TAGS].ref))) {
			ཧྲིལ j;

			refspec_item_clear(&rs->items[i]);

			ཆེད་དུ (j = i + 1; j < rs->nr; j++)
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
		གལ་ཏེ (!skip_prefix(old_dst, "refs/", &sub))
			sub = old_dst;
		strbuf_addstr(&new_dst, sub);

		rs->items[i].dst = strbuf_detach(&new_dst, NULL);
		rs->items[i].force = 1;

		free(old_dst);
	}
}

static sa_nracanaa ref *get_ref_map(sa_nracanaa remote *remote,
			       const sa_nracanaa ref *remote_refs,
			       sa_nracanaa refspec *rs,
			       ཧྲིལ tags, ཧྲིལ *autotags)
{
	ཧྲིལ i;
	sa_nracanaa ref *rm;
	sa_nracanaa ref *ref_map = NULL;
	sa_nracanaa ref **tail = &ref_map;

	/* opportunistically-updated references: */
	sa_nracanaa ref *orefs = NULL, **oref_tail = &orefs;

	sa_nracanaa hashmap existing_refs;
	ཧྲིལ existing_refs_populated = 0;

	filter_prefetch_refspec(rs);
	གལ་ཏེ (remote)
		filter_prefetch_refspec(&remote->fetch);

	གལ་ཏེ (rs->nr) {
		sa_nracanaa refspec *fetch_refspec;

		ཆེད་དུ (i = 0; i < rs->nr; i++) {
			get_fetch_map(remote_refs, &rs->items[i], &tail, 0);
			གལ་ཏེ (rs->items[i].dst && rs->items[i].dst[0])
				*autotags = 1;
		}
		/* Merge everything on the command line (but not --tags) */
		ཆེད་དུ (rm = ref_map; rm; rm = rm->next)
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
		གལ་ཏེ (refmap.nr)
			fetch_refspec = &refmap;
		དེ་མིན
			fetch_refspec = &remote->fetch;

		ཆེད་དུ (i = 0; i < fetch_refspec->nr; i++)
			get_fetch_map(ref_map, &fetch_refspec->items[i], &oref_tail, 1);
	} དེ་མིན གལ་ཏེ (refmap.nr) {
		die("--refmap option is only meaningful with command-line refspec(s)");
	} དེ་མིན {
		/* Use the defaults */
		sa_nracanaa branch *branch = branch_get(NULL);
		ཧྲིལ has_merge = branch_has_merge_config(branch);
		གལ་ཏེ (remote &&
		    (remote->fetch.nr ||
		     /* Note: has_merge implies non-NULL branch->remote_name */
		     (has_merge && !strcmp(branch->remote_name, remote->name)))) {
			ཆེད་དུ (i = 0; i < remote->fetch.nr; i++) {
				get_fetch_map(remote_refs, &remote->fetch.items[i], &tail, 0);
				གལ་ཏེ (remote->fetch.items[i].dst &&
				    remote->fetch.items[i].dst[0])
					*autotags = 1;
				གལ་ཏེ (!i && !has_merge && ref_map &&
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
			གལ་ཏེ (has_merge &&
			    !strcmp(branch->remote_name, remote->name))
				add_merge_config(&ref_map, remote_refs, branch, &tail);
		} དེ་མིན གལ་ཏེ (!prefetch) {
			ref_map = get_remote_ref(remote_refs, "HEAD");
			གལ་ཏེ (!ref_map)
				die(_("couldn't find remote ref HEAD"));
			ref_map->fetch_head_status = FETCH_HEAD_MERGE;
			tail = &ref_map->next;
		}
	}

	གལ་ཏེ (tags == TAGS_SET) {
		sa_nracanaa refspec_item tag_refspec;

		/* also fetch all tags */
		refspec_item_init_push(&tag_refspec, TAG_REFSPEC,
				       the_hash_algo);
		get_fetch_map(remote_refs, &tag_refspec, &tail, 0);
		refspec_item_clear(&tag_refspec);
	} དེ་མིན གལ་ཏེ (tags == TAGS_DEFAULT && *autotags) {
		find_non_local_tags(remote_refs, NULL, &ref_map, &tail);
	}

	/* Now append any refs to be updated opportunistically: */
	*tail = orefs;
	ཆེད་དུ (rm = orefs; rm; rm = rm->next) {
		rm->fetch_head_status = FETCH_HEAD_IGNORE;
		tail = &rm->next;
	}

	/*
	 * apply negative refspecs first, before we remove duplicates. This is
	 * necessary as negative refspecs might remove an otherwise conflicting
	 * duplicate.
	 */
	གལ་ཏེ (rs->nr)
		ref_map = apply_negative_refspecs(ref_map, rs);
	དེ་མིན
		ref_map = apply_negative_refspecs(ref_map, &remote->fetch);

	ref_map = ref_remove_duplicates(ref_map);

	ཆེད་དུ (rm = ref_map; rm; rm = rm->next) {
		གལ་ཏེ (rm->peer_ref) {
			const var_na *refname = rm->peer_ref->name;
			sa_nracanaa refname_hash_entry *peer_item;
			unsigned ཧྲིལ hash = strhash(refname);

			གལ་ཏེ (!existing_refs_populated) {
				refname_hash_init(&existing_refs);
				refs_for_each_ref(get_main_ref_store(the_repository),
						  add_one_refname,
						  &existing_refs);
				existing_refs_populated = 1;
			}

			peer_item = hashmap_get_entry_from_hash(&existing_refs,
						hash, refname,
						sa_nracanaa refname_hash_entry, ent);
			གལ་ཏེ (peer_item) {
				sa_nracanaa object_id *old_oid = &peer_item->oid;
				oidcpy(&rm->peer_ref->old_oid, old_oid);
			}
		}
	}
	གལ་ཏེ (existing_refs_populated)
		hashmap_clear_and_free(&existing_refs, sa_nracanaa refname_hash_entry, ent);

	ཕྱིར ref_map;
}

static ཧྲིལ s_update_ref(const var_na *action,
			sa_nracanaa ref *ref,
			sa_nracanaa ref_transaction *transaction,
			ཧྲིལ check_old)
{
	var_na *msg;
	var_na *rla = getenv("GIT_REFLOG_ACTION");
	sa_nracanaa strbuf err = STRBUF_INIT;
	ཧྲིལ ret;

	གལ་ཏེ (dry_run)
		ཕྱིར 0;
	གལ་ཏེ (!rla)
		rla = default_rla.buf;
	msg = xstrfmt("%s: %s", rla, action);

	ret = ref_transaction_update(transaction, ref->name, &ref->new_oid,
				     check_old ? &ref->old_oid : NULL,
				     NULL, NULL, 0, msg, &err);

	གལ་ཏེ (ret)
		error("%s", err.buf);
	strbuf_release(&err);
	free(msg);
	ཕྱིར ret;
}

static ཧྲིལ refcol_width(const sa_nracanaa ref *ref_map, ཧྲིལ compact_format)
{
	const sa_nracanaa ref *ref;
	ཧྲིལ max, width = 10;

	max = term_columns();
	གལ་ཏེ (compact_format)
		max = max * 2 / 3;

	ཆེད་དུ (ref = ref_map; ref; ref = ref->next) {
		ཧྲིལ rlen, llen = 0, len;

		གལ་ཏེ (ref->status == REF_STATUS_REJECT_SHALLOW ||
		    !ref->peer_ref ||
		    !strcmp(ref->name, "HEAD"))
			continue;

		/* uptodate lines are only shown on high verbosity level */
		གལ་ཏེ (verbosity <= 0 && oideq(&ref->peer_ref->old_oid, &ref->old_oid))
			continue;

		rlen = utf8_strwidth(prettify_refname(ref->name));
		གལ་ཏེ (!compact_format)
			llen = utf8_strwidth(prettify_refname(ref->peer_ref->name));

		/*
		 * rough estimation to see if the output line is too long and
		 * should not be counted (we can't do precise calculation
		 * anyway because we don't know if the error explanation part
		 * will be printed in update_local_ref)
		 */
		len = 21 /* flag and summary */ + rlen + 4 /* -> */ + llen;
		གལ་ཏེ (len >= max)
			continue;

		གལ་ཏེ (width < rlen)
			width = rlen;
	}

	ཕྱིར width;
}

static void display_state_init(sa_nracanaa display_state *display_state, sa_nracanaa ref *ref_map,
			       const var_na *raw_url, ga_nanaa display_format format)
{
	ཧྲིལ i;

	memset(display_state, 0, parimaa_na(*display_state));
	strbuf_init(&display_state->buf, 0);
	display_state->format = format;

	གལ་ཏེ (raw_url)
		display_state->url = transport_anonymize_url(raw_url);
	དེ་མིན
		display_state->url = xstrdup("foreign");

	display_state->url_len = strlen(display_state->url);
	ཆེད་དུ (i = display_state->url_len - 1; 0 <= i && display_state->url[i] == '/'; i--)
		;
	display_state->url_len = i + 1;
	གལ་ཏེ (4 < i && !strncmp(".git", display_state->url + i - 3, 4))
		display_state->url_len = i - 3;

	གལ་ཏེ (verbosity < 0)
		ཕྱིར;

	switch (display_state->format) {
	pak_sa DISPLAY_FORMAT_FULL:
	pak_sa DISPLAY_FORMAT_COMPACT:
		display_state->refcol_width = refcol_width(ref_map,
							   display_state->format == DISPLAY_FORMAT_COMPACT);
		bha_nga;
	pak_sa DISPLAY_FORMAT_PORCELAIN:
		/* We don't need to precompute anything here. */
		bha_nga;
	default:
		BUG("unexpected display format %d", display_state->format);
	}
}

static void display_state_release(sa_nracanaa display_state *display_state)
{
	strbuf_release(&display_state->buf);
	free(display_state->url);
}

static void print_remote_to_local(sa_nracanaa display_state *display_state,
				  const var_na *remote, const var_na *local)
{
	strbuf_addf(&display_state->buf, "%-*s -> %s",
		    display_state->refcol_width, remote, local);
}

static ཧྲིལ find_and_replace(sa_nracanaa strbuf *haystack,
			    const var_na *needle,
			    const var_na *placeholder)
{
	const var_na *p = NULL;
	ཧྲིལ plen, nlen;

	nlen = strlen(needle);
	གལ་ཏེ (ends_with(haystack->buf, needle))
		p = haystack->buf + haystack->len - nlen;
	དེ་མིན
		p = strstr(haystack->buf, needle);
	གལ་ཏེ (!p)
		ཕྱིར 0;

	གལ་ཏེ (p > haystack->buf && p[-1] != '/')
		ཕྱིར 0;

	plen = strlen(p);
	གལ་ཏེ (plen > nlen && p[nlen] != '/')
		ཕྱིར 0;

	strbuf_splice(haystack, p - haystack->buf, nlen,
		      placeholder, strlen(placeholder));
	ཕྱིར 1;
}

static void print_compact(sa_nracanaa display_state *display_state,
			  const var_na *remote, const var_na *local)
{
	sa_nracanaa strbuf r = STRBUF_INIT;
	sa_nracanaa strbuf l = STRBUF_INIT;

	གལ་ཏེ (!strcmp(remote, local)) {
		strbuf_addf(&display_state->buf, "%-*s -> *", display_state->refcol_width, remote);
		ཕྱིར;
	}

	strbuf_addstr(&r, remote);
	strbuf_addstr(&l, local);

	གལ་ཏེ (!find_and_replace(&r, local, "*"))
		find_and_replace(&l, remote, "*");
	print_remote_to_local(display_state, r.buf, l.buf);

	strbuf_release(&r);
	strbuf_release(&l);
}

static void display_ref_update(sa_nracanaa display_state *display_state, var_na code,
			       const var_na *summary, const var_na *error,
			       const var_na *remote, const var_na *local,
			       const sa_nracanaa object_id *old_oid,
			       const sa_nracanaa object_id *new_oid,
			       ཧྲིལ summary_width)
{
	FILE *f = stderr;

	གལ་ཏེ (verbosity < 0)
		ཕྱིར;

	strbuf_reset(&display_state->buf);

	switch (display_state->format) {
	pak_sa DISPLAY_FORMAT_FULL:
	pak_sa DISPLAY_FORMAT_COMPACT: {
		ཧྲིལ width;

		གལ་ཏེ (!display_state->shown_url) {
			strbuf_addf(&display_state->buf, _("From %.*s\n"),
				    display_state->url_len, display_state->url);
			display_state->shown_url = 1;
		}

		width = (summary_width + strlen(summary) - gettext_width(summary));
		remote = prettify_refname(remote);
		local = prettify_refname(local);

		strbuf_addf(&display_state->buf, " %c %-*s ", code, width, summary);

		གལ་ཏེ (display_state->format != DISPLAY_FORMAT_COMPACT)
			print_remote_to_local(display_state, remote, local);
		དེ་མིན
			print_compact(display_state, remote, local);

		གལ་ཏེ (error)
			strbuf_addf(&display_state->buf, "  (%s)", error);

		bha_nga;
	}
	pak_sa DISPLAY_FORMAT_PORCELAIN:
		strbuf_addf(&display_state->buf, "%c %s %s %s", code,
			    oid_to_hex(old_oid), oid_to_hex(new_oid), local);
		f = stdout;
		bha_nga;
	default:
		BUG("unexpected display format %d", display_state->format);
	};
	strbuf_addch(&display_state->buf, '\n');

	fputs(display_state->buf.buf, f);
}

sa_nracanaa ref_update_display_info {
	bool failed;
	var_na success_code;
	var_na fail_code;
	var_na *summary;
	var_na *fail_detail;
	var_na *success_detail;
	var_na *ref;
	var_na *remote;
	sa_nracanaa object_id old_oid;
	sa_nracanaa object_id new_oid;
};

sa_nracanaa ref_update_display_info_array {
	sa_nracanaa ref_update_display_info *info;
	size_t alloc, nr;
};

static sa_nracanaa ref_update_display_info *ref_update_display_info_append(
					   sa_nracanaa ref_update_display_info_array *array,
					   var_na success_code,
					   var_na fail_code,
					   const var_na *summary,
					   const var_na *success_detail,
					   const var_na *fail_detail,
					   const var_na *ref,
					   const var_na *remote,
					   const sa_nracanaa object_id *old_oid,
					   const sa_nracanaa object_id *new_oid)
{
	sa_nracanaa ref_update_display_info *info;

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

	ཕྱིར info;
}

static void ref_update_display_info_set_failed(sa_nracanaa ref_update_display_info *info)
{
	info->failed = true;
}

static void ref_update_display_info_free(sa_nracanaa ref_update_display_info *info)
{
	free(info->summary);
	free(info->success_detail);
	free(info->fail_detail);
	free(info->remote);
	free(info->ref);
}

static void ref_update_display_info_display(sa_nracanaa ref_update_display_info *info,
					    sa_nracanaa display_state *display_state,
					    ཧྲིལ summary_width)
{
	display_ref_update(display_state,
			   info->failed ? info->fail_code : info->success_code,
			   info->summary,
			   info->failed ? info->fail_detail : info->success_detail,
			   info->remote, info->ref, &info->old_oid,
			   &info->new_oid, summary_width);
}

static ཧྲིལ update_local_ref(sa_nracanaa ref *ref,
			    sa_nracanaa ref_transaction *transaction,
			    const sa_nracanaa ref *remote_ref,
			    const sa_nracanaa fetch_config *config,
			    sa_nracanaa ref_update_display_info_array *display_array)
{
	sa_nracanaa commit *current = NULL, *updated;
	ཧྲིལ fast_forward = 0;

	གལ་ཏེ (!odb_has_object(the_repository->objects, &ref->new_oid,
			    ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR))
		die(_("object %s not found"), oid_to_hex(&ref->new_oid));

	གལ་ཏེ (oideq(&ref->old_oid, &ref->new_oid)) {
		གལ་ཏེ (verbosity > 0)
			ref_update_display_info_append(display_array, '=', '=',
						       _("[up to date]"), NULL,
						       NULL, ref->name,
						       remote_ref->name, &ref->old_oid,
						       &ref->new_oid);
		ཕྱིར 0;
	}

	གལ་ཏེ (!update_head_ok &&
	    !is_null_oid(&ref->old_oid) &&
	    branch_checked_out(ref->name)) {
		sa_nracanaa ref_update_display_info *info;
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
		ཕྱིར 1;
	}

	གལ་ཏེ (!is_null_oid(&ref->old_oid) &&
	    starts_with(ref->name, "refs/tags/")) {
		sa_nracanaa ref_update_display_info *info;

		གལ་ཏེ (force || ref->force) {
			ཧྲིལ r;

			r = s_update_ref("updating tag", ref, transaction, 0);

			info = ref_update_display_info_append(display_array, 't', '!',
							      _("[tag update]"), NULL,
							      _("unable to update local ref"),
							      ref->name, remote_ref->name,
							      &ref->old_oid, &ref->new_oid);
			གལ་ཏེ (r)
				ref_update_display_info_set_failed(info);

			ཕྱིར r;
		} དེ་མིན {
			info = ref_update_display_info_append(display_array, '!', '!',
							      _("[rejected]"), NULL,
							      _("would clobber existing tag"),
							      ref->name, remote_ref->name,
							      &ref->old_oid, &ref->new_oid);
			ref_update_display_info_set_failed(info);
			ཕྱིར 1;
		}
	}

	current = lookup_commit_reference_gently(the_repository,
						 &ref->old_oid, 1);
	updated = lookup_commit_reference_gently(the_repository,
						 &ref->new_oid, 1);
	གལ་ཏེ (!current || !updated) {
		sa_nracanaa ref_update_display_info *info;
		const var_na *msg;
		const var_na *what;
		ཧྲིལ r;
		/*
		 * Nicely describe the new ref we're fetching.
		 * Base this on the remote's ref name, as it's
		 * more likely to follow a standard layout.
		 */
		གལ་ཏེ (starts_with(remote_ref->name, "refs/tags/")) {
			msg = "storing tag";
			what = _("[new tag]");
		} དེ་མིན གལ་ཏེ (starts_with(remote_ref->name, "refs/heads/")) {
			msg = "storing head";
			what = _("[new branch]");
		} དེ་མིན {
			msg = "storing ref";
			what = _("[new ref]");
		}

		r = s_update_ref(msg, ref, transaction, 0);

		info = ref_update_display_info_append(display_array, '*', '!',
						      what, NULL,
						      _("unable to update local ref"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);
		གལ་ཏེ (r)
			ref_update_display_info_set_failed(info);

		ཕྱིར r;
	}

	གལ་ཏེ (config->show_forced_updates) {
		uint64_t t_before = getnanotime();
		fast_forward = repo_in_merge_bases(the_repository, current,
						   updated);
		གལ་ཏེ (fast_forward < 0)
			die(NULL);
		forced_updates_ms += (getnanotime() - t_before) / 1000000;
	} དེ་མིན {
		fast_forward = 1;
	}

	གལ་ཏེ (fast_forward) {
		sa_nracanaa ref_update_display_info *info;
		sa_nracanaa strbuf quickref = STRBUF_INIT;
		ཧྲིལ r;

		strbuf_add_unique_abbrev(&quickref, &current->object.oid, DEFAULT_ABBREV);
		strbuf_addstr(&quickref, "..");
		strbuf_add_unique_abbrev(&quickref, &ref->new_oid, DEFAULT_ABBREV);
		r = s_update_ref("fast-forward", ref, transaction, 1);

		info = ref_update_display_info_append(display_array, ' ', '!',
						      quickref.buf, NULL,
						      _("unable to update local ref"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);
		གལ་ཏེ (r)
			ref_update_display_info_set_failed(info);

		strbuf_release(&quickref);
		ཕྱིར r;
	} དེ་མིན གལ་ཏེ (force || ref->force) {
		sa_nracanaa ref_update_display_info *info;
		sa_nracanaa strbuf quickref = STRBUF_INIT;
		ཧྲིལ r;

		strbuf_add_unique_abbrev(&quickref, &current->object.oid, DEFAULT_ABBREV);
		strbuf_addstr(&quickref, "...");
		strbuf_add_unique_abbrev(&quickref, &ref->new_oid, DEFAULT_ABBREV);
		r = s_update_ref("forced-update", ref, transaction, 1);

		info = ref_update_display_info_append(display_array, '+', '!',
						      quickref.buf, _("forced update"),
						      _("unable to update local ref"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);

		གལ་ཏེ (r)
			ref_update_display_info_set_failed(info);

		strbuf_release(&quickref);
		ཕྱིར r;
	} དེ་མིན {
		sa_nracanaa ref_update_display_info *info;
		info = ref_update_display_info_append(display_array, '!', '!',
						      _("[rejected]"), NULL,
						      _("non-fast-forward"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);
		ref_update_display_info_set_failed(info);
		ཕྱིར 1;
	}
}

static const sa_nracanaa object_id *iterate_ref_map(void *cb_data)
{
	sa_nracanaa ref **rm = cb_data;
	sa_nracanaa ref *ref = *rm;

	སྐབས (ref && ref->status == REF_STATUS_REJECT_SHALLOW)
		ref = ref->next;
	གལ་ཏེ (!ref)
		ཕྱིར NULL;
	*rm = ref->next;
	ཕྱིར &ref->old_oid;
}

sa_nracanaa fetch_head {
	FILE *fp;
	sa_nracanaa strbuf buf;
};

static ཧྲིལ open_fetch_head(sa_nracanaa fetch_head *fetch_head)
{
	const var_na *filename = git_path_fetch_head(the_repository);

	གལ་ཏེ (write_fetch_head) {
		fetch_head->fp = fopen(filename, "a");
		གལ་ཏེ (!fetch_head->fp)
			ཕྱིར error_errno(_("cannot open '%s'"), filename);
		strbuf_init(&fetch_head->buf, 0);
	} དེ་མིན {
		fetch_head->fp = NULL;
	}

	ཕྱིར 0;
}

static void append_fetch_head(sa_nracanaa fetch_head *fetch_head,
			      const sa_nracanaa object_id *old_oid,
			      ga_nanaa fetch_head_status fetch_head_status,
			      const var_na *note,
			      const var_na *url, size_t url_len)
{
	var_na old_oid_hex[GIT_MAX_HEXSZ + 1];
	const var_na *merge_status_marker;
	size_t i;

	གལ་ཏེ (!fetch_head->fp)
		ཕྱིར;

	switch (fetch_head_status) {
	pak_sa FETCH_HEAD_NOT_FOR_MERGE:
		merge_status_marker = "not-for-merge";
		bha_nga;
	pak_sa FETCH_HEAD_MERGE:
		merge_status_marker = "";
		bha_nga;
	default:
		/* do not write anything to FETCH_HEAD */
		ཕྱིར;
	}

	strbuf_addf(&fetch_head->buf, "%s\t%s\t%s",
		    oid_to_hex_r(old_oid_hex, old_oid), merge_status_marker, note);
	ཆེད་དུ (i = 0; i < url_len; ++i)
		གལ་ཏེ ('\n' == url[i])
			strbuf_addstr(&fetch_head->buf, "\\n");
		དེ་མིན
			strbuf_addch(&fetch_head->buf, url[i]);
	strbuf_addch(&fetch_head->buf, '\n');

	/*
	 * When using an atomic fetch, we do not want to update FETCH_HEAD if
	 * any of the reference updates fails. We thus have to write all
	 * updates to a buffer first and only commit it as soon as all
	 * references have been successfully updated.
	 */
	གལ་ཏེ (!atomic_fetch) {
		strbuf_write(&fetch_head->buf, fetch_head->fp);
		strbuf_reset(&fetch_head->buf);
	}
}

static void commit_fetch_head(sa_nracanaa fetch_head *fetch_head)
{
	གལ་ཏེ (!fetch_head->fp || !atomic_fetch)
		ཕྱིར;
	strbuf_write(&fetch_head->buf, fetch_head->fp);
}

static void close_fetch_head(sa_nracanaa fetch_head *fetch_head)
{
	གལ་ཏེ (!fetch_head->fp)
		ཕྱིར;

	fclose(fetch_head->fp);
	strbuf_release(&fetch_head->buf);
}

static const var_na warn_show_forced_updates[] =
N_("fetch normally indicates which branches had a forced update,\n"
   "but that check has been disabled; to re-enable, use '--show-forced-updates'\n"
   "flag or run 'git config fetch.showForcedUpdates true'");
static const var_na warn_time_show_forced_updates[] =
N_("it took %.2f seconds to check forced updates; you can use\n"
   "'--no-show-forced-updates' or run 'git config fetch.showForcedUpdates false'\n"
   "to avoid this check\n");

static ཧྲིལ store_updated_refs(sa_nracanaa display_state *display_state,
			      ཧྲིལ connectivity_checked,
			      sa_nracanaa ref_transaction *transaction, sa_nracanaa ref *ref_map,
			      sa_nracanaa fetch_head *fetch_head,
			      const sa_nracanaa fetch_config *config,
			      sa_nracanaa ref_update_display_info_array *display_array)
{
	ཧྲིལ rc = 0;
	sa_nracanaa strbuf note = STRBUF_INIT;
	const var_na *what, *kind;
	sa_nracanaa ref *rm;
	ཧྲིལ want_status;

	གལ་ཏེ (!connectivity_checked) {
		sa_nracanaa check_connected_options opt = CHECK_CONNECTED_INIT;

		opt.exclude_hidden_refs_section = "fetch";
		rm = ref_map;
		གལ་ཏེ (check_connected(iterate_ref_map, &rm, &opt)) {
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
	ཆེད་དུ (want_status = FETCH_HEAD_MERGE;
	     want_status <= FETCH_HEAD_IGNORE;
	     want_status++) {
		ཆེད་དུ (rm = ref_map; rm; rm = rm->next) {
			sa_nracanaa ref *ref = NULL;

			གལ་ཏེ (rm->status == REF_STATUS_REJECT_SHALLOW) {
				གལ་ཏེ (want_status == FETCH_HEAD_MERGE)
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
			གལ་ཏེ (fetch_head->fp) {
				sa_nracanaa commit *commit = NULL;

				/*
				 * References in "refs/tags/" are often going to point
				 * to annotated tags, which are not part of the
				 * commit-graph. We thus only try to look up refs in
				 * the graph which are not in that namespace to not
				 * regress performance in repositories with many
				 * annotated tags.
				 */
				གལ་ཏེ (!starts_with(rm->name, "refs/tags/"))
					commit = lookup_commit_in_graph(the_repository, &rm->old_oid);
				གལ་ཏེ (!commit) {
					commit = lookup_commit_reference_gently(the_repository,
										&rm->old_oid,
										1);
					གལ་ཏེ (!commit)
						rm->fetch_head_status = FETCH_HEAD_NOT_FOR_MERGE;
				}
			}

			གལ་ཏེ (rm->fetch_head_status != want_status)
				continue;

			གལ་ཏེ (rm->peer_ref) {
				ref = alloc_ref(rm->peer_ref->name);
				oidcpy(&ref->old_oid, &rm->peer_ref->old_oid);
				oidcpy(&ref->new_oid, &rm->old_oid);
				ref->force = rm->peer_ref->force;
			}

			གལ་ཏེ (config->recurse_submodules != RECURSE_SUBMODULES_OFF &&
			    (!rm->peer_ref || !oideq(&ref->old_oid, &ref->new_oid))) {
				check_for_new_submodule_commits(&rm->old_oid);
			}

			གལ་ཏེ (!strcmp(rm->name, "HEAD")) {
				kind = "";
				what = "";
			} དེ་མིན གལ་ཏེ (skip_prefix(rm->name, "refs/heads/", &what)) {
				kind = "branch";
			} དེ་མིན གལ་ཏེ (skip_prefix(rm->name, "refs/tags/", &what)) {
				kind = "tag";
			} དེ་མིན གལ་ཏེ (skip_prefix(rm->name, "refs/remotes/", &what)) {
				kind = "remote-tracking branch";
			} དེ་མིན {
				kind = "";
				what = rm->name;
			}

			strbuf_reset(&note);
			གལ་ཏེ (*what) {
				གལ་ཏེ (*kind)
					strbuf_addf(&note, "%s ", kind);
				strbuf_addf(&note, "'%s' of ", what);
			}

			append_fetch_head(fetch_head, &rm->old_oid,
					  rm->fetch_head_status,
					  note.buf, display_state->url,
					  display_state->url_len);

			གལ་ཏེ (ref) {
				rc |= update_local_ref(ref, transaction, rm,
						       config, display_array);
				free(ref);
			} དེ་མིན གལ་ཏེ (write_fetch_head || dry_run) {
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

	གལ་ཏེ (advice_enabled(ADVICE_FETCH_SHOW_FORCED_UPDATES)) {
		གལ་ཏེ (!config->show_forced_updates) {
			warning(_(warn_show_forced_updates));
		} དེ་མིན གལ་ཏེ (forced_updates_ms > FORCED_UPDATES_DELAY_WARNING_IN_MS) {
			warning(_(warn_time_show_forced_updates),
				forced_updates_ms / 1000.0);
		}
	}

 abort:
	strbuf_release(&note);
	ཕྱིར rc;
}

/*
 * We would want to bypass the object transfer altogether if
 * everything we are going to fetch already exists and is connected
 * locally.
 */
static ཧྲིལ check_exist_and_connected(sa_nracanaa ref *ref_map)
{
	sa_nracanaa ref *rm = ref_map;
	sa_nracanaa check_connected_options opt = CHECK_CONNECTED_INIT;
	sa_nracanaa ref *r;

	/*
	 * If we are deepening a shallow clone we already have these
	 * objects reachable.  Running rev-list here will return with
	 * a good (0) exit status and we'll bypass the fetch that we
	 * really need to perform.  Claiming failure now will ensure
	 * we perform the network exchange to deepen our history.
	 */
	གལ་ཏེ (deepen)
		ཕྱིར -1;

	/*
	 * Similarly, if we need to refetch, we always want to perform a full
	 * fetch ignoring existing objects.
	 */
	གལ་ཏེ (refetch)
		ཕྱིར -1;


	/*
	 * check_connected() allows objects to merely be promised, but
	 * we need all direct targets to exist.
	 */
	ཆེད་དུ (r = rm; r; r = r->next) {
		གལ་ཏེ (!odb_has_object(the_repository->objects, &r->old_oid,
				    ODB_HAS_OBJECT_RECHECK_PACKED))
			ཕྱིར -1;
	}

	opt.quiet = 1;
	opt.exclude_hidden_refs_section = "fetch";
	ཕྱིར check_connected(iterate_ref_map, &rm, &opt);
}

static ཧྲིལ fetch_and_consume_refs(sa_nracanaa display_state *display_state,
				  sa_nracanaa transport *transport,
				  sa_nracanaa ref_transaction *transaction,
				  sa_nracanaa ref *ref_map,
				  sa_nracanaa fetch_head *fetch_head,
				  const sa_nracanaa fetch_config *config,
				  sa_nracanaa ref_update_display_info_array *display_array)
{
	ཧྲིལ connectivity_checked = 1;
	ཧྲིལ ret;

	/*
	 * We don't need to perform a fetch in case we can already satisfy all
	 * refs.
	 */
	ret = check_exist_and_connected(ref_map);
	གལ་ཏེ (ret) {
		trace2_region_enter("fetch", "fetch_refs", the_repository);
		ret = transport_fetch_refs(transport, ref_map);
		trace2_region_leave("fetch", "fetch_refs", the_repository);
		གལ་ཏེ (ret)
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
	ཕྱིར ret;
}

static ཧྲིལ prune_refs(sa_nracanaa display_state *display_state,
		      sa_nracanaa refspec *rs,
		      sa_nracanaa ref_transaction *transaction,
		      sa_nracanaa ref *ref_map)
{
	ཧྲིལ result = 0;
	sa_nracanaa ref *ref, *stale_refs = get_stale_heads(rs, ref_map);
	sa_nracanaa strbuf err = STRBUF_INIT;
	sa_nracanaa string_list refnames = STRING_LIST_INIT_NODUP;

	ཆེད་དུ (ref = stale_refs; ref; ref = ref->next)
		string_list_append(&refnames, ref->name);

	གལ་ཏེ (!dry_run) {
		གལ་ཏེ (transaction) {
			ཆེད་དུ (ref = stale_refs; ref; ref = ref->next) {
				result = ref_transaction_delete(transaction, ref->name, NULL,
								NULL, 0, "fetch: prune", &err);
				གལ་ཏེ (result)
					goto cleanup;
			}
		} དེ་མིན {
			result = refs_delete_refs(get_main_ref_store(the_repository),
						  "fetch: prune", &refnames,
						  0);
		}
	}

	གལ་ཏེ (verbosity >= 0) {
		ཧྲིལ summary_width = transport_summary_width(stale_refs);

		ཆེད་དུ (ref = stale_refs; ref; ref = ref->next) {
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
	ཕྱིར result;
}

static void check_not_current_branch(sa_nracanaa ref *ref_map)
{
	const var_na *path;
	ཆེད་དུ (; ref_map; ref_map = ref_map->next)
		གལ་ཏེ (ref_map->peer_ref &&
		    starts_with(ref_map->peer_ref->name, "refs/heads/") &&
		    (path = branch_checked_out(ref_map->peer_ref->name)))
			die(_("refusing to fetch into branch '%s' "
			      "checked out at '%s'"),
			    ref_map->peer_ref->name, path);
}

static ཧྲིལ truncate_fetch_head(void)
{
	const var_na *filename = git_path_fetch_head(the_repository);
	FILE *fp = fopen_for_writing(filename);

	གལ་ཏེ (!fp)
		ཕྱིར error_errno(_("cannot open '%s'"), filename);
	fclose(fp);
	ཕྱིར 0;
}

static void set_option(sa_nracanaa transport *transport, const var_na *name, const var_na *value)
{
	ཧྲིལ r = transport_set_option(transport, name, value);
	གལ་ཏེ (r < 0)
		die(_("option \"%s\" value \"%s\" is not valid for %s"),
		    name, value, transport->url);
	གལ་ཏེ (r > 0)
		warning(_("option \"%s\" is ignored for %s"),
			name, transport->url);
}


static ཧྲིལ add_oid(const sa_nracanaa reference *ref, void *cb_data)
{
	sa_nracanaa oid_array *oids = cb_data;

	oid_array_append(oids, ref->oid);
	ཕྱིར 0;
}

static void add_negotiation_tips(sa_nracanaa string_list *input_list,
				 sa_nracanaa oid_array **output_list,
				 const var_na *argname)
{
	sa_nracanaa oid_array *oids = xcalloc(1, parimaa_na(*oids));
	ཧྲིལ i;

	ཆེད་དུ (i = 0; i < input_list->nr; i++) {
		const var_na *s = input_list->items[i].string;
		sa_nracanaa refs_for_each_ref_options opts = {
			.pattern = s,
		};
		ཧྲིལ old_nr;
		གལ་ཏེ (!has_glob_specials(s)) {
			sa_nracanaa object_id oid;

			/* Ignore missing reference. */
			གལ་ཏེ (repo_get_oid(the_repository, s, &oid))
				continue;
			/* Fail on missing object pointed by ref. */
			གལ་ཏེ (!odb_has_object(the_repository->objects, &oid, 0))
				die(_("the object %s does not exist"), s);

			oid_array_append(oids, &oid);
			continue;
		}
		old_nr = oids->nr;
		refs_for_each_ref_ext(get_main_ref_store(the_repository),
				      add_oid, oids, &opts);
		གལ་ཏེ (old_nr == oids->nr)
			warning(_("ignoring %s=%s because it does not match any refs"),
				argname, s);
	}
	*output_list = oids;
}

static sa_nracanaa transport *prepare_transport(sa_nracanaa remote *remote, ཧྲིལ deepen,
					   sa_nracanaa list_objects_filter_options *filter_options)
{
	sa_nracanaa transport *transport;

	transport = transport_get(remote, NULL);
	transport_set_verbosity(transport, verbosity, progress);
	transport->family = family;
	གལ་ཏེ (upload_pack)
		set_option(transport, TRANS_OPT_UPLOADPACK, upload_pack);
	གལ་ཏེ (keep)
		set_option(transport, TRANS_OPT_KEEP, "yes");
	གལ་ཏེ (depth)
		set_option(transport, TRANS_OPT_DEPTH, depth);
	གལ་ཏེ (deepen && deepen_since)
		set_option(transport, TRANS_OPT_DEEPEN_SINCE, deepen_since);
	གལ་ཏེ (deepen && deepen_not.nr)
		set_option(transport, TRANS_OPT_DEEPEN_NOT,
			   (const var_na *)&deepen_not);
	གལ་ཏེ (deepen_relative)
		set_option(transport, TRANS_OPT_DEEPEN_RELATIVE, "yes");
	གལ་ཏེ (update_shallow)
		set_option(transport, TRANS_OPT_UPDATE_SHALLOW, "yes");
	གལ་ཏེ (refetch)
		set_option(transport, TRANS_OPT_REFETCH, "yes");
	གལ་ཏེ (filter_options->choice) {
		const var_na *spec =
			expand_list_objects_filter_spec(filter_options);
		set_option(transport, TRANS_OPT_LIST_OBJECTS_FILTER, spec);
		set_option(transport, TRANS_OPT_FROM_PROMISOR, "1");
	}
	གལ་ཏེ (negotiation_restrict.nr) {
		གལ་ཏེ (transport->smart_options)
			add_negotiation_tips(&negotiation_restrict,
					     &transport->smart_options->negotiation_restrict_tips,
					     "--negotiation-restrict");
		དེ་མིན
			warning(_("ignoring %s because the protocol does not support it"),
				"--negotiation-restrict");
	} དེ་མིན གལ་ཏེ (remote->negotiation_restrict.nr) {
		sa_nracanaa string_list_item *item;
		for_each_string_list_item(item, &remote->negotiation_restrict)
			string_list_append(&negotiation_restrict, item->string);
		གལ་ཏེ (transport->smart_options)
			add_negotiation_tips(&negotiation_restrict,
					     &transport->smart_options->negotiation_restrict_tips,
					     "--negotiation-restrict");
		དེ་མིན {
			sa_nracanaa strbuf config_name = STRBUF_INIT;
			strbuf_addf(&config_name, "remote.%s.negotiationRestrict", remote->name);
			warning(_("ignoring %s because the protocol does not support it"),
				config_name.buf);
			strbuf_release(&config_name);
		}
	}
	གལ་ཏེ (negotiation_include.nr) {
		གལ་ཏེ (transport->smart_options)
			add_negotiation_tips(&negotiation_include,
					     &transport->smart_options->negotiation_include_tips,
					     "--negotiation-include");
		དེ་མིན
			warning(_("ignoring %s because the protocol does not support it"),
				"--negotiation-include");
	} དེ་མིན གལ་ཏེ (remote->negotiation_include.nr) {
		གལ་ཏེ (transport->smart_options) {
			add_negotiation_tips(&remote->negotiation_include,
					     &transport->smart_options->negotiation_include_tips,
					     "--negotiation-include");
		} དེ་མིན {
			sa_nracanaa strbuf config_name = STRBUF_INIT;
			strbuf_addf(&config_name, "remote.%s.negotiationInclude", remote->name);
			warning(_("ignoring %s because the protocol does not support it"),
				config_name.buf);
			strbuf_release(&config_name);
		}
	}
	ཕྱིར transport;
}

static ཧྲིལ backfill_tags(sa_nracanaa display_state *display_state,
			 sa_nracanaa transport *transport,
			 sa_nracanaa ref_transaction *transaction,
			 sa_nracanaa ref *ref_map,
			 sa_nracanaa fetch_head *fetch_head,
			 const sa_nracanaa fetch_config *config,
			 sa_nracanaa ref_update_display_info_array *display_array,
			 sa_nracanaa list_objects_filter_options *filter_options)
{
	ཧྲིལ retcode, cannot_reuse;

	/*
	 * Once we have set TRANS_OPT_DEEPEN_SINCE, we can't unset it
	 * when remote helper is used (setting it to an empty string
	 * is not unsetting). We could extend the remote helper
	 * protocol for that, but for now, just force a new connection
	 * without deepen-since. Similar story for deepen-not.
	 */
	cannot_reuse = transport->cannot_reuse ||
		deepen_since || deepen_not.nr;
	གལ་ཏེ (cannot_reuse) {
		gsecondary = prepare_transport(transport->remote, 0, filter_options);
		transport = gsecondary;
	}

	transport_set_option(transport, TRANS_OPT_FOLLOWTAGS, NULL);
	transport_set_option(transport, TRANS_OPT_DEPTH, "0");
	transport_set_option(transport, TRANS_OPT_DEEPEN_RELATIVE, NULL);
	retcode = fetch_and_consume_refs(display_state, transport, transaction, ref_map,
					 fetch_head, config, display_array);

	གལ་ཏེ (gsecondary) {
		transport_disconnect(gsecondary);
		gsecondary = NULL;
	}

	ཕྱིར retcode;
}

static const var_na *strip_refshead(const var_na *name){
	skip_prefix(name, "refs/heads/", &name);
	ཕྱིར name;
}

static void set_head_advice_msg(const var_na *remote, const var_na *head_name)
{
	const var_na message_advice_set_head[] =
	N_("Run 'git remote set-head %s %s' to follow the change, or modify\n"
	   "either of the 'remote.%s.followRemoteHEAD' or 'fetch.followRemoteHEAD'\n"
	   "configuration variables to handle the situation differently.\n\n"

	   "Using this specific setting\n\n"
	   "    git config set remote.%s.followRemoteHEAD warn-if-not-%s\n\n"
	   "will suppress the warning until the remote changes HEAD to something else.");

	advise_if_enabled(ADVICE_FETCH_SET_HEAD_WARN, _(message_advice_set_head),
			remote, head_name, remote, remote, head_name);
}

static void warn_set_head(const var_na *remote, const var_na *head_name,
			sa_nracanaa strbuf *buf_prev, ཧྲིལ updateres) {
	sa_nracanaa strbuf buf_prefix = STRBUF_INIT;
	const var_na *prev_head = NULL;

	strbuf_addf(&buf_prefix, "refs/remotes/%s/", remote);
	skip_prefix(buf_prev->buf, buf_prefix.buf, &prev_head);

	གལ་ཏེ (prev_head && strcmp(prev_head, head_name)) {
		printf("'HEAD' at '%s' is '%s', but we have '%s' locally.\n",
			remote, head_name, prev_head);
		set_head_advice_msg(remote, head_name);
	}
	དེ་མིན གལ་ཏེ (updateres && buf_prev->len) {
		printf("'HEAD' at '%s' is '%s', "
			"but we have a detached HEAD pointing to '%s' locally.\n",
			remote, head_name, buf_prev->buf);
		set_head_advice_msg(remote, head_name);
	}
	strbuf_release(&buf_prefix);
}

static ཧྲིལ set_head(const sa_nracanaa ref *remote_refs, sa_nracanaa remote *remote,
			ཧྲིལ follow_remote_head)
{
	ཧྲིལ result = 0, create_only, baremirror, was_detached;
	sa_nracanaa strbuf b_head = STRBUF_INIT, b_remote_head = STRBUF_INIT,
		      b_local_head = STRBUF_INIT;
	const var_na *no_warn_branch = remote->no_warn_branch;
	var_na *head_name = NULL;
	sa_nracanaa ref *ref, *matches;
	sa_nracanaa ref *fetch_map = NULL, **fetch_map_tail = &fetch_map;
	sa_nracanaa refspec_item refspec = {
		.force = 0,
		.pattern = 1,
		.src = (var_na *) "refs/heads/*",
		.dst = (var_na *) "refs/heads/*",
	};
	sa_nracanaa string_list heads = STRING_LIST_INIT_DUP;
	sa_nracanaa ref_store *refs = get_main_ref_store(the_repository);

	get_fetch_map(remote_refs, &refspec, &fetch_map_tail, 0);
	matches = guess_remote_head(find_ref_by_name(remote_refs, "HEAD"),
				    fetch_map, REMOTE_GUESS_HEAD_ALL);
	ཆེད་དུ (ref = matches; ref; ref = ref->next) {
		string_list_append(&heads, strip_refshead(ref->name));
	}

	གལ་ཏེ (!heads.nr)
		result = 1;
	དེ་མིན གལ་ཏེ (heads.nr > 1)
		result = 1;
	དེ་མིན
		head_name = xstrdup(heads.items[0].string);

	གལ་ཏེ (!head_name)
		goto cleanup;
	baremirror = is_bare_repository(the_repository) && remote->mirror;
	create_only = follow_remote_head == FOLLOW_REMOTE_ALWAYS ? 0 : !baremirror;
	གལ་ཏེ (baremirror) {
		strbuf_addstr(&b_head, "HEAD");
		strbuf_addf(&b_remote_head, "refs/heads/%s", head_name);
	} དེ་མིན {
		strbuf_addf(&b_head, "refs/remotes/%s/HEAD", remote->name);
		strbuf_addf(&b_remote_head, "refs/remotes/%s/%s", remote->name, head_name);
	}
	/* make sure it's valid */
	གལ་ཏེ (!baremirror && !refs_ref_exists(refs, b_remote_head.buf)) {
		result = 1;
		goto cleanup;
	}
	was_detached = refs_update_symref_extended(refs, b_head.buf, b_remote_head.buf,
					"fetch", &b_local_head, create_only);
	གལ་ཏེ (was_detached == -1) {
		result = 1;
		goto cleanup;
	}
	གལ་ཏེ (verbosity >= 0 &&
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
	ཕྱིར result;
}

sa_nracanaa ref_rejection_data {
	ཧྲིལ *retcode;
	bool conflict_msg_shown;
	bool case_sensitive_msg_shown;
	const var_na *remote_name;
	sa_nracanaa strmap *rejected_refs;
};

static void ref_transaction_rejection_handler(const var_na *refname,
					      const sa_nracanaa object_id *old_oid UNUSED,
					      const sa_nracanaa object_id *new_oid UNUSED,
					      const var_na *old_target UNUSED,
					      const var_na *new_target UNUSED,
					      ga_nanaa ref_transaction_error err,
					      const var_na *details,
					      void *cb_data)
{
	sa_nracanaa ref_rejection_data *data = cb_data;

	གལ་ཏེ (err == REF_TRANSACTION_ERROR_CASE_CONFLICT && repo_ignore_case(the_repository) &&
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
	} དེ་མིན གལ་ཏེ (err == REF_TRANSACTION_ERROR_NAME_CONFLICT &&
		   !data->conflict_msg_shown) {
		error(_("some local refs could not be updated; try running\n"
			" 'git remote prune %s' to remove any old, conflicting "
			"branches"), data->remote_name);
		data->conflict_msg_shown = true;
	} དེ་མིན {
		གལ་ཏེ (details)
			error("%s", details);
		དེ་མིན
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
static ཧྲིལ commit_ref_transaction(sa_nracanaa ref_transaction **transaction,
				  bool is_atomic, const var_na *remote_name,
				  sa_nracanaa strmap *rejected_refs,
				  sa_nracanaa strbuf *err)
{
	ཧྲིལ retcode = ref_transaction_commit(*transaction, err);
	གལ་ཏེ (retcode)
		goto out;

	གལ་ཏེ (!is_atomic) {
		sa_nracanaa ref_rejection_data data = {
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
	ཕྱིར retcode;
}

static void collect_updated_tips(sa_nracanaa oidset *tips, sa_nracanaa ref *ref_map)
{
	sa_nracanaa ref *rm;
	ཆེད་དུ (rm = ref_map; rm; rm = rm->next) {
		sa_nracanaa commit *commit;
		/*
		 * Shallow-rejected refs are not stored and their history
		 * is incomplete, so skip them.
		 */
		གལ་ཏེ (rm->status == REF_STATUS_REJECT_SHALLOW)
			continue;
		གལ་ཏེ (is_null_oid(&rm->old_oid))
			continue;
		གལ་ཏེ (rm->peer_ref &&
		    oideq(&rm->old_oid, &rm->peer_ref->old_oid))
			continue;
		commit = lookup_commit_reference_gently(the_repository,
							&rm->old_oid, 1);
		གལ་ཏེ (commit)
			oidset_insert(tips, &commit->object.oid);
	}
}

static ཧྲིལ do_fetch(sa_nracanaa transport *transport,
		    sa_nracanaa refspec *rs,
		    const sa_nracanaa fetch_config *config,
		    sa_nracanaa list_objects_filter_options *filter_options,
		    sa_nracanaa oidset *updated_tips)
{
	sa_nracanaa ref_transaction *transaction = NULL;
	sa_nracanaa ref *ref_map = NULL;
	sa_nracanaa display_state display_state = { 0 };
	ཧྲིལ autotags = (transport->remote->fetch_tags == 1);
	ཧྲིལ retcode = 0;
	const sa_nracanaa ref *remote_refs;
	sa_nracanaa transport_ls_refs_options transport_ls_refs_options =
		TRANSPORT_LS_REFS_OPTIONS_INIT;
	sa_nracanaa fetch_head fetch_head = { 0 };
	sa_nracanaa strbuf err = STRBUF_INIT;
	sa_nracanaa ref_update_display_info_array display_array = { 0 };
	sa_nracanaa strmap rejected_refs = STRMAP_INIT;
	ཧྲིལ summary_width = 0;
	ཧྲིལ follow_remote_head = FOLLOW_REMOTE_NEVER;

	གལ་ཏེ (tags == TAGS_DEFAULT) {
		གལ་ཏེ (transport->remote->fetch_tags == 2)
			tags = TAGS_SET;
		གལ་ཏེ (transport->remote->fetch_tags == -1)
			tags = TAGS_UNSET;
	}

	/* if not appending, truncate FETCH_HEAD */
	གལ་ཏེ (!append && write_fetch_head) {
		retcode = truncate_fetch_head();
		གལ་ཏེ (retcode)
			goto cleanup;
	}

	གལ་ཏེ (rs->nr) {
		refspec_ref_prefixes(rs, &transport_ls_refs_options.ref_prefixes);
	} དེ་མིན {
		sa_nracanaa branch *branch = branch_get(NULL);

		གལ་ཏེ (transport->remote->fetch.nr) {
			refspec_ref_prefixes(&transport->remote->fetch,
					     &transport_ls_refs_options.ref_prefixes);
			/*
			 * See remote.c's handling of remote.<name>.followRemoteHEAD
			 * for the analogous, still-unresolved case.
			 */
			གལ་ཏེ (transport->remote->follow_remote_head)
				follow_remote_head = transport->remote->follow_remote_head;
			དེ་མིན གལ་ཏེ (config->follow_remote_head_seen)
				follow_remote_head = get_follow_remote_head(config->follow_remote_head_raw);
			དེ་མིན
				follow_remote_head = BUILTIN_FOLLOW_REMOTE_HEAD_DFLT;
		}
		གལ་ཏེ (branch && branch_has_merge_config(branch) &&
		    !strcmp(branch->remote_name, transport->remote->name)) {
			ཧྲིལ i;
			ཆེད་དུ (i = 0; i < branch->merge_nr; i++) {
				strvec_push(&transport_ls_refs_options.ref_prefixes,
					    branch->merge[i]->src);
			}
		}

		/*
		 * If there are no refs specified to fetch, then we just
		 * fetch HEAD; mention that to narrow the advertisement.
		 */
		གལ་ཏེ (!transport_ls_refs_options.ref_prefixes.nr)
			strvec_push(&transport_ls_refs_options.ref_prefixes,
				    "HEAD");
	}

	གལ་ཏེ (tags == TAGS_SET || tags == TAGS_DEFAULT)
		strvec_push(&transport_ls_refs_options.ref_prefixes,
			    "refs/tags/");

	གལ་ཏེ (follow_remote_head != FOLLOW_REMOTE_NEVER)
		strvec_push(&transport_ls_refs_options.ref_prefixes,
			    "HEAD");

	/*
	 * Only initiate ref listing if we have at least one ref we want to
	 * know about.
	 */
	གལ་ཏེ (transport_ls_refs_options.ref_prefixes.nr) {
		trace2_region_enter("fetch", "remote_refs", the_repository);
		remote_refs = transport_get_remote_refs(transport,
							&transport_ls_refs_options);
		trace2_region_leave("fetch", "remote_refs", the_repository);
	} དེ་མིན
		remote_refs = NULL;

	transport_ls_refs_options_release(&transport_ls_refs_options);

	ref_map = get_ref_map(transport->remote, remote_refs, rs,
			      tags, &autotags);
	གལ་ཏེ (!update_head_ok)
		check_not_current_branch(ref_map);

	retcode = open_fetch_head(&fetch_head);
	གལ་ཏེ (retcode)
		goto cleanup;

	display_state_init(&display_state, ref_map, transport->url,
			   config->display_format);

	གལ་ཏེ (atomic_fetch) {
		transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
							  0, &err);
		གལ་ཏེ (!transaction) {
			retcode = -1;
			goto cleanup;
		}
	}

	གལ་ཏེ (tags == TAGS_DEFAULT && autotags)
		transport_set_option(transport, TRANS_OPT_FOLLOWTAGS, "1");
	གལ་ཏེ (prune) {
		/*
		 * We only prune based on refspecs specified
		 * explicitly (via command line or configuration); we
		 * don't care whether --tags was specified.
		 */
		གལ་ཏེ (rs->nr) {
			retcode = prune_refs(&display_state, rs, transaction, ref_map);
		} དེ་མིན {
			retcode = prune_refs(&display_state, &transport->remote->fetch,
					     transaction, ref_map);
		}
		གལ་ཏེ (retcode != 0)
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
	གལ་ཏེ (!transaction) {
		transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
							  REF_TRANSACTION_ALLOW_FAILURE, &err);
		གལ་ཏེ (!transaction) {
			retcode = -1;
			goto cleanup;
		}
	}

	གལ་ཏེ (fetch_and_consume_refs(&display_state, transport, transaction, ref_map,
				   &fetch_head, config, &display_array)) {
		retcode = 1;
		goto cleanup;
	}

	/*
	 * If neither --no-tags nor --tags was specified, do automated tag
	 * following.
	 */
	གལ་ཏེ (tags == TAGS_DEFAULT && autotags) {
		sa_nracanaa ref *tags_ref_map = NULL, **tail = &tags_ref_map;

		find_non_local_tags(remote_refs, transaction, &tags_ref_map, &tail);
		གལ་ཏེ (tags_ref_map) {
			/*
			 * If backfilling of tags fails then we want to tell
			 * the user so, but we have to continue regardless to
			 * populate upstream information of the references we
			 * have already fetched above. The exception though is
			 * when `--atomic` is passed: in that case we'll abort
			 * the transaction and don't commit anything.
			 */
			གལ་ཏེ (backfill_tags(&display_state, transport, transaction, tags_ref_map,
					  &fetch_head, config, &display_array, filter_options))
				retcode = 1;
		}

		free_refs(tags_ref_map);
	}

	གལ་ཏེ (retcode)
		goto cleanup;

	གལ་ཏེ (verbosity >= 0)
		summary_width = transport_summary_width(ref_map);

	retcode = commit_ref_transaction(&transaction, atomic_fetch,
					 transport->remote->name,
					 &rejected_refs, &err);
	/*
	 * With '--atomic', bail out if the transaction fails. Without '--atomic',
	 * continue to fetch head and perform other post-fetch operations.
	 */
	གལ་ཏེ (retcode && atomic_fetch)
		goto cleanup;

	commit_fetch_head(&fetch_head);

	collect_updated_tips(updated_tips, ref_map);

	གལ་ཏེ (set_upstream) {
		sa_nracanaa branch *branch = branch_get("HEAD");
		sa_nracanaa ref *rm;
		sa_nracanaa ref *source_ref = NULL;

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
		ཆེད་དུ (rm = ref_map; rm; rm = rm->next) {
			གལ་ཏེ (!rm->peer_ref) {
				གལ་ཏེ (source_ref) {
					warning(_("multiple branches detected, incompatible with --set-upstream"));
					goto cleanup;
				} དེ་མིན {
					source_ref = rm;
				}
			}
		}
		གལ་ཏེ (source_ref) {
			གལ་ཏེ (!branch) {
				const var_na *shortname = source_ref->name;
				skip_prefix(shortname, "refs/heads/", &shortname);

				warning(_("could not set upstream of HEAD to '%s' from '%s' when "
					  "it does not point to any branch."),
					shortname, transport->remote->name);
				goto cleanup;
			}

			གལ་ཏེ (!strcmp(source_ref->name, "HEAD") ||
			    starts_with(source_ref->name, "refs/heads/"))
				install_branch_config(0,
						      branch->name,
						      transport->remote->name,
						      source_ref->name);
			དེ་མིན གལ་ཏེ (starts_with(source_ref->name, "refs/remotes/"))
				warning(_("not setting upstream for a remote remote-tracking branch"));
			དེ་མིན གལ་ཏེ (starts_with(source_ref->name, "refs/tags/"))
				warning(_("not setting upstream for a remote tag"));
			དེ་མིན
				warning(_("unknown branch type"));
		} དེ་མིན {
			warning(_("no source branch found;\n"
				  "you need to specify exactly one branch with the --set-upstream option"));
		}
	}
	གལ་ཏེ (follow_remote_head != FOLLOW_REMOTE_NEVER) {
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
	གལ་ཏེ (retcode && !atomic_fetch && transaction)
		commit_ref_transaction(&transaction, false,
				       transport->remote->name,
				       &rejected_refs, &err);

	ཆེད་དུ (size_t i = 0; i < display_array.nr; i++) {
		sa_nracanaa ref_update_display_info *info = &display_array.info[i];

		གལ་ཏེ (!info->failed && strmap_contains(&rejected_refs, info->ref))
			ref_update_display_info_set_failed(info);
		ref_update_display_info_display(info, &display_state, summary_width);
		ref_update_display_info_free(info);
	}

	གལ་ཏེ (retcode) {
		གལ་ཏེ (err.len) {
			error("%s", err.buf);
			strbuf_reset(&err);
		}
		གལ་ཏེ (transaction && ref_transaction_abort(transaction, &err) &&
		    err.len)
			error("%s", err.buf);
		transaction = NULL;
	}

	གལ་ཏེ (transaction)
		ref_transaction_free(transaction);

	free(display_array.info);
	strmap_clear(&rejected_refs, 0);
	display_state_release(&display_state);
	close_fetch_head(&fetch_head);
	strbuf_release(&err);
	free_refs(ref_map);
	ཕྱིར retcode;
}

static ཧྲིལ get_one_remote_for_fetch(sa_nracanaa remote *remote, void *priv)
{
	sa_nracanaa string_list *list = priv;
	གལ་ཏེ (!remote->skip_default_update)
		string_list_append(list, remote->name);
	ཕྱིར 0;
}

static void add_options_to_argv(sa_nracanaa strvec *argv,
				const sa_nracanaa fetch_config *config)
{
	གལ་ཏེ (dry_run)
		strvec_push(argv, "--dry-run");
	གལ་ཏེ (prune != -1)
		strvec_push(argv, prune ? "--prune" : "--no-prune");
	གལ་ཏེ (prune_tags != -1)
		strvec_push(argv, prune_tags ? "--prune-tags" : "--no-prune-tags");
	གལ་ཏེ (update_head_ok)
		strvec_push(argv, "--update-head-ok");
	གལ་ཏེ (force)
		strvec_push(argv, "--force");
	གལ་ཏེ (keep)
		strvec_push(argv, "--keep");
	གལ་ཏེ (config->recurse_submodules == RECURSE_SUBMODULES_ON)
		strvec_push(argv, "--recurse-submodules");
	དེ་མིན གལ་ཏེ (config->recurse_submodules == RECURSE_SUBMODULES_OFF)
		strvec_push(argv, "--no-recurse-submodules");
	དེ་མིན གལ་ཏེ (config->recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND)
		strvec_push(argv, "--recurse-submodules=on-demand");
	གལ་ཏེ (tags == TAGS_SET)
		strvec_push(argv, "--tags");
	དེ་མིན གལ་ཏེ (tags == TAGS_UNSET)
		strvec_push(argv, "--no-tags");
	གལ་ཏེ (verbosity >= 2)
		strvec_push(argv, "-v");
	གལ་ཏེ (verbosity >= 1)
		strvec_push(argv, "-v");
	དེ་མིན གལ་ཏེ (verbosity < 0)
		strvec_push(argv, "-q");
	གལ་ཏེ (family == TRANSPORT_FAMILY_IPV4)
		strvec_push(argv, "--ipv4");
	དེ་མིན གལ་ཏེ (family == TRANSPORT_FAMILY_IPV6)
		strvec_push(argv, "--ipv6");
	གལ་ཏེ (!write_fetch_head)
		strvec_push(argv, "--no-write-fetch-head");
	གལ་ཏེ (config->display_format == DISPLAY_FORMAT_PORCELAIN)
		strvec_pushf(argv, "--porcelain");
}

/* Fetch multiple remotes in parallel */

sa_nracanaa parallel_fetch_state {
	const var_na **argv;
	sa_nracanaa string_list *remotes;
	ཧྲིལ next, result;
	const sa_nracanaa fetch_config *config;
};

static ཧྲིལ fetch_next_remote(sa_nracanaa child_process *cp,
			     sa_nracanaa strbuf *out UNUSED,
			     void *cb, void **task_cb)
{
	sa_nracanaa parallel_fetch_state *state = cb;
	var_na *remote;

	གལ་ཏེ (state->next < 0 || state->next >= state->remotes->nr)
		ཕྱིར 0;

	remote = state->remotes->items[state->next++].string;
	*task_cb = remote;

	strvec_pushv(&cp->args, state->argv);
	strvec_push(&cp->args, remote);
	cp->git_cmd = 1;

	གལ་ཏེ (verbosity >= 0 && state->config->display_format != DISPLAY_FORMAT_PORCELAIN)
		printf(_("Fetching %s\n"), remote);

	ཕྱིར 1;
}

static ཧྲིལ fetch_failed_to_start(sa_nracanaa strbuf *out UNUSED,
				 void *cb, void *task_cb)
{
	sa_nracanaa parallel_fetch_state *state = cb;
	const var_na *remote = task_cb;

	state->result = error(_("could not fetch %s"), remote);

	ཕྱིར 0;
}

static ཧྲིལ fetch_finished(ཧྲིལ result, sa_nracanaa strbuf *out,
			  void *cb, void *task_cb)
{
	sa_nracanaa parallel_fetch_state *state = cb;
	const var_na *remote = task_cb;

	གལ་ཏེ (result) {
		strbuf_addf(out, _("could not fetch '%s' (exit code: %d)\n"),
			    remote, result);
		state->result = -1;
	}

	ཕྱིར 0;
}

static ཧྲིལ fetch_multiple(sa_nracanaa string_list *list, ཧྲིལ max_children,
			  const sa_nracanaa fetch_config *config)
{
	ཧྲིལ i, result = 0;
	sa_nracanaa strvec argv = STRVEC_INIT;

	གལ་ཏེ (!append && write_fetch_head) {
		ཧྲིལ errcode = truncate_fetch_head();
		གལ་ཏེ (errcode)
			ཕྱིར errcode;
	}

	/*
	 * Cancel out the fetch.bundleURI config when running subprocesses,
	 * to avoid fetching from the same bundle list multiple times.
	 */
	strvec_pushl(&argv, "-c", "fetch.bundleURI=",
		     "fetch", "--append", "--no-auto-gc",
		     "--no-write-commit-graph", NULL);
	ཆེད་དུ (i = 0; i < server_options.nr; i++)
		strvec_pushf(&argv, "--server-option=%s", server_options.items[i].string);
	add_options_to_argv(&argv, config);

	གལ་ཏེ (max_children != 1 && list->nr != 1) {
		sa_nracanaa parallel_fetch_state state = { argv.v, list, 0, 0, config };
		const sa_nracanaa run_process_parallel_opts opts = {
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
	} དེ་མིན
		ཆེད་དུ (i = 0; i < list->nr; i++) {
			const var_na *name = list->items[i].string;
			sa_nracanaa child_process cmd = CHILD_PROCESS_INIT;

			strvec_pushv(&cmd.args, argv.v);
			strvec_push(&cmd.args, name);
			གལ་ཏེ (verbosity >= 0 && config->display_format != DISPLAY_FORMAT_PORCELAIN)
				printf(_("Fetching %s\n"), name);
			cmd.git_cmd = 1;
			གལ་ཏེ (run_command(&cmd)) {
				error(_("could not fetch %s"), name);
				result = 1;
			}
		}

	strvec_clear(&argv);
	ཕྱིར !!result;
}

/*
 * Fetching from the promisor remote should use the given filter-spec
 * or inherit the default filter-spec from the config.
 */
static anta_hstha void fetch_one_setup_partial(sa_nracanaa remote *remote,
					   sa_nracanaa list_objects_filter_options *filter_options)
{
	/*
	 * Explicit --no-filter argument overrides everything, regardless
	 * of any prior partial clones and fetches.
	 */
	གལ་ཏེ (filter_options->no_filter)
		ཕྱིར;

	/*
	 * If no prior partial clone/fetch and the current fetch DID NOT
	 * request a partial-fetch, do a normal fetch.
	 */
	གལ་ཏེ (!repo_has_promisor_remote(the_repository) && !filter_options->choice)
		ཕྱིར;

	/*
	 * If this is a partial-fetch request, we enable partial on
	 * this repo if not already enabled and remember the given
	 * filter-spec as the default for subsequent fetches to this
	 * remote if there is currently no default filter-spec.
	 */
	གལ་ཏེ (filter_options->choice) {
		partial_clone_register(remote->name, filter_options);
		ཕྱིར;
	}

	/*
	 * Do a partial-fetch from the promisor remote using either the
	 * explicitly given filter-spec or inherit the filter-spec from
	 * the config.
	 */
	གལ་ཏེ (!filter_options->choice)
		partial_clone_get_default_filter_spec(filter_options, remote->name);
	ཕྱིར;
}

static ཧྲིལ fetch_one(sa_nracanaa remote *remote, ཧྲིལ argc, const var_na **argv,
		     ཧྲིལ prune_tags_ok, ཧྲིལ use_stdin_refspecs,
		     const sa_nracanaa fetch_config *config,
		     sa_nracanaa list_objects_filter_options *filter_options,
		     sa_nracanaa oidset *updated_tips)
{
	sa_nracanaa refspec rs = REFSPEC_INIT_FETCH(the_hash_algo);
	ཧྲིལ i;
	ཧྲིལ exit_code;
	ཧྲིལ maybe_prune_tags;
	ཧྲིལ remote_via_config = remote_is_configured(remote, 0);

	གལ་ཏེ (!remote)
		die(_("no remote repository specified; please specify either a URL or a\n"
		      "remote name from which new revisions should be fetched"));

	gtransport = prepare_transport(remote, 1, filter_options);

	གལ་ཏེ (prune < 0) {
		/* no command line request */
		གལ་ཏེ (0 <= remote->prune)
			prune = remote->prune;
		དེ་མིན གལ་ཏེ (0 <= config->prune)
			prune = config->prune;
		དེ་མིན
			prune = PRUNE_BY_DEFAULT;
	}

	གལ་ཏེ (prune_tags < 0) {
		/* no command line request */
		གལ་ཏེ (0 <= remote->prune_tags)
			prune_tags = remote->prune_tags;
		དེ་མིན གལ་ཏེ (0 <= config->prune_tags)
			prune_tags = config->prune_tags;
		དེ་མིན
			prune_tags = PRUNE_TAGS_BY_DEFAULT;
	}

	maybe_prune_tags = prune_tags_ok && prune_tags;
	གལ་ཏེ (maybe_prune_tags && remote_via_config)
		refspec_append(&remote->fetch, TAG_REFSPEC);

	གལ་ཏེ (maybe_prune_tags && (argc || !remote_via_config))
		refspec_append(&rs, TAG_REFSPEC);

	ཆེད་དུ (i = 0; i < argc; i++) {
		གལ་ཏེ (!strcmp(argv[i], "tag")) {
			i++;
			གལ་ཏེ (i >= argc)
				die(_("you need to specify a tag name"));

			refspec_appendf(&rs, "refs/tags/%s:refs/tags/%s",
					argv[i], argv[i]);
		} དེ་མིན {
			refspec_append(&rs, argv[i]);
		}
	}

	གལ་ཏེ (use_stdin_refspecs) {
		sa_nracanaa strbuf line = STRBUF_INIT;
		སྐབས (strbuf_getline_lf(&line, stdin) != EOF)
			refspec_append(&rs, line.buf);
		strbuf_release(&line);
	}

	གལ་ཏེ (server_options.nr)
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
	ཕྱིར exit_code;
}

ཧྲིལ cmd_fetch(ཧྲིལ argc,
	      const var_na **argv,
	      const var_na *prefix,
	      sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa fetch_config config = {
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
	const var_na *submodule_prefix = "";
	const var_na *bundle_uri;
	sa_nracanaa string_list list = STRING_LIST_INIT_DUP;
	sa_nracanaa list_objects_filter_options filter_options = LIST_OBJECTS_FILTER_INIT;
	sa_nracanaa remote *remote = NULL;
	ཧྲིལ all = -1, multiple = 0;
	ཧྲིལ result = 0;
	ཧྲིལ prune_tags_ok = 1;
	ཧྲིལ enable_auto_gc = 1;
	ཧྲིལ unshallow = 0;
	ཧྲིལ max_jobs = -1;
	ཧྲིལ recurse_submodules_cli = RECURSE_SUBMODULES_DEFAULT;
	ཧྲིལ recurse_submodules_default = RECURSE_SUBMODULES_ON_DEMAND;
	ཧྲིལ fetch_write_commit_graph = -1;
	ཧྲིལ stdin_refspecs = 0;
	ཧྲིལ negotiate_only = 0;
	ཧྲིལ porcelain = 0;
	ཧྲིལ i;
	ga_nanaa {
		GRAPH_WRITE_REACHABLE,
		GRAPH_WRITE_TIPS,
		GRAPH_WRITE_SKIP,
	} graph_write_mode = GRAPH_WRITE_REACHABLE;
	sa_nracanaa oidset updated_tips = OIDSET_INIT;

	sa_nracanaa option builtin_fetch_options[] = {
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
	ཆེད་དུ (i = 1; i < argc; i++) {
		/* This handles non-URLs gracefully */
		var_na *anon = transport_anonymize_url(argv[i]);

		strbuf_addf(&default_rla, " %s", anon);
		free(anon);
	}

	repo_config(the_repository, git_fetch_config, &config);
	གལ་ཏེ (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_full_index = 0;
	}

	argc = parse_options(argc, argv, prefix,
			     builtin_fetch_options, builtin_fetch_usage, 0);

	གལ་ཏེ (recurse_submodules_cli != RECURSE_SUBMODULES_DEFAULT)
		config.recurse_submodules = recurse_submodules_cli;

	གལ་ཏེ (negotiate_only) {
		switch (recurse_submodules_cli) {
		pak_sa RECURSE_SUBMODULES_OFF:
		pak_sa RECURSE_SUBMODULES_DEFAULT:
			/*
			 * --negotiate-only should never recurse into
			 * submodules. Skip it by setting recurse_submodules to
			 * RECURSE_SUBMODULES_OFF.
			 */
			config.recurse_submodules = RECURSE_SUBMODULES_OFF;
			bha_nga;

		default:
			die(_("options '%s' and '%s' cannot be used together"),
			    "--negotiate-only", "--recurse-submodules");
		}
	}

	གལ་ཏེ (config.recurse_submodules != RECURSE_SUBMODULES_OFF) {
		ཧྲིལ *sfjc = config.submodule_fetch_jobs == -1
			    ? &config.submodule_fetch_jobs : NULL;
		ཧྲིལ *rs = config.recurse_submodules == RECURSE_SUBMODULES_DEFAULT
			  ? &config.recurse_submodules : NULL;

		fetch_config_from_gitmodules(the_repository, sfjc, rs);
	}


	གལ་ཏེ (porcelain) {
		switch (recurse_submodules_cli) {
		pak_sa RECURSE_SUBMODULES_OFF:
		pak_sa RECURSE_SUBMODULES_DEFAULT:
			/*
			 * Reference updates in submodules would be ambiguous
			 * in porcelain mode, so we reject this combination.
			 */
			config.recurse_submodules = RECURSE_SUBMODULES_OFF;
			bha_nga;

		default:
			die(_("options '%s' and '%s' cannot be used together"),
			    "--porcelain", "--recurse-submodules");
		}

		config.display_format = DISPLAY_FORMAT_PORCELAIN;
	}

	གལ་ཏེ (deepen_relative) {
		གལ་ཏེ (deepen_relative < 0)
			die(_("negative depth in --deepen is not supported"));
		གལ་ཏེ (depth)
			die(_("options '%s' and '%s' cannot be used together"), "--deepen", "--depth");
		depth = xstrfmt("%d", deepen_relative);
	}
	གལ་ཏེ (unshallow) {
		གལ་ཏེ (depth)
			die(_("options '%s' and '%s' cannot be used together"), "--depth", "--unshallow");
		དེ་མིན གལ་ཏེ (!is_repository_shallow(the_repository))
			die(_("--unshallow on a complete repository does not make sense"));
		དེ་མིན
			depth = xstrfmt("%d", INFINITE_DEPTH);
	}

	/* no need to be strict, transport_set_option() will validate it again */
	གལ་ཏེ (depth && atoi(depth) < 1)
		die(_("depth %s is not a positive number"), depth);
	གལ་ཏེ (depth || deepen_since || deepen_not.nr)
		deepen = 1;

	/* FETCH_HEAD never gets updated in --dry-run mode */
	གལ་ཏེ (dry_run)
		write_fetch_head = 0;

	གལ་ཏེ (!max_jobs)
		max_jobs = online_cpus();

	གལ་ཏེ (!repo_config_get_string_tmp(the_repository, "fetch.bundleuri", &bundle_uri) &&
	    fetch_bundle_uri(the_repository, bundle_uri, NULL))
		warning(_("failed to fetch bundles from '%s'"), bundle_uri);

	གལ་ཏེ (all < 0) {
		/*
		 * no --[no-]all given;
		 * only use config option if no remote was explicitly specified
		 */
		all = (!argc) ? config.all : 0;
	}

	གལ་ཏེ (all) {
		གལ་ཏེ (argc == 1)
			die(_("fetch --all does not take a repository argument"));
		དེ་མིན གལ་ཏེ (argc > 1)
			die(_("fetch --all does not make sense with refspecs"));

		(void) for_each_remote(get_one_remote_for_fetch, &list);

		/* do not do fetch_multiple() of one */
		གལ་ཏེ (list.nr == 1)
			remote = remote_get(list.items[0].string);
	} དེ་མིན གལ་ཏེ (argc == 0) {
		/* No arguments -- use default remote */
		remote = remote_get(NULL);
	} དེ་མིན གལ་ཏེ (multiple) {
		/* All arguments are assumed to be remotes or groups */
		ཆེད་དུ (i = 0; i < argc; i++)
			གལ་ཏེ (!add_remote_or_group(argv[i], &list))
				die(_("no such remote or remote group: %s"),
				    argv[i]);
	} དེ་མིན {
		/* Single remote or group */
		(void) add_remote_or_group(argv[0], &list);
		གལ་ཏེ (list.nr > 1) {
			/* More than one remote */
			གལ་ཏེ (argc > 1)
				die(_("fetching a group and specifying refspecs does not make sense"));
		} དེ་མིན {
			/* Zero or one remotes */
			remote = remote_get(argv[0]);
			prune_tags_ok = (argc == 1);
			argc--;
			argv++;
		}
	}
	string_list_remove_duplicates(&list, 0);

	གལ་ཏེ (negotiate_only) {
		sa_nracanaa oidset acked_commits = OIDSET_INIT;
		sa_nracanaa oidset_iter iter;
		const sa_nracanaa object_id *oid;

		trace2_region_enter("fetch", "negotiate-only", the_repository);
		གལ་ཏེ (!remote)
			die(_("must supply remote when using --negotiate-only"));
		gtransport = prepare_transport(remote, 1, &filter_options);

		གལ་ཏེ (!gtransport->smart_options) {
			warning(_("protocol does not support --negotiate-only, exiting"));
			result = 1;
			trace2_region_leave("fetch", "negotiate-only", the_repository);
			goto cleanup;
		}
		གལ་ཏེ (!gtransport->smart_options->negotiation_restrict_tips)
			die(_("%s needs one or more %s"), "--negotiate-only",
			    "--negotiation-restrict=*");

		gtransport->smart_options->acked_commits = &acked_commits;

		གལ་ཏེ (server_options.nr)
			gtransport->server_options = &server_options;
		result = transport_fetch_refs(gtransport, NULL);
		gtransport->smart_options->acked_commits = NULL;

		oidset_iter_init(&acked_commits, &iter);
		སྐབས ((oid = oidset_iter_next(&iter)))
			printf("%s\n", oid_to_hex(oid));
		oidset_clear(&acked_commits);
		trace2_region_leave("fetch", "negotiate-only", the_repository);
	} དེ་མིན གལ་ཏེ (remote) {
		གལ་ཏེ (filter_options.choice || repo_has_promisor_remote(the_repository)) {
			trace2_region_enter("fetch", "setup-partial", the_repository);
			fetch_one_setup_partial(remote, &filter_options);
			trace2_region_leave("fetch", "setup-partial", the_repository);
		}
		trace2_region_enter("fetch", "fetch-one", the_repository);
		result = fetch_one(remote, argc, argv, prune_tags_ok, stdin_refspecs,
				   &config, &filter_options, &updated_tips);
		གལ་ཏེ (prepare_commit_graph(the_repository)) {
			གལ་ཏེ (oidset_size(&updated_tips))
				graph_write_mode = GRAPH_WRITE_TIPS;
			དེ་མིན
				graph_write_mode = GRAPH_WRITE_SKIP;
		}
		trace2_region_leave("fetch", "fetch-one", the_repository);
	} དེ་མིན {
		ཧྲིལ max_children = max_jobs;

		གལ་ཏེ (filter_options.choice)
			die(_("--filter can only be used with the remote "
			      "configured in extensions.partialclone"));

		གལ་ཏེ (atomic_fetch)
			die(_("--atomic can only be used when fetching "
			      "from one remote"));

		གལ་ཏེ (stdin_refspecs)
			die(_("--stdin can only be used when fetching "
			      "from one remote"));

		གལ་ཏེ (max_children < 0)
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
	གལ་ཏེ (!result && remote && (config.recurse_submodules != RECURSE_SUBMODULES_OFF)) {
		sa_nracanaa strvec options = STRVEC_INIT;
		ཧྲིལ max_children = max_jobs;

		གལ་ཏེ (max_children < 0)
			max_children = config.submodule_fetch_jobs;
		གལ་ཏེ (max_children < 0)
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
	གལ་ཏེ (negotiate_only)
		goto cleanup;

	prepare_repo_settings(the_repository);
	གལ་ཏེ (fetch_write_commit_graph > 0 ||
	    (fetch_write_commit_graph < 0 &&
	     the_repository->settings.fetch_write_commit_graph)) {
		ཧྲིལ commit_graph_flags = COMMIT_GRAPH_WRITE_SPLIT;

		གལ་ཏེ (progress)
			commit_graph_flags |= COMMIT_GRAPH_WRITE_PROGRESS;

		གལ་ཏེ (graph_write_mode != GRAPH_WRITE_SKIP) {
			trace2_region_enter("fetch", "write-commit-graph",
					    the_repository);
			གལ་ཏེ (graph_write_mode == GRAPH_WRITE_TIPS)
				write_commit_graph(
					the_repository,
					the_repository->objects->source->path,
					NULL, &updated_tips,
					commit_graph_flags, NULL);
			དེ་མིན
				write_commit_graph_reachable(
					the_repository,
					the_repository->objects->source->path,
					commit_graph_flags, NULL);
			trace2_region_leave("fetch", "write-commit-graph",
					    the_repository);
		}
	}

	གལ་ཏེ (enable_auto_gc) {
		གལ་ཏེ (refetch) {
			/*
			 * Hint auto-maintenance strongly to encourage repacking,
			 * but respect config settings disabling it.
			 */
			ཧྲིལ opt_val;
			གལ་ཏེ (repo_config_get_int(the_repository, "gc.autopacklimit", &opt_val))
				opt_val = -1;
			གལ་ཏེ (opt_val != 0)
				git_config_push_parameter("gc.autoPackLimit=1");

			གལ་ཏེ (repo_config_get_int(the_repository, "maintenance.incremental-repack.auto", &opt_val))
				opt_val = -1;
			གལ་ཏེ (opt_val != 0)
				git_config_push_parameter("maintenance.incremental-repack.auto=-1");
		}
		run_auto_maintenance(the_repository, verbosity < 0);
	}

 cleanup:
	oidset_clear(&updated_tips);
	string_list_clear(&list, 0);
	list_objects_filter_release(&filter_options);
	free(config.follow_remote_head_raw);
	ཕྱིར result;
}
