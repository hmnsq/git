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

#define REACHABLE 0x0001
#define SEEN      0x0002
#define HAS_OBJ   0x0004
/* This flag is set if something points to this object. */
#define USED      0x0008

स्थिर पूर्णांक show_मूळ;
स्थिर पूर्णांक show_tags;
स्थिर पूर्णांक show_unreachable;
स्थिर पूर्णांक include_reflogs = 1;
स्थिर पूर्णांक connectivity_only;
स्थिर पूर्णांक check_strict;
स्थिर संरचना fsck_options fsck_walk_options;
स्थिर संरचना fsck_options fsck_obj_options;
स्थिर पूर्णांक errors_found;
स्थिर पूर्णांक write_lost_and_found;
स्थिर पूर्णांक verbose;
स्थिर पूर्णांक show_progress = -1;
स्थिर पूर्णांक show_dangling = 1;
स्थिर timestamp_t now;
#define ERROR_OBJECT 01
#define ERROR_REACHABLE 02
#define ERROR_REFS 010
#define ERROR_COMMIT_GRAPH 020

स्थिर स्थिरांक अक्षर *describe_object(स्थिरांक संरचना object_id *oid)
{
	परत fsck_describe_object(&fsck_walk_options, oid);
}

स्थिर स्थिरांक अक्षर *printable_type(संरचना repository *repo,
				  स्थिरांक संरचना object_id *oid,
				  गणना object_type type)
{
	स्थिरांक अक्षर *ret;

	जर (type == OBJ_NONE)
		type = odb_read_object_info(repo->objects, oid, रिक्त);

	ret = type_name(type);
	जर (!ret)
		ret = _("unknown");

	परत ret;
}

स्थिर पूर्णांक objerror(संरचना repository *repo, संरचना object *obj, स्थिरांक अक्षर *err)
{
	errors_found |= ERROR_OBJECT;
	/* TRANSLATORS: e.g. error in tree 01bfda: <more explanation> */
	fprintf_ln(मानक_त्रुटि, _("error in %s %s: %s"),
		   printable_type(repo, &obj->oid, obj->type),
		   describe_object(&obj->oid), err);
	परत -1;
}

स्थिर पूर्णांक fsck_objects_error_func(संरचना fsck_options *o,
				   शून्य *fsck_report,
				   गणना fsck_संदेश_type संदेश_type,
				   गणना fsck_संदेश_id संदेश_id UNUSED,
				   स्थिरांक अक्षर *message)
{
	संरचना fsck_object_report *report = fsck_report;
	स्थिरांक संरचना object_id *oid = report->oid;
	गणना object_type object_type = report->object_type;

	निवड (संदेश_type) {
	स्थिती FSCK_WARN:
		/* TRANSLATORS: e.g. warning in tree 01bfda: <more explanation> */
		fprintf_ln(मानक_त्रुटि, _("warning in %s %s: %s"),
			   printable_type(o->repo, oid, object_type),
			   describe_object(oid), message);
		परत 0;
	स्थिती FSCK_ERROR:
		/* TRANSLATORS: e.g. error in tree 01bfda: <more explanation> */
		fprintf_ln(मानक_त्रुटि, _("error in %s %s: %s"),
			   printable_type(o->repo, oid, object_type),
			   describe_object(oid), message);
		परत 1;
	पूर्वनिर्धारित:
		BUG("%d (FSCK_IGNORE?) should never trigger this callback",
		    संदेश_type);
	}
}

स्थिर संरचना object_array pending;

