/*
 * "git fetch"
 */

#تعریف_کن USE_THE_REPOSITORY_VARIABLE
#تعریف_کن DISABLE_SIGN_COMPARE_WARNINGS

#ضمن "builtin.h"
#ضمن "advice.h"
#ضمن "config.h"
#ضمن "gettext.h"
#ضمن "environment.h"
#ضمن "hex.h"
#ضمن "refs.h"
#ضمن "refspec.h"
#ضمن "object-name.h"
#ضمن "odb.h"
#ضمن "odb/source.h"
#ضمن "oidset.h"
#ضمن "oid-array.h"
#ضمن "commit.h"
#ضمن "string-list.h"
#ضمن "remote.h"
#ضمن "transport.h"
#ضمن "run-command.h"
#ضمن "parse-options.h"
#ضمن "sigchain.h"
#ضمن "submodule-config.h"
#ضمن "submodule.h"
#ضمن "connected.h"
#ضمن "strvec.h"
#ضمن "utf8.h"
#ضمن "pager.h"
#ضمن "path.h"
#ضمن "pkt-line.h"
#ضمن "list-objects-filter-options.h"
#ضمن "commit-reach.h"
#ضمن "branch.h"
#ضمن "promisor-remote.h"
#ضمن "commit-graph.h"
#ضمن "shallow.h"
#ضمن "trace.h"
#ضمن "trace2.h"
#ضمن "bundle-uri.h"

#تعریف_کن FORCED_UPDATES_DELAY_WARNING_درون_MS (10 * 1000)

ساكن ثابت حرف * ثابت builtin_fetch_usage[] = {
	N_("git fetch [<options>] [<repository> [<refspec>...]]"),
	N_("git fetch [<options>] <group>"),
	N_("git fetch --multiple [<options>] [(<repository>|<group>)...]"),
	N_("git fetch --all [<options>]"),
	عدم
};

تعداد {
	برچسبها_UNSET = 0,
	برچسبها_DEFAULT = 1,
	برچسبها_مجموعه = 2
};

تعداد display_format {
	DISPLAY_FORMAT_پر,
	DISPLAY_FORMAT_COMPACT,
	DISPLAY_FORMAT_PORCELAIN,
};

بنية display_state {
	بنية strbuf buf;

	صحيح refcol_عرض;
	تعداد display_format format;

	حرف *url;
	صحيح url_len, shown_url;
};

ساكن uint64_t forced_updates_ms = 0;
ساكن صحيح prefetch = 0;
ساكن صحيح prune = -1; /* unspecified */
#تعریف_کن PRUNE_BY_DEFAULT 0 /* do we prune by default? */

ساكن صحيح prune_برچسبها = -1; /* unspecified */
#تعریف_کن PRUNE_برچسبها_BY_DEFAULT 0 /* do we prune tags by default? */

ساكن صحيح append, dry_پیاپی, force, keep, update_سر_ok;
ساكن صحيح write_fetch_سر = 1;
ساكن صحيح verbosity, deepen_relative, مجموعه_upstream, refetch;
ساكن صحيح پیشرفت = -1;
ساكن صحيح برچسبها = برچسبها_DEFAULT, update_shallow, deepen;
ساكن صحيح atomic_fetch;
ساكن تعداد transport_family family;
ساكن ثابت حرف *عمق;
ساكن ثابت حرف *deepen_since;
ساكن ثابت حرف *upload_pack;
ساكن بنية string_list deepen_not = STRING_LIST_INIT_NODUP;
ساكن بنية strbuf default_rla = STRBUF_INIT;
ساكن بنية transport *gtransport;
ساكن بنية transport *gsecondary;
ساكن بنية refspec refmap;
ساكن بنية string_list server_options = STRING_LIST_INIT_DUP;
ساكن بنية string_list negotiation_restrict = STRING_LIST_INIT_NODUP;
ساكن بنية string_list negotiation_include = STRING_LIST_INIT_NODUP;

بنية fetch_config {
	تعداد display_format display_format;
	حرف *follow_remote_سر_raw;
	صحيح follow_remote_سر_دیده;
	صحيح all;
	صحيح prune;
	صحيح prune_برچسبها;
	صحيح show_forced_updates;
	صحيح recurse_submodules;
	صحيح parallel;
	صحيح submodule_fetch_jobs;
};

ساكن صحيح git_fetch_config(ثابت حرف *k, ثابت حرف *v,
			    ثابت بنية config_context *ctx, فراغ *cb)
{
	بنية fetch_config *fetch_config = cb;

	إذا (!strcmp(k, "fetch.all")) {
		fetch_config->all = git_config_bool(k, v);
		أرجع 0;
	}

	إذا (!strcmp(k, "fetch.prune")) {
		fetch_config->prune = git_config_bool(k, v);
		أرجع 0;
	}

	إذا (!strcmp(k, "fetch.prunetags")) {
		fetch_config->prune_برچسبها = git_config_bool(k, v);
		أرجع 0;
	}

	إذا (!strcmp(k, "fetch.showforcedupdates")) {
		fetch_config->show_forced_updates = git_config_bool(k, v);
		أرجع 0;
	}

	إذا (!strcmp(k, "submodule.recurse")) {
		صحيح r = git_config_bool(k, v) ?
			RECURSE_SUBMODULES_ON : RECURSE_SUBMODULES_OFF;
		fetch_config->recurse_submodules = r;
		أرجع 0;
	}

	إذا (!strcmp(k, "submodule.fetchjobs")) {
		fetch_config->submodule_fetch_jobs = parse_submodule_fetchjobs(k, v, ctx->kvi);
		أرجع 0;
	} وإلا إذا (!strcmp(k, "fetch.recursesubmodules")) {
		fetch_config->recurse_submodules = parse_fetch_recurse_submodules_arg(k, v);
		أرجع 0;
	}

	إذا (!strcmp(k, "fetch.parallel")) {
		fetch_config->parallel = git_config_int(k, v, ctx->kvi);
		إذا (fetch_config->parallel < 0)
			die(_("fetch.parallel cannot be negative"));
		إذا (!fetch_config->parallel)
			fetch_config->parallel = online_cpus();
		أرجع 0;
	}

	إذا (!strcmp(k, "fetch.output")) {
		إذا (!v)
			أرجع config_error_nonbool(k);
		وإلا إذا (!strcasecmp(v, "full"))
			fetch_config->display_format = DISPLAY_FORMAT_پر;
		وإلا إذا (!strcasecmp(v, "compact"))
			fetch_config->display_format = DISPLAY_FORMAT_COMPACT;
		وإلا
			die(_("invalid value for '%s': '%s'"),
			    "fetch.output", v);
		أرجع 0;
	}

	إذا (!strcmp(k, "fetch.followremotehead")) {
		حرر(fetch_config->follow_remote_سر_raw);
		fetch_config->follow_remote_سر_raw = xstrdup_or_null(v);
		fetch_config->follow_remote_سر_دیده = 1;
		أرجع 0;
	}

	أرجع git_default_config(k, v, ctx, cb);
}

ساكن تعداد follow_remote_سر_settings get_follow_remote_سر(ثابت حرف *setting)
{
	إذا (!setting)
		die(_("missing value for 'fetch.followRemoteHEAD'"));
	وإلا إذا (!strcmp(setting, "never"))
		أرجع FOLLOW_REMOTE_NEVER;
	وإلا إذا (!strcmp(setting, "create"))
		أرجع FOLLOW_REMOTE_CREATE;
	وإلا إذا (!strcmp(setting, "warn"))
		أرجع FOLLOW_REMOTE_WARN;
	وإلا إذا (!strcmp(setting, "always"))
		أرجع FOLLOW_REMOTE_ALWAYS;
	warning(_("unrecognized fetch.followRemoteHEAD value '%s' ignored"), setting);
	أرجع BUILTIN_FOLLOW_REMOTE_سر_DFLT;
}

ساكن صحيح parse_refmap_arg(ثابت بنية option *opt, ثابت حرف *arg, صحيح unset)
{
	BUG_ON_OPT_NEG(unset);

	/*
	 * "git fetch --refmap='' origin foo"
	 * can be used to tell the command not to store anywhere
	 */
	refspec_append(opt->مقدار, arg);

	أرجع 0;
}

ساكن فراغ unlock_pack(بیعلامت صحيح flags)
{
	إذا (gtransport)
		transport_unlock_pack(gtransport, flags);
	إذا (gsecondary)
		transport_unlock_pack(gsecondary, flags);
}

ساكن فراغ unlock_pack_atexit(فراغ)
{
	unlock_pack(0);
}

ساكن فراغ unlock_pack_on_signal(صحيح signo)
{
	unlock_pack(TRANSPORT_UNLOCK_PACK_درون_SIGNAL_HANDLER);
	sigchain_برداشت(signo);
	ارفع(signo);
}

ساكن فراغ جمع_کن_ادغام_config(بنية ref **سر,
			   ثابت بنية ref *remote_refs,
		           بنية branch *branch,
		           بنية ref ***دم)
{
	صحيح i;

	لكل (i = 0; i < branch->ادغام_nr; i++) {
		بنية ref *rm, **old_دم = *دم;
		بنية refspec_قلم refspec;

		لكل (rm = *سر; rm; rm = rm->بعدی) {
			إذا (branch_ادغام_matches(branch, i, rm->name)) {
				rm->fetch_سر_وضعیت = FETCH_سر_ادغام;
				اكسر;
			}
		}
		إذا (rm)
			استمر;

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
		refspec.src = branch->ادغام[i]->src;
		get_fetch_map(remote_refs, &refspec, دم, 1);
		لكل (rm = *old_دم; rm; rm = rm->بعدی)
			rm->fetch_سر_وضعیت = FETCH_سر_ادغام;
	}
}

ساكن فراغ create_fetch_oidset(بنية ref **سر, بنية oidset *بیرون)
{
	بنية ref *rm = *سر;
	طالما (rm) {
		oidset_درج(بیرون, &rm->old_oid);
		rm = rm->بعدی;
	}
}

بنية refname_hash_مدخل {
	بنية hashmap_مدخل ent;
	بنية object_id oid;
	صحيح ignore;
	حرف refname[FLEX_ARRAY];
};

ساكن صحيح refname_hash_مدخل_cmp(ثابت فراغ *hashmap_cmp_fn_داده استفادهنشده,
				  ثابت بنية hashmap_مدخل *eptr,
				  ثابت بنية hashmap_مدخل *مدخل_or_کلید,
				  ثابت فراغ *keydata)
{
	ثابت بنية refname_hash_مدخل *e1, *e2;

	e1 = container_از(eptr, ثابت بنية refname_hash_مدخل, ent);
	e2 = container_از(مدخل_or_کلید, ثابت بنية refname_hash_مدخل, ent);
	أرجع strcmp(e1->refname, keydata ? keydata : e2->refname);
}

ساكن بنية refname_hash_مدخل *refname_hash_جمع_کن(بنية hashmap *map,
						   ثابت حرف *refname,
						   ثابت بنية object_id *oid)
{
	بنية refname_hash_مدخل *ent;
	اندازه_t len = طول_رشته(refname);

	FLEX_ALLOC_MEM(ent, refname, refname, len);
	hashmap_مدخل_init(&ent->ent, strhash(refname));
	oidcpy(&ent->oid, oid);
	hashmap_جمع_کن(map, &ent->ent);
	أرجع ent;
}

ساكن صحيح جمع_کن_one_refname(ثابت بنية reference *ref, فراغ *cbdata)
{
	بنية hashmap *refname_map = cbdata;

	(فراغ) refname_hash_جمع_کن(refname_map, ref->name, ref->oid);
	أرجع 0;
}

ساكن فراغ refname_hash_init(بنية hashmap *map)
{
	hashmap_init(map, refname_hash_مدخل_cmp, عدم, 0);
}

ساكن صحيح refname_hash_exists(بنية hashmap *map, ثابت حرف *refname)
{
	أرجع !!hashmap_get_from_hash(map, strhash(refname), refname);
}

ساكن فراغ clear_قلم(بنية refname_hash_مدخل *قلم)
{
	قلم->ignore = 1;
}


ساكن فراغ جمع_کن_already_queued_برچسبها(ثابت حرف *refname,
				    ثابت بنية object_id *old_oid استفادهنشده,
				    ثابت بنية object_id *new_oid,
				    فراغ *cb_داده)
{
	بنية hashmap *queued_برچسبها = cb_داده;
	إذا (starts_with(refname, "refs/tags/") && new_oid)
		(فراغ) refname_hash_جمع_کن(queued_برچسبها, refname, new_oid);
}

