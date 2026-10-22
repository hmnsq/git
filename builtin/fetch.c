/*
 * "git fetch"
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "advice.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "hex.h"
#ಅನ್ತರ್ಭಾವ "refs.h"
#ಅನ್ತರ್ಭಾವ "refspec.h"
#ಅನ್ತರ್ಭಾವ "object-name.h"
#ಅನ್ತರ್ಭಾವ "odb.h"
#ಅನ್ತರ್ಭಾವ "odb/source.h"
#ಅನ್ತರ್ಭಾವ "oidset.h"
#ಅನ್ತರ್ಭಾವ "oid-array.h"
#ಅನ್ತರ್ಭಾವ "commit.h"
#ಅನ್ತರ್ಭಾವ "string-list.h"
#ಅನ್ತರ್ಭಾವ "remote.h"
#ಅನ್ತರ್ಭಾವ "transport.h"
#ಅನ್ತರ್ಭಾವ "run-command.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "sigchain.h"
#ಅನ್ತರ್ಭಾವ "submodule-config.h"
#ಅನ್ತರ್ಭಾವ "submodule.h"
#ಅನ್ತರ್ಭಾವ "connected.h"
#ಅನ್ತರ್ಭಾವ "strvec.h"
#ಅನ್ತರ್ಭಾವ "utf8.h"
#ಅನ್ತರ್ಭಾವ "pager.h"
#ಅನ್ತರ್ಭಾವ "path.h"
#ಅನ್ತರ್ಭಾವ "pkt-line.h"
#ಅನ್ತರ್ಭಾವ "list-objects-filter-options.h"
#ಅನ್ತರ್ಭಾವ "commit-reach.h"
#ಅನ್ತರ್ಭಾವ "branch.h"
#ಅನ್ತರ್ಭಾವ "promisor-remote.h"
#ಅನ್ತರ್ಭಾವ "commit-graph.h"
#ಅನ್ತರ್ಭಾವ "shallow.h"
#ಅನ್ತರ್ಭಾವ "trace.h"
#ಅನ್ತರ್ಭಾವ "trace2.h"
#ಅನ್ತರ್ಭಾವ "bundle-uri.h"

#define FORCED_UPDATES_DELAY_WARNING_IN_MS (10 * 1000)

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ builtin_fetch_usage[] = {
	N_("git fetch [<options>] [<repository> [<refspec>...]]"),
	N_("git fetch [<options>] <group>"),
	N_("git fetch --multiple [<options>] [(<repository>|<group>)...]"),
	N_("git fetch --all [<options>]"),
	ರಿಕ್ತ
};

ಗಣನಾ {
	TAGS_UNSET = 0,
	TAGS_DEFAULT = 1,
	TAGS_SET = 2
};

ಗಣನಾ display_format {
	DISPLAY_FORMAT_FULL,
	DISPLAY_FORMAT_COMPACT,
	DISPLAY_FORMAT_PORCELAIN,
};

ಸಂರಚನಾ display_state {
	ಸಂರಚನಾ strbuf buf;

	ಪೂರ್ಣಾಙ್ಕ refcol_width;
	ಗಣನಾ display_format format;

	ವರ್ಣ *url;
	ಪೂರ್ಣಾಙ್ಕ url_len, shown_url;
};

ಸ್ಥಿರ uint64_t forced_updates_ms = 0;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ prefetch = 0;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ prune = -1; /* unspecified */
#define PRUNE_BY_DEFAULT 0 /* do we prune by default? */

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ prune_tags = -1; /* unspecified */
#define PRUNE_TAGS_BY_DEFAULT 0 /* do we prune tags by default? */

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ append, dry_run, force, keep, update_head_ok;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ write_fetch_head = 1;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ verbosity, deepen_relative, set_upstream, refetch;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ progress = -1;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ tags = TAGS_DEFAULT, update_shallow, deepen;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ atomic_fetch;
ಸ್ಥಿರ ಗಣನಾ transport_family family;
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *depth;
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *deepen_since;
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *upload_pack;
ಸ್ಥಿರ ಸಂರಚನಾ string_list deepen_not = STRING_LIST_INIT_NODUP;
ಸ್ಥಿರ ಸಂರಚನಾ strbuf default_rla = STRBUF_INIT;
ಸ್ಥಿರ ಸಂರಚನಾ transport *gtransport;
ಸ್ಥಿರ ಸಂರಚನಾ transport *gsecondary;
ಸ್ಥಿರ ಸಂರಚನಾ refspec refmap;
ಸ್ಥಿರ ಸಂರಚನಾ string_list server_options = STRING_LIST_INIT_DUP;
ಸ್ಥಿರ ಸಂರಚನಾ string_list negotiation_restrict = STRING_LIST_INIT_NODUP;
ಸ್ಥಿರ ಸಂರಚನಾ string_list negotiation_include = STRING_LIST_INIT_NODUP;

