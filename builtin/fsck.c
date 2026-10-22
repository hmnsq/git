#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "hex.h"
#অন্তর্ভাব "config.h"
#অন্তর্ভাব "commit.h"
#অন্তর্ভাব "tree.h"
#অন্তর্ভাব "blob.h"
#অন্তর্ভাব "tag.h"
#অন্তর্ভাব "refs.h"
#অন্তর্ভাব "cache-tree.h"
#অন্তর্ভাব "fsck.h"
#অন্তর্ভাব "parse-options.h"
#অন্তর্ভাব "progress.h"
#অন্তর্ভাব "packfile.h"
#অন্তর্ভাব "object-name.h"
#অন্তর্ভাব "odb.h"
#অন্তর্ভাব "odb/streaming.h"
#অন্তর্ভাব "path.h"
#অন্তর্ভাব "read-cache-ll.h"
#অন্তর্ভাব "replace-object.h"
#অন্তর্ভাব "resolve-undo.h"
#অন্তর্ভাব "run-command.h"
#অন্তর্ভাব "sparse-index.h"
#অন্তর্ভাব "worktree.h"

#define REACHABLE 0x0001
#define SEEN      0x0002
#define HAS_OBJ   0x0004
/* This flag is set if something points to this object. */
#define USED      0x0008

স্থির পূর্ণসংখ্যা show_মূল;
স্থির পূর্ণসংখ্যা show_tags;
স্থির পূর্ণসংখ্যা show_unreachable;
স্থির পূর্ণসংখ্যা include_reflogs = 1;
স্থির পূর্ণসংখ্যা connectivity_only;
স্থির পূর্ণসংখ্যা check_strict;
স্থির কাঠামো fsck_options fsck_walk_options;
স্থির কাঠামো fsck_options fsck_obj_options;
স্থির পূর্ণসংখ্যা errors_found;
স্থির পূর্ণসংখ্যা write_lost_and_found;
স্থির পূর্ণসংখ্যা verbose;
স্থির পূর্ণসংখ্যা show_progress = -1;
স্থির পূর্ণসংখ্যা show_dangling = 1;
স্থির timestamp_t now;
#define ERROR_OBJECT 01
#define ERROR_REACHABLE 02
#define ERROR_REFS 010
#define ERROR_COMMIT_GRAPH 020

স্থির ধ্রুবক অক্ষর *describe_object(ধ্রুবক কাঠামো object_id *oid)
{
	ফেরত fsck_describe_object(&fsck_walk_options, oid);
}

স্থির ধ্রুবক অক্ষর *printable_type(কাঠামো repository *repo,
				  ধ্রুবক কাঠামো object_id *oid,
				  গণনা object_type type)
{
	ধ্রুবক অক্ষর *ret;

	যদি (type == OBJ_NONE)
		type = odb_read_object_info(repo->objects, oid, ফাঁকা);

	ret = type_name(type);
	যদি (!ret)
		ret = _("unknown");

	ফেরত ret;
}

স্থির পূর্ণসংখ্যা objerror(কাঠামো repository *repo, কাঠামো object *obj, ধ্রুবক অক্ষর *err)
{
	errors_found |= ERROR_OBJECT;
	/* TRANSLATORS: e.g. error in tree 01bfda: <more explanation> */
	fprintf_ln(stderr, _("error in %s %s: %s"),
		   printable_type(repo, &obj->oid, obj->type),
		   describe_object(&obj->oid), err);
	ফেরত -1;
}

স্থির পূর্ণসংখ্যা fsck_objects_error_func(কাঠামো fsck_options *o,
				   শূন্য *fsck_report,
				   গণনা fsck_বার্তা_type বার্তা_type,
				   গণনা fsck_বার্তা_id বার্তা_id UNUSED,
				   ধ্রুবক অক্ষর *message)
{
	কাঠামো fsck_object_report *report = fsck_report;
	ধ্রুবক কাঠামো object_id *oid = report->oid;
	গণনা object_type object_type = report->object_type;

	নির্বাচন (বার্তা_type) {
	ক্ষেত্র FSCK_WARN:
		/* TRANSLATORS: e.g. warning in tree 01bfda: <more explanation> */
		fprintf_ln(stderr, _("warning in %s %s: %s"),
			   printable_type(o->repo, oid, object_type),
			   describe_object(oid), message);
		ফেরত 0;
	ক্ষেত্র FSCK_ERROR:
		/* TRANSLATORS: e.g. error in tree 01bfda: <more explanation> */
		fprintf_ln(stderr, _("error in %s %s: %s"),
			   printable_type(o->repo, oid, object_type),
			   describe_object(oid), message);
		ফেরত 1;
	পূর্বনির্ধারিত:
		BUG("%d (FSCK_IGNORE?) should never trigger this callback",
		    বার্তা_type);
	}
}

