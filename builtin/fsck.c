#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "hex.h"
#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ "commit.h"
#അന്തര്ഭാവ "tree.h"
#അന്തര്ഭാവ "blob.h"
#അന്തര്ഭാവ "tag.h"
#അന്തര്ഭാവ "refs.h"
#അന്തര്ഭാവ "cache-tree.h"
#അന്തര്ഭാവ "fsck.h"
#അന്തര്ഭാവ "parse-options.h"
#അന്തര്ഭാവ "progress.h"
#അന്തര്ഭാവ "packfile.h"
#അന്തര്ഭാവ "object-name.h"
#അന്തര്ഭാവ "odb.h"
#അന്തര്ഭാവ "odb/streaming.h"
#അന്തര്ഭാവ "path.h"
#അന്തര്ഭാവ "read-cache-ll.h"
#അന്തര്ഭാവ "replace-object.h"
#അന്തര്ഭാവ "resolve-undo.h"
#അന്തര്ഭാവ "run-command.h"
#അന്തര്ഭാവ "sparse-index.h"
#അന്തര്ഭാവ "worktree.h"

#define REACHABLE 0x0001
#define SEEN      0x0002
#define HAS_OBJ   0x0004
/* This flag is set if something points to this object. */
#define USED      0x0008

സ്ഥിതം പൂർണ്ണസംഖ്യ show_വേര്;
സ്ഥിതം പൂർണ്ണസംഖ്യ show_tags;
സ്ഥിതം പൂർണ്ണസംഖ്യ show_unreachable;
സ്ഥിതം പൂർണ്ണസംഖ്യ include_reflogs = 1;
സ്ഥിതം പൂർണ്ണസംഖ്യ connectivity_only;
സ്ഥിതം പൂർണ്ണസംഖ്യ check_strict;
സ്ഥിതം ഘടന fsck_options fsck_walk_options;
സ്ഥിതം ഘടന fsck_options fsck_obj_options;
സ്ഥിതം പൂർണ്ണസംഖ്യ errors_found;
സ്ഥിതം പൂർണ്ണസംഖ്യ write_lost_and_found;
സ്ഥിതം പൂർണ്ണസംഖ്യ verbose;
സ്ഥിതം പൂർണ്ണസംഖ്യ show_progress = -1;
സ്ഥിതം പൂർണ്ണസംഖ്യ show_dangling = 1;
സ്ഥിതം timestamp_t now;
#define ERROR_OBJECT 01
#define ERROR_REACHABLE 02
#define ERROR_REFS 010
#define ERROR_COMMIT_GRAPH 020

സ്ഥിതം സ്ഥിരം അക്ഷരം *describe_object(സ്ഥിരം ഘടന object_id *oid)
{
	തിരികെ fsck_describe_object(&fsck_walk_options, oid);
}

