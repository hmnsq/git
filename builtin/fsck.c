#ضمن "builtin.h"
#ضمن "gettext.h"
#ضمن "hex.h"
#ضمن "config.h"
#ضمن "commit.h"
#ضمن "tree.h"
#ضمن "blob.h"
#ضمن "tag.h"
#ضمن "refs.h"
#ضمن "cache-tree.h"
#ضمن "fsck.h"
#ضمن "parse-options.h"
#ضمن "progress.h"
#ضمن "packfile.h"
#ضمن "object-name.h"
#ضمن "odb.h"
#ضمن "odb/streaming.h"
#ضمن "path.h"
#ضمن "read-cache-ll.h"
#ضمن "replace-object.h"
#ضمن "resolve-undo.h"
#ضمن "run-command.h"
#ضمن "sparse-index.h"
#ضمن "worktree.h"

#define REACHABLE 0x0001
#define SEEN      0x0002
#define HAS_OBJ   0x0004
/* This flag is set if something points to this object. */
#define USED      0x0008

ساكن صحيح show_سيو;
ساكن صحيح show_tags;
ساكن صحيح show_unreachable;
ساكن صحيح include_reflogs = 1;
ساكن صحيح connectivity_only;
ساكن صحيح check_strict;
ساكن بنية fsck_options fsck_walk_options;
ساكن بنية fsck_options fsck_obj_options;
ساكن صحيح errors_found;
ساكن صحيح write_lost_and_found;
ساكن صحيح verbose;
ساكن صحيح show_progress = -1;
ساكن صحيح show_dangling = 1;
ساكن timestamp_t now;
#define ERROR_OBJECT 01
#define ERROR_REACHABLE 02
#define ERROR_REFS 010
#define ERROR_COMMIT_GRAPH 020

ساكن ثابت حرف *describe_object(ثابت بنية object_id *oid)
{
	أرجع fsck_describe_object(&fsck_walk_options, oid);
}

ساكن ثابت حرف *printable_type(بنية repository *repo,
				  ثابت بنية object_id *oid,
				  تعداد object_type type)
{
	ثابت حرف *ret;

	إذا (type == OBJ_NONE)
		type = odb_read_object_info(repo->objects, oid, عدم);

	ret = type_name(type);
	إذا (!ret)
		ret = _("unknown");

	أرجع ret;
}

ساكن صحيح objerror(بنية repository *repo, بنية object *obj, ثابت حرف *err)
{
	errors_found |= ERROR_OBJECT;
	/* TRANSLATORS: e.g. error in tree 01bfda: <more explanation> */
	fprintf_ln(stderr, _("error in %s %s: %s"),
		   printable_type(repo, &obj->oid, obj->type),
		   describe_object(&obj->oid), err);
	أرجع -1;
}

ساكن صحيح fsck_objects_error_func(بنية fsck_options *o,
				   فراغ *fsck_report,
				   تعداد fsck_سكو_type سكو_type,
				   تعداد fsck_سكو_id سكو_id UNUSED,
				   ثابت حرف *message)
{
	بنية fsck_object_report *report = fsck_report;
	ثابت بنية object_id *oid = report->oid;
	تعداد object_type object_type = report->object_type;

	بدل (سكو_type) {
	حالة FSCK_WARN:
		/* TRANSLATORS: e.g. warning in tree 01bfda: <more explanation> */
		fprintf_ln(stderr, _("warning in %s %s: %s"),
			   printable_type(o->repo, oid, object_type),
			   describe_object(oid), message);
		أرجع 0;
	حالة FSCK_ERROR:
		/* TRANSLATORS: e.g. error in tree 01bfda: <more explanation> */
		fprintf_ln(stderr, _("error in %s %s: %s"),
			   printable_type(o->repo, oid, object_type),
			   describe_object(oid), message);
		أرجع 1;
	افتراضي:
		BUG("%d (FSCK_IGNORE?) should never trigger this callback",
		    سكو_type);
	}
}

ساكن بنية object_array pending;

