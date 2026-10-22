/*
 * "git fetch"
 */

#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE
#ವ್ಯಾಖ್ಯಾನಿಸು DISABLE_SIGN_COMPARE_WARNINGS

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

#ವ್ಯಾಖ್ಯಾನಿಸು FORCED_UPDATES_DELAY_WARNING_ಒಳ_MS (10 * 1000)

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ builtin_fetch_usage[] = {
	N_("git fetch [<options>] [<repository> [<refspec>...]]"),
	N_("git fetch [<options>] <group>"),
	N_("git fetch --multiple [<options>] [(<repository>|<group>)...]"),
	N_("git fetch --all [<options>]"),
	ಖಾಲಿ
};

ಎಣಿಕೆ {
	ಟ್ಯಾಗುಗಳು_UNSET = 0,
	ಟ್ಯಾಗುಗಳು_DEFAULT = 1,
	ಟ್ಯಾಗುಗಳು_ಗಣ = 2
};

ಎಣಿಕೆ display_format {
	DISPLAY_FORMAT_ತುಂಬಿದ,
	DISPLAY_FORMAT_COMPACT,
	DISPLAY_FORMAT_PORCELAIN,
};

ರಚನೆ display_state {
	ರಚನೆ strbuf buf;

	ಪೂರ್ಣಾಂಕ refcol_ಅಗಲ;
	ಎಣಿಕೆ display_format format;

	ಅಕ್ಷರ *url;
	ಪೂರ್ಣಾಂಕ url_len, shown_url;
};

ಸ್ಥಿರ uint64_t forced_updates_ms = 0;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ prefetch = 0;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ prune = -1; /* unspecified */
#ವ್ಯಾಖ್ಯಾನಿಸು PRUNE_BY_DEFAULT 0 /* do we prune by default? */

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ prune_ಟ್ಯಾಗುಗಳು = -1; /* unspecified */
#ವ್ಯಾಖ್ಯಾನಿಸು PRUNE_ಟ್ಯಾಗುಗಳು_BY_DEFAULT 0 /* do we prune tags by default? */

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ append, dry_ಸತತ, force, keep, update_ತಲೆ_ok;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ write_fetch_ತಲೆ = 1;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ verbosity, deepen_relative, ಗಣ_upstream, refetch;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಪ್ರಗತಿ = -1;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಟ್ಯಾಗುಗಳು = ಟ್ಯಾಗುಗಳು_DEFAULT, update_shallow, deepen;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ atomic_fetch;
ಸ್ಥಿರ ಎಣಿಕೆ transport_family family;
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಆಳ;
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *deepen_since;
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *upload_pack;
ಸ್ಥಿರ ರಚನೆ string_list deepen_not = STRING_LIST_INIT_NODUP;
ಸ್ಥಿರ ರಚನೆ strbuf default_rla = STRBUF_INIT;
ಸ್ಥಿರ ರಚನೆ transport *gtransport;
ಸ್ಥಿರ ರಚನೆ transport *gsecondary;
ಸ್ಥಿರ ರಚನೆ refspec refmap;
ಸ್ಥಿರ ರಚನೆ string_list server_options = STRING_LIST_INIT_DUP;
ಸ್ಥಿರ ರಚನೆ string_list negotiation_restrict = STRING_LIST_INIT_NODUP;
ಸ್ಥಿರ ರಚನೆ string_list negotiation_include = STRING_LIST_INIT_NODUP;

