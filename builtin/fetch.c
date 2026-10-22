/*
 * "git fetch"
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#شامل "builtin.h"
#شامل "advice.h"
#شامل "config.h"
#شامل "gettext.h"
#شامل "environment.h"
#شامل "hex.h"
#شامل "refs.h"
#شامل "refspec.h"
#شامل "object-name.h"
#شامل "odb.h"
#شامل "odb/source.h"
#شامل "oidset.h"
#شامل "oid-array.h"
#شامل "commit.h"
#شامل "string-list.h"
#شامل "remote.h"
#شامل "transport.h"
#شامل "run-command.h"
#شامل "parse-options.h"
#شامل "sigchain.h"
#شامل "submodule-config.h"
#شامل "submodule.h"
#شامل "connected.h"
#شامل "strvec.h"
#شامل "utf8.h"
#شامل "pager.h"
#شامل "path.h"
#شامل "pkt-line.h"
#شامل "list-objects-filter-options.h"
#شامل "commit-reach.h"
#شامل "branch.h"
#شامل "promisor-remote.h"
#شامل "commit-graph.h"
#شامل "shallow.h"
#شامل "trace.h"
#شامل "trace2.h"
#شامل "bundle-uri.h"

#define FORCED_UPDATES_DELAY_WARNING_IN_MS (10 * 1000)

ایستا ثابت نویسه * ثابت builtin_fetch_usage[] = {
	N_("git fetch [<options>] [<repository> [<refspec>...]]"),
	N_("git fetch [<options>] <group>"),
	N_("git fetch --multiple [<options>] [(<repository>|<group>)...]"),
	N_("git fetch --all [<options>]"),
	پوچ
};

شمارش {
	TAGS_UNSET = 0,
	TAGS_DEFAULT = 1,
	TAGS_SET = 2
};

شمارش display_format {
	DISPLAY_FORMAT_FULL,
	DISPLAY_FORMAT_COMPACT,
	DISPLAY_FORMAT_PORCELAIN,
};

ساختار display_state {
	ساختار strbuf buf;

	صحیح refcol_width;
	شمارش display_format format;

	نویسه *url;
	صحیح url_len, shown_url;
};

ایستا uint64_t forced_updates_ms = 0;
ایستا صحیح prefetch = 0;
ایستا صحیح prune = -1; /* unspecified */
#define PRUNE_BY_DEFAULT 0 /* do we prune by default? */

ایستا صحیح prune_tags = -1; /* unspecified */
#define PRUNE_TAGS_BY_DEFAULT 0 /* do we prune tags by default? */

ایستا صحیح append, dry_run, force, keep, update_head_ok;
ایستا صحیح write_fetch_head = 1;
ایستا صحیح verbosity, deepen_relative, set_upstream, refetch;
ایستا صحیح progress = -1;
ایستا صحیح tags = TAGS_DEFAULT, update_shallow, deepen;
ایستا صحیح atomic_fetch;
ایستا شمارش transport_family family;
ایستا ثابت نویسه *depth;
ایستا ثابت نویسه *deepen_since;
ایستا ثابت نویسه *upload_pack;
ایستا ساختار string_list deepen_not = STRING_LIST_INIT_NODUP;
ایستا ساختار strbuf default_rla = STRBUF_INIT;
ایستا ساختار transport *gtransport;
ایستا ساختار transport *gsecondary;
ایستا ساختار refspec refmap;
ایستا ساختار string_list server_options = STRING_LIST_INIT_DUP;
ایستا ساختار string_list negotiation_restrict = STRING_LIST_INIT_NODUP;
ایستا ساختار string_list negotiation_include = STRING_LIST_INIT_NODUP;

ساختار fetch_config {
	شمارش display_format display_format;
	نویسه *follow_remote_head_raw;
	صحیح follow_remote_head_seen;
	صحیح all;
	صحیح prune;
	صحیح prune_tags;
	صحیح show_forced_updates;
	صحیح recurse_submodules;
	صحیح parallel;
	صحیح submodule_fetch_jobs;
};

ایستا صحیح git_fetch_config(ثابت نویسه *k, ثابت نویسه *v,
			    ثابت ساختار config_context *ctx, تهی *cb)
{
	ساختار fetch_config *fetch_config = cb;

	اگر (!strcmp(k, "fetch.all")) {
		fetch_config->all = git_config_bool(k, v);
		بازگشت 0;
	}

	اگر (!strcmp(k, "fetch.prune")) {
		fetch_config->prune = git_config_bool(k, v);
		بازگشت 0;
	}

	اگر (!strcmp(k, "fetch.prunetags")) {
		fetch_config->prune_tags = git_config_bool(k, v);
		بازگشت 0;
	}

	اگر (!strcmp(k, "fetch.showforcedupdates")) {
		fetch_config->show_forced_updates = git_config_bool(k, v);
		بازگشت 0;
	}

	اگر (!strcmp(k, "submodule.recurse")) {
		صحیح r = git_config_bool(k, v) ?
			RECURSE_SUBMODULES_ON : RECURSE_SUBMODULES_OFF;
		fetch_config->recurse_submodules = r;
		بازگشت 0;
	}

	اگر (!strcmp(k, "submodule.fetchjobs")) {
		fetch_config->submodule_fetch_jobs = parse_submodule_fetchjobs(k, v, ctx->kvi);
		بازگشت 0;
	} وگرنه اگر (!strcmp(k, "fetch.recursesubmodules")) {
		fetch_config->recurse_submodules = parse_fetch_recurse_submodules_arg(k, v);
		بازگشت 0;
	}

	اگر (!strcmp(k, "fetch.parallel")) {
		fetch_config->parallel = git_config_int(k, v, ctx->kvi);
		اگر (fetch_config->parallel < 0)
			die(_("fetch.parallel cannot be negative"));
		اگر (!fetch_config->parallel)
			fetch_config->parallel = online_cpus();
		بازگشت 0;
	}

	اگر (!strcmp(k, "fetch.output")) {
		اگر (!v)
			بازگشت config_error_nonbool(k);
		وگرنه اگر (!strcasecmp(v, "full"))
			fetch_config->display_format = DISPLAY_FORMAT_FULL;
		وگرنه اگر (!strcasecmp(v, "compact"))
			fetch_config->display_format = DISPLAY_FORMAT_COMPACT;
		وگرنه
			die(_("invalid value for '%s': '%s'"),
			    "fetch.output", v);
		بازگشت 0;
	}

	اگر (!strcmp(k, "fetch.followremotehead")) {
		آزاد(fetch_config->follow_remote_head_raw);
		fetch_config->follow_remote_head_raw = xstrdup_or_null(v);
		fetch_config->follow_remote_head_seen = 1;
		بازگشت 0;
	}

	بازگشت git_default_config(k, v, ctx, cb);
}

ایستا شمارش follow_remote_head_settings get_follow_remote_head(ثابت نویسه *setting)
{
	اگر (!setting)
		die(_("missing value for 'fetch.followRemoteHEAD'"));
	وگرنه اگر (!strcmp(setting, "never"))
		بازگشت FOLLOW_REMOTE_NEVER;
	وگرنه اگر (!strcmp(setting, "create"))
		بازگشت FOLLOW_REMOTE_CREATE;
	وگرنه اگر (!strcmp(setting, "warn"))
		بازگشت FOLLOW_REMOTE_WARN;
	وگرنه اگر (!strcmp(setting, "always"))
		بازگشت FOLLOW_REMOTE_ALWAYS;
	warning(_("unrecognized fetch.followRemoteHEAD value '%s' ignored"), setting);
	بازگشت BUILTIN_FOLLOW_REMOTE_HEAD_DFLT;
}

ایستا صحیح parse_refmap_arg(ثابت ساختار option *opt, ثابت نویسه *arg, صحیح unset)
{
	BUG_ON_OPT_NEG(unset);

	/*
	 * "git fetch --refmap='' origin foo"
	 * can be used to tell the command not to store anywhere
	 */
	refspec_append(opt->مقدار, arg);

	بازگشت 0;
}

ایستا تهی unlock_pack(unsigned صحیح flags)
{
	اگر (gtransport)
		transport_unlock_pack(gtransport, flags);
	اگر (gsecondary)
		transport_unlock_pack(gsecondary, flags);
}

ایستا تهی unlock_pack_atexit(تهی)
{
	unlock_pack(0);
}

ایستا تهی unlock_pack_on_signal(صحیح signo)
{
	unlock_pack(TRANSPORT_UNLOCK_PACK_IN_SIGNAL_HANDLER);
	sigchain_برداشت(signo);
	بالاببر(signo);
}

ایستا تهی add_merge_config(ساختار ref **head,
			   ثابت ساختار ref *remote_refs,
		           ساختار branch *branch,
		           ساختار ref ***tail)
{
	صحیح i;

	برای (i = 0; i < branch->merge_nr; i++) {
		ساختار ref *rm, **old_tail = *tail;
		ساختار refspec_item refspec;

		برای (rm = *head; rm; rm = rm->next) {
			اگر (branch_merge_matches(branch, i, rm->name)) {
				rm->fetch_head_status = FETCH_HEAD_MERGE;
				بشکن;
			}
		}
		اگر (rm)
			ادامه;

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
		memset(&refspec, 0, اندازه(refspec));
		refspec.src = branch->merge[i]->src;
		get_fetch_map(remote_refs, &refspec, tail, 1);
		برای (rm = *old_tail; rm; rm = rm->next)
			rm->fetch_head_status = FETCH_HEAD_MERGE;
	}
}

ایستا تهی create_fetch_oidset(ساختار ref **head, ساختار oidset *out)
{
	ساختار ref *rm = *head;
	وقتی (rm) {
		oidset_درج(out, &rm->old_oid);
		rm = rm->next;
	}
}

ساختار refname_hash_entry {
	ساختار hashmap_entry ent;
	ساختار object_id oid;
	صحیح ignore;
	نویسه refname[FLEX_ARRAY];
};

ایستا صحیح refname_hash_entry_cmp(ثابت تهی *hashmap_cmp_fn_data UNUSED,
				  ثابت ساختار hashmap_entry *eptr,
				  ثابت ساختار hashmap_entry *entry_or_کلید,
				  ثابت تهی *keydata)
{
	ثابت ساختار refname_hash_entry *e1, *e2;

	e1 = container_of(eptr, ثابت ساختار refname_hash_entry, ent);
	e2 = container_of(entry_or_کلید, ثابت ساختار refname_hash_entry, ent);
	بازگشت strcmp(e1->refname, keydata ? keydata : e2->refname);
}

ایستا ساختار refname_hash_entry *refname_hash_add(ساختار hashmap *map,
						   ثابت نویسه *refname,
						   ثابت ساختار object_id *oid)
{
	ساختار refname_hash_entry *ent;
	size_t len = strlen(refname);

	FLEX_ALLOC_MEM(ent, refname, refname, len);
	hashmap_entry_init(&ent->ent, strhash(refname));
	oidcpy(&ent->oid, oid);
	hashmap_add(map, &ent->ent);
	بازگشت ent;
}

ایستا صحیح add_one_refname(ثابت ساختار reference *ref, تهی *cbdata)
{
	ساختار hashmap *refname_map = cbdata;

	(تهی) refname_hash_add(refname_map, ref->name, ref->oid);
	بازگشت 0;
}

ایستا تهی refname_hash_init(ساختار hashmap *map)
{
	hashmap_init(map, refname_hash_entry_cmp, پوچ, 0);
}

ایستا صحیح refname_hash_exists(ساختار hashmap *map, ثابت نویسه *refname)
{
	بازگشت !!hashmap_get_from_hash(map, strhash(refname), refname);
}

ایستا تهی clear_item(ساختار refname_hash_entry *item)
{
	item->ignore = 1;
}


ایستا تهی add_already_queued_tags(ثابت نویسه *refname,
				    ثابت ساختار object_id *old_oid UNUSED,
				    ثابت ساختار object_id *new_oid,
				    تهی *cb_data)
{
	ساختار hashmap *queued_tags = cb_data;
	اگر (starts_with(refname, "refs/tags/") && new_oid)
		(تهی) refname_hash_add(queued_tags, refname, new_oid);
}

