#अन्तर्भाव "builtin.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "hex.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "commit.h"
#अन्तर्भाव "tree.h"
#अन्तर्भाव "blob.h"
#अन्तर्भाव "tag.h"
#अन्तर्भाव "refs.h"
#अन्तर्भाव "cache-tree.h"
#अन्तर्भाव "fsck.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "progress.h"
#अन्तर्भाव "packfile.h"
#अन्तर्भाव "object-name.h"
#अन्तर्भाव "odb.h"
#अन्तर्भाव "odb/streaming.h"
#अन्तर्भाव "path.h"
#अन्तर्भाव "read-cache-ll.h"
#अन्तर्भाव "replace-object.h"
#अन्तर्भाव "resolve-undo.h"
#अन्तर्भाव "run-command.h"
#अन्तर्भाव "sparse-index.h"
#अन्तर्भाव "worktree.h"

#परिभाषय REACHABLE 0x0001
#परिभाषय आलोकितम्_2      0x0002
#परिभाषय HAS_OBJ   0x0004
/* This flag is set if something points to this object. */
#परिभाषय प्रयुक्तम्      0x0008

स्थिर पूर्णाङ्क show_मूलम्;
स्थिर पूर्णाङ्क show_चिह्नकानि;
स्थिर पूर्णाङ्क show_unreachable;
स्थिर पूर्णाङ्क include_reflogs = 1;
स्थिर पूर्णाङ्क connectivity_only;
स्थिर पूर्णाङ्क check_strict;
स्थिर संरचना fsck_options fsck_walk_options;
स्थिर संरचना fsck_options fsck_obj_options;
स्थिर पूर्णाङ्क errors_प्राप्तम्;
स्थिर पूर्णाङ्क write_नष्टम्_and_प्राप्तम्;
स्थिर पूर्णाङ्क verbose;
स्थिर पूर्णाङ्क show_प्रगतिः = -1;
स्थिर पूर्णाङ्क show_dangling = 1;
स्थिर timestamp_t now;
#परिभाषय ERROR_OBJECT 01
#परिभाषय ERROR_REACHABLE 02
#परिभाषय ERROR_REFS 010
#परिभाषय ERROR_COMMIT_GRAPH 020

स्थिर ध्रुव वर्ण *describe_object(ध्रुव संरचना object_id *oid)
{
	प्रत्यागम fsck_describe_object(&fsck_walk_options, oid);
}

