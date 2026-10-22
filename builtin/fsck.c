#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "hex.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "commit.h"
#அந்தர்பாவ "tree.h"
#அந்தர்பாவ "blob.h"
#அந்தர்பாவ "tag.h"
#அந்தர்பாவ "refs.h"
#அந்தர்பாவ "cache-tree.h"
#அந்தர்பாவ "fsck.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "progress.h"
#அந்தர்பாவ "packfile.h"
#அந்தர்பாவ "object-name.h"
#அந்தர்பாவ "odb.h"
#அந்தர்பாவ "odb/streaming.h"
#அந்தர்பாவ "path.h"
#அந்தர்பாவ "read-cache-ll.h"
#அந்தர்பாவ "replace-object.h"
#அந்தர்பாவ "resolve-undo.h"
#அந்தர்பாவ "run-command.h"
#அந்தர்பாவ "sparse-index.h"
#அந்தர்பாவ "worktree.h"

#define REACHABLE 0x0001
#define SEEN      0x0002
#define HAS_OBJ   0x0004
/* This flag is set if something points to this object. */
#define USED      0x0008

நிலையான முழு show_வேர்;
நிலையான முழு show_tags;
நிலையான முழு show_unreachable;
நிலையான முழு include_reflogs = 1;
நிலையான முழு connectivity_only;
நிலையான முழு check_strict;
நிலையான கட்டமைப்பு fsck_options fsck_walk_options;
நிலையான கட்டமைப்பு fsck_options fsck_obj_options;
நிலையான முழு errors_found;
நிலையான முழு write_lost_and_found;
நிலையான முழு verbose;
நிலையான முழு show_progress = -1;
நிலையான முழு show_dangling = 1;
நிலையான timestamp_t now;
#define ERROR_OBJECT 01
#define ERROR_REACHABLE 02
#define ERROR_REFS 010
#define ERROR_COMMIT_GRAPH 020

நிலையான மாறிலி எழுத்து *describe_object(மாறிலி கட்டமைப்பு object_id *oid)
{
	திரும்பு fsck_describe_object(&fsck_walk_options, oid);
}

நிலையான மாறிலி எழுத்து *printable_type(கட்டமைப்பு repository *repo,
				  மாறிலி கட்டமைப்பு object_id *oid,
				  எண்ணிக்கை object_type type)
{
	மாறிலி எழுத்து *ret;

	எனில் (type == OBJ_NONE)
		type = odb_read_object_info(repo->objects, oid, வெற்று);

	ret = type_name(type);
	எனில் (!ret)
		ret = _("unknown");

	திரும்பு ret;
}

நிலையான முழு objerror(கட்டமைப்பு repository *repo, கட்டமைப்பு object *obj, மாறிலி எழுத்து *err)
{
	errors_found |= ERROR_OBJECT;
	/* TRANSLATORS: e.g. error in tree 01bfda: <more explanation> */
	fprintf_ln(stderr, _("error in %s %s: %s"),
		   printable_type(repo, &obj->oid, obj->type),
		   describe_object(&obj->oid), err);
	திரும்பு -1;
}

நிலையான முழு fsck_objects_error_func(கட்டமைப்பு fsck_options *o,
				   வெறுமை *fsck_report,
				   எண்ணிக்கை fsck_செய்தி_type செய்தி_type,
				   எண்ணிக்கை fsck_செய்தி_id செய்தி_id UNUSED,
				   மாறிலி எழுத்து *message)
{
	கட்டமைப்பு fsck_object_report *report = fsck_report;
	மாறிலி கட்டமைப்பு object_id *oid = report->oid;
	எண்ணிக்கை object_type object_type = report->object_type;

	தேர்வு (செய்தி_type) {
	நிலை FSCK_WARN:
		/* TRANSLATORS: e.g. warning in tree 01bfda: <more explanation> */
		fprintf_ln(stderr, _("warning in %s %s: %s"),
			   printable_type(o->repo, oid, object_type),
			   describe_object(oid), message);
		திரும்பு 0;
	நிலை FSCK_ERROR:
		/* TRANSLATORS: e.g. error in tree 01bfda: <more explanation> */
		fprintf_ln(stderr, _("error in %s %s: %s"),
			   printable_type(o->repo, oid, object_type),
			   describe_object(oid), message);
		திரும்பு 1;
	இயல்பு:
		BUG("%d (FSCK_IGNORE?) should never trigger this callback",
		    செய்தி_type);
	}
}

