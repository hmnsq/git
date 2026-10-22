/*
 * "git fetch"
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "advice.h"
#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "environment.h"
#അന്തര്ഭാവ "hex.h"
#അന്തര്ഭാവ "refs.h"
#അന്തര്ഭാവ "refspec.h"
#അന്തര്ഭാവ "object-name.h"
#അന്തര്ഭാവ "odb.h"
#അന്തര്ഭാവ "odb/source.h"
#അന്തര്ഭാവ "oidset.h"
#അന്തര്ഭാവ "oid-array.h"
#അന്തര്ഭാവ "commit.h"
#അന്തര്ഭാവ "string-list.h"
#അന്തര്ഭാവ "remote.h"
#അന്തര്ഭാവ "transport.h"
#അന്തര്ഭാവ "run-command.h"
#അന്തര്ഭാവ "parse-options.h"
#അന്തര്ഭാവ "sigchain.h"
#അന്തര്ഭാവ "submodule-config.h"
#അന്തര്ഭാവ "submodule.h"
#അന്തര്ഭാവ "connected.h"
#അന്തര്ഭാവ "strvec.h"
#അന്തര്ഭാവ "utf8.h"
#അന്തര്ഭാവ "pager.h"
#അന്തര്ഭാവ "path.h"
#അന്തര്ഭാവ "pkt-line.h"
#അന്തര്ഭാവ "list-objects-filter-options.h"
#അന്തര്ഭാവ "commit-reach.h"
#അന്തര്ഭാവ "branch.h"
#അന്തര്ഭാവ "promisor-remote.h"
#അന്തര്ഭാവ "commit-graph.h"
#അന്തര്ഭാവ "shallow.h"
#അന്തര്ഭാവ "trace.h"
#അന്തര്ഭാവ "trace2.h"
#അന്തര്ഭാവ "bundle-uri.h"

#define FORCED_UPDATES_DELAY_WARNING_IN_MS (10 * 1000)

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം builtin_fetch_usage[] = {
	N_("git fetch [<options>] [<repository> [<refspec>...]]"),
	N_("git fetch [<options>] <group>"),
	N_("git fetch --multiple [<options>] [(<repository>|<group>)...]"),
	N_("git fetch --all [<options>]"),
	ശൂന്യ
};

എണ്ണൽ {
	TAGS_UNSET = 0,
	TAGS_DEFAULT = 1,
	TAGS_SET = 2
};

എണ്ണൽ display_format {
	DISPLAY_FORMAT_FULL,
	DISPLAY_FORMAT_COMPACT,
	DISPLAY_FORMAT_PORCELAIN,
};

ഘടന display_state {
	ഘടന strbuf buf;

	പൂർണ്ണസംഖ്യ refcol_width;
	എണ്ണൽ display_format format;

	അക്ഷരം *url;
	പൂർണ്ണസംഖ്യ url_len, shown_url;
};

സ്ഥിതം uint64_t forced_updates_ms = 0;
സ്ഥിതം പൂർണ്ണസംഖ്യ prefetch = 0;
സ്ഥിതം പൂർണ്ണസംഖ്യ prune = -1; /* unspecified */
#define PRUNE_BY_DEFAULT 0 /* do we prune by default? */

സ്ഥിതം പൂർണ്ണസംഖ്യ prune_tags = -1; /* unspecified */
#define PRUNE_TAGS_BY_DEFAULT 0 /* do we prune tags by default? */

സ്ഥിതം പൂർണ്ണസംഖ്യ append, dry_run, force, keep, update_head_ok;
സ്ഥിതം പൂർണ്ണസംഖ്യ write_fetch_head = 1;
സ്ഥിതം പൂർണ്ണസംഖ്യ verbosity, deepen_relative, set_upstream, refetch;
സ്ഥിതം പൂർണ്ണസംഖ്യ progress = -1;
സ്ഥിതം പൂർണ്ണസംഖ്യ tags = TAGS_DEFAULT, update_shallow, deepen;
സ്ഥിതം പൂർണ്ണസംഖ്യ atomic_fetch;
സ്ഥിതം എണ്ണൽ transport_family family;
സ്ഥിതം സ്ഥിരം അക്ഷരം *depth;
സ്ഥിതം സ്ഥിരം അക്ഷരം *deepen_since;
സ്ഥിതം സ്ഥിരം അക്ഷരം *upload_pack;
സ്ഥിതം ഘടന string_list deepen_not = STRING_LIST_INIT_NODUP;
സ്ഥിതം ഘടന strbuf default_rla = STRBUF_INIT;
സ്ഥിതം ഘടന transport *gtransport;
സ്ഥിതം ഘടന transport *gsecondary;
സ്ഥിതം ഘടന refspec refmap;
സ്ഥിതം ഘടന string_list server_options = STRING_LIST_INIT_DUP;
സ്ഥിതം ഘടന string_list negotiation_restrict = STRING_LIST_INIT_NODUP;
സ്ഥിതം ഘടന string_list negotiation_include = STRING_LIST_INIT_NODUP;

ഘടന fetch_config {
	എണ്ണൽ display_format display_format;
	അക്ഷരം *follow_remote_head_raw;
	പൂർണ്ണസംഖ്യ follow_remote_head_seen;
	പൂർണ്ണസംഖ്യ all;
	പൂർണ്ണസംഖ്യ prune;
	പൂർണ്ണസംഖ്യ prune_tags;
	പൂർണ്ണസംഖ്യ show_forced_updates;
	പൂർണ്ണസംഖ്യ recurse_submodules;
	പൂർണ്ണസംഖ്യ parallel;
	പൂർണ്ണസംഖ്യ submodule_fetch_jobs;
};

സ്ഥിതം പൂർണ്ണസംഖ്യ git_fetch_config(സ്ഥിരം അക്ഷരം *k, സ്ഥിരം അക്ഷരം *v,
			    സ്ഥിരം ഘടന config_context *ctx, ശൂന്യം *cb)
{
	ഘടന fetch_config *fetch_config = cb;

	എങ്കിൽ (!strcmp(k, "fetch.all")) {
		fetch_config->all = git_config_bool(k, v);
		തിരികെ 0;
	}

	എങ്കിൽ (!strcmp(k, "fetch.prune")) {
		fetch_config->prune = git_config_bool(k, v);
		തിരികെ 0;
	}

	എങ്കിൽ (!strcmp(k, "fetch.prunetags")) {
		fetch_config->prune_tags = git_config_bool(k, v);
		തിരികെ 0;
	}

	എങ്കിൽ (!strcmp(k, "fetch.showforcedupdates")) {
		fetch_config->show_forced_updates = git_config_bool(k, v);
		തിരികെ 0;
	}

	എങ്കിൽ (!strcmp(k, "submodule.recurse")) {
		പൂർണ്ണസംഖ്യ r = git_config_bool(k, v) ?
			RECURSE_SUBMODULES_ON : RECURSE_SUBMODULES_OFF;
		fetch_config->recurse_submodules = r;
		തിരികെ 0;
	}

	എങ്കിൽ (!strcmp(k, "submodule.fetchjobs")) {
		fetch_config->submodule_fetch_jobs = parse_submodule_fetchjobs(k, v, ctx->kvi);
		തിരികെ 0;
	} അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(k, "fetch.recursesubmodules")) {
		fetch_config->recurse_submodules = parse_fetch_recurse_submodules_arg(k, v);
		തിരികെ 0;
	}

	എങ്കിൽ (!strcmp(k, "fetch.parallel")) {
		fetch_config->parallel = git_config_int(k, v, ctx->kvi);
		എങ്കിൽ (fetch_config->parallel < 0)
			die(_("fetch.parallel cannot be negative"));
		എങ്കിൽ (!fetch_config->parallel)
			fetch_config->parallel = online_cpus();
		തിരികെ 0;
	}

	എങ്കിൽ (!strcmp(k, "fetch.output")) {
		എങ്കിൽ (!v)
			തിരികെ config_error_nonbool(k);
		അല്ലെങ്കിൽ എങ്കിൽ (!strcasecmp(v, "full"))
			fetch_config->display_format = DISPLAY_FORMAT_FULL;
		അല്ലെങ്കിൽ എങ്കിൽ (!strcasecmp(v, "compact"))
			fetch_config->display_format = DISPLAY_FORMAT_COMPACT;
		അല്ലെങ്കിൽ
			die(_("invalid value for '%s': '%s'"),
			    "fetch.output", v);
		തിരികെ 0;
	}

	എങ്കിൽ (!strcmp(k, "fetch.followremotehead")) {
		സ്വതന്ത്രം(fetch_config->follow_remote_head_raw);
		fetch_config->follow_remote_head_raw = xstrdup_or_null(v);
		fetch_config->follow_remote_head_seen = 1;
		തിരികെ 0;
	}

	തിരികെ git_default_config(k, v, ctx, cb);
}