ರಚನೆ fetch_config {
	ಎಣಿಕೆ display_format display_format;
	ಅಕ್ಷರ *follow_remote_ತಲೆ_raw;
	ಪೂರ್ಣಾಂಕ follow_remote_ತಲೆ_ನೋಡಿದ;
	ಪೂರ್ಣಾಂಕ all;
	ಪೂರ್ಣಾಂಕ prune;
	ಪೂರ್ಣಾಂಕ prune_ಟ್ಯಾಗುಗಳು;
	ಪೂರ್ಣಾಂಕ show_forced_updates;
	ಪೂರ್ಣಾಂಕ recurse_submodules;
	ಪೂರ್ಣಾಂಕ parallel;
	ಪೂರ್ಣಾಂಕ submodule_fetch_jobs;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ git_fetch_config(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *k, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *v,
			    ಸ್ಥಿರಾಂಕ ರಚನೆ config_context *ctx, ಶೂನ್ಯ *cb)
{
	ರಚನೆ fetch_config *fetch_config = cb;

	ಆದರೆ (!strcmp(k, "fetch.all")) {
		fetch_config->all = git_config_bool(k, v);
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (!strcmp(k, "fetch.prune")) {
		fetch_config->prune = git_config_bool(k, v);
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (!strcmp(k, "fetch.prunetags")) {
		fetch_config->prune_ಟ್ಯಾಗುಗಳು = git_config_bool(k, v);
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (!strcmp(k, "fetch.showforcedupdates")) {
		fetch_config->show_forced_updates = git_config_bool(k, v);
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (!strcmp(k, "submodule.recurse")) {
		ಪೂರ್ಣಾಂಕ r = git_config_bool(k, v) ?
			RECURSE_SUBMODULES_ON : RECURSE_SUBMODULES_OFF;
		fetch_config->recurse_submodules = r;
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (!strcmp(k, "submodule.fetchjobs")) {
		fetch_config->submodule_fetch_jobs = parse_submodule_fetchjobs(k, v, ctx->kvi);
		ಹಿಂತಿರುಗಿ 0;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(k, "fetch.recursesubmodules")) {
		fetch_config->recurse_submodules = parse_fetch_recurse_submodules_arg(k, v);
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (!strcmp(k, "fetch.parallel")) {
		fetch_config->parallel = git_config_int(k, v, ctx->kvi);
		ಆದರೆ (fetch_config->parallel < 0)
			die(_("fetch.parallel cannot be negative"));
		ಆದರೆ (!fetch_config->parallel)
			fetch_config->parallel = online_cpus();
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (!strcmp(k, "fetch.output")) {
		ಆದರೆ (!v)
			ಹಿಂತಿರುಗಿ config_error_nonbool(k);
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcasecmp(v, "full"))
			fetch_config->display_format = DISPLAY_FORMAT_ತುಂಬಿದ;
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcasecmp(v, "compact"))
			fetch_config->display_format = DISPLAY_FORMAT_COMPACT;
		ಇಲ್ಲದಿದ್ದರೆ
			die(_("invalid value for '%s': '%s'"),
			    "fetch.output", v);
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (!strcmp(k, "fetch.followremotehead")) {
		ಬಿಡುಗಡೆ(fetch_config->follow_remote_ತಲೆ_raw);
		fetch_config->follow_remote_ತಲೆ_raw = xstrdup_or_null(v);
		fetch_config->follow_remote_ತಲೆ_ನೋಡಿದ = 1;
		ಹಿಂತಿರುಗಿ 0;
	}

	ಹಿಂತಿರುಗಿ git_default_config(k, v, ctx, cb);
}

ಸ್ಥಿರ ಎಣಿಕೆ follow_remote_ತಲೆ_settings get_follow_remote_ತಲೆ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *setting)
{
	ಆದರೆ (!setting)
		die(_("missing value for 'fetch.followRemoteHEAD'"));
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(setting, "never"))
		ಹಿಂತಿರುಗಿ FOLLOW_REMOTE_NEVER;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(setting, "create"))
		ಹಿಂತಿರುಗಿ FOLLOW_REMOTE_CREATE;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(setting, "warn"))
		ಹಿಂತಿರುಗಿ FOLLOW_REMOTE_WARN;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(setting, "always"))
		ಹಿಂತಿರುಗಿ FOLLOW_REMOTE_ALWAYS;
	warning(_("unrecognized fetch.followRemoteHEAD value '%s' ignored"), setting);
	ಹಿಂತಿರುಗಿ BUILTIN_FOLLOW_REMOTE_ತಲೆ_DFLT;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ parse_refmap_arg(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	BUG_ON_OPT_NEG(unset);

	/*
	 * "git fetch --refmap='' origin foo"
	 * can be used to tell the command not to store anywhere
	 */
	refspec_append(opt->ಮೌಲ್ಯ, arg);

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ unlock_pack(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flags)
{
	ಆದರೆ (gtransport)
		transport_unlock_pack(gtransport, flags);
	ಆದರೆ (gsecondary)
		transport_unlock_pack(gsecondary, flags);
}

ಸ್ಥಿರ ಶೂನ್ಯ unlock_pack_atexit(ಶೂನ್ಯ)
{
	unlock_pack(0);
}

ಸ್ಥಿರ ಶೂನ್ಯ unlock_pack_on_signal(ಪೂರ್ಣಾಂಕ signo)
{
	unlock_pack(TRANSPORT_UNLOCK_PACK_ಒಳ_SIGNAL_HANDLER);
	sigchain_ತೆಗೆ(signo);
	ಎತ್ತು(signo);
}

ಸ್ಥಿರ ಶೂನ್ಯ ಕೂಡಿಸು_ವಿಲೀನ_config(ರಚನೆ ref **ತಲೆ,
			   ಸ್ಥಿರಾಂಕ ರಚನೆ ref *remote_refs,
		           ರಚನೆ branch *branch,
		           ರಚನೆ ref ***ಬಾಲ)
{
	ಪೂರ್ಣಾಂಕ i;

	ಗಾಗಿ (i = 0; i < branch->ವಿಲೀನ_nr; i++) {
		ರಚನೆ ref *rm, **old_ಬಾಲ = *ಬಾಲ;
		ರಚನೆ refspec_ಅಂಶ refspec;

		ಗಾಗಿ (rm = *ತಲೆ; rm; rm = rm->ಮುಂದಿನ) {
			ಆದರೆ (branch_ವಿಲೀನ_matches(branch, i, rm->name)) {
				rm->fetch_ತಲೆ_ಸ್ಥಿತಿ = FETCH_ತಲೆ_ವಿಲೀನ;
				ನಿಲ್ಲಿಸು;
			}
		}
		ಆದರೆ (rm)
			ಮುಂದುವರಿ;

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
		memset(&refspec, 0, ಗಾತ್ರ(refspec));
		refspec.src = branch->ವಿಲೀನ[i]->src;
		get_fetch_map(remote_refs, &refspec, ಬಾಲ, 1);
		ಗಾಗಿ (rm = *old_ಬಾಲ; rm; rm = rm->ಮುಂದಿನ)
			rm->fetch_ತಲೆ_ಸ್ಥಿತಿ = FETCH_ತಲೆ_ವಿಲೀನ;
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ create_fetch_oidset(ರಚನೆ ref **ತಲೆ, ರಚನೆ oidset *ಹೊರಹೋಗು)
{
	ರಚನೆ ref *rm = *ತಲೆ;
	ಆಗ (rm) {
		oidset_ಸೇರಿಸು(ಹೊರಹೋಗು, &rm->old_oid);
		rm = rm->ಮುಂದಿನ;
	}
}

ರಚನೆ refname_hash_ನಮೂದು {
	ರಚನೆ hashmap_ನಮೂದು ent;
	ರಚನೆ object_id oid;
	ಪೂರ್ಣಾಂಕ ignore;
	ಅಕ್ಷರ refname[FLEX_ARRAY];
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ refname_hash_ನಮೂದು_cmp(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *hashmap_cmp_fn_ದತ್ತಾಂಶ ಬಳಸದ,
				  ಸ್ಥಿರಾಂಕ ರಚನೆ hashmap_ನಮೂದು *eptr,
				  ಸ್ಥಿರಾಂಕ ರಚನೆ hashmap_ನಮೂದು *ನಮೂದು_or_ಕೀಲಿ,
				  ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *keydata)
{
	ಸ್ಥಿರಾಂಕ ರಚನೆ refname_hash_ನಮೂದು *e1, *e2;

	e1 = container_ದ(eptr, ಸ್ಥಿರಾಂಕ ರಚನೆ refname_hash_ನಮೂದು, ent);
	e2 = container_ದ(ನಮೂದು_or_ಕೀಲಿ, ಸ್ಥಿರಾಂಕ ರಚನೆ refname_hash_ನಮೂದು, ent);
	ಹಿಂತಿರುಗಿ strcmp(e1->refname, keydata ? keydata : e2->refname);
}

ಸ್ಥಿರ ರಚನೆ refname_hash_ನಮೂದು *refname_hash_ಕೂಡಿಸು(ರಚನೆ hashmap *map,
						   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *refname,
						   ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid)
{
	ರಚನೆ refname_hash_ನಮೂದು *ent;
	ಗಾತ್ರ_t len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(refname);

	FLEX_ALLOC_MEM(ent, refname, refname, len);
	hashmap_ನಮೂದು_init(&ent->ent, strhash(refname));
	oidcpy(&ent->oid, oid);
	hashmap_ಕೂಡಿಸು(map, &ent->ent);
	ಹಿಂತಿರುಗಿ ent;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಕೂಡಿಸು_one_refname(ಸ್ಥಿರಾಂಕ ರಚನೆ reference *ref, ಶೂನ್ಯ *cbdata)
{
	ರಚನೆ hashmap *refname_map = cbdata;

	(ಶೂನ್ಯ) refname_hash_ಕೂಡಿಸು(refname_map, ref->name, ref->oid);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ refname_hash_init(ರಚನೆ hashmap *map)
{
	hashmap_init(map, refname_hash_ನಮೂದು_cmp, ಖಾಲಿ, 0);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ refname_hash_exists(ರಚನೆ hashmap *map, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *refname)
{
	ಹಿಂತಿರುಗಿ !!hashmap_get_from_hash(map, strhash(refname), refname);
}

ಸ್ಥಿರ ಶೂನ್ಯ clear_ಅಂಶ(ರಚನೆ refname_hash_ನಮೂದು *ಅಂಶ)
{
	ಅಂಶ->ignore = 1;
}


ಸ್ಥಿರ ಶೂನ್ಯ ಕೂಡಿಸು_already_queued_ಟ್ಯಾಗುಗಳು(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *refname,
				    ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *old_oid ಬಳಸದ,
				    ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *new_oid,
				    ಶೂನ್ಯ *cb_ದತ್ತಾಂಶ)
{
	ರಚನೆ hashmap *queued_ಟ್ಯಾಗುಗಳು = cb_ದತ್ತಾಂಶ;
	ಆದರೆ (starts_with(refname, "refs/tags/") && new_oid)
		(ಶೂನ್ಯ) refname_hash_ಕೂಡಿಸು(queued_ಟ್ಯಾಗುಗಳು, refname, new_oid);
}

ಸ್ಥಿರ ಶೂನ್ಯ ಕಂಡುಹಿಡಿ_non_local_ಟ್ಯಾಗುಗಳು(ಸ್ಥಿರಾಂಕ ರಚನೆ ref *refs,
				ರಚನೆ ref_transaction *transaction,
				ರಚನೆ ref **ತಲೆ,
				ರಚನೆ ref ***ಬಾಲ)
{
	ರಚನೆ hashmap existing_refs;
	ರಚನೆ hashmap remote_refs;
	ರಚನೆ oidset fetch_oids = OIDSET_INIT;
	ರಚನೆ string_list remote_refs_list = STRING_LIST_INIT_NODUP;
	ರಚನೆ string_list_ಅಂಶ *remote_ref_ಅಂಶ;
	ಸ್ಥಿರಾಂಕ ರಚನೆ ref *ref;
	ರಚನೆ refname_hash_ನಮೂದು *ಅಂಶ = ಖಾಲಿ;

	refname_hash_init(&existing_refs);
	refname_hash_init(&remote_refs);
	create_fetch_oidset(ತಲೆ, &fetch_oids);

	refs_for_each_ref(get_main_ref_store(the_repository), ಕೂಡಿಸು_one_refname,
			  &existing_refs);

	/*
	 * If we already have a transaction, then we need to filter out all
	 * tags which have already been queued up.
	 */
	ಆದರೆ (transaction)
		ref_transaction_for_each_queued_update(transaction,
						       ಕೂಡಿಸು_already_queued_ಟ್ಯಾಗುಗಳು,
						       &existing_refs);

	ಗಾಗಿ (ref = refs; ref; ref = ref->ಮುಂದಿನ) {
		ಆದರೆ (!starts_with(ref->name, "refs/tags/"))
			ಮುಂದುವರಿ;

		/*
		 * The peeled ref always follows the matching base
		 * ref, so if we see a peeled ref that we don't want
		 * to fetch then we can mark the ref entry in the list
		 * as one to ignore by setting util to NULL.
		 */
		ಆದರೆ (ತುದಿಗಳು_with(ref->name, "^{}")) {
			ಆದರೆ (ಅಂಶ &&
			    !odb_has_object(the_repository->objects, &ref->old_oid, 0) &&
			    !oidset_ಒಳಗೊಂಡಿದೆ(&fetch_oids, &ref->old_oid) &&
			    !odb_has_object(the_repository->objects, &ಅಂಶ->oid, 0) &&
			    !oidset_ಒಳಗೊಂಡಿದೆ(&fetch_oids, &ಅಂಶ->oid))
				clear_ಅಂಶ(ಅಂಶ);
			ಅಂಶ = ಖಾಲಿ;
			ಮುಂದುವರಿ;
		}

		/*
		 * If item is non-NULL here, then we previously saw a
		 * ref not followed by a peeled reference, so we need
		 * to check if it is a lightweight tag that we want to
		 * fetch.
		 */
		ಆದರೆ (ಅಂಶ &&
		    !odb_has_object(the_repository->objects, &ಅಂಶ->oid, 0) &&
		    !oidset_ಒಳಗೊಂಡಿದೆ(&fetch_oids, &ಅಂಶ->oid))
			clear_ಅಂಶ(ಅಂಶ);

		ಅಂಶ = ಖಾಲಿ;

		/* skip duplicates and refs that we already have */
		ಆದರೆ (refname_hash_exists(&remote_refs, ref->name) ||
		    refname_hash_exists(&existing_refs, ref->name))
			ಮುಂದುವರಿ;

		ಅಂಶ = refname_hash_ಕೂಡಿಸು(&remote_refs, ref->name, &ref->old_oid);
		string_list_ಸೇರಿಸು(&remote_refs_list, ref->name);
	}
	hashmap_clear_and_free(&existing_refs, ರಚನೆ refname_hash_ನಮೂದು, ent);

	/*
	 * We may have a final lightweight tag that needs to be
	 * checked to see if it needs fetching.
	 */
	ಆದರೆ (ಅಂಶ &&
	    !odb_has_object(the_repository->objects, &ಅಂಶ->oid, 0) &&
	    !oidset_ಒಳಗೊಂಡಿದೆ(&fetch_oids, &ಅಂಶ->oid))
		clear_ಅಂಶ(ಅಂಶ);

	/*
	 * For all the tags in the remote_refs_list,
	 * add them to the list of refs to be fetched
	 */
	for_each_string_list_ಅಂಶ(remote_ref_ಅಂಶ, &remote_refs_list) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *refname = remote_ref_ಅಂಶ->string;
		ರಚನೆ ref *rm;
		ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ hash = strhash(refname);

		ಅಂಶ = hashmap_get_ನಮೂದು_from_hash(&remote_refs, hash, refname,
					ರಚನೆ refname_hash_ನಮೂದು, ent);
		ಆದರೆ (!ಅಂಶ)
			BUG("unseen remote ref?");

		/* Unless we have already decided to ignore this item... */
		ಆದರೆ (ಅಂಶ->ignore)
			ಮುಂದುವರಿ;

		rm = alloc_ref(ಅಂಶ->refname);
		rm->peer_ref = alloc_ref(ಅಂಶ->refname);
		oidcpy(&rm->old_oid, &ಅಂಶ->oid);
		**ಬಾಲ = rm;
		*ಬಾಲ = &rm->ಮುಂದಿನ;
	}
	hashmap_clear_and_free(&remote_refs, ರಚನೆ refname_hash_ನಮೂದು, ent);
	string_list_clear(&remote_refs_list, 0);
	oidset_clear(&fetch_oids);
}

ಸ್ಥಿರ ಶೂನ್ಯ filter_prefetch_refspec(ರಚನೆ refspec *rs)
{
	ಪೂರ್ಣಾಂಕ i;

	ಆದರೆ (!prefetch)
		ಹಿಂತಿರುಗಿ;

	ಗಾಗಿ (i = 0; i < rs->nr; i++) {
		ರಚನೆ strbuf new_dst = STRBUF_INIT;
		ಅಕ್ಷರ *old_dst;
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *sub = ಖಾಲಿ;

		ಆದರೆ (rs->ವಸ್ತುಗಳು[i].negative)
			ಮುಂದುವರಿ;
		ಆದರೆ (!rs->ವಸ್ತುಗಳು[i].dst ||
		    (rs->ವಸ್ತುಗಳು[i].src &&
		     starts_with(rs->ವಸ್ತುಗಳು[i].src,
				 ref_namespace[NAMESPACE_ಟ್ಯಾಗುಗಳು].ref))) {
			ಪೂರ್ಣಾಂಕ j;

			refspec_ಅಂಶ_clear(&rs->ವಸ್ತುಗಳು[i]);

			ಗಾಗಿ (j = i + 1; j < rs->nr; j++)
				rs->ವಸ್ತುಗಳು[j - 1] = rs->ವಸ್ತುಗಳು[j];
			rs->nr--;
			i--;
			ಮುಂದುವರಿ;
		}

		old_dst = rs->ವಸ್ತುಗಳು[i].dst;
		strbuf_addstr(&new_dst, ref_namespace[NAMESPACE_PREFETCH].ref);

		/*
		 * If old_dst starts with "refs/", then place
		 * sub after that prefix. Otherwise, start at
		 * the beginning of the string.
		 */
		ಆದರೆ (!skip_ಪೂರ್ವಪ್ರತ್ಯಯ(old_dst, "refs/", &sub))
			sub = old_dst;
		strbuf_addstr(&new_dst, sub);

		rs->ವಸ್ತುಗಳು[i].dst = strbuf_detach(&new_dst, ಖಾಲಿ);
		rs->ವಸ್ತುಗಳು[i].force = 1;

		ಬಿಡುಗಡೆ(old_dst);
	}
}

ಸ್ಥಿರ ರಚನೆ ref *get_ref_map(ರಚನೆ remote *remote,
			       ಸ್ಥಿರಾಂಕ ರಚನೆ ref *remote_refs,
			       ರಚನೆ refspec *rs,
			       ಪೂರ್ಣಾಂಕ ಟ್ಯಾಗುಗಳು, ಪೂರ್ಣಾಂಕ *autotags)
{
	ಪೂರ್ಣಾಂಕ i;
	ರಚನೆ ref *rm;
	ರಚನೆ ref *ref_map = ಖಾಲಿ;
	ರಚನೆ ref **ಬಾಲ = &ref_map;

	/* opportunistically-updated references: */
	ರಚನೆ ref *orefs = ಖಾಲಿ, **oref_ಬಾಲ = &orefs;

	ರಚನೆ hashmap existing_refs;
	ಪೂರ್ಣಾಂಕ existing_refs_populated = 0;

	filter_prefetch_refspec(rs);
	ಆದರೆ (remote)
		filter_prefetch_refspec(&remote->fetch);

	ಆದರೆ (rs->nr) {
		ರಚನೆ refspec *fetch_refspec;

		ಗಾಗಿ (i = 0; i < rs->nr; i++) {
			get_fetch_map(remote_refs, &rs->ವಸ್ತುಗಳು[i], &ಬಾಲ, 0);
			ಆದರೆ (rs->ವಸ್ತುಗಳು[i].dst && rs->ವಸ್ತುಗಳು[i].dst[0])
				*autotags = 1;
		}
		/* Merge everything on the command line (but not --tags) */
		ಗಾಗಿ (rm = ref_map; rm; rm = rm->ಮುಂದಿನ)
			rm->fetch_ತಲೆ_ಸ್ಥಿತಿ = FETCH_ತಲೆ_ವಿಲೀನ;

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
		ಆದರೆ (refmap.nr)
			fetch_refspec = &refmap;
		ಇಲ್ಲದಿದ್ದರೆ
			fetch_refspec = &remote->fetch;

		ಗಾಗಿ (i = 0; i < fetch_refspec->nr; i++)
			get_fetch_map(ref_map, &fetch_refspec->ವಸ್ತುಗಳು[i], &oref_ಬಾಲ, 1);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (refmap.nr) {
		die("--refmap option is only meaningful with command-line refspec(s)");
	} ಇಲ್ಲದಿದ್ದರೆ {
		/* Use the defaults */
		ರಚನೆ branch *branch = branch_get(ಖಾಲಿ);
		ಪೂರ್ಣಾಂಕ has_ವಿಲೀನ = branch_has_ವಿಲೀನ_config(branch);
		ಆದರೆ (remote &&
		    (remote->fetch.nr ||
		     /* Note: has_merge implies non-NULL branch->remote_name */
		     (has_ವಿಲೀನ && !strcmp(branch->remote_name, remote->name)))) {
			ಗಾಗಿ (i = 0; i < remote->fetch.nr; i++) {
				get_fetch_map(remote_refs, &remote->fetch.ವಸ್ತುಗಳು[i], &ಬಾಲ, 0);
				ಆದರೆ (remote->fetch.ವಸ್ತುಗಳು[i].dst &&
				    remote->fetch.ವಸ್ತುಗಳು[i].dst[0])
					*autotags = 1;
				ಆದರೆ (!i && !has_ವಿಲೀನ && ref_map &&
				    !remote->fetch.ವಸ್ತುಗಳು[0].ನಮೂನೆ)
					ref_map->fetch_ತಲೆ_ಸ್ಥಿತಿ = FETCH_ತಲೆ_ವಿಲೀನ;
			}
			/*
			 * if the remote we're fetching from is the same
			 * as given in branch.<name>.remote, we add the
			 * ref given in branch.<name>.merge, too.
			 *
			 * Note: has_merge implies non-NULL branch->remote_name
			 */
			ಆದರೆ (has_ವಿಲೀನ &&
			    !strcmp(branch->remote_name, remote->name))
				ಕೂಡಿಸು_ವಿಲೀನ_config(&ref_map, remote_refs, branch, &ಬಾಲ);
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!prefetch) {
			ref_map = get_remote_ref(remote_refs, "HEAD");
			ಆದರೆ (!ref_map)
				die(_("couldn't find remote ref HEAD"));
			ref_map->fetch_ತಲೆ_ಸ್ಥಿತಿ = FETCH_ತಲೆ_ವಿಲೀನ;
			ಬಾಲ = &ref_map->ಮುಂದಿನ;
		}
	}

	ಆದರೆ (ಟ್ಯಾಗುಗಳು == ಟ್ಯಾಗುಗಳು_ಗಣ) {
		ರಚನೆ refspec_ಅಂಶ ಟ್ಯಾಗ್_refspec;

		/* also fetch all tags */
		refspec_ಅಂಶ_init_ತಳ್ಳು(&ಟ್ಯಾಗ್_refspec, ಟ್ಯಾಗ್_REFSPEC,
				       the_hash_algo);
		get_fetch_map(remote_refs, &ಟ್ಯಾಗ್_refspec, &ಬಾಲ, 0);
		refspec_ಅಂಶ_clear(&ಟ್ಯಾಗ್_refspec);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಟ್ಯಾಗುಗಳು == ಟ್ಯಾಗುಗಳು_DEFAULT && *autotags) {
		ಕಂಡುಹಿಡಿ_non_local_ಟ್ಯಾಗುಗಳು(remote_refs, ಖಾಲಿ, &ref_map, &ಬಾಲ);
	}

	/* Now append any refs to be updated opportunistically: */
	*ಬಾಲ = orefs;
	ಗಾಗಿ (rm = orefs; rm; rm = rm->ಮುಂದಿನ) {
		rm->fetch_ತಲೆ_ಸ್ಥಿತಿ = FETCH_ತಲೆ_IGNORE;
		ಬಾಲ = &rm->ಮುಂದಿನ;
	}

	/*
	 * apply negative refspecs first, before we remove duplicates. This is
	 * necessary as negative refspecs might remove an otherwise conflicting
	 * duplicate.
	 */
	ಆದರೆ (rs->nr)
		ref_map = apply_negative_refspecs(ref_map, rs);
	ಇಲ್ಲದಿದ್ದರೆ
		ref_map = apply_negative_refspecs(ref_map, &remote->fetch);

	ref_map = ref_remove_duplicates(ref_map);

	ಗಾಗಿ (rm = ref_map; rm; rm = rm->ಮುಂದಿನ) {
		ಆದರೆ (rm->peer_ref) {
			ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *refname = rm->peer_ref->name;
			ರಚನೆ refname_hash_ನಮೂದು *peer_ಅಂಶ;
			ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ hash = strhash(refname);

			ಆದರೆ (!existing_refs_populated) {
				refname_hash_init(&existing_refs);
				refs_for_each_ref(get_main_ref_store(the_repository),
						  ಕೂಡಿಸು_one_refname,
						  &existing_refs);
				existing_refs_populated = 1;
			}

			peer_ಅಂಶ = hashmap_get_ನಮೂದು_from_hash(&existing_refs,
						hash, refname,
						ರಚನೆ refname_hash_ನಮೂದು, ent);
			ಆದರೆ (peer_ಅಂಶ) {
				ರಚನೆ object_id *old_oid = &peer_ಅಂಶ->oid;
				oidcpy(&rm->peer_ref->old_oid, old_oid);
			}
		}
	}
	ಆದರೆ (existing_refs_populated)
		hashmap_clear_and_free(&existing_refs, ರಚನೆ refname_hash_ನಮೂದು, ent);

	ಹಿಂತಿರುಗಿ ref_map;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ s_update_ref(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *action,
			ರಚನೆ ref *ref,
			ರಚನೆ ref_transaction *transaction,
			ಪೂರ್ಣಾಂಕ check_old)
{
	ಅಕ್ಷರ *ಸಂದೇಶ;
	ಅಕ್ಷರ *rla = getenv("GIT_REFLOG_ACTION");
	ರಚನೆ strbuf err = STRBUF_INIT;
	ಪೂರ್ಣಾಂಕ ret;

	ಆದರೆ (dry_ಸತತ)
		ಹಿಂತಿರುಗಿ 0;
	ಆದರೆ (!rla)
		rla = default_rla.buf;
	ಸಂದೇಶ = xstrfmt("%s: %s", rla, action);

	ret = ref_transaction_update(transaction, ref->name, &ref->new_oid,
				     check_old ? &ref->old_oid : ಖಾಲಿ,
				     ಖಾಲಿ, ಖಾಲಿ, 0, ಸಂದೇಶ, &err);

	ಆದರೆ (ret)
		error("%s", err.buf);
	strbuf_release(&err);
	ಬಿಡುಗಡೆ(ಸಂದೇಶ);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ refcol_ಅಗಲ(ಸ್ಥಿರಾಂಕ ರಚನೆ ref *ref_map, ಪೂರ್ಣಾಂಕ compact_format)
{
	ಸ್ಥಿರಾಂಕ ರಚನೆ ref *ref;
	ಪೂರ್ಣಾಂಕ max, ಅಗಲ = 10;

	max = ಪದ_columns();
	ಆದರೆ (compact_format)
		max = max * 2 / 3;

	ಗಾಗಿ (ref = ref_map; ref; ref = ref->ಮುಂದಿನ) {
		ಪೂರ್ಣಾಂಕ rlen, llen = 0, len;

		ಆದರೆ (ref->ಸ್ಥಿತಿ_2 == REF_ಸ್ಥಿತಿ_REJECT_SHALLOW ||
		    !ref->peer_ref ||
		    !strcmp(ref->name, "HEAD"))
			ಮುಂದುವರಿ;

		/* uptodate lines are only shown on high verbosity level */
		ಆದರೆ (verbosity <= 0 && oideq(&ref->peer_ref->old_oid, &ref->old_oid))
			ಮುಂದುವರಿ;

		rlen = utf8_strwidth(prettify_refname(ref->name));
		ಆದರೆ (!compact_format)
			llen = utf8_strwidth(prettify_refname(ref->peer_ref->name));

		/*
		 * rough estimation to see if the output line is too long and
		 * should not be counted (we can't do precise calculation
		 * anyway because we don't know if the error explanation part
		 * will be printed in update_local_ref)
		 */
		len = 21 /* flag and summary */ + rlen + 4 /* -> */ + llen;
		ಆದರೆ (len >= max)
			ಮುಂದುವರಿ;

		ಆದರೆ (ಅಗಲ < rlen)
			ಅಗಲ = rlen;
	}

	ಹಿಂತಿರುಗಿ ಅಗಲ;
}

ಸ್ಥಿರ ಶೂನ್ಯ display_state_init(ರಚನೆ display_state *display_state, ರಚನೆ ref *ref_map,
			       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *raw_url, ಎಣಿಕೆ display_format format)
{
	ಪೂರ್ಣಾಂಕ i;

	memset(display_state, 0, ಗಾತ್ರ(*display_state));
	strbuf_init(&display_state->buf, 0);
	display_state->format = format;

	ಆದರೆ (raw_url)
		display_state->url = transport_anonymize_url(raw_url);
	ಇಲ್ಲದಿದ್ದರೆ
		display_state->url = xstrdup("foreign");

	display_state->url_len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(display_state->url);
	ಗಾಗಿ (i = display_state->url_len - 1; 0 <= i && display_state->url[i] == '/'; i--)
		;
	display_state->url_len = i + 1;
	ಆದರೆ (4 < i && !ಅಕ್ಷರಸರಣಿ_ಹೋಲಿಕೆ(".git", display_state->url + i - 3, 4))
		display_state->url_len = i - 3;

	ಆದರೆ (verbosity < 0)
		ಹಿಂತಿರುಗಿ;

	ಆಯ್ಕೆ (display_state->format) {
	ಸ್ಥಿತಿ DISPLAY_FORMAT_ತುಂಬಿದ:
	ಸ್ಥಿತಿ DISPLAY_FORMAT_COMPACT:
		display_state->refcol_ಅಗಲ = refcol_ಅಗಲ(ref_map,
							   display_state->format == DISPLAY_FORMAT_COMPACT);
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ DISPLAY_FORMAT_PORCELAIN:
		/* We don't need to precompute anything here. */
		ನಿಲ್ಲಿಸು;
	ಪೂರ್ವನಿಯೋಜಿತ:
		BUG("unexpected display format %d", display_state->format);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ display_state_release(ರಚನೆ display_state *display_state)
{
	strbuf_release(&display_state->buf);
	ಬಿಡುಗಡೆ(display_state->url);
}

ಸ್ಥಿರ ಶೂನ್ಯ print_remote_ಗಮ್ಯ_local(ರಚನೆ display_state *display_state,
				  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *remote, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *local)
{
	strbuf_addf(&display_state->buf, "%-*s -> %s",
		    display_state->refcol_ಅಗಲ, remote, local);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಕಂಡುಹಿಡಿ_and_replace(ರಚನೆ strbuf *haystack,
			    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *needle,
			    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *placeholder)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *p = ಖಾಲಿ;
	ಪೂರ್ಣಾಂಕ plen, nlen;

	nlen = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(needle);
	ಆದರೆ (ತುದಿಗಳು_with(haystack->buf, needle))
		p = haystack->buf + haystack->len - nlen;
	ಇಲ್ಲದಿದ್ದರೆ
		p = ಅಕ್ಷರಸರಣಿ_ಹುಡುಕು(haystack->buf, needle);
	ಆದರೆ (!p)
		ಹಿಂತಿರುಗಿ 0;

	ಆದರೆ (p > haystack->buf && p[-1] != '/')
		ಹಿಂತಿರುಗಿ 0;

	plen = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(p);
	ಆದರೆ (plen > nlen && p[nlen] != '/')
		ಹಿಂತಿರುಗಿ 0;

	strbuf_splice(haystack, p - haystack->buf, nlen,
		      placeholder, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(placeholder));
	ಹಿಂತಿರುಗಿ 1;
}

ಸ್ಥಿರ ಶೂನ್ಯ print_compact(ರಚನೆ display_state *display_state,
			  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *remote, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *local)
{
	ರಚನೆ strbuf r = STRBUF_INIT;
	ರಚನೆ strbuf l = STRBUF_INIT;

	ಆದರೆ (!strcmp(remote, local)) {
		strbuf_addf(&display_state->buf, "%-*s -> *", display_state->refcol_ಅಗಲ, remote);
		ಹಿಂತಿರುಗಿ;
	}

	strbuf_addstr(&r, remote);
	strbuf_addstr(&l, local);

	ಆದರೆ (!ಕಂಡುಹಿಡಿ_and_replace(&r, local, "*"))
		ಕಂಡುಹಿಡಿ_and_replace(&l, remote, "*");
	print_remote_ಗಮ್ಯ_local(display_state, r.buf, l.buf);

	strbuf_release(&r);
	strbuf_release(&l);
}

ಸ್ಥಿರ ಶೂನ್ಯ display_ref_update(ರಚನೆ display_state *display_state, ಅಕ್ಷರ ಸಂಕೇತ_2,
			       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *summary, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *error,
			       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *remote, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *local,
			       ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *old_oid,
			       ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *new_oid,
			       ಪೂರ್ಣಾಂಕ summary_ಅಗಲ)
{
	FILE *f = stderr;

	ಆದರೆ (verbosity < 0)
		ಹಿಂತಿರುಗಿ;

	strbuf_reset(&display_state->buf);

	ಆಯ್ಕೆ (display_state->format) {
	ಸ್ಥಿತಿ DISPLAY_FORMAT_ತುಂಬಿದ:
	ಸ್ಥಿತಿ DISPLAY_FORMAT_COMPACT: {
		ಪೂರ್ಣಾಂಕ ಅಗಲ;

		ಆದರೆ (!display_state->shown_url) {
			strbuf_addf(&display_state->buf, _("From %.*s\n"),
				    display_state->url_len, display_state->url);
			display_state->shown_url = 1;
		}

		ಅಗಲ = (summary_ಅಗಲ + ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(summary) - gettext_ಅಗಲ(summary));
		remote = prettify_refname(remote);
		local = prettify_refname(local);

		strbuf_addf(&display_state->buf, " %c %-*s ", ಸಂಕೇತ_2, ಅಗಲ, summary);

		ಆದರೆ (display_state->format != DISPLAY_FORMAT_COMPACT)
			print_remote_ಗಮ್ಯ_local(display_state, remote, local);
		ಇಲ್ಲದಿದ್ದರೆ
			print_compact(display_state, remote, local);

		ಆದರೆ (error)
			strbuf_addf(&display_state->buf, "  (%s)", error);

		ನಿಲ್ಲಿಸು;
	}
	ಸ್ಥಿತಿ DISPLAY_FORMAT_PORCELAIN:
		strbuf_addf(&display_state->buf, "%c %s %s %s", ಸಂಕೇತ_2,
			    oid_ಗಮ್ಯ_hex(old_oid), oid_ಗಮ್ಯ_hex(new_oid), local);
		f = stdout;
		ನಿಲ್ಲಿಸು;
	ಪೂರ್ವನಿಯೋಜಿತ:
		BUG("unexpected display format %d", display_state->format);
	};
	strbuf_addch(&display_state->buf, '\n');

	fputs(display_state->buf.buf, f);
}

ರಚನೆ ref_update_display_info {
	bool failed;
	ಅಕ್ಷರ success_ಸಂಕೇತ;
	ಅಕ್ಷರ fail_ಸಂಕೇತ;
	ಅಕ್ಷರ *summary;
	ಅಕ್ಷರ *fail_detail;
	ಅಕ್ಷರ *success_detail;
	ಅಕ್ಷರ *ref;
	ಅಕ್ಷರ *remote;
	ರಚನೆ object_id old_oid;
	ರಚನೆ object_id new_oid;
};

ರಚನೆ ref_update_display_info_array {
	ರಚನೆ ref_update_display_info *info;
	ಗಾತ್ರ_t alloc, nr;
};

ಸ್ಥಿರ ರಚನೆ ref_update_display_info *ref_update_display_info_append(
					   ರಚನೆ ref_update_display_info_array *array,
					   ಅಕ್ಷರ success_ಸಂಕೇತ,
					   ಅಕ್ಷರ fail_ಸಂಕೇತ,
					   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *summary,
					   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *success_detail,
					   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fail_detail,
					   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ref,
					   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *remote,
					   ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *old_oid,
					   ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *new_oid)
{
	ರಚನೆ ref_update_display_info *info;

	ALLOC_GROW(array->info, array->nr + 1, array->alloc);
	info = &array->info[array->nr++];

	info->failed = false;
	info->success_ಸಂಕೇತ = success_ಸಂಕೇತ;
	info->fail_ಸಂಕೇತ = fail_ಸಂಕೇತ;
	info->summary = xstrdup(summary);
	info->success_detail = xstrdup_or_null(success_detail);
	info->fail_detail = xstrdup_or_null(fail_detail);
	info->remote = xstrdup(remote);
	info->ref = xstrdup(ref);

	oidcpy(&info->old_oid, old_oid);
	oidcpy(&info->new_oid, new_oid);

	ಹಿಂತಿರುಗಿ info;
}

ಸ್ಥಿರ ಶೂನ್ಯ ref_update_display_info_ಗಣ_failed(ರಚನೆ ref_update_display_info *info)
{
	info->failed = true;
}

ಸ್ಥಿರ ಶೂನ್ಯ ref_update_display_info_free(ರಚನೆ ref_update_display_info *info)
{
	ಬಿಡುಗಡೆ(info->summary);
	ಬಿಡುಗಡೆ(info->success_detail);
	ಬಿಡುಗಡೆ(info->fail_detail);
	ಬಿಡುಗಡೆ(info->remote);
	ಬಿಡುಗಡೆ(info->ref);
}

ಸ್ಥಿರ ಶೂನ್ಯ ref_update_display_info_display(ರಚನೆ ref_update_display_info *info,
					    ರಚನೆ display_state *display_state,
					    ಪೂರ್ಣಾಂಕ summary_ಅಗಲ)
{
	display_ref_update(display_state,
			   info->failed ? info->fail_ಸಂಕೇತ : info->success_ಸಂಕೇತ,
			   info->summary,
			   info->failed ? info->fail_detail : info->success_detail,
			   info->remote, info->ref, &info->old_oid,
			   &info->new_oid, summary_ಅಗಲ);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ update_local_ref(ರಚನೆ ref *ref,
			    ರಚನೆ ref_transaction *transaction,
			    ಸ್ಥಿರಾಂಕ ರಚನೆ ref *remote_ref,
			    ಸ್ಥಿರಾಂಕ ರಚನೆ fetch_config *config,
			    ರಚನೆ ref_update_display_info_array *display_array)
{
	ರಚನೆ commit *current = ಖಾಲಿ, *updated;
	ಪೂರ್ಣಾಂಕ fast_forward = 0;

	ಆದರೆ (!odb_has_object(the_repository->objects, &ref->new_oid,
			    ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR))
		die(_("object %s not found"), oid_ಗಮ್ಯ_hex(&ref->new_oid));

	ಆದರೆ (oideq(&ref->old_oid, &ref->new_oid)) {
		ಆದರೆ (verbosity > 0)
			ref_update_display_info_append(display_array, '=', '=',
						       _("[up to date]"), ಖಾಲಿ,
						       ಖಾಲಿ, ref->name,
						       remote_ref->name, &ref->old_oid,
						       &ref->new_oid);
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (!update_ತಲೆ_ok &&
	    !is_null_oid(&ref->old_oid) &&
	    branch_checked_ಹೊರಹೋಗು(ref->name)) {
		ರಚನೆ ref_update_display_info *info;
		/*
		 * If this is the head, and it's not okay to update
		 * the head, and the old value of the head isn't empty...
		 */
		info = ref_update_display_info_append(display_array, '!', '!',
						      _("[rejected]"), ಖಾಲಿ,
						      _("can't fetch into checked-out branch"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);
		ref_update_display_info_ಗಣ_failed(info);
		ಹಿಂತಿರುಗಿ 1;
	}

	ಆದರೆ (!is_null_oid(&ref->old_oid) &&
	    starts_with(ref->name, "refs/tags/")) {
		ರಚನೆ ref_update_display_info *info;

		ಆದರೆ (force || ref->force) {
			ಪೂರ್ಣಾಂಕ r;

			r = s_update_ref("updating tag", ref, transaction, 0);

			info = ref_update_display_info_append(display_array, 't', '!',
							      _("[tag update]"), ಖಾಲಿ,
							      _("unable to update local ref"),
							      ref->name, remote_ref->name,
							      &ref->old_oid, &ref->new_oid);
			ಆದರೆ (r)
				ref_update_display_info_ಗಣ_failed(info);

			ಹಿಂತಿರುಗಿ r;
		} ಇಲ್ಲದಿದ್ದರೆ {
			info = ref_update_display_info_append(display_array, '!', '!',
							      _("[rejected]"), ಖಾಲಿ,
							      _("would clobber existing tag"),
							      ref->name, remote_ref->name,
							      &ref->old_oid, &ref->new_oid);
			ref_update_display_info_ಗಣ_failed(info);
			ಹಿಂತಿರುಗಿ 1;
		}
	}

	current = lookup_commit_reference_gently(the_repository,
						 &ref->old_oid, 1);
	updated = lookup_commit_reference_gently(the_repository,
						 &ref->new_oid, 1);
	ಆದರೆ (!current || !updated) {
		ರಚನೆ ref_update_display_info *info;
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ;
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *what;
		ಪೂರ್ಣಾಂಕ r;
		/*
		 * Nicely describe the new ref we're fetching.
		 * Base this on the remote's ref name, as it's
		 * more likely to follow a standard layout.
		 */
		ಆದರೆ (starts_with(remote_ref->name, "refs/tags/")) {
			ಸಂದೇಶ = "storing tag";
			what = _("[new tag]");
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (starts_with(remote_ref->name, "refs/heads/")) {
			ಸಂದೇಶ = "storing head";
			what = _("[new branch]");
		} ಇಲ್ಲದಿದ್ದರೆ {
			ಸಂದೇಶ = "storing ref";
			what = _("[new ref]");
		}

		r = s_update_ref(ಸಂದೇಶ, ref, transaction, 0);

		info = ref_update_display_info_append(display_array, '*', '!',
						      what, ಖಾಲಿ,
						      _("unable to update local ref"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);
		ಆದರೆ (r)
			ref_update_display_info_ಗಣ_failed(info);

		ಹಿಂತಿರುಗಿ r;
	}

	ಆದರೆ (config->show_forced_updates) {
		uint64_t t_before = getnanotime();
		fast_forward = repo_ಒಳ_ವಿಲೀನ_bases(the_repository, current,
						   updated);
		ಆದರೆ (fast_forward < 0)
			die(ಖಾಲಿ);
		forced_updates_ms += (getnanotime() - t_before) / 1000000;
	} ಇಲ್ಲದಿದ್ದರೆ {
		fast_forward = 1;
	}

	ಆದರೆ (fast_forward) {
		ರಚನೆ ref_update_display_info *info;
		ರಚನೆ strbuf quickref = STRBUF_INIT;
		ಪೂರ್ಣಾಂಕ r;

		strbuf_ಕೂಡಿಸು_unique_abbrev(&quickref, &current->object.oid, DEFAULT_ABBREV);
		strbuf_addstr(&quickref, "..");
		strbuf_ಕೂಡಿಸು_unique_abbrev(&quickref, &ref->new_oid, DEFAULT_ABBREV);
		r = s_update_ref("fast-forward", ref, transaction, 1);

		info = ref_update_display_info_append(display_array, ' ', '!',
						      quickref.buf, ಖಾಲಿ,
						      _("unable to update local ref"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);
		ಆದರೆ (r)
			ref_update_display_info_ಗಣ_failed(info);

		strbuf_release(&quickref);
		ಹಿಂತಿರುಗಿ r;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (force || ref->force) {
		ರಚನೆ ref_update_display_info *info;
		ರಚನೆ strbuf quickref = STRBUF_INIT;
		ಪೂರ್ಣಾಂಕ r;

		strbuf_ಕೂಡಿಸು_unique_abbrev(&quickref, &current->object.oid, DEFAULT_ABBREV);
		strbuf_addstr(&quickref, "...");
		strbuf_ಕೂಡಿಸು_unique_abbrev(&quickref, &ref->new_oid, DEFAULT_ABBREV);
		r = s_update_ref("forced-update", ref, transaction, 1);

		info = ref_update_display_info_append(display_array, '+', '!',
						      quickref.buf, _("forced update"),
						      _("unable to update local ref"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);

		ಆದರೆ (r)
			ref_update_display_info_ಗಣ_failed(info);

		strbuf_release(&quickref);
		ಹಿಂತಿರುಗಿ r;
	} ಇಲ್ಲದಿದ್ದರೆ {
		ರಚನೆ ref_update_display_info *info;
		info = ref_update_display_info_append(display_array, '!', '!',
						      _("[rejected]"), ಖಾಲಿ,
						      _("non-fast-forward"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);
		ref_update_display_info_ಗಣ_failed(info);
		ಹಿಂತಿರುಗಿ 1;
	}
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *iterate_ref_map(ಶೂನ್ಯ *cb_ದತ್ತಾಂಶ)
{
	ರಚನೆ ref **rm = cb_ದತ್ತಾಂಶ;
	ರಚನೆ ref *ref = *rm;

	ಆಗ (ref && ref->ಸ್ಥಿತಿ_2 == REF_ಸ್ಥಿತಿ_REJECT_SHALLOW)
		ref = ref->ಮುಂದಿನ;
	ಆದರೆ (!ref)
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;
	*rm = ref->ಮುಂದಿನ;
	ಹಿಂತಿರುಗಿ &ref->old_oid;
}

ರಚನೆ fetch_ತಲೆ {
	FILE *fp;
	ರಚನೆ strbuf buf;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ open_fetch_ತಲೆ(ರಚನೆ fetch_ತಲೆ *fetch_ತಲೆ)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *filename = git_ಮಾರ್ಗ_fetch_ತಲೆ(the_repository);

	ಆದರೆ (write_fetch_ತಲೆ) {
		fetch_ತಲೆ->fp = fopen(filename, "a");
		ಆದರೆ (!fetch_ತಲೆ->fp)
			ಹಿಂತಿರುಗಿ error_errno(_("cannot open '%s'"), filename);
		strbuf_init(&fetch_ತಲೆ->buf, 0);
	} ಇಲ್ಲದಿದ್ದರೆ {
		fetch_ತಲೆ->fp = ಖಾಲಿ;
	}

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ append_fetch_ತಲೆ(ರಚನೆ fetch_ತಲೆ *fetch_ತಲೆ,
			      ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *old_oid,
			      ಎಣಿಕೆ fetch_ತಲೆ_ಸ್ಥಿತಿ fetch_ತಲೆ_ಸ್ಥಿತಿ,
			      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *note,
			      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *url, ಗಾತ್ರ_t url_len)
{
	ಅಕ್ಷರ old_oid_hex[GIT_MAX_HEXSZ + 1];
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ವಿಲೀನ_ಸ್ಥಿತಿ_marker;
	ಗಾತ್ರ_t i;

	ಆದರೆ (!fetch_ತಲೆ->fp)
		ಹಿಂತಿರುಗಿ;

	ಆಯ್ಕೆ (fetch_ತಲೆ_ಸ್ಥಿತಿ) {
	ಸ್ಥಿತಿ FETCH_ತಲೆ_NOT_FOR_ವಿಲೀನ:
		ವಿಲೀನ_ಸ್ಥಿತಿ_marker = "not-for-merge";
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ FETCH_ತಲೆ_ವಿಲೀನ:
		ವಿಲೀನ_ಸ್ಥಿತಿ_marker = "";
		ನಿಲ್ಲಿಸು;
	ಪೂರ್ವನಿಯೋಜಿತ:
		/* do not write anything to FETCH_HEAD */
		ಹಿಂತಿರುಗಿ;
	}

	strbuf_addf(&fetch_ತಲೆ->buf, "%s\t%s\t%s",
		    oid_ಗಮ್ಯ_hex_r(old_oid_hex, old_oid), ವಿಲೀನ_ಸ್ಥಿತಿ_marker, note);
	ಗಾಗಿ (i = 0; i < url_len; ++i)
		ಆದರೆ ('\n' == url[i])
			strbuf_addstr(&fetch_ತಲೆ->buf, "\\n");
		ಇಲ್ಲದಿದ್ದರೆ
			strbuf_addch(&fetch_ತಲೆ->buf, url[i]);
	strbuf_addch(&fetch_ತಲೆ->buf, '\n');

	/*
	 * When using an atomic fetch, we do not want to update FETCH_HEAD if
	 * any of the reference updates fails. We thus have to write all
	 * updates to a buffer first and only commit it as soon as all
	 * references have been successfully updated.
	 */
	ಆದರೆ (!atomic_fetch) {
		strbuf_write(&fetch_ತಲೆ->buf, fetch_ತಲೆ->fp);
		strbuf_reset(&fetch_ತಲೆ->buf);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ commit_fetch_ತಲೆ(ರಚನೆ fetch_ತಲೆ *fetch_ತಲೆ)
{
	ಆದರೆ (!fetch_ತಲೆ->fp || !atomic_fetch)
		ಹಿಂತಿರುಗಿ;
	strbuf_write(&fetch_ತಲೆ->buf, fetch_ತಲೆ->fp);
}

ಸ್ಥಿರ ಶೂನ್ಯ close_fetch_ತಲೆ(ರಚನೆ fetch_ತಲೆ *fetch_ತಲೆ)
{
	ಆದರೆ (!fetch_ತಲೆ->fp)
		ಹಿಂತಿರುಗಿ;

	fclose(fetch_ತಲೆ->fp);
	strbuf_release(&fetch_ತಲೆ->buf);
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ warn_show_forced_updates[] =
N_("fetch normally indicates which branches had a forced update,\n"
   "but that check has been disabled; to re-enable, use '--show-forced-updates'\n"
   "flag or run 'git config fetch.showForcedUpdates true'");
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ warn_ಸಮಯ_show_forced_updates[] =
N_("it took %.2f seconds to check forced updates; you can use\n"
   "'--no-show-forced-updates' or run 'git config fetch.showForcedUpdates false'\n"
   "to avoid this check\n");

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ store_updated_refs(ರಚನೆ display_state *display_state,
			      ಪೂರ್ಣಾಂಕ connectivity_checked,
			      ರಚನೆ ref_transaction *transaction, ರಚನೆ ref *ref_map,
			      ರಚನೆ fetch_ತಲೆ *fetch_ತಲೆ,
			      ಸ್ಥಿರಾಂಕ ರಚನೆ fetch_config *config,
			      ರಚನೆ ref_update_display_info_array *display_array)
{
	ಪೂರ್ಣಾಂಕ rc = 0;
	ರಚನೆ strbuf note = STRBUF_INIT;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *what, *ಬಗೆ;
	ರಚನೆ ref *rm;
	ಪೂರ್ಣಾಂಕ want_ಸ್ಥಿತಿ;

	ಆದರೆ (!connectivity_checked) {
		ರಚನೆ check_connected_options opt = CHECK_CONNECTED_INIT;

		opt.exclude_hidden_refs_section = "fetch";
		rm = ref_map;
		ಆದರೆ (check_connected(iterate_ref_map, &rm, &opt)) {
			rc = error(_("%s did not send all necessary objects"),
				   display_state->url);
			ಹೋಗು abort;
		}
	}

	/*
	 * We do a pass for each fetch_head_status type in their enum order, so
	 * merged entries are written before not-for-merge. That lets readers
	 * use FETCH_HEAD as a refname to refer to the ref to be merged.
	 */
	ಗಾಗಿ (want_ಸ್ಥಿತಿ = FETCH_ತಲೆ_ವಿಲೀನ;
	     want_ಸ್ಥಿತಿ <= FETCH_ತಲೆ_IGNORE;
	     want_ಸ್ಥಿತಿ++) {
		ಗಾಗಿ (rm = ref_map; rm; rm = rm->ಮುಂದಿನ) {
			ರಚನೆ ref *ref = ಖಾಲಿ;

			ಆದರೆ (rm->ಸ್ಥಿತಿ_2 == REF_ಸ್ಥಿತಿ_REJECT_SHALLOW) {
				ಆದರೆ (want_ಸ್ಥಿತಿ == FETCH_ತಲೆ_ವಿಲೀನ)
					warning(_("rejected %s because shallow roots are not allowed to be updated"),
						rm->peer_ref ? rm->peer_ref->name : rm->name);
				ಮುಂದುವರಿ;
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
			ಆದರೆ (fetch_ತಲೆ->fp) {
				ರಚನೆ commit *commit = ಖಾಲಿ;

				/*
				 * References in "refs/tags/" are often going to point
				 * to annotated tags, which are not part of the
				 * commit-graph. We thus only try to look up refs in
				 * the graph which are not in that namespace to not
				 * regress performance in repositories with many
				 * annotated tags.
				 */
				ಆದರೆ (!starts_with(rm->name, "refs/tags/"))
					commit = lookup_commit_ಒಳ_ನಕ್ಷೆ(the_repository, &rm->old_oid);
				ಆದರೆ (!commit) {
					commit = lookup_commit_reference_gently(the_repository,
										&rm->old_oid,
										1);
					ಆದರೆ (!commit)
						rm->fetch_ತಲೆ_ಸ್ಥಿತಿ = FETCH_ತಲೆ_NOT_FOR_ವಿಲೀನ;
				}
			}

			ಆದರೆ (rm->fetch_ತಲೆ_ಸ್ಥಿತಿ != want_ಸ್ಥಿತಿ)
				ಮುಂದುವರಿ;

			ಆದರೆ (rm->peer_ref) {
				ref = alloc_ref(rm->peer_ref->name);
				oidcpy(&ref->old_oid, &rm->peer_ref->old_oid);
				oidcpy(&ref->new_oid, &rm->old_oid);
				ref->force = rm->peer_ref->force;
			}

			ಆದರೆ (config->recurse_submodules != RECURSE_SUBMODULES_OFF &&
			    (!rm->peer_ref || !oideq(&ref->old_oid, &ref->new_oid))) {
				check_for_new_submodule_commits(&rm->old_oid);
			}

			ಆದರೆ (!strcmp(rm->name, "HEAD")) {
				ಬಗೆ = "";
				what = "";
			} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(rm->name, "refs/heads/", &what)) {
				ಬಗೆ = "branch";
			} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(rm->name, "refs/tags/", &what)) {
				ಬಗೆ = "tag";
			} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(rm->name, "refs/remotes/", &what)) {
				ಬಗೆ = "remote-tracking branch";
			} ಇಲ್ಲದಿದ್ದರೆ {
				ಬಗೆ = "";
				what = rm->name;
			}

			strbuf_reset(&note);
			ಆದರೆ (*what) {
				ಆದರೆ (*ಬಗೆ)
					strbuf_addf(&note, "%s ", ಬಗೆ);
				strbuf_addf(&note, "'%s' of ", what);
			}

			append_fetch_ತಲೆ(fetch_ತಲೆ, &rm->old_oid,
					  rm->fetch_ತಲೆ_ಸ್ಥಿತಿ,
					  note.buf, display_state->url,
					  display_state->url_len);

			ಆದರೆ (ref) {
				rc |= update_local_ref(ref, transaction, rm,
						       config, display_array);
				ಬಿಡುಗಡೆ(ref);
			} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (write_fetch_ತಲೆ || dry_ಸತತ) {
				/*
				 * Display fetches written to FETCH_HEAD (or
				 * would be written to FETCH_HEAD, if --dry-run
				 * is set).
				 */

				ref_update_display_info_append(display_array, '*', '*',
							       *ಬಗೆ ? ಬಗೆ : "branch",
							       ಖಾಲಿ, ಖಾಲಿ, "FETCH_HEAD",
							       rm->name, &rm->new_oid,
							       &rm->old_oid);
			}
		}
	}

	ಆದರೆ (advice_enabled(ADVICE_FETCH_SHOW_FORCED_UPDATES)) {
		ಆದರೆ (!config->show_forced_updates) {
			warning(_(warn_show_forced_updates));
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (forced_updates_ms > FORCED_UPDATES_DELAY_WARNING_ಒಳ_MS) {
			warning(_(warn_ಸಮಯ_show_forced_updates),
				forced_updates_ms / 1000.0);
		}
	}

 abort:
	strbuf_release(&note);
	ಹಿಂತಿರುಗಿ rc;
}

/*
 * We would want to bypass the object transfer altogether if
 * everything we are going to fetch already exists and is connected
 * locally.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ check_exist_and_connected(ರಚನೆ ref *ref_map)
{
	ರಚನೆ ref *rm = ref_map;
	ರಚನೆ check_connected_options opt = CHECK_CONNECTED_INIT;
	ರಚನೆ ref *r;

	/*
	 * If we are deepening a shallow clone we already have these
	 * objects reachable.  Running rev-list here will return with
	 * a good (0) exit status and we'll bypass the fetch that we
	 * really need to perform.  Claiming failure now will ensure
	 * we perform the network exchange to deepen our history.
	 */
	ಆದರೆ (deepen)
		ಹಿಂತಿರುಗಿ -1;

	/*
	 * Similarly, if we need to refetch, we always want to perform a full
	 * fetch ignoring existing objects.
	 */
	ಆದರೆ (refetch)
		ಹಿಂತಿರುಗಿ -1;


	/*
	 * check_connected() allows objects to merely be promised, but
	 * we need all direct targets to exist.
	 */
	ಗಾಗಿ (r = rm; r; r = r->ಮುಂದಿನ) {
		ಆದರೆ (!odb_has_object(the_repository->objects, &r->old_oid,
				    ODB_HAS_OBJECT_RECHECK_PACKED))
			ಹಿಂತಿರುಗಿ -1;
	}

	opt.quiet = 1;
	opt.exclude_hidden_refs_section = "fetch";
	ಹಿಂತಿರುಗಿ check_connected(iterate_ref_map, &rm, &opt);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ fetch_and_consume_refs(ರಚನೆ display_state *display_state,
				  ರಚನೆ transport *transport,
				  ರಚನೆ ref_transaction *transaction,
				  ರಚನೆ ref *ref_map,
				  ರಚನೆ fetch_ತಲೆ *fetch_ತಲೆ,
				  ಸ್ಥಿರಾಂಕ ರಚನೆ fetch_config *config,
				  ರಚನೆ ref_update_display_info_array *display_array)
{
	ಪೂರ್ಣಾಂಕ connectivity_checked = 1;
	ಪೂರ್ಣಾಂಕ ret;

	/*
	 * We don't need to perform a fetch in case we can already satisfy all
	 * refs.
	 */
	ret = check_exist_and_connected(ref_map);
	ಆದರೆ (ret) {
		trace2_region_enter("fetch", "fetch_refs", the_repository);
		ret = transport_fetch_refs(transport, ref_map);
		trace2_region_leave("fetch", "fetch_refs", the_repository);
		ಆದರೆ (ret)
			ಹೋಗು ಹೊರಹೋಗು;
		connectivity_checked = transport->smart_options ?
			transport->smart_options->connectivity_checked : 0;
	}

	trace2_region_enter("fetch", "consume_refs", the_repository);
	ret = store_updated_refs(display_state, connectivity_checked,
				 transaction, ref_map, fetch_ತಲೆ, config,
				 display_array);
	trace2_region_leave("fetch", "consume_refs", the_repository);

ಹೊರಹೋಗು:
	transport_unlock_pack(transport, 0);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ prune_refs(ರಚನೆ display_state *display_state,
		      ರಚನೆ refspec *rs,
		      ರಚನೆ ref_transaction *transaction,
		      ರಚನೆ ref *ref_map)
{
	ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ = 0;
	ರಚನೆ ref *ref, *stale_refs = get_stale_heads(rs, ref_map);
	ರಚನೆ strbuf err = STRBUF_INIT;
	ರಚನೆ string_list refnames = STRING_LIST_INIT_NODUP;

	ಗಾಗಿ (ref = stale_refs; ref; ref = ref->ಮುಂದಿನ)
		string_list_append(&refnames, ref->name);

	ಆದರೆ (!dry_ಸತತ) {
		ಆದರೆ (transaction) {
			ಗಾಗಿ (ref = stale_refs; ref; ref = ref->ಮುಂದಿನ) {
				ಫಲಿತಾಂಶ = ref_transaction_delete(transaction, ref->name, ಖಾಲಿ,
								ಖಾಲಿ, 0, "fetch: prune", &err);
				ಆದರೆ (ಫಲಿತಾಂಶ)
					ಹೋಗು cleanup;
			}
		} ಇಲ್ಲದಿದ್ದರೆ {
			ಫಲಿತಾಂಶ = refs_delete_refs(get_main_ref_store(the_repository),
						  "fetch: prune", &refnames,
						  0);
		}
	}

	ಆದರೆ (verbosity >= 0) {
		ಪೂರ್ಣಾಂಕ summary_ಅಗಲ = transport_summary_ಅಗಲ(stale_refs);

		ಗಾಗಿ (ref = stale_refs; ref; ref = ref->ಮುಂದಿನ) {
			display_ref_update(display_state, '-', _("[deleted]"), ಖಾಲಿ,
					   _("(none)"), ref->name,
					   &ref->new_oid, &ref->old_oid,
					   summary_ಅಗಲ);
		}
		string_list_ವಿಂಗಡಿಸು(&refnames);
		refs_warn_dangling_symrefs(get_main_ref_store(the_repository),
					   stderr, "   ", dry_ಸತತ, &refnames);
	}

cleanup:
	string_list_clear(&refnames, 0);
	strbuf_release(&err);
	free_refs(stale_refs);
	ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

ಸ್ಥಿರ ಶೂನ್ಯ check_not_current_branch(ರಚನೆ ref *ref_map)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ;
	ಗಾಗಿ (; ref_map; ref_map = ref_map->ಮುಂದಿನ)
		ಆದರೆ (ref_map->peer_ref &&
		    starts_with(ref_map->peer_ref->name, "refs/heads/") &&
		    (ಮಾರ್ಗ = branch_checked_ಹೊರಹೋಗು(ref_map->peer_ref->name)))
			die(_("refusing to fetch into branch '%s' "
			      "checked out at '%s'"),
			    ref_map->peer_ref->name, ಮಾರ್ಗ);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ truncate_fetch_ತಲೆ(ಶೂನ್ಯ)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *filename = git_ಮಾರ್ಗ_fetch_ತಲೆ(the_repository);
	FILE *fp = fopen_for_writing(filename);

	ಆದರೆ (!fp)
		ಹಿಂತಿರುಗಿ error_errno(_("cannot open '%s'"), filename);
	fclose(fp);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ ಗಣ_option(ರಚನೆ transport *transport, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ)
{
	ಪೂರ್ಣಾಂಕ r = transport_ಗಣ_option(transport, name, ಮೌಲ್ಯ);
	ಆದರೆ (r < 0)
		die(_("option \"%s\" value \"%s\" is not valid for %s"),
		    name, ಮೌಲ್ಯ, transport->url);
	ಆದರೆ (r > 0)
		warning(_("option \"%s\" is ignored for %s"),
			name, transport->url);
}


ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಕೂಡಿಸು_oid(ಸ್ಥಿರಾಂಕ ರಚನೆ reference *ref, ಶೂನ್ಯ *cb_ದತ್ತಾಂಶ)
{
	ರಚನೆ oid_array *oids = cb_ದತ್ತಾಂಶ;

	oid_array_append(oids, ref->oid);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ ಕೂಡಿಸು_negotiation_tips(ರಚನೆ string_list *ಒಳಹರಿವು_list,
				 ರಚನೆ oid_array **output_list,
				 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *argname)
{
	ರಚನೆ oid_array *oids = xcalloc(1, ಗಾತ್ರ(*oids));
	ಪೂರ್ಣಾಂಕ i;

	ಗಾಗಿ (i = 0; i < ಒಳಹರಿವು_list->nr; i++) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s = ಒಳಹರಿವು_list->ವಸ್ತುಗಳು[i].string;
		ರಚನೆ refs_for_each_ref_options opts = {
			.ನಮೂನೆ = s,
		};
		ಪೂರ್ಣಾಂಕ old_nr;
		ಆದರೆ (!has_glob_specials(s)) {
			ರಚನೆ object_id oid;

			/* Ignore missing reference. */
			ಆದರೆ (repo_get_oid(the_repository, s, &oid))
				ಮುಂದುವರಿ;
			/* Fail on missing object pointed by ref. */
			ಆದರೆ (!odb_has_object(the_repository->objects, &oid, 0))
				die(_("the object %s does not exist"), s);

			oid_array_append(oids, &oid);
			ಮುಂದುವರಿ;
		}
		old_nr = oids->nr;
		refs_for_each_ref_ext(get_main_ref_store(the_repository),
				      ಕೂಡಿಸು_oid, oids, &opts);
		ಆದರೆ (old_nr == oids->nr)
			warning(_("ignoring %s=%s because it does not match any refs"),
				argname, s);
	}
	*output_list = oids;
}

ಸ್ಥಿರ ರಚನೆ transport *prepare_transport(ರಚನೆ remote *remote, ಪೂರ್ಣಾಂಕ deepen,
					   ರಚನೆ list_objects_filter_options *filter_options)
{
	ರಚನೆ transport *transport;

	transport = transport_get(remote, ಖಾಲಿ);
	transport_ಗಣ_verbosity(transport, verbosity, ಪ್ರಗತಿ);
	transport->family = family;
	ಆದರೆ (upload_pack)
		ಗಣ_option(transport, TRANS_OPT_UPLOADPACK, upload_pack);
	ಆದರೆ (keep)
		ಗಣ_option(transport, TRANS_OPT_KEEP, "yes");
	ಆದರೆ (ಆಳ)
		ಗಣ_option(transport, TRANS_OPT_ಆಳ, ಆಳ);
	ಆದರೆ (deepen && deepen_since)
		ಗಣ_option(transport, TRANS_OPT_DEEPEN_SINCE, deepen_since);
	ಆದರೆ (deepen && deepen_not.nr)
		ಗಣ_option(transport, TRANS_OPT_DEEPEN_NOT,
			   (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *)&deepen_not);
	ಆದರೆ (deepen_relative)
		ಗಣ_option(transport, TRANS_OPT_DEEPEN_RELATIVE, "yes");
	ಆದರೆ (update_shallow)
		ಗಣ_option(transport, TRANS_OPT_UPDATE_SHALLOW, "yes");
	ಆದರೆ (refetch)
		ಗಣ_option(transport, TRANS_OPT_REFETCH, "yes");
	ಆದರೆ (filter_options->choice) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *spec =
			expand_list_objects_filter_spec(filter_options);
		ಗಣ_option(transport, TRANS_OPT_LIST_OBJECTS_FILTER, spec);
		ಗಣ_option(transport, TRANS_OPT_FROM_PROMISOR, "1");
	}
	ಆದರೆ (negotiation_restrict.nr) {
		ಆದರೆ (transport->smart_options)
			ಕೂಡಿಸು_negotiation_tips(&negotiation_restrict,
					     &transport->smart_options->negotiation_restrict_tips,
					     "--negotiation-restrict");
		ಇಲ್ಲದಿದ್ದರೆ
			warning(_("ignoring %s because the protocol does not support it"),
				"--negotiation-restrict");
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (remote->negotiation_restrict.nr) {
		ರಚನೆ string_list_ಅಂಶ *ಅಂಶ;
		for_each_string_list_ಅಂಶ(ಅಂಶ, &remote->negotiation_restrict)
			string_list_append(&negotiation_restrict, ಅಂಶ->string);
		ಆದರೆ (transport->smart_options)
			ಕೂಡಿಸು_negotiation_tips(&negotiation_restrict,
					     &transport->smart_options->negotiation_restrict_tips,
					     "--negotiation-restrict");
		ಇಲ್ಲದಿದ್ದರೆ {
			ರಚನೆ strbuf config_name = STRBUF_INIT;
			strbuf_addf(&config_name, "remote.%s.negotiationRestrict", remote->name);
			warning(_("ignoring %s because the protocol does not support it"),
				config_name.buf);
			strbuf_release(&config_name);
		}
	}
	ಆದರೆ (negotiation_include.nr) {
		ಆದರೆ (transport->smart_options)
			ಕೂಡಿಸು_negotiation_tips(&negotiation_include,
					     &transport->smart_options->negotiation_include_tips,
					     "--negotiation-include");
		ಇಲ್ಲದಿದ್ದರೆ
			warning(_("ignoring %s because the protocol does not support it"),
				"--negotiation-include");
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (remote->negotiation_include.nr) {
		ಆದರೆ (transport->smart_options) {
			ಕೂಡಿಸು_negotiation_tips(&remote->negotiation_include,
					     &transport->smart_options->negotiation_include_tips,
					     "--negotiation-include");
		} ಇಲ್ಲದಿದ್ದರೆ {
			ರಚನೆ strbuf config_name = STRBUF_INIT;
			strbuf_addf(&config_name, "remote.%s.negotiationInclude", remote->name);
			warning(_("ignoring %s because the protocol does not support it"),
				config_name.buf);
			strbuf_release(&config_name);
		}
	}
	ಹಿಂತಿರುಗಿ transport;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ backfill_ಟ್ಯಾಗುಗಳು(ರಚನೆ display_state *display_state,
			 ರಚನೆ transport *transport,
			 ರಚನೆ ref_transaction *transaction,
			 ರಚನೆ ref *ref_map,
			 ರಚನೆ fetch_ತಲೆ *fetch_ತಲೆ,
			 ಸ್ಥಿರಾಂಕ ರಚನೆ fetch_config *config,
			 ರಚನೆ ref_update_display_info_array *display_array,
			 ರಚನೆ list_objects_filter_options *filter_options)
{
	ಪೂರ್ಣಾಂಕ retcode, cannot_reuse;

	/*
	 * Once we have set TRANS_OPT_DEEPEN_SINCE, we can't unset it
	 * when remote helper is used (setting it to an empty string
	 * is not unsetting). We could extend the remote helper
	 * protocol for that, but for now, just force a new connection
	 * without deepen-since. Similar story for deepen-not.
	 */
	cannot_reuse = transport->cannot_reuse ||
		deepen_since || deepen_not.nr;
	ಆದರೆ (cannot_reuse) {
		gsecondary = prepare_transport(transport->remote, 0, filter_options);
		transport = gsecondary;
	}

	transport_ಗಣ_option(transport, TRANS_OPT_FOLLOWTAGS, ಖಾಲಿ);
	transport_ಗಣ_option(transport, TRANS_OPT_ಆಳ, "0");
	transport_ಗಣ_option(transport, TRANS_OPT_DEEPEN_RELATIVE, ಖಾಲಿ);
	retcode = fetch_and_consume_refs(display_state, transport, transaction, ref_map,
					 fetch_ತಲೆ, config, display_array);

	ಆದರೆ (gsecondary) {
		transport_disconnect(gsecondary);
		gsecondary = ಖಾಲಿ;
	}

	ಹಿಂತಿರುಗಿ retcode;
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *strip_refshead(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name){
	skip_ಪೂರ್ವಪ್ರತ್ಯಯ(name, "refs/heads/", &name);
	ಹಿಂತಿರುಗಿ name;
}

ಸ್ಥಿರ ಶೂನ್ಯ ಗಣ_ತಲೆ_advice_ಸಂದೇಶ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *remote, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ತಲೆ_name)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ ಸಂದೇಶ_advice_ಗಣ_ತಲೆ[] =
	N_("Run 'git remote set-head %s %s' to follow the change, or modify\n"
	   "either of the 'remote.%s.followRemoteHEAD' or 'fetch.followRemoteHEAD'\n"
	   "configuration variables to handle the situation differently.\n\n"

	   "Using this specific setting\n\n"
	   "    git config set remote.%s.followRemoteHEAD warn-if-not-%s\n\n"
	   "will suppress the warning until the remote changes HEAD to something else.");

	advise_if_enabled(ADVICE_FETCH_ಗಣ_ತಲೆ_WARN, _(ಸಂದೇಶ_advice_ಗಣ_ತಲೆ),
			remote, ತಲೆ_name, remote, remote, ತಲೆ_name);
}

ಸ್ಥಿರ ಶೂನ್ಯ warn_ಗಣ_ತಲೆ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *remote, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ತಲೆ_name,
			ರಚನೆ strbuf *buf_prev, ಪೂರ್ಣಾಂಕ updateres) {
	ರಚನೆ strbuf buf_ಪೂರ್ವಪ್ರತ್ಯಯ = STRBUF_INIT;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *prev_ತಲೆ = ಖಾಲಿ;

	strbuf_addf(&buf_ಪೂರ್ವಪ್ರತ್ಯಯ, "refs/remotes/%s/", remote);
	skip_ಪೂರ್ವಪ್ರತ್ಯಯ(buf_prev->buf, buf_ಪೂರ್ವಪ್ರತ್ಯಯ.buf, &prev_ತಲೆ);

	ಆದರೆ (prev_ತಲೆ && strcmp(prev_ತಲೆ, ತಲೆ_name)) {
		ಬರೆ("'HEAD' at '%s' is '%s', but we have '%s' locally.\n",
			remote, ತಲೆ_name, prev_ತಲೆ);
		ಗಣ_ತಲೆ_advice_ಸಂದೇಶ(remote, ತಲೆ_name);
	}
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (updateres && buf_prev->len) {
		ಬರೆ("'HEAD' at '%s' is '%s', "
			"but we have a detached HEAD pointing to '%s' locally.\n",
			remote, ತಲೆ_name, buf_prev->buf);
		ಗಣ_ತಲೆ_advice_ಸಂದೇಶ(remote, ತಲೆ_name);
	}
	strbuf_release(&buf_ಪೂರ್ವಪ್ರತ್ಯಯ);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಗಣ_ತಲೆ(ಸ್ಥಿರಾಂಕ ರಚನೆ ref *remote_refs, ರಚನೆ remote *remote,
			ಪೂರ್ಣಾಂಕ follow_remote_ತಲೆ)
{
	ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ = 0, create_only, baremirror, was_detached;
	ರಚನೆ strbuf b_ತಲೆ = STRBUF_INIT, b_remote_ತಲೆ = STRBUF_INIT,
		      b_local_ತಲೆ = STRBUF_INIT;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *no_warn_branch = remote->no_warn_branch;
	ಅಕ್ಷರ *ತಲೆ_name = ಖಾಲಿ;
	ರಚನೆ ref *ref, *matches;
	ರಚನೆ ref *fetch_map = ಖಾಲಿ, **fetch_map_ಬಾಲ = &fetch_map;
	ರಚನೆ refspec_ಅಂಶ refspec = {
		.force = 0,
		.ನಮೂನೆ = 1,
		.src = (ಅಕ್ಷರ *) "refs/heads/*",
		.dst = (ಅಕ್ಷರ *) "refs/heads/*",
	};
	ರಚನೆ string_list heads = STRING_LIST_INIT_DUP;
	ರಚನೆ ref_store *refs = get_main_ref_store(the_repository);

	get_fetch_map(remote_refs, &refspec, &fetch_map_ಬಾಲ, 0);
	matches = guess_remote_ತಲೆ(ಕಂಡುಹಿಡಿ_ref_by_name(remote_refs, "HEAD"),
				    fetch_map, REMOTE_GUESS_ತಲೆ_ALL);
	ಗಾಗಿ (ref = matches; ref; ref = ref->ಮುಂದಿನ) {
		string_list_append(&heads, strip_refshead(ref->name));
	}

	ಆದರೆ (!heads.nr)
		ಫಲಿತಾಂಶ = 1;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (heads.nr > 1)
		ಫಲಿತಾಂಶ = 1;
	ಇಲ್ಲದಿದ್ದರೆ
		ತಲೆ_name = xstrdup(heads.ವಸ್ತುಗಳು[0].string);

	ಆದರೆ (!ತಲೆ_name)
		ಹೋಗು cleanup;
	baremirror = is_bare_repository(the_repository) && remote->mirror;
	create_only = follow_remote_ತಲೆ == FOLLOW_REMOTE_ALWAYS ? 0 : !baremirror;
	ಆದರೆ (baremirror) {
		strbuf_addstr(&b_ತಲೆ, "HEAD");
		strbuf_addf(&b_remote_ತಲೆ, "refs/heads/%s", ತಲೆ_name);
	} ಇಲ್ಲದಿದ್ದರೆ {
		strbuf_addf(&b_ತಲೆ, "refs/remotes/%s/HEAD", remote->name);
		strbuf_addf(&b_remote_ತಲೆ, "refs/remotes/%s/%s", remote->name, ತಲೆ_name);
	}
	/* make sure it's valid */
	ಆದರೆ (!baremirror && !refs_ref_exists(refs, b_remote_ತಲೆ.buf)) {
		ಫಲಿತಾಂಶ = 1;
		ಹೋಗು cleanup;
	}
	was_detached = refs_update_symref_extended(refs, b_ತಲೆ.buf, b_remote_ತಲೆ.buf,
					"fetch", &b_local_ತಲೆ, create_only);
	ಆದರೆ (was_detached == -1) {
		ಫಲಿತಾಂಶ = 1;
		ಹೋಗು cleanup;
	}
	ಆದರೆ (verbosity >= 0 &&
		follow_remote_ತಲೆ == FOLLOW_REMOTE_WARN &&
		(!no_warn_branch || strcmp(no_warn_branch, ತಲೆ_name)))
		warn_ಗಣ_ತಲೆ(remote->name, ತಲೆ_name, &b_local_ತಲೆ, was_detached);

cleanup:
	ಬಿಡುಗಡೆ(ತಲೆ_name);
	free_refs(fetch_map);
	free_refs(matches);
	string_list_clear(&heads, 0);
	strbuf_release(&b_ತಲೆ);
	strbuf_release(&b_local_ತಲೆ);
	strbuf_release(&b_remote_ತಲೆ);
	ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

ರಚನೆ ref_rejection_ದತ್ತಾಂಶ {
	ಪೂರ್ಣಾಂಕ *retcode;
	bool conflict_ಸಂದೇಶ_shown;
	bool case_sensitive_ಸಂದೇಶ_shown;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *remote_name;
	ರಚನೆ strmap *rejected_refs;
};

ಸ್ಥಿರ ಶೂನ್ಯ ref_transaction_rejection_handler(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *refname,
					      ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *old_oid ಬಳಸದ,
					      ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *new_oid ಬಳಸದ,
					      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *old_ಗುರಿ ಬಳಸದ,
					      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *new_ಗುರಿ ಬಳಸದ,
					      ಎಣಿಕೆ ref_transaction_error err,
					      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *details,
					      ಶೂನ್ಯ *cb_ದತ್ತಾಂಶ)
{
	ರಚನೆ ref_rejection_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ = cb_ದತ್ತಾಂಶ;

	ಆದರೆ (err == REF_TRANSACTION_ERROR_CASE_CONFLICT && repo_ignore_case(the_repository) &&
	    !ದತ್ತಾಂಶ->case_sensitive_ಸಂದೇಶ_shown) {
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
		ದತ್ತಾಂಶ->case_sensitive_ಸಂದೇಶ_shown = true;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (err == REF_TRANSACTION_ERROR_NAME_CONFLICT &&
		   !ದತ್ತಾಂಶ->conflict_ಸಂದೇಶ_shown) {
		error(_("some local refs could not be updated; try running\n"
			" 'git remote prune %s' to remove any old, conflicting "
			"branches"), ದತ್ತಾಂಶ->remote_name);
		ದತ್ತಾಂಶ->conflict_ಸಂದೇಶ_shown = true;
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಆದರೆ (details)
			error("%s", details);
		ಇಲ್ಲದಿದ್ದರೆ
			error(_("fetching ref %s failed: %s"),
			      refname, ref_transaction_error_ಸಂದೇಶ(err));
	}

	strmap_put(ದತ್ತಾಂಶ->rejected_refs, refname, ಖಾಲಿ);
	*ದತ್ತಾಂಶ->retcode = 1;
}

/*
 * Commit the reference transaction. If it isn't an atomic transaction, handle
 * rejected updates as part of using batched updates.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ commit_ref_transaction(ರಚನೆ ref_transaction **transaction,
				  bool is_atomic, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *remote_name,
				  ರಚನೆ strmap *rejected_refs,
				  ರಚನೆ strbuf *err)
{
	ಪೂರ್ಣಾಂಕ retcode = ref_transaction_commit(*transaction, err);
	ಆದರೆ (retcode)
		ಹೋಗು ಹೊರಹೋಗು;

	ಆದರೆ (!is_atomic) {
		ರಚನೆ ref_rejection_ದತ್ತಾಂಶ ದತ್ತಾಂಶ = {
			.conflict_ಸಂದೇಶ_shown = 0,
			.remote_name = remote_name,
			.retcode = &retcode,
			.rejected_refs = rejected_refs,
		};

		ref_transaction_for_each_rejected_update(*transaction,
							 ref_transaction_rejection_handler,
							 &ದತ್ತಾಂಶ);
	}

ಹೊರಹೋಗು:
	ref_transaction_free(*transaction);
	*transaction = ಖಾಲಿ;
	ಹಿಂತಿರುಗಿ retcode;
}

ಸ್ಥಿರ ಶೂನ್ಯ collect_updated_tips(ರಚನೆ oidset *tips, ರಚನೆ ref *ref_map)
{
	ರಚನೆ ref *rm;
	ಗಾಗಿ (rm = ref_map; rm; rm = rm->ಮುಂದಿನ) {
		ರಚನೆ commit *commit;
		/*
		 * Shallow-rejected refs are not stored and their history
		 * is incomplete, so skip them.
		 */
		ಆದರೆ (rm->ಸ್ಥಿತಿ_2 == REF_ಸ್ಥಿತಿ_REJECT_SHALLOW)
			ಮುಂದುವರಿ;
		ಆದರೆ (is_null_oid(&rm->old_oid))
			ಮುಂದುವರಿ;
		ಆದರೆ (rm->peer_ref &&
		    oideq(&rm->old_oid, &rm->peer_ref->old_oid))
			ಮುಂದುವರಿ;
		commit = lookup_commit_reference_gently(the_repository,
							&rm->old_oid, 1);
		ಆದರೆ (commit)
			oidset_ಸೇರಿಸು(tips, &commit->object.oid);
	}
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ do_fetch(ರಚನೆ transport *transport,
		    ರಚನೆ refspec *rs,
		    ಸ್ಥಿರಾಂಕ ರಚನೆ fetch_config *config,
		    ರಚನೆ list_objects_filter_options *filter_options,
		    ರಚನೆ oidset *updated_tips)
{
	ರಚನೆ ref_transaction *transaction = ಖಾಲಿ;
	ರಚನೆ ref *ref_map = ಖಾಲಿ;
	ರಚನೆ display_state display_state = { 0 };
	ಪೂರ್ಣಾಂಕ autotags = (transport->remote->fetch_ಟ್ಯಾಗುಗಳು == 1);
	ಪೂರ್ಣಾಂಕ retcode = 0;
	ಸ್ಥಿರಾಂಕ ರಚನೆ ref *remote_refs;
	ರಚನೆ transport_ls_refs_options transport_ls_refs_options =
		TRANSPORT_LS_REFS_OPTIONS_INIT;
	ರಚನೆ fetch_ತಲೆ fetch_ತಲೆ = { 0 };
	ರಚನೆ strbuf err = STRBUF_INIT;
	ರಚನೆ ref_update_display_info_array display_array = { 0 };
	ರಚನೆ strmap rejected_refs = STRMAP_INIT;
	ಪೂರ್ಣಾಂಕ summary_ಅಗಲ = 0;
	ಪೂರ್ಣಾಂಕ follow_remote_ತಲೆ = FOLLOW_REMOTE_NEVER;

	ಆದರೆ (ಟ್ಯಾಗುಗಳು == ಟ್ಯಾಗುಗಳು_DEFAULT) {
		ಆದರೆ (transport->remote->fetch_ಟ್ಯಾಗುಗಳು == 2)
			ಟ್ಯಾಗುಗಳು = ಟ್ಯಾಗುಗಳು_ಗಣ;
		ಆದರೆ (transport->remote->fetch_ಟ್ಯಾಗುಗಳು == -1)
			ಟ್ಯಾಗುಗಳು = ಟ್ಯಾಗುಗಳು_UNSET;
	}

	/* if not appending, truncate FETCH_HEAD */
	ಆದರೆ (!append && write_fetch_ತಲೆ) {
		retcode = truncate_fetch_ತಲೆ();
		ಆದರೆ (retcode)
			ಹೋಗು cleanup;
	}

	ಆದರೆ (rs->nr) {
		refspec_ref_prefixes(rs, &transport_ls_refs_options.ref_prefixes);
	} ಇಲ್ಲದಿದ್ದರೆ {
		ರಚನೆ branch *branch = branch_get(ಖಾಲಿ);

		ಆದರೆ (transport->remote->fetch.nr) {
			refspec_ref_prefixes(&transport->remote->fetch,
					     &transport_ls_refs_options.ref_prefixes);
			/*
			 * See remote.c's handling of remote.<name>.followRemoteHEAD
			 * for the analogous, still-unresolved case.
			 */
			ಆದರೆ (transport->remote->follow_remote_ತಲೆ)
				follow_remote_ತಲೆ = transport->remote->follow_remote_ತಲೆ;
			ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (config->follow_remote_ತಲೆ_ನೋಡಿದ)
				follow_remote_ತಲೆ = get_follow_remote_ತಲೆ(config->follow_remote_ತಲೆ_raw);
			ಇಲ್ಲದಿದ್ದರೆ
				follow_remote_ತಲೆ = BUILTIN_FOLLOW_REMOTE_ತಲೆ_DFLT;
		}
		ಆದರೆ (branch && branch_has_ವಿಲೀನ_config(branch) &&
		    !strcmp(branch->remote_name, transport->remote->name)) {
			ಪೂರ್ಣಾಂಕ i;
			ಗಾಗಿ (i = 0; i < branch->ವಿಲೀನ_nr; i++) {
				strvec_ತಳ್ಳು(&transport_ls_refs_options.ref_prefixes,
					    branch->ವಿಲೀನ[i]->src);
			}
		}

		/*
		 * If there are no refs specified to fetch, then we just
		 * fetch HEAD; mention that to narrow the advertisement.
		 */
		ಆದರೆ (!transport_ls_refs_options.ref_prefixes.nr)
			strvec_ತಳ್ಳು(&transport_ls_refs_options.ref_prefixes,
				    "HEAD");
	}

	ಆದರೆ (ಟ್ಯಾಗುಗಳು == ಟ್ಯಾಗುಗಳು_ಗಣ || ಟ್ಯಾಗುಗಳು == ಟ್ಯಾಗುಗಳು_DEFAULT)
		strvec_ತಳ್ಳು(&transport_ls_refs_options.ref_prefixes,
			    "refs/tags/");

	ಆದರೆ (follow_remote_ತಲೆ != FOLLOW_REMOTE_NEVER)
		strvec_ತಳ್ಳು(&transport_ls_refs_options.ref_prefixes,
			    "HEAD");

	/*
	 * Only initiate ref listing if we have at least one ref we want to
	 * know about.
	 */
	ಆದರೆ (transport_ls_refs_options.ref_prefixes.nr) {
		trace2_region_enter("fetch", "remote_refs", the_repository);
		remote_refs = transport_get_remote_refs(transport,
							&transport_ls_refs_options);
		trace2_region_leave("fetch", "remote_refs", the_repository);
	} ಇಲ್ಲದಿದ್ದರೆ
		remote_refs = ಖಾಲಿ;

	transport_ls_refs_options_release(&transport_ls_refs_options);

	ref_map = get_ref_map(transport->remote, remote_refs, rs,
			      ಟ್ಯಾಗುಗಳು, &autotags);
	ಆದರೆ (!update_ತಲೆ_ok)
		check_not_current_branch(ref_map);

	retcode = open_fetch_ತಲೆ(&fetch_ತಲೆ);
	ಆದರೆ (retcode)
		ಹೋಗು cleanup;

	display_state_init(&display_state, ref_map, transport->url,
			   config->display_format);

	ಆದರೆ (atomic_fetch) {
		transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
							  0, &err);
		ಆದರೆ (!transaction) {
			retcode = -1;
			ಹೋಗು cleanup;
		}
	}

	ಆದರೆ (ಟ್ಯಾಗುಗಳು == ಟ್ಯಾಗುಗಳು_DEFAULT && autotags)
		transport_ಗಣ_option(transport, TRANS_OPT_FOLLOWTAGS, "1");
	ಆದರೆ (prune) {
		/*
		 * We only prune based on refspecs specified
		 * explicitly (via command line or configuration); we
		 * don't care whether --tags was specified.
		 */
		ಆದರೆ (rs->nr) {
			retcode = prune_refs(&display_state, rs, transaction, ref_map);
		} ಇಲ್ಲದಿದ್ದರೆ {
			retcode = prune_refs(&display_state, &transport->remote->fetch,
					     transaction, ref_map);
		}
		ಆದರೆ (retcode != 0)
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
	ಆದರೆ (!transaction) {
		transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
							  REF_TRANSACTION_ALLOW_FAILURE, &err);
		ಆದರೆ (!transaction) {
			retcode = -1;
			ಹೋಗು cleanup;
		}
	}

	ಆದರೆ (fetch_and_consume_refs(&display_state, transport, transaction, ref_map,
				   &fetch_ತಲೆ, config, &display_array)) {
		retcode = 1;
		ಹೋಗು cleanup;
	}

	/*
	 * If neither --no-tags nor --tags was specified, do automated tag
	 * following.
	 */
	ಆದರೆ (ಟ್ಯಾಗುಗಳು == ಟ್ಯಾಗುಗಳು_DEFAULT && autotags) {
		ರಚನೆ ref *ಟ್ಯಾಗುಗಳು_ref_map = ಖಾಲಿ, **ಬಾಲ = &ಟ್ಯಾಗುಗಳು_ref_map;

		ಕಂಡುಹಿಡಿ_non_local_ಟ್ಯಾಗುಗಳು(remote_refs, transaction, &ಟ್ಯಾಗುಗಳು_ref_map, &ಬಾಲ);
		ಆದರೆ (ಟ್ಯಾಗುಗಳು_ref_map) {
			/*
			 * If backfilling of tags fails then we want to tell
			 * the user so, but we have to continue regardless to
			 * populate upstream information of the references we
			 * have already fetched above. The exception though is
			 * when `--atomic` is passed: in that case we'll abort
			 * the transaction and don't commit anything.
			 */
			ಆದರೆ (backfill_ಟ್ಯಾಗುಗಳು(&display_state, transport, transaction, ಟ್ಯಾಗುಗಳು_ref_map,
					  &fetch_ತಲೆ, config, &display_array, filter_options))
				retcode = 1;
		}

		free_refs(ಟ್ಯಾಗುಗಳು_ref_map);
	}

	ಆದರೆ (retcode)
		ಹೋಗು cleanup;

	ಆದರೆ (verbosity >= 0)
		summary_ಅಗಲ = transport_summary_ಅಗಲ(ref_map);

	retcode = commit_ref_transaction(&transaction, atomic_fetch,
					 transport->remote->name,
					 &rejected_refs, &err);
	/*
	 * With '--atomic', bail out if the transaction fails. Without '--atomic',
	 * continue to fetch head and perform other post-fetch operations.
	 */
	ಆದರೆ (retcode && atomic_fetch)
		ಹೋಗು cleanup;

	commit_fetch_ತಲೆ(&fetch_ತಲೆ);

	collect_updated_tips(updated_tips, ref_map);

	ಆದರೆ (ಗಣ_upstream) {
		ರಚನೆ branch *branch = branch_get("HEAD");
		ರಚನೆ ref *rm;
		ರಚನೆ ref *ಮೂಲ_ref = ಖಾಲಿ;

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
		ಗಾಗಿ (rm = ref_map; rm; rm = rm->ಮುಂದಿನ) {
			ಆದರೆ (!rm->peer_ref) {
				ಆದರೆ (ಮೂಲ_ref) {
					warning(_("multiple branches detected, incompatible with --set-upstream"));
					ಹೋಗು cleanup;
				} ಇಲ್ಲದಿದ್ದರೆ {
					ಮೂಲ_ref = rm;
				}
			}
		}
		ಆದರೆ (ಮೂಲ_ref) {
			ಆದರೆ (!branch) {
				ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *shortname = ಮೂಲ_ref->name;
				skip_ಪೂರ್ವಪ್ರತ್ಯಯ(shortname, "refs/heads/", &shortname);

				warning(_("could not set upstream of HEAD to '%s' from '%s' when "
					  "it does not point to any branch."),
					shortname, transport->remote->name);
				ಹೋಗು cleanup;
			}

			ಆದರೆ (!strcmp(ಮೂಲ_ref->name, "HEAD") ||
			    starts_with(ಮೂಲ_ref->name, "refs/heads/"))
				install_branch_config(0,
						      branch->name,
						      transport->remote->name,
						      ಮೂಲ_ref->name);
			ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (starts_with(ಮೂಲ_ref->name, "refs/remotes/"))
				warning(_("not setting upstream for a remote remote-tracking branch"));
			ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (starts_with(ಮೂಲ_ref->name, "refs/tags/"))
				warning(_("not setting upstream for a remote tag"));
			ಇಲ್ಲದಿದ್ದರೆ
				warning(_("unknown branch type"));
		} ಇಲ್ಲದಿದ್ದರೆ {
			warning(_("no source branch found;\n"
				  "you need to specify exactly one branch with the --set-upstream option"));
		}
	}
	ಆದರೆ (follow_remote_ತಲೆ != FOLLOW_REMOTE_NEVER) {
		/*
		 * Way too many cases where this can go wrong so let's just
		 * ignore errors and fail silently for now.
		 */
		ಗಣ_ತಲೆ(remote_refs, transport->remote, follow_remote_ತಲೆ);
	}

cleanup:
	/*
	 * When using batched updates, we want to commit the non-rejected
	 * updates and also handle the rejections.
	 */
	ಆದರೆ (retcode && !atomic_fetch && transaction)
		commit_ref_transaction(&transaction, false,
				       transport->remote->name,
				       &rejected_refs, &err);

	ಗಾಗಿ (ಗಾತ್ರ_t i = 0; i < display_array.nr; i++) {
		ರಚನೆ ref_update_display_info *info = &display_array.info[i];

		ಆದರೆ (!info->failed && strmap_ಒಳಗೊಂಡಿದೆ(&rejected_refs, info->ref))
			ref_update_display_info_ಗಣ_failed(info);
		ref_update_display_info_display(info, &display_state, summary_ಅಗಲ);
		ref_update_display_info_free(info);
	}

	ಆದರೆ (retcode) {
		ಆದರೆ (err.len) {
			error("%s", err.buf);
			strbuf_reset(&err);
		}
		ಆದರೆ (transaction && ref_transaction_abort(transaction, &err) &&
		    err.len)
			error("%s", err.buf);
		transaction = ಖಾಲಿ;
	}

	ಆದರೆ (transaction)
		ref_transaction_free(transaction);

	ಬಿಡುಗಡೆ(display_array.info);
	strmap_clear(&rejected_refs, 0);
	display_state_release(&display_state);
	close_fetch_ತಲೆ(&fetch_ತಲೆ);
	strbuf_release(&err);
	free_refs(ref_map);
	ಹಿಂತಿರುಗಿ retcode;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ get_one_remote_for_fetch(ರಚನೆ remote *remote, ಶೂನ್ಯ *priv)
{
	ರಚನೆ string_list *list = priv;
	ಆದರೆ (!remote->skip_default_update)
		string_list_append(list, remote->name);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ ಕೂಡಿಸು_options_ಗಮ್ಯ_argv(ರಚನೆ strvec *argv,
				ಸ್ಥಿರಾಂಕ ರಚನೆ fetch_config *config)
{
	ಆದರೆ (dry_ಸತತ)
		strvec_ತಳ್ಳು(argv, "--dry-run");
	ಆದರೆ (prune != -1)
		strvec_ತಳ್ಳು(argv, prune ? "--prune" : "--no-prune");
	ಆದರೆ (prune_ಟ್ಯಾಗುಗಳು != -1)
		strvec_ತಳ್ಳು(argv, prune_ಟ್ಯಾಗುಗಳು ? "--prune-tags" : "--no-prune-tags");
	ಆದರೆ (update_ತಲೆ_ok)
		strvec_ತಳ್ಳು(argv, "--update-head-ok");
	ಆದರೆ (force)
		strvec_ತಳ್ಳು(argv, "--force");
	ಆದರೆ (keep)
		strvec_ತಳ್ಳು(argv, "--keep");
	ಆದರೆ (config->recurse_submodules == RECURSE_SUBMODULES_ON)
		strvec_ತಳ್ಳು(argv, "--recurse-submodules");
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (config->recurse_submodules == RECURSE_SUBMODULES_OFF)
		strvec_ತಳ್ಳು(argv, "--no-recurse-submodules");
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (config->recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND)
		strvec_ತಳ್ಳು(argv, "--recurse-submodules=on-demand");
	ಆದರೆ (ಟ್ಯಾಗುಗಳು == ಟ್ಯಾಗುಗಳು_ಗಣ)
		strvec_ತಳ್ಳು(argv, "--tags");
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಟ್ಯಾಗುಗಳು == ಟ್ಯಾಗುಗಳು_UNSET)
		strvec_ತಳ್ಳು(argv, "--no-tags");
	ಆದರೆ (verbosity >= 2)
		strvec_ತಳ್ಳು(argv, "-v");
	ಆದರೆ (verbosity >= 1)
		strvec_ತಳ್ಳು(argv, "-v");
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (verbosity < 0)
		strvec_ತಳ್ಳು(argv, "-q");
	ಆದರೆ (family == TRANSPORT_FAMILY_IPV4)
		strvec_ತಳ್ಳು(argv, "--ipv4");
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (family == TRANSPORT_FAMILY_IPV6)
		strvec_ತಳ್ಳು(argv, "--ipv6");
	ಆದರೆ (!write_fetch_ತಲೆ)
		strvec_ತಳ್ಳು(argv, "--no-write-fetch-head");
	ಆದರೆ (config->display_format == DISPLAY_FORMAT_PORCELAIN)
		strvec_pushf(argv, "--porcelain");
}

/* Fetch multiple remotes in parallel */

ರಚನೆ parallel_fetch_state {
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv;
	ರಚನೆ string_list *remotes;
	ಪೂರ್ಣಾಂಕ ಮುಂದಿನ, ಫಲಿತಾಂಶ;
	ಸ್ಥಿರಾಂಕ ರಚನೆ fetch_config *config;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ fetch_ಮುಂದಿನ_remote(ರಚನೆ ಮಗು_process *cp,
			     ರಚನೆ strbuf *ಹೊರಹೋಗು ಬಳಸದ,
			     ಶೂನ್ಯ *cb, ಶೂನ್ಯ **task_cb)
{
	ರಚನೆ parallel_fetch_state *state = cb;
	ಅಕ್ಷರ *remote;

	ಆದರೆ (state->ಮುಂದಿನ < 0 || state->ಮುಂದಿನ >= state->remotes->nr)
		ಹಿಂತಿರುಗಿ 0;

	remote = state->remotes->ವಸ್ತುಗಳು[state->ಮುಂದಿನ++].string;
	*task_cb = remote;

	strvec_pushv(&cp->ವಾದಗಳು, state->argv);
	strvec_ತಳ್ಳು(&cp->ವಾದಗಳು, remote);
	cp->git_cmd = 1;

	ಆದರೆ (verbosity >= 0 && state->config->display_format != DISPLAY_FORMAT_PORCELAIN)
		ಬರೆ(_("Fetching %s\n"), remote);

	ಹಿಂತಿರುಗಿ 1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ fetch_failed_ಗಮ್ಯ_ಆರಂಭ(ರಚನೆ strbuf *ಹೊರಹೋಗು ಬಳಸದ,
				 ಶೂನ್ಯ *cb, ಶೂನ್ಯ *task_cb)
{
	ರಚನೆ parallel_fetch_state *state = cb;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *remote = task_cb;

	state->ಫಲಿತಾಂಶ = error(_("could not fetch %s"), remote);

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ fetch_ಪೂರ್ಣಗೊಂಡ(ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ, ರಚನೆ strbuf *ಹೊರಹೋಗು,
			  ಶೂನ್ಯ *cb, ಶೂನ್ಯ *task_cb)
{
	ರಚನೆ parallel_fetch_state *state = cb;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *remote = task_cb;

	ಆದರೆ (ಫಲಿತಾಂಶ) {
		strbuf_addf(ಹೊರಹೋಗು, _("could not fetch '%s' (exit code: %d)\n"),
			    remote, ಫಲಿತಾಂಶ);
		state->ಫಲಿತಾಂಶ = -1;
	}

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ fetch_ಅಪವರ್ತ್ಯ(ರಚನೆ string_list *list, ಪೂರ್ಣಾಂಕ max_children,
			  ಸ್ಥಿರಾಂಕ ರಚನೆ fetch_config *config)
{
	ಪೂರ್ಣಾಂಕ i, ಫಲಿತಾಂಶ = 0;
	ರಚನೆ strvec argv = STRVEC_INIT;

	ಆದರೆ (!append && write_fetch_ತಲೆ) {
		ಪೂರ್ಣಾಂಕ errcode = truncate_fetch_ತಲೆ();
		ಆದರೆ (errcode)
			ಹಿಂತಿರುಗಿ errcode;
	}

	/*
	 * Cancel out the fetch.bundleURI config when running subprocesses,
	 * to avoid fetching from the same bundle list multiple times.
	 */
	strvec_pushl(&argv, "-c", "fetch.bundleURI=",
		     "fetch", "--append", "--no-auto-gc",
		     "--no-write-commit-graph", ಖಾಲಿ);
	ಗಾಗಿ (i = 0; i < server_options.nr; i++)
		strvec_pushf(&argv, "--server-option=%s", server_options.ವಸ್ತುಗಳು[i].string);
	ಕೂಡಿಸು_options_ಗಮ್ಯ_argv(&argv, config);

	ಆದರೆ (max_children != 1 && list->nr != 1) {
		ರಚನೆ parallel_fetch_state state = { argv.v, list, 0, 0, config };
		ಸ್ಥಿರಾಂಕ ರಚನೆ ಸತತ_process_parallel_opts opts = {
			.tr2_category = "fetch",
			.tr2_label = "parallel/fetch",

			.processes = max_children,

			.get_ಮುಂದಿನ_task = &fetch_ಮುಂದಿನ_remote,
			.ಆರಂಭ_failure = &fetch_failed_ಗಮ್ಯ_ಆರಂಭ,
			.task_ಪೂರ್ಣಗೊಂಡ = &fetch_ಪೂರ್ಣಗೊಂಡ,
			.ದತ್ತಾಂಶ = &state,
		};

		strvec_ತಳ್ಳು(&argv, "--end-of-options");

		ಸತತ_processes_parallel(&opts);
		ಫಲಿತಾಂಶ = state.ಫಲಿತಾಂಶ;
	} ಇಲ್ಲದಿದ್ದರೆ
		ಗಾಗಿ (i = 0; i < list->nr; i++) {
			ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name = list->ವಸ್ತುಗಳು[i].string;
			ರಚನೆ ಮಗು_process cmd = ಮಗು_PROCESS_INIT;

			strvec_pushv(&cmd.ವಾದಗಳು, argv.v);
			strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, name);
			ಆದರೆ (verbosity >= 0 && config->display_format != DISPLAY_FORMAT_PORCELAIN)
				ಬರೆ(_("Fetching %s\n"), name);
			cmd.git_cmd = 1;
			ಆದರೆ (ಸತತ_command(&cmd)) {
				error(_("could not fetch %s"), name);
				ಫಲಿತಾಂಶ = 1;
			}
		}

	strvec_clear(&argv);
	ಹಿಂತಿರುಗಿ !!ಫಲಿತಾಂಶ;
}

/*
 * Fetching from the promisor remote should use the given filter-spec
 * or inherit the default filter-spec from the config.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ fetch_one_ಸಿದ್ಧತೆ_partial(ರಚನೆ remote *remote,
					   ರಚನೆ list_objects_filter_options *filter_options)
{
	/*
	 * Explicit --no-filter argument overrides everything, regardless
	 * of any prior partial clones and fetches.
	 */
	ಆದರೆ (filter_options->no_filter)
		ಹಿಂತಿರುಗಿ;

	/*
	 * If no prior partial clone/fetch and the current fetch DID NOT
	 * request a partial-fetch, do a normal fetch.
	 */
	ಆದರೆ (!repo_has_promisor_remote(the_repository) && !filter_options->choice)
		ಹಿಂತಿರುಗಿ;

	/*
	 * If this is a partial-fetch request, we enable partial on
	 * this repo if not already enabled and remember the given
	 * filter-spec as the default for subsequent fetches to this
	 * remote if there is currently no default filter-spec.
	 */
	ಆದರೆ (filter_options->choice) {
		partial_clone_ರಿಜಿಸ್ಟರ್(remote->name, filter_options);
		ಹಿಂತಿರುಗಿ;
	}

	/*
	 * Do a partial-fetch from the promisor remote using either the
	 * explicitly given filter-spec or inherit the filter-spec from
	 * the config.
	 */
	ಆದರೆ (!filter_options->choice)
		partial_clone_get_default_filter_spec(filter_options, remote->name);
	ಹಿಂತಿರುಗಿ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ fetch_one(ರಚನೆ remote *remote, ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
		     ಪೂರ್ಣಾಂಕ prune_ಟ್ಯಾಗುಗಳು_ok, ಪೂರ್ಣಾಂಕ use_stdin_refspecs,
		     ಸ್ಥಿರಾಂಕ ರಚನೆ fetch_config *config,
		     ರಚನೆ list_objects_filter_options *filter_options,
		     ರಚನೆ oidset *updated_tips)
{
	ರಚನೆ refspec rs = REFSPEC_INIT_FETCH(the_hash_algo);
	ಪೂರ್ಣಾಂಕ i;
	ಪೂರ್ಣಾಂಕ exit_ಸಂಕೇತ;
	ಪೂರ್ಣಾಂಕ maybe_prune_ಟ್ಯಾಗುಗಳು;
	ಪೂರ್ಣಾಂಕ remote_ಮೂಲಕ_config = remote_is_configured(remote, 0);

	ಆದರೆ (!remote)
		die(_("no remote repository specified; please specify either a URL or a\n"
		      "remote name from which new revisions should be fetched"));

	gtransport = prepare_transport(remote, 1, filter_options);

	ಆದರೆ (prune < 0) {
		/* no command line request */
		ಆದರೆ (0 <= remote->prune)
			prune = remote->prune;
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (0 <= config->prune)
			prune = config->prune;
		ಇಲ್ಲದಿದ್ದರೆ
			prune = PRUNE_BY_DEFAULT;
	}

	ಆದರೆ (prune_ಟ್ಯಾಗುಗಳು < 0) {
		/* no command line request */
		ಆದರೆ (0 <= remote->prune_ಟ್ಯಾಗುಗಳು)
			prune_ಟ್ಯಾಗುಗಳು = remote->prune_ಟ್ಯಾಗುಗಳು;
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (0 <= config->prune_ಟ್ಯಾಗುಗಳು)
			prune_ಟ್ಯಾಗುಗಳು = config->prune_ಟ್ಯಾಗುಗಳು;
		ಇಲ್ಲದಿದ್ದರೆ
			prune_ಟ್ಯಾಗುಗಳು = PRUNE_ಟ್ಯಾಗುಗಳು_BY_DEFAULT;
	}

	maybe_prune_ಟ್ಯಾಗುಗಳು = prune_ಟ್ಯಾಗುಗಳು_ok && prune_ಟ್ಯಾಗುಗಳು;
	ಆದರೆ (maybe_prune_ಟ್ಯಾಗುಗಳು && remote_ಮೂಲಕ_config)
		refspec_append(&remote->fetch, ಟ್ಯಾಗ್_REFSPEC);

	ಆದರೆ (maybe_prune_ಟ್ಯಾಗುಗಳು && (argc || !remote_ಮೂಲಕ_config))
		refspec_append(&rs, ಟ್ಯಾಗ್_REFSPEC);

	ಗಾಗಿ (i = 0; i < argc; i++) {
		ಆದರೆ (!strcmp(argv[i], "tag")) {
			i++;
			ಆದರೆ (i >= argc)
				die(_("you need to specify a tag name"));

			refspec_appendf(&rs, "refs/tags/%s:refs/tags/%s",
					argv[i], argv[i]);
		} ಇಲ್ಲದಿದ್ದರೆ {
			refspec_append(&rs, argv[i]);
		}
	}

	ಆದರೆ (use_stdin_refspecs) {
		ರಚನೆ strbuf ಸಾಲು = STRBUF_INIT;
		ಆಗ (strbuf_getline_lf(&ಸಾಲು, stdin) != EOF)
			refspec_append(&rs, ಸಾಲು.buf);
		strbuf_release(&ಸಾಲು);
	}

	ಆದರೆ (server_options.nr)
		gtransport->server_options = &server_options;

	sigchain_ತಳ್ಳು_common(unlock_pack_on_signal);
	atexit(unlock_pack_atexit);
	sigchain_ತಳ್ಳು(SIGPIPE, SIG_IGN);
	exit_ಸಂಕೇತ = do_fetch(gtransport, &rs, config, filter_options,
			     updated_tips);
	sigchain_ತೆಗೆ(SIGPIPE);
	refspec_clear(&rs);
	transport_disconnect(gtransport);
	gtransport = ಖಾಲಿ;
	ಹಿಂತಿರುಗಿ exit_ಸಂಕೇತ;
}

ಪೂರ್ಣಾಂಕ cmd_fetch(ಪೂರ್ಣಾಂಕ argc,
	      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
	      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
	      ರಚನೆ repository *repo ಬಳಸದ)
{
	ರಚನೆ fetch_config config = {
		.display_format = DISPLAY_FORMAT_ತುಂಬಿದ,
		.follow_remote_ತಲೆ_raw = ಖಾಲಿ,
		.follow_remote_ತಲೆ_ನೋಡಿದ = 0,
		.prune = -1,
		.prune_ಟ್ಯಾಗುಗಳು = -1,
		.show_forced_updates = 1,
		.recurse_submodules = RECURSE_SUBMODULES_DEFAULT,
		.parallel = 1,
		.submodule_fetch_jobs = -1,
	};
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *submodule_ಪೂರ್ವಪ್ರತ್ಯಯ = "";
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *bundle_uri;
	ರಚನೆ string_list list = STRING_LIST_INIT_DUP;
	ರಚನೆ list_objects_filter_options filter_options = LIST_OBJECTS_FILTER_INIT;
	ರಚನೆ remote *remote = ಖಾಲಿ;
	ಪೂರ್ಣಾಂಕ all = -1, ಅಪವರ್ತ್ಯ = 0;
	ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ = 0;
	ಪೂರ್ಣಾಂಕ prune_ಟ್ಯಾಗುಗಳು_ok = 1;
	ಪೂರ್ಣಾಂಕ enable_auto_gc = 1;
	ಪೂರ್ಣಾಂಕ unshallow = 0;
	ಪೂರ್ಣಾಂಕ max_jobs = -1;
	ಪೂರ್ಣಾಂಕ recurse_submodules_cli = RECURSE_SUBMODULES_DEFAULT;
	ಪೂರ್ಣಾಂಕ recurse_submodules_default = RECURSE_SUBMODULES_ON_DEMAND;
	ಪೂರ್ಣಾಂಕ fetch_write_commit_ನಕ್ಷೆ = -1;
	ಪೂರ್ಣಾಂಕ stdin_refspecs = 0;
	ಪೂರ್ಣಾಂಕ negotiate_only = 0;
	ಪೂರ್ಣಾಂಕ porcelain = 0;
	ಪೂರ್ಣಾಂಕ i;
	ಎಣಿಕೆ {
		ನಕ್ಷೆ_WRITE_REACHABLE,
		ನಕ್ಷೆ_WRITE_TIPS,
		ನಕ್ಷೆ_WRITE_SKIP,
	} ನಕ್ಷೆ_write_ಬಹುಲಕ = ನಕ್ಷೆ_WRITE_REACHABLE;
	ರಚನೆ oidset updated_tips = OIDSET_INIT;

	ರಚನೆ option builtin_fetch_options[] = {
		OPT__VERBOSITY(&verbosity),
		OPT_BOOL(0, "all", &all,
			 N_("fetch from all remotes")),
		OPT_BOOL(0, "set-upstream", &ಗಣ_upstream,
			 N_("set upstream for git pull/fetch")),
		OPT_BOOL('a', "append", &append,
			 N_("append to .git/FETCH_HEAD instead of overwriting")),
		OPT_BOOL(0, "atomic", &atomic_fetch,
			 N_("use atomic transaction to update references")),
		OPT_STRING(0, "upload-pack", &upload_pack, N_("path"),
			   N_("path to upload pack on remote end")),
		OPT__FORCE(&force, N_("force overwrite of local reference"), 0),
		OPT_BOOL('m', "multiple", &ಅಪವರ್ತ್ಯ,
			 N_("fetch from multiple remotes")),
		OPT_ಗಣ_INT('t', "tags", &ಟ್ಯಾಗುಗಳು,
			    N_("fetch all tags and associated objects"), ಟ್ಯಾಗುಗಳು_ಗಣ),
		OPT_ಗಣ_INT('n', ಖಾಲಿ, &ಟ್ಯಾಗುಗಳು,
			    N_("do not fetch all tags (--no-tags)"), ಟ್ಯಾಗುಗಳು_UNSET),
		OPT_INTEGER('j', "jobs", &max_jobs,
			    N_("number of submodules fetched in parallel")),
		OPT_BOOL(0, "prefetch", &prefetch,
			 N_("modify the refspec to place all refs within refs/prefetch/")),
		OPT_BOOL('p', "prune", &prune,
			 N_("prune remote-tracking branches no longer on remote")),
		OPT_BOOL('P', "prune-tags", &prune_ಟ್ಯಾಗುಗಳು,
			 N_("prune local tags no longer on remote and clobber changed tags")),
		OPT_CALLBACK_F(0, "recurse-submodules", &recurse_submodules_cli, N_("on-demand"),
			    N_("control recursive fetching of submodules"),
			    PARSE_OPT_OPTARG, option_fetch_parse_recurse_submodules),
		OPT_BOOL(0, "dry-run", &dry_ಸತತ,
			 N_("dry run")),
		OPT_BOOL(0, "porcelain", &porcelain, N_("machine-readable output")),
		OPT_BOOL(0, "write-fetch-head", &write_fetch_ತಲೆ,
			 N_("write fetched references to the FETCH_HEAD file")),
		OPT_BOOL('k', "keep", &keep, N_("keep downloaded pack")),
		OPT_BOOL('u', "update-head-ok", &update_ತಲೆ_ok,
			    N_("allow updating of HEAD ref")),
		OPT_BOOL(0, "progress", &ಪ್ರಗತಿ, N_("force progress reporting")),
		OPT_STRING(0, "depth", &ಆಳ, N_("depth"),
			   N_("deepen history of shallow clone")),
		OPT_STRING(0, "shallow-since", &deepen_since, N_("time"),
			   N_("deepen history of shallow repository based on time")),
		OPT_STRING_LIST(0, "shallow-exclude", &deepen_not, N_("ref"),
				N_("deepen history of shallow clone, excluding ref")),
		OPT_INTEGER(0, "deepen", &deepen_relative,
			    N_("deepen history of shallow clone")),
		OPT_ಗಣ_INT_F(0, "unshallow", &unshallow,
			      N_("convert to a complete repository"),
			      1, PARSE_OPT_NONEG),
		OPT_ಗಣ_INT_F(0, "refetch", &refetch,
			      N_("re-fetch without negotiating common commits"),
			      1, PARSE_OPT_NONEG),
		{
			.type = OPTION_STRING,
			.long_name = "submodule-prefix",
			.ಮೌಲ್ಯ = &submodule_ಪೂರ್ವಪ್ರತ್ಯಯ,
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
		OPT_BOOL(0, "write-commit-graph", &fetch_write_commit_ನಕ್ಷೆ,
			 N_("write the commit-graph after fetching")),
		OPT_BOOL(0, "stdin", &stdin_refspecs,
			 N_("accept refspecs from stdin")),
		OPT_ಕೊನೆ()
	};

	filter_options.allow_auto_filter = 1;

	refspec_init_fetch(&refmap, the_hash_algo);

	packet_trace_ತಟಸ್ಥ("fetch");

	/* Record the command line for the reflog */
	strbuf_addstr(&default_rla, "fetch");
	ಗಾಗಿ (i = 1; i < argc; i++) {
		/* This handles non-URLs gracefully */
		ಅಕ್ಷರ *anon = transport_anonymize_url(argv[i]);

		strbuf_addf(&default_rla, " %s", anon);
		ಬಿಡುಗಡೆ(anon);
	}

	repo_config(the_repository, git_fetch_config, &config);
	ಆದರೆ (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_ತುಂಬಿದ_ಸೂಚ್ಯಂಕ = 0;
	}

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ,
			     builtin_fetch_options, builtin_fetch_usage, 0);

	ಆದರೆ (recurse_submodules_cli != RECURSE_SUBMODULES_DEFAULT)
		config.recurse_submodules = recurse_submodules_cli;

	ಆದರೆ (negotiate_only) {
		ಆಯ್ಕೆ (recurse_submodules_cli) {
		ಸ್ಥಿತಿ RECURSE_SUBMODULES_OFF:
		ಸ್ಥಿತಿ RECURSE_SUBMODULES_DEFAULT:
			/*
			 * --negotiate-only should never recurse into
			 * submodules. Skip it by setting recurse_submodules to
			 * RECURSE_SUBMODULES_OFF.
			 */
			config.recurse_submodules = RECURSE_SUBMODULES_OFF;
			ನಿಲ್ಲಿಸು;

		ಪೂರ್ವನಿಯೋಜಿತ:
			die(_("options '%s' and '%s' cannot be used together"),
			    "--negotiate-only", "--recurse-submodules");
		}
	}

	ಆದರೆ (config.recurse_submodules != RECURSE_SUBMODULES_OFF) {
		ಪೂರ್ಣಾಂಕ *sfjc = config.submodule_fetch_jobs == -1
			    ? &config.submodule_fetch_jobs : ಖಾಲಿ;
		ಪೂರ್ಣಾಂಕ *rs = config.recurse_submodules == RECURSE_SUBMODULES_DEFAULT
			  ? &config.recurse_submodules : ಖಾಲಿ;

		fetch_config_from_gitmodules(the_repository, sfjc, rs);
	}


	ಆದರೆ (porcelain) {
		ಆಯ್ಕೆ (recurse_submodules_cli) {
		ಸ್ಥಿತಿ RECURSE_SUBMODULES_OFF:
		ಸ್ಥಿತಿ RECURSE_SUBMODULES_DEFAULT:
			/*
			 * Reference updates in submodules would be ambiguous
			 * in porcelain mode, so we reject this combination.
			 */
			config.recurse_submodules = RECURSE_SUBMODULES_OFF;
			ನಿಲ್ಲಿಸು;

		ಪೂರ್ವನಿಯೋಜಿತ:
			die(_("options '%s' and '%s' cannot be used together"),
			    "--porcelain", "--recurse-submodules");
		}

		config.display_format = DISPLAY_FORMAT_PORCELAIN;
	}

	ಆದರೆ (deepen_relative) {
		ಆದರೆ (deepen_relative < 0)
			die(_("negative depth in --deepen is not supported"));
		ಆದರೆ (ಆಳ)
			die(_("options '%s' and '%s' cannot be used together"), "--deepen", "--depth");
		ಆಳ = xstrfmt("%d", deepen_relative);
	}
	ಆದರೆ (unshallow) {
		ಆದರೆ (ಆಳ)
			die(_("options '%s' and '%s' cannot be used together"), "--depth", "--unshallow");
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!is_repository_shallow(the_repository))
			die(_("--unshallow on a complete repository does not make sense"));
		ಇಲ್ಲದಿದ್ದರೆ
			ಆಳ = xstrfmt("%d", INFINITE_ಆಳ);
	}

	/* no need to be strict, transport_set_option() will validate it again */
	ಆದರೆ (ಆಳ && atoi(ಆಳ) < 1)
		die(_("depth %s is not a positive number"), ಆಳ);
	ಆದರೆ (ಆಳ || deepen_since || deepen_not.nr)
		deepen = 1;

	/* FETCH_HEAD never gets updated in --dry-run mode */
	ಆದರೆ (dry_ಸತತ)
		write_fetch_ತಲೆ = 0;

	ಆದರೆ (!max_jobs)
		max_jobs = online_cpus();

	ಆದರೆ (!repo_config_get_string_tmp(the_repository, "fetch.bundleuri", &bundle_uri) &&
	    fetch_bundle_uri(the_repository, bundle_uri, ಖಾಲಿ))
		warning(_("failed to fetch bundles from '%s'"), bundle_uri);

	ಆದರೆ (all < 0) {
		/*
		 * no --[no-]all given;
		 * only use config option if no remote was explicitly specified
		 */
		all = (!argc) ? config.all : 0;
	}

	ಆದರೆ (all) {
		ಆದರೆ (argc == 1)
			die(_("fetch --all does not take a repository argument"));
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (argc > 1)
			die(_("fetch --all does not make sense with refspecs"));

		(ಶೂನ್ಯ) for_each_remote(get_one_remote_for_fetch, &list);

		/* do not do fetch_multiple() of one */
		ಆದರೆ (list.nr == 1)
			remote = remote_get(list.ವಸ್ತುಗಳು[0].string);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (argc == 0) {
		/* No arguments -- use default remote */
		remote = remote_get(ಖಾಲಿ);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಅಪವರ್ತ್ಯ) {
		/* All arguments are assumed to be remotes or groups */
		ಗಾಗಿ (i = 0; i < argc; i++)
			ಆದರೆ (!ಕೂಡಿಸು_remote_or_ಗುಂಪು(argv[i], &list))
				die(_("no such remote or remote group: %s"),
				    argv[i]);
	} ಇಲ್ಲದಿದ್ದರೆ {
		/* Single remote or group */
		(ಶೂನ್ಯ) ಕೂಡಿಸು_remote_or_ಗುಂಪು(argv[0], &list);
		ಆದರೆ (list.nr > 1) {
			/* More than one remote */
			ಆದರೆ (argc > 1)
				die(_("fetching a group and specifying refspecs does not make sense"));
		} ಇಲ್ಲದಿದ್ದರೆ {
			/* Zero or one remotes */
			remote = remote_get(argv[0]);
			prune_ಟ್ಯಾಗುಗಳು_ok = (argc == 1);
			argc--;
			argv++;
		}
	}
	string_list_remove_duplicates(&list, 0);

	ಆದರೆ (negotiate_only) {
		ರಚನೆ oidset acked_commits = OIDSET_INIT;
		ರಚನೆ oidset_iter iter;
		ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid;

		trace2_region_enter("fetch", "negotiate-only", the_repository);
		ಆದರೆ (!remote)
			die(_("must supply remote when using --negotiate-only"));
		gtransport = prepare_transport(remote, 1, &filter_options);

		ಆದರೆ (!gtransport->smart_options) {
			warning(_("protocol does not support --negotiate-only, exiting"));
			ಫಲಿತಾಂಶ = 1;
			trace2_region_leave("fetch", "negotiate-only", the_repository);
			ಹೋಗು cleanup;
		}
		ಆದರೆ (!gtransport->smart_options->negotiation_restrict_tips)
			die(_("%s needs one or more %s"), "--negotiate-only",
			    "--negotiation-restrict=*");

		gtransport->smart_options->acked_commits = &acked_commits;

		ಆದರೆ (server_options.nr)
			gtransport->server_options = &server_options;
		ಫಲಿತಾಂಶ = transport_fetch_refs(gtransport, ಖಾಲಿ);
		gtransport->smart_options->acked_commits = ಖಾಲಿ;

		oidset_iter_init(&acked_commits, &iter);
		ಆಗ ((oid = oidset_iter_ಮುಂದಿನ(&iter)))
			ಬರೆ("%s\n", oid_ಗಮ್ಯ_hex(oid));
		oidset_clear(&acked_commits);
		trace2_region_leave("fetch", "negotiate-only", the_repository);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (remote) {
		ಆದರೆ (filter_options.choice || repo_has_promisor_remote(the_repository)) {
			trace2_region_enter("fetch", "setup-partial", the_repository);
			fetch_one_ಸಿದ್ಧತೆ_partial(remote, &filter_options);
			trace2_region_leave("fetch", "setup-partial", the_repository);
		}
		trace2_region_enter("fetch", "fetch-one", the_repository);
		ಫಲಿತಾಂಶ = fetch_one(remote, argc, argv, prune_ಟ್ಯಾಗುಗಳು_ok, stdin_refspecs,
				   &config, &filter_options, &updated_tips);
		ಆದರೆ (prepare_commit_ನಕ್ಷೆ(the_repository)) {
			ಆದರೆ (oidset_ಗಾತ್ರ(&updated_tips))
				ನಕ್ಷೆ_write_ಬಹುಲಕ = ನಕ್ಷೆ_WRITE_TIPS;
			ಇಲ್ಲದಿದ್ದರೆ
				ನಕ್ಷೆ_write_ಬಹುಲಕ = ನಕ್ಷೆ_WRITE_SKIP;
		}
		trace2_region_leave("fetch", "fetch-one", the_repository);
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಪೂರ್ಣಾಂಕ max_children = max_jobs;

		ಆದರೆ (filter_options.choice)
			die(_("--filter can only be used with the remote "
			      "configured in extensions.partialclone"));

		ಆದರೆ (atomic_fetch)
			die(_("--atomic can only be used when fetching "
			      "from one remote"));

		ಆದರೆ (stdin_refspecs)
			die(_("--stdin can only be used when fetching "
			      "from one remote"));

		ಆದರೆ (max_children < 0)
			max_children = config.parallel;

		/* TODO should this also die if we have a previous partial-clone? */
		trace2_region_enter("fetch", "fetch-multiple", the_repository);
		ಫಲಿತಾಂಶ = fetch_ಅಪವರ್ತ್ಯ(&list, max_children, &config);
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
	ಆದರೆ (!ಫಲಿತಾಂಶ && remote && (config.recurse_submodules != RECURSE_SUBMODULES_OFF)) {
		ರಚನೆ strvec options = STRVEC_INIT;
		ಪೂರ್ಣಾಂಕ max_children = max_jobs;

		ಆದರೆ (max_children < 0)
			max_children = config.submodule_fetch_jobs;
		ಆದರೆ (max_children < 0)
			max_children = config.parallel;

		ಕೂಡಿಸು_options_ಗಮ್ಯ_argv(&options, &config);
		trace2_region_enter_printf("fetch", "recurse-submodule", the_repository, "%s", submodule_ಪೂರ್ವಪ್ರತ್ಯಯ);
		ಫಲಿತಾಂಶ = fetch_submodules(the_repository,
					  &options,
					  submodule_ಪೂರ್ವಪ್ರತ್ಯಯ,
					  config.recurse_submodules,
					  recurse_submodules_default,
					  verbosity < 0,
					  max_children);
		trace2_region_leave_printf("fetch", "recurse-submodule", the_repository, "%s", submodule_ಪೂರ್ವಪ್ರತ್ಯಯ);
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
	ಆದರೆ (negotiate_only)
		ಹೋಗು cleanup;

	prepare_repo_settings(the_repository);
	ಆದರೆ (fetch_write_commit_ನಕ್ಷೆ > 0 ||
	    (fetch_write_commit_ನಕ್ಷೆ < 0 &&
	     the_repository->settings.fetch_write_commit_ನಕ್ಷೆ)) {
		ಪೂರ್ಣಾಂಕ commit_ನಕ್ಷೆ_flags = COMMIT_ನಕ್ಷೆ_WRITE_ವಿಭಜಿಸು;

		ಆದರೆ (ಪ್ರಗತಿ)
			commit_ನಕ್ಷೆ_flags |= COMMIT_ನಕ್ಷೆ_WRITE_ಪ್ರಗತಿ;

		ಆದರೆ (ನಕ್ಷೆ_write_ಬಹುಲಕ != ನಕ್ಷೆ_WRITE_SKIP) {
			trace2_region_enter("fetch", "write-commit-graph",
					    the_repository);
			ಆದರೆ (ನಕ್ಷೆ_write_ಬಹುಲಕ == ನಕ್ಷೆ_WRITE_TIPS)
				write_commit_ನಕ್ಷೆ(
					the_repository,
					the_repository->objects->ಮೂಲ->ಮಾರ್ಗ,
					ಖಾಲಿ, &updated_tips,
					commit_ನಕ್ಷೆ_flags, ಖಾಲಿ);
			ಇಲ್ಲದಿದ್ದರೆ
				write_commit_ನಕ್ಷೆ_reachable(
					the_repository,
					the_repository->objects->ಮೂಲ->ಮಾರ್ಗ,
					commit_ನಕ್ಷೆ_flags, ಖಾಲಿ);
			trace2_region_leave("fetch", "write-commit-graph",
					    the_repository);
		}
	}

	ಆದರೆ (enable_auto_gc) {
		ಆದರೆ (refetch) {
			/*
			 * Hint auto-maintenance strongly to encourage repacking,
			 * but respect config settings disabling it.
			 */
			ಪೂರ್ಣಾಂಕ opt_val;
			ಆದರೆ (repo_config_get_int(the_repository, "gc.autopacklimit", &opt_val))
				opt_val = -1;
			ಆದರೆ (opt_val != 0)
				git_config_ತಳ್ಳು_parameter("gc.autoPackLimit=1");

			ಆದರೆ (repo_config_get_int(the_repository, "maintenance.incremental-repack.auto", &opt_val))
				opt_val = -1;
			ಆದರೆ (opt_val != 0)
				git_config_ತಳ್ಳು_parameter("maintenance.incremental-repack.auto=-1");
		}
		ಸತತ_auto_maintenance(the_repository, verbosity < 0);
	}

 cleanup:
	oidset_clear(&updated_tips);
	string_list_clear(&list, 0);
	list_objects_filter_release(&filter_options);
	ಬಿಡುಗಡೆ(config.follow_remote_ತಲೆ_raw);
	ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}
