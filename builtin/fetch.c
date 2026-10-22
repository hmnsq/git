/*
 * "git fetch"
 */

#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
#تعریف_کرو DISABLE_SIGN_COMPARE_WARNINGS

#شامل "builtin.h"
#شامل "advice.h"
#شامل "config.h"
#شامل "gettext.h"
#شامل "environment.h"
#شامل "hex.h"
#شامل "refs.h"
#شامل "refspec.h"
#شامل "object-name.h"
#شامل "odb.h"
#شامل "odb/source.h"
#شامل "oidset.h"
#شامل "oid-array.h"
#شامل "commit.h"
#شامل "string-list.h"
#شامل "remote.h"
#شامل "transport.h"
#شامل "run-command.h"
#شامل "parse-options.h"
#شامل "sigchain.h"
#شامل "submodule-config.h"
#شامل "submodule.h"
#شامل "connected.h"
#شامل "strvec.h"
#شامل "utf8.h"
#شامل "pager.h"
#شامل "path.h"
#شامل "pkt-line.h"
#شامل "list-objects-filter-options.h"
#شامل "commit-reach.h"
#شامل "branch.h"
#شامل "promisor-remote.h"
#شامل "commit-graph.h"
#شامل "shallow.h"
#شامل "trace.h"
#شامل "trace2.h"
#شامل "bundle-uri.h"

#تعریف_کرو FORCED_UPDATES_DELAY_WARNING_اندر_MS (10 * 1000)

ساکن ثابت حرف * ثابت builtin_fetch_usage[] = {
	N_("git fetch [<options>] [<repository> [<refspec>...]]"),
	N_("git fetch [<options>] <group>"),
	N_("git fetch --multiple [<options>] [(<repository>|<group>)...]"),
	N_("git fetch --all [<options>]"),
	تہی
};

شمار {
	لیبل_فہرست_UNSET = 0,
	لیبل_فہرست_DEFAULT = 1,
	لیبل_فہرست_سیٹ = 2
};

شمار display_format {
	DISPLAY_FORMAT_بھرا,
	DISPLAY_FORMAT_COMPACT,
	DISPLAY_FORMAT_PORCELAIN,
};

ساخت display_state {
	ساخت strbuf buf;

	صحیح refcol_چوڑائی;
	شمار display_format format;

	حرف *url;
	صحیح url_len, shown_url;
};

ساکن uint64_t forced_updates_ms = 0;
ساکن صحیح prefetch = 0;
ساکن صحیح prune = -1; /* unspecified */
#تعریف_کرو PRUNE_BY_DEFAULT 0 /* do we prune by default? */

ساکن صحیح prune_لیبل_فہرست = -1; /* unspecified */
#تعریف_کرو PRUNE_لیبل_فہرست_BY_DEFAULT 0 /* do we prune tags by default? */

ساکن صحیح append, dry_تسلسل, force, keep, update_سر_ok;
ساکن صحیح write_fetch_سر = 1;
ساکن صحیح verbosity, deepen_relative, سیٹ_upstream, refetch;
ساکن صحیح پیش_رفت = -1;
ساکن صحیح لیبل_فہرست = لیبل_فہرست_DEFAULT, update_shallow, deepen;
ساکن صحیح atomic_fetch;
ساکن شمار transport_family family;
ساکن ثابت حرف *گہرائی;
ساکن ثابت حرف *deepen_since;
ساکن ثابت حرف *upload_pack;
ساکن ساخت string_list deepen_not = STRING_LIST_INIT_NODUP;
ساکن ساخت strbuf default_rla = STRBUF_INIT;
ساکن ساخت transport *gtransport;
ساکن ساخت transport *gsecondary;
ساکن ساخت refspec refmap;
ساکن ساخت string_list server_options = STRING_LIST_INIT_DUP;
ساکن ساخت string_list negotiation_restrict = STRING_LIST_INIT_NODUP;
ساکن ساخت string_list negotiation_include = STRING_LIST_INIT_NODUP;

ساخت fetch_config {
	شمار display_format display_format;
	حرف *follow_remote_سر_raw;
	صحیح follow_remote_سر_نظر_آیا;
	صحیح all;
	صحیح prune;
	صحیح prune_لیبل_فہرست;
	صحیح show_forced_updates;
	صحیح recurse_submodules;
	صحیح parallel;
	صحیح submodule_fetch_jobs;
};

ساکن صحیح git_fetch_config(ثابت حرف *k, ثابت حرف *v,
			    ثابت ساخت config_context *ctx, خالی *cb)
{
	ساخت fetch_config *fetch_config = cb;

	اگر (!strcmp(k, "fetch.all")) {
		fetch_config->all = git_config_bool(k, v);
		واپس 0;
	}

	اگر (!strcmp(k, "fetch.prune")) {
		fetch_config->prune = git_config_bool(k, v);
		واپس 0;
	}

	اگر (!strcmp(k, "fetch.prunetags")) {
		fetch_config->prune_لیبل_فہرست = git_config_bool(k, v);
		واپس 0;
	}

	اگر (!strcmp(k, "fetch.showforcedupdates")) {
		fetch_config->show_forced_updates = git_config_bool(k, v);
		واپس 0;
	}

	اگر (!strcmp(k, "submodule.recurse")) {
		صحیح r = git_config_bool(k, v) ?
			RECURSE_SUBMODULES_ON : RECURSE_SUBMODULES_OFF;
		fetch_config->recurse_submodules = r;
		واپس 0;
	}

	اگر (!strcmp(k, "submodule.fetchjobs")) {
		fetch_config->submodule_fetch_jobs = parse_submodule_fetchjobs(k, v, ctx->kvi);
		واپس 0;
	} ورنہ اگر (!strcmp(k, "fetch.recursesubmodules")) {
		fetch_config->recurse_submodules = parse_fetch_recurse_submodules_arg(k, v);
		واپس 0;
	}

	اگر (!strcmp(k, "fetch.parallel")) {
		fetch_config->parallel = git_config_int(k, v, ctx->kvi);
		اگر (fetch_config->parallel < 0)
			die(_("fetch.parallel cannot be negative"));
		اگر (!fetch_config->parallel)
			fetch_config->parallel = online_cpus();
		واپس 0;
	}

	اگر (!strcmp(k, "fetch.output")) {
		اگر (!v)
			واپس config_error_nonbool(k);
		ورنہ اگر (!strcasecmp(v, "full"))
			fetch_config->display_format = DISPLAY_FORMAT_بھرا;
		ورنہ اگر (!strcasecmp(v, "compact"))
			fetch_config->display_format = DISPLAY_FORMAT_COMPACT;
		ورنہ
			die(_("invalid value for '%s': '%s'"),
			    "fetch.output", v);
		واپس 0;
	}

	اگر (!strcmp(k, "fetch.followremotehead")) {
		آزاد(fetch_config->follow_remote_سر_raw);
		fetch_config->follow_remote_سر_raw = xstrdup_or_null(v);
		fetch_config->follow_remote_سر_نظر_آیا = 1;
		واپس 0;
	}

	واپس git_default_config(k, v, ctx, cb);
}

ساکن شمار follow_remote_سر_settings get_follow_remote_سر(ثابت حرف *setting)
{
	اگر (!setting)
		die(_("missing value for 'fetch.followRemoteHEAD'"));
	ورنہ اگر (!strcmp(setting, "never"))
		واپس FOLLOW_REMOTE_NEVER;
	ورنہ اگر (!strcmp(setting, "create"))
		واپس FOLLOW_REMOTE_CREATE;
	ورنہ اگر (!strcmp(setting, "warn"))
		واپس FOLLOW_REMOTE_WARN;
	ورنہ اگر (!strcmp(setting, "always"))
		واپس FOLLOW_REMOTE_ALWAYS;
	warning(_("unrecognized fetch.followRemoteHEAD value '%s' ignored"), setting);
	واپس BUILTIN_FOLLOW_REMOTE_سر_DFLT;
}

ساکن صحیح parse_refmap_arg(ثابت ساخت option *opt, ثابت حرف *arg, صحیح unset)
{
	BUG_ON_OPT_NEG(unset);

	/*
	 * "git fetch --refmap='' origin foo"
	 * can be used to tell the command not to store anywhere
	 */
	refspec_append(opt->قدر, arg);

	واپس 0;
}

ساکن خالی unlock_pack(غیرعلامتی صحیح flags)
{
	اگر (gtransport)
		transport_unlock_pack(gtransport, flags);
	اگر (gsecondary)
		transport_unlock_pack(gsecondary, flags);
}

ساکن خالی unlock_pack_atexit(خالی)
{
	unlock_pack(0);
}

ساکن خالی unlock_pack_on_signal(صحیح signo)
{
	unlock_pack(TRANSPORT_UNLOCK_PACK_اندر_SIGNAL_HANDLER);
	sigchain_نکالو(signo);
	اٹھاؤ(signo);
}

ساکن خالی جوڑو_ملاؤ_config(ساخت ref **سر,
			   ثابت ساخت ref *remote_refs,
		           ساخت branch *branch,
		           ساخت ref ***دم)
{
	صحیح i;

	برائے (i = 0; i < branch->ملاؤ_nr; i++) {
		ساخت ref *rm, **old_دم = *دم;
		ساخت refspec_شے refspec;

		برائے (rm = *سر; rm; rm = rm->اگلا) {
			اگر (branch_ملاؤ_matches(branch, i, rm->name)) {
				rm->fetch_سر_حالت = FETCH_سر_ملاؤ;
				توڑو;
			}
		}
		اگر (rm)
			جاری;

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
		memset(&refspec, 0, حجم(refspec));
		refspec.src = branch->ملاؤ[i]->src;
		get_fetch_map(remote_refs, &refspec, دم, 1);
		برائے (rm = *old_دم; rm; rm = rm->اگلا)
			rm->fetch_سر_حالت = FETCH_سر_ملاؤ;
	}
}

ساکن خالی create_fetch_oidset(ساخت ref **سر, ساخت oidset *برآمد)
{
	ساخت ref *rm = *سر;
	جب (rm) {
		oidset_داخل_کرو(برآمد, &rm->old_oid);
		rm = rm->اگلا;
	}
}

ساخت refname_hash_اندراج {
	ساخت hashmap_اندراج ent;
	ساخت object_id oid;
	صحیح ignore;
	حرف refname[FLEX_ARRAY];
};

ساکن صحیح refname_hash_اندراج_cmp(ثابت خالی *hashmap_cmp_fn_ڈیٹا غیر_مستعمل,
				  ثابت ساخت hashmap_اندراج *eptr,
				  ثابت ساخت hashmap_اندراج *اندراج_or_کلید,
				  ثابت خالی *keydata)
{
	ثابت ساخت refname_hash_اندراج *e1, *e2;

	e1 = container_کا(eptr, ثابت ساخت refname_hash_اندراج, ent);
	e2 = container_کا(اندراج_or_کلید, ثابت ساخت refname_hash_اندراج, ent);
	واپس strcmp(e1->refname, keydata ? keydata : e2->refname);
}

ساکن ساخت refname_hash_اندراج *refname_hash_جوڑو(ساخت hashmap *map,
						   ثابت حرف *refname,
						   ثابت ساخت object_id *oid)
{
	ساخت refname_hash_اندراج *ent;
	سائز_t len = لڑی_لمبائی(refname);

	FLEX_ALLOC_MEM(ent, refname, refname, len);
	hashmap_اندراج_init(&ent->ent, strhash(refname));
	oidcpy(&ent->oid, oid);
	hashmap_جوڑو(map, &ent->ent);
	واپس ent;
}

ساکن صحیح جوڑو_one_refname(ثابت ساخت reference *ref, خالی *cbdata)
{
	ساخت hashmap *refname_map = cbdata;

	(خالی) refname_hash_جوڑو(refname_map, ref->name, ref->oid);
	واپس 0;
}

ساکن خالی refname_hash_init(ساخت hashmap *map)
{
	hashmap_init(map, refname_hash_اندراج_cmp, تہی, 0);
}

ساکن صحیح refname_hash_exists(ساخت hashmap *map, ثابت حرف *refname)
{
	واپس !!hashmap_get_from_hash(map, strhash(refname), refname);
}

ساکن خالی clear_شے(ساخت refname_hash_اندراج *شے)
{
	شے->ignore = 1;
}


ساکن خالی جوڑو_already_queued_لیبل_فہرست(ثابت حرف *refname,
				    ثابت ساخت object_id *old_oid غیر_مستعمل,
				    ثابت ساخت object_id *new_oid,
				    خالی *cb_ڈیٹا)
{
	ساخت hashmap *queued_لیبل_فہرست = cb_ڈیٹا;
	اگر (starts_with(refname, "refs/tags/") && new_oid)
		(خالی) refname_hash_جوڑو(queued_لیبل_فہرست, refname, new_oid);
}

