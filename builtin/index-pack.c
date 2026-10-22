#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE
#ವ್ಯಾಖ್ಯಾನಿಸು DISABLE_SIGN_COMPARE_WARNINGS

#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "delta.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "hex.h"
#ಅನ್ತರ್ಭಾವ "pack.h"
#ಅನ್ತರ್ಭಾವ "csum-file.h"
#ಅನ್ತರ್ಭಾವ "blob.h"
#ಅನ್ತರ್ಭಾವ "commit.h"
#ಅನ್ತರ್ಭಾವ "tag.h"
#ಅನ್ತರ್ಭಾವ "tree.h"
#ಅನ್ತರ್ಭಾವ "progress.h"
#ಅನ್ತರ್ಭಾವ "fsck.h"
#ಅನ್ತರ್ಭಾವ "strbuf.h"
#ಅನ್ತರ್ಭಾವ "thread-utils.h"
#ಅನ್ತರ್ಭಾವ "packfile.h"
#ಅನ್ತರ್ಭಾವ "pack-revindex.h"
#ಅನ್ತರ್ಭಾವ "object-file.h"
#ಅನ್ತರ್ಭಾವ "odb.h"
#ಅನ್ತರ್ಭಾವ "odb/streaming.h"
#ಅನ್ತರ್ಭಾವ "oid-array.h"
#ಅನ್ತರ್ಭಾವ "oidset.h"
#ಅನ್ತರ್ಭಾವ "path.h"
#ಅನ್ತರ್ಭಾವ "replace-object.h"
#ಅನ್ತರ್ಭಾವ "tree-walk.h"
#ಅನ್ತರ್ಭಾವ "promisor-remote.h"
#ಅನ್ತರ್ಭಾವ "run-command.h"
#ಅನ್ತರ್ಭಾವ "setup.h"
#ಅನ್ತರ್ಭಾವ "strvec.h"

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ ಸೂಚ್ಯಂಕ_pack_usage[] =
"git index-pack [-v] [-o <index-file>] [--keep | --keep=<msg>] [--[no-]rev-index] [--verify] [--strict[=<msg-id>=<severity>...]] [--fsck-objects[=<msg-id>=<severity>...]] (<pack-file> | --stdin [--fix-thin] [<pack-file>])";

ರಚನೆ object_ನಮೂದು {
	ರಚನೆ pack_idx_ನಮೂದು idx;
	ಗಾತ್ರ_t ಗಾತ್ರ_2;
	ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ hdr_ಗಾತ್ರ;
	ಚಿಹ್ನೆಯುಳ್ಳ ಅಕ್ಷರ type;
	ಚಿಹ್ನೆಯುಳ್ಳ ಅಕ್ಷರ real_type;
};

ರಚನೆ object_stat {
	ಚಿಹ್ನೆರಹಿತ delta_ಆಳ;
	ಪೂರ್ಣಾಂಕ ತಳ_object_no;
};

ರಚನೆ ತಳ_ದತ್ತಾಂಶ {
	/* Initialized by make_base(). */
	ರಚನೆ ತಳ_ದತ್ತಾಂಶ *ತಳ;
	ರಚನೆ object_ನಮೂದು *obj;
	ಪೂರ್ಣಾಂಕ ref_ಮೊದಲ, ref_ಕೊನೆಯ;
	ಪೂರ್ಣಾಂಕ ofs_ಮೊದಲ, ofs_ಕೊನೆಯ;
	/*
	 * Threads should increment retain_data if they are about to call
	 * patch_delta() using this struct's data as a base, and decrement this
	 * when they are done. While retain_data is nonzero, this struct's data
	 * will not be freed even if the delta base cache limit is exceeded.
	 */
	ಪೂರ್ಣಾಂಕ retain_ದತ್ತಾಂಶ;
	/*
	 * The number of direct children that have not been fully processed
	 * (entered work_head, entered done_head, left done_head). When this
	 * number reaches zero, this struct base_data can be freed.
	 */
	ಪೂರ್ಣಾಂಕ children_ಉಳಿದ;

	/* Not initialized by make_base(). */
	ರಚನೆ list_ತಲೆ list;
	ಶೂನ್ಯ *ದತ್ತಾಂಶ;
	ಗಾತ್ರ_t ಗಾತ್ರ_2;
};

/*
 * Stack of struct base_data that have unprocessed children.
 * threaded_second_pass() uses this as a source of work (the other being the
 * objects array).
 *
 * Guarded by work_mutex.
 */
ಸ್ಥಿರ LIST_ತಲೆ(work_ತಲೆ);

/*
 * Stack of struct base_data that have children, all of whom have been
 * processed or are being processed, and at least one child is being processed.
 * These struct base_data must be kept around until the last child is
 * processed.
 *
 * Guarded by work_mutex.
 */
ಸ್ಥಿರ LIST_ತಲೆ(ಮುಗಿದ_ತಲೆ);

/*
 * All threads share one delta base cache.
 *
 * base_cache_used is guarded by work_mutex, and base_cache_limit is read-only
 * in a thread.
 */
ಸ್ಥಿರ ಗಾತ್ರ_t ತಳ_cache_ಬಳಸಿದ;
ಸ್ಥಿರ ಗಾತ್ರ_t ತಳ_cache_ಎಲ್ಲೆ;

ರಚನೆ thread_local_ದತ್ತಾಂಶ {
	pthread_t thread;
	ಪೂರ್ಣಾಂಕ pack_fd;
};

/* Remember to update object flag allocation in object.h */
#ವ್ಯಾಖ್ಯಾನಿಸು FLAG_LINK (1u<<20)
#ವ್ಯಾಖ್ಯಾನಿಸು FLAG_CHECKED (1u<<21)

ರಚನೆ ofs_delta_ನಮೂದು {
	off_t ಸ್ಥಾನಾಂತರ;
	ಪೂರ್ಣಾಂಕ obj_no;
};

ರಚನೆ ref_delta_ನಮೂದು {
	ರಚನೆ object_id oid;
	ಪೂರ್ಣಾಂಕ obj_no;
};

ಸ್ಥಿರ ರಚನೆ object_ನಮೂದು *objects;
ಸ್ಥಿರ ರಚನೆ object_stat *obj_stat;
ಸ್ಥಿರ ರಚನೆ ofs_delta_ನಮೂದು *ofs_deltas;
ಸ್ಥಿರ ರಚನೆ ref_delta_ನಮೂದು *ref_deltas;
ಸ್ಥಿರ ರಚನೆ thread_local_ದತ್ತಾಂಶ nothread_ದತ್ತಾಂಶ;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ nr_objects;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ nr_ofs_deltas;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ nr_ref_deltas;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ref_deltas_alloc;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ nr_resolved_deltas;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ nr_ಎಳೆಗಳು;

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ from_stdin;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ strict;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ do_fsck_object;
ಸ್ಥಿರ ರಚನೆ fsck_options fsck_options;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ verbose;
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪ್ರಗತಿ_title;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ show_resolving_ಪ್ರಗತಿ;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ show_stat;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ check_self_contained_and_connected;

ಸ್ಥಿರ ರಚನೆ ಪ್ರಗತಿ *ಪ್ರಗತಿ;

ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ ಒಳಹರಿವು_ಬಫರ್[DEFAULT_IO_ಬಫರ್_ಗಾತ್ರ];
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಒಳಹರಿವು_ಸ್ಥಾನಾಂತರ, ಒಳಹರಿವು_len;
ಸ್ಥಿರ off_t consumed_ಬೈಟುಗಳು;
ಸ್ಥಿರ off_t max_ಒಳಹರಿವು_ಗಾತ್ರ;
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಅತ್ಯಂತಆಳ_delta;
ಸ್ಥಿರ ರಚನೆ git_hash_ctx ಒಳಹರಿವು_ctx;
ಸ್ಥಿರ uint32_t ಒಳಹರಿವು_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_32;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಒಳಹರಿವು_fd, output_fd;
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *curr_pack;

/*
 * outgoing_links is guarded by read_mutex, and record_outgoing_links is
 * read-only in a thread.
 */
ಸ್ಥಿರ ರಚನೆ oidset outgoing_links = OIDSET_INIT;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ದಾಖಲೆ_outgoing_links;

ಸ್ಥಿರ ರಚನೆ thread_local_ದತ್ತಾಂಶ *thread_ದತ್ತಾಂಶ;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ nr_dispatched;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಎಳೆಗಳು_ಸಕ್ರಿಯ;

ಸ್ಥಿರ pthread_mutex_t read_mutex;
#ವ್ಯಾಖ್ಯಾನಿಸು read_ಬೀಗ()		ಬೀಗ_mutex(&read_mutex)
#ವ್ಯಾಖ್ಯಾನಿಸು read_unlock()		unlock_mutex(&read_mutex)

ಸ್ಥಿರ pthread_mutex_t counter_mutex;
#ವ್ಯಾಖ್ಯಾನಿಸು counter_ಬೀಗ()		ಬೀಗ_mutex(&counter_mutex)
#ವ್ಯಾಖ್ಯಾನಿಸು counter_unlock()	unlock_mutex(&counter_mutex)

ಸ್ಥಿರ pthread_mutex_t work_mutex;
#ವ್ಯಾಖ್ಯಾನಿಸು work_ಬೀಗ()		ಬೀಗ_mutex(&work_mutex)
#ವ್ಯಾಖ್ಯಾನಿಸು work_unlock()		unlock_mutex(&work_mutex)

ಸ್ಥಿರ pthread_mutex_t ಅತ್ಯಂತಆಳ_delta_mutex;
#ವ್ಯಾಖ್ಯಾನಿಸು ಅತ್ಯಂತಆಳ_delta_ಬೀಗ()	ಬೀಗ_mutex(&ಅತ್ಯಂತಆಳ_delta_mutex)
#ವ್ಯಾಖ್ಯಾನಿಸು ಅತ್ಯಂತಆಳ_delta_unlock()	unlock_mutex(&ಅತ್ಯಂತಆಳ_delta_mutex)

