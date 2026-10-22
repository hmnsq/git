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

#تعریف_کن REACHABLE 0x0001
#تعریف_کن دیده_2      0x0002
#تعریف_کن HAS_OBJ   0x0004
/* This flag is set if something points to this object. */
#تعریف_کن استفاده_شده      0x0008

ساكن صحيح show_ریشه;
ساكن صحيح show_برچسبها;
ساكن صحيح show_unreachable;
ساكن صحيح include_reflogs = 1;
ساكن صحيح connectivity_only;
ساكن صحيح check_strict;
ساكن بنية fsck_options fsck_walk_options;
ساكن بنية fsck_options fsck_obj_options;
ساكن صحيح errors_یافت;
ساكن صحيح write_گمشده_and_یافت;
ساكن صحيح verbose;
ساكن صحيح show_پیشرفت = -1;
ساكن صحيح show_dangling = 1;
ساكن timestamp_t now;
#تعریف_کن ERROR_OBJECT 01
#تعریف_کن ERROR_REACHABLE 02
#تعریف_کن ERROR_REFS 010
#تعریف_کن ERROR_COMMIT_گراف 020

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
	errors_یافت |= ERROR_OBJECT;
	/* TRANSLATORS: e.g. error in tree 01bfda: <more explanation> */
	fprintf_ln(stderr, _("error in %s %s: %s"),
		   printable_type(repo, &obj->oid, obj->type),
		   describe_object(&obj->oid), err);
	أرجع -1;
}

ساكن صحيح fsck_objects_error_func(بنية fsck_options *o,
				   فراغ *fsck_report,
				   تعداد fsck_پیام_type پیام_type,
				   تعداد fsck_پیام_id پیام_id_2 استفادهنشده,
				   ثابت حرف *پیام_2)
{
	بنية fsck_object_report *report = fsck_report;
	ثابت بنية object_id *oid = report->oid;
	تعداد object_type object_type = report->object_type;

	بدل (پیام_type) {
	حالة FSCK_WARN:
		/* TRANSLATORS: e.g. warning in tree 01bfda: <more explanation> */
		fprintf_ln(stderr, _("warning in %s %s: %s"),
			   printable_type(o->repo, oid, object_type),
			   describe_object(oid), پیام_2);
		أرجع 0;
	حالة FSCK_ERROR:
		/* TRANSLATORS: e.g. error in tree 01bfda: <more explanation> */
		fprintf_ln(stderr, _("error in %s %s: %s"),
			   printable_type(o->repo, oid, object_type),
			   describe_object(oid), پیام_2);
		أرجع 1;
	افتراضي:
		BUG("%d (FSCK_IGNORE?) should never trigger this callback",
		    پیام_type);
	}
}

ساكن بنية object_array pending;

ساكن صحيح mark_object(بنية object *obj, تعداد object_type type,
		       فراغ *داده, بنية fsck_options *options)
{
	بنية object *والد = داده;

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
		errors_یافت |= ERROR_REACHABLE;
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
			errors_یافت |= ERROR_REACHABLE;
		}
		أرجع 1;
	}

	جمع_کن_object_array(obj, عدم, &pending);
	أرجع 0;
}

ساكن فراغ mark_object_reachable(بنية object *obj)
{
	mark_object(obj, OBJ_ANY, عدم, &fsck_walk_options);
}

ساكن صحيح traverse_one_object(بنية object *obj)
{
	صحيح نتیجه = fsck_walk(obj, obj, &fsck_walk_options);

	إذا (obj->type == OBJ_درخت) {
		بنية درخت *درخت = (بنية درخت *)obj;
		free_درخت_میانگیر(درخت);
	}
	أرجع نتیجه;
}