ایستا تهی find_non_local_tags(ثابت ساختار ref *refs,
				ساختار ref_transaction *transaction,
				ساختار ref **head,
				ساختار ref ***tail)
{
	ساختار hashmap existing_refs;
	ساختار hashmap remote_refs;
	ساختار oidset fetch_oids = OIDSET_INIT;
	ساختار string_list remote_refs_list = STRING_LIST_INIT_NODUP;
	ساختار string_list_item *remote_ref_item;
	ثابت ساختار ref *ref;
	ساختار refname_hash_entry *item = پوچ;

	refname_hash_init(&existing_refs);
	refname_hash_init(&remote_refs);
	create_fetch_oidset(head, &fetch_oids);

	refs_for_each_ref(get_main_ref_store(the_repository), add_one_refname,
			  &existing_refs);

	/*
	 * If we already have a transaction, then we need to filter out all
	 * tags which have already been queued up.
	 */
	اگر (transaction)
		ref_transaction_for_each_queued_update(transaction,
						       add_already_queued_tags,
						       &existing_refs);

	برای (ref = refs; ref; ref = ref->next) {
		اگر (!starts_with(ref->name, "refs/tags/"))
			ادامه;

		/*
		 * The peeled ref always follows the matching base
		 * ref, so if we see a peeled ref that we don't want
		 * to fetch then we can mark the ref entry in the list
		 * as one to ignore by setting util to NULL.
		 */
		اگر (ends_with(ref->name, "^{}")) {
			اگر (item &&
			    !odb_has_object(the_repository->objects, &ref->old_oid, 0) &&
			    !oidset_contains(&fetch_oids, &ref->old_oid) &&
			    !odb_has_object(the_repository->objects, &item->oid, 0) &&
			    !oidset_contains(&fetch_oids, &item->oid))
				clear_item(item);
			item = پوچ;
			ادامه;
		}

		/*
		 * If item is non-NULL here, then we previously saw a
		 * ref not followed by a peeled reference, so we need
		 * to check if it is a lightweight tag that we want to
		 * fetch.
		 */
		اگر (item &&
		    !odb_has_object(the_repository->objects, &item->oid, 0) &&
		    !oidset_contains(&fetch_oids, &item->oid))
			clear_item(item);

		item = پوچ;

		/* skip duplicates and refs that we already have */
		اگر (refname_hash_exists(&remote_refs, ref->name) ||
		    refname_hash_exists(&existing_refs, ref->name))
			ادامه;

		item = refname_hash_add(&remote_refs, ref->name, &ref->old_oid);
		string_list_درج(&remote_refs_list, ref->name);
	}
	hashmap_clear_and_free(&existing_refs, ساختار refname_hash_entry, ent);

	/*
	 * We may have a final lightweight tag that needs to be
	 * checked to see if it needs fetching.
	 */
	اگر (item &&
	    !odb_has_object(the_repository->objects, &item->oid, 0) &&
	    !oidset_contains(&fetch_oids, &item->oid))
		clear_item(item);

	/*
	 * For all the tags in the remote_refs_list,
	 * add them to the list of refs to be fetched
	 */
	for_each_string_list_item(remote_ref_item, &remote_refs_list) {
		ثابت نویسه *refname = remote_ref_item->string;
		ساختار ref *rm;
		unsigned صحیح hash = strhash(refname);

		item = hashmap_get_entry_from_hash(&remote_refs, hash, refname,
					ساختار refname_hash_entry, ent);
		اگر (!item)
			BUG("unseen remote ref?");

		/* Unless we have already decided to ignore this item... */
		اگر (item->ignore)
			ادامه;

		rm = alloc_ref(item->refname);
		rm->peer_ref = alloc_ref(item->refname);
		oidcpy(&rm->old_oid, &item->oid);
		**tail = rm;
		*tail = &rm->next;
	}
	hashmap_clear_and_free(&remote_refs, ساختار refname_hash_entry, ent);
	string_list_clear(&remote_refs_list, 0);
	oidset_clear(&fetch_oids);
}

ایستا تهی filter_prefetch_refspec(ساختار refspec *rs)
{
	صحیح i;

	اگر (!prefetch)
		بازگشت;

	برای (i = 0; i < rs->nr; i++) {
		ساختار strbuf new_dst = STRBUF_INIT;
		نویسه *old_dst;
		ثابت نویسه *sub = پوچ;

		اگر (rs->اقلام[i].negative)
			ادامه;
		اگر (!rs->اقلام[i].dst ||
		    (rs->اقلام[i].src &&
		     starts_with(rs->اقلام[i].src,
				 ref_namespace[NAMESPACE_TAGS].ref))) {
			صحیح j;

			refspec_item_clear(&rs->اقلام[i]);

			برای (j = i + 1; j < rs->nr; j++)
				rs->اقلام[j - 1] = rs->اقلام[j];
			rs->nr--;
			i--;
			ادامه;
		}

		old_dst = rs->اقلام[i].dst;
		strbuf_addstr(&new_dst, ref_namespace[NAMESPACE_PREFETCH].ref);

		/*
		 * If old_dst starts with "refs/", then place
		 * sub after that prefix. Otherwise, start at
		 * the beginning of the string.
		 */
		اگر (!skip_prefix(old_dst, "refs/", &sub))
			sub = old_dst;
		strbuf_addstr(&new_dst, sub);

		rs->اقلام[i].dst = strbuf_detach(&new_dst, پوچ);
		rs->اقلام[i].force = 1;

		آزاد(old_dst);
	}
}

ایستا ساختار ref *get_ref_map(ساختار remote *remote,
			       ثابت ساختار ref *remote_refs,
			       ساختار refspec *rs,
			       صحیح tags, صحیح *autotags)
{
	صحیح i;
	ساختار ref *rm;
	ساختار ref *ref_map = پوچ;
	ساختار ref **tail = &ref_map;

	/* opportunistically-updated references: */
	ساختار ref *orefs = پوچ, **oref_tail = &orefs;

	ساختار hashmap existing_refs;
	صحیح existing_refs_populated = 0;

	filter_prefetch_refspec(rs);
	اگر (remote)
		filter_prefetch_refspec(&remote->fetch);

	اگر (rs->nr) {
		ساختار refspec *fetch_refspec;

		برای (i = 0; i < rs->nr; i++) {
			get_fetch_map(remote_refs, &rs->اقلام[i], &tail, 0);
			اگر (rs->اقلام[i].dst && rs->اقلام[i].dst[0])
				*autotags = 1;
		}
		/* Merge everything on the command line (but not --tags) */
		برای (rm = ref_map; rm; rm = rm->next)
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
		اگر (refmap.nr)
			fetch_refspec = &refmap;
		وگرنه
			fetch_refspec = &remote->fetch;

		برای (i = 0; i < fetch_refspec->nr; i++)
			get_fetch_map(ref_map, &fetch_refspec->اقلام[i], &oref_tail, 1);
	} وگرنه اگر (refmap.nr) {
		die("--refmap option is only meaningful with command-line refspec(s)");
	} وگرنه {
		/* Use the defaults */
		ساختار branch *branch = branch_get(پوچ);
		صحیح has_merge = branch_has_merge_config(branch);
		اگر (remote &&
		    (remote->fetch.nr ||
		     /* Note: has_merge implies non-NULL branch->remote_name */
		     (has_merge && !strcmp(branch->remote_name, remote->name)))) {
			برای (i = 0; i < remote->fetch.nr; i++) {
				get_fetch_map(remote_refs, &remote->fetch.اقلام[i], &tail, 0);
				اگر (remote->fetch.اقلام[i].dst &&
				    remote->fetch.اقلام[i].dst[0])
					*autotags = 1;
				اگر (!i && !has_merge && ref_map &&
				    !remote->fetch.اقلام[0].pattern)
					ref_map->fetch_head_status = FETCH_HEAD_MERGE;
			}
			/*
			 * if the remote we're fetching from is the same
			 * as given in branch.<name>.remote, we add the
			 * ref given in branch.<name>.merge, too.
			 *
			 * Note: has_merge implies non-NULL branch->remote_name
			 */
			اگر (has_merge &&
			    !strcmp(branch->remote_name, remote->name))
				add_merge_config(&ref_map, remote_refs, branch, &tail);
		} وگرنه اگر (!prefetch) {
			ref_map = get_remote_ref(remote_refs, "HEAD");
			اگر (!ref_map)
				die(_("couldn't find remote ref HEAD"));
			ref_map->fetch_head_status = FETCH_HEAD_MERGE;
			tail = &ref_map->next;
		}
	}

	اگر (tags == TAGS_SET) {
		ساختار refspec_item tag_refspec;

		/* also fetch all tags */
		refspec_item_init_افزودن(&tag_refspec, TAG_REFSPEC,
				       the_hash_algo);
		get_fetch_map(remote_refs, &tag_refspec, &tail, 0);
		refspec_item_clear(&tag_refspec);
	} وگرنه اگر (tags == TAGS_DEFAULT && *autotags) {
		find_non_local_tags(remote_refs, پوچ, &ref_map, &tail);
	}

	/* Now append any refs to be updated opportunistically: */
	*tail = orefs;
	برای (rm = orefs; rm; rm = rm->next) {
		rm->fetch_head_status = FETCH_HEAD_IGNORE;
		tail = &rm->next;
	}

	/*
	 * apply negative refspecs first, before we remove duplicates. This is
	 * necessary as negative refspecs might remove an otherwise conflicting
	 * duplicate.
	 */
	اگر (rs->nr)
		ref_map = apply_negative_refspecs(ref_map, rs);
	وگرنه
		ref_map = apply_negative_refspecs(ref_map, &remote->fetch);

	ref_map = ref_remove_duplicates(ref_map);

	برای (rm = ref_map; rm; rm = rm->next) {
		اگر (rm->peer_ref) {
			ثابت نویسه *refname = rm->peer_ref->name;
			ساختار refname_hash_entry *peer_item;
			unsigned صحیح hash = strhash(refname);

			اگر (!existing_refs_populated) {
				refname_hash_init(&existing_refs);
				refs_for_each_ref(get_main_ref_store(the_repository),
						  add_one_refname,
						  &existing_refs);
				existing_refs_populated = 1;
			}

			peer_item = hashmap_get_entry_from_hash(&existing_refs,
						hash, refname,
						ساختار refname_hash_entry, ent);
			اگر (peer_item) {
				ساختار object_id *old_oid = &peer_item->oid;
				oidcpy(&rm->peer_ref->old_oid, old_oid);
			}
		}
	}
	اگر (existing_refs_populated)
		hashmap_clear_and_free(&existing_refs, ساختار refname_hash_entry, ent);

	بازگشت ref_map;
}

ایستا صحیح s_update_ref(ثابت نویسه *action,
			ساختار ref *ref,
			ساختار ref_transaction *transaction,
			صحیح check_old)
{
	نویسه *پیام;
	نویسه *rla = getenv("GIT_REFLOG_ACTION");
	ساختار strbuf err = STRBUF_INIT;
	صحیح ret;

	اگر (dry_run)
		بازگشت 0;
	اگر (!rla)
		rla = default_rla.buf;
	پیام = xstrfmt("%s: %s", rla, action);

	ret = ref_transaction_update(transaction, ref->name, &ref->new_oid,
				     check_old ? &ref->old_oid : پوچ,
				     پوچ, پوچ, 0, پیام, &err);

	اگر (ret)
		error("%s", err.buf);
	strbuf_release(&err);
	آزاد(پیام);
	بازگشت ret;
}

