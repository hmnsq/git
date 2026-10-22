/*
 * "git fetch"
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "advice.h"
#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "environment.h"
#ଅନ୍ତର୍ଭାଵ "hex.h"
#ଅନ୍ତର୍ଭାଵ "refs.h"
#ଅନ୍ତର୍ଭାଵ "refspec.h"
#ଅନ୍ତର୍ଭାଵ "object-name.h"
#ଅନ୍ତର୍ଭାଵ "odb.h"
#ଅନ୍ତର୍ଭାଵ "odb/source.h"
#ଅନ୍ତର୍ଭାଵ "oidset.h"
#ଅନ୍ତର୍ଭାଵ "oid-array.h"
#ଅନ୍ତର୍ଭାଵ "commit.h"
#ଅନ୍ତର୍ଭାଵ "string-list.h"
#ଅନ୍ତର୍ଭାଵ "remote.h"
#ଅନ୍ତର୍ଭାଵ "transport.h"
#ଅନ୍ତର୍ଭାଵ "run-command.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"
#ଅନ୍ତର୍ଭାଵ "sigchain.h"
#ଅନ୍ତର୍ଭାଵ "submodule-config.h"
#ଅନ୍ତର୍ଭାଵ "submodule.h"
#ଅନ୍ତର୍ଭାଵ "connected.h"
#ଅନ୍ତର୍ଭାଵ "strvec.h"
#ଅନ୍ତର୍ଭାଵ "utf8.h"
#ଅନ୍ତର୍ଭାଵ "pager.h"
#ଅନ୍ତର୍ଭାଵ "path.h"
#ଅନ୍ତର୍ଭାଵ "pkt-line.h"
#ଅନ୍ତର୍ଭାଵ "list-objects-filter-options.h"
#ଅନ୍ତର୍ଭାଵ "commit-reach.h"
#ଅନ୍ତର୍ଭାଵ "branch.h"
#ଅନ୍ତର୍ଭାଵ "promisor-remote.h"
#ଅନ୍ତର୍ଭାଵ "commit-graph.h"
#ଅନ୍ତର୍ଭାଵ "shallow.h"
#ଅନ୍ତର୍ଭାଵ "trace.h"
#ଅନ୍ତର୍ଭାଵ "trace2.h"
#ଅନ୍ତର୍ଭାଵ "bundle-uri.h"

#define FORCED_UPDATES_DELAY_WARNING_IN_MS (10 * 1000)

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ builtin_fetch_usage[] = {
	N_("git fetch [<options>] [<repository> [<refspec>...]]"),
	N_("git fetch [<options>] <group>"),
	N_("git fetch --multiple [<options>] [(<repository>|<group>)...]"),
	N_("git fetch --all [<options>]"),
	ଖାଲି
};

ଗଣନା {
	TAGS_UNSET = 0,
	TAGS_DEFAULT = 1,
	TAGS_SET = 2
};

ଗଣନା display_format {
	DISPLAY_FORMAT_FULL,
	DISPLAY_FORMAT_COMPACT,
	DISPLAY_FORMAT_PORCELAIN,
};

ସଂରଚନା display_state {
	ସଂରଚନା strbuf buf;

	ପୂର୍ଣ୍ଣସଂଖ୍ୟା refcol_width;
	ଗଣନା display_format format;

	ଅକ୍ଷର *url;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା url_len, shown_url;
};

ସ୍ଥିର uint64_t forced_updates_ms = 0;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା prefetch = 0;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା prune = -1; /* unspecified */
#define PRUNE_BY_DEFAULT 0 /* do we prune by default? */

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା prune_tags = -1; /* unspecified */
#define PRUNE_TAGS_BY_DEFAULT 0 /* do we prune tags by default? */

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା append, dry_run, force, keep, update_head_ok;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା write_fetch_head = 1;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା verbosity, deepen_relative, set_upstream, refetch;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା progress = -1;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା tags = TAGS_DEFAULT, update_shallow, deepen;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା atomic_fetch;
ସ୍ଥିର ଗଣନା transport_family family;
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *depth;
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *deepen_since;
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *upload_pack;
ସ୍ଥିର ସଂରଚନା string_list deepen_not = STRING_LIST_INIT_NODUP;
ସ୍ଥିର ସଂରଚନା strbuf default_rla = STRBUF_INIT;
ସ୍ଥିର ସଂରଚନା transport *gtransport;
ସ୍ଥିର ସଂରଚନା transport *gsecondary;
ସ୍ଥିର ସଂରଚନା refspec refmap;
ସ୍ଥିର ସଂରଚନା string_list server_options = STRING_LIST_INIT_DUP;
ସ୍ଥିର ସଂରଚନା string_list negotiation_restrict = STRING_LIST_INIT_NODUP;
ସ୍ଥିର ସଂରଚନା string_list negotiation_include = STRING_LIST_INIT_NODUP;