ساکن خالی ڈھونڈو_non_local_لیبل_فہرست(ثابت ساخت ref *refs,
				ساخت ref_transaction *transaction,
				ساخت ref **سر,
				ساخت ref ***دم)
{
	ساخت hashmap existing_refs;
	ساخت hashmap remote_refs;
	ساخت oidset fetch_oids = OIDSET_INIT;
	ساخت string_list remote_refs_list = STRING_LIST_INIT_NODUP;
	ساخت string_list_شے *remote_ref_شے;
	ثابت ساخت ref *ref;
	ساخت refname_hash_اندراج *شے = تہی;

	refname_hash_init(&existing_refs);
	refname_hash_init(&remote_refs);
	create_fetch_oidset(سر, &fetch_oids);

	refs_for_each_ref(get_main_ref_store(the_repository), جوڑو_one_refname,
			  &existing_refs);

	/*
	 * If we already have a transaction, then we need to filter out all
	 * tags which have already been queued up.
	 */
	اگر (transaction)
		ref_transaction_for_each_queued_update(transaction,
						       جوڑو_already_queued_لیبل_فہرست,
						       &existing_refs);

	برائے (ref = refs; ref; ref = ref->اگلا) {
		اگر (!starts_with(ref->name, "refs/tags/"))
			جاری;

		/*
		 * The peeled ref always follows the matching base
		 * ref, so if we see a peeled ref that we don't want
		 * to fetch then we can mark the ref entry in the list
		 * as one to ignore by setting util to NULL.
		 */
		اگر (سرے_with(ref->name, "^{}")) {
			اگر (شے &&
			    !odb_has_object(the_repository->objects, &ref->old_oid, 0) &&
			    !oidset_شامل_ہے(&fetch_oids, &ref->old_oid) &&
			    !odb_has_object(the_repository->objects, &شے->oid, 0) &&
			    !oidset_شامل_ہے(&fetch_oids, &شے->oid))
				clear_شے(شے);
			شے = تہی;
			جاری;
		}

		/*
		 * If item is non-NULL here, then we previously saw a
		 * ref not followed by a peeled reference, so we need
		 * to check if it is a lightweight tag that we want to
		 * fetch.
		 */
		اگر (شے &&
		    !odb_has_object(the_repository->objects, &شے->oid, 0) &&
		    !oidset_شامل_ہے(&fetch_oids, &شے->oid))
			clear_شے(شے);

		شے = تہی;

		/* skip duplicates and refs that we already have */
		اگر (refname_hash_exists(&remote_refs, ref->name) ||
		    refname_hash_exists(&existing_refs, ref->name))
			جاری;

		شے = refname_hash_جوڑو(&remote_refs, ref->name, &ref->old_oid);
		string_list_داخل_کرو(&remote_refs_list, ref->name);
	}
	hashmap_clear_and_free(&existing_refs, ساخت refname_hash_اندراج, ent);

	/*
	 * We may have a final lightweight tag that needs to be
	 * checked to see if it needs fetching.
	 */
	اگر (شے &&
	    !odb_has_object(the_repository->objects, &شے->oid, 0) &&
	    !oidset_شامل_ہے(&fetch_oids, &شے->oid))
		clear_شے(شے);

	/*
	 * For all the tags in the remote_refs_list,
	 * add them to the list of refs to be fetched
	 */
	for_each_string_list_شے(remote_ref_شے, &remote_refs_list) {
		ثابت حرف *refname = remote_ref_شے->string;
		ساخت ref *rm;
		غیرعلامتی صحیح hash = strhash(refname);

		شے = hashmap_get_اندراج_from_hash(&remote_refs, hash, refname,
					ساخت refname_hash_اندراج, ent);
		اگر (!شے)
			BUG("unseen remote ref?");

		/* Unless we have already decided to ignore this item... */
		اگر (شے->ignore)
			جاری;

		rm = alloc_ref(شے->refname);
		rm->peer_ref = alloc_ref(شے->refname);
		oidcpy(&rm->old_oid, &شے->oid);
		**دم = rm;
		*دم = &rm->اگلا;
	}
	hashmap_clear_and_free(&remote_refs, ساخت refname_hash_اندراج, ent);
	string_list_clear(&remote_refs_list, 0);
	oidset_clear(&fetch_oids);
}

ساکن خالی filter_prefetch_refspec(ساخت refspec *rs)
{
	صحیح i;

	اگر (!prefetch)
		واپس;

	برائے (i = 0; i < rs->nr; i++) {
		ساخت strbuf new_dst = STRBUF_INIT;
		حرف *old_dst;
		ثابت حرف *sub = تہی;

		اگر (rs->اشیاء[i].negative)
			جاری;
		اگر (!rs->اشیاء[i].dst ||
		    (rs->اشیاء[i].src &&
		     starts_with(rs->اشیاء[i].src,
				 ref_namespace[NAMESPACE_لیبل_فہرست].ref))) {
			صحیح j;

			refspec_شے_clear(&rs->اشیاء[i]);

			برائے (j = i + 1; j < rs->nr; j++)
				rs->اشیاء[j - 1] = rs->اشیاء[j];
			rs->nr--;
			i--;
			جاری;
		}

		old_dst = rs->اشیاء[i].dst;
		strbuf_addstr(&new_dst, ref_namespace[NAMESPACE_PREFETCH].ref);

		/*
		 * If old_dst starts with "refs/", then place
		 * sub after that prefix. Otherwise, start at
		 * the beginning of the string.
		 */
		اگر (!skip_سابقہ(old_dst, "refs/", &sub))
			sub = old_dst;
		strbuf_addstr(&new_dst, sub);

		rs->اشیاء[i].dst = strbuf_detach(&new_dst, تہی);
		rs->اشیاء[i].force = 1;

		آزاد(old_dst);
	}
}

ساکن ساخت ref *get_ref_map(ساخت remote *remote,
			       ثابت ساخت ref *remote_refs,
			       ساخت refspec *rs,
			       صحیح لیبل_فہرست, صحیح *autotags)
{
	صحیح i;
	ساخت ref *rm;
	ساخت ref *ref_map = تہی;
	ساخت ref **دم = &ref_map;

	/* opportunistically-updated references: */
	ساخت ref *orefs = تہی, **oref_دم = &orefs;

	ساخت hashmap existing_refs;
	صحیح existing_refs_populated = 0;

	filter_prefetch_refspec(rs);
	اگر (remote)
		filter_prefetch_refspec(&remote->fetch);

	اگر (rs->nr) {
		ساخت refspec *fetch_refspec;

		برائے (i = 0; i < rs->nr; i++) {
			get_fetch_map(remote_refs, &rs->اشیاء[i], &دم, 0);
			اگر (rs->اشیاء[i].dst && rs->اشیاء[i].dst[0])
				*autotags = 1;
		}
		/* Merge everything on the command line (but not --tags) */
		برائے (rm = ref_map; rm; rm = rm->اگلا)
			rm->fetch_سر_حالت = FETCH_سر_ملاؤ;

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
		اگر (refmap.nr)
			fetch_refspec = &refmap;
		ورنہ
			fetch_refspec = &remote->fetch;

		برائے (i = 0; i < fetch_refspec->nr; i++)
			get_fetch_map(ref_map, &fetch_refspec->اشیاء[i], &oref_دم, 1);
	} ورنہ اگر (refmap.nr) {
		die("--refmap option is only meaningful with command-line refspec(s)");
	} ورنہ {
		/* Use the defaults */
		ساخت branch *branch = branch_get(تہی);
		صحیح has_ملاؤ = branch_has_ملاؤ_config(branch);
		اگر (remote &&
		    (remote->fetch.nr ||
		     /* Note: has_merge implies non-NULL branch->remote_name */
		     (has_ملاؤ && !strcmp(branch->remote_name, remote->name)))) {
			برائے (i = 0; i < remote->fetch.nr; i++) {
				get_fetch_map(remote_refs, &remote->fetch.اشیاء[i], &دم, 0);
				اگر (remote->fetch.اشیاء[i].dst &&
				    remote->fetch.اشیاء[i].dst[0])
					*autotags = 1;
				اگر (!i && !has_ملاؤ && ref_map &&
				    !remote->fetch.اشیاء[0].نمونہ)
					ref_map->fetch_سر_حالت = FETCH_سر_ملاؤ;
			}
			/*
			 * if the remote we're fetching from is the same
			 * as given in branch.<name>.remote, we add the
			 * ref given in branch.<name>.merge, too.
			 *
			 * Note: has_merge implies non-NULL branch->remote_name
			 */
			اگر (has_ملاؤ &&
			    !strcmp(branch->remote_name, remote->name))
				جوڑو_ملاؤ_config(&ref_map, remote_refs, branch, &دم);
		} ورنہ اگر (!prefetch) {
			ref_map = get_remote_ref(remote_refs, "HEAD");
			اگر (!ref_map)
				die(_("couldn't find remote ref HEAD"));
			ref_map->fetch_سر_حالت = FETCH_سر_ملاؤ;
			دم = &ref_map->اگلا;
		}
	}

	اگر (لیبل_فہرست == لیبل_فہرست_سیٹ) {
		ساخت refspec_شے لیبل_refspec;

		/* also fetch all tags */
		refspec_شے_init_ڈالو(&لیبل_refspec, لیبل_REFSPEC,
				       the_hash_algo);
		get_fetch_map(remote_refs, &لیبل_refspec, &دم, 0);
		refspec_شے_clear(&لیبل_refspec);
	} ورنہ اگر (لیبل_فہرست == لیبل_فہرست_DEFAULT && *autotags) {
		ڈھونڈو_non_local_لیبل_فہرست(remote_refs, تہی, &ref_map, &دم);
	}

	/* Now append any refs to be updated opportunistically: */
	*دم = orefs;
	برائے (rm = orefs; rm; rm = rm->اگلا) {
		rm->fetch_سر_حالت = FETCH_سر_IGNORE;
		دم = &rm->اگلا;
	}

	/*
	 * apply negative refspecs first, before we remove duplicates. This is
	 * necessary as negative refspecs might remove an otherwise conflicting
	 * duplicate.
	 */
	اگر (rs->nr)
		ref_map = apply_negative_refspecs(ref_map, rs);
	ورنہ
		ref_map = apply_negative_refspecs(ref_map, &remote->fetch);

	ref_map = ref_remove_duplicates(ref_map);

	برائے (rm = ref_map; rm; rm = rm->اگلا) {
		اگر (rm->peer_ref) {
			ثابت حرف *refname = rm->peer_ref->name;
			ساخت refname_hash_اندراج *peer_شے;
			غیرعلامتی صحیح hash = strhash(refname);

			اگر (!existing_refs_populated) {
				refname_hash_init(&existing_refs);
				refs_for_each_ref(get_main_ref_store(the_repository),
						  جوڑو_one_refname,
						  &existing_refs);
				existing_refs_populated = 1;
			}

			peer_شے = hashmap_get_اندراج_from_hash(&existing_refs,
						hash, refname,
						ساخت refname_hash_اندراج, ent);
			اگر (peer_شے) {
				ساخت object_id *old_oid = &peer_شے->oid;
				oidcpy(&rm->peer_ref->old_oid, old_oid);
			}
		}
	}
	اگر (existing_refs_populated)
		hashmap_clear_and_free(&existing_refs, ساخت refname_hash_اندراج, ent);

	واپس ref_map;
}

ساکن صحیح s_update_ref(ثابت حرف *action,
			ساخت ref *ref,
			ساخت ref_transaction *transaction,
			صحیح check_old)
{
	حرف *پیغام;
	حرف *rla = getenv("GIT_REFLOG_ACTION");
	ساخت strbuf err = STRBUF_INIT;
	صحیح ret;

	اگر (dry_تسلسل)
		واپس 0;
	اگر (!rla)
		rla = default_rla.buf;
	پیغام = xstrfmt("%s: %s", rla, action);

	ret = ref_transaction_update(transaction, ref->name, &ref->new_oid,
				     check_old ? &ref->old_oid : تہی,
				     تہی, تہی, 0, پیغام, &err);

	اگر (ret)
		error("%s", err.buf);
	strbuf_release(&err);
	آزاد(پیغام);
	واپس ret;
}

ساکن صحیح refcol_چوڑائی(ثابت ساخت ref *ref_map, صحیح compact_format)
{
	ثابت ساخت ref *ref;
	صحیح max, چوڑائی = 10;

	max = رکن_columns();
	اگر (compact_format)
		max = max * 2 / 3;

	برائے (ref = ref_map; ref; ref = ref->اگلا) {
		صحیح rlen, llen = 0, len;

		اگر (ref->حالت_2 == REF_حالت_REJECT_SHALLOW ||
		    !ref->peer_ref ||
		    !strcmp(ref->name, "HEAD"))
			جاری;

		/* uptodate lines are only shown on high verbosity level */
		اگر (verbosity <= 0 && oideq(&ref->peer_ref->old_oid, &ref->old_oid))
			جاری;

		rlen = utf8_strwidth(prettify_refname(ref->name));
		اگر (!compact_format)
			llen = utf8_strwidth(prettify_refname(ref->peer_ref->name));

		/*
		 * rough estimation to see if the output line is too long and
		 * should not be counted (we can't do precise calculation
		 * anyway because we don't know if the error explanation part
		 * will be printed in update_local_ref)
		 */
		len = 21 /* flag and summary */ + rlen + 4 /* -> */ + llen;
		اگر (len >= max)
			جاری;

		اگر (چوڑائی < rlen)
			چوڑائی = rlen;
	}

	واپس چوڑائی;
}

ساکن خالی display_state_init(ساخت display_state *display_state, ساخت ref *ref_map,
			       ثابت حرف *raw_url, شمار display_format format)
{
	صحیح i;

	memset(display_state, 0, حجم(*display_state));
	strbuf_init(&display_state->buf, 0);
	display_state->format = format;

	اگر (raw_url)
		display_state->url = transport_anonymize_url(raw_url);
	ورنہ
		display_state->url = xstrdup("foreign");

	display_state->url_len = لڑی_لمبائی(display_state->url);
	برائے (i = display_state->url_len - 1; 0 <= i && display_state->url[i] == '/'; i--)
		;
	display_state->url_len = i + 1;
	اگر (4 < i && !لڑی_موازنہ(".git", display_state->url + i - 3, 4))
		display_state->url_len = i - 3;

	اگر (verbosity < 0)
		واپس;

	انتخاب (display_state->format) {
	حالت DISPLAY_FORMAT_بھرا:
	حالت DISPLAY_FORMAT_COMPACT:
		display_state->refcol_چوڑائی = refcol_چوڑائی(ref_map,
							   display_state->format == DISPLAY_FORMAT_COMPACT);
		توڑو;
	حالت DISPLAY_FORMAT_PORCELAIN:
		/* We don't need to precompute anything here. */
		توڑو;
	طےشدہ:
		BUG("unexpected display format %d", display_state->format);
	}
}