ಸ್ಥಿರ pthread_ಕೀಲಿ_t ಕೀಲಿ;

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ ಬೀಗ_mutex(pthread_mutex_t *mutex)
{
	ಆದರೆ (ಎಳೆಗಳು_ಸಕ್ರಿಯ)
		pthread_mutex_ಬೀಗ(mutex);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ unlock_mutex(pthread_mutex_t *mutex)
{
	ಆದರೆ (ಎಳೆಗಳು_ಸಕ್ರಿಯ)
		pthread_mutex_unlock(mutex);
}

/*
 * Mutex and conditional variable can't be statically-initialized on Windows.
 */
ಸ್ಥಿರ ಶೂನ್ಯ init_thread(ಶೂನ್ಯ)
{
	ಪೂರ್ಣಾಂಕ i;
	init_recursive_mutex(&read_mutex);
	pthread_mutex_init(&counter_mutex, ಖಾಲಿ);
	pthread_mutex_init(&work_mutex, ಖಾಲಿ);
	ಆದರೆ (show_stat)
		pthread_mutex_init(&ಅತ್ಯಂತಆಳ_delta_mutex, ಖಾಲಿ);
	pthread_ಕೀಲಿ_create(&ಕೀಲಿ, ಖಾಲಿ);
	CALLOC_ARRAY(thread_ದತ್ತಾಂಶ, nr_ಎಳೆಗಳು);
	ಗಾಗಿ (i = 0; i < nr_ಎಳೆಗಳು; i++) {
		thread_ದತ್ತಾಂಶ[i].pack_fd = xopen(curr_pack, O_RDONLY);
	}

	ಎಳೆಗಳು_ಸಕ್ರಿಯ = 1;
}

ಸ್ಥಿರ ಶೂನ್ಯ cleanup_thread(ಶೂನ್ಯ)
{
	ಪೂರ್ಣಾಂಕ i;
	ಆದರೆ (!ಎಳೆಗಳು_ಸಕ್ರಿಯ)
		ಹಿಂತಿರುಗಿ;
	ಎಳೆಗಳು_ಸಕ್ರಿಯ = 0;
	pthread_mutex_destroy(&read_mutex);
	pthread_mutex_destroy(&counter_mutex);
	pthread_mutex_destroy(&work_mutex);
	ಆದರೆ (show_stat)
		pthread_mutex_destroy(&ಅತ್ಯಂತಆಳ_delta_mutex);
	ಗಾಗಿ (i = 0; i < nr_ಎಳೆಗಳು; i++)
		close(thread_ದತ್ತಾಂಶ[i].pack_fd);
	pthread_ಕೀಲಿ_delete(ಕೀಲಿ);
	ಬಿಡುಗಡೆ(thread_ದತ್ತಾಂಶ);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ mark_link(ರಚನೆ object *obj, ಎಣಿಕೆ object_type type,
		     ಶೂನ್ಯ *ದತ್ತಾಂಶ ಬಳಸದ,
		     ರಚನೆ fsck_options *options ಬಳಸದ)
{
	ಆದರೆ (!obj)
		ಹಿಂತಿರುಗಿ -1;

	ಆದರೆ (type != OBJ_ANY && obj->type != type)
		die(_("object type mismatch at %s"), oid_ಗಮ್ಯ_hex(&obj->oid));

	obj->flags |= FLAG_LINK;
	ಹಿಂತಿರುಗಿ 0;
}

/* The content of each linked object must have been checked
   or it must be already present in the object database */
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ check_object(ರಚನೆ object *obj)
{
	ಆದರೆ (!obj)
		ಹಿಂತಿರುಗಿ 0;

	ಆದರೆ (!(obj->flags & FLAG_LINK))
		ಹಿಂತಿರುಗಿ 0;

	ಆದರೆ (!(obj->flags & FLAG_CHECKED)) {
		ಗಾತ್ರ_t ಗಾತ್ರ_2;
		ಪೂರ್ಣಾಂಕ type = odb_read_object_info(the_repository->objects,
						&obj->oid, &ಗಾತ್ರ_2);
		ಆದರೆ (type <= 0)
			die(_("did not receive expected object %s"),
			      oid_ಗಮ್ಯ_hex(&obj->oid));
		ಆದರೆ (type != obj->type)
			die(_("object %s: expected type %s, found %s"),
			    oid_ಗಮ್ಯ_hex(&obj->oid),
			    type_name(obj->type), type_name(type));
		obj->flags |= FLAG_CHECKED;
		ಹಿಂತಿರುಗಿ 1;
	}

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ check_objects(ಶೂನ್ಯ)
{
	ಚಿಹ್ನೆರಹಿತ i, max, foreign_nr = 0;

	max = get_max_object_ಸೂಚ್ಯಂಕ(the_repository);

	ಆದರೆ (verbose)
		ಪ್ರಗತಿ = ಆರಂಭ_delayed_ಪ್ರಗತಿ(the_repository,
						  _("Checking objects"), max);

	ಗಾಗಿ (i = 0; i < max; i++) {
		foreign_nr += check_object(get_ಸೂಚ್ಯಂಕಿತ_object(the_repository, i));
		display_ಪ್ರಗತಿ(ಪ್ರಗತಿ, i + 1);
	}

	stop_ಪ್ರಗತಿ(&ಪ್ರಗತಿ);
	ಹಿಂತಿರುಗಿ foreign_nr;
}


/* Discard current buffer used content. */
ಸ್ಥಿರ ಶೂನ್ಯ flush(ಶೂನ್ಯ)
{
	ಆದರೆ (ಒಳಹರಿವು_ಸ್ಥಾನಾಂತರ) {
		ಆದರೆ (output_fd >= 0)
			write_or_die(output_fd, ಒಳಹರಿವು_ಬಫರ್, ಒಳಹರಿವು_ಸ್ಥಾನಾಂತರ);
		git_hash_update(&ಒಳಹರಿವು_ctx, ಒಳಹರಿವು_ಬಫರ್, ಒಳಹರಿವು_ಸ್ಥಾನಾಂತರ);
		memmove(ಒಳಹರಿವು_ಬಫರ್, ಒಳಹರಿವು_ಬಫರ್ + ಒಳಹರಿವು_ಸ್ಥಾನಾಂತರ, ಒಳಹರಿವು_len);
		ಒಳಹರಿವು_ಸ್ಥಾನಾಂತರ = 0;
	}
}

/*
 * Make sure at least "min" bytes are available in the buffer, and
 * return the pointer to the buffer.
 */
ಸ್ಥಿರ ಶೂನ್ಯ *fill(ಪೂರ್ಣಾಂಕ min)
{
	ಆದರೆ (min <= ಒಳಹರಿವು_len)
		ಹಿಂತಿರುಗಿ ಒಳಹರಿವು_ಬಫರ್ + ಒಳಹರಿವು_ಸ್ಥಾನಾಂತರ;
	ಆದರೆ (min > ಗಾತ್ರ(ಒಳಹರಿವು_ಬಫರ್))
		die(Q_("cannot fill %d byte",
		       "cannot fill %d bytes",
		       min),
		    min);
	flush();
	ಮಾಡು {
		ssize_t ret = xread(ಒಳಹರಿವು_fd, ಒಳಹರಿವು_ಬಫರ್ + ಒಳಹರಿವು_len,
				ಗಾತ್ರ(ಒಳಹರಿವು_ಬಫರ್) - ಒಳಹರಿವು_len);
		ಆದರೆ (ret <= 0) {
			ಆದರೆ (!ret)
				die(_("early EOF"));
			die_errno(_("read error on input"));
		}
		ಒಳಹರಿವು_len += ret;
		ಆದರೆ (from_stdin)
			display_throughput(ಪ್ರಗತಿ, consumed_ಬೈಟುಗಳು + ಒಳಹರಿವು_len);
	} ಆಗ (ಒಳಹರಿವು_len < min);
	ಹಿಂತಿರುಗಿ ಒಳಹರಿವು_ಬಫರ್;
}

ಸ್ಥಿರ ಶೂನ್ಯ use(ಪೂರ್ಣಾಂಕ ಬೈಟುಗಳು)
{
	ಆದರೆ (ಬೈಟುಗಳು > ಒಳಹರಿವು_len)
		die(_("used more bytes than were available"));
	ಒಳಹರಿವು_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_32 = ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_32(ಒಳಹರಿವು_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_32, ಒಳಹರಿವು_ಬಫರ್ + ಒಳಹರಿವು_ಸ್ಥಾನಾಂತರ, ಬೈಟುಗಳು);
	ಒಳಹರಿವು_len -= ಬೈಟುಗಳು;
	ಒಳಹರಿವು_ಸ್ಥಾನಾಂತರ += ಬೈಟುಗಳು;

	/* make sure off_t is sufficiently large not to wrap */
	ಆದರೆ (signed_ಕೂಡಿಸು_overflows(consumed_ಬೈಟುಗಳು, ಬೈಟುಗಳು))
		die(_("pack too large for current definition of off_t"));
	consumed_ಬೈಟುಗಳು += ಬೈಟುಗಳು;
	ಆದರೆ (max_ಒಳಹರಿವು_ಗಾತ್ರ && consumed_ಬೈಟುಗಳು > max_ಒಳಹರಿವು_ಗಾತ್ರ) {
		ರಚನೆ strbuf ಗಾತ್ರ_ಎಲ್ಲೆ = STRBUF_INIT;
		strbuf_humanise_ಬೈಟುಗಳು(&ಗಾತ್ರ_ಎಲ್ಲೆ, max_ಒಳಹರಿವು_ಗಾತ್ರ);
		die(_("pack exceeds maximum allowed size (%s)"),
		    ಗಾತ್ರ_ಎಲ್ಲೆ.buf);
	}
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *open_pack_file(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *pack_name)
{
	ಆದರೆ (from_stdin) {
		ಒಳಹರಿವು_fd = 0;
		ಆದರೆ (!pack_name) {
			ರಚನೆ strbuf tmp_file = STRBUF_INIT;
			output_fd = odb_mkstemp(the_repository->objects, &tmp_file,
						"pack/tmp_pack_XXXXXX");
			pack_name = strbuf_detach(&tmp_file, ಖಾಲಿ);
		} ಇಲ್ಲದಿದ್ದರೆ {
			output_fd = xopen(pack_name, O_CREAT|O_EXCL|O_RDWR, 0600);
		}
		nothread_ದತ್ತಾಂಶ.pack_fd = output_fd;
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಒಳಹರಿವು_fd = xopen(pack_name, O_RDONLY);
		output_fd = -1;
		nothread_ದತ್ತಾಂಶ.pack_fd = ಒಳಹರಿವು_fd;
	}
	git_hash_init(&ಒಳಹರಿವು_ctx, the_hash_algo);
	ಹಿಂತಿರುಗಿ pack_name;
}

ಸ್ಥಿರ ಶೂನ್ಯ parse_pack_ಶೀರ್ಷಿಕೆ(ಶೂನ್ಯ)
{
	ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *hdr = fill(ಗಾತ್ರ(ರಚನೆ pack_ಶೀರ್ಷಿಕೆ));

	/* Header consistency check */
	ಆದರೆ (get_be32(hdr) != PACK_SIGNATURE)
		die(_("pack signature mismatch"));
	hdr += 4;
	ಆದರೆ (!pack_ಆವೃತ್ತಿ_ok_native(get_be32(hdr)))
		die(_("pack version %"PRIu32" unsupported"),
		    get_be32(hdr));
	hdr += 4;

	nr_objects = get_be32(hdr);
	use(ಗಾತ್ರ(ರಚನೆ pack_ಶೀರ್ಷಿಕೆ));
}

__attribute__((format (ಬರೆ, 2, 3)))
ಸ್ಥಿರ NORETURN ಶೂನ್ಯ bad_object(off_t ಸ್ಥಾನಾಂತರ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *format, ...)
{
	va_list params;
	ಅಕ್ಷರ buf[1024];

	va_ಆರಂಭ(params, format);
	vsnprintf(buf, ಗಾತ್ರ(buf), format, params);
	va_ಕೊನೆ(params);
	die(_("pack has bad object at offset %"PRIuMAX": %s"),
	    (uintmax_t)ಸ್ಥಾನಾಂತರ, buf);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ thread_local_ದತ್ತಾಂಶ *get_thread_ದತ್ತಾಂಶ(ಶೂನ್ಯ)
{
	ಆದರೆ (HAVE_ಎಳೆಗಳು) {
		ಆದರೆ (ಎಳೆಗಳು_ಸಕ್ರಿಯ)
			ಹಿಂತಿರುಗಿ pthread_getspecific(ಕೀಲಿ);
		ಪ್ರತಿಜ್ಞಾ(!ಎಳೆಗಳು_ಸಕ್ರಿಯ &&
		       "This should only be reached when all threads are gone");
	}
	ಹಿಂತಿರುಗಿ &nothread_ದತ್ತಾಂಶ;
}

ಸ್ಥಿರ ಶೂನ್ಯ ಗಣ_thread_ದತ್ತಾಂಶ(ರಚನೆ thread_local_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ)
{
	ಆದರೆ (ಎಳೆಗಳು_ಸಕ್ರಿಯ)
		pthread_setspecific(ಕೀಲಿ, ದತ್ತಾಂಶ);
}

ಸ್ಥಿರ ಶೂನ್ಯ free_ತಳ_ದತ್ತಾಂಶ(ರಚನೆ ತಳ_ದತ್ತಾಂಶ *c)
{
	ಆದರೆ (c->ದತ್ತಾಂಶ) {
		FREE_AND_NULL(c->ದತ್ತಾಂಶ);
		ತಳ_cache_ಬಳಸಿದ -= c->ಗಾತ್ರ_2;
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ prune_ತಳ_ದತ್ತಾಂಶ(ರಚನೆ ತಳ_ದತ್ತಾಂಶ *retain)
{
	ರಚನೆ list_ತಲೆ *pos;

	ಆದರೆ (ತಳ_cache_ಬಳಸಿದ <= ತಳ_cache_ಎಲ್ಲೆ)
		ಹಿಂತಿರುಗಿ;

	list_for_each_prev(pos, &ಮುಗಿದ_ತಲೆ) {
		ರಚನೆ ತಳ_ದತ್ತಾಂಶ *b = list_ನಮೂದು(pos, ರಚನೆ ತಳ_ದತ್ತಾಂಶ, list);
		ಆದರೆ (b->retain_ದತ್ತಾಂಶ || b == retain)
			ಮುಂದುವರಿ;
		ಆದರೆ (b->ದತ್ತಾಂಶ) {
			free_ತಳ_ದತ್ತಾಂಶ(b);
			ಆದರೆ (ತಳ_cache_ಬಳಸಿದ <= ತಳ_cache_ಎಲ್ಲೆ)
				ಹಿಂತಿರುಗಿ;
		}
	}

	list_for_each_prev(pos, &work_ತಲೆ) {
		ರಚನೆ ತಳ_ದತ್ತಾಂಶ *b = list_ನಮೂದು(pos, ರಚನೆ ತಳ_ದತ್ತಾಂಶ, list);
		ಆದರೆ (b->retain_ದತ್ತಾಂಶ || b == retain)
			ಮುಂದುವರಿ;
		ಆದರೆ (b->ದತ್ತಾಂಶ) {
			free_ತಳ_ದತ್ತಾಂಶ(b);
			ಆದರೆ (ತಳ_cache_ಬಳಸಿದ <= ತಳ_cache_ಎಲ್ಲೆ)
				ಹಿಂತಿರುಗಿ;
		}
	}
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ is_delta_type(ಎಣಿಕೆ object_type type)
{
	ಹಿಂತಿರುಗಿ (type == OBJ_REF_DELTA || type == OBJ_OFS_DELTA);
}

ಸ್ಥಿರ ಶೂನ್ಯ *unpack_ನಮೂದು_ದತ್ತಾಂಶ(off_t ಸ್ಥಾನಾಂತರ, ಗಾತ್ರ_t ಗಾತ್ರ_2,
			       ಎಣಿಕೆ object_type type, ರಚನೆ object_id *oid)
{
	ಸ್ಥಿರ ಅಕ್ಷರ fixed_buf[8192];
	ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2;
	git_zstream ಹರಿವು;
	ಶೂನ್ಯ *buf;
	ರಚನೆ git_hash_ctx c;
	ಅಕ್ಷರ hdr[32];
	ಪೂರ್ಣಾಂಕ hdrlen;

	ಆದರೆ (!is_delta_type(type)) {
		hdrlen = format_object_ಶೀರ್ಷಿಕೆ(hdr, ಗಾತ್ರ(hdr), type, ಗಾತ್ರ_2);
		git_hash_init(&c, the_hash_algo);
		git_hash_update(&c, hdr, hdrlen);
	} ಇಲ್ಲದಿದ್ದರೆ
		oid = ಖಾಲಿ;
	ಆದರೆ (type == OBJ_BLOB &&
	    ಗಾತ್ರ_2 > repo_settings_get_big_file_ಮಿತಿ(the_repository))
		buf = fixed_buf;
	ಇಲ್ಲದಿದ್ದರೆ
		buf = xmallocz(ಗಾತ್ರ_2);

	memset(&ಹರಿವು, 0, ಗಾತ್ರ(ಹರಿವು));
	git_inflate_init(&ಹರಿವು);
	ಹರಿವು.ಮುಂದಿನ_ಹೊರಹೋಗು = buf;
	ಹರಿವು.avail_ಹೊರಹೋಗು = buf == fixed_buf ? ಗಾತ್ರ(fixed_buf) : ಗಾತ್ರ_2;

	ಮಾಡು {
		ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *ಕೊನೆಯ_ಹೊರಹೋಗು = ಹರಿವು.ಮುಂದಿನ_ಹೊರಹೋಗು;
		ಹರಿವು.ಮುಂದಿನ_ಒಳ = fill(1);
		ಹರಿವು.avail_ಒಳ = ಒಳಹರಿವು_len;
		ಸ್ಥಿತಿ_2 = git_inflate(&ಹರಿವು, 0);
		use(ಒಳಹರಿವು_len - ಹರಿವು.avail_ಒಳ);
		ಆದರೆ (oid)
			git_hash_update(&c, ಕೊನೆಯ_ಹೊರಹೋಗು, ಹರಿವು.ಮುಂದಿನ_ಹೊರಹೋಗು - ಕೊನೆಯ_ಹೊರಹೋಗು);
		ಆದರೆ (buf == fixed_buf) {
			ಹರಿವು.ಮುಂದಿನ_ಹೊರಹೋಗು = buf;
			ಹರಿವು.avail_ಹೊರಹೋಗು = ಗಾತ್ರ(fixed_buf);
		}
	} ಆಗ (ಸ್ಥಿತಿ_2 == Z_OK);
	ಆದರೆ (ಹರಿವು.ಒಟ್ಟು_ಹೊರಹೋಗು != ಗಾತ್ರ_2 || ಸ್ಥಿತಿ_2 != Z_ಹರಿವು_ಕೊನೆ)
		bad_object(ಸ್ಥಾನಾಂತರ, _("inflate returned %d"), ಸ್ಥಿತಿ_2);
	git_inflate_ಕೊನೆ(&ಹರಿವು);
	ಆದರೆ (oid)
		git_hash_final_oid(oid, &c);
	ಹಿಂತಿರುಗಿ buf == fixed_buf ? ಖಾಲಿ : buf;
}

ಸ್ಥಿರ ಶೂನ್ಯ *unpack_raw_ನಮೂದು(ರಚನೆ object_ನಮೂದು *obj,
			      off_t *ofs_ಸ್ಥಾನಾಂತರ,
			      ರಚನೆ object_id *ref_oid,
			      ರಚನೆ object_id *oid)
{
	ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *p;
	ಗಾತ್ರ_t ಗಾತ್ರ_2, c;
	off_t ತಳ_ಸ್ಥಾನಾಂತರ;
	ಚಿಹ್ನೆರಹಿತ ಸರಿಸು;
	ಶೂನ್ಯ *ದತ್ತಾಂಶ;

	obj->idx.ಸ್ಥಾನಾಂತರ = consumed_ಬೈಟುಗಳು;
	ಒಳಹರಿವು_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_32 = ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_32(0, ಖಾಲಿ, 0);

	p = fill(1);
	c = *p;
	use(1);
	obj->type = (c >> 4) & 7;
	ಗಾತ್ರ_2 = (c & 15);
	ಸರಿಸು = 4;
	ಆಗ (c & 0x80) {
		ಆದರೆ ((bitsizeof(ಗಾತ್ರ_t) - 7) < ಸರಿಸು)
			die(_("object size too large for this platform"));
		p = fill(1);
		c = *p;
		use(1);
		ಗಾತ್ರ_2 += (c & 0x7f) << ಸರಿಸು;
		ಸರಿಸು += 7;
	}
	obj->ಗಾತ್ರ_2 = ಗಾತ್ರ_2;

	ಆಯ್ಕೆ (obj->type) {
	ಸ್ಥಿತಿ OBJ_REF_DELTA:
		oidread(ref_oid, fill(the_hash_algo->rawsz),
			the_repository->hash_algo);
		use(the_hash_algo->rawsz);
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ OBJ_OFS_DELTA:
		p = fill(1);
		c = *p;
		use(1);
		ತಳ_ಸ್ಥಾನಾಂತರ = c & 127;
		ಆಗ (c & 128) {
			ತಳ_ಸ್ಥಾನಾಂತರ += 1;
			ಆದರೆ (!ತಳ_ಸ್ಥಾನಾಂತರ || MSB(ತಳ_ಸ್ಥಾನಾಂತರ, 7))
				bad_object(obj->idx.ಸ್ಥಾನಾಂತರ, _("offset value overflow for delta base object"));
			p = fill(1);
			c = *p;
			use(1);
			ತಳ_ಸ್ಥಾನಾಂತರ = (ತಳ_ಸ್ಥಾನಾಂತರ << 7) + (c & 127);
		}
		*ofs_ಸ್ಥಾನಾಂತರ = obj->idx.ಸ್ಥಾನಾಂತರ - ತಳ_ಸ್ಥಾನಾಂತರ;
		ಆದರೆ (*ofs_ಸ್ಥಾನಾಂತರ <= 0 || *ofs_ಸ್ಥಾನಾಂತರ >= obj->idx.ಸ್ಥಾನಾಂತರ)
			bad_object(obj->idx.ಸ್ಥಾನಾಂತರ, _("delta base offset is out of bound"));
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ OBJ_COMMIT:
	ಸ್ಥಿತಿ OBJ_ವೃಕ್ಷ:
	ಸ್ಥಿತಿ OBJ_BLOB:
	ಸ್ಥಿತಿ OBJ_ಟ್ಯಾಗ್:
		ನಿಲ್ಲಿಸು;
	ಪೂರ್ವನಿಯೋಜಿತ:
		bad_object(obj->idx.ಸ್ಥಾನಾಂತರ, _("unknown object type %d"), obj->type);
	}
	obj->hdr_ಗಾತ್ರ = consumed_ಬೈಟುಗಳು - obj->idx.ಸ್ಥಾನಾಂತರ;

	ದತ್ತಾಂಶ = unpack_ನಮೂದು_ದತ್ತಾಂಶ(obj->idx.ಸ್ಥಾನಾಂತರ, obj->ಗಾತ್ರ_2, obj->type, oid);
	obj->idx.ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_32 = ಒಳಹರಿವು_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_32;
	ಹಿಂತಿರುಗಿ ದತ್ತಾಂಶ;
}

ಸ್ಥಿರ ಶೂನ್ಯ *unpack_ದತ್ತಾಂಶ(ರಚನೆ object_ನಮೂದು *obj,
			 ಪೂರ್ಣಾಂಕ (*consume)(ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ, ಶೂನ್ಯ *),
			 ಶೂನ್ಯ *cb_ದತ್ತಾಂಶ)
{
	off_t from = obj[0].idx.ಸ್ಥಾನಾಂತರ + obj[0].hdr_ಗಾತ್ರ;
	off_t len = obj[1].idx.ಸ್ಥಾನಾಂತರ - from;
	ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *ದತ್ತಾಂಶ, *inbuf;
	git_zstream ಹರಿವು;
	ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2;

	ದತ್ತಾಂಶ = xmallocz(consume ? 64*1024 : obj->ಗಾತ್ರ_2);
	inbuf = xmalloc((len < 64*1024) ? (ಪೂರ್ಣಾಂಕ)len : 64*1024);

	memset(&ಹರಿವು, 0, ಗಾತ್ರ(ಹರಿವು));
	git_inflate_init(&ಹರಿವು);
	ಹರಿವು.ಮುಂದಿನ_ಹೊರಹೋಗು = ದತ್ತಾಂಶ;
	ಹರಿವು.avail_ಹೊರಹೋಗು = consume ? 64*1024 : obj->ಗಾತ್ರ_2;

	ಮಾಡು {
		ssize_t n = (len < 64*1024) ? (ssize_t)len : 64*1024;
		n = xpread(get_thread_ದತ್ತಾಂಶ()->pack_fd, inbuf, n, from);
		ಆದರೆ (n < 0)
			die_errno(_("cannot pread pack file"));
		ಆದರೆ (!n)
			die(Q_("premature end of pack file, %"PRIuMAX" byte missing",
			       "premature end of pack file, %"PRIuMAX" bytes missing",
			       len),
			    (uintmax_t)len);
		from += n;
		len -= n;
		ಹರಿವು.ಮುಂದಿನ_ಒಳ = inbuf;
		ಹರಿವು.avail_ಒಳ = n;
		ಆದರೆ (!consume)
			ಸ್ಥಿತಿ_2 = git_inflate(&ಹರಿವು, 0);
		ಇಲ್ಲದಿದ್ದರೆ {
			ಮಾಡು {
				ಸ್ಥಿತಿ_2 = git_inflate(&ಹರಿವು, 0);
				ಆದರೆ (consume(ದತ್ತಾಂಶ, ಹರಿವು.ಮುಂದಿನ_ಹೊರಹೋಗು - ದತ್ತಾಂಶ, cb_ದತ್ತಾಂಶ)) {
					ಬಿಡುಗಡೆ(inbuf);
					ಬಿಡುಗಡೆ(ದತ್ತಾಂಶ);
					ಹಿಂತಿರುಗಿ ಖಾಲಿ;
				}
				ಹರಿವು.ಮುಂದಿನ_ಹೊರಹೋಗು = ದತ್ತಾಂಶ;
				ಹರಿವು.avail_ಹೊರಹೋಗು = 64*1024;
			} ಆಗ (ಸ್ಥಿತಿ_2 == Z_OK && ಹರಿವು.avail_ಒಳ);
		}
	} ಆಗ (len && ಸ್ಥಿತಿ_2 == Z_OK && !ಹರಿವು.avail_ಒಳ);

	/* This has been inflated OK when first encountered, so... */
	ಆದರೆ (ಸ್ಥಿತಿ_2 != Z_ಹರಿವು_ಕೊನೆ || ಹರಿವು.ಒಟ್ಟು_ಹೊರಹೋಗು != obj->ಗಾತ್ರ_2)
		die(_("serious inflate inconsistency"));

	git_inflate_ಕೊನೆ(&ಹರಿವು);
	ಬಿಡುಗಡೆ(inbuf);
	ಆದರೆ (consume) {
		FREE_AND_NULL(ದತ್ತಾಂಶ);
	}
	ಹಿಂತಿರುಗಿ ದತ್ತಾಂಶ;
}

ಸ್ಥಿರ ಶೂನ್ಯ *get_ದತ್ತಾಂಶ_from_pack(ರಚನೆ object_ನಮೂದು *obj)
{
	ಹಿಂತಿರುಗಿ unpack_ದತ್ತಾಂಶ(obj, ಖಾಲಿ, ಖಾಲಿ);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ compare_ofs_delta_bases(off_t offset1, off_t offset2,
				   ಎಣಿಕೆ object_type type1,
				   ಎಣಿಕೆ object_type type2)
{
	ಪೂರ್ಣಾಂಕ cmp = type1 - type2;
	ಆದರೆ (cmp)
		ಹಿಂತಿರುಗಿ cmp;
	ಹಿಂತಿರುಗಿ offset1 < offset2 ? -1 :
	       offset1 > offset2 ?  1 :
	       0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಕಂಡುಹಿಡಿ_ofs_delta(ಸ್ಥಿರಾಂಕ off_t ಸ್ಥಾನಾಂತರ)
{
	ಪೂರ್ಣಾಂಕ ಮೊದಲ = 0, ಕೊನೆಯ = nr_ofs_deltas;

	ಆಗ (ಮೊದಲ < ಕೊನೆಯ) {
		ಪೂರ್ಣಾಂಕ ಮುಂದಿನ = ಮೊದಲ + (ಕೊನೆಯ - ಮೊದಲ) / 2;
		ರಚನೆ ofs_delta_ನಮೂದು *delta = &ofs_deltas[ಮುಂದಿನ];
		ಪೂರ್ಣಾಂಕ cmp;

		cmp = compare_ofs_delta_bases(ಸ್ಥಾನಾಂತರ, delta->ಸ್ಥಾನಾಂತರ,
					      OBJ_OFS_DELTA,
					      objects[delta->obj_no].type);
		ಆದರೆ (!cmp)
			ಹಿಂತಿರುಗಿ ಮುಂದಿನ;
		ಆದರೆ (cmp < 0) {
			ಕೊನೆಯ = ಮುಂದಿನ;
			ಮುಂದುವರಿ;
		}
		ಮೊದಲ = ಮುಂದಿನ+1;
	}
	ಹಿಂತಿರುಗಿ -ಮೊದಲ-1;
}

ಸ್ಥಿರ ಶೂನ್ಯ ಕಂಡುಹಿಡಿ_ofs_delta_children(off_t ಸ್ಥಾನಾಂತರ,
				    ಪೂರ್ಣಾಂಕ *ಮೊದಲ_ಸೂಚ್ಯಂಕ, ಪೂರ್ಣಾಂಕ *ಕೊನೆಯ_ಸೂಚ್ಯಂಕ)
{
	ಪೂರ್ಣಾಂಕ ಮೊದಲ = ಕಂಡುಹಿಡಿ_ofs_delta(ಸ್ಥಾನಾಂತರ);
	ಪೂರ್ಣಾಂಕ ಕೊನೆಯ = ಮೊದಲ;
	ಪೂರ್ಣಾಂಕ ಕೊನೆ = nr_ofs_deltas - 1;

	ಆದರೆ (ಮೊದಲ < 0) {
		*ಮೊದಲ_ಸೂಚ್ಯಂಕ = 0;
		*ಕೊನೆಯ_ಸೂಚ್ಯಂಕ = -1;
		ಹಿಂತಿರುಗಿ;
	}
	ಆಗ (ಮೊದಲ > 0 && ofs_deltas[ಮೊದಲ - 1].ಸ್ಥಾನಾಂತರ == ಸ್ಥಾನಾಂತರ)
		--ಮೊದಲ;
	ಆಗ (ಕೊನೆಯ < ಕೊನೆ && ofs_deltas[ಕೊನೆಯ + 1].ಸ್ಥಾನಾಂತರ == ಸ್ಥಾನಾಂತರ)
		++ಕೊನೆಯ;
	*ಮೊದಲ_ಸೂಚ್ಯಂಕ = ಮೊದಲ;
	*ಕೊನೆಯ_ಸೂಚ್ಯಂಕ = ಕೊನೆಯ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ compare_ref_delta_bases(ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid1,
				   ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid2,
				   ಎಣಿಕೆ object_type type1,
				   ಎಣಿಕೆ object_type type2)
{
	ಪೂರ್ಣಾಂಕ cmp = type1 - type2;
	ಆದರೆ (cmp)
		ಹಿಂತಿರುಗಿ cmp;
	ಹಿಂತಿರುಗಿ oidcmp(oid1, oid2);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಕಂಡುಹಿಡಿ_ref_delta(ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid)
{
	ಪೂರ್ಣಾಂಕ ಮೊದಲ = 0, ಕೊನೆಯ = nr_ref_deltas;

	ಆಗ (ಮೊದಲ < ಕೊನೆಯ) {
		ಪೂರ್ಣಾಂಕ ಮುಂದಿನ = ಮೊದಲ + (ಕೊನೆಯ - ಮೊದಲ) / 2;
		ರಚನೆ ref_delta_ನಮೂದು *delta = &ref_deltas[ಮುಂದಿನ];
		ಪೂರ್ಣಾಂಕ cmp;

		cmp = compare_ref_delta_bases(oid, &delta->oid,
					      OBJ_REF_DELTA,
					      objects[delta->obj_no].type);
		ಆದರೆ (!cmp)
			ಹಿಂತಿರುಗಿ ಮುಂದಿನ;
		ಆದರೆ (cmp < 0) {
			ಕೊನೆಯ = ಮುಂದಿನ;
			ಮುಂದುವರಿ;
		}
		ಮೊದಲ = ಮುಂದಿನ+1;
	}
	ಹಿಂತಿರುಗಿ -ಮೊದಲ-1;
}

ಸ್ಥಿರ ಶೂನ್ಯ ಕಂಡುಹಿಡಿ_ref_delta_children(ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid,
				    ಪೂರ್ಣಾಂಕ *ಮೊದಲ_ಸೂಚ್ಯಂಕ, ಪೂರ್ಣಾಂಕ *ಕೊನೆಯ_ಸೂಚ್ಯಂಕ)
{
	ಪೂರ್ಣಾಂಕ ಮೊದಲ = ಕಂಡುಹಿಡಿ_ref_delta(oid);
	ಪೂರ್ಣಾಂಕ ಕೊನೆಯ = ಮೊದಲ;
	ಪೂರ್ಣಾಂಕ ಕೊನೆ = nr_ref_deltas - 1;

	ಆದರೆ (ಮೊದಲ < 0) {
		*ಮೊದಲ_ಸೂಚ್ಯಂಕ = 0;
		*ಕೊನೆಯ_ಸೂಚ್ಯಂಕ = -1;
		ಹಿಂತಿರುಗಿ;
	}
	ಆಗ (ಮೊದಲ > 0 && oideq(&ref_deltas[ಮೊದಲ - 1].oid, oid))
		--ಮೊದಲ;
	ಆಗ (ಕೊನೆಯ < ಕೊನೆ && oideq(&ref_deltas[ಕೊನೆಯ + 1].oid, oid))
		++ಕೊನೆಯ;
	*ಮೊದಲ_ಸೂಚ್ಯಂಕ = ಮೊದಲ;
	*ಕೊನೆಯ_ಸೂಚ್ಯಂಕ = ಕೊನೆಯ;
}

ರಚನೆ compare_ದತ್ತಾಂಶ {
	ರಚನೆ object_ನಮೂದು *ನಮೂದು;
	ರಚನೆ odb_ಹರಿವು *st;
	ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *buf;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ buf_ಗಾತ್ರ;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ compare_objects(ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *buf, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಗಾತ್ರ_2,
			   ಶೂನ್ಯ *cb_ದತ್ತಾಂಶ)
{
	ರಚನೆ compare_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ = cb_ದತ್ತಾಂಶ;

	ಆದರೆ (ದತ್ತಾಂಶ->buf_ಗಾತ್ರ < ಗಾತ್ರ_2) {
		ಬಿಡುಗಡೆ(ದತ್ತಾಂಶ->buf);
		ದತ್ತಾಂಶ->buf = xmalloc(ಗಾತ್ರ_2);
		ದತ್ತಾಂಶ->buf_ಗಾತ್ರ = ಗಾತ್ರ_2;
	}

	ಆಗ (ಗಾತ್ರ_2) {
		ssize_t len = odb_ಹರಿವು_read(ದತ್ತಾಂಶ->st, ದತ್ತಾಂಶ->buf, ಗಾತ್ರ_2);
		ಆದರೆ (len == 0)
			die(_("SHA1 COLLISION FOUND WITH %s !"),
			    oid_ಗಮ್ಯ_hex(&ದತ್ತಾಂಶ->ನಮೂದು->idx.oid));
		ಆದರೆ (len < 0)
			die(_("unable to read %s"),
			    oid_ಗಮ್ಯ_hex(&ದತ್ತಾಂಶ->ನಮೂದು->idx.oid));
		ಆದರೆ (memcmp(buf, ದತ್ತಾಂಶ->buf, len))
			die(_("SHA1 COLLISION FOUND WITH %s !"),
			    oid_ಗಮ್ಯ_hex(&ದತ್ತಾಂಶ->ನಮೂದು->idx.oid));
		ಗಾತ್ರ_2 -= len;
		buf += len;
	}
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ check_collison(ರಚನೆ object_ನಮೂದು *ನಮೂದು)
{
	ರಚನೆ compare_ದತ್ತಾಂಶ ದತ್ತಾಂಶ;

	ಆದರೆ (ನಮೂದು->ಗಾತ್ರ_2 <= repo_settings_get_big_file_ಮಿತಿ(the_repository) ||
	    ನಮೂದು->type != OBJ_BLOB)
		ಹಿಂತಿರುಗಿ -1;

	memset(&ದತ್ತಾಂಶ, 0, ಗಾತ್ರ(ದತ್ತಾಂಶ));
	ದತ್ತಾಂಶ.ನಮೂದು = ನಮೂದು;
	ದತ್ತಾಂಶ.st = odb_ಹರಿವು_from_object(the_repository->objects, &ನಮೂದು->idx.oid, ಖಾಲಿ);
	ಆದರೆ (!ದತ್ತಾಂಶ.st)
		ಹಿಂತಿರುಗಿ -1;
	ಆದರೆ (ದತ್ತಾಂಶ.st->ಗಾತ್ರ_2 != ನಮೂದು->ಗಾತ್ರ_2 || ದತ್ತಾಂಶ.st->type != ನಮೂದು->type)
		die(_("SHA1 COLLISION FOUND WITH %s !"),
		    oid_ಗಮ್ಯ_hex(&ನಮೂದು->idx.oid));
	unpack_ದತ್ತಾಂಶ(ನಮೂದು, compare_objects, &ದತ್ತಾಂಶ);
	odb_ಹರಿವು_close(ದತ್ತಾಂಶ.st);
	ಬಿಡುಗಡೆ(ದತ್ತಾಂಶ.buf);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ ದಾಖಲೆ_outgoing_link(ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid)
{
	oidset_ಸೇರಿಸು(&outgoing_links, oid);
}

ಸ್ಥಿರ ಶೂನ್ಯ maybe_ದಾಖಲೆ_name_ನಮೂದು(ಸ್ಥಿರಾಂಕ ರಚನೆ name_ನಮೂದು *ನಮೂದು)
{
	/*
	 * Checking only trees here results in a significantly faster packfile
	 * indexing, but the drawback is that if the packfile to be indexed
	 * references a local blob only directly (that is, never through a
	 * local tree), that local blob is in danger of being garbage
	 * collected. Such a situation may arise if we push local commits,
	 * including one with a change to a blob in the root tree, and then the
	 * server incorporates them into its main branch through a "rebase" or
	 * "squash" merge strategy, and then we fetch the new main branch from
	 * the server.
	 *
	 * This situation has not been observed yet - we have only noticed
	 * missing commits, not missing trees or blobs. (In fact, if it were
	 * believed that only missing commits are problematic, one could argue
	 * that we should also exclude trees during the outgoing link check;
	 * but it is safer to include them.)
	 *
	 * Due to the rarity of the situation (it has not been observed to
	 * happen in real life), and because the "penalty" in such a situation
	 * is merely to refetch the missing blob when it's needed (and this
	 * happens only once - when refetched, the blob goes into a promisor
	 * pack, so it won't be GC-ed, the tradeoff seems worth it.
	*/
	ಆದರೆ (S_ISDIR(ನಮೂದು->ಬಹುಲಕ))
		ದಾಖಲೆ_outgoing_link(&ನಮೂದು->oid);
}

ಸ್ಥಿರ ಶೂನ್ಯ do_ದಾಖಲೆ_outgoing_links(ರಚನೆ object *obj)
{
	ಆದರೆ (obj->type == OBJ_ವೃಕ್ಷ) {
		ರಚನೆ ವೃಕ್ಷ *ವೃಕ್ಷ = (ರಚನೆ ವೃಕ್ಷ *)obj;
		ರಚನೆ ವೃಕ್ಷ_desc desc;
		ರಚನೆ name_ನಮೂದು ನಮೂದು;
		ಆದರೆ (init_ವೃಕ್ಷ_desc_gently(&desc, &ವೃಕ್ಷ->object.oid,
					  ವೃಕ್ಷ->ಬಫರ್, ವೃಕ್ಷ->ಗಾತ್ರ_2, 0))
			/*
			 * Error messages are given when packs are
			 * verified, so do not print any here.
			 */
			ಹಿಂತಿರುಗಿ;
		ಆಗ (ವೃಕ್ಷ_ನಮೂದು_gently(&desc, &ನಮೂದು))
			maybe_ದಾಖಲೆ_name_ನಮೂದು(&ನಮೂದು);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (obj->type == OBJ_COMMIT) {
		ರಚನೆ commit *commit = (ರಚನೆ commit *) obj;
		ರಚನೆ commit_list *parents = commit->parents;

		ದಾಖಲೆ_outgoing_link(get_commit_ವೃಕ್ಷ_oid(commit));
		ಗಾಗಿ (; parents; parents = parents->ಮುಂದಿನ)
			ದಾಖಲೆ_outgoing_link(&parents->ಅಂಶ->object.oid);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (obj->type == OBJ_ಟ್ಯಾಗ್) {
		ರಚನೆ ಟ್ಯಾಗ್ *ಟ್ಯಾಗ್ = (ರಚನೆ ಟ್ಯಾಗ್ *) obj;
		ದಾಖಲೆ_outgoing_link(get_tagged_oid(ಟ್ಯಾಗ್));
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ sha1_object(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ದತ್ತಾಂಶ, ರಚನೆ object_ನಮೂದು *obj_ನಮೂದು,
			ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಗಾತ್ರ_2, ಎಣಿಕೆ object_type type,
			ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid)
{
	ಶೂನ್ಯ *new_ದತ್ತಾಂಶ = ಖಾಲಿ;
	ಪೂರ್ಣಾಂಕ collision_test_needed = 0;

	ಪ್ರತಿಜ್ಞಾ(ದತ್ತಾಂಶ || obj_ನಮೂದು);

	ಆದರೆ (startup_info->have_repository) {
		read_ಬೀಗ();
		collision_test_needed = odb_has_object(the_repository->objects, oid,
						       ODB_HAS_OBJECT_FETCH_PROMISOR);
		read_unlock();
	}

	ಆದರೆ (collision_test_needed && !ದತ್ತಾಂಶ) {
		read_ಬೀಗ();
		ಆದರೆ (!check_collison(obj_ನಮೂದು))
			collision_test_needed = 0;
		read_unlock();
	}
	ಆದರೆ (collision_test_needed) {
		ಶೂನ್ಯ *has_ದತ್ತಾಂಶ;
		ಎಣಿಕೆ object_type has_type;
		ಗಾತ್ರ_t has_ಗಾತ್ರ;
		read_ಬೀಗ();
		has_type = odb_read_object_info(the_repository->objects, oid, &has_ಗಾತ್ರ);
		ಆದರೆ (has_type < 0)
			die(_("cannot read existing object info %s"), oid_ಗಮ್ಯ_hex(oid));
		ಆದರೆ (has_type != type || has_ಗಾತ್ರ != ಗಾತ್ರ_2)
			die(_("SHA1 COLLISION FOUND WITH %s !"), oid_ಗಮ್ಯ_hex(oid));
		has_ದತ್ತಾಂಶ = odb_read_object(the_repository->objects, oid,
					   &has_type, &has_ಗಾತ್ರ);
		read_unlock();
		ಆದರೆ (!ದತ್ತಾಂಶ)
			ದತ್ತಾಂಶ = new_ದತ್ತಾಂಶ = get_ದತ್ತಾಂಶ_from_pack(obj_ನಮೂದು);
		ಆದರೆ (!has_ದತ್ತಾಂಶ)
			die(_("cannot read existing object %s"), oid_ಗಮ್ಯ_hex(oid));
		ಆದರೆ (ಗಾತ್ರ_2 != has_ಗಾತ್ರ || type != has_type ||
		    memcmp(ದತ್ತಾಂಶ, has_ದತ್ತಾಂಶ, ಗಾತ್ರ_2) != 0)
			die(_("SHA1 COLLISION FOUND WITH %s !"), oid_ಗಮ್ಯ_hex(oid));
		ಬಿಡುಗಡೆ(has_ದತ್ತಾಂಶ);
	}

	ಆದರೆ (strict || do_fsck_object || ದಾಖಲೆ_outgoing_links) {
		read_ಬೀಗ();
		ಆದರೆ (type == OBJ_BLOB) {
			ರಚನೆ blob *blob = lookup_blob(the_repository, oid);
			ಆದರೆ (blob)
				blob->object.flags |= FLAG_CHECKED;
			ಇಲ್ಲದಿದ್ದರೆ
				die(_("invalid blob object %s"), oid_ಗಮ್ಯ_hex(oid));
			ಆದರೆ (do_fsck_object &&
			    fsck_object(&blob->object, (ಶೂನ್ಯ *)ದತ್ತಾಂಶ, ಗಾತ್ರ_2, &fsck_options))
				die(_("fsck error in packed object"));
		} ಇಲ್ಲದಿದ್ದರೆ {
			ರಚನೆ object *obj;
			ಪೂರ್ಣಾಂಕ eaten;
			ಶೂನ್ಯ *buf = (ಶೂನ್ಯ *) ದತ್ತಾಂಶ;

			ಪ್ರತಿಜ್ಞಾ(ದತ್ತಾಂಶ && "data can only be NULL for large _blobs_");

			/*
			 * we do not need to free the memory here, as the
			 * buf is deleted by the caller.
			 */
			obj = parse_object_ಬಫರ್(the_repository, oid, type,
						  ಗಾತ್ರ_2, buf,
						  &eaten);
			ಆದರೆ (!obj)
				die(_("invalid %s"), type_name(type));
			ಆದರೆ (do_fsck_object &&
			    fsck_object(obj, buf, ಗಾತ್ರ_2, &fsck_options))
				die(_("fsck error in packed object"));
			ಆದರೆ (strict && fsck_walk(obj, ಖಾಲಿ, &fsck_options))
				die(_("Not all child objects of %s are reachable"), oid_ಗಮ್ಯ_hex(&obj->oid));
			ಆದರೆ (ದಾಖಲೆ_outgoing_links)
				do_ದಾಖಲೆ_outgoing_links(obj);

			ಆದರೆ (obj->type == OBJ_ವೃಕ್ಷ) {
				ರಚನೆ ವೃಕ್ಷ *ಅಂಶ = (ರಚನೆ ವೃಕ್ಷ *) obj;
				ಅಂಶ->ಬಫರ್ = ಖಾಲಿ;
				obj->parsed = 0;
			}
			ಆದರೆ (obj->type == OBJ_COMMIT) {
				ರಚನೆ commit *commit = (ರಚನೆ commit *) obj;
				ಆದರೆ (detach_commit_ಬಫರ್(commit, ಖಾಲಿ) != ದತ್ತಾಂಶ)
					BUG("parse_object_buffer transmogrified our buffer");
			}
			obj->flags |= FLAG_CHECKED;
		}
		read_unlock();
	}

	ಬಿಡುಗಡೆ(new_ದತ್ತಾಂಶ);
}

/*
 * Ensure that this node has been reconstructed and return its contents.
 *
 * In the typical and best case, this node would already be reconstructed
 * (through the invocation to resolve_delta() in threaded_second_pass()) and it
 * would not be pruned. However, if pruning of this node was necessary due to
 * reaching delta_base_cache_limit, this function will find the closest
 * ancestor with reconstructed data that has not been pruned (or if there is
 * none, the ultimate base object), and reconstruct each node in the delta
 * chain in order to generate the reconstructed data for this node.
 */
ಸ್ಥಿರ ಶೂನ್ಯ *get_ತಳ_ದತ್ತಾಂಶ(ರಚನೆ ತಳ_ದತ್ತಾಂಶ *c)
{
	ಆದರೆ (!c->ದತ್ತಾಂಶ) {
		ರಚನೆ object_ನಮೂದು *obj = c->obj;
		ರಚನೆ ತಳ_ದತ್ತಾಂಶ **delta = ಖಾಲಿ;
		ಪೂರ್ಣಾಂಕ delta_nr = 0, delta_alloc = 0;

		ಆಗ (is_delta_type(c->obj->type) && !c->ದತ್ತಾಂಶ) {
			ALLOC_GROW(delta, delta_nr + 1, delta_alloc);
			delta[delta_nr++] = c;
			c = c->ತಳ;
		}
		ಆದರೆ (!delta_nr) {
			c->ದತ್ತಾಂಶ = get_ದತ್ತಾಂಶ_from_pack(obj);
			c->ಗಾತ್ರ_2 = obj->ಗಾತ್ರ_2;
			ತಳ_cache_ಬಳಸಿದ += c->ಗಾತ್ರ_2;
			prune_ತಳ_ದತ್ತಾಂಶ(c);
		}
		ಗಾಗಿ (; delta_nr > 0; delta_nr--) {
			ಶೂನ್ಯ *ತಳ, *raw;
			c = delta[delta_nr - 1];
			obj = c->obj;
			ತಳ = get_ತಳ_ದತ್ತಾಂಶ(c->ತಳ);
			raw = get_ದತ್ತಾಂಶ_from_pack(obj);
			c->ದತ್ತಾಂಶ = patch_delta(
				ತಳ, c->ತಳ->ಗಾತ್ರ_2,
				raw, obj->ಗಾತ್ರ_2,
				&c->ಗಾತ್ರ_2);
			ಬಿಡುಗಡೆ(raw);
			ಆದರೆ (!c->ದತ್ತಾಂಶ)
				bad_object(obj->idx.ಸ್ಥಾನಾಂತರ, _("failed to apply delta"));
			ತಳ_cache_ಬಳಸಿದ += c->ಗಾತ್ರ_2;
			prune_ತಳ_ದತ್ತಾಂಶ(c);
		}
		ಬಿಡುಗಡೆ(delta);
	}
	ಹಿಂತಿರುಗಿ c->ದತ್ತಾಂಶ;
}

ಸ್ಥಿರ ರಚನೆ ತಳ_ದತ್ತಾಂಶ *make_ತಳ(ರಚನೆ object_ನಮೂದು *obj,
				   ರಚನೆ ತಳ_ದತ್ತಾಂಶ *ಪೋಷಕ)
{
	ರಚನೆ ತಳ_ದತ್ತಾಂಶ *ತಳ = xcalloc(1, ಗಾತ್ರ(ರಚನೆ ತಳ_ದತ್ತಾಂಶ));
	ತಳ->ತಳ = ಪೋಷಕ;
	ತಳ->obj = obj;
	ಕಂಡುಹಿಡಿ_ref_delta_children(&obj->idx.oid,
				&ತಳ->ref_ಮೊದಲ, &ತಳ->ref_ಕೊನೆಯ);
	ಕಂಡುಹಿಡಿ_ofs_delta_children(obj->idx.ಸ್ಥಾನಾಂತರ,
				&ತಳ->ofs_ಮೊದಲ, &ತಳ->ofs_ಕೊನೆಯ);
	ತಳ->children_ಉಳಿದ = ತಳ->ref_ಕೊನೆಯ - ತಳ->ref_ಮೊದಲ +
		ತಳ->ofs_ಕೊನೆಯ - ತಳ->ofs_ಮೊದಲ + 2;
	ಹಿಂತಿರುಗಿ ತಳ;
}

ಸ್ಥಿರ ರಚನೆ ತಳ_ದತ್ತಾಂಶ *resolve_delta(ರಚನೆ object_ನಮೂದು *delta_obj,
				       ರಚನೆ ತಳ_ದತ್ತಾಂಶ *ತಳ)
{
	ಶೂನ್ಯ *delta_ದತ್ತಾಂಶ, *ಫಲಿತಾಂಶ_ದತ್ತಾಂಶ;
	ರಚನೆ ತಳ_ದತ್ತಾಂಶ *ಫಲಿತಾಂಶ;
	ಗಾತ್ರ_t ಫಲಿತಾಂಶ_ಗಾತ್ರ;

	ಆದರೆ (show_stat) {
		ಪೂರ್ಣಾಂಕ i = delta_obj - objects;
		ಪೂರ್ಣಾಂಕ j = ತಳ->obj - objects;
		obj_stat[i].delta_ಆಳ = obj_stat[j].delta_ಆಳ + 1;
		ಅತ್ಯಂತಆಳ_delta_ಬೀಗ();
		ಆದರೆ (ಅತ್ಯಂತಆಳ_delta < obj_stat[i].delta_ಆಳ)
			ಅತ್ಯಂತಆಳ_delta = obj_stat[i].delta_ಆಳ;
		ಅತ್ಯಂತಆಳ_delta_unlock();
		obj_stat[i].ತಳ_object_no = j;
	}
	delta_ದತ್ತಾಂಶ = get_ದತ್ತಾಂಶ_from_pack(delta_obj);
	ಪ್ರತಿಜ್ಞಾ(ತಳ->ದತ್ತಾಂಶ);
	ಫಲಿತಾಂಶ_ದತ್ತಾಂಶ = patch_delta(ತಳ->ದತ್ತಾಂಶ, ತಳ->ಗಾತ್ರ_2,
				  delta_ದತ್ತಾಂಶ, delta_obj->ಗಾತ್ರ_2, &ಫಲಿತಾಂಶ_ಗಾತ್ರ);
	ಬಿಡುಗಡೆ(delta_ದತ್ತಾಂಶ);
	ಆದರೆ (!ಫಲಿತಾಂಶ_ದತ್ತಾಂಶ)
		bad_object(delta_obj->idx.ಸ್ಥಾನಾಂತರ, _("failed to apply delta"));
	hash_object_file(the_hash_algo, ಫಲಿತಾಂಶ_ದತ್ತಾಂಶ, ಫಲಿತಾಂಶ_ಗಾತ್ರ,
			 delta_obj->real_type, &delta_obj->idx.oid);
	sha1_object(ಫಲಿತಾಂಶ_ದತ್ತಾಂಶ, ಖಾಲಿ, ಫಲಿತಾಂಶ_ಗಾತ್ರ, delta_obj->real_type,
		    &delta_obj->idx.oid);

	ಫಲಿತಾಂಶ = make_ತಳ(delta_obj, ತಳ);
	ಫಲಿತಾಂಶ->ದತ್ತಾಂಶ = ಫಲಿತಾಂಶ_ದತ್ತಾಂಶ;
	ಫಲಿತಾಂಶ->ಗಾತ್ರ_2 = ಫಲಿತಾಂಶ_ಗಾತ್ರ;

	counter_ಬೀಗ();
	nr_resolved_deltas++;
	counter_unlock();

	ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ compare_ofs_delta_ನಮೂದು(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *a, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *b)
{
	ಸ್ಥಿರಾಂಕ ರಚನೆ ofs_delta_ನಮೂದು *delta_a = a;
	ಸ್ಥಿರಾಂಕ ರಚನೆ ofs_delta_ನಮೂದು *delta_b = b;

	ಹಿಂತಿರುಗಿ delta_a->ಸ್ಥಾನಾಂತರ < delta_b->ಸ್ಥಾನಾಂತರ ? -1 :
	       delta_a->ಸ್ಥಾನಾಂತರ > delta_b->ಸ್ಥಾನಾಂತರ ?  1 :
	       0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ compare_ref_delta_ನಮೂದು(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *a, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *b)
{
	ಸ್ಥಿರಾಂಕ ರಚನೆ ref_delta_ನಮೂದು *delta_a = a;
	ಸ್ಥಿರಾಂಕ ರಚನೆ ref_delta_ನಮೂದು *delta_b = b;

	ಹಿಂತಿರುಗಿ oidcmp(&delta_a->oid, &delta_b->oid);
}

ಸ್ಥಿರ ಶೂನ್ಯ *threaded_ಎರಡನೇ_pass(ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
	ಆದರೆ (ದತ್ತಾಂಶ)
		ಗಣ_thread_ದತ್ತಾಂಶ(ದತ್ತಾಂಶ);
	ಗಾಗಿ (;;) {
		ರಚನೆ ತಳ_ದತ್ತಾಂಶ *ಪೋಷಕ = ಖಾಲಿ;
		ರಚನೆ object_ನಮೂದು *ಮಗು_obj = ಖಾಲಿ;
		ರಚನೆ ತಳ_ದತ್ತಾಂಶ *ಮಗು = ಖಾಲಿ;

		counter_ಬೀಗ();
		display_ಪ್ರಗತಿ(ಪ್ರಗತಿ, nr_resolved_deltas);
		counter_unlock();

		work_ಬೀಗ();
		ಆದರೆ (list_ಖಾಲಿಗಳು(&work_ತಲೆ)) {
			/*
			 * Take an object from the object array.
			 */
			ಆಗ (nr_dispatched < nr_objects &&
			       is_delta_type(objects[nr_dispatched].type))
				nr_dispatched++;
			ಆದರೆ (nr_dispatched >= nr_objects) {
				work_unlock();
				ನಿಲ್ಲಿಸು;
			}
			ಮಗು_obj = &objects[nr_dispatched++];
		} ಇಲ್ಲದಿದ್ದರೆ {
			/*
			 * Peek at the top of the stack, and take a child from
			 * it.
			 */
			ಪೋಷಕ = list_ಮೊದಲ_ನಮೂದು(&work_ತಲೆ, ರಚನೆ ತಳ_ದತ್ತಾಂಶ,
						  list);

			ಆಗ (ಪೋಷಕ->ref_ಮೊದಲ <= ಪೋಷಕ->ref_ಕೊನೆಯ) {
				ಪೂರ್ಣಾಂಕ ಸ್ಥಾನಾಂತರ = ref_deltas[ಪೋಷಕ->ref_ಮೊದಲ++].obj_no;
				ಮಗು_obj = objects + ಸ್ಥಾನಾಂತರ;
				ಆದರೆ (ಮಗು_obj->real_type != OBJ_REF_DELTA) {
					ಮಗು_obj = ಖಾಲಿ;
					ಮುಂದುವರಿ;
				}
				ಮಗು_obj->real_type = ಪೋಷಕ->obj->real_type;
				ನಿಲ್ಲಿಸು;
			}

			ಆದರೆ (!ಮಗು_obj && ಪೋಷಕ->ofs_ಮೊದಲ <= ಪೋಷಕ->ofs_ಕೊನೆಯ) {
				ಮಗು_obj = objects +
					ofs_deltas[ಪೋಷಕ->ofs_ಮೊದಲ++].obj_no;
				ಪ್ರತಿಜ್ಞಾ(ಮಗು_obj->real_type == OBJ_OFS_DELTA);
				ಮಗು_obj->real_type = ಪೋಷಕ->obj->real_type;
			}

			ಆದರೆ (ಪೋಷಕ->ref_ಮೊದಲ > ಪೋಷಕ->ref_ಕೊನೆಯ &&
			    ಪೋಷಕ->ofs_ಮೊದಲ > ಪೋಷಕ->ofs_ಕೊನೆಯ) {
				/*
				 * This parent has run out of children, so move
				 * it to done_head.
				 */
				list_del(&ಪೋಷಕ->list);
				list_ಕೂಡಿಸು(&ಪೋಷಕ->list, &ಮುಗಿದ_ತಲೆ);
			}

			/*
			 * Ensure that the parent has data, since we will need
			 * it later.
			 *
			 * NEEDSWORK: If parent data needs to be reloaded, this
			 * prolongs the time that the current thread spends in
			 * the mutex. A mitigating factor is that parent data
			 * needs to be reloaded only if the delta base cache
			 * limit is exceeded, so in the typical case, this does
			 * not happen.
			 */
			get_ತಳ_ದತ್ತಾಂಶ(ಪೋಷಕ);
			ಪೋಷಕ->retain_ದತ್ತಾಂಶ++;
		}
		work_unlock();

		ಆದರೆ (ಮಗು_obj) {
			ಆದರೆ (ಪೋಷಕ) {
				ಮಗು = resolve_delta(ಮಗು_obj, ಪೋಷಕ);
				ಆದರೆ (!ಮಗು->children_ಉಳಿದ)
					FREE_AND_NULL(ಮಗು->ದತ್ತಾಂಶ);
			} ಇಲ್ಲದಿದ್ದರೆ{
				ಮಗು = make_ತಳ(ಮಗು_obj, ಖಾಲಿ);
				ಆದರೆ (ಮಗು->children_ಉಳಿದ) {
					/*
					 * Since this child has its own delta children,
					 * we will need this data in the future.
					 * Inflate now so that future iterations will
					 * have access to this object's data while
					 * outside the work mutex.
					 */
					ಮಗು->ದತ್ತಾಂಶ = get_ದತ್ತಾಂಶ_from_pack(ಮಗು_obj);
					ಮಗು->ಗಾತ್ರ_2 = ಮಗು_obj->ಗಾತ್ರ_2;
				}
			}
		}

		work_ಬೀಗ();
		ಆದರೆ (ಪೋಷಕ)
			ಪೋಷಕ->retain_ದತ್ತಾಂಶ--;

		ಆದರೆ (ಮಗು && ಮಗು->ದತ್ತಾಂಶ) {
			/*
			 * This child has its own children, so add it to
			 * work_head.
			 */
			list_ಕೂಡಿಸು(&ಮಗು->list, &work_ತಲೆ);
			ತಳ_cache_ಬಳಸಿದ += ಮಗು->ಗಾತ್ರ_2;
			prune_ತಳ_ದತ್ತಾಂಶ(ಖಾಲಿ);
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಮಗು) {
			/*
			 * This child does not have its own children. It may be
			 * the last descendant of its ancestors; free those
			 * that we can.
			 */
			ರಚನೆ ತಳ_ದತ್ತಾಂಶ *p = ಪೋಷಕ;

			ಆಗ (p) {
				ರಚನೆ ತಳ_ದತ್ತಾಂಶ *ಮುಂದಿನ_p;

				p->children_ಉಳಿದ--;
				ಆದರೆ (p->children_ಉಳಿದ)
					ನಿಲ್ಲಿಸು;

				ಮುಂದಿನ_p = p->ತಳ;
				free_ತಳ_ದತ್ತಾಂಶ(p);
				list_del(&p->list);
				ಬಿಡುಗಡೆ(p);

				p = ಮುಂದಿನ_p;
			}
			FREE_AND_NULL(ಮಗು);
		}
		work_unlock();
	}
	ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

/*
 * First pass:
 * - find locations of all objects;
 * - calculate SHA1 of all non-delta objects;
 * - remember base (SHA1 or offset) for all deltas.
 */
ಸ್ಥಿರ ಶೂನ್ಯ parse_pack_objects(ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *hash)
{
	ಪೂರ್ಣಾಂಕ i, nr_delays = 0;
	ರಚನೆ ofs_delta_ನಮೂದು *ofs_delta = ofs_deltas;
	ರಚನೆ object_id ref_delta_oid;
	ರಚನೆ stat st;
	ರಚನೆ git_hash_ctx tmp_ctx;

	ಆದರೆ (verbose)
		ಪ್ರಗತಿ = ಆರಂಭ_ಪ್ರಗತಿ(
				the_repository,
				ಪ್ರಗತಿ_title ? ಪ್ರಗತಿ_title :
				from_stdin ? _("Receiving objects") : _("Indexing objects"),
				nr_objects);
	ಗಾಗಿ (i = 0; i < nr_objects; i++) {
		ರಚನೆ object_ನಮೂದು *obj = &objects[i];
		ಶೂನ್ಯ *ದತ್ತಾಂಶ = unpack_raw_ನಮೂದು(obj, &ofs_delta->ಸ್ಥಾನಾಂತರ,
					      &ref_delta_oid,
					      &obj->idx.oid);
		obj->real_type = obj->type;
		ಆದರೆ (obj->type == OBJ_OFS_DELTA) {
			nr_ofs_deltas++;
			ofs_delta->obj_no = i;
			ofs_delta++;
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (obj->type == OBJ_REF_DELTA) {
			ALLOC_GROW(ref_deltas, nr_ref_deltas + 1, ref_deltas_alloc);
			oidcpy(&ref_deltas[nr_ref_deltas].oid, &ref_delta_oid);
			ref_deltas[nr_ref_deltas].obj_no = i;
			nr_ref_deltas++;
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!ದತ್ತಾಂಶ) {
			/* large blobs, check later */
			obj->real_type = OBJ_BAD;
			nr_delays++;
		} ಇಲ್ಲದಿದ್ದರೆ
			sha1_object(ದತ್ತಾಂಶ, ಖಾಲಿ, obj->ಗಾತ್ರ_2, obj->type,
				    &obj->idx.oid);
		ಬಿಡುಗಡೆ(ದತ್ತಾಂಶ);
		display_ಪ್ರಗತಿ(ಪ್ರಗತಿ, i+1);
	}
	objects[i].idx.ಸ್ಥಾನಾಂತರ = consumed_ಬೈಟುಗಳು;
	stop_ಪ್ರಗತಿ(&ಪ್ರಗತಿ);

	/* Check pack integrity */
	flush();
	git_hash_init(&tmp_ctx, the_hash_algo);
	git_hash_clone(&tmp_ctx, &ಒಳಹರಿವು_ctx);
	git_hash_final(hash, &tmp_ctx);
	ಆದರೆ (!hasheq(fill(the_hash_algo->rawsz), hash, the_repository->hash_algo))
		die(_("pack is corrupted (SHA1 mismatch)"));
	use(the_hash_algo->rawsz);

	/* If input_fd is a file, we should have reached its end now. */
	ಆದರೆ (fstat(ಒಳಹರಿವು_fd, &st))
		die_errno(_("cannot fstat packfile"));
	ಆದರೆ (S_ISREG(st.st_ಬಹುಲಕ) &&
			lseek(ಒಳಹರಿವು_fd, 0, SEEK_CUR) - ಒಳಹರಿವು_len != st.st_ಗಾತ್ರ)
		die(_("pack has junk at the end"));

	ಗಾಗಿ (i = 0; i < nr_objects; i++) {
		ರಚನೆ object_ನಮೂದು *obj = &objects[i];
		ಆದರೆ (obj->real_type != OBJ_BAD)
			ಮುಂದುವರಿ;
		obj->real_type = obj->type;
		sha1_object(ಖಾಲಿ, obj, obj->ಗಾತ್ರ_2, obj->type,
			    &obj->idx.oid);
		nr_delays--;
	}
	ಆದರೆ (nr_delays)
		die(_("confusion beyond insanity in parse_pack_objects()"));
}

/*
 * Second pass:
 * - for all non-delta objects, look if it is used as a base for
 *   deltas;
 * - if used as a base, uncompress the object and apply all deltas,
 *   recursively checking if the resulting object is used as a base
 *   for some more deltas.
 */
ಸ್ಥಿರ ಶೂನ್ಯ resolve_deltas(ರಚನೆ pack_idx_option *opts)
{
	ಪೂರ್ಣಾಂಕ i;

	ಆದರೆ (!nr_ofs_deltas && !nr_ref_deltas)
		ಹಿಂತಿರುಗಿ;

	/* Sort deltas by base SHA1/offset for fast searching */
	QSORT(ofs_deltas, nr_ofs_deltas, compare_ofs_delta_ನಮೂದು);
	QSORT(ref_deltas, nr_ref_deltas, compare_ref_delta_ನಮೂದು);

	ಆದರೆ (verbose || show_resolving_ಪ್ರಗತಿ)
		ಪ್ರಗತಿ = ಆರಂಭ_ಪ್ರಗತಿ(the_repository,
					  _("Resolving deltas"),
					  nr_ref_deltas + nr_ofs_deltas);

	nr_dispatched = 0;
	ತಳ_cache_ಎಲ್ಲೆ = opts->delta_ತಳ_cache_ಎಲ್ಲೆ * nr_ಎಳೆಗಳು;
	ಆದರೆ (nr_ಎಳೆಗಳು > 1 || getenv("GIT_FORCE_THREADS")) {
		init_thread();
		ಗಾಗಿ (i = 0; i < nr_ಎಳೆಗಳು; i++) {
			ಪೂರ್ಣಾಂಕ ret = pthread_create(&thread_ದತ್ತಾಂಶ[i].thread, ಖಾಲಿ,
						 threaded_ಎರಡನೇ_pass, thread_ದತ್ತಾಂಶ + i);
			ಆದರೆ (ret)
				die(_("unable to create thread: %s"),
				    strerror(ret));
		}
		ಗಾಗಿ (i = 0; i < nr_ಎಳೆಗಳು; i++)
			pthread_join(thread_ದತ್ತಾಂಶ[i].thread, ಖಾಲಿ);
		cleanup_thread();
		ಹಿಂತಿರುಗಿ;
	}
	threaded_ಎರಡನೇ_pass(&nothread_ದತ್ತಾಂಶ);
}

/*
 * Third pass:
 * - append objects to convert thin pack to full pack if required
 * - write the final pack hash
 */
ಸ್ಥಿರ ಶೂನ್ಯ fix_unresolved_deltas(ರಚನೆ hashfile *f);
ಸ್ಥಿರ ಶೂನ್ಯ conclude_pack(ಪೂರ್ಣಾಂಕ fix_thin_pack, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *curr_pack, ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *pack_hash)
{
	ಆದರೆ (nr_ref_deltas + nr_ofs_deltas == nr_resolved_deltas) {
		stop_ಪ್ರಗತಿ(&ಪ್ರಗತಿ);
		/* Flush remaining pack final hash. */
		flush();
		ಹಿಂತಿರುಗಿ;
	}

	ಆದರೆ (fix_thin_pack) {
		ರಚನೆ hashfile *f;
		ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ read_hash[GIT_MAX_RAWSZ], ಬಾಲ_hash[GIT_MAX_RAWSZ];
		ರಚನೆ strbuf ಸಂದೇಶ = STRBUF_INIT;
		ಪೂರ್ಣಾಂಕ nr_unresolved = nr_ofs_deltas + nr_ref_deltas - nr_resolved_deltas;
		ಪೂರ್ಣಾಂಕ nr_objects_initial = nr_objects;
		ಆದರೆ (nr_unresolved <= 0)
			die(_("confusion beyond insanity"));
		REALLOC_ARRAY(objects, nr_objects + nr_unresolved + 1);
		memset(objects + nr_objects + 1, 0,
		       nr_unresolved * ಗಾತ್ರ(*objects));
		f = hashfd(the_repository->hash_algo, output_fd, curr_pack);
		fix_unresolved_deltas(f);
		strbuf_addf(&ಸಂದೇಶ, Q_("completed with %d local object",
				     "completed with %d local objects",
				     nr_objects - nr_objects_initial),
			    nr_objects - nr_objects_initial);
		stop_ಪ್ರಗತಿ_ಸಂದೇಶ(&ಪ್ರಗತಿ, ಸಂದೇಶ.buf);
		strbuf_release(&ಸಂದೇಶ);
		finalize_hashfile(f, ಬಾಲ_hash, FSYNC_COMPONENT_PACK, 0);
		hashcpy(read_hash, pack_hash, the_repository->hash_algo);
		fixup_pack_ಶೀರ್ಷಿಕೆ_footer(the_hash_algo, output_fd, pack_hash,
					 curr_pack, nr_objects,
					 read_hash, consumed_ಬೈಟುಗಳು-the_hash_algo->rawsz);
		ಆದರೆ (!hasheq(read_hash, ಬಾಲ_hash, the_repository->hash_algo))
			die(_("Unexpected tail checksum for %s "
			      "(disk corruption?)"), curr_pack);
	}
	ಆದರೆ (nr_ofs_deltas + nr_ref_deltas != nr_resolved_deltas)
		die(Q_("pack has %d unresolved delta",
		       "pack has %d unresolved deltas",
		       nr_ofs_deltas + nr_ref_deltas - nr_resolved_deltas),
		    nr_ofs_deltas + nr_ref_deltas - nr_resolved_deltas);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ write_compressed(ರಚನೆ hashfile *f, ಶೂನ್ಯ *ಒಳ, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2)
{
	git_zstream ಹರಿವು;
	ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2;
	ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ outbuf[4096];
	ರಚನೆ repo_config_ಮೌಲ್ಯಗಳು *cfg = repo_config_ಮೌಲ್ಯಗಳು(the_repository);

	git_deflate_init(&ಹರಿವು, cfg->zlib_compression_level);
	ಹರಿವು.ಮುಂದಿನ_ಒಳ = ಒಳ;
	ಹರಿವು.avail_ಒಳ = ಗಾತ್ರ_2;

	ಮಾಡು {
		ಹರಿವು.ಮುಂದಿನ_ಹೊರಹೋಗು = outbuf;
		ಹರಿವು.avail_ಹೊರಹೋಗು = ಗಾತ್ರ(outbuf);
		ಸ್ಥಿತಿ_2 = git_deflate(&ಹರಿವು, Z_ಮುಕ್ತಾಯ);
		hashwrite(f, outbuf, ಗಾತ್ರ(outbuf) - ಹರಿವು.avail_ಹೊರಹೋಗು);
	} ಆಗ (ಸ್ಥಿತಿ_2 == Z_OK);

	ಆದರೆ (ಸ್ಥಿತಿ_2 != Z_ಹರಿವು_ಕೊನೆ)
		die(_("unable to deflate appended object (%d)"), ಸ್ಥಿತಿ_2);
	ಗಾತ್ರ_2 = ಹರಿವು.ಒಟ್ಟು_ಹೊರಹೋಗು;
	git_deflate_ಕೊನೆ(&ಹರಿವು);
	ಹಿಂತಿರುಗಿ ಗಾತ್ರ_2;
}

ಸ್ಥಿರ ರಚನೆ object_ನಮೂದು *append_obj_ಗಮ್ಯ_pack(ರಚನೆ hashfile *f,
			       ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *sha1, ಶೂನ್ಯ *buf,
			       ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಗಾತ್ರ_2, ಎಣಿಕೆ object_type type)
{
	ರಚನೆ object_ನಮೂದು *obj = &objects[nr_objects++];
	ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ ಶೀರ್ಷಿಕೆ[10];
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ s = ಗಾತ್ರ_2;
	ಪೂರ್ಣಾಂಕ n = 0;
	ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ c = (type << 4) | (s & 15);
	s >>= 4;
	ಆಗ (s) {
		ಶೀರ್ಷಿಕೆ[n++] = c | 0x80;
		c = s & 0x7f;
		s >>= 7;
	}
	ಶೀರ್ಷಿಕೆ[n++] = c;
	ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_32_begin(f);
	hashwrite(f, ಶೀರ್ಷಿಕೆ, n);
	obj[0].ಗಾತ್ರ_2 = ಗಾತ್ರ_2;
	obj[0].hdr_ಗಾತ್ರ = n;
	obj[0].type = type;
	obj[0].real_type = type;
	obj[1].idx.ಸ್ಥಾನಾಂತರ = obj[0].idx.ಸ್ಥಾನಾಂತರ + n;
	obj[1].idx.ಸ್ಥಾನಾಂತರ += write_compressed(f, buf, ಗಾತ್ರ_2);
	obj[0].idx.ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_32 = ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_32_ಕೊನೆ(f);
	hashflush(f);
	oidread(&obj->idx.oid, sha1, the_repository->hash_algo);
	ಹಿಂತಿರುಗಿ obj;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ delta_pos_compare(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *_a, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *_b)
{
	ರಚನೆ ref_delta_ನಮೂದು *a = *(ರಚನೆ ref_delta_ನಮೂದು **)_a;
	ರಚನೆ ref_delta_ನಮೂದು *b = *(ರಚನೆ ref_delta_ನಮೂದು **)_b;
	ಹಿಂತಿರುಗಿ a->obj_no - b->obj_no;
}

ಸ್ಥಿರ ಶೂನ್ಯ fix_unresolved_deltas(ರಚನೆ hashfile *f)
{
	ರಚನೆ ref_delta_ನಮೂದು **sorted_by_pos;
	ಪೂರ್ಣಾಂಕ i;

	/*
	 * Since many unresolved deltas may well be themselves base objects
	 * for more unresolved deltas, we really want to include the
	 * smallest number of base objects that would cover as much delta
	 * as possible by picking the
	 * trunc deltas first, allowing for other deltas to resolve without
	 * additional base objects.  Since most base objects are to be found
	 * before deltas depending on them, a good heuristic is to start
	 * resolving deltas in the same order as their position in the pack.
	 */
	ALLOC_ARRAY(sorted_by_pos, nr_ref_deltas);
	ಗಾಗಿ (i = 0; i < nr_ref_deltas; i++)
		sorted_by_pos[i] = &ref_deltas[i];
	QSORT(sorted_by_pos, nr_ref_deltas, delta_pos_compare);

	ಆದರೆ (repo_has_promisor_remote(the_repository)) {
		/*
		 * Prefetch the delta bases.
		 */
		ರಚನೆ oid_array ಗಮ್ಯ_fetch = OID_ARRAY_INIT;
		ಗಾಗಿ (i = 0; i < nr_ref_deltas; i++) {
			ರಚನೆ ref_delta_ನಮೂದು *d = sorted_by_pos[i];
			ಆದರೆ (!odb_read_object_info_extended(the_repository->objects,
							   &d->oid, ಖಾಲಿ,
							   OBJECT_INFO_FOR_PREFETCH))
				ಮುಂದುವರಿ;
			oid_array_append(&ಗಮ್ಯ_fetch, &d->oid);
		}
		promisor_remote_get_ನೇರ(the_repository,
					   ಗಮ್ಯ_fetch.oid, ಗಮ್ಯ_fetch.nr);
		oid_array_clear(&ಗಮ್ಯ_fetch);
	}

	ಗಾಗಿ (i = 0; i < nr_ref_deltas; i++) {
		ರಚನೆ ref_delta_ನಮೂದು *d = sorted_by_pos[i];
		ಎಣಿಕೆ object_type type;
		ಶೂನ್ಯ *ದತ್ತಾಂಶ;
		ಗಾತ್ರ_t ಗಾತ್ರ_2;

		ಆದರೆ (objects[d->obj_no].real_type != OBJ_REF_DELTA)
			ಮುಂದುವರಿ;
		ದತ್ತಾಂಶ = odb_read_object(the_repository->objects, &d->oid,
				       &type, &ಗಾತ್ರ_2);
		ಆದರೆ (!ದತ್ತಾಂಶ)
			ಮುಂದುವರಿ;

		ಆದರೆ (check_object_signature(the_repository, &d->oid, ದತ್ತಾಂಶ, ಗಾತ್ರ_2,
					   type) < 0)
			die(_("local object %s is corrupt"), oid_ಗಮ್ಯ_hex(&d->oid));

		/*
		 * Add this as an object to the objects array and call
		 * threaded_second_pass() (which will pick up the added
		 * object).
		 */
		append_obj_ಗಮ್ಯ_pack(f, d->oid.hash, ದತ್ತಾಂಶ, ಗಾತ್ರ_2, type);
		ಬಿಡುಗಡೆ(ದತ್ತಾಂಶ);
		threaded_ಎರಡನೇ_pass(ಖಾಲಿ);

		display_ಪ್ರಗತಿ(ಪ್ರಗತಿ, nr_resolved_deltas);
	}
	ಬಿಡುಗಡೆ(sorted_by_pos);
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *derive_filename(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *pack_name, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *strip,
				   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *suffix, ರಚನೆ strbuf *buf)
{
	ಗಾತ್ರ_t len;
	ಆದರೆ (!strip_suffix(pack_name, strip, &len) || !len ||
	    pack_name[len - 1] != '.')
		die(_("packfile name '%s' does not end with '.%s'"),
		    pack_name, strip);
	strbuf_ಕೂಡಿಸು(buf, pack_name, len);
	strbuf_addstr(buf, suffix);
	ಹಿಂತಿರುಗಿ buf->buf;
}

ಸ್ಥಿರ ಶೂನ್ಯ write_special_file(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *suffix, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ,
			       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *pack_name, ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *hash,
			       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **report)
{
	ರಚನೆ strbuf name_buf = STRBUF_INIT;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *filename;
	ಪೂರ್ಣಾಂಕ fd;
	ಪೂರ್ಣಾಂಕ ಸಂದೇಶ_len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(ಸಂದೇಶ);

	ಆದರೆ (pack_name)
		filename = derive_filename(pack_name, "pack", suffix, &name_buf);
	ಇಲ್ಲದಿದ್ದರೆ
		filename = odb_pack_name(the_repository, &name_buf, hash, suffix);

	fd = ಸುರಕ್ಷಿತ_create_file_with_leading_directories(the_repository, filename);
	ಆದರೆ (fd < 0) {
		ಆದರೆ (errno != EEXIST)
			die_errno(_("cannot write %s file '%s'"),
				  suffix, filename);
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಆದರೆ (ಸಂದೇಶ_len > 0) {
			write_or_die(fd, ಸಂದೇಶ, ಸಂದೇಶ_len);
			write_or_die(fd, "\n", 1);
		}
		ಆದರೆ (close(fd) != 0)
			die_errno(_("cannot close written %s file '%s'"),
				  suffix, filename);
		ಆದರೆ (report)
			*report = suffix;
	}
	strbuf_release(&name_buf);
}

ಸ್ಥಿರ ಶೂನ್ಯ rename_tmp_packfile(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **final_name,
				ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *curr_name,
				ರಚನೆ strbuf *name, ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *hash,
				ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ext, ಪೂರ್ಣಾಂಕ make_read_only_if_same)
{
	ಆದರೆ (!*final_name || strcmp(*final_name, curr_name)) {
		ಆದರೆ (!*final_name)
			*final_name = odb_pack_name(the_repository, name, hash, ext);
		ಆದರೆ (finalize_object_file(the_repository, curr_name, *final_name))
			die(_("unable to rename temporary '*.%s' file to '%s'"),
			    ext, *final_name);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (make_read_only_if_same) {
		chmod(*final_name, 0444);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ final(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *final_pack_name, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *curr_pack_name,
		  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *final_ಸೂಚ್ಯಂಕ_name, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *curr_ಸೂಚ್ಯಂಕ_name,
		  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *final_rev_ಸೂಚ್ಯಂಕ_name, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *curr_rev_ಸೂಚ್ಯಂಕ_name,
		  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *keep_ಸಂದೇಶ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *promisor_ಸಂದೇಶ,
		  ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *hash)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *report = "pack";
	ರಚನೆ strbuf pack_name = STRBUF_INIT;
	ರಚನೆ strbuf ಸೂಚ್ಯಂಕ_name = STRBUF_INIT;
	ರಚನೆ strbuf rev_ಸೂಚ್ಯಂಕ_name = STRBUF_INIT;

	ಆದರೆ (!from_stdin) {
		close(ಒಳಹರಿವು_fd);
	} ಇಲ್ಲದಿದ್ದರೆ {
		fsync_component_or_die(FSYNC_COMPONENT_PACK, output_fd, curr_pack_name);
		ಆದರೆ (close(output_fd))
			die_errno(_("error while closing pack file"));
	}

	ಆದರೆ (keep_ಸಂದೇಶ)
		write_special_file("keep", keep_ಸಂದೇಶ, final_pack_name, hash,
				   &report);
	ಆದರೆ (promisor_ಸಂದೇಶ)
		write_special_file("promisor", promisor_ಸಂದೇಶ, final_pack_name,
				   hash, ಖಾಲಿ);

	rename_tmp_packfile(&final_pack_name, curr_pack_name, &pack_name,
			    hash, "pack", from_stdin);
	ಆದರೆ (curr_rev_ಸೂಚ್ಯಂಕ_name)
		rename_tmp_packfile(&final_rev_ಸೂಚ್ಯಂಕ_name, curr_rev_ಸೂಚ್ಯಂಕ_name,
				    &rev_ಸೂಚ್ಯಂಕ_name, hash, "rev", 1);
	rename_tmp_packfile(&final_ಸೂಚ್ಯಂಕ_name, curr_ಸೂಚ್ಯಂಕ_name, &ಸೂಚ್ಯಂಕ_name,
			    hash, "idx", 1);

	ಆದರೆ (do_fsck_object && startup_info->have_repository) {
		ರಚನೆ odb_ಮೂಲ_files *files =
			odb_ಮೂಲ_files_downcast(the_repository->objects->ಮೂಲ);
		packfile_store_ತುಂಬು_pack(files->dirs->packed, final_ಸೂಚ್ಯಂಕ_name, 0);
	}

	ಆದರೆ (!from_stdin) {
		ಬರೆ("%s\n", hash_ಗಮ್ಯ_hex(hash));
	} ಇಲ್ಲದಿದ್ದರೆ {
		ರಚನೆ strbuf buf = STRBUF_INIT;

		strbuf_addf(&buf, "%s\t%s\n", report, hash_ಗಮ್ಯ_hex(hash));
		write_or_die(1, buf.buf, buf.len);
		strbuf_release(&buf);

		/* Write the last part of the buffer to stdout */
		write_ಒಳ_ತುಂಬಿದ(1, ಒಳಹರಿವು_ಬಫರ್ + ಒಳಹರಿವು_ಸ್ಥಾನಾಂತರ, ಒಳಹರಿವು_len);
	}

	strbuf_release(&rev_ಸೂಚ್ಯಂಕ_name);
	strbuf_release(&ಸೂಚ್ಯಂಕ_name);
	strbuf_release(&pack_name);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ git_ಸೂಚ್ಯಂಕ_pack_config(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *k, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *v,
				 ಸ್ಥಿರಾಂಕ ರಚನೆ config_context *ctx, ಶೂನ್ಯ *cb)
{
	ರಚನೆ pack_idx_option *opts = cb;

	ಆದರೆ (!strcmp(k, "pack.indexversion")) {
		opts->ಆವೃತ್ತಿ = git_config_int(k, v, ctx->kvi);
		ಆದರೆ (opts->ಆವೃತ್ತಿ > 2)
			die(_("bad pack.indexVersion=%"PRIu32), opts->ಆವೃತ್ತಿ);
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (!strcmp(k, "pack.threads")) {
		nr_ಎಳೆಗಳು = git_config_int(k, v, ctx->kvi);
		ಆದರೆ (nr_ಎಳೆಗಳು < 0)
			die(_("invalid number of threads specified (%d)"),
			    nr_ಎಳೆಗಳು);
		ಆದರೆ (!HAVE_ಎಳೆಗಳು && nr_ಎಳೆಗಳು != 1) {
			warning(_("no threads support, ignoring %s"), k);
			nr_ಎಳೆಗಳು = 1;
		}
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (!strcmp(k, "pack.writereverseindex")) {
		ಆದರೆ (git_config_bool(k, v))
			opts->flags |= WRITE_REV;
		ಇಲ್ಲದಿದ್ದರೆ
			opts->flags &= ~WRITE_REV;
	}
	ಆದರೆ (!strcmp(k, "core.deltabasecachelimit")) {
		opts->delta_ತಳ_cache_ಎಲ್ಲೆ = git_config_ulong(k, v, ctx->kvi);
		ಹಿಂತಿರುಗಿ 0;
	}
	ಹಿಂತಿರುಗಿ git_default_config(k, v, ctx, cb);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cmp_uint32(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *a_, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *b_)
{
	uint32_t a = *((uint32_t *)a_);
	uint32_t b = *((uint32_t *)b_);

	ಹಿಂತಿರುಗಿ (a < b) ? -1 : (a != b);
}

ಸ್ಥಿರ ಶೂನ್ಯ read_v2_anomalous_offsets(ರಚನೆ packed_git *p,
				      ರಚನೆ pack_idx_option *opts)
{
	ಸ್ಥಿರಾಂಕ uint32_t *idx1, *idx2;
	uint32_t i;

	/* The address of the 4-byte offset table */
	idx1 = (((ಸ್ಥಿರಾಂಕ uint32_t *)((ಸ್ಥಿರಾಂಕ uint8_t *)p->ಸೂಚ್ಯಂಕ_ದತ್ತಾಂಶ + p->ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_ಸ್ಥಾನಾಂತರ))
		+ (ಗಾತ್ರ_t)p->ಅಂಕೆ_objects /* CRC32 table */
		);

	/* The address of the 8-byte offset table */
	idx2 = idx1 + p->ಅಂಕೆ_objects;

	ಗಾಗಿ (i = 0; i < p->ಅಂಕೆ_objects; i++) {
		uint32_t off = ntohl(idx1[i]);
		ಆದರೆ (!(off & 0x80000000))
			ಮುಂದುವರಿ;
		off = off & 0x7fffffff;
		check_pack_ಸೂಚ್ಯಂಕ_ptr(p, &idx2[off * 2]);
		ಆದರೆ (idx2[off * 2])
			ಮುಂದುವರಿ;
		/*
		 * The real offset is ntohl(idx2[off * 2]) in high 4
		 * octets, and ntohl(idx2[off * 2 + 1]) in low 4
		 * octets.  But idx2[off * 2] is Zero!!!
		 */
		ALLOC_GROW(opts->anomaly, opts->anomaly_nr + 1, opts->anomaly_alloc);
		opts->anomaly[opts->anomaly_nr++] = ntohl(idx2[off * 2 + 1]);
	}

	QSORT(opts->anomaly, opts->anomaly_nr, cmp_uint32);
}

ಸ್ಥಿರ ಶೂನ್ಯ read_idx_option(ರಚನೆ pack_idx_option *opts, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *pack_name)
{
	ರಚನೆ packed_git *p = ಕೂಡಿಸು_packed_git(the_repository, pack_name,
					      ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(pack_name), 1);

	ಆದರೆ (!p)
		die(_("Cannot open existing pack file '%s'"), pack_name);
	ಆದರೆ (open_pack_ಸೂಚ್ಯಂಕ(p))
		die(_("Cannot open existing pack idx file for '%s'"), pack_name);

	/* Read the attributes from the existing idx file */
	opts->ಆವೃತ್ತಿ = p->ಸೂಚ್ಯಂಕ_ಆವೃತ್ತಿ;

	ಆದರೆ (opts->ಆವೃತ್ತಿ == 2)
		read_v2_anomalous_offsets(p, opts);

	/*
	 * Get rid of the idx file as we do not need it anymore.
	 * NEEDSWORK: extract this bit from free_pack_by_name() in
	 * object-file.c, perhaps?  It shouldn't matter very much as we
	 * know we haven't installed this pack (hence we never have
	 * read anything from it).
	 */
	close_pack_ಸೂಚ್ಯಂಕ(p);
	ಬಿಡುಗಡೆ(p);
}

ಸ್ಥಿರ ಶೂನ್ಯ show_pack_info(ಪೂರ್ಣಾಂಕ stat_only)
{
	ಪೂರ್ಣಾಂಕ i, baseobjects = nr_objects - nr_ref_deltas - nr_ofs_deltas;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ *chain_histogram = ಖಾಲಿ;

	ಆದರೆ (ಅತ್ಯಂತಆಳ_delta)
		CALLOC_ARRAY(chain_histogram, ಅತ್ಯಂತಆಳ_delta);

	ಗಾಗಿ (i = 0; i < nr_objects; i++) {
		ರಚನೆ object_ನಮೂದು *obj = &objects[i];

		ಆದರೆ (is_delta_type(obj->type))
			chain_histogram[obj_stat[i].delta_ಆಳ - 1]++;
		ಆದರೆ (stat_only)
			ಮುಂದುವರಿ;
		ಬರೆ("%s %-6s %"PRIuMAX" %"PRIuMAX" %"PRIuMAX,
		       oid_ಗಮ್ಯ_hex(&obj->idx.oid),
		       type_name(obj->real_type), (uintmax_t)obj->ಗಾತ್ರ_2,
		       (uintmax_t)(obj[1].idx.ಸ್ಥಾನಾಂತರ - obj->idx.ಸ್ಥಾನಾಂತರ),
		       (uintmax_t)obj->idx.ಸ್ಥಾನಾಂತರ);
		ಆದರೆ (is_delta_type(obj->type)) {
			ರಚನೆ object_ನಮೂದು *bobj = &objects[obj_stat[i].ತಳ_object_no];
			ಬರೆ(" %u %s", obj_stat[i].delta_ಆಳ,
			       oid_ಗಮ್ಯ_hex(&bobj->idx.oid));
		}
		ಅಕ್ಷರ_ಬರೆ('\n');
	}

	ಆದರೆ (baseobjects)
		printf_ln(Q_("non delta: %d object",
			     "non delta: %d objects",
			     baseobjects),
			  baseobjects);
	ಗಾಗಿ (i = 0; i < ಅತ್ಯಂತಆಳ_delta; i++) {
		ಆದರೆ (!chain_histogram[i])
			ಮುಂದುವರಿ;
		printf_ln(Q_("chain length = %d: %lu object",
			     "chain length = %d: %lu objects",
			     chain_histogram[i]),
			  i + 1,
			  chain_histogram[i]);
	}
	ಬಿಡುಗಡೆ(chain_histogram);
}

ಸ್ಥಿರ ಶೂನ್ಯ repack_local_links(ಶೂನ್ಯ)
{
	ರಚನೆ ಮಗು_process cmd = ಮಗು_PROCESS_INIT;
	FILE *ಹೊರಹೋಗು;
	ರಚನೆ strbuf ಸಾಲು = STRBUF_INIT;
	ರಚನೆ oidset_iter iter;
	ರಚನೆ object_id *oid;
	ಅಕ್ಷರ *ತಳ_name = ಖಾಲಿ;

	ಆದರೆ (!oidset_ಗಾತ್ರ(&outgoing_links))
		ಹಿಂತಿರುಗಿ;

	oidset_iter_init(&outgoing_links, &iter);
	ಆಗ ((oid = oidset_iter_ಮುಂದಿನ(&iter))) {
		ರಚನೆ odb_ಮೂಲ_info ಮೂಲ_info;
		ರಚನೆ object_info info = {
			.ಮೂಲ_infop = &ಮೂಲ_info,
		};

		ಆದರೆ (odb_read_object_info_extended(the_repository->objects, oid, &info, 0))
			/* Missing; assume it is a promisor object */
			ಮುಂದುವರಿ;
		ಆದರೆ (ಮೂಲ_info.ಮೂಲ->type == ODB_ಮೂಲ_PACKED &&
		    ಮೂಲ_info.u.packed.pack->pack_promisor)
			ಮುಂದುವರಿ;

		ಆದರೆ (!cmd.ವಾದಗಳು.nr) {
			ತಳ_name = mkpathdup(
				"%s/pack/pack",
				repo_get_object_directory(the_repository));
			strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, "pack-objects");
			strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು,
				    "--exclude-promisor-objects-best-effort");
			strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, ತಳ_name);
			cmd.git_cmd = 1;
			cmd.ಒಳ = -1;
			cmd.ಹೊರಹೋಗು = -1;
			ಆದರೆ (ಆರಂಭ_command(&cmd))
				die(_("could not start pack-objects to repack local links"));
		}

		ಆದರೆ (write_ಒಳ_ತುಂಬಿದ(cmd.ಒಳ, oid_ಗಮ್ಯ_hex(oid), the_hash_algo->hexsz) < 0 ||
		    write_ಒಳ_ತುಂಬಿದ(cmd.ಒಳ, "\n", 1) < 0)
			die(_("failed to feed local object to pack-objects"));
	}

	ಆದರೆ (!cmd.ವಾದಗಳು.nr)
		ಹಿಂತಿರುಗಿ;

	close(cmd.ಒಳ);

	ಹೊರಹೋಗು = xfdopen(cmd.ಹೊರಹೋಗು, "r");
	ಆಗ (strbuf_getline_lf(&ಸಾಲು, ಹೊರಹೋಗು) != EOF) {
		ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ ದ್ವಿಮಾನ[GIT_MAX_RAWSZ];
		ಆದರೆ (ಸಾಲು.len != the_hash_algo->hexsz ||
		    !hex_ಗಮ್ಯ_ಬೈಟುಗಳು(ದ್ವಿಮಾನ, ಸಾಲು.buf, ಸಾಲು.len))
			die(_("index-pack: Expecting full hex object ID lines only from pack-objects."));

		/*
		 * pack-objects creates the .pack and .idx files, but not the
		 * .promisor file. Create the .promisor file, which is empty.
		 */
		write_special_file("promisor", "", ಖಾಲಿ, ದ್ವಿಮಾನ, ಖಾಲಿ);
	}

	fclose(ಹೊರಹೋಗು);
	ಆದರೆ (ಮುಕ್ತಾಯ_command(&cmd))
		die(_("could not finish pack-objects to repack local links"));
	strbuf_release(&ಸಾಲು);
	ಬಿಡುಗಡೆ(ತಳ_name);
}

ಪೂರ್ಣಾಂಕ cmd_ಸೂಚ್ಯಂಕ_pack(ಪೂರ್ಣಾಂಕ argc,
		   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
		   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		   ರಚನೆ repository *repo)
{
	ಪೂರ್ಣಾಂಕ i, fix_thin_pack = 0, verify = 0, stat_only = 0, rev_ಸೂಚ್ಯಂಕ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *curr_ಸೂಚ್ಯಂಕ;
	ಅಕ್ಷರ *curr_rev_ಸೂಚ್ಯಂಕ = ಖಾಲಿ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸೂಚ್ಯಂಕ_name = ಖಾಲಿ, *pack_name = ಖಾಲಿ, *rev_ಸೂಚ್ಯಂಕ_name = ಖಾಲಿ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *keep_ಸಂದೇಶ = ಖಾಲಿ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *promisor_ಸಂದೇಶ = ಖಾಲಿ;
	ರಚನೆ strbuf ಸೂಚ್ಯಂಕ_name_buf = STRBUF_INIT;
	ರಚನೆ strbuf rev_ಸೂಚ್ಯಂಕ_name_buf = STRBUF_INIT;
	ರಚನೆ pack_idx_ನಮೂದು **idx_objects;
	ರಚನೆ pack_idx_option opts;
	ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ pack_hash[GIT_MAX_RAWSZ];
	ಚಿಹ್ನೆರಹಿತ foreign_nr = 1;	/* zero is a "good" value, assume bad */
	ಪೂರ್ಣಾಂಕ report_ಕೊನೆ_ದ_ಒಳಹರಿವು = 0;
	ಪೂರ್ಣಾಂಕ hash_algo = 0;

	show_usage_if_asked(argc, argv, ಸೂಚ್ಯಂಕ_pack_usage);

	/*
	 * index-pack never needs to fetch missing objects except when
	 * REF_DELTA bases are missing (which are explicitly handled). It only
	 * accesses the repo to do hash collision checks and to check which
	 * REF_DELTA bases need to be fetched.
	 */
	(repo ? repo : the_repository)->fetch_if_missing = 0;

	disable_replace_refs();

	fsck_options_init(&fsck_options, the_repository, FSCK_OPTIONS_MISSING_GITMODULES);
	fsck_options.walk = mark_link;

	reset_pack_idx_option(&opts);
	opts.flags |= WRITE_REV;
	repo_config(the_repository, git_ಸೂಚ್ಯಂಕ_pack_config, &opts);
	ಆದರೆ (ಪೂರ್ವಪ್ರತ್ಯಯ && chdir(ಪೂರ್ವಪ್ರತ್ಯಯ))
		die(_("Cannot come back to cwd"));

	ಆದರೆ (git_env_bool(GIT_TEST_NO_WRITE_REV_ಸೂಚ್ಯಂಕ, 0))
		rev_ಸೂಚ್ಯಂಕ = 0;
	ಇಲ್ಲದಿದ್ದರೆ
		rev_ಸೂಚ್ಯಂಕ = !!(opts.flags & (WRITE_REV_VERIFY | WRITE_REV));

	ಗಾಗಿ (i = 1; i < argc; i++) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg = argv[i];

		ಆದರೆ (*arg == '-') {
			ಆದರೆ (!strcmp(arg, "--stdin")) {
				from_stdin = 1;
			} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(arg, "--fix-thin")) {
				fix_thin_pack = 1;
			} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (skip_ಗಮ್ಯ_optional_arg(arg, "--strict", &arg)) {
				strict = 1;
				do_fsck_object = 1;
				fsck_ಗಣ_ಸಂದೇಶ_types(&fsck_options, arg);
			} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(arg, "--check-self-contained-and-connected")) {
				strict = 1;
				check_self_contained_and_connected = 1;
			} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (skip_ಗಮ್ಯ_optional_arg(arg, "--fsck-objects", &arg)) {
				do_fsck_object = 1;
				fsck_ಗಣ_ಸಂದೇಶ_types(&fsck_options, arg);
			} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(arg, "--verify")) {
				verify = 1;
			} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(arg, "--verify-stat")) {
				verify = 1;
				show_stat = 1;
			} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(arg, "--verify-stat-only")) {
				verify = 1;
				show_stat = 1;
				stat_only = 1;
			} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (skip_ಗಮ್ಯ_optional_arg(arg, "--keep", &keep_ಸಂದೇಶ)) {
				; /* nothing to do */
			} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (skip_ಗಮ್ಯ_optional_arg(arg, "--promisor", &promisor_ಸಂದೇಶ)) {
				ದಾಖಲೆ_outgoing_links = 1;
			} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (starts_with(arg, "--threads=")) {
				ಅಕ್ಷರ *ಕೊನೆ;
				nr_ಎಳೆಗಳು = strtoul(arg+10, &ಕೊನೆ, 0);
				ಆದರೆ (!arg[10] || *ಕೊನೆ || nr_ಎಳೆಗಳು < 0)
					usage(ಸೂಚ್ಯಂಕ_pack_usage);
				ಆದರೆ (!HAVE_ಎಳೆಗಳು && nr_ಎಳೆಗಳು != 1) {
					warning(_("no threads support, ignoring %s"), arg);
					nr_ಎಳೆಗಳು = 1;
				}
			} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(arg, "--pack_header=", &arg)) {
				ಆದರೆ (parse_pack_ಶೀರ್ಷಿಕೆ_option(arg,
							     ಒಳಹರಿವು_ಬಫರ್,
							     &ಒಳಹರಿವು_len) < 0)
					die(_("bad --pack_header: %s"), arg);
			} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(arg, "-v")) {
				verbose = 1;
			} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(arg, "--progress-title")) {
				ಆದರೆ (ಪ್ರಗತಿ_title || (i+1) >= argc)
					usage(ಸೂಚ್ಯಂಕ_pack_usage);
				ಪ್ರಗತಿ_title = argv[++i];
			} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(arg, "--show-resolving-progress")) {
				show_resolving_ಪ್ರಗತಿ = 1;
			} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(arg, "--report-end-of-input")) {
				report_ಕೊನೆ_ದ_ಒಳಹರಿವು = 1;
			} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(arg, "-o")) {
				ಆದರೆ (ಸೂಚ್ಯಂಕ_name || (i+1) >= argc)
					usage(ಸೂಚ್ಯಂಕ_pack_usage);
				ಸೂಚ್ಯಂಕ_name = argv[++i];
			} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (starts_with(arg, "--index-version=")) {
				ಅಕ್ಷರ *c;
				opts.ಆವೃತ್ತಿ = strtoul(arg + 16, &c, 10);
				ಆದರೆ (opts.ಆವೃತ್ತಿ > 2)
					die(_("bad %s"), arg);
				ಆದರೆ (*c == ',')
					opts.off32_ಎಲ್ಲೆ = strtoul(c+1, &c, 0);
				ಆದರೆ (*c || opts.off32_ಎಲ್ಲೆ & 0x80000000)
					die(_("bad %s"), arg);
			} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(arg, "--max-input-size=", &arg)) {
				max_ಒಳಹರಿವು_ಗಾತ್ರ = strtoumax(arg, ಖಾಲಿ, 10);
			} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(arg, "--object-format=", &arg)) {
				hash_algo = hash_algo_by_name(arg);
				ಆದರೆ (hash_algo == GIT_HASH_UNKNOWN)
					die(_("unknown hash algorithm '%s'"), arg);
				repo_ಗಣ_hash_algo(the_repository, hash_algo);
			} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(arg, "--rev-index")) {
				rev_ಸೂಚ್ಯಂಕ = 1;
			} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(arg, "--no-rev-index")) {
				rev_ಸೂಚ್ಯಂಕ = 0;
			} ಇಲ್ಲದಿದ್ದರೆ
				usage(ಸೂಚ್ಯಂಕ_pack_usage);
			ಮುಂದುವರಿ;
		}

		ಆದರೆ (pack_name)
			usage(ಸೂಚ್ಯಂಕ_pack_usage);
		pack_name = arg;
	}

	ಆದರೆ (!pack_name && !from_stdin)
		usage(ಸೂಚ್ಯಂಕ_pack_usage);
	ಆದರೆ (fix_thin_pack && !from_stdin)
		die(_("the option '%s' requires '%s'"), "--fix-thin", "--stdin");
	ಆದರೆ (promisor_ಸಂದೇಶ && pack_name)
		die(_("--promisor cannot be used with a pack name"));
	ಆದರೆ (from_stdin && !startup_info->have_repository)
		die(_("--stdin requires a git repository"));
	ಆದರೆ (from_stdin && hash_algo)
		die(_("options '%s' and '%s' cannot be used together"), "--object-format", "--stdin");
	ಆದರೆ (!ಸೂಚ್ಯಂಕ_name && pack_name)
		ಸೂಚ್ಯಂಕ_name = derive_filename(pack_name, "pack", "idx", &ಸೂಚ್ಯಂಕ_name_buf);

	/*
	 * Packfiles and indices do not carry enough information to be able to
	 * identify their object hash. So when we are neither in a repository
	 * nor has the user told us which object hash to use we have no other
	 * choice but to guess the object hash.
	 */
	ಆದರೆ (!the_repository->hash_algo)
		repo_ಗಣ_hash_algo(the_repository, GIT_HASH_DEFAULT);

	opts.flags &= ~(WRITE_REV | WRITE_REV_VERIFY);
	ಆದರೆ (rev_ಸೂಚ್ಯಂಕ) {
		opts.flags |= verify ? WRITE_REV_VERIFY : WRITE_REV;
		ಆದರೆ (ಸೂಚ್ಯಂಕ_name)
			rev_ಸೂಚ್ಯಂಕ_name = derive_filename(ಸೂಚ್ಯಂಕ_name,
							 "idx", "rev",
							 &rev_ಸೂಚ್ಯಂಕ_name_buf);
	}

	ಆದರೆ (verify) {
		ಆದರೆ (!ಸೂಚ್ಯಂಕ_name)
			die(_("--verify with no packfile name given"));
		read_idx_option(&opts, ಸೂಚ್ಯಂಕ_name);
		opts.flags |= WRITE_IDX_VERIFY | WRITE_IDX_STRICT;
	}
	ಆದರೆ (strict)
		opts.flags |= WRITE_IDX_STRICT;

	ಆದರೆ (HAVE_ಎಳೆಗಳು && !nr_ಎಳೆಗಳು) {
		nr_ಎಳೆಗಳು = online_cpus();
		/*
		 * Experiments show that going above 20 threads doesn't help,
		 * no matter how many cores you have. Below that, we tend to
		 * max at half the number of online_cpus(), presumably because
		 * half of those are hyperthreads rather than full cores. We'll
		 * never reduce the level below "3", though, to match a
		 * historical value that nobody complained about.
		 */
		ಆದರೆ (nr_ಎಳೆಗಳು < 4)
			; /* too few cores to consider capping */
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (nr_ಎಳೆಗಳು < 6)
			nr_ಎಳೆಗಳು = 3; /* historic cap */
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (nr_ಎಳೆಗಳು < 40)
			nr_ಎಳೆಗಳು /= 2;
		ಇಲ್ಲದಿದ್ದರೆ
			nr_ಎಳೆಗಳು = 20; /* hard cap */
	}

	curr_pack = open_pack_file(pack_name);
	parse_pack_ಶೀರ್ಷಿಕೆ();
	CALLOC_ARRAY(objects, st_ಕೂಡಿಸು(nr_objects, 1));
	ಆದರೆ (show_stat)
		CALLOC_ARRAY(obj_stat, st_ಕೂಡಿಸು(nr_objects, 1));
	CALLOC_ARRAY(ofs_deltas, nr_objects);
	parse_pack_objects(pack_hash);
	ಆದರೆ (report_ಕೊನೆ_ದ_ಒಳಹರಿವು)
		write_ಒಳ_ತುಂಬಿದ(2, "\0", 1);
	resolve_deltas(&opts);
	conclude_pack(fix_thin_pack, curr_pack, pack_hash);
	ಬಿಡುಗಡೆ(ofs_deltas);
	ಬಿಡುಗಡೆ(ref_deltas);
	ಆದರೆ (strict)
		foreign_nr = check_objects();

	ಆದರೆ (show_stat)
		show_pack_info(stat_only);

	ALLOC_ARRAY(idx_objects, nr_objects);
	ಗಾಗಿ (i = 0; i < nr_objects; i++)
		idx_objects[i] = &objects[i].idx;
	curr_ಸೂಚ್ಯಂಕ = write_idx_file(the_repository, ಸೂಚ್ಯಂಕ_name, idx_objects,
				    nr_objects, &opts, pack_hash);
	ಆದರೆ (rev_ಸೂಚ್ಯಂಕ)
		curr_rev_ಸೂಚ್ಯಂಕ = write_rev_file(the_repository, rev_ಸೂಚ್ಯಂಕ_name,
						idx_objects, nr_objects,
						pack_hash, opts.flags);
	ಬಿಡುಗಡೆ(idx_objects);

	ಆದರೆ (!verify)
		final(pack_name, curr_pack,
		      ಸೂಚ್ಯಂಕ_name, curr_ಸೂಚ್ಯಂಕ,
		      rev_ಸೂಚ್ಯಂಕ_name, curr_rev_ಸೂಚ್ಯಂಕ,
		      keep_ಸಂದೇಶ, promisor_ಸಂದೇಶ,
		      pack_hash);
	ಇಲ್ಲದಿದ್ದರೆ
		close(ಒಳಹರಿವು_fd);

	ಆದರೆ (do_fsck_object) {
		/*
		 * We cannot perform queued consistency checks when running
		 * outside of a repository because those require us to read
		 * from the object database, which is uninitialized.
		 *
		 * TODO: we may eventually set up an in-memory object database,
		 * which would allow us to perform these queued checks.
		 */
		ಆದರೆ (!startup_info->have_repository &&
		    fsck_has_queued_checks(&fsck_options))
			die(_("cannot perform queued object checks outside "
			      "of a repository"));

		ಆದರೆ (fsck_ಮುಕ್ತಾಯ(&fsck_options))
			die(_("fsck error in pack objects"));
	}

	ಬಿಡುಗಡೆ(opts.anomaly);
	ಬಿಡುಗಡೆ(objects);
	strbuf_release(&ಸೂಚ್ಯಂಕ_name_buf);
	strbuf_release(&rev_ಸೂಚ್ಯಂಕ_name_buf);
	ಆದರೆ (!pack_name)
		ಬಿಡುಗಡೆ((ಶೂನ್ಯ *) curr_pack);
	ಆದರೆ (!ಸೂಚ್ಯಂಕ_name)
		ಬಿಡುಗಡೆ((ಶೂನ್ಯ *) curr_ಸೂಚ್ಯಂಕ);
	ಬಿಡುಗಡೆ(curr_rev_ಸೂಚ್ಯಂಕ);

	repack_local_links();

	/*
	 * Let the caller know this pack is not self contained
	 */
	ಆದರೆ (check_self_contained_and_connected && foreign_nr)
		ಹಿಂತಿರುಗಿ 1;

	ಹಿಂತಿರುಗಿ 0;
}
