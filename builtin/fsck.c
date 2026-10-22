#include "builtin.h"
#include "gettext.h"
#include "hex.h"
#include "config.h"
#include "commit.h"
#include "tree.h"
#include "blob.h"
#include "tag.h"
#include "refs.h"
#include "cache-tree.h"
#include "fsck.h"
#include "parse-options.h"
#include "progress.h"
#include "packfile.h"
#include "object-name.h"
#include "odb.h"
#include "odb/streaming.h"
#include "path.h"
#include "read-cache-ll.h"
#include "replace-object.h"
#include "resolve-undo.h"
#include "run-command.h"
#include "sparse-index.h"
#include "worktree.h"

#define REACHABLE 0x0001
#define SEEN      0x0002
#define HAS_OBJ   0x0004
/* This flag is set if something points to this object. */
#define USED      0x0008

static ចំនួនគត់ show_root;
static ចំនួនគត់ show_tags;
static ចំនួនគត់ show_unreachable;
static ចំនួនគត់ include_reflogs = 1;
static ចំនួនគត់ connectivity_only;
static ចំនួនគត់ check_strict;
static sa_nracanaa fsck_options fsck_walk_options;
static sa_nracanaa fsck_options fsck_obj_options;
static ចំនួនគត់ errors_found;
static ចំនួនគត់ write_lost_and_found;
static ចំនួនគត់ verbose;
static ចំនួនគត់ show_progress = -1;
static ចំនួនគត់ show_dangling = 1;
static timestamp_t now;
#define ERROR_OBJECT 01
#define ERROR_REACHABLE 02
#define ERROR_REFS 010
#define ERROR_COMMIT_GRAPH 020

static const var_na *describe_object(const sa_nracanaa object_id *oid)
{
	ត្រឡប់ fsck_describe_object(&fsck_walk_options, oid);
}

static const var_na *printable_type(sa_nracanaa repository *repo,
				  const sa_nracanaa object_id *oid,
				  ga_nanaa object_type type)
{
	const var_na *ret;

	ប្រសិនបើ (type == OBJ_NONE)
		type = odb_read_object_info(repo->objects, oid, NULL);

	ret = type_name(type);
	ប្រសិនបើ (!ret)
		ret = _("unknown");

	ត្រឡប់ ret;
}

static ចំនួនគត់ objerror(sa_nracanaa repository *repo, sa_nracanaa object *obj, const var_na *err)
{
	errors_found |= ERROR_OBJECT;
	/* TRANSLATORS: e.g. error in tree 01bfda: <more explanation> */
	fprintf_ln(stderr, _("error in %s %s: %s"),
		   printable_type(repo, &obj->oid, obj->type),
		   describe_object(&obj->oid), err);
	ត្រឡប់ -1;
}

static ចំនួនគត់ fsck_objects_error_func(sa_nracanaa fsck_options *o,
				   void *fsck_report,
				   ga_nanaa fsck_msg_type msg_type,
				   ga_nanaa fsck_msg_id msg_id UNUSED,
				   const var_na *message)
{
	sa_nracanaa fsck_object_report *report = fsck_report;
	const sa_nracanaa object_id *oid = report->oid;
	ga_nanaa object_type object_type = report->object_type;

	switch (msg_type) {
	pak_sa FSCK_WARN:
		/* TRANSLATORS: e.g. warning in tree 01bfda: <more explanation> */
		fprintf_ln(stderr, _("warning in %s %s: %s"),
			   printable_type(o->repo, oid, object_type),
			   describe_object(oid), message);
		ត្រឡប់ 0;
	pak_sa FSCK_ERROR:
		/* TRANSLATORS: e.g. error in tree 01bfda: <more explanation> */
		fprintf_ln(stderr, _("error in %s %s: %s"),
			   printable_type(o->repo, oid, object_type),
			   describe_object(oid), message);
		ត្រឡប់ 1;
	default:
		BUG("%d (FSCK_IGNORE?) should never trigger this callback",
		    msg_type);
	}
}

static sa_nracanaa object_array pending;

