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

#تعریف_کرو REACHABLE 0x0001
#تعریف_کرو نظر_آیا_2      0x0002
#تعریف_کرو HAS_OBJ   0x0004
/* This flag is set if something points to this object. */
#تعریف_کرو مستعمل      0x0008

ساکن صحیح show_جڑ;
ساکن صحیح show_لیبل_فہرست;
ساکن صحیح show_unreachable;
ساکن صحیح include_reflogs = 1;
ساکن صحیح connectivity_only;
ساکن صحیح check_strict;
ساکن ساخت fsck_options fsck_walk_options;
ساکن ساخت fsck_options fsck_obj_options;
ساکن صحیح errors_ملا;
ساکن صحیح write_گمشدہ_and_ملا;
ساکن صحیح verbose;
ساکن صحیح show_پیش_رفت = -1;
ساکن صحیح show_dangling = 1;
ساکن timestamp_t now;
#تعریف_کرو ERROR_OBJECT 01
#تعریف_کرو ERROR_REACHABLE 02
#تعریف_کرو ERROR_REFS 010
#تعریف_کرو ERROR_COMMIT_گراف 020

ساکن ثابت حرف *describe_object(ثابت ساخت object_id *oid)
{
	واپس fsck_describe_object(&fsck_walk_options, oid);
}

ساکن ثابت حرف *printable_type(ساخت repository *repo,
				  ثابت ساخت object_id *oid,
				  شمار object_type type)
{
	ثابت حرف *ret;

	اگر (type == OBJ_NONE)
		type = odb_read_object_info(repo->objects, oid, تہی);

	ret = type_name(type);
	اگر (!ret)
		ret = _("unknown");

	واپس ret;
}

ساکن صحیح objerror(ساخت repository *repo, ساخت object *obj, ثابت حرف *err)
{
	errors_ملا |= ERROR_OBJECT;
	/* TRANSLATORS: e.g. error in tree 01bfda: <more explanation> */
	fprintf_ln(stderr, _("error in %s %s: %s"),
		   printable_type(repo, &obj->oid, obj->type),
		   describe_object(&obj->oid), err);
	واپس -1;
}

ساکن صحیح fsck_objects_error_func(ساخت fsck_options *o,
				   خالی *fsck_report,
				   شمار fsck_پیغام_type پیغام_type,
				   شمار fsck_پیغام_id پیغام_id_2 غیر_مستعمل,
				   ثابت حرف *پیغام_2)
{
	ساخت fsck_object_report *report = fsck_report;
	ثابت ساخت object_id *oid = report->oid;
	شمار object_type object_type = report->object_type;

	انتخاب (پیغام_type) {
	حالت FSCK_WARN:
		/* TRANSLATORS: e.g. warning in tree 01bfda: <more explanation> */
		fprintf_ln(stderr, _("warning in %s %s: %s"),
			   printable_type(o->repo, oid, object_type),
			   describe_object(oid), پیغام_2);
		واپس 0;
	حالت FSCK_ERROR:
		/* TRANSLATORS: e.g. error in tree 01bfda: <more explanation> */
		fprintf_ln(stderr, _("error in %s %s: %s"),
			   printable_type(o->repo, oid, object_type),
			   describe_object(oid), پیغام_2);
		واپس 1;
	طےشدہ:
		BUG("%d (FSCK_IGNORE?) should never trigger this callback",
		    پیغام_type);
	}
}

ساکن ساخت object_array pending;

