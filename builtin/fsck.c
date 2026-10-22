#شامل "builtin.h"
#شامل "gettext.h"
#شامل "hex.h"
#شامل "config.h"
#شامل "commit.h"
#شامل "tree.h"
#شامل "blob.h"
#شامل "tag.h"
#شامل "refs.h"
#شامل "cache-tree.h"
#شامل "fsck.h"
#شامل "parse-options.h"
#شامل "progress.h"
#شامل "packfile.h"
#شامل "object-name.h"
#شامل "odb.h"
#شامل "odb/streaming.h"
#شامل "path.h"
#شامل "read-cache-ll.h"
#شامل "replace-object.h"
#شامل "resolve-undo.h"
#شامل "run-command.h"
#شامل "sparse-index.h"
#شامل "worktree.h"

#define REACHABLE 0x0001
#define SEEN      0x0002
#define HAS_OBJ   0x0004
/* This flag is set if something points to this object. */
#define USED      0x0008

ایستا صحیح show_ریشه;
ایستا صحیح show_tags;
ایستا صحیح show_unreachable;
ایستا صحیح include_reflogs = 1;
ایستا صحیح connectivity_only;
ایستا صحیح check_strict;
ایستا ساختار fsck_options fsck_walk_options;
ایستا ساختار fsck_options fsck_obj_options;
ایستا صحیح errors_found;
ایستا صحیح write_lost_and_found;
ایستا صحیح verbose;
ایستا صحیح show_progress = -1;
ایستا صحیح show_dangling = 1;
ایستا timestamp_t now;
#define ERROR_OBJECT 01
#define ERROR_REACHABLE 02
#define ERROR_REFS 010
#define ERROR_COMMIT_GRAPH 020

ایستا ثابت نویسه *describe_object(ثابت ساختار object_id *oid)
{
	بازگشت fsck_describe_object(&fsck_walk_options, oid);
}

ایستا ثابت نویسه *printable_type(ساختار repository *repo,
				  ثابت ساختار object_id *oid,
				  شمارش object_type type)
{
	ثابت نویسه *ret;

	اگر (type == OBJ_NONE)
		type = odb_read_object_info(repo->objects, oid, پوچ);

	ret = type_name(type);
	اگر (!ret)
		ret = _("unknown");

	بازگشت ret;
}

ایستا صحیح objerror(ساختار repository *repo, ساختار object *obj, ثابت نویسه *err)
{
	errors_found |= ERROR_OBJECT;
	/* TRANSLATORS: e.g. error in tree 01bfda: <more explanation> */
	fprintf_ln(stderr, _("error in %s %s: %s"),
		   printable_type(repo, &obj->oid, obj->type),
		   describe_object(&obj->oid), err);
	بازگشت -1;
}

ایستا صحیح fsck_objects_error_func(ساختار fsck_options *o,
				   تهی *fsck_report,
				   شمارش fsck_پیام_type پیام_type,
				   شمارش fsck_پیام_id پیام_id UNUSED,
				   ثابت نویسه *message)
{
	ساختار fsck_object_report *report = fsck_report;
	ثابت ساختار object_id *oid = report->oid;
	شمارش object_type object_type = report->object_type;

	انتخاب (پیام_type) {
	حالت FSCK_WARN:
		/* TRANSLATORS: e.g. warning in tree 01bfda: <more explanation> */
		fprintf_ln(stderr, _("warning in %s %s: %s"),
			   printable_type(o->repo, oid, object_type),
			   describe_object(oid), message);
		بازگشت 0;
	حالت FSCK_ERROR:
		/* TRANSLATORS: e.g. error in tree 01bfda: <more explanation> */
		fprintf_ln(stderr, _("error in %s %s: %s"),
			   printable_type(o->repo, oid, object_type),
			   describe_object(oid), message);
		بازگشت 1;
	پیشفرض:
		BUG("%d (FSCK_IGNORE?) should never trigger this callback",
		    پیام_type);
	}
}

ایستا ساختار object_array pending;

