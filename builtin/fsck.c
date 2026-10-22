#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "hex.h"
#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ "commit.h"
#અન્તર્ભાવ "tree.h"
#અન્તર્ભાવ "blob.h"
#અન્તર્ભાવ "tag.h"
#અન્તર્ભાવ "refs.h"
#અન્તર્ભાવ "cache-tree.h"
#અન્તર્ભાવ "fsck.h"
#અન્તર્ભાવ "parse-options.h"
#અન્તર્ભાવ "progress.h"
#અન્તર્ભાવ "packfile.h"
#અન્તર્ભાવ "object-name.h"
#અન્તર્ભાવ "odb.h"
#અન્તર્ભાવ "odb/streaming.h"
#અન્તર્ભાવ "path.h"
#અન્તર્ભાવ "read-cache-ll.h"
#અન્તર્ભાવ "replace-object.h"
#અન્તર્ભાવ "resolve-undo.h"
#અન્તર્ભાવ "run-command.h"
#અન્તર્ભાવ "sparse-index.h"
#અન્તર્ભાવ "worktree.h"

#define REACHABLE 0x0001
#define SEEN      0x0002
#define HAS_OBJ   0x0004
/* This flag is set if something points to this object. */
#define USED      0x0008

સ્થિર પૂર્ણાંક show_મૂળ;
સ્થિર પૂર્ણાંક show_tags;
સ્થિર પૂર્ણાંક show_unreachable;
સ્થિર પૂર્ણાંક include_reflogs = 1;
સ્થિર પૂર્ણાંક connectivity_only;
સ્થિર પૂર્ણાંક check_strict;
સ્થિર સંરચના fsck_options fsck_walk_options;
સ્થિર સંરચના fsck_options fsck_obj_options;
સ્થિર પૂર્ણાંક errors_found;
સ્થિર પૂર્ણાંક write_lost_and_found;
સ્થિર પૂર્ણાંક verbose;
સ્થિર પૂર્ણાંક show_progress = -1;
સ્થિર પૂર્ણાંક show_dangling = 1;
સ્થિર timestamp_t now;
#define ERROR_OBJECT 01
#define ERROR_REACHABLE 02
#define ERROR_REFS 010
#define ERROR_COMMIT_GRAPH 020

સ્થિર અચલ અક્ષર *describe_object(અચલ સંરચના object_id *oid)
{
	પાછા fsck_describe_object(&fsck_walk_options, oid);
}

સ્થિર અચલ અક્ષર *printable_type(સંરચના repository *repo,
				  અચલ સંરચના object_id *oid,
				  ગણના object_type type)
{
	અચલ અક્ષર *ret;

	જો (type == OBJ_NONE)
		type = odb_read_object_info(repo->objects, oid, ખાલી);

	ret = type_name(type);
	જો (!ret)
		ret = _("unknown");

	પાછા ret;
}

સ્થિર પૂર્ણાંક objerror(સંરચના repository *repo, સંરચના object *obj, અચલ અક્ષર *err)
{
	errors_found |= ERROR_OBJECT;
	/* TRANSLATORS: e.g. error in tree 01bfda: <more explanation> */
	fprintf_ln(stderr, _("error in %s %s: %s"),
		   printable_type(repo, &obj->oid, obj->type),
		   describe_object(&obj->oid), err);
	પાછા -1;
}

સ્થિર પૂર્ણાંક fsck_objects_error_func(સંરચના fsck_options *o,
				   શૂન્ય *fsck_report,
				   ગણના fsck_સંદેશ_type સંદેશ_type,
				   ગણના fsck_સંદેશ_id સંદેશ_id UNUSED,
				   અચલ અક્ષર *message)
{
	સંરચના fsck_object_report *report = fsck_report;
	અચલ સંરચના object_id *oid = report->oid;
	ગણના object_type object_type = report->object_type;

	પસંદગી (સંદેશ_type) {
	સ્થિતિ FSCK_WARN:
		/* TRANSLATORS: e.g. warning in tree 01bfda: <more explanation> */
		fprintf_ln(stderr, _("warning in %s %s: %s"),
			   printable_type(o->repo, oid, object_type),
			   describe_object(oid), message);
		પાછા 0;
	સ્થિતિ FSCK_ERROR:
		/* TRANSLATORS: e.g. error in tree 01bfda: <more explanation> */
		fprintf_ln(stderr, _("error in %s %s: %s"),
			   printable_type(o->repo, oid, object_type),
			   describe_object(oid), message);
		પાછા 1;
	મૂળભૂત:
		BUG("%d (FSCK_IGNORE?) should never trigger this callback",
		    સંદેશ_type);
	}
}

