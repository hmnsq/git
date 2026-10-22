#అన్తర్భావ "builtin.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "hex.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "commit.h"
#అన్తర్భావ "tree.h"
#అన్తర్భావ "blob.h"
#అన్తర్భావ "tag.h"
#అన్తర్భావ "refs.h"
#అన్తర్భావ "cache-tree.h"
#అన్తర్భావ "fsck.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "progress.h"
#అన్తర్భావ "packfile.h"
#అన్తర్భావ "object-name.h"
#అన్తర్భావ "odb.h"
#అన్తర్భావ "odb/streaming.h"
#అన్తర్భావ "path.h"
#అన్తర్భావ "read-cache-ll.h"
#అన్తర్భావ "replace-object.h"
#అన్తర్భావ "resolve-undo.h"
#అన్తర్భావ "run-command.h"
#అన్తర్భావ "sparse-index.h"
#అన్తర్భావ "worktree.h"

#నిర్వచించు REACHABLE 0x0001
#నిర్వచించు చూసిన_2      0x0002
#నిర్వచించు HAS_OBJ   0x0004
/* This flag is set if something points to this object. */
#నిర్వచించు వాడిన      0x0008

స్థిర పూర్ణాంకం show_మూలం;
స్థిర పూర్ణాంకం show_ట్యాగులు;
స్థిర పూర్ణాంకం show_unreachable;
స్థిర పూర్ణాంకం include_reflogs = 1;
స్థిర పూర్ణాంకం connectivity_only;
స్థిర పూర్ణాంకం check_strict;
స్థిర నిర్మాణం fsck_options fsck_walk_options;
స్థిర నిర్మాణం fsck_options fsck_obj_options;
స్థిర పూర్ణాంకం errors_దొరికింది;
స్థిర పూర్ణాంకం write_కోల్పోయిన_and_దొరికింది;
స్థిర పూర్ణాంకం verbose;
స్థిర పూర్ణాంకం show_పురోగతి = -1;
స్థిర పూర్ణాంకం show_dangling = 1;
స్థిర timestamp_t now;
#నిర్వచించు ERROR_OBJECT 01
#నిర్వచించు ERROR_REACHABLE 02
#నిర్వచించు ERROR_REFS 010
#నిర్వచించు ERROR_COMMIT_లేఖాచిత్రం 020

స్థిర స్థిరరాశి అక్షరం *describe_object(స్థిరరాశి నిర్మాణం object_id *oid)
{
	తిరిగి fsck_describe_object(&fsck_walk_options, oid);
}

స్థిర స్థిరరాశి అక్షరం *printable_type(నిర్మాణం repository *repo,
				  స్థిరరాశి నిర్మాణం object_id *oid,
				  గణన object_type type)
{
	స్థిరరాశి అక్షరం *ret;

	ఒకవేళ (type == OBJ_NONE)
		type = odb_read_object_info(repo->objects, oid, ఖాళీ);

	ret = type_name(type);
	ఒకవేళ (!ret)
		ret = _("unknown");

	తిరిగి ret;
}

స్థిర పూర్ణాంకం objerror(నిర్మాణం repository *repo, నిర్మాణం object *obj, స్థిరరాశి అక్షరం *err)
{
	errors_దొరికింది |= ERROR_OBJECT;
	/* TRANSLATORS: e.g. error in tree 01bfda: <more explanation> */
	fprintf_ln(stderr, _("error in %s %s: %s"),
		   printable_type(repo, &obj->oid, obj->type),
		   describe_object(&obj->oid), err);
	తిరిగి -1;
}

స్థిర పూర్ణాంకం fsck_objects_error_func(నిర్మాణం fsck_options *o,
				   శూన్య *fsck_report,
				   గణన fsck_సందేశం_type సందేశం_type,
				   గణన fsck_సందేశం_id సందేశం_id_2 వాడని,
				   స్థిరరాశి అక్షరం *సందేశం_2)
{
	నిర్మాణం fsck_object_report *report = fsck_report;
	స్థిరరాశి నిర్మాణం object_id *oid = report->oid;
	గణన object_type object_type = report->object_type;

	ఎంపిక (సందేశం_type) {
	స్థితి FSCK_WARN:
		/* TRANSLATORS: e.g. warning in tree 01bfda: <more explanation> */
		fprintf_ln(stderr, _("warning in %s %s: %s"),
			   printable_type(o->repo, oid, object_type),
			   describe_object(oid), సందేశం_2);
		తిరిగి 0;
	స్థితి FSCK_ERROR:
		/* TRANSLATORS: e.g. error in tree 01bfda: <more explanation> */
		fprintf_ln(stderr, _("error in %s %s: %s"),
			   printable_type(o->repo, oid, object_type),
			   describe_object(oid), సందేశం_2);
		తిరిగి 1;
	అప్రమేయ:
		BUG("%d (FSCK_IGNORE?) should never trigger this callback",
		    సందేశం_type);
	}
}

స్థిర నిర్మాణం object_array pending;