ایستا صحیح refcol_width(ثابت ساختار ref *ref_map, صحیح compact_format)
{
	ثابت ساختار ref *ref;
	صحیح max, width = 10;

	max = term_columns();
	اگر (compact_format)
		max = max * 2 / 3;

	برای (ref = ref_map; ref; ref = ref->next) {
		صحیح rlen, llen = 0, len;

		اگر (ref->status == REF_STATUS_REJECT_SHALLOW ||
		    !ref->peer_ref ||
		    !strcmp(ref->name, "HEAD"))
			ادامه;

		/* uptodate lines are only shown on high verbosity level */
		اگر (verbosity <= 0 && oideq(&ref->peer_ref->old_oid, &ref->old_oid))
			ادامه;

		rlen = utf8_strwidth(prettify_refname(ref->name));
		اگر (!compact_format)
			llen = utf8_strwidth(prettify_refname(ref->peer_ref->name));

		/*
		 * rough estimation to see if the output line is too long and
		 * should not be counted (we can't do precise calculation
		 * anyway because we don't know if the error explanation part
		 * will be printed in update_local_ref)
		 */
		len = 21 /* flag and summary */ + rlen + 4 /* -> */ + llen;
		اگر (len >= max)
			ادامه;

		اگر (width < rlen)
			width = rlen;
	}

	بازگشت width;
}

ایستا تهی display_state_init(ساختار display_state *display_state, ساختار ref *ref_map,
			       ثابت نویسه *raw_url, شمارش display_format format)
{
	صحیح i;

	memset(display_state, 0, اندازه(*display_state));
	strbuf_init(&display_state->buf, 0);
	display_state->format = format;

	اگر (raw_url)
		display_state->url = transport_anonymize_url(raw_url);
	وگرنه
		display_state->url = xstrdup("foreign");

	display_state->url_len = strlen(display_state->url);
	برای (i = display_state->url_len - 1; 0 <= i && display_state->url[i] == '/'; i--)
		;
	display_state->url_len = i + 1;
	اگر (4 < i && !strncmp(".git", display_state->url + i - 3, 4))
		display_state->url_len = i - 3;

	اگر (verbosity < 0)
		بازگشت;

	انتخاب (display_state->format) {
	حالت DISPLAY_FORMAT_FULL:
	حالت DISPLAY_FORMAT_COMPACT:
		display_state->refcol_width = refcol_width(ref_map,
							   display_state->format == DISPLAY_FORMAT_COMPACT);
		بشکن;
	حالت DISPLAY_FORMAT_PORCELAIN:
		/* We don't need to precompute anything here. */
		بشکن;
	پیشفرض:
		BUG("unexpected display format %d", display_state->format);
	}
}

ایستا تهی display_state_release(ساختار display_state *display_state)
{
	strbuf_release(&display_state->buf);
	آزاد(display_state->url);
}

ایستا تهی print_remote_to_local(ساختار display_state *display_state,
				  ثابت نویسه *remote, ثابت نویسه *local)
{
	strbuf_addf(&display_state->buf, "%-*s -> %s",
		    display_state->refcol_width, remote, local);
}

ایستا صحیح find_and_replace(ساختار strbuf *haystack,
			    ثابت نویسه *needle,
			    ثابت نویسه *placeholder)
{
	ثابت نویسه *p = پوچ;
	صحیح plen, nlen;

	nlen = strlen(needle);
	اگر (ends_with(haystack->buf, needle))
		p = haystack->buf + haystack->len - nlen;
	وگرنه
		p = strstr(haystack->buf, needle);
	اگر (!p)
		بازگشت 0;

	اگر (p > haystack->buf && p[-1] != '/')
		بازگشت 0;

	plen = strlen(p);
	اگر (plen > nlen && p[nlen] != '/')
		بازگشت 0;

	strbuf_splice(haystack, p - haystack->buf, nlen,
		      placeholder, strlen(placeholder));
	بازگشت 1;
}

ایستا تهی print_compact(ساختار display_state *display_state,
			  ثابت نویسه *remote, ثابت نویسه *local)
{
	ساختار strbuf r = STRBUF_INIT;
	ساختار strbuf l = STRBUF_INIT;

	اگر (!strcmp(remote, local)) {
		strbuf_addf(&display_state->buf, "%-*s -> *", display_state->refcol_width, remote);
		بازگشت;
	}

	strbuf_addstr(&r, remote);
	strbuf_addstr(&l, local);

	اگر (!find_and_replace(&r, local, "*"))
		find_and_replace(&l, remote, "*");
	print_remote_to_local(display_state, r.buf, l.buf);

	strbuf_release(&r);
	strbuf_release(&l);
}

ایستا تهی display_ref_update(ساختار display_state *display_state, نویسه code,
			       ثابت نویسه *summary, ثابت نویسه *error,
			       ثابت نویسه *remote, ثابت نویسه *local,
			       ثابت ساختار object_id *old_oid,
			       ثابت ساختار object_id *new_oid,
			       صحیح summary_width)
{
	FILE *f = stderr;

	اگر (verbosity < 0)
		بازگشت;

	strbuf_reset(&display_state->buf);

	انتخاب (display_state->format) {
	حالت DISPLAY_FORMAT_FULL:
	حالت DISPLAY_FORMAT_COMPACT: {
		صحیح width;

		اگر (!display_state->shown_url) {
			strbuf_addf(&display_state->buf, _("From %.*s\n"),
				    display_state->url_len, display_state->url);
			display_state->shown_url = 1;
		}

		width = (summary_width + strlen(summary) - gettext_width(summary));
		remote = prettify_refname(remote);
		local = prettify_refname(local);

		strbuf_addf(&display_state->buf, " %c %-*s ", code, width, summary);

		اگر (display_state->format != DISPLAY_FORMAT_COMPACT)
			print_remote_to_local(display_state, remote, local);
		وگرنه
			print_compact(display_state, remote, local);

		اگر (error)
			strbuf_addf(&display_state->buf, "  (%s)", error);

		بشکن;
	}
	حالت DISPLAY_FORMAT_PORCELAIN:
		strbuf_addf(&display_state->buf, "%c %s %s %s", code,
			    oid_to_hex(old_oid), oid_to_hex(new_oid), local);
		f = stdout;
		بشکن;
	پیشفرض:
		BUG("unexpected display format %d", display_state->format);
	};
	strbuf_addch(&display_state->buf, '\n');

	fputs(display_state->buf.buf, f);
}

ساختار ref_update_display_info {
	bool failed;
	نویسه success_code;
	نویسه fail_code;
	نویسه *summary;
	نویسه *fail_detail;
	نویسه *success_detail;
	نویسه *ref;
	نویسه *remote;
	ساختار object_id old_oid;
	ساختار object_id new_oid;
};

ساختار ref_update_display_info_array {
	ساختار ref_update_display_info *info;
	size_t alloc, nr;
};

ایستا ساختار ref_update_display_info *ref_update_display_info_append(
					   ساختار ref_update_display_info_array *array,
					   نویسه success_code,
					   نویسه fail_code,
					   ثابت نویسه *summary,
					   ثابت نویسه *success_detail,
					   ثابت نویسه *fail_detail,
					   ثابت نویسه *ref,
					   ثابت نویسه *remote,
					   ثابت ساختار object_id *old_oid,
					   ثابت ساختار object_id *new_oid)
{
	ساختار ref_update_display_info *info;

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

	بازگشت info;
}

ایستا تهی ref_update_display_info_set_failed(ساختار ref_update_display_info *info)
{
	info->failed = true;
}

ایستا تهی ref_update_display_info_free(ساختار ref_update_display_info *info)
{
	آزاد(info->summary);
	آزاد(info->success_detail);
	آزاد(info->fail_detail);
	آزاد(info->remote);
	آزاد(info->ref);
}

ایستا تهی ref_update_display_info_display(ساختار ref_update_display_info *info,
					    ساختار display_state *display_state,
					    صحیح summary_width)
{
	display_ref_update(display_state,
			   info->failed ? info->fail_code : info->success_code,
			   info->summary,
			   info->failed ? info->fail_detail : info->success_detail,
			   info->remote, info->ref, &info->old_oid,
			   &info->new_oid, summary_width);
}

