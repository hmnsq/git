/*
 * "git fetch"
 */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DISABLE_SIGN_COMPARE_WARNINGS

#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "advice.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hex.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "refs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "refspec.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object-name.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "odb.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "odb/source.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "oidset.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "oid-array.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "commit.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "string-list.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "remote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "transport.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "run-command.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "sigchain.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "submodule-config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "submodule.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "connected.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "strvec.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utf8.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "pager.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "path.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "pkt-line.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "list-objects-filter-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "commit-reach.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "branch.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "promisor-remote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "commit-graph.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "shallow.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "trace.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "trace2.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "bundle-uri.h"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FORCED_UPDATES_DELAY_WARNING_ਵਿੱਚ_MS (10 * 1000)

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ builtin_fetch_usage[] = {
	N_("git fetch [<options>] [<repository> [<refspec>...]]"),
	N_("git fetch [<options>] <group>"),
	N_("git fetch --multiple [<options>] [(<repository>|<group>)...]"),
	N_("git fetch --all [<options>]"),
	ਨਿਰਾ
};

ਗਿਣਤੀ {
	ਟੈਗ_ਸੂਚੀ_UNSET = 0,
	ਟੈਗ_ਸੂਚੀ_DEFAULT = 1,
	ਟੈਗ_ਸੂਚੀ_ਸੈੱਟ = 2
};

ਗਿਣਤੀ display_format {
	DISPLAY_FORMAT_ਭਰਿਆ,
	DISPLAY_FORMAT_COMPACT,
	DISPLAY_FORMAT_PORCELAIN,
};

ਬਣਤਰ display_state {
	ਬਣਤਰ strbuf buf;

	ਪੂਰਨ refcol_ਚੌੜਾਈ;
	ਗਿਣਤੀ display_format format;

	ਅੱਖਰ *url;
	ਪੂਰਨ url_len, shown_url;
};

ਸਥਿਰ uint64_t forced_updates_ms = 0;
ਸਥਿਰ ਪੂਰਨ prefetch = 0;
ਸਥਿਰ ਪੂਰਨ prune = -1; /* unspecified */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PRUNE_BY_DEFAULT 0 /* do we prune by default? */

ਸਥਿਰ ਪੂਰਨ prune_ਟੈਗ_ਸੂਚੀ = -1; /* unspecified */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PRUNE_ਟੈਗ_ਸੂਚੀ_BY_DEFAULT 0 /* do we prune tags by default? */

ਸਥਿਰ ਪੂਰਨ append, dry_ਲੜੀਵਾਰ, force, keep, update_ਸਿਰ_ok;
ਸਥਿਰ ਪੂਰਨ write_fetch_ਸਿਰ = 1;
ਸਥਿਰ ਪੂਰਨ verbosity, deepen_relative, ਸੈੱਟ_upstream, refetch;
ਸਥਿਰ ਪੂਰਨ ਤਰੱਕੀ = -1;
ਸਥਿਰ ਪੂਰਨ ਟੈਗ_ਸੂਚੀ = ਟੈਗ_ਸੂਚੀ_DEFAULT, update_shallow, deepen;
ਸਥਿਰ ਪੂਰਨ atomic_fetch;
ਸਥਿਰ ਗਿਣਤੀ transport_family family;
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਡੂੰਘਾਈ;
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *deepen_since;
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *upload_pack;
ਸਥਿਰ ਬਣਤਰ string_list deepen_not = STRING_LIST_INIT_NODUP;
ਸਥਿਰ ਬਣਤਰ strbuf default_rla = STRBUF_INIT;
ਸਥਿਰ ਬਣਤਰ transport *gtransport;
ਸਥਿਰ ਬਣਤਰ transport *gsecondary;
ਸਥਿਰ ਬਣਤਰ refspec refmap;
ਸਥਿਰ ਬਣਤਰ string_list server_options = STRING_LIST_INIT_DUP;
ਸਥਿਰ ਬਣਤਰ string_list negotiation_restrict = STRING_LIST_INIT_NODUP;
ਸਥਿਰ ਬਣਤਰ string_list negotiation_include = STRING_LIST_INIT_NODUP;

ਬਣਤਰ fetch_config {
	ਗਿਣਤੀ display_format display_format;
	ਅੱਖਰ *follow_remote_ਸਿਰ_raw;
	ਪੂਰਨ follow_remote_ਸਿਰ_ਵੇਖਿਆ;
	ਪੂਰਨ all;
	ਪੂਰਨ prune;
	ਪੂਰਨ prune_ਟੈਗ_ਸੂਚੀ;
	ਪੂਰਨ show_forced_updates;
	ਪੂਰਨ recurse_submodules;
	ਪੂਰਨ parallel;
	ਪੂਰਨ submodule_fetch_jobs;
};

ਸਥਿਰ ਪੂਰਨ git_fetch_config(ਅਚਲ ਅੱਖਰ *k, ਅਚਲ ਅੱਖਰ *v,
			    ਅਚਲ ਬਣਤਰ config_context *ctx, ਖਾਲੀ *cb)
{
	ਬਣਤਰ fetch_config *fetch_config = cb;

	ਜੇ (!strcmp(k, "fetch.all")) {
		fetch_config->all = git_config_bool(k, v);
		ਵਾਪਸ 0;
	}

	ਜੇ (!strcmp(k, "fetch.prune")) {
		fetch_config->prune = git_config_bool(k, v);
		ਵਾਪਸ 0;
	}

	ਜੇ (!strcmp(k, "fetch.prunetags")) {
		fetch_config->prune_ਟੈਗ_ਸੂਚੀ = git_config_bool(k, v);
		ਵਾਪਸ 0;
	}

	ਜੇ (!strcmp(k, "fetch.showforcedupdates")) {
		fetch_config->show_forced_updates = git_config_bool(k, v);
		ਵਾਪਸ 0;
	}

	ਜੇ (!strcmp(k, "submodule.recurse")) {
		ਪੂਰਨ r = git_config_bool(k, v) ?
			RECURSE_SUBMODULES_ON : RECURSE_SUBMODULES_OFF;
		fetch_config->recurse_submodules = r;
		ਵਾਪਸ 0;
	}

	ਜੇ (!strcmp(k, "submodule.fetchjobs")) {
		fetch_config->submodule_fetch_jobs = parse_submodule_fetchjobs(k, v, ctx->kvi);
		ਵਾਪਸ 0;
	} ਨਹੀਂਤਾਂ ਜੇ (!strcmp(k, "fetch.recursesubmodules")) {
		fetch_config->recurse_submodules = parse_fetch_recurse_submodules_arg(k, v);
		ਵਾਪਸ 0;
	}

	ਜੇ (!strcmp(k, "fetch.parallel")) {
		fetch_config->parallel = git_config_int(k, v, ctx->kvi);
		ਜੇ (fetch_config->parallel < 0)
			die(_("fetch.parallel cannot be negative"));
		ਜੇ (!fetch_config->parallel)
			fetch_config->parallel = online_cpus();
		ਵਾਪਸ 0;
	}

	ਜੇ (!strcmp(k, "fetch.output")) {
		ਜੇ (!v)
			ਵਾਪਸ config_error_nonbool(k);
		ਨਹੀਂਤਾਂ ਜੇ (!strcasecmp(v, "full"))
			fetch_config->display_format = DISPLAY_FORMAT_ਭਰਿਆ;
		ਨਹੀਂਤਾਂ ਜੇ (!strcasecmp(v, "compact"))
			fetch_config->display_format = DISPLAY_FORMAT_COMPACT;
		ਨਹੀਂਤਾਂ
			die(_("invalid value for '%s': '%s'"),
			    "fetch.output", v);
		ਵਾਪਸ 0;
	}

	ਜੇ (!strcmp(k, "fetch.followremotehead")) {
		ਮੁਕਤ(fetch_config->follow_remote_ਸਿਰ_raw);
		fetch_config->follow_remote_ਸਿਰ_raw = xstrdup_or_null(v);
		fetch_config->follow_remote_ਸਿਰ_ਵੇਖਿਆ = 1;
		ਵਾਪਸ 0;
	}

	ਵਾਪਸ git_default_config(k, v, ctx, cb);
}

ਸਥਿਰ ਗਿਣਤੀ follow_remote_ਸਿਰ_settings get_follow_remote_ਸਿਰ(ਅਚਲ ਅੱਖਰ *setting)
{
	ਜੇ (!setting)
		die(_("missing value for 'fetch.followRemoteHEAD'"));
	ਨਹੀਂਤਾਂ ਜੇ (!strcmp(setting, "never"))
		ਵਾਪਸ FOLLOW_REMOTE_NEVER;
	ਨਹੀਂਤਾਂ ਜੇ (!strcmp(setting, "create"))
		ਵਾਪਸ FOLLOW_REMOTE_CREATE;
	ਨਹੀਂਤਾਂ ਜੇ (!strcmp(setting, "warn"))
		ਵਾਪਸ FOLLOW_REMOTE_WARN;
	ਨਹੀਂਤਾਂ ਜੇ (!strcmp(setting, "always"))
		ਵਾਪਸ FOLLOW_REMOTE_ALWAYS;
	warning(_("unrecognized fetch.followRemoteHEAD value '%s' ignored"), setting);
	ਵਾਪਸ BUILTIN_FOLLOW_REMOTE_ਸਿਰ_DFLT;
}

