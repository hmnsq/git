/*
 * "git fetch"
 */

#నిర్వచించు USE_THE_REPOSITORY_VARIABLE
#నిర్వచించు DISABLE_SIGN_COMPARE_WARNINGS

#అన్తర్భావ "builtin.h"
#అన్తర్భావ "advice.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "environment.h"
#అన్తర్భావ "hex.h"
#అన్తర్భావ "refs.h"
#అన్తర్భావ "refspec.h"
#అన్తర్భావ "object-name.h"
#అన్తర్భావ "odb.h"
#అన్తర్భావ "odb/source.h"
#అన్తర్భావ "oidset.h"
#అన్తర్భావ "oid-array.h"
#అన్తర్భావ "commit.h"
#అన్తర్భావ "string-list.h"
#అన్తర్భావ "remote.h"
#అన్తర్భావ "transport.h"
#అన్తర్భావ "run-command.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "sigchain.h"
#అన్తర్భావ "submodule-config.h"
#అన్తర్భావ "submodule.h"
#అన్తర్భావ "connected.h"
#అన్తర్భావ "strvec.h"
#అన్తర్భావ "utf8.h"
#అన్తర్భావ "pager.h"
#అన్తర్భావ "path.h"
#అన్తర్భావ "pkt-line.h"
#అన్తర్భావ "list-objects-filter-options.h"
#అన్తర్భావ "commit-reach.h"
#అన్తర్భావ "branch.h"
#అన్తర్భావ "promisor-remote.h"
#అన్తర్భావ "commit-graph.h"
#అన్తర్భావ "shallow.h"
#అన్తర్భావ "trace.h"
#అన్తర్భావ "trace2.h"
#అన్తర్భావ "bundle-uri.h"

#నిర్వచించు FORCED_UPDATES_DELAY_WARNING_లోపల_MS (10 * 1000)

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి builtin_fetch_usage[] = {
	N_("git fetch [<options>] [<repository> [<refspec>...]]"),
	N_("git fetch [<options>] <group>"),
	N_("git fetch --multiple [<options>] [(<repository>|<group>)...]"),
	N_("git fetch --all [<options>]"),
	ఖాళీ
};

గణన {
	ట్యాగులు_UNSET = 0,
	ట్యాగులు_DEFAULT = 1,
	ట్యాగులు_సమితి = 2
};

గణన display_format {
	DISPLAY_FORMAT_నిండిన,
	DISPLAY_FORMAT_COMPACT,
	DISPLAY_FORMAT_PORCELAIN,
};

నిర్మాణం display_state {
	నిర్మాణం strbuf buf;

	పూర్ణాంకం refcol_వెడల్పు;
	గణన display_format format;

	అక్షరం *url;
	పూర్ణాంకం url_len, shown_url;
};

స్థిర uint64_t forced_updates_ms = 0;
స్థిర పూర్ణాంకం prefetch = 0;
స్థిర పూర్ణాంకం prune = -1; /* unspecified */
#నిర్వచించు PRUNE_BY_DEFAULT 0 /* do we prune by default? */

స్థిర పూర్ణాంకం prune_ట్యాగులు = -1; /* unspecified */
#నిర్వచించు PRUNE_ట్యాగులు_BY_DEFAULT 0 /* do we prune tags by default? */

స్థిర పూర్ణాంకం append, dry_నిరంతరం, force, keep, update_తల_ok;
స్థిర పూర్ణాంకం write_fetch_తల = 1;
స్థిర పూర్ణాంకం verbosity, deepen_relative, సమితి_upstream, refetch;
స్థిర పూర్ణాంకం పురోగతి = -1;
స్థిర పూర్ణాంకం ట్యాగులు = ట్యాగులు_DEFAULT, update_shallow, deepen;
స్థిర పూర్ణాంకం atomic_fetch;
స్థిర గణన transport_family family;
స్థిర స్థిరరాశి అక్షరం *లోతు;
స్థిర స్థిరరాశి అక్షరం *deepen_since;
స్థిర స్థిరరాశి అక్షరం *upload_pack;
స్థిర నిర్మాణం string_list deepen_not = STRING_LIST_INIT_NODUP;
స్థిర నిర్మాణం strbuf default_rla = STRBUF_INIT;
స్థిర నిర్మాణం transport *gtransport;
స్థిర నిర్మాణం transport *gsecondary;
స్థిర నిర్మాణం refspec refmap;
స్థిర నిర్మాణం string_list server_options = STRING_LIST_INIT_DUP;
స్థిర నిర్మాణం string_list negotiation_restrict = STRING_LIST_INIT_NODUP;
స్థిర నిర్మాణం string_list negotiation_include = STRING_LIST_INIT_NODUP;

నిర్మాణం fetch_config {
	గణన display_format display_format;
	అక్షరం *follow_remote_తల_raw;
	పూర్ణాంకం follow_remote_తల_చూసిన;
	పూర్ణాంకం all;
	పూర్ణాంకం prune;
	పూర్ణాంకం prune_ట్యాగులు;
	పూర్ణాంకం show_forced_updates;
	పూర్ణాంకం recurse_submodules;
	పూర్ణాంకం parallel;
	పూర్ణాంకం submodule_fetch_jobs;
};

స్థిర పూర్ణాంకం git_fetch_config(స్థిరరాశి అక్షరం *k, స్థిరరాశి అక్షరం *v,
			    స్థిరరాశి నిర్మాణం config_context *ctx, శూన్య *cb)
{
	నిర్మాణం fetch_config *fetch_config = cb;

	ఒకవేళ (!strcmp(k, "fetch.all")) {
		fetch_config->all = git_config_bool(k, v);
		తిరిగి 0;
	}

	ఒకవేళ (!strcmp(k, "fetch.prune")) {
		fetch_config->prune = git_config_bool(k, v);
		తిరిగి 0;
	}

	ఒకవేళ (!strcmp(k, "fetch.prunetags")) {
		fetch_config->prune_ట్యాగులు = git_config_bool(k, v);
		తిరిగి 0;
	}

	ఒకవేళ (!strcmp(k, "fetch.showforcedupdates")) {
		fetch_config->show_forced_updates = git_config_bool(k, v);
		తిరిగి 0;
	}

	ఒకవేళ (!strcmp(k, "submodule.recurse")) {
		పూర్ణాంకం r = git_config_bool(k, v) ?
			RECURSE_SUBMODULES_ON : RECURSE_SUBMODULES_OFF;
		fetch_config->recurse_submodules = r;
		తిరిగి 0;
	}

	ఒకవేళ (!strcmp(k, "submodule.fetchjobs")) {
		fetch_config->submodule_fetch_jobs = parse_submodule_fetchjobs(k, v, ctx->kvi);
		తిరిగి 0;
	} లేకపోతే ఒకవేళ (!strcmp(k, "fetch.recursesubmodules")) {
		fetch_config->recurse_submodules = parse_fetch_recurse_submodules_arg(k, v);
		తిరిగి 0;
	}

	ఒకవేళ (!strcmp(k, "fetch.parallel")) {
		fetch_config->parallel = git_config_int(k, v, ctx->kvi);
		ఒకవేళ (fetch_config->parallel < 0)
			die(_("fetch.parallel cannot be negative"));
		ఒకవేళ (!fetch_config->parallel)
			fetch_config->parallel = online_cpus();
		తిరిగి 0;
	}

	ఒకవేళ (!strcmp(k, "fetch.output")) {
		ఒకవేళ (!v)
			తిరిగి config_error_nonbool(k);
		లేకపోతే ఒకవేళ (!strcasecmp(v, "full"))
			fetch_config->display_format = DISPLAY_FORMAT_నిండిన;
		లేకపోతే ఒకవేళ (!strcasecmp(v, "compact"))
			fetch_config->display_format = DISPLAY_FORMAT_COMPACT;
		లేకపోతే
			die(_("invalid value for '%s': '%s'"),
			    "fetch.output", v);
		తిరిగి 0;
	}

	ఒకవేళ (!strcmp(k, "fetch.followremotehead")) {
		విడుదల(fetch_config->follow_remote_తల_raw);
		fetch_config->follow_remote_తల_raw = xstrdup_or_null(v);
		fetch_config->follow_remote_తల_చూసిన = 1;
		తిరిగి 0;
	}

	తిరిగి git_default_config(k, v, ctx, cb);
}

స్థిర గణన follow_remote_తల_settings get_follow_remote_తల(స్థిరరాశి అక్షరం *setting)
{
	ఒకవేళ (!setting)
		die(_("missing value for 'fetch.followRemoteHEAD'"));
	లేకపోతే ఒకవేళ (!strcmp(setting, "never"))
		తిరిగి FOLLOW_REMOTE_NEVER;
	లేకపోతే ఒకవేళ (!strcmp(setting, "create"))
		తిరిగి FOLLOW_REMOTE_CREATE;
	లేకపోతే ఒకవేళ (!strcmp(setting, "warn"))
		తిరిగి FOLLOW_REMOTE_WARN;
	లేకపోతే ఒకవేళ (!strcmp(setting, "always"))
		తిరిగి FOLLOW_REMOTE_ALWAYS;
	warning(_("unrecognized fetch.followRemoteHEAD value '%s' ignored"), setting);
	తిరిగి BUILTIN_FOLLOW_REMOTE_తల_DFLT;
}

స్థిర పూర్ణాంకం parse_refmap_arg(స్థిరరాశి నిర్మాణం option *opt, స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	BUG_ON_OPT_NEG(unset);

	/*
	 * "git fetch --refmap='' origin foo"
	 * can be used to tell the command not to store anywhere
	 */
	refspec_append(opt->విలువ, arg);

	తిరిగి 0;
}

స్థిర శూన్య unlock_pack(గుర్తులేని పూర్ణాంకం flags)
{
	ఒకవేళ (gtransport)
		transport_unlock_pack(gtransport, flags);
	ఒకవేళ (gsecondary)
		transport_unlock_pack(gsecondary, flags);
}

స్థిర శూన్య unlock_pack_atexit(శూన్య)
{
	unlock_pack(0);
}

స్థిర శూన్య unlock_pack_on_signal(పూర్ణాంకం signo)
{
	unlock_pack(TRANSPORT_UNLOCK_PACK_లోపల_SIGNAL_HANDLER);
	sigchain_తీయి(signo);
	లేపు(signo);
}

స్థిర శూన్య కూడు_విలీనం_config(నిర్మాణం ref **తల,
			   స్థిరరాశి నిర్మాణం ref *remote_refs,
		           నిర్మాణం branch *branch,
		           నిర్మాణం ref ***తోక)
{
	పూర్ణాంకం i;

	కొరకు (i = 0; i < branch->విలీనం_nr; i++) {
		నిర్మాణం ref *rm, **old_తోక = *తోక;
		నిర్మాణం refspec_అంశం refspec;

		కొరకు (rm = *తల; rm; rm = rm->తదుపరి) {
			ఒకవేళ (branch_విలీనం_matches(branch, i, rm->name)) {
				rm->fetch_తల_స్థితి = FETCH_తల_విలీనం;
				ఆపు;
			}
		}
		ఒకవేళ (rm)
			కొనసాగు;

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
		memset(&refspec, 0, పరిమాణం(refspec));
		refspec.src = branch->విలీనం[i]->src;
		get_fetch_map(remote_refs, &refspec, తోక, 1);
		కొరకు (rm = *old_తోక; rm; rm = rm->తదుపరి)
			rm->fetch_తల_స్థితి = FETCH_తల_విలీనం;
	}
}

స్థిర శూన్య create_fetch_oidset(నిర్మాణం ref **తల, నిర్మాణం oidset *బహిర్గతం)
{
	నిర్మాణం ref *rm = *తల;
	అయితే (rm) {
		oidset_చొప్పించు(బహిర్గతం, &rm->old_oid);
		rm = rm->తదుపరి;
	}
}

నిర్మాణం refname_hash_నమోదు {
	నిర్మాణం hashmap_నమోదు ent;
	నిర్మాణం object_id oid;
	పూర్ణాంకం ignore;
	అక్షరం refname[FLEX_ARRAY];
};

స్థిర పూర్ణాంకం refname_hash_నమోదు_cmp(స్థిరరాశి శూన్య *hashmap_cmp_fn_దత్తాంశం వాడని,
				  స్థిరరాశి నిర్మాణం hashmap_నమోదు *eptr,
				  స్థిరరాశి నిర్మాణం hashmap_నమోదు *నమోదు_or_కీలకం,
				  స్థిరరాశి శూన్య *keydata)
{
	స్థిరరాశి నిర్మాణం refname_hash_నమోదు *e1, *e2;

	e1 = container_యొక్క(eptr, స్థిరరాశి నిర్మాణం refname_hash_నమోదు, ent);
	e2 = container_యొక్క(నమోదు_or_కీలకం, స్థిరరాశి నిర్మాణం refname_hash_నమోదు, ent);
	తిరిగి strcmp(e1->refname, keydata ? keydata : e2->refname);
}

స్థిర నిర్మాణం refname_hash_నమోదు *refname_hash_కూడు(నిర్మాణం hashmap *map,
						   స్థిరరాశి అక్షరం *refname,
						   స్థిరరాశి నిర్మాణం object_id *oid)
{
	నిర్మాణం refname_hash_నమోదు *ent;
	కొలత_t len = అక్షరశ్రేణి_పొడవు(refname);

	FLEX_ALLOC_MEM(ent, refname, refname, len);
	hashmap_నమోదు_init(&ent->ent, strhash(refname));
	oidcpy(&ent->oid, oid);
	hashmap_కూడు(map, &ent->ent);
	తిరిగి ent;
}

స్థిర పూర్ణాంకం కూడు_one_refname(స్థిరరాశి నిర్మాణం reference *ref, శూన్య *cbdata)
{
	నిర్మాణం hashmap *refname_map = cbdata;

	(శూన్య) refname_hash_కూడు(refname_map, ref->name, ref->oid);
	తిరిగి 0;
}

స్థిర శూన్య refname_hash_init(నిర్మాణం hashmap *map)
{
	hashmap_init(map, refname_hash_నమోదు_cmp, ఖాళీ, 0);
}

స్థిర పూర్ణాంకం refname_hash_exists(నిర్మాణం hashmap *map, స్థిరరాశి అక్షరం *refname)
{
	తిరిగి !!hashmap_get_from_hash(map, strhash(refname), refname);
}

స్థిర శూన్య clear_అంశం(నిర్మాణం refname_hash_నమోదు *అంశం)
{
	అంశం->ignore = 1;
}


స్థిర శూన్య కూడు_already_queued_ట్యాగులు(స్థిరరాశి అక్షరం *refname,
				    స్థిరరాశి నిర్మాణం object_id *old_oid వాడని,
				    స్థిరరాశి నిర్మాణం object_id *new_oid,
				    శూన్య *cb_దత్తాంశం)
{
	నిర్మాణం hashmap *queued_ట్యాగులు = cb_దత్తాంశం;
	ఒకవేళ (starts_with(refname, "refs/tags/") && new_oid)
		(శూన్య) refname_hash_కూడు(queued_ట్యాగులు, refname, new_oid);
}