ساکن خالی display_state_release(ساخت display_state *display_state)
{
	strbuf_release(&display_state->buf);
	آزاد(display_state->url);
}

ساکن خالی print_remote_مقصد_local(ساخت display_state *display_state,
				  ثابت حرف *remote, ثابت حرف *local)
{
	strbuf_addf(&display_state->buf, "%-*s -> %s",
		    display_state->refcol_چوڑائی, remote, local);
}

ساکن صحیح ڈھونڈو_and_replace(ساخت strbuf *haystack,
			    ثابت حرف *needle,
			    ثابت حرف *placeholder)
{
	ثابت حرف *p = تہی;
	صحیح plen, nlen;

	nlen = لڑی_لمبائی(needle);
	اگر (سرے_with(haystack->buf, needle))
		p = haystack->buf + haystack->len - nlen;
	ورنہ
		p = لڑی_تلاش(haystack->buf, needle);
	اگر (!p)
		واپس 0;

	اگر (p > haystack->buf && p[-1] != '/')
		واپس 0;

	plen = لڑی_لمبائی(p);
	اگر (plen > nlen && p[nlen] != '/')
		واپس 0;

	strbuf_splice(haystack, p - haystack->buf, nlen,
		      placeholder, لڑی_لمبائی(placeholder));
	واپس 1;
}

ساکن خالی print_compact(ساخت display_state *display_state,
			  ثابت حرف *remote, ثابت حرف *local)
{
	ساخت strbuf r = STRBUF_INIT;
	ساخت strbuf l = STRBUF_INIT;

	اگر (!strcmp(remote, local)) {
		strbuf_addf(&display_state->buf, "%-*s -> *", display_state->refcol_چوڑائی, remote);
		واپس;
	}

	strbuf_addstr(&r, remote);
	strbuf_addstr(&l, local);

	اگر (!ڈھونڈو_and_replace(&r, local, "*"))
		ڈھونڈو_and_replace(&l, remote, "*");
	print_remote_مقصد_local(display_state, r.buf, l.buf);

	strbuf_release(&r);
	strbuf_release(&l);
}

ساکن خالی display_ref_update(ساخت display_state *display_state, حرف رمز,
			       ثابت حرف *summary, ثابت حرف *error,
			       ثابت حرف *remote, ثابت حرف *local,
			       ثابت ساخت object_id *old_oid,
			       ثابت ساخت object_id *new_oid,
			       صحیح summary_چوڑائی)
{
	FILE *f = stderr;

	اگر (verbosity < 0)
		واپس;

	strbuf_reset(&display_state->buf);

	انتخاب (display_state->format) {
	حالت DISPLAY_FORMAT_بھرا:
	حالت DISPLAY_FORMAT_COMPACT: {
		صحیح چوڑائی;

		اگر (!display_state->shown_url) {
			strbuf_addf(&display_state->buf, _("From %.*s\n"),
				    display_state->url_len, display_state->url);
			display_state->shown_url = 1;
		}

		چوڑائی = (summary_چوڑائی + لڑی_لمبائی(summary) - gettext_چوڑائی(summary));
		remote = prettify_refname(remote);
		local = prettify_refname(local);

		strbuf_addf(&display_state->buf, " %c %-*s ", رمز, چوڑائی, summary);

		اگر (display_state->format != DISPLAY_FORMAT_COMPACT)
			print_remote_مقصد_local(display_state, remote, local);
		ورنہ
			print_compact(display_state, remote, local);

		اگر (error)
			strbuf_addf(&display_state->buf, "  (%s)", error);

		توڑو;
	}
	حالت DISPLAY_FORMAT_PORCELAIN:
		strbuf_addf(&display_state->buf, "%c %s %s %s", رمز,
			    oid_مقصد_hex(old_oid), oid_مقصد_hex(new_oid), local);
		f = stdout;
		توڑو;
	طےشدہ:
		BUG("unexpected display format %d", display_state->format);
	};
	strbuf_addch(&display_state->buf, '\n');

	fputs(display_state->buf.buf, f);
}

ساخت ref_update_display_info {
	bool failed;
	حرف success_رمز;
	حرف fail_رمز;
	حرف *summary;
	حرف *fail_detail;
	حرف *success_detail;
	حرف *ref;
	حرف *remote;
	ساخت object_id old_oid;
	ساخت object_id new_oid;
};

ساخت ref_update_display_info_array {
	ساخت ref_update_display_info *info;
	سائز_t alloc, nr;
};

ساکن ساخت ref_update_display_info *ref_update_display_info_append(
					   ساخت ref_update_display_info_array *array,
					   حرف success_رمز,
					   حرف fail_رمز,
					   ثابت حرف *summary,
					   ثابت حرف *success_detail,
					   ثابت حرف *fail_detail,
					   ثابت حرف *ref,
					   ثابت حرف *remote,
					   ثابت ساخت object_id *old_oid,
					   ثابت ساخت object_id *new_oid)
{
	ساخت ref_update_display_info *info;

	ALLOC_GROW(array->info, array->nr + 1, array->alloc);
	info = &array->info[array->nr++];

	info->failed = false;
	info->success_رمز = success_رمز;
	info->fail_رمز = fail_رمز;
	info->summary = xstrdup(summary);
	info->success_detail = xstrdup_or_null(success_detail);
	info->fail_detail = xstrdup_or_null(fail_detail);
	info->remote = xstrdup(remote);
	info->ref = xstrdup(ref);

	oidcpy(&info->old_oid, old_oid);
	oidcpy(&info->new_oid, new_oid);

	واپس info;
}

ساکن خالی ref_update_display_info_سیٹ_failed(ساخت ref_update_display_info *info)
{
	info->failed = true;
}

ساکن خالی ref_update_display_info_free(ساخت ref_update_display_info *info)
{
	آزاد(info->summary);
	آزاد(info->success_detail);
	آزاد(info->fail_detail);
	آزاد(info->remote);
	آزاد(info->ref);
}

ساکن خالی ref_update_display_info_display(ساخت ref_update_display_info *info,
					    ساخت display_state *display_state,
					    صحیح summary_چوڑائی)
{
	display_ref_update(display_state,
			   info->failed ? info->fail_رمز : info->success_رمز,
			   info->summary,
			   info->failed ? info->fail_detail : info->success_detail,
			   info->remote, info->ref, &info->old_oid,
			   &info->new_oid, summary_چوڑائی);
}