స్థిర పూర్ణాంకం mark_object(నిర్మాణం object *obj, గణన object_type type,
		       శూన్య *దత్తాంశం, నిర్మాణం fsck_options *options)
{
	నిర్మాణం object *మాతృ = దత్తాంశం;

	/*
	 * The only case data is NULL or type is OBJ_ANY is when
	 * mark_object_reachable() calls us.  All the callers of
	 * that function has non-NULL obj hence ...
	 */
	ఒకవేళ (!obj) {
		/* ... these references to parent->fld are safe here */
		printf_ln(_("broken link from %7s %s"),
			  printable_type(options->repo, &మాతృ->oid, మాతృ->type),
			  describe_object(&మాతృ->oid));
		printf_ln(_("broken link from %7s %s"),
			  (type == OBJ_ANY ? _("unknown") : type_name(type)),
			  _("unknown"));
		errors_దొరికింది |= ERROR_REACHABLE;
		తిరిగి 1;
	}

	ఒకవేళ (type != OBJ_ANY && obj->type != type)
		/* ... and the reference to parent is safe here */
		objerror(options->repo, మాతృ, _("wrong object type in link"));

	ఒకవేళ (obj->flags & REACHABLE)
		తిరిగి 0;
	obj->flags |= REACHABLE;

	ఒకవేళ (is_promisor_object(options->repo, &obj->oid))
		/*
		 * Further recursion does not need to be performed on this
		 * object since it is a promisor object (so it does not need to
		 * be added to "pending").
		 */
		తిరిగి 0;

	ఒకవేళ (!(obj->flags & HAS_OBJ)) {
		ఒకవేళ (మాతృ && !odb_has_object(options->repo->objects, &obj->oid,
					      ODB_HAS_OBJECT_RECHECK_PACKED)) {
			printf_ln(_("broken link from %7s %s\n"
				    "              to %7s %s"),
				  printable_type(options->repo, &మాతృ->oid, మాతృ->type),
				  describe_object(&మాతృ->oid),
				  printable_type(options->repo, &obj->oid, obj->type),
				  describe_object(&obj->oid));
			errors_దొరికింది |= ERROR_REACHABLE;
		}
		తిరిగి 1;
	}

	కూడు_object_array(obj, ఖాళీ, &pending);
	తిరిగి 0;
}

స్థిర శూన్య mark_object_reachable(నిర్మాణం object *obj)
{
	mark_object(obj, OBJ_ANY, ఖాళీ, &fsck_walk_options);
}

స్థిర పూర్ణాంకం traverse_one_object(నిర్మాణం object *obj)
{
	పూర్ణాంకం ఫలితం = fsck_walk(obj, obj, &fsck_walk_options);

	ఒకవేళ (obj->type == OBJ_వృక్షం) {
		నిర్మాణం వృక్షం *వృక్షం = (నిర్మాణం వృక్షం *)obj;
		free_వృక్షం_బఫరు(వృక్షం);
	}
	తిరిగి ఫలితం;
}

స్థిర పూర్ణాంకం traverse_reachable(నిర్మాణం repository *repo)
{
	నిర్మాణం పురోగతి *పురోగతి = ఖాళీ;
	గుర్తులేని పూర్ణాంకం nr = 0;
	పూర్ణాంకం ఫలితం = 0;
	ఒకవేళ (show_పురోగతి)
		పురోగతి = ప్రారంభం_delayed_పురోగతి(repo,
						  _("Checking connectivity"), 0);
	అయితే (pending.nr) {
		ఫలితం |= traverse_one_object(object_array_తీయి(&pending));
		display_పురోగతి(పురోగతి, ++nr);
	}
	stop_పురోగతి(&పురోగతి);
	తిరిగి !!ఫలితం;
}

స్థిర పూర్ణాంకం mark_వాడిన(నిర్మాణం object *obj, గణన object_type type వాడని,
		     శూన్య *దత్తాంశం వాడని, నిర్మాణం fsck_options *options వాడని)
{
	ఒకవేళ (!obj)
		తిరిగి 1;
	obj->flags |= వాడిన;
	తిరిగి 0;
}

స్థిర పూర్ణాంకం mark_unreachable_referents(స్థిరరాశి నిర్మాణం object_id *oid,
				      నిర్మాణం object_info *oi వాడని,
				      శూన్య *దత్తాంశం)
{
	నిర్మాణం repository *repo = దత్తాంశం;
	నిర్మాణం fsck_options options;
	నిర్మాణం object *obj = lookup_object(దత్తాంశం, oid);

	ఒకవేళ (!obj || !(obj->flags & HAS_OBJ))
		తిరిగి 0; /* not part of our original set */
	ఒకవేళ (obj->flags & REACHABLE)
		తిరిగి 0; /* reachable objects already traversed */

	/*
	 * Avoid passing OBJ_NONE to fsck_walk, which will parse the object
	 * (and we want to avoid parsing blobs).
	 */
	ఒకవేళ (obj->type == OBJ_NONE) {
		గణన object_type type = odb_read_object_info(repo->objects,
							     &obj->oid, ఖాళీ);
		ఒకవేళ (type > 0)
			object_as_type(obj, type, 0);
	}

	fsck_options_init(&options, repo, FSCK_OPTIONS_DEFAULT);
	options.walk = mark_వాడిన;
	fsck_walk(obj, ఖాళీ, &options);
	ఒకవేళ (obj->type == OBJ_వృక్షం)
		free_వృక్షం_బఫరు((నిర్మాణం వృక్షం *)obj);

	తిరిగి 0;
}