സ്ഥിതം എണ്ണൽ follow_remote_head_settings get_follow_remote_head(സ്ഥിരം അക്ഷരം *setting)
{
	എങ്കിൽ (!setting)
		die(_("missing value for 'fetch.followRemoteHEAD'"));
	അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(setting, "never"))
		തിരികെ FOLLOW_REMOTE_NEVER;
	അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(setting, "create"))
		തിരികെ FOLLOW_REMOTE_CREATE;
	അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(setting, "warn"))
		തിരികെ FOLLOW_REMOTE_WARN;
	അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(setting, "always"))
		തിരികെ FOLLOW_REMOTE_ALWAYS;
	warning(_("unrecognized fetch.followRemoteHEAD value '%s' ignored"), setting);
	തിരികെ BUILTIN_FOLLOW_REMOTE_HEAD_DFLT;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ parse_refmap_arg(സ്ഥിരം ഘടന option *opt, സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	BUG_ON_OPT_NEG(unset);

	/*
	 * "git fetch --refmap='' origin foo"
	 * can be used to tell the command not to store anywhere
	 */
	refspec_append(opt->മൂല്യം, arg);

	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം unlock_pack(unsigned പൂർണ്ണസംഖ്യ flags)
{
	എങ്കിൽ (gtransport)
		transport_unlock_pack(gtransport, flags);
	എങ്കിൽ (gsecondary)
		transport_unlock_pack(gsecondary, flags);
}

സ്ഥിതം ശൂന്യം unlock_pack_atexit(ശൂന്യം)
{
	unlock_pack(0);
}

സ്ഥിതം ശൂന്യം unlock_pack_on_signal(പൂർണ്ണസംഖ്യ signo)
{
	unlock_pack(TRANSPORT_UNLOCK_PACK_IN_SIGNAL_HANDLER);
	sigchain_എടുക്കുക(signo);
	ഉയർത്തുക(signo);
}

സ്ഥിതം ശൂന്യം add_merge_config(ഘടന ref **head,
			   സ്ഥിരം ഘടന ref *remote_refs,
		           ഘടന branch *branch,
		           ഘടന ref ***tail)
{
	പൂർണ്ണസംഖ്യ i;

	വേണ്ടി (i = 0; i < branch->merge_nr; i++) {
		ഘടന ref *rm, **old_tail = *tail;
		ഘടന refspec_item refspec;

		വേണ്ടി (rm = *head; rm; rm = rm->next) {
			എങ്കിൽ (branch_merge_matches(branch, i, rm->name)) {
				rm->fetch_head_status = FETCH_HEAD_MERGE;
				നിർത്തുക;
			}
		}
		എങ്കിൽ (rm)
			തുടരുക;

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
		memset(&refspec, 0, വലുപ്പം(refspec));
		refspec.src = branch->merge[i]->src;
		get_fetch_map(remote_refs, &refspec, tail, 1);
		വേണ്ടി (rm = *old_tail; rm; rm = rm->next)
			rm->fetch_head_status = FETCH_HEAD_MERGE;
	}
}

സ്ഥിതം ശൂന്യം create_fetch_oidset(ഘടന ref **head, ഘടന oidset *out)
{
	ഘടന ref *rm = *head;
	ആകുമ്പോൾ (rm) {
		oidset_തിരുകുക(out, &rm->old_oid);
		rm = rm->next;
	}
}

ഘടന refname_hash_entry {
	ഘടന hashmap_entry ent;
	ഘടന object_id oid;
	പൂർണ്ണസംഖ്യ ignore;
	അക്ഷരം refname[FLEX_ARRAY];
};

സ്ഥിതം പൂർണ്ണസംഖ്യ refname_hash_entry_cmp(സ്ഥിരം ശൂന്യം *hashmap_cmp_fn_data UNUSED,
				  സ്ഥിരം ഘടന hashmap_entry *eptr,
				  സ്ഥിരം ഘടന hashmap_entry *entry_or_താക്കോൽ,
				  സ്ഥിരം ശൂന്യം *keydata)
{
	സ്ഥിരം ഘടന refname_hash_entry *e1, *e2;

	e1 = container_of(eptr, സ്ഥിരം ഘടന refname_hash_entry, ent);
	e2 = container_of(entry_or_താക്കോൽ, സ്ഥിരം ഘടന refname_hash_entry, ent);
	തിരികെ strcmp(e1->refname, keydata ? keydata : e2->refname);
}

സ്ഥിതം ഘടന refname_hash_entry *refname_hash_add(ഘടന hashmap *map,
						   സ്ഥിരം അക്ഷരം *refname,
						   സ്ഥിരം ഘടന object_id *oid)
{
	ഘടന refname_hash_entry *ent;
	size_t len = strlen(refname);

	FLEX_ALLOC_MEM(ent, refname, refname, len);
	hashmap_entry_init(&ent->ent, strhash(refname));
	oidcpy(&ent->oid, oid);
	hashmap_add(map, &ent->ent);
	തിരികെ ent;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ add_one_refname(സ്ഥിരം ഘടന reference *ref, ശൂന്യം *cbdata)
{
	ഘടന hashmap *refname_map = cbdata;

	(ശൂന്യം) refname_hash_add(refname_map, ref->name, ref->oid);
	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം refname_hash_init(ഘടന hashmap *map)
{
	hashmap_init(map, refname_hash_entry_cmp, ശൂന്യ, 0);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ refname_hash_exists(ഘടന hashmap *map, സ്ഥിരം അക്ഷരം *refname)
{
	തിരികെ !!hashmap_get_from_hash(map, strhash(refname), refname);
}

സ്ഥിതം ശൂന്യം clear_item(ഘടന refname_hash_entry *item)
{
	item->ignore = 1;
}


സ്ഥിതം ശൂന്യം add_already_queued_tags(സ്ഥിരം അക്ഷരം *refname,
				    സ്ഥിരം ഘടന object_id *old_oid UNUSED,
				    സ്ഥിരം ഘടന object_id *new_oid,
				    ശൂന്യം *cb_data)
{
	ഘടന hashmap *queued_tags = cb_data;
	എങ്കിൽ (starts_with(refname, "refs/tags/") && new_oid)
		(ശൂന്യം) refname_hash_add(queued_tags, refname, new_oid);
}

സ്ഥിതം ശൂന്യം find_non_local_tags(സ്ഥിരം ഘടന ref *refs,
				ഘടന ref_transaction *transaction,
				ഘടന ref **head,
				ഘടന ref ***tail)
{
	ഘടന hashmap existing_refs;
	ഘടന hashmap remote_refs;
	ഘടന oidset fetch_oids = OIDSET_INIT;
	ഘടന string_list remote_refs_list = STRING_LIST_INIT_NODUP;
	ഘടന string_list_item *remote_ref_item;
	സ്ഥിരം ഘടന ref *ref;
	ഘടന refname_hash_entry *item = ശൂന്യ;

	refname_hash_init(&existing_refs);
	refname_hash_init(&remote_refs);
	create_fetch_oidset(head, &fetch_oids);

	refs_for_each_ref(get_main_ref_store(the_repository), add_one_refname,
			  &existing_refs);

	/*
	 * If we already have a transaction, then we need to filter out all
	 * tags which have already been queued up.
	 */
	എങ്കിൽ (transaction)
		ref_transaction_for_each_queued_update(transaction,
						       add_already_queued_tags,
						       &existing_refs);

	വേണ്ടി (ref = refs; ref; ref = ref->next) {
		എങ്കിൽ (!starts_with(ref->name, "refs/tags/"))
			തുടരുക;

		/*
		 * The peeled ref always follows the matching base
		 * ref, so if we see a peeled ref that we don't want
		 * to fetch then we can mark the ref entry in the list
		 * as one to ignore by setting util to NULL.
		 */
		എങ്കിൽ (ends_with(ref->name, "^{}")) {
			എങ്കിൽ (item &&
			    !odb_has_object(the_repository->objects, &ref->old_oid, 0) &&
			    !oidset_contains(&fetch_oids, &ref->old_oid) &&
			    !odb_has_object(the_repository->objects, &item->oid, 0) &&
			    !oidset_contains(&fetch_oids, &item->oid))
				clear_item(item);
			item = ശൂന്യ;
			തുടരുക;
		}

		/*
		 * If item is non-NULL here, then we previously saw a
		 * ref not followed by a peeled reference, so we need
		 * to check if it is a lightweight tag that we want to
		 * fetch.
		 */
		എങ്കിൽ (item &&
		    !odb_has_object(the_repository->objects, &item->oid, 0) &&
		    !oidset_contains(&fetch_oids, &item->oid))
			clear_item(item);

		item = ശൂന്യ;

		/* skip duplicates and refs that we already have */
		എങ്കിൽ (refname_hash_exists(&remote_refs, ref->name) ||
		    refname_hash_exists(&existing_refs, ref->name))
			തുടരുക;

		item = refname_hash_add(&remote_refs, ref->name, &ref->old_oid);
		string_list_തിരുകുക(&remote_refs_list, ref->name);
	}
	hashmap_clear_and_free(&existing_refs, ഘടന refname_hash_entry, ent);

	/*
	 * We may have a final lightweight tag that needs to be
	 * checked to see if it needs fetching.
	 */
	എങ്കിൽ (item &&
	    !odb_has_object(the_repository->objects, &item->oid, 0) &&
	    !oidset_contains(&fetch_oids, &item->oid))
		clear_item(item);

	/*
	 * For all the tags in the remote_refs_list,
	 * add them to the list of refs to be fetched
	 */
	for_each_string_list_item(remote_ref_item, &remote_refs_list) {
		സ്ഥിരം അക്ഷരം *refname = remote_ref_item->string;
		ഘടന ref *rm;
		unsigned പൂർണ്ണസംഖ്യ hash = strhash(refname);

		item = hashmap_get_entry_from_hash(&remote_refs, hash, refname,
					ഘടന refname_hash_entry, ent);
		എങ്കിൽ (!item)
			BUG("unseen remote ref?");

		/* Unless we have already decided to ignore this item... */
		എങ്കിൽ (item->ignore)
			തുടരുക;

		rm = alloc_ref(item->refname);
		rm->peer_ref = alloc_ref(item->refname);
		oidcpy(&rm->old_oid, &item->oid);
		**tail = rm;
		*tail = &rm->next;
	}
	hashmap_clear_and_free(&remote_refs, ഘടന refname_hash_entry, ent);
	string_list_clear(&remote_refs_list, 0);
	oidset_clear(&fetch_oids);
}

സ്ഥിതം ശൂന്യം filter_prefetch_refspec(ഘടന refspec *rs)
{
	പൂർണ്ണസംഖ്യ i;

	എങ്കിൽ (!prefetch)
		തിരികെ;

	വേണ്ടി (i = 0; i < rs->nr; i++) {
		ഘടന strbuf new_dst = STRBUF_INIT;
		അക്ഷരം *old_dst;
		സ്ഥിരം അക്ഷരം *sub = ശൂന്യ;

		എങ്കിൽ (rs->ഇനങ്ങൾ[i].negative)
			തുടരുക;
		എങ്കിൽ (!rs->ഇനങ്ങൾ[i].dst ||
		    (rs->ഇനങ്ങൾ[i].src &&
		     starts_with(rs->ഇനങ്ങൾ[i].src,
				 ref_namespace[NAMESPACE_TAGS].ref))) {
			പൂർണ്ണസംഖ്യ j;

			refspec_item_clear(&rs->ഇനങ്ങൾ[i]);

			വേണ്ടി (j = i + 1; j < rs->nr; j++)
				rs->ഇനങ്ങൾ[j - 1] = rs->ഇനങ്ങൾ[j];
			rs->nr--;
			i--;
			തുടരുക;
		}

		old_dst = rs->ഇനങ്ങൾ[i].dst;
		strbuf_addstr(&new_dst, ref_namespace[NAMESPACE_PREFETCH].ref);

		/*
		 * If old_dst starts with "refs/", then place
		 * sub after that prefix. Otherwise, start at
		 * the beginning of the string.
		 */
		എങ്കിൽ (!skip_prefix(old_dst, "refs/", &sub))
			sub = old_dst;
		strbuf_addstr(&new_dst, sub);

		rs->ഇനങ്ങൾ[i].dst = strbuf_detach(&new_dst, ശൂന്യ);
		rs->ഇനങ്ങൾ[i].force = 1;

		സ്വതന്ത്രം(old_dst);
	}
}

സ്ഥിതം ഘടന ref *get_ref_map(ഘടന remote *remote,
			       സ്ഥിരം ഘടന ref *remote_refs,
			       ഘടന refspec *rs,
			       പൂർണ്ണസംഖ്യ tags, പൂർണ്ണസംഖ്യ *autotags)
{
	പൂർണ്ണസംഖ്യ i;
	ഘടന ref *rm;
	ഘടന ref *ref_map = ശൂന്യ;
	ഘടന ref **tail = &ref_map;

	/* opportunistically-updated references: */
	ഘടന ref *orefs = ശൂന്യ, **oref_tail = &orefs;

	ഘടന hashmap existing_refs;
	പൂർണ്ണസംഖ്യ existing_refs_populated = 0;

	filter_prefetch_refspec(rs);
	എങ്കിൽ (remote)
		filter_prefetch_refspec(&remote->fetch);

	എങ്കിൽ (rs->nr) {
		ഘടന refspec *fetch_refspec;

		വേണ്ടി (i = 0; i < rs->nr; i++) {
			get_fetch_map(remote_refs, &rs->ഇനങ്ങൾ[i], &tail, 0);
			എങ്കിൽ (rs->ഇനങ്ങൾ[i].dst && rs->ഇനങ്ങൾ[i].dst[0])
				*autotags = 1;
		}
		/* Merge everything on the command line (but not --tags) */
		വേണ്ടി (rm = ref_map; rm; rm = rm->next)
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
		എങ്കിൽ (refmap.nr)
			fetch_refspec = &refmap;
		അല്ലെങ്കിൽ
			fetch_refspec = &remote->fetch;

		വേണ്ടി (i = 0; i < fetch_refspec->nr; i++)
			get_fetch_map(ref_map, &fetch_refspec->ഇനങ്ങൾ[i], &oref_tail, 1);
	} അല്ലെങ്കിൽ എങ്കിൽ (refmap.nr) {
		die("--refmap option is only meaningful with command-line refspec(s)");
	} അല്ലെങ്കിൽ {
		/* Use the defaults */
		ഘടന branch *branch = branch_get(ശൂന്യ);
		പൂർണ്ണസംഖ്യ has_merge = branch_has_merge_config(branch);
		എങ്കിൽ (remote &&
		    (remote->fetch.nr ||
		     /* Note: has_merge implies non-NULL branch->remote_name */
		     (has_merge && !strcmp(branch->remote_name, remote->name)))) {
			വേണ്ടി (i = 0; i < remote->fetch.nr; i++) {
				get_fetch_map(remote_refs, &remote->fetch.ഇനങ്ങൾ[i], &tail, 0);
				എങ്കിൽ (remote->fetch.ഇനങ്ങൾ[i].dst &&
				    remote->fetch.ഇനങ്ങൾ[i].dst[0])
					*autotags = 1;
				എങ്കിൽ (!i && !has_merge && ref_map &&
				    !remote->fetch.ഇനങ്ങൾ[0].pattern)
					ref_map->fetch_head_status = FETCH_HEAD_MERGE;
			}
			/*
			 * if the remote we're fetching from is the same
			 * as given in branch.<name>.remote, we add the
			 * ref given in branch.<name>.merge, too.
			 *
			 * Note: has_merge implies non-NULL branch->remote_name
			 */
			എങ്കിൽ (has_merge &&
			    !strcmp(branch->remote_name, remote->name))
				add_merge_config(&ref_map, remote_refs, branch, &tail);
		} അല്ലെങ്കിൽ എങ്കിൽ (!prefetch) {
			ref_map = get_remote_ref(remote_refs, "HEAD");
			എങ്കിൽ (!ref_map)
				die(_("couldn't find remote ref HEAD"));
			ref_map->fetch_head_status = FETCH_HEAD_MERGE;
			tail = &ref_map->next;
		}
	}

	എങ്കിൽ (tags == TAGS_SET) {
		ഘടന refspec_item tag_refspec;

		/* also fetch all tags */
		refspec_item_init_തള്ളുക(&tag_refspec, TAG_REFSPEC,
				       the_hash_algo);
		get_fetch_map(remote_refs, &tag_refspec, &tail, 0);
		refspec_item_clear(&tag_refspec);
	} അല്ലെങ്കിൽ എങ്കിൽ (tags == TAGS_DEFAULT && *autotags) {
		find_non_local_tags(remote_refs, ശൂന്യ, &ref_map, &tail);
	}

	/* Now append any refs to be updated opportunistically: */
	*tail = orefs;
	വേണ്ടി (rm = orefs; rm; rm = rm->next) {
		rm->fetch_head_status = FETCH_HEAD_IGNORE;
		tail = &rm->next;
	}

	/*
	 * apply negative refspecs first, before we remove duplicates. This is
	 * necessary as negative refspecs might remove an otherwise conflicting
	 * duplicate.
	 */
	എങ്കിൽ (rs->nr)
		ref_map = apply_negative_refspecs(ref_map, rs);
	അല്ലെങ്കിൽ
		ref_map = apply_negative_refspecs(ref_map, &remote->fetch);

	ref_map = ref_remove_duplicates(ref_map);

	വേണ്ടി (rm = ref_map; rm; rm = rm->next) {
		എങ്കിൽ (rm->peer_ref) {
			സ്ഥിരം അക്ഷരം *refname = rm->peer_ref->name;
			ഘടന refname_hash_entry *peer_item;
			unsigned പൂർണ്ണസംഖ്യ hash = strhash(refname);

			എങ്കിൽ (!existing_refs_populated) {
				refname_hash_init(&existing_refs);
				refs_for_each_ref(get_main_ref_store(the_repository),
						  add_one_refname,
						  &existing_refs);
				existing_refs_populated = 1;
			}

			peer_item = hashmap_get_entry_from_hash(&existing_refs,
						hash, refname,
						ഘടന refname_hash_entry, ent);
			എങ്കിൽ (peer_item) {
				ഘടന object_id *old_oid = &peer_item->oid;
				oidcpy(&rm->peer_ref->old_oid, old_oid);
			}
		}
	}
	എങ്കിൽ (existing_refs_populated)
		hashmap_clear_and_free(&existing_refs, ഘടന refname_hash_entry, ent);

	തിരികെ ref_map;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ s_update_ref(സ്ഥിരം അക്ഷരം *action,
			ഘടന ref *ref,
			ഘടന ref_transaction *transaction,
			പൂർണ്ണസംഖ്യ check_old)
{
	അക്ഷരം *സന്ദേശം;
	അക്ഷരം *rla = getenv("GIT_REFLOG_ACTION");
	ഘടന strbuf err = STRBUF_INIT;
	പൂർണ്ണസംഖ്യ ret;

	എങ്കിൽ (dry_run)
		തിരികെ 0;
	എങ്കിൽ (!rla)
		rla = default_rla.buf;
	സന്ദേശം = xstrfmt("%s: %s", rla, action);

	ret = ref_transaction_update(transaction, ref->name, &ref->new_oid,
				     check_old ? &ref->old_oid : ശൂന്യ,
				     ശൂന്യ, ശൂന്യ, 0, സന്ദേശം, &err);

	എങ്കിൽ (ret)
		error("%s", err.buf);
	strbuf_release(&err);
	സ്വതന്ത്രം(സന്ദേശം);
	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ refcol_width(സ്ഥിരം ഘടന ref *ref_map, പൂർണ്ണസംഖ്യ compact_format)
{
	സ്ഥിരം ഘടന ref *ref;
	പൂർണ്ണസംഖ്യ max, width = 10;

	max = term_columns();
	എങ്കിൽ (compact_format)
		max = max * 2 / 3;

	വേണ്ടി (ref = ref_map; ref; ref = ref->next) {
		പൂർണ്ണസംഖ്യ rlen, llen = 0, len;

		എങ്കിൽ (ref->status == REF_STATUS_REJECT_SHALLOW ||
		    !ref->peer_ref ||
		    !strcmp(ref->name, "HEAD"))
			തുടരുക;

		/* uptodate lines are only shown on high verbosity level */
		എങ്കിൽ (verbosity <= 0 && oideq(&ref->peer_ref->old_oid, &ref->old_oid))
			തുടരുക;

		rlen = utf8_strwidth(prettify_refname(ref->name));
		എങ്കിൽ (!compact_format)
			llen = utf8_strwidth(prettify_refname(ref->peer_ref->name));

		/*
		 * rough estimation to see if the output line is too long and
		 * should not be counted (we can't do precise calculation
		 * anyway because we don't know if the error explanation part
		 * will be printed in update_local_ref)
		 */
		len = 21 /* flag and summary */ + rlen + 4 /* -> */ + llen;
		എങ്കിൽ (len >= max)
			തുടരുക;

		എങ്കിൽ (width < rlen)
			width = rlen;
	}

	തിരികെ width;
}

സ്ഥിതം ശൂന്യം display_state_init(ഘടന display_state *display_state, ഘടന ref *ref_map,
			       സ്ഥിരം അക്ഷരം *raw_url, എണ്ണൽ display_format format)
{
	പൂർണ്ണസംഖ്യ i;

	memset(display_state, 0, വലുപ്പം(*display_state));
	strbuf_init(&display_state->buf, 0);
	display_state->format = format;

	എങ്കിൽ (raw_url)
		display_state->url = transport_anonymize_url(raw_url);
	അല്ലെങ്കിൽ
		display_state->url = xstrdup("foreign");

	display_state->url_len = strlen(display_state->url);
	വേണ്ടി (i = display_state->url_len - 1; 0 <= i && display_state->url[i] == '/'; i--)
		;
	display_state->url_len = i + 1;
	എങ്കിൽ (4 < i && !strncmp(".git", display_state->url + i - 3, 4))
		display_state->url_len = i - 3;

	എങ്കിൽ (verbosity < 0)
		തിരികെ;

	തിരഞ്ഞെടുപ്പ് (display_state->format) {
	അവസ്ഥ DISPLAY_FORMAT_FULL:
	അവസ്ഥ DISPLAY_FORMAT_COMPACT:
		display_state->refcol_width = refcol_width(ref_map,
							   display_state->format == DISPLAY_FORMAT_COMPACT);
		നിർത്തുക;
	അവസ്ഥ DISPLAY_FORMAT_PORCELAIN:
		/* We don't need to precompute anything here. */
		നിർത്തുക;
	സ്ഥിരസ്ഥിതി:
		BUG("unexpected display format %d", display_state->format);
	}
}

സ്ഥിതം ശൂന്യം display_state_release(ഘടന display_state *display_state)
{
	strbuf_release(&display_state->buf);
	സ്വതന്ത്രം(display_state->url);
}

സ്ഥിതം ശൂന്യം print_remote_to_local(ഘടന display_state *display_state,
				  സ്ഥിരം അക്ഷരം *remote, സ്ഥിരം അക്ഷരം *local)
{
	strbuf_addf(&display_state->buf, "%-*s -> %s",
		    display_state->refcol_width, remote, local);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ find_and_replace(ഘടന strbuf *haystack,
			    സ്ഥിരം അക്ഷരം *needle,
			    സ്ഥിരം അക്ഷരം *placeholder)
{
	സ്ഥിരം അക്ഷരം *p = ശൂന്യ;
	പൂർണ്ണസംഖ്യ plen, nlen;

	nlen = strlen(needle);
	എങ്കിൽ (ends_with(haystack->buf, needle))
		p = haystack->buf + haystack->len - nlen;
	അല്ലെങ്കിൽ
		p = strstr(haystack->buf, needle);
	എങ്കിൽ (!p)
		തിരികെ 0;

	എങ്കിൽ (p > haystack->buf && p[-1] != '/')
		തിരികെ 0;

	plen = strlen(p);
	എങ്കിൽ (plen > nlen && p[nlen] != '/')
		തിരികെ 0;

	strbuf_splice(haystack, p - haystack->buf, nlen,
		      placeholder, strlen(placeholder));
	തിരികെ 1;
}

സ്ഥിതം ശൂന്യം print_compact(ഘടന display_state *display_state,
			  സ്ഥിരം അക്ഷരം *remote, സ്ഥിരം അക്ഷരം *local)
{
	ഘടന strbuf r = STRBUF_INIT;
	ഘടന strbuf l = STRBUF_INIT;

	എങ്കിൽ (!strcmp(remote, local)) {
		strbuf_addf(&display_state->buf, "%-*s -> *", display_state->refcol_width, remote);
		തിരികെ;
	}

	strbuf_addstr(&r, remote);
	strbuf_addstr(&l, local);

	എങ്കിൽ (!find_and_replace(&r, local, "*"))
		find_and_replace(&l, remote, "*");
	print_remote_to_local(display_state, r.buf, l.buf);

	strbuf_release(&r);
	strbuf_release(&l);
}

സ്ഥിതം ശൂന്യം display_ref_update(ഘടന display_state *display_state, അക്ഷരം code,
			       സ്ഥിരം അക്ഷരം *summary, സ്ഥിരം അക്ഷരം *error,
			       സ്ഥിരം അക്ഷരം *remote, സ്ഥിരം അക്ഷരം *local,
			       സ്ഥിരം ഘടന object_id *old_oid,
			       സ്ഥിരം ഘടന object_id *new_oid,
			       പൂർണ്ണസംഖ്യ summary_width)
{
	FILE *f = stderr;

	എങ്കിൽ (verbosity < 0)
		തിരികെ;

	strbuf_reset(&display_state->buf);

	തിരഞ്ഞെടുപ്പ് (display_state->format) {
	അവസ്ഥ DISPLAY_FORMAT_FULL:
	അവസ്ഥ DISPLAY_FORMAT_COMPACT: {
		പൂർണ്ണസംഖ്യ width;

		എങ്കിൽ (!display_state->shown_url) {
			strbuf_addf(&display_state->buf, _("From %.*s\n"),
				    display_state->url_len, display_state->url);
			display_state->shown_url = 1;
		}

		width = (summary_width + strlen(summary) - gettext_width(summary));
		remote = prettify_refname(remote);
		local = prettify_refname(local);

		strbuf_addf(&display_state->buf, " %c %-*s ", code, width, summary);

		എങ്കിൽ (display_state->format != DISPLAY_FORMAT_COMPACT)
			print_remote_to_local(display_state, remote, local);
		അല്ലെങ്കിൽ
			print_compact(display_state, remote, local);

		എങ്കിൽ (error)
			strbuf_addf(&display_state->buf, "  (%s)", error);

		നിർത്തുക;
	}
	അവസ്ഥ DISPLAY_FORMAT_PORCELAIN:
		strbuf_addf(&display_state->buf, "%c %s %s %s", code,
			    oid_to_hex(old_oid), oid_to_hex(new_oid), local);
		f = stdout;
		നിർത്തുക;
	സ്ഥിരസ്ഥിതി:
		BUG("unexpected display format %d", display_state->format);
	};
	strbuf_addch(&display_state->buf, '\n');

	fputs(display_state->buf.buf, f);
}

ഘടന ref_update_display_info {
	bool failed;
	അക്ഷരം success_code;
	അക്ഷരം fail_code;
	അക്ഷരം *summary;
	അക്ഷരം *fail_detail;
	അക്ഷരം *success_detail;
	അക്ഷരം *ref;
	അക്ഷരം *remote;
	ഘടന object_id old_oid;
	ഘടന object_id new_oid;
};

ഘടന ref_update_display_info_array {
	ഘടന ref_update_display_info *info;
	size_t alloc, nr;
};

സ്ഥിതം ഘടന ref_update_display_info *ref_update_display_info_append(
					   ഘടന ref_update_display_info_array *array,
					   അക്ഷരം success_code,
					   അക്ഷരം fail_code,
					   സ്ഥിരം അക്ഷരം *summary,
					   സ്ഥിരം അക്ഷരം *success_detail,
					   സ്ഥിരം അക്ഷരം *fail_detail,
					   സ്ഥിരം അക്ഷരം *ref,
					   സ്ഥിരം അക്ഷരം *remote,
					   സ്ഥിരം ഘടന object_id *old_oid,
					   സ്ഥിരം ഘടന object_id *new_oid)
{
	ഘടന ref_update_display_info *info;

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

	തിരികെ info;
}

സ്ഥിതം ശൂന്യം ref_update_display_info_set_failed(ഘടന ref_update_display_info *info)
{
	info->failed = true;
}

സ്ഥിതം ശൂന്യം ref_update_display_info_free(ഘടന ref_update_display_info *info)
{
	സ്വതന്ത്രം(info->summary);
	സ്വതന്ത്രം(info->success_detail);
	സ്വതന്ത്രം(info->fail_detail);
	സ്വതന്ത്രം(info->remote);
	സ്വതന്ത്രം(info->ref);
}

സ്ഥിതം ശൂന്യം ref_update_display_info_display(ഘടന ref_update_display_info *info,
					    ഘടന display_state *display_state,
					    പൂർണ്ണസംഖ്യ summary_width)
{
	display_ref_update(display_state,
			   info->failed ? info->fail_code : info->success_code,
			   info->summary,
			   info->failed ? info->fail_detail : info->success_detail,
			   info->remote, info->ref, &info->old_oid,
			   &info->new_oid, summary_width);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ update_local_ref(ഘടന ref *ref,
			    ഘടന ref_transaction *transaction,
			    സ്ഥിരം ഘടന ref *remote_ref,
			    സ്ഥിരം ഘടന fetch_config *config,
			    ഘടന ref_update_display_info_array *display_array)
{
	ഘടന commit *current = ശൂന്യ, *updated;
	പൂർണ്ണസംഖ്യ fast_forward = 0;

	എങ്കിൽ (!odb_has_object(the_repository->objects, &ref->new_oid,
			    ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR))
		die(_("object %s not found"), oid_to_hex(&ref->new_oid));

	എങ്കിൽ (oideq(&ref->old_oid, &ref->new_oid)) {
		എങ്കിൽ (verbosity > 0)
			ref_update_display_info_append(display_array, '=', '=',
						       _("[up to date]"), ശൂന്യ,
						       ശൂന്യ, ref->name,
						       remote_ref->name, &ref->old_oid,
						       &ref->new_oid);
		തിരികെ 0;
	}

	എങ്കിൽ (!update_head_ok &&
	    !is_null_oid(&ref->old_oid) &&
	    branch_checked_out(ref->name)) {
		ഘടന ref_update_display_info *info;
		/*
		 * If this is the head, and it's not okay to update
		 * the head, and the old value of the head isn't empty...
		 */
		info = ref_update_display_info_append(display_array, '!', '!',
						      _("[rejected]"), ശൂന്യ,
						      _("can't fetch into checked-out branch"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);
		ref_update_display_info_set_failed(info);
		തിരികെ 1;
	}

	എങ്കിൽ (!is_null_oid(&ref->old_oid) &&
	    starts_with(ref->name, "refs/tags/")) {
		ഘടന ref_update_display_info *info;

		എങ്കിൽ (force || ref->force) {
			പൂർണ്ണസംഖ്യ r;

			r = s_update_ref("updating tag", ref, transaction, 0);

			info = ref_update_display_info_append(display_array, 't', '!',
							      _("[tag update]"), ശൂന്യ,
							      _("unable to update local ref"),
							      ref->name, remote_ref->name,
							      &ref->old_oid, &ref->new_oid);
			എങ്കിൽ (r)
				ref_update_display_info_set_failed(info);

			തിരികെ r;
		} അല്ലെങ്കിൽ {
			info = ref_update_display_info_append(display_array, '!', '!',
							      _("[rejected]"), ശൂന്യ,
							      _("would clobber existing tag"),
							      ref->name, remote_ref->name,
							      &ref->old_oid, &ref->new_oid);
			ref_update_display_info_set_failed(info);
			തിരികെ 1;
		}
	}

	current = lookup_commit_reference_gently(the_repository,
						 &ref->old_oid, 1);
	updated = lookup_commit_reference_gently(the_repository,
						 &ref->new_oid, 1);
	എങ്കിൽ (!current || !updated) {
		ഘടന ref_update_display_info *info;
		സ്ഥിരം അക്ഷരം *സന്ദേശം;
		സ്ഥിരം അക്ഷരം *what;
		പൂർണ്ണസംഖ്യ r;
		/*
		 * Nicely describe the new ref we're fetching.
		 * Base this on the remote's ref name, as it's
		 * more likely to follow a standard layout.
		 */
		എങ്കിൽ (starts_with(remote_ref->name, "refs/tags/")) {
			സന്ദേശം = "storing tag";
			what = _("[new tag]");
		} അല്ലെങ്കിൽ എങ്കിൽ (starts_with(remote_ref->name, "refs/heads/")) {
			സന്ദേശം = "storing head";
			what = _("[new branch]");
		} അല്ലെങ്കിൽ {
			സന്ദേശം = "storing ref";
			what = _("[new ref]");
		}

		r = s_update_ref(സന്ദേശം, ref, transaction, 0);

		info = ref_update_display_info_append(display_array, '*', '!',
						      what, ശൂന്യ,
						      _("unable to update local ref"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);
		എങ്കിൽ (r)
			ref_update_display_info_set_failed(info);

		തിരികെ r;
	}

	എങ്കിൽ (config->show_forced_updates) {
		uint64_t t_before = getnanotime();
		fast_forward = repo_in_merge_bases(the_repository, current,
						   updated);
		എങ്കിൽ (fast_forward < 0)
			die(ശൂന്യ);
		forced_updates_ms += (getnanotime() - t_before) / 1000000;
	} അല്ലെങ്കിൽ {
		fast_forward = 1;
	}

	എങ്കിൽ (fast_forward) {
		ഘടന ref_update_display_info *info;
		ഘടന strbuf quickref = STRBUF_INIT;
		പൂർണ്ണസംഖ്യ r;

		strbuf_add_unique_abbrev(&quickref, &current->object.oid, DEFAULT_ABBREV);
		strbuf_addstr(&quickref, "..");
		strbuf_add_unique_abbrev(&quickref, &ref->new_oid, DEFAULT_ABBREV);
		r = s_update_ref("fast-forward", ref, transaction, 1);

		info = ref_update_display_info_append(display_array, ' ', '!',
						      quickref.buf, ശൂന്യ,
						      _("unable to update local ref"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);
		എങ്കിൽ (r)
			ref_update_display_info_set_failed(info);

		strbuf_release(&quickref);
		തിരികെ r;
	} അല്ലെങ്കിൽ എങ്കിൽ (force || ref->force) {
		ഘടന ref_update_display_info *info;
		ഘടന strbuf quickref = STRBUF_INIT;
		പൂർണ്ണസംഖ്യ r;

		strbuf_add_unique_abbrev(&quickref, &current->object.oid, DEFAULT_ABBREV);
		strbuf_addstr(&quickref, "...");
		strbuf_add_unique_abbrev(&quickref, &ref->new_oid, DEFAULT_ABBREV);
		r = s_update_ref("forced-update", ref, transaction, 1);

		info = ref_update_display_info_append(display_array, '+', '!',
						      quickref.buf, _("forced update"),
						      _("unable to update local ref"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);

		എങ്കിൽ (r)
			ref_update_display_info_set_failed(info);

		strbuf_release(&quickref);
		തിരികെ r;
	} അല്ലെങ്കിൽ {
		ഘടന ref_update_display_info *info;
		info = ref_update_display_info_append(display_array, '!', '!',
						      _("[rejected]"), ശൂന്യ,
						      _("non-fast-forward"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);
		ref_update_display_info_set_failed(info);
		തിരികെ 1;
	}
}

സ്ഥിതം സ്ഥിരം ഘടന object_id *iterate_ref_map(ശൂന്യം *cb_data)
{
	ഘടന ref **rm = cb_data;
	ഘടന ref *ref = *rm;

	ആകുമ്പോൾ (ref && ref->status == REF_STATUS_REJECT_SHALLOW)
		ref = ref->next;
	എങ്കിൽ (!ref)
		തിരികെ ശൂന്യ;
	*rm = ref->next;
	തിരികെ &ref->old_oid;
}

ഘടന fetch_head {
	FILE *fp;
	ഘടന strbuf buf;
};

സ്ഥിതം പൂർണ്ണസംഖ്യ open_fetch_head(ഘടന fetch_head *fetch_head)
{
	സ്ഥിരം അക്ഷരം *filename = git_path_fetch_head(the_repository);

	എങ്കിൽ (write_fetch_head) {
		fetch_head->fp = fopen(filename, "a");
		എങ്കിൽ (!fetch_head->fp)
			തിരികെ error_errno(_("cannot open '%s'"), filename);
		strbuf_init(&fetch_head->buf, 0);
	} അല്ലെങ്കിൽ {
		fetch_head->fp = ശൂന്യ;
	}

	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം append_fetch_head(ഘടന fetch_head *fetch_head,
			      സ്ഥിരം ഘടന object_id *old_oid,
			      എണ്ണൽ fetch_head_status fetch_head_status,
			      സ്ഥിരം അക്ഷരം *note,
			      സ്ഥിരം അക്ഷരം *url, size_t url_len)
{
	അക്ഷരം old_oid_hex[GIT_MAX_HEXSZ + 1];
	സ്ഥിരം അക്ഷരം *merge_status_marker;
	size_t i;

	എങ്കിൽ (!fetch_head->fp)
		തിരികെ;

	തിരഞ്ഞെടുപ്പ് (fetch_head_status) {
	അവസ്ഥ FETCH_HEAD_NOT_FOR_MERGE:
		merge_status_marker = "not-for-merge";
		നിർത്തുക;
	അവസ്ഥ FETCH_HEAD_MERGE:
		merge_status_marker = "";
		നിർത്തുക;
	സ്ഥിരസ്ഥിതി:
		/* do not write anything to FETCH_HEAD */
		തിരികെ;
	}

	strbuf_addf(&fetch_head->buf, "%s\t%s\t%s",
		    oid_to_hex_r(old_oid_hex, old_oid), merge_status_marker, note);
	വേണ്ടി (i = 0; i < url_len; ++i)
		എങ്കിൽ ('\n' == url[i])
			strbuf_addstr(&fetch_head->buf, "\\n");
		അല്ലെങ്കിൽ
			strbuf_addch(&fetch_head->buf, url[i]);
	strbuf_addch(&fetch_head->buf, '\n');

	/*
	 * When using an atomic fetch, we do not want to update FETCH_HEAD if
	 * any of the reference updates fails. We thus have to write all
	 * updates to a buffer first and only commit it as soon as all
	 * references have been successfully updated.
	 */
	എങ്കിൽ (!atomic_fetch) {
		strbuf_write(&fetch_head->buf, fetch_head->fp);
		strbuf_reset(&fetch_head->buf);
	}
}

സ്ഥിതം ശൂന്യം commit_fetch_head(ഘടന fetch_head *fetch_head)
{
	എങ്കിൽ (!fetch_head->fp || !atomic_fetch)
		തിരികെ;
	strbuf_write(&fetch_head->buf, fetch_head->fp);
}

സ്ഥിതം ശൂന്യം close_fetch_head(ഘടന fetch_head *fetch_head)
{
	എങ്കിൽ (!fetch_head->fp)
		തിരികെ;

	fclose(fetch_head->fp);
	strbuf_release(&fetch_head->buf);
}

സ്ഥിതം സ്ഥിരം അക്ഷരം warn_show_forced_updates[] =
N_("fetch normally indicates which branches had a forced update,\n"
   "but that check has been disabled; to re-enable, use '--show-forced-updates'\n"
   "flag or run 'git config fetch.showForcedUpdates true'");
സ്ഥിതം സ്ഥിരം അക്ഷരം warn_time_show_forced_updates[] =
N_("it took %.2f seconds to check forced updates; you can use\n"
   "'--no-show-forced-updates' or run 'git config fetch.showForcedUpdates false'\n"
   "to avoid this check\n");

സ്ഥിതം പൂർണ്ണസംഖ്യ store_updated_refs(ഘടന display_state *display_state,
			      പൂർണ്ണസംഖ്യ connectivity_checked,
			      ഘടന ref_transaction *transaction, ഘടന ref *ref_map,
			      ഘടന fetch_head *fetch_head,
			      സ്ഥിരം ഘടന fetch_config *config,
			      ഘടന ref_update_display_info_array *display_array)
{
	പൂർണ്ണസംഖ്യ rc = 0;
	ഘടന strbuf note = STRBUF_INIT;
	സ്ഥിരം അക്ഷരം *what, *kind;
	ഘടന ref *rm;
	പൂർണ്ണസംഖ്യ want_status;

	എങ്കിൽ (!connectivity_checked) {
		ഘടന check_connected_options opt = CHECK_CONNECTED_INIT;

		opt.exclude_hidden_refs_section = "fetch";
		rm = ref_map;
		എങ്കിൽ (check_connected(iterate_ref_map, &rm, &opt)) {
			rc = error(_("%s did not send all necessary objects"),
				   display_state->url);
			പോകുക abort;
		}
	}

	/*
	 * We do a pass for each fetch_head_status type in their enum order, so
	 * merged entries are written before not-for-merge. That lets readers
	 * use FETCH_HEAD as a refname to refer to the ref to be merged.
	 */
	വേണ്ടി (want_status = FETCH_HEAD_MERGE;
	     want_status <= FETCH_HEAD_IGNORE;
	     want_status++) {
		വേണ്ടി (rm = ref_map; rm; rm = rm->next) {
			ഘടന ref *ref = ശൂന്യ;

			എങ്കിൽ (rm->status == REF_STATUS_REJECT_SHALLOW) {
				എങ്കിൽ (want_status == FETCH_HEAD_MERGE)
					warning(_("rejected %s because shallow roots are not allowed to be updated"),
						rm->peer_ref ? rm->peer_ref->name : rm->name);
				തുടരുക;
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
			എങ്കിൽ (fetch_head->fp) {
				ഘടന commit *commit = ശൂന്യ;

				/*
				 * References in "refs/tags/" are often going to point
				 * to annotated tags, which are not part of the
				 * commit-graph. We thus only try to look up refs in
				 * the graph which are not in that namespace to not
				 * regress performance in repositories with many
				 * annotated tags.
				 */
				എങ്കിൽ (!starts_with(rm->name, "refs/tags/"))
					commit = lookup_commit_in_graph(the_repository, &rm->old_oid);
				എങ്കിൽ (!commit) {
					commit = lookup_commit_reference_gently(the_repository,
										&rm->old_oid,
										1);
					എങ്കിൽ (!commit)
						rm->fetch_head_status = FETCH_HEAD_NOT_FOR_MERGE;
				}
			}

			എങ്കിൽ (rm->fetch_head_status != want_status)
				തുടരുക;

			എങ്കിൽ (rm->peer_ref) {
				ref = alloc_ref(rm->peer_ref->name);
				oidcpy(&ref->old_oid, &rm->peer_ref->old_oid);
				oidcpy(&ref->new_oid, &rm->old_oid);
				ref->force = rm->peer_ref->force;
			}

			എങ്കിൽ (config->recurse_submodules != RECURSE_SUBMODULES_OFF &&
			    (!rm->peer_ref || !oideq(&ref->old_oid, &ref->new_oid))) {
				check_for_new_submodule_commits(&rm->old_oid);
			}

			എങ്കിൽ (!strcmp(rm->name, "HEAD")) {
				kind = "";
				what = "";
			} അല്ലെങ്കിൽ എങ്കിൽ (skip_prefix(rm->name, "refs/heads/", &what)) {
				kind = "branch";
			} അല്ലെങ്കിൽ എങ്കിൽ (skip_prefix(rm->name, "refs/tags/", &what)) {
				kind = "tag";
			} അല്ലെങ്കിൽ എങ്കിൽ (skip_prefix(rm->name, "refs/remotes/", &what)) {
				kind = "remote-tracking branch";
			} അല്ലെങ്കിൽ {
				kind = "";
				what = rm->name;
			}

			strbuf_reset(&note);
			എങ്കിൽ (*what) {
				എങ്കിൽ (*kind)
					strbuf_addf(&note, "%s ", kind);
				strbuf_addf(&note, "'%s' of ", what);
			}

			append_fetch_head(fetch_head, &rm->old_oid,
					  rm->fetch_head_status,
					  note.buf, display_state->url,
					  display_state->url_len);

			എങ്കിൽ (ref) {
				rc |= update_local_ref(ref, transaction, rm,
						       config, display_array);
				സ്വതന്ത്രം(ref);
			} അല്ലെങ്കിൽ എങ്കിൽ (write_fetch_head || dry_run) {
				/*
				 * Display fetches written to FETCH_HEAD (or
				 * would be written to FETCH_HEAD, if --dry-run
				 * is set).
				 */

				ref_update_display_info_append(display_array, '*', '*',
							       *kind ? kind : "branch",
							       ശൂന്യ, ശൂന്യ, "FETCH_HEAD",
							       rm->name, &rm->new_oid,
							       &rm->old_oid);
			}
		}
	}

	എങ്കിൽ (advice_enabled(ADVICE_FETCH_SHOW_FORCED_UPDATES)) {
		എങ്കിൽ (!config->show_forced_updates) {
			warning(_(warn_show_forced_updates));
		} അല്ലെങ്കിൽ എങ്കിൽ (forced_updates_ms > FORCED_UPDATES_DELAY_WARNING_IN_MS) {
			warning(_(warn_time_show_forced_updates),
				forced_updates_ms / 1000.0);
		}
	}

 abort:
	strbuf_release(&note);
	തിരികെ rc;
}

/*
 * We would want to bypass the object transfer altogether if
 * everything we are going to fetch already exists and is connected
 * locally.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ check_exist_and_connected(ഘടന ref *ref_map)
{
	ഘടന ref *rm = ref_map;
	ഘടന check_connected_options opt = CHECK_CONNECTED_INIT;
	ഘടന ref *r;

	/*
	 * If we are deepening a shallow clone we already have these
	 * objects reachable.  Running rev-list here will return with
	 * a good (0) exit status and we'll bypass the fetch that we
	 * really need to perform.  Claiming failure now will ensure
	 * we perform the network exchange to deepen our history.
	 */
	എങ്കിൽ (deepen)
		തിരികെ -1;

	/*
	 * Similarly, if we need to refetch, we always want to perform a full
	 * fetch ignoring existing objects.
	 */
	എങ്കിൽ (refetch)
		തിരികെ -1;


	/*
	 * check_connected() allows objects to merely be promised, but
	 * we need all direct targets to exist.
	 */
	വേണ്ടി (r = rm; r; r = r->next) {
		എങ്കിൽ (!odb_has_object(the_repository->objects, &r->old_oid,
				    ODB_HAS_OBJECT_RECHECK_PACKED))
			തിരികെ -1;
	}

	opt.quiet = 1;
	opt.exclude_hidden_refs_section = "fetch";
	തിരികെ check_connected(iterate_ref_map, &rm, &opt);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ fetch_and_consume_refs(ഘടന display_state *display_state,
				  ഘടന transport *transport,
				  ഘടന ref_transaction *transaction,
				  ഘടന ref *ref_map,
				  ഘടന fetch_head *fetch_head,
				  സ്ഥിരം ഘടന fetch_config *config,
				  ഘടന ref_update_display_info_array *display_array)
{
	പൂർണ്ണസംഖ്യ connectivity_checked = 1;
	പൂർണ്ണസംഖ്യ ret;

	/*
	 * We don't need to perform a fetch in case we can already satisfy all
	 * refs.
	 */
	ret = check_exist_and_connected(ref_map);
	എങ്കിൽ (ret) {
		trace2_region_enter("fetch", "fetch_refs", the_repository);
		ret = transport_fetch_refs(transport, ref_map);
		trace2_region_leave("fetch", "fetch_refs", the_repository);
		എങ്കിൽ (ret)
			പോകുക out;
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
	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ prune_refs(ഘടന display_state *display_state,
		      ഘടന refspec *rs,
		      ഘടന ref_transaction *transaction,
		      ഘടന ref *ref_map)
{
	പൂർണ്ണസംഖ്യ result = 0;
	ഘടന ref *ref, *stale_refs = get_stale_heads(rs, ref_map);
	ഘടന strbuf err = STRBUF_INIT;
	ഘടന string_list refnames = STRING_LIST_INIT_NODUP;

	വേണ്ടി (ref = stale_refs; ref; ref = ref->next)
		string_list_append(&refnames, ref->name);

	എങ്കിൽ (!dry_run) {
		എങ്കിൽ (transaction) {
			വേണ്ടി (ref = stale_refs; ref; ref = ref->next) {
				result = ref_transaction_delete(transaction, ref->name, ശൂന്യ,
								ശൂന്യ, 0, "fetch: prune", &err);
				എങ്കിൽ (result)
					പോകുക cleanup;
			}
		} അല്ലെങ്കിൽ {
			result = refs_delete_refs(get_main_ref_store(the_repository),
						  "fetch: prune", &refnames,
						  0);
		}
	}

	എങ്കിൽ (verbosity >= 0) {
		പൂർണ്ണസംഖ്യ summary_width = transport_summary_width(stale_refs);

		വേണ്ടി (ref = stale_refs; ref; ref = ref->next) {
			display_ref_update(display_state, '-', _("[deleted]"), ശൂന്യ,
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
	തിരികെ result;
}

സ്ഥിതം ശൂന്യം check_not_current_branch(ഘടന ref *ref_map)
{
	സ്ഥിരം അക്ഷരം *path;
	വേണ്ടി (; ref_map; ref_map = ref_map->next)
		എങ്കിൽ (ref_map->peer_ref &&
		    starts_with(ref_map->peer_ref->name, "refs/heads/") &&
		    (path = branch_checked_out(ref_map->peer_ref->name)))
			die(_("refusing to fetch into branch '%s' "
			      "checked out at '%s'"),
			    ref_map->peer_ref->name, path);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ truncate_fetch_head(ശൂന്യം)
{
	സ്ഥിരം അക്ഷരം *filename = git_path_fetch_head(the_repository);
	FILE *fp = fopen_for_writing(filename);

	എങ്കിൽ (!fp)
		തിരികെ error_errno(_("cannot open '%s'"), filename);
	fclose(fp);
	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം set_option(ഘടന transport *transport, സ്ഥിരം അക്ഷരം *name, സ്ഥിരം അക്ഷരം *മൂല്യം)
{
	പൂർണ്ണസംഖ്യ r = transport_set_option(transport, name, മൂല്യം);
	എങ്കിൽ (r < 0)
		die(_("option \"%s\" value \"%s\" is not valid for %s"),
		    name, മൂല്യം, transport->url);
	എങ്കിൽ (r > 0)
		warning(_("option \"%s\" is ignored for %s"),
			name, transport->url);
}


സ്ഥിതം പൂർണ്ണസംഖ്യ add_oid(സ്ഥിരം ഘടന reference *ref, ശൂന്യം *cb_data)
{
	ഘടന oid_array *oids = cb_data;

	oid_array_append(oids, ref->oid);
	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം add_negotiation_tips(ഘടന string_list *നിവേശം_list,
				 ഘടന oid_array **output_list,
				 സ്ഥിരം അക്ഷരം *argname)
{
	ഘടന oid_array *oids = xcalloc(1, വലുപ്പം(*oids));
	പൂർണ്ണസംഖ്യ i;

	വേണ്ടി (i = 0; i < നിവേശം_list->nr; i++) {
		സ്ഥിരം അക്ഷരം *s = നിവേശം_list->ഇനങ്ങൾ[i].string;
		ഘടന refs_for_each_ref_options opts = {
			.pattern = s,
		};
		പൂർണ്ണസംഖ്യ old_nr;
		എങ്കിൽ (!has_glob_specials(s)) {
			ഘടന object_id oid;

			/* Ignore missing reference. */
			എങ്കിൽ (repo_get_oid(the_repository, s, &oid))
				തുടരുക;
			/* Fail on missing object pointed by ref. */
			എങ്കിൽ (!odb_has_object(the_repository->objects, &oid, 0))
				die(_("the object %s does not exist"), s);

			oid_array_append(oids, &oid);
			തുടരുക;
		}
		old_nr = oids->nr;
		refs_for_each_ref_ext(get_main_ref_store(the_repository),
				      add_oid, oids, &opts);
		എങ്കിൽ (old_nr == oids->nr)
			warning(_("ignoring %s=%s because it does not match any refs"),
				argname, s);
	}
	*output_list = oids;
}

സ്ഥിതം ഘടന transport *prepare_transport(ഘടന remote *remote, പൂർണ്ണസംഖ്യ deepen,
					   ഘടന list_objects_filter_options *filter_options)
{
	ഘടന transport *transport;

	transport = transport_get(remote, ശൂന്യ);
	transport_set_verbosity(transport, verbosity, progress);
	transport->family = family;
	എങ്കിൽ (upload_pack)
		set_option(transport, TRANS_OPT_UPLOADPACK, upload_pack);
	എങ്കിൽ (keep)
		set_option(transport, TRANS_OPT_KEEP, "yes");
	എങ്കിൽ (depth)
		set_option(transport, TRANS_OPT_DEPTH, depth);
	എങ്കിൽ (deepen && deepen_since)
		set_option(transport, TRANS_OPT_DEEPEN_SINCE, deepen_since);
	എങ്കിൽ (deepen && deepen_not.nr)
		set_option(transport, TRANS_OPT_DEEPEN_NOT,
			   (സ്ഥിരം അക്ഷരം *)&deepen_not);
	എങ്കിൽ (deepen_relative)
		set_option(transport, TRANS_OPT_DEEPEN_RELATIVE, "yes");
	എങ്കിൽ (update_shallow)
		set_option(transport, TRANS_OPT_UPDATE_SHALLOW, "yes");
	എങ്കിൽ (refetch)
		set_option(transport, TRANS_OPT_REFETCH, "yes");
	എങ്കിൽ (filter_options->choice) {
		സ്ഥിരം അക്ഷരം *spec =
			expand_list_objects_filter_spec(filter_options);
		set_option(transport, TRANS_OPT_LIST_OBJECTS_FILTER, spec);
		set_option(transport, TRANS_OPT_FROM_PROMISOR, "1");
	}
	എങ്കിൽ (negotiation_restrict.nr) {
		എങ്കിൽ (transport->smart_options)
			add_negotiation_tips(&negotiation_restrict,
					     &transport->smart_options->negotiation_restrict_tips,
					     "--negotiation-restrict");
		അല്ലെങ്കിൽ
			warning(_("ignoring %s because the protocol does not support it"),
				"--negotiation-restrict");
	} അല്ലെങ്കിൽ എങ്കിൽ (remote->negotiation_restrict.nr) {
		ഘടന string_list_item *item;
		for_each_string_list_item(item, &remote->negotiation_restrict)
			string_list_append(&negotiation_restrict, item->string);
		എങ്കിൽ (transport->smart_options)
			add_negotiation_tips(&negotiation_restrict,
					     &transport->smart_options->negotiation_restrict_tips,
					     "--negotiation-restrict");
		അല്ലെങ്കിൽ {
			ഘടന strbuf config_name = STRBUF_INIT;
			strbuf_addf(&config_name, "remote.%s.negotiationRestrict", remote->name);
			warning(_("ignoring %s because the protocol does not support it"),
				config_name.buf);
			strbuf_release(&config_name);
		}
	}
	എങ്കിൽ (negotiation_include.nr) {
		എങ്കിൽ (transport->smart_options)
			add_negotiation_tips(&negotiation_include,
					     &transport->smart_options->negotiation_include_tips,
					     "--negotiation-include");
		അല്ലെങ്കിൽ
			warning(_("ignoring %s because the protocol does not support it"),
				"--negotiation-include");
	} അല്ലെങ്കിൽ എങ്കിൽ (remote->negotiation_include.nr) {
		എങ്കിൽ (transport->smart_options) {
			add_negotiation_tips(&remote->negotiation_include,
					     &transport->smart_options->negotiation_include_tips,
					     "--negotiation-include");
		} അല്ലെങ്കിൽ {
			ഘടന strbuf config_name = STRBUF_INIT;
			strbuf_addf(&config_name, "remote.%s.negotiationInclude", remote->name);
			warning(_("ignoring %s because the protocol does not support it"),
				config_name.buf);
			strbuf_release(&config_name);
		}
	}
	തിരികെ transport;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ backfill_tags(ഘടന display_state *display_state,
			 ഘടന transport *transport,
			 ഘടന ref_transaction *transaction,
			 ഘടന ref *ref_map,
			 ഘടന fetch_head *fetch_head,
			 സ്ഥിരം ഘടന fetch_config *config,
			 ഘടന ref_update_display_info_array *display_array,
			 ഘടന list_objects_filter_options *filter_options)
{
	പൂർണ്ണസംഖ്യ retcode, cannot_reuse;

	/*
	 * Once we have set TRANS_OPT_DEEPEN_SINCE, we can't unset it
	 * when remote helper is used (setting it to an empty string
	 * is not unsetting). We could extend the remote helper
	 * protocol for that, but for now, just force a new connection
	 * without deepen-since. Similar story for deepen-not.
	 */
	cannot_reuse = transport->cannot_reuse ||
		deepen_since || deepen_not.nr;
	എങ്കിൽ (cannot_reuse) {
		gsecondary = prepare_transport(transport->remote, 0, filter_options);
		transport = gsecondary;
	}

	transport_set_option(transport, TRANS_OPT_FOLLOWTAGS, ശൂന്യ);
	transport_set_option(transport, TRANS_OPT_DEPTH, "0");
	transport_set_option(transport, TRANS_OPT_DEEPEN_RELATIVE, ശൂന്യ);
	retcode = fetch_and_consume_refs(display_state, transport, transaction, ref_map,
					 fetch_head, config, display_array);

	എങ്കിൽ (gsecondary) {
		transport_disconnect(gsecondary);
		gsecondary = ശൂന്യ;
	}

	തിരികെ retcode;
}

സ്ഥിതം സ്ഥിരം അക്ഷരം *strip_refshead(സ്ഥിരം അക്ഷരം *name){
	skip_prefix(name, "refs/heads/", &name);
	തിരികെ name;
}

സ്ഥിതം ശൂന്യം set_head_advice_സന്ദേശം(സ്ഥിരം അക്ഷരം *remote, സ്ഥിരം അക്ഷരം *head_name)
{
	സ്ഥിരം അക്ഷരം message_advice_set_head[] =
	N_("Run 'git remote set-head %s %s' to follow the change, or modify\n"
	   "either of the 'remote.%s.followRemoteHEAD' or 'fetch.followRemoteHEAD'\n"
	   "configuration variables to handle the situation differently.\n\n"

	   "Using this specific setting\n\n"
	   "    git config set remote.%s.followRemoteHEAD warn-if-not-%s\n\n"
	   "will suppress the warning until the remote changes HEAD to something else.");

	advise_if_enabled(ADVICE_FETCH_SET_HEAD_WARN, _(message_advice_set_head),
			remote, head_name, remote, remote, head_name);
}

സ്ഥിതം ശൂന്യം warn_set_head(സ്ഥിരം അക്ഷരം *remote, സ്ഥിരം അക്ഷരം *head_name,
			ഘടന strbuf *buf_prev, പൂർണ്ണസംഖ്യ updateres) {
	ഘടന strbuf buf_prefix = STRBUF_INIT;
	സ്ഥിരം അക്ഷരം *prev_head = ശൂന്യ;

	strbuf_addf(&buf_prefix, "refs/remotes/%s/", remote);
	skip_prefix(buf_prev->buf, buf_prefix.buf, &prev_head);

	എങ്കിൽ (prev_head && strcmp(prev_head, head_name)) {
		printf("'HEAD' at '%s' is '%s', but we have '%s' locally.\n",
			remote, head_name, prev_head);
		set_head_advice_സന്ദേശം(remote, head_name);
	}
	അല്ലെങ്കിൽ എങ്കിൽ (updateres && buf_prev->len) {
		printf("'HEAD' at '%s' is '%s', "
			"but we have a detached HEAD pointing to '%s' locally.\n",
			remote, head_name, buf_prev->buf);
		set_head_advice_സന്ദേശം(remote, head_name);
	}
	strbuf_release(&buf_prefix);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ set_head(സ്ഥിരം ഘടന ref *remote_refs, ഘടന remote *remote,
			പൂർണ്ണസംഖ്യ follow_remote_head)
{
	പൂർണ്ണസംഖ്യ result = 0, create_only, baremirror, was_detached;
	ഘടന strbuf b_head = STRBUF_INIT, b_remote_head = STRBUF_INIT,
		      b_local_head = STRBUF_INIT;
	സ്ഥിരം അക്ഷരം *no_warn_branch = remote->no_warn_branch;
	അക്ഷരം *head_name = ശൂന്യ;
	ഘടന ref *ref, *matches;
	ഘടന ref *fetch_map = ശൂന്യ, **fetch_map_tail = &fetch_map;
	ഘടന refspec_item refspec = {
		.force = 0,
		.pattern = 1,
		.src = (അക്ഷരം *) "refs/heads/*",
		.dst = (അക്ഷരം *) "refs/heads/*",
	};
	ഘടന string_list heads = STRING_LIST_INIT_DUP;
	ഘടന ref_store *refs = get_main_ref_store(the_repository);

	get_fetch_map(remote_refs, &refspec, &fetch_map_tail, 0);
	matches = guess_remote_head(find_ref_by_name(remote_refs, "HEAD"),
				    fetch_map, REMOTE_GUESS_HEAD_ALL);
	വേണ്ടി (ref = matches; ref; ref = ref->next) {
		string_list_append(&heads, strip_refshead(ref->name));
	}

	എങ്കിൽ (!heads.nr)
		result = 1;
	അല്ലെങ്കിൽ എങ്കിൽ (heads.nr > 1)
		result = 1;
	അല്ലെങ്കിൽ
		head_name = xstrdup(heads.ഇനങ്ങൾ[0].string);

	എങ്കിൽ (!head_name)
		പോകുക cleanup;
	baremirror = is_bare_repository(the_repository) && remote->mirror;
	create_only = follow_remote_head == FOLLOW_REMOTE_ALWAYS ? 0 : !baremirror;
	എങ്കിൽ (baremirror) {
		strbuf_addstr(&b_head, "HEAD");
		strbuf_addf(&b_remote_head, "refs/heads/%s", head_name);
	} അല്ലെങ്കിൽ {
		strbuf_addf(&b_head, "refs/remotes/%s/HEAD", remote->name);
		strbuf_addf(&b_remote_head, "refs/remotes/%s/%s", remote->name, head_name);
	}
	/* make sure it's valid */
	എങ്കിൽ (!baremirror && !refs_ref_exists(refs, b_remote_head.buf)) {
		result = 1;
		പോകുക cleanup;
	}
	was_detached = refs_update_symref_extended(refs, b_head.buf, b_remote_head.buf,
					"fetch", &b_local_head, create_only);
	എങ്കിൽ (was_detached == -1) {
		result = 1;
		പോകുക cleanup;
	}
	എങ്കിൽ (verbosity >= 0 &&
		follow_remote_head == FOLLOW_REMOTE_WARN &&
		(!no_warn_branch || strcmp(no_warn_branch, head_name)))
		warn_set_head(remote->name, head_name, &b_local_head, was_detached);

cleanup:
	സ്വതന്ത്രം(head_name);
	free_refs(fetch_map);
	free_refs(matches);
	string_list_clear(&heads, 0);
	strbuf_release(&b_head);
	strbuf_release(&b_local_head);
	strbuf_release(&b_remote_head);
	തിരികെ result;
}

ഘടന ref_rejection_data {
	പൂർണ്ണസംഖ്യ *retcode;
	bool conflict_സന്ദേശം_shown;
	bool case_sensitive_സന്ദേശം_shown;
	സ്ഥിരം അക്ഷരം *remote_name;
	ഘടന strmap *rejected_refs;
};

സ്ഥിതം ശൂന്യം ref_transaction_rejection_handler(സ്ഥിരം അക്ഷരം *refname,
					      സ്ഥിരം ഘടന object_id *old_oid UNUSED,
					      സ്ഥിരം ഘടന object_id *new_oid UNUSED,
					      സ്ഥിരം അക്ഷരം *old_target UNUSED,
					      സ്ഥിരം അക്ഷരം *new_target UNUSED,
					      എണ്ണൽ ref_transaction_error err,
					      സ്ഥിരം അക്ഷരം *details,
					      ശൂന്യം *cb_data)
{
	ഘടന ref_rejection_data *data = cb_data;

	എങ്കിൽ (err == REF_TRANSACTION_ERROR_CASE_CONFLICT && repo_ignore_case(the_repository) &&
	    !data->case_sensitive_സന്ദേശം_shown) {
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
		data->case_sensitive_സന്ദേശം_shown = true;
	} അല്ലെങ്കിൽ എങ്കിൽ (err == REF_TRANSACTION_ERROR_NAME_CONFLICT &&
		   !data->conflict_സന്ദേശം_shown) {
		error(_("some local refs could not be updated; try running\n"
			" 'git remote prune %s' to remove any old, conflicting "
			"branches"), data->remote_name);
		data->conflict_സന്ദേശം_shown = true;
	} അല്ലെങ്കിൽ {
		എങ്കിൽ (details)
			error("%s", details);
		അല്ലെങ്കിൽ
			error(_("fetching ref %s failed: %s"),
			      refname, ref_transaction_error_സന്ദേശം(err));
	}

	strmap_put(data->rejected_refs, refname, ശൂന്യ);
	*data->retcode = 1;
}

/*
 * Commit the reference transaction. If it isn't an atomic transaction, handle
 * rejected updates as part of using batched updates.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ commit_ref_transaction(ഘടന ref_transaction **transaction,
				  bool is_atomic, സ്ഥിരം അക്ഷരം *remote_name,
				  ഘടന strmap *rejected_refs,
				  ഘടന strbuf *err)
{
	പൂർണ്ണസംഖ്യ retcode = ref_transaction_commit(*transaction, err);
	എങ്കിൽ (retcode)
		പോകുക out;

	എങ്കിൽ (!is_atomic) {
		ഘടന ref_rejection_data data = {
			.conflict_സന്ദേശം_shown = 0,
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
	*transaction = ശൂന്യ;
	തിരികെ retcode;
}

സ്ഥിതം ശൂന്യം collect_updated_tips(ഘടന oidset *tips, ഘടന ref *ref_map)
{
	ഘടന ref *rm;
	വേണ്ടി (rm = ref_map; rm; rm = rm->next) {
		ഘടന commit *commit;
		/*
		 * Shallow-rejected refs are not stored and their history
		 * is incomplete, so skip them.
		 */
		എങ്കിൽ (rm->status == REF_STATUS_REJECT_SHALLOW)
			തുടരുക;
		എങ്കിൽ (is_null_oid(&rm->old_oid))
			തുടരുക;
		എങ്കിൽ (rm->peer_ref &&
		    oideq(&rm->old_oid, &rm->peer_ref->old_oid))
			തുടരുക;
		commit = lookup_commit_reference_gently(the_repository,
							&rm->old_oid, 1);
		എങ്കിൽ (commit)
			oidset_തിരുകുക(tips, &commit->object.oid);
	}
}

സ്ഥിതം പൂർണ്ണസംഖ്യ do_fetch(ഘടന transport *transport,
		    ഘടന refspec *rs,
		    സ്ഥിരം ഘടന fetch_config *config,
		    ഘടന list_objects_filter_options *filter_options,
		    ഘടന oidset *updated_tips)
{
	ഘടന ref_transaction *transaction = ശൂന്യ;
	ഘടന ref *ref_map = ശൂന്യ;
	ഘടന display_state display_state = { 0 };
	പൂർണ്ണസംഖ്യ autotags = (transport->remote->fetch_tags == 1);
	പൂർണ്ണസംഖ്യ retcode = 0;
	സ്ഥിരം ഘടന ref *remote_refs;
	ഘടന transport_ls_refs_options transport_ls_refs_options =
		TRANSPORT_LS_REFS_OPTIONS_INIT;
	ഘടന fetch_head fetch_head = { 0 };
	ഘടന strbuf err = STRBUF_INIT;
	ഘടന ref_update_display_info_array display_array = { 0 };
	ഘടന strmap rejected_refs = STRMAP_INIT;
	പൂർണ്ണസംഖ്യ summary_width = 0;
	പൂർണ്ണസംഖ്യ follow_remote_head = FOLLOW_REMOTE_NEVER;

	എങ്കിൽ (tags == TAGS_DEFAULT) {
		എങ്കിൽ (transport->remote->fetch_tags == 2)
			tags = TAGS_SET;
		എങ്കിൽ (transport->remote->fetch_tags == -1)
			tags = TAGS_UNSET;
	}

	/* if not appending, truncate FETCH_HEAD */
	എങ്കിൽ (!append && write_fetch_head) {
		retcode = truncate_fetch_head();
		എങ്കിൽ (retcode)
			പോകുക cleanup;
	}

	എങ്കിൽ (rs->nr) {
		refspec_ref_prefixes(rs, &transport_ls_refs_options.ref_prefixes);
	} അല്ലെങ്കിൽ {
		ഘടന branch *branch = branch_get(ശൂന്യ);

		എങ്കിൽ (transport->remote->fetch.nr) {
			refspec_ref_prefixes(&transport->remote->fetch,
					     &transport_ls_refs_options.ref_prefixes);
			/*
			 * See remote.c's handling of remote.<name>.followRemoteHEAD
			 * for the analogous, still-unresolved case.
			 */
			എങ്കിൽ (transport->remote->follow_remote_head)
				follow_remote_head = transport->remote->follow_remote_head;
			അല്ലെങ്കിൽ എങ്കിൽ (config->follow_remote_head_seen)
				follow_remote_head = get_follow_remote_head(config->follow_remote_head_raw);
			അല്ലെങ്കിൽ
				follow_remote_head = BUILTIN_FOLLOW_REMOTE_HEAD_DFLT;
		}
		എങ്കിൽ (branch && branch_has_merge_config(branch) &&
		    !strcmp(branch->remote_name, transport->remote->name)) {
			പൂർണ്ണസംഖ്യ i;
			വേണ്ടി (i = 0; i < branch->merge_nr; i++) {
				strvec_തള്ളുക(&transport_ls_refs_options.ref_prefixes,
					    branch->merge[i]->src);
			}
		}

		/*
		 * If there are no refs specified to fetch, then we just
		 * fetch HEAD; mention that to narrow the advertisement.
		 */
		എങ്കിൽ (!transport_ls_refs_options.ref_prefixes.nr)
			strvec_തള്ളുക(&transport_ls_refs_options.ref_prefixes,
				    "HEAD");
	}

	എങ്കിൽ (tags == TAGS_SET || tags == TAGS_DEFAULT)
		strvec_തള്ളുക(&transport_ls_refs_options.ref_prefixes,
			    "refs/tags/");

	എങ്കിൽ (follow_remote_head != FOLLOW_REMOTE_NEVER)
		strvec_തള്ളുക(&transport_ls_refs_options.ref_prefixes,
			    "HEAD");

	/*
	 * Only initiate ref listing if we have at least one ref we want to
	 * know about.
	 */
	എങ്കിൽ (transport_ls_refs_options.ref_prefixes.nr) {
		trace2_region_enter("fetch", "remote_refs", the_repository);
		remote_refs = transport_get_remote_refs(transport,
							&transport_ls_refs_options);
		trace2_region_leave("fetch", "remote_refs", the_repository);
	} അല്ലെങ്കിൽ
		remote_refs = ശൂന്യ;

	transport_ls_refs_options_release(&transport_ls_refs_options);

	ref_map = get_ref_map(transport->remote, remote_refs, rs,
			      tags, &autotags);
	എങ്കിൽ (!update_head_ok)
		check_not_current_branch(ref_map);

	retcode = open_fetch_head(&fetch_head);
	എങ്കിൽ (retcode)
		പോകുക cleanup;

	display_state_init(&display_state, ref_map, transport->url,
			   config->display_format);

	എങ്കിൽ (atomic_fetch) {
		transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
							  0, &err);
		എങ്കിൽ (!transaction) {
			retcode = -1;
			പോകുക cleanup;
		}
	}

	എങ്കിൽ (tags == TAGS_DEFAULT && autotags)
		transport_set_option(transport, TRANS_OPT_FOLLOWTAGS, "1");
	എങ്കിൽ (prune) {
		/*
		 * We only prune based on refspecs specified
		 * explicitly (via command line or configuration); we
		 * don't care whether --tags was specified.
		 */
		എങ്കിൽ (rs->nr) {
			retcode = prune_refs(&display_state, rs, transaction, ref_map);
		} അല്ലെങ്കിൽ {
			retcode = prune_refs(&display_state, &transport->remote->fetch,
					     transaction, ref_map);
		}
		എങ്കിൽ (retcode != 0)
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
	എങ്കിൽ (!transaction) {
		transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
							  REF_TRANSACTION_ALLOW_FAILURE, &err);
		എങ്കിൽ (!transaction) {
			retcode = -1;
			പോകുക cleanup;
		}
	}

	എങ്കിൽ (fetch_and_consume_refs(&display_state, transport, transaction, ref_map,
				   &fetch_head, config, &display_array)) {
		retcode = 1;
		പോകുക cleanup;
	}

	/*
	 * If neither --no-tags nor --tags was specified, do automated tag
	 * following.
	 */
	എങ്കിൽ (tags == TAGS_DEFAULT && autotags) {
		ഘടന ref *tags_ref_map = ശൂന്യ, **tail = &tags_ref_map;

		find_non_local_tags(remote_refs, transaction, &tags_ref_map, &tail);
		എങ്കിൽ (tags_ref_map) {
			/*
			 * If backfilling of tags fails then we want to tell
			 * the user so, but we have to continue regardless to
			 * populate upstream information of the references we
			 * have already fetched above. The exception though is
			 * when `--atomic` is passed: in that case we'll abort
			 * the transaction and don't commit anything.
			 */
			എങ്കിൽ (backfill_tags(&display_state, transport, transaction, tags_ref_map,
					  &fetch_head, config, &display_array, filter_options))
				retcode = 1;
		}

		free_refs(tags_ref_map);
	}

	എങ്കിൽ (retcode)
		പോകുക cleanup;

	എങ്കിൽ (verbosity >= 0)
		summary_width = transport_summary_width(ref_map);

	retcode = commit_ref_transaction(&transaction, atomic_fetch,
					 transport->remote->name,
					 &rejected_refs, &err);
	/*
	 * With '--atomic', bail out if the transaction fails. Without '--atomic',
	 * continue to fetch head and perform other post-fetch operations.
	 */
	എങ്കിൽ (retcode && atomic_fetch)
		പോകുക cleanup;

	commit_fetch_head(&fetch_head);

	collect_updated_tips(updated_tips, ref_map);

	എങ്കിൽ (set_upstream) {
		ഘടന branch *branch = branch_get("HEAD");
		ഘടന ref *rm;
		ഘടന ref *source_ref = ശൂന്യ;

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
		വേണ്ടി (rm = ref_map; rm; rm = rm->next) {
			എങ്കിൽ (!rm->peer_ref) {
				എങ്കിൽ (source_ref) {
					warning(_("multiple branches detected, incompatible with --set-upstream"));
					പോകുക cleanup;
				} അല്ലെങ്കിൽ {
					source_ref = rm;
				}
			}
		}
		എങ്കിൽ (source_ref) {
			എങ്കിൽ (!branch) {
				സ്ഥിരം അക്ഷരം *shortname = source_ref->name;
				skip_prefix(shortname, "refs/heads/", &shortname);

				warning(_("could not set upstream of HEAD to '%s' from '%s' when "
					  "it does not point to any branch."),
					shortname, transport->remote->name);
				പോകുക cleanup;
			}

			എങ്കിൽ (!strcmp(source_ref->name, "HEAD") ||
			    starts_with(source_ref->name, "refs/heads/"))
				install_branch_config(0,
						      branch->name,
						      transport->remote->name,
						      source_ref->name);
			അല്ലെങ്കിൽ എങ്കിൽ (starts_with(source_ref->name, "refs/remotes/"))
				warning(_("not setting upstream for a remote remote-tracking branch"));
			അല്ലെങ്കിൽ എങ്കിൽ (starts_with(source_ref->name, "refs/tags/"))
				warning(_("not setting upstream for a remote tag"));
			അല്ലെങ്കിൽ
				warning(_("unknown branch type"));
		} അല്ലെങ്കിൽ {
			warning(_("no source branch found;\n"
				  "you need to specify exactly one branch with the --set-upstream option"));
		}
	}
	എങ്കിൽ (follow_remote_head != FOLLOW_REMOTE_NEVER) {
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
	എങ്കിൽ (retcode && !atomic_fetch && transaction)
		commit_ref_transaction(&transaction, false,
				       transport->remote->name,
				       &rejected_refs, &err);

	വേണ്ടി (size_t i = 0; i < display_array.nr; i++) {
		ഘടന ref_update_display_info *info = &display_array.info[i];

		എങ്കിൽ (!info->failed && strmap_contains(&rejected_refs, info->ref))
			ref_update_display_info_set_failed(info);
		ref_update_display_info_display(info, &display_state, summary_width);
		ref_update_display_info_free(info);
	}

	എങ്കിൽ (retcode) {
		എങ്കിൽ (err.len) {
			error("%s", err.buf);
			strbuf_reset(&err);
		}
		എങ്കിൽ (transaction && ref_transaction_abort(transaction, &err) &&
		    err.len)
			error("%s", err.buf);
		transaction = ശൂന്യ;
	}

	എങ്കിൽ (transaction)
		ref_transaction_free(transaction);

	സ്വതന്ത്രം(display_array.info);
	strmap_clear(&rejected_refs, 0);
	display_state_release(&display_state);
	close_fetch_head(&fetch_head);
	strbuf_release(&err);
	free_refs(ref_map);
	തിരികെ retcode;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ get_one_remote_for_fetch(ഘടന remote *remote, ശൂന്യം *priv)
{
	ഘടന string_list *list = priv;
	എങ്കിൽ (!remote->skip_default_update)
		string_list_append(list, remote->name);
	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം add_options_to_argv(ഘടന strvec *argv,
				സ്ഥിരം ഘടന fetch_config *config)
{
	എങ്കിൽ (dry_run)
		strvec_തള്ളുക(argv, "--dry-run");
	എങ്കിൽ (prune != -1)
		strvec_തള്ളുക(argv, prune ? "--prune" : "--no-prune");
	എങ്കിൽ (prune_tags != -1)
		strvec_തള്ളുക(argv, prune_tags ? "--prune-tags" : "--no-prune-tags");
	എങ്കിൽ (update_head_ok)
		strvec_തള്ളുക(argv, "--update-head-ok");
	എങ്കിൽ (force)
		strvec_തള്ളുക(argv, "--force");
	എങ്കിൽ (keep)
		strvec_തള്ളുക(argv, "--keep");
	എങ്കിൽ (config->recurse_submodules == RECURSE_SUBMODULES_ON)
		strvec_തള്ളുക(argv, "--recurse-submodules");
	അല്ലെങ്കിൽ എങ്കിൽ (config->recurse_submodules == RECURSE_SUBMODULES_OFF)
		strvec_തള്ളുക(argv, "--no-recurse-submodules");
	അല്ലെങ്കിൽ എങ്കിൽ (config->recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND)
		strvec_തള്ളുക(argv, "--recurse-submodules=on-demand");
	എങ്കിൽ (tags == TAGS_SET)
		strvec_തള്ളുക(argv, "--tags");
	അല്ലെങ്കിൽ എങ്കിൽ (tags == TAGS_UNSET)
		strvec_തള്ളുക(argv, "--no-tags");
	എങ്കിൽ (verbosity >= 2)
		strvec_തള്ളുക(argv, "-v");
	എങ്കിൽ (verbosity >= 1)
		strvec_തള്ളുക(argv, "-v");
	അല്ലെങ്കിൽ എങ്കിൽ (verbosity < 0)
		strvec_തള്ളുക(argv, "-q");
	എങ്കിൽ (family == TRANSPORT_FAMILY_IPV4)
		strvec_തള്ളുക(argv, "--ipv4");
	അല്ലെങ്കിൽ എങ്കിൽ (family == TRANSPORT_FAMILY_IPV6)
		strvec_തള്ളുക(argv, "--ipv6");
	എങ്കിൽ (!write_fetch_head)
		strvec_തള്ളുക(argv, "--no-write-fetch-head");
	എങ്കിൽ (config->display_format == DISPLAY_FORMAT_PORCELAIN)
		strvec_pushf(argv, "--porcelain");
}

/* Fetch multiple remotes in parallel */

ഘടന parallel_fetch_state {
	സ്ഥിരം അക്ഷരം **argv;
	ഘടന string_list *remotes;
	പൂർണ്ണസംഖ്യ next, result;
	സ്ഥിരം ഘടന fetch_config *config;
};

സ്ഥിതം പൂർണ്ണസംഖ്യ fetch_next_remote(ഘടന child_process *cp,
			     ഘടന strbuf *out UNUSED,
			     ശൂന്യം *cb, ശൂന്യം **task_cb)
{
	ഘടന parallel_fetch_state *state = cb;
	അക്ഷരം *remote;

	എങ്കിൽ (state->next < 0 || state->next >= state->remotes->nr)
		തിരികെ 0;

	remote = state->remotes->ഇനങ്ങൾ[state->next++].string;
	*task_cb = remote;

	strvec_pushv(&cp->args, state->argv);
	strvec_തള്ളുക(&cp->args, remote);
	cp->git_cmd = 1;

	എങ്കിൽ (verbosity >= 0 && state->config->display_format != DISPLAY_FORMAT_PORCELAIN)
		printf(_("Fetching %s\n"), remote);

	തിരികെ 1;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ fetch_failed_to_start(ഘടന strbuf *out UNUSED,
				 ശൂന്യം *cb, ശൂന്യം *task_cb)
{
	ഘടന parallel_fetch_state *state = cb;
	സ്ഥിരം അക്ഷരം *remote = task_cb;

	state->result = error(_("could not fetch %s"), remote);

	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ fetch_finished(പൂർണ്ണസംഖ്യ result, ഘടന strbuf *out,
			  ശൂന്യം *cb, ശൂന്യം *task_cb)
{
	ഘടന parallel_fetch_state *state = cb;
	സ്ഥിരം അക്ഷരം *remote = task_cb;

	എങ്കിൽ (result) {
		strbuf_addf(out, _("could not fetch '%s' (exit code: %d)\n"),
			    remote, result);
		state->result = -1;
	}

	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ fetch_multiple(ഘടന string_list *list, പൂർണ്ണസംഖ്യ max_children,
			  സ്ഥിരം ഘടന fetch_config *config)
{
	പൂർണ്ണസംഖ്യ i, result = 0;
	ഘടന strvec argv = STRVEC_INIT;

	എങ്കിൽ (!append && write_fetch_head) {
		പൂർണ്ണസംഖ്യ errcode = truncate_fetch_head();
		എങ്കിൽ (errcode)
			തിരികെ errcode;
	}

	/*
	 * Cancel out the fetch.bundleURI config when running subprocesses,
	 * to avoid fetching from the same bundle list multiple times.
	 */
	strvec_pushl(&argv, "-c", "fetch.bundleURI=",
		     "fetch", "--append", "--no-auto-gc",
		     "--no-write-commit-graph", ശൂന്യ);
	വേണ്ടി (i = 0; i < server_options.nr; i++)
		strvec_pushf(&argv, "--server-option=%s", server_options.ഇനങ്ങൾ[i].string);
	add_options_to_argv(&argv, config);

	എങ്കിൽ (max_children != 1 && list->nr != 1) {
		ഘടന parallel_fetch_state state = { argv.v, list, 0, 0, config };
		സ്ഥിരം ഘടന run_process_parallel_opts opts = {
			.tr2_category = "fetch",
			.tr2_label = "parallel/fetch",

			.processes = max_children,

			.get_next_task = &fetch_next_remote,
			.start_failure = &fetch_failed_to_start,
			.task_finished = &fetch_finished,
			.data = &state,
		};

		strvec_തള്ളുക(&argv, "--end-of-options");

		run_processes_parallel(&opts);
		result = state.result;
	} അല്ലെങ്കിൽ
		വേണ്ടി (i = 0; i < list->nr; i++) {
			സ്ഥിരം അക്ഷരം *name = list->ഇനങ്ങൾ[i].string;
			ഘടന child_process cmd = CHILD_PROCESS_INIT;

			strvec_pushv(&cmd.args, argv.v);
			strvec_തള്ളുക(&cmd.args, name);
			എങ്കിൽ (verbosity >= 0 && config->display_format != DISPLAY_FORMAT_PORCELAIN)
				printf(_("Fetching %s\n"), name);
			cmd.git_cmd = 1;
			എങ്കിൽ (run_command(&cmd)) {
				error(_("could not fetch %s"), name);
				result = 1;
			}
		}

	strvec_clear(&argv);
	തിരികെ !!result;
}

/*
 * Fetching from the promisor remote should use the given filter-spec
 * or inherit the default filter-spec from the config.
 */
സ്ഥിതം വരിയിൽ ശൂന്യം fetch_one_setup_partial(ഘടന remote *remote,
					   ഘടന list_objects_filter_options *filter_options)
{
	/*
	 * Explicit --no-filter argument overrides everything, regardless
	 * of any prior partial clones and fetches.
	 */
	എങ്കിൽ (filter_options->no_filter)
		തിരികെ;

	/*
	 * If no prior partial clone/fetch and the current fetch DID NOT
	 * request a partial-fetch, do a normal fetch.
	 */
	എങ്കിൽ (!repo_has_promisor_remote(the_repository) && !filter_options->choice)
		തിരികെ;

	/*
	 * If this is a partial-fetch request, we enable partial on
	 * this repo if not already enabled and remember the given
	 * filter-spec as the default for subsequent fetches to this
	 * remote if there is currently no default filter-spec.
	 */
	എങ്കിൽ (filter_options->choice) {
		partial_clone_register(remote->name, filter_options);
		തിരികെ;
	}

	/*
	 * Do a partial-fetch from the promisor remote using either the
	 * explicitly given filter-spec or inherit the filter-spec from
	 * the config.
	 */
	എങ്കിൽ (!filter_options->choice)
		partial_clone_get_default_filter_spec(filter_options, remote->name);
	തിരികെ;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ fetch_one(ഘടന remote *remote, പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv,
		     പൂർണ്ണസംഖ്യ prune_tags_ok, പൂർണ്ണസംഖ്യ use_stdin_refspecs,
		     സ്ഥിരം ഘടന fetch_config *config,
		     ഘടന list_objects_filter_options *filter_options,
		     ഘടന oidset *updated_tips)
{
	ഘടന refspec rs = REFSPEC_INIT_FETCH(the_hash_algo);
	പൂർണ്ണസംഖ്യ i;
	പൂർണ്ണസംഖ്യ exit_code;
	പൂർണ്ണസംഖ്യ maybe_prune_tags;
	പൂർണ്ണസംഖ്യ remote_via_config = remote_is_configured(remote, 0);

	എങ്കിൽ (!remote)
		die(_("no remote repository specified; please specify either a URL or a\n"
		      "remote name from which new revisions should be fetched"));

	gtransport = prepare_transport(remote, 1, filter_options);

	എങ്കിൽ (prune < 0) {
		/* no command line request */
		എങ്കിൽ (0 <= remote->prune)
			prune = remote->prune;
		അല്ലെങ്കിൽ എങ്കിൽ (0 <= config->prune)
			prune = config->prune;
		അല്ലെങ്കിൽ
			prune = PRUNE_BY_DEFAULT;
	}

	എങ്കിൽ (prune_tags < 0) {
		/* no command line request */
		എങ്കിൽ (0 <= remote->prune_tags)
			prune_tags = remote->prune_tags;
		അല്ലെങ്കിൽ എങ്കിൽ (0 <= config->prune_tags)
			prune_tags = config->prune_tags;
		അല്ലെങ്കിൽ
			prune_tags = PRUNE_TAGS_BY_DEFAULT;
	}

	maybe_prune_tags = prune_tags_ok && prune_tags;
	എങ്കിൽ (maybe_prune_tags && remote_via_config)
		refspec_append(&remote->fetch, TAG_REFSPEC);

	എങ്കിൽ (maybe_prune_tags && (argc || !remote_via_config))
		refspec_append(&rs, TAG_REFSPEC);

	വേണ്ടി (i = 0; i < argc; i++) {
		എങ്കിൽ (!strcmp(argv[i], "tag")) {
			i++;
			എങ്കിൽ (i >= argc)
				die(_("you need to specify a tag name"));

			refspec_appendf(&rs, "refs/tags/%s:refs/tags/%s",
					argv[i], argv[i]);
		} അല്ലെങ്കിൽ {
			refspec_append(&rs, argv[i]);
		}
	}

	എങ്കിൽ (use_stdin_refspecs) {
		ഘടന strbuf വരി = STRBUF_INIT;
		ആകുമ്പോൾ (strbuf_getline_lf(&വരി, stdin) != EOF)
			refspec_append(&rs, വരി.buf);
		strbuf_release(&വരി);
	}

	എങ്കിൽ (server_options.nr)
		gtransport->server_options = &server_options;

	sigchain_തള്ളുക_common(unlock_pack_on_signal);
	atexit(unlock_pack_atexit);
	sigchain_തള്ളുക(SIGPIPE, SIG_IGN);
	exit_code = do_fetch(gtransport, &rs, config, filter_options,
			     updated_tips);
	sigchain_എടുക്കുക(SIGPIPE);
	refspec_clear(&rs);
	transport_disconnect(gtransport);
	gtransport = ശൂന്യ;
	തിരികെ exit_code;
}

പൂർണ്ണസംഖ്യ cmd_fetch(പൂർണ്ണസംഖ്യ argc,
	      സ്ഥിരം അക്ഷരം **argv,
	      സ്ഥിരം അക്ഷരം *prefix,
	      ഘടന repository *repo UNUSED)
{
	ഘടന fetch_config config = {
		.display_format = DISPLAY_FORMAT_FULL,
		.follow_remote_head_raw = ശൂന്യ,
		.follow_remote_head_seen = 0,
		.prune = -1,
		.prune_tags = -1,
		.show_forced_updates = 1,
		.recurse_submodules = RECURSE_SUBMODULES_DEFAULT,
		.parallel = 1,
		.submodule_fetch_jobs = -1,
	};
	സ്ഥിരം അക്ഷരം *submodule_prefix = "";
	സ്ഥിരം അക്ഷരം *bundle_uri;
	ഘടന string_list list = STRING_LIST_INIT_DUP;
	ഘടന list_objects_filter_options filter_options = LIST_OBJECTS_FILTER_INIT;
	ഘടന remote *remote = ശൂന്യ;
	പൂർണ്ണസംഖ്യ all = -1, multiple = 0;
	പൂർണ്ണസംഖ്യ result = 0;
	പൂർണ്ണസംഖ്യ prune_tags_ok = 1;
	പൂർണ്ണസംഖ്യ enable_auto_gc = 1;
	പൂർണ്ണസംഖ്യ unshallow = 0;
	പൂർണ്ണസംഖ്യ max_jobs = -1;
	പൂർണ്ണസംഖ്യ recurse_submodules_cli = RECURSE_SUBMODULES_DEFAULT;
	പൂർണ്ണസംഖ്യ recurse_submodules_default = RECURSE_SUBMODULES_ON_DEMAND;
	പൂർണ്ണസംഖ്യ fetch_write_commit_graph = -1;
	പൂർണ്ണസംഖ്യ stdin_refspecs = 0;
	പൂർണ്ണസംഖ്യ negotiate_only = 0;
	പൂർണ്ണസംഖ്യ porcelain = 0;
	പൂർണ്ണസംഖ്യ i;
	എണ്ണൽ {
		GRAPH_WRITE_REACHABLE,
		GRAPH_WRITE_TIPS,
		GRAPH_WRITE_SKIP,
	} graph_write_mode = GRAPH_WRITE_REACHABLE;
	ഘടന oidset updated_tips = OIDSET_INIT;

	ഘടന option builtin_fetch_options[] = {
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
		OPT_SET_INT('n', ശൂന്യ, &tags,
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
			.മൂല്യം = &submodule_prefix,
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
	വേണ്ടി (i = 1; i < argc; i++) {
		/* This handles non-URLs gracefully */
		അക്ഷരം *anon = transport_anonymize_url(argv[i]);

		strbuf_addf(&default_rla, " %s", anon);
		സ്വതന്ത്രം(anon);
	}

	repo_config(the_repository, git_fetch_config, &config);
	എങ്കിൽ (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_full_index = 0;
	}

	argc = parse_options(argc, argv, prefix,
			     builtin_fetch_options, builtin_fetch_usage, 0);

	എങ്കിൽ (recurse_submodules_cli != RECURSE_SUBMODULES_DEFAULT)
		config.recurse_submodules = recurse_submodules_cli;

	എങ്കിൽ (negotiate_only) {
		തിരഞ്ഞെടുപ്പ് (recurse_submodules_cli) {
		അവസ്ഥ RECURSE_SUBMODULES_OFF:
		അവസ്ഥ RECURSE_SUBMODULES_DEFAULT:
			/*
			 * --negotiate-only should never recurse into
			 * submodules. Skip it by setting recurse_submodules to
			 * RECURSE_SUBMODULES_OFF.
			 */
			config.recurse_submodules = RECURSE_SUBMODULES_OFF;
			നിർത്തുക;

		സ്ഥിരസ്ഥിതി:
			die(_("options '%s' and '%s' cannot be used together"),
			    "--negotiate-only", "--recurse-submodules");
		}
	}

	എങ്കിൽ (config.recurse_submodules != RECURSE_SUBMODULES_OFF) {
		പൂർണ്ണസംഖ്യ *sfjc = config.submodule_fetch_jobs == -1
			    ? &config.submodule_fetch_jobs : ശൂന്യ;
		പൂർണ്ണസംഖ്യ *rs = config.recurse_submodules == RECURSE_SUBMODULES_DEFAULT
			  ? &config.recurse_submodules : ശൂന്യ;

		fetch_config_from_gitmodules(the_repository, sfjc, rs);
	}


	എങ്കിൽ (porcelain) {
		തിരഞ്ഞെടുപ്പ് (recurse_submodules_cli) {
		അവസ്ഥ RECURSE_SUBMODULES_OFF:
		അവസ്ഥ RECURSE_SUBMODULES_DEFAULT:
			/*
			 * Reference updates in submodules would be ambiguous
			 * in porcelain mode, so we reject this combination.
			 */
			config.recurse_submodules = RECURSE_SUBMODULES_OFF;
			നിർത്തുക;

		സ്ഥിരസ്ഥിതി:
			die(_("options '%s' and '%s' cannot be used together"),
			    "--porcelain", "--recurse-submodules");
		}

		config.display_format = DISPLAY_FORMAT_PORCELAIN;
	}

	എങ്കിൽ (deepen_relative) {
		എങ്കിൽ (deepen_relative < 0)
			die(_("negative depth in --deepen is not supported"));
		എങ്കിൽ (depth)
			die(_("options '%s' and '%s' cannot be used together"), "--deepen", "--depth");
		depth = xstrfmt("%d", deepen_relative);
	}
	എങ്കിൽ (unshallow) {
		എങ്കിൽ (depth)
			die(_("options '%s' and '%s' cannot be used together"), "--depth", "--unshallow");
		അല്ലെങ്കിൽ എങ്കിൽ (!is_repository_shallow(the_repository))
			die(_("--unshallow on a complete repository does not make sense"));
		അല്ലെങ്കിൽ
			depth = xstrfmt("%d", INFINITE_DEPTH);
	}

	/* no need to be strict, transport_set_option() will validate it again */
	എങ്കിൽ (depth && atoi(depth) < 1)
		die(_("depth %s is not a positive number"), depth);
	എങ്കിൽ (depth || deepen_since || deepen_not.nr)
		deepen = 1;

	/* FETCH_HEAD never gets updated in --dry-run mode */
	എങ്കിൽ (dry_run)
		write_fetch_head = 0;

	എങ്കിൽ (!max_jobs)
		max_jobs = online_cpus();

	എങ്കിൽ (!repo_config_get_string_tmp(the_repository, "fetch.bundleuri", &bundle_uri) &&
	    fetch_bundle_uri(the_repository, bundle_uri, ശൂന്യ))
		warning(_("failed to fetch bundles from '%s'"), bundle_uri);

	എങ്കിൽ (all < 0) {
		/*
		 * no --[no-]all given;
		 * only use config option if no remote was explicitly specified
		 */
		all = (!argc) ? config.all : 0;
	}

	എങ്കിൽ (all) {
		എങ്കിൽ (argc == 1)
			die(_("fetch --all does not take a repository argument"));
		അല്ലെങ്കിൽ എങ്കിൽ (argc > 1)
			die(_("fetch --all does not make sense with refspecs"));

		(ശൂന്യം) for_each_remote(get_one_remote_for_fetch, &list);

		/* do not do fetch_multiple() of one */
		എങ്കിൽ (list.nr == 1)
			remote = remote_get(list.ഇനങ്ങൾ[0].string);
	} അല്ലെങ്കിൽ എങ്കിൽ (argc == 0) {
		/* No arguments -- use default remote */
		remote = remote_get(ശൂന്യ);
	} അല്ലെങ്കിൽ എങ്കിൽ (multiple) {
		/* All arguments are assumed to be remotes or groups */
		വേണ്ടി (i = 0; i < argc; i++)
			എങ്കിൽ (!add_remote_or_group(argv[i], &list))
				die(_("no such remote or remote group: %s"),
				    argv[i]);
	} അല്ലെങ്കിൽ {
		/* Single remote or group */
		(ശൂന്യം) add_remote_or_group(argv[0], &list);
		എങ്കിൽ (list.nr > 1) {
			/* More than one remote */
			എങ്കിൽ (argc > 1)
				die(_("fetching a group and specifying refspecs does not make sense"));
		} അല്ലെങ്കിൽ {
			/* Zero or one remotes */
			remote = remote_get(argv[0]);
			prune_tags_ok = (argc == 1);
			argc--;
			argv++;
		}
	}
	string_list_remove_duplicates(&list, 0);

	എങ്കിൽ (negotiate_only) {
		ഘടന oidset acked_commits = OIDSET_INIT;
		ഘടന oidset_iter iter;
		സ്ഥിരം ഘടന object_id *oid;

		trace2_region_enter("fetch", "negotiate-only", the_repository);
		എങ്കിൽ (!remote)
			die(_("must supply remote when using --negotiate-only"));
		gtransport = prepare_transport(remote, 1, &filter_options);

		എങ്കിൽ (!gtransport->smart_options) {
			warning(_("protocol does not support --negotiate-only, exiting"));
			result = 1;
			trace2_region_leave("fetch", "negotiate-only", the_repository);
			പോകുക cleanup;
		}
		എങ്കിൽ (!gtransport->smart_options->negotiation_restrict_tips)
			die(_("%s needs one or more %s"), "--negotiate-only",
			    "--negotiation-restrict=*");

		gtransport->smart_options->acked_commits = &acked_commits;

		എങ്കിൽ (server_options.nr)
			gtransport->server_options = &server_options;
		result = transport_fetch_refs(gtransport, ശൂന്യ);
		gtransport->smart_options->acked_commits = ശൂന്യ;

		oidset_iter_init(&acked_commits, &iter);
		ആകുമ്പോൾ ((oid = oidset_iter_next(&iter)))
			printf("%s\n", oid_to_hex(oid));
		oidset_clear(&acked_commits);
		trace2_region_leave("fetch", "negotiate-only", the_repository);
	} അല്ലെങ്കിൽ എങ്കിൽ (remote) {
		എങ്കിൽ (filter_options.choice || repo_has_promisor_remote(the_repository)) {
			trace2_region_enter("fetch", "setup-partial", the_repository);
			fetch_one_setup_partial(remote, &filter_options);
			trace2_region_leave("fetch", "setup-partial", the_repository);
		}
		trace2_region_enter("fetch", "fetch-one", the_repository);
		result = fetch_one(remote, argc, argv, prune_tags_ok, stdin_refspecs,
				   &config, &filter_options, &updated_tips);
		എങ്കിൽ (prepare_commit_graph(the_repository)) {
			എങ്കിൽ (oidset_size(&updated_tips))
				graph_write_mode = GRAPH_WRITE_TIPS;
			അല്ലെങ്കിൽ
				graph_write_mode = GRAPH_WRITE_SKIP;
		}
		trace2_region_leave("fetch", "fetch-one", the_repository);
	} അല്ലെങ്കിൽ {
		പൂർണ്ണസംഖ്യ max_children = max_jobs;

		എങ്കിൽ (filter_options.choice)
			die(_("--filter can only be used with the remote "
			      "configured in extensions.partialclone"));

		എങ്കിൽ (atomic_fetch)
			die(_("--atomic can only be used when fetching "
			      "from one remote"));

		എങ്കിൽ (stdin_refspecs)
			die(_("--stdin can only be used when fetching "
			      "from one remote"));

		എങ്കിൽ (max_children < 0)
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
	എങ്കിൽ (!result && remote && (config.recurse_submodules != RECURSE_SUBMODULES_OFF)) {
		ഘടന strvec options = STRVEC_INIT;
		പൂർണ്ണസംഖ്യ max_children = max_jobs;

		എങ്കിൽ (max_children < 0)
			max_children = config.submodule_fetch_jobs;
		എങ്കിൽ (max_children < 0)
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
	എങ്കിൽ (negotiate_only)
		പോകുക cleanup;

	prepare_repo_settings(the_repository);
	എങ്കിൽ (fetch_write_commit_graph > 0 ||
	    (fetch_write_commit_graph < 0 &&
	     the_repository->settings.fetch_write_commit_graph)) {
		പൂർണ്ണസംഖ്യ commit_graph_flags = COMMIT_GRAPH_WRITE_SPLIT;

		എങ്കിൽ (progress)
			commit_graph_flags |= COMMIT_GRAPH_WRITE_PROGRESS;

		എങ്കിൽ (graph_write_mode != GRAPH_WRITE_SKIP) {
			trace2_region_enter("fetch", "write-commit-graph",
					    the_repository);
			എങ്കിൽ (graph_write_mode == GRAPH_WRITE_TIPS)
				write_commit_graph(
					the_repository,
					the_repository->objects->source->path,
					ശൂന്യ, &updated_tips,
					commit_graph_flags, ശൂന്യ);
			അല്ലെങ്കിൽ
				write_commit_graph_reachable(
					the_repository,
					the_repository->objects->source->path,
					commit_graph_flags, ശൂന്യ);
			trace2_region_leave("fetch", "write-commit-graph",
					    the_repository);
		}
	}

	എങ്കിൽ (enable_auto_gc) {
		എങ്കിൽ (refetch) {
			/*
			 * Hint auto-maintenance strongly to encourage repacking,
			 * but respect config settings disabling it.
			 */
			പൂർണ്ണസംഖ്യ opt_val;
			എങ്കിൽ (repo_config_get_int(the_repository, "gc.autopacklimit", &opt_val))
				opt_val = -1;
			എങ്കിൽ (opt_val != 0)
				git_config_തള്ളുക_parameter("gc.autoPackLimit=1");

			എങ്കിൽ (repo_config_get_int(the_repository, "maintenance.incremental-repack.auto", &opt_val))
				opt_val = -1;
			എങ്കിൽ (opt_val != 0)
				git_config_തള്ളുക_parameter("maintenance.incremental-repack.auto=-1");
		}
		run_auto_maintenance(the_repository, verbosity < 0);
	}

 cleanup:
	oidset_clear(&updated_tips);
	string_list_clear(&list, 0);
	list_objects_filter_release(&filter_options);
	സ്വതന്ത്രം(config.follow_remote_head_raw);
	തിരികെ result;
}
