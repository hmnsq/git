/*
 * "git fetch"
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "advice.h"
#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "environment.h"
#અન્તર્ભાવ "hex.h"
#અન્તર્ભાવ "refs.h"
#અન્તર્ભાવ "refspec.h"
#અન્તર્ભાવ "object-name.h"
#અન્તર્ભાવ "odb.h"
#અન્તર્ભાવ "odb/source.h"
#અન્તર્ભાવ "oidset.h"
#અન્તર્ભાવ "oid-array.h"
#અન્તર્ભાવ "commit.h"
#અન્તર્ભાવ "string-list.h"
#અન્તર્ભાવ "remote.h"
#અન્તર્ભાવ "transport.h"
#અન્તર્ભાવ "run-command.h"
#અન્તર્ભાવ "parse-options.h"
#અન્તર્ભાવ "sigchain.h"
#અન્તર્ભાવ "submodule-config.h"
#અન્તર્ભાવ "submodule.h"
#અન્તર્ભાવ "connected.h"
#અન્તર્ભાવ "strvec.h"
#અન્તર્ભાવ "utf8.h"
#અન્તર્ભાવ "pager.h"
#અન્તર્ભાવ "path.h"
#અન્તર્ભાવ "pkt-line.h"
#અન્તર્ભાવ "list-objects-filter-options.h"
#અન્તર્ભાવ "commit-reach.h"
#અન્તર્ભાવ "branch.h"
#અન્તર્ભાવ "promisor-remote.h"
#અન્તર્ભાવ "commit-graph.h"
#અન્તર્ભાવ "shallow.h"
#અન્તર્ભાવ "trace.h"
#અન્તર્ભાવ "trace2.h"
#અન્તર્ભાવ "bundle-uri.h"

#define FORCED_UPDATES_DELAY_WARNING_IN_MS (10 * 1000)

સ્થિર અચલ અક્ષર * અચલ builtin_fetch_usage[] = {
	N_("git fetch [<options>] [<repository> [<refspec>...]]"),
	N_("git fetch [<options>] <group>"),
	N_("git fetch --multiple [<options>] [(<repository>|<group>)...]"),
	N_("git fetch --all [<options>]"),
	ખાલી
};

ગણના {
	TAGS_UNSET = 0,
	TAGS_DEFAULT = 1,
	TAGS_SET = 2
};

ગણના display_format {
	DISPLAY_FORMAT_FULL,
	DISPLAY_FORMAT_COMPACT,
	DISPLAY_FORMAT_PORCELAIN,
};

સંરચના display_state {
	સંરચના strbuf buf;

	પૂર્ણાંક refcol_width;
	ગણના display_format format;

	અક્ષર *url;
	પૂર્ણાંક url_len, shown_url;
};

સ્થિર uint64_t forced_updates_ms = 0;
સ્થિર પૂર્ણાંક prefetch = 0;
સ્થિર પૂર્ણાંક prune = -1; /* unspecified */
#define PRUNE_BY_DEFAULT 0 /* do we prune by default? */

સ્થિર પૂર્ણાંક prune_tags = -1; /* unspecified */
#define PRUNE_TAGS_BY_DEFAULT 0 /* do we prune tags by default? */

સ્થિર પૂર્ણાંક append, dry_run, force, keep, update_head_ok;
સ્થિર પૂર્ણાંક write_fetch_head = 1;
સ્થિર પૂર્ણાંક verbosity, deepen_relative, set_upstream, refetch;
સ્થિર પૂર્ણાંક progress = -1;
સ્થિર પૂર્ણાંક tags = TAGS_DEFAULT, update_shallow, deepen;
સ્થિર પૂર્ણાંક atomic_fetch;
સ્થિર ગણના transport_family family;
સ્થિર અચલ અક્ષર *depth;
સ્થિર અચલ અક્ષર *deepen_since;
સ્થિર અચલ અક્ષર *upload_pack;
સ્થિર સંરચના string_list deepen_not = STRING_LIST_INIT_NODUP;
સ્થિર સંરચના strbuf default_rla = STRBUF_INIT;
સ્થિર સંરચના transport *gtransport;
સ્થિર સંરચના transport *gsecondary;
સ્થિર સંરચના refspec refmap;
સ્થિર સંરચના string_list server_options = STRING_LIST_INIT_DUP;
સ્થિર સંરચના string_list negotiation_restrict = STRING_LIST_INIT_NODUP;
સ્થિર સંરચના string_list negotiation_include = STRING_LIST_INIT_NODUP;

સંરચના fetch_config {
	ગણના display_format display_format;
	અક્ષર *follow_remote_head_raw;
	પૂર્ણાંક follow_remote_head_seen;
	પૂર્ણાંક all;
	પૂર્ણાંક prune;
	પૂર્ણાંક prune_tags;
	પૂર્ણાંક show_forced_updates;
	પૂર્ણાંક recurse_submodules;
	પૂર્ણાંક parallel;
	પૂર્ણાંક submodule_fetch_jobs;
};

સ્થિર પૂર્ણાંક git_fetch_config(અચલ અક્ષર *k, અચલ અક્ષર *v,
			    અચલ સંરચના config_context *ctx, શૂન્ય *cb)
{
	સંરચના fetch_config *fetch_config = cb;

	જો (!strcmp(k, "fetch.all")) {
		fetch_config->all = git_config_bool(k, v);
		પાછા 0;
	}

	જો (!strcmp(k, "fetch.prune")) {
		fetch_config->prune = git_config_bool(k, v);
		પાછા 0;
	}

	જો (!strcmp(k, "fetch.prunetags")) {
		fetch_config->prune_tags = git_config_bool(k, v);
		પાછા 0;
	}

	જો (!strcmp(k, "fetch.showforcedupdates")) {
		fetch_config->show_forced_updates = git_config_bool(k, v);
		પાછા 0;
	}

	જો (!strcmp(k, "submodule.recurse")) {
		પૂર્ણાંક r = git_config_bool(k, v) ?
			RECURSE_SUBMODULES_ON : RECURSE_SUBMODULES_OFF;
		fetch_config->recurse_submodules = r;
		પાછા 0;
	}

	જો (!strcmp(k, "submodule.fetchjobs")) {
		fetch_config->submodule_fetch_jobs = parse_submodule_fetchjobs(k, v, ctx->kvi);
		પાછા 0;
	} નહીંતર જો (!strcmp(k, "fetch.recursesubmodules")) {
		fetch_config->recurse_submodules = parse_fetch_recurse_submodules_arg(k, v);
		પાછા 0;
	}

	જો (!strcmp(k, "fetch.parallel")) {
		fetch_config->parallel = git_config_int(k, v, ctx->kvi);
		જો (fetch_config->parallel < 0)
			die(_("fetch.parallel cannot be negative"));
		જો (!fetch_config->parallel)
			fetch_config->parallel = online_cpus();
		પાછા 0;
	}

	જો (!strcmp(k, "fetch.output")) {
		જો (!v)
			પાછા config_error_nonbool(k);
		નહીંતર જો (!strcasecmp(v, "full"))
			fetch_config->display_format = DISPLAY_FORMAT_FULL;
		નહીંતર જો (!strcasecmp(v, "compact"))
			fetch_config->display_format = DISPLAY_FORMAT_COMPACT;
		નહીંતર
			die(_("invalid value for '%s': '%s'"),
			    "fetch.output", v);
		પાછા 0;
	}

	જો (!strcmp(k, "fetch.followremotehead")) {
		મુક્ત(fetch_config->follow_remote_head_raw);
		fetch_config->follow_remote_head_raw = xstrdup_or_null(v);
		fetch_config->follow_remote_head_seen = 1;
		પાછા 0;
	}

	પાછા git_default_config(k, v, ctx, cb);
}

સ્થિર ગણના follow_remote_head_settings get_follow_remote_head(અચલ અક્ષર *setting)
{
	જો (!setting)
		die(_("missing value for 'fetch.followRemoteHEAD'"));
	નહીંતર જો (!strcmp(setting, "never"))
		પાછા FOLLOW_REMOTE_NEVER;
	નહીંતર જો (!strcmp(setting, "create"))
		પાછા FOLLOW_REMOTE_CREATE;
	નહીંતર જો (!strcmp(setting, "warn"))
		પાછા FOLLOW_REMOTE_WARN;
	નહીંતર જો (!strcmp(setting, "always"))
		પાછા FOLLOW_REMOTE_ALWAYS;
	warning(_("unrecognized fetch.followRemoteHEAD value '%s' ignored"), setting);
	પાછા BUILTIN_FOLLOW_REMOTE_HEAD_DFLT;
}

સ્થિર પૂર્ણાંક parse_refmap_arg(અચલ સંરચના option *opt, અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	BUG_ON_OPT_NEG(unset);

	/*
	 * "git fetch --refmap='' origin foo"
	 * can be used to tell the command not to store anywhere
	 */
	refspec_append(opt->કિંમત, arg);

	પાછા 0;
}

સ્થિર શૂન્ય unlock_pack(unsigned પૂર્ણાંક flags)
{
	જો (gtransport)
		transport_unlock_pack(gtransport, flags);
	જો (gsecondary)
		transport_unlock_pack(gsecondary, flags);
}

સ્થિર શૂન્ય unlock_pack_atexit(શૂન્ય)
{
	unlock_pack(0);
}

સ્થિર શૂન્ય unlock_pack_on_signal(પૂર્ણાંક signo)
{
	unlock_pack(TRANSPORT_UNLOCK_PACK_IN_SIGNAL_HANDLER);
	sigchain_કાઢો(signo);
	ઉઠાવો(signo);
}

સ્થિર શૂન્ય add_merge_config(સંરચના ref **head,
			   અચલ સંરચના ref *remote_refs,
		           સંરચના branch *branch,
		           સંરચના ref ***tail)
{
	પૂર્ણાંક i;

	માટે (i = 0; i < branch->merge_nr; i++) {
		સંરચના ref *rm, **old_tail = *tail;
		સંરચના refspec_item refspec;

		માટે (rm = *head; rm; rm = rm->next) {
			જો (branch_merge_matches(branch, i, rm->name)) {
				rm->fetch_head_status = FETCH_HEAD_MERGE;
				તોડો;
			}
		}
		જો (rm)
			ચાલુ;

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
		memset(&refspec, 0, કદ(refspec));
		refspec.src = branch->merge[i]->src;
		get_fetch_map(remote_refs, &refspec, tail, 1);
		માટે (rm = *old_tail; rm; rm = rm->next)
			rm->fetch_head_status = FETCH_HEAD_MERGE;
	}
}

સ્થિર શૂન્ય create_fetch_oidset(સંરચના ref **head, સંરચના oidset *out)
{
	સંરચના ref *rm = *head;
	જ્યારે (rm) {
		oidset_ઉમેરો(out, &rm->old_oid);
		rm = rm->next;
	}
}

સંરચના refname_hash_entry {
	સંરચના hashmap_entry ent;
	સંરચના object_id oid;
	પૂર્ણાંક ignore;
	અક્ષર refname[FLEX_ARRAY];
};

સ્થિર પૂર્ણાંક refname_hash_entry_cmp(અચલ શૂન્ય *hashmap_cmp_fn_data UNUSED,
				  અચલ સંરચના hashmap_entry *eptr,
				  અચલ સંરચના hashmap_entry *entry_or_ચાવી,
				  અચલ શૂન્ય *keydata)
{
	અચલ સંરચના refname_hash_entry *e1, *e2;

	e1 = container_of(eptr, અચલ સંરચના refname_hash_entry, ent);
	e2 = container_of(entry_or_ચાવી, અચલ સંરચના refname_hash_entry, ent);
	પાછા strcmp(e1->refname, keydata ? keydata : e2->refname);
}

સ્થિર સંરચના refname_hash_entry *refname_hash_add(સંરચના hashmap *map,
						   અચલ અક્ષર *refname,
						   અચલ સંરચના object_id *oid)
{
	સંરચના refname_hash_entry *ent;
	size_t len = strlen(refname);

	FLEX_ALLOC_MEM(ent, refname, refname, len);
	hashmap_entry_init(&ent->ent, strhash(refname));
	oidcpy(&ent->oid, oid);
	hashmap_add(map, &ent->ent);
	પાછા ent;
}

સ્થિર પૂર્ણાંક add_one_refname(અચલ સંરચના reference *ref, શૂન્ય *cbdata)
{
	સંરચના hashmap *refname_map = cbdata;

	(શૂન્ય) refname_hash_add(refname_map, ref->name, ref->oid);
	પાછા 0;
}

સ્થિર શૂન્ય refname_hash_init(સંરચના hashmap *map)
{
	hashmap_init(map, refname_hash_entry_cmp, ખાલી, 0);
}