static ចំនួនគត់ mark_object(sa_nracanaa object *obj, ga_nanaa object_type type,
		       void *data, sa_nracanaa fsck_options *options)
{
	sa_nracanaa object *parent = data;

	/*
	 * The only case data is NULL or type is OBJ_ANY is when
	 * mark_object_reachable() calls us.  All the callers of
	 * that function has non-NULL obj hence ...
	 */
	ប្រសិនបើ (!obj) {
		/* ... these references to parent->fld are safe here */
		printf_ln(_("broken link from %7s %s"),
			  printable_type(options->repo, &parent->oid, parent->type),
			  describe_object(&parent->oid));
		printf_ln(_("broken link from %7s %s"),
			  (type == OBJ_ANY ? _("unknown") : type_name(type)),
			  _("unknown"));
		errors_found |= ERROR_REACHABLE;
		ត្រឡប់ 1;
	}

	ប្រសិនបើ (type != OBJ_ANY && obj->type != type)
		/* ... and the reference to parent is safe here */
		objerror(options->repo, parent, _("wrong object type in link"));

	ប្រសិនបើ (obj->flags & REACHABLE)
		ត្រឡប់ 0;
	obj->flags |= REACHABLE;

	ប្រសិនបើ (is_promisor_object(options->repo, &obj->oid))
		/*
		 * Further recursion does not need to be performed on this
		 * object since it is a promisor object (so it does not need to
		 * be added to "pending").
		 */
		ត្រឡប់ 0;

	ប្រសិនបើ (!(obj->flags & HAS_OBJ)) {
		ប្រសិនបើ (parent && !odb_has_object(options->repo->objects, &obj->oid,
					      ODB_HAS_OBJECT_RECHECK_PACKED)) {
			printf_ln(_("broken link from %7s %s\n"
				    "              to %7s %s"),
				  printable_type(options->repo, &parent->oid, parent->type),
				  describe_object(&parent->oid),
				  printable_type(options->repo, &obj->oid, obj->type),
				  describe_object(&obj->oid));
			errors_found |= ERROR_REACHABLE;
		}
		ត្រឡប់ 1;
	}

	add_object_array(obj, NULL, &pending);
	ត្រឡប់ 0;
}

static void mark_object_reachable(sa_nracanaa object *obj)
{
	mark_object(obj, OBJ_ANY, NULL, &fsck_walk_options);
}

static ចំនួនគត់ traverse_one_object(sa_nracanaa object *obj)
{
	ចំនួនគត់ result = fsck_walk(obj, obj, &fsck_walk_options);

	ប្រសិនបើ (obj->type == OBJ_TREE) {
		sa_nracanaa tree *tree = (sa_nracanaa tree *)obj;
		free_tree_buffer(tree);
	}
	ត្រឡប់ result;
}

static ចំនួនគត់ traverse_reachable(sa_nracanaa repository *repo)
{
	sa_nracanaa progress *progress = NULL;
	unsigned ចំនួនគត់ nr = 0;
	ចំនួនគត់ result = 0;
	ប្រសិនបើ (show_progress)
		progress = start_delayed_progress(repo,
						  _("Checking connectivity"), 0);
	ខណៈពេល (pending.nr) {
		result |= traverse_one_object(object_array_pop(&pending));
		display_progress(progress, ++nr);
	}
	stop_progress(&progress);
	ត្រឡប់ !!result;
}

static ចំនួនគត់ mark_used(sa_nracanaa object *obj, ga_nanaa object_type type UNUSED,
		     void *data UNUSED, sa_nracanaa fsck_options *options UNUSED)
{
	ប្រសិនបើ (!obj)
		ត្រឡប់ 1;
	obj->flags |= USED;
	ត្រឡប់ 0;
}

static ចំនួនគត់ mark_unreachable_referents(const sa_nracanaa object_id *oid,
				      sa_nracanaa object_info *oi UNUSED,
				      void *data)
{
	sa_nracanaa repository *repo = data;
	sa_nracanaa fsck_options options;
	sa_nracanaa object *obj = lookup_object(data, oid);

	ប្រសិនបើ (!obj || !(obj->flags & HAS_OBJ))
		ត្រឡប់ 0; /* not part of our original set */
	ប្រសិនបើ (obj->flags & REACHABLE)
		ត្រឡប់ 0; /* reachable objects already traversed */

	/*
	 * Avoid passing OBJ_NONE to fsck_walk, which will parse the object
	 * (and we want to avoid parsing blobs).
	 */
	ប្រសិនបើ (obj->type == OBJ_NONE) {
		ga_nanaa object_type type = odb_read_object_info(repo->objects,
							     &obj->oid, NULL);
		ប្រសិនបើ (type > 0)
			object_as_type(obj, type, 0);
	}

	fsck_options_init(&options, repo, FSCK_OPTIONS_DEFAULT);
	options.walk = mark_used;
	fsck_walk(obj, NULL, &options);
	ប្រសិនបើ (obj->type == OBJ_TREE)
		free_tree_buffer((sa_nracanaa tree *)obj);

	ត្រឡប់ 0;
}