ساکن صحیح mark_object(ساخت object *obj, شمار object_type type,
		       خالی *ڈیٹا, ساخت fsck_options *options)
{
	ساخت object *والد = ڈیٹا;

	/*
	 * The only case data is NULL or type is OBJ_ANY is when
	 * mark_object_reachable() calls us.  All the callers of
	 * that function has non-NULL obj hence ...
	 */
	اگر (!obj) {
		/* ... these references to parent->fld are safe here */
		printf_ln(_("broken link from %7s %s"),
			  printable_type(options->repo, &والد->oid, والد->type),
			  describe_object(&والد->oid));
		printf_ln(_("broken link from %7s %s"),
			  (type == OBJ_ANY ? _("unknown") : type_name(type)),
			  _("unknown"));
		errors_ملا |= ERROR_REACHABLE;
		واپس 1;
	}

	اگر (type != OBJ_ANY && obj->type != type)
		/* ... and the reference to parent is safe here */
		objerror(options->repo, والد, _("wrong object type in link"));

	اگر (obj->flags & REACHABLE)
		واپس 0;
	obj->flags |= REACHABLE;

	اگر (is_promisor_object(options->repo, &obj->oid))
		/*
		 * Further recursion does not need to be performed on this
		 * object since it is a promisor object (so it does not need to
		 * be added to "pending").
		 */
		واپس 0;

	اگر (!(obj->flags & HAS_OBJ)) {
		اگر (والد && !odb_has_object(options->repo->objects, &obj->oid,
					      ODB_HAS_OBJECT_RECHECK_PACKED)) {
			printf_ln(_("broken link from %7s %s\n"
				    "              to %7s %s"),
				  printable_type(options->repo, &والد->oid, والد->type),
				  describe_object(&والد->oid),
				  printable_type(options->repo, &obj->oid, obj->type),
				  describe_object(&obj->oid));
			errors_ملا |= ERROR_REACHABLE;
		}
		واپس 1;
	}

	جوڑو_object_array(obj, تہی, &pending);
	واپس 0;
}

ساکن خالی mark_object_reachable(ساخت object *obj)
{
	mark_object(obj, OBJ_ANY, تہی, &fsck_walk_options);
}

ساکن صحیح traverse_one_object(ساخت object *obj)
{
	صحیح نتیجہ = fsck_walk(obj, obj, &fsck_walk_options);

	اگر (obj->type == OBJ_درخت) {
		ساخت درخت *درخت = (ساخت درخت *)obj;
		free_درخت_بفر(درخت);
	}
	واپس نتیجہ;
}

ساکن صحیح traverse_reachable(ساخت repository *repo)
{
	ساخت پیش_رفت *پیش_رفت = تہی;
	غیرعلامتی صحیح nr = 0;
	صحیح نتیجہ = 0;
	اگر (show_پیش_رفت)
		پیش_رفت = آغاز_delayed_پیش_رفت(repo,
						  _("Checking connectivity"), 0);
	جب (pending.nr) {
		نتیجہ |= traverse_one_object(object_array_نکالو(&pending));
		display_پیش_رفت(پیش_رفت, ++nr);
	}
	stop_پیش_رفت(&پیش_رفت);
	واپس !!نتیجہ;
}

ساکن صحیح mark_مستعمل(ساخت object *obj, شمار object_type type غیر_مستعمل,
		     خالی *ڈیٹا غیر_مستعمل, ساخت fsck_options *options غیر_مستعمل)
{
	اگر (!obj)
		واپس 1;
	obj->flags |= مستعمل;
	واپس 0;
}

ساکن صحیح mark_unreachable_referents(ثابت ساخت object_id *oid,
				      ساخت object_info *oi غیر_مستعمل,
				      خالی *ڈیٹا)
{
	ساخت repository *repo = ڈیٹا;
	ساخت fsck_options options;
	ساخت object *obj = lookup_object(ڈیٹا, oid);

	اگر (!obj || !(obj->flags & HAS_OBJ))
		واپس 0; /* not part of our original set */
	اگر (obj->flags & REACHABLE)
		واپس 0; /* reachable objects already traversed */

	/*
	 * Avoid passing OBJ_NONE to fsck_walk, which will parse the object
	 * (and we want to avoid parsing blobs).
	 */
	اگر (obj->type == OBJ_NONE) {
		شمار object_type type = odb_read_object_info(repo->objects,
							     &obj->oid, تہی);
		اگر (type > 0)
			object_as_type(obj, type, 0);
	}

	fsck_options_init(&options, repo, FSCK_OPTIONS_DEFAULT);
	options.walk = mark_مستعمل;
	fsck_walk(obj, تہی, &options);
	اگر (obj->type == OBJ_درخت)
		free_درخت_بفر((ساخت درخت *)obj);

	واپس 0;
}

/*
 * Check a single reachable object
 */