ساكن صحيح traverse_reachable(بنية repository *repo)
{
	بنية پیشرفت *پیشرفت = عدم;
	بیعلامت صحيح nr = 0;
	صحيح نتیجه = 0;
	إذا (show_پیشرفت)
		پیشرفت = شروع_delayed_پیشرفت(repo,
						  _("Checking connectivity"), 0);
	طالما (pending.nr) {
		نتیجه |= traverse_one_object(object_array_برداشت(&pending));
		display_پیشرفت(پیشرفت, ++nr);
	}
	stop_پیشرفت(&پیشرفت);
	أرجع !!نتیجه;
}

ساكن صحيح mark_استفاده_شده(بنية object *obj, تعداد object_type type استفادهنشده,
		     فراغ *داده استفادهنشده, بنية fsck_options *options استفادهنشده)
{
	إذا (!obj)
		أرجع 1;
	obj->flags |= استفاده_شده;
	أرجع 0;
}

ساكن صحيح mark_unreachable_referents(ثابت بنية object_id *oid,
				      بنية object_info *oi استفادهنشده,
				      فراغ *داده)
{
	بنية repository *repo = داده;
	بنية fsck_options options;
	بنية object *obj = lookup_object(داده, oid);

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
	options.walk = mark_استفاده_شده;
	fsck_walk(obj, عدم, &options);
	إذا (obj->type == OBJ_درخت)
		free_درخت_میانگیر((بنية درخت *)obj);

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
		errors_یافت |= ERROR_REACHABLE;
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
	إذا (!(obj->flags & استفاده_شده)) {
		إذا (show_dangling)
			printf_ln(_("dangling %s %s"),
				  printable_type(repo, &obj->oid, obj->type),
				  describe_object(&obj->oid));
		إذا (write_گمشده_and_یافت) {
			حرف *filename = repo_git_مسیر(repo, "lost-found/%s/%s",
				obj->type == OBJ_COMMIT ? "commit" : "other",
				describe_object(&obj->oid));
			FILE *f;

			إذا (امن_create_leading_directories_const(repo, filename)) {
				error(_("could not create lost-found"));
				حرر(filename);
				أرجع;
			}
			f = xfopen(filename, "w");
			إذا (obj->type == OBJ_BLOB) {
				إذا (odb_جریان_blob_مقصد_fd(repo->objects, fileno(f),
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
	إذا (connectivity_only && (show_dangling || write_گمشده_and_یافت)) {
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
	max = get_max_object_شاخص(repo);
	إذا (verbose)
		fprintf_ln(stderr, _("Checking connectivity (%d objects)"), max);

	لكل (i = 0; i < max; i++) {
		بنية object *obj = get_نمایهای_object(repo, i);

		إذا (obj)
			check_object(repo, obj);
	}
}

ساكن صحيح fsck_obj_میانگیر(ثابت بنية object_id *oid, تعداد object_type type,
			   بیعلامت دراز اندازه, فراغ *میانگیر, صحيح *eaten, فراغ *cb_داده)
{
	بنية repository *repo = cb_داده;
	بنية object *obj;
	صحيح err;

	/*
	 * Note, buffer may be NULL if type is OBJ_BLOB. See
	 * verify_packfile(), data_valid variable for details.
	 */
	obj = parse_object_میانگیر(repo, oid, type, اندازه, میانگیر, eaten);
	إذا (!obj) {
		errors_یافت |= ERROR_OBJECT;
		err = error(_("%s: object corrupt or missing"),
			    oid_مقصد_hex(oid));
		اذهب بیرون;
	}

	obj->flags &= ~REACHABLE;
	obj->flags |= HAS_OBJ | دیده_2;

	إذا (verbose)
		fprintf_ln(stderr, _("Checking %s %s"),
			   printable_type(repo, &obj->oid, obj->type),
			   describe_object(&obj->oid));

	إذا (fsck_walk(obj, عدم, &fsck_obj_options))
		objerror(repo, obj, _("broken links"));

	err = fsck_object(obj, میانگیر, اندازه, &fsck_obj_options);
	إذا (err)
		اذهب بیرون;

	إذا (obj->type == OBJ_COMMIT) {
		بنية commit *commit = (بنية commit *) obj;

		إذا (!commit->parents && show_ریشه)
			printf_ln(_("root %s"),
				  describe_object(&commit->object.oid));
	}

	إذا (obj->type == OBJ_برچسب) {
		بنية برچسب *برچسب = (بنية برچسب *) obj;

		إذا (show_برچسبها && برچسب->tagged) {
			printf_ln(_("tagged %s %s (%s) in %s"),
				  printable_type(repo, &برچسب->tagged->oid, برچسب->tagged->type),
				  describe_object(&برچسب->tagged->oid),
				  برچسب->برچسب,
				  describe_object(&برچسب->object.oid));
		}
	}

بیرون:
	إذا (obj && obj->type == OBJ_درخت)
		free_درخت_میانگیر((بنية درخت *)obj);
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
			obj->flags |= استفاده_شده;
			mark_object_reachable(obj);
		} وإلا إذا (!is_promisor_object(repo, oid)) {
			error(_("%s: invalid reflog entry %s"),
			      refname, oid_مقصد_hex(oid));
			errors_یافت |= ERROR_REACHABLE;
		}
	}
}

ساكن صحيح fsck_handle_reflog_ent(ثابت حرف *refname,
				  بنية object_id *ooid, بنية object_id *noid,
				  ثابت حرف *email استفادهنشده,
				  timestamp_t timestamp, صحيح tz استفادهنشده,
				  ثابت حرف *پیام_2 استفادهنشده, فراغ *cb_داده)
{
	بنية repository *repo = cb_داده;

	إذا (now && timestamp > now)
		أرجع 0;

	إذا (verbose)
		fprintf_ln(stderr, _("Checking reflog %s->%s"),
			   oid_مقصد_hex(ooid), oid_مقصد_hex(noid));

	fsck_handle_reflog_oid(repo, refname, ooid, 0);
	fsck_handle_reflog_oid(repo, refname, noid, timestamp);
	أرجع 0;
}

ساكن صحيح fsck_handle_reflog(ثابت حرف *logname, فراغ *cb_داده)
{
	بنية strbuf refname = STRBUF_INIT;
	بنية worktree *wt = cb_داده;

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
	اندازه_t nr;
	اندازه_t alloc;
	بنية ref_snapshot *ref;
	/* TODO: Consider also snapshotting the index of each worktree. */
};

بنية snapshot_ref_داده {
	بنية repository *repo;
	بنية snapshot *snap;
};

ساكن صحيح snapshot_ref(ثابت بنية reference *ref, فراغ *cb_داده)
{
	بنية snapshot_ref_داده *داده = cb_داده;
	بنية snapshot *snap = داده->snap;
	بنية object *obj;

	obj = parse_object(داده->repo, ref->oid);
	إذا (!obj) {
		إذا (is_promisor_object(داده->repo, ref->oid)) {
			/*
			 * Increment default_refs anyway, because this is a
			 * valid ref.
			 */
			 default_refs++;
			 أرجع 0;
		}
		error(_("%s: invalid sha1 pointer %s"),
		      ref->name, oid_مقصد_hex(ref->oid));
		errors_یافت |= ERROR_REACHABLE;
		/* We'll continue with the rest despite the error.. */
		أرجع 0;
	}
	إذا (obj->type != OBJ_COMMIT && is_branch(ref->name)) {
		error(_("%s: not a commit"), ref->name);
		errors_یافت |= ERROR_REFS;
	}
	default_refs++;

	ALLOC_GROW(snap->ref, snap->nr + 1, snap->alloc);
	snap->ref[snap->nr].refname = xstrdup(ref->name);
	oidcpy(&snap->ref[snap->nr].oid, ref->oid);
	snap->nr++;

	أرجع 0;
}

ساكن صحيح fsck_handle_ref(ثابت بنية reference *ref, فراغ *cb_داده)
{
	بنية repository *repo = cb_داده;
	بنية object *obj;

	obj = parse_object(repo, ref->oid);
	obj->flags |= استفاده_شده;
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
	بنية snapshot_ref_داده داده = {
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

			snapshot_ref(&ref, &داده);
			استمر;
		}
		error(_("invalid parameter: expected sha1, got '%s'"), arg);
		errors_یافت |= ERROR_OBJECT;
	}

	إذا (argc) {
		include_reflogs = 0;
		أرجع;
	}

	refs_for_each_ref_ext(get_main_ref_store(repo),
			      snapshot_ref, &داده, &opts);

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

			snapshot_ref(&ref, &داده);
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
	now = زمان(عدم);
}


ساكن فراغ free_snapshot_refs(بنية snapshot *snap)
{
	لكل (اندازه_t i = 0; i < snap->nr; i++)
		حرر(snap->ref[i].refname);
	حرر(snap->ref);
}

ساكن فراغ process_refs(بنية repository *repo, بنية snapshot *snap)
{
	بنية worktree **worktrees, **p;

	لكل (اندازه_t i = 0; i < snap->nr; i++) {
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

ساكن صحيح fsck_cache_درخت(بنية repository *repo, بنية cache_درخت *it, ثابت حرف *شاخص_مسیر)
{
	صحيح i;
	صحيح err = 0;

	إذا (verbose)
		fprintf_ln(stderr, _("Checking cache tree of %s"), شاخص_مسیر);

	إذا (0 <= it->مدخل_شمار) {
		بنية object *obj = parse_object(repo, &it->oid);
		إذا (!obj) {
			error(_("%s: invalid sha1 pointer in cache-tree of %s"),
			      oid_مقصد_hex(&it->oid), شاخص_مسیر);
			errors_یافت |= ERROR_REFS;
			أرجع 1;
		}
		obj->flags |= استفاده_شده;
		fsck_put_object_name(&fsck_walk_options, &it->oid, ":");
		mark_object_reachable(obj);
		إذا (obj->type != OBJ_درخت)
			err |= objerror(repo, obj, _("non-tree in cache-tree"));
	}
	لكل (i = 0; i < it->subtree_nr; i++)
		err |= fsck_cache_درخت(repo, it->پایین[i]->cache_درخت, شاخص_مسیر);
	أرجع err;
}

ساكن صحيح fsck_resolve_undo(بنية شاخص_state *istate,
			     ثابت حرف *شاخص_مسیر)
{
	بنية string_list_قلم *قلم;
	بنية string_list *resolve_undo = istate->resolve_undo;

	إذا (!resolve_undo)
		أرجع 0;

	for_each_string_list_قلم(قلم, resolve_undo) {
		ثابت حرف *مسیر = قلم->string;
		بنية resolve_undo_info *ru = قلم->util;
		صحيح i;

		إذا (!ru)
			استمر;
		لكل (i = 0; i < 3; i++) {
			بنية object *obj;

			إذا (!ru->نما[i] || !S_ISREG(ru->نما[i]))
				استمر;

			obj = parse_object(istate->repo, &ru->oid[i]);
			إذا (!obj) {
				error(_("%s: invalid sha1 pointer in resolve-undo of %s"),
				      oid_مقصد_hex(&ru->oid[i]),
				      شاخص_مسیر);
				errors_یافت |= ERROR_REFS;
				استمر;
			}
			obj->flags |= استفاده_شده;
			fsck_put_object_name(&fsck_walk_options, &ru->oid[i],
					     ":(%d):%s", i, مسیر);
			mark_object_reachable(obj);
		}
	}
	أرجع 0;
}

ساكن فراغ fsck_شاخص(بنية شاخص_state *istate, ثابت حرف *شاخص_مسیر,
		       صحيح is_current_worktree)
{
	بیعلامت صحيح i;

	/* TODO: audit for interaction with sparse-index. */
	ensure_پر_شاخص(istate);
	لكل (i = 0; i < istate->cache_nr; i++) {
		بیعلامت صحيح نما;
		بنية blob *blob;
		بنية object *obj;

		نما = istate->cache[i]->ce_نما;
		إذا (S_ISGITLINK(نما))
			استمر;
		blob = lookup_blob(istate->repo,
				   &istate->cache[i]->oid);
		إذا (!blob)
			استمر;
		obj = &blob->object;
		obj->flags |= استفاده_شده;
		fsck_put_object_name(&fsck_walk_options, &obj->oid,
				     "%s:%s",
				     is_current_worktree ? "" : شاخص_مسیر,
				     istate->cache[i]->name);
		mark_object_reachable(obj);
	}
	إذا (istate->cache_درخت)
		fsck_cache_درخت(istate->repo, istate->cache_درخت, شاخص_مسیر);
	fsck_resolve_undo(istate, شاخص_مسیر);
}

ساكن صحيح mark_object_for_connectivity(ثابت بنية object_id *oid,
					بنية object_info *oi استفادهنشده,
					فراغ *cb_داده)
{
	بنية repository *repo = cb_داده;
	بنية object *obj = lookup_unknown_object(repo, oid);
	obj->flags |= HAS_OBJ;
	أرجع 0;
}

ساكن فراغ fsck_refs(بنية repository *r)
{
	بنية فرزند_process refs_verify = فرزند_PROCESS_INIT;
	بنية پیشرفت *پیشرفت = عدم;

	إذا (show_پیشرفت)
		پیشرفت = شروع_پیشرفت(r, _("Checking ref database"), 1);

	إذا (verbose)
		fprintf_ln(stderr, _("Checking ref database"));

	فرزند_process_init(&refs_verify);
	refs_verify.git_cmd = 1;
	strvec_pushl(&refs_verify.آرگومانها, "refs", "verify", عدم);
	إذا (verbose)
		strvec_افزودن(&refs_verify.آرگومانها, "--verbose");
	إذا (check_strict)
		strvec_افزودن(&refs_verify.آرگومانها, "--strict");

	إذا (پیاپی_command(&refs_verify))
		errors_یافت |= ERROR_REFS;

	display_پیشرفت(پیشرفت, 1);
	stop_پیشرفت(&پیشرفت);
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
	     ثابت حرف *پیشوند,
	     بنية repository *repo)
{
	بنية odb_fsck_options odb_fsck_opts = {
		.flags = ODB_FSCK_پر,
		.object_cb = fsck_obj_میانگیر,
		.object_payload = repo,
	};
	صحيح keep_cache_objects = 0;
	صحيح name_objects = 0;
	صحيح check_references = 1;
	بنية option fsck_opts[] = {
		OPT__VERBOSE(&verbose, N_("be verbose")),
		OPT_BOOL(0, "unreachable", &show_unreachable, N_("show unreachable objects")),
		OPT_BOOL(0, "dangling", &show_dangling, N_("show dangling objects")),
		OPT_BOOL(0, "tags", &show_برچسبها, N_("report tags")),
		OPT_BOOL(0, "root", &show_ریشه, N_("report root nodes")),
		OPT_BOOL(0, "cache", &keep_cache_objects, N_("make index objects head nodes")),
		OPT_BOOL(0, "reflogs", &include_reflogs, N_("make reflogs head nodes (default)")),
		OPT_بیت(0, "full", &odb_fsck_opts.flags,
			N_("also consider packs and alternate objects"), ODB_FSCK_پر),
		OPT_BOOL(0, "connectivity-only", &connectivity_only, N_("check only connectivity")),
		OPT_BOOL(0, "strict", &check_strict, N_("enable more strict checking")),
		OPT_BOOL(0, "lost-found", &write_گمشده_and_یافت,
					N_("write dangling objects in .git/lost-found")),
		OPT_BOOL(0, "progress", &show_پیشرفت, N_("show progress")),
		OPT_BOOL(0, "name-objects", &name_objects, N_("show verbose names for reachable objects")),
		OPT_BOOL(0, "references", &check_references, N_("check reference database consistency")),
		OPT_انتها(),
	};
	بنية snapshot snap = {
		.nr = 0,
		.alloc = 0,
		.ref = عدم
	};

	errors_یافت = 0;
	disable_replace_refs();
	save_commit_میانگیر = 0;

	argc = parse_options(argc, argv, پیشوند, fsck_opts, fsck_usage, 0);

	/* fsck knows how to handle missing promisor objects */
	repo->fetch_if_missing = 0;

	fsck_options_init(&fsck_walk_options, repo, FSCK_OPTIONS_DEFAULT);
	fsck_walk_options.walk = mark_object;

	fsck_options_init(&fsck_obj_options, repo, FSCK_OPTIONS_DEFAULT);
	fsck_obj_options.walk = mark_استفاده_شده;
	fsck_obj_options.error_func = fsck_objects_error_func;
	إذا (check_strict)
		fsck_obj_options.strict = 1;

	إذا (show_پیشرفت == -1)
		show_پیشرفت = isatty(2);
	إذا (verbose) {
		show_پیشرفت = 0;
		odb_fsck_opts.flags |= ODB_FSCK_VERBOSE;
	}
	إذا (show_پیشرفت)
		odb_fsck_opts.flags |= ODB_FSCK_پیشرفت;

	إذا (write_گمشده_and_یافت) {
		odb_fsck_opts.flags |= ODB_FSCK_پر;
		include_reflogs = 0;
	}

	إذا (name_objects)
		fsck_enable_object_نامها(&fsck_walk_options);

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
			errors_یافت |= ERROR_OBJECT;

		إذا (fsck_پایان(&fsck_obj_options))
			errors_یافت |= ERROR_OBJECT;
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

		verify_شاخص_checksum = 1;
		verify_ce_ترتیب = 1;

		worktrees = get_worktrees(repo);
		لكل (p = worktrees; *p; p++) {
			بنية worktree *wt = *p;
			بنية شاخص_state istate =
				شاخص_STATE_INIT(repo);
			حرف *مسیر, *wt_gitdir;

			/*
			 * Make a copy since the buffer is reusable
			 * and may get overwritten by other calls
			 * while we're examining the index.
			 */
			مسیر = xstrdup(worktree_git_مسیر(wt, "index"));
			wt_gitdir = get_worktree_git_dir(wt);

			read_شاخص_from(&istate, مسیر, wt_gitdir);
			fsck_شاخص(&istate, مسیر, wt->is_current);

			discard_شاخص(&istate);
			حرر(wt_gitdir);
			حرر(مسیر);
		}
		free_worktrees(worktrees);
	}

	check_connectivity(repo);

	إذا (repo->settings.core_commit_گراف) {
		بنية odb_منبع_files *files = odb_منبع_files_downcast(repo->objects->منبع);
		بنية فرزند_process commit_گراف_verify = فرزند_PROCESS_INIT;

		لكل (بنية odb_files_dir *dir = files->dirs; dir; dir = dir->بعدی) {
			فرزند_process_init(&commit_گراف_verify);
			commit_گراف_verify.git_cmd = 1;
			strvec_pushl(&commit_گراف_verify.آرگومانها, "commit-graph",
				     "verify", "--object-dir", dir->abspath, عدم);
			إذا (show_پیشرفت)
				strvec_افزودن(&commit_گراف_verify.آرگومانها, "--progress");
			وإلا
				strvec_افزودن(&commit_گراف_verify.آرگومانها, "--no-progress");
			إذا (پیاپی_command(&commit_گراف_verify))
				errors_یافت |= ERROR_COMMIT_گراف;
		}
	}

	free_snapshot_refs(&snap);
	أرجع errors_یافت;
}