ایستا صحیح mark_object(ساختار object *obj, شمارش object_type type,
		       تهی *data, ساختار fsck_options *options)
{
	ساختار object *parent = data;

	/*
	 * The only case data is NULL or type is OBJ_ANY is when
	 * mark_object_reachable() calls us.  All the callers of
	 * that function has non-NULL obj hence ...
	 */
	اگر (!obj) {
		/* ... these references to parent->fld are safe here */
		printf_ln(_("broken link from %7s %s"),
			  printable_type(options->repo, &parent->oid, parent->type),
			  describe_object(&parent->oid));
		printf_ln(_("broken link from %7s %s"),
			  (type == OBJ_ANY ? _("unknown") : type_name(type)),
			  _("unknown"));
		errors_found |= ERROR_REACHABLE;
		بازگشت 1;
	}

	اگر (type != OBJ_ANY && obj->type != type)
		/* ... and the reference to parent is safe here */
		objerror(options->repo, parent, _("wrong object type in link"));

	اگر (obj->flags & REACHABLE)
		بازگشت 0;
	obj->flags |= REACHABLE;

	اگر (is_promisor_object(options->repo, &obj->oid))
		/*
		 * Further recursion does not need to be performed on this
		 * object since it is a promisor object (so it does not need to
		 * be added to "pending").
		 */
		بازگشت 0;

	اگر (!(obj->flags & HAS_OBJ)) {
		اگر (parent && !odb_has_object(options->repo->objects, &obj->oid,
					      ODB_HAS_OBJECT_RECHECK_PACKED)) {
			printf_ln(_("broken link from %7s %s\n"
				    "              to %7s %s"),
				  printable_type(options->repo, &parent->oid, parent->type),
				  describe_object(&parent->oid),
				  printable_type(options->repo, &obj->oid, obj->type),
				  describe_object(&obj->oid));
			errors_found |= ERROR_REACHABLE;
		}
		بازگشت 1;
	}

	add_object_array(obj, پوچ, &pending);
	بازگشت 0;
}

ایستا تهی mark_object_reachable(ساختار object *obj)
{
	mark_object(obj, OBJ_ANY, پوچ, &fsck_walk_options);
}

ایستا صحیح traverse_one_object(ساختار object *obj)
{
	صحیح result = fsck_walk(obj, obj, &fsck_walk_options);

	اگر (obj->type == OBJ_درخت) {
		ساختار درخت *درخت = (ساختار درخت *)obj;
		free_درخت_buffer(درخت);
	}
	بازگشت result;
}

ایستا صحیح traverse_reachable(ساختار repository *repo)
{
	ساختار progress *progress = پوچ;
	unsigned صحیح nr = 0;
	صحیح result = 0;
	اگر (show_progress)
		progress = start_delayed_progress(repo,
						  _("Checking connectivity"), 0);
	وقتی (pending.nr) {
		result |= traverse_one_object(object_array_برداشت(&pending));
		display_progress(progress, ++nr);
	}
	stop_progress(&progress);
	بازگشت !!result;
}

ایستا صحیح mark_used(ساختار object *obj, شمارش object_type type UNUSED,
		     تهی *data UNUSED, ساختار fsck_options *options UNUSED)
{
	اگر (!obj)
		بازگشت 1;
	obj->flags |= USED;
	بازگشت 0;
}

ایستا صحیح mark_unreachable_referents(ثابت ساختار object_id *oid,
				      ساختار object_info *oi UNUSED,
				      تهی *data)
{
	ساختار repository *repo = data;
	ساختار fsck_options options;
	ساختار object *obj = lookup_object(data, oid);

	اگر (!obj || !(obj->flags & HAS_OBJ))
		بازگشت 0; /* not part of our original set */
	اگر (obj->flags & REACHABLE)
		بازگشت 0; /* reachable objects already traversed */

	/*
	 * Avoid passing OBJ_NONE to fsck_walk, which will parse the object
	 * (and we want to avoid parsing blobs).
	 */
	اگر (obj->type == OBJ_NONE) {
		شمارش object_type type = odb_read_object_info(repo->objects,
							     &obj->oid, پوچ);
		اگر (type > 0)
			object_as_type(obj, type, 0);
	}

	fsck_options_init(&options, repo, FSCK_OPTIONS_DEFAULT);
	options.walk = mark_used;
	fsck_walk(obj, پوچ, &options);
	اگر (obj->type == OBJ_درخت)
		free_درخت_buffer((ساختار درخت *)obj);

	بازگشت 0;
}

/*
 * Check a single reachable object
 */
