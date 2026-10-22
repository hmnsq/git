/*
 * "git fetch"
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#अन्तर्भाव "builtin.h"
#अन्तर्भाव "advice.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "hex.h"
#अन्तर्भाव "refs.h"
#अन्तर्भाव "refspec.h"
#अन्तर्भाव "object-name.h"
#अन्तर्भाव "odb.h"
#अन्तर्भाव "odb/source.h"
#अन्तर्भाव "oidset.h"
#अन्तर्भाव "oid-array.h"
#अन्तर्भाव "commit.h"
#अन्तर्भाव "string-list.h"
#अन्तर्भाव "remote.h"
#अन्तर्भाव "transport.h"
#अन्तर्भाव "run-command.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "sigchain.h"
#अन्तर्भाव "submodule-config.h"
#अन्तर्भाव "submodule.h"
#अन्तर्भाव "connected.h"
#अन्तर्भाव "strvec.h"
#अन्तर्भाव "utf8.h"
#अन्तर्भाव "pager.h"
#अन्तर्भाव "path.h"
#अन्तर्भाव "pkt-line.h"
#अन्तर्भाव "list-objects-filter-options.h"
#अन्तर्भाव "commit-reach.h"
#अन्तर्भाव "branch.h"
#अन्तर्भाव "promisor-remote.h"
#अन्तर्भाव "commit-graph.h"
#अन्तर्भाव "shallow.h"
#अन्तर्भाव "trace.h"
#अन्तर्भाव "trace2.h"
#अन्तर्भाव "bundle-uri.h"

#define FORCED_UPDATES_DELAY_WARNING_IN_MS (10 * 1000)

स्थिर अचर वर्ण * अचर builtin_fetch_usage[] = {
	N_("git fetch [<options>] [<repository> [<refspec>...]]"),
	N_("git fetch [<options>] <group>"),
	N_("git fetch --multiple [<options>] [(<repository>|<group>)...]"),
	N_("git fetch --all [<options>]"),
	रिक्त
};

गणना {
	TAGS_UNSET = 0,
	TAGS_DEFAULT = 1,
	TAGS_SET = 2
};

गणना display_format {
	DISPLAY_FORMAT_FULL,
	DISPLAY_FORMAT_COMPACT,
	DISPLAY_FORMAT_PORCELAIN,
};

संरचना display_state {
	संरचना strbuf buf;

	पूर्णाङ्क refcol_width;
	गणना display_format format;

	वर्ण *url;
	पूर्णाङ्क url_len, shown_url;
};

स्थिर uint64_t forced_updates_ms = 0;
स्थिर पूर्णाङ्क prefetch = 0;
स्थिर पूर्णाङ्क prune = -1; /* unspecified */
#define PRUNE_BY_DEFAULT 0 /* do we prune by default? */

स्थिर पूर्णाङ्क prune_tags = -1; /* unspecified */
#define PRUNE_TAGS_BY_DEFAULT 0 /* do we prune tags by default? */

स्थिर पूर्णाङ्क append, dry_run, force, keep, update_head_ok;
स्थिर पूर्णाङ्क write_fetch_head = 1;
स्थिर पूर्णाङ्क verbosity, deepen_relative, set_upstream, refetch;
स्थिर पूर्णाङ्क progress = -1;
स्थिर पूर्णाङ्क tags = TAGS_DEFAULT, update_shallow, deepen;
स्थिर पूर्णाङ्क atomic_fetch;
स्थिर गणना transport_family family;
स्थिर अचर वर्ण *depth;
स्थिर अचर वर्ण *deepen_since;
स्थिर अचर वर्ण *upload_pack;
स्थिर संरचना string_list deepen_not = STRING_LIST_INIT_NODUP;
स्थिर संरचना strbuf default_rla = STRBUF_INIT;
स्थिर संरचना transport *gtransport;
स्थिर संरचना transport *gsecondary;
स्थिर संरचना refspec refmap;
स्थिर संरचना string_list server_options = STRING_LIST_INIT_DUP;
स्थिर संरचना string_list negotiation_restrict = STRING_LIST_INIT_NODUP;
स्थिर संरचना string_list negotiation_include = STRING_LIST_INIT_NODUP;

संरचना fetch_config {
	गणना display_format display_format;
	वर्ण *follow_remote_head_raw;
	पूर्णाङ्क follow_remote_head_seen;
	पूर्णाङ्क all;
	पूर्णाङ्क prune;
	पूर्णाङ्क prune_tags;
	पूर्णाङ्क show_forced_updates;
	पूर्णाङ्क recurse_submodules;
	पूर्णाङ्क parallel;
	पूर्णाङ्क submodule_fetch_jobs;
};

स्थिर पूर्णाङ्क git_fetch_config(अचर वर्ण *k, अचर वर्ण *v,
			    अचर संरचना config_context *ctx, शून्य *cb)
{
	संरचना fetch_config *fetch_config = cb;

	यदि (!म_भेद(k, "fetch.all")) {
		fetch_config->all = git_config_bool(k, v);
		फर्काऊ 0;
	}

	यदि (!म_भेद(k, "fetch.prune")) {
		fetch_config->prune = git_config_bool(k, v);
		फर्काऊ 0;
	}

	यदि (!म_भेद(k, "fetch.prunetags")) {
		fetch_config->prune_tags = git_config_bool(k, v);
		फर्काऊ 0;
	}

	यदि (!म_भेद(k, "fetch.showforcedupdates")) {
		fetch_config->show_forced_updates = git_config_bool(k, v);
		फर्काऊ 0;
	}

	यदि (!म_भेद(k, "submodule.recurse")) {
		पूर्णाङ्क r = git_config_bool(k, v) ?
			RECURSE_SUBMODULES_ON : RECURSE_SUBMODULES_OFF;
		fetch_config->recurse_submodules = r;
		फर्काऊ 0;
	}

	यदि (!म_भेद(k, "submodule.fetchjobs")) {
		fetch_config->submodule_fetch_jobs = parse_submodule_fetchjobs(k, v, ctx->kvi);
		फर्काऊ 0;
	} अन्यथा यदि (!म_भेद(k, "fetch.recursesubmodules")) {
		fetch_config->recurse_submodules = parse_fetch_recurse_submodules_arg(k, v);
		फर्काऊ 0;
	}

	यदि (!म_भेद(k, "fetch.parallel")) {
		fetch_config->parallel = git_config_int(k, v, ctx->kvi);
		यदि (fetch_config->parallel < 0)
			die(_("fetch.parallel cannot be negative"));
		यदि (!fetch_config->parallel)
			fetch_config->parallel = online_cpus();
		फर्काऊ 0;
	}

	यदि (!म_भेद(k, "fetch.output")) {
		यदि (!v)
			फर्काऊ config_error_nonbool(k);
		अन्यथा यदि (!strcasecmp(v, "full"))
			fetch_config->display_format = DISPLAY_FORMAT_FULL;
		अन्यथा यदि (!strcasecmp(v, "compact"))
			fetch_config->display_format = DISPLAY_FORMAT_COMPACT;
		अन्यथा
			die(_("invalid value for '%s': '%s'"),
			    "fetch.output", v);
		फर्काऊ 0;
	}

	यदि (!म_भेद(k, "fetch.followremotehead")) {
		मुक्त(fetch_config->follow_remote_head_raw);
		fetch_config->follow_remote_head_raw = xstrdup_or_null(v);
		fetch_config->follow_remote_head_seen = 1;
		फर्काऊ 0;
	}

	फर्काऊ git_default_config(k, v, ctx, cb);
}

स्थिर गणना follow_remote_head_settings get_follow_remote_head(अचर वर्ण *setting)
{
	यदि (!setting)
		die(_("missing value for 'fetch.followRemoteHEAD'"));
	अन्यथा यदि (!म_भेद(setting, "never"))
		फर्काऊ FOLLOW_REMOTE_NEVER;
	अन्यथा यदि (!म_भेद(setting, "create"))
		फर्काऊ FOLLOW_REMOTE_CREATE;
	अन्यथा यदि (!म_भेद(setting, "warn"))
		फर्काऊ FOLLOW_REMOTE_WARN;
	अन्यथा यदि (!म_भेद(setting, "always"))
		फर्काऊ FOLLOW_REMOTE_ALWAYS;
	warning(_("unrecognized fetch.followRemoteHEAD value '%s' ignored"), setting);
	फर्काऊ BUILTIN_FOLLOW_REMOTE_HEAD_DFLT;
}

स्थिर पूर्णाङ्क parse_refmap_arg(अचर संरचना option *opt, अचर वर्ण *arg, पूर्णाङ्क unset)
{
	BUG_ON_OPT_NEG(unset);

	/*
	 * "git fetch --refmap='' origin foo"
	 * can be used to tell the command not to store anywhere
	 */
	refspec_append(opt->मान, arg);

	फर्काऊ 0;
}

स्थिर शून्य unlock_pack(अचिन्हित पूर्णाङ्क flags)
{
	यदि (gtransport)
		transport_unlock_pack(gtransport, flags);
	यदि (gsecondary)
		transport_unlock_pack(gsecondary, flags);
}

स्थिर शून्य unlock_pack_atexit(शून्य)
{
	unlock_pack(0);
}

स्थिर शून्य unlock_pack_on_signal(पूर्णाङ्क signo)
{
	unlock_pack(TRANSPORT_UNLOCK_PACK_IN_SIGNAL_HANDLER);
	sigchain_झिक(signo);
	उत्थापय(signo);
}

स्थिर शून्य add_merge_config(संरचना ref **head,
			   अचर संरचना ref *remote_refs,
		           संरचना branch *branch,
		           संरचना ref ***tail)
{
	पूर्णाङ्क i;

	लागि (i = 0; i < branch->merge_nr; i++) {
		संरचना ref *rm, **old_tail = *tail;
		संरचना refspec_item refspec;

		लागि (rm = *head; rm; rm = rm->next) {
			यदि (branch_merge_matches(branch, i, rm->name)) {
				rm->fetch_head_status = FETCH_HEAD_MERGE;
				रोक;
			}
		}
		यदि (rm)
			जारी;

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
		स_रखो(&refspec, 0, परिमाण(refspec));
		refspec.src = branch->merge[i]->src;
		get_fetch_map(remote_refs, &refspec, tail, 1);
		लागि (rm = *old_tail; rm; rm = rm->next)
			rm->fetch_head_status = FETCH_HEAD_MERGE;
	}
}

स्थिर शून्य create_fetch_oidset(संरचना ref **head, संरचना oidset *out)
{
	संरचना ref *rm = *head;
	जबसम्म (rm) {
		oidset_घुसाउ(out, &rm->old_oid);
		rm = rm->next;
	}
}

संरचना refname_hash_entry {
	संरचना hashmap_entry ent;
	संरचना object_id oid;
	पूर्णाङ्क ignore;
	वर्ण refname[FLEX_ARRAY];
};

स्थिर पूर्णाङ्क refname_hash_entry_cmp(अचर शून्य *hashmap_cmp_fn_data UNUSED,
				  अचर संरचना hashmap_entry *eptr,
				  अचर संरचना hashmap_entry *entry_or_साँचो,
				  अचर शून्य *keydata)
{
	अचर संरचना refname_hash_entry *e1, *e2;

	e1 = container_of(eptr, अचर संरचना refname_hash_entry, ent);
	e2 = container_of(entry_or_साँचो, अचर संरचना refname_hash_entry, ent);
	फर्काऊ म_भेद(e1->refname, keydata ? keydata : e2->refname);
}

स्थिर संरचना refname_hash_entry *refname_hash_add(संरचना hashmap *map,
						   अचर वर्ण *refname,
						   अचर संरचना object_id *oid)
{
	संरचना refname_hash_entry *ent;
	माप_प्रकार len = म_माप(refname);

	FLEX_ALLOC_MEM(ent, refname, refname, len);
	hashmap_entry_init(&ent->ent, strhash(refname));
	oidcpy(&ent->oid, oid);
	hashmap_add(map, &ent->ent);
	फर्काऊ ent;
}

स्थिर पूर्णाङ्क add_one_refname(अचर संरचना reference *ref, शून्य *cbdata)
{
	संरचना hashmap *refname_map = cbdata;

	(शून्य) refname_hash_add(refname_map, ref->name, ref->oid);
	फर्काऊ 0;
}

स्थिर शून्य refname_hash_init(संरचना hashmap *map)
{
	hashmap_init(map, refname_hash_entry_cmp, रिक्त, 0);
}

स्थिर पूर्णाङ्क refname_hash_exists(संरचना hashmap *map, अचर वर्ण *refname)
{
	फर्काऊ !!hashmap_get_from_hash(map, strhash(refname), refname);
}

