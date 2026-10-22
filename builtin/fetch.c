/*
 * "git fetch"
 */

#घोषणा USE_THE_REPOSITORY_VARIABLE
#घोषणा DISABLE_SIGN_COMPARE_WARNINGS

#समावेश "builtin.h"
#समावेश "advice.h"
#समावेश "config.h"
#समावेश "gettext.h"
#समावेश "environment.h"
#समावेश "hex.h"
#समावेश "refs.h"
#समावेश "refspec.h"
#समावेश "object-name.h"
#समावेश "odb.h"
#समावेश "odb/source.h"
#समावेश "oidset.h"
#समावेश "oid-array.h"
#समावेश "commit.h"
#समावेश "string-list.h"
#समावेश "remote.h"
#समावेश "transport.h"
#समावेश "run-command.h"
#समावेश "parse-options.h"
#समावेश "sigchain.h"
#समावेश "submodule-config.h"
#समावेश "submodule.h"
#समावेश "connected.h"
#समावेश "strvec.h"
#समावेश "utf8.h"
#समावेश "pager.h"
#समावेश "path.h"
#समावेश "pkt-line.h"
#समावेश "list-objects-filter-options.h"
#समावेश "commit-reach.h"
#समावेश "branch.h"
#समावेश "promisor-remote.h"
#समावेश "commit-graph.h"
#समावेश "shallow.h"
#समावेश "trace.h"
#समावेश "trace2.h"
#समावेश "bundle-uri.h"

#घोषणा FORCED_UPDATES_DELAY_WARNING_IN_MS (10 * 1000)

अटल स्थिर अक्षर * स्थिर builtin_fetch_usage[] = {
	N_("git fetch [<options>] [<repository> [<refspec>...]]"),
	N_("git fetch [<options>] <group>"),
	N_("git fetch --multiple [<options>] [(<repository>|<group>)...]"),
	N_("git fetch --all [<options>]"),
	शून्य
};

क्रमागत {
	TAGS_UNSET = 0,
	TAGS_DEFAULT = 1,
	TAGS_SET = 2
};

क्रमागत display_format {
	DISPLAY_FORMAT_FULL,
	DISPLAY_FORMAT_COMPACT,
	DISPLAY_FORMAT_PORCELAIN,
};

काष्ठा display_state {
	काष्ठा strbuf buf;

	पूर्णांक refcol_width;
	क्रमागत display_format format;

	अक्षर *url;
	पूर्णांक url_len, shown_url;
};

अटल uint64_t forced_updates_ms = 0;
अटल पूर्णांक prefetch = 0;
अटल पूर्णांक prune = -1; /* unspecified */
#घोषणा PRUNE_BY_DEFAULT 0 /* do we prune by default? */

अटल पूर्णांक prune_tags = -1; /* unspecified */
#घोषणा PRUNE_TAGS_BY_DEFAULT 0 /* do we prune tags by default? */

अटल पूर्णांक append, dry_run, force, keep, update_शीर्ष_ok;
अटल पूर्णांक write_fetch_शीर्ष = 1;
अटल पूर्णांक verbosity, deepen_relative, set_upstream, refetch;
अटल पूर्णांक progress = -1;
अटल पूर्णांक tags = TAGS_DEFAULT, update_shallow, deepen;
अटल पूर्णांक atomic_fetch;
अटल क्रमागत transport_family family;
अटल स्थिर अक्षर *depth;
अटल स्थिर अक्षर *deepen_since;
अटल स्थिर अक्षर *upload_pack;
अटल काष्ठा string_list deepen_not = STRING_LIST_INIT_NODUP;
अटल काष्ठा strbuf default_rla = STRBUF_INIT;
अटल काष्ठा transport *gtransport;
अटल काष्ठा transport *gsecondary;
अटल काष्ठा refspec refmap;
अटल काष्ठा string_list server_options = STRING_LIST_INIT_DUP;
अटल काष्ठा string_list negotiation_restrict = STRING_LIST_INIT_NODUP;
अटल काष्ठा string_list negotiation_include = STRING_LIST_INIT_NODUP;

काष्ठा fetch_config {
	क्रमागत display_format display_format;
	अक्षर *follow_remote_शीर्ष_raw;
	पूर्णांक follow_remote_शीर्ष_seen;
	पूर्णांक all;
	पूर्णांक prune;
	पूर्णांक prune_tags;
	पूर्णांक show_forced_updates;
	पूर्णांक recurse_submodules;
	पूर्णांक parallel;
	पूर्णांक submodule_fetch_jobs;
};

अटल पूर्णांक git_fetch_config(स्थिर अक्षर *k, स्थिर अक्षर *v,
			    स्थिर काष्ठा config_context *ctx, व्योम *cb)
{
	काष्ठा fetch_config *fetch_config = cb;

	अगर (!म_भेद(k, "fetch.all")) {
		fetch_config->all = git_config_bool(k, v);
		वापस 0;
	}

	अगर (!म_भेद(k, "fetch.prune")) {
		fetch_config->prune = git_config_bool(k, v);
		वापस 0;
	}

	अगर (!म_भेद(k, "fetch.prunetags")) {
		fetch_config->prune_tags = git_config_bool(k, v);
		वापस 0;
	}

	अगर (!म_भेद(k, "fetch.showforcedupdates")) {
		fetch_config->show_forced_updates = git_config_bool(k, v);
		वापस 0;
	}

	अगर (!म_भेद(k, "submodule.recurse")) {
		पूर्णांक r = git_config_bool(k, v) ?
			RECURSE_SUBMODULES_ON : RECURSE_SUBMODULES_OFF;
		fetch_config->recurse_submodules = r;
		वापस 0;
	}

	अगर (!म_भेद(k, "submodule.fetchjobs")) {
		fetch_config->submodule_fetch_jobs = parse_submodule_fetchjobs(k, v, ctx->kvi);
		वापस 0;
	} अन्यथा अगर (!म_भेद(k, "fetch.recursesubmodules")) {
		fetch_config->recurse_submodules = parse_fetch_recurse_submodules_arg(k, v);
		वापस 0;
	}

	अगर (!म_भेद(k, "fetch.parallel")) {
		fetch_config->parallel = git_config_int(k, v, ctx->kvi);
		अगर (fetch_config->parallel < 0)
			die(_("fetch.parallel cannot be negative"));
		अगर (!fetch_config->parallel)
			fetch_config->parallel = online_cpus();
		वापस 0;
	}

	अगर (!म_भेद(k, "fetch.output")) {
		अगर (!v)
			वापस config_error_nonbool(k);
		अन्यथा अगर (!strcasecmp(v, "full"))
			fetch_config->display_format = DISPLAY_FORMAT_FULL;
		अन्यथा अगर (!strcasecmp(v, "compact"))
			fetch_config->display_format = DISPLAY_FORMAT_COMPACT;
		अन्यथा
			die(_("invalid value for '%s': '%s'"),
			    "fetch.output", v);
		वापस 0;
	}

	अगर (!म_भेद(k, "fetch.followremotehead")) {
		मुक्त(fetch_config->follow_remote_शीर्ष_raw);
		fetch_config->follow_remote_शीर्ष_raw = xstrdup_or_null(v);
		fetch_config->follow_remote_शीर्ष_seen = 1;
		वापस 0;
	}

	वापस git_default_config(k, v, ctx, cb);
}

अटल क्रमागत follow_remote_शीर्ष_settings get_follow_remote_शीर्ष(स्थिर अक्षर *setting)
{
	अगर (!setting)
		die(_("missing value for 'fetch.followRemoteHEAD'"));
	अन्यथा अगर (!म_भेद(setting, "never"))
		वापस FOLLOW_REMOTE_NEVER;
	अन्यथा अगर (!म_भेद(setting, "create"))
		वापस FOLLOW_REMOTE_CREATE;
	अन्यथा अगर (!म_भेद(setting, "warn"))
		वापस FOLLOW_REMOTE_WARN;
	अन्यथा अगर (!म_भेद(setting, "always"))
		वापस FOLLOW_REMOTE_ALWAYS;
	warning(_("unrecognized fetch.followRemoteHEAD value '%s' ignored"), setting);
	वापस BUILTIN_FOLLOW_REMOTE_शीर्ष_DFLT;
}

अटल पूर्णांक parse_refmap_arg(स्थिर काष्ठा option *opt, स्थिर अक्षर *arg, पूर्णांक unset)
{
	BUG_ON_OPT_NEG(unset);

	/*
	 * "git fetch --refmap='' origin foo"
	 * can be used to tell the command not to store anywhere
	 */
	refspec_append(opt->मूल्य, arg);

	वापस 0;
}

अटल व्योम unlock_pack(अचिन्हित पूर्णांक flags)
{
	अगर (gtransport)
		transport_unlock_pack(gtransport, flags);
	अगर (gsecondary)
		transport_unlock_pack(gsecondary, flags);
}

अटल व्योम unlock_pack_atexit(व्योम)
{
	unlock_pack(0);
}

अटल व्योम unlock_pack_on_signal(पूर्णांक signo)
{
	unlock_pack(TRANSPORT_UNLOCK_PACK_IN_SIGNAL_HANDLER);
	sigchain_निकालो(signo);
	उठाओ(signo);
}

अटल व्योम add_merge_config(काष्ठा ref **शीर्ष,
			   स्थिर काष्ठा ref *remote_refs,
		           काष्ठा branch *branch,
		           काष्ठा ref ***पुच्छ)
{
	पूर्णांक i;

	क्रम (i = 0; i < branch->merge_nr; i++) {
		काष्ठा ref *rm, **old_पुच्छ = *पुच्छ;
		काष्ठा refspec_item refspec;

		क्रम (rm = *शीर्ष; rm; rm = rm->अग्र) {
			अगर (branch_merge_matches(branch, i, rm->name)) {
				rm->fetch_शीर्ष_status = FETCH_शीर्ष_MERGE;
				अवरोध;
			}
		}
		अगर (rm)
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
		स_रखो(&refspec, 0, माप(refspec));
		refspec.src = branch->merge[i]->src;
		get_fetch_map(remote_refs, &refspec, पुच्छ, 1);
		क्रम (rm = *old_पुच्छ; rm; rm = rm->अग्र)
			rm->fetch_शीर्ष_status = FETCH_शीर्ष_MERGE;
	}
}

अटल व्योम create_fetch_oidset(काष्ठा ref **शीर्ष, काष्ठा oidset *out)
{
	काष्ठा ref *rm = *शीर्ष;
	जबतक (rm) {
		oidset_जोड़ो(out, &rm->old_oid);
		rm = rm->अग्र;
	}
}

काष्ठा refname_hash_entry {
	काष्ठा hashmap_entry ent;
	काष्ठा object_id oid;
	पूर्णांक ignore;
	अक्षर refname[FLEX_ARRAY];
};

अटल पूर्णांक refname_hash_entry_cmp(स्थिर व्योम *hashmap_cmp_fn_तथ्य UNUSED,
				  स्थिर काष्ठा hashmap_entry *eptr,
				  स्थिर काष्ठा hashmap_entry *entry_or_कुंजी,
				  स्थिर व्योम *keydata)
{
	स्थिर काष्ठा refname_hash_entry *e1, *e2;

	e1 = container_of(eptr, स्थिर काष्ठा refname_hash_entry, ent);
	e2 = container_of(entry_or_कुंजी, स्थिर काष्ठा refname_hash_entry, ent);
	वापस म_भेद(e1->refname, keydata ? keydata : e2->refname);
}

अटल काष्ठा refname_hash_entry *refname_hash_add(काष्ठा hashmap *map,
						   स्थिर अक्षर *refname,
						   स्थिर काष्ठा object_id *oid)
{
	काष्ठा refname_hash_entry *ent;
	माप_प्रकार len = म_माप(refname);

	FLEX_ALLOC_MEM(ent, refname, refname, len);
	hashmap_entry_init(&ent->ent, strhash(refname));
	oidcpy(&ent->oid, oid);
	hashmap_add(map, &ent->ent);
	वापस ent;
}

अटल पूर्णांक add_one_refname(स्थिर काष्ठा reference *ref, व्योम *cbdata)
{
	काष्ठा hashmap *refname_map = cbdata;

	(व्योम) refname_hash_add(refname_map, ref->name, ref->oid);
	वापस 0;
}

अटल व्योम refname_hash_init(काष्ठा hashmap *map)
{
	hashmap_init(map, refname_hash_entry_cmp, शून्य, 0);
}

अटल पूर्णांक refname_hash_exists(काष्ठा hashmap *map, स्थिर अक्षर *refname)
{
	वापस !!hashmap_get_from_hash(map, strhash(refname), refname);
}

अटल व्योम clear_item(काष्ठा refname_hash_entry *item)
{
	item->ignore = 1;
}


अटल व्योम add_already_queued_tags(स्थिर अक्षर *refname,
				    स्थिर काष्ठा object_id *old_oid UNUSED,
				    स्थिर काष्ठा object_id *new_oid,
				    व्योम *cb_तथ्य)
{
	काष्ठा hashmap *queued_tags = cb_तथ्य;
	अगर (starts_with(refname, "refs/tags/") && new_oid)
		(व्योम) refname_hash_add(queued_tags, refname, new_oid);
}