స్థిర శూన్య కనుగొను_non_local_ట్యాగులు(స్థిరరాశి నిర్మాణం ref *refs,
				నిర్మాణం ref_transaction *transaction,
				నిర్మాణం ref **తల,
				నిర్మాణం ref ***తోక)
{
	నిర్మాణం hashmap existing_refs;
	నిర్మాణం hashmap remote_refs;
	నిర్మాణం oidset fetch_oids = OIDSET_INIT;
	నిర్మాణం string_list remote_refs_list = STRING_LIST_INIT_NODUP;
	నిర్మాణం string_list_అంశం *remote_ref_అంశం;
	స్థిరరాశి నిర్మాణం ref *ref;
	నిర్మాణం refname_hash_నమోదు *అంశం = ఖాళీ;

	refname_hash_init(&existing_refs);
	refname_hash_init(&remote_refs);
	create_fetch_oidset(తల, &fetch_oids);

	refs_for_each_ref(get_main_ref_store(the_repository), కూడు_one_refname,
			  &existing_refs);

	/*
	 * If we already have a transaction, then we need to filter out all
	 * tags which have already been queued up.
	 */
	ఒకవేళ (transaction)
		ref_transaction_for_each_queued_update(transaction,
						       కూడు_already_queued_ట్యాగులు,
						       &existing_refs);

	కొరకు (ref = refs; ref; ref = ref->తదుపరి) {
		ఒకవేళ (!starts_with(ref->name, "refs/tags/"))
			కొనసాగు;

		/*
		 * The peeled ref always follows the matching base
		 * ref, so if we see a peeled ref that we don't want
		 * to fetch then we can mark the ref entry in the list
		 * as one to ignore by setting util to NULL.
		 */
		ఒకవేళ (చివర్లు_with(ref->name, "^{}")) {
			ఒకవేళ (అంశం &&
			    !odb_has_object(the_repository->objects, &ref->old_oid, 0) &&
			    !oidset_కలిగియుంది(&fetch_oids, &ref->old_oid) &&
			    !odb_has_object(the_repository->objects, &అంశం->oid, 0) &&
			    !oidset_కలిగియుంది(&fetch_oids, &అంశం->oid))
				clear_అంశం(అంశం);
			అంశం = ఖాళీ;
			కొనసాగు;
		}

		/*
		 * If item is non-NULL here, then we previously saw a
		 * ref not followed by a peeled reference, so we need
		 * to check if it is a lightweight tag that we want to
		 * fetch.
		 */
		ఒకవేళ (అంశం &&
		    !odb_has_object(the_repository->objects, &అంశం->oid, 0) &&
		    !oidset_కలిగియుంది(&fetch_oids, &అంశం->oid))
			clear_అంశం(అంశం);

		అంశం = ఖాళీ;

		/* skip duplicates and refs that we already have */
		ఒకవేళ (refname_hash_exists(&remote_refs, ref->name) ||
		    refname_hash_exists(&existing_refs, ref->name))
			కొనసాగు;

		అంశం = refname_hash_కూడు(&remote_refs, ref->name, &ref->old_oid);
		string_list_చొప్పించు(&remote_refs_list, ref->name);
	}
	hashmap_clear_and_free(&existing_refs, నిర్మాణం refname_hash_నమోదు, ent);

	/*
	 * We may have a final lightweight tag that needs to be
	 * checked to see if it needs fetching.
	 */
	ఒకవేళ (అంశం &&
	    !odb_has_object(the_repository->objects, &అంశం->oid, 0) &&
	    !oidset_కలిగియుంది(&fetch_oids, &అంశం->oid))
		clear_అంశం(అంశం);

	/*
	 * For all the tags in the remote_refs_list,
	 * add them to the list of refs to be fetched
	 */
	for_each_string_list_అంశం(remote_ref_అంశం, &remote_refs_list) {
		స్థిరరాశి అక్షరం *refname = remote_ref_అంశం->string;
		నిర్మాణం ref *rm;
		గుర్తులేని పూర్ణాంకం hash = strhash(refname);

		అంశం = hashmap_get_నమోదు_from_hash(&remote_refs, hash, refname,
					నిర్మాణం refname_hash_నమోదు, ent);
		ఒకవేళ (!అంశం)
			BUG("unseen remote ref?");

		/* Unless we have already decided to ignore this item... */
		ఒకవేళ (అంశం->ignore)
			కొనసాగు;

		rm = alloc_ref(అంశం->refname);
		rm->peer_ref = alloc_ref(అంశం->refname);
		oidcpy(&rm->old_oid, &అంశం->oid);
		**తోక = rm;
		*తోక = &rm->తదుపరి;
	}
	hashmap_clear_and_free(&remote_refs, నిర్మాణం refname_hash_నమోదు, ent);
	string_list_clear(&remote_refs_list, 0);
	oidset_clear(&fetch_oids);
}

స్థిర శూన్య filter_prefetch_refspec(నిర్మాణం refspec *rs)
{
	పూర్ణాంకం i;

	ఒకవేళ (!prefetch)
		తిరిగి;

	కొరకు (i = 0; i < rs->nr; i++) {
		నిర్మాణం strbuf new_dst = STRBUF_INIT;
		అక్షరం *old_dst;
		స్థిరరాశి అక్షరం *sub = ఖాళీ;

		ఒకవేళ (rs->వస్తువులు[i].negative)
			కొనసాగు;
		ఒకవేళ (!rs->వస్తువులు[i].dst ||
		    (rs->వస్తువులు[i].src &&
		     starts_with(rs->వస్తువులు[i].src,
				 ref_namespace[NAMESPACE_ట్యాగులు].ref))) {
			పూర్ణాంకం j;

			refspec_అంశం_clear(&rs->వస్తువులు[i]);

			కొరకు (j = i + 1; j < rs->nr; j++)
				rs->వస్తువులు[j - 1] = rs->వస్తువులు[j];
			rs->nr--;
			i--;
			కొనసాగు;
		}

		old_dst = rs->వస్తువులు[i].dst;
		strbuf_addstr(&new_dst, ref_namespace[NAMESPACE_PREFETCH].ref);

		/*
		 * If old_dst starts with "refs/", then place
		 * sub after that prefix. Otherwise, start at
		 * the beginning of the string.
		 */
		ఒకవేళ (!skip_ఉపసర్గ(old_dst, "refs/", &sub))
			sub = old_dst;
		strbuf_addstr(&new_dst, sub);

		rs->వస్తువులు[i].dst = strbuf_detach(&new_dst, ఖాళీ);
		rs->వస్తువులు[i].force = 1;

		విడుదల(old_dst);
	}
}

స్థిర నిర్మాణం ref *get_ref_map(నిర్మాణం remote *remote,
			       స్థిరరాశి నిర్మాణం ref *remote_refs,
			       నిర్మాణం refspec *rs,
			       పూర్ణాంకం ట్యాగులు, పూర్ణాంకం *autotags)
{
	పూర్ణాంకం i;
	నిర్మాణం ref *rm;
	నిర్మాణం ref *ref_map = ఖాళీ;
	నిర్మాణం ref **తోక = &ref_map;

	/* opportunistically-updated references: */
	నిర్మాణం ref *orefs = ఖాళీ, **oref_తోక = &orefs;

	నిర్మాణం hashmap existing_refs;
	పూర్ణాంకం existing_refs_populated = 0;

	filter_prefetch_refspec(rs);
	ఒకవేళ (remote)
		filter_prefetch_refspec(&remote->fetch);

	ఒకవేళ (rs->nr) {
		నిర్మాణం refspec *fetch_refspec;

		కొరకు (i = 0; i < rs->nr; i++) {
			get_fetch_map(remote_refs, &rs->వస్తువులు[i], &తోక, 0);
			ఒకవేళ (rs->వస్తువులు[i].dst && rs->వస్తువులు[i].dst[0])
				*autotags = 1;
		}
		/* Merge everything on the command line (but not --tags) */
		కొరకు (rm = ref_map; rm; rm = rm->తదుపరి)
			rm->fetch_తల_స్థితి = FETCH_తల_విలీనం;

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
		ఒకవేళ (refmap.nr)
			fetch_refspec = &refmap;
		లేకపోతే
			fetch_refspec = &remote->fetch;

		కొరకు (i = 0; i < fetch_refspec->nr; i++)
			get_fetch_map(ref_map, &fetch_refspec->వస్తువులు[i], &oref_తోక, 1);
	} లేకపోతే ఒకవేళ (refmap.nr) {
		die("--refmap option is only meaningful with command-line refspec(s)");
	} లేకపోతే {
		/* Use the defaults */
		నిర్మాణం branch *branch = branch_get(ఖాళీ);
		పూర్ణాంకం has_విలీనం = branch_has_విలీనం_config(branch);
		ఒకవేళ (remote &&
		    (remote->fetch.nr ||
		     /* Note: has_merge implies non-NULL branch->remote_name */
		     (has_విలీనం && !strcmp(branch->remote_name, remote->name)))) {
			కొరకు (i = 0; i < remote->fetch.nr; i++) {
				get_fetch_map(remote_refs, &remote->fetch.వస్తువులు[i], &తోక, 0);
				ఒకవేళ (remote->fetch.వస్తువులు[i].dst &&
				    remote->fetch.వస్తువులు[i].dst[0])
					*autotags = 1;
				ఒకవేళ (!i && !has_విలీనం && ref_map &&
				    !remote->fetch.వస్తువులు[0].నమూనా)
					ref_map->fetch_తల_స్థితి = FETCH_తల_విలీనం;
			}
			/*
			 * if the remote we're fetching from is the same
			 * as given in branch.<name>.remote, we add the
			 * ref given in branch.<name>.merge, too.
			 *
			 * Note: has_merge implies non-NULL branch->remote_name
			 */
			ఒకవేళ (has_విలీనం &&
			    !strcmp(branch->remote_name, remote->name))
				కూడు_విలీనం_config(&ref_map, remote_refs, branch, &తోక);
		} లేకపోతే ఒకవేళ (!prefetch) {
			ref_map = get_remote_ref(remote_refs, "HEAD");
			ఒకవేళ (!ref_map)
				die(_("couldn't find remote ref HEAD"));
			ref_map->fetch_తల_స్థితి = FETCH_తల_విలీనం;
			తోక = &ref_map->తదుపరి;
		}
	}

	ఒకవేళ (ట్యాగులు == ట్యాగులు_సమితి) {
		నిర్మాణం refspec_అంశం ట్యాగ్_refspec;

		/* also fetch all tags */
		refspec_అంశం_init_నెట్టు(&ట్యాగ్_refspec, ట్యాగ్_REFSPEC,
				       the_hash_algo);
		get_fetch_map(remote_refs, &ట్యాగ్_refspec, &తోక, 0);
		refspec_అంశం_clear(&ట్యాగ్_refspec);
	} లేకపోతే ఒకవేళ (ట్యాగులు == ట్యాగులు_DEFAULT && *autotags) {
		కనుగొను_non_local_ట్యాగులు(remote_refs, ఖాళీ, &ref_map, &తోక);
	}

	/* Now append any refs to be updated opportunistically: */
	*తోక = orefs;
	కొరకు (rm = orefs; rm; rm = rm->తదుపరి) {
		rm->fetch_తల_స్థితి = FETCH_తల_IGNORE;
		తోక = &rm->తదుపరి;
	}

	/*
	 * apply negative refspecs first, before we remove duplicates. This is
	 * necessary as negative refspecs might remove an otherwise conflicting
	 * duplicate.
	 */
	ఒకవేళ (rs->nr)
		ref_map = apply_negative_refspecs(ref_map, rs);
	లేకపోతే
		ref_map = apply_negative_refspecs(ref_map, &remote->fetch);

	ref_map = ref_remove_duplicates(ref_map);

	కొరకు (rm = ref_map; rm; rm = rm->తదుపరి) {
		ఒకవేళ (rm->peer_ref) {
			స్థిరరాశి అక్షరం *refname = rm->peer_ref->name;
			నిర్మాణం refname_hash_నమోదు *peer_అంశం;
			గుర్తులేని పూర్ణాంకం hash = strhash(refname);

			ఒకవేళ (!existing_refs_populated) {
				refname_hash_init(&existing_refs);
				refs_for_each_ref(get_main_ref_store(the_repository),
						  కూడు_one_refname,
						  &existing_refs);
				existing_refs_populated = 1;
			}

			peer_అంశం = hashmap_get_నమోదు_from_hash(&existing_refs,
						hash, refname,
						నిర్మాణం refname_hash_నమోదు, ent);
			ఒకవేళ (peer_అంశం) {
				నిర్మాణం object_id *old_oid = &peer_అంశం->oid;
				oidcpy(&rm->peer_ref->old_oid, old_oid);
			}
		}
	}
	ఒకవేళ (existing_refs_populated)
		hashmap_clear_and_free(&existing_refs, నిర్మాణం refname_hash_నమోదు, ent);

	తిరిగి ref_map;
}

స్థిర పూర్ణాంకం s_update_ref(స్థిరరాశి అక్షరం *action,
			నిర్మాణం ref *ref,
			నిర్మాణం ref_transaction *transaction,
			పూర్ణాంకం check_old)
{
	అక్షరం *సందేశం;
	అక్షరం *rla = getenv("GIT_REFLOG_ACTION");
	నిర్మాణం strbuf err = STRBUF_INIT;
	పూర్ణాంకం ret;

	ఒకవేళ (dry_నిరంతరం)
		తిరిగి 0;
	ఒకవేళ (!rla)
		rla = default_rla.buf;
	సందేశం = xstrfmt("%s: %s", rla, action);

	ret = ref_transaction_update(transaction, ref->name, &ref->new_oid,
				     check_old ? &ref->old_oid : ఖాళీ,
				     ఖాళీ, ఖాళీ, 0, సందేశం, &err);

	ఒకవేళ (ret)
		error("%s", err.buf);
	strbuf_release(&err);
	విడుదల(సందేశం);
	తిరిగి ret;
}

స్థిర పూర్ణాంకం refcol_వెడల్పు(స్థిరరాశి నిర్మాణం ref *ref_map, పూర్ణాంకం compact_format)
{
	స్థిరరాశి నిర్మాణం ref *ref;
	పూర్ణాంకం max, వెడల్పు = 10;

	max = పదం_columns();
	ఒకవేళ (compact_format)
		max = max * 2 / 3;

	కొరకు (ref = ref_map; ref; ref = ref->తదుపరి) {
		పూర్ణాంకం rlen, llen = 0, len;

		ఒకవేళ (ref->స్థితి_2 == REF_స్థితి_REJECT_SHALLOW ||
		    !ref->peer_ref ||
		    !strcmp(ref->name, "HEAD"))
			కొనసాగు;

		/* uptodate lines are only shown on high verbosity level */
		ఒకవేళ (verbosity <= 0 && oideq(&ref->peer_ref->old_oid, &ref->old_oid))
			కొనసాగు;

		rlen = utf8_strwidth(prettify_refname(ref->name));
		ఒకవేళ (!compact_format)
			llen = utf8_strwidth(prettify_refname(ref->peer_ref->name));

		/*
		 * rough estimation to see if the output line is too long and
		 * should not be counted (we can't do precise calculation
		 * anyway because we don't know if the error explanation part
		 * will be printed in update_local_ref)
		 */
		len = 21 /* flag and summary */ + rlen + 4 /* -> */ + llen;
		ఒకవేళ (len >= max)
			కొనసాగు;

		ఒకవేళ (వెడల్పు < rlen)
			వెడల్పు = rlen;
	}

	తిరిగి వెడల్పు;
}