स्थिर शून्य clear_item(संरचना refname_hash_entry *item)
{
	item->ignore = 1;
}


स्थिर शून्य add_already_queued_tags(अचर वर्ण *refname,
				    अचर संरचना object_id *old_oid UNUSED,
				    अचर संरचना object_id *new_oid,
				    शून्य *cb_data)
{
	संरचना hashmap *queued_tags = cb_data;
	यदि (starts_with(refname, "refs/tags/") && new_oid)
		(शून्य) refname_hash_add(queued_tags, refname, new_oid);
}

स्थिर शून्य find_non_local_tags(अचर संरचना ref *refs,
				संरचना ref_transaction *transaction,
				संरचना ref **head,
				संरचना ref ***tail)
{
	संरचना hashmap existing_refs;
	संरचना hashmap remote_refs;
	संरचना oidset fetch_oids = OIDSET_INIT;
	संरचना string_list remote_refs_list = STRING_LIST_INIT_NODUP;
	संरचना string_list_item *remote_ref_item;
	अचर संरचना ref *ref;
	संरचना refname_hash_entry *item = रिक्त;

	refname_hash_init(&existing_refs);
	refname_hash_init(&remote_refs);
	create_fetch_oidset(head, &fetch_oids);

	refs_for_each_ref(get_main_ref_store(the_repository), add_one_refname,
			  &existing_refs);

	/*
	 * If we already have a transaction, then we need to filter out all
	 * tags which have already been queued up.
	 */
	यदि (transaction)
		ref_transaction_for_each_queued_update(transaction,
						       add_already_queued_tags,
						       &existing_refs);

	लागि (ref = refs; ref; ref = ref->next) {
		यदि (!starts_with(ref->name, "refs/tags/"))
			जारी;

		/*
		 * The peeled ref always follows the matching base
		 * ref, so if we see a peeled ref that we don't want
		 * to fetch then we can mark the ref entry in the list
		 * as one to ignore by setting util to NULL.
		 */
		यदि (ends_with(ref->name, "^{}")) {
			यदि (item &&
			    !odb_has_object(the_repository->objects, &ref->old_oid, 0) &&
			    !oidset_contains(&fetch_oids, &ref->old_oid) &&
			    !odb_has_object(the_repository->objects, &item->oid, 0) &&
			    !oidset_contains(&fetch_oids, &item->oid))
				clear_item(item);
			item = रिक्त;
			जारी;
		}

		/*
		 * If item is non-NULL here, then we previously saw a
		 * ref not followed by a peeled reference, so we need
		 * to check if it is a lightweight tag that we want to
		 * fetch.
		 */
		यदि (item &&
		    !odb_has_object(the_repository->objects, &item->oid, 0) &&
		    !oidset_contains(&fetch_oids, &item->oid))
			clear_item(item);

		item = रिक्त;

		/* skip duplicates and refs that we already have */
		यदि (refname_hash_exists(&remote_refs, ref->name) ||
		    refname_hash_exists(&existing_refs, ref->name))
			जारी;

		item = refname_hash_add(&remote_refs, ref->name, &ref->old_oid);
		string_list_घुसाउ(&remote_refs_list, ref->name);
	}
	hashmap_clear_and_free(&existing_refs, संरचना refname_hash_entry, ent);

	/*
	 * We may have a final lightweight tag that needs to be
	 * checked to see if it needs fetching.
	 */
	यदि (item &&
	    !odb_has_object(the_repository->objects, &item->oid, 0) &&
	    !oidset_contains(&fetch_oids, &item->oid))
		clear_item(item);

	/*
	 * For all the tags in the remote_refs_list,
	 * add them to the list of refs to be fetched
	 */
	for_each_string_list_item(remote_ref_item, &remote_refs_list) {
		अचर वर्ण *refname = remote_ref_item->string;
		संरचना ref *rm;
		अचिन्हित पूर्णाङ्क hash = strhash(refname);

		item = hashmap_get_entry_from_hash(&remote_refs, hash, refname,
					संरचना refname_hash_entry, ent);
		यदि (!item)
			BUG("unseen remote ref?");

		/* Unless we have already decided to ignore this item... */
		यदि (item->ignore)
			जारी;

		rm = alloc_ref(item->refname);
		rm->peer_ref = alloc_ref(item->refname);
		oidcpy(&rm->old_oid, &item->oid);
		**tail = rm;
		*tail = &rm->next;
	}
	hashmap_clear_and_free(&remote_refs, संरचना refname_hash_entry, ent);
	string_list_clear(&remote_refs_list, 0);
	oidset_clear(&fetch_oids);
}

स्थिर शून्य filter_prefetch_refspec(संरचना refspec *rs)
{
	पूर्णाङ्क i;

	यदि (!prefetch)
		फर्काऊ;

	लागि (i = 0; i < rs->nr; i++) {
		संरचना strbuf new_dst = STRBUF_INIT;
		वर्ण *old_dst;
		अचर वर्ण *sub = रिक्त;

		यदि (rs->वस्तुहरू[i].negative)
			जारी;
		यदि (!rs->वस्तुहरू[i].dst ||
		    (rs->वस्तुहरू[i].src &&
		     starts_with(rs->वस्तुहरू[i].src,
				 ref_namespace[NAMESPACE_TAGS].ref))) {
			पूर्णाङ्क j;

			refspec_item_clear(&rs->वस्तुहरू[i]);

			लागि (j = i + 1; j < rs->nr; j++)
				rs->वस्तुहरू[j - 1] = rs->वस्तुहरू[j];
			rs->nr--;
			i--;
			जारी;
		}

		old_dst = rs->वस्तुहरू[i].dst;
		strbuf_addstr(&new_dst, ref_namespace[NAMESPACE_PREFETCH].ref);

		/*
		 * If old_dst starts with "refs/", then place
		 * sub after that prefix. Otherwise, start at
		 * the beginning of the string.
		 */
		यदि (!skip_prefix(old_dst, "refs/", &sub))
			sub = old_dst;
		strbuf_addstr(&new_dst, sub);

		rs->वस्तुहरू[i].dst = strbuf_detach(&new_dst, रिक्त);
		rs->वस्तुहरू[i].force = 1;

		मुक्त(old_dst);
	}
}

स्थिर संरचना ref *get_ref_map(संरचना remote *remote,
			       अचर संरचना ref *remote_refs,
			       संरचना refspec *rs,
			       पूर्णाङ्क tags, पूर्णाङ्क *autotags)
{
	पूर्णाङ्क i;
	संरचना ref *rm;
	संरचना ref *ref_map = रिक्त;
	संरचना ref **tail = &ref_map;

	/* opportunistically-updated references: */
	संरचना ref *orefs = रिक्त, **oref_tail = &orefs;

	संरचना hashmap existing_refs;
	पूर्णाङ्क existing_refs_populated = 0;

	filter_prefetch_refspec(rs);
	यदि (remote)
		filter_prefetch_refspec(&remote->fetch);

	यदि (rs->nr) {
		संरचना refspec *fetch_refspec;

		लागि (i = 0; i < rs->nr; i++) {
			get_fetch_map(remote_refs, &rs->वस्तुहरू[i], &tail, 0);
			यदि (rs->वस्तुहरू[i].dst && rs->वस्तुहरू[i].dst[0])
				*autotags = 1;
		}
		/* Merge everything on the command line (but not --tags) */
		लागि (rm = ref_map; rm; rm = rm->next)
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
		यदि (refmap.nr)
			fetch_refspec = &refmap;
		अन्यथा
			fetch_refspec = &remote->fetch;

		लागि (i = 0; i < fetch_refspec->nr; i++)
			get_fetch_map(ref_map, &fetch_refspec->वस्तुहरू[i], &oref_tail, 1);
	} अन्यथा यदि (refmap.nr) {
		die("--refmap option is only meaningful with command-line refspec(s)");
	} अन्यथा {
		/* Use the defaults */
		संरचना branch *branch = branch_get(रिक्त);
		पूर्णाङ्क has_merge = branch_has_merge_config(branch);
		यदि (remote &&
		    (remote->fetch.nr ||
		     /* Note: has_merge implies non-NULL branch->remote_name */
		     (has_merge && !म_भेद(branch->remote_name, remote->name)))) {
			लागि (i = 0; i < remote->fetch.nr; i++) {
				get_fetch_map(remote_refs, &remote->fetch.वस्तुहरू[i], &tail, 0);
				यदि (remote->fetch.वस्तुहरू[i].dst &&
				    remote->fetch.वस्तुहरू[i].dst[0])
					*autotags = 1;
				यदि (!i && !has_merge && ref_map &&
				    !remote->fetch.वस्तुहरू[0].pattern)
					ref_map->fetch_head_status = FETCH_HEAD_MERGE;
			}
			/*
			 * if the remote we're fetching from is the same
			 * as given in branch.<name>.remote, we add the
			 * ref given in branch.<name>.merge, too.
			 *
			 * Note: has_merge implies non-NULL branch->remote_name
			 */
			यदि (has_merge &&
			    !म_भेद(branch->remote_name, remote->name))
				add_merge_config(&ref_map, remote_refs, branch, &tail);
		} अन्यथा यदि (!prefetch) {
			ref_map = get_remote_ref(remote_refs, "HEAD");
			यदि (!ref_map)
				die(_("couldn't find remote ref HEAD"));
			ref_map->fetch_head_status = FETCH_HEAD_MERGE;
			tail = &ref_map->next;
		}
	}

	यदि (tags == TAGS_SET) {
		संरचना refspec_item tag_refspec;

		/* also fetch all tags */
		refspec_item_init_थप(&tag_refspec, TAG_REFSPEC,
				       the_hash_algo);
		get_fetch_map(remote_refs, &tag_refspec, &tail, 0);
		refspec_item_clear(&tag_refspec);
	} अन्यथा यदि (tags == TAGS_DEFAULT && *autotags) {
		find_non_local_tags(remote_refs, रिक्त, &ref_map, &tail);
	}

	/* Now append any refs to be updated opportunistically: */
	*tail = orefs;
	लागि (rm = orefs; rm; rm = rm->next) {
		rm->fetch_head_status = FETCH_HEAD_IGNORE;
		tail = &rm->next;
	}

	/*
	 * apply negative refspecs first, before we remove duplicates. This is
	 * necessary as negative refspecs might remove an otherwise conflicting
	 * duplicate.
	 */
	यदि (rs->nr)
		ref_map = apply_negative_refspecs(ref_map, rs);
	अन्यथा
		ref_map = apply_negative_refspecs(ref_map, &remote->fetch);

	ref_map = ref_remove_duplicates(ref_map);

	लागि (rm = ref_map; rm; rm = rm->next) {
		यदि (rm->peer_ref) {
			अचर वर्ण *refname = rm->peer_ref->name;
			संरचना refname_hash_entry *peer_item;
			अचिन्हित पूर्णाङ्क hash = strhash(refname);

			यदि (!existing_refs_populated) {
				refname_hash_init(&existing_refs);
				refs_for_each_ref(get_main_ref_store(the_repository),
						  add_one_refname,
						  &existing_refs);
				existing_refs_populated = 1;
			}

			peer_item = hashmap_get_entry_from_hash(&existing_refs,
						hash, refname,
						संरचना refname_hash_entry, ent);
			यदि (peer_item) {
				संरचना object_id *old_oid = &peer_item->oid;
				oidcpy(&rm->peer_ref->old_oid, old_oid);
			}
		}
	}
	यदि (existing_refs_populated)
		hashmap_clear_and_free(&existing_refs, संरचना refname_hash_entry, ent);

	फर्काऊ ref_map;
}

स्थिर पूर्णाङ्क s_update_ref(अचर वर्ण *action,
			संरचना ref *ref,
			संरचना ref_transaction *transaction,
			पूर्णाङ्क check_old)
{
	वर्ण *सन्देश;
	वर्ण *rla = दो_पर्या("GIT_REFLOG_ACTION");
	संरचना strbuf err = STRBUF_INIT;
	पूर्णाङ्क ret;

	यदि (dry_run)
		फर्काऊ 0;
	यदि (!rla)
		rla = default_rla.buf;
	सन्देश = xstrfmt("%s: %s", rla, action);

	ret = ref_transaction_update(transaction, ref->name, &ref->new_oid,
				     check_old ? &ref->old_oid : रिक्त,
				     रिक्त, रिक्त, 0, सन्देश, &err);

	यदि (ret)
		error("%s", err.buf);
	strbuf_release(&err);
	मुक्त(सन्देश);
	फर्काऊ ret;
}