ساکن خالی check_reachable_object(ساخت repository *repo, ساخت object *obj)
{
	/*
	 * We obviously want the object to be parsed,
	 * except if it was in a pack-file and we didn't
	 * do a full fsck
	 */
	اگر (!(obj->flags & HAS_OBJ)) {
		اگر (is_promisor_object(repo, &obj->oid))
			واپس;
		اگر (has_object_pack(repo, &obj->oid))
			واپس; /* it is in pack - forget about it */
		printf_ln(_("missing %s %s"),
			  printable_type(repo, &obj->oid, obj->type),
			  describe_object(&obj->oid));
		errors_ملا |= ERROR_REACHABLE;
		واپس;
	}
}

/*
 * Check a single unreachable object
 */
ساکن خالی check_unreachable_object(ساخت repository *repo, ساخت object *obj)
{
	/*
	 * Missing unreachable object? Ignore it. It's not like
	 * we miss it (since it can't be reached), nor do we want
	 * to complain about it being unreachable (since it does
	 * not exist).
	 */
	اگر (!(obj->flags & HAS_OBJ))
		واپس;

	/*
	 * Unreachable object that exists? Show it if asked to,
	 * since this is something that is prunable.
	 */
	اگر (show_unreachable) {
		printf_ln(_("unreachable %s %s"),
			  printable_type(repo, &obj->oid, obj->type),
			  describe_object(&obj->oid));
		واپس;
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
	اگر (!(obj->flags & مستعمل)) {
		اگر (show_dangling)
			printf_ln(_("dangling %s %s"),
				  printable_type(repo, &obj->oid, obj->type),
				  describe_object(&obj->oid));
		اگر (write_گمشدہ_and_ملا) {
			حرف *filename = repo_git_راستہ(repo, "lost-found/%s/%s",
				obj->type == OBJ_COMMIT ? "commit" : "other",
				describe_object(&obj->oid));
			FILE *f;

			اگر (محفوظ_create_leading_directories_const(repo, filename)) {
				error(_("could not create lost-found"));
				آزاد(filename);
				واپس;
			}
			f = xfopen(filename, "w");
			اگر (obj->type == OBJ_BLOB) {
				اگر (odb_دھارا_blob_مقصد_fd(repo->objects, fileno(f),
							  &obj->oid, تہی, 1))
					die_errno(_("could not write '%s'"), filename);
			} ورنہ
				fprintf(f, "%s\n", describe_object(&obj->oid));
			اگر (fclose(f))
				die_errno(_("could not finish '%s'"),
					  filename);
			آزاد(filename);
		}
		واپس;
	}

	/*
	 * Otherwise? It's there, it's unreachable, and some other unreachable
	 * object points to it. Ignore it - it's not interesting, and we showed
	 * all the interesting cases above.
	 */
}

ساکن خالی check_object(ساخت repository *repo, ساخت object *obj)
{
	اگر (verbose)
		fprintf_ln(stderr, _("Checking %s"), describe_object(&obj->oid));

	اگر (obj->flags & REACHABLE)
		check_reachable_object(repo, obj);
	ورنہ
		check_unreachable_object(repo, obj);
}

ساکن خالی check_connectivity(ساخت repository *repo)
{
	صحیح i, max;

	/* Traverse the pending reachable objects */
	traverse_reachable(repo);

	/*
	 * With --connectivity-only, we won't have actually opened and marked
	 * unreachable objects with USED. Do that now to make --dangling, etc
	 * accurate.
	 */
	اگر (connectivity_only && (show_dangling || write_گمشدہ_and_ملا)) {
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
		odb_for_each_object(repo->objects, تہی,
				    mark_unreachable_referents, repo, 0);
	}

	/* Look up all the requirements, warn about missing objects.. */
	max = get_max_object_اشاریہ(repo);
	اگر (verbose)
		fprintf_ln(stderr, _("Checking connectivity (%d objects)"), max);

	برائے (i = 0; i < max; i++) {
		ساخت object *obj = get_اشاریہ_دار_object(repo, i);

		اگر (obj)
			check_object(repo, obj);
	}
}

ساکن صحیح fsck_obj_بفر(ثابت ساخت object_id *oid, شمار object_type type,
			   غیرعلامتی طویل سائز, خالی *بفر, صحیح *eaten, خالی *cb_ڈیٹا)
{
	ساخت repository *repo = cb_ڈیٹا;
	ساخت object *obj;
	صحیح err;

	/*
	 * Note, buffer may be NULL if type is OBJ_BLOB. See
	 * verify_packfile(), data_valid variable for details.
	 */
	obj = parse_object_بفر(repo, oid, type, سائز, بفر, eaten);
	اگر (!obj) {
		errors_ملا |= ERROR_OBJECT;
		err = error(_("%s: object corrupt or missing"),
			    oid_مقصد_hex(oid));
		جاؤ برآمد;
	}

	obj->flags &= ~REACHABLE;
	obj->flags |= HAS_OBJ | نظر_آیا_2;

	اگر (verbose)
		fprintf_ln(stderr, _("Checking %s %s"),
			   printable_type(repo, &obj->oid, obj->type),
			   describe_object(&obj->oid));

	اگر (fsck_walk(obj, تہی, &fsck_obj_options))
		objerror(repo, obj, _("broken links"));

	err = fsck_object(obj, بفر, سائز, &fsck_obj_options);
	اگر (err)
		جاؤ برآمد;

	اگر (obj->type == OBJ_COMMIT) {
		ساخت commit *commit = (ساخت commit *) obj;

		اگر (!commit->parents && show_جڑ)
			printf_ln(_("root %s"),
				  describe_object(&commit->object.oid));
	}

	اگر (obj->type == OBJ_لیبل) {
		ساخت لیبل *لیبل = (ساخت لیبل *) obj;

		اگر (show_لیبل_فہرست && لیبل->tagged) {
			printf_ln(_("tagged %s %s (%s) in %s"),
				  printable_type(repo, &لیبل->tagged->oid, لیبل->tagged->type),
				  describe_object(&لیبل->tagged->oid),
				  لیبل->لیبل,
				  describe_object(&لیبل->object.oid));
		}
	}

برآمد:
	اگر (obj && obj->type == OBJ_درخت)
		free_درخت_بفر((ساخت درخت *)obj);
	واپس err;
}

ساکن صحیح default_refs;

ساکن خالی fsck_handle_reflog_oid(ساخت repository *repo,
				   ثابت حرف *refname, ساخت object_id *oid,
				   timestamp_t timestamp)
{
	ساخت object *obj;

	اگر (!is_null_oid(oid)) {
		obj = lookup_object(repo, oid);
		اگر (obj && (obj->flags & HAS_OBJ)) {
			اگر (timestamp)
				fsck_put_object_name(&fsck_walk_options, oid,
						     "%s@{%"PRItime"}",
						     refname, timestamp);
			obj->flags |= مستعمل;
			mark_object_reachable(obj);
		} ورنہ اگر (!is_promisor_object(repo, oid)) {
			error(_("%s: invalid reflog entry %s"),
			      refname, oid_مقصد_hex(oid));
			errors_ملا |= ERROR_REACHABLE;
		}
	}
}

ساکن صحیح fsck_handle_reflog_ent(ثابت حرف *refname,
				  ساخت object_id *ooid, ساخت object_id *noid,
				  ثابت حرف *email غیر_مستعمل,
				  timestamp_t timestamp, صحیح tz غیر_مستعمل,
				  ثابت حرف *پیغام_2 غیر_مستعمل, خالی *cb_ڈیٹا)
{
	ساخت repository *repo = cb_ڈیٹا;

	اگر (now && timestamp > now)
		واپس 0;

	اگر (verbose)
		fprintf_ln(stderr, _("Checking reflog %s->%s"),
			   oid_مقصد_hex(ooid), oid_مقصد_hex(noid));

	fsck_handle_reflog_oid(repo, refname, ooid, 0);
	fsck_handle_reflog_oid(repo, refname, noid, timestamp);
	واپس 0;
}

ساکن صحیح fsck_handle_reflog(ثابت حرف *logname, خالی *cb_ڈیٹا)
{
	ساخت strbuf refname = STRBUF_INIT;
	ساخت worktree *wt = cb_ڈیٹا;

	strbuf_worktree_ref(wt, &refname, logname);
	refs_for_each_reflog_ent(get_main_ref_store(wt->repo),
				 refname.buf, fsck_handle_reflog_ent,
				 wt->repo);
	strbuf_release(&refname);
	واپس 0;
}

ساخت ref_snapshot {
	حرف *refname;
	ساخت object_id oid;
	/* TODO: Maybe supplement with latest reflog entry info too? */
};

ساخت snapshot {
	سائز_t nr;
	سائز_t alloc;
	ساخت ref_snapshot *ref;
	/* TODO: Consider also snapshotting the index of each worktree. */
};

ساخت snapshot_ref_ڈیٹا {
	ساخت repository *repo;
	ساخت snapshot *snap;
};

ساکن صحیح snapshot_ref(ثابت ساخت reference *ref, خالی *cb_ڈیٹا)
{
	ساخت snapshot_ref_ڈیٹا *ڈیٹا = cb_ڈیٹا;
	ساخت snapshot *snap = ڈیٹا->snap;
	ساخت object *obj;

	obj = parse_object(ڈیٹا->repo, ref->oid);
	اگر (!obj) {
		اگر (is_promisor_object(ڈیٹا->repo, ref->oid)) {
			/*
			 * Increment default_refs anyway, because this is a
			 * valid ref.
			 */
			 default_refs++;
			 واپس 0;
		}
		error(_("%s: invalid sha1 pointer %s"),
		      ref->name, oid_مقصد_hex(ref->oid));
		errors_ملا |= ERROR_REACHABLE;
		/* We'll continue with the rest despite the error.. */
		واپس 0;
	}
	اگر (obj->type != OBJ_COMMIT && is_branch(ref->name)) {
		error(_("%s: not a commit"), ref->name);
		errors_ملا |= ERROR_REFS;
	}
	default_refs++;

	ALLOC_GROW(snap->ref, snap->nr + 1, snap->alloc);
	snap->ref[snap->nr].refname = xstrdup(ref->name);
	oidcpy(&snap->ref[snap->nr].oid, ref->oid);
	snap->nr++;

	واپس 0;
}

ساکن صحیح fsck_handle_ref(ثابت ساخت reference *ref, خالی *cb_ڈیٹا)
{
	ساخت repository *repo = cb_ڈیٹا;
	ساخت object *obj;

	obj = parse_object(repo, ref->oid);
	obj->flags |= مستعمل;
	fsck_put_object_name(&fsck_walk_options,
			     ref->oid, "%s", ref->name);
	mark_object_reachable(obj);

	واپس 0;
}

ساکن خالی snapshot_refs(ساخت repository *repo,
			  ساخت snapshot *snap, صحیح argc, ثابت حرف **argv)
{
	ساخت refs_for_each_ref_options opts = {
		.flags = REFS_FOR_EACH_INCLUDE_BROKEN,
	};
	ساخت snapshot_ref_ڈیٹا ڈیٹا = {
		.repo = repo,
		.snap = snap,
	};
	ساخت worktree **worktrees, **p;
	ثابت حرف *سر_points_at;
	ساخت object_id سر_oid;

	برائے (صحیح i = 0; i < argc; i++) {
		ثابت حرف *arg = argv[i];
		ساخت object_id oid;
		اگر (!repo_get_oid(repo, arg, &oid)) {
			ساخت reference ref = {
				.name = arg,
				.oid = &oid,
			};

			snapshot_ref(&ref, &ڈیٹا);
			جاری;
		}
		error(_("invalid parameter: expected sha1, got '%s'"), arg);
		errors_ملا |= ERROR_OBJECT;
	}

	اگر (argc) {
		include_reflogs = 0;
		واپس;
	}

	refs_for_each_ref_ext(get_main_ref_store(repo),
			      snapshot_ref, &ڈیٹا, &opts);

	worktrees = get_worktrees(repo);
	برائے (p = worktrees; *p; p++) {
		ساخت worktree *wt = *p;
		ساخت strbuf refname = STRBUF_INIT;

		strbuf_worktree_ref(wt, &refname, "HEAD");

		سر_points_at = refs_resolve_ref_unsafe(get_main_ref_store(repo),
							 refname.buf, 0, &سر_oid, تہی);

		اگر (سر_points_at && !is_null_oid(&سر_oid)) {
			ساخت reference ref = {
				.name = refname.buf,
				.oid = &سر_oid,
			};

			snapshot_ref(&ref, &ڈیٹا);
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
	now = وقت(تہی);
}


ساکن خالی free_snapshot_refs(ساخت snapshot *snap)
{
	برائے (سائز_t i = 0; i < snap->nr; i++)
		آزاد(snap->ref[i].refname);
	آزاد(snap->ref);
}

ساکن خالی process_refs(ساخت repository *repo, ساخت snapshot *snap)
{
	ساخت worktree **worktrees, **p;

	برائے (سائز_t i = 0; i < snap->nr; i++) {
		ساخت reference ref = {
			.name = snap->ref[i].refname,
			.oid = &snap->ref[i].oid,
		};
		fsck_handle_ref(&ref, repo);
	}

	اگر (include_reflogs) {
		worktrees = get_worktrees(repo);
		برائے (p = worktrees; *p; p++) {
			ساخت worktree *wt = *p;

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

ساکن صحیح fsck_cache_درخت(ساخت repository *repo, ساخت cache_درخت *it, ثابت حرف *اشاریہ_راستہ)
{
	صحیح i;
	صحیح err = 0;

	اگر (verbose)
		fprintf_ln(stderr, _("Checking cache tree of %s"), اشاریہ_راستہ);

	اگر (0 <= it->اندراج_گنتی) {
		ساخت object *obj = parse_object(repo, &it->oid);
		اگر (!obj) {
			error(_("%s: invalid sha1 pointer in cache-tree of %s"),
			      oid_مقصد_hex(&it->oid), اشاریہ_راستہ);
			errors_ملا |= ERROR_REFS;
			واپس 1;
		}
		obj->flags |= مستعمل;
		fsck_put_object_name(&fsck_walk_options, &it->oid, ":");
		mark_object_reachable(obj);
		اگر (obj->type != OBJ_درخت)
			err |= objerror(repo, obj, _("non-tree in cache-tree"));
	}
	برائے (i = 0; i < it->subtree_nr; i++)
		err |= fsck_cache_درخت(repo, it->نیچے_کو[i]->cache_درخت, اشاریہ_راستہ);
	واپس err;
}

ساکن صحیح fsck_resolve_undo(ساخت اشاریہ_state *istate,
			     ثابت حرف *اشاریہ_راستہ)
{
	ساخت string_list_شے *شے;
	ساخت string_list *resolve_undo = istate->resolve_undo;

	اگر (!resolve_undo)
		واپس 0;

	for_each_string_list_شے(شے, resolve_undo) {
		ثابت حرف *راستہ = شے->string;
		ساخت resolve_undo_info *ru = شے->util;
		صحیح i;

		اگر (!ru)
			جاری;
		برائے (i = 0; i < 3; i++) {
			ساخت object *obj;

			اگر (!ru->منوال[i] || !S_ISREG(ru->منوال[i]))
				جاری;

			obj = parse_object(istate->repo, &ru->oid[i]);
			اگر (!obj) {
				error(_("%s: invalid sha1 pointer in resolve-undo of %s"),
				      oid_مقصد_hex(&ru->oid[i]),
				      اشاریہ_راستہ);
				errors_ملا |= ERROR_REFS;
				جاری;
			}
			obj->flags |= مستعمل;
			fsck_put_object_name(&fsck_walk_options, &ru->oid[i],
					     ":(%d):%s", i, راستہ);
			mark_object_reachable(obj);
		}
	}
	واپس 0;
}

ساکن خالی fsck_اشاریہ(ساخت اشاریہ_state *istate, ثابت حرف *اشاریہ_راستہ,
		       صحیح is_current_worktree)
{
	غیرعلامتی صحیح i;

	/* TODO: audit for interaction with sparse-index. */
	ensure_بھرا_اشاریہ(istate);
	برائے (i = 0; i < istate->cache_nr; i++) {
		غیرعلامتی صحیح منوال;
		ساخت blob *blob;
		ساخت object *obj;

		منوال = istate->cache[i]->ce_منوال;
		اگر (S_ISGITLINK(منوال))
			جاری;
		blob = lookup_blob(istate->repo,
				   &istate->cache[i]->oid);
		اگر (!blob)
			جاری;
		obj = &blob->object;
		obj->flags |= مستعمل;
		fsck_put_object_name(&fsck_walk_options, &obj->oid,
				     "%s:%s",
				     is_current_worktree ? "" : اشاریہ_راستہ,
				     istate->cache[i]->name);
		mark_object_reachable(obj);
	}
	اگر (istate->cache_درخت)
		fsck_cache_درخت(istate->repo, istate->cache_درخت, اشاریہ_راستہ);
	fsck_resolve_undo(istate, اشاریہ_راستہ);
}

ساکن صحیح mark_object_for_connectivity(ثابت ساخت object_id *oid,
					ساخت object_info *oi غیر_مستعمل,
					خالی *cb_ڈیٹا)
{
	ساخت repository *repo = cb_ڈیٹا;
	ساخت object *obj = lookup_unknown_object(repo, oid);
	obj->flags |= HAS_OBJ;
	واپس 0;
}

ساکن خالی fsck_refs(ساخت repository *r)
{
	ساخت بچہ_process refs_verify = بچہ_PROCESS_INIT;
	ساخت پیش_رفت *پیش_رفت = تہی;

	اگر (show_پیش_رفت)
		پیش_رفت = آغاز_پیش_رفت(r, _("Checking ref database"), 1);

	اگر (verbose)
		fprintf_ln(stderr, _("Checking ref database"));

	بچہ_process_init(&refs_verify);
	refs_verify.git_cmd = 1;
	strvec_pushl(&refs_verify.دلائل, "refs", "verify", تہی);
	اگر (verbose)
		strvec_ڈالو(&refs_verify.دلائل, "--verbose");
	اگر (check_strict)
		strvec_ڈالو(&refs_verify.دلائل, "--strict");

	اگر (تسلسل_command(&refs_verify))
		errors_ملا |= ERROR_REFS;

	display_پیش_رفت(پیش_رفت, 1);
	stop_پیش_رفت(&پیش_رفت);
}

ساکن حرف ثابت * ثابت fsck_usage[] = {
	N_("git fsck [--tags] [--root] [--unreachable] [--cache] [--no-reflogs]\n"
	   "         [--[no-]full] [--strict] [--verbose] [--lost-found]\n"
	   "         [--[no-]dangling] [--[no-]progress] [--connectivity-only]\n"
	   "         [--[no-]name-objects] [--[no-]references] [<object>...]"),
	تہی
};

صحیح cmd_fsck(صحیح argc,
	     ثابت حرف **argv,
	     ثابت حرف *سابقہ,
	     ساخت repository *repo)
{
	ساخت odb_fsck_options odb_fsck_opts = {
		.flags = ODB_FSCK_بھرا,
		.object_cb = fsck_obj_بفر,
		.object_payload = repo,
	};
	صحیح keep_cache_objects = 0;
	صحیح name_objects = 0;
	صحیح check_references = 1;
	ساخت option fsck_opts[] = {
		OPT__VERBOSE(&verbose, N_("be verbose")),
		OPT_BOOL(0, "unreachable", &show_unreachable, N_("show unreachable objects")),
		OPT_BOOL(0, "dangling", &show_dangling, N_("show dangling objects")),
		OPT_BOOL(0, "tags", &show_لیبل_فہرست, N_("report tags")),
		OPT_BOOL(0, "root", &show_جڑ, N_("report root nodes")),
		OPT_BOOL(0, "cache", &keep_cache_objects, N_("make index objects head nodes")),
		OPT_BOOL(0, "reflogs", &include_reflogs, N_("make reflogs head nodes (default)")),
		OPT_ثنائی_ہندسہ(0, "full", &odb_fsck_opts.flags,
			N_("also consider packs and alternate objects"), ODB_FSCK_بھرا),
		OPT_BOOL(0, "connectivity-only", &connectivity_only, N_("check only connectivity")),
		OPT_BOOL(0, "strict", &check_strict, N_("enable more strict checking")),
		OPT_BOOL(0, "lost-found", &write_گمشدہ_and_ملا,
					N_("write dangling objects in .git/lost-found")),
		OPT_BOOL(0, "progress", &show_پیش_رفت, N_("show progress")),
		OPT_BOOL(0, "name-objects", &name_objects, N_("show verbose names for reachable objects")),
		OPT_BOOL(0, "references", &check_references, N_("check reference database consistency")),
		OPT_انتہا(),
	};
	ساخت snapshot snap = {
		.nr = 0,
		.alloc = 0,
		.ref = تہی
	};

	errors_ملا = 0;
	disable_replace_refs();
	save_commit_بفر = 0;

	argc = parse_options(argc, argv, سابقہ, fsck_opts, fsck_usage, 0);

	/* fsck knows how to handle missing promisor objects */
	repo->fetch_if_missing = 0;

	fsck_options_init(&fsck_walk_options, repo, FSCK_OPTIONS_DEFAULT);
	fsck_walk_options.walk = mark_object;

	fsck_options_init(&fsck_obj_options, repo, FSCK_OPTIONS_DEFAULT);
	fsck_obj_options.walk = mark_مستعمل;
	fsck_obj_options.error_func = fsck_objects_error_func;
	اگر (check_strict)
		fsck_obj_options.strict = 1;

	اگر (show_پیش_رفت == -1)
		show_پیش_رفت = isatty(2);
	اگر (verbose) {
		show_پیش_رفت = 0;
		odb_fsck_opts.flags |= ODB_FSCK_VERBOSE;
	}
	اگر (show_پیش_رفت)
		odb_fsck_opts.flags |= ODB_FSCK_پیش_رفت;

	اگر (write_گمشدہ_and_ملا) {
		odb_fsck_opts.flags |= ODB_FSCK_بھرا;
		include_reflogs = 0;
	}

	اگر (name_objects)
		fsck_enable_object_نام(&fsck_walk_options);

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
		odb_for_each_object(repo->objects, تہی,
				    mark_object_for_connectivity, repo, 0);
	} ورنہ {
		اگر (odb_fsck(repo->objects, &odb_fsck_opts) < 0)
			errors_ملا |= ERROR_OBJECT;

		اگر (fsck_اختتام(&fsck_obj_options))
			errors_ملا |= ERROR_OBJECT;
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
		ساخت worktree **worktrees, **p;

		verify_اشاریہ_checksum = 1;
		verify_ce_ترتیب = 1;

		worktrees = get_worktrees(repo);
		برائے (p = worktrees; *p; p++) {
			ساخت worktree *wt = *p;
			ساخت اشاریہ_state istate =
				اشاریہ_STATE_INIT(repo);
			حرف *راستہ, *wt_gitdir;

			/*
			 * Make a copy since the buffer is reusable
			 * and may get overwritten by other calls
			 * while we're examining the index.
			 */
			راستہ = xstrdup(worktree_git_راستہ(wt, "index"));
			wt_gitdir = get_worktree_git_dir(wt);

			read_اشاریہ_from(&istate, راستہ, wt_gitdir);
			fsck_اشاریہ(&istate, راستہ, wt->is_current);

			discard_اشاریہ(&istate);
			آزاد(wt_gitdir);
			آزاد(راستہ);
		}
		free_worktrees(worktrees);
	}

	check_connectivity(repo);

	اگر (repo->settings.core_commit_گراف) {
		ساخت odb_ماخذ_files *files = odb_ماخذ_files_downcast(repo->objects->ماخذ);
		ساخت بچہ_process commit_گراف_verify = بچہ_PROCESS_INIT;

		برائے (ساخت odb_files_dir *dir = files->dirs; dir; dir = dir->اگلا) {
			بچہ_process_init(&commit_گراف_verify);
			commit_گراف_verify.git_cmd = 1;
			strvec_pushl(&commit_گراف_verify.دلائل, "commit-graph",
				     "verify", "--object-dir", dir->abspath, تہی);
			اگر (show_پیش_رفت)
				strvec_ڈالو(&commit_گراف_verify.دلائل, "--progress");
			ورنہ
				strvec_ڈالو(&commit_گراف_verify.دلائل, "--no-progress");
			اگر (تسلسل_command(&commit_گراف_verify))
				errors_ملا |= ERROR_COMMIT_گراف;
		}
	}

	free_snapshot_refs(&snap);
	واپس errors_ملا;
}