સ્થિર પૂર્ણાંક refname_hash_exists(સંરચના hashmap *map, અચલ અક્ષર *refname)
{
	પાછા !!hashmap_get_from_hash(map, strhash(refname), refname);
}

સ્થિર શૂન્ય clear_item(સંરચના refname_hash_entry *item)
{
	item->ignore = 1;
}


સ્થિર શૂન્ય add_already_queued_tags(અચલ અક્ષર *refname,
				    અચલ સંરચના object_id *old_oid UNUSED,
				    અચલ સંરચના object_id *new_oid,
				    શૂન્ય *cb_data)
{
	સંરચના hashmap *queued_tags = cb_data;
	જો (starts_with(refname, "refs/tags/") && new_oid)
		(શૂન્ય) refname_hash_add(queued_tags, refname, new_oid);
}

સ્થિર શૂન્ય find_non_local_tags(અચલ સંરચના ref *refs,
				સંરચના ref_transaction *transaction,
				સંરચના ref **head,
				સંરચના ref ***tail)
{
	સંરચના hashmap existing_refs;
	સંરચના hashmap remote_refs;
	સંરચના oidset fetch_oids = OIDSET_INIT;
	સંરચના string_list remote_refs_list = STRING_LIST_INIT_NODUP;
	સંરચના string_list_item *remote_ref_item;
	અચલ સંરચના ref *ref;
	સંરચના refname_hash_entry *item = ખાલી;

	refname_hash_init(&existing_refs);
	refname_hash_init(&remote_refs);
	create_fetch_oidset(head, &fetch_oids);

	refs_for_each_ref(get_main_ref_store(the_repository), add_one_refname,
			  &existing_refs);

	/*
	 * If we already have a transaction, then we need to filter out all
	 * tags which have already been queued up.
	 */
	જો (transaction)
		ref_transaction_for_each_queued_update(transaction,
						       add_already_queued_tags,
						       &existing_refs);

	માટે (ref = refs; ref; ref = ref->next) {
		જો (!starts_with(ref->name, "refs/tags/"))
			ચાલુ;

		/*
		 * The peeled ref always follows the matching base
		 * ref, so if we see a peeled ref that we don't want
		 * to fetch then we can mark the ref entry in the list
		 * as one to ignore by setting util to NULL.
		 */
		જો (ends_with(ref->name, "^{}")) {
			જો (item &&
			    !odb_has_object(the_repository->objects, &ref->old_oid, 0) &&
			    !oidset_contains(&fetch_oids, &ref->old_oid) &&
			    !odb_has_object(the_repository->objects, &item->oid, 0) &&
			    !oidset_contains(&fetch_oids, &item->oid))
				clear_item(item);
			item = ખાલી;
			ચાલુ;
		}

		/*
		 * If item is non-NULL here, then we previously saw a
		 * ref not followed by a peeled reference, so we need
		 * to check if it is a lightweight tag that we want to
		 * fetch.
		 */
		જો (item &&
		    !odb_has_object(the_repository->objects, &item->oid, 0) &&
		    !oidset_contains(&fetch_oids, &item->oid))
			clear_item(item);

		item = ખાલી;

		/* skip duplicates and refs that we already have */
		જો (refname_hash_exists(&remote_refs, ref->name) ||
		    refname_hash_exists(&existing_refs, ref->name))
			ચાલુ;

		item = refname_hash_add(&remote_refs, ref->name, &ref->old_oid);
		string_list_ઉમેરો(&remote_refs_list, ref->name);
	}
	hashmap_clear_and_free(&existing_refs, સંરચના refname_hash_entry, ent);

	/*
	 * We may have a final lightweight tag that needs to be
	 * checked to see if it needs fetching.
	 */
	જો (item &&
	    !odb_has_object(the_repository->objects, &item->oid, 0) &&
	    !oidset_contains(&fetch_oids, &item->oid))
		clear_item(item);

	/*
	 * For all the tags in the remote_refs_list,
	 * add them to the list of refs to be fetched
	 */
	for_each_string_list_item(remote_ref_item, &remote_refs_list) {
		અચલ અક્ષર *refname = remote_ref_item->string;
		સંરચના ref *rm;
		unsigned પૂર્ણાંક hash = strhash(refname);

		item = hashmap_get_entry_from_hash(&remote_refs, hash, refname,
					સંરચના refname_hash_entry, ent);
		જો (!item)
			BUG("unseen remote ref?");

		/* Unless we have already decided to ignore this item... */
		જો (item->ignore)
			ચાલુ;

		rm = alloc_ref(item->refname);
		rm->peer_ref = alloc_ref(item->refname);
		oidcpy(&rm->old_oid, &item->oid);
		**tail = rm;
		*tail = &rm->next;
	}
	hashmap_clear_and_free(&remote_refs, સંરચના refname_hash_entry, ent);
	string_list_clear(&remote_refs_list, 0);
	oidset_clear(&fetch_oids);
}

સ્થિર શૂન્ય filter_prefetch_refspec(સંરચના refspec *rs)
{
	પૂર્ણાંક i;

	જો (!prefetch)
		પાછા;

	માટે (i = 0; i < rs->nr; i++) {
		સંરચના strbuf new_dst = STRBUF_INIT;
		અક્ષર *old_dst;
		અચલ અક્ષર *sub = ખાલી;

		જો (rs->વસ્તુઓ[i].negative)
			ચાલુ;
		જો (!rs->વસ્તુઓ[i].dst ||
		    (rs->વસ્તુઓ[i].src &&
		     starts_with(rs->વસ્તુઓ[i].src,
				 ref_namespace[NAMESPACE_TAGS].ref))) {
			પૂર્ણાંક j;

			refspec_item_clear(&rs->વસ્તુઓ[i]);

			માટે (j = i + 1; j < rs->nr; j++)
				rs->વસ્તુઓ[j - 1] = rs->વસ્તુઓ[j];
			rs->nr--;
			i--;
			ચાલુ;
		}

		old_dst = rs->વસ્તુઓ[i].dst;
		strbuf_addstr(&new_dst, ref_namespace[NAMESPACE_PREFETCH].ref);

		/*
		 * If old_dst starts with "refs/", then place
		 * sub after that prefix. Otherwise, start at
		 * the beginning of the string.
		 */
		જો (!skip_prefix(old_dst, "refs/", &sub))
			sub = old_dst;
		strbuf_addstr(&new_dst, sub);

		rs->વસ્તુઓ[i].dst = strbuf_detach(&new_dst, ખાલી);
		rs->વસ્તુઓ[i].force = 1;

		મુક્ત(old_dst);
	}
}

સ્થિર સંરચના ref *get_ref_map(સંરચના remote *remote,
			       અચલ સંરચના ref *remote_refs,
			       સંરચના refspec *rs,
			       પૂર્ણાંક tags, પૂર્ણાંક *autotags)
{
	પૂર્ણાંક i;
	સંરચના ref *rm;
	સંરચના ref *ref_map = ખાલી;
	સંરચના ref **tail = &ref_map;

	/* opportunistically-updated references: */
	સંરચના ref *orefs = ખાલી, **oref_tail = &orefs;

	સંરચના hashmap existing_refs;
	પૂર્ણાંક existing_refs_populated = 0;

	filter_prefetch_refspec(rs);
	જો (remote)
		filter_prefetch_refspec(&remote->fetch);

	જો (rs->nr) {
		સંરચના refspec *fetch_refspec;

		માટે (i = 0; i < rs->nr; i++) {
			get_fetch_map(remote_refs, &rs->વસ્તુઓ[i], &tail, 0);
			જો (rs->વસ્તુઓ[i].dst && rs->વસ્તુઓ[i].dst[0])
				*autotags = 1;
		}
		/* Merge everything on the command line (but not --tags) */
		માટે (rm = ref_map; rm; rm = rm->next)
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
		જો (refmap.nr)
			fetch_refspec = &refmap;
		નહીંતર
			fetch_refspec = &remote->fetch;

		માટે (i = 0; i < fetch_refspec->nr; i++)
			get_fetch_map(ref_map, &fetch_refspec->વસ્તુઓ[i], &oref_tail, 1);
	} નહીંતર જો (refmap.nr) {
		die("--refmap option is only meaningful with command-line refspec(s)");
	} નહીંતર {
		/* Use the defaults */
		સંરચના branch *branch = branch_get(ખાલી);
		પૂર્ણાંક has_merge = branch_has_merge_config(branch);
		જો (remote &&
		    (remote->fetch.nr ||
		     /* Note: has_merge implies non-NULL branch->remote_name */
		     (has_merge && !strcmp(branch->remote_name, remote->name)))) {
			માટે (i = 0; i < remote->fetch.nr; i++) {
				get_fetch_map(remote_refs, &remote->fetch.વસ્તુઓ[i], &tail, 0);
				જો (remote->fetch.વસ્તુઓ[i].dst &&
				    remote->fetch.વસ્તુઓ[i].dst[0])
					*autotags = 1;
				જો (!i && !has_merge && ref_map &&
				    !remote->fetch.વસ્તુઓ[0].pattern)
					ref_map->fetch_head_status = FETCH_HEAD_MERGE;
			}
			/*
			 * if the remote we're fetching from is the same
			 * as given in branch.<name>.remote, we add the
			 * ref given in branch.<name>.merge, too.
			 *
			 * Note: has_merge implies non-NULL branch->remote_name
			 */
			જો (has_merge &&
			    !strcmp(branch->remote_name, remote->name))
				add_merge_config(&ref_map, remote_refs, branch, &tail);
		} નહીંતર જો (!prefetch) {
			ref_map = get_remote_ref(remote_refs, "HEAD");
			જો (!ref_map)
				die(_("couldn't find remote ref HEAD"));
			ref_map->fetch_head_status = FETCH_HEAD_MERGE;
			tail = &ref_map->next;
		}
	}

	જો (tags == TAGS_SET) {
		સંરચના refspec_item tag_refspec;

		/* also fetch all tags */
		refspec_item_init_ધકેલો(&tag_refspec, TAG_REFSPEC,
				       the_hash_algo);
		get_fetch_map(remote_refs, &tag_refspec, &tail, 0);
		refspec_item_clear(&tag_refspec);
	} નહીંતર જો (tags == TAGS_DEFAULT && *autotags) {
		find_non_local_tags(remote_refs, ખાલી, &ref_map, &tail);
	}

	/* Now append any refs to be updated opportunistically: */
	*tail = orefs;
	માટે (rm = orefs; rm; rm = rm->next) {
		rm->fetch_head_status = FETCH_HEAD_IGNORE;
		tail = &rm->next;
	}

	/*
	 * apply negative refspecs first, before we remove duplicates. This is
	 * necessary as negative refspecs might remove an otherwise conflicting
	 * duplicate.
	 */
	જો (rs->nr)
		ref_map = apply_negative_refspecs(ref_map, rs);
	નહીંતર
		ref_map = apply_negative_refspecs(ref_map, &remote->fetch);

	ref_map = ref_remove_duplicates(ref_map);

	માટે (rm = ref_map; rm; rm = rm->next) {
		જો (rm->peer_ref) {
			અચલ અક્ષર *refname = rm->peer_ref->name;
			સંરચના refname_hash_entry *peer_item;
			unsigned પૂર્ણાંક hash = strhash(refname);

			જો (!existing_refs_populated) {
				refname_hash_init(&existing_refs);
				refs_for_each_ref(get_main_ref_store(the_repository),
						  add_one_refname,
						  &existing_refs);
				existing_refs_populated = 1;
			}

			peer_item = hashmap_get_entry_from_hash(&existing_refs,
						hash, refname,
						સંરચના refname_hash_entry, ent);
			જો (peer_item) {
				સંરચના object_id *old_oid = &peer_item->oid;
				oidcpy(&rm->peer_ref->old_oid, old_oid);
			}
		}
	}
	જો (existing_refs_populated)
		hashmap_clear_and_free(&existing_refs, સંરચના refname_hash_entry, ent);

	પાછા ref_map;
}

સ્થિર પૂર્ણાંક s_update_ref(અચલ અક્ષર *action,
			સંરચના ref *ref,
			સંરચના ref_transaction *transaction,
			પૂર્ણાંક check_old)
{
	અક્ષર *સંદેશ;
	અક્ષર *rla = getenv("GIT_REFLOG_ACTION");
	સંરચના strbuf err = STRBUF_INIT;
	પૂર્ણાંક ret;

	જો (dry_run)
		પાછા 0;
	જો (!rla)
		rla = default_rla.buf;
	સંદેશ = xstrfmt("%s: %s", rla, action);

	ret = ref_transaction_update(transaction, ref->name, &ref->new_oid,
				     check_old ? &ref->old_oid : ખાલી,
				     ખાલી, ખાલી, 0, સંદેશ, &err);

	જો (ret)
		error("%s", err.buf);
	strbuf_release(&err);
	મુક્ત(સંદેશ);
	પાછા ret;
}