స్థిర శూన్య display_state_init(నిర్మాణం display_state *display_state, నిర్మాణం ref *ref_map,
			       స్థిరరాశి అక్షరం *raw_url, గణన display_format format)
{
	పూర్ణాంకం i;

	memset(display_state, 0, పరిమాణం(*display_state));
	strbuf_init(&display_state->buf, 0);
	display_state->format = format;

	ఒకవేళ (raw_url)
		display_state->url = transport_anonymize_url(raw_url);
	లేకపోతే
		display_state->url = xstrdup("foreign");

	display_state->url_len = అక్షరశ్రేణి_పొడవు(display_state->url);
	కొరకు (i = display_state->url_len - 1; 0 <= i && display_state->url[i] == '/'; i--)
		;
	display_state->url_len = i + 1;
	ఒకవేళ (4 < i && !అక్షరశ్రేణి_పోలిక(".git", display_state->url + i - 3, 4))
		display_state->url_len = i - 3;

	ఒకవేళ (verbosity < 0)
		తిరిగి;

	ఎంపిక (display_state->format) {
	స్థితి DISPLAY_FORMAT_నిండిన:
	స్థితి DISPLAY_FORMAT_COMPACT:
		display_state->refcol_వెడల్పు = refcol_వెడల్పు(ref_map,
							   display_state->format == DISPLAY_FORMAT_COMPACT);
		ఆపు;
	స్థితి DISPLAY_FORMAT_PORCELAIN:
		/* We don't need to precompute anything here. */
		ఆపు;
	అప్రమేయ:
		BUG("unexpected display format %d", display_state->format);
	}
}

స్థిర శూన్య display_state_release(నిర్మాణం display_state *display_state)
{
	strbuf_release(&display_state->buf);
	విడుదల(display_state->url);
}

స్థిర శూన్య print_remote_గమ్యం_local(నిర్మాణం display_state *display_state,
				  స్థిరరాశి అక్షరం *remote, స్థిరరాశి అక్షరం *local)
{
	strbuf_addf(&display_state->buf, "%-*s -> %s",
		    display_state->refcol_వెడల్పు, remote, local);
}

స్థిర పూర్ణాంకం కనుగొను_and_replace(నిర్మాణం strbuf *haystack,
			    స్థిరరాశి అక్షరం *needle,
			    స్థిరరాశి అక్షరం *placeholder)
{
	స్థిరరాశి అక్షరం *p = ఖాళీ;
	పూర్ణాంకం plen, nlen;

	nlen = అక్షరశ్రేణి_పొడవు(needle);
	ఒకవేళ (చివర్లు_with(haystack->buf, needle))
		p = haystack->buf + haystack->len - nlen;
	లేకపోతే
		p = అక్షరశ్రేణి_శోధన(haystack->buf, needle);
	ఒకవేళ (!p)
		తిరిగి 0;

	ఒకవేళ (p > haystack->buf && p[-1] != '/')
		తిరిగి 0;

	plen = అక్షరశ్రేణి_పొడవు(p);
	ఒకవేళ (plen > nlen && p[nlen] != '/')
		తిరిగి 0;

	strbuf_splice(haystack, p - haystack->buf, nlen,
		      placeholder, అక్షరశ్రేణి_పొడవు(placeholder));
	తిరిగి 1;
}

స్థిర శూన్య print_compact(నిర్మాణం display_state *display_state,
			  స్థిరరాశి అక్షరం *remote, స్థిరరాశి అక్షరం *local)
{
	నిర్మాణం strbuf r = STRBUF_INIT;
	నిర్మాణం strbuf l = STRBUF_INIT;

	ఒకవేళ (!strcmp(remote, local)) {
		strbuf_addf(&display_state->buf, "%-*s -> *", display_state->refcol_వెడల్పు, remote);
		తిరిగి;
	}

	strbuf_addstr(&r, remote);
	strbuf_addstr(&l, local);

	ఒకవేళ (!కనుగొను_and_replace(&r, local, "*"))
		కనుగొను_and_replace(&l, remote, "*");
	print_remote_గమ్యం_local(display_state, r.buf, l.buf);

	strbuf_release(&r);
	strbuf_release(&l);
}

స్థిర శూన్య display_ref_update(నిర్మాణం display_state *display_state, అక్షరం సంకేతం_2,
			       స్థిరరాశి అక్షరం *summary, స్థిరరాశి అక్షరం *error,
			       స్థిరరాశి అక్షరం *remote, స్థిరరాశి అక్షరం *local,
			       స్థిరరాశి నిర్మాణం object_id *old_oid,
			       స్థిరరాశి నిర్మాణం object_id *new_oid,
			       పూర్ణాంకం summary_వెడల్పు)
{
	FILE *f = stderr;

	ఒకవేళ (verbosity < 0)
		తిరిగి;

	strbuf_reset(&display_state->buf);

	ఎంపిక (display_state->format) {
	స్థితి DISPLAY_FORMAT_నిండిన:
	స్థితి DISPLAY_FORMAT_COMPACT: {
		పూర్ణాంకం వెడల్పు;

		ఒకవేళ (!display_state->shown_url) {
			strbuf_addf(&display_state->buf, _("From %.*s\n"),
				    display_state->url_len, display_state->url);
			display_state->shown_url = 1;
		}

		వెడల్పు = (summary_వెడల్పు + అక్షరశ్రేణి_పొడవు(summary) - gettext_వెడల్పు(summary));
		remote = prettify_refname(remote);
		local = prettify_refname(local);

		strbuf_addf(&display_state->buf, " %c %-*s ", సంకేతం_2, వెడల్పు, summary);

		ఒకవేళ (display_state->format != DISPLAY_FORMAT_COMPACT)
			print_remote_గమ్యం_local(display_state, remote, local);
		లేకపోతే
			print_compact(display_state, remote, local);

		ఒకవేళ (error)
			strbuf_addf(&display_state->buf, "  (%s)", error);

		ఆపు;
	}
	స్థితి DISPLAY_FORMAT_PORCELAIN:
		strbuf_addf(&display_state->buf, "%c %s %s %s", సంకేతం_2,
			    oid_గమ్యం_hex(old_oid), oid_గమ్యం_hex(new_oid), local);
		f = stdout;
		ఆపు;
	అప్రమేయ:
		BUG("unexpected display format %d", display_state->format);
	};
	strbuf_addch(&display_state->buf, '\n');

	fputs(display_state->buf.buf, f);
}

నిర్మాణం ref_update_display_info {
	bool failed;
	అక్షరం success_సంకేతం;
	అక్షరం fail_సంకేతం;
	అక్షరం *summary;
	అక్షరం *fail_detail;
	అక్షరం *success_detail;
	అక్షరం *ref;
	అక్షరం *remote;
	నిర్మాణం object_id old_oid;
	నిర్మాణం object_id new_oid;
};

నిర్మాణం ref_update_display_info_array {
	నిర్మాణం ref_update_display_info *info;
	కొలత_t alloc, nr;
};

స్థిర నిర్మాణం ref_update_display_info *ref_update_display_info_append(
					   నిర్మాణం ref_update_display_info_array *array,
					   అక్షరం success_సంకేతం,
					   అక్షరం fail_సంకేతం,
					   స్థిరరాశి అక్షరం *summary,
					   స్థిరరాశి అక్షరం *success_detail,
					   స్థిరరాశి అక్షరం *fail_detail,
					   స్థిరరాశి అక్షరం *ref,
					   స్థిరరాశి అక్షరం *remote,
					   స్థిరరాశి నిర్మాణం object_id *old_oid,
					   స్థిరరాశి నిర్మాణం object_id *new_oid)
{
	నిర్మాణం ref_update_display_info *info;

	ALLOC_GROW(array->info, array->nr + 1, array->alloc);
	info = &array->info[array->nr++];

	info->failed = false;
	info->success_సంకేతం = success_సంకేతం;
	info->fail_సంకేతం = fail_సంకేతం;
	info->summary = xstrdup(summary);
	info->success_detail = xstrdup_or_null(success_detail);
	info->fail_detail = xstrdup_or_null(fail_detail);
	info->remote = xstrdup(remote);
	info->ref = xstrdup(ref);

	oidcpy(&info->old_oid, old_oid);
	oidcpy(&info->new_oid, new_oid);

	తిరిగి info;
}

స్థిర శూన్య ref_update_display_info_సమితి_failed(నిర్మాణం ref_update_display_info *info)
{
	info->failed = true;
}

స్థిర శూన్య ref_update_display_info_free(నిర్మాణం ref_update_display_info *info)
{
	విడుదల(info->summary);
	విడుదల(info->success_detail);
	విడుదల(info->fail_detail);
	విడుదల(info->remote);
	విడుదల(info->ref);
}

స్థిర శూన్య ref_update_display_info_display(నిర్మాణం ref_update_display_info *info,
					    నిర్మాణం display_state *display_state,
					    పూర్ణాంకం summary_వెడల్పు)
{
	display_ref_update(display_state,
			   info->failed ? info->fail_సంకేతం : info->success_సంకేతం,
			   info->summary,
			   info->failed ? info->fail_detail : info->success_detail,
			   info->remote, info->ref, &info->old_oid,
			   &info->new_oid, summary_వెడల్పు);
}

