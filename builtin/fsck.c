#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hex.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "commit.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "tree.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "blob.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "tag.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "refs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cache-tree.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "fsck.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "progress.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "packfile.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object-name.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "odb.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "odb/streaming.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "path.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "read-cache-ll.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "replace-object.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "resolve-undo.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "run-command.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "sparse-index.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "worktree.h"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ REACHABLE 0x0001
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਵੇਖਿਆ_2      0x0002
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HAS_OBJ   0x0004
/* This flag is set if something points to this object. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਵਰਤਿਆ      0x0008

ਸਥਿਰ ਪੂਰਨ show_ਜੜ੍ਹ;
ਸਥਿਰ ਪੂਰਨ show_ਟੈਗ_ਸੂਚੀ;
ਸਥਿਰ ਪੂਰਨ show_unreachable;
ਸਥਿਰ ਪੂਰਨ include_reflogs = 1;
ਸਥਿਰ ਪੂਰਨ connectivity_only;
ਸਥਿਰ ਪੂਰਨ check_strict;
ਸਥਿਰ ਬਣਤਰ fsck_options fsck_walk_options;
ਸਥਿਰ ਬਣਤਰ fsck_options fsck_obj_options;
ਸਥਿਰ ਪੂਰਨ errors_ਮਿਲਿਆ;
ਸਥਿਰ ਪੂਰਨ write_ਗੁਆਚਾ_and_ਮਿਲਿਆ;
ਸਥਿਰ ਪੂਰਨ verbose;
ਸਥਿਰ ਪੂਰਨ show_ਤਰੱਕੀ = -1;
ਸਥਿਰ ਪੂਰਨ show_dangling = 1;
ਸਥਿਰ timestamp_t now;
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ERROR_OBJECT 01
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ERROR_REACHABLE 02
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ERROR_REFS 010
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ERROR_COMMIT_ਗ੍ਰਾਫ 020

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *describe_object(ਅਚਲ ਬਣਤਰ object_id *oid)
{
	ਵਾਪਸ fsck_describe_object(&fsck_walk_options, oid);
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *printable_type(ਬਣਤਰ repository *repo,
				  ਅਚਲ ਬਣਤਰ object_id *oid,
				  ਗਿਣਤੀ object_type type)
{
	ਅਚਲ ਅੱਖਰ *ret;

	ਜੇ (type == OBJ_NONE)
		type = odb_read_object_info(repo->objects, oid, ਨਿਰਾ);

	ret = type_name(type);
	ਜੇ (!ret)
		ret = _("unknown");

	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ objerror(ਬਣਤਰ repository *repo, ਬਣਤਰ object *obj, ਅਚਲ ਅੱਖਰ *err)
{
	errors_ਮਿਲਿਆ |= ERROR_OBJECT;
	/* TRANSLATORS: e.g. error in tree 01bfda: <more explanation> */
	fprintf_ln(stderr, _("error in %s %s: %s"),
		   printable_type(repo, &obj->oid, obj->type),
		   describe_object(&obj->oid), err);
	ਵਾਪਸ -1;
}

ਸਥਿਰ ਪੂਰਨ fsck_objects_error_func(ਬਣਤਰ fsck_options *o,
				   ਖਾਲੀ *fsck_report,
				   ਗਿਣਤੀ fsck_ਸੁਨੇਹਾ_type ਸੁਨੇਹਾ_type,
				   ਗਿਣਤੀ fsck_ਸੁਨੇਹਾ_id ਸੁਨੇਹਾ_id_2 ਅਣਵਰਤਿਆ,
				   ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ_2)
{
	ਬਣਤਰ fsck_object_report *report = fsck_report;
	ਅਚਲ ਬਣਤਰ object_id *oid = report->oid;
	ਗਿਣਤੀ object_type object_type = report->object_type;

	ਚੋਣ (ਸੁਨੇਹਾ_type) {
	ਹਾਲਤ FSCK_WARN:
		/* TRANSLATORS: e.g. warning in tree 01bfda: <more explanation> */
		fprintf_ln(stderr, _("warning in %s %s: %s"),
			   printable_type(o->repo, oid, object_type),
			   describe_object(oid), ਸੁਨੇਹਾ_2);
		ਵਾਪਸ 0;
	ਹਾਲਤ FSCK_ERROR:
		/* TRANSLATORS: e.g. error in tree 01bfda: <more explanation> */
		fprintf_ln(stderr, _("error in %s %s: %s"),
			   printable_type(o->repo, oid, object_type),
			   describe_object(oid), ਸੁਨੇਹਾ_2);
		ਵਾਪਸ 1;
	ਮੂਲ:
		BUG("%d (FSCK_IGNORE?) should never trigger this callback",
		    ਸੁਨੇਹਾ_type);
	}
}

ਸਥਿਰ ਬਣਤਰ object_array pending;