സ്ഥിതം സ്ഥിരം അക്ഷരം *printable_type(ഘടന repository *repo,
				  സ്ഥിരം ഘടന object_id *oid,
				  എണ്ണൽ object_type type)
{
	സ്ഥിരം അക്ഷരം *ret;

	എങ്കിൽ (type == OBJ_NONE)
		type = odb_read_object_info(repo->objects, oid, ശൂന്യ);

	ret = type_name(type);
	എങ്കിൽ (!ret)
		ret = _("unknown");

	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ objerror(ഘടന repository *repo, ഘടന object *obj, സ്ഥിരം അക്ഷരം *err)
{
	errors_found |= ERROR_OBJECT;
	/* TRANSLATORS: e.g. error in tree 01bfda: <more explanation> */
	fprintf_ln(stderr, _("error in %s %s: %s"),
		   printable_type(repo, &obj->oid, obj->type),
		   describe_object(&obj->oid), err);
	തിരികെ -1;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ fsck_objects_error_func(ഘടന fsck_options *o,
				   ശൂന്യം *fsck_report,
				   എണ്ണൽ fsck_സന്ദേശം_type സന്ദേശം_type,
				   എണ്ണൽ fsck_സന്ദേശം_id സന്ദേശം_id UNUSED,
				   സ്ഥിരം അക്ഷരം *message)
{
	ഘടന fsck_object_report *report = fsck_report;
	സ്ഥിരം ഘടന object_id *oid = report->oid;
	എണ്ണൽ object_type object_type = report->object_type;

	തിരഞ്ഞെടുപ്പ് (സന്ദേശം_type) {
	അവസ്ഥ FSCK_WARN:
		/* TRANSLATORS: e.g. warning in tree 01bfda: <more explanation> */
		fprintf_ln(stderr, _("warning in %s %s: %s"),
			   printable_type(o->repo, oid, object_type),
			   describe_object(oid), message);
		തിരികെ 0;
	അവസ്ഥ FSCK_ERROR:
		/* TRANSLATORS: e.g. error in tree 01bfda: <more explanation> */
		fprintf_ln(stderr, _("error in %s %s: %s"),
			   printable_type(o->repo, oid, object_type),
			   describe_object(oid), message);
		തിരികെ 1;
	സ്ഥിരസ്ഥിതി:
		BUG("%d (FSCK_IGNORE?) should never trigger this callback",
		    സന്ദേശം_type);
	}
}

സ്ഥിതം ഘടന object_array pending;

സ്ഥിതം പൂർണ്ണസംഖ്യ mark_object(ഘടന object *obj, എണ്ണൽ object_type type,
		       ശൂന്യം *data, ഘടന fsck_options *options)
{
	ഘടന object *parent = data;

	/*
	 * The only case data is NULL or type is OBJ_ANY is when
	 * mark_object_reachable() calls us.  All the callers of
	 * that function has non-NULL obj hence ...
	 */
	എങ്കിൽ (!obj) {
		/* ... these references to parent->fld are safe here */
		printf_ln(_("broken link from %7s %s"),
			  printable_type(options->repo, &parent->oid, parent->type),
			  describe_object(&parent->oid));
		printf_ln(_("broken link from %7s %s"),
			  (type == OBJ_ANY ? _("unknown") : type_name(type)),
			  _("unknown"));
		errors_found |= ERROR_REACHABLE;
		തിരികെ 1;
	}

	എങ്കിൽ (type != OBJ_ANY && obj->type != type)
		/* ... and the reference to parent is safe here */
		objerror(options->repo, parent, _("wrong object type in link"));

	എങ്കിൽ (obj->flags & REACHABLE)
		തിരികെ 0;
	obj->flags |= REACHABLE;

	എങ്കിൽ (is_promisor_object(options->repo, &obj->oid))
		/*
		 * Further recursion does not need to be performed on this
		 * object since it is a promisor object (so it does not need to
		 * be added to "pending").
		 */
		തിരികെ 0;

	എങ്കിൽ (!(obj->flags & HAS_OBJ)) {
		എങ്കിൽ (parent && !odb_has_object(options->repo->objects, &obj->oid,
					      ODB_HAS_OBJECT_RECHECK_PACKED)) {
			printf_ln(_("broken link from %7s %s\n"
				    "              to %7s %s"),
				  printable_type(options->repo, &parent->oid, parent->type),
				  describe_object(&parent->oid),
				  printable_type(options->repo, &obj->oid, obj->type),
				  describe_object(&obj->oid));
			errors_found |= ERROR_REACHABLE;
		}
		തിരികെ 1;
	}

	add_object_array(obj, ശൂന്യ, &pending);
	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം mark_object_reachable(ഘടന object *obj)
{
	mark_object(obj, OBJ_ANY, ശൂന്യ, &fsck_walk_options);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ traverse_one_object(ഘടന object *obj)
{
	പൂർണ്ണസംഖ്യ result = fsck_walk(obj, obj, &fsck_walk_options);

	എങ്കിൽ (obj->type == OBJ_വൃക്ഷം) {
		ഘടന വൃക്ഷം *വൃക്ഷം = (ഘടന വൃക്ഷം *)obj;
		free_വൃക്ഷം_buffer(വൃക്ഷം);
	}
	തിരികെ result;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ traverse_reachable(ഘടന repository *repo)
{
	ഘടന progress *progress = ശൂന്യ;
	unsigned പൂർണ്ണസംഖ്യ nr = 0;
	പൂർണ്ണസംഖ്യ result = 0;
	എങ്കിൽ (show_progress)
		progress = start_delayed_progress(repo,
						  _("Checking connectivity"), 0);
	ആകുമ്പോൾ (pending.nr) {
		result |= traverse_one_object(object_array_എടുക്കുക(&pending));
		display_progress(progress, ++nr);
	}
	stop_progress(&progress);
	തിരികെ !!result;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ mark_used(ഘടന object *obj, എണ്ണൽ object_type type UNUSED,
		     ശൂന്യം *data UNUSED, ഘടന fsck_options *options UNUSED)
{
	എങ്കിൽ (!obj)
		തിരികെ 1;
	obj->flags |= USED;
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ mark_unreachable_referents(സ്ഥിരം ഘടന object_id *oid,
				      ഘടന object_info *oi UNUSED,
				      ശൂന്യം *data)
{
	ഘടന repository *repo = data;
	ഘടന fsck_options options;
	ഘടന object *obj = lookup_object(data, oid);

	എങ്കിൽ (!obj || !(obj->flags & HAS_OBJ))
		തിരികെ 0; /* not part of our original set */
	എങ്കിൽ (obj->flags & REACHABLE)
		തിരികെ 0; /* reachable objects already traversed */

	/*
	 * Avoid passing OBJ_NONE to fsck_walk, which will parse the object
	 * (and we want to avoid parsing blobs).
	 */
	എങ്കിൽ (obj->type == OBJ_NONE) {
		എണ്ണൽ object_type type = odb_read_object_info(repo->objects,
							     &obj->oid, ശൂന്യ);
		എങ്കിൽ (type > 0)
			object_as_type(obj, type, 0);
	}

	fsck_options_init(&options, repo, FSCK_OPTIONS_DEFAULT);
	options.walk = mark_used;
	fsck_walk(obj, ശൂന്യ, &options);
	എങ്കിൽ (obj->type == OBJ_വൃക്ഷം)
		free_വൃക്ഷം_buffer((ഘടന വൃക്ഷം *)obj);

	തിരികെ 0;
}

/*
 * Check a single reachable object
 */
സ്ഥിതം ശൂന്യം check_reachable_object(ഘടന repository *repo, ഘടന object *obj)
{
	/*
	 * We obviously want the object to be parsed,
	 * except if it was in a pack-file and we didn't
	 * do a full fsck
	 */
	എങ്കിൽ (!(obj->flags & HAS_OBJ)) {
		എങ്കിൽ (is_promisor_object(repo, &obj->oid))
			തിരികെ;
		എങ്കിൽ (has_object_pack(repo, &obj->oid))
			തിരികെ; /* it is in pack - forget about it */
		printf_ln(_("missing %s %s"),
			  printable_type(repo, &obj->oid, obj->type),
			  describe_object(&obj->oid));
		errors_found |= ERROR_REACHABLE;
		തിരികെ;
	}
}

/*
 * Check a single unreachable object
 */
സ്ഥിതം ശൂന്യം check_unreachable_object(ഘടന repository *repo, ഘടന object *obj)
{
	/*
	 * Missing unreachable object? Ignore it. It's not like
	 * we miss it (since it can't be reached), nor do we want
	 * to complain about it being unreachable (since it does
	 * not exist).
	 */
	എങ്കിൽ (!(obj->flags & HAS_OBJ))
		തിരികെ;

	/*
	 * Unreachable object that exists? Show it if asked to,
	 * since this is something that is prunable.
	 */
	എങ്കിൽ (show_unreachable) {
		printf_ln(_("unreachable %s %s"),
			  printable_type(repo, &obj->oid, obj->type),
			  describe_object(&obj->oid));
		തിരികെ;
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
	എങ്കിൽ (!(obj->flags & USED)) {
		എങ്കിൽ (show_dangling)
			printf_ln(_("dangling %s %s"),
				  printable_type(repo, &obj->oid, obj->type),
				  describe_object(&obj->oid));
		എങ്കിൽ (write_lost_and_found) {
			അക്ഷരം *filename = repo_git_path(repo, "lost-found/%s/%s",
				obj->type == OBJ_COMMIT ? "commit" : "other",
				describe_object(&obj->oid));
			FILE *f;

			എങ്കിൽ (safe_create_leading_directories_const(repo, filename)) {
				error(_("could not create lost-found"));
				സ്വതന്ത്രം(filename);
				തിരികെ;
			}
			f = xfopen(filename, "w");
			എങ്കിൽ (obj->type == OBJ_BLOB) {
				എങ്കിൽ (odb_stream_blob_to_fd(repo->objects, fileno(f),
							  &obj->oid, ശൂന്യ, 1))
					die_errno(_("could not write '%s'"), filename);
			} അല്ലെങ്കിൽ
				fprintf(f, "%s\n", describe_object(&obj->oid));
			എങ്കിൽ (fclose(f))
				die_errno(_("could not finish '%s'"),
					  filename);
			സ്വതന്ത്രം(filename);
		}
		തിരികെ;
	}

	/*
	 * Otherwise? It's there, it's unreachable, and some other unreachable
	 * object points to it. Ignore it - it's not interesting, and we showed
	 * all the interesting cases above.
	 */
}

സ്ഥിതം ശൂന്യം check_object(ഘടന repository *repo, ഘടന object *obj)
{
	എങ്കിൽ (verbose)
		fprintf_ln(stderr, _("Checking %s"), describe_object(&obj->oid));

	എങ്കിൽ (obj->flags & REACHABLE)
		check_reachable_object(repo, obj);
	അല്ലെങ്കിൽ
		check_unreachable_object(repo, obj);
}

സ്ഥിതം ശൂന്യം check_connectivity(ഘടന repository *repo)
{
	പൂർണ്ണസംഖ്യ i, max;

	/* Traverse the pending reachable objects */
	traverse_reachable(repo);

	/*
	 * With --connectivity-only, we won't have actually opened and marked
	 * unreachable objects with USED. Do that now to make --dangling, etc
	 * accurate.
	 */
	എങ്കിൽ (connectivity_only && (show_dangling || write_lost_and_found)) {
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
		odb_for_each_object(repo->objects, ശൂന്യ,
				    mark_unreachable_referents, repo, 0);
	}

	/* Look up all the requirements, warn about missing objects.. */
	max = get_max_object_index(repo);
	എങ്കിൽ (verbose)
		fprintf_ln(stderr, _("Checking connectivity (%d objects)"), max);

	വേണ്ടി (i = 0; i < max; i++) {
		ഘടന object *obj = get_indexed_object(repo, i);

		എങ്കിൽ (obj)
			check_object(repo, obj);
	}
}

സ്ഥിതം പൂർണ്ണസംഖ്യ fsck_obj_buffer(സ്ഥിരം ഘടന object_id *oid, എണ്ണൽ object_type type,
			   unsigned long size, ശൂന്യം *buffer, പൂർണ്ണസംഖ്യ *eaten, ശൂന്യം *cb_data)
{
	ഘടന repository *repo = cb_data;
	ഘടന object *obj;
	പൂർണ്ണസംഖ്യ err;

	/*
	 * Note, buffer may be NULL if type is OBJ_BLOB. See
	 * verify_packfile(), data_valid variable for details.
	 */
	obj = parse_object_buffer(repo, oid, type, size, buffer, eaten);
	എങ്കിൽ (!obj) {
		errors_found |= ERROR_OBJECT;
		err = error(_("%s: object corrupt or missing"),
			    oid_to_hex(oid));
		പോകുക out;
	}

	obj->flags &= ~REACHABLE;
	obj->flags |= HAS_OBJ | SEEN;

	എങ്കിൽ (verbose)
		fprintf_ln(stderr, _("Checking %s %s"),
			   printable_type(repo, &obj->oid, obj->type),
			   describe_object(&obj->oid));

	എങ്കിൽ (fsck_walk(obj, ശൂന്യ, &fsck_obj_options))
		objerror(repo, obj, _("broken links"));

	err = fsck_object(obj, buffer, size, &fsck_obj_options);
	എങ്കിൽ (err)
		പോകുക out;

	എങ്കിൽ (obj->type == OBJ_COMMIT) {
		ഘടന commit *commit = (ഘടന commit *) obj;

		എങ്കിൽ (!commit->parents && show_വേര്)
			printf_ln(_("root %s"),
				  describe_object(&commit->object.oid));
	}

	എങ്കിൽ (obj->type == OBJ_TAG) {
		ഘടന tag *tag = (ഘടന tag *) obj;

		എങ്കിൽ (show_tags && tag->tagged) {
			printf_ln(_("tagged %s %s (%s) in %s"),
				  printable_type(repo, &tag->tagged->oid, tag->tagged->type),
				  describe_object(&tag->tagged->oid),
				  tag->tag,
				  describe_object(&tag->object.oid));
		}
	}

out:
	എങ്കിൽ (obj && obj->type == OBJ_വൃക്ഷം)
		free_വൃക്ഷം_buffer((ഘടന വൃക്ഷം *)obj);
	തിരികെ err;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ default_refs;

സ്ഥിതം ശൂന്യം fsck_handle_reflog_oid(ഘടന repository *repo,
				   സ്ഥിരം അക്ഷരം *refname, ഘടന object_id *oid,
				   timestamp_t timestamp)
{
	ഘടന object *obj;

	എങ്കിൽ (!is_null_oid(oid)) {
		obj = lookup_object(repo, oid);
		എങ്കിൽ (obj && (obj->flags & HAS_OBJ)) {
			എങ്കിൽ (timestamp)
				fsck_put_object_name(&fsck_walk_options, oid,
						     "%s@{%"PRItime"}",
						     refname, timestamp);
			obj->flags |= USED;
			mark_object_reachable(obj);
		} അല്ലെങ്കിൽ എങ്കിൽ (!is_promisor_object(repo, oid)) {
			error(_("%s: invalid reflog entry %s"),
			      refname, oid_to_hex(oid));
			errors_found |= ERROR_REACHABLE;
		}
	}
}

സ്ഥിതം പൂർണ്ണസംഖ്യ fsck_handle_reflog_ent(സ്ഥിരം അക്ഷരം *refname,
				  ഘടന object_id *ooid, ഘടന object_id *noid,
				  സ്ഥിരം അക്ഷരം *email UNUSED,
				  timestamp_t timestamp, പൂർണ്ണസംഖ്യ tz UNUSED,
				  സ്ഥിരം അക്ഷരം *message UNUSED, ശൂന്യം *cb_data)
{
	ഘടന repository *repo = cb_data;

	എങ്കിൽ (now && timestamp > now)
		തിരികെ 0;

	എങ്കിൽ (verbose)
		fprintf_ln(stderr, _("Checking reflog %s->%s"),
			   oid_to_hex(ooid), oid_to_hex(noid));

	fsck_handle_reflog_oid(repo, refname, ooid, 0);
	fsck_handle_reflog_oid(repo, refname, noid, timestamp);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ fsck_handle_reflog(സ്ഥിരം അക്ഷരം *logname, ശൂന്യം *cb_data)
{
	ഘടന strbuf refname = STRBUF_INIT;
	ഘടന worktree *wt = cb_data;

	strbuf_worktree_ref(wt, &refname, logname);
	refs_for_each_reflog_ent(get_main_ref_store(wt->repo),
				 refname.buf, fsck_handle_reflog_ent,
				 wt->repo);
	strbuf_release(&refname);
	തിരികെ 0;
}

ഘടന ref_snapshot {
	അക്ഷരം *refname;
	ഘടന object_id oid;
	/* TODO: Maybe supplement with latest reflog entry info too? */
};

ഘടന snapshot {
	size_t nr;
	size_t alloc;
	ഘടന ref_snapshot *ref;
	/* TODO: Consider also snapshotting the index of each worktree. */
};

ഘടന snapshot_ref_data {
	ഘടന repository *repo;
	ഘടന snapshot *snap;
};

സ്ഥിതം പൂർണ്ണസംഖ്യ snapshot_ref(സ്ഥിരം ഘടന reference *ref, ശൂന്യം *cb_data)
{
	ഘടന snapshot_ref_data *data = cb_data;
	ഘടന snapshot *snap = data->snap;
	ഘടന object *obj;

	obj = parse_object(data->repo, ref->oid);
	എങ്കിൽ (!obj) {
		എങ്കിൽ (is_promisor_object(data->repo, ref->oid)) {
			/*
			 * Increment default_refs anyway, because this is a
			 * valid ref.
			 */
			 default_refs++;
			 തിരികെ 0;
		}
		error(_("%s: invalid sha1 pointer %s"),
		      ref->name, oid_to_hex(ref->oid));
		errors_found |= ERROR_REACHABLE;
		/* We'll continue with the rest despite the error.. */
		തിരികെ 0;
	}
	എങ്കിൽ (obj->type != OBJ_COMMIT && is_branch(ref->name)) {
		error(_("%s: not a commit"), ref->name);
		errors_found |= ERROR_REFS;
	}
	default_refs++;

	ALLOC_GROW(snap->ref, snap->nr + 1, snap->alloc);
	snap->ref[snap->nr].refname = xstrdup(ref->name);
	oidcpy(&snap->ref[snap->nr].oid, ref->oid);
	snap->nr++;

	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ fsck_handle_ref(സ്ഥിരം ഘടന reference *ref, ശൂന്യം *cb_data)
{
	ഘടന repository *repo = cb_data;
	ഘടന object *obj;

	obj = parse_object(repo, ref->oid);
	obj->flags |= USED;
	fsck_put_object_name(&fsck_walk_options,
			     ref->oid, "%s", ref->name);
	mark_object_reachable(obj);

	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം snapshot_refs(ഘടന repository *repo,
			  ഘടന snapshot *snap, പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv)
{
	ഘടന refs_for_each_ref_options opts = {
		.flags = REFS_FOR_EACH_INCLUDE_BROKEN,
	};
	ഘടന snapshot_ref_data data = {
		.repo = repo,
		.snap = snap,
	};
	ഘടന worktree **worktrees, **p;
	സ്ഥിരം അക്ഷരം *head_points_at;
	ഘടന object_id head_oid;

	വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < argc; i++) {
		സ്ഥിരം അക്ഷരം *arg = argv[i];
		ഘടന object_id oid;
		എങ്കിൽ (!repo_get_oid(repo, arg, &oid)) {
			ഘടന reference ref = {
				.name = arg,
				.oid = &oid,
			};

			snapshot_ref(&ref, &data);
			തുടരുക;
		}
		error(_("invalid parameter: expected sha1, got '%s'"), arg);
		errors_found |= ERROR_OBJECT;
	}

	എങ്കിൽ (argc) {
		include_reflogs = 0;
		തിരികെ;
	}

	refs_for_each_ref_ext(get_main_ref_store(repo),
			      snapshot_ref, &data, &opts);

	worktrees = get_worktrees(repo);
	വേണ്ടി (p = worktrees; *p; p++) {
		ഘടന worktree *wt = *p;
		ഘടന strbuf refname = STRBUF_INIT;

		strbuf_worktree_ref(wt, &refname, "HEAD");

		head_points_at = refs_resolve_ref_unsafe(get_main_ref_store(repo),
							 refname.buf, 0, &head_oid, ശൂന്യ);

		എങ്കിൽ (head_points_at && !is_null_oid(&head_oid)) {
			ഘടന reference ref = {
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
	now = time(ശൂന്യ);
}


സ്ഥിതം ശൂന്യം free_snapshot_refs(ഘടന snapshot *snap)
{
	വേണ്ടി (size_t i = 0; i < snap->nr; i++)
		സ്വതന്ത്രം(snap->ref[i].refname);
	സ്വതന്ത്രം(snap->ref);
}

സ്ഥിതം ശൂന്യം process_refs(ഘടന repository *repo, ഘടന snapshot *snap)
{
	ഘടന worktree **worktrees, **p;

	വേണ്ടി (size_t i = 0; i < snap->nr; i++) {
		ഘടന reference ref = {
			.name = snap->ref[i].refname,
			.oid = &snap->ref[i].oid,
		};
		fsck_handle_ref(&ref, repo);
	}

	എങ്കിൽ (include_reflogs) {
		worktrees = get_worktrees(repo);
		വേണ്ടി (p = worktrees; *p; p++) {
			ഘടന worktree *wt = *p;

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
	എങ്കിൽ (!default_refs) {
		fprintf_ln(stderr, _("notice: No default references"));
		show_unreachable = 0;
	}
}

സ്ഥിതം പൂർണ്ണസംഖ്യ fsck_cache_വൃക്ഷം(ഘടന repository *repo, ഘടന cache_വൃക്ഷം *it, സ്ഥിരം അക്ഷരം *index_path)
{
	പൂർണ്ണസംഖ്യ i;
	പൂർണ്ണസംഖ്യ err = 0;

	എങ്കിൽ (verbose)
		fprintf_ln(stderr, _("Checking cache tree of %s"), index_path);

	എങ്കിൽ (0 <= it->entry_എണ്ണം) {
		ഘടന object *obj = parse_object(repo, &it->oid);
		എങ്കിൽ (!obj) {
			error(_("%s: invalid sha1 pointer in cache-tree of %s"),
			      oid_to_hex(&it->oid), index_path);
			errors_found |= ERROR_REFS;
			തിരികെ 1;
		}
		obj->flags |= USED;
		fsck_put_object_name(&fsck_walk_options, &it->oid, ":");
		mark_object_reachable(obj);
		എങ്കിൽ (obj->type != OBJ_വൃക്ഷം)
			err |= objerror(repo, obj, _("non-tree in cache-tree"));
	}
	വേണ്ടി (i = 0; i < it->subtree_nr; i++)
		err |= fsck_cache_വൃക്ഷം(repo, it->down[i]->cache_വൃക്ഷം, index_path);
	തിരികെ err;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ fsck_resolve_undo(ഘടന index_state *istate,
			     സ്ഥിരം അക്ഷരം *index_path)
{
	ഘടന string_list_item *item;
	ഘടന string_list *resolve_undo = istate->resolve_undo;

	എങ്കിൽ (!resolve_undo)
		തിരികെ 0;

	for_each_string_list_item(item, resolve_undo) {
		സ്ഥിരം അക്ഷരം *path = item->string;
		ഘടന resolve_undo_info *ru = item->util;
		പൂർണ്ണസംഖ്യ i;

		എങ്കിൽ (!ru)
			തുടരുക;
		വേണ്ടി (i = 0; i < 3; i++) {
			ഘടന object *obj;

			എങ്കിൽ (!ru->mode[i] || !S_ISREG(ru->mode[i]))
				തുടരുക;

			obj = parse_object(istate->repo, &ru->oid[i]);
			എങ്കിൽ (!obj) {
				error(_("%s: invalid sha1 pointer in resolve-undo of %s"),
				      oid_to_hex(&ru->oid[i]),
				      index_path);
				errors_found |= ERROR_REFS;
				തുടരുക;
			}
			obj->flags |= USED;
			fsck_put_object_name(&fsck_walk_options, &ru->oid[i],
					     ":(%d):%s", i, path);
			mark_object_reachable(obj);
		}
	}
	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം fsck_index(ഘടന index_state *istate, സ്ഥിരം അക്ഷരം *index_path,
		       പൂർണ്ണസംഖ്യ is_current_worktree)
{
	unsigned പൂർണ്ണസംഖ്യ i;

	/* TODO: audit for interaction with sparse-index. */
	ensure_full_index(istate);
	വേണ്ടി (i = 0; i < istate->cache_nr; i++) {
		unsigned പൂർണ്ണസംഖ്യ mode;
		ഘടന blob *blob;
		ഘടന object *obj;

		mode = istate->cache[i]->ce_mode;
		എങ്കിൽ (S_ISGITLINK(mode))
			തുടരുക;
		blob = lookup_blob(istate->repo,
				   &istate->cache[i]->oid);
		എങ്കിൽ (!blob)
			തുടരുക;
		obj = &blob->object;
		obj->flags |= USED;
		fsck_put_object_name(&fsck_walk_options, &obj->oid,
				     "%s:%s",
				     is_current_worktree ? "" : index_path,
				     istate->cache[i]->name);
		mark_object_reachable(obj);
	}
	എങ്കിൽ (istate->cache_വൃക്ഷം)
		fsck_cache_വൃക്ഷം(istate->repo, istate->cache_വൃക്ഷം, index_path);
	fsck_resolve_undo(istate, index_path);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ mark_object_for_connectivity(സ്ഥിരം ഘടന object_id *oid,
					ഘടന object_info *oi UNUSED,
					ശൂന്യം *cb_data)
{
	ഘടന repository *repo = cb_data;
	ഘടന object *obj = lookup_unknown_object(repo, oid);
	obj->flags |= HAS_OBJ;
	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം fsck_refs(ഘടന repository *r)
{
	ഘടന child_process refs_verify = CHILD_PROCESS_INIT;
	ഘടന progress *progress = ശൂന്യ;

	എങ്കിൽ (show_progress)
		progress = start_progress(r, _("Checking ref database"), 1);

	എങ്കിൽ (verbose)
		fprintf_ln(stderr, _("Checking ref database"));

	child_process_init(&refs_verify);
	refs_verify.git_cmd = 1;
	strvec_pushl(&refs_verify.args, "refs", "verify", ശൂന്യ);
	എങ്കിൽ (verbose)
		strvec_തള്ളുക(&refs_verify.args, "--verbose");
	എങ്കിൽ (check_strict)
		strvec_തള്ളുക(&refs_verify.args, "--strict");

	എങ്കിൽ (run_command(&refs_verify))
		errors_found |= ERROR_REFS;

	display_progress(progress, 1);
	stop_progress(&progress);
}

സ്ഥിതം അക്ഷരം സ്ഥിരം * സ്ഥിരം fsck_usage[] = {
	N_("git fsck [--tags] [--root] [--unreachable] [--cache] [--no-reflogs]\n"
	   "         [--[no-]full] [--strict] [--verbose] [--lost-found]\n"
	   "         [--[no-]dangling] [--[no-]progress] [--connectivity-only]\n"
	   "         [--[no-]name-objects] [--[no-]references] [<object>...]"),
	ശൂന്യ
};

പൂർണ്ണസംഖ്യ cmd_fsck(പൂർണ്ണസംഖ്യ argc,
	     സ്ഥിരം അക്ഷരം **argv,
	     സ്ഥിരം അക്ഷരം *prefix,
	     ഘടന repository *repo)
{
	ഘടന odb_fsck_options odb_fsck_opts = {
		.flags = ODB_FSCK_FULL,
		.object_cb = fsck_obj_buffer,
		.object_payload = repo,
	};
	പൂർണ്ണസംഖ്യ keep_cache_objects = 0;
	പൂർണ്ണസംഖ്യ name_objects = 0;
	പൂർണ്ണസംഖ്യ check_references = 1;
	ഘടന option fsck_opts[] = {
		OPT__VERBOSE(&verbose, N_("be verbose")),
		OPT_BOOL(0, "unreachable", &show_unreachable, N_("show unreachable objects")),
		OPT_BOOL(0, "dangling", &show_dangling, N_("show dangling objects")),
		OPT_BOOL(0, "tags", &show_tags, N_("report tags")),
		OPT_BOOL(0, "root", &show_വേര്, N_("report root nodes")),
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
	ഘടന snapshot snap = {
		.nr = 0,
		.alloc = 0,
		.ref = ശൂന്യ
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
	എങ്കിൽ (check_strict)
		fsck_obj_options.strict = 1;

	എങ്കിൽ (show_progress == -1)
		show_progress = isatty(2);
	എങ്കിൽ (verbose) {
		show_progress = 0;
		odb_fsck_opts.flags |= ODB_FSCK_VERBOSE;
	}
	എങ്കിൽ (show_progress)
		odb_fsck_opts.flags |= ODB_FSCK_PROGRESS;

	എങ്കിൽ (write_lost_and_found) {
		odb_fsck_opts.flags |= ODB_FSCK_FULL;
		include_reflogs = 0;
	}

	എങ്കിൽ (name_objects)
		fsck_enable_object_names(&fsck_walk_options);

	repo_config(repo, git_fsck_config, &fsck_obj_options);
	prepare_repo_settings(repo);

	എങ്കിൽ (check_references)
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

	എങ്കിൽ (connectivity_only) {
		odb_for_each_object(repo->objects, ശൂന്യ,
				    mark_object_for_connectivity, repo, 0);
	} അല്ലെങ്കിൽ {
		എങ്കിൽ (odb_fsck(repo->objects, &odb_fsck_opts) < 0)
			errors_found |= ERROR_OBJECT;

		എങ്കിൽ (fsck_finish(&fsck_obj_options))
			errors_found |= ERROR_OBJECT;
	}

	/* Process the snapshotted refs and the reflogs. */
	process_refs(repo, &snap);

	/* If not given any explicit objects, process index files too. */
	എങ്കിൽ (!argc)
		keep_cache_objects = 1;
	എങ്കിൽ (keep_cache_objects) {
		/*
		 * TODO: Consider first walking these indexes in snapshot_refs,
		 * to snapshot where the index entries used to point, and then
		 * check those snapshotted locations here.
		 */
		ഘടന worktree **worktrees, **p;

		verify_index_checksum = 1;
		verify_ce_order = 1;

		worktrees = get_worktrees(repo);
		വേണ്ടി (p = worktrees; *p; p++) {
			ഘടന worktree *wt = *p;
			ഘടന index_state istate =
				INDEX_STATE_INIT(repo);
			അക്ഷരം *path, *wt_gitdir;

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
			സ്വതന്ത്രം(wt_gitdir);
			സ്വതന്ത്രം(path);
		}
		free_worktrees(worktrees);
	}

	check_connectivity(repo);

	എങ്കിൽ (repo->settings.core_commit_graph) {
		ഘടന odb_source_files *files = odb_source_files_downcast(repo->objects->source);
		ഘടന child_process commit_graph_verify = CHILD_PROCESS_INIT;

		വേണ്ടി (ഘടന odb_files_dir *dir = files->dirs; dir; dir = dir->next) {
			child_process_init(&commit_graph_verify);
			commit_graph_verify.git_cmd = 1;
			strvec_pushl(&commit_graph_verify.args, "commit-graph",
				     "verify", "--object-dir", dir->abspath, ശൂന്യ);
			എങ്കിൽ (show_progress)
				strvec_തള്ളുക(&commit_graph_verify.args, "--progress");
			അല്ലെങ്കിൽ
				strvec_തള്ളുക(&commit_graph_verify.args, "--no-progress");
			എങ്കിൽ (run_command(&commit_graph_verify))
				errors_found |= ERROR_COMMIT_GRAPH;
		}
	}

	free_snapshot_refs(&snap);
	തിരികെ errors_found;
}
