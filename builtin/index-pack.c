#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DISABLE_SIGN_COMPARE_WARNINGS

#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "delta.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hex.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "pack.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "csum-file.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "blob.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "commit.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "tag.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "tree.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "progress.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "fsck.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "strbuf.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "thread-utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "packfile.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "pack-revindex.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object-file.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "odb.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "odb/streaming.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "oid-array.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "oidset.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "path.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "replace-object.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "tree-walk.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "promisor-remote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "run-command.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "setup.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "strvec.h"

ਸਥਿਰ ਅਚਲ ਅੱਖਰ ਸੂਚਕਾਂਕ_pack_usage[] =
"git index-pack [-v] [-o <index-file>] [--keep | --keep=<msg>] [--[no-]rev-index] [--verify] [--strict[=<msg-id>=<severity>...]] [--fsck-objects[=<msg-id>=<severity>...]] (<pack-file> | --stdin [--fix-thin] [<pack-file>])";

ਬਣਤਰ object_ਇੰਦਰਾਜ {
	ਬਣਤਰ pack_idx_ਇੰਦਰਾਜ idx;
	ਮਾਪ_t ਮਾਪ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ hdr_ਮਾਪ;
	ਨਿਸ਼ਾਨੀਵਾਲਾ ਅੱਖਰ type;
	ਨਿਸ਼ਾਨੀਵਾਲਾ ਅੱਖਰ real_type;
};

ਬਣਤਰ object_stat {
	ਬਿਨਾਨਿਸ਼ਾਨੀ delta_ਡੂੰਘਾਈ;
	ਪੂਰਨ ਅਧਾਰ_object_no;
};

ਬਣਤਰ ਅਧਾਰ_ਅੰਕੜੇ {
	/* Initialized by make_base(). */
	ਬਣਤਰ ਅਧਾਰ_ਅੰਕੜੇ *ਅਧਾਰ;
	ਬਣਤਰ object_ਇੰਦਰਾਜ *obj;
	ਪੂਰਨ ref_ਪਹਿਲਾ, ref_ਆਖਰੀ;
	ਪੂਰਨ ofs_ਪਹਿਲਾ, ofs_ਆਖਰੀ;
	/*
	 * Threads should increment retain_data if they are about to call
	 * patch_delta() using this struct's data as a base, and decrement this
	 * when they are done. While retain_data is nonzero, this struct's data
	 * will not be freed even if the delta base cache limit is exceeded.
	 */
	ਪੂਰਨ retain_ਅੰਕੜੇ;
	/*
	 * The number of direct children that have not been fully processed
	 * (entered work_head, entered done_head, left done_head). When this
	 * number reaches zero, this struct base_data can be freed.
	 */
	ਪੂਰਨ children_ਬਚਿਆ;

	/* Not initialized by make_base(). */
	ਬਣਤਰ list_ਸਿਰ list;
	ਖਾਲੀ *ਅੰਕੜੇ;
	ਮਾਪ_t ਮਾਪ;
};

/*
 * Stack of struct base_data that have unprocessed children.
 * threaded_second_pass() uses this as a source of work (the other being the
 * objects array).
 *
 * Guarded by work_mutex.
 */
ਸਥਿਰ LIST_ਸਿਰ(work_ਸਿਰ);

/*
 * Stack of struct base_data that have children, all of whom have been
 * processed or are being processed, and at least one child is being processed.
 * These struct base_data must be kept around until the last child is
 * processed.
 *
 * Guarded by work_mutex.
 */
ਸਥਿਰ LIST_ਸਿਰ(ਪੂਰਾ_ਸਿਰ);

/*
 * All threads share one delta base cache.
 *
 * base_cache_used is guarded by work_mutex, and base_cache_limit is read-only
 * in a thread.
 */
ਸਥਿਰ ਮਾਪ_t ਅਧਾਰ_cache_ਵਰਤਿਆ;
ਸਥਿਰ ਮਾਪ_t ਅਧਾਰ_cache_ਹੱਦਬੰਦੀ;

ਬਣਤਰ thread_local_ਅੰਕੜੇ {
	pthread_t thread;
	ਪੂਰਨ pack_fd;
};

/* Remember to update object flag allocation in object.h */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FLAG_LINK (1u<<20)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FLAG_CHECKED (1u<<21)

ਬਣਤਰ ofs_delta_ਇੰਦਰਾਜ {
	off_t ਵਿੱਥ;
	ਪੂਰਨ obj_no;
};

ਬਣਤਰ ref_delta_ਇੰਦਰਾਜ {
	ਬਣਤਰ object_id oid;
	ਪੂਰਨ obj_no;
};

ਸਥਿਰ ਬਣਤਰ object_ਇੰਦਰਾਜ *objects;
ਸਥਿਰ ਬਣਤਰ object_stat *obj_stat;
ਸਥਿਰ ਬਣਤਰ ofs_delta_ਇੰਦਰਾਜ *ofs_deltas;
ਸਥਿਰ ਬਣਤਰ ref_delta_ਇੰਦਰਾਜ *ref_deltas;
ਸਥਿਰ ਬਣਤਰ thread_local_ਅੰਕੜੇ nothread_ਅੰਕੜੇ;
ਸਥਿਰ ਪੂਰਨ nr_objects;
ਸਥਿਰ ਪੂਰਨ nr_ofs_deltas;
ਸਥਿਰ ਪੂਰਨ nr_ref_deltas;
ਸਥਿਰ ਪੂਰਨ ref_deltas_alloc;
ਸਥਿਰ ਪੂਰਨ nr_resolved_deltas;
ਸਥਿਰ ਪੂਰਨ nr_ਧਾਗੇ;

ਸਥਿਰ ਪੂਰਨ from_stdin;
ਸਥਿਰ ਪੂਰਨ strict;
ਸਥਿਰ ਪੂਰਨ do_fsck_object;
ਸਥਿਰ ਬਣਤਰ fsck_options fsck_options;
ਸਥਿਰ ਪੂਰਨ verbose;
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਤਰੱਕੀ_title;
ਸਥਿਰ ਪੂਰਨ show_resolving_ਤਰੱਕੀ;
ਸਥਿਰ ਪੂਰਨ show_stat;
ਸਥਿਰ ਪੂਰਨ check_self_contained_and_connected;

ਸਥਿਰ ਬਣਤਰ ਤਰੱਕੀ *ਤਰੱਕੀ;

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ ਨਿਵੇਸ਼_ਬਫਰ[DEFAULT_IO_ਬਫਰ_ਮਾਪ];
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਨਿਵੇਸ਼_ਵਿੱਥ, ਨਿਵੇਸ਼_len;
ਸਥਿਰ off_t consumed_ਬਾਈਟਾਂ;
ਸਥਿਰ off_t max_ਨਿਵੇਸ਼_ਮਾਪ;
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਭ_ਤੋਂ_ਡੂੰਘਾ_delta;
ਸਥਿਰ ਬਣਤਰ git_hash_ctx ਨਿਵੇਸ਼_ctx;
ਸਥਿਰ uint32_t ਨਿਵੇਸ਼_ਚੱਕਰੀ_ਜਾਂਚ_32;
ਸਥਿਰ ਪੂਰਨ ਨਿਵੇਸ਼_fd, output_fd;
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *curr_pack;

/*
 * outgoing_links is guarded by read_mutex, and record_outgoing_links is
 * read-only in a thread.
 */
ਸਥਿਰ ਬਣਤਰ oidset outgoing_links = OIDSET_INIT;
ਸਥਿਰ ਪੂਰਨ ਰਿਕਾਰਡ_outgoing_links;

ਸਥਿਰ ਬਣਤਰ thread_local_ਅੰਕੜੇ *thread_ਅੰਕੜੇ;
ਸਥਿਰ ਪੂਰਨ nr_dispatched;
ਸਥਿਰ ਪੂਰਨ ਧਾਗੇ_ਸਰਗਰਮ;

ਸਥਿਰ pthread_mutex_t read_mutex;
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ read_ਤਾਲਾ()		ਤਾਲਾ_mutex(&read_mutex)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ read_unlock()		unlock_mutex(&read_mutex)

ਸਥਿਰ pthread_mutex_t counter_mutex;
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ counter_ਤਾਲਾ()		ਤਾਲਾ_mutex(&counter_mutex)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ counter_unlock()	unlock_mutex(&counter_mutex)

ਸਥਿਰ pthread_mutex_t work_mutex;
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ work_ਤਾਲਾ()		ਤਾਲਾ_mutex(&work_mutex)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ work_unlock()		unlock_mutex(&work_mutex)

ਸਥਿਰ pthread_mutex_t ਸਭ_ਤੋਂ_ਡੂੰਘਾ_delta_mutex;
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਸਭ_ਤੋਂ_ਡੂੰਘਾ_delta_ਤਾਲਾ()	ਤਾਲਾ_mutex(&ਸਭ_ਤੋਂ_ਡੂੰਘਾ_delta_mutex)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਸਭ_ਤੋਂ_ਡੂੰਘਾ_delta_unlock()	unlock_mutex(&ਸਭ_ਤੋਂ_ਡੂੰਘਾ_delta_mutex)