स्थिर पूर्णांक mark_object(संरचना object *obj, गणना object_type type,
		       शून्य *data, संरचना fsck_options *options)
{
	संरचना object *parent = data;

	/*
	 * The only case data is NULL or type is OBJ_ANY is when
	 * mark_object_reachable() calls us.  All the callers of
	 * that function has non-NULL obj hence ...
	 */
	जर (!obj) {
		/* ... these references to parent->fld are safe here */
		printf_ln(_("broken link from %7s %s"),
			  printable_type(options->repo, &parent->oid, parent->type),
			  describe_object(&parent->oid));
		printf_ln(_("broken link from %7s %s"),
			  (type == OBJ_ANY ? _("unknown") : type_name(type)),
			  _("unknown"));
		errors_found |= ERROR_REACHABLE;
		परत 1;
	}

	जर (type != OBJ_ANY && obj->type != type)
		/* ... and the reference to parent is safe here */
		objerror(options->repo, parent, _("wrong object type in link"));

	जर (obj->flags & REACHABLE)
		परत 0;
	obj->flags |= REACHABLE;

	जर (is_promisor_object(options->repo, &obj->oid))
		/*
		 * Further recursion does not need to be performed on this
		 * object since it is a promisor object (so it does not need to
		 * be added to "pending").
		 */
		परत 0;

	जर (!(obj->flags & HAS_OBJ)) {
		जर (parent && !odb_has_object(options->repo->objects, &obj->oid,
					      ODB_HAS_OBJECT_RECHECK_PACKED)) {
			printf_ln(_("broken link from %7s %s\n"
				    "              to %7s %s"),
				  printable_type(options->repo, &parent->oid, parent->type),
				  describe_object(&parent->oid),
				  printable_type(options->repo, &obj->oid, obj->type),
				  describe_object(&obj->oid));
			errors_found |= ERROR_REACHABLE;
		}
		परत 1;
	}

	add_object_array(obj, रिक्त, &pending);
	परत 0;
}

स्थिर शून्य mark_object_reachable(संरचना object *obj)
{
	mark_object(obj, OBJ_ANY, रिक्त, &fsck_walk_options);
}

स्थिर पूर्णांक traverse_one_object(संरचना object *obj)
{
	पूर्णांक result = fsck_walk(obj, obj, &fsck_walk_options);

	जर (obj->type == OBJ_वृक्ष) {
		संरचना वृक्ष *वृक्ष = (संरचना वृक्ष *)obj;
		free_वृक्ष_buffer(वृक्ष);
	}
	परत result;
}

स्थिर पूर्णांक traverse_reachable(संरचना repository *repo)
{
	संरचना progress *progress = रिक्त;
	अचिन्हित पूर्णांक nr = 0;
	पूर्णांक result = 0;
	जर (show_progress)
		progress = start_delayed_progress(repo,
						  _("Checking connectivity"), 0);
	पर्यंत (pending.nr) {
		result |= traverse_one_object(object_array_काढा(&pending));
		display_progress(progress, ++nr);
	}
	stop_progress(&progress);
	परत !!result;
}

स्थिर पूर्णांक mark_used(संरचना object *obj, गणना object_type type UNUSED,
		     शून्य *data UNUSED, संरचना fsck_options *options UNUSED)
{
	जर (!obj)
		परत 1;
	obj->flags |= USED;
	परत 0;
}

