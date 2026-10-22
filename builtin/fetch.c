/*
 * "git fetch"
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "advice.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "environment.h"
#அந்தர்பாவ "hex.h"
#அந்தர்பாவ "refs.h"
#அந்தர்பாவ "refspec.h"
#அந்தர்பாவ "object-name.h"
#அந்தர்பாவ "odb.h"
#அந்தர்பாவ "odb/source.h"
#அந்தர்பாவ "oidset.h"
#அந்தர்பாவ "oid-array.h"
#அந்தர்பாவ "commit.h"
#அந்தர்பாவ "string-list.h"
#அந்தர்பாவ "remote.h"
#அந்தர்பாவ "transport.h"
#அந்தர்பாவ "run-command.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "sigchain.h"
#அந்தர்பாவ "submodule-config.h"
#அந்தர்பாவ "submodule.h"
#அந்தர்பாவ "connected.h"
#அந்தர்பாவ "strvec.h"
#அந்தர்பாவ "utf8.h"
#அந்தர்பாவ "pager.h"
#அந்தர்பாவ "path.h"
#அந்தர்பாவ "pkt-line.h"
#அந்தர்பாவ "list-objects-filter-options.h"
#அந்தர்பாவ "commit-reach.h"
#அந்தர்பாவ "branch.h"
#அந்தர்பாவ "promisor-remote.h"
#அந்தர்பாவ "commit-graph.h"
#அந்தர்பாவ "shallow.h"
#அந்தர்பாவ "trace.h"
#அந்தர்பாவ "trace2.h"
#அந்தர்பாவ "bundle-uri.h"

#define FORCED_UPDATES_DELAY_WARNING_IN_MS (10 * 1000)

நிலையான மாறிலி எழுத்து * மாறிலி builtin_fetch_usage[] = {
	N_("git fetch [<options>] [<repository> [<refspec>...]]"),
	N_("git fetch [<options>] <group>"),
	N_("git fetch --multiple [<options>] [(<repository>|<group>)...]"),
	N_("git fetch --all [<options>]"),
	வெற்று
};

எண்ணிக்கை {
	TAGS_UNSET = 0,
	TAGS_DEFAULT = 1,
	TAGS_SET = 2
};

எண்ணிக்கை display_format {
	DISPLAY_FORMAT_FULL,
	DISPLAY_FORMAT_COMPACT,
	DISPLAY_FORMAT_PORCELAIN,
};

கட்டமைப்பு display_state {
	கட்டமைப்பு strbuf buf;

	முழு refcol_width;
	எண்ணிக்கை display_format format;

	எழுத்து *url;
	முழு url_len, shown_url;
};

நிலையான uint64_t forced_updates_ms = 0;
நிலையான முழு prefetch = 0;
நிலையான முழு prune = -1; /* unspecified */
#define PRUNE_BY_DEFAULT 0 /* do we prune by default? */

நிலையான முழு prune_tags = -1; /* unspecified */
#define PRUNE_TAGS_BY_DEFAULT 0 /* do we prune tags by default? */

நிலையான முழு append, dry_run, force, keep, update_head_ok;
நிலையான முழு write_fetch_head = 1;
நிலையான முழு verbosity, deepen_relative, set_upstream, refetch;
நிலையான முழு progress = -1;
நிலையான முழு tags = TAGS_DEFAULT, update_shallow, deepen;
நிலையான முழு atomic_fetch;
நிலையான எண்ணிக்கை transport_family family;
நிலையான மாறிலி எழுத்து *depth;
நிலையான மாறிலி எழுத்து *deepen_since;
நிலையான மாறிலி எழுத்து *upload_pack;
நிலையான கட்டமைப்பு string_list deepen_not = STRING_LIST_INIT_NODUP;
நிலையான கட்டமைப்பு strbuf default_rla = STRBUF_INIT;
நிலையான கட்டமைப்பு transport *gtransport;
நிலையான கட்டமைப்பு transport *gsecondary;
நிலையான கட்டமைப்பு refspec refmap;
நிலையான கட்டமைப்பு string_list server_options = STRING_LIST_INIT_DUP;
நிலையான கட்டமைப்பு string_list negotiation_restrict = STRING_LIST_INIT_NODUP;
நிலையான கட்டமைப்பு string_list negotiation_include = STRING_LIST_INIT_NODUP;

கட்டமைப்பு fetch_config {
	எண்ணிக்கை display_format display_format;
	எழுத்து *follow_remote_head_raw;
	முழு follow_remote_head_seen;
	முழு all;
	முழு prune;
	முழு prune_tags;
	முழு show_forced_updates;
	முழு recurse_submodules;
	முழு parallel;
	முழு submodule_fetch_jobs;
};

நிலையான முழு git_fetch_config(மாறிலி எழுத்து *k, மாறிலி எழுத்து *v,
			    மாறிலி கட்டமைப்பு config_context *ctx, வெறுமை *cb)
{
	கட்டமைப்பு fetch_config *fetch_config = cb;

	எனில் (!strcmp(k, "fetch.all")) {
		fetch_config->all = git_config_bool(k, v);
		திரும்பு 0;
	}

	எனில் (!strcmp(k, "fetch.prune")) {
		fetch_config->prune = git_config_bool(k, v);
		திரும்பு 0;
	}

	எனில் (!strcmp(k, "fetch.prunetags")) {
		fetch_config->prune_tags = git_config_bool(k, v);
		திரும்பு 0;
	}

	எனில் (!strcmp(k, "fetch.showforcedupdates")) {
		fetch_config->show_forced_updates = git_config_bool(k, v);
		திரும்பு 0;
	}

	எனில் (!strcmp(k, "submodule.recurse")) {
		முழு r = git_config_bool(k, v) ?
			RECURSE_SUBMODULES_ON : RECURSE_SUBMODULES_OFF;
		fetch_config->recurse_submodules = r;
		திரும்பு 0;
	}

	எனில் (!strcmp(k, "submodule.fetchjobs")) {
		fetch_config->submodule_fetch_jobs = parse_submodule_fetchjobs(k, v, ctx->kvi);
		திரும்பு 0;
	} இல்லையெனில் எனில் (!strcmp(k, "fetch.recursesubmodules")) {
		fetch_config->recurse_submodules = parse_fetch_recurse_submodules_arg(k, v);
		திரும்பு 0;
	}

	எனில் (!strcmp(k, "fetch.parallel")) {
		fetch_config->parallel = git_config_int(k, v, ctx->kvi);
		எனில் (fetch_config->parallel < 0)
			die(_("fetch.parallel cannot be negative"));
		எனில் (!fetch_config->parallel)
			fetch_config->parallel = online_cpus();
		திரும்பு 0;
	}

	எனில் (!strcmp(k, "fetch.output")) {
		எனில் (!v)
			திரும்பு config_error_nonbool(k);
		இல்லையெனில் எனில் (!strcasecmp(v, "full"))
			fetch_config->display_format = DISPLAY_FORMAT_FULL;
		இல்லையெனில் எனில் (!strcasecmp(v, "compact"))
			fetch_config->display_format = DISPLAY_FORMAT_COMPACT;
		இல்லையெனில்
			die(_("invalid value for '%s': '%s'"),
			    "fetch.output", v);
		திரும்பு 0;
	}

	எனில் (!strcmp(k, "fetch.followremotehead")) {
		விடுவி(fetch_config->follow_remote_head_raw);
		fetch_config->follow_remote_head_raw = xstrdup_or_null(v);
		fetch_config->follow_remote_head_seen = 1;
		திரும்பு 0;
	}

	திரும்பு git_default_config(k, v, ctx, cb);
}

நிலையான எண்ணிக்கை follow_remote_head_settings get_follow_remote_head(மாறிலி எழுத்து *setting)
{
	எனில் (!setting)
		die(_("missing value for 'fetch.followRemoteHEAD'"));
	இல்லையெனில் எனில் (!strcmp(setting, "never"))
		திரும்பு FOLLOW_REMOTE_NEVER;
	இல்லையெனில் எனில் (!strcmp(setting, "create"))
		திரும்பு FOLLOW_REMOTE_CREATE;
	இல்லையெனில் எனில் (!strcmp(setting, "warn"))
		திரும்பு FOLLOW_REMOTE_WARN;
	இல்லையெனில் எனில் (!strcmp(setting, "always"))
		திரும்பு FOLLOW_REMOTE_ALWAYS;
	warning(_("unrecognized fetch.followRemoteHEAD value '%s' ignored"), setting);
	திரும்பு BUILTIN_FOLLOW_REMOTE_HEAD_DFLT;
}

நிலையான முழு parse_refmap_arg(மாறிலி கட்டமைப்பு option *opt, மாறிலி எழுத்து *arg, முழு unset)
{
	BUG_ON_OPT_NEG(unset);

	/*
	 * "git fetch --refmap='' origin foo"
	 * can be used to tell the command not to store anywhere
	 */
	refspec_append(opt->மதிப்பு, arg);

	திரும்பு 0;
}

நிலையான வெறுமை unlock_pack(unsigned முழு flags)
{
	எனில் (gtransport)
		transport_unlock_pack(gtransport, flags);
	எனில் (gsecondary)
		transport_unlock_pack(gsecondary, flags);
}

நிலையான வெறுமை unlock_pack_atexit(வெறுமை)
{
	unlock_pack(0);
}

நிலையான வெறுமை unlock_pack_on_signal(முழு signo)
{
	unlock_pack(TRANSPORT_UNLOCK_PACK_IN_SIGNAL_HANDLER);
	sigchain_எடு(signo);
	எழுப்பு(signo);
}

நிலையான வெறுமை add_merge_config(கட்டமைப்பு ref **head,
			   மாறிலி கட்டமைப்பு ref *remote_refs,
		           கட்டமைப்பு branch *branch,
		           கட்டமைப்பு ref ***tail)
{
	முழு i;

	க்கு (i = 0; i < branch->merge_nr; i++) {
		கட்டமைப்பு ref *rm, **old_tail = *tail;
		கட்டமைப்பு refspec_item refspec;

		க்கு (rm = *head; rm; rm = rm->next) {
			எனில் (branch_merge_matches(branch, i, rm->name)) {
				rm->fetch_head_status = FETCH_HEAD_MERGE;
				நிறுத்து;
			}
		}
		எனில் (rm)
			தொடர்;

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
		memset(&refspec, 0, அளவு(refspec));
		refspec.src = branch->merge[i]->src;
		get_fetch_map(remote_refs, &refspec, tail, 1);
		க்கு (rm = *old_tail; rm; rm = rm->next)
			rm->fetch_head_status = FETCH_HEAD_MERGE;
	}
}

நிலையான வெறுமை create_fetch_oidset(கட்டமைப்பு ref **head, கட்டமைப்பு oidset *out)
{
	கட்டமைப்பு ref *rm = *head;
	போது (rm) {
		oidset_செருகு(out, &rm->old_oid);
		rm = rm->next;
	}
}

கட்டமைப்பு refname_hash_entry {
	கட்டமைப்பு hashmap_entry ent;
	கட்டமைப்பு object_id oid;
	முழு ignore;
	எழுத்து refname[FLEX_ARRAY];
};

நிலையான முழு refname_hash_entry_cmp(மாறிலி வெறுமை *hashmap_cmp_fn_data UNUSED,
				  மாறிலி கட்டமைப்பு hashmap_entry *eptr,
				  மாறிலி கட்டமைப்பு hashmap_entry *entry_or_சாவி,
				  மாறிலி வெறுமை *keydata)
{
	மாறிலி கட்டமைப்பு refname_hash_entry *e1, *e2;

	e1 = container_of(eptr, மாறிலி கட்டமைப்பு refname_hash_entry, ent);
	e2 = container_of(entry_or_சாவி, மாறிலி கட்டமைப்பு refname_hash_entry, ent);
	திரும்பு strcmp(e1->refname, keydata ? keydata : e2->refname);
}

நிலையான கட்டமைப்பு refname_hash_entry *refname_hash_add(கட்டமைப்பு hashmap *map,
						   மாறிலி எழுத்து *refname,
						   மாறிலி கட்டமைப்பு object_id *oid)
{
	கட்டமைப்பு refname_hash_entry *ent;
	size_t len = strlen(refname);

	FLEX_ALLOC_MEM(ent, refname, refname, len);
	hashmap_entry_init(&ent->ent, strhash(refname));
	oidcpy(&ent->oid, oid);
	hashmap_add(map, &ent->ent);
	திரும்பு ent;
}

நிலையான முழு add_one_refname(மாறிலி கட்டமைப்பு reference *ref, வெறுமை *cbdata)
{
	கட்டமைப்பு hashmap *refname_map = cbdata;

	(வெறுமை) refname_hash_add(refname_map, ref->name, ref->oid);
	திரும்பு 0;
}

நிலையான வெறுமை refname_hash_init(கட்டமைப்பு hashmap *map)
{
	hashmap_init(map, refname_hash_entry_cmp, வெற்று, 0);
}

நிலையான முழு refname_hash_exists(கட்டமைப்பு hashmap *map, மாறிலி எழுத்து *refname)
{
	திரும்பு !!hashmap_get_from_hash(map, strhash(refname), refname);
}