अटल व्योम find_non_local_tags(स्थिर काष्ठा ref *refs,
				काष्ठा ref_transaction *transaction,
				काष्ठा ref **शीर्ष,
				काष्ठा ref ***पुच्छ)
{
	काष्ठा hashmap existing_refs;
	काष्ठा hashmap remote_refs;
	काष्ठा oidset fetch_oids = OIDSET_INIT;
	काष्ठा string_list remote_refs_list = STRING_LIST_INIT_NODUP;
	काष्ठा string_list_item *remote_ref_item;
	स्थिर काष्ठा ref *ref;
	काष्ठा refname_hash_entry *item = शून्य;

	refname_hash_init(&existing_refs);
	refname_hash_init(&remote_refs);
	create_fetch_oidset(शीर्ष, &fetch_oids);

	refs_for_each_ref(get_main_ref_store(the_repository), add_one_refname,
			  &existing_refs);

	/*
	 * If we already have a transaction, then we need to filter out all
	 * tags which have already been queued up.
	 */
	अगर (transaction)
		ref_transaction_for_each_queued_update(transaction,
						       add_already_queued_tags,
						       &existing_refs);

	क्रम (ref = refs; ref; ref = ref->अग्र) {
		अगर (!starts_with(ref->name, "refs/tags/"))
			जारी;

		/*
		 * The peeled ref always follows the matching base
		 * ref, so if we see a peeled ref that we don't want
		 * to fetch then we can mark the ref entry in the list
		 * as one to ignore by setting util to NULL.
		 */
		अगर (ends_with(ref->name, "^{}")) {
			अगर (item &&
			    !odb_has_object(the_repository->objects, &ref->old_oid, 0) &&
			    !oidset_contains(&fetch_oids, &ref->old_oid) &&
			    !odb_has_object(the_repository->objects, &item->oid, 0) &&
			    !oidset_contains(&fetch_oids, &item->oid))
				clear_item(item);
			item = शून्य;
			जारी;
		}

		/*
		 * If item is non-NULL here, then we previously saw a
		 * ref not followed by a peeled reference, so we need
		 * to check if it is a lightweight tag that we want to
		 * fetch.
		 */
		अगर (item &&
		    !odb_has_object(the_repository->objects, &item->oid, 0) &&
		    !oidset_contains(&fetch_oids, &item->oid))
			clear_item(item);

		item = शून्य;

		/* skip duplicates and refs that we already have */
		अगर (refname_hash_exists(&remote_refs, ref->name) ||
		    refname_hash_exists(&existing_refs, ref->name))
			जारी;

		item = refname_hash_add(&remote_refs, ref->name, &ref->old_oid);
		string_list_जोड़ो(&remote_refs_list, ref->name);
	}
	hashmap_clear_and_free(&existing_refs, काष्ठा refname_hash_entry, ent);

	/*
	 * We may have a final lightweight tag that needs to be
	 * checked to see if it needs fetching.
	 */
	अगर (item &&
	    !odb_has_object(the_repository->objects, &item->oid, 0) &&
	    !oidset_contains(&fetch_oids, &item->oid))
		clear_item(item);

	/*
	 * For all the tags in the remote_refs_list,
	 * add them to the list of refs to be fetched
	 */
	for_each_string_list_item(remote_ref_item, &remote_refs_list) {
		स्थिर अक्षर *refname = remote_ref_item->string;
		काष्ठा ref *rm;
		अचिन्हित पूर्णांक hash = strhash(refname);

		item = hashmap_get_entry_from_hash(&remote_refs, hash, refname,
					काष्ठा refname_hash_entry, ent);
		अगर (!item)
			BUG("unseen remote ref?");

		/* Unless we have already decided to ignore this item... */
		अगर (item->ignore)
			जारी;

		rm = alloc_ref(item->refname);
		rm->peer_ref = alloc_ref(item->refname);
		oidcpy(&rm->old_oid, &item->oid);
		**पुच्छ = rm;
		*पुच्छ = &rm->अग्र;
	}
	hashmap_clear_and_free(&remote_refs, काष्ठा refname_hash_entry, ent);
	string_list_clear(&remote_refs_list, 0);
	oidset_clear(&fetch_oids);
}

अटल व्योम filter_prefetch_refspec(काष्ठा refspec *rs)
{
	पूर्णांक i;

	अगर (!prefetch)
		वापस;

	क्रम (i = 0; i < rs->nr; i++) {
		काष्ठा strbuf new_dst = STRBUF_INIT;
		अक्षर *old_dst;
		स्थिर अक्षर *sub = शून्य;

		अगर (rs->वस्तुएँ[i].negative)
			जारी;
		अगर (!rs->वस्तुएँ[i].dst ||
		    (rs->वस्तुएँ[i].src &&
		     starts_with(rs->वस्तुएँ[i].src,
				 ref_namespace[NAMESPACE_TAGS].ref))) {
			पूर्णांक j;

			refspec_item_clear(&rs->वस्तुएँ[i]);

			क्रम (j = i + 1; j < rs->nr; j++)
				rs->वस्तुएँ[j - 1] = rs->वस्तुएँ[j];
			rs->nr--;
			i--;
			जारी;
		}

		old_dst = rs->वस्तुएँ[i].dst;
		strbuf_addstr(&new_dst, ref_namespace[NAMESPACE_PREFETCH].ref);

		/*
		 * If old_dst starts with "refs/", then place
		 * sub after that prefix. Otherwise, start at
		 * the beginning of the string.
		 */
		अगर (!skip_prefix(old_dst, "refs/", &sub))
			sub = old_dst;
		strbuf_addstr(&new_dst, sub);

		rs->वस्तुएँ[i].dst = strbuf_detach(&new_dst, शून्य);
		rs->वस्तुएँ[i].force = 1;

		मुक्त(old_dst);
	}
}

अटल काष्ठा ref *get_ref_map(काष्ठा remote *remote,
			       स्थिर काष्ठा ref *remote_refs,
			       काष्ठा refspec *rs,
			       पूर्णांक tags, पूर्णांक *autotags)
{
	पूर्णांक i;
	काष्ठा ref *rm;
	काष्ठा ref *ref_map = शून्य;
	काष्ठा ref **पुच्छ = &ref_map;

	/* opportunistically-updated references: */
	काष्ठा ref *orefs = शून्य, **oref_पुच्छ = &orefs;

	काष्ठा hashmap existing_refs;
	पूर्णांक existing_refs_populated = 0;

	filter_prefetch_refspec(rs);
	अगर (remote)
		filter_prefetch_refspec(&remote->fetch);

	अगर (rs->nr) {
		काष्ठा refspec *fetch_refspec;

		क्रम (i = 0; i < rs->nr; i++) {
			get_fetch_map(remote_refs, &rs->वस्तुएँ[i], &पुच्छ, 0);
			अगर (rs->वस्तुएँ[i].dst && rs->वस्तुएँ[i].dst[0])
				*autotags = 1;
		}
		/* Merge everything on the command line (but not --tags) */
		क्रम (rm = ref_map; rm; rm = rm->अग्र)
			rm->fetch_शीर्ष_status = FETCH_शीर्ष_MERGE;

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
		अगर (refmap.nr)
			fetch_refspec = &refmap;
		अन्यथा
			fetch_refspec = &remote->fetch;

		क्रम (i = 0; i < fetch_refspec->nr; i++)
			get_fetch_map(ref_map, &fetch_refspec->वस्तुएँ[i], &oref_पुच्छ, 1);
	} अन्यथा अगर (refmap.nr) {
		die("--refmap option is only meaningful with command-line refspec(s)");
	} अन्यथा {
		/* Use the defaults */
		काष्ठा branch *branch = branch_get(शून्य);
		पूर्णांक has_merge = branch_has_merge_config(branch);
		अगर (remote &&
		    (remote->fetch.nr ||
		     /* Note: has_merge implies non-NULL branch->remote_name */
		     (has_merge && !म_भेद(branch->remote_name, remote->name)))) {
			क्रम (i = 0; i < remote->fetch.nr; i++) {
				get_fetch_map(remote_refs, &remote->fetch.वस्तुएँ[i], &पुच्छ, 0);
				अगर (remote->fetch.वस्तुएँ[i].dst &&
				    remote->fetch.वस्तुएँ[i].dst[0])
					*autotags = 1;
				अगर (!i && !has_merge && ref_map &&
				    !remote->fetch.वस्तुएँ[0].pattern)
					ref_map->fetch_शीर्ष_status = FETCH_शीर्ष_MERGE;
			}
			/*
			 * if the remote we're fetching from is the same
			 * as given in branch.<name>.remote, we add the
			 * ref given in branch.<name>.merge, too.
			 *
			 * Note: has_merge implies non-NULL branch->remote_name
			 */
			अगर (has_merge &&
			    !म_भेद(branch->remote_name, remote->name))
				add_merge_config(&ref_map, remote_refs, branch, &पुच्छ);
		} अन्यथा अगर (!prefetch) {
			ref_map = get_remote_ref(remote_refs, "HEAD");
			अगर (!ref_map)
				die(_("couldn't find remote ref HEAD"));
			ref_map->fetch_शीर्ष_status = FETCH_शीर्ष_MERGE;
			पुच्छ = &ref_map->अग्र;
		}
	}

	अगर (tags == TAGS_SET) {
		काष्ठा refspec_item tag_refspec;

		/* also fetch all tags */
		refspec_item_init_धकेलो(&tag_refspec, TAG_REFSPEC,
				       the_hash_algo);
		get_fetch_map(remote_refs, &tag_refspec, &पुच्छ, 0);
		refspec_item_clear(&tag_refspec);
	} अन्यथा अगर (tags == TAGS_DEFAULT && *autotags) {
		find_non_local_tags(remote_refs, शून्य, &ref_map, &पुच्छ);
	}

	/* Now append any refs to be updated opportunistically: */
	*पुच्छ = orefs;
	क्रम (rm = orefs; rm; rm = rm->अग्र) {
		rm->fetch_शीर्ष_status = FETCH_शीर्ष_IGNORE;
		पुच्छ = &rm->अग्र;
	}

	/*
	 * apply negative refspecs first, before we remove duplicates. This is
	 * necessary as negative refspecs might remove an otherwise conflicting
	 * duplicate.
	 */
	अगर (rs->nr)
		ref_map = apply_negative_refspecs(ref_map, rs);
	अन्यथा
		ref_map = apply_negative_refspecs(ref_map, &remote->fetch);

	ref_map = ref_remove_duplicates(ref_map);

	क्रम (rm = ref_map; rm; rm = rm->अग्र) {
		अगर (rm->peer_ref) {
			स्थिर अक्षर *refname = rm->peer_ref->name;
			काष्ठा refname_hash_entry *peer_item;
			अचिन्हित पूर्णांक hash = strhash(refname);

			अगर (!existing_refs_populated) {
				refname_hash_init(&existing_refs);
				refs_for_each_ref(get_main_ref_store(the_repository),
						  add_one_refname,
						  &existing_refs);
				existing_refs_populated = 1;
			}

			peer_item = hashmap_get_entry_from_hash(&existing_refs,
						hash, refname,
						काष्ठा refname_hash_entry, ent);
			अगर (peer_item) {
				काष्ठा object_id *old_oid = &peer_item->oid;
				oidcpy(&rm->peer_ref->old_oid, old_oid);
			}
		}
	}
	अगर (existing_refs_populated)
		hashmap_clear_and_free(&existing_refs, काष्ठा refname_hash_entry, ent);

	वापस ref_map;
}

अटल पूर्णांक s_update_ref(स्थिर अक्षर *action,
			काष्ठा ref *ref,
			काष्ठा ref_transaction *transaction,
			पूर्णांक check_old)
{
	अक्षर *संदेश;
	अक्षर *rla = दो_पर्या("GIT_REFLOG_ACTION");
	काष्ठा strbuf err = STRBUF_INIT;
	पूर्णांक ret;

	अगर (dry_run)
		वापस 0;
	अगर (!rla)
		rla = default_rla.buf;
	संदेश = xstrfmt("%s: %s", rla, action);

	ret = ref_transaction_update(transaction, ref->name, &ref->new_oid,
				     check_old ? &ref->old_oid : शून्य,
				     शून्य, शून्य, 0, संदेश, &err);

	अगर (ret)
		error("%s", err.buf);
	strbuf_release(&err);
	मुक्त(संदेश);
	वापस ret;
}

अटल पूर्णांक refcol_width(स्थिर काष्ठा ref *ref_map, पूर्णांक compact_format)
{
	स्थिर काष्ठा ref *ref;
	पूर्णांक max, width = 10;

	max = term_columns();
	अगर (compact_format)
		max = max * 2 / 3;

	क्रम (ref = ref_map; ref; ref = ref->अग्र) {
		पूर्णांक rlen, llen = 0, len;

		अगर (ref->status == REF_STATUS_REJECT_SHALLOW ||
		    !ref->peer_ref ||
		    !म_भेद(ref->name, "HEAD"))
			जारी;

		/* uptodate lines are only shown on high verbosity level */
		अगर (verbosity <= 0 && oideq(&ref->peer_ref->old_oid, &ref->old_oid))
			जारी;

		rlen = utf8_strwidth(prettify_refname(ref->name));
		अगर (!compact_format)
			llen = utf8_strwidth(prettify_refname(ref->peer_ref->name));

		/*
		 * rough estimation to see if the output line is too long and
		 * should not be counted (we can't do precise calculation
		 * anyway because we don't know if the error explanation part
		 * will be printed in update_local_ref)
		 */
		len = 21 /* flag and summary */ + rlen + 4 /* -> */ + llen;
		अगर (len >= max)
			जारी;

		अगर (width < rlen)
			width = rlen;
	}

	वापस width;
}