ಸಂರಚನಾ fetch_config {
	ಗಣನಾ display_format display_format;
	ವರ್ಣ *follow_remote_head_raw;
	ಪೂರ್ಣಾಙ್ಕ follow_remote_head_seen;
	ಪೂರ್ಣಾಙ್ಕ all;
	ಪೂರ್ಣಾಙ್ಕ prune;
	ಪೂರ್ಣಾಙ್ಕ prune_tags;
	ಪೂರ್ಣಾಙ್ಕ show_forced_updates;
	ಪೂರ್ಣಾಙ್ಕ recurse_submodules;
	ಪೂರ್ಣಾಙ್ಕ parallel;
	ಪೂರ್ಣಾಙ್ಕ submodule_fetch_jobs;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ git_fetch_config(ಧ್ರುವ ವರ್ಣ *k, ಧ್ರುವ ವರ್ಣ *v,
			    ಧ್ರುವ ಸಂರಚನಾ config_context *ctx, ಶೂನ್ಯ *cb)
{
	ಸಂರಚನಾ fetch_config *fetch_config = cb;

	ಯದಿ (!strcmp(k, "fetch.all")) {
		fetch_config->all = git_config_bool(k, v);
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (!strcmp(k, "fetch.prune")) {
		fetch_config->prune = git_config_bool(k, v);
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (!strcmp(k, "fetch.prunetags")) {
		fetch_config->prune_tags = git_config_bool(k, v);
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (!strcmp(k, "fetch.showforcedupdates")) {
		fetch_config->show_forced_updates = git_config_bool(k, v);
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (!strcmp(k, "submodule.recurse")) {
		ಪೂರ್ಣಾಙ್ಕ r = git_config_bool(k, v) ?
			RECURSE_SUBMODULES_ON : RECURSE_SUBMODULES_OFF;
		fetch_config->recurse_submodules = r;
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (!strcmp(k, "submodule.fetchjobs")) {
		fetch_config->submodule_fetch_jobs = parse_submodule_fetchjobs(k, v, ctx->kvi);
		ಪ್ರತ್ಯಾಗಮ 0;
	} ಅನ್ಯಥಾ ಯದಿ (!strcmp(k, "fetch.recursesubmodules")) {
		fetch_config->recurse_submodules = parse_fetch_recurse_submodules_arg(k, v);
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (!strcmp(k, "fetch.parallel")) {
		fetch_config->parallel = git_config_int(k, v, ctx->kvi);
		ಯದಿ (fetch_config->parallel < 0)
			die(_("fetch.parallel cannot be negative"));
		ಯದಿ (!fetch_config->parallel)
			fetch_config->parallel = online_cpus();
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (!strcmp(k, "fetch.output")) {
		ಯದಿ (!v)
			ಪ್ರತ್ಯಾಗಮ config_error_nonbool(k);
		ಅನ್ಯಥಾ ಯದಿ (!strcasecmp(v, "full"))
			fetch_config->display_format = DISPLAY_FORMAT_FULL;
		ಅನ್ಯಥಾ ಯದಿ (!strcasecmp(v, "compact"))
			fetch_config->display_format = DISPLAY_FORMAT_COMPACT;
		ಅನ್ಯಥಾ
			die(_("invalid value for '%s': '%s'"),
			    "fetch.output", v);
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (!strcmp(k, "fetch.followremotehead")) {
		ಮುಕ್ತ(fetch_config->follow_remote_head_raw);
		fetch_config->follow_remote_head_raw = xstrdup_or_null(v);
		fetch_config->follow_remote_head_seen = 1;
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಪ್ರತ್ಯಾಗಮ git_default_config(k, v, ctx, cb);
}

ಸ್ಥಿರ ಗಣನಾ follow_remote_head_settings get_follow_remote_head(ಧ್ರುವ ವರ್ಣ *setting)
{
	ಯದಿ (!setting)
		die(_("missing value for 'fetch.followRemoteHEAD'"));
	ಅನ್ಯಥಾ ಯದಿ (!strcmp(setting, "never"))
		ಪ್ರತ್ಯಾಗಮ FOLLOW_REMOTE_NEVER;
	ಅನ್ಯಥಾ ಯದಿ (!strcmp(setting, "create"))
		ಪ್ರತ್ಯಾಗಮ FOLLOW_REMOTE_CREATE;
	ಅನ್ಯಥಾ ಯದಿ (!strcmp(setting, "warn"))
		ಪ್ರತ್ಯಾಗಮ FOLLOW_REMOTE_WARN;
	ಅನ್ಯಥಾ ಯದಿ (!strcmp(setting, "always"))
		ಪ್ರತ್ಯಾಗಮ FOLLOW_REMOTE_ALWAYS;
	warning(_("unrecognized fetch.followRemoteHEAD value '%s' ignored"), setting);
	ಪ್ರತ್ಯಾಗಮ BUILTIN_FOLLOW_REMOTE_HEAD_DFLT;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ parse_refmap_arg(ಧ್ರುವ ಸಂರಚನಾ option *opt, ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	BUG_ON_OPT_NEG(unset);

	/*
	 * "git fetch --refmap='' origin foo"
	 * can be used to tell the command not to store anywhere
	 */
	refspec_append(opt->ಮೌಲ್ಯ, arg);

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ unlock_pack(unsigned ಪೂರ್ಣಾಙ್ಕ flags)
{
	ಯದಿ (gtransport)
		transport_unlock_pack(gtransport, flags);
	ಯದಿ (gsecondary)
		transport_unlock_pack(gsecondary, flags);
}

ಸ್ಥಿರ ಶೂನ್ಯ unlock_pack_atexit(ಶೂನ್ಯ)
{
	unlock_pack(0);
}

ಸ್ಥಿರ ಶೂನ್ಯ unlock_pack_on_signal(ಪೂರ್ಣಾಙ್ಕ signo)
{
	unlock_pack(TRANSPORT_UNLOCK_PACK_IN_SIGNAL_HANDLER);
	sigchain_ತೆಗೆ(signo);
	ಉತ್ಥಾಪಯ(signo);
}

ಸ್ಥಿರ ಶೂನ್ಯ add_merge_config(ಸಂರಚನಾ ref **head,
			   ಧ್ರುವ ಸಂರಚನಾ ref *remote_refs,
		           ಸಂರಚನಾ branch *branch,
		           ಸಂರಚನಾ ref ***tail)
{
	ಪೂರ್ಣಾಙ್ಕ i;

	ಪ್ರತಿ (i = 0; i < branch->merge_nr; i++) {
		ಸಂರಚನಾ ref *rm, **old_tail = *tail;
		ಸಂರಚನಾ refspec_item refspec;

		ಪ್ರತಿ (rm = *head; rm; rm = rm->next) {
			ಯದಿ (branch_merge_matches(branch, i, rm->name)) {
				rm->fetch_head_status = FETCH_HEAD_MERGE;
				ಭಙ್ಗ;
			}
		}
		ಯದಿ (rm)
			ಅನುವರ್ತ;

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
		memset(&refspec, 0, ಪರಿಮಾಣ(refspec));
		refspec.src = branch->merge[i]->src;
		get_fetch_map(remote_refs, &refspec, tail, 1);
		ಪ್ರತಿ (rm = *old_tail; rm; rm = rm->next)
			rm->fetch_head_status = FETCH_HEAD_MERGE;
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ create_fetch_oidset(ಸಂರಚನಾ ref **head, ಸಂರಚನಾ oidset *out)
{
	ಸಂರಚನಾ ref *rm = *head;
	ಯಾವತ್ (rm) {
		oidset_ಸೇರಿಸು(out, &rm->old_oid);
		rm = rm->next;
	}
}

ಸಂರಚನಾ refname_hash_entry {
	ಸಂರಚನಾ hashmap_entry ent;
	ಸಂರಚನಾ object_id oid;
	ಪೂರ್ಣಾಙ್ಕ ignore;
	ವರ್ಣ refname[FLEX_ARRAY];
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ refname_hash_entry_cmp(ಧ್ರುವ ಶೂನ್ಯ *hashmap_cmp_fn_data UNUSED,
				  ಧ್ರುವ ಸಂರಚನಾ hashmap_entry *eptr,
				  ಧ್ರುವ ಸಂರಚನಾ hashmap_entry *entry_or_ಕೀಲಿ,
				  ಧ್ರುವ ಶೂನ್ಯ *keydata)
{
	ಧ್ರುವ ಸಂರಚನಾ refname_hash_entry *e1, *e2;

	e1 = container_of(eptr, ಧ್ರುವ ಸಂರಚನಾ refname_hash_entry, ent);
	e2 = container_of(entry_or_ಕೀಲಿ, ಧ್ರುವ ಸಂರಚನಾ refname_hash_entry, ent);
	ಪ್ರತ್ಯಾಗಮ strcmp(e1->refname, keydata ? keydata : e2->refname);
}

ಸ್ಥಿರ ಸಂರಚನಾ refname_hash_entry *refname_hash_add(ಸಂರಚನಾ hashmap *map,
						   ಧ್ರುವ ವರ್ಣ *refname,
						   ಧ್ರುವ ಸಂರಚನಾ object_id *oid)
{
	ಸಂರಚನಾ refname_hash_entry *ent;
	size_t len = strlen(refname);

	FLEX_ALLOC_MEM(ent, refname, refname, len);
	hashmap_entry_init(&ent->ent, strhash(refname));
	oidcpy(&ent->oid, oid);
	hashmap_add(map, &ent->ent);
	ಪ್ರತ್ಯಾಗಮ ent;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ add_one_refname(ಧ್ರುವ ಸಂರಚನಾ reference *ref, ಶೂನ್ಯ *cbdata)
{
	ಸಂರಚನಾ hashmap *refname_map = cbdata;

	(ಶೂನ್ಯ) refname_hash_add(refname_map, ref->name, ref->oid);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ refname_hash_init(ಸಂರಚನಾ hashmap *map)
{
	hashmap_init(map, refname_hash_entry_cmp, ರಿಕ್ತ, 0);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ refname_hash_exists(ಸಂರಚನಾ hashmap *map, ಧ್ರುವ ವರ್ಣ *refname)
{
	ಪ್ರತ್ಯಾಗಮ !!hashmap_get_from_hash(map, strhash(refname), refname);
}

ಸ್ಥಿರ ಶೂನ್ಯ clear_item(ಸಂರಚನಾ refname_hash_entry *item)
{
	item->ignore = 1;
}


ಸ್ಥಿರ ಶೂನ್ಯ add_already_queued_tags(ಧ್ರುವ ವರ್ಣ *refname,
				    ಧ್ರುವ ಸಂರಚನಾ object_id *old_oid UNUSED,
				    ಧ್ರುವ ಸಂರಚನಾ object_id *new_oid,
				    ಶೂನ್ಯ *cb_data)
{
	ಸಂರಚನಾ hashmap *queued_tags = cb_data;
	ಯದಿ (starts_with(refname, "refs/tags/") && new_oid)
		(ಶೂನ್ಯ) refname_hash_add(queued_tags, refname, new_oid);
}

ಸ್ಥಿರ ಶೂನ್ಯ find_non_local_tags(ಧ್ರುವ ಸಂರಚನಾ ref *refs,
				ಸಂರಚನಾ ref_transaction *transaction,
				ಸಂರಚನಾ ref **head,
				ಸಂರಚನಾ ref ***tail)
{
	ಸಂರಚನಾ hashmap existing_refs;
	ಸಂರಚನಾ hashmap remote_refs;
	ಸಂರಚನಾ oidset fetch_oids = OIDSET_INIT;
	ಸಂರಚನಾ string_list remote_refs_list = STRING_LIST_INIT_NODUP;
	ಸಂರಚನಾ string_list_item *remote_ref_item;
	ಧ್ರುವ ಸಂರಚನಾ ref *ref;
	ಸಂರಚನಾ refname_hash_entry *item = ರಿಕ್ತ;

	refname_hash_init(&existing_refs);
	refname_hash_init(&remote_refs);
	create_fetch_oidset(head, &fetch_oids);

	refs_for_each_ref(get_main_ref_store(the_repository), add_one_refname,
			  &existing_refs);

	/*
	 * If we already have a transaction, then we need to filter out all
	 * tags which have already been queued up.
	 */
	ಯದಿ (transaction)
		ref_transaction_for_each_queued_update(transaction,
						       add_already_queued_tags,
						       &existing_refs);

	ಪ್ರತಿ (ref = refs; ref; ref = ref->next) {
		ಯದಿ (!starts_with(ref->name, "refs/tags/"))
			ಅನುವರ್ತ;

		/*
		 * The peeled ref always follows the matching base
		 * ref, so if we see a peeled ref that we don't want
		 * to fetch then we can mark the ref entry in the list
		 * as one to ignore by setting util to NULL.
		 */
		ಯದಿ (ends_with(ref->name, "^{}")) {
			ಯದಿ (item &&
			    !odb_has_object(the_repository->objects, &ref->old_oid, 0) &&
			    !oidset_contains(&fetch_oids, &ref->old_oid) &&
			    !odb_has_object(the_repository->objects, &item->oid, 0) &&
			    !oidset_contains(&fetch_oids, &item->oid))
				clear_item(item);
			item = ರಿಕ್ತ;
			ಅನುವರ್ತ;
		}

		/*
		 * If item is non-NULL here, then we previously saw a
		 * ref not followed by a peeled reference, so we need
		 * to check if it is a lightweight tag that we want to
		 * fetch.
		 */
		ಯದಿ (item &&
		    !odb_has_object(the_repository->objects, &item->oid, 0) &&
		    !oidset_contains(&fetch_oids, &item->oid))
			clear_item(item);

		item = ರಿಕ್ತ;

		/* skip duplicates and refs that we already have */
		ಯದಿ (refname_hash_exists(&remote_refs, ref->name) ||
		    refname_hash_exists(&existing_refs, ref->name))
			ಅನುವರ್ತ;

		item = refname_hash_add(&remote_refs, ref->name, &ref->old_oid);
		string_list_ಸೇರಿಸು(&remote_refs_list, ref->name);
	}
	hashmap_clear_and_free(&existing_refs, ಸಂರಚನಾ refname_hash_entry, ent);

	/*
	 * We may have a final lightweight tag that needs to be
	 * checked to see if it needs fetching.
	 */
	ಯದಿ (item &&
	    !odb_has_object(the_repository->objects, &item->oid, 0) &&
	    !oidset_contains(&fetch_oids, &item->oid))
		clear_item(item);

	/*
	 * For all the tags in the remote_refs_list,
	 * add them to the list of refs to be fetched
	 */
	for_each_string_list_item(remote_ref_item, &remote_refs_list) {
		ಧ್ರುವ ವರ್ಣ *refname = remote_ref_item->string;
		ಸಂರಚನಾ ref *rm;
		unsigned ಪೂರ್ಣಾಙ್ಕ hash = strhash(refname);

		item = hashmap_get_entry_from_hash(&remote_refs, hash, refname,
					ಸಂರಚನಾ refname_hash_entry, ent);
		ಯದಿ (!item)
			BUG("unseen remote ref?");

		/* Unless we have already decided to ignore this item... */
		ಯದಿ (item->ignore)
			ಅನುವರ್ತ;

		rm = alloc_ref(item->refname);
		rm->peer_ref = alloc_ref(item->refname);
		oidcpy(&rm->old_oid, &item->oid);
		**tail = rm;
		*tail = &rm->next;
	}
	hashmap_clear_and_free(&remote_refs, ಸಂರಚನಾ refname_hash_entry, ent);
	string_list_clear(&remote_refs_list, 0);
	oidset_clear(&fetch_oids);
}

ಸ್ಥಿರ ಶೂನ್ಯ filter_prefetch_refspec(ಸಂರಚನಾ refspec *rs)
{
	ಪೂರ್ಣಾಙ್ಕ i;

	ಯದಿ (!prefetch)
		ಪ್ರತ್ಯಾಗಮ;

	ಪ್ರತಿ (i = 0; i < rs->nr; i++) {
		ಸಂರಚನಾ strbuf new_dst = STRBUF_INIT;
		ವರ್ಣ *old_dst;
		ಧ್ರುವ ವರ್ಣ *sub = ರಿಕ್ತ;

		ಯದಿ (rs->ವಸ್ತುಗಳು[i].negative)
			ಅನುವರ್ತ;
		ಯದಿ (!rs->ವಸ್ತುಗಳು[i].dst ||
		    (rs->ವಸ್ತುಗಳು[i].src &&
		     starts_with(rs->ವಸ್ತುಗಳು[i].src,
				 ref_namespace[NAMESPACE_TAGS].ref))) {
			ಪೂರ್ಣಾಙ್ಕ j;

			refspec_item_clear(&rs->ವಸ್ತುಗಳು[i]);

			ಪ್ರತಿ (j = i + 1; j < rs->nr; j++)
				rs->ವಸ್ತುಗಳು[j - 1] = rs->ವಸ್ತುಗಳು[j];
			rs->nr--;
			i--;
			ಅನುವರ್ತ;
		}

		old_dst = rs->ವಸ್ತುಗಳು[i].dst;
		strbuf_addstr(&new_dst, ref_namespace[NAMESPACE_PREFETCH].ref);

		/*
		 * If old_dst starts with "refs/", then place
		 * sub after that prefix. Otherwise, start at
		 * the beginning of the string.
		 */
		ಯದಿ (!skip_prefix(old_dst, "refs/", &sub))
			sub = old_dst;
		strbuf_addstr(&new_dst, sub);

		rs->ವಸ್ತುಗಳು[i].dst = strbuf_detach(&new_dst, ರಿಕ್ತ);
		rs->ವಸ್ತುಗಳು[i].force = 1;

		ಮುಕ್ತ(old_dst);
	}
}

ಸ್ಥಿರ ಸಂರಚನಾ ref *get_ref_map(ಸಂರಚನಾ remote *remote,
			       ಧ್ರುವ ಸಂರಚನಾ ref *remote_refs,
			       ಸಂರಚನಾ refspec *rs,
			       ಪೂರ್ಣಾಙ್ಕ tags, ಪೂರ್ಣಾಙ್ಕ *autotags)
{
	ಪೂರ್ಣಾಙ್ಕ i;
	ಸಂರಚನಾ ref *rm;
	ಸಂರಚನಾ ref *ref_map = ರಿಕ್ತ;
	ಸಂರಚನಾ ref **tail = &ref_map;

	/* opportunistically-updated references: */
	ಸಂರಚನಾ ref *orefs = ರಿಕ್ತ, **oref_tail = &orefs;

	ಸಂರಚನಾ hashmap existing_refs;
	ಪೂರ್ಣಾಙ್ಕ existing_refs_populated = 0;

	filter_prefetch_refspec(rs);
	ಯದಿ (remote)
		filter_prefetch_refspec(&remote->fetch);

	ಯದಿ (rs->nr) {
		ಸಂರಚನಾ refspec *fetch_refspec;

		ಪ್ರತಿ (i = 0; i < rs->nr; i++) {
			get_fetch_map(remote_refs, &rs->ವಸ್ತುಗಳು[i], &tail, 0);
			ಯದಿ (rs->ವಸ್ತುಗಳು[i].dst && rs->ವಸ್ತುಗಳು[i].dst[0])
				*autotags = 1;
		}
		/* Merge everything on the command line (but not --tags) */
		ಪ್ರತಿ (rm = ref_map; rm; rm = rm->next)
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
		ಯದಿ (refmap.nr)
			fetch_refspec = &refmap;
		ಅನ್ಯಥಾ
			fetch_refspec = &remote->fetch;

		ಪ್ರತಿ (i = 0; i < fetch_refspec->nr; i++)
			get_fetch_map(ref_map, &fetch_refspec->ವಸ್ತುಗಳು[i], &oref_tail, 1);
	} ಅನ್ಯಥಾ ಯದಿ (refmap.nr) {
		die("--refmap option is only meaningful with command-line refspec(s)");
	} ಅನ್ಯಥಾ {
		/* Use the defaults */
		ಸಂರಚನಾ branch *branch = branch_get(ರಿಕ್ತ);
		ಪೂರ್ಣಾಙ್ಕ has_merge = branch_has_merge_config(branch);
		ಯದಿ (remote &&
		    (remote->fetch.nr ||
		     /* Note: has_merge implies non-NULL branch->remote_name */
		     (has_merge && !strcmp(branch->remote_name, remote->name)))) {
			ಪ್ರತಿ (i = 0; i < remote->fetch.nr; i++) {
				get_fetch_map(remote_refs, &remote->fetch.ವಸ್ತುಗಳು[i], &tail, 0);
				ಯದಿ (remote->fetch.ವಸ್ತುಗಳು[i].dst &&
				    remote->fetch.ವಸ್ತುಗಳು[i].dst[0])
					*autotags = 1;
				ಯದಿ (!i && !has_merge && ref_map &&
				    !remote->fetch.ವಸ್ತುಗಳು[0].pattern)
					ref_map->fetch_head_status = FETCH_HEAD_MERGE;
			}
			/*
			 * if the remote we're fetching from is the same
			 * as given in branch.<name>.remote, we add the
			 * ref given in branch.<name>.merge, too.
			 *
			 * Note: has_merge implies non-NULL branch->remote_name
			 */
			ಯದಿ (has_merge &&
			    !strcmp(branch->remote_name, remote->name))
				add_merge_config(&ref_map, remote_refs, branch, &tail);
		} ಅನ್ಯಥಾ ಯದಿ (!prefetch) {
			ref_map = get_remote_ref(remote_refs, "HEAD");
			ಯದಿ (!ref_map)
				die(_("couldn't find remote ref HEAD"));
			ref_map->fetch_head_status = FETCH_HEAD_MERGE;
			tail = &ref_map->next;
		}
	}

	ಯದಿ (tags == TAGS_SET) {
		ಸಂರಚನಾ refspec_item tag_refspec;

		/* also fetch all tags */
		refspec_item_init_ತಳ್ಳು(&tag_refspec, TAG_REFSPEC,
				       the_hash_algo);
		get_fetch_map(remote_refs, &tag_refspec, &tail, 0);
		refspec_item_clear(&tag_refspec);
	} ಅನ್ಯಥಾ ಯದಿ (tags == TAGS_DEFAULT && *autotags) {
		find_non_local_tags(remote_refs, ರಿಕ್ತ, &ref_map, &tail);
	}

	/* Now append any refs to be updated opportunistically: */
	*tail = orefs;
	ಪ್ರತಿ (rm = orefs; rm; rm = rm->next) {
		rm->fetch_head_status = FETCH_HEAD_IGNORE;
		tail = &rm->next;
	}

	/*
	 * apply negative refspecs first, before we remove duplicates. This is
	 * necessary as negative refspecs might remove an otherwise conflicting
	 * duplicate.
	 */
	ಯದಿ (rs->nr)
		ref_map = apply_negative_refspecs(ref_map, rs);
	ಅನ್ಯಥಾ
		ref_map = apply_negative_refspecs(ref_map, &remote->fetch);

	ref_map = ref_remove_duplicates(ref_map);

	ಪ್ರತಿ (rm = ref_map; rm; rm = rm->next) {
		ಯದಿ (rm->peer_ref) {
			ಧ್ರುವ ವರ್ಣ *refname = rm->peer_ref->name;
			ಸಂರಚನಾ refname_hash_entry *peer_item;
			unsigned ಪೂರ್ಣಾಙ್ಕ hash = strhash(refname);

			ಯದಿ (!existing_refs_populated) {
				refname_hash_init(&existing_refs);
				refs_for_each_ref(get_main_ref_store(the_repository),
						  add_one_refname,
						  &existing_refs);
				existing_refs_populated = 1;
			}

			peer_item = hashmap_get_entry_from_hash(&existing_refs,
						hash, refname,
						ಸಂರಚನಾ refname_hash_entry, ent);
			ಯದಿ (peer_item) {
				ಸಂರಚನಾ object_id *old_oid = &peer_item->oid;
				oidcpy(&rm->peer_ref->old_oid, old_oid);
			}
		}
	}
	ಯದಿ (existing_refs_populated)
		hashmap_clear_and_free(&existing_refs, ಸಂರಚನಾ refname_hash_entry, ent);

	ಪ್ರತ್ಯಾಗಮ ref_map;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ s_update_ref(ಧ್ರುವ ವರ್ಣ *action,
			ಸಂರಚನಾ ref *ref,
			ಸಂರಚನಾ ref_transaction *transaction,
			ಪೂರ್ಣಾಙ್ಕ check_old)
{
	ವರ್ಣ *ಸಂದೇಶ;
	ವರ್ಣ *rla = getenv("GIT_REFLOG_ACTION");
	ಸಂರಚನಾ strbuf err = STRBUF_INIT;
	ಪೂರ್ಣಾಙ್ಕ ret;

	ಯದಿ (dry_run)
		ಪ್ರತ್ಯಾಗಮ 0;
	ಯದಿ (!rla)
		rla = default_rla.buf;
	ಸಂದೇಶ = xstrfmt("%s: %s", rla, action);

	ret = ref_transaction_update(transaction, ref->name, &ref->new_oid,
				     check_old ? &ref->old_oid : ರಿಕ್ತ,
				     ರಿಕ್ತ, ರಿಕ್ತ, 0, ಸಂದೇಶ, &err);

	ಯದಿ (ret)
		error("%s", err.buf);
	strbuf_release(&err);
	ಮುಕ್ತ(ಸಂದೇಶ);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ refcol_width(ಧ್ರುವ ಸಂರಚನಾ ref *ref_map, ಪೂರ್ಣಾಙ್ಕ compact_format)
{
	ಧ್ರುವ ಸಂರಚನಾ ref *ref;
	ಪೂರ್ಣಾಙ್ಕ max, width = 10;

	max = term_columns();
	ಯದಿ (compact_format)
		max = max * 2 / 3;

	ಪ್ರತಿ (ref = ref_map; ref; ref = ref->next) {
		ಪೂರ್ಣಾಙ್ಕ rlen, llen = 0, len;

		ಯದಿ (ref->status == REF_STATUS_REJECT_SHALLOW ||
		    !ref->peer_ref ||
		    !strcmp(ref->name, "HEAD"))
			ಅನುವರ್ತ;

		/* uptodate lines are only shown on high verbosity level */
		ಯದಿ (verbosity <= 0 && oideq(&ref->peer_ref->old_oid, &ref->old_oid))
			ಅನುವರ್ತ;

		rlen = utf8_strwidth(prettify_refname(ref->name));
		ಯದಿ (!compact_format)
			llen = utf8_strwidth(prettify_refname(ref->peer_ref->name));

		/*
		 * rough estimation to see if the output line is too long and
		 * should not be counted (we can't do precise calculation
		 * anyway because we don't know if the error explanation part
		 * will be printed in update_local_ref)
		 */
		len = 21 /* flag and summary */ + rlen + 4 /* -> */ + llen;
		ಯದಿ (len >= max)
			ಅನುವರ್ತ;

		ಯದಿ (width < rlen)
			width = rlen;
	}

	ಪ್ರತ್ಯಾಗಮ width;
}

ಸ್ಥಿರ ಶೂನ್ಯ display_state_init(ಸಂರಚನಾ display_state *display_state, ಸಂರಚನಾ ref *ref_map,
			       ಧ್ರುವ ವರ್ಣ *raw_url, ಗಣನಾ display_format format)
{
	ಪೂರ್ಣಾಙ್ಕ i;

	memset(display_state, 0, ಪರಿಮಾಣ(*display_state));
	strbuf_init(&display_state->buf, 0);
	display_state->format = format;

	ಯದಿ (raw_url)
		display_state->url = transport_anonymize_url(raw_url);
	ಅನ್ಯಥಾ
		display_state->url = xstrdup("foreign");

	display_state->url_len = strlen(display_state->url);
	ಪ್ರತಿ (i = display_state->url_len - 1; 0 <= i && display_state->url[i] == '/'; i--)
		;
	display_state->url_len = i + 1;
	ಯದಿ (4 < i && !strncmp(".git", display_state->url + i - 3, 4))
		display_state->url_len = i - 3;

	ಯದಿ (verbosity < 0)
		ಪ್ರತ್ಯಾಗಮ;

	ವಿಕಲ್ಪ (display_state->format) {
	ಪಕ್ಷ DISPLAY_FORMAT_FULL:
	ಪಕ್ಷ DISPLAY_FORMAT_COMPACT:
		display_state->refcol_width = refcol_width(ref_map,
							   display_state->format == DISPLAY_FORMAT_COMPACT);
		ಭಙ್ಗ;
	ಪಕ್ಷ DISPLAY_FORMAT_PORCELAIN:
		/* We don't need to precompute anything here. */
		ಭಙ್ಗ;
	ಸ್ವಭಾವ:
		BUG("unexpected display format %d", display_state->format);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ display_state_release(ಸಂರಚನಾ display_state *display_state)
{
	strbuf_release(&display_state->buf);
	ಮುಕ್ತ(display_state->url);
}

ಸ್ಥಿರ ಶೂನ್ಯ print_remote_to_local(ಸಂರಚನಾ display_state *display_state,
				  ಧ್ರುವ ವರ್ಣ *remote, ಧ್ರುವ ವರ್ಣ *local)
{
	strbuf_addf(&display_state->buf, "%-*s -> %s",
		    display_state->refcol_width, remote, local);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ find_and_replace(ಸಂರಚನಾ strbuf *haystack,
			    ಧ್ರುವ ವರ್ಣ *needle,
			    ಧ್ರುವ ವರ್ಣ *placeholder)
{
	ಧ್ರುವ ವರ್ಣ *p = ರಿಕ್ತ;
	ಪೂರ್ಣಾಙ್ಕ plen, nlen;

	nlen = strlen(needle);
	ಯದಿ (ends_with(haystack->buf, needle))
		p = haystack->buf + haystack->len - nlen;
	ಅನ್ಯಥಾ
		p = strstr(haystack->buf, needle);
	ಯದಿ (!p)
		ಪ್ರತ್ಯಾಗಮ 0;

	ಯದಿ (p > haystack->buf && p[-1] != '/')
		ಪ್ರತ್ಯಾಗಮ 0;

	plen = strlen(p);
	ಯದಿ (plen > nlen && p[nlen] != '/')
		ಪ್ರತ್ಯಾಗಮ 0;

	strbuf_splice(haystack, p - haystack->buf, nlen,
		      placeholder, strlen(placeholder));
	ಪ್ರತ್ಯಾಗಮ 1;
}

ಸ್ಥಿರ ಶೂನ್ಯ print_compact(ಸಂರಚನಾ display_state *display_state,
			  ಧ್ರುವ ವರ್ಣ *remote, ಧ್ರುವ ವರ್ಣ *local)
{
	ಸಂರಚನಾ strbuf r = STRBUF_INIT;
	ಸಂರಚನಾ strbuf l = STRBUF_INIT;

	ಯದಿ (!strcmp(remote, local)) {
		strbuf_addf(&display_state->buf, "%-*s -> *", display_state->refcol_width, remote);
		ಪ್ರತ್ಯಾಗಮ;
	}

	strbuf_addstr(&r, remote);
	strbuf_addstr(&l, local);

	ಯದಿ (!find_and_replace(&r, local, "*"))
		find_and_replace(&l, remote, "*");
	print_remote_to_local(display_state, r.buf, l.buf);

	strbuf_release(&r);
	strbuf_release(&l);
}

ಸ್ಥಿರ ಶೂನ್ಯ display_ref_update(ಸಂರಚನಾ display_state *display_state, ವರ್ಣ code,
			       ಧ್ರುವ ವರ್ಣ *summary, ಧ್ರುವ ವರ್ಣ *error,
			       ಧ್ರುವ ವರ್ಣ *remote, ಧ್ರುವ ವರ್ಣ *local,
			       ಧ್ರುವ ಸಂರಚನಾ object_id *old_oid,
			       ಧ್ರುವ ಸಂರಚನಾ object_id *new_oid,
			       ಪೂರ್ಣಾಙ್ಕ summary_width)
{
	FILE *f = stderr;

	ಯದಿ (verbosity < 0)
		ಪ್ರತ್ಯಾಗಮ;

	strbuf_reset(&display_state->buf);

	ವಿಕಲ್ಪ (display_state->format) {
	ಪಕ್ಷ DISPLAY_FORMAT_FULL:
	ಪಕ್ಷ DISPLAY_FORMAT_COMPACT: {
		ಪೂರ್ಣಾಙ್ಕ width;

		ಯದಿ (!display_state->shown_url) {
			strbuf_addf(&display_state->buf, _("From %.*s\n"),
				    display_state->url_len, display_state->url);
			display_state->shown_url = 1;
		}

		width = (summary_width + strlen(summary) - gettext_width(summary));
		remote = prettify_refname(remote);
		local = prettify_refname(local);

		strbuf_addf(&display_state->buf, " %c %-*s ", code, width, summary);

		ಯದಿ (display_state->format != DISPLAY_FORMAT_COMPACT)
			print_remote_to_local(display_state, remote, local);
		ಅನ್ಯಥಾ
			print_compact(display_state, remote, local);

		ಯದಿ (error)
			strbuf_addf(&display_state->buf, "  (%s)", error);

		ಭಙ್ಗ;
	}
	ಪಕ್ಷ DISPLAY_FORMAT_PORCELAIN:
		strbuf_addf(&display_state->buf, "%c %s %s %s", code,
			    oid_to_hex(old_oid), oid_to_hex(new_oid), local);
		f = stdout;
		ಭಙ್ಗ;
	ಸ್ವಭಾವ:
		BUG("unexpected display format %d", display_state->format);
	};
	strbuf_addch(&display_state->buf, '\n');

	fputs(display_state->buf.buf, f);
}

ಸಂರಚನಾ ref_update_display_info {
	bool failed;
	ವರ್ಣ success_code;
	ವರ್ಣ fail_code;
	ವರ್ಣ *summary;
	ವರ್ಣ *fail_detail;
	ವರ್ಣ *success_detail;
	ವರ್ಣ *ref;
	ವರ್ಣ *remote;
	ಸಂರಚನಾ object_id old_oid;
	ಸಂರಚನಾ object_id new_oid;
};

ಸಂರಚನಾ ref_update_display_info_array {
	ಸಂರಚನಾ ref_update_display_info *info;
	size_t alloc, nr;
};

ಸ್ಥಿರ ಸಂರಚನಾ ref_update_display_info *ref_update_display_info_append(
					   ಸಂರಚನಾ ref_update_display_info_array *array,
					   ವರ್ಣ success_code,
					   ವರ್ಣ fail_code,
					   ಧ್ರುವ ವರ್ಣ *summary,
					   ಧ್ರುವ ವರ್ಣ *success_detail,
					   ಧ್ರುವ ವರ್ಣ *fail_detail,
					   ಧ್ರುವ ವರ್ಣ *ref,
					   ಧ್ರುವ ವರ್ಣ *remote,
					   ಧ್ರುವ ಸಂರಚನಾ object_id *old_oid,
					   ಧ್ರುವ ಸಂರಚನಾ object_id *new_oid)
{
	ಸಂರಚನಾ ref_update_display_info *info;

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

	ಪ್ರತ್ಯಾಗಮ info;
}

ಸ್ಥಿರ ಶೂನ್ಯ ref_update_display_info_set_failed(ಸಂರಚನಾ ref_update_display_info *info)
{
	info->failed = true;
}

ಸ್ಥಿರ ಶೂನ್ಯ ref_update_display_info_free(ಸಂರಚನಾ ref_update_display_info *info)
{
	ಮುಕ್ತ(info->summary);
	ಮುಕ್ತ(info->success_detail);
	ಮುಕ್ತ(info->fail_detail);
	ಮುಕ್ತ(info->remote);
	ಮುಕ್ತ(info->ref);
}

ಸ್ಥಿರ ಶೂನ್ಯ ref_update_display_info_display(ಸಂರಚನಾ ref_update_display_info *info,
					    ಸಂರಚನಾ display_state *display_state,
					    ಪೂರ್ಣಾಙ್ಕ summary_width)
{
	display_ref_update(display_state,
			   info->failed ? info->fail_code : info->success_code,
			   info->summary,
			   info->failed ? info->fail_detail : info->success_detail,
			   info->remote, info->ref, &info->old_oid,
			   &info->new_oid, summary_width);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ update_local_ref(ಸಂರಚನಾ ref *ref,
			    ಸಂರಚನಾ ref_transaction *transaction,
			    ಧ್ರುವ ಸಂರಚನಾ ref *remote_ref,
			    ಧ್ರುವ ಸಂರಚನಾ fetch_config *config,
			    ಸಂರಚನಾ ref_update_display_info_array *display_array)
{
	ಸಂರಚನಾ commit *current = ರಿಕ್ತ, *updated;
	ಪೂರ್ಣಾಙ್ಕ fast_forward = 0;

	ಯದಿ (!odb_has_object(the_repository->objects, &ref->new_oid,
			    ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR))
		die(_("object %s not found"), oid_to_hex(&ref->new_oid));

	ಯದಿ (oideq(&ref->old_oid, &ref->new_oid)) {
		ಯದಿ (verbosity > 0)
			ref_update_display_info_append(display_array, '=', '=',
						       _("[up to date]"), ರಿಕ್ತ,
						       ರಿಕ್ತ, ref->name,
						       remote_ref->name, &ref->old_oid,
						       &ref->new_oid);
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (!update_head_ok &&
	    !is_null_oid(&ref->old_oid) &&
	    branch_checked_out(ref->name)) {
		ಸಂರಚನಾ ref_update_display_info *info;
		/*
		 * If this is the head, and it's not okay to update
		 * the head, and the old value of the head isn't empty...
		 */
		info = ref_update_display_info_append(display_array, '!', '!',
						      _("[rejected]"), ರಿಕ್ತ,
						      _("can't fetch into checked-out branch"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);
		ref_update_display_info_set_failed(info);
		ಪ್ರತ್ಯಾಗಮ 1;
	}

	ಯದಿ (!is_null_oid(&ref->old_oid) &&
	    starts_with(ref->name, "refs/tags/")) {
		ಸಂರಚನಾ ref_update_display_info *info;

		ಯದಿ (force || ref->force) {
			ಪೂರ್ಣಾಙ್ಕ r;

			r = s_update_ref("updating tag", ref, transaction, 0);

			info = ref_update_display_info_append(display_array, 't', '!',
							      _("[tag update]"), ರಿಕ್ತ,
							      _("unable to update local ref"),
							      ref->name, remote_ref->name,
							      &ref->old_oid, &ref->new_oid);
			ಯದಿ (r)
				ref_update_display_info_set_failed(info);

			ಪ್ರತ್ಯಾಗಮ r;
		} ಅನ್ಯಥಾ {
			info = ref_update_display_info_append(display_array, '!', '!',
							      _("[rejected]"), ರಿಕ್ತ,
							      _("would clobber existing tag"),
							      ref->name, remote_ref->name,
							      &ref->old_oid, &ref->new_oid);
			ref_update_display_info_set_failed(info);
			ಪ್ರತ್ಯಾಗಮ 1;
		}
	}

	current = lookup_commit_reference_gently(the_repository,
						 &ref->old_oid, 1);
	updated = lookup_commit_reference_gently(the_repository,
						 &ref->new_oid, 1);
	ಯದಿ (!current || !updated) {
		ಸಂರಚನಾ ref_update_display_info *info;
		ಧ್ರುವ ವರ್ಣ *ಸಂದೇಶ;
		ಧ್ರುವ ವರ್ಣ *what;
		ಪೂರ್ಣಾಙ್ಕ r;
		/*
		 * Nicely describe the new ref we're fetching.
		 * Base this on the remote's ref name, as it's
		 * more likely to follow a standard layout.
		 */
		ಯದಿ (starts_with(remote_ref->name, "refs/tags/")) {
			ಸಂದೇಶ = "storing tag";
			what = _("[new tag]");
		} ಅನ್ಯಥಾ ಯದಿ (starts_with(remote_ref->name, "refs/heads/")) {
			ಸಂದೇಶ = "storing head";
			what = _("[new branch]");
		} ಅನ್ಯಥಾ {
			ಸಂದೇಶ = "storing ref";
			what = _("[new ref]");
		}

		r = s_update_ref(ಸಂದೇಶ, ref, transaction, 0);

		info = ref_update_display_info_append(display_array, '*', '!',
						      what, ರಿಕ್ತ,
						      _("unable to update local ref"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);
		ಯದಿ (r)
			ref_update_display_info_set_failed(info);

		ಪ್ರತ್ಯಾಗಮ r;
	}

	ಯದಿ (config->show_forced_updates) {
		uint64_t t_before = getnanotime();
		fast_forward = repo_in_merge_bases(the_repository, current,
						   updated);
		ಯದಿ (fast_forward < 0)
			die(ರಿಕ್ತ);
		forced_updates_ms += (getnanotime() - t_before) / 1000000;
	} ಅನ್ಯಥಾ {
		fast_forward = 1;
	}

	ಯದಿ (fast_forward) {
		ಸಂರಚನಾ ref_update_display_info *info;
		ಸಂರಚನಾ strbuf quickref = STRBUF_INIT;
		ಪೂರ್ಣಾಙ್ಕ r;

		strbuf_add_unique_abbrev(&quickref, &current->object.oid, DEFAULT_ABBREV);
		strbuf_addstr(&quickref, "..");
		strbuf_add_unique_abbrev(&quickref, &ref->new_oid, DEFAULT_ABBREV);
		r = s_update_ref("fast-forward", ref, transaction, 1);

		info = ref_update_display_info_append(display_array, ' ', '!',
						      quickref.buf, ರಿಕ್ತ,
						      _("unable to update local ref"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);
		ಯದಿ (r)
			ref_update_display_info_set_failed(info);

		strbuf_release(&quickref);
		ಪ್ರತ್ಯಾಗಮ r;
	} ಅನ್ಯಥಾ ಯದಿ (force || ref->force) {
		ಸಂರಚನಾ ref_update_display_info *info;
		ಸಂರಚನಾ strbuf quickref = STRBUF_INIT;
		ಪೂರ್ಣಾಙ್ಕ r;

		strbuf_add_unique_abbrev(&quickref, &current->object.oid, DEFAULT_ABBREV);
		strbuf_addstr(&quickref, "...");
		strbuf_add_unique_abbrev(&quickref, &ref->new_oid, DEFAULT_ABBREV);
		r = s_update_ref("forced-update", ref, transaction, 1);

		info = ref_update_display_info_append(display_array, '+', '!',
						      quickref.buf, _("forced update"),
						      _("unable to update local ref"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);

		ಯದಿ (r)
			ref_update_display_info_set_failed(info);

		strbuf_release(&quickref);
		ಪ್ರತ್ಯಾಗಮ r;
	} ಅನ್ಯಥಾ {
		ಸಂರಚನಾ ref_update_display_info *info;
		info = ref_update_display_info_append(display_array, '!', '!',
						      _("[rejected]"), ರಿಕ್ತ,
						      _("non-fast-forward"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);
		ref_update_display_info_set_failed(info);
		ಪ್ರತ್ಯಾಗಮ 1;
	}
}

ಸ್ಥಿರ ಧ್ರುವ ಸಂರಚನಾ object_id *iterate_ref_map(ಶೂನ್ಯ *cb_data)
{
	ಸಂರಚನಾ ref **rm = cb_data;
	ಸಂರಚನಾ ref *ref = *rm;

	ಯಾವತ್ (ref && ref->status == REF_STATUS_REJECT_SHALLOW)
		ref = ref->next;
	ಯದಿ (!ref)
		ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
	*rm = ref->next;
	ಪ್ರತ್ಯಾಗಮ &ref->old_oid;
}

ಸಂರಚನಾ fetch_head {
	FILE *fp;
	ಸಂರಚನಾ strbuf buf;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ open_fetch_head(ಸಂರಚನಾ fetch_head *fetch_head)
{
	ಧ್ರುವ ವರ್ಣ *filename = git_path_fetch_head(the_repository);

	ಯದಿ (write_fetch_head) {
		fetch_head->fp = fopen(filename, "a");
		ಯದಿ (!fetch_head->fp)
			ಪ್ರತ್ಯಾಗಮ error_errno(_("cannot open '%s'"), filename);
		strbuf_init(&fetch_head->buf, 0);
	} ಅನ್ಯಥಾ {
		fetch_head->fp = ರಿಕ್ತ;
	}

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ append_fetch_head(ಸಂರಚನಾ fetch_head *fetch_head,
			      ಧ್ರುವ ಸಂರಚನಾ object_id *old_oid,
			      ಗಣನಾ fetch_head_status fetch_head_status,
			      ಧ್ರುವ ವರ್ಣ *note,
			      ಧ್ರುವ ವರ್ಣ *url, size_t url_len)
{
	ವರ್ಣ old_oid_hex[GIT_MAX_HEXSZ + 1];
	ಧ್ರುವ ವರ್ಣ *merge_status_marker;
	size_t i;

	ಯದಿ (!fetch_head->fp)
		ಪ್ರತ್ಯಾಗಮ;

	ವಿಕಲ್ಪ (fetch_head_status) {
	ಪಕ್ಷ FETCH_HEAD_NOT_FOR_MERGE:
		merge_status_marker = "not-for-merge";
		ಭಙ್ಗ;
	ಪಕ್ಷ FETCH_HEAD_MERGE:
		merge_status_marker = "";
		ಭಙ್ಗ;
	ಸ್ವಭಾವ:
		/* do not write anything to FETCH_HEAD */
		ಪ್ರತ್ಯಾಗಮ;
	}

	strbuf_addf(&fetch_head->buf, "%s\t%s\t%s",
		    oid_to_hex_r(old_oid_hex, old_oid), merge_status_marker, note);
	ಪ್ರತಿ (i = 0; i < url_len; ++i)
		ಯದಿ ('\n' == url[i])
			strbuf_addstr(&fetch_head->buf, "\\n");
		ಅನ್ಯಥಾ
			strbuf_addch(&fetch_head->buf, url[i]);
	strbuf_addch(&fetch_head->buf, '\n');

	/*
	 * When using an atomic fetch, we do not want to update FETCH_HEAD if
	 * any of the reference updates fails. We thus have to write all
	 * updates to a buffer first and only commit it as soon as all
	 * references have been successfully updated.
	 */
	ಯದಿ (!atomic_fetch) {
		strbuf_write(&fetch_head->buf, fetch_head->fp);
		strbuf_reset(&fetch_head->buf);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ commit_fetch_head(ಸಂರಚನಾ fetch_head *fetch_head)
{
	ಯದಿ (!fetch_head->fp || !atomic_fetch)
		ಪ್ರತ್ಯಾಗಮ;
	strbuf_write(&fetch_head->buf, fetch_head->fp);
}

ಸ್ಥಿರ ಶೂನ್ಯ close_fetch_head(ಸಂರಚನಾ fetch_head *fetch_head)
{
	ಯದಿ (!fetch_head->fp)
		ಪ್ರತ್ಯಾಗಮ;

	fclose(fetch_head->fp);
	strbuf_release(&fetch_head->buf);
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ warn_show_forced_updates[] =
N_("fetch normally indicates which branches had a forced update,\n"
   "but that check has been disabled; to re-enable, use '--show-forced-updates'\n"
   "flag or run 'git config fetch.showForcedUpdates true'");
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ warn_time_show_forced_updates[] =
N_("it took %.2f seconds to check forced updates; you can use\n"
   "'--no-show-forced-updates' or run 'git config fetch.showForcedUpdates false'\n"
   "to avoid this check\n");

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ store_updated_refs(ಸಂರಚನಾ display_state *display_state,
			      ಪೂರ್ಣಾಙ್ಕ connectivity_checked,
			      ಸಂರಚನಾ ref_transaction *transaction, ಸಂರಚನಾ ref *ref_map,
			      ಸಂರಚನಾ fetch_head *fetch_head,
			      ಧ್ರುವ ಸಂರಚನಾ fetch_config *config,
			      ಸಂರಚನಾ ref_update_display_info_array *display_array)
{
	ಪೂರ್ಣಾಙ್ಕ rc = 0;
	ಸಂರಚನಾ strbuf note = STRBUF_INIT;
	ಧ್ರುವ ವರ್ಣ *what, *kind;
	ಸಂರಚನಾ ref *rm;
	ಪೂರ್ಣಾಙ್ಕ want_status;

	ಯದಿ (!connectivity_checked) {
		ಸಂರಚನಾ check_connected_options opt = CHECK_CONNECTED_INIT;

		opt.exclude_hidden_refs_section = "fetch";
		rm = ref_map;
		ಯದಿ (check_connected(iterate_ref_map, &rm, &opt)) {
			rc = error(_("%s did not send all necessary objects"),
				   display_state->url);
			ಗಚ್ಛ abort;
		}
	}

	/*
	 * We do a pass for each fetch_head_status type in their enum order, so
	 * merged entries are written before not-for-merge. That lets readers
	 * use FETCH_HEAD as a refname to refer to the ref to be merged.
	 */
	ಪ್ರತಿ (want_status = FETCH_HEAD_MERGE;
	     want_status <= FETCH_HEAD_IGNORE;
	     want_status++) {
		ಪ್ರತಿ (rm = ref_map; rm; rm = rm->next) {
			ಸಂರಚನಾ ref *ref = ರಿಕ್ತ;

			ಯದಿ (rm->status == REF_STATUS_REJECT_SHALLOW) {
				ಯದಿ (want_status == FETCH_HEAD_MERGE)
					warning(_("rejected %s because shallow roots are not allowed to be updated"),
						rm->peer_ref ? rm->peer_ref->name : rm->name);
				ಅನುವರ್ತ;
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
			ಯದಿ (fetch_head->fp) {
				ಸಂರಚನಾ commit *commit = ರಿಕ್ತ;

				/*
				 * References in "refs/tags/" are often going to point
				 * to annotated tags, which are not part of the
				 * commit-graph. We thus only try to look up refs in
				 * the graph which are not in that namespace to not
				 * regress performance in repositories with many
				 * annotated tags.
				 */
				ಯದಿ (!starts_with(rm->name, "refs/tags/"))
					commit = lookup_commit_in_graph(the_repository, &rm->old_oid);
				ಯದಿ (!commit) {
					commit = lookup_commit_reference_gently(the_repository,
										&rm->old_oid,
										1);
					ಯದಿ (!commit)
						rm->fetch_head_status = FETCH_HEAD_NOT_FOR_MERGE;
				}
			}

			ಯದಿ (rm->fetch_head_status != want_status)
				ಅನುವರ್ತ;

			ಯದಿ (rm->peer_ref) {
				ref = alloc_ref(rm->peer_ref->name);
				oidcpy(&ref->old_oid, &rm->peer_ref->old_oid);
				oidcpy(&ref->new_oid, &rm->old_oid);
				ref->force = rm->peer_ref->force;
			}

			ಯದಿ (config->recurse_submodules != RECURSE_SUBMODULES_OFF &&
			    (!rm->peer_ref || !oideq(&ref->old_oid, &ref->new_oid))) {
				check_for_new_submodule_commits(&rm->old_oid);
			}

			ಯದಿ (!strcmp(rm->name, "HEAD")) {
				kind = "";
				what = "";
			} ಅನ್ಯಥಾ ಯದಿ (skip_prefix(rm->name, "refs/heads/", &what)) {
				kind = "branch";
			} ಅನ್ಯಥಾ ಯದಿ (skip_prefix(rm->name, "refs/tags/", &what)) {
				kind = "tag";
			} ಅನ್ಯಥಾ ಯದಿ (skip_prefix(rm->name, "refs/remotes/", &what)) {
				kind = "remote-tracking branch";
			} ಅನ್ಯಥಾ {
				kind = "";
				what = rm->name;
			}

			strbuf_reset(&note);
			ಯದಿ (*what) {
				ಯದಿ (*kind)
					strbuf_addf(&note, "%s ", kind);
				strbuf_addf(&note, "'%s' of ", what);
			}

			append_fetch_head(fetch_head, &rm->old_oid,
					  rm->fetch_head_status,
					  note.buf, display_state->url,
					  display_state->url_len);

			ಯದಿ (ref) {
				rc |= update_local_ref(ref, transaction, rm,
						       config, display_array);
				ಮುಕ್ತ(ref);
			} ಅನ್ಯಥಾ ಯದಿ (write_fetch_head || dry_run) {
				/*
				 * Display fetches written to FETCH_HEAD (or
				 * would be written to FETCH_HEAD, if --dry-run
				 * is set).
				 */

				ref_update_display_info_append(display_array, '*', '*',
							       *kind ? kind : "branch",
							       ರಿಕ್ತ, ರಿಕ್ತ, "FETCH_HEAD",
							       rm->name, &rm->new_oid,
							       &rm->old_oid);
			}
		}
	}

	ಯದಿ (advice_enabled(ADVICE_FETCH_SHOW_FORCED_UPDATES)) {
		ಯದಿ (!config->show_forced_updates) {
			warning(_(warn_show_forced_updates));
		} ಅನ್ಯಥಾ ಯದಿ (forced_updates_ms > FORCED_UPDATES_DELAY_WARNING_IN_MS) {
			warning(_(warn_time_show_forced_updates),
				forced_updates_ms / 1000.0);
		}
	}

 abort:
	strbuf_release(&note);
	ಪ್ರತ್ಯಾಗಮ rc;
}

/*
 * We would want to bypass the object transfer altogether if
 * everything we are going to fetch already exists and is connected
 * locally.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ check_exist_and_connected(ಸಂರಚನಾ ref *ref_map)
{
	ಸಂರಚನಾ ref *rm = ref_map;
	ಸಂರಚನಾ check_connected_options opt = CHECK_CONNECTED_INIT;
	ಸಂರಚನಾ ref *r;

	/*
	 * If we are deepening a shallow clone we already have these
	 * objects reachable.  Running rev-list here will return with
	 * a good (0) exit status and we'll bypass the fetch that we
	 * really need to perform.  Claiming failure now will ensure
	 * we perform the network exchange to deepen our history.
	 */
	ಯದಿ (deepen)
		ಪ್ರತ್ಯಾಗಮ -1;

	/*
	 * Similarly, if we need to refetch, we always want to perform a full
	 * fetch ignoring existing objects.
	 */
	ಯದಿ (refetch)
		ಪ್ರತ್ಯಾಗಮ -1;


	/*
	 * check_connected() allows objects to merely be promised, but
	 * we need all direct targets to exist.
	 */
	ಪ್ರತಿ (r = rm; r; r = r->next) {
		ಯದಿ (!odb_has_object(the_repository->objects, &r->old_oid,
				    ODB_HAS_OBJECT_RECHECK_PACKED))
			ಪ್ರತ್ಯಾಗಮ -1;
	}

	opt.quiet = 1;
	opt.exclude_hidden_refs_section = "fetch";
	ಪ್ರತ್ಯಾಗಮ check_connected(iterate_ref_map, &rm, &opt);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ fetch_and_consume_refs(ಸಂರಚನಾ display_state *display_state,
				  ಸಂರಚನಾ transport *transport,
				  ಸಂರಚನಾ ref_transaction *transaction,
				  ಸಂರಚನಾ ref *ref_map,
				  ಸಂರಚನಾ fetch_head *fetch_head,
				  ಧ್ರುವ ಸಂರಚನಾ fetch_config *config,
				  ಸಂರಚನಾ ref_update_display_info_array *display_array)
{
	ಪೂರ್ಣಾಙ್ಕ connectivity_checked = 1;
	ಪೂರ್ಣಾಙ್ಕ ret;

	/*
	 * We don't need to perform a fetch in case we can already satisfy all
	 * refs.
	 */
	ret = check_exist_and_connected(ref_map);
	ಯದಿ (ret) {
		trace2_region_enter("fetch", "fetch_refs", the_repository);
		ret = transport_fetch_refs(transport, ref_map);
		trace2_region_leave("fetch", "fetch_refs", the_repository);
		ಯದಿ (ret)
			ಗಚ್ಛ out;
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
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ prune_refs(ಸಂರಚನಾ display_state *display_state,
		      ಸಂರಚನಾ refspec *rs,
		      ಸಂರಚನಾ ref_transaction *transaction,
		      ಸಂರಚನಾ ref *ref_map)
{
	ಪೂರ್ಣಾಙ್ಕ result = 0;
	ಸಂರಚನಾ ref *ref, *stale_refs = get_stale_heads(rs, ref_map);
	ಸಂರಚನಾ strbuf err = STRBUF_INIT;
	ಸಂರಚನಾ string_list refnames = STRING_LIST_INIT_NODUP;

	ಪ್ರತಿ (ref = stale_refs; ref; ref = ref->next)
		string_list_append(&refnames, ref->name);

	ಯದಿ (!dry_run) {
		ಯದಿ (transaction) {
			ಪ್ರತಿ (ref = stale_refs; ref; ref = ref->next) {
				result = ref_transaction_delete(transaction, ref->name, ರಿಕ್ತ,
								ರಿಕ್ತ, 0, "fetch: prune", &err);
				ಯದಿ (result)
					ಗಚ್ಛ cleanup;
			}
		} ಅನ್ಯಥಾ {
			result = refs_delete_refs(get_main_ref_store(the_repository),
						  "fetch: prune", &refnames,
						  0);
		}
	}

	ಯದಿ (verbosity >= 0) {
		ಪೂರ್ಣಾಙ್ಕ summary_width = transport_summary_width(stale_refs);

		ಪ್ರತಿ (ref = stale_refs; ref; ref = ref->next) {
			display_ref_update(display_state, '-', _("[deleted]"), ರಿಕ್ತ,
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
	ಪ್ರತ್ಯಾಗಮ result;
}

ಸ್ಥಿರ ಶೂನ್ಯ check_not_current_branch(ಸಂರಚನಾ ref *ref_map)
{
	ಧ್ರುವ ವರ್ಣ *path;
	ಪ್ರತಿ (; ref_map; ref_map = ref_map->next)
		ಯದಿ (ref_map->peer_ref &&
		    starts_with(ref_map->peer_ref->name, "refs/heads/") &&
		    (path = branch_checked_out(ref_map->peer_ref->name)))
			die(_("refusing to fetch into branch '%s' "
			      "checked out at '%s'"),
			    ref_map->peer_ref->name, path);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ truncate_fetch_head(ಶೂನ್ಯ)
{
	ಧ್ರುವ ವರ್ಣ *filename = git_path_fetch_head(the_repository);
	FILE *fp = fopen_for_writing(filename);

	ಯದಿ (!fp)
		ಪ್ರತ್ಯಾಗಮ error_errno(_("cannot open '%s'"), filename);
	fclose(fp);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ set_option(ಸಂರಚನಾ transport *transport, ಧ್ರುವ ವರ್ಣ *name, ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ)
{
	ಪೂರ್ಣಾಙ್ಕ r = transport_set_option(transport, name, ಮೌಲ್ಯ);
	ಯದಿ (r < 0)
		die(_("option \"%s\" value \"%s\" is not valid for %s"),
		    name, ಮೌಲ್ಯ, transport->url);
	ಯದಿ (r > 0)
		warning(_("option \"%s\" is ignored for %s"),
			name, transport->url);
}


ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ add_oid(ಧ್ರುವ ಸಂರಚನಾ reference *ref, ಶೂನ್ಯ *cb_data)
{
	ಸಂರಚನಾ oid_array *oids = cb_data;

	oid_array_append(oids, ref->oid);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ add_negotiation_tips(ಸಂರಚನಾ string_list *ಒಳಹರಿವು_list,
				 ಸಂರಚನಾ oid_array **output_list,
				 ಧ್ರುವ ವರ್ಣ *argname)
{
	ಸಂರಚನಾ oid_array *oids = xcalloc(1, ಪರಿಮಾಣ(*oids));
	ಪೂರ್ಣಾಙ್ಕ i;

	ಪ್ರತಿ (i = 0; i < ಒಳಹರಿವು_list->nr; i++) {
		ಧ್ರುವ ವರ್ಣ *s = ಒಳಹರಿವು_list->ವಸ್ತುಗಳು[i].string;
		ಸಂರಚನಾ refs_for_each_ref_options opts = {
			.pattern = s,
		};
		ಪೂರ್ಣಾಙ್ಕ old_nr;
		ಯದಿ (!has_glob_specials(s)) {
			ಸಂರಚನಾ object_id oid;

			/* Ignore missing reference. */
			ಯದಿ (repo_get_oid(the_repository, s, &oid))
				ಅನುವರ್ತ;
			/* Fail on missing object pointed by ref. */
			ಯದಿ (!odb_has_object(the_repository->objects, &oid, 0))
				die(_("the object %s does not exist"), s);

			oid_array_append(oids, &oid);
			ಅನುವರ್ತ;
		}
		old_nr = oids->nr;
		refs_for_each_ref_ext(get_main_ref_store(the_repository),
				      add_oid, oids, &opts);
		ಯದಿ (old_nr == oids->nr)
			warning(_("ignoring %s=%s because it does not match any refs"),
				argname, s);
	}
	*output_list = oids;
}

ಸ್ಥಿರ ಸಂರಚನಾ transport *prepare_transport(ಸಂರಚನಾ remote *remote, ಪೂರ್ಣಾಙ್ಕ deepen,
					   ಸಂರಚನಾ list_objects_filter_options *filter_options)
{
	ಸಂರಚನಾ transport *transport;

	transport = transport_get(remote, ರಿಕ್ತ);
	transport_set_verbosity(transport, verbosity, progress);
	transport->family = family;
	ಯದಿ (upload_pack)
		set_option(transport, TRANS_OPT_UPLOADPACK, upload_pack);
	ಯದಿ (keep)
		set_option(transport, TRANS_OPT_KEEP, "yes");
	ಯದಿ (depth)
		set_option(transport, TRANS_OPT_DEPTH, depth);
	ಯದಿ (deepen && deepen_since)
		set_option(transport, TRANS_OPT_DEEPEN_SINCE, deepen_since);
	ಯದಿ (deepen && deepen_not.nr)
		set_option(transport, TRANS_OPT_DEEPEN_NOT,
			   (ಧ್ರುವ ವರ್ಣ *)&deepen_not);
	ಯದಿ (deepen_relative)
		set_option(transport, TRANS_OPT_DEEPEN_RELATIVE, "yes");
	ಯದಿ (update_shallow)
		set_option(transport, TRANS_OPT_UPDATE_SHALLOW, "yes");
	ಯದಿ (refetch)
		set_option(transport, TRANS_OPT_REFETCH, "yes");
	ಯದಿ (filter_options->choice) {
		ಧ್ರುವ ವರ್ಣ *spec =
			expand_list_objects_filter_spec(filter_options);
		set_option(transport, TRANS_OPT_LIST_OBJECTS_FILTER, spec);
		set_option(transport, TRANS_OPT_FROM_PROMISOR, "1");
	}
	ಯದಿ (negotiation_restrict.nr) {
		ಯದಿ (transport->smart_options)
			add_negotiation_tips(&negotiation_restrict,
					     &transport->smart_options->negotiation_restrict_tips,
					     "--negotiation-restrict");
		ಅನ್ಯಥಾ
			warning(_("ignoring %s because the protocol does not support it"),
				"--negotiation-restrict");
	} ಅನ್ಯಥಾ ಯದಿ (remote->negotiation_restrict.nr) {
		ಸಂರಚನಾ string_list_item *item;
		for_each_string_list_item(item, &remote->negotiation_restrict)
			string_list_append(&negotiation_restrict, item->string);
		ಯದಿ (transport->smart_options)
			add_negotiation_tips(&negotiation_restrict,
					     &transport->smart_options->negotiation_restrict_tips,
					     "--negotiation-restrict");
		ಅನ್ಯಥಾ {
			ಸಂರಚನಾ strbuf config_name = STRBUF_INIT;
			strbuf_addf(&config_name, "remote.%s.negotiationRestrict", remote->name);
			warning(_("ignoring %s because the protocol does not support it"),
				config_name.buf);
			strbuf_release(&config_name);
		}
	}
	ಯದಿ (negotiation_include.nr) {
		ಯದಿ (transport->smart_options)
			add_negotiation_tips(&negotiation_include,
					     &transport->smart_options->negotiation_include_tips,
					     "--negotiation-include");
		ಅನ್ಯಥಾ
			warning(_("ignoring %s because the protocol does not support it"),
				"--negotiation-include");
	} ಅನ್ಯಥಾ ಯದಿ (remote->negotiation_include.nr) {
		ಯದಿ (transport->smart_options) {
			add_negotiation_tips(&remote->negotiation_include,
					     &transport->smart_options->negotiation_include_tips,
					     "--negotiation-include");
		} ಅನ್ಯಥಾ {
			ಸಂರಚನಾ strbuf config_name = STRBUF_INIT;
			strbuf_addf(&config_name, "remote.%s.negotiationInclude", remote->name);
			warning(_("ignoring %s because the protocol does not support it"),
				config_name.buf);
			strbuf_release(&config_name);
		}
	}
	ಪ್ರತ್ಯಾಗಮ transport;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ backfill_tags(ಸಂರಚನಾ display_state *display_state,
			 ಸಂರಚನಾ transport *transport,
			 ಸಂರಚನಾ ref_transaction *transaction,
			 ಸಂರಚನಾ ref *ref_map,
			 ಸಂರಚನಾ fetch_head *fetch_head,
			 ಧ್ರುವ ಸಂರಚನಾ fetch_config *config,
			 ಸಂರಚನಾ ref_update_display_info_array *display_array,
			 ಸಂರಚನಾ list_objects_filter_options *filter_options)
{
	ಪೂರ್ಣಾಙ್ಕ retcode, cannot_reuse;

	/*
	 * Once we have set TRANS_OPT_DEEPEN_SINCE, we can't unset it
	 * when remote helper is used (setting it to an empty string
	 * is not unsetting). We could extend the remote helper
	 * protocol for that, but for now, just force a new connection
	 * without deepen-since. Similar story for deepen-not.
	 */
	cannot_reuse = transport->cannot_reuse ||
		deepen_since || deepen_not.nr;
	ಯದಿ (cannot_reuse) {
		gsecondary = prepare_transport(transport->remote, 0, filter_options);
		transport = gsecondary;
	}

	transport_set_option(transport, TRANS_OPT_FOLLOWTAGS, ರಿಕ್ತ);
	transport_set_option(transport, TRANS_OPT_DEPTH, "0");
	transport_set_option(transport, TRANS_OPT_DEEPEN_RELATIVE, ರಿಕ್ತ);
	retcode = fetch_and_consume_refs(display_state, transport, transaction, ref_map,
					 fetch_head, config, display_array);

	ಯದಿ (gsecondary) {
		transport_disconnect(gsecondary);
		gsecondary = ರಿಕ್ತ;
	}

	ಪ್ರತ್ಯಾಗಮ retcode;
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *strip_refshead(ಧ್ರುವ ವರ್ಣ *name){
	skip_prefix(name, "refs/heads/", &name);
	ಪ್ರತ್ಯಾಗಮ name;
}

ಸ್ಥಿರ ಶೂನ್ಯ set_head_advice_ಸಂದೇಶ(ಧ್ರುವ ವರ್ಣ *remote, ಧ್ರುವ ವರ್ಣ *head_name)
{
	ಧ್ರುವ ವರ್ಣ message_advice_set_head[] =
	N_("Run 'git remote set-head %s %s' to follow the change, or modify\n"
	   "either of the 'remote.%s.followRemoteHEAD' or 'fetch.followRemoteHEAD'\n"
	   "configuration variables to handle the situation differently.\n\n"

	   "Using this specific setting\n\n"
	   "    git config set remote.%s.followRemoteHEAD warn-if-not-%s\n\n"
	   "will suppress the warning until the remote changes HEAD to something else.");

	advise_if_enabled(ADVICE_FETCH_SET_HEAD_WARN, _(message_advice_set_head),
			remote, head_name, remote, remote, head_name);
}

ಸ್ಥಿರ ಶೂನ್ಯ warn_set_head(ಧ್ರುವ ವರ್ಣ *remote, ಧ್ರುವ ವರ್ಣ *head_name,
			ಸಂರಚನಾ strbuf *buf_prev, ಪೂರ್ಣಾಙ್ಕ updateres) {
	ಸಂರಚನಾ strbuf buf_prefix = STRBUF_INIT;
	ಧ್ರುವ ವರ್ಣ *prev_head = ರಿಕ್ತ;

	strbuf_addf(&buf_prefix, "refs/remotes/%s/", remote);
	skip_prefix(buf_prev->buf, buf_prefix.buf, &prev_head);

	ಯದಿ (prev_head && strcmp(prev_head, head_name)) {
		printf("'HEAD' at '%s' is '%s', but we have '%s' locally.\n",
			remote, head_name, prev_head);
		set_head_advice_ಸಂದೇಶ(remote, head_name);
	}
	ಅನ್ಯಥಾ ಯದಿ (updateres && buf_prev->len) {
		printf("'HEAD' at '%s' is '%s', "
			"but we have a detached HEAD pointing to '%s' locally.\n",
			remote, head_name, buf_prev->buf);
		set_head_advice_ಸಂದೇಶ(remote, head_name);
	}
	strbuf_release(&buf_prefix);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ set_head(ಧ್ರುವ ಸಂರಚನಾ ref *remote_refs, ಸಂರಚನಾ remote *remote,
			ಪೂರ್ಣಾಙ್ಕ follow_remote_head)
{
	ಪೂರ್ಣಾಙ್ಕ result = 0, create_only, baremirror, was_detached;
	ಸಂರಚನಾ strbuf b_head = STRBUF_INIT, b_remote_head = STRBUF_INIT,
		      b_local_head = STRBUF_INIT;
	ಧ್ರುವ ವರ್ಣ *no_warn_branch = remote->no_warn_branch;
	ವರ್ಣ *head_name = ರಿಕ್ತ;
	ಸಂರಚನಾ ref *ref, *matches;
	ಸಂರಚನಾ ref *fetch_map = ರಿಕ್ತ, **fetch_map_tail = &fetch_map;
	ಸಂರಚನಾ refspec_item refspec = {
		.force = 0,
		.pattern = 1,
		.src = (ವರ್ಣ *) "refs/heads/*",
		.dst = (ವರ್ಣ *) "refs/heads/*",
	};
	ಸಂರಚನಾ string_list heads = STRING_LIST_INIT_DUP;
	ಸಂರಚನಾ ref_store *refs = get_main_ref_store(the_repository);

	get_fetch_map(remote_refs, &refspec, &fetch_map_tail, 0);
	matches = guess_remote_head(find_ref_by_name(remote_refs, "HEAD"),
				    fetch_map, REMOTE_GUESS_HEAD_ALL);
	ಪ್ರತಿ (ref = matches; ref; ref = ref->next) {
		string_list_append(&heads, strip_refshead(ref->name));
	}

	ಯದಿ (!heads.nr)
		result = 1;
	ಅನ್ಯಥಾ ಯದಿ (heads.nr > 1)
		result = 1;
	ಅನ್ಯಥಾ
		head_name = xstrdup(heads.ವಸ್ತುಗಳು[0].string);

	ಯದಿ (!head_name)
		ಗಚ್ಛ cleanup;
	baremirror = is_bare_repository(the_repository) && remote->mirror;
	create_only = follow_remote_head == FOLLOW_REMOTE_ALWAYS ? 0 : !baremirror;
	ಯದಿ (baremirror) {
		strbuf_addstr(&b_head, "HEAD");
		strbuf_addf(&b_remote_head, "refs/heads/%s", head_name);
	} ಅನ್ಯಥಾ {
		strbuf_addf(&b_head, "refs/remotes/%s/HEAD", remote->name);
		strbuf_addf(&b_remote_head, "refs/remotes/%s/%s", remote->name, head_name);
	}
	/* make sure it's valid */
	ಯದಿ (!baremirror && !refs_ref_exists(refs, b_remote_head.buf)) {
		result = 1;
		ಗಚ್ಛ cleanup;
	}
	was_detached = refs_update_symref_extended(refs, b_head.buf, b_remote_head.buf,
					"fetch", &b_local_head, create_only);
	ಯದಿ (was_detached == -1) {
		result = 1;
		ಗಚ್ಛ cleanup;
	}
	ಯದಿ (verbosity >= 0 &&
		follow_remote_head == FOLLOW_REMOTE_WARN &&
		(!no_warn_branch || strcmp(no_warn_branch, head_name)))
		warn_set_head(remote->name, head_name, &b_local_head, was_detached);

cleanup:
	ಮುಕ್ತ(head_name);
	free_refs(fetch_map);
	free_refs(matches);
	string_list_clear(&heads, 0);
	strbuf_release(&b_head);
	strbuf_release(&b_local_head);
	strbuf_release(&b_remote_head);
	ಪ್ರತ್ಯಾಗಮ result;
}

ಸಂರಚನಾ ref_rejection_data {
	ಪೂರ್ಣಾಙ್ಕ *retcode;
	bool conflict_ಸಂದೇಶ_shown;
	bool case_sensitive_ಸಂದೇಶ_shown;
	ಧ್ರುವ ವರ್ಣ *remote_name;
	ಸಂರಚನಾ strmap *rejected_refs;
};

ಸ್ಥಿರ ಶೂನ್ಯ ref_transaction_rejection_handler(ಧ್ರುವ ವರ್ಣ *refname,
					      ಧ್ರುವ ಸಂರಚನಾ object_id *old_oid UNUSED,
					      ಧ್ರುವ ಸಂರಚನಾ object_id *new_oid UNUSED,
					      ಧ್ರುವ ವರ್ಣ *old_target UNUSED,
					      ಧ್ರುವ ವರ್ಣ *new_target UNUSED,
					      ಗಣನಾ ref_transaction_error err,
					      ಧ್ರುವ ವರ್ಣ *details,
					      ಶೂನ್ಯ *cb_data)
{
	ಸಂರಚನಾ ref_rejection_data *data = cb_data;

	ಯದಿ (err == REF_TRANSACTION_ERROR_CASE_CONFLICT && repo_ignore_case(the_repository) &&
	    !data->case_sensitive_ಸಂದೇಶ_shown) {
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
		data->case_sensitive_ಸಂದೇಶ_shown = true;
	} ಅನ್ಯಥಾ ಯದಿ (err == REF_TRANSACTION_ERROR_NAME_CONFLICT &&
		   !data->conflict_ಸಂದೇಶ_shown) {
		error(_("some local refs could not be updated; try running\n"
			" 'git remote prune %s' to remove any old, conflicting "
			"branches"), data->remote_name);
		data->conflict_ಸಂದೇಶ_shown = true;
	} ಅನ್ಯಥಾ {
		ಯದಿ (details)
			error("%s", details);
		ಅನ್ಯಥಾ
			error(_("fetching ref %s failed: %s"),
			      refname, ref_transaction_error_ಸಂದೇಶ(err));
	}

	strmap_put(data->rejected_refs, refname, ರಿಕ್ತ);
	*data->retcode = 1;
}

/*
 * Commit the reference transaction. If it isn't an atomic transaction, handle
 * rejected updates as part of using batched updates.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ commit_ref_transaction(ಸಂರಚನಾ ref_transaction **transaction,
				  bool is_atomic, ಧ್ರುವ ವರ್ಣ *remote_name,
				  ಸಂರಚನಾ strmap *rejected_refs,
				  ಸಂರಚನಾ strbuf *err)
{
	ಪೂರ್ಣಾಙ್ಕ retcode = ref_transaction_commit(*transaction, err);
	ಯದಿ (retcode)
		ಗಚ್ಛ out;

	ಯದಿ (!is_atomic) {
		ಸಂರಚನಾ ref_rejection_data data = {
			.conflict_ಸಂದೇಶ_shown = 0,
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
	*transaction = ರಿಕ್ತ;
	ಪ್ರತ್ಯಾಗಮ retcode;
}

ಸ್ಥಿರ ಶೂನ್ಯ collect_updated_tips(ಸಂರಚನಾ oidset *tips, ಸಂರಚನಾ ref *ref_map)
{
	ಸಂರಚನಾ ref *rm;
	ಪ್ರತಿ (rm = ref_map; rm; rm = rm->next) {
		ಸಂರಚನಾ commit *commit;
		/*
		 * Shallow-rejected refs are not stored and their history
		 * is incomplete, so skip them.
		 */
		ಯದಿ (rm->status == REF_STATUS_REJECT_SHALLOW)
			ಅನುವರ್ತ;
		ಯದಿ (is_null_oid(&rm->old_oid))
			ಅನುವರ್ತ;
		ಯದಿ (rm->peer_ref &&
		    oideq(&rm->old_oid, &rm->peer_ref->old_oid))
			ಅನುವರ್ತ;
		commit = lookup_commit_reference_gently(the_repository,
							&rm->old_oid, 1);
		ಯದಿ (commit)
			oidset_ಸೇರಿಸು(tips, &commit->object.oid);
	}
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ do_fetch(ಸಂರಚನಾ transport *transport,
		    ಸಂರಚನಾ refspec *rs,
		    ಧ್ರುವ ಸಂರಚನಾ fetch_config *config,
		    ಸಂರಚನಾ list_objects_filter_options *filter_options,
		    ಸಂರಚನಾ oidset *updated_tips)
{
	ಸಂರಚನಾ ref_transaction *transaction = ರಿಕ್ತ;
	ಸಂರಚನಾ ref *ref_map = ರಿಕ್ತ;
	ಸಂರಚನಾ display_state display_state = { 0 };
	ಪೂರ್ಣಾಙ್ಕ autotags = (transport->remote->fetch_tags == 1);
	ಪೂರ್ಣಾಙ್ಕ retcode = 0;
	ಧ್ರುವ ಸಂರಚನಾ ref *remote_refs;
	ಸಂರಚನಾ transport_ls_refs_options transport_ls_refs_options =
		TRANSPORT_LS_REFS_OPTIONS_INIT;
	ಸಂರಚನಾ fetch_head fetch_head = { 0 };
	ಸಂರಚನಾ strbuf err = STRBUF_INIT;
	ಸಂರಚನಾ ref_update_display_info_array display_array = { 0 };
	ಸಂರಚನಾ strmap rejected_refs = STRMAP_INIT;
	ಪೂರ್ಣಾಙ್ಕ summary_width = 0;
	ಪೂರ್ಣಾಙ್ಕ follow_remote_head = FOLLOW_REMOTE_NEVER;

	ಯದಿ (tags == TAGS_DEFAULT) {
		ಯದಿ (transport->remote->fetch_tags == 2)
			tags = TAGS_SET;
		ಯದಿ (transport->remote->fetch_tags == -1)
			tags = TAGS_UNSET;
	}

	/* if not appending, truncate FETCH_HEAD */
	ಯದಿ (!append && write_fetch_head) {
		retcode = truncate_fetch_head();
		ಯದಿ (retcode)
			ಗಚ್ಛ cleanup;
	}

	ಯದಿ (rs->nr) {
		refspec_ref_prefixes(rs, &transport_ls_refs_options.ref_prefixes);
	} ಅನ್ಯಥಾ {
		ಸಂರಚನಾ branch *branch = branch_get(ರಿಕ್ತ);

		ಯದಿ (transport->remote->fetch.nr) {
			refspec_ref_prefixes(&transport->remote->fetch,
					     &transport_ls_refs_options.ref_prefixes);
			/*
			 * See remote.c's handling of remote.<name>.followRemoteHEAD
			 * for the analogous, still-unresolved case.
			 */
			ಯದಿ (transport->remote->follow_remote_head)
				follow_remote_head = transport->remote->follow_remote_head;
			ಅನ್ಯಥಾ ಯದಿ (config->follow_remote_head_seen)
				follow_remote_head = get_follow_remote_head(config->follow_remote_head_raw);
			ಅನ್ಯಥಾ
				follow_remote_head = BUILTIN_FOLLOW_REMOTE_HEAD_DFLT;
		}
		ಯದಿ (branch && branch_has_merge_config(branch) &&
		    !strcmp(branch->remote_name, transport->remote->name)) {
			ಪೂರ್ಣಾಙ್ಕ i;
			ಪ್ರತಿ (i = 0; i < branch->merge_nr; i++) {
				strvec_ತಳ್ಳು(&transport_ls_refs_options.ref_prefixes,
					    branch->merge[i]->src);
			}
		}

		/*
		 * If there are no refs specified to fetch, then we just
		 * fetch HEAD; mention that to narrow the advertisement.
		 */
		ಯದಿ (!transport_ls_refs_options.ref_prefixes.nr)
			strvec_ತಳ್ಳು(&transport_ls_refs_options.ref_prefixes,
				    "HEAD");
	}

	ಯದಿ (tags == TAGS_SET || tags == TAGS_DEFAULT)
		strvec_ತಳ್ಳು(&transport_ls_refs_options.ref_prefixes,
			    "refs/tags/");

	ಯದಿ (follow_remote_head != FOLLOW_REMOTE_NEVER)
		strvec_ತಳ್ಳು(&transport_ls_refs_options.ref_prefixes,
			    "HEAD");

	/*
	 * Only initiate ref listing if we have at least one ref we want to
	 * know about.
	 */
	ಯದಿ (transport_ls_refs_options.ref_prefixes.nr) {
		trace2_region_enter("fetch", "remote_refs", the_repository);
		remote_refs = transport_get_remote_refs(transport,
							&transport_ls_refs_options);
		trace2_region_leave("fetch", "remote_refs", the_repository);
	} ಅನ್ಯಥಾ
		remote_refs = ರಿಕ್ತ;

	transport_ls_refs_options_release(&transport_ls_refs_options);

	ref_map = get_ref_map(transport->remote, remote_refs, rs,
			      tags, &autotags);
	ಯದಿ (!update_head_ok)
		check_not_current_branch(ref_map);

	retcode = open_fetch_head(&fetch_head);
	ಯದಿ (retcode)
		ಗಚ್ಛ cleanup;

	display_state_init(&display_state, ref_map, transport->url,
			   config->display_format);

	ಯದಿ (atomic_fetch) {
		transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
							  0, &err);
		ಯದಿ (!transaction) {
			retcode = -1;
			ಗಚ್ಛ cleanup;
		}
	}

	ಯದಿ (tags == TAGS_DEFAULT && autotags)
		transport_set_option(transport, TRANS_OPT_FOLLOWTAGS, "1");
	ಯದಿ (prune) {
		/*
		 * We only prune based on refspecs specified
		 * explicitly (via command line or configuration); we
		 * don't care whether --tags was specified.
		 */
		ಯದಿ (rs->nr) {
			retcode = prune_refs(&display_state, rs, transaction, ref_map);
		} ಅನ್ಯಥಾ {
			retcode = prune_refs(&display_state, &transport->remote->fetch,
					     transaction, ref_map);
		}
		ಯದಿ (retcode != 0)
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
	ಯದಿ (!transaction) {
		transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
							  REF_TRANSACTION_ALLOW_FAILURE, &err);
		ಯದಿ (!transaction) {
			retcode = -1;
			ಗಚ್ಛ cleanup;
		}
	}

	ಯದಿ (fetch_and_consume_refs(&display_state, transport, transaction, ref_map,
				   &fetch_head, config, &display_array)) {
		retcode = 1;
		ಗಚ್ಛ cleanup;
	}

	/*
	 * If neither --no-tags nor --tags was specified, do automated tag
	 * following.
	 */
	ಯದಿ (tags == TAGS_DEFAULT && autotags) {
		ಸಂರಚನಾ ref *tags_ref_map = ರಿಕ್ತ, **tail = &tags_ref_map;

		find_non_local_tags(remote_refs, transaction, &tags_ref_map, &tail);
		ಯದಿ (tags_ref_map) {
			/*
			 * If backfilling of tags fails then we want to tell
			 * the user so, but we have to continue regardless to
			 * populate upstream information of the references we
			 * have already fetched above. The exception though is
			 * when `--atomic` is passed: in that case we'll abort
			 * the transaction and don't commit anything.
			 */
			ಯದಿ (backfill_tags(&display_state, transport, transaction, tags_ref_map,
					  &fetch_head, config, &display_array, filter_options))
				retcode = 1;
		}

		free_refs(tags_ref_map);
	}

	ಯದಿ (retcode)
		ಗಚ್ಛ cleanup;

	ಯದಿ (verbosity >= 0)
		summary_width = transport_summary_width(ref_map);

	retcode = commit_ref_transaction(&transaction, atomic_fetch,
					 transport->remote->name,
					 &rejected_refs, &err);
	/*
	 * With '--atomic', bail out if the transaction fails. Without '--atomic',
	 * continue to fetch head and perform other post-fetch operations.
	 */
	ಯದಿ (retcode && atomic_fetch)
		ಗಚ್ಛ cleanup;

	commit_fetch_head(&fetch_head);

	collect_updated_tips(updated_tips, ref_map);

	ಯದಿ (set_upstream) {
		ಸಂರಚನಾ branch *branch = branch_get("HEAD");
		ಸಂರಚನಾ ref *rm;
		ಸಂರಚನಾ ref *source_ref = ರಿಕ್ತ;

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
		ಪ್ರತಿ (rm = ref_map; rm; rm = rm->next) {
			ಯದಿ (!rm->peer_ref) {
				ಯದಿ (source_ref) {
					warning(_("multiple branches detected, incompatible with --set-upstream"));
					ಗಚ್ಛ cleanup;
				} ಅನ್ಯಥಾ {
					source_ref = rm;
				}
			}
		}
		ಯದಿ (source_ref) {
			ಯದಿ (!branch) {
				ಧ್ರುವ ವರ್ಣ *shortname = source_ref->name;
				skip_prefix(shortname, "refs/heads/", &shortname);

				warning(_("could not set upstream of HEAD to '%s' from '%s' when "
					  "it does not point to any branch."),
					shortname, transport->remote->name);
				ಗಚ್ಛ cleanup;
			}

			ಯದಿ (!strcmp(source_ref->name, "HEAD") ||
			    starts_with(source_ref->name, "refs/heads/"))
				install_branch_config(0,
						      branch->name,
						      transport->remote->name,
						      source_ref->name);
			ಅನ್ಯಥಾ ಯದಿ (starts_with(source_ref->name, "refs/remotes/"))
				warning(_("not setting upstream for a remote remote-tracking branch"));
			ಅನ್ಯಥಾ ಯದಿ (starts_with(source_ref->name, "refs/tags/"))
				warning(_("not setting upstream for a remote tag"));
			ಅನ್ಯಥಾ
				warning(_("unknown branch type"));
		} ಅನ್ಯಥಾ {
			warning(_("no source branch found;\n"
				  "you need to specify exactly one branch with the --set-upstream option"));
		}
	}
	ಯದಿ (follow_remote_head != FOLLOW_REMOTE_NEVER) {
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
	ಯದಿ (retcode && !atomic_fetch && transaction)
		commit_ref_transaction(&transaction, false,
				       transport->remote->name,
				       &rejected_refs, &err);

	ಪ್ರತಿ (size_t i = 0; i < display_array.nr; i++) {
		ಸಂರಚನಾ ref_update_display_info *info = &display_array.info[i];

		ಯದಿ (!info->failed && strmap_contains(&rejected_refs, info->ref))
			ref_update_display_info_set_failed(info);
		ref_update_display_info_display(info, &display_state, summary_width);
		ref_update_display_info_free(info);
	}

	ಯದಿ (retcode) {
		ಯದಿ (err.len) {
			error("%s", err.buf);
			strbuf_reset(&err);
		}
		ಯದಿ (transaction && ref_transaction_abort(transaction, &err) &&
		    err.len)
			error("%s", err.buf);
		transaction = ರಿಕ್ತ;
	}

	ಯದಿ (transaction)
		ref_transaction_free(transaction);

	ಮುಕ್ತ(display_array.info);
	strmap_clear(&rejected_refs, 0);
	display_state_release(&display_state);
	close_fetch_head(&fetch_head);
	strbuf_release(&err);
	free_refs(ref_map);
	ಪ್ರತ್ಯಾಗಮ retcode;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ get_one_remote_for_fetch(ಸಂರಚನಾ remote *remote, ಶೂನ್ಯ *priv)
{
	ಸಂರಚನಾ string_list *list = priv;
	ಯದಿ (!remote->skip_default_update)
		string_list_append(list, remote->name);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ add_options_to_argv(ಸಂರಚನಾ strvec *argv,
				ಧ್ರುವ ಸಂರಚನಾ fetch_config *config)
{
	ಯದಿ (dry_run)
		strvec_ತಳ್ಳು(argv, "--dry-run");
	ಯದಿ (prune != -1)
		strvec_ತಳ್ಳು(argv, prune ? "--prune" : "--no-prune");
	ಯದಿ (prune_tags != -1)
		strvec_ತಳ್ಳು(argv, prune_tags ? "--prune-tags" : "--no-prune-tags");
	ಯದಿ (update_head_ok)
		strvec_ತಳ್ಳು(argv, "--update-head-ok");
	ಯದಿ (force)
		strvec_ತಳ್ಳು(argv, "--force");
	ಯದಿ (keep)
		strvec_ತಳ್ಳು(argv, "--keep");
	ಯದಿ (config->recurse_submodules == RECURSE_SUBMODULES_ON)
		strvec_ತಳ್ಳು(argv, "--recurse-submodules");
	ಅನ್ಯಥಾ ಯದಿ (config->recurse_submodules == RECURSE_SUBMODULES_OFF)
		strvec_ತಳ್ಳು(argv, "--no-recurse-submodules");
	ಅನ್ಯಥಾ ಯದಿ (config->recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND)
		strvec_ತಳ್ಳು(argv, "--recurse-submodules=on-demand");
	ಯದಿ (tags == TAGS_SET)
		strvec_ತಳ್ಳು(argv, "--tags");
	ಅನ್ಯಥಾ ಯದಿ (tags == TAGS_UNSET)
		strvec_ತಳ್ಳು(argv, "--no-tags");
	ಯದಿ (verbosity >= 2)
		strvec_ತಳ್ಳು(argv, "-v");
	ಯದಿ (verbosity >= 1)
		strvec_ತಳ್ಳು(argv, "-v");
	ಅನ್ಯಥಾ ಯದಿ (verbosity < 0)
		strvec_ತಳ್ಳು(argv, "-q");
	ಯದಿ (family == TRANSPORT_FAMILY_IPV4)
		strvec_ತಳ್ಳು(argv, "--ipv4");
	ಅನ್ಯಥಾ ಯದಿ (family == TRANSPORT_FAMILY_IPV6)
		strvec_ತಳ್ಳು(argv, "--ipv6");
	ಯದಿ (!write_fetch_head)
		strvec_ತಳ್ಳು(argv, "--no-write-fetch-head");
	ಯದಿ (config->display_format == DISPLAY_FORMAT_PORCELAIN)
		strvec_pushf(argv, "--porcelain");
}

/* Fetch multiple remotes in parallel */

ಸಂರಚನಾ parallel_fetch_state {
	ಧ್ರುವ ವರ್ಣ **argv;
	ಸಂರಚನಾ string_list *remotes;
	ಪೂರ್ಣಾಙ್ಕ next, result;
	ಧ್ರುವ ಸಂರಚನಾ fetch_config *config;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ fetch_next_remote(ಸಂರಚನಾ child_process *cp,
			     ಸಂರಚನಾ strbuf *out UNUSED,
			     ಶೂನ್ಯ *cb, ಶೂನ್ಯ **task_cb)
{
	ಸಂರಚನಾ parallel_fetch_state *state = cb;
	ವರ್ಣ *remote;

	ಯದಿ (state->next < 0 || state->next >= state->remotes->nr)
		ಪ್ರತ್ಯಾಗಮ 0;

	remote = state->remotes->ವಸ್ತುಗಳು[state->next++].string;
	*task_cb = remote;

	strvec_pushv(&cp->args, state->argv);
	strvec_ತಳ್ಳು(&cp->args, remote);
	cp->git_cmd = 1;

	ಯದಿ (verbosity >= 0 && state->config->display_format != DISPLAY_FORMAT_PORCELAIN)
		printf(_("Fetching %s\n"), remote);

	ಪ್ರತ್ಯಾಗಮ 1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ fetch_failed_to_start(ಸಂರಚನಾ strbuf *out UNUSED,
				 ಶೂನ್ಯ *cb, ಶೂನ್ಯ *task_cb)
{
	ಸಂರಚನಾ parallel_fetch_state *state = cb;
	ಧ್ರುವ ವರ್ಣ *remote = task_cb;

	state->result = error(_("could not fetch %s"), remote);

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ fetch_finished(ಪೂರ್ಣಾಙ್ಕ result, ಸಂರಚನಾ strbuf *out,
			  ಶೂನ್ಯ *cb, ಶೂನ್ಯ *task_cb)
{
	ಸಂರಚನಾ parallel_fetch_state *state = cb;
	ಧ್ರುವ ವರ್ಣ *remote = task_cb;

	ಯದಿ (result) {
		strbuf_addf(out, _("could not fetch '%s' (exit code: %d)\n"),
			    remote, result);
		state->result = -1;
	}

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ fetch_multiple(ಸಂರಚನಾ string_list *list, ಪೂರ್ಣಾಙ್ಕ max_children,
			  ಧ್ರುವ ಸಂರಚನಾ fetch_config *config)
{
	ಪೂರ್ಣಾಙ್ಕ i, result = 0;
	ಸಂರಚನಾ strvec argv = STRVEC_INIT;

	ಯದಿ (!append && write_fetch_head) {
		ಪೂರ್ಣಾಙ್ಕ errcode = truncate_fetch_head();
		ಯದಿ (errcode)
			ಪ್ರತ್ಯಾಗಮ errcode;
	}

	/*
	 * Cancel out the fetch.bundleURI config when running subprocesses,
	 * to avoid fetching from the same bundle list multiple times.
	 */
	strvec_pushl(&argv, "-c", "fetch.bundleURI=",
		     "fetch", "--append", "--no-auto-gc",
		     "--no-write-commit-graph", ರಿಕ್ತ);
	ಪ್ರತಿ (i = 0; i < server_options.nr; i++)
		strvec_pushf(&argv, "--server-option=%s", server_options.ವಸ್ತುಗಳು[i].string);
	add_options_to_argv(&argv, config);

	ಯದಿ (max_children != 1 && list->nr != 1) {
		ಸಂರಚನಾ parallel_fetch_state state = { argv.v, list, 0, 0, config };
		ಧ್ರುವ ಸಂರಚನಾ run_process_parallel_opts opts = {
			.tr2_category = "fetch",
			.tr2_label = "parallel/fetch",

			.processes = max_children,

			.get_next_task = &fetch_next_remote,
			.start_failure = &fetch_failed_to_start,
			.task_finished = &fetch_finished,
			.data = &state,
		};

		strvec_ತಳ್ಳು(&argv, "--end-of-options");

		run_processes_parallel(&opts);
		result = state.result;
	} ಅನ್ಯಥಾ
		ಪ್ರತಿ (i = 0; i < list->nr; i++) {
			ಧ್ರುವ ವರ್ಣ *name = list->ವಸ್ತುಗಳು[i].string;
			ಸಂರಚನಾ child_process cmd = CHILD_PROCESS_INIT;

			strvec_pushv(&cmd.args, argv.v);
			strvec_ತಳ್ಳು(&cmd.args, name);
			ಯದಿ (verbosity >= 0 && config->display_format != DISPLAY_FORMAT_PORCELAIN)
				printf(_("Fetching %s\n"), name);
			cmd.git_cmd = 1;
			ಯದಿ (run_command(&cmd)) {
				error(_("could not fetch %s"), name);
				result = 1;
			}
		}

	strvec_clear(&argv);
	ಪ್ರತ್ಯಾಗಮ !!result;
}

/*
 * Fetching from the promisor remote should use the given filter-spec
 * or inherit the default filter-spec from the config.
 */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ fetch_one_setup_partial(ಸಂರಚನಾ remote *remote,
					   ಸಂರಚನಾ list_objects_filter_options *filter_options)
{
	/*
	 * Explicit --no-filter argument overrides everything, regardless
	 * of any prior partial clones and fetches.
	 */
	ಯದಿ (filter_options->no_filter)
		ಪ್ರತ್ಯಾಗಮ;

	/*
	 * If no prior partial clone/fetch and the current fetch DID NOT
	 * request a partial-fetch, do a normal fetch.
	 */
	ಯದಿ (!repo_has_promisor_remote(the_repository) && !filter_options->choice)
		ಪ್ರತ್ಯಾಗಮ;

	/*
	 * If this is a partial-fetch request, we enable partial on
	 * this repo if not already enabled and remember the given
	 * filter-spec as the default for subsequent fetches to this
	 * remote if there is currently no default filter-spec.
	 */
	ಯದಿ (filter_options->choice) {
		partial_clone_register(remote->name, filter_options);
		ಪ್ರತ್ಯಾಗಮ;
	}

	/*
	 * Do a partial-fetch from the promisor remote using either the
	 * explicitly given filter-spec or inherit the filter-spec from
	 * the config.
	 */
	ಯದಿ (!filter_options->choice)
		partial_clone_get_default_filter_spec(filter_options, remote->name);
	ಪ್ರತ್ಯಾಗಮ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ fetch_one(ಸಂರಚನಾ remote *remote, ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv,
		     ಪೂರ್ಣಾಙ್ಕ prune_tags_ok, ಪೂರ್ಣಾಙ್ಕ use_stdin_refspecs,
		     ಧ್ರುವ ಸಂರಚನಾ fetch_config *config,
		     ಸಂರಚನಾ list_objects_filter_options *filter_options,
		     ಸಂರಚನಾ oidset *updated_tips)
{
	ಸಂರಚನಾ refspec rs = REFSPEC_INIT_FETCH(the_hash_algo);
	ಪೂರ್ಣಾಙ್ಕ i;
	ಪೂರ್ಣಾಙ್ಕ exit_code;
	ಪೂರ್ಣಾಙ್ಕ maybe_prune_tags;
	ಪೂರ್ಣಾಙ್ಕ remote_via_config = remote_is_configured(remote, 0);

	ಯದಿ (!remote)
		die(_("no remote repository specified; please specify either a URL or a\n"
		      "remote name from which new revisions should be fetched"));

	gtransport = prepare_transport(remote, 1, filter_options);

	ಯದಿ (prune < 0) {
		/* no command line request */
		ಯದಿ (0 <= remote->prune)
			prune = remote->prune;
		ಅನ್ಯಥಾ ಯದಿ (0 <= config->prune)
			prune = config->prune;
		ಅನ್ಯಥಾ
			prune = PRUNE_BY_DEFAULT;
	}

	ಯದಿ (prune_tags < 0) {
		/* no command line request */
		ಯದಿ (0 <= remote->prune_tags)
			prune_tags = remote->prune_tags;
		ಅನ್ಯಥಾ ಯದಿ (0 <= config->prune_tags)
			prune_tags = config->prune_tags;
		ಅನ್ಯಥಾ
			prune_tags = PRUNE_TAGS_BY_DEFAULT;
	}

	maybe_prune_tags = prune_tags_ok && prune_tags;
	ಯದಿ (maybe_prune_tags && remote_via_config)
		refspec_append(&remote->fetch, TAG_REFSPEC);

	ಯದಿ (maybe_prune_tags && (argc || !remote_via_config))
		refspec_append(&rs, TAG_REFSPEC);

	ಪ್ರತಿ (i = 0; i < argc; i++) {
		ಯದಿ (!strcmp(argv[i], "tag")) {
			i++;
			ಯದಿ (i >= argc)
				die(_("you need to specify a tag name"));

			refspec_appendf(&rs, "refs/tags/%s:refs/tags/%s",
					argv[i], argv[i]);
		} ಅನ್ಯಥಾ {
			refspec_append(&rs, argv[i]);
		}
	}

	ಯದಿ (use_stdin_refspecs) {
		ಸಂರಚನಾ strbuf ಸಾಲು = STRBUF_INIT;
		ಯಾವತ್ (strbuf_getline_lf(&ಸಾಲು, stdin) != EOF)
			refspec_append(&rs, ಸಾಲು.buf);
		strbuf_release(&ಸಾಲು);
	}

	ಯದಿ (server_options.nr)
		gtransport->server_options = &server_options;

	sigchain_ತಳ್ಳು_common(unlock_pack_on_signal);
	atexit(unlock_pack_atexit);
	sigchain_ತಳ್ಳು(SIGPIPE, SIG_IGN);
	exit_code = do_fetch(gtransport, &rs, config, filter_options,
			     updated_tips);
	sigchain_ತೆಗೆ(SIGPIPE);
	refspec_clear(&rs);
	transport_disconnect(gtransport);
	gtransport = ರಿಕ್ತ;
	ಪ್ರತ್ಯಾಗಮ exit_code;
}

ಪೂರ್ಣಾಙ್ಕ cmd_fetch(ಪೂರ್ಣಾಙ್ಕ argc,
	      ಧ್ರುವ ವರ್ಣ **argv,
	      ಧ್ರುವ ವರ್ಣ *prefix,
	      ಸಂರಚನಾ repository *repo UNUSED)
{
	ಸಂರಚನಾ fetch_config config = {
		.display_format = DISPLAY_FORMAT_FULL,
		.follow_remote_head_raw = ರಿಕ್ತ,
		.follow_remote_head_seen = 0,
		.prune = -1,
		.prune_tags = -1,
		.show_forced_updates = 1,
		.recurse_submodules = RECURSE_SUBMODULES_DEFAULT,
		.parallel = 1,
		.submodule_fetch_jobs = -1,
	};
	ಧ್ರುವ ವರ್ಣ *submodule_prefix = "";
	ಧ್ರುವ ವರ್ಣ *bundle_uri;
	ಸಂರಚನಾ string_list list = STRING_LIST_INIT_DUP;
	ಸಂರಚನಾ list_objects_filter_options filter_options = LIST_OBJECTS_FILTER_INIT;
	ಸಂರಚನಾ remote *remote = ರಿಕ್ತ;
	ಪೂರ್ಣಾಙ್ಕ all = -1, multiple = 0;
	ಪೂರ್ಣಾಙ್ಕ result = 0;
	ಪೂರ್ಣಾಙ್ಕ prune_tags_ok = 1;
	ಪೂರ್ಣಾಙ್ಕ enable_auto_gc = 1;
	ಪೂರ್ಣಾಙ್ಕ unshallow = 0;
	ಪೂರ್ಣಾಙ್ಕ max_jobs = -1;
	ಪೂರ್ಣಾಙ್ಕ recurse_submodules_cli = RECURSE_SUBMODULES_DEFAULT;
	ಪೂರ್ಣಾಙ್ಕ recurse_submodules_default = RECURSE_SUBMODULES_ON_DEMAND;
	ಪೂರ್ಣಾಙ್ಕ fetch_write_commit_graph = -1;
	ಪೂರ್ಣಾಙ್ಕ stdin_refspecs = 0;
	ಪೂರ್ಣಾಙ್ಕ negotiate_only = 0;
	ಪೂರ್ಣಾಙ್ಕ porcelain = 0;
	ಪೂರ್ಣಾಙ್ಕ i;
	ಗಣನಾ {
		GRAPH_WRITE_REACHABLE,
		GRAPH_WRITE_TIPS,
		GRAPH_WRITE_SKIP,
	} graph_write_mode = GRAPH_WRITE_REACHABLE;
	ಸಂರಚನಾ oidset updated_tips = OIDSET_INIT;

	ಸಂರಚನಾ option builtin_fetch_options[] = {
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
		OPT_SET_INT('n', ರಿಕ್ತ, &tags,
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
			.ಮೌಲ್ಯ = &submodule_prefix,
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
	ಪ್ರತಿ (i = 1; i < argc; i++) {
		/* This handles non-URLs gracefully */
		ವರ್ಣ *anon = transport_anonymize_url(argv[i]);

		strbuf_addf(&default_rla, " %s", anon);
		ಮುಕ್ತ(anon);
	}

	repo_config(the_repository, git_fetch_config, &config);
	ಯದಿ (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_full_index = 0;
	}

	argc = parse_options(argc, argv, prefix,
			     builtin_fetch_options, builtin_fetch_usage, 0);

	ಯದಿ (recurse_submodules_cli != RECURSE_SUBMODULES_DEFAULT)
		config.recurse_submodules = recurse_submodules_cli;

	ಯದಿ (negotiate_only) {
		ವಿಕಲ್ಪ (recurse_submodules_cli) {
		ಪಕ್ಷ RECURSE_SUBMODULES_OFF:
		ಪಕ್ಷ RECURSE_SUBMODULES_DEFAULT:
			/*
			 * --negotiate-only should never recurse into
			 * submodules. Skip it by setting recurse_submodules to
			 * RECURSE_SUBMODULES_OFF.
			 */
			config.recurse_submodules = RECURSE_SUBMODULES_OFF;
			ಭಙ್ಗ;

		ಸ್ವಭಾವ:
			die(_("options '%s' and '%s' cannot be used together"),
			    "--negotiate-only", "--recurse-submodules");
		}
	}

	ಯದಿ (config.recurse_submodules != RECURSE_SUBMODULES_OFF) {
		ಪೂರ್ಣಾಙ್ಕ *sfjc = config.submodule_fetch_jobs == -1
			    ? &config.submodule_fetch_jobs : ರಿಕ್ತ;
		ಪೂರ್ಣಾಙ್ಕ *rs = config.recurse_submodules == RECURSE_SUBMODULES_DEFAULT
			  ? &config.recurse_submodules : ರಿಕ್ತ;

		fetch_config_from_gitmodules(the_repository, sfjc, rs);
	}


	ಯದಿ (porcelain) {
		ವಿಕಲ್ಪ (recurse_submodules_cli) {
		ಪಕ್ಷ RECURSE_SUBMODULES_OFF:
		ಪಕ್ಷ RECURSE_SUBMODULES_DEFAULT:
			/*
			 * Reference updates in submodules would be ambiguous
			 * in porcelain mode, so we reject this combination.
			 */
			config.recurse_submodules = RECURSE_SUBMODULES_OFF;
			ಭಙ್ಗ;

		ಸ್ವಭಾವ:
			die(_("options '%s' and '%s' cannot be used together"),
			    "--porcelain", "--recurse-submodules");
		}

		config.display_format = DISPLAY_FORMAT_PORCELAIN;
	}

	ಯದಿ (deepen_relative) {
		ಯದಿ (deepen_relative < 0)
			die(_("negative depth in --deepen is not supported"));
		ಯದಿ (depth)
			die(_("options '%s' and '%s' cannot be used together"), "--deepen", "--depth");
		depth = xstrfmt("%d", deepen_relative);
	}
	ಯದಿ (unshallow) {
		ಯದಿ (depth)
			die(_("options '%s' and '%s' cannot be used together"), "--depth", "--unshallow");
		ಅನ್ಯಥಾ ಯದಿ (!is_repository_shallow(the_repository))
			die(_("--unshallow on a complete repository does not make sense"));
		ಅನ್ಯಥಾ
			depth = xstrfmt("%d", INFINITE_DEPTH);
	}

	/* no need to be strict, transport_set_option() will validate it again */
	ಯದಿ (depth && atoi(depth) < 1)
		die(_("depth %s is not a positive number"), depth);
	ಯದಿ (depth || deepen_since || deepen_not.nr)
		deepen = 1;

	/* FETCH_HEAD never gets updated in --dry-run mode */
	ಯದಿ (dry_run)
		write_fetch_head = 0;

	ಯದಿ (!max_jobs)
		max_jobs = online_cpus();

	ಯದಿ (!repo_config_get_string_tmp(the_repository, "fetch.bundleuri", &bundle_uri) &&
	    fetch_bundle_uri(the_repository, bundle_uri, ರಿಕ್ತ))
		warning(_("failed to fetch bundles from '%s'"), bundle_uri);

	ಯದಿ (all < 0) {
		/*
		 * no --[no-]all given;
		 * only use config option if no remote was explicitly specified
		 */
		all = (!argc) ? config.all : 0;
	}

	ಯದಿ (all) {
		ಯದಿ (argc == 1)
			die(_("fetch --all does not take a repository argument"));
		ಅನ್ಯಥಾ ಯದಿ (argc > 1)
			die(_("fetch --all does not make sense with refspecs"));

		(ಶೂನ್ಯ) for_each_remote(get_one_remote_for_fetch, &list);

		/* do not do fetch_multiple() of one */
		ಯದಿ (list.nr == 1)
			remote = remote_get(list.ವಸ್ತುಗಳು[0].string);
	} ಅನ್ಯಥಾ ಯದಿ (argc == 0) {
		/* No arguments -- use default remote */
		remote = remote_get(ರಿಕ್ತ);
	} ಅನ್ಯಥಾ ಯದಿ (multiple) {
		/* All arguments are assumed to be remotes or groups */
		ಪ್ರತಿ (i = 0; i < argc; i++)
			ಯದಿ (!add_remote_or_group(argv[i], &list))
				die(_("no such remote or remote group: %s"),
				    argv[i]);
	} ಅನ್ಯಥಾ {
		/* Single remote or group */
		(ಶೂನ್ಯ) add_remote_or_group(argv[0], &list);
		ಯದಿ (list.nr > 1) {
			/* More than one remote */
			ಯದಿ (argc > 1)
				die(_("fetching a group and specifying refspecs does not make sense"));
		} ಅನ್ಯಥಾ {
			/* Zero or one remotes */
			remote = remote_get(argv[0]);
			prune_tags_ok = (argc == 1);
			argc--;
			argv++;
		}
	}
	string_list_remove_duplicates(&list, 0);

	ಯದಿ (negotiate_only) {
		ಸಂರಚನಾ oidset acked_commits = OIDSET_INIT;
		ಸಂರಚನಾ oidset_iter iter;
		ಧ್ರುವ ಸಂರಚನಾ object_id *oid;

		trace2_region_enter("fetch", "negotiate-only", the_repository);
		ಯದಿ (!remote)
			die(_("must supply remote when using --negotiate-only"));
		gtransport = prepare_transport(remote, 1, &filter_options);

		ಯದಿ (!gtransport->smart_options) {
			warning(_("protocol does not support --negotiate-only, exiting"));
			result = 1;
			trace2_region_leave("fetch", "negotiate-only", the_repository);
			ಗಚ್ಛ cleanup;
		}
		ಯದಿ (!gtransport->smart_options->negotiation_restrict_tips)
			die(_("%s needs one or more %s"), "--negotiate-only",
			    "--negotiation-restrict=*");

		gtransport->smart_options->acked_commits = &acked_commits;

		ಯದಿ (server_options.nr)
			gtransport->server_options = &server_options;
		result = transport_fetch_refs(gtransport, ರಿಕ್ತ);
		gtransport->smart_options->acked_commits = ರಿಕ್ತ;

		oidset_iter_init(&acked_commits, &iter);
		ಯಾವತ್ ((oid = oidset_iter_next(&iter)))
			printf("%s\n", oid_to_hex(oid));
		oidset_clear(&acked_commits);
		trace2_region_leave("fetch", "negotiate-only", the_repository);
	} ಅನ್ಯಥಾ ಯದಿ (remote) {
		ಯದಿ (filter_options.choice || repo_has_promisor_remote(the_repository)) {
			trace2_region_enter("fetch", "setup-partial", the_repository);
			fetch_one_setup_partial(remote, &filter_options);
			trace2_region_leave("fetch", "setup-partial", the_repository);
		}
		trace2_region_enter("fetch", "fetch-one", the_repository);
		result = fetch_one(remote, argc, argv, prune_tags_ok, stdin_refspecs,
				   &config, &filter_options, &updated_tips);
		ಯದಿ (prepare_commit_graph(the_repository)) {
			ಯದಿ (oidset_size(&updated_tips))
				graph_write_mode = GRAPH_WRITE_TIPS;
			ಅನ್ಯಥಾ
				graph_write_mode = GRAPH_WRITE_SKIP;
		}
		trace2_region_leave("fetch", "fetch-one", the_repository);
	} ಅನ್ಯಥಾ {
		ಪೂರ್ಣಾಙ್ಕ max_children = max_jobs;

		ಯದಿ (filter_options.choice)
			die(_("--filter can only be used with the remote "
			      "configured in extensions.partialclone"));

		ಯದಿ (atomic_fetch)
			die(_("--atomic can only be used when fetching "
			      "from one remote"));

		ಯದಿ (stdin_refspecs)
			die(_("--stdin can only be used when fetching "
			      "from one remote"));

		ಯದಿ (max_children < 0)
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
	ಯದಿ (!result && remote && (config.recurse_submodules != RECURSE_SUBMODULES_OFF)) {
		ಸಂರಚನಾ strvec options = STRVEC_INIT;
		ಪೂರ್ಣಾಙ್ಕ max_children = max_jobs;

		ಯದಿ (max_children < 0)
			max_children = config.submodule_fetch_jobs;
		ಯದಿ (max_children < 0)
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
	ಯದಿ (negotiate_only)
		ಗಚ್ಛ cleanup;

	prepare_repo_settings(the_repository);
	ಯದಿ (fetch_write_commit_graph > 0 ||
	    (fetch_write_commit_graph < 0 &&
	     the_repository->settings.fetch_write_commit_graph)) {
		ಪೂರ್ಣಾಙ್ಕ commit_graph_flags = COMMIT_GRAPH_WRITE_SPLIT;

		ಯದಿ (progress)
			commit_graph_flags |= COMMIT_GRAPH_WRITE_PROGRESS;

		ಯದಿ (graph_write_mode != GRAPH_WRITE_SKIP) {
			trace2_region_enter("fetch", "write-commit-graph",
					    the_repository);
			ಯದಿ (graph_write_mode == GRAPH_WRITE_TIPS)
				write_commit_graph(
					the_repository,
					the_repository->objects->source->path,
					ರಿಕ್ತ, &updated_tips,
					commit_graph_flags, ರಿಕ್ತ);
			ಅನ್ಯಥಾ
				write_commit_graph_reachable(
					the_repository,
					the_repository->objects->source->path,
					commit_graph_flags, ರಿಕ್ತ);
			trace2_region_leave("fetch", "write-commit-graph",
					    the_repository);
		}
	}

	ಯದಿ (enable_auto_gc) {
		ಯದಿ (refetch) {
			/*
			 * Hint auto-maintenance strongly to encourage repacking,
			 * but respect config settings disabling it.
			 */
			ಪೂರ್ಣಾಙ್ಕ opt_val;
			ಯದಿ (repo_config_get_int(the_repository, "gc.autopacklimit", &opt_val))
				opt_val = -1;
			ಯದಿ (opt_val != 0)
				git_config_ತಳ್ಳು_parameter("gc.autoPackLimit=1");

			ಯದಿ (repo_config_get_int(the_repository, "maintenance.incremental-repack.auto", &opt_val))
				opt_val = -1;
			ಯದಿ (opt_val != 0)
				git_config_ತಳ್ಳು_parameter("maintenance.incremental-repack.auto=-1");
		}
		run_auto_maintenance(the_repository, verbosity < 0);
	}

 cleanup:
	oidset_clear(&updated_tips);
	string_list_clear(&list, 0);
	list_objects_filter_release(&filter_options);
	ಮುಕ್ತ(config.follow_remote_head_raw);
	ಪ್ರತ್ಯಾಗಮ result;
}