நிலையான கட்டமைப்பு object_array pending;

நிலையான முழு mark_object(கட்டமைப்பு object *obj, எண்ணிக்கை object_type type,
		       வெறுமை *data, கட்டமைப்பு fsck_options *options)
{
	கட்டமைப்பு object *parent = data;

	/*
	 * The only case data is NULL or type is OBJ_ANY is when
	 * mark_object_reachable() calls us.  All the callers of
	 * that function has non-NULL obj hence ...
	 */
	எனில் (!obj) {
		/* ... these references to parent->fld are safe here */
		printf_ln(_("broken link from %7s %s"),
			  printable_type(options->repo, &parent->oid, parent->type),
			  describe_object(&parent->oid));
		printf_ln(_("broken link from %7s %s"),
			  (type == OBJ_ANY ? _("unknown") : type_name(type)),
			  _("unknown"));
		errors_found |= ERROR_REACHABLE;
		திரும்பு 1;
	}

	எனில் (type != OBJ_ANY && obj->type != type)
		/* ... and the reference to parent is safe here */
		objerror(options->repo, parent, _("wrong object type in link"));

	எனில் (obj->flags & REACHABLE)
		திரும்பு 0;
	obj->flags |= REACHABLE;

	எனில் (is_promisor_object(options->repo, &obj->oid))
		/*
		 * Further recursion does not need to be performed on this
		 * object since it is a promisor object (so it does not need to
		 * be added to "pending").
		 */
		திரும்பு 0;

	எனில் (!(obj->flags & HAS_OBJ)) {
		எனில் (parent && !odb_has_object(options->repo->objects, &obj->oid,
					      ODB_HAS_OBJECT_RECHECK_PACKED)) {
			printf_ln(_("broken link from %7s %s\n"
				    "              to %7s %s"),
				  printable_type(options->repo, &parent->oid, parent->type),
				  describe_object(&parent->oid),
				  printable_type(options->repo, &obj->oid, obj->type),
				  describe_object(&obj->oid));
			errors_found |= ERROR_REACHABLE;
		}
		திரும்பு 1;
	}

	add_object_array(obj, வெற்று, &pending);
	திரும்பு 0;
}

நிலையான வெறுமை mark_object_reachable(கட்டமைப்பு object *obj)
{
	mark_object(obj, OBJ_ANY, வெற்று, &fsck_walk_options);
}

நிலையான முழு traverse_one_object(கட்டமைப்பு object *obj)
{
	முழு result = fsck_walk(obj, obj, &fsck_walk_options);

	எனில் (obj->type == OBJ_மரம்) {
		கட்டமைப்பு மரம் *மரம் = (கட்டமைப்பு மரம் *)obj;
		free_மரம்_buffer(மரம்);
	}
	திரும்பு result;
}

நிலையான முழு traverse_reachable(கட்டமைப்பு repository *repo)
{
	கட்டமைப்பு progress *progress = வெற்று;
	unsigned முழு nr = 0;
	முழு result = 0;
	எனில் (show_progress)
		progress = start_delayed_progress(repo,
						  _("Checking connectivity"), 0);
	போது (pending.nr) {
		result |= traverse_one_object(object_array_எடு(&pending));
		display_progress(progress, ++nr);
	}
	stop_progress(&progress);
	திரும்பு !!result;
}

நிலையான முழு mark_used(கட்டமைப்பு object *obj, எண்ணிக்கை object_type type UNUSED,
		     வெறுமை *data UNUSED, கட்டமைப்பு fsck_options *options UNUSED)
{
	எனில் (!obj)
		திரும்பு 1;
	obj->flags |= USED;
	திரும்பு 0;
}