స్థిర పూర్ణాంకం update_local_ref(నిర్మాణం ref *ref,
			    నిర్మాణం ref_transaction *transaction,
			    స్థిరరాశి నిర్మాణం ref *remote_ref,
			    స్థిరరాశి నిర్మాణం fetch_config *config,
			    నిర్మాణం ref_update_display_info_array *display_array)
{
	నిర్మాణం commit *current = ఖాళీ, *updated;
	పూర్ణాంకం fast_forward = 0;

	ఒకవేళ (!odb_has_object(the_repository->objects, &ref->new_oid,
			    ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR))
		die(_("object %s not found"), oid_గమ్యం_hex(&ref->new_oid));

	ఒకవేళ (oideq(&ref->old_oid, &ref->new_oid)) {
		ఒకవేళ (verbosity > 0)
			ref_update_display_info_append(display_array, '=', '=',
						       _("[up to date]"), ఖాళీ,
						       ఖాళీ, ref->name,
						       remote_ref->name, &ref->old_oid,
						       &ref->new_oid);
		తిరిగి 0;
	}

	ఒకవేళ (!update_తల_ok &&
	    !is_null_oid(&ref->old_oid) &&
	    branch_checked_బహిర్గతం(ref->name)) {
		నిర్మాణం ref_update_display_info *info;
		/*
		 * If this is the head, and it's not okay to update
		 * the head, and the old value of the head isn't empty...
		 */
		info = ref_update_display_info_append(display_array, '!', '!',
						      _("[rejected]"), ఖాళీ,
						      _("can't fetch into checked-out branch"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);
		ref_update_display_info_సమితి_failed(info);
		తిరిగి 1;
	}

	ఒకవేళ (!is_null_oid(&ref->old_oid) &&
	    starts_with(ref->name, "refs/tags/")) {
		నిర్మాణం ref_update_display_info *info;

		ఒకవేళ (force || ref->force) {
			పూర్ణాంకం r;

			r = s_update_ref("updating tag", ref, transaction, 0);

			info = ref_update_display_info_append(display_array, 't', '!',
							      _("[tag update]"), ఖాళీ,
							      _("unable to update local ref"),
							      ref->name, remote_ref->name,
							      &ref->old_oid, &ref->new_oid);
			ఒకవేళ (r)
				ref_update_display_info_సమితి_failed(info);

			తిరిగి r;
		} లేకపోతే {
			info = ref_update_display_info_append(display_array, '!', '!',
							      _("[rejected]"), ఖాళీ,
							      _("would clobber existing tag"),
							      ref->name, remote_ref->name,
							      &ref->old_oid, &ref->new_oid);
			ref_update_display_info_సమితి_failed(info);
			తిరిగి 1;
		}
	}

	current = lookup_commit_reference_gently(the_repository,
						 &ref->old_oid, 1);
	updated = lookup_commit_reference_gently(the_repository,
						 &ref->new_oid, 1);
	ఒకవేళ (!current || !updated) {
		నిర్మాణం ref_update_display_info *info;
		స్థిరరాశి అక్షరం *సందేశం;
		స్థిరరాశి అక్షరం *what;
		పూర్ణాంకం r;
		/*
		 * Nicely describe the new ref we're fetching.
		 * Base this on the remote's ref name, as it's
		 * more likely to follow a standard layout.
		 */
		ఒకవేళ (starts_with(remote_ref->name, "refs/tags/")) {
			సందేశం = "storing tag";
			what = _("[new tag]");
		} లేకపోతే ఒకవేళ (starts_with(remote_ref->name, "refs/heads/")) {
			సందేశం = "storing head";
			what = _("[new branch]");
		} లేకపోతే {
			సందేశం = "storing ref";
			what = _("[new ref]");
		}

		r = s_update_ref(సందేశం, ref, transaction, 0);

		info = ref_update_display_info_append(display_array, '*', '!',
						      what, ఖాళీ,
						      _("unable to update local ref"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);
		ఒకవేళ (r)
			ref_update_display_info_సమితి_failed(info);

		తిరిగి r;
	}

	ఒకవేళ (config->show_forced_updates) {
		uint64_t t_before = getnanotime();
		fast_forward = repo_లోపల_విలీనం_bases(the_repository, current,
						   updated);
		ఒకవేళ (fast_forward < 0)
			die(ఖాళీ);
		forced_updates_ms += (getnanotime() - t_before) / 1000000;
	} లేకపోతే {
		fast_forward = 1;
	}

	ఒకవేళ (fast_forward) {
		నిర్మాణం ref_update_display_info *info;
		నిర్మాణం strbuf quickref = STRBUF_INIT;
		పూర్ణాంకం r;

		strbuf_కూడు_unique_abbrev(&quickref, &current->object.oid, DEFAULT_ABBREV);
		strbuf_addstr(&quickref, "..");
		strbuf_కూడు_unique_abbrev(&quickref, &ref->new_oid, DEFAULT_ABBREV);
		r = s_update_ref("fast-forward", ref, transaction, 1);

		info = ref_update_display_info_append(display_array, ' ', '!',
						      quickref.buf, ఖాళీ,
						      _("unable to update local ref"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);
		ఒకవేళ (r)
			ref_update_display_info_సమితి_failed(info);

		strbuf_release(&quickref);
		తిరిగి r;
	} లేకపోతే ఒకవేళ (force || ref->force) {
		నిర్మాణం ref_update_display_info *info;
		నిర్మాణం strbuf quickref = STRBUF_INIT;
		పూర్ణాంకం r;

		strbuf_కూడు_unique_abbrev(&quickref, &current->object.oid, DEFAULT_ABBREV);
		strbuf_addstr(&quickref, "...");
		strbuf_కూడు_unique_abbrev(&quickref, &ref->new_oid, DEFAULT_ABBREV);
		r = s_update_ref("forced-update", ref, transaction, 1);

		info = ref_update_display_info_append(display_array, '+', '!',
						      quickref.buf, _("forced update"),
						      _("unable to update local ref"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);

		ఒకవేళ (r)
			ref_update_display_info_సమితి_failed(info);

		strbuf_release(&quickref);
		తిరిగి r;
	} లేకపోతే {
		నిర్మాణం ref_update_display_info *info;
		info = ref_update_display_info_append(display_array, '!', '!',
						      _("[rejected]"), ఖాళీ,
						      _("non-fast-forward"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);
		ref_update_display_info_సమితి_failed(info);
		తిరిగి 1;
	}
}

స్థిర స్థిరరాశి నిర్మాణం object_id *iterate_ref_map(శూన్య *cb_దత్తాంశం)
{
	నిర్మాణం ref **rm = cb_దత్తాంశం;
	నిర్మాణం ref *ref = *rm;

	అయితే (ref && ref->స్థితి_2 == REF_స్థితి_REJECT_SHALLOW)
		ref = ref->తదుపరి;
	ఒకవేళ (!ref)
		తిరిగి ఖాళీ;
	*rm = ref->తదుపరి;
	తిరిగి &ref->old_oid;
}

నిర్మాణం fetch_తల {
	FILE *fp;
	నిర్మాణం strbuf buf;
};

స్థిర పూర్ణాంకం open_fetch_తల(నిర్మాణం fetch_తల *fetch_తల)
{
	స్థిరరాశి అక్షరం *filename = git_మార్గం_fetch_తల(the_repository);

	ఒకవేళ (write_fetch_తల) {
		fetch_తల->fp = fopen(filename, "a");
		ఒకవేళ (!fetch_తల->fp)
			తిరిగి error_errno(_("cannot open '%s'"), filename);
		strbuf_init(&fetch_తల->buf, 0);
	} లేకపోతే {
		fetch_తల->fp = ఖాళీ;
	}

	తిరిగి 0;
}

స్థిర శూన్య append_fetch_తల(నిర్మాణం fetch_తల *fetch_తల,
			      స్థిరరాశి నిర్మాణం object_id *old_oid,
			      గణన fetch_తల_స్థితి fetch_తల_స్థితి,
			      స్థిరరాశి అక్షరం *note,
			      స్థిరరాశి అక్షరం *url, కొలత_t url_len)
{
	అక్షరం old_oid_hex[GIT_MAX_HEXSZ + 1];
	స్థిరరాశి అక్షరం *విలీనం_స్థితి_marker;
	కొలత_t i;

	ఒకవేళ (!fetch_తల->fp)
		తిరిగి;

	ఎంపిక (fetch_తల_స్థితి) {
	స్థితి FETCH_తల_NOT_FOR_విలీనం:
		విలీనం_స్థితి_marker = "not-for-merge";
		ఆపు;
	స్థితి FETCH_తల_విలీనం:
		విలీనం_స్థితి_marker = "";
		ఆపు;
	అప్రమేయ:
		/* do not write anything to FETCH_HEAD */
		తిరిగి;
	}

	strbuf_addf(&fetch_తల->buf, "%s\t%s\t%s",
		    oid_గమ్యం_hex_r(old_oid_hex, old_oid), విలీనం_స్థితి_marker, note);
	కొరకు (i = 0; i < url_len; ++i)
		ఒకవేళ ('\n' == url[i])
			strbuf_addstr(&fetch_తల->buf, "\\n");
		లేకపోతే
			strbuf_addch(&fetch_తల->buf, url[i]);
	strbuf_addch(&fetch_తల->buf, '\n');

	/*
	 * When using an atomic fetch, we do not want to update FETCH_HEAD if
	 * any of the reference updates fails. We thus have to write all
	 * updates to a buffer first and only commit it as soon as all
	 * references have been successfully updated.
	 */
	ఒకవేళ (!atomic_fetch) {
		strbuf_write(&fetch_తల->buf, fetch_తల->fp);
		strbuf_reset(&fetch_తల->buf);
	}
}

స్థిర శూన్య commit_fetch_తల(నిర్మాణం fetch_తల *fetch_తల)
{
	ఒకవేళ (!fetch_తల->fp || !atomic_fetch)
		తిరిగి;
	strbuf_write(&fetch_తల->buf, fetch_తల->fp);
}

స్థిర శూన్య close_fetch_తల(నిర్మాణం fetch_తల *fetch_తల)
{
	ఒకవేళ (!fetch_తల->fp)
		తిరిగి;

	fclose(fetch_తల->fp);
	strbuf_release(&fetch_తల->buf);
}

స్థిర స్థిరరాశి అక్షరం warn_show_forced_updates[] =
N_("fetch normally indicates which branches had a forced update,\n"
   "but that check has been disabled; to re-enable, use '--show-forced-updates'\n"
   "flag or run 'git config fetch.showForcedUpdates true'");
స్థిర స్థిరరాశి అక్షరం warn_సమయం_show_forced_updates[] =
N_("it took %.2f seconds to check forced updates; you can use\n"
   "'--no-show-forced-updates' or run 'git config fetch.showForcedUpdates false'\n"
   "to avoid this check\n");

స్థిర పూర్ణాంకం store_updated_refs(నిర్మాణం display_state *display_state,
			      పూర్ణాంకం connectivity_checked,
			      నిర్మాణం ref_transaction *transaction, నిర్మాణం ref *ref_map,
			      నిర్మాణం fetch_తల *fetch_తల,
			      స్థిరరాశి నిర్మాణం fetch_config *config,
			      నిర్మాణం ref_update_display_info_array *display_array)
{
	పూర్ణాంకం rc = 0;
	నిర్మాణం strbuf note = STRBUF_INIT;
	స్థిరరాశి అక్షరం *what, *రకం;
	నిర్మాణం ref *rm;
	పూర్ణాంకం want_స్థితి;

	ఒకవేళ (!connectivity_checked) {
		నిర్మాణం check_connected_options opt = CHECK_CONNECTED_INIT;

		opt.exclude_hidden_refs_section = "fetch";
		rm = ref_map;
		ఒకవేళ (check_connected(iterate_ref_map, &rm, &opt)) {
			rc = error(_("%s did not send all necessary objects"),
				   display_state->url);
			వెళ్ళు abort;
		}
	}

	/*
	 * We do a pass for each fetch_head_status type in their enum order, so
	 * merged entries are written before not-for-merge. That lets readers
	 * use FETCH_HEAD as a refname to refer to the ref to be merged.
	 */
	కొరకు (want_స్థితి = FETCH_తల_విలీనం;
	     want_స్థితి <= FETCH_తల_IGNORE;
	     want_స్థితి++) {
		కొరకు (rm = ref_map; rm; rm = rm->తదుపరి) {
			నిర్మాణం ref *ref = ఖాళీ;

			ఒకవేళ (rm->స్థితి_2 == REF_స్థితి_REJECT_SHALLOW) {
				ఒకవేళ (want_స్థితి == FETCH_తల_విలీనం)
					warning(_("rejected %s because shallow roots are not allowed to be updated"),
						rm->peer_ref ? rm->peer_ref->name : rm->name);
				కొనసాగు;
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
			ఒకవేళ (fetch_తల->fp) {
				నిర్మాణం commit *commit = ఖాళీ;

				/*
				 * References in "refs/tags/" are often going to point
				 * to annotated tags, which are not part of the
				 * commit-graph. We thus only try to look up refs in
				 * the graph which are not in that namespace to not
				 * regress performance in repositories with many
				 * annotated tags.
				 */
				ఒకవేళ (!starts_with(rm->name, "refs/tags/"))
					commit = lookup_commit_లోపల_లేఖాచిత్రం(the_repository, &rm->old_oid);
				ఒకవేళ (!commit) {
					commit = lookup_commit_reference_gently(the_repository,
										&rm->old_oid,
										1);
					ఒకవేళ (!commit)
						rm->fetch_తల_స్థితి = FETCH_తల_NOT_FOR_విలీనం;
				}
			}

			ఒకవేళ (rm->fetch_తల_స్థితి != want_స్థితి)
				కొనసాగు;

			ఒకవేళ (rm->peer_ref) {
				ref = alloc_ref(rm->peer_ref->name);
				oidcpy(&ref->old_oid, &rm->peer_ref->old_oid);
				oidcpy(&ref->new_oid, &rm->old_oid);
				ref->force = rm->peer_ref->force;
			}

			ఒకవేళ (config->recurse_submodules != RECURSE_SUBMODULES_OFF &&
			    (!rm->peer_ref || !oideq(&ref->old_oid, &ref->new_oid))) {
				check_for_new_submodule_commits(&rm->old_oid);
			}

			ఒకవేళ (!strcmp(rm->name, "HEAD")) {
				రకం = "";
				what = "";
			} లేకపోతే ఒకవేళ (skip_ఉపసర్గ(rm->name, "refs/heads/", &what)) {
				రకం = "branch";
			} లేకపోతే ఒకవేళ (skip_ఉపసర్గ(rm->name, "refs/tags/", &what)) {
				రకం = "tag";
			} లేకపోతే ఒకవేళ (skip_ఉపసర్గ(rm->name, "refs/remotes/", &what)) {
				రకం = "remote-tracking branch";
			} లేకపోతే {
				రకం = "";
				what = rm->name;
			}

			strbuf_reset(&note);
			ఒకవేళ (*what) {
				ఒకవేళ (*రకం)
					strbuf_addf(&note, "%s ", రకం);
				strbuf_addf(&note, "'%s' of ", what);
			}

			append_fetch_తల(fetch_తల, &rm->old_oid,
					  rm->fetch_తల_స్థితి,
					  note.buf, display_state->url,
					  display_state->url_len);

			ఒకవేళ (ref) {
				rc |= update_local_ref(ref, transaction, rm,
						       config, display_array);
				విడుదల(ref);
			} లేకపోతే ఒకవేళ (write_fetch_తల || dry_నిరంతరం) {
				/*
				 * Display fetches written to FETCH_HEAD (or
				 * would be written to FETCH_HEAD, if --dry-run
				 * is set).
				 */

				ref_update_display_info_append(display_array, '*', '*',
							       *రకం ? రకం : "branch",
							       ఖాళీ, ఖాళీ, "FETCH_HEAD",
							       rm->name, &rm->new_oid,
							       &rm->old_oid);
			}
		}
	}

	ఒకవేళ (advice_enabled(ADVICE_FETCH_SHOW_FORCED_UPDATES)) {
		ఒకవేళ (!config->show_forced_updates) {
			warning(_(warn_show_forced_updates));
		} లేకపోతే ఒకవేళ (forced_updates_ms > FORCED_UPDATES_DELAY_WARNING_లోపల_MS) {
			warning(_(warn_సమయం_show_forced_updates),
				forced_updates_ms / 1000.0);
		}
	}

 abort:
	strbuf_release(&note);
	తిరిగి rc;
}

/*
 * We would want to bypass the object transfer altogether if
 * everything we are going to fetch already exists and is connected
 * locally.
 */
స్థిర పూర్ణాంకం check_exist_and_connected(నిర్మాణం ref *ref_map)
{
	నిర్మాణం ref *rm = ref_map;
	నిర్మాణం check_connected_options opt = CHECK_CONNECTED_INIT;
	నిర్మాణం ref *r;

	/*
	 * If we are deepening a shallow clone we already have these
	 * objects reachable.  Running rev-list here will return with
	 * a good (0) exit status and we'll bypass the fetch that we
	 * really need to perform.  Claiming failure now will ensure
	 * we perform the network exchange to deepen our history.
	 */
	ఒకవేళ (deepen)
		తిరిగి -1;

	/*
	 * Similarly, if we need to refetch, we always want to perform a full
	 * fetch ignoring existing objects.
	 */
	ఒకవేళ (refetch)
		తిరిగి -1;


	/*
	 * check_connected() allows objects to merely be promised, but
	 * we need all direct targets to exist.
	 */
	కొరకు (r = rm; r; r = r->తదుపరి) {
		ఒకవేళ (!odb_has_object(the_repository->objects, &r->old_oid,
				    ODB_HAS_OBJECT_RECHECK_PACKED))
			తిరిగి -1;
	}

	opt.quiet = 1;
	opt.exclude_hidden_refs_section = "fetch";
	తిరిగి check_connected(iterate_ref_map, &rm, &opt);
}

స్థిర పూర్ణాంకం fetch_and_consume_refs(నిర్మాణం display_state *display_state,
				  నిర్మాణం transport *transport,
				  నిర్మాణం ref_transaction *transaction,
				  నిర్మాణం ref *ref_map,
				  నిర్మాణం fetch_తల *fetch_తల,
				  స్థిరరాశి నిర్మాణం fetch_config *config,
				  నిర్మాణం ref_update_display_info_array *display_array)
{
	పూర్ణాంకం connectivity_checked = 1;
	పూర్ణాంకం ret;

	/*
	 * We don't need to perform a fetch in case we can already satisfy all
	 * refs.
	 */
	ret = check_exist_and_connected(ref_map);
	ఒకవేళ (ret) {
		trace2_region_enter("fetch", "fetch_refs", the_repository);
		ret = transport_fetch_refs(transport, ref_map);
		trace2_region_leave("fetch", "fetch_refs", the_repository);
		ఒకవేళ (ret)
			వెళ్ళు బహిర్గతం;
		connectivity_checked = transport->smart_options ?
			transport->smart_options->connectivity_checked : 0;
	}

	trace2_region_enter("fetch", "consume_refs", the_repository);
	ret = store_updated_refs(display_state, connectivity_checked,
				 transaction, ref_map, fetch_తల, config,
				 display_array);
	trace2_region_leave("fetch", "consume_refs", the_repository);

బహిర్గతం:
	transport_unlock_pack(transport, 0);
	తిరిగి ret;
}

స్థిర పూర్ణాంకం prune_refs(నిర్మాణం display_state *display_state,
		      నిర్మాణం refspec *rs,
		      నిర్మాణం ref_transaction *transaction,
		      నిర్మాణం ref *ref_map)
{
	పూర్ణాంకం ఫలితం = 0;
	నిర్మాణం ref *ref, *stale_refs = get_stale_heads(rs, ref_map);
	నిర్మాణం strbuf err = STRBUF_INIT;
	నిర్మాణం string_list refnames = STRING_LIST_INIT_NODUP;

	కొరకు (ref = stale_refs; ref; ref = ref->తదుపరి)
		string_list_append(&refnames, ref->name);

	ఒకవేళ (!dry_నిరంతరం) {
		ఒకవేళ (transaction) {
			కొరకు (ref = stale_refs; ref; ref = ref->తదుపరి) {
				ఫలితం = ref_transaction_delete(transaction, ref->name, ఖాళీ,
								ఖాళీ, 0, "fetch: prune", &err);
				ఒకవేళ (ఫలితం)
					వెళ్ళు cleanup;
			}
		} లేకపోతే {
			ఫలితం = refs_delete_refs(get_main_ref_store(the_repository),
						  "fetch: prune", &refnames,
						  0);
		}
	}

	ఒకవేళ (verbosity >= 0) {
		పూర్ణాంకం summary_వెడల్పు = transport_summary_వెడల్పు(stale_refs);

		కొరకు (ref = stale_refs; ref; ref = ref->తదుపరి) {
			display_ref_update(display_state, '-', _("[deleted]"), ఖాళీ,
					   _("(none)"), ref->name,
					   &ref->new_oid, &ref->old_oid,
					   summary_వెడల్పు);
		}
		string_list_క్రమీకరించు(&refnames);
		refs_warn_dangling_symrefs(get_main_ref_store(the_repository),
					   stderr, "   ", dry_నిరంతరం, &refnames);
	}

cleanup:
	string_list_clear(&refnames, 0);
	strbuf_release(&err);
	free_refs(stale_refs);
	తిరిగి ఫలితం;
}

స్థిర శూన్య check_not_current_branch(నిర్మాణం ref *ref_map)
{
	స్థిరరాశి అక్షరం *మార్గం;
	కొరకు (; ref_map; ref_map = ref_map->తదుపరి)
		ఒకవేళ (ref_map->peer_ref &&
		    starts_with(ref_map->peer_ref->name, "refs/heads/") &&
		    (మార్గం = branch_checked_బహిర్గతం(ref_map->peer_ref->name)))
			die(_("refusing to fetch into branch '%s' "
			      "checked out at '%s'"),
			    ref_map->peer_ref->name, మార్గం);
}

స్థిర పూర్ణాంకం truncate_fetch_తల(శూన్య)
{
	స్థిరరాశి అక్షరం *filename = git_మార్గం_fetch_తల(the_repository);
	FILE *fp = fopen_for_writing(filename);

	ఒకవేళ (!fp)
		తిరిగి error_errno(_("cannot open '%s'"), filename);
	fclose(fp);
	తిరిగి 0;
}

స్థిర శూన్య సమితి_option(నిర్మాణం transport *transport, స్థిరరాశి అక్షరం *name, స్థిరరాశి అక్షరం *విలువ)
{
	పూర్ణాంకం r = transport_సమితి_option(transport, name, విలువ);
	ఒకవేళ (r < 0)
		die(_("option \"%s\" value \"%s\" is not valid for %s"),
		    name, విలువ, transport->url);
	ఒకవేళ (r > 0)
		warning(_("option \"%s\" is ignored for %s"),
			name, transport->url);
}


స్థిర పూర్ణాంకం కూడు_oid(స్థిరరాశి నిర్మాణం reference *ref, శూన్య *cb_దత్తాంశం)
{
	నిర్మాణం oid_array *oids = cb_దత్తాంశం;

	oid_array_append(oids, ref->oid);
	తిరిగి 0;
}

స్థిర శూన్య కూడు_negotiation_tips(నిర్మాణం string_list *ఆదానం_list,
				 నిర్మాణం oid_array **output_list,
				 స్థిరరాశి అక్షరం *argname)
{
	నిర్మాణం oid_array *oids = xcalloc(1, పరిమాణం(*oids));
	పూర్ణాంకం i;

	కొరకు (i = 0; i < ఆదానం_list->nr; i++) {
		స్థిరరాశి అక్షరం *s = ఆదానం_list->వస్తువులు[i].string;
		నిర్మాణం refs_for_each_ref_options opts = {
			.నమూనా = s,
		};
		పూర్ణాంకం old_nr;
		ఒకవేళ (!has_glob_specials(s)) {
			నిర్మాణం object_id oid;

			/* Ignore missing reference. */
			ఒకవేళ (repo_get_oid(the_repository, s, &oid))
				కొనసాగు;
			/* Fail on missing object pointed by ref. */
			ఒకవేళ (!odb_has_object(the_repository->objects, &oid, 0))
				die(_("the object %s does not exist"), s);

			oid_array_append(oids, &oid);
			కొనసాగు;
		}
		old_nr = oids->nr;
		refs_for_each_ref_ext(get_main_ref_store(the_repository),
				      కూడు_oid, oids, &opts);
		ఒకవేళ (old_nr == oids->nr)
			warning(_("ignoring %s=%s because it does not match any refs"),
				argname, s);
	}
	*output_list = oids;
}

స్థిర నిర్మాణం transport *prepare_transport(నిర్మాణం remote *remote, పూర్ణాంకం deepen,
					   నిర్మాణం list_objects_filter_options *filter_options)
{
	నిర్మాణం transport *transport;

	transport = transport_get(remote, ఖాళీ);
	transport_సమితి_verbosity(transport, verbosity, పురోగతి);
	transport->family = family;
	ఒకవేళ (upload_pack)
		సమితి_option(transport, TRANS_OPT_UPLOADPACK, upload_pack);
	ఒకవేళ (keep)
		సమితి_option(transport, TRANS_OPT_KEEP, "yes");
	ఒకవేళ (లోతు)
		సమితి_option(transport, TRANS_OPT_లోతు, లోతు);
	ఒకవేళ (deepen && deepen_since)
		సమితి_option(transport, TRANS_OPT_DEEPEN_SINCE, deepen_since);
	ఒకవేళ (deepen && deepen_not.nr)
		సమితి_option(transport, TRANS_OPT_DEEPEN_NOT,
			   (స్థిరరాశి అక్షరం *)&deepen_not);
	ఒకవేళ (deepen_relative)
		సమితి_option(transport, TRANS_OPT_DEEPEN_RELATIVE, "yes");
	ఒకవేళ (update_shallow)
		సమితి_option(transport, TRANS_OPT_UPDATE_SHALLOW, "yes");
	ఒకవేళ (refetch)
		సమితి_option(transport, TRANS_OPT_REFETCH, "yes");
	ఒకవేళ (filter_options->choice) {
		స్థిరరాశి అక్షరం *spec =
			expand_list_objects_filter_spec(filter_options);
		సమితి_option(transport, TRANS_OPT_LIST_OBJECTS_FILTER, spec);
		సమితి_option(transport, TRANS_OPT_FROM_PROMISOR, "1");
	}
	ఒకవేళ (negotiation_restrict.nr) {
		ఒకవేళ (transport->smart_options)
			కూడు_negotiation_tips(&negotiation_restrict,
					     &transport->smart_options->negotiation_restrict_tips,
					     "--negotiation-restrict");
		లేకపోతే
			warning(_("ignoring %s because the protocol does not support it"),
				"--negotiation-restrict");
	} లేకపోతే ఒకవేళ (remote->negotiation_restrict.nr) {
		నిర్మాణం string_list_అంశం *అంశం;
		for_each_string_list_అంశం(అంశం, &remote->negotiation_restrict)
			string_list_append(&negotiation_restrict, అంశం->string);
		ఒకవేళ (transport->smart_options)
			కూడు_negotiation_tips(&negotiation_restrict,
					     &transport->smart_options->negotiation_restrict_tips,
					     "--negotiation-restrict");
		లేకపోతే {
			నిర్మాణం strbuf config_name = STRBUF_INIT;
			strbuf_addf(&config_name, "remote.%s.negotiationRestrict", remote->name);
			warning(_("ignoring %s because the protocol does not support it"),
				config_name.buf);
			strbuf_release(&config_name);
		}
	}
	ఒకవేళ (negotiation_include.nr) {
		ఒకవేళ (transport->smart_options)
			కూడు_negotiation_tips(&negotiation_include,
					     &transport->smart_options->negotiation_include_tips,
					     "--negotiation-include");
		లేకపోతే
			warning(_("ignoring %s because the protocol does not support it"),
				"--negotiation-include");
	} లేకపోతే ఒకవేళ (remote->negotiation_include.nr) {
		ఒకవేళ (transport->smart_options) {
			కూడు_negotiation_tips(&remote->negotiation_include,
					     &transport->smart_options->negotiation_include_tips,
					     "--negotiation-include");
		} లేకపోతే {
			నిర్మాణం strbuf config_name = STRBUF_INIT;
			strbuf_addf(&config_name, "remote.%s.negotiationInclude", remote->name);
			warning(_("ignoring %s because the protocol does not support it"),
				config_name.buf);
			strbuf_release(&config_name);
		}
	}
	తిరిగి transport;
}

స్థిర పూర్ణాంకం backfill_ట్యాగులు(నిర్మాణం display_state *display_state,
			 నిర్మాణం transport *transport,
			 నిర్మాణం ref_transaction *transaction,
			 నిర్మాణం ref *ref_map,
			 నిర్మాణం fetch_తల *fetch_తల,
			 స్థిరరాశి నిర్మాణం fetch_config *config,
			 నిర్మాణం ref_update_display_info_array *display_array,
			 నిర్మాణం list_objects_filter_options *filter_options)
{
	పూర్ణాంకం retcode, cannot_reuse;

	/*
	 * Once we have set TRANS_OPT_DEEPEN_SINCE, we can't unset it
	 * when remote helper is used (setting it to an empty string
	 * is not unsetting). We could extend the remote helper
	 * protocol for that, but for now, just force a new connection
	 * without deepen-since. Similar story for deepen-not.
	 */
	cannot_reuse = transport->cannot_reuse ||
		deepen_since || deepen_not.nr;
	ఒకవేళ (cannot_reuse) {
		gsecondary = prepare_transport(transport->remote, 0, filter_options);
		transport = gsecondary;
	}

	transport_సమితి_option(transport, TRANS_OPT_FOLLOWTAGS, ఖాళీ);
	transport_సమితి_option(transport, TRANS_OPT_లోతు, "0");
	transport_సమితి_option(transport, TRANS_OPT_DEEPEN_RELATIVE, ఖాళీ);
	retcode = fetch_and_consume_refs(display_state, transport, transaction, ref_map,
					 fetch_తల, config, display_array);

	ఒకవేళ (gsecondary) {
		transport_disconnect(gsecondary);
		gsecondary = ఖాళీ;
	}

	తిరిగి retcode;
}

స్థిర స్థిరరాశి అక్షరం *strip_refshead(స్థిరరాశి అక్షరం *name){
	skip_ఉపసర్గ(name, "refs/heads/", &name);
	తిరిగి name;
}

స్థిర శూన్య సమితి_తల_advice_సందేశం(స్థిరరాశి అక్షరం *remote, స్థిరరాశి అక్షరం *తల_name)
{
	స్థిరరాశి అక్షరం సందేశం_advice_సమితి_తల[] =
	N_("Run 'git remote set-head %s %s' to follow the change, or modify\n"
	   "either of the 'remote.%s.followRemoteHEAD' or 'fetch.followRemoteHEAD'\n"
	   "configuration variables to handle the situation differently.\n\n"

	   "Using this specific setting\n\n"
	   "    git config set remote.%s.followRemoteHEAD warn-if-not-%s\n\n"
	   "will suppress the warning until the remote changes HEAD to something else.");

	advise_if_enabled(ADVICE_FETCH_సమితి_తల_WARN, _(సందేశం_advice_సమితి_తల),
			remote, తల_name, remote, remote, తల_name);
}

స్థిర శూన్య warn_సమితి_తల(స్థిరరాశి అక్షరం *remote, స్థిరరాశి అక్షరం *తల_name,
			నిర్మాణం strbuf *buf_prev, పూర్ణాంకం updateres) {
	నిర్మాణం strbuf buf_ఉపసర్గ = STRBUF_INIT;
	స్థిరరాశి అక్షరం *prev_తల = ఖాళీ;

	strbuf_addf(&buf_ఉపసర్గ, "refs/remotes/%s/", remote);
	skip_ఉపసర్గ(buf_prev->buf, buf_ఉపసర్గ.buf, &prev_తల);

	ఒకవేళ (prev_తల && strcmp(prev_తల, తల_name)) {
		రాయి("'HEAD' at '%s' is '%s', but we have '%s' locally.\n",
			remote, తల_name, prev_తల);
		సమితి_తల_advice_సందేశం(remote, తల_name);
	}
	లేకపోతే ఒకవేళ (updateres && buf_prev->len) {
		రాయి("'HEAD' at '%s' is '%s', "
			"but we have a detached HEAD pointing to '%s' locally.\n",
			remote, తల_name, buf_prev->buf);
		సమితి_తల_advice_సందేశం(remote, తల_name);
	}
	strbuf_release(&buf_ఉపసర్గ);
}

స్థిర పూర్ణాంకం సమితి_తల(స్థిరరాశి నిర్మాణం ref *remote_refs, నిర్మాణం remote *remote,
			పూర్ణాంకం follow_remote_తల)
{
	పూర్ణాంకం ఫలితం = 0, create_only, baremirror, was_detached;
	నిర్మాణం strbuf b_తల = STRBUF_INIT, b_remote_తల = STRBUF_INIT,
		      b_local_తల = STRBUF_INIT;
	స్థిరరాశి అక్షరం *no_warn_branch = remote->no_warn_branch;
	అక్షరం *తల_name = ఖాళీ;
	నిర్మాణం ref *ref, *matches;
	నిర్మాణం ref *fetch_map = ఖాళీ, **fetch_map_తోక = &fetch_map;
	నిర్మాణం refspec_అంశం refspec = {
		.force = 0,
		.నమూనా = 1,
		.src = (అక్షరం *) "refs/heads/*",
		.dst = (అక్షరం *) "refs/heads/*",
	};
	నిర్మాణం string_list heads = STRING_LIST_INIT_DUP;
	నిర్మాణం ref_store *refs = get_main_ref_store(the_repository);

	get_fetch_map(remote_refs, &refspec, &fetch_map_తోక, 0);
	matches = guess_remote_తల(కనుగొను_ref_by_name(remote_refs, "HEAD"),
				    fetch_map, REMOTE_GUESS_తల_ALL);
	కొరకు (ref = matches; ref; ref = ref->తదుపరి) {
		string_list_append(&heads, strip_refshead(ref->name));
	}

	ఒకవేళ (!heads.nr)
		ఫలితం = 1;
	లేకపోతే ఒకవేళ (heads.nr > 1)
		ఫలితం = 1;
	లేకపోతే
		తల_name = xstrdup(heads.వస్తువులు[0].string);

	ఒకవేళ (!తల_name)
		వెళ్ళు cleanup;
	baremirror = is_bare_repository(the_repository) && remote->mirror;
	create_only = follow_remote_తల == FOLLOW_REMOTE_ALWAYS ? 0 : !baremirror;
	ఒకవేళ (baremirror) {
		strbuf_addstr(&b_తల, "HEAD");
		strbuf_addf(&b_remote_తల, "refs/heads/%s", తల_name);
	} లేకపోతే {
		strbuf_addf(&b_తల, "refs/remotes/%s/HEAD", remote->name);
		strbuf_addf(&b_remote_తల, "refs/remotes/%s/%s", remote->name, తల_name);
	}
	/* make sure it's valid */
	ఒకవేళ (!baremirror && !refs_ref_exists(refs, b_remote_తల.buf)) {
		ఫలితం = 1;
		వెళ్ళు cleanup;
	}
	was_detached = refs_update_symref_extended(refs, b_తల.buf, b_remote_తల.buf,
					"fetch", &b_local_తల, create_only);
	ఒకవేళ (was_detached == -1) {
		ఫలితం = 1;
		వెళ్ళు cleanup;
	}
	ఒకవేళ (verbosity >= 0 &&
		follow_remote_తల == FOLLOW_REMOTE_WARN &&
		(!no_warn_branch || strcmp(no_warn_branch, తల_name)))
		warn_సమితి_తల(remote->name, తల_name, &b_local_తల, was_detached);

cleanup:
	విడుదల(తల_name);
	free_refs(fetch_map);
	free_refs(matches);
	string_list_clear(&heads, 0);
	strbuf_release(&b_తల);
	strbuf_release(&b_local_తల);
	strbuf_release(&b_remote_తల);
	తిరిగి ఫలితం;
}

నిర్మాణం ref_rejection_దత్తాంశం {
	పూర్ణాంకం *retcode;
	bool conflict_సందేశం_shown;
	bool case_sensitive_సందేశం_shown;
	స్థిరరాశి అక్షరం *remote_name;
	నిర్మాణం strmap *rejected_refs;
};

స్థిర శూన్య ref_transaction_rejection_handler(స్థిరరాశి అక్షరం *refname,
					      స్థిరరాశి నిర్మాణం object_id *old_oid వాడని,
					      స్థిరరాశి నిర్మాణం object_id *new_oid వాడని,
					      స్థిరరాశి అక్షరం *old_లక్ష్యం వాడని,
					      స్థిరరాశి అక్షరం *new_లక్ష్యం వాడని,
					      గణన ref_transaction_error err,
					      స్థిరరాశి అక్షరం *details,
					      శూన్య *cb_దత్తాంశం)
{
	నిర్మాణం ref_rejection_దత్తాంశం *దత్తాంశం = cb_దత్తాంశం;

	ఒకవేళ (err == REF_TRANSACTION_ERROR_CASE_CONFLICT && repo_ignore_case(the_repository) &&
	    !దత్తాంశం->case_sensitive_సందేశం_shown) {
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
		దత్తాంశం->case_sensitive_సందేశం_shown = true;
	} లేకపోతే ఒకవేళ (err == REF_TRANSACTION_ERROR_NAME_CONFLICT &&
		   !దత్తాంశం->conflict_సందేశం_shown) {
		error(_("some local refs could not be updated; try running\n"
			" 'git remote prune %s' to remove any old, conflicting "
			"branches"), దత్తాంశం->remote_name);
		దత్తాంశం->conflict_సందేశం_shown = true;
	} లేకపోతే {
		ఒకవేళ (details)
			error("%s", details);
		లేకపోతే
			error(_("fetching ref %s failed: %s"),
			      refname, ref_transaction_error_సందేశం(err));
	}

	strmap_put(దత్తాంశం->rejected_refs, refname, ఖాళీ);
	*దత్తాంశం->retcode = 1;
}

/*
 * Commit the reference transaction. If it isn't an atomic transaction, handle
 * rejected updates as part of using batched updates.
 */
స్థిర పూర్ణాంకం commit_ref_transaction(నిర్మాణం ref_transaction **transaction,
				  bool is_atomic, స్థిరరాశి అక్షరం *remote_name,
				  నిర్మాణం strmap *rejected_refs,
				  నిర్మాణం strbuf *err)
{
	పూర్ణాంకం retcode = ref_transaction_commit(*transaction, err);
	ఒకవేళ (retcode)
		వెళ్ళు బహిర్గతం;

	ఒకవేళ (!is_atomic) {
		నిర్మాణం ref_rejection_దత్తాంశం దత్తాంశం = {
			.conflict_సందేశం_shown = 0,
			.remote_name = remote_name,
			.retcode = &retcode,
			.rejected_refs = rejected_refs,
		};

		ref_transaction_for_each_rejected_update(*transaction,
							 ref_transaction_rejection_handler,
							 &దత్తాంశం);
	}

బహిర్గతం:
	ref_transaction_free(*transaction);
	*transaction = ఖాళీ;
	తిరిగి retcode;
}

స్థిర శూన్య collect_updated_tips(నిర్మాణం oidset *tips, నిర్మాణం ref *ref_map)
{
	నిర్మాణం ref *rm;
	కొరకు (rm = ref_map; rm; rm = rm->తదుపరి) {
		నిర్మాణం commit *commit;
		/*
		 * Shallow-rejected refs are not stored and their history
		 * is incomplete, so skip them.
		 */
		ఒకవేళ (rm->స్థితి_2 == REF_స్థితి_REJECT_SHALLOW)
			కొనసాగు;
		ఒకవేళ (is_null_oid(&rm->old_oid))
			కొనసాగు;
		ఒకవేళ (rm->peer_ref &&
		    oideq(&rm->old_oid, &rm->peer_ref->old_oid))
			కొనసాగు;
		commit = lookup_commit_reference_gently(the_repository,
							&rm->old_oid, 1);
		ఒకవేళ (commit)
			oidset_చొప్పించు(tips, &commit->object.oid);
	}
}

స్థిర పూర్ణాంకం do_fetch(నిర్మాణం transport *transport,
		    నిర్మాణం refspec *rs,
		    స్థిరరాశి నిర్మాణం fetch_config *config,
		    నిర్మాణం list_objects_filter_options *filter_options,
		    నిర్మాణం oidset *updated_tips)
{
	నిర్మాణం ref_transaction *transaction = ఖాళీ;
	నిర్మాణం ref *ref_map = ఖాళీ;
	నిర్మాణం display_state display_state = { 0 };
	పూర్ణాంకం autotags = (transport->remote->fetch_ట్యాగులు == 1);
	పూర్ణాంకం retcode = 0;
	స్థిరరాశి నిర్మాణం ref *remote_refs;
	నిర్మాణం transport_ls_refs_options transport_ls_refs_options =
		TRANSPORT_LS_REFS_OPTIONS_INIT;
	నిర్మాణం fetch_తల fetch_తల = { 0 };
	నిర్మాణం strbuf err = STRBUF_INIT;
	నిర్మాణం ref_update_display_info_array display_array = { 0 };
	నిర్మాణం strmap rejected_refs = STRMAP_INIT;
	పూర్ణాంకం summary_వెడల్పు = 0;
	పూర్ణాంకం follow_remote_తల = FOLLOW_REMOTE_NEVER;

	ఒకవేళ (ట్యాగులు == ట్యాగులు_DEFAULT) {
		ఒకవేళ (transport->remote->fetch_ట్యాగులు == 2)
			ట్యాగులు = ట్యాగులు_సమితి;
		ఒకవేళ (transport->remote->fetch_ట్యాగులు == -1)
			ట్యాగులు = ట్యాగులు_UNSET;
	}

	/* if not appending, truncate FETCH_HEAD */
	ఒకవేళ (!append && write_fetch_తల) {
		retcode = truncate_fetch_తల();
		ఒకవేళ (retcode)
			వెళ్ళు cleanup;
	}

	ఒకవేళ (rs->nr) {
		refspec_ref_prefixes(rs, &transport_ls_refs_options.ref_prefixes);
	} లేకపోతే {
		నిర్మాణం branch *branch = branch_get(ఖాళీ);

		ఒకవేళ (transport->remote->fetch.nr) {
			refspec_ref_prefixes(&transport->remote->fetch,
					     &transport_ls_refs_options.ref_prefixes);
			/*
			 * See remote.c's handling of remote.<name>.followRemoteHEAD
			 * for the analogous, still-unresolved case.
			 */
			ఒకవేళ (transport->remote->follow_remote_తల)
				follow_remote_తల = transport->remote->follow_remote_తల;
			లేకపోతే ఒకవేళ (config->follow_remote_తల_చూసిన)
				follow_remote_తల = get_follow_remote_తల(config->follow_remote_తల_raw);
			లేకపోతే
				follow_remote_తల = BUILTIN_FOLLOW_REMOTE_తల_DFLT;
		}
		ఒకవేళ (branch && branch_has_విలీనం_config(branch) &&
		    !strcmp(branch->remote_name, transport->remote->name)) {
			పూర్ణాంకం i;
			కొరకు (i = 0; i < branch->విలీనం_nr; i++) {
				strvec_నెట్టు(&transport_ls_refs_options.ref_prefixes,
					    branch->విలీనం[i]->src);
			}
		}

		/*
		 * If there are no refs specified to fetch, then we just
		 * fetch HEAD; mention that to narrow the advertisement.
		 */
		ఒకవేళ (!transport_ls_refs_options.ref_prefixes.nr)
			strvec_నెట్టు(&transport_ls_refs_options.ref_prefixes,
				    "HEAD");
	}

	ఒకవేళ (ట్యాగులు == ట్యాగులు_సమితి || ట్యాగులు == ట్యాగులు_DEFAULT)
		strvec_నెట్టు(&transport_ls_refs_options.ref_prefixes,
			    "refs/tags/");

	ఒకవేళ (follow_remote_తల != FOLLOW_REMOTE_NEVER)
		strvec_నెట్టు(&transport_ls_refs_options.ref_prefixes,
			    "HEAD");

	/*
	 * Only initiate ref listing if we have at least one ref we want to
	 * know about.
	 */
	ఒకవేళ (transport_ls_refs_options.ref_prefixes.nr) {
		trace2_region_enter("fetch", "remote_refs", the_repository);
		remote_refs = transport_get_remote_refs(transport,
							&transport_ls_refs_options);
		trace2_region_leave("fetch", "remote_refs", the_repository);
	} లేకపోతే
		remote_refs = ఖాళీ;

	transport_ls_refs_options_release(&transport_ls_refs_options);

	ref_map = get_ref_map(transport->remote, remote_refs, rs,
			      ట్యాగులు, &autotags);
	ఒకవేళ (!update_తల_ok)
		check_not_current_branch(ref_map);

	retcode = open_fetch_తల(&fetch_తల);
	ఒకవేళ (retcode)
		వెళ్ళు cleanup;

	display_state_init(&display_state, ref_map, transport->url,
			   config->display_format);

	ఒకవేళ (atomic_fetch) {
		transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
							  0, &err);
		ఒకవేళ (!transaction) {
			retcode = -1;
			వెళ్ళు cleanup;
		}
	}

	ఒకవేళ (ట్యాగులు == ట్యాగులు_DEFAULT && autotags)
		transport_సమితి_option(transport, TRANS_OPT_FOLLOWTAGS, "1");
	ఒకవేళ (prune) {
		/*
		 * We only prune based on refspecs specified
		 * explicitly (via command line or configuration); we
		 * don't care whether --tags was specified.
		 */
		ఒకవేళ (rs->nr) {
			retcode = prune_refs(&display_state, rs, transaction, ref_map);
		} లేకపోతే {
			retcode = prune_refs(&display_state, &transport->remote->fetch,
					     transaction, ref_map);
		}
		ఒకవేళ (retcode != 0)
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
	ఒకవేళ (!transaction) {
		transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
							  REF_TRANSACTION_ALLOW_FAILURE, &err);
		ఒకవేళ (!transaction) {
			retcode = -1;
			వెళ్ళు cleanup;
		}
	}

	ఒకవేళ (fetch_and_consume_refs(&display_state, transport, transaction, ref_map,
				   &fetch_తల, config, &display_array)) {
		retcode = 1;
		వెళ్ళు cleanup;
	}

	/*
	 * If neither --no-tags nor --tags was specified, do automated tag
	 * following.
	 */
	ఒకవేళ (ట్యాగులు == ట్యాగులు_DEFAULT && autotags) {
		నిర్మాణం ref *ట్యాగులు_ref_map = ఖాళీ, **తోక = &ట్యాగులు_ref_map;

		కనుగొను_non_local_ట్యాగులు(remote_refs, transaction, &ట్యాగులు_ref_map, &తోక);
		ఒకవేళ (ట్యాగులు_ref_map) {
			/*
			 * If backfilling of tags fails then we want to tell
			 * the user so, but we have to continue regardless to
			 * populate upstream information of the references we
			 * have already fetched above. The exception though is
			 * when `--atomic` is passed: in that case we'll abort
			 * the transaction and don't commit anything.
			 */
			ఒకవేళ (backfill_ట్యాగులు(&display_state, transport, transaction, ట్యాగులు_ref_map,
					  &fetch_తల, config, &display_array, filter_options))
				retcode = 1;
		}

		free_refs(ట్యాగులు_ref_map);
	}

	ఒకవేళ (retcode)
		వెళ్ళు cleanup;

	ఒకవేళ (verbosity >= 0)
		summary_వెడల్పు = transport_summary_వెడల్పు(ref_map);

	retcode = commit_ref_transaction(&transaction, atomic_fetch,
					 transport->remote->name,
					 &rejected_refs, &err);
	/*
	 * With '--atomic', bail out if the transaction fails. Without '--atomic',
	 * continue to fetch head and perform other post-fetch operations.
	 */
	ఒకవేళ (retcode && atomic_fetch)
		వెళ్ళు cleanup;

	commit_fetch_తల(&fetch_తల);

	collect_updated_tips(updated_tips, ref_map);

	ఒకవేళ (సమితి_upstream) {
		నిర్మాణం branch *branch = branch_get("HEAD");
		నిర్మాణం ref *rm;
		నిర్మాణం ref *ఆధారం_ref = ఖాళీ;

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
		కొరకు (rm = ref_map; rm; rm = rm->తదుపరి) {
			ఒకవేళ (!rm->peer_ref) {
				ఒకవేళ (ఆధారం_ref) {
					warning(_("multiple branches detected, incompatible with --set-upstream"));
					వెళ్ళు cleanup;
				} లేకపోతే {
					ఆధారం_ref = rm;
				}
			}
		}
		ఒకవేళ (ఆధారం_ref) {
			ఒకవేళ (!branch) {
				స్థిరరాశి అక్షరం *shortname = ఆధారం_ref->name;
				skip_ఉపసర్గ(shortname, "refs/heads/", &shortname);

				warning(_("could not set upstream of HEAD to '%s' from '%s' when "
					  "it does not point to any branch."),
					shortname, transport->remote->name);
				వెళ్ళు cleanup;
			}

			ఒకవేళ (!strcmp(ఆధారం_ref->name, "HEAD") ||
			    starts_with(ఆధారం_ref->name, "refs/heads/"))
				install_branch_config(0,
						      branch->name,
						      transport->remote->name,
						      ఆధారం_ref->name);
			లేకపోతే ఒకవేళ (starts_with(ఆధారం_ref->name, "refs/remotes/"))
				warning(_("not setting upstream for a remote remote-tracking branch"));
			లేకపోతే ఒకవేళ (starts_with(ఆధారం_ref->name, "refs/tags/"))
				warning(_("not setting upstream for a remote tag"));
			లేకపోతే
				warning(_("unknown branch type"));
		} లేకపోతే {
			warning(_("no source branch found;\n"
				  "you need to specify exactly one branch with the --set-upstream option"));
		}
	}
	ఒకవేళ (follow_remote_తల != FOLLOW_REMOTE_NEVER) {
		/*
		 * Way too many cases where this can go wrong so let's just
		 * ignore errors and fail silently for now.
		 */
		సమితి_తల(remote_refs, transport->remote, follow_remote_తల);
	}

cleanup:
	/*
	 * When using batched updates, we want to commit the non-rejected
	 * updates and also handle the rejections.
	 */
	ఒకవేళ (retcode && !atomic_fetch && transaction)
		commit_ref_transaction(&transaction, false,
				       transport->remote->name,
				       &rejected_refs, &err);

	కొరకు (కొలత_t i = 0; i < display_array.nr; i++) {
		నిర్మాణం ref_update_display_info *info = &display_array.info[i];

		ఒకవేళ (!info->failed && strmap_కలిగియుంది(&rejected_refs, info->ref))
			ref_update_display_info_సమితి_failed(info);
		ref_update_display_info_display(info, &display_state, summary_వెడల్పు);
		ref_update_display_info_free(info);
	}

	ఒకవేళ (retcode) {
		ఒకవేళ (err.len) {
			error("%s", err.buf);
			strbuf_reset(&err);
		}
		ఒకవేళ (transaction && ref_transaction_abort(transaction, &err) &&
		    err.len)
			error("%s", err.buf);
		transaction = ఖాళీ;
	}

	ఒకవేళ (transaction)
		ref_transaction_free(transaction);

	విడుదల(display_array.info);
	strmap_clear(&rejected_refs, 0);
	display_state_release(&display_state);
	close_fetch_తల(&fetch_తల);
	strbuf_release(&err);
	free_refs(ref_map);
	తిరిగి retcode;
}

స్థిర పూర్ణాంకం get_one_remote_for_fetch(నిర్మాణం remote *remote, శూన్య *priv)
{
	నిర్మాణం string_list *list = priv;
	ఒకవేళ (!remote->skip_default_update)
		string_list_append(list, remote->name);
	తిరిగి 0;
}

స్థిర శూన్య కూడు_options_గమ్యం_argv(నిర్మాణం strvec *argv,
				స్థిరరాశి నిర్మాణం fetch_config *config)
{
	ఒకవేళ (dry_నిరంతరం)
		strvec_నెట్టు(argv, "--dry-run");
	ఒకవేళ (prune != -1)
		strvec_నెట్టు(argv, prune ? "--prune" : "--no-prune");
	ఒకవేళ (prune_ట్యాగులు != -1)
		strvec_నెట్టు(argv, prune_ట్యాగులు ? "--prune-tags" : "--no-prune-tags");
	ఒకవేళ (update_తల_ok)
		strvec_నెట్టు(argv, "--update-head-ok");
	ఒకవేళ (force)
		strvec_నెట్టు(argv, "--force");
	ఒకవేళ (keep)
		strvec_నెట్టు(argv, "--keep");
	ఒకవేళ (config->recurse_submodules == RECURSE_SUBMODULES_ON)
		strvec_నెట్టు(argv, "--recurse-submodules");
	లేకపోతే ఒకవేళ (config->recurse_submodules == RECURSE_SUBMODULES_OFF)
		strvec_నెట్టు(argv, "--no-recurse-submodules");
	లేకపోతే ఒకవేళ (config->recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND)
		strvec_నెట్టు(argv, "--recurse-submodules=on-demand");
	ఒకవేళ (ట్యాగులు == ట్యాగులు_సమితి)
		strvec_నెట్టు(argv, "--tags");
	లేకపోతే ఒకవేళ (ట్యాగులు == ట్యాగులు_UNSET)
		strvec_నెట్టు(argv, "--no-tags");
	ఒకవేళ (verbosity >= 2)
		strvec_నెట్టు(argv, "-v");
	ఒకవేళ (verbosity >= 1)
		strvec_నెట్టు(argv, "-v");
	లేకపోతే ఒకవేళ (verbosity < 0)
		strvec_నెట్టు(argv, "-q");
	ఒకవేళ (family == TRANSPORT_FAMILY_IPV4)
		strvec_నెట్టు(argv, "--ipv4");
	లేకపోతే ఒకవేళ (family == TRANSPORT_FAMILY_IPV6)
		strvec_నెట్టు(argv, "--ipv6");
	ఒకవేళ (!write_fetch_తల)
		strvec_నెట్టు(argv, "--no-write-fetch-head");
	ఒకవేళ (config->display_format == DISPLAY_FORMAT_PORCELAIN)
		strvec_pushf(argv, "--porcelain");
}

/* Fetch multiple remotes in parallel */

నిర్మాణం parallel_fetch_state {
	స్థిరరాశి అక్షరం **argv;
	నిర్మాణం string_list *remotes;
	పూర్ణాంకం తదుపరి, ఫలితం;
	స్థిరరాశి నిర్మాణం fetch_config *config;
};

స్థిర పూర్ణాంకం fetch_తదుపరి_remote(నిర్మాణం బిడ్డ_process *cp,
			     నిర్మాణం strbuf *బహిర్గతం వాడని,
			     శూన్య *cb, శూన్య **task_cb)
{
	నిర్మాణం parallel_fetch_state *state = cb;
	అక్షరం *remote;

	ఒకవేళ (state->తదుపరి < 0 || state->తదుపరి >= state->remotes->nr)
		తిరిగి 0;

	remote = state->remotes->వస్తువులు[state->తదుపరి++].string;
	*task_cb = remote;

	strvec_pushv(&cp->వాదనలు, state->argv);
	strvec_నెట్టు(&cp->వాదనలు, remote);
	cp->git_cmd = 1;

	ఒకవేళ (verbosity >= 0 && state->config->display_format != DISPLAY_FORMAT_PORCELAIN)
		రాయి(_("Fetching %s\n"), remote);

	తిరిగి 1;
}

స్థిర పూర్ణాంకం fetch_failed_గమ్యం_ప్రారంభం(నిర్మాణం strbuf *బహిర్గతం వాడని,
				 శూన్య *cb, శూన్య *task_cb)
{
	నిర్మాణం parallel_fetch_state *state = cb;
	స్థిరరాశి అక్షరం *remote = task_cb;

	state->ఫలితం = error(_("could not fetch %s"), remote);

	తిరిగి 0;
}

స్థిర పూర్ణాంకం fetch_ముగిసిన(పూర్ణాంకం ఫలితం, నిర్మాణం strbuf *బహిర్గతం,
			  శూన్య *cb, శూన్య *task_cb)
{
	నిర్మాణం parallel_fetch_state *state = cb;
	స్థిరరాశి అక్షరం *remote = task_cb;

	ఒకవేళ (ఫలితం) {
		strbuf_addf(బహిర్గతం, _("could not fetch '%s' (exit code: %d)\n"),
			    remote, ఫలితం);
		state->ఫలితం = -1;
	}

	తిరిగి 0;
}

స్థిర పూర్ణాంకం fetch_గుణిజం(నిర్మాణం string_list *list, పూర్ణాంకం max_children,
			  స్థిరరాశి నిర్మాణం fetch_config *config)
{
	పూర్ణాంకం i, ఫలితం = 0;
	నిర్మాణం strvec argv = STRVEC_INIT;

	ఒకవేళ (!append && write_fetch_తల) {
		పూర్ణాంకం errcode = truncate_fetch_తల();
		ఒకవేళ (errcode)
			తిరిగి errcode;
	}

	/*
	 * Cancel out the fetch.bundleURI config when running subprocesses,
	 * to avoid fetching from the same bundle list multiple times.
	 */
	strvec_pushl(&argv, "-c", "fetch.bundleURI=",
		     "fetch", "--append", "--no-auto-gc",
		     "--no-write-commit-graph", ఖాళీ);
	కొరకు (i = 0; i < server_options.nr; i++)
		strvec_pushf(&argv, "--server-option=%s", server_options.వస్తువులు[i].string);
	కూడు_options_గమ్యం_argv(&argv, config);

	ఒకవేళ (max_children != 1 && list->nr != 1) {
		నిర్మాణం parallel_fetch_state state = { argv.v, list, 0, 0, config };
		స్థిరరాశి నిర్మాణం నిరంతరం_process_parallel_opts opts = {
			.tr2_category = "fetch",
			.tr2_label = "parallel/fetch",

			.processes = max_children,

			.get_తదుపరి_task = &fetch_తదుపరి_remote,
			.ప్రారంభం_failure = &fetch_failed_గమ్యం_ప్రారంభం,
			.task_ముగిసిన = &fetch_ముగిసిన,
			.దత్తాంశం = &state,
		};

		strvec_నెట్టు(&argv, "--end-of-options");

		నిరంతరం_processes_parallel(&opts);
		ఫలితం = state.ఫలితం;
	} లేకపోతే
		కొరకు (i = 0; i < list->nr; i++) {
			స్థిరరాశి అక్షరం *name = list->వస్తువులు[i].string;
			నిర్మాణం బిడ్డ_process cmd = బిడ్డ_PROCESS_INIT;

			strvec_pushv(&cmd.వాదనలు, argv.v);
			strvec_నెట్టు(&cmd.వాదనలు, name);
			ఒకవేళ (verbosity >= 0 && config->display_format != DISPLAY_FORMAT_PORCELAIN)
				రాయి(_("Fetching %s\n"), name);
			cmd.git_cmd = 1;
			ఒకవేళ (నిరంతరం_command(&cmd)) {
				error(_("could not fetch %s"), name);
				ఫలితం = 1;
			}
		}

	strvec_clear(&argv);
	తిరిగి !!ఫలితం;
}

/*
 * Fetching from the promisor remote should use the given filter-spec
 * or inherit the default filter-spec from the config.
 */
స్థిర అంతఃవరుస శూన్య fetch_one_అమరిక_partial(నిర్మాణం remote *remote,
					   నిర్మాణం list_objects_filter_options *filter_options)
{
	/*
	 * Explicit --no-filter argument overrides everything, regardless
	 * of any prior partial clones and fetches.
	 */
	ఒకవేళ (filter_options->no_filter)
		తిరిగి;

	/*
	 * If no prior partial clone/fetch and the current fetch DID NOT
	 * request a partial-fetch, do a normal fetch.
	 */
	ఒకవేళ (!repo_has_promisor_remote(the_repository) && !filter_options->choice)
		తిరిగి;

	/*
	 * If this is a partial-fetch request, we enable partial on
	 * this repo if not already enabled and remember the given
	 * filter-spec as the default for subsequent fetches to this
	 * remote if there is currently no default filter-spec.
	 */
	ఒకవేళ (filter_options->choice) {
		partial_clone_రిజిస్టరు(remote->name, filter_options);
		తిరిగి;
	}

	/*
	 * Do a partial-fetch from the promisor remote using either the
	 * explicitly given filter-spec or inherit the filter-spec from
	 * the config.
	 */
	ఒకవేళ (!filter_options->choice)
		partial_clone_get_default_filter_spec(filter_options, remote->name);
	తిరిగి;
}

స్థిర పూర్ణాంకం fetch_one(నిర్మాణం remote *remote, పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv,
		     పూర్ణాంకం prune_ట్యాగులు_ok, పూర్ణాంకం use_stdin_refspecs,
		     స్థిరరాశి నిర్మాణం fetch_config *config,
		     నిర్మాణం list_objects_filter_options *filter_options,
		     నిర్మాణం oidset *updated_tips)
{
	నిర్మాణం refspec rs = REFSPEC_INIT_FETCH(the_hash_algo);
	పూర్ణాంకం i;
	పూర్ణాంకం exit_సంకేతం;
	పూర్ణాంకం maybe_prune_ట్యాగులు;
	పూర్ణాంకం remote_ద్వారా_config = remote_is_configured(remote, 0);

	ఒకవేళ (!remote)
		die(_("no remote repository specified; please specify either a URL or a\n"
		      "remote name from which new revisions should be fetched"));

	gtransport = prepare_transport(remote, 1, filter_options);

	ఒకవేళ (prune < 0) {
		/* no command line request */
		ఒకవేళ (0 <= remote->prune)
			prune = remote->prune;
		లేకపోతే ఒకవేళ (0 <= config->prune)
			prune = config->prune;
		లేకపోతే
			prune = PRUNE_BY_DEFAULT;
	}

	ఒకవేళ (prune_ట్యాగులు < 0) {
		/* no command line request */
		ఒకవేళ (0 <= remote->prune_ట్యాగులు)
			prune_ట్యాగులు = remote->prune_ట్యాగులు;
		లేకపోతే ఒకవేళ (0 <= config->prune_ట్యాగులు)
			prune_ట్యాగులు = config->prune_ట్యాగులు;
		లేకపోతే
			prune_ట్యాగులు = PRUNE_ట్యాగులు_BY_DEFAULT;
	}

	maybe_prune_ట్యాగులు = prune_ట్యాగులు_ok && prune_ట్యాగులు;
	ఒకవేళ (maybe_prune_ట్యాగులు && remote_ద్వారా_config)
		refspec_append(&remote->fetch, ట్యాగ్_REFSPEC);

	ఒకవేళ (maybe_prune_ట్యాగులు && (argc || !remote_ద్వారా_config))
		refspec_append(&rs, ట్యాగ్_REFSPEC);

	కొరకు (i = 0; i < argc; i++) {
		ఒకవేళ (!strcmp(argv[i], "tag")) {
			i++;
			ఒకవేళ (i >= argc)
				die(_("you need to specify a tag name"));

			refspec_appendf(&rs, "refs/tags/%s:refs/tags/%s",
					argv[i], argv[i]);
		} లేకపోతే {
			refspec_append(&rs, argv[i]);
		}
	}

	ఒకవేళ (use_stdin_refspecs) {
		నిర్మాణం strbuf పంక్తి = STRBUF_INIT;
		అయితే (strbuf_getline_lf(&పంక్తి, stdin) != EOF)
			refspec_append(&rs, పంక్తి.buf);
		strbuf_release(&పంక్తి);
	}

	ఒకవేళ (server_options.nr)
		gtransport->server_options = &server_options;

	sigchain_నెట్టు_common(unlock_pack_on_signal);
	atexit(unlock_pack_atexit);
	sigchain_నెట్టు(SIGPIPE, SIG_IGN);
	exit_సంకేతం = do_fetch(gtransport, &rs, config, filter_options,
			     updated_tips);
	sigchain_తీయి(SIGPIPE);
	refspec_clear(&rs);
	transport_disconnect(gtransport);
	gtransport = ఖాళీ;
	తిరిగి exit_సంకేతం;
}

పూర్ణాంకం cmd_fetch(పూర్ణాంకం argc,
	      స్థిరరాశి అక్షరం **argv,
	      స్థిరరాశి అక్షరం *ఉపసర్గ,
	      నిర్మాణం repository *repo వాడని)
{
	నిర్మాణం fetch_config config = {
		.display_format = DISPLAY_FORMAT_నిండిన,
		.follow_remote_తల_raw = ఖాళీ,
		.follow_remote_తల_చూసిన = 0,
		.prune = -1,
		.prune_ట్యాగులు = -1,
		.show_forced_updates = 1,
		.recurse_submodules = RECURSE_SUBMODULES_DEFAULT,
		.parallel = 1,
		.submodule_fetch_jobs = -1,
	};
	స్థిరరాశి అక్షరం *submodule_ఉపసర్గ = "";
	స్థిరరాశి అక్షరం *bundle_uri;
	నిర్మాణం string_list list = STRING_LIST_INIT_DUP;
	నిర్మాణం list_objects_filter_options filter_options = LIST_OBJECTS_FILTER_INIT;
	నిర్మాణం remote *remote = ఖాళీ;
	పూర్ణాంకం all = -1, గుణిజం = 0;
	పూర్ణాంకం ఫలితం = 0;
	పూర్ణాంకం prune_ట్యాగులు_ok = 1;
	పూర్ణాంకం enable_auto_gc = 1;
	పూర్ణాంకం unshallow = 0;
	పూర్ణాంకం max_jobs = -1;
	పూర్ణాంకం recurse_submodules_cli = RECURSE_SUBMODULES_DEFAULT;
	పూర్ణాంకం recurse_submodules_default = RECURSE_SUBMODULES_ON_DEMAND;
	పూర్ణాంకం fetch_write_commit_లేఖాచిత్రం = -1;
	పూర్ణాంకం stdin_refspecs = 0;
	పూర్ణాంకం negotiate_only = 0;
	పూర్ణాంకం porcelain = 0;
	పూర్ణాంకం i;
	గణన {
		లేఖాచిత్రం_WRITE_REACHABLE,
		లేఖాచిత్రం_WRITE_TIPS,
		లేఖాచిత్రం_WRITE_SKIP,
	} లేఖాచిత్రం_write_బహుళకం = లేఖాచిత్రం_WRITE_REACHABLE;
	నిర్మాణం oidset updated_tips = OIDSET_INIT;

	నిర్మాణం option builtin_fetch_options[] = {
		OPT__VERBOSITY(&verbosity),
		OPT_BOOL(0, "all", &all,
			 N_("fetch from all remotes")),
		OPT_BOOL(0, "set-upstream", &సమితి_upstream,
			 N_("set upstream for git pull/fetch")),
		OPT_BOOL('a', "append", &append,
			 N_("append to .git/FETCH_HEAD instead of overwriting")),
		OPT_BOOL(0, "atomic", &atomic_fetch,
			 N_("use atomic transaction to update references")),
		OPT_STRING(0, "upload-pack", &upload_pack, N_("path"),
			   N_("path to upload pack on remote end")),
		OPT__FORCE(&force, N_("force overwrite of local reference"), 0),
		OPT_BOOL('m', "multiple", &గుణిజం,
			 N_("fetch from multiple remotes")),
		OPT_సమితి_INT('t', "tags", &ట్యాగులు,
			    N_("fetch all tags and associated objects"), ట్యాగులు_సమితి),
		OPT_సమితి_INT('n', ఖాళీ, &ట్యాగులు,
			    N_("do not fetch all tags (--no-tags)"), ట్యాగులు_UNSET),
		OPT_INTEGER('j', "jobs", &max_jobs,
			    N_("number of submodules fetched in parallel")),
		OPT_BOOL(0, "prefetch", &prefetch,
			 N_("modify the refspec to place all refs within refs/prefetch/")),
		OPT_BOOL('p', "prune", &prune,
			 N_("prune remote-tracking branches no longer on remote")),
		OPT_BOOL('P', "prune-tags", &prune_ట్యాగులు,
			 N_("prune local tags no longer on remote and clobber changed tags")),
		OPT_CALLBACK_F(0, "recurse-submodules", &recurse_submodules_cli, N_("on-demand"),
			    N_("control recursive fetching of submodules"),
			    PARSE_OPT_OPTARG, option_fetch_parse_recurse_submodules),
		OPT_BOOL(0, "dry-run", &dry_నిరంతరం,
			 N_("dry run")),
		OPT_BOOL(0, "porcelain", &porcelain, N_("machine-readable output")),
		OPT_BOOL(0, "write-fetch-head", &write_fetch_తల,
			 N_("write fetched references to the FETCH_HEAD file")),
		OPT_BOOL('k', "keep", &keep, N_("keep downloaded pack")),
		OPT_BOOL('u', "update-head-ok", &update_తల_ok,
			    N_("allow updating of HEAD ref")),
		OPT_BOOL(0, "progress", &పురోగతి, N_("force progress reporting")),
		OPT_STRING(0, "depth", &లోతు, N_("depth"),
			   N_("deepen history of shallow clone")),
		OPT_STRING(0, "shallow-since", &deepen_since, N_("time"),
			   N_("deepen history of shallow repository based on time")),
		OPT_STRING_LIST(0, "shallow-exclude", &deepen_not, N_("ref"),
				N_("deepen history of shallow clone, excluding ref")),
		OPT_INTEGER(0, "deepen", &deepen_relative,
			    N_("deepen history of shallow clone")),
		OPT_సమితి_INT_F(0, "unshallow", &unshallow,
			      N_("convert to a complete repository"),
			      1, PARSE_OPT_NONEG),
		OPT_సమితి_INT_F(0, "refetch", &refetch,
			      N_("re-fetch without negotiating common commits"),
			      1, PARSE_OPT_NONEG),
		{
			.type = OPTION_STRING,
			.long_name = "submodule-prefix",
			.విలువ = &submodule_ఉపసర్గ,
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
		OPT_BOOL(0, "write-commit-graph", &fetch_write_commit_లేఖాచిత్రం,
			 N_("write the commit-graph after fetching")),
		OPT_BOOL(0, "stdin", &stdin_refspecs,
			 N_("accept refspecs from stdin")),
		OPT_చివర()
	};

	filter_options.allow_auto_filter = 1;

	refspec_init_fetch(&refmap, the_hash_algo);

	packet_trace_తటస్థం("fetch");

	/* Record the command line for the reflog */
	strbuf_addstr(&default_rla, "fetch");
	కొరకు (i = 1; i < argc; i++) {
		/* This handles non-URLs gracefully */
		అక్షరం *anon = transport_anonymize_url(argv[i]);

		strbuf_addf(&default_rla, " %s", anon);
		విడుదల(anon);
	}

	repo_config(the_repository, git_fetch_config, &config);
	ఒకవేళ (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_నిండిన_సూచీ = 0;
	}

	argc = parse_options(argc, argv, ఉపసర్గ,
			     builtin_fetch_options, builtin_fetch_usage, 0);

	ఒకవేళ (recurse_submodules_cli != RECURSE_SUBMODULES_DEFAULT)
		config.recurse_submodules = recurse_submodules_cli;

	ఒకవేళ (negotiate_only) {
		ఎంపిక (recurse_submodules_cli) {
		స్థితి RECURSE_SUBMODULES_OFF:
		స్థితి RECURSE_SUBMODULES_DEFAULT:
			/*
			 * --negotiate-only should never recurse into
			 * submodules. Skip it by setting recurse_submodules to
			 * RECURSE_SUBMODULES_OFF.
			 */
			config.recurse_submodules = RECURSE_SUBMODULES_OFF;
			ఆపు;

		అప్రమేయ:
			die(_("options '%s' and '%s' cannot be used together"),
			    "--negotiate-only", "--recurse-submodules");
		}
	}

	ఒకవేళ (config.recurse_submodules != RECURSE_SUBMODULES_OFF) {
		పూర్ణాంకం *sfjc = config.submodule_fetch_jobs == -1
			    ? &config.submodule_fetch_jobs : ఖాళీ;
		పూర్ణాంకం *rs = config.recurse_submodules == RECURSE_SUBMODULES_DEFAULT
			  ? &config.recurse_submodules : ఖాళీ;

		fetch_config_from_gitmodules(the_repository, sfjc, rs);
	}


	ఒకవేళ (porcelain) {
		ఎంపిక (recurse_submodules_cli) {
		స్థితి RECURSE_SUBMODULES_OFF:
		స్థితి RECURSE_SUBMODULES_DEFAULT:
			/*
			 * Reference updates in submodules would be ambiguous
			 * in porcelain mode, so we reject this combination.
			 */
			config.recurse_submodules = RECURSE_SUBMODULES_OFF;
			ఆపు;

		అప్రమేయ:
			die(_("options '%s' and '%s' cannot be used together"),
			    "--porcelain", "--recurse-submodules");
		}

		config.display_format = DISPLAY_FORMAT_PORCELAIN;
	}

	ఒకవేళ (deepen_relative) {
		ఒకవేళ (deepen_relative < 0)
			die(_("negative depth in --deepen is not supported"));
		ఒకవేళ (లోతు)
			die(_("options '%s' and '%s' cannot be used together"), "--deepen", "--depth");
		లోతు = xstrfmt("%d", deepen_relative);
	}
	ఒకవేళ (unshallow) {
		ఒకవేళ (లోతు)
			die(_("options '%s' and '%s' cannot be used together"), "--depth", "--unshallow");
		లేకపోతే ఒకవేళ (!is_repository_shallow(the_repository))
			die(_("--unshallow on a complete repository does not make sense"));
		లేకపోతే
			లోతు = xstrfmt("%d", INFINITE_లోతు);
	}

	/* no need to be strict, transport_set_option() will validate it again */
	ఒకవేళ (లోతు && atoi(లోతు) < 1)
		die(_("depth %s is not a positive number"), లోతు);
	ఒకవేళ (లోతు || deepen_since || deepen_not.nr)
		deepen = 1;

	/* FETCH_HEAD never gets updated in --dry-run mode */
	ఒకవేళ (dry_నిరంతరం)
		write_fetch_తల = 0;

	ఒకవేళ (!max_jobs)
		max_jobs = online_cpus();

	ఒకవేళ (!repo_config_get_string_tmp(the_repository, "fetch.bundleuri", &bundle_uri) &&
	    fetch_bundle_uri(the_repository, bundle_uri, ఖాళీ))
		warning(_("failed to fetch bundles from '%s'"), bundle_uri);

	ఒకవేళ (all < 0) {
		/*
		 * no --[no-]all given;
		 * only use config option if no remote was explicitly specified
		 */
		all = (!argc) ? config.all : 0;
	}

	ఒకవేళ (all) {
		ఒకవేళ (argc == 1)
			die(_("fetch --all does not take a repository argument"));
		లేకపోతే ఒకవేళ (argc > 1)
			die(_("fetch --all does not make sense with refspecs"));

		(శూన్య) for_each_remote(get_one_remote_for_fetch, &list);

		/* do not do fetch_multiple() of one */
		ఒకవేళ (list.nr == 1)
			remote = remote_get(list.వస్తువులు[0].string);
	} లేకపోతే ఒకవేళ (argc == 0) {
		/* No arguments -- use default remote */
		remote = remote_get(ఖాళీ);
	} లేకపోతే ఒకవేళ (గుణిజం) {
		/* All arguments are assumed to be remotes or groups */
		కొరకు (i = 0; i < argc; i++)
			ఒకవేళ (!కూడు_remote_or_సమూహం(argv[i], &list))
				die(_("no such remote or remote group: %s"),
				    argv[i]);
	} లేకపోతే {
		/* Single remote or group */
		(శూన్య) కూడు_remote_or_సమూహం(argv[0], &list);
		ఒకవేళ (list.nr > 1) {
			/* More than one remote */
			ఒకవేళ (argc > 1)
				die(_("fetching a group and specifying refspecs does not make sense"));
		} లేకపోతే {
			/* Zero or one remotes */
			remote = remote_get(argv[0]);
			prune_ట్యాగులు_ok = (argc == 1);
			argc--;
			argv++;
		}
	}
	string_list_remove_duplicates(&list, 0);

	ఒకవేళ (negotiate_only) {
		నిర్మాణం oidset acked_commits = OIDSET_INIT;
		నిర్మాణం oidset_iter iter;
		స్థిరరాశి నిర్మాణం object_id *oid;

		trace2_region_enter("fetch", "negotiate-only", the_repository);
		ఒకవేళ (!remote)
			die(_("must supply remote when using --negotiate-only"));
		gtransport = prepare_transport(remote, 1, &filter_options);

		ఒకవేళ (!gtransport->smart_options) {
			warning(_("protocol does not support --negotiate-only, exiting"));
			ఫలితం = 1;
			trace2_region_leave("fetch", "negotiate-only", the_repository);
			వెళ్ళు cleanup;
		}
		ఒకవేళ (!gtransport->smart_options->negotiation_restrict_tips)
			die(_("%s needs one or more %s"), "--negotiate-only",
			    "--negotiation-restrict=*");

		gtransport->smart_options->acked_commits = &acked_commits;

		ఒకవేళ (server_options.nr)
			gtransport->server_options = &server_options;
		ఫలితం = transport_fetch_refs(gtransport, ఖాళీ);
		gtransport->smart_options->acked_commits = ఖాళీ;

		oidset_iter_init(&acked_commits, &iter);
		అయితే ((oid = oidset_iter_తదుపరి(&iter)))
			రాయి("%s\n", oid_గమ్యం_hex(oid));
		oidset_clear(&acked_commits);
		trace2_region_leave("fetch", "negotiate-only", the_repository);
	} లేకపోతే ఒకవేళ (remote) {
		ఒకవేళ (filter_options.choice || repo_has_promisor_remote(the_repository)) {
			trace2_region_enter("fetch", "setup-partial", the_repository);
			fetch_one_అమరిక_partial(remote, &filter_options);
			trace2_region_leave("fetch", "setup-partial", the_repository);
		}
		trace2_region_enter("fetch", "fetch-one", the_repository);
		ఫలితం = fetch_one(remote, argc, argv, prune_ట్యాగులు_ok, stdin_refspecs,
				   &config, &filter_options, &updated_tips);
		ఒకవేళ (prepare_commit_లేఖాచిత్రం(the_repository)) {
			ఒకవేళ (oidset_కొలత(&updated_tips))
				లేఖాచిత్రం_write_బహుళకం = లేఖాచిత్రం_WRITE_TIPS;
			లేకపోతే
				లేఖాచిత్రం_write_బహుళకం = లేఖాచిత్రం_WRITE_SKIP;
		}
		trace2_region_leave("fetch", "fetch-one", the_repository);
	} లేకపోతే {
		పూర్ణాంకం max_children = max_jobs;

		ఒకవేళ (filter_options.choice)
			die(_("--filter can only be used with the remote "
			      "configured in extensions.partialclone"));

		ఒకవేళ (atomic_fetch)
			die(_("--atomic can only be used when fetching "
			      "from one remote"));

		ఒకవేళ (stdin_refspecs)
			die(_("--stdin can only be used when fetching "
			      "from one remote"));

		ఒకవేళ (max_children < 0)
			max_children = config.parallel;

		/* TODO should this also die if we have a previous partial-clone? */
		trace2_region_enter("fetch", "fetch-multiple", the_repository);
		ఫలితం = fetch_గుణిజం(&list, max_children, &config);
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
	ఒకవేళ (!ఫలితం && remote && (config.recurse_submodules != RECURSE_SUBMODULES_OFF)) {
		నిర్మాణం strvec options = STRVEC_INIT;
		పూర్ణాంకం max_children = max_jobs;

		ఒకవేళ (max_children < 0)
			max_children = config.submodule_fetch_jobs;
		ఒకవేళ (max_children < 0)
			max_children = config.parallel;

		కూడు_options_గమ్యం_argv(&options, &config);
		trace2_region_enter_printf("fetch", "recurse-submodule", the_repository, "%s", submodule_ఉపసర్గ);
		ఫలితం = fetch_submodules(the_repository,
					  &options,
					  submodule_ఉపసర్గ,
					  config.recurse_submodules,
					  recurse_submodules_default,
					  verbosity < 0,
					  max_children);
		trace2_region_leave_printf("fetch", "recurse-submodule", the_repository, "%s", submodule_ఉపసర్గ);
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
	ఒకవేళ (negotiate_only)
		వెళ్ళు cleanup;

	prepare_repo_settings(the_repository);
	ఒకవేళ (fetch_write_commit_లేఖాచిత్రం > 0 ||
	    (fetch_write_commit_లేఖాచిత్రం < 0 &&
	     the_repository->settings.fetch_write_commit_లేఖాచిత్రం)) {
		పూర్ణాంకం commit_లేఖాచిత్రం_flags = COMMIT_లేఖాచిత్రం_WRITE_విభజించు;

		ఒకవేళ (పురోగతి)
			commit_లేఖాచిత్రం_flags |= COMMIT_లేఖాచిత్రం_WRITE_పురోగతి;

		ఒకవేళ (లేఖాచిత్రం_write_బహుళకం != లేఖాచిత్రం_WRITE_SKIP) {
			trace2_region_enter("fetch", "write-commit-graph",
					    the_repository);
			ఒకవేళ (లేఖాచిత్రం_write_బహుళకం == లేఖాచిత్రం_WRITE_TIPS)
				write_commit_లేఖాచిత్రం(
					the_repository,
					the_repository->objects->ఆధారం->మార్గం,
					ఖాళీ, &updated_tips,
					commit_లేఖాచిత్రం_flags, ఖాళీ);
			లేకపోతే
				write_commit_లేఖాచిత్రం_reachable(
					the_repository,
					the_repository->objects->ఆధారం->మార్గం,
					commit_లేఖాచిత్రం_flags, ఖాళీ);
			trace2_region_leave("fetch", "write-commit-graph",
					    the_repository);
		}
	}

	ఒకవేళ (enable_auto_gc) {
		ఒకవేళ (refetch) {
			/*
			 * Hint auto-maintenance strongly to encourage repacking,
			 * but respect config settings disabling it.
			 */
			పూర్ణాంకం opt_val;
			ఒకవేళ (repo_config_get_int(the_repository, "gc.autopacklimit", &opt_val))
				opt_val = -1;
			ఒకవేళ (opt_val != 0)
				git_config_నెట్టు_parameter("gc.autoPackLimit=1");

			ఒకవేళ (repo_config_get_int(the_repository, "maintenance.incremental-repack.auto", &opt_val))
				opt_val = -1;
			ఒకవేళ (opt_val != 0)
				git_config_నెట్టు_parameter("maintenance.incremental-repack.auto=-1");
		}
		నిరంతరం_auto_maintenance(the_repository, verbosity < 0);
	}

 cleanup:
	oidset_clear(&updated_tips);
	string_list_clear(&list, 0);
	list_objects_filter_release(&filter_options);
	విడుదల(config.follow_remote_తల_raw);
	తిరిగి ఫలితం;
}