स्थिर पूर्णाङ्क refcol_width(अचर संरचना ref *ref_map, पूर्णाङ्क compact_format)
{
	अचर संरचना ref *ref;
	पूर्णाङ्क max, width = 10;

	max = term_columns();
	यदि (compact_format)
		max = max * 2 / 3;

	लागि (ref = ref_map; ref; ref = ref->next) {
		पूर्णाङ्क rlen, llen = 0, len;

		यदि (ref->status == REF_STATUS_REJECT_SHALLOW ||
		    !ref->peer_ref ||
		    !म_भेद(ref->name, "HEAD"))
			जारी;

		/* uptodate lines are only shown on high verbosity level */
		यदि (verbosity <= 0 && oideq(&ref->peer_ref->old_oid, &ref->old_oid))
			जारी;

		rlen = utf8_strwidth(prettify_refname(ref->name));
		यदि (!compact_format)
			llen = utf8_strwidth(prettify_refname(ref->peer_ref->name));

		/*
		 * rough estimation to see if the output line is too long and
		 * should not be counted (we can't do precise calculation
		 * anyway because we don't know if the error explanation part
		 * will be printed in update_local_ref)
		 */
		len = 21 /* flag and summary */ + rlen + 4 /* -> */ + llen;
		यदि (len >= max)
			जारी;

		यदि (width < rlen)
			width = rlen;
	}

	फर्काऊ width;
}

स्थिर शून्य display_state_init(संरचना display_state *display_state, संरचना ref *ref_map,
			       अचर वर्ण *raw_url, गणना display_format format)
{
	पूर्णाङ्क i;

	स_रखो(display_state, 0, परिमाण(*display_state));
	strbuf_init(&display_state->buf, 0);
	display_state->format = format;

	यदि (raw_url)
		display_state->url = transport_anonymize_url(raw_url);
	अन्यथा
		display_state->url = xstrdup("foreign");

	display_state->url_len = म_माप(display_state->url);
	लागि (i = display_state->url_len - 1; 0 <= i && display_state->url[i] == '/'; i--)
		;
	display_state->url_len = i + 1;
	यदि (4 < i && !म_भेदन(".git", display_state->url + i - 3, 4))
		display_state->url_len = i - 3;

	यदि (verbosity < 0)
		फर्काऊ;

	विकल्प (display_state->format) {
	पक्ष DISPLAY_FORMAT_FULL:
	पक्ष DISPLAY_FORMAT_COMPACT:
		display_state->refcol_width = refcol_width(ref_map,
							   display_state->format == DISPLAY_FORMAT_COMPACT);
		रोक;
	पक्ष DISPLAY_FORMAT_PORCELAIN:
		/* We don't need to precompute anything here. */
		रोक;
	स्वभाव:
		BUG("unexpected display format %d", display_state->format);
	}
}

स्थिर शून्य display_state_release(संरचना display_state *display_state)
{
	strbuf_release(&display_state->buf);
	मुक्त(display_state->url);
}

स्थिर शून्य print_remote_to_local(संरचना display_state *display_state,
				  अचर वर्ण *remote, अचर वर्ण *local)
{
	strbuf_addf(&display_state->buf, "%-*s -> %s",
		    display_state->refcol_width, remote, local);
}

स्थिर पूर्णाङ्क find_and_replace(संरचना strbuf *haystack,
			    अचर वर्ण *needle,
			    अचर वर्ण *placeholder)
{
	अचर वर्ण *p = रिक्त;
	पूर्णाङ्क plen, nlen;

	nlen = म_माप(needle);
	यदि (ends_with(haystack->buf, needle))
		p = haystack->buf + haystack->len - nlen;
	अन्यथा
		p = म_माला(haystack->buf, needle);
	यदि (!p)
		फर्काऊ 0;

	यदि (p > haystack->buf && p[-1] != '/')
		फर्काऊ 0;

	plen = म_माप(p);
	यदि (plen > nlen && p[nlen] != '/')
		फर्काऊ 0;

	strbuf_splice(haystack, p - haystack->buf, nlen,
		      placeholder, म_माप(placeholder));
	फर्काऊ 1;
}

स्थिर शून्य print_compact(संरचना display_state *display_state,
			  अचर वर्ण *remote, अचर वर्ण *local)
{
	संरचना strbuf r = STRBUF_INIT;
	संरचना strbuf l = STRBUF_INIT;

	यदि (!म_भेद(remote, local)) {
		strbuf_addf(&display_state->buf, "%-*s -> *", display_state->refcol_width, remote);
		फर्काऊ;
	}

	strbuf_addstr(&r, remote);
	strbuf_addstr(&l, local);

	यदि (!find_and_replace(&r, local, "*"))
		find_and_replace(&l, remote, "*");
	print_remote_to_local(display_state, r.buf, l.buf);

	strbuf_release(&r);
	strbuf_release(&l);
}

स्थिर शून्य display_ref_update(संरचना display_state *display_state, वर्ण code,
			       अचर वर्ण *summary, अचर वर्ण *error,
			       अचर वर्ण *remote, अचर वर्ण *local,
			       अचर संरचना object_id *old_oid,
			       अचर संरचना object_id *new_oid,
			       पूर्णाङ्क summary_width)
{
	खाता *f = मानक_त्रुटि;

	यदि (verbosity < 0)
		फर्काऊ;

	strbuf_reset(&display_state->buf);

	विकल्प (display_state->format) {
	पक्ष DISPLAY_FORMAT_FULL:
	पक्ष DISPLAY_FORMAT_COMPACT: {
		पूर्णाङ्क width;

		यदि (!display_state->shown_url) {
			strbuf_addf(&display_state->buf, _("From %.*s\n"),
				    display_state->url_len, display_state->url);
			display_state->shown_url = 1;
		}

		width = (summary_width + म_माप(summary) - gettext_width(summary));
		remote = prettify_refname(remote);
		local = prettify_refname(local);

		strbuf_addf(&display_state->buf, " %c %-*s ", code, width, summary);

		यदि (display_state->format != DISPLAY_FORMAT_COMPACT)
			print_remote_to_local(display_state, remote, local);
		अन्यथा
			print_compact(display_state, remote, local);

		यदि (error)
			strbuf_addf(&display_state->buf, "  (%s)", error);

		रोक;
	}
	पक्ष DISPLAY_FORMAT_PORCELAIN:
		strbuf_addf(&display_state->buf, "%c %s %s %s", code,
			    oid_to_hex(old_oid), oid_to_hex(new_oid), local);
		f = मानक_निकास;
		रोक;
	स्वभाव:
		BUG("unexpected display format %d", display_state->format);
	};
	strbuf_addch(&display_state->buf, '\n');

	ख_माला_दो(display_state->buf.buf, f);
}

संरचना ref_update_display_info {
	bool failed;
	वर्ण success_code;
	वर्ण fail_code;
	वर्ण *summary;
	वर्ण *fail_detail;
	वर्ण *success_detail;
	वर्ण *ref;
	वर्ण *remote;
	संरचना object_id old_oid;
	संरचना object_id new_oid;
};

संरचना ref_update_display_info_array {
	संरचना ref_update_display_info *info;
	माप_प्रकार alloc, nr;
};

स्थिर संरचना ref_update_display_info *ref_update_display_info_append(
					   संरचना ref_update_display_info_array *array,
					   वर्ण success_code,
					   वर्ण fail_code,
					   अचर वर्ण *summary,
					   अचर वर्ण *success_detail,
					   अचर वर्ण *fail_detail,
					   अचर वर्ण *ref,
					   अचर वर्ण *remote,
					   अचर संरचना object_id *old_oid,
					   अचर संरचना object_id *new_oid)
{
	संरचना ref_update_display_info *info;

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

	फर्काऊ info;
}

स्थिर शून्य ref_update_display_info_set_failed(संरचना ref_update_display_info *info)
{
	info->failed = true;
}

स्थिर शून्य ref_update_display_info_free(संरचना ref_update_display_info *info)
{
	मुक्त(info->summary);
	मुक्त(info->success_detail);
	मुक्त(info->fail_detail);
	मुक्त(info->remote);
	मुक्त(info->ref);
}

स्थिर शून्य ref_update_display_info_display(संरचना ref_update_display_info *info,
					    संरचना display_state *display_state,
					    पूर्णाङ्क summary_width)
{
	display_ref_update(display_state,
			   info->failed ? info->fail_code : info->success_code,
			   info->summary,
			   info->failed ? info->fail_detail : info->success_detail,
			   info->remote, info->ref, &info->old_oid,
			   &info->new_oid, summary_width);
}

