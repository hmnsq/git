/*
 * "git fetch"
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "advice.h"
#অন্তর্ভাব "config.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "environment.h"
#অন্তর্ভাব "hex.h"
#অন্তর্ভাব "refs.h"
#অন্তর্ভাব "refspec.h"
#অন্তর্ভাব "object-name.h"
#অন্তর্ভাব "odb.h"
#অন্তর্ভাব "odb/source.h"
#অন্তর্ভাব "oidset.h"
#অন্তর্ভাব "oid-array.h"
#অন্তর্ভাব "commit.h"
#অন্তর্ভাব "string-list.h"
#অন্তর্ভাব "remote.h"
#অন্তর্ভাব "transport.h"
#অন্তর্ভাব "run-command.h"
#অন্তর্ভাব "parse-options.h"
#অন্তর্ভাব "sigchain.h"
#অন্তর্ভাব "submodule-config.h"
#অন্তর্ভাব "submodule.h"
#অন্তর্ভাব "connected.h"
#অন্তর্ভাব "strvec.h"
#অন্তর্ভাব "utf8.h"
#অন্তর্ভাব "pager.h"
#অন্তর্ভাব "path.h"
#অন্তর্ভাব "pkt-line.h"
#অন্তর্ভাব "list-objects-filter-options.h"
#অন্তর্ভাব "commit-reach.h"
#অন্তর্ভাব "branch.h"
#অন্তর্ভাব "promisor-remote.h"
#অন্তর্ভাব "commit-graph.h"
#অন্তর্ভাব "shallow.h"
#অন্তর্ভাব "trace.h"
#অন্তর্ভাব "trace2.h"
#অন্তর্ভাব "bundle-uri.h"

#define FORCED_UPDATES_DELAY_WARNING_IN_MS (10 * 1000)

স্থির ধ্রুব বর্ণ * ধ্রুব builtin_fetch_usage[] = {
	N_("git fetch [<options>] [<repository> [<refspec>...]]"),
	N_("git fetch [<options>] <group>"),
	N_("git fetch --multiple [<options>] [(<repository>|<group>)...]"),
	N_("git fetch --all [<options>]"),
	রিক্ত
};

গণনা {
	TAGS_UNSET = 0,
	TAGS_DEFAULT = 1,
	TAGS_SET = 2
};

গণনা display_format {
	DISPLAY_FORMAT_FULL,
	DISPLAY_FORMAT_COMPACT,
	DISPLAY_FORMAT_PORCELAIN,
};

সংরচনা display_state {
	সংরচনা strbuf buf;

	পূর্ণাঙ্ক refcol_width;
	গণনা display_format format;

	বর্ণ *url;
	পূর্ণাঙ্ক url_len, shown_url;
};

স্থির uint64_t forced_updates_ms = 0;
স্থির পূর্ণাঙ্ক prefetch = 0;
স্থির পূর্ণাঙ্ক prune = -1; /* unspecified */
#define PRUNE_BY_DEFAULT 0 /* do we prune by default? */

স্থির পূর্ণাঙ্ক prune_tags = -1; /* unspecified */
#define PRUNE_TAGS_BY_DEFAULT 0 /* do we prune tags by default? */

স্থির পূর্ণাঙ্ক append, dry_run, force, keep, update_head_ok;
স্থির পূর্ণাঙ্ক write_fetch_head = 1;
স্থির পূর্ণাঙ্ক verbosity, deepen_relative, set_upstream, refetch;
স্থির পূর্ণাঙ্ক progress = -1;
স্থির পূর্ণাঙ্ক tags = TAGS_DEFAULT, update_shallow, deepen;
স্থির পূর্ণাঙ্ক atomic_fetch;
স্থির গণনা transport_family family;
স্থির ধ্রুব বর্ণ *depth;
স্থির ধ্রুব বর্ণ *deepen_since;
স্থির ধ্রুব বর্ণ *upload_pack;
স্থির সংরচনা string_list deepen_not = STRING_LIST_INIT_NODUP;
স্থির সংরচনা strbuf default_rla = STRBUF_INIT;
স্থির সংরচনা transport *gtransport;
স্থির সংরচনা transport *gsecondary;
স্থির সংরচনা refspec refmap;
স্থির সংরচনা string_list server_options = STRING_LIST_INIT_DUP;
স্থির সংরচনা string_list negotiation_restrict = STRING_LIST_INIT_NODUP;
স্থির সংরচনা string_list negotiation_include = STRING_LIST_INIT_NODUP;

সংরচনা fetch_config {
	গণনা display_format display_format;
	বর্ণ *follow_remote_head_raw;
	পূর্ণাঙ্ক follow_remote_head_seen;
	পূর্ণাঙ্ক all;
	পূর্ণাঙ্ক prune;
	পূর্ণাঙ্ক prune_tags;
	পূর্ণাঙ্ক show_forced_updates;
	পূর্ণাঙ্ক recurse_submodules;
	পূর্ণাঙ্ক parallel;
	পূর্ণাঙ্ক submodule_fetch_jobs;
};

স্থির পূর্ণাঙ্ক git_fetch_config(ধ্রুব বর্ণ *k, ধ্রুব বর্ণ *v,
			    ধ্রুব সংরচনা config_context *ctx, শূন্য *cb)
{
	সংরচনা fetch_config *fetch_config = cb;

	যদি (!strcmp(k, "fetch.all")) {
		fetch_config->all = git_config_bool(k, v);
		প্রত্যাগম 0;
	}

	যদি (!strcmp(k, "fetch.prune")) {
		fetch_config->prune = git_config_bool(k, v);
		প্রত্যাগম 0;
	}

	যদি (!strcmp(k, "fetch.prunetags")) {
		fetch_config->prune_tags = git_config_bool(k, v);
		প্রত্যাগম 0;
	}

	যদি (!strcmp(k, "fetch.showforcedupdates")) {
		fetch_config->show_forced_updates = git_config_bool(k, v);
		প্রত্যাগম 0;
	}

	যদি (!strcmp(k, "submodule.recurse")) {
		পূর্ণাঙ্ক r = git_config_bool(k, v) ?
			RECURSE_SUBMODULES_ON : RECURSE_SUBMODULES_OFF;
		fetch_config->recurse_submodules = r;
		প্রত্যাগম 0;
	}

	যদি (!strcmp(k, "submodule.fetchjobs")) {
		fetch_config->submodule_fetch_jobs = parse_submodule_fetchjobs(k, v, ctx->kvi);
		প্রত্যাগম 0;
	} অন্যথা যদি (!strcmp(k, "fetch.recursesubmodules")) {
		fetch_config->recurse_submodules = parse_fetch_recurse_submodules_arg(k, v);
		প্রত্যাগম 0;
	}

	যদি (!strcmp(k, "fetch.parallel")) {
		fetch_config->parallel = git_config_int(k, v, ctx->kvi);
		যদি (fetch_config->parallel < 0)
			die(_("fetch.parallel cannot be negative"));
		যদি (!fetch_config->parallel)
			fetch_config->parallel = online_cpus();
		প্রত্যাগম 0;
	}

	যদি (!strcmp(k, "fetch.output")) {
		যদি (!v)
			প্রত্যাগম config_error_nonbool(k);
		অন্যথা যদি (!strcasecmp(v, "full"))
			fetch_config->display_format = DISPLAY_FORMAT_FULL;
		অন্যথা যদি (!strcasecmp(v, "compact"))
			fetch_config->display_format = DISPLAY_FORMAT_COMPACT;
		অন্যথা
			die(_("invalid value for '%s': '%s'"),
			    "fetch.output", v);
		প্রত্যাগম 0;
	}

	যদি (!strcmp(k, "fetch.followremotehead")) {
		মুক্ত(fetch_config->follow_remote_head_raw);
		fetch_config->follow_remote_head_raw = xstrdup_or_null(v);
		fetch_config->follow_remote_head_seen = 1;
		প্রত্যাগম 0;
	}

	প্রত্যাগম git_default_config(k, v, ctx, cb);
}

স্থির গণনা follow_remote_head_settings get_follow_remote_head(ধ্রুব বর্ণ *setting)
{
	যদি (!setting)
		die(_("missing value for 'fetch.followRemoteHEAD'"));
	অন্যথা যদি (!strcmp(setting, "never"))
		প্রত্যাগম FOLLOW_REMOTE_NEVER;
	অন্যথা যদি (!strcmp(setting, "create"))
		প্রত্যাগম FOLLOW_REMOTE_CREATE;
	অন্যথা যদি (!strcmp(setting, "warn"))
		প্রত্যাগম FOLLOW_REMOTE_WARN;
	অন্যথা যদি (!strcmp(setting, "always"))
		প্রত্যাগম FOLLOW_REMOTE_ALWAYS;
	warning(_("unrecognized fetch.followRemoteHEAD value '%s' ignored"), setting);
	প্রত্যাগম BUILTIN_FOLLOW_REMOTE_HEAD_DFLT;
}

স্থির পূর্ণাঙ্ক parse_refmap_arg(ধ্রুব সংরচনা option *opt, ধ্রুব বর্ণ *arg, পূর্ণাঙ্ক unset)
{
	BUG_ON_OPT_NEG(unset);

	/*
	 * "git fetch --refmap='' origin foo"
	 * can be used to tell the command not to store anywhere
	 */
	refspec_append(opt->মান, arg);

	প্রত্যাগম 0;
}

স্থির শূন্য unlock_pack(unsigned পূর্ণাঙ্ক flags)
{
	যদি (gtransport)
		transport_unlock_pack(gtransport, flags);
	যদি (gsecondary)
		transport_unlock_pack(gsecondary, flags);
}

স্থির শূন্য unlock_pack_atexit(শূন্য)
{
	unlock_pack(0);
}

স্থির শূন্য unlock_pack_on_signal(পূর্ণাঙ্ক signo)
{
	unlock_pack(TRANSPORT_UNLOCK_PACK_IN_SIGNAL_HANDLER);
	sigchain_বের_করো(signo);
	উত্থাপয(signo);
}

স্থির শূন্য add_merge_config(সংরচনা ref **head,
			   ধ্রুব সংরচনা ref *remote_refs,
		           সংরচনা branch *branch,
		           সংরচনা ref ***tail)
{
	পূর্ণাঙ্ক i;

	প্রতি (i = 0; i < branch->merge_nr; i++) {
		সংরচনা ref *rm, **old_tail = *tail;
		সংরচনা refspec_item refspec;

		প্রতি (rm = *head; rm; rm = rm->next) {
			যদি (branch_merge_matches(branch, i, rm->name)) {
				rm->fetch_head_status = FETCH_HEAD_MERGE;
				ভঙ্গ;
			}
		}
		যদি (rm)
			অনুবর্ত;

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
		memset(&refspec, 0, পরিমাণ(refspec));
		refspec.src = branch->merge[i]->src;
		get_fetch_map(remote_refs, &refspec, tail, 1);
		প্রতি (rm = *old_tail; rm; rm = rm->next)
			rm->fetch_head_status = FETCH_HEAD_MERGE;
	}
}

স্থির শূন্য create_fetch_oidset(সংরচনা ref **head, সংরচনা oidset *out)
{
	সংরচনা ref *rm = *head;
	যাবত্ (rm) {
		oidset_ঢোকাও(out, &rm->old_oid);
		rm = rm->next;
	}
}

সংরচনা refname_hash_entry {
	সংরচনা hashmap_entry ent;
	সংরচনা object_id oid;
	পূর্ণাঙ্ক ignore;
	বর্ণ refname[FLEX_ARRAY];
};

স্থির পূর্ণাঙ্ক refname_hash_entry_cmp(ধ্রুব শূন্য *hashmap_cmp_fn_data UNUSED,
				  ধ্রুব সংরচনা hashmap_entry *eptr,
				  ধ্রুব সংরচনা hashmap_entry *entry_or_চাবি,
				  ধ্রুব শূন্য *keydata)
{
	ধ্রুব সংরচনা refname_hash_entry *e1, *e2;

	e1 = container_of(eptr, ধ্রুব সংরচনা refname_hash_entry, ent);
	e2 = container_of(entry_or_চাবি, ধ্রুব সংরচনা refname_hash_entry, ent);
	প্রত্যাগম strcmp(e1->refname, keydata ? keydata : e2->refname);
}

স্থির সংরচনা refname_hash_entry *refname_hash_add(সংরচনা hashmap *map,
						   ধ্রুব বর্ণ *refname,
						   ধ্রুব সংরচনা object_id *oid)
{
	সংরচনা refname_hash_entry *ent;
	size_t len = strlen(refname);

	FLEX_ALLOC_MEM(ent, refname, refname, len);
	hashmap_entry_init(&ent->ent, strhash(refname));
	oidcpy(&ent->oid, oid);
	hashmap_add(map, &ent->ent);
	প্রত্যাগম ent;
}

স্থির পূর্ণাঙ্ক add_one_refname(ধ্রুব সংরচনা reference *ref, শূন্য *cbdata)
{
	সংরচনা hashmap *refname_map = cbdata;

	(শূন্য) refname_hash_add(refname_map, ref->name, ref->oid);
	প্রত্যাগম 0;
}