ایستا تهی check_reachable_object(ساختار repository *repo, ساختار object *obj)
{
	/*
	 * We obviously want the object to be parsed,
	 * except if it was in a pack-file and we didn't
	 * do a full fsck
	 */
	اگر (!(obj->flags & HAS_OBJ)) {
		اگر (is_promisor_object(repo, &obj->oid))
			بازگشت;
		اگر (has_object_pack(repo, &obj->oid))
			بازگشت; /* it is in pack - forget about it */
		printf_ln(_("missing %s %s"),
			  printable_type(repo, &obj->oid, obj->type),
			  describe_object(&obj->oid));
		errors_found |= ERROR_REACHABLE;
		بازگشت;
	}
}

/*
 * Check a single unreachable object
 */
ایستا تهی check_unreachable_object(ساختار repository *repo, ساختار object *obj)
{
	/*
	 * Missing unreachable object? Ignore it. It's not like
	 * we miss it (since it can't be reached), nor do we want
	 * to complain about it being unreachable (since it does
	 * not exist).
	 */
	اگر (!(obj->flags & HAS_OBJ))
		بازگشت;

	/*
	 * Unreachable object that exists? Show it if asked to,
	 * since this is something that is prunable.
	 */
	اگر (show_unreachable) {
		printf_ln(_("unreachable %s %s"),
			  printable_type(repo, &obj->oid, obj->type),
			  describe_object(&obj->oid));
		بازگشت;
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
	اگر (!(obj->flags & USED)) {
		اگر (show_dangling)
			printf_ln(_("dangling %s %s"),
				  printable_type(repo, &obj->oid, obj->type),
				  describe_object(&obj->oid));
		اگر (write_lost_and_found) {
			نویسه *filename = repo_git_path(repo, "lost-found/%s/%s",
				obj->type == OBJ_COMMIT ? "commit" : "other",
				describe_object(&obj->oid));
			FILE *f;

			اگر (safe_create_leading_directories_const(repo, filename)) {
				error(_("could not create lost-found"));
				آزاد(filename);
				بازگشت;
			}
			f = xfopen(filename, "w");
			اگر (obj->type == OBJ_BLOB) {
				اگر (odb_stream_blob_to_fd(repo->objects, fileno(f),
							  &obj->oid, پوچ, 1))
					die_errno(_("could not write '%s'"), filename);
			} وگرنه
				fprintf(f, "%s\n", describe_object(&obj->oid));
			اگر (fclose(f))
				die_errno(_("could not finish '%s'"),
					  filename);
			آزاد(filename);
		}
		بازگشت;
	}

	/*
	 * Otherwise? It's there, it's unreachable, and some other unreachable
	 * object points to it. Ignore it - it's not interesting, and we showed
	 * all the interesting cases above.
	 */
}

ایستا تهی check_object(ساختار repository *repo, ساختار object *obj)
{
	اگر (verbose)
		fprintf_ln(stderr, _("Checking %s"), describe_object(&obj->oid));

	اگر (obj->flags & REACHABLE)
		check_reachable_object(repo, obj);
	وگرنه
		check_unreachable_object(repo, obj);
}

ایستا تهی check_connectivity(ساختار repository *repo)
{
	صحیح i, max;

	/* Traverse the pending reachable objects */
	traverse_reachable(repo);

	/*
	 * With --connectivity-only, we won't have actually opened and marked
	 * unreachable objects with USED. Do that now to make --dangling, etc
	 * accurate.
	 */
	اگر (connectivity_only && (show_dangling || write_lost_and_found)) {
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
		odb_for_each_object(repo->objects, پوچ,
				    mark_unreachable_referents, repo, 0);
	}

	/* Look up all the requirements, warn about missing objects.. */
	max = get_max_object_index(repo);
	اگر (verbose)
		fprintf_ln(stderr, _("Checking connectivity (%d objects)"), max);

	برای (i = 0; i < max; i++) {
		ساختار object *obj = get_indexed_object(repo, i);

		اگر (obj)
			check_object(repo, obj);
	}
}

ایستا صحیح fsck_obj_buffer(ثابت ساختار object_id *oid, شمارش object_type type,
			   unsigned long size, تهی *buffer, صحیح *eaten, تهی *cb_data)
{
	ساختار repository *repo = cb_data;
	ساختار object *obj;
	صحیح err;

	/*
	 * Note, buffer may be NULL if type is OBJ_BLOB. See
	 * verify_packfile(), data_valid variable for details.
	 */
	obj = parse_object_buffer(repo, oid, type, size, buffer, eaten);
	اگر (!obj) {
		errors_found |= ERROR_OBJECT;
		err = error(_("%s: object corrupt or missing"),
			    oid_to_hex(oid));
		برو out;
	}

	obj->flags &= ~REACHABLE;
	obj->flags |= HAS_OBJ | SEEN;

	اگر (verbose)
		fprintf_ln(stderr, _("Checking %s %s"),
			   printable_type(repo, &obj->oid, obj->type),
			   describe_object(&obj->oid));

	اگر (fsck_walk(obj, پوچ, &fsck_obj_options))
		objerror(repo, obj, _("broken links"));

	err = fsck_object(obj, buffer, size, &fsck_obj_options);
	اگر (err)
		برو out;

	اگر (obj->type == OBJ_COMMIT) {
		ساختار commit *commit = (ساختار commit *) obj;

		اگر (!commit->parents && show_ریشه)
			printf_ln(_("root %s"),
				  describe_object(&commit->object.oid));
	}

	اگر (obj->type == OBJ_TAG) {
		ساختار tag *tag = (ساختار tag *) obj;

		اگر (show_tags && tag->tagged) {
			printf_ln(_("tagged %s %s (%s) in %s"),
				  printable_type(repo, &tag->tagged->oid, tag->tagged->type),
				  describe_object(&tag->tagged->oid),
				  tag->tag,
				  describe_object(&tag->object.oid));
		}
	}

out:
	اگر (obj && obj->type == OBJ_درخت)
		free_درخت_buffer((ساختار درخت *)obj);
	بازگشت err;
}

ایستا صحیح default_refs;

ایستا تهی fsck_handle_reflog_oid(ساختار repository *repo,
				   ثابت نویسه *refname, ساختار object_id *oid,
				   timestamp_t timestamp)
{
	ساختار object *obj;

	اگر (!is_null_oid(oid)) {
		obj = lookup_object(repo, oid);
		اگر (obj && (obj->flags & HAS_OBJ)) {
			اگر (timestamp)
				fsck_put_object_name(&fsck_walk_options, oid,
						     "%s@{%"PRItime"}",
						     refname, timestamp);
			obj->flags |= USED;
			mark_object_reachable(obj);
		} وگرنه اگر (!is_promisor_object(repo, oid)) {
			error(_("%s: invalid reflog entry %s"),
			      refname, oid_to_hex(oid));
			errors_found |= ERROR_REACHABLE;
		}
	}
}

ایستا صحیح fsck_handle_reflog_ent(ثابت نویسه *refname,
				  ساختار object_id *ooid, ساختار object_id *noid,
				  ثابت نویسه *email UNUSED,
				  timestamp_t timestamp, صحیح tz UNUSED,
				  ثابت نویسه *message UNUSED, تهی *cb_data)
{
	ساختار repository *repo = cb_data;

	اگر (now && timestamp > now)
		بازگشت 0;

	اگر (verbose)
		fprintf_ln(stderr, _("Checking reflog %s->%s"),
			   oid_to_hex(ooid), oid_to_hex(noid));

	fsck_handle_reflog_oid(repo, refname, ooid, 0);
	fsck_handle_reflog_oid(repo, refname, noid, timestamp);
	بازگشت 0;
}

ایستا صحیح fsck_handle_reflog(ثابت نویسه *logname, تهی *cb_data)
{
	ساختار strbuf refname = STRBUF_INIT;
	ساختار worktree *wt = cb_data;

	strbuf_worktree_ref(wt, &refname, logname);
	refs_for_each_reflog_ent(get_main_ref_store(wt->repo),
				 refname.buf, fsck_handle_reflog_ent,
				 wt->repo);
	strbuf_release(&refname);
	بازگشت 0;
}

ساختار ref_snapshot {
	نویسه *refname;
	ساختار object_id oid;
	/* TODO: Maybe supplement with latest reflog entry info too? */
};

ساختار snapshot {
	size_t nr;
	size_t alloc;
	ساختار ref_snapshot *ref;
	/* TODO: Consider also snapshotting the index of each worktree. */
};

ساختار snapshot_ref_data {
	ساختار repository *repo;
	ساختار snapshot *snap;
};

ایستا صحیح snapshot_ref(ثابت ساختار reference *ref, تهی *cb_data)
{
	ساختار snapshot_ref_data *data = cb_data;
	ساختار snapshot *snap = data->snap;
	ساختار object *obj;

	obj = parse_object(data->repo, ref->oid);
	اگر (!obj) {
		اگر (is_promisor_object(data->repo, ref->oid)) {
			/*
			 * Increment default_refs anyway, because this is a
			 * valid ref.
			 */
			 default_refs++;
			 بازگشت 0;
		}
		error(_("%s: invalid sha1 pointer %s"),
		      ref->name, oid_to_hex(ref->oid));
		errors_found |= ERROR_REACHABLE;
		/* We'll continue with the rest despite the error.. */
		بازگشت 0;
	}
	اگر (obj->type != OBJ_COMMIT && is_branch(ref->name)) {
		error(_("%s: not a commit"), ref->name);
		errors_found |= ERROR_REFS;
	}
	default_refs++;

	ALLOC_GROW(snap->ref, snap->nr + 1, snap->alloc);
	snap->ref[snap->nr].refname = xstrdup(ref->name);
	oidcpy(&snap->ref[snap->nr].oid, ref->oid);
	snap->nr++;

	بازگشت 0;
}

ایستا صحیح fsck_handle_ref(ثابت ساختار reference *ref, تهی *cb_data)
{
	ساختار repository *repo = cb_data;
	ساختار object *obj;

	obj = parse_object(repo, ref->oid);
	obj->flags |= USED;
	fsck_put_object_name(&fsck_walk_options,
			     ref->oid, "%s", ref->name);
	mark_object_reachable(obj);

	بازگشت 0;
}

ایستا تهی snapshot_refs(ساختار repository *repo,
			  ساختار snapshot *snap, صحیح argc, ثابت نویسه **argv)
{
	ساختار refs_for_each_ref_options opts = {
		.flags = REFS_FOR_EACH_INCLUDE_BROKEN,
	};
	ساختار snapshot_ref_data data = {
		.repo = repo,
		.snap = snap,
	};
	ساختار worktree **worktrees, **p;
	ثابت نویسه *head_points_at;
	ساختار object_id head_oid;

	برای (صحیح i = 0; i < argc; i++) {
		ثابت نویسه *arg = argv[i];
		ساختار object_id oid;
		اگر (!repo_get_oid(repo, arg, &oid)) {
			ساختار reference ref = {
				.name = arg,
				.oid = &oid,
			};

			snapshot_ref(&ref, &data);
			ادامه;
		}
		error(_("invalid parameter: expected sha1, got '%s'"), arg);
		errors_found |= ERROR_OBJECT;
	}

	اگر (argc) {
		include_reflogs = 0;
		بازگشت;
	}

	refs_for_each_ref_ext(get_main_ref_store(repo),
			      snapshot_ref, &data, &opts);

	worktrees = get_worktrees(repo);
	برای (p = worktrees; *p; p++) {
		ساختار worktree *wt = *p;
		ساختار strbuf refname = STRBUF_INIT;

		strbuf_worktree_ref(wt, &refname, "HEAD");

		head_points_at = refs_resolve_ref_unsafe(get_main_ref_store(repo),
							 refname.buf, 0, &head_oid, پوچ);

		اگر (head_points_at && !is_null_oid(&head_oid)) {
			ساختار reference ref = {
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
	now = time(پوچ);
}


ایستا تهی free_snapshot_refs(ساختار snapshot *snap)
{
	برای (size_t i = 0; i < snap->nr; i++)
		آزاد(snap->ref[i].refname);
	آزاد(snap->ref);
}

ایستا تهی process_refs(ساختار repository *repo, ساختار snapshot *snap)
{
	ساختار worktree **worktrees, **p;

	برای (size_t i = 0; i < snap->nr; i++) {
		ساختار reference ref = {
			.name = snap->ref[i].refname,
			.oid = &snap->ref[i].oid,
		};
		fsck_handle_ref(&ref, repo);
	}

	اگر (include_reflogs) {
		worktrees = get_worktrees(repo);
		برای (p = worktrees; *p; p++) {
			ساختار worktree *wt = *p;

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
	اگر (!default_refs) {
		fprintf_ln(stderr, _("notice: No default references"));
		show_unreachable = 0;
	}
}

ایستا صحیح fsck_cache_درخت(ساختار repository *repo, ساختار cache_درخت *it, ثابت نویسه *index_path)
{
	صحیح i;
	صحیح err = 0;

	اگر (verbose)
		fprintf_ln(stderr, _("Checking cache tree of %s"), index_path);

	اگر (0 <= it->entry_شمار) {
		ساختار object *obj = parse_object(repo, &it->oid);
		اگر (!obj) {
			error(_("%s: invalid sha1 pointer in cache-tree of %s"),
			      oid_to_hex(&it->oid), index_path);
			errors_found |= ERROR_REFS;
			بازگشت 1;
		}
		obj->flags |= USED;
		fsck_put_object_name(&fsck_walk_options, &it->oid, ":");
		mark_object_reachable(obj);
		اگر (obj->type != OBJ_درخت)
			err |= objerror(repo, obj, _("non-tree in cache-tree"));
	}
	برای (i = 0; i < it->subtree_nr; i++)
		err |= fsck_cache_درخت(repo, it->down[i]->cache_درخت, index_path);
	بازگشت err;
}

ایستا صحیح fsck_resolve_undo(ساختار index_state *istate,
			     ثابت نویسه *index_path)
{
	ساختار string_list_item *item;
	ساختار string_list *resolve_undo = istate->resolve_undo;

	اگر (!resolve_undo)
		بازگشت 0;

	for_each_string_list_item(item, resolve_undo) {
		ثابت نویسه *path = item->string;
		ساختار resolve_undo_info *ru = item->util;
		صحیح i;

		اگر (!ru)
			ادامه;
		برای (i = 0; i < 3; i++) {
			ساختار object *obj;

			اگر (!ru->mode[i] || !S_ISREG(ru->mode[i]))
				ادامه;

			obj = parse_object(istate->repo, &ru->oid[i]);
			اگر (!obj) {
				error(_("%s: invalid sha1 pointer in resolve-undo of %s"),
				      oid_to_hex(&ru->oid[i]),
				      index_path);
				errors_found |= ERROR_REFS;
				ادامه;
			}
			obj->flags |= USED;
			fsck_put_object_name(&fsck_walk_options, &ru->oid[i],
					     ":(%d):%s", i, path);
			mark_object_reachable(obj);
		}
	}
	بازگشت 0;
}

ایستا تهی fsck_index(ساختار index_state *istate, ثابت نویسه *index_path,
		       صحیح is_current_worktree)
{
	unsigned صحیح i;

	/* TODO: audit for interaction with sparse-index. */
	ensure_full_index(istate);
	برای (i = 0; i < istate->cache_nr; i++) {
		unsigned صحیح mode;
		ساختار blob *blob;
		ساختار object *obj;

		mode = istate->cache[i]->ce_mode;
		اگر (S_ISGITLINK(mode))
			ادامه;
		blob = lookup_blob(istate->repo,
				   &istate->cache[i]->oid);
		اگر (!blob)
			ادامه;
		obj = &blob->object;
		obj->flags |= USED;
		fsck_put_object_name(&fsck_walk_options, &obj->oid,
				     "%s:%s",
				     is_current_worktree ? "" : index_path,
				     istate->cache[i]->name);
		mark_object_reachable(obj);
	}
	اگر (istate->cache_درخت)
		fsck_cache_درخت(istate->repo, istate->cache_درخت, index_path);
	fsck_resolve_undo(istate, index_path);
}

ایستا صحیح mark_object_for_connectivity(ثابت ساختار object_id *oid,
					ساختار object_info *oi UNUSED,
					تهی *cb_data)
{
	ساختار repository *repo = cb_data;
	ساختار object *obj = lookup_unknown_object(repo, oid);
	obj->flags |= HAS_OBJ;
	بازگشت 0;
}

ایستا تهی fsck_refs(ساختار repository *r)
{
	ساختار child_process refs_verify = CHILD_PROCESS_INIT;
	ساختار progress *progress = پوچ;

	اگر (show_progress)
		progress = start_progress(r, _("Checking ref database"), 1);

	اگر (verbose)
		fprintf_ln(stderr, _("Checking ref database"));

	child_process_init(&refs_verify);
	refs_verify.git_cmd = 1;
	strvec_pushl(&refs_verify.args, "refs", "verify", پوچ);
	اگر (verbose)
		strvec_افزودن(&refs_verify.args, "--verbose");
	اگر (check_strict)
		strvec_افزودن(&refs_verify.args, "--strict");

	اگر (run_command(&refs_verify))
		errors_found |= ERROR_REFS;

	display_progress(progress, 1);
	stop_progress(&progress);
}

ایستا نویسه ثابت * ثابت fsck_usage[] = {
	N_("git fsck [--tags] [--root] [--unreachable] [--cache] [--no-reflogs]\n"
	   "         [--[no-]full] [--strict] [--verbose] [--lost-found]\n"
	   "         [--[no-]dangling] [--[no-]progress] [--connectivity-only]\n"
	   "         [--[no-]name-objects] [--[no-]references] [<object>...]"),
	پوچ
};

صحیح cmd_fsck(صحیح argc,
	     ثابت نویسه **argv,
	     ثابت نویسه *prefix,
	     ساختار repository *repo)
{
	ساختار odb_fsck_options odb_fsck_opts = {
		.flags = ODB_FSCK_FULL,
		.object_cb = fsck_obj_buffer,
		.object_payload = repo,
	};
	صحیح keep_cache_objects = 0;
	صحیح name_objects = 0;
	صحیح check_references = 1;
	ساختار option fsck_opts[] = {
		OPT__VERBOSE(&verbose, N_("be verbose")),
		OPT_BOOL(0, "unreachable", &show_unreachable, N_("show unreachable objects")),
		OPT_BOOL(0, "dangling", &show_dangling, N_("show dangling objects")),
		OPT_BOOL(0, "tags", &show_tags, N_("report tags")),
		OPT_BOOL(0, "root", &show_ریشه, N_("report root nodes")),
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
	ساختار snapshot snap = {
		.nr = 0,
		.alloc = 0,
		.ref = پوچ
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
	اگر (check_strict)
		fsck_obj_options.strict = 1;

	اگر (show_progress == -1)
		show_progress = isatty(2);
	اگر (verbose) {
		show_progress = 0;
		odb_fsck_opts.flags |= ODB_FSCK_VERBOSE;
	}
	اگر (show_progress)
		odb_fsck_opts.flags |= ODB_FSCK_PROGRESS;

	اگر (write_lost_and_found) {
		odb_fsck_opts.flags |= ODB_FSCK_FULL;
		include_reflogs = 0;
	}

	اگر (name_objects)
		fsck_enable_object_names(&fsck_walk_options);

	repo_config(repo, git_fsck_config, &fsck_obj_options);
	prepare_repo_settings(repo);

	اگر (check_references)
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

	اگر (connectivity_only) {
		odb_for_each_object(repo->objects, پوچ,
				    mark_object_for_connectivity, repo, 0);
	} وگرنه {
		اگر (odb_fsck(repo->objects, &odb_fsck_opts) < 0)
			errors_found |= ERROR_OBJECT;

		اگر (fsck_finish(&fsck_obj_options))
			errors_found |= ERROR_OBJECT;
	}

	/* Process the snapshotted refs and the reflogs. */
	process_refs(repo, &snap);

	/* If not given any explicit objects, process index files too. */
	اگر (!argc)
		keep_cache_objects = 1;
	اگر (keep_cache_objects) {
		/*
		 * TODO: Consider first walking these indexes in snapshot_refs,
		 * to snapshot where the index entries used to point, and then
		 * check those snapshotted locations here.
		 */
		ساختار worktree **worktrees, **p;

		verify_index_checksum = 1;
		verify_ce_order = 1;

		worktrees = get_worktrees(repo);
		برای (p = worktrees; *p; p++) {
			ساختار worktree *wt = *p;
			ساختار index_state istate =
				INDEX_STATE_INIT(repo);
			نویسه *path, *wt_gitdir;

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
			آزاد(wt_gitdir);
			آزاد(path);
		}
		free_worktrees(worktrees);
	}

	check_connectivity(repo);

	اگر (repo->settings.core_commit_graph) {
		ساختار odb_source_files *files = odb_source_files_downcast(repo->objects->source);
		ساختار child_process commit_graph_verify = CHILD_PROCESS_INIT;

		برای (ساختار odb_files_dir *dir = files->dirs; dir; dir = dir->next) {
			child_process_init(&commit_graph_verify);
			commit_graph_verify.git_cmd = 1;
			strvec_pushl(&commit_graph_verify.args, "commit-graph",
				     "verify", "--object-dir", dir->abspath, پوچ);
			اگر (show_progress)
				strvec_افزودن(&commit_graph_verify.args, "--progress");
			وگرنه
				strvec_افزودن(&commit_graph_verify.args, "--no-progress");
			اگر (run_command(&commit_graph_verify))
				errors_found |= ERROR_COMMIT_GRAPH;
		}
	}

	free_snapshot_refs(&snap);
	بازگشت errors_found;
}
