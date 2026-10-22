#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ "delta.h"
#ଅନ୍ତର୍ଭାଵ "environment.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "hex.h"
#ଅନ୍ତର୍ଭାଵ "pack.h"
#ଅନ୍ତର୍ଭାଵ "csum-file.h"
#ଅନ୍ତର୍ଭାଵ "blob.h"
#ଅନ୍ତର୍ଭାଵ "commit.h"
#ଅନ୍ତର୍ଭାଵ "tag.h"
#ଅନ୍ତର୍ଭାଵ "tree.h"
#ଅନ୍ତର୍ଭାଵ "progress.h"
#ଅନ୍ତର୍ଭାଵ "fsck.h"
#ଅନ୍ତର୍ଭାଵ "strbuf.h"
#ଅନ୍ତର୍ଭାଵ "thread-utils.h"
#ଅନ୍ତର୍ଭାଵ "packfile.h"
#ଅନ୍ତର୍ଭାଵ "pack-revindex.h"
#ଅନ୍ତର୍ଭାଵ "object-file.h"
#ଅନ୍ତର୍ଭାଵ "odb.h"
#ଅନ୍ତର୍ଭାଵ "odb/streaming.h"
#ଅନ୍ତର୍ଭାଵ "oid-array.h"
#ଅନ୍ତର୍ଭାଵ "oidset.h"
#ଅନ୍ତର୍ଭାଵ "path.h"
#ଅନ୍ତର୍ଭାଵ "replace-object.h"
#ଅନ୍ତର୍ଭାଵ "tree-walk.h"
#ଅନ୍ତର୍ଭାଵ "promisor-remote.h"
#ଅନ୍ତର୍ଭାଵ "run-command.h"
#ଅନ୍ତର୍ଭାଵ "setup.h"
#ଅନ୍ତର୍ଭାଵ "strvec.h"

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର index_pack_usage[] =
"git index-pack [-v] [-o <index-file>] [--keep | --keep=<msg>] [--[no-]rev-index] [--verify] [--strict[=<msg-id>=<severity>...]] [--fsck-objects[=<msg-id>=<severity>...]] (<pack-file> | --stdin [--fix-thin] [<pack-file>])";

ସଂରଚନା object_entry {
	ସଂରଚନା pack_idx_entry idx;
	size_t size;
	unsigned ଅକ୍ଷର hdr_size;
	signed ଅକ୍ଷର type;
	signed ଅକ୍ଷର real_type;
};

ସଂରଚନା object_stat {
	unsigned delta_depth;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା base_object_no;
};

ସଂରଚନା base_data {
	/* Initialized by make_base(). */
	ସଂରଚନା base_data *base;
	ସଂରଚନା object_entry *obj;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ref_first, ref_last;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ofs_first, ofs_last;
	/*
	 * Threads should increment retain_data if they are about to call
	 * patch_delta() using this struct's data as a base, and decrement this
	 * when they are done. While retain_data is nonzero, this struct's data
	 * will not be freed even if the delta base cache limit is exceeded.
	 */
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା retain_data;
	/*
	 * The number of direct children that have not been fully processed
	 * (entered work_head, entered done_head, left done_head). When this
	 * number reaches zero, this struct base_data can be freed.
	 */
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା children_remaining;

	/* Not initialized by make_base(). */
	ସଂରଚନା list_head list;
	ଶୂନ୍ୟ *data;
	size_t size;
};

/*
 * Stack of struct base_data that have unprocessed children.
 * threaded_second_pass() uses this as a source of work (the other being the
 * objects array).
 *
 * Guarded by work_mutex.
 */
ସ୍ଥିର LIST_HEAD(work_head);

/*
 * Stack of struct base_data that have children, all of whom have been
 * processed or are being processed, and at least one child is being processed.
 * These struct base_data must be kept around until the last child is
 * processed.
 *
 * Guarded by work_mutex.
 */
ସ୍ଥିର LIST_HEAD(done_head);

/*
 * All threads share one delta base cache.
 *
 * base_cache_used is guarded by work_mutex, and base_cache_limit is read-only
 * in a thread.
 */
ସ୍ଥିର size_t base_cache_used;
ସ୍ଥିର size_t base_cache_limit;

ସଂରଚନା thread_local_data {
	pthread_t thread;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା pack_fd;
};

/* Remember to update object flag allocation in object.h */
#define FLAG_LINK (1u<<20)
#define FLAG_CHECKED (1u<<21)

ସଂରଚନା ofs_delta_entry {
	off_t offset;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା obj_no;
};

ସଂରଚନା ref_delta_entry {
	ସଂରଚନା object_id oid;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା obj_no;
};

ସ୍ଥିର ସଂରଚନା object_entry *objects;
ସ୍ଥିର ସଂରଚନା object_stat *obj_stat;
ସ୍ଥିର ସଂରଚନା ofs_delta_entry *ofs_deltas;
ସ୍ଥିର ସଂରଚନା ref_delta_entry *ref_deltas;
ସ୍ଥିର ସଂରଚନା thread_local_data nothread_data;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା nr_objects;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା nr_ofs_deltas;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା nr_ref_deltas;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା ref_deltas_alloc;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା nr_resolved_deltas;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା nr_threads;

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା from_stdin;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା strict;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା do_fsck_object;
ସ୍ଥିର ସଂରଚନା fsck_options fsck_options;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା verbose;
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *progress_title;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_resolving_progress;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_stat;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା check_self_contained_and_connected;

ସ୍ଥିର ସଂରଚନା progress *progress;

ସ୍ଥିର unsigned ଅକ୍ଷର ନିବେଶ_buffer[DEFAULT_IO_BUFFER_SIZE];
ସ୍ଥିର unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ନିବେଶ_offset, ନିବେଶ_len;
ସ୍ଥିର off_t consumed_bytes;
ସ୍ଥିର off_t max_ନିବେଶ_size;
ସ୍ଥିର unsigned deepest_delta;
ସ୍ଥିର ସଂରଚନା git_hash_ctx ନିବେଶ_ctx;
ସ୍ଥିର uint32_t ନିବେଶ_crc32;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା ନିବେଶ_fd, output_fd;
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *curr_pack;

/*
 * outgoing_links is guarded by read_mutex, and record_outgoing_links is
 * read-only in a thread.
 */
ସ୍ଥିର ସଂରଚନା oidset outgoing_links = OIDSET_INIT;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା record_outgoing_links;

ସ୍ଥିର ସଂରଚନା thread_local_data *thread_data;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା nr_dispatched;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା threads_active;

ସ୍ଥିର pthread_mutex_t read_mutex;
#define read_lock()		lock_mutex(&read_mutex)
#define read_unlock()		unlock_mutex(&read_mutex)

ସ୍ଥିର pthread_mutex_t counter_mutex;
#define counter_lock()		lock_mutex(&counter_mutex)
#define counter_unlock()	unlock_mutex(&counter_mutex)

ସ୍ଥିର pthread_mutex_t work_mutex;
#define work_lock()		lock_mutex(&work_mutex)
#define work_unlock()		unlock_mutex(&work_mutex)

ସ୍ଥିର pthread_mutex_t deepest_delta_mutex;
#define deepest_delta_lock()	lock_mutex(&deepest_delta_mutex)
#define deepest_delta_unlock()	unlock_mutex(&deepest_delta_mutex)

ସ୍ଥିର pthread_ଚାବି_t ଚାବି;

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ lock_mutex(pthread_mutex_t *mutex)
{
	ଯଦି (threads_active)
		pthread_mutex_lock(mutex);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ unlock_mutex(pthread_mutex_t *mutex)
{
	ଯଦି (threads_active)
		pthread_mutex_unlock(mutex);
}

/*
 * Mutex and conditional variable can't be statically-initialized on Windows.
 */
ସ୍ଥିର ଶୂନ୍ୟ init_thread(ଶୂନ୍ୟ)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
	init_recursive_mutex(&read_mutex);
	pthread_mutex_init(&counter_mutex, ଖାଲି);
	pthread_mutex_init(&work_mutex, ଖାଲି);
	ଯଦି (show_stat)
		pthread_mutex_init(&deepest_delta_mutex, ଖାଲି);
	pthread_ଚାବି_create(&ଚାବି, ଖାଲି);
	CALLOC_ARRAY(thread_data, nr_threads);
	ପାଇଁ (i = 0; i < nr_threads; i++) {
		thread_data[i].pack_fd = xopen(curr_pack, O_RDONLY);
	}

	threads_active = 1;
}

ସ୍ଥିର ଶୂନ୍ୟ cleanup_thread(ଶୂନ୍ୟ)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
	ଯଦି (!threads_active)
		ଫେରନ୍ତୁ;
	threads_active = 0;
	pthread_mutex_destroy(&read_mutex);
	pthread_mutex_destroy(&counter_mutex);
	pthread_mutex_destroy(&work_mutex);
	ଯଦି (show_stat)
		pthread_mutex_destroy(&deepest_delta_mutex);
	ପାଇଁ (i = 0; i < nr_threads; i++)
		close(thread_data[i].pack_fd);
	pthread_ଚାବି_delete(ଚାବି);
	ମୁକ୍ତ(thread_data);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା mark_link(ସଂରଚନା object *obj, ଗଣନା object_type type,
		     ଶୂନ୍ୟ *data UNUSED,
		     ସଂରଚନା fsck_options *options UNUSED)
{
	ଯଦି (!obj)
		ଫେରନ୍ତୁ -1;

	ଯଦି (type != OBJ_ANY && obj->type != type)
		die(_("object type mismatch at %s"), oid_to_hex(&obj->oid));

	obj->flags |= FLAG_LINK;
	ଫେରନ୍ତୁ 0;
}

/* The content of each linked object must have been checked
   or it must be already present in the object database */