अटल व्योम display_state_init(काष्ठा display_state *display_state, काष्ठा ref *ref_map,
			       स्थिर अक्षर *raw_url, क्रमागत display_format format)
{
	पूर्णांक i;

	स_रखो(display_state, 0, माप(*display_state));
	strbuf_init(&display_state->buf, 0);
	display_state->format = format;

	अगर (raw_url)
		display_state->url = transport_anonymize_url(raw_url);
	अन्यथा
		display_state->url = xstrdup("foreign");

	display_state->url_len = म_माप(display_state->url);
	क्रम (i = display_state->url_len - 1; 0 <= i && display_state->url[i] == '/'; i--)
		;
	display_state->url_len = i + 1;
	अगर (4 < i && !म_भेदन(".git", display_state->url + i - 3, 4))
		display_state->url_len = i - 3;

	अगर (verbosity < 0)
		वापस;

	चयन (display_state->format) {
	हाल DISPLAY_FORMAT_FULL:
	हाल DISPLAY_FORMAT_COMPACT:
		display_state->refcol_width = refcol_width(ref_map,
							   display_state->format == DISPLAY_FORMAT_COMPACT);
		अवरोध;
	हाल DISPLAY_FORMAT_PORCELAIN:
		/* We don't need to precompute anything here. */
		अवरोध;
	शेष:
		BUG("unexpected display format %d", display_state->format);
	}
}

अटल व्योम display_state_release(काष्ठा display_state *display_state)
{
	strbuf_release(&display_state->buf);
	मुक्त(display_state->url);
}

अटल व्योम print_remote_to_local(काष्ठा display_state *display_state,
				  स्थिर अक्षर *remote, स्थिर अक्षर *local)
{
	strbuf_addf(&display_state->buf, "%-*s -> %s",
		    display_state->refcol_width, remote, local);
}

अटल पूर्णांक find_and_replace(काष्ठा strbuf *haystack,
			    स्थिर अक्षर *needle,
			    स्थिर अक्षर *placeholder)
{
	स्थिर अक्षर *p = शून्य;
	पूर्णांक plen, nlen;

	nlen = म_माप(needle);
	अगर (ends_with(haystack->buf, needle))
		p = haystack->buf + haystack->len - nlen;
	अन्यथा
		p = म_माला(haystack->buf, needle);
	अगर (!p)
		वापस 0;

	अगर (p > haystack->buf && p[-1] != '/')
		वापस 0;

	plen = म_माप(p);
	अगर (plen > nlen && p[nlen] != '/')
		वापस 0;

	strbuf_splice(haystack, p - haystack->buf, nlen,
		      placeholder, म_माप(placeholder));
	वापस 1;
}

अटल व्योम print_compact(काष्ठा display_state *display_state,
			  स्थिर अक्षर *remote, स्थिर अक्षर *local)
{
	काष्ठा strbuf r = STRBUF_INIT;
	काष्ठा strbuf l = STRBUF_INIT;

	अगर (!म_भेद(remote, local)) {
		strbuf_addf(&display_state->buf, "%-*s -> *", display_state->refcol_width, remote);
		वापस;
	}

	strbuf_addstr(&r, remote);
	strbuf_addstr(&l, local);

	अगर (!find_and_replace(&r, local, "*"))
		find_and_replace(&l, remote, "*");
	print_remote_to_local(display_state, r.buf, l.buf);

	strbuf_release(&r);
	strbuf_release(&l);
}

अटल व्योम display_ref_update(काष्ठा display_state *display_state, अक्षर code,
			       स्थिर अक्षर *summary, स्थिर अक्षर *error,
			       स्थिर अक्षर *remote, स्थिर अक्षर *local,
			       स्थिर काष्ठा object_id *old_oid,
			       स्थिर काष्ठा object_id *new_oid,
			       पूर्णांक summary_width)
{
	खाता *f = मानक_त्रुटि;

	अगर (verbosity < 0)
		वापस;

	strbuf_reset(&display_state->buf);

	चयन (display_state->format) {
	हाल DISPLAY_FORMAT_FULL:
	हाल DISPLAY_FORMAT_COMPACT: {
		पूर्णांक width;

		अगर (!display_state->shown_url) {
			strbuf_addf(&display_state->buf, _("From %.*s\n"),
				    display_state->url_len, display_state->url);
			display_state->shown_url = 1;
		}

		width = (summary_width + म_माप(summary) - gettext_width(summary));
		remote = prettify_refname(remote);
		local = prettify_refname(local);

		strbuf_addf(&display_state->buf, " %c %-*s ", code, width, summary);

		अगर (display_state->format != DISPLAY_FORMAT_COMPACT)
			print_remote_to_local(display_state, remote, local);
		अन्यथा
			print_compact(display_state, remote, local);

		अगर (error)
			strbuf_addf(&display_state->buf, "  (%s)", error);

		अवरोध;
	}
	हाल DISPLAY_FORMAT_PORCELAIN:
		strbuf_addf(&display_state->buf, "%c %s %s %s", code,
			    oid_to_hex(old_oid), oid_to_hex(new_oid), local);
		f = मानक_निकास;
		अवरोध;
	शेष:
		BUG("unexpected display format %d", display_state->format);
	};
	strbuf_addch(&display_state->buf, '\n');

	ख_माला_दो(display_state->buf.buf, f);
}

काष्ठा ref_update_display_info {
	bool failed;
	अक्षर success_code;
	अक्षर fail_code;
	अक्षर *summary;
	अक्षर *fail_detail;
	अक्षर *success_detail;
	अक्षर *ref;
	अक्षर *remote;
	काष्ठा object_id old_oid;
	काष्ठा object_id new_oid;
};

काष्ठा ref_update_display_info_array {
	काष्ठा ref_update_display_info *info;
	माप_प्रकार alloc, nr;
};

अटल काष्ठा ref_update_display_info *ref_update_display_info_append(
					   काष्ठा ref_update_display_info_array *array,
					   अक्षर success_code,
					   अक्षर fail_code,
					   स्थिर अक्षर *summary,
					   स्थिर अक्षर *success_detail,
					   स्थिर अक्षर *fail_detail,
					   स्थिर अक्षर *ref,
					   स्थिर अक्षर *remote,
					   स्थिर काष्ठा object_id *old_oid,
					   स्थिर काष्ठा object_id *new_oid)
{
	काष्ठा ref_update_display_info *info;

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

	वापस info;
}

अटल व्योम ref_update_display_info_set_failed(काष्ठा ref_update_display_info *info)
{
	info->failed = true;
}

अटल व्योम ref_update_display_info_free(काष्ठा ref_update_display_info *info)
{
	मुक्त(info->summary);
	मुक्त(info->success_detail);
	मुक्त(info->fail_detail);
	मुक्त(info->remote);
	मुक्त(info->ref);
}

अटल व्योम ref_update_display_info_display(काष्ठा ref_update_display_info *info,
					    काष्ठा display_state *display_state,
					    पूर्णांक summary_width)
{
	display_ref_update(display_state,
			   info->failed ? info->fail_code : info->success_code,
			   info->summary,
			   info->failed ? info->fail_detail : info->success_detail,
			   info->remote, info->ref, &info->old_oid,
			   &info->new_oid, summary_width);
}

