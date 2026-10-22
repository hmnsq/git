#समावेश "builtin.h"
#समावेश "gettext.h"
#समावेश "hex.h"
#समावेश "config.h"
#समावेश "commit.h"
#समावेश "tree.h"
#समावेश "blob.h"
#समावेश "tag.h"
#समावेश "refs.h"
#समावेश "cache-tree.h"
#समावेश "fsck.h"
#समावेश "parse-options.h"
#समावेश "progress.h"
#समावेश "packfile.h"
#समावेश "object-name.h"
#समावेश "odb.h"
#समावेश "odb/streaming.h"
#समावेश "path.h"
#समावेश "read-cache-ll.h"
#समावेश "replace-object.h"
#समावेश "resolve-undo.h"
#समावेश "run-command.h"
#समावेश "sparse-index.h"
#समावेश "worktree.h"

#घोषणा REACHABLE 0x0001
#घोषणा SEEN      0x0002
#घोषणा HAS_OBJ   0x0004
/* This flag is set if something points to this object. */
#घोषणा USED      0x0008

अटल पूर्णांक show_मूल;
अटल पूर्णांक show_tags;
अटल पूर्णांक show_unreachable;
अटल पूर्णांक include_reflogs = 1;
अटल पूर्णांक connectivity_only;
अटल पूर्णांक check_strict;
अटल काष्ठा fsck_options fsck_walk_options;
अटल काष्ठा fsck_options fsck_obj_options;
अटल पूर्णांक errors_found;
अटल पूर्णांक write_lost_and_found;
अटल पूर्णांक verbose;
अटल पूर्णांक show_progress = -1;
अटल पूर्णांक show_dangling = 1;
अटल timestamp_t now;
#घोषणा ERROR_OBJECT 01
#घोषणा ERROR_REACHABLE 02
#घोषणा ERROR_REFS 010
#घोषणा ERROR_COMMIT_GRAPH 020

अटल स्थिर अक्षर *describe_object(स्थिर काष्ठा object_id *oid)
{
	वापस fsck_describe_object(&fsck_walk_options, oid);
}

अटल स्थिर अक्षर *printable_type(काष्ठा repository *repo,
				  स्थिर काष्ठा object_id *oid,
				  क्रमागत object_type type)
{
	स्थिर अक्षर *ret;

	अगर (type == OBJ_NONE)
		type = odb_read_object_info(repo->objects, oid, शून्य);

	ret = type_name(type);
	अगर (!ret)
		ret = _("unknown");

	वापस ret;
}

अटल पूर्णांक objerror(काष्ठा repository *repo, काष्ठा object *obj, स्थिर अक्षर *err)
{
	errors_found |= ERROR_OBJECT;
	/* TRANSLATORS: e.g. error in tree 01bfda: <more explanation> */
	fprintf_ln(मानक_त्रुटि, _("error in %s %s: %s"),
		   printable_type(repo, &obj->oid, obj->type),
		   describe_object(&obj->oid), err);
	वापस -1;
}

अटल पूर्णांक fsck_objects_error_func(काष्ठा fsck_options *o,
				   व्योम *fsck_report,
				   क्रमागत fsck_संदेश_type संदेश_type,
				   क्रमागत fsck_संदेश_id संदेश_id UNUSED,
				   स्थिर अक्षर *message)
{
	काष्ठा fsck_object_report *report = fsck_report;
	स्थिर काष्ठा object_id *oid = report->oid;
	क्रमागत object_type object_type = report->object_type;

	चयन (संदेश_type) {
	हाल FSCK_WARN:
		/* TRANSLATORS: e.g. warning in tree 01bfda: <more explanation> */
		fprintf_ln(मानक_त्रुटि, _("warning in %s %s: %s"),
			   printable_type(o->repo, oid, object_type),
			   describe_object(oid), message);
		वापस 0;
	हाल FSCK_ERROR:
		/* TRANSLATORS: e.g. error in tree 01bfda: <more explanation> */
		fprintf_ln(मानक_त्रुटि, _("error in %s %s: %s"),
			   printable_type(o->repo, oid, object_type),
			   describe_object(oid), message);
		वापस 1;
	शेष:
		BUG("%d (FSCK_IGNORE?) should never trigger this callback",
		    संदेश_type);
	}
}