স্থির কাঠামো object_array pending;

স্থির পূর্ণসংখ্যা mark_object(কাঠামো object *obj, গণনা object_type type,
		       শূন্য *তথ্য, কাঠামো fsck_options *options)
{
	কাঠামো object *parent = তথ্য;

	/*
	 * The only case data is NULL or type is OBJ_ANY is when
	 * mark_object_reachable() calls us.  All the callers of
	 * that function has non-NULL obj hence ...
	 */
	যদি (!obj) {
		/* ... these references to parent->fld are safe here */
		printf_ln(_("broken link from %7s %s"),
			  printable_type(options->repo, &parent->oid, parent->type),
			  describe_object(&parent->oid));
		printf_ln(_("broken link from %7s %s"),
			  (type == OBJ_ANY ? _("unknown") : type_name(type)),
			  _("unknown"));
		errors_found |= ERROR_REACHABLE;
		ফেরত 1;
	}

	যদি (type != OBJ_ANY && obj->type != type)
		/* ... and the reference to parent is safe here */
		objerror(options->repo, parent, _("wrong object type in link"));

	যদি (obj->flags & REACHABLE)
		ফেরত 0;
	obj->flags |= REACHABLE;

	যদি (is_promisor_object(options->repo, &obj->oid))
		/*
		 * Further recursion does not need to be performed on this
		 * object since it is a promisor object (so it does not need to
		 * be added to "pending").
		 */
		ফেরত 0;

	যদি (!(obj->flags & HAS_OBJ)) {
		যদি (parent && !odb_has_object(options->repo->objects, &obj->oid,
					      ODB_HAS_OBJECT_RECHECK_PACKED)) {
			printf_ln(_("broken link from %7s %s\n"
				    "              to %7s %s"),
				  printable_type(options->repo, &parent->oid, parent->type),
				  describe_object(&parent->oid),
				  printable_type(options->repo, &obj->oid, obj->type),
				  describe_object(&obj->oid));
			errors_found |= ERROR_REACHABLE;
		}
		ফেরত 1;
	}

	add_object_array(obj, ফাঁকা, &pending);
	ফেরত 0;
}

স্থির শূন্য mark_object_reachable(কাঠামো object *obj)
{
	mark_object(obj, OBJ_ANY, ফাঁকা, &fsck_walk_options);
}

স্থির পূর্ণসংখ্যা traverse_one_object(কাঠামো object *obj)
{
	পূর্ণসংখ্যা ফলাফল = fsck_walk(obj, obj, &fsck_walk_options);

	যদি (obj->type == OBJ_বৃক্ষ) {
		কাঠামো বৃক্ষ *বৃক্ষ = (কাঠামো বৃক্ষ *)obj;
		free_বৃক্ষ_বাফার(বৃক্ষ);
	}
	ফেরত ফলাফল;
}

স্থির পূর্ণসংখ্যা traverse_reachable(কাঠামো repository *repo)
{
	কাঠামো progress *progress = ফাঁকা;
	unsigned পূর্ণসংখ্যা nr = 0;
	পূর্ণসংখ্যা ফলাফল = 0;
	যদি (show_progress)
		progress = start_delayed_progress(repo,
						  _("Checking connectivity"), 0);
	যখন (pending.nr) {
		ফলাফল |= traverse_one_object(object_array_বের_করো(&pending));
		display_progress(progress, ++nr);
	}
	stop_progress(&progress);
	ফেরত !!ফলাফল;
}