ਸਥਿਰ ਪੂਰਨ parse_refmap_arg(ਅਚਲ ਬਣਤਰ option *opt, ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	BUG_ON_OPT_NEG(unset);

	/*
	 * "git fetch --refmap='' origin foo"
	 * can be used to tell the command not to store anywhere
	 */
	refspec_append(opt->ਮੁੱਲ, arg);

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ unlock_pack(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags)
{
	ਜੇ (gtransport)
		transport_unlock_pack(gtransport, flags);
	ਜੇ (gsecondary)
		transport_unlock_pack(gsecondary, flags);
}

ਸਥਿਰ ਖਾਲੀ unlock_pack_atexit(ਖਾਲੀ)
{
	unlock_pack(0);
}

ਸਥਿਰ ਖਾਲੀ unlock_pack_on_signal(ਪੂਰਨ signo)
{
	unlock_pack(TRANSPORT_UNLOCK_PACK_ਵਿੱਚ_SIGNAL_HANDLER);
	sigchain_ਕੱਢੋ(signo);
	ਉਠਾਓ(signo);
}

ਸਥਿਰ ਖਾਲੀ ਜੋੜੋ_ਮਿਲਾਓ_config(ਬਣਤਰ ref **ਸਿਰ,
			   ਅਚਲ ਬਣਤਰ ref *remote_refs,
		           ਬਣਤਰ branch *branch,
		           ਬਣਤਰ ref ***ਪੂਛ)
{
	ਪੂਰਨ i;

	ਲਈ (i = 0; i < branch->ਮਿਲਾਓ_nr; i++) {
		ਬਣਤਰ ref *rm, **old_ਪੂਛ = *ਪੂਛ;
		ਬਣਤਰ refspec_ਵਸਤੂ refspec;

		ਲਈ (rm = *ਸਿਰ; rm; rm = rm->ਅਗਲਾ) {
			ਜੇ (branch_ਮਿਲਾਓ_matches(branch, i, rm->name)) {
				rm->fetch_ਸਿਰ_ਸਥਿਤੀ = FETCH_ਸਿਰ_ਮਿਲਾਓ;
				ਰੋਕੋ;
			}
		}
		ਜੇ (rm)
			ਜਾਰੀ;

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
		memset(&refspec, 0, ਆਕਾਰ(refspec));
		refspec.src = branch->ਮਿਲਾਓ[i]->src;
		get_fetch_map(remote_refs, &refspec, ਪੂਛ, 1);
		ਲਈ (rm = *old_ਪੂਛ; rm; rm = rm->ਅਗਲਾ)
			rm->fetch_ਸਿਰ_ਸਥਿਤੀ = FETCH_ਸਿਰ_ਮਿਲਾਓ;
	}
}

ਸਥਿਰ ਖਾਲੀ create_fetch_oidset(ਬਣਤਰ ref **ਸਿਰ, ਬਣਤਰ oidset *ਬਾਹਰਲਾ)
{
	ਬਣਤਰ ref *rm = *ਸਿਰ;
	ਜਦੋਂ (rm) {
		oidset_ਪਾਓ(ਬਾਹਰਲਾ, &rm->old_oid);
		rm = rm->ਅਗਲਾ;
	}
}

ਬਣਤਰ refname_hash_ਇੰਦਰਾਜ {
	ਬਣਤਰ hashmap_ਇੰਦਰਾਜ ent;
	ਬਣਤਰ object_id oid;
	ਪੂਰਨ ignore;
	ਅੱਖਰ refname[FLEX_ARRAY];
};

ਸਥਿਰ ਪੂਰਨ refname_hash_ਇੰਦਰਾਜ_cmp(ਅਚਲ ਖਾਲੀ *hashmap_cmp_fn_ਅੰਕੜੇ ਅਣਵਰਤਿਆ,
				  ਅਚਲ ਬਣਤਰ hashmap_ਇੰਦਰਾਜ *eptr,
				  ਅਚਲ ਬਣਤਰ hashmap_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ_or_ਕੁੰਜੀ,
				  ਅਚਲ ਖਾਲੀ *keydata)
{
	ਅਚਲ ਬਣਤਰ refname_hash_ਇੰਦਰਾਜ *e1, *e2;

	e1 = container_ਦਾ(eptr, ਅਚਲ ਬਣਤਰ refname_hash_ਇੰਦਰਾਜ, ent);
	e2 = container_ਦਾ(ਇੰਦਰਾਜ_or_ਕੁੰਜੀ, ਅਚਲ ਬਣਤਰ refname_hash_ਇੰਦਰਾਜ, ent);
	ਵਾਪਸ strcmp(e1->refname, keydata ? keydata : e2->refname);
}

ਸਥਿਰ ਬਣਤਰ refname_hash_ਇੰਦਰਾਜ *refname_hash_ਜੋੜੋ(ਬਣਤਰ hashmap *map,
						   ਅਚਲ ਅੱਖਰ *refname,
						   ਅਚਲ ਬਣਤਰ object_id *oid)
{
	ਬਣਤਰ refname_hash_ਇੰਦਰਾਜ *ent;
	ਮਾਪ_t len = ਲੜੀ_ਲੰਬਾਈ(refname);

	FLEX_ALLOC_MEM(ent, refname, refname, len);
	hashmap_ਇੰਦਰਾਜ_init(&ent->ent, strhash(refname));
	oidcpy(&ent->oid, oid);
	hashmap_ਜੋੜੋ(map, &ent->ent);
	ਵਾਪਸ ent;
}

ਸਥਿਰ ਪੂਰਨ ਜੋੜੋ_one_refname(ਅਚਲ ਬਣਤਰ reference *ref, ਖਾਲੀ *cbdata)
{
	ਬਣਤਰ hashmap *refname_map = cbdata;

	(ਖਾਲੀ) refname_hash_ਜੋੜੋ(refname_map, ref->name, ref->oid);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ refname_hash_init(ਬਣਤਰ hashmap *map)
{
	hashmap_init(map, refname_hash_ਇੰਦਰਾਜ_cmp, ਨਿਰਾ, 0);
}

ਸਥਿਰ ਪੂਰਨ refname_hash_exists(ਬਣਤਰ hashmap *map, ਅਚਲ ਅੱਖਰ *refname)
{
	ਵਾਪਸ !!hashmap_get_from_hash(map, strhash(refname), refname);
}

ਸਥਿਰ ਖਾਲੀ clear_ਵਸਤੂ(ਬਣਤਰ refname_hash_ਇੰਦਰਾਜ *ਵਸਤੂ)
{
	ਵਸਤੂ->ignore = 1;
}


ਸਥਿਰ ਖਾਲੀ ਜੋੜੋ_already_queued_ਟੈਗ_ਸੂਚੀ(ਅਚਲ ਅੱਖਰ *refname,
				    ਅਚਲ ਬਣਤਰ object_id *old_oid ਅਣਵਰਤਿਆ,
				    ਅਚਲ ਬਣਤਰ object_id *new_oid,
				    ਖਾਲੀ *cb_ਅੰਕੜੇ)
{
	ਬਣਤਰ hashmap *queued_ਟੈਗ_ਸੂਚੀ = cb_ਅੰਕੜੇ;
	ਜੇ (starts_with(refname, "refs/tags/") && new_oid)
		(ਖਾਲੀ) refname_hash_ਜੋੜੋ(queued_ਟੈਗ_ਸੂਚੀ, refname, new_oid);
}

ਸਥਿਰ ਖਾਲੀ ਲੱਭੋ_non_local_ਟੈਗ_ਸੂਚੀ(ਅਚਲ ਬਣਤਰ ref *refs,
				ਬਣਤਰ ref_transaction *transaction,
				ਬਣਤਰ ref **ਸਿਰ,
				ਬਣਤਰ ref ***ਪੂਛ)
{
	ਬਣਤਰ hashmap existing_refs;
	ਬਣਤਰ hashmap remote_refs;
	ਬਣਤਰ oidset fetch_oids = OIDSET_INIT;
	ਬਣਤਰ string_list remote_refs_list = STRING_LIST_INIT_NODUP;
	ਬਣਤਰ string_list_ਵਸਤੂ *remote_ref_ਵਸਤੂ;
	ਅਚਲ ਬਣਤਰ ref *ref;
	ਬਣਤਰ refname_hash_ਇੰਦਰਾਜ *ਵਸਤੂ = ਨਿਰਾ;

	refname_hash_init(&existing_refs);
	refname_hash_init(&remote_refs);
	create_fetch_oidset(ਸਿਰ, &fetch_oids);

	refs_for_each_ref(get_main_ref_store(the_repository), ਜੋੜੋ_one_refname,
			  &existing_refs);

	/*
	 * If we already have a transaction, then we need to filter out all
	 * tags which have already been queued up.
	 */
	ਜੇ (transaction)
		ref_transaction_for_each_queued_update(transaction,
						       ਜੋੜੋ_already_queued_ਟੈਗ_ਸੂਚੀ,
						       &existing_refs);

	ਲਈ (ref = refs; ref; ref = ref->ਅਗਲਾ) {
		ਜੇ (!starts_with(ref->name, "refs/tags/"))
			ਜਾਰੀ;

		/*
		 * The peeled ref always follows the matching base
		 * ref, so if we see a peeled ref that we don't want
		 * to fetch then we can mark the ref entry in the list
		 * as one to ignore by setting util to NULL.
		 */
		ਜੇ (ਸਿਰੇ_with(ref->name, "^{}")) {
			ਜੇ (ਵਸਤੂ &&
			    !odb_has_object(the_repository->objects, &ref->old_oid, 0) &&
			    !oidset_ਸ਼ਾਮਲ_ਹੈ(&fetch_oids, &ref->old_oid) &&
			    !odb_has_object(the_repository->objects, &ਵਸਤੂ->oid, 0) &&
			    !oidset_ਸ਼ਾਮਲ_ਹੈ(&fetch_oids, &ਵਸਤੂ->oid))
				clear_ਵਸਤੂ(ਵਸਤੂ);
			ਵਸਤੂ = ਨਿਰਾ;
			ਜਾਰੀ;
		}

		/*
		 * If item is non-NULL here, then we previously saw a
		 * ref not followed by a peeled reference, so we need
		 * to check if it is a lightweight tag that we want to
		 * fetch.
		 */
		ਜੇ (ਵਸਤੂ &&
		    !odb_has_object(the_repository->objects, &ਵਸਤੂ->oid, 0) &&
		    !oidset_ਸ਼ਾਮਲ_ਹੈ(&fetch_oids, &ਵਸਤੂ->oid))
			clear_ਵਸਤੂ(ਵਸਤੂ);

		ਵਸਤੂ = ਨਿਰਾ;

		/* skip duplicates and refs that we already have */
		ਜੇ (refname_hash_exists(&remote_refs, ref->name) ||
		    refname_hash_exists(&existing_refs, ref->name))
			ਜਾਰੀ;

		ਵਸਤੂ = refname_hash_ਜੋੜੋ(&remote_refs, ref->name, &ref->old_oid);
		string_list_ਪਾਓ(&remote_refs_list, ref->name);
	}
	hashmap_clear_and_free(&existing_refs, ਬਣਤਰ refname_hash_ਇੰਦਰਾਜ, ent);

	/*
	 * We may have a final lightweight tag that needs to be
	 * checked to see if it needs fetching.
	 */
	ਜੇ (ਵਸਤੂ &&
	    !odb_has_object(the_repository->objects, &ਵਸਤੂ->oid, 0) &&
	    !oidset_ਸ਼ਾਮਲ_ਹੈ(&fetch_oids, &ਵਸਤੂ->oid))
		clear_ਵਸਤੂ(ਵਸਤੂ);

	/*
	 * For all the tags in the remote_refs_list,
	 * add them to the list of refs to be fetched
	 */
	for_each_string_list_ਵਸਤੂ(remote_ref_ਵਸਤੂ, &remote_refs_list) {
		ਅਚਲ ਅੱਖਰ *refname = remote_ref_ਵਸਤੂ->string;
		ਬਣਤਰ ref *rm;
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ hash = strhash(refname);

		ਵਸਤੂ = hashmap_get_ਇੰਦਰਾਜ_from_hash(&remote_refs, hash, refname,
					ਬਣਤਰ refname_hash_ਇੰਦਰਾਜ, ent);
		ਜੇ (!ਵਸਤੂ)
			BUG("unseen remote ref?");

		/* Unless we have already decided to ignore this item... */
		ਜੇ (ਵਸਤੂ->ignore)
			ਜਾਰੀ;

		rm = alloc_ref(ਵਸਤੂ->refname);
		rm->peer_ref = alloc_ref(ਵਸਤੂ->refname);
		oidcpy(&rm->old_oid, &ਵਸਤੂ->oid);
		**ਪੂਛ = rm;
		*ਪੂਛ = &rm->ਅਗਲਾ;
	}
	hashmap_clear_and_free(&remote_refs, ਬਣਤਰ refname_hash_ਇੰਦਰਾਜ, ent);
	string_list_clear(&remote_refs_list, 0);
	oidset_clear(&fetch_oids);
}

ਸਥਿਰ ਖਾਲੀ filter_prefetch_refspec(ਬਣਤਰ refspec *rs)
{
	ਪੂਰਨ i;

	ਜੇ (!prefetch)
		ਵਾਪਸ;

	ਲਈ (i = 0; i < rs->nr; i++) {
		ਬਣਤਰ strbuf new_dst = STRBUF_INIT;
		ਅੱਖਰ *old_dst;
		ਅਚਲ ਅੱਖਰ *sub = ਨਿਰਾ;

		ਜੇ (rs->ਚੀਜ਼ਾਂ[i].negative)
			ਜਾਰੀ;
		ਜੇ (!rs->ਚੀਜ਼ਾਂ[i].dst ||
		    (rs->ਚੀਜ਼ਾਂ[i].src &&
		     starts_with(rs->ਚੀਜ਼ਾਂ[i].src,
				 ref_namespace[NAMESPACE_ਟੈਗ_ਸੂਚੀ].ref))) {
			ਪੂਰਨ j;

			refspec_ਵਸਤੂ_clear(&rs->ਚੀਜ਼ਾਂ[i]);

			ਲਈ (j = i + 1; j < rs->nr; j++)
				rs->ਚੀਜ਼ਾਂ[j - 1] = rs->ਚੀਜ਼ਾਂ[j];
			rs->nr--;
			i--;
			ਜਾਰੀ;
		}

		old_dst = rs->ਚੀਜ਼ਾਂ[i].dst;
		strbuf_addstr(&new_dst, ref_namespace[NAMESPACE_PREFETCH].ref);

		/*
		 * If old_dst starts with "refs/", then place
		 * sub after that prefix. Otherwise, start at
		 * the beginning of the string.
		 */
		ਜੇ (!skip_ਅਗੇਤਰ(old_dst, "refs/", &sub))
			sub = old_dst;
		strbuf_addstr(&new_dst, sub);

		rs->ਚੀਜ਼ਾਂ[i].dst = strbuf_detach(&new_dst, ਨਿਰਾ);
		rs->ਚੀਜ਼ਾਂ[i].force = 1;

		ਮੁਕਤ(old_dst);
	}
}

ਸਥਿਰ ਬਣਤਰ ref *get_ref_map(ਬਣਤਰ remote *remote,
			       ਅਚਲ ਬਣਤਰ ref *remote_refs,
			       ਬਣਤਰ refspec *rs,
			       ਪੂਰਨ ਟੈਗ_ਸੂਚੀ, ਪੂਰਨ *autotags)
{
	ਪੂਰਨ i;
	ਬਣਤਰ ref *rm;
	ਬਣਤਰ ref *ref_map = ਨਿਰਾ;
	ਬਣਤਰ ref **ਪੂਛ = &ref_map;

	/* opportunistically-updated references: */
	ਬਣਤਰ ref *orefs = ਨਿਰਾ, **oref_ਪੂਛ = &orefs;

	ਬਣਤਰ hashmap existing_refs;
	ਪੂਰਨ existing_refs_populated = 0;

	filter_prefetch_refspec(rs);
	ਜੇ (remote)
		filter_prefetch_refspec(&remote->fetch);

	ਜੇ (rs->nr) {
		ਬਣਤਰ refspec *fetch_refspec;

		ਲਈ (i = 0; i < rs->nr; i++) {
			get_fetch_map(remote_refs, &rs->ਚੀਜ਼ਾਂ[i], &ਪੂਛ, 0);
			ਜੇ (rs->ਚੀਜ਼ਾਂ[i].dst && rs->ਚੀਜ਼ਾਂ[i].dst[0])
				*autotags = 1;
		}
		/* Merge everything on the command line (but not --tags) */
		ਲਈ (rm = ref_map; rm; rm = rm->ਅਗਲਾ)
			rm->fetch_ਸਿਰ_ਸਥਿਤੀ = FETCH_ਸਿਰ_ਮਿਲਾਓ;

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
		ਜੇ (refmap.nr)
			fetch_refspec = &refmap;
		ਨਹੀਂਤਾਂ
			fetch_refspec = &remote->fetch;

		ਲਈ (i = 0; i < fetch_refspec->nr; i++)
			get_fetch_map(ref_map, &fetch_refspec->ਚੀਜ਼ਾਂ[i], &oref_ਪੂਛ, 1);
	} ਨਹੀਂਤਾਂ ਜੇ (refmap.nr) {
		die("--refmap option is only meaningful with command-line refspec(s)");
	} ਨਹੀਂਤਾਂ {
		/* Use the defaults */
		ਬਣਤਰ branch *branch = branch_get(ਨਿਰਾ);
		ਪੂਰਨ has_ਮਿਲਾਓ = branch_has_ਮਿਲਾਓ_config(branch);
		ਜੇ (remote &&
		    (remote->fetch.nr ||
		     /* Note: has_merge implies non-NULL branch->remote_name */
		     (has_ਮਿਲਾਓ && !strcmp(branch->remote_name, remote->name)))) {
			ਲਈ (i = 0; i < remote->fetch.nr; i++) {
				get_fetch_map(remote_refs, &remote->fetch.ਚੀਜ਼ਾਂ[i], &ਪੂਛ, 0);
				ਜੇ (remote->fetch.ਚੀਜ਼ਾਂ[i].dst &&
				    remote->fetch.ਚੀਜ਼ਾਂ[i].dst[0])
					*autotags = 1;
				ਜੇ (!i && !has_ਮਿਲਾਓ && ref_map &&
				    !remote->fetch.ਚੀਜ਼ਾਂ[0].ਨਮੂਨਾ)
					ref_map->fetch_ਸਿਰ_ਸਥਿਤੀ = FETCH_ਸਿਰ_ਮਿਲਾਓ;
			}
			/*
			 * if the remote we're fetching from is the same
			 * as given in branch.<name>.remote, we add the
			 * ref given in branch.<name>.merge, too.
			 *
			 * Note: has_merge implies non-NULL branch->remote_name
			 */
			ਜੇ (has_ਮਿਲਾਓ &&
			    !strcmp(branch->remote_name, remote->name))
				ਜੋੜੋ_ਮਿਲਾਓ_config(&ref_map, remote_refs, branch, &ਪੂਛ);
		} ਨਹੀਂਤਾਂ ਜੇ (!prefetch) {
			ref_map = get_remote_ref(remote_refs, "HEAD");
			ਜੇ (!ref_map)
				die(_("couldn't find remote ref HEAD"));
			ref_map->fetch_ਸਿਰ_ਸਥਿਤੀ = FETCH_ਸਿਰ_ਮਿਲਾਓ;
			ਪੂਛ = &ref_map->ਅਗਲਾ;
		}
	}

	ਜੇ (ਟੈਗ_ਸੂਚੀ == ਟੈਗ_ਸੂਚੀ_ਸੈੱਟ) {
		ਬਣਤਰ refspec_ਵਸਤੂ ਟੈਗ_refspec;

		/* also fetch all tags */
		refspec_ਵਸਤੂ_init_ਧੱਕੋ(&ਟੈਗ_refspec, ਟੈਗ_REFSPEC,
				       the_hash_algo);
		get_fetch_map(remote_refs, &ਟੈਗ_refspec, &ਪੂਛ, 0);
		refspec_ਵਸਤੂ_clear(&ਟੈਗ_refspec);
	} ਨਹੀਂਤਾਂ ਜੇ (ਟੈਗ_ਸੂਚੀ == ਟੈਗ_ਸੂਚੀ_DEFAULT && *autotags) {
		ਲੱਭੋ_non_local_ਟੈਗ_ਸੂਚੀ(remote_refs, ਨਿਰਾ, &ref_map, &ਪੂਛ);
	}

	/* Now append any refs to be updated opportunistically: */
	*ਪੂਛ = orefs;
	ਲਈ (rm = orefs; rm; rm = rm->ਅਗਲਾ) {
		rm->fetch_ਸਿਰ_ਸਥਿਤੀ = FETCH_ਸਿਰ_IGNORE;
		ਪੂਛ = &rm->ਅਗਲਾ;
	}

	/*
	 * apply negative refspecs first, before we remove duplicates. This is
	 * necessary as negative refspecs might remove an otherwise conflicting
	 * duplicate.
	 */
	ਜੇ (rs->nr)
		ref_map = apply_negative_refspecs(ref_map, rs);
	ਨਹੀਂਤਾਂ
		ref_map = apply_negative_refspecs(ref_map, &remote->fetch);

	ref_map = ref_remove_duplicates(ref_map);

	ਲਈ (rm = ref_map; rm; rm = rm->ਅਗਲਾ) {
		ਜੇ (rm->peer_ref) {
			ਅਚਲ ਅੱਖਰ *refname = rm->peer_ref->name;
			ਬਣਤਰ refname_hash_ਇੰਦਰਾਜ *peer_ਵਸਤੂ;
			ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ hash = strhash(refname);

			ਜੇ (!existing_refs_populated) {
				refname_hash_init(&existing_refs);
				refs_for_each_ref(get_main_ref_store(the_repository),
						  ਜੋੜੋ_one_refname,
						  &existing_refs);
				existing_refs_populated = 1;
			}

			peer_ਵਸਤੂ = hashmap_get_ਇੰਦਰਾਜ_from_hash(&existing_refs,
						hash, refname,
						ਬਣਤਰ refname_hash_ਇੰਦਰਾਜ, ent);
			ਜੇ (peer_ਵਸਤੂ) {
				ਬਣਤਰ object_id *old_oid = &peer_ਵਸਤੂ->oid;
				oidcpy(&rm->peer_ref->old_oid, old_oid);
			}
		}
	}
	ਜੇ (existing_refs_populated)
		hashmap_clear_and_free(&existing_refs, ਬਣਤਰ refname_hash_ਇੰਦਰਾਜ, ent);

	ਵਾਪਸ ref_map;
}

ਸਥਿਰ ਪੂਰਨ s_update_ref(ਅਚਲ ਅੱਖਰ *action,
			ਬਣਤਰ ref *ref,
			ਬਣਤਰ ref_transaction *transaction,
			ਪੂਰਨ check_old)
{
	ਅੱਖਰ *ਸੁਨੇਹਾ;
	ਅੱਖਰ *rla = getenv("GIT_REFLOG_ACTION");
	ਬਣਤਰ strbuf err = STRBUF_INIT;
	ਪੂਰਨ ret;

	ਜੇ (dry_ਲੜੀਵਾਰ)
		ਵਾਪਸ 0;
	ਜੇ (!rla)
		rla = default_rla.buf;
	ਸੁਨੇਹਾ = xstrfmt("%s: %s", rla, action);

	ret = ref_transaction_update(transaction, ref->name, &ref->new_oid,
				     check_old ? &ref->old_oid : ਨਿਰਾ,
				     ਨਿਰਾ, ਨਿਰਾ, 0, ਸੁਨੇਹਾ, &err);

	ਜੇ (ret)
		error("%s", err.buf);
	strbuf_release(&err);
	ਮੁਕਤ(ਸੁਨੇਹਾ);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ refcol_ਚੌੜਾਈ(ਅਚਲ ਬਣਤਰ ref *ref_map, ਪੂਰਨ compact_format)
{
	ਅਚਲ ਬਣਤਰ ref *ref;
	ਪੂਰਨ max, ਚੌੜਾਈ = 10;

	max = ਪਦ_columns();
	ਜੇ (compact_format)
		max = max * 2 / 3;

	ਲਈ (ref = ref_map; ref; ref = ref->ਅਗਲਾ) {
		ਪੂਰਨ rlen, llen = 0, len;

		ਜੇ (ref->ਸਥਿਤੀ == REF_ਸਥਿਤੀ_REJECT_SHALLOW ||
		    !ref->peer_ref ||
		    !strcmp(ref->name, "HEAD"))
			ਜਾਰੀ;

		/* uptodate lines are only shown on high verbosity level */
		ਜੇ (verbosity <= 0 && oideq(&ref->peer_ref->old_oid, &ref->old_oid))
			ਜਾਰੀ;

		rlen = utf8_strwidth(prettify_refname(ref->name));
		ਜੇ (!compact_format)
			llen = utf8_strwidth(prettify_refname(ref->peer_ref->name));

		/*
		 * rough estimation to see if the output line is too long and
		 * should not be counted (we can't do precise calculation
		 * anyway because we don't know if the error explanation part
		 * will be printed in update_local_ref)
		 */
		len = 21 /* flag and summary */ + rlen + 4 /* -> */ + llen;
		ਜੇ (len >= max)
			ਜਾਰੀ;

		ਜੇ (ਚੌੜਾਈ < rlen)
			ਚੌੜਾਈ = rlen;
	}

	ਵਾਪਸ ਚੌੜਾਈ;
}

ਸਥਿਰ ਖਾਲੀ display_state_init(ਬਣਤਰ display_state *display_state, ਬਣਤਰ ref *ref_map,
			       ਅਚਲ ਅੱਖਰ *raw_url, ਗਿਣਤੀ display_format format)
{
	ਪੂਰਨ i;

	memset(display_state, 0, ਆਕਾਰ(*display_state));
	strbuf_init(&display_state->buf, 0);
	display_state->format = format;

	ਜੇ (raw_url)
		display_state->url = transport_anonymize_url(raw_url);
	ਨਹੀਂਤਾਂ
		display_state->url = xstrdup("foreign");

	display_state->url_len = ਲੜੀ_ਲੰਬਾਈ(display_state->url);
	ਲਈ (i = display_state->url_len - 1; 0 <= i && display_state->url[i] == '/'; i--)
		;
	display_state->url_len = i + 1;
	ਜੇ (4 < i && !ਲੜੀ_ਤੁਲਨਾ(".git", display_state->url + i - 3, 4))
		display_state->url_len = i - 3;

	ਜੇ (verbosity < 0)
		ਵਾਪਸ;

	ਚੋਣ (display_state->format) {
	ਹਾਲਤ DISPLAY_FORMAT_ਭਰਿਆ:
	ਹਾਲਤ DISPLAY_FORMAT_COMPACT:
		display_state->refcol_ਚੌੜਾਈ = refcol_ਚੌੜਾਈ(ref_map,
							   display_state->format == DISPLAY_FORMAT_COMPACT);
		ਰੋਕੋ;
	ਹਾਲਤ DISPLAY_FORMAT_PORCELAIN:
		/* We don't need to precompute anything here. */
		ਰੋਕੋ;
	ਮੂਲ:
		BUG("unexpected display format %d", display_state->format);
	}
}

ਸਥਿਰ ਖਾਲੀ display_state_release(ਬਣਤਰ display_state *display_state)
{
	strbuf_release(&display_state->buf);
	ਮੁਕਤ(display_state->url);
}

ਸਥਿਰ ਖਾਲੀ print_remote_ਮੰਜ਼ਿਲ_local(ਬਣਤਰ display_state *display_state,
				  ਅਚਲ ਅੱਖਰ *remote, ਅਚਲ ਅੱਖਰ *local)
{
	strbuf_addf(&display_state->buf, "%-*s -> %s",
		    display_state->refcol_ਚੌੜਾਈ, remote, local);
}

ਸਥਿਰ ਪੂਰਨ ਲੱਭੋ_and_replace(ਬਣਤਰ strbuf *haystack,
			    ਅਚਲ ਅੱਖਰ *needle,
			    ਅਚਲ ਅੱਖਰ *placeholder)
{
	ਅਚਲ ਅੱਖਰ *p = ਨਿਰਾ;
	ਪੂਰਨ plen, nlen;

	nlen = ਲੜੀ_ਲੰਬਾਈ(needle);
	ਜੇ (ਸਿਰੇ_with(haystack->buf, needle))
		p = haystack->buf + haystack->len - nlen;
	ਨਹੀਂਤਾਂ
		p = ਲੜੀ_ਖੋਜ(haystack->buf, needle);
	ਜੇ (!p)
		ਵਾਪਸ 0;

	ਜੇ (p > haystack->buf && p[-1] != '/')
		ਵਾਪਸ 0;

	plen = ਲੜੀ_ਲੰਬਾਈ(p);
	ਜੇ (plen > nlen && p[nlen] != '/')
		ਵਾਪਸ 0;

	strbuf_splice(haystack, p - haystack->buf, nlen,
		      placeholder, ਲੜੀ_ਲੰਬਾਈ(placeholder));
	ਵਾਪਸ 1;
}

ਸਥਿਰ ਖਾਲੀ print_compact(ਬਣਤਰ display_state *display_state,
			  ਅਚਲ ਅੱਖਰ *remote, ਅਚਲ ਅੱਖਰ *local)
{
	ਬਣਤਰ strbuf r = STRBUF_INIT;
	ਬਣਤਰ strbuf l = STRBUF_INIT;

	ਜੇ (!strcmp(remote, local)) {
		strbuf_addf(&display_state->buf, "%-*s -> *", display_state->refcol_ਚੌੜਾਈ, remote);
		ਵਾਪਸ;
	}

	strbuf_addstr(&r, remote);
	strbuf_addstr(&l, local);

	ਜੇ (!ਲੱਭੋ_and_replace(&r, local, "*"))
		ਲੱਭੋ_and_replace(&l, remote, "*");
	print_remote_ਮੰਜ਼ਿਲ_local(display_state, r.buf, l.buf);

	strbuf_release(&r);
	strbuf_release(&l);
}

ਸਥਿਰ ਖਾਲੀ display_ref_update(ਬਣਤਰ display_state *display_state, ਅੱਖਰ ਕੋਡ,
			       ਅਚਲ ਅੱਖਰ *summary, ਅਚਲ ਅੱਖਰ *error,
			       ਅਚਲ ਅੱਖਰ *remote, ਅਚਲ ਅੱਖਰ *local,
			       ਅਚਲ ਬਣਤਰ object_id *old_oid,
			       ਅਚਲ ਬਣਤਰ object_id *new_oid,
			       ਪੂਰਨ summary_ਚੌੜਾਈ)
{
	FILE *f = stderr;

	ਜੇ (verbosity < 0)
		ਵਾਪਸ;

	strbuf_reset(&display_state->buf);

	ਚੋਣ (display_state->format) {
	ਹਾਲਤ DISPLAY_FORMAT_ਭਰਿਆ:
	ਹਾਲਤ DISPLAY_FORMAT_COMPACT: {
		ਪੂਰਨ ਚੌੜਾਈ;

		ਜੇ (!display_state->shown_url) {
			strbuf_addf(&display_state->buf, _("From %.*s\n"),
				    display_state->url_len, display_state->url);
			display_state->shown_url = 1;
		}

		ਚੌੜਾਈ = (summary_ਚੌੜਾਈ + ਲੜੀ_ਲੰਬਾਈ(summary) - gettext_ਚੌੜਾਈ(summary));
		remote = prettify_refname(remote);
		local = prettify_refname(local);

		strbuf_addf(&display_state->buf, " %c %-*s ", ਕੋਡ, ਚੌੜਾਈ, summary);

		ਜੇ (display_state->format != DISPLAY_FORMAT_COMPACT)
			print_remote_ਮੰਜ਼ਿਲ_local(display_state, remote, local);
		ਨਹੀਂਤਾਂ
			print_compact(display_state, remote, local);

		ਜੇ (error)
			strbuf_addf(&display_state->buf, "  (%s)", error);

		ਰੋਕੋ;
	}
	ਹਾਲਤ DISPLAY_FORMAT_PORCELAIN:
		strbuf_addf(&display_state->buf, "%c %s %s %s", ਕੋਡ,
			    oid_ਮੰਜ਼ਿਲ_hex(old_oid), oid_ਮੰਜ਼ਿਲ_hex(new_oid), local);
		f = stdout;
		ਰੋਕੋ;
	ਮੂਲ:
		BUG("unexpected display format %d", display_state->format);
	};
	strbuf_addch(&display_state->buf, '\n');

	fputs(display_state->buf.buf, f);
}

ਬਣਤਰ ref_update_display_info {
	bool failed;
	ਅੱਖਰ success_ਕੋਡ;
	ਅੱਖਰ fail_ਕੋਡ;
	ਅੱਖਰ *summary;
	ਅੱਖਰ *fail_detail;
	ਅੱਖਰ *success_detail;
	ਅੱਖਰ *ref;
	ਅੱਖਰ *remote;
	ਬਣਤਰ object_id old_oid;
	ਬਣਤਰ object_id new_oid;
};

ਬਣਤਰ ref_update_display_info_array {
	ਬਣਤਰ ref_update_display_info *info;
	ਮਾਪ_t alloc, nr;
};

ਸਥਿਰ ਬਣਤਰ ref_update_display_info *ref_update_display_info_append(
					   ਬਣਤਰ ref_update_display_info_array *array,
					   ਅੱਖਰ success_ਕੋਡ,
					   ਅੱਖਰ fail_ਕੋਡ,
					   ਅਚਲ ਅੱਖਰ *summary,
					   ਅਚਲ ਅੱਖਰ *success_detail,
					   ਅਚਲ ਅੱਖਰ *fail_detail,
					   ਅਚਲ ਅੱਖਰ *ref,
					   ਅਚਲ ਅੱਖਰ *remote,
					   ਅਚਲ ਬਣਤਰ object_id *old_oid,
					   ਅਚਲ ਬਣਤਰ object_id *new_oid)
{
	ਬਣਤਰ ref_update_display_info *info;

	ALLOC_GROW(array->info, array->nr + 1, array->alloc);
	info = &array->info[array->nr++];

	info->failed = false;
	info->success_ਕੋਡ = success_ਕੋਡ;
	info->fail_ਕੋਡ = fail_ਕੋਡ;
	info->summary = xstrdup(summary);
	info->success_detail = xstrdup_or_null(success_detail);
	info->fail_detail = xstrdup_or_null(fail_detail);
	info->remote = xstrdup(remote);
	info->ref = xstrdup(ref);

	oidcpy(&info->old_oid, old_oid);
	oidcpy(&info->new_oid, new_oid);

	ਵਾਪਸ info;
}

ਸਥਿਰ ਖਾਲੀ ref_update_display_info_ਸੈੱਟ_failed(ਬਣਤਰ ref_update_display_info *info)
{
	info->failed = true;
}

ਸਥਿਰ ਖਾਲੀ ref_update_display_info_free(ਬਣਤਰ ref_update_display_info *info)
{
	ਮੁਕਤ(info->summary);
	ਮੁਕਤ(info->success_detail);
	ਮੁਕਤ(info->fail_detail);
	ਮੁਕਤ(info->remote);
	ਮੁਕਤ(info->ref);
}

ਸਥਿਰ ਖਾਲੀ ref_update_display_info_display(ਬਣਤਰ ref_update_display_info *info,
					    ਬਣਤਰ display_state *display_state,
					    ਪੂਰਨ summary_ਚੌੜਾਈ)
{
	display_ref_update(display_state,
			   info->failed ? info->fail_ਕੋਡ : info->success_ਕੋਡ,
			   info->summary,
			   info->failed ? info->fail_detail : info->success_detail,
			   info->remote, info->ref, &info->old_oid,
			   &info->new_oid, summary_ਚੌੜਾਈ);
}

ਸਥਿਰ ਪੂਰਨ update_local_ref(ਬਣਤਰ ref *ref,
			    ਬਣਤਰ ref_transaction *transaction,
			    ਅਚਲ ਬਣਤਰ ref *remote_ref,
			    ਅਚਲ ਬਣਤਰ fetch_config *config,
			    ਬਣਤਰ ref_update_display_info_array *display_array)
{
	ਬਣਤਰ commit *current = ਨਿਰਾ, *updated;
	ਪੂਰਨ fast_forward = 0;

	ਜੇ (!odb_has_object(the_repository->objects, &ref->new_oid,
			    ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR))
		die(_("object %s not found"), oid_ਮੰਜ਼ਿਲ_hex(&ref->new_oid));

	ਜੇ (oideq(&ref->old_oid, &ref->new_oid)) {
		ਜੇ (verbosity > 0)
			ref_update_display_info_append(display_array, '=', '=',
						       _("[up to date]"), ਨਿਰਾ,
						       ਨਿਰਾ, ref->name,
						       remote_ref->name, &ref->old_oid,
						       &ref->new_oid);
		ਵਾਪਸ 0;
	}

	ਜੇ (!update_ਸਿਰ_ok &&
	    !is_null_oid(&ref->old_oid) &&
	    branch_checked_ਬਾਹਰਲਾ(ref->name)) {
		ਬਣਤਰ ref_update_display_info *info;
		/*
		 * If this is the head, and it's not okay to update
		 * the head, and the old value of the head isn't empty...
		 */
		info = ref_update_display_info_append(display_array, '!', '!',
						      _("[rejected]"), ਨਿਰਾ,
						      _("can't fetch into checked-out branch"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);
		ref_update_display_info_ਸੈੱਟ_failed(info);
		ਵਾਪਸ 1;
	}

	ਜੇ (!is_null_oid(&ref->old_oid) &&
	    starts_with(ref->name, "refs/tags/")) {
		ਬਣਤਰ ref_update_display_info *info;

		ਜੇ (force || ref->force) {
			ਪੂਰਨ r;

			r = s_update_ref("updating tag", ref, transaction, 0);

			info = ref_update_display_info_append(display_array, 't', '!',
							      _("[tag update]"), ਨਿਰਾ,
							      _("unable to update local ref"),
							      ref->name, remote_ref->name,
							      &ref->old_oid, &ref->new_oid);
			ਜੇ (r)
				ref_update_display_info_ਸੈੱਟ_failed(info);

			ਵਾਪਸ r;
		} ਨਹੀਂਤਾਂ {
			info = ref_update_display_info_append(display_array, '!', '!',
							      _("[rejected]"), ਨਿਰਾ,
							      _("would clobber existing tag"),
							      ref->name, remote_ref->name,
							      &ref->old_oid, &ref->new_oid);
			ref_update_display_info_ਸੈੱਟ_failed(info);
			ਵਾਪਸ 1;
		}
	}

	current = lookup_commit_reference_gently(the_repository,
						 &ref->old_oid, 1);
	updated = lookup_commit_reference_gently(the_repository,
						 &ref->new_oid, 1);
	ਜੇ (!current || !updated) {
		ਬਣਤਰ ref_update_display_info *info;
		ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ;
		ਅਚਲ ਅੱਖਰ *what;
		ਪੂਰਨ r;
		/*
		 * Nicely describe the new ref we're fetching.
		 * Base this on the remote's ref name, as it's
		 * more likely to follow a standard layout.
		 */
		ਜੇ (starts_with(remote_ref->name, "refs/tags/")) {
			ਸੁਨੇਹਾ = "storing tag";
			what = _("[new tag]");
		} ਨਹੀਂਤਾਂ ਜੇ (starts_with(remote_ref->name, "refs/heads/")) {
			ਸੁਨੇਹਾ = "storing head";
			what = _("[new branch]");
		} ਨਹੀਂਤਾਂ {
			ਸੁਨੇਹਾ = "storing ref";
			what = _("[new ref]");
		}

		r = s_update_ref(ਸੁਨੇਹਾ, ref, transaction, 0);

		info = ref_update_display_info_append(display_array, '*', '!',
						      what, ਨਿਰਾ,
						      _("unable to update local ref"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);
		ਜੇ (r)
			ref_update_display_info_ਸੈੱਟ_failed(info);

		ਵਾਪਸ r;
	}

	ਜੇ (config->show_forced_updates) {
		uint64_t t_before = getnanotime();
		fast_forward = repo_ਵਿੱਚ_ਮਿਲਾਓ_bases(the_repository, current,
						   updated);
		ਜੇ (fast_forward < 0)
			die(ਨਿਰਾ);
		forced_updates_ms += (getnanotime() - t_before) / 1000000;
	} ਨਹੀਂਤਾਂ {
		fast_forward = 1;
	}

	ਜੇ (fast_forward) {
		ਬਣਤਰ ref_update_display_info *info;
		ਬਣਤਰ strbuf quickref = STRBUF_INIT;
		ਪੂਰਨ r;

		strbuf_ਜੋੜੋ_unique_abbrev(&quickref, &current->object.oid, DEFAULT_ABBREV);
		strbuf_addstr(&quickref, "..");
		strbuf_ਜੋੜੋ_unique_abbrev(&quickref, &ref->new_oid, DEFAULT_ABBREV);
		r = s_update_ref("fast-forward", ref, transaction, 1);

		info = ref_update_display_info_append(display_array, ' ', '!',
						      quickref.buf, ਨਿਰਾ,
						      _("unable to update local ref"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);
		ਜੇ (r)
			ref_update_display_info_ਸੈੱਟ_failed(info);

		strbuf_release(&quickref);
		ਵਾਪਸ r;
	} ਨਹੀਂਤਾਂ ਜੇ (force || ref->force) {
		ਬਣਤਰ ref_update_display_info *info;
		ਬਣਤਰ strbuf quickref = STRBUF_INIT;
		ਪੂਰਨ r;

		strbuf_ਜੋੜੋ_unique_abbrev(&quickref, &current->object.oid, DEFAULT_ABBREV);
		strbuf_addstr(&quickref, "...");
		strbuf_ਜੋੜੋ_unique_abbrev(&quickref, &ref->new_oid, DEFAULT_ABBREV);
		r = s_update_ref("forced-update", ref, transaction, 1);

		info = ref_update_display_info_append(display_array, '+', '!',
						      quickref.buf, _("forced update"),
						      _("unable to update local ref"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);

		ਜੇ (r)
			ref_update_display_info_ਸੈੱਟ_failed(info);

		strbuf_release(&quickref);
		ਵਾਪਸ r;
	} ਨਹੀਂਤਾਂ {
		ਬਣਤਰ ref_update_display_info *info;
		info = ref_update_display_info_append(display_array, '!', '!',
						      _("[rejected]"), ਨਿਰਾ,
						      _("non-fast-forward"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);
		ref_update_display_info_ਸੈੱਟ_failed(info);
		ਵਾਪਸ 1;
	}
}

ਸਥਿਰ ਅਚਲ ਬਣਤਰ object_id *iterate_ref_map(ਖਾਲੀ *cb_ਅੰਕੜੇ)
{
	ਬਣਤਰ ref **rm = cb_ਅੰਕੜੇ;
	ਬਣਤਰ ref *ref = *rm;

	ਜਦੋਂ (ref && ref->ਸਥਿਤੀ == REF_ਸਥਿਤੀ_REJECT_SHALLOW)
		ref = ref->ਅਗਲਾ;
	ਜੇ (!ref)
		ਵਾਪਸ ਨਿਰਾ;
	*rm = ref->ਅਗਲਾ;
	ਵਾਪਸ &ref->old_oid;
}

ਬਣਤਰ fetch_ਸਿਰ {
	FILE *fp;
	ਬਣਤਰ strbuf buf;
};

ਸਥਿਰ ਪੂਰਨ open_fetch_ਸਿਰ(ਬਣਤਰ fetch_ਸਿਰ *fetch_ਸਿਰ)
{
	ਅਚਲ ਅੱਖਰ *filename = git_ਰਸਤਾ_fetch_ਸਿਰ(the_repository);

	ਜੇ (write_fetch_ਸਿਰ) {
		fetch_ਸਿਰ->fp = fopen(filename, "a");
		ਜੇ (!fetch_ਸਿਰ->fp)
			ਵਾਪਸ error_errno(_("cannot open '%s'"), filename);
		strbuf_init(&fetch_ਸਿਰ->buf, 0);
	} ਨਹੀਂਤਾਂ {
		fetch_ਸਿਰ->fp = ਨਿਰਾ;
	}

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ append_fetch_ਸਿਰ(ਬਣਤਰ fetch_ਸਿਰ *fetch_ਸਿਰ,
			      ਅਚਲ ਬਣਤਰ object_id *old_oid,
			      ਗਿਣਤੀ fetch_ਸਿਰ_ਸਥਿਤੀ fetch_ਸਿਰ_ਸਥਿਤੀ,
			      ਅਚਲ ਅੱਖਰ *note,
			      ਅਚਲ ਅੱਖਰ *url, ਮਾਪ_t url_len)
{
	ਅੱਖਰ old_oid_hex[GIT_MAX_HEXSZ + 1];
	ਅਚਲ ਅੱਖਰ *ਮਿਲਾਓ_ਸਥਿਤੀ_marker;
	ਮਾਪ_t i;

	ਜੇ (!fetch_ਸਿਰ->fp)
		ਵਾਪਸ;

	ਚੋਣ (fetch_ਸਿਰ_ਸਥਿਤੀ) {
	ਹਾਲਤ FETCH_ਸਿਰ_NOT_FOR_ਮਿਲਾਓ:
		ਮਿਲਾਓ_ਸਥਿਤੀ_marker = "not-for-merge";
		ਰੋਕੋ;
	ਹਾਲਤ FETCH_ਸਿਰ_ਮਿਲਾਓ:
		ਮਿਲਾਓ_ਸਥਿਤੀ_marker = "";
		ਰੋਕੋ;
	ਮੂਲ:
		/* do not write anything to FETCH_HEAD */
		ਵਾਪਸ;
	}

	strbuf_addf(&fetch_ਸਿਰ->buf, "%s\t%s\t%s",
		    oid_ਮੰਜ਼ਿਲ_hex_r(old_oid_hex, old_oid), ਮਿਲਾਓ_ਸਥਿਤੀ_marker, note);
	ਲਈ (i = 0; i < url_len; ++i)
		ਜੇ ('\n' == url[i])
			strbuf_addstr(&fetch_ਸਿਰ->buf, "\\n");
		ਨਹੀਂਤਾਂ
			strbuf_addch(&fetch_ਸਿਰ->buf, url[i]);
	strbuf_addch(&fetch_ਸਿਰ->buf, '\n');

	/*
	 * When using an atomic fetch, we do not want to update FETCH_HEAD if
	 * any of the reference updates fails. We thus have to write all
	 * updates to a buffer first and only commit it as soon as all
	 * references have been successfully updated.
	 */
	ਜੇ (!atomic_fetch) {
		strbuf_write(&fetch_ਸਿਰ->buf, fetch_ਸਿਰ->fp);
		strbuf_reset(&fetch_ਸਿਰ->buf);
	}
}

ਸਥਿਰ ਖਾਲੀ commit_fetch_ਸਿਰ(ਬਣਤਰ fetch_ਸਿਰ *fetch_ਸਿਰ)
{
	ਜੇ (!fetch_ਸਿਰ->fp || !atomic_fetch)
		ਵਾਪਸ;
	strbuf_write(&fetch_ਸਿਰ->buf, fetch_ਸਿਰ->fp);
}

ਸਥਿਰ ਖਾਲੀ close_fetch_ਸਿਰ(ਬਣਤਰ fetch_ਸਿਰ *fetch_ਸਿਰ)
{
	ਜੇ (!fetch_ਸਿਰ->fp)
		ਵਾਪਸ;

	fclose(fetch_ਸਿਰ->fp);
	strbuf_release(&fetch_ਸਿਰ->buf);
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ warn_show_forced_updates[] =
N_("fetch normally indicates which branches had a forced update,\n"
   "but that check has been disabled; to re-enable, use '--show-forced-updates'\n"
   "flag or run 'git config fetch.showForcedUpdates true'");
ਸਥਿਰ ਅਚਲ ਅੱਖਰ warn_ਸਮਾਂ_show_forced_updates[] =
N_("it took %.2f seconds to check forced updates; you can use\n"
   "'--no-show-forced-updates' or run 'git config fetch.showForcedUpdates false'\n"
   "to avoid this check\n");

ਸਥਿਰ ਪੂਰਨ store_updated_refs(ਬਣਤਰ display_state *display_state,
			      ਪੂਰਨ connectivity_checked,
			      ਬਣਤਰ ref_transaction *transaction, ਬਣਤਰ ref *ref_map,
			      ਬਣਤਰ fetch_ਸਿਰ *fetch_ਸਿਰ,
			      ਅਚਲ ਬਣਤਰ fetch_config *config,
			      ਬਣਤਰ ref_update_display_info_array *display_array)
{
	ਪੂਰਨ rc = 0;
	ਬਣਤਰ strbuf note = STRBUF_INIT;
	ਅਚਲ ਅੱਖਰ *what, *ਕਿਸਮ;
	ਬਣਤਰ ref *rm;
	ਪੂਰਨ want_ਸਥਿਤੀ;

	ਜੇ (!connectivity_checked) {
		ਬਣਤਰ check_connected_options opt = CHECK_CONNECTED_INIT;

		opt.exclude_hidden_refs_section = "fetch";
		rm = ref_map;
		ਜੇ (check_connected(iterate_ref_map, &rm, &opt)) {
			rc = error(_("%s did not send all necessary objects"),
				   display_state->url);
			ਜਾਓ abort;
		}
	}

	/*
	 * We do a pass for each fetch_head_status type in their enum order, so
	 * merged entries are written before not-for-merge. That lets readers
	 * use FETCH_HEAD as a refname to refer to the ref to be merged.
	 */
	ਲਈ (want_ਸਥਿਤੀ = FETCH_ਸਿਰ_ਮਿਲਾਓ;
	     want_ਸਥਿਤੀ <= FETCH_ਸਿਰ_IGNORE;
	     want_ਸਥਿਤੀ++) {
		ਲਈ (rm = ref_map; rm; rm = rm->ਅਗਲਾ) {
			ਬਣਤਰ ref *ref = ਨਿਰਾ;

			ਜੇ (rm->ਸਥਿਤੀ == REF_ਸਥਿਤੀ_REJECT_SHALLOW) {
				ਜੇ (want_ਸਥਿਤੀ == FETCH_ਸਿਰ_ਮਿਲਾਓ)
					warning(_("rejected %s because shallow roots are not allowed to be updated"),
						rm->peer_ref ? rm->peer_ref->name : rm->name);
				ਜਾਰੀ;
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
			ਜੇ (fetch_ਸਿਰ->fp) {
				ਬਣਤਰ commit *commit = ਨਿਰਾ;

				/*
				 * References in "refs/tags/" are often going to point
				 * to annotated tags, which are not part of the
				 * commit-graph. We thus only try to look up refs in
				 * the graph which are not in that namespace to not
				 * regress performance in repositories with many
				 * annotated tags.
				 */
				ਜੇ (!starts_with(rm->name, "refs/tags/"))
					commit = lookup_commit_ਵਿੱਚ_ਗ੍ਰਾਫ(the_repository, &rm->old_oid);
				ਜੇ (!commit) {
					commit = lookup_commit_reference_gently(the_repository,
										&rm->old_oid,
										1);
					ਜੇ (!commit)
						rm->fetch_ਸਿਰ_ਸਥਿਤੀ = FETCH_ਸਿਰ_NOT_FOR_ਮਿਲਾਓ;
				}
			}

			ਜੇ (rm->fetch_ਸਿਰ_ਸਥਿਤੀ != want_ਸਥਿਤੀ)
				ਜਾਰੀ;

			ਜੇ (rm->peer_ref) {
				ref = alloc_ref(rm->peer_ref->name);
				oidcpy(&ref->old_oid, &rm->peer_ref->old_oid);
				oidcpy(&ref->new_oid, &rm->old_oid);
				ref->force = rm->peer_ref->force;
			}

			ਜੇ (config->recurse_submodules != RECURSE_SUBMODULES_OFF &&
			    (!rm->peer_ref || !oideq(&ref->old_oid, &ref->new_oid))) {
				check_for_new_submodule_commits(&rm->old_oid);
			}

			ਜੇ (!strcmp(rm->name, "HEAD")) {
				ਕਿਸਮ = "";
				what = "";
			} ਨਹੀਂਤਾਂ ਜੇ (skip_ਅਗੇਤਰ(rm->name, "refs/heads/", &what)) {
				ਕਿਸਮ = "branch";
			} ਨਹੀਂਤਾਂ ਜੇ (skip_ਅਗੇਤਰ(rm->name, "refs/tags/", &what)) {
				ਕਿਸਮ = "tag";
			} ਨਹੀਂਤਾਂ ਜੇ (skip_ਅਗੇਤਰ(rm->name, "refs/remotes/", &what)) {
				ਕਿਸਮ = "remote-tracking branch";
			} ਨਹੀਂਤਾਂ {
				ਕਿਸਮ = "";
				what = rm->name;
			}

			strbuf_reset(&note);
			ਜੇ (*what) {
				ਜੇ (*ਕਿਸਮ)
					strbuf_addf(&note, "%s ", ਕਿਸਮ);
				strbuf_addf(&note, "'%s' of ", what);
			}

			append_fetch_ਸਿਰ(fetch_ਸਿਰ, &rm->old_oid,
					  rm->fetch_ਸਿਰ_ਸਥਿਤੀ,
					  note.buf, display_state->url,
					  display_state->url_len);

			ਜੇ (ref) {
				rc |= update_local_ref(ref, transaction, rm,
						       config, display_array);
				ਮੁਕਤ(ref);
			} ਨਹੀਂਤਾਂ ਜੇ (write_fetch_ਸਿਰ || dry_ਲੜੀਵਾਰ) {
				/*
				 * Display fetches written to FETCH_HEAD (or
				 * would be written to FETCH_HEAD, if --dry-run
				 * is set).
				 */

				ref_update_display_info_append(display_array, '*', '*',
							       *ਕਿਸਮ ? ਕਿਸਮ : "branch",
							       ਨਿਰਾ, ਨਿਰਾ, "FETCH_HEAD",
							       rm->name, &rm->new_oid,
							       &rm->old_oid);
			}
		}
	}

	ਜੇ (advice_enabled(ADVICE_FETCH_SHOW_FORCED_UPDATES)) {
		ਜੇ (!config->show_forced_updates) {
			warning(_(warn_show_forced_updates));
		} ਨਹੀਂਤਾਂ ਜੇ (forced_updates_ms > FORCED_UPDATES_DELAY_WARNING_ਵਿੱਚ_MS) {
			warning(_(warn_ਸਮਾਂ_show_forced_updates),
				forced_updates_ms / 1000.0);
		}
	}

 abort:
	strbuf_release(&note);
	ਵਾਪਸ rc;
}

/*
 * We would want to bypass the object transfer altogether if
 * everything we are going to fetch already exists and is connected
 * locally.
 */
ਸਥਿਰ ਪੂਰਨ check_exist_and_connected(ਬਣਤਰ ref *ref_map)
{
	ਬਣਤਰ ref *rm = ref_map;
	ਬਣਤਰ check_connected_options opt = CHECK_CONNECTED_INIT;
	ਬਣਤਰ ref *r;

	/*
	 * If we are deepening a shallow clone we already have these
	 * objects reachable.  Running rev-list here will return with
	 * a good (0) exit status and we'll bypass the fetch that we
	 * really need to perform.  Claiming failure now will ensure
	 * we perform the network exchange to deepen our history.
	 */
	ਜੇ (deepen)
		ਵਾਪਸ -1;

	/*
	 * Similarly, if we need to refetch, we always want to perform a full
	 * fetch ignoring existing objects.
	 */
	ਜੇ (refetch)
		ਵਾਪਸ -1;


	/*
	 * check_connected() allows objects to merely be promised, but
	 * we need all direct targets to exist.
	 */
	ਲਈ (r = rm; r; r = r->ਅਗਲਾ) {
		ਜੇ (!odb_has_object(the_repository->objects, &r->old_oid,
				    ODB_HAS_OBJECT_RECHECK_PACKED))
			ਵਾਪਸ -1;
	}

	opt.quiet = 1;
	opt.exclude_hidden_refs_section = "fetch";
	ਵਾਪਸ check_connected(iterate_ref_map, &rm, &opt);
}

ਸਥਿਰ ਪੂਰਨ fetch_and_consume_refs(ਬਣਤਰ display_state *display_state,
				  ਬਣਤਰ transport *transport,
				  ਬਣਤਰ ref_transaction *transaction,
				  ਬਣਤਰ ref *ref_map,
				  ਬਣਤਰ fetch_ਸਿਰ *fetch_ਸਿਰ,
				  ਅਚਲ ਬਣਤਰ fetch_config *config,
				  ਬਣਤਰ ref_update_display_info_array *display_array)
{
	ਪੂਰਨ connectivity_checked = 1;
	ਪੂਰਨ ret;

	/*
	 * We don't need to perform a fetch in case we can already satisfy all
	 * refs.
	 */
	ret = check_exist_and_connected(ref_map);
	ਜੇ (ret) {
		trace2_region_enter("fetch", "fetch_refs", the_repository);
		ret = transport_fetch_refs(transport, ref_map);
		trace2_region_leave("fetch", "fetch_refs", the_repository);
		ਜੇ (ret)
			ਜਾਓ ਬਾਹਰਲਾ;
		connectivity_checked = transport->smart_options ?
			transport->smart_options->connectivity_checked : 0;
	}

	trace2_region_enter("fetch", "consume_refs", the_repository);
	ret = store_updated_refs(display_state, connectivity_checked,
				 transaction, ref_map, fetch_ਸਿਰ, config,
				 display_array);
	trace2_region_leave("fetch", "consume_refs", the_repository);

ਬਾਹਰਲਾ:
	transport_unlock_pack(transport, 0);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ prune_refs(ਬਣਤਰ display_state *display_state,
		      ਬਣਤਰ refspec *rs,
		      ਬਣਤਰ ref_transaction *transaction,
		      ਬਣਤਰ ref *ref_map)
{
	ਪੂਰਨ ਨਤੀਜਾ = 0;
	ਬਣਤਰ ref *ref, *stale_refs = get_stale_heads(rs, ref_map);
	ਬਣਤਰ strbuf err = STRBUF_INIT;
	ਬਣਤਰ string_list refnames = STRING_LIST_INIT_NODUP;

	ਲਈ (ref = stale_refs; ref; ref = ref->ਅਗਲਾ)
		string_list_append(&refnames, ref->name);

	ਜੇ (!dry_ਲੜੀਵਾਰ) {
		ਜੇ (transaction) {
			ਲਈ (ref = stale_refs; ref; ref = ref->ਅਗਲਾ) {
				ਨਤੀਜਾ = ref_transaction_delete(transaction, ref->name, ਨਿਰਾ,
								ਨਿਰਾ, 0, "fetch: prune", &err);
				ਜੇ (ਨਤੀਜਾ)
					ਜਾਓ cleanup;
			}
		} ਨਹੀਂਤਾਂ {
			ਨਤੀਜਾ = refs_delete_refs(get_main_ref_store(the_repository),
						  "fetch: prune", &refnames,
						  0);
		}
	}

	ਜੇ (verbosity >= 0) {
		ਪੂਰਨ summary_ਚੌੜਾਈ = transport_summary_ਚੌੜਾਈ(stale_refs);

		ਲਈ (ref = stale_refs; ref; ref = ref->ਅਗਲਾ) {
			display_ref_update(display_state, '-', _("[deleted]"), ਨਿਰਾ,
					   _("(none)"), ref->name,
					   &ref->new_oid, &ref->old_oid,
					   summary_ਚੌੜਾਈ);
		}
		string_list_ਛਾਂਟੋ(&refnames);
		refs_warn_dangling_symrefs(get_main_ref_store(the_repository),
					   stderr, "   ", dry_ਲੜੀਵਾਰ, &refnames);
	}

cleanup:
	string_list_clear(&refnames, 0);
	strbuf_release(&err);
	free_refs(stale_refs);
	ਵਾਪਸ ਨਤੀਜਾ;
}

ਸਥਿਰ ਖਾਲੀ check_not_current_branch(ਬਣਤਰ ref *ref_map)
{
	ਅਚਲ ਅੱਖਰ *ਰਸਤਾ;
	ਲਈ (; ref_map; ref_map = ref_map->ਅਗਲਾ)
		ਜੇ (ref_map->peer_ref &&
		    starts_with(ref_map->peer_ref->name, "refs/heads/") &&
		    (ਰਸਤਾ = branch_checked_ਬਾਹਰਲਾ(ref_map->peer_ref->name)))
			die(_("refusing to fetch into branch '%s' "
			      "checked out at '%s'"),
			    ref_map->peer_ref->name, ਰਸਤਾ);
}

ਸਥਿਰ ਪੂਰਨ truncate_fetch_ਸਿਰ(ਖਾਲੀ)
{
	ਅਚਲ ਅੱਖਰ *filename = git_ਰਸਤਾ_fetch_ਸਿਰ(the_repository);
	FILE *fp = fopen_for_writing(filename);

	ਜੇ (!fp)
		ਵਾਪਸ error_errno(_("cannot open '%s'"), filename);
	fclose(fp);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ ਸੈੱਟ_option(ਬਣਤਰ transport *transport, ਅਚਲ ਅੱਖਰ *name, ਅਚਲ ਅੱਖਰ *ਮੁੱਲ)
{
	ਪੂਰਨ r = transport_ਸੈੱਟ_option(transport, name, ਮੁੱਲ);
	ਜੇ (r < 0)
		die(_("option \"%s\" value \"%s\" is not valid for %s"),
		    name, ਮੁੱਲ, transport->url);
	ਜੇ (r > 0)
		warning(_("option \"%s\" is ignored for %s"),
			name, transport->url);
}


ਸਥਿਰ ਪੂਰਨ ਜੋੜੋ_oid(ਅਚਲ ਬਣਤਰ reference *ref, ਖਾਲੀ *cb_ਅੰਕੜੇ)
{
	ਬਣਤਰ oid_array *oids = cb_ਅੰਕੜੇ;

	oid_array_append(oids, ref->oid);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ ਜੋੜੋ_negotiation_tips(ਬਣਤਰ string_list *ਨਿਵੇਸ਼_list,
				 ਬਣਤਰ oid_array **output_list,
				 ਅਚਲ ਅੱਖਰ *argname)
{
	ਬਣਤਰ oid_array *oids = xcalloc(1, ਆਕਾਰ(*oids));
	ਪੂਰਨ i;

	ਲਈ (i = 0; i < ਨਿਵੇਸ਼_list->nr; i++) {
		ਅਚਲ ਅੱਖਰ *s = ਨਿਵੇਸ਼_list->ਚੀਜ਼ਾਂ[i].string;
		ਬਣਤਰ refs_for_each_ref_options opts = {
			.ਨਮੂਨਾ = s,
		};
		ਪੂਰਨ old_nr;
		ਜੇ (!has_glob_specials(s)) {
			ਬਣਤਰ object_id oid;

			/* Ignore missing reference. */
			ਜੇ (repo_get_oid(the_repository, s, &oid))
				ਜਾਰੀ;
			/* Fail on missing object pointed by ref. */
			ਜੇ (!odb_has_object(the_repository->objects, &oid, 0))
				die(_("the object %s does not exist"), s);

			oid_array_append(oids, &oid);
			ਜਾਰੀ;
		}
		old_nr = oids->nr;
		refs_for_each_ref_ext(get_main_ref_store(the_repository),
				      ਜੋੜੋ_oid, oids, &opts);
		ਜੇ (old_nr == oids->nr)
			warning(_("ignoring %s=%s because it does not match any refs"),
				argname, s);
	}
	*output_list = oids;
}

ਸਥਿਰ ਬਣਤਰ transport *prepare_transport(ਬਣਤਰ remote *remote, ਪੂਰਨ deepen,
					   ਬਣਤਰ list_objects_filter_options *filter_options)
{
	ਬਣਤਰ transport *transport;

	transport = transport_get(remote, ਨਿਰਾ);
	transport_ਸੈੱਟ_verbosity(transport, verbosity, ਤਰੱਕੀ);
	transport->family = family;
	ਜੇ (upload_pack)
		ਸੈੱਟ_option(transport, TRANS_OPT_UPLOADPACK, upload_pack);
	ਜੇ (keep)
		ਸੈੱਟ_option(transport, TRANS_OPT_KEEP, "yes");
	ਜੇ (ਡੂੰਘਾਈ)
		ਸੈੱਟ_option(transport, TRANS_OPT_ਡੂੰਘਾਈ, ਡੂੰਘਾਈ);
	ਜੇ (deepen && deepen_since)
		ਸੈੱਟ_option(transport, TRANS_OPT_DEEPEN_SINCE, deepen_since);
	ਜੇ (deepen && deepen_not.nr)
		ਸੈੱਟ_option(transport, TRANS_OPT_DEEPEN_NOT,
			   (ਅਚਲ ਅੱਖਰ *)&deepen_not);
	ਜੇ (deepen_relative)
		ਸੈੱਟ_option(transport, TRANS_OPT_DEEPEN_RELATIVE, "yes");
	ਜੇ (update_shallow)
		ਸੈੱਟ_option(transport, TRANS_OPT_UPDATE_SHALLOW, "yes");
	ਜੇ (refetch)
		ਸੈੱਟ_option(transport, TRANS_OPT_REFETCH, "yes");
	ਜੇ (filter_options->choice) {
		ਅਚਲ ਅੱਖਰ *spec =
			expand_list_objects_filter_spec(filter_options);
		ਸੈੱਟ_option(transport, TRANS_OPT_LIST_OBJECTS_FILTER, spec);
		ਸੈੱਟ_option(transport, TRANS_OPT_FROM_PROMISOR, "1");
	}
	ਜੇ (negotiation_restrict.nr) {
		ਜੇ (transport->smart_options)
			ਜੋੜੋ_negotiation_tips(&negotiation_restrict,
					     &transport->smart_options->negotiation_restrict_tips,
					     "--negotiation-restrict");
		ਨਹੀਂਤਾਂ
			warning(_("ignoring %s because the protocol does not support it"),
				"--negotiation-restrict");
	} ਨਹੀਂਤਾਂ ਜੇ (remote->negotiation_restrict.nr) {
		ਬਣਤਰ string_list_ਵਸਤੂ *ਵਸਤੂ;
		for_each_string_list_ਵਸਤੂ(ਵਸਤੂ, &remote->negotiation_restrict)
			string_list_append(&negotiation_restrict, ਵਸਤੂ->string);
		ਜੇ (transport->smart_options)
			ਜੋੜੋ_negotiation_tips(&negotiation_restrict,
					     &transport->smart_options->negotiation_restrict_tips,
					     "--negotiation-restrict");
		ਨਹੀਂਤਾਂ {
			ਬਣਤਰ strbuf config_name = STRBUF_INIT;
			strbuf_addf(&config_name, "remote.%s.negotiationRestrict", remote->name);
			warning(_("ignoring %s because the protocol does not support it"),
				config_name.buf);
			strbuf_release(&config_name);
		}
	}
	ਜੇ (negotiation_include.nr) {
		ਜੇ (transport->smart_options)
			ਜੋੜੋ_negotiation_tips(&negotiation_include,
					     &transport->smart_options->negotiation_include_tips,
					     "--negotiation-include");
		ਨਹੀਂਤਾਂ
			warning(_("ignoring %s because the protocol does not support it"),
				"--negotiation-include");
	} ਨਹੀਂਤਾਂ ਜੇ (remote->negotiation_include.nr) {
		ਜੇ (transport->smart_options) {
			ਜੋੜੋ_negotiation_tips(&remote->negotiation_include,
					     &transport->smart_options->negotiation_include_tips,
					     "--negotiation-include");
		} ਨਹੀਂਤਾਂ {
			ਬਣਤਰ strbuf config_name = STRBUF_INIT;
			strbuf_addf(&config_name, "remote.%s.negotiationInclude", remote->name);
			warning(_("ignoring %s because the protocol does not support it"),
				config_name.buf);
			strbuf_release(&config_name);
		}
	}
	ਵਾਪਸ transport;
}

ਸਥਿਰ ਪੂਰਨ backfill_ਟੈਗ_ਸੂਚੀ(ਬਣਤਰ display_state *display_state,
			 ਬਣਤਰ transport *transport,
			 ਬਣਤਰ ref_transaction *transaction,
			 ਬਣਤਰ ref *ref_map,
			 ਬਣਤਰ fetch_ਸਿਰ *fetch_ਸਿਰ,
			 ਅਚਲ ਬਣਤਰ fetch_config *config,
			 ਬਣਤਰ ref_update_display_info_array *display_array,
			 ਬਣਤਰ list_objects_filter_options *filter_options)
{
	ਪੂਰਨ retcode, cannot_reuse;

	/*
	 * Once we have set TRANS_OPT_DEEPEN_SINCE, we can't unset it
	 * when remote helper is used (setting it to an empty string
	 * is not unsetting). We could extend the remote helper
	 * protocol for that, but for now, just force a new connection
	 * without deepen-since. Similar story for deepen-not.
	 */
	cannot_reuse = transport->cannot_reuse ||
		deepen_since || deepen_not.nr;
	ਜੇ (cannot_reuse) {
		gsecondary = prepare_transport(transport->remote, 0, filter_options);
		transport = gsecondary;
	}

	transport_ਸੈੱਟ_option(transport, TRANS_OPT_FOLLOWTAGS, ਨਿਰਾ);
	transport_ਸੈੱਟ_option(transport, TRANS_OPT_ਡੂੰਘਾਈ, "0");
	transport_ਸੈੱਟ_option(transport, TRANS_OPT_DEEPEN_RELATIVE, ਨਿਰਾ);
	retcode = fetch_and_consume_refs(display_state, transport, transaction, ref_map,
					 fetch_ਸਿਰ, config, display_array);

	ਜੇ (gsecondary) {
		transport_disconnect(gsecondary);
		gsecondary = ਨਿਰਾ;
	}

	ਵਾਪਸ retcode;
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *strip_refshead(ਅਚਲ ਅੱਖਰ *name){
	skip_ਅਗੇਤਰ(name, "refs/heads/", &name);
	ਵਾਪਸ name;
}

ਸਥਿਰ ਖਾਲੀ ਸੈੱਟ_ਸਿਰ_advice_ਸੁਨੇਹਾ(ਅਚਲ ਅੱਖਰ *remote, ਅਚਲ ਅੱਖਰ *ਸਿਰ_name)
{
	ਅਚਲ ਅੱਖਰ ਸੁਨੇਹਾ_advice_ਸੈੱਟ_ਸਿਰ[] =
	N_("Run 'git remote set-head %s %s' to follow the change, or modify\n"
	   "either of the 'remote.%s.followRemoteHEAD' or 'fetch.followRemoteHEAD'\n"
	   "configuration variables to handle the situation differently.\n\n"

	   "Using this specific setting\n\n"
	   "    git config set remote.%s.followRemoteHEAD warn-if-not-%s\n\n"
	   "will suppress the warning until the remote changes HEAD to something else.");

	advise_if_enabled(ADVICE_FETCH_ਸੈੱਟ_ਸਿਰ_WARN, _(ਸੁਨੇਹਾ_advice_ਸੈੱਟ_ਸਿਰ),
			remote, ਸਿਰ_name, remote, remote, ਸਿਰ_name);
}

ਸਥਿਰ ਖਾਲੀ warn_ਸੈੱਟ_ਸਿਰ(ਅਚਲ ਅੱਖਰ *remote, ਅਚਲ ਅੱਖਰ *ਸਿਰ_name,
			ਬਣਤਰ strbuf *buf_prev, ਪੂਰਨ updateres) {
	ਬਣਤਰ strbuf buf_ਅਗੇਤਰ = STRBUF_INIT;
	ਅਚਲ ਅੱਖਰ *prev_ਸਿਰ = ਨਿਰਾ;

	strbuf_addf(&buf_ਅਗੇਤਰ, "refs/remotes/%s/", remote);
	skip_ਅਗੇਤਰ(buf_prev->buf, buf_ਅਗੇਤਰ.buf, &prev_ਸਿਰ);

	ਜੇ (prev_ਸਿਰ && strcmp(prev_ਸਿਰ, ਸਿਰ_name)) {
		ਲਿਖੋ("'HEAD' at '%s' is '%s', but we have '%s' locally.\n",
			remote, ਸਿਰ_name, prev_ਸਿਰ);
		ਸੈੱਟ_ਸਿਰ_advice_ਸੁਨੇਹਾ(remote, ਸਿਰ_name);
	}
	ਨਹੀਂਤਾਂ ਜੇ (updateres && buf_prev->len) {
		ਲਿਖੋ("'HEAD' at '%s' is '%s', "
			"but we have a detached HEAD pointing to '%s' locally.\n",
			remote, ਸਿਰ_name, buf_prev->buf);
		ਸੈੱਟ_ਸਿਰ_advice_ਸੁਨੇਹਾ(remote, ਸਿਰ_name);
	}
	strbuf_release(&buf_ਅਗੇਤਰ);
}

ਸਥਿਰ ਪੂਰਨ ਸੈੱਟ_ਸਿਰ(ਅਚਲ ਬਣਤਰ ref *remote_refs, ਬਣਤਰ remote *remote,
			ਪੂਰਨ follow_remote_ਸਿਰ)
{
	ਪੂਰਨ ਨਤੀਜਾ = 0, create_only, baremirror, was_detached;
	ਬਣਤਰ strbuf b_ਸਿਰ = STRBUF_INIT, b_remote_ਸਿਰ = STRBUF_INIT,
		      b_local_ਸਿਰ = STRBUF_INIT;
	ਅਚਲ ਅੱਖਰ *no_warn_branch = remote->no_warn_branch;
	ਅੱਖਰ *ਸਿਰ_name = ਨਿਰਾ;
	ਬਣਤਰ ref *ref, *matches;
	ਬਣਤਰ ref *fetch_map = ਨਿਰਾ, **fetch_map_ਪੂਛ = &fetch_map;
	ਬਣਤਰ refspec_ਵਸਤੂ refspec = {
		.force = 0,
		.ਨਮੂਨਾ = 1,
		.src = (ਅੱਖਰ *) "refs/heads/*",
		.dst = (ਅੱਖਰ *) "refs/heads/*",
	};
	ਬਣਤਰ string_list heads = STRING_LIST_INIT_DUP;
	ਬਣਤਰ ref_store *refs = get_main_ref_store(the_repository);

	get_fetch_map(remote_refs, &refspec, &fetch_map_ਪੂਛ, 0);
	matches = guess_remote_ਸਿਰ(ਲੱਭੋ_ref_by_name(remote_refs, "HEAD"),
				    fetch_map, REMOTE_GUESS_ਸਿਰ_ALL);
	ਲਈ (ref = matches; ref; ref = ref->ਅਗਲਾ) {
		string_list_append(&heads, strip_refshead(ref->name));
	}

	ਜੇ (!heads.nr)
		ਨਤੀਜਾ = 1;
	ਨਹੀਂਤਾਂ ਜੇ (heads.nr > 1)
		ਨਤੀਜਾ = 1;
	ਨਹੀਂਤਾਂ
		ਸਿਰ_name = xstrdup(heads.ਚੀਜ਼ਾਂ[0].string);

	ਜੇ (!ਸਿਰ_name)
		ਜਾਓ cleanup;
	baremirror = is_bare_repository(the_repository) && remote->mirror;
	create_only = follow_remote_ਸਿਰ == FOLLOW_REMOTE_ALWAYS ? 0 : !baremirror;
	ਜੇ (baremirror) {
		strbuf_addstr(&b_ਸਿਰ, "HEAD");
		strbuf_addf(&b_remote_ਸਿਰ, "refs/heads/%s", ਸਿਰ_name);
	} ਨਹੀਂਤਾਂ {
		strbuf_addf(&b_ਸਿਰ, "refs/remotes/%s/HEAD", remote->name);
		strbuf_addf(&b_remote_ਸਿਰ, "refs/remotes/%s/%s", remote->name, ਸਿਰ_name);
	}
	/* make sure it's valid */
	ਜੇ (!baremirror && !refs_ref_exists(refs, b_remote_ਸਿਰ.buf)) {
		ਨਤੀਜਾ = 1;
		ਜਾਓ cleanup;
	}
	was_detached = refs_update_symref_extended(refs, b_ਸਿਰ.buf, b_remote_ਸਿਰ.buf,
					"fetch", &b_local_ਸਿਰ, create_only);
	ਜੇ (was_detached == -1) {
		ਨਤੀਜਾ = 1;
		ਜਾਓ cleanup;
	}
	ਜੇ (verbosity >= 0 &&
		follow_remote_ਸਿਰ == FOLLOW_REMOTE_WARN &&
		(!no_warn_branch || strcmp(no_warn_branch, ਸਿਰ_name)))
		warn_ਸੈੱਟ_ਸਿਰ(remote->name, ਸਿਰ_name, &b_local_ਸਿਰ, was_detached);

cleanup:
	ਮੁਕਤ(ਸਿਰ_name);
	free_refs(fetch_map);
	free_refs(matches);
	string_list_clear(&heads, 0);
	strbuf_release(&b_ਸਿਰ);
	strbuf_release(&b_local_ਸਿਰ);
	strbuf_release(&b_remote_ਸਿਰ);
	ਵਾਪਸ ਨਤੀਜਾ;
}

ਬਣਤਰ ref_rejection_ਅੰਕੜੇ {
	ਪੂਰਨ *retcode;
	bool conflict_ਸੁਨੇਹਾ_shown;
	bool case_sensitive_ਸੁਨੇਹਾ_shown;
	ਅਚਲ ਅੱਖਰ *remote_name;
	ਬਣਤਰ strmap *rejected_refs;
};

ਸਥਿਰ ਖਾਲੀ ref_transaction_rejection_handler(ਅਚਲ ਅੱਖਰ *refname,
					      ਅਚਲ ਬਣਤਰ object_id *old_oid ਅਣਵਰਤਿਆ,
					      ਅਚਲ ਬਣਤਰ object_id *new_oid ਅਣਵਰਤਿਆ,
					      ਅਚਲ ਅੱਖਰ *old_ਨਿਸ਼ਾਨਾ ਅਣਵਰਤਿਆ,
					      ਅਚਲ ਅੱਖਰ *new_ਨਿਸ਼ਾਨਾ ਅਣਵਰਤਿਆ,
					      ਗਿਣਤੀ ref_transaction_error err,
					      ਅਚਲ ਅੱਖਰ *details,
					      ਖਾਲੀ *cb_ਅੰਕੜੇ)
{
	ਬਣਤਰ ref_rejection_ਅੰਕੜੇ *ਅੰਕੜੇ = cb_ਅੰਕੜੇ;

	ਜੇ (err == REF_TRANSACTION_ERROR_CASE_CONFLICT && repo_ignore_case(the_repository) &&
	    !ਅੰਕੜੇ->case_sensitive_ਸੁਨੇਹਾ_shown) {
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
		ਅੰਕੜੇ->case_sensitive_ਸੁਨੇਹਾ_shown = true;
	} ਨਹੀਂਤਾਂ ਜੇ (err == REF_TRANSACTION_ERROR_NAME_CONFLICT &&
		   !ਅੰਕੜੇ->conflict_ਸੁਨੇਹਾ_shown) {
		error(_("some local refs could not be updated; try running\n"
			" 'git remote prune %s' to remove any old, conflicting "
			"branches"), ਅੰਕੜੇ->remote_name);
		ਅੰਕੜੇ->conflict_ਸੁਨੇਹਾ_shown = true;
	} ਨਹੀਂਤਾਂ {
		ਜੇ (details)
			error("%s", details);
		ਨਹੀਂਤਾਂ
			error(_("fetching ref %s failed: %s"),
			      refname, ref_transaction_error_ਸੁਨੇਹਾ(err));
	}

	strmap_put(ਅੰਕੜੇ->rejected_refs, refname, ਨਿਰਾ);
	*ਅੰਕੜੇ->retcode = 1;
}

/*
 * Commit the reference transaction. If it isn't an atomic transaction, handle
 * rejected updates as part of using batched updates.
 */
ਸਥਿਰ ਪੂਰਨ commit_ref_transaction(ਬਣਤਰ ref_transaction **transaction,
				  bool is_atomic, ਅਚਲ ਅੱਖਰ *remote_name,
				  ਬਣਤਰ strmap *rejected_refs,
				  ਬਣਤਰ strbuf *err)
{
	ਪੂਰਨ retcode = ref_transaction_commit(*transaction, err);
	ਜੇ (retcode)
		ਜਾਓ ਬਾਹਰਲਾ;

	ਜੇ (!is_atomic) {
		ਬਣਤਰ ref_rejection_ਅੰਕੜੇ ਅੰਕੜੇ = {
			.conflict_ਸੁਨੇਹਾ_shown = 0,
			.remote_name = remote_name,
			.retcode = &retcode,
			.rejected_refs = rejected_refs,
		};

		ref_transaction_for_each_rejected_update(*transaction,
							 ref_transaction_rejection_handler,
							 &ਅੰਕੜੇ);
	}

ਬਾਹਰਲਾ:
	ref_transaction_free(*transaction);
	*transaction = ਨਿਰਾ;
	ਵਾਪਸ retcode;
}

ਸਥਿਰ ਖਾਲੀ collect_updated_tips(ਬਣਤਰ oidset *tips, ਬਣਤਰ ref *ref_map)
{
	ਬਣਤਰ ref *rm;
	ਲਈ (rm = ref_map; rm; rm = rm->ਅਗਲਾ) {
		ਬਣਤਰ commit *commit;
		/*
		 * Shallow-rejected refs are not stored and their history
		 * is incomplete, so skip them.
		 */
		ਜੇ (rm->ਸਥਿਤੀ == REF_ਸਥਿਤੀ_REJECT_SHALLOW)
			ਜਾਰੀ;
		ਜੇ (is_null_oid(&rm->old_oid))
			ਜਾਰੀ;
		ਜੇ (rm->peer_ref &&
		    oideq(&rm->old_oid, &rm->peer_ref->old_oid))
			ਜਾਰੀ;
		commit = lookup_commit_reference_gently(the_repository,
							&rm->old_oid, 1);
		ਜੇ (commit)
			oidset_ਪਾਓ(tips, &commit->object.oid);
	}
}

ਸਥਿਰ ਪੂਰਨ do_fetch(ਬਣਤਰ transport *transport,
		    ਬਣਤਰ refspec *rs,
		    ਅਚਲ ਬਣਤਰ fetch_config *config,
		    ਬਣਤਰ list_objects_filter_options *filter_options,
		    ਬਣਤਰ oidset *updated_tips)
{
	ਬਣਤਰ ref_transaction *transaction = ਨਿਰਾ;
	ਬਣਤਰ ref *ref_map = ਨਿਰਾ;
	ਬਣਤਰ display_state display_state = { 0 };
	ਪੂਰਨ autotags = (transport->remote->fetch_ਟੈਗ_ਸੂਚੀ == 1);
	ਪੂਰਨ retcode = 0;
	ਅਚਲ ਬਣਤਰ ref *remote_refs;
	ਬਣਤਰ transport_ls_refs_options transport_ls_refs_options =
		TRANSPORT_LS_REFS_OPTIONS_INIT;
	ਬਣਤਰ fetch_ਸਿਰ fetch_ਸਿਰ = { 0 };
	ਬਣਤਰ strbuf err = STRBUF_INIT;
	ਬਣਤਰ ref_update_display_info_array display_array = { 0 };
	ਬਣਤਰ strmap rejected_refs = STRMAP_INIT;
	ਪੂਰਨ summary_ਚੌੜਾਈ = 0;
	ਪੂਰਨ follow_remote_ਸਿਰ = FOLLOW_REMOTE_NEVER;

	ਜੇ (ਟੈਗ_ਸੂਚੀ == ਟੈਗ_ਸੂਚੀ_DEFAULT) {
		ਜੇ (transport->remote->fetch_ਟੈਗ_ਸੂਚੀ == 2)
			ਟੈਗ_ਸੂਚੀ = ਟੈਗ_ਸੂਚੀ_ਸੈੱਟ;
		ਜੇ (transport->remote->fetch_ਟੈਗ_ਸੂਚੀ == -1)
			ਟੈਗ_ਸੂਚੀ = ਟੈਗ_ਸੂਚੀ_UNSET;
	}

	/* if not appending, truncate FETCH_HEAD */
	ਜੇ (!append && write_fetch_ਸਿਰ) {
		retcode = truncate_fetch_ਸਿਰ();
		ਜੇ (retcode)
			ਜਾਓ cleanup;
	}

	ਜੇ (rs->nr) {
		refspec_ref_prefixes(rs, &transport_ls_refs_options.ref_prefixes);
	} ਨਹੀਂਤਾਂ {
		ਬਣਤਰ branch *branch = branch_get(ਨਿਰਾ);

		ਜੇ (transport->remote->fetch.nr) {
			refspec_ref_prefixes(&transport->remote->fetch,
					     &transport_ls_refs_options.ref_prefixes);
			/*
			 * See remote.c's handling of remote.<name>.followRemoteHEAD
			 * for the analogous, still-unresolved case.
			 */
			ਜੇ (transport->remote->follow_remote_ਸਿਰ)
				follow_remote_ਸਿਰ = transport->remote->follow_remote_ਸਿਰ;
			ਨਹੀਂਤਾਂ ਜੇ (config->follow_remote_ਸਿਰ_ਵੇਖਿਆ)
				follow_remote_ਸਿਰ = get_follow_remote_ਸਿਰ(config->follow_remote_ਸਿਰ_raw);
			ਨਹੀਂਤਾਂ
				follow_remote_ਸਿਰ = BUILTIN_FOLLOW_REMOTE_ਸਿਰ_DFLT;
		}
		ਜੇ (branch && branch_has_ਮਿਲਾਓ_config(branch) &&
		    !strcmp(branch->remote_name, transport->remote->name)) {
			ਪੂਰਨ i;
			ਲਈ (i = 0; i < branch->ਮਿਲਾਓ_nr; i++) {
				strvec_ਧੱਕੋ(&transport_ls_refs_options.ref_prefixes,
					    branch->ਮਿਲਾਓ[i]->src);
			}
		}

		/*
		 * If there are no refs specified to fetch, then we just
		 * fetch HEAD; mention that to narrow the advertisement.
		 */
		ਜੇ (!transport_ls_refs_options.ref_prefixes.nr)
			strvec_ਧੱਕੋ(&transport_ls_refs_options.ref_prefixes,
				    "HEAD");
	}

	ਜੇ (ਟੈਗ_ਸੂਚੀ == ਟੈਗ_ਸੂਚੀ_ਸੈੱਟ || ਟੈਗ_ਸੂਚੀ == ਟੈਗ_ਸੂਚੀ_DEFAULT)
		strvec_ਧੱਕੋ(&transport_ls_refs_options.ref_prefixes,
			    "refs/tags/");

	ਜੇ (follow_remote_ਸਿਰ != FOLLOW_REMOTE_NEVER)
		strvec_ਧੱਕੋ(&transport_ls_refs_options.ref_prefixes,
			    "HEAD");

	/*
	 * Only initiate ref listing if we have at least one ref we want to
	 * know about.
	 */
	ਜੇ (transport_ls_refs_options.ref_prefixes.nr) {
		trace2_region_enter("fetch", "remote_refs", the_repository);
		remote_refs = transport_get_remote_refs(transport,
							&transport_ls_refs_options);
		trace2_region_leave("fetch", "remote_refs", the_repository);
	} ਨਹੀਂਤਾਂ
		remote_refs = ਨਿਰਾ;

	transport_ls_refs_options_release(&transport_ls_refs_options);

	ref_map = get_ref_map(transport->remote, remote_refs, rs,
			      ਟੈਗ_ਸੂਚੀ, &autotags);
	ਜੇ (!update_ਸਿਰ_ok)
		check_not_current_branch(ref_map);

	retcode = open_fetch_ਸਿਰ(&fetch_ਸਿਰ);
	ਜੇ (retcode)
		ਜਾਓ cleanup;

	display_state_init(&display_state, ref_map, transport->url,
			   config->display_format);

	ਜੇ (atomic_fetch) {
		transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
							  0, &err);
		ਜੇ (!transaction) {
			retcode = -1;
			ਜਾਓ cleanup;
		}
	}

	ਜੇ (ਟੈਗ_ਸੂਚੀ == ਟੈਗ_ਸੂਚੀ_DEFAULT && autotags)
		transport_ਸੈੱਟ_option(transport, TRANS_OPT_FOLLOWTAGS, "1");
	ਜੇ (prune) {
		/*
		 * We only prune based on refspecs specified
		 * explicitly (via command line or configuration); we
		 * don't care whether --tags was specified.
		 */
		ਜੇ (rs->nr) {
			retcode = prune_refs(&display_state, rs, transaction, ref_map);
		} ਨਹੀਂਤਾਂ {
			retcode = prune_refs(&display_state, &transport->remote->fetch,
					     transaction, ref_map);
		}
		ਜੇ (retcode != 0)
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
	ਜੇ (!transaction) {
		transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
							  REF_TRANSACTION_ALLOW_FAILURE, &err);
		ਜੇ (!transaction) {
			retcode = -1;
			ਜਾਓ cleanup;
		}
	}

	ਜੇ (fetch_and_consume_refs(&display_state, transport, transaction, ref_map,
				   &fetch_ਸਿਰ, config, &display_array)) {
		retcode = 1;
		ਜਾਓ cleanup;
	}

	/*
	 * If neither --no-tags nor --tags was specified, do automated tag
	 * following.
	 */
	ਜੇ (ਟੈਗ_ਸੂਚੀ == ਟੈਗ_ਸੂਚੀ_DEFAULT && autotags) {
		ਬਣਤਰ ref *ਟੈਗ_ਸੂਚੀ_ref_map = ਨਿਰਾ, **ਪੂਛ = &ਟੈਗ_ਸੂਚੀ_ref_map;

		ਲੱਭੋ_non_local_ਟੈਗ_ਸੂਚੀ(remote_refs, transaction, &ਟੈਗ_ਸੂਚੀ_ref_map, &ਪੂਛ);
		ਜੇ (ਟੈਗ_ਸੂਚੀ_ref_map) {
			/*
			 * If backfilling of tags fails then we want to tell
			 * the user so, but we have to continue regardless to
			 * populate upstream information of the references we
			 * have already fetched above. The exception though is
			 * when `--atomic` is passed: in that case we'll abort
			 * the transaction and don't commit anything.
			 */
			ਜੇ (backfill_ਟੈਗ_ਸੂਚੀ(&display_state, transport, transaction, ਟੈਗ_ਸੂਚੀ_ref_map,
					  &fetch_ਸਿਰ, config, &display_array, filter_options))
				retcode = 1;
		}

		free_refs(ਟੈਗ_ਸੂਚੀ_ref_map);
	}

	ਜੇ (retcode)
		ਜਾਓ cleanup;

	ਜੇ (verbosity >= 0)
		summary_ਚੌੜਾਈ = transport_summary_ਚੌੜਾਈ(ref_map);

	retcode = commit_ref_transaction(&transaction, atomic_fetch,
					 transport->remote->name,
					 &rejected_refs, &err);
	/*
	 * With '--atomic', bail out if the transaction fails. Without '--atomic',
	 * continue to fetch head and perform other post-fetch operations.
	 */
	ਜੇ (retcode && atomic_fetch)
		ਜਾਓ cleanup;

	commit_fetch_ਸਿਰ(&fetch_ਸਿਰ);

	collect_updated_tips(updated_tips, ref_map);

	ਜੇ (ਸੈੱਟ_upstream) {
		ਬਣਤਰ branch *branch = branch_get("HEAD");
		ਬਣਤਰ ref *rm;
		ਬਣਤਰ ref *ਸਰੋਤ_ref = ਨਿਰਾ;

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
		ਲਈ (rm = ref_map; rm; rm = rm->ਅਗਲਾ) {
			ਜੇ (!rm->peer_ref) {
				ਜੇ (ਸਰੋਤ_ref) {
					warning(_("multiple branches detected, incompatible with --set-upstream"));
					ਜਾਓ cleanup;
				} ਨਹੀਂਤਾਂ {
					ਸਰੋਤ_ref = rm;
				}
			}
		}
		ਜੇ (ਸਰੋਤ_ref) {
			ਜੇ (!branch) {
				ਅਚਲ ਅੱਖਰ *shortname = ਸਰੋਤ_ref->name;
				skip_ਅਗੇਤਰ(shortname, "refs/heads/", &shortname);

				warning(_("could not set upstream of HEAD to '%s' from '%s' when "
					  "it does not point to any branch."),
					shortname, transport->remote->name);
				ਜਾਓ cleanup;
			}

			ਜੇ (!strcmp(ਸਰੋਤ_ref->name, "HEAD") ||
			    starts_with(ਸਰੋਤ_ref->name, "refs/heads/"))
				install_branch_config(0,
						      branch->name,
						      transport->remote->name,
						      ਸਰੋਤ_ref->name);
			ਨਹੀਂਤਾਂ ਜੇ (starts_with(ਸਰੋਤ_ref->name, "refs/remotes/"))
				warning(_("not setting upstream for a remote remote-tracking branch"));
			ਨਹੀਂਤਾਂ ਜੇ (starts_with(ਸਰੋਤ_ref->name, "refs/tags/"))
				warning(_("not setting upstream for a remote tag"));
			ਨਹੀਂਤਾਂ
				warning(_("unknown branch type"));
		} ਨਹੀਂਤਾਂ {
			warning(_("no source branch found;\n"
				  "you need to specify exactly one branch with the --set-upstream option"));
		}
	}
	ਜੇ (follow_remote_ਸਿਰ != FOLLOW_REMOTE_NEVER) {
		/*
		 * Way too many cases where this can go wrong so let's just
		 * ignore errors and fail silently for now.
		 */
		ਸੈੱਟ_ਸਿਰ(remote_refs, transport->remote, follow_remote_ਸਿਰ);
	}

cleanup:
	/*
	 * When using batched updates, we want to commit the non-rejected
	 * updates and also handle the rejections.
	 */
	ਜੇ (retcode && !atomic_fetch && transaction)
		commit_ref_transaction(&transaction, false,
				       transport->remote->name,
				       &rejected_refs, &err);

	ਲਈ (ਮਾਪ_t i = 0; i < display_array.nr; i++) {
		ਬਣਤਰ ref_update_display_info *info = &display_array.info[i];

		ਜੇ (!info->failed && strmap_ਸ਼ਾਮਲ_ਹੈ(&rejected_refs, info->ref))
			ref_update_display_info_ਸੈੱਟ_failed(info);
		ref_update_display_info_display(info, &display_state, summary_ਚੌੜਾਈ);
		ref_update_display_info_free(info);
	}

	ਜੇ (retcode) {
		ਜੇ (err.len) {
			error("%s", err.buf);
			strbuf_reset(&err);
		}
		ਜੇ (transaction && ref_transaction_abort(transaction, &err) &&
		    err.len)
			error("%s", err.buf);
		transaction = ਨਿਰਾ;
	}

	ਜੇ (transaction)
		ref_transaction_free(transaction);

	ਮੁਕਤ(display_array.info);
	strmap_clear(&rejected_refs, 0);
	display_state_release(&display_state);
	close_fetch_ਸਿਰ(&fetch_ਸਿਰ);
	strbuf_release(&err);
	free_refs(ref_map);
	ਵਾਪਸ retcode;
}

ਸਥਿਰ ਪੂਰਨ get_one_remote_for_fetch(ਬਣਤਰ remote *remote, ਖਾਲੀ *priv)
{
	ਬਣਤਰ string_list *list = priv;
	ਜੇ (!remote->skip_default_update)
		string_list_append(list, remote->name);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ ਜੋੜੋ_options_ਮੰਜ਼ਿਲ_argv(ਬਣਤਰ strvec *argv,
				ਅਚਲ ਬਣਤਰ fetch_config *config)
{
	ਜੇ (dry_ਲੜੀਵਾਰ)
		strvec_ਧੱਕੋ(argv, "--dry-run");
	ਜੇ (prune != -1)
		strvec_ਧੱਕੋ(argv, prune ? "--prune" : "--no-prune");
	ਜੇ (prune_ਟੈਗ_ਸੂਚੀ != -1)
		strvec_ਧੱਕੋ(argv, prune_ਟੈਗ_ਸੂਚੀ ? "--prune-tags" : "--no-prune-tags");
	ਜੇ (update_ਸਿਰ_ok)
		strvec_ਧੱਕੋ(argv, "--update-head-ok");
	ਜੇ (force)
		strvec_ਧੱਕੋ(argv, "--force");
	ਜੇ (keep)
		strvec_ਧੱਕੋ(argv, "--keep");
	ਜੇ (config->recurse_submodules == RECURSE_SUBMODULES_ON)
		strvec_ਧੱਕੋ(argv, "--recurse-submodules");
	ਨਹੀਂਤਾਂ ਜੇ (config->recurse_submodules == RECURSE_SUBMODULES_OFF)
		strvec_ਧੱਕੋ(argv, "--no-recurse-submodules");
	ਨਹੀਂਤਾਂ ਜੇ (config->recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND)
		strvec_ਧੱਕੋ(argv, "--recurse-submodules=on-demand");
	ਜੇ (ਟੈਗ_ਸੂਚੀ == ਟੈਗ_ਸੂਚੀ_ਸੈੱਟ)
		strvec_ਧੱਕੋ(argv, "--tags");
	ਨਹੀਂਤਾਂ ਜੇ (ਟੈਗ_ਸੂਚੀ == ਟੈਗ_ਸੂਚੀ_UNSET)
		strvec_ਧੱਕੋ(argv, "--no-tags");
	ਜੇ (verbosity >= 2)
		strvec_ਧੱਕੋ(argv, "-v");
	ਜੇ (verbosity >= 1)
		strvec_ਧੱਕੋ(argv, "-v");
	ਨਹੀਂਤਾਂ ਜੇ (verbosity < 0)
		strvec_ਧੱਕੋ(argv, "-q");
	ਜੇ (family == TRANSPORT_FAMILY_IPV4)
		strvec_ਧੱਕੋ(argv, "--ipv4");
	ਨਹੀਂਤਾਂ ਜੇ (family == TRANSPORT_FAMILY_IPV6)
		strvec_ਧੱਕੋ(argv, "--ipv6");
	ਜੇ (!write_fetch_ਸਿਰ)
		strvec_ਧੱਕੋ(argv, "--no-write-fetch-head");
	ਜੇ (config->display_format == DISPLAY_FORMAT_PORCELAIN)
		strvec_pushf(argv, "--porcelain");
}

/* Fetch multiple remotes in parallel */

ਬਣਤਰ parallel_fetch_state {
	ਅਚਲ ਅੱਖਰ **argv;
	ਬਣਤਰ string_list *remotes;
	ਪੂਰਨ ਅਗਲਾ, ਨਤੀਜਾ;
	ਅਚਲ ਬਣਤਰ fetch_config *config;
};

ਸਥਿਰ ਪੂਰਨ fetch_ਅਗਲਾ_remote(ਬਣਤਰ ਬੱਚਾ_process *cp,
			     ਬਣਤਰ strbuf *ਬਾਹਰਲਾ ਅਣਵਰਤਿਆ,
			     ਖਾਲੀ *cb, ਖਾਲੀ **task_cb)
{
	ਬਣਤਰ parallel_fetch_state *state = cb;
	ਅੱਖਰ *remote;

	ਜੇ (state->ਅਗਲਾ < 0 || state->ਅਗਲਾ >= state->remotes->nr)
		ਵਾਪਸ 0;

	remote = state->remotes->ਚੀਜ਼ਾਂ[state->ਅਗਲਾ++].string;
	*task_cb = remote;

	strvec_pushv(&cp->ਦਲੀਲਾਂ, state->argv);
	strvec_ਧੱਕੋ(&cp->ਦਲੀਲਾਂ, remote);
	cp->git_cmd = 1;

	ਜੇ (verbosity >= 0 && state->config->display_format != DISPLAY_FORMAT_PORCELAIN)
		ਲਿਖੋ(_("Fetching %s\n"), remote);

	ਵਾਪਸ 1;
}

ਸਥਿਰ ਪੂਰਨ fetch_failed_ਮੰਜ਼ਿਲ_ਸ਼ੁਰੂ(ਬਣਤਰ strbuf *ਬਾਹਰਲਾ ਅਣਵਰਤਿਆ,
				 ਖਾਲੀ *cb, ਖਾਲੀ *task_cb)
{
	ਬਣਤਰ parallel_fetch_state *state = cb;
	ਅਚਲ ਅੱਖਰ *remote = task_cb;

	state->ਨਤੀਜਾ = error(_("could not fetch %s"), remote);

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ fetch_ਮੁਕੰਮਲ(ਪੂਰਨ ਨਤੀਜਾ, ਬਣਤਰ strbuf *ਬਾਹਰਲਾ,
			  ਖਾਲੀ *cb, ਖਾਲੀ *task_cb)
{
	ਬਣਤਰ parallel_fetch_state *state = cb;
	ਅਚਲ ਅੱਖਰ *remote = task_cb;

	ਜੇ (ਨਤੀਜਾ) {
		strbuf_addf(ਬਾਹਰਲਾ, _("could not fetch '%s' (exit code: %d)\n"),
			    remote, ਨਤੀਜਾ);
		state->ਨਤੀਜਾ = -1;
	}

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ fetch_ਗੁਣਜ(ਬਣਤਰ string_list *list, ਪੂਰਨ max_children,
			  ਅਚਲ ਬਣਤਰ fetch_config *config)
{
	ਪੂਰਨ i, ਨਤੀਜਾ = 0;
	ਬਣਤਰ strvec argv = STRVEC_INIT;

	ਜੇ (!append && write_fetch_ਸਿਰ) {
		ਪੂਰਨ errcode = truncate_fetch_ਸਿਰ();
		ਜੇ (errcode)
			ਵਾਪਸ errcode;
	}

	/*
	 * Cancel out the fetch.bundleURI config when running subprocesses,
	 * to avoid fetching from the same bundle list multiple times.
	 */
	strvec_pushl(&argv, "-c", "fetch.bundleURI=",
		     "fetch", "--append", "--no-auto-gc",
		     "--no-write-commit-graph", ਨਿਰਾ);
	ਲਈ (i = 0; i < server_options.nr; i++)
		strvec_pushf(&argv, "--server-option=%s", server_options.ਚੀਜ਼ਾਂ[i].string);
	ਜੋੜੋ_options_ਮੰਜ਼ਿਲ_argv(&argv, config);

	ਜੇ (max_children != 1 && list->nr != 1) {
		ਬਣਤਰ parallel_fetch_state state = { argv.v, list, 0, 0, config };
		ਅਚਲ ਬਣਤਰ ਲੜੀਵਾਰ_process_parallel_opts opts = {
			.tr2_category = "fetch",
			.tr2_label = "parallel/fetch",

			.processes = max_children,

			.get_ਅਗਲਾ_task = &fetch_ਅਗਲਾ_remote,
			.ਸ਼ੁਰੂ_failure = &fetch_failed_ਮੰਜ਼ਿਲ_ਸ਼ੁਰੂ,
			.task_ਮੁਕੰਮਲ = &fetch_ਮੁਕੰਮਲ,
			.ਅੰਕੜੇ = &state,
		};

		strvec_ਧੱਕੋ(&argv, "--end-of-options");

		ਲੜੀਵਾਰ_processes_parallel(&opts);
		ਨਤੀਜਾ = state.ਨਤੀਜਾ;
	} ਨਹੀਂਤਾਂ
		ਲਈ (i = 0; i < list->nr; i++) {
			ਅਚਲ ਅੱਖਰ *name = list->ਚੀਜ਼ਾਂ[i].string;
			ਬਣਤਰ ਬੱਚਾ_process cmd = ਬੱਚਾ_PROCESS_INIT;

			strvec_pushv(&cmd.ਦਲੀਲਾਂ, argv.v);
			strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, name);
			ਜੇ (verbosity >= 0 && config->display_format != DISPLAY_FORMAT_PORCELAIN)
				ਲਿਖੋ(_("Fetching %s\n"), name);
			cmd.git_cmd = 1;
			ਜੇ (ਲੜੀਵਾਰ_command(&cmd)) {
				error(_("could not fetch %s"), name);
				ਨਤੀਜਾ = 1;
			}
		}

	strvec_clear(&argv);
	ਵਾਪਸ !!ਨਤੀਜਾ;
}

/*
 * Fetching from the promisor remote should use the given filter-spec
 * or inherit the default filter-spec from the config.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ fetch_one_ਤਿਆਰੀ_partial(ਬਣਤਰ remote *remote,
					   ਬਣਤਰ list_objects_filter_options *filter_options)
{
	/*
	 * Explicit --no-filter argument overrides everything, regardless
	 * of any prior partial clones and fetches.
	 */
	ਜੇ (filter_options->no_filter)
		ਵਾਪਸ;

	/*
	 * If no prior partial clone/fetch and the current fetch DID NOT
	 * request a partial-fetch, do a normal fetch.
	 */
	ਜੇ (!repo_has_promisor_remote(the_repository) && !filter_options->choice)
		ਵਾਪਸ;

	/*
	 * If this is a partial-fetch request, we enable partial on
	 * this repo if not already enabled and remember the given
	 * filter-spec as the default for subsequent fetches to this
	 * remote if there is currently no default filter-spec.
	 */
	ਜੇ (filter_options->choice) {
		partial_clone_ਰਜਿਸਟਰ(remote->name, filter_options);
		ਵਾਪਸ;
	}

	/*
	 * Do a partial-fetch from the promisor remote using either the
	 * explicitly given filter-spec or inherit the filter-spec from
	 * the config.
	 */
	ਜੇ (!filter_options->choice)
		partial_clone_get_default_filter_spec(filter_options, remote->name);
	ਵਾਪਸ;
}

ਸਥਿਰ ਪੂਰਨ fetch_one(ਬਣਤਰ remote *remote, ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv,
		     ਪੂਰਨ prune_ਟੈਗ_ਸੂਚੀ_ok, ਪੂਰਨ use_stdin_refspecs,
		     ਅਚਲ ਬਣਤਰ fetch_config *config,
		     ਬਣਤਰ list_objects_filter_options *filter_options,
		     ਬਣਤਰ oidset *updated_tips)
{
	ਬਣਤਰ refspec rs = REFSPEC_INIT_FETCH(the_hash_algo);
	ਪੂਰਨ i;
	ਪੂਰਨ exit_ਕੋਡ;
	ਪੂਰਨ maybe_prune_ਟੈਗ_ਸੂਚੀ;
	ਪੂਰਨ remote_ਰਾਹੀਂ_config = remote_is_configured(remote, 0);

	ਜੇ (!remote)
		die(_("no remote repository specified; please specify either a URL or a\n"
		      "remote name from which new revisions should be fetched"));

	gtransport = prepare_transport(remote, 1, filter_options);

	ਜੇ (prune < 0) {
		/* no command line request */
		ਜੇ (0 <= remote->prune)
			prune = remote->prune;
		ਨਹੀਂਤਾਂ ਜੇ (0 <= config->prune)
			prune = config->prune;
		ਨਹੀਂਤਾਂ
			prune = PRUNE_BY_DEFAULT;
	}

	ਜੇ (prune_ਟੈਗ_ਸੂਚੀ < 0) {
		/* no command line request */
		ਜੇ (0 <= remote->prune_ਟੈਗ_ਸੂਚੀ)
			prune_ਟੈਗ_ਸੂਚੀ = remote->prune_ਟੈਗ_ਸੂਚੀ;
		ਨਹੀਂਤਾਂ ਜੇ (0 <= config->prune_ਟੈਗ_ਸੂਚੀ)
			prune_ਟੈਗ_ਸੂਚੀ = config->prune_ਟੈਗ_ਸੂਚੀ;
		ਨਹੀਂਤਾਂ
			prune_ਟੈਗ_ਸੂਚੀ = PRUNE_ਟੈਗ_ਸੂਚੀ_BY_DEFAULT;
	}

	maybe_prune_ਟੈਗ_ਸੂਚੀ = prune_ਟੈਗ_ਸੂਚੀ_ok && prune_ਟੈਗ_ਸੂਚੀ;
	ਜੇ (maybe_prune_ਟੈਗ_ਸੂਚੀ && remote_ਰਾਹੀਂ_config)
		refspec_append(&remote->fetch, ਟੈਗ_REFSPEC);

	ਜੇ (maybe_prune_ਟੈਗ_ਸੂਚੀ && (argc || !remote_ਰਾਹੀਂ_config))
		refspec_append(&rs, ਟੈਗ_REFSPEC);

	ਲਈ (i = 0; i < argc; i++) {
		ਜੇ (!strcmp(argv[i], "tag")) {
			i++;
			ਜੇ (i >= argc)
				die(_("you need to specify a tag name"));

			refspec_appendf(&rs, "refs/tags/%s:refs/tags/%s",
					argv[i], argv[i]);
		} ਨਹੀਂਤਾਂ {
			refspec_append(&rs, argv[i]);
		}
	}

	ਜੇ (use_stdin_refspecs) {
		ਬਣਤਰ strbuf ਸਤਰ = STRBUF_INIT;
		ਜਦੋਂ (strbuf_getline_lf(&ਸਤਰ, stdin) != EOF)
			refspec_append(&rs, ਸਤਰ.buf);
		strbuf_release(&ਸਤਰ);
	}

	ਜੇ (server_options.nr)
		gtransport->server_options = &server_options;

	sigchain_ਧੱਕੋ_common(unlock_pack_on_signal);
	atexit(unlock_pack_atexit);
	sigchain_ਧੱਕੋ(SIGPIPE, SIG_IGN);
	exit_ਕੋਡ = do_fetch(gtransport, &rs, config, filter_options,
			     updated_tips);
	sigchain_ਕੱਢੋ(SIGPIPE);
	refspec_clear(&rs);
	transport_disconnect(gtransport);
	gtransport = ਨਿਰਾ;
	ਵਾਪਸ exit_ਕੋਡ;
}

ਪੂਰਨ cmd_fetch(ਪੂਰਨ argc,
	      ਅਚਲ ਅੱਖਰ **argv,
	      ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
	      ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ fetch_config config = {
		.display_format = DISPLAY_FORMAT_ਭਰਿਆ,
		.follow_remote_ਸਿਰ_raw = ਨਿਰਾ,
		.follow_remote_ਸਿਰ_ਵੇਖਿਆ = 0,
		.prune = -1,
		.prune_ਟੈਗ_ਸੂਚੀ = -1,
		.show_forced_updates = 1,
		.recurse_submodules = RECURSE_SUBMODULES_DEFAULT,
		.parallel = 1,
		.submodule_fetch_jobs = -1,
	};
	ਅਚਲ ਅੱਖਰ *submodule_ਅਗੇਤਰ = "";
	ਅਚਲ ਅੱਖਰ *bundle_uri;
	ਬਣਤਰ string_list list = STRING_LIST_INIT_DUP;
	ਬਣਤਰ list_objects_filter_options filter_options = LIST_OBJECTS_FILTER_INIT;
	ਬਣਤਰ remote *remote = ਨਿਰਾ;
	ਪੂਰਨ all = -1, ਗੁਣਜ = 0;
	ਪੂਰਨ ਨਤੀਜਾ = 0;
	ਪੂਰਨ prune_ਟੈਗ_ਸੂਚੀ_ok = 1;
	ਪੂਰਨ enable_auto_gc = 1;
	ਪੂਰਨ unshallow = 0;
	ਪੂਰਨ max_jobs = -1;
	ਪੂਰਨ recurse_submodules_cli = RECURSE_SUBMODULES_DEFAULT;
	ਪੂਰਨ recurse_submodules_default = RECURSE_SUBMODULES_ON_DEMAND;
	ਪੂਰਨ fetch_write_commit_ਗ੍ਰਾਫ = -1;
	ਪੂਰਨ stdin_refspecs = 0;
	ਪੂਰਨ negotiate_only = 0;
	ਪੂਰਨ porcelain = 0;
	ਪੂਰਨ i;
	ਗਿਣਤੀ {
		ਗ੍ਰਾਫ_WRITE_REACHABLE,
		ਗ੍ਰਾਫ_WRITE_TIPS,
		ਗ੍ਰਾਫ_WRITE_SKIP,
	} ਗ੍ਰਾਫ_write_ਬਹੁਲਕ = ਗ੍ਰਾਫ_WRITE_REACHABLE;
	ਬਣਤਰ oidset updated_tips = OIDSET_INIT;

	ਬਣਤਰ option builtin_fetch_options[] = {
		OPT__VERBOSITY(&verbosity),
		OPT_BOOL(0, "all", &all,
			 N_("fetch from all remotes")),
		OPT_BOOL(0, "set-upstream", &ਸੈੱਟ_upstream,
			 N_("set upstream for git pull/fetch")),
		OPT_BOOL('a', "append", &append,
			 N_("append to .git/FETCH_HEAD instead of overwriting")),
		OPT_BOOL(0, "atomic", &atomic_fetch,
			 N_("use atomic transaction to update references")),
		OPT_STRING(0, "upload-pack", &upload_pack, N_("path"),
			   N_("path to upload pack on remote end")),
		OPT__FORCE(&force, N_("force overwrite of local reference"), 0),
		OPT_BOOL('m', "multiple", &ਗੁਣਜ,
			 N_("fetch from multiple remotes")),
		OPT_ਸੈੱਟ_INT('t', "tags", &ਟੈਗ_ਸੂਚੀ,
			    N_("fetch all tags and associated objects"), ਟੈਗ_ਸੂਚੀ_ਸੈੱਟ),
		OPT_ਸੈੱਟ_INT('n', ਨਿਰਾ, &ਟੈਗ_ਸੂਚੀ,
			    N_("do not fetch all tags (--no-tags)"), ਟੈਗ_ਸੂਚੀ_UNSET),
		OPT_INTEGER('j', "jobs", &max_jobs,
			    N_("number of submodules fetched in parallel")),
		OPT_BOOL(0, "prefetch", &prefetch,
			 N_("modify the refspec to place all refs within refs/prefetch/")),
		OPT_BOOL('p', "prune", &prune,
			 N_("prune remote-tracking branches no longer on remote")),
		OPT_BOOL('P', "prune-tags", &prune_ਟੈਗ_ਸੂਚੀ,
			 N_("prune local tags no longer on remote and clobber changed tags")),
		OPT_CALLBACK_F(0, "recurse-submodules", &recurse_submodules_cli, N_("on-demand"),
			    N_("control recursive fetching of submodules"),
			    PARSE_OPT_OPTARG, option_fetch_parse_recurse_submodules),
		OPT_BOOL(0, "dry-run", &dry_ਲੜੀਵਾਰ,
			 N_("dry run")),
		OPT_BOOL(0, "porcelain", &porcelain, N_("machine-readable output")),
		OPT_BOOL(0, "write-fetch-head", &write_fetch_ਸਿਰ,
			 N_("write fetched references to the FETCH_HEAD file")),
		OPT_BOOL('k', "keep", &keep, N_("keep downloaded pack")),
		OPT_BOOL('u', "update-head-ok", &update_ਸਿਰ_ok,
			    N_("allow updating of HEAD ref")),
		OPT_BOOL(0, "progress", &ਤਰੱਕੀ, N_("force progress reporting")),
		OPT_STRING(0, "depth", &ਡੂੰਘਾਈ, N_("depth"),
			   N_("deepen history of shallow clone")),
		OPT_STRING(0, "shallow-since", &deepen_since, N_("time"),
			   N_("deepen history of shallow repository based on time")),
		OPT_STRING_LIST(0, "shallow-exclude", &deepen_not, N_("ref"),
				N_("deepen history of shallow clone, excluding ref")),
		OPT_INTEGER(0, "deepen", &deepen_relative,
			    N_("deepen history of shallow clone")),
		OPT_ਸੈੱਟ_INT_F(0, "unshallow", &unshallow,
			      N_("convert to a complete repository"),
			      1, PARSE_OPT_NONEG),
		OPT_ਸੈੱਟ_INT_F(0, "refetch", &refetch,
			      N_("re-fetch without negotiating common commits"),
			      1, PARSE_OPT_NONEG),
		{
			.type = OPTION_STRING,
			.long_name = "submodule-prefix",
			.ਮੁੱਲ = &submodule_ਅਗੇਤਰ,
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
		OPT_BOOL(0, "write-commit-graph", &fetch_write_commit_ਗ੍ਰਾਫ,
			 N_("write the commit-graph after fetching")),
		OPT_BOOL(0, "stdin", &stdin_refspecs,
			 N_("accept refspecs from stdin")),
		OPT_ਸਿਰਾ()
	};

	filter_options.allow_auto_filter = 1;

	refspec_init_fetch(&refmap, the_hash_algo);

	packet_trace_ਪਛਾਣ_ਤੱਤ("fetch");

	/* Record the command line for the reflog */
	strbuf_addstr(&default_rla, "fetch");
	ਲਈ (i = 1; i < argc; i++) {
		/* This handles non-URLs gracefully */
		ਅੱਖਰ *anon = transport_anonymize_url(argv[i]);

		strbuf_addf(&default_rla, " %s", anon);
		ਮੁਕਤ(anon);
	}

	repo_config(the_repository, git_fetch_config, &config);
	ਜੇ (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_ਭਰਿਆ_ਸੂਚਕਾਂਕ = 0;
	}

	argc = parse_options(argc, argv, ਅਗੇਤਰ,
			     builtin_fetch_options, builtin_fetch_usage, 0);

	ਜੇ (recurse_submodules_cli != RECURSE_SUBMODULES_DEFAULT)
		config.recurse_submodules = recurse_submodules_cli;

	ਜੇ (negotiate_only) {
		ਚੋਣ (recurse_submodules_cli) {
		ਹਾਲਤ RECURSE_SUBMODULES_OFF:
		ਹਾਲਤ RECURSE_SUBMODULES_DEFAULT:
			/*
			 * --negotiate-only should never recurse into
			 * submodules. Skip it by setting recurse_submodules to
			 * RECURSE_SUBMODULES_OFF.
			 */
			config.recurse_submodules = RECURSE_SUBMODULES_OFF;
			ਰੋਕੋ;

		ਮੂਲ:
			die(_("options '%s' and '%s' cannot be used together"),
			    "--negotiate-only", "--recurse-submodules");
		}
	}

	ਜੇ (config.recurse_submodules != RECURSE_SUBMODULES_OFF) {
		ਪੂਰਨ *sfjc = config.submodule_fetch_jobs == -1
			    ? &config.submodule_fetch_jobs : ਨਿਰਾ;
		ਪੂਰਨ *rs = config.recurse_submodules == RECURSE_SUBMODULES_DEFAULT
			  ? &config.recurse_submodules : ਨਿਰਾ;

		fetch_config_from_gitmodules(the_repository, sfjc, rs);
	}


	ਜੇ (porcelain) {
		ਚੋਣ (recurse_submodules_cli) {
		ਹਾਲਤ RECURSE_SUBMODULES_OFF:
		ਹਾਲਤ RECURSE_SUBMODULES_DEFAULT:
			/*
			 * Reference updates in submodules would be ambiguous
			 * in porcelain mode, so we reject this combination.
			 */
			config.recurse_submodules = RECURSE_SUBMODULES_OFF;
			ਰੋਕੋ;

		ਮੂਲ:
			die(_("options '%s' and '%s' cannot be used together"),
			    "--porcelain", "--recurse-submodules");
		}

		config.display_format = DISPLAY_FORMAT_PORCELAIN;
	}

	ਜੇ (deepen_relative) {
		ਜੇ (deepen_relative < 0)
			die(_("negative depth in --deepen is not supported"));
		ਜੇ (ਡੂੰਘਾਈ)
			die(_("options '%s' and '%s' cannot be used together"), "--deepen", "--depth");
		ਡੂੰਘਾਈ = xstrfmt("%d", deepen_relative);
	}
	ਜੇ (unshallow) {
		ਜੇ (ਡੂੰਘਾਈ)
			die(_("options '%s' and '%s' cannot be used together"), "--depth", "--unshallow");
		ਨਹੀਂਤਾਂ ਜੇ (!is_repository_shallow(the_repository))
			die(_("--unshallow on a complete repository does not make sense"));
		ਨਹੀਂਤਾਂ
			ਡੂੰਘਾਈ = xstrfmt("%d", INFINITE_ਡੂੰਘਾਈ);
	}

	/* no need to be strict, transport_set_option() will validate it again */
	ਜੇ (ਡੂੰਘਾਈ && atoi(ਡੂੰਘਾਈ) < 1)
		die(_("depth %s is not a positive number"), ਡੂੰਘਾਈ);
	ਜੇ (ਡੂੰਘਾਈ || deepen_since || deepen_not.nr)
		deepen = 1;

	/* FETCH_HEAD never gets updated in --dry-run mode */
	ਜੇ (dry_ਲੜੀਵਾਰ)
		write_fetch_ਸਿਰ = 0;

	ਜੇ (!max_jobs)
		max_jobs = online_cpus();

	ਜੇ (!repo_config_get_string_tmp(the_repository, "fetch.bundleuri", &bundle_uri) &&
	    fetch_bundle_uri(the_repository, bundle_uri, ਨਿਰਾ))
		warning(_("failed to fetch bundles from '%s'"), bundle_uri);

	ਜੇ (all < 0) {
		/*
		 * no --[no-]all given;
		 * only use config option if no remote was explicitly specified
		 */
		all = (!argc) ? config.all : 0;
	}

	ਜੇ (all) {
		ਜੇ (argc == 1)
			die(_("fetch --all does not take a repository argument"));
		ਨਹੀਂਤਾਂ ਜੇ (argc > 1)
			die(_("fetch --all does not make sense with refspecs"));

		(ਖਾਲੀ) for_each_remote(get_one_remote_for_fetch, &list);

		/* do not do fetch_multiple() of one */
		ਜੇ (list.nr == 1)
			remote = remote_get(list.ਚੀਜ਼ਾਂ[0].string);
	} ਨਹੀਂਤਾਂ ਜੇ (argc == 0) {
		/* No arguments -- use default remote */
		remote = remote_get(ਨਿਰਾ);
	} ਨਹੀਂਤਾਂ ਜੇ (ਗੁਣਜ) {
		/* All arguments are assumed to be remotes or groups */
		ਲਈ (i = 0; i < argc; i++)
			ਜੇ (!ਜੋੜੋ_remote_or_ਸਮੂਹ(argv[i], &list))
				die(_("no such remote or remote group: %s"),
				    argv[i]);
	} ਨਹੀਂਤਾਂ {
		/* Single remote or group */
		(ਖਾਲੀ) ਜੋੜੋ_remote_or_ਸਮੂਹ(argv[0], &list);
		ਜੇ (list.nr > 1) {
			/* More than one remote */
			ਜੇ (argc > 1)
				die(_("fetching a group and specifying refspecs does not make sense"));
		} ਨਹੀਂਤਾਂ {
			/* Zero or one remotes */
			remote = remote_get(argv[0]);
			prune_ਟੈਗ_ਸੂਚੀ_ok = (argc == 1);
			argc--;
			argv++;
		}
	}
	string_list_remove_duplicates(&list, 0);

	ਜੇ (negotiate_only) {
		ਬਣਤਰ oidset acked_commits = OIDSET_INIT;
		ਬਣਤਰ oidset_iter iter;
		ਅਚਲ ਬਣਤਰ object_id *oid;

		trace2_region_enter("fetch", "negotiate-only", the_repository);
		ਜੇ (!remote)
			die(_("must supply remote when using --negotiate-only"));
		gtransport = prepare_transport(remote, 1, &filter_options);

		ਜੇ (!gtransport->smart_options) {
			warning(_("protocol does not support --negotiate-only, exiting"));
			ਨਤੀਜਾ = 1;
			trace2_region_leave("fetch", "negotiate-only", the_repository);
			ਜਾਓ cleanup;
		}
		ਜੇ (!gtransport->smart_options->negotiation_restrict_tips)
			die(_("%s needs one or more %s"), "--negotiate-only",
			    "--negotiation-restrict=*");

		gtransport->smart_options->acked_commits = &acked_commits;

		ਜੇ (server_options.nr)
			gtransport->server_options = &server_options;
		ਨਤੀਜਾ = transport_fetch_refs(gtransport, ਨਿਰਾ);
		gtransport->smart_options->acked_commits = ਨਿਰਾ;

		oidset_iter_init(&acked_commits, &iter);
		ਜਦੋਂ ((oid = oidset_iter_ਅਗਲਾ(&iter)))
			ਲਿਖੋ("%s\n", oid_ਮੰਜ਼ਿਲ_hex(oid));
		oidset_clear(&acked_commits);
		trace2_region_leave("fetch", "negotiate-only", the_repository);
	} ਨਹੀਂਤਾਂ ਜੇ (remote) {
		ਜੇ (filter_options.choice || repo_has_promisor_remote(the_repository)) {
			trace2_region_enter("fetch", "setup-partial", the_repository);
			fetch_one_ਤਿਆਰੀ_partial(remote, &filter_options);
			trace2_region_leave("fetch", "setup-partial", the_repository);
		}
		trace2_region_enter("fetch", "fetch-one", the_repository);
		ਨਤੀਜਾ = fetch_one(remote, argc, argv, prune_ਟੈਗ_ਸੂਚੀ_ok, stdin_refspecs,
				   &config, &filter_options, &updated_tips);
		ਜੇ (prepare_commit_ਗ੍ਰਾਫ(the_repository)) {
			ਜੇ (oidset_ਮਾਪ(&updated_tips))
				ਗ੍ਰਾਫ_write_ਬਹੁਲਕ = ਗ੍ਰਾਫ_WRITE_TIPS;
			ਨਹੀਂਤਾਂ
				ਗ੍ਰਾਫ_write_ਬਹੁਲਕ = ਗ੍ਰਾਫ_WRITE_SKIP;
		}
		trace2_region_leave("fetch", "fetch-one", the_repository);
	} ਨਹੀਂਤਾਂ {
		ਪੂਰਨ max_children = max_jobs;

		ਜੇ (filter_options.choice)
			die(_("--filter can only be used with the remote "
			      "configured in extensions.partialclone"));

		ਜੇ (atomic_fetch)
			die(_("--atomic can only be used when fetching "
			      "from one remote"));

		ਜੇ (stdin_refspecs)
			die(_("--stdin can only be used when fetching "
			      "from one remote"));

		ਜੇ (max_children < 0)
			max_children = config.parallel;

		/* TODO should this also die if we have a previous partial-clone? */
		trace2_region_enter("fetch", "fetch-multiple", the_repository);
		ਨਤੀਜਾ = fetch_ਗੁਣਜ(&list, max_children, &config);
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
	ਜੇ (!ਨਤੀਜਾ && remote && (config.recurse_submodules != RECURSE_SUBMODULES_OFF)) {
		ਬਣਤਰ strvec options = STRVEC_INIT;
		ਪੂਰਨ max_children = max_jobs;

		ਜੇ (max_children < 0)
			max_children = config.submodule_fetch_jobs;
		ਜੇ (max_children < 0)
			max_children = config.parallel;

		ਜੋੜੋ_options_ਮੰਜ਼ਿਲ_argv(&options, &config);
		trace2_region_enter_printf("fetch", "recurse-submodule", the_repository, "%s", submodule_ਅਗੇਤਰ);
		ਨਤੀਜਾ = fetch_submodules(the_repository,
					  &options,
					  submodule_ਅਗੇਤਰ,
					  config.recurse_submodules,
					  recurse_submodules_default,
					  verbosity < 0,
					  max_children);
		trace2_region_leave_printf("fetch", "recurse-submodule", the_repository, "%s", submodule_ਅਗੇਤਰ);
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
	ਜੇ (negotiate_only)
		ਜਾਓ cleanup;

	prepare_repo_settings(the_repository);
	ਜੇ (fetch_write_commit_ਗ੍ਰਾਫ > 0 ||
	    (fetch_write_commit_ਗ੍ਰਾਫ < 0 &&
	     the_repository->settings.fetch_write_commit_ਗ੍ਰਾਫ)) {
		ਪੂਰਨ commit_ਗ੍ਰਾਫ_flags = COMMIT_ਗ੍ਰਾਫ_WRITE_ਵੰਡੋ;

		ਜੇ (ਤਰੱਕੀ)
			commit_ਗ੍ਰਾਫ_flags |= COMMIT_ਗ੍ਰਾਫ_WRITE_ਤਰੱਕੀ;

		ਜੇ (ਗ੍ਰਾਫ_write_ਬਹੁਲਕ != ਗ੍ਰਾਫ_WRITE_SKIP) {
			trace2_region_enter("fetch", "write-commit-graph",
					    the_repository);
			ਜੇ (ਗ੍ਰਾਫ_write_ਬਹੁਲਕ == ਗ੍ਰਾਫ_WRITE_TIPS)
				write_commit_ਗ੍ਰਾਫ(
					the_repository,
					the_repository->objects->ਸਰੋਤ->ਰਸਤਾ,
					ਨਿਰਾ, &updated_tips,
					commit_ਗ੍ਰਾਫ_flags, ਨਿਰਾ);
			ਨਹੀਂਤਾਂ
				write_commit_ਗ੍ਰਾਫ_reachable(
					the_repository,
					the_repository->objects->ਸਰੋਤ->ਰਸਤਾ,
					commit_ਗ੍ਰਾਫ_flags, ਨਿਰਾ);
			trace2_region_leave("fetch", "write-commit-graph",
					    the_repository);
		}
	}

	ਜੇ (enable_auto_gc) {
		ਜੇ (refetch) {
			/*
			 * Hint auto-maintenance strongly to encourage repacking,
			 * but respect config settings disabling it.
			 */
			ਪੂਰਨ opt_val;
			ਜੇ (repo_config_get_int(the_repository, "gc.autopacklimit", &opt_val))
				opt_val = -1;
			ਜੇ (opt_val != 0)
				git_config_ਧੱਕੋ_parameter("gc.autoPackLimit=1");

			ਜੇ (repo_config_get_int(the_repository, "maintenance.incremental-repack.auto", &opt_val))
				opt_val = -1;
			ਜੇ (opt_val != 0)
				git_config_ਧੱਕੋ_parameter("maintenance.incremental-repack.auto=-1");
		}
		ਲੜੀਵਾਰ_auto_maintenance(the_repository, verbosity < 0);
	}

 cleanup:
	oidset_clear(&updated_tips);
	string_list_clear(&list, 0);
	list_objects_filter_release(&filter_options);
	ਮੁਕਤ(config.follow_remote_ਸਿਰ_raw);
	ਵਾਪਸ ਨਤੀਜਾ;
}