ସ୍ଥିର unsigned check_object(ସଂରଚନା object *obj)
{
	ଯଦି (!obj)
		ଫେରନ୍ତୁ 0;

	ଯଦି (!(obj->flags & FLAG_LINK))
		ଫେରନ୍ତୁ 0;

	ଯଦି (!(obj->flags & FLAG_CHECKED)) {
		size_t size;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା type = odb_read_object_info(the_repository->objects,
						&obj->oid, &size);
		ଯଦି (type <= 0)
			die(_("did not receive expected object %s"),
			      oid_to_hex(&obj->oid));
		ଯଦି (type != obj->type)
			die(_("object %s: expected type %s, found %s"),
			    oid_to_hex(&obj->oid),
			    type_name(obj->type), type_name(type));
		obj->flags |= FLAG_CHECKED;
		ଫେରନ୍ତୁ 1;
	}

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର unsigned check_objects(ଶୂନ୍ୟ)
{
	unsigned i, max, foreign_nr = 0;

	max = get_max_object_index(the_repository);

	ଯଦି (verbose)
		progress = start_delayed_progress(the_repository,
						  _("Checking objects"), max);

	ପାଇଁ (i = 0; i < max; i++) {
		foreign_nr += check_object(get_indexed_object(the_repository, i));
		display_progress(progress, i + 1);
	}

	stop_progress(&progress);
	ଫେରନ୍ତୁ foreign_nr;
}


/* Discard current buffer used content. */
ସ୍ଥିର ଶୂନ୍ୟ flush(ଶୂନ୍ୟ)
{
	ଯଦି (ନିବେଶ_offset) {
		ଯଦି (output_fd >= 0)
			write_or_die(output_fd, ନିବେଶ_buffer, ନିବେଶ_offset);
		git_hash_update(&ନିବେଶ_ctx, ନିବେଶ_buffer, ନିବେଶ_offset);
		memmove(ନିବେଶ_buffer, ନିବେଶ_buffer + ନିବେଶ_offset, ନିବେଶ_len);
		ନିବେଶ_offset = 0;
	}
}

/*
 * Make sure at least "min" bytes are available in the buffer, and
 * return the pointer to the buffer.
 */
ସ୍ଥିର ଶୂନ୍ୟ *fill(ପୂର୍ଣ୍ଣସଂଖ୍ୟା min)
{
	ଯଦି (min <= ନିବେଶ_len)
		ଫେରନ୍ତୁ ନିବେଶ_buffer + ନିବେଶ_offset;
	ଯଦି (min > ଆକାର(ନିବେଶ_buffer))
		die(Q_("cannot fill %d byte",
		       "cannot fill %d bytes",
		       min),
		    min);
	flush();
	କର {
		ssize_t ret = xread(ନିବେଶ_fd, ନିବେଶ_buffer + ନିବେଶ_len,
				ଆକାର(ନିବେଶ_buffer) - ନିବେଶ_len);
		ଯଦି (ret <= 0) {
			ଯଦି (!ret)
				die(_("early EOF"));
			die_errno(_("read error on input"));
		}
		ନିବେଶ_len += ret;
		ଯଦି (from_stdin)
			display_throughput(progress, consumed_bytes + ନିବେଶ_len);
	} ଯେତେବେଳେ (ନିବେଶ_len < min);
	ଫେରନ୍ତୁ ନିବେଶ_buffer;
}

ସ୍ଥିର ଶୂନ୍ୟ use(ପୂର୍ଣ୍ଣସଂଖ୍ୟା bytes)
{
	ଯଦି (bytes > ନିବେଶ_len)
		die(_("used more bytes than were available"));
	ନିବେଶ_crc32 = crc32(ନିବେଶ_crc32, ନିବେଶ_buffer + ନିବେଶ_offset, bytes);
	ନିବେଶ_len -= bytes;
	ନିବେଶ_offset += bytes;

	/* make sure off_t is sufficiently large not to wrap */
	ଯଦି (signed_add_overflows(consumed_bytes, bytes))
		die(_("pack too large for current definition of off_t"));
	consumed_bytes += bytes;
	ଯଦି (max_ନିବେଶ_size && consumed_bytes > max_ନିବେଶ_size) {
		ସଂରଚନା strbuf size_limit = STRBUF_INIT;
		strbuf_humanise_bytes(&size_limit, max_ନିବେଶ_size);
		die(_("pack exceeds maximum allowed size (%s)"),
		    size_limit.buf);
	}
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *open_pack_file(ଅଚଳ ଅକ୍ଷର *pack_name)
{
	ଯଦି (from_stdin) {
		ନିବେଶ_fd = 0;
		ଯଦି (!pack_name) {
			ସଂରଚନା strbuf tmp_file = STRBUF_INIT;
			output_fd = odb_mkstemp(the_repository->objects, &tmp_file,
						"pack/tmp_pack_XXXXXX");
			pack_name = strbuf_detach(&tmp_file, ଖାଲି);
		} ଅନ୍ୟଥା {
			output_fd = xopen(pack_name, O_CREAT|O_EXCL|O_RDWR, 0600);
		}
		nothread_data.pack_fd = output_fd;
	} ଅନ୍ୟଥା {
		ନିବେଶ_fd = xopen(pack_name, O_RDONLY);
		output_fd = -1;
		nothread_data.pack_fd = ନିବେଶ_fd;
	}
	git_hash_init(&ନିବେଶ_ctx, the_hash_algo);
	ଫେରନ୍ତୁ pack_name;
}

ସ୍ଥିର ଶୂନ୍ୟ parse_pack_header(ଶୂନ୍ୟ)
{
	unsigned ଅକ୍ଷର *hdr = fill(ଆକାର(ସଂରଚନା pack_header));

	/* Header consistency check */
	ଯଦି (get_be32(hdr) != PACK_SIGNATURE)
		die(_("pack signature mismatch"));
	hdr += 4;
	ଯଦି (!pack_version_ok_native(get_be32(hdr)))
		die(_("pack version %"PRIu32" unsupported"),
		    get_be32(hdr));
	hdr += 4;

	nr_objects = get_be32(hdr);
	use(ଆକାର(ସଂରଚନା pack_header));
}

__attribute__((format (printf, 2, 3)))
ସ୍ଥିର NORETURN ଶୂନ୍ୟ bad_object(off_t offset, ଅଚଳ ଅକ୍ଷର *format, ...)
{
	va_list params;
	ଅକ୍ଷର buf[1024];

	va_start(params, format);
	vsnprintf(buf, ଆକାର(buf), format, params);
	va_end(params);
	die(_("pack has bad object at offset %"PRIuMAX": %s"),
	    (uintmax_t)offset, buf);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା thread_local_data *get_thread_data(ଶୂନ୍ୟ)
{
	ଯଦି (HAVE_THREADS) {
		ଯଦି (threads_active)
			ଫେରନ୍ତୁ pthread_getspecific(ଚାବି);
		ପ୍ରତିଜ୍ଞା(!threads_active &&
		       "This should only be reached when all threads are gone");
	}
	ଫେରନ୍ତୁ &nothread_data;
}

ସ୍ଥିର ଶୂନ୍ୟ set_thread_data(ସଂରଚନା thread_local_data *data)
{
	ଯଦି (threads_active)
		pthread_setspecific(ଚାବି, data);
}

ସ୍ଥିର ଶୂନ୍ୟ free_base_data(ସଂରଚନା base_data *c)
{
	ଯଦି (c->data) {
		FREE_AND_NULL(c->data);
		base_cache_used -= c->size;
	}
}

ସ୍ଥିର ଶୂନ୍ୟ prune_base_data(ସଂରଚନା base_data *retain)
{
	ସଂରଚନା list_head *pos;

	ଯଦି (base_cache_used <= base_cache_limit)
		ଫେରନ୍ତୁ;

	list_for_each_prev(pos, &done_head) {
		ସଂରଚନା base_data *b = list_entry(pos, ସଂରଚନା base_data, list);
		ଯଦି (b->retain_data || b == retain)
			ଜାରି;
		ଯଦି (b->data) {
			free_base_data(b);
			ଯଦି (base_cache_used <= base_cache_limit)
				ଫେରନ୍ତୁ;
		}
	}

	list_for_each_prev(pos, &work_head) {
		ସଂରଚନା base_data *b = list_entry(pos, ସଂରଚନା base_data, list);
		ଯଦି (b->retain_data || b == retain)
			ଜାରି;
		ଯଦି (b->data) {
			free_base_data(b);
			ଯଦି (base_cache_used <= base_cache_limit)
				ଫେରନ୍ତୁ;
		}
	}
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା is_delta_type(ଗଣନା object_type type)
{
	ଫେରନ୍ତୁ (type == OBJ_REF_DELTA || type == OBJ_OFS_DELTA);
}

ସ୍ଥିର ଶୂନ୍ୟ *unpack_entry_data(off_t offset, size_t size,
			       ଗଣନା object_type type, ସଂରଚନା object_id *oid)
{
	ସ୍ଥିର ଅକ୍ଷର fixed_buf[8192];
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା status;
	git_zstream stream;
	ଶୂନ୍ୟ *buf;
	ସଂରଚନା git_hash_ctx c;
	ଅକ୍ଷର hdr[32];
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା hdrlen;

	ଯଦି (!is_delta_type(type)) {
		hdrlen = format_object_header(hdr, ଆକାର(hdr), type, size);
		git_hash_init(&c, the_hash_algo);
		git_hash_update(&c, hdr, hdrlen);
	} ଅନ୍ୟଥା
		oid = ଖାଲି;
	ଯଦି (type == OBJ_BLOB &&
	    size > repo_settings_get_big_file_threshold(the_repository))
		buf = fixed_buf;
	ଅନ୍ୟଥା
		buf = xmallocz(size);

	memset(&stream, 0, ଆକାର(stream));
	git_inflate_init(&stream);
	stream.next_out = buf;
	stream.avail_out = buf == fixed_buf ? ଆକାର(fixed_buf) : size;

	କର {
		unsigned ଅକ୍ଷର *last_out = stream.next_out;
		stream.next_in = fill(1);
		stream.avail_in = ନିବେଶ_len;
		status = git_inflate(&stream, 0);
		use(ନିବେଶ_len - stream.avail_in);
		ଯଦି (oid)
			git_hash_update(&c, last_out, stream.next_out - last_out);
		ଯଦି (buf == fixed_buf) {
			stream.next_out = buf;
			stream.avail_out = ଆକାର(fixed_buf);
		}
	} ଯେତେବେଳେ (status == Z_OK);
	ଯଦି (stream.total_out != size || status != Z_STREAM_END)
		bad_object(offset, _("inflate returned %d"), status);
	git_inflate_end(&stream);
	ଯଦି (oid)
		git_hash_final_oid(oid, &c);
	ଫେରନ୍ତୁ buf == fixed_buf ? ଖାଲି : buf;
}

ସ୍ଥିର ଶୂନ୍ୟ *unpack_raw_entry(ସଂରଚନା object_entry *obj,
			      off_t *ofs_offset,
			      ସଂରଚନା object_id *ref_oid,
			      ସଂରଚନା object_id *oid)
{
	unsigned ଅକ୍ଷର *p;
	size_t size, c;
	off_t base_offset;
	unsigned shift;
	ଶୂନ୍ୟ *data;

	obj->idx.offset = consumed_bytes;
	ନିବେଶ_crc32 = crc32(0, ଖାଲି, 0);

	p = fill(1);
	c = *p;
	use(1);
	obj->type = (c >> 4) & 7;
	size = (c & 15);
	shift = 4;
	ଯେତେବେଳେ (c & 0x80) {
		ଯଦି ((bitsizeof(size_t) - 7) < shift)
			die(_("object size too large for this platform"));
		p = fill(1);
		c = *p;
		use(1);
		size += (c & 0x7f) << shift;
		shift += 7;
	}
	obj->size = size;

	ବାଛ (obj->type) {
	ସ୍ଥିତି OBJ_REF_DELTA:
		oidread(ref_oid, fill(the_hash_algo->rawsz),
			the_repository->hash_algo);
		use(the_hash_algo->rawsz);
		ଭାଙ୍ଗ;
	ସ୍ଥିତି OBJ_OFS_DELTA:
		p = fill(1);
		c = *p;
		use(1);
		base_offset = c & 127;
		ଯେତେବେଳେ (c & 128) {
			base_offset += 1;
			ଯଦି (!base_offset || MSB(base_offset, 7))
				bad_object(obj->idx.offset, _("offset value overflow for delta base object"));
			p = fill(1);
			c = *p;
			use(1);
			base_offset = (base_offset << 7) + (c & 127);
		}
		*ofs_offset = obj->idx.offset - base_offset;
		ଯଦି (*ofs_offset <= 0 || *ofs_offset >= obj->idx.offset)
			bad_object(obj->idx.offset, _("delta base offset is out of bound"));
		ଭାଙ୍ଗ;
	ସ୍ଥିତି OBJ_COMMIT:
	ସ୍ଥିତି OBJ_ବୃକ୍ଷ:
	ସ୍ଥିତି OBJ_BLOB:
	ସ୍ଥିତି OBJ_TAG:
		ଭାଙ୍ଗ;
	ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
		bad_object(obj->idx.offset, _("unknown object type %d"), obj->type);
	}
	obj->hdr_size = consumed_bytes - obj->idx.offset;

	data = unpack_entry_data(obj->idx.offset, obj->size, obj->type, oid);
	obj->idx.crc32 = ନିବେଶ_crc32;
	ଫେରନ୍ତୁ data;
}

ସ୍ଥିର ଶୂନ୍ୟ *unpack_data(ସଂରଚନା object_entry *obj,
			 ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*consume)(ଅଚଳ unsigned ଅକ୍ଷର *, unsigned long, ଶୂନ୍ୟ *),
			 ଶୂନ୍ୟ *cb_data)
{
	off_t from = obj[0].idx.offset + obj[0].hdr_size;
	off_t len = obj[1].idx.offset - from;
	unsigned ଅକ୍ଷର *data, *inbuf;
	git_zstream stream;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା status;

	data = xmallocz(consume ? 64*1024 : obj->size);
	inbuf = xmalloc((len < 64*1024) ? (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)len : 64*1024);

	memset(&stream, 0, ଆକାର(stream));
	git_inflate_init(&stream);
	stream.next_out = data;
	stream.avail_out = consume ? 64*1024 : obj->size;

	କର {
		ssize_t n = (len < 64*1024) ? (ssize_t)len : 64*1024;
		n = xpread(get_thread_data()->pack_fd, inbuf, n, from);
		ଯଦି (n < 0)
			die_errno(_("cannot pread pack file"));
		ଯଦି (!n)
			die(Q_("premature end of pack file, %"PRIuMAX" byte missing",
			       "premature end of pack file, %"PRIuMAX" bytes missing",
			       len),
			    (uintmax_t)len);
		from += n;
		len -= n;
		stream.next_in = inbuf;
		stream.avail_in = n;
		ଯଦି (!consume)
			status = git_inflate(&stream, 0);
		ଅନ୍ୟଥା {
			କର {
				status = git_inflate(&stream, 0);
				ଯଦି (consume(data, stream.next_out - data, cb_data)) {
					ମୁକ୍ତ(inbuf);
					ମୁକ୍ତ(data);
					ଫେରନ୍ତୁ ଖାଲି;
				}
				stream.next_out = data;
				stream.avail_out = 64*1024;
			} ଯେତେବେଳେ (status == Z_OK && stream.avail_in);
		}
	} ଯେତେବେଳେ (len && status == Z_OK && !stream.avail_in);

	/* This has been inflated OK when first encountered, so... */
	ଯଦି (status != Z_STREAM_END || stream.total_out != obj->size)
		die(_("serious inflate inconsistency"));

	git_inflate_end(&stream);
	ମୁକ୍ତ(inbuf);
	ଯଦି (consume) {
		FREE_AND_NULL(data);
	}
	ଫେରନ୍ତୁ data;
}

ସ୍ଥିର ଶୂନ୍ୟ *get_data_from_pack(ସଂରଚନା object_entry *obj)
{
	ଫେରନ୍ତୁ unpack_data(obj, ଖାଲି, ଖାଲି);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା compare_ofs_delta_bases(off_t offset1, off_t offset2,
				   ଗଣନା object_type type1,
				   ଗଣନା object_type type2)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp = type1 - type2;
	ଯଦି (cmp)
		ଫେରନ୍ତୁ cmp;
	ଫେରନ୍ତୁ offset1 < offset2 ? -1 :
	       offset1 > offset2 ?  1 :
	       0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା find_ofs_delta(ଅଚଳ off_t offset)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା first = 0, last = nr_ofs_deltas;

	ଯେତେବେଳେ (first < last) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା next = first + (last - first) / 2;
		ସଂରଚନା ofs_delta_entry *delta = &ofs_deltas[next];
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp;

		cmp = compare_ofs_delta_bases(offset, delta->offset,
					      OBJ_OFS_DELTA,
					      objects[delta->obj_no].type);
		ଯଦି (!cmp)
			ଫେରନ୍ତୁ next;
		ଯଦି (cmp < 0) {
			last = next;
			ଜାରି;
		}
		first = next+1;
	}
	ଫେରନ୍ତୁ -first-1;
}

ସ୍ଥିର ଶୂନ୍ୟ find_ofs_delta_children(off_t offset,
				    ପୂର୍ଣ୍ଣସଂଖ୍ୟା *first_index, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *last_index)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା first = find_ofs_delta(offset);
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା last = first;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା end = nr_ofs_deltas - 1;

	ଯଦି (first < 0) {
		*first_index = 0;
		*last_index = -1;
		ଫେରନ୍ତୁ;
	}
	ଯେତେବେଳେ (first > 0 && ofs_deltas[first - 1].offset == offset)
		--first;
	ଯେତେବେଳେ (last < end && ofs_deltas[last + 1].offset == offset)
		++last;
	*first_index = first;
	*last_index = last;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା compare_ref_delta_bases(ଅଚଳ ସଂରଚନା object_id *oid1,
				   ଅଚଳ ସଂରଚନା object_id *oid2,
				   ଗଣନା object_type type1,
				   ଗଣନା object_type type2)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp = type1 - type2;
	ଯଦି (cmp)
		ଫେରନ୍ତୁ cmp;
	ଫେରନ୍ତୁ oidcmp(oid1, oid2);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା find_ref_delta(ଅଚଳ ସଂରଚନା object_id *oid)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା first = 0, last = nr_ref_deltas;

	ଯେତେବେଳେ (first < last) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା next = first + (last - first) / 2;
		ସଂରଚନା ref_delta_entry *delta = &ref_deltas[next];
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp;

		cmp = compare_ref_delta_bases(oid, &delta->oid,
					      OBJ_REF_DELTA,
					      objects[delta->obj_no].type);
		ଯଦି (!cmp)
			ଫେରନ୍ତୁ next;
		ଯଦି (cmp < 0) {
			last = next;
			ଜାରି;
		}
		first = next+1;
	}
	ଫେରନ୍ତୁ -first-1;
}