अटल पूर्णांक update_local_ref(काष्ठा ref *ref,
			    काष्ठा ref_transaction *transaction,
			    स्थिर काष्ठा ref *remote_ref,
			    स्थिर काष्ठा fetch_config *config,
			    काष्ठा ref_update_display_info_array *display_array)
{
	काष्ठा commit *current = शून्य, *updated;
	पूर्णांक fast_forward = 0;

	अगर (!odb_has_object(the_repository->objects, &ref->new_oid,
			    ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR))
		die(_("object %s not found"), oid_to_hex(&ref->new_oid));

	अगर (oideq(&ref->old_oid, &ref->new_oid)) {
		अगर (verbosity > 0)
			ref_update_display_info_append(display_array, '=', '=',
						       _("[up to date]"), शून्य,
						       शून्य, ref->name,
						       remote_ref->name, &ref->old_oid,
						       &ref->new_oid);
		वापस 0;
	}

	अगर (!update_शीर्ष_ok &&
	    !is_null_oid(&ref->old_oid) &&
	    branch_checked_out(ref->name)) {
		काष्ठा ref_update_display_info *info;
		/*
		 * If this is the head, and it's not okay to update
		 * the head, and the old value of the head isn't empty...
		 */
		info = ref_update_display_info_append(display_array, '!', '!',
						      _("[rejected]"), शून्य,
						      _("can't fetch into checked-out branch"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);
		ref_update_display_info_set_failed(info);
		वापस 1;
	}

	अगर (!is_null_oid(&ref->old_oid) &&
	    starts_with(ref->name, "refs/tags/")) {
		काष्ठा ref_update_display_info *info;

		अगर (force || ref->force) {
			पूर्णांक r;

			r = s_update_ref("updating tag", ref, transaction, 0);

			info = ref_update_display_info_append(display_array, 't', '!',
							      _("[tag update]"), शून्य,
							      _("unable to update local ref"),
							      ref->name, remote_ref->name,
							      &ref->old_oid, &ref->new_oid);
			अगर (r)
				ref_update_display_info_set_failed(info);

			वापस r;
		} अन्यथा {
			info = ref_update_display_info_append(display_array, '!', '!',
							      _("[rejected]"), शून्य,
							      _("would clobber existing tag"),
							      ref->name, remote_ref->name,
							      &ref->old_oid, &ref->new_oid);
			ref_update_display_info_set_failed(info);
			वापस 1;
		}
	}

	current = lookup_commit_reference_gently(the_repository,
						 &ref->old_oid, 1);
	updated = lookup_commit_reference_gently(the_repository,
						 &ref->new_oid, 1);
	अगर (!current || !updated) {
		काष्ठा ref_update_display_info *info;
		स्थिर अक्षर *संदेश;
		स्थिर अक्षर *what;
		पूर्णांक r;
		/*
		 * Nicely describe the new ref we're fetching.
		 * Base this on the remote's ref name, as it's
		 * more likely to follow a standard layout.
		 */
		अगर (starts_with(remote_ref->name, "refs/tags/")) {
			संदेश = "storing tag";
			what = _("[new tag]");
		} अन्यथा अगर (starts_with(remote_ref->name, "refs/heads/")) {
			संदेश = "storing head";
			what = _("[new branch]");
		} अन्यथा {
			संदेश = "storing ref";
			what = _("[new ref]");
		}

		r = s_update_ref(संदेश, ref, transaction, 0);

		info = ref_update_display_info_append(display_array, '*', '!',
						      what, शून्य,
						      _("unable to update local ref"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);
		अगर (r)
			ref_update_display_info_set_failed(info);

		वापस r;
	}

	अगर (config->show_forced_updates) {
		uint64_t t_before = getnanotime();
		fast_forward = repo_in_merge_bases(the_repository, current,
						   updated);
		अगर (fast_forward < 0)
			die(शून्य);
		forced_updates_ms += (getnanotime() - t_before) / 1000000;
	} अन्यथा {
		fast_forward = 1;
	}

	अगर (fast_forward) {
		काष्ठा ref_update_display_info *info;
		काष्ठा strbuf quickref = STRBUF_INIT;
		पूर्णांक r;

		strbuf_add_unique_abbrev(&quickref, &current->object.oid, DEFAULT_ABBREV);
		strbuf_addstr(&quickref, "..");
		strbuf_add_unique_abbrev(&quickref, &ref->new_oid, DEFAULT_ABBREV);
		r = s_update_ref("fast-forward", ref, transaction, 1);

		info = ref_update_display_info_append(display_array, ' ', '!',
						      quickref.buf, शून्य,
						      _("unable to update local ref"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);
		अगर (r)
			ref_update_display_info_set_failed(info);

		strbuf_release(&quickref);
		वापस r;
	} अन्यथा अगर (force || ref->force) {
		काष्ठा ref_update_display_info *info;
		काष्ठा strbuf quickref = STRBUF_INIT;
		पूर्णांक r;

		strbuf_add_unique_abbrev(&quickref, &current->object.oid, DEFAULT_ABBREV);
		strbuf_addstr(&quickref, "...");
		strbuf_add_unique_abbrev(&quickref, &ref->new_oid, DEFAULT_ABBREV);
		r = s_update_ref("forced-update", ref, transaction, 1);

		info = ref_update_display_info_append(display_array, '+', '!',
						      quickref.buf, _("forced update"),
						      _("unable to update local ref"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);

		अगर (r)
			ref_update_display_info_set_failed(info);

		strbuf_release(&quickref);
		वापस r;
	} अन्यथा {
		काष्ठा ref_update_display_info *info;
		info = ref_update_display_info_append(display_array, '!', '!',
						      _("[rejected]"), शून्य,
						      _("non-fast-forward"),
						      ref->name, remote_ref->name,
						      &ref->old_oid, &ref->new_oid);
		ref_update_display_info_set_failed(info);
		वापस 1;
	}
}

अटल स्थिर काष्ठा object_id *iterate_ref_map(व्योम *cb_तथ्य)
{
	काष्ठा ref **rm = cb_तथ्य;
	काष्ठा ref *ref = *rm;

	जबतक (ref && ref->status == REF_STATUS_REJECT_SHALLOW)
		ref = ref->अग्र;
	अगर (!ref)
		वापस शून्य;
	*rm = ref->अग्र;
	वापस &ref->old_oid;
}

काष्ठा fetch_शीर्ष {
	खाता *fp;
	काष्ठा strbuf buf;
};

अटल पूर्णांक open_fetch_शीर्ष(काष्ठा fetch_शीर्ष *fetch_शीर्ष)
{
	स्थिर अक्षर *filename = git_path_fetch_शीर्ष(the_repository);

	अगर (write_fetch_शीर्ष) {
		fetch_शीर्ष->fp = ख_खोलो(filename, "a");
		अगर (!fetch_शीर्ष->fp)
			वापस error_errno(_("cannot open '%s'"), filename);
		strbuf_init(&fetch_शीर्ष->buf, 0);
	} अन्यथा {
		fetch_शीर्ष->fp = शून्य;
	}

	वापस 0;
}

अटल व्योम append_fetch_शीर्ष(काष्ठा fetch_शीर्ष *fetch_शीर्ष,
			      स्थिर काष्ठा object_id *old_oid,
			      क्रमागत fetch_शीर्ष_status fetch_शीर्ष_status,
			      स्थिर अक्षर *note,
			      स्थिर अक्षर *url, माप_प्रकार url_len)
{
	अक्षर old_oid_hex[GIT_MAX_HEXSZ + 1];
	स्थिर अक्षर *merge_status_marker;
	माप_प्रकार i;

	अगर (!fetch_शीर्ष->fp)
		वापस;

	चयन (fetch_शीर्ष_status) {
	हाल FETCH_शीर्ष_NOT_FOR_MERGE:
		merge_status_marker = "not-for-merge";
		अवरोध;
	हाल FETCH_शीर्ष_MERGE:
		merge_status_marker = "";
		अवरोध;
	शेष:
		/* do not write anything to FETCH_HEAD */
		वापस;
	}

	strbuf_addf(&fetch_शीर्ष->buf, "%s\t%s\t%s",
		    oid_to_hex_r(old_oid_hex, old_oid), merge_status_marker, note);
	क्रम (i = 0; i < url_len; ++i)
		अगर ('\n' == url[i])
			strbuf_addstr(&fetch_शीर्ष->buf, "\\n");
		अन्यथा
			strbuf_addch(&fetch_शीर्ष->buf, url[i]);
	strbuf_addch(&fetch_शीर्ष->buf, '\n');

	/*
	 * When using an atomic fetch, we do not want to update FETCH_HEAD if
	 * any of the reference updates fails. We thus have to write all
	 * updates to a buffer first and only commit it as soon as all
	 * references have been successfully updated.
	 */
	अगर (!atomic_fetch) {
		strbuf_write(&fetch_शीर्ष->buf, fetch_शीर्ष->fp);
		strbuf_reset(&fetch_शीर्ष->buf);
	}
}

अटल व्योम commit_fetch_शीर्ष(काष्ठा fetch_शीर्ष *fetch_शीर्ष)
{
	अगर (!fetch_शीर्ष->fp || !atomic_fetch)
		वापस;
	strbuf_write(&fetch_शीर्ष->buf, fetch_शीर्ष->fp);
}

अटल व्योम close_fetch_शीर्ष(काष्ठा fetch_शीर्ष *fetch_शीर्ष)
{
	अगर (!fetch_शीर्ष->fp)
		वापस;

	ख_बंद(fetch_शीर्ष->fp);
	strbuf_release(&fetch_शीर्ष->buf);
}

अटल स्थिर अक्षर warn_show_forced_updates[] =
N_("fetch normally indicates which branches had a forced update,\n"
   "but that check has been disabled; to re-enable, use '--show-forced-updates'\n"
   "flag or run 'git config fetch.showForcedUpdates true'");
अटल स्थिर अक्षर warn_time_show_forced_updates[] =
N_("it took %.2f seconds to check forced updates; you can use\n"
   "'--no-show-forced-updates' or run 'git config fetch.showForcedUpdates false'\n"
   "to avoid this check\n");

अटल पूर्णांक store_updated_refs(काष्ठा display_state *display_state,
			      पूर्णांक connectivity_checked,
			      काष्ठा ref_transaction *transaction, काष्ठा ref *ref_map,
			      काष्ठा fetch_शीर्ष *fetch_शीर्ष,
			      स्थिर काष्ठा fetch_config *config,
			      काष्ठा ref_update_display_info_array *display_array)
{
	पूर्णांक rc = 0;
	काष्ठा strbuf note = STRBUF_INIT;
	स्थिर अक्षर *what, *kind;
	काष्ठा ref *rm;
	पूर्णांक want_status;

	अगर (!connectivity_checked) {
		काष्ठा check_connected_options opt = CHECK_CONNECTED_INIT;

		opt.exclude_hidden_refs_section = "fetch";
		rm = ref_map;
		अगर (check_connected(iterate_ref_map, &rm, &opt)) {
			rc = error(_("%s did not send all necessary objects"),
				   display_state->url);
			जाओ पात;
		}
	}

	/*
	 * We do a pass for each fetch_head_status type in their enum order, so
	 * merged entries are written before not-for-merge. That lets readers
	 * use FETCH_HEAD as a refname to refer to the ref to be merged.
	 */
	क्रम (want_status = FETCH_शीर्ष_MERGE;
	     want_status <= FETCH_शीर्ष_IGNORE;
	     want_status++) {
		क्रम (rm = ref_map; rm; rm = rm->अग्र) {
			काष्ठा ref *ref = शून्य;

			अगर (rm->status == REF_STATUS_REJECT_SHALLOW) {
				अगर (want_status == FETCH_शीर्ष_MERGE)
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
			अगर (fetch_शीर्ष->fp) {
				काष्ठा commit *commit = शून्य;

				/*
				 * References in "refs/tags/" are often going to point
				 * to annotated tags, which are not part of the
				 * commit-graph. We thus only try to look up refs in
				 * the graph which are not in that namespace to not
				 * regress performance in repositories with many
				 * annotated tags.
				 */
				अगर (!starts_with(rm->name, "refs/tags/"))
					commit = lookup_commit_in_graph(the_repository, &rm->old_oid);
				अगर (!commit) {
					commit = lookup_commit_reference_gently(the_repository,
										&rm->old_oid,
										1);
					अगर (!commit)
						rm->fetch_शीर्ष_status = FETCH_शीर्ष_NOT_FOR_MERGE;
				}
			}

			अगर (rm->fetch_शीर्ष_status != want_status)
				जारी;

			अगर (rm->peer_ref) {
				ref = alloc_ref(rm->peer_ref->name);
				oidcpy(&ref->old_oid, &rm->peer_ref->old_oid);
				oidcpy(&ref->new_oid, &rm->old_oid);
				ref->force = rm->peer_ref->force;
			}

			अगर (config->recurse_submodules != RECURSE_SUBMODULES_OFF &&
			    (!rm->peer_ref || !oideq(&ref->old_oid, &ref->new_oid))) {
				check_for_new_submodule_commits(&rm->old_oid);
			}

			अगर (!म_भेद(rm->name, "HEAD")) {
				kind = "";
				what = "";
			} अन्यथा अगर (skip_prefix(rm->name, "refs/heads/", &what)) {
				kind = "branch";
			} अन्यथा अगर (skip_prefix(rm->name, "refs/tags/", &what)) {
				kind = "tag";
			} अन्यथा अगर (skip_prefix(rm->name, "refs/remotes/", &what)) {
				kind = "remote-tracking branch";
			} अन्यथा {
				kind = "";
				what = rm->name;
			}

			strbuf_reset(&note);
			अगर (*what) {
				अगर (*kind)
					strbuf_addf(&note, "%s ", kind);
				strbuf_addf(&note, "'%s' of ", what);
			}

			append_fetch_शीर्ष(fetch_शीर्ष, &rm->old_oid,
					  rm->fetch_शीर्ष_status,
					  note.buf, display_state->url,
					  display_state->url_len);

			अगर (ref) {
				rc |= update_local_ref(ref, transaction, rm,
						       config, display_array);
				मुक्त(ref);
			} अन्यथा अगर (write_fetch_शीर्ष || dry_run) {
				/*
				 * Display fetches written to FETCH_HEAD (or
				 * would be written to FETCH_HEAD, if --dry-run
				 * is set).
				 */

				ref_update_display_info_append(display_array, '*', '*',
							       *kind ? kind : "branch",
							       शून्य, शून्य, "FETCH_HEAD",
							       rm->name, &rm->new_oid,
							       &rm->old_oid);
			}
		}
	}

	अगर (advice_enabled(ADVICE_FETCH_SHOW_FORCED_UPDATES)) {
		अगर (!config->show_forced_updates) {
			warning(_(warn_show_forced_updates));
		} अन्यथा अगर (forced_updates_ms > FORCED_UPDATES_DELAY_WARNING_IN_MS) {
			warning(_(warn_time_show_forced_updates),
				forced_updates_ms / 1000.0);
		}
	}

 पात:
	strbuf_release(&note);
	वापस rc;
}

/*
 * We would want to bypass the object transfer altogether if
 * everything we are going to fetch already exists and is connected
 * locally.
 */
अटल पूर्णांक check_exist_and_connected(काष्ठा ref *ref_map)
{
	काष्ठा ref *rm = ref_map;
	काष्ठा check_connected_options opt = CHECK_CONNECTED_INIT;
	काष्ठा ref *r;

	/*
	 * If we are deepening a shallow clone we already have these
	 * objects reachable.  Running rev-list here will return with
	 * a good (0) exit status and we'll bypass the fetch that we
	 * really need to perform.  Claiming failure now will ensure
	 * we perform the network exchange to deepen our history.
	 */
	अगर (deepen)
		वापस -1;

	/*
	 * Similarly, if we need to refetch, we always want to perform a full
	 * fetch ignoring existing objects.
	 */
	अगर (refetch)
		वापस -1;


	/*
	 * check_connected() allows objects to merely be promised, but
	 * we need all direct targets to exist.
	 */
	क्रम (r = rm; r; r = r->अग्र) {
		अगर (!odb_has_object(the_repository->objects, &r->old_oid,
				    ODB_HAS_OBJECT_RECHECK_PACKED))
			वापस -1;
	}

	opt.quiet = 1;
	opt.exclude_hidden_refs_section = "fetch";
	वापस check_connected(iterate_ref_map, &rm, &opt);
}

अटल पूर्णांक fetch_and_consume_refs(काष्ठा display_state *display_state,
				  काष्ठा transport *transport,
				  काष्ठा ref_transaction *transaction,
				  काष्ठा ref *ref_map,
				  काष्ठा fetch_शीर्ष *fetch_शीर्ष,
				  स्थिर काष्ठा fetch_config *config,
				  काष्ठा ref_update_display_info_array *display_array)
{
	पूर्णांक connectivity_checked = 1;
	पूर्णांक ret;

	/*
	 * We don't need to perform a fetch in case we can already satisfy all
	 * refs.
	 */
	ret = check_exist_and_connected(ref_map);
	अगर (ret) {
		trace2_region_enter("fetch", "fetch_refs", the_repository);
		ret = transport_fetch_refs(transport, ref_map);
		trace2_region_leave("fetch", "fetch_refs", the_repository);
		अगर (ret)
			जाओ out;
		connectivity_checked = transport->smart_options ?
			transport->smart_options->connectivity_checked : 0;
	}

	trace2_region_enter("fetch", "consume_refs", the_repository);
	ret = store_updated_refs(display_state, connectivity_checked,
				 transaction, ref_map, fetch_शीर्ष, config,
				 display_array);
	trace2_region_leave("fetch", "consume_refs", the_repository);

out:
	transport_unlock_pack(transport, 0);
	वापस ret;
}

अटल पूर्णांक prune_refs(काष्ठा display_state *display_state,
		      काष्ठा refspec *rs,
		      काष्ठा ref_transaction *transaction,
		      काष्ठा ref *ref_map)
{
	पूर्णांक परिणाम = 0;
	काष्ठा ref *ref, *stale_refs = get_stale_heads(rs, ref_map);
	काष्ठा strbuf err = STRBUF_INIT;
	काष्ठा string_list refnames = STRING_LIST_INIT_NODUP;

	क्रम (ref = stale_refs; ref; ref = ref->अग्र)
		string_list_append(&refnames, ref->name);

	अगर (!dry_run) {
		अगर (transaction) {
			क्रम (ref = stale_refs; ref; ref = ref->अग्र) {
				परिणाम = ref_transaction_delete(transaction, ref->name, शून्य,
								शून्य, 0, "fetch: prune", &err);
				अगर (परिणाम)
					जाओ cleanup;
			}
		} अन्यथा {
			परिणाम = refs_delete_refs(get_main_ref_store(the_repository),
						  "fetch: prune", &refnames,
						  0);
		}
	}

	अगर (verbosity >= 0) {
		पूर्णांक summary_width = transport_summary_width(stale_refs);

		क्रम (ref = stale_refs; ref; ref = ref->अग्र) {
			display_ref_update(display_state, '-', _("[deleted]"), शून्य,
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
	वापस परिणाम;
}

अटल व्योम check_not_current_branch(काष्ठा ref *ref_map)
{
	स्थिर अक्षर *path;
	क्रम (; ref_map; ref_map = ref_map->अग्र)
		अगर (ref_map->peer_ref &&
		    starts_with(ref_map->peer_ref->name, "refs/heads/") &&
		    (path = branch_checked_out(ref_map->peer_ref->name)))
			die(_("refusing to fetch into branch '%s' "
			      "checked out at '%s'"),
			    ref_map->peer_ref->name, path);
}

अटल पूर्णांक truncate_fetch_शीर्ष(व्योम)
{
	स्थिर अक्षर *filename = git_path_fetch_शीर्ष(the_repository);
	खाता *fp = fopen_for_writing(filename);

	अगर (!fp)
		वापस error_errno(_("cannot open '%s'"), filename);
	ख_बंद(fp);
	वापस 0;
}

अटल व्योम set_option(काष्ठा transport *transport, स्थिर अक्षर *name, स्थिर अक्षर *मूल्य)
{
	पूर्णांक r = transport_set_option(transport, name, मूल्य);
	अगर (r < 0)
		die(_("option \"%s\" value \"%s\" is not valid for %s"),
		    name, मूल्य, transport->url);
	अगर (r > 0)
		warning(_("option \"%s\" is ignored for %s"),
			name, transport->url);
}


अटल पूर्णांक add_oid(स्थिर काष्ठा reference *ref, व्योम *cb_तथ्य)
{
	काष्ठा oid_array *oids = cb_तथ्य;

	oid_array_append(oids, ref->oid);
	वापस 0;
}

अटल व्योम add_negotiation_tips(काष्ठा string_list *निवेश_list,
				 काष्ठा oid_array **output_list,
				 स्थिर अक्षर *argname)
{
	काष्ठा oid_array *oids = xcalloc(1, माप(*oids));
	पूर्णांक i;

	क्रम (i = 0; i < निवेश_list->nr; i++) {
		स्थिर अक्षर *s = निवेश_list->वस्तुएँ[i].string;
		काष्ठा refs_for_each_ref_options opts = {
			.pattern = s,
		};
		पूर्णांक old_nr;
		अगर (!has_glob_specials(s)) {
			काष्ठा object_id oid;

			/* Ignore missing reference. */
			अगर (repo_get_oid(the_repository, s, &oid))
				जारी;
			/* Fail on missing object pointed by ref. */
			अगर (!odb_has_object(the_repository->objects, &oid, 0))
				die(_("the object %s does not exist"), s);

			oid_array_append(oids, &oid);
			जारी;
		}
		old_nr = oids->nr;
		refs_for_each_ref_ext(get_main_ref_store(the_repository),
				      add_oid, oids, &opts);
		अगर (old_nr == oids->nr)
			warning(_("ignoring %s=%s because it does not match any refs"),
				argname, s);
	}
	*output_list = oids;
}

अटल काष्ठा transport *prepare_transport(काष्ठा remote *remote, पूर्णांक deepen,
					   काष्ठा list_objects_filter_options *filter_options)
{
	काष्ठा transport *transport;

	transport = transport_get(remote, शून्य);
	transport_set_verbosity(transport, verbosity, progress);
	transport->family = family;
	अगर (upload_pack)
		set_option(transport, TRANS_OPT_UPLOADPACK, upload_pack);
	अगर (keep)
		set_option(transport, TRANS_OPT_KEEP, "yes");
	अगर (depth)
		set_option(transport, TRANS_OPT_DEPTH, depth);
	अगर (deepen && deepen_since)
		set_option(transport, TRANS_OPT_DEEPEN_SINCE, deepen_since);
	अगर (deepen && deepen_not.nr)
		set_option(transport, TRANS_OPT_DEEPEN_NOT,
			   (स्थिर अक्षर *)&deepen_not);
	अगर (deepen_relative)
		set_option(transport, TRANS_OPT_DEEPEN_RELATIVE, "yes");
	अगर (update_shallow)
		set_option(transport, TRANS_OPT_UPDATE_SHALLOW, "yes");
	अगर (refetch)
		set_option(transport, TRANS_OPT_REFETCH, "yes");
	अगर (filter_options->choice) {
		स्थिर अक्षर *spec =
			expand_list_objects_filter_spec(filter_options);
		set_option(transport, TRANS_OPT_LIST_OBJECTS_FILTER, spec);
		set_option(transport, TRANS_OPT_FROM_PROMISOR, "1");
	}
	अगर (negotiation_restrict.nr) {
		अगर (transport->smart_options)
			add_negotiation_tips(&negotiation_restrict,
					     &transport->smart_options->negotiation_restrict_tips,
					     "--negotiation-restrict");
		अन्यथा
			warning(_("ignoring %s because the protocol does not support it"),
				"--negotiation-restrict");
	} अन्यथा अगर (remote->negotiation_restrict.nr) {
		काष्ठा string_list_item *item;
		for_each_string_list_item(item, &remote->negotiation_restrict)
			string_list_append(&negotiation_restrict, item->string);
		अगर (transport->smart_options)
			add_negotiation_tips(&negotiation_restrict,
					     &transport->smart_options->negotiation_restrict_tips,
					     "--negotiation-restrict");
		अन्यथा {
			काष्ठा strbuf config_name = STRBUF_INIT;
			strbuf_addf(&config_name, "remote.%s.negotiationRestrict", remote->name);
			warning(_("ignoring %s because the protocol does not support it"),
				config_name.buf);
			strbuf_release(&config_name);
		}
	}
	अगर (negotiation_include.nr) {
		अगर (transport->smart_options)
			add_negotiation_tips(&negotiation_include,
					     &transport->smart_options->negotiation_include_tips,
					     "--negotiation-include");
		अन्यथा
			warning(_("ignoring %s because the protocol does not support it"),
				"--negotiation-include");
	} अन्यथा अगर (remote->negotiation_include.nr) {
		अगर (transport->smart_options) {
			add_negotiation_tips(&remote->negotiation_include,
					     &transport->smart_options->negotiation_include_tips,
					     "--negotiation-include");
		} अन्यथा {
			काष्ठा strbuf config_name = STRBUF_INIT;
			strbuf_addf(&config_name, "remote.%s.negotiationInclude", remote->name);
			warning(_("ignoring %s because the protocol does not support it"),
				config_name.buf);
			strbuf_release(&config_name);
		}
	}
	वापस transport;
}

अटल पूर्णांक backfill_tags(काष्ठा display_state *display_state,
			 काष्ठा transport *transport,
			 काष्ठा ref_transaction *transaction,
			 काष्ठा ref *ref_map,
			 काष्ठा fetch_शीर्ष *fetch_शीर्ष,
			 स्थिर काष्ठा fetch_config *config,
			 काष्ठा ref_update_display_info_array *display_array,
			 काष्ठा list_objects_filter_options *filter_options)
{
	पूर्णांक retcode, cannot_reuse;

	/*
	 * Once we have set TRANS_OPT_DEEPEN_SINCE, we can't unset it
	 * when remote helper is used (setting it to an empty string
	 * is not unsetting). We could extend the remote helper
	 * protocol for that, but for now, just force a new connection
	 * without deepen-since. Similar story for deepen-not.
	 */
	cannot_reuse = transport->cannot_reuse ||
		deepen_since || deepen_not.nr;
	अगर (cannot_reuse) {
		gsecondary = prepare_transport(transport->remote, 0, filter_options);
		transport = gsecondary;
	}

	transport_set_option(transport, TRANS_OPT_FOLLOWTAGS, शून्य);
	transport_set_option(transport, TRANS_OPT_DEPTH, "0");
	transport_set_option(transport, TRANS_OPT_DEEPEN_RELATIVE, शून्य);
	retcode = fetch_and_consume_refs(display_state, transport, transaction, ref_map,
					 fetch_शीर्ष, config, display_array);

	अगर (gsecondary) {
		transport_disconnect(gsecondary);
		gsecondary = शून्य;
	}

	वापस retcode;
}

अटल स्थिर अक्षर *strip_refshead(स्थिर अक्षर *name){
	skip_prefix(name, "refs/heads/", &name);
	वापस name;
}

अटल व्योम set_शीर्ष_advice_संदेश(स्थिर अक्षर *remote, स्थिर अक्षर *शीर्ष_name)
{
	स्थिर अक्षर message_advice_set_शीर्ष[] =
	N_("Run 'git remote set-head %s %s' to follow the change, or modify\n"
	   "either of the 'remote.%s.followRemoteHEAD' or 'fetch.followRemoteHEAD'\n"
	   "configuration variables to handle the situation differently.\n\n"

	   "Using this specific setting\n\n"
	   "    git config set remote.%s.followRemoteHEAD warn-if-not-%s\n\n"
	   "will suppress the warning until the remote changes HEAD to something else.");

	advise_if_enabled(ADVICE_FETCH_SET_शीर्ष_WARN, _(message_advice_set_शीर्ष),
			remote, शीर्ष_name, remote, remote, शीर्ष_name);
}

अटल व्योम warn_set_शीर्ष(स्थिर अक्षर *remote, स्थिर अक्षर *शीर्ष_name,
			काष्ठा strbuf *buf_पूर्व, पूर्णांक updateres) {
	काष्ठा strbuf buf_prefix = STRBUF_INIT;
	स्थिर अक्षर *पूर्व_शीर्ष = शून्य;

	strbuf_addf(&buf_prefix, "refs/remotes/%s/", remote);
	skip_prefix(buf_पूर्व->buf, buf_prefix.buf, &पूर्व_शीर्ष);

	अगर (पूर्व_शीर्ष && म_भेद(पूर्व_शीर्ष, शीर्ष_name)) {
		म_लिखो("'HEAD' at '%s' is '%s', but we have '%s' locally.\n",
			remote, शीर्ष_name, पूर्व_शीर्ष);
		set_शीर्ष_advice_संदेश(remote, शीर्ष_name);
	}
	अन्यथा अगर (updateres && buf_पूर्व->len) {
		म_लिखो("'HEAD' at '%s' is '%s', "
			"but we have a detached HEAD pointing to '%s' locally.\n",
			remote, शीर्ष_name, buf_पूर्व->buf);
		set_शीर्ष_advice_संदेश(remote, शीर्ष_name);
	}
	strbuf_release(&buf_prefix);
}

अटल पूर्णांक set_शीर्ष(स्थिर काष्ठा ref *remote_refs, काष्ठा remote *remote,
			पूर्णांक follow_remote_शीर्ष)
{
	पूर्णांक परिणाम = 0, create_only, baremirror, was_detached;
	काष्ठा strbuf b_शीर्ष = STRBUF_INIT, b_remote_शीर्ष = STRBUF_INIT,
		      b_local_शीर्ष = STRBUF_INIT;
	स्थिर अक्षर *no_warn_branch = remote->no_warn_branch;
	अक्षर *शीर्ष_name = शून्य;
	काष्ठा ref *ref, *matches;
	काष्ठा ref *fetch_map = शून्य, **fetch_map_पुच्छ = &fetch_map;
	काष्ठा refspec_item refspec = {
		.force = 0,
		.pattern = 1,
		.src = (अक्षर *) "refs/heads/*",
		.dst = (अक्षर *) "refs/heads/*",
	};
	काष्ठा string_list heads = STRING_LIST_INIT_DUP;
	काष्ठा ref_store *refs = get_main_ref_store(the_repository);

	get_fetch_map(remote_refs, &refspec, &fetch_map_पुच्छ, 0);
	matches = guess_remote_शीर्ष(find_ref_by_name(remote_refs, "HEAD"),
				    fetch_map, REMOTE_GUESS_शीर्ष_ALL);
	क्रम (ref = matches; ref; ref = ref->अग्र) {
		string_list_append(&heads, strip_refshead(ref->name));
	}

	अगर (!heads.nr)
		परिणाम = 1;
	अन्यथा अगर (heads.nr > 1)
		परिणाम = 1;
	अन्यथा
		शीर्ष_name = xstrdup(heads.वस्तुएँ[0].string);

	अगर (!शीर्ष_name)
		जाओ cleanup;
	baremirror = is_bare_repository(the_repository) && remote->mirror;
	create_only = follow_remote_शीर्ष == FOLLOW_REMOTE_ALWAYS ? 0 : !baremirror;
	अगर (baremirror) {
		strbuf_addstr(&b_शीर्ष, "HEAD");
		strbuf_addf(&b_remote_शीर्ष, "refs/heads/%s", शीर्ष_name);
	} अन्यथा {
		strbuf_addf(&b_शीर्ष, "refs/remotes/%s/HEAD", remote->name);
		strbuf_addf(&b_remote_शीर्ष, "refs/remotes/%s/%s", remote->name, शीर्ष_name);
	}
	/* make sure it's valid */
	अगर (!baremirror && !refs_ref_exists(refs, b_remote_शीर्ष.buf)) {
		परिणाम = 1;
		जाओ cleanup;
	}
	was_detached = refs_update_symref_extended(refs, b_शीर्ष.buf, b_remote_शीर्ष.buf,
					"fetch", &b_local_शीर्ष, create_only);
	अगर (was_detached == -1) {
		परिणाम = 1;
		जाओ cleanup;
	}
	अगर (verbosity >= 0 &&
		follow_remote_शीर्ष == FOLLOW_REMOTE_WARN &&
		(!no_warn_branch || म_भेद(no_warn_branch, शीर्ष_name)))
		warn_set_शीर्ष(remote->name, शीर्ष_name, &b_local_शीर्ष, was_detached);

cleanup:
	मुक्त(शीर्ष_name);
	free_refs(fetch_map);
	free_refs(matches);
	string_list_clear(&heads, 0);
	strbuf_release(&b_शीर्ष);
	strbuf_release(&b_local_शीर्ष);
	strbuf_release(&b_remote_शीर्ष);
	वापस परिणाम;
}

काष्ठा ref_rejection_तथ्य {
	पूर्णांक *retcode;
	bool conflict_संदेश_shown;
	bool case_sensitive_संदेश_shown;
	स्थिर अक्षर *remote_name;
	काष्ठा strmap *rejected_refs;
};

अटल व्योम ref_transaction_rejection_handler(स्थिर अक्षर *refname,
					      स्थिर काष्ठा object_id *old_oid UNUSED,
					      स्थिर काष्ठा object_id *new_oid UNUSED,
					      स्थिर अक्षर *old_target UNUSED,
					      स्थिर अक्षर *new_target UNUSED,
					      क्रमागत ref_transaction_error err,
					      स्थिर अक्षर *details,
					      व्योम *cb_तथ्य)
{
	काष्ठा ref_rejection_तथ्य *तथ्य = cb_तथ्य;

	अगर (err == REF_TRANSACTION_ERROR_CASE_CONFLICT && repo_ignore_case(the_repository) &&
	    !तथ्य->case_sensitive_संदेश_shown) {
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
		तथ्य->case_sensitive_संदेश_shown = true;
	} अन्यथा अगर (err == REF_TRANSACTION_ERROR_NAME_CONFLICT &&
		   !तथ्य->conflict_संदेश_shown) {
		error(_("some local refs could not be updated; try running\n"
			" 'git remote prune %s' to remove any old, conflicting "
			"branches"), तथ्य->remote_name);
		तथ्य->conflict_संदेश_shown = true;
	} अन्यथा {
		अगर (details)
			error("%s", details);
		अन्यथा
			error(_("fetching ref %s failed: %s"),
			      refname, ref_transaction_error_संदेश(err));
	}

	strmap_put(तथ्य->rejected_refs, refname, शून्य);
	*तथ्य->retcode = 1;
}

/*
 * Commit the reference transaction. If it isn't an atomic transaction, handle
 * rejected updates as part of using batched updates.
 */
अटल पूर्णांक commit_ref_transaction(काष्ठा ref_transaction **transaction,
				  bool is_atomic, स्थिर अक्षर *remote_name,
				  काष्ठा strmap *rejected_refs,
				  काष्ठा strbuf *err)
{
	पूर्णांक retcode = ref_transaction_commit(*transaction, err);
	अगर (retcode)
		जाओ out;

	अगर (!is_atomic) {
		काष्ठा ref_rejection_तथ्य तथ्य = {
			.conflict_संदेश_shown = 0,
			.remote_name = remote_name,
			.retcode = &retcode,
			.rejected_refs = rejected_refs,
		};

		ref_transaction_for_each_rejected_update(*transaction,
							 ref_transaction_rejection_handler,
							 &तथ्य);
	}

out:
	ref_transaction_free(*transaction);
	*transaction = शून्य;
	वापस retcode;
}

अटल व्योम collect_updated_tips(काष्ठा oidset *tips, काष्ठा ref *ref_map)
{
	काष्ठा ref *rm;
	क्रम (rm = ref_map; rm; rm = rm->अग्र) {
		काष्ठा commit *commit;
		/*
		 * Shallow-rejected refs are not stored and their history
		 * is incomplete, so skip them.
		 */
		अगर (rm->status == REF_STATUS_REJECT_SHALLOW)
			जारी;
		अगर (is_null_oid(&rm->old_oid))
			जारी;
		अगर (rm->peer_ref &&
		    oideq(&rm->old_oid, &rm->peer_ref->old_oid))
			जारी;
		commit = lookup_commit_reference_gently(the_repository,
							&rm->old_oid, 1);
		अगर (commit)
			oidset_जोड़ो(tips, &commit->object.oid);
	}
}

अटल पूर्णांक do_fetch(काष्ठा transport *transport,
		    काष्ठा refspec *rs,
		    स्थिर काष्ठा fetch_config *config,
		    काष्ठा list_objects_filter_options *filter_options,
		    काष्ठा oidset *updated_tips)
{
	काष्ठा ref_transaction *transaction = शून्य;
	काष्ठा ref *ref_map = शून्य;
	काष्ठा display_state display_state = { 0 };
	पूर्णांक autotags = (transport->remote->fetch_tags == 1);
	पूर्णांक retcode = 0;
	स्थिर काष्ठा ref *remote_refs;
	काष्ठा transport_ls_refs_options transport_ls_refs_options =
		TRANSPORT_LS_REFS_OPTIONS_INIT;
	काष्ठा fetch_शीर्ष fetch_शीर्ष = { 0 };
	काष्ठा strbuf err = STRBUF_INIT;
	काष्ठा ref_update_display_info_array display_array = { 0 };
	काष्ठा strmap rejected_refs = STRMAP_INIT;
	पूर्णांक summary_width = 0;
	पूर्णांक follow_remote_शीर्ष = FOLLOW_REMOTE_NEVER;

	अगर (tags == TAGS_DEFAULT) {
		अगर (transport->remote->fetch_tags == 2)
			tags = TAGS_SET;
		अगर (transport->remote->fetch_tags == -1)
			tags = TAGS_UNSET;
	}

	/* if not appending, truncate FETCH_HEAD */
	अगर (!append && write_fetch_शीर्ष) {
		retcode = truncate_fetch_शीर्ष();
		अगर (retcode)
			जाओ cleanup;
	}

	अगर (rs->nr) {
		refspec_ref_prefixes(rs, &transport_ls_refs_options.ref_prefixes);
	} अन्यथा {
		काष्ठा branch *branch = branch_get(शून्य);

		अगर (transport->remote->fetch.nr) {
			refspec_ref_prefixes(&transport->remote->fetch,
					     &transport_ls_refs_options.ref_prefixes);
			/*
			 * See remote.c's handling of remote.<name>.followRemoteHEAD
			 * for the analogous, still-unresolved case.
			 */
			अगर (transport->remote->follow_remote_शीर्ष)
				follow_remote_शीर्ष = transport->remote->follow_remote_शीर्ष;
			अन्यथा अगर (config->follow_remote_शीर्ष_seen)
				follow_remote_शीर्ष = get_follow_remote_शीर्ष(config->follow_remote_शीर्ष_raw);
			अन्यथा
				follow_remote_शीर्ष = BUILTIN_FOLLOW_REMOTE_शीर्ष_DFLT;
		}
		अगर (branch && branch_has_merge_config(branch) &&
		    !म_भेद(branch->remote_name, transport->remote->name)) {
			पूर्णांक i;
			क्रम (i = 0; i < branch->merge_nr; i++) {
				strvec_धकेलो(&transport_ls_refs_options.ref_prefixes,
					    branch->merge[i]->src);
			}
		}

		/*
		 * If there are no refs specified to fetch, then we just
		 * fetch HEAD; mention that to narrow the advertisement.
		 */
		अगर (!transport_ls_refs_options.ref_prefixes.nr)
			strvec_धकेलो(&transport_ls_refs_options.ref_prefixes,
				    "HEAD");
	}

	अगर (tags == TAGS_SET || tags == TAGS_DEFAULT)
		strvec_धकेलो(&transport_ls_refs_options.ref_prefixes,
			    "refs/tags/");

	अगर (follow_remote_शीर्ष != FOLLOW_REMOTE_NEVER)
		strvec_धकेलो(&transport_ls_refs_options.ref_prefixes,
			    "HEAD");

	/*
	 * Only initiate ref listing if we have at least one ref we want to
	 * know about.
	 */
	अगर (transport_ls_refs_options.ref_prefixes.nr) {
		trace2_region_enter("fetch", "remote_refs", the_repository);
		remote_refs = transport_get_remote_refs(transport,
							&transport_ls_refs_options);
		trace2_region_leave("fetch", "remote_refs", the_repository);
	} अन्यथा
		remote_refs = शून्य;

	transport_ls_refs_options_release(&transport_ls_refs_options);

	ref_map = get_ref_map(transport->remote, remote_refs, rs,
			      tags, &autotags);
	अगर (!update_शीर्ष_ok)
		check_not_current_branch(ref_map);

	retcode = open_fetch_शीर्ष(&fetch_शीर्ष);
	अगर (retcode)
		जाओ cleanup;

	display_state_init(&display_state, ref_map, transport->url,
			   config->display_format);

	अगर (atomic_fetch) {
		transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
							  0, &err);
		अगर (!transaction) {
			retcode = -1;
			जाओ cleanup;
		}
	}

	अगर (tags == TAGS_DEFAULT && autotags)
		transport_set_option(transport, TRANS_OPT_FOLLOWTAGS, "1");
	अगर (prune) {
		/*
		 * We only prune based on refspecs specified
		 * explicitly (via command line or configuration); we
		 * don't care whether --tags was specified.
		 */
		अगर (rs->nr) {
			retcode = prune_refs(&display_state, rs, transaction, ref_map);
		} अन्यथा {
			retcode = prune_refs(&display_state, &transport->remote->fetch,
					     transaction, ref_map);
		}
		अगर (retcode != 0)
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
	अगर (!transaction) {
		transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
							  REF_TRANSACTION_ALLOW_FAILURE, &err);
		अगर (!transaction) {
			retcode = -1;
			जाओ cleanup;
		}
	}

	अगर (fetch_and_consume_refs(&display_state, transport, transaction, ref_map,
				   &fetch_शीर्ष, config, &display_array)) {
		retcode = 1;
		जाओ cleanup;
	}

	/*
	 * If neither --no-tags nor --tags was specified, do automated tag
	 * following.
	 */
	अगर (tags == TAGS_DEFAULT && autotags) {
		काष्ठा ref *tags_ref_map = शून्य, **पुच्छ = &tags_ref_map;

		find_non_local_tags(remote_refs, transaction, &tags_ref_map, &पुच्छ);
		अगर (tags_ref_map) {
			/*
			 * If backfilling of tags fails then we want to tell
			 * the user so, but we have to continue regardless to
			 * populate upstream information of the references we
			 * have already fetched above. The exception though is
			 * when `--atomic` is passed: in that case we'll abort
			 * the transaction and don't commit anything.
			 */
			अगर (backfill_tags(&display_state, transport, transaction, tags_ref_map,
					  &fetch_शीर्ष, config, &display_array, filter_options))
				retcode = 1;
		}

		free_refs(tags_ref_map);
	}

	अगर (retcode)
		जाओ cleanup;

	अगर (verbosity >= 0)
		summary_width = transport_summary_width(ref_map);

	retcode = commit_ref_transaction(&transaction, atomic_fetch,
					 transport->remote->name,
					 &rejected_refs, &err);
	/*
	 * With '--atomic', bail out if the transaction fails. Without '--atomic',
	 * continue to fetch head and perform other post-fetch operations.
	 */
	अगर (retcode && atomic_fetch)
		जाओ cleanup;

	commit_fetch_शीर्ष(&fetch_शीर्ष);

	collect_updated_tips(updated_tips, ref_map);

	अगर (set_upstream) {
		काष्ठा branch *branch = branch_get("HEAD");
		काष्ठा ref *rm;
		काष्ठा ref *source_ref = शून्य;

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
		क्रम (rm = ref_map; rm; rm = rm->अग्र) {
			अगर (!rm->peer_ref) {
				अगर (source_ref) {
					warning(_("multiple branches detected, incompatible with --set-upstream"));
					जाओ cleanup;
				} अन्यथा {
					source_ref = rm;
				}
			}
		}
		अगर (source_ref) {
			अगर (!branch) {
				स्थिर अक्षर *shortname = source_ref->name;
				skip_prefix(shortname, "refs/heads/", &shortname);

				warning(_("could not set upstream of HEAD to '%s' from '%s' when "
					  "it does not point to any branch."),
					shortname, transport->remote->name);
				जाओ cleanup;
			}

			अगर (!म_भेद(source_ref->name, "HEAD") ||
			    starts_with(source_ref->name, "refs/heads/"))
				install_branch_config(0,
						      branch->name,
						      transport->remote->name,
						      source_ref->name);
			अन्यथा अगर (starts_with(source_ref->name, "refs/remotes/"))
				warning(_("not setting upstream for a remote remote-tracking branch"));
			अन्यथा अगर (starts_with(source_ref->name, "refs/tags/"))
				warning(_("not setting upstream for a remote tag"));
			अन्यथा
				warning(_("unknown branch type"));
		} अन्यथा {
			warning(_("no source branch found;\n"
				  "you need to specify exactly one branch with the --set-upstream option"));
		}
	}
	अगर (follow_remote_शीर्ष != FOLLOW_REMOTE_NEVER) {
		/*
		 * Way too many cases where this can go wrong so let's just
		 * ignore errors and fail silently for now.
		 */
		set_शीर्ष(remote_refs, transport->remote, follow_remote_शीर्ष);
	}

cleanup:
	/*
	 * When using batched updates, we want to commit the non-rejected
	 * updates and also handle the rejections.
	 */
	अगर (retcode && !atomic_fetch && transaction)
		commit_ref_transaction(&transaction, false,
				       transport->remote->name,
				       &rejected_refs, &err);

	क्रम (माप_प्रकार i = 0; i < display_array.nr; i++) {
		काष्ठा ref_update_display_info *info = &display_array.info[i];

		अगर (!info->failed && strmap_contains(&rejected_refs, info->ref))
			ref_update_display_info_set_failed(info);
		ref_update_display_info_display(info, &display_state, summary_width);
		ref_update_display_info_free(info);
	}

	अगर (retcode) {
		अगर (err.len) {
			error("%s", err.buf);
			strbuf_reset(&err);
		}
		अगर (transaction && ref_transaction_abort(transaction, &err) &&
		    err.len)
			error("%s", err.buf);
		transaction = शून्य;
	}

	अगर (transaction)
		ref_transaction_free(transaction);

	मुक्त(display_array.info);
	strmap_clear(&rejected_refs, 0);
	display_state_release(&display_state);
	close_fetch_शीर्ष(&fetch_शीर्ष);
	strbuf_release(&err);
	free_refs(ref_map);
	वापस retcode;
}

अटल पूर्णांक get_one_remote_for_fetch(काष्ठा remote *remote, व्योम *priv)
{
	काष्ठा string_list *list = priv;
	अगर (!remote->skip_default_update)
		string_list_append(list, remote->name);
	वापस 0;
}

अटल व्योम add_options_to_argv(काष्ठा strvec *argv,
				स्थिर काष्ठा fetch_config *config)
{
	अगर (dry_run)
		strvec_धकेलो(argv, "--dry-run");
	अगर (prune != -1)
		strvec_धकेलो(argv, prune ? "--prune" : "--no-prune");
	अगर (prune_tags != -1)
		strvec_धकेलो(argv, prune_tags ? "--prune-tags" : "--no-prune-tags");
	अगर (update_शीर्ष_ok)
		strvec_धकेलो(argv, "--update-head-ok");
	अगर (force)
		strvec_धकेलो(argv, "--force");
	अगर (keep)
		strvec_धकेलो(argv, "--keep");
	अगर (config->recurse_submodules == RECURSE_SUBMODULES_ON)
		strvec_धकेलो(argv, "--recurse-submodules");
	अन्यथा अगर (config->recurse_submodules == RECURSE_SUBMODULES_OFF)
		strvec_धकेलो(argv, "--no-recurse-submodules");
	अन्यथा अगर (config->recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND)
		strvec_धकेलो(argv, "--recurse-submodules=on-demand");
	अगर (tags == TAGS_SET)
		strvec_धकेलो(argv, "--tags");
	अन्यथा अगर (tags == TAGS_UNSET)
		strvec_धकेलो(argv, "--no-tags");
	अगर (verbosity >= 2)
		strvec_धकेलो(argv, "-v");
	अगर (verbosity >= 1)
		strvec_धकेलो(argv, "-v");
	अन्यथा अगर (verbosity < 0)
		strvec_धकेलो(argv, "-q");
	अगर (family == TRANSPORT_FAMILY_IPV4)
		strvec_धकेलो(argv, "--ipv4");
	अन्यथा अगर (family == TRANSPORT_FAMILY_IPV6)
		strvec_धकेलो(argv, "--ipv6");
	अगर (!write_fetch_शीर्ष)
		strvec_धकेलो(argv, "--no-write-fetch-head");
	अगर (config->display_format == DISPLAY_FORMAT_PORCELAIN)
		strvec_pushf(argv, "--porcelain");
}

/* Fetch multiple remotes in parallel */

काष्ठा parallel_fetch_state {
	स्थिर अक्षर **argv;
	काष्ठा string_list *remotes;
	पूर्णांक अग्र, परिणाम;
	स्थिर काष्ठा fetch_config *config;
};

अटल पूर्णांक fetch_अग्र_remote(काष्ठा child_process *cp,
			     काष्ठा strbuf *out UNUSED,
			     व्योम *cb, व्योम **task_cb)
{
	काष्ठा parallel_fetch_state *state = cb;
	अक्षर *remote;

	अगर (state->अग्र < 0 || state->अग्र >= state->remotes->nr)
		वापस 0;

	remote = state->remotes->वस्तुएँ[state->अग्र++].string;
	*task_cb = remote;

	strvec_pushv(&cp->args, state->argv);
	strvec_धकेलो(&cp->args, remote);
	cp->git_cmd = 1;

	अगर (verbosity >= 0 && state->config->display_format != DISPLAY_FORMAT_PORCELAIN)
		म_लिखो(_("Fetching %s\n"), remote);

	वापस 1;
}

अटल पूर्णांक fetch_failed_to_start(काष्ठा strbuf *out UNUSED,
				 व्योम *cb, व्योम *task_cb)
{
	काष्ठा parallel_fetch_state *state = cb;
	स्थिर अक्षर *remote = task_cb;

	state->परिणाम = error(_("could not fetch %s"), remote);

	वापस 0;
}

अटल पूर्णांक fetch_finished(पूर्णांक परिणाम, काष्ठा strbuf *out,
			  व्योम *cb, व्योम *task_cb)
{
	काष्ठा parallel_fetch_state *state = cb;
	स्थिर अक्षर *remote = task_cb;

	अगर (परिणाम) {
		strbuf_addf(out, _("could not fetch '%s' (exit code: %d)\n"),
			    remote, परिणाम);
		state->परिणाम = -1;
	}

	वापस 0;
}

अटल पूर्णांक fetch_multiple(काष्ठा string_list *list, पूर्णांक max_children,
			  स्थिर काष्ठा fetch_config *config)
{
	पूर्णांक i, परिणाम = 0;
	काष्ठा strvec argv = STRVEC_INIT;

	अगर (!append && write_fetch_शीर्ष) {
		पूर्णांक errcode = truncate_fetch_शीर्ष();
		अगर (errcode)
			वापस errcode;
	}

	/*
	 * Cancel out the fetch.bundleURI config when running subprocesses,
	 * to avoid fetching from the same bundle list multiple times.
	 */
	strvec_pushl(&argv, "-c", "fetch.bundleURI=",
		     "fetch", "--append", "--no-auto-gc",
		     "--no-write-commit-graph", शून्य);
	क्रम (i = 0; i < server_options.nr; i++)
		strvec_pushf(&argv, "--server-option=%s", server_options.वस्तुएँ[i].string);
	add_options_to_argv(&argv, config);

	अगर (max_children != 1 && list->nr != 1) {
		काष्ठा parallel_fetch_state state = { argv.v, list, 0, 0, config };
		स्थिर काष्ठा run_process_parallel_opts opts = {
			.tr2_category = "fetch",
			.tr2_label = "parallel/fetch",

			.processes = max_children,

			.get_अग्र_task = &fetch_अग्र_remote,
			.start_failure = &fetch_failed_to_start,
			.task_finished = &fetch_finished,
			.तथ्य = &state,
		};

		strvec_धकेलो(&argv, "--end-of-options");

		run_processes_parallel(&opts);
		परिणाम = state.परिणाम;
	} अन्यथा
		क्रम (i = 0; i < list->nr; i++) {
			स्थिर अक्षर *name = list->वस्तुएँ[i].string;
			काष्ठा child_process cmd = CHILD_PROCESS_INIT;

			strvec_pushv(&cmd.args, argv.v);
			strvec_धकेलो(&cmd.args, name);
			अगर (verbosity >= 0 && config->display_format != DISPLAY_FORMAT_PORCELAIN)
				म_लिखो(_("Fetching %s\n"), name);
			cmd.git_cmd = 1;
			अगर (run_command(&cmd)) {
				error(_("could not fetch %s"), name);
				परिणाम = 1;
			}
		}

	strvec_clear(&argv);
	वापस !!परिणाम;
}

/*
 * Fetching from the promisor remote should use the given filter-spec
 * or inherit the default filter-spec from the config.
 */
अटल अंतरभूत व्योम fetch_one_setup_partial(काष्ठा remote *remote,
					   काष्ठा list_objects_filter_options *filter_options)
{
	/*
	 * Explicit --no-filter argument overrides everything, regardless
	 * of any prior partial clones and fetches.
	 */
	अगर (filter_options->no_filter)
		वापस;

	/*
	 * If no prior partial clone/fetch and the current fetch DID NOT
	 * request a partial-fetch, do a normal fetch.
	 */
	अगर (!repo_has_promisor_remote(the_repository) && !filter_options->choice)
		वापस;

	/*
	 * If this is a partial-fetch request, we enable partial on
	 * this repo if not already enabled and remember the given
	 * filter-spec as the default for subsequent fetches to this
	 * remote if there is currently no default filter-spec.
	 */
	अगर (filter_options->choice) {
		partial_clone_register(remote->name, filter_options);
		वापस;
	}

	/*
	 * Do a partial-fetch from the promisor remote using either the
	 * explicitly given filter-spec or inherit the filter-spec from
	 * the config.
	 */
	अगर (!filter_options->choice)
		partial_clone_get_default_filter_spec(filter_options, remote->name);
	वापस;
}

अटल पूर्णांक fetch_one(काष्ठा remote *remote, पूर्णांक argc, स्थिर अक्षर **argv,
		     पूर्णांक prune_tags_ok, पूर्णांक use_stdin_refspecs,
		     स्थिर काष्ठा fetch_config *config,
		     काष्ठा list_objects_filter_options *filter_options,
		     काष्ठा oidset *updated_tips)
{
	काष्ठा refspec rs = REFSPEC_INIT_FETCH(the_hash_algo);
	पूर्णांक i;
	पूर्णांक exit_code;
	पूर्णांक maybe_prune_tags;
	पूर्णांक remote_via_config = remote_is_configured(remote, 0);

	अगर (!remote)
		die(_("no remote repository specified; please specify either a URL or a\n"
		      "remote name from which new revisions should be fetched"));

	gtransport = prepare_transport(remote, 1, filter_options);

	अगर (prune < 0) {
		/* no command line request */
		अगर (0 <= remote->prune)
			prune = remote->prune;
		अन्यथा अगर (0 <= config->prune)
			prune = config->prune;
		अन्यथा
			prune = PRUNE_BY_DEFAULT;
	}

	अगर (prune_tags < 0) {
		/* no command line request */
		अगर (0 <= remote->prune_tags)
			prune_tags = remote->prune_tags;
		अन्यथा अगर (0 <= config->prune_tags)
			prune_tags = config->prune_tags;
		अन्यथा
			prune_tags = PRUNE_TAGS_BY_DEFAULT;
	}

	maybe_prune_tags = prune_tags_ok && prune_tags;
	अगर (maybe_prune_tags && remote_via_config)
		refspec_append(&remote->fetch, TAG_REFSPEC);

	अगर (maybe_prune_tags && (argc || !remote_via_config))
		refspec_append(&rs, TAG_REFSPEC);

	क्रम (i = 0; i < argc; i++) {
		अगर (!म_भेद(argv[i], "tag")) {
			i++;
			अगर (i >= argc)
				die(_("you need to specify a tag name"));

			refspec_appendf(&rs, "refs/tags/%s:refs/tags/%s",
					argv[i], argv[i]);
		} अन्यथा {
			refspec_append(&rs, argv[i]);
		}
	}

	अगर (use_stdin_refspecs) {
		काष्ठा strbuf पंक्ति = STRBUF_INIT;
		जबतक (strbuf_getline_lf(&पंक्ति, मानक_निवेश) != खातापूर्ण)
			refspec_append(&rs, पंक्ति.buf);
		strbuf_release(&पंक्ति);
	}

	अगर (server_options.nr)
		gtransport->server_options = &server_options;

	sigchain_धकेलो_common(unlock_pack_on_signal);
	निकास_पर(unlock_pack_atexit);
	sigchain_धकेलो(SIGPIPE, संक_छोड़ो);
	exit_code = do_fetch(gtransport, &rs, config, filter_options,
			     updated_tips);
	sigchain_निकालो(SIGPIPE);
	refspec_clear(&rs);
	transport_disconnect(gtransport);
	gtransport = शून्य;
	वापस exit_code;
}

पूर्णांक cmd_fetch(पूर्णांक argc,
	      स्थिर अक्षर **argv,
	      स्थिर अक्षर *prefix,
	      काष्ठा repository *repo UNUSED)
{
	काष्ठा fetch_config config = {
		.display_format = DISPLAY_FORMAT_FULL,
		.follow_remote_शीर्ष_raw = शून्य,
		.follow_remote_शीर्ष_seen = 0,
		.prune = -1,
		.prune_tags = -1,
		.show_forced_updates = 1,
		.recurse_submodules = RECURSE_SUBMODULES_DEFAULT,
		.parallel = 1,
		.submodule_fetch_jobs = -1,
	};
	स्थिर अक्षर *submodule_prefix = "";
	स्थिर अक्षर *bundle_uri;
	काष्ठा string_list list = STRING_LIST_INIT_DUP;
	काष्ठा list_objects_filter_options filter_options = LIST_OBJECTS_FILTER_INIT;
	काष्ठा remote *remote = शून्य;
	पूर्णांक all = -1, multiple = 0;
	पूर्णांक परिणाम = 0;
	पूर्णांक prune_tags_ok = 1;
	पूर्णांक enable_auto_gc = 1;
	पूर्णांक unshallow = 0;
	पूर्णांक max_jobs = -1;
	पूर्णांक recurse_submodules_cli = RECURSE_SUBMODULES_DEFAULT;
	पूर्णांक recurse_submodules_default = RECURSE_SUBMODULES_ON_DEMAND;
	पूर्णांक fetch_write_commit_graph = -1;
	पूर्णांक stdin_refspecs = 0;
	पूर्णांक negotiate_only = 0;
	पूर्णांक porcelain = 0;
	पूर्णांक i;
	क्रमागत {
		GRAPH_WRITE_REACHABLE,
		GRAPH_WRITE_TIPS,
		GRAPH_WRITE_SKIP,
	} graph_write_mode = GRAPH_WRITE_REACHABLE;
	काष्ठा oidset updated_tips = OIDSET_INIT;

	काष्ठा option builtin_fetch_options[] = {
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
		OPT_SET_INT('n', शून्य, &tags,
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
		OPT_BOOL(0, "write-fetch-head", &write_fetch_शीर्ष,
			 N_("write fetched references to the FETCH_HEAD file")),
		OPT_BOOL('k', "keep", &keep, N_("keep downloaded pack")),
		OPT_BOOL('u', "update-head-ok", &update_शीर्ष_ok,
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
			.मूल्य = &submodule_prefix,
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
	क्रम (i = 1; i < argc; i++) {
		/* This handles non-URLs gracefully */
		अक्षर *anon = transport_anonymize_url(argv[i]);

		strbuf_addf(&default_rla, " %s", anon);
		मुक्त(anon);
	}

	repo_config(the_repository, git_fetch_config, &config);
	अगर (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_full_सूचकांक = 0;
	}

	argc = parse_options(argc, argv, prefix,
			     builtin_fetch_options, builtin_fetch_usage, 0);

	अगर (recurse_submodules_cli != RECURSE_SUBMODULES_DEFAULT)
		config.recurse_submodules = recurse_submodules_cli;

	अगर (negotiate_only) {
		चयन (recurse_submodules_cli) {
		हाल RECURSE_SUBMODULES_OFF:
		हाल RECURSE_SUBMODULES_DEFAULT:
			/*
			 * --negotiate-only should never recurse into
			 * submodules. Skip it by setting recurse_submodules to
			 * RECURSE_SUBMODULES_OFF.
			 */
			config.recurse_submodules = RECURSE_SUBMODULES_OFF;
			अवरोध;

		शेष:
			die(_("options '%s' and '%s' cannot be used together"),
			    "--negotiate-only", "--recurse-submodules");
		}
	}

	अगर (config.recurse_submodules != RECURSE_SUBMODULES_OFF) {
		पूर्णांक *sfjc = config.submodule_fetch_jobs == -1
			    ? &config.submodule_fetch_jobs : शून्य;
		पूर्णांक *rs = config.recurse_submodules == RECURSE_SUBMODULES_DEFAULT
			  ? &config.recurse_submodules : शून्य;

		fetch_config_from_gitmodules(the_repository, sfjc, rs);
	}


	अगर (porcelain) {
		चयन (recurse_submodules_cli) {
		हाल RECURSE_SUBMODULES_OFF:
		हाल RECURSE_SUBMODULES_DEFAULT:
			/*
			 * Reference updates in submodules would be ambiguous
			 * in porcelain mode, so we reject this combination.
			 */
			config.recurse_submodules = RECURSE_SUBMODULES_OFF;
			अवरोध;

		शेष:
			die(_("options '%s' and '%s' cannot be used together"),
			    "--porcelain", "--recurse-submodules");
		}

		config.display_format = DISPLAY_FORMAT_PORCELAIN;
	}

	अगर (deepen_relative) {
		अगर (deepen_relative < 0)
			die(_("negative depth in --deepen is not supported"));
		अगर (depth)
			die(_("options '%s' and '%s' cannot be used together"), "--deepen", "--depth");
		depth = xstrfmt("%d", deepen_relative);
	}
	अगर (unshallow) {
		अगर (depth)
			die(_("options '%s' and '%s' cannot be used together"), "--depth", "--unshallow");
		अन्यथा अगर (!is_repository_shallow(the_repository))
			die(_("--unshallow on a complete repository does not make sense"));
		अन्यथा
			depth = xstrfmt("%d", INFINITE_DEPTH);
	}

	/* no need to be strict, transport_set_option() will validate it again */
	अगर (depth && म_से_प(depth) < 1)
		die(_("depth %s is not a positive number"), depth);
	अगर (depth || deepen_since || deepen_not.nr)
		deepen = 1;

	/* FETCH_HEAD never gets updated in --dry-run mode */
	अगर (dry_run)
		write_fetch_शीर्ष = 0;

	अगर (!max_jobs)
		max_jobs = online_cpus();

	अगर (!repo_config_get_string_tmp(the_repository, "fetch.bundleuri", &bundle_uri) &&
	    fetch_bundle_uri(the_repository, bundle_uri, शून्य))
		warning(_("failed to fetch bundles from '%s'"), bundle_uri);

	अगर (all < 0) {
		/*
		 * no --[no-]all given;
		 * only use config option if no remote was explicitly specified
		 */
		all = (!argc) ? config.all : 0;
	}

	अगर (all) {
		अगर (argc == 1)
			die(_("fetch --all does not take a repository argument"));
		अन्यथा अगर (argc > 1)
			die(_("fetch --all does not make sense with refspecs"));

		(व्योम) for_each_remote(get_one_remote_for_fetch, &list);

		/* do not do fetch_multiple() of one */
		अगर (list.nr == 1)
			remote = remote_get(list.वस्तुएँ[0].string);
	} अन्यथा अगर (argc == 0) {
		/* No arguments -- use default remote */
		remote = remote_get(शून्य);
	} अन्यथा अगर (multiple) {
		/* All arguments are assumed to be remotes or groups */
		क्रम (i = 0; i < argc; i++)
			अगर (!add_remote_or_group(argv[i], &list))
				die(_("no such remote or remote group: %s"),
				    argv[i]);
	} अन्यथा {
		/* Single remote or group */
		(व्योम) add_remote_or_group(argv[0], &list);
		अगर (list.nr > 1) {
			/* More than one remote */
			अगर (argc > 1)
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

	अगर (negotiate_only) {
		काष्ठा oidset acked_commits = OIDSET_INIT;
		काष्ठा oidset_iter iter;
		स्थिर काष्ठा object_id *oid;

		trace2_region_enter("fetch", "negotiate-only", the_repository);
		अगर (!remote)
			die(_("must supply remote when using --negotiate-only"));
		gtransport = prepare_transport(remote, 1, &filter_options);

		अगर (!gtransport->smart_options) {
			warning(_("protocol does not support --negotiate-only, exiting"));
			परिणाम = 1;
			trace2_region_leave("fetch", "negotiate-only", the_repository);
			जाओ cleanup;
		}
		अगर (!gtransport->smart_options->negotiation_restrict_tips)
			die(_("%s needs one or more %s"), "--negotiate-only",
			    "--negotiation-restrict=*");

		gtransport->smart_options->acked_commits = &acked_commits;

		अगर (server_options.nr)
			gtransport->server_options = &server_options;
		परिणाम = transport_fetch_refs(gtransport, शून्य);
		gtransport->smart_options->acked_commits = शून्य;

		oidset_iter_init(&acked_commits, &iter);
		जबतक ((oid = oidset_iter_अग्र(&iter)))
			म_लिखो("%s\n", oid_to_hex(oid));
		oidset_clear(&acked_commits);
		trace2_region_leave("fetch", "negotiate-only", the_repository);
	} अन्यथा अगर (remote) {
		अगर (filter_options.choice || repo_has_promisor_remote(the_repository)) {
			trace2_region_enter("fetch", "setup-partial", the_repository);
			fetch_one_setup_partial(remote, &filter_options);
			trace2_region_leave("fetch", "setup-partial", the_repository);
		}
		trace2_region_enter("fetch", "fetch-one", the_repository);
		परिणाम = fetch_one(remote, argc, argv, prune_tags_ok, stdin_refspecs,
				   &config, &filter_options, &updated_tips);
		अगर (prepare_commit_graph(the_repository)) {
			अगर (oidset_आकार(&updated_tips))
				graph_write_mode = GRAPH_WRITE_TIPS;
			अन्यथा
				graph_write_mode = GRAPH_WRITE_SKIP;
		}
		trace2_region_leave("fetch", "fetch-one", the_repository);
	} अन्यथा {
		पूर्णांक max_children = max_jobs;

		अगर (filter_options.choice)
			die(_("--filter can only be used with the remote "
			      "configured in extensions.partialclone"));

		अगर (atomic_fetch)
			die(_("--atomic can only be used when fetching "
			      "from one remote"));

		अगर (stdin_refspecs)
			die(_("--stdin can only be used when fetching "
			      "from one remote"));

		अगर (max_children < 0)
			max_children = config.parallel;

		/* TODO should this also die if we have a previous partial-clone? */
		trace2_region_enter("fetch", "fetch-multiple", the_repository);
		परिणाम = fetch_multiple(&list, max_children, &config);
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
	अगर (!परिणाम && remote && (config.recurse_submodules != RECURSE_SUBMODULES_OFF)) {
		काष्ठा strvec options = STRVEC_INIT;
		पूर्णांक max_children = max_jobs;

		अगर (max_children < 0)
			max_children = config.submodule_fetch_jobs;
		अगर (max_children < 0)
			max_children = config.parallel;

		add_options_to_argv(&options, &config);
		trace2_region_enter_printf("fetch", "recurse-submodule", the_repository, "%s", submodule_prefix);
		परिणाम = fetch_submodules(the_repository,
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
	अगर (negotiate_only)
		जाओ cleanup;

	prepare_repo_settings(the_repository);
	अगर (fetch_write_commit_graph > 0 ||
	    (fetch_write_commit_graph < 0 &&
	     the_repository->settings.fetch_write_commit_graph)) {
		पूर्णांक commit_graph_flags = COMMIT_GRAPH_WRITE_SPLIT;

		अगर (progress)
			commit_graph_flags |= COMMIT_GRAPH_WRITE_PROGRESS;

		अगर (graph_write_mode != GRAPH_WRITE_SKIP) {
			trace2_region_enter("fetch", "write-commit-graph",
					    the_repository);
			अगर (graph_write_mode == GRAPH_WRITE_TIPS)
				write_commit_graph(
					the_repository,
					the_repository->objects->source->path,
					शून्य, &updated_tips,
					commit_graph_flags, शून्य);
			अन्यथा
				write_commit_graph_reachable(
					the_repository,
					the_repository->objects->source->path,
					commit_graph_flags, शून्य);
			trace2_region_leave("fetch", "write-commit-graph",
					    the_repository);
		}
	}

	अगर (enable_auto_gc) {
		अगर (refetch) {
			/*
			 * Hint auto-maintenance strongly to encourage repacking,
			 * but respect config settings disabling it.
			 */
			पूर्णांक opt_val;
			अगर (repo_config_get_int(the_repository, "gc.autopacklimit", &opt_val))
				opt_val = -1;
			अगर (opt_val != 0)
				git_config_धकेलो_parameter("gc.autoPackLimit=1");

			अगर (repo_config_get_int(the_repository, "maintenance.incremental-repack.auto", &opt_val))
				opt_val = -1;
			अगर (opt_val != 0)
				git_config_धकेलो_parameter("maintenance.incremental-repack.auto=-1");
		}
		run_auto_maintenance(the_repository, verbosity < 0);
	}

 cleanup:
	oidset_clear(&updated_tips);
	string_list_clear(&list, 0);
	list_objects_filter_release(&filter_options);
	मुक्त(config.follow_remote_शीर्ष_raw);
	वापस परिणाम;
}
