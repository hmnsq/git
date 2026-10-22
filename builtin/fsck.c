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

#وصف_ڪريو REACHABLE 0x0001
#وصف_ڪريو ڏٺل_2      0x0002
#وصف_ڪريو HAS_OBJ   0x0004
/* This flag is set if something points to this object. */
#وصف_ڪريو استعمال_ٿيل      0x0008

ساكن صحيح show_پاڙ;
ساكن صحيح show_ٽيگ_فهرست;
ساكن صحيح show_unreachable;
ساكن صحيح include_reflogs = 1;
ساكن صحيح connectivity_only;
ساكن صحيح check_strict;
ساكن بنية fsck_options fsck_walk_options;
ساكن بنية fsck_options fsck_obj_options;
ساكن صحيح errors_مليو;
ساكن صحيح write_گم_and_مليو;
ساكن صحيح verbose;
ساكن صحيح show_ترقي = -1;
ساكن صحيح show_dangling = 1;
ساكن timestamp_t now;
#وصف_ڪريو ERROR_OBJECT 01
#وصف_ڪريو ERROR_REACHABLE 02
#وصف_ڪريو ERROR_REFS 010
#وصف_ڪريو ERROR_COMMIT_گراف 020

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
	errors_مليو |= ERROR_OBJECT;
	/* TRANSLATORS: e.g. error in tree 01bfda: <more explanation> */
	fprintf_ln(stderr, _("error in %s %s: %s"),
		   printable_type(repo, &obj->oid, obj->type),
		   describe_object(&obj->oid), err);
	أرجع -1;
}

ساكن صحيح fsck_objects_error_func(بنية fsck_options *o,
				   فراغ *fsck_report,
				   تعداد fsck_نياپو_type نياپو_type,
				   تعداد fsck_نياپو_id نياپو_id اڻ_استعمال_ٿيل,
				   ثابت حرف *پيغام)
{
	بنية fsck_object_report *report = fsck_report;
	ثابت بنية object_id *oid = report->oid;
	تعداد object_type object_type = report->object_type;

	بدل (نياپو_type) {
	حالة FSCK_WARN:
		/* TRANSLATORS: e.g. warning in tree 01bfda: <more explanation> */
		fprintf_ln(stderr, _("warning in %s %s: %s"),
			   printable_type(o->repo, oid, object_type),
			   describe_object(oid), پيغام);
		أرجع 0;
	حالة FSCK_ERROR:
		/* TRANSLATORS: e.g. error in tree 01bfda: <more explanation> */
		fprintf_ln(stderr, _("error in %s %s: %s"),
			   printable_type(o->repo, oid, object_type),
			   describe_object(oid), پيغام);
		أرجع 1;
	افتراضي:
		BUG("%d (FSCK_IGNORE?) should never trigger this callback",
		    نياپو_type);
	}
}

ساكن بنية object_array pending;

ساكن صحيح mark_object(بنية object *obj, تعداد object_type type,
		       فراغ *معلومات, بنية fsck_options *options)
{
	بنية object *والد = معلومات;

	/*
	 * The only case data is NULL or type is OBJ_ANY is when
	 * mark_object_reachable() calls us.  All the callers of
	 * that function has non-NULL obj hence ...
	 */
	إذا (!obj) {
		/* ... these references to parent->fld are safe here */
		printf_ln(_("broken link from %7s %s"),
			  printable_type(options->repo, &والد->oid, والد->type),
			  describe_object(&والد->oid));
		printf_ln(_("broken link from %7s %s"),
			  (type == OBJ_ANY ? _("unknown") : type_name(type)),
			  _("unknown"));
		errors_مليو |= ERROR_REACHABLE;
		أرجع 1;
	}

	إذا (type != OBJ_ANY && obj->type != type)
		/* ... and the reference to parent is safe here */
		objerror(options->repo, والد, _("wrong object type in link"));

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
		إذا (والد && !odb_has_object(options->repo->objects, &obj->oid,
					      ODB_HAS_OBJECT_RECHECK_PACKED)) {
			printf_ln(_("broken link from %7s %s\n"
				    "              to %7s %s"),
				  printable_type(options->repo, &والد->oid, والد->type),
				  describe_object(&والد->oid),
				  printable_type(options->repo, &obj->oid, obj->type),
				  describe_object(&obj->oid));
			errors_مليو |= ERROR_REACHABLE;
		}
		أرجع 1;
	}

	جوڙيو_object_array(obj, عدم, &pending);
	أرجع 0;
}