ساكن فراغ پیدا_کن_non_local_برچسبها(ثابت بنية ref *refs,
				بنية ref_transaction *transaction,
				بنية ref **سر,
				بنية ref ***دم)
{
	بنية hashmap existing_refs;
	بنية hashmap remote_refs;
	بنية oidset fetch_oids = OIDSET_INIT;
	بنية string_list remote_refs_list = STRING_LIST_INIT_NODUP;
	بنية string_list_قلم *remote_ref_قلم;
	ثابت بنية ref *ref;
	بنية refname_hash_مدخل *قلم = عدم;

	refname_hash_init(&existing_refs);
	refname_hash_init(&remote_refs);
	create_fetch_oidset(سر, &fetch_oids);

	refs_for_each_ref(get_main_ref_store(the_repository), جمع_کن_one_refname,
			  &existing_refs);

	/*
	 * If we already have a transaction, then we need to filter out all
	 * tags which have already been queued up.
	 */
	إذا (transaction)
		ref_transaction_for_each_queued_update(transaction,
						       جمع_کن_already_queued_برچسبها,
						       &existing_refs);

	لكل (ref = refs; ref; ref = ref->بعدی) {
		إذا (!starts_with(ref->name, "refs/tags/"))
			استمر;

		/*
		 * The peeled ref always follows the matching base
		 * ref, so if we see a peeled ref that we don't want
		 * to fetch then we can mark the ref entry in the list
		 * as one to ignore by setting util to NULL.
		 */
		إذا (دوسر_with(ref->name, "^{}")) {
			إذا (قلم &&
			    !odb_has_object(the_repository->objects, &ref->old_oid, 0) &&
			    !oidset_شامل_است(&fetch_oids, &ref->old_oid) &&
			    !odb_has_object(the_repository->objects, &قلم->oid, 0) &&
			    !oidset_شامل_است(&fetch_oids, &قلم->oid))
				clear_قلم(قلم);
			قلم = عدم;
			استمر;
		}

		/*
		 * If item is non-NULL here, then we previously saw a
		 * ref not followed by a peeled reference, so we need
		 * to check if it is a lightweight tag that we want to
		 * fetch.
		 */
		إذا (قلم &&
		    !odb_has_object(the_repository->objects, &قلم->oid, 0) &&
		    !oidset_شامل_است(&fetch_oids, &قلم->oid))
			clear_قلم(قلم);

		قلم = عدم;

		/* skip duplicates and refs that we already have */
		إذا (refname_hash_exists(&remote_refs, ref->name) ||
		    refname_hash_exists(&existing_refs, ref->name))
			استمر;

		قلم = refname_hash_جمع_کن(&remote_refs, ref->name, &ref->old_oid);
		string_list_درج(&remote_refs_list, ref->name);
	}
	hashmap_clear_and_free(&existing_refs, بنية refname_hash_مدخل, ent);

	/*
	 * We may have a final lightweight tag that needs to be
	 * checked to see if it needs fetching.
	 */
	إذا (قلم &&
	    !odb_has_object(the_repository->objects, &قلم->oid, 0) &&
	    !oidset_شامل_است(&fetch_oids, &قلم->oid))
		clear_قلم(قلم);

	/*
	 * For all the tags in the remote_refs_list,
	 * add them to the list of refs to be fetched
	 */
	for_each_string_list_قلم(remote_ref_قلم, &remote_refs_list) {
		ثابت حرف *refname = remote_ref_قلم->string;
		بنية ref *rm;
		بیعلامت صحيح hash = strhash(refname);

		قلم = hashmap_get_مدخل_from_hash(&remote_refs, hash, refname,
					بنية refname_hash_مدخل, ent);
		إذا (!قلم)
			BUG("unseen remote ref?");

		/* Unless we have already decided to ignore this item... */
		إذا (قلم->ignore)
			استمر;

		rm = alloc_ref(قلم->refname);
		rm->peer_ref = alloc_ref(قلم->refname);
		oidcpy(&rm->old_oid, &قلم->oid);
		**دم = rm;
		*دم = &rm->بعدی;
	}
	hashmap_clear_and_free(&remote_refs, بنية refname_hash_مدخل, ent);
	string_list_clear(&remote_refs_list, 0);
	oidset_clear(&fetch_oids);
}

ساكن فراغ filter_prefetch_refspec(بنية refspec *rs)
{
	صحيح i;

	إذا (!prefetch)
		أرجع;

	لكل (i = 0; i < rs->nr; i++) {
		بنية strbuf new_dst = STRBUF_INIT;
		حرف *old_dst;
		ثابت حرف *sub = عدم;

		إذا (rs->اقلام[i].negative)
			استمر;
		إذا (!rs->اقلام[i].dst ||
		    (rs->اقلام[i].src &&
		     starts_with(rs->اقلام[i].src,
				 ref_namespace[NAMESPACE_برچسبها].ref))) {
			صحيح j;

			refspec_قلم_clear(&rs->اقلام[i]);

			لكل (j = i + 1; j < rs->nr; j++)
				rs->اقلام[j - 1] = rs->اقلام[j];
			rs->nr--;
			i--;
			استمر;
		}

		old_dst = rs->اقلام[i].dst;
		strbuf_addstr(&new_dst, ref_namespace[NAMESPACE_PREFETCH].ref);

		/*
		 * If old_dst starts with "refs/", then place
		 * sub after that prefix. Otherwise, start at
		 * the beginning of the string.
		 */
		إذا (!skip_پیشوند(old_dst, "refs/", &sub))
			sub = old_dst;
		strbuf_addstr(&new_dst, sub);

		rs->اقلام[i].dst = strbuf_detach(&new_dst, عدم);
		rs->اقلام[i].force = 1;

		حرر(old_dst);
	}
}

ساكن بنية ref *get_ref_map(بنية remote *remote,
			       ثابت بنية ref *remote_refs,
			       بنية refspec *rs,
			       صحيح برچسبها, صحيح *autotags)
{
	صحيح i;
	بنية ref *rm;
	بنية ref *ref_map = عدم;
	بنية ref **دم = &ref_map;

	/* opportunistically-updated references: */
	بنية ref *orefs = عدم, **oref_دم = &orefs;

	بنية hashmap existing_refs;
	صحيح existing_refs_populated = 0;

	filter_prefetch_refspec(rs);
	إذا (remote)
		filter_prefetch_refspec(&remote->fetch);

	إذا (rs->nr) {
		بنية refspec *fetch_refspec;

		لكل (i = 0; i < rs->nr; i++) {
			get_fetch_map(remote_refs, &rs->اقلام[i], &دم, 0);
			إذا (rs->اقلام[i].dst && rs->اقلام[i].dst[0])
				*autotags = 1;
		}
		/* Merge everything on the command line (but not --tags) */
		لكل (rm = ref_map; rm; rm = rm->بعدی)
			rm->fetch_سر_وضعیت = FETCH_سر_ادغام;

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
		إذا (refmap.nr)
			fetch_refspec = &refmap;
		وإلا
			fetch_refspec = &remote->fetch;

		لكل (i = 0; i < fetch_refspec->nr; i++)
			get_fetch_map(ref_map, &fetch_refspec->اقلام[i], &oref_دم, 1);
	} وإلا إذا (refmap.nr) {
		die("--refmap option is only meaningful with command-line refspec(s)");
	} وإلا {
		/* Use the defaults */
		بنية branch *branch = branch_get(عدم);
		صحيح has_ادغام = branch_has_ادغام_config(branch);
		إذا (remote &&
		    (remote->fetch.nr ||
		     /* Note: has_merge implies non-NULL branch->remote_name */
		     (has_ادغام && !strcmp(branch->remote_name, remote->name)))) {
			لكل (i = 0; i < remote->fetch.nr; i++) {
				get_fetch_map(remote_refs, &remote->fetch.اقلام[i], &دم, 0);
				إذا (remote->fetch.اقلام[i].dst &&
				    remote->fetch.اقلام[i].dst[0])
					*autotags = 1;
				إذا (!i && !has_ادغام && ref_map &&
				    !remote->fetch.اقلام[0].الگو)
					ref_map->fetch_سر_وضعیت = FETCH_سر_ادغام;
			}
			/*
			 * if the remote we're fetching from is the same
			 * as given in branch.<name>.remote, we add the
			 * ref given in branch.<name>.merge, too.
			 *
			 * Note: has_merge implies non-NULL branch->remote_name
			 */
			إذا (has_ادغام &&
			    !strcmp(branch->remote_name, remote->name))
				جمع_کن_ادغام_config(&ref_map, remote_refs, branch, &دم);
		} وإلا إذا (!prefetch) {
			ref_map = get_remote_ref(remote_refs, "HEAD");
			إذا (!ref_map)
				die(_("couldn't find remote ref HEAD"));
			ref_map->fetch_سر_وضعیت = FETCH_سر_ادغام;
			دم = &ref_map->بعدی;
		}
	}

	إذا (برچسبها == برچسبها_مجموعه) {
		بنية refspec_قلم برچسب_refspec;

		/* also fetch all tags */
		refspec_قلم_init_افزودن(&برچسب_refspec, برچسب_REFSPEC,
				       the_hash_algo);
		get_fetch_map(remote_refs, &برچسب_refspec, &دم, 0);
		refspec_قلم_clear(&برچسب_refspec);
	} وإلا إذا (برچسبها == برچسبها_DEFAULT && *autotags) {
		پیدا_کن_non_local_برچسبها(remote_refs, عدم, &ref_map, &دم);
	}

	/* Now append any refs to be updated opportunistically: */
	*دم = orefs;
	لكل (rm = orefs; rm; rm = rm->بعدی) {
		rm->fetch_سر_وضعیت = FETCH_سر_IGNORE;
		دم = &rm->بعدی;
	}

	/*
	 * apply negative refspecs first, before we remove duplicates. This is
	 * necessary as negative refspecs might remove an otherwise conflicting
	 * duplicate.
	 */
	إذا (rs->nr)
		ref_map = apply_negative_refspecs(ref_map, rs);
	وإلا
		ref_map = apply_negative_refspecs(ref_map, &remote->fetch);

	ref_map = ref_remove_duplicates(ref_map);

	لكل (rm = ref_map; rm; rm = rm->بعدی) {
		إذا (rm->peer_ref) {
			ثابت حرف *refname = rm->peer_ref->name;
			بنية refname_hash_مدخل *peer_قلم;
			بیعلامت صحيح hash = strhash(refname);

			إذا (!existing_refs_populated) {
				refname_hash_init(&existing_refs);
				refs_for_each_ref(get_main_ref_store(the_repository),
						  جمع_کن_one_refname,
						  &existing_refs);
				existing_refs_populated = 1;
			}

			peer_قلم = hashmap_get_مدخل_from_hash(&existing_refs,
						hash, refname,
						بنية refname_hash_مدخل, ent);
			إذا (peer_قلم) {
				بنية object_id *old_oid = &peer_قلم->oid;
				oidcpy(&rm->peer_ref->old_oid, old_oid);
			}
		}
	}
	إذا (existing_refs_populated)
		hashmap_clear_and_free(&existing_refs, بنية refname_hash_مدخل, ent);

	أرجع ref_map;
}

ساكن صحيح s_update_ref(ثابت حرف *action,
			بنية ref *ref,
			بنية ref_transaction *transaction,
			صحيح check_old)
{
	حرف *پیام;
	حرف *rla = getenv("GIT_REFLOG_ACTION");
	بنية strbuf err = STRBUF_INIT;
	صحيح ret;

	إذا (dry_پیاپی)
		أرجع 0;
	إذا (!rla)
		rla = default_rla.buf;
	پیام = xstrfmt("%s: %s", rla, action);

	ret = ref_transaction_update(transaction, ref->name, &ref->new_oid,
				     check_old ? &ref->old_oid : عدم,
				     عدم, عدم, 0, پیام, &err);

	إذا (ret)
		error("%s", err.buf);
	strbuf_release(&err);
	حرر(پیام);
	أرجع ret;
}

ساكن صحيح refcol_عرض(ثابت بنية ref *ref_map, صحيح compact_format)
{
	ثابت بنية ref *ref;
	صحيح max, عرض = 10;

	max = جمله_columns();
	إذا (compact_format)
		max = max * 2 / 3;

	لكل (ref = ref_map; ref; ref = ref->بعدی) {
		صحيح rlen, llen = 0, len;

		إذا (ref->وضعیت == REF_وضعیت_REJECT_SHALLOW ||
		    !ref->peer_ref ||
		    !strcmp(ref->name, "HEAD"))
			استمر;

		/* uptodate lines are only shown on high verbosity level */
		إذا (verbosity <= 0 && oideq(&ref->peer_ref->old_oid, &ref->old_oid))
			استمر;

		rlen = utf8_strwidth(prettify_refname(ref->name));
		إذا (!compact_format)
			llen = utf8_strwidth(prettify_refname(ref->peer_ref->name));

		/*
		 * rough estimation to see if the output line is too long and
		 * should not be counted (we can't do precise calculation
		 * anyway because we don't know if the error explanation part
		 * will be printed in update_local_ref)
		 */
		len = 21 /* flag and summary */ + rlen + 4 /* -> */ + llen;
		إذا (len >= max)
			استمر;

		إذا (عرض < rlen)
			عرض = rlen;
	}

	أرجع عرض;
}

