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

স্থির ধ্রুবক অক্ষর * ধ্রুবক builtin_fetch_usage[] = {
	N_("git fetch [<options>] [<repository> [<refspec>...]]"),
	N_("git fetch [<options>] <group>"),
	N_("git fetch --multiple [<options>] [(<repository>|<group>)...]"),
	N_("git fetch --all [<options>]"),
	ফাঁকা
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

কাঠামো display_state {
	কাঠামো strbuf buf;

	পূর্ণসংখ্যা refcol_width;
	গণনা display_format format;

	অক্ষর *url;
	পূর্ণসংখ্যা url_len, shown_url;
};

স্থির uint64_t forced_updates_ms = 0;
স্থির পূর্ণসংখ্যা prefetch = 0;
স্থির পূর্ণসংখ্যা prune = -1; /* unspecified */
#define PRUNE_BY_DEFAULT 0 /* do we prune by default? */

স্থির পূর্ণসংখ্যা prune_tags = -1; /* unspecified */
#define PRUNE_TAGS_BY_DEFAULT 0 /* do we prune tags by default? */

স্থির পূর্ণসংখ্যা append, dry_run, force, keep, update_শীর্ষ_ok;
স্থির পূর্ণসংখ্যা write_fetch_শীর্ষ = 1;
স্থির পূর্ণসংখ্যা verbosity, deepen_relative, set_upstream, refetch;
স্থির পূর্ণসংখ্যা progress = -1;
স্থির পূর্ণসংখ্যা tags = TAGS_DEFAULT, update_shallow, deepen;
স্থির পূর্ণসংখ্যা atomic_fetch;
স্থির গণনা transport_family family;
স্থির ধ্রুবক অক্ষর *depth;
স্থির ধ্রুবক অক্ষর *deepen_since;
স্থির ধ্রুবক অক্ষর *upload_pack;
স্থির কাঠামো string_list deepen_not = STRING_LIST_INIT_NODUP;
স্থির কাঠামো strbuf default_rla = STRBUF_INIT;
স্থির কাঠামো transport *gtransport;
স্থির কাঠামো transport *gsecondary;
স্থির কাঠামো refspec refmap;
স্থির কাঠামো string_list server_options = STRING_LIST_INIT_DUP;
স্থির কাঠামো string_list negotiation_restrict = STRING_LIST_INIT_NODUP;
স্থির কাঠামো string_list negotiation_include = STRING_LIST_INIT_NODUP;

কাঠামো fetch_config {
	গণনা display_format display_format;
	অক্ষর *follow_remote_শীর্ষ_raw;
	পূর্ণসংখ্যা follow_remote_শীর্ষ_seen;
	পূর্ণসংখ্যা all;
	পূর্ণসংখ্যা prune;
	পূর্ণসংখ্যা prune_tags;
	পূর্ণসংখ্যা show_forced_updates;
	পূর্ণসংখ্যা recurse_submodules;
	পূর্ণসংখ্যা parallel;
	পূর্ণসংখ্যা submodule_fetch_jobs;
};

স্থির পূর্ণসংখ্যা git_fetch_config(ধ্রুবক অক্ষর *k, ধ্রুবক অক্ষর *v,
			    ধ্রুবক কাঠামো config_context *ctx, শূন্য *cb)
{
	কাঠামো fetch_config *fetch_config = cb;

	যদি (!strcmp(k, "fetch.all")) {
		fetch_config->all = git_config_bool(k, v);
		ফেরত 0;
	}

	যদি (!strcmp(k, "fetch.prune")) {
		fetch_config->prune = git_config_bool(k, v);
		ফেরত 0;
	}

	যদি (!strcmp(k, "fetch.prunetags")) {
		fetch_config->prune_tags = git_config_bool(k, v);
		ফেরত 0;
	}

	যদি (!strcmp(k, "fetch.showforcedupdates")) {
		fetch_config->show_forced_updates = git_config_bool(k, v);
		ফেরত 0;
	}

	যদি (!strcmp(k, "submodule.recurse")) {
		পূর্ণসংখ্যা r = git_config_bool(k, v) ?
			RECURSE_SUBMODULES_ON : RECURSE_SUBMODULES_OFF;
		fetch_config->recurse_submodules = r;
		ফেরত 0;
	}

	যদি (!strcmp(k, "submodule.fetchjobs")) {
		fetch_config->submodule_fetch_jobs = parse_submodule_fetchjobs(k, v, ctx->kvi);
		ফেরত 0;
	} অন্যথা যদি (!strcmp(k, "fetch.recursesubmodules")) {
		fetch_config->recurse_submodules = parse_fetch_recurse_submodules_arg(k, v);
		ফেরত 0;
	}

	যদি (!strcmp(k, "fetch.parallel")) {
		fetch_config->parallel = git_config_int(k, v, ctx->kvi);
		যদি (fetch_config->parallel < 0)
			die(_("fetch.parallel cannot be negative"));
		যদি (!fetch_config->parallel)
			fetch_config->parallel = online_cpus();
		ফেরত 0;
	}

	যদি (!strcmp(k, "fetch.output")) {
		যদি (!v)
			ফেরত config_error_nonbool(k);
		অন্যথা যদি (!strcasecmp(v, "full"))
			fetch_config->display_format = DISPLAY_FORMAT_FULL;
		অন্যথা যদি (!strcasecmp(v, "compact"))
			fetch_config->display_format = DISPLAY_FORMAT_COMPACT;
		অন্যথা
			die(_("invalid value for '%s': '%s'"),
			    "fetch.output", v);
		ফেরত 0;
	}

	যদি (!strcmp(k, "fetch.followremotehead")) {
		মুক্ত(fetch_config->follow_remote_শীর্ষ_raw);
		fetch_config->follow_remote_শীর্ষ_raw = xstrdup_or_null(v);
		fetch_config->follow_remote_শীর্ষ_seen = 1;
		ফেরত 0;
	}

	ফেরত git_default_config(k, v, ctx, cb);
}

স্থির গণনা follow_remote_শীর্ষ_settings get_follow_remote_শীর্ষ(ধ্রুবক অক্ষর *setting)
{
	যদি (!setting)
		die(_("missing value for 'fetch.followRemoteHEAD'"));
	অন্যথা যদি (!strcmp(setting, "never"))
		ফেরত FOLLOW_REMOTE_NEVER;
	অন্যথা যদি (!strcmp(setting, "create"))
		ফেরত FOLLOW_REMOTE_CREATE;
	অন্যথা যদি (!strcmp(setting, "warn"))
		ফেরত FOLLOW_REMOTE_WARN;
	অন্যথা যদি (!strcmp(setting, "always"))
		ফেরত FOLLOW_REMOTE_ALWAYS;
	warning(_("unrecognized fetch.followRemoteHEAD value '%s' ignored"), setting);
	ফেরত BUILTIN_FOLLOW_REMOTE_শীর্ষ_DFLT;
}

স্থির পূর্ণসংখ্যা parse_refmap_arg(ধ্রুবক কাঠামো option *opt, ধ্রুবক অক্ষর *arg, পূর্ণসংখ্যা unset)
{
	BUG_ON_OPT_NEG(unset);

	/*
	 * "git fetch --refmap='' origin foo"
	 * can be used to tell the command not to store anywhere
	 */
	refspec_append(opt->মান, arg);

	ফেরত 0;
}

স্থির শূন্য unlock_pack(unsigned পূর্ণসংখ্যা flags)
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

স্থির শূন্য unlock_pack_on_signal(পূর্ণসংখ্যা signo)
{
	unlock_pack(TRANSPORT_UNLOCK_PACK_IN_SIGNAL_HANDLER);
	sigchain_বের_করো(signo);
	তোলো(signo);
}

স্থির শূন্য add_merge_config(কাঠামো ref **শীর্ষ,
			   ধ্রুবক কাঠামো ref *remote_refs,
		           কাঠামো branch *branch,
		           কাঠামো ref ***লেজ)
{
	পূর্ণসংখ্যা i;

	জন্য (i = 0; i < branch->merge_nr; i++) {
		কাঠামো ref *rm, **old_লেজ = *লেজ;
		কাঠামো refspec_item refspec;

		জন্য (rm = *শীর্ষ; rm; rm = rm->পরবর্তী) {
			যদি (branch_merge_matches(branch, i, rm->name)) {
				rm->fetch_শীর্ষ_status = FETCH_শীর্ষ_MERGE;
				বিরতি;
			}
		}
		যদি (rm)
			চালু;

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
		memset(&refspec, 0, আকার(refspec));
		refspec.src = branch->merge[i]->src;
		get_fetch_map(remote_refs, &refspec, লেজ, 1);
		জন্য (rm = *old_লেজ; rm; rm = rm->পরবর্তী)
			rm->fetch_শীর্ষ_status = FETCH_শীর্ষ_MERGE;
	}
}

স্থির শূন্য create_fetch_oidset(কাঠামো ref **শীর্ষ, কাঠামো oidset *out)
{
	কাঠামো ref *rm = *শীর্ষ;
	যখন (rm) {
		oidset_ঢোকাও(out, &rm->old_oid);
		rm = rm->পরবর্তী;
	}
}

কাঠামো refname_hash_entry {
	কাঠামো hashmap_entry ent;
	কাঠামো object_id oid;
	পূর্ণসংখ্যা ignore;
	অক্ষর refname[FLEX_ARRAY];
};

স্থির পূর্ণসংখ্যা refname_hash_entry_cmp(ধ্রুবক শূন্য *hashmap_cmp_fn_তথ্য UNUSED,
				  ধ্রুবক কাঠামো hashmap_entry *eptr,
				  ধ্রুবক কাঠামো hashmap_entry *entry_or_চাবি,
				  ধ্রুবক শূন্য *keydata)
{
	ধ্রুবক কাঠামো refname_hash_entry *e1, *e2;

	e1 = container_of(eptr, ধ্রুবক কাঠামো refname_hash_entry, ent);
	e2 = container_of(entry_or_চাবি, ধ্রুবক কাঠামো refname_hash_entry, ent);
	ফেরত strcmp(e1->refname, keydata ? keydata : e2->refname);
}

স্থির কাঠামো refname_hash_entry *refname_hash_add(কাঠামো hashmap *map,
						   ধ্রুবক অক্ষর *refname,
						   ধ্রুবক কাঠামো object_id *oid)
{
	কাঠামো refname_hash_entry *ent;
	আকার_t len = strlen(refname);

	FLEX_ALLOC_MEM(ent, refname, refname, len);
	hashmap_entry_init(&ent->ent, strhash(refname));
	oidcpy(&ent->oid, oid);
	hashmap_add(map, &ent->ent);
	ফেরত ent;
}

স্থির পূর্ণসংখ্যা add_one_refname(ধ্রুবক কাঠামো reference *ref, শূন্য *cbdata)
{
	কাঠামো hashmap *refname_map = cbdata;

	(শূন্য) refname_hash_add(refname_map, ref->name, ref->oid);
	ফেরত 0;
}

স্থির শূন্য refname_hash_init(কাঠামো hashmap *map)
{
	hashmap_init(map, refname_hash_entry_cmp, ফাঁকা, 0);
}

স্থির পূর্ণসংখ্যা refname_hash_exists(কাঠামো hashmap *map, ধ্রুবক অক্ষর *refname)
{
	ফেরত !!hashmap_get_from_hash(map, strhash(refname), refname);
}

স্থির শূন্য clear_item(কাঠামো refname_hash_entry *item)
{
	item->ignore = 1;
}


স্থির শূন্য add_already_queued_tags(ধ্রুবক অক্ষর *refname,
				    ধ্রুবক কাঠামো object_id *old_oid UNUSED,
				    ধ্রুবক কাঠামো object_id *new_oid,
				    শূন্য *cb_তথ্য)
{
	কাঠামো hashmap *queued_tags = cb_তথ্য;
	যদি (starts_with(refname, "refs/tags/") && new_oid)
		(শূন্য) refname_hash_add(queued_tags, refname, new_oid);
}

স্থির শূন্য find_non_local_tags(ধ্রুবক কাঠামো ref *refs,
				কাঠামো ref_transaction *transaction,
				কাঠামো ref **শীর্ষ,
				কাঠামো ref ***লেজ)
{
	কাঠামো hashmap existing_refs;
	কাঠামো hashmap remote_refs;
	কাঠামো oidset fetch_oids = OIDSET_INIT;
	কাঠামো string_list remote_refs_list = STRING_LIST_INIT_NODUP;
	কাঠামো string_list_item *remote_ref_item;
	ধ্রুবক কাঠামো ref *ref;
	কাঠামো refname_hash_entry *item = ফাঁকা;

	refname_hash_init(&existing_refs);
	refname_hash_init(&remote_refs);
	create_fetch_oidset(শীর্ষ, &fetch_oids);

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

	জন্য (ref = refs; ref; ref = ref->পরবর্তী) {
		যদি (!starts_with(ref->name, "refs/tags/"))
			চালু;

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
			item = ফাঁকা;
			চালু;
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

		item = ফাঁকা;

		/* skip duplicates and refs that we already have */
		যদি (refname_hash_exists(&remote_refs, ref->name) ||
		    refname_hash_exists(&existing_refs, ref->name))
			চালু;

		item = refname_hash_add(&remote_refs, ref->name, &ref->old_oid);
		string_list_ঢোকাও(&remote_refs_list, ref->name);
	}
	hashmap_clear_and_free(&existing_refs, কাঠামো refname_hash_entry, ent);

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
		ধ্রুবক অক্ষর *refname = remote_ref_item->string;
		কাঠামো ref *rm;
		unsigned পূর্ণসংখ্যা hash = strhash(refname);

		item = hashmap_get_entry_from_hash(&remote_refs, hash, refname,
					কাঠামো refname_hash_entry, ent);
		যদি (!item)
			BUG("unseen remote ref?");

		/* Unless we have already decided to ignore this item... */
		যদি (item->ignore)
			চালু;

		rm = alloc_ref(item->refname);
		rm->peer_ref = alloc_ref(item->refname);
		oidcpy(&rm->old_oid, &item->oid);
		**লেজ = rm;
		*লেজ = &rm->পরবর্তী;
	}
	hashmap_clear_and_free(&remote_refs, কাঠামো refname_hash_entry, ent);
	string_list_clear(&remote_refs_list, 0);
	oidset_clear(&fetch_oids);
}