ایستا صحیح update_local_ref(ساختار ref *ref,
			    ساختار ref_transaction *transaction,
			    ثابت ساختار ref *remote_ref,
			    ثابت ساختار fetch_config *config,
			    ساختار ref_update_display_info_array *display_array)
{
	ساختار commit *current = پوچ, *updated;
	صحیح fast_forward = 0;

	اگر (!odb_has_object(the_repository->objects, &ref->new_oid,
			    ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR))
		die(_("object %s not found"), oid_to_hex(&ref->new_oid));

	اگر (oideq(&ref->old_oid, &ref->new_oid)) {
		اگر (verbosity > 0)
			ref_update_display_info_append(display_array, '=', '=',
						       _("[up to date]"), پوچ,
						       پوچ, ref->name,
						       remote_ref->name, &ref->old_oid,
						       &ref->new_oid);
		بازگشت 0;
	}

	اگر (!update_head_ok &&
	    !is_null_oid(&ref->old_oid) &&
	    branch_checked_out(ref->name)) {
		ساختار ref_update_display_info *info;
		/*
		 * If this is the head, and it's not okay to update
		 * the head, and the old value of the head isn't empty...
		 */
		info = ref_update_display_info_append(display_array, '!', '!',
						      _("[rejected]"), پوچ,
						      _("can't fetch into checked-out branch"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);
		ref_update_display_info_set_failed(info);
		بازگشت 1;
	}

	اگر (!is_null_oid(&ref->old_oid) &&
	    starts_with(ref->name, "refs/tags/")) {
		ساختار ref_update_display_info *info;

		اگر (force || ref->force) {
			صحیح r;

			r = s_update_ref("updating tag", ref, transaction, 0);

			info = ref_update_display_info_append(display_array, 't', '!',
							      _("[tag update]"), پوچ,
							      _("unable to update local ref"),
							      ref->name, remote_ref->name,
							      &ref->old_oid, &ref->new_oid);
			اگر (r)
				ref_update_display_info_set_failed(info);

			بازگشت r;
		} وگرنه {
			info = ref_update_display_info_append(display_array, '!', '!',
							      _("[rejected]"), پوچ,
							      _("would clobber existing tag"),
							      ref->name, remote_ref->name,
							      &ref->old_oid, &ref->new_oid);
			ref_update_display_info_set_failed(info);
			بازگشت 1;
		}
	}

	current = lookup_commit_reference_gently(the_repository,
						 &ref->old_oid, 1);
	updated = lookup_commit_reference_gently(the_repository,
						 &ref->new_oid, 1);
	اگر (!current || !updated) {
		ساختار ref_update_display_info *info;
		ثابت نویسه *پیام;
		ثابت نویسه *what;
		صحیح r;
		/*
		 * Nicely describe the new ref we're fetching.
		 * Base this on the remote's ref name, as it's
		 * more likely to follow a standard layout.
		 */
		اگر (starts_with(remote_ref->name, "refs/tags/")) {
			پیام = "storing tag";
			what = _("[new tag]");
		} وگرنه اگر (starts_with(remote_ref->name, "refs/heads/")) {
			پیام = "storing head";
			what = _("[new branch]");
		} وگرنه {
			پیام = "storing ref";
			what = _("[new ref]");
		}

		r = s_update_ref(پیام, ref, transaction, 0);

		info = ref_update_display_info_append(display_array, '*', '!',
						      what, پوچ,
						      _("unable to update local ref"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);
		اگر (r)
			ref_update_display_info_set_failed(info);

		بازگشت r;
	}

	اگر (config->show_forced_updates) {
		uint64_t t_before = getnanotime();
		fast_forward = repo_in_merge_bases(the_repository, current,
						   updated);
		اگر (fast_forward < 0)
			die(پوچ);
		forced_updates_ms += (getnanotime() - t_before) / 1000000;
	} وگرنه {
		fast_forward = 1;
	}

	اگر (fast_forward) {
		ساختار ref_update_display_info *info;
		ساختار strbuf quickref = STRBUF_INIT;
		صحیح r;

		strbuf_add_unique_abbrev(&quickref, &current->object.oid, DEFAULT_ABBREV);
		strbuf_addstr(&quickref, "..");
		strbuf_add_unique_abbrev(&quickref, &ref->new_oid, DEFAULT_ABBREV);
		r = s_update_ref("fast-forward", ref, transaction, 1);

		info = ref_update_display_info_append(display_array, ' ', '!',
						      quickref.buf, پوچ,
						      _("unable to update local ref"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);
		اگر (r)
			ref_update_display_info_set_failed(info);

		strbuf_release(&quickref);
		بازگشت r;
	} وگرنه اگر (force || ref->force) {
		ساختار ref_update_display_info *info;
		ساختار strbuf quickref = STRBUF_INIT;
		صحیح r;

		strbuf_add_unique_abbrev(&quickref, &current->object.oid, DEFAULT_ABBREV);
		strbuf_addstr(&quickref, "...");
		strbuf_add_unique_abbrev(&quickref, &ref->new_oid, DEFAULT_ABBREV);
		r = s_update_ref("forced-update", ref, transaction, 1);

		info = ref_update_display_info_append(display_array, '+', '!',
						      quickref.buf, _("forced update"),
						      _("unable to update local ref"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);

		اگر (r)
			ref_update_display_info_set_failed(info);

		strbuf_release(&quickref);
		بازگشت r;
	} وگرنه {
		ساختار ref_update_display_info *info;
		info = ref_update_display_info_append(display_array, '!', '!',
						      _("[rejected]"), پوچ,
						      _("non-fast-forward"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);
		ref_update_display_info_set_failed(info);
		بازگشت 1;
	}
}

ایستا ثابت ساختار object_id *iterate_ref_map(تهی *cb_data)
{
	ساختار ref **rm = cb_data;
	ساختار ref *ref = *rm;

	وقتی (ref && ref->status == REF_STATUS_REJECT_SHALLOW)
		ref = ref->next;
	اگر (!ref)
		بازگشت پوچ;
	*rm = ref->next;
	بازگشت &ref->old_oid;
}

ساختار fetch_head {
	FILE *fp;
	ساختار strbuf buf;
};

ایستا صحیح open_fetch_head(ساختار fetch_head *fetch_head)
{
	ثابت نویسه *filename = git_path_fetch_head(the_repository);

	اگر (write_fetch_head) {
		fetch_head->fp = fopen(filename, "a");
		اگر (!fetch_head->fp)
			بازگشت error_errno(_("cannot open '%s'"), filename);
		strbuf_init(&fetch_head->buf, 0);
	} وگرنه {
		fetch_head->fp = پوچ;
	}

	بازگشت 0;
}

ایستا تهی append_fetch_head(ساختار fetch_head *fetch_head,
			      ثابت ساختار object_id *old_oid,
			      شمارش fetch_head_status fetch_head_status,
			      ثابت نویسه *note,
			      ثابت نویسه *url, size_t url_len)
{
	نویسه old_oid_hex[GIT_MAX_HEXSZ + 1];
	ثابت نویسه *merge_status_marker;
	size_t i;

	اگر (!fetch_head->fp)
		بازگشت;

	انتخاب (fetch_head_status) {
	حالت FETCH_HEAD_NOT_FOR_MERGE:
		merge_status_marker = "not-for-merge";
		بشکن;
	حالت FETCH_HEAD_MERGE:
		merge_status_marker = "";
		بشکن;
	پیشفرض:
		/* do not write anything to FETCH_HEAD */
		بازگشت;
	}

	strbuf_addf(&fetch_head->buf, "%s\t%s\t%s",
		    oid_to_hex_r(old_oid_hex, old_oid), merge_status_marker, note);
	برای (i = 0; i < url_len; ++i)
		اگر ('\n' == url[i])
			strbuf_addstr(&fetch_head->buf, "\\n");
		وگرنه
			strbuf_addch(&fetch_head->buf, url[i]);
	strbuf_addch(&fetch_head->buf, '\n');

	/*
	 * When using an atomic fetch, we do not want to update FETCH_HEAD if
	 * any of the reference updates fails. We thus have to write all
	 * updates to a buffer first and only commit it as soon as all
	 * references have been successfully updated.
	 */
	اگر (!atomic_fetch) {
		strbuf_write(&fetch_head->buf, fetch_head->fp);
		strbuf_reset(&fetch_head->buf);
	}
}

ایستا تهی commit_fetch_head(ساختار fetch_head *fetch_head)
{
	اگر (!fetch_head->fp || !atomic_fetch)
		بازگشت;
	strbuf_write(&fetch_head->buf, fetch_head->fp);
}

ایستا تهی close_fetch_head(ساختار fetch_head *fetch_head)
{
	اگر (!fetch_head->fp)
		بازگشت;

	fclose(fetch_head->fp);
	strbuf_release(&fetch_head->buf);
}

ایستا ثابت نویسه warn_show_forced_updates[] =
N_("fetch normally indicates which branches had a forced update,\n"
   "but that check has been disabled; to re-enable, use '--show-forced-updates'\n"
   "flag or run 'git config fetch.showForcedUpdates true'");
ایستا ثابت نویسه warn_time_show_forced_updates[] =
N_("it took %.2f seconds to check forced updates; you can use\n"
   "'--no-show-forced-updates' or run 'git config fetch.showForcedUpdates false'\n"
   "to avoid this check\n");

ایستا صحیح store_updated_refs(ساختار display_state *display_state,
			      صحیح connectivity_checked,
			      ساختار ref_transaction *transaction, ساختار ref *ref_map,
			      ساختار fetch_head *fetch_head,
			      ثابت ساختار fetch_config *config,
			      ساختار ref_update_display_info_array *display_array)
{
	صحیح rc = 0;
	ساختار strbuf note = STRBUF_INIT;
	ثابت نویسه *what, *kind;
	ساختار ref *rm;
	صحیح want_status;

	اگر (!connectivity_checked) {
		ساختار check_connected_options opt = CHECK_CONNECTED_INIT;

		opt.exclude_hidden_refs_section = "fetch";
		rm = ref_map;
		اگر (check_connected(iterate_ref_map, &rm, &opt)) {
			rc = error(_("%s did not send all necessary objects"),
				   display_state->url);
			برو abort;
		}
	}

	/*
	 * We do a pass for each fetch_head_status type in their enum order, so
	 * merged entries are written before not-for-merge. That lets readers
	 * use FETCH_HEAD as a refname to refer to the ref to be merged.
	 */
	برای (want_status = FETCH_HEAD_MERGE;
	     want_status <= FETCH_HEAD_IGNORE;
	     want_status++) {
		برای (rm = ref_map; rm; rm = rm->next) {
			ساختار ref *ref = پوچ;

			اگر (rm->status == REF_STATUS_REJECT_SHALLOW) {
				اگر (want_status == FETCH_HEAD_MERGE)
					warning(_("rejected %s because shallow roots are not allowed to be updated"),
						rm->peer_ref ? rm->peer_ref->name : rm->name);
				ادامه;
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
			اگر (fetch_head->fp) {
				ساختار commit *commit = پوچ;

				/*
				 * References in "refs/tags/" are often going to point
				 * to annotated tags, which are not part of the
				 * commit-graph. We thus only try to look up refs in
				 * the graph which are not in that namespace to not
				 * regress performance in repositories with many
				 * annotated tags.
				 */
				اگر (!starts_with(rm->name, "refs/tags/"))
					commit = lookup_commit_in_graph(the_repository, &rm->old_oid);
				اگر (!commit) {
					commit = lookup_commit_reference_gently(the_repository,
										&rm->old_oid,
										1);
					اگر (!commit)
						rm->fetch_head_status = FETCH_HEAD_NOT_FOR_MERGE;
				}
			}

			اگر (rm->fetch_head_status != want_status)
				ادامه;

			اگر (rm->peer_ref) {
				ref = alloc_ref(rm->peer_ref->name);
				oidcpy(&ref->old_oid, &rm->peer_ref->old_oid);
				oidcpy(&ref->new_oid, &rm->old_oid);
				ref->force = rm->peer_ref->force;
			}

			اگر (config->recurse_submodules != RECURSE_SUBMODULES_OFF &&
			    (!rm->peer_ref || !oideq(&ref->old_oid, &ref->new_oid))) {
				check_for_new_submodule_commits(&rm->old_oid);
			}

			اگر (!strcmp(rm->name, "HEAD")) {
				kind = "";
				what = "";
			} وگرنه اگر (skip_prefix(rm->name, "refs/heads/", &what)) {
				kind = "branch";
			} وگرنه اگر (skip_prefix(rm->name, "refs/tags/", &what)) {
				kind = "tag";
			} وگرنه اگر (skip_prefix(rm->name, "refs/remotes/", &what)) {
				kind = "remote-tracking branch";
			} وگرنه {
				kind = "";
				what = rm->name;
			}

			strbuf_reset(&note);
			اگر (*what) {
				اگر (*kind)
					strbuf_addf(&note, "%s ", kind);
				strbuf_addf(&note, "'%s' of ", what);
			}

			append_fetch_head(fetch_head, &rm->old_oid,
					  rm->fetch_head_status,
					  note.buf, display_state->url,
					  display_state->url_len);

			اگر (ref) {
				rc |= update_local_ref(ref, transaction, rm,
						       config, display_array);
				آزاد(ref);
			} وگرنه اگر (write_fetch_head || dry_run) {
				/*
				 * Display fetches written to FETCH_HEAD (or
				 * would be written to FETCH_HEAD, if --dry-run
				 * is set).
				 */

				ref_update_display_info_append(display_array, '*', '*',
							       *kind ? kind : "branch",
							       پوچ, پوچ, "FETCH_HEAD",
							       rm->name, &rm->new_oid,
							       &rm->old_oid);
			}
		}
	}

	اگر (advice_enabled(ADVICE_FETCH_SHOW_FORCED_UPDATES)) {
		اگر (!config->show_forced_updates) {
			warning(_(warn_show_forced_updates));
		} وگرنه اگر (forced_updates_ms > FORCED_UPDATES_DELAY_WARNING_IN_MS) {
			warning(_(warn_time_show_forced_updates),
				forced_updates_ms / 1000.0);
		}
	}

 abort:
	strbuf_release(&note);
	بازگشت rc;
}

/*
 * We would want to bypass the object transfer altogether if
 * everything we are going to fetch already exists and is connected
 * locally.
 */
ایستا صحیح check_exist_and_connected(ساختار ref *ref_map)
{
	ساختار ref *rm = ref_map;
	ساختار check_connected_options opt = CHECK_CONNECTED_INIT;
	ساختار ref *r;

	/*
	 * If we are deepening a shallow clone we already have these
	 * objects reachable.  Running rev-list here will return with
	 * a good (0) exit status and we'll bypass the fetch that we
	 * really need to perform.  Claiming failure now will ensure
	 * we perform the network exchange to deepen our history.
	 */
	اگر (deepen)
		بازگشت -1;

	/*
	 * Similarly, if we need to refetch, we always want to perform a full
	 * fetch ignoring existing objects.
	 */
	اگر (refetch)
		بازگشت -1;


	/*
	 * check_connected() allows objects to merely be promised, but
	 * we need all direct targets to exist.
	 */
	برای (r = rm; r; r = r->next) {
		اگر (!odb_has_object(the_repository->objects, &r->old_oid,
				    ODB_HAS_OBJECT_RECHECK_PACKED))
			بازگشت -1;
	}

	opt.quiet = 1;
	opt.exclude_hidden_refs_section = "fetch";
	بازگشت check_connected(iterate_ref_map, &rm, &opt);
}

ایستا صحیح fetch_and_consume_refs(ساختار display_state *display_state,
				  ساختار transport *transport,
				  ساختار ref_transaction *transaction,
				  ساختار ref *ref_map,
				  ساختار fetch_head *fetch_head,
				  ثابت ساختار fetch_config *config,
				  ساختار ref_update_display_info_array *display_array)
{
	صحیح connectivity_checked = 1;
	صحیح ret;

	/*
	 * We don't need to perform a fetch in case we can already satisfy all
	 * refs.
	 */
	ret = check_exist_and_connected(ref_map);
	اگر (ret) {
		trace2_region_enter("fetch", "fetch_refs", the_repository);
		ret = transport_fetch_refs(transport, ref_map);
		trace2_region_leave("fetch", "fetch_refs", the_repository);
		اگر (ret)
			برو out;
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
	بازگشت ret;
}

ایستا صحیح prune_refs(ساختار display_state *display_state,
		      ساختار refspec *rs,
		      ساختار ref_transaction *transaction,
		      ساختار ref *ref_map)
{
	صحیح result = 0;
	ساختار ref *ref, *stale_refs = get_stale_heads(rs, ref_map);
	ساختار strbuf err = STRBUF_INIT;
	ساختار string_list refnames = STRING_LIST_INIT_NODUP;

	برای (ref = stale_refs; ref; ref = ref->next)
		string_list_append(&refnames, ref->name);

	اگر (!dry_run) {
		اگر (transaction) {
			برای (ref = stale_refs; ref; ref = ref->next) {
				result = ref_transaction_delete(transaction, ref->name, پوچ,
								پوچ, 0, "fetch: prune", &err);
				اگر (result)
					برو cleanup;
			}
		} وگرنه {
			result = refs_delete_refs(get_main_ref_store(the_repository),
						  "fetch: prune", &refnames,
						  0);
		}
	}

	اگر (verbosity >= 0) {
		صحیح summary_width = transport_summary_width(stale_refs);

		برای (ref = stale_refs; ref; ref = ref->next) {
			display_ref_update(display_state, '-', _("[deleted]"), پوچ,
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
	بازگشت result;
}

ایستا تهی check_not_current_branch(ساختار ref *ref_map)
{
	ثابت نویسه *path;
	برای (; ref_map; ref_map = ref_map->next)
		اگر (ref_map->peer_ref &&
		    starts_with(ref_map->peer_ref->name, "refs/heads/") &&
		    (path = branch_checked_out(ref_map->peer_ref->name)))
			die(_("refusing to fetch into branch '%s' "
			      "checked out at '%s'"),
			    ref_map->peer_ref->name, path);
}

ایستا صحیح truncate_fetch_head(تهی)
{
	ثابت نویسه *filename = git_path_fetch_head(the_repository);
	FILE *fp = fopen_for_writing(filename);

	اگر (!fp)
		بازگشت error_errno(_("cannot open '%s'"), filename);
	fclose(fp);
	بازگشت 0;
}

ایستا تهی set_option(ساختار transport *transport, ثابت نویسه *name, ثابت نویسه *مقدار)
{
	صحیح r = transport_set_option(transport, name, مقدار);
	اگر (r < 0)
		die(_("option \"%s\" value \"%s\" is not valid for %s"),
		    name, مقدار, transport->url);
	اگر (r > 0)
		warning(_("option \"%s\" is ignored for %s"),
			name, transport->url);
}


ایستا صحیح add_oid(ثابت ساختار reference *ref, تهی *cb_data)
{
	ساختار oid_array *oids = cb_data;

	oid_array_append(oids, ref->oid);
	بازگشت 0;
}

ایستا تهی add_negotiation_tips(ساختار string_list *درون_داد_list,
				 ساختار oid_array **output_list,
				 ثابت نویسه *argname)
{
	ساختار oid_array *oids = xcalloc(1, اندازه(*oids));
	صحیح i;

	برای (i = 0; i < درون_داد_list->nr; i++) {
		ثابت نویسه *s = درون_داد_list->اقلام[i].string;
		ساختار refs_for_each_ref_options opts = {
			.pattern = s,
		};
		صحیح old_nr;
		اگر (!has_glob_specials(s)) {
			ساختار object_id oid;

			/* Ignore missing reference. */
			اگر (repo_get_oid(the_repository, s, &oid))
				ادامه;
			/* Fail on missing object pointed by ref. */
			اگر (!odb_has_object(the_repository->objects, &oid, 0))
				die(_("the object %s does not exist"), s);

			oid_array_append(oids, &oid);
			ادامه;
		}
		old_nr = oids->nr;
		refs_for_each_ref_ext(get_main_ref_store(the_repository),
				      add_oid, oids, &opts);
		اگر (old_nr == oids->nr)
			warning(_("ignoring %s=%s because it does not match any refs"),
				argname, s);
	}
	*output_list = oids;
}

ایستا ساختار transport *prepare_transport(ساختار remote *remote, صحیح deepen,
					   ساختار list_objects_filter_options *filter_options)
{
	ساختار transport *transport;

	transport = transport_get(remote, پوچ);
	transport_set_verbosity(transport, verbosity, progress);
	transport->family = family;
	اگر (upload_pack)
		set_option(transport, TRANS_OPT_UPLOADPACK, upload_pack);
	اگر (keep)
		set_option(transport, TRANS_OPT_KEEP, "yes");
	اگر (depth)
		set_option(transport, TRANS_OPT_DEPTH, depth);
	اگر (deepen && deepen_since)
		set_option(transport, TRANS_OPT_DEEPEN_SINCE, deepen_since);
	اگر (deepen && deepen_not.nr)
		set_option(transport, TRANS_OPT_DEEPEN_NOT,
			   (ثابت نویسه *)&deepen_not);
	اگر (deepen_relative)
		set_option(transport, TRANS_OPT_DEEPEN_RELATIVE, "yes");
	اگر (update_shallow)
		set_option(transport, TRANS_OPT_UPDATE_SHALLOW, "yes");
	اگر (refetch)
		set_option(transport, TRANS_OPT_REFETCH, "yes");
	اگر (filter_options->choice) {
		ثابت نویسه *spec =
			expand_list_objects_filter_spec(filter_options);
		set_option(transport, TRANS_OPT_LIST_OBJECTS_FILTER, spec);
		set_option(transport, TRANS_OPT_FROM_PROMISOR, "1");
	}
	اگر (negotiation_restrict.nr) {
		اگر (transport->smart_options)
			add_negotiation_tips(&negotiation_restrict,
					     &transport->smart_options->negotiation_restrict_tips,
					     "--negotiation-restrict");
		وگرنه
			warning(_("ignoring %s because the protocol does not support it"),
				"--negotiation-restrict");
	} وگرنه اگر (remote->negotiation_restrict.nr) {
		ساختار string_list_item *item;
		for_each_string_list_item(item, &remote->negotiation_restrict)
			string_list_append(&negotiation_restrict, item->string);
		اگر (transport->smart_options)
			add_negotiation_tips(&negotiation_restrict,
					     &transport->smart_options->negotiation_restrict_tips,
					     "--negotiation-restrict");
		وگرنه {
			ساختار strbuf config_name = STRBUF_INIT;
			strbuf_addf(&config_name, "remote.%s.negotiationRestrict", remote->name);
			warning(_("ignoring %s because the protocol does not support it"),
				config_name.buf);
			strbuf_release(&config_name);
		}
	}
	اگر (negotiation_include.nr) {
		اگر (transport->smart_options)
			add_negotiation_tips(&negotiation_include,
					     &transport->smart_options->negotiation_include_tips,
					     "--negotiation-include");
		وگرنه
			warning(_("ignoring %s because the protocol does not support it"),
				"--negotiation-include");
	} وگرنه اگر (remote->negotiation_include.nr) {
		اگر (transport->smart_options) {
			add_negotiation_tips(&remote->negotiation_include,
					     &transport->smart_options->negotiation_include_tips,
					     "--negotiation-include");
		} وگرنه {
			ساختار strbuf config_name = STRBUF_INIT;
			strbuf_addf(&config_name, "remote.%s.negotiationInclude", remote->name);
			warning(_("ignoring %s because the protocol does not support it"),
				config_name.buf);
			strbuf_release(&config_name);
		}
	}
	بازگشت transport;
}

ایستا صحیح backfill_tags(ساختار display_state *display_state,
			 ساختار transport *transport,
			 ساختار ref_transaction *transaction,
			 ساختار ref *ref_map,
			 ساختار fetch_head *fetch_head,
			 ثابت ساختار fetch_config *config,
			 ساختار ref_update_display_info_array *display_array,
			 ساختار list_objects_filter_options *filter_options)
{
	صحیح retcode, cannot_reuse;

	/*
	 * Once we have set TRANS_OPT_DEEPEN_SINCE, we can't unset it
	 * when remote helper is used (setting it to an empty string
	 * is not unsetting). We could extend the remote helper
	 * protocol for that, but for now, just force a new connection
	 * without deepen-since. Similar story for deepen-not.
	 */
	cannot_reuse = transport->cannot_reuse ||
		deepen_since || deepen_not.nr;
	اگر (cannot_reuse) {
		gsecondary = prepare_transport(transport->remote, 0, filter_options);
		transport = gsecondary;
	}

	transport_set_option(transport, TRANS_OPT_FOLLOWTAGS, پوچ);
	transport_set_option(transport, TRANS_OPT_DEPTH, "0");
	transport_set_option(transport, TRANS_OPT_DEEPEN_RELATIVE, پوچ);
	retcode = fetch_and_consume_refs(display_state, transport, transaction, ref_map,
					 fetch_head, config, display_array);

	اگر (gsecondary) {
		transport_disconnect(gsecondary);
		gsecondary = پوچ;
	}

	بازگشت retcode;
}

ایستا ثابت نویسه *strip_refshead(ثابت نویسه *name){
	skip_prefix(name, "refs/heads/", &name);
	بازگشت name;
}

ایستا تهی set_head_advice_پیام(ثابت نویسه *remote, ثابت نویسه *head_name)
{
	ثابت نویسه message_advice_set_head[] =
	N_("Run 'git remote set-head %s %s' to follow the change, or modify\n"
	   "either of the 'remote.%s.followRemoteHEAD' or 'fetch.followRemoteHEAD'\n"
	   "configuration variables to handle the situation differently.\n\n"

	   "Using this specific setting\n\n"
	   "    git config set remote.%s.followRemoteHEAD warn-if-not-%s\n\n"
	   "will suppress the warning until the remote changes HEAD to something else.");

	advise_if_enabled(ADVICE_FETCH_SET_HEAD_WARN, _(message_advice_set_head),
			remote, head_name, remote, remote, head_name);
}

ایستا تهی warn_set_head(ثابت نویسه *remote, ثابت نویسه *head_name,
			ساختار strbuf *buf_prev, صحیح updateres) {
	ساختار strbuf buf_prefix = STRBUF_INIT;
	ثابت نویسه *prev_head = پوچ;

	strbuf_addf(&buf_prefix, "refs/remotes/%s/", remote);
	skip_prefix(buf_prev->buf, buf_prefix.buf, &prev_head);

	اگر (prev_head && strcmp(prev_head, head_name)) {
		printf("'HEAD' at '%s' is '%s', but we have '%s' locally.\n",
			remote, head_name, prev_head);
		set_head_advice_پیام(remote, head_name);
	}
	وگرنه اگر (updateres && buf_prev->len) {
		printf("'HEAD' at '%s' is '%s', "
			"but we have a detached HEAD pointing to '%s' locally.\n",
			remote, head_name, buf_prev->buf);
		set_head_advice_پیام(remote, head_name);
	}
	strbuf_release(&buf_prefix);
}

ایستا صحیح set_head(ثابت ساختار ref *remote_refs, ساختار remote *remote,
			صحیح follow_remote_head)
{
	صحیح result = 0, create_only, baremirror, was_detached;
	ساختار strbuf b_head = STRBUF_INIT, b_remote_head = STRBUF_INIT,
		      b_local_head = STRBUF_INIT;
	ثابت نویسه *no_warn_branch = remote->no_warn_branch;
	نویسه *head_name = پوچ;
	ساختار ref *ref, *matches;
	ساختار ref *fetch_map = پوچ, **fetch_map_tail = &fetch_map;
	ساختار refspec_item refspec = {
		.force = 0,
		.pattern = 1,
		.src = (نویسه *) "refs/heads/*",
		.dst = (نویسه *) "refs/heads/*",
	};
	ساختار string_list heads = STRING_LIST_INIT_DUP;
	ساختار ref_store *refs = get_main_ref_store(the_repository);

	get_fetch_map(remote_refs, &refspec, &fetch_map_tail, 0);
	matches = guess_remote_head(find_ref_by_name(remote_refs, "HEAD"),
				    fetch_map, REMOTE_GUESS_HEAD_ALL);
	برای (ref = matches; ref; ref = ref->next) {
		string_list_append(&heads, strip_refshead(ref->name));
	}

	اگر (!heads.nr)
		result = 1;
	وگرنه اگر (heads.nr > 1)
		result = 1;
	وگرنه
		head_name = xstrdup(heads.اقلام[0].string);

	اگر (!head_name)
		برو cleanup;
	baremirror = is_bare_repository(the_repository) && remote->mirror;
	create_only = follow_remote_head == FOLLOW_REMOTE_ALWAYS ? 0 : !baremirror;
	اگر (baremirror) {
		strbuf_addstr(&b_head, "HEAD");
		strbuf_addf(&b_remote_head, "refs/heads/%s", head_name);
	} وگرنه {
		strbuf_addf(&b_head, "refs/remotes/%s/HEAD", remote->name);
		strbuf_addf(&b_remote_head, "refs/remotes/%s/%s", remote->name, head_name);
	}
	/* make sure it's valid */
	اگر (!baremirror && !refs_ref_exists(refs, b_remote_head.buf)) {
		result = 1;
		برو cleanup;
	}
	was_detached = refs_update_symref_extended(refs, b_head.buf, b_remote_head.buf,
					"fetch", &b_local_head, create_only);
	اگر (was_detached == -1) {
		result = 1;
		برو cleanup;
	}
	اگر (verbosity >= 0 &&
		follow_remote_head == FOLLOW_REMOTE_WARN &&
		(!no_warn_branch || strcmp(no_warn_branch, head_name)))
		warn_set_head(remote->name, head_name, &b_local_head, was_detached);

cleanup:
	آزاد(head_name);
	free_refs(fetch_map);
	free_refs(matches);
	string_list_clear(&heads, 0);
	strbuf_release(&b_head);
	strbuf_release(&b_local_head);
	strbuf_release(&b_remote_head);
	بازگشت result;
}

ساختار ref_rejection_data {
	صحیح *retcode;
	bool conflict_پیام_shown;
	bool case_sensitive_پیام_shown;
	ثابت نویسه *remote_name;
	ساختار strmap *rejected_refs;
};

ایستا تهی ref_transaction_rejection_handler(ثابت نویسه *refname,
					      ثابت ساختار object_id *old_oid UNUSED,
					      ثابت ساختار object_id *new_oid UNUSED,
					      ثابت نویسه *old_target UNUSED,
					      ثابت نویسه *new_target UNUSED,
					      شمارش ref_transaction_error err,
					      ثابت نویسه *details,
					      تهی *cb_data)
{
	ساختار ref_rejection_data *data = cb_data;

	اگر (err == REF_TRANSACTION_ERROR_CASE_CONFLICT && repo_ignore_case(the_repository) &&
	    !data->case_sensitive_پیام_shown) {
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
		data->case_sensitive_پیام_shown = true;
	} وگرنه اگر (err == REF_TRANSACTION_ERROR_NAME_CONFLICT &&
		   !data->conflict_پیام_shown) {
		error(_("some local refs could not be updated; try running\n"
			" 'git remote prune %s' to remove any old, conflicting "
			"branches"), data->remote_name);
		data->conflict_پیام_shown = true;
	} وگرنه {
		اگر (details)
			error("%s", details);
		وگرنه
			error(_("fetching ref %s failed: %s"),
			      refname, ref_transaction_error_پیام(err));
	}

	strmap_put(data->rejected_refs, refname, پوچ);
	*data->retcode = 1;
}

/*
 * Commit the reference transaction. If it isn't an atomic transaction, handle
 * rejected updates as part of using batched updates.
 */
ایستا صحیح commit_ref_transaction(ساختار ref_transaction **transaction,
				  bool is_atomic, ثابت نویسه *remote_name,
				  ساختار strmap *rejected_refs,
				  ساختار strbuf *err)
{
	صحیح retcode = ref_transaction_commit(*transaction, err);
	اگر (retcode)
		برو out;

	اگر (!is_atomic) {
		ساختار ref_rejection_data data = {
			.conflict_پیام_shown = 0,
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
	*transaction = پوچ;
	بازگشت retcode;
}

ایستا تهی collect_updated_tips(ساختار oidset *tips, ساختار ref *ref_map)
{
	ساختار ref *rm;
	برای (rm = ref_map; rm; rm = rm->next) {
		ساختار commit *commit;
		/*
		 * Shallow-rejected refs are not stored and their history
		 * is incomplete, so skip them.
		 */
		اگر (rm->status == REF_STATUS_REJECT_SHALLOW)
			ادامه;
		اگر (is_null_oid(&rm->old_oid))
			ادامه;
		اگر (rm->peer_ref &&
		    oideq(&rm->old_oid, &rm->peer_ref->old_oid))
			ادامه;
		commit = lookup_commit_reference_gently(the_repository,
							&rm->old_oid, 1);
		اگر (commit)
			oidset_درج(tips, &commit->object.oid);
	}
}

ایستا صحیح do_fetch(ساختار transport *transport,
		    ساختار refspec *rs,
		    ثابت ساختار fetch_config *config,
		    ساختار list_objects_filter_options *filter_options,
		    ساختار oidset *updated_tips)
{
	ساختار ref_transaction *transaction = پوچ;
	ساختار ref *ref_map = پوچ;
	ساختار display_state display_state = { 0 };
	صحیح autotags = (transport->remote->fetch_tags == 1);
	صحیح retcode = 0;
	ثابت ساختار ref *remote_refs;
	ساختار transport_ls_refs_options transport_ls_refs_options =
		TRANSPORT_LS_REFS_OPTIONS_INIT;
	ساختار fetch_head fetch_head = { 0 };
	ساختار strbuf err = STRBUF_INIT;
	ساختار ref_update_display_info_array display_array = { 0 };
	ساختار strmap rejected_refs = STRMAP_INIT;
	صحیح summary_width = 0;
	صحیح follow_remote_head = FOLLOW_REMOTE_NEVER;

	اگر (tags == TAGS_DEFAULT) {
		اگر (transport->remote->fetch_tags == 2)
			tags = TAGS_SET;
		اگر (transport->remote->fetch_tags == -1)
			tags = TAGS_UNSET;
	}

	/* if not appending, truncate FETCH_HEAD */
	اگر (!append && write_fetch_head) {
		retcode = truncate_fetch_head();
		اگر (retcode)
			برو cleanup;
	}

	اگر (rs->nr) {
		refspec_ref_prefixes(rs, &transport_ls_refs_options.ref_prefixes);
	} وگرنه {
		ساختار branch *branch = branch_get(پوچ);

		اگر (transport->remote->fetch.nr) {
			refspec_ref_prefixes(&transport->remote->fetch,
					     &transport_ls_refs_options.ref_prefixes);
			/*
			 * See remote.c's handling of remote.<name>.followRemoteHEAD
			 * for the analogous, still-unresolved case.
			 */
			اگر (transport->remote->follow_remote_head)
				follow_remote_head = transport->remote->follow_remote_head;
			وگرنه اگر (config->follow_remote_head_seen)
				follow_remote_head = get_follow_remote_head(config->follow_remote_head_raw);
			وگرنه
				follow_remote_head = BUILTIN_FOLLOW_REMOTE_HEAD_DFLT;
		}
		اگر (branch && branch_has_merge_config(branch) &&
		    !strcmp(branch->remote_name, transport->remote->name)) {
			صحیح i;
			برای (i = 0; i < branch->merge_nr; i++) {
				strvec_افزودن(&transport_ls_refs_options.ref_prefixes,
					    branch->merge[i]->src);
			}
		}

		/*
		 * If there are no refs specified to fetch, then we just
		 * fetch HEAD; mention that to narrow the advertisement.
		 */
		اگر (!transport_ls_refs_options.ref_prefixes.nr)
			strvec_افزودن(&transport_ls_refs_options.ref_prefixes,
				    "HEAD");
	}

	اگر (tags == TAGS_SET || tags == TAGS_DEFAULT)
		strvec_افزودن(&transport_ls_refs_options.ref_prefixes,
			    "refs/tags/");

	اگر (follow_remote_head != FOLLOW_REMOTE_NEVER)
		strvec_افزودن(&transport_ls_refs_options.ref_prefixes,
			    "HEAD");

	/*
	 * Only initiate ref listing if we have at least one ref we want to
	 * know about.
	 */
	اگر (transport_ls_refs_options.ref_prefixes.nr) {
		trace2_region_enter("fetch", "remote_refs", the_repository);
		remote_refs = transport_get_remote_refs(transport,
							&transport_ls_refs_options);
		trace2_region_leave("fetch", "remote_refs", the_repository);
	} وگرنه
		remote_refs = پوچ;

	transport_ls_refs_options_release(&transport_ls_refs_options);

	ref_map = get_ref_map(transport->remote, remote_refs, rs,
			      tags, &autotags);
	اگر (!update_head_ok)
		check_not_current_branch(ref_map);

	retcode = open_fetch_head(&fetch_head);
	اگر (retcode)
		برو cleanup;

	display_state_init(&display_state, ref_map, transport->url,
			   config->display_format);

	اگر (atomic_fetch) {
		transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
							  0, &err);
		اگر (!transaction) {
			retcode = -1;
			برو cleanup;
		}
	}

	اگر (tags == TAGS_DEFAULT && autotags)
		transport_set_option(transport, TRANS_OPT_FOLLOWTAGS, "1");
	اگر (prune) {
		/*
		 * We only prune based on refspecs specified
		 * explicitly (via command line or configuration); we
		 * don't care whether --tags was specified.
		 */
		اگر (rs->nr) {
			retcode = prune_refs(&display_state, rs, transaction, ref_map);
		} وگرنه {
			retcode = prune_refs(&display_state, &transport->remote->fetch,
					     transaction, ref_map);
		}
		اگر (retcode != 0)
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
	اگر (!transaction) {
		transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
							  REF_TRANSACTION_ALLOW_FAILURE, &err);
		اگر (!transaction) {
			retcode = -1;
			برو cleanup;
		}
	}

	اگر (fetch_and_consume_refs(&display_state, transport, transaction, ref_map,
				   &fetch_head, config, &display_array)) {
		retcode = 1;
		برو cleanup;
	}

	/*
	 * If neither --no-tags nor --tags was specified, do automated tag
	 * following.
	 */
	اگر (tags == TAGS_DEFAULT && autotags) {
		ساختار ref *tags_ref_map = پوچ, **tail = &tags_ref_map;

		find_non_local_tags(remote_refs, transaction, &tags_ref_map, &tail);
		اگر (tags_ref_map) {
			/*
			 * If backfilling of tags fails then we want to tell
			 * the user so, but we have to continue regardless to
			 * populate upstream information of the references we
			 * have already fetched above. The exception though is
			 * when `--atomic` is passed: in that case we'll abort
			 * the transaction and don't commit anything.
			 */
			اگر (backfill_tags(&display_state, transport, transaction, tags_ref_map,
					  &fetch_head, config, &display_array, filter_options))
				retcode = 1;
		}

		free_refs(tags_ref_map);
	}

	اگر (retcode)
		برو cleanup;

	اگر (verbosity >= 0)
		summary_width = transport_summary_width(ref_map);

	retcode = commit_ref_transaction(&transaction, atomic_fetch,
					 transport->remote->name,
					 &rejected_refs, &err);
	/*
	 * With '--atomic', bail out if the transaction fails. Without '--atomic',
	 * continue to fetch head and perform other post-fetch operations.
	 */
	اگر (retcode && atomic_fetch)
		برو cleanup;

	commit_fetch_head(&fetch_head);

	collect_updated_tips(updated_tips, ref_map);

	اگر (set_upstream) {
		ساختار branch *branch = branch_get("HEAD");
		ساختار ref *rm;
		ساختار ref *source_ref = پوچ;

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
		برای (rm = ref_map; rm; rm = rm->next) {
			اگر (!rm->peer_ref) {
				اگر (source_ref) {
					warning(_("multiple branches detected, incompatible with --set-upstream"));
					برو cleanup;
				} وگرنه {
					source_ref = rm;
				}
			}
		}
		اگر (source_ref) {
			اگر (!branch) {
				ثابت نویسه *shortname = source_ref->name;
				skip_prefix(shortname, "refs/heads/", &shortname);

				warning(_("could not set upstream of HEAD to '%s' from '%s' when "
					  "it does not point to any branch."),
					shortname, transport->remote->name);
				برو cleanup;
			}

			اگر (!strcmp(source_ref->name, "HEAD") ||
			    starts_with(source_ref->name, "refs/heads/"))
				install_branch_config(0,
						      branch->name,
						      transport->remote->name,
						      source_ref->name);
			وگرنه اگر (starts_with(source_ref->name, "refs/remotes/"))
				warning(_("not setting upstream for a remote remote-tracking branch"));
			وگرنه اگر (starts_with(source_ref->name, "refs/tags/"))
				warning(_("not setting upstream for a remote tag"));
			وگرنه
				warning(_("unknown branch type"));
		} وگرنه {
			warning(_("no source branch found;\n"
				  "you need to specify exactly one branch with the --set-upstream option"));
		}
	}
	اگر (follow_remote_head != FOLLOW_REMOTE_NEVER) {
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
	اگر (retcode && !atomic_fetch && transaction)
		commit_ref_transaction(&transaction, false,
				       transport->remote->name,
				       &rejected_refs, &err);

	برای (size_t i = 0; i < display_array.nr; i++) {
		ساختار ref_update_display_info *info = &display_array.info[i];

		اگر (!info->failed && strmap_contains(&rejected_refs, info->ref))
			ref_update_display_info_set_failed(info);
		ref_update_display_info_display(info, &display_state, summary_width);
		ref_update_display_info_free(info);
	}

	اگر (retcode) {
		اگر (err.len) {
			error("%s", err.buf);
			strbuf_reset(&err);
		}
		اگر (transaction && ref_transaction_abort(transaction, &err) &&
		    err.len)
			error("%s", err.buf);
		transaction = پوچ;
	}

	اگر (transaction)
		ref_transaction_free(transaction);

	آزاد(display_array.info);
	strmap_clear(&rejected_refs, 0);
	display_state_release(&display_state);
	close_fetch_head(&fetch_head);
	strbuf_release(&err);
	free_refs(ref_map);
	بازگشت retcode;
}

ایستا صحیح get_one_remote_for_fetch(ساختار remote *remote, تهی *priv)
{
	ساختار string_list *list = priv;
	اگر (!remote->skip_default_update)
		string_list_append(list, remote->name);
	بازگشت 0;
}

ایستا تهی add_options_to_argv(ساختار strvec *argv,
				ثابت ساختار fetch_config *config)
{
	اگر (dry_run)
		strvec_افزودن(argv, "--dry-run");
	اگر (prune != -1)
		strvec_افزودن(argv, prune ? "--prune" : "--no-prune");
	اگر (prune_tags != -1)
		strvec_افزودن(argv, prune_tags ? "--prune-tags" : "--no-prune-tags");
	اگر (update_head_ok)
		strvec_افزودن(argv, "--update-head-ok");
	اگر (force)
		strvec_افزودن(argv, "--force");
	اگر (keep)
		strvec_افزودن(argv, "--keep");
	اگر (config->recurse_submodules == RECURSE_SUBMODULES_ON)
		strvec_افزودن(argv, "--recurse-submodules");
	وگرنه اگر (config->recurse_submodules == RECURSE_SUBMODULES_OFF)
		strvec_افزودن(argv, "--no-recurse-submodules");
	وگرنه اگر (config->recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND)
		strvec_افزودن(argv, "--recurse-submodules=on-demand");
	اگر (tags == TAGS_SET)
		strvec_افزودن(argv, "--tags");
	وگرنه اگر (tags == TAGS_UNSET)
		strvec_افزودن(argv, "--no-tags");
	اگر (verbosity >= 2)
		strvec_افزودن(argv, "-v");
	اگر (verbosity >= 1)
		strvec_افزودن(argv, "-v");
	وگرنه اگر (verbosity < 0)
		strvec_افزودن(argv, "-q");
	اگر (family == TRANSPORT_FAMILY_IPV4)
		strvec_افزودن(argv, "--ipv4");
	وگرنه اگر (family == TRANSPORT_FAMILY_IPV6)
		strvec_افزودن(argv, "--ipv6");
	اگر (!write_fetch_head)
		strvec_افزودن(argv, "--no-write-fetch-head");
	اگر (config->display_format == DISPLAY_FORMAT_PORCELAIN)
		strvec_pushf(argv, "--porcelain");
}

/* Fetch multiple remotes in parallel */

ساختار parallel_fetch_state {
	ثابت نویسه **argv;
	ساختار string_list *remotes;
	صحیح next, result;
	ثابت ساختار fetch_config *config;
};

ایستا صحیح fetch_next_remote(ساختار child_process *cp,
			     ساختار strbuf *out UNUSED,
			     تهی *cb, تهی **task_cb)
{
	ساختار parallel_fetch_state *state = cb;
	نویسه *remote;

	اگر (state->next < 0 || state->next >= state->remotes->nr)
		بازگشت 0;

	remote = state->remotes->اقلام[state->next++].string;
	*task_cb = remote;

	strvec_pushv(&cp->args, state->argv);
	strvec_افزودن(&cp->args, remote);
	cp->git_cmd = 1;

	اگر (verbosity >= 0 && state->config->display_format != DISPLAY_FORMAT_PORCELAIN)
		printf(_("Fetching %s\n"), remote);

	بازگشت 1;
}

ایستا صحیح fetch_failed_to_start(ساختار strbuf *out UNUSED,
				 تهی *cb, تهی *task_cb)
{
	ساختار parallel_fetch_state *state = cb;
	ثابت نویسه *remote = task_cb;

	state->result = error(_("could not fetch %s"), remote);

	بازگشت 0;
}

ایستا صحیح fetch_finished(صحیح result, ساختار strbuf *out,
			  تهی *cb, تهی *task_cb)
{
	ساختار parallel_fetch_state *state = cb;
	ثابت نویسه *remote = task_cb;

	اگر (result) {
		strbuf_addf(out, _("could not fetch '%s' (exit code: %d)\n"),
			    remote, result);
		state->result = -1;
	}

	بازگشت 0;
}

ایستا صحیح fetch_multiple(ساختار string_list *list, صحیح max_children,
			  ثابت ساختار fetch_config *config)
{
	صحیح i, result = 0;
	ساختار strvec argv = STRVEC_INIT;

	اگر (!append && write_fetch_head) {
		صحیح errcode = truncate_fetch_head();
		اگر (errcode)
			بازگشت errcode;
	}

	/*
	 * Cancel out the fetch.bundleURI config when running subprocesses,
	 * to avoid fetching from the same bundle list multiple times.
	 */
	strvec_pushl(&argv, "-c", "fetch.bundleURI=",
		     "fetch", "--append", "--no-auto-gc",
		     "--no-write-commit-graph", پوچ);
	برای (i = 0; i < server_options.nr; i++)
		strvec_pushf(&argv, "--server-option=%s", server_options.اقلام[i].string);
	add_options_to_argv(&argv, config);

	اگر (max_children != 1 && list->nr != 1) {
		ساختار parallel_fetch_state state = { argv.v, list, 0, 0, config };
		ثابت ساختار run_process_parallel_opts opts = {
			.tr2_category = "fetch",
			.tr2_label = "parallel/fetch",

			.processes = max_children,

			.get_next_task = &fetch_next_remote,
			.start_failure = &fetch_failed_to_start,
			.task_finished = &fetch_finished,
			.data = &state,
		};

		strvec_افزودن(&argv, "--end-of-options");

		run_processes_parallel(&opts);
		result = state.result;
	} وگرنه
		برای (i = 0; i < list->nr; i++) {
			ثابت نویسه *name = list->اقلام[i].string;
			ساختار child_process cmd = CHILD_PROCESS_INIT;

			strvec_pushv(&cmd.args, argv.v);
			strvec_افزودن(&cmd.args, name);
			اگر (verbosity >= 0 && config->display_format != DISPLAY_FORMAT_PORCELAIN)
				printf(_("Fetching %s\n"), name);
			cmd.git_cmd = 1;
			اگر (run_command(&cmd)) {
				error(_("could not fetch %s"), name);
				result = 1;
			}
		}

	strvec_clear(&argv);
	بازگشت !!result;
}

/*
 * Fetching from the promisor remote should use the given filter-spec
 * or inherit the default filter-spec from the config.
 */
ایستا درونخطی تهی fetch_one_setup_partial(ساختار remote *remote,
					   ساختار list_objects_filter_options *filter_options)
{
	/*
	 * Explicit --no-filter argument overrides everything, regardless
	 * of any prior partial clones and fetches.
	 */
	اگر (filter_options->no_filter)
		بازگشت;

	/*
	 * If no prior partial clone/fetch and the current fetch DID NOT
	 * request a partial-fetch, do a normal fetch.
	 */
	اگر (!repo_has_promisor_remote(the_repository) && !filter_options->choice)
		بازگشت;

	/*
	 * If this is a partial-fetch request, we enable partial on
	 * this repo if not already enabled and remember the given
	 * filter-spec as the default for subsequent fetches to this
	 * remote if there is currently no default filter-spec.
	 */
	اگر (filter_options->choice) {
		partial_clone_register(remote->name, filter_options);
		بازگشت;
	}

	/*
	 * Do a partial-fetch from the promisor remote using either the
	 * explicitly given filter-spec or inherit the filter-spec from
	 * the config.
	 */
	اگر (!filter_options->choice)
		partial_clone_get_default_filter_spec(filter_options, remote->name);
	بازگشت;
}

ایستا صحیح fetch_one(ساختار remote *remote, صحیح argc, ثابت نویسه **argv,
		     صحیح prune_tags_ok, صحیح use_stdin_refspecs,
		     ثابت ساختار fetch_config *config,
		     ساختار list_objects_filter_options *filter_options,
		     ساختار oidset *updated_tips)
{
	ساختار refspec rs = REFSPEC_INIT_FETCH(the_hash_algo);
	صحیح i;
	صحیح exit_code;
	صحیح maybe_prune_tags;
	صحیح remote_via_config = remote_is_configured(remote, 0);

	اگر (!remote)
		die(_("no remote repository specified; please specify either a URL or a\n"
		      "remote name from which new revisions should be fetched"));

	gtransport = prepare_transport(remote, 1, filter_options);

	اگر (prune < 0) {
		/* no command line request */
		اگر (0 <= remote->prune)
			prune = remote->prune;
		وگرنه اگر (0 <= config->prune)
			prune = config->prune;
		وگرنه
			prune = PRUNE_BY_DEFAULT;
	}

	اگر (prune_tags < 0) {
		/* no command line request */
		اگر (0 <= remote->prune_tags)
			prune_tags = remote->prune_tags;
		وگرنه اگر (0 <= config->prune_tags)
			prune_tags = config->prune_tags;
		وگرنه
			prune_tags = PRUNE_TAGS_BY_DEFAULT;
	}

	maybe_prune_tags = prune_tags_ok && prune_tags;
	اگر (maybe_prune_tags && remote_via_config)
		refspec_append(&remote->fetch, TAG_REFSPEC);

	اگر (maybe_prune_tags && (argc || !remote_via_config))
		refspec_append(&rs, TAG_REFSPEC);

	برای (i = 0; i < argc; i++) {
		اگر (!strcmp(argv[i], "tag")) {
			i++;
			اگر (i >= argc)
				die(_("you need to specify a tag name"));

			refspec_appendf(&rs, "refs/tags/%s:refs/tags/%s",
					argv[i], argv[i]);
		} وگرنه {
			refspec_append(&rs, argv[i]);
		}
	}

	اگر (use_stdin_refspecs) {
		ساختار strbuf سطر = STRBUF_INIT;
		وقتی (strbuf_getline_lf(&سطر, stdin) != EOF)
			refspec_append(&rs, سطر.buf);
		strbuf_release(&سطر);
	}

	اگر (server_options.nr)
		gtransport->server_options = &server_options;

	sigchain_افزودن_common(unlock_pack_on_signal);
	atexit(unlock_pack_atexit);
	sigchain_افزودن(SIGPIPE, SIG_IGN);
	exit_code = do_fetch(gtransport, &rs, config, filter_options,
			     updated_tips);
	sigchain_برداشت(SIGPIPE);
	refspec_clear(&rs);
	transport_disconnect(gtransport);
	gtransport = پوچ;
	بازگشت exit_code;
}

صحیح cmd_fetch(صحیح argc,
	      ثابت نویسه **argv,
	      ثابت نویسه *prefix,
	      ساختار repository *repo UNUSED)
{
	ساختار fetch_config config = {
		.display_format = DISPLAY_FORMAT_FULL,
		.follow_remote_head_raw = پوچ,
		.follow_remote_head_seen = 0,
		.prune = -1,
		.prune_tags = -1,
		.show_forced_updates = 1,
		.recurse_submodules = RECURSE_SUBMODULES_DEFAULT,
		.parallel = 1,
		.submodule_fetch_jobs = -1,
	};
	ثابت نویسه *submodule_prefix = "";
	ثابت نویسه *bundle_uri;
	ساختار string_list list = STRING_LIST_INIT_DUP;
	ساختار list_objects_filter_options filter_options = LIST_OBJECTS_FILTER_INIT;
	ساختار remote *remote = پوچ;
	صحیح all = -1, multiple = 0;
	صحیح result = 0;
	صحیح prune_tags_ok = 1;
	صحیح enable_auto_gc = 1;
	صحیح unshallow = 0;
	صحیح max_jobs = -1;
	صحیح recurse_submodules_cli = RECURSE_SUBMODULES_DEFAULT;
	صحیح recurse_submodules_default = RECURSE_SUBMODULES_ON_DEMAND;
	صحیح fetch_write_commit_graph = -1;
	صحیح stdin_refspecs = 0;
	صحیح negotiate_only = 0;
	صحیح porcelain = 0;
	صحیح i;
	شمارش {
		GRAPH_WRITE_REACHABLE,
		GRAPH_WRITE_TIPS,
		GRAPH_WRITE_SKIP,
	} graph_write_mode = GRAPH_WRITE_REACHABLE;
	ساختار oidset updated_tips = OIDSET_INIT;

	ساختار option builtin_fetch_options[] = {
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
		OPT_SET_INT('n', پوچ, &tags,
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
			.مقدار = &submodule_prefix,
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
	برای (i = 1; i < argc; i++) {
		/* This handles non-URLs gracefully */
		نویسه *anon = transport_anonymize_url(argv[i]);

		strbuf_addf(&default_rla, " %s", anon);
		آزاد(anon);
	}

	repo_config(the_repository, git_fetch_config, &config);
	اگر (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_full_index = 0;
	}

	argc = parse_options(argc, argv, prefix,
			     builtin_fetch_options, builtin_fetch_usage, 0);

	اگر (recurse_submodules_cli != RECURSE_SUBMODULES_DEFAULT)
		config.recurse_submodules = recurse_submodules_cli;

	اگر (negotiate_only) {
		انتخاب (recurse_submodules_cli) {
		حالت RECURSE_SUBMODULES_OFF:
		حالت RECURSE_SUBMODULES_DEFAULT:
			/*
			 * --negotiate-only should never recurse into
			 * submodules. Skip it by setting recurse_submodules to
			 * RECURSE_SUBMODULES_OFF.
			 */
			config.recurse_submodules = RECURSE_SUBMODULES_OFF;
			بشکن;

		پیشفرض:
			die(_("options '%s' and '%s' cannot be used together"),
			    "--negotiate-only", "--recurse-submodules");
		}
	}

	اگر (config.recurse_submodules != RECURSE_SUBMODULES_OFF) {
		صحیح *sfjc = config.submodule_fetch_jobs == -1
			    ? &config.submodule_fetch_jobs : پوچ;
		صحیح *rs = config.recurse_submodules == RECURSE_SUBMODULES_DEFAULT
			  ? &config.recurse_submodules : پوچ;

		fetch_config_from_gitmodules(the_repository, sfjc, rs);
	}


	اگر (porcelain) {
		انتخاب (recurse_submodules_cli) {
		حالت RECURSE_SUBMODULES_OFF:
		حالت RECURSE_SUBMODULES_DEFAULT:
			/*
			 * Reference updates in submodules would be ambiguous
			 * in porcelain mode, so we reject this combination.
			 */
			config.recurse_submodules = RECURSE_SUBMODULES_OFF;
			بشکن;

		پیشفرض:
			die(_("options '%s' and '%s' cannot be used together"),
			    "--porcelain", "--recurse-submodules");
		}

		config.display_format = DISPLAY_FORMAT_PORCELAIN;
	}

	اگر (deepen_relative) {
		اگر (deepen_relative < 0)
			die(_("negative depth in --deepen is not supported"));
		اگر (depth)
			die(_("options '%s' and '%s' cannot be used together"), "--deepen", "--depth");
		depth = xstrfmt("%d", deepen_relative);
	}
	اگر (unshallow) {
		اگر (depth)
			die(_("options '%s' and '%s' cannot be used together"), "--depth", "--unshallow");
		وگرنه اگر (!is_repository_shallow(the_repository))
			die(_("--unshallow on a complete repository does not make sense"));
		وگرنه
			depth = xstrfmt("%d", INFINITE_DEPTH);
	}

	/* no need to be strict, transport_set_option() will validate it again */
	اگر (depth && atoi(depth) < 1)
		die(_("depth %s is not a positive number"), depth);
	اگر (depth || deepen_since || deepen_not.nr)
		deepen = 1;

	/* FETCH_HEAD never gets updated in --dry-run mode */
	اگر (dry_run)
		write_fetch_head = 0;

	اگر (!max_jobs)
		max_jobs = online_cpus();

	اگر (!repo_config_get_string_tmp(the_repository, "fetch.bundleuri", &bundle_uri) &&
	    fetch_bundle_uri(the_repository, bundle_uri, پوچ))
		warning(_("failed to fetch bundles from '%s'"), bundle_uri);

	اگر (all < 0) {
		/*
		 * no --[no-]all given;
		 * only use config option if no remote was explicitly specified
		 */
		all = (!argc) ? config.all : 0;
	}

	اگر (all) {
		اگر (argc == 1)
			die(_("fetch --all does not take a repository argument"));
		وگرنه اگر (argc > 1)
			die(_("fetch --all does not make sense with refspecs"));

		(تهی) for_each_remote(get_one_remote_for_fetch, &list);

		/* do not do fetch_multiple() of one */
		اگر (list.nr == 1)
			remote = remote_get(list.اقلام[0].string);
	} وگرنه اگر (argc == 0) {
		/* No arguments -- use default remote */
		remote = remote_get(پوچ);
	} وگرنه اگر (multiple) {
		/* All arguments are assumed to be remotes or groups */
		برای (i = 0; i < argc; i++)
			اگر (!add_remote_or_group(argv[i], &list))
				die(_("no such remote or remote group: %s"),
				    argv[i]);
	} وگرنه {
		/* Single remote or group */
		(تهی) add_remote_or_group(argv[0], &list);
		اگر (list.nr > 1) {
			/* More than one remote */
			اگر (argc > 1)
				die(_("fetching a group and specifying refspecs does not make sense"));
		} وگرنه {
			/* Zero or one remotes */
			remote = remote_get(argv[0]);
			prune_tags_ok = (argc == 1);
			argc--;
			argv++;
		}
	}
	string_list_remove_duplicates(&list, 0);

	اگر (negotiate_only) {
		ساختار oidset acked_commits = OIDSET_INIT;
		ساختار oidset_iter iter;
		ثابت ساختار object_id *oid;

		trace2_region_enter("fetch", "negotiate-only", the_repository);
		اگر (!remote)
			die(_("must supply remote when using --negotiate-only"));
		gtransport = prepare_transport(remote, 1, &filter_options);

		اگر (!gtransport->smart_options) {
			warning(_("protocol does not support --negotiate-only, exiting"));
			result = 1;
			trace2_region_leave("fetch", "negotiate-only", the_repository);
			برو cleanup;
		}
		اگر (!gtransport->smart_options->negotiation_restrict_tips)
			die(_("%s needs one or more %s"), "--negotiate-only",
			    "--negotiation-restrict=*");

		gtransport->smart_options->acked_commits = &acked_commits;

		اگر (server_options.nr)
			gtransport->server_options = &server_options;
		result = transport_fetch_refs(gtransport, پوچ);
		gtransport->smart_options->acked_commits = پوچ;

		oidset_iter_init(&acked_commits, &iter);
		وقتی ((oid = oidset_iter_next(&iter)))
			printf("%s\n", oid_to_hex(oid));
		oidset_clear(&acked_commits);
		trace2_region_leave("fetch", "negotiate-only", the_repository);
	} وگرنه اگر (remote) {
		اگر (filter_options.choice || repo_has_promisor_remote(the_repository)) {
			trace2_region_enter("fetch", "setup-partial", the_repository);
			fetch_one_setup_partial(remote, &filter_options);
			trace2_region_leave("fetch", "setup-partial", the_repository);
		}
		trace2_region_enter("fetch", "fetch-one", the_repository);
		result = fetch_one(remote, argc, argv, prune_tags_ok, stdin_refspecs,
				   &config, &filter_options, &updated_tips);
		اگر (prepare_commit_graph(the_repository)) {
			اگر (oidset_size(&updated_tips))
				graph_write_mode = GRAPH_WRITE_TIPS;
			وگرنه
				graph_write_mode = GRAPH_WRITE_SKIP;
		}
		trace2_region_leave("fetch", "fetch-one", the_repository);
	} وگرنه {
		صحیح max_children = max_jobs;

		اگر (filter_options.choice)
			die(_("--filter can only be used with the remote "
			      "configured in extensions.partialclone"));

		اگر (atomic_fetch)
			die(_("--atomic can only be used when fetching "
			      "from one remote"));

		اگر (stdin_refspecs)
			die(_("--stdin can only be used when fetching "
			      "from one remote"));

		اگر (max_children < 0)
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
	اگر (!result && remote && (config.recurse_submodules != RECURSE_SUBMODULES_OFF)) {
		ساختار strvec options = STRVEC_INIT;
		صحیح max_children = max_jobs;

		اگر (max_children < 0)
			max_children = config.submodule_fetch_jobs;
		اگر (max_children < 0)
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
	اگر (negotiate_only)
		برو cleanup;

	prepare_repo_settings(the_repository);
	اگر (fetch_write_commit_graph > 0 ||
	    (fetch_write_commit_graph < 0 &&
	     the_repository->settings.fetch_write_commit_graph)) {
		صحیح commit_graph_flags = COMMIT_GRAPH_WRITE_SPLIT;

		اگر (progress)
			commit_graph_flags |= COMMIT_GRAPH_WRITE_PROGRESS;

		اگر (graph_write_mode != GRAPH_WRITE_SKIP) {
			trace2_region_enter("fetch", "write-commit-graph",
					    the_repository);
			اگر (graph_write_mode == GRAPH_WRITE_TIPS)
				write_commit_graph(
					the_repository,
					the_repository->objects->source->path,
					پوچ, &updated_tips,
					commit_graph_flags, پوچ);
			وگرنه
				write_commit_graph_reachable(
					the_repository,
					the_repository->objects->source->path,
					commit_graph_flags, پوچ);
			trace2_region_leave("fetch", "write-commit-graph",
					    the_repository);
		}
	}

	اگر (enable_auto_gc) {
		اگر (refetch) {
			/*
			 * Hint auto-maintenance strongly to encourage repacking,
			 * but respect config settings disabling it.
			 */
			صحیح opt_val;
			اگر (repo_config_get_int(the_repository, "gc.autopacklimit", &opt_val))
				opt_val = -1;
			اگر (opt_val != 0)
				git_config_افزودن_parameter("gc.autoPackLimit=1");

			اگر (repo_config_get_int(the_repository, "maintenance.incremental-repack.auto", &opt_val))
				opt_val = -1;
			اگر (opt_val != 0)
				git_config_افزودن_parameter("maintenance.incremental-repack.auto=-1");
		}
		run_auto_maintenance(the_repository, verbosity < 0);
	}

 cleanup:
	oidset_clear(&updated_tips);
	string_list_clear(&list, 0);
	list_objects_filter_release(&filter_options);
	آزاد(config.follow_remote_head_raw);
	بازگشت result;
}