સ્થિર પૂર્ણાંક refcol_width(અચલ સંરચના ref *ref_map, પૂર્ણાંક compact_format)
{
	અચલ સંરચના ref *ref;
	પૂર્ણાંક max, width = 10;

	max = term_columns();
	જો (compact_format)
		max = max * 2 / 3;

	માટે (ref = ref_map; ref; ref = ref->next) {
		પૂર્ણાંક rlen, llen = 0, len;

		જો (ref->status == REF_STATUS_REJECT_SHALLOW ||
		    !ref->peer_ref ||
		    !strcmp(ref->name, "HEAD"))
			ચાલુ;

		/* uptodate lines are only shown on high verbosity level */
		જો (verbosity <= 0 && oideq(&ref->peer_ref->old_oid, &ref->old_oid))
			ચાલુ;

		rlen = utf8_strwidth(prettify_refname(ref->name));
		જો (!compact_format)
			llen = utf8_strwidth(prettify_refname(ref->peer_ref->name));

		/*
		 * rough estimation to see if the output line is too long and
		 * should not be counted (we can't do precise calculation
		 * anyway because we don't know if the error explanation part
		 * will be printed in update_local_ref)
		 */
		len = 21 /* flag and summary */ + rlen + 4 /* -> */ + llen;
		જો (len >= max)
			ચાલુ;

		જો (width < rlen)
			width = rlen;
	}

	પાછા width;
}

સ્થિર શૂન્ય display_state_init(સંરચના display_state *display_state, સંરચના ref *ref_map,
			       અચલ અક્ષર *raw_url, ગણના display_format format)
{
	પૂર્ણાંક i;

	memset(display_state, 0, કદ(*display_state));
	strbuf_init(&display_state->buf, 0);
	display_state->format = format;

	જો (raw_url)
		display_state->url = transport_anonymize_url(raw_url);
	નહીંતર
		display_state->url = xstrdup("foreign");

	display_state->url_len = strlen(display_state->url);
	માટે (i = display_state->url_len - 1; 0 <= i && display_state->url[i] == '/'; i--)
		;
	display_state->url_len = i + 1;
	જો (4 < i && !strncmp(".git", display_state->url + i - 3, 4))
		display_state->url_len = i - 3;

	જો (verbosity < 0)
		પાછા;

	પસંદગી (display_state->format) {
	સ્થિતિ DISPLAY_FORMAT_FULL:
	સ્થિતિ DISPLAY_FORMAT_COMPACT:
		display_state->refcol_width = refcol_width(ref_map,
							   display_state->format == DISPLAY_FORMAT_COMPACT);
		તોડો;
	સ્થિતિ DISPLAY_FORMAT_PORCELAIN:
		/* We don't need to precompute anything here. */
		તોડો;
	મૂળભૂત:
		BUG("unexpected display format %d", display_state->format);
	}
}

સ્થિર શૂન્ય display_state_release(સંરચના display_state *display_state)
{
	strbuf_release(&display_state->buf);
	મુક્ત(display_state->url);
}

સ્થિર શૂન્ય print_remote_to_local(સંરચના display_state *display_state,
				  અચલ અક્ષર *remote, અચલ અક્ષર *local)
{
	strbuf_addf(&display_state->buf, "%-*s -> %s",
		    display_state->refcol_width, remote, local);
}

સ્થિર પૂર્ણાંક find_and_replace(સંરચના strbuf *haystack,
			    અચલ અક્ષર *needle,
			    અચલ અક્ષર *placeholder)
{
	અચલ અક્ષર *p = ખાલી;
	પૂર્ણાંક plen, nlen;

	nlen = strlen(needle);
	જો (ends_with(haystack->buf, needle))
		p = haystack->buf + haystack->len - nlen;
	નહીંતર
		p = strstr(haystack->buf, needle);
	જો (!p)
		પાછા 0;

	જો (p > haystack->buf && p[-1] != '/')
		પાછા 0;

	plen = strlen(p);
	જો (plen > nlen && p[nlen] != '/')
		પાછા 0;

	strbuf_splice(haystack, p - haystack->buf, nlen,
		      placeholder, strlen(placeholder));
	પાછા 1;
}

સ્થિર શૂન્ય print_compact(સંરચના display_state *display_state,
			  અચલ અક્ષર *remote, અચલ અક્ષર *local)
{
	સંરચના strbuf r = STRBUF_INIT;
	સંરચના strbuf l = STRBUF_INIT;

	જો (!strcmp(remote, local)) {
		strbuf_addf(&display_state->buf, "%-*s -> *", display_state->refcol_width, remote);
		પાછા;
	}

	strbuf_addstr(&r, remote);
	strbuf_addstr(&l, local);

	જો (!find_and_replace(&r, local, "*"))
		find_and_replace(&l, remote, "*");
	print_remote_to_local(display_state, r.buf, l.buf);

	strbuf_release(&r);
	strbuf_release(&l);
}

સ્થિર શૂન્ય display_ref_update(સંરચના display_state *display_state, અક્ષર code,
			       અચલ અક્ષર *summary, અચલ અક્ષર *error,
			       અચલ અક્ષર *remote, અચલ અક્ષર *local,
			       અચલ સંરચના object_id *old_oid,
			       અચલ સંરચના object_id *new_oid,
			       પૂર્ણાંક summary_width)
{
	FILE *f = stderr;

	જો (verbosity < 0)
		પાછા;

	strbuf_reset(&display_state->buf);

	પસંદગી (display_state->format) {
	સ્થિતિ DISPLAY_FORMAT_FULL:
	સ્થિતિ DISPLAY_FORMAT_COMPACT: {
		પૂર્ણાંક width;

		જો (!display_state->shown_url) {
			strbuf_addf(&display_state->buf, _("From %.*s\n"),
				    display_state->url_len, display_state->url);
			display_state->shown_url = 1;
		}

		width = (summary_width + strlen(summary) - gettext_width(summary));
		remote = prettify_refname(remote);
		local = prettify_refname(local);

		strbuf_addf(&display_state->buf, " %c %-*s ", code, width, summary);

		જો (display_state->format != DISPLAY_FORMAT_COMPACT)
			print_remote_to_local(display_state, remote, local);
		નહીંતર
			print_compact(display_state, remote, local);

		જો (error)
			strbuf_addf(&display_state->buf, "  (%s)", error);

		તોડો;
	}
	સ્થિતિ DISPLAY_FORMAT_PORCELAIN:
		strbuf_addf(&display_state->buf, "%c %s %s %s", code,
			    oid_to_hex(old_oid), oid_to_hex(new_oid), local);
		f = stdout;
		તોડો;
	મૂળભૂત:
		BUG("unexpected display format %d", display_state->format);
	};
	strbuf_addch(&display_state->buf, '\n');

	fputs(display_state->buf.buf, f);
}

સંરચના ref_update_display_info {
	bool failed;
	અક્ષર success_code;
	અક્ષર fail_code;
	અક્ષર *summary;
	અક્ષર *fail_detail;
	અક્ષર *success_detail;
	અક્ષર *ref;
	અક્ષર *remote;
	સંરચના object_id old_oid;
	સંરચના object_id new_oid;
};

સંરચના ref_update_display_info_array {
	સંરચના ref_update_display_info *info;
	size_t alloc, nr;
};

સ્થિર સંરચના ref_update_display_info *ref_update_display_info_append(
					   સંરચના ref_update_display_info_array *array,
					   અક્ષર success_code,
					   અક્ષર fail_code,
					   અચલ અક્ષર *summary,
					   અચલ અક્ષર *success_detail,
					   અચલ અક્ષર *fail_detail,
					   અચલ અક્ષર *ref,
					   અચલ અક્ષર *remote,
					   અચલ સંરચના object_id *old_oid,
					   અચલ સંરચના object_id *new_oid)
{
	સંરચના ref_update_display_info *info;

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

	પાછા info;
}

સ્થિર શૂન્ય ref_update_display_info_set_failed(સંરચના ref_update_display_info *info)
{
	info->failed = true;
}

સ્થિર શૂન્ય ref_update_display_info_free(સંરચના ref_update_display_info *info)
{
	મુક્ત(info->summary);
	મુક્ત(info->success_detail);
	મુક્ત(info->fail_detail);
	મુક્ત(info->remote);
	મુક્ત(info->ref);
}

સ્થિર શૂન્ય ref_update_display_info_display(સંરચના ref_update_display_info *info,
					    સંરચના display_state *display_state,
					    પૂર્ણાંક summary_width)
{
	display_ref_update(display_state,
			   info->failed ? info->fail_code : info->success_code,
			   info->summary,
			   info->failed ? info->fail_detail : info->success_detail,
			   info->remote, info->ref, &info->old_oid,
			   &info->new_oid, summary_width);
}