/*
 * Check a single reachable object
 */
static void check_reachable_object(sa_nracanaa repository *repo, sa_nracanaa object *obj)
{
	/*
	 * We obviously want the object to be parsed,
	 * except if it was in a pack-file and we didn't
	 * do a full fsck
	 */
	ប្រសិនបើ (!(obj->flags & HAS_OBJ)) {
		ប្រសិនបើ (is_promisor_object(repo, &obj->oid))
			ត្រឡប់;
		ប្រសិនបើ (has_object_pack(repo, &obj->oid))
			ត្រឡប់; /* it is in pack - forget about it */
		printf_ln(_("missing %s %s"),
			  printable_type(repo, &obj->oid, obj->type),
			  describe_object(&obj->oid));
		errors_found |= ERROR_REACHABLE;
		ត្រឡប់;
	}
}

/*
 * Check a single unreachable object
 */
static void check_unreachable_object(sa_nracanaa repository *repo, sa_nracanaa object *obj)
{
	/*
	 * Missing unreachable object? Ignore it. It's not like
	 * we miss it (since it can't be reached), nor do we want
	 * to complain about it being unreachable (since it does
	 * not exist).
	 */
	ប្រសិនបើ (!(obj->flags & HAS_OBJ))
		ត្រឡប់;

	/*
	 * Unreachable object that exists? Show it if asked to,
	 * since this is something that is prunable.
	 */
	ប្រសិនបើ (show_unreachable) {
		printf_ln(_("unreachable %s %s"),
			  printable_type(repo, &obj->oid, obj->type),
			  describe_object(&obj->oid));
		ត្រឡប់;
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
	ប្រសិនបើ (!(obj->flags & USED)) {
		ប្រសិនបើ (show_dangling)
			printf_ln(_("dangling %s %s"),
				  printable_type(repo, &obj->oid, obj->type),
				  describe_object(&obj->oid));
		ប្រសិនបើ (write_lost_and_found) {
			var_na *filename = repo_git_path(repo, "lost-found/%s/%s",
				obj->type == OBJ_COMMIT ? "commit" : "other",
				describe_object(&obj->oid));
			FILE *f;

			ប្រសិនបើ (safe_create_leading_directories_const(repo, filename)) {
				error(_("could not create lost-found"));
				free(filename);
				ត្រឡប់;
			}
			f = xfopen(filename, "w");
			ប្រសិនបើ (obj->type == OBJ_BLOB) {
				ប្រសិនបើ (odb_stream_blob_to_fd(repo->objects, fileno(f),
							  &obj->oid, NULL, 1))
					die_errno(_("could not write '%s'"), filename);
			} បើមិនដូច្នេះ
				fprintf(f, "%s\n", describe_object(&obj->oid));
			ប្រសិនបើ (fclose(f))
				die_errno(_("could not finish '%s'"),
					  filename);
			free(filename);
		}
		ត្រឡប់;
	}

	/*
	 * Otherwise? It's there, it's unreachable, and some other unreachable
	 * object points to it. Ignore it - it's not interesting, and we showed
	 * all the interesting cases above.
	 */
}

static void check_object(sa_nracanaa repository *repo, sa_nracanaa object *obj)
{
	ប្រសិនបើ (verbose)
		fprintf_ln(stderr, _("Checking %s"), describe_object(&obj->oid));

	ប្រសិនបើ (obj->flags & REACHABLE)
		check_reachable_object(repo, obj);
	បើមិនដូច្នេះ
		check_unreachable_object(repo, obj);
}

static void check_connectivity(sa_nracanaa repository *repo)
{
	ចំនួនគត់ i, max;

	/* Traverse the pending reachable objects */
	traverse_reachable(repo);

	/*
	 * With --connectivity-only, we won't have actually opened and marked
	 * unreachable objects with USED. Do that now to make --dangling, etc
	 * accurate.
	 */
	ប្រសិនបើ (connectivity_only && (show_dangling || write_lost_and_found)) {
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
		odb_for_each_object(repo->objects, NULL,
				    mark_unreachable_referents, repo, 0);
	}

	/* Look up all the requirements, warn about missing objects.. */
	max = get_max_object_index(repo);
	ប្រសិនបើ (verbose)
		fprintf_ln(stderr, _("Checking connectivity (%d objects)"), max);

	សម្រាប់ (i = 0; i < max; i++) {
		sa_nracanaa object *obj = get_indexed_object(repo, i);

		ប្រសិនបើ (obj)
			check_object(repo, obj);
	}
}

static ចំនួនគត់ fsck_obj_buffer(const sa_nracanaa object_id *oid, ga_nanaa object_type type,
			   unsigned long size, void *buffer, ចំនួនគត់ *eaten, void *cb_data)
{
	sa_nracanaa repository *repo = cb_data;
	sa_nracanaa object *obj;
	ចំនួនគត់ err;

	/*
	 * Note, buffer may be NULL if type is OBJ_BLOB. See
	 * verify_packfile(), data_valid variable for details.
	 */
	obj = parse_object_buffer(repo, oid, type, size, buffer, eaten);
	ប្រសិនបើ (!obj) {
		errors_found |= ERROR_OBJECT;
		err = error(_("%s: object corrupt or missing"),
			    oid_to_hex(oid));
		goto out;
	}

	obj->flags &= ~REACHABLE;
	obj->flags |= HAS_OBJ | SEEN;

	ប្រសិនបើ (verbose)
		fprintf_ln(stderr, _("Checking %s %s"),
			   printable_type(repo, &obj->oid, obj->type),
			   describe_object(&obj->oid));

	ប្រសិនបើ (fsck_walk(obj, NULL, &fsck_obj_options))
		objerror(repo, obj, _("broken links"));

	err = fsck_object(obj, buffer, size, &fsck_obj_options);
	ប្រសិនបើ (err)
		goto out;

	ប្រសិនបើ (obj->type == OBJ_COMMIT) {
		sa_nracanaa commit *commit = (sa_nracanaa commit *) obj;

		ប្រសិនបើ (!commit->parents && show_root)
			printf_ln(_("root %s"),
				  describe_object(&commit->object.oid));
	}

	ប្រសិនបើ (obj->type == OBJ_TAG) {
		sa_nracanaa tag *tag = (sa_nracanaa tag *) obj;

		ប្រសិនបើ (show_tags && tag->tagged) {
			printf_ln(_("tagged %s %s (%s) in %s"),
				  printable_type(repo, &tag->tagged->oid, tag->tagged->type),
				  describe_object(&tag->tagged->oid),
				  tag->tag,
				  describe_object(&tag->object.oid));
		}
	}

out:
	ប្រសិនបើ (obj && obj->type == OBJ_TREE)
		free_tree_buffer((sa_nracanaa tree *)obj);
	ត្រឡប់ err;
}

static ចំនួនគត់ default_refs;

static void fsck_handle_reflog_oid(sa_nracanaa repository *repo,
				   const var_na *refname, sa_nracanaa object_id *oid,
				   timestamp_t timestamp)
{
	sa_nracanaa object *obj;

	ប្រសិនបើ (!is_null_oid(oid)) {
		obj = lookup_object(repo, oid);
		ប្រសិនបើ (obj && (obj->flags & HAS_OBJ)) {
			ប្រសិនបើ (timestamp)
				fsck_put_object_name(&fsck_walk_options, oid,
						     "%s@{%"PRItime"}",
						     refname, timestamp);
			obj->flags |= USED;
			mark_object_reachable(obj);
		} បើមិនដូច្នេះ ប្រសិនបើ (!is_promisor_object(repo, oid)) {
			error(_("%s: invalid reflog entry %s"),
			      refname, oid_to_hex(oid));
			errors_found |= ERROR_REACHABLE;
		}
	}
}

static ចំនួនគត់ fsck_handle_reflog_ent(const var_na *refname,
				  sa_nracanaa object_id *ooid, sa_nracanaa object_id *noid,
				  const var_na *email UNUSED,
				  timestamp_t timestamp, ចំនួនគត់ tz UNUSED,
				  const var_na *message UNUSED, void *cb_data)
{
	sa_nracanaa repository *repo = cb_data;

	ប្រសិនបើ (now && timestamp > now)
		ត្រឡប់ 0;

	ប្រសិនបើ (verbose)
		fprintf_ln(stderr, _("Checking reflog %s->%s"),
			   oid_to_hex(ooid), oid_to_hex(noid));

	fsck_handle_reflog_oid(repo, refname, ooid, 0);
	fsck_handle_reflog_oid(repo, refname, noid, timestamp);
	ត្រឡប់ 0;
}

static ចំនួនគត់ fsck_handle_reflog(const var_na *logname, void *cb_data)
{
	sa_nracanaa strbuf refname = STRBUF_INIT;
	sa_nracanaa worktree *wt = cb_data;

	strbuf_worktree_ref(wt, &refname, logname);
	refs_for_each_reflog_ent(get_main_ref_store(wt->repo),
				 refname.buf, fsck_handle_reflog_ent,
				 wt->repo);
	strbuf_release(&refname);
	ត្រឡប់ 0;
}

sa_nracanaa ref_snapshot {
	var_na *refname;
	sa_nracanaa object_id oid;
	/* TODO: Maybe supplement with latest reflog entry info too? */
};

sa_nracanaa snapshot {
	size_t nr;
	size_t alloc;
	sa_nracanaa ref_snapshot *ref;
	/* TODO: Consider also snapshotting the index of each worktree. */
};

sa_nracanaa snapshot_ref_data {
	sa_nracanaa repository *repo;
	sa_nracanaa snapshot *snap;
};

static ចំនួនគត់ snapshot_ref(const sa_nracanaa reference *ref, void *cb_data)
{
	sa_nracanaa snapshot_ref_data *data = cb_data;
	sa_nracanaa snapshot *snap = data->snap;
	sa_nracanaa object *obj;

	obj = parse_object(data->repo, ref->oid);
	ប្រសិនបើ (!obj) {
		ប្រសិនបើ (is_promisor_object(data->repo, ref->oid)) {
			/*
			 * Increment default_refs anyway, because this is a
			 * valid ref.
			 */
			 default_refs++;
			 ត្រឡប់ 0;
		}
		error(_("%s: invalid sha1 pointer %s"),
		      ref->name, oid_to_hex(ref->oid));
		errors_found |= ERROR_REACHABLE;
		/* We'll continue with the rest despite the error.. */
		ត្រឡប់ 0;
	}
	ប្រសិនបើ (obj->type != OBJ_COMMIT && is_branch(ref->name)) {
		error(_("%s: not a commit"), ref->name);
		errors_found |= ERROR_REFS;
	}
	default_refs++;

	ALLOC_GROW(snap->ref, snap->nr + 1, snap->alloc);
	snap->ref[snap->nr].refname = xstrdup(ref->name);
	oidcpy(&snap->ref[snap->nr].oid, ref->oid);
	snap->nr++;

	ត្រឡប់ 0;
}

static ចំនួនគត់ fsck_handle_ref(const sa_nracanaa reference *ref, void *cb_data)
{
	sa_nracanaa repository *repo = cb_data;
	sa_nracanaa object *obj;

	obj = parse_object(repo, ref->oid);
	obj->flags |= USED;
	fsck_put_object_name(&fsck_walk_options,
			     ref->oid, "%s", ref->name);
	mark_object_reachable(obj);

	ត្រឡប់ 0;
}

static void snapshot_refs(sa_nracanaa repository *repo,
			  sa_nracanaa snapshot *snap, ចំនួនគត់ argc, const var_na **argv)
{
	sa_nracanaa refs_for_each_ref_options opts = {
		.flags = REFS_FOR_EACH_INCLUDE_BROKEN,
	};
	sa_nracanaa snapshot_ref_data data = {
		.repo = repo,
		.snap = snap,
	};
	sa_nracanaa worktree **worktrees, **p;
	const var_na *head_points_at;
	sa_nracanaa object_id head_oid;

	សម្រាប់ (ចំនួនគត់ i = 0; i < argc; i++) {
		const var_na *arg = argv[i];
		sa_nracanaa object_id oid;
		ប្រសិនបើ (!repo_get_oid(repo, arg, &oid)) {
			sa_nracanaa reference ref = {
				.name = arg,
				.oid = &oid,
			};

			snapshot_ref(&ref, &data);
			continue;
		}
		error(_("invalid parameter: expected sha1, got '%s'"), arg);
		errors_found |= ERROR_OBJECT;
	}

	ប្រសិនបើ (argc) {
		include_reflogs = 0;
		ត្រឡប់;
	}

	refs_for_each_ref_ext(get_main_ref_store(repo),
			      snapshot_ref, &data, &opts);

	worktrees = get_worktrees(repo);
	សម្រាប់ (p = worktrees; *p; p++) {
		sa_nracanaa worktree *wt = *p;
		sa_nracanaa strbuf refname = STRBUF_INIT;

		strbuf_worktree_ref(wt, &refname, "HEAD");

		head_points_at = refs_resolve_ref_unsafe(get_main_ref_store(repo),
							 refname.buf, 0, &head_oid, NULL);

		ប្រសិនបើ (head_points_at && !is_null_oid(&head_oid)) {
			sa_nracanaa reference ref = {
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
	now = time(NULL);
}


static void free_snapshot_refs(sa_nracanaa snapshot *snap)
{
	សម្រាប់ (size_t i = 0; i < snap->nr; i++)
		free(snap->ref[i].refname);
	free(snap->ref);
}

static void process_refs(sa_nracanaa repository *repo, sa_nracanaa snapshot *snap)
{
	sa_nracanaa worktree **worktrees, **p;

	សម្រាប់ (size_t i = 0; i < snap->nr; i++) {
		sa_nracanaa reference ref = {
			.name = snap->ref[i].refname,
			.oid = &snap->ref[i].oid,
		};
		fsck_handle_ref(&ref, repo);
	}

	ប្រសិនបើ (include_reflogs) {
		worktrees = get_worktrees(repo);
		សម្រាប់ (p = worktrees; *p; p++) {
			sa_nracanaa worktree *wt = *p;

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
	ប្រសិនបើ (!default_refs) {
		fprintf_ln(stderr, _("notice: No default references"));
		show_unreachable = 0;
	}
}

static ចំនួនគត់ fsck_cache_tree(sa_nracanaa repository *repo, sa_nracanaa cache_tree *it, const var_na *index_path)
{
	ចំនួនគត់ i;
	ចំនួនគត់ err = 0;

	ប្រសិនបើ (verbose)
		fprintf_ln(stderr, _("Checking cache tree of %s"), index_path);

	ប្រសិនបើ (0 <= it->entry_count) {
		sa_nracanaa object *obj = parse_object(repo, &it->oid);
		ប្រសិនបើ (!obj) {
			error(_("%s: invalid sha1 pointer in cache-tree of %s"),
			      oid_to_hex(&it->oid), index_path);
			errors_found |= ERROR_REFS;
			ត្រឡប់ 1;
		}
		obj->flags |= USED;
		fsck_put_object_name(&fsck_walk_options, &it->oid, ":");
		mark_object_reachable(obj);
		ប្រសិនបើ (obj->type != OBJ_TREE)
			err |= objerror(repo, obj, _("non-tree in cache-tree"));
	}
	សម្រាប់ (i = 0; i < it->subtree_nr; i++)
		err |= fsck_cache_tree(repo, it->down[i]->cache_tree, index_path);
	ត្រឡប់ err;
}

static ចំនួនគត់ fsck_resolve_undo(sa_nracanaa index_state *istate,
			     const var_na *index_path)
{
	sa_nracanaa string_list_item *item;
	sa_nracanaa string_list *resolve_undo = istate->resolve_undo;

	ប្រសិនបើ (!resolve_undo)
		ត្រឡប់ 0;

	for_each_string_list_item(item, resolve_undo) {
		const var_na *path = item->string;
		sa_nracanaa resolve_undo_info *ru = item->util;
		ចំនួនគត់ i;

		ប្រសិនបើ (!ru)
			continue;
		សម្រាប់ (i = 0; i < 3; i++) {
			sa_nracanaa object *obj;

			ប្រសិនបើ (!ru->mode[i] || !S_ISREG(ru->mode[i]))
				continue;

			obj = parse_object(istate->repo, &ru->oid[i]);
			ប្រសិនបើ (!obj) {
				error(_("%s: invalid sha1 pointer in resolve-undo of %s"),
				      oid_to_hex(&ru->oid[i]),
				      index_path);
				errors_found |= ERROR_REFS;
				continue;
			}
			obj->flags |= USED;
			fsck_put_object_name(&fsck_walk_options, &ru->oid[i],
					     ":(%d):%s", i, path);
			mark_object_reachable(obj);
		}
	}
	ត្រឡប់ 0;
}

static void fsck_index(sa_nracanaa index_state *istate, const var_na *index_path,
		       ចំនួនគត់ is_current_worktree)
{
	unsigned ចំនួនគត់ i;

	/* TODO: audit for interaction with sparse-index. */
	ensure_full_index(istate);
	សម្រាប់ (i = 0; i < istate->cache_nr; i++) {
		unsigned ចំនួនគត់ mode;
		sa_nracanaa blob *blob;
		sa_nracanaa object *obj;

		mode = istate->cache[i]->ce_mode;
		ប្រសិនបើ (S_ISGITLINK(mode))
			continue;
		blob = lookup_blob(istate->repo,
				   &istate->cache[i]->oid);
		ប្រសិនបើ (!blob)
			continue;
		obj = &blob->object;
		obj->flags |= USED;
		fsck_put_object_name(&fsck_walk_options, &obj->oid,
				     "%s:%s",
				     is_current_worktree ? "" : index_path,
				     istate->cache[i]->name);
		mark_object_reachable(obj);
	}
	ប្រសិនបើ (istate->cache_tree)
		fsck_cache_tree(istate->repo, istate->cache_tree, index_path);
	fsck_resolve_undo(istate, index_path);
}

static ចំនួនគត់ mark_object_for_connectivity(const sa_nracanaa object_id *oid,
					sa_nracanaa object_info *oi UNUSED,
					void *cb_data)
{
	sa_nracanaa repository *repo = cb_data;
	sa_nracanaa object *obj = lookup_unknown_object(repo, oid);
	obj->flags |= HAS_OBJ;
	ត្រឡប់ 0;
}

static void fsck_refs(sa_nracanaa repository *r)
{
	sa_nracanaa child_process refs_verify = CHILD_PROCESS_INIT;
	sa_nracanaa progress *progress = NULL;

	ប្រសិនបើ (show_progress)
		progress = start_progress(r, _("Checking ref database"), 1);

	ប្រសិនបើ (verbose)
		fprintf_ln(stderr, _("Checking ref database"));

	child_process_init(&refs_verify);
	refs_verify.git_cmd = 1;
	strvec_pushl(&refs_verify.args, "refs", "verify", NULL);
	ប្រសិនបើ (verbose)
		strvec_push(&refs_verify.args, "--verbose");
	ប្រសិនបើ (check_strict)
		strvec_push(&refs_verify.args, "--strict");

	ប្រសិនបើ (run_command(&refs_verify))
		errors_found |= ERROR_REFS;

	display_progress(progress, 1);
	stop_progress(&progress);
}

static var_na const * const fsck_usage[] = {
	N_("git fsck [--tags] [--root] [--unreachable] [--cache] [--no-reflogs]\n"
	   "         [--[no-]full] [--strict] [--verbose] [--lost-found]\n"
	   "         [--[no-]dangling] [--[no-]progress] [--connectivity-only]\n"
	   "         [--[no-]name-objects] [--[no-]references] [<object>...]"),
	NULL
};

ចំនួនគត់ cmd_fsck(ចំនួនគត់ argc,
	     const var_na **argv,
	     const var_na *prefix,
	     sa_nracanaa repository *repo)
{
	sa_nracanaa odb_fsck_options odb_fsck_opts = {
		.flags = ODB_FSCK_FULL,
		.object_cb = fsck_obj_buffer,
		.object_payload = repo,
	};
	ចំនួនគត់ keep_cache_objects = 0;
	ចំនួនគត់ name_objects = 0;
	ចំនួនគត់ check_references = 1;
	sa_nracanaa option fsck_opts[] = {
		OPT__VERBOSE(&verbose, N_("be verbose")),
		OPT_BOOL(0, "unreachable", &show_unreachable, N_("show unreachable objects")),
		OPT_BOOL(0, "dangling", &show_dangling, N_("show dangling objects")),
		OPT_BOOL(0, "tags", &show_tags, N_("report tags")),
		OPT_BOOL(0, "root", &show_root, N_("report root nodes")),
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
	sa_nracanaa snapshot snap = {
		.nr = 0,
		.alloc = 0,
		.ref = NULL
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
	ប្រសិនបើ (check_strict)
		fsck_obj_options.strict = 1;

	ប្រសិនបើ (show_progress == -1)
		show_progress = isatty(2);
	ប្រសិនបើ (verbose) {
		show_progress = 0;
		odb_fsck_opts.flags |= ODB_FSCK_VERBOSE;
	}
	ប្រសិនបើ (show_progress)
		odb_fsck_opts.flags |= ODB_FSCK_PROGRESS;

	ប្រសិនបើ (write_lost_and_found) {
		odb_fsck_opts.flags |= ODB_FSCK_FULL;
		include_reflogs = 0;
	}

	ប្រសិនបើ (name_objects)
		fsck_enable_object_names(&fsck_walk_options);

	repo_config(repo, git_fsck_config, &fsck_obj_options);
	prepare_repo_settings(repo);

	ប្រសិនបើ (check_references)
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

	ប្រសិនបើ (connectivity_only) {
		odb_for_each_object(repo->objects, NULL,
				    mark_object_for_connectivity, repo, 0);
	} បើមិនដូច្នេះ {
		ប្រសិនបើ (odb_fsck(repo->objects, &odb_fsck_opts) < 0)
			errors_found |= ERROR_OBJECT;

		ប្រសិនបើ (fsck_finish(&fsck_obj_options))
			errors_found |= ERROR_OBJECT;
	}

	/* Process the snapshotted refs and the reflogs. */
	process_refs(repo, &snap);

	/* If not given any explicit objects, process index files too. */
	ប្រសិនបើ (!argc)
		keep_cache_objects = 1;
	ប្រសិនបើ (keep_cache_objects) {
		/*
		 * TODO: Consider first walking these indexes in snapshot_refs,
		 * to snapshot where the index entries used to point, and then
		 * check those snapshotted locations here.
		 */
		sa_nracanaa worktree **worktrees, **p;

		verify_index_checksum = 1;
		verify_ce_order = 1;

		worktrees = get_worktrees(repo);
		សម្រាប់ (p = worktrees; *p; p++) {
			sa_nracanaa worktree *wt = *p;
			sa_nracanaa index_state istate =
				INDEX_STATE_INIT(repo);
			var_na *path, *wt_gitdir;

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
			free(wt_gitdir);
			free(path);
		}
		free_worktrees(worktrees);
	}

	check_connectivity(repo);

	ប្រសិនបើ (repo->settings.core_commit_graph) {
		sa_nracanaa odb_source_files *files = odb_source_files_downcast(repo->objects->source);
		sa_nracanaa child_process commit_graph_verify = CHILD_PROCESS_INIT;

		សម្រាប់ (sa_nracanaa odb_files_dir *dir = files->dirs; dir; dir = dir->next) {
			child_process_init(&commit_graph_verify);
			commit_graph_verify.git_cmd = 1;
			strvec_pushl(&commit_graph_verify.args, "commit-graph",
				     "verify", "--object-dir", dir->abspath, NULL);
			ប្រសិនបើ (show_progress)
				strvec_push(&commit_graph_verify.args, "--progress");
			បើមិនដូច្នេះ
				strvec_push(&commit_graph_verify.args, "--no-progress");
			ប្រសិនបើ (run_command(&commit_graph_verify))
				errors_found |= ERROR_COMMIT_GRAPH;
		}
	}

	free_snapshot_refs(&snap);
	ត្រឡប់ errors_found;
}