સ્થિર સંરચના object_array pending;

સ્થિર પૂર્ણાંક mark_object(સંરચના object *obj, ગણના object_type type,
		       શૂન્ય *data, સંરચના fsck_options *options)
{
	સંરચના object *parent = data;

	/*
	 * The only case data is NULL or type is OBJ_ANY is when
	 * mark_object_reachable() calls us.  All the callers of
	 * that function has non-NULL obj hence ...
	 */
	જો (!obj) {
		/* ... these references to parent->fld are safe here */
		printf_ln(_("broken link from %7s %s"),
			  printable_type(options->repo, &parent->oid, parent->type),
			  describe_object(&parent->oid));
		printf_ln(_("broken link from %7s %s"),
			  (type == OBJ_ANY ? _("unknown") : type_name(type)),
			  _("unknown"));
		errors_found |= ERROR_REACHABLE;
		પાછા 1;
	}

	જો (type != OBJ_ANY && obj->type != type)
		/* ... and the reference to parent is safe here */
		objerror(options->repo, parent, _("wrong object type in link"));

	જો (obj->flags & REACHABLE)
		પાછા 0;
	obj->flags |= REACHABLE;

	જો (is_promisor_object(options->repo, &obj->oid))
		/*
		 * Further recursion does not need to be performed on this
		 * object since it is a promisor object (so it does not need to
		 * be added to "pending").
		 */
		પાછા 0;

	જો (!(obj->flags & HAS_OBJ)) {
		જો (parent && !odb_has_object(options->repo->objects, &obj->oid,
					      ODB_HAS_OBJECT_RECHECK_PACKED)) {
			printf_ln(_("broken link from %7s %s\n"
				    "              to %7s %s"),
				  printable_type(options->repo, &parent->oid, parent->type),
				  describe_object(&parent->oid),
				  printable_type(options->repo, &obj->oid, obj->type),
				  describe_object(&obj->oid));
			errors_found |= ERROR_REACHABLE;
		}
		પાછા 1;
	}

	add_object_array(obj, ખાલી, &pending);
	પાછા 0;
}

સ્થિર શૂન્ય mark_object_reachable(સંરચના object *obj)
{
	mark_object(obj, OBJ_ANY, ખાલી, &fsck_walk_options);
}

સ્થિર પૂર્ણાંક traverse_one_object(સંરચના object *obj)
{
	પૂર્ણાંક result = fsck_walk(obj, obj, &fsck_walk_options);

	જો (obj->type == OBJ_વૃક્ષ) {
		સંરચના વૃક્ષ *વૃક્ષ = (સંરચના વૃક્ષ *)obj;
		free_વૃક્ષ_buffer(વૃક્ષ);
	}
	પાછા result;
}

સ્થિર પૂર્ણાંક traverse_reachable(સંરચના repository *repo)
{
	સંરચના progress *progress = ખાલી;
	unsigned પૂર્ણાંક nr = 0;
	પૂર્ણાંક result = 0;
	જો (show_progress)
		progress = start_delayed_progress(repo,
						  _("Checking connectivity"), 0);
	જ્યારે (pending.nr) {
		result |= traverse_one_object(object_array_કાઢો(&pending));
		display_progress(progress, ++nr);
	}
	stop_progress(&progress);
	પાછા !!result;
}

સ્થિર પૂર્ણાંક mark_used(સંરચના object *obj, ગણના object_type type UNUSED,
		     શૂન્ય *data UNUSED, સંરચના fsck_options *options UNUSED)
{
	જો (!obj)
		પાછા 1;
	obj->flags |= USED;
	પાછા 0;
}