સ્થિર પૂર્ણાંક update_local_ref(સંરચના ref *ref,
			    સંરચના ref_transaction *transaction,
			    અચલ સંરચના ref *remote_ref,
			    અચલ સંરચના fetch_config *config,
			    સંરચના ref_update_display_info_array *display_array)
{
	સંરચના commit *current = ખાલી, *updated;
	પૂર્ણાંક fast_forward = 0;

	જો (!odb_has_object(the_repository->objects, &ref->new_oid,
			    ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR))
		die(_("object %s not found"), oid_to_hex(&ref->new_oid));

	જો (oideq(&ref->old_oid, &ref->new_oid)) {
		જો (verbosity > 0)
			ref_update_display_info_append(display_array, '=', '=',
						       _("[up to date]"), ખાલી,
						       ખાલી, ref->name,
						       remote_ref->name, &ref->old_oid,
						       &ref->new_oid);
		પાછા 0;
	}

	જો (!update_head_ok &&
	    !is_null_oid(&ref->old_oid) &&
	    branch_checked_out(ref->name)) {
		સંરચના ref_update_display_info *info;
		/*
		 * If this is the head, and it's not okay to update
		 * the head, and the old value of the head isn't empty...
		 */
		info = ref_update_display_info_append(display_array, '!', '!',
						      _("[rejected]"), ખાલી,
						      _("can't fetch into checked-out branch"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);
		ref_update_display_info_set_failed(info);
		પાછા 1;
	}

	જો (!is_null_oid(&ref->old_oid) &&
	    starts_with(ref->name, "refs/tags/")) {
		સંરચના ref_update_display_info *info;

		જો (force || ref->force) {
			પૂર્ણાંક r;

			r = s_update_ref("updating tag", ref, transaction, 0);

			info = ref_update_display_info_append(display_array, 't', '!',
							      _("[tag update]"), ખાલી,
							      _("unable to update local ref"),
							      ref->name, remote_ref->name,
							      &ref->old_oid, &ref->new_oid);
			જો (r)
				ref_update_display_info_set_failed(info);

			પાછા r;
		} નહીંતર {
			info = ref_update_display_info_append(display_array, '!', '!',
							      _("[rejected]"), ખાલી,
							      _("would clobber existing tag"),
							      ref->name, remote_ref->name,
							      &ref->old_oid, &ref->new_oid);
			ref_update_display_info_set_failed(info);
			પાછા 1;
		}
	}

	current = lookup_commit_reference_gently(the_repository,
						 &ref->old_oid, 1);
	updated = lookup_commit_reference_gently(the_repository,
						 &ref->new_oid, 1);
	જો (!current || !updated) {
		સંરચના ref_update_display_info *info;
		અચલ અક્ષર *સંદેશ;
		અચલ અક્ષર *what;
		પૂર્ણાંક r;
		/*
		 * Nicely describe the new ref we're fetching.
		 * Base this on the remote's ref name, as it's
		 * more likely to follow a standard layout.
		 */
		જો (starts_with(remote_ref->name, "refs/tags/")) {
			સંદેશ = "storing tag";
			what = _("[new tag]");
		} નહીંતર જો (starts_with(remote_ref->name, "refs/heads/")) {
			સંદેશ = "storing head";
			what = _("[new branch]");
		} નહીંતર {
			સંદેશ = "storing ref";
			what = _("[new ref]");
		}

		r = s_update_ref(સંદેશ, ref, transaction, 0);

		info = ref_update_display_info_append(display_array, '*', '!',
						      what, ખાલી,
						      _("unable to update local ref"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);
		જો (r)
			ref_update_display_info_set_failed(info);

		પાછા r;
	}

	જો (config->show_forced_updates) {
		uint64_t t_before = getnanotime();
		fast_forward = repo_in_merge_bases(the_repository, current,
						   updated);
		જો (fast_forward < 0)
			die(ખાલી);
		forced_updates_ms += (getnanotime() - t_before) / 1000000;
	} નહીંતર {
		fast_forward = 1;
	}

	જો (fast_forward) {
		સંરચના ref_update_display_info *info;
		સંરચના strbuf quickref = STRBUF_INIT;
		પૂર્ણાંક r;

		strbuf_add_unique_abbrev(&quickref, &current->object.oid, DEFAULT_ABBREV);
		strbuf_addstr(&quickref, "..");
		strbuf_add_unique_abbrev(&quickref, &ref->new_oid, DEFAULT_ABBREV);
		r = s_update_ref("fast-forward", ref, transaction, 1);

		info = ref_update_display_info_append(display_array, ' ', '!',
						      quickref.buf, ખાલી,
						      _("unable to update local ref"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);
		જો (r)
			ref_update_display_info_set_failed(info);

		strbuf_release(&quickref);
		પાછા r;
	} નહીંતર જો (force || ref->force) {
		સંરચના ref_update_display_info *info;
		સંરચના strbuf quickref = STRBUF_INIT;
		પૂર્ણાંક r;

		strbuf_add_unique_abbrev(&quickref, &current->object.oid, DEFAULT_ABBREV);
		strbuf_addstr(&quickref, "...");
		strbuf_add_unique_abbrev(&quickref, &ref->new_oid, DEFAULT_ABBREV);
		r = s_update_ref("forced-update", ref, transaction, 1);

		info = ref_update_display_info_append(display_array, '+', '!',
						      quickref.buf, _("forced update"),
						      _("unable to update local ref"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);

		જો (r)
			ref_update_display_info_set_failed(info);

		strbuf_release(&quickref);
		પાછા r;
	} નહીંતર {
		સંરચના ref_update_display_info *info;
		info = ref_update_display_info_append(display_array, '!', '!',
						      _("[rejected]"), ખાલી,
						      _("non-fast-forward"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);
		ref_update_display_info_set_failed(info);
		પાછા 1;
	}
}

સ્થિર અચલ સંરચના object_id *iterate_ref_map(શૂન્ય *cb_data)
{
	સંરચના ref **rm = cb_data;
	સંરચના ref *ref = *rm;

	જ્યારે (ref && ref->status == REF_STATUS_REJECT_SHALLOW)
		ref = ref->next;
	જો (!ref)
		પાછા ખાલી;
	*rm = ref->next;
	પાછા &ref->old_oid;
}

સંરચના fetch_head {
	FILE *fp;
	સંરચના strbuf buf;
};

સ્થિર પૂર્ણાંક open_fetch_head(સંરચના fetch_head *fetch_head)
{
	અચલ અક્ષર *filename = git_path_fetch_head(the_repository);

	જો (write_fetch_head) {
		fetch_head->fp = fopen(filename, "a");
		જો (!fetch_head->fp)
			પાછા error_errno(_("cannot open '%s'"), filename);
		strbuf_init(&fetch_head->buf, 0);
	} નહીંતર {
		fetch_head->fp = ખાલી;
	}

	પાછા 0;
}

સ્થિર શૂન્ય append_fetch_head(સંરચના fetch_head *fetch_head,
			      અચલ સંરચના object_id *old_oid,
			      ગણના fetch_head_status fetch_head_status,
			      અચલ અક્ષર *note,
			      અચલ અક્ષર *url, size_t url_len)
{
	અક્ષર old_oid_hex[GIT_MAX_HEXSZ + 1];
	અચલ અક્ષર *merge_status_marker;
	size_t i;

	જો (!fetch_head->fp)
		પાછા;

	પસંદગી (fetch_head_status) {
	સ્થિતિ FETCH_HEAD_NOT_FOR_MERGE:
		merge_status_marker = "not-for-merge";
		તોડો;
	સ્થિતિ FETCH_HEAD_MERGE:
		merge_status_marker = "";
		તોડો;
	મૂળભૂત:
		/* do not write anything to FETCH_HEAD */
		પાછા;
	}

	strbuf_addf(&fetch_head->buf, "%s\t%s\t%s",
		    oid_to_hex_r(old_oid_hex, old_oid), merge_status_marker, note);
	માટે (i = 0; i < url_len; ++i)
		જો ('\n' == url[i])
			strbuf_addstr(&fetch_head->buf, "\\n");
		નહીંતર
			strbuf_addch(&fetch_head->buf, url[i]);
	strbuf_addch(&fetch_head->buf, '\n');

	/*
	 * When using an atomic fetch, we do not want to update FETCH_HEAD if
	 * any of the reference updates fails. We thus have to write all
	 * updates to a buffer first and only commit it as soon as all
	 * references have been successfully updated.
	 */
	જો (!atomic_fetch) {
		strbuf_write(&fetch_head->buf, fetch_head->fp);
		strbuf_reset(&fetch_head->buf);
	}
}

સ્થિર શૂન્ય commit_fetch_head(સંરચના fetch_head *fetch_head)
{
	જો (!fetch_head->fp || !atomic_fetch)
		પાછા;
	strbuf_write(&fetch_head->buf, fetch_head->fp);
}

સ્થિર શૂન્ય close_fetch_head(સંરચના fetch_head *fetch_head)
{
	જો (!fetch_head->fp)
		પાછા;

	fclose(fetch_head->fp);
	strbuf_release(&fetch_head->buf);
}

સ્થિર અચલ અક્ષર warn_show_forced_updates[] =
N_("fetch normally indicates which branches had a forced update,\n"
   "but that check has been disabled; to re-enable, use '--show-forced-updates'\n"
   "flag or run 'git config fetch.showForcedUpdates true'");
સ્થિર અચલ અક્ષર warn_time_show_forced_updates[] =
N_("it took %.2f seconds to check forced updates; you can use\n"
   "'--no-show-forced-updates' or run 'git config fetch.showForcedUpdates false'\n"
   "to avoid this check\n");

સ્થિર પૂર્ણાંક store_updated_refs(સંરચના display_state *display_state,
			      પૂર્ણાંક connectivity_checked,
			      સંરચના ref_transaction *transaction, સંરચના ref *ref_map,
			      સંરચના fetch_head *fetch_head,
			      અચલ સંરચના fetch_config *config,
			      સંરચના ref_update_display_info_array *display_array)
{
	પૂર્ણાંક rc = 0;
	સંરચના strbuf note = STRBUF_INIT;
	અચલ અક્ષર *what, *kind;
	સંરચના ref *rm;
	પૂર્ણાંક want_status;

	જો (!connectivity_checked) {
		સંરચના check_connected_options opt = CHECK_CONNECTED_INIT;

		opt.exclude_hidden_refs_section = "fetch";
		rm = ref_map;
		જો (check_connected(iterate_ref_map, &rm, &opt)) {
			rc = error(_("%s did not send all necessary objects"),
				   display_state->url);
			જાઓ abort;
		}
	}

	/*
	 * We do a pass for each fetch_head_status type in their enum order, so
	 * merged entries are written before not-for-merge. That lets readers
	 * use FETCH_HEAD as a refname to refer to the ref to be merged.
	 */
	માટે (want_status = FETCH_HEAD_MERGE;
	     want_status <= FETCH_HEAD_IGNORE;
	     want_status++) {
		માટે (rm = ref_map; rm; rm = rm->next) {
			સંરચના ref *ref = ખાલી;

			જો (rm->status == REF_STATUS_REJECT_SHALLOW) {
				જો (want_status == FETCH_HEAD_MERGE)
					warning(_("rejected %s because shallow roots are not allowed to be updated"),
						rm->peer_ref ? rm->peer_ref->name : rm->name);
				ચાલુ;
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
			જો (fetch_head->fp) {
				સંરચના commit *commit = ખાલી;

				/*
				 * References in "refs/tags/" are often going to point
				 * to annotated tags, which are not part of the
				 * commit-graph. We thus only try to look up refs in
				 * the graph which are not in that namespace to not
				 * regress performance in repositories with many
				 * annotated tags.
				 */
				જો (!starts_with(rm->name, "refs/tags/"))
					commit = lookup_commit_in_graph(the_repository, &rm->old_oid);
				જો (!commit) {
					commit = lookup_commit_reference_gently(the_repository,
										&rm->old_oid,
										1);
					જો (!commit)
						rm->fetch_head_status = FETCH_HEAD_NOT_FOR_MERGE;
				}
			}

			જો (rm->fetch_head_status != want_status)
				ચાલુ;

			જો (rm->peer_ref) {
				ref = alloc_ref(rm->peer_ref->name);
				oidcpy(&ref->old_oid, &rm->peer_ref->old_oid);
				oidcpy(&ref->new_oid, &rm->old_oid);
				ref->force = rm->peer_ref->force;
			}

			જો (config->recurse_submodules != RECURSE_SUBMODULES_OFF &&
			    (!rm->peer_ref || !oideq(&ref->old_oid, &ref->new_oid))) {
				check_for_new_submodule_commits(&rm->old_oid);
			}

			જો (!strcmp(rm->name, "HEAD")) {
				kind = "";
				what = "";
			} નહીંતર જો (skip_prefix(rm->name, "refs/heads/", &what)) {
				kind = "branch";
			} નહીંતર જો (skip_prefix(rm->name, "refs/tags/", &what)) {
				kind = "tag";
			} નહીંતર જો (skip_prefix(rm->name, "refs/remotes/", &what)) {
				kind = "remote-tracking branch";
			} નહીંતર {
				kind = "";
				what = rm->name;
			}

			strbuf_reset(&note);
			જો (*what) {
				જો (*kind)
					strbuf_addf(&note, "%s ", kind);
				strbuf_addf(&note, "'%s' of ", what);
			}

			append_fetch_head(fetch_head, &rm->old_oid,
					  rm->fetch_head_status,
					  note.buf, display_state->url,
					  display_state->url_len);

			જો (ref) {
				rc |= update_local_ref(ref, transaction, rm,
						       config, display_array);
				મુક્ત(ref);
			} નહીંતર જો (write_fetch_head || dry_run) {
				/*
				 * Display fetches written to FETCH_HEAD (or
				 * would be written to FETCH_HEAD, if --dry-run
				 * is set).
				 */

				ref_update_display_info_append(display_array, '*', '*',
							       *kind ? kind : "branch",
							       ખાલી, ખાલી, "FETCH_HEAD",
							       rm->name, &rm->new_oid,
							       &rm->old_oid);
			}
		}
	}

	જો (advice_enabled(ADVICE_FETCH_SHOW_FORCED_UPDATES)) {
		જો (!config->show_forced_updates) {
			warning(_(warn_show_forced_updates));
		} નહીંતર જો (forced_updates_ms > FORCED_UPDATES_DELAY_WARNING_IN_MS) {
			warning(_(warn_time_show_forced_updates),
				forced_updates_ms / 1000.0);
		}
	}

 abort:
	strbuf_release(&note);
	પાછા rc;
}

/*
 * We would want to bypass the object transfer altogether if
 * everything we are going to fetch already exists and is connected
 * locally.
 */
સ્થિર પૂર્ણાંક check_exist_and_connected(સંરચના ref *ref_map)
{
	સંરચના ref *rm = ref_map;
	સંરચના check_connected_options opt = CHECK_CONNECTED_INIT;
	સંરચના ref *r;

	/*
	 * If we are deepening a shallow clone we already have these
	 * objects reachable.  Running rev-list here will return with
	 * a good (0) exit status and we'll bypass the fetch that we
	 * really need to perform.  Claiming failure now will ensure
	 * we perform the network exchange to deepen our history.
	 */
	જો (deepen)
		પાછા -1;

	/*
	 * Similarly, if we need to refetch, we always want to perform a full
	 * fetch ignoring existing objects.
	 */
	જો (refetch)
		પાછા -1;


	/*
	 * check_connected() allows objects to merely be promised, but
	 * we need all direct targets to exist.
	 */
	માટે (r = rm; r; r = r->next) {
		જો (!odb_has_object(the_repository->objects, &r->old_oid,
				    ODB_HAS_OBJECT_RECHECK_PACKED))
			પાછા -1;
	}

	opt.quiet = 1;
	opt.exclude_hidden_refs_section = "fetch";
	પાછા check_connected(iterate_ref_map, &rm, &opt);
}

સ્થિર પૂર્ણાંક fetch_and_consume_refs(સંરચના display_state *display_state,
				  સંરચના transport *transport,
				  સંરચના ref_transaction *transaction,
				  સંરચના ref *ref_map,
				  સંરચના fetch_head *fetch_head,
				  અચલ સંરચના fetch_config *config,
				  સંરચના ref_update_display_info_array *display_array)
{
	પૂર્ણાંક connectivity_checked = 1;
	પૂર્ણાંક ret;

	/*
	 * We don't need to perform a fetch in case we can already satisfy all
	 * refs.
	 */
	ret = check_exist_and_connected(ref_map);
	જો (ret) {
		trace2_region_enter("fetch", "fetch_refs", the_repository);
		ret = transport_fetch_refs(transport, ref_map);
		trace2_region_leave("fetch", "fetch_refs", the_repository);
		જો (ret)
			જાઓ out;
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
	પાછા ret;
}

સ્થિર પૂર્ણાંક prune_refs(સંરચના display_state *display_state,
		      સંરચના refspec *rs,
		      સંરચના ref_transaction *transaction,
		      સંરચના ref *ref_map)
{
	પૂર્ણાંક result = 0;
	સંરચના ref *ref, *stale_refs = get_stale_heads(rs, ref_map);
	સંરચના strbuf err = STRBUF_INIT;
	સંરચના string_list refnames = STRING_LIST_INIT_NODUP;

	માટે (ref = stale_refs; ref; ref = ref->next)
		string_list_append(&refnames, ref->name);

	જો (!dry_run) {
		જો (transaction) {
			માટે (ref = stale_refs; ref; ref = ref->next) {
				result = ref_transaction_delete(transaction, ref->name, ખાલી,
								ખાલી, 0, "fetch: prune", &err);
				જો (result)
					જાઓ cleanup;
			}
		} નહીંતર {
			result = refs_delete_refs(get_main_ref_store(the_repository),
						  "fetch: prune", &refnames,
						  0);
		}
	}

	જો (verbosity >= 0) {
		પૂર્ણાંક summary_width = transport_summary_width(stale_refs);

		માટે (ref = stale_refs; ref; ref = ref->next) {
			display_ref_update(display_state, '-', _("[deleted]"), ખાલી,
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
	પાછા result;
}

સ્થિર શૂન્ય check_not_current_branch(સંરચના ref *ref_map)
{
	અચલ અક્ષર *path;
	માટે (; ref_map; ref_map = ref_map->next)
		જો (ref_map->peer_ref &&
		    starts_with(ref_map->peer_ref->name, "refs/heads/") &&
		    (path = branch_checked_out(ref_map->peer_ref->name)))
			die(_("refusing to fetch into branch '%s' "
			      "checked out at '%s'"),
			    ref_map->peer_ref->name, path);
}

સ્થિર પૂર્ણાંક truncate_fetch_head(શૂન્ય)
{
	અચલ અક્ષર *filename = git_path_fetch_head(the_repository);
	FILE *fp = fopen_for_writing(filename);

	જો (!fp)
		પાછા error_errno(_("cannot open '%s'"), filename);
	fclose(fp);
	પાછા 0;
}

સ્થિર શૂન્ય set_option(સંરચના transport *transport, અચલ અક્ષર *name, અચલ અક્ષર *કિંમત)
{
	પૂર્ણાંક r = transport_set_option(transport, name, કિંમત);
	જો (r < 0)
		die(_("option \"%s\" value \"%s\" is not valid for %s"),
		    name, કિંમત, transport->url);
	જો (r > 0)
		warning(_("option \"%s\" is ignored for %s"),
			name, transport->url);
}


સ્થિર પૂર્ણાંક add_oid(અચલ સંરચના reference *ref, શૂન્ય *cb_data)
{
	સંરચના oid_array *oids = cb_data;

	oid_array_append(oids, ref->oid);
	પાછા 0;
}

સ્થિર શૂન્ય add_negotiation_tips(સંરચના string_list *આગત_list,
				 સંરચના oid_array **output_list,
				 અચલ અક્ષર *argname)
{
	સંરચના oid_array *oids = xcalloc(1, કદ(*oids));
	પૂર્ણાંક i;

	માટે (i = 0; i < આગત_list->nr; i++) {
		અચલ અક્ષર *s = આગત_list->વસ્તુઓ[i].string;
		સંરચના refs_for_each_ref_options opts = {
			.pattern = s,
		};
		પૂર્ણાંક old_nr;
		જો (!has_glob_specials(s)) {
			સંરચના object_id oid;

			/* Ignore missing reference. */
			જો (repo_get_oid(the_repository, s, &oid))
				ચાલુ;
			/* Fail on missing object pointed by ref. */
			જો (!odb_has_object(the_repository->objects, &oid, 0))
				die(_("the object %s does not exist"), s);

			oid_array_append(oids, &oid);
			ચાલુ;
		}
		old_nr = oids->nr;
		refs_for_each_ref_ext(get_main_ref_store(the_repository),
				      add_oid, oids, &opts);
		જો (old_nr == oids->nr)
			warning(_("ignoring %s=%s because it does not match any refs"),
				argname, s);
	}
	*output_list = oids;
}

સ્થિર સંરચના transport *prepare_transport(સંરચના remote *remote, પૂર્ણાંક deepen,
					   સંરચના list_objects_filter_options *filter_options)
{
	સંરચના transport *transport;

	transport = transport_get(remote, ખાલી);
	transport_set_verbosity(transport, verbosity, progress);
	transport->family = family;
	જો (upload_pack)
		set_option(transport, TRANS_OPT_UPLOADPACK, upload_pack);
	જો (keep)
		set_option(transport, TRANS_OPT_KEEP, "yes");
	જો (depth)
		set_option(transport, TRANS_OPT_DEPTH, depth);
	જો (deepen && deepen_since)
		set_option(transport, TRANS_OPT_DEEPEN_SINCE, deepen_since);
	જો (deepen && deepen_not.nr)
		set_option(transport, TRANS_OPT_DEEPEN_NOT,
			   (અચલ અક્ષર *)&deepen_not);
	જો (deepen_relative)
		set_option(transport, TRANS_OPT_DEEPEN_RELATIVE, "yes");
	જો (update_shallow)
		set_option(transport, TRANS_OPT_UPDATE_SHALLOW, "yes");
	જો (refetch)
		set_option(transport, TRANS_OPT_REFETCH, "yes");
	જો (filter_options->choice) {
		અચલ અક્ષર *spec =
			expand_list_objects_filter_spec(filter_options);
		set_option(transport, TRANS_OPT_LIST_OBJECTS_FILTER, spec);
		set_option(transport, TRANS_OPT_FROM_PROMISOR, "1");
	}
	જો (negotiation_restrict.nr) {
		જો (transport->smart_options)
			add_negotiation_tips(&negotiation_restrict,
					     &transport->smart_options->negotiation_restrict_tips,
					     "--negotiation-restrict");
		નહીંતર
			warning(_("ignoring %s because the protocol does not support it"),
				"--negotiation-restrict");
	} નહીંતર જો (remote->negotiation_restrict.nr) {
		સંરચના string_list_item *item;
		for_each_string_list_item(item, &remote->negotiation_restrict)
			string_list_append(&negotiation_restrict, item->string);
		જો (transport->smart_options)
			add_negotiation_tips(&negotiation_restrict,
					     &transport->smart_options->negotiation_restrict_tips,
					     "--negotiation-restrict");
		નહીંતર {
			સંરચના strbuf config_name = STRBUF_INIT;
			strbuf_addf(&config_name, "remote.%s.negotiationRestrict", remote->name);
			warning(_("ignoring %s because the protocol does not support it"),
				config_name.buf);
			strbuf_release(&config_name);
		}
	}
	જો (negotiation_include.nr) {
		જો (transport->smart_options)
			add_negotiation_tips(&negotiation_include,
					     &transport->smart_options->negotiation_include_tips,
					     "--negotiation-include");
		નહીંતર
			warning(_("ignoring %s because the protocol does not support it"),
				"--negotiation-include");
	} નહીંતર જો (remote->negotiation_include.nr) {
		જો (transport->smart_options) {
			add_negotiation_tips(&remote->negotiation_include,
					     &transport->smart_options->negotiation_include_tips,
					     "--negotiation-include");
		} નહીંતર {
			સંરચના strbuf config_name = STRBUF_INIT;
			strbuf_addf(&config_name, "remote.%s.negotiationInclude", remote->name);
			warning(_("ignoring %s because the protocol does not support it"),
				config_name.buf);
			strbuf_release(&config_name);
		}
	}
	પાછા transport;
}

સ્થિર પૂર્ણાંક backfill_tags(સંરચના display_state *display_state,
			 સંરચના transport *transport,
			 સંરચના ref_transaction *transaction,
			 સંરચના ref *ref_map,
			 સંરચના fetch_head *fetch_head,
			 અચલ સંરચના fetch_config *config,
			 સંરચના ref_update_display_info_array *display_array,
			 સંરચના list_objects_filter_options *filter_options)
{
	પૂર્ણાંક retcode, cannot_reuse;

	/*
	 * Once we have set TRANS_OPT_DEEPEN_SINCE, we can't unset it
	 * when remote helper is used (setting it to an empty string
	 * is not unsetting). We could extend the remote helper
	 * protocol for that, but for now, just force a new connection
	 * without deepen-since. Similar story for deepen-not.
	 */
	cannot_reuse = transport->cannot_reuse ||
		deepen_since || deepen_not.nr;
	જો (cannot_reuse) {
		gsecondary = prepare_transport(transport->remote, 0, filter_options);
		transport = gsecondary;
	}

	transport_set_option(transport, TRANS_OPT_FOLLOWTAGS, ખાલી);
	transport_set_option(transport, TRANS_OPT_DEPTH, "0");
	transport_set_option(transport, TRANS_OPT_DEEPEN_RELATIVE, ખાલી);
	retcode = fetch_and_consume_refs(display_state, transport, transaction, ref_map,
					 fetch_head, config, display_array);

	જો (gsecondary) {
		transport_disconnect(gsecondary);
		gsecondary = ખાલી;
	}

	પાછા retcode;
}

સ્થિર અચલ અક્ષર *strip_refshead(અચલ અક્ષર *name){
	skip_prefix(name, "refs/heads/", &name);
	પાછા name;
}

સ્થિર શૂન્ય set_head_advice_સંદેશ(અચલ અક્ષર *remote, અચલ અક્ષર *head_name)
{
	અચલ અક્ષર message_advice_set_head[] =
	N_("Run 'git remote set-head %s %s' to follow the change, or modify\n"
	   "either of the 'remote.%s.followRemoteHEAD' or 'fetch.followRemoteHEAD'\n"
	   "configuration variables to handle the situation differently.\n\n"

	   "Using this specific setting\n\n"
	   "    git config set remote.%s.followRemoteHEAD warn-if-not-%s\n\n"
	   "will suppress the warning until the remote changes HEAD to something else.");

	advise_if_enabled(ADVICE_FETCH_SET_HEAD_WARN, _(message_advice_set_head),
			remote, head_name, remote, remote, head_name);
}

સ્થિર શૂન્ય warn_set_head(અચલ અક્ષર *remote, અચલ અક્ષર *head_name,
			સંરચના strbuf *buf_prev, પૂર્ણાંક updateres) {
	સંરચના strbuf buf_prefix = STRBUF_INIT;
	અચલ અક્ષર *prev_head = ખાલી;

	strbuf_addf(&buf_prefix, "refs/remotes/%s/", remote);
	skip_prefix(buf_prev->buf, buf_prefix.buf, &prev_head);

	જો (prev_head && strcmp(prev_head, head_name)) {
		printf("'HEAD' at '%s' is '%s', but we have '%s' locally.\n",
			remote, head_name, prev_head);
		set_head_advice_સંદેશ(remote, head_name);
	}
	નહીંતર જો (updateres && buf_prev->len) {
		printf("'HEAD' at '%s' is '%s', "
			"but we have a detached HEAD pointing to '%s' locally.\n",
			remote, head_name, buf_prev->buf);
		set_head_advice_સંદેશ(remote, head_name);
	}
	strbuf_release(&buf_prefix);
}

સ્થિર પૂર્ણાંક set_head(અચલ સંરચના ref *remote_refs, સંરચના remote *remote,
			પૂર્ણાંક follow_remote_head)
{
	પૂર્ણાંક result = 0, create_only, baremirror, was_detached;
	સંરચના strbuf b_head = STRBUF_INIT, b_remote_head = STRBUF_INIT,
		      b_local_head = STRBUF_INIT;
	અચલ અક્ષર *no_warn_branch = remote->no_warn_branch;
	અક્ષર *head_name = ખાલી;
	સંરચના ref *ref, *matches;
	સંરચના ref *fetch_map = ખાલી, **fetch_map_tail = &fetch_map;
	સંરચના refspec_item refspec = {
		.force = 0,
		.pattern = 1,
		.src = (અક્ષર *) "refs/heads/*",
		.dst = (અક્ષર *) "refs/heads/*",
	};
	સંરચના string_list heads = STRING_LIST_INIT_DUP;
	સંરચના ref_store *refs = get_main_ref_store(the_repository);

	get_fetch_map(remote_refs, &refspec, &fetch_map_tail, 0);
	matches = guess_remote_head(find_ref_by_name(remote_refs, "HEAD"),
				    fetch_map, REMOTE_GUESS_HEAD_ALL);
	માટે (ref = matches; ref; ref = ref->next) {
		string_list_append(&heads, strip_refshead(ref->name));
	}

	જો (!heads.nr)
		result = 1;
	નહીંતર જો (heads.nr > 1)
		result = 1;
	નહીંતર
		head_name = xstrdup(heads.વસ્તુઓ[0].string);

	જો (!head_name)
		જાઓ cleanup;
	baremirror = is_bare_repository(the_repository) && remote->mirror;
	create_only = follow_remote_head == FOLLOW_REMOTE_ALWAYS ? 0 : !baremirror;
	જો (baremirror) {
		strbuf_addstr(&b_head, "HEAD");
		strbuf_addf(&b_remote_head, "refs/heads/%s", head_name);
	} નહીંતર {
		strbuf_addf(&b_head, "refs/remotes/%s/HEAD", remote->name);
		strbuf_addf(&b_remote_head, "refs/remotes/%s/%s", remote->name, head_name);
	}
	/* make sure it's valid */
	જો (!baremirror && !refs_ref_exists(refs, b_remote_head.buf)) {
		result = 1;
		જાઓ cleanup;
	}
	was_detached = refs_update_symref_extended(refs, b_head.buf, b_remote_head.buf,
					"fetch", &b_local_head, create_only);
	જો (was_detached == -1) {
		result = 1;
		જાઓ cleanup;
	}
	જો (verbosity >= 0 &&
		follow_remote_head == FOLLOW_REMOTE_WARN &&
		(!no_warn_branch || strcmp(no_warn_branch, head_name)))
		warn_set_head(remote->name, head_name, &b_local_head, was_detached);

cleanup:
	મુક્ત(head_name);
	free_refs(fetch_map);
	free_refs(matches);
	string_list_clear(&heads, 0);
	strbuf_release(&b_head);
	strbuf_release(&b_local_head);
	strbuf_release(&b_remote_head);
	પાછા result;
}

સંરચના ref_rejection_data {
	પૂર્ણાંક *retcode;
	bool conflict_સંદેશ_shown;
	bool case_sensitive_સંદેશ_shown;
	અચલ અક્ષર *remote_name;
	સંરચના strmap *rejected_refs;
};

સ્થિર શૂન્ય ref_transaction_rejection_handler(અચલ અક્ષર *refname,
					      અચલ સંરચના object_id *old_oid UNUSED,
					      અચલ સંરચના object_id *new_oid UNUSED,
					      અચલ અક્ષર *old_target UNUSED,
					      અચલ અક્ષર *new_target UNUSED,
					      ગણના ref_transaction_error err,
					      અચલ અક્ષર *details,
					      શૂન્ય *cb_data)
{
	સંરચના ref_rejection_data *data = cb_data;

	જો (err == REF_TRANSACTION_ERROR_CASE_CONFLICT && repo_ignore_case(the_repository) &&
	    !data->case_sensitive_સંદેશ_shown) {
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
		data->case_sensitive_સંદેશ_shown = true;
	} નહીંતર જો (err == REF_TRANSACTION_ERROR_NAME_CONFLICT &&
		   !data->conflict_સંદેશ_shown) {
		error(_("some local refs could not be updated; try running\n"
			" 'git remote prune %s' to remove any old, conflicting "
			"branches"), data->remote_name);
		data->conflict_સંદેશ_shown = true;
	} નહીંતર {
		જો (details)
			error("%s", details);
		નહીંતર
			error(_("fetching ref %s failed: %s"),
			      refname, ref_transaction_error_સંદેશ(err));
	}

	strmap_put(data->rejected_refs, refname, ખાલી);
	*data->retcode = 1;
}

/*
 * Commit the reference transaction. If it isn't an atomic transaction, handle
 * rejected updates as part of using batched updates.
 */
સ્થિર પૂર્ણાંક commit_ref_transaction(સંરચના ref_transaction **transaction,
				  bool is_atomic, અચલ અક્ષર *remote_name,
				  સંરચના strmap *rejected_refs,
				  સંરચના strbuf *err)
{
	પૂર્ણાંક retcode = ref_transaction_commit(*transaction, err);
	જો (retcode)
		જાઓ out;

	જો (!is_atomic) {
		સંરચના ref_rejection_data data = {
			.conflict_સંદેશ_shown = 0,
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
	*transaction = ખાલી;
	પાછા retcode;
}

સ્થિર શૂન્ય collect_updated_tips(સંરચના oidset *tips, સંરચના ref *ref_map)
{
	સંરચના ref *rm;
	માટે (rm = ref_map; rm; rm = rm->next) {
		સંરચના commit *commit;
		/*
		 * Shallow-rejected refs are not stored and their history
		 * is incomplete, so skip them.
		 */
		જો (rm->status == REF_STATUS_REJECT_SHALLOW)
			ચાલુ;
		જો (is_null_oid(&rm->old_oid))
			ચાલુ;
		જો (rm->peer_ref &&
		    oideq(&rm->old_oid, &rm->peer_ref->old_oid))
			ચાલુ;
		commit = lookup_commit_reference_gently(the_repository,
							&rm->old_oid, 1);
		જો (commit)
			oidset_ઉમેરો(tips, &commit->object.oid);
	}
}

સ્થિર પૂર્ણાંક do_fetch(સંરચના transport *transport,
		    સંરચના refspec *rs,
		    અચલ સંરચના fetch_config *config,
		    સંરચના list_objects_filter_options *filter_options,
		    સંરચના oidset *updated_tips)
{
	સંરચના ref_transaction *transaction = ખાલી;
	સંરચના ref *ref_map = ખાલી;
	સંરચના display_state display_state = { 0 };
	પૂર્ણાંક autotags = (transport->remote->fetch_tags == 1);
	પૂર્ણાંક retcode = 0;
	અચલ સંરચના ref *remote_refs;
	સંરચના transport_ls_refs_options transport_ls_refs_options =
		TRANSPORT_LS_REFS_OPTIONS_INIT;
	સંરચના fetch_head fetch_head = { 0 };
	સંરચના strbuf err = STRBUF_INIT;
	સંરચના ref_update_display_info_array display_array = { 0 };
	સંરચના strmap rejected_refs = STRMAP_INIT;
	પૂર્ણાંક summary_width = 0;
	પૂર્ણાંક follow_remote_head = FOLLOW_REMOTE_NEVER;

	જો (tags == TAGS_DEFAULT) {
		જો (transport->remote->fetch_tags == 2)
			tags = TAGS_SET;
		જો (transport->remote->fetch_tags == -1)
			tags = TAGS_UNSET;
	}

	/* if not appending, truncate FETCH_HEAD */
	જો (!append && write_fetch_head) {
		retcode = truncate_fetch_head();
		જો (retcode)
			જાઓ cleanup;
	}

	જો (rs->nr) {
		refspec_ref_prefixes(rs, &transport_ls_refs_options.ref_prefixes);
	} નહીંતર {
		સંરચના branch *branch = branch_get(ખાલી);

		જો (transport->remote->fetch.nr) {
			refspec_ref_prefixes(&transport->remote->fetch,
					     &transport_ls_refs_options.ref_prefixes);
			/*
			 * See remote.c's handling of remote.<name>.followRemoteHEAD
			 * for the analogous, still-unresolved case.
			 */
			જો (transport->remote->follow_remote_head)
				follow_remote_head = transport->remote->follow_remote_head;
			નહીંતર જો (config->follow_remote_head_seen)
				follow_remote_head = get_follow_remote_head(config->follow_remote_head_raw);
			નહીંતર
				follow_remote_head = BUILTIN_FOLLOW_REMOTE_HEAD_DFLT;
		}
		જો (branch && branch_has_merge_config(branch) &&
		    !strcmp(branch->remote_name, transport->remote->name)) {
			પૂર્ણાંક i;
			માટે (i = 0; i < branch->merge_nr; i++) {
				strvec_ધકેલો(&transport_ls_refs_options.ref_prefixes,
					    branch->merge[i]->src);
			}
		}

		/*
		 * If there are no refs specified to fetch, then we just
		 * fetch HEAD; mention that to narrow the advertisement.
		 */
		જો (!transport_ls_refs_options.ref_prefixes.nr)
			strvec_ધકેલો(&transport_ls_refs_options.ref_prefixes,
				    "HEAD");
	}

	જો (tags == TAGS_SET || tags == TAGS_DEFAULT)
		strvec_ધકેલો(&transport_ls_refs_options.ref_prefixes,
			    "refs/tags/");

	જો (follow_remote_head != FOLLOW_REMOTE_NEVER)
		strvec_ધકેલો(&transport_ls_refs_options.ref_prefixes,
			    "HEAD");

	/*
	 * Only initiate ref listing if we have at least one ref we want to
	 * know about.
	 */
	જો (transport_ls_refs_options.ref_prefixes.nr) {
		trace2_region_enter("fetch", "remote_refs", the_repository);
		remote_refs = transport_get_remote_refs(transport,
							&transport_ls_refs_options);
		trace2_region_leave("fetch", "remote_refs", the_repository);
	} નહીંતર
		remote_refs = ખાલી;

	transport_ls_refs_options_release(&transport_ls_refs_options);

	ref_map = get_ref_map(transport->remote, remote_refs, rs,
			      tags, &autotags);
	જો (!update_head_ok)
		check_not_current_branch(ref_map);

	retcode = open_fetch_head(&fetch_head);
	જો (retcode)
		જાઓ cleanup;

	display_state_init(&display_state, ref_map, transport->url,
			   config->display_format);

	જો (atomic_fetch) {
		transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
							  0, &err);
		જો (!transaction) {
			retcode = -1;
			જાઓ cleanup;
		}
	}

	જો (tags == TAGS_DEFAULT && autotags)
		transport_set_option(transport, TRANS_OPT_FOLLOWTAGS, "1");
	જો (prune) {
		/*
		 * We only prune based on refspecs specified
		 * explicitly (via command line or configuration); we
		 * don't care whether --tags was specified.
		 */
		જો (rs->nr) {
			retcode = prune_refs(&display_state, rs, transaction, ref_map);
		} નહીંતર {
			retcode = prune_refs(&display_state, &transport->remote->fetch,
					     transaction, ref_map);
		}
		જો (retcode != 0)
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
	જો (!transaction) {
		transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
							  REF_TRANSACTION_ALLOW_FAILURE, &err);
		જો (!transaction) {
			retcode = -1;
			જાઓ cleanup;
		}
	}

	જો (fetch_and_consume_refs(&display_state, transport, transaction, ref_map,
				   &fetch_head, config, &display_array)) {
		retcode = 1;
		જાઓ cleanup;
	}

	/*
	 * If neither --no-tags nor --tags was specified, do automated tag
	 * following.
	 */
	જો (tags == TAGS_DEFAULT && autotags) {
		સંરચના ref *tags_ref_map = ખાલી, **tail = &tags_ref_map;

		find_non_local_tags(remote_refs, transaction, &tags_ref_map, &tail);
		જો (tags_ref_map) {
			/*
			 * If backfilling of tags fails then we want to tell
			 * the user so, but we have to continue regardless to
			 * populate upstream information of the references we
			 * have already fetched above. The exception though is
			 * when `--atomic` is passed: in that case we'll abort
			 * the transaction and don't commit anything.
			 */
			જો (backfill_tags(&display_state, transport, transaction, tags_ref_map,
					  &fetch_head, config, &display_array, filter_options))
				retcode = 1;
		}

		free_refs(tags_ref_map);
	}

	જો (retcode)
		જાઓ cleanup;

	જો (verbosity >= 0)
		summary_width = transport_summary_width(ref_map);

	retcode = commit_ref_transaction(&transaction, atomic_fetch,
					 transport->remote->name,
					 &rejected_refs, &err);
	/*
	 * With '--atomic', bail out if the transaction fails. Without '--atomic',
	 * continue to fetch head and perform other post-fetch operations.
	 */
	જો (retcode && atomic_fetch)
		જાઓ cleanup;

	commit_fetch_head(&fetch_head);

	collect_updated_tips(updated_tips, ref_map);

	જો (set_upstream) {
		સંરચના branch *branch = branch_get("HEAD");
		સંરચના ref *rm;
		સંરચના ref *source_ref = ખાલી;

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
		માટે (rm = ref_map; rm; rm = rm->next) {
			જો (!rm->peer_ref) {
				જો (source_ref) {
					warning(_("multiple branches detected, incompatible with --set-upstream"));
					જાઓ cleanup;
				} નહીંતર {
					source_ref = rm;
				}
			}
		}
		જો (source_ref) {
			જો (!branch) {
				અચલ અક્ષર *shortname = source_ref->name;
				skip_prefix(shortname, "refs/heads/", &shortname);

				warning(_("could not set upstream of HEAD to '%s' from '%s' when "
					  "it does not point to any branch."),
					shortname, transport->remote->name);
				જાઓ cleanup;
			}

			જો (!strcmp(source_ref->name, "HEAD") ||
			    starts_with(source_ref->name, "refs/heads/"))
				install_branch_config(0,
						      branch->name,
						      transport->remote->name,
						      source_ref->name);
			નહીંતર જો (starts_with(source_ref->name, "refs/remotes/"))
				warning(_("not setting upstream for a remote remote-tracking branch"));
			નહીંતર જો (starts_with(source_ref->name, "refs/tags/"))
				warning(_("not setting upstream for a remote tag"));
			નહીંતર
				warning(_("unknown branch type"));
		} નહીંતર {
			warning(_("no source branch found;\n"
				  "you need to specify exactly one branch with the --set-upstream option"));
		}
	}
	જો (follow_remote_head != FOLLOW_REMOTE_NEVER) {
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
	જો (retcode && !atomic_fetch && transaction)
		commit_ref_transaction(&transaction, false,
				       transport->remote->name,
				       &rejected_refs, &err);

	માટે (size_t i = 0; i < display_array.nr; i++) {
		સંરચના ref_update_display_info *info = &display_array.info[i];

		જો (!info->failed && strmap_contains(&rejected_refs, info->ref))
			ref_update_display_info_set_failed(info);
		ref_update_display_info_display(info, &display_state, summary_width);
		ref_update_display_info_free(info);
	}

	જો (retcode) {
		જો (err.len) {
			error("%s", err.buf);
			strbuf_reset(&err);
		}
		જો (transaction && ref_transaction_abort(transaction, &err) &&
		    err.len)
			error("%s", err.buf);
		transaction = ખાલી;
	}

	જો (transaction)
		ref_transaction_free(transaction);

	મુક્ત(display_array.info);
	strmap_clear(&rejected_refs, 0);
	display_state_release(&display_state);
	close_fetch_head(&fetch_head);
	strbuf_release(&err);
	free_refs(ref_map);
	પાછા retcode;
}

સ્થિર પૂર્ણાંક get_one_remote_for_fetch(સંરચના remote *remote, શૂન્ય *priv)
{
	સંરચના string_list *list = priv;
	જો (!remote->skip_default_update)
		string_list_append(list, remote->name);
	પાછા 0;
}

સ્થિર શૂન્ય add_options_to_argv(સંરચના strvec *argv,
				અચલ સંરચના fetch_config *config)
{
	જો (dry_run)
		strvec_ધકેલો(argv, "--dry-run");
	જો (prune != -1)
		strvec_ધકેલો(argv, prune ? "--prune" : "--no-prune");
	જો (prune_tags != -1)
		strvec_ધકેલો(argv, prune_tags ? "--prune-tags" : "--no-prune-tags");
	જો (update_head_ok)
		strvec_ધકેલો(argv, "--update-head-ok");
	જો (force)
		strvec_ધકેલો(argv, "--force");
	જો (keep)
		strvec_ધકેલો(argv, "--keep");
	જો (config->recurse_submodules == RECURSE_SUBMODULES_ON)
		strvec_ધકેલો(argv, "--recurse-submodules");
	નહીંતર જો (config->recurse_submodules == RECURSE_SUBMODULES_OFF)
		strvec_ધકેલો(argv, "--no-recurse-submodules");
	નહીંતર જો (config->recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND)
		strvec_ધકેલો(argv, "--recurse-submodules=on-demand");
	જો (tags == TAGS_SET)
		strvec_ધકેલો(argv, "--tags");
	નહીંતર જો (tags == TAGS_UNSET)
		strvec_ધકેલો(argv, "--no-tags");
	જો (verbosity >= 2)
		strvec_ધકેલો(argv, "-v");
	જો (verbosity >= 1)
		strvec_ધકેલો(argv, "-v");
	નહીંતર જો (verbosity < 0)
		strvec_ધકેલો(argv, "-q");
	જો (family == TRANSPORT_FAMILY_IPV4)
		strvec_ધકેલો(argv, "--ipv4");
	નહીંતર જો (family == TRANSPORT_FAMILY_IPV6)
		strvec_ધકેલો(argv, "--ipv6");
	જો (!write_fetch_head)
		strvec_ધકેલો(argv, "--no-write-fetch-head");
	જો (config->display_format == DISPLAY_FORMAT_PORCELAIN)
		strvec_pushf(argv, "--porcelain");
}

/* Fetch multiple remotes in parallel */

સંરચના parallel_fetch_state {
	અચલ અક્ષર **argv;
	સંરચના string_list *remotes;
	પૂર્ણાંક next, result;
	અચલ સંરચના fetch_config *config;
};

સ્થિર પૂર્ણાંક fetch_next_remote(સંરચના child_process *cp,
			     સંરચના strbuf *out UNUSED,
			     શૂન્ય *cb, શૂન્ય **task_cb)
{
	સંરચના parallel_fetch_state *state = cb;
	અક્ષર *remote;

	જો (state->next < 0 || state->next >= state->remotes->nr)
		પાછા 0;

	remote = state->remotes->વસ્તુઓ[state->next++].string;
	*task_cb = remote;

	strvec_pushv(&cp->args, state->argv);
	strvec_ધકેલો(&cp->args, remote);
	cp->git_cmd = 1;

	જો (verbosity >= 0 && state->config->display_format != DISPLAY_FORMAT_PORCELAIN)
		printf(_("Fetching %s\n"), remote);

	પાછા 1;
}

સ્થિર પૂર્ણાંક fetch_failed_to_start(સંરચના strbuf *out UNUSED,
				 શૂન્ય *cb, શૂન્ય *task_cb)
{
	સંરચના parallel_fetch_state *state = cb;
	અચલ અક્ષર *remote = task_cb;

	state->result = error(_("could not fetch %s"), remote);

	પાછા 0;
}

સ્થિર પૂર્ણાંક fetch_finished(પૂર્ણાંક result, સંરચના strbuf *out,
			  શૂન્ય *cb, શૂન્ય *task_cb)
{
	સંરચના parallel_fetch_state *state = cb;
	અચલ અક્ષર *remote = task_cb;

	જો (result) {
		strbuf_addf(out, _("could not fetch '%s' (exit code: %d)\n"),
			    remote, result);
		state->result = -1;
	}

	પાછા 0;
}

સ્થિર પૂર્ણાંક fetch_multiple(સંરચના string_list *list, પૂર્ણાંક max_children,
			  અચલ સંરચના fetch_config *config)
{
	પૂર્ણાંક i, result = 0;
	સંરચના strvec argv = STRVEC_INIT;

	જો (!append && write_fetch_head) {
		પૂર્ણાંક errcode = truncate_fetch_head();
		જો (errcode)
			પાછા errcode;
	}

	/*
	 * Cancel out the fetch.bundleURI config when running subprocesses,
	 * to avoid fetching from the same bundle list multiple times.
	 */
	strvec_pushl(&argv, "-c", "fetch.bundleURI=",
		     "fetch", "--append", "--no-auto-gc",
		     "--no-write-commit-graph", ખાલી);
	માટે (i = 0; i < server_options.nr; i++)
		strvec_pushf(&argv, "--server-option=%s", server_options.વસ્તુઓ[i].string);
	add_options_to_argv(&argv, config);

	જો (max_children != 1 && list->nr != 1) {
		સંરચના parallel_fetch_state state = { argv.v, list, 0, 0, config };
		અચલ સંરચના run_process_parallel_opts opts = {
			.tr2_category = "fetch",
			.tr2_label = "parallel/fetch",

			.processes = max_children,

			.get_next_task = &fetch_next_remote,
			.start_failure = &fetch_failed_to_start,
			.task_finished = &fetch_finished,
			.data = &state,
		};

		strvec_ધકેલો(&argv, "--end-of-options");

		run_processes_parallel(&opts);
		result = state.result;
	} નહીંતર
		માટે (i = 0; i < list->nr; i++) {
			અચલ અક્ષર *name = list->વસ્તુઓ[i].string;
			સંરચના child_process cmd = CHILD_PROCESS_INIT;

			strvec_pushv(&cmd.args, argv.v);
			strvec_ધકેલો(&cmd.args, name);
			જો (verbosity >= 0 && config->display_format != DISPLAY_FORMAT_PORCELAIN)
				printf(_("Fetching %s\n"), name);
			cmd.git_cmd = 1;
			જો (run_command(&cmd)) {
				error(_("could not fetch %s"), name);
				result = 1;
			}
		}

	strvec_clear(&argv);
	પાછા !!result;
}

/*
 * Fetching from the promisor remote should use the given filter-spec
 * or inherit the default filter-spec from the config.
 */
સ્થિર અંતઃરેખ શૂન્ય fetch_one_setup_partial(સંરચના remote *remote,
					   સંરચના list_objects_filter_options *filter_options)
{
	/*
	 * Explicit --no-filter argument overrides everything, regardless
	 * of any prior partial clones and fetches.
	 */
	જો (filter_options->no_filter)
		પાછા;

	/*
	 * If no prior partial clone/fetch and the current fetch DID NOT
	 * request a partial-fetch, do a normal fetch.
	 */
	જો (!repo_has_promisor_remote(the_repository) && !filter_options->choice)
		પાછા;

	/*
	 * If this is a partial-fetch request, we enable partial on
	 * this repo if not already enabled and remember the given
	 * filter-spec as the default for subsequent fetches to this
	 * remote if there is currently no default filter-spec.
	 */
	જો (filter_options->choice) {
		partial_clone_register(remote->name, filter_options);
		પાછા;
	}

	/*
	 * Do a partial-fetch from the promisor remote using either the
	 * explicitly given filter-spec or inherit the filter-spec from
	 * the config.
	 */
	જો (!filter_options->choice)
		partial_clone_get_default_filter_spec(filter_options, remote->name);
	પાછા;
}

સ્થિર પૂર્ણાંક fetch_one(સંરચના remote *remote, પૂર્ણાંક argc, અચલ અક્ષર **argv,
		     પૂર્ણાંક prune_tags_ok, પૂર્ણાંક use_stdin_refspecs,
		     અચલ સંરચના fetch_config *config,
		     સંરચના list_objects_filter_options *filter_options,
		     સંરચના oidset *updated_tips)
{
	સંરચના refspec rs = REFSPEC_INIT_FETCH(the_hash_algo);
	પૂર્ણાંક i;
	પૂર્ણાંક exit_code;
	પૂર્ણાંક maybe_prune_tags;
	પૂર્ણાંક remote_via_config = remote_is_configured(remote, 0);

	જો (!remote)
		die(_("no remote repository specified; please specify either a URL or a\n"
		      "remote name from which new revisions should be fetched"));

	gtransport = prepare_transport(remote, 1, filter_options);

	જો (prune < 0) {
		/* no command line request */
		જો (0 <= remote->prune)
			prune = remote->prune;
		નહીંતર જો (0 <= config->prune)
			prune = config->prune;
		નહીંતર
			prune = PRUNE_BY_DEFAULT;
	}

	જો (prune_tags < 0) {
		/* no command line request */
		જો (0 <= remote->prune_tags)
			prune_tags = remote->prune_tags;
		નહીંતર જો (0 <= config->prune_tags)
			prune_tags = config->prune_tags;
		નહીંતર
			prune_tags = PRUNE_TAGS_BY_DEFAULT;
	}

	maybe_prune_tags = prune_tags_ok && prune_tags;
	જો (maybe_prune_tags && remote_via_config)
		refspec_append(&remote->fetch, TAG_REFSPEC);

	જો (maybe_prune_tags && (argc || !remote_via_config))
		refspec_append(&rs, TAG_REFSPEC);

	માટે (i = 0; i < argc; i++) {
		જો (!strcmp(argv[i], "tag")) {
			i++;
			જો (i >= argc)
				die(_("you need to specify a tag name"));

			refspec_appendf(&rs, "refs/tags/%s:refs/tags/%s",
					argv[i], argv[i]);
		} નહીંતર {
			refspec_append(&rs, argv[i]);
		}
	}

	જો (use_stdin_refspecs) {
		સંરચના strbuf લીટી = STRBUF_INIT;
		જ્યારે (strbuf_getline_lf(&લીટી, stdin) != EOF)
			refspec_append(&rs, લીટી.buf);
		strbuf_release(&લીટી);
	}

	જો (server_options.nr)
		gtransport->server_options = &server_options;

	sigchain_ધકેલો_common(unlock_pack_on_signal);
	atexit(unlock_pack_atexit);
	sigchain_ધકેલો(SIGPIPE, SIG_IGN);
	exit_code = do_fetch(gtransport, &rs, config, filter_options,
			     updated_tips);
	sigchain_કાઢો(SIGPIPE);
	refspec_clear(&rs);
	transport_disconnect(gtransport);
	gtransport = ખાલી;
	પાછા exit_code;
}

પૂર્ણાંક cmd_fetch(પૂર્ણાંક argc,
	      અચલ અક્ષર **argv,
	      અચલ અક્ષર *prefix,
	      સંરચના repository *repo UNUSED)
{
	સંરચના fetch_config config = {
		.display_format = DISPLAY_FORMAT_FULL,
		.follow_remote_head_raw = ખાલી,
		.follow_remote_head_seen = 0,
		.prune = -1,
		.prune_tags = -1,
		.show_forced_updates = 1,
		.recurse_submodules = RECURSE_SUBMODULES_DEFAULT,
		.parallel = 1,
		.submodule_fetch_jobs = -1,
	};
	અચલ અક્ષર *submodule_prefix = "";
	અચલ અક્ષર *bundle_uri;
	સંરચના string_list list = STRING_LIST_INIT_DUP;
	સંરચના list_objects_filter_options filter_options = LIST_OBJECTS_FILTER_INIT;
	સંરચના remote *remote = ખાલી;
	પૂર્ણાંક all = -1, multiple = 0;
	પૂર્ણાંક result = 0;
	પૂર્ણાંક prune_tags_ok = 1;
	પૂર્ણાંક enable_auto_gc = 1;
	પૂર્ણાંક unshallow = 0;
	પૂર્ણાંક max_jobs = -1;
	પૂર્ણાંક recurse_submodules_cli = RECURSE_SUBMODULES_DEFAULT;
	પૂર્ણાંક recurse_submodules_default = RECURSE_SUBMODULES_ON_DEMAND;
	પૂર્ણાંક fetch_write_commit_graph = -1;
	પૂર્ણાંક stdin_refspecs = 0;
	પૂર્ણાંક negotiate_only = 0;
	પૂર્ણાંક porcelain = 0;
	પૂર્ણાંક i;
	ગણના {
		GRAPH_WRITE_REACHABLE,
		GRAPH_WRITE_TIPS,
		GRAPH_WRITE_SKIP,
	} graph_write_mode = GRAPH_WRITE_REACHABLE;
	સંરચના oidset updated_tips = OIDSET_INIT;

	સંરચના option builtin_fetch_options[] = {
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
		OPT_SET_INT('n', ખાલી, &tags,
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
			.કિંમત = &submodule_prefix,
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
	માટે (i = 1; i < argc; i++) {
		/* This handles non-URLs gracefully */
		અક્ષર *anon = transport_anonymize_url(argv[i]);

		strbuf_addf(&default_rla, " %s", anon);
		મુક્ત(anon);
	}

	repo_config(the_repository, git_fetch_config, &config);
	જો (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_full_index = 0;
	}

	argc = parse_options(argc, argv, prefix,
			     builtin_fetch_options, builtin_fetch_usage, 0);

	જો (recurse_submodules_cli != RECURSE_SUBMODULES_DEFAULT)
		config.recurse_submodules = recurse_submodules_cli;

	જો (negotiate_only) {
		પસંદગી (recurse_submodules_cli) {
		સ્થિતિ RECURSE_SUBMODULES_OFF:
		સ્થિતિ RECURSE_SUBMODULES_DEFAULT:
			/*
			 * --negotiate-only should never recurse into
			 * submodules. Skip it by setting recurse_submodules to
			 * RECURSE_SUBMODULES_OFF.
			 */
			config.recurse_submodules = RECURSE_SUBMODULES_OFF;
			તોડો;

		મૂળભૂત:
			die(_("options '%s' and '%s' cannot be used together"),
			    "--negotiate-only", "--recurse-submodules");
		}
	}

	જો (config.recurse_submodules != RECURSE_SUBMODULES_OFF) {
		પૂર્ણાંક *sfjc = config.submodule_fetch_jobs == -1
			    ? &config.submodule_fetch_jobs : ખાલી;
		પૂર્ણાંક *rs = config.recurse_submodules == RECURSE_SUBMODULES_DEFAULT
			  ? &config.recurse_submodules : ખાલી;

		fetch_config_from_gitmodules(the_repository, sfjc, rs);
	}


	જો (porcelain) {
		પસંદગી (recurse_submodules_cli) {
		સ્થિતિ RECURSE_SUBMODULES_OFF:
		સ્થિતિ RECURSE_SUBMODULES_DEFAULT:
			/*
			 * Reference updates in submodules would be ambiguous
			 * in porcelain mode, so we reject this combination.
			 */
			config.recurse_submodules = RECURSE_SUBMODULES_OFF;
			તોડો;

		મૂળભૂત:
			die(_("options '%s' and '%s' cannot be used together"),
			    "--porcelain", "--recurse-submodules");
		}

		config.display_format = DISPLAY_FORMAT_PORCELAIN;
	}

	જો (deepen_relative) {
		જો (deepen_relative < 0)
			die(_("negative depth in --deepen is not supported"));
		જો (depth)
			die(_("options '%s' and '%s' cannot be used together"), "--deepen", "--depth");
		depth = xstrfmt("%d", deepen_relative);
	}
	જો (unshallow) {
		જો (depth)
			die(_("options '%s' and '%s' cannot be used together"), "--depth", "--unshallow");
		નહીંતર જો (!is_repository_shallow(the_repository))
			die(_("--unshallow on a complete repository does not make sense"));
		નહીંતર
			depth = xstrfmt("%d", INFINITE_DEPTH);
	}

	/* no need to be strict, transport_set_option() will validate it again */
	જો (depth && atoi(depth) < 1)
		die(_("depth %s is not a positive number"), depth);
	જો (depth || deepen_since || deepen_not.nr)
		deepen = 1;

	/* FETCH_HEAD never gets updated in --dry-run mode */
	જો (dry_run)
		write_fetch_head = 0;

	જો (!max_jobs)
		max_jobs = online_cpus();

	જો (!repo_config_get_string_tmp(the_repository, "fetch.bundleuri", &bundle_uri) &&
	    fetch_bundle_uri(the_repository, bundle_uri, ખાલી))
		warning(_("failed to fetch bundles from '%s'"), bundle_uri);

	જો (all < 0) {
		/*
		 * no --[no-]all given;
		 * only use config option if no remote was explicitly specified
		 */
		all = (!argc) ? config.all : 0;
	}

	જો (all) {
		જો (argc == 1)
			die(_("fetch --all does not take a repository argument"));
		નહીંતર જો (argc > 1)
			die(_("fetch --all does not make sense with refspecs"));

		(શૂન્ય) for_each_remote(get_one_remote_for_fetch, &list);

		/* do not do fetch_multiple() of one */
		જો (list.nr == 1)
			remote = remote_get(list.વસ્તુઓ[0].string);
	} નહીંતર જો (argc == 0) {
		/* No arguments -- use default remote */
		remote = remote_get(ખાલી);
	} નહીંતર જો (multiple) {
		/* All arguments are assumed to be remotes or groups */
		માટે (i = 0; i < argc; i++)
			જો (!add_remote_or_group(argv[i], &list))
				die(_("no such remote or remote group: %s"),
				    argv[i]);
	} નહીંતર {
		/* Single remote or group */
		(શૂન્ય) add_remote_or_group(argv[0], &list);
		જો (list.nr > 1) {
			/* More than one remote */
			જો (argc > 1)
				die(_("fetching a group and specifying refspecs does not make sense"));
		} નહીંતર {
			/* Zero or one remotes */
			remote = remote_get(argv[0]);
			prune_tags_ok = (argc == 1);
			argc--;
			argv++;
		}
	}
	string_list_remove_duplicates(&list, 0);

	જો (negotiate_only) {
		સંરચના oidset acked_commits = OIDSET_INIT;
		સંરચના oidset_iter iter;
		અચલ સંરચના object_id *oid;

		trace2_region_enter("fetch", "negotiate-only", the_repository);
		જો (!remote)
			die(_("must supply remote when using --negotiate-only"));
		gtransport = prepare_transport(remote, 1, &filter_options);

		જો (!gtransport->smart_options) {
			warning(_("protocol does not support --negotiate-only, exiting"));
			result = 1;
			trace2_region_leave("fetch", "negotiate-only", the_repository);
			જાઓ cleanup;
		}
		જો (!gtransport->smart_options->negotiation_restrict_tips)
			die(_("%s needs one or more %s"), "--negotiate-only",
			    "--negotiation-restrict=*");

		gtransport->smart_options->acked_commits = &acked_commits;

		જો (server_options.nr)
			gtransport->server_options = &server_options;
		result = transport_fetch_refs(gtransport, ખાલી);
		gtransport->smart_options->acked_commits = ખાલી;

		oidset_iter_init(&acked_commits, &iter);
		જ્યારે ((oid = oidset_iter_next(&iter)))
			printf("%s\n", oid_to_hex(oid));
		oidset_clear(&acked_commits);
		trace2_region_leave("fetch", "negotiate-only", the_repository);
	} નહીંતર જો (remote) {
		જો (filter_options.choice || repo_has_promisor_remote(the_repository)) {
			trace2_region_enter("fetch", "setup-partial", the_repository);
			fetch_one_setup_partial(remote, &filter_options);
			trace2_region_leave("fetch", "setup-partial", the_repository);
		}
		trace2_region_enter("fetch", "fetch-one", the_repository);
		result = fetch_one(remote, argc, argv, prune_tags_ok, stdin_refspecs,
				   &config, &filter_options, &updated_tips);
		જો (prepare_commit_graph(the_repository)) {
			જો (oidset_size(&updated_tips))
				graph_write_mode = GRAPH_WRITE_TIPS;
			નહીંતર
				graph_write_mode = GRAPH_WRITE_SKIP;
		}
		trace2_region_leave("fetch", "fetch-one", the_repository);
	} નહીંતર {
		પૂર્ણાંક max_children = max_jobs;

		જો (filter_options.choice)
			die(_("--filter can only be used with the remote "
			      "configured in extensions.partialclone"));

		જો (atomic_fetch)
			die(_("--atomic can only be used when fetching "
			      "from one remote"));

		જો (stdin_refspecs)
			die(_("--stdin can only be used when fetching "
			      "from one remote"));

		જો (max_children < 0)
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
	જો (!result && remote && (config.recurse_submodules != RECURSE_SUBMODULES_OFF)) {
		સંરચના strvec options = STRVEC_INIT;
		પૂર્ણાંક max_children = max_jobs;

		જો (max_children < 0)
			max_children = config.submodule_fetch_jobs;
		જો (max_children < 0)
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
	જો (negotiate_only)
		જાઓ cleanup;

	prepare_repo_settings(the_repository);
	જો (fetch_write_commit_graph > 0 ||
	    (fetch_write_commit_graph < 0 &&
	     the_repository->settings.fetch_write_commit_graph)) {
		પૂર્ણાંક commit_graph_flags = COMMIT_GRAPH_WRITE_SPLIT;

		જો (progress)
			commit_graph_flags |= COMMIT_GRAPH_WRITE_PROGRESS;

		જો (graph_write_mode != GRAPH_WRITE_SKIP) {
			trace2_region_enter("fetch", "write-commit-graph",
					    the_repository);
			જો (graph_write_mode == GRAPH_WRITE_TIPS)
				write_commit_graph(
					the_repository,
					the_repository->objects->source->path,
					ખાલી, &updated_tips,
					commit_graph_flags, ખાલી);
			નહીંતર
				write_commit_graph_reachable(
					the_repository,
					the_repository->objects->source->path,
					commit_graph_flags, ખાલી);
			trace2_region_leave("fetch", "write-commit-graph",
					    the_repository);
		}
	}

	જો (enable_auto_gc) {
		જો (refetch) {
			/*
			 * Hint auto-maintenance strongly to encourage repacking,
			 * but respect config settings disabling it.
			 */
			પૂર્ણાંક opt_val;
			જો (repo_config_get_int(the_repository, "gc.autopacklimit", &opt_val))
				opt_val = -1;
			જો (opt_val != 0)
				git_config_ધકેલો_parameter("gc.autoPackLimit=1");

			જો (repo_config_get_int(the_repository, "maintenance.incremental-repack.auto", &opt_val))
				opt_val = -1;
			જો (opt_val != 0)
				git_config_ધકેલો_parameter("maintenance.incremental-repack.auto=-1");
		}
		run_auto_maintenance(the_repository, verbosity < 0);
	}

 cleanup:
	oidset_clear(&updated_tips);
	string_list_clear(&list, 0);
	list_objects_filter_release(&filter_options);
	મુક્ત(config.follow_remote_head_raw);
	પાછા result;
}