স্থির শূন্য refname_hash_init(সংরচনা hashmap *map)
{
	hashmap_init(map, refname_hash_entry_cmp, রিক্ত, 0);
}

স্থির পূর্ণাঙ্ক refname_hash_exists(সংরচনা hashmap *map, ধ্রুব বর্ণ *refname)
{
	প্রত্যাগম !!hashmap_get_from_hash(map, strhash(refname), refname);
}

স্থির শূন্য clear_item(সংরচনা refname_hash_entry *item)
{
	item->ignore = 1;
}


স্থির শূন্য add_already_queued_tags(ধ্রুব বর্ণ *refname,
				    ধ্রুব সংরচনা object_id *old_oid UNUSED,
				    ধ্রুব সংরচনা object_id *new_oid,
				    শূন্য *cb_data)
{
	সংরচনা hashmap *queued_tags = cb_data;
	যদি (starts_with(refname, "refs/tags/") && new_oid)
		(শূন্য) refname_hash_add(queued_tags, refname, new_oid);
}

স্থির শূন্য find_non_local_tags(ধ্রুব সংরচনা ref *refs,
				সংরচনা ref_transaction *transaction,
				সংরচনা ref **head,
				সংরচনা ref ***tail)
{
	সংরচনা hashmap existing_refs;
	সংরচনা hashmap remote_refs;
	সংরচনা oidset fetch_oids = OIDSET_INIT;
	সংরচনা string_list remote_refs_list = STRING_LIST_INIT_NODUP;
	সংরচনা string_list_item *remote_ref_item;
	ধ্রুব সংরচনা ref *ref;
	সংরচনা refname_hash_entry *item = রিক্ত;

	refname_hash_init(&existing_refs);
	refname_hash_init(&remote_refs);
	create_fetch_oidset(head, &fetch_oids);

	refs_for_each_ref(get_main_ref_store(the_repository), add_one_refname,
			  &existing_refs);

	/*
	 * If we already have a transaction, then we need to filter out all
	 * tags which have already been queued up.
	 */
	যদি (transaction)
		ref_transaction_for_each_queued_update(transaction,
						       add_already_queued_tags,
						       &existing_refs);

	প্রতি (ref = refs; ref; ref = ref->next) {
		যদি (!starts_with(ref->name, "refs/tags/"))
			অনুবর্ত;

		/*
		 * The peeled ref always follows the matching base
		 * ref, so if we see a peeled ref that we don't want
		 * to fetch then we can mark the ref entry in the list
		 * as one to ignore by setting util to NULL.
		 */
		যদি (ends_with(ref->name, "^{}")) {
			যদি (item &&
			    !odb_has_object(the_repository->objects, &ref->old_oid, 0) &&
			    !oidset_contains(&fetch_oids, &ref->old_oid) &&
			    !odb_has_object(the_repository->objects, &item->oid, 0) &&
			    !oidset_contains(&fetch_oids, &item->oid))
				clear_item(item);
			item = রিক্ত;
			অনুবর্ত;
		}

		/*
		 * If item is non-NULL here, then we previously saw a
		 * ref not followed by a peeled reference, so we need
		 * to check if it is a lightweight tag that we want to
		 * fetch.
		 */
		যদি (item &&
		    !odb_has_object(the_repository->objects, &item->oid, 0) &&
		    !oidset_contains(&fetch_oids, &item->oid))
			clear_item(item);

		item = রিক্ত;

		/* skip duplicates and refs that we already have */
		যদি (refname_hash_exists(&remote_refs, ref->name) ||
		    refname_hash_exists(&existing_refs, ref->name))
			অনুবর্ত;

		item = refname_hash_add(&remote_refs, ref->name, &ref->old_oid);
		string_list_ঢোকাও(&remote_refs_list, ref->name);
	}
	hashmap_clear_and_free(&existing_refs, সংরচনা refname_hash_entry, ent);

	/*
	 * We may have a final lightweight tag that needs to be
	 * checked to see if it needs fetching.
	 */
	যদি (item &&
	    !odb_has_object(the_repository->objects, &item->oid, 0) &&
	    !oidset_contains(&fetch_oids, &item->oid))
		clear_item(item);

	/*
	 * For all the tags in the remote_refs_list,
	 * add them to the list of refs to be fetched
	 */
	for_each_string_list_item(remote_ref_item, &remote_refs_list) {
		ধ্রুব বর্ণ *refname = remote_ref_item->string;
		সংরচনা ref *rm;
		unsigned পূর্ণাঙ্ক hash = strhash(refname);

		item = hashmap_get_entry_from_hash(&remote_refs, hash, refname,
					সংরচনা refname_hash_entry, ent);
		যদি (!item)
			BUG("unseen remote ref?");

		/* Unless we have already decided to ignore this item... */
		যদি (item->ignore)
			অনুবর্ত;

		rm = alloc_ref(item->refname);
		rm->peer_ref = alloc_ref(item->refname);
		oidcpy(&rm->old_oid, &item->oid);
		**tail = rm;
		*tail = &rm->next;
	}
	hashmap_clear_and_free(&remote_refs, সংরচনা refname_hash_entry, ent);
	string_list_clear(&remote_refs_list, 0);
	oidset_clear(&fetch_oids);
}

স্থির শূন্য filter_prefetch_refspec(সংরচনা refspec *rs)
{
	পূর্ণাঙ্ক i;

	যদি (!prefetch)
		প্রত্যাগম;

	প্রতি (i = 0; i < rs->nr; i++) {
		সংরচনা strbuf new_dst = STRBUF_INIT;
		বর্ণ *old_dst;
		ধ্রুব বর্ণ *sub = রিক্ত;

		যদি (rs->বস্তুসমূহ[i].negative)
			অনুবর্ত;
		যদি (!rs->বস্তুসমূহ[i].dst ||
		    (rs->বস্তুসমূহ[i].src &&
		     starts_with(rs->বস্তুসমূহ[i].src,
				 ref_namespace[NAMESPACE_TAGS].ref))) {
			পূর্ণাঙ্ক j;

			refspec_item_clear(&rs->বস্তুসমূহ[i]);

			প্রতি (j = i + 1; j < rs->nr; j++)
				rs->বস্তুসমূহ[j - 1] = rs->বস্তুসমূহ[j];
			rs->nr--;
			i--;
			অনুবর্ত;
		}

		old_dst = rs->বস্তুসমূহ[i].dst;
		strbuf_addstr(&new_dst, ref_namespace[NAMESPACE_PREFETCH].ref);

		/*
		 * If old_dst starts with "refs/", then place
		 * sub after that prefix. Otherwise, start at
		 * the beginning of the string.
		 */
		যদি (!skip_prefix(old_dst, "refs/", &sub))
			sub = old_dst;
		strbuf_addstr(&new_dst, sub);

		rs->বস্তুসমূহ[i].dst = strbuf_detach(&new_dst, রিক্ত);
		rs->বস্তুসমূহ[i].force = 1;

		মুক্ত(old_dst);
	}
}

স্থির সংরচনা ref *get_ref_map(সংরচনা remote *remote,
			       ধ্রুব সংরচনা ref *remote_refs,
			       সংরচনা refspec *rs,
			       পূর্ণাঙ্ক tags, পূর্ণাঙ্ক *autotags)
{
	পূর্ণাঙ্ক i;
	সংরচনা ref *rm;
	সংরচনা ref *ref_map = রিক্ত;
	সংরচনা ref **tail = &ref_map;

	/* opportunistically-updated references: */
	সংরচনা ref *orefs = রিক্ত, **oref_tail = &orefs;

	সংরচনা hashmap existing_refs;
	পূর্ণাঙ্ক existing_refs_populated = 0;

	filter_prefetch_refspec(rs);
	যদি (remote)
		filter_prefetch_refspec(&remote->fetch);

	যদি (rs->nr) {
		সংরচনা refspec *fetch_refspec;

		প্রতি (i = 0; i < rs->nr; i++) {
			get_fetch_map(remote_refs, &rs->বস্তুসমূহ[i], &tail, 0);
			যদি (rs->বস্তুসমূহ[i].dst && rs->বস্তুসমূহ[i].dst[0])
				*autotags = 1;
		}
		/* Merge everything on the command line (but not --tags) */
		প্রতি (rm = ref_map; rm; rm = rm->next)
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
		যদি (refmap.nr)
			fetch_refspec = &refmap;
		অন্যথা
			fetch_refspec = &remote->fetch;

		প্রতি (i = 0; i < fetch_refspec->nr; i++)
			get_fetch_map(ref_map, &fetch_refspec->বস্তুসমূহ[i], &oref_tail, 1);
	} অন্যথা যদি (refmap.nr) {
		die("--refmap option is only meaningful with command-line refspec(s)");
	} অন্যথা {
		/* Use the defaults */
		সংরচনা branch *branch = branch_get(রিক্ত);
		পূর্ণাঙ্ক has_merge = branch_has_merge_config(branch);
		যদি (remote &&
		    (remote->fetch.nr ||
		     /* Note: has_merge implies non-NULL branch->remote_name */
		     (has_merge && !strcmp(branch->remote_name, remote->name)))) {
			প্রতি (i = 0; i < remote->fetch.nr; i++) {
				get_fetch_map(remote_refs, &remote->fetch.বস্তুসমূহ[i], &tail, 0);
				যদি (remote->fetch.বস্তুসমূহ[i].dst &&
				    remote->fetch.বস্তুসমূহ[i].dst[0])
					*autotags = 1;
				যদি (!i && !has_merge && ref_map &&
				    !remote->fetch.বস্তুসমূহ[0].pattern)
					ref_map->fetch_head_status = FETCH_HEAD_MERGE;
			}
			/*
			 * if the remote we're fetching from is the same
			 * as given in branch.<name>.remote, we add the
			 * ref given in branch.<name>.merge, too.
			 *
			 * Note: has_merge implies non-NULL branch->remote_name
			 */
			যদি (has_merge &&
			    !strcmp(branch->remote_name, remote->name))
				add_merge_config(&ref_map, remote_refs, branch, &tail);
		} অন্যথা যদি (!prefetch) {
			ref_map = get_remote_ref(remote_refs, "HEAD");
			যদি (!ref_map)
				die(_("couldn't find remote ref HEAD"));
			ref_map->fetch_head_status = FETCH_HEAD_MERGE;
			tail = &ref_map->next;
		}
	}

	যদি (tags == TAGS_SET) {
		সংরচনা refspec_item tag_refspec;

		/* also fetch all tags */
		refspec_item_init_ঠেলো(&tag_refspec, TAG_REFSPEC,
				       the_hash_algo);
		get_fetch_map(remote_refs, &tag_refspec, &tail, 0);
		refspec_item_clear(&tag_refspec);
	} অন্যথা যদি (tags == TAGS_DEFAULT && *autotags) {
		find_non_local_tags(remote_refs, রিক্ত, &ref_map, &tail);
	}

	/* Now append any refs to be updated opportunistically: */
	*tail = orefs;
	প্রতি (rm = orefs; rm; rm = rm->next) {
		rm->fetch_head_status = FETCH_HEAD_IGNORE;
		tail = &rm->next;
	}

	/*
	 * apply negative refspecs first, before we remove duplicates. This is
	 * necessary as negative refspecs might remove an otherwise conflicting
	 * duplicate.
	 */
	যদি (rs->nr)
		ref_map = apply_negative_refspecs(ref_map, rs);
	অন্যথা
		ref_map = apply_negative_refspecs(ref_map, &remote->fetch);

	ref_map = ref_remove_duplicates(ref_map);

	প্রতি (rm = ref_map; rm; rm = rm->next) {
		যদি (rm->peer_ref) {
			ধ্রুব বর্ণ *refname = rm->peer_ref->name;
			সংরচনা refname_hash_entry *peer_item;
			unsigned পূর্ণাঙ্ক hash = strhash(refname);

			যদি (!existing_refs_populated) {
				refname_hash_init(&existing_refs);
				refs_for_each_ref(get_main_ref_store(the_repository),
						  add_one_refname,
						  &existing_refs);
				existing_refs_populated = 1;
			}

			peer_item = hashmap_get_entry_from_hash(&existing_refs,
						hash, refname,
						সংরচনা refname_hash_entry, ent);
			যদি (peer_item) {
				সংরচনা object_id *old_oid = &peer_item->oid;
				oidcpy(&rm->peer_ref->old_oid, old_oid);
			}
		}
	}
	যদি (existing_refs_populated)
		hashmap_clear_and_free(&existing_refs, সংরচনা refname_hash_entry, ent);

	প্রত্যাগম ref_map;
}

স্থির পূর্ণাঙ্ক s_update_ref(ধ্রুব বর্ণ *action,
			সংরচনা ref *ref,
			সংরচনা ref_transaction *transaction,
			পূর্ণাঙ্ক check_old)
{
	বর্ণ *বার্তা;
	বর্ণ *rla = getenv("GIT_REFLOG_ACTION");
	সংরচনা strbuf err = STRBUF_INIT;
	পূর্ণাঙ্ক ret;

	যদি (dry_run)
		প্রত্যাগম 0;
	যদি (!rla)
		rla = default_rla.buf;
	বার্তা = xstrfmt("%s: %s", rla, action);

	ret = ref_transaction_update(transaction, ref->name, &ref->new_oid,
				     check_old ? &ref->old_oid : রিক্ত,
				     রিক্ত, রিক্ত, 0, বার্তা, &err);

	যদি (ret)
		error("%s", err.buf);
	strbuf_release(&err);
	মুক্ত(বার্তা);
	প্রত্যাগম ret;
}