ସଂରଚନା fetch_config {
	ଗଣନା display_format display_format;
	ଅକ୍ଷର *follow_remote_head_raw;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା follow_remote_head_seen;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା all;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା prune;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା prune_tags;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_forced_updates;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା recurse_submodules;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା parallel;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା submodule_fetch_jobs;
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା git_fetch_config(ଅଚଳ ଅକ୍ଷର *k, ଅଚଳ ଅକ୍ଷର *v,
			    ଅଚଳ ସଂରଚନା config_context *ctx, ଶୂନ୍ୟ *cb)
{
	ସଂରଚନା fetch_config *fetch_config = cb;

	ଯଦି (!strcmp(k, "fetch.all")) {
		fetch_config->all = git_config_bool(k, v);
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (!strcmp(k, "fetch.prune")) {
		fetch_config->prune = git_config_bool(k, v);
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (!strcmp(k, "fetch.prunetags")) {
		fetch_config->prune_tags = git_config_bool(k, v);
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (!strcmp(k, "fetch.showforcedupdates")) {
		fetch_config->show_forced_updates = git_config_bool(k, v);
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (!strcmp(k, "submodule.recurse")) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା r = git_config_bool(k, v) ?
			RECURSE_SUBMODULES_ON : RECURSE_SUBMODULES_OFF;
		fetch_config->recurse_submodules = r;
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (!strcmp(k, "submodule.fetchjobs")) {
		fetch_config->submodule_fetch_jobs = parse_submodule_fetchjobs(k, v, ctx->kvi);
		ଫେରନ୍ତୁ 0;
	} ଅନ୍ୟଥା ଯଦି (!strcmp(k, "fetch.recursesubmodules")) {
		fetch_config->recurse_submodules = parse_fetch_recurse_submodules_arg(k, v);
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (!strcmp(k, "fetch.parallel")) {
		fetch_config->parallel = git_config_int(k, v, ctx->kvi);
		ଯଦି (fetch_config->parallel < 0)
			die(_("fetch.parallel cannot be negative"));
		ଯଦି (!fetch_config->parallel)
			fetch_config->parallel = online_cpus();
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (!strcmp(k, "fetch.output")) {
		ଯଦି (!v)
			ଫେରନ୍ତୁ config_error_nonbool(k);
		ଅନ୍ୟଥା ଯଦି (!strcasecmp(v, "full"))
			fetch_config->display_format = DISPLAY_FORMAT_FULL;
		ଅନ୍ୟଥା ଯଦି (!strcasecmp(v, "compact"))
			fetch_config->display_format = DISPLAY_FORMAT_COMPACT;
		ଅନ୍ୟଥା
			die(_("invalid value for '%s': '%s'"),
			    "fetch.output", v);
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (!strcmp(k, "fetch.followremotehead")) {
		ମୁକ୍ତ(fetch_config->follow_remote_head_raw);
		fetch_config->follow_remote_head_raw = xstrdup_or_null(v);
		fetch_config->follow_remote_head_seen = 1;
		ଫେରନ୍ତୁ 0;
	}

	ଫେରନ୍ତୁ git_default_config(k, v, ctx, cb);
}

ସ୍ଥିର ଗଣନା follow_remote_head_settings get_follow_remote_head(ଅଚଳ ଅକ୍ଷର *setting)
{
	ଯଦି (!setting)
		die(_("missing value for 'fetch.followRemoteHEAD'"));
	ଅନ୍ୟଥା ଯଦି (!strcmp(setting, "never"))
		ଫେରନ୍ତୁ FOLLOW_REMOTE_NEVER;
	ଅନ୍ୟଥା ଯଦି (!strcmp(setting, "create"))
		ଫେରନ୍ତୁ FOLLOW_REMOTE_CREATE;
	ଅନ୍ୟଥା ଯଦି (!strcmp(setting, "warn"))
		ଫେରନ୍ତୁ FOLLOW_REMOTE_WARN;
	ଅନ୍ୟଥା ଯଦି (!strcmp(setting, "always"))
		ଫେରନ୍ତୁ FOLLOW_REMOTE_ALWAYS;
	warning(_("unrecognized fetch.followRemoteHEAD value '%s' ignored"), setting);
	ଫେରନ୍ତୁ BUILTIN_FOLLOW_REMOTE_HEAD_DFLT;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା parse_refmap_arg(ଅଚଳ ସଂରଚନା option *opt, ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	BUG_ON_OPT_NEG(unset);

	/*
	 * "git fetch --refmap='' origin foo"
	 * can be used to tell the command not to store anywhere
	 */
	refspec_append(opt->ମୂଲ୍ୟ, arg);

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ unlock_pack(unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
	ଯଦି (gtransport)
		transport_unlock_pack(gtransport, flags);
	ଯଦି (gsecondary)
		transport_unlock_pack(gsecondary, flags);
}

ସ୍ଥିର ଶୂନ୍ୟ unlock_pack_atexit(ଶୂନ୍ୟ)
{
	unlock_pack(0);
}

ସ୍ଥିର ଶୂନ୍ୟ unlock_pack_on_signal(ପୂର୍ଣ୍ଣସଂଖ୍ୟା signo)
{
	unlock_pack(TRANSPORT_UNLOCK_PACK_IN_SIGNAL_HANDLER);
	sigchain_କାଢ଼(signo);
	ଉଠାଅ(signo);
}

ସ୍ଥିର ଶୂନ୍ୟ add_merge_config(ସଂରଚନା ref **head,
			   ଅଚଳ ସଂରଚନା ref *remote_refs,
		           ସଂରଚନା branch *branch,
		           ସଂରଚନା ref ***tail)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

	ପାଇଁ (i = 0; i < branch->merge_nr; i++) {
		ସଂରଚନା ref *rm, **old_tail = *tail;
		ସଂରଚନା refspec_item refspec;

		ପାଇଁ (rm = *head; rm; rm = rm->next) {
			ଯଦି (branch_merge_matches(branch, i, rm->name)) {
				rm->fetch_head_status = FETCH_HEAD_MERGE;
				ଭାଙ୍ଗ;
			}
		}
		ଯଦି (rm)
			ଜାରି;

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
		memset(&refspec, 0, ଆକାର(refspec));
		refspec.src = branch->merge[i]->src;
		get_fetch_map(remote_refs, &refspec, tail, 1);
		ପାଇଁ (rm = *old_tail; rm; rm = rm->next)
			rm->fetch_head_status = FETCH_HEAD_MERGE;
	}
}

ସ୍ଥିର ଶୂନ୍ୟ create_fetch_oidset(ସଂରଚନା ref **head, ସଂରଚନା oidset *out)
{
	ସଂରଚନା ref *rm = *head;
	ଯେତେବେଳେ (rm) {
		oidset_ଭର୍ତ୍ତି(out, &rm->old_oid);
		rm = rm->next;
	}
}

ସଂରଚନା refname_hash_entry {
	ସଂରଚନା hashmap_entry ent;
	ସଂରଚନା object_id oid;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ignore;
	ଅକ୍ଷର refname[FLEX_ARRAY];
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା refname_hash_entry_cmp(ଅଚଳ ଶୂନ୍ୟ *hashmap_cmp_fn_data UNUSED,
				  ଅଚଳ ସଂରଚନା hashmap_entry *eptr,
				  ଅଚଳ ସଂରଚନା hashmap_entry *entry_or_ଚାବି,
				  ଅଚଳ ଶୂନ୍ୟ *keydata)
{
	ଅଚଳ ସଂରଚନା refname_hash_entry *e1, *e2;

	e1 = container_of(eptr, ଅଚଳ ସଂରଚନା refname_hash_entry, ent);
	e2 = container_of(entry_or_ଚାବି, ଅଚଳ ସଂରଚନା refname_hash_entry, ent);
	ଫେରନ୍ତୁ strcmp(e1->refname, keydata ? keydata : e2->refname);
}

ସ୍ଥିର ସଂରଚନା refname_hash_entry *refname_hash_add(ସଂରଚନା hashmap *map,
						   ଅଚଳ ଅକ୍ଷର *refname,
						   ଅଚଳ ସଂରଚନା object_id *oid)
{
	ସଂରଚନା refname_hash_entry *ent;
	size_t len = strlen(refname);

	FLEX_ALLOC_MEM(ent, refname, refname, len);
	hashmap_entry_init(&ent->ent, strhash(refname));
	oidcpy(&ent->oid, oid);
	hashmap_add(map, &ent->ent);
	ଫେରନ୍ତୁ ent;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା add_one_refname(ଅଚଳ ସଂରଚନା reference *ref, ଶୂନ୍ୟ *cbdata)
{
	ସଂରଚନା hashmap *refname_map = cbdata;

	(ଶୂନ୍ୟ) refname_hash_add(refname_map, ref->name, ref->oid);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ refname_hash_init(ସଂରଚନା hashmap *map)
{
	hashmap_init(map, refname_hash_entry_cmp, ଖାଲି, 0);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା refname_hash_exists(ସଂରଚନା hashmap *map, ଅଚଳ ଅକ୍ଷର *refname)
{
	ଫେରନ୍ତୁ !!hashmap_get_from_hash(map, strhash(refname), refname);
}

ସ୍ଥିର ଶୂନ୍ୟ clear_item(ସଂରଚନା refname_hash_entry *item)
{
	item->ignore = 1;
}


ସ୍ଥିର ଶୂନ୍ୟ add_already_queued_tags(ଅଚଳ ଅକ୍ଷର *refname,
				    ଅଚଳ ସଂରଚନା object_id *old_oid UNUSED,
				    ଅଚଳ ସଂରଚନା object_id *new_oid,
				    ଶୂନ୍ୟ *cb_data)
{
	ସଂରଚନା hashmap *queued_tags = cb_data;
	ଯଦି (starts_with(refname, "refs/tags/") && new_oid)
		(ଶୂନ୍ୟ) refname_hash_add(queued_tags, refname, new_oid);
}

ସ୍ଥିର ଶୂନ୍ୟ find_non_local_tags(ଅଚଳ ସଂରଚନା ref *refs,
				ସଂରଚନା ref_transaction *transaction,
				ସଂରଚନା ref **head,
				ସଂରଚନା ref ***tail)
{
	ସଂରଚନା hashmap existing_refs;
	ସଂରଚନା hashmap remote_refs;
	ସଂରଚନା oidset fetch_oids = OIDSET_INIT;
	ସଂରଚନା string_list remote_refs_list = STRING_LIST_INIT_NODUP;
	ସଂରଚନା string_list_item *remote_ref_item;
	ଅଚଳ ସଂରଚନା ref *ref;
	ସଂରଚନା refname_hash_entry *item = ଖାଲି;

	refname_hash_init(&existing_refs);
	refname_hash_init(&remote_refs);
	create_fetch_oidset(head, &fetch_oids);

	refs_for_each_ref(get_main_ref_store(the_repository), add_one_refname,
			  &existing_refs);

	/*
	 * If we already have a transaction, then we need to filter out all
	 * tags which have already been queued up.
	 */
	ଯଦି (transaction)
		ref_transaction_for_each_queued_update(transaction,
						       add_already_queued_tags,
						       &existing_refs);

	ପାଇଁ (ref = refs; ref; ref = ref->next) {
		ଯଦି (!starts_with(ref->name, "refs/tags/"))
			ଜାରି;

		/*
		 * The peeled ref always follows the matching base
		 * ref, so if we see a peeled ref that we don't want
		 * to fetch then we can mark the ref entry in the list
		 * as one to ignore by setting util to NULL.
		 */
		ଯଦି (ends_with(ref->name, "^{}")) {
			ଯଦି (item &&
			    !odb_has_object(the_repository->objects, &ref->old_oid, 0) &&
			    !oidset_contains(&fetch_oids, &ref->old_oid) &&
			    !odb_has_object(the_repository->objects, &item->oid, 0) &&
			    !oidset_contains(&fetch_oids, &item->oid))
				clear_item(item);
			item = ଖାଲି;
			ଜାରି;
		}

		/*
		 * If item is non-NULL here, then we previously saw a
		 * ref not followed by a peeled reference, so we need
		 * to check if it is a lightweight tag that we want to
		 * fetch.
		 */
		ଯଦି (item &&
		    !odb_has_object(the_repository->objects, &item->oid, 0) &&
		    !oidset_contains(&fetch_oids, &item->oid))
			clear_item(item);

		item = ଖାଲି;

		/* skip duplicates and refs that we already have */
		ଯଦି (refname_hash_exists(&remote_refs, ref->name) ||
		    refname_hash_exists(&existing_refs, ref->name))
			ଜାରି;

		item = refname_hash_add(&remote_refs, ref->name, &ref->old_oid);
		string_list_ଭର୍ତ୍ତି(&remote_refs_list, ref->name);
	}
	hashmap_clear_and_free(&existing_refs, ସଂରଚନା refname_hash_entry, ent);

	/*
	 * We may have a final lightweight tag that needs to be
	 * checked to see if it needs fetching.
	 */
	ଯଦି (item &&
	    !odb_has_object(the_repository->objects, &item->oid, 0) &&
	    !oidset_contains(&fetch_oids, &item->oid))
		clear_item(item);

	/*
	 * For all the tags in the remote_refs_list,
	 * add them to the list of refs to be fetched
	 */
	for_each_string_list_item(remote_ref_item, &remote_refs_list) {
		ଅଚଳ ଅକ୍ଷର *refname = remote_ref_item->string;
		ସଂରଚନା ref *rm;
		unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା hash = strhash(refname);

		item = hashmap_get_entry_from_hash(&remote_refs, hash, refname,
					ସଂରଚନା refname_hash_entry, ent);
		ଯଦି (!item)
			BUG("unseen remote ref?");

		/* Unless we have already decided to ignore this item... */
		ଯଦି (item->ignore)
			ଜାରି;

		rm = alloc_ref(item->refname);
		rm->peer_ref = alloc_ref(item->refname);
		oidcpy(&rm->old_oid, &item->oid);
		**tail = rm;
		*tail = &rm->next;
	}
	hashmap_clear_and_free(&remote_refs, ସଂରଚନା refname_hash_entry, ent);
	string_list_clear(&remote_refs_list, 0);
	oidset_clear(&fetch_oids);
}

ସ୍ଥିର ଶୂନ୍ୟ filter_prefetch_refspec(ସଂରଚନା refspec *rs)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

	ଯଦି (!prefetch)
		ଫେରନ୍ତୁ;

	ପାଇଁ (i = 0; i < rs->nr; i++) {
		ସଂରଚନା strbuf new_dst = STRBUF_INIT;
		ଅକ୍ଷର *old_dst;
		ଅଚଳ ଅକ୍ଷର *sub = ଖାଲି;

		ଯଦି (rs->ବସ୍ତୁଗୁଡ଼ିକ[i].negative)
			ଜାରି;
		ଯଦି (!rs->ବସ୍ତୁଗୁଡ଼ିକ[i].dst ||
		    (rs->ବସ୍ତୁଗୁଡ଼ିକ[i].src &&
		     starts_with(rs->ବସ୍ତୁଗୁଡ଼ିକ[i].src,
				 ref_namespace[NAMESPACE_TAGS].ref))) {
			ପୂର୍ଣ୍ଣସଂଖ୍ୟା j;

			refspec_item_clear(&rs->ବସ୍ତୁଗୁଡ଼ିକ[i]);

			ପାଇଁ (j = i + 1; j < rs->nr; j++)
				rs->ବସ୍ତୁଗୁଡ଼ିକ[j - 1] = rs->ବସ୍ତୁଗୁଡ଼ିକ[j];
			rs->nr--;
			i--;
			ଜାରି;
		}

		old_dst = rs->ବସ୍ତୁଗୁଡ଼ିକ[i].dst;
		strbuf_addstr(&new_dst, ref_namespace[NAMESPACE_PREFETCH].ref);

		/*
		 * If old_dst starts with "refs/", then place
		 * sub after that prefix. Otherwise, start at
		 * the beginning of the string.
		 */
		ଯଦି (!skip_prefix(old_dst, "refs/", &sub))
			sub = old_dst;
		strbuf_addstr(&new_dst, sub);

		rs->ବସ୍ତୁଗୁଡ଼ିକ[i].dst = strbuf_detach(&new_dst, ଖାଲି);
		rs->ବସ୍ତୁଗୁଡ଼ିକ[i].force = 1;

		ମୁକ୍ତ(old_dst);
	}
}

ସ୍ଥିର ସଂରଚନା ref *get_ref_map(ସଂରଚନା remote *remote,
			       ଅଚଳ ସଂରଚନା ref *remote_refs,
			       ସଂରଚନା refspec *rs,
			       ପୂର୍ଣ୍ଣସଂଖ୍ୟା tags, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *autotags)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
	ସଂରଚନା ref *rm;
	ସଂରଚନା ref *ref_map = ଖାଲି;
	ସଂରଚନା ref **tail = &ref_map;

	/* opportunistically-updated references: */
	ସଂରଚନା ref *orefs = ଖାଲି, **oref_tail = &orefs;

	ସଂରଚନା hashmap existing_refs;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା existing_refs_populated = 0;

	filter_prefetch_refspec(rs);
	ଯଦି (remote)
		filter_prefetch_refspec(&remote->fetch);

	ଯଦି (rs->nr) {
		ସଂରଚନା refspec *fetch_refspec;

		ପାଇଁ (i = 0; i < rs->nr; i++) {
			get_fetch_map(remote_refs, &rs->ବସ୍ତୁଗୁଡ଼ିକ[i], &tail, 0);
			ଯଦି (rs->ବସ୍ତୁଗୁଡ଼ିକ[i].dst && rs->ବସ୍ତୁଗୁଡ଼ିକ[i].dst[0])
				*autotags = 1;
		}
		/* Merge everything on the command line (but not --tags) */
		ପାଇଁ (rm = ref_map; rm; rm = rm->next)
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
		ଯଦି (refmap.nr)
			fetch_refspec = &refmap;
		ଅନ୍ୟଥା
			fetch_refspec = &remote->fetch;

		ପାଇଁ (i = 0; i < fetch_refspec->nr; i++)
			get_fetch_map(ref_map, &fetch_refspec->ବସ୍ତୁଗୁଡ଼ିକ[i], &oref_tail, 1);
	} ଅନ୍ୟଥା ଯଦି (refmap.nr) {
		die("--refmap option is only meaningful with command-line refspec(s)");
	} ଅନ୍ୟଥା {
		/* Use the defaults */
		ସଂରଚନା branch *branch = branch_get(ଖାଲି);
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା has_merge = branch_has_merge_config(branch);
		ଯଦି (remote &&
		    (remote->fetch.nr ||
		     /* Note: has_merge implies non-NULL branch->remote_name */
		     (has_merge && !strcmp(branch->remote_name, remote->name)))) {
			ପାଇଁ (i = 0; i < remote->fetch.nr; i++) {
				get_fetch_map(remote_refs, &remote->fetch.ବସ୍ତୁଗୁଡ଼ିକ[i], &tail, 0);
				ଯଦି (remote->fetch.ବସ୍ତୁଗୁଡ଼ିକ[i].dst &&
				    remote->fetch.ବସ୍ତୁଗୁଡ଼ିକ[i].dst[0])
					*autotags = 1;
				ଯଦି (!i && !has_merge && ref_map &&
				    !remote->fetch.ବସ୍ତୁଗୁଡ଼ିକ[0].pattern)
					ref_map->fetch_head_status = FETCH_HEAD_MERGE;
			}
			/*
			 * if the remote we're fetching from is the same
			 * as given in branch.<name>.remote, we add the
			 * ref given in branch.<name>.merge, too.
			 *
			 * Note: has_merge implies non-NULL branch->remote_name
			 */
			ଯଦି (has_merge &&
			    !strcmp(branch->remote_name, remote->name))
				add_merge_config(&ref_map, remote_refs, branch, &tail);
		} ଅନ୍ୟଥା ଯଦି (!prefetch) {
			ref_map = get_remote_ref(remote_refs, "HEAD");
			ଯଦି (!ref_map)
				die(_("couldn't find remote ref HEAD"));
			ref_map->fetch_head_status = FETCH_HEAD_MERGE;
			tail = &ref_map->next;
		}
	}

	ଯଦି (tags == TAGS_SET) {
		ସଂରଚନା refspec_item tag_refspec;

		/* also fetch all tags */
		refspec_item_init_ଠେଲ(&tag_refspec, TAG_REFSPEC,
				       the_hash_algo);
		get_fetch_map(remote_refs, &tag_refspec, &tail, 0);
		refspec_item_clear(&tag_refspec);
	} ଅନ୍ୟଥା ଯଦି (tags == TAGS_DEFAULT && *autotags) {
		find_non_local_tags(remote_refs, ଖାଲି, &ref_map, &tail);
	}

	/* Now append any refs to be updated opportunistically: */
	*tail = orefs;
	ପାଇଁ (rm = orefs; rm; rm = rm->next) {
		rm->fetch_head_status = FETCH_HEAD_IGNORE;
		tail = &rm->next;
	}

	/*
	 * apply negative refspecs first, before we remove duplicates. This is
	 * necessary as negative refspecs might remove an otherwise conflicting
	 * duplicate.
	 */
	ଯଦି (rs->nr)
		ref_map = apply_negative_refspecs(ref_map, rs);
	ଅନ୍ୟଥା
		ref_map = apply_negative_refspecs(ref_map, &remote->fetch);

	ref_map = ref_remove_duplicates(ref_map);

	ପାଇଁ (rm = ref_map; rm; rm = rm->next) {
		ଯଦି (rm->peer_ref) {
			ଅଚଳ ଅକ୍ଷର *refname = rm->peer_ref->name;
			ସଂରଚନା refname_hash_entry *peer_item;
			unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା hash = strhash(refname);

			ଯଦି (!existing_refs_populated) {
				refname_hash_init(&existing_refs);
				refs_for_each_ref(get_main_ref_store(the_repository),
						  add_one_refname,
						  &existing_refs);
				existing_refs_populated = 1;
			}

			peer_item = hashmap_get_entry_from_hash(&existing_refs,
						hash, refname,
						ସଂରଚନା refname_hash_entry, ent);
			ଯଦି (peer_item) {
				ସଂରଚନା object_id *old_oid = &peer_item->oid;
				oidcpy(&rm->peer_ref->old_oid, old_oid);
			}
		}
	}
	ଯଦି (existing_refs_populated)
		hashmap_clear_and_free(&existing_refs, ସଂରଚନା refname_hash_entry, ent);

	ଫେରନ୍ତୁ ref_map;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା s_update_ref(ଅଚଳ ଅକ୍ଷର *action,
			ସଂରଚନା ref *ref,
			ସଂରଚନା ref_transaction *transaction,
			ପୂର୍ଣ୍ଣସଂଖ୍ୟା check_old)
{
	ଅକ୍ଷର *ସନ୍ଦେଶ;
	ଅକ୍ଷର *rla = getenv("GIT_REFLOG_ACTION");
	ସଂରଚନା strbuf err = STRBUF_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	ଯଦି (dry_run)
		ଫେରନ୍ତୁ 0;
	ଯଦି (!rla)
		rla = default_rla.buf;
	ସନ୍ଦେଶ = xstrfmt("%s: %s", rla, action);

	ret = ref_transaction_update(transaction, ref->name, &ref->new_oid,
				     check_old ? &ref->old_oid : ଖାଲି,
				     ଖାଲି, ଖାଲି, 0, ସନ୍ଦେଶ, &err);

	ଯଦି (ret)
		error("%s", err.buf);
	strbuf_release(&err);
	ମୁକ୍ତ(ସନ୍ଦେଶ);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା refcol_width(ଅଚଳ ସଂରଚନା ref *ref_map, ପୂର୍ଣ୍ଣସଂଖ୍ୟା compact_format)
{
	ଅଚଳ ସଂରଚନା ref *ref;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା max, width = 10;

	max = term_columns();
	ଯଦି (compact_format)
		max = max * 2 / 3;

	ପାଇଁ (ref = ref_map; ref; ref = ref->next) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା rlen, llen = 0, len;

		ଯଦି (ref->status == REF_STATUS_REJECT_SHALLOW ||
		    !ref->peer_ref ||
		    !strcmp(ref->name, "HEAD"))
			ଜାରି;

		/* uptodate lines are only shown on high verbosity level */
		ଯଦି (verbosity <= 0 && oideq(&ref->peer_ref->old_oid, &ref->old_oid))
			ଜାରି;

		rlen = utf8_strwidth(prettify_refname(ref->name));
		ଯଦି (!compact_format)
			llen = utf8_strwidth(prettify_refname(ref->peer_ref->name));

		/*
		 * rough estimation to see if the output line is too long and
		 * should not be counted (we can't do precise calculation
		 * anyway because we don't know if the error explanation part
		 * will be printed in update_local_ref)
		 */
		len = 21 /* flag and summary */ + rlen + 4 /* -> */ + llen;
		ଯଦି (len >= max)
			ଜାରି;

		ଯଦି (width < rlen)
			width = rlen;
	}

	ଫେରନ୍ତୁ width;
}

ସ୍ଥିର ଶୂନ୍ୟ display_state_init(ସଂରଚନା display_state *display_state, ସଂରଚନା ref *ref_map,
			       ଅଚଳ ଅକ୍ଷର *raw_url, ଗଣନା display_format format)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

	memset(display_state, 0, ଆକାର(*display_state));
	strbuf_init(&display_state->buf, 0);
	display_state->format = format;

	ଯଦି (raw_url)
		display_state->url = transport_anonymize_url(raw_url);
	ଅନ୍ୟଥା
		display_state->url = xstrdup("foreign");

	display_state->url_len = strlen(display_state->url);
	ପାଇଁ (i = display_state->url_len - 1; 0 <= i && display_state->url[i] == '/'; i--)
		;
	display_state->url_len = i + 1;
	ଯଦି (4 < i && !strncmp(".git", display_state->url + i - 3, 4))
		display_state->url_len = i - 3;

	ଯଦି (verbosity < 0)
		ଫେରନ୍ତୁ;

	ବାଛ (display_state->format) {
	ସ୍ଥିତି DISPLAY_FORMAT_FULL:
	ସ୍ଥିତି DISPLAY_FORMAT_COMPACT:
		display_state->refcol_width = refcol_width(ref_map,
							   display_state->format == DISPLAY_FORMAT_COMPACT);
		ଭାଙ୍ଗ;
	ସ୍ଥିତି DISPLAY_FORMAT_PORCELAIN:
		/* We don't need to precompute anything here. */
		ଭାଙ୍ଗ;
	ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
		BUG("unexpected display format %d", display_state->format);
	}
}

ସ୍ଥିର ଶୂନ୍ୟ display_state_release(ସଂରଚନା display_state *display_state)
{
	strbuf_release(&display_state->buf);
	ମୁକ୍ତ(display_state->url);
}

ସ୍ଥିର ଶୂନ୍ୟ print_remote_to_local(ସଂରଚନା display_state *display_state,
				  ଅଚଳ ଅକ୍ଷର *remote, ଅଚଳ ଅକ୍ଷର *local)
{
	strbuf_addf(&display_state->buf, "%-*s -> %s",
		    display_state->refcol_width, remote, local);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା find_and_replace(ସଂରଚନା strbuf *haystack,
			    ଅଚଳ ଅକ୍ଷର *needle,
			    ଅଚଳ ଅକ୍ଷର *placeholder)
{
	ଅଚଳ ଅକ୍ଷର *p = ଖାଲି;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା plen, nlen;

	nlen = strlen(needle);
	ଯଦି (ends_with(haystack->buf, needle))
		p = haystack->buf + haystack->len - nlen;
	ଅନ୍ୟଥା
		p = strstr(haystack->buf, needle);
	ଯଦି (!p)
		ଫେରନ୍ତୁ 0;

	ଯଦି (p > haystack->buf && p[-1] != '/')
		ଫେରନ୍ତୁ 0;

	plen = strlen(p);
	ଯଦି (plen > nlen && p[nlen] != '/')
		ଫେରନ୍ତୁ 0;

	strbuf_splice(haystack, p - haystack->buf, nlen,
		      placeholder, strlen(placeholder));
	ଫେରନ୍ତୁ 1;
}

ସ୍ଥିର ଶୂନ୍ୟ print_compact(ସଂରଚନା display_state *display_state,
			  ଅଚଳ ଅକ୍ଷର *remote, ଅଚଳ ଅକ୍ଷର *local)
{
	ସଂରଚନା strbuf r = STRBUF_INIT;
	ସଂରଚନା strbuf l = STRBUF_INIT;

	ଯଦି (!strcmp(remote, local)) {
		strbuf_addf(&display_state->buf, "%-*s -> *", display_state->refcol_width, remote);
		ଫେରନ୍ତୁ;
	}

	strbuf_addstr(&r, remote);
	strbuf_addstr(&l, local);

	ଯଦି (!find_and_replace(&r, local, "*"))
		find_and_replace(&l, remote, "*");
	print_remote_to_local(display_state, r.buf, l.buf);

	strbuf_release(&r);
	strbuf_release(&l);
}

ସ୍ଥିର ଶୂନ୍ୟ display_ref_update(ସଂରଚନା display_state *display_state, ଅକ୍ଷର code,
			       ଅଚଳ ଅକ୍ଷର *summary, ଅଚଳ ଅକ୍ଷର *error,
			       ଅଚଳ ଅକ୍ଷର *remote, ଅଚଳ ଅକ୍ଷର *local,
			       ଅଚଳ ସଂରଚନା object_id *old_oid,
			       ଅଚଳ ସଂରଚନା object_id *new_oid,
			       ପୂର୍ଣ୍ଣସଂଖ୍ୟା summary_width)
{
	FILE *f = stderr;

	ଯଦି (verbosity < 0)
		ଫେରନ୍ତୁ;

	strbuf_reset(&display_state->buf);

	ବାଛ (display_state->format) {
	ସ୍ଥିତି DISPLAY_FORMAT_FULL:
	ସ୍ଥିତି DISPLAY_FORMAT_COMPACT: {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା width;

		ଯଦି (!display_state->shown_url) {
			strbuf_addf(&display_state->buf, _("From %.*s\n"),
				    display_state->url_len, display_state->url);
			display_state->shown_url = 1;
		}

		width = (summary_width + strlen(summary) - gettext_width(summary));
		remote = prettify_refname(remote);
		local = prettify_refname(local);

		strbuf_addf(&display_state->buf, " %c %-*s ", code, width, summary);

		ଯଦି (display_state->format != DISPLAY_FORMAT_COMPACT)
			print_remote_to_local(display_state, remote, local);
		ଅନ୍ୟଥା
			print_compact(display_state, remote, local);

		ଯଦି (error)
			strbuf_addf(&display_state->buf, "  (%s)", error);

		ଭାଙ୍ଗ;
	}
	ସ୍ଥିତି DISPLAY_FORMAT_PORCELAIN:
		strbuf_addf(&display_state->buf, "%c %s %s %s", code,
			    oid_to_hex(old_oid), oid_to_hex(new_oid), local);
		f = stdout;
		ଭାଙ୍ଗ;
	ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
		BUG("unexpected display format %d", display_state->format);
	};
	strbuf_addch(&display_state->buf, '\n');

	fputs(display_state->buf.buf, f);
}

ସଂରଚନା ref_update_display_info {
	bool failed;
	ଅକ୍ଷର success_code;
	ଅକ୍ଷର fail_code;
	ଅକ୍ଷର *summary;
	ଅକ୍ଷର *fail_detail;
	ଅକ୍ଷର *success_detail;
	ଅକ୍ଷର *ref;
	ଅକ୍ଷର *remote;
	ସଂରଚନା object_id old_oid;
	ସଂରଚନା object_id new_oid;
};

ସଂରଚନା ref_update_display_info_array {
	ସଂରଚନା ref_update_display_info *info;
	size_t alloc, nr;
};

ସ୍ଥିର ସଂରଚନା ref_update_display_info *ref_update_display_info_append(
					   ସଂରଚନା ref_update_display_info_array *array,
					   ଅକ୍ଷର success_code,
					   ଅକ୍ଷର fail_code,
					   ଅଚଳ ଅକ୍ଷର *summary,
					   ଅଚଳ ଅକ୍ଷର *success_detail,
					   ଅଚଳ ଅକ୍ଷର *fail_detail,
					   ଅଚଳ ଅକ୍ଷର *ref,
					   ଅଚଳ ଅକ୍ଷର *remote,
					   ଅଚଳ ସଂରଚନା object_id *old_oid,
					   ଅଚଳ ସଂରଚନା object_id *new_oid)
{
	ସଂରଚନା ref_update_display_info *info;

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

	ଫେରନ୍ତୁ info;
}

ସ୍ଥିର ଶୂନ୍ୟ ref_update_display_info_set_failed(ସଂରଚନା ref_update_display_info *info)
{
	info->failed = true;
}

ସ୍ଥିର ଶୂନ୍ୟ ref_update_display_info_free(ସଂରଚନା ref_update_display_info *info)
{
	ମୁକ୍ତ(info->summary);
	ମୁକ୍ତ(info->success_detail);
	ମୁକ୍ତ(info->fail_detail);
	ମୁକ୍ତ(info->remote);
	ମୁକ୍ତ(info->ref);
}

ସ୍ଥିର ଶୂନ୍ୟ ref_update_display_info_display(ସଂରଚନା ref_update_display_info *info,
					    ସଂରଚନା display_state *display_state,
					    ପୂର୍ଣ୍ଣସଂଖ୍ୟା summary_width)
{
	display_ref_update(display_state,
			   info->failed ? info->fail_code : info->success_code,
			   info->summary,
			   info->failed ? info->fail_detail : info->success_detail,
			   info->remote, info->ref, &info->old_oid,
			   &info->new_oid, summary_width);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା update_local_ref(ସଂରଚନା ref *ref,
			    ସଂରଚନା ref_transaction *transaction,
			    ଅଚଳ ସଂରଚନା ref *remote_ref,
			    ଅଚଳ ସଂରଚନା fetch_config *config,
			    ସଂରଚନା ref_update_display_info_array *display_array)
{
	ସଂରଚନା commit *current = ଖାଲି, *updated;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା fast_forward = 0;

	ଯଦି (!odb_has_object(the_repository->objects, &ref->new_oid,
			    ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR))
		die(_("object %s not found"), oid_to_hex(&ref->new_oid));

	ଯଦି (oideq(&ref->old_oid, &ref->new_oid)) {
		ଯଦି (verbosity > 0)
			ref_update_display_info_append(display_array, '=', '=',
						       _("[up to date]"), ଖାଲି,
						       ଖାଲି, ref->name,
						       remote_ref->name, &ref->old_oid,
						       &ref->new_oid);
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (!update_head_ok &&
	    !is_null_oid(&ref->old_oid) &&
	    branch_checked_out(ref->name)) {
		ସଂରଚନା ref_update_display_info *info;
		/*
		 * If this is the head, and it's not okay to update
		 * the head, and the old value of the head isn't empty...
		 */
		info = ref_update_display_info_append(display_array, '!', '!',
						      _("[rejected]"), ଖାଲି,
						      _("can't fetch into checked-out branch"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);
		ref_update_display_info_set_failed(info);
		ଫେରନ୍ତୁ 1;
	}

	ଯଦି (!is_null_oid(&ref->old_oid) &&
	    starts_with(ref->name, "refs/tags/")) {
		ସଂରଚନା ref_update_display_info *info;

		ଯଦି (force || ref->force) {
			ପୂର୍ଣ୍ଣସଂଖ୍ୟା r;

			r = s_update_ref("updating tag", ref, transaction, 0);

			info = ref_update_display_info_append(display_array, 't', '!',
							      _("[tag update]"), ଖାଲି,
							      _("unable to update local ref"),
							      ref->name, remote_ref->name,
							      &ref->old_oid, &ref->new_oid);
			ଯଦି (r)
				ref_update_display_info_set_failed(info);

			ଫେରନ୍ତୁ r;
		} ଅନ୍ୟଥା {
			info = ref_update_display_info_append(display_array, '!', '!',
							      _("[rejected]"), ଖାଲି,
							      _("would clobber existing tag"),
							      ref->name, remote_ref->name,
							      &ref->old_oid, &ref->new_oid);
			ref_update_display_info_set_failed(info);
			ଫେରନ୍ତୁ 1;
		}
	}

	current = lookup_commit_reference_gently(the_repository,
						 &ref->old_oid, 1);
	updated = lookup_commit_reference_gently(the_repository,
						 &ref->new_oid, 1);
	ଯଦି (!current || !updated) {
		ସଂରଚନା ref_update_display_info *info;
		ଅଚଳ ଅକ୍ଷର *ସନ୍ଦେଶ;
		ଅଚଳ ଅକ୍ଷର *what;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା r;
		/*
		 * Nicely describe the new ref we're fetching.
		 * Base this on the remote's ref name, as it's
		 * more likely to follow a standard layout.
		 */
		ଯଦି (starts_with(remote_ref->name, "refs/tags/")) {
			ସନ୍ଦେଶ = "storing tag";
			what = _("[new tag]");
		} ଅନ୍ୟଥା ଯଦି (starts_with(remote_ref->name, "refs/heads/")) {
			ସନ୍ଦେଶ = "storing head";
			what = _("[new branch]");
		} ଅନ୍ୟଥା {
			ସନ୍ଦେଶ = "storing ref";
			what = _("[new ref]");
		}

		r = s_update_ref(ସନ୍ଦେଶ, ref, transaction, 0);

		info = ref_update_display_info_append(display_array, '*', '!',
						      what, ଖାଲି,
						      _("unable to update local ref"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);
		ଯଦି (r)
			ref_update_display_info_set_failed(info);

		ଫେରନ୍ତୁ r;
	}

	ଯଦି (config->show_forced_updates) {
		uint64_t t_before = getnanotime();
		fast_forward = repo_in_merge_bases(the_repository, current,
						   updated);
		ଯଦି (fast_forward < 0)
			die(ଖାଲି);
		forced_updates_ms += (getnanotime() - t_before) / 1000000;
	} ଅନ୍ୟଥା {
		fast_forward = 1;
	}

	ଯଦି (fast_forward) {
		ସଂରଚନା ref_update_display_info *info;
		ସଂରଚନା strbuf quickref = STRBUF_INIT;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା r;

		strbuf_add_unique_abbrev(&quickref, &current->object.oid, DEFAULT_ABBREV);
		strbuf_addstr(&quickref, "..");
		strbuf_add_unique_abbrev(&quickref, &ref->new_oid, DEFAULT_ABBREV);
		r = s_update_ref("fast-forward", ref, transaction, 1);

		info = ref_update_display_info_append(display_array, ' ', '!',
						      quickref.buf, ଖାଲି,
						      _("unable to update local ref"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);
		ଯଦି (r)
			ref_update_display_info_set_failed(info);

		strbuf_release(&quickref);
		ଫେରନ୍ତୁ r;
	} ଅନ୍ୟଥା ଯଦି (force || ref->force) {
		ସଂରଚନା ref_update_display_info *info;
		ସଂରଚନା strbuf quickref = STRBUF_INIT;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା r;

		strbuf_add_unique_abbrev(&quickref, &current->object.oid, DEFAULT_ABBREV);
		strbuf_addstr(&quickref, "...");
		strbuf_add_unique_abbrev(&quickref, &ref->new_oid, DEFAULT_ABBREV);
		r = s_update_ref("forced-update", ref, transaction, 1);

		info = ref_update_display_info_append(display_array, '+', '!',
						      quickref.buf, _("forced update"),
						      _("unable to update local ref"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);

		ଯଦି (r)
			ref_update_display_info_set_failed(info);

		strbuf_release(&quickref);
		ଫେରନ୍ତୁ r;
	} ଅନ୍ୟଥା {
		ସଂରଚନା ref_update_display_info *info;
		info = ref_update_display_info_append(display_array, '!', '!',
						      _("[rejected]"), ଖାଲି,
						      _("non-fast-forward"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);
		ref_update_display_info_set_failed(info);
		ଫେରନ୍ତୁ 1;
	}
}

ସ୍ଥିର ଅଚଳ ସଂରଚନା object_id *iterate_ref_map(ଶୂନ୍ୟ *cb_data)
{
	ସଂରଚନା ref **rm = cb_data;
	ସଂରଚନା ref *ref = *rm;

	ଯେତେବେଳେ (ref && ref->status == REF_STATUS_REJECT_SHALLOW)
		ref = ref->next;
	ଯଦି (!ref)
		ଫେରନ୍ତୁ ଖାଲି;
	*rm = ref->next;
	ଫେରନ୍ତୁ &ref->old_oid;
}

ସଂରଚନା fetch_head {
	FILE *fp;
	ସଂରଚନା strbuf buf;
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା open_fetch_head(ସଂରଚନା fetch_head *fetch_head)
{
	ଅଚଳ ଅକ୍ଷର *filename = git_path_fetch_head(the_repository);

	ଯଦି (write_fetch_head) {
		fetch_head->fp = fopen(filename, "a");
		ଯଦି (!fetch_head->fp)
			ଫେରନ୍ତୁ error_errno(_("cannot open '%s'"), filename);
		strbuf_init(&fetch_head->buf, 0);
	} ଅନ୍ୟଥା {
		fetch_head->fp = ଖାଲି;
	}

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ append_fetch_head(ସଂରଚନା fetch_head *fetch_head,
			      ଅଚଳ ସଂରଚନା object_id *old_oid,
			      ଗଣନା fetch_head_status fetch_head_status,
			      ଅଚଳ ଅକ୍ଷର *note,
			      ଅଚଳ ଅକ୍ଷର *url, size_t url_len)
{
	ଅକ୍ଷର old_oid_hex[GIT_MAX_HEXSZ + 1];
	ଅଚଳ ଅକ୍ଷର *merge_status_marker;
	size_t i;

	ଯଦି (!fetch_head->fp)
		ଫେରନ୍ତୁ;

	ବାଛ (fetch_head_status) {
	ସ୍ଥିତି FETCH_HEAD_NOT_FOR_MERGE:
		merge_status_marker = "not-for-merge";
		ଭାଙ୍ଗ;
	ସ୍ଥିତି FETCH_HEAD_MERGE:
		merge_status_marker = "";
		ଭାଙ୍ଗ;
	ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
		/* do not write anything to FETCH_HEAD */
		ଫେରନ୍ତୁ;
	}

	strbuf_addf(&fetch_head->buf, "%s\t%s\t%s",
		    oid_to_hex_r(old_oid_hex, old_oid), merge_status_marker, note);
	ପାଇଁ (i = 0; i < url_len; ++i)
		ଯଦି ('\n' == url[i])
			strbuf_addstr(&fetch_head->buf, "\\n");
		ଅନ୍ୟଥା
			strbuf_addch(&fetch_head->buf, url[i]);
	strbuf_addch(&fetch_head->buf, '\n');

	/*
	 * When using an atomic fetch, we do not want to update FETCH_HEAD if
	 * any of the reference updates fails. We thus have to write all
	 * updates to a buffer first and only commit it as soon as all
	 * references have been successfully updated.
	 */
	ଯଦି (!atomic_fetch) {
		strbuf_write(&fetch_head->buf, fetch_head->fp);
		strbuf_reset(&fetch_head->buf);
	}
}

ସ୍ଥିର ଶୂନ୍ୟ commit_fetch_head(ସଂରଚନା fetch_head *fetch_head)
{
	ଯଦି (!fetch_head->fp || !atomic_fetch)
		ଫେରନ୍ତୁ;
	strbuf_write(&fetch_head->buf, fetch_head->fp);
}

ସ୍ଥିର ଶୂନ୍ୟ close_fetch_head(ସଂରଚନା fetch_head *fetch_head)
{
	ଯଦି (!fetch_head->fp)
		ଫେରନ୍ତୁ;

	fclose(fetch_head->fp);
	strbuf_release(&fetch_head->buf);
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର warn_show_forced_updates[] =
N_("fetch normally indicates which branches had a forced update,\n"
   "but that check has been disabled; to re-enable, use '--show-forced-updates'\n"
   "flag or run 'git config fetch.showForcedUpdates true'");
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର warn_time_show_forced_updates[] =
N_("it took %.2f seconds to check forced updates; you can use\n"
   "'--no-show-forced-updates' or run 'git config fetch.showForcedUpdates false'\n"
   "to avoid this check\n");

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା store_updated_refs(ସଂରଚନା display_state *display_state,
			      ପୂର୍ଣ୍ଣସଂଖ୍ୟା connectivity_checked,
			      ସଂରଚନା ref_transaction *transaction, ସଂରଚନା ref *ref_map,
			      ସଂରଚନା fetch_head *fetch_head,
			      ଅଚଳ ସଂରଚନା fetch_config *config,
			      ସଂରଚନା ref_update_display_info_array *display_array)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା rc = 0;
	ସଂରଚନା strbuf note = STRBUF_INIT;
	ଅଚଳ ଅକ୍ଷର *what, *kind;
	ସଂରଚନା ref *rm;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା want_status;

	ଯଦି (!connectivity_checked) {
		ସଂରଚନା check_connected_options opt = CHECK_CONNECTED_INIT;

		opt.exclude_hidden_refs_section = "fetch";
		rm = ref_map;
		ଯଦି (check_connected(iterate_ref_map, &rm, &opt)) {
			rc = error(_("%s did not send all necessary objects"),
				   display_state->url);
			ଯାଅ abort;
		}
	}

	/*
	 * We do a pass for each fetch_head_status type in their enum order, so
	 * merged entries are written before not-for-merge. That lets readers
	 * use FETCH_HEAD as a refname to refer to the ref to be merged.
	 */
	ପାଇଁ (want_status = FETCH_HEAD_MERGE;
	     want_status <= FETCH_HEAD_IGNORE;
	     want_status++) {
		ପାଇଁ (rm = ref_map; rm; rm = rm->next) {
			ସଂରଚନା ref *ref = ଖାଲି;

			ଯଦି (rm->status == REF_STATUS_REJECT_SHALLOW) {
				ଯଦି (want_status == FETCH_HEAD_MERGE)
					warning(_("rejected %s because shallow roots are not allowed to be updated"),
						rm->peer_ref ? rm->peer_ref->name : rm->name);
				ଜାରି;
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
			ଯଦି (fetch_head->fp) {
				ସଂରଚନା commit *commit = ଖାଲି;

				/*
				 * References in "refs/tags/" are often going to point
				 * to annotated tags, which are not part of the
				 * commit-graph. We thus only try to look up refs in
				 * the graph which are not in that namespace to not
				 * regress performance in repositories with many
				 * annotated tags.
				 */
				ଯଦି (!starts_with(rm->name, "refs/tags/"))
					commit = lookup_commit_in_graph(the_repository, &rm->old_oid);
				ଯଦି (!commit) {
					commit = lookup_commit_reference_gently(the_repository,
										&rm->old_oid,
										1);
					ଯଦି (!commit)
						rm->fetch_head_status = FETCH_HEAD_NOT_FOR_MERGE;
				}
			}

			ଯଦି (rm->fetch_head_status != want_status)
				ଜାରି;

			ଯଦି (rm->peer_ref) {
				ref = alloc_ref(rm->peer_ref->name);
				oidcpy(&ref->old_oid, &rm->peer_ref->old_oid);
				oidcpy(&ref->new_oid, &rm->old_oid);
				ref->force = rm->peer_ref->force;
			}

			ଯଦି (config->recurse_submodules != RECURSE_SUBMODULES_OFF &&
			    (!rm->peer_ref || !oideq(&ref->old_oid, &ref->new_oid))) {
				check_for_new_submodule_commits(&rm->old_oid);
			}

			ଯଦି (!strcmp(rm->name, "HEAD")) {
				kind = "";
				what = "";
			} ଅନ୍ୟଥା ଯଦି (skip_prefix(rm->name, "refs/heads/", &what)) {
				kind = "branch";
			} ଅନ୍ୟଥା ଯଦି (skip_prefix(rm->name, "refs/tags/", &what)) {
				kind = "tag";
			} ଅନ୍ୟଥା ଯଦି (skip_prefix(rm->name, "refs/remotes/", &what)) {
				kind = "remote-tracking branch";
			} ଅନ୍ୟଥା {
				kind = "";
				what = rm->name;
			}

			strbuf_reset(&note);
			ଯଦି (*what) {
				ଯଦି (*kind)
					strbuf_addf(&note, "%s ", kind);
				strbuf_addf(&note, "'%s' of ", what);
			}

			append_fetch_head(fetch_head, &rm->old_oid,
					  rm->fetch_head_status,
					  note.buf, display_state->url,
					  display_state->url_len);

			ଯଦି (ref) {
				rc |= update_local_ref(ref, transaction, rm,
						       config, display_array);
				ମୁକ୍ତ(ref);
			} ଅନ୍ୟଥା ଯଦି (write_fetch_head || dry_run) {
				/*
				 * Display fetches written to FETCH_HEAD (or
				 * would be written to FETCH_HEAD, if --dry-run
				 * is set).
				 */

				ref_update_display_info_append(display_array, '*', '*',
							       *kind ? kind : "branch",
							       ଖାଲି, ଖାଲି, "FETCH_HEAD",
							       rm->name, &rm->new_oid,
							       &rm->old_oid);
			}
		}
	}

	ଯଦି (advice_enabled(ADVICE_FETCH_SHOW_FORCED_UPDATES)) {
		ଯଦି (!config->show_forced_updates) {
			warning(_(warn_show_forced_updates));
		} ଅନ୍ୟଥା ଯଦି (forced_updates_ms > FORCED_UPDATES_DELAY_WARNING_IN_MS) {
			warning(_(warn_time_show_forced_updates),
				forced_updates_ms / 1000.0);
		}
	}

 abort:
	strbuf_release(&note);
	ଫେରନ୍ତୁ rc;
}

/*
 * We would want to bypass the object transfer altogether if
 * everything we are going to fetch already exists and is connected
 * locally.
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା check_exist_and_connected(ସଂରଚନା ref *ref_map)
{
	ସଂରଚନା ref *rm = ref_map;
	ସଂରଚନା check_connected_options opt = CHECK_CONNECTED_INIT;
	ସଂରଚନା ref *r;

	/*
	 * If we are deepening a shallow clone we already have these
	 * objects reachable.  Running rev-list here will return with
	 * a good (0) exit status and we'll bypass the fetch that we
	 * really need to perform.  Claiming failure now will ensure
	 * we perform the network exchange to deepen our history.
	 */
	ଯଦି (deepen)
		ଫେରନ୍ତୁ -1;

	/*
	 * Similarly, if we need to refetch, we always want to perform a full
	 * fetch ignoring existing objects.
	 */
	ଯଦି (refetch)
		ଫେରନ୍ତୁ -1;


	/*
	 * check_connected() allows objects to merely be promised, but
	 * we need all direct targets to exist.
	 */
	ପାଇଁ (r = rm; r; r = r->next) {
		ଯଦି (!odb_has_object(the_repository->objects, &r->old_oid,
				    ODB_HAS_OBJECT_RECHECK_PACKED))
			ଫେରନ୍ତୁ -1;
	}

	opt.quiet = 1;
	opt.exclude_hidden_refs_section = "fetch";
	ଫେରନ୍ତୁ check_connected(iterate_ref_map, &rm, &opt);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା fetch_and_consume_refs(ସଂରଚନା display_state *display_state,
				  ସଂରଚନା transport *transport,
				  ସଂରଚନା ref_transaction *transaction,
				  ସଂରଚନା ref *ref_map,
				  ସଂରଚନା fetch_head *fetch_head,
				  ଅଚଳ ସଂରଚନା fetch_config *config,
				  ସଂରଚନା ref_update_display_info_array *display_array)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା connectivity_checked = 1;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	/*
	 * We don't need to perform a fetch in case we can already satisfy all
	 * refs.
	 */
	ret = check_exist_and_connected(ref_map);
	ଯଦି (ret) {
		trace2_region_enter("fetch", "fetch_refs", the_repository);
		ret = transport_fetch_refs(transport, ref_map);
		trace2_region_leave("fetch", "fetch_refs", the_repository);
		ଯଦି (ret)
			ଯାଅ out;
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
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା prune_refs(ସଂରଚନା display_state *display_state,
		      ସଂରଚନା refspec *rs,
		      ସଂରଚନା ref_transaction *transaction,
		      ସଂରଚନା ref *ref_map)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା result = 0;
	ସଂରଚନା ref *ref, *stale_refs = get_stale_heads(rs, ref_map);
	ସଂରଚନା strbuf err = STRBUF_INIT;
	ସଂରଚନା string_list refnames = STRING_LIST_INIT_NODUP;

	ପାଇଁ (ref = stale_refs; ref; ref = ref->next)
		string_list_append(&refnames, ref->name);

	ଯଦି (!dry_run) {
		ଯଦି (transaction) {
			ପାଇଁ (ref = stale_refs; ref; ref = ref->next) {
				result = ref_transaction_delete(transaction, ref->name, ଖାଲି,
								ଖାଲି, 0, "fetch: prune", &err);
				ଯଦି (result)
					ଯାଅ cleanup;
			}
		} ଅନ୍ୟଥା {
			result = refs_delete_refs(get_main_ref_store(the_repository),
						  "fetch: prune", &refnames,
						  0);
		}
	}

	ଯଦି (verbosity >= 0) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା summary_width = transport_summary_width(stale_refs);

		ପାଇଁ (ref = stale_refs; ref; ref = ref->next) {
			display_ref_update(display_state, '-', _("[deleted]"), ଖାଲି,
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
	ଫେରନ୍ତୁ result;
}

ସ୍ଥିର ଶୂନ୍ୟ check_not_current_branch(ସଂରଚନା ref *ref_map)
{
	ଅଚଳ ଅକ୍ଷର *path;
	ପାଇଁ (; ref_map; ref_map = ref_map->next)
		ଯଦି (ref_map->peer_ref &&
		    starts_with(ref_map->peer_ref->name, "refs/heads/") &&
		    (path = branch_checked_out(ref_map->peer_ref->name)))
			die(_("refusing to fetch into branch '%s' "
			      "checked out at '%s'"),
			    ref_map->peer_ref->name, path);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା truncate_fetch_head(ଶୂନ୍ୟ)
{
	ଅଚଳ ଅକ୍ଷର *filename = git_path_fetch_head(the_repository);
	FILE *fp = fopen_for_writing(filename);

	ଯଦି (!fp)
		ଫେରନ୍ତୁ error_errno(_("cannot open '%s'"), filename);
	fclose(fp);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ set_option(ସଂରଚନା transport *transport, ଅଚଳ ଅକ୍ଷର *name, ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା r = transport_set_option(transport, name, ମୂଲ୍ୟ);
	ଯଦି (r < 0)
		die(_("option \"%s\" value \"%s\" is not valid for %s"),
		    name, ମୂଲ୍ୟ, transport->url);
	ଯଦି (r > 0)
		warning(_("option \"%s\" is ignored for %s"),
			name, transport->url);
}


ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା add_oid(ଅଚଳ ସଂରଚନା reference *ref, ଶୂନ୍ୟ *cb_data)
{
	ସଂରଚନା oid_array *oids = cb_data;

	oid_array_append(oids, ref->oid);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ add_negotiation_tips(ସଂରଚନା string_list *ନିବେଶ_list,
				 ସଂରଚନା oid_array **output_list,
				 ଅଚଳ ଅକ୍ଷର *argname)
{
	ସଂରଚନା oid_array *oids = xcalloc(1, ଆକାର(*oids));
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

	ପାଇଁ (i = 0; i < ନିବେଶ_list->nr; i++) {
		ଅଚଳ ଅକ୍ଷର *s = ନିବେଶ_list->ବସ୍ତୁଗୁଡ଼ିକ[i].string;
		ସଂରଚନା refs_for_each_ref_options opts = {
			.pattern = s,
		};
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା old_nr;
		ଯଦି (!has_glob_specials(s)) {
			ସଂରଚନା object_id oid;

			/* Ignore missing reference. */
			ଯଦି (repo_get_oid(the_repository, s, &oid))
				ଜାରି;
			/* Fail on missing object pointed by ref. */
			ଯଦି (!odb_has_object(the_repository->objects, &oid, 0))
				die(_("the object %s does not exist"), s);

			oid_array_append(oids, &oid);
			ଜାରି;
		}
		old_nr = oids->nr;
		refs_for_each_ref_ext(get_main_ref_store(the_repository),
				      add_oid, oids, &opts);
		ଯଦି (old_nr == oids->nr)
			warning(_("ignoring %s=%s because it does not match any refs"),
				argname, s);
	}
	*output_list = oids;
}

ସ୍ଥିର ସଂରଚନା transport *prepare_transport(ସଂରଚନା remote *remote, ପୂର୍ଣ୍ଣସଂଖ୍ୟା deepen,
					   ସଂରଚନା list_objects_filter_options *filter_options)
{
	ସଂରଚନା transport *transport;

	transport = transport_get(remote, ଖାଲି);
	transport_set_verbosity(transport, verbosity, progress);
	transport->family = family;
	ଯଦି (upload_pack)
		set_option(transport, TRANS_OPT_UPLOADPACK, upload_pack);
	ଯଦି (keep)
		set_option(transport, TRANS_OPT_KEEP, "yes");
	ଯଦି (depth)
		set_option(transport, TRANS_OPT_DEPTH, depth);
	ଯଦି (deepen && deepen_since)
		set_option(transport, TRANS_OPT_DEEPEN_SINCE, deepen_since);
	ଯଦି (deepen && deepen_not.nr)
		set_option(transport, TRANS_OPT_DEEPEN_NOT,
			   (ଅଚଳ ଅକ୍ଷର *)&deepen_not);
	ଯଦି (deepen_relative)
		set_option(transport, TRANS_OPT_DEEPEN_RELATIVE, "yes");
	ଯଦି (update_shallow)
		set_option(transport, TRANS_OPT_UPDATE_SHALLOW, "yes");
	ଯଦି (refetch)
		set_option(transport, TRANS_OPT_REFETCH, "yes");
	ଯଦି (filter_options->choice) {
		ଅଚଳ ଅକ୍ଷର *spec =
			expand_list_objects_filter_spec(filter_options);
		set_option(transport, TRANS_OPT_LIST_OBJECTS_FILTER, spec);
		set_option(transport, TRANS_OPT_FROM_PROMISOR, "1");
	}
	ଯଦି (negotiation_restrict.nr) {
		ଯଦି (transport->smart_options)
			add_negotiation_tips(&negotiation_restrict,
					     &transport->smart_options->negotiation_restrict_tips,
					     "--negotiation-restrict");
		ଅନ୍ୟଥା
			warning(_("ignoring %s because the protocol does not support it"),
				"--negotiation-restrict");
	} ଅନ୍ୟଥା ଯଦି (remote->negotiation_restrict.nr) {
		ସଂରଚନା string_list_item *item;
		for_each_string_list_item(item, &remote->negotiation_restrict)
			string_list_append(&negotiation_restrict, item->string);
		ଯଦି (transport->smart_options)
			add_negotiation_tips(&negotiation_restrict,
					     &transport->smart_options->negotiation_restrict_tips,
					     "--negotiation-restrict");
		ଅନ୍ୟଥା {
			ସଂରଚନା strbuf config_name = STRBUF_INIT;
			strbuf_addf(&config_name, "remote.%s.negotiationRestrict", remote->name);
			warning(_("ignoring %s because the protocol does not support it"),
				config_name.buf);
			strbuf_release(&config_name);
		}
	}
	ଯଦି (negotiation_include.nr) {
		ଯଦି (transport->smart_options)
			add_negotiation_tips(&negotiation_include,
					     &transport->smart_options->negotiation_include_tips,
					     "--negotiation-include");
		ଅନ୍ୟଥା
			warning(_("ignoring %s because the protocol does not support it"),
				"--negotiation-include");
	} ଅନ୍ୟଥା ଯଦି (remote->negotiation_include.nr) {
		ଯଦି (transport->smart_options) {
			add_negotiation_tips(&remote->negotiation_include,
					     &transport->smart_options->negotiation_include_tips,
					     "--negotiation-include");
		} ଅନ୍ୟଥା {
			ସଂରଚନା strbuf config_name = STRBUF_INIT;
			strbuf_addf(&config_name, "remote.%s.negotiationInclude", remote->name);
			warning(_("ignoring %s because the protocol does not support it"),
				config_name.buf);
			strbuf_release(&config_name);
		}
	}
	ଫେରନ୍ତୁ transport;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା backfill_tags(ସଂରଚନା display_state *display_state,
			 ସଂରଚନା transport *transport,
			 ସଂରଚନା ref_transaction *transaction,
			 ସଂରଚନା ref *ref_map,
			 ସଂରଚନା fetch_head *fetch_head,
			 ଅଚଳ ସଂରଚନା fetch_config *config,
			 ସଂରଚନା ref_update_display_info_array *display_array,
			 ସଂରଚନା list_objects_filter_options *filter_options)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା retcode, cannot_reuse;

	/*
	 * Once we have set TRANS_OPT_DEEPEN_SINCE, we can't unset it
	 * when remote helper is used (setting it to an empty string
	 * is not unsetting). We could extend the remote helper
	 * protocol for that, but for now, just force a new connection
	 * without deepen-since. Similar story for deepen-not.
	 */
	cannot_reuse = transport->cannot_reuse ||
		deepen_since || deepen_not.nr;
	ଯଦି (cannot_reuse) {
		gsecondary = prepare_transport(transport->remote, 0, filter_options);
		transport = gsecondary;
	}

	transport_set_option(transport, TRANS_OPT_FOLLOWTAGS, ଖାଲି);
	transport_set_option(transport, TRANS_OPT_DEPTH, "0");
	transport_set_option(transport, TRANS_OPT_DEEPEN_RELATIVE, ଖାଲି);
	retcode = fetch_and_consume_refs(display_state, transport, transaction, ref_map,
					 fetch_head, config, display_array);

	ଯଦି (gsecondary) {
		transport_disconnect(gsecondary);
		gsecondary = ଖାଲି;
	}

	ଫେରନ୍ତୁ retcode;
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *strip_refshead(ଅଚଳ ଅକ୍ଷର *name){
	skip_prefix(name, "refs/heads/", &name);
	ଫେରନ୍ତୁ name;
}

ସ୍ଥିର ଶୂନ୍ୟ set_head_advice_ସନ୍ଦେଶ(ଅଚଳ ଅକ୍ଷର *remote, ଅଚଳ ଅକ୍ଷର *head_name)
{
	ଅଚଳ ଅକ୍ଷର message_advice_set_head[] =
	N_("Run 'git remote set-head %s %s' to follow the change, or modify\n"
	   "either of the 'remote.%s.followRemoteHEAD' or 'fetch.followRemoteHEAD'\n"
	   "configuration variables to handle the situation differently.\n\n"

	   "Using this specific setting\n\n"
	   "    git config set remote.%s.followRemoteHEAD warn-if-not-%s\n\n"
	   "will suppress the warning until the remote changes HEAD to something else.");

	advise_if_enabled(ADVICE_FETCH_SET_HEAD_WARN, _(message_advice_set_head),
			remote, head_name, remote, remote, head_name);
}

ସ୍ଥିର ଶୂନ୍ୟ warn_set_head(ଅଚଳ ଅକ୍ଷର *remote, ଅଚଳ ଅକ୍ଷର *head_name,
			ସଂରଚନା strbuf *buf_prev, ପୂର୍ଣ୍ଣସଂଖ୍ୟା updateres) {
	ସଂରଚନା strbuf buf_prefix = STRBUF_INIT;
	ଅଚଳ ଅକ୍ଷର *prev_head = ଖାଲି;

	strbuf_addf(&buf_prefix, "refs/remotes/%s/", remote);
	skip_prefix(buf_prev->buf, buf_prefix.buf, &prev_head);

	ଯଦି (prev_head && strcmp(prev_head, head_name)) {
		printf("'HEAD' at '%s' is '%s', but we have '%s' locally.\n",
			remote, head_name, prev_head);
		set_head_advice_ସନ୍ଦେଶ(remote, head_name);
	}
	ଅନ୍ୟଥା ଯଦି (updateres && buf_prev->len) {
		printf("'HEAD' at '%s' is '%s', "
			"but we have a detached HEAD pointing to '%s' locally.\n",
			remote, head_name, buf_prev->buf);
		set_head_advice_ସନ୍ଦେଶ(remote, head_name);
	}
	strbuf_release(&buf_prefix);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା set_head(ଅଚଳ ସଂରଚନା ref *remote_refs, ସଂରଚନା remote *remote,
			ପୂର୍ଣ୍ଣସଂଖ୍ୟା follow_remote_head)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା result = 0, create_only, baremirror, was_detached;
	ସଂରଚନା strbuf b_head = STRBUF_INIT, b_remote_head = STRBUF_INIT,
		      b_local_head = STRBUF_INIT;
	ଅଚଳ ଅକ୍ଷର *no_warn_branch = remote->no_warn_branch;
	ଅକ୍ଷର *head_name = ଖାଲି;
	ସଂରଚନା ref *ref, *matches;
	ସଂରଚନା ref *fetch_map = ଖାଲି, **fetch_map_tail = &fetch_map;
	ସଂରଚନା refspec_item refspec = {
		.force = 0,
		.pattern = 1,
		.src = (ଅକ୍ଷର *) "refs/heads/*",
		.dst = (ଅକ୍ଷର *) "refs/heads/*",
	};
	ସଂରଚନା string_list heads = STRING_LIST_INIT_DUP;
	ସଂରଚନା ref_store *refs = get_main_ref_store(the_repository);

	get_fetch_map(remote_refs, &refspec, &fetch_map_tail, 0);
	matches = guess_remote_head(find_ref_by_name(remote_refs, "HEAD"),
				    fetch_map, REMOTE_GUESS_HEAD_ALL);
	ପାଇଁ (ref = matches; ref; ref = ref->next) {
		string_list_append(&heads, strip_refshead(ref->name));
	}

	ଯଦି (!heads.nr)
		result = 1;
	ଅନ୍ୟଥା ଯଦି (heads.nr > 1)
		result = 1;
	ଅନ୍ୟଥା
		head_name = xstrdup(heads.ବସ୍ତୁଗୁଡ଼ିକ[0].string);

	ଯଦି (!head_name)
		ଯାଅ cleanup;
	baremirror = is_bare_repository(the_repository) && remote->mirror;
	create_only = follow_remote_head == FOLLOW_REMOTE_ALWAYS ? 0 : !baremirror;
	ଯଦି (baremirror) {
		strbuf_addstr(&b_head, "HEAD");
		strbuf_addf(&b_remote_head, "refs/heads/%s", head_name);
	} ଅନ୍ୟଥା {
		strbuf_addf(&b_head, "refs/remotes/%s/HEAD", remote->name);
		strbuf_addf(&b_remote_head, "refs/remotes/%s/%s", remote->name, head_name);
	}
	/* make sure it's valid */
	ଯଦି (!baremirror && !refs_ref_exists(refs, b_remote_head.buf)) {
		result = 1;
		ଯାଅ cleanup;
	}
	was_detached = refs_update_symref_extended(refs, b_head.buf, b_remote_head.buf,
					"fetch", &b_local_head, create_only);
	ଯଦି (was_detached == -1) {
		result = 1;
		ଯାଅ cleanup;
	}
	ଯଦି (verbosity >= 0 &&
		follow_remote_head == FOLLOW_REMOTE_WARN &&
		(!no_warn_branch || strcmp(no_warn_branch, head_name)))
		warn_set_head(remote->name, head_name, &b_local_head, was_detached);

cleanup:
	ମୁକ୍ତ(head_name);
	free_refs(fetch_map);
	free_refs(matches);
	string_list_clear(&heads, 0);
	strbuf_release(&b_head);
	strbuf_release(&b_local_head);
	strbuf_release(&b_remote_head);
	ଫେରନ୍ତୁ result;
}

ସଂରଚନା ref_rejection_data {
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା *retcode;
	bool conflict_ସନ୍ଦେଶ_shown;
	bool case_sensitive_ସନ୍ଦେଶ_shown;
	ଅଚଳ ଅକ୍ଷର *remote_name;
	ସଂରଚନା strmap *rejected_refs;
};

ସ୍ଥିର ଶୂନ୍ୟ ref_transaction_rejection_handler(ଅଚଳ ଅକ୍ଷର *refname,
					      ଅଚଳ ସଂରଚନା object_id *old_oid UNUSED,
					      ଅଚଳ ସଂରଚନା object_id *new_oid UNUSED,
					      ଅଚଳ ଅକ୍ଷର *old_target UNUSED,
					      ଅଚଳ ଅକ୍ଷର *new_target UNUSED,
					      ଗଣନା ref_transaction_error err,
					      ଅଚଳ ଅକ୍ଷର *details,
					      ଶୂନ୍ୟ *cb_data)
{
	ସଂରଚନା ref_rejection_data *data = cb_data;

	ଯଦି (err == REF_TRANSACTION_ERROR_CASE_CONFLICT && repo_ignore_case(the_repository) &&
	    !data->case_sensitive_ସନ୍ଦେଶ_shown) {
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
		data->case_sensitive_ସନ୍ଦେଶ_shown = true;
	} ଅନ୍ୟଥା ଯଦି (err == REF_TRANSACTION_ERROR_NAME_CONFLICT &&
		   !data->conflict_ସନ୍ଦେଶ_shown) {
		error(_("some local refs could not be updated; try running\n"
			" 'git remote prune %s' to remove any old, conflicting "
			"branches"), data->remote_name);
		data->conflict_ସନ୍ଦେଶ_shown = true;
	} ଅନ୍ୟଥା {
		ଯଦି (details)
			error("%s", details);
		ଅନ୍ୟଥା
			error(_("fetching ref %s failed: %s"),
			      refname, ref_transaction_error_ସନ୍ଦେଶ(err));
	}

	strmap_put(data->rejected_refs, refname, ଖାଲି);
	*data->retcode = 1;
}

/*
 * Commit the reference transaction. If it isn't an atomic transaction, handle
 * rejected updates as part of using batched updates.
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା commit_ref_transaction(ସଂରଚନା ref_transaction **transaction,
				  bool is_atomic, ଅଚଳ ଅକ୍ଷର *remote_name,
				  ସଂରଚନା strmap *rejected_refs,
				  ସଂରଚନା strbuf *err)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା retcode = ref_transaction_commit(*transaction, err);
	ଯଦି (retcode)
		ଯାଅ out;

	ଯଦି (!is_atomic) {
		ସଂରଚନା ref_rejection_data data = {
			.conflict_ସନ୍ଦେଶ_shown = 0,
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
	*transaction = ଖାଲି;
	ଫେରନ୍ତୁ retcode;
}

ସ୍ଥିର ଶୂନ୍ୟ collect_updated_tips(ସଂରଚନା oidset *tips, ସଂରଚନା ref *ref_map)
{
	ସଂରଚନା ref *rm;
	ପାଇଁ (rm = ref_map; rm; rm = rm->next) {
		ସଂରଚନା commit *commit;
		/*
		 * Shallow-rejected refs are not stored and their history
		 * is incomplete, so skip them.
		 */
		ଯଦି (rm->status == REF_STATUS_REJECT_SHALLOW)
			ଜାରି;
		ଯଦି (is_null_oid(&rm->old_oid))
			ଜାରି;
		ଯଦି (rm->peer_ref &&
		    oideq(&rm->old_oid, &rm->peer_ref->old_oid))
			ଜାରି;
		commit = lookup_commit_reference_gently(the_repository,
							&rm->old_oid, 1);
		ଯଦି (commit)
			oidset_ଭର୍ତ୍ତି(tips, &commit->object.oid);
	}
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା do_fetch(ସଂରଚନା transport *transport,
		    ସଂରଚନା refspec *rs,
		    ଅଚଳ ସଂରଚନା fetch_config *config,
		    ସଂରଚନା list_objects_filter_options *filter_options,
		    ସଂରଚନା oidset *updated_tips)
{
	ସଂରଚନା ref_transaction *transaction = ଖାଲି;
	ସଂରଚନା ref *ref_map = ଖାଲି;
	ସଂରଚନା display_state display_state = { 0 };
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା autotags = (transport->remote->fetch_tags == 1);
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା retcode = 0;
	ଅଚଳ ସଂରଚନା ref *remote_refs;
	ସଂରଚନା transport_ls_refs_options transport_ls_refs_options =
		TRANSPORT_LS_REFS_OPTIONS_INIT;
	ସଂରଚନା fetch_head fetch_head = { 0 };
	ସଂରଚନା strbuf err = STRBUF_INIT;
	ସଂରଚନା ref_update_display_info_array display_array = { 0 };
	ସଂରଚନା strmap rejected_refs = STRMAP_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା summary_width = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା follow_remote_head = FOLLOW_REMOTE_NEVER;

	ଯଦି (tags == TAGS_DEFAULT) {
		ଯଦି (transport->remote->fetch_tags == 2)
			tags = TAGS_SET;
		ଯଦି (transport->remote->fetch_tags == -1)
			tags = TAGS_UNSET;
	}

	/* if not appending, truncate FETCH_HEAD */
	ଯଦି (!append && write_fetch_head) {
		retcode = truncate_fetch_head();
		ଯଦି (retcode)
			ଯାଅ cleanup;
	}

	ଯଦି (rs->nr) {
		refspec_ref_prefixes(rs, &transport_ls_refs_options.ref_prefixes);
	} ଅନ୍ୟଥା {
		ସଂରଚନା branch *branch = branch_get(ଖାଲି);

		ଯଦି (transport->remote->fetch.nr) {
			refspec_ref_prefixes(&transport->remote->fetch,
					     &transport_ls_refs_options.ref_prefixes);
			/*
			 * See remote.c's handling of remote.<name>.followRemoteHEAD
			 * for the analogous, still-unresolved case.
			 */
			ଯଦି (transport->remote->follow_remote_head)
				follow_remote_head = transport->remote->follow_remote_head;
			ଅନ୍ୟଥା ଯଦି (config->follow_remote_head_seen)
				follow_remote_head = get_follow_remote_head(config->follow_remote_head_raw);
			ଅନ୍ୟଥା
				follow_remote_head = BUILTIN_FOLLOW_REMOTE_HEAD_DFLT;
		}
		ଯଦି (branch && branch_has_merge_config(branch) &&
		    !strcmp(branch->remote_name, transport->remote->name)) {
			ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
			ପାଇଁ (i = 0; i < branch->merge_nr; i++) {
				strvec_ଠେଲ(&transport_ls_refs_options.ref_prefixes,
					    branch->merge[i]->src);
			}
		}

		/*
		 * If there are no refs specified to fetch, then we just
		 * fetch HEAD; mention that to narrow the advertisement.
		 */
		ଯଦି (!transport_ls_refs_options.ref_prefixes.nr)
			strvec_ଠେଲ(&transport_ls_refs_options.ref_prefixes,
				    "HEAD");
	}

	ଯଦି (tags == TAGS_SET || tags == TAGS_DEFAULT)
		strvec_ଠେଲ(&transport_ls_refs_options.ref_prefixes,
			    "refs/tags/");

	ଯଦି (follow_remote_head != FOLLOW_REMOTE_NEVER)
		strvec_ଠେଲ(&transport_ls_refs_options.ref_prefixes,
			    "HEAD");

	/*
	 * Only initiate ref listing if we have at least one ref we want to
	 * know about.
	 */
	ଯଦି (transport_ls_refs_options.ref_prefixes.nr) {
		trace2_region_enter("fetch", "remote_refs", the_repository);
		remote_refs = transport_get_remote_refs(transport,
							&transport_ls_refs_options);
		trace2_region_leave("fetch", "remote_refs", the_repository);
	} ଅନ୍ୟଥା
		remote_refs = ଖାଲି;

	transport_ls_refs_options_release(&transport_ls_refs_options);

	ref_map = get_ref_map(transport->remote, remote_refs, rs,
			      tags, &autotags);
	ଯଦି (!update_head_ok)
		check_not_current_branch(ref_map);

	retcode = open_fetch_head(&fetch_head);
	ଯଦି (retcode)
		ଯାଅ cleanup;

	display_state_init(&display_state, ref_map, transport->url,
			   config->display_format);

	ଯଦି (atomic_fetch) {
		transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
							  0, &err);
		ଯଦି (!transaction) {
			retcode = -1;
			ଯାଅ cleanup;
		}
	}

	ଯଦି (tags == TAGS_DEFAULT && autotags)
		transport_set_option(transport, TRANS_OPT_FOLLOWTAGS, "1");
	ଯଦି (prune) {
		/*
		 * We only prune based on refspecs specified
		 * explicitly (via command line or configuration); we
		 * don't care whether --tags was specified.
		 */
		ଯଦି (rs->nr) {
			retcode = prune_refs(&display_state, rs, transaction, ref_map);
		} ଅନ୍ୟଥା {
			retcode = prune_refs(&display_state, &transport->remote->fetch,
					     transaction, ref_map);
		}
		ଯଦି (retcode != 0)
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
	ଯଦି (!transaction) {
		transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
							  REF_TRANSACTION_ALLOW_FAILURE, &err);
		ଯଦି (!transaction) {
			retcode = -1;
			ଯାଅ cleanup;
		}
	}

	ଯଦି (fetch_and_consume_refs(&display_state, transport, transaction, ref_map,
				   &fetch_head, config, &display_array)) {
		retcode = 1;
		ଯାଅ cleanup;
	}

	/*
	 * If neither --no-tags nor --tags was specified, do automated tag
	 * following.
	 */
	ଯଦି (tags == TAGS_DEFAULT && autotags) {
		ସଂରଚନା ref *tags_ref_map = ଖାଲି, **tail = &tags_ref_map;

		find_non_local_tags(remote_refs, transaction, &tags_ref_map, &tail);
		ଯଦି (tags_ref_map) {
			/*
			 * If backfilling of tags fails then we want to tell
			 * the user so, but we have to continue regardless to
			 * populate upstream information of the references we
			 * have already fetched above. The exception though is
			 * when `--atomic` is passed: in that case we'll abort
			 * the transaction and don't commit anything.
			 */
			ଯଦି (backfill_tags(&display_state, transport, transaction, tags_ref_map,
					  &fetch_head, config, &display_array, filter_options))
				retcode = 1;
		}

		free_refs(tags_ref_map);
	}

	ଯଦି (retcode)
		ଯାଅ cleanup;

	ଯଦି (verbosity >= 0)
		summary_width = transport_summary_width(ref_map);

	retcode = commit_ref_transaction(&transaction, atomic_fetch,
					 transport->remote->name,
					 &rejected_refs, &err);
	/*
	 * With '--atomic', bail out if the transaction fails. Without '--atomic',
	 * continue to fetch head and perform other post-fetch operations.
	 */
	ଯଦି (retcode && atomic_fetch)
		ଯାଅ cleanup;

	commit_fetch_head(&fetch_head);

	collect_updated_tips(updated_tips, ref_map);

	ଯଦି (set_upstream) {
		ସଂରଚନା branch *branch = branch_get("HEAD");
		ସଂରଚନା ref *rm;
		ସଂରଚନା ref *source_ref = ଖାଲି;

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
		ପାଇଁ (rm = ref_map; rm; rm = rm->next) {
			ଯଦି (!rm->peer_ref) {
				ଯଦି (source_ref) {
					warning(_("multiple branches detected, incompatible with --set-upstream"));
					ଯାଅ cleanup;
				} ଅନ୍ୟଥା {
					source_ref = rm;
				}
			}
		}
		ଯଦି (source_ref) {
			ଯଦି (!branch) {
				ଅଚଳ ଅକ୍ଷର *shortname = source_ref->name;
				skip_prefix(shortname, "refs/heads/", &shortname);

				warning(_("could not set upstream of HEAD to '%s' from '%s' when "
					  "it does not point to any branch."),
					shortname, transport->remote->name);
				ଯାଅ cleanup;
			}

			ଯଦି (!strcmp(source_ref->name, "HEAD") ||
			    starts_with(source_ref->name, "refs/heads/"))
				install_branch_config(0,
						      branch->name,
						      transport->remote->name,
						      source_ref->name);
			ଅନ୍ୟଥା ଯଦି (starts_with(source_ref->name, "refs/remotes/"))
				warning(_("not setting upstream for a remote remote-tracking branch"));
			ଅନ୍ୟଥା ଯଦି (starts_with(source_ref->name, "refs/tags/"))
				warning(_("not setting upstream for a remote tag"));
			ଅନ୍ୟଥା
				warning(_("unknown branch type"));
		} ଅନ୍ୟଥା {
			warning(_("no source branch found;\n"
				  "you need to specify exactly one branch with the --set-upstream option"));
		}
	}
	ଯଦି (follow_remote_head != FOLLOW_REMOTE_NEVER) {
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
	ଯଦି (retcode && !atomic_fetch && transaction)
		commit_ref_transaction(&transaction, false,
				       transport->remote->name,
				       &rejected_refs, &err);

	ପାଇଁ (size_t i = 0; i < display_array.nr; i++) {
		ସଂରଚନା ref_update_display_info *info = &display_array.info[i];

		ଯଦି (!info->failed && strmap_contains(&rejected_refs, info->ref))
			ref_update_display_info_set_failed(info);
		ref_update_display_info_display(info, &display_state, summary_width);
		ref_update_display_info_free(info);
	}

	ଯଦି (retcode) {
		ଯଦି (err.len) {
			error("%s", err.buf);
			strbuf_reset(&err);
		}
		ଯଦି (transaction && ref_transaction_abort(transaction, &err) &&
		    err.len)
			error("%s", err.buf);
		transaction = ଖାଲି;
	}

	ଯଦି (transaction)
		ref_transaction_free(transaction);

	ମୁକ୍ତ(display_array.info);
	strmap_clear(&rejected_refs, 0);
	display_state_release(&display_state);
	close_fetch_head(&fetch_head);
	strbuf_release(&err);
	free_refs(ref_map);
	ଫେରନ୍ତୁ retcode;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_one_remote_for_fetch(ସଂରଚନା remote *remote, ଶୂନ୍ୟ *priv)
{
	ସଂରଚନା string_list *list = priv;
	ଯଦି (!remote->skip_default_update)
		string_list_append(list, remote->name);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ add_options_to_argv(ସଂରଚନା strvec *argv,
				ଅଚଳ ସଂରଚନା fetch_config *config)
{
	ଯଦି (dry_run)
		strvec_ଠେଲ(argv, "--dry-run");
	ଯଦି (prune != -1)
		strvec_ଠେଲ(argv, prune ? "--prune" : "--no-prune");
	ଯଦି (prune_tags != -1)
		strvec_ଠେଲ(argv, prune_tags ? "--prune-tags" : "--no-prune-tags");
	ଯଦି (update_head_ok)
		strvec_ଠେଲ(argv, "--update-head-ok");
	ଯଦି (force)
		strvec_ଠେଲ(argv, "--force");
	ଯଦି (keep)
		strvec_ଠେଲ(argv, "--keep");
	ଯଦି (config->recurse_submodules == RECURSE_SUBMODULES_ON)
		strvec_ଠେଲ(argv, "--recurse-submodules");
	ଅନ୍ୟଥା ଯଦି (config->recurse_submodules == RECURSE_SUBMODULES_OFF)
		strvec_ଠେଲ(argv, "--no-recurse-submodules");
	ଅନ୍ୟଥା ଯଦି (config->recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND)
		strvec_ଠେଲ(argv, "--recurse-submodules=on-demand");
	ଯଦି (tags == TAGS_SET)
		strvec_ଠେଲ(argv, "--tags");
	ଅନ୍ୟଥା ଯଦି (tags == TAGS_UNSET)
		strvec_ଠେଲ(argv, "--no-tags");
	ଯଦି (verbosity >= 2)
		strvec_ଠେଲ(argv, "-v");
	ଯଦି (verbosity >= 1)
		strvec_ଠେଲ(argv, "-v");
	ଅନ୍ୟଥା ଯଦି (verbosity < 0)
		strvec_ଠେଲ(argv, "-q");
	ଯଦି (family == TRANSPORT_FAMILY_IPV4)
		strvec_ଠେଲ(argv, "--ipv4");
	ଅନ୍ୟଥା ଯଦି (family == TRANSPORT_FAMILY_IPV6)
		strvec_ଠେଲ(argv, "--ipv6");
	ଯଦି (!write_fetch_head)
		strvec_ଠେଲ(argv, "--no-write-fetch-head");
	ଯଦି (config->display_format == DISPLAY_FORMAT_PORCELAIN)
		strvec_pushf(argv, "--porcelain");
}

/* Fetch multiple remotes in parallel */

ସଂରଚନା parallel_fetch_state {
	ଅଚଳ ଅକ୍ଷର **argv;
	ସଂରଚନା string_list *remotes;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା next, result;
	ଅଚଳ ସଂରଚନା fetch_config *config;
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା fetch_next_remote(ସଂରଚନା child_process *cp,
			     ସଂରଚନା strbuf *out UNUSED,
			     ଶୂନ୍ୟ *cb, ଶୂନ୍ୟ **task_cb)
{
	ସଂରଚନା parallel_fetch_state *state = cb;
	ଅକ୍ଷର *remote;

	ଯଦି (state->next < 0 || state->next >= state->remotes->nr)
		ଫେରନ୍ତୁ 0;

	remote = state->remotes->ବସ୍ତୁଗୁଡ଼ିକ[state->next++].string;
	*task_cb = remote;

	strvec_pushv(&cp->args, state->argv);
	strvec_ଠେଲ(&cp->args, remote);
	cp->git_cmd = 1;

	ଯଦି (verbosity >= 0 && state->config->display_format != DISPLAY_FORMAT_PORCELAIN)
		printf(_("Fetching %s\n"), remote);

	ଫେରନ୍ତୁ 1;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା fetch_failed_to_start(ସଂରଚନା strbuf *out UNUSED,
				 ଶୂନ୍ୟ *cb, ଶୂନ୍ୟ *task_cb)
{
	ସଂରଚନା parallel_fetch_state *state = cb;
	ଅଚଳ ଅକ୍ଷର *remote = task_cb;

	state->result = error(_("could not fetch %s"), remote);

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା fetch_finished(ପୂର୍ଣ୍ଣସଂଖ୍ୟା result, ସଂରଚନା strbuf *out,
			  ଶୂନ୍ୟ *cb, ଶୂନ୍ୟ *task_cb)
{
	ସଂରଚନା parallel_fetch_state *state = cb;
	ଅଚଳ ଅକ୍ଷର *remote = task_cb;

	ଯଦି (result) {
		strbuf_addf(out, _("could not fetch '%s' (exit code: %d)\n"),
			    remote, result);
		state->result = -1;
	}

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା fetch_multiple(ସଂରଚନା string_list *list, ପୂର୍ଣ୍ଣସଂଖ୍ୟା max_children,
			  ଅଚଳ ସଂରଚନା fetch_config *config)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, result = 0;
	ସଂରଚନା strvec argv = STRVEC_INIT;

	ଯଦି (!append && write_fetch_head) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା errcode = truncate_fetch_head();
		ଯଦି (errcode)
			ଫେରନ୍ତୁ errcode;
	}

	/*
	 * Cancel out the fetch.bundleURI config when running subprocesses,
	 * to avoid fetching from the same bundle list multiple times.
	 */
	strvec_pushl(&argv, "-c", "fetch.bundleURI=",
		     "fetch", "--append", "--no-auto-gc",
		     "--no-write-commit-graph", ଖାଲି);
	ପାଇଁ (i = 0; i < server_options.nr; i++)
		strvec_pushf(&argv, "--server-option=%s", server_options.ବସ୍ତୁଗୁଡ଼ିକ[i].string);
	add_options_to_argv(&argv, config);

	ଯଦି (max_children != 1 && list->nr != 1) {
		ସଂରଚନା parallel_fetch_state state = { argv.v, list, 0, 0, config };
		ଅଚଳ ସଂରଚନା run_process_parallel_opts opts = {
			.tr2_category = "fetch",
			.tr2_label = "parallel/fetch",

			.processes = max_children,

			.get_next_task = &fetch_next_remote,
			.start_failure = &fetch_failed_to_start,
			.task_finished = &fetch_finished,
			.data = &state,
		};

		strvec_ଠେଲ(&argv, "--end-of-options");

		run_processes_parallel(&opts);
		result = state.result;
	} ଅନ୍ୟଥା
		ପାଇଁ (i = 0; i < list->nr; i++) {
			ଅଚଳ ଅକ୍ଷର *name = list->ବସ୍ତୁଗୁଡ଼ିକ[i].string;
			ସଂରଚନା child_process cmd = CHILD_PROCESS_INIT;

			strvec_pushv(&cmd.args, argv.v);
			strvec_ଠେଲ(&cmd.args, name);
			ଯଦି (verbosity >= 0 && config->display_format != DISPLAY_FORMAT_PORCELAIN)
				printf(_("Fetching %s\n"), name);
			cmd.git_cmd = 1;
			ଯଦି (run_command(&cmd)) {
				error(_("could not fetch %s"), name);
				result = 1;
			}
		}

	strvec_clear(&argv);
	ଫେରନ୍ତୁ !!result;
}

/*
 * Fetching from the promisor remote should use the given filter-spec
 * or inherit the default filter-spec from the config.
 */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ fetch_one_setup_partial(ସଂରଚନା remote *remote,
					   ସଂରଚନା list_objects_filter_options *filter_options)
{
	/*
	 * Explicit --no-filter argument overrides everything, regardless
	 * of any prior partial clones and fetches.
	 */
	ଯଦି (filter_options->no_filter)
		ଫେରନ୍ତୁ;

	/*
	 * If no prior partial clone/fetch and the current fetch DID NOT
	 * request a partial-fetch, do a normal fetch.
	 */
	ଯଦି (!repo_has_promisor_remote(the_repository) && !filter_options->choice)
		ଫେରନ୍ତୁ;

	/*
	 * If this is a partial-fetch request, we enable partial on
	 * this repo if not already enabled and remember the given
	 * filter-spec as the default for subsequent fetches to this
	 * remote if there is currently no default filter-spec.
	 */
	ଯଦି (filter_options->choice) {
		partial_clone_register(remote->name, filter_options);
		ଫେରନ୍ତୁ;
	}

	/*
	 * Do a partial-fetch from the promisor remote using either the
	 * explicitly given filter-spec or inherit the filter-spec from
	 * the config.
	 */
	ଯଦି (!filter_options->choice)
		partial_clone_get_default_filter_spec(filter_options, remote->name);
	ଫେରନ୍ତୁ;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା fetch_one(ସଂରଚନା remote *remote, ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv,
		     ପୂର୍ଣ୍ଣସଂଖ୍ୟା prune_tags_ok, ପୂର୍ଣ୍ଣସଂଖ୍ୟା use_stdin_refspecs,
		     ଅଚଳ ସଂରଚନା fetch_config *config,
		     ସଂରଚନା list_objects_filter_options *filter_options,
		     ସଂରଚନା oidset *updated_tips)
{
	ସଂରଚନା refspec rs = REFSPEC_INIT_FETCH(the_hash_algo);
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା exit_code;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା maybe_prune_tags;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା remote_via_config = remote_is_configured(remote, 0);

	ଯଦି (!remote)
		die(_("no remote repository specified; please specify either a URL or a\n"
		      "remote name from which new revisions should be fetched"));

	gtransport = prepare_transport(remote, 1, filter_options);

	ଯଦି (prune < 0) {
		/* no command line request */
		ଯଦି (0 <= remote->prune)
			prune = remote->prune;
		ଅନ୍ୟଥା ଯଦି (0 <= config->prune)
			prune = config->prune;
		ଅନ୍ୟଥା
			prune = PRUNE_BY_DEFAULT;
	}

	ଯଦି (prune_tags < 0) {
		/* no command line request */
		ଯଦି (0 <= remote->prune_tags)
			prune_tags = remote->prune_tags;
		ଅନ୍ୟଥା ଯଦି (0 <= config->prune_tags)
			prune_tags = config->prune_tags;
		ଅନ୍ୟଥା
			prune_tags = PRUNE_TAGS_BY_DEFAULT;
	}

	maybe_prune_tags = prune_tags_ok && prune_tags;
	ଯଦି (maybe_prune_tags && remote_via_config)
		refspec_append(&remote->fetch, TAG_REFSPEC);

	ଯଦି (maybe_prune_tags && (argc || !remote_via_config))
		refspec_append(&rs, TAG_REFSPEC);

	ପାଇଁ (i = 0; i < argc; i++) {
		ଯଦି (!strcmp(argv[i], "tag")) {
			i++;
			ଯଦି (i >= argc)
				die(_("you need to specify a tag name"));

			refspec_appendf(&rs, "refs/tags/%s:refs/tags/%s",
					argv[i], argv[i]);
		} ଅନ୍ୟଥା {
			refspec_append(&rs, argv[i]);
		}
	}

	ଯଦି (use_stdin_refspecs) {
		ସଂରଚନା strbuf ଧାଡ଼ି = STRBUF_INIT;
		ଯେତେବେଳେ (strbuf_getline_lf(&ଧାଡ଼ି, stdin) != EOF)
			refspec_append(&rs, ଧାଡ଼ି.buf);
		strbuf_release(&ଧାଡ଼ି);
	}

	ଯଦି (server_options.nr)
		gtransport->server_options = &server_options;

	sigchain_ଠେଲ_common(unlock_pack_on_signal);
	atexit(unlock_pack_atexit);
	sigchain_ଠେଲ(SIGPIPE, SIG_IGN);
	exit_code = do_fetch(gtransport, &rs, config, filter_options,
			     updated_tips);
	sigchain_କାଢ଼(SIGPIPE);
	refspec_clear(&rs);
	transport_disconnect(gtransport);
	gtransport = ଖାଲି;
	ଫେରନ୍ତୁ exit_code;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_fetch(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
	      ଅଚଳ ଅକ୍ଷର **argv,
	      ଅଚଳ ଅକ୍ଷର *prefix,
	      ସଂରଚନା repository *repo UNUSED)
{
	ସଂରଚନା fetch_config config = {
		.display_format = DISPLAY_FORMAT_FULL,
		.follow_remote_head_raw = ଖାଲି,
		.follow_remote_head_seen = 0,
		.prune = -1,
		.prune_tags = -1,
		.show_forced_updates = 1,
		.recurse_submodules = RECURSE_SUBMODULES_DEFAULT,
		.parallel = 1,
		.submodule_fetch_jobs = -1,
	};
	ଅଚଳ ଅକ୍ଷର *submodule_prefix = "";
	ଅଚଳ ଅକ୍ଷର *bundle_uri;
	ସଂରଚନା string_list list = STRING_LIST_INIT_DUP;
	ସଂରଚନା list_objects_filter_options filter_options = LIST_OBJECTS_FILTER_INIT;
	ସଂରଚନା remote *remote = ଖାଲି;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା all = -1, multiple = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା result = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା prune_tags_ok = 1;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା enable_auto_gc = 1;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା unshallow = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା max_jobs = -1;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା recurse_submodules_cli = RECURSE_SUBMODULES_DEFAULT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା recurse_submodules_default = RECURSE_SUBMODULES_ON_DEMAND;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା fetch_write_commit_graph = -1;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା stdin_refspecs = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା negotiate_only = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା porcelain = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
	ଗଣନା {
		GRAPH_WRITE_REACHABLE,
		GRAPH_WRITE_TIPS,
		GRAPH_WRITE_SKIP,
	} graph_write_mode = GRAPH_WRITE_REACHABLE;
	ସଂରଚନା oidset updated_tips = OIDSET_INIT;

	ସଂରଚନା option builtin_fetch_options[] = {
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
		OPT_SET_INT('n', ଖାଲି, &tags,
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
			.ମୂଲ୍ୟ = &submodule_prefix,
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
	ପାଇଁ (i = 1; i < argc; i++) {
		/* This handles non-URLs gracefully */
		ଅକ୍ଷର *anon = transport_anonymize_url(argv[i]);

		strbuf_addf(&default_rla, " %s", anon);
		ମୁକ୍ତ(anon);
	}

	repo_config(the_repository, git_fetch_config, &config);
	ଯଦି (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_full_index = 0;
	}

	argc = parse_options(argc, argv, prefix,
			     builtin_fetch_options, builtin_fetch_usage, 0);

	ଯଦି (recurse_submodules_cli != RECURSE_SUBMODULES_DEFAULT)
		config.recurse_submodules = recurse_submodules_cli;

	ଯଦି (negotiate_only) {
		ବାଛ (recurse_submodules_cli) {
		ସ୍ଥିତି RECURSE_SUBMODULES_OFF:
		ସ୍ଥିତି RECURSE_SUBMODULES_DEFAULT:
			/*
			 * --negotiate-only should never recurse into
			 * submodules. Skip it by setting recurse_submodules to
			 * RECURSE_SUBMODULES_OFF.
			 */
			config.recurse_submodules = RECURSE_SUBMODULES_OFF;
			ଭାଙ୍ଗ;

		ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
			die(_("options '%s' and '%s' cannot be used together"),
			    "--negotiate-only", "--recurse-submodules");
		}
	}

	ଯଦି (config.recurse_submodules != RECURSE_SUBMODULES_OFF) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା *sfjc = config.submodule_fetch_jobs == -1
			    ? &config.submodule_fetch_jobs : ଖାଲି;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା *rs = config.recurse_submodules == RECURSE_SUBMODULES_DEFAULT
			  ? &config.recurse_submodules : ଖାଲି;

		fetch_config_from_gitmodules(the_repository, sfjc, rs);
	}


	ଯଦି (porcelain) {
		ବାଛ (recurse_submodules_cli) {
		ସ୍ଥିତି RECURSE_SUBMODULES_OFF:
		ସ୍ଥିତି RECURSE_SUBMODULES_DEFAULT:
			/*
			 * Reference updates in submodules would be ambiguous
			 * in porcelain mode, so we reject this combination.
			 */
			config.recurse_submodules = RECURSE_SUBMODULES_OFF;
			ଭାଙ୍ଗ;

		ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
			die(_("options '%s' and '%s' cannot be used together"),
			    "--porcelain", "--recurse-submodules");
		}

		config.display_format = DISPLAY_FORMAT_PORCELAIN;
	}

	ଯଦି (deepen_relative) {
		ଯଦି (deepen_relative < 0)
			die(_("negative depth in --deepen is not supported"));
		ଯଦି (depth)
			die(_("options '%s' and '%s' cannot be used together"), "--deepen", "--depth");
		depth = xstrfmt("%d", deepen_relative);
	}
	ଯଦି (unshallow) {
		ଯଦି (depth)
			die(_("options '%s' and '%s' cannot be used together"), "--depth", "--unshallow");
		ଅନ୍ୟଥା ଯଦି (!is_repository_shallow(the_repository))
			die(_("--unshallow on a complete repository does not make sense"));
		ଅନ୍ୟଥା
			depth = xstrfmt("%d", INFINITE_DEPTH);
	}

	/* no need to be strict, transport_set_option() will validate it again */
	ଯଦି (depth && atoi(depth) < 1)
		die(_("depth %s is not a positive number"), depth);
	ଯଦି (depth || deepen_since || deepen_not.nr)
		deepen = 1;

	/* FETCH_HEAD never gets updated in --dry-run mode */
	ଯଦି (dry_run)
		write_fetch_head = 0;

	ଯଦି (!max_jobs)
		max_jobs = online_cpus();

	ଯଦି (!repo_config_get_string_tmp(the_repository, "fetch.bundleuri", &bundle_uri) &&
	    fetch_bundle_uri(the_repository, bundle_uri, ଖାଲି))
		warning(_("failed to fetch bundles from '%s'"), bundle_uri);

	ଯଦି (all < 0) {
		/*
		 * no --[no-]all given;
		 * only use config option if no remote was explicitly specified
		 */
		all = (!argc) ? config.all : 0;
	}

	ଯଦି (all) {
		ଯଦି (argc == 1)
			die(_("fetch --all does not take a repository argument"));
		ଅନ୍ୟଥା ଯଦି (argc > 1)
			die(_("fetch --all does not make sense with refspecs"));

		(ଶୂନ୍ୟ) for_each_remote(get_one_remote_for_fetch, &list);

		/* do not do fetch_multiple() of one */
		ଯଦି (list.nr == 1)
			remote = remote_get(list.ବସ୍ତୁଗୁଡ଼ିକ[0].string);
	} ଅନ୍ୟଥା ଯଦି (argc == 0) {
		/* No arguments -- use default remote */
		remote = remote_get(ଖାଲି);
	} ଅନ୍ୟଥା ଯଦି (multiple) {
		/* All arguments are assumed to be remotes or groups */
		ପାଇଁ (i = 0; i < argc; i++)
			ଯଦି (!add_remote_or_group(argv[i], &list))
				die(_("no such remote or remote group: %s"),
				    argv[i]);
	} ଅନ୍ୟଥା {
		/* Single remote or group */
		(ଶୂନ୍ୟ) add_remote_or_group(argv[0], &list);
		ଯଦି (list.nr > 1) {
			/* More than one remote */
			ଯଦି (argc > 1)
				die(_("fetching a group and specifying refspecs does not make sense"));
		} ଅନ୍ୟଥା {
			/* Zero or one remotes */
			remote = remote_get(argv[0]);
			prune_tags_ok = (argc == 1);
			argc--;
			argv++;
		}
	}
	string_list_remove_duplicates(&list, 0);

	ଯଦି (negotiate_only) {
		ସଂରଚନା oidset acked_commits = OIDSET_INIT;
		ସଂରଚନା oidset_iter iter;
		ଅଚଳ ସଂରଚନା object_id *oid;

		trace2_region_enter("fetch", "negotiate-only", the_repository);
		ଯଦି (!remote)
			die(_("must supply remote when using --negotiate-only"));
		gtransport = prepare_transport(remote, 1, &filter_options);

		ଯଦି (!gtransport->smart_options) {
			warning(_("protocol does not support --negotiate-only, exiting"));
			result = 1;
			trace2_region_leave("fetch", "negotiate-only", the_repository);
			ଯାଅ cleanup;
		}
		ଯଦି (!gtransport->smart_options->negotiation_restrict_tips)
			die(_("%s needs one or more %s"), "--negotiate-only",
			    "--negotiation-restrict=*");

		gtransport->smart_options->acked_commits = &acked_commits;

		ଯଦି (server_options.nr)
			gtransport->server_options = &server_options;
		result = transport_fetch_refs(gtransport, ଖାଲି);
		gtransport->smart_options->acked_commits = ଖାଲି;

		oidset_iter_init(&acked_commits, &iter);
		ଯେତେବେଳେ ((oid = oidset_iter_next(&iter)))
			printf("%s\n", oid_to_hex(oid));
		oidset_clear(&acked_commits);
		trace2_region_leave("fetch", "negotiate-only", the_repository);
	} ଅନ୍ୟଥା ଯଦି (remote) {
		ଯଦି (filter_options.choice || repo_has_promisor_remote(the_repository)) {
			trace2_region_enter("fetch", "setup-partial", the_repository);
			fetch_one_setup_partial(remote, &filter_options);
			trace2_region_leave("fetch", "setup-partial", the_repository);
		}
		trace2_region_enter("fetch", "fetch-one", the_repository);
		result = fetch_one(remote, argc, argv, prune_tags_ok, stdin_refspecs,
				   &config, &filter_options, &updated_tips);
		ଯଦି (prepare_commit_graph(the_repository)) {
			ଯଦି (oidset_size(&updated_tips))
				graph_write_mode = GRAPH_WRITE_TIPS;
			ଅନ୍ୟଥା
				graph_write_mode = GRAPH_WRITE_SKIP;
		}
		trace2_region_leave("fetch", "fetch-one", the_repository);
	} ଅନ୍ୟଥା {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା max_children = max_jobs;

		ଯଦି (filter_options.choice)
			die(_("--filter can only be used with the remote "
			      "configured in extensions.partialclone"));

		ଯଦି (atomic_fetch)
			die(_("--atomic can only be used when fetching "
			      "from one remote"));

		ଯଦି (stdin_refspecs)
			die(_("--stdin can only be used when fetching "
			      "from one remote"));

		ଯଦି (max_children < 0)
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
	ଯଦି (!result && remote && (config.recurse_submodules != RECURSE_SUBMODULES_OFF)) {
		ସଂରଚନା strvec options = STRVEC_INIT;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା max_children = max_jobs;

		ଯଦି (max_children < 0)
			max_children = config.submodule_fetch_jobs;
		ଯଦି (max_children < 0)
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
	ଯଦି (negotiate_only)
		ଯାଅ cleanup;

	prepare_repo_settings(the_repository);
	ଯଦି (fetch_write_commit_graph > 0 ||
	    (fetch_write_commit_graph < 0 &&
	     the_repository->settings.fetch_write_commit_graph)) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା commit_graph_flags = COMMIT_GRAPH_WRITE_SPLIT;

		ଯଦି (progress)
			commit_graph_flags |= COMMIT_GRAPH_WRITE_PROGRESS;

		ଯଦି (graph_write_mode != GRAPH_WRITE_SKIP) {
			trace2_region_enter("fetch", "write-commit-graph",
					    the_repository);
			ଯଦି (graph_write_mode == GRAPH_WRITE_TIPS)
				write_commit_graph(
					the_repository,
					the_repository->objects->source->path,
					ଖାଲି, &updated_tips,
					commit_graph_flags, ଖାଲି);
			ଅନ୍ୟଥା
				write_commit_graph_reachable(
					the_repository,
					the_repository->objects->source->path,
					commit_graph_flags, ଖାଲି);
			trace2_region_leave("fetch", "write-commit-graph",
					    the_repository);
		}
	}

	ଯଦି (enable_auto_gc) {
		ଯଦି (refetch) {
			/*
			 * Hint auto-maintenance strongly to encourage repacking,
			 * but respect config settings disabling it.
			 */
			ପୂର୍ଣ୍ଣସଂଖ୍ୟା opt_val;
			ଯଦି (repo_config_get_int(the_repository, "gc.autopacklimit", &opt_val))
				opt_val = -1;
			ଯଦି (opt_val != 0)
				git_config_ଠେଲ_parameter("gc.autoPackLimit=1");

			ଯଦି (repo_config_get_int(the_repository, "maintenance.incremental-repack.auto", &opt_val))
				opt_val = -1;
			ଯଦି (opt_val != 0)
				git_config_ଠେଲ_parameter("maintenance.incremental-repack.auto=-1");
		}
		run_auto_maintenance(the_repository, verbosity < 0);
	}

 cleanup:
	oidset_clear(&updated_tips);
	string_list_clear(&list, 0);
	list_objects_filter_release(&filter_options);
	ମୁକ୍ତ(config.follow_remote_head_raw);
	ଫେରନ୍ତୁ result;
}