நிலையான வெறுமை clear_item(கட்டமைப்பு refname_hash_entry *item)
{
	item->ignore = 1;
}


நிலையான வெறுமை add_already_queued_tags(மாறிலி எழுத்து *refname,
				    மாறிலி கட்டமைப்பு object_id *old_oid UNUSED,
				    மாறிலி கட்டமைப்பு object_id *new_oid,
				    வெறுமை *cb_data)
{
	கட்டமைப்பு hashmap *queued_tags = cb_data;
	எனில் (starts_with(refname, "refs/tags/") && new_oid)
		(வெறுமை) refname_hash_add(queued_tags, refname, new_oid);
}

நிலையான வெறுமை find_non_local_tags(மாறிலி கட்டமைப்பு ref *refs,
				கட்டமைப்பு ref_transaction *transaction,
				கட்டமைப்பு ref **head,
				கட்டமைப்பு ref ***tail)
{
	கட்டமைப்பு hashmap existing_refs;
	கட்டமைப்பு hashmap remote_refs;
	கட்டமைப்பு oidset fetch_oids = OIDSET_INIT;
	கட்டமைப்பு string_list remote_refs_list = STRING_LIST_INIT_NODUP;
	கட்டமைப்பு string_list_item *remote_ref_item;
	மாறிலி கட்டமைப்பு ref *ref;
	கட்டமைப்பு refname_hash_entry *item = வெற்று;

	refname_hash_init(&existing_refs);
	refname_hash_init(&remote_refs);
	create_fetch_oidset(head, &fetch_oids);

	refs_for_each_ref(get_main_ref_store(the_repository), add_one_refname,
			  &existing_refs);

	/*
	 * If we already have a transaction, then we need to filter out all
	 * tags which have already been queued up.
	 */
	எனில் (transaction)
		ref_transaction_for_each_queued_update(transaction,
						       add_already_queued_tags,
						       &existing_refs);

	க்கு (ref = refs; ref; ref = ref->next) {
		எனில் (!starts_with(ref->name, "refs/tags/"))
			தொடர்;

		/*
		 * The peeled ref always follows the matching base
		 * ref, so if we see a peeled ref that we don't want
		 * to fetch then we can mark the ref entry in the list
		 * as one to ignore by setting util to NULL.
		 */
		எனில் (ends_with(ref->name, "^{}")) {
			எனில் (item &&
			    !odb_has_object(the_repository->objects, &ref->old_oid, 0) &&
			    !oidset_contains(&fetch_oids, &ref->old_oid) &&
			    !odb_has_object(the_repository->objects, &item->oid, 0) &&
			    !oidset_contains(&fetch_oids, &item->oid))
				clear_item(item);
			item = வெற்று;
			தொடர்;
		}

		/*
		 * If item is non-NULL here, then we previously saw a
		 * ref not followed by a peeled reference, so we need
		 * to check if it is a lightweight tag that we want to
		 * fetch.
		 */
		எனில் (item &&
		    !odb_has_object(the_repository->objects, &item->oid, 0) &&
		    !oidset_contains(&fetch_oids, &item->oid))
			clear_item(item);

		item = வெற்று;

		/* skip duplicates and refs that we already have */
		எனில் (refname_hash_exists(&remote_refs, ref->name) ||
		    refname_hash_exists(&existing_refs, ref->name))
			தொடர்;

		item = refname_hash_add(&remote_refs, ref->name, &ref->old_oid);
		string_list_செருகு(&remote_refs_list, ref->name);
	}
	hashmap_clear_and_free(&existing_refs, கட்டமைப்பு refname_hash_entry, ent);

	/*
	 * We may have a final lightweight tag that needs to be
	 * checked to see if it needs fetching.
	 */
	எனில் (item &&
	    !odb_has_object(the_repository->objects, &item->oid, 0) &&
	    !oidset_contains(&fetch_oids, &item->oid))
		clear_item(item);

	/*
	 * For all the tags in the remote_refs_list,
	 * add them to the list of refs to be fetched
	 */
	for_each_string_list_item(remote_ref_item, &remote_refs_list) {
		மாறிலி எழுத்து *refname = remote_ref_item->string;
		கட்டமைப்பு ref *rm;
		unsigned முழு hash = strhash(refname);

		item = hashmap_get_entry_from_hash(&remote_refs, hash, refname,
					கட்டமைப்பு refname_hash_entry, ent);
		எனில் (!item)
			BUG("unseen remote ref?");

		/* Unless we have already decided to ignore this item... */
		எனில் (item->ignore)
			தொடர்;

		rm = alloc_ref(item->refname);
		rm->peer_ref = alloc_ref(item->refname);
		oidcpy(&rm->old_oid, &item->oid);
		**tail = rm;
		*tail = &rm->next;
	}
	hashmap_clear_and_free(&remote_refs, கட்டமைப்பு refname_hash_entry, ent);
	string_list_clear(&remote_refs_list, 0);
	oidset_clear(&fetch_oids);
}

நிலையான வெறுமை filter_prefetch_refspec(கட்டமைப்பு refspec *rs)
{
	முழு i;

	எனில் (!prefetch)
		திரும்பு;

	க்கு (i = 0; i < rs->nr; i++) {
		கட்டமைப்பு strbuf new_dst = STRBUF_INIT;
		எழுத்து *old_dst;
		மாறிலி எழுத்து *sub = வெற்று;

		எனில் (rs->பொருட்கள்[i].negative)
			தொடர்;
		எனில் (!rs->பொருட்கள்[i].dst ||
		    (rs->பொருட்கள்[i].src &&
		     starts_with(rs->பொருட்கள்[i].src,
				 ref_namespace[NAMESPACE_TAGS].ref))) {
			முழு j;

			refspec_item_clear(&rs->பொருட்கள்[i]);

			க்கு (j = i + 1; j < rs->nr; j++)
				rs->பொருட்கள்[j - 1] = rs->பொருட்கள்[j];
			rs->nr--;
			i--;
			தொடர்;
		}

		old_dst = rs->பொருட்கள்[i].dst;
		strbuf_addstr(&new_dst, ref_namespace[NAMESPACE_PREFETCH].ref);

		/*
		 * If old_dst starts with "refs/", then place
		 * sub after that prefix. Otherwise, start at
		 * the beginning of the string.
		 */
		எனில் (!skip_prefix(old_dst, "refs/", &sub))
			sub = old_dst;
		strbuf_addstr(&new_dst, sub);

		rs->பொருட்கள்[i].dst = strbuf_detach(&new_dst, வெற்று);
		rs->பொருட்கள்[i].force = 1;

		விடுவி(old_dst);
	}
}

நிலையான கட்டமைப்பு ref *get_ref_map(கட்டமைப்பு remote *remote,
			       மாறிலி கட்டமைப்பு ref *remote_refs,
			       கட்டமைப்பு refspec *rs,
			       முழு tags, முழு *autotags)
{
	முழு i;
	கட்டமைப்பு ref *rm;
	கட்டமைப்பு ref *ref_map = வெற்று;
	கட்டமைப்பு ref **tail = &ref_map;

	/* opportunistically-updated references: */
	கட்டமைப்பு ref *orefs = வெற்று, **oref_tail = &orefs;

	கட்டமைப்பு hashmap existing_refs;
	முழு existing_refs_populated = 0;

	filter_prefetch_refspec(rs);
	எனில் (remote)
		filter_prefetch_refspec(&remote->fetch);

	எனில் (rs->nr) {
		கட்டமைப்பு refspec *fetch_refspec;

		க்கு (i = 0; i < rs->nr; i++) {
			get_fetch_map(remote_refs, &rs->பொருட்கள்[i], &tail, 0);
			எனில் (rs->பொருட்கள்[i].dst && rs->பொருட்கள்[i].dst[0])
				*autotags = 1;
		}
		/* Merge everything on the command line (but not --tags) */
		க்கு (rm = ref_map; rm; rm = rm->next)
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
		எனில் (refmap.nr)
			fetch_refspec = &refmap;
		இல்லையெனில்
			fetch_refspec = &remote->fetch;

		க்கு (i = 0; i < fetch_refspec->nr; i++)
			get_fetch_map(ref_map, &fetch_refspec->பொருட்கள்[i], &oref_tail, 1);
	} இல்லையெனில் எனில் (refmap.nr) {
		die("--refmap option is only meaningful with command-line refspec(s)");
	} இல்லையெனில் {
		/* Use the defaults */
		கட்டமைப்பு branch *branch = branch_get(வெற்று);
		முழு has_merge = branch_has_merge_config(branch);
		எனில் (remote &&
		    (remote->fetch.nr ||
		     /* Note: has_merge implies non-NULL branch->remote_name */
		     (has_merge && !strcmp(branch->remote_name, remote->name)))) {
			க்கு (i = 0; i < remote->fetch.nr; i++) {
				get_fetch_map(remote_refs, &remote->fetch.பொருட்கள்[i], &tail, 0);
				எனில் (remote->fetch.பொருட்கள்[i].dst &&
				    remote->fetch.பொருட்கள்[i].dst[0])
					*autotags = 1;
				எனில் (!i && !has_merge && ref_map &&
				    !remote->fetch.பொருட்கள்[0].pattern)
					ref_map->fetch_head_status = FETCH_HEAD_MERGE;
			}
			/*
			 * if the remote we're fetching from is the same
			 * as given in branch.<name>.remote, we add the
			 * ref given in branch.<name>.merge, too.
			 *
			 * Note: has_merge implies non-NULL branch->remote_name
			 */
			எனில் (has_merge &&
			    !strcmp(branch->remote_name, remote->name))
				add_merge_config(&ref_map, remote_refs, branch, &tail);
		} இல்லையெனில் எனில் (!prefetch) {
			ref_map = get_remote_ref(remote_refs, "HEAD");
			எனில் (!ref_map)
				die(_("couldn't find remote ref HEAD"));
			ref_map->fetch_head_status = FETCH_HEAD_MERGE;
			tail = &ref_map->next;
		}
	}

	எனில் (tags == TAGS_SET) {
		கட்டமைப்பு refspec_item tag_refspec;

		/* also fetch all tags */
		refspec_item_init_தள்ளு(&tag_refspec, TAG_REFSPEC,
				       the_hash_algo);
		get_fetch_map(remote_refs, &tag_refspec, &tail, 0);
		refspec_item_clear(&tag_refspec);
	} இல்லையெனில் எனில் (tags == TAGS_DEFAULT && *autotags) {
		find_non_local_tags(remote_refs, வெற்று, &ref_map, &tail);
	}

	/* Now append any refs to be updated opportunistically: */
	*tail = orefs;
	க்கு (rm = orefs; rm; rm = rm->next) {
		rm->fetch_head_status = FETCH_HEAD_IGNORE;
		tail = &rm->next;
	}

	/*
	 * apply negative refspecs first, before we remove duplicates. This is
	 * necessary as negative refspecs might remove an otherwise conflicting
	 * duplicate.
	 */
	எனில் (rs->nr)
		ref_map = apply_negative_refspecs(ref_map, rs);
	இல்லையெனில்
		ref_map = apply_negative_refspecs(ref_map, &remote->fetch);

	ref_map = ref_remove_duplicates(ref_map);

	க்கு (rm = ref_map; rm; rm = rm->next) {
		எனில் (rm->peer_ref) {
			மாறிலி எழுத்து *refname = rm->peer_ref->name;
			கட்டமைப்பு refname_hash_entry *peer_item;
			unsigned முழு hash = strhash(refname);

			எனில் (!existing_refs_populated) {
				refname_hash_init(&existing_refs);
				refs_for_each_ref(get_main_ref_store(the_repository),
						  add_one_refname,
						  &existing_refs);
				existing_refs_populated = 1;
			}

			peer_item = hashmap_get_entry_from_hash(&existing_refs,
						hash, refname,
						கட்டமைப்பு refname_hash_entry, ent);
			எனில் (peer_item) {
				கட்டமைப்பு object_id *old_oid = &peer_item->oid;
				oidcpy(&rm->peer_ref->old_oid, old_oid);
			}
		}
	}
	எனில் (existing_refs_populated)
		hashmap_clear_and_free(&existing_refs, கட்டமைப்பு refname_hash_entry, ent);

	திரும்பு ref_map;
}

நிலையான முழு s_update_ref(மாறிலி எழுத்து *action,
			கட்டமைப்பு ref *ref,
			கட்டமைப்பு ref_transaction *transaction,
			முழு check_old)
{
	எழுத்து *செய்தி;
	எழுத்து *rla = getenv("GIT_REFLOG_ACTION");
	கட்டமைப்பு strbuf err = STRBUF_INIT;
	முழு ret;

	எனில் (dry_run)
		திரும்பு 0;
	எனில் (!rla)
		rla = default_rla.buf;
	செய்தி = xstrfmt("%s: %s", rla, action);

	ret = ref_transaction_update(transaction, ref->name, &ref->new_oid,
				     check_old ? &ref->old_oid : வெற்று,
				     வெற்று, வெற்று, 0, செய்தி, &err);

	எனில் (ret)
		error("%s", err.buf);
	strbuf_release(&err);
	விடுவி(செய்தி);
	திரும்பு ret;
}