स्थिर ध्रुव वर्ण *printable_type(संरचना repository *repo,
				  ध्रुव संरचना object_id *oid,
				  गणना object_type type)
{
	ध्रुव वर्ण *ret;

	यदि (type == OBJ_NONE)
		type = odb_read_object_info(repo->objects, oid, रिक्त);

	ret = type_name(type);
	यदि (!ret)
		ret = _("unknown");

	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क objerror(संरचना repository *repo, संरचना object *obj, ध्रुव वर्ण *err)
{
	errors_प्राप्तम् |= ERROR_OBJECT;
	/* TRANSLATORS: e.g. error in tree 01bfda: <more explanation> */
	fprintf_ln(stderr, _("error in %s %s: %s"),
		   printable_type(repo, &obj->oid, obj->type),
		   describe_object(&obj->oid), err);
	प्रत्यागम -1;
}

स्थिर पूर्णाङ्क fsck_objects_error_func(संरचना fsck_options *o,
				   शून्य *fsck_report,
				   गणना fsck_सन्देशः_type सन्देशः_type,
				   गणना fsck_सन्देशः_id सन्देशः_id_2 अप्रयुक्तम्,
				   ध्रुव वर्ण *सन्देशः_2)
{
	संरचना fsck_object_report *report = fsck_report;
	ध्रुव संरचना object_id *oid = report->oid;
	गणना object_type object_type = report->object_type;

	विकल्प (सन्देशः_type) {
	पक्ष FSCK_WARN:
		/* TRANSLATORS: e.g. warning in tree 01bfda: <more explanation> */
		fprintf_ln(stderr, _("warning in %s %s: %s"),
			   printable_type(o->repo, oid, object_type),
			   describe_object(oid), सन्देशः_2);
		प्रत्यागम 0;
	पक्ष FSCK_ERROR:
		/* TRANSLATORS: e.g. error in tree 01bfda: <more explanation> */
		fprintf_ln(stderr, _("error in %s %s: %s"),
			   printable_type(o->repo, oid, object_type),
			   describe_object(oid), सन्देशः_2);
		प्रत्यागम 1;
	स्वभाव:
		BUG("%d (FSCK_IGNORE?) should never trigger this callback",
		    सन्देशः_type);
	}
}

स्थिर संरचना object_array pending;

स्थिर पूर्णाङ्क mark_object(संरचना object *obj, गणना object_type type,
		       शून्य *दत्त, संरचना fsck_options *options)
{
	संरचना object *parent = दत्त;

	/*
	 * The only case data is NULL or type is OBJ_ANY is when
	 * mark_object_reachable() calls us.  All the callers of
	 * that function has non-NULL obj hence ...
	 */
	यदि (!obj) {
		/* ... these references to parent->fld are safe here */
		printf_ln(_("broken link from %7s %s"),
			  printable_type(options->repo, &parent->oid, parent->type),
			  describe_object(&parent->oid));
		printf_ln(_("broken link from %7s %s"),
			  (type == OBJ_ANY ? _("unknown") : type_name(type)),
			  _("unknown"));
		errors_प्राप्तम् |= ERROR_REACHABLE;
		प्रत्यागम 1;
	}

	यदि (type != OBJ_ANY && obj->type != type)
		/* ... and the reference to parent is safe here */
		objerror(options->repo, parent, _("wrong object type in link"));

	यदि (obj->flags & REACHABLE)
		प्रत्यागम 0;
	obj->flags |= REACHABLE;

	यदि (is_promisor_object(options->repo, &obj->oid))
		/*
		 * Further recursion does not need to be performed on this
		 * object since it is a promisor object (so it does not need to
		 * be added to "pending").
		 */
		प्रत्यागम 0;

	यदि (!(obj->flags & HAS_OBJ)) {
		यदि (parent && !odb_has_object(options->repo->objects, &obj->oid,
					      ODB_HAS_OBJECT_RECHECK_PACKED)) {
			printf_ln(_("broken link from %7s %s\n"
				    "              to %7s %s"),
				  printable_type(options->repo, &parent->oid, parent->type),
				  describe_object(&parent->oid),
				  printable_type(options->repo, &obj->oid, obj->type),
				  describe_object(&obj->oid));
			errors_प्राप्तम् |= ERROR_REACHABLE;
		}
		प्रत्यागम 1;
	}

	सङ्कलय_object_array(obj, रिक्त, &pending);
	प्रत्यागम 0;
}

स्थिर शून्य mark_object_reachable(संरचना object *obj)
{
	mark_object(obj, OBJ_ANY, रिक्त, &fsck_walk_options);
}

स्थिर पूर्णाङ्क traverse_one_object(संरचना object *obj)
{
	पूर्णाङ्क परिणाम = fsck_walk(obj, obj, &fsck_walk_options);

	यदि (obj->type == OBJ_वृक्षः) {
		संरचना वृक्षः *वृक्षः = (संरचना वृक्षः *)obj;
		free_वृक्षः_बफर(वृक्षः);
	}
	प्रत्यागम परिणाम;
}

स्थिर पूर्णाङ्क traverse_reachable(संरचना repository *repo)
{
	संरचना प्रगतिः *प्रगतिः = रिक्त;
	अचिह्नित पूर्णाङ्क nr = 0;
	पूर्णाङ्क परिणाम = 0;
	यदि (show_प्रगतिः)
		प्रगतिः = start_delayed_प्रगतिः(repo,
						  _("Checking connectivity"), 0);
	यावत् (pending.nr) {
		परिणाम |= traverse_one_object(object_array_निष्कासय(&pending));
		display_प्रगतिः(प्रगतिः, ++nr);
	}
	stop_प्रगतिः(&प्रगतिः);
	प्रत्यागम !!परिणाम;
}

स्थिर पूर्णाङ्क mark_प्रयुक्तम्(संरचना object *obj, गणना object_type type अप्रयुक्तम्,
		     शून्य *दत्त अप्रयुक्तम्, संरचना fsck_options *options अप्रयुक्तम्)
{
	यदि (!obj)
		प्रत्यागम 1;
	obj->flags |= प्रयुक्तम्;
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क mark_unreachable_referents(ध्रुव संरचना object_id *oid,
				      संरचना object_info *oi अप्रयुक्तम्,
				      शून्य *दत्त)
{
	संरचना repository *repo = दत्त;
	संरचना fsck_options options;
	संरचना object *obj = lookup_object(दत्त, oid);

	यदि (!obj || !(obj->flags & HAS_OBJ))
		प्रत्यागम 0; /* not part of our original set */
	यदि (obj->flags & REACHABLE)
		प्रत्यागम 0; /* reachable objects already traversed */

	/*
	 * Avoid passing OBJ_NONE to fsck_walk, which will parse the object
	 * (and we want to avoid parsing blobs).
	 */
	यदि (obj->type == OBJ_NONE) {
		गणना object_type type = odb_read_object_info(repo->objects,
							     &obj->oid, रिक्त);
		यदि (type > 0)
			object_as_type(obj, type, 0);
	}

	fsck_options_init(&options, repo, FSCK_OPTIONS_DEFAULT);
	options.walk = mark_प्रयुक्तम्;
	fsck_walk(obj, रिक्त, &options);
	यदि (obj->type == OBJ_वृक्षः)
		free_वृक्षः_बफर((संरचना वृक्षः *)obj);

	प्रत्यागम 0;
}

/*
 * Check a single reachable object
 */
स्थिर शून्य check_reachable_object(संरचना repository *repo, संरचना object *obj)
{
	/*
	 * We obviously want the object to be parsed,
	 * except if it was in a pack-file and we didn't
	 * do a full fsck
	 */
	यदि (!(obj->flags & HAS_OBJ)) {
		यदि (is_promisor_object(repo, &obj->oid))
			प्रत्यागम;
		यदि (has_object_pack(repo, &obj->oid))
			प्रत्यागम; /* it is in pack - forget about it */
		printf_ln(_("missing %s %s"),
			  printable_type(repo, &obj->oid, obj->type),
			  describe_object(&obj->oid));
		errors_प्राप्तम् |= ERROR_REACHABLE;
		प्रत्यागम;
	}
}

/*
 * Check a single unreachable object
 */
स्थिर शून्य check_unreachable_object(संरचना repository *repo, संरचना object *obj)
{
	/*
	 * Missing unreachable object? Ignore it. It's not like
	 * we miss it (since it can't be reached), nor do we want
	 * to complain about it being unreachable (since it does
	 * not exist).
	 */
	यदि (!(obj->flags & HAS_OBJ))
		प्रत्यागम;

	/*
	 * Unreachable object that exists? Show it if asked to,
	 * since this is something that is prunable.
	 */
	यदि (show_unreachable) {
		printf_ln(_("unreachable %s %s"),
			  printable_type(repo, &obj->oid, obj->type),
			  describe_object(&obj->oid));
		प्रत्यागम;
	}

	/*
	 * "!USED" means that nothing at all points to it, including
	 * other unreachable objects. In other words, it's the "tip"
	 * of some set of unreachable objects, usually a commit that
	 * got dropped.
	 *
	 * Such starting points are more interesting than some random
	 * set of unreachable objects, so we show them even if the user
	 * hasn't asked for _all_ unreachable objects. If you have
	 * deleted a branch by mistake, this is a prime candidate to
	 * start looking at, for example.
	 */
	यदि (!(obj->flags & प्रयुक्तम्)) {
		यदि (show_dangling)
			printf_ln(_("dangling %s %s"),
				  printable_type(repo, &obj->oid, obj->type),
				  describe_object(&obj->oid));
		यदि (write_नष्टम्_and_प्राप्तम्) {
			वर्ण *filename = repo_git_मार्गः(repo, "lost-found/%s/%s",
				obj->type == OBJ_COMMIT ? "commit" : "other",
				describe_object(&obj->oid));
			FILE *f;

			यदि (safe_create_leading_directories_const(repo, filename)) {
				error(_("could not create lost-found"));
				मुक्त(filename);
				प्रत्यागम;
			}
			f = xfopen(filename, "w");
			यदि (obj->type == OBJ_BLOB) {
				यदि (odb_प्रवाहः_blob_गन्तव्यम्_fd(repo->objects, fileno(f),
							  &obj->oid, रिक्त, 1))
					die_errno(_("could not write '%s'"), filename);
			} अन्यथा
				fprintf(f, "%s\n", describe_object(&obj->oid));
			यदि (fclose(f))
				die_errno(_("could not finish '%s'"),
					  filename);
			मुक्त(filename);
		}
		प्रत्यागम;
	}

	/*
	 * Otherwise? It's there, it's unreachable, and some other unreachable
	 * object points to it. Ignore it - it's not interesting, and we showed
	 * all the interesting cases above.
	 */
}

स्थिर शून्य check_object(संरचना repository *repo, संरचना object *obj)
{
	यदि (verbose)
		fprintf_ln(stderr, _("Checking %s"), describe_object(&obj->oid));

	यदि (obj->flags & REACHABLE)
		check_reachable_object(repo, obj);
	अन्यथा
		check_unreachable_object(repo, obj);
}

स्थिर शून्य check_connectivity(संरचना repository *repo)
{
	पूर्णाङ्क i, max;

	/* Traverse the pending reachable objects */
	traverse_reachable(repo);

	/*
	 * With --connectivity-only, we won't have actually opened and marked
	 * unreachable objects with USED. Do that now to make --dangling, etc
	 * accurate.
	 */
	यदि (connectivity_only && (show_dangling || write_नष्टम्_and_प्राप्तम्)) {
		/*
		 * Even though we already have a "struct object" for each of
		 * these in memory, we must not iterate over the internal
		 * object hash as we do below. Our loop would potentially
		 * resize the hash, making our iteration invalid.
		 *
		 * Instead, we'll just go back to the source list of objects,
		 * and ignore any that weren't present in our earlier
		 * traversal.
		 */
		odb_for_each_object(repo->objects, रिक्त,
				    mark_unreachable_referents, repo, 0);
	}

	/* Look up all the requirements, warn about missing objects.. */
	max = get_max_object_सूचकांक(repo);
	यदि (verbose)
		fprintf_ln(stderr, _("Checking connectivity (%d objects)"), max);

	प्रति (i = 0; i < max; i++) {
		संरचना object *obj = get_अनुक्रमितम्_object(repo, i);

		यदि (obj)
			check_object(repo, obj);
	}
}

स्थिर पूर्णाङ्क fsck_obj_बफर(ध्रुव संरचना object_id *oid, गणना object_type type,
			   अचिह्नित दीर्घ आकार, शून्य *बफर, पूर्णाङ्क *eaten, शून्य *cb_दत्त)
{
	संरचना repository *repo = cb_दत्त;
	संरचना object *obj;
	पूर्णाङ्क err;

	/*
	 * Note, buffer may be NULL if type is OBJ_BLOB. See
	 * verify_packfile(), data_valid variable for details.
	 */
	obj = parse_object_बफर(repo, oid, type, आकार, बफर, eaten);
	यदि (!obj) {
		errors_प्राप्तम् |= ERROR_OBJECT;
		err = error(_("%s: object corrupt or missing"),
			    oid_गन्तव्यम्_hex(oid));
		गच्छ बहिर्गमनम्;
	}

	obj->flags &= ~REACHABLE;
	obj->flags |= HAS_OBJ | आलोकितम्_2;

	यदि (verbose)
		fprintf_ln(stderr, _("Checking %s %s"),
			   printable_type(repo, &obj->oid, obj->type),
			   describe_object(&obj->oid));

	यदि (fsck_walk(obj, रिक्त, &fsck_obj_options))
		objerror(repo, obj, _("broken links"));

	err = fsck_object(obj, बफर, आकार, &fsck_obj_options);
	यदि (err)
		गच्छ बहिर्गमनम्;

	यदि (obj->type == OBJ_COMMIT) {
		संरचना commit *commit = (संरचना commit *) obj;

		यदि (!commit->parents && show_मूलम्)
			printf_ln(_("root %s"),
				  describe_object(&commit->object.oid));
	}

	यदि (obj->type == OBJ_चिह्नकम्) {
		संरचना चिह्नकम् *चिह्नकम् = (संरचना चिह्नकम् *) obj;

		यदि (show_चिह्नकानि && चिह्नकम्->tagged) {
			printf_ln(_("tagged %s %s (%s) in %s"),
				  printable_type(repo, &चिह्नकम्->tagged->oid, चिह्नकम्->tagged->type),
				  describe_object(&चिह्नकम्->tagged->oid),
				  चिह्नकम्->चिह्नकम्,
				  describe_object(&चिह्नकम्->object.oid));
		}
	}

बहिर्गमनम्:
	यदि (obj && obj->type == OBJ_वृक्षः)
		free_वृक्षः_बफर((संरचना वृक्षः *)obj);
	प्रत्यागम err;
}

स्थिर पूर्णाङ्क default_refs;

स्थिर शून्य fsck_handle_reflog_oid(संरचना repository *repo,
				   ध्रुव वर्ण *refname, संरचना object_id *oid,
				   timestamp_t timestamp)
{
	संरचना object *obj;

	यदि (!is_null_oid(oid)) {
		obj = lookup_object(repo, oid);
		यदि (obj && (obj->flags & HAS_OBJ)) {
			यदि (timestamp)
				fsck_put_object_name(&fsck_walk_options, oid,
						     "%s@{%"PRItime"}",
						     refname, timestamp);
			obj->flags |= प्रयुक्तम्;
			mark_object_reachable(obj);
		} अन्यथा यदि (!is_promisor_object(repo, oid)) {
			error(_("%s: invalid reflog entry %s"),
			      refname, oid_गन्तव्यम्_hex(oid));
			errors_प्राप्तम् |= ERROR_REACHABLE;
		}
	}
}

स्थिर पूर्णाङ्क fsck_handle_reflog_ent(ध्रुव वर्ण *refname,
				  संरचना object_id *ooid, संरचना object_id *noid,
				  ध्रुव वर्ण *email अप्रयुक्तम्,
				  timestamp_t timestamp, पूर्णाङ्क tz अप्रयुक्तम्,
				  ध्रुव वर्ण *सन्देशः_2 अप्रयुक्तम्, शून्य *cb_दत्त)
{
	संरचना repository *repo = cb_दत्त;

	यदि (now && timestamp > now)
		प्रत्यागम 0;

	यदि (verbose)
		fprintf_ln(stderr, _("Checking reflog %s->%s"),
			   oid_गन्तव्यम्_hex(ooid), oid_गन्तव्यम्_hex(noid));

	fsck_handle_reflog_oid(repo, refname, ooid, 0);
	fsck_handle_reflog_oid(repo, refname, noid, timestamp);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क fsck_handle_reflog(ध्रुव वर्ण *logname, शून्य *cb_दत्त)
{
	संरचना strbuf refname = STRBUF_INIT;
	संरचना worktree *wt = cb_दत्त;

	strbuf_worktree_ref(wt, &refname, logname);
	refs_for_each_reflog_ent(get_main_ref_store(wt->repo),
				 refname.buf, fsck_handle_reflog_ent,
				 wt->repo);
	strbuf_release(&refname);
	प्रत्यागम 0;
}

संरचना ref_snapshot {
	वर्ण *refname;
	संरचना object_id oid;
	/* TODO: Maybe supplement with latest reflog entry info too? */
};

संरचना snapshot {
	आकार_t nr;
	आकार_t alloc;
	संरचना ref_snapshot *ref;
	/* TODO: Consider also snapshotting the index of each worktree. */
};

संरचना snapshot_ref_दत्त {
	संरचना repository *repo;
	संरचना snapshot *snap;
};

स्थिर पूर्णाङ्क snapshot_ref(ध्रुव संरचना reference *ref, शून्य *cb_दत्त)
{
	संरचना snapshot_ref_दत्त *दत्त = cb_दत्त;
	संरचना snapshot *snap = दत्त->snap;
	संरचना object *obj;

	obj = parse_object(दत्त->repo, ref->oid);
	यदि (!obj) {
		यदि (is_promisor_object(दत्त->repo, ref->oid)) {
			/*
			 * Increment default_refs anyway, because this is a
			 * valid ref.
			 */
			 default_refs++;
			 प्रत्यागम 0;
		}
		error(_("%s: invalid sha1 pointer %s"),
		      ref->name, oid_गन्तव्यम्_hex(ref->oid));
		errors_प्राप्तम् |= ERROR_REACHABLE;
		/* We'll continue with the rest despite the error.. */
		प्रत्यागम 0;
	}
	यदि (obj->type != OBJ_COMMIT && is_branch(ref->name)) {
		error(_("%s: not a commit"), ref->name);
		errors_प्राप्तम् |= ERROR_REFS;
	}
	default_refs++;

	ALLOC_GROW(snap->ref, snap->nr + 1, snap->alloc);
	snap->ref[snap->nr].refname = xstrdup(ref->name);
	oidcpy(&snap->ref[snap->nr].oid, ref->oid);
	snap->nr++;

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क fsck_handle_ref(ध्रुव संरचना reference *ref, शून्य *cb_दत्त)
{
	संरचना repository *repo = cb_दत्त;
	संरचना object *obj;

	obj = parse_object(repo, ref->oid);
	obj->flags |= प्रयुक्तम्;
	fsck_put_object_name(&fsck_walk_options,
			     ref->oid, "%s", ref->name);
	mark_object_reachable(obj);

	प्रत्यागम 0;
}

स्थिर शून्य snapshot_refs(संरचना repository *repo,
			  संरचना snapshot *snap, पूर्णाङ्क argc, ध्रुव वर्ण **argv)
{
	संरचना refs_for_each_ref_options opts = {
		.flags = REFS_FOR_EACH_INCLUDE_BROKEN,
	};
	संरचना snapshot_ref_दत्त दत्त = {
		.repo = repo,
		.snap = snap,
	};
	संरचना worktree **worktrees, **p;
	ध्रुव वर्ण *शीर्ष_points_at;
	संरचना object_id शीर्ष_oid;

	प्रति (पूर्णाङ्क i = 0; i < argc; i++) {
		ध्रुव वर्ण *arg = argv[i];
		संरचना object_id oid;
		यदि (!repo_get_oid(repo, arg, &oid)) {
			संरचना reference ref = {
				.name = arg,
				.oid = &oid,
			};

			snapshot_ref(&ref, &दत्त);
			अनुवर्त;
		}
		error(_("invalid parameter: expected sha1, got '%s'"), arg);
		errors_प्राप्तम् |= ERROR_OBJECT;
	}

	यदि (argc) {
		include_reflogs = 0;
		प्रत्यागम;
	}

	refs_for_each_ref_ext(get_main_ref_store(repo),
			      snapshot_ref, &दत्त, &opts);

	worktrees = get_worktrees(repo);
	प्रति (p = worktrees; *p; p++) {
		संरचना worktree *wt = *p;
		संरचना strbuf refname = STRBUF_INIT;

		strbuf_worktree_ref(wt, &refname, "HEAD");

		शीर्ष_points_at = refs_resolve_ref_unsafe(get_main_ref_store(repo),
							 refname.buf, 0, &शीर्ष_oid, रिक्त);

		यदि (शीर्ष_points_at && !is_null_oid(&शीर्ष_oid)) {
			संरचना reference ref = {
				.name = refname.buf,
				.oid = &शीर्ष_oid,
			};

			snapshot_ref(&ref, &दत्त);
		}
		strbuf_release(&refname);

		/*
		 * TODO: Could use refs_for_each_reflog(...) to find
		 * latest entry instead of using a global 'now' for that
		 * purpose.
		 */
	}
	free_worktrees(worktrees);

	/* Ignore reflogs newer than now */
	now = time(रिक्त);
}


स्थिर शून्य free_snapshot_refs(संरचना snapshot *snap)
{
	प्रति (आकार_t i = 0; i < snap->nr; i++)
		मुक्त(snap->ref[i].refname);
	मुक्त(snap->ref);
}

स्थिर शून्य process_refs(संरचना repository *repo, संरचना snapshot *snap)
{
	संरचना worktree **worktrees, **p;

	प्रति (आकार_t i = 0; i < snap->nr; i++) {
		संरचना reference ref = {
			.name = snap->ref[i].refname,
			.oid = &snap->ref[i].oid,
		};
		fsck_handle_ref(&ref, repo);
	}

	यदि (include_reflogs) {
		worktrees = get_worktrees(repo);
		प्रति (p = worktrees; *p; p++) {
			संरचना worktree *wt = *p;

			refs_for_each_reflog(get_worktree_ref_store(wt),
					     fsck_handle_reflog, wt);
		}
		free_worktrees(worktrees);
	}

	/*
	 * Not having any default heads isn't really fatal, but
	 * it does mean that "--unreachable" no longer makes any
	 * sense (since in this case everything will obviously
	 * be unreachable by definition.
	 *
	 * Showing dangling objects is valid, though (as those
	 * dangling objects are likely lost heads).
	 *
	 * So we just print a warning about it, and clear the
	 * "show_unreachable" flag.
	 */
	यदि (!default_refs) {
		fprintf_ln(stderr, _("notice: No default references"));
		show_unreachable = 0;
	}
}

स्थिर पूर्णाङ्क fsck_cache_वृक्षः(संरचना repository *repo, संरचना cache_वृक्षः *it, ध्रुव वर्ण *सूचकांक_मार्गः)
{
	पूर्णाङ्क i;
	पूर्णाङ्क err = 0;

	यदि (verbose)
		fprintf_ln(stderr, _("Checking cache tree of %s"), सूचकांक_मार्गः);

	यदि (0 <= it->प्रविष्टिः_गणनम्) {
		संरचना object *obj = parse_object(repo, &it->oid);
		यदि (!obj) {
			error(_("%s: invalid sha1 pointer in cache-tree of %s"),
			      oid_गन्तव्यम्_hex(&it->oid), सूचकांक_मार्गः);
			errors_प्राप्तम् |= ERROR_REFS;
			प्रत्यागम 1;
		}
		obj->flags |= प्रयुक्तम्;
		fsck_put_object_name(&fsck_walk_options, &it->oid, ":");
		mark_object_reachable(obj);
		यदि (obj->type != OBJ_वृक्षः)
			err |= objerror(repo, obj, _("non-tree in cache-tree"));
	}
	प्रति (i = 0; i < it->subtree_nr; i++)
		err |= fsck_cache_वृक्षः(repo, it->अधस्तात्[i]->cache_वृक्षः, सूचकांक_मार्गः);
	प्रत्यागम err;
}

स्थिर पूर्णाङ्क fsck_resolve_undo(संरचना सूचकांक_state *istate,
			     ध्रुव वर्ण *सूचकांक_मार्गः)
{
	संरचना string_list_item *item;
	संरचना string_list *resolve_undo = istate->resolve_undo;

	यदि (!resolve_undo)
		प्रत्यागम 0;

	for_each_string_list_item(item, resolve_undo) {
		ध्रुव वर्ण *मार्गः = item->string;
		संरचना resolve_undo_info *ru = item->util;
		पूर्णाङ्क i;

		यदि (!ru)
			अनुवर्त;
		प्रति (i = 0; i < 3; i++) {
			संरचना object *obj;

			यदि (!ru->बहुलकम्[i] || !S_ISREG(ru->बहुलकम्[i]))
				अनुवर्त;

			obj = parse_object(istate->repo, &ru->oid[i]);
			यदि (!obj) {
				error(_("%s: invalid sha1 pointer in resolve-undo of %s"),
				      oid_गन्तव्यम्_hex(&ru->oid[i]),
				      सूचकांक_मार्गः);
				errors_प्राप्तम् |= ERROR_REFS;
				अनुवर्त;
			}
			obj->flags |= प्रयुक्तम्;
			fsck_put_object_name(&fsck_walk_options, &ru->oid[i],
					     ":(%d):%s", i, मार्गः);
			mark_object_reachable(obj);
		}
	}
	प्रत्यागम 0;
}

स्थिर शून्य fsck_सूचकांक(संरचना सूचकांक_state *istate, ध्रुव वर्ण *सूचकांक_मार्गः,
		       पूर्णाङ्क is_current_worktree)
{
	अचिह्नित पूर्णाङ्क i;

	/* TODO: audit for interaction with sparse-index. */
	ensure_पूर्णम्_सूचकांक(istate);
	प्रति (i = 0; i < istate->cache_nr; i++) {
		अचिह्नित पूर्णाङ्क बहुलकम्;
		संरचना blob *blob;
		संरचना object *obj;

		बहुलकम् = istate->cache[i]->ce_बहुलकम्;
		यदि (S_ISGITLINK(बहुलकम्))
			अनुवर्त;
		blob = lookup_blob(istate->repo,
				   &istate->cache[i]->oid);
		यदि (!blob)
			अनुवर्त;
		obj = &blob->object;
		obj->flags |= प्रयुक्तम्;
		fsck_put_object_name(&fsck_walk_options, &obj->oid,
				     "%s:%s",
				     is_current_worktree ? "" : सूचकांक_मार्गः,
				     istate->cache[i]->name);
		mark_object_reachable(obj);
	}
	यदि (istate->cache_वृक्षः)
		fsck_cache_वृक्षः(istate->repo, istate->cache_वृक्षः, सूचकांक_मार्गः);
	fsck_resolve_undo(istate, सूचकांक_मार्गः);
}

स्थिर पूर्णाङ्क mark_object_for_connectivity(ध्रुव संरचना object_id *oid,
					संरचना object_info *oi अप्रयुक्तम्,
					शून्य *cb_दत्त)
{
	संरचना repository *repo = cb_दत्त;
	संरचना object *obj = lookup_unknown_object(repo, oid);
	obj->flags |= HAS_OBJ;
	प्रत्यागम 0;
}

स्थिर शून्य fsck_refs(संरचना repository *r)
{
	संरचना child_process refs_verify = CHILD_PROCESS_INIT;
	संरचना प्रगतिः *प्रगतिः = रिक्त;

	यदि (show_प्रगतिः)
		प्रगतिः = start_प्रगतिः(r, _("Checking ref database"), 1);

	यदि (verbose)
		fprintf_ln(stderr, _("Checking ref database"));

	child_process_init(&refs_verify);
	refs_verify.git_cmd = 1;
	strvec_pushl(&refs_verify.तर्काः, "refs", "verify", रिक्त);
	यदि (verbose)
		strvec_प्रक्षिप(&refs_verify.तर्काः, "--verbose");
	यदि (check_strict)
		strvec_प्रक्षिप(&refs_verify.तर्काः, "--strict");

	यदि (सततम्_command(&refs_verify))
		errors_प्राप्तम् |= ERROR_REFS;

	display_प्रगतिः(प्रगतिः, 1);
	stop_प्रगतिः(&प्रगतिः);
}

स्थिर वर्ण ध्रुव * ध्रुव fsck_usage[] = {
	N_("git fsck [--tags] [--root] [--unreachable] [--cache] [--no-reflogs]\n"
	   "         [--[no-]full] [--strict] [--verbose] [--lost-found]\n"
	   "         [--[no-]dangling] [--[no-]progress] [--connectivity-only]\n"
	   "         [--[no-]name-objects] [--[no-]references] [<object>...]"),
	रिक्त
};

पूर्णाङ्क cmd_fsck(पूर्णाङ्क argc,
	     ध्रुव वर्ण **argv,
	     ध्रुव वर्ण *उपसर्गः,
	     संरचना repository *repo)
{
	संरचना odb_fsck_options odb_fsck_opts = {
		.flags = ODB_FSCK_पूर्णम्,
		.object_cb = fsck_obj_बफर,
		.object_payload = repo,
	};
	पूर्णाङ्क keep_cache_objects = 0;
	पूर्णाङ्क name_objects = 0;
	पूर्णाङ्क check_references = 1;
	संरचना option fsck_opts[] = {
		OPT__VERBOSE(&verbose, N_("be verbose")),
		OPT_BOOL(0, "unreachable", &show_unreachable, N_("show unreachable objects")),
		OPT_BOOL(0, "dangling", &show_dangling, N_("show dangling objects")),
		OPT_BOOL(0, "tags", &show_चिह्नकानि, N_("report tags")),
		OPT_BOOL(0, "root", &show_मूलम्, N_("report root nodes")),
		OPT_BOOL(0, "cache", &keep_cache_objects, N_("make index objects head nodes")),
		OPT_BOOL(0, "reflogs", &include_reflogs, N_("make reflogs head nodes (default)")),
		OPT_BIT(0, "full", &odb_fsck_opts.flags,
			N_("also consider packs and alternate objects"), ODB_FSCK_पूर्णम्),
		OPT_BOOL(0, "connectivity-only", &connectivity_only, N_("check only connectivity")),
		OPT_BOOL(0, "strict", &check_strict, N_("enable more strict checking")),
		OPT_BOOL(0, "lost-found", &write_नष्टम्_and_प्राप्तम्,
					N_("write dangling objects in .git/lost-found")),
		OPT_BOOL(0, "progress", &show_प्रगतिः, N_("show progress")),
		OPT_BOOL(0, "name-objects", &name_objects, N_("show verbose names for reachable objects")),
		OPT_BOOL(0, "references", &check_references, N_("check reference database consistency")),
		OPT_अन्तः(),
	};
	संरचना snapshot snap = {
		.nr = 0,
		.alloc = 0,
		.ref = रिक्त
	};

	errors_प्राप्तम् = 0;
	disable_replace_refs();
	save_commit_बफर = 0;

	argc = parse_options(argc, argv, उपसर्गः, fsck_opts, fsck_usage, 0);

	/* fsck knows how to handle missing promisor objects */
	repo->fetch_if_missing = 0;

	fsck_options_init(&fsck_walk_options, repo, FSCK_OPTIONS_DEFAULT);
	fsck_walk_options.walk = mark_object;

	fsck_options_init(&fsck_obj_options, repo, FSCK_OPTIONS_DEFAULT);
	fsck_obj_options.walk = mark_प्रयुक्तम्;
	fsck_obj_options.error_func = fsck_objects_error_func;
	यदि (check_strict)
		fsck_obj_options.strict = 1;

	यदि (show_प्रगतिः == -1)
		show_प्रगतिः = isatty(2);
	यदि (verbose) {
		show_प्रगतिः = 0;
		odb_fsck_opts.flags |= ODB_FSCK_VERBOSE;
	}
	यदि (show_प्रगतिः)
		odb_fsck_opts.flags |= ODB_FSCK_प्रगतिः;

	यदि (write_नष्टम्_and_प्राप्तम्) {
		odb_fsck_opts.flags |= ODB_FSCK_पूर्णम्;
		include_reflogs = 0;
	}

	यदि (name_objects)
		fsck_enable_object_नामानि(&fsck_walk_options);

	repo_config(repo, git_fsck_config, &fsck_obj_options);
	prepare_repo_settings(repo);

	यदि (check_references)
		fsck_refs(repo);

	/*
	 * Take a snapshot of the refs before walking objects to avoid looking
	 * at a set of refs that may be changed by the user while we are walking
	 * objects. We can still walk over new objects that are added during the
	 * execution of fsck but won't miss any objects that were reachable.
	 */
	snapshot_refs(repo, &snap, argc, argv);

	/* Ensure we get a "fresh" view of the odb */
	odb_reprepare(repo->objects);

	यदि (connectivity_only) {
		odb_for_each_object(repo->objects, रिक्त,
				    mark_object_for_connectivity, repo, 0);
	} अन्यथा {
		यदि (odb_fsck(repo->objects, &odb_fsck_opts) < 0)
			errors_प्राप्तम् |= ERROR_OBJECT;

		यदि (fsck_finish(&fsck_obj_options))
			errors_प्राप्तम् |= ERROR_OBJECT;
	}

	/* Process the snapshotted refs and the reflogs. */
	process_refs(repo, &snap);

	/* If not given any explicit objects, process index files too. */
	यदि (!argc)
		keep_cache_objects = 1;
	यदि (keep_cache_objects) {
		/*
		 * TODO: Consider first walking these indexes in snapshot_refs,
		 * to snapshot where the index entries used to point, and then
		 * check those snapshotted locations here.
		 */
		संरचना worktree **worktrees, **p;

		verify_सूचकांक_checksum = 1;
		verify_ce_क्रमः = 1;

		worktrees = get_worktrees(repo);
		प्रति (p = worktrees; *p; p++) {
			संरचना worktree *wt = *p;
			संरचना सूचकांक_state istate =
				सूचकांक_STATE_INIT(repo);
			वर्ण *मार्गः, *wt_gitdir;

			/*
			 * Make a copy since the buffer is reusable
			 * and may get overwritten by other calls
			 * while we're examining the index.
			 */
			मार्गः = xstrdup(worktree_git_मार्गः(wt, "index"));
			wt_gitdir = get_worktree_git_dir(wt);

			read_सूचकांक_from(&istate, मार्गः, wt_gitdir);
			fsck_सूचकांक(&istate, मार्गः, wt->is_current);

			discard_सूचकांक(&istate);
			मुक्त(wt_gitdir);
			मुक्त(मार्गः);
		}
		free_worktrees(worktrees);
	}

	check_connectivity(repo);

	यदि (repo->settings.core_commit_graph) {
		संरचना odb_source_files *files = odb_source_files_downcast(repo->objects->source);
		संरचना child_process commit_graph_verify = CHILD_PROCESS_INIT;

		प्रति (संरचना odb_files_dir *dir = files->dirs; dir; dir = dir->अग्र) {
			child_process_init(&commit_graph_verify);
			commit_graph_verify.git_cmd = 1;
			strvec_pushl(&commit_graph_verify.तर्काः, "commit-graph",
				     "verify", "--object-dir", dir->abspath, रिक्त);
			यदि (show_प्रगतिः)
				strvec_प्रक्षिप(&commit_graph_verify.तर्काः, "--progress");
			अन्यथा
				strvec_प्रक्षिप(&commit_graph_verify.तर्काः, "--no-progress");
			यदि (सततम्_command(&commit_graph_verify))
				errors_प्राप्तम् |= ERROR_COMMIT_GRAPH;
		}
	}

	free_snapshot_refs(&snap);
	प्रत्यागम errors_प्राप्तम्;
}