স্থির পূর্ণাঙ্ক refcol_width(ধ্রুব সংরচনা ref *ref_map, পূর্ণাঙ্ক compact_format)
{
	ধ্রুব সংরচনা ref *ref;
	পূর্ণাঙ্ক max, width = 10;

	max = term_columns();
	যদি (compact_format)
		max = max * 2 / 3;

	প্রতি (ref = ref_map; ref; ref = ref->next) {
		পূর্ণাঙ্ক rlen, llen = 0, len;

		যদি (ref->status == REF_STATUS_REJECT_SHALLOW ||
		    !ref->peer_ref ||
		    !strcmp(ref->name, "HEAD"))
			অনুবর্ত;

		/* uptodate lines are only shown on high verbosity level */
		যদি (verbosity <= 0 && oideq(&ref->peer_ref->old_oid, &ref->old_oid))
			অনুবর্ত;

		rlen = utf8_strwidth(prettify_refname(ref->name));
		যদি (!compact_format)
			llen = utf8_strwidth(prettify_refname(ref->peer_ref->name));

		/*
		 * rough estimation to see if the output line is too long and
		 * should not be counted (we can't do precise calculation
		 * anyway because we don't know if the error explanation part
		 * will be printed in update_local_ref)
		 */
		len = 21 /* flag and summary */ + rlen + 4 /* -> */ + llen;
		যদি (len >= max)
			অনুবর্ত;

		যদি (width < rlen)
			width = rlen;
	}

	প্রত্যাগম width;
}

স্থির শূন্য display_state_init(সংরচনা display_state *display_state, সংরচনা ref *ref_map,
			       ধ্রুব বর্ণ *raw_url, গণনা display_format format)
{
	পূর্ণাঙ্ক i;

	memset(display_state, 0, পরিমাণ(*display_state));
	strbuf_init(&display_state->buf, 0);
	display_state->format = format;

	যদি (raw_url)
		display_state->url = transport_anonymize_url(raw_url);
	অন্যথা
		display_state->url = xstrdup("foreign");

	display_state->url_len = strlen(display_state->url);
	প্রতি (i = display_state->url_len - 1; 0 <= i && display_state->url[i] == '/'; i--)
		;
	display_state->url_len = i + 1;
	যদি (4 < i && !strncmp(".git", display_state->url + i - 3, 4))
		display_state->url_len = i - 3;

	যদি (verbosity < 0)
		প্রত্যাগম;

	বিকল্প (display_state->format) {
	পক্ষ DISPLAY_FORMAT_FULL:
	পক্ষ DISPLAY_FORMAT_COMPACT:
		display_state->refcol_width = refcol_width(ref_map,
							   display_state->format == DISPLAY_FORMAT_COMPACT);
		ভঙ্গ;
	পক্ষ DISPLAY_FORMAT_PORCELAIN:
		/* We don't need to precompute anything here. */
		ভঙ্গ;
	স্বভাব:
		BUG("unexpected display format %d", display_state->format);
	}
}

স্থির শূন্য display_state_release(সংরচনা display_state *display_state)
{
	strbuf_release(&display_state->buf);
	মুক্ত(display_state->url);
}

স্থির শূন্য print_remote_to_local(সংরচনা display_state *display_state,
				  ধ্রুব বর্ণ *remote, ধ্রুব বর্ণ *local)
{
	strbuf_addf(&display_state->buf, "%-*s -> %s",
		    display_state->refcol_width, remote, local);
}

স্থির পূর্ণাঙ্ক find_and_replace(সংরচনা strbuf *haystack,
			    ধ্রুব বর্ণ *needle,
			    ধ্রুব বর্ণ *placeholder)
{
	ধ্রুব বর্ণ *p = রিক্ত;
	পূর্ণাঙ্ক plen, nlen;

	nlen = strlen(needle);
	যদি (ends_with(haystack->buf, needle))
		p = haystack->buf + haystack->len - nlen;
	অন্যথা
		p = strstr(haystack->buf, needle);
	যদি (!p)
		প্রত্যাগম 0;

	যদি (p > haystack->buf && p[-1] != '/')
		প্রত্যাগম 0;

	plen = strlen(p);
	যদি (plen > nlen && p[nlen] != '/')
		প্রত্যাগম 0;

	strbuf_splice(haystack, p - haystack->buf, nlen,
		      placeholder, strlen(placeholder));
	প্রত্যাগম 1;
}

স্থির শূন্য print_compact(সংরচনা display_state *display_state,
			  ধ্রুব বর্ণ *remote, ধ্রুব বর্ণ *local)
{
	সংরচনা strbuf r = STRBUF_INIT;
	সংরচনা strbuf l = STRBUF_INIT;

	যদি (!strcmp(remote, local)) {
		strbuf_addf(&display_state->buf, "%-*s -> *", display_state->refcol_width, remote);
		প্রত্যাগম;
	}

	strbuf_addstr(&r, remote);
	strbuf_addstr(&l, local);

	যদি (!find_and_replace(&r, local, "*"))
		find_and_replace(&l, remote, "*");
	print_remote_to_local(display_state, r.buf, l.buf);

	strbuf_release(&r);
	strbuf_release(&l);
}

স্থির শূন্য display_ref_update(সংরচনা display_state *display_state, বর্ণ code,
			       ধ্রুব বর্ণ *summary, ধ্রুব বর্ণ *error,
			       ধ্রুব বর্ণ *remote, ধ্রুব বর্ণ *local,
			       ধ্রুব সংরচনা object_id *old_oid,
			       ধ্রুব সংরচনা object_id *new_oid,
			       পূর্ণাঙ্ক summary_width)
{
	FILE *f = stderr;

	যদি (verbosity < 0)
		প্রত্যাগম;

	strbuf_reset(&display_state->buf);

	বিকল্প (display_state->format) {
	পক্ষ DISPLAY_FORMAT_FULL:
	পক্ষ DISPLAY_FORMAT_COMPACT: {
		পূর্ণাঙ্ক width;

		যদি (!display_state->shown_url) {
			strbuf_addf(&display_state->buf, _("From %.*s\n"),
				    display_state->url_len, display_state->url);
			display_state->shown_url = 1;
		}

		width = (summary_width + strlen(summary) - gettext_width(summary));
		remote = prettify_refname(remote);
		local = prettify_refname(local);

		strbuf_addf(&display_state->buf, " %c %-*s ", code, width, summary);

		যদি (display_state->format != DISPLAY_FORMAT_COMPACT)
			print_remote_to_local(display_state, remote, local);
		অন্যথা
			print_compact(display_state, remote, local);

		যদি (error)
			strbuf_addf(&display_state->buf, "  (%s)", error);

		ভঙ্গ;
	}
	পক্ষ DISPLAY_FORMAT_PORCELAIN:
		strbuf_addf(&display_state->buf, "%c %s %s %s", code,
			    oid_to_hex(old_oid), oid_to_hex(new_oid), local);
		f = stdout;
		ভঙ্গ;
	স্বভাব:
		BUG("unexpected display format %d", display_state->format);
	};
	strbuf_addch(&display_state->buf, '\n');

	fputs(display_state->buf.buf, f);
}

সংরচনা ref_update_display_info {
	bool failed;
	বর্ণ success_code;
	বর্ণ fail_code;
	বর্ণ *summary;
	বর্ণ *fail_detail;
	বর্ণ *success_detail;
	বর্ণ *ref;
	বর্ণ *remote;
	সংরচনা object_id old_oid;
	সংরচনা object_id new_oid;
};

সংরচনা ref_update_display_info_array {
	সংরচনা ref_update_display_info *info;
	size_t alloc, nr;
};

স্থির সংরচনা ref_update_display_info *ref_update_display_info_append(
					   সংরচনা ref_update_display_info_array *array,
					   বর্ণ success_code,
					   বর্ণ fail_code,
					   ধ্রুব বর্ণ *summary,
					   ধ্রুব বর্ণ *success_detail,
					   ধ্রুব বর্ণ *fail_detail,
					   ধ্রুব বর্ণ *ref,
					   ধ্রুব বর্ণ *remote,
					   ধ্রুব সংরচনা object_id *old_oid,
					   ধ্রুব সংরচনা object_id *new_oid)
{
	সংরচনা ref_update_display_info *info;

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

	প্রত্যাগম info;
}

স্থির শূন্য ref_update_display_info_set_failed(সংরচনা ref_update_display_info *info)
{
	info->failed = true;
}

স্থির শূন্য ref_update_display_info_free(সংরচনা ref_update_display_info *info)
{
	মুক্ত(info->summary);
	মুক্ত(info->success_detail);
	মুক্ত(info->fail_detail);
	মুক্ত(info->remote);
	মুক্ত(info->ref);
}

স্থির শূন্য ref_update_display_info_display(সংরচনা ref_update_display_info *info,
					    সংরচনা display_state *display_state,
					    পূর্ণাঙ্ক summary_width)
{
	display_ref_update(display_state,
			   info->failed ? info->fail_code : info->success_code,
			   info->summary,
			   info->failed ? info->fail_detail : info->success_detail,
			   info->remote, info->ref, &info->old_oid,
			   &info->new_oid, summary_width);
}