स्थिर पूर्णांक mark_unreachable_referents(स्थिरांक संरचना object_id *oid,
				      संरचना object_info *oi UNUSED,
				      शून्य *data)
{
	संरचना repository *repo = data;
	संरचना fsck_options options;
	संरचना object *obj = lookup_object(data, oid);

	जर (!obj || !(obj->flags & HAS_OBJ))
		परत 0; /* not part of our original set */
	जर (obj->flags & REACHABLE)
		परत 0; /* reachable objects already traversed */

	/*
	 * Avoid passing OBJ_NONE to fsck_walk, which will parse the object
	 * (and we want to avoid parsing blobs).
	 */
	जर (obj->type == OBJ_NONE) {
		गणना object_type type = odb_read_object_info(repo->objects,
							     &obj->oid, रिक्त);
		जर (type > 0)
			object_as_type(obj, type, 0);
	}

	fsck_options_init(&options, repo, FSCK_OPTIONS_DEFAULT);
	options.walk = mark_used;
	fsck_walk(obj, रिक्त, &options);
	जर (obj->type == OBJ_वृक्ष)
		free_वृक्ष_buffer((संरचना वृक्ष *)obj);

	परत 0;
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
	जर (!(obj->flags & HAS_OBJ)) {
		जर (is_promisor_object(repo, &obj->oid))
			परत;
		जर (has_object_pack(repo, &obj->oid))
			परत; /* it is in pack - forget about it */
		printf_ln(_("missing %s %s"),
			  printable_type(repo, &obj->oid, obj->type),
			  describe_object(&obj->oid));
		errors_found |= ERROR_REACHABLE;
		परत;
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
	जर (!(obj->flags & HAS_OBJ))
		परत;

	/*
	 * Unreachable object that exists? Show it if asked to,
	 * since this is something that is prunable.
	 */
	जर (show_unreachable) {
		printf_ln(_("unreachable %s %s"),
			  printable_type(repo, &obj->oid, obj->type),
			  describe_object(&obj->oid));
		परत;
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
	जर (!(obj->flags & USED)) {
		जर (show_dangling)
			printf_ln(_("dangling %s %s"),
				  printable_type(repo, &obj->oid, obj->type),
				  describe_object(&obj->oid));
		जर (write_lost_and_found) {
			अक्षर *filename = repo_git_path(repo, "lost-found/%s/%s",
				obj->type == OBJ_COMMIT ? "commit" : "other",
				describe_object(&obj->oid));
			खाता *f;

			जर (safe_create_leading_directories_const(repo, filename)) {
				error(_("could not create lost-found"));
				मुक्त(filename);
				परत;
			}
			f = xfopen(filename, "w");
			जर (obj->type == OBJ_BLOB) {
				जर (odb_stream_blob_to_fd(repo->objects, fileno(f),
							  &obj->oid, रिक्त, 1))
					die_errno(_("could not write '%s'"), filename);
			} अन्यथा
				ख_लिखो(f, "%s\n", describe_object(&obj->oid));
			जर (ख_बंद(f))
				die_errno(_("could not finish '%s'"),
					  filename);
			मुक्त(filename);
		}
		परत;
	}

	/*
	 * Otherwise? It's there, it's unreachable, and some other unreachable
	 * object points to it. Ignore it - it's not interesting, and we showed
	 * all the interesting cases above.
	 */
}

स्थिर शून्य check_object(संरचना repository *repo, संरचना object *obj)
{
	जर (verbose)
		fprintf_ln(मानक_त्रुटि, _("Checking %s"), describe_object(&obj->oid));

	जर (obj->flags & REACHABLE)
		check_reachable_object(repo, obj);
	अन्यथा
		check_unreachable_object(repo, obj);
}

स्थिर शून्य check_connectivity(संरचना repository *repo)
{
	पूर्णांक i, max;

	/* Traverse the pending reachable objects */
	traverse_reachable(repo);

	/*
	 * With --connectivity-only, we won't have actually opened and marked
	 * unreachable objects with USED. Do that now to make --dangling, etc
	 * accurate.
	 */
	जर (connectivity_only && (show_dangling || write_lost_and_found)) {
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
	max = get_max_object_index(repo);
	जर (verbose)
		fprintf_ln(मानक_त्रुटि, _("Checking connectivity (%d objects)"), max);

	साठी (i = 0; i < max; i++) {
		संरचना object *obj = get_indexed_object(repo, i);

		जर (obj)
			check_object(repo, obj);
	}
}

स्थिर पूर्णांक fsck_obj_buffer(स्थिरांक संरचना object_id *oid, गणना object_type type,
			   अचिन्हित दीर्घ size, शून्य *buffer, पूर्णांक *eaten, शून्य *cb_data)
{
	संरचना repository *repo = cb_data;
	संरचना object *obj;
	पूर्णांक err;

	/*
	 * Note, buffer may be NULL if type is OBJ_BLOB. See
	 * verify_packfile(), data_valid variable for details.
	 */
	obj = parse_object_buffer(repo, oid, type, size, buffer, eaten);
	जर (!obj) {
		errors_found |= ERROR_OBJECT;
		err = error(_("%s: object corrupt or missing"),
			    oid_to_hex(oid));
		जा out;
	}

	obj->flags &= ~REACHABLE;
	obj->flags |= HAS_OBJ | SEEN;

	जर (verbose)
		fprintf_ln(मानक_त्रुटि, _("Checking %s %s"),
			   printable_type(repo, &obj->oid, obj->type),
			   describe_object(&obj->oid));

	जर (fsck_walk(obj, रिक्त, &fsck_obj_options))
		objerror(repo, obj, _("broken links"));

	err = fsck_object(obj, buffer, size, &fsck_obj_options);
	जर (err)
		जा out;

	जर (obj->type == OBJ_COMMIT) {
		संरचना commit *commit = (संरचना commit *) obj;

		जर (!commit->parents && show_मूळ)
			printf_ln(_("root %s"),
				  describe_object(&commit->object.oid));
	}

	जर (obj->type == OBJ_TAG) {
		संरचना tag *tag = (संरचना tag *) obj;

		जर (show_tags && tag->tagged) {
			printf_ln(_("tagged %s %s (%s) in %s"),
				  printable_type(repo, &tag->tagged->oid, tag->tagged->type),
				  describe_object(&tag->tagged->oid),
				  tag->tag,
				  describe_object(&tag->object.oid));
		}
	}

out:
	जर (obj && obj->type == OBJ_वृक्ष)
		free_वृक्ष_buffer((संरचना वृक्ष *)obj);
	परत err;
}

स्थिर पूर्णांक default_refs;

स्थिर शून्य fsck_handle_reflog_oid(संरचना repository *repo,
				   स्थिरांक अक्षर *refname, संरचना object_id *oid,
				   timestamp_t timestamp)
{
	संरचना object *obj;

	जर (!is_null_oid(oid)) {
		obj = lookup_object(repo, oid);
		जर (obj && (obj->flags & HAS_OBJ)) {
			जर (timestamp)
				fsck_put_object_name(&fsck_walk_options, oid,
						     "%s@{%"PRItime"}",
						     refname, timestamp);
			obj->flags |= USED;
			mark_object_reachable(obj);
		} अन्यथा जर (!is_promisor_object(repo, oid)) {
			error(_("%s: invalid reflog entry %s"),
			      refname, oid_to_hex(oid));
			errors_found |= ERROR_REACHABLE;
		}
	}
}

स्थिर पूर्णांक fsck_handle_reflog_ent(स्थिरांक अक्षर *refname,
				  संरचना object_id *ooid, संरचना object_id *noid,
				  स्थिरांक अक्षर *email UNUSED,
				  timestamp_t timestamp, पूर्णांक tz UNUSED,
				  स्थिरांक अक्षर *message UNUSED, शून्य *cb_data)
{
	संरचना repository *repo = cb_data;

	जर (now && timestamp > now)
		परत 0;

	जर (verbose)
		fprintf_ln(मानक_त्रुटि, _("Checking reflog %s->%s"),
			   oid_to_hex(ooid), oid_to_hex(noid));

	fsck_handle_reflog_oid(repo, refname, ooid, 0);
	fsck_handle_reflog_oid(repo, refname, noid, timestamp);
	परत 0;
}

स्थिर पूर्णांक fsck_handle_reflog(स्थिरांक अक्षर *logname, शून्य *cb_data)
{
	संरचना strbuf refname = STRBUF_INIT;
	संरचना worktree *wt = cb_data;

	strbuf_worktree_ref(wt, &refname, logname);
	refs_for_each_reflog_ent(get_main_ref_store(wt->repo),
				 refname.buf, fsck_handle_reflog_ent,
				 wt->repo);
	strbuf_release(&refname);
	परत 0;
}

संरचना ref_snapshot {
	अक्षर *refname;
	संरचना object_id oid;
	/* TODO: Maybe supplement with latest reflog entry info too? */
};

संरचना snapshot {
	माप_प्रकार nr;
	माप_प्रकार alloc;
	संरचना ref_snapshot *ref;
	/* TODO: Consider also snapshotting the index of each worktree. */
};

संरचना snapshot_ref_data {
	संरचना repository *repo;
	संरचना snapshot *snap;
};

स्थिर पूर्णांक snapshot_ref(स्थिरांक संरचना reference *ref, शून्य *cb_data)
{
	संरचना snapshot_ref_data *data = cb_data;
	संरचना snapshot *snap = data->snap;
	संरचना object *obj;

	obj = parse_object(data->repo, ref->oid);
	जर (!obj) {
		जर (is_promisor_object(data->repo, ref->oid)) {
			/*
			 * Increment default_refs anyway, because this is a
			 * valid ref.
			 */
			 default_refs++;
			 परत 0;
		}
		error(_("%s: invalid sha1 pointer %s"),
		      ref->name, oid_to_hex(ref->oid));
		errors_found |= ERROR_REACHABLE;
		/* We'll continue with the rest despite the error.. */
		परत 0;
	}
	जर (obj->type != OBJ_COMMIT && is_branch(ref->name)) {
		error(_("%s: not a commit"), ref->name);
		errors_found |= ERROR_REFS;
	}
	default_refs++;

	ALLOC_GROW(snap->ref, snap->nr + 1, snap->alloc);
	snap->ref[snap->nr].refname = xstrdup(ref->name);
	oidcpy(&snap->ref[snap->nr].oid, ref->oid);
	snap->nr++;

	परत 0;
}

स्थिर पूर्णांक fsck_handle_ref(स्थिरांक संरचना reference *ref, शून्य *cb_data)
{
	संरचना repository *repo = cb_data;
	संरचना object *obj;

	obj = parse_object(repo, ref->oid);
	obj->flags |= USED;
	fsck_put_object_name(&fsck_walk_options,
			     ref->oid, "%s", ref->name);
	mark_object_reachable(obj);

	परत 0;
}

स्थिर शून्य snapshot_refs(संरचना repository *repo,
			  संरचना snapshot *snap, पूर्णांक argc, स्थिरांक अक्षर **argv)
{
	संरचना refs_for_each_ref_options opts = {
		.flags = REFS_FOR_EACH_INCLUDE_BROKEN,
	};
	संरचना snapshot_ref_data data = {
		.repo = repo,
		.snap = snap,
	};
	संरचना worktree **worktrees, **p;
	स्थिरांक अक्षर *head_points_at;
	संरचना object_id head_oid;

	साठी (पूर्णांक i = 0; i < argc; i++) {
		स्थिरांक अक्षर *arg = argv[i];
		संरचना object_id oid;
		जर (!repo_get_oid(repo, arg, &oid)) {
			संरचना reference ref = {
				.name = arg,
				.oid = &oid,
			};

			snapshot_ref(&ref, &data);
			सुरूठेवा;
		}
		error(_("invalid parameter: expected sha1, got '%s'"), arg);
		errors_found |= ERROR_OBJECT;
	}

	जर (argc) {
		include_reflogs = 0;
		परत;
	}

	refs_for_each_ref_ext(get_main_ref_store(repo),
			      snapshot_ref, &data, &opts);

	worktrees = get_worktrees(repo);
	साठी (p = worktrees; *p; p++) {
		संरचना worktree *wt = *p;
		संरचना strbuf refname = STRBUF_INIT;

		strbuf_worktree_ref(wt, &refname, "HEAD");

		head_points_at = refs_resolve_ref_unsafe(get_main_ref_store(repo),
							 refname.buf, 0, &head_oid, रिक्त);

		जर (head_points_at && !is_null_oid(&head_oid)) {
			संरचना reference ref = {
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
	now = समय(रिक्त);
}


स्थिर शून्य free_snapshot_refs(संरचना snapshot *snap)
{
	साठी (माप_प्रकार i = 0; i < snap->nr; i++)
		मुक्त(snap->ref[i].refname);
	मुक्त(snap->ref);
}

स्थिर शून्य process_refs(संरचना repository *repo, संरचना snapshot *snap)
{
	संरचना worktree **worktrees, **p;

	साठी (माप_प्रकार i = 0; i < snap->nr; i++) {
		संरचना reference ref = {
			.name = snap->ref[i].refname,
			.oid = &snap->ref[i].oid,
		};
		fsck_handle_ref(&ref, repo);
	}

	जर (include_reflogs) {
		worktrees = get_worktrees(repo);
		साठी (p = worktrees; *p; p++) {
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
	जर (!default_refs) {
		fprintf_ln(मानक_त्रुटि, _("notice: No default references"));
		show_unreachable = 0;
	}
}

स्थिर पूर्णांक fsck_cache_वृक्ष(संरचना repository *repo, संरचना cache_वृक्ष *it, स्थिरांक अक्षर *index_path)
{
	पूर्णांक i;
	पूर्णांक err = 0;

	जर (verbose)
		fprintf_ln(मानक_त्रुटि, _("Checking cache tree of %s"), index_path);

	जर (0 <= it->entry_मोजणी) {
		संरचना object *obj = parse_object(repo, &it->oid);
		जर (!obj) {
			error(_("%s: invalid sha1 pointer in cache-tree of %s"),
			      oid_to_hex(&it->oid), index_path);
			errors_found |= ERROR_REFS;
			परत 1;
		}
		obj->flags |= USED;
		fsck_put_object_name(&fsck_walk_options, &it->oid, ":");
		mark_object_reachable(obj);
		जर (obj->type != OBJ_वृक्ष)
			err |= objerror(repo, obj, _("non-tree in cache-tree"));
	}
	साठी (i = 0; i < it->subtree_nr; i++)
		err |= fsck_cache_वृक्ष(repo, it->down[i]->cache_वृक्ष, index_path);
	परत err;
}

स्थिर पूर्णांक fsck_resolve_undo(संरचना index_state *istate,
			     स्थिरांक अक्षर *index_path)
{
	संरचना string_list_item *item;
	संरचना string_list *resolve_undo = istate->resolve_undo;

	जर (!resolve_undo)
		परत 0;

	for_each_string_list_item(item, resolve_undo) {
		स्थिरांक अक्षर *path = item->string;
		संरचना resolve_undo_info *ru = item->util;
		पूर्णांक i;

		जर (!ru)
			सुरूठेवा;
		साठी (i = 0; i < 3; i++) {
			संरचना object *obj;

			जर (!ru->mode[i] || !S_ISREG(ru->mode[i]))
				सुरूठेवा;

			obj = parse_object(istate->repo, &ru->oid[i]);
			जर (!obj) {
				error(_("%s: invalid sha1 pointer in resolve-undo of %s"),
				      oid_to_hex(&ru->oid[i]),
				      index_path);
				errors_found |= ERROR_REFS;
				सुरूठेवा;
			}
			obj->flags |= USED;
			fsck_put_object_name(&fsck_walk_options, &ru->oid[i],
					     ":(%d):%s", i, path);
			mark_object_reachable(obj);
		}
	}
	परत 0;
}

स्थिर शून्य fsck_index(संरचना index_state *istate, स्थिरांक अक्षर *index_path,
		       पूर्णांक is_current_worktree)
{
	अचिन्हित पूर्णांक i;

	/* TODO: audit for interaction with sparse-index. */
	ensure_full_index(istate);
	साठी (i = 0; i < istate->cache_nr; i++) {
		अचिन्हित पूर्णांक mode;
		संरचना blob *blob;
		संरचना object *obj;

		mode = istate->cache[i]->ce_mode;
		जर (S_ISGITLINK(mode))
			सुरूठेवा;
		blob = lookup_blob(istate->repo,
				   &istate->cache[i]->oid);
		जर (!blob)
			सुरूठेवा;
		obj = &blob->object;
		obj->flags |= USED;
		fsck_put_object_name(&fsck_walk_options, &obj->oid,
				     "%s:%s",
				     is_current_worktree ? "" : index_path,
				     istate->cache[i]->name);
		mark_object_reachable(obj);
	}
	जर (istate->cache_वृक्ष)
		fsck_cache_वृक्ष(istate->repo, istate->cache_वृक्ष, index_path);
	fsck_resolve_undo(istate, index_path);
}

स्थिर पूर्णांक mark_object_for_connectivity(स्थिरांक संरचना object_id *oid,
					संरचना object_info *oi UNUSED,
					शून्य *cb_data)
{
	संरचना repository *repo = cb_data;
	संरचना object *obj = lookup_unknown_object(repo, oid);
	obj->flags |= HAS_OBJ;
	परत 0;
}

स्थिर शून्य fsck_refs(संरचना repository *r)
{
	संरचना child_process refs_verify = CHILD_PROCESS_INIT;
	संरचना progress *progress = रिक्त;

	जर (show_progress)
		progress = start_progress(r, _("Checking ref database"), 1);

	जर (verbose)
		fprintf_ln(मानक_त्रुटि, _("Checking ref database"));

	child_process_init(&refs_verify);
	refs_verify.git_cmd = 1;
	strvec_pushl(&refs_verify.args, "refs", "verify", रिक्त);
	जर (verbose)
		strvec_ढकला(&refs_verify.args, "--verbose");
	जर (check_strict)
		strvec_ढकला(&refs_verify.args, "--strict");

	जर (run_command(&refs_verify))
		errors_found |= ERROR_REFS;

	display_progress(progress, 1);
	stop_progress(&progress);
}

स्थिर अक्षर स्थिरांक * स्थिरांक fsck_usage[] = {
	N_("git fsck [--tags] [--root] [--unreachable] [--cache] [--no-reflogs]\n"
	   "         [--[no-]full] [--strict] [--verbose] [--lost-found]\n"
	   "         [--[no-]dangling] [--[no-]progress] [--connectivity-only]\n"
	   "         [--[no-]name-objects] [--[no-]references] [<object>...]"),
	रिक्त
};

पूर्णांक cmd_fsck(पूर्णांक argc,
	     स्थिरांक अक्षर **argv,
	     स्थिरांक अक्षर *prefix,
	     संरचना repository *repo)
{
	संरचना odb_fsck_options odb_fsck_opts = {
		.flags = ODB_FSCK_FULL,
		.object_cb = fsck_obj_buffer,
		.object_payload = repo,
	};
	पूर्णांक keep_cache_objects = 0;
	पूर्णांक name_objects = 0;
	पूर्णांक check_references = 1;
	संरचना option fsck_opts[] = {
		OPT__VERBOSE(&verbose, N_("be verbose")),
		OPT_BOOL(0, "unreachable", &show_unreachable, N_("show unreachable objects")),
		OPT_BOOL(0, "dangling", &show_dangling, N_("show dangling objects")),
		OPT_BOOL(0, "tags", &show_tags, N_("report tags")),
		OPT_BOOL(0, "root", &show_मूळ, N_("report root nodes")),
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
	संरचना snapshot snap = {
		.nr = 0,
		.alloc = 0,
		.ref = रिक्त
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
	जर (check_strict)
		fsck_obj_options.strict = 1;

	जर (show_progress == -1)
		show_progress = isatty(2);
	जर (verbose) {
		show_progress = 0;
		odb_fsck_opts.flags |= ODB_FSCK_VERBOSE;
	}
	जर (show_progress)
		odb_fsck_opts.flags |= ODB_FSCK_PROGRESS;

	जर (write_lost_and_found) {
		odb_fsck_opts.flags |= ODB_FSCK_FULL;
		include_reflogs = 0;
	}

	जर (name_objects)
		fsck_enable_object_names(&fsck_walk_options);

	repo_config(repo, git_fsck_config, &fsck_obj_options);
	prepare_repo_settings(repo);

	जर (check_references)
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

	जर (connectivity_only) {
		odb_for_each_object(repo->objects, रिक्त,
				    mark_object_for_connectivity, repo, 0);
	} अन्यथा {
		जर (odb_fsck(repo->objects, &odb_fsck_opts) < 0)
			errors_found |= ERROR_OBJECT;

		जर (fsck_finish(&fsck_obj_options))
			errors_found |= ERROR_OBJECT;
	}

	/* Process the snapshotted refs and the reflogs. */
	process_refs(repo, &snap);

	/* If not given any explicit objects, process index files too. */
	जर (!argc)
		keep_cache_objects = 1;
	जर (keep_cache_objects) {
		/*
		 * TODO: Consider first walking these indexes in snapshot_refs,
		 * to snapshot where the index entries used to point, and then
		 * check those snapshotted locations here.
		 */
		संरचना worktree **worktrees, **p;

		verify_index_checksum = 1;
		verify_ce_order = 1;

		worktrees = get_worktrees(repo);
		साठी (p = worktrees; *p; p++) {
			संरचना worktree *wt = *p;
			संरचना index_state istate =
				INDEX_STATE_INIT(repo);
			अक्षर *path, *wt_gitdir;

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
			मुक्त(wt_gitdir);
			मुक्त(path);
		}
		free_worktrees(worktrees);
	}

	check_connectivity(repo);

	जर (repo->settings.core_commit_graph) {
		संरचना odb_source_files *files = odb_source_files_downcast(repo->objects->source);
		संरचना child_process commit_graph_verify = CHILD_PROCESS_INIT;

		साठी (संरचना odb_files_dir *dir = files->dirs; dir; dir = dir->next) {
			child_process_init(&commit_graph_verify);
			commit_graph_verify.git_cmd = 1;
			strvec_pushl(&commit_graph_verify.args, "commit-graph",
				     "verify", "--object-dir", dir->abspath, रिक्त);
			जर (show_progress)
				strvec_ढकला(&commit_graph_verify.args, "--progress");
			अन्यथा
				strvec_ढकला(&commit_graph_verify.args, "--no-progress");
			जर (run_command(&commit_graph_verify))
				errors_found |= ERROR_COMMIT_GRAPH;
		}
	}

	free_snapshot_refs(&snap);
	परत errors_found;
}