நிலையான முழு refcol_width(மாறிலி கட்டமைப்பு ref *ref_map, முழு compact_format)
{
	மாறிலி கட்டமைப்பு ref *ref;
	முழு max, width = 10;

	max = term_columns();
	எனில் (compact_format)
		max = max * 2 / 3;

	க்கு (ref = ref_map; ref; ref = ref->next) {
		முழு rlen, llen = 0, len;

		எனில் (ref->status == REF_STATUS_REJECT_SHALLOW ||
		    !ref->peer_ref ||
		    !strcmp(ref->name, "HEAD"))
			தொடர்;

		/* uptodate lines are only shown on high verbosity level */
		எனில் (verbosity <= 0 && oideq(&ref->peer_ref->old_oid, &ref->old_oid))
			தொடர்;

		rlen = utf8_strwidth(prettify_refname(ref->name));
		எனில் (!compact_format)
			llen = utf8_strwidth(prettify_refname(ref->peer_ref->name));

		/*
		 * rough estimation to see if the output line is too long and
		 * should not be counted (we can't do precise calculation
		 * anyway because we don't know if the error explanation part
		 * will be printed in update_local_ref)
		 */
		len = 21 /* flag and summary */ + rlen + 4 /* -> */ + llen;
		எனில் (len >= max)
			தொடர்;

		எனில் (width < rlen)
			width = rlen;
	}

	திரும்பு width;
}

நிலையான வெறுமை display_state_init(கட்டமைப்பு display_state *display_state, கட்டமைப்பு ref *ref_map,
			       மாறிலி எழுத்து *raw_url, எண்ணிக்கை display_format format)
{
	முழு i;

	memset(display_state, 0, அளவு(*display_state));
	strbuf_init(&display_state->buf, 0);
	display_state->format = format;

	எனில் (raw_url)
		display_state->url = transport_anonymize_url(raw_url);
	இல்லையெனில்
		display_state->url = xstrdup("foreign");

	display_state->url_len = strlen(display_state->url);
	க்கு (i = display_state->url_len - 1; 0 <= i && display_state->url[i] == '/'; i--)
		;
	display_state->url_len = i + 1;
	எனில் (4 < i && !strncmp(".git", display_state->url + i - 3, 4))
		display_state->url_len = i - 3;

	எனில் (verbosity < 0)
		திரும்பு;

	தேர்வு (display_state->format) {
	நிலை DISPLAY_FORMAT_FULL:
	நிலை DISPLAY_FORMAT_COMPACT:
		display_state->refcol_width = refcol_width(ref_map,
							   display_state->format == DISPLAY_FORMAT_COMPACT);
		நிறுத்து;
	நிலை DISPLAY_FORMAT_PORCELAIN:
		/* We don't need to precompute anything here. */
		நிறுத்து;
	இயல்பு:
		BUG("unexpected display format %d", display_state->format);
	}
}

நிலையான வெறுமை display_state_release(கட்டமைப்பு display_state *display_state)
{
	strbuf_release(&display_state->buf);
	விடுவி(display_state->url);
}

நிலையான வெறுமை print_remote_to_local(கட்டமைப்பு display_state *display_state,
				  மாறிலி எழுத்து *remote, மாறிலி எழுத்து *local)
{
	strbuf_addf(&display_state->buf, "%-*s -> %s",
		    display_state->refcol_width, remote, local);
}

நிலையான முழு find_and_replace(கட்டமைப்பு strbuf *haystack,
			    மாறிலி எழுத்து *needle,
			    மாறிலி எழுத்து *placeholder)
{
	மாறிலி எழுத்து *p = வெற்று;
	முழு plen, nlen;

	nlen = strlen(needle);
	எனில் (ends_with(haystack->buf, needle))
		p = haystack->buf + haystack->len - nlen;
	இல்லையெனில்
		p = strstr(haystack->buf, needle);
	எனில் (!p)
		திரும்பு 0;

	எனில் (p > haystack->buf && p[-1] != '/')
		திரும்பு 0;

	plen = strlen(p);
	எனில் (plen > nlen && p[nlen] != '/')
		திரும்பு 0;

	strbuf_splice(haystack, p - haystack->buf, nlen,
		      placeholder, strlen(placeholder));
	திரும்பு 1;
}

நிலையான வெறுமை print_compact(கட்டமைப்பு display_state *display_state,
			  மாறிலி எழுத்து *remote, மாறிலி எழுத்து *local)
{
	கட்டமைப்பு strbuf r = STRBUF_INIT;
	கட்டமைப்பு strbuf l = STRBUF_INIT;

	எனில் (!strcmp(remote, local)) {
		strbuf_addf(&display_state->buf, "%-*s -> *", display_state->refcol_width, remote);
		திரும்பு;
	}

	strbuf_addstr(&r, remote);
	strbuf_addstr(&l, local);

	எனில் (!find_and_replace(&r, local, "*"))
		find_and_replace(&l, remote, "*");
	print_remote_to_local(display_state, r.buf, l.buf);

	strbuf_release(&r);
	strbuf_release(&l);
}

நிலையான வெறுமை display_ref_update(கட்டமைப்பு display_state *display_state, எழுத்து code,
			       மாறிலி எழுத்து *summary, மாறிலி எழுத்து *error,
			       மாறிலி எழுத்து *remote, மாறிலி எழுத்து *local,
			       மாறிலி கட்டமைப்பு object_id *old_oid,
			       மாறிலி கட்டமைப்பு object_id *new_oid,
			       முழு summary_width)
{
	FILE *f = stderr;

	எனில் (verbosity < 0)
		திரும்பு;

	strbuf_reset(&display_state->buf);

	தேர்வு (display_state->format) {
	நிலை DISPLAY_FORMAT_FULL:
	நிலை DISPLAY_FORMAT_COMPACT: {
		முழு width;

		எனில் (!display_state->shown_url) {
			strbuf_addf(&display_state->buf, _("From %.*s\n"),
				    display_state->url_len, display_state->url);
			display_state->shown_url = 1;
		}

		width = (summary_width + strlen(summary) - gettext_width(summary));
		remote = prettify_refname(remote);
		local = prettify_refname(local);

		strbuf_addf(&display_state->buf, " %c %-*s ", code, width, summary);

		எனில் (display_state->format != DISPLAY_FORMAT_COMPACT)
			print_remote_to_local(display_state, remote, local);
		இல்லையெனில்
			print_compact(display_state, remote, local);

		எனில் (error)
			strbuf_addf(&display_state->buf, "  (%s)", error);

		நிறுத்து;
	}
	நிலை DISPLAY_FORMAT_PORCELAIN:
		strbuf_addf(&display_state->buf, "%c %s %s %s", code,
			    oid_to_hex(old_oid), oid_to_hex(new_oid), local);
		f = stdout;
		நிறுத்து;
	இயல்பு:
		BUG("unexpected display format %d", display_state->format);
	};
	strbuf_addch(&display_state->buf, '\n');

	fputs(display_state->buf.buf, f);
}

கட்டமைப்பு ref_update_display_info {
	bool failed;
	எழுத்து success_code;
	எழுத்து fail_code;
	எழுத்து *summary;
	எழுத்து *fail_detail;
	எழுத்து *success_detail;
	எழுத்து *ref;
	எழுத்து *remote;
	கட்டமைப்பு object_id old_oid;
	கட்டமைப்பு object_id new_oid;
};

கட்டமைப்பு ref_update_display_info_array {
	கட்டமைப்பு ref_update_display_info *info;
	size_t alloc, nr;
};

நிலையான கட்டமைப்பு ref_update_display_info *ref_update_display_info_append(
					   கட்டமைப்பு ref_update_display_info_array *array,
					   எழுத்து success_code,
					   எழுத்து fail_code,
					   மாறிலி எழுத்து *summary,
					   மாறிலி எழுத்து *success_detail,
					   மாறிலி எழுத்து *fail_detail,
					   மாறிலி எழுத்து *ref,
					   மாறிலி எழுத்து *remote,
					   மாறிலி கட்டமைப்பு object_id *old_oid,
					   மாறிலி கட்டமைப்பு object_id *new_oid)
{
	கட்டமைப்பு ref_update_display_info *info;

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

	திரும்பு info;
}

நிலையான வெறுமை ref_update_display_info_set_failed(கட்டமைப்பு ref_update_display_info *info)
{
	info->failed = true;
}

நிலையான வெறுமை ref_update_display_info_free(கட்டமைப்பு ref_update_display_info *info)
{
	விடுவி(info->summary);
	விடுவி(info->success_detail);
	விடுவி(info->fail_detail);
	விடுவி(info->remote);
	விடுவி(info->ref);
}

நிலையான வெறுமை ref_update_display_info_display(கட்டமைப்பு ref_update_display_info *info,
					    கட்டமைப்பு display_state *display_state,
					    முழு summary_width)
{
	display_ref_update(display_state,
			   info->failed ? info->fail_code : info->success_code,
			   info->summary,
			   info->failed ? info->fail_detail : info->success_detail,
			   info->remote, info->ref, &info->old_oid,
			   &info->new_oid, summary_width);
}