ਸਥਿਰ ਪੂਰਨ mark_object(ਬਣਤਰ object *obj, ਗਿਣਤੀ object_type type,
		       ਖਾਲੀ *ਅੰਕੜੇ, ਬਣਤਰ fsck_options *options)
{
	ਬਣਤਰ object *ਮਾਪਾ = ਅੰਕੜੇ;

	/*
	 * The only case data is NULL or type is OBJ_ANY is when
	 * mark_object_reachable() calls us.  All the callers of
	 * that function has non-NULL obj hence ...
	 */
	ਜੇ (!obj) {
		/* ... these references to parent->fld are safe here */
		printf_ln(_("broken link from %7s %s"),
			  printable_type(options->repo, &ਮਾਪਾ->oid, ਮਾਪਾ->type),
			  describe_object(&ਮਾਪਾ->oid));
		printf_ln(_("broken link from %7s %s"),
			  (type == OBJ_ANY ? _("unknown") : type_name(type)),
			  _("unknown"));
		errors_ਮਿਲਿਆ |= ERROR_REACHABLE;
		ਵਾਪਸ 1;
	}

	ਜੇ (type != OBJ_ANY && obj->type != type)
		/* ... and the reference to parent is safe here */
		objerror(options->repo, ਮਾਪਾ, _("wrong object type in link"));

	ਜੇ (obj->flags & REACHABLE)
		ਵਾਪਸ 0;
	obj->flags |= REACHABLE;

	ਜੇ (is_promisor_object(options->repo, &obj->oid))
		/*
		 * Further recursion does not need to be performed on this
		 * object since it is a promisor object (so it does not need to
		 * be added to "pending").
		 */
		ਵਾਪਸ 0;

	ਜੇ (!(obj->flags & HAS_OBJ)) {
		ਜੇ (ਮਾਪਾ && !odb_has_object(options->repo->objects, &obj->oid,
					      ODB_HAS_OBJECT_RECHECK_PACKED)) {
			printf_ln(_("broken link from %7s %s\n"
				    "              to %7s %s"),
				  printable_type(options->repo, &ਮਾਪਾ->oid, ਮਾਪਾ->type),
				  describe_object(&ਮਾਪਾ->oid),
				  printable_type(options->repo, &obj->oid, obj->type),
				  describe_object(&obj->oid));
			errors_ਮਿਲਿਆ |= ERROR_REACHABLE;
		}
		ਵਾਪਸ 1;
	}

	ਜੋੜੋ_object_array(obj, ਨਿਰਾ, &pending);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ mark_object_reachable(ਬਣਤਰ object *obj)
{
	mark_object(obj, OBJ_ANY, ਨਿਰਾ, &fsck_walk_options);
}

ਸਥਿਰ ਪੂਰਨ traverse_one_object(ਬਣਤਰ object *obj)
{
	ਪੂਰਨ ਨਤੀਜਾ = fsck_walk(obj, obj, &fsck_walk_options);

	ਜੇ (obj->type == OBJ_ਰੁੱਖ) {
		ਬਣਤਰ ਰੁੱਖ *ਰੁੱਖ = (ਬਣਤਰ ਰੁੱਖ *)obj;
		free_ਰੁੱਖ_ਬਫਰ(ਰੁੱਖ);
	}
	ਵਾਪਸ ਨਤੀਜਾ;
}

ਸਥਿਰ ਪੂਰਨ traverse_reachable(ਬਣਤਰ repository *repo)
{
	ਬਣਤਰ ਤਰੱਕੀ *ਤਰੱਕੀ = ਨਿਰਾ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ nr = 0;
	ਪੂਰਨ ਨਤੀਜਾ = 0;
	ਜੇ (show_ਤਰੱਕੀ)
		ਤਰੱਕੀ = ਸ਼ੁਰੂ_delayed_ਤਰੱਕੀ(repo,
						  _("Checking connectivity"), 0);
	ਜਦੋਂ (pending.nr) {
		ਨਤੀਜਾ |= traverse_one_object(object_array_ਕੱਢੋ(&pending));
		display_ਤਰੱਕੀ(ਤਰੱਕੀ, ++nr);
	}
	stop_ਤਰੱਕੀ(&ਤਰੱਕੀ);
	ਵਾਪਸ !!ਨਤੀਜਾ;
}

ਸਥਿਰ ਪੂਰਨ mark_ਵਰਤਿਆ(ਬਣਤਰ object *obj, ਗਿਣਤੀ object_type type ਅਣਵਰਤਿਆ,
		     ਖਾਲੀ *ਅੰਕੜੇ ਅਣਵਰਤਿਆ, ਬਣਤਰ fsck_options *options ਅਣਵਰਤਿਆ)
{
	ਜੇ (!obj)
		ਵਾਪਸ 1;
	obj->flags |= ਵਰਤਿਆ;
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ mark_unreachable_referents(ਅਚਲ ਬਣਤਰ object_id *oid,
				      ਬਣਤਰ object_info *oi ਅਣਵਰਤਿਆ,
				      ਖਾਲੀ *ਅੰਕੜੇ)
{
	ਬਣਤਰ repository *repo = ਅੰਕੜੇ;
	ਬਣਤਰ fsck_options options;
	ਬਣਤਰ object *obj = lookup_object(ਅੰਕੜੇ, oid);

	ਜੇ (!obj || !(obj->flags & HAS_OBJ))
		ਵਾਪਸ 0; /* not part of our original set */
	ਜੇ (obj->flags & REACHABLE)
		ਵਾਪਸ 0; /* reachable objects already traversed */

	/*
	 * Avoid passing OBJ_NONE to fsck_walk, which will parse the object
	 * (and we want to avoid parsing blobs).
	 */
	ਜੇ (obj->type == OBJ_NONE) {
		ਗਿਣਤੀ object_type type = odb_read_object_info(repo->objects,
							     &obj->oid, ਨਿਰਾ);
		ਜੇ (type > 0)
			object_as_type(obj, type, 0);
	}

	fsck_options_init(&options, repo, FSCK_OPTIONS_DEFAULT);
	options.walk = mark_ਵਰਤਿਆ;
	fsck_walk(obj, ਨਿਰਾ, &options);
	ਜੇ (obj->type == OBJ_ਰੁੱਖ)
		free_ਰੁੱਖ_ਬਫਰ((ਬਣਤਰ ਰੁੱਖ *)obj);

	ਵਾਪਸ 0;
}

/*
 * Check a single reachable object
 */
ਸਥਿਰ ਖਾਲੀ check_reachable_object(ਬਣਤਰ repository *repo, ਬਣਤਰ object *obj)
{
	/*
	 * We obviously want the object to be parsed,
	 * except if it was in a pack-file and we didn't
	 * do a full fsck
	 */
	ਜੇ (!(obj->flags & HAS_OBJ)) {
		ਜੇ (is_promisor_object(repo, &obj->oid))
			ਵਾਪਸ;
		ਜੇ (has_object_pack(repo, &obj->oid))
			ਵਾਪਸ; /* it is in pack - forget about it */
		printf_ln(_("missing %s %s"),
			  printable_type(repo, &obj->oid, obj->type),
			  describe_object(&obj->oid));
		errors_ਮਿਲਿਆ |= ERROR_REACHABLE;
		ਵਾਪਸ;
	}
}

/*
 * Check a single unreachable object
 */
ਸਥਿਰ ਖਾਲੀ check_unreachable_object(ਬਣਤਰ repository *repo, ਬਣਤਰ object *obj)
{
	/*
	 * Missing unreachable object? Ignore it. It's not like
	 * we miss it (since it can't be reached), nor do we want
	 * to complain about it being unreachable (since it does
	 * not exist).
	 */
	ਜੇ (!(obj->flags & HAS_OBJ))
		ਵਾਪਸ;

	/*
	 * Unreachable object that exists? Show it if asked to,
	 * since this is something that is prunable.
	 */
	ਜੇ (show_unreachable) {
		printf_ln(_("unreachable %s %s"),
			  printable_type(repo, &obj->oid, obj->type),
			  describe_object(&obj->oid));
		ਵਾਪਸ;
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
	ਜੇ (!(obj->flags & ਵਰਤਿਆ)) {
		ਜੇ (show_dangling)
			printf_ln(_("dangling %s %s"),
				  printable_type(repo, &obj->oid, obj->type),
				  describe_object(&obj->oid));
		ਜੇ (write_ਗੁਆਚਾ_and_ਮਿਲਿਆ) {
			ਅੱਖਰ *filename = repo_git_ਰਸਤਾ(repo, "lost-found/%s/%s",
				obj->type == OBJ_COMMIT ? "commit" : "other",
				describe_object(&obj->oid));
			FILE *f;

			ਜੇ (ਸੁਰੱਖਿਅਤ_create_leading_directories_const(repo, filename)) {
				error(_("could not create lost-found"));
				ਮੁਕਤ(filename);
				ਵਾਪਸ;
			}
			f = xfopen(filename, "w");
			ਜੇ (obj->type == OBJ_BLOB) {
				ਜੇ (odb_ਧਾਰਾ_blob_ਮੰਜ਼ਿਲ_fd(repo->objects, fileno(f),
							  &obj->oid, ਨਿਰਾ, 1))
					die_errno(_("could not write '%s'"), filename);
			} ਨਹੀਂਤਾਂ
				fprintf(f, "%s\n", describe_object(&obj->oid));
			ਜੇ (fclose(f))
				die_errno(_("could not finish '%s'"),
					  filename);
			ਮੁਕਤ(filename);
		}
		ਵਾਪਸ;
	}

	/*
	 * Otherwise? It's there, it's unreachable, and some other unreachable
	 * object points to it. Ignore it - it's not interesting, and we showed
	 * all the interesting cases above.
	 */
}

ਸਥਿਰ ਖਾਲੀ check_object(ਬਣਤਰ repository *repo, ਬਣਤਰ object *obj)
{
	ਜੇ (verbose)
		fprintf_ln(stderr, _("Checking %s"), describe_object(&obj->oid));

	ਜੇ (obj->flags & REACHABLE)
		check_reachable_object(repo, obj);
	ਨਹੀਂਤਾਂ
		check_unreachable_object(repo, obj);
}

ਸਥਿਰ ਖਾਲੀ check_connectivity(ਬਣਤਰ repository *repo)
{
	ਪੂਰਨ i, max;

	/* Traverse the pending reachable objects */
	traverse_reachable(repo);

	/*
	 * With --connectivity-only, we won't have actually opened and marked
	 * unreachable objects with USED. Do that now to make --dangling, etc
	 * accurate.
	 */
	ਜੇ (connectivity_only && (show_dangling || write_ਗੁਆਚਾ_and_ਮਿਲਿਆ)) {
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
		odb_for_each_object(repo->objects, ਨਿਰਾ,
				    mark_unreachable_referents, repo, 0);
	}

	/* Look up all the requirements, warn about missing objects.. */
	max = get_max_object_ਸੂਚਕਾਂਕ(repo);
	ਜੇ (verbose)
		fprintf_ln(stderr, _("Checking connectivity (%d objects)"), max);

	ਲਈ (i = 0; i < max; i++) {
		ਬਣਤਰ object *obj = get_ਸੂਚਕਾਂਕਿਤ_object(repo, i);

		ਜੇ (obj)
			check_object(repo, obj);
	}
}

ਸਥਿਰ ਪੂਰਨ fsck_obj_ਬਫਰ(ਅਚਲ ਬਣਤਰ object_id *oid, ਗਿਣਤੀ object_type type,
			   ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਮਾਪ, ਖਾਲੀ *ਬਫਰ, ਪੂਰਨ *eaten, ਖਾਲੀ *cb_ਅੰਕੜੇ)
{
	ਬਣਤਰ repository *repo = cb_ਅੰਕੜੇ;
	ਬਣਤਰ object *obj;
	ਪੂਰਨ err;

	/*
	 * Note, buffer may be NULL if type is OBJ_BLOB. See
	 * verify_packfile(), data_valid variable for details.
	 */
	obj = parse_object_ਬਫਰ(repo, oid, type, ਮਾਪ, ਬਫਰ, eaten);
	ਜੇ (!obj) {
		errors_ਮਿਲਿਆ |= ERROR_OBJECT;
		err = error(_("%s: object corrupt or missing"),
			    oid_ਮੰਜ਼ਿਲ_hex(oid));
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	obj->flags &= ~REACHABLE;
	obj->flags |= HAS_OBJ | ਵੇਖਿਆ_2;

	ਜੇ (verbose)
		fprintf_ln(stderr, _("Checking %s %s"),
			   printable_type(repo, &obj->oid, obj->type),
			   describe_object(&obj->oid));

	ਜੇ (fsck_walk(obj, ਨਿਰਾ, &fsck_obj_options))
		objerror(repo, obj, _("broken links"));

	err = fsck_object(obj, ਬਫਰ, ਮਾਪ, &fsck_obj_options);
	ਜੇ (err)
		ਜਾਓ ਬਾਹਰਲਾ;

	ਜੇ (obj->type == OBJ_COMMIT) {
		ਬਣਤਰ commit *commit = (ਬਣਤਰ commit *) obj;

		ਜੇ (!commit->parents && show_ਜੜ੍ਹ)
			printf_ln(_("root %s"),
				  describe_object(&commit->object.oid));
	}

	ਜੇ (obj->type == OBJ_ਟੈਗ) {
		ਬਣਤਰ ਟੈਗ *ਟੈਗ = (ਬਣਤਰ ਟੈਗ *) obj;

		ਜੇ (show_ਟੈਗ_ਸੂਚੀ && ਟੈਗ->tagged) {
			printf_ln(_("tagged %s %s (%s) in %s"),
				  printable_type(repo, &ਟੈਗ->tagged->oid, ਟੈਗ->tagged->type),
				  describe_object(&ਟੈਗ->tagged->oid),
				  ਟੈਗ->ਟੈਗ,
				  describe_object(&ਟੈਗ->object.oid));
		}
	}

ਬਾਹਰਲਾ:
	ਜੇ (obj && obj->type == OBJ_ਰੁੱਖ)
		free_ਰੁੱਖ_ਬਫਰ((ਬਣਤਰ ਰੁੱਖ *)obj);
	ਵਾਪਸ err;
}

ਸਥਿਰ ਪੂਰਨ default_refs;

ਸਥਿਰ ਖਾਲੀ fsck_handle_reflog_oid(ਬਣਤਰ repository *repo,
				   ਅਚਲ ਅੱਖਰ *refname, ਬਣਤਰ object_id *oid,
				   timestamp_t timestamp)
{
	ਬਣਤਰ object *obj;

	ਜੇ (!is_null_oid(oid)) {
		obj = lookup_object(repo, oid);
		ਜੇ (obj && (obj->flags & HAS_OBJ)) {
			ਜੇ (timestamp)
				fsck_put_object_name(&fsck_walk_options, oid,
						     "%s@{%"PRItime"}",
						     refname, timestamp);
			obj->flags |= ਵਰਤਿਆ;
			mark_object_reachable(obj);
		} ਨਹੀਂਤਾਂ ਜੇ (!is_promisor_object(repo, oid)) {
			error(_("%s: invalid reflog entry %s"),
			      refname, oid_ਮੰਜ਼ਿਲ_hex(oid));
			errors_ਮਿਲਿਆ |= ERROR_REACHABLE;
		}
	}
}

ਸਥਿਰ ਪੂਰਨ fsck_handle_reflog_ent(ਅਚਲ ਅੱਖਰ *refname,
				  ਬਣਤਰ object_id *ooid, ਬਣਤਰ object_id *noid,
				  ਅਚਲ ਅੱਖਰ *email ਅਣਵਰਤਿਆ,
				  timestamp_t timestamp, ਪੂਰਨ tz ਅਣਵਰਤਿਆ,
				  ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ_2 ਅਣਵਰਤਿਆ, ਖਾਲੀ *cb_ਅੰਕੜੇ)
{
	ਬਣਤਰ repository *repo = cb_ਅੰਕੜੇ;

	ਜੇ (now && timestamp > now)
		ਵਾਪਸ 0;

	ਜੇ (verbose)
		fprintf_ln(stderr, _("Checking reflog %s->%s"),
			   oid_ਮੰਜ਼ਿਲ_hex(ooid), oid_ਮੰਜ਼ਿਲ_hex(noid));

	fsck_handle_reflog_oid(repo, refname, ooid, 0);
	fsck_handle_reflog_oid(repo, refname, noid, timestamp);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ fsck_handle_reflog(ਅਚਲ ਅੱਖਰ *logname, ਖਾਲੀ *cb_ਅੰਕੜੇ)
{
	ਬਣਤਰ strbuf refname = STRBUF_INIT;
	ਬਣਤਰ worktree *wt = cb_ਅੰਕੜੇ;

	strbuf_worktree_ref(wt, &refname, logname);
	refs_for_each_reflog_ent(get_main_ref_store(wt->repo),
				 refname.buf, fsck_handle_reflog_ent,
				 wt->repo);
	strbuf_release(&refname);
	ਵਾਪਸ 0;
}

ਬਣਤਰ ref_snapshot {
	ਅੱਖਰ *refname;
	ਬਣਤਰ object_id oid;
	/* TODO: Maybe supplement with latest reflog entry info too? */
};

ਬਣਤਰ snapshot {
	ਮਾਪ_t nr;
	ਮਾਪ_t alloc;
	ਬਣਤਰ ref_snapshot *ref;
	/* TODO: Consider also snapshotting the index of each worktree. */
};

ਬਣਤਰ snapshot_ref_ਅੰਕੜੇ {
	ਬਣਤਰ repository *repo;
	ਬਣਤਰ snapshot *snap;
};

ਸਥਿਰ ਪੂਰਨ snapshot_ref(ਅਚਲ ਬਣਤਰ reference *ref, ਖਾਲੀ *cb_ਅੰਕੜੇ)
{
	ਬਣਤਰ snapshot_ref_ਅੰਕੜੇ *ਅੰਕੜੇ = cb_ਅੰਕੜੇ;
	ਬਣਤਰ snapshot *snap = ਅੰਕੜੇ->snap;
	ਬਣਤਰ object *obj;

	obj = parse_object(ਅੰਕੜੇ->repo, ref->oid);
	ਜੇ (!obj) {
		ਜੇ (is_promisor_object(ਅੰਕੜੇ->repo, ref->oid)) {
			/*
			 * Increment default_refs anyway, because this is a
			 * valid ref.
			 */
			 default_refs++;
			 ਵਾਪਸ 0;
		}
		error(_("%s: invalid sha1 pointer %s"),
		      ref->name, oid_ਮੰਜ਼ਿਲ_hex(ref->oid));
		errors_ਮਿਲਿਆ |= ERROR_REACHABLE;
		/* We'll continue with the rest despite the error.. */
		ਵਾਪਸ 0;
	}
	ਜੇ (obj->type != OBJ_COMMIT && is_branch(ref->name)) {
		error(_("%s: not a commit"), ref->name);
		errors_ਮਿਲਿਆ |= ERROR_REFS;
	}
	default_refs++;

	ALLOC_GROW(snap->ref, snap->nr + 1, snap->alloc);
	snap->ref[snap->nr].refname = xstrdup(ref->name);
	oidcpy(&snap->ref[snap->nr].oid, ref->oid);
	snap->nr++;

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ fsck_handle_ref(ਅਚਲ ਬਣਤਰ reference *ref, ਖਾਲੀ *cb_ਅੰਕੜੇ)
{
	ਬਣਤਰ repository *repo = cb_ਅੰਕੜੇ;
	ਬਣਤਰ object *obj;

	obj = parse_object(repo, ref->oid);
	obj->flags |= ਵਰਤਿਆ;
	fsck_put_object_name(&fsck_walk_options,
			     ref->oid, "%s", ref->name);
	mark_object_reachable(obj);

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ snapshot_refs(ਬਣਤਰ repository *repo,
			  ਬਣਤਰ snapshot *snap, ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv)
{
	ਬਣਤਰ refs_for_each_ref_options opts = {
		.flags = REFS_FOR_EACH_INCLUDE_BROKEN,
	};
	ਬਣਤਰ snapshot_ref_ਅੰਕੜੇ ਅੰਕੜੇ = {
		.repo = repo,
		.snap = snap,
	};
	ਬਣਤਰ worktree **worktrees, **p;
	ਅਚਲ ਅੱਖਰ *ਸਿਰ_points_at;
	ਬਣਤਰ object_id ਸਿਰ_oid;

	ਲਈ (ਪੂਰਨ i = 0; i < argc; i++) {
		ਅਚਲ ਅੱਖਰ *arg = argv[i];
		ਬਣਤਰ object_id oid;
		ਜੇ (!repo_get_oid(repo, arg, &oid)) {
			ਬਣਤਰ reference ref = {
				.name = arg,
				.oid = &oid,
			};

			snapshot_ref(&ref, &ਅੰਕੜੇ);
			ਜਾਰੀ;
		}
		error(_("invalid parameter: expected sha1, got '%s'"), arg);
		errors_ਮਿਲਿਆ |= ERROR_OBJECT;
	}

	ਜੇ (argc) {
		include_reflogs = 0;
		ਵਾਪਸ;
	}

	refs_for_each_ref_ext(get_main_ref_store(repo),
			      snapshot_ref, &ਅੰਕੜੇ, &opts);

	worktrees = get_worktrees(repo);
	ਲਈ (p = worktrees; *p; p++) {
		ਬਣਤਰ worktree *wt = *p;
		ਬਣਤਰ strbuf refname = STRBUF_INIT;

		strbuf_worktree_ref(wt, &refname, "HEAD");

		ਸਿਰ_points_at = refs_resolve_ref_unsafe(get_main_ref_store(repo),
							 refname.buf, 0, &ਸਿਰ_oid, ਨਿਰਾ);

		ਜੇ (ਸਿਰ_points_at && !is_null_oid(&ਸਿਰ_oid)) {
			ਬਣਤਰ reference ref = {
				.name = refname.buf,
				.oid = &ਸਿਰ_oid,
			};

			snapshot_ref(&ref, &ਅੰਕੜੇ);
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
	now = ਸਮਾਂ(ਨਿਰਾ);
}


ਸਥਿਰ ਖਾਲੀ free_snapshot_refs(ਬਣਤਰ snapshot *snap)
{
	ਲਈ (ਮਾਪ_t i = 0; i < snap->nr; i++)
		ਮੁਕਤ(snap->ref[i].refname);
	ਮੁਕਤ(snap->ref);
}

ਸਥਿਰ ਖਾਲੀ process_refs(ਬਣਤਰ repository *repo, ਬਣਤਰ snapshot *snap)
{
	ਬਣਤਰ worktree **worktrees, **p;

	ਲਈ (ਮਾਪ_t i = 0; i < snap->nr; i++) {
		ਬਣਤਰ reference ref = {
			.name = snap->ref[i].refname,
			.oid = &snap->ref[i].oid,
		};
		fsck_handle_ref(&ref, repo);
	}

	ਜੇ (include_reflogs) {
		worktrees = get_worktrees(repo);
		ਲਈ (p = worktrees; *p; p++) {
			ਬਣਤਰ worktree *wt = *p;

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
	ਜੇ (!default_refs) {
		fprintf_ln(stderr, _("notice: No default references"));
		show_unreachable = 0;
	}
}

ਸਥਿਰ ਪੂਰਨ fsck_cache_ਰੁੱਖ(ਬਣਤਰ repository *repo, ਬਣਤਰ cache_ਰੁੱਖ *it, ਅਚਲ ਅੱਖਰ *ਸੂਚਕਾਂਕ_ਰਸਤਾ)
{
	ਪੂਰਨ i;
	ਪੂਰਨ err = 0;

	ਜੇ (verbose)
		fprintf_ln(stderr, _("Checking cache tree of %s"), ਸੂਚਕਾਂਕ_ਰਸਤਾ);

	ਜੇ (0 <= it->ਇੰਦਰਾਜ_ਗਿਣਤੀ) {
		ਬਣਤਰ object *obj = parse_object(repo, &it->oid);
		ਜੇ (!obj) {
			error(_("%s: invalid sha1 pointer in cache-tree of %s"),
			      oid_ਮੰਜ਼ਿਲ_hex(&it->oid), ਸੂਚਕਾਂਕ_ਰਸਤਾ);
			errors_ਮਿਲਿਆ |= ERROR_REFS;
			ਵਾਪਸ 1;
		}
		obj->flags |= ਵਰਤਿਆ;
		fsck_put_object_name(&fsck_walk_options, &it->oid, ":");
		mark_object_reachable(obj);
		ਜੇ (obj->type != OBJ_ਰੁੱਖ)
			err |= objerror(repo, obj, _("non-tree in cache-tree"));
	}
	ਲਈ (i = 0; i < it->subtree_nr; i++)
		err |= fsck_cache_ਰੁੱਖ(repo, it->ਥੱਲੇ[i]->cache_ਰੁੱਖ, ਸੂਚਕਾਂਕ_ਰਸਤਾ);
	ਵਾਪਸ err;
}

ਸਥਿਰ ਪੂਰਨ fsck_resolve_undo(ਬਣਤਰ ਸੂਚਕਾਂਕ_state *istate,
			     ਅਚਲ ਅੱਖਰ *ਸੂਚਕਾਂਕ_ਰਸਤਾ)
{
	ਬਣਤਰ string_list_ਵਸਤੂ *ਵਸਤੂ;
	ਬਣਤਰ string_list *resolve_undo = istate->resolve_undo;

	ਜੇ (!resolve_undo)
		ਵਾਪਸ 0;

	for_each_string_list_ਵਸਤੂ(ਵਸਤੂ, resolve_undo) {
		ਅਚਲ ਅੱਖਰ *ਰਸਤਾ = ਵਸਤੂ->string;
		ਬਣਤਰ resolve_undo_info *ru = ਵਸਤੂ->util;
		ਪੂਰਨ i;

		ਜੇ (!ru)
			ਜਾਰੀ;
		ਲਈ (i = 0; i < 3; i++) {
			ਬਣਤਰ object *obj;

			ਜੇ (!ru->ਬਹੁਲਕ[i] || !S_ISREG(ru->ਬਹੁਲਕ[i]))
				ਜਾਰੀ;

			obj = parse_object(istate->repo, &ru->oid[i]);
			ਜੇ (!obj) {
				error(_("%s: invalid sha1 pointer in resolve-undo of %s"),
				      oid_ਮੰਜ਼ਿਲ_hex(&ru->oid[i]),
				      ਸੂਚਕਾਂਕ_ਰਸਤਾ);
				errors_ਮਿਲਿਆ |= ERROR_REFS;
				ਜਾਰੀ;
			}
			obj->flags |= ਵਰਤਿਆ;
			fsck_put_object_name(&fsck_walk_options, &ru->oid[i],
					     ":(%d):%s", i, ਰਸਤਾ);
			mark_object_reachable(obj);
		}
	}
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ fsck_ਸੂਚਕਾਂਕ(ਬਣਤਰ ਸੂਚਕਾਂਕ_state *istate, ਅਚਲ ਅੱਖਰ *ਸੂਚਕਾਂਕ_ਰਸਤਾ,
		       ਪੂਰਨ is_current_worktree)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i;

	/* TODO: audit for interaction with sparse-index. */
	ensure_ਭਰਿਆ_ਸੂਚਕਾਂਕ(istate);
	ਲਈ (i = 0; i < istate->cache_nr; i++) {
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਬਹੁਲਕ;
		ਬਣਤਰ blob *blob;
		ਬਣਤਰ object *obj;

		ਬਹੁਲਕ = istate->cache[i]->ce_ਬਹੁਲਕ;
		ਜੇ (S_ISGITLINK(ਬਹੁਲਕ))
			ਜਾਰੀ;
		blob = lookup_blob(istate->repo,
				   &istate->cache[i]->oid);
		ਜੇ (!blob)
			ਜਾਰੀ;
		obj = &blob->object;
		obj->flags |= ਵਰਤਿਆ;
		fsck_put_object_name(&fsck_walk_options, &obj->oid,
				     "%s:%s",
				     is_current_worktree ? "" : ਸੂਚਕਾਂਕ_ਰਸਤਾ,
				     istate->cache[i]->name);
		mark_object_reachable(obj);
	}
	ਜੇ (istate->cache_ਰੁੱਖ)
		fsck_cache_ਰੁੱਖ(istate->repo, istate->cache_ਰੁੱਖ, ਸੂਚਕਾਂਕ_ਰਸਤਾ);
	fsck_resolve_undo(istate, ਸੂਚਕਾਂਕ_ਰਸਤਾ);
}

ਸਥਿਰ ਪੂਰਨ mark_object_for_connectivity(ਅਚਲ ਬਣਤਰ object_id *oid,
					ਬਣਤਰ object_info *oi ਅਣਵਰਤਿਆ,
					ਖਾਲੀ *cb_ਅੰਕੜੇ)
{
	ਬਣਤਰ repository *repo = cb_ਅੰਕੜੇ;
	ਬਣਤਰ object *obj = lookup_unknown_object(repo, oid);
	obj->flags |= HAS_OBJ;
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ fsck_refs(ਬਣਤਰ repository *r)
{
	ਬਣਤਰ ਬੱਚਾ_process refs_verify = ਬੱਚਾ_PROCESS_INIT;
	ਬਣਤਰ ਤਰੱਕੀ *ਤਰੱਕੀ = ਨਿਰਾ;

	ਜੇ (show_ਤਰੱਕੀ)
		ਤਰੱਕੀ = ਸ਼ੁਰੂ_ਤਰੱਕੀ(r, _("Checking ref database"), 1);

	ਜੇ (verbose)
		fprintf_ln(stderr, _("Checking ref database"));

	ਬੱਚਾ_process_init(&refs_verify);
	refs_verify.git_cmd = 1;
	strvec_pushl(&refs_verify.ਦਲੀਲਾਂ, "refs", "verify", ਨਿਰਾ);
	ਜੇ (verbose)
		strvec_ਧੱਕੋ(&refs_verify.ਦਲੀਲਾਂ, "--verbose");
	ਜੇ (check_strict)
		strvec_ਧੱਕੋ(&refs_verify.ਦਲੀਲਾਂ, "--strict");

	ਜੇ (ਲੜੀਵਾਰ_command(&refs_verify))
		errors_ਮਿਲਿਆ |= ERROR_REFS;

	display_ਤਰੱਕੀ(ਤਰੱਕੀ, 1);
	stop_ਤਰੱਕੀ(&ਤਰੱਕੀ);
}

ਸਥਿਰ ਅੱਖਰ ਅਚਲ * ਅਚਲ fsck_usage[] = {
	N_("git fsck [--tags] [--root] [--unreachable] [--cache] [--no-reflogs]\n"
	   "         [--[no-]full] [--strict] [--verbose] [--lost-found]\n"
	   "         [--[no-]dangling] [--[no-]progress] [--connectivity-only]\n"
	   "         [--[no-]name-objects] [--[no-]references] [<object>...]"),
	ਨਿਰਾ
};

ਪੂਰਨ cmd_fsck(ਪੂਰਨ argc,
	     ਅਚਲ ਅੱਖਰ **argv,
	     ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
	     ਬਣਤਰ repository *repo)
{
	ਬਣਤਰ odb_fsck_options odb_fsck_opts = {
		.flags = ODB_FSCK_ਭਰਿਆ,
		.object_cb = fsck_obj_ਬਫਰ,
		.object_payload = repo,
	};
	ਪੂਰਨ keep_cache_objects = 0;
	ਪੂਰਨ name_objects = 0;
	ਪੂਰਨ check_references = 1;
	ਬਣਤਰ option fsck_opts[] = {
		OPT__VERBOSE(&verbose, N_("be verbose")),
		OPT_BOOL(0, "unreachable", &show_unreachable, N_("show unreachable objects")),
		OPT_BOOL(0, "dangling", &show_dangling, N_("show dangling objects")),
		OPT_BOOL(0, "tags", &show_ਟੈਗ_ਸੂਚੀ, N_("report tags")),
		OPT_BOOL(0, "root", &show_ਜੜ੍ਹ, N_("report root nodes")),
		OPT_BOOL(0, "cache", &keep_cache_objects, N_("make index objects head nodes")),
		OPT_BOOL(0, "reflogs", &include_reflogs, N_("make reflogs head nodes (default)")),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ(0, "full", &odb_fsck_opts.flags,
			N_("also consider packs and alternate objects"), ODB_FSCK_ਭਰਿਆ),
		OPT_BOOL(0, "connectivity-only", &connectivity_only, N_("check only connectivity")),
		OPT_BOOL(0, "strict", &check_strict, N_("enable more strict checking")),
		OPT_BOOL(0, "lost-found", &write_ਗੁਆਚਾ_and_ਮਿਲਿਆ,
					N_("write dangling objects in .git/lost-found")),
		OPT_BOOL(0, "progress", &show_ਤਰੱਕੀ, N_("show progress")),
		OPT_BOOL(0, "name-objects", &name_objects, N_("show verbose names for reachable objects")),
		OPT_BOOL(0, "references", &check_references, N_("check reference database consistency")),
		OPT_ਸਿਰਾ(),
	};
	ਬਣਤਰ snapshot snap = {
		.nr = 0,
		.alloc = 0,
		.ref = ਨਿਰਾ
	};

	errors_ਮਿਲਿਆ = 0;
	disable_replace_refs();
	save_commit_ਬਫਰ = 0;

	argc = parse_options(argc, argv, ਅਗੇਤਰ, fsck_opts, fsck_usage, 0);

	/* fsck knows how to handle missing promisor objects */
	repo->fetch_if_missing = 0;

	fsck_options_init(&fsck_walk_options, repo, FSCK_OPTIONS_DEFAULT);
	fsck_walk_options.walk = mark_object;

	fsck_options_init(&fsck_obj_options, repo, FSCK_OPTIONS_DEFAULT);
	fsck_obj_options.walk = mark_ਵਰਤਿਆ;
	fsck_obj_options.error_func = fsck_objects_error_func;
	ਜੇ (check_strict)
		fsck_obj_options.strict = 1;

	ਜੇ (show_ਤਰੱਕੀ == -1)
		show_ਤਰੱਕੀ = isatty(2);
	ਜੇ (verbose) {
		show_ਤਰੱਕੀ = 0;
		odb_fsck_opts.flags |= ODB_FSCK_VERBOSE;
	}
	ਜੇ (show_ਤਰੱਕੀ)
		odb_fsck_opts.flags |= ODB_FSCK_ਤਰੱਕੀ;

	ਜੇ (write_ਗੁਆਚਾ_and_ਮਿਲਿਆ) {
		odb_fsck_opts.flags |= ODB_FSCK_ਭਰਿਆ;
		include_reflogs = 0;
	}

	ਜੇ (name_objects)
		fsck_enable_object_ਨਾਂ(&fsck_walk_options);

	repo_config(repo, git_fsck_config, &fsck_obj_options);
	prepare_repo_settings(repo);

	ਜੇ (check_references)
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

	ਜੇ (connectivity_only) {
		odb_for_each_object(repo->objects, ਨਿਰਾ,
				    mark_object_for_connectivity, repo, 0);
	} ਨਹੀਂਤਾਂ {
		ਜੇ (odb_fsck(repo->objects, &odb_fsck_opts) < 0)
			errors_ਮਿਲਿਆ |= ERROR_OBJECT;

		ਜੇ (fsck_ਅੰਤ(&fsck_obj_options))
			errors_ਮਿਲਿਆ |= ERROR_OBJECT;
	}

	/* Process the snapshotted refs and the reflogs. */
	process_refs(repo, &snap);

	/* If not given any explicit objects, process index files too. */
	ਜੇ (!argc)
		keep_cache_objects = 1;
	ਜੇ (keep_cache_objects) {
		/*
		 * TODO: Consider first walking these indexes in snapshot_refs,
		 * to snapshot where the index entries used to point, and then
		 * check those snapshotted locations here.
		 */
		ਬਣਤਰ worktree **worktrees, **p;

		verify_ਸੂਚਕਾਂਕ_checksum = 1;
		verify_ce_ਕ੍ਰਮ = 1;

		worktrees = get_worktrees(repo);
		ਲਈ (p = worktrees; *p; p++) {
			ਬਣਤਰ worktree *wt = *p;
			ਬਣਤਰ ਸੂਚਕਾਂਕ_state istate =
				ਸੂਚਕਾਂਕ_STATE_INIT(repo);
			ਅੱਖਰ *ਰਸਤਾ, *wt_gitdir;

			/*
			 * Make a copy since the buffer is reusable
			 * and may get overwritten by other calls
			 * while we're examining the index.
			 */
			ਰਸਤਾ = xstrdup(worktree_git_ਰਸਤਾ(wt, "index"));
			wt_gitdir = get_worktree_git_dir(wt);

			read_ਸੂਚਕਾਂਕ_from(&istate, ਰਸਤਾ, wt_gitdir);
			fsck_ਸੂਚਕਾਂਕ(&istate, ਰਸਤਾ, wt->is_current);

			discard_ਸੂਚਕਾਂਕ(&istate);
			ਮੁਕਤ(wt_gitdir);
			ਮੁਕਤ(ਰਸਤਾ);
		}
		free_worktrees(worktrees);
	}

	check_connectivity(repo);

	ਜੇ (repo->settings.core_commit_ਗ੍ਰਾਫ) {
		ਬਣਤਰ odb_ਸਰੋਤ_files *files = odb_ਸਰੋਤ_files_downcast(repo->objects->ਸਰੋਤ);
		ਬਣਤਰ ਬੱਚਾ_process commit_ਗ੍ਰਾਫ_verify = ਬੱਚਾ_PROCESS_INIT;

		ਲਈ (ਬਣਤਰ odb_files_dir *dir = files->dirs; dir; dir = dir->ਅਗਲਾ) {
			ਬੱਚਾ_process_init(&commit_ਗ੍ਰਾਫ_verify);
			commit_ਗ੍ਰਾਫ_verify.git_cmd = 1;
			strvec_pushl(&commit_ਗ੍ਰਾਫ_verify.ਦਲੀਲਾਂ, "commit-graph",
				     "verify", "--object-dir", dir->abspath, ਨਿਰਾ);
			ਜੇ (show_ਤਰੱਕੀ)
				strvec_ਧੱਕੋ(&commit_ਗ੍ਰਾਫ_verify.ਦਲੀਲਾਂ, "--progress");
			ਨਹੀਂਤਾਂ
				strvec_ਧੱਕੋ(&commit_ਗ੍ਰਾਫ_verify.ਦਲੀਲਾਂ, "--no-progress");
			ਜੇ (ਲੜੀਵਾਰ_command(&commit_ਗ੍ਰਾਫ_verify))
				errors_ਮਿਲਿਆ |= ERROR_COMMIT_ਗ੍ਰਾਫ;
		}
	}

	free_snapshot_refs(&snap);
	ਵਾਪਸ errors_ਮਿਲਿਆ;
}