நிலையான முழு mark_unreachable_referents(மாறிலி கட்டமைப்பு object_id *oid,
				      கட்டமைப்பு object_info *oi UNUSED,
				      வெறுமை *data)
{
	கட்டமைப்பு repository *repo = data;
	கட்டமைப்பு fsck_options options;
	கட்டமைப்பு object *obj = lookup_object(data, oid);

	எனில் (!obj || !(obj->flags & HAS_OBJ))
		திரும்பு 0; /* not part of our original set */
	எனில் (obj->flags & REACHABLE)
		திரும்பு 0; /* reachable objects already traversed */

	/*
	 * Avoid passing OBJ_NONE to fsck_walk, which will parse the object
	 * (and we want to avoid parsing blobs).
	 */
	எனில் (obj->type == OBJ_NONE) {
		எண்ணிக்கை object_type type = odb_read_object_info(repo->objects,
							     &obj->oid, வெற்று);
		எனில் (type > 0)
			object_as_type(obj, type, 0);
	}

	fsck_options_init(&options, repo, FSCK_OPTIONS_DEFAULT);
	options.walk = mark_used;
	fsck_walk(obj, வெற்று, &options);
	எனில் (obj->type == OBJ_மரம்)
		free_மரம்_buffer((கட்டமைப்பு மரம் *)obj);

	திரும்பு 0;
}

/*
 * Check a single reachable object
 */
நிலையான வெறுமை check_reachable_object(கட்டமைப்பு repository *repo, கட்டமைப்பு object *obj)
{
	/*
	 * We obviously want the object to be parsed,
	 * except if it was in a pack-file and we didn't
	 * do a full fsck
	 */
	எனில் (!(obj->flags & HAS_OBJ)) {
		எனில் (is_promisor_object(repo, &obj->oid))
			திரும்பு;
		எனில் (has_object_pack(repo, &obj->oid))
			திரும்பு; /* it is in pack - forget about it */
		printf_ln(_("missing %s %s"),
			  printable_type(repo, &obj->oid, obj->type),
			  describe_object(&obj->oid));
		errors_found |= ERROR_REACHABLE;
		திரும்பு;
	}
}

/*
 * Check a single unreachable object
 */
