#অন্তর্ভাৱ "builtin.h"
#অন্তর্ভাৱ "gettext.h"
#অন্তর্ভাৱ "hex.h"
#অন্তর্ভাৱ "config.h"
#অন্তর্ভাৱ "commit.h"
#অন্তর্ভাৱ "tree.h"
#অন্তর্ভাৱ "blob.h"
#অন্তর্ভাৱ "tag.h"
#অন্তর্ভাৱ "refs.h"
#অন্তর্ভাৱ "cache-tree.h"
#অন্তর্ভাৱ "fsck.h"
#অন্তর্ভাৱ "parse-options.h"
#অন্তর্ভাৱ "progress.h"
#অন্তর্ভাৱ "packfile.h"
#অন্তর্ভাৱ "object-name.h"
#অন্তর্ভাৱ "odb.h"
#অন্তর্ভাৱ "odb/streaming.h"
#অন্তর্ভাৱ "path.h"
#অন্তর্ভাৱ "read-cache-ll.h"
#অন্তর্ভাৱ "replace-object.h"
#অন্তর্ভাৱ "resolve-undo.h"
#অন্তর্ভাৱ "run-command.h"
#অন্তর্ভাৱ "sparse-index.h"
#অন্তর্ভাৱ "worktree.h"

#define REACHABLE 0x0001
#define SEEN      0x0002
#define HAS_OBJ   0x0004
/* This flag is set if something points to this object. */
#define USED      0x0008

স্থির পূর্ণাঙ্ক show_মূল;
স্থির পূর্ণাঙ্ক show_tags;
স্থির পূর্ণাঙ্ক show_unreachable;
স্থির পূর্ণাঙ্ক include_reflogs = 1;
স্থির পূর্ণাঙ্ক connectivity_only;
স্থির পূর্ণাঙ্ক check_strict;
স্থির সংরচনা fsck_options fsck_walk_options;
স্থির সংরচনা fsck_options fsck_obj_options;
স্থির পূর্ণাঙ্ক errors_found;
স্থির পূর্ণাঙ্ক write_lost_and_found;
স্থির পূর্ণাঙ্ক verbose;
স্থির পূর্ণাঙ্ক show_progress = -1;
স্থির পূর্ণাঙ্ক show_dangling = 1;
স্থির timestamp_t now;
#define ERROR_OBJECT 01
#define ERROR_REACHABLE 02
#define ERROR_REFS 010
#define ERROR_COMMIT_GRAPH 020

স্থির ধ্রুৱ ৱর্ণ *describe_object(ধ্রুৱ সংরচনা object_id *oid)
{
	ঘূৰাই fsck_describe_object(&fsck_walk_options, oid);
}

স্থির ধ্রুৱ ৱর্ণ *printable_type(সংরচনা repository *repo,
				  ধ্রুৱ সংরচনা object_id *oid,
				  গণনা object_type type)
{
	ধ্রুৱ ৱর্ণ *ret;

	যদি (type == OBJ_NONE)
		type = odb_read_object_info(repo->objects, oid, রিক্ত);

	ret = type_name(type);
	যদি (!ret)
		ret = _("unknown");

	ঘূৰাই ret;
}

স্থির পূর্ণাঙ্ক objerror(সংরচনা repository *repo, সংরচনা object *obj, ধ্রুৱ ৱর্ণ *err)
{
	errors_found |= ERROR_OBJECT;
	/* TRANSLATORS: e.g. error in tree 01bfda: <more explanation> */
	fprintf_ln(stderr, _("error in %s %s: %s"),
		   printable_type(repo, &obj->oid, obj->type),
		   describe_object(&obj->oid), err);
	ঘূৰাই -1;
}