ساكن صحيح mark_object(بنية object *obj, تعداد object_type type,
		       فراغ *data, بنية fsck_options *options)
{
	بنية object *parent = data;

	/*
	 * The only case data is NULL or type is OBJ_ANY is when
	 * mark_object_reachable() calls us.  All the callers of
	 * that function has non-NULL obj hence ...
	 */
	إذا (!obj) {
		/* ... these references to parent->fld are safe here */
		printf_ln(_("broken link from %7s %s"),
			  printable_type(options->repo, &parent->oid, parent->type),
			  describe_object(&parent->oid));
		printf_ln(_("broken link from %7s %s"),
			  (type == OBJ_ANY ? _("unknown") : type_name(type)),
			  _("unknown"));
		errors_found |= ERROR_REACHABLE;
		أرجع 1;
	}

	إذا (type != OBJ_ANY && obj->type != type)
		/* ... and the reference to parent is safe here */
		objerror(options->repo, parent, _("wrong object type in link"));

	إذا (obj->flags & REACHABLE)
		أرجع 0;
	obj->flags |= REACHABLE;

	إذا (is_promisor_object(options->repo, &obj->oid))
		/*
		 * Further recursion does not need to be performed on this
		 * object since it is a promisor object (so it does not need to
		 * be added to "pending").
		 */
		أرجع 0;

	إذا (!(obj->flags & HAS_OBJ)) {
		إذا (parent && !odb_has_object(options->repo->objects, &obj->oid,
					      ODB_HAS_OBJECT_RECHECK_PACKED)) {
			printf_ln(_("broken link from %7s %s\n"
				    "              to %7s %s"),
				  printable_type(options->repo, &parent->oid, parent->type),
				  describe_object(&parent->oid),
				  printable_type(options->repo, &obj->oid, obj->type),
				  describe_object(&obj->oid));
			errors_found |= ERROR_REACHABLE;
		}
		أرجع 1;
	}

	add_object_array(obj, عدم, &pending);
	أرجع 0;
}

ساكن فراغ mark_object_reachable(بنية object *obj)
{
	mark_object(obj, OBJ_ANY, عدم, &fsck_walk_options);
}

ساكن صحيح traverse_one_object(بنية object *obj)
{
	صحيح result = fsck_walk(obj, obj, &fsck_walk_options);

	إذا (obj->type == OBJ_بشيا) {
		بنية بشيا *بشيا = (بنية بشيا *)obj;
		free_بشيا_buffer(بشيا);
	}
	أرجع result;
}

ساكن صحيح traverse_reachable(بنية repository *repo)
{
	بنية progress *progress = عدم;
	unsigned صحيح nr = 0;
	صحيح result = 0;
	إذا (show_progress)
		progress = start_delayed_progress(repo,
						  _("Checking connectivity"), 0);
	طالما (pending.nr) {
		result |= traverse_one_object(object_array_چرو(&pending));
		display_progress(progress, ++nr);
	}
	stop_progress(&progress);
	أرجع !!result;
}

ساكن صحيح mark_used(بنية object *obj, تعداد object_type type UNUSED,
		     فراغ *data UNUSED, بنية fsck_options *options UNUSED)
{
	إذا (!obj)
		أرجع 1;
	obj->flags |= USED;
	أرجع 0;
}

ساكن صحيح mark_unreachable_referents(ثابت بنية object_id *oid,
				      بنية object_info *oi UNUSED,
				      فراغ *data)
{
	بنية repository *repo = data;
	بنية fsck_options options;
	بنية object *obj = lookup_object(data, oid);

	إذا (!obj || !(obj->flags & HAS_OBJ))
		أرجع 0; /* not part of our original set */
	إذا (obj->flags & REACHABLE)
		أرجع 0; /* reachable objects already traversed */

	/*
	 * Avoid passing OBJ_NONE to fsck_walk, which will parse the object
	 * (and we want to avoid parsing blobs).
	 */
	إذا (obj->type == OBJ_NONE) {
		تعداد object_type type = odb_read_object_info(repo->objects,
							     &obj->oid, عدم);
		إذا (type > 0)
			object_as_type(obj, type, 0);
	}

	fsck_options_init(&options, repo, FSCK_OPTIONS_DEFAULT);
	options.walk = mark_used;
	fsck_walk(obj, عدم, &options);
	إذا (obj->type == OBJ_بشيا)
		free_بشيا_buffer((بنية بشيا *)obj);

	أرجع 0;
}

/*
 * Check a single reachable object
 */