ସ୍ଥିର ଶୂନ୍ୟ find_ref_delta_children(ଅଚଳ ସଂରଚନା object_id *oid,
				    ପୂର୍ଣ୍ଣସଂଖ୍ୟା *first_index, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *last_index)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା first = find_ref_delta(oid);
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା last = first;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା end = nr_ref_deltas - 1;

	ଯଦି (first < 0) {
		*first_index = 0;
		*last_index = -1;
		ଫେରନ୍ତୁ;
	}
	ଯେତେବେଳେ (first > 0 && oideq(&ref_deltas[first - 1].oid, oid))
		--first;
	ଯେତେବେଳେ (last < end && oideq(&ref_deltas[last + 1].oid, oid))
		++last;
	*first_index = first;
	*last_index = last;
}

ସଂରଚନା compare_data {
	ସଂରଚନା object_entry *entry;
	ସଂରଚନା odb_stream *st;
	unsigned ଅକ୍ଷର *buf;
	unsigned long buf_size;
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା compare_objects(ଅଚଳ unsigned ଅକ୍ଷର *buf, unsigned long size,
			   ଶୂନ୍ୟ *cb_data)
{
	ସଂରଚନା compare_data *data = cb_data;

	ଯଦି (data->buf_size < size) {
		ମୁକ୍ତ(data->buf);
		data->buf = xmalloc(size);
		data->buf_size = size;
	}

	ଯେତେବେଳେ (size) {
		ssize_t len = odb_stream_read(data->st, data->buf, size);
		ଯଦି (len == 0)
			die(_("SHA1 COLLISION FOUND WITH %s !"),
			    oid_to_hex(&data->entry->idx.oid));
		ଯଦି (len < 0)
			die(_("unable to read %s"),
			    oid_to_hex(&data->entry->idx.oid));
		ଯଦି (memcmp(buf, data->buf, len))
			die(_("SHA1 COLLISION FOUND WITH %s !"),
			    oid_to_hex(&data->entry->idx.oid));
		size -= len;
		buf += len;
	}
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା check_collison(ସଂରଚନା object_entry *entry)
{
	ସଂରଚନା compare_data data;

	ଯଦି (entry->size <= repo_settings_get_big_file_threshold(the_repository) ||
	    entry->type != OBJ_BLOB)
		ଫେରନ୍ତୁ -1;

	memset(&data, 0, ଆକାର(data));
	data.entry = entry;
	data.st = odb_stream_from_object(the_repository->objects, &entry->idx.oid, ଖାଲି);
	ଯଦି (!data.st)
		ଫେରନ୍ତୁ -1;
	ଯଦି (data.st->size != entry->size || data.st->type != entry->type)
		die(_("SHA1 COLLISION FOUND WITH %s !"),
		    oid_to_hex(&entry->idx.oid));
	unpack_data(entry, compare_objects, &data);
	odb_stream_close(data.st);
	ମୁକ୍ତ(data.buf);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ record_outgoing_link(ଅଚଳ ସଂରଚନା object_id *oid)
{
	oidset_ଭର୍ତ୍ତି(&outgoing_links, oid);
}

ସ୍ଥିର ଶୂନ୍ୟ maybe_record_name_entry(ଅଚଳ ସଂରଚନା name_entry *entry)
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
	ଯଦି (S_ISDIR(entry->mode))
		record_outgoing_link(&entry->oid);
}

ସ୍ଥିର ଶୂନ୍ୟ do_record_outgoing_links(ସଂରଚନା object *obj)
{
	ଯଦି (obj->type == OBJ_ବୃକ୍ଷ) {
		ସଂରଚନା ବୃକ୍ଷ *ବୃକ୍ଷ = (ସଂରଚନା ବୃକ୍ଷ *)obj;
		ସଂରଚନା ବୃକ୍ଷ_desc desc;
		ସଂରଚନା name_entry entry;
		ଯଦି (init_ବୃକ୍ଷ_desc_gently(&desc, &ବୃକ୍ଷ->object.oid,
					  ବୃକ୍ଷ->buffer, ବୃକ୍ଷ->size, 0))
			/*
			 * Error messages are given when packs are
			 * verified, so do not print any here.
			 */
			ଫେରନ୍ତୁ;
		ଯେତେବେଳେ (ବୃକ୍ଷ_entry_gently(&desc, &entry))
			maybe_record_name_entry(&entry);
	} ଅନ୍ୟଥା ଯଦି (obj->type == OBJ_COMMIT) {
		ସଂରଚନା commit *commit = (ସଂରଚନା commit *) obj;
		ସଂରଚନା commit_list *parents = commit->parents;

		record_outgoing_link(get_commit_ବୃକ୍ଷ_oid(commit));
		ପାଇଁ (; parents; parents = parents->next)
			record_outgoing_link(&parents->item->object.oid);
	} ଅନ୍ୟଥା ଯଦି (obj->type == OBJ_TAG) {
		ସଂରଚନା tag *tag = (ସଂରଚନା tag *) obj;
		record_outgoing_link(get_tagged_oid(tag));
	}
}

ସ୍ଥିର ଶୂନ୍ୟ sha1_object(ଅଚଳ ଶୂନ୍ୟ *data, ସଂରଚନା object_entry *obj_entry,
			unsigned long size, ଗଣନା object_type type,
			ଅଚଳ ସଂରଚନା object_id *oid)
{
	ଶୂନ୍ୟ *new_data = ଖାଲି;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା collision_test_needed = 0;

	ପ୍ରତିଜ୍ଞା(data || obj_entry);

	ଯଦି (startup_info->have_repository) {
		read_lock();
		collision_test_needed = odb_has_object(the_repository->objects, oid,
						       ODB_HAS_OBJECT_FETCH_PROMISOR);
		read_unlock();
	}

	ଯଦି (collision_test_needed && !data) {
		read_lock();
		ଯଦି (!check_collison(obj_entry))
			collision_test_needed = 0;
		read_unlock();
	}
	ଯଦି (collision_test_needed) {
		ଶୂନ୍ୟ *has_data;
		ଗଣନା object_type has_type;
		size_t has_size;
		read_lock();
		has_type = odb_read_object_info(the_repository->objects, oid, &has_size);
		ଯଦି (has_type < 0)
			die(_("cannot read existing object info %s"), oid_to_hex(oid));
		ଯଦି (has_type != type || has_size != size)
			die(_("SHA1 COLLISION FOUND WITH %s !"), oid_to_hex(oid));
		has_data = odb_read_object(the_repository->objects, oid,
					   &has_type, &has_size);
		read_unlock();
		ଯଦି (!data)
			data = new_data = get_data_from_pack(obj_entry);
		ଯଦି (!has_data)
			die(_("cannot read existing object %s"), oid_to_hex(oid));
		ଯଦି (size != has_size || type != has_type ||
		    memcmp(data, has_data, size) != 0)
			die(_("SHA1 COLLISION FOUND WITH %s !"), oid_to_hex(oid));
		ମୁକ୍ତ(has_data);
	}

	ଯଦି (strict || do_fsck_object || record_outgoing_links) {
		read_lock();
		ଯଦି (type == OBJ_BLOB) {
			ସଂରଚନା blob *blob = lookup_blob(the_repository, oid);
			ଯଦି (blob)
				blob->object.flags |= FLAG_CHECKED;
			ଅନ୍ୟଥା
				die(_("invalid blob object %s"), oid_to_hex(oid));
			ଯଦି (do_fsck_object &&
			    fsck_object(&blob->object, (ଶୂନ୍ୟ *)data, size, &fsck_options))
				die(_("fsck error in packed object"));
		} ଅନ୍ୟଥା {
			ସଂରଚନା object *obj;
			ପୂର୍ଣ୍ଣସଂଖ୍ୟା eaten;
			ଶୂନ୍ୟ *buf = (ଶୂନ୍ୟ *) data;

			ପ୍ରତିଜ୍ଞା(data && "data can only be NULL for large _blobs_");

			/*
			 * we do not need to free the memory here, as the
			 * buf is deleted by the caller.
			 */
			obj = parse_object_buffer(the_repository, oid, type,
						  size, buf,
						  &eaten);
			ଯଦି (!obj)
				die(_("invalid %s"), type_name(type));
			ଯଦି (do_fsck_object &&
			    fsck_object(obj, buf, size, &fsck_options))
				die(_("fsck error in packed object"));
			ଯଦି (strict && fsck_walk(obj, ଖାଲି, &fsck_options))
				die(_("Not all child objects of %s are reachable"), oid_to_hex(&obj->oid));
			ଯଦି (record_outgoing_links)
				do_record_outgoing_links(obj);

			ଯଦି (obj->type == OBJ_ବୃକ୍ଷ) {
				ସଂରଚନା ବୃକ୍ଷ *item = (ସଂରଚନା ବୃକ୍ଷ *) obj;
				item->buffer = ଖାଲି;
				obj->parsed = 0;
			}
			ଯଦି (obj->type == OBJ_COMMIT) {
				ସଂରଚନା commit *commit = (ସଂରଚନା commit *) obj;
				ଯଦି (detach_commit_buffer(commit, ଖାଲି) != data)
					BUG("parse_object_buffer transmogrified our buffer");
			}
			obj->flags |= FLAG_CHECKED;
		}
		read_unlock();
	}

	ମୁକ୍ତ(new_data);
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
ସ୍ଥିର ଶୂନ୍ୟ *get_base_data(ସଂରଚନା base_data *c)
{
	ଯଦି (!c->data) {
		ସଂରଚନା object_entry *obj = c->obj;
		ସଂରଚନା base_data **delta = ଖାଲି;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା delta_nr = 0, delta_alloc = 0;

		ଯେତେବେଳେ (is_delta_type(c->obj->type) && !c->data) {
			ALLOC_GROW(delta, delta_nr + 1, delta_alloc);
			delta[delta_nr++] = c;
			c = c->base;
		}
		ଯଦି (!delta_nr) {
			c->data = get_data_from_pack(obj);
			c->size = obj->size;
			base_cache_used += c->size;
			prune_base_data(c);
		}
		ପାଇଁ (; delta_nr > 0; delta_nr--) {
			ଶୂନ୍ୟ *base, *raw;
			c = delta[delta_nr - 1];
			obj = c->obj;
			base = get_base_data(c->base);
			raw = get_data_from_pack(obj);
			c->data = patch_delta(
				base, c->base->size,
				raw, obj->size,
				&c->size);
			ମୁକ୍ତ(raw);
			ଯଦି (!c->data)
				bad_object(obj->idx.offset, _("failed to apply delta"));
			base_cache_used += c->size;
			prune_base_data(c);
		}
		ମୁକ୍ତ(delta);
	}
	ଫେରନ୍ତୁ c->data;
}

ସ୍ଥିର ସଂରଚନା base_data *make_base(ସଂରଚନା object_entry *obj,
				   ସଂରଚନା base_data *parent)
{
	ସଂରଚନା base_data *base = xcalloc(1, ଆକାର(ସଂରଚନା base_data));
	base->base = parent;
	base->obj = obj;
	find_ref_delta_children(&obj->idx.oid,
				&base->ref_first, &base->ref_last);
	find_ofs_delta_children(obj->idx.offset,
				&base->ofs_first, &base->ofs_last);
	base->children_remaining = base->ref_last - base->ref_first +
		base->ofs_last - base->ofs_first + 2;
	ଫେରନ୍ତୁ base;
}

ସ୍ଥିର ସଂରଚନା base_data *resolve_delta(ସଂରଚନା object_entry *delta_obj,
				       ସଂରଚନା base_data *base)
{
	ଶୂନ୍ୟ *delta_data, *result_data;
	ସଂରଚନା base_data *result;
	size_t result_size;

	ଯଦି (show_stat) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = delta_obj - objects;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = base->obj - objects;
		obj_stat[i].delta_depth = obj_stat[j].delta_depth + 1;
		deepest_delta_lock();
		ଯଦି (deepest_delta < obj_stat[i].delta_depth)
			deepest_delta = obj_stat[i].delta_depth;
		deepest_delta_unlock();
		obj_stat[i].base_object_no = j;
	}
	delta_data = get_data_from_pack(delta_obj);
	ପ୍ରତିଜ୍ଞା(base->data);
	result_data = patch_delta(base->data, base->size,
				  delta_data, delta_obj->size, &result_size);
	ମୁକ୍ତ(delta_data);
	ଯଦି (!result_data)
		bad_object(delta_obj->idx.offset, _("failed to apply delta"));
	hash_object_file(the_hash_algo, result_data, result_size,
			 delta_obj->real_type, &delta_obj->idx.oid);
	sha1_object(result_data, ଖାଲି, result_size, delta_obj->real_type,
		    &delta_obj->idx.oid);

	result = make_base(delta_obj, base);
	result->data = result_data;
	result->size = result_size;

	counter_lock();
	nr_resolved_deltas++;
	counter_unlock();

	ଫେରନ୍ତୁ result;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା compare_ofs_delta_entry(ଅଚଳ ଶୂନ୍ୟ *a, ଅଚଳ ଶୂନ୍ୟ *b)
{
	ଅଚଳ ସଂରଚନା ofs_delta_entry *delta_a = a;
	ଅଚଳ ସଂରଚନା ofs_delta_entry *delta_b = b;

	ଫେରନ୍ତୁ delta_a->offset < delta_b->offset ? -1 :
	       delta_a->offset > delta_b->offset ?  1 :
	       0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା compare_ref_delta_entry(ଅଚଳ ଶୂନ୍ୟ *a, ଅଚଳ ଶୂନ୍ୟ *b)
{
	ଅଚଳ ସଂରଚନା ref_delta_entry *delta_a = a;
	ଅଚଳ ସଂରଚନା ref_delta_entry *delta_b = b;

	ଫେରନ୍ତୁ oidcmp(&delta_a->oid, &delta_b->oid);
}

ସ୍ଥିର ଶୂନ୍ୟ *threaded_second_pass(ଶୂନ୍ୟ *data)
{
	ଯଦି (data)
		set_thread_data(data);
	ପାଇଁ (;;) {
		ସଂରଚନା base_data *parent = ଖାଲି;
		ସଂରଚନା object_entry *child_obj = ଖାଲି;
		ସଂରଚନା base_data *child = ଖାଲି;

		counter_lock();
		display_progress(progress, nr_resolved_deltas);
		counter_unlock();

		work_lock();
		ଯଦି (list_empty(&work_head)) {
			/*
			 * Take an object from the object array.
			 */
			ଯେତେବେଳେ (nr_dispatched < nr_objects &&
			       is_delta_type(objects[nr_dispatched].type))
				nr_dispatched++;
			ଯଦି (nr_dispatched >= nr_objects) {
				work_unlock();
				ଭାଙ୍ଗ;
			}
			child_obj = &objects[nr_dispatched++];
		} ଅନ୍ୟଥା {
			/*
			 * Peek at the top of the stack, and take a child from
			 * it.
			 */
			parent = list_first_entry(&work_head, ସଂରଚନା base_data,
						  list);

			ଯେତେବେଳେ (parent->ref_first <= parent->ref_last) {
				ପୂର୍ଣ୍ଣସଂଖ୍ୟା offset = ref_deltas[parent->ref_first++].obj_no;
				child_obj = objects + offset;
				ଯଦି (child_obj->real_type != OBJ_REF_DELTA) {
					child_obj = ଖାଲି;
					ଜାରି;
				}
				child_obj->real_type = parent->obj->real_type;
				ଭାଙ୍ଗ;
			}

			ଯଦି (!child_obj && parent->ofs_first <= parent->ofs_last) {
				child_obj = objects +
					ofs_deltas[parent->ofs_first++].obj_no;
				ପ୍ରତିଜ୍ଞା(child_obj->real_type == OBJ_OFS_DELTA);
				child_obj->real_type = parent->obj->real_type;
			}

			ଯଦି (parent->ref_first > parent->ref_last &&
			    parent->ofs_first > parent->ofs_last) {
				/*
				 * This parent has run out of children, so move
				 * it to done_head.
				 */
				list_del(&parent->list);
				list_add(&parent->list, &done_head);
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
			get_base_data(parent);
			parent->retain_data++;
		}
		work_unlock();

		ଯଦି (child_obj) {
			ଯଦି (parent) {
				child = resolve_delta(child_obj, parent);
				ଯଦି (!child->children_remaining)
					FREE_AND_NULL(child->data);
			} ଅନ୍ୟଥା{
				child = make_base(child_obj, ଖାଲି);
				ଯଦି (child->children_remaining) {
					/*
					 * Since this child has its own delta children,
					 * we will need this data in the future.
					 * Inflate now so that future iterations will
					 * have access to this object's data while
					 * outside the work mutex.
					 */
					child->data = get_data_from_pack(child_obj);
					child->size = child_obj->size;
				}
			}
		}

		work_lock();
		ଯଦି (parent)
			parent->retain_data--;

		ଯଦି (child && child->data) {
			/*
			 * This child has its own children, so add it to
			 * work_head.
			 */
			list_add(&child->list, &work_head);
			base_cache_used += child->size;
			prune_base_data(ଖାଲି);
		} ଅନ୍ୟଥା ଯଦି (child) {
			/*
			 * This child does not have its own children. It may be
			 * the last descendant of its ancestors; free those
			 * that we can.
			 */
			ସଂରଚନା base_data *p = parent;

			ଯେତେବେଳେ (p) {
				ସଂରଚନା base_data *next_p;

				p->children_remaining--;
				ଯଦି (p->children_remaining)
					ଭାଙ୍ଗ;

				next_p = p->base;
				free_base_data(p);
				list_del(&p->list);
				ମୁକ୍ତ(p);

				p = next_p;
			}
			FREE_AND_NULL(child);
		}
		work_unlock();
	}
	ଫେରନ୍ତୁ ଖାଲି;
}

/*
 * First pass:
 * - find locations of all objects;
 * - calculate SHA1 of all non-delta objects;
 * - remember base (SHA1 or offset) for all deltas.
 */
ସ୍ଥିର ଶୂନ୍ୟ parse_pack_objects(unsigned ଅକ୍ଷର *hash)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, nr_delays = 0;
	ସଂରଚନା ofs_delta_entry *ofs_delta = ofs_deltas;
	ସଂରଚନା object_id ref_delta_oid;
	ସଂରଚନା stat st;
	ସଂରଚନା git_hash_ctx tmp_ctx;

	ଯଦି (verbose)
		progress = start_progress(
				the_repository,
				progress_title ? progress_title :
				from_stdin ? _("Receiving objects") : _("Indexing objects"),
				nr_objects);
	ପାଇଁ (i = 0; i < nr_objects; i++) {
		ସଂରଚନା object_entry *obj = &objects[i];
		ଶୂନ୍ୟ *data = unpack_raw_entry(obj, &ofs_delta->offset,
					      &ref_delta_oid,
					      &obj->idx.oid);
		obj->real_type = obj->type;
		ଯଦି (obj->type == OBJ_OFS_DELTA) {
			nr_ofs_deltas++;
			ofs_delta->obj_no = i;
			ofs_delta++;
		} ଅନ୍ୟଥା ଯଦି (obj->type == OBJ_REF_DELTA) {
			ALLOC_GROW(ref_deltas, nr_ref_deltas + 1, ref_deltas_alloc);
			oidcpy(&ref_deltas[nr_ref_deltas].oid, &ref_delta_oid);
			ref_deltas[nr_ref_deltas].obj_no = i;
			nr_ref_deltas++;
		} ଅନ୍ୟଥା ଯଦି (!data) {
			/* large blobs, check later */
			obj->real_type = OBJ_BAD;
			nr_delays++;
		} ଅନ୍ୟଥା
			sha1_object(data, ଖାଲି, obj->size, obj->type,
				    &obj->idx.oid);
		ମୁକ୍ତ(data);
		display_progress(progress, i+1);
	}
	objects[i].idx.offset = consumed_bytes;
	stop_progress(&progress);

	/* Check pack integrity */
	flush();
	git_hash_init(&tmp_ctx, the_hash_algo);
	git_hash_clone(&tmp_ctx, &ନିବେଶ_ctx);
	git_hash_final(hash, &tmp_ctx);
	ଯଦି (!hasheq(fill(the_hash_algo->rawsz), hash, the_repository->hash_algo))
		die(_("pack is corrupted (SHA1 mismatch)"));
	use(the_hash_algo->rawsz);

	/* If input_fd is a file, we should have reached its end now. */
	ଯଦି (fstat(ନିବେଶ_fd, &st))
		die_errno(_("cannot fstat packfile"));
	ଯଦି (S_ISREG(st.st_mode) &&
			lseek(ନିବେଶ_fd, 0, SEEK_CUR) - ନିବେଶ_len != st.st_size)
		die(_("pack has junk at the end"));

	ପାଇଁ (i = 0; i < nr_objects; i++) {
		ସଂରଚନା object_entry *obj = &objects[i];
		ଯଦି (obj->real_type != OBJ_BAD)
			ଜାରି;
		obj->real_type = obj->type;
		sha1_object(ଖାଲି, obj, obj->size, obj->type,
			    &obj->idx.oid);
		nr_delays--;
	}
	ଯଦି (nr_delays)
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
ସ୍ଥିର ଶୂନ୍ୟ resolve_deltas(ସଂରଚନା pack_idx_option *opts)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

	ଯଦି (!nr_ofs_deltas && !nr_ref_deltas)
		ଫେରନ୍ତୁ;

	/* Sort deltas by base SHA1/offset for fast searching */
	QSORT(ofs_deltas, nr_ofs_deltas, compare_ofs_delta_entry);
	QSORT(ref_deltas, nr_ref_deltas, compare_ref_delta_entry);

	ଯଦି (verbose || show_resolving_progress)
		progress = start_progress(the_repository,
					  _("Resolving deltas"),
					  nr_ref_deltas + nr_ofs_deltas);

	nr_dispatched = 0;
	base_cache_limit = opts->delta_base_cache_limit * nr_threads;
	ଯଦି (nr_threads > 1 || getenv("GIT_FORCE_THREADS")) {
		init_thread();
		ପାଇଁ (i = 0; i < nr_threads; i++) {
			ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = pthread_create(&thread_data[i].thread, ଖାଲି,
						 threaded_second_pass, thread_data + i);
			ଯଦି (ret)
				die(_("unable to create thread: %s"),
				    strerror(ret));
		}
		ପାଇଁ (i = 0; i < nr_threads; i++)
			pthread_join(thread_data[i].thread, ଖାଲି);
		cleanup_thread();
		ଫେରନ୍ତୁ;
	}
	threaded_second_pass(&nothread_data);
}

/*
 * Third pass:
 * - append objects to convert thin pack to full pack if required
 * - write the final pack hash
 */
ସ୍ଥିର ଶୂନ୍ୟ fix_unresolved_deltas(ସଂରଚନା hashfile *f);
ସ୍ଥିର ଶୂନ୍ୟ conclude_pack(ପୂର୍ଣ୍ଣସଂଖ୍ୟା fix_thin_pack, ଅଚଳ ଅକ୍ଷର *curr_pack, unsigned ଅକ୍ଷର *pack_hash)
{
	ଯଦି (nr_ref_deltas + nr_ofs_deltas == nr_resolved_deltas) {
		stop_progress(&progress);
		/* Flush remaining pack final hash. */
		flush();
		ଫେରନ୍ତୁ;
	}

	ଯଦି (fix_thin_pack) {
		ସଂରଚନା hashfile *f;
		unsigned ଅକ୍ଷର read_hash[GIT_MAX_RAWSZ], tail_hash[GIT_MAX_RAWSZ];
		ସଂରଚନା strbuf ସନ୍ଦେଶ = STRBUF_INIT;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା nr_unresolved = nr_ofs_deltas + nr_ref_deltas - nr_resolved_deltas;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା nr_objects_initial = nr_objects;
		ଯଦି (nr_unresolved <= 0)
			die(_("confusion beyond insanity"));
		REALLOC_ARRAY(objects, nr_objects + nr_unresolved + 1);
		memset(objects + nr_objects + 1, 0,
		       nr_unresolved * ଆକାର(*objects));
		f = hashfd(the_repository->hash_algo, output_fd, curr_pack);
		fix_unresolved_deltas(f);
		strbuf_addf(&ସନ୍ଦେଶ, Q_("completed with %d local object",
				     "completed with %d local objects",
				     nr_objects - nr_objects_initial),
			    nr_objects - nr_objects_initial);
		stop_progress_ସନ୍ଦେଶ(&progress, ସନ୍ଦେଶ.buf);
		strbuf_release(&ସନ୍ଦେଶ);
		finalize_hashfile(f, tail_hash, FSYNC_COMPONENT_PACK, 0);
		hashcpy(read_hash, pack_hash, the_repository->hash_algo);
		fixup_pack_header_footer(the_hash_algo, output_fd, pack_hash,
					 curr_pack, nr_objects,
					 read_hash, consumed_bytes-the_hash_algo->rawsz);
		ଯଦି (!hasheq(read_hash, tail_hash, the_repository->hash_algo))
			die(_("Unexpected tail checksum for %s "
			      "(disk corruption?)"), curr_pack);
	}
	ଯଦି (nr_ofs_deltas + nr_ref_deltas != nr_resolved_deltas)
		die(Q_("pack has %d unresolved delta",
		       "pack has %d unresolved deltas",
		       nr_ofs_deltas + nr_ref_deltas - nr_resolved_deltas),
		    nr_ofs_deltas + nr_ref_deltas - nr_resolved_deltas);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା write_compressed(ସଂରଚନା hashfile *f, ଶୂନ୍ୟ *in, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା size)
{
	git_zstream stream;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା status;
	unsigned ଅକ୍ଷର outbuf[4096];
	ସଂରଚନା repo_config_ମୂଲ୍ୟଗୁଡ଼ିକ *cfg = repo_config_ମୂଲ୍ୟଗୁଡ଼ିକ(the_repository);

	git_deflate_init(&stream, cfg->zlib_compression_level);
	stream.next_in = in;
	stream.avail_in = size;

	କର {
		stream.next_out = outbuf;
		stream.avail_out = ଆକାର(outbuf);
		status = git_deflate(&stream, Z_FINISH);
		hashwrite(f, outbuf, ଆକାର(outbuf) - stream.avail_out);
	} ଯେତେବେଳେ (status == Z_OK);

	ଯଦି (status != Z_STREAM_END)
		die(_("unable to deflate appended object (%d)"), status);
	size = stream.total_out;
	git_deflate_end(&stream);
	ଫେରନ୍ତୁ size;
}

ସ୍ଥିର ସଂରଚନା object_entry *append_obj_to_pack(ସଂରଚନା hashfile *f,
			       ଅଚଳ unsigned ଅକ୍ଷର *sha1, ଶୂନ୍ୟ *buf,
			       unsigned long size, ଗଣନା object_type type)
{
	ସଂରଚନା object_entry *obj = &objects[nr_objects++];
	unsigned ଅକ୍ଷର header[10];
	unsigned long s = size;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା n = 0;
	unsigned ଅକ୍ଷର c = (type << 4) | (s & 15);
	s >>= 4;
	ଯେତେବେଳେ (s) {
		header[n++] = c | 0x80;
		c = s & 0x7f;
		s >>= 7;
	}
	header[n++] = c;
	crc32_begin(f);
	hashwrite(f, header, n);
	obj[0].size = size;
	obj[0].hdr_size = n;
	obj[0].type = type;
	obj[0].real_type = type;
	obj[1].idx.offset = obj[0].idx.offset + n;
	obj[1].idx.offset += write_compressed(f, buf, size);
	obj[0].idx.crc32 = crc32_end(f);
	hashflush(f);
	oidread(&obj->idx.oid, sha1, the_repository->hash_algo);
	ଫେରନ୍ତୁ obj;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା delta_pos_compare(ଅଚଳ ଶୂନ୍ୟ *_a, ଅଚଳ ଶୂନ୍ୟ *_b)
{
	ସଂରଚନା ref_delta_entry *a = *(ସଂରଚନା ref_delta_entry **)_a;
	ସଂରଚନା ref_delta_entry *b = *(ସଂରଚନା ref_delta_entry **)_b;
	ଫେରନ୍ତୁ a->obj_no - b->obj_no;
}

ସ୍ଥିର ଶୂନ୍ୟ fix_unresolved_deltas(ସଂରଚନା hashfile *f)
{
	ସଂରଚନା ref_delta_entry **sorted_by_pos;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

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
	ପାଇଁ (i = 0; i < nr_ref_deltas; i++)
		sorted_by_pos[i] = &ref_deltas[i];
	QSORT(sorted_by_pos, nr_ref_deltas, delta_pos_compare);

	ଯଦି (repo_has_promisor_remote(the_repository)) {
		/*
		 * Prefetch the delta bases.
		 */
		ସଂରଚନା oid_array to_fetch = OID_ARRAY_INIT;
		ପାଇଁ (i = 0; i < nr_ref_deltas; i++) {
			ସଂରଚନା ref_delta_entry *d = sorted_by_pos[i];
			ଯଦି (!odb_read_object_info_extended(the_repository->objects,
							   &d->oid, ଖାଲି,
							   OBJECT_INFO_FOR_PREFETCH))
				ଜାରି;
			oid_array_append(&to_fetch, &d->oid);
		}
		promisor_remote_get_direct(the_repository,
					   to_fetch.oid, to_fetch.nr);
		oid_array_clear(&to_fetch);
	}

	ପାଇଁ (i = 0; i < nr_ref_deltas; i++) {
		ସଂରଚନା ref_delta_entry *d = sorted_by_pos[i];
		ଗଣନା object_type type;
		ଶୂନ୍ୟ *data;
		size_t size;

		ଯଦି (objects[d->obj_no].real_type != OBJ_REF_DELTA)
			ଜାରି;
		data = odb_read_object(the_repository->objects, &d->oid,
				       &type, &size);
		ଯଦି (!data)
			ଜାରି;

		ଯଦି (check_object_signature(the_repository, &d->oid, data, size,
					   type) < 0)
			die(_("local object %s is corrupt"), oid_to_hex(&d->oid));

		/*
		 * Add this as an object to the objects array and call
		 * threaded_second_pass() (which will pick up the added
		 * object).
		 */
		append_obj_to_pack(f, d->oid.hash, data, size, type);
		ମୁକ୍ତ(data);
		threaded_second_pass(ଖାଲି);

		display_progress(progress, nr_resolved_deltas);
	}
	ମୁକ୍ତ(sorted_by_pos);
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *derive_filename(ଅଚଳ ଅକ୍ଷର *pack_name, ଅଚଳ ଅକ୍ଷର *strip,
				   ଅଚଳ ଅକ୍ଷର *suffix, ସଂରଚନା strbuf *buf)
{
	size_t len;
	ଯଦି (!strip_suffix(pack_name, strip, &len) || !len ||
	    pack_name[len - 1] != '.')
		die(_("packfile name '%s' does not end with '.%s'"),
		    pack_name, strip);
	strbuf_add(buf, pack_name, len);
	strbuf_addstr(buf, suffix);
	ଫେରନ୍ତୁ buf->buf;
}

ସ୍ଥିର ଶୂନ୍ୟ write_special_file(ଅଚଳ ଅକ୍ଷର *suffix, ଅଚଳ ଅକ୍ଷର *ସନ୍ଦେଶ,
			       ଅଚଳ ଅକ୍ଷର *pack_name, ଅଚଳ unsigned ଅକ୍ଷର *hash,
			       ଅଚଳ ଅକ୍ଷର **report)
{
	ସଂରଚନା strbuf name_buf = STRBUF_INIT;
	ଅଚଳ ଅକ୍ଷର *filename;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ସନ୍ଦେଶ_len = strlen(ସନ୍ଦେଶ);

	ଯଦି (pack_name)
		filename = derive_filename(pack_name, "pack", suffix, &name_buf);
	ଅନ୍ୟଥା
		filename = odb_pack_name(the_repository, &name_buf, hash, suffix);

	fd = safe_create_file_with_leading_directories(the_repository, filename);
	ଯଦି (fd < 0) {
		ଯଦି (errno != EEXIST)
			die_errno(_("cannot write %s file '%s'"),
				  suffix, filename);
	} ଅନ୍ୟଥା {
		ଯଦି (ସନ୍ଦେଶ_len > 0) {
			write_or_die(fd, ସନ୍ଦେଶ, ସନ୍ଦେଶ_len);
			write_or_die(fd, "\n", 1);
		}
		ଯଦି (close(fd) != 0)
			die_errno(_("cannot close written %s file '%s'"),
				  suffix, filename);
		ଯଦି (report)
			*report = suffix;
	}
	strbuf_release(&name_buf);
}

ସ୍ଥିର ଶୂନ୍ୟ rename_tmp_packfile(ଅଚଳ ଅକ୍ଷର **final_name,
				ଅଚଳ ଅକ୍ଷର *curr_name,
				ସଂରଚନା strbuf *name, unsigned ଅକ୍ଷର *hash,
				ଅଚଳ ଅକ୍ଷର *ext, ପୂର୍ଣ୍ଣସଂଖ୍ୟା make_read_only_if_same)
{
	ଯଦି (!*final_name || strcmp(*final_name, curr_name)) {
		ଯଦି (!*final_name)
			*final_name = odb_pack_name(the_repository, name, hash, ext);
		ଯଦି (finalize_object_file(the_repository, curr_name, *final_name))
			die(_("unable to rename temporary '*.%s' file to '%s'"),
			    ext, *final_name);
	} ଅନ୍ୟଥା ଯଦି (make_read_only_if_same) {
		chmod(*final_name, 0444);
	}
}

ସ୍ଥିର ଶୂନ୍ୟ final(ଅଚଳ ଅକ୍ଷର *final_pack_name, ଅଚଳ ଅକ୍ଷର *curr_pack_name,
		  ଅଚଳ ଅକ୍ଷର *final_index_name, ଅଚଳ ଅକ୍ଷର *curr_index_name,
		  ଅଚଳ ଅକ୍ଷର *final_rev_index_name, ଅଚଳ ଅକ୍ଷର *curr_rev_index_name,
		  ଅଚଳ ଅକ୍ଷର *keep_ସନ୍ଦେଶ, ଅଚଳ ଅକ୍ଷର *promisor_ସନ୍ଦେଶ,
		  unsigned ଅକ୍ଷର *hash)
{
	ଅଚଳ ଅକ୍ଷର *report = "pack";
	ସଂରଚନା strbuf pack_name = STRBUF_INIT;
	ସଂରଚନା strbuf index_name = STRBUF_INIT;
	ସଂରଚନା strbuf rev_index_name = STRBUF_INIT;

	ଯଦି (!from_stdin) {
		close(ନିବେଶ_fd);
	} ଅନ୍ୟଥା {
		fsync_component_or_die(FSYNC_COMPONENT_PACK, output_fd, curr_pack_name);
		ଯଦି (close(output_fd))
			die_errno(_("error while closing pack file"));
	}

	ଯଦି (keep_ସନ୍ଦେଶ)
		write_special_file("keep", keep_ସନ୍ଦେଶ, final_pack_name, hash,
				   &report);
	ଯଦି (promisor_ସନ୍ଦେଶ)
		write_special_file("promisor", promisor_ସନ୍ଦେଶ, final_pack_name,
				   hash, ଖାଲି);

	rename_tmp_packfile(&final_pack_name, curr_pack_name, &pack_name,
			    hash, "pack", from_stdin);
	ଯଦି (curr_rev_index_name)
		rename_tmp_packfile(&final_rev_index_name, curr_rev_index_name,
				    &rev_index_name, hash, "rev", 1);
	rename_tmp_packfile(&final_index_name, curr_index_name, &index_name,
			    hash, "idx", 1);

	ଯଦି (do_fsck_object && startup_info->have_repository) {
		ସଂରଚନା odb_source_files *files =
			odb_source_files_downcast(the_repository->objects->source);
		packfile_store_load_pack(files->dirs->packed, final_index_name, 0);
	}

	ଯଦି (!from_stdin) {
		printf("%s\n", hash_to_hex(hash));
	} ଅନ୍ୟଥା {
		ସଂରଚନା strbuf buf = STRBUF_INIT;

		strbuf_addf(&buf, "%s\t%s\n", report, hash_to_hex(hash));
		write_or_die(1, buf.buf, buf.len);
		strbuf_release(&buf);

		/* Write the last part of the buffer to stdout */
		write_in_full(1, ନିବେଶ_buffer + ନିବେଶ_offset, ନିବେଶ_len);
	}

	strbuf_release(&rev_index_name);
	strbuf_release(&index_name);
	strbuf_release(&pack_name);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା git_index_pack_config(ଅଚଳ ଅକ୍ଷର *k, ଅଚଳ ଅକ୍ଷର *v,
				 ଅଚଳ ସଂରଚନା config_context *ctx, ଶୂନ୍ୟ *cb)
{
	ସଂରଚନା pack_idx_option *opts = cb;

	ଯଦି (!strcmp(k, "pack.indexversion")) {
		opts->version = git_config_int(k, v, ctx->kvi);
		ଯଦି (opts->version > 2)
			die(_("bad pack.indexVersion=%"PRIu32), opts->version);
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (!strcmp(k, "pack.threads")) {
		nr_threads = git_config_int(k, v, ctx->kvi);
		ଯଦି (nr_threads < 0)
			die(_("invalid number of threads specified (%d)"),
			    nr_threads);
		ଯଦି (!HAVE_THREADS && nr_threads != 1) {
			warning(_("no threads support, ignoring %s"), k);
			nr_threads = 1;
		}
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (!strcmp(k, "pack.writereverseindex")) {
		ଯଦି (git_config_bool(k, v))
			opts->flags |= WRITE_REV;
		ଅନ୍ୟଥା
			opts->flags &= ~WRITE_REV;
	}
	ଯଦି (!strcmp(k, "core.deltabasecachelimit")) {
		opts->delta_base_cache_limit = git_config_ulong(k, v, ctx->kvi);
		ଫେରନ୍ତୁ 0;
	}
	ଫେରନ୍ତୁ git_default_config(k, v, ctx, cb);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp_uint32(ଅଚଳ ଶୂନ୍ୟ *a_, ଅଚଳ ଶୂନ୍ୟ *b_)
{
	uint32_t a = *((uint32_t *)a_);
	uint32_t b = *((uint32_t *)b_);

	ଫେରନ୍ତୁ (a < b) ? -1 : (a != b);
}

ସ୍ଥିର ଶୂନ୍ୟ read_v2_anomalous_offsets(ସଂରଚନା packed_git *p,
				      ସଂରଚନା pack_idx_option *opts)
{
	ଅଚଳ uint32_t *idx1, *idx2;
	uint32_t i;

	/* The address of the 4-byte offset table */
	idx1 = (((ଅଚଳ uint32_t *)((ଅଚଳ uint8_t *)p->index_data + p->crc_offset))
		+ (size_t)p->ଅଙ୍କ_objects /* CRC32 table */
		);

	/* The address of the 8-byte offset table */
	idx2 = idx1 + p->ଅଙ୍କ_objects;

	ପାଇଁ (i = 0; i < p->ଅଙ୍କ_objects; i++) {
		uint32_t off = ntohl(idx1[i]);
		ଯଦି (!(off & 0x80000000))
			ଜାରି;
		off = off & 0x7fffffff;
		check_pack_index_ptr(p, &idx2[off * 2]);
		ଯଦି (idx2[off * 2])
			ଜାରି;
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

ସ୍ଥିର ଶୂନ୍ୟ read_idx_option(ସଂରଚନା pack_idx_option *opts, ଅଚଳ ଅକ୍ଷର *pack_name)
{
	ସଂରଚନା packed_git *p = add_packed_git(the_repository, pack_name,
					      strlen(pack_name), 1);

	ଯଦି (!p)
		die(_("Cannot open existing pack file '%s'"), pack_name);
	ଯଦି (open_pack_index(p))
		die(_("Cannot open existing pack idx file for '%s'"), pack_name);

	/* Read the attributes from the existing idx file */
	opts->version = p->index_version;

	ଯଦି (opts->version == 2)
		read_v2_anomalous_offsets(p, opts);

	/*
	 * Get rid of the idx file as we do not need it anymore.
	 * NEEDSWORK: extract this bit from free_pack_by_name() in
	 * object-file.c, perhaps?  It shouldn't matter very much as we
	 * know we haven't installed this pack (hence we never have
	 * read anything from it).
	 */
	close_pack_index(p);
	ମୁକ୍ତ(p);
}

ସ୍ଥିର ଶୂନ୍ୟ show_pack_info(ପୂର୍ଣ୍ଣସଂଖ୍ୟା stat_only)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, baseobjects = nr_objects - nr_ref_deltas - nr_ofs_deltas;
	unsigned long *chain_histogram = ଖାଲି;

	ଯଦି (deepest_delta)
		CALLOC_ARRAY(chain_histogram, deepest_delta);

	ପାଇଁ (i = 0; i < nr_objects; i++) {
		ସଂରଚନା object_entry *obj = &objects[i];

		ଯଦି (is_delta_type(obj->type))
			chain_histogram[obj_stat[i].delta_depth - 1]++;
		ଯଦି (stat_only)
			ଜାରି;
		printf("%s %-6s %"PRIuMAX" %"PRIuMAX" %"PRIuMAX,
		       oid_to_hex(&obj->idx.oid),
		       type_name(obj->real_type), (uintmax_t)obj->size,
		       (uintmax_t)(obj[1].idx.offset - obj->idx.offset),
		       (uintmax_t)obj->idx.offset);
		ଯଦି (is_delta_type(obj->type)) {
			ସଂରଚନା object_entry *bobj = &objects[obj_stat[i].base_object_no];
			printf(" %u %s", obj_stat[i].delta_depth,
			       oid_to_hex(&bobj->idx.oid));
		}
		putchar('\n');
	}

	ଯଦି (baseobjects)
		printf_ln(Q_("non delta: %d object",
			     "non delta: %d objects",
			     baseobjects),
			  baseobjects);
	ପାଇଁ (i = 0; i < deepest_delta; i++) {
		ଯଦି (!chain_histogram[i])
			ଜାରି;
		printf_ln(Q_("chain length = %d: %lu object",
			     "chain length = %d: %lu objects",
			     chain_histogram[i]),
			  i + 1,
			  chain_histogram[i]);
	}
	ମୁକ୍ତ(chain_histogram);
}

ସ୍ଥିର ଶୂନ୍ୟ repack_local_links(ଶୂନ୍ୟ)
{
	ସଂରଚନା child_process cmd = CHILD_PROCESS_INIT;
	FILE *out;
	ସଂରଚନା strbuf ଧାଡ଼ି = STRBUF_INIT;
	ସଂରଚନା oidset_iter iter;
	ସଂରଚନା object_id *oid;
	ଅକ୍ଷର *base_name = ଖାଲି;

	ଯଦି (!oidset_size(&outgoing_links))
		ଫେରନ୍ତୁ;

	oidset_iter_init(&outgoing_links, &iter);
	ଯେତେବେଳେ ((oid = oidset_iter_next(&iter))) {
		ସଂରଚନା odb_source_info source_info;
		ସଂରଚନା object_info info = {
			.source_infop = &source_info,
		};

		ଯଦି (odb_read_object_info_extended(the_repository->objects, oid, &info, 0))
			/* Missing; assume it is a promisor object */
			ଜାରି;
		ଯଦି (source_info.source->type == ODB_SOURCE_PACKED &&
		    source_info.u.packed.pack->pack_promisor)
			ଜାରି;

		ଯଦି (!cmd.args.nr) {
			base_name = mkpathdup(
				"%s/pack/pack",
				repo_get_object_directory(the_repository));
			strvec_ଠେଲ(&cmd.args, "pack-objects");
			strvec_ଠେଲ(&cmd.args,
				    "--exclude-promisor-objects-best-effort");
			strvec_ଠେଲ(&cmd.args, base_name);
			cmd.git_cmd = 1;
			cmd.in = -1;
			cmd.out = -1;
			ଯଦି (start_command(&cmd))
				die(_("could not start pack-objects to repack local links"));
		}

		ଯଦି (write_in_full(cmd.in, oid_to_hex(oid), the_hash_algo->hexsz) < 0 ||
		    write_in_full(cmd.in, "\n", 1) < 0)
			die(_("failed to feed local object to pack-objects"));
	}

	ଯଦି (!cmd.args.nr)
		ଫେରନ୍ତୁ;

	close(cmd.in);

	out = xfdopen(cmd.out, "r");
	ଯେତେବେଳେ (strbuf_getline_lf(&ଧାଡ଼ି, out) != EOF) {
		unsigned ଅକ୍ଷର ଦ୍ୱିମିକ[GIT_MAX_RAWSZ];
		ଯଦି (ଧାଡ଼ି.len != the_hash_algo->hexsz ||
		    !hex_to_bytes(ଦ୍ୱିମିକ, ଧାଡ଼ି.buf, ଧାଡ଼ି.len))
			die(_("index-pack: Expecting full hex object ID lines only from pack-objects."));

		/*
		 * pack-objects creates the .pack and .idx files, but not the
		 * .promisor file. Create the .promisor file, which is empty.
		 */
		write_special_file("promisor", "", ଖାଲି, ଦ୍ୱିମିକ, ଖାଲି);
	}

	fclose(out);
	ଯଦି (finish_command(&cmd))
		die(_("could not finish pack-objects to repack local links"));
	strbuf_release(&ଧାଡ଼ି);
	ମୁକ୍ତ(base_name);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_index_pack(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
		   ଅଚଳ ଅକ୍ଷର **argv,
		   ଅଚଳ ଅକ୍ଷର *prefix,
		   ସଂରଚନା repository *repo)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, fix_thin_pack = 0, verify = 0, stat_only = 0, rev_index;
	ଅଚଳ ଅକ୍ଷର *curr_index;
	ଅକ୍ଷର *curr_rev_index = ଖାଲି;
	ଅଚଳ ଅକ୍ଷର *index_name = ଖାଲି, *pack_name = ଖାଲି, *rev_index_name = ଖାଲି;
	ଅଚଳ ଅକ୍ଷର *keep_ସନ୍ଦେଶ = ଖାଲି;
	ଅଚଳ ଅକ୍ଷର *promisor_ସନ୍ଦେଶ = ଖାଲି;
	ସଂରଚନା strbuf index_name_buf = STRBUF_INIT;
	ସଂରଚନା strbuf rev_index_name_buf = STRBUF_INIT;
	ସଂରଚନା pack_idx_entry **idx_objects;
	ସଂରଚନା pack_idx_option opts;
	unsigned ଅକ୍ଷର pack_hash[GIT_MAX_RAWSZ];
	unsigned foreign_nr = 1;	/* zero is a "good" value, assume bad */
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା report_end_of_ନିବେଶ = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା hash_algo = 0;

	show_usage_if_asked(argc, argv, index_pack_usage);

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
	repo_config(the_repository, git_index_pack_config, &opts);
	ଯଦି (prefix && chdir(prefix))
		die(_("Cannot come back to cwd"));

	ଯଦି (git_env_bool(GIT_TEST_NO_WRITE_REV_INDEX, 0))
		rev_index = 0;
	ଅନ୍ୟଥା
		rev_index = !!(opts.flags & (WRITE_REV_VERIFY | WRITE_REV));

	ପାଇଁ (i = 1; i < argc; i++) {
		ଅଚଳ ଅକ୍ଷର *arg = argv[i];

		ଯଦି (*arg == '-') {
			ଯଦି (!strcmp(arg, "--stdin")) {
				from_stdin = 1;
			} ଅନ୍ୟଥା ଯଦି (!strcmp(arg, "--fix-thin")) {
				fix_thin_pack = 1;
			} ଅନ୍ୟଥା ଯଦି (skip_to_optional_arg(arg, "--strict", &arg)) {
				strict = 1;
				do_fsck_object = 1;
				fsck_set_ସନ୍ଦେଶ_types(&fsck_options, arg);
			} ଅନ୍ୟଥା ଯଦି (!strcmp(arg, "--check-self-contained-and-connected")) {
				strict = 1;
				check_self_contained_and_connected = 1;
			} ଅନ୍ୟଥା ଯଦି (skip_to_optional_arg(arg, "--fsck-objects", &arg)) {
				do_fsck_object = 1;
				fsck_set_ସନ୍ଦେଶ_types(&fsck_options, arg);
			} ଅନ୍ୟଥା ଯଦି (!strcmp(arg, "--verify")) {
				verify = 1;
			} ଅନ୍ୟଥା ଯଦି (!strcmp(arg, "--verify-stat")) {
				verify = 1;
				show_stat = 1;
			} ଅନ୍ୟଥା ଯଦି (!strcmp(arg, "--verify-stat-only")) {
				verify = 1;
				show_stat = 1;
				stat_only = 1;
			} ଅନ୍ୟଥା ଯଦି (skip_to_optional_arg(arg, "--keep", &keep_ସନ୍ଦେଶ)) {
				; /* nothing to do */
			} ଅନ୍ୟଥା ଯଦି (skip_to_optional_arg(arg, "--promisor", &promisor_ସନ୍ଦେଶ)) {
				record_outgoing_links = 1;
			} ଅନ୍ୟଥା ଯଦି (starts_with(arg, "--threads=")) {
				ଅକ୍ଷର *end;
				nr_threads = strtoul(arg+10, &end, 0);
				ଯଦି (!arg[10] || *end || nr_threads < 0)
					usage(index_pack_usage);
				ଯଦି (!HAVE_THREADS && nr_threads != 1) {
					warning(_("no threads support, ignoring %s"), arg);
					nr_threads = 1;
				}
			} ଅନ୍ୟଥା ଯଦି (skip_prefix(arg, "--pack_header=", &arg)) {
				ଯଦି (parse_pack_header_option(arg,
							     ନିବେଶ_buffer,
							     &ନିବେଶ_len) < 0)
					die(_("bad --pack_header: %s"), arg);
			} ଅନ୍ୟଥା ଯଦି (!strcmp(arg, "-v")) {
				verbose = 1;
			} ଅନ୍ୟଥା ଯଦି (!strcmp(arg, "--progress-title")) {
				ଯଦି (progress_title || (i+1) >= argc)
					usage(index_pack_usage);
				progress_title = argv[++i];
			} ଅନ୍ୟଥା ଯଦି (!strcmp(arg, "--show-resolving-progress")) {
				show_resolving_progress = 1;
			} ଅନ୍ୟଥା ଯଦି (!strcmp(arg, "--report-end-of-input")) {
				report_end_of_ନିବେଶ = 1;
			} ଅନ୍ୟଥା ଯଦି (!strcmp(arg, "-o")) {
				ଯଦି (index_name || (i+1) >= argc)
					usage(index_pack_usage);
				index_name = argv[++i];
			} ଅନ୍ୟଥା ଯଦି (starts_with(arg, "--index-version=")) {
				ଅକ୍ଷର *c;
				opts.version = strtoul(arg + 16, &c, 10);
				ଯଦି (opts.version > 2)
					die(_("bad %s"), arg);
				ଯଦି (*c == ',')
					opts.off32_limit = strtoul(c+1, &c, 0);
				ଯଦି (*c || opts.off32_limit & 0x80000000)
					die(_("bad %s"), arg);
			} ଅନ୍ୟଥା ଯଦି (skip_prefix(arg, "--max-input-size=", &arg)) {
				max_ନିବେଶ_size = strtoumax(arg, ଖାଲି, 10);
			} ଅନ୍ୟଥା ଯଦି (skip_prefix(arg, "--object-format=", &arg)) {
				hash_algo = hash_algo_by_name(arg);
				ଯଦି (hash_algo == GIT_HASH_UNKNOWN)
					die(_("unknown hash algorithm '%s'"), arg);
				repo_set_hash_algo(the_repository, hash_algo);
			} ଅନ୍ୟଥା ଯଦି (!strcmp(arg, "--rev-index")) {
				rev_index = 1;
			} ଅନ୍ୟଥା ଯଦି (!strcmp(arg, "--no-rev-index")) {
				rev_index = 0;
			} ଅନ୍ୟଥା
				usage(index_pack_usage);
			ଜାରି;
		}

		ଯଦି (pack_name)
			usage(index_pack_usage);
		pack_name = arg;
	}

	ଯଦି (!pack_name && !from_stdin)
		usage(index_pack_usage);
	ଯଦି (fix_thin_pack && !from_stdin)
		die(_("the option '%s' requires '%s'"), "--fix-thin", "--stdin");
	ଯଦି (promisor_ସନ୍ଦେଶ && pack_name)
		die(_("--promisor cannot be used with a pack name"));
	ଯଦି (from_stdin && !startup_info->have_repository)
		die(_("--stdin requires a git repository"));
	ଯଦି (from_stdin && hash_algo)
		die(_("options '%s' and '%s' cannot be used together"), "--object-format", "--stdin");
	ଯଦି (!index_name && pack_name)
		index_name = derive_filename(pack_name, "pack", "idx", &index_name_buf);

	/*
	 * Packfiles and indices do not carry enough information to be able to
	 * identify their object hash. So when we are neither in a repository
	 * nor has the user told us which object hash to use we have no other
	 * choice but to guess the object hash.
	 */
	ଯଦି (!the_repository->hash_algo)
		repo_set_hash_algo(the_repository, GIT_HASH_DEFAULT);

	opts.flags &= ~(WRITE_REV | WRITE_REV_VERIFY);
	ଯଦି (rev_index) {
		opts.flags |= verify ? WRITE_REV_VERIFY : WRITE_REV;
		ଯଦି (index_name)
			rev_index_name = derive_filename(index_name,
							 "idx", "rev",
							 &rev_index_name_buf);
	}

	ଯଦି (verify) {
		ଯଦି (!index_name)
			die(_("--verify with no packfile name given"));
		read_idx_option(&opts, index_name);
		opts.flags |= WRITE_IDX_VERIFY | WRITE_IDX_STRICT;
	}
	ଯଦି (strict)
		opts.flags |= WRITE_IDX_STRICT;

	ଯଦି (HAVE_THREADS && !nr_threads) {
		nr_threads = online_cpus();
		/*
		 * Experiments show that going above 20 threads doesn't help,
		 * no matter how many cores you have. Below that, we tend to
		 * max at half the number of online_cpus(), presumably because
		 * half of those are hyperthreads rather than full cores. We'll
		 * never reduce the level below "3", though, to match a
		 * historical value that nobody complained about.
		 */
		ଯଦି (nr_threads < 4)
			; /* too few cores to consider capping */
		ଅନ୍ୟଥା ଯଦି (nr_threads < 6)
			nr_threads = 3; /* historic cap */
		ଅନ୍ୟଥା ଯଦି (nr_threads < 40)
			nr_threads /= 2;
		ଅନ୍ୟଥା
			nr_threads = 20; /* hard cap */
	}

	curr_pack = open_pack_file(pack_name);
	parse_pack_header();
	CALLOC_ARRAY(objects, st_add(nr_objects, 1));
	ଯଦି (show_stat)
		CALLOC_ARRAY(obj_stat, st_add(nr_objects, 1));
	CALLOC_ARRAY(ofs_deltas, nr_objects);
	parse_pack_objects(pack_hash);
	ଯଦି (report_end_of_ନିବେଶ)
		write_in_full(2, "\0", 1);
	resolve_deltas(&opts);
	conclude_pack(fix_thin_pack, curr_pack, pack_hash);
	ମୁକ୍ତ(ofs_deltas);
	ମୁକ୍ତ(ref_deltas);
	ଯଦି (strict)
		foreign_nr = check_objects();

	ଯଦି (show_stat)
		show_pack_info(stat_only);

	ALLOC_ARRAY(idx_objects, nr_objects);
	ପାଇଁ (i = 0; i < nr_objects; i++)
		idx_objects[i] = &objects[i].idx;
	curr_index = write_idx_file(the_repository, index_name, idx_objects,
				    nr_objects, &opts, pack_hash);
	ଯଦି (rev_index)
		curr_rev_index = write_rev_file(the_repository, rev_index_name,
						idx_objects, nr_objects,
						pack_hash, opts.flags);
	ମୁକ୍ତ(idx_objects);

	ଯଦି (!verify)
		final(pack_name, curr_pack,
		      index_name, curr_index,
		      rev_index_name, curr_rev_index,
		      keep_ସନ୍ଦେଶ, promisor_ସନ୍ଦେଶ,
		      pack_hash);
	ଅନ୍ୟଥା
		close(ନିବେଶ_fd);

	ଯଦି (do_fsck_object) {
		/*
		 * We cannot perform queued consistency checks when running
		 * outside of a repository because those require us to read
		 * from the object database, which is uninitialized.
		 *
		 * TODO: we may eventually set up an in-memory object database,
		 * which would allow us to perform these queued checks.
		 */
		ଯଦି (!startup_info->have_repository &&
		    fsck_has_queued_checks(&fsck_options))
			die(_("cannot perform queued object checks outside "
			      "of a repository"));

		ଯଦି (fsck_finish(&fsck_options))
			die(_("fsck error in pack objects"));
	}

	ମୁକ୍ତ(opts.anomaly);
	ମୁକ୍ତ(objects);
	strbuf_release(&index_name_buf);
	strbuf_release(&rev_index_name_buf);
	ଯଦି (!pack_name)
		ମୁକ୍ତ((ଶୂନ୍ୟ *) curr_pack);
	ଯଦି (!index_name)
		ମୁକ୍ତ((ଶୂନ୍ୟ *) curr_index);
	ମୁକ୍ତ(curr_rev_index);

	repack_local_links();

	/*
	 * Let the caller know this pack is not self contained
	 */
	ଯଦି (check_self_contained_and_connected && foreign_nr)
		ଫେରନ୍ତୁ 1;

	ଫେରନ୍ତୁ 0;
}