ساكن فراغ mark_object_reachable(بنية object *obj)
{
	mark_object(obj, OBJ_ANY, عدم, &fsck_walk_options);
}

ساكن صحيح traverse_one_object(بنية object *obj)
{
	صحيح نتيجو = fsck_walk(obj, obj, &fsck_walk_options);

	إذا (obj->type == OBJ_وڻ) {
		بنية وڻ *وڻ = (بنية وڻ *)obj;
		free_وڻ_بفر(وڻ);
	}
	أرجع نتيجو;
}

ساكن صحيح traverse_reachable(بنية repository *repo)
{
	بنية ترقي *ترقي = عدم;
	بنانشان صحيح nr = 0;
	صحيح نتيجو = 0;
	إذا (show_ترقي)
		ترقي = شروعات_delayed_ترقي(repo,
						  _("Checking connectivity"), 0);
	طالما (pending.nr) {
		نتيجو |= traverse_one_object(object_array_ڪڍو(&pending));
		display_ترقي(ترقي, ++nr);
	}
	stop_ترقي(&ترقي);
	أرجع !!نتيجو;
}

ساكن صحيح mark_استعمال_ٿيل(بنية object *obj, تعداد object_type type اڻ_استعمال_ٿيل,
		     فراغ *معلومات اڻ_استعمال_ٿيل, بنية fsck_options *options اڻ_استعمال_ٿيل)
{
	إذا (!obj)
		أرجع 1;
	obj->flags |= استعمال_ٿيل;
	أرجع 0;
}