স্থির শূন্য filter_prefetch_refspec(কাঠামো refspec *rs)
{
	পূর্ণসংখ্যা i;

	যদি (!prefetch)
		ফেরত;

	জন্য (i = 0; i < rs->nr; i++) {
		কাঠামো strbuf new_dst = STRBUF_INIT;
		অক্ষর *old_dst;
		ধ্রুবক অক্ষর *sub = ফাঁকা;

		যদি (rs->বস্তুসমূহ[i].negative)
			চালু;
		যদি (!rs->বস্তুসমূহ[i].dst ||
		    (rs->বস্তুসমূহ[i].src &&
		     starts_with(rs->বস্তুসমূহ[i].src,
				 ref_namespace[NAMESPACE_TAGS].ref))) {
			পূর্ণসংখ্যা j;

			refspec_item_clear(&rs->বস্তুসমূহ[i]);

			জন্য (j = i + 1; j < rs->nr; j++)
				rs->বস্তুসমূহ[j - 1] = rs->বস্তুসমূহ[j];
			rs->nr--;
			i--;
			চালু;
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

		rs->বস্তুসমূহ[i].dst = strbuf_detach(&new_dst, ফাঁকা);
		rs->বস্তুসমূহ[i].force = 1;

		মুক্ত(old_dst);
	}
}

স্থির কাঠামো ref *get_ref_map(কাঠামো remote *remote,
			       ধ্রুবক কাঠামো ref *remote_refs,
			       কাঠামো refspec *rs,
			       পূর্ণসংখ্যা tags, পূর্ণসংখ্যা *autotags)
{
	পূর্ণসংখ্যা i;
	কাঠামো ref *rm;
	কাঠামো ref *ref_map = ফাঁকা;
	কাঠামো ref **লেজ = &ref_map;

	/* opportunistically-updated references: */
	কাঠামো ref *orefs = ফাঁকা, **oref_লেজ = &orefs;

	কাঠামো hashmap existing_refs;
	পূর্ণসংখ্যা existing_refs_populated = 0;

	filter_prefetch_refspec(rs);
	যদি (remote)
		filter_prefetch_refspec(&remote->fetch);

	যদি (rs->nr) {
		কাঠামো refspec *fetch_refspec;

		জন্য (i = 0; i < rs->nr; i++) {
			get_fetch_map(remote_refs, &rs->বস্তুসমূহ[i], &লেজ, 0);
			যদি (rs->বস্তুসমূহ[i].dst && rs->বস্তুসমূহ[i].dst[0])
				*autotags = 1;
		}
		/* Merge everything on the command line (but not --tags) */
		জন্য (rm = ref_map; rm; rm = rm->পরবর্তী)
			rm->fetch_শীর্ষ_status = FETCH_শীর্ষ_MERGE;

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

		জন্য (i = 0; i < fetch_refspec->nr; i++)
			get_fetch_map(ref_map, &fetch_refspec->বস্তুসমূহ[i], &oref_লেজ, 1);
	} অন্যথা যদি (refmap.nr) {
		die("--refmap option is only meaningful with command-line refspec(s)");
	} অন্যথা {
		/* Use the defaults */
		কাঠামো branch *branch = branch_get(ফাঁকা);
		পূর্ণসংখ্যা has_merge = branch_has_merge_config(branch);
		যদি (remote &&
		    (remote->fetch.nr ||
		     /* Note: has_merge implies non-NULL branch->remote_name */
		     (has_merge && !strcmp(branch->remote_name, remote->name)))) {
			জন্য (i = 0; i < remote->fetch.nr; i++) {
				get_fetch_map(remote_refs, &remote->fetch.বস্তুসমূহ[i], &লেজ, 0);
				যদি (remote->fetch.বস্তুসমূহ[i].dst &&
				    remote->fetch.বস্তুসমূহ[i].dst[0])
					*autotags = 1;
				যদি (!i && !has_merge && ref_map &&
				    !remote->fetch.বস্তুসমূহ[0].pattern)
					ref_map->fetch_শীর্ষ_status = FETCH_শীর্ষ_MERGE;
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
				add_merge_config(&ref_map, remote_refs, branch, &লেজ);
		} অন্যথা যদি (!prefetch) {
			ref_map = get_remote_ref(remote_refs, "HEAD");
			যদি (!ref_map)
				die(_("couldn't find remote ref HEAD"));
			ref_map->fetch_শীর্ষ_status = FETCH_শীর্ষ_MERGE;
			লেজ = &ref_map->পরবর্তী;
		}
	}

	যদি (tags == TAGS_SET) {
		কাঠামো refspec_item tag_refspec;

		/* also fetch all tags */
		refspec_item_init_ঠেলো(&tag_refspec, TAG_REFSPEC,
				       the_hash_algo);
		get_fetch_map(remote_refs, &tag_refspec, &লেজ, 0);
		refspec_item_clear(&tag_refspec);
	} অন্যথা যদি (tags == TAGS_DEFAULT && *autotags) {
		find_non_local_tags(remote_refs, ফাঁকা, &ref_map, &লেজ);
	}

	/* Now append any refs to be updated opportunistically: */
	*লেজ = orefs;
	জন্য (rm = orefs; rm; rm = rm->পরবর্তী) {
		rm->fetch_শীর্ষ_status = FETCH_শীর্ষ_IGNORE;
		লেজ = &rm->পরবর্তী;
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

	জন্য (rm = ref_map; rm; rm = rm->পরবর্তী) {
		যদি (rm->peer_ref) {
			ধ্রুবক অক্ষর *refname = rm->peer_ref->name;
			কাঠামো refname_hash_entry *peer_item;
			unsigned পূর্ণসংখ্যা hash = strhash(refname);

			যদি (!existing_refs_populated) {
				refname_hash_init(&existing_refs);
				refs_for_each_ref(get_main_ref_store(the_repository),
						  add_one_refname,
						  &existing_refs);
				existing_refs_populated = 1;
			}

			peer_item = hashmap_get_entry_from_hash(&existing_refs,
						hash, refname,
						কাঠামো refname_hash_entry, ent);
			যদি (peer_item) {
				কাঠামো object_id *old_oid = &peer_item->oid;
				oidcpy(&rm->peer_ref->old_oid, old_oid);
			}
		}
	}
	যদি (existing_refs_populated)
		hashmap_clear_and_free(&existing_refs, কাঠামো refname_hash_entry, ent);

	ফেরত ref_map;
}

স্থির পূর্ণসংখ্যা s_update_ref(ধ্রুবক অক্ষর *action,
			কাঠামো ref *ref,
			কাঠামো ref_transaction *transaction,
			পূর্ণসংখ্যা check_old)
{
	অক্ষর *বার্তা;
	অক্ষর *rla = getenv("GIT_REFLOG_ACTION");
	কাঠামো strbuf err = STRBUF_INIT;
	পূর্ণসংখ্যা ret;

	যদি (dry_run)
		ফেরত 0;
	যদি (!rla)
		rla = default_rla.buf;
	বার্তা = xstrfmt("%s: %s", rla, action);

	ret = ref_transaction_update(transaction, ref->name, &ref->new_oid,
				     check_old ? &ref->old_oid : ফাঁকা,
				     ফাঁকা, ফাঁকা, 0, বার্তা, &err);

	যদি (ret)
		error("%s", err.buf);
	strbuf_release(&err);
	মুক্ত(বার্তা);
	ফেরত ret;
}

স্থির পূর্ণসংখ্যা refcol_width(ধ্রুবক কাঠামো ref *ref_map, পূর্ণসংখ্যা compact_format)
{
	ধ্রুবক কাঠামো ref *ref;
	পূর্ণসংখ্যা max, width = 10;

	max = term_columns();
	যদি (compact_format)
		max = max * 2 / 3;

	জন্য (ref = ref_map; ref; ref = ref->পরবর্তী) {
		পূর্ণসংখ্যা rlen, llen = 0, len;

		যদি (ref->status == REF_STATUS_REJECT_SHALLOW ||
		    !ref->peer_ref ||
		    !strcmp(ref->name, "HEAD"))
			চালু;

		/* uptodate lines are only shown on high verbosity level */
		যদি (verbosity <= 0 && oideq(&ref->peer_ref->old_oid, &ref->old_oid))
			চালু;

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
			চালু;

		যদি (width < rlen)
			width = rlen;
	}

	ফেরত width;
}

স্থির শূন্য display_state_init(কাঠামো display_state *display_state, কাঠামো ref *ref_map,
			       ধ্রুবক অক্ষর *raw_url, গণনা display_format format)
{
	পূর্ণসংখ্যা i;

	memset(display_state, 0, আকার(*display_state));
	strbuf_init(&display_state->buf, 0);
	display_state->format = format;

	যদি (raw_url)
		display_state->url = transport_anonymize_url(raw_url);
	অন্যথা
		display_state->url = xstrdup("foreign");

	display_state->url_len = strlen(display_state->url);
	জন্য (i = display_state->url_len - 1; 0 <= i && display_state->url[i] == '/'; i--)
		;
	display_state->url_len = i + 1;
	যদি (4 < i && !strncmp(".git", display_state->url + i - 3, 4))
		display_state->url_len = i - 3;

	যদি (verbosity < 0)
		ফেরত;

	নির্বাচন (display_state->format) {
	ক্ষেত্র DISPLAY_FORMAT_FULL:
	ক্ষেত্র DISPLAY_FORMAT_COMPACT:
		display_state->refcol_width = refcol_width(ref_map,
							   display_state->format == DISPLAY_FORMAT_COMPACT);
		বিরতি;
	ক্ষেত্র DISPLAY_FORMAT_PORCELAIN:
		/* We don't need to precompute anything here. */
		বিরতি;
	পূর্বনির্ধারিত:
		BUG("unexpected display format %d", display_state->format);
	}
}

স্থির শূন্য display_state_release(কাঠামো display_state *display_state)
{
	strbuf_release(&display_state->buf);
	মুক্ত(display_state->url);
}

স্থির শূন্য print_remote_to_local(কাঠামো display_state *display_state,
				  ধ্রুবক অক্ষর *remote, ধ্রুবক অক্ষর *local)
{
	strbuf_addf(&display_state->buf, "%-*s -> %s",
		    display_state->refcol_width, remote, local);
}

স্থির পূর্ণসংখ্যা find_and_replace(কাঠামো strbuf *haystack,
			    ধ্রুবক অক্ষর *needle,
			    ধ্রুবক অক্ষর *placeholder)
{
	ধ্রুবক অক্ষর *p = ফাঁকা;
	পূর্ণসংখ্যা plen, nlen;

	nlen = strlen(needle);
	যদি (ends_with(haystack->buf, needle))
		p = haystack->buf + haystack->len - nlen;
	অন্যথা
		p = strstr(haystack->buf, needle);
	যদি (!p)
		ফেরত 0;

	যদি (p > haystack->buf && p[-1] != '/')
		ফেরত 0;

	plen = strlen(p);
	যদি (plen > nlen && p[nlen] != '/')
		ফেরত 0;

	strbuf_splice(haystack, p - haystack->buf, nlen,
		      placeholder, strlen(placeholder));
	ফেরত 1;
}

স্থির শূন্য print_compact(কাঠামো display_state *display_state,
			  ধ্রুবক অক্ষর *remote, ধ্রুবক অক্ষর *local)
{
	কাঠামো strbuf r = STRBUF_INIT;
	কাঠামো strbuf l = STRBUF_INIT;

	যদি (!strcmp(remote, local)) {
		strbuf_addf(&display_state->buf, "%-*s -> *", display_state->refcol_width, remote);
		ফেরত;
	}

	strbuf_addstr(&r, remote);
	strbuf_addstr(&l, local);

	যদি (!find_and_replace(&r, local, "*"))
		find_and_replace(&l, remote, "*");
	print_remote_to_local(display_state, r.buf, l.buf);

	strbuf_release(&r);
	strbuf_release(&l);
}

স্থির শূন্য display_ref_update(কাঠামো display_state *display_state, অক্ষর code,
			       ধ্রুবক অক্ষর *summary, ধ্রুবক অক্ষর *error,
			       ধ্রুবক অক্ষর *remote, ধ্রুবক অক্ষর *local,
			       ধ্রুবক কাঠামো object_id *old_oid,
			       ধ্রুবক কাঠামো object_id *new_oid,
			       পূর্ণসংখ্যা summary_width)
{
	FILE *f = stderr;

	যদি (verbosity < 0)
		ফেরত;

	strbuf_reset(&display_state->buf);

	নির্বাচন (display_state->format) {
	ক্ষেত্র DISPLAY_FORMAT_FULL:
	ক্ষেত্র DISPLAY_FORMAT_COMPACT: {
		পূর্ণসংখ্যা width;

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

		বিরতি;
	}
	ক্ষেত্র DISPLAY_FORMAT_PORCELAIN:
		strbuf_addf(&display_state->buf, "%c %s %s %s", code,
			    oid_to_hex(old_oid), oid_to_hex(new_oid), local);
		f = stdout;
		বিরতি;
	পূর্বনির্ধারিত:
		BUG("unexpected display format %d", display_state->format);
	};
	strbuf_addch(&display_state->buf, '\n');

	fputs(display_state->buf.buf, f);
}

কাঠামো ref_update_display_info {
	bool failed;
	অক্ষর success_code;
	অক্ষর fail_code;
	অক্ষর *summary;
	অক্ষর *fail_detail;
	অক্ষর *success_detail;
	অক্ষর *ref;
	অক্ষর *remote;
	কাঠামো object_id old_oid;
	কাঠামো object_id new_oid;
};

কাঠামো ref_update_display_info_array {
	কাঠামো ref_update_display_info *info;
	আকার_t alloc, nr;
};

স্থির কাঠামো ref_update_display_info *ref_update_display_info_append(
					   কাঠামো ref_update_display_info_array *array,
					   অক্ষর success_code,
					   অক্ষর fail_code,
					   ধ্রুবক অক্ষর *summary,
					   ধ্রুবক অক্ষর *success_detail,
					   ধ্রুবক অক্ষর *fail_detail,
					   ধ্রুবক অক্ষর *ref,
					   ধ্রুবক অক্ষর *remote,
					   ধ্রুবক কাঠামো object_id *old_oid,
					   ধ্রুবক কাঠামো object_id *new_oid)
{
	কাঠামো ref_update_display_info *info;

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

	ফেরত info;
}

স্থির শূন্য ref_update_display_info_set_failed(কাঠামো ref_update_display_info *info)
{
	info->failed = true;
}

স্থির শূন্য ref_update_display_info_free(কাঠামো ref_update_display_info *info)
{
	মুক্ত(info->summary);
	মুক্ত(info->success_detail);
	মুক্ত(info->fail_detail);
	মুক্ত(info->remote);
	মুক্ত(info->ref);
}

স্থির শূন্য ref_update_display_info_display(কাঠামো ref_update_display_info *info,
					    কাঠামো display_state *display_state,
					    পূর্ণসংখ্যা summary_width)
{
	display_ref_update(display_state,
			   info->failed ? info->fail_code : info->success_code,
			   info->summary,
			   info->failed ? info->fail_detail : info->success_detail,
			   info->remote, info->ref, &info->old_oid,
			   &info->new_oid, summary_width);
}

স্থির পূর্ণসংখ্যা update_local_ref(কাঠামো ref *ref,
			    কাঠামো ref_transaction *transaction,
			    ধ্রুবক কাঠামো ref *remote_ref,
			    ধ্রুবক কাঠামো fetch_config *config,
			    কাঠামো ref_update_display_info_array *display_array)
{
	কাঠামো commit *current = ফাঁকা, *updated;
	পূর্ণসংখ্যা fast_forward = 0;

	যদি (!odb_has_object(the_repository->objects, &ref->new_oid,
			    ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR))
		die(_("object %s not found"), oid_to_hex(&ref->new_oid));

	যদি (oideq(&ref->old_oid, &ref->new_oid)) {
		যদি (verbosity > 0)
			ref_update_display_info_append(display_array, '=', '=',
						       _("[up to date]"), ফাঁকা,
						       ফাঁকা, ref->name,
						       remote_ref->name, &ref->old_oid,
						       &ref->new_oid);
		ফেরত 0;
	}

	যদি (!update_শীর্ষ_ok &&
	    !is_null_oid(&ref->old_oid) &&
	    branch_checked_out(ref->name)) {
		কাঠামো ref_update_display_info *info;
		/*
		 * If this is the head, and it's not okay to update
		 * the head, and the old value of the head isn't empty...
		 */
		info = ref_update_display_info_append(display_array, '!', '!',
						      _("[rejected]"), ফাঁকা,
						      _("can't fetch into checked-out branch"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);
		ref_update_display_info_set_failed(info);
		ফেরত 1;
	}

	যদি (!is_null_oid(&ref->old_oid) &&
	    starts_with(ref->name, "refs/tags/")) {
		কাঠামো ref_update_display_info *info;

		যদি (force || ref->force) {
			পূর্ণসংখ্যা r;

			r = s_update_ref("updating tag", ref, transaction, 0);

			info = ref_update_display_info_append(display_array, 't', '!',
							      _("[tag update]"), ফাঁকা,
							      _("unable to update local ref"),
							      ref->name, remote_ref->name,
							      &ref->old_oid, &ref->new_oid);
			যদি (r)
				ref_update_display_info_set_failed(info);

			ফেরত r;
		} অন্যথা {
			info = ref_update_display_info_append(display_array, '!', '!',
							      _("[rejected]"), ফাঁকা,
							      _("would clobber existing tag"),
							      ref->name, remote_ref->name,
							      &ref->old_oid, &ref->new_oid);
			ref_update_display_info_set_failed(info);
			ফেরত 1;
		}
	}

	current = lookup_commit_reference_gently(the_repository,
						 &ref->old_oid, 1);
	updated = lookup_commit_reference_gently(the_repository,
						 &ref->new_oid, 1);
	যদি (!current || !updated) {
		কাঠামো ref_update_display_info *info;
		ধ্রুবক অক্ষর *বার্তা;
		ধ্রুবক অক্ষর *what;
		পূর্ণসংখ্যা r;
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
						      what, ফাঁকা,
						      _("unable to update local ref"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);
		যদি (r)
			ref_update_display_info_set_failed(info);

		ফেরত r;
	}

	যদি (config->show_forced_updates) {
		uint64_t t_before = getnanotime();
		fast_forward = repo_in_merge_bases(the_repository, current,
						   updated);
		যদি (fast_forward < 0)
			die(ফাঁকা);
		forced_updates_ms += (getnanotime() - t_before) / 1000000;
	} অন্যথা {
		fast_forward = 1;
	}

	যদি (fast_forward) {
		কাঠামো ref_update_display_info *info;
		কাঠামো strbuf quickref = STRBUF_INIT;
		পূর্ণসংখ্যা r;

		strbuf_add_unique_abbrev(&quickref, &current->object.oid, DEFAULT_ABBREV);
		strbuf_addstr(&quickref, "..");
		strbuf_add_unique_abbrev(&quickref, &ref->new_oid, DEFAULT_ABBREV);
		r = s_update_ref("fast-forward", ref, transaction, 1);

		info = ref_update_display_info_append(display_array, ' ', '!',
						      quickref.buf, ফাঁকা,
						      _("unable to update local ref"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);
		যদি (r)
			ref_update_display_info_set_failed(info);

		strbuf_release(&quickref);
		ফেরত r;
	} অন্যথা যদি (force || ref->force) {
		কাঠামো ref_update_display_info *info;
		কাঠামো strbuf quickref = STRBUF_INIT;
		পূর্ণসংখ্যা r;

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
		ফেরত r;
	} অন্যথা {
		কাঠামো ref_update_display_info *info;
		info = ref_update_display_info_append(display_array, '!', '!',
						      _("[rejected]"), ফাঁকা,
						      _("non-fast-forward"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);
		ref_update_display_info_set_failed(info);
		ফেরত 1;
	}
}

স্থির ধ্রুবক কাঠামো object_id *iterate_ref_map(শূন্য *cb_তথ্য)
{
	কাঠামো ref **rm = cb_তথ্য;
	কাঠামো ref *ref = *rm;

	যখন (ref && ref->status == REF_STATUS_REJECT_SHALLOW)
		ref = ref->পরবর্তী;
	যদি (!ref)
		ফেরত ফাঁকা;
	*rm = ref->পরবর্তী;
	ফেরত &ref->old_oid;
}

কাঠামো fetch_শীর্ষ {
	FILE *fp;
	কাঠামো strbuf buf;
};

স্থির পূর্ণসংখ্যা open_fetch_শীর্ষ(কাঠামো fetch_শীর্ষ *fetch_শীর্ষ)
{
	ধ্রুবক অক্ষর *filename = git_path_fetch_শীর্ষ(the_repository);

	যদি (write_fetch_শীর্ষ) {
		fetch_শীর্ষ->fp = fopen(filename, "a");
		যদি (!fetch_শীর্ষ->fp)
			ফেরত error_errno(_("cannot open '%s'"), filename);
		strbuf_init(&fetch_শীর্ষ->buf, 0);
	} অন্যথা {
		fetch_শীর্ষ->fp = ফাঁকা;
	}

	ফেরত 0;
}

স্থির শূন্য append_fetch_শীর্ষ(কাঠামো fetch_শীর্ষ *fetch_শীর্ষ,
			      ধ্রুবক কাঠামো object_id *old_oid,
			      গণনা fetch_শীর্ষ_status fetch_শীর্ষ_status,
			      ধ্রুবক অক্ষর *note,
			      ধ্রুবক অক্ষর *url, আকার_t url_len)
{
	অক্ষর old_oid_hex[GIT_MAX_HEXSZ + 1];
	ধ্রুবক অক্ষর *merge_status_marker;
	আকার_t i;

	যদি (!fetch_শীর্ষ->fp)
		ফেরত;

	নির্বাচন (fetch_শীর্ষ_status) {
	ক্ষেত্র FETCH_শীর্ষ_NOT_FOR_MERGE:
		merge_status_marker = "not-for-merge";
		বিরতি;
	ক্ষেত্র FETCH_শীর্ষ_MERGE:
		merge_status_marker = "";
		বিরতি;
	পূর্বনির্ধারিত:
		/* do not write anything to FETCH_HEAD */
		ফেরত;
	}

	strbuf_addf(&fetch_শীর্ষ->buf, "%s\t%s\t%s",
		    oid_to_hex_r(old_oid_hex, old_oid), merge_status_marker, note);
	জন্য (i = 0; i < url_len; ++i)
		যদি ('\n' == url[i])
			strbuf_addstr(&fetch_শীর্ষ->buf, "\\n");
		অন্যথা
			strbuf_addch(&fetch_শীর্ষ->buf, url[i]);
	strbuf_addch(&fetch_শীর্ষ->buf, '\n');

	/*
	 * When using an atomic fetch, we do not want to update FETCH_HEAD if
	 * any of the reference updates fails. We thus have to write all
	 * updates to a buffer first and only commit it as soon as all
	 * references have been successfully updated.
	 */
	যদি (!atomic_fetch) {
		strbuf_write(&fetch_শীর্ষ->buf, fetch_শীর্ষ->fp);
		strbuf_reset(&fetch_শীর্ষ->buf);
	}
}

স্থির শূন্য commit_fetch_শীর্ষ(কাঠামো fetch_শীর্ষ *fetch_শীর্ষ)
{
	যদি (!fetch_শীর্ষ->fp || !atomic_fetch)
		ফেরত;
	strbuf_write(&fetch_শীর্ষ->buf, fetch_শীর্ষ->fp);
}

স্থির শূন্য close_fetch_শীর্ষ(কাঠামো fetch_শীর্ষ *fetch_শীর্ষ)
{
	যদি (!fetch_শীর্ষ->fp)
		ফেরত;

	fclose(fetch_শীর্ষ->fp);
	strbuf_release(&fetch_শীর্ষ->buf);
}

স্থির ধ্রুবক অক্ষর warn_show_forced_updates[] =
N_("fetch normally indicates which branches had a forced update,\n"
   "but that check has been disabled; to re-enable, use '--show-forced-updates'\n"
   "flag or run 'git config fetch.showForcedUpdates true'");
স্থির ধ্রুবক অক্ষর warn_time_show_forced_updates[] =
N_("it took %.2f seconds to check forced updates; you can use\n"
   "'--no-show-forced-updates' or run 'git config fetch.showForcedUpdates false'\n"
   "to avoid this check\n");

স্থির পূর্ণসংখ্যা store_updated_refs(কাঠামো display_state *display_state,
			      পূর্ণসংখ্যা connectivity_checked,
			      কাঠামো ref_transaction *transaction, কাঠামো ref *ref_map,
			      কাঠামো fetch_শীর্ষ *fetch_শীর্ষ,
			      ধ্রুবক কাঠামো fetch_config *config,
			      কাঠামো ref_update_display_info_array *display_array)
{
	পূর্ণসংখ্যা rc = 0;
	কাঠামো strbuf note = STRBUF_INIT;
	ধ্রুবক অক্ষর *what, *kind;
	কাঠামো ref *rm;
	পূর্ণসংখ্যা want_status;

	যদি (!connectivity_checked) {
		কাঠামো check_connected_options opt = CHECK_CONNECTED_INIT;

		opt.exclude_hidden_refs_section = "fetch";
		rm = ref_map;
		যদি (check_connected(iterate_ref_map, &rm, &opt)) {
			rc = error(_("%s did not send all necessary objects"),
				   display_state->url);
			যাও abort;
		}
	}

	/*
	 * We do a pass for each fetch_head_status type in their enum order, so
	 * merged entries are written before not-for-merge. That lets readers
	 * use FETCH_HEAD as a refname to refer to the ref to be merged.
	 */
	জন্য (want_status = FETCH_শীর্ষ_MERGE;
	     want_status <= FETCH_শীর্ষ_IGNORE;
	     want_status++) {
		জন্য (rm = ref_map; rm; rm = rm->পরবর্তী) {
			কাঠামো ref *ref = ফাঁকা;

			যদি (rm->status == REF_STATUS_REJECT_SHALLOW) {
				যদি (want_status == FETCH_শীর্ষ_MERGE)
					warning(_("rejected %s because shallow roots are not allowed to be updated"),
						rm->peer_ref ? rm->peer_ref->name : rm->name);
				চালু;
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
			যদি (fetch_শীর্ষ->fp) {
				কাঠামো commit *commit = ফাঁকা;

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
						rm->fetch_শীর্ষ_status = FETCH_শীর্ষ_NOT_FOR_MERGE;
				}
			}

			যদি (rm->fetch_শীর্ষ_status != want_status)
				চালু;

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

			append_fetch_শীর্ষ(fetch_শীর্ষ, &rm->old_oid,
					  rm->fetch_শীর্ষ_status,
					  note.buf, display_state->url,
					  display_state->url_len);

			যদি (ref) {
				rc |= update_local_ref(ref, transaction, rm,
						       config, display_array);
				মুক্ত(ref);
			} অন্যথা যদি (write_fetch_শীর্ষ || dry_run) {
				/*
				 * Display fetches written to FETCH_HEAD (or
				 * would be written to FETCH_HEAD, if --dry-run
				 * is set).
				 */

				ref_update_display_info_append(display_array, '*', '*',
							       *kind ? kind : "branch",
							       ফাঁকা, ফাঁকা, "FETCH_HEAD",
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
	ফেরত rc;
}

/*
 * We would want to bypass the object transfer altogether if
 * everything we are going to fetch already exists and is connected
 * locally.
 */
স্থির পূর্ণসংখ্যা check_exist_and_connected(কাঠামো ref *ref_map)
{
	কাঠামো ref *rm = ref_map;
	কাঠামো check_connected_options opt = CHECK_CONNECTED_INIT;
	কাঠামো ref *r;

	/*
	 * If we are deepening a shallow clone we already have these
	 * objects reachable.  Running rev-list here will return with
	 * a good (0) exit status and we'll bypass the fetch that we
	 * really need to perform.  Claiming failure now will ensure
	 * we perform the network exchange to deepen our history.
	 */
	যদি (deepen)
		ফেরত -1;

	/*
	 * Similarly, if we need to refetch, we always want to perform a full
	 * fetch ignoring existing objects.
	 */
	যদি (refetch)
		ফেরত -1;


	/*
	 * check_connected() allows objects to merely be promised, but
	 * we need all direct targets to exist.
	 */
	জন্য (r = rm; r; r = r->পরবর্তী) {
		যদি (!odb_has_object(the_repository->objects, &r->old_oid,
				    ODB_HAS_OBJECT_RECHECK_PACKED))
			ফেরত -1;
	}

	opt.quiet = 1;
	opt.exclude_hidden_refs_section = "fetch";
	ফেরত check_connected(iterate_ref_map, &rm, &opt);
}

স্থির পূর্ণসংখ্যা fetch_and_consume_refs(কাঠামো display_state *display_state,
				  কাঠামো transport *transport,
				  কাঠামো ref_transaction *transaction,
				  কাঠামো ref *ref_map,
				  কাঠামো fetch_শীর্ষ *fetch_শীর্ষ,
				  ধ্রুবক কাঠামো fetch_config *config,
				  কাঠামো ref_update_display_info_array *display_array)
{
	পূর্ণসংখ্যা connectivity_checked = 1;
	পূর্ণসংখ্যা ret;

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
			যাও out;
		connectivity_checked = transport->smart_options ?
			transport->smart_options->connectivity_checked : 0;
	}

	trace2_region_enter("fetch", "consume_refs", the_repository);
	ret = store_updated_refs(display_state, connectivity_checked,
				 transaction, ref_map, fetch_শীর্ষ, config,
				 display_array);
	trace2_region_leave("fetch", "consume_refs", the_repository);

out:
	transport_unlock_pack(transport, 0);
	ফেরত ret;
}

স্থির পূর্ণসংখ্যা prune_refs(কাঠামো display_state *display_state,
		      কাঠামো refspec *rs,
		      কাঠামো ref_transaction *transaction,
		      কাঠামো ref *ref_map)
{
	পূর্ণসংখ্যা ফলাফল = 0;
	কাঠামো ref *ref, *stale_refs = get_stale_heads(rs, ref_map);
	কাঠামো strbuf err = STRBUF_INIT;
	কাঠামো string_list refnames = STRING_LIST_INIT_NODUP;

	জন্য (ref = stale_refs; ref; ref = ref->পরবর্তী)
		string_list_append(&refnames, ref->name);

	যদি (!dry_run) {
		যদি (transaction) {
			জন্য (ref = stale_refs; ref; ref = ref->পরবর্তী) {
				ফলাফল = ref_transaction_delete(transaction, ref->name, ফাঁকা,
								ফাঁকা, 0, "fetch: prune", &err);
				যদি (ফলাফল)
					যাও cleanup;
			}
		} অন্যথা {
			ফলাফল = refs_delete_refs(get_main_ref_store(the_repository),
						  "fetch: prune", &refnames,
						  0);
		}
	}

	যদি (verbosity >= 0) {
		পূর্ণসংখ্যা summary_width = transport_summary_width(stale_refs);

		জন্য (ref = stale_refs; ref; ref = ref->পরবর্তী) {
			display_ref_update(display_state, '-', _("[deleted]"), ফাঁকা,
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
	ফেরত ফলাফল;
}

স্থির শূন্য check_not_current_branch(কাঠামো ref *ref_map)
{
	ধ্রুবক অক্ষর *path;
	জন্য (; ref_map; ref_map = ref_map->পরবর্তী)
		যদি (ref_map->peer_ref &&
		    starts_with(ref_map->peer_ref->name, "refs/heads/") &&
		    (path = branch_checked_out(ref_map->peer_ref->name)))
			die(_("refusing to fetch into branch '%s' "
			      "checked out at '%s'"),
			    ref_map->peer_ref->name, path);
}

স্থির পূর্ণসংখ্যা truncate_fetch_শীর্ষ(শূন্য)
{
	ধ্রুবক অক্ষর *filename = git_path_fetch_শীর্ষ(the_repository);
	FILE *fp = fopen_for_writing(filename);

	যদি (!fp)
		ফেরত error_errno(_("cannot open '%s'"), filename);
	fclose(fp);
	ফেরত 0;
}

স্থির শূন্য set_option(কাঠামো transport *transport, ধ্রুবক অক্ষর *name, ধ্রুবক অক্ষর *মান)
{
	পূর্ণসংখ্যা r = transport_set_option(transport, name, মান);
	যদি (r < 0)
		die(_("option \"%s\" value \"%s\" is not valid for %s"),
		    name, মান, transport->url);
	যদি (r > 0)
		warning(_("option \"%s\" is ignored for %s"),
			name, transport->url);
}


স্থির পূর্ণসংখ্যা add_oid(ধ্রুবক কাঠামো reference *ref, শূন্য *cb_তথ্য)
{
	কাঠামো oid_array *oids = cb_তথ্য;

	oid_array_append(oids, ref->oid);
	ফেরত 0;
}

স্থির শূন্য add_negotiation_tips(কাঠামো string_list *প্রবেশ_list,
				 কাঠামো oid_array **output_list,
				 ধ্রুবক অক্ষর *argname)
{
	কাঠামো oid_array *oids = xcalloc(1, আকার(*oids));
	পূর্ণসংখ্যা i;

	জন্য (i = 0; i < প্রবেশ_list->nr; i++) {
		ধ্রুবক অক্ষর *s = প্রবেশ_list->বস্তুসমূহ[i].string;
		কাঠামো refs_for_each_ref_options opts = {
			.pattern = s,
		};
		পূর্ণসংখ্যা old_nr;
		যদি (!has_glob_specials(s)) {
			কাঠামো object_id oid;

			/* Ignore missing reference. */
			যদি (repo_get_oid(the_repository, s, &oid))
				চালু;
			/* Fail on missing object pointed by ref. */
			যদি (!odb_has_object(the_repository->objects, &oid, 0))
				die(_("the object %s does not exist"), s);

			oid_array_append(oids, &oid);
			চালু;
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

স্থির কাঠামো transport *prepare_transport(কাঠামো remote *remote, পূর্ণসংখ্যা deepen,
					   কাঠামো list_objects_filter_options *filter_options)
{
	কাঠামো transport *transport;

	transport = transport_get(remote, ফাঁকা);
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
			   (ধ্রুবক অক্ষর *)&deepen_not);
	যদি (deepen_relative)
		set_option(transport, TRANS_OPT_DEEPEN_RELATIVE, "yes");
	যদি (update_shallow)
		set_option(transport, TRANS_OPT_UPDATE_SHALLOW, "yes");
	যদি (refetch)
		set_option(transport, TRANS_OPT_REFETCH, "yes");
	যদি (filter_options->choice) {
		ধ্রুবক অক্ষর *spec =
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
		কাঠামো string_list_item *item;
		for_each_string_list_item(item, &remote->negotiation_restrict)
			string_list_append(&negotiation_restrict, item->string);
		যদি (transport->smart_options)
			add_negotiation_tips(&negotiation_restrict,
					     &transport->smart_options->negotiation_restrict_tips,
					     "--negotiation-restrict");
		অন্যথা {
			কাঠামো strbuf config_name = STRBUF_INIT;
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
			কাঠামো strbuf config_name = STRBUF_INIT;
			strbuf_addf(&config_name, "remote.%s.negotiationInclude", remote->name);
			warning(_("ignoring %s because the protocol does not support it"),
				config_name.buf);
			strbuf_release(&config_name);
		}
	}
	ফেরত transport;
}

স্থির পূর্ণসংখ্যা backfill_tags(কাঠামো display_state *display_state,
			 কাঠামো transport *transport,
			 কাঠামো ref_transaction *transaction,
			 কাঠামো ref *ref_map,
			 কাঠামো fetch_শীর্ষ *fetch_শীর্ষ,
			 ধ্রুবক কাঠামো fetch_config *config,
			 কাঠামো ref_update_display_info_array *display_array,
			 কাঠামো list_objects_filter_options *filter_options)
{
	পূর্ণসংখ্যা retcode, cannot_reuse;

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

	transport_set_option(transport, TRANS_OPT_FOLLOWTAGS, ফাঁকা);
	transport_set_option(transport, TRANS_OPT_DEPTH, "0");
	transport_set_option(transport, TRANS_OPT_DEEPEN_RELATIVE, ফাঁকা);
	retcode = fetch_and_consume_refs(display_state, transport, transaction, ref_map,
					 fetch_শীর্ষ, config, display_array);

	যদি (gsecondary) {
		transport_disconnect(gsecondary);
		gsecondary = ফাঁকা;
	}

	ফেরত retcode;
}

স্থির ধ্রুবক অক্ষর *strip_refshead(ধ্রুবক অক্ষর *name){
	skip_prefix(name, "refs/heads/", &name);
	ফেরত name;
}

স্থির শূন্য set_শীর্ষ_advice_বার্তা(ধ্রুবক অক্ষর *remote, ধ্রুবক অক্ষর *শীর্ষ_name)
{
	ধ্রুবক অক্ষর message_advice_set_শীর্ষ[] =
	N_("Run 'git remote set-head %s %s' to follow the change, or modify\n"
	   "either of the 'remote.%s.followRemoteHEAD' or 'fetch.followRemoteHEAD'\n"
	   "configuration variables to handle the situation differently.\n\n"

	   "Using this specific setting\n\n"
	   "    git config set remote.%s.followRemoteHEAD warn-if-not-%s\n\n"
	   "will suppress the warning until the remote changes HEAD to something else.");

	advise_if_enabled(ADVICE_FETCH_SET_শীর্ষ_WARN, _(message_advice_set_শীর্ষ),
			remote, শীর্ষ_name, remote, remote, শীর্ষ_name);
}

স্থির শূন্য warn_set_শীর্ষ(ধ্রুবক অক্ষর *remote, ধ্রুবক অক্ষর *শীর্ষ_name,
			কাঠামো strbuf *buf_পূর্ব, পূর্ণসংখ্যা updateres) {
	কাঠামো strbuf buf_prefix = STRBUF_INIT;
	ধ্রুবক অক্ষর *পূর্ব_শীর্ষ = ফাঁকা;

	strbuf_addf(&buf_prefix, "refs/remotes/%s/", remote);
	skip_prefix(buf_পূর্ব->buf, buf_prefix.buf, &পূর্ব_শীর্ষ);

	যদি (পূর্ব_শীর্ষ && strcmp(পূর্ব_শীর্ষ, শীর্ষ_name)) {
		printf("'HEAD' at '%s' is '%s', but we have '%s' locally.\n",
			remote, শীর্ষ_name, পূর্ব_শীর্ষ);
		set_শীর্ষ_advice_বার্তা(remote, শীর্ষ_name);
	}
	অন্যথা যদি (updateres && buf_পূর্ব->len) {
		printf("'HEAD' at '%s' is '%s', "
			"but we have a detached HEAD pointing to '%s' locally.\n",
			remote, শীর্ষ_name, buf_পূর্ব->buf);
		set_শীর্ষ_advice_বার্তা(remote, শীর্ষ_name);
	}
	strbuf_release(&buf_prefix);
}

স্থির পূর্ণসংখ্যা set_শীর্ষ(ধ্রুবক কাঠামো ref *remote_refs, কাঠামো remote *remote,
			পূর্ণসংখ্যা follow_remote_শীর্ষ)
{
	পূর্ণসংখ্যা ফলাফল = 0, create_only, baremirror, was_detached;
	কাঠামো strbuf b_শীর্ষ = STRBUF_INIT, b_remote_শীর্ষ = STRBUF_INIT,
		      b_local_শীর্ষ = STRBUF_INIT;
	ধ্রুবক অক্ষর *no_warn_branch = remote->no_warn_branch;
	অক্ষর *শীর্ষ_name = ফাঁকা;
	কাঠামো ref *ref, *matches;
	কাঠামো ref *fetch_map = ফাঁকা, **fetch_map_লেজ = &fetch_map;
	কাঠামো refspec_item refspec = {
		.force = 0,
		.pattern = 1,
		.src = (অক্ষর *) "refs/heads/*",
		.dst = (অক্ষর *) "refs/heads/*",
	};
	কাঠামো string_list heads = STRING_LIST_INIT_DUP;
	কাঠামো ref_store *refs = get_main_ref_store(the_repository);

	get_fetch_map(remote_refs, &refspec, &fetch_map_লেজ, 0);
	matches = guess_remote_শীর্ষ(find_ref_by_name(remote_refs, "HEAD"),
				    fetch_map, REMOTE_GUESS_শীর্ষ_ALL);
	জন্য (ref = matches; ref; ref = ref->পরবর্তী) {
		string_list_append(&heads, strip_refshead(ref->name));
	}

	যদি (!heads.nr)
		ফলাফল = 1;
	অন্যথা যদি (heads.nr > 1)
		ফলাফল = 1;
	অন্যথা
		শীর্ষ_name = xstrdup(heads.বস্তুসমূহ[0].string);

	যদি (!শীর্ষ_name)
		যাও cleanup;
	baremirror = is_bare_repository(the_repository) && remote->mirror;
	create_only = follow_remote_শীর্ষ == FOLLOW_REMOTE_ALWAYS ? 0 : !baremirror;
	যদি (baremirror) {
		strbuf_addstr(&b_শীর্ষ, "HEAD");
		strbuf_addf(&b_remote_শীর্ষ, "refs/heads/%s", শীর্ষ_name);
	} অন্যথা {
		strbuf_addf(&b_শীর্ষ, "refs/remotes/%s/HEAD", remote->name);
		strbuf_addf(&b_remote_শীর্ষ, "refs/remotes/%s/%s", remote->name, শীর্ষ_name);
	}
	/* make sure it's valid */
	যদি (!baremirror && !refs_ref_exists(refs, b_remote_শীর্ষ.buf)) {
		ফলাফল = 1;
		যাও cleanup;
	}
	was_detached = refs_update_symref_extended(refs, b_শীর্ষ.buf, b_remote_শীর্ষ.buf,
					"fetch", &b_local_শীর্ষ, create_only);
	যদি (was_detached == -1) {
		ফলাফল = 1;
		যাও cleanup;
	}
	যদি (verbosity >= 0 &&
		follow_remote_শীর্ষ == FOLLOW_REMOTE_WARN &&
		(!no_warn_branch || strcmp(no_warn_branch, শীর্ষ_name)))
		warn_set_শীর্ষ(remote->name, শীর্ষ_name, &b_local_শীর্ষ, was_detached);

cleanup:
	মুক্ত(শীর্ষ_name);
	free_refs(fetch_map);
	free_refs(matches);
	string_list_clear(&heads, 0);
	strbuf_release(&b_শীর্ষ);
	strbuf_release(&b_local_শীর্ষ);
	strbuf_release(&b_remote_শীর্ষ);
	ফেরত ফলাফল;
}

কাঠামো ref_rejection_তথ্য {
	পূর্ণসংখ্যা *retcode;
	bool conflict_বার্তা_shown;
	bool case_sensitive_বার্তা_shown;
	ধ্রুবক অক্ষর *remote_name;
	কাঠামো strmap *rejected_refs;
};

স্থির শূন্য ref_transaction_rejection_handler(ধ্রুবক অক্ষর *refname,
					      ধ্রুবক কাঠামো object_id *old_oid UNUSED,
					      ধ্রুবক কাঠামো object_id *new_oid UNUSED,
					      ধ্রুবক অক্ষর *old_target UNUSED,
					      ধ্রুবক অক্ষর *new_target UNUSED,
					      গণনা ref_transaction_error err,
					      ধ্রুবক অক্ষর *details,
					      শূন্য *cb_তথ্য)
{
	কাঠামো ref_rejection_তথ্য *তথ্য = cb_তথ্য;

	যদি (err == REF_TRANSACTION_ERROR_CASE_CONFLICT && repo_ignore_case(the_repository) &&
	    !তথ্য->case_sensitive_বার্তা_shown) {
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
		তথ্য->case_sensitive_বার্তা_shown = true;
	} অন্যথা যদি (err == REF_TRANSACTION_ERROR_NAME_CONFLICT &&
		   !তথ্য->conflict_বার্তা_shown) {
		error(_("some local refs could not be updated; try running\n"
			" 'git remote prune %s' to remove any old, conflicting "
			"branches"), তথ্য->remote_name);
		তথ্য->conflict_বার্তা_shown = true;
	} অন্যথা {
		যদি (details)
			error("%s", details);
		অন্যথা
			error(_("fetching ref %s failed: %s"),
			      refname, ref_transaction_error_বার্তা(err));
	}

	strmap_put(তথ্য->rejected_refs, refname, ফাঁকা);
	*তথ্য->retcode = 1;
}

/*
 * Commit the reference transaction. If it isn't an atomic transaction, handle
 * rejected updates as part of using batched updates.
 */
স্থির পূর্ণসংখ্যা commit_ref_transaction(কাঠামো ref_transaction **transaction,
				  bool is_atomic, ধ্রুবক অক্ষর *remote_name,
				  কাঠামো strmap *rejected_refs,
				  কাঠামো strbuf *err)
{
	পূর্ণসংখ্যা retcode = ref_transaction_commit(*transaction, err);
	যদি (retcode)
		যাও out;

	যদি (!is_atomic) {
		কাঠামো ref_rejection_তথ্য তথ্য = {
			.conflict_বার্তা_shown = 0,
			.remote_name = remote_name,
			.retcode = &retcode,
			.rejected_refs = rejected_refs,
		};

		ref_transaction_for_each_rejected_update(*transaction,
							 ref_transaction_rejection_handler,
							 &তথ্য);
	}

out:
	ref_transaction_free(*transaction);
	*transaction = ফাঁকা;
	ফেরত retcode;
}

স্থির শূন্য collect_updated_tips(কাঠামো oidset *tips, কাঠামো ref *ref_map)
{
	কাঠামো ref *rm;
	জন্য (rm = ref_map; rm; rm = rm->পরবর্তী) {
		কাঠামো commit *commit;
		/*
		 * Shallow-rejected refs are not stored and their history
		 * is incomplete, so skip them.
		 */
		যদি (rm->status == REF_STATUS_REJECT_SHALLOW)
			চালু;
		যদি (is_null_oid(&rm->old_oid))
			চালু;
		যদি (rm->peer_ref &&
		    oideq(&rm->old_oid, &rm->peer_ref->old_oid))
			চালু;
		commit = lookup_commit_reference_gently(the_repository,
							&rm->old_oid, 1);
		যদি (commit)
			oidset_ঢোকাও(tips, &commit->object.oid);
	}
}

স্থির পূর্ণসংখ্যা do_fetch(কাঠামো transport *transport,
		    কাঠামো refspec *rs,
		    ধ্রুবক কাঠামো fetch_config *config,
		    কাঠামো list_objects_filter_options *filter_options,
		    কাঠামো oidset *updated_tips)
{
	কাঠামো ref_transaction *transaction = ফাঁকা;
	কাঠামো ref *ref_map = ফাঁকা;
	কাঠামো display_state display_state = { 0 };
	পূর্ণসংখ্যা autotags = (transport->remote->fetch_tags == 1);
	পূর্ণসংখ্যা retcode = 0;
	ধ্রুবক কাঠামো ref *remote_refs;
	কাঠামো transport_ls_refs_options transport_ls_refs_options =
		TRANSPORT_LS_REFS_OPTIONS_INIT;
	কাঠামো fetch_শীর্ষ fetch_শীর্ষ = { 0 };
	কাঠামো strbuf err = STRBUF_INIT;
	কাঠামো ref_update_display_info_array display_array = { 0 };
	কাঠামো strmap rejected_refs = STRMAP_INIT;
	পূর্ণসংখ্যা summary_width = 0;
	পূর্ণসংখ্যা follow_remote_শীর্ষ = FOLLOW_REMOTE_NEVER;

	যদি (tags == TAGS_DEFAULT) {
		যদি (transport->remote->fetch_tags == 2)
			tags = TAGS_SET;
		যদি (transport->remote->fetch_tags == -1)
			tags = TAGS_UNSET;
	}

	/* if not appending, truncate FETCH_HEAD */
	যদি (!append && write_fetch_শীর্ষ) {
		retcode = truncate_fetch_শীর্ষ();
		যদি (retcode)
			যাও cleanup;
	}

	যদি (rs->nr) {
		refspec_ref_prefixes(rs, &transport_ls_refs_options.ref_prefixes);
	} অন্যথা {
		কাঠামো branch *branch = branch_get(ফাঁকা);

		যদি (transport->remote->fetch.nr) {
			refspec_ref_prefixes(&transport->remote->fetch,
					     &transport_ls_refs_options.ref_prefixes);
			/*
			 * See remote.c's handling of remote.<name>.followRemoteHEAD
			 * for the analogous, still-unresolved case.
			 */
			যদি (transport->remote->follow_remote_শীর্ষ)
				follow_remote_শীর্ষ = transport->remote->follow_remote_শীর্ষ;
			অন্যথা যদি (config->follow_remote_শীর্ষ_seen)
				follow_remote_শীর্ষ = get_follow_remote_শীর্ষ(config->follow_remote_শীর্ষ_raw);
			অন্যথা
				follow_remote_শীর্ষ = BUILTIN_FOLLOW_REMOTE_শীর্ষ_DFLT;
		}
		যদি (branch && branch_has_merge_config(branch) &&
		    !strcmp(branch->remote_name, transport->remote->name)) {
			পূর্ণসংখ্যা i;
			জন্য (i = 0; i < branch->merge_nr; i++) {
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

	যদি (follow_remote_শীর্ষ != FOLLOW_REMOTE_NEVER)
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
		remote_refs = ফাঁকা;

	transport_ls_refs_options_release(&transport_ls_refs_options);

	ref_map = get_ref_map(transport->remote, remote_refs, rs,
			      tags, &autotags);
	যদি (!update_শীর্ষ_ok)
		check_not_current_branch(ref_map);

	retcode = open_fetch_শীর্ষ(&fetch_শীর্ষ);
	যদি (retcode)
		যাও cleanup;

	display_state_init(&display_state, ref_map, transport->url,
			   config->display_format);

	যদি (atomic_fetch) {
		transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
							  0, &err);
		যদি (!transaction) {
			retcode = -1;
			যাও cleanup;
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
			যাও cleanup;
		}
	}

	যদি (fetch_and_consume_refs(&display_state, transport, transaction, ref_map,
				   &fetch_শীর্ষ, config, &display_array)) {
		retcode = 1;
		যাও cleanup;
	}

	/*
	 * If neither --no-tags nor --tags was specified, do automated tag
	 * following.
	 */
	যদি (tags == TAGS_DEFAULT && autotags) {
		কাঠামো ref *tags_ref_map = ফাঁকা, **লেজ = &tags_ref_map;

		find_non_local_tags(remote_refs, transaction, &tags_ref_map, &লেজ);
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
					  &fetch_শীর্ষ, config, &display_array, filter_options))
				retcode = 1;
		}

		free_refs(tags_ref_map);
	}

	যদি (retcode)
		যাও cleanup;

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
		যাও cleanup;

	commit_fetch_শীর্ষ(&fetch_শীর্ষ);

	collect_updated_tips(updated_tips, ref_map);

	যদি (set_upstream) {
		কাঠামো branch *branch = branch_get("HEAD");
		কাঠামো ref *rm;
		কাঠামো ref *source_ref = ফাঁকা;

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
		জন্য (rm = ref_map; rm; rm = rm->পরবর্তী) {
			যদি (!rm->peer_ref) {
				যদি (source_ref) {
					warning(_("multiple branches detected, incompatible with --set-upstream"));
					যাও cleanup;
				} অন্যথা {
					source_ref = rm;
				}
			}
		}
		যদি (source_ref) {
			যদি (!branch) {
				ধ্রুবক অক্ষর *shortname = source_ref->name;
				skip_prefix(shortname, "refs/heads/", &shortname);

				warning(_("could not set upstream of HEAD to '%s' from '%s' when "
					  "it does not point to any branch."),
					shortname, transport->remote->name);
				যাও cleanup;
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
	যদি (follow_remote_শীর্ষ != FOLLOW_REMOTE_NEVER) {
		/*
		 * Way too many cases where this can go wrong so let's just
		 * ignore errors and fail silently for now.
		 */
		set_শীর্ষ(remote_refs, transport->remote, follow_remote_শীর্ষ);
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

	জন্য (আকার_t i = 0; i < display_array.nr; i++) {
		কাঠামো ref_update_display_info *info = &display_array.info[i];

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
		transaction = ফাঁকা;
	}

	যদি (transaction)
		ref_transaction_free(transaction);

	মুক্ত(display_array.info);
	strmap_clear(&rejected_refs, 0);
	display_state_release(&display_state);
	close_fetch_শীর্ষ(&fetch_শীর্ষ);
	strbuf_release(&err);
	free_refs(ref_map);
	ফেরত retcode;
}

স্থির পূর্ণসংখ্যা get_one_remote_for_fetch(কাঠামো remote *remote, শূন্য *priv)
{
	কাঠামো string_list *list = priv;
	যদি (!remote->skip_default_update)
		string_list_append(list, remote->name);
	ফেরত 0;
}

স্থির শূন্য add_options_to_argv(কাঠামো strvec *argv,
				ধ্রুবক কাঠামো fetch_config *config)
{
	যদি (dry_run)
		strvec_ঠেলো(argv, "--dry-run");
	যদি (prune != -1)
		strvec_ঠেলো(argv, prune ? "--prune" : "--no-prune");
	যদি (prune_tags != -1)
		strvec_ঠেলো(argv, prune_tags ? "--prune-tags" : "--no-prune-tags");
	যদি (update_শীর্ষ_ok)
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
	যদি (!write_fetch_শীর্ষ)
		strvec_ঠেলো(argv, "--no-write-fetch-head");
	যদি (config->display_format == DISPLAY_FORMAT_PORCELAIN)
		strvec_pushf(argv, "--porcelain");
}

/* Fetch multiple remotes in parallel */

কাঠামো parallel_fetch_state {
	ধ্রুবক অক্ষর **argv;
	কাঠামো string_list *remotes;
	পূর্ণসংখ্যা পরবর্তী, ফলাফল;
	ধ্রুবক কাঠামো fetch_config *config;
};

স্থির পূর্ণসংখ্যা fetch_পরবর্তী_remote(কাঠামো child_process *cp,
			     কাঠামো strbuf *out UNUSED,
			     শূন্য *cb, শূন্য **task_cb)
{
	কাঠামো parallel_fetch_state *state = cb;
	অক্ষর *remote;

	যদি (state->পরবর্তী < 0 || state->পরবর্তী >= state->remotes->nr)
		ফেরত 0;

	remote = state->remotes->বস্তুসমূহ[state->পরবর্তী++].string;
	*task_cb = remote;

	strvec_pushv(&cp->args, state->argv);
	strvec_ঠেলো(&cp->args, remote);
	cp->git_cmd = 1;

	যদি (verbosity >= 0 && state->config->display_format != DISPLAY_FORMAT_PORCELAIN)
		printf(_("Fetching %s\n"), remote);

	ফেরত 1;
}

স্থির পূর্ণসংখ্যা fetch_failed_to_start(কাঠামো strbuf *out UNUSED,
				 শূন্য *cb, শূন্য *task_cb)
{
	কাঠামো parallel_fetch_state *state = cb;
	ধ্রুবক অক্ষর *remote = task_cb;

	state->ফলাফল = error(_("could not fetch %s"), remote);

	ফেরত 0;
}

স্থির পূর্ণসংখ্যা fetch_finished(পূর্ণসংখ্যা ফলাফল, কাঠামো strbuf *out,
			  শূন্য *cb, শূন্য *task_cb)
{
	কাঠামো parallel_fetch_state *state = cb;
	ধ্রুবক অক্ষর *remote = task_cb;

	যদি (ফলাফল) {
		strbuf_addf(out, _("could not fetch '%s' (exit code: %d)\n"),
			    remote, ফলাফল);
		state->ফলাফল = -1;
	}

	ফেরত 0;
}

স্থির পূর্ণসংখ্যা fetch_multiple(কাঠামো string_list *list, পূর্ণসংখ্যা max_children,
			  ধ্রুবক কাঠামো fetch_config *config)
{
	পূর্ণসংখ্যা i, ফলাফল = 0;
	কাঠামো strvec argv = STRVEC_INIT;

	যদি (!append && write_fetch_শীর্ষ) {
		পূর্ণসংখ্যা errcode = truncate_fetch_শীর্ষ();
		যদি (errcode)
			ফেরত errcode;
	}

	/*
	 * Cancel out the fetch.bundleURI config when running subprocesses,
	 * to avoid fetching from the same bundle list multiple times.
	 */
	strvec_pushl(&argv, "-c", "fetch.bundleURI=",
		     "fetch", "--append", "--no-auto-gc",
		     "--no-write-commit-graph", ফাঁকা);
	জন্য (i = 0; i < server_options.nr; i++)
		strvec_pushf(&argv, "--server-option=%s", server_options.বস্তুসমূহ[i].string);
	add_options_to_argv(&argv, config);

	যদি (max_children != 1 && list->nr != 1) {
		কাঠামো parallel_fetch_state state = { argv.v, list, 0, 0, config };
		ধ্রুবক কাঠামো run_process_parallel_opts opts = {
			.tr2_category = "fetch",
			.tr2_label = "parallel/fetch",

			.processes = max_children,

			.get_পরবর্তী_task = &fetch_পরবর্তী_remote,
			.start_failure = &fetch_failed_to_start,
			.task_finished = &fetch_finished,
			.তথ্য = &state,
		};

		strvec_ঠেলো(&argv, "--end-of-options");

		run_processes_parallel(&opts);
		ফলাফল = state.ফলাফল;
	} অন্যথা
		জন্য (i = 0; i < list->nr; i++) {
			ধ্রুবক অক্ষর *name = list->বস্তুসমূহ[i].string;
			কাঠামো child_process cmd = CHILD_PROCESS_INIT;

			strvec_pushv(&cmd.args, argv.v);
			strvec_ঠেলো(&cmd.args, name);
			যদি (verbosity >= 0 && config->display_format != DISPLAY_FORMAT_PORCELAIN)
				printf(_("Fetching %s\n"), name);
			cmd.git_cmd = 1;
			যদি (run_command(&cmd)) {
				error(_("could not fetch %s"), name);
				ফলাফল = 1;
			}
		}

	strvec_clear(&argv);
	ফেরত !!ফলাফল;
}

/*
 * Fetching from the promisor remote should use the given filter-spec
 * or inherit the default filter-spec from the config.
 */
স্থির অন্তঃরেখ শূন্য fetch_one_setup_partial(কাঠামো remote *remote,
					   কাঠামো list_objects_filter_options *filter_options)
{
	/*
	 * Explicit --no-filter argument overrides everything, regardless
	 * of any prior partial clones and fetches.
	 */
	যদি (filter_options->no_filter)
		ফেরত;

	/*
	 * If no prior partial clone/fetch and the current fetch DID NOT
	 * request a partial-fetch, do a normal fetch.
	 */
	যদি (!repo_has_promisor_remote(the_repository) && !filter_options->choice)
		ফেরত;

	/*
	 * If this is a partial-fetch request, we enable partial on
	 * this repo if not already enabled and remember the given
	 * filter-spec as the default for subsequent fetches to this
	 * remote if there is currently no default filter-spec.
	 */
	যদি (filter_options->choice) {
		partial_clone_register(remote->name, filter_options);
		ফেরত;
	}

	/*
	 * Do a partial-fetch from the promisor remote using either the
	 * explicitly given filter-spec or inherit the filter-spec from
	 * the config.
	 */
	যদি (!filter_options->choice)
		partial_clone_get_default_filter_spec(filter_options, remote->name);
	ফেরত;
}

স্থির পূর্ণসংখ্যা fetch_one(কাঠামো remote *remote, পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv,
		     পূর্ণসংখ্যা prune_tags_ok, পূর্ণসংখ্যা use_stdin_refspecs,
		     ধ্রুবক কাঠামো fetch_config *config,
		     কাঠামো list_objects_filter_options *filter_options,
		     কাঠামো oidset *updated_tips)
{
	কাঠামো refspec rs = REFSPEC_INIT_FETCH(the_hash_algo);
	পূর্ণসংখ্যা i;
	পূর্ণসংখ্যা exit_code;
	পূর্ণসংখ্যা maybe_prune_tags;
	পূর্ণসংখ্যা remote_via_config = remote_is_configured(remote, 0);

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

	জন্য (i = 0; i < argc; i++) {
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
		কাঠামো strbuf সারি = STRBUF_INIT;
		যখন (strbuf_getline_lf(&সারি, stdin) != EOF)
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
	gtransport = ফাঁকা;
	ফেরত exit_code;
}

পূর্ণসংখ্যা cmd_fetch(পূর্ণসংখ্যা argc,
	      ধ্রুবক অক্ষর **argv,
	      ধ্রুবক অক্ষর *prefix,
	      কাঠামো repository *repo UNUSED)
{
	কাঠামো fetch_config config = {
		.display_format = DISPLAY_FORMAT_FULL,
		.follow_remote_শীর্ষ_raw = ফাঁকা,
		.follow_remote_শীর্ষ_seen = 0,
		.prune = -1,
		.prune_tags = -1,
		.show_forced_updates = 1,
		.recurse_submodules = RECURSE_SUBMODULES_DEFAULT,
		.parallel = 1,
		.submodule_fetch_jobs = -1,
	};
	ধ্রুবক অক্ষর *submodule_prefix = "";
	ধ্রুবক অক্ষর *bundle_uri;
	কাঠামো string_list list = STRING_LIST_INIT_DUP;
	কাঠামো list_objects_filter_options filter_options = LIST_OBJECTS_FILTER_INIT;
	কাঠামো remote *remote = ফাঁকা;
	পূর্ণসংখ্যা all = -1, multiple = 0;
	পূর্ণসংখ্যা ফলাফল = 0;
	পূর্ণসংখ্যা prune_tags_ok = 1;
	পূর্ণসংখ্যা enable_auto_gc = 1;
	পূর্ণসংখ্যা unshallow = 0;
	পূর্ণসংখ্যা max_jobs = -1;
	পূর্ণসংখ্যা recurse_submodules_cli = RECURSE_SUBMODULES_DEFAULT;
	পূর্ণসংখ্যা recurse_submodules_default = RECURSE_SUBMODULES_ON_DEMAND;
	পূর্ণসংখ্যা fetch_write_commit_graph = -1;
	পূর্ণসংখ্যা stdin_refspecs = 0;
	পূর্ণসংখ্যা negotiate_only = 0;
	পূর্ণসংখ্যা porcelain = 0;
	পূর্ণসংখ্যা i;
	গণনা {
		GRAPH_WRITE_REACHABLE,
		GRAPH_WRITE_TIPS,
		GRAPH_WRITE_SKIP,
	} graph_write_mode = GRAPH_WRITE_REACHABLE;
	কাঠামো oidset updated_tips = OIDSET_INIT;

	কাঠামো option builtin_fetch_options[] = {
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
		OPT_SET_INT('n', ফাঁকা, &tags,
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
		OPT_BOOL(0, "write-fetch-head", &write_fetch_শীর্ষ,
			 N_("write fetched references to the FETCH_HEAD file")),
		OPT_BOOL('k', "keep", &keep, N_("keep downloaded pack")),
		OPT_BOOL('u', "update-head-ok", &update_শীর্ষ_ok,
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
	জন্য (i = 1; i < argc; i++) {
		/* This handles non-URLs gracefully */
		অক্ষর *anon = transport_anonymize_url(argv[i]);

		strbuf_addf(&default_rla, " %s", anon);
		মুক্ত(anon);
	}

	repo_config(the_repository, git_fetch_config, &config);
	যদি (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_full_সূচক = 0;
	}

	argc = parse_options(argc, argv, prefix,
			     builtin_fetch_options, builtin_fetch_usage, 0);

	যদি (recurse_submodules_cli != RECURSE_SUBMODULES_DEFAULT)
		config.recurse_submodules = recurse_submodules_cli;

	যদি (negotiate_only) {
		নির্বাচন (recurse_submodules_cli) {
		ক্ষেত্র RECURSE_SUBMODULES_OFF:
		ক্ষেত্র RECURSE_SUBMODULES_DEFAULT:
			/*
			 * --negotiate-only should never recurse into
			 * submodules. Skip it by setting recurse_submodules to
			 * RECURSE_SUBMODULES_OFF.
			 */
			config.recurse_submodules = RECURSE_SUBMODULES_OFF;
			বিরতি;

		পূর্বনির্ধারিত:
			die(_("options '%s' and '%s' cannot be used together"),
			    "--negotiate-only", "--recurse-submodules");
		}
	}

	যদি (config.recurse_submodules != RECURSE_SUBMODULES_OFF) {
		পূর্ণসংখ্যা *sfjc = config.submodule_fetch_jobs == -1
			    ? &config.submodule_fetch_jobs : ফাঁকা;
		পূর্ণসংখ্যা *rs = config.recurse_submodules == RECURSE_SUBMODULES_DEFAULT
			  ? &config.recurse_submodules : ফাঁকা;

		fetch_config_from_gitmodules(the_repository, sfjc, rs);
	}


	যদি (porcelain) {
		নির্বাচন (recurse_submodules_cli) {
		ক্ষেত্র RECURSE_SUBMODULES_OFF:
		ক্ষেত্র RECURSE_SUBMODULES_DEFAULT:
			/*
			 * Reference updates in submodules would be ambiguous
			 * in porcelain mode, so we reject this combination.
			 */
			config.recurse_submodules = RECURSE_SUBMODULES_OFF;
			বিরতি;

		পূর্বনির্ধারিত:
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
		write_fetch_শীর্ষ = 0;

	যদি (!max_jobs)
		max_jobs = online_cpus();

	যদি (!repo_config_get_string_tmp(the_repository, "fetch.bundleuri", &bundle_uri) &&
	    fetch_bundle_uri(the_repository, bundle_uri, ফাঁকা))
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
		remote = remote_get(ফাঁকা);
	} অন্যথা যদি (multiple) {
		/* All arguments are assumed to be remotes or groups */
		জন্য (i = 0; i < argc; i++)
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
		কাঠামো oidset acked_commits = OIDSET_INIT;
		কাঠামো oidset_iter iter;
		ধ্রুবক কাঠামো object_id *oid;

		trace2_region_enter("fetch", "negotiate-only", the_repository);
		যদি (!remote)
			die(_("must supply remote when using --negotiate-only"));
		gtransport = prepare_transport(remote, 1, &filter_options);

		যদি (!gtransport->smart_options) {
			warning(_("protocol does not support --negotiate-only, exiting"));
			ফলাফল = 1;
			trace2_region_leave("fetch", "negotiate-only", the_repository);
			যাও cleanup;
		}
		যদি (!gtransport->smart_options->negotiation_restrict_tips)
			die(_("%s needs one or more %s"), "--negotiate-only",
			    "--negotiation-restrict=*");

		gtransport->smart_options->acked_commits = &acked_commits;

		যদি (server_options.nr)
			gtransport->server_options = &server_options;
		ফলাফল = transport_fetch_refs(gtransport, ফাঁকা);
		gtransport->smart_options->acked_commits = ফাঁকা;

		oidset_iter_init(&acked_commits, &iter);
		যখন ((oid = oidset_iter_পরবর্তী(&iter)))
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
		ফলাফল = fetch_one(remote, argc, argv, prune_tags_ok, stdin_refspecs,
				   &config, &filter_options, &updated_tips);
		যদি (prepare_commit_graph(the_repository)) {
			যদি (oidset_আকার(&updated_tips))
				graph_write_mode = GRAPH_WRITE_TIPS;
			অন্যথা
				graph_write_mode = GRAPH_WRITE_SKIP;
		}
		trace2_region_leave("fetch", "fetch-one", the_repository);
	} অন্যথা {
		পূর্ণসংখ্যা max_children = max_jobs;

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
		ফলাফল = fetch_multiple(&list, max_children, &config);
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
	যদি (!ফলাফল && remote && (config.recurse_submodules != RECURSE_SUBMODULES_OFF)) {
		কাঠামো strvec options = STRVEC_INIT;
		পূর্ণসংখ্যা max_children = max_jobs;

		যদি (max_children < 0)
			max_children = config.submodule_fetch_jobs;
		যদি (max_children < 0)
			max_children = config.parallel;

		add_options_to_argv(&options, &config);
		trace2_region_enter_printf("fetch", "recurse-submodule", the_repository, "%s", submodule_prefix);
		ফলাফল = fetch_submodules(the_repository,
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
		যাও cleanup;

	prepare_repo_settings(the_repository);
	যদি (fetch_write_commit_graph > 0 ||
	    (fetch_write_commit_graph < 0 &&
	     the_repository->settings.fetch_write_commit_graph)) {
		পূর্ণসংখ্যা commit_graph_flags = COMMIT_GRAPH_WRITE_SPLIT;

		যদি (progress)
			commit_graph_flags |= COMMIT_GRAPH_WRITE_PROGRESS;

		যদি (graph_write_mode != GRAPH_WRITE_SKIP) {
			trace2_region_enter("fetch", "write-commit-graph",
					    the_repository);
			যদি (graph_write_mode == GRAPH_WRITE_TIPS)
				write_commit_graph(
					the_repository,
					the_repository->objects->source->path,
					ফাঁকা, &updated_tips,
					commit_graph_flags, ফাঁকা);
			অন্যথা
				write_commit_graph_reachable(
					the_repository,
					the_repository->objects->source->path,
					commit_graph_flags, ফাঁকা);
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
			পূর্ণসংখ্যা opt_val;
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
	মুক্ত(config.follow_remote_শীর্ষ_raw);
	ফেরত ফলাফল;
}