સ્થિર પૂર્ણાંક mark_unreachable_referents(અચલ સંરચના object_id *oid,
				      સંરચના object_info *oi UNUSED,
				      શૂન્ય *data)
{
	સંરચના repository *repo = data;
	સંરચના fsck_options options;
	સંરચના object *obj = lookup_object(data, oid);

	જો (!obj || !(obj->flags & HAS_OBJ))
		પાછા 0; /* not part of our original set */
	જો (obj->flags & REACHABLE)
		પાછા 0; /* reachable objects already traversed */

	/*
	 * Avoid passing OBJ_NONE to fsck_walk, which will parse the object
	 * (and we want to avoid parsing blobs).
	 */
	જો (obj->type == OBJ_NONE) {
		ગણના object_type type = odb_read_object_info(repo->objects,
							     &obj->oid, ખાલી);
		જો (type > 0)
			object_as_type(obj, type, 0);
	}

	fsck_options_init(&options, repo, FSCK_OPTIONS_DEFAULT);
	options.walk = mark_used;
	fsck_walk(obj, ખાલી, &options);
	જો (obj->type == OBJ_વૃક્ષ)
		free_વૃક્ષ_buffer((સંરચના વૃક્ષ *)obj);

	પાછા 0;
}

/*
 * Check a single reachable object
 */
સ્થિર શૂન્ય check_reachable_object(સંરચના repository *repo, સંરચના object *obj)
{
	/*
	 * We obviously want the object to be parsed,
	 * except if it was in a pack-file and we didn't
	 * do a full fsck
	 */
	જો (!(obj->flags & HAS_OBJ)) {
		જો (is_promisor_object(repo, &obj->oid))
			પાછા;
		જો (has_object_pack(repo, &obj->oid))
			પાછા; /* it is in pack - forget about it */
		printf_ln(_("missing %s %s"),
			  printable_type(repo, &obj->oid, obj->type),
			  describe_object(&obj->oid));
		errors_found |= ERROR_REACHABLE;
		પાછા;
	}
}

/*
 * Check a single unreachable object
 */