ساكن صحيح mark_unreachable_referents(ثابت بنية object_id *oid,
				      بنية object_info *oi اڻ_استعمال_ٿيل,
				      فراغ *معلومات)
{
	بنية repository *repo = معلومات;
	بنية fsck_options options;
	بنية object *obj = lookup_object(معلومات, oid);

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
	options.walk = mark_استعمال_ٿيل;
	fsck_walk(obj, عدم, &options);
	إذا (obj->type == OBJ_وڻ)
		free_وڻ_بفر((بنية وڻ *)obj);

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
		errors_مليو |= ERROR_REACHABLE;
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
	إذا (!(obj->flags & استعمال_ٿيل)) {
		إذا (show_dangling)
			printf_ln(_("dangling %s %s"),
				  printable_type(repo, &obj->oid, obj->type),
				  describe_object(&obj->oid));
		إذا (write_گم_and_مليو) {
			حرف *filename = repo_git_رستو(repo, "lost-found/%s/%s",
				obj->type == OBJ_COMMIT ? "commit" : "other",
				describe_object(&obj->oid));
			FILE *f;

			إذا (محفوظ_create_leading_directories_const(repo, filename)) {
				error(_("could not create lost-found"));
				حرر(filename);
				أرجع;
			}
			f = xfopen(filename, "w");
			إذا (obj->type == OBJ_BLOB) {
				إذا (odb_وهڪرو_blob_منزل_fd(repo->objects, fileno(f),
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
	إذا (connectivity_only && (show_dangling || write_گم_and_مليو)) {
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
	max = get_max_object_اشاريو(repo);
	إذا (verbose)
		fprintf_ln(stderr, _("Checking connectivity (%d objects)"), max);

	لكل (i = 0; i < max; i++) {
		بنية object *obj = get_اشاريي_وارو_object(repo, i);

		إذا (obj)
			check_object(repo, obj);
	}
}

ساكن صحيح fsck_obj_بفر(ثابت بنية object_id *oid, تعداد object_type type,
			   بنانشان ڊگهو سائيز, فراغ *بفر, صحيح *eaten, فراغ *cb_معلومات)
{
	بنية repository *repo = cb_معلومات;
	بنية object *obj;
	صحيح err;

	/*
	 * Note, buffer may be NULL if type is OBJ_BLOB. See
	 * verify_packfile(), data_valid variable for details.
	 */
	obj = parse_object_بفر(repo, oid, type, سائيز, بفر, eaten);
	إذا (!obj) {
		errors_مليو |= ERROR_OBJECT;
		err = error(_("%s: object corrupt or missing"),
			    oid_منزل_hex(oid));
		اذهب ٻاهريون;
	}

	obj->flags &= ~REACHABLE;
	obj->flags |= HAS_OBJ | ڏٺل_2;

	إذا (verbose)
		fprintf_ln(stderr, _("Checking %s %s"),
			   printable_type(repo, &obj->oid, obj->type),
			   describe_object(&obj->oid));

	إذا (fsck_walk(obj, عدم, &fsck_obj_options))
		objerror(repo, obj, _("broken links"));

	err = fsck_object(obj, بفر, سائيز, &fsck_obj_options);
	إذا (err)
		اذهب ٻاهريون;

	إذا (obj->type == OBJ_COMMIT) {
		بنية commit *commit = (بنية commit *) obj;

		إذا (!commit->parents && show_پاڙ)
			printf_ln(_("root %s"),
				  describe_object(&commit->object.oid));
	}

	إذا (obj->type == OBJ_ٽيگ) {
		بنية ٽيگ *ٽيگ = (بنية ٽيگ *) obj;

		إذا (show_ٽيگ_فهرست && ٽيگ->tagged) {
			printf_ln(_("tagged %s %s (%s) in %s"),
				  printable_type(repo, &ٽيگ->tagged->oid, ٽيگ->tagged->type),
				  describe_object(&ٽيگ->tagged->oid),
				  ٽيگ->ٽيگ,
				  describe_object(&ٽيگ->object.oid));
		}
	}

ٻاهريون:
	إذا (obj && obj->type == OBJ_وڻ)
		free_وڻ_بفر((بنية وڻ *)obj);
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
			obj->flags |= استعمال_ٿيل;
			mark_object_reachable(obj);
		} وإلا إذا (!is_promisor_object(repo, oid)) {
			error(_("%s: invalid reflog entry %s"),
			      refname, oid_منزل_hex(oid));
			errors_مليو |= ERROR_REACHABLE;
		}
	}
}

ساكن صحيح fsck_handle_reflog_ent(ثابت حرف *refname,
				  بنية object_id *ooid, بنية object_id *noid,
				  ثابت حرف *email اڻ_استعمال_ٿيل,
				  timestamp_t timestamp, صحيح tz اڻ_استعمال_ٿيل,
				  ثابت حرف *پيغام اڻ_استعمال_ٿيل, فراغ *cb_معلومات)
{
	بنية repository *repo = cb_معلومات;

	إذا (now && timestamp > now)
		أرجع 0;

	إذا (verbose)
		fprintf_ln(stderr, _("Checking reflog %s->%s"),
			   oid_منزل_hex(ooid), oid_منزل_hex(noid));

	fsck_handle_reflog_oid(repo, refname, ooid, 0);
	fsck_handle_reflog_oid(repo, refname, noid, timestamp);
	أرجع 0;
}

ساكن صحيح fsck_handle_reflog(ثابت حرف *logname, فراغ *cb_معلومات)
{
	بنية strbuf refname = STRBUF_INIT;
	بنية worktree *wt = cb_معلومات;

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
	سائيز_t nr;
	سائيز_t alloc;
	بنية ref_snapshot *ref;
	/* TODO: Consider also snapshotting the index of each worktree. */
};

بنية snapshot_ref_معلومات {
	بنية repository *repo;
	بنية snapshot *snap;
};

ساكن صحيح snapshot_ref(ثابت بنية reference *ref, فراغ *cb_معلومات)
{
	بنية snapshot_ref_معلومات *معلومات = cb_معلومات;
	بنية snapshot *snap = معلومات->snap;
	بنية object *obj;

	obj = parse_object(معلومات->repo, ref->oid);
	إذا (!obj) {
		إذا (is_promisor_object(معلومات->repo, ref->oid)) {
			/*
			 * Increment default_refs anyway, because this is a
			 * valid ref.
			 */
			 default_refs++;
			 أرجع 0;
		}
		error(_("%s: invalid sha1 pointer %s"),
		      ref->name, oid_منزل_hex(ref->oid));
		errors_مليو |= ERROR_REACHABLE;
		/* We'll continue with the rest despite the error.. */
		أرجع 0;
	}
	إذا (obj->type != OBJ_COMMIT && is_branch(ref->name)) {
		error(_("%s: not a commit"), ref->name);
		errors_مليو |= ERROR_REFS;
	}
	default_refs++;

	ALLOC_GROW(snap->ref, snap->nr + 1, snap->alloc);
	snap->ref[snap->nr].refname = xstrdup(ref->name);
	oidcpy(&snap->ref[snap->nr].oid, ref->oid);
	snap->nr++;

	أرجع 0;
}

ساكن صحيح fsck_handle_ref(ثابت بنية reference *ref, فراغ *cb_معلومات)
{
	بنية repository *repo = cb_معلومات;
	بنية object *obj;

	obj = parse_object(repo, ref->oid);
	obj->flags |= استعمال_ٿيل;
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
	بنية snapshot_ref_معلومات معلومات = {
		.repo = repo,
		.snap = snap,
	};
	بنية worktree **worktrees, **p;
	ثابت حرف *سر_points_at;
	بنية object_id سر_oid;

	لكل (صحيح i = 0; i < argc; i++) {
		ثابت حرف *arg = argv[i];
		بنية object_id oid;
		إذا (!repo_get_oid(repo, arg, &oid)) {
			بنية reference ref = {
				.name = arg,
				.oid = &oid,
			};

			snapshot_ref(&ref, &معلومات);
			استمر;
		}
		error(_("invalid parameter: expected sha1, got '%s'"), arg);
		errors_مليو |= ERROR_OBJECT;
	}

	إذا (argc) {
		include_reflogs = 0;
		أرجع;
	}

	refs_for_each_ref_ext(get_main_ref_store(repo),
			      snapshot_ref, &معلومات, &opts);

	worktrees = get_worktrees(repo);
	لكل (p = worktrees; *p; p++) {
		بنية worktree *wt = *p;
		بنية strbuf refname = STRBUF_INIT;

		strbuf_worktree_ref(wt, &refname, "HEAD");

		سر_points_at = refs_resolve_ref_unsafe(get_main_ref_store(repo),
							 refname.buf, 0, &سر_oid, عدم);

		إذا (سر_points_at && !is_null_oid(&سر_oid)) {
			بنية reference ref = {
				.name = refname.buf,
				.oid = &سر_oid,
			};

			snapshot_ref(&ref, &معلومات);
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
	now = وقت(عدم);
}


ساكن فراغ free_snapshot_refs(بنية snapshot *snap)
{
	لكل (سائيز_t i = 0; i < snap->nr; i++)
		حرر(snap->ref[i].refname);
	حرر(snap->ref);
}

ساكن فراغ process_refs(بنية repository *repo, بنية snapshot *snap)
{
	بنية worktree **worktrees, **p;

	لكل (سائيز_t i = 0; i < snap->nr; i++) {
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

ساكن صحيح fsck_cache_وڻ(بنية repository *repo, بنية cache_وڻ *it, ثابت حرف *اشاريو_رستو)
{
	صحيح i;
	صحيح err = 0;

	إذا (verbose)
		fprintf_ln(stderr, _("Checking cache tree of %s"), اشاريو_رستو);

	إذا (0 <= it->داخلا_ڳڻپ) {
		بنية object *obj = parse_object(repo, &it->oid);
		إذا (!obj) {
			error(_("%s: invalid sha1 pointer in cache-tree of %s"),
			      oid_منزل_hex(&it->oid), اشاريو_رستو);
			errors_مليو |= ERROR_REFS;
			أرجع 1;
		}
		obj->flags |= استعمال_ٿيل;
		fsck_put_object_name(&fsck_walk_options, &it->oid, ":");
		mark_object_reachable(obj);
		إذا (obj->type != OBJ_وڻ)
			err |= objerror(repo, obj, _("non-tree in cache-tree"));
	}
	لكل (i = 0; i < it->subtree_nr; i++)
		err |= fsck_cache_وڻ(repo, it->هيٺ_ڏانهن[i]->cache_وڻ, اشاريو_رستو);
	أرجع err;
}

ساكن صحيح fsck_resolve_undo(بنية اشاريو_state *istate,
			     ثابت حرف *اشاريو_رستو)
{
	بنية string_list_شي *شي;
	بنية string_list *resolve_undo = istate->resolve_undo;

	إذا (!resolve_undo)
		أرجع 0;

	for_each_string_list_شي(شي, resolve_undo) {
		ثابت حرف *رستو = شي->string;
		بنية resolve_undo_info *ru = شي->util;
		صحيح i;

		إذا (!ru)
			استمر;
		لكل (i = 0; i < 3; i++) {
			بنية object *obj;

			إذا (!ru->گهڻائي[i] || !S_ISREG(ru->گهڻائي[i]))
				استمر;

			obj = parse_object(istate->repo, &ru->oid[i]);
			إذا (!obj) {
				error(_("%s: invalid sha1 pointer in resolve-undo of %s"),
				      oid_منزل_hex(&ru->oid[i]),
				      اشاريو_رستو);
				errors_مليو |= ERROR_REFS;
				استمر;
			}
			obj->flags |= استعمال_ٿيل;
			fsck_put_object_name(&fsck_walk_options, &ru->oid[i],
					     ":(%d):%s", i, رستو);
			mark_object_reachable(obj);
		}
	}
	أرجع 0;
}

ساكن فراغ fsck_اشاريو(بنية اشاريو_state *istate, ثابت حرف *اشاريو_رستو,
		       صحيح is_current_worktree)
{
	بنانشان صحيح i;

	/* TODO: audit for interaction with sparse-index. */
	ensure_ڀريل_اشاريو(istate);
	لكل (i = 0; i < istate->cache_nr; i++) {
		بنانشان صحيح گهڻائي;
		بنية blob *blob;
		بنية object *obj;

		گهڻائي = istate->cache[i]->ce_گهڻائي;
		إذا (S_ISGITLINK(گهڻائي))
			استمر;
		blob = lookup_blob(istate->repo,
				   &istate->cache[i]->oid);
		إذا (!blob)
			استمر;
		obj = &blob->object;
		obj->flags |= استعمال_ٿيل;
		fsck_put_object_name(&fsck_walk_options, &obj->oid,
				     "%s:%s",
				     is_current_worktree ? "" : اشاريو_رستو,
				     istate->cache[i]->name);
		mark_object_reachable(obj);
	}
	إذا (istate->cache_وڻ)
		fsck_cache_وڻ(istate->repo, istate->cache_وڻ, اشاريو_رستو);
	fsck_resolve_undo(istate, اشاريو_رستو);
}

ساكن صحيح mark_object_for_connectivity(ثابت بنية object_id *oid,
					بنية object_info *oi اڻ_استعمال_ٿيل,
					فراغ *cb_معلومات)
{
	بنية repository *repo = cb_معلومات;
	بنية object *obj = lookup_unknown_object(repo, oid);
	obj->flags |= HAS_OBJ;
	أرجع 0;
}

ساكن فراغ fsck_refs(بنية repository *r)
{
	بنية ٻار_process refs_verify = ٻار_PROCESS_INIT;
	بنية ترقي *ترقي = عدم;

	إذا (show_ترقي)
		ترقي = شروعات_ترقي(r, _("Checking ref database"), 1);

	إذا (verbose)
		fprintf_ln(stderr, _("Checking ref database"));

	ٻار_process_init(&refs_verify);
	refs_verify.git_cmd = 1;
	strvec_pushl(&refs_verify.دليلون, "refs", "verify", عدم);
	إذا (verbose)
		strvec_ڌڪو(&refs_verify.دليلون, "--verbose");
	إذا (check_strict)
		strvec_ڌڪو(&refs_verify.دليلون, "--strict");

	إذا (لاڳيتو_command(&refs_verify))
		errors_مليو |= ERROR_REFS;

	display_ترقي(ترقي, 1);
	stop_ترقي(&ترقي);
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
	     ثابت حرف *اڳياڙي,
	     بنية repository *repo)
{
	بنية odb_fsck_options odb_fsck_opts = {
		.flags = ODB_FSCK_ڀريل,
		.object_cb = fsck_obj_بفر,
		.object_payload = repo,
	};
	صحيح keep_cache_objects = 0;
	صحيح name_objects = 0;
	صحيح check_references = 1;
	بنية option fsck_opts[] = {
		OPT__VERBOSE(&verbose, N_("be verbose")),
		OPT_BOOL(0, "unreachable", &show_unreachable, N_("show unreachable objects")),
		OPT_BOOL(0, "dangling", &show_dangling, N_("show dangling objects")),
		OPT_BOOL(0, "tags", &show_ٽيگ_فهرست, N_("report tags")),
		OPT_BOOL(0, "root", &show_پاڙ, N_("report root nodes")),
		OPT_BOOL(0, "cache", &keep_cache_objects, N_("make index objects head nodes")),
		OPT_BOOL(0, "reflogs", &include_reflogs, N_("make reflogs head nodes (default)")),
		OPT_ٻٽو_انگ(0, "full", &odb_fsck_opts.flags,
			N_("also consider packs and alternate objects"), ODB_FSCK_ڀريل),
		OPT_BOOL(0, "connectivity-only", &connectivity_only, N_("check only connectivity")),
		OPT_BOOL(0, "strict", &check_strict, N_("enable more strict checking")),
		OPT_BOOL(0, "lost-found", &write_گم_and_مليو,
					N_("write dangling objects in .git/lost-found")),
		OPT_BOOL(0, "progress", &show_ترقي, N_("show progress")),
		OPT_BOOL(0, "name-objects", &name_objects, N_("show verbose names for reachable objects")),
		OPT_BOOL(0, "references", &check_references, N_("check reference database consistency")),
		OPT_پڇاڙي(),
	};
	بنية snapshot snap = {
		.nr = 0,
		.alloc = 0,
		.ref = عدم
	};

	errors_مليو = 0;
	disable_replace_refs();
	save_commit_بفر = 0;

	argc = parse_options(argc, argv, اڳياڙي, fsck_opts, fsck_usage, 0);

	/* fsck knows how to handle missing promisor objects */
	repo->fetch_if_missing = 0;

	fsck_options_init(&fsck_walk_options, repo, FSCK_OPTIONS_DEFAULT);
	fsck_walk_options.walk = mark_object;

	fsck_options_init(&fsck_obj_options, repo, FSCK_OPTIONS_DEFAULT);
	fsck_obj_options.walk = mark_استعمال_ٿيل;
	fsck_obj_options.error_func = fsck_objects_error_func;
	إذا (check_strict)
		fsck_obj_options.strict = 1;

	إذا (show_ترقي == -1)
		show_ترقي = isatty(2);
	إذا (verbose) {
		show_ترقي = 0;
		odb_fsck_opts.flags |= ODB_FSCK_VERBOSE;
	}
	إذا (show_ترقي)
		odb_fsck_opts.flags |= ODB_FSCK_ترقي;

	إذا (write_گم_and_مليو) {
		odb_fsck_opts.flags |= ODB_FSCK_ڀريل;
		include_reflogs = 0;
	}

	إذا (name_objects)
		fsck_enable_object_نالا(&fsck_walk_options);

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
			errors_مليو |= ERROR_OBJECT;

		إذا (fsck_پڄاڻي(&fsck_obj_options))
			errors_مليو |= ERROR_OBJECT;
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

		verify_اشاريو_checksum = 1;
		verify_ce_ترتيب = 1;

		worktrees = get_worktrees(repo);
		لكل (p = worktrees; *p; p++) {
			بنية worktree *wt = *p;
			بنية اشاريو_state istate =
				اشاريو_STATE_INIT(repo);
			حرف *رستو, *wt_gitdir;

			/*
			 * Make a copy since the buffer is reusable
			 * and may get overwritten by other calls
			 * while we're examining the index.
			 */
			رستو = xstrdup(worktree_git_رستو(wt, "index"));
			wt_gitdir = get_worktree_git_dir(wt);

			read_اشاريو_from(&istate, رستو, wt_gitdir);
			fsck_اشاريو(&istate, رستو, wt->is_current);

			discard_اشاريو(&istate);
			حرر(wt_gitdir);
			حرر(رستو);
		}
		free_worktrees(worktrees);
	}

	check_connectivity(repo);

	إذا (repo->settings.core_commit_گراف) {
		بنية odb_ذريعو_files *files = odb_ذريعو_files_downcast(repo->objects->ذريعو);
		بنية ٻار_process commit_گراف_verify = ٻار_PROCESS_INIT;

		لكل (بنية odb_files_dir *dir = files->dirs; dir; dir = dir->ايندڙ) {
			ٻار_process_init(&commit_گراف_verify);
			commit_گراف_verify.git_cmd = 1;
			strvec_pushl(&commit_گراف_verify.دليلون, "commit-graph",
				     "verify", "--object-dir", dir->abspath, عدم);
			إذا (show_ترقي)
				strvec_ڌڪو(&commit_گراف_verify.دليلون, "--progress");
			وإلا
				strvec_ڌڪو(&commit_گراف_verify.دليلون, "--no-progress");
			إذا (لاڳيتو_command(&commit_گراف_verify))
				errors_مليو |= ERROR_COMMIT_گراف;
		}
	}

	free_snapshot_refs(&snap);
	أرجع errors_مليو;
}
