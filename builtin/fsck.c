#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "hex.h"
#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ "commit.h"
#ଅନ୍ତର୍ଭାଵ "tree.h"
#ଅନ୍ତର୍ଭାଵ "blob.h"
#ଅନ୍ତର୍ଭାଵ "tag.h"
#ଅନ୍ତର୍ଭାଵ "refs.h"
#ଅନ୍ତର୍ଭାଵ "cache-tree.h"
#ଅନ୍ତର୍ଭାଵ "fsck.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"
#ଅନ୍ତର୍ଭାଵ "progress.h"
#ଅନ୍ତର୍ଭାଵ "packfile.h"
#ଅନ୍ତର୍ଭାଵ "object-name.h"
#ଅନ୍ତର୍ଭାଵ "odb.h"
#ଅନ୍ତର୍ଭାଵ "odb/streaming.h"
#ଅନ୍ତର୍ଭାଵ "path.h"
#ଅନ୍ତର୍ଭାଵ "read-cache-ll.h"
#ଅନ୍ତର୍ଭାଵ "replace-object.h"
#ଅନ୍ତର୍ଭାଵ "resolve-undo.h"
#ଅନ୍ତର୍ଭାଵ "run-command.h"
#ଅନ୍ତର୍ଭାଵ "sparse-index.h"
#ଅନ୍ତର୍ଭାଵ "worktree.h"