સ્થિર શૂન્ય check_unreachable_object(સંરચના repository *repo, સંરચના object *obj)
{
	/*
	 * Missing unreachable object? Ignore it. It's not like
	 * we miss it (since it can't be reached), nor do we want
	 * to complain about it being unreachable (since it does
	 * not exist).
	 */
	જો (!(obj->flags & HAS_OBJ))
		પાછા;

	/*
	 * Unreachable object that exists? Show it if asked to,
	 * since this is something that is prunable.
	 */
	જો (show_unreachable) {
		printf_ln(_("unreachable %s %s"),
			  printable_type(repo, &obj->oid, obj->type),
			  describe_object(&obj->oid));
		પાછા;
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
	જો (!(obj->flags & USED)) {
		જો (show_dangling)
			printf_ln(_("dangling %s %s"),
				  printable_type(repo, &obj->oid, obj->type),
				  describe_object(&obj->oid));
		જો (write_lost_and_found) {
			અક્ષર *filename = repo_git_path(repo, "lost-found/%s/%s",
				obj->type == OBJ_COMMIT ? "commit" : "other",
				describe_object(&obj->oid));
			FILE *f;

			જો (safe_create_leading_directories_const(repo, filename)) {
				error(_("could not create lost-found"));
				મુક્ત(filename);
				પાછા;
			}
			f = xfopen(filename, "w");
			જો (obj->type == OBJ_BLOB) {
				જો (odb_stream_blob_to_fd(repo->objects, fileno(f),
							  &obj->oid, ખાલી, 1))
					die_errno(_("could not write '%s'"), filename);
			} નહીંતર
				fprintf(f, "%s\n", describe_object(&obj->oid));
			જો (fclose(f))
				die_errno(_("could not finish '%s'"),
					  filename);
			મુક્ત(filename);
		}
		પાછા;
	}

	/*
	 * Otherwise? It's there, it's unreachable, and some other unreachable
	 * object points to it. Ignore it - it's not interesting, and we showed
	 * all the interesting cases above.
	 */
}

સ્થિર શૂન્ય check_object(સંરચના repository *repo, સંરચના object *obj)
{
	જો (verbose)
		fprintf_ln(stderr, _("Checking %s"), describe_object(&obj->oid));

	જો (obj->flags & REACHABLE)
		check_reachable_object(repo, obj);
	નહીંતર
		check_unreachable_object(repo, obj);
}

સ્થિર શૂન્ય check_connectivity(સંરચના repository *repo)
{
	પૂર્ણાંક i, max;

	/* Traverse the pending reachable objects */
	traverse_reachable(repo);

	/*
	 * With --connectivity-only, we won't have actually opened and marked
	 * unreachable objects with USED. Do that now to make --dangling, etc
	 * accurate.
	 */
	જો (connectivity_only && (show_dangling || write_lost_and_found)) {
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
		odb_for_each_object(repo->objects, ખાલી,
				    mark_unreachable_referents, repo, 0);
	}

	/* Look up all the requirements, warn about missing objects.. */
	max = get_max_object_index(repo);
	જો (verbose)
		fprintf_ln(stderr, _("Checking connectivity (%d objects)"), max);

	માટે (i = 0; i < max; i++) {
		સંરચના object *obj = get_indexed_object(repo, i);

		જો (obj)
			check_object(repo, obj);
	}
}

સ્થિર પૂર્ણાંક fsck_obj_buffer(અચલ સંરચના object_id *oid, ગણના object_type type,
			   unsigned long size, શૂન્ય *buffer, પૂર્ણાંક *eaten, શૂન્ય *cb_data)
{
	સંરચના repository *repo = cb_data;
	સંરચના object *obj;
	પૂર્ણાંક err;

	/*
	 * Note, buffer may be NULL if type is OBJ_BLOB. See
	 * verify_packfile(), data_valid variable for details.
	 */
	obj = parse_object_buffer(repo, oid, type, size, buffer, eaten);
	જો (!obj) {
		errors_found |= ERROR_OBJECT;
		err = error(_("%s: object corrupt or missing"),
			    oid_to_hex(oid));
		જાઓ out;
	}

	obj->flags &= ~REACHABLE;
	obj->flags |= HAS_OBJ | SEEN;

	જો (verbose)
		fprintf_ln(stderr, _("Checking %s %s"),
			   printable_type(repo, &obj->oid, obj->type),
			   describe_object(&obj->oid));

	જો (fsck_walk(obj, ખાલી, &fsck_obj_options))
		objerror(repo, obj, _("broken links"));

	err = fsck_object(obj, buffer, size, &fsck_obj_options);
	જો (err)
		જાઓ out;

	જો (obj->type == OBJ_COMMIT) {
		સંરચના commit *commit = (સંરચના commit *) obj;

		જો (!commit->parents && show_મૂળ)
			printf_ln(_("root %s"),
				  describe_object(&commit->object.oid));
	}

	જો (obj->type == OBJ_TAG) {
		સંરચના tag *tag = (સંરચના tag *) obj;

		જો (show_tags && tag->tagged) {
			printf_ln(_("tagged %s %s (%s) in %s"),
				  printable_type(repo, &tag->tagged->oid, tag->tagged->type),
				  describe_object(&tag->tagged->oid),
				  tag->tag,
				  describe_object(&tag->object.oid));
		}
	}

out:
	જો (obj && obj->type == OBJ_વૃક્ષ)
		free_વૃક્ષ_buffer((સંરચના વૃક્ષ *)obj);
	પાછા err;
}

સ્થિર પૂર્ણાંક default_refs;

સ્થિર શૂન્ય fsck_handle_reflog_oid(સંરચના repository *repo,
				   અચલ અક્ષર *refname, સંરચના object_id *oid,
				   timestamp_t timestamp)
{
	સંરચના object *obj;

	જો (!is_null_oid(oid)) {
		obj = lookup_object(repo, oid);
		જો (obj && (obj->flags & HAS_OBJ)) {
			જો (timestamp)
				fsck_put_object_name(&fsck_walk_options, oid,
						     "%s@{%"PRItime"}",
						     refname, timestamp);
			obj->flags |= USED;
			mark_object_reachable(obj);
		} નહીંતર જો (!is_promisor_object(repo, oid)) {
			error(_("%s: invalid reflog entry %s"),
			      refname, oid_to_hex(oid));
			errors_found |= ERROR_REACHABLE;
		}
	}
}

સ્થિર પૂર્ણાંક fsck_handle_reflog_ent(અચલ અક્ષર *refname,
				  સંરચના object_id *ooid, સંરચના object_id *noid,
				  અચલ અક્ષર *email UNUSED,
				  timestamp_t timestamp, પૂર્ણાંક tz UNUSED,
				  અચલ અક્ષર *message UNUSED, શૂન્ય *cb_data)
{
	સંરચના repository *repo = cb_data;

	જો (now && timestamp > now)
		પાછા 0;

	જો (verbose)
		fprintf_ln(stderr, _("Checking reflog %s->%s"),
			   oid_to_hex(ooid), oid_to_hex(noid));

	fsck_handle_reflog_oid(repo, refname, ooid, 0);
	fsck_handle_reflog_oid(repo, refname, noid, timestamp);
	પાછા 0;
}

સ્થિર પૂર્ણાંક fsck_handle_reflog(અચલ અક્ષર *logname, શૂન્ય *cb_data)
{
	સંરચના strbuf refname = STRBUF_INIT;
	સંરચના worktree *wt = cb_data;

	strbuf_worktree_ref(wt, &refname, logname);
	refs_for_each_reflog_ent(get_main_ref_store(wt->repo),
				 refname.buf, fsck_handle_reflog_ent,
				 wt->repo);
	strbuf_release(&refname);
	પાછા 0;
}

સંરચના ref_snapshot {
	અક્ષર *refname;
	સંરચના object_id oid;
	/* TODO: Maybe supplement with latest reflog entry info too? */
};

સંરચના snapshot {
	size_t nr;
	size_t alloc;
	સંરચના ref_snapshot *ref;
	/* TODO: Consider also snapshotting the index of each worktree. */
};

સંરચના snapshot_ref_data {
	સંરચના repository *repo;
	સંરચના snapshot *snap;
};

સ્થિર પૂર્ણાંક snapshot_ref(અચલ સંરચના reference *ref, શૂન્ય *cb_data)
{
	સંરચના snapshot_ref_data *data = cb_data;
	સંરચના snapshot *snap = data->snap;
	સંરચના object *obj;

	obj = parse_object(data->repo, ref->oid);
	જો (!obj) {
		જો (is_promisor_object(data->repo, ref->oid)) {
			/*
			 * Increment default_refs anyway, because this is a
			 * valid ref.
			 */
			 default_refs++;
			 પાછા 0;
		}
		error(_("%s: invalid sha1 pointer %s"),
		      ref->name, oid_to_hex(ref->oid));
		errors_found |= ERROR_REACHABLE;
		/* We'll continue with the rest despite the error.. */
		પાછા 0;
	}
	જો (obj->type != OBJ_COMMIT && is_branch(ref->name)) {
		error(_("%s: not a commit"), ref->name);
		errors_found |= ERROR_REFS;
	}
	default_refs++;

	ALLOC_GROW(snap->ref, snap->nr + 1, snap->alloc);
	snap->ref[snap->nr].refname = xstrdup(ref->name);
	oidcpy(&snap->ref[snap->nr].oid, ref->oid);
	snap->nr++;

	પાછા 0;
}

સ્થિર પૂર્ણાંક fsck_handle_ref(અચલ સંરચના reference *ref, શૂન્ય *cb_data)
{
	સંરચના repository *repo = cb_data;
	સંરચના object *obj;

	obj = parse_object(repo, ref->oid);
	obj->flags |= USED;
	fsck_put_object_name(&fsck_walk_options,
			     ref->oid, "%s", ref->name);
	mark_object_reachable(obj);

	પાછા 0;
}

સ્થિર શૂન્ય snapshot_refs(સંરચના repository *repo,
			  સંરચના snapshot *snap, પૂર્ણાંક argc, અચલ અક્ષર **argv)
{
	સંરચના refs_for_each_ref_options opts = {
		.flags = REFS_FOR_EACH_INCLUDE_BROKEN,
	};
	સંરચના snapshot_ref_data data = {
		.repo = repo,
		.snap = snap,
	};
	સંરચના worktree **worktrees, **p;
	અચલ અક્ષર *head_points_at;
	સંરચના object_id head_oid;

	માટે (પૂર્ણાંક i = 0; i < argc; i++) {
		અચલ અક્ષર *arg = argv[i];
		સંરચના object_id oid;
		જો (!repo_get_oid(repo, arg, &oid)) {
			સંરચના reference ref = {
				.name = arg,
				.oid = &oid,
			};

			snapshot_ref(&ref, &data);
			ચાલુ;
		}
		error(_("invalid parameter: expected sha1, got '%s'"), arg);
		errors_found |= ERROR_OBJECT;
	}

	જો (argc) {
		include_reflogs = 0;
		પાછા;
	}

	refs_for_each_ref_ext(get_main_ref_store(repo),
			      snapshot_ref, &data, &opts);

	worktrees = get_worktrees(repo);
	માટે (p = worktrees; *p; p++) {
		સંરચના worktree *wt = *p;
		સંરચના strbuf refname = STRBUF_INIT;

		strbuf_worktree_ref(wt, &refname, "HEAD");

		head_points_at = refs_resolve_ref_unsafe(get_main_ref_store(repo),
							 refname.buf, 0, &head_oid, ખાલી);

		જો (head_points_at && !is_null_oid(&head_oid)) {
			સંરચના reference ref = {
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
	now = time(ખાલી);
}


સ્થિર શૂન્ય free_snapshot_refs(સંરચના snapshot *snap)
{
	માટે (size_t i = 0; i < snap->nr; i++)
		મુક્ત(snap->ref[i].refname);
	મુક્ત(snap->ref);
}

સ્થિર શૂન્ય process_refs(સંરચના repository *repo, સંરચના snapshot *snap)
{
	સંરચના worktree **worktrees, **p;

	માટે (size_t i = 0; i < snap->nr; i++) {
		સંરચના reference ref = {
			.name = snap->ref[i].refname,
			.oid = &snap->ref[i].oid,
		};
		fsck_handle_ref(&ref, repo);
	}

	જો (include_reflogs) {
		worktrees = get_worktrees(repo);
		માટે (p = worktrees; *p; p++) {
			સંરચના worktree *wt = *p;

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
	જો (!default_refs) {
		fprintf_ln(stderr, _("notice: No default references"));
		show_unreachable = 0;
	}
}

સ્થિર પૂર્ણાંક fsck_cache_વૃક્ષ(સંરચના repository *repo, સંરચના cache_વૃક્ષ *it, અચલ અક્ષર *index_path)
{
	પૂર્ણાંક i;
	પૂર્ણાંક err = 0;

	જો (verbose)
		fprintf_ln(stderr, _("Checking cache tree of %s"), index_path);

	જો (0 <= it->entry_ગણતરી) {
		સંરચના object *obj = parse_object(repo, &it->oid);
		જો (!obj) {
			error(_("%s: invalid sha1 pointer in cache-tree of %s"),
			      oid_to_hex(&it->oid), index_path);
			errors_found |= ERROR_REFS;
			પાછા 1;
		}
		obj->flags |= USED;
		fsck_put_object_name(&fsck_walk_options, &it->oid, ":");
		mark_object_reachable(obj);
		જો (obj->type != OBJ_વૃક્ષ)
			err |= objerror(repo, obj, _("non-tree in cache-tree"));
	}
	માટે (i = 0; i < it->subtree_nr; i++)
		err |= fsck_cache_વૃક્ષ(repo, it->down[i]->cache_વૃક્ષ, index_path);
	પાછા err;
}

સ્થિર પૂર્ણાંક fsck_resolve_undo(સંરચના index_state *istate,
			     અચલ અક્ષર *index_path)
{
	સંરચના string_list_item *item;
	સંરચના string_list *resolve_undo = istate->resolve_undo;

	જો (!resolve_undo)
		પાછા 0;

	for_each_string_list_item(item, resolve_undo) {
		અચલ અક્ષર *path = item->string;
		સંરચના resolve_undo_info *ru = item->util;
		પૂર્ણાંક i;

		જો (!ru)
			ચાલુ;
		માટે (i = 0; i < 3; i++) {
			સંરચના object *obj;

			જો (!ru->mode[i] || !S_ISREG(ru->mode[i]))
				ચાલુ;

			obj = parse_object(istate->repo, &ru->oid[i]);
			જો (!obj) {
				error(_("%s: invalid sha1 pointer in resolve-undo of %s"),
				      oid_to_hex(&ru->oid[i]),
				      index_path);
				errors_found |= ERROR_REFS;
				ચાલુ;
			}
			obj->flags |= USED;
			fsck_put_object_name(&fsck_walk_options, &ru->oid[i],
					     ":(%d):%s", i, path);
			mark_object_reachable(obj);
		}
	}
	પાછા 0;
}

સ્થિર શૂન્ય fsck_index(સંરચના index_state *istate, અચલ અક્ષર *index_path,
		       પૂર્ણાંક is_current_worktree)
{
	unsigned પૂર્ણાંક i;

	/* TODO: audit for interaction with sparse-index. */
	ensure_full_index(istate);
	માટે (i = 0; i < istate->cache_nr; i++) {
		unsigned પૂર્ણાંક mode;
		સંરચના blob *blob;
		સંરચના object *obj;

		mode = istate->cache[i]->ce_mode;
		જો (S_ISGITLINK(mode))
			ચાલુ;
		blob = lookup_blob(istate->repo,
				   &istate->cache[i]->oid);
		જો (!blob)
			ચાલુ;
		obj = &blob->object;
		obj->flags |= USED;
		fsck_put_object_name(&fsck_walk_options, &obj->oid,
				     "%s:%s",
				     is_current_worktree ? "" : index_path,
				     istate->cache[i]->name);
		mark_object_reachable(obj);
	}
	જો (istate->cache_વૃક્ષ)
		fsck_cache_વૃક્ષ(istate->repo, istate->cache_વૃક્ષ, index_path);
	fsck_resolve_undo(istate, index_path);
}

સ્થિર પૂર્ણાંક mark_object_for_connectivity(અચલ સંરચના object_id *oid,
					સંરચના object_info *oi UNUSED,
					શૂન્ય *cb_data)
{
	સંરચના repository *repo = cb_data;
	સંરચના object *obj = lookup_unknown_object(repo, oid);
	obj->flags |= HAS_OBJ;
	પાછા 0;
}

સ્થિર શૂન્ય fsck_refs(સંરચના repository *r)
{
	સંરચના child_process refs_verify = CHILD_PROCESS_INIT;
	સંરચના progress *progress = ખાલી;

	જો (show_progress)
		progress = start_progress(r, _("Checking ref database"), 1);

	જો (verbose)
		fprintf_ln(stderr, _("Checking ref database"));

	child_process_init(&refs_verify);
	refs_verify.git_cmd = 1;
	strvec_pushl(&refs_verify.args, "refs", "verify", ખાલી);
	જો (verbose)
		strvec_ધકેલો(&refs_verify.args, "--verbose");
	જો (check_strict)
		strvec_ધકેલો(&refs_verify.args, "--strict");

	જો (run_command(&refs_verify))
		errors_found |= ERROR_REFS;

	display_progress(progress, 1);
	stop_progress(&progress);
}

સ્થિર અક્ષર અચલ * અચલ fsck_usage[] = {
	N_("git fsck [--tags] [--root] [--unreachable] [--cache] [--no-reflogs]\n"
	   "         [--[no-]full] [--strict] [--verbose] [--lost-found]\n"
	   "         [--[no-]dangling] [--[no-]progress] [--connectivity-only]\n"
	   "         [--[no-]name-objects] [--[no-]references] [<object>...]"),
	ખાલી
};

પૂર્ણાંક cmd_fsck(પૂર્ણાંક argc,
	     અચલ અક્ષર **argv,
	     અચલ અક્ષર *prefix,
	     સંરચના repository *repo)
{
	સંરચના odb_fsck_options odb_fsck_opts = {
		.flags = ODB_FSCK_FULL,
		.object_cb = fsck_obj_buffer,
		.object_payload = repo,
	};
	પૂર્ણાંક keep_cache_objects = 0;
	પૂર્ણાંક name_objects = 0;
	પૂર્ણાંક check_references = 1;
	સંરચના option fsck_opts[] = {
		OPT__VERBOSE(&verbose, N_("be verbose")),
		OPT_BOOL(0, "unreachable", &show_unreachable, N_("show unreachable objects")),
		OPT_BOOL(0, "dangling", &show_dangling, N_("show dangling objects")),
		OPT_BOOL(0, "tags", &show_tags, N_("report tags")),
		OPT_BOOL(0, "root", &show_મૂળ, N_("report root nodes")),
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
	સંરચના snapshot snap = {
		.nr = 0,
		.alloc = 0,
		.ref = ખાલી
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
	જો (check_strict)
		fsck_obj_options.strict = 1;

	જો (show_progress == -1)
		show_progress = isatty(2);
	જો (verbose) {
		show_progress = 0;
		odb_fsck_opts.flags |= ODB_FSCK_VERBOSE;
	}
	જો (show_progress)
		odb_fsck_opts.flags |= ODB_FSCK_PROGRESS;

	જો (write_lost_and_found) {
		odb_fsck_opts.flags |= ODB_FSCK_FULL;
		include_reflogs = 0;
	}

	જો (name_objects)
		fsck_enable_object_names(&fsck_walk_options);

	repo_config(repo, git_fsck_config, &fsck_obj_options);
	prepare_repo_settings(repo);

	જો (check_references)
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

	જો (connectivity_only) {
		odb_for_each_object(repo->objects, ખાલી,
				    mark_object_for_connectivity, repo, 0);
	} નહીંતર {
		જો (odb_fsck(repo->objects, &odb_fsck_opts) < 0)
			errors_found |= ERROR_OBJECT;

		જો (fsck_finish(&fsck_obj_options))
			errors_found |= ERROR_OBJECT;
	}

	/* Process the snapshotted refs and the reflogs. */
	process_refs(repo, &snap);

	/* If not given any explicit objects, process index files too. */
	જો (!argc)
		keep_cache_objects = 1;
	જો (keep_cache_objects) {
		/*
		 * TODO: Consider first walking these indexes in snapshot_refs,
		 * to snapshot where the index entries used to point, and then
		 * check those snapshotted locations here.
		 */
		સંરચના worktree **worktrees, **p;

		verify_index_checksum = 1;
		verify_ce_order = 1;

		worktrees = get_worktrees(repo);
		માટે (p = worktrees; *p; p++) {
			સંરચના worktree *wt = *p;
			સંરચના index_state istate =
				INDEX_STATE_INIT(repo);
			અક્ષર *path, *wt_gitdir;

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
			મુક્ત(wt_gitdir);
			મુક્ત(path);
		}
		free_worktrees(worktrees);
	}

	check_connectivity(repo);

	જો (repo->settings.core_commit_graph) {
		સંરચના odb_source_files *files = odb_source_files_downcast(repo->objects->source);
		સંરચના child_process commit_graph_verify = CHILD_PROCESS_INIT;

		માટે (સંરચના odb_files_dir *dir = files->dirs; dir; dir = dir->next) {
			child_process_init(&commit_graph_verify);
			commit_graph_verify.git_cmd = 1;
			strvec_pushl(&commit_graph_verify.args, "commit-graph",
				     "verify", "--object-dir", dir->abspath, ખાલી);
			જો (show_progress)
				strvec_ધકેલો(&commit_graph_verify.args, "--progress");
			નહીંતર
				strvec_ધકેલો(&commit_graph_verify.args, "--no-progress");
			જો (run_command(&commit_graph_verify))
				errors_found |= ERROR_COMMIT_GRAPH;
		}
	}

	free_snapshot_refs(&snap);
	પાછા errors_found;
}