/*
 * Check a single reachable object
 */
స్థిర శూన్య check_reachable_object(నిర్మాణం repository *repo, నిర్మాణం object *obj)
{
	/*
	 * We obviously want the object to be parsed,
	 * except if it was in a pack-file and we didn't
	 * do a full fsck
	 */
	ఒకవేళ (!(obj->flags & HAS_OBJ)) {
		ఒకవేళ (is_promisor_object(repo, &obj->oid))
			తిరిగి;
		ఒకవేళ (has_object_pack(repo, &obj->oid))
			తిరిగి; /* it is in pack - forget about it */
		printf_ln(_("missing %s %s"),
			  printable_type(repo, &obj->oid, obj->type),
			  describe_object(&obj->oid));
		errors_దొరికింది |= ERROR_REACHABLE;
		తిరిగి;
	}
}

/*
 * Check a single unreachable object
 */
స్థిర శూన్య check_unreachable_object(నిర్మాణం repository *repo, నిర్మాణం object *obj)
{
	/*
	 * Missing unreachable object? Ignore it. It's not like
	 * we miss it (since it can't be reached), nor do we want
	 * to complain about it being unreachable (since it does
	 * not exist).
	 */
	ఒకవేళ (!(obj->flags & HAS_OBJ))
		తిరిగి;

	/*
	 * Unreachable object that exists? Show it if asked to,
	 * since this is something that is prunable.
	 */
	ఒకవేళ (show_unreachable) {
		printf_ln(_("unreachable %s %s"),
			  printable_type(repo, &obj->oid, obj->type),
			  describe_object(&obj->oid));
		తిరిగి;
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
	ఒకవేళ (!(obj->flags & వాడిన)) {
		ఒకవేళ (show_dangling)
			printf_ln(_("dangling %s %s"),
				  printable_type(repo, &obj->oid, obj->type),
				  describe_object(&obj->oid));
		ఒకవేళ (write_కోల్పోయిన_and_దొరికింది) {
			అక్షరం *filename = repo_git_మార్గం(repo, "lost-found/%s/%s",
				obj->type == OBJ_COMMIT ? "commit" : "other",
				describe_object(&obj->oid));
			FILE *f;

			ఒకవేళ (సురక్షిత_create_leading_directories_const(repo, filename)) {
				error(_("could not create lost-found"));
				విడుదల(filename);
				తిరిగి;
			}
			f = xfopen(filename, "w");
			ఒకవేళ (obj->type == OBJ_BLOB) {
				ఒకవేళ (odb_ప్రవాహం_blob_గమ్యం_fd(repo->objects, fileno(f),
							  &obj->oid, ఖాళీ, 1))
					die_errno(_("could not write '%s'"), filename);
			} లేకపోతే
				fprintf(f, "%s\n", describe_object(&obj->oid));
			ఒకవేళ (fclose(f))
				die_errno(_("could not finish '%s'"),
					  filename);
			విడుదల(filename);
		}
		తిరిగి;
	}

	/*
	 * Otherwise? It's there, it's unreachable, and some other unreachable
	 * object points to it. Ignore it - it's not interesting, and we showed
	 * all the interesting cases above.
	 */
}

స్థిర శూన్య check_object(నిర్మాణం repository *repo, నిర్మాణం object *obj)
{
	ఒకవేళ (verbose)
		fprintf_ln(stderr, _("Checking %s"), describe_object(&obj->oid));

	ఒకవేళ (obj->flags & REACHABLE)
		check_reachable_object(repo, obj);
	లేకపోతే
		check_unreachable_object(repo, obj);
}

స్థిర శూన్య check_connectivity(నిర్మాణం repository *repo)
{
	పూర్ణాంకం i, max;

	/* Traverse the pending reachable objects */
	traverse_reachable(repo);

	/*
	 * With --connectivity-only, we won't have actually opened and marked
	 * unreachable objects with USED. Do that now to make --dangling, etc
	 * accurate.
	 */
	ఒకవేళ (connectivity_only && (show_dangling || write_కోల్పోయిన_and_దొరికింది)) {
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
		odb_for_each_object(repo->objects, ఖాళీ,
				    mark_unreachable_referents, repo, 0);
	}

	/* Look up all the requirements, warn about missing objects.. */
	max = get_max_object_సూచీ(repo);
	ఒకవేళ (verbose)
		fprintf_ln(stderr, _("Checking connectivity (%d objects)"), max);

	కొరకు (i = 0; i < max; i++) {
		నిర్మాణం object *obj = get_సూచీకృత_object(repo, i);

		ఒకవేళ (obj)
			check_object(repo, obj);
	}
}

స్థిర పూర్ణాంకం fsck_obj_బఫరు(స్థిరరాశి నిర్మాణం object_id *oid, గణన object_type type,
			   గుర్తులేని దీర్ఘ కొలత, శూన్య *బఫరు, పూర్ణాంకం *eaten, శూన్య *cb_దత్తాంశం)
{
	నిర్మాణం repository *repo = cb_దత్తాంశం;
	నిర్మాణం object *obj;
	పూర్ణాంకం err;

	/*
	 * Note, buffer may be NULL if type is OBJ_BLOB. See
	 * verify_packfile(), data_valid variable for details.
	 */
	obj = parse_object_బఫరు(repo, oid, type, కొలత, బఫరు, eaten);
	ఒకవేళ (!obj) {
		errors_దొరికింది |= ERROR_OBJECT;
		err = error(_("%s: object corrupt or missing"),
			    oid_గమ్యం_hex(oid));
		వెళ్ళు బహిర్గతం;
	}

	obj->flags &= ~REACHABLE;
	obj->flags |= HAS_OBJ | చూసిన_2;

	ఒకవేళ (verbose)
		fprintf_ln(stderr, _("Checking %s %s"),
			   printable_type(repo, &obj->oid, obj->type),
			   describe_object(&obj->oid));

	ఒకవేళ (fsck_walk(obj, ఖాళీ, &fsck_obj_options))
		objerror(repo, obj, _("broken links"));

	err = fsck_object(obj, బఫరు, కొలత, &fsck_obj_options);
	ఒకవేళ (err)
		వెళ్ళు బహిర్గతం;

	ఒకవేళ (obj->type == OBJ_COMMIT) {
		నిర్మాణం commit *commit = (నిర్మాణం commit *) obj;

		ఒకవేళ (!commit->parents && show_మూలం)
			printf_ln(_("root %s"),
				  describe_object(&commit->object.oid));
	}

	ఒకవేళ (obj->type == OBJ_ట్యాగ్) {
		నిర్మాణం ట్యాగ్ *ట్యాగ్ = (నిర్మాణం ట్యాగ్ *) obj;

		ఒకవేళ (show_ట్యాగులు && ట్యాగ్->tagged) {
			printf_ln(_("tagged %s %s (%s) in %s"),
				  printable_type(repo, &ట్యాగ్->tagged->oid, ట్యాగ్->tagged->type),
				  describe_object(&ట్యాగ్->tagged->oid),
				  ట్యాగ్->ట్యాగ్,
				  describe_object(&ట్యాగ్->object.oid));
		}
	}

బహిర్గతం:
	ఒకవేళ (obj && obj->type == OBJ_వృక్షం)
		free_వృక్షం_బఫరు((నిర్మాణం వృక్షం *)obj);
	తిరిగి err;
}

స్థిర పూర్ణాంకం default_refs;

స్థిర శూన్య fsck_handle_reflog_oid(నిర్మాణం repository *repo,
				   స్థిరరాశి అక్షరం *refname, నిర్మాణం object_id *oid,
				   timestamp_t timestamp)
{
	నిర్మాణం object *obj;

	ఒకవేళ (!is_null_oid(oid)) {
		obj = lookup_object(repo, oid);
		ఒకవేళ (obj && (obj->flags & HAS_OBJ)) {
			ఒకవేళ (timestamp)
				fsck_put_object_name(&fsck_walk_options, oid,
						     "%s@{%"PRItime"}",
						     refname, timestamp);
			obj->flags |= వాడిన;
			mark_object_reachable(obj);
		} లేకపోతే ఒకవేళ (!is_promisor_object(repo, oid)) {
			error(_("%s: invalid reflog entry %s"),
			      refname, oid_గమ్యం_hex(oid));
			errors_దొరికింది |= ERROR_REACHABLE;
		}
	}
}

స్థిర పూర్ణాంకం fsck_handle_reflog_ent(స్థిరరాశి అక్షరం *refname,
				  నిర్మాణం object_id *ooid, నిర్మాణం object_id *noid,
				  స్థిరరాశి అక్షరం *email వాడని,
				  timestamp_t timestamp, పూర్ణాంకం tz వాడని,
				  స్థిరరాశి అక్షరం *సందేశం_2 వాడని, శూన్య *cb_దత్తాంశం)
{
	నిర్మాణం repository *repo = cb_దత్తాంశం;

	ఒకవేళ (now && timestamp > now)
		తిరిగి 0;

	ఒకవేళ (verbose)
		fprintf_ln(stderr, _("Checking reflog %s->%s"),
			   oid_గమ్యం_hex(ooid), oid_గమ్యం_hex(noid));

	fsck_handle_reflog_oid(repo, refname, ooid, 0);
	fsck_handle_reflog_oid(repo, refname, noid, timestamp);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం fsck_handle_reflog(స్థిరరాశి అక్షరం *logname, శూన్య *cb_దత్తాంశం)
{
	నిర్మాణం strbuf refname = STRBUF_INIT;
	నిర్మాణం worktree *wt = cb_దత్తాంశం;

	strbuf_worktree_ref(wt, &refname, logname);
	refs_for_each_reflog_ent(get_main_ref_store(wt->repo),
				 refname.buf, fsck_handle_reflog_ent,
				 wt->repo);
	strbuf_release(&refname);
	తిరిగి 0;
}

నిర్మాణం ref_snapshot {
	అక్షరం *refname;
	నిర్మాణం object_id oid;
	/* TODO: Maybe supplement with latest reflog entry info too? */
};

నిర్మాణం snapshot {
	కొలత_t nr;
	కొలత_t alloc;
	నిర్మాణం ref_snapshot *ref;
	/* TODO: Consider also snapshotting the index of each worktree. */
};

నిర్మాణం snapshot_ref_దత్తాంశం {
	నిర్మాణం repository *repo;
	నిర్మాణం snapshot *snap;
};

స్థిర పూర్ణాంకం snapshot_ref(స్థిరరాశి నిర్మాణం reference *ref, శూన్య *cb_దత్తాంశం)
{
	నిర్మాణం snapshot_ref_దత్తాంశం *దత్తాంశం = cb_దత్తాంశం;
	నిర్మాణం snapshot *snap = దత్తాంశం->snap;
	నిర్మాణం object *obj;

	obj = parse_object(దత్తాంశం->repo, ref->oid);
	ఒకవేళ (!obj) {
		ఒకవేళ (is_promisor_object(దత్తాంశం->repo, ref->oid)) {
			/*
			 * Increment default_refs anyway, because this is a
			 * valid ref.
			 */
			 default_refs++;
			 తిరిగి 0;
		}
		error(_("%s: invalid sha1 pointer %s"),
		      ref->name, oid_గమ్యం_hex(ref->oid));
		errors_దొరికింది |= ERROR_REACHABLE;
		/* We'll continue with the rest despite the error.. */
		తిరిగి 0;
	}
	ఒకవేళ (obj->type != OBJ_COMMIT && is_branch(ref->name)) {
		error(_("%s: not a commit"), ref->name);
		errors_దొరికింది |= ERROR_REFS;
	}
	default_refs++;

	ALLOC_GROW(snap->ref, snap->nr + 1, snap->alloc);
	snap->ref[snap->nr].refname = xstrdup(ref->name);
	oidcpy(&snap->ref[snap->nr].oid, ref->oid);
	snap->nr++;

	తిరిగి 0;
}

స్థిర పూర్ణాంకం fsck_handle_ref(స్థిరరాశి నిర్మాణం reference *ref, శూన్య *cb_దత్తాంశం)
{
	నిర్మాణం repository *repo = cb_దత్తాంశం;
	నిర్మాణం object *obj;

	obj = parse_object(repo, ref->oid);
	obj->flags |= వాడిన;
	fsck_put_object_name(&fsck_walk_options,
			     ref->oid, "%s", ref->name);
	mark_object_reachable(obj);

	తిరిగి 0;
}

స్థిర శూన్య snapshot_refs(నిర్మాణం repository *repo,
			  నిర్మాణం snapshot *snap, పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv)
{
	నిర్మాణం refs_for_each_ref_options opts = {
		.flags = REFS_FOR_EACH_INCLUDE_BROKEN,
	};
	నిర్మాణం snapshot_ref_దత్తాంశం దత్తాంశం = {
		.repo = repo,
		.snap = snap,
	};
	నిర్మాణం worktree **worktrees, **p;
	స్థిరరాశి అక్షరం *తల_points_at;
	నిర్మాణం object_id తల_oid;

	కొరకు (పూర్ణాంకం i = 0; i < argc; i++) {
		స్థిరరాశి అక్షరం *arg = argv[i];
		నిర్మాణం object_id oid;
		ఒకవేళ (!repo_get_oid(repo, arg, &oid)) {
			నిర్మాణం reference ref = {
				.name = arg,
				.oid = &oid,
			};

			snapshot_ref(&ref, &దత్తాంశం);
			కొనసాగు;
		}
		error(_("invalid parameter: expected sha1, got '%s'"), arg);
		errors_దొరికింది |= ERROR_OBJECT;
	}

	ఒకవేళ (argc) {
		include_reflogs = 0;
		తిరిగి;
	}

	refs_for_each_ref_ext(get_main_ref_store(repo),
			      snapshot_ref, &దత్తాంశం, &opts);

	worktrees = get_worktrees(repo);
	కొరకు (p = worktrees; *p; p++) {
		నిర్మాణం worktree *wt = *p;
		నిర్మాణం strbuf refname = STRBUF_INIT;

		strbuf_worktree_ref(wt, &refname, "HEAD");

		తల_points_at = refs_resolve_ref_unsafe(get_main_ref_store(repo),
							 refname.buf, 0, &తల_oid, ఖాళీ);

		ఒకవేళ (తల_points_at && !is_null_oid(&తల_oid)) {
			నిర్మాణం reference ref = {
				.name = refname.buf,
				.oid = &తల_oid,
			};

			snapshot_ref(&ref, &దత్తాంశం);
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
	now = సమయం(ఖాళీ);
}


స్థిర శూన్య free_snapshot_refs(నిర్మాణం snapshot *snap)
{
	కొరకు (కొలత_t i = 0; i < snap->nr; i++)
		విడుదల(snap->ref[i].refname);
	విడుదల(snap->ref);
}

స్థిర శూన్య process_refs(నిర్మాణం repository *repo, నిర్మాణం snapshot *snap)
{
	నిర్మాణం worktree **worktrees, **p;

	కొరకు (కొలత_t i = 0; i < snap->nr; i++) {
		నిర్మాణం reference ref = {
			.name = snap->ref[i].refname,
			.oid = &snap->ref[i].oid,
		};
		fsck_handle_ref(&ref, repo);
	}

	ఒకవేళ (include_reflogs) {
		worktrees = get_worktrees(repo);
		కొరకు (p = worktrees; *p; p++) {
			నిర్మాణం worktree *wt = *p;

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
	ఒకవేళ (!default_refs) {
		fprintf_ln(stderr, _("notice: No default references"));
		show_unreachable = 0;
	}
}

స్థిర పూర్ణాంకం fsck_cache_వృక్షం(నిర్మాణం repository *repo, నిర్మాణం cache_వృక్షం *it, స్థిరరాశి అక్షరం *సూచీ_మార్గం)
{
	పూర్ణాంకం i;
	పూర్ణాంకం err = 0;

	ఒకవేళ (verbose)
		fprintf_ln(stderr, _("Checking cache tree of %s"), సూచీ_మార్గం);

	ఒకవేళ (0 <= it->నమోదు_లెక్క) {
		నిర్మాణం object *obj = parse_object(repo, &it->oid);
		ఒకవేళ (!obj) {
			error(_("%s: invalid sha1 pointer in cache-tree of %s"),
			      oid_గమ్యం_hex(&it->oid), సూచీ_మార్గం);
			errors_దొరికింది |= ERROR_REFS;
			తిరిగి 1;
		}
		obj->flags |= వాడిన;
		fsck_put_object_name(&fsck_walk_options, &it->oid, ":");
		mark_object_reachable(obj);
		ఒకవేళ (obj->type != OBJ_వృక్షం)
			err |= objerror(repo, obj, _("non-tree in cache-tree"));
	}
	కొరకు (i = 0; i < it->subtree_nr; i++)
		err |= fsck_cache_వృక్షం(repo, it->కిందికి[i]->cache_వృక్షం, సూచీ_మార్గం);
	తిరిగి err;
}

స్థిర పూర్ణాంకం fsck_resolve_undo(నిర్మాణం సూచీ_state *istate,
			     స్థిరరాశి అక్షరం *సూచీ_మార్గం)
{
	నిర్మాణం string_list_అంశం *అంశం;
	నిర్మాణం string_list *resolve_undo = istate->resolve_undo;

	ఒకవేళ (!resolve_undo)
		తిరిగి 0;

	for_each_string_list_అంశం(అంశం, resolve_undo) {
		స్థిరరాశి అక్షరం *మార్గం = అంశం->string;
		నిర్మాణం resolve_undo_info *ru = అంశం->util;
		పూర్ణాంకం i;

		ఒకవేళ (!ru)
			కొనసాగు;
		కొరకు (i = 0; i < 3; i++) {
			నిర్మాణం object *obj;

			ఒకవేళ (!ru->బహుళకం[i] || !S_ISREG(ru->బహుళకం[i]))
				కొనసాగు;

			obj = parse_object(istate->repo, &ru->oid[i]);
			ఒకవేళ (!obj) {
				error(_("%s: invalid sha1 pointer in resolve-undo of %s"),
				      oid_గమ్యం_hex(&ru->oid[i]),
				      సూచీ_మార్గం);
				errors_దొరికింది |= ERROR_REFS;
				కొనసాగు;
			}
			obj->flags |= వాడిన;
			fsck_put_object_name(&fsck_walk_options, &ru->oid[i],
					     ":(%d):%s", i, మార్గం);
			mark_object_reachable(obj);
		}
	}
	తిరిగి 0;
}

స్థిర శూన్య fsck_సూచీ(నిర్మాణం సూచీ_state *istate, స్థిరరాశి అక్షరం *సూచీ_మార్గం,
		       పూర్ణాంకం is_current_worktree)
{
	గుర్తులేని పూర్ణాంకం i;

	/* TODO: audit for interaction with sparse-index. */
	ensure_నిండిన_సూచీ(istate);
	కొరకు (i = 0; i < istate->cache_nr; i++) {
		గుర్తులేని పూర్ణాంకం బహుళకం;
		నిర్మాణం blob *blob;
		నిర్మాణం object *obj;

		బహుళకం = istate->cache[i]->ce_బహుళకం;
		ఒకవేళ (S_ISGITLINK(బహుళకం))
			కొనసాగు;
		blob = lookup_blob(istate->repo,
				   &istate->cache[i]->oid);
		ఒకవేళ (!blob)
			కొనసాగు;
		obj = &blob->object;
		obj->flags |= వాడిన;
		fsck_put_object_name(&fsck_walk_options, &obj->oid,
				     "%s:%s",
				     is_current_worktree ? "" : సూచీ_మార్గం,
				     istate->cache[i]->name);
		mark_object_reachable(obj);
	}
	ఒకవేళ (istate->cache_వృక్షం)
		fsck_cache_వృక్షం(istate->repo, istate->cache_వృక్షం, సూచీ_మార్గం);
	fsck_resolve_undo(istate, సూచీ_మార్గం);
}

స్థిర పూర్ణాంకం mark_object_for_connectivity(స్థిరరాశి నిర్మాణం object_id *oid,
					నిర్మాణం object_info *oi వాడని,
					శూన్య *cb_దత్తాంశం)
{
	నిర్మాణం repository *repo = cb_దత్తాంశం;
	నిర్మాణం object *obj = lookup_unknown_object(repo, oid);
	obj->flags |= HAS_OBJ;
	తిరిగి 0;
}

స్థిర శూన్య fsck_refs(నిర్మాణం repository *r)
{
	నిర్మాణం బిడ్డ_process refs_verify = బిడ్డ_PROCESS_INIT;
	నిర్మాణం పురోగతి *పురోగతి = ఖాళీ;

	ఒకవేళ (show_పురోగతి)
		పురోగతి = ప్రారంభం_పురోగతి(r, _("Checking ref database"), 1);

	ఒకవేళ (verbose)
		fprintf_ln(stderr, _("Checking ref database"));

	బిడ్డ_process_init(&refs_verify);
	refs_verify.git_cmd = 1;
	strvec_pushl(&refs_verify.వాదనలు, "refs", "verify", ఖాళీ);
	ఒకవేళ (verbose)
		strvec_నెట్టు(&refs_verify.వాదనలు, "--verbose");
	ఒకవేళ (check_strict)
		strvec_నెట్టు(&refs_verify.వాదనలు, "--strict");

	ఒకవేళ (నిరంతరం_command(&refs_verify))
		errors_దొరికింది |= ERROR_REFS;

	display_పురోగతి(పురోగతి, 1);
	stop_పురోగతి(&పురోగతి);
}

స్థిర అక్షరం స్థిరరాశి * స్థిరరాశి fsck_usage[] = {
	N_("git fsck [--tags] [--root] [--unreachable] [--cache] [--no-reflogs]\n"
	   "         [--[no-]full] [--strict] [--verbose] [--lost-found]\n"
	   "         [--[no-]dangling] [--[no-]progress] [--connectivity-only]\n"
	   "         [--[no-]name-objects] [--[no-]references] [<object>...]"),
	ఖాళీ
};

పూర్ణాంకం cmd_fsck(పూర్ణాంకం argc,
	     స్థిరరాశి అక్షరం **argv,
	     స్థిరరాశి అక్షరం *ఉపసర్గ,
	     నిర్మాణం repository *repo)
{
	నిర్మాణం odb_fsck_options odb_fsck_opts = {
		.flags = ODB_FSCK_నిండిన,
		.object_cb = fsck_obj_బఫరు,
		.object_payload = repo,
	};
	పూర్ణాంకం keep_cache_objects = 0;
	పూర్ణాంకం name_objects = 0;
	పూర్ణాంకం check_references = 1;
	నిర్మాణం option fsck_opts[] = {
		OPT__VERBOSE(&verbose, N_("be verbose")),
		OPT_BOOL(0, "unreachable", &show_unreachable, N_("show unreachable objects")),
		OPT_BOOL(0, "dangling", &show_dangling, N_("show dangling objects")),
		OPT_BOOL(0, "tags", &show_ట్యాగులు, N_("report tags")),
		OPT_BOOL(0, "root", &show_మూలం, N_("report root nodes")),
		OPT_BOOL(0, "cache", &keep_cache_objects, N_("make index objects head nodes")),
		OPT_BOOL(0, "reflogs", &include_reflogs, N_("make reflogs head nodes (default)")),
		OPT_బిట్టు(0, "full", &odb_fsck_opts.flags,
			N_("also consider packs and alternate objects"), ODB_FSCK_నిండిన),
		OPT_BOOL(0, "connectivity-only", &connectivity_only, N_("check only connectivity")),
		OPT_BOOL(0, "strict", &check_strict, N_("enable more strict checking")),
		OPT_BOOL(0, "lost-found", &write_కోల్పోయిన_and_దొరికింది,
					N_("write dangling objects in .git/lost-found")),
		OPT_BOOL(0, "progress", &show_పురోగతి, N_("show progress")),
		OPT_BOOL(0, "name-objects", &name_objects, N_("show verbose names for reachable objects")),
		OPT_BOOL(0, "references", &check_references, N_("check reference database consistency")),
		OPT_చివర(),
	};
	నిర్మాణం snapshot snap = {
		.nr = 0,
		.alloc = 0,
		.ref = ఖాళీ
	};

	errors_దొరికింది = 0;
	disable_replace_refs();
	save_commit_బఫరు = 0;

	argc = parse_options(argc, argv, ఉపసర్గ, fsck_opts, fsck_usage, 0);

	/* fsck knows how to handle missing promisor objects */
	repo->fetch_if_missing = 0;

	fsck_options_init(&fsck_walk_options, repo, FSCK_OPTIONS_DEFAULT);
	fsck_walk_options.walk = mark_object;

	fsck_options_init(&fsck_obj_options, repo, FSCK_OPTIONS_DEFAULT);
	fsck_obj_options.walk = mark_వాడిన;
	fsck_obj_options.error_func = fsck_objects_error_func;
	ఒకవేళ (check_strict)
		fsck_obj_options.strict = 1;

	ఒకవేళ (show_పురోగతి == -1)
		show_పురోగతి = isatty(2);
	ఒకవేళ (verbose) {
		show_పురోగతి = 0;
		odb_fsck_opts.flags |= ODB_FSCK_VERBOSE;
	}
	ఒకవేళ (show_పురోగతి)
		odb_fsck_opts.flags |= ODB_FSCK_పురోగతి;

	ఒకవేళ (write_కోల్పోయిన_and_దొరికింది) {
		odb_fsck_opts.flags |= ODB_FSCK_నిండిన;
		include_reflogs = 0;
	}

	ఒకవేళ (name_objects)
		fsck_enable_object_పేర్లు(&fsck_walk_options);

	repo_config(repo, git_fsck_config, &fsck_obj_options);
	prepare_repo_settings(repo);

	ఒకవేళ (check_references)
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

	ఒకవేళ (connectivity_only) {
		odb_for_each_object(repo->objects, ఖాళీ,
				    mark_object_for_connectivity, repo, 0);
	} లేకపోతే {
		ఒకవేళ (odb_fsck(repo->objects, &odb_fsck_opts) < 0)
			errors_దొరికింది |= ERROR_OBJECT;

		ఒకవేళ (fsck_ముగింపు(&fsck_obj_options))
			errors_దొరికింది |= ERROR_OBJECT;
	}

	/* Process the snapshotted refs and the reflogs. */
	process_refs(repo, &snap);

	/* If not given any explicit objects, process index files too. */
	ఒకవేళ (!argc)
		keep_cache_objects = 1;
	ఒకవేళ (keep_cache_objects) {
		/*
		 * TODO: Consider first walking these indexes in snapshot_refs,
		 * to snapshot where the index entries used to point, and then
		 * check those snapshotted locations here.
		 */
		నిర్మాణం worktree **worktrees, **p;

		verify_సూచీ_checksum = 1;
		verify_ce_క్రమం = 1;

		worktrees = get_worktrees(repo);
		కొరకు (p = worktrees; *p; p++) {
			నిర్మాణం worktree *wt = *p;
			నిర్మాణం సూచీ_state istate =
				సూచీ_STATE_INIT(repo);
			అక్షరం *మార్గం, *wt_gitdir;

			/*
			 * Make a copy since the buffer is reusable
			 * and may get overwritten by other calls
			 * while we're examining the index.
			 */
			మార్గం = xstrdup(worktree_git_మార్గం(wt, "index"));
			wt_gitdir = get_worktree_git_dir(wt);

			read_సూచీ_from(&istate, మార్గం, wt_gitdir);
			fsck_సూచీ(&istate, మార్గం, wt->is_current);

			discard_సూచీ(&istate);
			విడుదల(wt_gitdir);
			విడుదల(మార్గం);
		}
		free_worktrees(worktrees);
	}

	check_connectivity(repo);

	ఒకవేళ (repo->settings.core_commit_లేఖాచిత్రం) {
		నిర్మాణం odb_ఆధారం_files *files = odb_ఆధారం_files_downcast(repo->objects->ఆధారం);
		నిర్మాణం బిడ్డ_process commit_లేఖాచిత్రం_verify = బిడ్డ_PROCESS_INIT;

		కొరకు (నిర్మాణం odb_files_dir *dir = files->dirs; dir; dir = dir->తదుపరి) {
			బిడ్డ_process_init(&commit_లేఖాచిత్రం_verify);
			commit_లేఖాచిత్రం_verify.git_cmd = 1;
			strvec_pushl(&commit_లేఖాచిత్రం_verify.వాదనలు, "commit-graph",
				     "verify", "--object-dir", dir->abspath, ఖాళీ);
			ఒకవేళ (show_పురోగతి)
				strvec_నెట్టు(&commit_లేఖాచిత్రం_verify.వాదనలు, "--progress");
			లేకపోతే
				strvec_నెట్టు(&commit_లేఖాచిత్రం_verify.వాదనలు, "--no-progress");
			ఒకవేళ (నిరంతరం_command(&commit_లేఖాచిత్రం_verify))
				errors_దొరికింది |= ERROR_COMMIT_లేఖాచిత్రం;
		}
	}

	free_snapshot_refs(&snap);
	తిరిగి errors_దొరికింది;
}