স্থির পূর্ণাঙ্ক fsck_objects_error_func(সংরচনা fsck_options *o,
				   শূন্য *fsck_report,
				   গণনা fsck_বাৰ্তা_type বাৰ্তা_type,
				   গণনা fsck_বাৰ্তা_id বাৰ্তা_id UNUSED,
				   ধ্রুৱ ৱর্ণ *message)
{
	সংরচনা fsck_object_report *report = fsck_report;
	ধ্রুৱ সংরচনা object_id *oid = report->oid;
	গণনা object_type object_type = report->object_type;

	ৱিকল্প (বাৰ্তা_type) {
	পক্ষ FSCK_WARN:
		/* TRANSLATORS: e.g. warning in tree 01bfda: <more explanation> */
		fprintf_ln(stderr, _("warning in %s %s: %s"),
			   printable_type(o->repo, oid, object_type),
			   describe_object(oid), message);
		ঘূৰাই 0;
	পক্ষ FSCK_ERROR:
		/* TRANSLATORS: e.g. error in tree 01bfda: <more explanation> */
		fprintf_ln(stderr, _("error in %s %s: %s"),
			   printable_type(o->repo, oid, object_type),
			   describe_object(oid), message);
		ঘূৰাই 1;
	স্ৱভাৱ:
		BUG("%d (FSCK_IGNORE?) should never trigger this callback",
		    বাৰ্তা_type);
	}
}

স্থির সংরচনা object_array pending;

স্থির পূর্ণাঙ্ক mark_object(সংরচনা object *obj, গণনা object_type type,
		       শূন্য *data, সংরচনা fsck_options *options)
{
	সংরচনা object *parent = data;

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
		ঘূৰাই 1;
	}

	যদি (type != OBJ_ANY && obj->type != type)
		/* ... and the reference to parent is safe here */
		objerror(options->repo, parent, _("wrong object type in link"));

	যদি (obj->flags & REACHABLE)
		ঘূৰাই 0;
	obj->flags |= REACHABLE;

	যদি (is_promisor_object(options->repo, &obj->oid))
		/*
		 * Further recursion does not need to be performed on this
		 * object since it is a promisor object (so it does not need to
		 * be added to "pending").
		 */
		ঘূৰাই 0;

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
		ঘূৰাই 1;
	}

	add_object_array(obj, রিক্ত, &pending);
	ঘূৰাই 0;
}

স্থির শূন্য mark_object_reachable(সংরচনা object *obj)
{
	mark_object(obj, OBJ_ANY, রিক্ত, &fsck_walk_options);
}

স্থির পূর্ণাঙ্ক traverse_one_object(সংরচনা object *obj)
{
	পূর্ণাঙ্ক result = fsck_walk(obj, obj, &fsck_walk_options);

	যদি (obj->type == OBJ_বৃক্ষ) {
		সংরচনা বৃক্ষ *বৃক্ষ = (সংরচনা বৃক্ষ *)obj;
		free_বৃক্ষ_buffer(বৃক্ষ);
	}
	ঘূৰাই result;
}

স্থির পূর্ণাঙ্ক traverse_reachable(সংরচনা repository *repo)
{
	সংরচনা progress *progress = রিক্ত;
	unsigned পূর্ণাঙ্ক nr = 0;
	পূর্ণাঙ্ক result = 0;
	যদি (show_progress)
		progress = start_delayed_progress(repo,
						  _("Checking connectivity"), 0);
	যেতিয়ালৈ (pending.nr) {
		result |= traverse_one_object(object_array_বেৰ_কৰো(&pending));
		display_progress(progress, ++nr);
	}
	stop_progress(&progress);
	ঘূৰাই !!result;
}