ساکن صحیح update_local_ref(ساخت ref *ref,
			    ساخت ref_transaction *transaction,
			    ثابت ساخت ref *remote_ref,
			    ثابت ساخت fetch_config *config,
			    ساخت ref_update_display_info_array *display_array)
{
	ساخت commit *current = تہی, *updated;
	صحیح fast_forward = 0;

	اگر (!odb_has_object(the_repository->objects, &ref->new_oid,
			    ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR))
		die(_("object %s not found"), oid_مقصد_hex(&ref->new_oid));

	اگر (oideq(&ref->old_oid, &ref->new_oid)) {
		اگر (verbosity > 0)
			ref_update_display_info_append(display_array, '=', '=',
						       _("[up to date]"), تہی,
						       تہی, ref->name,
						       remote_ref->name, &ref->old_oid,
						       &ref->new_oid);
		واپس 0;
	}

	اگر (!update_سر_ok &&
	    !is_null_oid(&ref->old_oid) &&
	    branch_checked_برآمد(ref->name)) {
		ساخت ref_update_display_info *info;
		/*
		 * If this is the head, and it's not okay to update
		 * the head, and the old value of the head isn't empty...
		 */
		info = ref_update_display_info_append(display_array, '!', '!',
						      _("[rejected]"), تہی,
						      _("can't fetch into checked-out branch"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);
		ref_update_display_info_سیٹ_failed(info);
		واپس 1;
	}

	اگر (!is_null_oid(&ref->old_oid) &&
	    starts_with(ref->name, "refs/tags/")) {
		ساخت ref_update_display_info *info;

		اگر (force || ref->force) {
			صحیح r;

			r = s_update_ref("updating tag", ref, transaction, 0);

			info = ref_update_display_info_append(display_array, 't', '!',
							      _("[tag update]"), تہی,
							      _("unable to update local ref"),
							      ref->name, remote_ref->name,
							      &ref->old_oid, &ref->new_oid);
			اگر (r)
				ref_update_display_info_سیٹ_failed(info);

			واپس r;
		} ورنہ {
			info = ref_update_display_info_append(display_array, '!', '!',
							      _("[rejected]"), تہی,
							      _("would clobber existing tag"),
							      ref->name, remote_ref->name,
							      &ref->old_oid, &ref->new_oid);
			ref_update_display_info_سیٹ_failed(info);
			واپس 1;
		}
	}

	current = lookup_commit_reference_gently(the_repository,
						 &ref->old_oid, 1);
	updated = lookup_commit_reference_gently(the_repository,
						 &ref->new_oid, 1);
	اگر (!current || !updated) {
		ساخت ref_update_display_info *info;
		ثابت حرف *پیغام;
		ثابت حرف *what;
		صحیح r;
		/*
		 * Nicely describe the new ref we're fetching.
		 * Base this on the remote's ref name, as it's
		 * more likely to follow a standard layout.
		 */
		اگر (starts_with(remote_ref->name, "refs/tags/")) {
			پیغام = "storing tag";
			what = _("[new tag]");
		} ورنہ اگر (starts_with(remote_ref->name, "refs/heads/")) {
			پیغام = "storing head";
			what = _("[new branch]");
		} ورنہ {
			پیغام = "storing ref";
			what = _("[new ref]");
		}

		r = s_update_ref(پیغام, ref, transaction, 0);

		info = ref_update_display_info_append(display_array, '*', '!',
						      what, تہی,
						      _("unable to update local ref"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);
		اگر (r)
			ref_update_display_info_سیٹ_failed(info);

		واپس r;
	}

	اگر (config->show_forced_updates) {
		uint64_t t_before = getnanotime();
		fast_forward = repo_اندر_ملاؤ_bases(the_repository, current,
						   updated);
		اگر (fast_forward < 0)
			die(تہی);
		forced_updates_ms += (getnanotime() - t_before) / 1000000;
	} ورنہ {
		fast_forward = 1;
	}

	اگر (fast_forward) {
		ساخت ref_update_display_info *info;
		ساخت strbuf quickref = STRBUF_INIT;
		صحیح r;

		strbuf_جوڑو_unique_abbrev(&quickref, &current->object.oid, DEFAULT_ABBREV);
		strbuf_addstr(&quickref, "..");
		strbuf_جوڑو_unique_abbrev(&quickref, &ref->new_oid, DEFAULT_ABBREV);
		r = s_update_ref("fast-forward", ref, transaction, 1);

		info = ref_update_display_info_append(display_array, ' ', '!',
						      quickref.buf, تہی,
						      _("unable to update local ref"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);
		اگر (r)
			ref_update_display_info_سیٹ_failed(info);

		strbuf_release(&quickref);
		واپس r;
	} ورنہ اگر (force || ref->force) {
		ساخت ref_update_display_info *info;
		ساخت strbuf quickref = STRBUF_INIT;
		صحیح r;

		strbuf_جوڑو_unique_abbrev(&quickref, &current->object.oid, DEFAULT_ABBREV);
		strbuf_addstr(&quickref, "...");
		strbuf_جوڑو_unique_abbrev(&quickref, &ref->new_oid, DEFAULT_ABBREV);
		r = s_update_ref("forced-update", ref, transaction, 1);

		info = ref_update_display_info_append(display_array, '+', '!',
						      quickref.buf, _("forced update"),
						      _("unable to update local ref"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);

		اگر (r)
			ref_update_display_info_سیٹ_failed(info);

		strbuf_release(&quickref);
		واپس r;
	} ورنہ {
		ساخت ref_update_display_info *info;
		info = ref_update_display_info_append(display_array, '!', '!',
						      _("[rejected]"), تہی,
						      _("non-fast-forward"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);
		ref_update_display_info_سیٹ_failed(info);
		واپس 1;
	}
}

ساکن ثابت ساخت object_id *iterate_ref_map(خالی *cb_ڈیٹا)
{
	ساخت ref **rm = cb_ڈیٹا;
	ساخت ref *ref = *rm;

	جب (ref && ref->حالت_2 == REF_حالت_REJECT_SHALLOW)
		ref = ref->اگلا;
	اگر (!ref)
		واپس تہی;
	*rm = ref->اگلا;
	واپس &ref->old_oid;
}

ساخت fetch_سر {
	FILE *fp;
	ساخت strbuf buf;
};

ساکن صحیح open_fetch_سر(ساخت fetch_سر *fetch_سر)
{
	ثابت حرف *filename = git_راستہ_fetch_سر(the_repository);

	اگر (write_fetch_سر) {
		fetch_سر->fp = fopen(filename, "a");
		اگر (!fetch_سر->fp)
			واپس error_errno(_("cannot open '%s'"), filename);
		strbuf_init(&fetch_سر->buf, 0);
	} ورنہ {
		fetch_سر->fp = تہی;
	}

	واپس 0;
}

ساکن خالی append_fetch_سر(ساخت fetch_سر *fetch_سر,
			      ثابت ساخت object_id *old_oid,
			      شمار fetch_سر_حالت fetch_سر_حالت,
			      ثابت حرف *note,
			      ثابت حرف *url, سائز_t url_len)
{
	حرف old_oid_hex[GIT_MAX_HEXSZ + 1];
	ثابت حرف *ملاؤ_حالت_marker;
	سائز_t i;

	اگر (!fetch_سر->fp)
		واپس;

	انتخاب (fetch_سر_حالت) {
	حالت FETCH_سر_NOT_FOR_ملاؤ:
		ملاؤ_حالت_marker = "not-for-merge";
		توڑو;
	حالت FETCH_سر_ملاؤ:
		ملاؤ_حالت_marker = "";
		توڑو;
	طےشدہ:
		/* do not write anything to FETCH_HEAD */
		واپس;
	}

	strbuf_addf(&fetch_سر->buf, "%s\t%s\t%s",
		    oid_مقصد_hex_r(old_oid_hex, old_oid), ملاؤ_حالت_marker, note);
	برائے (i = 0; i < url_len; ++i)
		اگر ('\n' == url[i])
			strbuf_addstr(&fetch_سر->buf, "\\n");
		ورنہ
			strbuf_addch(&fetch_سر->buf, url[i]);
	strbuf_addch(&fetch_سر->buf, '\n');

	/*
	 * When using an atomic fetch, we do not want to update FETCH_HEAD if
	 * any of the reference updates fails. We thus have to write all
	 * updates to a buffer first and only commit it as soon as all
	 * references have been successfully updated.
	 */
	اگر (!atomic_fetch) {
		strbuf_write(&fetch_سر->buf, fetch_سر->fp);
		strbuf_reset(&fetch_سر->buf);
	}
}

ساکن خالی commit_fetch_سر(ساخت fetch_سر *fetch_سر)
{
	اگر (!fetch_سر->fp || !atomic_fetch)
		واپس;
	strbuf_write(&fetch_سر->buf, fetch_سر->fp);
}

ساکن خالی close_fetch_سر(ساخت fetch_سر *fetch_سر)
{
	اگر (!fetch_سر->fp)
		واپس;

	fclose(fetch_سر->fp);
	strbuf_release(&fetch_سر->buf);
}

ساکن ثابت حرف warn_show_forced_updates[] =
N_("fetch normally indicates which branches had a forced update,\n"
   "but that check has been disabled; to re-enable, use '--show-forced-updates'\n"
   "flag or run 'git config fetch.showForcedUpdates true'");
ساکن ثابت حرف warn_وقت_show_forced_updates[] =
N_("it took %.2f seconds to check forced updates; you can use\n"
   "'--no-show-forced-updates' or run 'git config fetch.showForcedUpdates false'\n"
   "to avoid this check\n");

ساکن صحیح store_updated_refs(ساخت display_state *display_state,
			      صحیح connectivity_checked,
			      ساخت ref_transaction *transaction, ساخت ref *ref_map,
			      ساخت fetch_سر *fetch_سر,
			      ثابت ساخت fetch_config *config,
			      ساخت ref_update_display_info_array *display_array)
{
	صحیح rc = 0;
	ساخت strbuf note = STRBUF_INIT;
	ثابت حرف *what, *قسم;
	ساخت ref *rm;
	صحیح want_حالت;

	اگر (!connectivity_checked) {
		ساخت check_connected_options opt = CHECK_CONNECTED_INIT;

		opt.exclude_hidden_refs_section = "fetch";
		rm = ref_map;
		اگر (check_connected(iterate_ref_map, &rm, &opt)) {
			rc = error(_("%s did not send all necessary objects"),
				   display_state->url);
			جاؤ abort;
		}
	}

	/*
	 * We do a pass for each fetch_head_status type in their enum order, so
	 * merged entries are written before not-for-merge. That lets readers
	 * use FETCH_HEAD as a refname to refer to the ref to be merged.
	 */
	برائے (want_حالت = FETCH_سر_ملاؤ;
	     want_حالت <= FETCH_سر_IGNORE;
	     want_حالت++) {
		برائے (rm = ref_map; rm; rm = rm->اگلا) {
			ساخت ref *ref = تہی;

			اگر (rm->حالت_2 == REF_حالت_REJECT_SHALLOW) {
				اگر (want_حالت == FETCH_سر_ملاؤ)
					warning(_("rejected %s because shallow roots are not allowed to be updated"),
						rm->peer_ref ? rm->peer_ref->name : rm->name);
				جاری;
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
			اگر (fetch_سر->fp) {
				ساخت commit *commit = تہی;

				/*
				 * References in "refs/tags/" are often going to point
				 * to annotated tags, which are not part of the
				 * commit-graph. We thus only try to look up refs in
				 * the graph which are not in that namespace to not
				 * regress performance in repositories with many
				 * annotated tags.
				 */
				اگر (!starts_with(rm->name, "refs/tags/"))
					commit = lookup_commit_اندر_گراف(the_repository, &rm->old_oid);
				اگر (!commit) {
					commit = lookup_commit_reference_gently(the_repository,
										&rm->old_oid,
										1);
					اگر (!commit)
						rm->fetch_سر_حالت = FETCH_سر_NOT_FOR_ملاؤ;
				}
			}

			اگر (rm->fetch_سر_حالت != want_حالت)
				جاری;

			اگر (rm->peer_ref) {
				ref = alloc_ref(rm->peer_ref->name);
				oidcpy(&ref->old_oid, &rm->peer_ref->old_oid);
				oidcpy(&ref->new_oid, &rm->old_oid);
				ref->force = rm->peer_ref->force;
			}

			اگر (config->recurse_submodules != RECURSE_SUBMODULES_OFF &&
			    (!rm->peer_ref || !oideq(&ref->old_oid, &ref->new_oid))) {
				check_for_new_submodule_commits(&rm->old_oid);
			}

			اگر (!strcmp(rm->name, "HEAD")) {
				قسم = "";
				what = "";
			} ورنہ اگر (skip_سابقہ(rm->name, "refs/heads/", &what)) {
				قسم = "branch";
			} ورنہ اگر (skip_سابقہ(rm->name, "refs/tags/", &what)) {
				قسم = "tag";
			} ورنہ اگر (skip_سابقہ(rm->name, "refs/remotes/", &what)) {
				قسم = "remote-tracking branch";
			} ورنہ {
				قسم = "";
				what = rm->name;
			}

			strbuf_reset(&note);
			اگر (*what) {
				اگر (*قسم)
					strbuf_addf(&note, "%s ", قسم);
				strbuf_addf(&note, "'%s' of ", what);
			}

			append_fetch_سر(fetch_سر, &rm->old_oid,
					  rm->fetch_سر_حالت,
					  note.buf, display_state->url,
					  display_state->url_len);

			اگر (ref) {
				rc |= update_local_ref(ref, transaction, rm,
						       config, display_array);
				آزاد(ref);
			} ورنہ اگر (write_fetch_سر || dry_تسلسل) {
				/*
				 * Display fetches written to FETCH_HEAD (or
				 * would be written to FETCH_HEAD, if --dry-run
				 * is set).
				 */

				ref_update_display_info_append(display_array, '*', '*',
							       *قسم ? قسم : "branch",
							       تہی, تہی, "FETCH_HEAD",
							       rm->name, &rm->new_oid,
							       &rm->old_oid);
			}
		}
	}

	اگر (advice_enabled(ADVICE_FETCH_SHOW_FORCED_UPDATES)) {
		اگر (!config->show_forced_updates) {
			warning(_(warn_show_forced_updates));
		} ورنہ اگر (forced_updates_ms > FORCED_UPDATES_DELAY_WARNING_اندر_MS) {
			warning(_(warn_وقت_show_forced_updates),
				forced_updates_ms / 1000.0);
		}
	}

 abort:
	strbuf_release(&note);
	واپس rc;
}

/*
 * We would want to bypass the object transfer altogether if
 * everything we are going to fetch already exists and is connected
 * locally.
 */
ساکن صحیح check_exist_and_connected(ساخت ref *ref_map)
{
	ساخت ref *rm = ref_map;
	ساخت check_connected_options opt = CHECK_CONNECTED_INIT;
	ساخت ref *r;

	/*
	 * If we are deepening a shallow clone we already have these
	 * objects reachable.  Running rev-list here will return with
	 * a good (0) exit status and we'll bypass the fetch that we
	 * really need to perform.  Claiming failure now will ensure
	 * we perform the network exchange to deepen our history.
	 */
	اگر (deepen)
		واپس -1;

	/*
	 * Similarly, if we need to refetch, we always want to perform a full
	 * fetch ignoring existing objects.
	 */
	اگر (refetch)
		واپس -1;


	/*
	 * check_connected() allows objects to merely be promised, but
	 * we need all direct targets to exist.
	 */
	برائے (r = rm; r; r = r->اگلا) {
		اگر (!odb_has_object(the_repository->objects, &r->old_oid,
				    ODB_HAS_OBJECT_RECHECK_PACKED))
			واپس -1;
	}

	opt.quiet = 1;
	opt.exclude_hidden_refs_section = "fetch";
	واپس check_connected(iterate_ref_map, &rm, &opt);
}

ساکن صحیح fetch_and_consume_refs(ساخت display_state *display_state,
				  ساخت transport *transport,
				  ساخت ref_transaction *transaction,
				  ساخت ref *ref_map,
				  ساخت fetch_سر *fetch_سر,
				  ثابت ساخت fetch_config *config,
				  ساخت ref_update_display_info_array *display_array)
{
	صحیح connectivity_checked = 1;
	صحیح ret;

	/*
	 * We don't need to perform a fetch in case we can already satisfy all
	 * refs.
	 */
	ret = check_exist_and_connected(ref_map);
	اگر (ret) {
		trace2_region_enter("fetch", "fetch_refs", the_repository);
		ret = transport_fetch_refs(transport, ref_map);
		trace2_region_leave("fetch", "fetch_refs", the_repository);
		اگر (ret)
			جاؤ برآمد;
		connectivity_checked = transport->smart_options ?
			transport->smart_options->connectivity_checked : 0;
	}

	trace2_region_enter("fetch", "consume_refs", the_repository);
	ret = store_updated_refs(display_state, connectivity_checked,
				 transaction, ref_map, fetch_سر, config,
				 display_array);
	trace2_region_leave("fetch", "consume_refs", the_repository);

برآمد:
	transport_unlock_pack(transport, 0);
	واپس ret;
}

ساکن صحیح prune_refs(ساخت display_state *display_state,
		      ساخت refspec *rs,
		      ساخت ref_transaction *transaction,
		      ساخت ref *ref_map)
{
	صحیح نتیجہ = 0;
	ساخت ref *ref, *stale_refs = get_stale_heads(rs, ref_map);
	ساخت strbuf err = STRBUF_INIT;
	ساخت string_list refnames = STRING_LIST_INIT_NODUP;

	برائے (ref = stale_refs; ref; ref = ref->اگلا)
		string_list_append(&refnames, ref->name);

	اگر (!dry_تسلسل) {
		اگر (transaction) {
			برائے (ref = stale_refs; ref; ref = ref->اگلا) {
				نتیجہ = ref_transaction_delete(transaction, ref->name, تہی,
								تہی, 0, "fetch: prune", &err);
				اگر (نتیجہ)
					جاؤ cleanup;
			}
		} ورنہ {
			نتیجہ = refs_delete_refs(get_main_ref_store(the_repository),
						  "fetch: prune", &refnames,
						  0);
		}
	}

	اگر (verbosity >= 0) {
		صحیح summary_چوڑائی = transport_summary_چوڑائی(stale_refs);

		برائے (ref = stale_refs; ref; ref = ref->اگلا) {
			display_ref_update(display_state, '-', _("[deleted]"), تہی,
					   _("(none)"), ref->name,
					   &ref->new_oid, &ref->old_oid,
					   summary_چوڑائی);
		}
		string_list_ترتیب_دو(&refnames);
		refs_warn_dangling_symrefs(get_main_ref_store(the_repository),
					   stderr, "   ", dry_تسلسل, &refnames);
	}

cleanup:
	string_list_clear(&refnames, 0);
	strbuf_release(&err);
	free_refs(stale_refs);
	واپس نتیجہ;
}

ساکن خالی check_not_current_branch(ساخت ref *ref_map)
{
	ثابت حرف *راستہ;
	برائے (; ref_map; ref_map = ref_map->اگلا)
		اگر (ref_map->peer_ref &&
		    starts_with(ref_map->peer_ref->name, "refs/heads/") &&
		    (راستہ = branch_checked_برآمد(ref_map->peer_ref->name)))
			die(_("refusing to fetch into branch '%s' "
			      "checked out at '%s'"),
			    ref_map->peer_ref->name, راستہ);
}

ساکن صحیح truncate_fetch_سر(خالی)
{
	ثابت حرف *filename = git_راستہ_fetch_سر(the_repository);
	FILE *fp = fopen_for_writing(filename);

	اگر (!fp)
		واپس error_errno(_("cannot open '%s'"), filename);
	fclose(fp);
	واپس 0;
}

ساکن خالی سیٹ_option(ساخت transport *transport, ثابت حرف *name, ثابت حرف *قدر)
{
	صحیح r = transport_سیٹ_option(transport, name, قدر);
	اگر (r < 0)
		die(_("option \"%s\" value \"%s\" is not valid for %s"),
		    name, قدر, transport->url);
	اگر (r > 0)
		warning(_("option \"%s\" is ignored for %s"),
			name, transport->url);
}


ساکن صحیح جوڑو_oid(ثابت ساخت reference *ref, خالی *cb_ڈیٹا)
{
	ساخت oid_array *oids = cb_ڈیٹا;

	oid_array_append(oids, ref->oid);
	واپس 0;
}

ساکن خالی جوڑو_negotiation_tips(ساخت string_list *داخلہ_list,
				 ساخت oid_array **output_list,
				 ثابت حرف *argname)
{
	ساخت oid_array *oids = xcalloc(1, حجم(*oids));
	صحیح i;

	برائے (i = 0; i < داخلہ_list->nr; i++) {
		ثابت حرف *s = داخلہ_list->اشیاء[i].string;
		ساخت refs_for_each_ref_options opts = {
			.نمونہ = s,
		};
		صحیح old_nr;
		اگر (!has_glob_specials(s)) {
			ساخت object_id oid;

			/* Ignore missing reference. */
			اگر (repo_get_oid(the_repository, s, &oid))
				جاری;
			/* Fail on missing object pointed by ref. */
			اگر (!odb_has_object(the_repository->objects, &oid, 0))
				die(_("the object %s does not exist"), s);

			oid_array_append(oids, &oid);
			جاری;
		}
		old_nr = oids->nr;
		refs_for_each_ref_ext(get_main_ref_store(the_repository),
				      جوڑو_oid, oids, &opts);
		اگر (old_nr == oids->nr)
			warning(_("ignoring %s=%s because it does not match any refs"),
				argname, s);
	}
	*output_list = oids;
}

ساکن ساخت transport *prepare_transport(ساخت remote *remote, صحیح deepen,
					   ساخت list_objects_filter_options *filter_options)
{
	ساخت transport *transport;

	transport = transport_get(remote, تہی);
	transport_سیٹ_verbosity(transport, verbosity, پیش_رفت);
	transport->family = family;
	اگر (upload_pack)
		سیٹ_option(transport, TRANS_OPT_UPLOADPACK, upload_pack);
	اگر (keep)
		سیٹ_option(transport, TRANS_OPT_KEEP, "yes");
	اگر (گہرائی)
		سیٹ_option(transport, TRANS_OPT_گہرائی, گہرائی);
	اگر (deepen && deepen_since)
		سیٹ_option(transport, TRANS_OPT_DEEPEN_SINCE, deepen_since);
	اگر (deepen && deepen_not.nr)
		سیٹ_option(transport, TRANS_OPT_DEEPEN_NOT,
			   (ثابت حرف *)&deepen_not);
	اگر (deepen_relative)
		سیٹ_option(transport, TRANS_OPT_DEEPEN_RELATIVE, "yes");
	اگر (update_shallow)
		سیٹ_option(transport, TRANS_OPT_UPDATE_SHALLOW, "yes");
	اگر (refetch)
		سیٹ_option(transport, TRANS_OPT_REFETCH, "yes");
	اگر (filter_options->choice) {
		ثابت حرف *spec =
			expand_list_objects_filter_spec(filter_options);
		سیٹ_option(transport, TRANS_OPT_LIST_OBJECTS_FILTER, spec);
		سیٹ_option(transport, TRANS_OPT_FROM_PROMISOR, "1");
	}
	اگر (negotiation_restrict.nr) {
		اگر (transport->smart_options)
			جوڑو_negotiation_tips(&negotiation_restrict,
					     &transport->smart_options->negotiation_restrict_tips,
					     "--negotiation-restrict");
		ورنہ
			warning(_("ignoring %s because the protocol does not support it"),
				"--negotiation-restrict");
	} ورنہ اگر (remote->negotiation_restrict.nr) {
		ساخت string_list_شے *شے;
		for_each_string_list_شے(شے, &remote->negotiation_restrict)
			string_list_append(&negotiation_restrict, شے->string);
		اگر (transport->smart_options)
			جوڑو_negotiation_tips(&negotiation_restrict,
					     &transport->smart_options->negotiation_restrict_tips,
					     "--negotiation-restrict");
		ورنہ {
			ساخت strbuf config_name = STRBUF_INIT;
			strbuf_addf(&config_name, "remote.%s.negotiationRestrict", remote->name);
			warning(_("ignoring %s because the protocol does not support it"),
				config_name.buf);
			strbuf_release(&config_name);
		}
	}
	اگر (negotiation_include.nr) {
		اگر (transport->smart_options)
			جوڑو_negotiation_tips(&negotiation_include,
					     &transport->smart_options->negotiation_include_tips,
					     "--negotiation-include");
		ورنہ
			warning(_("ignoring %s because the protocol does not support it"),
				"--negotiation-include");
	} ورنہ اگر (remote->negotiation_include.nr) {
		اگر (transport->smart_options) {
			جوڑو_negotiation_tips(&remote->negotiation_include,
					     &transport->smart_options->negotiation_include_tips,
					     "--negotiation-include");
		} ورنہ {
			ساخت strbuf config_name = STRBUF_INIT;
			strbuf_addf(&config_name, "remote.%s.negotiationInclude", remote->name);
			warning(_("ignoring %s because the protocol does not support it"),
				config_name.buf);
			strbuf_release(&config_name);
		}
	}
	واپس transport;
}

ساکن صحیح backfill_لیبل_فہرست(ساخت display_state *display_state,
			 ساخت transport *transport,
			 ساخت ref_transaction *transaction,
			 ساخت ref *ref_map,
			 ساخت fetch_سر *fetch_سر,
			 ثابت ساخت fetch_config *config,
			 ساخت ref_update_display_info_array *display_array,
			 ساخت list_objects_filter_options *filter_options)
{
	صحیح retcode, cannot_reuse;

	/*
	 * Once we have set TRANS_OPT_DEEPEN_SINCE, we can't unset it
	 * when remote helper is used (setting it to an empty string
	 * is not unsetting). We could extend the remote helper
	 * protocol for that, but for now, just force a new connection
	 * without deepen-since. Similar story for deepen-not.
	 */
	cannot_reuse = transport->cannot_reuse ||
		deepen_since || deepen_not.nr;
	اگر (cannot_reuse) {
		gsecondary = prepare_transport(transport->remote, 0, filter_options);
		transport = gsecondary;
	}

	transport_سیٹ_option(transport, TRANS_OPT_FOLLOWTAGS, تہی);
	transport_سیٹ_option(transport, TRANS_OPT_گہرائی, "0");
	transport_سیٹ_option(transport, TRANS_OPT_DEEPEN_RELATIVE, تہی);
	retcode = fetch_and_consume_refs(display_state, transport, transaction, ref_map,
					 fetch_سر, config, display_array);

	اگر (gsecondary) {
		transport_disconnect(gsecondary);
		gsecondary = تہی;
	}

	واپس retcode;
}

ساکن ثابت حرف *strip_refshead(ثابت حرف *name){
	skip_سابقہ(name, "refs/heads/", &name);
	واپس name;
}

ساکن خالی سیٹ_سر_advice_پیغام(ثابت حرف *remote, ثابت حرف *سر_name)
{
	ثابت حرف پیغام_advice_سیٹ_سر[] =
	N_("Run 'git remote set-head %s %s' to follow the change, or modify\n"
	   "either of the 'remote.%s.followRemoteHEAD' or 'fetch.followRemoteHEAD'\n"
	   "configuration variables to handle the situation differently.\n\n"

	   "Using this specific setting\n\n"
	   "    git config set remote.%s.followRemoteHEAD warn-if-not-%s\n\n"
	   "will suppress the warning until the remote changes HEAD to something else.");

	advise_if_enabled(ADVICE_FETCH_سیٹ_سر_WARN, _(پیغام_advice_سیٹ_سر),
			remote, سر_name, remote, remote, سر_name);
}

ساکن خالی warn_سیٹ_سر(ثابت حرف *remote, ثابت حرف *سر_name,
			ساخت strbuf *buf_پچھلا, صحیح updateres) {
	ساخت strbuf buf_سابقہ = STRBUF_INIT;
	ثابت حرف *پچھلا_سر = تہی;

	strbuf_addf(&buf_سابقہ, "refs/remotes/%s/", remote);
	skip_سابقہ(buf_پچھلا->buf, buf_سابقہ.buf, &پچھلا_سر);

	اگر (پچھلا_سر && strcmp(پچھلا_سر, سر_name)) {
		لکھو("'HEAD' at '%s' is '%s', but we have '%s' locally.\n",
			remote, سر_name, پچھلا_سر);
		سیٹ_سر_advice_پیغام(remote, سر_name);
	}
	ورنہ اگر (updateres && buf_پچھلا->len) {
		لکھو("'HEAD' at '%s' is '%s', "
			"but we have a detached HEAD pointing to '%s' locally.\n",
			remote, سر_name, buf_پچھلا->buf);
		سیٹ_سر_advice_پیغام(remote, سر_name);
	}
	strbuf_release(&buf_سابقہ);
}

ساکن صحیح سیٹ_سر(ثابت ساخت ref *remote_refs, ساخت remote *remote,
			صحیح follow_remote_سر)
{
	صحیح نتیجہ = 0, create_only, baremirror, was_detached;
	ساخت strbuf b_سر = STRBUF_INIT, b_remote_سر = STRBUF_INIT,
		      b_local_سر = STRBUF_INIT;
	ثابت حرف *no_warn_branch = remote->no_warn_branch;
	حرف *سر_name = تہی;
	ساخت ref *ref, *matches;
	ساخت ref *fetch_map = تہی, **fetch_map_دم = &fetch_map;
	ساخت refspec_شے refspec = {
		.force = 0,
		.نمونہ = 1,
		.src = (حرف *) "refs/heads/*",
		.dst = (حرف *) "refs/heads/*",
	};
	ساخت string_list heads = STRING_LIST_INIT_DUP;
	ساخت ref_store *refs = get_main_ref_store(the_repository);

	get_fetch_map(remote_refs, &refspec, &fetch_map_دم, 0);
	matches = guess_remote_سر(ڈھونڈو_ref_by_name(remote_refs, "HEAD"),
				    fetch_map, REMOTE_GUESS_سر_ALL);
	برائے (ref = matches; ref; ref = ref->اگلا) {
		string_list_append(&heads, strip_refshead(ref->name));
	}

	اگر (!heads.nr)
		نتیجہ = 1;
	ورنہ اگر (heads.nr > 1)
		نتیجہ = 1;
	ورنہ
		سر_name = xstrdup(heads.اشیاء[0].string);

	اگر (!سر_name)
		جاؤ cleanup;
	baremirror = is_bare_repository(the_repository) && remote->mirror;
	create_only = follow_remote_سر == FOLLOW_REMOTE_ALWAYS ? 0 : !baremirror;
	اگر (baremirror) {
		strbuf_addstr(&b_سر, "HEAD");
		strbuf_addf(&b_remote_سر, "refs/heads/%s", سر_name);
	} ورنہ {
		strbuf_addf(&b_سر, "refs/remotes/%s/HEAD", remote->name);
		strbuf_addf(&b_remote_سر, "refs/remotes/%s/%s", remote->name, سر_name);
	}
	/* make sure it's valid */
	اگر (!baremirror && !refs_ref_exists(refs, b_remote_سر.buf)) {
		نتیجہ = 1;
		جاؤ cleanup;
	}
	was_detached = refs_update_symref_extended(refs, b_سر.buf, b_remote_سر.buf,
					"fetch", &b_local_سر, create_only);
	اگر (was_detached == -1) {
		نتیجہ = 1;
		جاؤ cleanup;
	}
	اگر (verbosity >= 0 &&
		follow_remote_سر == FOLLOW_REMOTE_WARN &&
		(!no_warn_branch || strcmp(no_warn_branch, سر_name)))
		warn_سیٹ_سر(remote->name, سر_name, &b_local_سر, was_detached);

cleanup:
	آزاد(سر_name);
	free_refs(fetch_map);
	free_refs(matches);
	string_list_clear(&heads, 0);
	strbuf_release(&b_سر);
	strbuf_release(&b_local_سر);
	strbuf_release(&b_remote_سر);
	واپس نتیجہ;
}

ساخت ref_rejection_ڈیٹا {
	صحیح *retcode;
	bool conflict_پیغام_shown;
	bool case_sensitive_پیغام_shown;
	ثابت حرف *remote_name;
	ساخت strmap *rejected_refs;
};

ساکن خالی ref_transaction_rejection_handler(ثابت حرف *refname,
					      ثابت ساخت object_id *old_oid غیر_مستعمل,
					      ثابت ساخت object_id *new_oid غیر_مستعمل,
					      ثابت حرف *old_ہدف غیر_مستعمل,
					      ثابت حرف *new_ہدف غیر_مستعمل,
					      شمار ref_transaction_error err,
					      ثابت حرف *details,
					      خالی *cb_ڈیٹا)
{
	ساخت ref_rejection_ڈیٹا *ڈیٹا = cb_ڈیٹا;

	اگر (err == REF_TRANSACTION_ERROR_CASE_CONFLICT && repo_ignore_case(the_repository) &&
	    !ڈیٹا->case_sensitive_پیغام_shown) {
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
		ڈیٹا->case_sensitive_پیغام_shown = true;
	} ورنہ اگر (err == REF_TRANSACTION_ERROR_NAME_CONFLICT &&
		   !ڈیٹا->conflict_پیغام_shown) {
		error(_("some local refs could not be updated; try running\n"
			" 'git remote prune %s' to remove any old, conflicting "
			"branches"), ڈیٹا->remote_name);
		ڈیٹا->conflict_پیغام_shown = true;
	} ورنہ {
		اگر (details)
			error("%s", details);
		ورنہ
			error(_("fetching ref %s failed: %s"),
			      refname, ref_transaction_error_پیغام(err));
	}

	strmap_put(ڈیٹا->rejected_refs, refname, تہی);
	*ڈیٹا->retcode = 1;
}

/*
 * Commit the reference transaction. If it isn't an atomic transaction, handle
 * rejected updates as part of using batched updates.
 */
ساکن صحیح commit_ref_transaction(ساخت ref_transaction **transaction,
				  bool is_atomic, ثابت حرف *remote_name,
				  ساخت strmap *rejected_refs,
				  ساخت strbuf *err)
{
	صحیح retcode = ref_transaction_commit(*transaction, err);
	اگر (retcode)
		جاؤ برآمد;

	اگر (!is_atomic) {
		ساخت ref_rejection_ڈیٹا ڈیٹا = {
			.conflict_پیغام_shown = 0,
			.remote_name = remote_name,
			.retcode = &retcode,
			.rejected_refs = rejected_refs,
		};

		ref_transaction_for_each_rejected_update(*transaction,
							 ref_transaction_rejection_handler,
							 &ڈیٹا);
	}

برآمد:
	ref_transaction_free(*transaction);
	*transaction = تہی;
	واپس retcode;
}

ساکن خالی collect_updated_tips(ساخت oidset *tips, ساخت ref *ref_map)
{
	ساخت ref *rm;
	برائے (rm = ref_map; rm; rm = rm->اگلا) {
		ساخت commit *commit;
		/*
		 * Shallow-rejected refs are not stored and their history
		 * is incomplete, so skip them.
		 */
		اگر (rm->حالت_2 == REF_حالت_REJECT_SHALLOW)
			جاری;
		اگر (is_null_oid(&rm->old_oid))
			جاری;
		اگر (rm->peer_ref &&
		    oideq(&rm->old_oid, &rm->peer_ref->old_oid))
			جاری;
		commit = lookup_commit_reference_gently(the_repository,
							&rm->old_oid, 1);
		اگر (commit)
			oidset_داخل_کرو(tips, &commit->object.oid);
	}
}

ساکن صحیح do_fetch(ساخت transport *transport,
		    ساخت refspec *rs,
		    ثابت ساخت fetch_config *config,
		    ساخت list_objects_filter_options *filter_options,
		    ساخت oidset *updated_tips)
{
	ساخت ref_transaction *transaction = تہی;
	ساخت ref *ref_map = تہی;
	ساخت display_state display_state = { 0 };
	صحیح autotags = (transport->remote->fetch_لیبل_فہرست == 1);
	صحیح retcode = 0;
	ثابت ساخت ref *remote_refs;
	ساخت transport_ls_refs_options transport_ls_refs_options =
		TRANSPORT_LS_REFS_OPTIONS_INIT;
	ساخت fetch_سر fetch_سر = { 0 };
	ساخت strbuf err = STRBUF_INIT;
	ساخت ref_update_display_info_array display_array = { 0 };
	ساخت strmap rejected_refs = STRMAP_INIT;
	صحیح summary_چوڑائی = 0;
	صحیح follow_remote_سر = FOLLOW_REMOTE_NEVER;

	اگر (لیبل_فہرست == لیبل_فہرست_DEFAULT) {
		اگر (transport->remote->fetch_لیبل_فہرست == 2)
			لیبل_فہرست = لیبل_فہرست_سیٹ;
		اگر (transport->remote->fetch_لیبل_فہرست == -1)
			لیبل_فہرست = لیبل_فہرست_UNSET;
	}

	/* if not appending, truncate FETCH_HEAD */
	اگر (!append && write_fetch_سر) {
		retcode = truncate_fetch_سر();
		اگر (retcode)
			جاؤ cleanup;
	}

	اگر (rs->nr) {
		refspec_ref_prefixes(rs, &transport_ls_refs_options.ref_prefixes);
	} ورنہ {
		ساخت branch *branch = branch_get(تہی);

		اگر (transport->remote->fetch.nr) {
			refspec_ref_prefixes(&transport->remote->fetch,
					     &transport_ls_refs_options.ref_prefixes);
			/*
			 * See remote.c's handling of remote.<name>.followRemoteHEAD
			 * for the analogous, still-unresolved case.
			 */
			اگر (transport->remote->follow_remote_سر)
				follow_remote_سر = transport->remote->follow_remote_سر;
			ورنہ اگر (config->follow_remote_سر_نظر_آیا)
				follow_remote_سر = get_follow_remote_سر(config->follow_remote_سر_raw);
			ورنہ
				follow_remote_سر = BUILTIN_FOLLOW_REMOTE_سر_DFLT;
		}
		اگر (branch && branch_has_ملاؤ_config(branch) &&
		    !strcmp(branch->remote_name, transport->remote->name)) {
			صحیح i;
			برائے (i = 0; i < branch->ملاؤ_nr; i++) {
				strvec_ڈالو(&transport_ls_refs_options.ref_prefixes,
					    branch->ملاؤ[i]->src);
			}
		}

		/*
		 * If there are no refs specified to fetch, then we just
		 * fetch HEAD; mention that to narrow the advertisement.
		 */
		اگر (!transport_ls_refs_options.ref_prefixes.nr)
			strvec_ڈالو(&transport_ls_refs_options.ref_prefixes,
				    "HEAD");
	}

	اگر (لیبل_فہرست == لیبل_فہرست_سیٹ || لیبل_فہرست == لیبل_فہرست_DEFAULT)
		strvec_ڈالو(&transport_ls_refs_options.ref_prefixes,
			    "refs/tags/");

	اگر (follow_remote_سر != FOLLOW_REMOTE_NEVER)
		strvec_ڈالو(&transport_ls_refs_options.ref_prefixes,
			    "HEAD");

	/*
	 * Only initiate ref listing if we have at least one ref we want to
	 * know about.
	 */
	اگر (transport_ls_refs_options.ref_prefixes.nr) {
		trace2_region_enter("fetch", "remote_refs", the_repository);
		remote_refs = transport_get_remote_refs(transport,
							&transport_ls_refs_options);
		trace2_region_leave("fetch", "remote_refs", the_repository);
	} ورنہ
		remote_refs = تہی;

	transport_ls_refs_options_release(&transport_ls_refs_options);

	ref_map = get_ref_map(transport->remote, remote_refs, rs,
			      لیبل_فہرست, &autotags);
	اگر (!update_سر_ok)
		check_not_current_branch(ref_map);

	retcode = open_fetch_سر(&fetch_سر);
	اگر (retcode)
		جاؤ cleanup;

	display_state_init(&display_state, ref_map, transport->url,
			   config->display_format);

	اگر (atomic_fetch) {
		transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
							  0, &err);
		اگر (!transaction) {
			retcode = -1;
			جاؤ cleanup;
		}
	}

	اگر (لیبل_فہرست == لیبل_فہرست_DEFAULT && autotags)
		transport_سیٹ_option(transport, TRANS_OPT_FOLLOWTAGS, "1");
	اگر (prune) {
		/*
		 * We only prune based on refspecs specified
		 * explicitly (via command line or configuration); we
		 * don't care whether --tags was specified.
		 */
		اگر (rs->nr) {
			retcode = prune_refs(&display_state, rs, transaction, ref_map);
		} ورنہ {
			retcode = prune_refs(&display_state, &transport->remote->fetch,
					     transaction, ref_map);
		}
		اگر (retcode != 0)
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
	اگر (!transaction) {
		transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
							  REF_TRANSACTION_ALLOW_FAILURE, &err);
		اگر (!transaction) {
			retcode = -1;
			جاؤ cleanup;
		}
	}

	اگر (fetch_and_consume_refs(&display_state, transport, transaction, ref_map,
				   &fetch_سر, config, &display_array)) {
		retcode = 1;
		جاؤ cleanup;
	}

	/*
	 * If neither --no-tags nor --tags was specified, do automated tag
	 * following.
	 */
	اگر (لیبل_فہرست == لیبل_فہرست_DEFAULT && autotags) {
		ساخت ref *لیبل_فہرست_ref_map = تہی, **دم = &لیبل_فہرست_ref_map;

		ڈھونڈو_non_local_لیبل_فہرست(remote_refs, transaction, &لیبل_فہرست_ref_map, &دم);
		اگر (لیبل_فہرست_ref_map) {
			/*
			 * If backfilling of tags fails then we want to tell
			 * the user so, but we have to continue regardless to
			 * populate upstream information of the references we
			 * have already fetched above. The exception though is
			 * when `--atomic` is passed: in that case we'll abort
			 * the transaction and don't commit anything.
			 */
			اگر (backfill_لیبل_فہرست(&display_state, transport, transaction, لیبل_فہرست_ref_map,
					  &fetch_سر, config, &display_array, filter_options))
				retcode = 1;
		}

		free_refs(لیبل_فہرست_ref_map);
	}

	اگر (retcode)
		جاؤ cleanup;

	اگر (verbosity >= 0)
		summary_چوڑائی = transport_summary_چوڑائی(ref_map);

	retcode = commit_ref_transaction(&transaction, atomic_fetch,
					 transport->remote->name,
					 &rejected_refs, &err);
	/*
	 * With '--atomic', bail out if the transaction fails. Without '--atomic',
	 * continue to fetch head and perform other post-fetch operations.
	 */
	اگر (retcode && atomic_fetch)
		جاؤ cleanup;

	commit_fetch_سر(&fetch_سر);

	collect_updated_tips(updated_tips, ref_map);

	اگر (سیٹ_upstream) {
		ساخت branch *branch = branch_get("HEAD");
		ساخت ref *rm;
		ساخت ref *ماخذ_ref = تہی;

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
		برائے (rm = ref_map; rm; rm = rm->اگلا) {
			اگر (!rm->peer_ref) {
				اگر (ماخذ_ref) {
					warning(_("multiple branches detected, incompatible with --set-upstream"));
					جاؤ cleanup;
				} ورنہ {
					ماخذ_ref = rm;
				}
			}
		}
		اگر (ماخذ_ref) {
			اگر (!branch) {
				ثابت حرف *shortname = ماخذ_ref->name;
				skip_سابقہ(shortname, "refs/heads/", &shortname);

				warning(_("could not set upstream of HEAD to '%s' from '%s' when "
					  "it does not point to any branch."),
					shortname, transport->remote->name);
				جاؤ cleanup;
			}

			اگر (!strcmp(ماخذ_ref->name, "HEAD") ||
			    starts_with(ماخذ_ref->name, "refs/heads/"))
				install_branch_config(0,
						      branch->name,
						      transport->remote->name,
						      ماخذ_ref->name);
			ورنہ اگر (starts_with(ماخذ_ref->name, "refs/remotes/"))
				warning(_("not setting upstream for a remote remote-tracking branch"));
			ورنہ اگر (starts_with(ماخذ_ref->name, "refs/tags/"))
				warning(_("not setting upstream for a remote tag"));
			ورنہ
				warning(_("unknown branch type"));
		} ورنہ {
			warning(_("no source branch found;\n"
				  "you need to specify exactly one branch with the --set-upstream option"));
		}
	}
	اگر (follow_remote_سر != FOLLOW_REMOTE_NEVER) {
		/*
		 * Way too many cases where this can go wrong so let's just
		 * ignore errors and fail silently for now.
		 */
		سیٹ_سر(remote_refs, transport->remote, follow_remote_سر);
	}

cleanup:
	/*
	 * When using batched updates, we want to commit the non-rejected
	 * updates and also handle the rejections.
	 */
	اگر (retcode && !atomic_fetch && transaction)
		commit_ref_transaction(&transaction, false,
				       transport->remote->name,
				       &rejected_refs, &err);

	برائے (سائز_t i = 0; i < display_array.nr; i++) {
		ساخت ref_update_display_info *info = &display_array.info[i];

		اگر (!info->failed && strmap_شامل_ہے(&rejected_refs, info->ref))
			ref_update_display_info_سیٹ_failed(info);
		ref_update_display_info_display(info, &display_state, summary_چوڑائی);
		ref_update_display_info_free(info);
	}

	اگر (retcode) {
		اگر (err.len) {
			error("%s", err.buf);
			strbuf_reset(&err);
		}
		اگر (transaction && ref_transaction_abort(transaction, &err) &&
		    err.len)
			error("%s", err.buf);
		transaction = تہی;
	}

	اگر (transaction)
		ref_transaction_free(transaction);

	آزاد(display_array.info);
	strmap_clear(&rejected_refs, 0);
	display_state_release(&display_state);
	close_fetch_سر(&fetch_سر);
	strbuf_release(&err);
	free_refs(ref_map);
	واپس retcode;
}

ساکن صحیح get_one_remote_for_fetch(ساخت remote *remote, خالی *priv)
{
	ساخت string_list *list = priv;
	اگر (!remote->skip_default_update)
		string_list_append(list, remote->name);
	واپس 0;
}

ساکن خالی جوڑو_options_مقصد_argv(ساخت strvec *argv,
				ثابت ساخت fetch_config *config)
{
	اگر (dry_تسلسل)
		strvec_ڈالو(argv, "--dry-run");
	اگر (prune != -1)
		strvec_ڈالو(argv, prune ? "--prune" : "--no-prune");
	اگر (prune_لیبل_فہرست != -1)
		strvec_ڈالو(argv, prune_لیبل_فہرست ? "--prune-tags" : "--no-prune-tags");
	اگر (update_سر_ok)
		strvec_ڈالو(argv, "--update-head-ok");
	اگر (force)
		strvec_ڈالو(argv, "--force");
	اگر (keep)
		strvec_ڈالو(argv, "--keep");
	اگر (config->recurse_submodules == RECURSE_SUBMODULES_ON)
		strvec_ڈالو(argv, "--recurse-submodules");
	ورنہ اگر (config->recurse_submodules == RECURSE_SUBMODULES_OFF)
		strvec_ڈالو(argv, "--no-recurse-submodules");
	ورنہ اگر (config->recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND)
		strvec_ڈالو(argv, "--recurse-submodules=on-demand");
	اگر (لیبل_فہرست == لیبل_فہرست_سیٹ)
		strvec_ڈالو(argv, "--tags");
	ورنہ اگر (لیبل_فہرست == لیبل_فہرست_UNSET)
		strvec_ڈالو(argv, "--no-tags");
	اگر (verbosity >= 2)
		strvec_ڈالو(argv, "-v");
	اگر (verbosity >= 1)
		strvec_ڈالو(argv, "-v");
	ورنہ اگر (verbosity < 0)
		strvec_ڈالو(argv, "-q");
	اگر (family == TRANSPORT_FAMILY_IPV4)
		strvec_ڈالو(argv, "--ipv4");
	ورنہ اگر (family == TRANSPORT_FAMILY_IPV6)
		strvec_ڈالو(argv, "--ipv6");
	اگر (!write_fetch_سر)
		strvec_ڈالو(argv, "--no-write-fetch-head");
	اگر (config->display_format == DISPLAY_FORMAT_PORCELAIN)
		strvec_pushf(argv, "--porcelain");
}

/* Fetch multiple remotes in parallel */

ساخت parallel_fetch_state {
	ثابت حرف **argv;
	ساخت string_list *remotes;
	صحیح اگلا, نتیجہ;
	ثابت ساخت fetch_config *config;
};

ساکن صحیح fetch_اگلا_remote(ساخت بچہ_process *cp,
			     ساخت strbuf *برآمد غیر_مستعمل,
			     خالی *cb, خالی **task_cb)
{
	ساخت parallel_fetch_state *state = cb;
	حرف *remote;

	اگر (state->اگلا < 0 || state->اگلا >= state->remotes->nr)
		واپس 0;

	remote = state->remotes->اشیاء[state->اگلا++].string;
	*task_cb = remote;

	strvec_pushv(&cp->دلائل, state->argv);
	strvec_ڈالو(&cp->دلائل, remote);
	cp->git_cmd = 1;

	اگر (verbosity >= 0 && state->config->display_format != DISPLAY_FORMAT_PORCELAIN)
		لکھو(_("Fetching %s\n"), remote);

	واپس 1;
}

ساکن صحیح fetch_failed_مقصد_آغاز(ساخت strbuf *برآمد غیر_مستعمل,
				 خالی *cb, خالی *task_cb)
{
	ساخت parallel_fetch_state *state = cb;
	ثابت حرف *remote = task_cb;

	state->نتیجہ = error(_("could not fetch %s"), remote);

	واپس 0;
}

ساکن صحیح fetch_ختم_شدہ(صحیح نتیجہ, ساخت strbuf *برآمد,
			  خالی *cb, خالی *task_cb)
{
	ساخت parallel_fetch_state *state = cb;
	ثابت حرف *remote = task_cb;

	اگر (نتیجہ) {
		strbuf_addf(برآمد, _("could not fetch '%s' (exit code: %d)\n"),
			    remote, نتیجہ);
		state->نتیجہ = -1;
	}

	واپس 0;
}

ساکن صحیح fetch_مضرب(ساخت string_list *list, صحیح max_children,
			  ثابت ساخت fetch_config *config)
{
	صحیح i, نتیجہ = 0;
	ساخت strvec argv = STRVEC_INIT;

	اگر (!append && write_fetch_سر) {
		صحیح errcode = truncate_fetch_سر();
		اگر (errcode)
			واپس errcode;
	}

	/*
	 * Cancel out the fetch.bundleURI config when running subprocesses,
	 * to avoid fetching from the same bundle list multiple times.
	 */
	strvec_pushl(&argv, "-c", "fetch.bundleURI=",
		     "fetch", "--append", "--no-auto-gc",
		     "--no-write-commit-graph", تہی);
	برائے (i = 0; i < server_options.nr; i++)
		strvec_pushf(&argv, "--server-option=%s", server_options.اشیاء[i].string);
	جوڑو_options_مقصد_argv(&argv, config);

	اگر (max_children != 1 && list->nr != 1) {
		ساخت parallel_fetch_state state = { argv.v, list, 0, 0, config };
		ثابت ساخت تسلسل_process_parallel_opts opts = {
			.tr2_category = "fetch",
			.tr2_label = "parallel/fetch",

			.processes = max_children,

			.get_اگلا_task = &fetch_اگلا_remote,
			.آغاز_failure = &fetch_failed_مقصد_آغاز,
			.task_ختم_شدہ = &fetch_ختم_شدہ,
			.ڈیٹا = &state,
		};

		strvec_ڈالو(&argv, "--end-of-options");

		تسلسل_processes_parallel(&opts);
		نتیجہ = state.نتیجہ;
	} ورنہ
		برائے (i = 0; i < list->nr; i++) {
			ثابت حرف *name = list->اشیاء[i].string;
			ساخت بچہ_process cmd = بچہ_PROCESS_INIT;

			strvec_pushv(&cmd.دلائل, argv.v);
			strvec_ڈالو(&cmd.دلائل, name);
			اگر (verbosity >= 0 && config->display_format != DISPLAY_FORMAT_PORCELAIN)
				لکھو(_("Fetching %s\n"), name);
			cmd.git_cmd = 1;
			اگر (تسلسل_command(&cmd)) {
				error(_("could not fetch %s"), name);
				نتیجہ = 1;
			}
		}

	strvec_clear(&argv);
	واپس !!نتیجہ;
}

/*
 * Fetching from the promisor remote should use the given filter-spec
 * or inherit the default filter-spec from the config.
 */
ساکن اندرونی خالی fetch_one_تیاری_partial(ساخت remote *remote,
					   ساخت list_objects_filter_options *filter_options)
{
	/*
	 * Explicit --no-filter argument overrides everything, regardless
	 * of any prior partial clones and fetches.
	 */
	اگر (filter_options->no_filter)
		واپس;

	/*
	 * If no prior partial clone/fetch and the current fetch DID NOT
	 * request a partial-fetch, do a normal fetch.
	 */
	اگر (!repo_has_promisor_remote(the_repository) && !filter_options->choice)
		واپس;

	/*
	 * If this is a partial-fetch request, we enable partial on
	 * this repo if not already enabled and remember the given
	 * filter-spec as the default for subsequent fetches to this
	 * remote if there is currently no default filter-spec.
	 */
	اگر (filter_options->choice) {
		partial_clone_رجسٹر(remote->name, filter_options);
		واپس;
	}

	/*
	 * Do a partial-fetch from the promisor remote using either the
	 * explicitly given filter-spec or inherit the filter-spec from
	 * the config.
	 */
	اگر (!filter_options->choice)
		partial_clone_get_default_filter_spec(filter_options, remote->name);
	واپس;
}

ساکن صحیح fetch_one(ساخت remote *remote, صحیح argc, ثابت حرف **argv,
		     صحیح prune_لیبل_فہرست_ok, صحیح use_stdin_refspecs,
		     ثابت ساخت fetch_config *config,
		     ساخت list_objects_filter_options *filter_options,
		     ساخت oidset *updated_tips)
{
	ساخت refspec rs = REFSPEC_INIT_FETCH(the_hash_algo);
	صحیح i;
	صحیح exit_رمز;
	صحیح maybe_prune_لیبل_فہرست;
	صحیح remote_بذریعہ_config = remote_is_configured(remote, 0);

	اگر (!remote)
		die(_("no remote repository specified; please specify either a URL or a\n"
		      "remote name from which new revisions should be fetched"));

	gtransport = prepare_transport(remote, 1, filter_options);

	اگر (prune < 0) {
		/* no command line request */
		اگر (0 <= remote->prune)
			prune = remote->prune;
		ورنہ اگر (0 <= config->prune)
			prune = config->prune;
		ورنہ
			prune = PRUNE_BY_DEFAULT;
	}

	اگر (prune_لیبل_فہرست < 0) {
		/* no command line request */
		اگر (0 <= remote->prune_لیبل_فہرست)
			prune_لیبل_فہرست = remote->prune_لیبل_فہرست;
		ورنہ اگر (0 <= config->prune_لیبل_فہرست)
			prune_لیبل_فہرست = config->prune_لیبل_فہرست;
		ورنہ
			prune_لیبل_فہرست = PRUNE_لیبل_فہرست_BY_DEFAULT;
	}

	maybe_prune_لیبل_فہرست = prune_لیبل_فہرست_ok && prune_لیبل_فہرست;
	اگر (maybe_prune_لیبل_فہرست && remote_بذریعہ_config)
		refspec_append(&remote->fetch, لیبل_REFSPEC);

	اگر (maybe_prune_لیبل_فہرست && (argc || !remote_بذریعہ_config))
		refspec_append(&rs, لیبل_REFSPEC);

	برائے (i = 0; i < argc; i++) {
		اگر (!strcmp(argv[i], "tag")) {
			i++;
			اگر (i >= argc)
				die(_("you need to specify a tag name"));

			refspec_appendf(&rs, "refs/tags/%s:refs/tags/%s",
					argv[i], argv[i]);
		} ورنہ {
			refspec_append(&rs, argv[i]);
		}
	}

	اگر (use_stdin_refspecs) {
		ساخت strbuf سطر = STRBUF_INIT;
		جب (strbuf_getline_lf(&سطر, stdin) != EOF)
			refspec_append(&rs, سطر.buf);
		strbuf_release(&سطر);
	}

	اگر (server_options.nr)
		gtransport->server_options = &server_options;

	sigchain_ڈالو_common(unlock_pack_on_signal);
	atexit(unlock_pack_atexit);
	sigchain_ڈالو(SIGPIPE, SIG_IGN);
	exit_رمز = do_fetch(gtransport, &rs, config, filter_options,
			     updated_tips);
	sigchain_نکالو(SIGPIPE);
	refspec_clear(&rs);
	transport_disconnect(gtransport);
	gtransport = تہی;
	واپس exit_رمز;
}

صحیح cmd_fetch(صحیح argc,
	      ثابت حرف **argv,
	      ثابت حرف *سابقہ,
	      ساخت repository *repo غیر_مستعمل)
{
	ساخت fetch_config config = {
		.display_format = DISPLAY_FORMAT_بھرا,
		.follow_remote_سر_raw = تہی,
		.follow_remote_سر_نظر_آیا = 0,
		.prune = -1,
		.prune_لیبل_فہرست = -1,
		.show_forced_updates = 1,
		.recurse_submodules = RECURSE_SUBMODULES_DEFAULT,
		.parallel = 1,
		.submodule_fetch_jobs = -1,
	};
	ثابت حرف *submodule_سابقہ = "";
	ثابت حرف *bundle_uri;
	ساخت string_list list = STRING_LIST_INIT_DUP;
	ساخت list_objects_filter_options filter_options = LIST_OBJECTS_FILTER_INIT;
	ساخت remote *remote = تہی;
	صحیح all = -1, مضرب = 0;
	صحیح نتیجہ = 0;
	صحیح prune_لیبل_فہرست_ok = 1;
	صحیح enable_auto_gc = 1;
	صحیح unshallow = 0;
	صحیح max_jobs = -1;
	صحیح recurse_submodules_cli = RECURSE_SUBMODULES_DEFAULT;
	صحیح recurse_submodules_default = RECURSE_SUBMODULES_ON_DEMAND;
	صحیح fetch_write_commit_گراف = -1;
	صحیح stdin_refspecs = 0;
	صحیح negotiate_only = 0;
	صحیح porcelain = 0;
	صحیح i;
	شمار {
		گراف_WRITE_REACHABLE,
		گراف_WRITE_TIPS,
		گراف_WRITE_SKIP,
	} گراف_write_منوال = گراف_WRITE_REACHABLE;
	ساخت oidset updated_tips = OIDSET_INIT;

	ساخت option builtin_fetch_options[] = {
		OPT__VERBOSITY(&verbosity),
		OPT_BOOL(0, "all", &all,
			 N_("fetch from all remotes")),
		OPT_BOOL(0, "set-upstream", &سیٹ_upstream,
			 N_("set upstream for git pull/fetch")),
		OPT_BOOL('a', "append", &append,
			 N_("append to .git/FETCH_HEAD instead of overwriting")),
		OPT_BOOL(0, "atomic", &atomic_fetch,
			 N_("use atomic transaction to update references")),
		OPT_STRING(0, "upload-pack", &upload_pack, N_("path"),
			   N_("path to upload pack on remote end")),
		OPT__FORCE(&force, N_("force overwrite of local reference"), 0),
		OPT_BOOL('m', "multiple", &مضرب,
			 N_("fetch from multiple remotes")),
		OPT_سیٹ_INT('t', "tags", &لیبل_فہرست,
			    N_("fetch all tags and associated objects"), لیبل_فہرست_سیٹ),
		OPT_سیٹ_INT('n', تہی, &لیبل_فہرست,
			    N_("do not fetch all tags (--no-tags)"), لیبل_فہرست_UNSET),
		OPT_INTEGER('j', "jobs", &max_jobs,
			    N_("number of submodules fetched in parallel")),
		OPT_BOOL(0, "prefetch", &prefetch,
			 N_("modify the refspec to place all refs within refs/prefetch/")),
		OPT_BOOL('p', "prune", &prune,
			 N_("prune remote-tracking branches no longer on remote")),
		OPT_BOOL('P', "prune-tags", &prune_لیبل_فہرست,
			 N_("prune local tags no longer on remote and clobber changed tags")),
		OPT_CALLBACK_F(0, "recurse-submodules", &recurse_submodules_cli, N_("on-demand"),
			    N_("control recursive fetching of submodules"),
			    PARSE_OPT_OPTARG, option_fetch_parse_recurse_submodules),
		OPT_BOOL(0, "dry-run", &dry_تسلسل,
			 N_("dry run")),
		OPT_BOOL(0, "porcelain", &porcelain, N_("machine-readable output")),
		OPT_BOOL(0, "write-fetch-head", &write_fetch_سر,
			 N_("write fetched references to the FETCH_HEAD file")),
		OPT_BOOL('k', "keep", &keep, N_("keep downloaded pack")),
		OPT_BOOL('u', "update-head-ok", &update_سر_ok,
			    N_("allow updating of HEAD ref")),
		OPT_BOOL(0, "progress", &پیش_رفت, N_("force progress reporting")),
		OPT_STRING(0, "depth", &گہرائی, N_("depth"),
			   N_("deepen history of shallow clone")),
		OPT_STRING(0, "shallow-since", &deepen_since, N_("time"),
			   N_("deepen history of shallow repository based on time")),
		OPT_STRING_LIST(0, "shallow-exclude", &deepen_not, N_("ref"),
				N_("deepen history of shallow clone, excluding ref")),
		OPT_INTEGER(0, "deepen", &deepen_relative,
			    N_("deepen history of shallow clone")),
		OPT_سیٹ_INT_F(0, "unshallow", &unshallow,
			      N_("convert to a complete repository"),
			      1, PARSE_OPT_NONEG),
		OPT_سیٹ_INT_F(0, "refetch", &refetch,
			      N_("re-fetch without negotiating common commits"),
			      1, PARSE_OPT_NONEG),
		{
			.type = OPTION_STRING,
			.long_name = "submodule-prefix",
			.قدر = &submodule_سابقہ,
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
		OPT_BOOL(0, "write-commit-graph", &fetch_write_commit_گراف,
			 N_("write the commit-graph after fetching")),
		OPT_BOOL(0, "stdin", &stdin_refspecs,
			 N_("accept refspecs from stdin")),
		OPT_انتہا()
	};

	filter_options.allow_auto_filter = 1;

	refspec_init_fetch(&refmap, the_hash_algo);

	packet_trace_شناختی_عنصر("fetch");

	/* Record the command line for the reflog */
	strbuf_addstr(&default_rla, "fetch");
	برائے (i = 1; i < argc; i++) {
		/* This handles non-URLs gracefully */
		حرف *anon = transport_anonymize_url(argv[i]);

		strbuf_addf(&default_rla, " %s", anon);
		آزاد(anon);
	}

	repo_config(the_repository, git_fetch_config, &config);
	اگر (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_بھرا_اشاریہ = 0;
	}

	argc = parse_options(argc, argv, سابقہ,
			     builtin_fetch_options, builtin_fetch_usage, 0);

	اگر (recurse_submodules_cli != RECURSE_SUBMODULES_DEFAULT)
		config.recurse_submodules = recurse_submodules_cli;

	اگر (negotiate_only) {
		انتخاب (recurse_submodules_cli) {
		حالت RECURSE_SUBMODULES_OFF:
		حالت RECURSE_SUBMODULES_DEFAULT:
			/*
			 * --negotiate-only should never recurse into
			 * submodules. Skip it by setting recurse_submodules to
			 * RECURSE_SUBMODULES_OFF.
			 */
			config.recurse_submodules = RECURSE_SUBMODULES_OFF;
			توڑو;

		طےشدہ:
			die(_("options '%s' and '%s' cannot be used together"),
			    "--negotiate-only", "--recurse-submodules");
		}
	}

	اگر (config.recurse_submodules != RECURSE_SUBMODULES_OFF) {
		صحیح *sfjc = config.submodule_fetch_jobs == -1
			    ? &config.submodule_fetch_jobs : تہی;
		صحیح *rs = config.recurse_submodules == RECURSE_SUBMODULES_DEFAULT
			  ? &config.recurse_submodules : تہی;

		fetch_config_from_gitmodules(the_repository, sfjc, rs);
	}


	اگر (porcelain) {
		انتخاب (recurse_submodules_cli) {
		حالت RECURSE_SUBMODULES_OFF:
		حالت RECURSE_SUBMODULES_DEFAULT:
			/*
			 * Reference updates in submodules would be ambiguous
			 * in porcelain mode, so we reject this combination.
			 */
			config.recurse_submodules = RECURSE_SUBMODULES_OFF;
			توڑو;

		طےشدہ:
			die(_("options '%s' and '%s' cannot be used together"),
			    "--porcelain", "--recurse-submodules");
		}

		config.display_format = DISPLAY_FORMAT_PORCELAIN;
	}

	اگر (deepen_relative) {
		اگر (deepen_relative < 0)
			die(_("negative depth in --deepen is not supported"));
		اگر (گہرائی)
			die(_("options '%s' and '%s' cannot be used together"), "--deepen", "--depth");
		گہرائی = xstrfmt("%d", deepen_relative);
	}
	اگر (unshallow) {
		اگر (گہرائی)
			die(_("options '%s' and '%s' cannot be used together"), "--depth", "--unshallow");
		ورنہ اگر (!is_repository_shallow(the_repository))
			die(_("--unshallow on a complete repository does not make sense"));
		ورنہ
			گہرائی = xstrfmt("%d", INFINITE_گہرائی);
	}

	/* no need to be strict, transport_set_option() will validate it again */
	اگر (گہرائی && atoi(گہرائی) < 1)
		die(_("depth %s is not a positive number"), گہرائی);
	اگر (گہرائی || deepen_since || deepen_not.nr)
		deepen = 1;

	/* FETCH_HEAD never gets updated in --dry-run mode */
	اگر (dry_تسلسل)
		write_fetch_سر = 0;

	اگر (!max_jobs)
		max_jobs = online_cpus();

	اگر (!repo_config_get_string_tmp(the_repository, "fetch.bundleuri", &bundle_uri) &&
	    fetch_bundle_uri(the_repository, bundle_uri, تہی))
		warning(_("failed to fetch bundles from '%s'"), bundle_uri);

	اگر (all < 0) {
		/*
		 * no --[no-]all given;
		 * only use config option if no remote was explicitly specified
		 */
		all = (!argc) ? config.all : 0;
	}

	اگر (all) {
		اگر (argc == 1)
			die(_("fetch --all does not take a repository argument"));
		ورنہ اگر (argc > 1)
			die(_("fetch --all does not make sense with refspecs"));

		(خالی) for_each_remote(get_one_remote_for_fetch, &list);

		/* do not do fetch_multiple() of one */
		اگر (list.nr == 1)
			remote = remote_get(list.اشیاء[0].string);
	} ورنہ اگر (argc == 0) {
		/* No arguments -- use default remote */
		remote = remote_get(تہی);
	} ورنہ اگر (مضرب) {
		/* All arguments are assumed to be remotes or groups */
		برائے (i = 0; i < argc; i++)
			اگر (!جوڑو_remote_or_گروہ(argv[i], &list))
				die(_("no such remote or remote group: %s"),
				    argv[i]);
	} ورنہ {
		/* Single remote or group */
		(خالی) جوڑو_remote_or_گروہ(argv[0], &list);
		اگر (list.nr > 1) {
			/* More than one remote */
			اگر (argc > 1)
				die(_("fetching a group and specifying refspecs does not make sense"));
		} ورنہ {
			/* Zero or one remotes */
			remote = remote_get(argv[0]);
			prune_لیبل_فہرست_ok = (argc == 1);
			argc--;
			argv++;
		}
	}
	string_list_remove_duplicates(&list, 0);

	اگر (negotiate_only) {
		ساخت oidset acked_commits = OIDSET_INIT;
		ساخت oidset_iter iter;
		ثابت ساخت object_id *oid;

		trace2_region_enter("fetch", "negotiate-only", the_repository);
		اگر (!remote)
			die(_("must supply remote when using --negotiate-only"));
		gtransport = prepare_transport(remote, 1, &filter_options);

		اگر (!gtransport->smart_options) {
			warning(_("protocol does not support --negotiate-only, exiting"));
			نتیجہ = 1;
			trace2_region_leave("fetch", "negotiate-only", the_repository);
			جاؤ cleanup;
		}
		اگر (!gtransport->smart_options->negotiation_restrict_tips)
			die(_("%s needs one or more %s"), "--negotiate-only",
			    "--negotiation-restrict=*");

		gtransport->smart_options->acked_commits = &acked_commits;

		اگر (server_options.nr)
			gtransport->server_options = &server_options;
		نتیجہ = transport_fetch_refs(gtransport, تہی);
		gtransport->smart_options->acked_commits = تہی;

		oidset_iter_init(&acked_commits, &iter);
		جب ((oid = oidset_iter_اگلا(&iter)))
			لکھو("%s\n", oid_مقصد_hex(oid));
		oidset_clear(&acked_commits);
		trace2_region_leave("fetch", "negotiate-only", the_repository);
	} ورنہ اگر (remote) {
		اگر (filter_options.choice || repo_has_promisor_remote(the_repository)) {
			trace2_region_enter("fetch", "setup-partial", the_repository);
			fetch_one_تیاری_partial(remote, &filter_options);
			trace2_region_leave("fetch", "setup-partial", the_repository);
		}
		trace2_region_enter("fetch", "fetch-one", the_repository);
		نتیجہ = fetch_one(remote, argc, argv, prune_لیبل_فہرست_ok, stdin_refspecs,
				   &config, &filter_options, &updated_tips);
		اگر (prepare_commit_گراف(the_repository)) {
			اگر (oidset_سائز(&updated_tips))
				گراف_write_منوال = گراف_WRITE_TIPS;
			ورنہ
				گراف_write_منوال = گراف_WRITE_SKIP;
		}
		trace2_region_leave("fetch", "fetch-one", the_repository);
	} ورنہ {
		صحیح max_children = max_jobs;

		اگر (filter_options.choice)
			die(_("--filter can only be used with the remote "
			      "configured in extensions.partialclone"));

		اگر (atomic_fetch)
			die(_("--atomic can only be used when fetching "
			      "from one remote"));

		اگر (stdin_refspecs)
			die(_("--stdin can only be used when fetching "
			      "from one remote"));

		اگر (max_children < 0)
			max_children = config.parallel;

		/* TODO should this also die if we have a previous partial-clone? */
		trace2_region_enter("fetch", "fetch-multiple", the_repository);
		نتیجہ = fetch_مضرب(&list, max_children, &config);
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
	اگر (!نتیجہ && remote && (config.recurse_submodules != RECURSE_SUBMODULES_OFF)) {
		ساخت strvec options = STRVEC_INIT;
		صحیح max_children = max_jobs;

		اگر (max_children < 0)
			max_children = config.submodule_fetch_jobs;
		اگر (max_children < 0)
			max_children = config.parallel;

		جوڑو_options_مقصد_argv(&options, &config);
		trace2_region_enter_printf("fetch", "recurse-submodule", the_repository, "%s", submodule_سابقہ);
		نتیجہ = fetch_submodules(the_repository,
					  &options,
					  submodule_سابقہ,
					  config.recurse_submodules,
					  recurse_submodules_default,
					  verbosity < 0,
					  max_children);
		trace2_region_leave_printf("fetch", "recurse-submodule", the_repository, "%s", submodule_سابقہ);
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
	اگر (negotiate_only)
		جاؤ cleanup;

	prepare_repo_settings(the_repository);
	اگر (fetch_write_commit_گراف > 0 ||
	    (fetch_write_commit_گراف < 0 &&
	     the_repository->settings.fetch_write_commit_گراف)) {
		صحیح commit_گراف_flags = COMMIT_گراف_WRITE_بانٹو;

		اگر (پیش_رفت)
			commit_گراف_flags |= COMMIT_گراف_WRITE_پیش_رفت;

		اگر (گراف_write_منوال != گراف_WRITE_SKIP) {
			trace2_region_enter("fetch", "write-commit-graph",
					    the_repository);
			اگر (گراف_write_منوال == گراف_WRITE_TIPS)
				write_commit_گراف(
					the_repository,
					the_repository->objects->ماخذ->راستہ,
					تہی, &updated_tips,
					commit_گراف_flags, تہی);
			ورنہ
				write_commit_گراف_reachable(
					the_repository,
					the_repository->objects->ماخذ->راستہ,
					commit_گراف_flags, تہی);
			trace2_region_leave("fetch", "write-commit-graph",
					    the_repository);
		}
	}

	اگر (enable_auto_gc) {
		اگر (refetch) {
			/*
			 * Hint auto-maintenance strongly to encourage repacking,
			 * but respect config settings disabling it.
			 */
			صحیح opt_val;
			اگر (repo_config_get_int(the_repository, "gc.autopacklimit", &opt_val))
				opt_val = -1;
			اگر (opt_val != 0)
				git_config_ڈالو_parameter("gc.autoPackLimit=1");

			اگر (repo_config_get_int(the_repository, "maintenance.incremental-repack.auto", &opt_val))
				opt_val = -1;
			اگر (opt_val != 0)
				git_config_ڈالو_parameter("maintenance.incremental-repack.auto=-1");
		}
		تسلسل_auto_maintenance(the_repository, verbosity < 0);
	}

 cleanup:
	oidset_clear(&updated_tips);
	string_list_clear(&list, 0);
	list_objects_filter_release(&filter_options);
	آزاد(config.follow_remote_سر_raw);
	واپس نتیجہ;
}