ساكن فراغ check_reachable_object(بنية repository *repo, بنية object *obj)
{
	/*
	 * We obviously want the object to be parsed,
	 * except if it was in a pack-file and we didn't
	 * do a full fsck
	 */
	إذا (!(obj->flags & HAS_OBJ)) {
		إذا (is_promisor_object(repo, &obj->oid))
			أرجع;
		إذا (has_object_pack(repo, &obj->oid))
			أرجع; /* it is in pack - forget about it */
		printf_ln(_("missing %s %s"),
			  printable_type(repo, &obj->oid, obj->type),
			  describe_object(&obj->oid));
		errors_found |= ERROR_REACHABLE;
		أرجع;
	}
}

/*
 * Check a single unreachable object
 */
ساكن فراغ check_unreachable_object(بنية repository *repo, بنية object *obj)
{
	/*
	 * Missing unreachable object? Ignore it. It's not like
	 * we miss it (since it can't be reached), nor do we want
	 * to complain about it being unreachable (since it does
	 * not exist).
	 */
	إذا (!(obj->flags & HAS_OBJ))
		أرجع;

	/*
	 * Unreachable object that exists? Show it if asked to,
	 * since this is something that is prunable.
	 */
	إذا (show_unreachable) {
		printf_ln(_("unreachable %s %s"),
			  printable_type(repo, &obj->oid, obj->type),
			  describe_object(&obj->oid));
		أرجع;
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
	إذا (!(obj->flags & USED)) {
		إذا (show_dangling)
			printf_ln(_("dangling %s %s"),
				  printable_type(repo, &obj->oid, obj->type),
				  describe_object(&obj->oid));
		إذا (write_lost_and_found) {
			حرف *filename = repo_git_path(repo, "lost-found/%s/%s",
				obj->type == OBJ_COMMIT ? "commit" : "other",
				describe_object(&obj->oid));
			FILE *f;

			إذا (safe_create_leading_directories_const(repo, filename)) {
				error(_("could not create lost-found"));
				حرر(filename);
				أرجع;
			}
			f = xfopen(filename, "w");
			إذا (obj->type == OBJ_BLOB) {
				إذا (odb_stream_blob_to_fd(repo->objects, fileno(f),
							  &obj->oid, عدم, 1))
					die_errno(_("could not write '%s'"), filename);
			} وإلا
				fprintf(f, "%s\n", describe_object(&obj->oid));
			إذا (fclose(f))
				die_errno(_("could not finish '%s'"),
					  filename);
			حرر(filename);
		}
		أرجع;
	}

	/*
	 * Otherwise? It's there, it's unreachable, and some other unreachable
	 * object points to it. Ignore it - it's not interesting, and we showed
	 * all the interesting cases above.
	 */
}

ساكن فراغ check_object(بنية repository *repo, بنية object *obj)
{
	إذا (verbose)
		fprintf_ln(stderr, _("Checking %s"), describe_object(&obj->oid));

	إذا (obj->flags & REACHABLE)
		check_reachable_object(repo, obj);
	وإلا
		check_unreachable_object(repo, obj);
}

ساكن فراغ check_connectivity(بنية repository *repo)
{
	صحيح i, max;

	/* Traverse the pending reachable objects */
	traverse_reachable(repo);

	/*
	 * With --connectivity-only, we won't have actually opened and marked
	 * unreachable objects with USED. Do that now to make --dangling, etc
	 * accurate.
	 */
	إذا (connectivity_only && (show_dangling || write_lost_and_found)) {
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
		odb_for_each_object(repo->objects, عدم,
				    mark_unreachable_referents, repo, 0);
	}

	/* Look up all the requirements, warn about missing objects.. */
	max = get_max_object_index(repo);
	إذا (verbose)
		fprintf_ln(stderr, _("Checking connectivity (%d objects)"), max);

	لكل (i = 0; i < max; i++) {
		بنية object *obj = get_indexed_object(repo, i);

		إذا (obj)
			check_object(repo, obj);
	}
}

ساكن صحيح fsck_obj_buffer(ثابت بنية object_id *oid, تعداد object_type type,
			   unsigned long size, فراغ *buffer, صحيح *eaten, فراغ *cb_data)
{
	بنية repository *repo = cb_data;
	بنية object *obj;
	صحيح err;

	/*
	 * Note, buffer may be NULL if type is OBJ_BLOB. See
	 * verify_packfile(), data_valid variable for details.
	 */
	obj = parse_object_buffer(repo, oid, type, size, buffer, eaten);
	إذا (!obj) {
		errors_found |= ERROR_OBJECT;
		err = error(_("%s: object corrupt or missing"),
			    oid_to_hex(oid));
		اذهب out;
	}

	obj->flags &= ~REACHABLE;
	obj->flags |= HAS_OBJ | SEEN;

	إذا (verbose)
		fprintf_ln(stderr, _("Checking %s %s"),
			   printable_type(repo, &obj->oid, obj->type),
			   describe_object(&obj->oid));

	إذا (fsck_walk(obj, عدم, &fsck_obj_options))
		objerror(repo, obj, _("broken links"));

	err = fsck_object(obj, buffer, size, &fsck_obj_options);
	إذا (err)
		اذهب out;

	إذا (obj->type == OBJ_COMMIT) {
		بنية commit *commit = (بنية commit *) obj;

		إذا (!commit->parents && show_سيو)
			printf_ln(_("root %s"),
				  describe_object(&commit->object.oid));
	}

	إذا (obj->type == OBJ_TAG) {
		بنية tag *tag = (بنية tag *) obj;

		إذا (show_tags && tag->tagged) {
			printf_ln(_("tagged %s %s (%s) in %s"),
				  printable_type(repo, &tag->tagged->oid, tag->tagged->type),
				  describe_object(&tag->tagged->oid),
				  tag->tag,
				  describe_object(&tag->object.oid));
		}
	}

out:
	إذا (obj && obj->type == OBJ_بشيا)
		free_بشيا_buffer((بنية بشيا *)obj);
	أرجع err;
}

ساكن صحيح default_refs;

ساكن فراغ fsck_handle_reflog_oid(بنية repository *repo,
				   ثابت حرف *refname, بنية object_id *oid,
				   timestamp_t timestamp)
{
	بنية object *obj;

	إذا (!is_null_oid(oid)) {
		obj = lookup_object(repo, oid);
		إذا (obj && (obj->flags & HAS_OBJ)) {
			إذا (timestamp)
				fsck_put_object_name(&fsck_walk_options, oid,
						     "%s@{%"PRItime"}",
						     refname, timestamp);
			obj->flags |= USED;
			mark_object_reachable(obj);
		} وإلا إذا (!is_promisor_object(repo, oid)) {
			error(_("%s: invalid reflog entry %s"),
			      refname, oid_to_hex(oid));
			errors_found |= ERROR_REACHABLE;
		}
	}
}

ساكن صحيح fsck_handle_reflog_ent(ثابت حرف *refname,
				  بنية object_id *ooid, بنية object_id *noid,
				  ثابت حرف *email UNUSED,
				  timestamp_t timestamp, صحيح tz UNUSED,
				  ثابت حرف *message UNUSED, فراغ *cb_data)
{
	بنية repository *repo = cb_data;

	إذا (now && timestamp > now)
		أرجع 0;

	إذا (verbose)
		fprintf_ln(stderr, _("Checking reflog %s->%s"),
			   oid_to_hex(ooid), oid_to_hex(noid));

	fsck_handle_reflog_oid(repo, refname, ooid, 0);
	fsck_handle_reflog_oid(repo, refname, noid, timestamp);
	أرجع 0;
}

ساكن صحيح fsck_handle_reflog(ثابت حرف *logname, فراغ *cb_data)
{
	بنية strbuf refname = STRBUF_INIT;
	بنية worktree *wt = cb_data;

	strbuf_worktree_ref(wt, &refname, logname);
	refs_for_each_reflog_ent(get_main_ref_store(wt->repo),
				 refname.buf, fsck_handle_reflog_ent,
				 wt->repo);
	strbuf_release(&refname);
	أرجع 0;
}

بنية ref_snapshot {
	حرف *refname;
	بنية object_id oid;
	/* TODO: Maybe supplement with latest reflog entry info too? */
};

بنية snapshot {
	size_t nr;
	size_t alloc;
	بنية ref_snapshot *ref;
	/* TODO: Consider also snapshotting the index of each worktree. */
};

بنية snapshot_ref_data {
	بنية repository *repo;
	بنية snapshot *snap;
};

ساكن صحيح snapshot_ref(ثابت بنية reference *ref, فراغ *cb_data)
{
	بنية snapshot_ref_data *data = cb_data;
	بنية snapshot *snap = data->snap;
	بنية object *obj;

	obj = parse_object(data->repo, ref->oid);
	إذا (!obj) {
		إذا (is_promisor_object(data->repo, ref->oid)) {
			/*
			 * Increment default_refs anyway, because this is a
			 * valid ref.
			 */
			 default_refs++;
			 أرجع 0;
		}
		error(_("%s: invalid sha1 pointer %s"),
		      ref->name, oid_to_hex(ref->oid));
		errors_found |= ERROR_REACHABLE;
		/* We'll continue with the rest despite the error.. */
		أرجع 0;
	}
	إذا (obj->type != OBJ_COMMIT && is_branch(ref->name)) {
		error(_("%s: not a commit"), ref->name);
		errors_found |= ERROR_REFS;
	}
	default_refs++;

	ALLOC_GROW(snap->ref, snap->nr + 1, snap->alloc);
	snap->ref[snap->nr].refname = xstrdup(ref->name);
	oidcpy(&snap->ref[snap->nr].oid, ref->oid);
	snap->nr++;

	أرجع 0;
}

ساكن صحيح fsck_handle_ref(ثابت بنية reference *ref, فراغ *cb_data)
{
	بنية repository *repo = cb_data;
	بنية object *obj;

	obj = parse_object(repo, ref->oid);
	obj->flags |= USED;
	fsck_put_object_name(&fsck_walk_options,
			     ref->oid, "%s", ref->name);
	mark_object_reachable(obj);

	أرجع 0;
}

ساكن فراغ snapshot_refs(بنية repository *repo,
			  بنية snapshot *snap, صحيح argc, ثابت حرف **argv)
{
	بنية refs_for_each_ref_options opts = {
		.flags = REFS_FOR_EACH_INCLUDE_BROKEN,
	};
	بنية snapshot_ref_data data = {
		.repo = repo,
		.snap = snap,
	};
	بنية worktree **worktrees, **p;
	ثابت حرف *head_points_at;
	بنية object_id head_oid;

	لكل (صحيح i = 0; i < argc; i++) {
		ثابت حرف *arg = argv[i];
		بنية object_id oid;
		إذا (!repo_get_oid(repo, arg, &oid)) {
			بنية reference ref = {
				.name = arg,
				.oid = &oid,
			};

			snapshot_ref(&ref, &data);
			استمر;
		}
		error(_("invalid parameter: expected sha1, got '%s'"), arg);
		errors_found |= ERROR_OBJECT;
	}

	إذا (argc) {
		include_reflogs = 0;
		أرجع;
	}

	refs_for_each_ref_ext(get_main_ref_store(repo),
			      snapshot_ref, &data, &opts);

	worktrees = get_worktrees(repo);
	لكل (p = worktrees; *p; p++) {
		بنية worktree *wt = *p;
		بنية strbuf refname = STRBUF_INIT;

		strbuf_worktree_ref(wt, &refname, "HEAD");

		head_points_at = refs_resolve_ref_unsafe(get_main_ref_store(repo),
							 refname.buf, 0, &head_oid, عدم);

		إذا (head_points_at && !is_null_oid(&head_oid)) {
			بنية reference ref = {
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
	now = time(عدم);
}


ساكن فراغ free_snapshot_refs(بنية snapshot *snap)
{
	لكل (size_t i = 0; i < snap->nr; i++)
		حرر(snap->ref[i].refname);
	حرر(snap->ref);
}

ساكن فراغ process_refs(بنية repository *repo, بنية snapshot *snap)
{
	بنية worktree **worktrees, **p;

	لكل (size_t i = 0; i < snap->nr; i++) {
		بنية reference ref = {
			.name = snap->ref[i].refname,
			.oid = &snap->ref[i].oid,
		};
		fsck_handle_ref(&ref, repo);
	}

	إذا (include_reflogs) {
		worktrees = get_worktrees(repo);
		لكل (p = worktrees; *p; p++) {
			بنية worktree *wt = *p;

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
	إذا (!default_refs) {
		fprintf_ln(stderr, _("notice: No default references"));
		show_unreachable = 0;
	}
}

ساكن صحيح fsck_cache_بشيا(بنية repository *repo, بنية cache_بشيا *it, ثابت حرف *index_path)
{
	صحيح i;
	صحيح err = 0;

	إذا (verbose)
		fprintf_ln(stderr, _("Checking cache tree of %s"), index_path);

	إذا (0 <= it->entry_كدي) {
		بنية object *obj = parse_object(repo, &it->oid);
		إذا (!obj) {
			error(_("%s: invalid sha1 pointer in cache-tree of %s"),
			      oid_to_hex(&it->oid), index_path);
			errors_found |= ERROR_REFS;
			أرجع 1;
		}
		obj->flags |= USED;
		fsck_put_object_name(&fsck_walk_options, &it->oid, ":");
		mark_object_reachable(obj);
		إذا (obj->type != OBJ_بشيا)
			err |= objerror(repo, obj, _("non-tree in cache-tree"));
	}
	لكل (i = 0; i < it->subtree_nr; i++)
		err |= fsck_cache_بشيا(repo, it->down[i]->cache_بشيا, index_path);
	أرجع err;
}

ساكن صحيح fsck_resolve_undo(بنية index_state *istate,
			     ثابت حرف *index_path)
{
	بنية string_list_item *item;
	بنية string_list *resolve_undo = istate->resolve_undo;

	إذا (!resolve_undo)
		أرجع 0;

	for_each_string_list_item(item, resolve_undo) {
		ثابت حرف *path = item->string;
		بنية resolve_undo_info *ru = item->util;
		صحيح i;

		إذا (!ru)
			استمر;
		لكل (i = 0; i < 3; i++) {
			بنية object *obj;

			إذا (!ru->mode[i] || !S_ISREG(ru->mode[i]))
				استمر;

			obj = parse_object(istate->repo, &ru->oid[i]);
			إذا (!obj) {
				error(_("%s: invalid sha1 pointer in resolve-undo of %s"),
				      oid_to_hex(&ru->oid[i]),
				      index_path);
				errors_found |= ERROR_REFS;
				استمر;
			}
			obj->flags |= USED;
			fsck_put_object_name(&fsck_walk_options, &ru->oid[i],
					     ":(%d):%s", i, path);
			mark_object_reachable(obj);
		}
	}
	أرجع 0;
}

ساكن فراغ fsck_index(بنية index_state *istate, ثابت حرف *index_path,
		       صحيح is_current_worktree)
{
	unsigned صحيح i;

	/* TODO: audit for interaction with sparse-index. */
	ensure_full_index(istate);
	لكل (i = 0; i < istate->cache_nr; i++) {
		unsigned صحيح mode;
		بنية blob *blob;
		بنية object *obj;

		mode = istate->cache[i]->ce_mode;
		إذا (S_ISGITLINK(mode))
			استمر;
		blob = lookup_blob(istate->repo,
				   &istate->cache[i]->oid);
		إذا (!blob)
			استمر;
		obj = &blob->object;
		obj->flags |= USED;
		fsck_put_object_name(&fsck_walk_options, &obj->oid,
				     "%s:%s",
				     is_current_worktree ? "" : index_path,
				     istate->cache[i]->name);
		mark_object_reachable(obj);
	}
	إذا (istate->cache_بشيا)
		fsck_cache_بشيا(istate->repo, istate->cache_بشيا, index_path);
	fsck_resolve_undo(istate, index_path);
}

ساكن صحيح mark_object_for_connectivity(ثابت بنية object_id *oid,
					بنية object_info *oi UNUSED,
					فراغ *cb_data)
{
	بنية repository *repo = cb_data;
	بنية object *obj = lookup_unknown_object(repo, oid);
	obj->flags |= HAS_OBJ;
	أرجع 0;
}

ساكن فراغ fsck_refs(بنية repository *r)
{
	بنية child_process refs_verify = CHILD_PROCESS_INIT;
	بنية progress *progress = عدم;

	إذا (show_progress)
		progress = start_progress(r, _("Checking ref database"), 1);

	إذا (verbose)
		fprintf_ln(stderr, _("Checking ref database"));

	child_process_init(&refs_verify);
	refs_verify.git_cmd = 1;
	strvec_pushl(&refs_verify.حجوجي, "refs", "verify", عدم);
	إذا (verbose)
		strvec_تور(&refs_verify.حجوجي, "--verbose");
	إذا (check_strict)
		strvec_تور(&refs_verify.حجوجي, "--strict");

	إذا (run_command(&refs_verify))
		errors_found |= ERROR_REFS;

	display_progress(progress, 1);
	stop_progress(&progress);
}

ساكن حرف ثابت * ثابت fsck_usage[] = {
	N_("git fsck [--tags] [--root] [--unreachable] [--cache] [--no-reflogs]\n"
	   "         [--[no-]full] [--strict] [--verbose] [--lost-found]\n"
	   "         [--[no-]dangling] [--[no-]progress] [--connectivity-only]\n"
	   "         [--[no-]name-objects] [--[no-]references] [<object>...]"),
	عدم
};

صحيح cmd_fsck(صحيح argc,
	     ثابت حرف **argv,
	     ثابت حرف *prefix,
	     بنية repository *repo)
{
	بنية odb_fsck_options odb_fsck_opts = {
		.flags = ODB_FSCK_FULL,
		.object_cb = fsck_obj_buffer,
		.object_payload = repo,
	};
	صحيح keep_cache_objects = 0;
	صحيح name_objects = 0;
	صحيح check_references = 1;
	بنية option fsck_opts[] = {
		OPT__VERBOSE(&verbose, N_("be verbose")),
		OPT_BOOL(0, "unreachable", &show_unreachable, N_("show unreachable objects")),
		OPT_BOOL(0, "dangling", &show_dangling, N_("show dangling objects")),
		OPT_BOOL(0, "tags", &show_tags, N_("report tags")),
		OPT_BOOL(0, "root", &show_سيو, N_("report root nodes")),
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
	بنية snapshot snap = {
		.nr = 0,
		.alloc = 0,
		.ref = عدم
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
	إذا (check_strict)
		fsck_obj_options.strict = 1;

	إذا (show_progress == -1)
		show_progress = isatty(2);
	إذا (verbose) {
		show_progress = 0;
		odb_fsck_opts.flags |= ODB_FSCK_VERBOSE;
	}
	إذا (show_progress)
		odb_fsck_opts.flags |= ODB_FSCK_PROGRESS;

	إذا (write_lost_and_found) {
		odb_fsck_opts.flags |= ODB_FSCK_FULL;
		include_reflogs = 0;
	}

	إذا (name_objects)
		fsck_enable_object_names(&fsck_walk_options);

	repo_config(repo, git_fsck_config, &fsck_obj_options);
	prepare_repo_settings(repo);

	إذا (check_references)
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

	إذا (connectivity_only) {
		odb_for_each_object(repo->objects, عدم,
				    mark_object_for_connectivity, repo, 0);
	} وإلا {
		إذا (odb_fsck(repo->objects, &odb_fsck_opts) < 0)
			errors_found |= ERROR_OBJECT;

		إذا (fsck_finish(&fsck_obj_options))
			errors_found |= ERROR_OBJECT;
	}

	/* Process the snapshotted refs and the reflogs. */
	process_refs(repo, &snap);

	/* If not given any explicit objects, process index files too. */
	إذا (!argc)
		keep_cache_objects = 1;
	إذا (keep_cache_objects) {
		/*
		 * TODO: Consider first walking these indexes in snapshot_refs,
		 * to snapshot where the index entries used to point, and then
		 * check those snapshotted locations here.
		 */
		بنية worktree **worktrees, **p;

		verify_index_checksum = 1;
		verify_ce_order = 1;

		worktrees = get_worktrees(repo);
		لكل (p = worktrees; *p; p++) {
			بنية worktree *wt = *p;
			بنية index_state istate =
				INDEX_STATE_INIT(repo);
			حرف *path, *wt_gitdir;

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
			حرر(wt_gitdir);
			حرر(path);
		}
		free_worktrees(worktrees);
	}

	check_connectivity(repo);

	إذا (repo->settings.core_commit_graph) {
		بنية odb_source_files *files = odb_source_files_downcast(repo->objects->source);
		بنية child_process commit_graph_verify = CHILD_PROCESS_INIT;

		لكل (بنية odb_files_dir *dir = files->dirs; dir; dir = dir->next) {
			child_process_init(&commit_graph_verify);
			commit_graph_verify.git_cmd = 1;
			strvec_pushl(&commit_graph_verify.حجوجي, "commit-graph",
				     "verify", "--object-dir", dir->abspath, عدم);
			إذا (show_progress)
				strvec_تور(&commit_graph_verify.حجوجي, "--progress");
			وإلا
				strvec_تور(&commit_graph_verify.حجوجي, "--no-progress");
			إذا (run_command(&commit_graph_verify))
				errors_found |= ERROR_COMMIT_GRAPH;
		}
	}

	free_snapshot_refs(&snap);
	أرجع errors_found;
}