স্থির পূর্ণাঙ্ক mark_used(সংরচনা object *obj, গণনা object_type type UNUSED,
		     শূন্য *data UNUSED, সংরচনা fsck_options *options UNUSED)
{
	যদি (!obj)
		ঘূৰাই 1;
	obj->flags |= USED;
	ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক mark_unreachable_referents(ধ্রুৱ সংরচনা object_id *oid,
				      সংরচনা object_info *oi UNUSED,
				      শূন্য *data)
{
	সংরচনা repository *repo = data;
	সংরচনা fsck_options options;
	সংরচনা object *obj = lookup_object(data, oid);

	যদি (!obj || !(obj->flags & HAS_OBJ))
		ঘূৰাই 0; /* not part of our original set */
	যদি (obj->flags & REACHABLE)
		ঘূৰাই 0; /* reachable objects already traversed */

	/*
	 * Avoid passing OBJ_NONE to fsck_walk, which will parse the object
	 * (and we want to avoid parsing blobs).
	 */
	যদি (obj->type == OBJ_NONE) {
		গণনা object_type type = odb_read_object_info(repo->objects,
							     &obj->oid, রিক্ত);
		যদি (type > 0)
			object_as_type(obj, type, 0);
	}

	fsck_options_init(&options, repo, FSCK_OPTIONS_DEFAULT);
	options.walk = mark_used;
	fsck_walk(obj, রিক্ত, &options);
	যদি (obj->type == OBJ_বৃক্ষ)
		free_বৃক্ষ_buffer((সংরচনা বৃক্ষ *)obj);

	ঘূৰাই 0;
}

/*
 * Check a single reachable object
 */
স্থির শূন্য check_reachable_object(সংরচনা repository *repo, সংরচনা object *obj)
{
	/*
	 * We obviously want the object to be parsed,
	 * except if it was in a pack-file and we didn't
	 * do a full fsck
	 */
	যদি (!(obj->flags & HAS_OBJ)) {
		যদি (is_promisor_object(repo, &obj->oid))
			ঘূৰাই;
		যদি (has_object_pack(repo, &obj->oid))
			ঘূৰাই; /* it is in pack - forget about it */
		printf_ln(_("missing %s %s"),
			  printable_type(repo, &obj->oid, obj->type),
			  describe_object(&obj->oid));
		errors_found |= ERROR_REACHABLE;
		ঘূৰাই;
	}
}

/*
 * Check a single unreachable object
 */
স্থির শূন্য check_unreachable_object(সংরচনা repository *repo, সংরচনা object *obj)
{
	/*
	 * Missing unreachable object? Ignore it. It's not like
	 * we miss it (since it can't be reached), nor do we want
	 * to complain about it being unreachable (since it does
	 * not exist).
	 */
	যদি (!(obj->flags & HAS_OBJ))
		ঘূৰাই;

	/*
	 * Unreachable object that exists? Show it if asked to,
	 * since this is something that is prunable.
	 */
	যদি (show_unreachable) {
		printf_ln(_("unreachable %s %s"),
			  printable_type(repo, &obj->oid, obj->type),
			  describe_object(&obj->oid));
		ঘূৰাই;
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
			ৱর্ণ *filename = repo_git_path(repo, "lost-found/%s/%s",
				obj->type == OBJ_COMMIT ? "commit" : "other",
				describe_object(&obj->oid));
			FILE *f;

			যদি (safe_create_leading_directories_const(repo, filename)) {
				error(_("could not create lost-found"));
				মুক্ত(filename);
				ঘূৰাই;
			}
			f = xfopen(filename, "w");
			যদি (obj->type == OBJ_BLOB) {
				যদি (odb_stream_blob_to_fd(repo->objects, fileno(f),
							  &obj->oid, রিক্ত, 1))
					die_errno(_("could not write '%s'"), filename);
			} অন্যথা
				fprintf(f, "%s\n", describe_object(&obj->oid));
			যদি (fclose(f))
				die_errno(_("could not finish '%s'"),
					  filename);
			মুক্ত(filename);
		}
		ঘূৰাই;
	}

	/*
	 * Otherwise? It's there, it's unreachable, and some other unreachable
	 * object points to it. Ignore it - it's not interesting, and we showed
	 * all the interesting cases above.
	 */
}

স্থির শূন্য check_object(সংরচনা repository *repo, সংরচনা object *obj)
{
	যদি (verbose)
		fprintf_ln(stderr, _("Checking %s"), describe_object(&obj->oid));

	যদি (obj->flags & REACHABLE)
		check_reachable_object(repo, obj);
	অন্যথা
		check_unreachable_object(repo, obj);
}

স্থির শূন্য check_connectivity(সংরচনা repository *repo)
{
	পূর্ণাঙ্ক i, max;

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
		odb_for_each_object(repo->objects, রিক্ত,
				    mark_unreachable_referents, repo, 0);
	}

	/* Look up all the requirements, warn about missing objects.. */
	max = get_max_object_index(repo);
	যদি (verbose)
		fprintf_ln(stderr, _("Checking connectivity (%d objects)"), max);

	বাবে (i = 0; i < max; i++) {
		সংরচনা object *obj = get_indexed_object(repo, i);

		যদি (obj)
			check_object(repo, obj);
	}
}