நிலையான முழு update_local_ref(கட்டமைப்பு ref *ref,
			    கட்டமைப்பு ref_transaction *transaction,
			    மாறிலி கட்டமைப்பு ref *remote_ref,
			    மாறிலி கட்டமைப்பு fetch_config *config,
			    கட்டமைப்பு ref_update_display_info_array *display_array)
{
	கட்டமைப்பு commit *current = வெற்று, *updated;
	முழு fast_forward = 0;

	எனில் (!odb_has_object(the_repository->objects, &ref->new_oid,
			    ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR))
		die(_("object %s not found"), oid_to_hex(&ref->new_oid));

	எனில் (oideq(&ref->old_oid, &ref->new_oid)) {
		எனில் (verbosity > 0)
			ref_update_display_info_append(display_array, '=', '=',
						       _("[up to date]"), வெற்று,
						       வெற்று, ref->name,
						       remote_ref->name, &ref->old_oid,
						       &ref->new_oid);
		திரும்பு 0;
	}

	எனில் (!update_head_ok &&
	    !is_null_oid(&ref->old_oid) &&
	    branch_checked_out(ref->name)) {
		கட்டமைப்பு ref_update_display_info *info;
		/*
		 * If this is the head, and it's not okay to update
		 * the head, and the old value of the head isn't empty...
		 */
		info = ref_update_display_info_append(display_array, '!', '!',
						      _("[rejected]"), வெற்று,
						      _("can't fetch into checked-out branch"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);
		ref_update_display_info_set_failed(info);
		திரும்பு 1;
	}

	எனில் (!is_null_oid(&ref->old_oid) &&
	    starts_with(ref->name, "refs/tags/")) {
		கட்டமைப்பு ref_update_display_info *info;

		எனில் (force || ref->force) {
			முழு r;

			r = s_update_ref("updating tag", ref, transaction, 0);

			info = ref_update_display_info_append(display_array, 't', '!',
							      _("[tag update]"), வெற்று,
							      _("unable to update local ref"),
							      ref->name, remote_ref->name,
							      &ref->old_oid, &ref->new_oid);
			எனில் (r)
				ref_update_display_info_set_failed(info);

			திரும்பு r;
		} இல்லையெனில் {
			info = ref_update_display_info_append(display_array, '!', '!',
							      _("[rejected]"), வெற்று,
							      _("would clobber existing tag"),
							      ref->name, remote_ref->name,
							      &ref->old_oid, &ref->new_oid);
			ref_update_display_info_set_failed(info);
			திரும்பு 1;
		}
	}

	current = lookup_commit_reference_gently(the_repository,
						 &ref->old_oid, 1);
	updated = lookup_commit_reference_gently(the_repository,
						 &ref->new_oid, 1);
	எனில் (!current || !updated) {
		கட்டமைப்பு ref_update_display_info *info;
		மாறிலி எழுத்து *செய்தி;
		மாறிலி எழுத்து *what;
		முழு r;
		/*
		 * Nicely describe the new ref we're fetching.
		 * Base this on the remote's ref name, as it's
		 * more likely to follow a standard layout.
		 */
		எனில் (starts_with(remote_ref->name, "refs/tags/")) {
			செய்தி = "storing tag";
			what = _("[new tag]");
		} இல்லையெனில் எனில் (starts_with(remote_ref->name, "refs/heads/")) {
			செய்தி = "storing head";
			what = _("[new branch]");
		} இல்லையெனில் {
			செய்தி = "storing ref";
			what = _("[new ref]");
		}

		r = s_update_ref(செய்தி, ref, transaction, 0);

		info = ref_update_display_info_append(display_array, '*', '!',
						      what, வெற்று,
						      _("unable to update local ref"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);
		எனில் (r)
			ref_update_display_info_set_failed(info);

		திரும்பு r;
	}

	எனில் (config->show_forced_updates) {
		uint64_t t_before = getnanotime();
		fast_forward = repo_in_merge_bases(the_repository, current,
						   updated);
		எனில் (fast_forward < 0)
			die(வெற்று);
		forced_updates_ms += (getnanotime() - t_before) / 1000000;
	} இல்லையெனில் {
		fast_forward = 1;
	}

	எனில் (fast_forward) {
		கட்டமைப்பு ref_update_display_info *info;
		கட்டமைப்பு strbuf quickref = STRBUF_INIT;
		முழு r;

		strbuf_add_unique_abbrev(&quickref, &current->object.oid, DEFAULT_ABBREV);
		strbuf_addstr(&quickref, "..");
		strbuf_add_unique_abbrev(&quickref, &ref->new_oid, DEFAULT_ABBREV);
		r = s_update_ref("fast-forward", ref, transaction, 1);

		info = ref_update_display_info_append(display_array, ' ', '!',
						      quickref.buf, வெற்று,
						      _("unable to update local ref"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);
		எனில் (r)
			ref_update_display_info_set_failed(info);

		strbuf_release(&quickref);
		திரும்பு r;
	} இல்லையெனில் எனில் (force || ref->force) {
		கட்டமைப்பு ref_update_display_info *info;
		கட்டமைப்பு strbuf quickref = STRBUF_INIT;
		முழு r;

		strbuf_add_unique_abbrev(&quickref, &current->object.oid, DEFAULT_ABBREV);
		strbuf_addstr(&quickref, "...");
		strbuf_add_unique_abbrev(&quickref, &ref->new_oid, DEFAULT_ABBREV);
		r = s_update_ref("forced-update", ref, transaction, 1);

		info = ref_update_display_info_append(display_array, '+', '!',
						      quickref.buf, _("forced update"),
						      _("unable to update local ref"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);

		எனில் (r)
			ref_update_display_info_set_failed(info);

		strbuf_release(&quickref);
		திரும்பு r;
	} இல்லையெனில் {
		கட்டமைப்பு ref_update_display_info *info;
		info = ref_update_display_info_append(display_array, '!', '!',
						      _("[rejected]"), வெற்று,
						      _("non-fast-forward"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);
		ref_update_display_info_set_failed(info);
		திரும்பு 1;
	}
}

நிலையான மாறிலி கட்டமைப்பு object_id *iterate_ref_map(வெறுமை *cb_data)
{
	கட்டமைப்பு ref **rm = cb_data;
	கட்டமைப்பு ref *ref = *rm;

	போது (ref && ref->status == REF_STATUS_REJECT_SHALLOW)
		ref = ref->next;
	எனில் (!ref)
		திரும்பு வெற்று;
	*rm = ref->next;
	திரும்பு &ref->old_oid;
}

கட்டமைப்பு fetch_head {
	FILE *fp;
	கட்டமைப்பு strbuf buf;
};

நிலையான முழு open_fetch_head(கட்டமைப்பு fetch_head *fetch_head)
{
	மாறிலி எழுத்து *filename = git_path_fetch_head(the_repository);

	எனில் (write_fetch_head) {
		fetch_head->fp = fopen(filename, "a");
		எனில் (!fetch_head->fp)
			திரும்பு error_errno(_("cannot open '%s'"), filename);
		strbuf_init(&fetch_head->buf, 0);
	} இல்லையெனில் {
		fetch_head->fp = வெற்று;
	}

	திரும்பு 0;
}

நிலையான வெறுமை append_fetch_head(கட்டமைப்பு fetch_head *fetch_head,
			      மாறிலி கட்டமைப்பு object_id *old_oid,
			      எண்ணிக்கை fetch_head_status fetch_head_status,
			      மாறிலி எழுத்து *note,
			      மாறிலி எழுத்து *url, size_t url_len)
{
	எழுத்து old_oid_hex[GIT_MAX_HEXSZ + 1];
	மாறிலி எழுத்து *merge_status_marker;
	size_t i;

	எனில் (!fetch_head->fp)
		திரும்பு;

	தேர்வு (fetch_head_status) {
	நிலை FETCH_HEAD_NOT_FOR_MERGE:
		merge_status_marker = "not-for-merge";
		நிறுத்து;
	நிலை FETCH_HEAD_MERGE:
		merge_status_marker = "";
		நிறுத்து;
	இயல்பு:
		/* do not write anything to FETCH_HEAD */
		திரும்பு;
	}

	strbuf_addf(&fetch_head->buf, "%s\t%s\t%s",
		    oid_to_hex_r(old_oid_hex, old_oid), merge_status_marker, note);
	க்கு (i = 0; i < url_len; ++i)
		எனில் ('\n' == url[i])
			strbuf_addstr(&fetch_head->buf, "\\n");
		இல்லையெனில்
			strbuf_addch(&fetch_head->buf, url[i]);
	strbuf_addch(&fetch_head->buf, '\n');

	/*
	 * When using an atomic fetch, we do not want to update FETCH_HEAD if
	 * any of the reference updates fails. We thus have to write all
	 * updates to a buffer first and only commit it as soon as all
	 * references have been successfully updated.
	 */
	எனில் (!atomic_fetch) {
		strbuf_write(&fetch_head->buf, fetch_head->fp);
		strbuf_reset(&fetch_head->buf);
	}
}

நிலையான வெறுமை commit_fetch_head(கட்டமைப்பு fetch_head *fetch_head)
{
	எனில் (!fetch_head->fp || !atomic_fetch)
		திரும்பு;
	strbuf_write(&fetch_head->buf, fetch_head->fp);
}

நிலையான வெறுமை close_fetch_head(கட்டமைப்பு fetch_head *fetch_head)
{
	எனில் (!fetch_head->fp)
		திரும்பு;

	fclose(fetch_head->fp);
	strbuf_release(&fetch_head->buf);
}

நிலையான மாறிலி எழுத்து warn_show_forced_updates[] =
N_("fetch normally indicates which branches had a forced update,\n"
   "but that check has been disabled; to re-enable, use '--show-forced-updates'\n"
   "flag or run 'git config fetch.showForcedUpdates true'");
நிலையான மாறிலி எழுத்து warn_time_show_forced_updates[] =
N_("it took %.2f seconds to check forced updates; you can use\n"
   "'--no-show-forced-updates' or run 'git config fetch.showForcedUpdates false'\n"
   "to avoid this check\n");

நிலையான முழு store_updated_refs(கட்டமைப்பு display_state *display_state,
			      முழு connectivity_checked,
			      கட்டமைப்பு ref_transaction *transaction, கட்டமைப்பு ref *ref_map,
			      கட்டமைப்பு fetch_head *fetch_head,
			      மாறிலி கட்டமைப்பு fetch_config *config,
			      கட்டமைப்பு ref_update_display_info_array *display_array)
{
	முழு rc = 0;
	கட்டமைப்பு strbuf note = STRBUF_INIT;
	மாறிலி எழுத்து *what, *kind;
	கட்டமைப்பு ref *rm;
	முழு want_status;

	எனில் (!connectivity_checked) {
		கட்டமைப்பு check_connected_options opt = CHECK_CONNECTED_INIT;

		opt.exclude_hidden_refs_section = "fetch";
		rm = ref_map;
		எனில் (check_connected(iterate_ref_map, &rm, &opt)) {
			rc = error(_("%s did not send all necessary objects"),
				   display_state->url);
			செல் abort;
		}
	}

	/*
	 * We do a pass for each fetch_head_status type in their enum order, so
	 * merged entries are written before not-for-merge. That lets readers
	 * use FETCH_HEAD as a refname to refer to the ref to be merged.
	 */
	க்கு (want_status = FETCH_HEAD_MERGE;
	     want_status <= FETCH_HEAD_IGNORE;
	     want_status++) {
		க்கு (rm = ref_map; rm; rm = rm->next) {
			கட்டமைப்பு ref *ref = வெற்று;

			எனில் (rm->status == REF_STATUS_REJECT_SHALLOW) {
				எனில் (want_status == FETCH_HEAD_MERGE)
					warning(_("rejected %s because shallow roots are not allowed to be updated"),
						rm->peer_ref ? rm->peer_ref->name : rm->name);
				தொடர்;
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
			எனில் (fetch_head->fp) {
				கட்டமைப்பு commit *commit = வெற்று;

				/*
				 * References in "refs/tags/" are often going to point
				 * to annotated tags, which are not part of the
				 * commit-graph. We thus only try to look up refs in
				 * the graph which are not in that namespace to not
				 * regress performance in repositories with many
				 * annotated tags.
				 */
				எனில் (!starts_with(rm->name, "refs/tags/"))
					commit = lookup_commit_in_graph(the_repository, &rm->old_oid);
				எனில் (!commit) {
					commit = lookup_commit_reference_gently(the_repository,
										&rm->old_oid,
										1);
					எனில் (!commit)
						rm->fetch_head_status = FETCH_HEAD_NOT_FOR_MERGE;
				}
			}

			எனில் (rm->fetch_head_status != want_status)
				தொடர்;

			எனில் (rm->peer_ref) {
				ref = alloc_ref(rm->peer_ref->name);
				oidcpy(&ref->old_oid, &rm->peer_ref->old_oid);
				oidcpy(&ref->new_oid, &rm->old_oid);
				ref->force = rm->peer_ref->force;
			}

			எனில் (config->recurse_submodules != RECURSE_SUBMODULES_OFF &&
			    (!rm->peer_ref || !oideq(&ref->old_oid, &ref->new_oid))) {
				check_for_new_submodule_commits(&rm->old_oid);
			}

			எனில் (!strcmp(rm->name, "HEAD")) {
				kind = "";
				what = "";
			} இல்லையெனில் எனில் (skip_prefix(rm->name, "refs/heads/", &what)) {
				kind = "branch";
			} இல்லையெனில் எனில் (skip_prefix(rm->name, "refs/tags/", &what)) {
				kind = "tag";
			} இல்லையெனில் எனில் (skip_prefix(rm->name, "refs/remotes/", &what)) {
				kind = "remote-tracking branch";
			} இல்லையெனில் {
				kind = "";
				what = rm->name;
			}

			strbuf_reset(&note);
			எனில் (*what) {
				எனில் (*kind)
					strbuf_addf(&note, "%s ", kind);
				strbuf_addf(&note, "'%s' of ", what);
			}

			append_fetch_head(fetch_head, &rm->old_oid,
					  rm->fetch_head_status,
					  note.buf, display_state->url,
					  display_state->url_len);

			எனில் (ref) {
				rc |= update_local_ref(ref, transaction, rm,
						       config, display_array);
				விடுவி(ref);
			} இல்லையெனில் எனில் (write_fetch_head || dry_run) {
				/*
				 * Display fetches written to FETCH_HEAD (or
				 * would be written to FETCH_HEAD, if --dry-run
				 * is set).
				 */

				ref_update_display_info_append(display_array, '*', '*',
							       *kind ? kind : "branch",
							       வெற்று, வெற்று, "FETCH_HEAD",
							       rm->name, &rm->new_oid,
							       &rm->old_oid);
			}
		}
	}

	எனில் (advice_enabled(ADVICE_FETCH_SHOW_FORCED_UPDATES)) {
		எனில் (!config->show_forced_updates) {
			warning(_(warn_show_forced_updates));
		} இல்லையெனில் எனில் (forced_updates_ms > FORCED_UPDATES_DELAY_WARNING_IN_MS) {
			warning(_(warn_time_show_forced_updates),
				forced_updates_ms / 1000.0);
		}
	}

 abort:
	strbuf_release(&note);
	திரும்பு rc;
}

/*
 * We would want to bypass the object transfer altogether if
 * everything we are going to fetch already exists and is connected
 * locally.
 */
நிலையான முழு check_exist_and_connected(கட்டமைப்பு ref *ref_map)
{
	கட்டமைப்பு ref *rm = ref_map;
	கட்டமைப்பு check_connected_options opt = CHECK_CONNECTED_INIT;
	கட்டமைப்பு ref *r;

	/*
	 * If we are deepening a shallow clone we already have these
	 * objects reachable.  Running rev-list here will return with
	 * a good (0) exit status and we'll bypass the fetch that we
	 * really need to perform.  Claiming failure now will ensure
	 * we perform the network exchange to deepen our history.
	 */
	எனில் (deepen)
		திரும்பு -1;

	/*
	 * Similarly, if we need to refetch, we always want to perform a full
	 * fetch ignoring existing objects.
	 */
	எனில் (refetch)
		திரும்பு -1;


	/*
	 * check_connected() allows objects to merely be promised, but
	 * we need all direct targets to exist.
	 */
	க்கு (r = rm; r; r = r->next) {
		எனில் (!odb_has_object(the_repository->objects, &r->old_oid,
				    ODB_HAS_OBJECT_RECHECK_PACKED))
			திரும்பு -1;
	}

	opt.quiet = 1;
	opt.exclude_hidden_refs_section = "fetch";
	திரும்பு check_connected(iterate_ref_map, &rm, &opt);
}

நிலையான முழு fetch_and_consume_refs(கட்டமைப்பு display_state *display_state,
				  கட்டமைப்பு transport *transport,
				  கட்டமைப்பு ref_transaction *transaction,
				  கட்டமைப்பு ref *ref_map,
				  கட்டமைப்பு fetch_head *fetch_head,
				  மாறிலி கட்டமைப்பு fetch_config *config,
				  கட்டமைப்பு ref_update_display_info_array *display_array)
{
	முழு connectivity_checked = 1;
	முழு ret;

	/*
	 * We don't need to perform a fetch in case we can already satisfy all
	 * refs.
	 */
	ret = check_exist_and_connected(ref_map);
	எனில் (ret) {
		trace2_region_enter("fetch", "fetch_refs", the_repository);
		ret = transport_fetch_refs(transport, ref_map);
		trace2_region_leave("fetch", "fetch_refs", the_repository);
		எனில் (ret)
			செல் out;
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
	திரும்பு ret;
}

நிலையான முழு prune_refs(கட்டமைப்பு display_state *display_state,
		      கட்டமைப்பு refspec *rs,
		      கட்டமைப்பு ref_transaction *transaction,
		      கட்டமைப்பு ref *ref_map)
{
	முழு result = 0;
	கட்டமைப்பு ref *ref, *stale_refs = get_stale_heads(rs, ref_map);
	கட்டமைப்பு strbuf err = STRBUF_INIT;
	கட்டமைப்பு string_list refnames = STRING_LIST_INIT_NODUP;

	க்கு (ref = stale_refs; ref; ref = ref->next)
		string_list_append(&refnames, ref->name);

	எனில் (!dry_run) {
		எனில் (transaction) {
			க்கு (ref = stale_refs; ref; ref = ref->next) {
				result = ref_transaction_delete(transaction, ref->name, வெற்று,
								வெற்று, 0, "fetch: prune", &err);
				எனில் (result)
					செல் cleanup;
			}
		} இல்லையெனில் {
			result = refs_delete_refs(get_main_ref_store(the_repository),
						  "fetch: prune", &refnames,
						  0);
		}
	}

	எனில் (verbosity >= 0) {
		முழு summary_width = transport_summary_width(stale_refs);

		க்கு (ref = stale_refs; ref; ref = ref->next) {
			display_ref_update(display_state, '-', _("[deleted]"), வெற்று,
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
	திரும்பு result;
}

நிலையான வெறுமை check_not_current_branch(கட்டமைப்பு ref *ref_map)
{
	மாறிலி எழுத்து *path;
	க்கு (; ref_map; ref_map = ref_map->next)
		எனில் (ref_map->peer_ref &&
		    starts_with(ref_map->peer_ref->name, "refs/heads/") &&
		    (path = branch_checked_out(ref_map->peer_ref->name)))
			die(_("refusing to fetch into branch '%s' "
			      "checked out at '%s'"),
			    ref_map->peer_ref->name, path);
}

நிலையான முழு truncate_fetch_head(வெறுமை)
{
	மாறிலி எழுத்து *filename = git_path_fetch_head(the_repository);
	FILE *fp = fopen_for_writing(filename);

	எனில் (!fp)
		திரும்பு error_errno(_("cannot open '%s'"), filename);
	fclose(fp);
	திரும்பு 0;
}

நிலையான வெறுமை set_option(கட்டமைப்பு transport *transport, மாறிலி எழுத்து *name, மாறிலி எழுத்து *மதிப்பு)
{
	முழு r = transport_set_option(transport, name, மதிப்பு);
	எனில் (r < 0)
		die(_("option \"%s\" value \"%s\" is not valid for %s"),
		    name, மதிப்பு, transport->url);
	எனில் (r > 0)
		warning(_("option \"%s\" is ignored for %s"),
			name, transport->url);
}


நிலையான முழு add_oid(மாறிலி கட்டமைப்பு reference *ref, வெறுமை *cb_data)
{
	கட்டமைப்பு oid_array *oids = cb_data;

	oid_array_append(oids, ref->oid);
	திரும்பு 0;
}

நிலையான வெறுமை add_negotiation_tips(கட்டமைப்பு string_list *உள்ளீடு_list,
				 கட்டமைப்பு oid_array **output_list,
				 மாறிலி எழுத்து *argname)
{
	கட்டமைப்பு oid_array *oids = xcalloc(1, அளவு(*oids));
	முழு i;

	க்கு (i = 0; i < உள்ளீடு_list->nr; i++) {
		மாறிலி எழுத்து *s = உள்ளீடு_list->பொருட்கள்[i].string;
		கட்டமைப்பு refs_for_each_ref_options opts = {
			.pattern = s,
		};
		முழு old_nr;
		எனில் (!has_glob_specials(s)) {
			கட்டமைப்பு object_id oid;

			/* Ignore missing reference. */
			எனில் (repo_get_oid(the_repository, s, &oid))
				தொடர்;
			/* Fail on missing object pointed by ref. */
			எனில் (!odb_has_object(the_repository->objects, &oid, 0))
				die(_("the object %s does not exist"), s);

			oid_array_append(oids, &oid);
			தொடர்;
		}
		old_nr = oids->nr;
		refs_for_each_ref_ext(get_main_ref_store(the_repository),
				      add_oid, oids, &opts);
		எனில் (old_nr == oids->nr)
			warning(_("ignoring %s=%s because it does not match any refs"),
				argname, s);
	}
	*output_list = oids;
}

நிலையான கட்டமைப்பு transport *prepare_transport(கட்டமைப்பு remote *remote, முழு deepen,
					   கட்டமைப்பு list_objects_filter_options *filter_options)
{
	கட்டமைப்பு transport *transport;

	transport = transport_get(remote, வெற்று);
	transport_set_verbosity(transport, verbosity, progress);
	transport->family = family;
	எனில் (upload_pack)
		set_option(transport, TRANS_OPT_UPLOADPACK, upload_pack);
	எனில் (keep)
		set_option(transport, TRANS_OPT_KEEP, "yes");
	எனில் (depth)
		set_option(transport, TRANS_OPT_DEPTH, depth);
	எனில் (deepen && deepen_since)
		set_option(transport, TRANS_OPT_DEEPEN_SINCE, deepen_since);
	எனில் (deepen && deepen_not.nr)
		set_option(transport, TRANS_OPT_DEEPEN_NOT,
			   (மாறிலி எழுத்து *)&deepen_not);
	எனில் (deepen_relative)
		set_option(transport, TRANS_OPT_DEEPEN_RELATIVE, "yes");
	எனில் (update_shallow)
		set_option(transport, TRANS_OPT_UPDATE_SHALLOW, "yes");
	எனில் (refetch)
		set_option(transport, TRANS_OPT_REFETCH, "yes");
	எனில் (filter_options->choice) {
		மாறிலி எழுத்து *spec =
			expand_list_objects_filter_spec(filter_options);
		set_option(transport, TRANS_OPT_LIST_OBJECTS_FILTER, spec);
		set_option(transport, TRANS_OPT_FROM_PROMISOR, "1");
	}
	எனில் (negotiation_restrict.nr) {
		எனில் (transport->smart_options)
			add_negotiation_tips(&negotiation_restrict,
					     &transport->smart_options->negotiation_restrict_tips,
					     "--negotiation-restrict");
		இல்லையெனில்
			warning(_("ignoring %s because the protocol does not support it"),
				"--negotiation-restrict");
	} இல்லையெனில் எனில் (remote->negotiation_restrict.nr) {
		கட்டமைப்பு string_list_item *item;
		for_each_string_list_item(item, &remote->negotiation_restrict)
			string_list_append(&negotiation_restrict, item->string);
		எனில் (transport->smart_options)
			add_negotiation_tips(&negotiation_restrict,
					     &transport->smart_options->negotiation_restrict_tips,
					     "--negotiation-restrict");
		இல்லையெனில் {
			கட்டமைப்பு strbuf config_name = STRBUF_INIT;
			strbuf_addf(&config_name, "remote.%s.negotiationRestrict", remote->name);
			warning(_("ignoring %s because the protocol does not support it"),
				config_name.buf);
			strbuf_release(&config_name);
		}
	}
	எனில் (negotiation_include.nr) {
		எனில் (transport->smart_options)
			add_negotiation_tips(&negotiation_include,
					     &transport->smart_options->negotiation_include_tips,
					     "--negotiation-include");
		இல்லையெனில்
			warning(_("ignoring %s because the protocol does not support it"),
				"--negotiation-include");
	} இல்லையெனில் எனில் (remote->negotiation_include.nr) {
		எனில் (transport->smart_options) {
			add_negotiation_tips(&remote->negotiation_include,
					     &transport->smart_options->negotiation_include_tips,
					     "--negotiation-include");
		} இல்லையெனில் {
			கட்டமைப்பு strbuf config_name = STRBUF_INIT;
			strbuf_addf(&config_name, "remote.%s.negotiationInclude", remote->name);
			warning(_("ignoring %s because the protocol does not support it"),
				config_name.buf);
			strbuf_release(&config_name);
		}
	}
	திரும்பு transport;
}

நிலையான முழு backfill_tags(கட்டமைப்பு display_state *display_state,
			 கட்டமைப்பு transport *transport,
			 கட்டமைப்பு ref_transaction *transaction,
			 கட்டமைப்பு ref *ref_map,
			 கட்டமைப்பு fetch_head *fetch_head,
			 மாறிலி கட்டமைப்பு fetch_config *config,
			 கட்டமைப்பு ref_update_display_info_array *display_array,
			 கட்டமைப்பு list_objects_filter_options *filter_options)
{
	முழு retcode, cannot_reuse;

	/*
	 * Once we have set TRANS_OPT_DEEPEN_SINCE, we can't unset it
	 * when remote helper is used (setting it to an empty string
	 * is not unsetting). We could extend the remote helper
	 * protocol for that, but for now, just force a new connection
	 * without deepen-since. Similar story for deepen-not.
	 */
	cannot_reuse = transport->cannot_reuse ||
		deepen_since || deepen_not.nr;
	எனில் (cannot_reuse) {
		gsecondary = prepare_transport(transport->remote, 0, filter_options);
		transport = gsecondary;
	}

	transport_set_option(transport, TRANS_OPT_FOLLOWTAGS, வெற்று);
	transport_set_option(transport, TRANS_OPT_DEPTH, "0");
	transport_set_option(transport, TRANS_OPT_DEEPEN_RELATIVE, வெற்று);
	retcode = fetch_and_consume_refs(display_state, transport, transaction, ref_map,
					 fetch_head, config, display_array);

	எனில் (gsecondary) {
		transport_disconnect(gsecondary);
		gsecondary = வெற்று;
	}

	திரும்பு retcode;
}

நிலையான மாறிலி எழுத்து *strip_refshead(மாறிலி எழுத்து *name){
	skip_prefix(name, "refs/heads/", &name);
	திரும்பு name;
}

நிலையான வெறுமை set_head_advice_செய்தி(மாறிலி எழுத்து *remote, மாறிலி எழுத்து *head_name)
{
	மாறிலி எழுத்து message_advice_set_head[] =
	N_("Run 'git remote set-head %s %s' to follow the change, or modify\n"
	   "either of the 'remote.%s.followRemoteHEAD' or 'fetch.followRemoteHEAD'\n"
	   "configuration variables to handle the situation differently.\n\n"

	   "Using this specific setting\n\n"
	   "    git config set remote.%s.followRemoteHEAD warn-if-not-%s\n\n"
	   "will suppress the warning until the remote changes HEAD to something else.");

	advise_if_enabled(ADVICE_FETCH_SET_HEAD_WARN, _(message_advice_set_head),
			remote, head_name, remote, remote, head_name);
}

நிலையான வெறுமை warn_set_head(மாறிலி எழுத்து *remote, மாறிலி எழுத்து *head_name,
			கட்டமைப்பு strbuf *buf_prev, முழு updateres) {
	கட்டமைப்பு strbuf buf_prefix = STRBUF_INIT;
	மாறிலி எழுத்து *prev_head = வெற்று;

	strbuf_addf(&buf_prefix, "refs/remotes/%s/", remote);
	skip_prefix(buf_prev->buf, buf_prefix.buf, &prev_head);

	எனில் (prev_head && strcmp(prev_head, head_name)) {
		printf("'HEAD' at '%s' is '%s', but we have '%s' locally.\n",
			remote, head_name, prev_head);
		set_head_advice_செய்தி(remote, head_name);
	}
	இல்லையெனில் எனில் (updateres && buf_prev->len) {
		printf("'HEAD' at '%s' is '%s', "
			"but we have a detached HEAD pointing to '%s' locally.\n",
			remote, head_name, buf_prev->buf);
		set_head_advice_செய்தி(remote, head_name);
	}
	strbuf_release(&buf_prefix);
}

நிலையான முழு set_head(மாறிலி கட்டமைப்பு ref *remote_refs, கட்டமைப்பு remote *remote,
			முழு follow_remote_head)
{
	முழு result = 0, create_only, baremirror, was_detached;
	கட்டமைப்பு strbuf b_head = STRBUF_INIT, b_remote_head = STRBUF_INIT,
		      b_local_head = STRBUF_INIT;
	மாறிலி எழுத்து *no_warn_branch = remote->no_warn_branch;
	எழுத்து *head_name = வெற்று;
	கட்டமைப்பு ref *ref, *matches;
	கட்டமைப்பு ref *fetch_map = வெற்று, **fetch_map_tail = &fetch_map;
	கட்டமைப்பு refspec_item refspec = {
		.force = 0,
		.pattern = 1,
		.src = (எழுத்து *) "refs/heads/*",
		.dst = (எழுத்து *) "refs/heads/*",
	};
	கட்டமைப்பு string_list heads = STRING_LIST_INIT_DUP;
	கட்டமைப்பு ref_store *refs = get_main_ref_store(the_repository);

	get_fetch_map(remote_refs, &refspec, &fetch_map_tail, 0);
	matches = guess_remote_head(find_ref_by_name(remote_refs, "HEAD"),
				    fetch_map, REMOTE_GUESS_HEAD_ALL);
	க்கு (ref = matches; ref; ref = ref->next) {
		string_list_append(&heads, strip_refshead(ref->name));
	}

	எனில் (!heads.nr)
		result = 1;
	இல்லையெனில் எனில் (heads.nr > 1)
		result = 1;
	இல்லையெனில்
		head_name = xstrdup(heads.பொருட்கள்[0].string);

	எனில் (!head_name)
		செல் cleanup;
	baremirror = is_bare_repository(the_repository) && remote->mirror;
	create_only = follow_remote_head == FOLLOW_REMOTE_ALWAYS ? 0 : !baremirror;
	எனில் (baremirror) {
		strbuf_addstr(&b_head, "HEAD");
		strbuf_addf(&b_remote_head, "refs/heads/%s", head_name);
	} இல்லையெனில் {
		strbuf_addf(&b_head, "refs/remotes/%s/HEAD", remote->name);
		strbuf_addf(&b_remote_head, "refs/remotes/%s/%s", remote->name, head_name);
	}
	/* make sure it's valid */
	எனில் (!baremirror && !refs_ref_exists(refs, b_remote_head.buf)) {
		result = 1;
		செல் cleanup;
	}
	was_detached = refs_update_symref_extended(refs, b_head.buf, b_remote_head.buf,
					"fetch", &b_local_head, create_only);
	எனில் (was_detached == -1) {
		result = 1;
		செல் cleanup;
	}
	எனில் (verbosity >= 0 &&
		follow_remote_head == FOLLOW_REMOTE_WARN &&
		(!no_warn_branch || strcmp(no_warn_branch, head_name)))
		warn_set_head(remote->name, head_name, &b_local_head, was_detached);

cleanup:
	விடுவி(head_name);
	free_refs(fetch_map);
	free_refs(matches);
	string_list_clear(&heads, 0);
	strbuf_release(&b_head);
	strbuf_release(&b_local_head);
	strbuf_release(&b_remote_head);
	திரும்பு result;
}

கட்டமைப்பு ref_rejection_data {
	முழு *retcode;
	bool conflict_செய்தி_shown;
	bool case_sensitive_செய்தி_shown;
	மாறிலி எழுத்து *remote_name;
	கட்டமைப்பு strmap *rejected_refs;
};

நிலையான வெறுமை ref_transaction_rejection_handler(மாறிலி எழுத்து *refname,
					      மாறிலி கட்டமைப்பு object_id *old_oid UNUSED,
					      மாறிலி கட்டமைப்பு object_id *new_oid UNUSED,
					      மாறிலி எழுத்து *old_target UNUSED,
					      மாறிலி எழுத்து *new_target UNUSED,
					      எண்ணிக்கை ref_transaction_error err,
					      மாறிலி எழுத்து *details,
					      வெறுமை *cb_data)
{
	கட்டமைப்பு ref_rejection_data *data = cb_data;

	எனில் (err == REF_TRANSACTION_ERROR_CASE_CONFLICT && repo_ignore_case(the_repository) &&
	    !data->case_sensitive_செய்தி_shown) {
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
		data->case_sensitive_செய்தி_shown = true;
	} இல்லையெனில் எனில் (err == REF_TRANSACTION_ERROR_NAME_CONFLICT &&
		   !data->conflict_செய்தி_shown) {
		error(_("some local refs could not be updated; try running\n"
			" 'git remote prune %s' to remove any old, conflicting "
			"branches"), data->remote_name);
		data->conflict_செய்தி_shown = true;
	} இல்லையெனில் {
		எனில் (details)
			error("%s", details);
		இல்லையெனில்
			error(_("fetching ref %s failed: %s"),
			      refname, ref_transaction_error_செய்தி(err));
	}

	strmap_put(data->rejected_refs, refname, வெற்று);
	*data->retcode = 1;
}

/*
 * Commit the reference transaction. If it isn't an atomic transaction, handle
 * rejected updates as part of using batched updates.
 */
நிலையான முழு commit_ref_transaction(கட்டமைப்பு ref_transaction **transaction,
				  bool is_atomic, மாறிலி எழுத்து *remote_name,
				  கட்டமைப்பு strmap *rejected_refs,
				  கட்டமைப்பு strbuf *err)
{
	முழு retcode = ref_transaction_commit(*transaction, err);
	எனில் (retcode)
		செல் out;

	எனில் (!is_atomic) {
		கட்டமைப்பு ref_rejection_data data = {
			.conflict_செய்தி_shown = 0,
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
	*transaction = வெற்று;
	திரும்பு retcode;
}

நிலையான வெறுமை collect_updated_tips(கட்டமைப்பு oidset *tips, கட்டமைப்பு ref *ref_map)
{
	கட்டமைப்பு ref *rm;
	க்கு (rm = ref_map; rm; rm = rm->next) {
		கட்டமைப்பு commit *commit;
		/*
		 * Shallow-rejected refs are not stored and their history
		 * is incomplete, so skip them.
		 */
		எனில் (rm->status == REF_STATUS_REJECT_SHALLOW)
			தொடர்;
		எனில் (is_null_oid(&rm->old_oid))
			தொடர்;
		எனில் (rm->peer_ref &&
		    oideq(&rm->old_oid, &rm->peer_ref->old_oid))
			தொடர்;
		commit = lookup_commit_reference_gently(the_repository,
							&rm->old_oid, 1);
		எனில் (commit)
			oidset_செருகு(tips, &commit->object.oid);
	}
}

நிலையான முழு do_fetch(கட்டமைப்பு transport *transport,
		    கட்டமைப்பு refspec *rs,
		    மாறிலி கட்டமைப்பு fetch_config *config,
		    கட்டமைப்பு list_objects_filter_options *filter_options,
		    கட்டமைப்பு oidset *updated_tips)
{
	கட்டமைப்பு ref_transaction *transaction = வெற்று;
	கட்டமைப்பு ref *ref_map = வெற்று;
	கட்டமைப்பு display_state display_state = { 0 };
	முழு autotags = (transport->remote->fetch_tags == 1);
	முழு retcode = 0;
	மாறிலி கட்டமைப்பு ref *remote_refs;
	கட்டமைப்பு transport_ls_refs_options transport_ls_refs_options =
		TRANSPORT_LS_REFS_OPTIONS_INIT;
	கட்டமைப்பு fetch_head fetch_head = { 0 };
	கட்டமைப்பு strbuf err = STRBUF_INIT;
	கட்டமைப்பு ref_update_display_info_array display_array = { 0 };
	கட்டமைப்பு strmap rejected_refs = STRMAP_INIT;
	முழு summary_width = 0;
	முழு follow_remote_head = FOLLOW_REMOTE_NEVER;

	எனில் (tags == TAGS_DEFAULT) {
		எனில் (transport->remote->fetch_tags == 2)
			tags = TAGS_SET;
		எனில் (transport->remote->fetch_tags == -1)
			tags = TAGS_UNSET;
	}

	/* if not appending, truncate FETCH_HEAD */
	எனில் (!append && write_fetch_head) {
		retcode = truncate_fetch_head();
		எனில் (retcode)
			செல் cleanup;
	}

	எனில் (rs->nr) {
		refspec_ref_prefixes(rs, &transport_ls_refs_options.ref_prefixes);
	} இல்லையெனில் {
		கட்டமைப்பு branch *branch = branch_get(வெற்று);

		எனில் (transport->remote->fetch.nr) {
			refspec_ref_prefixes(&transport->remote->fetch,
					     &transport_ls_refs_options.ref_prefixes);
			/*
			 * See remote.c's handling of remote.<name>.followRemoteHEAD
			 * for the analogous, still-unresolved case.
			 */
			எனில் (transport->remote->follow_remote_head)
				follow_remote_head = transport->remote->follow_remote_head;
			இல்லையெனில் எனில் (config->follow_remote_head_seen)
				follow_remote_head = get_follow_remote_head(config->follow_remote_head_raw);
			இல்லையெனில்
				follow_remote_head = BUILTIN_FOLLOW_REMOTE_HEAD_DFLT;
		}
		எனில் (branch && branch_has_merge_config(branch) &&
		    !strcmp(branch->remote_name, transport->remote->name)) {
			முழு i;
			க்கு (i = 0; i < branch->merge_nr; i++) {
				strvec_தள்ளு(&transport_ls_refs_options.ref_prefixes,
					    branch->merge[i]->src);
			}
		}

		/*
		 * If there are no refs specified to fetch, then we just
		 * fetch HEAD; mention that to narrow the advertisement.
		 */
		எனில் (!transport_ls_refs_options.ref_prefixes.nr)
			strvec_தள்ளு(&transport_ls_refs_options.ref_prefixes,
				    "HEAD");
	}

	எனில் (tags == TAGS_SET || tags == TAGS_DEFAULT)
		strvec_தள்ளு(&transport_ls_refs_options.ref_prefixes,
			    "refs/tags/");

	எனில் (follow_remote_head != FOLLOW_REMOTE_NEVER)
		strvec_தள்ளு(&transport_ls_refs_options.ref_prefixes,
			    "HEAD");

	/*
	 * Only initiate ref listing if we have at least one ref we want to
	 * know about.
	 */
	எனில் (transport_ls_refs_options.ref_prefixes.nr) {
		trace2_region_enter("fetch", "remote_refs", the_repository);
		remote_refs = transport_get_remote_refs(transport,
							&transport_ls_refs_options);
		trace2_region_leave("fetch", "remote_refs", the_repository);
	} இல்லையெனில்
		remote_refs = வெற்று;

	transport_ls_refs_options_release(&transport_ls_refs_options);

	ref_map = get_ref_map(transport->remote, remote_refs, rs,
			      tags, &autotags);
	எனில் (!update_head_ok)
		check_not_current_branch(ref_map);

	retcode = open_fetch_head(&fetch_head);
	எனில் (retcode)
		செல் cleanup;

	display_state_init(&display_state, ref_map, transport->url,
			   config->display_format);

	எனில் (atomic_fetch) {
		transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
							  0, &err);
		எனில் (!transaction) {
			retcode = -1;
			செல் cleanup;
		}
	}

	எனில் (tags == TAGS_DEFAULT && autotags)
		transport_set_option(transport, TRANS_OPT_FOLLOWTAGS, "1");
	எனில் (prune) {
		/*
		 * We only prune based on refspecs specified
		 * explicitly (via command line or configuration); we
		 * don't care whether --tags was specified.
		 */
		எனில் (rs->nr) {
			retcode = prune_refs(&display_state, rs, transaction, ref_map);
		} இல்லையெனில் {
			retcode = prune_refs(&display_state, &transport->remote->fetch,
					     transaction, ref_map);
		}
		எனில் (retcode != 0)
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
	எனில் (!transaction) {
		transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
							  REF_TRANSACTION_ALLOW_FAILURE, &err);
		எனில் (!transaction) {
			retcode = -1;
			செல் cleanup;
		}
	}

	எனில் (fetch_and_consume_refs(&display_state, transport, transaction, ref_map,
				   &fetch_head, config, &display_array)) {
		retcode = 1;
		செல் cleanup;
	}

	/*
	 * If neither --no-tags nor --tags was specified, do automated tag
	 * following.
	 */
	எனில் (tags == TAGS_DEFAULT && autotags) {
		கட்டமைப்பு ref *tags_ref_map = வெற்று, **tail = &tags_ref_map;

		find_non_local_tags(remote_refs, transaction, &tags_ref_map, &tail);
		எனில் (tags_ref_map) {
			/*
			 * If backfilling of tags fails then we want to tell
			 * the user so, but we have to continue regardless to
			 * populate upstream information of the references we
			 * have already fetched above. The exception though is
			 * when `--atomic` is passed: in that case we'll abort
			 * the transaction and don't commit anything.
			 */
			எனில் (backfill_tags(&display_state, transport, transaction, tags_ref_map,
					  &fetch_head, config, &display_array, filter_options))
				retcode = 1;
		}

		free_refs(tags_ref_map);
	}

	எனில் (retcode)
		செல் cleanup;

	எனில் (verbosity >= 0)
		summary_width = transport_summary_width(ref_map);

	retcode = commit_ref_transaction(&transaction, atomic_fetch,
					 transport->remote->name,
					 &rejected_refs, &err);
	/*
	 * With '--atomic', bail out if the transaction fails. Without '--atomic',
	 * continue to fetch head and perform other post-fetch operations.
	 */
	எனில் (retcode && atomic_fetch)
		செல் cleanup;

	commit_fetch_head(&fetch_head);

	collect_updated_tips(updated_tips, ref_map);

	எனில் (set_upstream) {
		கட்டமைப்பு branch *branch = branch_get("HEAD");
		கட்டமைப்பு ref *rm;
		கட்டமைப்பு ref *source_ref = வெற்று;

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
		க்கு (rm = ref_map; rm; rm = rm->next) {
			எனில் (!rm->peer_ref) {
				எனில் (source_ref) {
					warning(_("multiple branches detected, incompatible with --set-upstream"));
					செல் cleanup;
				} இல்லையெனில் {
					source_ref = rm;
				}
			}
		}
		எனில் (source_ref) {
			எனில் (!branch) {
				மாறிலி எழுத்து *shortname = source_ref->name;
				skip_prefix(shortname, "refs/heads/", &shortname);

				warning(_("could not set upstream of HEAD to '%s' from '%s' when "
					  "it does not point to any branch."),
					shortname, transport->remote->name);
				செல் cleanup;
			}

			எனில் (!strcmp(source_ref->name, "HEAD") ||
			    starts_with(source_ref->name, "refs/heads/"))
				install_branch_config(0,
						      branch->name,
						      transport->remote->name,
						      source_ref->name);
			இல்லையெனில் எனில் (starts_with(source_ref->name, "refs/remotes/"))
				warning(_("not setting upstream for a remote remote-tracking branch"));
			இல்லையெனில் எனில் (starts_with(source_ref->name, "refs/tags/"))
				warning(_("not setting upstream for a remote tag"));
			இல்லையெனில்
				warning(_("unknown branch type"));
		} இல்லையெனில் {
			warning(_("no source branch found;\n"
				  "you need to specify exactly one branch with the --set-upstream option"));
		}
	}
	எனில் (follow_remote_head != FOLLOW_REMOTE_NEVER) {
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
	எனில் (retcode && !atomic_fetch && transaction)
		commit_ref_transaction(&transaction, false,
				       transport->remote->name,
				       &rejected_refs, &err);

	க்கு (size_t i = 0; i < display_array.nr; i++) {
		கட்டமைப்பு ref_update_display_info *info = &display_array.info[i];

		எனில் (!info->failed && strmap_contains(&rejected_refs, info->ref))
			ref_update_display_info_set_failed(info);
		ref_update_display_info_display(info, &display_state, summary_width);
		ref_update_display_info_free(info);
	}

	எனில் (retcode) {
		எனில் (err.len) {
			error("%s", err.buf);
			strbuf_reset(&err);
		}
		எனில் (transaction && ref_transaction_abort(transaction, &err) &&
		    err.len)
			error("%s", err.buf);
		transaction = வெற்று;
	}

	எனில் (transaction)
		ref_transaction_free(transaction);

	விடுவி(display_array.info);
	strmap_clear(&rejected_refs, 0);
	display_state_release(&display_state);
	close_fetch_head(&fetch_head);
	strbuf_release(&err);
	free_refs(ref_map);
	திரும்பு retcode;
}

நிலையான முழு get_one_remote_for_fetch(கட்டமைப்பு remote *remote, வெறுமை *priv)
{
	கட்டமைப்பு string_list *list = priv;
	எனில் (!remote->skip_default_update)
		string_list_append(list, remote->name);
	திரும்பு 0;
}

நிலையான வெறுமை add_options_to_argv(கட்டமைப்பு strvec *argv,
				மாறிலி கட்டமைப்பு fetch_config *config)
{
	எனில் (dry_run)
		strvec_தள்ளு(argv, "--dry-run");
	எனில் (prune != -1)
		strvec_தள்ளு(argv, prune ? "--prune" : "--no-prune");
	எனில் (prune_tags != -1)
		strvec_தள்ளு(argv, prune_tags ? "--prune-tags" : "--no-prune-tags");
	எனில் (update_head_ok)
		strvec_தள்ளு(argv, "--update-head-ok");
	எனில் (force)
		strvec_தள்ளு(argv, "--force");
	எனில் (keep)
		strvec_தள்ளு(argv, "--keep");
	எனில் (config->recurse_submodules == RECURSE_SUBMODULES_ON)
		strvec_தள்ளு(argv, "--recurse-submodules");
	இல்லையெனில் எனில் (config->recurse_submodules == RECURSE_SUBMODULES_OFF)
		strvec_தள்ளு(argv, "--no-recurse-submodules");
	இல்லையெனில் எனில் (config->recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND)
		strvec_தள்ளு(argv, "--recurse-submodules=on-demand");
	எனில் (tags == TAGS_SET)
		strvec_தள்ளு(argv, "--tags");
	இல்லையெனில் எனில் (tags == TAGS_UNSET)
		strvec_தள்ளு(argv, "--no-tags");
	எனில் (verbosity >= 2)
		strvec_தள்ளு(argv, "-v");
	எனில் (verbosity >= 1)
		strvec_தள்ளு(argv, "-v");
	இல்லையெனில் எனில் (verbosity < 0)
		strvec_தள்ளு(argv, "-q");
	எனில் (family == TRANSPORT_FAMILY_IPV4)
		strvec_தள்ளு(argv, "--ipv4");
	இல்லையெனில் எனில் (family == TRANSPORT_FAMILY_IPV6)
		strvec_தள்ளு(argv, "--ipv6");
	எனில் (!write_fetch_head)
		strvec_தள்ளு(argv, "--no-write-fetch-head");
	எனில் (config->display_format == DISPLAY_FORMAT_PORCELAIN)
		strvec_pushf(argv, "--porcelain");
}

/* Fetch multiple remotes in parallel */

கட்டமைப்பு parallel_fetch_state {
	மாறிலி எழுத்து **argv;
	கட்டமைப்பு string_list *remotes;
	முழு next, result;
	மாறிலி கட்டமைப்பு fetch_config *config;
};

நிலையான முழு fetch_next_remote(கட்டமைப்பு child_process *cp,
			     கட்டமைப்பு strbuf *out UNUSED,
			     வெறுமை *cb, வெறுமை **task_cb)
{
	கட்டமைப்பு parallel_fetch_state *state = cb;
	எழுத்து *remote;

	எனில் (state->next < 0 || state->next >= state->remotes->nr)
		திரும்பு 0;

	remote = state->remotes->பொருட்கள்[state->next++].string;
	*task_cb = remote;

	strvec_pushv(&cp->args, state->argv);
	strvec_தள்ளு(&cp->args, remote);
	cp->git_cmd = 1;

	எனில் (verbosity >= 0 && state->config->display_format != DISPLAY_FORMAT_PORCELAIN)
		printf(_("Fetching %s\n"), remote);

	திரும்பு 1;
}

நிலையான முழு fetch_failed_to_start(கட்டமைப்பு strbuf *out UNUSED,
				 வெறுமை *cb, வெறுமை *task_cb)
{
	கட்டமைப்பு parallel_fetch_state *state = cb;
	மாறிலி எழுத்து *remote = task_cb;

	state->result = error(_("could not fetch %s"), remote);

	திரும்பு 0;
}

நிலையான முழு fetch_finished(முழு result, கட்டமைப்பு strbuf *out,
			  வெறுமை *cb, வெறுமை *task_cb)
{
	கட்டமைப்பு parallel_fetch_state *state = cb;
	மாறிலி எழுத்து *remote = task_cb;

	எனில் (result) {
		strbuf_addf(out, _("could not fetch '%s' (exit code: %d)\n"),
			    remote, result);
		state->result = -1;
	}

	திரும்பு 0;
}

நிலையான முழு fetch_multiple(கட்டமைப்பு string_list *list, முழு max_children,
			  மாறிலி கட்டமைப்பு fetch_config *config)
{
	முழு i, result = 0;
	கட்டமைப்பு strvec argv = STRVEC_INIT;

	எனில் (!append && write_fetch_head) {
		முழு errcode = truncate_fetch_head();
		எனில் (errcode)
			திரும்பு errcode;
	}

	/*
	 * Cancel out the fetch.bundleURI config when running subprocesses,
	 * to avoid fetching from the same bundle list multiple times.
	 */
	strvec_pushl(&argv, "-c", "fetch.bundleURI=",
		     "fetch", "--append", "--no-auto-gc",
		     "--no-write-commit-graph", வெற்று);
	க்கு (i = 0; i < server_options.nr; i++)
		strvec_pushf(&argv, "--server-option=%s", server_options.பொருட்கள்[i].string);
	add_options_to_argv(&argv, config);

	எனில் (max_children != 1 && list->nr != 1) {
		கட்டமைப்பு parallel_fetch_state state = { argv.v, list, 0, 0, config };
		மாறிலி கட்டமைப்பு run_process_parallel_opts opts = {
			.tr2_category = "fetch",
			.tr2_label = "parallel/fetch",

			.processes = max_children,

			.get_next_task = &fetch_next_remote,
			.start_failure = &fetch_failed_to_start,
			.task_finished = &fetch_finished,
			.data = &state,
		};

		strvec_தள்ளு(&argv, "--end-of-options");

		run_processes_parallel(&opts);
		result = state.result;
	} இல்லையெனில்
		க்கு (i = 0; i < list->nr; i++) {
			மாறிலி எழுத்து *name = list->பொருட்கள்[i].string;
			கட்டமைப்பு child_process cmd = CHILD_PROCESS_INIT;

			strvec_pushv(&cmd.args, argv.v);
			strvec_தள்ளு(&cmd.args, name);
			எனில் (verbosity >= 0 && config->display_format != DISPLAY_FORMAT_PORCELAIN)
				printf(_("Fetching %s\n"), name);
			cmd.git_cmd = 1;
			எனில் (run_command(&cmd)) {
				error(_("could not fetch %s"), name);
				result = 1;
			}
		}

	strvec_clear(&argv);
	திரும்பு !!result;
}

/*
 * Fetching from the promisor remote should use the given filter-spec
 * or inherit the default filter-spec from the config.
 */
நிலையான உள்வரி வெறுமை fetch_one_setup_partial(கட்டமைப்பு remote *remote,
					   கட்டமைப்பு list_objects_filter_options *filter_options)
{
	/*
	 * Explicit --no-filter argument overrides everything, regardless
	 * of any prior partial clones and fetches.
	 */
	எனில் (filter_options->no_filter)
		திரும்பு;

	/*
	 * If no prior partial clone/fetch and the current fetch DID NOT
	 * request a partial-fetch, do a normal fetch.
	 */
	எனில் (!repo_has_promisor_remote(the_repository) && !filter_options->choice)
		திரும்பு;

	/*
	 * If this is a partial-fetch request, we enable partial on
	 * this repo if not already enabled and remember the given
	 * filter-spec as the default for subsequent fetches to this
	 * remote if there is currently no default filter-spec.
	 */
	எனில் (filter_options->choice) {
		partial_clone_register(remote->name, filter_options);
		திரும்பு;
	}

	/*
	 * Do a partial-fetch from the promisor remote using either the
	 * explicitly given filter-spec or inherit the filter-spec from
	 * the config.
	 */
	எனில் (!filter_options->choice)
		partial_clone_get_default_filter_spec(filter_options, remote->name);
	திரும்பு;
}

நிலையான முழு fetch_one(கட்டமைப்பு remote *remote, முழு argc, மாறிலி எழுத்து **argv,
		     முழு prune_tags_ok, முழு use_stdin_refspecs,
		     மாறிலி கட்டமைப்பு fetch_config *config,
		     கட்டமைப்பு list_objects_filter_options *filter_options,
		     கட்டமைப்பு oidset *updated_tips)
{
	கட்டமைப்பு refspec rs = REFSPEC_INIT_FETCH(the_hash_algo);
	முழு i;
	முழு exit_code;
	முழு maybe_prune_tags;
	முழு remote_via_config = remote_is_configured(remote, 0);

	எனில் (!remote)
		die(_("no remote repository specified; please specify either a URL or a\n"
		      "remote name from which new revisions should be fetched"));

	gtransport = prepare_transport(remote, 1, filter_options);

	எனில் (prune < 0) {
		/* no command line request */
		எனில் (0 <= remote->prune)
			prune = remote->prune;
		இல்லையெனில் எனில் (0 <= config->prune)
			prune = config->prune;
		இல்லையெனில்
			prune = PRUNE_BY_DEFAULT;
	}

	எனில் (prune_tags < 0) {
		/* no command line request */
		எனில் (0 <= remote->prune_tags)
			prune_tags = remote->prune_tags;
		இல்லையெனில் எனில் (0 <= config->prune_tags)
			prune_tags = config->prune_tags;
		இல்லையெனில்
			prune_tags = PRUNE_TAGS_BY_DEFAULT;
	}

	maybe_prune_tags = prune_tags_ok && prune_tags;
	எனில் (maybe_prune_tags && remote_via_config)
		refspec_append(&remote->fetch, TAG_REFSPEC);

	எனில் (maybe_prune_tags && (argc || !remote_via_config))
		refspec_append(&rs, TAG_REFSPEC);

	க்கு (i = 0; i < argc; i++) {
		எனில் (!strcmp(argv[i], "tag")) {
			i++;
			எனில் (i >= argc)
				die(_("you need to specify a tag name"));

			refspec_appendf(&rs, "refs/tags/%s:refs/tags/%s",
					argv[i], argv[i]);
		} இல்லையெனில் {
			refspec_append(&rs, argv[i]);
		}
	}

	எனில் (use_stdin_refspecs) {
		கட்டமைப்பு strbuf வரி = STRBUF_INIT;
		போது (strbuf_getline_lf(&வரி, stdin) != EOF)
			refspec_append(&rs, வரி.buf);
		strbuf_release(&வரி);
	}

	எனில் (server_options.nr)
		gtransport->server_options = &server_options;

	sigchain_தள்ளு_common(unlock_pack_on_signal);
	atexit(unlock_pack_atexit);
	sigchain_தள்ளு(SIGPIPE, SIG_IGN);
	exit_code = do_fetch(gtransport, &rs, config, filter_options,
			     updated_tips);
	sigchain_எடு(SIGPIPE);
	refspec_clear(&rs);
	transport_disconnect(gtransport);
	gtransport = வெற்று;
	திரும்பு exit_code;
}

முழு cmd_fetch(முழு argc,
	      மாறிலி எழுத்து **argv,
	      மாறிலி எழுத்து *prefix,
	      கட்டமைப்பு repository *repo UNUSED)
{
	கட்டமைப்பு fetch_config config = {
		.display_format = DISPLAY_FORMAT_FULL,
		.follow_remote_head_raw = வெற்று,
		.follow_remote_head_seen = 0,
		.prune = -1,
		.prune_tags = -1,
		.show_forced_updates = 1,
		.recurse_submodules = RECURSE_SUBMODULES_DEFAULT,
		.parallel = 1,
		.submodule_fetch_jobs = -1,
	};
	மாறிலி எழுத்து *submodule_prefix = "";
	மாறிலி எழுத்து *bundle_uri;
	கட்டமைப்பு string_list list = STRING_LIST_INIT_DUP;
	கட்டமைப்பு list_objects_filter_options filter_options = LIST_OBJECTS_FILTER_INIT;
	கட்டமைப்பு remote *remote = வெற்று;
	முழு all = -1, multiple = 0;
	முழு result = 0;
	முழு prune_tags_ok = 1;
	முழு enable_auto_gc = 1;
	முழு unshallow = 0;
	முழு max_jobs = -1;
	முழு recurse_submodules_cli = RECURSE_SUBMODULES_DEFAULT;
	முழு recurse_submodules_default = RECURSE_SUBMODULES_ON_DEMAND;
	முழு fetch_write_commit_graph = -1;
	முழு stdin_refspecs = 0;
	முழு negotiate_only = 0;
	முழு porcelain = 0;
	முழு i;
	எண்ணிக்கை {
		GRAPH_WRITE_REACHABLE,
		GRAPH_WRITE_TIPS,
		GRAPH_WRITE_SKIP,
	} graph_write_mode = GRAPH_WRITE_REACHABLE;
	கட்டமைப்பு oidset updated_tips = OIDSET_INIT;

	கட்டமைப்பு option builtin_fetch_options[] = {
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
		OPT_SET_INT('n', வெற்று, &tags,
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
			.மதிப்பு = &submodule_prefix,
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
	க்கு (i = 1; i < argc; i++) {
		/* This handles non-URLs gracefully */
		எழுத்து *anon = transport_anonymize_url(argv[i]);

		strbuf_addf(&default_rla, " %s", anon);
		விடுவி(anon);
	}

	repo_config(the_repository, git_fetch_config, &config);
	எனில் (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_full_index = 0;
	}

	argc = parse_options(argc, argv, prefix,
			     builtin_fetch_options, builtin_fetch_usage, 0);

	எனில் (recurse_submodules_cli != RECURSE_SUBMODULES_DEFAULT)
		config.recurse_submodules = recurse_submodules_cli;

	எனில் (negotiate_only) {
		தேர்வு (recurse_submodules_cli) {
		நிலை RECURSE_SUBMODULES_OFF:
		நிலை RECURSE_SUBMODULES_DEFAULT:
			/*
			 * --negotiate-only should never recurse into
			 * submodules. Skip it by setting recurse_submodules to
			 * RECURSE_SUBMODULES_OFF.
			 */
			config.recurse_submodules = RECURSE_SUBMODULES_OFF;
			நிறுத்து;

		இயல்பு:
			die(_("options '%s' and '%s' cannot be used together"),
			    "--negotiate-only", "--recurse-submodules");
		}
	}

	எனில் (config.recurse_submodules != RECURSE_SUBMODULES_OFF) {
		முழு *sfjc = config.submodule_fetch_jobs == -1
			    ? &config.submodule_fetch_jobs : வெற்று;
		முழு *rs = config.recurse_submodules == RECURSE_SUBMODULES_DEFAULT
			  ? &config.recurse_submodules : வெற்று;

		fetch_config_from_gitmodules(the_repository, sfjc, rs);
	}


	எனில் (porcelain) {
		தேர்வு (recurse_submodules_cli) {
		நிலை RECURSE_SUBMODULES_OFF:
		நிலை RECURSE_SUBMODULES_DEFAULT:
			/*
			 * Reference updates in submodules would be ambiguous
			 * in porcelain mode, so we reject this combination.
			 */
			config.recurse_submodules = RECURSE_SUBMODULES_OFF;
			நிறுத்து;

		இயல்பு:
			die(_("options '%s' and '%s' cannot be used together"),
			    "--porcelain", "--recurse-submodules");
		}

		config.display_format = DISPLAY_FORMAT_PORCELAIN;
	}

	எனில் (deepen_relative) {
		எனில் (deepen_relative < 0)
			die(_("negative depth in --deepen is not supported"));
		எனில் (depth)
			die(_("options '%s' and '%s' cannot be used together"), "--deepen", "--depth");
		depth = xstrfmt("%d", deepen_relative);
	}
	எனில் (unshallow) {
		எனில் (depth)
			die(_("options '%s' and '%s' cannot be used together"), "--depth", "--unshallow");
		இல்லையெனில் எனில் (!is_repository_shallow(the_repository))
			die(_("--unshallow on a complete repository does not make sense"));
		இல்லையெனில்
			depth = xstrfmt("%d", INFINITE_DEPTH);
	}

	/* no need to be strict, transport_set_option() will validate it again */
	எனில் (depth && atoi(depth) < 1)
		die(_("depth %s is not a positive number"), depth);
	எனில் (depth || deepen_since || deepen_not.nr)
		deepen = 1;

	/* FETCH_HEAD never gets updated in --dry-run mode */
	எனில் (dry_run)
		write_fetch_head = 0;

	எனில் (!max_jobs)
		max_jobs = online_cpus();

	எனில் (!repo_config_get_string_tmp(the_repository, "fetch.bundleuri", &bundle_uri) &&
	    fetch_bundle_uri(the_repository, bundle_uri, வெற்று))
		warning(_("failed to fetch bundles from '%s'"), bundle_uri);

	எனில் (all < 0) {
		/*
		 * no --[no-]all given;
		 * only use config option if no remote was explicitly specified
		 */
		all = (!argc) ? config.all : 0;
	}

	எனில் (all) {
		எனில் (argc == 1)
			die(_("fetch --all does not take a repository argument"));
		இல்லையெனில் எனில் (argc > 1)
			die(_("fetch --all does not make sense with refspecs"));

		(வெறுமை) for_each_remote(get_one_remote_for_fetch, &list);

		/* do not do fetch_multiple() of one */
		எனில் (list.nr == 1)
			remote = remote_get(list.பொருட்கள்[0].string);
	} இல்லையெனில் எனில் (argc == 0) {
		/* No arguments -- use default remote */
		remote = remote_get(வெற்று);
	} இல்லையெனில் எனில் (multiple) {
		/* All arguments are assumed to be remotes or groups */
		க்கு (i = 0; i < argc; i++)
			எனில் (!add_remote_or_group(argv[i], &list))
				die(_("no such remote or remote group: %s"),
				    argv[i]);
	} இல்லையெனில் {
		/* Single remote or group */
		(வெறுமை) add_remote_or_group(argv[0], &list);
		எனில் (list.nr > 1) {
			/* More than one remote */
			எனில் (argc > 1)
				die(_("fetching a group and specifying refspecs does not make sense"));
		} இல்லையெனில் {
			/* Zero or one remotes */
			remote = remote_get(argv[0]);
			prune_tags_ok = (argc == 1);
			argc--;
			argv++;
		}
	}
	string_list_remove_duplicates(&list, 0);

	எனில் (negotiate_only) {
		கட்டமைப்பு oidset acked_commits = OIDSET_INIT;
		கட்டமைப்பு oidset_iter iter;
		மாறிலி கட்டமைப்பு object_id *oid;

		trace2_region_enter("fetch", "negotiate-only", the_repository);
		எனில் (!remote)
			die(_("must supply remote when using --negotiate-only"));
		gtransport = prepare_transport(remote, 1, &filter_options);

		எனில் (!gtransport->smart_options) {
			warning(_("protocol does not support --negotiate-only, exiting"));
			result = 1;
			trace2_region_leave("fetch", "negotiate-only", the_repository);
			செல் cleanup;
		}
		எனில் (!gtransport->smart_options->negotiation_restrict_tips)
			die(_("%s needs one or more %s"), "--negotiate-only",
			    "--negotiation-restrict=*");

		gtransport->smart_options->acked_commits = &acked_commits;

		எனில் (server_options.nr)
			gtransport->server_options = &server_options;
		result = transport_fetch_refs(gtransport, வெற்று);
		gtransport->smart_options->acked_commits = வெற்று;

		oidset_iter_init(&acked_commits, &iter);
		போது ((oid = oidset_iter_next(&iter)))
			printf("%s\n", oid_to_hex(oid));
		oidset_clear(&acked_commits);
		trace2_region_leave("fetch", "negotiate-only", the_repository);
	} இல்லையெனில் எனில் (remote) {
		எனில் (filter_options.choice || repo_has_promisor_remote(the_repository)) {
			trace2_region_enter("fetch", "setup-partial", the_repository);
			fetch_one_setup_partial(remote, &filter_options);
			trace2_region_leave("fetch", "setup-partial", the_repository);
		}
		trace2_region_enter("fetch", "fetch-one", the_repository);
		result = fetch_one(remote, argc, argv, prune_tags_ok, stdin_refspecs,
				   &config, &filter_options, &updated_tips);
		எனில் (prepare_commit_graph(the_repository)) {
			எனில் (oidset_size(&updated_tips))
				graph_write_mode = GRAPH_WRITE_TIPS;
			இல்லையெனில்
				graph_write_mode = GRAPH_WRITE_SKIP;
		}
		trace2_region_leave("fetch", "fetch-one", the_repository);
	} இல்லையெனில் {
		முழு max_children = max_jobs;

		எனில் (filter_options.choice)
			die(_("--filter can only be used with the remote "
			      "configured in extensions.partialclone"));

		எனில் (atomic_fetch)
			die(_("--atomic can only be used when fetching "
			      "from one remote"));

		எனில் (stdin_refspecs)
			die(_("--stdin can only be used when fetching "
			      "from one remote"));

		எனில் (max_children < 0)
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
	எனில் (!result && remote && (config.recurse_submodules != RECURSE_SUBMODULES_OFF)) {
		கட்டமைப்பு strvec options = STRVEC_INIT;
		முழு max_children = max_jobs;

		எனில் (max_children < 0)
			max_children = config.submodule_fetch_jobs;
		எனில் (max_children < 0)
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
	எனில் (negotiate_only)
		செல் cleanup;

	prepare_repo_settings(the_repository);
	எனில் (fetch_write_commit_graph > 0 ||
	    (fetch_write_commit_graph < 0 &&
	     the_repository->settings.fetch_write_commit_graph)) {
		முழு commit_graph_flags = COMMIT_GRAPH_WRITE_SPLIT;

		எனில் (progress)
			commit_graph_flags |= COMMIT_GRAPH_WRITE_PROGRESS;

		எனில் (graph_write_mode != GRAPH_WRITE_SKIP) {
			trace2_region_enter("fetch", "write-commit-graph",
					    the_repository);
			எனில் (graph_write_mode == GRAPH_WRITE_TIPS)
				write_commit_graph(
					the_repository,
					the_repository->objects->source->path,
					வெற்று, &updated_tips,
					commit_graph_flags, வெற்று);
			இல்லையெனில்
				write_commit_graph_reachable(
					the_repository,
					the_repository->objects->source->path,
					commit_graph_flags, வெற்று);
			trace2_region_leave("fetch", "write-commit-graph",
					    the_repository);
		}
	}

	எனில் (enable_auto_gc) {
		எனில் (refetch) {
			/*
			 * Hint auto-maintenance strongly to encourage repacking,
			 * but respect config settings disabling it.
			 */
			முழு opt_val;
			எனில் (repo_config_get_int(the_repository, "gc.autopacklimit", &opt_val))
				opt_val = -1;
			எனில் (opt_val != 0)
				git_config_தள்ளு_parameter("gc.autoPackLimit=1");

			எனில் (repo_config_get_int(the_repository, "maintenance.incremental-repack.auto", &opt_val))
				opt_val = -1;
			எனில் (opt_val != 0)
				git_config_தள்ளு_parameter("maintenance.incremental-repack.auto=-1");
		}
		run_auto_maintenance(the_repository, verbosity < 0);
	}

 cleanup:
	oidset_clear(&updated_tips);
	string_list_clear(&list, 0);
	list_objects_filter_release(&filter_options);
	விடுவி(config.follow_remote_head_raw);
	திரும்பு result;
}