अटल काष्ठा object_array pending;

अटल पूर्णांक mark_object(काष्ठा object *obj, क्रमागत object_type type,
		       व्योम *तथ्य, काष्ठा fsck_options *options)
{
	काष्ठा object *parent = तथ्य;

	/*
	 * The only case data is NULL or type is OBJ_ANY is when
	 * mark_object_reachable() calls us.  All the callers of
	 * that function has non-NULL obj hence ...
	 */
	अगर (!obj) {
		/* ... these references to parent->fld are safe here */
		printf_ln(_("broken link from %7s %s"),
			  printable_type(options->repo, &parent->oid, parent->type),
			  describe_object(&parent->oid));
		printf_ln(_("broken link from %7s %s"),
			  (type == OBJ_ANY ? _("unknown") : type_name(type)),
			  _("unknown"));
		errors_found |= ERROR_REACHABLE;
		वापस 1;
	}

	अगर (type != OBJ_ANY && obj->type != type)
		/* ... and the reference to parent is safe here */
		objerror(options->repo, parent, _("wrong object type in link"));

	अगर (obj->flags & REACHABLE)
		वापस 0;
	obj->flags |= REACHABLE;

	अगर (is_promisor_object(options->repo, &obj->oid))
		/*
		 * Further recursion does not need to be performed on this
		 * object since it is a promisor object (so it does not need to
		 * be added to "pending").
		 */
		वापस 0;

	अगर (!(obj->flags & HAS_OBJ)) {
		अगर (parent && !odb_has_object(options->repo->objects, &obj->oid,
					      ODB_HAS_OBJECT_RECHECK_PACKED)) {
			printf_ln(_("broken link from %7s %s\n"
				    "              to %7s %s"),
				  printable_type(options->repo, &parent->oid, parent->type),
				  describe_object(&parent->oid),
				  printable_type(options->repo, &obj->oid, obj->type),
				  describe_object(&obj->oid));
			errors_found |= ERROR_REACHABLE;
		}
		वापस 1;
	}

	add_object_array(obj, शून्य, &pending);
	वापस 0;
}

अटल व्योम mark_object_reachable(काष्ठा object *obj)
{
	mark_object(obj, OBJ_ANY, शून्य, &fsck_walk_options);
}

अटल पूर्णांक traverse_one_object(काष्ठा object *obj)
{
	पूर्णांक परिणाम = fsck_walk(obj, obj, &fsck_walk_options);

	अगर (obj->type == OBJ_वृक्ष) {
		काष्ठा वृक्ष *वृक्ष = (काष्ठा वृक्ष *)obj;
		free_वृक्ष_बफर(वृक्ष);
	}
	वापस परिणाम;
}

अटल पूर्णांक traverse_reachable(काष्ठा repository *repo)
{
	काष्ठा progress *progress = शून्य;
	अचिन्हित पूर्णांक nr = 0;
	पूर्णांक परिणाम = 0;
	अगर (show_progress)
		progress = start_delayed_progress(repo,
						  _("Checking connectivity"), 0);
	जबतक (pending.nr) {
		परिणाम |= traverse_one_object(object_array_निकालो(&pending));
		display_progress(progress, ++nr);
	}
	stop_progress(&progress);
	वापस !!परिणाम;
}

अटल पूर्णांक mark_used(काष्ठा object *obj, क्रमागत object_type type UNUSED,
		     व्योम *तथ्य UNUSED, काष्ठा fsck_options *options UNUSED)
{
	अगर (!obj)
		वापस 1;
	obj->flags |= USED;
	वापस 0;
}

अटल पूर्णांक mark_unreachable_referents(स्थिर काष्ठा object_id *oid,
				      काष्ठा object_info *oi UNUSED,
				      व्योम *तथ्य)
{
	काष्ठा repository *repo = तथ्य;
	काष्ठा fsck_options options;
	काष्ठा object *obj = lookup_object(तथ्य, oid);

	अगर (!obj || !(obj->flags & HAS_OBJ))
		वापस 0; /* not part of our original set */
	अगर (obj->flags & REACHABLE)
		वापस 0; /* reachable objects already traversed */

	/*
	 * Avoid passing OBJ_NONE to fsck_walk, which will parse the object
	 * (and we want to avoid parsing blobs).
	 */
	अगर (obj->type == OBJ_NONE) {
		क्रमागत object_type type = odb_read_object_info(repo->objects,
							     &obj->oid, शून्य);
		अगर (type > 0)
			object_as_type(obj, type, 0);
	}

	fsck_options_init(&options, repo, FSCK_OPTIONS_DEFAULT);
	options.walk = mark_used;
	fsck_walk(obj, शून्य, &options);
	अगर (obj->type == OBJ_वृक्ष)
		free_वृक्ष_बफर((काष्ठा वृक्ष *)obj);

	वापस 0;
}