ساكن فراغ display_state_init(بنية display_state *display_state, بنية ref *ref_map,
			       ثابت حرف *raw_url, تعداد display_format format)
{
	صحيح i;

	memset(display_state, 0, حجم(*display_state));
	strbuf_init(&display_state->buf, 0);
	display_state->format = format;

	إذا (raw_url)
		display_state->url = transport_anonymize_url(raw_url);
	وإلا
		display_state->url = xstrdup("foreign");

	display_state->url_len = طول_رشته(display_state->url);
	لكل (i = display_state->url_len - 1; 0 <= i && display_state->url[i] == '/'; i--)
		;
	display_state->url_len = i + 1;
	إذا (4 < i && !مقایسه_رشته(".git", display_state->url + i - 3, 4))
		display_state->url_len = i - 3;

	إذا (verbosity < 0)
		أرجع;

	بدل (display_state->format) {
	حالة DISPLAY_FORMAT_پر:
	حالة DISPLAY_FORMAT_COMPACT:
		display_state->refcol_عرض = refcol_عرض(ref_map,
							   display_state->format == DISPLAY_FORMAT_COMPACT);
		اكسر;
	حالة DISPLAY_FORMAT_PORCELAIN:
		/* We don't need to precompute anything here. */
		اكسر;
	افتراضي:
		BUG("unexpected display format %d", display_state->format);
	}
}

ساكن فراغ display_state_release(بنية display_state *display_state)
{
	strbuf_release(&display_state->buf);
	حرر(display_state->url);
}

ساكن فراغ print_remote_مقصد_local(بنية display_state *display_state,
				  ثابت حرف *remote, ثابت حرف *local)
{
	strbuf_addf(&display_state->buf, "%-*s -> %s",
		    display_state->refcol_عرض, remote, local);
}

ساكن صحيح پیدا_کن_and_replace(بنية strbuf *haystack,
			    ثابت حرف *needle,
			    ثابت حرف *placeholder)
{
	ثابت حرف *p = عدم;
	صحيح plen, nlen;

	nlen = طول_رشته(needle);
	إذا (دوسر_with(haystack->buf, needle))
		p = haystack->buf + haystack->len - nlen;
	وإلا
		p = جستجوی_رشته(haystack->buf, needle);
	إذا (!p)
		أرجع 0;

	إذا (p > haystack->buf && p[-1] != '/')
		أرجع 0;

	plen = طول_رشته(p);
	إذا (plen > nlen && p[nlen] != '/')
		أرجع 0;

	strbuf_splice(haystack, p - haystack->buf, nlen,
		      placeholder, طول_رشته(placeholder));
	أرجع 1;
}

ساكن فراغ print_compact(بنية display_state *display_state,
			  ثابت حرف *remote, ثابت حرف *local)
{
	بنية strbuf r = STRBUF_INIT;
	بنية strbuf l = STRBUF_INIT;

	إذا (!strcmp(remote, local)) {
		strbuf_addf(&display_state->buf, "%-*s -> *", display_state->refcol_عرض, remote);
		أرجع;
	}

	strbuf_addstr(&r, remote);
	strbuf_addstr(&l, local);

	إذا (!پیدا_کن_and_replace(&r, local, "*"))
		پیدا_کن_and_replace(&l, remote, "*");
	print_remote_مقصد_local(display_state, r.buf, l.buf);

	strbuf_release(&r);
	strbuf_release(&l);
}

ساكن فراغ display_ref_update(بنية display_state *display_state, حرف کد,
			       ثابت حرف *summary, ثابت حرف *error,
			       ثابت حرف *remote, ثابت حرف *local,
			       ثابت بنية object_id *old_oid,
			       ثابت بنية object_id *new_oid,
			       صحيح summary_عرض)
{
	FILE *f = stderr;

	إذا (verbosity < 0)
		أرجع;

	strbuf_reset(&display_state->buf);

	بدل (display_state->format) {
	حالة DISPLAY_FORMAT_پر:
	حالة DISPLAY_FORMAT_COMPACT: {
		صحيح عرض;

		إذا (!display_state->shown_url) {
			strbuf_addf(&display_state->buf, _("From %.*s\n"),
				    display_state->url_len, display_state->url);
			display_state->shown_url = 1;
		}

		عرض = (summary_عرض + طول_رشته(summary) - gettext_عرض(summary));
		remote = prettify_refname(remote);
		local = prettify_refname(local);

		strbuf_addf(&display_state->buf, " %c %-*s ", کد, عرض, summary);

		إذا (display_state->format != DISPLAY_FORMAT_COMPACT)
			print_remote_مقصد_local(display_state, remote, local);
		وإلا
			print_compact(display_state, remote, local);

		إذا (error)
			strbuf_addf(&display_state->buf, "  (%s)", error);

		اكسر;
	}
	حالة DISPLAY_FORMAT_PORCELAIN:
		strbuf_addf(&display_state->buf, "%c %s %s %s", کد,
			    oid_مقصد_hex(old_oid), oid_مقصد_hex(new_oid), local);
		f = stdout;
		اكسر;
	افتراضي:
		BUG("unexpected display format %d", display_state->format);
	};
	strbuf_addch(&display_state->buf, '\n');

	fputs(display_state->buf.buf, f);
}

بنية ref_update_display_info {
	bool failed;
	حرف success_کد;
	حرف fail_کد;
	حرف *summary;
	حرف *fail_detail;
	حرف *success_detail;
	حرف *ref;
	حرف *remote;
	بنية object_id old_oid;
	بنية object_id new_oid;
};

بنية ref_update_display_info_array {
	بنية ref_update_display_info *info;
	اندازه_t alloc, nr;
};

ساكن بنية ref_update_display_info *ref_update_display_info_append(
					   بنية ref_update_display_info_array *array,
					   حرف success_کد,
					   حرف fail_کد,
					   ثابت حرف *summary,
					   ثابت حرف *success_detail,
					   ثابت حرف *fail_detail,
					   ثابت حرف *ref,
					   ثابت حرف *remote,
					   ثابت بنية object_id *old_oid,
					   ثابت بنية object_id *new_oid)
{
	بنية ref_update_display_info *info;

	ALLOC_GROW(array->info, array->nr + 1, array->alloc);
	info = &array->info[array->nr++];

	info->failed = false;
	info->success_کد = success_کد;
	info->fail_کد = fail_کد;
	info->summary = xstrdup(summary);
	info->success_detail = xstrdup_or_null(success_detail);
	info->fail_detail = xstrdup_or_null(fail_detail);
	info->remote = xstrdup(remote);
	info->ref = xstrdup(ref);

	oidcpy(&info->old_oid, old_oid);
	oidcpy(&info->new_oid, new_oid);

	أرجع info;
}

ساكن فراغ ref_update_display_info_مجموعه_failed(بنية ref_update_display_info *info)
{
	info->failed = true;
}

ساكن فراغ ref_update_display_info_free(بنية ref_update_display_info *info)
{
	حرر(info->summary);
	حرر(info->success_detail);
	حرر(info->fail_detail);
	حرر(info->remote);
	حرر(info->ref);
}

ساكن فراغ ref_update_display_info_display(بنية ref_update_display_info *info,
					    بنية display_state *display_state,
					    صحيح summary_عرض)
{
	display_ref_update(display_state,
			   info->failed ? info->fail_کد : info->success_کد,
			   info->summary,
			   info->failed ? info->fail_detail : info->success_detail,
			   info->remote, info->ref, &info->old_oid,
			   &info->new_oid, summary_عرض);
}