স্থির পূর্ণাঙ্ক update_local_ref(সংরচনা ref *ref,
			    সংরচনা ref_transaction *transaction,
			    ধ্রুব সংরচনা ref *remote_ref,
			    ধ্রুব সংরচনা fetch_config *config,
			    সংরচনা ref_update_display_info_array *display_array)
{
	সংরচনা commit *current = রিক্ত, *updated;
	পূর্ণাঙ্ক fast_forward = 0;

	যদি (!odb_has_object(the_repository->objects, &ref->new_oid,
			    ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR))
		die(_("object %s not found"), oid_to_hex(&ref->new_oid));

	যদি (oideq(&ref->old_oid, &ref->new_oid)) {
		যদি (verbosity > 0)
			ref_update_display_info_append(display_array, '=', '=',
						       _("[up to date]"), রিক্ত,
						       রিক্ত, ref->name,
						       remote_ref->name, &ref->old_oid,
						       &ref->new_oid);
		প্রত্যাগম 0;
	}

	যদি (!update_head_ok &&
	    !is_null_oid(&ref->old_oid) &&
	    branch_checked_out(ref->name)) {
		সংরচনা ref_update_display_info *info;
		/*
		 * If this is the head, and it's not okay to update
		 * the head, and the old value of the head isn't empty...
		 */
		info = ref_update_display_info_append(display_array, '!', '!',
						      _("[rejected]"), রিক্ত,
						      _("can't fetch into checked-out branch"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);
		ref_update_display_info_set_failed(info);
		প্রত্যাগম 1;
	}

	যদি (!is_null_oid(&ref->old_oid) &&
	    starts_with(ref->name, "refs/tags/")) {
		সংরচনা ref_update_display_info *info;

		যদি (force || ref->force) {
			পূর্ণাঙ্ক r;

			r = s_update_ref("updating tag", ref, transaction, 0);

			info = ref_update_display_info_append(display_array, 't', '!',
							      _("[tag update]"), রিক্ত,
							      _("unable to update local ref"),
							      ref->name, remote_ref->name,
							      &ref->old_oid, &ref->new_oid);
			যদি (r)
				ref_update_display_info_set_failed(info);

			প্রত্যাগম r;
		} অন্যথা {
			info = ref_update_display_info_append(display_array, '!', '!',
							      _("[rejected]"), রিক্ত,
							      _("would clobber existing tag"),
							      ref->name, remote_ref->name,
							      &ref->old_oid, &ref->new_oid);
			ref_update_display_info_set_failed(info);
			প্রত্যাগম 1;
		}
	}

	current = lookup_commit_reference_gently(the_repository,
						 &ref->old_oid, 1);
	updated = lookup_commit_reference_gently(the_repository,
						 &ref->new_oid, 1);
	যদি (!current || !updated) {
		সংরচনা ref_update_display_info *info;
		ধ্রুব বর্ণ *বার্তা;
		ধ্রুব বর্ণ *what;
		পূর্ণাঙ্ক r;
		/*
		 * Nicely describe the new ref we're fetching.
		 * Base this on the remote's ref name, as it's
		 * more likely to follow a standard layout.
		 */
		যদি (starts_with(remote_ref->name, "refs/tags/")) {
			বার্তা = "storing tag";
			what = _("[new tag]");
		} অন্যথা যদি (starts_with(remote_ref->name, "refs/heads/")) {
			বার্তা = "storing head";
			what = _("[new branch]");
		} অন্যথা {
			বার্তা = "storing ref";
			what = _("[new ref]");
		}

		r = s_update_ref(বার্তা, ref, transaction, 0);

		info = ref_update_display_info_append(display_array, '*', '!',
						      what, রিক্ত,
						      _("unable to update local ref"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);
		যদি (r)
			ref_update_display_info_set_failed(info);

		প্রত্যাগম r;
	}

	যদি (config->show_forced_updates) {
		uint64_t t_before = getnanotime();
		fast_forward = repo_in_merge_bases(the_repository, current,
						   updated);
		যদি (fast_forward < 0)
			die(রিক্ত);
		forced_updates_ms += (getnanotime() - t_before) / 1000000;
	} অন্যথা {
		fast_forward = 1;
	}

	যদি (fast_forward) {
		সংরচনা ref_update_display_info *info;
		সংরচনা strbuf quickref = STRBUF_INIT;
		পূর্ণাঙ্ক r;

		strbuf_add_unique_abbrev(&quickref, &current->object.oid, DEFAULT_ABBREV);
		strbuf_addstr(&quickref, "..");
		strbuf_add_unique_abbrev(&quickref, &ref->new_oid, DEFAULT_ABBREV);
		r = s_update_ref("fast-forward", ref, transaction, 1);

		info = ref_update_display_info_append(display_array, ' ', '!',
						      quickref.buf, রিক্ত,
						      _("unable to update local ref"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);
		যদি (r)
			ref_update_display_info_set_failed(info);

		strbuf_release(&quickref);
		প্রত্যাগম r;
	} অন্যথা যদি (force || ref->force) {
		সংরচনা ref_update_display_info *info;
		সংরচনা strbuf quickref = STRBUF_INIT;
		পূর্ণাঙ্ক r;

		strbuf_add_unique_abbrev(&quickref, &current->object.oid, DEFAULT_ABBREV);
		strbuf_addstr(&quickref, "...");
		strbuf_add_unique_abbrev(&quickref, &ref->new_oid, DEFAULT_ABBREV);
		r = s_update_ref("forced-update", ref, transaction, 1);

		info = ref_update_display_info_append(display_array, '+', '!',
						      quickref.buf, _("forced update"),
						      _("unable to update local ref"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);

		যদি (r)
			ref_update_display_info_set_failed(info);

		strbuf_release(&quickref);
		প্রত্যাগম r;
	} অন্যথা {
		সংরচনা ref_update_display_info *info;
		info = ref_update_display_info_append(display_array, '!', '!',
						      _("[rejected]"), রিক্ত,
						      _("non-fast-forward"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);
		ref_update_display_info_set_failed(info);
		প্রত্যাগম 1;
	}
}

স্থির ধ্রুব সংরচনা object_id *iterate_ref_map(শূন্য *cb_data)
{
	সংরচনা ref **rm = cb_data;
	সংরচনা ref *ref = *rm;

	যাবত্ (ref && ref->status == REF_STATUS_REJECT_SHALLOW)
		ref = ref->next;
	যদি (!ref)
		প্রত্যাগম রিক্ত;
	*rm = ref->next;
	প্রত্যাগম &ref->old_oid;
}

সংরচনা fetch_head {
	FILE *fp;
	সংরচনা strbuf buf;
};

স্থির পূর্ণাঙ্ক open_fetch_head(সংরচনা fetch_head *fetch_head)
{
	ধ্রুব বর্ণ *filename = git_path_fetch_head(the_repository);

	যদি (write_fetch_head) {
		fetch_head->fp = fopen(filename, "a");
		যদি (!fetch_head->fp)
			প্রত্যাগম error_errno(_("cannot open '%s'"), filename);
		strbuf_init(&fetch_head->buf, 0);
	} অন্যথা {
		fetch_head->fp = রিক্ত;
	}

	প্রত্যাগম 0;
}

স্থির শূন্য append_fetch_head(সংরচনা fetch_head *fetch_head,
			      ধ্রুব সংরচনা object_id *old_oid,
			      গণনা fetch_head_status fetch_head_status,
			      ধ্রুব বর্ণ *note,
			      ধ্রুব বর্ণ *url, size_t url_len)
{
	বর্ণ old_oid_hex[GIT_MAX_HEXSZ + 1];
	ধ্রুব বর্ণ *merge_status_marker;
	size_t i;

	যদি (!fetch_head->fp)
		প্রত্যাগম;

	বিকল্প (fetch_head_status) {
	পক্ষ FETCH_HEAD_NOT_FOR_MERGE:
		merge_status_marker = "not-for-merge";
		ভঙ্গ;
	পক্ষ FETCH_HEAD_MERGE:
		merge_status_marker = "";
		ভঙ্গ;
	স্বভাব:
		/* do not write anything to FETCH_HEAD */
		প্রত্যাগম;
	}

	strbuf_addf(&fetch_head->buf, "%s\t%s\t%s",
		    oid_to_hex_r(old_oid_hex, old_oid), merge_status_marker, note);
	প্রতি (i = 0; i < url_len; ++i)
		যদি ('\n' == url[i])
			strbuf_addstr(&fetch_head->buf, "\\n");
		অন্যথা
			strbuf_addch(&fetch_head->buf, url[i]);
	strbuf_addch(&fetch_head->buf, '\n');

	/*
	 * When using an atomic fetch, we do not want to update FETCH_HEAD if
	 * any of the reference updates fails. We thus have to write all
	 * updates to a buffer first and only commit it as soon as all
	 * references have been successfully updated.
	 */
	যদি (!atomic_fetch) {
		strbuf_write(&fetch_head->buf, fetch_head->fp);
		strbuf_reset(&fetch_head->buf);
	}
}

স্থির শূন্য commit_fetch_head(সংরচনা fetch_head *fetch_head)
{
	যদি (!fetch_head->fp || !atomic_fetch)
		প্রত্যাগম;
	strbuf_write(&fetch_head->buf, fetch_head->fp);
}

স্থির শূন্য close_fetch_head(সংরচনা fetch_head *fetch_head)
{
	যদি (!fetch_head->fp)
		প্রত্যাগম;

	fclose(fetch_head->fp);
	strbuf_release(&fetch_head->buf);
}

স্থির ধ্রুব বর্ণ warn_show_forced_updates[] =
N_("fetch normally indicates which branches had a forced update,\n"
   "but that check has been disabled; to re-enable, use '--show-forced-updates'\n"
   "flag or run 'git config fetch.showForcedUpdates true'");
স্থির ধ্রুব বর্ণ warn_time_show_forced_updates[] =
N_("it took %.2f seconds to check forced updates; you can use\n"
   "'--no-show-forced-updates' or run 'git config fetch.showForcedUpdates false'\n"
   "to avoid this check\n");

স্থির পূর্ণাঙ্ক store_updated_refs(সংরচনা display_state *display_state,
			      পূর্ণাঙ্ক connectivity_checked,
			      সংরচনা ref_transaction *transaction, সংরচনা ref *ref_map,
			      সংরচনা fetch_head *fetch_head,
			      ধ্রুব সংরচনা fetch_config *config,
			      সংরচনা ref_update_display_info_array *display_array)
{
	পূর্ণাঙ্ক rc = 0;
	সংরচনা strbuf note = STRBUF_INIT;
	ধ্রুব বর্ণ *what, *kind;
	সংরচনা ref *rm;
	পূর্ণাঙ্ক want_status;

	যদি (!connectivity_checked) {
		সংরচনা check_connected_options opt = CHECK_CONNECTED_INIT;

		opt.exclude_hidden_refs_section = "fetch";
		rm = ref_map;
		যদি (check_connected(iterate_ref_map, &rm, &opt)) {
			rc = error(_("%s did not send all necessary objects"),
				   display_state->url);
			গচ্ছ abort;
		}
	}

	/*
	 * We do a pass for each fetch_head_status type in their enum order, so
	 * merged entries are written before not-for-merge. That lets readers
	 * use FETCH_HEAD as a refname to refer to the ref to be merged.
	 */
	প্রতি (want_status = FETCH_HEAD_MERGE;
	     want_status <= FETCH_HEAD_IGNORE;
	     want_status++) {
		প্রতি (rm = ref_map; rm; rm = rm->next) {
			সংরচনা ref *ref = রিক্ত;

			যদি (rm->status == REF_STATUS_REJECT_SHALLOW) {
				যদি (want_status == FETCH_HEAD_MERGE)
					warning(_("rejected %s because shallow roots are not allowed to be updated"),
						rm->peer_ref ? rm->peer_ref->name : rm->name);
				অনুবর্ত;
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
			যদি (fetch_head->fp) {
				সংরচনা commit *commit = রিক্ত;

				/*
				 * References in "refs/tags/" are often going to point
				 * to annotated tags, which are not part of the
				 * commit-graph. We thus only try to look up refs in
				 * the graph which are not in that namespace to not
				 * regress performance in repositories with many
				 * annotated tags.
				 */
				যদি (!starts_with(rm->name, "refs/tags/"))
					commit = lookup_commit_in_graph(the_repository, &rm->old_oid);
				যদি (!commit) {
					commit = lookup_commit_reference_gently(the_repository,
										&rm->old_oid,
										1);
					যদি (!commit)
						rm->fetch_head_status = FETCH_HEAD_NOT_FOR_MERGE;
				}
			}

			যদি (rm->fetch_head_status != want_status)
				অনুবর্ত;

			যদি (rm->peer_ref) {
				ref = alloc_ref(rm->peer_ref->name);
				oidcpy(&ref->old_oid, &rm->peer_ref->old_oid);
				oidcpy(&ref->new_oid, &rm->old_oid);
				ref->force = rm->peer_ref->force;
			}

			যদি (config->recurse_submodules != RECURSE_SUBMODULES_OFF &&
			    (!rm->peer_ref || !oideq(&ref->old_oid, &ref->new_oid))) {
				check_for_new_submodule_commits(&rm->old_oid);
			}

			যদি (!strcmp(rm->name, "HEAD")) {
				kind = "";
				what = "";
			} অন্যথা যদি (skip_prefix(rm->name, "refs/heads/", &what)) {
				kind = "branch";
			} অন্যথা যদি (skip_prefix(rm->name, "refs/tags/", &what)) {
				kind = "tag";
			} অন্যথা যদি (skip_prefix(rm->name, "refs/remotes/", &what)) {
				kind = "remote-tracking branch";
			} অন্যথা {
				kind = "";
				what = rm->name;
			}

			strbuf_reset(&note);
			যদি (*what) {
				যদি (*kind)
					strbuf_addf(&note, "%s ", kind);
				strbuf_addf(&note, "'%s' of ", what);
			}

			append_fetch_head(fetch_head, &rm->old_oid,
					  rm->fetch_head_status,
					  note.buf, display_state->url,
					  display_state->url_len);

			যদি (ref) {
				rc |= update_local_ref(ref, transaction, rm,
						       config, display_array);
				মুক্ত(ref);
			} অন্যথা যদি (write_fetch_head || dry_run) {
				/*
				 * Display fetches written to FETCH_HEAD (or
				 * would be written to FETCH_HEAD, if --dry-run
				 * is set).
				 */

				ref_update_display_info_append(display_array, '*', '*',
							       *kind ? kind : "branch",
							       রিক্ত, রিক্ত, "FETCH_HEAD",
							       rm->name, &rm->new_oid,
							       &rm->old_oid);
			}
		}
	}

	যদি (advice_enabled(ADVICE_FETCH_SHOW_FORCED_UPDATES)) {
		যদি (!config->show_forced_updates) {
			warning(_(warn_show_forced_updates));
		} অন্যথা যদি (forced_updates_ms > FORCED_UPDATES_DELAY_WARNING_IN_MS) {
			warning(_(warn_time_show_forced_updates),
				forced_updates_ms / 1000.0);
		}
	}

 abort:
	strbuf_release(&note);
	প্রত্যাগম rc;
}

/*
 * We would want to bypass the object transfer altogether if
 * everything we are going to fetch already exists and is connected
 * locally.
 */
স্থির পূর্ণাঙ্ক check_exist_and_connected(সংরচনা ref *ref_map)
{
	সংরচনা ref *rm = ref_map;
	সংরচনা check_connected_options opt = CHECK_CONNECTED_INIT;
	সংরচনা ref *r;

	/*
	 * If we are deepening a shallow clone we already have these
	 * objects reachable.  Running rev-list here will return with
	 * a good (0) exit status and we'll bypass the fetch that we
	 * really need to perform.  Claiming failure now will ensure
	 * we perform the network exchange to deepen our history.
	 */
	যদি (deepen)
		প্রত্যাগম -1;

	/*
	 * Similarly, if we need to refetch, we always want to perform a full
	 * fetch ignoring existing objects.
	 */
	যদি (refetch)
		প্রত্যাগম -1;


	/*
	 * check_connected() allows objects to merely be promised, but
	 * we need all direct targets to exist.
	 */
	প্রতি (r = rm; r; r = r->next) {
		যদি (!odb_has_object(the_repository->objects, &r->old_oid,
				    ODB_HAS_OBJECT_RECHECK_PACKED))
			প্রত্যাগম -1;
	}

	opt.quiet = 1;
	opt.exclude_hidden_refs_section = "fetch";
	প্রত্যাগম check_connected(iterate_ref_map, &rm, &opt);
}

স্থির পূর্ণাঙ্ক fetch_and_consume_refs(সংরচনা display_state *display_state,
				  সংরচনা transport *transport,
				  সংরচনা ref_transaction *transaction,
				  সংরচনা ref *ref_map,
				  সংরচনা fetch_head *fetch_head,
				  ধ্রুব সংরচনা fetch_config *config,
				  সংরচনা ref_update_display_info_array *display_array)
{
	পূর্ণাঙ্ক connectivity_checked = 1;
	পূর্ণাঙ্ক ret;

	/*
	 * We don't need to perform a fetch in case we can already satisfy all
	 * refs.
	 */
	ret = check_exist_and_connected(ref_map);
	যদি (ret) {
		trace2_region_enter("fetch", "fetch_refs", the_repository);
		ret = transport_fetch_refs(transport, ref_map);
		trace2_region_leave("fetch", "fetch_refs", the_repository);
		যদি (ret)
			গচ্ছ out;
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
	প্রত্যাগম ret;
}

স্থির পূর্ণাঙ্ক prune_refs(সংরচনা display_state *display_state,
		      সংরচনা refspec *rs,
		      সংরচনা ref_transaction *transaction,
		      সংরচনা ref *ref_map)
{
	পূর্ণাঙ্ক result = 0;
	সংরচনা ref *ref, *stale_refs = get_stale_heads(rs, ref_map);
	সংরচনা strbuf err = STRBUF_INIT;
	সংরচনা string_list refnames = STRING_LIST_INIT_NODUP;

	প্রতি (ref = stale_refs; ref; ref = ref->next)
		string_list_append(&refnames, ref->name);

	যদি (!dry_run) {
		যদি (transaction) {
			প্রতি (ref = stale_refs; ref; ref = ref->next) {
				result = ref_transaction_delete(transaction, ref->name, রিক্ত,
								রিক্ত, 0, "fetch: prune", &err);
				যদি (result)
					গচ্ছ cleanup;
			}
		} অন্যথা {
			result = refs_delete_refs(get_main_ref_store(the_repository),
						  "fetch: prune", &refnames,
						  0);
		}
	}

	যদি (verbosity >= 0) {
		পূর্ণাঙ্ক summary_width = transport_summary_width(stale_refs);

		প্রতি (ref = stale_refs; ref; ref = ref->next) {
			display_ref_update(display_state, '-', _("[deleted]"), রিক্ত,
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
	প্রত্যাগম result;
}

স্থির শূন্য check_not_current_branch(সংরচনা ref *ref_map)
{
	ধ্রুব বর্ণ *path;
	প্রতি (; ref_map; ref_map = ref_map->next)
		যদি (ref_map->peer_ref &&
		    starts_with(ref_map->peer_ref->name, "refs/heads/") &&
		    (path = branch_checked_out(ref_map->peer_ref->name)))
			die(_("refusing to fetch into branch '%s' "
			      "checked out at '%s'"),
			    ref_map->peer_ref->name, path);
}

স্থির পূর্ণাঙ্ক truncate_fetch_head(শূন্য)
{
	ধ্রুব বর্ণ *filename = git_path_fetch_head(the_repository);
	FILE *fp = fopen_for_writing(filename);

	যদি (!fp)
		প্রত্যাগম error_errno(_("cannot open '%s'"), filename);
	fclose(fp);
	প্রত্যাগম 0;
}

স্থির শূন্য set_option(সংরচনা transport *transport, ধ্রুব বর্ণ *name, ধ্রুব বর্ণ *মান)
{
	পূর্ণাঙ্ক r = transport_set_option(transport, name, মান);
	যদি (r < 0)
		die(_("option \"%s\" value \"%s\" is not valid for %s"),
		    name, মান, transport->url);
	যদি (r > 0)
		warning(_("option \"%s\" is ignored for %s"),
			name, transport->url);
}


স্থির পূর্ণাঙ্ক add_oid(ধ্রুব সংরচনা reference *ref, শূন্য *cb_data)
{
	সংরচনা oid_array *oids = cb_data;

	oid_array_append(oids, ref->oid);
	প্রত্যাগম 0;
}

স্থির শূন্য add_negotiation_tips(সংরচনা string_list *প্রবেশ_list,
				 সংরচনা oid_array **output_list,
				 ধ্রুব বর্ণ *argname)
{
	সংরচনা oid_array *oids = xcalloc(1, পরিমাণ(*oids));
	পূর্ণাঙ্ক i;

	প্রতি (i = 0; i < প্রবেশ_list->nr; i++) {
		ধ্রুব বর্ণ *s = প্রবেশ_list->বস্তুসমূহ[i].string;
		সংরচনা refs_for_each_ref_options opts = {
			.pattern = s,
		};
		পূর্ণাঙ্ক old_nr;
		যদি (!has_glob_specials(s)) {
			সংরচনা object_id oid;

			/* Ignore missing reference. */
			যদি (repo_get_oid(the_repository, s, &oid))
				অনুবর্ত;
			/* Fail on missing object pointed by ref. */
			যদি (!odb_has_object(the_repository->objects, &oid, 0))
				die(_("the object %s does not exist"), s);

			oid_array_append(oids, &oid);
			অনুবর্ত;
		}
		old_nr = oids->nr;
		refs_for_each_ref_ext(get_main_ref_store(the_repository),
				      add_oid, oids, &opts);
		যদি (old_nr == oids->nr)
			warning(_("ignoring %s=%s because it does not match any refs"),
				argname, s);
	}
	*output_list = oids;
}

স্থির সংরচনা transport *prepare_transport(সংরচনা remote *remote, পূর্ণাঙ্ক deepen,
					   সংরচনা list_objects_filter_options *filter_options)
{
	সংরচনা transport *transport;

	transport = transport_get(remote, রিক্ত);
	transport_set_verbosity(transport, verbosity, progress);
	transport->family = family;
	যদি (upload_pack)
		set_option(transport, TRANS_OPT_UPLOADPACK, upload_pack);
	যদি (keep)
		set_option(transport, TRANS_OPT_KEEP, "yes");
	যদি (depth)
		set_option(transport, TRANS_OPT_DEPTH, depth);
	যদি (deepen && deepen_since)
		set_option(transport, TRANS_OPT_DEEPEN_SINCE, deepen_since);
	যদি (deepen && deepen_not.nr)
		set_option(transport, TRANS_OPT_DEEPEN_NOT,
			   (ধ্রুব বর্ণ *)&deepen_not);
	যদি (deepen_relative)
		set_option(transport, TRANS_OPT_DEEPEN_RELATIVE, "yes");
	যদি (update_shallow)
		set_option(transport, TRANS_OPT_UPDATE_SHALLOW, "yes");
	যদি (refetch)
		set_option(transport, TRANS_OPT_REFETCH, "yes");
	যদি (filter_options->choice) {
		ধ্রুব বর্ণ *spec =
			expand_list_objects_filter_spec(filter_options);
		set_option(transport, TRANS_OPT_LIST_OBJECTS_FILTER, spec);
		set_option(transport, TRANS_OPT_FROM_PROMISOR, "1");
	}
	যদি (negotiation_restrict.nr) {
		যদি (transport->smart_options)
			add_negotiation_tips(&negotiation_restrict,
					     &transport->smart_options->negotiation_restrict_tips,
					     "--negotiation-restrict");
		অন্যথা
			warning(_("ignoring %s because the protocol does not support it"),
				"--negotiation-restrict");
	} অন্যথা যদি (remote->negotiation_restrict.nr) {
		সংরচনা string_list_item *item;
		for_each_string_list_item(item, &remote->negotiation_restrict)
			string_list_append(&negotiation_restrict, item->string);
		যদি (transport->smart_options)
			add_negotiation_tips(&negotiation_restrict,
					     &transport->smart_options->negotiation_restrict_tips,
					     "--negotiation-restrict");
		অন্যথা {
			সংরচনা strbuf config_name = STRBUF_INIT;
			strbuf_addf(&config_name, "remote.%s.negotiationRestrict", remote->name);
			warning(_("ignoring %s because the protocol does not support it"),
				config_name.buf);
			strbuf_release(&config_name);
		}
	}
	যদি (negotiation_include.nr) {
		যদি (transport->smart_options)
			add_negotiation_tips(&negotiation_include,
					     &transport->smart_options->negotiation_include_tips,
					     "--negotiation-include");
		অন্যথা
			warning(_("ignoring %s because the protocol does not support it"),
				"--negotiation-include");
	} অন্যথা যদি (remote->negotiation_include.nr) {
		যদি (transport->smart_options) {
			add_negotiation_tips(&remote->negotiation_include,
					     &transport->smart_options->negotiation_include_tips,
					     "--negotiation-include");
		} অন্যথা {
			সংরচনা strbuf config_name = STRBUF_INIT;
			strbuf_addf(&config_name, "remote.%s.negotiationInclude", remote->name);
			warning(_("ignoring %s because the protocol does not support it"),
				config_name.buf);
			strbuf_release(&config_name);
		}
	}
	প্রত্যাগম transport;
}

স্থির পূর্ণাঙ্ক backfill_tags(সংরচনা display_state *display_state,
			 সংরচনা transport *transport,
			 সংরচনা ref_transaction *transaction,
			 সংরচনা ref *ref_map,
			 সংরচনা fetch_head *fetch_head,
			 ধ্রুব সংরচনা fetch_config *config,
			 সংরচনা ref_update_display_info_array *display_array,
			 সংরচনা list_objects_filter_options *filter_options)
{
	পূর্ণাঙ্ক retcode, cannot_reuse;

	/*
	 * Once we have set TRANS_OPT_DEEPEN_SINCE, we can't unset it
	 * when remote helper is used (setting it to an empty string
	 * is not unsetting). We could extend the remote helper
	 * protocol for that, but for now, just force a new connection
	 * without deepen-since. Similar story for deepen-not.
	 */
	cannot_reuse = transport->cannot_reuse ||
		deepen_since || deepen_not.nr;
	যদি (cannot_reuse) {
		gsecondary = prepare_transport(transport->remote, 0, filter_options);
		transport = gsecondary;
	}

	transport_set_option(transport, TRANS_OPT_FOLLOWTAGS, রিক্ত);
	transport_set_option(transport, TRANS_OPT_DEPTH, "0");
	transport_set_option(transport, TRANS_OPT_DEEPEN_RELATIVE, রিক্ত);
	retcode = fetch_and_consume_refs(display_state, transport, transaction, ref_map,
					 fetch_head, config, display_array);

	যদি (gsecondary) {
		transport_disconnect(gsecondary);
		gsecondary = রিক্ত;
	}

	প্রত্যাগম retcode;
}

স্থির ধ্রুব বর্ণ *strip_refshead(ধ্রুব বর্ণ *name){
	skip_prefix(name, "refs/heads/", &name);
	প্রত্যাগম name;
}

স্থির শূন্য set_head_advice_বার্তা(ধ্রুব বর্ণ *remote, ধ্রুব বর্ণ *head_name)
{
	ধ্রুব বর্ণ message_advice_set_head[] =
	N_("Run 'git remote set-head %s %s' to follow the change, or modify\n"
	   "either of the 'remote.%s.followRemoteHEAD' or 'fetch.followRemoteHEAD'\n"
	   "configuration variables to handle the situation differently.\n\n"

	   "Using this specific setting\n\n"
	   "    git config set remote.%s.followRemoteHEAD warn-if-not-%s\n\n"
	   "will suppress the warning until the remote changes HEAD to something else.");

	advise_if_enabled(ADVICE_FETCH_SET_HEAD_WARN, _(message_advice_set_head),
			remote, head_name, remote, remote, head_name);
}

স্থির শূন্য warn_set_head(ধ্রুব বর্ণ *remote, ধ্রুব বর্ণ *head_name,
			সংরচনা strbuf *buf_prev, পূর্ণাঙ্ক updateres) {
	সংরচনা strbuf buf_prefix = STRBUF_INIT;
	ধ্রুব বর্ণ *prev_head = রিক্ত;

	strbuf_addf(&buf_prefix, "refs/remotes/%s/", remote);
	skip_prefix(buf_prev->buf, buf_prefix.buf, &prev_head);

	যদি (prev_head && strcmp(prev_head, head_name)) {
		printf("'HEAD' at '%s' is '%s', but we have '%s' locally.\n",
			remote, head_name, prev_head);
		set_head_advice_বার্তা(remote, head_name);
	}
	অন্যথা যদি (updateres && buf_prev->len) {
		printf("'HEAD' at '%s' is '%s', "
			"but we have a detached HEAD pointing to '%s' locally.\n",
			remote, head_name, buf_prev->buf);
		set_head_advice_বার্তা(remote, head_name);
	}
	strbuf_release(&buf_prefix);
}

স্থির পূর্ণাঙ্ক set_head(ধ্রুব সংরচনা ref *remote_refs, সংরচনা remote *remote,
			পূর্ণাঙ্ক follow_remote_head)
{
	পূর্ণাঙ্ক result = 0, create_only, baremirror, was_detached;
	সংরচনা strbuf b_head = STRBUF_INIT, b_remote_head = STRBUF_INIT,
		      b_local_head = STRBUF_INIT;
	ধ্রুব বর্ণ *no_warn_branch = remote->no_warn_branch;
	বর্ণ *head_name = রিক্ত;
	সংরচনা ref *ref, *matches;
	সংরচনা ref *fetch_map = রিক্ত, **fetch_map_tail = &fetch_map;
	সংরচনা refspec_item refspec = {
		.force = 0,
		.pattern = 1,
		.src = (বর্ণ *) "refs/heads/*",
		.dst = (বর্ণ *) "refs/heads/*",
	};
	সংরচনা string_list heads = STRING_LIST_INIT_DUP;
	সংরচনা ref_store *refs = get_main_ref_store(the_repository);

	get_fetch_map(remote_refs, &refspec, &fetch_map_tail, 0);
	matches = guess_remote_head(find_ref_by_name(remote_refs, "HEAD"),
				    fetch_map, REMOTE_GUESS_HEAD_ALL);
	প্রতি (ref = matches; ref; ref = ref->next) {
		string_list_append(&heads, strip_refshead(ref->name));
	}

	যদি (!heads.nr)
		result = 1;
	অন্যথা যদি (heads.nr > 1)
		result = 1;
	অন্যথা
		head_name = xstrdup(heads.বস্তুসমূহ[0].string);

	যদি (!head_name)
		গচ্ছ cleanup;
	baremirror = is_bare_repository(the_repository) && remote->mirror;
	create_only = follow_remote_head == FOLLOW_REMOTE_ALWAYS ? 0 : !baremirror;
	যদি (baremirror) {
		strbuf_addstr(&b_head, "HEAD");
		strbuf_addf(&b_remote_head, "refs/heads/%s", head_name);
	} অন্যথা {
		strbuf_addf(&b_head, "refs/remotes/%s/HEAD", remote->name);
		strbuf_addf(&b_remote_head, "refs/remotes/%s/%s", remote->name, head_name);
	}
	/* make sure it's valid */
	যদি (!baremirror && !refs_ref_exists(refs, b_remote_head.buf)) {
		result = 1;
		গচ্ছ cleanup;
	}
	was_detached = refs_update_symref_extended(refs, b_head.buf, b_remote_head.buf,
					"fetch", &b_local_head, create_only);
	যদি (was_detached == -1) {
		result = 1;
		গচ্ছ cleanup;
	}
	যদি (verbosity >= 0 &&
		follow_remote_head == FOLLOW_REMOTE_WARN &&
		(!no_warn_branch || strcmp(no_warn_branch, head_name)))
		warn_set_head(remote->name, head_name, &b_local_head, was_detached);

cleanup:
	মুক্ত(head_name);
	free_refs(fetch_map);
	free_refs(matches);
	string_list_clear(&heads, 0);
	strbuf_release(&b_head);
	strbuf_release(&b_local_head);
	strbuf_release(&b_remote_head);
	প্রত্যাগম result;
}

সংরচনা ref_rejection_data {
	পূর্ণাঙ্ক *retcode;
	bool conflict_বার্তা_shown;
	bool case_sensitive_বার্তা_shown;
	ধ্রুব বর্ণ *remote_name;
	সংরচনা strmap *rejected_refs;
};

স্থির শূন্য ref_transaction_rejection_handler(ধ্রুব বর্ণ *refname,
					      ধ্রুব সংরচনা object_id *old_oid UNUSED,
					      ধ্রুব সংরচনা object_id *new_oid UNUSED,
					      ধ্রুব বর্ণ *old_target UNUSED,
					      ধ্রুব বর্ণ *new_target UNUSED,
					      গণনা ref_transaction_error err,
					      ধ্রুব বর্ণ *details,
					      শূন্য *cb_data)
{
	সংরচনা ref_rejection_data *data = cb_data;

	যদি (err == REF_TRANSACTION_ERROR_CASE_CONFLICT && repo_ignore_case(the_repository) &&
	    !data->case_sensitive_বার্তা_shown) {
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
		data->case_sensitive_বার্তা_shown = true;
	} অন্যথা যদি (err == REF_TRANSACTION_ERROR_NAME_CONFLICT &&
		   !data->conflict_বার্তা_shown) {
		error(_("some local refs could not be updated; try running\n"
			" 'git remote prune %s' to remove any old, conflicting "
			"branches"), data->remote_name);
		data->conflict_বার্তা_shown = true;
	} অন্যথা {
		যদি (details)
			error("%s", details);
		অন্যথা
			error(_("fetching ref %s failed: %s"),
			      refname, ref_transaction_error_বার্তা(err));
	}

	strmap_put(data->rejected_refs, refname, রিক্ত);
	*data->retcode = 1;
}

/*
 * Commit the reference transaction. If it isn't an atomic transaction, handle
 * rejected updates as part of using batched updates.
 */
স্থির পূর্ণাঙ্ক commit_ref_transaction(সংরচনা ref_transaction **transaction,
				  bool is_atomic, ধ্রুব বর্ণ *remote_name,
				  সংরচনা strmap *rejected_refs,
				  সংরচনা strbuf *err)
{
	পূর্ণাঙ্ক retcode = ref_transaction_commit(*transaction, err);
	যদি (retcode)
		গচ্ছ out;

	যদি (!is_atomic) {
		সংরচনা ref_rejection_data data = {
			.conflict_বার্তা_shown = 0,
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
	*transaction = রিক্ত;
	প্রত্যাগম retcode;
}

স্থির শূন্য collect_updated_tips(সংরচনা oidset *tips, সংরচনা ref *ref_map)
{
	সংরচনা ref *rm;
	প্রতি (rm = ref_map; rm; rm = rm->next) {
		সংরচনা commit *commit;
		/*
		 * Shallow-rejected refs are not stored and their history
		 * is incomplete, so skip them.
		 */
		যদি (rm->status == REF_STATUS_REJECT_SHALLOW)
			অনুবর্ত;
		যদি (is_null_oid(&rm->old_oid))
			অনুবর্ত;
		যদি (rm->peer_ref &&
		    oideq(&rm->old_oid, &rm->peer_ref->old_oid))
			অনুবর্ত;
		commit = lookup_commit_reference_gently(the_repository,
							&rm->old_oid, 1);
		যদি (commit)
			oidset_ঢোকাও(tips, &commit->object.oid);
	}
}

স্থির পূর্ণাঙ্ক do_fetch(সংরচনা transport *transport,
		    সংরচনা refspec *rs,
		    ধ্রুব সংরচনা fetch_config *config,
		    সংরচনা list_objects_filter_options *filter_options,
		    সংরচনা oidset *updated_tips)
{
	সংরচনা ref_transaction *transaction = রিক্ত;
	সংরচনা ref *ref_map = রিক্ত;
	সংরচনা display_state display_state = { 0 };
	পূর্ণাঙ্ক autotags = (transport->remote->fetch_tags == 1);
	পূর্ণাঙ্ক retcode = 0;
	ধ্রুব সংরচনা ref *remote_refs;
	সংরচনা transport_ls_refs_options transport_ls_refs_options =
		TRANSPORT_LS_REFS_OPTIONS_INIT;
	সংরচনা fetch_head fetch_head = { 0 };
	সংরচনা strbuf err = STRBUF_INIT;
	সংরচনা ref_update_display_info_array display_array = { 0 };
	সংরচনা strmap rejected_refs = STRMAP_INIT;
	পূর্ণাঙ্ক summary_width = 0;
	পূর্ণাঙ্ক follow_remote_head = FOLLOW_REMOTE_NEVER;

	যদি (tags == TAGS_DEFAULT) {
		যদি (transport->remote->fetch_tags == 2)
			tags = TAGS_SET;
		যদি (transport->remote->fetch_tags == -1)
			tags = TAGS_UNSET;
	}

	/* if not appending, truncate FETCH_HEAD */
	যদি (!append && write_fetch_head) {
		retcode = truncate_fetch_head();
		যদি (retcode)
			গচ্ছ cleanup;
	}

	যদি (rs->nr) {
		refspec_ref_prefixes(rs, &transport_ls_refs_options.ref_prefixes);
	} অন্যথা {
		সংরচনা branch *branch = branch_get(রিক্ত);

		যদি (transport->remote->fetch.nr) {
			refspec_ref_prefixes(&transport->remote->fetch,
					     &transport_ls_refs_options.ref_prefixes);
			/*
			 * See remote.c's handling of remote.<name>.followRemoteHEAD
			 * for the analogous, still-unresolved case.
			 */
			যদি (transport->remote->follow_remote_head)
				follow_remote_head = transport->remote->follow_remote_head;
			অন্যথা যদি (config->follow_remote_head_seen)
				follow_remote_head = get_follow_remote_head(config->follow_remote_head_raw);
			অন্যথা
				follow_remote_head = BUILTIN_FOLLOW_REMOTE_HEAD_DFLT;
		}
		যদি (branch && branch_has_merge_config(branch) &&
		    !strcmp(branch->remote_name, transport->remote->name)) {
			পূর্ণাঙ্ক i;
			প্রতি (i = 0; i < branch->merge_nr; i++) {
				strvec_ঠেলো(&transport_ls_refs_options.ref_prefixes,
					    branch->merge[i]->src);
			}
		}

		/*
		 * If there are no refs specified to fetch, then we just
		 * fetch HEAD; mention that to narrow the advertisement.
		 */
		যদি (!transport_ls_refs_options.ref_prefixes.nr)
			strvec_ঠেলো(&transport_ls_refs_options.ref_prefixes,
				    "HEAD");
	}

	যদি (tags == TAGS_SET || tags == TAGS_DEFAULT)
		strvec_ঠেলো(&transport_ls_refs_options.ref_prefixes,
			    "refs/tags/");

	যদি (follow_remote_head != FOLLOW_REMOTE_NEVER)
		strvec_ঠেলো(&transport_ls_refs_options.ref_prefixes,
			    "HEAD");

	/*
	 * Only initiate ref listing if we have at least one ref we want to
	 * know about.
	 */
	যদি (transport_ls_refs_options.ref_prefixes.nr) {
		trace2_region_enter("fetch", "remote_refs", the_repository);
		remote_refs = transport_get_remote_refs(transport,
							&transport_ls_refs_options);
		trace2_region_leave("fetch", "remote_refs", the_repository);
	} অন্যথা
		remote_refs = রিক্ত;

	transport_ls_refs_options_release(&transport_ls_refs_options);

	ref_map = get_ref_map(transport->remote, remote_refs, rs,
			      tags, &autotags);
	যদি (!update_head_ok)
		check_not_current_branch(ref_map);

	retcode = open_fetch_head(&fetch_head);
	যদি (retcode)
		গচ্ছ cleanup;

	display_state_init(&display_state, ref_map, transport->url,
			   config->display_format);

	যদি (atomic_fetch) {
		transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
							  0, &err);
		যদি (!transaction) {
			retcode = -1;
			গচ্ছ cleanup;
		}
	}

	যদি (tags == TAGS_DEFAULT && autotags)
		transport_set_option(transport, TRANS_OPT_FOLLOWTAGS, "1");
	যদি (prune) {
		/*
		 * We only prune based on refspecs specified
		 * explicitly (via command line or configuration); we
		 * don't care whether --tags was specified.
		 */
		যদি (rs->nr) {
			retcode = prune_refs(&display_state, rs, transaction, ref_map);
		} অন্যথা {
			retcode = prune_refs(&display_state, &transport->remote->fetch,
					     transaction, ref_map);
		}
		যদি (retcode != 0)
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
	যদি (!transaction) {
		transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
							  REF_TRANSACTION_ALLOW_FAILURE, &err);
		যদি (!transaction) {
			retcode = -1;
			গচ্ছ cleanup;
		}
	}

	যদি (fetch_and_consume_refs(&display_state, transport, transaction, ref_map,
				   &fetch_head, config, &display_array)) {
		retcode = 1;
		গচ্ছ cleanup;
	}

	/*
	 * If neither --no-tags nor --tags was specified, do automated tag
	 * following.
	 */
	যদি (tags == TAGS_DEFAULT && autotags) {
		সংরচনা ref *tags_ref_map = রিক্ত, **tail = &tags_ref_map;

		find_non_local_tags(remote_refs, transaction, &tags_ref_map, &tail);
		যদি (tags_ref_map) {
			/*
			 * If backfilling of tags fails then we want to tell
			 * the user so, but we have to continue regardless to
			 * populate upstream information of the references we
			 * have already fetched above. The exception though is
			 * when `--atomic` is passed: in that case we'll abort
			 * the transaction and don't commit anything.
			 */
			যদি (backfill_tags(&display_state, transport, transaction, tags_ref_map,
					  &fetch_head, config, &display_array, filter_options))
				retcode = 1;
		}

		free_refs(tags_ref_map);
	}

	যদি (retcode)
		গচ্ছ cleanup;

	যদি (verbosity >= 0)
		summary_width = transport_summary_width(ref_map);

	retcode = commit_ref_transaction(&transaction, atomic_fetch,
					 transport->remote->name,
					 &rejected_refs, &err);
	/*
	 * With '--atomic', bail out if the transaction fails. Without '--atomic',
	 * continue to fetch head and perform other post-fetch operations.
	 */
	যদি (retcode && atomic_fetch)
		গচ্ছ cleanup;

	commit_fetch_head(&fetch_head);

	collect_updated_tips(updated_tips, ref_map);

	যদি (set_upstream) {
		সংরচনা branch *branch = branch_get("HEAD");
		সংরচনা ref *rm;
		সংরচনা ref *source_ref = রিক্ত;

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
		প্রতি (rm = ref_map; rm; rm = rm->next) {
			যদি (!rm->peer_ref) {
				যদি (source_ref) {
					warning(_("multiple branches detected, incompatible with --set-upstream"));
					গচ্ছ cleanup;
				} অন্যথা {
					source_ref = rm;
				}
			}
		}
		যদি (source_ref) {
			যদি (!branch) {
				ধ্রুব বর্ণ *shortname = source_ref->name;
				skip_prefix(shortname, "refs/heads/", &shortname);

				warning(_("could not set upstream of HEAD to '%s' from '%s' when "
					  "it does not point to any branch."),
					shortname, transport->remote->name);
				গচ্ছ cleanup;
			}

			যদি (!strcmp(source_ref->name, "HEAD") ||
			    starts_with(source_ref->name, "refs/heads/"))
				install_branch_config(0,
						      branch->name,
						      transport->remote->name,
						      source_ref->name);
			অন্যথা যদি (starts_with(source_ref->name, "refs/remotes/"))
				warning(_("not setting upstream for a remote remote-tracking branch"));
			অন্যথা যদি (starts_with(source_ref->name, "refs/tags/"))
				warning(_("not setting upstream for a remote tag"));
			অন্যথা
				warning(_("unknown branch type"));
		} অন্যথা {
			warning(_("no source branch found;\n"
				  "you need to specify exactly one branch with the --set-upstream option"));
		}
	}
	যদি (follow_remote_head != FOLLOW_REMOTE_NEVER) {
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
	যদি (retcode && !atomic_fetch && transaction)
		commit_ref_transaction(&transaction, false,
				       transport->remote->name,
				       &rejected_refs, &err);

	প্রতি (size_t i = 0; i < display_array.nr; i++) {
		সংরচনা ref_update_display_info *info = &display_array.info[i];

		যদি (!info->failed && strmap_contains(&rejected_refs, info->ref))
			ref_update_display_info_set_failed(info);
		ref_update_display_info_display(info, &display_state, summary_width);
		ref_update_display_info_free(info);
	}

	যদি (retcode) {
		যদি (err.len) {
			error("%s", err.buf);
			strbuf_reset(&err);
		}
		যদি (transaction && ref_transaction_abort(transaction, &err) &&
		    err.len)
			error("%s", err.buf);
		transaction = রিক্ত;
	}

	যদি (transaction)
		ref_transaction_free(transaction);

	মুক্ত(display_array.info);
	strmap_clear(&rejected_refs, 0);
	display_state_release(&display_state);
	close_fetch_head(&fetch_head);
	strbuf_release(&err);
	free_refs(ref_map);
	প্রত্যাগম retcode;
}

স্থির পূর্ণাঙ্ক get_one_remote_for_fetch(সংরচনা remote *remote, শূন্য *priv)
{
	সংরচনা string_list *list = priv;
	যদি (!remote->skip_default_update)
		string_list_append(list, remote->name);
	প্রত্যাগম 0;
}

স্থির শূন্য add_options_to_argv(সংরচনা strvec *argv,
				ধ্রুব সংরচনা fetch_config *config)
{
	যদি (dry_run)
		strvec_ঠেলো(argv, "--dry-run");
	যদি (prune != -1)
		strvec_ঠেলো(argv, prune ? "--prune" : "--no-prune");
	যদি (prune_tags != -1)
		strvec_ঠেলো(argv, prune_tags ? "--prune-tags" : "--no-prune-tags");
	যদি (update_head_ok)
		strvec_ঠেলো(argv, "--update-head-ok");
	যদি (force)
		strvec_ঠেলো(argv, "--force");
	যদি (keep)
		strvec_ঠেলো(argv, "--keep");
	যদি (config->recurse_submodules == RECURSE_SUBMODULES_ON)
		strvec_ঠেলো(argv, "--recurse-submodules");
	অন্যথা যদি (config->recurse_submodules == RECURSE_SUBMODULES_OFF)
		strvec_ঠেলো(argv, "--no-recurse-submodules");
	অন্যথা যদি (config->recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND)
		strvec_ঠেলো(argv, "--recurse-submodules=on-demand");
	যদি (tags == TAGS_SET)
		strvec_ঠেলো(argv, "--tags");
	অন্যথা যদি (tags == TAGS_UNSET)
		strvec_ঠেলো(argv, "--no-tags");
	যদি (verbosity >= 2)
		strvec_ঠেলো(argv, "-v");
	যদি (verbosity >= 1)
		strvec_ঠেলো(argv, "-v");
	অন্যথা যদি (verbosity < 0)
		strvec_ঠেলো(argv, "-q");
	যদি (family == TRANSPORT_FAMILY_IPV4)
		strvec_ঠেলো(argv, "--ipv4");
	অন্যথা যদি (family == TRANSPORT_FAMILY_IPV6)
		strvec_ঠেলো(argv, "--ipv6");
	যদি (!write_fetch_head)
		strvec_ঠেলো(argv, "--no-write-fetch-head");
	যদি (config->display_format == DISPLAY_FORMAT_PORCELAIN)
		strvec_pushf(argv, "--porcelain");
}

/* Fetch multiple remotes in parallel */

সংরচনা parallel_fetch_state {
	ধ্রুব বর্ণ **argv;
	সংরচনা string_list *remotes;
	পূর্ণাঙ্ক next, result;
	ধ্রুব সংরচনা fetch_config *config;
};

স্থির পূর্ণাঙ্ক fetch_next_remote(সংরচনা child_process *cp,
			     সংরচনা strbuf *out UNUSED,
			     শূন্য *cb, শূন্য **task_cb)
{
	সংরচনা parallel_fetch_state *state = cb;
	বর্ণ *remote;

	যদি (state->next < 0 || state->next >= state->remotes->nr)
		প্রত্যাগম 0;

	remote = state->remotes->বস্তুসমূহ[state->next++].string;
	*task_cb = remote;

	strvec_pushv(&cp->args, state->argv);
	strvec_ঠেলো(&cp->args, remote);
	cp->git_cmd = 1;

	যদি (verbosity >= 0 && state->config->display_format != DISPLAY_FORMAT_PORCELAIN)
		printf(_("Fetching %s\n"), remote);

	প্রত্যাগম 1;
}

স্থির পূর্ণাঙ্ক fetch_failed_to_start(সংরচনা strbuf *out UNUSED,
				 শূন্য *cb, শূন্য *task_cb)
{
	সংরচনা parallel_fetch_state *state = cb;
	ধ্রুব বর্ণ *remote = task_cb;

	state->result = error(_("could not fetch %s"), remote);

	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক fetch_finished(পূর্ণাঙ্ক result, সংরচনা strbuf *out,
			  শূন্য *cb, শূন্য *task_cb)
{
	সংরচনা parallel_fetch_state *state = cb;
	ধ্রুব বর্ণ *remote = task_cb;

	যদি (result) {
		strbuf_addf(out, _("could not fetch '%s' (exit code: %d)\n"),
			    remote, result);
		state->result = -1;
	}

	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক fetch_multiple(সংরচনা string_list *list, পূর্ণাঙ্ক max_children,
			  ধ্রুব সংরচনা fetch_config *config)
{
	পূর্ণাঙ্ক i, result = 0;
	সংরচনা strvec argv = STRVEC_INIT;

	যদি (!append && write_fetch_head) {
		পূর্ণাঙ্ক errcode = truncate_fetch_head();
		যদি (errcode)
			প্রত্যাগম errcode;
	}

	/*
	 * Cancel out the fetch.bundleURI config when running subprocesses,
	 * to avoid fetching from the same bundle list multiple times.
	 */
	strvec_pushl(&argv, "-c", "fetch.bundleURI=",
		     "fetch", "--append", "--no-auto-gc",
		     "--no-write-commit-graph", রিক্ত);
	প্রতি (i = 0; i < server_options.nr; i++)
		strvec_pushf(&argv, "--server-option=%s", server_options.বস্তুসমূহ[i].string);
	add_options_to_argv(&argv, config);

	যদি (max_children != 1 && list->nr != 1) {
		সংরচনা parallel_fetch_state state = { argv.v, list, 0, 0, config };
		ধ্রুব সংরচনা run_process_parallel_opts opts = {
			.tr2_category = "fetch",
			.tr2_label = "parallel/fetch",

			.processes = max_children,

			.get_next_task = &fetch_next_remote,
			.start_failure = &fetch_failed_to_start,
			.task_finished = &fetch_finished,
			.data = &state,
		};

		strvec_ঠেলো(&argv, "--end-of-options");

		run_processes_parallel(&opts);
		result = state.result;
	} অন্যথা
		প্রতি (i = 0; i < list->nr; i++) {
			ধ্রুব বর্ণ *name = list->বস্তুসমূহ[i].string;
			সংরচনা child_process cmd = CHILD_PROCESS_INIT;

			strvec_pushv(&cmd.args, argv.v);
			strvec_ঠেলো(&cmd.args, name);
			যদি (verbosity >= 0 && config->display_format != DISPLAY_FORMAT_PORCELAIN)
				printf(_("Fetching %s\n"), name);
			cmd.git_cmd = 1;
			যদি (run_command(&cmd)) {
				error(_("could not fetch %s"), name);
				result = 1;
			}
		}

	strvec_clear(&argv);
	প্রত্যাগম !!result;
}

/*
 * Fetching from the promisor remote should use the given filter-spec
 * or inherit the default filter-spec from the config.
 */
স্থির অন্তঃস্থ শূন্য fetch_one_setup_partial(সংরচনা remote *remote,
					   সংরচনা list_objects_filter_options *filter_options)
{
	/*
	 * Explicit --no-filter argument overrides everything, regardless
	 * of any prior partial clones and fetches.
	 */
	যদি (filter_options->no_filter)
		প্রত্যাগম;

	/*
	 * If no prior partial clone/fetch and the current fetch DID NOT
	 * request a partial-fetch, do a normal fetch.
	 */
	যদি (!repo_has_promisor_remote(the_repository) && !filter_options->choice)
		প্রত্যাগম;

	/*
	 * If this is a partial-fetch request, we enable partial on
	 * this repo if not already enabled and remember the given
	 * filter-spec as the default for subsequent fetches to this
	 * remote if there is currently no default filter-spec.
	 */
	যদি (filter_options->choice) {
		partial_clone_register(remote->name, filter_options);
		প্রত্যাগম;
	}

	/*
	 * Do a partial-fetch from the promisor remote using either the
	 * explicitly given filter-spec or inherit the filter-spec from
	 * the config.
	 */
	যদি (!filter_options->choice)
		partial_clone_get_default_filter_spec(filter_options, remote->name);
	প্রত্যাগম;
}

স্থির পূর্ণাঙ্ক fetch_one(সংরচনা remote *remote, পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv,
		     পূর্ণাঙ্ক prune_tags_ok, পূর্ণাঙ্ক use_stdin_refspecs,
		     ধ্রুব সংরচনা fetch_config *config,
		     সংরচনা list_objects_filter_options *filter_options,
		     সংরচনা oidset *updated_tips)
{
	সংরচনা refspec rs = REFSPEC_INIT_FETCH(the_hash_algo);
	পূর্ণাঙ্ক i;
	পূর্ণাঙ্ক exit_code;
	পূর্ণাঙ্ক maybe_prune_tags;
	পূর্ণাঙ্ক remote_via_config = remote_is_configured(remote, 0);

	যদি (!remote)
		die(_("no remote repository specified; please specify either a URL or a\n"
		      "remote name from which new revisions should be fetched"));

	gtransport = prepare_transport(remote, 1, filter_options);

	যদি (prune < 0) {
		/* no command line request */
		যদি (0 <= remote->prune)
			prune = remote->prune;
		অন্যথা যদি (0 <= config->prune)
			prune = config->prune;
		অন্যথা
			prune = PRUNE_BY_DEFAULT;
	}

	যদি (prune_tags < 0) {
		/* no command line request */
		যদি (0 <= remote->prune_tags)
			prune_tags = remote->prune_tags;
		অন্যথা যদি (0 <= config->prune_tags)
			prune_tags = config->prune_tags;
		অন্যথা
			prune_tags = PRUNE_TAGS_BY_DEFAULT;
	}

	maybe_prune_tags = prune_tags_ok && prune_tags;
	যদি (maybe_prune_tags && remote_via_config)
		refspec_append(&remote->fetch, TAG_REFSPEC);

	যদি (maybe_prune_tags && (argc || !remote_via_config))
		refspec_append(&rs, TAG_REFSPEC);

	প্রতি (i = 0; i < argc; i++) {
		যদি (!strcmp(argv[i], "tag")) {
			i++;
			যদি (i >= argc)
				die(_("you need to specify a tag name"));

			refspec_appendf(&rs, "refs/tags/%s:refs/tags/%s",
					argv[i], argv[i]);
		} অন্যথা {
			refspec_append(&rs, argv[i]);
		}
	}

	যদি (use_stdin_refspecs) {
		সংরচনা strbuf সারি = STRBUF_INIT;
		যাবত্ (strbuf_getline_lf(&সারি, stdin) != EOF)
			refspec_append(&rs, সারি.buf);
		strbuf_release(&সারি);
	}

	যদি (server_options.nr)
		gtransport->server_options = &server_options;

	sigchain_ঠেলো_common(unlock_pack_on_signal);
	atexit(unlock_pack_atexit);
	sigchain_ঠেলো(SIGPIPE, SIG_IGN);
	exit_code = do_fetch(gtransport, &rs, config, filter_options,
			     updated_tips);
	sigchain_বের_করো(SIGPIPE);
	refspec_clear(&rs);
	transport_disconnect(gtransport);
	gtransport = রিক্ত;
	প্রত্যাগম exit_code;
}

পূর্ণাঙ্ক cmd_fetch(পূর্ণাঙ্ক argc,
	      ধ্রুব বর্ণ **argv,
	      ধ্রুব বর্ণ *prefix,
	      সংরচনা repository *repo UNUSED)
{
	সংরচনা fetch_config config = {
		.display_format = DISPLAY_FORMAT_FULL,
		.follow_remote_head_raw = রিক্ত,
		.follow_remote_head_seen = 0,
		.prune = -1,
		.prune_tags = -1,
		.show_forced_updates = 1,
		.recurse_submodules = RECURSE_SUBMODULES_DEFAULT,
		.parallel = 1,
		.submodule_fetch_jobs = -1,
	};
	ধ্রুব বর্ণ *submodule_prefix = "";
	ধ্রুব বর্ণ *bundle_uri;
	সংরচনা string_list list = STRING_LIST_INIT_DUP;
	সংরচনা list_objects_filter_options filter_options = LIST_OBJECTS_FILTER_INIT;
	সংরচনা remote *remote = রিক্ত;
	পূর্ণাঙ্ক all = -1, multiple = 0;
	পূর্ণাঙ্ক result = 0;
	পূর্ণাঙ্ক prune_tags_ok = 1;
	পূর্ণাঙ্ক enable_auto_gc = 1;
	পূর্ণাঙ্ক unshallow = 0;
	পূর্ণাঙ্ক max_jobs = -1;
	পূর্ণাঙ্ক recurse_submodules_cli = RECURSE_SUBMODULES_DEFAULT;
	পূর্ণাঙ্ক recurse_submodules_default = RECURSE_SUBMODULES_ON_DEMAND;
	পূর্ণাঙ্ক fetch_write_commit_graph = -1;
	পূর্ণাঙ্ক stdin_refspecs = 0;
	পূর্ণাঙ্ক negotiate_only = 0;
	পূর্ণাঙ্ক porcelain = 0;
	পূর্ণাঙ্ক i;
	গণনা {
		GRAPH_WRITE_REACHABLE,
		GRAPH_WRITE_TIPS,
		GRAPH_WRITE_SKIP,
	} graph_write_mode = GRAPH_WRITE_REACHABLE;
	সংরচনা oidset updated_tips = OIDSET_INIT;

	সংরচনা option builtin_fetch_options[] = {
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
		OPT_SET_INT('n', রিক্ত, &tags,
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
			.মান = &submodule_prefix,
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
	প্রতি (i = 1; i < argc; i++) {
		/* This handles non-URLs gracefully */
		বর্ণ *anon = transport_anonymize_url(argv[i]);

		strbuf_addf(&default_rla, " %s", anon);
		মুক্ত(anon);
	}

	repo_config(the_repository, git_fetch_config, &config);
	যদি (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_full_index = 0;
	}

	argc = parse_options(argc, argv, prefix,
			     builtin_fetch_options, builtin_fetch_usage, 0);

	যদি (recurse_submodules_cli != RECURSE_SUBMODULES_DEFAULT)
		config.recurse_submodules = recurse_submodules_cli;

	যদি (negotiate_only) {
		বিকল্প (recurse_submodules_cli) {
		পক্ষ RECURSE_SUBMODULES_OFF:
		পক্ষ RECURSE_SUBMODULES_DEFAULT:
			/*
			 * --negotiate-only should never recurse into
			 * submodules. Skip it by setting recurse_submodules to
			 * RECURSE_SUBMODULES_OFF.
			 */
			config.recurse_submodules = RECURSE_SUBMODULES_OFF;
			ভঙ্গ;

		স্বভাব:
			die(_("options '%s' and '%s' cannot be used together"),
			    "--negotiate-only", "--recurse-submodules");
		}
	}

	যদি (config.recurse_submodules != RECURSE_SUBMODULES_OFF) {
		পূর্ণাঙ্ক *sfjc = config.submodule_fetch_jobs == -1
			    ? &config.submodule_fetch_jobs : রিক্ত;
		পূর্ণাঙ্ক *rs = config.recurse_submodules == RECURSE_SUBMODULES_DEFAULT
			  ? &config.recurse_submodules : রিক্ত;

		fetch_config_from_gitmodules(the_repository, sfjc, rs);
	}


	যদি (porcelain) {
		বিকল্প (recurse_submodules_cli) {
		পক্ষ RECURSE_SUBMODULES_OFF:
		পক্ষ RECURSE_SUBMODULES_DEFAULT:
			/*
			 * Reference updates in submodules would be ambiguous
			 * in porcelain mode, so we reject this combination.
			 */
			config.recurse_submodules = RECURSE_SUBMODULES_OFF;
			ভঙ্গ;

		স্বভাব:
			die(_("options '%s' and '%s' cannot be used together"),
			    "--porcelain", "--recurse-submodules");
		}

		config.display_format = DISPLAY_FORMAT_PORCELAIN;
	}

	যদি (deepen_relative) {
		যদি (deepen_relative < 0)
			die(_("negative depth in --deepen is not supported"));
		যদি (depth)
			die(_("options '%s' and '%s' cannot be used together"), "--deepen", "--depth");
		depth = xstrfmt("%d", deepen_relative);
	}
	যদি (unshallow) {
		যদি (depth)
			die(_("options '%s' and '%s' cannot be used together"), "--depth", "--unshallow");
		অন্যথা যদি (!is_repository_shallow(the_repository))
			die(_("--unshallow on a complete repository does not make sense"));
		অন্যথা
			depth = xstrfmt("%d", INFINITE_DEPTH);
	}

	/* no need to be strict, transport_set_option() will validate it again */
	যদি (depth && atoi(depth) < 1)
		die(_("depth %s is not a positive number"), depth);
	যদি (depth || deepen_since || deepen_not.nr)
		deepen = 1;

	/* FETCH_HEAD never gets updated in --dry-run mode */
	যদি (dry_run)
		write_fetch_head = 0;

	যদি (!max_jobs)
		max_jobs = online_cpus();

	যদি (!repo_config_get_string_tmp(the_repository, "fetch.bundleuri", &bundle_uri) &&
	    fetch_bundle_uri(the_repository, bundle_uri, রিক্ত))
		warning(_("failed to fetch bundles from '%s'"), bundle_uri);

	যদি (all < 0) {
		/*
		 * no --[no-]all given;
		 * only use config option if no remote was explicitly specified
		 */
		all = (!argc) ? config.all : 0;
	}

	যদি (all) {
		যদি (argc == 1)
			die(_("fetch --all does not take a repository argument"));
		অন্যথা যদি (argc > 1)
			die(_("fetch --all does not make sense with refspecs"));

		(শূন্য) for_each_remote(get_one_remote_for_fetch, &list);

		/* do not do fetch_multiple() of one */
		যদি (list.nr == 1)
			remote = remote_get(list.বস্তুসমূহ[0].string);
	} অন্যথা যদি (argc == 0) {
		/* No arguments -- use default remote */
		remote = remote_get(রিক্ত);
	} অন্যথা যদি (multiple) {
		/* All arguments are assumed to be remotes or groups */
		প্রতি (i = 0; i < argc; i++)
			যদি (!add_remote_or_group(argv[i], &list))
				die(_("no such remote or remote group: %s"),
				    argv[i]);
	} অন্যথা {
		/* Single remote or group */
		(শূন্য) add_remote_or_group(argv[0], &list);
		যদি (list.nr > 1) {
			/* More than one remote */
			যদি (argc > 1)
				die(_("fetching a group and specifying refspecs does not make sense"));
		} অন্যথা {
			/* Zero or one remotes */
			remote = remote_get(argv[0]);
			prune_tags_ok = (argc == 1);
			argc--;
			argv++;
		}
	}
	string_list_remove_duplicates(&list, 0);

	যদি (negotiate_only) {
		সংরচনা oidset acked_commits = OIDSET_INIT;
		সংরচনা oidset_iter iter;
		ধ্রুব সংরচনা object_id *oid;

		trace2_region_enter("fetch", "negotiate-only", the_repository);
		যদি (!remote)
			die(_("must supply remote when using --negotiate-only"));
		gtransport = prepare_transport(remote, 1, &filter_options);

		যদি (!gtransport->smart_options) {
			warning(_("protocol does not support --negotiate-only, exiting"));
			result = 1;
			trace2_region_leave("fetch", "negotiate-only", the_repository);
			গচ্ছ cleanup;
		}
		যদি (!gtransport->smart_options->negotiation_restrict_tips)
			die(_("%s needs one or more %s"), "--negotiate-only",
			    "--negotiation-restrict=*");

		gtransport->smart_options->acked_commits = &acked_commits;

		যদি (server_options.nr)
			gtransport->server_options = &server_options;
		result = transport_fetch_refs(gtransport, রিক্ত);
		gtransport->smart_options->acked_commits = রিক্ত;

		oidset_iter_init(&acked_commits, &iter);
		যাবত্ ((oid = oidset_iter_next(&iter)))
			printf("%s\n", oid_to_hex(oid));
		oidset_clear(&acked_commits);
		trace2_region_leave("fetch", "negotiate-only", the_repository);
	} অন্যথা যদি (remote) {
		যদি (filter_options.choice || repo_has_promisor_remote(the_repository)) {
			trace2_region_enter("fetch", "setup-partial", the_repository);
			fetch_one_setup_partial(remote, &filter_options);
			trace2_region_leave("fetch", "setup-partial", the_repository);
		}
		trace2_region_enter("fetch", "fetch-one", the_repository);
		result = fetch_one(remote, argc, argv, prune_tags_ok, stdin_refspecs,
				   &config, &filter_options, &updated_tips);
		যদি (prepare_commit_graph(the_repository)) {
			যদি (oidset_size(&updated_tips))
				graph_write_mode = GRAPH_WRITE_TIPS;
			অন্যথা
				graph_write_mode = GRAPH_WRITE_SKIP;
		}
		trace2_region_leave("fetch", "fetch-one", the_repository);
	} অন্যথা {
		পূর্ণাঙ্ক max_children = max_jobs;

		যদি (filter_options.choice)
			die(_("--filter can only be used with the remote "
			      "configured in extensions.partialclone"));

		যদি (atomic_fetch)
			die(_("--atomic can only be used when fetching "
			      "from one remote"));

		যদি (stdin_refspecs)
			die(_("--stdin can only be used when fetching "
			      "from one remote"));

		যদি (max_children < 0)
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
	যদি (!result && remote && (config.recurse_submodules != RECURSE_SUBMODULES_OFF)) {
		সংরচনা strvec options = STRVEC_INIT;
		পূর্ণাঙ্ক max_children = max_jobs;

		যদি (max_children < 0)
			max_children = config.submodule_fetch_jobs;
		যদি (max_children < 0)
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
	যদি (negotiate_only)
		গচ্ছ cleanup;

	prepare_repo_settings(the_repository);
	যদি (fetch_write_commit_graph > 0 ||
	    (fetch_write_commit_graph < 0 &&
	     the_repository->settings.fetch_write_commit_graph)) {
		পূর্ণাঙ্ক commit_graph_flags = COMMIT_GRAPH_WRITE_SPLIT;

		যদি (progress)
			commit_graph_flags |= COMMIT_GRAPH_WRITE_PROGRESS;

		যদি (graph_write_mode != GRAPH_WRITE_SKIP) {
			trace2_region_enter("fetch", "write-commit-graph",
					    the_repository);
			যদি (graph_write_mode == GRAPH_WRITE_TIPS)
				write_commit_graph(
					the_repository,
					the_repository->objects->source->path,
					রিক্ত, &updated_tips,
					commit_graph_flags, রিক্ত);
			অন্যথা
				write_commit_graph_reachable(
					the_repository,
					the_repository->objects->source->path,
					commit_graph_flags, রিক্ত);
			trace2_region_leave("fetch", "write-commit-graph",
					    the_repository);
		}
	}

	যদি (enable_auto_gc) {
		যদি (refetch) {
			/*
			 * Hint auto-maintenance strongly to encourage repacking,
			 * but respect config settings disabling it.
			 */
			পূর্ণাঙ্ক opt_val;
			যদি (repo_config_get_int(the_repository, "gc.autopacklimit", &opt_val))
				opt_val = -1;
			যদি (opt_val != 0)
				git_config_ঠেলো_parameter("gc.autoPackLimit=1");

			যদি (repo_config_get_int(the_repository, "maintenance.incremental-repack.auto", &opt_val))
				opt_val = -1;
			যদি (opt_val != 0)
				git_config_ঠেলো_parameter("maintenance.incremental-repack.auto=-1");
		}
		run_auto_maintenance(the_repository, verbosity < 0);
	}

 cleanup:
	oidset_clear(&updated_tips);
	string_list_clear(&list, 0);
	list_objects_filter_release(&filter_options);
	মুক্ত(config.follow_remote_head_raw);
	প্রত্যাগম result;
}