/*
 * Check a single reachable object
 */
अटल व्योम check_reachable_object(काष्ठा repository *repo, काष्ठा object *obj)
{
	/*
	 * We obviously want the object to be parsed,
	 * except if it was in a pack-file and we didn't
	 * do a full fsck
	 */
	अगर (!(obj->flags & HAS_OBJ)) {
		अगर (is_promisor_object(repo, &obj->oid))
			वापस;
		अगर (has_object_pack(repo, &obj->oid))
			वापस; /* it is in pack - forget about it */
		printf_ln(_("missing %s %s"),
			  printable_type(repo, &obj->oid, obj->type),
			  describe_object(&obj->oid));
		errors_found |= ERROR_REACHABLE;
		वापस;
	}
}

/*
 * Check a single unreachable object
 */
अटल व्योम check_unreachable_object(काष्ठा repository *repo, काष्ठा object *obj)
{
	/*
	 * Missing unreachable object? Ignore it. It's not like
	 * we miss it (since it can't be reached), nor do we want
	 * to complain about it being unreachable (since it does
	 * not exist).
	 */
	अगर (!(obj->flags & HAS_OBJ))
		वापस;

	/*
	 * Unreachable object that exists? Show it if asked to,
	 * since this is something that is prunable.
	 */
	अगर (show_unreachable) {
		printf_ln(_("unreachable %s %s"),
			  printable_type(repo, &obj->oid, obj->type),
			  describe_object(&obj->oid));
		वापस;
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
	अगर (!(obj->flags & USED)) {
		अगर (show_dangling)
			printf_ln(_("dangling %s %s"),
				  printable_type(repo, &obj->oid, obj->type),
				  describe_object(&obj->oid));
		अगर (write_lost_and_found) {
			अक्षर *filename = repo_git_path(repo, "lost-found/%s/%s",
				obj->type == OBJ_COMMIT ? "commit" : "other",
				describe_object(&obj->oid));
			खाता *f;

			अगर (safe_create_leading_directories_const(repo, filename)) {
				error(_("could not create lost-found"));
				मुक्त(filename);
				वापस;
			}
			f = xfopen(filename, "w");
			अगर (obj->type == OBJ_BLOB) {
				अगर (odb_stream_blob_to_fd(repo->objects, fileno(f),
							  &obj->oid, शून्य, 1))
					die_errno(_("could not write '%s'"), filename);
			} अन्यथा
				ख_लिखो(f, "%s\n", describe_object(&obj->oid));
			अगर (ख_बंद(f))
				die_errno(_("could not finish '%s'"),
					  filename);
			मुक्त(filename);
		}
		वापस;
	}

	/*
	 * Otherwise? It's there, it's unreachable, and some other unreachable
	 * object points to it. Ignore it - it's not interesting, and we showed
	 * all the interesting cases above.
	 */
}

अटल व्योम check_object(काष्ठा repository *repo, काष्ठा object *obj)
{
	अगर (verbose)
		fprintf_ln(मानक_त्रुटि, _("Checking %s"), describe_object(&obj->oid));

	अगर (obj->flags & REACHABLE)
		check_reachable_object(repo, obj);
	अन्यथा
		check_unreachable_object(repo, obj);
}

अटल व्योम check_connectivity(काष्ठा repository *repo)
{
	पूर्णांक i, max;

	/* Traverse the pending reachable objects */
	traverse_reachable(repo);

	/*
	 * With --connectivity-only, we won't have actually opened and marked
	 * unreachable objects with USED. Do that now to make --dangling, etc
	 * accurate.
	 */
	अगर (connectivity_only && (show_dangling || write_lost_and_found)) {
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
		odb_for_each_object(repo->objects, शून्य,
				    mark_unreachable_referents, repo, 0);
	}

	/* Look up all the requirements, warn about missing objects.. */
	max = get_max_object_सूचकांक(repo);
	अगर (verbose)
		fprintf_ln(मानक_त्रुटि, _("Checking connectivity (%d objects)"), max);

	क्रम (i = 0; i < max; i++) {
		काष्ठा object *obj = get_indexed_object(repo, i);

		अगर (obj)
			check_object(repo, obj);
	}
}

अटल पूर्णांक fsck_obj_बफर(स्थिर काष्ठा object_id *oid, क्रमागत object_type type,
			   अचिन्हित दीर्घ आकार, व्योम *बफर, पूर्णांक *eaten, व्योम *cb_तथ्य)
{
	काष्ठा repository *repo = cb_तथ्य;
	काष्ठा object *obj;
	पूर्णांक err;

	/*
	 * Note, buffer may be NULL if type is OBJ_BLOB. See
	 * verify_packfile(), data_valid variable for details.
	 */
	obj = parse_object_बफर(repo, oid, type, आकार, बफर, eaten);
	अगर (!obj) {
		errors_found |= ERROR_OBJECT;
		err = error(_("%s: object corrupt or missing"),
			    oid_to_hex(oid));
		जाओ out;
	}

	obj->flags &= ~REACHABLE;
	obj->flags |= HAS_OBJ | SEEN;

	अगर (verbose)
		fprintf_ln(मानक_त्रुटि, _("Checking %s %s"),
			   printable_type(repo, &obj->oid, obj->type),
			   describe_object(&obj->oid));

	अगर (fsck_walk(obj, शून्य, &fsck_obj_options))
		objerror(repo, obj, _("broken links"));

	err = fsck_object(obj, बफर, आकार, &fsck_obj_options);
	अगर (err)
		जाओ out;

	अगर (obj->type == OBJ_COMMIT) {
		काष्ठा commit *commit = (काष्ठा commit *) obj;

		अगर (!commit->parents && show_मूल)
			printf_ln(_("root %s"),
				  describe_object(&commit->object.oid));
	}

	अगर (obj->type == OBJ_TAG) {
		काष्ठा tag *tag = (काष्ठा tag *) obj;

		अगर (show_tags && tag->tagged) {
			printf_ln(_("tagged %s %s (%s) in %s"),
				  printable_type(repo, &tag->tagged->oid, tag->tagged->type),
				  describe_object(&tag->tagged->oid),
				  tag->tag,
				  describe_object(&tag->object.oid));
		}
	}

out:
	अगर (obj && obj->type == OBJ_वृक्ष)
		free_वृक्ष_बफर((काष्ठा वृक्ष *)obj);
	वापस err;
}

अटल पूर्णांक default_refs;

अटल व्योम fsck_handle_reflog_oid(काष्ठा repository *repo,
				   स्थिर अक्षर *refname, काष्ठा object_id *oid,
				   timestamp_t timestamp)
{
	काष्ठा object *obj;

	अगर (!is_null_oid(oid)) {
		obj = lookup_object(repo, oid);
		अगर (obj && (obj->flags & HAS_OBJ)) {
			अगर (timestamp)
				fsck_put_object_name(&fsck_walk_options, oid,
						     "%s@{%"PRItime"}",
						     refname, timestamp);
			obj->flags |= USED;
			mark_object_reachable(obj);
		} अन्यथा अगर (!is_promisor_object(repo, oid)) {
			error(_("%s: invalid reflog entry %s"),
			      refname, oid_to_hex(oid));
			errors_found |= ERROR_REACHABLE;
		}
	}
}

अटल पूर्णांक fsck_handle_reflog_ent(स्थिर अक्षर *refname,
				  काष्ठा object_id *ooid, काष्ठा object_id *noid,
				  स्थिर अक्षर *email UNUSED,
				  timestamp_t timestamp, पूर्णांक tz UNUSED,
				  स्थिर अक्षर *message UNUSED, व्योम *cb_तथ्य)
{
	काष्ठा repository *repo = cb_तथ्य;

	अगर (now && timestamp > now)
		वापस 0;

	अगर (verbose)
		fprintf_ln(मानक_त्रुटि, _("Checking reflog %s->%s"),
			   oid_to_hex(ooid), oid_to_hex(noid));

	fsck_handle_reflog_oid(repo, refname, ooid, 0);
	fsck_handle_reflog_oid(repo, refname, noid, timestamp);
	वापस 0;
}

अटल पूर्णांक fsck_handle_reflog(स्थिर अक्षर *logname, व्योम *cb_तथ्य)
{
	काष्ठा strbuf refname = STRBUF_INIT;
	काष्ठा worktree *wt = cb_तथ्य;

	strbuf_worktree_ref(wt, &refname, logname);
	refs_for_each_reflog_ent(get_main_ref_store(wt->repo),
				 refname.buf, fsck_handle_reflog_ent,
				 wt->repo);
	strbuf_release(&refname);
	वापस 0;
}

काष्ठा ref_snapshot {
	अक्षर *refname;
	काष्ठा object_id oid;
	/* TODO: Maybe supplement with latest reflog entry info too? */
};

काष्ठा snapshot {
	माप_प्रकार nr;
	माप_प्रकार alloc;
	काष्ठा ref_snapshot *ref;
	/* TODO: Consider also snapshotting the index of each worktree. */
};

काष्ठा snapshot_ref_तथ्य {
	काष्ठा repository *repo;
	काष्ठा snapshot *snap;
};

अटल पूर्णांक snapshot_ref(स्थिर काष्ठा reference *ref, व्योम *cb_तथ्य)
{
	काष्ठा snapshot_ref_तथ्य *तथ्य = cb_तथ्य;
	काष्ठा snapshot *snap = तथ्य->snap;
	काष्ठा object *obj;

	obj = parse_object(तथ्य->repo, ref->oid);
	अगर (!obj) {
		अगर (is_promisor_object(तथ्य->repo, ref->oid)) {
			/*
			 * Increment default_refs anyway, because this is a
			 * valid ref.
			 */
			 default_refs++;
			 वापस 0;
		}
		error(_("%s: invalid sha1 pointer %s"),
		      ref->name, oid_to_hex(ref->oid));
		errors_found |= ERROR_REACHABLE;
		/* We'll continue with the rest despite the error.. */
		वापस 0;
	}
	अगर (obj->type != OBJ_COMMIT && is_branch(ref->name)) {
		error(_("%s: not a commit"), ref->name);
		errors_found |= ERROR_REFS;
	}
	default_refs++;

	ALLOC_GROW(snap->ref, snap->nr + 1, snap->alloc);
	snap->ref[snap->nr].refname = xstrdup(ref->name);
	oidcpy(&snap->ref[snap->nr].oid, ref->oid);
	snap->nr++;

	वापस 0;
}

अटल पूर्णांक fsck_handle_ref(स्थिर काष्ठा reference *ref, व्योम *cb_तथ्य)
{
	काष्ठा repository *repo = cb_तथ्य;
	काष्ठा object *obj;

	obj = parse_object(repo, ref->oid);
	obj->flags |= USED;
	fsck_put_object_name(&fsck_walk_options,
			     ref->oid, "%s", ref->name);
	mark_object_reachable(obj);

	वापस 0;
}

अटल व्योम snapshot_refs(काष्ठा repository *repo,
			  काष्ठा snapshot *snap, पूर्णांक argc, स्थिर अक्षर **argv)
{
	काष्ठा refs_for_each_ref_options opts = {
		.flags = REFS_FOR_EACH_INCLUDE_BROKEN,
	};
	काष्ठा snapshot_ref_तथ्य तथ्य = {
		.repo = repo,
		.snap = snap,
	};
	काष्ठा worktree **worktrees, **p;
	स्थिर अक्षर *शीर्ष_points_at;
	काष्ठा object_id शीर्ष_oid;

	क्रम (पूर्णांक i = 0; i < argc; i++) {
		स्थिर अक्षर *arg = argv[i];
		काष्ठा object_id oid;
		अगर (!repo_get_oid(repo, arg, &oid)) {
			काष्ठा reference ref = {
				.name = arg,
				.oid = &oid,
			};

			snapshot_ref(&ref, &तथ्य);
			जारी;
		}
		error(_("invalid parameter: expected sha1, got '%s'"), arg);
		errors_found |= ERROR_OBJECT;
	}

	अगर (argc) {
		include_reflogs = 0;
		वापस;
	}

	refs_for_each_ref_ext(get_main_ref_store(repo),
			      snapshot_ref, &तथ्य, &opts);

	worktrees = get_worktrees(repo);
	क्रम (p = worktrees; *p; p++) {
		काष्ठा worktree *wt = *p;
		काष्ठा strbuf refname = STRBUF_INIT;

		strbuf_worktree_ref(wt, &refname, "HEAD");

		शीर्ष_points_at = refs_resolve_ref_unsafe(get_main_ref_store(repo),
							 refname.buf, 0, &शीर्ष_oid, शून्य);

		अगर (शीर्ष_points_at && !is_null_oid(&शीर्ष_oid)) {
			काष्ठा reference ref = {
				.name = refname.buf,
				.oid = &शीर्ष_oid,
			};

			snapshot_ref(&ref, &तथ्य);
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
	now = समय(शून्य);
}


अटल व्योम free_snapshot_refs(काष्ठा snapshot *snap)
{
	क्रम (माप_प्रकार i = 0; i < snap->nr; i++)
		मुक्त(snap->ref[i].refname);
	मुक्त(snap->ref);
}

अटल व्योम process_refs(काष्ठा repository *repo, काष्ठा snapshot *snap)
{
	काष्ठा worktree **worktrees, **p;

	क्रम (माप_प्रकार i = 0; i < snap->nr; i++) {
		काष्ठा reference ref = {
			.name = snap->ref[i].refname,
			.oid = &snap->ref[i].oid,
		};
		fsck_handle_ref(&ref, repo);
	}

	अगर (include_reflogs) {
		worktrees = get_worktrees(repo);
		क्रम (p = worktrees; *p; p++) {
			काष्ठा worktree *wt = *p;

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
	अगर (!default_refs) {
		fprintf_ln(मानक_त्रुटि, _("notice: No default references"));
		show_unreachable = 0;
	}
}

अटल पूर्णांक fsck_cache_वृक्ष(काष्ठा repository *repo, काष्ठा cache_वृक्ष *it, स्थिर अक्षर *सूचकांक_path)
{
	पूर्णांक i;
	पूर्णांक err = 0;

	अगर (verbose)
		fprintf_ln(मानक_त्रुटि, _("Checking cache tree of %s"), सूचकांक_path);

	अगर (0 <= it->entry_गिनती) {
		काष्ठा object *obj = parse_object(repo, &it->oid);
		अगर (!obj) {
			error(_("%s: invalid sha1 pointer in cache-tree of %s"),
			      oid_to_hex(&it->oid), सूचकांक_path);
			errors_found |= ERROR_REFS;
			वापस 1;
		}
		obj->flags |= USED;
		fsck_put_object_name(&fsck_walk_options, &it->oid, ":");
		mark_object_reachable(obj);
		अगर (obj->type != OBJ_वृक्ष)
			err |= objerror(repo, obj, _("non-tree in cache-tree"));
	}
	क्रम (i = 0; i < it->subtree_nr; i++)
		err |= fsck_cache_वृक्ष(repo, it->down[i]->cache_वृक्ष, सूचकांक_path);
	वापस err;
}

अटल पूर्णांक fsck_resolve_undo(काष्ठा सूचकांक_state *istate,
			     स्थिर अक्षर *सूचकांक_path)
{
	काष्ठा string_list_item *item;
	काष्ठा string_list *resolve_undo = istate->resolve_undo;

	अगर (!resolve_undo)
		वापस 0;

	for_each_string_list_item(item, resolve_undo) {
		स्थिर अक्षर *path = item->string;
		काष्ठा resolve_undo_info *ru = item->util;
		पूर्णांक i;

		अगर (!ru)
			जारी;
		क्रम (i = 0; i < 3; i++) {
			काष्ठा object *obj;

			अगर (!ru->mode[i] || !S_ISREG(ru->mode[i]))
				जारी;

			obj = parse_object(istate->repo, &ru->oid[i]);
			अगर (!obj) {
				error(_("%s: invalid sha1 pointer in resolve-undo of %s"),
				      oid_to_hex(&ru->oid[i]),
				      सूचकांक_path);
				errors_found |= ERROR_REFS;
				जारी;
			}
			obj->flags |= USED;
			fsck_put_object_name(&fsck_walk_options, &ru->oid[i],
					     ":(%d):%s", i, path);
			mark_object_reachable(obj);
		}
	}
	वापस 0;
}

अटल व्योम fsck_सूचकांक(काष्ठा सूचकांक_state *istate, स्थिर अक्षर *सूचकांक_path,
		       पूर्णांक is_current_worktree)
{
	अचिन्हित पूर्णांक i;

	/* TODO: audit for interaction with sparse-index. */
	ensure_full_सूचकांक(istate);
	क्रम (i = 0; i < istate->cache_nr; i++) {
		अचिन्हित पूर्णांक mode;
		काष्ठा blob *blob;
		काष्ठा object *obj;

		mode = istate->cache[i]->ce_mode;
		अगर (S_ISGITLINK(mode))
			जारी;
		blob = lookup_blob(istate->repo,
				   &istate->cache[i]->oid);
		अगर (!blob)
			जारी;
		obj = &blob->object;
		obj->flags |= USED;
		fsck_put_object_name(&fsck_walk_options, &obj->oid,
				     "%s:%s",
				     is_current_worktree ? "" : सूचकांक_path,
				     istate->cache[i]->name);
		mark_object_reachable(obj);
	}
	अगर (istate->cache_वृक्ष)
		fsck_cache_वृक्ष(istate->repo, istate->cache_वृक्ष, सूचकांक_path);
	fsck_resolve_undo(istate, सूचकांक_path);
}

अटल पूर्णांक mark_object_for_connectivity(स्थिर काष्ठा object_id *oid,
					काष्ठा object_info *oi UNUSED,
					व्योम *cb_तथ्य)
{
	काष्ठा repository *repo = cb_तथ्य;
	काष्ठा object *obj = lookup_unknown_object(repo, oid);
	obj->flags |= HAS_OBJ;
	वापस 0;
}

अटल व्योम fsck_refs(काष्ठा repository *r)
{
	काष्ठा child_process refs_verify = CHILD_PROCESS_INIT;
	काष्ठा progress *progress = शून्य;

	अगर (show_progress)
		progress = start_progress(r, _("Checking ref database"), 1);

	अगर (verbose)
		fprintf_ln(मानक_त्रुटि, _("Checking ref database"));

	child_process_init(&refs_verify);
	refs_verify.git_cmd = 1;
	strvec_pushl(&refs_verify.args, "refs", "verify", शून्य);
	अगर (verbose)
		strvec_धकेलो(&refs_verify.args, "--verbose");
	अगर (check_strict)
		strvec_धकेलो(&refs_verify.args, "--strict");

	अगर (run_command(&refs_verify))
		errors_found |= ERROR_REFS;

	display_progress(progress, 1);
	stop_progress(&progress);
}

अटल अक्षर स्थिर * स्थिर fsck_usage[] = {
	N_("git fsck [--tags] [--root] [--unreachable] [--cache] [--no-reflogs]\n"
	   "         [--[no-]full] [--strict] [--verbose] [--lost-found]\n"
	   "         [--[no-]dangling] [--[no-]progress] [--connectivity-only]\n"
	   "         [--[no-]name-objects] [--[no-]references] [<object>...]"),
	शून्य
};

पूर्णांक cmd_fsck(पूर्णांक argc,
	     स्थिर अक्षर **argv,
	     स्थिर अक्षर *prefix,
	     काष्ठा repository *repo)
{
	काष्ठा odb_fsck_options odb_fsck_opts = {
		.flags = ODB_FSCK_FULL,
		.object_cb = fsck_obj_बफर,
		.object_payload = repo,
	};
	पूर्णांक keep_cache_objects = 0;
	पूर्णांक name_objects = 0;
	पूर्णांक check_references = 1;
	काष्ठा option fsck_opts[] = {
		OPT__VERBOSE(&verbose, N_("be verbose")),
		OPT_BOOL(0, "unreachable", &show_unreachable, N_("show unreachable objects")),
		OPT_BOOL(0, "dangling", &show_dangling, N_("show dangling objects")),
		OPT_BOOL(0, "tags", &show_tags, N_("report tags")),
		OPT_BOOL(0, "root", &show_मूल, N_("report root nodes")),
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
	काष्ठा snapshot snap = {
		.nr = 0,
		.alloc = 0,
		.ref = शून्य
	};

	errors_found = 0;
	disable_replace_refs();
	save_commit_बफर = 0;

	argc = parse_options(argc, argv, prefix, fsck_opts, fsck_usage, 0);

	/* fsck knows how to handle missing promisor objects */
	repo->fetch_if_missing = 0;

	fsck_options_init(&fsck_walk_options, repo, FSCK_OPTIONS_DEFAULT);
	fsck_walk_options.walk = mark_object;

	fsck_options_init(&fsck_obj_options, repo, FSCK_OPTIONS_DEFAULT);
	fsck_obj_options.walk = mark_used;
	fsck_obj_options.error_func = fsck_objects_error_func;
	अगर (check_strict)
		fsck_obj_options.strict = 1;

	अगर (show_progress == -1)
		show_progress = isatty(2);
	अगर (verbose) {
		show_progress = 0;
		odb_fsck_opts.flags |= ODB_FSCK_VERBOSE;
	}
	अगर (show_progress)
		odb_fsck_opts.flags |= ODB_FSCK_PROGRESS;

	अगर (write_lost_and_found) {
		odb_fsck_opts.flags |= ODB_FSCK_FULL;
		include_reflogs = 0;
	}

	अगर (name_objects)
		fsck_enable_object_names(&fsck_walk_options);

	repo_config(repo, git_fsck_config, &fsck_obj_options);
	prepare_repo_settings(repo);

	अगर (check_references)
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

	अगर (connectivity_only) {
		odb_for_each_object(repo->objects, शून्य,
				    mark_object_for_connectivity, repo, 0);
	} अन्यथा {
		अगर (odb_fsck(repo->objects, &odb_fsck_opts) < 0)
			errors_found |= ERROR_OBJECT;

		अगर (fsck_finish(&fsck_obj_options))
			errors_found |= ERROR_OBJECT;
	}

	/* Process the snapshotted refs and the reflogs. */
	process_refs(repo, &snap);

	/* If not given any explicit objects, process index files too. */
	अगर (!argc)
		keep_cache_objects = 1;
	अगर (keep_cache_objects) {
		/*
		 * TODO: Consider first walking these indexes in snapshot_refs,
		 * to snapshot where the index entries used to point, and then
		 * check those snapshotted locations here.
		 */
		काष्ठा worktree **worktrees, **p;

		verify_सूचकांक_checksum = 1;
		verify_ce_order = 1;

		worktrees = get_worktrees(repo);
		क्रम (p = worktrees; *p; p++) {
			काष्ठा worktree *wt = *p;
			काष्ठा सूचकांक_state istate =
				सूचकांक_STATE_INIT(repo);
			अक्षर *path, *wt_gitdir;

			/*
			 * Make a copy since the buffer is reusable
			 * and may get overwritten by other calls
			 * while we're examining the index.
			 */
			path = xstrdup(worktree_git_path(wt, "index"));
			wt_gitdir = get_worktree_git_dir(wt);

			read_सूचकांक_from(&istate, path, wt_gitdir);
			fsck_सूचकांक(&istate, path, wt->is_current);

			discard_सूचकांक(&istate);
			मुक्त(wt_gitdir);
			मुक्त(path);
		}
		free_worktrees(worktrees);
	}

	check_connectivity(repo);

	अगर (repo->settings.core_commit_graph) {
		काष्ठा odb_source_files *files = odb_source_files_downcast(repo->objects->source);
		काष्ठा child_process commit_graph_verify = CHILD_PROCESS_INIT;

		क्रम (काष्ठा odb_files_dir *dir = files->dirs; dir; dir = dir->अग्र) {
			child_process_init(&commit_graph_verify);
			commit_graph_verify.git_cmd = 1;
			strvec_pushl(&commit_graph_verify.args, "commit-graph",
				     "verify", "--object-dir", dir->abspath, शून्य);
			अगर (show_progress)
				strvec_धकेलो(&commit_graph_verify.args, "--progress");
			अन्यथा
				strvec_धकेलो(&commit_graph_verify.args, "--no-progress");
			अगर (run_command(&commit_graph_verify))
				errors_found |= ERROR_COMMIT_GRAPH;
		}
	}

	free_snapshot_refs(&snap);
	वापस errors_found;
}