ساكن صحيح update_local_ref(بنية ref *ref,
			    بنية ref_transaction *transaction,
			    ثابت بنية ref *remote_ref,
			    ثابت بنية fetch_config *config,
			    بنية ref_update_display_info_array *display_array)
{
	بنية commit *current = عدم, *updated;
	صحيح fast_forward = 0;

	إذا (!odb_has_object(the_repository->objects, &ref->new_oid,
			    ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR))
		die(_("object %s not found"), oid_مقصد_hex(&ref->new_oid));

	إذا (oideq(&ref->old_oid, &ref->new_oid)) {
		إذا (verbosity > 0)
			ref_update_display_info_append(display_array, '=', '=',
						       _("[up to date]"), عدم,
						       عدم, ref->name,
						       remote_ref->name, &ref->old_oid,
						       &ref->new_oid);
		أرجع 0;
	}

	إذا (!update_سر_ok &&
	    !is_null_oid(&ref->old_oid) &&
	    branch_checked_بیرون(ref->name)) {
		بنية ref_update_display_info *info;
		/*
		 * If this is the head, and it's not okay to update
		 * the head, and the old value of the head isn't empty...
		 */
		info = ref_update_display_info_append(display_array, '!', '!',
						      _("[rejected]"), عدم,
						      _("can't fetch into checked-out branch"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);
		ref_update_display_info_مجموعه_failed(info);
		أرجع 1;
	}

	إذا (!is_null_oid(&ref->old_oid) &&
	    starts_with(ref->name, "refs/tags/")) {
		بنية ref_update_display_info *info;

		إذا (force || ref->force) {
			صحيح r;

			r = s_update_ref("updating tag", ref, transaction, 0);

			info = ref_update_display_info_append(display_array, 't', '!',
							      _("[tag update]"), عدم,
							      _("unable to update local ref"),
							      ref->name, remote_ref->name,
							      &ref->old_oid, &ref->new_oid);
			إذا (r)
				ref_update_display_info_مجموعه_failed(info);

			أرجع r;
		} وإلا {
			info = ref_update_display_info_append(display_array, '!', '!',
							      _("[rejected]"), عدم,
							      _("would clobber existing tag"),
							      ref->name, remote_ref->name,
							      &ref->old_oid, &ref->new_oid);
			ref_update_display_info_مجموعه_failed(info);
			أرجع 1;
		}
	}

	current = lookup_commit_reference_gently(the_repository,
						 &ref->old_oid, 1);
	updated = lookup_commit_reference_gently(the_repository,
						 &ref->new_oid, 1);
	إذا (!current || !updated) {
		بنية ref_update_display_info *info;
		ثابت حرف *پیام;
		ثابت حرف *what;
		صحيح r;
		/*
		 * Nicely describe the new ref we're fetching.
		 * Base this on the remote's ref name, as it's
		 * more likely to follow a standard layout.
		 */
		إذا (starts_with(remote_ref->name, "refs/tags/")) {
			پیام = "storing tag";
			what = _("[new tag]");
		} وإلا إذا (starts_with(remote_ref->name, "refs/heads/")) {
			پیام = "storing head";
			what = _("[new branch]");
		} وإلا {
			پیام = "storing ref";
			what = _("[new ref]");
		}

		r = s_update_ref(پیام, ref, transaction, 0);

		info = ref_update_display_info_append(display_array, '*', '!',
						      what, عدم,
						      _("unable to update local ref"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);
		إذا (r)
			ref_update_display_info_مجموعه_failed(info);

		أرجع r;
	}

	إذا (config->show_forced_updates) {
		uint64_t t_before = getnanotime();
		fast_forward = repo_درون_ادغام_bases(the_repository, current,
						   updated);
		إذا (fast_forward < 0)
			die(عدم);
		forced_updates_ms += (getnanotime() - t_before) / 1000000;
	} وإلا {
		fast_forward = 1;
	}

	إذا (fast_forward) {
		بنية ref_update_display_info *info;
		بنية strbuf quickref = STRBUF_INIT;
		صحيح r;

		strbuf_جمع_کن_unique_abbrev(&quickref, &current->object.oid, DEFAULT_ABBREV);
		strbuf_addstr(&quickref, "..");
		strbuf_جمع_کن_unique_abbrev(&quickref, &ref->new_oid, DEFAULT_ABBREV);
		r = s_update_ref("fast-forward", ref, transaction, 1);

		info = ref_update_display_info_append(display_array, ' ', '!',
						      quickref.buf, عدم,
						      _("unable to update local ref"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);
		إذا (r)
			ref_update_display_info_مجموعه_failed(info);

		strbuf_release(&quickref);
		أرجع r;
	} وإلا إذا (force || ref->force) {
		بنية ref_update_display_info *info;
		بنية strbuf quickref = STRBUF_INIT;
		صحيح r;

		strbuf_جمع_کن_unique_abbrev(&quickref, &current->object.oid, DEFAULT_ABBREV);
		strbuf_addstr(&quickref, "...");
		strbuf_جمع_کن_unique_abbrev(&quickref, &ref->new_oid, DEFAULT_ABBREV);
		r = s_update_ref("forced-update", ref, transaction, 1);

		info = ref_update_display_info_append(display_array, '+', '!',
						      quickref.buf, _("forced update"),
						      _("unable to update local ref"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);

		إذا (r)
			ref_update_display_info_مجموعه_failed(info);

		strbuf_release(&quickref);
		أرجع r;
	} وإلا {
		بنية ref_update_display_info *info;
		info = ref_update_display_info_append(display_array, '!', '!',
						      _("[rejected]"), عدم,
						      _("non-fast-forward"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);
		ref_update_display_info_مجموعه_failed(info);
		أرجع 1;
	}
}

ساكن ثابت بنية object_id *iterate_ref_map(فراغ *cb_داده)
{
	بنية ref **rm = cb_داده;
	بنية ref *ref = *rm;

	طالما (ref && ref->وضعیت == REF_وضعیت_REJECT_SHALLOW)
		ref = ref->بعدی;
	إذا (!ref)
		أرجع عدم;
	*rm = ref->بعدی;
	أرجع &ref->old_oid;
}

بنية fetch_سر {
	FILE *fp;
	بنية strbuf buf;
};

ساكن صحيح open_fetch_سر(بنية fetch_سر *fetch_سر)
{
	ثابت حرف *filename = git_مسیر_fetch_سر(the_repository);

	إذا (write_fetch_سر) {
		fetch_سر->fp = fopen(filename, "a");
		إذا (!fetch_سر->fp)
			أرجع error_errno(_("cannot open '%s'"), filename);
		strbuf_init(&fetch_سر->buf, 0);
	} وإلا {
		fetch_سر->fp = عدم;
	}

	أرجع 0;
}

ساكن فراغ append_fetch_سر(بنية fetch_سر *fetch_سر,
			      ثابت بنية object_id *old_oid,
			      تعداد fetch_سر_وضعیت fetch_سر_وضعیت,
			      ثابت حرف *note,
			      ثابت حرف *url, اندازه_t url_len)
{
	حرف old_oid_hex[GIT_MAX_HEXSZ + 1];
	ثابت حرف *ادغام_وضعیت_marker;
	اندازه_t i;

	إذا (!fetch_سر->fp)
		أرجع;

	بدل (fetch_سر_وضعیت) {
	حالة FETCH_سر_NOT_FOR_ادغام:
		ادغام_وضعیت_marker = "not-for-merge";
		اكسر;
	حالة FETCH_سر_ادغام:
		ادغام_وضعیت_marker = "";
		اكسر;
	افتراضي:
		/* do not write anything to FETCH_HEAD */
		أرجع;
	}

	strbuf_addf(&fetch_سر->buf, "%s\t%s\t%s",
		    oid_مقصد_hex_r(old_oid_hex, old_oid), ادغام_وضعیت_marker, note);
	لكل (i = 0; i < url_len; ++i)
		إذا ('\n' == url[i])
			strbuf_addstr(&fetch_سر->buf, "\\n");
		وإلا
			strbuf_addch(&fetch_سر->buf, url[i]);
	strbuf_addch(&fetch_سر->buf, '\n');

	/*
	 * When using an atomic fetch, we do not want to update FETCH_HEAD if
	 * any of the reference updates fails. We thus have to write all
	 * updates to a buffer first and only commit it as soon as all
	 * references have been successfully updated.
	 */
	إذا (!atomic_fetch) {
		strbuf_write(&fetch_سر->buf, fetch_سر->fp);
		strbuf_reset(&fetch_سر->buf);
	}
}

ساكن فراغ commit_fetch_سر(بنية fetch_سر *fetch_سر)
{
	إذا (!fetch_سر->fp || !atomic_fetch)
		أرجع;
	strbuf_write(&fetch_سر->buf, fetch_سر->fp);
}

ساكن فراغ close_fetch_سر(بنية fetch_سر *fetch_سر)
{
	إذا (!fetch_سر->fp)
		أرجع;

	fclose(fetch_سر->fp);
	strbuf_release(&fetch_سر->buf);
}

ساكن ثابت حرف warn_show_forced_updates[] =
N_("fetch normally indicates which branches had a forced update,\n"
   "but that check has been disabled; to re-enable, use '--show-forced-updates'\n"
   "flag or run 'git config fetch.showForcedUpdates true'");
ساكن ثابت حرف warn_زمان_show_forced_updates[] =
N_("it took %.2f seconds to check forced updates; you can use\n"
   "'--no-show-forced-updates' or run 'git config fetch.showForcedUpdates false'\n"
   "to avoid this check\n");

ساكن صحيح store_updated_refs(بنية display_state *display_state,
			      صحيح connectivity_checked,
			      بنية ref_transaction *transaction, بنية ref *ref_map,
			      بنية fetch_سر *fetch_سر,
			      ثابت بنية fetch_config *config,
			      بنية ref_update_display_info_array *display_array)
{
	صحيح rc = 0;
	بنية strbuf note = STRBUF_INIT;
	ثابت حرف *what, *نوع;
	بنية ref *rm;
	صحيح want_وضعیت;

	إذا (!connectivity_checked) {
		بنية check_connected_options opt = CHECK_CONNECTED_INIT;

		opt.exclude_hidden_refs_section = "fetch";
		rm = ref_map;
		إذا (check_connected(iterate_ref_map, &rm, &opt)) {
			rc = error(_("%s did not send all necessary objects"),
				   display_state->url);
			اذهب abort;
		}
	}

	/*
	 * We do a pass for each fetch_head_status type in their enum order, so
	 * merged entries are written before not-for-merge. That lets readers
	 * use FETCH_HEAD as a refname to refer to the ref to be merged.
	 */
	لكل (want_وضعیت = FETCH_سر_ادغام;
	     want_وضعیت <= FETCH_سر_IGNORE;
	     want_وضعیت++) {
		لكل (rm = ref_map; rm; rm = rm->بعدی) {
			بنية ref *ref = عدم;

			إذا (rm->وضعیت == REF_وضعیت_REJECT_SHALLOW) {
				إذا (want_وضعیت == FETCH_سر_ادغام)
					warning(_("rejected %s because shallow roots are not allowed to be updated"),
						rm->peer_ref ? rm->peer_ref->name : rm->name);
				استمر;
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
			إذا (fetch_سر->fp) {
				بنية commit *commit = عدم;

				/*
				 * References in "refs/tags/" are often going to point
				 * to annotated tags, which are not part of the
				 * commit-graph. We thus only try to look up refs in
				 * the graph which are not in that namespace to not
				 * regress performance in repositories with many
				 * annotated tags.
				 */
				إذا (!starts_with(rm->name, "refs/tags/"))
					commit = lookup_commit_درون_گراف(the_repository, &rm->old_oid);
				إذا (!commit) {
					commit = lookup_commit_reference_gently(the_repository,
										&rm->old_oid,
										1);
					إذا (!commit)
						rm->fetch_سر_وضعیت = FETCH_سر_NOT_FOR_ادغام;
				}
			}

			إذا (rm->fetch_سر_وضعیت != want_وضعیت)
				استمر;

			إذا (rm->peer_ref) {
				ref = alloc_ref(rm->peer_ref->name);
				oidcpy(&ref->old_oid, &rm->peer_ref->old_oid);
				oidcpy(&ref->new_oid, &rm->old_oid);
				ref->force = rm->peer_ref->force;
			}

			إذا (config->recurse_submodules != RECURSE_SUBMODULES_OFF &&
			    (!rm->peer_ref || !oideq(&ref->old_oid, &ref->new_oid))) {
				check_for_new_submodule_commits(&rm->old_oid);
			}

			إذا (!strcmp(rm->name, "HEAD")) {
				نوع = "";
				what = "";
			} وإلا إذا (skip_پیشوند(rm->name, "refs/heads/", &what)) {
				نوع = "branch";
			} وإلا إذا (skip_پیشوند(rm->name, "refs/tags/", &what)) {
				نوع = "tag";
			} وإلا إذا (skip_پیشوند(rm->name, "refs/remotes/", &what)) {
				نوع = "remote-tracking branch";
			} وإلا {
				نوع = "";
				what = rm->name;
			}

			strbuf_reset(&note);
			إذا (*what) {
				إذا (*نوع)
					strbuf_addf(&note, "%s ", نوع);
				strbuf_addf(&note, "'%s' of ", what);
			}

			append_fetch_سر(fetch_سر, &rm->old_oid,
					  rm->fetch_سر_وضعیت,
					  note.buf, display_state->url,
					  display_state->url_len);

			إذا (ref) {
				rc |= update_local_ref(ref, transaction, rm,
						       config, display_array);
				حرر(ref);
			} وإلا إذا (write_fetch_سر || dry_پیاپی) {
				/*
				 * Display fetches written to FETCH_HEAD (or
				 * would be written to FETCH_HEAD, if --dry-run
				 * is set).
				 */

				ref_update_display_info_append(display_array, '*', '*',
							       *نوع ? نوع : "branch",
							       عدم, عدم, "FETCH_HEAD",
							       rm->name, &rm->new_oid,
							       &rm->old_oid);
			}
		}
	}

	إذا (advice_enabled(ADVICE_FETCH_SHOW_FORCED_UPDATES)) {
		إذا (!config->show_forced_updates) {
			warning(_(warn_show_forced_updates));
		} وإلا إذا (forced_updates_ms > FORCED_UPDATES_DELAY_WARNING_درون_MS) {
			warning(_(warn_زمان_show_forced_updates),
				forced_updates_ms / 1000.0);
		}
	}

 abort:
	strbuf_release(&note);
	أرجع rc;
}

/*
 * We would want to bypass the object transfer altogether if
 * everything we are going to fetch already exists and is connected
 * locally.
 */
ساكن صحيح check_exist_and_connected(بنية ref *ref_map)
{
	بنية ref *rm = ref_map;
	بنية check_connected_options opt = CHECK_CONNECTED_INIT;
	بنية ref *r;

	/*
	 * If we are deepening a shallow clone we already have these
	 * objects reachable.  Running rev-list here will return with
	 * a good (0) exit status and we'll bypass the fetch that we
	 * really need to perform.  Claiming failure now will ensure
	 * we perform the network exchange to deepen our history.
	 */
	إذا (deepen)
		أرجع -1;

	/*
	 * Similarly, if we need to refetch, we always want to perform a full
	 * fetch ignoring existing objects.
	 */
	إذا (refetch)
		أرجع -1;


	/*
	 * check_connected() allows objects to merely be promised, but
	 * we need all direct targets to exist.
	 */
	لكل (r = rm; r; r = r->بعدی) {
		إذا (!odb_has_object(the_repository->objects, &r->old_oid,
				    ODB_HAS_OBJECT_RECHECK_PACKED))
			أرجع -1;
	}

	opt.quiet = 1;
	opt.exclude_hidden_refs_section = "fetch";
	أرجع check_connected(iterate_ref_map, &rm, &opt);
}

ساكن صحيح fetch_and_consume_refs(بنية display_state *display_state,
				  بنية transport *transport,
				  بنية ref_transaction *transaction,
				  بنية ref *ref_map,
				  بنية fetch_سر *fetch_سر,
				  ثابت بنية fetch_config *config,
				  بنية ref_update_display_info_array *display_array)
{
	صحيح connectivity_checked = 1;
	صحيح ret;

	/*
	 * We don't need to perform a fetch in case we can already satisfy all
	 * refs.
	 */
	ret = check_exist_and_connected(ref_map);
	إذا (ret) {
		trace2_region_enter("fetch", "fetch_refs", the_repository);
		ret = transport_fetch_refs(transport, ref_map);
		trace2_region_leave("fetch", "fetch_refs", the_repository);
		إذا (ret)
			اذهب بیرون;
		connectivity_checked = transport->smart_options ?
			transport->smart_options->connectivity_checked : 0;
	}

	trace2_region_enter("fetch", "consume_refs", the_repository);
	ret = store_updated_refs(display_state, connectivity_checked,
				 transaction, ref_map, fetch_سر, config,
				 display_array);
	trace2_region_leave("fetch", "consume_refs", the_repository);

بیرون:
	transport_unlock_pack(transport, 0);
	أرجع ret;
}

ساكن صحيح prune_refs(بنية display_state *display_state,
		      بنية refspec *rs,
		      بنية ref_transaction *transaction,
		      بنية ref *ref_map)
{
	صحيح نتیجه = 0;
	بنية ref *ref, *stale_refs = get_stale_heads(rs, ref_map);
	بنية strbuf err = STRBUF_INIT;
	بنية string_list refnames = STRING_LIST_INIT_NODUP;

	لكل (ref = stale_refs; ref; ref = ref->بعدی)
		string_list_append(&refnames, ref->name);

	إذا (!dry_پیاپی) {
		إذا (transaction) {
			لكل (ref = stale_refs; ref; ref = ref->بعدی) {
				نتیجه = ref_transaction_delete(transaction, ref->name, عدم,
								عدم, 0, "fetch: prune", &err);
				إذا (نتیجه)
					اذهب cleanup;
			}
		} وإلا {
			نتیجه = refs_delete_refs(get_main_ref_store(the_repository),
						  "fetch: prune", &refnames,
						  0);
		}
	}

	إذا (verbosity >= 0) {
		صحيح summary_عرض = transport_summary_عرض(stale_refs);

		لكل (ref = stale_refs; ref; ref = ref->بعدی) {
			display_ref_update(display_state, '-', _("[deleted]"), عدم,
					   _("(none)"), ref->name,
					   &ref->new_oid, &ref->old_oid,
					   summary_عرض);
		}
		string_list_مرتب_سازی(&refnames);
		refs_warn_dangling_symrefs(get_main_ref_store(the_repository),
					   stderr, "   ", dry_پیاپی, &refnames);
	}

cleanup:
	string_list_clear(&refnames, 0);
	strbuf_release(&err);
	free_refs(stale_refs);
	أرجع نتیجه;
}

ساكن فراغ check_not_current_branch(بنية ref *ref_map)
{
	ثابت حرف *مسیر;
	لكل (; ref_map; ref_map = ref_map->بعدی)
		إذا (ref_map->peer_ref &&
		    starts_with(ref_map->peer_ref->name, "refs/heads/") &&
		    (مسیر = branch_checked_بیرون(ref_map->peer_ref->name)))
			die(_("refusing to fetch into branch '%s' "
			      "checked out at '%s'"),
			    ref_map->peer_ref->name, مسیر);
}

ساكن صحيح truncate_fetch_سر(فراغ)
{
	ثابت حرف *filename = git_مسیر_fetch_سر(the_repository);
	FILE *fp = fopen_for_writing(filename);

	إذا (!fp)
		أرجع error_errno(_("cannot open '%s'"), filename);
	fclose(fp);
	أرجع 0;
}

ساكن فراغ مجموعه_option(بنية transport *transport, ثابت حرف *name, ثابت حرف *مقدار)
{
	صحيح r = transport_مجموعه_option(transport, name, مقدار);
	إذا (r < 0)
		die(_("option \"%s\" value \"%s\" is not valid for %s"),
		    name, مقدار, transport->url);
	إذا (r > 0)
		warning(_("option \"%s\" is ignored for %s"),
			name, transport->url);
}


ساكن صحيح جمع_کن_oid(ثابت بنية reference *ref, فراغ *cb_داده)
{
	بنية oid_array *oids = cb_داده;

	oid_array_append(oids, ref->oid);
	أرجع 0;
}

ساكن فراغ جمع_کن_negotiation_tips(بنية string_list *درون_داد_list,
				 بنية oid_array **output_list,
				 ثابت حرف *argname)
{
	بنية oid_array *oids = xcalloc(1, حجم(*oids));
	صحيح i;

	لكل (i = 0; i < درون_داد_list->nr; i++) {
		ثابت حرف *s = درون_داد_list->اقلام[i].string;
		بنية refs_for_each_ref_options opts = {
			.الگو = s,
		};
		صحيح old_nr;
		إذا (!has_glob_specials(s)) {
			بنية object_id oid;

			/* Ignore missing reference. */
			إذا (repo_get_oid(the_repository, s, &oid))
				استمر;
			/* Fail on missing object pointed by ref. */
			إذا (!odb_has_object(the_repository->objects, &oid, 0))
				die(_("the object %s does not exist"), s);

			oid_array_append(oids, &oid);
			استمر;
		}
		old_nr = oids->nr;
		refs_for_each_ref_ext(get_main_ref_store(the_repository),
				      جمع_کن_oid, oids, &opts);
		إذا (old_nr == oids->nr)
			warning(_("ignoring %s=%s because it does not match any refs"),
				argname, s);
	}
	*output_list = oids;
}

ساكن بنية transport *prepare_transport(بنية remote *remote, صحيح deepen,
					   بنية list_objects_filter_options *filter_options)
{
	بنية transport *transport;

	transport = transport_get(remote, عدم);
	transport_مجموعه_verbosity(transport, verbosity, پیشرفت);
	transport->family = family;
	إذا (upload_pack)
		مجموعه_option(transport, TRANS_OPT_UPLOADPACK, upload_pack);
	إذا (keep)
		مجموعه_option(transport, TRANS_OPT_KEEP, "yes");
	إذا (عمق)
		مجموعه_option(transport, TRANS_OPT_عمق, عمق);
	إذا (deepen && deepen_since)
		مجموعه_option(transport, TRANS_OPT_DEEPEN_SINCE, deepen_since);
	إذا (deepen && deepen_not.nr)
		مجموعه_option(transport, TRANS_OPT_DEEPEN_NOT,
			   (ثابت حرف *)&deepen_not);
	إذا (deepen_relative)
		مجموعه_option(transport, TRANS_OPT_DEEPEN_RELATIVE, "yes");
	إذا (update_shallow)
		مجموعه_option(transport, TRANS_OPT_UPDATE_SHALLOW, "yes");
	إذا (refetch)
		مجموعه_option(transport, TRANS_OPT_REFETCH, "yes");
	إذا (filter_options->choice) {
		ثابت حرف *spec =
			expand_list_objects_filter_spec(filter_options);
		مجموعه_option(transport, TRANS_OPT_LIST_OBJECTS_FILTER, spec);
		مجموعه_option(transport, TRANS_OPT_FROM_PROMISOR, "1");
	}
	إذا (negotiation_restrict.nr) {
		إذا (transport->smart_options)
			جمع_کن_negotiation_tips(&negotiation_restrict,
					     &transport->smart_options->negotiation_restrict_tips,
					     "--negotiation-restrict");
		وإلا
			warning(_("ignoring %s because the protocol does not support it"),
				"--negotiation-restrict");
	} وإلا إذا (remote->negotiation_restrict.nr) {
		بنية string_list_قلم *قلم;
		for_each_string_list_قلم(قلم, &remote->negotiation_restrict)
			string_list_append(&negotiation_restrict, قلم->string);
		إذا (transport->smart_options)
			جمع_کن_negotiation_tips(&negotiation_restrict,
					     &transport->smart_options->negotiation_restrict_tips,
					     "--negotiation-restrict");
		وإلا {
			بنية strbuf config_name = STRBUF_INIT;
			strbuf_addf(&config_name, "remote.%s.negotiationRestrict", remote->name);
			warning(_("ignoring %s because the protocol does not support it"),
				config_name.buf);
			strbuf_release(&config_name);
		}
	}
	إذا (negotiation_include.nr) {
		إذا (transport->smart_options)
			جمع_کن_negotiation_tips(&negotiation_include,
					     &transport->smart_options->negotiation_include_tips,
					     "--negotiation-include");
		وإلا
			warning(_("ignoring %s because the protocol does not support it"),
				"--negotiation-include");
	} وإلا إذا (remote->negotiation_include.nr) {
		إذا (transport->smart_options) {
			جمع_کن_negotiation_tips(&remote->negotiation_include,
					     &transport->smart_options->negotiation_include_tips,
					     "--negotiation-include");
		} وإلا {
			بنية strbuf config_name = STRBUF_INIT;
			strbuf_addf(&config_name, "remote.%s.negotiationInclude", remote->name);
			warning(_("ignoring %s because the protocol does not support it"),
				config_name.buf);
			strbuf_release(&config_name);
		}
	}
	أرجع transport;
}

ساكن صحيح backfill_برچسبها(بنية display_state *display_state,
			 بنية transport *transport,
			 بنية ref_transaction *transaction,
			 بنية ref *ref_map,
			 بنية fetch_سر *fetch_سر,
			 ثابت بنية fetch_config *config,
			 بنية ref_update_display_info_array *display_array,
			 بنية list_objects_filter_options *filter_options)
{
	صحيح retcode, cannot_reuse;

	/*
	 * Once we have set TRANS_OPT_DEEPEN_SINCE, we can't unset it
	 * when remote helper is used (setting it to an empty string
	 * is not unsetting). We could extend the remote helper
	 * protocol for that, but for now, just force a new connection
	 * without deepen-since. Similar story for deepen-not.
	 */
	cannot_reuse = transport->cannot_reuse ||
		deepen_since || deepen_not.nr;
	إذا (cannot_reuse) {
		gsecondary = prepare_transport(transport->remote, 0, filter_options);
		transport = gsecondary;
	}

	transport_مجموعه_option(transport, TRANS_OPT_FOLLOWTAGS, عدم);
	transport_مجموعه_option(transport, TRANS_OPT_عمق, "0");
	transport_مجموعه_option(transport, TRANS_OPT_DEEPEN_RELATIVE, عدم);
	retcode = fetch_and_consume_refs(display_state, transport, transaction, ref_map,
					 fetch_سر, config, display_array);

	إذا (gsecondary) {
		transport_disconnect(gsecondary);
		gsecondary = عدم;
	}

	أرجع retcode;
}

ساكن ثابت حرف *strip_refshead(ثابت حرف *name){
	skip_پیشوند(name, "refs/heads/", &name);
	أرجع name;
}

ساكن فراغ مجموعه_سر_advice_پیام(ثابت حرف *remote, ثابت حرف *سر_name)
{
	ثابت حرف پیام_advice_مجموعه_سر[] =
	N_("Run 'git remote set-head %s %s' to follow the change, or modify\n"
	   "either of the 'remote.%s.followRemoteHEAD' or 'fetch.followRemoteHEAD'\n"
	   "configuration variables to handle the situation differently.\n\n"

	   "Using this specific setting\n\n"
	   "    git config set remote.%s.followRemoteHEAD warn-if-not-%s\n\n"
	   "will suppress the warning until the remote changes HEAD to something else.");

	advise_if_enabled(ADVICE_FETCH_مجموعه_سر_WARN, _(پیام_advice_مجموعه_سر),
			remote, سر_name, remote, remote, سر_name);
}

ساكن فراغ warn_مجموعه_سر(ثابت حرف *remote, ثابت حرف *سر_name,
			بنية strbuf *buf_prev, صحيح updateres) {
	بنية strbuf buf_پیشوند = STRBUF_INIT;
	ثابت حرف *prev_سر = عدم;

	strbuf_addf(&buf_پیشوند, "refs/remotes/%s/", remote);
	skip_پیشوند(buf_prev->buf, buf_پیشوند.buf, &prev_سر);

	إذا (prev_سر && strcmp(prev_سر, سر_name)) {
		چاپ_کن("'HEAD' at '%s' is '%s', but we have '%s' locally.\n",
			remote, سر_name, prev_سر);
		مجموعه_سر_advice_پیام(remote, سر_name);
	}
	وإلا إذا (updateres && buf_prev->len) {
		چاپ_کن("'HEAD' at '%s' is '%s', "
			"but we have a detached HEAD pointing to '%s' locally.\n",
			remote, سر_name, buf_prev->buf);
		مجموعه_سر_advice_پیام(remote, سر_name);
	}
	strbuf_release(&buf_پیشوند);
}

ساكن صحيح مجموعه_سر(ثابت بنية ref *remote_refs, بنية remote *remote,
			صحيح follow_remote_سر)
{
	صحيح نتیجه = 0, create_only, baremirror, was_detached;
	بنية strbuf b_سر = STRBUF_INIT, b_remote_سر = STRBUF_INIT,
		      b_local_سر = STRBUF_INIT;
	ثابت حرف *no_warn_branch = remote->no_warn_branch;
	حرف *سر_name = عدم;
	بنية ref *ref, *matches;
	بنية ref *fetch_map = عدم, **fetch_map_دم = &fetch_map;
	بنية refspec_قلم refspec = {
		.force = 0,
		.الگو = 1,
		.src = (حرف *) "refs/heads/*",
		.dst = (حرف *) "refs/heads/*",
	};
	بنية string_list heads = STRING_LIST_INIT_DUP;
	بنية ref_store *refs = get_main_ref_store(the_repository);

	get_fetch_map(remote_refs, &refspec, &fetch_map_دم, 0);
	matches = guess_remote_سر(پیدا_کن_ref_by_name(remote_refs, "HEAD"),
				    fetch_map, REMOTE_GUESS_سر_ALL);
	لكل (ref = matches; ref; ref = ref->بعدی) {
		string_list_append(&heads, strip_refshead(ref->name));
	}

	إذا (!heads.nr)
		نتیجه = 1;
	وإلا إذا (heads.nr > 1)
		نتیجه = 1;
	وإلا
		سر_name = xstrdup(heads.اقلام[0].string);

	إذا (!سر_name)
		اذهب cleanup;
	baremirror = is_bare_repository(the_repository) && remote->mirror;
	create_only = follow_remote_سر == FOLLOW_REMOTE_ALWAYS ? 0 : !baremirror;
	إذا (baremirror) {
		strbuf_addstr(&b_سر, "HEAD");
		strbuf_addf(&b_remote_سر, "refs/heads/%s", سر_name);
	} وإلا {
		strbuf_addf(&b_سر, "refs/remotes/%s/HEAD", remote->name);
		strbuf_addf(&b_remote_سر, "refs/remotes/%s/%s", remote->name, سر_name);
	}
	/* make sure it's valid */
	إذا (!baremirror && !refs_ref_exists(refs, b_remote_سر.buf)) {
		نتیجه = 1;
		اذهب cleanup;
	}
	was_detached = refs_update_symref_extended(refs, b_سر.buf, b_remote_سر.buf,
					"fetch", &b_local_سر, create_only);
	إذا (was_detached == -1) {
		نتیجه = 1;
		اذهب cleanup;
	}
	إذا (verbosity >= 0 &&
		follow_remote_سر == FOLLOW_REMOTE_WARN &&
		(!no_warn_branch || strcmp(no_warn_branch, سر_name)))
		warn_مجموعه_سر(remote->name, سر_name, &b_local_سر, was_detached);

cleanup:
	حرر(سر_name);
	free_refs(fetch_map);
	free_refs(matches);
	string_list_clear(&heads, 0);
	strbuf_release(&b_سر);
	strbuf_release(&b_local_سر);
	strbuf_release(&b_remote_سر);
	أرجع نتیجه;
}

بنية ref_rejection_داده {
	صحيح *retcode;
	bool conflict_پیام_shown;
	bool case_sensitive_پیام_shown;
	ثابت حرف *remote_name;
	بنية strmap *rejected_refs;
};

ساكن فراغ ref_transaction_rejection_handler(ثابت حرف *refname,
					      ثابت بنية object_id *old_oid استفادهنشده,
					      ثابت بنية object_id *new_oid استفادهنشده,
					      ثابت حرف *old_هدف استفادهنشده,
					      ثابت حرف *new_هدف استفادهنشده,
					      تعداد ref_transaction_error err,
					      ثابت حرف *details,
					      فراغ *cb_داده)
{
	بنية ref_rejection_داده *داده = cb_داده;

	إذا (err == REF_TRANSACTION_ERROR_CASE_CONFLICT && repo_ignore_case(the_repository) &&
	    !داده->case_sensitive_پیام_shown) {
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
		داده->case_sensitive_پیام_shown = true;
	} وإلا إذا (err == REF_TRANSACTION_ERROR_NAME_CONFLICT &&
		   !داده->conflict_پیام_shown) {
		error(_("some local refs could not be updated; try running\n"
			" 'git remote prune %s' to remove any old, conflicting "
			"branches"), داده->remote_name);
		داده->conflict_پیام_shown = true;
	} وإلا {
		إذا (details)
			error("%s", details);
		وإلا
			error(_("fetching ref %s failed: %s"),
			      refname, ref_transaction_error_پیام(err));
	}

	strmap_put(داده->rejected_refs, refname, عدم);
	*داده->retcode = 1;
}

/*
 * Commit the reference transaction. If it isn't an atomic transaction, handle
 * rejected updates as part of using batched updates.
 */
ساكن صحيح commit_ref_transaction(بنية ref_transaction **transaction,
				  bool is_atomic, ثابت حرف *remote_name,
				  بنية strmap *rejected_refs,
				  بنية strbuf *err)
{
	صحيح retcode = ref_transaction_commit(*transaction, err);
	إذا (retcode)
		اذهب بیرون;

	إذا (!is_atomic) {
		بنية ref_rejection_داده داده = {
			.conflict_پیام_shown = 0,
			.remote_name = remote_name,
			.retcode = &retcode,
			.rejected_refs = rejected_refs,
		};

		ref_transaction_for_each_rejected_update(*transaction,
							 ref_transaction_rejection_handler,
							 &داده);
	}

بیرون:
	ref_transaction_free(*transaction);
	*transaction = عدم;
	أرجع retcode;
}

ساكن فراغ collect_updated_tips(بنية oidset *tips, بنية ref *ref_map)
{
	بنية ref *rm;
	لكل (rm = ref_map; rm; rm = rm->بعدی) {
		بنية commit *commit;
		/*
		 * Shallow-rejected refs are not stored and their history
		 * is incomplete, so skip them.
		 */
		إذا (rm->وضعیت == REF_وضعیت_REJECT_SHALLOW)
			استمر;
		إذا (is_null_oid(&rm->old_oid))
			استمر;
		إذا (rm->peer_ref &&
		    oideq(&rm->old_oid, &rm->peer_ref->old_oid))
			استمر;
		commit = lookup_commit_reference_gently(the_repository,
							&rm->old_oid, 1);
		إذا (commit)
			oidset_درج(tips, &commit->object.oid);
	}
}

ساكن صحيح do_fetch(بنية transport *transport,
		    بنية refspec *rs,
		    ثابت بنية fetch_config *config,
		    بنية list_objects_filter_options *filter_options,
		    بنية oidset *updated_tips)
{
	بنية ref_transaction *transaction = عدم;
	بنية ref *ref_map = عدم;
	بنية display_state display_state = { 0 };
	صحيح autotags = (transport->remote->fetch_برچسبها == 1);
	صحيح retcode = 0;
	ثابت بنية ref *remote_refs;
	بنية transport_ls_refs_options transport_ls_refs_options =
		TRANSPORT_LS_REFS_OPTIONS_INIT;
	بنية fetch_سر fetch_سر = { 0 };
	بنية strbuf err = STRBUF_INIT;
	بنية ref_update_display_info_array display_array = { 0 };
	بنية strmap rejected_refs = STRMAP_INIT;
	صحيح summary_عرض = 0;
	صحيح follow_remote_سر = FOLLOW_REMOTE_NEVER;

	إذا (برچسبها == برچسبها_DEFAULT) {
		إذا (transport->remote->fetch_برچسبها == 2)
			برچسبها = برچسبها_مجموعه;
		إذا (transport->remote->fetch_برچسبها == -1)
			برچسبها = برچسبها_UNSET;
	}

	/* if not appending, truncate FETCH_HEAD */
	إذا (!append && write_fetch_سر) {
		retcode = truncate_fetch_سر();
		إذا (retcode)
			اذهب cleanup;
	}

	إذا (rs->nr) {
		refspec_ref_prefixes(rs, &transport_ls_refs_options.ref_prefixes);
	} وإلا {
		بنية branch *branch = branch_get(عدم);

		إذا (transport->remote->fetch.nr) {
			refspec_ref_prefixes(&transport->remote->fetch,
					     &transport_ls_refs_options.ref_prefixes);
			/*
			 * See remote.c's handling of remote.<name>.followRemoteHEAD
			 * for the analogous, still-unresolved case.
			 */
			إذا (transport->remote->follow_remote_سر)
				follow_remote_سر = transport->remote->follow_remote_سر;
			وإلا إذا (config->follow_remote_سر_دیده)
				follow_remote_سر = get_follow_remote_سر(config->follow_remote_سر_raw);
			وإلا
				follow_remote_سر = BUILTIN_FOLLOW_REMOTE_سر_DFLT;
		}
		إذا (branch && branch_has_ادغام_config(branch) &&
		    !strcmp(branch->remote_name, transport->remote->name)) {
			صحيح i;
			لكل (i = 0; i < branch->ادغام_nr; i++) {
				strvec_افزودن(&transport_ls_refs_options.ref_prefixes,
					    branch->ادغام[i]->src);
			}
		}

		/*
		 * If there are no refs specified to fetch, then we just
		 * fetch HEAD; mention that to narrow the advertisement.
		 */
		إذا (!transport_ls_refs_options.ref_prefixes.nr)
			strvec_افزودن(&transport_ls_refs_options.ref_prefixes,
				    "HEAD");
	}

	إذا (برچسبها == برچسبها_مجموعه || برچسبها == برچسبها_DEFAULT)
		strvec_افزودن(&transport_ls_refs_options.ref_prefixes,
			    "refs/tags/");

	إذا (follow_remote_سر != FOLLOW_REMOTE_NEVER)
		strvec_افزودن(&transport_ls_refs_options.ref_prefixes,
			    "HEAD");

	/*
	 * Only initiate ref listing if we have at least one ref we want to
	 * know about.
	 */
	إذا (transport_ls_refs_options.ref_prefixes.nr) {
		trace2_region_enter("fetch", "remote_refs", the_repository);
		remote_refs = transport_get_remote_refs(transport,
							&transport_ls_refs_options);
		trace2_region_leave("fetch", "remote_refs", the_repository);
	} وإلا
		remote_refs = عدم;

	transport_ls_refs_options_release(&transport_ls_refs_options);

	ref_map = get_ref_map(transport->remote, remote_refs, rs,
			      برچسبها, &autotags);
	إذا (!update_سر_ok)
		check_not_current_branch(ref_map);

	retcode = open_fetch_سر(&fetch_سر);
	إذا (retcode)
		اذهب cleanup;

	display_state_init(&display_state, ref_map, transport->url,
			   config->display_format);

	إذا (atomic_fetch) {
		transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
							  0, &err);
		إذا (!transaction) {
			retcode = -1;
			اذهب cleanup;
		}
	}

	إذا (برچسبها == برچسبها_DEFAULT && autotags)
		transport_مجموعه_option(transport, TRANS_OPT_FOLLOWTAGS, "1");
	إذا (prune) {
		/*
		 * We only prune based on refspecs specified
		 * explicitly (via command line or configuration); we
		 * don't care whether --tags was specified.
		 */
		إذا (rs->nr) {
			retcode = prune_refs(&display_state, rs, transaction, ref_map);
		} وإلا {
			retcode = prune_refs(&display_state, &transport->remote->fetch,
					     transaction, ref_map);
		}
		إذا (retcode != 0)
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
	إذا (!transaction) {
		transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
							  REF_TRANSACTION_ALLOW_FAILURE, &err);
		إذا (!transaction) {
			retcode = -1;
			اذهب cleanup;
		}
	}

	إذا (fetch_and_consume_refs(&display_state, transport, transaction, ref_map,
				   &fetch_سر, config, &display_array)) {
		retcode = 1;
		اذهب cleanup;
	}

	/*
	 * If neither --no-tags nor --tags was specified, do automated tag
	 * following.
	 */
	إذا (برچسبها == برچسبها_DEFAULT && autotags) {
		بنية ref *برچسبها_ref_map = عدم, **دم = &برچسبها_ref_map;

		پیدا_کن_non_local_برچسبها(remote_refs, transaction, &برچسبها_ref_map, &دم);
		إذا (برچسبها_ref_map) {
			/*
			 * If backfilling of tags fails then we want to tell
			 * the user so, but we have to continue regardless to
			 * populate upstream information of the references we
			 * have already fetched above. The exception though is
			 * when `--atomic` is passed: in that case we'll abort
			 * the transaction and don't commit anything.
			 */
			إذا (backfill_برچسبها(&display_state, transport, transaction, برچسبها_ref_map,
					  &fetch_سر, config, &display_array, filter_options))
				retcode = 1;
		}

		free_refs(برچسبها_ref_map);
	}

	إذا (retcode)
		اذهب cleanup;

	إذا (verbosity >= 0)
		summary_عرض = transport_summary_عرض(ref_map);

	retcode = commit_ref_transaction(&transaction, atomic_fetch,
					 transport->remote->name,
					 &rejected_refs, &err);
	/*
	 * With '--atomic', bail out if the transaction fails. Without '--atomic',
	 * continue to fetch head and perform other post-fetch operations.
	 */
	إذا (retcode && atomic_fetch)
		اذهب cleanup;

	commit_fetch_سر(&fetch_سر);

	collect_updated_tips(updated_tips, ref_map);

	إذا (مجموعه_upstream) {
		بنية branch *branch = branch_get("HEAD");
		بنية ref *rm;
		بنية ref *منبع_ref = عدم;

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
		لكل (rm = ref_map; rm; rm = rm->بعدی) {
			إذا (!rm->peer_ref) {
				إذا (منبع_ref) {
					warning(_("multiple branches detected, incompatible with --set-upstream"));
					اذهب cleanup;
				} وإلا {
					منبع_ref = rm;
				}
			}
		}
		إذا (منبع_ref) {
			إذا (!branch) {
				ثابت حرف *shortname = منبع_ref->name;
				skip_پیشوند(shortname, "refs/heads/", &shortname);

				warning(_("could not set upstream of HEAD to '%s' from '%s' when "
					  "it does not point to any branch."),
					shortname, transport->remote->name);
				اذهب cleanup;
			}

			إذا (!strcmp(منبع_ref->name, "HEAD") ||
			    starts_with(منبع_ref->name, "refs/heads/"))
				install_branch_config(0,
						      branch->name,
						      transport->remote->name,
						      منبع_ref->name);
			وإلا إذا (starts_with(منبع_ref->name, "refs/remotes/"))
				warning(_("not setting upstream for a remote remote-tracking branch"));
			وإلا إذا (starts_with(منبع_ref->name, "refs/tags/"))
				warning(_("not setting upstream for a remote tag"));
			وإلا
				warning(_("unknown branch type"));
		} وإلا {
			warning(_("no source branch found;\n"
				  "you need to specify exactly one branch with the --set-upstream option"));
		}
	}
	إذا (follow_remote_سر != FOLLOW_REMOTE_NEVER) {
		/*
		 * Way too many cases where this can go wrong so let's just
		 * ignore errors and fail silently for now.
		 */
		مجموعه_سر(remote_refs, transport->remote, follow_remote_سر);
	}

cleanup:
	/*
	 * When using batched updates, we want to commit the non-rejected
	 * updates and also handle the rejections.
	 */
	إذا (retcode && !atomic_fetch && transaction)
		commit_ref_transaction(&transaction, false,
				       transport->remote->name,
				       &rejected_refs, &err);

	لكل (اندازه_t i = 0; i < display_array.nr; i++) {
		بنية ref_update_display_info *info = &display_array.info[i];

		إذا (!info->failed && strmap_شامل_است(&rejected_refs, info->ref))
			ref_update_display_info_مجموعه_failed(info);
		ref_update_display_info_display(info, &display_state, summary_عرض);
		ref_update_display_info_free(info);
	}

	إذا (retcode) {
		إذا (err.len) {
			error("%s", err.buf);
			strbuf_reset(&err);
		}
		إذا (transaction && ref_transaction_abort(transaction, &err) &&
		    err.len)
			error("%s", err.buf);
		transaction = عدم;
	}

	إذا (transaction)
		ref_transaction_free(transaction);

	حرر(display_array.info);
	strmap_clear(&rejected_refs, 0);
	display_state_release(&display_state);
	close_fetch_سر(&fetch_سر);
	strbuf_release(&err);
	free_refs(ref_map);
	أرجع retcode;
}

ساكن صحيح get_one_remote_for_fetch(بنية remote *remote, فراغ *priv)
{
	بنية string_list *list = priv;
	إذا (!remote->skip_default_update)
		string_list_append(list, remote->name);
	أرجع 0;
}

ساكن فراغ جمع_کن_options_مقصد_argv(بنية strvec *argv,
				ثابت بنية fetch_config *config)
{
	إذا (dry_پیاپی)
		strvec_افزودن(argv, "--dry-run");
	إذا (prune != -1)
		strvec_افزودن(argv, prune ? "--prune" : "--no-prune");
	إذا (prune_برچسبها != -1)
		strvec_افزودن(argv, prune_برچسبها ? "--prune-tags" : "--no-prune-tags");
	إذا (update_سر_ok)
		strvec_افزودن(argv, "--update-head-ok");
	إذا (force)
		strvec_افزودن(argv, "--force");
	إذا (keep)
		strvec_افزودن(argv, "--keep");
	إذا (config->recurse_submodules == RECURSE_SUBMODULES_ON)
		strvec_افزودن(argv, "--recurse-submodules");
	وإلا إذا (config->recurse_submodules == RECURSE_SUBMODULES_OFF)
		strvec_افزودن(argv, "--no-recurse-submodules");
	وإلا إذا (config->recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND)
		strvec_افزودن(argv, "--recurse-submodules=on-demand");
	إذا (برچسبها == برچسبها_مجموعه)
		strvec_افزودن(argv, "--tags");
	وإلا إذا (برچسبها == برچسبها_UNSET)
		strvec_افزودن(argv, "--no-tags");
	إذا (verbosity >= 2)
		strvec_افزودن(argv, "-v");
	إذا (verbosity >= 1)
		strvec_افزودن(argv, "-v");
	وإلا إذا (verbosity < 0)
		strvec_افزودن(argv, "-q");
	إذا (family == TRANSPORT_FAMILY_IPV4)
		strvec_افزودن(argv, "--ipv4");
	وإلا إذا (family == TRANSPORT_FAMILY_IPV6)
		strvec_افزودن(argv, "--ipv6");
	إذا (!write_fetch_سر)
		strvec_افزودن(argv, "--no-write-fetch-head");
	إذا (config->display_format == DISPLAY_FORMAT_PORCELAIN)
		strvec_pushf(argv, "--porcelain");
}

/* Fetch multiple remotes in parallel */

بنية parallel_fetch_state {
	ثابت حرف **argv;
	بنية string_list *remotes;
	صحيح بعدی, نتیجه;
	ثابت بنية fetch_config *config;
};

ساكن صحيح fetch_بعدی_remote(بنية فرزند_process *cp,
			     بنية strbuf *بیرون استفادهنشده,
			     فراغ *cb, فراغ **task_cb)
{
	بنية parallel_fetch_state *state = cb;
	حرف *remote;

	إذا (state->بعدی < 0 || state->بعدی >= state->remotes->nr)
		أرجع 0;

	remote = state->remotes->اقلام[state->بعدی++].string;
	*task_cb = remote;

	strvec_pushv(&cp->آرگومانها, state->argv);
	strvec_افزودن(&cp->آرگومانها, remote);
	cp->git_cmd = 1;

	إذا (verbosity >= 0 && state->config->display_format != DISPLAY_FORMAT_PORCELAIN)
		چاپ_کن(_("Fetching %s\n"), remote);

	أرجع 1;
}

ساكن صحيح fetch_failed_مقصد_شروع(بنية strbuf *بیرون استفادهنشده,
				 فراغ *cb, فراغ *task_cb)
{
	بنية parallel_fetch_state *state = cb;
	ثابت حرف *remote = task_cb;

	state->نتیجه = error(_("could not fetch %s"), remote);

	أرجع 0;
}

ساكن صحيح fetch_پایانیافته(صحيح نتیجه, بنية strbuf *بیرون,
			  فراغ *cb, فراغ *task_cb)
{
	بنية parallel_fetch_state *state = cb;
	ثابت حرف *remote = task_cb;

	إذا (نتیجه) {
		strbuf_addf(بیرون, _("could not fetch '%s' (exit code: %d)\n"),
			    remote, نتیجه);
		state->نتیجه = -1;
	}

	أرجع 0;
}

ساكن صحيح fetch_مضرب(بنية string_list *list, صحيح max_children,
			  ثابت بنية fetch_config *config)
{
	صحيح i, نتیجه = 0;
	بنية strvec argv = STRVEC_INIT;

	إذا (!append && write_fetch_سر) {
		صحيح errcode = truncate_fetch_سر();
		إذا (errcode)
			أرجع errcode;
	}

	/*
	 * Cancel out the fetch.bundleURI config when running subprocesses,
	 * to avoid fetching from the same bundle list multiple times.
	 */
	strvec_pushl(&argv, "-c", "fetch.bundleURI=",
		     "fetch", "--append", "--no-auto-gc",
		     "--no-write-commit-graph", عدم);
	لكل (i = 0; i < server_options.nr; i++)
		strvec_pushf(&argv, "--server-option=%s", server_options.اقلام[i].string);
	جمع_کن_options_مقصد_argv(&argv, config);

	إذا (max_children != 1 && list->nr != 1) {
		بنية parallel_fetch_state state = { argv.v, list, 0, 0, config };
		ثابت بنية پیاپی_process_parallel_opts opts = {
			.tr2_category = "fetch",
			.tr2_label = "parallel/fetch",

			.processes = max_children,

			.get_بعدی_task = &fetch_بعدی_remote,
			.شروع_failure = &fetch_failed_مقصد_شروع,
			.task_پایانیافته = &fetch_پایانیافته,
			.داده = &state,
		};

		strvec_افزودن(&argv, "--end-of-options");

		پیاپی_processes_parallel(&opts);
		نتیجه = state.نتیجه;
	} وإلا
		لكل (i = 0; i < list->nr; i++) {
			ثابت حرف *name = list->اقلام[i].string;
			بنية فرزند_process cmd = فرزند_PROCESS_INIT;

			strvec_pushv(&cmd.آرگومانها, argv.v);
			strvec_افزودن(&cmd.آرگومانها, name);
			إذا (verbosity >= 0 && config->display_format != DISPLAY_FORMAT_PORCELAIN)
				چاپ_کن(_("Fetching %s\n"), name);
			cmd.git_cmd = 1;
			إذا (پیاپی_command(&cmd)) {
				error(_("could not fetch %s"), name);
				نتیجه = 1;
			}
		}

	strvec_clear(&argv);
	أرجع !!نتیجه;
}

/*
 * Fetching from the promisor remote should use the given filter-spec
 * or inherit the default filter-spec from the config.
 */
ساكن مضمن فراغ fetch_one_راهاندازی_partial(بنية remote *remote,
					   بنية list_objects_filter_options *filter_options)
{
	/*
	 * Explicit --no-filter argument overrides everything, regardless
	 * of any prior partial clones and fetches.
	 */
	إذا (filter_options->no_filter)
		أرجع;

	/*
	 * If no prior partial clone/fetch and the current fetch DID NOT
	 * request a partial-fetch, do a normal fetch.
	 */
	إذا (!repo_has_promisor_remote(the_repository) && !filter_options->choice)
		أرجع;

	/*
	 * If this is a partial-fetch request, we enable partial on
	 * this repo if not already enabled and remember the given
	 * filter-spec as the default for subsequent fetches to this
	 * remote if there is currently no default filter-spec.
	 */
	إذا (filter_options->choice) {
		partial_clone_ثبات(remote->name, filter_options);
		أرجع;
	}

	/*
	 * Do a partial-fetch from the promisor remote using either the
	 * explicitly given filter-spec or inherit the filter-spec from
	 * the config.
	 */
	إذا (!filter_options->choice)
		partial_clone_get_default_filter_spec(filter_options, remote->name);
	أرجع;
}

ساكن صحيح fetch_one(بنية remote *remote, صحيح argc, ثابت حرف **argv,
		     صحيح prune_برچسبها_ok, صحيح use_stdin_refspecs,
		     ثابت بنية fetch_config *config,
		     بنية list_objects_filter_options *filter_options,
		     بنية oidset *updated_tips)
{
	بنية refspec rs = REFSPEC_INIT_FETCH(the_hash_algo);
	صحيح i;
	صحيح exit_کد;
	صحيح maybe_prune_برچسبها;
	صحيح remote_از_طریق_config = remote_is_configured(remote, 0);

	إذا (!remote)
		die(_("no remote repository specified; please specify either a URL or a\n"
		      "remote name from which new revisions should be fetched"));

	gtransport = prepare_transport(remote, 1, filter_options);

	إذا (prune < 0) {
		/* no command line request */
		إذا (0 <= remote->prune)
			prune = remote->prune;
		وإلا إذا (0 <= config->prune)
			prune = config->prune;
		وإلا
			prune = PRUNE_BY_DEFAULT;
	}

	إذا (prune_برچسبها < 0) {
		/* no command line request */
		إذا (0 <= remote->prune_برچسبها)
			prune_برچسبها = remote->prune_برچسبها;
		وإلا إذا (0 <= config->prune_برچسبها)
			prune_برچسبها = config->prune_برچسبها;
		وإلا
			prune_برچسبها = PRUNE_برچسبها_BY_DEFAULT;
	}

	maybe_prune_برچسبها = prune_برچسبها_ok && prune_برچسبها;
	إذا (maybe_prune_برچسبها && remote_از_طریق_config)
		refspec_append(&remote->fetch, برچسب_REFSPEC);

	إذا (maybe_prune_برچسبها && (argc || !remote_از_طریق_config))
		refspec_append(&rs, برچسب_REFSPEC);

	لكل (i = 0; i < argc; i++) {
		إذا (!strcmp(argv[i], "tag")) {
			i++;
			إذا (i >= argc)
				die(_("you need to specify a tag name"));

			refspec_appendf(&rs, "refs/tags/%s:refs/tags/%s",
					argv[i], argv[i]);
		} وإلا {
			refspec_append(&rs, argv[i]);
		}
	}

	إذا (use_stdin_refspecs) {
		بنية strbuf سطر = STRBUF_INIT;
		طالما (strbuf_getline_lf(&سطر, stdin) != EOF)
			refspec_append(&rs, سطر.buf);
		strbuf_release(&سطر);
	}

	إذا (server_options.nr)
		gtransport->server_options = &server_options;

	sigchain_افزودن_common(unlock_pack_on_signal);
	atexit(unlock_pack_atexit);
	sigchain_افزودن(SIGPIPE, SIG_IGN);
	exit_کد = do_fetch(gtransport, &rs, config, filter_options,
			     updated_tips);
	sigchain_برداشت(SIGPIPE);
	refspec_clear(&rs);
	transport_disconnect(gtransport);
	gtransport = عدم;
	أرجع exit_کد;
}

صحيح cmd_fetch(صحيح argc,
	      ثابت حرف **argv,
	      ثابت حرف *پیشوند,
	      بنية repository *repo استفادهنشده)
{
	بنية fetch_config config = {
		.display_format = DISPLAY_FORMAT_پر,
		.follow_remote_سر_raw = عدم,
		.follow_remote_سر_دیده = 0,
		.prune = -1,
		.prune_برچسبها = -1,
		.show_forced_updates = 1,
		.recurse_submodules = RECURSE_SUBMODULES_DEFAULT,
		.parallel = 1,
		.submodule_fetch_jobs = -1,
	};
	ثابت حرف *submodule_پیشوند = "";
	ثابت حرف *bundle_uri;
	بنية string_list list = STRING_LIST_INIT_DUP;
	بنية list_objects_filter_options filter_options = LIST_OBJECTS_FILTER_INIT;
	بنية remote *remote = عدم;
	صحيح all = -1, مضرب = 0;
	صحيح نتیجه = 0;
	صحيح prune_برچسبها_ok = 1;
	صحيح enable_auto_gc = 1;
	صحيح unshallow = 0;
	صحيح max_jobs = -1;
	صحيح recurse_submodules_cli = RECURSE_SUBMODULES_DEFAULT;
	صحيح recurse_submodules_default = RECURSE_SUBMODULES_ON_DEMAND;
	صحيح fetch_write_commit_گراف = -1;
	صحيح stdin_refspecs = 0;
	صحيح negotiate_only = 0;
	صحيح porcelain = 0;
	صحيح i;
	تعداد {
		گراف_WRITE_REACHABLE,
		گراف_WRITE_TIPS,
		گراف_WRITE_SKIP,
	} گراف_write_نما = گراف_WRITE_REACHABLE;
	بنية oidset updated_tips = OIDSET_INIT;

	بنية option builtin_fetch_options[] = {
		OPT__VERBOSITY(&verbosity),
		OPT_BOOL(0, "all", &all,
			 N_("fetch from all remotes")),
		OPT_BOOL(0, "set-upstream", &مجموعه_upstream,
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
		OPT_مجموعه_INT('t', "tags", &برچسبها,
			    N_("fetch all tags and associated objects"), برچسبها_مجموعه),
		OPT_مجموعه_INT('n', عدم, &برچسبها,
			    N_("do not fetch all tags (--no-tags)"), برچسبها_UNSET),
		OPT_INTEGER('j', "jobs", &max_jobs,
			    N_("number of submodules fetched in parallel")),
		OPT_BOOL(0, "prefetch", &prefetch,
			 N_("modify the refspec to place all refs within refs/prefetch/")),
		OPT_BOOL('p', "prune", &prune,
			 N_("prune remote-tracking branches no longer on remote")),
		OPT_BOOL('P', "prune-tags", &prune_برچسبها,
			 N_("prune local tags no longer on remote and clobber changed tags")),
		OPT_CALLBACK_F(0, "recurse-submodules", &recurse_submodules_cli, N_("on-demand"),
			    N_("control recursive fetching of submodules"),
			    PARSE_OPT_OPTARG, option_fetch_parse_recurse_submodules),
		OPT_BOOL(0, "dry-run", &dry_پیاپی,
			 N_("dry run")),
		OPT_BOOL(0, "porcelain", &porcelain, N_("machine-readable output")),
		OPT_BOOL(0, "write-fetch-head", &write_fetch_سر,
			 N_("write fetched references to the FETCH_HEAD file")),
		OPT_BOOL('k', "keep", &keep, N_("keep downloaded pack")),
		OPT_BOOL('u', "update-head-ok", &update_سر_ok,
			    N_("allow updating of HEAD ref")),
		OPT_BOOL(0, "progress", &پیشرفت, N_("force progress reporting")),
		OPT_STRING(0, "depth", &عمق, N_("depth"),
			   N_("deepen history of shallow clone")),
		OPT_STRING(0, "shallow-since", &deepen_since, N_("time"),
			   N_("deepen history of shallow repository based on time")),
		OPT_STRING_LIST(0, "shallow-exclude", &deepen_not, N_("ref"),
				N_("deepen history of shallow clone, excluding ref")),
		OPT_INTEGER(0, "deepen", &deepen_relative,
			    N_("deepen history of shallow clone")),
		OPT_مجموعه_INT_F(0, "unshallow", &unshallow,
			      N_("convert to a complete repository"),
			      1, PARSE_OPT_NONEG),
		OPT_مجموعه_INT_F(0, "refetch", &refetch,
			      N_("re-fetch without negotiating common commits"),
			      1, PARSE_OPT_NONEG),
		{
			.type = OPTION_STRING,
			.long_name = "submodule-prefix",
			.مقدار = &submodule_پیشوند,
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
		OPT_انتها()
	};

	filter_options.allow_auto_filter = 1;

	refspec_init_fetch(&refmap, the_hash_algo);

	packet_trace_عنصر_خنثی("fetch");

	/* Record the command line for the reflog */
	strbuf_addstr(&default_rla, "fetch");
	لكل (i = 1; i < argc; i++) {
		/* This handles non-URLs gracefully */
		حرف *anon = transport_anonymize_url(argv[i]);

		strbuf_addf(&default_rla, " %s", anon);
		حرر(anon);
	}

	repo_config(the_repository, git_fetch_config, &config);
	إذا (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_پر_شاخص = 0;
	}

	argc = parse_options(argc, argv, پیشوند,
			     builtin_fetch_options, builtin_fetch_usage, 0);

	إذا (recurse_submodules_cli != RECURSE_SUBMODULES_DEFAULT)
		config.recurse_submodules = recurse_submodules_cli;

	إذا (negotiate_only) {
		بدل (recurse_submodules_cli) {
		حالة RECURSE_SUBMODULES_OFF:
		حالة RECURSE_SUBMODULES_DEFAULT:
			/*
			 * --negotiate-only should never recurse into
			 * submodules. Skip it by setting recurse_submodules to
			 * RECURSE_SUBMODULES_OFF.
			 */
			config.recurse_submodules = RECURSE_SUBMODULES_OFF;
			اكسر;

		افتراضي:
			die(_("options '%s' and '%s' cannot be used together"),
			    "--negotiate-only", "--recurse-submodules");
		}
	}

	إذا (config.recurse_submodules != RECURSE_SUBMODULES_OFF) {
		صحيح *sfjc = config.submodule_fetch_jobs == -1
			    ? &config.submodule_fetch_jobs : عدم;
		صحيح *rs = config.recurse_submodules == RECURSE_SUBMODULES_DEFAULT
			  ? &config.recurse_submodules : عدم;

		fetch_config_from_gitmodules(the_repository, sfjc, rs);
	}


	إذا (porcelain) {
		بدل (recurse_submodules_cli) {
		حالة RECURSE_SUBMODULES_OFF:
		حالة RECURSE_SUBMODULES_DEFAULT:
			/*
			 * Reference updates in submodules would be ambiguous
			 * in porcelain mode, so we reject this combination.
			 */
			config.recurse_submodules = RECURSE_SUBMODULES_OFF;
			اكسر;

		افتراضي:
			die(_("options '%s' and '%s' cannot be used together"),
			    "--porcelain", "--recurse-submodules");
		}

		config.display_format = DISPLAY_FORMAT_PORCELAIN;
	}

	إذا (deepen_relative) {
		إذا (deepen_relative < 0)
			die(_("negative depth in --deepen is not supported"));
		إذا (عمق)
			die(_("options '%s' and '%s' cannot be used together"), "--deepen", "--depth");
		عمق = xstrfmt("%d", deepen_relative);
	}
	إذا (unshallow) {
		إذا (عمق)
			die(_("options '%s' and '%s' cannot be used together"), "--depth", "--unshallow");
		وإلا إذا (!is_repository_shallow(the_repository))
			die(_("--unshallow on a complete repository does not make sense"));
		وإلا
			عمق = xstrfmt("%d", INFINITE_عمق);
	}

	/* no need to be strict, transport_set_option() will validate it again */
	إذا (عمق && atoi(عمق) < 1)
		die(_("depth %s is not a positive number"), عمق);
	إذا (عمق || deepen_since || deepen_not.nr)
		deepen = 1;

	/* FETCH_HEAD never gets updated in --dry-run mode */
	إذا (dry_پیاپی)
		write_fetch_سر = 0;

	إذا (!max_jobs)
		max_jobs = online_cpus();

	إذا (!repo_config_get_string_tmp(the_repository, "fetch.bundleuri", &bundle_uri) &&
	    fetch_bundle_uri(the_repository, bundle_uri, عدم))
		warning(_("failed to fetch bundles from '%s'"), bundle_uri);

	إذا (all < 0) {
		/*
		 * no --[no-]all given;
		 * only use config option if no remote was explicitly specified
		 */
		all = (!argc) ? config.all : 0;
	}

	إذا (all) {
		إذا (argc == 1)
			die(_("fetch --all does not take a repository argument"));
		وإلا إذا (argc > 1)
			die(_("fetch --all does not make sense with refspecs"));

		(فراغ) for_each_remote(get_one_remote_for_fetch, &list);

		/* do not do fetch_multiple() of one */
		إذا (list.nr == 1)
			remote = remote_get(list.اقلام[0].string);
	} وإلا إذا (argc == 0) {
		/* No arguments -- use default remote */
		remote = remote_get(عدم);
	} وإلا إذا (مضرب) {
		/* All arguments are assumed to be remotes or groups */
		لكل (i = 0; i < argc; i++)
			إذا (!جمع_کن_remote_or_گروه(argv[i], &list))
				die(_("no such remote or remote group: %s"),
				    argv[i]);
	} وإلا {
		/* Single remote or group */
		(فراغ) جمع_کن_remote_or_گروه(argv[0], &list);
		إذا (list.nr > 1) {
			/* More than one remote */
			إذا (argc > 1)
				die(_("fetching a group and specifying refspecs does not make sense"));
		} وإلا {
			/* Zero or one remotes */
			remote = remote_get(argv[0]);
			prune_برچسبها_ok = (argc == 1);
			argc--;
			argv++;
		}
	}
	string_list_remove_duplicates(&list, 0);

	إذا (negotiate_only) {
		بنية oidset acked_commits = OIDSET_INIT;
		بنية oidset_iter iter;
		ثابت بنية object_id *oid;

		trace2_region_enter("fetch", "negotiate-only", the_repository);
		إذا (!remote)
			die(_("must supply remote when using --negotiate-only"));
		gtransport = prepare_transport(remote, 1, &filter_options);

		إذا (!gtransport->smart_options) {
			warning(_("protocol does not support --negotiate-only, exiting"));
			نتیجه = 1;
			trace2_region_leave("fetch", "negotiate-only", the_repository);
			اذهب cleanup;
		}
		إذا (!gtransport->smart_options->negotiation_restrict_tips)
			die(_("%s needs one or more %s"), "--negotiate-only",
			    "--negotiation-restrict=*");

		gtransport->smart_options->acked_commits = &acked_commits;

		إذا (server_options.nr)
			gtransport->server_options = &server_options;
		نتیجه = transport_fetch_refs(gtransport, عدم);
		gtransport->smart_options->acked_commits = عدم;

		oidset_iter_init(&acked_commits, &iter);
		طالما ((oid = oidset_iter_بعدی(&iter)))
			چاپ_کن("%s\n", oid_مقصد_hex(oid));
		oidset_clear(&acked_commits);
		trace2_region_leave("fetch", "negotiate-only", the_repository);
	} وإلا إذا (remote) {
		إذا (filter_options.choice || repo_has_promisor_remote(the_repository)) {
			trace2_region_enter("fetch", "setup-partial", the_repository);
			fetch_one_راهاندازی_partial(remote, &filter_options);
			trace2_region_leave("fetch", "setup-partial", the_repository);
		}
		trace2_region_enter("fetch", "fetch-one", the_repository);
		نتیجه = fetch_one(remote, argc, argv, prune_برچسبها_ok, stdin_refspecs,
				   &config, &filter_options, &updated_tips);
		إذا (prepare_commit_گراف(the_repository)) {
			إذا (oidset_اندازه(&updated_tips))
				گراف_write_نما = گراف_WRITE_TIPS;
			وإلا
				گراف_write_نما = گراف_WRITE_SKIP;
		}
		trace2_region_leave("fetch", "fetch-one", the_repository);
	} وإلا {
		صحيح max_children = max_jobs;

		إذا (filter_options.choice)
			die(_("--filter can only be used with the remote "
			      "configured in extensions.partialclone"));

		إذا (atomic_fetch)
			die(_("--atomic can only be used when fetching "
			      "from one remote"));

		إذا (stdin_refspecs)
			die(_("--stdin can only be used when fetching "
			      "from one remote"));

		إذا (max_children < 0)
			max_children = config.parallel;

		/* TODO should this also die if we have a previous partial-clone? */
		trace2_region_enter("fetch", "fetch-multiple", the_repository);
		نتیجه = fetch_مضرب(&list, max_children, &config);
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
	إذا (!نتیجه && remote && (config.recurse_submodules != RECURSE_SUBMODULES_OFF)) {
		بنية strvec options = STRVEC_INIT;
		صحيح max_children = max_jobs;

		إذا (max_children < 0)
			max_children = config.submodule_fetch_jobs;
		إذا (max_children < 0)
			max_children = config.parallel;

		جمع_کن_options_مقصد_argv(&options, &config);
		trace2_region_enter_printf("fetch", "recurse-submodule", the_repository, "%s", submodule_پیشوند);
		نتیجه = fetch_submodules(the_repository,
					  &options,
					  submodule_پیشوند,
					  config.recurse_submodules,
					  recurse_submodules_default,
					  verbosity < 0,
					  max_children);
		trace2_region_leave_printf("fetch", "recurse-submodule", the_repository, "%s", submodule_پیشوند);
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
	إذا (negotiate_only)
		اذهب cleanup;

	prepare_repo_settings(the_repository);
	إذا (fetch_write_commit_گراف > 0 ||
	    (fetch_write_commit_گراف < 0 &&
	     the_repository->settings.fetch_write_commit_گراف)) {
		صحيح commit_گراف_flags = COMMIT_گراف_WRITE_تقسیم;

		إذا (پیشرفت)
			commit_گراف_flags |= COMMIT_گراف_WRITE_پیشرفت;

		إذا (گراف_write_نما != گراف_WRITE_SKIP) {
			trace2_region_enter("fetch", "write-commit-graph",
					    the_repository);
			إذا (گراف_write_نما == گراف_WRITE_TIPS)
				write_commit_گراف(
					the_repository,
					the_repository->objects->منبع->مسیر,
					عدم, &updated_tips,
					commit_گراف_flags, عدم);
			وإلا
				write_commit_گراف_reachable(
					the_repository,
					the_repository->objects->منبع->مسیر,
					commit_گراف_flags, عدم);
			trace2_region_leave("fetch", "write-commit-graph",
					    the_repository);
		}
	}

	إذا (enable_auto_gc) {
		إذا (refetch) {
			/*
			 * Hint auto-maintenance strongly to encourage repacking,
			 * but respect config settings disabling it.
			 */
			صحيح opt_val;
			إذا (repo_config_get_int(the_repository, "gc.autopacklimit", &opt_val))
				opt_val = -1;
			إذا (opt_val != 0)
				git_config_افزودن_parameter("gc.autoPackLimit=1");

			إذا (repo_config_get_int(the_repository, "maintenance.incremental-repack.auto", &opt_val))
				opt_val = -1;
			إذا (opt_val != 0)
				git_config_افزودن_parameter("maintenance.incremental-repack.auto=-1");
		}
		پیاپی_auto_maintenance(the_repository, verbosity < 0);
	}

 cleanup:
	oidset_clear(&updated_tips);
	string_list_clear(&list, 0);
	list_objects_filter_release(&filter_options);
	حرر(config.follow_remote_سر_raw);
	أرجع نتیجه;
}