நிலையான வெறுமை check_unreachable_object(கட்டமைப்பு repository *repo, கட்டமைப்பு object *obj)
{
	/*
	 * Missing unreachable object? Ignore it. It's not like
	 * we miss it (since it can't be reached), nor do we want
	 * to complain about it being unreachable (since it does
	 * not exist).
	 */
	எனில் (!(obj->flags & HAS_OBJ))
		திரும்பு;

	/*
	 * Unreachable object that exists? Show it if asked to,
	 * since this is something that is prunable.
	 */
	எனில் (show_unreachable) {
		printf_ln(_("unreachable %s %s"),
			  printable_type(repo, &obj->oid, obj->type),
			  describe_object(&obj->oid));
		திரும்பு;
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
	எனில் (!(obj->flags & USED)) {
		எனில் (show_dangling)
			printf_ln(_("dangling %s %s"),
				  printable_type(repo, &obj->oid, obj->type),
				  describe_object(&obj->oid));
		எனில் (write_lost_and_found) {
			எழுத்து *filename = repo_git_path(repo, "lost-found/%s/%s",
				obj->type == OBJ_COMMIT ? "commit" : "other",
				describe_object(&obj->oid));
			FILE *f;

			எனில் (safe_create_leading_directories_const(repo, filename)) {
				error(_("could not create lost-found"));
				விடுவி(filename);
				திரும்பு;
			}
			f = xfopen(filename, "w");
			எனில் (obj->type == OBJ_BLOB) {
				எனில் (odb_stream_blob_to_fd(repo->objects, fileno(f),
							  &obj->oid, வெற்று, 1))
					die_errno(_("could not write '%s'"), filename);
			} இல்லையெனில்
				fprintf(f, "%s\n", describe_object(&obj->oid));
			எனில் (fclose(f))
				die_errno(_("could not finish '%s'"),
					  filename);
			விடுவி(filename);
		}
		திரும்பு;
	}

	/*
	 * Otherwise? It's there, it's unreachable, and some other unreachable
	 * object points to it. Ignore it - it's not interesting, and we showed
	 * all the interesting cases above.
	 */
}

நிலையான வெறுமை check_object(கட்டமைப்பு repository *repo, கட்டமைப்பு object *obj)
{
	எனில் (verbose)
		fprintf_ln(stderr, _("Checking %s"), describe_object(&obj->oid));

	எனில் (obj->flags & REACHABLE)
		check_reachable_object(repo, obj);
	இல்லையெனில்
		check_unreachable_object(repo, obj);
}

நிலையான வெறுமை check_connectivity(கட்டமைப்பு repository *repo)
{
	முழு i, max;

	/* Traverse the pending reachable objects */
	traverse_reachable(repo);

	/*
	 * With --connectivity-only, we won't have actually opened and marked
	 * unreachable objects with USED. Do that now to make --dangling, etc
	 * accurate.
	 */
	எனில் (connectivity_only && (show_dangling || write_lost_and_found)) {
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
		odb_for_each_object(repo->objects, வெற்று,
				    mark_unreachable_referents, repo, 0);
	}

	/* Look up all the requirements, warn about missing objects.. */
	max = get_max_object_index(repo);
	எனில் (verbose)
		fprintf_ln(stderr, _("Checking connectivity (%d objects)"), max);

	க்கு (i = 0; i < max; i++) {
		கட்டமைப்பு object *obj = get_indexed_object(repo, i);

		எனில் (obj)
			check_object(repo, obj);
	}
}

நிலையான முழு fsck_obj_buffer(மாறிலி கட்டமைப்பு object_id *oid, எண்ணிக்கை object_type type,
			   unsigned long size, வெறுமை *buffer, முழு *eaten, வெறுமை *cb_data)
{
	கட்டமைப்பு repository *repo = cb_data;
	கட்டமைப்பு object *obj;
	முழு err;

	/*
	 * Note, buffer may be NULL if type is OBJ_BLOB. See
	 * verify_packfile(), data_valid variable for details.
	 */
	obj = parse_object_buffer(repo, oid, type, size, buffer, eaten);
	எனில் (!obj) {
		errors_found |= ERROR_OBJECT;
		err = error(_("%s: object corrupt or missing"),
			    oid_to_hex(oid));
		செல் out;
	}

	obj->flags &= ~REACHABLE;
	obj->flags |= HAS_OBJ | SEEN;

	எனில் (verbose)
		fprintf_ln(stderr, _("Checking %s %s"),
			   printable_type(repo, &obj->oid, obj->type),
			   describe_object(&obj->oid));

	எனில் (fsck_walk(obj, வெற்று, &fsck_obj_options))
		objerror(repo, obj, _("broken links"));

	err = fsck_object(obj, buffer, size, &fsck_obj_options);
	எனில் (err)
		செல் out;

	எனில் (obj->type == OBJ_COMMIT) {
		கட்டமைப்பு commit *commit = (கட்டமைப்பு commit *) obj;

		எனில் (!commit->parents && show_வேர்)
			printf_ln(_("root %s"),
				  describe_object(&commit->object.oid));
	}

	எனில் (obj->type == OBJ_TAG) {
		கட்டமைப்பு tag *tag = (கட்டமைப்பு tag *) obj;

		எனில் (show_tags && tag->tagged) {
			printf_ln(_("tagged %s %s (%s) in %s"),
				  printable_type(repo, &tag->tagged->oid, tag->tagged->type),
				  describe_object(&tag->tagged->oid),
				  tag->tag,
				  describe_object(&tag->object.oid));
		}
	}

out:
	எனில் (obj && obj->type == OBJ_மரம்)
		free_மரம்_buffer((கட்டமைப்பு மரம் *)obj);
	திரும்பு err;
}

நிலையான முழு default_refs;

நிலையான வெறுமை fsck_handle_reflog_oid(கட்டமைப்பு repository *repo,
				   மாறிலி எழுத்து *refname, கட்டமைப்பு object_id *oid,
				   timestamp_t timestamp)
{
	கட்டமைப்பு object *obj;

	எனில் (!is_null_oid(oid)) {
		obj = lookup_object(repo, oid);
		எனில் (obj && (obj->flags & HAS_OBJ)) {
			எனில் (timestamp)
				fsck_put_object_name(&fsck_walk_options, oid,
						     "%s@{%"PRItime"}",
						     refname, timestamp);
			obj->flags |= USED;
			mark_object_reachable(obj);
		} இல்லையெனில் எனில் (!is_promisor_object(repo, oid)) {
			error(_("%s: invalid reflog entry %s"),
			      refname, oid_to_hex(oid));
			errors_found |= ERROR_REACHABLE;
		}
	}
}

நிலையான முழு fsck_handle_reflog_ent(மாறிலி எழுத்து *refname,
				  கட்டமைப்பு object_id *ooid, கட்டமைப்பு object_id *noid,
				  மாறிலி எழுத்து *email UNUSED,
				  timestamp_t timestamp, முழு tz UNUSED,
				  மாறிலி எழுத்து *message UNUSED, வெறுமை *cb_data)
{
	கட்டமைப்பு repository *repo = cb_data;

	எனில் (now && timestamp > now)
		திரும்பு 0;

	எனில் (verbose)
		fprintf_ln(stderr, _("Checking reflog %s->%s"),
			   oid_to_hex(ooid), oid_to_hex(noid));

	fsck_handle_reflog_oid(repo, refname, ooid, 0);
	fsck_handle_reflog_oid(repo, refname, noid, timestamp);
	திரும்பு 0;
}

நிலையான முழு fsck_handle_reflog(மாறிலி எழுத்து *logname, வெறுமை *cb_data)
{
	கட்டமைப்பு strbuf refname = STRBUF_INIT;
	கட்டமைப்பு worktree *wt = cb_data;

	strbuf_worktree_ref(wt, &refname, logname);
	refs_for_each_reflog_ent(get_main_ref_store(wt->repo),
				 refname.buf, fsck_handle_reflog_ent,
				 wt->repo);
	strbuf_release(&refname);
	திரும்பு 0;
}

கட்டமைப்பு ref_snapshot {
	எழுத்து *refname;
	கட்டமைப்பு object_id oid;
	/* TODO: Maybe supplement with latest reflog entry info too? */
};

கட்டமைப்பு snapshot {
	size_t nr;
	size_t alloc;
	கட்டமைப்பு ref_snapshot *ref;
	/* TODO: Consider also snapshotting the index of each worktree. */
};

கட்டமைப்பு snapshot_ref_data {
	கட்டமைப்பு repository *repo;
	கட்டமைப்பு snapshot *snap;
};

நிலையான முழு snapshot_ref(மாறிலி கட்டமைப்பு reference *ref, வெறுமை *cb_data)
{
	கட்டமைப்பு snapshot_ref_data *data = cb_data;
	கட்டமைப்பு snapshot *snap = data->snap;
	கட்டமைப்பு object *obj;

	obj = parse_object(data->repo, ref->oid);
	எனில் (!obj) {
		எனில் (is_promisor_object(data->repo, ref->oid)) {
			/*
			 * Increment default_refs anyway, because this is a
			 * valid ref.
			 */
			 default_refs++;
			 திரும்பு 0;
		}
		error(_("%s: invalid sha1 pointer %s"),
		      ref->name, oid_to_hex(ref->oid));
		errors_found |= ERROR_REACHABLE;
		/* We'll continue with the rest despite the error.. */
		திரும்பு 0;
	}
	எனில் (obj->type != OBJ_COMMIT && is_branch(ref->name)) {
		error(_("%s: not a commit"), ref->name);
		errors_found |= ERROR_REFS;
	}
	default_refs++;

	ALLOC_GROW(snap->ref, snap->nr + 1, snap->alloc);
	snap->ref[snap->nr].refname = xstrdup(ref->name);
	oidcpy(&snap->ref[snap->nr].oid, ref->oid);
	snap->nr++;

	திரும்பு 0;
}

நிலையான முழு fsck_handle_ref(மாறிலி கட்டமைப்பு reference *ref, வெறுமை *cb_data)
{
	கட்டமைப்பு repository *repo = cb_data;
	கட்டமைப்பு object *obj;

	obj = parse_object(repo, ref->oid);
	obj->flags |= USED;
	fsck_put_object_name(&fsck_walk_options,
			     ref->oid, "%s", ref->name);
	mark_object_reachable(obj);

	திரும்பு 0;
}

நிலையான வெறுமை snapshot_refs(கட்டமைப்பு repository *repo,
			  கட்டமைப்பு snapshot *snap, முழு argc, மாறிலி எழுத்து **argv)
{
	கட்டமைப்பு refs_for_each_ref_options opts = {
		.flags = REFS_FOR_EACH_INCLUDE_BROKEN,
	};
	கட்டமைப்பு snapshot_ref_data data = {
		.repo = repo,
		.snap = snap,
	};
	கட்டமைப்பு worktree **worktrees, **p;
	மாறிலி எழுத்து *head_points_at;
	கட்டமைப்பு object_id head_oid;

	க்கு (முழு i = 0; i < argc; i++) {
		மாறிலி எழுத்து *arg = argv[i];
		கட்டமைப்பு object_id oid;
		எனில் (!repo_get_oid(repo, arg, &oid)) {
			கட்டமைப்பு reference ref = {
				.name = arg,
				.oid = &oid,
			};

			snapshot_ref(&ref, &data);
			தொடர்;
		}
		error(_("invalid parameter: expected sha1, got '%s'"), arg);
		errors_found |= ERROR_OBJECT;
	}

	எனில் (argc) {
		include_reflogs = 0;
		திரும்பு;
	}

	refs_for_each_ref_ext(get_main_ref_store(repo),
			      snapshot_ref, &data, &opts);

	worktrees = get_worktrees(repo);
	க்கு (p = worktrees; *p; p++) {
		கட்டமைப்பு worktree *wt = *p;
		கட்டமைப்பு strbuf refname = STRBUF_INIT;

		strbuf_worktree_ref(wt, &refname, "HEAD");

		head_points_at = refs_resolve_ref_unsafe(get_main_ref_store(repo),
							 refname.buf, 0, &head_oid, வெற்று);

		எனில் (head_points_at && !is_null_oid(&head_oid)) {
			கட்டமைப்பு reference ref = {
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
	now = time(வெற்று);
}


நிலையான வெறுமை free_snapshot_refs(கட்டமைப்பு snapshot *snap)
{
	க்கு (size_t i = 0; i < snap->nr; i++)
		விடுவி(snap->ref[i].refname);
	விடுவி(snap->ref);
}

நிலையான வெறுமை process_refs(கட்டமைப்பு repository *repo, கட்டமைப்பு snapshot *snap)
{
	கட்டமைப்பு worktree **worktrees, **p;

	க்கு (size_t i = 0; i < snap->nr; i++) {
		கட்டமைப்பு reference ref = {
			.name = snap->ref[i].refname,
			.oid = &snap->ref[i].oid,
		};
		fsck_handle_ref(&ref, repo);
	}

	எனில் (include_reflogs) {
		worktrees = get_worktrees(repo);
		க்கு (p = worktrees; *p; p++) {
			கட்டமைப்பு worktree *wt = *p;

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
	எனில் (!default_refs) {
		fprintf_ln(stderr, _("notice: No default references"));
		show_unreachable = 0;
	}
}

நிலையான முழு fsck_cache_மரம்(கட்டமைப்பு repository *repo, கட்டமைப்பு cache_மரம் *it, மாறிலி எழுத்து *index_path)
{
	முழு i;
	முழு err = 0;

	எனில் (verbose)
		fprintf_ln(stderr, _("Checking cache tree of %s"), index_path);

	எனில் (0 <= it->entry_மொத்தம்) {
		கட்டமைப்பு object *obj = parse_object(repo, &it->oid);
		எனில் (!obj) {
			error(_("%s: invalid sha1 pointer in cache-tree of %s"),
			      oid_to_hex(&it->oid), index_path);
			errors_found |= ERROR_REFS;
			திரும்பு 1;
		}
		obj->flags |= USED;
		fsck_put_object_name(&fsck_walk_options, &it->oid, ":");
		mark_object_reachable(obj);
		எனில் (obj->type != OBJ_மரம்)
			err |= objerror(repo, obj, _("non-tree in cache-tree"));
	}
	க்கு (i = 0; i < it->subtree_nr; i++)
		err |= fsck_cache_மரம்(repo, it->down[i]->cache_மரம், index_path);
	திரும்பு err;
}

நிலையான முழு fsck_resolve_undo(கட்டமைப்பு index_state *istate,
			     மாறிலி எழுத்து *index_path)
{
	கட்டமைப்பு string_list_item *item;
	கட்டமைப்பு string_list *resolve_undo = istate->resolve_undo;

	எனில் (!resolve_undo)
		திரும்பு 0;

	for_each_string_list_item(item, resolve_undo) {
		மாறிலி எழுத்து *path = item->string;
		கட்டமைப்பு resolve_undo_info *ru = item->util;
		முழு i;

		எனில் (!ru)
			தொடர்;
		க்கு (i = 0; i < 3; i++) {
			கட்டமைப்பு object *obj;

			எனில் (!ru->mode[i] || !S_ISREG(ru->mode[i]))
				தொடர்;

			obj = parse_object(istate->repo, &ru->oid[i]);
			எனில் (!obj) {
				error(_("%s: invalid sha1 pointer in resolve-undo of %s"),
				      oid_to_hex(&ru->oid[i]),
				      index_path);
				errors_found |= ERROR_REFS;
				தொடர்;
			}
			obj->flags |= USED;
			fsck_put_object_name(&fsck_walk_options, &ru->oid[i],
					     ":(%d):%s", i, path);
			mark_object_reachable(obj);
		}
	}
	திரும்பு 0;
}

நிலையான வெறுமை fsck_index(கட்டமைப்பு index_state *istate, மாறிலி எழுத்து *index_path,
		       முழு is_current_worktree)
{
	unsigned முழு i;

	/* TODO: audit for interaction with sparse-index. */
	ensure_full_index(istate);
	க்கு (i = 0; i < istate->cache_nr; i++) {
		unsigned முழு mode;
		கட்டமைப்பு blob *blob;
		கட்டமைப்பு object *obj;

		mode = istate->cache[i]->ce_mode;
		எனில் (S_ISGITLINK(mode))
			தொடர்;
		blob = lookup_blob(istate->repo,
				   &istate->cache[i]->oid);
		எனில் (!blob)
			தொடர்;
		obj = &blob->object;
		obj->flags |= USED;
		fsck_put_object_name(&fsck_walk_options, &obj->oid,
				     "%s:%s",
				     is_current_worktree ? "" : index_path,
				     istate->cache[i]->name);
		mark_object_reachable(obj);
	}
	எனில் (istate->cache_மரம்)
		fsck_cache_மரம்(istate->repo, istate->cache_மரம், index_path);
	fsck_resolve_undo(istate, index_path);
}

நிலையான முழு mark_object_for_connectivity(மாறிலி கட்டமைப்பு object_id *oid,
					கட்டமைப்பு object_info *oi UNUSED,
					வெறுமை *cb_data)
{
	கட்டமைப்பு repository *repo = cb_data;
	கட்டமைப்பு object *obj = lookup_unknown_object(repo, oid);
	obj->flags |= HAS_OBJ;
	திரும்பு 0;
}

நிலையான வெறுமை fsck_refs(கட்டமைப்பு repository *r)
{
	கட்டமைப்பு child_process refs_verify = CHILD_PROCESS_INIT;
	கட்டமைப்பு progress *progress = வெற்று;

	எனில் (show_progress)
		progress = start_progress(r, _("Checking ref database"), 1);

	எனில் (verbose)
		fprintf_ln(stderr, _("Checking ref database"));

	child_process_init(&refs_verify);
	refs_verify.git_cmd = 1;
	strvec_pushl(&refs_verify.args, "refs", "verify", வெற்று);
	எனில் (verbose)
		strvec_தள்ளு(&refs_verify.args, "--verbose");
	எனில் (check_strict)
		strvec_தள்ளு(&refs_verify.args, "--strict");

	எனில் (run_command(&refs_verify))
		errors_found |= ERROR_REFS;

	display_progress(progress, 1);
	stop_progress(&progress);
}

நிலையான எழுத்து மாறிலி * மாறிலி fsck_usage[] = {
	N_("git fsck [--tags] [--root] [--unreachable] [--cache] [--no-reflogs]\n"
	   "         [--[no-]full] [--strict] [--verbose] [--lost-found]\n"
	   "         [--[no-]dangling] [--[no-]progress] [--connectivity-only]\n"
	   "         [--[no-]name-objects] [--[no-]references] [<object>...]"),
	வெற்று
};

முழு cmd_fsck(முழு argc,
	     மாறிலி எழுத்து **argv,
	     மாறிலி எழுத்து *prefix,
	     கட்டமைப்பு repository *repo)
{
	கட்டமைப்பு odb_fsck_options odb_fsck_opts = {
		.flags = ODB_FSCK_FULL,
		.object_cb = fsck_obj_buffer,
		.object_payload = repo,
	};
	முழு keep_cache_objects = 0;
	முழு name_objects = 0;
	முழு check_references = 1;
	கட்டமைப்பு option fsck_opts[] = {
		OPT__VERBOSE(&verbose, N_("be verbose")),
		OPT_BOOL(0, "unreachable", &show_unreachable, N_("show unreachable objects")),
		OPT_BOOL(0, "dangling", &show_dangling, N_("show dangling objects")),
		OPT_BOOL(0, "tags", &show_tags, N_("report tags")),
		OPT_BOOL(0, "root", &show_வேர், N_("report root nodes")),
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
	கட்டமைப்பு snapshot snap = {
		.nr = 0,
		.alloc = 0,
		.ref = வெற்று
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
	எனில் (check_strict)
		fsck_obj_options.strict = 1;

	எனில் (show_progress == -1)
		show_progress = isatty(2);
	எனில் (verbose) {
		show_progress = 0;
		odb_fsck_opts.flags |= ODB_FSCK_VERBOSE;
	}
	எனில் (show_progress)
		odb_fsck_opts.flags |= ODB_FSCK_PROGRESS;

	எனில் (write_lost_and_found) {
		odb_fsck_opts.flags |= ODB_FSCK_FULL;
		include_reflogs = 0;
	}

	எனில் (name_objects)
		fsck_enable_object_names(&fsck_walk_options);

	repo_config(repo, git_fsck_config, &fsck_obj_options);
	prepare_repo_settings(repo);

	எனில் (check_references)
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

	எனில் (connectivity_only) {
		odb_for_each_object(repo->objects, வெற்று,
				    mark_object_for_connectivity, repo, 0);
	} இல்லையெனில் {
		எனில் (odb_fsck(repo->objects, &odb_fsck_opts) < 0)
			errors_found |= ERROR_OBJECT;

		எனில் (fsck_finish(&fsck_obj_options))
			errors_found |= ERROR_OBJECT;
	}

	/* Process the snapshotted refs and the reflogs. */
	process_refs(repo, &snap);

	/* If not given any explicit objects, process index files too. */
	எனில் (!argc)
		keep_cache_objects = 1;
	எனில் (keep_cache_objects) {
		/*
		 * TODO: Consider first walking these indexes in snapshot_refs,
		 * to snapshot where the index entries used to point, and then
		 * check those snapshotted locations here.
		 */
		கட்டமைப்பு worktree **worktrees, **p;

		verify_index_checksum = 1;
		verify_ce_order = 1;

		worktrees = get_worktrees(repo);
		க்கு (p = worktrees; *p; p++) {
			கட்டமைப்பு worktree *wt = *p;
			கட்டமைப்பு index_state istate =
				INDEX_STATE_INIT(repo);
			எழுத்து *path, *wt_gitdir;

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
			விடுவி(wt_gitdir);
			விடுவி(path);
		}
		free_worktrees(worktrees);
	}

	check_connectivity(repo);

	எனில் (repo->settings.core_commit_graph) {
		கட்டமைப்பு odb_source_files *files = odb_source_files_downcast(repo->objects->source);
		கட்டமைப்பு child_process commit_graph_verify = CHILD_PROCESS_INIT;

		க்கு (கட்டமைப்பு odb_files_dir *dir = files->dirs; dir; dir = dir->next) {
			child_process_init(&commit_graph_verify);
			commit_graph_verify.git_cmd = 1;
			strvec_pushl(&commit_graph_verify.args, "commit-graph",
				     "verify", "--object-dir", dir->abspath, வெற்று);
			எனில் (show_progress)
				strvec_தள்ளு(&commit_graph_verify.args, "--progress");
			இல்லையெனில்
				strvec_தள்ளு(&commit_graph_verify.args, "--no-progress");
			எனில் (run_command(&commit_graph_verify))
				errors_found |= ERROR_COMMIT_GRAPH;
		}
	}

	free_snapshot_refs(&snap);
	திரும்பு errors_found;
}