#define REACHABLE 0x0001
#define SEEN      0x0002
#define HAS_OBJ   0x0004
/* This flag is set if something points to this object. */
#define USED      0x0008

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_ମୂଳ;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_tags;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_unreachable;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା include_reflogs = 1;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା connectivity_only;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା check_strict;
ସ୍ଥିର ସଂରଚନା fsck_options fsck_walk_options;
ସ୍ଥିର ସଂରଚନା fsck_options fsck_obj_options;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା errors_found;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା write_lost_and_found;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା verbose;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_progress = -1;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_dangling = 1;
ସ୍ଥିର timestamp_t now;
#define ERROR_OBJECT 01
#define ERROR_REACHABLE 02
#define ERROR_REFS 010
#define ERROR_COMMIT_GRAPH 020

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *describe_object(ଅଚଳ ସଂରଚନା object_id *oid)
{
	ଫେରନ୍ତୁ fsck_describe_object(&fsck_walk_options, oid);
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *printable_type(ସଂରଚନା repository *repo,
				  ଅଚଳ ସଂରଚନା object_id *oid,
				  ଗଣନା object_type type)
{
	ଅଚଳ ଅକ୍ଷର *ret;

	ଯଦି (type == OBJ_NONE)
		type = odb_read_object_info(repo->objects, oid, ଖାଲି);

	ret = type_name(type);
	ଯଦି (!ret)
		ret = _("unknown");

	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା objerror(ସଂରଚନା repository *repo, ସଂରଚନା object *obj, ଅଚଳ ଅକ୍ଷର *err)
{
	errors_found |= ERROR_OBJECT;
	/* TRANSLATORS: e.g. error in tree 01bfda: <more explanation> */
	fprintf_ln(stderr, _("error in %s %s: %s"),
		   printable_type(repo, &obj->oid, obj->type),
		   describe_object(&obj->oid), err);
	ଫେରନ୍ତୁ -1;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା fsck_objects_error_func(ସଂରଚନା fsck_options *o,
				   ଶୂନ୍ୟ *fsck_report,
				   ଗଣନା fsck_ସନ୍ଦେଶ_type ସନ୍ଦେଶ_type,
				   ଗଣନା fsck_ସନ୍ଦେଶ_id ସନ୍ଦେଶ_id UNUSED,
				   ଅଚଳ ଅକ୍ଷର *message)
{
	ସଂରଚନା fsck_object_report *report = fsck_report;
	ଅଚଳ ସଂରଚନା object_id *oid = report->oid;
	ଗଣନା object_type object_type = report->object_type;

	ବାଛ (ସନ୍ଦେଶ_type) {
	ସ୍ଥିତି FSCK_WARN:
		/* TRANSLATORS: e.g. warning in tree 01bfda: <more explanation> */
		fprintf_ln(stderr, _("warning in %s %s: %s"),
			   printable_type(o->repo, oid, object_type),
			   describe_object(oid), message);
		ଫେରନ୍ତୁ 0;
	ସ୍ଥିତି FSCK_ERROR:
		/* TRANSLATORS: e.g. error in tree 01bfda: <more explanation> */
		fprintf_ln(stderr, _("error in %s %s: %s"),
			   printable_type(o->repo, oid, object_type),
			   describe_object(oid), message);
		ଫେରନ୍ତୁ 1;
	ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
		BUG("%d (FSCK_IGNORE?) should never trigger this callback",
		    ସନ୍ଦେଶ_type);
	}
}

ସ୍ଥିର ସଂରଚନା object_array pending;

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା mark_object(ସଂରଚନା object *obj, ଗଣନା object_type type,
		       ଶୂନ୍ୟ *data, ସଂରଚନା fsck_options *options)
{
	ସଂରଚନା object *parent = data;

	/*
	 * The only case data is NULL or type is OBJ_ANY is when
	 * mark_object_reachable() calls us.  All the callers of
	 * that function has non-NULL obj hence ...
	 */
	ଯଦି (!obj) {
		/* ... these references to parent->fld are safe here */
		printf_ln(_("broken link from %7s %s"),
			  printable_type(options->repo, &parent->oid, parent->type),
			  describe_object(&parent->oid));
		printf_ln(_("broken link from %7s %s"),
			  (type == OBJ_ANY ? _("unknown") : type_name(type)),
			  _("unknown"));
		errors_found |= ERROR_REACHABLE;
		ଫେରନ୍ତୁ 1;
	}

	ଯଦି (type != OBJ_ANY && obj->type != type)
		/* ... and the reference to parent is safe here */
		objerror(options->repo, parent, _("wrong object type in link"));

	ଯଦି (obj->flags & REACHABLE)
		ଫେରନ୍ତୁ 0;
	obj->flags |= REACHABLE;

	ଯଦି (is_promisor_object(options->repo, &obj->oid))
		/*
		 * Further recursion does not need to be performed on this
		 * object since it is a promisor object (so it does not need to
		 * be added to "pending").
		 */
		ଫେରନ୍ତୁ 0;

	ଯଦି (!(obj->flags & HAS_OBJ)) {
		ଯଦି (parent && !odb_has_object(options->repo->objects, &obj->oid,
					      ODB_HAS_OBJECT_RECHECK_PACKED)) {
			printf_ln(_("broken link from %7s %s\n"
				    "              to %7s %s"),
				  printable_type(options->repo, &parent->oid, parent->type),
				  describe_object(&parent->oid),
				  printable_type(options->repo, &obj->oid, obj->type),
				  describe_object(&obj->oid));
			errors_found |= ERROR_REACHABLE;
		}
		ଫେରନ୍ତୁ 1;
	}

	add_object_array(obj, ଖାଲି, &pending);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ mark_object_reachable(ସଂରଚନା object *obj)
{
	mark_object(obj, OBJ_ANY, ଖାଲି, &fsck_walk_options);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା traverse_one_object(ସଂରଚନା object *obj)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା result = fsck_walk(obj, obj, &fsck_walk_options);

	ଯଦି (obj->type == OBJ_ବୃକ୍ଷ) {
		ସଂରଚନା ବୃକ୍ଷ *ବୃକ୍ଷ = (ସଂରଚନା ବୃକ୍ଷ *)obj;
		free_ବୃକ୍ଷ_buffer(ବୃକ୍ଷ);
	}
	ଫେରନ୍ତୁ result;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା traverse_reachable(ସଂରଚନା repository *repo)
{
	ସଂରଚନା progress *progress = ଖାଲି;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା nr = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା result = 0;
	ଯଦି (show_progress)
		progress = start_delayed_progress(repo,
						  _("Checking connectivity"), 0);
	ଯେତେବେଳେ (pending.nr) {
		result |= traverse_one_object(object_array_କାଢ଼(&pending));
		display_progress(progress, ++nr);
	}
	stop_progress(&progress);
	ଫେରନ୍ତୁ !!result;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା mark_used(ସଂରଚନା object *obj, ଗଣନା object_type type UNUSED,
		     ଶୂନ୍ୟ *data UNUSED, ସଂରଚନା fsck_options *options UNUSED)
{
	ଯଦି (!obj)
		ଫେରନ୍ତୁ 1;
	obj->flags |= USED;
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା mark_unreachable_referents(ଅଚଳ ସଂରଚନା object_id *oid,
				      ସଂରଚନା object_info *oi UNUSED,
				      ଶୂନ୍ୟ *data)
{
	ସଂରଚନା repository *repo = data;
	ସଂରଚନା fsck_options options;
	ସଂରଚନା object *obj = lookup_object(data, oid);

	ଯଦି (!obj || !(obj->flags & HAS_OBJ))
		ଫେରନ୍ତୁ 0; /* not part of our original set */
	ଯଦି (obj->flags & REACHABLE)
		ଫେରନ୍ତୁ 0; /* reachable objects already traversed */

	/*
	 * Avoid passing OBJ_NONE to fsck_walk, which will parse the object
	 * (and we want to avoid parsing blobs).
	 */
	ଯଦି (obj->type == OBJ_NONE) {
		ଗଣନା object_type type = odb_read_object_info(repo->objects,
							     &obj->oid, ଖାଲି);
		ଯଦି (type > 0)
			object_as_type(obj, type, 0);
	}

	fsck_options_init(&options, repo, FSCK_OPTIONS_DEFAULT);
	options.walk = mark_used;
	fsck_walk(obj, ଖାଲି, &options);
	ଯଦି (obj->type == OBJ_ବୃକ୍ଷ)
		free_ବୃକ୍ଷ_buffer((ସଂରଚନା ବୃକ୍ଷ *)obj);

	ଫେରନ୍ତୁ 0;
}

/*
 * Check a single reachable object
 */
ସ୍ଥିର ଶୂନ୍ୟ check_reachable_object(ସଂରଚନା repository *repo, ସଂରଚନା object *obj)
{
	/*
	 * We obviously want the object to be parsed,
	 * except if it was in a pack-file and we didn't
	 * do a full fsck
	 */
	ଯଦି (!(obj->flags & HAS_OBJ)) {
		ଯଦି (is_promisor_object(repo, &obj->oid))
			ଫେରନ୍ତୁ;
		ଯଦି (has_object_pack(repo, &obj->oid))
			ଫେରନ୍ତୁ; /* it is in pack - forget about it */
		printf_ln(_("missing %s %s"),
			  printable_type(repo, &obj->oid, obj->type),
			  describe_object(&obj->oid));
		errors_found |= ERROR_REACHABLE;
		ଫେରନ୍ତୁ;
	}
}

/*
 * Check a single unreachable object
 */
ସ୍ଥିର ଶୂନ୍ୟ check_unreachable_object(ସଂରଚନା repository *repo, ସଂରଚନା object *obj)
{
	/*
	 * Missing unreachable object? Ignore it. It's not like
	 * we miss it (since it can't be reached), nor do we want
	 * to complain about it being unreachable (since it does
	 * not exist).
	 */
	ଯଦି (!(obj->flags & HAS_OBJ))
		ଫେରନ୍ତୁ;

	/*
	 * Unreachable object that exists? Show it if asked to,
	 * since this is something that is prunable.
	 */
	ଯଦି (show_unreachable) {
		printf_ln(_("unreachable %s %s"),
			  printable_type(repo, &obj->oid, obj->type),
			  describe_object(&obj->oid));
		ଫେରନ୍ତୁ;
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
	ଯଦି (!(obj->flags & USED)) {
		ଯଦି (show_dangling)
			printf_ln(_("dangling %s %s"),
				  printable_type(repo, &obj->oid, obj->type),
				  describe_object(&obj->oid));
		ଯଦି (write_lost_and_found) {
			ଅକ୍ଷର *filename = repo_git_path(repo, "lost-found/%s/%s",
				obj->type == OBJ_COMMIT ? "commit" : "other",
				describe_object(&obj->oid));
			FILE *f;

			ଯଦି (safe_create_leading_directories_const(repo, filename)) {
				error(_("could not create lost-found"));
				ମୁକ୍ତ(filename);
				ଫେରନ୍ତୁ;
			}
			f = xfopen(filename, "w");
			ଯଦି (obj->type == OBJ_BLOB) {
				ଯଦି (odb_stream_blob_to_fd(repo->objects, fileno(f),
							  &obj->oid, ଖାଲି, 1))
					die_errno(_("could not write '%s'"), filename);
			} ଅନ୍ୟଥା
				fprintf(f, "%s\n", describe_object(&obj->oid));
			ଯଦି (fclose(f))
				die_errno(_("could not finish '%s'"),
					  filename);
			ମୁକ୍ତ(filename);
		}
		ଫେରନ୍ତୁ;
	}

	/*
	 * Otherwise? It's there, it's unreachable, and some other unreachable
	 * object points to it. Ignore it - it's not interesting, and we showed
	 * all the interesting cases above.
	 */
}

ସ୍ଥିର ଶୂନ୍ୟ check_object(ସଂରଚନା repository *repo, ସଂରଚନା object *obj)
{
	ଯଦି (verbose)
		fprintf_ln(stderr, _("Checking %s"), describe_object(&obj->oid));

	ଯଦି (obj->flags & REACHABLE)
		check_reachable_object(repo, obj);
	ଅନ୍ୟଥା
		check_unreachable_object(repo, obj);
}

ସ୍ଥିର ଶୂନ୍ୟ check_connectivity(ସଂରଚନା repository *repo)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, max;

	/* Traverse the pending reachable objects */
	traverse_reachable(repo);

	/*
	 * With --connectivity-only, we won't have actually opened and marked
	 * unreachable objects with USED. Do that now to make --dangling, etc
	 * accurate.
	 */
	ଯଦି (connectivity_only && (show_dangling || write_lost_and_found)) {
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
		odb_for_each_object(repo->objects, ଖାଲି,
				    mark_unreachable_referents, repo, 0);
	}

	/* Look up all the requirements, warn about missing objects.. */
	max = get_max_object_index(repo);
	ଯଦି (verbose)
		fprintf_ln(stderr, _("Checking connectivity (%d objects)"), max);

	ପାଇଁ (i = 0; i < max; i++) {
		ସଂରଚନା object *obj = get_indexed_object(repo, i);

		ଯଦି (obj)
			check_object(repo, obj);
	}
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା fsck_obj_buffer(ଅଚଳ ସଂରଚନା object_id *oid, ଗଣନା object_type type,
			   unsigned long size, ଶୂନ୍ୟ *buffer, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *eaten, ଶୂନ୍ୟ *cb_data)
{
	ସଂରଚନା repository *repo = cb_data;
	ସଂରଚନା object *obj;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା err;

	/*
	 * Note, buffer may be NULL if type is OBJ_BLOB. See
	 * verify_packfile(), data_valid variable for details.
	 */
	obj = parse_object_buffer(repo, oid, type, size, buffer, eaten);
	ଯଦି (!obj) {
		errors_found |= ERROR_OBJECT;
		err = error(_("%s: object corrupt or missing"),
			    oid_to_hex(oid));
		ଯାଅ out;
	}

	obj->flags &= ~REACHABLE;
	obj->flags |= HAS_OBJ | SEEN;

	ଯଦି (verbose)
		fprintf_ln(stderr, _("Checking %s %s"),
			   printable_type(repo, &obj->oid, obj->type),
			   describe_object(&obj->oid));

	ଯଦି (fsck_walk(obj, ଖାଲି, &fsck_obj_options))
		objerror(repo, obj, _("broken links"));

	err = fsck_object(obj, buffer, size, &fsck_obj_options);
	ଯଦି (err)
		ଯାଅ out;

	ଯଦି (obj->type == OBJ_COMMIT) {
		ସଂରଚନା commit *commit = (ସଂରଚନା commit *) obj;

		ଯଦି (!commit->parents && show_ମୂଳ)
			printf_ln(_("root %s"),
				  describe_object(&commit->object.oid));
	}

	ଯଦି (obj->type == OBJ_TAG) {
		ସଂରଚନା tag *tag = (ସଂରଚନା tag *) obj;

		ଯଦି (show_tags && tag->tagged) {
			printf_ln(_("tagged %s %s (%s) in %s"),
				  printable_type(repo, &tag->tagged->oid, tag->tagged->type),
				  describe_object(&tag->tagged->oid),
				  tag->tag,
				  describe_object(&tag->object.oid));
		}
	}

out:
	ଯଦି (obj && obj->type == OBJ_ବୃକ୍ଷ)
		free_ବୃକ୍ଷ_buffer((ସଂରଚନା ବୃକ୍ଷ *)obj);
	ଫେରନ୍ତୁ err;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା default_refs;

ସ୍ଥିର ଶୂନ୍ୟ fsck_handle_reflog_oid(ସଂରଚନା repository *repo,
				   ଅଚଳ ଅକ୍ଷର *refname, ସଂରଚନା object_id *oid,
				   timestamp_t timestamp)
{
	ସଂରଚନା object *obj;

	ଯଦି (!is_null_oid(oid)) {
		obj = lookup_object(repo, oid);
		ଯଦି (obj && (obj->flags & HAS_OBJ)) {
			ଯଦି (timestamp)
				fsck_put_object_name(&fsck_walk_options, oid,
						     "%s@{%"PRItime"}",
						     refname, timestamp);
			obj->flags |= USED;
			mark_object_reachable(obj);
		} ଅନ୍ୟଥା ଯଦି (!is_promisor_object(repo, oid)) {
			error(_("%s: invalid reflog entry %s"),
			      refname, oid_to_hex(oid));
			errors_found |= ERROR_REACHABLE;
		}
	}
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା fsck_handle_reflog_ent(ଅଚଳ ଅକ୍ଷର *refname,
				  ସଂରଚନା object_id *ooid, ସଂରଚନା object_id *noid,
				  ଅଚଳ ଅକ୍ଷର *email UNUSED,
				  timestamp_t timestamp, ପୂର୍ଣ୍ଣସଂଖ୍ୟା tz UNUSED,
				  ଅଚଳ ଅକ୍ଷର *message UNUSED, ଶୂନ୍ୟ *cb_data)
{
	ସଂରଚନା repository *repo = cb_data;

	ଯଦି (now && timestamp > now)
		ଫେରନ୍ତୁ 0;

	ଯଦି (verbose)
		fprintf_ln(stderr, _("Checking reflog %s->%s"),
			   oid_to_hex(ooid), oid_to_hex(noid));

	fsck_handle_reflog_oid(repo, refname, ooid, 0);
	fsck_handle_reflog_oid(repo, refname, noid, timestamp);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା fsck_handle_reflog(ଅଚଳ ଅକ୍ଷର *logname, ଶୂନ୍ୟ *cb_data)
{
	ସଂରଚନା strbuf refname = STRBUF_INIT;
	ସଂରଚନା worktree *wt = cb_data;

	strbuf_worktree_ref(wt, &refname, logname);
	refs_for_each_reflog_ent(get_main_ref_store(wt->repo),
				 refname.buf, fsck_handle_reflog_ent,
				 wt->repo);
	strbuf_release(&refname);
	ଫେରନ୍ତୁ 0;
}

ସଂରଚନା ref_snapshot {
	ଅକ୍ଷର *refname;
	ସଂରଚନା object_id oid;
	/* TODO: Maybe supplement with latest reflog entry info too? */
};

ସଂରଚନା snapshot {
	size_t nr;
	size_t alloc;
	ସଂରଚନା ref_snapshot *ref;
	/* TODO: Consider also snapshotting the index of each worktree. */
};

ସଂରଚନା snapshot_ref_data {
	ସଂରଚନା repository *repo;
	ସଂରଚନା snapshot *snap;
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା snapshot_ref(ଅଚଳ ସଂରଚନା reference *ref, ଶୂନ୍ୟ *cb_data)
{
	ସଂରଚନା snapshot_ref_data *data = cb_data;
	ସଂରଚନା snapshot *snap = data->snap;
	ସଂରଚନା object *obj;

	obj = parse_object(data->repo, ref->oid);
	ଯଦି (!obj) {
		ଯଦି (is_promisor_object(data->repo, ref->oid)) {
			/*
			 * Increment default_refs anyway, because this is a
			 * valid ref.
			 */
			 default_refs++;
			 ଫେରନ୍ତୁ 0;
		}
		error(_("%s: invalid sha1 pointer %s"),
		      ref->name, oid_to_hex(ref->oid));
		errors_found |= ERROR_REACHABLE;
		/* We'll continue with the rest despite the error.. */
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (obj->type != OBJ_COMMIT && is_branch(ref->name)) {
		error(_("%s: not a commit"), ref->name);
		errors_found |= ERROR_REFS;
	}
	default_refs++;

	ALLOC_GROW(snap->ref, snap->nr + 1, snap->alloc);
	snap->ref[snap->nr].refname = xstrdup(ref->name);
	oidcpy(&snap->ref[snap->nr].oid, ref->oid);
	snap->nr++;

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା fsck_handle_ref(ଅଚଳ ସଂରଚନା reference *ref, ଶୂନ୍ୟ *cb_data)
{
	ସଂରଚନା repository *repo = cb_data;
	ସଂରଚନା object *obj;

	obj = parse_object(repo, ref->oid);
	obj->flags |= USED;
	fsck_put_object_name(&fsck_walk_options,
			     ref->oid, "%s", ref->name);
	mark_object_reachable(obj);

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ snapshot_refs(ସଂରଚନା repository *repo,
			  ସଂରଚନା snapshot *snap, ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv)
{
	ସଂରଚନା refs_for_each_ref_options opts = {
		.flags = REFS_FOR_EACH_INCLUDE_BROKEN,
	};
	ସଂରଚନା snapshot_ref_data data = {
		.repo = repo,
		.snap = snap,
	};
	ସଂରଚନା worktree **worktrees, **p;
	ଅଚଳ ଅକ୍ଷର *head_points_at;
	ସଂରଚନା object_id head_oid;

	ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < argc; i++) {
		ଅଚଳ ଅକ୍ଷର *arg = argv[i];
		ସଂରଚନା object_id oid;
		ଯଦି (!repo_get_oid(repo, arg, &oid)) {
			ସଂରଚନା reference ref = {
				.name = arg,
				.oid = &oid,
			};

			snapshot_ref(&ref, &data);
			ଜାରି;
		}
		error(_("invalid parameter: expected sha1, got '%s'"), arg);
		errors_found |= ERROR_OBJECT;
	}

	ଯଦି (argc) {
		include_reflogs = 0;
		ଫେରନ୍ତୁ;
	}

	refs_for_each_ref_ext(get_main_ref_store(repo),
			      snapshot_ref, &data, &opts);

	worktrees = get_worktrees(repo);
	ପାଇଁ (p = worktrees; *p; p++) {
		ସଂରଚନା worktree *wt = *p;
		ସଂରଚନା strbuf refname = STRBUF_INIT;

		strbuf_worktree_ref(wt, &refname, "HEAD");

		head_points_at = refs_resolve_ref_unsafe(get_main_ref_store(repo),
							 refname.buf, 0, &head_oid, ଖାଲି);

		ଯଦି (head_points_at && !is_null_oid(&head_oid)) {
			ସଂରଚନା reference ref = {
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
	now = time(ଖାଲି);
}


ସ୍ଥିର ଶୂନ୍ୟ free_snapshot_refs(ସଂରଚନା snapshot *snap)
{
	ପାଇଁ (size_t i = 0; i < snap->nr; i++)
		ମୁକ୍ତ(snap->ref[i].refname);
	ମୁକ୍ତ(snap->ref);
}

ସ୍ଥିର ଶୂନ୍ୟ process_refs(ସଂରଚନା repository *repo, ସଂରଚନା snapshot *snap)
{
	ସଂରଚନା worktree **worktrees, **p;

	ପାଇଁ (size_t i = 0; i < snap->nr; i++) {
		ସଂରଚନା reference ref = {
			.name = snap->ref[i].refname,
			.oid = &snap->ref[i].oid,
		};
		fsck_handle_ref(&ref, repo);
	}

	ଯଦି (include_reflogs) {
		worktrees = get_worktrees(repo);
		ପାଇଁ (p = worktrees; *p; p++) {
			ସଂରଚନା worktree *wt = *p;

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
	ଯଦି (!default_refs) {
		fprintf_ln(stderr, _("notice: No default references"));
		show_unreachable = 0;
	}
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା fsck_cache_ବୃକ୍ଷ(ସଂରଚନା repository *repo, ସଂରଚନା cache_ବୃକ୍ଷ *it, ଅଚଳ ଅକ୍ଷର *index_path)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା err = 0;

	ଯଦି (verbose)
		fprintf_ln(stderr, _("Checking cache tree of %s"), index_path);

	ଯଦି (0 <= it->entry_ଗଣନା) {
		ସଂରଚନା object *obj = parse_object(repo, &it->oid);
		ଯଦି (!obj) {
			error(_("%s: invalid sha1 pointer in cache-tree of %s"),
			      oid_to_hex(&it->oid), index_path);
			errors_found |= ERROR_REFS;
			ଫେରନ୍ତୁ 1;
		}
		obj->flags |= USED;
		fsck_put_object_name(&fsck_walk_options, &it->oid, ":");
		mark_object_reachable(obj);
		ଯଦି (obj->type != OBJ_ବୃକ୍ଷ)
			err |= objerror(repo, obj, _("non-tree in cache-tree"));
	}
	ପାଇଁ (i = 0; i < it->subtree_nr; i++)
		err |= fsck_cache_ବୃକ୍ଷ(repo, it->down[i]->cache_ବୃକ୍ଷ, index_path);
	ଫେରନ୍ତୁ err;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା fsck_resolve_undo(ସଂରଚନା index_state *istate,
			     ଅଚଳ ଅକ୍ଷର *index_path)
{
	ସଂରଚନା string_list_item *item;
	ସଂରଚନା string_list *resolve_undo = istate->resolve_undo;

	ଯଦି (!resolve_undo)
		ଫେରନ୍ତୁ 0;

	for_each_string_list_item(item, resolve_undo) {
		ଅଚଳ ଅକ୍ଷର *path = item->string;
		ସଂରଚନା resolve_undo_info *ru = item->util;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

		ଯଦି (!ru)
			ଜାରି;
		ପାଇଁ (i = 0; i < 3; i++) {
			ସଂରଚନା object *obj;

			ଯଦି (!ru->mode[i] || !S_ISREG(ru->mode[i]))
				ଜାରି;

			obj = parse_object(istate->repo, &ru->oid[i]);
			ଯଦି (!obj) {
				error(_("%s: invalid sha1 pointer in resolve-undo of %s"),
				      oid_to_hex(&ru->oid[i]),
				      index_path);
				errors_found |= ERROR_REFS;
				ଜାରି;
			}
			obj->flags |= USED;
			fsck_put_object_name(&fsck_walk_options, &ru->oid[i],
					     ":(%d):%s", i, path);
			mark_object_reachable(obj);
		}
	}
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ fsck_index(ସଂରଚନା index_state *istate, ଅଚଳ ଅକ୍ଷର *index_path,
		       ପୂର୍ଣ୍ଣସଂଖ୍ୟା is_current_worktree)
{
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

	/* TODO: audit for interaction with sparse-index. */
	ensure_full_index(istate);
	ପାଇଁ (i = 0; i < istate->cache_nr; i++) {
		unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା mode;
		ସଂରଚନା blob *blob;
		ସଂରଚନା object *obj;

		mode = istate->cache[i]->ce_mode;
		ଯଦି (S_ISGITLINK(mode))
			ଜାରି;
		blob = lookup_blob(istate->repo,
				   &istate->cache[i]->oid);
		ଯଦି (!blob)
			ଜାରି;
		obj = &blob->object;
		obj->flags |= USED;
		fsck_put_object_name(&fsck_walk_options, &obj->oid,
				     "%s:%s",
				     is_current_worktree ? "" : index_path,
				     istate->cache[i]->name);
		mark_object_reachable(obj);
	}
	ଯଦି (istate->cache_ବୃକ୍ଷ)
		fsck_cache_ବୃକ୍ଷ(istate->repo, istate->cache_ବୃକ୍ଷ, index_path);
	fsck_resolve_undo(istate, index_path);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା mark_object_for_connectivity(ଅଚଳ ସଂରଚନା object_id *oid,
					ସଂରଚନା object_info *oi UNUSED,
					ଶୂନ୍ୟ *cb_data)
{
	ସଂରଚନା repository *repo = cb_data;
	ସଂରଚନା object *obj = lookup_unknown_object(repo, oid);
	obj->flags |= HAS_OBJ;
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ fsck_refs(ସଂରଚନା repository *r)
{
	ସଂରଚନା child_process refs_verify = CHILD_PROCESS_INIT;
	ସଂରଚନା progress *progress = ଖାଲି;

	ଯଦି (show_progress)
		progress = start_progress(r, _("Checking ref database"), 1);

	ଯଦି (verbose)
		fprintf_ln(stderr, _("Checking ref database"));

	child_process_init(&refs_verify);
	refs_verify.git_cmd = 1;
	strvec_pushl(&refs_verify.args, "refs", "verify", ଖାଲି);
	ଯଦି (verbose)
		strvec_ଠେଲ(&refs_verify.args, "--verbose");
	ଯଦି (check_strict)
		strvec_ଠେଲ(&refs_verify.args, "--strict");

	ଯଦି (run_command(&refs_verify))
		errors_found |= ERROR_REFS;

	display_progress(progress, 1);
	stop_progress(&progress);
}

ସ୍ଥିର ଅକ୍ଷର ଅଚଳ * ଅଚଳ fsck_usage[] = {
	N_("git fsck [--tags] [--root] [--unreachable] [--cache] [--no-reflogs]\n"
	   "         [--[no-]full] [--strict] [--verbose] [--lost-found]\n"
	   "         [--[no-]dangling] [--[no-]progress] [--connectivity-only]\n"
	   "         [--[no-]name-objects] [--[no-]references] [<object>...]"),
	ଖାଲି
};

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_fsck(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
	     ଅଚଳ ଅକ୍ଷର **argv,
	     ଅଚଳ ଅକ୍ଷର *prefix,
	     ସଂରଚନା repository *repo)
{
	ସଂରଚନା odb_fsck_options odb_fsck_opts = {
		.flags = ODB_FSCK_FULL,
		.object_cb = fsck_obj_buffer,
		.object_payload = repo,
	};
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା keep_cache_objects = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା name_objects = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା check_references = 1;
	ସଂରଚନା option fsck_opts[] = {
		OPT__VERBOSE(&verbose, N_("be verbose")),
		OPT_BOOL(0, "unreachable", &show_unreachable, N_("show unreachable objects")),
		OPT_BOOL(0, "dangling", &show_dangling, N_("show dangling objects")),
		OPT_BOOL(0, "tags", &show_tags, N_("report tags")),
		OPT_BOOL(0, "root", &show_ମୂଳ, N_("report root nodes")),
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
	ସଂରଚନା snapshot snap = {
		.nr = 0,
		.alloc = 0,
		.ref = ଖାଲି
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
	ଯଦି (check_strict)
		fsck_obj_options.strict = 1;

	ଯଦି (show_progress == -1)
		show_progress = isatty(2);
	ଯଦି (verbose) {
		show_progress = 0;
		odb_fsck_opts.flags |= ODB_FSCK_VERBOSE;
	}
	ଯଦି (show_progress)
		odb_fsck_opts.flags |= ODB_FSCK_PROGRESS;

	ଯଦି (write_lost_and_found) {
		odb_fsck_opts.flags |= ODB_FSCK_FULL;
		include_reflogs = 0;
	}

	ଯଦି (name_objects)
		fsck_enable_object_names(&fsck_walk_options);

	repo_config(repo, git_fsck_config, &fsck_obj_options);
	prepare_repo_settings(repo);

	ଯଦି (check_references)
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

	ଯଦି (connectivity_only) {
		odb_for_each_object(repo->objects, ଖାଲି,
				    mark_object_for_connectivity, repo, 0);
	} ଅନ୍ୟଥା {
		ଯଦି (odb_fsck(repo->objects, &odb_fsck_opts) < 0)
			errors_found |= ERROR_OBJECT;

		ଯଦି (fsck_finish(&fsck_obj_options))
			errors_found |= ERROR_OBJECT;
	}

	/* Process the snapshotted refs and the reflogs. */
	process_refs(repo, &snap);

	/* If not given any explicit objects, process index files too. */
	ଯଦି (!argc)
		keep_cache_objects = 1;
	ଯଦି (keep_cache_objects) {
		/*
		 * TODO: Consider first walking these indexes in snapshot_refs,
		 * to snapshot where the index entries used to point, and then
		 * check those snapshotted locations here.
		 */
		ସଂରଚନା worktree **worktrees, **p;

		verify_index_checksum = 1;
		verify_ce_order = 1;

		worktrees = get_worktrees(repo);
		ପାଇଁ (p = worktrees; *p; p++) {
			ସଂରଚନା worktree *wt = *p;
			ସଂରଚନା index_state istate =
				INDEX_STATE_INIT(repo);
			ଅକ୍ଷର *path, *wt_gitdir;

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
			ମୁକ୍ତ(wt_gitdir);
			ମୁକ୍ତ(path);
		}
		free_worktrees(worktrees);
	}

	check_connectivity(repo);

	ଯଦି (repo->settings.core_commit_graph) {
		ସଂରଚନା odb_source_files *files = odb_source_files_downcast(repo->objects->source);
		ସଂରଚନା child_process commit_graph_verify = CHILD_PROCESS_INIT;

		ପାଇଁ (ସଂରଚନା odb_files_dir *dir = files->dirs; dir; dir = dir->next) {
			child_process_init(&commit_graph_verify);
			commit_graph_verify.git_cmd = 1;
			strvec_pushl(&commit_graph_verify.args, "commit-graph",
				     "verify", "--object-dir", dir->abspath, ଖାଲି);
			ଯଦି (show_progress)
				strvec_ଠେଲ(&commit_graph_verify.args, "--progress");
			ଅନ୍ୟଥା
				strvec_ଠେଲ(&commit_graph_verify.args, "--no-progress");
			ଯଦି (run_command(&commit_graph_verify))
				errors_found |= ERROR_COMMIT_GRAPH;
		}
	}

	free_snapshot_refs(&snap);
	ଫେରନ୍ତୁ errors_found;
}