স্থির পূর্ণাঙ্ক fsck_obj_buffer(ধ্রুৱ সংরচনা object_id *oid, গণনা object_type type,
			   unsigned long size, শূন্য *buffer, পূর্ণাঙ্ক *eaten, শূন্য *cb_data)
{
	সংরচনা repository *repo = cb_data;
	সংরচনা object *obj;
	পূর্ণাঙ্ক err;

	/*
	 * Note, buffer may be NULL if type is OBJ_BLOB. See
	 * verify_packfile(), data_valid variable for details.
	 */
	obj = parse_object_buffer(repo, oid, type, size, buffer, eaten);
	যদি (!obj) {
		errors_found |= ERROR_OBJECT;
		err = error(_("%s: object corrupt or missing"),
			    oid_to_hex(oid));
		গচ্ছ out;
	}

	obj->flags &= ~REACHABLE;
	obj->flags |= HAS_OBJ | SEEN;

	যদি (verbose)
		fprintf_ln(stderr, _("Checking %s %s"),
			   printable_type(repo, &obj->oid, obj->type),
			   describe_object(&obj->oid));

	যদি (fsck_walk(obj, রিক্ত, &fsck_obj_options))
		objerror(repo, obj, _("broken links"));

	err = fsck_object(obj, buffer, size, &fsck_obj_options);
	যদি (err)
		গচ্ছ out;

	যদি (obj->type == OBJ_COMMIT) {
		সংরচনা commit *commit = (সংরচনা commit *) obj;

		যদি (!commit->parents && show_মূল)
			printf_ln(_("root %s"),
				  describe_object(&commit->object.oid));
	}

	যদি (obj->type == OBJ_TAG) {
		সংরচনা tag *tag = (সংরচনা tag *) obj;

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
		free_বৃক্ষ_buffer((সংরচনা বৃক্ষ *)obj);
	ঘূৰাই err;
}

স্থির পূর্ণাঙ্ক default_refs;

স্থির শূন্য fsck_handle_reflog_oid(সংরচনা repository *repo,
				   ধ্রুৱ ৱর্ণ *refname, সংরচনা object_id *oid,
				   timestamp_t timestamp)
{
	সংরচনা object *obj;

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

স্থির পূর্ণাঙ্ক fsck_handle_reflog_ent(ধ্রুৱ ৱর্ণ *refname,
				  সংরচনা object_id *ooid, সংরচনা object_id *noid,
				  ধ্রুৱ ৱর্ণ *email UNUSED,
				  timestamp_t timestamp, পূর্ণাঙ্ক tz UNUSED,
				  ধ্রুৱ ৱর্ণ *message UNUSED, শূন্য *cb_data)
{
	সংরচনা repository *repo = cb_data;

	যদি (now && timestamp > now)
		ঘূৰাই 0;

	যদি (verbose)
		fprintf_ln(stderr, _("Checking reflog %s->%s"),
			   oid_to_hex(ooid), oid_to_hex(noid));

	fsck_handle_reflog_oid(repo, refname, ooid, 0);
	fsck_handle_reflog_oid(repo, refname, noid, timestamp);
	ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক fsck_handle_reflog(ধ্রুৱ ৱর্ণ *logname, শূন্য *cb_data)
{
	সংরচনা strbuf refname = STRBUF_INIT;
	সংরচনা worktree *wt = cb_data;

	strbuf_worktree_ref(wt, &refname, logname);
	refs_for_each_reflog_ent(get_main_ref_store(wt->repo),
				 refname.buf, fsck_handle_reflog_ent,
				 wt->repo);
	strbuf_release(&refname);
	ঘূৰাই 0;
}

সংরচনা ref_snapshot {
	ৱর্ণ *refname;
	সংরচনা object_id oid;
	/* TODO: Maybe supplement with latest reflog entry info too? */
};

সংরচনা snapshot {
	size_t nr;
	size_t alloc;
	সংরচনা ref_snapshot *ref;
	/* TODO: Consider also snapshotting the index of each worktree. */
};

সংরচনা snapshot_ref_data {
	সংরচনা repository *repo;
	সংরচনা snapshot *snap;
};

স্থির পূর্ণাঙ্ক snapshot_ref(ধ্রুৱ সংরচনা reference *ref, শূন্য *cb_data)
{
	সংরচনা snapshot_ref_data *data = cb_data;
	সংরচনা snapshot *snap = data->snap;
	সংরচনা object *obj;

	obj = parse_object(data->repo, ref->oid);
	যদি (!obj) {
		যদি (is_promisor_object(data->repo, ref->oid)) {
			/*
			 * Increment default_refs anyway, because this is a
			 * valid ref.
			 */
			 default_refs++;
			 ঘূৰাই 0;
		}
		error(_("%s: invalid sha1 pointer %s"),
		      ref->name, oid_to_hex(ref->oid));
		errors_found |= ERROR_REACHABLE;
		/* We'll continue with the rest despite the error.. */
		ঘূৰাই 0;
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

	ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক fsck_handle_ref(ধ্রুৱ সংরচনা reference *ref, শূন্য *cb_data)
{
	সংরচনা repository *repo = cb_data;
	সংরচনা object *obj;

	obj = parse_object(repo, ref->oid);
	obj->flags |= USED;
	fsck_put_object_name(&fsck_walk_options,
			     ref->oid, "%s", ref->name);
	mark_object_reachable(obj);

	ঘূৰাই 0;
}

স্থির শূন্য snapshot_refs(সংরচনা repository *repo,
			  সংরচনা snapshot *snap, পূর্ণাঙ্ক argc, ধ্রুৱ ৱর্ণ **argv)
{
	সংরচনা refs_for_each_ref_options opts = {
		.flags = REFS_FOR_EACH_INCLUDE_BROKEN,
	};
	সংরচনা snapshot_ref_data data = {
		.repo = repo,
		.snap = snap,
	};
	সংরচনা worktree **worktrees, **p;
	ধ্রুৱ ৱর্ণ *head_points_at;
	সংরচনা object_id head_oid;

	বাবে (পূর্ণাঙ্ক i = 0; i < argc; i++) {
		ধ্রুৱ ৱর্ণ *arg = argv[i];
		সংরচনা object_id oid;
		যদি (!repo_get_oid(repo, arg, &oid)) {
			সংরচনা reference ref = {
				.name = arg,
				.oid = &oid,
			};

			snapshot_ref(&ref, &data);
			অনুৱর্ত;
		}
		error(_("invalid parameter: expected sha1, got '%s'"), arg);
		errors_found |= ERROR_OBJECT;
	}

	যদি (argc) {
		include_reflogs = 0;
		ঘূৰাই;
	}

	refs_for_each_ref_ext(get_main_ref_store(repo),
			      snapshot_ref, &data, &opts);

	worktrees = get_worktrees(repo);
	বাবে (p = worktrees; *p; p++) {
		সংরচনা worktree *wt = *p;
		সংরচনা strbuf refname = STRBUF_INIT;

		strbuf_worktree_ref(wt, &refname, "HEAD");

		head_points_at = refs_resolve_ref_unsafe(get_main_ref_store(repo),
							 refname.buf, 0, &head_oid, রিক্ত);

		যদি (head_points_at && !is_null_oid(&head_oid)) {
			সংরচনা reference ref = {
				.name = refname.buf,
				.oid = &head_oid,
			};

			snapshot_ref(&ref, &data);
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
	now = time(রিক্ত);
}


স্থির শূন্য free_snapshot_refs(সংরচনা snapshot *snap)
{
	বাবে (size_t i = 0; i < snap->nr; i++)
		মুক্ত(snap->ref[i].refname);
	মুক্ত(snap->ref);
}

স্থির শূন্য process_refs(সংরচনা repository *repo, সংরচনা snapshot *snap)
{
	সংরচনা worktree **worktrees, **p;

	বাবে (size_t i = 0; i < snap->nr; i++) {
		সংরচনা reference ref = {
			.name = snap->ref[i].refname,
			.oid = &snap->ref[i].oid,
		};
		fsck_handle_ref(&ref, repo);
	}

	যদি (include_reflogs) {
		worktrees = get_worktrees(repo);
		বাবে (p = worktrees; *p; p++) {
			সংরচনা worktree *wt = *p;

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

স্থির পূর্ণাঙ্ক fsck_cache_বৃক্ষ(সংরচনা repository *repo, সংরচনা cache_বৃক্ষ *it, ধ্রুৱ ৱর্ণ *index_path)
{
	পূর্ণাঙ্ক i;
	পূর্ণাঙ্ক err = 0;

	যদি (verbose)
		fprintf_ln(stderr, _("Checking cache tree of %s"), index_path);

	যদি (0 <= it->entry_গুনতি) {
		সংরচনা object *obj = parse_object(repo, &it->oid);
		যদি (!obj) {
			error(_("%s: invalid sha1 pointer in cache-tree of %s"),
			      oid_to_hex(&it->oid), index_path);
			errors_found |= ERROR_REFS;
			ঘূৰাই 1;
		}
		obj->flags |= USED;
		fsck_put_object_name(&fsck_walk_options, &it->oid, ":");
		mark_object_reachable(obj);
		যদি (obj->type != OBJ_বৃক্ষ)
			err |= objerror(repo, obj, _("non-tree in cache-tree"));
	}
	বাবে (i = 0; i < it->subtree_nr; i++)
		err |= fsck_cache_বৃক্ষ(repo, it->down[i]->cache_বৃক্ষ, index_path);
	ঘূৰাই err;
}

স্থির পূর্ণাঙ্ক fsck_resolve_undo(সংরচনা index_state *istate,
			     ধ্রুৱ ৱর্ণ *index_path)
{
	সংরচনা string_list_item *item;
	সংরচনা string_list *resolve_undo = istate->resolve_undo;

	যদি (!resolve_undo)
		ঘূৰাই 0;

	for_each_string_list_item(item, resolve_undo) {
		ধ্রুৱ ৱর্ণ *path = item->string;
		সংরচনা resolve_undo_info *ru = item->util;
		পূর্ণাঙ্ক i;

		যদি (!ru)
			অনুৱর্ত;
		বাবে (i = 0; i < 3; i++) {
			সংরচনা object *obj;

			যদি (!ru->mode[i] || !S_ISREG(ru->mode[i]))
				অনুৱর্ত;

			obj = parse_object(istate->repo, &ru->oid[i]);
			যদি (!obj) {
				error(_("%s: invalid sha1 pointer in resolve-undo of %s"),
				      oid_to_hex(&ru->oid[i]),
				      index_path);
				errors_found |= ERROR_REFS;
				অনুৱর্ত;
			}
			obj->flags |= USED;
			fsck_put_object_name(&fsck_walk_options, &ru->oid[i],
					     ":(%d):%s", i, path);
			mark_object_reachable(obj);
		}
	}
	ঘূৰাই 0;
}

স্থির শূন্য fsck_index(সংরচনা index_state *istate, ধ্রুৱ ৱর্ণ *index_path,
		       পূর্ণাঙ্ক is_current_worktree)
{
	unsigned পূর্ণাঙ্ক i;

	/* TODO: audit for interaction with sparse-index. */
	ensure_full_index(istate);
	বাবে (i = 0; i < istate->cache_nr; i++) {
		unsigned পূর্ণাঙ্ক mode;
		সংরচনা blob *blob;
		সংরচনা object *obj;

		mode = istate->cache[i]->ce_mode;
		যদি (S_ISGITLINK(mode))
			অনুৱর্ত;
		blob = lookup_blob(istate->repo,
				   &istate->cache[i]->oid);
		যদি (!blob)
			অনুৱর্ত;
		obj = &blob->object;
		obj->flags |= USED;
		fsck_put_object_name(&fsck_walk_options, &obj->oid,
				     "%s:%s",
				     is_current_worktree ? "" : index_path,
				     istate->cache[i]->name);
		mark_object_reachable(obj);
	}
	যদি (istate->cache_বৃক্ষ)
		fsck_cache_বৃক্ষ(istate->repo, istate->cache_বৃক্ষ, index_path);
	fsck_resolve_undo(istate, index_path);
}

স্থির পূর্ণাঙ্ক mark_object_for_connectivity(ধ্রুৱ সংরচনা object_id *oid,
					সংরচনা object_info *oi UNUSED,
					শূন্য *cb_data)
{
	সংরচনা repository *repo = cb_data;
	সংরচনা object *obj = lookup_unknown_object(repo, oid);
	obj->flags |= HAS_OBJ;
	ঘূৰাই 0;
}

স্থির শূন্য fsck_refs(সংরচনা repository *r)
{
	সংরচনা child_process refs_verify = CHILD_PROCESS_INIT;
	সংরচনা progress *progress = রিক্ত;

	যদি (show_progress)
		progress = start_progress(r, _("Checking ref database"), 1);

	যদি (verbose)
		fprintf_ln(stderr, _("Checking ref database"));

	child_process_init(&refs_verify);
	refs_verify.git_cmd = 1;
	strvec_pushl(&refs_verify.args, "refs", "verify", রিক্ত);
	যদি (verbose)
		strvec_ঠেলো(&refs_verify.args, "--verbose");
	যদি (check_strict)
		strvec_ঠেলো(&refs_verify.args, "--strict");

	যদি (run_command(&refs_verify))
		errors_found |= ERROR_REFS;

	display_progress(progress, 1);
	stop_progress(&progress);
}

স্থির ৱর্ণ ধ্রুৱ * ধ্রুৱ fsck_usage[] = {
	N_("git fsck [--tags] [--root] [--unreachable] [--cache] [--no-reflogs]\n"
	   "         [--[no-]full] [--strict] [--verbose] [--lost-found]\n"
	   "         [--[no-]dangling] [--[no-]progress] [--connectivity-only]\n"
	   "         [--[no-]name-objects] [--[no-]references] [<object>...]"),
	রিক্ত
};

পূর্ণাঙ্ক cmd_fsck(পূর্ণাঙ্ক argc,
	     ধ্রুৱ ৱর্ণ **argv,
	     ধ্রুৱ ৱর্ণ *prefix,
	     সংরচনা repository *repo)
{
	সংরচনা odb_fsck_options odb_fsck_opts = {
		.flags = ODB_FSCK_FULL,
		.object_cb = fsck_obj_buffer,
		.object_payload = repo,
	};
	পূর্ণাঙ্ক keep_cache_objects = 0;
	পূর্ণাঙ্ক name_objects = 0;
	পূর্ণাঙ্ক check_references = 1;
	সংরচনা option fsck_opts[] = {
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
	সংরচনা snapshot snap = {
		.nr = 0,
		.alloc = 0,
		.ref = রিক্ত
	};

	errors_found = 0;
	disable_replace_refs();
	save_commit_buffer = 0;

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
		odb_for_each_object(repo->objects, রিক্ত,
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
		সংরচনা worktree **worktrees, **p;

		verify_index_checksum = 1;
		verify_ce_order = 1;

		worktrees = get_worktrees(repo);
		বাবে (p = worktrees; *p; p++) {
			সংরচনা worktree *wt = *p;
			সংরচনা index_state istate =
				INDEX_STATE_INIT(repo);
			ৱর্ণ *path, *wt_gitdir;

			/*
			 * Make a copy since the buffer is reusable
			 * and may get overwritten by other calls
			 * while we're examining the index.
			 */
			path = xstrdup(worktree_git_path(wt, "index"));
			wt_gitdir = get_worktree_git_dir(wt);

			read_index_from(&istate, path, wt_gitdir);
			fsck_index(&istate, path, wt->is_current);

			discard_index(&istate);
			মুক্ত(wt_gitdir);
			মুক্ত(path);
		}
		free_worktrees(worktrees);
	}

	check_connectivity(repo);

	যদি (repo->settings.core_commit_graph) {
		সংরচনা odb_source_files *files = odb_source_files_downcast(repo->objects->source);
		সংরচনা child_process commit_graph_verify = CHILD_PROCESS_INIT;

		বাবে (সংরচনা odb_files_dir *dir = files->dirs; dir; dir = dir->next) {
			child_process_init(&commit_graph_verify);
			commit_graph_verify.git_cmd = 1;
			strvec_pushl(&commit_graph_verify.args, "commit-graph",
				     "verify", "--object-dir", dir->abspath, রিক্ত);
			যদি (show_progress)
				strvec_ঠেলো(&commit_graph_verify.args, "--progress");
			অন্যথা
				strvec_ঠেলো(&commit_graph_verify.args, "--no-progress");
			যদি (run_command(&commit_graph_verify))
				errors_found |= ERROR_COMMIT_GRAPH;
		}
	}

	free_snapshot_refs(&snap);
	ঘূৰাই errors_found;
}