স্থির পূর্ণসংখ্যা mark_used(কাঠামো object *obj, গণনা object_type type UNUSED,
		     শূন্য *তথ্য UNUSED, কাঠামো fsck_options *options UNUSED)
{
	যদি (!obj)
		ফেরত 1;
	obj->flags |= USED;
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা mark_unreachable_referents(ধ্রুবক কাঠামো object_id *oid,
				      কাঠামো object_info *oi UNUSED,
				      শূন্য *তথ্য)
{
	কাঠামো repository *repo = তথ্য;
	কাঠামো fsck_options options;
	কাঠামো object *obj = lookup_object(তথ্য, oid);

	যদি (!obj || !(obj->flags & HAS_OBJ))
		ফেরত 0; /* not part of our original set */
	যদি (obj->flags & REACHABLE)
		ফেরত 0; /* reachable objects already traversed */

	/*
	 * Avoid passing OBJ_NONE to fsck_walk, which will parse the object
	 * (and we want to avoid parsing blobs).
	 */
	যদি (obj->type == OBJ_NONE) {
		গণনা object_type type = odb_read_object_info(repo->objects,
							     &obj->oid, ফাঁকা);
		যদি (type > 0)
			object_as_type(obj, type, 0);
	}

	fsck_options_init(&options, repo, FSCK_OPTIONS_DEFAULT);
	options.walk = mark_used;
	fsck_walk(obj, ফাঁকা, &options);
	যদি (obj->type == OBJ_বৃক্ষ)
		free_বৃক্ষ_বাফার((কাঠামো বৃক্ষ *)obj);

	ফেরত 0;
}

/*
 * Check a single reachable object
 */
স্থির শূন্য check_reachable_object(কাঠামো repository *repo, কাঠামো object *obj)
{
	/*
	 * We obviously want the object to be parsed,
	 * except if it was in a pack-file and we didn't
	 * do a full fsck
	 */
	যদি (!(obj->flags & HAS_OBJ)) {
		যদি (is_promisor_object(repo, &obj->oid))
			ফেরত;
		যদি (has_object_pack(repo, &obj->oid))
			ফেরত; /* it is in pack - forget about it */
		printf_ln(_("missing %s %s"),
			  printable_type(repo, &obj->oid, obj->type),
			  describe_object(&obj->oid));
		errors_found |= ERROR_REACHABLE;
		ফেরত;
	}
}

/*
 * Check a single unreachable object
 */
স্থির শূন্য check_unreachable_object(কাঠামো repository *repo, কাঠামো object *obj)
{
	/*
	 * Missing unreachable object? Ignore it. It's not like
	 * we miss it (since it can't be reached), nor do we want
	 * to complain about it being unreachable (since it does
	 * not exist).
	 */
	যদি (!(obj->flags & HAS_OBJ))
		ফেরত;

	/*
	 * Unreachable object that exists? Show it if asked to,
	 * since this is something that is prunable.
	 */
	যদি (show_unreachable) {
		printf_ln(_("unreachable %s %s"),
			  printable_type(repo, &obj->oid, obj->type),
			  describe_object(&obj->oid));
		ফেরত;
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
	যদি (!(obj->flags & USED)) {
		যদি (show_dangling)
			printf_ln(_("dangling %s %s"),
				  printable_type(repo, &obj->oid, obj->type),
				  describe_object(&obj->oid));
		যদি (write_lost_and_found) {
			অক্ষর *filename = repo_git_path(repo, "lost-found/%s/%s",
				obj->type == OBJ_COMMIT ? "commit" : "other",
				describe_object(&obj->oid));
			FILE *f;

			যদি (safe_create_leading_directories_const(repo, filename)) {
				error(_("could not create lost-found"));
				মুক্ত(filename);
				ফেরত;
			}
			f = xfopen(filename, "w");
			যদি (obj->type == OBJ_BLOB) {
				যদি (odb_stream_blob_to_fd(repo->objects, fileno(f),
							  &obj->oid, ফাঁকা, 1))
					die_errno(_("could not write '%s'"), filename);
			} অন্যথা
				fprintf(f, "%s\n", describe_object(&obj->oid));
			যদি (fclose(f))
				die_errno(_("could not finish '%s'"),
					  filename);
			মুক্ত(filename);
		}
		ফেরত;
	}

	/*
	 * Otherwise? It's there, it's unreachable, and some other unreachable
	 * object points to it. Ignore it - it's not interesting, and we showed
	 * all the interesting cases above.
	 */
}

স্থির শূন্য check_object(কাঠামো repository *repo, কাঠামো object *obj)
{
	যদি (verbose)
		fprintf_ln(stderr, _("Checking %s"), describe_object(&obj->oid));

	যদি (obj->flags & REACHABLE)
		check_reachable_object(repo, obj);
	অন্যথা
		check_unreachable_object(repo, obj);
}

স্থির শূন্য check_connectivity(কাঠামো repository *repo)
{
	পূর্ণসংখ্যা i, max;

	/* Traverse the pending reachable objects */
	traverse_reachable(repo);

	/*
	 * With --connectivity-only, we won't have actually opened and marked
	 * unreachable objects with USED. Do that now to make --dangling, etc
	 * accurate.
	 */
	যদি (connectivity_only && (show_dangling || write_lost_and_found)) {
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
		odb_for_each_object(repo->objects, ফাঁকা,
				    mark_unreachable_referents, repo, 0);
	}

	/* Look up all the requirements, warn about missing objects.. */
	max = get_max_object_সূচক(repo);
	যদি (verbose)
		fprintf_ln(stderr, _("Checking connectivity (%d objects)"), max);

	জন্য (i = 0; i < max; i++) {
		কাঠামো object *obj = get_indexed_object(repo, i);

		যদি (obj)
			check_object(repo, obj);
	}
}

স্থির পূর্ণসংখ্যা fsck_obj_বাফার(ধ্রুবক কাঠামো object_id *oid, গণনা object_type type,
			   unsigned long আকার_2, শূন্য *বাফার, পূর্ণসংখ্যা *eaten, শূন্য *cb_তথ্য)
{
	কাঠামো repository *repo = cb_তথ্য;
	কাঠামো object *obj;
	পূর্ণসংখ্যা err;

	/*
	 * Note, buffer may be NULL if type is OBJ_BLOB. See
	 * verify_packfile(), data_valid variable for details.
	 */
	obj = parse_object_বাফার(repo, oid, type, আকার_2, বাফার, eaten);
	যদি (!obj) {
		errors_found |= ERROR_OBJECT;
		err = error(_("%s: object corrupt or missing"),
			    oid_to_hex(oid));
		যাও out;
	}

	obj->flags &= ~REACHABLE;
	obj->flags |= HAS_OBJ | SEEN;

	যদি (verbose)
		fprintf_ln(stderr, _("Checking %s %s"),
			   printable_type(repo, &obj->oid, obj->type),
			   describe_object(&obj->oid));

	যদি (fsck_walk(obj, ফাঁকা, &fsck_obj_options))
		objerror(repo, obj, _("broken links"));

	err = fsck_object(obj, বাফার, আকার_2, &fsck_obj_options);
	যদি (err)
		যাও out;

	যদি (obj->type == OBJ_COMMIT) {
		কাঠামো commit *commit = (কাঠামো commit *) obj;

		যদি (!commit->parents && show_মূল)
			printf_ln(_("root %s"),
				  describe_object(&commit->object.oid));
	}

	যদি (obj->type == OBJ_TAG) {
		কাঠামো tag *tag = (কাঠামো tag *) obj;

		যদি (show_tags && tag->tagged) {
			printf_ln(_("tagged %s %s (%s) in %s"),
				  printable_type(repo, &tag->tagged->oid, tag->tagged->type),
				  describe_object(&tag->tagged->oid),
				  tag->tag,
				  describe_object(&tag->object.oid));
		}
	}

out:
	যদি (obj && obj->type == OBJ_বৃক্ষ)
		free_বৃক্ষ_বাফার((কাঠামো বৃক্ষ *)obj);
	ফেরত err;
}

স্থির পূর্ণসংখ্যা default_refs;

স্থির শূন্য fsck_handle_reflog_oid(কাঠামো repository *repo,
				   ধ্রুবক অক্ষর *refname, কাঠামো object_id *oid,
				   timestamp_t timestamp)
{
	কাঠামো object *obj;

	যদি (!is_null_oid(oid)) {
		obj = lookup_object(repo, oid);
		যদি (obj && (obj->flags & HAS_OBJ)) {
			যদি (timestamp)
				fsck_put_object_name(&fsck_walk_options, oid,
						     "%s@{%"PRItime"}",
						     refname, timestamp);
			obj->flags |= USED;
			mark_object_reachable(obj);
		} অন্যথা যদি (!is_promisor_object(repo, oid)) {
			error(_("%s: invalid reflog entry %s"),
			      refname, oid_to_hex(oid));
			errors_found |= ERROR_REACHABLE;
		}
	}
}

স্থির পূর্ণসংখ্যা fsck_handle_reflog_ent(ধ্রুবক অক্ষর *refname,
				  কাঠামো object_id *ooid, কাঠামো object_id *noid,
				  ধ্রুবক অক্ষর *email UNUSED,
				  timestamp_t timestamp, পূর্ণসংখ্যা tz UNUSED,
				  ধ্রুবক অক্ষর *message UNUSED, শূন্য *cb_তথ্য)
{
	কাঠামো repository *repo = cb_তথ্য;

	যদি (now && timestamp > now)
		ফেরত 0;

	যদি (verbose)
		fprintf_ln(stderr, _("Checking reflog %s->%s"),
			   oid_to_hex(ooid), oid_to_hex(noid));

	fsck_handle_reflog_oid(repo, refname, ooid, 0);
	fsck_handle_reflog_oid(repo, refname, noid, timestamp);
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা fsck_handle_reflog(ধ্রুবক অক্ষর *logname, শূন্য *cb_তথ্য)
{
	কাঠামো strbuf refname = STRBUF_INIT;
	কাঠামো worktree *wt = cb_তথ্য;

	strbuf_worktree_ref(wt, &refname, logname);
	refs_for_each_reflog_ent(get_main_ref_store(wt->repo),
				 refname.buf, fsck_handle_reflog_ent,
				 wt->repo);
	strbuf_release(&refname);
	ফেরত 0;
}

কাঠামো ref_snapshot {
	অক্ষর *refname;
	কাঠামো object_id oid;
	/* TODO: Maybe supplement with latest reflog entry info too? */
};

কাঠামো snapshot {
	আকার_t nr;
	আকার_t alloc;
	কাঠামো ref_snapshot *ref;
	/* TODO: Consider also snapshotting the index of each worktree. */
};

কাঠামো snapshot_ref_তথ্য {
	কাঠামো repository *repo;
	কাঠামো snapshot *snap;
};

স্থির পূর্ণসংখ্যা snapshot_ref(ধ্রুবক কাঠামো reference *ref, শূন্য *cb_তথ্য)
{
	কাঠামো snapshot_ref_তথ্য *তথ্য = cb_তথ্য;
	কাঠামো snapshot *snap = তথ্য->snap;
	কাঠামো object *obj;

	obj = parse_object(তথ্য->repo, ref->oid);
	যদি (!obj) {
		যদি (is_promisor_object(তথ্য->repo, ref->oid)) {
			/*
			 * Increment default_refs anyway, because this is a
			 * valid ref.
			 */
			 default_refs++;
			 ফেরত 0;
		}
		error(_("%s: invalid sha1 pointer %s"),
		      ref->name, oid_to_hex(ref->oid));
		errors_found |= ERROR_REACHABLE;
		/* We'll continue with the rest despite the error.. */
		ফেরত 0;
	}
	যদি (obj->type != OBJ_COMMIT && is_branch(ref->name)) {
		error(_("%s: not a commit"), ref->name);
		errors_found |= ERROR_REFS;
	}
	default_refs++;

	ALLOC_GROW(snap->ref, snap->nr + 1, snap->alloc);
	snap->ref[snap->nr].refname = xstrdup(ref->name);
	oidcpy(&snap->ref[snap->nr].oid, ref->oid);
	snap->nr++;

	ফেরত 0;
}

স্থির পূর্ণসংখ্যা fsck_handle_ref(ধ্রুবক কাঠামো reference *ref, শূন্য *cb_তথ্য)
{
	কাঠামো repository *repo = cb_তথ্য;
	কাঠামো object *obj;

	obj = parse_object(repo, ref->oid);
	obj->flags |= USED;
	fsck_put_object_name(&fsck_walk_options,
			     ref->oid, "%s", ref->name);
	mark_object_reachable(obj);

	ফেরত 0;
}

স্থির শূন্য snapshot_refs(কাঠামো repository *repo,
			  কাঠামো snapshot *snap, পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv)
{
	কাঠামো refs_for_each_ref_options opts = {
		.flags = REFS_FOR_EACH_INCLUDE_BROKEN,
	};
	কাঠামো snapshot_ref_তথ্য তথ্য = {
		.repo = repo,
		.snap = snap,
	};
	কাঠামো worktree **worktrees, **p;
	ধ্রুবক অক্ষর *শীর্ষ_points_at;
	কাঠামো object_id শীর্ষ_oid;

	জন্য (পূর্ণসংখ্যা i = 0; i < argc; i++) {
		ধ্রুবক অক্ষর *arg = argv[i];
		কাঠামো object_id oid;
		যদি (!repo_get_oid(repo, arg, &oid)) {
			কাঠামো reference ref = {
				.name = arg,
				.oid = &oid,
			};

			snapshot_ref(&ref, &তথ্য);
			চালু;
		}
		error(_("invalid parameter: expected sha1, got '%s'"), arg);
		errors_found |= ERROR_OBJECT;
	}

	যদি (argc) {
		include_reflogs = 0;
		ফেরত;
	}

	refs_for_each_ref_ext(get_main_ref_store(repo),
			      snapshot_ref, &তথ্য, &opts);

	worktrees = get_worktrees(repo);
	জন্য (p = worktrees; *p; p++) {
		কাঠামো worktree *wt = *p;
		কাঠামো strbuf refname = STRBUF_INIT;

		strbuf_worktree_ref(wt, &refname, "HEAD");

		শীর্ষ_points_at = refs_resolve_ref_unsafe(get_main_ref_store(repo),
							 refname.buf, 0, &শীর্ষ_oid, ফাঁকা);

		যদি (শীর্ষ_points_at && !is_null_oid(&শীর্ষ_oid)) {
			কাঠামো reference ref = {
				.name = refname.buf,
				.oid = &শীর্ষ_oid,
			};

			snapshot_ref(&ref, &তথ্য);
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
	now = time(ফাঁকা);
}


স্থির শূন্য free_snapshot_refs(কাঠামো snapshot *snap)
{
	জন্য (আকার_t i = 0; i < snap->nr; i++)
		মুক্ত(snap->ref[i].refname);
	মুক্ত(snap->ref);
}

স্থির শূন্য process_refs(কাঠামো repository *repo, কাঠামো snapshot *snap)
{
	কাঠামো worktree **worktrees, **p;

	জন্য (আকার_t i = 0; i < snap->nr; i++) {
		কাঠামো reference ref = {
			.name = snap->ref[i].refname,
			.oid = &snap->ref[i].oid,
		};
		fsck_handle_ref(&ref, repo);
	}

	যদি (include_reflogs) {
		worktrees = get_worktrees(repo);
		জন্য (p = worktrees; *p; p++) {
			কাঠামো worktree *wt = *p;

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
	যদি (!default_refs) {
		fprintf_ln(stderr, _("notice: No default references"));
		show_unreachable = 0;
	}
}

স্থির পূর্ণসংখ্যা fsck_cache_বৃক্ষ(কাঠামো repository *repo, কাঠামো cache_বৃক্ষ *it, ধ্রুবক অক্ষর *সূচক_path)
{
	পূর্ণসংখ্যা i;
	পূর্ণসংখ্যা err = 0;

	যদি (verbose)
		fprintf_ln(stderr, _("Checking cache tree of %s"), সূচক_path);

	যদি (0 <= it->entry_গুনতি) {
		কাঠামো object *obj = parse_object(repo, &it->oid);
		যদি (!obj) {
			error(_("%s: invalid sha1 pointer in cache-tree of %s"),
			      oid_to_hex(&it->oid), সূচক_path);
			errors_found |= ERROR_REFS;
			ফেরত 1;
		}
		obj->flags |= USED;
		fsck_put_object_name(&fsck_walk_options, &it->oid, ":");
		mark_object_reachable(obj);
		যদি (obj->type != OBJ_বৃক্ষ)
			err |= objerror(repo, obj, _("non-tree in cache-tree"));
	}
	জন্য (i = 0; i < it->subtree_nr; i++)
		err |= fsck_cache_বৃক্ষ(repo, it->down[i]->cache_বৃক্ষ, সূচক_path);
	ফেরত err;
}

স্থির পূর্ণসংখ্যা fsck_resolve_undo(কাঠামো সূচক_state *istate,
			     ধ্রুবক অক্ষর *সূচক_path)
{
	কাঠামো string_list_item *item;
	কাঠামো string_list *resolve_undo = istate->resolve_undo;

	যদি (!resolve_undo)
		ফেরত 0;

	for_each_string_list_item(item, resolve_undo) {
		ধ্রুবক অক্ষর *path = item->string;
		কাঠামো resolve_undo_info *ru = item->util;
		পূর্ণসংখ্যা i;

		যদি (!ru)
			চালু;
		জন্য (i = 0; i < 3; i++) {
			কাঠামো object *obj;

			যদি (!ru->mode[i] || !S_ISREG(ru->mode[i]))
				চালু;

			obj = parse_object(istate->repo, &ru->oid[i]);
			যদি (!obj) {
				error(_("%s: invalid sha1 pointer in resolve-undo of %s"),
				      oid_to_hex(&ru->oid[i]),
				      সূচক_path);
				errors_found |= ERROR_REFS;
				চালু;
			}
			obj->flags |= USED;
			fsck_put_object_name(&fsck_walk_options, &ru->oid[i],
					     ":(%d):%s", i, path);
			mark_object_reachable(obj);
		}
	}
	ফেরত 0;
}

স্থির শূন্য fsck_সূচক(কাঠামো সূচক_state *istate, ধ্রুবক অক্ষর *সূচক_path,
		       পূর্ণসংখ্যা is_current_worktree)
{
	unsigned পূর্ণসংখ্যা i;

	/* TODO: audit for interaction with sparse-index. */
	ensure_full_সূচক(istate);
	জন্য (i = 0; i < istate->cache_nr; i++) {
		unsigned পূর্ণসংখ্যা mode;
		কাঠামো blob *blob;
		কাঠামো object *obj;

		mode = istate->cache[i]->ce_mode;
		যদি (S_ISGITLINK(mode))
			চালু;
		blob = lookup_blob(istate->repo,
				   &istate->cache[i]->oid);
		যদি (!blob)
			চালু;
		obj = &blob->object;
		obj->flags |= USED;
		fsck_put_object_name(&fsck_walk_options, &obj->oid,
				     "%s:%s",
				     is_current_worktree ? "" : সূচক_path,
				     istate->cache[i]->name);
		mark_object_reachable(obj);
	}
	যদি (istate->cache_বৃক্ষ)
		fsck_cache_বৃক্ষ(istate->repo, istate->cache_বৃক্ষ, সূচক_path);
	fsck_resolve_undo(istate, সূচক_path);
}

স্থির পূর্ণসংখ্যা mark_object_for_connectivity(ধ্রুবক কাঠামো object_id *oid,
					কাঠামো object_info *oi UNUSED,
					শূন্য *cb_তথ্য)
{
	কাঠামো repository *repo = cb_তথ্য;
	কাঠামো object *obj = lookup_unknown_object(repo, oid);
	obj->flags |= HAS_OBJ;
	ফেরত 0;
}

স্থির শূন্য fsck_refs(কাঠামো repository *r)
{
	কাঠামো child_process refs_verify = CHILD_PROCESS_INIT;
	কাঠামো progress *progress = ফাঁকা;

	যদি (show_progress)
		progress = start_progress(r, _("Checking ref database"), 1);

	যদি (verbose)
		fprintf_ln(stderr, _("Checking ref database"));

	child_process_init(&refs_verify);
	refs_verify.git_cmd = 1;
	strvec_pushl(&refs_verify.args, "refs", "verify", ফাঁকা);
	যদি (verbose)
		strvec_ঠেলো(&refs_verify.args, "--verbose");
	যদি (check_strict)
		strvec_ঠেলো(&refs_verify.args, "--strict");

	যদি (run_command(&refs_verify))
		errors_found |= ERROR_REFS;

	display_progress(progress, 1);
	stop_progress(&progress);
}

স্থির অক্ষর ধ্রুবক * ধ্রুবক fsck_usage[] = {
	N_("git fsck [--tags] [--root] [--unreachable] [--cache] [--no-reflogs]\n"
	   "         [--[no-]full] [--strict] [--verbose] [--lost-found]\n"
	   "         [--[no-]dangling] [--[no-]progress] [--connectivity-only]\n"
	   "         [--[no-]name-objects] [--[no-]references] [<object>...]"),
	ফাঁকা
};

পূর্ণসংখ্যা cmd_fsck(পূর্ণসংখ্যা argc,
	     ধ্রুবক অক্ষর **argv,
	     ধ্রুবক অক্ষর *prefix,
	     কাঠামো repository *repo)
{
	কাঠামো odb_fsck_options odb_fsck_opts = {
		.flags = ODB_FSCK_FULL,
		.object_cb = fsck_obj_বাফার,
		.object_payload = repo,
	};
	পূর্ণসংখ্যা keep_cache_objects = 0;
	পূর্ণসংখ্যা name_objects = 0;
	পূর্ণসংখ্যা check_references = 1;
	কাঠামো option fsck_opts[] = {
		OPT__VERBOSE(&verbose, N_("be verbose")),
		OPT_BOOL(0, "unreachable", &show_unreachable, N_("show unreachable objects")),
		OPT_BOOL(0, "dangling", &show_dangling, N_("show dangling objects")),
		OPT_BOOL(0, "tags", &show_tags, N_("report tags")),
		OPT_BOOL(0, "root", &show_মূল, N_("report root nodes")),
		OPT_BOOL(0, "cache", &keep_cache_objects, N_("make index objects head nodes")),
		OPT_BOOL(0, "reflogs", &include_reflogs, N_("make reflogs head nodes (default)")),
		OPT_BIT(0, "full", &odb_fsck_opts.flags,
			N_("also consider packs and alternate objects"), ODB_FSCK_FULL),
		OPT_BOOL(0, "connectivity-only", &connectivity_only, N_("check only connectivity")),
		OPT_BOOL(0, "strict", &check_strict, N_("enable more strict checking")),
		OPT_BOOL(0, "lost-found", &write_lost_and_found,
					N_("write dangling objects in .git/lost-found")),
		OPT_BOOL(0, "progress", &show_progress, N_("show progress")),
		OPT_BOOL(0, "name-objects", &name_objects, N_("show verbose names for reachable objects")),
		OPT_BOOL(0, "references", &check_references, N_("check reference database consistency")),
		OPT_END(),
	};
	কাঠামো snapshot snap = {
		.nr = 0,
		.alloc = 0,
		.ref = ফাঁকা
	};

	errors_found = 0;
	disable_replace_refs();
	save_commit_বাফার = 0;

	argc = parse_options(argc, argv, prefix, fsck_opts, fsck_usage, 0);

	/* fsck knows how to handle missing promisor objects */
	repo->fetch_if_missing = 0;

	fsck_options_init(&fsck_walk_options, repo, FSCK_OPTIONS_DEFAULT);
	fsck_walk_options.walk = mark_object;

	fsck_options_init(&fsck_obj_options, repo, FSCK_OPTIONS_DEFAULT);
	fsck_obj_options.walk = mark_used;
	fsck_obj_options.error_func = fsck_objects_error_func;
	যদি (check_strict)
		fsck_obj_options.strict = 1;

	যদি (show_progress == -1)
		show_progress = isatty(2);
	যদি (verbose) {
		show_progress = 0;
		odb_fsck_opts.flags |= ODB_FSCK_VERBOSE;
	}
	যদি (show_progress)
		odb_fsck_opts.flags |= ODB_FSCK_PROGRESS;

	যদি (write_lost_and_found) {
		odb_fsck_opts.flags |= ODB_FSCK_FULL;
		include_reflogs = 0;
	}

	যদি (name_objects)
		fsck_enable_object_names(&fsck_walk_options);

	repo_config(repo, git_fsck_config, &fsck_obj_options);
	prepare_repo_settings(repo);

	যদি (check_references)
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

	যদি (connectivity_only) {
		odb_for_each_object(repo->objects, ফাঁকা,
				    mark_object_for_connectivity, repo, 0);
	} অন্যথা {
		যদি (odb_fsck(repo->objects, &odb_fsck_opts) < 0)
			errors_found |= ERROR_OBJECT;

		যদি (fsck_finish(&fsck_obj_options))
			errors_found |= ERROR_OBJECT;
	}

	/* Process the snapshotted refs and the reflogs. */
	process_refs(repo, &snap);

	/* If not given any explicit objects, process index files too. */
	যদি (!argc)
		keep_cache_objects = 1;
	যদি (keep_cache_objects) {
		/*
		 * TODO: Consider first walking these indexes in snapshot_refs,
		 * to snapshot where the index entries used to point, and then
		 * check those snapshotted locations here.
		 */
		কাঠামো worktree **worktrees, **p;

		verify_সূচক_checksum = 1;
		verify_ce_order = 1;

		worktrees = get_worktrees(repo);
		জন্য (p = worktrees; *p; p++) {
			কাঠামো worktree *wt = *p;
			কাঠামো সূচক_state istate =
				সূচক_STATE_INIT(repo);
			অক্ষর *path, *wt_gitdir;

			/*
			 * Make a copy since the buffer is reusable
			 * and may get overwritten by other calls
			 * while we're examining the index.
			 */
			path = xstrdup(worktree_git_path(wt, "index"));
			wt_gitdir = get_worktree_git_dir(wt);

			read_সূচক_from(&istate, path, wt_gitdir);
			fsck_সূচক(&istate, path, wt->is_current);

			discard_সূচক(&istate);
			মুক্ত(wt_gitdir);
			মুক্ত(path);
		}
		free_worktrees(worktrees);
	}

	check_connectivity(repo);

	যদি (repo->settings.core_commit_graph) {
		কাঠামো odb_source_files *files = odb_source_files_downcast(repo->objects->source);
		কাঠামো child_process commit_graph_verify = CHILD_PROCESS_INIT;

		জন্য (কাঠামো odb_files_dir *dir = files->dirs; dir; dir = dir->পরবর্তী) {
			child_process_init(&commit_graph_verify);
			commit_graph_verify.git_cmd = 1;
			strvec_pushl(&commit_graph_verify.args, "commit-graph",
				     "verify", "--object-dir", dir->abspath, ফাঁকা);
			যদি (show_progress)
				strvec_ঠেলো(&commit_graph_verify.args, "--progress");
			অন্যথা
				strvec_ঠেলো(&commit_graph_verify.args, "--no-progress");
			যদি (run_command(&commit_graph_verify))
				errors_found |= ERROR_COMMIT_GRAPH;
		}
	}

	free_snapshot_refs(&snap);
	ফেরত errors_found;
}