ਸਥਿਰ pthread_ਕੁੰਜੀ_t ਕੁੰਜੀ;

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ ਤਾਲਾ_mutex(pthread_mutex_t *mutex)
{
	ਜੇ (ਧਾਗੇ_ਸਰਗਰਮ)
		pthread_mutex_ਤਾਲਾ(mutex);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ unlock_mutex(pthread_mutex_t *mutex)
{
	ਜੇ (ਧਾਗੇ_ਸਰਗਰਮ)
		pthread_mutex_unlock(mutex);
}

/*
 * Mutex and conditional variable can't be statically-initialized on Windows.
 */
ਸਥਿਰ ਖਾਲੀ init_thread(ਖਾਲੀ)
{
	ਪੂਰਨ i;
	init_recursive_mutex(&read_mutex);
	pthread_mutex_init(&counter_mutex, ਨਿਰਾ);
	pthread_mutex_init(&work_mutex, ਨਿਰਾ);
	ਜੇ (show_stat)
		pthread_mutex_init(&ਸਭ_ਤੋਂ_ਡੂੰਘਾ_delta_mutex, ਨਿਰਾ);
	pthread_ਕੁੰਜੀ_create(&ਕੁੰਜੀ, ਨਿਰਾ);
	CALLOC_ARRAY(thread_ਅੰਕੜੇ, nr_ਧਾਗੇ);
	ਲਈ (i = 0; i < nr_ਧਾਗੇ; i++) {
		thread_ਅੰਕੜੇ[i].pack_fd = xopen(curr_pack, O_RDONLY);
	}

	ਧਾਗੇ_ਸਰਗਰਮ = 1;
}

ਸਥਿਰ ਖਾਲੀ cleanup_thread(ਖਾਲੀ)
{
	ਪੂਰਨ i;
	ਜੇ (!ਧਾਗੇ_ਸਰਗਰਮ)
		ਵਾਪਸ;
	ਧਾਗੇ_ਸਰਗਰਮ = 0;
	pthread_mutex_destroy(&read_mutex);
	pthread_mutex_destroy(&counter_mutex);
	pthread_mutex_destroy(&work_mutex);
	ਜੇ (show_stat)
		pthread_mutex_destroy(&ਸਭ_ਤੋਂ_ਡੂੰਘਾ_delta_mutex);
	ਲਈ (i = 0; i < nr_ਧਾਗੇ; i++)
		close(thread_ਅੰਕੜੇ[i].pack_fd);
	pthread_ਕੁੰਜੀ_delete(ਕੁੰਜੀ);
	ਮੁਕਤ(thread_ਅੰਕੜੇ);
}

ਸਥਿਰ ਪੂਰਨ mark_link(ਬਣਤਰ object *obj, ਗਿਣਤੀ object_type type,
		     ਖਾਲੀ *ਅੰਕੜੇ ਅਣਵਰਤਿਆ,
		     ਬਣਤਰ fsck_options *options ਅਣਵਰਤਿਆ)
{
	ਜੇ (!obj)
		ਵਾਪਸ -1;

	ਜੇ (type != OBJ_ANY && obj->type != type)
		die(_("object type mismatch at %s"), oid_ਮੰਜ਼ਿਲ_hex(&obj->oid));

	obj->flags |= FLAG_LINK;
	ਵਾਪਸ 0;
}

/* The content of each linked object must have been checked
   or it must be already present in the object database */
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ check_object(ਬਣਤਰ object *obj)
{
	ਜੇ (!obj)
		ਵਾਪਸ 0;

	ਜੇ (!(obj->flags & FLAG_LINK))
		ਵਾਪਸ 0;

	ਜੇ (!(obj->flags & FLAG_CHECKED)) {
		ਮਾਪ_t ਮਾਪ;
		ਪੂਰਨ type = odb_read_object_info(the_repository->objects,
						&obj->oid, &ਮਾਪ);
		ਜੇ (type <= 0)
			die(_("did not receive expected object %s"),
			      oid_ਮੰਜ਼ਿਲ_hex(&obj->oid));
		ਜੇ (type != obj->type)
			die(_("object %s: expected type %s, found %s"),
			    oid_ਮੰਜ਼ਿਲ_hex(&obj->oid),
			    type_name(obj->type), type_name(type));
		obj->flags |= FLAG_CHECKED;
		ਵਾਪਸ 1;
	}

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ check_objects(ਖਾਲੀ)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ i, max, foreign_nr = 0;

	max = get_max_object_ਸੂਚਕਾਂਕ(the_repository);

	ਜੇ (verbose)
		ਤਰੱਕੀ = ਸ਼ੁਰੂ_delayed_ਤਰੱਕੀ(the_repository,
						  _("Checking objects"), max);

	ਲਈ (i = 0; i < max; i++) {
		foreign_nr += check_object(get_ਸੂਚਕਾਂਕਿਤ_object(the_repository, i));
		display_ਤਰੱਕੀ(ਤਰੱਕੀ, i + 1);
	}

	stop_ਤਰੱਕੀ(&ਤਰੱਕੀ);
	ਵਾਪਸ foreign_nr;
}


/* Discard current buffer used content. */
ਸਥਿਰ ਖਾਲੀ flush(ਖਾਲੀ)
{
	ਜੇ (ਨਿਵੇਸ਼_ਵਿੱਥ) {
		ਜੇ (output_fd >= 0)
			write_or_die(output_fd, ਨਿਵੇਸ਼_ਬਫਰ, ਨਿਵੇਸ਼_ਵਿੱਥ);
		git_hash_update(&ਨਿਵੇਸ਼_ctx, ਨਿਵੇਸ਼_ਬਫਰ, ਨਿਵੇਸ਼_ਵਿੱਥ);
		memmove(ਨਿਵੇਸ਼_ਬਫਰ, ਨਿਵੇਸ਼_ਬਫਰ + ਨਿਵੇਸ਼_ਵਿੱਥ, ਨਿਵੇਸ਼_len);
		ਨਿਵੇਸ਼_ਵਿੱਥ = 0;
	}
}

/*
 * Make sure at least "min" bytes are available in the buffer, and
 * return the pointer to the buffer.
 */
ਸਥਿਰ ਖਾਲੀ *fill(ਪੂਰਨ min)
{
	ਜੇ (min <= ਨਿਵੇਸ਼_len)
		ਵਾਪਸ ਨਿਵੇਸ਼_ਬਫਰ + ਨਿਵੇਸ਼_ਵਿੱਥ;
	ਜੇ (min > ਆਕਾਰ(ਨਿਵੇਸ਼_ਬਫਰ))
		die(Q_("cannot fill %d byte",
		       "cannot fill %d bytes",
		       min),
		    min);
	flush();
	ਕਰੋ {
		ssize_t ret = xread(ਨਿਵੇਸ਼_fd, ਨਿਵੇਸ਼_ਬਫਰ + ਨਿਵੇਸ਼_len,
				ਆਕਾਰ(ਨਿਵੇਸ਼_ਬਫਰ) - ਨਿਵੇਸ਼_len);
		ਜੇ (ret <= 0) {
			ਜੇ (!ret)
				die(_("early EOF"));
			die_errno(_("read error on input"));
		}
		ਨਿਵੇਸ਼_len += ret;
		ਜੇ (from_stdin)
			display_throughput(ਤਰੱਕੀ, consumed_ਬਾਈਟਾਂ + ਨਿਵੇਸ਼_len);
	} ਜਦੋਂ (ਨਿਵੇਸ਼_len < min);
	ਵਾਪਸ ਨਿਵੇਸ਼_ਬਫਰ;
}

ਸਥਿਰ ਖਾਲੀ use(ਪੂਰਨ ਬਾਈਟਾਂ)
{
	ਜੇ (ਬਾਈਟਾਂ > ਨਿਵੇਸ਼_len)
		die(_("used more bytes than were available"));
	ਨਿਵੇਸ਼_ਚੱਕਰੀ_ਜਾਂਚ_32 = ਚੱਕਰੀ_ਜਾਂਚ_32(ਨਿਵੇਸ਼_ਚੱਕਰੀ_ਜਾਂਚ_32, ਨਿਵੇਸ਼_ਬਫਰ + ਨਿਵੇਸ਼_ਵਿੱਥ, ਬਾਈਟਾਂ);
	ਨਿਵੇਸ਼_len -= ਬਾਈਟਾਂ;
	ਨਿਵੇਸ਼_ਵਿੱਥ += ਬਾਈਟਾਂ;

	/* make sure off_t is sufficiently large not to wrap */
	ਜੇ (signed_ਜੋੜੋ_overflows(consumed_ਬਾਈਟਾਂ, ਬਾਈਟਾਂ))
		die(_("pack too large for current definition of off_t"));
	consumed_ਬਾਈਟਾਂ += ਬਾਈਟਾਂ;
	ਜੇ (max_ਨਿਵੇਸ਼_ਮਾਪ && consumed_ਬਾਈਟਾਂ > max_ਨਿਵੇਸ਼_ਮਾਪ) {
		ਬਣਤਰ strbuf ਮਾਪ_ਹੱਦਬੰਦੀ = STRBUF_INIT;
		strbuf_humanise_ਬਾਈਟਾਂ(&ਮਾਪ_ਹੱਦਬੰਦੀ, max_ਨਿਵੇਸ਼_ਮਾਪ);
		die(_("pack exceeds maximum allowed size (%s)"),
		    ਮਾਪ_ਹੱਦਬੰਦੀ.buf);
	}
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *open_pack_file(ਅਚਲ ਅੱਖਰ *pack_name)
{
	ਜੇ (from_stdin) {
		ਨਿਵੇਸ਼_fd = 0;
		ਜੇ (!pack_name) {
			ਬਣਤਰ strbuf tmp_file = STRBUF_INIT;
			output_fd = odb_mkstemp(the_repository->objects, &tmp_file,
						"pack/tmp_pack_XXXXXX");
			pack_name = strbuf_detach(&tmp_file, ਨਿਰਾ);
		} ਨਹੀਂਤਾਂ {
			output_fd = xopen(pack_name, O_CREAT|O_EXCL|O_RDWR, 0600);
		}
		nothread_ਅੰਕੜੇ.pack_fd = output_fd;
	} ਨਹੀਂਤਾਂ {
		ਨਿਵੇਸ਼_fd = xopen(pack_name, O_RDONLY);
		output_fd = -1;
		nothread_ਅੰਕੜੇ.pack_fd = ਨਿਵੇਸ਼_fd;
	}
	git_hash_init(&ਨਿਵੇਸ਼_ctx, the_hash_algo);
	ਵਾਪਸ pack_name;
}

ਸਥਿਰ ਖਾਲੀ parse_pack_ਸਿਰਲੇਖ(ਖਾਲੀ)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *hdr = fill(ਆਕਾਰ(ਬਣਤਰ pack_ਸਿਰਲੇਖ));

	/* Header consistency check */
	ਜੇ (get_be32(hdr) != PACK_SIGNATURE)
		die(_("pack signature mismatch"));
	hdr += 4;
	ਜੇ (!pack_ਸੰਸਕਰਣ_ok_native(get_be32(hdr)))
		die(_("pack version %"PRIu32" unsupported"),
		    get_be32(hdr));
	hdr += 4;

	nr_objects = get_be32(hdr);
	use(ਆਕਾਰ(ਬਣਤਰ pack_ਸਿਰਲੇਖ));
}

__attribute__((format (ਲਿਖੋ, 2, 3)))
ਸਥਿਰ NORETURN ਖਾਲੀ bad_object(off_t ਵਿੱਥ, ਅਚਲ ਅੱਖਰ *format, ...)
{
	va_list params;
	ਅੱਖਰ buf[1024];

	va_ਸ਼ੁਰੂ(params, format);
	vsnprintf(buf, ਆਕਾਰ(buf), format, params);
	va_ਸਿਰਾ(params);
	die(_("pack has bad object at offset %"PRIuMAX": %s"),
	    (uintmax_t)ਵਿੱਥ, buf);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ thread_local_ਅੰਕੜੇ *get_thread_ਅੰਕੜੇ(ਖਾਲੀ)
{
	ਜੇ (HAVE_ਧਾਗੇ) {
		ਜੇ (ਧਾਗੇ_ਸਰਗਰਮ)
			ਵਾਪਸ pthread_getspecific(ਕੁੰਜੀ);
		ਪ੍ਰਤਿਜ੍ਞਾ(!ਧਾਗੇ_ਸਰਗਰਮ &&
		       "This should only be reached when all threads are gone");
	}
	ਵਾਪਸ &nothread_ਅੰਕੜੇ;
}

ਸਥਿਰ ਖਾਲੀ ਸੈੱਟ_thread_ਅੰਕੜੇ(ਬਣਤਰ thread_local_ਅੰਕੜੇ *ਅੰਕੜੇ)
{
	ਜੇ (ਧਾਗੇ_ਸਰਗਰਮ)
		pthread_setspecific(ਕੁੰਜੀ, ਅੰਕੜੇ);
}

ਸਥਿਰ ਖਾਲੀ free_ਅਧਾਰ_ਅੰਕੜੇ(ਬਣਤਰ ਅਧਾਰ_ਅੰਕੜੇ *c)
{
	ਜੇ (c->ਅੰਕੜੇ) {
		FREE_AND_NULL(c->ਅੰਕੜੇ);
		ਅਧਾਰ_cache_ਵਰਤਿਆ -= c->ਮਾਪ;
	}
}

ਸਥਿਰ ਖਾਲੀ prune_ਅਧਾਰ_ਅੰਕੜੇ(ਬਣਤਰ ਅਧਾਰ_ਅੰਕੜੇ *retain)
{
	ਬਣਤਰ list_ਸਿਰ *pos;

	ਜੇ (ਅਧਾਰ_cache_ਵਰਤਿਆ <= ਅਧਾਰ_cache_ਹੱਦਬੰਦੀ)
		ਵਾਪਸ;

	list_for_each_prev(pos, &ਪੂਰਾ_ਸਿਰ) {
		ਬਣਤਰ ਅਧਾਰ_ਅੰਕੜੇ *b = list_ਇੰਦਰਾਜ(pos, ਬਣਤਰ ਅਧਾਰ_ਅੰਕੜੇ, list);
		ਜੇ (b->retain_ਅੰਕੜੇ || b == retain)
			ਜਾਰੀ;
		ਜੇ (b->ਅੰਕੜੇ) {
			free_ਅਧਾਰ_ਅੰਕੜੇ(b);
			ਜੇ (ਅਧਾਰ_cache_ਵਰਤਿਆ <= ਅਧਾਰ_cache_ਹੱਦਬੰਦੀ)
				ਵਾਪਸ;
		}
	}

	list_for_each_prev(pos, &work_ਸਿਰ) {
		ਬਣਤਰ ਅਧਾਰ_ਅੰਕੜੇ *b = list_ਇੰਦਰਾਜ(pos, ਬਣਤਰ ਅਧਾਰ_ਅੰਕੜੇ, list);
		ਜੇ (b->retain_ਅੰਕੜੇ || b == retain)
			ਜਾਰੀ;
		ਜੇ (b->ਅੰਕੜੇ) {
			free_ਅਧਾਰ_ਅੰਕੜੇ(b);
			ਜੇ (ਅਧਾਰ_cache_ਵਰਤਿਆ <= ਅਧਾਰ_cache_ਹੱਦਬੰਦੀ)
				ਵਾਪਸ;
		}
	}
}

ਸਥਿਰ ਪੂਰਨ is_delta_type(ਗਿਣਤੀ object_type type)
{
	ਵਾਪਸ (type == OBJ_REF_DELTA || type == OBJ_OFS_DELTA);
}

ਸਥਿਰ ਖਾਲੀ *unpack_ਇੰਦਰਾਜ_ਅੰਕੜੇ(off_t ਵਿੱਥ, ਮਾਪ_t ਮਾਪ,
			       ਗਿਣਤੀ object_type type, ਬਣਤਰ object_id *oid)
{
	ਸਥਿਰ ਅੱਖਰ fixed_buf[8192];
	ਪੂਰਨ ਸਥਿਤੀ;
	git_zstream ਧਾਰਾ;
	ਖਾਲੀ *buf;
	ਬਣਤਰ git_hash_ctx c;
	ਅੱਖਰ hdr[32];
	ਪੂਰਨ hdrlen;

	ਜੇ (!is_delta_type(type)) {
		hdrlen = format_object_ਸਿਰਲੇਖ(hdr, ਆਕਾਰ(hdr), type, ਮਾਪ);
		git_hash_init(&c, the_hash_algo);
		git_hash_update(&c, hdr, hdrlen);
	} ਨਹੀਂਤਾਂ
		oid = ਨਿਰਾ;
	ਜੇ (type == OBJ_BLOB &&
	    ਮਾਪ > repo_settings_get_big_file_ਹੱਦ(the_repository))
		buf = fixed_buf;
	ਨਹੀਂਤਾਂ
		buf = xmallocz(ਮਾਪ);

	memset(&ਧਾਰਾ, 0, ਆਕਾਰ(ਧਾਰਾ));
	git_inflate_init(&ਧਾਰਾ);
	ਧਾਰਾ.ਅਗਲਾ_ਬਾਹਰਲਾ = buf;
	ਧਾਰਾ.avail_ਬਾਹਰਲਾ = buf == fixed_buf ? ਆਕਾਰ(fixed_buf) : ਮਾਪ;

	ਕਰੋ {
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *ਆਖਰੀ_ਬਾਹਰਲਾ = ਧਾਰਾ.ਅਗਲਾ_ਬਾਹਰਲਾ;
		ਧਾਰਾ.ਅਗਲਾ_ਵਿੱਚ = fill(1);
		ਧਾਰਾ.avail_ਵਿੱਚ = ਨਿਵੇਸ਼_len;
		ਸਥਿਤੀ = git_inflate(&ਧਾਰਾ, 0);
		use(ਨਿਵੇਸ਼_len - ਧਾਰਾ.avail_ਵਿੱਚ);
		ਜੇ (oid)
			git_hash_update(&c, ਆਖਰੀ_ਬਾਹਰਲਾ, ਧਾਰਾ.ਅਗਲਾ_ਬਾਹਰਲਾ - ਆਖਰੀ_ਬਾਹਰਲਾ);
		ਜੇ (buf == fixed_buf) {
			ਧਾਰਾ.ਅਗਲਾ_ਬਾਹਰਲਾ = buf;
			ਧਾਰਾ.avail_ਬਾਹਰਲਾ = ਆਕਾਰ(fixed_buf);
		}
	} ਜਦੋਂ (ਸਥਿਤੀ == Z_OK);
	ਜੇ (ਧਾਰਾ.ਕੁੱਲ_ਬਾਹਰਲਾ != ਮਾਪ || ਸਥਿਤੀ != Z_ਧਾਰਾ_ਸਿਰਾ)
		bad_object(ਵਿੱਥ, _("inflate returned %d"), ਸਥਿਤੀ);
	git_inflate_ਸਿਰਾ(&ਧਾਰਾ);
	ਜੇ (oid)
		git_hash_final_oid(oid, &c);
	ਵਾਪਸ buf == fixed_buf ? ਨਿਰਾ : buf;
}

ਸਥਿਰ ਖਾਲੀ *unpack_raw_ਇੰਦਰਾਜ(ਬਣਤਰ object_ਇੰਦਰਾਜ *obj,
			      off_t *ofs_ਵਿੱਥ,
			      ਬਣਤਰ object_id *ref_oid,
			      ਬਣਤਰ object_id *oid)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *p;
	ਮਾਪ_t ਮਾਪ, c;
	off_t ਅਧਾਰ_ਵਿੱਥ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਖਿਸਕਾਓ;
	ਖਾਲੀ *ਅੰਕੜੇ;

	obj->idx.ਵਿੱਥ = consumed_ਬਾਈਟਾਂ;
	ਨਿਵੇਸ਼_ਚੱਕਰੀ_ਜਾਂਚ_32 = ਚੱਕਰੀ_ਜਾਂਚ_32(0, ਨਿਰਾ, 0);

	p = fill(1);
	c = *p;
	use(1);
	obj->type = (c >> 4) & 7;
	ਮਾਪ = (c & 15);
	ਖਿਸਕਾਓ = 4;
	ਜਦੋਂ (c & 0x80) {
		ਜੇ ((bitsizeof(ਮਾਪ_t) - 7) < ਖਿਸਕਾਓ)
			die(_("object size too large for this platform"));
		p = fill(1);
		c = *p;
		use(1);
		ਮਾਪ += (c & 0x7f) << ਖਿਸਕਾਓ;
		ਖਿਸਕਾਓ += 7;
	}
	obj->ਮਾਪ = ਮਾਪ;

	ਚੋਣ (obj->type) {
	ਹਾਲਤ OBJ_REF_DELTA:
		oidread(ref_oid, fill(the_hash_algo->rawsz),
			the_repository->hash_algo);
		use(the_hash_algo->rawsz);
		ਰੋਕੋ;
	ਹਾਲਤ OBJ_OFS_DELTA:
		p = fill(1);
		c = *p;
		use(1);
		ਅਧਾਰ_ਵਿੱਥ = c & 127;
		ਜਦੋਂ (c & 128) {
			ਅਧਾਰ_ਵਿੱਥ += 1;
			ਜੇ (!ਅਧਾਰ_ਵਿੱਥ || MSB(ਅਧਾਰ_ਵਿੱਥ, 7))
				bad_object(obj->idx.ਵਿੱਥ, _("offset value overflow for delta base object"));
			p = fill(1);
			c = *p;
			use(1);
			ਅਧਾਰ_ਵਿੱਥ = (ਅਧਾਰ_ਵਿੱਥ << 7) + (c & 127);
		}
		*ofs_ਵਿੱਥ = obj->idx.ਵਿੱਥ - ਅਧਾਰ_ਵਿੱਥ;
		ਜੇ (*ofs_ਵਿੱਥ <= 0 || *ofs_ਵਿੱਥ >= obj->idx.ਵਿੱਥ)
			bad_object(obj->idx.ਵਿੱਥ, _("delta base offset is out of bound"));
		ਰੋਕੋ;
	ਹਾਲਤ OBJ_COMMIT:
	ਹਾਲਤ OBJ_ਰੁੱਖ:
	ਹਾਲਤ OBJ_BLOB:
	ਹਾਲਤ OBJ_ਟੈਗ:
		ਰੋਕੋ;
	ਮੂਲ:
		bad_object(obj->idx.ਵਿੱਥ, _("unknown object type %d"), obj->type);
	}
	obj->hdr_ਮਾਪ = consumed_ਬਾਈਟਾਂ - obj->idx.ਵਿੱਥ;

	ਅੰਕੜੇ = unpack_ਇੰਦਰਾਜ_ਅੰਕੜੇ(obj->idx.ਵਿੱਥ, obj->ਮਾਪ, obj->type, oid);
	obj->idx.ਚੱਕਰੀ_ਜਾਂਚ_32 = ਨਿਵੇਸ਼_ਚੱਕਰੀ_ਜਾਂਚ_32;
	ਵਾਪਸ ਅੰਕੜੇ;
}

ਸਥਿਰ ਖਾਲੀ *unpack_ਅੰਕੜੇ(ਬਣਤਰ object_ਇੰਦਰਾਜ *obj,
			 ਪੂਰਨ (*consume)(ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ, ਖਾਲੀ *),
			 ਖਾਲੀ *cb_ਅੰਕੜੇ)
{
	off_t from = obj[0].idx.ਵਿੱਥ + obj[0].hdr_ਮਾਪ;
	off_t len = obj[1].idx.ਵਿੱਥ - from;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *ਅੰਕੜੇ, *inbuf;
	git_zstream ਧਾਰਾ;
	ਪੂਰਨ ਸਥਿਤੀ;

	ਅੰਕੜੇ = xmallocz(consume ? 64*1024 : obj->ਮਾਪ);
	inbuf = xmalloc((len < 64*1024) ? (ਪੂਰਨ)len : 64*1024);

	memset(&ਧਾਰਾ, 0, ਆਕਾਰ(ਧਾਰਾ));
	git_inflate_init(&ਧਾਰਾ);
	ਧਾਰਾ.ਅਗਲਾ_ਬਾਹਰਲਾ = ਅੰਕੜੇ;
	ਧਾਰਾ.avail_ਬਾਹਰਲਾ = consume ? 64*1024 : obj->ਮਾਪ;

	ਕਰੋ {
		ssize_t n = (len < 64*1024) ? (ssize_t)len : 64*1024;
		n = xpread(get_thread_ਅੰਕੜੇ()->pack_fd, inbuf, n, from);
		ਜੇ (n < 0)
			die_errno(_("cannot pread pack file"));
		ਜੇ (!n)
			die(Q_("premature end of pack file, %"PRIuMAX" byte missing",
			       "premature end of pack file, %"PRIuMAX" bytes missing",
			       len),
			    (uintmax_t)len);
		from += n;
		len -= n;
		ਧਾਰਾ.ਅਗਲਾ_ਵਿੱਚ = inbuf;
		ਧਾਰਾ.avail_ਵਿੱਚ = n;
		ਜੇ (!consume)
			ਸਥਿਤੀ = git_inflate(&ਧਾਰਾ, 0);
		ਨਹੀਂਤਾਂ {
			ਕਰੋ {
				ਸਥਿਤੀ = git_inflate(&ਧਾਰਾ, 0);
				ਜੇ (consume(ਅੰਕੜੇ, ਧਾਰਾ.ਅਗਲਾ_ਬਾਹਰਲਾ - ਅੰਕੜੇ, cb_ਅੰਕੜੇ)) {
					ਮੁਕਤ(inbuf);
					ਮੁਕਤ(ਅੰਕੜੇ);
					ਵਾਪਸ ਨਿਰਾ;
				}
				ਧਾਰਾ.ਅਗਲਾ_ਬਾਹਰਲਾ = ਅੰਕੜੇ;
				ਧਾਰਾ.avail_ਬਾਹਰਲਾ = 64*1024;
			} ਜਦੋਂ (ਸਥਿਤੀ == Z_OK && ਧਾਰਾ.avail_ਵਿੱਚ);
		}
	} ਜਦੋਂ (len && ਸਥਿਤੀ == Z_OK && !ਧਾਰਾ.avail_ਵਿੱਚ);

	/* This has been inflated OK when first encountered, so... */
	ਜੇ (ਸਥਿਤੀ != Z_ਧਾਰਾ_ਸਿਰਾ || ਧਾਰਾ.ਕੁੱਲ_ਬਾਹਰਲਾ != obj->ਮਾਪ)
		die(_("serious inflate inconsistency"));

	git_inflate_ਸਿਰਾ(&ਧਾਰਾ);
	ਮੁਕਤ(inbuf);
	ਜੇ (consume) {
		FREE_AND_NULL(ਅੰਕੜੇ);
	}
	ਵਾਪਸ ਅੰਕੜੇ;
}

ਸਥਿਰ ਖਾਲੀ *get_ਅੰਕੜੇ_from_pack(ਬਣਤਰ object_ਇੰਦਰਾਜ *obj)
{
	ਵਾਪਸ unpack_ਅੰਕੜੇ(obj, ਨਿਰਾ, ਨਿਰਾ);
}

ਸਥਿਰ ਪੂਰਨ compare_ofs_delta_bases(off_t offset1, off_t offset2,
				   ਗਿਣਤੀ object_type type1,
				   ਗਿਣਤੀ object_type type2)
{
	ਪੂਰਨ cmp = type1 - type2;
	ਜੇ (cmp)
		ਵਾਪਸ cmp;
	ਵਾਪਸ offset1 < offset2 ? -1 :
	       offset1 > offset2 ?  1 :
	       0;
}

ਸਥਿਰ ਪੂਰਨ ਲੱਭੋ_ofs_delta(ਅਚਲ off_t ਵਿੱਥ)
{
	ਪੂਰਨ ਪਹਿਲਾ = 0, ਆਖਰੀ = nr_ofs_deltas;

	ਜਦੋਂ (ਪਹਿਲਾ < ਆਖਰੀ) {
		ਪੂਰਨ ਅਗਲਾ = ਪਹਿਲਾ + (ਆਖਰੀ - ਪਹਿਲਾ) / 2;
		ਬਣਤਰ ofs_delta_ਇੰਦਰਾਜ *delta = &ofs_deltas[ਅਗਲਾ];
		ਪੂਰਨ cmp;

		cmp = compare_ofs_delta_bases(ਵਿੱਥ, delta->ਵਿੱਥ,
					      OBJ_OFS_DELTA,
					      objects[delta->obj_no].type);
		ਜੇ (!cmp)
			ਵਾਪਸ ਅਗਲਾ;
		ਜੇ (cmp < 0) {
			ਆਖਰੀ = ਅਗਲਾ;
			ਜਾਰੀ;
		}
		ਪਹਿਲਾ = ਅਗਲਾ+1;
	}
	ਵਾਪਸ -ਪਹਿਲਾ-1;
}

ਸਥਿਰ ਖਾਲੀ ਲੱਭੋ_ofs_delta_children(off_t ਵਿੱਥ,
				    ਪੂਰਨ *ਪਹਿਲਾ_ਸੂਚਕਾਂਕ, ਪੂਰਨ *ਆਖਰੀ_ਸੂਚਕਾਂਕ)
{
	ਪੂਰਨ ਪਹਿਲਾ = ਲੱਭੋ_ofs_delta(ਵਿੱਥ);
	ਪੂਰਨ ਆਖਰੀ = ਪਹਿਲਾ;
	ਪੂਰਨ ਸਿਰਾ = nr_ofs_deltas - 1;

	ਜੇ (ਪਹਿਲਾ < 0) {
		*ਪਹਿਲਾ_ਸੂਚਕਾਂਕ = 0;
		*ਆਖਰੀ_ਸੂਚਕਾਂਕ = -1;
		ਵਾਪਸ;
	}
	ਜਦੋਂ (ਪਹਿਲਾ > 0 && ofs_deltas[ਪਹਿਲਾ - 1].ਵਿੱਥ == ਵਿੱਥ)
		--ਪਹਿਲਾ;
	ਜਦੋਂ (ਆਖਰੀ < ਸਿਰਾ && ofs_deltas[ਆਖਰੀ + 1].ਵਿੱਥ == ਵਿੱਥ)
		++ਆਖਰੀ;
	*ਪਹਿਲਾ_ਸੂਚਕਾਂਕ = ਪਹਿਲਾ;
	*ਆਖਰੀ_ਸੂਚਕਾਂਕ = ਆਖਰੀ;
}

ਸਥਿਰ ਪੂਰਨ compare_ref_delta_bases(ਅਚਲ ਬਣਤਰ object_id *oid1,
				   ਅਚਲ ਬਣਤਰ object_id *oid2,
				   ਗਿਣਤੀ object_type type1,
				   ਗਿਣਤੀ object_type type2)
{
	ਪੂਰਨ cmp = type1 - type2;
	ਜੇ (cmp)
		ਵਾਪਸ cmp;
	ਵਾਪਸ oidcmp(oid1, oid2);
}

ਸਥਿਰ ਪੂਰਨ ਲੱਭੋ_ref_delta(ਅਚਲ ਬਣਤਰ object_id *oid)
{
	ਪੂਰਨ ਪਹਿਲਾ = 0, ਆਖਰੀ = nr_ref_deltas;

	ਜਦੋਂ (ਪਹਿਲਾ < ਆਖਰੀ) {
		ਪੂਰਨ ਅਗਲਾ = ਪਹਿਲਾ + (ਆਖਰੀ - ਪਹਿਲਾ) / 2;
		ਬਣਤਰ ref_delta_ਇੰਦਰਾਜ *delta = &ref_deltas[ਅਗਲਾ];
		ਪੂਰਨ cmp;

		cmp = compare_ref_delta_bases(oid, &delta->oid,
					      OBJ_REF_DELTA,
					      objects[delta->obj_no].type);
		ਜੇ (!cmp)
			ਵਾਪਸ ਅਗਲਾ;
		ਜੇ (cmp < 0) {
			ਆਖਰੀ = ਅਗਲਾ;
			ਜਾਰੀ;
		}
		ਪਹਿਲਾ = ਅਗਲਾ+1;
	}
	ਵਾਪਸ -ਪਹਿਲਾ-1;
}

ਸਥਿਰ ਖਾਲੀ ਲੱਭੋ_ref_delta_children(ਅਚਲ ਬਣਤਰ object_id *oid,
				    ਪੂਰਨ *ਪਹਿਲਾ_ਸੂਚਕਾਂਕ, ਪੂਰਨ *ਆਖਰੀ_ਸੂਚਕਾਂਕ)
{
	ਪੂਰਨ ਪਹਿਲਾ = ਲੱਭੋ_ref_delta(oid);
	ਪੂਰਨ ਆਖਰੀ = ਪਹਿਲਾ;
	ਪੂਰਨ ਸਿਰਾ = nr_ref_deltas - 1;

	ਜੇ (ਪਹਿਲਾ < 0) {
		*ਪਹਿਲਾ_ਸੂਚਕਾਂਕ = 0;
		*ਆਖਰੀ_ਸੂਚਕਾਂਕ = -1;
		ਵਾਪਸ;
	}
	ਜਦੋਂ (ਪਹਿਲਾ > 0 && oideq(&ref_deltas[ਪਹਿਲਾ - 1].oid, oid))
		--ਪਹਿਲਾ;
	ਜਦੋਂ (ਆਖਰੀ < ਸਿਰਾ && oideq(&ref_deltas[ਆਖਰੀ + 1].oid, oid))
		++ਆਖਰੀ;
	*ਪਹਿਲਾ_ਸੂਚਕਾਂਕ = ਪਹਿਲਾ;
	*ਆਖਰੀ_ਸੂਚਕਾਂਕ = ਆਖਰੀ;
}

ਬਣਤਰ compare_ਅੰਕੜੇ {
	ਬਣਤਰ object_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ;
	ਬਣਤਰ odb_ਧਾਰਾ *st;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *buf;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ buf_ਮਾਪ;
};

ਸਥਿਰ ਪੂਰਨ compare_objects(ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *buf, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਮਾਪ,
			   ਖਾਲੀ *cb_ਅੰਕੜੇ)
{
	ਬਣਤਰ compare_ਅੰਕੜੇ *ਅੰਕੜੇ = cb_ਅੰਕੜੇ;

	ਜੇ (ਅੰਕੜੇ->buf_ਮਾਪ < ਮਾਪ) {
		ਮੁਕਤ(ਅੰਕੜੇ->buf);
		ਅੰਕੜੇ->buf = xmalloc(ਮਾਪ);
		ਅੰਕੜੇ->buf_ਮਾਪ = ਮਾਪ;
	}

	ਜਦੋਂ (ਮਾਪ) {
		ssize_t len = odb_ਧਾਰਾ_read(ਅੰਕੜੇ->st, ਅੰਕੜੇ->buf, ਮਾਪ);
		ਜੇ (len == 0)
			die(_("SHA1 COLLISION FOUND WITH %s !"),
			    oid_ਮੰਜ਼ਿਲ_hex(&ਅੰਕੜੇ->ਇੰਦਰਾਜ->idx.oid));
		ਜੇ (len < 0)
			die(_("unable to read %s"),
			    oid_ਮੰਜ਼ਿਲ_hex(&ਅੰਕੜੇ->ਇੰਦਰਾਜ->idx.oid));
		ਜੇ (memcmp(buf, ਅੰਕੜੇ->buf, len))
			die(_("SHA1 COLLISION FOUND WITH %s !"),
			    oid_ਮੰਜ਼ਿਲ_hex(&ਅੰਕੜੇ->ਇੰਦਰਾਜ->idx.oid));
		ਮਾਪ -= len;
		buf += len;
	}
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ check_collison(ਬਣਤਰ object_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ)
{
	ਬਣਤਰ compare_ਅੰਕੜੇ ਅੰਕੜੇ;

	ਜੇ (ਇੰਦਰਾਜ->ਮਾਪ <= repo_settings_get_big_file_ਹੱਦ(the_repository) ||
	    ਇੰਦਰਾਜ->type != OBJ_BLOB)
		ਵਾਪਸ -1;

	memset(&ਅੰਕੜੇ, 0, ਆਕਾਰ(ਅੰਕੜੇ));
	ਅੰਕੜੇ.ਇੰਦਰਾਜ = ਇੰਦਰਾਜ;
	ਅੰਕੜੇ.st = odb_ਧਾਰਾ_from_object(the_repository->objects, &ਇੰਦਰਾਜ->idx.oid, ਨਿਰਾ);
	ਜੇ (!ਅੰਕੜੇ.st)
		ਵਾਪਸ -1;
	ਜੇ (ਅੰਕੜੇ.st->ਮਾਪ != ਇੰਦਰਾਜ->ਮਾਪ || ਅੰਕੜੇ.st->type != ਇੰਦਰਾਜ->type)
		die(_("SHA1 COLLISION FOUND WITH %s !"),
		    oid_ਮੰਜ਼ਿਲ_hex(&ਇੰਦਰਾਜ->idx.oid));
	unpack_ਅੰਕੜੇ(ਇੰਦਰਾਜ, compare_objects, &ਅੰਕੜੇ);
	odb_ਧਾਰਾ_close(ਅੰਕੜੇ.st);
	ਮੁਕਤ(ਅੰਕੜੇ.buf);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ ਰਿਕਾਰਡ_outgoing_link(ਅਚਲ ਬਣਤਰ object_id *oid)
{
	oidset_ਪਾਓ(&outgoing_links, oid);
}

ਸਥਿਰ ਖਾਲੀ maybe_ਰਿਕਾਰਡ_name_ਇੰਦਰਾਜ(ਅਚਲ ਬਣਤਰ name_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ)
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
	ਜੇ (S_ISDIR(ਇੰਦਰਾਜ->ਬਹੁਲਕ))
		ਰਿਕਾਰਡ_outgoing_link(&ਇੰਦਰਾਜ->oid);
}

ਸਥਿਰ ਖਾਲੀ do_ਰਿਕਾਰਡ_outgoing_links(ਬਣਤਰ object *obj)
{
	ਜੇ (obj->type == OBJ_ਰੁੱਖ) {
		ਬਣਤਰ ਰੁੱਖ *ਰੁੱਖ = (ਬਣਤਰ ਰੁੱਖ *)obj;
		ਬਣਤਰ ਰੁੱਖ_desc desc;
		ਬਣਤਰ name_ਇੰਦਰਾਜ ਇੰਦਰਾਜ;
		ਜੇ (init_ਰੁੱਖ_desc_gently(&desc, &ਰੁੱਖ->object.oid,
					  ਰੁੱਖ->ਬਫਰ, ਰੁੱਖ->ਮਾਪ, 0))
			/*
			 * Error messages are given when packs are
			 * verified, so do not print any here.
			 */
			ਵਾਪਸ;
		ਜਦੋਂ (ਰੁੱਖ_ਇੰਦਰਾਜ_gently(&desc, &ਇੰਦਰਾਜ))
			maybe_ਰਿਕਾਰਡ_name_ਇੰਦਰਾਜ(&ਇੰਦਰਾਜ);
	} ਨਹੀਂਤਾਂ ਜੇ (obj->type == OBJ_COMMIT) {
		ਬਣਤਰ commit *commit = (ਬਣਤਰ commit *) obj;
		ਬਣਤਰ commit_list *parents = commit->parents;

		ਰਿਕਾਰਡ_outgoing_link(get_commit_ਰੁੱਖ_oid(commit));
		ਲਈ (; parents; parents = parents->ਅਗਲਾ)
			ਰਿਕਾਰਡ_outgoing_link(&parents->ਵਸਤੂ->object.oid);
	} ਨਹੀਂਤਾਂ ਜੇ (obj->type == OBJ_ਟੈਗ) {
		ਬਣਤਰ ਟੈਗ *ਟੈਗ = (ਬਣਤਰ ਟੈਗ *) obj;
		ਰਿਕਾਰਡ_outgoing_link(get_tagged_oid(ਟੈਗ));
	}
}

ਸਥਿਰ ਖਾਲੀ sha1_object(ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ, ਬਣਤਰ object_ਇੰਦਰਾਜ *obj_ਇੰਦਰਾਜ,
			ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਮਾਪ, ਗਿਣਤੀ object_type type,
			ਅਚਲ ਬਣਤਰ object_id *oid)
{
	ਖਾਲੀ *new_ਅੰਕੜੇ = ਨਿਰਾ;
	ਪੂਰਨ collision_test_needed = 0;

	ਪ੍ਰਤਿਜ੍ਞਾ(ਅੰਕੜੇ || obj_ਇੰਦਰਾਜ);

	ਜੇ (startup_info->have_repository) {
		read_ਤਾਲਾ();
		collision_test_needed = odb_has_object(the_repository->objects, oid,
						       ODB_HAS_OBJECT_FETCH_PROMISOR);
		read_unlock();
	}

	ਜੇ (collision_test_needed && !ਅੰਕੜੇ) {
		read_ਤਾਲਾ();
		ਜੇ (!check_collison(obj_ਇੰਦਰਾਜ))
			collision_test_needed = 0;
		read_unlock();
	}
	ਜੇ (collision_test_needed) {
		ਖਾਲੀ *has_ਅੰਕੜੇ;
		ਗਿਣਤੀ object_type has_type;
		ਮਾਪ_t has_ਮਾਪ;
		read_ਤਾਲਾ();
		has_type = odb_read_object_info(the_repository->objects, oid, &has_ਮਾਪ);
		ਜੇ (has_type < 0)
			die(_("cannot read existing object info %s"), oid_ਮੰਜ਼ਿਲ_hex(oid));
		ਜੇ (has_type != type || has_ਮਾਪ != ਮਾਪ)
			die(_("SHA1 COLLISION FOUND WITH %s !"), oid_ਮੰਜ਼ਿਲ_hex(oid));
		has_ਅੰਕੜੇ = odb_read_object(the_repository->objects, oid,
					   &has_type, &has_ਮਾਪ);
		read_unlock();
		ਜੇ (!ਅੰਕੜੇ)
			ਅੰਕੜੇ = new_ਅੰਕੜੇ = get_ਅੰਕੜੇ_from_pack(obj_ਇੰਦਰਾਜ);
		ਜੇ (!has_ਅੰਕੜੇ)
			die(_("cannot read existing object %s"), oid_ਮੰਜ਼ਿਲ_hex(oid));
		ਜੇ (ਮਾਪ != has_ਮਾਪ || type != has_type ||
		    memcmp(ਅੰਕੜੇ, has_ਅੰਕੜੇ, ਮਾਪ) != 0)
			die(_("SHA1 COLLISION FOUND WITH %s !"), oid_ਮੰਜ਼ਿਲ_hex(oid));
		ਮੁਕਤ(has_ਅੰਕੜੇ);
	}

	ਜੇ (strict || do_fsck_object || ਰਿਕਾਰਡ_outgoing_links) {
		read_ਤਾਲਾ();
		ਜੇ (type == OBJ_BLOB) {
			ਬਣਤਰ blob *blob = lookup_blob(the_repository, oid);
			ਜੇ (blob)
				blob->object.flags |= FLAG_CHECKED;
			ਨਹੀਂਤਾਂ
				die(_("invalid blob object %s"), oid_ਮੰਜ਼ਿਲ_hex(oid));
			ਜੇ (do_fsck_object &&
			    fsck_object(&blob->object, (ਖਾਲੀ *)ਅੰਕੜੇ, ਮਾਪ, &fsck_options))
				die(_("fsck error in packed object"));
		} ਨਹੀਂਤਾਂ {
			ਬਣਤਰ object *obj;
			ਪੂਰਨ eaten;
			ਖਾਲੀ *buf = (ਖਾਲੀ *) ਅੰਕੜੇ;

			ਪ੍ਰਤਿਜ੍ਞਾ(ਅੰਕੜੇ && "data can only be NULL for large _blobs_");

			/*
			 * we do not need to free the memory here, as the
			 * buf is deleted by the caller.
			 */
			obj = parse_object_ਬਫਰ(the_repository, oid, type,
						  ਮਾਪ, buf,
						  &eaten);
			ਜੇ (!obj)
				die(_("invalid %s"), type_name(type));
			ਜੇ (do_fsck_object &&
			    fsck_object(obj, buf, ਮਾਪ, &fsck_options))
				die(_("fsck error in packed object"));
			ਜੇ (strict && fsck_walk(obj, ਨਿਰਾ, &fsck_options))
				die(_("Not all child objects of %s are reachable"), oid_ਮੰਜ਼ਿਲ_hex(&obj->oid));
			ਜੇ (ਰਿਕਾਰਡ_outgoing_links)
				do_ਰਿਕਾਰਡ_outgoing_links(obj);

			ਜੇ (obj->type == OBJ_ਰੁੱਖ) {
				ਬਣਤਰ ਰੁੱਖ *ਵਸਤੂ = (ਬਣਤਰ ਰੁੱਖ *) obj;
				ਵਸਤੂ->ਬਫਰ = ਨਿਰਾ;
				obj->parsed = 0;
			}
			ਜੇ (obj->type == OBJ_COMMIT) {
				ਬਣਤਰ commit *commit = (ਬਣਤਰ commit *) obj;
				ਜੇ (detach_commit_ਬਫਰ(commit, ਨਿਰਾ) != ਅੰਕੜੇ)
					BUG("parse_object_buffer transmogrified our buffer");
			}
			obj->flags |= FLAG_CHECKED;
		}
		read_unlock();
	}

	ਮੁਕਤ(new_ਅੰਕੜੇ);
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
ਸਥਿਰ ਖਾਲੀ *get_ਅਧਾਰ_ਅੰਕੜੇ(ਬਣਤਰ ਅਧਾਰ_ਅੰਕੜੇ *c)
{
	ਜੇ (!c->ਅੰਕੜੇ) {
		ਬਣਤਰ object_ਇੰਦਰਾਜ *obj = c->obj;
		ਬਣਤਰ ਅਧਾਰ_ਅੰਕੜੇ **delta = ਨਿਰਾ;
		ਪੂਰਨ delta_nr = 0, delta_alloc = 0;

		ਜਦੋਂ (is_delta_type(c->obj->type) && !c->ਅੰਕੜੇ) {
			ALLOC_GROW(delta, delta_nr + 1, delta_alloc);
			delta[delta_nr++] = c;
			c = c->ਅਧਾਰ;
		}
		ਜੇ (!delta_nr) {
			c->ਅੰਕੜੇ = get_ਅੰਕੜੇ_from_pack(obj);
			c->ਮਾਪ = obj->ਮਾਪ;
			ਅਧਾਰ_cache_ਵਰਤਿਆ += c->ਮਾਪ;
			prune_ਅਧਾਰ_ਅੰਕੜੇ(c);
		}
		ਲਈ (; delta_nr > 0; delta_nr--) {
			ਖਾਲੀ *ਅਧਾਰ, *raw;
			c = delta[delta_nr - 1];
			obj = c->obj;
			ਅਧਾਰ = get_ਅਧਾਰ_ਅੰਕੜੇ(c->ਅਧਾਰ);
			raw = get_ਅੰਕੜੇ_from_pack(obj);
			c->ਅੰਕੜੇ = patch_delta(
				ਅਧਾਰ, c->ਅਧਾਰ->ਮਾਪ,
				raw, obj->ਮਾਪ,
				&c->ਮਾਪ);
			ਮੁਕਤ(raw);
			ਜੇ (!c->ਅੰਕੜੇ)
				bad_object(obj->idx.ਵਿੱਥ, _("failed to apply delta"));
			ਅਧਾਰ_cache_ਵਰਤਿਆ += c->ਮਾਪ;
			prune_ਅਧਾਰ_ਅੰਕੜੇ(c);
		}
		ਮੁਕਤ(delta);
	}
	ਵਾਪਸ c->ਅੰਕੜੇ;
}

ਸਥਿਰ ਬਣਤਰ ਅਧਾਰ_ਅੰਕੜੇ *make_ਅਧਾਰ(ਬਣਤਰ object_ਇੰਦਰਾਜ *obj,
				   ਬਣਤਰ ਅਧਾਰ_ਅੰਕੜੇ *ਮਾਪਾ)
{
	ਬਣਤਰ ਅਧਾਰ_ਅੰਕੜੇ *ਅਧਾਰ = xcalloc(1, ਆਕਾਰ(ਬਣਤਰ ਅਧਾਰ_ਅੰਕੜੇ));
	ਅਧਾਰ->ਅਧਾਰ = ਮਾਪਾ;
	ਅਧਾਰ->obj = obj;
	ਲੱਭੋ_ref_delta_children(&obj->idx.oid,
				&ਅਧਾਰ->ref_ਪਹਿਲਾ, &ਅਧਾਰ->ref_ਆਖਰੀ);
	ਲੱਭੋ_ofs_delta_children(obj->idx.ਵਿੱਥ,
				&ਅਧਾਰ->ofs_ਪਹਿਲਾ, &ਅਧਾਰ->ofs_ਆਖਰੀ);
	ਅਧਾਰ->children_ਬਚਿਆ = ਅਧਾਰ->ref_ਆਖਰੀ - ਅਧਾਰ->ref_ਪਹਿਲਾ +
		ਅਧਾਰ->ofs_ਆਖਰੀ - ਅਧਾਰ->ofs_ਪਹਿਲਾ + 2;
	ਵਾਪਸ ਅਧਾਰ;
}

ਸਥਿਰ ਬਣਤਰ ਅਧਾਰ_ਅੰਕੜੇ *resolve_delta(ਬਣਤਰ object_ਇੰਦਰਾਜ *delta_obj,
				       ਬਣਤਰ ਅਧਾਰ_ਅੰਕੜੇ *ਅਧਾਰ)
{
	ਖਾਲੀ *delta_ਅੰਕੜੇ, *ਨਤੀਜਾ_ਅੰਕੜੇ;
	ਬਣਤਰ ਅਧਾਰ_ਅੰਕੜੇ *ਨਤੀਜਾ;
	ਮਾਪ_t ਨਤੀਜਾ_ਮਾਪ;

	ਜੇ (show_stat) {
		ਪੂਰਨ i = delta_obj - objects;
		ਪੂਰਨ j = ਅਧਾਰ->obj - objects;
		obj_stat[i].delta_ਡੂੰਘਾਈ = obj_stat[j].delta_ਡੂੰਘਾਈ + 1;
		ਸਭ_ਤੋਂ_ਡੂੰਘਾ_delta_ਤਾਲਾ();
		ਜੇ (ਸਭ_ਤੋਂ_ਡੂੰਘਾ_delta < obj_stat[i].delta_ਡੂੰਘਾਈ)
			ਸਭ_ਤੋਂ_ਡੂੰਘਾ_delta = obj_stat[i].delta_ਡੂੰਘਾਈ;
		ਸਭ_ਤੋਂ_ਡੂੰਘਾ_delta_unlock();
		obj_stat[i].ਅਧਾਰ_object_no = j;
	}
	delta_ਅੰਕੜੇ = get_ਅੰਕੜੇ_from_pack(delta_obj);
	ਪ੍ਰਤਿਜ੍ਞਾ(ਅਧਾਰ->ਅੰਕੜੇ);
	ਨਤੀਜਾ_ਅੰਕੜੇ = patch_delta(ਅਧਾਰ->ਅੰਕੜੇ, ਅਧਾਰ->ਮਾਪ,
				  delta_ਅੰਕੜੇ, delta_obj->ਮਾਪ, &ਨਤੀਜਾ_ਮਾਪ);
	ਮੁਕਤ(delta_ਅੰਕੜੇ);
	ਜੇ (!ਨਤੀਜਾ_ਅੰਕੜੇ)
		bad_object(delta_obj->idx.ਵਿੱਥ, _("failed to apply delta"));
	hash_object_file(the_hash_algo, ਨਤੀਜਾ_ਅੰਕੜੇ, ਨਤੀਜਾ_ਮਾਪ,
			 delta_obj->real_type, &delta_obj->idx.oid);
	sha1_object(ਨਤੀਜਾ_ਅੰਕੜੇ, ਨਿਰਾ, ਨਤੀਜਾ_ਮਾਪ, delta_obj->real_type,
		    &delta_obj->idx.oid);

	ਨਤੀਜਾ = make_ਅਧਾਰ(delta_obj, ਅਧਾਰ);
	ਨਤੀਜਾ->ਅੰਕੜੇ = ਨਤੀਜਾ_ਅੰਕੜੇ;
	ਨਤੀਜਾ->ਮਾਪ = ਨਤੀਜਾ_ਮਾਪ;

	counter_ਤਾਲਾ();
	nr_resolved_deltas++;
	counter_unlock();

	ਵਾਪਸ ਨਤੀਜਾ;
}

ਸਥਿਰ ਪੂਰਨ compare_ofs_delta_ਇੰਦਰਾਜ(ਅਚਲ ਖਾਲੀ *a, ਅਚਲ ਖਾਲੀ *b)
{
	ਅਚਲ ਬਣਤਰ ofs_delta_ਇੰਦਰਾਜ *delta_a = a;
	ਅਚਲ ਬਣਤਰ ofs_delta_ਇੰਦਰਾਜ *delta_b = b;

	ਵਾਪਸ delta_a->ਵਿੱਥ < delta_b->ਵਿੱਥ ? -1 :
	       delta_a->ਵਿੱਥ > delta_b->ਵਿੱਥ ?  1 :
	       0;
}

ਸਥਿਰ ਪੂਰਨ compare_ref_delta_ਇੰਦਰਾਜ(ਅਚਲ ਖਾਲੀ *a, ਅਚਲ ਖਾਲੀ *b)
{
	ਅਚਲ ਬਣਤਰ ref_delta_ਇੰਦਰਾਜ *delta_a = a;
	ਅਚਲ ਬਣਤਰ ref_delta_ਇੰਦਰਾਜ *delta_b = b;

	ਵਾਪਸ oidcmp(&delta_a->oid, &delta_b->oid);
}

ਸਥਿਰ ਖਾਲੀ *threaded_ਦੂਜਾ_pass(ਖਾਲੀ *ਅੰਕੜੇ)
{
	ਜੇ (ਅੰਕੜੇ)
		ਸੈੱਟ_thread_ਅੰਕੜੇ(ਅੰਕੜੇ);
	ਲਈ (;;) {
		ਬਣਤਰ ਅਧਾਰ_ਅੰਕੜੇ *ਮਾਪਾ = ਨਿਰਾ;
		ਬਣਤਰ object_ਇੰਦਰਾਜ *ਬੱਚਾ_obj = ਨਿਰਾ;
		ਬਣਤਰ ਅਧਾਰ_ਅੰਕੜੇ *ਬੱਚਾ = ਨਿਰਾ;

		counter_ਤਾਲਾ();
		display_ਤਰੱਕੀ(ਤਰੱਕੀ, nr_resolved_deltas);
		counter_unlock();

		work_ਤਾਲਾ();
		ਜੇ (list_ਖਾਲੀ_ਥਾਵਾਂ(&work_ਸਿਰ)) {
			/*
			 * Take an object from the object array.
			 */
			ਜਦੋਂ (nr_dispatched < nr_objects &&
			       is_delta_type(objects[nr_dispatched].type))
				nr_dispatched++;
			ਜੇ (nr_dispatched >= nr_objects) {
				work_unlock();
				ਰੋਕੋ;
			}
			ਬੱਚਾ_obj = &objects[nr_dispatched++];
		} ਨਹੀਂਤਾਂ {
			/*
			 * Peek at the top of the stack, and take a child from
			 * it.
			 */
			ਮਾਪਾ = list_ਪਹਿਲਾ_ਇੰਦਰਾਜ(&work_ਸਿਰ, ਬਣਤਰ ਅਧਾਰ_ਅੰਕੜੇ,
						  list);

			ਜਦੋਂ (ਮਾਪਾ->ref_ਪਹਿਲਾ <= ਮਾਪਾ->ref_ਆਖਰੀ) {
				ਪੂਰਨ ਵਿੱਥ = ref_deltas[ਮਾਪਾ->ref_ਪਹਿਲਾ++].obj_no;
				ਬੱਚਾ_obj = objects + ਵਿੱਥ;
				ਜੇ (ਬੱਚਾ_obj->real_type != OBJ_REF_DELTA) {
					ਬੱਚਾ_obj = ਨਿਰਾ;
					ਜਾਰੀ;
				}
				ਬੱਚਾ_obj->real_type = ਮਾਪਾ->obj->real_type;
				ਰੋਕੋ;
			}

			ਜੇ (!ਬੱਚਾ_obj && ਮਾਪਾ->ofs_ਪਹਿਲਾ <= ਮਾਪਾ->ofs_ਆਖਰੀ) {
				ਬੱਚਾ_obj = objects +
					ofs_deltas[ਮਾਪਾ->ofs_ਪਹਿਲਾ++].obj_no;
				ਪ੍ਰਤਿਜ੍ਞਾ(ਬੱਚਾ_obj->real_type == OBJ_OFS_DELTA);
				ਬੱਚਾ_obj->real_type = ਮਾਪਾ->obj->real_type;
			}

			ਜੇ (ਮਾਪਾ->ref_ਪਹਿਲਾ > ਮਾਪਾ->ref_ਆਖਰੀ &&
			    ਮਾਪਾ->ofs_ਪਹਿਲਾ > ਮਾਪਾ->ofs_ਆਖਰੀ) {
				/*
				 * This parent has run out of children, so move
				 * it to done_head.
				 */
				list_del(&ਮਾਪਾ->list);
				list_ਜੋੜੋ(&ਮਾਪਾ->list, &ਪੂਰਾ_ਸਿਰ);
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
			get_ਅਧਾਰ_ਅੰਕੜੇ(ਮਾਪਾ);
			ਮਾਪਾ->retain_ਅੰਕੜੇ++;
		}
		work_unlock();

		ਜੇ (ਬੱਚਾ_obj) {
			ਜੇ (ਮਾਪਾ) {
				ਬੱਚਾ = resolve_delta(ਬੱਚਾ_obj, ਮਾਪਾ);
				ਜੇ (!ਬੱਚਾ->children_ਬਚਿਆ)
					FREE_AND_NULL(ਬੱਚਾ->ਅੰਕੜੇ);
			} ਨਹੀਂਤਾਂ{
				ਬੱਚਾ = make_ਅਧਾਰ(ਬੱਚਾ_obj, ਨਿਰਾ);
				ਜੇ (ਬੱਚਾ->children_ਬਚਿਆ) {
					/*
					 * Since this child has its own delta children,
					 * we will need this data in the future.
					 * Inflate now so that future iterations will
					 * have access to this object's data while
					 * outside the work mutex.
					 */
					ਬੱਚਾ->ਅੰਕੜੇ = get_ਅੰਕੜੇ_from_pack(ਬੱਚਾ_obj);
					ਬੱਚਾ->ਮਾਪ = ਬੱਚਾ_obj->ਮਾਪ;
				}
			}
		}

		work_ਤਾਲਾ();
		ਜੇ (ਮਾਪਾ)
			ਮਾਪਾ->retain_ਅੰਕੜੇ--;

		ਜੇ (ਬੱਚਾ && ਬੱਚਾ->ਅੰਕੜੇ) {
			/*
			 * This child has its own children, so add it to
			 * work_head.
			 */
			list_ਜੋੜੋ(&ਬੱਚਾ->list, &work_ਸਿਰ);
			ਅਧਾਰ_cache_ਵਰਤਿਆ += ਬੱਚਾ->ਮਾਪ;
			prune_ਅਧਾਰ_ਅੰਕੜੇ(ਨਿਰਾ);
		} ਨਹੀਂਤਾਂ ਜੇ (ਬੱਚਾ) {
			/*
			 * This child does not have its own children. It may be
			 * the last descendant of its ancestors; free those
			 * that we can.
			 */
			ਬਣਤਰ ਅਧਾਰ_ਅੰਕੜੇ *p = ਮਾਪਾ;

			ਜਦੋਂ (p) {
				ਬਣਤਰ ਅਧਾਰ_ਅੰਕੜੇ *ਅਗਲਾ_p;

				p->children_ਬਚਿਆ--;
				ਜੇ (p->children_ਬਚਿਆ)
					ਰੋਕੋ;

				ਅਗਲਾ_p = p->ਅਧਾਰ;
				free_ਅਧਾਰ_ਅੰਕੜੇ(p);
				list_del(&p->list);
				ਮੁਕਤ(p);

				p = ਅਗਲਾ_p;
			}
			FREE_AND_NULL(ਬੱਚਾ);
		}
		work_unlock();
	}
	ਵਾਪਸ ਨਿਰਾ;
}

/*
 * First pass:
 * - find locations of all objects;
 * - calculate SHA1 of all non-delta objects;
 * - remember base (SHA1 or offset) for all deltas.
 */
ਸਥਿਰ ਖਾਲੀ parse_pack_objects(ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *hash)
{
	ਪੂਰਨ i, nr_delays = 0;
	ਬਣਤਰ ofs_delta_ਇੰਦਰਾਜ *ofs_delta = ofs_deltas;
	ਬਣਤਰ object_id ref_delta_oid;
	ਬਣਤਰ stat st;
	ਬਣਤਰ git_hash_ctx tmp_ctx;

	ਜੇ (verbose)
		ਤਰੱਕੀ = ਸ਼ੁਰੂ_ਤਰੱਕੀ(
				the_repository,
				ਤਰੱਕੀ_title ? ਤਰੱਕੀ_title :
				from_stdin ? _("Receiving objects") : _("Indexing objects"),
				nr_objects);
	ਲਈ (i = 0; i < nr_objects; i++) {
		ਬਣਤਰ object_ਇੰਦਰਾਜ *obj = &objects[i];
		ਖਾਲੀ *ਅੰਕੜੇ = unpack_raw_ਇੰਦਰਾਜ(obj, &ofs_delta->ਵਿੱਥ,
					      &ref_delta_oid,
					      &obj->idx.oid);
		obj->real_type = obj->type;
		ਜੇ (obj->type == OBJ_OFS_DELTA) {
			nr_ofs_deltas++;
			ofs_delta->obj_no = i;
			ofs_delta++;
		} ਨਹੀਂਤਾਂ ਜੇ (obj->type == OBJ_REF_DELTA) {
			ALLOC_GROW(ref_deltas, nr_ref_deltas + 1, ref_deltas_alloc);
			oidcpy(&ref_deltas[nr_ref_deltas].oid, &ref_delta_oid);
			ref_deltas[nr_ref_deltas].obj_no = i;
			nr_ref_deltas++;
		} ਨਹੀਂਤਾਂ ਜੇ (!ਅੰਕੜੇ) {
			/* large blobs, check later */
			obj->real_type = OBJ_BAD;
			nr_delays++;
		} ਨਹੀਂਤਾਂ
			sha1_object(ਅੰਕੜੇ, ਨਿਰਾ, obj->ਮਾਪ, obj->type,
				    &obj->idx.oid);
		ਮੁਕਤ(ਅੰਕੜੇ);
		display_ਤਰੱਕੀ(ਤਰੱਕੀ, i+1);
	}
	objects[i].idx.ਵਿੱਥ = consumed_ਬਾਈਟਾਂ;
	stop_ਤਰੱਕੀ(&ਤਰੱਕੀ);

	/* Check pack integrity */
	flush();
	git_hash_init(&tmp_ctx, the_hash_algo);
	git_hash_clone(&tmp_ctx, &ਨਿਵੇਸ਼_ctx);
	git_hash_final(hash, &tmp_ctx);
	ਜੇ (!hasheq(fill(the_hash_algo->rawsz), hash, the_repository->hash_algo))
		die(_("pack is corrupted (SHA1 mismatch)"));
	use(the_hash_algo->rawsz);

	/* If input_fd is a file, we should have reached its end now. */
	ਜੇ (fstat(ਨਿਵੇਸ਼_fd, &st))
		die_errno(_("cannot fstat packfile"));
	ਜੇ (S_ISREG(st.st_ਬਹੁਲਕ) &&
			lseek(ਨਿਵੇਸ਼_fd, 0, SEEK_CUR) - ਨਿਵੇਸ਼_len != st.st_ਮਾਪ)
		die(_("pack has junk at the end"));

	ਲਈ (i = 0; i < nr_objects; i++) {
		ਬਣਤਰ object_ਇੰਦਰਾਜ *obj = &objects[i];
		ਜੇ (obj->real_type != OBJ_BAD)
			ਜਾਰੀ;
		obj->real_type = obj->type;
		sha1_object(ਨਿਰਾ, obj, obj->ਮਾਪ, obj->type,
			    &obj->idx.oid);
		nr_delays--;
	}
	ਜੇ (nr_delays)
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
ਸਥਿਰ ਖਾਲੀ resolve_deltas(ਬਣਤਰ pack_idx_option *opts)
{
	ਪੂਰਨ i;

	ਜੇ (!nr_ofs_deltas && !nr_ref_deltas)
		ਵਾਪਸ;

	/* Sort deltas by base SHA1/offset for fast searching */
	QSORT(ofs_deltas, nr_ofs_deltas, compare_ofs_delta_ਇੰਦਰਾਜ);
	QSORT(ref_deltas, nr_ref_deltas, compare_ref_delta_ਇੰਦਰਾਜ);

	ਜੇ (verbose || show_resolving_ਤਰੱਕੀ)
		ਤਰੱਕੀ = ਸ਼ੁਰੂ_ਤਰੱਕੀ(the_repository,
					  _("Resolving deltas"),
					  nr_ref_deltas + nr_ofs_deltas);

	nr_dispatched = 0;
	ਅਧਾਰ_cache_ਹੱਦਬੰਦੀ = opts->delta_ਅਧਾਰ_cache_ਹੱਦਬੰਦੀ * nr_ਧਾਗੇ;
	ਜੇ (nr_ਧਾਗੇ > 1 || getenv("GIT_FORCE_THREADS")) {
		init_thread();
		ਲਈ (i = 0; i < nr_ਧਾਗੇ; i++) {
			ਪੂਰਨ ret = pthread_create(&thread_ਅੰਕੜੇ[i].thread, ਨਿਰਾ,
						 threaded_ਦੂਜਾ_pass, thread_ਅੰਕੜੇ + i);
			ਜੇ (ret)
				die(_("unable to create thread: %s"),
				    strerror(ret));
		}
		ਲਈ (i = 0; i < nr_ਧਾਗੇ; i++)
			pthread_join(thread_ਅੰਕੜੇ[i].thread, ਨਿਰਾ);
		cleanup_thread();
		ਵਾਪਸ;
	}
	threaded_ਦੂਜਾ_pass(&nothread_ਅੰਕੜੇ);
}

/*
 * Third pass:
 * - append objects to convert thin pack to full pack if required
 * - write the final pack hash
 */
ਸਥਿਰ ਖਾਲੀ fix_unresolved_deltas(ਬਣਤਰ hashfile *f);
ਸਥਿਰ ਖਾਲੀ conclude_pack(ਪੂਰਨ fix_thin_pack, ਅਚਲ ਅੱਖਰ *curr_pack, ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *pack_hash)
{
	ਜੇ (nr_ref_deltas + nr_ofs_deltas == nr_resolved_deltas) {
		stop_ਤਰੱਕੀ(&ਤਰੱਕੀ);
		/* Flush remaining pack final hash. */
		flush();
		ਵਾਪਸ;
	}

	ਜੇ (fix_thin_pack) {
		ਬਣਤਰ hashfile *f;
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ read_hash[GIT_MAX_RAWSZ], ਪੂਛ_hash[GIT_MAX_RAWSZ];
		ਬਣਤਰ strbuf ਸੁਨੇਹਾ = STRBUF_INIT;
		ਪੂਰਨ nr_unresolved = nr_ofs_deltas + nr_ref_deltas - nr_resolved_deltas;
		ਪੂਰਨ nr_objects_initial = nr_objects;
		ਜੇ (nr_unresolved <= 0)
			die(_("confusion beyond insanity"));
		REALLOC_ARRAY(objects, nr_objects + nr_unresolved + 1);
		memset(objects + nr_objects + 1, 0,
		       nr_unresolved * ਆਕਾਰ(*objects));
		f = hashfd(the_repository->hash_algo, output_fd, curr_pack);
		fix_unresolved_deltas(f);
		strbuf_addf(&ਸੁਨੇਹਾ, Q_("completed with %d local object",
				     "completed with %d local objects",
				     nr_objects - nr_objects_initial),
			    nr_objects - nr_objects_initial);
		stop_ਤਰੱਕੀ_ਸੁਨੇਹਾ(&ਤਰੱਕੀ, ਸੁਨੇਹਾ.buf);
		strbuf_release(&ਸੁਨੇਹਾ);
		finalize_hashfile(f, ਪੂਛ_hash, FSYNC_COMPONENT_PACK, 0);
		hashcpy(read_hash, pack_hash, the_repository->hash_algo);
		fixup_pack_ਸਿਰਲੇਖ_footer(the_hash_algo, output_fd, pack_hash,
					 curr_pack, nr_objects,
					 read_hash, consumed_ਬਾਈਟਾਂ-the_hash_algo->rawsz);
		ਜੇ (!hasheq(read_hash, ਪੂਛ_hash, the_repository->hash_algo))
			die(_("Unexpected tail checksum for %s "
			      "(disk corruption?)"), curr_pack);
	}
	ਜੇ (nr_ofs_deltas + nr_ref_deltas != nr_resolved_deltas)
		die(Q_("pack has %d unresolved delta",
		       "pack has %d unresolved deltas",
		       nr_ofs_deltas + nr_ref_deltas - nr_resolved_deltas),
		    nr_ofs_deltas + nr_ref_deltas - nr_resolved_deltas);
}

ਸਥਿਰ ਪੂਰਨ write_compressed(ਬਣਤਰ hashfile *f, ਖਾਲੀ *ਵਿੱਚ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਮਾਪ)
{
	git_zstream ਧਾਰਾ;
	ਪੂਰਨ ਸਥਿਤੀ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ outbuf[4096];
	ਬਣਤਰ repo_config_ਮੁੱਲਾਂ *cfg = repo_config_ਮੁੱਲਾਂ(the_repository);

	git_deflate_init(&ਧਾਰਾ, cfg->zlib_compression_level);
	ਧਾਰਾ.ਅਗਲਾ_ਵਿੱਚ = ਵਿੱਚ;
	ਧਾਰਾ.avail_ਵਿੱਚ = ਮਾਪ;

	ਕਰੋ {
		ਧਾਰਾ.ਅਗਲਾ_ਬਾਹਰਲਾ = outbuf;
		ਧਾਰਾ.avail_ਬਾਹਰਲਾ = ਆਕਾਰ(outbuf);
		ਸਥਿਤੀ = git_deflate(&ਧਾਰਾ, Z_ਅੰਤ);
		hashwrite(f, outbuf, ਆਕਾਰ(outbuf) - ਧਾਰਾ.avail_ਬਾਹਰਲਾ);
	} ਜਦੋਂ (ਸਥਿਤੀ == Z_OK);

	ਜੇ (ਸਥਿਤੀ != Z_ਧਾਰਾ_ਸਿਰਾ)
		die(_("unable to deflate appended object (%d)"), ਸਥਿਤੀ);
	ਮਾਪ = ਧਾਰਾ.ਕੁੱਲ_ਬਾਹਰਲਾ;
	git_deflate_ਸਿਰਾ(&ਧਾਰਾ);
	ਵਾਪਸ ਮਾਪ;
}

ਸਥਿਰ ਬਣਤਰ object_ਇੰਦਰਾਜ *append_obj_ਮੰਜ਼ਿਲ_pack(ਬਣਤਰ hashfile *f,
			       ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *sha1, ਖਾਲੀ *buf,
			       ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਮਾਪ, ਗਿਣਤੀ object_type type)
{
	ਬਣਤਰ object_ਇੰਦਰਾਜ *obj = &objects[nr_objects++];
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ ਸਿਰਲੇਖ[10];
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ s = ਮਾਪ;
	ਪੂਰਨ n = 0;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ c = (type << 4) | (s & 15);
	s >>= 4;
	ਜਦੋਂ (s) {
		ਸਿਰਲੇਖ[n++] = c | 0x80;
		c = s & 0x7f;
		s >>= 7;
	}
	ਸਿਰਲੇਖ[n++] = c;
	ਚੱਕਰੀ_ਜਾਂਚ_32_begin(f);
	hashwrite(f, ਸਿਰਲੇਖ, n);
	obj[0].ਮਾਪ = ਮਾਪ;
	obj[0].hdr_ਮਾਪ = n;
	obj[0].type = type;
	obj[0].real_type = type;
	obj[1].idx.ਵਿੱਥ = obj[0].idx.ਵਿੱਥ + n;
	obj[1].idx.ਵਿੱਥ += write_compressed(f, buf, ਮਾਪ);
	obj[0].idx.ਚੱਕਰੀ_ਜਾਂਚ_32 = ਚੱਕਰੀ_ਜਾਂਚ_32_ਸਿਰਾ(f);
	hashflush(f);
	oidread(&obj->idx.oid, sha1, the_repository->hash_algo);
	ਵਾਪਸ obj;
}

ਸਥਿਰ ਪੂਰਨ delta_pos_compare(ਅਚਲ ਖਾਲੀ *_a, ਅਚਲ ਖਾਲੀ *_b)
{
	ਬਣਤਰ ref_delta_ਇੰਦਰਾਜ *a = *(ਬਣਤਰ ref_delta_ਇੰਦਰਾਜ **)_a;
	ਬਣਤਰ ref_delta_ਇੰਦਰਾਜ *b = *(ਬਣਤਰ ref_delta_ਇੰਦਰਾਜ **)_b;
	ਵਾਪਸ a->obj_no - b->obj_no;
}

ਸਥਿਰ ਖਾਲੀ fix_unresolved_deltas(ਬਣਤਰ hashfile *f)
{
	ਬਣਤਰ ref_delta_ਇੰਦਰਾਜ **sorted_by_pos;
	ਪੂਰਨ i;

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
	ਲਈ (i = 0; i < nr_ref_deltas; i++)
		sorted_by_pos[i] = &ref_deltas[i];
	QSORT(sorted_by_pos, nr_ref_deltas, delta_pos_compare);

	ਜੇ (repo_has_promisor_remote(the_repository)) {
		/*
		 * Prefetch the delta bases.
		 */
		ਬਣਤਰ oid_array ਮੰਜ਼ਿਲ_fetch = OID_ARRAY_INIT;
		ਲਈ (i = 0; i < nr_ref_deltas; i++) {
			ਬਣਤਰ ref_delta_ਇੰਦਰਾਜ *d = sorted_by_pos[i];
			ਜੇ (!odb_read_object_info_extended(the_repository->objects,
							   &d->oid, ਨਿਰਾ,
							   OBJECT_INFO_FOR_PREFETCH))
				ਜਾਰੀ;
			oid_array_append(&ਮੰਜ਼ਿਲ_fetch, &d->oid);
		}
		promisor_remote_get_ਸਿੱਧਾ(the_repository,
					   ਮੰਜ਼ਿਲ_fetch.oid, ਮੰਜ਼ਿਲ_fetch.nr);
		oid_array_clear(&ਮੰਜ਼ਿਲ_fetch);
	}

	ਲਈ (i = 0; i < nr_ref_deltas; i++) {
		ਬਣਤਰ ref_delta_ਇੰਦਰਾਜ *d = sorted_by_pos[i];
		ਗਿਣਤੀ object_type type;
		ਖਾਲੀ *ਅੰਕੜੇ;
		ਮਾਪ_t ਮਾਪ;

		ਜੇ (objects[d->obj_no].real_type != OBJ_REF_DELTA)
			ਜਾਰੀ;
		ਅੰਕੜੇ = odb_read_object(the_repository->objects, &d->oid,
				       &type, &ਮਾਪ);
		ਜੇ (!ਅੰਕੜੇ)
			ਜਾਰੀ;

		ਜੇ (check_object_signature(the_repository, &d->oid, ਅੰਕੜੇ, ਮਾਪ,
					   type) < 0)
			die(_("local object %s is corrupt"), oid_ਮੰਜ਼ਿਲ_hex(&d->oid));

		/*
		 * Add this as an object to the objects array and call
		 * threaded_second_pass() (which will pick up the added
		 * object).
		 */
		append_obj_ਮੰਜ਼ਿਲ_pack(f, d->oid.hash, ਅੰਕੜੇ, ਮਾਪ, type);
		ਮੁਕਤ(ਅੰਕੜੇ);
		threaded_ਦੂਜਾ_pass(ਨਿਰਾ);

		display_ਤਰੱਕੀ(ਤਰੱਕੀ, nr_resolved_deltas);
	}
	ਮੁਕਤ(sorted_by_pos);
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *derive_filename(ਅਚਲ ਅੱਖਰ *pack_name, ਅਚਲ ਅੱਖਰ *strip,
				   ਅਚਲ ਅੱਖਰ *suffix, ਬਣਤਰ strbuf *buf)
{
	ਮਾਪ_t len;
	ਜੇ (!strip_suffix(pack_name, strip, &len) || !len ||
	    pack_name[len - 1] != '.')
		die(_("packfile name '%s' does not end with '.%s'"),
		    pack_name, strip);
	strbuf_ਜੋੜੋ(buf, pack_name, len);
	strbuf_addstr(buf, suffix);
	ਵਾਪਸ buf->buf;
}

ਸਥਿਰ ਖਾਲੀ write_special_file(ਅਚਲ ਅੱਖਰ *suffix, ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ,
			       ਅਚਲ ਅੱਖਰ *pack_name, ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *hash,
			       ਅਚਲ ਅੱਖਰ **report)
{
	ਬਣਤਰ strbuf name_buf = STRBUF_INIT;
	ਅਚਲ ਅੱਖਰ *filename;
	ਪੂਰਨ fd;
	ਪੂਰਨ ਸੁਨੇਹਾ_len = ਲੜੀ_ਲੰਬਾਈ(ਸੁਨੇਹਾ);

	ਜੇ (pack_name)
		filename = derive_filename(pack_name, "pack", suffix, &name_buf);
	ਨਹੀਂਤਾਂ
		filename = odb_pack_name(the_repository, &name_buf, hash, suffix);

	fd = ਸੁਰੱਖਿਅਤ_create_file_with_leading_directories(the_repository, filename);
	ਜੇ (fd < 0) {
		ਜੇ (errno != EEXIST)
			die_errno(_("cannot write %s file '%s'"),
				  suffix, filename);
	} ਨਹੀਂਤਾਂ {
		ਜੇ (ਸੁਨੇਹਾ_len > 0) {
			write_or_die(fd, ਸੁਨੇਹਾ, ਸੁਨੇਹਾ_len);
			write_or_die(fd, "\n", 1);
		}
		ਜੇ (close(fd) != 0)
			die_errno(_("cannot close written %s file '%s'"),
				  suffix, filename);
		ਜੇ (report)
			*report = suffix;
	}
	strbuf_release(&name_buf);
}

ਸਥਿਰ ਖਾਲੀ rename_tmp_packfile(ਅਚਲ ਅੱਖਰ **final_name,
				ਅਚਲ ਅੱਖਰ *curr_name,
				ਬਣਤਰ strbuf *name, ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *hash,
				ਅਚਲ ਅੱਖਰ *ext, ਪੂਰਨ make_read_only_if_same)
{
	ਜੇ (!*final_name || strcmp(*final_name, curr_name)) {
		ਜੇ (!*final_name)
			*final_name = odb_pack_name(the_repository, name, hash, ext);
		ਜੇ (finalize_object_file(the_repository, curr_name, *final_name))
			die(_("unable to rename temporary '*.%s' file to '%s'"),
			    ext, *final_name);
	} ਨਹੀਂਤਾਂ ਜੇ (make_read_only_if_same) {
		chmod(*final_name, 0444);
	}
}

ਸਥਿਰ ਖਾਲੀ final(ਅਚਲ ਅੱਖਰ *final_pack_name, ਅਚਲ ਅੱਖਰ *curr_pack_name,
		  ਅਚਲ ਅੱਖਰ *final_ਸੂਚਕਾਂਕ_name, ਅਚਲ ਅੱਖਰ *curr_ਸੂਚਕਾਂਕ_name,
		  ਅਚਲ ਅੱਖਰ *final_rev_ਸੂਚਕਾਂਕ_name, ਅਚਲ ਅੱਖਰ *curr_rev_ਸੂਚਕਾਂਕ_name,
		  ਅਚਲ ਅੱਖਰ *keep_ਸੁਨੇਹਾ, ਅਚਲ ਅੱਖਰ *promisor_ਸੁਨੇਹਾ,
		  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *hash)
{
	ਅਚਲ ਅੱਖਰ *report = "pack";
	ਬਣਤਰ strbuf pack_name = STRBUF_INIT;
	ਬਣਤਰ strbuf ਸੂਚਕਾਂਕ_name = STRBUF_INIT;
	ਬਣਤਰ strbuf rev_ਸੂਚਕਾਂਕ_name = STRBUF_INIT;

	ਜੇ (!from_stdin) {
		close(ਨਿਵੇਸ਼_fd);
	} ਨਹੀਂਤਾਂ {
		fsync_component_or_die(FSYNC_COMPONENT_PACK, output_fd, curr_pack_name);
		ਜੇ (close(output_fd))
			die_errno(_("error while closing pack file"));
	}

	ਜੇ (keep_ਸੁਨੇਹਾ)
		write_special_file("keep", keep_ਸੁਨੇਹਾ, final_pack_name, hash,
				   &report);
	ਜੇ (promisor_ਸੁਨੇਹਾ)
		write_special_file("promisor", promisor_ਸੁਨੇਹਾ, final_pack_name,
				   hash, ਨਿਰਾ);

	rename_tmp_packfile(&final_pack_name, curr_pack_name, &pack_name,
			    hash, "pack", from_stdin);
	ਜੇ (curr_rev_ਸੂਚਕਾਂਕ_name)
		rename_tmp_packfile(&final_rev_ਸੂਚਕਾਂਕ_name, curr_rev_ਸੂਚਕਾਂਕ_name,
				    &rev_ਸੂਚਕਾਂਕ_name, hash, "rev", 1);
	rename_tmp_packfile(&final_ਸੂਚਕਾਂਕ_name, curr_ਸੂਚਕਾਂਕ_name, &ਸੂਚਕਾਂਕ_name,
			    hash, "idx", 1);

	ਜੇ (do_fsck_object && startup_info->have_repository) {
		ਬਣਤਰ odb_ਸਰੋਤ_files *files =
			odb_ਸਰੋਤ_files_downcast(the_repository->objects->ਸਰੋਤ);
		packfile_store_ਲੱਦੋ_pack(files->dirs->packed, final_ਸੂਚਕਾਂਕ_name, 0);
	}

	ਜੇ (!from_stdin) {
		ਲਿਖੋ("%s\n", hash_ਮੰਜ਼ਿਲ_hex(hash));
	} ਨਹੀਂਤਾਂ {
		ਬਣਤਰ strbuf buf = STRBUF_INIT;

		strbuf_addf(&buf, "%s\t%s\n", report, hash_ਮੰਜ਼ਿਲ_hex(hash));
		write_or_die(1, buf.buf, buf.len);
		strbuf_release(&buf);

		/* Write the last part of the buffer to stdout */
		write_ਵਿੱਚ_ਭਰਿਆ(1, ਨਿਵੇਸ਼_ਬਫਰ + ਨਿਵੇਸ਼_ਵਿੱਥ, ਨਿਵੇਸ਼_len);
	}

	strbuf_release(&rev_ਸੂਚਕਾਂਕ_name);
	strbuf_release(&ਸੂਚਕਾਂਕ_name);
	strbuf_release(&pack_name);
}

ਸਥਿਰ ਪੂਰਨ git_ਸੂਚਕਾਂਕ_pack_config(ਅਚਲ ਅੱਖਰ *k, ਅਚਲ ਅੱਖਰ *v,
				 ਅਚਲ ਬਣਤਰ config_context *ctx, ਖਾਲੀ *cb)
{
	ਬਣਤਰ pack_idx_option *opts = cb;

	ਜੇ (!strcmp(k, "pack.indexversion")) {
		opts->ਸੰਸਕਰਣ = git_config_int(k, v, ctx->kvi);
		ਜੇ (opts->ਸੰਸਕਰਣ > 2)
			die(_("bad pack.indexVersion=%"PRIu32), opts->ਸੰਸਕਰਣ);
		ਵਾਪਸ 0;
	}
	ਜੇ (!strcmp(k, "pack.threads")) {
		nr_ਧਾਗੇ = git_config_int(k, v, ctx->kvi);
		ਜੇ (nr_ਧਾਗੇ < 0)
			die(_("invalid number of threads specified (%d)"),
			    nr_ਧਾਗੇ);
		ਜੇ (!HAVE_ਧਾਗੇ && nr_ਧਾਗੇ != 1) {
			warning(_("no threads support, ignoring %s"), k);
			nr_ਧਾਗੇ = 1;
		}
		ਵਾਪਸ 0;
	}
	ਜੇ (!strcmp(k, "pack.writereverseindex")) {
		ਜੇ (git_config_bool(k, v))
			opts->flags |= WRITE_REV;
		ਨਹੀਂਤਾਂ
			opts->flags &= ~WRITE_REV;
	}
	ਜੇ (!strcmp(k, "core.deltabasecachelimit")) {
		opts->delta_ਅਧਾਰ_cache_ਹੱਦਬੰਦੀ = git_config_ulong(k, v, ctx->kvi);
		ਵਾਪਸ 0;
	}
	ਵਾਪਸ git_default_config(k, v, ctx, cb);
}

ਸਥਿਰ ਪੂਰਨ cmp_uint32(ਅਚਲ ਖਾਲੀ *a_, ਅਚਲ ਖਾਲੀ *b_)
{
	uint32_t a = *((uint32_t *)a_);
	uint32_t b = *((uint32_t *)b_);

	ਵਾਪਸ (a < b) ? -1 : (a != b);
}

ਸਥਿਰ ਖਾਲੀ read_v2_anomalous_offsets(ਬਣਤਰ packed_git *p,
				      ਬਣਤਰ pack_idx_option *opts)
{
	ਅਚਲ uint32_t *idx1, *idx2;
	uint32_t i;

	/* The address of the 4-byte offset table */
	idx1 = (((ਅਚਲ uint32_t *)((ਅਚਲ uint8_t *)p->ਸੂਚਕਾਂਕ_ਅੰਕੜੇ + p->ਚੱਕਰੀ_ਜਾਂਚ_ਵਿੱਥ))
		+ (ਮਾਪ_t)p->ਅੰਕ_objects /* CRC32 table */
		);

	/* The address of the 8-byte offset table */
	idx2 = idx1 + p->ਅੰਕ_objects;

	ਲਈ (i = 0; i < p->ਅੰਕ_objects; i++) {
		uint32_t off = ntohl(idx1[i]);
		ਜੇ (!(off & 0x80000000))
			ਜਾਰੀ;
		off = off & 0x7fffffff;
		check_pack_ਸੂਚਕਾਂਕ_ptr(p, &idx2[off * 2]);
		ਜੇ (idx2[off * 2])
			ਜਾਰੀ;
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

ਸਥਿਰ ਖਾਲੀ read_idx_option(ਬਣਤਰ pack_idx_option *opts, ਅਚਲ ਅੱਖਰ *pack_name)
{
	ਬਣਤਰ packed_git *p = ਜੋੜੋ_packed_git(the_repository, pack_name,
					      ਲੜੀ_ਲੰਬਾਈ(pack_name), 1);

	ਜੇ (!p)
		die(_("Cannot open existing pack file '%s'"), pack_name);
	ਜੇ (open_pack_ਸੂਚਕਾਂਕ(p))
		die(_("Cannot open existing pack idx file for '%s'"), pack_name);

	/* Read the attributes from the existing idx file */
	opts->ਸੰਸਕਰਣ = p->ਸੂਚਕਾਂਕ_ਸੰਸਕਰਣ;

	ਜੇ (opts->ਸੰਸਕਰਣ == 2)
		read_v2_anomalous_offsets(p, opts);

	/*
	 * Get rid of the idx file as we do not need it anymore.
	 * NEEDSWORK: extract this bit from free_pack_by_name() in
	 * object-file.c, perhaps?  It shouldn't matter very much as we
	 * know we haven't installed this pack (hence we never have
	 * read anything from it).
	 */
	close_pack_ਸੂਚਕਾਂਕ(p);
	ਮੁਕਤ(p);
}

ਸਥਿਰ ਖਾਲੀ show_pack_info(ਪੂਰਨ stat_only)
{
	ਪੂਰਨ i, baseobjects = nr_objects - nr_ref_deltas - nr_ofs_deltas;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ *chain_histogram = ਨਿਰਾ;

	ਜੇ (ਸਭ_ਤੋਂ_ਡੂੰਘਾ_delta)
		CALLOC_ARRAY(chain_histogram, ਸਭ_ਤੋਂ_ਡੂੰਘਾ_delta);

	ਲਈ (i = 0; i < nr_objects; i++) {
		ਬਣਤਰ object_ਇੰਦਰਾਜ *obj = &objects[i];

		ਜੇ (is_delta_type(obj->type))
			chain_histogram[obj_stat[i].delta_ਡੂੰਘਾਈ - 1]++;
		ਜੇ (stat_only)
			ਜਾਰੀ;
		ਲਿਖੋ("%s %-6s %"PRIuMAX" %"PRIuMAX" %"PRIuMAX,
		       oid_ਮੰਜ਼ਿਲ_hex(&obj->idx.oid),
		       type_name(obj->real_type), (uintmax_t)obj->ਮਾਪ,
		       (uintmax_t)(obj[1].idx.ਵਿੱਥ - obj->idx.ਵਿੱਥ),
		       (uintmax_t)obj->idx.ਵਿੱਥ);
		ਜੇ (is_delta_type(obj->type)) {
			ਬਣਤਰ object_ਇੰਦਰਾਜ *bobj = &objects[obj_stat[i].ਅਧਾਰ_object_no];
			ਲਿਖੋ(" %u %s", obj_stat[i].delta_ਡੂੰਘਾਈ,
			       oid_ਮੰਜ਼ਿਲ_hex(&bobj->idx.oid));
		}
		ਅੱਖਰ_ਲਿਖੋ('\n');
	}

	ਜੇ (baseobjects)
		printf_ln(Q_("non delta: %d object",
			     "non delta: %d objects",
			     baseobjects),
			  baseobjects);
	ਲਈ (i = 0; i < ਸਭ_ਤੋਂ_ਡੂੰਘਾ_delta; i++) {
		ਜੇ (!chain_histogram[i])
			ਜਾਰੀ;
		printf_ln(Q_("chain length = %d: %lu object",
			     "chain length = %d: %lu objects",
			     chain_histogram[i]),
			  i + 1,
			  chain_histogram[i]);
	}
	ਮੁਕਤ(chain_histogram);
}

ਸਥਿਰ ਖਾਲੀ repack_local_links(ਖਾਲੀ)
{
	ਬਣਤਰ ਬੱਚਾ_process cmd = ਬੱਚਾ_PROCESS_INIT;
	FILE *ਬਾਹਰਲਾ;
	ਬਣਤਰ strbuf ਸਤਰ = STRBUF_INIT;
	ਬਣਤਰ oidset_iter iter;
	ਬਣਤਰ object_id *oid;
	ਅੱਖਰ *ਅਧਾਰ_name = ਨਿਰਾ;

	ਜੇ (!oidset_ਮਾਪ(&outgoing_links))
		ਵਾਪਸ;

	oidset_iter_init(&outgoing_links, &iter);
	ਜਦੋਂ ((oid = oidset_iter_ਅਗਲਾ(&iter))) {
		ਬਣਤਰ odb_ਸਰੋਤ_info ਸਰੋਤ_info;
		ਬਣਤਰ object_info info = {
			.ਸਰੋਤ_infop = &ਸਰੋਤ_info,
		};

		ਜੇ (odb_read_object_info_extended(the_repository->objects, oid, &info, 0))
			/* Missing; assume it is a promisor object */
			ਜਾਰੀ;
		ਜੇ (ਸਰੋਤ_info.ਸਰੋਤ->type == ODB_ਸਰੋਤ_PACKED &&
		    ਸਰੋਤ_info.u.packed.pack->pack_promisor)
			ਜਾਰੀ;

		ਜੇ (!cmd.ਦਲੀਲਾਂ.nr) {
			ਅਧਾਰ_name = mkpathdup(
				"%s/pack/pack",
				repo_get_object_directory(the_repository));
			strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, "pack-objects");
			strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ,
				    "--exclude-promisor-objects-best-effort");
			strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, ਅਧਾਰ_name);
			cmd.git_cmd = 1;
			cmd.ਵਿੱਚ = -1;
			cmd.ਬਾਹਰਲਾ = -1;
			ਜੇ (ਸ਼ੁਰੂ_command(&cmd))
				die(_("could not start pack-objects to repack local links"));
		}

		ਜੇ (write_ਵਿੱਚ_ਭਰਿਆ(cmd.ਵਿੱਚ, oid_ਮੰਜ਼ਿਲ_hex(oid), the_hash_algo->hexsz) < 0 ||
		    write_ਵਿੱਚ_ਭਰਿਆ(cmd.ਵਿੱਚ, "\n", 1) < 0)
			die(_("failed to feed local object to pack-objects"));
	}

	ਜੇ (!cmd.ਦਲੀਲਾਂ.nr)
		ਵਾਪਸ;

	close(cmd.ਵਿੱਚ);

	ਬਾਹਰਲਾ = xfdopen(cmd.ਬਾਹਰਲਾ, "r");
	ਜਦੋਂ (strbuf_getline_lf(&ਸਤਰ, ਬਾਹਰਲਾ) != EOF) {
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ ਦੁਆਧਾਰੀ[GIT_MAX_RAWSZ];
		ਜੇ (ਸਤਰ.len != the_hash_algo->hexsz ||
		    !hex_ਮੰਜ਼ਿਲ_ਬਾਈਟਾਂ(ਦੁਆਧਾਰੀ, ਸਤਰ.buf, ਸਤਰ.len))
			die(_("index-pack: Expecting full hex object ID lines only from pack-objects."));

		/*
		 * pack-objects creates the .pack and .idx files, but not the
		 * .promisor file. Create the .promisor file, which is empty.
		 */
		write_special_file("promisor", "", ਨਿਰਾ, ਦੁਆਧਾਰੀ, ਨਿਰਾ);
	}

	fclose(ਬਾਹਰਲਾ);
	ਜੇ (ਅੰਤ_command(&cmd))
		die(_("could not finish pack-objects to repack local links"));
	strbuf_release(&ਸਤਰ);
	ਮੁਕਤ(ਅਧਾਰ_name);
}

ਪੂਰਨ cmd_ਸੂਚਕਾਂਕ_pack(ਪੂਰਨ argc,
		   ਅਚਲ ਅੱਖਰ **argv,
		   ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		   ਬਣਤਰ repository *repo)
{
	ਪੂਰਨ i, fix_thin_pack = 0, verify = 0, stat_only = 0, rev_ਸੂਚਕਾਂਕ;
	ਅਚਲ ਅੱਖਰ *curr_ਸੂਚਕਾਂਕ;
	ਅੱਖਰ *curr_rev_ਸੂਚਕਾਂਕ = ਨਿਰਾ;
	ਅਚਲ ਅੱਖਰ *ਸੂਚਕਾਂਕ_name = ਨਿਰਾ, *pack_name = ਨਿਰਾ, *rev_ਸੂਚਕਾਂਕ_name = ਨਿਰਾ;
	ਅਚਲ ਅੱਖਰ *keep_ਸੁਨੇਹਾ = ਨਿਰਾ;
	ਅਚਲ ਅੱਖਰ *promisor_ਸੁਨੇਹਾ = ਨਿਰਾ;
	ਬਣਤਰ strbuf ਸੂਚਕਾਂਕ_name_buf = STRBUF_INIT;
	ਬਣਤਰ strbuf rev_ਸੂਚਕਾਂਕ_name_buf = STRBUF_INIT;
	ਬਣਤਰ pack_idx_ਇੰਦਰਾਜ **idx_objects;
	ਬਣਤਰ pack_idx_option opts;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ pack_hash[GIT_MAX_RAWSZ];
	ਬਿਨਾਨਿਸ਼ਾਨੀ foreign_nr = 1;	/* zero is a "good" value, assume bad */
	ਪੂਰਨ report_ਸਿਰਾ_ਦਾ_ਨਿਵੇਸ਼ = 0;
	ਪੂਰਨ hash_algo = 0;

	show_usage_if_asked(argc, argv, ਸੂਚਕਾਂਕ_pack_usage);

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
	repo_config(the_repository, git_ਸੂਚਕਾਂਕ_pack_config, &opts);
	ਜੇ (ਅਗੇਤਰ && chdir(ਅਗੇਤਰ))
		die(_("Cannot come back to cwd"));

	ਜੇ (git_env_bool(GIT_TEST_NO_WRITE_REV_ਸੂਚਕਾਂਕ, 0))
		rev_ਸੂਚਕਾਂਕ = 0;
	ਨਹੀਂਤਾਂ
		rev_ਸੂਚਕਾਂਕ = !!(opts.flags & (WRITE_REV_VERIFY | WRITE_REV));

	ਲਈ (i = 1; i < argc; i++) {
		ਅਚਲ ਅੱਖਰ *arg = argv[i];

		ਜੇ (*arg == '-') {
			ਜੇ (!strcmp(arg, "--stdin")) {
				from_stdin = 1;
			} ਨਹੀਂਤਾਂ ਜੇ (!strcmp(arg, "--fix-thin")) {
				fix_thin_pack = 1;
			} ਨਹੀਂਤਾਂ ਜੇ (skip_ਮੰਜ਼ਿਲ_optional_arg(arg, "--strict", &arg)) {
				strict = 1;
				do_fsck_object = 1;
				fsck_ਸੈੱਟ_ਸੁਨੇਹਾ_types(&fsck_options, arg);
			} ਨਹੀਂਤਾਂ ਜੇ (!strcmp(arg, "--check-self-contained-and-connected")) {
				strict = 1;
				check_self_contained_and_connected = 1;
			} ਨਹੀਂਤਾਂ ਜੇ (skip_ਮੰਜ਼ਿਲ_optional_arg(arg, "--fsck-objects", &arg)) {
				do_fsck_object = 1;
				fsck_ਸੈੱਟ_ਸੁਨੇਹਾ_types(&fsck_options, arg);
			} ਨਹੀਂਤਾਂ ਜੇ (!strcmp(arg, "--verify")) {
				verify = 1;
			} ਨਹੀਂਤਾਂ ਜੇ (!strcmp(arg, "--verify-stat")) {
				verify = 1;
				show_stat = 1;
			} ਨਹੀਂਤਾਂ ਜੇ (!strcmp(arg, "--verify-stat-only")) {
				verify = 1;
				show_stat = 1;
				stat_only = 1;
			} ਨਹੀਂਤਾਂ ਜੇ (skip_ਮੰਜ਼ਿਲ_optional_arg(arg, "--keep", &keep_ਸੁਨੇਹਾ)) {
				; /* nothing to do */
			} ਨਹੀਂਤਾਂ ਜੇ (skip_ਮੰਜ਼ਿਲ_optional_arg(arg, "--promisor", &promisor_ਸੁਨੇਹਾ)) {
				ਰਿਕਾਰਡ_outgoing_links = 1;
			} ਨਹੀਂਤਾਂ ਜੇ (starts_with(arg, "--threads=")) {
				ਅੱਖਰ *ਸਿਰਾ;
				nr_ਧਾਗੇ = strtoul(arg+10, &ਸਿਰਾ, 0);
				ਜੇ (!arg[10] || *ਸਿਰਾ || nr_ਧਾਗੇ < 0)
					usage(ਸੂਚਕਾਂਕ_pack_usage);
				ਜੇ (!HAVE_ਧਾਗੇ && nr_ਧਾਗੇ != 1) {
					warning(_("no threads support, ignoring %s"), arg);
					nr_ਧਾਗੇ = 1;
				}
			} ਨਹੀਂਤਾਂ ਜੇ (skip_ਅਗੇਤਰ(arg, "--pack_header=", &arg)) {
				ਜੇ (parse_pack_ਸਿਰਲੇਖ_option(arg,
							     ਨਿਵੇਸ਼_ਬਫਰ,
							     &ਨਿਵੇਸ਼_len) < 0)
					die(_("bad --pack_header: %s"), arg);
			} ਨਹੀਂਤਾਂ ਜੇ (!strcmp(arg, "-v")) {
				verbose = 1;
			} ਨਹੀਂਤਾਂ ਜੇ (!strcmp(arg, "--progress-title")) {
				ਜੇ (ਤਰੱਕੀ_title || (i+1) >= argc)
					usage(ਸੂਚਕਾਂਕ_pack_usage);
				ਤਰੱਕੀ_title = argv[++i];
			} ਨਹੀਂਤਾਂ ਜੇ (!strcmp(arg, "--show-resolving-progress")) {
				show_resolving_ਤਰੱਕੀ = 1;
			} ਨਹੀਂਤਾਂ ਜੇ (!strcmp(arg, "--report-end-of-input")) {
				report_ਸਿਰਾ_ਦਾ_ਨਿਵੇਸ਼ = 1;
			} ਨਹੀਂਤਾਂ ਜੇ (!strcmp(arg, "-o")) {
				ਜੇ (ਸੂਚਕਾਂਕ_name || (i+1) >= argc)
					usage(ਸੂਚਕਾਂਕ_pack_usage);
				ਸੂਚਕਾਂਕ_name = argv[++i];
			} ਨਹੀਂਤਾਂ ਜੇ (starts_with(arg, "--index-version=")) {
				ਅੱਖਰ *c;
				opts.ਸੰਸਕਰਣ = strtoul(arg + 16, &c, 10);
				ਜੇ (opts.ਸੰਸਕਰਣ > 2)
					die(_("bad %s"), arg);
				ਜੇ (*c == ',')
					opts.off32_ਹੱਦਬੰਦੀ = strtoul(c+1, &c, 0);
				ਜੇ (*c || opts.off32_ਹੱਦਬੰਦੀ & 0x80000000)
					die(_("bad %s"), arg);
			} ਨਹੀਂਤਾਂ ਜੇ (skip_ਅਗੇਤਰ(arg, "--max-input-size=", &arg)) {
				max_ਨਿਵੇਸ਼_ਮਾਪ = strtoumax(arg, ਨਿਰਾ, 10);
			} ਨਹੀਂਤਾਂ ਜੇ (skip_ਅਗੇਤਰ(arg, "--object-format=", &arg)) {
				hash_algo = hash_algo_by_name(arg);
				ਜੇ (hash_algo == GIT_HASH_UNKNOWN)
					die(_("unknown hash algorithm '%s'"), arg);
				repo_ਸੈੱਟ_hash_algo(the_repository, hash_algo);
			} ਨਹੀਂਤਾਂ ਜੇ (!strcmp(arg, "--rev-index")) {
				rev_ਸੂਚਕਾਂਕ = 1;
			} ਨਹੀਂਤਾਂ ਜੇ (!strcmp(arg, "--no-rev-index")) {
				rev_ਸੂਚਕਾਂਕ = 0;
			} ਨਹੀਂਤਾਂ
				usage(ਸੂਚਕਾਂਕ_pack_usage);
			ਜਾਰੀ;
		}

		ਜੇ (pack_name)
			usage(ਸੂਚਕਾਂਕ_pack_usage);
		pack_name = arg;
	}

	ਜੇ (!pack_name && !from_stdin)
		usage(ਸੂਚਕਾਂਕ_pack_usage);
	ਜੇ (fix_thin_pack && !from_stdin)
		die(_("the option '%s' requires '%s'"), "--fix-thin", "--stdin");
	ਜੇ (promisor_ਸੁਨੇਹਾ && pack_name)
		die(_("--promisor cannot be used with a pack name"));
	ਜੇ (from_stdin && !startup_info->have_repository)
		die(_("--stdin requires a git repository"));
	ਜੇ (from_stdin && hash_algo)
		die(_("options '%s' and '%s' cannot be used together"), "--object-format", "--stdin");
	ਜੇ (!ਸੂਚਕਾਂਕ_name && pack_name)
		ਸੂਚਕਾਂਕ_name = derive_filename(pack_name, "pack", "idx", &ਸੂਚਕਾਂਕ_name_buf);

	/*
	 * Packfiles and indices do not carry enough information to be able to
	 * identify their object hash. So when we are neither in a repository
	 * nor has the user told us which object hash to use we have no other
	 * choice but to guess the object hash.
	 */
	ਜੇ (!the_repository->hash_algo)
		repo_ਸੈੱਟ_hash_algo(the_repository, GIT_HASH_DEFAULT);

	opts.flags &= ~(WRITE_REV | WRITE_REV_VERIFY);
	ਜੇ (rev_ਸੂਚਕਾਂਕ) {
		opts.flags |= verify ? WRITE_REV_VERIFY : WRITE_REV;
		ਜੇ (ਸੂਚਕਾਂਕ_name)
			rev_ਸੂਚਕਾਂਕ_name = derive_filename(ਸੂਚਕਾਂਕ_name,
							 "idx", "rev",
							 &rev_ਸੂਚਕਾਂਕ_name_buf);
	}

	ਜੇ (verify) {
		ਜੇ (!ਸੂਚਕਾਂਕ_name)
			die(_("--verify with no packfile name given"));
		read_idx_option(&opts, ਸੂਚਕਾਂਕ_name);
		opts.flags |= WRITE_IDX_VERIFY | WRITE_IDX_STRICT;
	}
	ਜੇ (strict)
		opts.flags |= WRITE_IDX_STRICT;

	ਜੇ (HAVE_ਧਾਗੇ && !nr_ਧਾਗੇ) {
		nr_ਧਾਗੇ = online_cpus();
		/*
		 * Experiments show that going above 20 threads doesn't help,
		 * no matter how many cores you have. Below that, we tend to
		 * max at half the number of online_cpus(), presumably because
		 * half of those are hyperthreads rather than full cores. We'll
		 * never reduce the level below "3", though, to match a
		 * historical value that nobody complained about.
		 */
		ਜੇ (nr_ਧਾਗੇ < 4)
			; /* too few cores to consider capping */
		ਨਹੀਂਤਾਂ ਜੇ (nr_ਧਾਗੇ < 6)
			nr_ਧਾਗੇ = 3; /* historic cap */
		ਨਹੀਂਤਾਂ ਜੇ (nr_ਧਾਗੇ < 40)
			nr_ਧਾਗੇ /= 2;
		ਨਹੀਂਤਾਂ
			nr_ਧਾਗੇ = 20; /* hard cap */
	}

	curr_pack = open_pack_file(pack_name);
	parse_pack_ਸਿਰਲੇਖ();
	CALLOC_ARRAY(objects, st_ਜੋੜੋ(nr_objects, 1));
	ਜੇ (show_stat)
		CALLOC_ARRAY(obj_stat, st_ਜੋੜੋ(nr_objects, 1));
	CALLOC_ARRAY(ofs_deltas, nr_objects);
	parse_pack_objects(pack_hash);
	ਜੇ (report_ਸਿਰਾ_ਦਾ_ਨਿਵੇਸ਼)
		write_ਵਿੱਚ_ਭਰਿਆ(2, "\0", 1);
	resolve_deltas(&opts);
	conclude_pack(fix_thin_pack, curr_pack, pack_hash);
	ਮੁਕਤ(ofs_deltas);
	ਮੁਕਤ(ref_deltas);
	ਜੇ (strict)
		foreign_nr = check_objects();

	ਜੇ (show_stat)
		show_pack_info(stat_only);

	ALLOC_ARRAY(idx_objects, nr_objects);
	ਲਈ (i = 0; i < nr_objects; i++)
		idx_objects[i] = &objects[i].idx;
	curr_ਸੂਚਕਾਂਕ = write_idx_file(the_repository, ਸੂਚਕਾਂਕ_name, idx_objects,
				    nr_objects, &opts, pack_hash);
	ਜੇ (rev_ਸੂਚਕਾਂਕ)
		curr_rev_ਸੂਚਕਾਂਕ = write_rev_file(the_repository, rev_ਸੂਚਕਾਂਕ_name,
						idx_objects, nr_objects,
						pack_hash, opts.flags);
	ਮੁਕਤ(idx_objects);

	ਜੇ (!verify)
		final(pack_name, curr_pack,
		      ਸੂਚਕਾਂਕ_name, curr_ਸੂਚਕਾਂਕ,
		      rev_ਸੂਚਕਾਂਕ_name, curr_rev_ਸੂਚਕਾਂਕ,
		      keep_ਸੁਨੇਹਾ, promisor_ਸੁਨੇਹਾ,
		      pack_hash);
	ਨਹੀਂਤਾਂ
		close(ਨਿਵੇਸ਼_fd);

	ਜੇ (do_fsck_object) {
		/*
		 * We cannot perform queued consistency checks when running
		 * outside of a repository because those require us to read
		 * from the object database, which is uninitialized.
		 *
		 * TODO: we may eventually set up an in-memory object database,
		 * which would allow us to perform these queued checks.
		 */
		ਜੇ (!startup_info->have_repository &&
		    fsck_has_queued_checks(&fsck_options))
			die(_("cannot perform queued object checks outside "
			      "of a repository"));

		ਜੇ (fsck_ਅੰਤ(&fsck_options))
			die(_("fsck error in pack objects"));
	}

	ਮੁਕਤ(opts.anomaly);
	ਮੁਕਤ(objects);
	strbuf_release(&ਸੂਚਕਾਂਕ_name_buf);
	strbuf_release(&rev_ਸੂਚਕਾਂਕ_name_buf);
	ਜੇ (!pack_name)
		ਮੁਕਤ((ਖਾਲੀ *) curr_pack);
	ਜੇ (!ਸੂਚਕਾਂਕ_name)
		ਮੁਕਤ((ਖਾਲੀ *) curr_ਸੂਚਕਾਂਕ);
	ਮੁਕਤ(curr_rev_ਸੂਚਕਾਂਕ);

	repack_local_links();

	/*
	 * Let the caller know this pack is not self contained
	 */
	ਜੇ (check_self_contained_and_connected && foreign_nr)
		ਵਾਪਸ 1;

	ਵਾਪਸ 0;
}