स्थिर पूर्णाङ्क update_local_ref(संरचना ref *ref,
			    संरचना ref_transaction *transaction,
			    अचर संरचना ref *remote_ref,
			    अचर संरचना fetch_config *config,
			    संरचना ref_update_display_info_array *display_array)
{
	संरचना commit *current = रिक्त, *updated;
	पूर्णाङ्क fast_forward = 0;

	यदि (!odb_has_object(the_repository->objects, &ref->new_oid,
			    ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR))
		die(_("object %s not found"), oid_to_hex(&ref->new_oid));

	यदि (oideq(&ref->old_oid, &ref->new_oid)) {
		यदि (verbosity > 0)
			ref_update_display_info_append(display_array, '=', '=',
						       _("[up to date]"), रिक्त,
						       रिक्त, ref->name,
						       remote_ref->name, &ref->old_oid,
						       &ref->new_oid);
		फर्काऊ 0;
	}

	यदि (!update_head_ok &&
	    !is_null_oid(&ref->old_oid) &&
	    branch_checked_out(ref->name)) {
		संरचना ref_update_display_info *info;
		/*
		 * If this is the head, and it's not okay to update
		 * the head, and the old value of the head isn't empty...
		 */
		info = ref_update_display_info_append(display_array, '!', '!',
						      _("[rejected]"), रिक्त,
						      _("can't fetch into checked-out branch"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);
		ref_update_display_info_set_failed(info);
		फर्काऊ 1;
	}

	यदि (!is_null_oid(&ref->old_oid) &&
	    starts_with(ref->name, "refs/tags/")) {
		संरचना ref_update_display_info *info;

		यदि (force || ref->force) {
			पूर्णाङ्क r;

			r = s_update_ref("updating tag", ref, transaction, 0);

			info = ref_update_display_info_append(display_array, 't', '!',
							      _("[tag update]"), रिक्त,
							      _("unable to update local ref"),
							      ref->name, remote_ref->name,
							      &ref->old_oid, &ref->new_oid);
			यदि (r)
				ref_update_display_info_set_failed(info);

			फर्काऊ r;
		} अन्यथा {
			info = ref_update_display_info_append(display_array, '!', '!',
							      _("[rejected]"), रिक्त,
							      _("would clobber existing tag"),
							      ref->name, remote_ref->name,
							      &ref->old_oid, &ref->new_oid);
			ref_update_display_info_set_failed(info);
			फर्काऊ 1;
		}
	}

	current = lookup_commit_reference_gently(the_repository,
						 &ref->old_oid, 1);
	updated = lookup_commit_reference_gently(the_repository,
						 &ref->new_oid, 1);
	यदि (!current || !updated) {
		संरचना ref_update_display_info *info;
		अचर वर्ण *सन्देश;
		अचर वर्ण *what;
		पूर्णाङ्क r;
		/*
		 * Nicely describe the new ref we're fetching.
		 * Base this on the remote's ref name, as it's
		 * more likely to follow a standard layout.
		 */
		यदि (starts_with(remote_ref->name, "refs/tags/")) {
			सन्देश = "storing tag";
			what = _("[new tag]");
		} अन्यथा यदि (starts_with(remote_ref->name, "refs/heads/")) {
			सन्देश = "storing head";
			what = _("[new branch]");
		} अन्यथा {
			सन्देश = "storing ref";
			what = _("[new ref]");
		}

		r = s_update_ref(सन्देश, ref, transaction, 0);

		info = ref_update_display_info_append(display_array, '*', '!',
						      what, रिक्त,
						      _("unable to update local ref"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);
		यदि (r)
			ref_update_display_info_set_failed(info);

		फर्काऊ r;
	}

	यदि (config->show_forced_updates) {
		uint64_t t_before = getnanotime();
		fast_forward = repo_in_merge_bases(the_repository, current,
						   updated);
		यदि (fast_forward < 0)
			die(रिक्त);
		forced_updates_ms += (getnanotime() - t_before) / 1000000;
	} अन्यथा {
		fast_forward = 1;
	}

	यदि (fast_forward) {
		संरचना ref_update_display_info *info;
		संरचना strbuf quickref = STRBUF_INIT;
		पूर्णाङ्क r;

		strbuf_add_unique_abbrev(&quickref, &current->object.oid, DEFAULT_ABBREV);
		strbuf_addstr(&quickref, "..");
		strbuf_add_unique_abbrev(&quickref, &ref->new_oid, DEFAULT_ABBREV);
		r = s_update_ref("fast-forward", ref, transaction, 1);

		info = ref_update_display_info_append(display_array, ' ', '!',
						      quickref.buf, रिक्त,
						      _("unable to update local ref"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);
		यदि (r)
			ref_update_display_info_set_failed(info);

		strbuf_release(&quickref);
		फर्काऊ r;
	} अन्यथा यदि (force || ref->force) {
		संरचना ref_update_display_info *info;
		संरचना strbuf quickref = STRBUF_INIT;
		पूर्णाङ्क r;

		strbuf_add_unique_abbrev(&quickref, &current->object.oid, DEFAULT_ABBREV);
		strbuf_addstr(&quickref, "...");
		strbuf_add_unique_abbrev(&quickref, &ref->new_oid, DEFAULT_ABBREV);
		r = s_update_ref("forced-update", ref, transaction, 1);

		info = ref_update_display_info_append(display_array, '+', '!',
						      quickref.buf, _("forced update"),
						      _("unable to update local ref"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);

		यदि (r)
			ref_update_display_info_set_failed(info);

		strbuf_release(&quickref);
		फर्काऊ r;
	} अन्यथा {
		संरचना ref_update_display_info *info;
		info = ref_update_display_info_append(display_array, '!', '!',
						      _("[rejected]"), रिक्त,
						      _("non-fast-forward"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);
		ref_update_display_info_set_failed(info);
		फर्काऊ 1;
	}
}

स्थिर अचर संरचना object_id *iterate_ref_map(शून्य *cb_data)
{
	संरचना ref **rm = cb_data;
	संरचना ref *ref = *rm;

	जबसम्म (ref && ref->status == REF_STATUS_REJECT_SHALLOW)
		ref = ref->next;
	यदि (!ref)
		फर्काऊ रिक्त;
	*rm = ref->next;
	फर्काऊ &ref->old_oid;
}

संरचना fetch_head {
	खाता *fp;
	संरचना strbuf buf;
};

स्थिर पूर्णाङ्क open_fetch_head(संरचना fetch_head *fetch_head)
{
	अचर वर्ण *filename = git_path_fetch_head(the_repository);

	यदि (write_fetch_head) {
		fetch_head->fp = ख_खोलो(filename, "a");
		यदि (!fetch_head->fp)
			फर्काऊ error_errno(_("cannot open '%s'"), filename);
		strbuf_init(&fetch_head->buf, 0);
	} अन्यथा {
		fetch_head->fp = रिक्त;
	}

	फर्काऊ 0;
}

स्थिर शून्य append_fetch_head(संरचना fetch_head *fetch_head,
			      अचर संरचना object_id *old_oid,
			      गणना fetch_head_status fetch_head_status,
			      अचर वर्ण *note,
			      अचर वर्ण *url, माप_प्रकार url_len)
{
	वर्ण old_oid_hex[GIT_MAX_HEXSZ + 1];
	अचर वर्ण *merge_status_marker;
	माप_प्रकार i;

	यदि (!fetch_head->fp)
		फर्काऊ;

	विकल्प (fetch_head_status) {
	पक्ष FETCH_HEAD_NOT_FOR_MERGE:
		merge_status_marker = "not-for-merge";
		रोक;
	पक्ष FETCH_HEAD_MERGE:
		merge_status_marker = "";
		रोक;
	स्वभाव:
		/* do not write anything to FETCH_HEAD */
		फर्काऊ;
	}

	strbuf_addf(&fetch_head->buf, "%s\t%s\t%s",
		    oid_to_hex_r(old_oid_hex, old_oid), merge_status_marker, note);
	लागि (i = 0; i < url_len; ++i)
		यदि ('\n' == url[i])
			strbuf_addstr(&fetch_head->buf, "\\n");
		अन्यथा
			strbuf_addch(&fetch_head->buf, url[i]);
	strbuf_addch(&fetch_head->buf, '\n');

	/*
	 * When using an atomic fetch, we do not want to update FETCH_HEAD if
	 * any of the reference updates fails. We thus have to write all
	 * updates to a buffer first and only commit it as soon as all
	 * references have been successfully updated.
	 */
	यदि (!atomic_fetch) {
		strbuf_write(&fetch_head->buf, fetch_head->fp);
		strbuf_reset(&fetch_head->buf);
	}
}

स्थिर शून्य commit_fetch_head(संरचना fetch_head *fetch_head)
{
	यदि (!fetch_head->fp || !atomic_fetch)
		फर्काऊ;
	strbuf_write(&fetch_head->buf, fetch_head->fp);
}

स्थिर शून्य close_fetch_head(संरचना fetch_head *fetch_head)
{
	यदि (!fetch_head->fp)
		फर्काऊ;

	ख_बंद(fetch_head->fp);
	strbuf_release(&fetch_head->buf);
}

स्थिर अचर वर्ण warn_show_forced_updates[] =
N_("fetch normally indicates which branches had a forced update,\n"
   "but that check has been disabled; to re-enable, use '--show-forced-updates'\n"
   "flag or run 'git config fetch.showForcedUpdates true'");
स्थिर अचर वर्ण warn_time_show_forced_updates[] =
N_("it took %.2f seconds to check forced updates; you can use\n"
   "'--no-show-forced-updates' or run 'git config fetch.showForcedUpdates false'\n"
   "to avoid this check\n");

स्थिर पूर्णाङ्क store_updated_refs(संरचना display_state *display_state,
			      पूर्णाङ्क connectivity_checked,
			      संरचना ref_transaction *transaction, संरचना ref *ref_map,
			      संरचना fetch_head *fetch_head,
			      अचर संरचना fetch_config *config,
			      संरचना ref_update_display_info_array *display_array)
{
	पूर्णाङ्क rc = 0;
	संरचना strbuf note = STRBUF_INIT;
	अचर वर्ण *what, *kind;
	संरचना ref *rm;
	पूर्णाङ्क want_status;

	यदि (!connectivity_checked) {
		संरचना check_connected_options opt = CHECK_CONNECTED_INIT;

		opt.exclude_hidden_refs_section = "fetch";
		rm = ref_map;
		यदि (check_connected(iterate_ref_map, &rm, &opt)) {
			rc = error(_("%s did not send all necessary objects"),
				   display_state->url);
			गच्छ पात;
		}
	}

	/*
	 * We do a pass for each fetch_head_status type in their enum order, so
	 * merged entries are written before not-for-merge. That lets readers
	 * use FETCH_HEAD as a refname to refer to the ref to be merged.
	 */
	लागि (want_status = FETCH_HEAD_MERGE;
	     want_status <= FETCH_HEAD_IGNORE;
	     want_status++) {
		लागि (rm = ref_map; rm; rm = rm->next) {
			संरचना ref *ref = रिक्त;

			यदि (rm->status == REF_STATUS_REJECT_SHALLOW) {
				यदि (want_status == FETCH_HEAD_MERGE)
					warning(_("rejected %s because shallow roots are not allowed to be updated"),
						rm->peer_ref ? rm->peer_ref->name : rm->name);
				जारी;
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
			यदि (fetch_head->fp) {
				संरचना commit *commit = रिक्त;

				/*
				 * References in "refs/tags/" are often going to point
				 * to annotated tags, which are not part of the
				 * commit-graph. We thus only try to look up refs in
				 * the graph which are not in that namespace to not
				 * regress performance in repositories with many
				 * annotated tags.
				 */
				यदि (!starts_with(rm->name, "refs/tags/"))
					commit = lookup_commit_in_graph(the_repository, &rm->old_oid);
				यदि (!commit) {
					commit = lookup_commit_reference_gently(the_repository,
										&rm->old_oid,
										1);
					यदि (!commit)
						rm->fetch_head_status = FETCH_HEAD_NOT_FOR_MERGE;
				}
			}

			यदि (rm->fetch_head_status != want_status)
				जारी;

			यदि (rm->peer_ref) {
				ref = alloc_ref(rm->peer_ref->name);
				oidcpy(&ref->old_oid, &rm->peer_ref->old_oid);
				oidcpy(&ref->new_oid, &rm->old_oid);
				ref->force = rm->peer_ref->force;
			}

			यदि (config->recurse_submodules != RECURSE_SUBMODULES_OFF &&
			    (!rm->peer_ref || !oideq(&ref->old_oid, &ref->new_oid))) {
				check_for_new_submodule_commits(&rm->old_oid);
			}

			यदि (!म_भेद(rm->name, "HEAD")) {
				kind = "";
				what = "";
			} अन्यथा यदि (skip_prefix(rm->name, "refs/heads/", &what)) {
				kind = "branch";
			} अन्यथा यदि (skip_prefix(rm->name, "refs/tags/", &what)) {
				kind = "tag";
			} अन्यथा यदि (skip_prefix(rm->name, "refs/remotes/", &what)) {
				kind = "remote-tracking branch";
			} अन्यथा {
				kind = "";
				what = rm->name;
			}

			strbuf_reset(&note);
			यदि (*what) {
				यदि (*kind)
					strbuf_addf(&note, "%s ", kind);
				strbuf_addf(&note, "'%s' of ", what);
			}

			append_fetch_head(fetch_head, &rm->old_oid,
					  rm->fetch_head_status,
					  note.buf, display_state->url,
					  display_state->url_len);

			यदि (ref) {
				rc |= update_local_ref(ref, transaction, rm,
						       config, display_array);
				मुक्त(ref);
			} अन्यथा यदि (write_fetch_head || dry_run) {
				/*
				 * Display fetches written to FETCH_HEAD (or
				 * would be written to FETCH_HEAD, if --dry-run
				 * is set).
				 */

				ref_update_display_info_append(display_array, '*', '*',
							       *kind ? kind : "branch",
							       रिक्त, रिक्त, "FETCH_HEAD",
							       rm->name, &rm->new_oid,
							       &rm->old_oid);
			}
		}
	}

	यदि (advice_enabled(ADVICE_FETCH_SHOW_FORCED_UPDATES)) {
		यदि (!config->show_forced_updates) {
			warning(_(warn_show_forced_updates));
		} अन्यथा यदि (forced_updates_ms > FORCED_UPDATES_DELAY_WARNING_IN_MS) {
			warning(_(warn_time_show_forced_updates),
				forced_updates_ms / 1000.0);
		}
	}

 पात:
	strbuf_release(&note);
	फर्काऊ rc;
}

/*
 * We would want to bypass the object transfer altogether if
 * everything we are going to fetch already exists and is connected
 * locally.
 */
स्थिर पूर्णाङ्क check_exist_and_connected(संरचना ref *ref_map)
{
	संरचना ref *rm = ref_map;
	संरचना check_connected_options opt = CHECK_CONNECTED_INIT;
	संरचना ref *r;

	/*
	 * If we are deepening a shallow clone we already have these
	 * objects reachable.  Running rev-list here will return with
	 * a good (0) exit status and we'll bypass the fetch that we
	 * really need to perform.  Claiming failure now will ensure
	 * we perform the network exchange to deepen our history.
	 */
	यदि (deepen)
		फर्काऊ -1;

	/*
	 * Similarly, if we need to refetch, we always want to perform a full
	 * fetch ignoring existing objects.
	 */
	यदि (refetch)
		फर्काऊ -1;


	/*
	 * check_connected() allows objects to merely be promised, but
	 * we need all direct targets to exist.
	 */
	लागि (r = rm; r; r = r->next) {
		यदि (!odb_has_object(the_repository->objects, &r->old_oid,
				    ODB_HAS_OBJECT_RECHECK_PACKED))
			फर्काऊ -1;
	}

	opt.quiet = 1;
	opt.exclude_hidden_refs_section = "fetch";
	फर्काऊ check_connected(iterate_ref_map, &rm, &opt);
}

स्थिर पूर्णाङ्क fetch_and_consume_refs(संरचना display_state *display_state,
				  संरचना transport *transport,
				  संरचना ref_transaction *transaction,
				  संरचना ref *ref_map,
				  संरचना fetch_head *fetch_head,
				  अचर संरचना fetch_config *config,
				  संरचना ref_update_display_info_array *display_array)
{
	पूर्णाङ्क connectivity_checked = 1;
	पूर्णाङ्क ret;

	/*
	 * We don't need to perform a fetch in case we can already satisfy all
	 * refs.
	 */
	ret = check_exist_and_connected(ref_map);
	यदि (ret) {
		trace2_region_enter("fetch", "fetch_refs", the_repository);
		ret = transport_fetch_refs(transport, ref_map);
		trace2_region_leave("fetch", "fetch_refs", the_repository);
		यदि (ret)
			गच्छ out;
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
	फर्काऊ ret;
}

स्थिर पूर्णाङ्क prune_refs(संरचना display_state *display_state,
		      संरचना refspec *rs,
		      संरचना ref_transaction *transaction,
		      संरचना ref *ref_map)
{
	पूर्णाङ्क result = 0;
	संरचना ref *ref, *stale_refs = get_stale_heads(rs, ref_map);
	संरचना strbuf err = STRBUF_INIT;
	संरचना string_list refnames = STRING_LIST_INIT_NODUP;

	लागि (ref = stale_refs; ref; ref = ref->next)
		string_list_append(&refnames, ref->name);

	यदि (!dry_run) {
		यदि (transaction) {
			लागि (ref = stale_refs; ref; ref = ref->next) {
				result = ref_transaction_delete(transaction, ref->name, रिक्त,
								रिक्त, 0, "fetch: prune", &err);
				यदि (result)
					गच्छ cleanup;
			}
		} अन्यथा {
			result = refs_delete_refs(get_main_ref_store(the_repository),
						  "fetch: prune", &refnames,
						  0);
		}
	}

	यदि (verbosity >= 0) {
		पूर्णाङ्क summary_width = transport_summary_width(stale_refs);

		लागि (ref = stale_refs; ref; ref = ref->next) {
			display_ref_update(display_state, '-', _("[deleted]"), रिक्त,
					   _("(none)"), ref->name,
					   &ref->new_oid, &ref->old_oid,
					   summary_width);
		}
		string_list_sort(&refnames);
		refs_warn_dangling_symrefs(get_main_ref_store(the_repository),
					   मानक_त्रुटि, "   ", dry_run, &refnames);
	}

cleanup:
	string_list_clear(&refnames, 0);
	strbuf_release(&err);
	free_refs(stale_refs);
	फर्काऊ result;
}

स्थिर शून्य check_not_current_branch(संरचना ref *ref_map)
{
	अचर वर्ण *path;
	लागि (; ref_map; ref_map = ref_map->next)
		यदि (ref_map->peer_ref &&
		    starts_with(ref_map->peer_ref->name, "refs/heads/") &&
		    (path = branch_checked_out(ref_map->peer_ref->name)))
			die(_("refusing to fetch into branch '%s' "
			      "checked out at '%s'"),
			    ref_map->peer_ref->name, path);
}

स्थिर पूर्णाङ्क truncate_fetch_head(शून्य)
{
	अचर वर्ण *filename = git_path_fetch_head(the_repository);
	खाता *fp = fopen_for_writing(filename);

	यदि (!fp)
		फर्काऊ error_errno(_("cannot open '%s'"), filename);
	ख_बंद(fp);
	फर्काऊ 0;
}

स्थिर शून्य set_option(संरचना transport *transport, अचर वर्ण *name, अचर वर्ण *मान)
{
	पूर्णाङ्क r = transport_set_option(transport, name, मान);
	यदि (r < 0)
		die(_("option \"%s\" value \"%s\" is not valid for %s"),
		    name, मान, transport->url);
	यदि (r > 0)
		warning(_("option \"%s\" is ignored for %s"),
			name, transport->url);
}


स्थिर पूर्णाङ्क add_oid(अचर संरचना reference *ref, शून्य *cb_data)
{
	संरचना oid_array *oids = cb_data;

	oid_array_append(oids, ref->oid);
	फर्काऊ 0;
}

स्थिर शून्य add_negotiation_tips(संरचना string_list *आगत_list,
				 संरचना oid_array **output_list,
				 अचर वर्ण *argname)
{
	संरचना oid_array *oids = xcalloc(1, परिमाण(*oids));
	पूर्णाङ्क i;

	लागि (i = 0; i < आगत_list->nr; i++) {
		अचर वर्ण *s = आगत_list->वस्तुहरू[i].string;
		संरचना refs_for_each_ref_options opts = {
			.pattern = s,
		};
		पूर्णाङ्क old_nr;
		यदि (!has_glob_specials(s)) {
			संरचना object_id oid;

			/* Ignore missing reference. */
			यदि (repo_get_oid(the_repository, s, &oid))
				जारी;
			/* Fail on missing object pointed by ref. */
			यदि (!odb_has_object(the_repository->objects, &oid, 0))
				die(_("the object %s does not exist"), s);

			oid_array_append(oids, &oid);
			जारी;
		}
		old_nr = oids->nr;
		refs_for_each_ref_ext(get_main_ref_store(the_repository),
				      add_oid, oids, &opts);
		यदि (old_nr == oids->nr)
			warning(_("ignoring %s=%s because it does not match any refs"),
				argname, s);
	}
	*output_list = oids;
}

स्थिर संरचना transport *prepare_transport(संरचना remote *remote, पूर्णाङ्क deepen,
					   संरचना list_objects_filter_options *filter_options)
{
	संरचना transport *transport;

	transport = transport_get(remote, रिक्त);
	transport_set_verbosity(transport, verbosity, progress);
	transport->family = family;
	यदि (upload_pack)
		set_option(transport, TRANS_OPT_UPLOADPACK, upload_pack);
	यदि (keep)
		set_option(transport, TRANS_OPT_KEEP, "yes");
	यदि (depth)
		set_option(transport, TRANS_OPT_DEPTH, depth);
	यदि (deepen && deepen_since)
		set_option(transport, TRANS_OPT_DEEPEN_SINCE, deepen_since);
	यदि (deepen && deepen_not.nr)
		set_option(transport, TRANS_OPT_DEEPEN_NOT,
			   (अचर वर्ण *)&deepen_not);
	यदि (deepen_relative)
		set_option(transport, TRANS_OPT_DEEPEN_RELATIVE, "yes");
	यदि (update_shallow)
		set_option(transport, TRANS_OPT_UPDATE_SHALLOW, "yes");
	यदि (refetch)
		set_option(transport, TRANS_OPT_REFETCH, "yes");
	यदि (filter_options->choice) {
		अचर वर्ण *spec =
			expand_list_objects_filter_spec(filter_options);
		set_option(transport, TRANS_OPT_LIST_OBJECTS_FILTER, spec);
		set_option(transport, TRANS_OPT_FROM_PROMISOR, "1");
	}
	यदि (negotiation_restrict.nr) {
		यदि (transport->smart_options)
			add_negotiation_tips(&negotiation_restrict,
					     &transport->smart_options->negotiation_restrict_tips,
					     "--negotiation-restrict");
		अन्यथा
			warning(_("ignoring %s because the protocol does not support it"),
				"--negotiation-restrict");
	} अन्यथा यदि (remote->negotiation_restrict.nr) {
		संरचना string_list_item *item;
		for_each_string_list_item(item, &remote->negotiation_restrict)
			string_list_append(&negotiation_restrict, item->string);
		यदि (transport->smart_options)
			add_negotiation_tips(&negotiation_restrict,
					     &transport->smart_options->negotiation_restrict_tips,
					     "--negotiation-restrict");
		अन्यथा {
			संरचना strbuf config_name = STRBUF_INIT;
			strbuf_addf(&config_name, "remote.%s.negotiationRestrict", remote->name);
			warning(_("ignoring %s because the protocol does not support it"),
				config_name.buf);
			strbuf_release(&config_name);
		}
	}
	यदि (negotiation_include.nr) {
		यदि (transport->smart_options)
			add_negotiation_tips(&negotiation_include,
					     &transport->smart_options->negotiation_include_tips,
					     "--negotiation-include");
		अन्यथा
			warning(_("ignoring %s because the protocol does not support it"),
				"--negotiation-include");
	} अन्यथा यदि (remote->negotiation_include.nr) {
		यदि (transport->smart_options) {
			add_negotiation_tips(&remote->negotiation_include,
					     &transport->smart_options->negotiation_include_tips,
					     "--negotiation-include");
		} अन्यथा {
			संरचना strbuf config_name = STRBUF_INIT;
			strbuf_addf(&config_name, "remote.%s.negotiationInclude", remote->name);
			warning(_("ignoring %s because the protocol does not support it"),
				config_name.buf);
			strbuf_release(&config_name);
		}
	}
	फर्काऊ transport;
}

स्थिर पूर्णाङ्क backfill_tags(संरचना display_state *display_state,
			 संरचना transport *transport,
			 संरचना ref_transaction *transaction,
			 संरचना ref *ref_map,
			 संरचना fetch_head *fetch_head,
			 अचर संरचना fetch_config *config,
			 संरचना ref_update_display_info_array *display_array,
			 संरचना list_objects_filter_options *filter_options)
{
	पूर्णाङ्क retcode, cannot_reuse;

	/*
	 * Once we have set TRANS_OPT_DEEPEN_SINCE, we can't unset it
	 * when remote helper is used (setting it to an empty string
	 * is not unsetting). We could extend the remote helper
	 * protocol for that, but for now, just force a new connection
	 * without deepen-since. Similar story for deepen-not.
	 */
	cannot_reuse = transport->cannot_reuse ||
		deepen_since || deepen_not.nr;
	यदि (cannot_reuse) {
		gsecondary = prepare_transport(transport->remote, 0, filter_options);
		transport = gsecondary;
	}

	transport_set_option(transport, TRANS_OPT_FOLLOWTAGS, रिक्त);
	transport_set_option(transport, TRANS_OPT_DEPTH, "0");
	transport_set_option(transport, TRANS_OPT_DEEPEN_RELATIVE, रिक्त);
	retcode = fetch_and_consume_refs(display_state, transport, transaction, ref_map,
					 fetch_head, config, display_array);

	यदि (gsecondary) {
		transport_disconnect(gsecondary);
		gsecondary = रिक्त;
	}

	फर्काऊ retcode;
}

स्थिर अचर वर्ण *strip_refshead(अचर वर्ण *name){
	skip_prefix(name, "refs/heads/", &name);
	फर्काऊ name;
}

स्थिर शून्य set_head_advice_सन्देश(अचर वर्ण *remote, अचर वर्ण *head_name)
{
	अचर वर्ण message_advice_set_head[] =
	N_("Run 'git remote set-head %s %s' to follow the change, or modify\n"
	   "either of the 'remote.%s.followRemoteHEAD' or 'fetch.followRemoteHEAD'\n"
	   "configuration variables to handle the situation differently.\n\n"

	   "Using this specific setting\n\n"
	   "    git config set remote.%s.followRemoteHEAD warn-if-not-%s\n\n"
	   "will suppress the warning until the remote changes HEAD to something else.");

	advise_if_enabled(ADVICE_FETCH_SET_HEAD_WARN, _(message_advice_set_head),
			remote, head_name, remote, remote, head_name);
}

स्थिर शून्य warn_set_head(अचर वर्ण *remote, अचर वर्ण *head_name,
			संरचना strbuf *buf_prev, पूर्णाङ्क updateres) {
	संरचना strbuf buf_prefix = STRBUF_INIT;
	अचर वर्ण *prev_head = रिक्त;

	strbuf_addf(&buf_prefix, "refs/remotes/%s/", remote);
	skip_prefix(buf_prev->buf, buf_prefix.buf, &prev_head);

	यदि (prev_head && म_भेद(prev_head, head_name)) {
		म_लिखो("'HEAD' at '%s' is '%s', but we have '%s' locally.\n",
			remote, head_name, prev_head);
		set_head_advice_सन्देश(remote, head_name);
	}
	अन्यथा यदि (updateres && buf_prev->len) {
		म_लिखो("'HEAD' at '%s' is '%s', "
			"but we have a detached HEAD pointing to '%s' locally.\n",
			remote, head_name, buf_prev->buf);
		set_head_advice_सन्देश(remote, head_name);
	}
	strbuf_release(&buf_prefix);
}

स्थिर पूर्णाङ्क set_head(अचर संरचना ref *remote_refs, संरचना remote *remote,
			पूर्णाङ्क follow_remote_head)
{
	पूर्णाङ्क result = 0, create_only, baremirror, was_detached;
	संरचना strbuf b_head = STRBUF_INIT, b_remote_head = STRBUF_INIT,
		      b_local_head = STRBUF_INIT;
	अचर वर्ण *no_warn_branch = remote->no_warn_branch;
	वर्ण *head_name = रिक्त;
	संरचना ref *ref, *matches;
	संरचना ref *fetch_map = रिक्त, **fetch_map_tail = &fetch_map;
	संरचना refspec_item refspec = {
		.force = 0,
		.pattern = 1,
		.src = (वर्ण *) "refs/heads/*",
		.dst = (वर्ण *) "refs/heads/*",
	};
	संरचना string_list heads = STRING_LIST_INIT_DUP;
	संरचना ref_store *refs = get_main_ref_store(the_repository);

	get_fetch_map(remote_refs, &refspec, &fetch_map_tail, 0);
	matches = guess_remote_head(find_ref_by_name(remote_refs, "HEAD"),
				    fetch_map, REMOTE_GUESS_HEAD_ALL);
	लागि (ref = matches; ref; ref = ref->next) {
		string_list_append(&heads, strip_refshead(ref->name));
	}

	यदि (!heads.nr)
		result = 1;
	अन्यथा यदि (heads.nr > 1)
		result = 1;
	अन्यथा
		head_name = xstrdup(heads.वस्तुहरू[0].string);

	यदि (!head_name)
		गच्छ cleanup;
	baremirror = is_bare_repository(the_repository) && remote->mirror;
	create_only = follow_remote_head == FOLLOW_REMOTE_ALWAYS ? 0 : !baremirror;
	यदि (baremirror) {
		strbuf_addstr(&b_head, "HEAD");
		strbuf_addf(&b_remote_head, "refs/heads/%s", head_name);
	} अन्यथा {
		strbuf_addf(&b_head, "refs/remotes/%s/HEAD", remote->name);
		strbuf_addf(&b_remote_head, "refs/remotes/%s/%s", remote->name, head_name);
	}
	/* make sure it's valid */
	यदि (!baremirror && !refs_ref_exists(refs, b_remote_head.buf)) {
		result = 1;
		गच्छ cleanup;
	}
	was_detached = refs_update_symref_extended(refs, b_head.buf, b_remote_head.buf,
					"fetch", &b_local_head, create_only);
	यदि (was_detached == -1) {
		result = 1;
		गच्छ cleanup;
	}
	यदि (verbosity >= 0 &&
		follow_remote_head == FOLLOW_REMOTE_WARN &&
		(!no_warn_branch || म_भेद(no_warn_branch, head_name)))
		warn_set_head(remote->name, head_name, &b_local_head, was_detached);

cleanup:
	मुक्त(head_name);
	free_refs(fetch_map);
	free_refs(matches);
	string_list_clear(&heads, 0);
	strbuf_release(&b_head);
	strbuf_release(&b_local_head);
	strbuf_release(&b_remote_head);
	फर्काऊ result;
}

संरचना ref_rejection_data {
	पूर्णाङ्क *retcode;
	bool conflict_सन्देश_shown;
	bool case_sensitive_सन्देश_shown;
	अचर वर्ण *remote_name;
	संरचना strmap *rejected_refs;
};

स्थिर शून्य ref_transaction_rejection_handler(अचर वर्ण *refname,
					      अचर संरचना object_id *old_oid UNUSED,
					      अचर संरचना object_id *new_oid UNUSED,
					      अचर वर्ण *old_target UNUSED,
					      अचर वर्ण *new_target UNUSED,
					      गणना ref_transaction_error err,
					      अचर वर्ण *details,
					      शून्य *cb_data)
{
	संरचना ref_rejection_data *data = cb_data;

	यदि (err == REF_TRANSACTION_ERROR_CASE_CONFLICT && repo_ignore_case(the_repository) &&
	    !data->case_sensitive_सन्देश_shown) {
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
		data->case_sensitive_सन्देश_shown = true;
	} अन्यथा यदि (err == REF_TRANSACTION_ERROR_NAME_CONFLICT &&
		   !data->conflict_सन्देश_shown) {
		error(_("some local refs could not be updated; try running\n"
			" 'git remote prune %s' to remove any old, conflicting "
			"branches"), data->remote_name);
		data->conflict_सन्देश_shown = true;
	} अन्यथा {
		यदि (details)
			error("%s", details);
		अन्यथा
			error(_("fetching ref %s failed: %s"),
			      refname, ref_transaction_error_सन्देश(err));
	}

	strmap_put(data->rejected_refs, refname, रिक्त);
	*data->retcode = 1;
}

/*
 * Commit the reference transaction. If it isn't an atomic transaction, handle
 * rejected updates as part of using batched updates.
 */
स्थिर पूर्णाङ्क commit_ref_transaction(संरचना ref_transaction **transaction,
				  bool is_atomic, अचर वर्ण *remote_name,
				  संरचना strmap *rejected_refs,
				  संरचना strbuf *err)
{
	पूर्णाङ्क retcode = ref_transaction_commit(*transaction, err);
	यदि (retcode)
		गच्छ out;

	यदि (!is_atomic) {
		संरचना ref_rejection_data data = {
			.conflict_सन्देश_shown = 0,
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
	*transaction = रिक्त;
	फर्काऊ retcode;
}

स्थिर शून्य collect_updated_tips(संरचना oidset *tips, संरचना ref *ref_map)
{
	संरचना ref *rm;
	लागि (rm = ref_map; rm; rm = rm->next) {
		संरचना commit *commit;
		/*
		 * Shallow-rejected refs are not stored and their history
		 * is incomplete, so skip them.
		 */
		यदि (rm->status == REF_STATUS_REJECT_SHALLOW)
			जारी;
		यदि (is_null_oid(&rm->old_oid))
			जारी;
		यदि (rm->peer_ref &&
		    oideq(&rm->old_oid, &rm->peer_ref->old_oid))
			जारी;
		commit = lookup_commit_reference_gently(the_repository,
							&rm->old_oid, 1);
		यदि (commit)
			oidset_घुसाउ(tips, &commit->object.oid);
	}
}

स्थिर पूर्णाङ्क do_fetch(संरचना transport *transport,
		    संरचना refspec *rs,
		    अचर संरचना fetch_config *config,
		    संरचना list_objects_filter_options *filter_options,
		    संरचना oidset *updated_tips)
{
	संरचना ref_transaction *transaction = रिक्त;
	संरचना ref *ref_map = रिक्त;
	संरचना display_state display_state = { 0 };
	पूर्णाङ्क autotags = (transport->remote->fetch_tags == 1);
	पूर्णाङ्क retcode = 0;
	अचर संरचना ref *remote_refs;
	संरचना transport_ls_refs_options transport_ls_refs_options =
		TRANSPORT_LS_REFS_OPTIONS_INIT;
	संरचना fetch_head fetch_head = { 0 };
	संरचना strbuf err = STRBUF_INIT;
	संरचना ref_update_display_info_array display_array = { 0 };
	संरचना strmap rejected_refs = STRMAP_INIT;
	पूर्णाङ्क summary_width = 0;
	पूर्णाङ्क follow_remote_head = FOLLOW_REMOTE_NEVER;

	यदि (tags == TAGS_DEFAULT) {
		यदि (transport->remote->fetch_tags == 2)
			tags = TAGS_SET;
		यदि (transport->remote->fetch_tags == -1)
			tags = TAGS_UNSET;
	}

	/* if not appending, truncate FETCH_HEAD */
	यदि (!append && write_fetch_head) {
		retcode = truncate_fetch_head();
		यदि (retcode)
			गच्छ cleanup;
	}

	यदि (rs->nr) {
		refspec_ref_prefixes(rs, &transport_ls_refs_options.ref_prefixes);
	} अन्यथा {
		संरचना branch *branch = branch_get(रिक्त);

		यदि (transport->remote->fetch.nr) {
			refspec_ref_prefixes(&transport->remote->fetch,
					     &transport_ls_refs_options.ref_prefixes);
			/*
			 * See remote.c's handling of remote.<name>.followRemoteHEAD
			 * for the analogous, still-unresolved case.
			 */
			यदि (transport->remote->follow_remote_head)
				follow_remote_head = transport->remote->follow_remote_head;
			अन्यथा यदि (config->follow_remote_head_seen)
				follow_remote_head = get_follow_remote_head(config->follow_remote_head_raw);
			अन्यथा
				follow_remote_head = BUILTIN_FOLLOW_REMOTE_HEAD_DFLT;
		}
		यदि (branch && branch_has_merge_config(branch) &&
		    !म_भेद(branch->remote_name, transport->remote->name)) {
			पूर्णाङ्क i;
			लागि (i = 0; i < branch->merge_nr; i++) {
				strvec_थप(&transport_ls_refs_options.ref_prefixes,
					    branch->merge[i]->src);
			}
		}

		/*
		 * If there are no refs specified to fetch, then we just
		 * fetch HEAD; mention that to narrow the advertisement.
		 */
		यदि (!transport_ls_refs_options.ref_prefixes.nr)
			strvec_थप(&transport_ls_refs_options.ref_prefixes,
				    "HEAD");
	}

	यदि (tags == TAGS_SET || tags == TAGS_DEFAULT)
		strvec_थप(&transport_ls_refs_options.ref_prefixes,
			    "refs/tags/");

	यदि (follow_remote_head != FOLLOW_REMOTE_NEVER)
		strvec_थप(&transport_ls_refs_options.ref_prefixes,
			    "HEAD");

	/*
	 * Only initiate ref listing if we have at least one ref we want to
	 * know about.
	 */
	यदि (transport_ls_refs_options.ref_prefixes.nr) {
		trace2_region_enter("fetch", "remote_refs", the_repository);
		remote_refs = transport_get_remote_refs(transport,
							&transport_ls_refs_options);
		trace2_region_leave("fetch", "remote_refs", the_repository);
	} अन्यथा
		remote_refs = रिक्त;

	transport_ls_refs_options_release(&transport_ls_refs_options);

	ref_map = get_ref_map(transport->remote, remote_refs, rs,
			      tags, &autotags);
	यदि (!update_head_ok)
		check_not_current_branch(ref_map);

	retcode = open_fetch_head(&fetch_head);
	यदि (retcode)
		गच्छ cleanup;

	display_state_init(&display_state, ref_map, transport->url,
			   config->display_format);

	यदि (atomic_fetch) {
		transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
							  0, &err);
		यदि (!transaction) {
			retcode = -1;
			गच्छ cleanup;
		}
	}

	यदि (tags == TAGS_DEFAULT && autotags)
		transport_set_option(transport, TRANS_OPT_FOLLOWTAGS, "1");
	यदि (prune) {
		/*
		 * We only prune based on refspecs specified
		 * explicitly (via command line or configuration); we
		 * don't care whether --tags was specified.
		 */
		यदि (rs->nr) {
			retcode = prune_refs(&display_state, rs, transaction, ref_map);
		} अन्यथा {
			retcode = prune_refs(&display_state, &transport->remote->fetch,
					     transaction, ref_map);
		}
		यदि (retcode != 0)
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
	यदि (!transaction) {
		transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
							  REF_TRANSACTION_ALLOW_FAILURE, &err);
		यदि (!transaction) {
			retcode = -1;
			गच्छ cleanup;
		}
	}

	यदि (fetch_and_consume_refs(&display_state, transport, transaction, ref_map,
				   &fetch_head, config, &display_array)) {
		retcode = 1;
		गच्छ cleanup;
	}

	/*
	 * If neither --no-tags nor --tags was specified, do automated tag
	 * following.
	 */
	यदि (tags == TAGS_DEFAULT && autotags) {
		संरचना ref *tags_ref_map = रिक्त, **tail = &tags_ref_map;

		find_non_local_tags(remote_refs, transaction, &tags_ref_map, &tail);
		यदि (tags_ref_map) {
			/*
			 * If backfilling of tags fails then we want to tell
			 * the user so, but we have to continue regardless to
			 * populate upstream information of the references we
			 * have already fetched above. The exception though is
			 * when `--atomic` is passed: in that case we'll abort
			 * the transaction and don't commit anything.
			 */
			यदि (backfill_tags(&display_state, transport, transaction, tags_ref_map,
					  &fetch_head, config, &display_array, filter_options))
				retcode = 1;
		}

		free_refs(tags_ref_map);
	}

	यदि (retcode)
		गच्छ cleanup;

	यदि (verbosity >= 0)
		summary_width = transport_summary_width(ref_map);

	retcode = commit_ref_transaction(&transaction, atomic_fetch,
					 transport->remote->name,
					 &rejected_refs, &err);
	/*
	 * With '--atomic', bail out if the transaction fails. Without '--atomic',
	 * continue to fetch head and perform other post-fetch operations.
	 */
	यदि (retcode && atomic_fetch)
		गच्छ cleanup;

	commit_fetch_head(&fetch_head);

	collect_updated_tips(updated_tips, ref_map);

	यदि (set_upstream) {
		संरचना branch *branch = branch_get("HEAD");
		संरचना ref *rm;
		संरचना ref *source_ref = रिक्त;

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
		लागि (rm = ref_map; rm; rm = rm->next) {
			यदि (!rm->peer_ref) {
				यदि (source_ref) {
					warning(_("multiple branches detected, incompatible with --set-upstream"));
					गच्छ cleanup;
				} अन्यथा {
					source_ref = rm;
				}
			}
		}
		यदि (source_ref) {
			यदि (!branch) {
				अचर वर्ण *shortname = source_ref->name;
				skip_prefix(shortname, "refs/heads/", &shortname);

				warning(_("could not set upstream of HEAD to '%s' from '%s' when "
					  "it does not point to any branch."),
					shortname, transport->remote->name);
				गच्छ cleanup;
			}

			यदि (!म_भेद(source_ref->name, "HEAD") ||
			    starts_with(source_ref->name, "refs/heads/"))
				install_branch_config(0,
						      branch->name,
						      transport->remote->name,
						      source_ref->name);
			अन्यथा यदि (starts_with(source_ref->name, "refs/remotes/"))
				warning(_("not setting upstream for a remote remote-tracking branch"));
			अन्यथा यदि (starts_with(source_ref->name, "refs/tags/"))
				warning(_("not setting upstream for a remote tag"));
			अन्यथा
				warning(_("unknown branch type"));
		} अन्यथा {
			warning(_("no source branch found;\n"
				  "you need to specify exactly one branch with the --set-upstream option"));
		}
	}
	यदि (follow_remote_head != FOLLOW_REMOTE_NEVER) {
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
	यदि (retcode && !atomic_fetch && transaction)
		commit_ref_transaction(&transaction, false,
				       transport->remote->name,
				       &rejected_refs, &err);

	लागि (माप_प्रकार i = 0; i < display_array.nr; i++) {
		संरचना ref_update_display_info *info = &display_array.info[i];

		यदि (!info->failed && strmap_contains(&rejected_refs, info->ref))
			ref_update_display_info_set_failed(info);
		ref_update_display_info_display(info, &display_state, summary_width);
		ref_update_display_info_free(info);
	}

	यदि (retcode) {
		यदि (err.len) {
			error("%s", err.buf);
			strbuf_reset(&err);
		}
		यदि (transaction && ref_transaction_abort(transaction, &err) &&
		    err.len)
			error("%s", err.buf);
		transaction = रिक्त;
	}

	यदि (transaction)
		ref_transaction_free(transaction);

	मुक्त(display_array.info);
	strmap_clear(&rejected_refs, 0);
	display_state_release(&display_state);
	close_fetch_head(&fetch_head);
	strbuf_release(&err);
	free_refs(ref_map);
	फर्काऊ retcode;
}

स्थिर पूर्णाङ्क get_one_remote_for_fetch(संरचना remote *remote, शून्य *priv)
{
	संरचना string_list *list = priv;
	यदि (!remote->skip_default_update)
		string_list_append(list, remote->name);
	फर्काऊ 0;
}

स्थिर शून्य add_options_to_argv(संरचना strvec *argv,
				अचर संरचना fetch_config *config)
{
	यदि (dry_run)
		strvec_थप(argv, "--dry-run");
	यदि (prune != -1)
		strvec_थप(argv, prune ? "--prune" : "--no-prune");
	यदि (prune_tags != -1)
		strvec_थप(argv, prune_tags ? "--prune-tags" : "--no-prune-tags");
	यदि (update_head_ok)
		strvec_थप(argv, "--update-head-ok");
	यदि (force)
		strvec_थप(argv, "--force");
	यदि (keep)
		strvec_थप(argv, "--keep");
	यदि (config->recurse_submodules == RECURSE_SUBMODULES_ON)
		strvec_थप(argv, "--recurse-submodules");
	अन्यथा यदि (config->recurse_submodules == RECURSE_SUBMODULES_OFF)
		strvec_थप(argv, "--no-recurse-submodules");
	अन्यथा यदि (config->recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND)
		strvec_थप(argv, "--recurse-submodules=on-demand");
	यदि (tags == TAGS_SET)
		strvec_थप(argv, "--tags");
	अन्यथा यदि (tags == TAGS_UNSET)
		strvec_थप(argv, "--no-tags");
	यदि (verbosity >= 2)
		strvec_थप(argv, "-v");
	यदि (verbosity >= 1)
		strvec_थप(argv, "-v");
	अन्यथा यदि (verbosity < 0)
		strvec_थप(argv, "-q");
	यदि (family == TRANSPORT_FAMILY_IPV4)
		strvec_थप(argv, "--ipv4");
	अन्यथा यदि (family == TRANSPORT_FAMILY_IPV6)
		strvec_थप(argv, "--ipv6");
	यदि (!write_fetch_head)
		strvec_थप(argv, "--no-write-fetch-head");
	यदि (config->display_format == DISPLAY_FORMAT_PORCELAIN)
		strvec_pushf(argv, "--porcelain");
}

/* Fetch multiple remotes in parallel */

संरचना parallel_fetch_state {
	अचर वर्ण **argv;
	संरचना string_list *remotes;
	पूर्णाङ्क next, result;
	अचर संरचना fetch_config *config;
};

स्थिर पूर्णाङ्क fetch_next_remote(संरचना child_process *cp,
			     संरचना strbuf *out UNUSED,
			     शून्य *cb, शून्य **task_cb)
{
	संरचना parallel_fetch_state *state = cb;
	वर्ण *remote;

	यदि (state->next < 0 || state->next >= state->remotes->nr)
		फर्काऊ 0;

	remote = state->remotes->वस्तुहरू[state->next++].string;
	*task_cb = remote;

	strvec_pushv(&cp->args, state->argv);
	strvec_थप(&cp->args, remote);
	cp->git_cmd = 1;

	यदि (verbosity >= 0 && state->config->display_format != DISPLAY_FORMAT_PORCELAIN)
		म_लिखो(_("Fetching %s\n"), remote);

	फर्काऊ 1;
}

स्थिर पूर्णाङ्क fetch_failed_to_start(संरचना strbuf *out UNUSED,
				 शून्य *cb, शून्य *task_cb)
{
	संरचना parallel_fetch_state *state = cb;
	अचर वर्ण *remote = task_cb;

	state->result = error(_("could not fetch %s"), remote);

	फर्काऊ 0;
}

स्थिर पूर्णाङ्क fetch_finished(पूर्णाङ्क result, संरचना strbuf *out,
			  शून्य *cb, शून्य *task_cb)
{
	संरचना parallel_fetch_state *state = cb;
	अचर वर्ण *remote = task_cb;

	यदि (result) {
		strbuf_addf(out, _("could not fetch '%s' (exit code: %d)\n"),
			    remote, result);
		state->result = -1;
	}

	फर्काऊ 0;
}

स्थिर पूर्णाङ्क fetch_multiple(संरचना string_list *list, पूर्णाङ्क max_children,
			  अचर संरचना fetch_config *config)
{
	पूर्णाङ्क i, result = 0;
	संरचना strvec argv = STRVEC_INIT;

	यदि (!append && write_fetch_head) {
		पूर्णाङ्क errcode = truncate_fetch_head();
		यदि (errcode)
			फर्काऊ errcode;
	}

	/*
	 * Cancel out the fetch.bundleURI config when running subprocesses,
	 * to avoid fetching from the same bundle list multiple times.
	 */
	strvec_pushl(&argv, "-c", "fetch.bundleURI=",
		     "fetch", "--append", "--no-auto-gc",
		     "--no-write-commit-graph", रिक्त);
	लागि (i = 0; i < server_options.nr; i++)
		strvec_pushf(&argv, "--server-option=%s", server_options.वस्तुहरू[i].string);
	add_options_to_argv(&argv, config);

	यदि (max_children != 1 && list->nr != 1) {
		संरचना parallel_fetch_state state = { argv.v, list, 0, 0, config };
		अचर संरचना run_process_parallel_opts opts = {
			.tr2_category = "fetch",
			.tr2_label = "parallel/fetch",

			.processes = max_children,

			.get_next_task = &fetch_next_remote,
			.start_failure = &fetch_failed_to_start,
			.task_finished = &fetch_finished,
			.data = &state,
		};

		strvec_थप(&argv, "--end-of-options");

		run_processes_parallel(&opts);
		result = state.result;
	} अन्यथा
		लागि (i = 0; i < list->nr; i++) {
			अचर वर्ण *name = list->वस्तुहरू[i].string;
			संरचना child_process cmd = CHILD_PROCESS_INIT;

			strvec_pushv(&cmd.args, argv.v);
			strvec_थप(&cmd.args, name);
			यदि (verbosity >= 0 && config->display_format != DISPLAY_FORMAT_PORCELAIN)
				म_लिखो(_("Fetching %s\n"), name);
			cmd.git_cmd = 1;
			यदि (run_command(&cmd)) {
				error(_("could not fetch %s"), name);
				result = 1;
			}
		}

	strvec_clear(&argv);
	फर्काऊ !!result;
}

/*
 * Fetching from the promisor remote should use the given filter-spec
 * or inherit the default filter-spec from the config.
 */
स्थिर अन्तःस्थ शून्य fetch_one_setup_partial(संरचना remote *remote,
					   संरचना list_objects_filter_options *filter_options)
{
	/*
	 * Explicit --no-filter argument overrides everything, regardless
	 * of any prior partial clones and fetches.
	 */
	यदि (filter_options->no_filter)
		फर्काऊ;

	/*
	 * If no prior partial clone/fetch and the current fetch DID NOT
	 * request a partial-fetch, do a normal fetch.
	 */
	यदि (!repo_has_promisor_remote(the_repository) && !filter_options->choice)
		फर्काऊ;

	/*
	 * If this is a partial-fetch request, we enable partial on
	 * this repo if not already enabled and remember the given
	 * filter-spec as the default for subsequent fetches to this
	 * remote if there is currently no default filter-spec.
	 */
	यदि (filter_options->choice) {
		partial_clone_register(remote->name, filter_options);
		फर्काऊ;
	}

	/*
	 * Do a partial-fetch from the promisor remote using either the
	 * explicitly given filter-spec or inherit the filter-spec from
	 * the config.
	 */
	यदि (!filter_options->choice)
		partial_clone_get_default_filter_spec(filter_options, remote->name);
	फर्काऊ;
}

स्थिर पूर्णाङ्क fetch_one(संरचना remote *remote, पूर्णाङ्क argc, अचर वर्ण **argv,
		     पूर्णाङ्क prune_tags_ok, पूर्णाङ्क use_stdin_refspecs,
		     अचर संरचना fetch_config *config,
		     संरचना list_objects_filter_options *filter_options,
		     संरचना oidset *updated_tips)
{
	संरचना refspec rs = REFSPEC_INIT_FETCH(the_hash_algo);
	पूर्णाङ्क i;
	पूर्णाङ्क exit_code;
	पूर्णाङ्क maybe_prune_tags;
	पूर्णाङ्क remote_via_config = remote_is_configured(remote, 0);

	यदि (!remote)
		die(_("no remote repository specified; please specify either a URL or a\n"
		      "remote name from which new revisions should be fetched"));

	gtransport = prepare_transport(remote, 1, filter_options);

	यदि (prune < 0) {
		/* no command line request */
		यदि (0 <= remote->prune)
			prune = remote->prune;
		अन्यथा यदि (0 <= config->prune)
			prune = config->prune;
		अन्यथा
			prune = PRUNE_BY_DEFAULT;
	}

	यदि (prune_tags < 0) {
		/* no command line request */
		यदि (0 <= remote->prune_tags)
			prune_tags = remote->prune_tags;
		अन्यथा यदि (0 <= config->prune_tags)
			prune_tags = config->prune_tags;
		अन्यथा
			prune_tags = PRUNE_TAGS_BY_DEFAULT;
	}

	maybe_prune_tags = prune_tags_ok && prune_tags;
	यदि (maybe_prune_tags && remote_via_config)
		refspec_append(&remote->fetch, TAG_REFSPEC);

	यदि (maybe_prune_tags && (argc || !remote_via_config))
		refspec_append(&rs, TAG_REFSPEC);

	लागि (i = 0; i < argc; i++) {
		यदि (!म_भेद(argv[i], "tag")) {
			i++;
			यदि (i >= argc)
				die(_("you need to specify a tag name"));

			refspec_appendf(&rs, "refs/tags/%s:refs/tags/%s",
					argv[i], argv[i]);
		} अन्यथा {
			refspec_append(&rs, argv[i]);
		}
	}

	यदि (use_stdin_refspecs) {
		संरचना strbuf पङ्क्ति = STRBUF_INIT;
		जबसम्म (strbuf_getline_lf(&पङ्क्ति, मानक_निवेश) != खातापूर्ण)
			refspec_append(&rs, पङ्क्ति.buf);
		strbuf_release(&पङ्क्ति);
	}

	यदि (server_options.nr)
		gtransport->server_options = &server_options;

	sigchain_थप_common(unlock_pack_on_signal);
	निकास_पर(unlock_pack_atexit);
	sigchain_थप(SIGPIPE, संक_छोड़ो);
	exit_code = do_fetch(gtransport, &rs, config, filter_options,
			     updated_tips);
	sigchain_झिक(SIGPIPE);
	refspec_clear(&rs);
	transport_disconnect(gtransport);
	gtransport = रिक्त;
	फर्काऊ exit_code;
}

पूर्णाङ्क cmd_fetch(पूर्णाङ्क argc,
	      अचर वर्ण **argv,
	      अचर वर्ण *prefix,
	      संरचना repository *repo UNUSED)
{
	संरचना fetch_config config = {
		.display_format = DISPLAY_FORMAT_FULL,
		.follow_remote_head_raw = रिक्त,
		.follow_remote_head_seen = 0,
		.prune = -1,
		.prune_tags = -1,
		.show_forced_updates = 1,
		.recurse_submodules = RECURSE_SUBMODULES_DEFAULT,
		.parallel = 1,
		.submodule_fetch_jobs = -1,
	};
	अचर वर्ण *submodule_prefix = "";
	अचर वर्ण *bundle_uri;
	संरचना string_list list = STRING_LIST_INIT_DUP;
	संरचना list_objects_filter_options filter_options = LIST_OBJECTS_FILTER_INIT;
	संरचना remote *remote = रिक्त;
	पूर्णाङ्क all = -1, multiple = 0;
	पूर्णाङ्क result = 0;
	पूर्णाङ्क prune_tags_ok = 1;
	पूर्णाङ्क enable_auto_gc = 1;
	पूर्णाङ्क unshallow = 0;
	पूर्णाङ्क max_jobs = -1;
	पूर्णाङ्क recurse_submodules_cli = RECURSE_SUBMODULES_DEFAULT;
	पूर्णाङ्क recurse_submodules_default = RECURSE_SUBMODULES_ON_DEMAND;
	पूर्णाङ्क fetch_write_commit_graph = -1;
	पूर्णाङ्क stdin_refspecs = 0;
	पूर्णाङ्क negotiate_only = 0;
	पूर्णाङ्क porcelain = 0;
	पूर्णाङ्क i;
	गणना {
		GRAPH_WRITE_REACHABLE,
		GRAPH_WRITE_TIPS,
		GRAPH_WRITE_SKIP,
	} graph_write_mode = GRAPH_WRITE_REACHABLE;
	संरचना oidset updated_tips = OIDSET_INIT;

	संरचना option builtin_fetch_options[] = {
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
		OPT_SET_INT('n', रिक्त, &tags,
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
			.मान = &submodule_prefix,
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
	लागि (i = 1; i < argc; i++) {
		/* This handles non-URLs gracefully */
		वर्ण *anon = transport_anonymize_url(argv[i]);

		strbuf_addf(&default_rla, " %s", anon);
		मुक्त(anon);
	}

	repo_config(the_repository, git_fetch_config, &config);
	यदि (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_full_index = 0;
	}

	argc = parse_options(argc, argv, prefix,
			     builtin_fetch_options, builtin_fetch_usage, 0);

	यदि (recurse_submodules_cli != RECURSE_SUBMODULES_DEFAULT)
		config.recurse_submodules = recurse_submodules_cli;

	यदि (negotiate_only) {
		विकल्प (recurse_submodules_cli) {
		पक्ष RECURSE_SUBMODULES_OFF:
		पक्ष RECURSE_SUBMODULES_DEFAULT:
			/*
			 * --negotiate-only should never recurse into
			 * submodules. Skip it by setting recurse_submodules to
			 * RECURSE_SUBMODULES_OFF.
			 */
			config.recurse_submodules = RECURSE_SUBMODULES_OFF;
			रोक;

		स्वभाव:
			die(_("options '%s' and '%s' cannot be used together"),
			    "--negotiate-only", "--recurse-submodules");
		}
	}

	यदि (config.recurse_submodules != RECURSE_SUBMODULES_OFF) {
		पूर्णाङ्क *sfjc = config.submodule_fetch_jobs == -1
			    ? &config.submodule_fetch_jobs : रिक्त;
		पूर्णाङ्क *rs = config.recurse_submodules == RECURSE_SUBMODULES_DEFAULT
			  ? &config.recurse_submodules : रिक्त;

		fetch_config_from_gitmodules(the_repository, sfjc, rs);
	}


	यदि (porcelain) {
		विकल्प (recurse_submodules_cli) {
		पक्ष RECURSE_SUBMODULES_OFF:
		पक्ष RECURSE_SUBMODULES_DEFAULT:
			/*
			 * Reference updates in submodules would be ambiguous
			 * in porcelain mode, so we reject this combination.
			 */
			config.recurse_submodules = RECURSE_SUBMODULES_OFF;
			रोक;

		स्वभाव:
			die(_("options '%s' and '%s' cannot be used together"),
			    "--porcelain", "--recurse-submodules");
		}

		config.display_format = DISPLAY_FORMAT_PORCELAIN;
	}

	यदि (deepen_relative) {
		यदि (deepen_relative < 0)
			die(_("negative depth in --deepen is not supported"));
		यदि (depth)
			die(_("options '%s' and '%s' cannot be used together"), "--deepen", "--depth");
		depth = xstrfmt("%d", deepen_relative);
	}
	यदि (unshallow) {
		यदि (depth)
			die(_("options '%s' and '%s' cannot be used together"), "--depth", "--unshallow");
		अन्यथा यदि (!is_repository_shallow(the_repository))
			die(_("--unshallow on a complete repository does not make sense"));
		अन्यथा
			depth = xstrfmt("%d", INFINITE_DEPTH);
	}

	/* no need to be strict, transport_set_option() will validate it again */
	यदि (depth && म_से_प(depth) < 1)
		die(_("depth %s is not a positive number"), depth);
	यदि (depth || deepen_since || deepen_not.nr)
		deepen = 1;

	/* FETCH_HEAD never gets updated in --dry-run mode */
	यदि (dry_run)
		write_fetch_head = 0;

	यदि (!max_jobs)
		max_jobs = online_cpus();

	यदि (!repo_config_get_string_tmp(the_repository, "fetch.bundleuri", &bundle_uri) &&
	    fetch_bundle_uri(the_repository, bundle_uri, रिक्त))
		warning(_("failed to fetch bundles from '%s'"), bundle_uri);

	यदि (all < 0) {
		/*
		 * no --[no-]all given;
		 * only use config option if no remote was explicitly specified
		 */
		all = (!argc) ? config.all : 0;
	}

	यदि (all) {
		यदि (argc == 1)
			die(_("fetch --all does not take a repository argument"));
		अन्यथा यदि (argc > 1)
			die(_("fetch --all does not make sense with refspecs"));

		(शून्य) for_each_remote(get_one_remote_for_fetch, &list);

		/* do not do fetch_multiple() of one */
		यदि (list.nr == 1)
			remote = remote_get(list.वस्तुहरू[0].string);
	} अन्यथा यदि (argc == 0) {
		/* No arguments -- use default remote */
		remote = remote_get(रिक्त);
	} अन्यथा यदि (multiple) {
		/* All arguments are assumed to be remotes or groups */
		लागि (i = 0; i < argc; i++)
			यदि (!add_remote_or_group(argv[i], &list))
				die(_("no such remote or remote group: %s"),
				    argv[i]);
	} अन्यथा {
		/* Single remote or group */
		(शून्य) add_remote_or_group(argv[0], &list);
		यदि (list.nr > 1) {
			/* More than one remote */
			यदि (argc > 1)
				die(_("fetching a group and specifying refspecs does not make sense"));
		} अन्यथा {
			/* Zero or one remotes */
			remote = remote_get(argv[0]);
			prune_tags_ok = (argc == 1);
			argc--;
			argv++;
		}
	}
	string_list_remove_duplicates(&list, 0);

	यदि (negotiate_only) {
		संरचना oidset acked_commits = OIDSET_INIT;
		संरचना oidset_iter iter;
		अचर संरचना object_id *oid;

		trace2_region_enter("fetch", "negotiate-only", the_repository);
		यदि (!remote)
			die(_("must supply remote when using --negotiate-only"));
		gtransport = prepare_transport(remote, 1, &filter_options);

		यदि (!gtransport->smart_options) {
			warning(_("protocol does not support --negotiate-only, exiting"));
			result = 1;
			trace2_region_leave("fetch", "negotiate-only", the_repository);
			गच्छ cleanup;
		}
		यदि (!gtransport->smart_options->negotiation_restrict_tips)
			die(_("%s needs one or more %s"), "--negotiate-only",
			    "--negotiation-restrict=*");

		gtransport->smart_options->acked_commits = &acked_commits;

		यदि (server_options.nr)
			gtransport->server_options = &server_options;
		result = transport_fetch_refs(gtransport, रिक्त);
		gtransport->smart_options->acked_commits = रिक्त;

		oidset_iter_init(&acked_commits, &iter);
		जबसम्म ((oid = oidset_iter_next(&iter)))
			म_लिखो("%s\n", oid_to_hex(oid));
		oidset_clear(&acked_commits);
		trace2_region_leave("fetch", "negotiate-only", the_repository);
	} अन्यथा यदि (remote) {
		यदि (filter_options.choice || repo_has_promisor_remote(the_repository)) {
			trace2_region_enter("fetch", "setup-partial", the_repository);
			fetch_one_setup_partial(remote, &filter_options);
			trace2_region_leave("fetch", "setup-partial", the_repository);
		}
		trace2_region_enter("fetch", "fetch-one", the_repository);
		result = fetch_one(remote, argc, argv, prune_tags_ok, stdin_refspecs,
				   &config, &filter_options, &updated_tips);
		यदि (prepare_commit_graph(the_repository)) {
			यदि (oidset_size(&updated_tips))
				graph_write_mode = GRAPH_WRITE_TIPS;
			अन्यथा
				graph_write_mode = GRAPH_WRITE_SKIP;
		}
		trace2_region_leave("fetch", "fetch-one", the_repository);
	} अन्यथा {
		पूर्णाङ्क max_children = max_jobs;

		यदि (filter_options.choice)
			die(_("--filter can only be used with the remote "
			      "configured in extensions.partialclone"));

		यदि (atomic_fetch)
			die(_("--atomic can only be used when fetching "
			      "from one remote"));

		यदि (stdin_refspecs)
			die(_("--stdin can only be used when fetching "
			      "from one remote"));

		यदि (max_children < 0)
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
	यदि (!result && remote && (config.recurse_submodules != RECURSE_SUBMODULES_OFF)) {
		संरचना strvec options = STRVEC_INIT;
		पूर्णाङ्क max_children = max_jobs;

		यदि (max_children < 0)
			max_children = config.submodule_fetch_jobs;
		यदि (max_children < 0)
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
	यदि (negotiate_only)
		गच्छ cleanup;

	prepare_repo_settings(the_repository);
	यदि (fetch_write_commit_graph > 0 ||
	    (fetch_write_commit_graph < 0 &&
	     the_repository->settings.fetch_write_commit_graph)) {
		पूर्णाङ्क commit_graph_flags = COMMIT_GRAPH_WRITE_SPLIT;

		यदि (progress)
			commit_graph_flags |= COMMIT_GRAPH_WRITE_PROGRESS;

		यदि (graph_write_mode != GRAPH_WRITE_SKIP) {
			trace2_region_enter("fetch", "write-commit-graph",
					    the_repository);
			यदि (graph_write_mode == GRAPH_WRITE_TIPS)
				write_commit_graph(
					the_repository,
					the_repository->objects->source->path,
					रिक्त, &updated_tips,
					commit_graph_flags, रिक्त);
			अन्यथा
				write_commit_graph_reachable(
					the_repository,
					the_repository->objects->source->path,
					commit_graph_flags, रिक्त);
			trace2_region_leave("fetch", "write-commit-graph",
					    the_repository);
		}
	}

	यदि (enable_auto_gc) {
		यदि (refetch) {
			/*
			 * Hint auto-maintenance strongly to encourage repacking,
			 * but respect config settings disabling it.
			 */
			पूर्णाङ्क opt_val;
			यदि (repo_config_get_int(the_repository, "gc.autopacklimit", &opt_val))
				opt_val = -1;
			यदि (opt_val != 0)
				git_config_थप_parameter("gc.autoPackLimit=1");

			यदि (repo_config_get_int(the_repository, "maintenance.incremental-repack.auto", &opt_val))
				opt_val = -1;
			यदि (opt_val != 0)
				git_config_थप_parameter("maintenance.incremental-repack.auto=-1");
		}
		run_auto_maintenance(the_repository, verbosity < 0);
	}

 cleanup:
	oidset_clear(&updated_tips);
	string_list_clear(&list, 0);
	list_objects_filter_release(&filter_options);
	मुक्त(config.follow_remote_head_raw);
	फर्काऊ result;
}
