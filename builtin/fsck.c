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

#ವ್ಯಾಖ್ಯಾನಿಸು REACHABLE 0x0001
#ವ್ಯಾಖ್ಯಾನಿಸು ನೋಡಿದ_2      0x0002
#ವ್ಯಾಖ್ಯಾನಿಸು HAS_OBJ   0x0004
/* This flag is set if something points to this object. */
#ವ್ಯಾಖ್ಯಾನಿಸು ಬಳಸಿದ      0x0008

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ show_ಬೇರು;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ show_ಟ್ಯಾಗುಗಳು;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ show_unreachable;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ include_reflogs = 1;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ connectivity_only;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ check_strict;
ಸ್ಥಿರ ರಚನೆ fsck_options fsck_walk_options;
ಸ್ಥಿರ ರಚನೆ fsck_options fsck_obj_options;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ errors_ಸಿಕ್ಕಿತು;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ write_ಕಳೆದುಹೋದ_and_ಸಿಕ್ಕಿತು;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ verbose;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ show_ಪ್ರಗತಿ = -1;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ show_dangling = 1;
ಸ್ಥಿರ timestamp_t now;
#ವ್ಯಾಖ್ಯಾನಿಸು ERROR_OBJECT 01
#ವ್ಯಾಖ್ಯಾನಿಸು ERROR_REACHABLE 02
#ವ್ಯಾಖ್ಯಾನಿಸು ERROR_REFS 010
#ವ್ಯಾಖ್ಯಾನಿಸು ERROR_COMMIT_ನಕ್ಷೆ 020

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *describe_object(ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid)
{
	ಹಿಂತಿರುಗಿ fsck_describe_object(&fsck_walk_options, oid);
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *printable_type(ರಚನೆ repository *repo,
				  ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid,
				  ಎಣಿಕೆ object_type type)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ret;

	ಆದರೆ (type == OBJ_NONE)
		type = odb_read_object_info(repo->objects, oid, ಖಾಲಿ);

	ret = type_name(type);
	ಆದರೆ (!ret)
		ret = _("unknown");

	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ objerror(ರಚನೆ repository *repo, ರಚನೆ object *obj, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *err)
{
	errors_ಸಿಕ್ಕಿತು |= ERROR_OBJECT;
	/* TRANSLATORS: e.g. error in tree 01bfda: <more explanation> */
	fprintf_ln(stderr, _("error in %s %s: %s"),
		   printable_type(repo, &obj->oid, obj->type),
		   describe_object(&obj->oid), err);
	ಹಿಂತಿರುಗಿ -1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ fsck_objects_error_func(ರಚನೆ fsck_options *o,
				   ಶೂನ್ಯ *fsck_report,
				   ಎಣಿಕೆ fsck_ಸಂದೇಶ_type ಸಂದೇಶ_type,
				   ಎಣಿಕೆ fsck_ಸಂದೇಶ_id ಸಂದೇಶ_id_2 ಬಳಸದ,
				   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ_2)
{
	ರಚನೆ fsck_object_report *report = fsck_report;
	ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid = report->oid;
	ಎಣಿಕೆ object_type object_type = report->object_type;

	ಆಯ್ಕೆ (ಸಂದೇಶ_type) {
	ಸ್ಥಿತಿ FSCK_WARN:
		/* TRANSLATORS: e.g. warning in tree 01bfda: <more explanation> */
		fprintf_ln(stderr, _("warning in %s %s: %s"),
			   printable_type(o->repo, oid, object_type),
			   describe_object(oid), ಸಂದೇಶ_2);
		ಹಿಂತಿರುಗಿ 0;
	ಸ್ಥಿತಿ FSCK_ERROR:
		/* TRANSLATORS: e.g. error in tree 01bfda: <more explanation> */
		fprintf_ln(stderr, _("error in %s %s: %s"),
			   printable_type(o->repo, oid, object_type),
			   describe_object(oid), ಸಂದೇಶ_2);
		ಹಿಂತಿರುಗಿ 1;
	ಪೂರ್ವನಿಯೋಜಿತ:
		BUG("%d (FSCK_IGNORE?) should never trigger this callback",
		    ಸಂದೇಶ_type);
	}
}

ಸ್ಥಿರ ರಚನೆ object_array pending;

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ mark_object(ರಚನೆ object *obj, ಎಣಿಕೆ object_type type,
		       ಶೂನ್ಯ *ದತ್ತಾಂಶ, ರಚನೆ fsck_options *options)
{
	ರಚನೆ object *ಪೋಷಕ = ದತ್ತಾಂಶ;

	/*
	 * The only case data is NULL or type is OBJ_ANY is when
	 * mark_object_reachable() calls us.  All the callers of
	 * that function has non-NULL obj hence ...
	 */
	ಆದರೆ (!obj) {
		/* ... these references to parent->fld are safe here */
		printf_ln(_("broken link from %7s %s"),
			  printable_type(options->repo, &ಪೋಷಕ->oid, ಪೋಷಕ->type),
			  describe_object(&ಪೋಷಕ->oid));
		printf_ln(_("broken link from %7s %s"),
			  (type == OBJ_ANY ? _("unknown") : type_name(type)),
			  _("unknown"));
		errors_ಸಿಕ್ಕಿತು |= ERROR_REACHABLE;
		ಹಿಂತಿರುಗಿ 1;
	}

	ಆದರೆ (type != OBJ_ANY && obj->type != type)
		/* ... and the reference to parent is safe here */
		objerror(options->repo, ಪೋಷಕ, _("wrong object type in link"));

	ಆದರೆ (obj->flags & REACHABLE)
		ಹಿಂತಿರುಗಿ 0;
	obj->flags |= REACHABLE;

	ಆದರೆ (is_promisor_object(options->repo, &obj->oid))
		/*
		 * Further recursion does not need to be performed on this
		 * object since it is a promisor object (so it does not need to
		 * be added to "pending").
		 */
		ಹಿಂತಿರುಗಿ 0;

	ಆದರೆ (!(obj->flags & HAS_OBJ)) {
		ಆದರೆ (ಪೋಷಕ && !odb_has_object(options->repo->objects, &obj->oid,
					      ODB_HAS_OBJECT_RECHECK_PACKED)) {
			printf_ln(_("broken link from %7s %s\n"
				    "              to %7s %s"),
				  printable_type(options->repo, &ಪೋಷಕ->oid, ಪೋಷಕ->type),
				  describe_object(&ಪೋಷಕ->oid),
				  printable_type(options->repo, &obj->oid, obj->type),
				  describe_object(&obj->oid));
			errors_ಸಿಕ್ಕಿತು |= ERROR_REACHABLE;
		}
		ಹಿಂತಿರುಗಿ 1;
	}

	ಕೂಡಿಸು_object_array(obj, ಖಾಲಿ, &pending);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ mark_object_reachable(ರಚನೆ object *obj)
{
	mark_object(obj, OBJ_ANY, ಖಾಲಿ, &fsck_walk_options);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ traverse_one_object(ರಚನೆ object *obj)
{
	ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ = fsck_walk(obj, obj, &fsck_walk_options);

	ಆದರೆ (obj->type == OBJ_ವೃಕ್ಷ) {
		ರಚನೆ ವೃಕ್ಷ *ವೃಕ್ಷ = (ರಚನೆ ವೃಕ್ಷ *)obj;
		free_ವೃಕ್ಷ_ಬಫರ್(ವೃಕ್ಷ);
	}
	ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ traverse_reachable(ರಚನೆ repository *repo)
{
	ರಚನೆ ಪ್ರಗತಿ *ಪ್ರಗತಿ = ಖಾಲಿ;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ nr = 0;
	ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ = 0;
	ಆದರೆ (show_ಪ್ರಗತಿ)
		ಪ್ರಗತಿ = ಆರಂಭ_delayed_ಪ್ರಗತಿ(repo,
						  _("Checking connectivity"), 0);
	ಆಗ (pending.nr) {
		ಫಲಿತಾಂಶ |= traverse_one_object(object_array_ತೆಗೆ(&pending));
		display_ಪ್ರಗತಿ(ಪ್ರಗತಿ, ++nr);
	}
	stop_ಪ್ರಗತಿ(&ಪ್ರಗತಿ);
	ಹಿಂತಿರುಗಿ !!ಫಲಿತಾಂಶ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ mark_ಬಳಸಿದ(ರಚನೆ object *obj, ಎಣಿಕೆ object_type type ಬಳಸದ,
		     ಶೂನ್ಯ *ದತ್ತಾಂಶ ಬಳಸದ, ರಚನೆ fsck_options *options ಬಳಸದ)
{
	ಆದರೆ (!obj)
		ಹಿಂತಿರುಗಿ 1;
	obj->flags |= ಬಳಸಿದ;
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ mark_unreachable_referents(ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid,
				      ರಚನೆ object_info *oi ಬಳಸದ,
				      ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
	ರಚನೆ repository *repo = ದತ್ತಾಂಶ;
	ರಚನೆ fsck_options options;
	ರಚನೆ object *obj = lookup_object(ದತ್ತಾಂಶ, oid);

	ಆದರೆ (!obj || !(obj->flags & HAS_OBJ))
		ಹಿಂತಿರುಗಿ 0; /* not part of our original set */
	ಆದರೆ (obj->flags & REACHABLE)
		ಹಿಂತಿರುಗಿ 0; /* reachable objects already traversed */

	/*
	 * Avoid passing OBJ_NONE to fsck_walk, which will parse the object
	 * (and we want to avoid parsing blobs).
	 */
	ಆದರೆ (obj->type == OBJ_NONE) {
		ಎಣಿಕೆ object_type type = odb_read_object_info(repo->objects,
							     &obj->oid, ಖಾಲಿ);
		ಆದರೆ (type > 0)
			object_as_type(obj, type, 0);
	}

	fsck_options_init(&options, repo, FSCK_OPTIONS_DEFAULT);
	options.walk = mark_ಬಳಸಿದ;
	fsck_walk(obj, ಖಾಲಿ, &options);
	ಆದರೆ (obj->type == OBJ_ವೃಕ್ಷ)
		free_ವೃಕ್ಷ_ಬಫರ್((ರಚನೆ ವೃಕ್ಷ *)obj);

	ಹಿಂತಿರುಗಿ 0;
}

/*
 * Check a single reachable object
 */
ಸ್ಥಿರ ಶೂನ್ಯ check_reachable_object(ರಚನೆ repository *repo, ರಚನೆ object *obj)
{
	/*
	 * We obviously want the object to be parsed,
	 * except if it was in a pack-file and we didn't
	 * do a full fsck
	 */
	ಆದರೆ (!(obj->flags & HAS_OBJ)) {
		ಆದರೆ (is_promisor_object(repo, &obj->oid))
			ಹಿಂತಿರುಗಿ;
		ಆದರೆ (has_object_pack(repo, &obj->oid))
			ಹಿಂತಿರುಗಿ; /* it is in pack - forget about it */
		printf_ln(_("missing %s %s"),
			  printable_type(repo, &obj->oid, obj->type),
			  describe_object(&obj->oid));
		errors_ಸಿಕ್ಕಿತು |= ERROR_REACHABLE;
		ಹಿಂತಿರುಗಿ;
	}
}

/*
 * Check a single unreachable object
 */
ಸ್ಥಿರ ಶೂನ್ಯ check_unreachable_object(ರಚನೆ repository *repo, ರಚನೆ object *obj)
{
	/*
	 * Missing unreachable object? Ignore it. It's not like
	 * we miss it (since it can't be reached), nor do we want
	 * to complain about it being unreachable (since it does
	 * not exist).
	 */
	ಆದರೆ (!(obj->flags & HAS_OBJ))
		ಹಿಂತಿರುಗಿ;

	/*
	 * Unreachable object that exists? Show it if asked to,
	 * since this is something that is prunable.
	 */
	ಆದರೆ (show_unreachable) {
		printf_ln(_("unreachable %s %s"),
			  printable_type(repo, &obj->oid, obj->type),
			  describe_object(&obj->oid));
		ಹಿಂತಿರುಗಿ;
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
	ಆದರೆ (!(obj->flags & ಬಳಸಿದ)) {
		ಆದರೆ (show_dangling)
			printf_ln(_("dangling %s %s"),
				  printable_type(repo, &obj->oid, obj->type),
				  describe_object(&obj->oid));
		ಆದರೆ (write_ಕಳೆದುಹೋದ_and_ಸಿಕ್ಕಿತು) {
			ಅಕ್ಷರ *filename = repo_git_ಮಾರ್ಗ(repo, "lost-found/%s/%s",
				obj->type == OBJ_COMMIT ? "commit" : "other",
				describe_object(&obj->oid));
			FILE *f;

			ಆದರೆ (ಸುರಕ್ಷಿತ_create_leading_directories_const(repo, filename)) {
				error(_("could not create lost-found"));
				ಬಿಡುಗಡೆ(filename);
				ಹಿಂತಿರುಗಿ;
			}
			f = xfopen(filename, "w");
			ಆದರೆ (obj->type == OBJ_BLOB) {
				ಆದರೆ (odb_ಹರಿವು_blob_ಗಮ್ಯ_fd(repo->objects, fileno(f),
							  &obj->oid, ಖಾಲಿ, 1))
					die_errno(_("could not write '%s'"), filename);
			} ಇಲ್ಲದಿದ್ದರೆ
				fprintf(f, "%s\n", describe_object(&obj->oid));
			ಆದರೆ (fclose(f))
				die_errno(_("could not finish '%s'"),
					  filename);
			ಬಿಡುಗಡೆ(filename);
		}
		ಹಿಂತಿರುಗಿ;
	}

	/*
	 * Otherwise? It's there, it's unreachable, and some other unreachable
	 * object points to it. Ignore it - it's not interesting, and we showed
	 * all the interesting cases above.
	 */
}

ಸ್ಥಿರ ಶೂನ್ಯ check_object(ರಚನೆ repository *repo, ರಚನೆ object *obj)
{
	ಆದರೆ (verbose)
		fprintf_ln(stderr, _("Checking %s"), describe_object(&obj->oid));

	ಆದರೆ (obj->flags & REACHABLE)
		check_reachable_object(repo, obj);
	ಇಲ್ಲದಿದ್ದರೆ
		check_unreachable_object(repo, obj);
}

ಸ್ಥಿರ ಶೂನ್ಯ check_connectivity(ರಚನೆ repository *repo)
{
	ಪೂರ್ಣಾಂಕ i, max;

	/* Traverse the pending reachable objects */
	traverse_reachable(repo);

	/*
	 * With --connectivity-only, we won't have actually opened and marked
	 * unreachable objects with USED. Do that now to make --dangling, etc
	 * accurate.
	 */
	ಆದರೆ (connectivity_only && (show_dangling || write_ಕಳೆದುಹೋದ_and_ಸಿಕ್ಕಿತು)) {
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
		odb_for_each_object(repo->objects, ಖಾಲಿ,
				    mark_unreachable_referents, repo, 0);
	}

	/* Look up all the requirements, warn about missing objects.. */
	max = get_max_object_ಸೂಚ್ಯಂಕ(repo);
	ಆದರೆ (verbose)
		fprintf_ln(stderr, _("Checking connectivity (%d objects)"), max);

	ಗಾಗಿ (i = 0; i < max; i++) {
		ರಚನೆ object *obj = get_ಸೂಚ್ಯಂಕಿತ_object(repo, i);

		ಆದರೆ (obj)
			check_object(repo, obj);
	}
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ fsck_obj_ಬಫರ್(ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid, ಎಣಿಕೆ object_type type,
			   ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಗಾತ್ರ_2, ಶೂನ್ಯ *ಬಫರ್, ಪೂರ್ಣಾಂಕ *eaten, ಶೂನ್ಯ *cb_ದತ್ತಾಂಶ)
{
	ರಚನೆ repository *repo = cb_ದತ್ತಾಂಶ;
	ರಚನೆ object *obj;
	ಪೂರ್ಣಾಂಕ err;

	/*
	 * Note, buffer may be NULL if type is OBJ_BLOB. See
	 * verify_packfile(), data_valid variable for details.
	 */
	obj = parse_object_ಬಫರ್(repo, oid, type, ಗಾತ್ರ_2, ಬಫರ್, eaten);
	ಆದರೆ (!obj) {
		errors_ಸಿಕ್ಕಿತು |= ERROR_OBJECT;
		err = error(_("%s: object corrupt or missing"),
			    oid_ಗಮ್ಯ_hex(oid));
		ಹೋಗು ಹೊರಹೋಗು;
	}

	obj->flags &= ~REACHABLE;
	obj->flags |= HAS_OBJ | ನೋಡಿದ_2;

	ಆದರೆ (verbose)
		fprintf_ln(stderr, _("Checking %s %s"),
			   printable_type(repo, &obj->oid, obj->type),
			   describe_object(&obj->oid));

	ಆದರೆ (fsck_walk(obj, ಖಾಲಿ, &fsck_obj_options))
		objerror(repo, obj, _("broken links"));

	err = fsck_object(obj, ಬಫರ್, ಗಾತ್ರ_2, &fsck_obj_options);
	ಆದರೆ (err)
		ಹೋಗು ಹೊರಹೋಗು;

	ಆದರೆ (obj->type == OBJ_COMMIT) {
		ರಚನೆ commit *commit = (ರಚನೆ commit *) obj;

		ಆದರೆ (!commit->parents && show_ಬೇರು)
			printf_ln(_("root %s"),
				  describe_object(&commit->object.oid));
	}

	ಆದರೆ (obj->type == OBJ_ಟ್ಯಾಗ್) {
		ರಚನೆ ಟ್ಯಾಗ್ *ಟ್ಯಾಗ್ = (ರಚನೆ ಟ್ಯಾಗ್ *) obj;

		ಆದರೆ (show_ಟ್ಯಾಗುಗಳು && ಟ್ಯಾಗ್->tagged) {
			printf_ln(_("tagged %s %s (%s) in %s"),
				  printable_type(repo, &ಟ್ಯಾಗ್->tagged->oid, ಟ್ಯಾಗ್->tagged->type),
				  describe_object(&ಟ್ಯಾಗ್->tagged->oid),
				  ಟ್ಯಾಗ್->ಟ್ಯಾಗ್,
				  describe_object(&ಟ್ಯಾಗ್->object.oid));
		}
	}

ಹೊರಹೋಗು:
	ಆದರೆ (obj && obj->type == OBJ_ವೃಕ್ಷ)
		free_ವೃಕ್ಷ_ಬಫರ್((ರಚನೆ ವೃಕ್ಷ *)obj);
	ಹಿಂತಿರುಗಿ err;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ default_refs;

ಸ್ಥಿರ ಶೂನ್ಯ fsck_handle_reflog_oid(ರಚನೆ repository *repo,
				   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *refname, ರಚನೆ object_id *oid,
				   timestamp_t timestamp)
{
	ರಚನೆ object *obj;

	ಆದರೆ (!is_null_oid(oid)) {
		obj = lookup_object(repo, oid);
		ಆದರೆ (obj && (obj->flags & HAS_OBJ)) {
			ಆದರೆ (timestamp)
				fsck_put_object_name(&fsck_walk_options, oid,
						     "%s@{%"PRItime"}",
						     refname, timestamp);
			obj->flags |= ಬಳಸಿದ;
			mark_object_reachable(obj);
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!is_promisor_object(repo, oid)) {
			error(_("%s: invalid reflog entry %s"),
			      refname, oid_ಗಮ್ಯ_hex(oid));
			errors_ಸಿಕ್ಕಿತು |= ERROR_REACHABLE;
		}
	}
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ fsck_handle_reflog_ent(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *refname,
				  ರಚನೆ object_id *ooid, ರಚನೆ object_id *noid,
				  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *email ಬಳಸದ,
				  timestamp_t timestamp, ಪೂರ್ಣಾಂಕ tz ಬಳಸದ,
				  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ_2 ಬಳಸದ, ಶೂನ್ಯ *cb_ದತ್ತಾಂಶ)
{
	ರಚನೆ repository *repo = cb_ದತ್ತಾಂಶ;

	ಆದರೆ (now && timestamp > now)
		ಹಿಂತಿರುಗಿ 0;

	ಆದರೆ (verbose)
		fprintf_ln(stderr, _("Checking reflog %s->%s"),
			   oid_ಗಮ್ಯ_hex(ooid), oid_ಗಮ್ಯ_hex(noid));

	fsck_handle_reflog_oid(repo, refname, ooid, 0);
	fsck_handle_reflog_oid(repo, refname, noid, timestamp);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ fsck_handle_reflog(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *logname, ಶೂನ್ಯ *cb_ದತ್ತಾಂಶ)
{
	ರಚನೆ strbuf refname = STRBUF_INIT;
	ರಚನೆ worktree *wt = cb_ದತ್ತಾಂಶ;

	strbuf_worktree_ref(wt, &refname, logname);
	refs_for_each_reflog_ent(get_main_ref_store(wt->repo),
				 refname.buf, fsck_handle_reflog_ent,
				 wt->repo);
	strbuf_release(&refname);
	ಹಿಂತಿರುಗಿ 0;
}

ರಚನೆ ref_snapshot {
	ಅಕ್ಷರ *refname;
	ರಚನೆ object_id oid;
	/* TODO: Maybe supplement with latest reflog entry info too? */
};

ರಚನೆ snapshot {
	ಗಾತ್ರ_t nr;
	ಗಾತ್ರ_t alloc;
	ರಚನೆ ref_snapshot *ref;
	/* TODO: Consider also snapshotting the index of each worktree. */
};

ರಚನೆ snapshot_ref_ದತ್ತಾಂಶ {
	ರಚನೆ repository *repo;
	ರಚನೆ snapshot *snap;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ snapshot_ref(ಸ್ಥಿರಾಂಕ ರಚನೆ reference *ref, ಶೂನ್ಯ *cb_ದತ್ತಾಂಶ)
{
	ರಚನೆ snapshot_ref_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ = cb_ದತ್ತಾಂಶ;
	ರಚನೆ snapshot *snap = ದತ್ತಾಂಶ->snap;
	ರಚನೆ object *obj;

	obj = parse_object(ದತ್ತಾಂಶ->repo, ref->oid);
	ಆದರೆ (!obj) {
		ಆದರೆ (is_promisor_object(ದತ್ತಾಂಶ->repo, ref->oid)) {
			/*
			 * Increment default_refs anyway, because this is a
			 * valid ref.
			 */
			 default_refs++;
			 ಹಿಂತಿರುಗಿ 0;
		}
		error(_("%s: invalid sha1 pointer %s"),
		      ref->name, oid_ಗಮ್ಯ_hex(ref->oid));
		errors_ಸಿಕ್ಕಿತು |= ERROR_REACHABLE;
		/* We'll continue with the rest despite the error.. */
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (obj->type != OBJ_COMMIT && is_branch(ref->name)) {
		error(_("%s: not a commit"), ref->name);
		errors_ಸಿಕ್ಕಿತು |= ERROR_REFS;
	}
	default_refs++;

	ALLOC_GROW(snap->ref, snap->nr + 1, snap->alloc);
	snap->ref[snap->nr].refname = xstrdup(ref->name);
	oidcpy(&snap->ref[snap->nr].oid, ref->oid);
	snap->nr++;

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ fsck_handle_ref(ಸ್ಥಿರಾಂಕ ರಚನೆ reference *ref, ಶೂನ್ಯ *cb_ದತ್ತಾಂಶ)
{
	ರಚನೆ repository *repo = cb_ದತ್ತಾಂಶ;
	ರಚನೆ object *obj;

	obj = parse_object(repo, ref->oid);
	obj->flags |= ಬಳಸಿದ;
	fsck_put_object_name(&fsck_walk_options,
			     ref->oid, "%s", ref->name);
	mark_object_reachable(obj);

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ snapshot_refs(ರಚನೆ repository *repo,
			  ರಚನೆ snapshot *snap, ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv)
{
	ರಚನೆ refs_for_each_ref_options opts = {
		.flags = REFS_FOR_EACH_INCLUDE_BROKEN,
	};
	ರಚನೆ snapshot_ref_ದತ್ತಾಂಶ ದತ್ತಾಂಶ = {
		.repo = repo,
		.snap = snap,
	};
	ರಚನೆ worktree **worktrees, **p;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ತಲೆ_points_at;
	ರಚನೆ object_id ತಲೆ_oid;

	ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < argc; i++) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg = argv[i];
		ರಚನೆ object_id oid;
		ಆದರೆ (!repo_get_oid(repo, arg, &oid)) {
			ರಚನೆ reference ref = {
				.name = arg,
				.oid = &oid,
			};

			snapshot_ref(&ref, &ದತ್ತಾಂಶ);
			ಮುಂದುವರಿ;
		}
		error(_("invalid parameter: expected sha1, got '%s'"), arg);
		errors_ಸಿಕ್ಕಿತು |= ERROR_OBJECT;
	}

	ಆದರೆ (argc) {
		include_reflogs = 0;
		ಹಿಂತಿರುಗಿ;
	}

	refs_for_each_ref_ext(get_main_ref_store(repo),
			      snapshot_ref, &ದತ್ತಾಂಶ, &opts);

	worktrees = get_worktrees(repo);
	ಗಾಗಿ (p = worktrees; *p; p++) {
		ರಚನೆ worktree *wt = *p;
		ರಚನೆ strbuf refname = STRBUF_INIT;

		strbuf_worktree_ref(wt, &refname, "HEAD");

		ತಲೆ_points_at = refs_resolve_ref_unsafe(get_main_ref_store(repo),
							 refname.buf, 0, &ತಲೆ_oid, ಖಾಲಿ);

		ಆದರೆ (ತಲೆ_points_at && !is_null_oid(&ತಲೆ_oid)) {
			ರಚನೆ reference ref = {
				.name = refname.buf,
				.oid = &ತಲೆ_oid,
			};

			snapshot_ref(&ref, &ದತ್ತಾಂಶ);
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
	now = ಸಮಯ(ಖಾಲಿ);
}


ಸ್ಥಿರ ಶೂನ್ಯ free_snapshot_refs(ರಚನೆ snapshot *snap)
{
	ಗಾಗಿ (ಗಾತ್ರ_t i = 0; i < snap->nr; i++)
		ಬಿಡುಗಡೆ(snap->ref[i].refname);
	ಬಿಡುಗಡೆ(snap->ref);
}

ಸ್ಥಿರ ಶೂನ್ಯ process_refs(ರಚನೆ repository *repo, ರಚನೆ snapshot *snap)
{
	ರಚನೆ worktree **worktrees, **p;

	ಗಾಗಿ (ಗಾತ್ರ_t i = 0; i < snap->nr; i++) {
		ರಚನೆ reference ref = {
			.name = snap->ref[i].refname,
			.oid = &snap->ref[i].oid,
		};
		fsck_handle_ref(&ref, repo);
	}

	ಆದರೆ (include_reflogs) {
		worktrees = get_worktrees(repo);
		ಗಾಗಿ (p = worktrees; *p; p++) {
			ರಚನೆ worktree *wt = *p;

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
	ಆದರೆ (!default_refs) {
		fprintf_ln(stderr, _("notice: No default references"));
		show_unreachable = 0;
	}
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ fsck_cache_ವೃಕ್ಷ(ರಚನೆ repository *repo, ರಚನೆ cache_ವೃಕ್ಷ *it, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸೂಚ್ಯಂಕ_ಮಾರ್ಗ)
{
	ಪೂರ್ಣಾಂಕ i;
	ಪೂರ್ಣಾಂಕ err = 0;

	ಆದರೆ (verbose)
		fprintf_ln(stderr, _("Checking cache tree of %s"), ಸೂಚ್ಯಂಕ_ಮಾರ್ಗ);

	ಆದರೆ (0 <= it->ನಮೂದು_ಎಣಿಕೆ) {
		ರಚನೆ object *obj = parse_object(repo, &it->oid);
		ಆದರೆ (!obj) {
			error(_("%s: invalid sha1 pointer in cache-tree of %s"),
			      oid_ಗಮ್ಯ_hex(&it->oid), ಸೂಚ್ಯಂಕ_ಮಾರ್ಗ);
			errors_ಸಿಕ್ಕಿತು |= ERROR_REFS;
			ಹಿಂತಿರುಗಿ 1;
		}
		obj->flags |= ಬಳಸಿದ;
		fsck_put_object_name(&fsck_walk_options, &it->oid, ":");
		mark_object_reachable(obj);
		ಆದರೆ (obj->type != OBJ_ವೃಕ್ಷ)
			err |= objerror(repo, obj, _("non-tree in cache-tree"));
	}
	ಗಾಗಿ (i = 0; i < it->subtree_nr; i++)
		err |= fsck_cache_ವೃಕ್ಷ(repo, it->ಕೆಳಕ್ಕೆ[i]->cache_ವೃಕ್ಷ, ಸೂಚ್ಯಂಕ_ಮಾರ್ಗ);
	ಹಿಂತಿರುಗಿ err;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ fsck_resolve_undo(ರಚನೆ ಸೂಚ್ಯಂಕ_state *istate,
			     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸೂಚ್ಯಂಕ_ಮಾರ್ಗ)
{
	ರಚನೆ string_list_ಅಂಶ *ಅಂಶ;
	ರಚನೆ string_list *resolve_undo = istate->resolve_undo;

	ಆದರೆ (!resolve_undo)
		ಹಿಂತಿರುಗಿ 0;

	for_each_string_list_ಅಂಶ(ಅಂಶ, resolve_undo) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ = ಅಂಶ->string;
		ರಚನೆ resolve_undo_info *ru = ಅಂಶ->util;
		ಪೂರ್ಣಾಂಕ i;

		ಆದರೆ (!ru)
			ಮುಂದುವರಿ;
		ಗಾಗಿ (i = 0; i < 3; i++) {
			ರಚನೆ object *obj;

			ಆದರೆ (!ru->ಬಹುಲಕ[i] || !S_ISREG(ru->ಬಹುಲಕ[i]))
				ಮುಂದುವರಿ;

			obj = parse_object(istate->repo, &ru->oid[i]);
			ಆದರೆ (!obj) {
				error(_("%s: invalid sha1 pointer in resolve-undo of %s"),
				      oid_ಗಮ್ಯ_hex(&ru->oid[i]),
				      ಸೂಚ್ಯಂಕ_ಮಾರ್ಗ);
				errors_ಸಿಕ್ಕಿತು |= ERROR_REFS;
				ಮುಂದುವರಿ;
			}
			obj->flags |= ಬಳಸಿದ;
			fsck_put_object_name(&fsck_walk_options, &ru->oid[i],
					     ":(%d):%s", i, ಮಾರ್ಗ);
			mark_object_reachable(obj);
		}
	}
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ fsck_ಸೂಚ್ಯಂಕ(ರಚನೆ ಸೂಚ್ಯಂಕ_state *istate, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸೂಚ್ಯಂಕ_ಮಾರ್ಗ,
		       ಪೂರ್ಣಾಂಕ is_current_worktree)
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ i;

	/* TODO: audit for interaction with sparse-index. */
	ensure_ತುಂಬಿದ_ಸೂಚ್ಯಂಕ(istate);
	ಗಾಗಿ (i = 0; i < istate->cache_nr; i++) {
		ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಬಹುಲಕ;
		ರಚನೆ blob *blob;
		ರಚನೆ object *obj;

		ಬಹುಲಕ = istate->cache[i]->ce_ಬಹುಲಕ;
		ಆದರೆ (S_ISGITLINK(ಬಹುಲಕ))
			ಮುಂದುವರಿ;
		blob = lookup_blob(istate->repo,
				   &istate->cache[i]->oid);
		ಆದರೆ (!blob)
			ಮುಂದುವರಿ;
		obj = &blob->object;
		obj->flags |= ಬಳಸಿದ;
		fsck_put_object_name(&fsck_walk_options, &obj->oid,
				     "%s:%s",
				     is_current_worktree ? "" : ಸೂಚ್ಯಂಕ_ಮಾರ್ಗ,
				     istate->cache[i]->name);
		mark_object_reachable(obj);
	}
	ಆದರೆ (istate->cache_ವೃಕ್ಷ)
		fsck_cache_ವೃಕ್ಷ(istate->repo, istate->cache_ವೃಕ್ಷ, ಸೂಚ್ಯಂಕ_ಮಾರ್ಗ);
	fsck_resolve_undo(istate, ಸೂಚ್ಯಂಕ_ಮಾರ್ಗ);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ mark_object_for_connectivity(ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid,
					ರಚನೆ object_info *oi ಬಳಸದ,
					ಶೂನ್ಯ *cb_ದತ್ತಾಂಶ)
{
	ರಚನೆ repository *repo = cb_ದತ್ತಾಂಶ;
	ರಚನೆ object *obj = lookup_unknown_object(repo, oid);
	obj->flags |= HAS_OBJ;
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ fsck_refs(ರಚನೆ repository *r)
{
	ರಚನೆ ಮಗು_process refs_verify = ಮಗು_PROCESS_INIT;
	ರಚನೆ ಪ್ರಗತಿ *ಪ್ರಗತಿ = ಖಾಲಿ;

	ಆದರೆ (show_ಪ್ರಗತಿ)
		ಪ್ರಗತಿ = ಆರಂಭ_ಪ್ರಗತಿ(r, _("Checking ref database"), 1);

	ಆದರೆ (verbose)
		fprintf_ln(stderr, _("Checking ref database"));

	ಮಗು_process_init(&refs_verify);
	refs_verify.git_cmd = 1;
	strvec_pushl(&refs_verify.ವಾದಗಳು, "refs", "verify", ಖಾಲಿ);
	ಆದರೆ (verbose)
		strvec_ತಳ್ಳು(&refs_verify.ವಾದಗಳು, "--verbose");
	ಆದರೆ (check_strict)
		strvec_ತಳ್ಳು(&refs_verify.ವಾದಗಳು, "--strict");

	ಆದರೆ (ಸತತ_command(&refs_verify))
		errors_ಸಿಕ್ಕಿತು |= ERROR_REFS;

	display_ಪ್ರಗತಿ(ಪ್ರಗತಿ, 1);
	stop_ಪ್ರಗತಿ(&ಪ್ರಗತಿ);
}

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ * ಸ್ಥಿರಾಂಕ fsck_usage[] = {
	N_("git fsck [--tags] [--root] [--unreachable] [--cache] [--no-reflogs]\n"
	   "         [--[no-]full] [--strict] [--verbose] [--lost-found]\n"
	   "         [--[no-]dangling] [--[no-]progress] [--connectivity-only]\n"
	   "         [--[no-]name-objects] [--[no-]references] [<object>...]"),
	ಖಾಲಿ
};

ಪೂರ್ಣಾಂಕ cmd_fsck(ಪೂರ್ಣಾಂಕ argc,
	     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
	     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
	     ರಚನೆ repository *repo)
{
	ರಚನೆ odb_fsck_options odb_fsck_opts = {
		.flags = ODB_FSCK_ತುಂಬಿದ,
		.object_cb = fsck_obj_ಬಫರ್,
		.object_payload = repo,
	};
	ಪೂರ್ಣಾಂಕ keep_cache_objects = 0;
	ಪೂರ್ಣಾಂಕ name_objects = 0;
	ಪೂರ್ಣಾಂಕ check_references = 1;
	ರಚನೆ option fsck_opts[] = {
		OPT__VERBOSE(&verbose, N_("be verbose")),
		OPT_BOOL(0, "unreachable", &show_unreachable, N_("show unreachable objects")),
		OPT_BOOL(0, "dangling", &show_dangling, N_("show dangling objects")),
		OPT_BOOL(0, "tags", &show_ಟ್ಯಾಗುಗಳು, N_("report tags")),
		OPT_BOOL(0, "root", &show_ಬೇರು, N_("report root nodes")),
		OPT_BOOL(0, "cache", &keep_cache_objects, N_("make index objects head nodes")),
		OPT_BOOL(0, "reflogs", &include_reflogs, N_("make reflogs head nodes (default)")),
		OPT_ಬಿಟ್(0, "full", &odb_fsck_opts.flags,
			N_("also consider packs and alternate objects"), ODB_FSCK_ತುಂಬಿದ),
		OPT_BOOL(0, "connectivity-only", &connectivity_only, N_("check only connectivity")),
		OPT_BOOL(0, "strict", &check_strict, N_("enable more strict checking")),
		OPT_BOOL(0, "lost-found", &write_ಕಳೆದುಹೋದ_and_ಸಿಕ್ಕಿತು,
					N_("write dangling objects in .git/lost-found")),
		OPT_BOOL(0, "progress", &show_ಪ್ರಗತಿ, N_("show progress")),
		OPT_BOOL(0, "name-objects", &name_objects, N_("show verbose names for reachable objects")),
		OPT_BOOL(0, "references", &check_references, N_("check reference database consistency")),
		OPT_ಕೊನೆ(),
	};
	ರಚನೆ snapshot snap = {
		.nr = 0,
		.alloc = 0,
		.ref = ಖಾಲಿ
	};

	errors_ಸಿಕ್ಕಿತು = 0;
	disable_replace_refs();
	save_commit_ಬಫರ್ = 0;

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, fsck_opts, fsck_usage, 0);

	/* fsck knows how to handle missing promisor objects */
	repo->fetch_if_missing = 0;

	fsck_options_init(&fsck_walk_options, repo, FSCK_OPTIONS_DEFAULT);
	fsck_walk_options.walk = mark_object;

	fsck_options_init(&fsck_obj_options, repo, FSCK_OPTIONS_DEFAULT);
	fsck_obj_options.walk = mark_ಬಳಸಿದ;
	fsck_obj_options.error_func = fsck_objects_error_func;
	ಆದರೆ (check_strict)
		fsck_obj_options.strict = 1;

	ಆದರೆ (show_ಪ್ರಗತಿ == -1)
		show_ಪ್ರಗತಿ = isatty(2);
	ಆದರೆ (verbose) {
		show_ಪ್ರಗತಿ = 0;
		odb_fsck_opts.flags |= ODB_FSCK_VERBOSE;
	}
	ಆದರೆ (show_ಪ್ರಗತಿ)
		odb_fsck_opts.flags |= ODB_FSCK_ಪ್ರಗತಿ;

	ಆದರೆ (write_ಕಳೆದುಹೋದ_and_ಸಿಕ್ಕಿತು) {
		odb_fsck_opts.flags |= ODB_FSCK_ತುಂಬಿದ;
		include_reflogs = 0;
	}

	ಆದರೆ (name_objects)
		fsck_enable_object_ಹೆಸರುಗಳು(&fsck_walk_options);

	repo_config(repo, git_fsck_config, &fsck_obj_options);
	prepare_repo_settings(repo);

	ಆದರೆ (check_references)
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

	ಆದರೆ (connectivity_only) {
		odb_for_each_object(repo->objects, ಖಾಲಿ,
				    mark_object_for_connectivity, repo, 0);
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಆದರೆ (odb_fsck(repo->objects, &odb_fsck_opts) < 0)
			errors_ಸಿಕ್ಕಿತು |= ERROR_OBJECT;

		ಆದರೆ (fsck_ಮುಕ್ತಾಯ(&fsck_obj_options))
			errors_ಸಿಕ್ಕಿತು |= ERROR_OBJECT;
	}

	/* Process the snapshotted refs and the reflogs. */
	process_refs(repo, &snap);

	/* If not given any explicit objects, process index files too. */
	ಆದರೆ (!argc)
		keep_cache_objects = 1;
	ಆದರೆ (keep_cache_objects) {
		/*
		 * TODO: Consider first walking these indexes in snapshot_refs,
		 * to snapshot where the index entries used to point, and then
		 * check those snapshotted locations here.
		 */
		ರಚನೆ worktree **worktrees, **p;

		verify_ಸೂಚ್ಯಂಕ_checksum = 1;
		verify_ce_ಕ್ರಮ = 1;

		worktrees = get_worktrees(repo);
		ಗಾಗಿ (p = worktrees; *p; p++) {
			ರಚನೆ worktree *wt = *p;
			ರಚನೆ ಸೂಚ್ಯಂಕ_state istate =
				ಸೂಚ್ಯಂಕ_STATE_INIT(repo);
			ಅಕ್ಷರ *ಮಾರ್ಗ, *wt_gitdir;

			/*
			 * Make a copy since the buffer is reusable
			 * and may get overwritten by other calls
			 * while we're examining the index.
			 */
			ಮಾರ್ಗ = xstrdup(worktree_git_ಮಾರ್ಗ(wt, "index"));
			wt_gitdir = get_worktree_git_dir(wt);

			read_ಸೂಚ್ಯಂಕ_from(&istate, ಮಾರ್ಗ, wt_gitdir);
			fsck_ಸೂಚ್ಯಂಕ(&istate, ಮಾರ್ಗ, wt->is_current);

			discard_ಸೂಚ್ಯಂಕ(&istate);
			ಬಿಡುಗಡೆ(wt_gitdir);
			ಬಿಡುಗಡೆ(ಮಾರ್ಗ);
		}
		free_worktrees(worktrees);
	}

	check_connectivity(repo);

	ಆದರೆ (repo->settings.core_commit_ನಕ್ಷೆ) {
		ರಚನೆ odb_ಮೂಲ_files *files = odb_ಮೂಲ_files_downcast(repo->objects->ಮೂಲ);
		ರಚನೆ ಮಗು_process commit_ನಕ್ಷೆ_verify = ಮಗು_PROCESS_INIT;

		ಗಾಗಿ (ರಚನೆ odb_files_dir *dir = files->dirs; dir; dir = dir->ಮುಂದಿನ) {
			ಮಗು_process_init(&commit_ನಕ್ಷೆ_verify);
			commit_ನಕ್ಷೆ_verify.git_cmd = 1;
			strvec_pushl(&commit_ನಕ್ಷೆ_verify.ವಾದಗಳು, "commit-graph",
				     "verify", "--object-dir", dir->abspath, ಖಾಲಿ);
			ಆದರೆ (show_ಪ್ರಗತಿ)
				strvec_ತಳ್ಳು(&commit_ನಕ್ಷೆ_verify.ವಾದಗಳು, "--progress");
			ಇಲ್ಲದಿದ್ದರೆ
				strvec_ತಳ್ಳು(&commit_ನಕ್ಷೆ_verify.ವಾದಗಳು, "--no-progress");
			ಆದರೆ (ಸತತ_command(&commit_ನಕ್ಷೆ_verify))
				errors_ಸಿಕ್ಕಿತು |= ERROR_COMMIT_ನಕ್ಷೆ;
		}
	}

	free_snapshot_refs(&snap);
	ಹಿಂತಿರುಗಿ errors_ಸಿಕ್ಕಿತು;
}
