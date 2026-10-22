#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "hex.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "commit.h"
#ಅನ್ತರ್ಭಾವ "tree.h"
#ಅನ್ತರ್ಭಾವ "blob.h"
#ಅನ್ತರ್ಭಾವ "tag.h"
#ಅನ್ತರ್ಭಾವ "refs.h"
#ಅನ್ತರ್ಭಾವ "cache-tree.h"
#ಅನ್ತರ್ಭಾವ "fsck.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "progress.h"
#ಅನ್ತರ್ಭಾವ "packfile.h"
#ಅನ್ತರ್ಭಾವ "object-name.h"
#ಅನ್ತರ್ಭಾವ "odb.h"
#ಅನ್ತರ್ಭಾವ "odb/streaming.h"
#ಅನ್ತರ್ಭಾವ "path.h"
#ಅನ್ತರ್ಭಾವ "read-cache-ll.h"
#ಅನ್ತರ್ಭಾವ "replace-object.h"
#ಅನ್ತರ್ಭಾವ "resolve-undo.h"
#ಅನ್ತರ್ಭಾವ "run-command.h"
#ಅನ್ತರ್ಭಾವ "sparse-index.h"
#ಅನ್ತರ್ಭಾವ "worktree.h"

#define REACHABLE 0x0001
#define SEEN      0x0002
#define HAS_OBJ   0x0004
/* This flag is set if something points to this object. */
#define USED      0x0008

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ show_ಬೇರು;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ show_tags;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ show_unreachable;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ include_reflogs = 1;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ connectivity_only;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ check_strict;
ಸ್ಥಿರ ಸಂರಚನಾ fsck_options fsck_walk_options;
ಸ್ಥಿರ ಸಂರಚನಾ fsck_options fsck_obj_options;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ errors_found;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ write_lost_and_found;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ verbose;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ show_progress = -1;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ show_dangling = 1;
ಸ್ಥಿರ timestamp_t now;
#define ERROR_OBJECT 01
#define ERROR_REACHABLE 02
#define ERROR_REFS 010
#define ERROR_COMMIT_GRAPH 020

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *describe_object(ಧ್ರುವ ಸಂರಚನಾ object_id *oid)
{
	ಪ್ರತ್ಯಾಗಮ fsck_describe_object(&fsck_walk_options, oid);
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *printable_type(ಸಂರಚನಾ repository *repo,
				  ಧ್ರುವ ಸಂರಚನಾ object_id *oid,
				  ಗಣನಾ object_type type)
{
	ಧ್ರುವ ವರ್ಣ *ret;

	ಯದಿ (type == OBJ_NONE)
		type = odb_read_object_info(repo->objects, oid, ರಿಕ್ತ);

	ret = type_name(type);
	ಯದಿ (!ret)
		ret = _("unknown");

	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ objerror(ಸಂರಚನಾ repository *repo, ಸಂರಚನಾ object *obj, ಧ್ರುವ ವರ್ಣ *err)
{
	errors_found |= ERROR_OBJECT;
	/* TRANSLATORS: e.g. error in tree 01bfda: <more explanation> */
	fprintf_ln(stderr, _("error in %s %s: %s"),
		   printable_type(repo, &obj->oid, obj->type),
		   describe_object(&obj->oid), err);
	ಪ್ರತ್ಯಾಗಮ -1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ fsck_objects_error_func(ಸಂರಚನಾ fsck_options *o,
				   ಶೂನ್ಯ *fsck_report,
				   ಗಣನಾ fsck_ಸಂದೇಶ_type ಸಂದೇಶ_type,
				   ಗಣನಾ fsck_ಸಂದೇಶ_id ಸಂದೇಶ_id UNUSED,
				   ಧ್ರುವ ವರ್ಣ *message)
{
	ಸಂರಚನಾ fsck_object_report *report = fsck_report;
	ಧ್ರುವ ಸಂರಚನಾ object_id *oid = report->oid;
	ಗಣನಾ object_type object_type = report->object_type;

	ವಿಕಲ್ಪ (ಸಂದೇಶ_type) {
	ಪಕ್ಷ FSCK_WARN:
		/* TRANSLATORS: e.g. warning in tree 01bfda: <more explanation> */
		fprintf_ln(stderr, _("warning in %s %s: %s"),
			   printable_type(o->repo, oid, object_type),
			   describe_object(oid), message);
		ಪ್ರತ್ಯಾಗಮ 0;
	ಪಕ್ಷ FSCK_ERROR:
		/* TRANSLATORS: e.g. error in tree 01bfda: <more explanation> */
		fprintf_ln(stderr, _("error in %s %s: %s"),
			   printable_type(o->repo, oid, object_type),
			   describe_object(oid), message);
		ಪ್ರತ್ಯಾಗಮ 1;
	ಸ್ವಭಾವ:
		BUG("%d (FSCK_IGNORE?) should never trigger this callback",
		    ಸಂದೇಶ_type);
	}
}

ಸ್ಥಿರ ಸಂರಚನಾ object_array pending;

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ mark_object(ಸಂರಚನಾ object *obj, ಗಣನಾ object_type type,
		       ಶೂನ್ಯ *data, ಸಂರಚನಾ fsck_options *options)
{
	ಸಂರಚನಾ object *parent = data;

	/*
	 * The only case data is NULL or type is OBJ_ANY is when
	 * mark_object_reachable() calls us.  All the callers of
	 * that function has non-NULL obj hence ...
	 */
	ಯದಿ (!obj) {
		/* ... these references to parent->fld are safe here */
		printf_ln(_("broken link from %7s %s"),
			  printable_type(options->repo, &parent->oid, parent->type),
			  describe_object(&parent->oid));
		printf_ln(_("broken link from %7s %s"),
			  (type == OBJ_ANY ? _("unknown") : type_name(type)),
			  _("unknown"));
		errors_found |= ERROR_REACHABLE;
		ಪ್ರತ್ಯಾಗಮ 1;
	}

	ಯದಿ (type != OBJ_ANY && obj->type != type)
		/* ... and the reference to parent is safe here */
		objerror(options->repo, parent, _("wrong object type in link"));

	ಯದಿ (obj->flags & REACHABLE)
		ಪ್ರತ್ಯಾಗಮ 0;
	obj->flags |= REACHABLE;

	ಯದಿ (is_promisor_object(options->repo, &obj->oid))
		/*
		 * Further recursion does not need to be performed on this
		 * object since it is a promisor object (so it does not need to
		 * be added to "pending").
		 */
		ಪ್ರತ್ಯಾಗಮ 0;

	ಯದಿ (!(obj->flags & HAS_OBJ)) {
		ಯದಿ (parent && !odb_has_object(options->repo->objects, &obj->oid,
					      ODB_HAS_OBJECT_RECHECK_PACKED)) {
			printf_ln(_("broken link from %7s %s\n"
				    "              to %7s %s"),
				  printable_type(options->repo, &parent->oid, parent->type),
				  describe_object(&parent->oid),
				  printable_type(options->repo, &obj->oid, obj->type),
				  describe_object(&obj->oid));
			errors_found |= ERROR_REACHABLE;
		}
		ಪ್ರತ್ಯಾಗಮ 1;
	}

	add_object_array(obj, ರಿಕ್ತ, &pending);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ mark_object_reachable(ಸಂರಚನಾ object *obj)
{
	mark_object(obj, OBJ_ANY, ರಿಕ್ತ, &fsck_walk_options);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ traverse_one_object(ಸಂರಚನಾ object *obj)
{
	ಪೂರ್ಣಾಙ್ಕ result = fsck_walk(obj, obj, &fsck_walk_options);

	ಯದಿ (obj->type == OBJ_ವೃಕ್ಷ) {
		ಸಂರಚನಾ ವೃಕ್ಷ *ವೃಕ್ಷ = (ಸಂರಚನಾ ವೃಕ್ಷ *)obj;
		free_ವೃಕ್ಷ_buffer(ವೃಕ್ಷ);
	}
	ಪ್ರತ್ಯಾಗಮ result;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ traverse_reachable(ಸಂರಚನಾ repository *repo)
{
	ಸಂರಚನಾ progress *progress = ರಿಕ್ತ;
	unsigned ಪೂರ್ಣಾಙ್ಕ nr = 0;
	ಪೂರ್ಣಾಙ್ಕ result = 0;
	ಯದಿ (show_progress)
		progress = start_delayed_progress(repo,
						  _("Checking connectivity"), 0);
	ಯಾವತ್ (pending.nr) {
		result |= traverse_one_object(object_array_ತೆಗೆ(&pending));
		display_progress(progress, ++nr);
	}
	stop_progress(&progress);
	ಪ್ರತ್ಯಾಗಮ !!result;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ mark_used(ಸಂರಚನಾ object *obj, ಗಣನಾ object_type type UNUSED,
		     ಶೂನ್ಯ *data UNUSED, ಸಂರಚನಾ fsck_options *options UNUSED)
{
	ಯದಿ (!obj)
		ಪ್ರತ್ಯಾಗಮ 1;
	obj->flags |= USED;
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ mark_unreachable_referents(ಧ್ರುವ ಸಂರಚನಾ object_id *oid,
				      ಸಂರಚನಾ object_info *oi UNUSED,
				      ಶೂನ್ಯ *data)
{
	ಸಂರಚನಾ repository *repo = data;
	ಸಂರಚನಾ fsck_options options;
	ಸಂರಚನಾ object *obj = lookup_object(data, oid);

	ಯದಿ (!obj || !(obj->flags & HAS_OBJ))
		ಪ್ರತ್ಯಾಗಮ 0; /* not part of our original set */
	ಯದಿ (obj->flags & REACHABLE)
		ಪ್ರತ್ಯಾಗಮ 0; /* reachable objects already traversed */

	/*
	 * Avoid passing OBJ_NONE to fsck_walk, which will parse the object
	 * (and we want to avoid parsing blobs).
	 */
	ಯದಿ (obj->type == OBJ_NONE) {
		ಗಣನಾ object_type type = odb_read_object_info(repo->objects,
							     &obj->oid, ರಿಕ್ತ);
		ಯದಿ (type > 0)
			object_as_type(obj, type, 0);
	}

	fsck_options_init(&options, repo, FSCK_OPTIONS_DEFAULT);
	options.walk = mark_used;
	fsck_walk(obj, ರಿಕ್ತ, &options);
	ಯದಿ (obj->type == OBJ_ವೃಕ್ಷ)
		free_ವೃಕ್ಷ_buffer((ಸಂರಚನಾ ವೃಕ್ಷ *)obj);

	ಪ್ರತ್ಯಾಗಮ 0;
}

/*
 * Check a single reachable object
 */
ಸ್ಥಿರ ಶೂನ್ಯ check_reachable_object(ಸಂರಚನಾ repository *repo, ಸಂರಚನಾ object *obj)
{
	/*
	 * We obviously want the object to be parsed,
	 * except if it was in a pack-file and we didn't
	 * do a full fsck
	 */
	ಯದಿ (!(obj->flags & HAS_OBJ)) {
		ಯದಿ (is_promisor_object(repo, &obj->oid))
			ಪ್ರತ್ಯಾಗಮ;
		ಯದಿ (has_object_pack(repo, &obj->oid))
			ಪ್ರತ್ಯಾಗಮ; /* it is in pack - forget about it */
		printf_ln(_("missing %s %s"),
			  printable_type(repo, &obj->oid, obj->type),
			  describe_object(&obj->oid));
		errors_found |= ERROR_REACHABLE;
		ಪ್ರತ್ಯಾಗಮ;
	}
}

/*
 * Check a single unreachable object
 */
ಸ್ಥಿರ ಶೂನ್ಯ check_unreachable_object(ಸಂರಚನಾ repository *repo, ಸಂರಚನಾ object *obj)
{
	/*
	 * Missing unreachable object? Ignore it. It's not like
	 * we miss it (since it can't be reached), nor do we want
	 * to complain about it being unreachable (since it does
	 * not exist).
	 */
	ಯದಿ (!(obj->flags & HAS_OBJ))
		ಪ್ರತ್ಯಾಗಮ;

	/*
	 * Unreachable object that exists? Show it if asked to,
	 * since this is something that is prunable.
	 */
	ಯದಿ (show_unreachable) {
		printf_ln(_("unreachable %s %s"),
			  printable_type(repo, &obj->oid, obj->type),
			  describe_object(&obj->oid));
		ಪ್ರತ್ಯಾಗಮ;
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
	ಯದಿ (!(obj->flags & USED)) {
		ಯದಿ (show_dangling)
			printf_ln(_("dangling %s %s"),
				  printable_type(repo, &obj->oid, obj->type),
				  describe_object(&obj->oid));
		ಯದಿ (write_lost_and_found) {
			ವರ್ಣ *filename = repo_git_path(repo, "lost-found/%s/%s",
				obj->type == OBJ_COMMIT ? "commit" : "other",
				describe_object(&obj->oid));
			FILE *f;

			ಯದಿ (safe_create_leading_directories_const(repo, filename)) {
				error(_("could not create lost-found"));
				ಮುಕ್ತ(filename);
				ಪ್ರತ್ಯಾಗಮ;
			}
			f = xfopen(filename, "w");
			ಯದಿ (obj->type == OBJ_BLOB) {
				ಯದಿ (odb_stream_blob_to_fd(repo->objects, fileno(f),
							  &obj->oid, ರಿಕ್ತ, 1))
					die_errno(_("could not write '%s'"), filename);
			} ಅನ್ಯಥಾ
				fprintf(f, "%s\n", describe_object(&obj->oid));
			ಯದಿ (fclose(f))
				die_errno(_("could not finish '%s'"),
					  filename);
			ಮುಕ್ತ(filename);
		}
		ಪ್ರತ್ಯಾಗಮ;
	}

	/*
	 * Otherwise? It's there, it's unreachable, and some other unreachable
	 * object points to it. Ignore it - it's not interesting, and we showed
	 * all the interesting cases above.
	 */
}

ಸ್ಥಿರ ಶೂನ್ಯ check_object(ಸಂರಚನಾ repository *repo, ಸಂರಚನಾ object *obj)
{
	ಯದಿ (verbose)
		fprintf_ln(stderr, _("Checking %s"), describe_object(&obj->oid));

	ಯದಿ (obj->flags & REACHABLE)
		check_reachable_object(repo, obj);
	ಅನ್ಯಥಾ
		check_unreachable_object(repo, obj);
}

ಸ್ಥಿರ ಶೂನ್ಯ check_connectivity(ಸಂರಚನಾ repository *repo)
{
	ಪೂರ್ಣಾಙ್ಕ i, max;

	/* Traverse the pending reachable objects */
	traverse_reachable(repo);

	/*
	 * With --connectivity-only, we won't have actually opened and marked
	 * unreachable objects with USED. Do that now to make --dangling, etc
	 * accurate.
	 */
	ಯದಿ (connectivity_only && (show_dangling || write_lost_and_found)) {
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
		odb_for_each_object(repo->objects, ರಿಕ್ತ,
				    mark_unreachable_referents, repo, 0);
	}

	/* Look up all the requirements, warn about missing objects.. */
	max = get_max_object_index(repo);
	ಯದಿ (verbose)
		fprintf_ln(stderr, _("Checking connectivity (%d objects)"), max);

	ಪ್ರತಿ (i = 0; i < max; i++) {
		ಸಂರಚನಾ object *obj = get_indexed_object(repo, i);

		ಯದಿ (obj)
			check_object(repo, obj);
	}
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ fsck_obj_buffer(ಧ್ರುವ ಸಂರಚನಾ object_id *oid, ಗಣನಾ object_type type,
			   unsigned long size, ಶೂನ್ಯ *buffer, ಪೂರ್ಣಾಙ್ಕ *eaten, ಶೂನ್ಯ *cb_data)
{
	ಸಂರಚನಾ repository *repo = cb_data;
	ಸಂರಚನಾ object *obj;
	ಪೂರ್ಣಾಙ್ಕ err;

	/*
	 * Note, buffer may be NULL if type is OBJ_BLOB. See
	 * verify_packfile(), data_valid variable for details.
	 */
	obj = parse_object_buffer(repo, oid, type, size, buffer, eaten);
	ಯದಿ (!obj) {
		errors_found |= ERROR_OBJECT;
		err = error(_("%s: object corrupt or missing"),
			    oid_to_hex(oid));
		ಗಚ್ಛ out;
	}

	obj->flags &= ~REACHABLE;
	obj->flags |= HAS_OBJ | SEEN;

	ಯದಿ (verbose)
		fprintf_ln(stderr, _("Checking %s %s"),
			   printable_type(repo, &obj->oid, obj->type),
			   describe_object(&obj->oid));

	ಯದಿ (fsck_walk(obj, ರಿಕ್ತ, &fsck_obj_options))
		objerror(repo, obj, _("broken links"));

	err = fsck_object(obj, buffer, size, &fsck_obj_options);
	ಯದಿ (err)
		ಗಚ್ಛ out;

	ಯದಿ (obj->type == OBJ_COMMIT) {
		ಸಂರಚನಾ commit *commit = (ಸಂರಚನಾ commit *) obj;

		ಯದಿ (!commit->parents && show_ಬೇರು)
			printf_ln(_("root %s"),
				  describe_object(&commit->object.oid));
	}

	ಯದಿ (obj->type == OBJ_TAG) {
		ಸಂರಚನಾ tag *tag = (ಸಂರಚನಾ tag *) obj;

		ಯದಿ (show_tags && tag->tagged) {
			printf_ln(_("tagged %s %s (%s) in %s"),
				  printable_type(repo, &tag->tagged->oid, tag->tagged->type),
				  describe_object(&tag->tagged->oid),
				  tag->tag,
				  describe_object(&tag->object.oid));
		}
	}

out:
	ಯದಿ (obj && obj->type == OBJ_ವೃಕ್ಷ)
		free_ವೃಕ್ಷ_buffer((ಸಂರಚನಾ ವೃಕ್ಷ *)obj);
	ಪ್ರತ್ಯಾಗಮ err;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ default_refs;

ಸ್ಥಿರ ಶೂನ್ಯ fsck_handle_reflog_oid(ಸಂರಚನಾ repository *repo,
				   ಧ್ರುವ ವರ್ಣ *refname, ಸಂರಚನಾ object_id *oid,
				   timestamp_t timestamp)
{
	ಸಂರಚನಾ object *obj;

	ಯದಿ (!is_null_oid(oid)) {
		obj = lookup_object(repo, oid);
		ಯದಿ (obj && (obj->flags & HAS_OBJ)) {
			ಯದಿ (timestamp)
				fsck_put_object_name(&fsck_walk_options, oid,
						     "%s@{%"PRItime"}",
						     refname, timestamp);
			obj->flags |= USED;
			mark_object_reachable(obj);
		} ಅನ್ಯಥಾ ಯದಿ (!is_promisor_object(repo, oid)) {
			error(_("%s: invalid reflog entry %s"),
			      refname, oid_to_hex(oid));
			errors_found |= ERROR_REACHABLE;
		}
	}
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ fsck_handle_reflog_ent(ಧ್ರುವ ವರ್ಣ *refname,
				  ಸಂರಚನಾ object_id *ooid, ಸಂರಚನಾ object_id *noid,
				  ಧ್ರುವ ವರ್ಣ *email UNUSED,
				  timestamp_t timestamp, ಪೂರ್ಣಾಙ್ಕ tz UNUSED,
				  ಧ್ರುವ ವರ್ಣ *message UNUSED, ಶೂನ್ಯ *cb_data)
{
	ಸಂರಚನಾ repository *repo = cb_data;

	ಯದಿ (now && timestamp > now)
		ಪ್ರತ್ಯಾಗಮ 0;

	ಯದಿ (verbose)
		fprintf_ln(stderr, _("Checking reflog %s->%s"),
			   oid_to_hex(ooid), oid_to_hex(noid));

	fsck_handle_reflog_oid(repo, refname, ooid, 0);
	fsck_handle_reflog_oid(repo, refname, noid, timestamp);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ fsck_handle_reflog(ಧ್ರುವ ವರ್ಣ *logname, ಶೂನ್ಯ *cb_data)
{
	ಸಂರಚನಾ strbuf refname = STRBUF_INIT;
	ಸಂರಚನಾ worktree *wt = cb_data;

	strbuf_worktree_ref(wt, &refname, logname);
	refs_for_each_reflog_ent(get_main_ref_store(wt->repo),
				 refname.buf, fsck_handle_reflog_ent,
				 wt->repo);
	strbuf_release(&refname);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸಂರಚನಾ ref_snapshot {
	ವರ್ಣ *refname;
	ಸಂರಚನಾ object_id oid;
	/* TODO: Maybe supplement with latest reflog entry info too? */
};

ಸಂರಚನಾ snapshot {
	size_t nr;
	size_t alloc;
	ಸಂರಚನಾ ref_snapshot *ref;
	/* TODO: Consider also snapshotting the index of each worktree. */
};

ಸಂರಚನಾ snapshot_ref_data {
	ಸಂರಚನಾ repository *repo;
	ಸಂರಚನಾ snapshot *snap;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ snapshot_ref(ಧ್ರುವ ಸಂರಚನಾ reference *ref, ಶೂನ್ಯ *cb_data)
{
	ಸಂರಚನಾ snapshot_ref_data *data = cb_data;
	ಸಂರಚನಾ snapshot *snap = data->snap;
	ಸಂರಚನಾ object *obj;

	obj = parse_object(data->repo, ref->oid);
	ಯದಿ (!obj) {
		ಯದಿ (is_promisor_object(data->repo, ref->oid)) {
			/*
			 * Increment default_refs anyway, because this is a
			 * valid ref.
			 */
			 default_refs++;
			 ಪ್ರತ್ಯಾಗಮ 0;
		}
		error(_("%s: invalid sha1 pointer %s"),
		      ref->name, oid_to_hex(ref->oid));
		errors_found |= ERROR_REACHABLE;
		/* We'll continue with the rest despite the error.. */
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (obj->type != OBJ_COMMIT && is_branch(ref->name)) {
		error(_("%s: not a commit"), ref->name);
		errors_found |= ERROR_REFS;
	}
	default_refs++;

	ALLOC_GROW(snap->ref, snap->nr + 1, snap->alloc);
	snap->ref[snap->nr].refname = xstrdup(ref->name);
	oidcpy(&snap->ref[snap->nr].oid, ref->oid);
	snap->nr++;

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ fsck_handle_ref(ಧ್ರುವ ಸಂರಚನಾ reference *ref, ಶೂನ್ಯ *cb_data)
{
	ಸಂರಚನಾ repository *repo = cb_data;
	ಸಂರಚನಾ object *obj;

	obj = parse_object(repo, ref->oid);
	obj->flags |= USED;
	fsck_put_object_name(&fsck_walk_options,
			     ref->oid, "%s", ref->name);
	mark_object_reachable(obj);

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ snapshot_refs(ಸಂರಚನಾ repository *repo,
			  ಸಂರಚನಾ snapshot *snap, ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv)
{
	ಸಂರಚನಾ refs_for_each_ref_options opts = {
		.flags = REFS_FOR_EACH_INCLUDE_BROKEN,
	};
	ಸಂರಚನಾ snapshot_ref_data data = {
		.repo = repo,
		.snap = snap,
	};
	ಸಂರಚನಾ worktree **worktrees, **p;
	ಧ್ರುವ ವರ್ಣ *head_points_at;
	ಸಂರಚನಾ object_id head_oid;

	ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < argc; i++) {
		ಧ್ರುವ ವರ್ಣ *arg = argv[i];
		ಸಂರಚನಾ object_id oid;
		ಯದಿ (!repo_get_oid(repo, arg, &oid)) {
			ಸಂರಚನಾ reference ref = {
				.name = arg,
				.oid = &oid,
			};

			snapshot_ref(&ref, &data);
			ಅನುವರ್ತ;
		}
		error(_("invalid parameter: expected sha1, got '%s'"), arg);
		errors_found |= ERROR_OBJECT;
	}

	ಯದಿ (argc) {
		include_reflogs = 0;
		ಪ್ರತ್ಯಾಗಮ;
	}

	refs_for_each_ref_ext(get_main_ref_store(repo),
			      snapshot_ref, &data, &opts);

	worktrees = get_worktrees(repo);
	ಪ್ರತಿ (p = worktrees; *p; p++) {
		ಸಂರಚನಾ worktree *wt = *p;
		ಸಂರಚನಾ strbuf refname = STRBUF_INIT;

		strbuf_worktree_ref(wt, &refname, "HEAD");

		head_points_at = refs_resolve_ref_unsafe(get_main_ref_store(repo),
							 refname.buf, 0, &head_oid, ರಿಕ್ತ);

		ಯದಿ (head_points_at && !is_null_oid(&head_oid)) {
			ಸಂರಚನಾ reference ref = {
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
	now = time(ರಿಕ್ತ);
}


ಸ್ಥಿರ ಶೂನ್ಯ free_snapshot_refs(ಸಂರಚನಾ snapshot *snap)
{
	ಪ್ರತಿ (size_t i = 0; i < snap->nr; i++)
		ಮುಕ್ತ(snap->ref[i].refname);
	ಮುಕ್ತ(snap->ref);
}

ಸ್ಥಿರ ಶೂನ್ಯ process_refs(ಸಂರಚನಾ repository *repo, ಸಂರಚನಾ snapshot *snap)
{
	ಸಂರಚನಾ worktree **worktrees, **p;

	ಪ್ರತಿ (size_t i = 0; i < snap->nr; i++) {
		ಸಂರಚನಾ reference ref = {
			.name = snap->ref[i].refname,
			.oid = &snap->ref[i].oid,
		};
		fsck_handle_ref(&ref, repo);
	}

	ಯದಿ (include_reflogs) {
		worktrees = get_worktrees(repo);
		ಪ್ರತಿ (p = worktrees; *p; p++) {
			ಸಂರಚನಾ worktree *wt = *p;

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
	ಯದಿ (!default_refs) {
		fprintf_ln(stderr, _("notice: No default references"));
		show_unreachable = 0;
	}
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ fsck_cache_ವೃಕ್ಷ(ಸಂರಚನಾ repository *repo, ಸಂರಚನಾ cache_ವೃಕ್ಷ *it, ಧ್ರುವ ವರ್ಣ *index_path)
{
	ಪೂರ್ಣಾಙ್ಕ i;
	ಪೂರ್ಣಾಙ್ಕ err = 0;

	ಯದಿ (verbose)
		fprintf_ln(stderr, _("Checking cache tree of %s"), index_path);

	ಯದಿ (0 <= it->entry_ಎಣಿಕೆ) {
		ಸಂರಚನಾ object *obj = parse_object(repo, &it->oid);
		ಯದಿ (!obj) {
			error(_("%s: invalid sha1 pointer in cache-tree of %s"),
			      oid_to_hex(&it->oid), index_path);
			errors_found |= ERROR_REFS;
			ಪ್ರತ್ಯಾಗಮ 1;
		}
		obj->flags |= USED;
		fsck_put_object_name(&fsck_walk_options, &it->oid, ":");
		mark_object_reachable(obj);
		ಯದಿ (obj->type != OBJ_ವೃಕ್ಷ)
			err |= objerror(repo, obj, _("non-tree in cache-tree"));
	}
	ಪ್ರತಿ (i = 0; i < it->subtree_nr; i++)
		err |= fsck_cache_ವೃಕ್ಷ(repo, it->down[i]->cache_ವೃಕ್ಷ, index_path);
	ಪ್ರತ್ಯಾಗಮ err;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ fsck_resolve_undo(ಸಂರಚನಾ index_state *istate,
			     ಧ್ರುವ ವರ್ಣ *index_path)
{
	ಸಂರಚನಾ string_list_item *item;
	ಸಂರಚನಾ string_list *resolve_undo = istate->resolve_undo;

	ಯದಿ (!resolve_undo)
		ಪ್ರತ್ಯಾಗಮ 0;

	for_each_string_list_item(item, resolve_undo) {
		ಧ್ರುವ ವರ್ಣ *path = item->string;
		ಸಂರಚನಾ resolve_undo_info *ru = item->util;
		ಪೂರ್ಣಾಙ್ಕ i;

		ಯದಿ (!ru)
			ಅನುವರ್ತ;
		ಪ್ರತಿ (i = 0; i < 3; i++) {
			ಸಂರಚನಾ object *obj;

			ಯದಿ (!ru->mode[i] || !S_ISREG(ru->mode[i]))
				ಅನುವರ್ತ;

			obj = parse_object(istate->repo, &ru->oid[i]);
			ಯದಿ (!obj) {
				error(_("%s: invalid sha1 pointer in resolve-undo of %s"),
				      oid_to_hex(&ru->oid[i]),
				      index_path);
				errors_found |= ERROR_REFS;
				ಅನುವರ್ತ;
			}
			obj->flags |= USED;
			fsck_put_object_name(&fsck_walk_options, &ru->oid[i],
					     ":(%d):%s", i, path);
			mark_object_reachable(obj);
		}
	}
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ fsck_index(ಸಂರಚನಾ index_state *istate, ಧ್ರುವ ವರ್ಣ *index_path,
		       ಪೂರ್ಣಾಙ್ಕ is_current_worktree)
{
	unsigned ಪೂರ್ಣಾಙ್ಕ i;

	/* TODO: audit for interaction with sparse-index. */
	ensure_full_index(istate);
	ಪ್ರತಿ (i = 0; i < istate->cache_nr; i++) {
		unsigned ಪೂರ್ಣಾಙ್ಕ mode;
		ಸಂರಚನಾ blob *blob;
		ಸಂರಚನಾ object *obj;

		mode = istate->cache[i]->ce_mode;
		ಯದಿ (S_ISGITLINK(mode))
			ಅನುವರ್ತ;
		blob = lookup_blob(istate->repo,
				   &istate->cache[i]->oid);
		ಯದಿ (!blob)
			ಅನುವರ್ತ;
		obj = &blob->object;
		obj->flags |= USED;
		fsck_put_object_name(&fsck_walk_options, &obj->oid,
				     "%s:%s",
				     is_current_worktree ? "" : index_path,
				     istate->cache[i]->name);
		mark_object_reachable(obj);
	}
	ಯದಿ (istate->cache_ವೃಕ್ಷ)
		fsck_cache_ವೃಕ್ಷ(istate->repo, istate->cache_ವೃಕ್ಷ, index_path);
	fsck_resolve_undo(istate, index_path);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ mark_object_for_connectivity(ಧ್ರುವ ಸಂರಚನಾ object_id *oid,
					ಸಂರಚನಾ object_info *oi UNUSED,
					ಶೂನ್ಯ *cb_data)
{
	ಸಂರಚನಾ repository *repo = cb_data;
	ಸಂರಚನಾ object *obj = lookup_unknown_object(repo, oid);
	obj->flags |= HAS_OBJ;
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ fsck_refs(ಸಂರಚನಾ repository *r)
{
	ಸಂರಚನಾ child_process refs_verify = CHILD_PROCESS_INIT;
	ಸಂರಚನಾ progress *progress = ರಿಕ್ತ;

	ಯದಿ (show_progress)
		progress = start_progress(r, _("Checking ref database"), 1);

	ಯದಿ (verbose)
		fprintf_ln(stderr, _("Checking ref database"));

	child_process_init(&refs_verify);
	refs_verify.git_cmd = 1;
	strvec_pushl(&refs_verify.args, "refs", "verify", ರಿಕ್ತ);
	ಯದಿ (verbose)
		strvec_ತಳ್ಳು(&refs_verify.args, "--verbose");
	ಯದಿ (check_strict)
		strvec_ತಳ್ಳು(&refs_verify.args, "--strict");

	ಯದಿ (run_command(&refs_verify))
		errors_found |= ERROR_REFS;

	display_progress(progress, 1);
	stop_progress(&progress);
}

ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ * ಧ್ರುವ fsck_usage[] = {
	N_("git fsck [--tags] [--root] [--unreachable] [--cache] [--no-reflogs]\n"
	   "         [--[no-]full] [--strict] [--verbose] [--lost-found]\n"
	   "         [--[no-]dangling] [--[no-]progress] [--connectivity-only]\n"
	   "         [--[no-]name-objects] [--[no-]references] [<object>...]"),
	ರಿಕ್ತ
};

ಪೂರ್ಣಾಙ್ಕ cmd_fsck(ಪೂರ್ಣಾಙ್ಕ argc,
	     ಧ್ರುವ ವರ್ಣ **argv,
	     ಧ್ರುವ ವರ್ಣ *prefix,
	     ಸಂರಚನಾ repository *repo)
{
	ಸಂರಚನಾ odb_fsck_options odb_fsck_opts = {
		.flags = ODB_FSCK_FULL,
		.object_cb = fsck_obj_buffer,
		.object_payload = repo,
	};
	ಪೂರ್ಣಾಙ್ಕ keep_cache_objects = 0;
	ಪೂರ್ಣಾಙ್ಕ name_objects = 0;
	ಪೂರ್ಣಾಙ್ಕ check_references = 1;
	ಸಂರಚನಾ option fsck_opts[] = {
		OPT__VERBOSE(&verbose, N_("be verbose")),
		OPT_BOOL(0, "unreachable", &show_unreachable, N_("show unreachable objects")),
		OPT_BOOL(0, "dangling", &show_dangling, N_("show dangling objects")),
		OPT_BOOL(0, "tags", &show_tags, N_("report tags")),
		OPT_BOOL(0, "root", &show_ಬೇರು, N_("report root nodes")),
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
	ಸಂರಚನಾ snapshot snap = {
		.nr = 0,
		.alloc = 0,
		.ref = ರಿಕ್ತ
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
	ಯದಿ (check_strict)
		fsck_obj_options.strict = 1;

	ಯದಿ (show_progress == -1)
		show_progress = isatty(2);
	ಯದಿ (verbose) {
		show_progress = 0;
		odb_fsck_opts.flags |= ODB_FSCK_VERBOSE;
	}
	ಯದಿ (show_progress)
		odb_fsck_opts.flags |= ODB_FSCK_PROGRESS;

	ಯದಿ (write_lost_and_found) {
		odb_fsck_opts.flags |= ODB_FSCK_FULL;
		include_reflogs = 0;
	}

	ಯದಿ (name_objects)
		fsck_enable_object_names(&fsck_walk_options);

	repo_config(repo, git_fsck_config, &fsck_obj_options);
	prepare_repo_settings(repo);

	ಯದಿ (check_references)
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

	ಯದಿ (connectivity_only) {
		odb_for_each_object(repo->objects, ರಿಕ್ತ,
				    mark_object_for_connectivity, repo, 0);
	} ಅನ್ಯಥಾ {
		ಯದಿ (odb_fsck(repo->objects, &odb_fsck_opts) < 0)
			errors_found |= ERROR_OBJECT;

		ಯದಿ (fsck_finish(&fsck_obj_options))
			errors_found |= ERROR_OBJECT;
	}

	/* Process the snapshotted refs and the reflogs. */
	process_refs(repo, &snap);

	/* If not given any explicit objects, process index files too. */
	ಯದಿ (!argc)
		keep_cache_objects = 1;
	ಯದಿ (keep_cache_objects) {
		/*
		 * TODO: Consider first walking these indexes in snapshot_refs,
		 * to snapshot where the index entries used to point, and then
		 * check those snapshotted locations here.
		 */
		ಸಂರಚನಾ worktree **worktrees, **p;

		verify_index_checksum = 1;
		verify_ce_order = 1;

		worktrees = get_worktrees(repo);
		ಪ್ರತಿ (p = worktrees; *p; p++) {
			ಸಂರಚನಾ worktree *wt = *p;
			ಸಂರಚನಾ index_state istate =
				INDEX_STATE_INIT(repo);
			ವರ್ಣ *path, *wt_gitdir;

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
			ಮುಕ್ತ(wt_gitdir);
			ಮುಕ್ತ(path);
		}
		free_worktrees(worktrees);
	}

	check_connectivity(repo);

	ಯದಿ (repo->settings.core_commit_graph) {
		ಸಂರಚನಾ odb_source_files *files = odb_source_files_downcast(repo->objects->source);
		ಸಂರಚನಾ child_process commit_graph_verify = CHILD_PROCESS_INIT;

		ಪ್ರತಿ (ಸಂರಚನಾ odb_files_dir *dir = files->dirs; dir; dir = dir->next) {
			child_process_init(&commit_graph_verify);
			commit_graph_verify.git_cmd = 1;
			strvec_pushl(&commit_graph_verify.args, "commit-graph",
				     "verify", "--object-dir", dir->abspath, ರಿಕ್ತ);
			ಯದಿ (show_progress)
				strvec_ತಳ್ಳು(&commit_graph_verify.args, "--progress");
			ಅನ್ಯಥಾ
				strvec_ತಳ್ಳು(&commit_graph_verify.args, "--no-progress");
			ಯದಿ (run_command(&commit_graph_verify))
				errors_found |= ERROR_COMMIT_GRAPH;
		}
	}

	free_snapshot_refs(&snap);
	ಪ್ರತ್ಯಾಗಮ errors_found;
}
