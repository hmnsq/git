#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#include "builtin.h"
#include "config.h"
#include "delta.h"
#include "environment.h"
#include "gettext.h"
#include "hex.h"
#include "pack.h"
#include "csum-file.h"
#include "blob.h"
#include "commit.h"
#include "tag.h"
#include "tree.h"
#include "progress.h"
#include "fsck.h"
#include "strbuf.h"
#include "thread-utils.h"
#include "packfile.h"
#include "pack-revindex.h"
#include "object-file.h"
#include "odb.h"
#include "odb/streaming.h"
#include "oid-array.h"
#include "oidset.h"
#include "path.h"
#include "replace-object.h"
#include "tree-walk.h"
#include "promisor-remote.h"
#include "run-command.h"
#include "setup.h"
#include "strvec.h"

static const char index_pack_usage[] =
"git index-pack [-v] [-o <index-file>] [--keep | --keep=<msg>] [--[no-]rev-index] [--verify] [--strict[=<msg-id>=<severity>...]] [--fsck-objects[=<msg-id>=<severity>...]] (<pack-file> | --stdin [--fix-thin] [<pack-file>])";

struct object_entry {
	struct pack_idx_entry idx;
	size_t size;
	unsigned char hdr_size;
	signed char type;
	signed char real_type;
};

struct object_stat {
	unsigned delta_depth;
	ціле base_object_no;
};

struct base_data {
	/* Initialized by make_base(). */
	struct base_data *base;
	struct object_entry *obj;
	ціле ref_first, ref_last;
	ціле ofs_first, ofs_last;
	/*
	 * Threads should increment retain_data if they are about to call
	 * patch_delta() using this struct's data as a base, and decrement this
	 * when they are done. While retain_data is nonzero, this struct's data
	 * will not be freed even if the delta base cache limit is exceeded.
	 */
	ціле retain_data;
	/*
	 * The number of direct children that have not been fully processed
	 * (entered work_head, entered done_head, left done_head). When this
	 * number reaches zero, this struct base_data can be freed.
	 */
	ціле children_remaining;

	/* Not initialized by make_base(). */
	struct list_head list;
	void *data;
	size_t size;
};

/*
 * Stack of struct base_data that have unprocessed children.
 * threaded_second_pass() uses this as a source of work (the other being the
 * objects array).
 *
 * Guarded by work_mutex.
 */
static LIST_HEAD(work_head);

/*
 * Stack of struct base_data that have children, all of whom have been
 * processed or are being processed, and at least one child is being processed.
 * These struct base_data must be kept around until the last child is
 * processed.
 *
 * Guarded by work_mutex.
 */
static LIST_HEAD(done_head);

/*
 * All threads share one delta base cache.
 *
 * base_cache_used is guarded by work_mutex, and base_cache_limit is read-only
 * in a thread.
 */
static size_t base_cache_used;
static size_t base_cache_limit;

struct thread_local_data {
	pthread_t thread;
	ціле pack_fd;
};

/* Remember to update object flag allocation in object.h */
#define FLAG_LINK (1u<<20)
#define FLAG_CHECKED (1u<<21)

struct ofs_delta_entry {
	off_t offset;
	ціле obj_no;
};

struct ref_delta_entry {
	struct object_id oid;
	ціле obj_no;
};

static struct object_entry *objects;
static struct object_stat *obj_stat;
static struct ofs_delta_entry *ofs_deltas;
static struct ref_delta_entry *ref_deltas;
static struct thread_local_data nothread_data;
static ціле nr_objects;
static ціле nr_ofs_deltas;
static ціле nr_ref_deltas;
static ціле ref_deltas_alloc;
static ціле nr_resolved_deltas;
static ціле nr_threads;

static ціле from_stdin;
static ціле strict;
static ціле do_fsck_object;
static struct fsck_options fsck_options;
static ціле verbose;
static const char *progress_title;
static ціле show_resolving_progress;
static ціле show_stat;
static ціле check_self_contained_and_connected;

static struct progress *progress;

static unsigned char input_buffer[DEFAULT_IO_BUFFER_SIZE];
static unsigned ціле input_offset, input_len;
static off_t consumed_bytes;
static off_t max_input_size;
static unsigned deepest_delta;
static struct git_hash_ctx input_ctx;
static uint32_t input_crc32;
static ціле input_fd, output_fd;
static const char *curr_pack;

/*
 * outgoing_links is guarded by read_mutex, and record_outgoing_links is
 * read-only in a thread.
 */
static struct oidset outgoing_links = OIDSET_INIT;
static ціле record_outgoing_links;

static struct thread_local_data *thread_data;
static ціле nr_dispatched;
static ціле threads_active;

static pthread_mutex_t read_mutex;
#define read_lock()		lock_mutex(&read_mutex)
#define read_unlock()		unlock_mutex(&read_mutex)

static pthread_mutex_t counter_mutex;
#define counter_lock()		lock_mutex(&counter_mutex)
#define counter_unlock()	unlock_mutex(&counter_mutex)

static pthread_mutex_t work_mutex;
#define work_lock()		lock_mutex(&work_mutex)
#define work_unlock()		unlock_mutex(&work_mutex)

static pthread_mutex_t deepest_delta_mutex;
#define deepest_delta_lock()	lock_mutex(&deepest_delta_mutex)
#define deepest_delta_unlock()	unlock_mutex(&deepest_delta_mutex)

static pthread_key_t key;

static inline void lock_mutex(pthread_mutex_t *mutex)
{
	якщо (threads_active)
		pthread_mutex_lock(mutex);
}

static inline void unlock_mutex(pthread_mutex_t *mutex)
{
	якщо (threads_active)
		pthread_mutex_unlock(mutex);
}

/*
 * Mutex and conditional variable can't be statically-initialized on Windows.
 */
static void init_thread(void)
{
	ціле i;
	init_recursive_mutex(&read_mutex);
	pthread_mutex_init(&counter_mutex, NULL);
	pthread_mutex_init(&work_mutex, NULL);
	якщо (show_stat)
		pthread_mutex_init(&deepest_delta_mutex, NULL);
	pthread_key_create(&key, NULL);
	CALLOC_ARRAY(thread_data, nr_threads);
	для (i = 0; i < nr_threads; i++) {
		thread_data[i].pack_fd = xopen(curr_pack, O_RDONLY);
	}

	threads_active = 1;
}

static void cleanup_thread(void)
{
	ціле i;
	якщо (!threads_active)
		повернути;
	threads_active = 0;
	pthread_mutex_destroy(&read_mutex);
	pthread_mutex_destroy(&counter_mutex);
	pthread_mutex_destroy(&work_mutex);
	якщо (show_stat)
		pthread_mutex_destroy(&deepest_delta_mutex);
	для (i = 0; i < nr_threads; i++)
		close(thread_data[i].pack_fd);
	pthread_key_delete(key);
	free(thread_data);
}

static ціле mark_link(struct object *obj, enum object_type type,
		     void *data UNUSED,
		     struct fsck_options *options UNUSED)
{
	якщо (!obj)
		повернути -1;

	якщо (type != OBJ_ANY && obj->type != type)
		die(_("object type mismatch at %s"), oid_to_hex(&obj->oid));

	obj->flags |= FLAG_LINK;
	повернути 0;
}

/* The content of each linked object must have been checked
   or it must be already present in the object database */
static unsigned check_object(struct object *obj)
{
	якщо (!obj)
		повернути 0;

	якщо (!(obj->flags & FLAG_LINK))
		повернути 0;

	якщо (!(obj->flags & FLAG_CHECKED)) {
		size_t size;
		ціле type = odb_read_object_info(the_repository->objects,
						&obj->oid, &size);
		якщо (type <= 0)
			die(_("did not receive expected object %s"),
			      oid_to_hex(&obj->oid));
		якщо (type != obj->type)
			die(_("object %s: expected type %s, found %s"),
			    oid_to_hex(&obj->oid),
			    type_name(obj->type), type_name(type));
		obj->flags |= FLAG_CHECKED;
		повернути 1;
	}

	повернути 0;
}

static unsigned check_objects(void)
{
	unsigned i, max, foreign_nr = 0;

	max = get_max_object_index(the_repository);

	якщо (verbose)
		progress = start_delayed_progress(the_repository,
						  _("Checking objects"), max);

	для (i = 0; i < max; i++) {
		foreign_nr += check_object(get_indexed_object(the_repository, i));
		display_progress(progress, i + 1);
	}

	stop_progress(&progress);
	повернути foreign_nr;
}


/* Discard current buffer used content. */
static void flush(void)
{
	якщо (input_offset) {
		якщо (output_fd >= 0)
			write_or_die(output_fd, input_buffer, input_offset);
		git_hash_update(&input_ctx, input_buffer, input_offset);
		memmove(input_buffer, input_buffer + input_offset, input_len);
		input_offset = 0;
	}
}

/*
 * Make sure at least "min" bytes are available in the buffer, and
 * return the pointer to the buffer.
 */
static void *fill(ціле min)
{
	якщо (min <= input_len)
		повернути input_buffer + input_offset;
	якщо (min > sizeof(input_buffer))
		die(Q_("cannot fill %d byte",
		       "cannot fill %d bytes",
		       min),
		    min);
	flush();
	do {
		ssize_t ret = xread(input_fd, input_buffer + input_len,
				sizeof(input_buffer) - input_len);
		якщо (ret <= 0) {
			якщо (!ret)
				die(_("early EOF"));
			die_errno(_("read error on input"));
		}
		input_len += ret;
		якщо (from_stdin)
			display_throughput(progress, consumed_bytes + input_len);
	} поки (input_len < min);
	повернути input_buffer;
}

static void use(ціле bytes)
{
	якщо (bytes > input_len)
		die(_("used more bytes than were available"));
	input_crc32 = crc32(input_crc32, input_buffer + input_offset, bytes);
	input_len -= bytes;
	input_offset += bytes;

	/* make sure off_t is sufficiently large not to wrap */
	якщо (signed_add_overflows(consumed_bytes, bytes))
		die(_("pack too large for current definition of off_t"));
	consumed_bytes += bytes;
	якщо (max_input_size && consumed_bytes > max_input_size) {
		struct strbuf size_limit = STRBUF_INIT;
		strbuf_humanise_bytes(&size_limit, max_input_size);
		die(_("pack exceeds maximum allowed size (%s)"),
		    size_limit.buf);
	}
}

static const char *open_pack_file(const char *pack_name)
{
	якщо (from_stdin) {
		input_fd = 0;
		якщо (!pack_name) {
			struct strbuf tmp_file = STRBUF_INIT;
			output_fd = odb_mkstemp(the_repository->objects, &tmp_file,
						"pack/tmp_pack_XXXXXX");
			pack_name = strbuf_detach(&tmp_file, NULL);
		} інакше {
			output_fd = xopen(pack_name, O_CREAT|O_EXCL|O_RDWR, 0600);
		}
		nothread_data.pack_fd = output_fd;
	} інакше {
		input_fd = xopen(pack_name, O_RDONLY);
		output_fd = -1;
		nothread_data.pack_fd = input_fd;
	}
	git_hash_init(&input_ctx, the_hash_algo);
	повернути pack_name;
}

static void parse_pack_header(void)
{
	unsigned char *hdr = fill(sizeof(struct pack_header));

	/* Header consistency check */
	якщо (get_be32(hdr) != PACK_SIGNATURE)
		die(_("pack signature mismatch"));
	hdr += 4;
	якщо (!pack_version_ok_native(get_be32(hdr)))
		die(_("pack version %"PRIu32" unsupported"),
		    get_be32(hdr));
	hdr += 4;

	nr_objects = get_be32(hdr);
	use(sizeof(struct pack_header));
}

__attribute__((format (printf, 2, 3)))
static NORETURN void bad_object(off_t offset, const char *format, ...)
{
	va_list params;
	char buf[1024];

	va_start(params, format);
	vsnprintf(buf, sizeof(buf), format, params);
	va_end(params);
	die(_("pack has bad object at offset %"PRIuMAX": %s"),
	    (uintmax_t)offset, buf);
}

static inline struct thread_local_data *get_thread_data(void)
{
	якщо (HAVE_THREADS) {
		якщо (threads_active)
			повернути pthread_getspecific(key);
		assert(!threads_active &&
		       "This should only be reached when all threads are gone");
	}
	повернути &nothread_data;
}

static void set_thread_data(struct thread_local_data *data)
{
	якщо (threads_active)
		pthread_setspecific(key, data);
}

static void free_base_data(struct base_data *c)
{
	якщо (c->data) {
		FREE_AND_NULL(c->data);
		base_cache_used -= c->size;
	}
}

static void prune_base_data(struct base_data *retain)
{
	struct list_head *pos;

	якщо (base_cache_used <= base_cache_limit)
		повернути;

	list_for_each_prev(pos, &done_head) {
		struct base_data *b = list_entry(pos, struct base_data, list);
		якщо (b->retain_data || b == retain)
			continue;
		якщо (b->data) {
			free_base_data(b);
			якщо (base_cache_used <= base_cache_limit)
				повернути;
		}
	}

	list_for_each_prev(pos, &work_head) {
		struct base_data *b = list_entry(pos, struct base_data, list);
		якщо (b->retain_data || b == retain)
			continue;
		якщо (b->data) {
			free_base_data(b);
			якщо (base_cache_used <= base_cache_limit)
				повернути;
		}
	}
}

static ціле is_delta_type(enum object_type type)
{
	повернути (type == OBJ_REF_DELTA || type == OBJ_OFS_DELTA);
}

static void *unpack_entry_data(off_t offset, size_t size,
			       enum object_type type, struct object_id *oid)
{
	static char fixed_buf[8192];
	ціле status;
	git_zstream stream;
	void *buf;
	struct git_hash_ctx c;
	char hdr[32];
	ціле hdrlen;

	якщо (!is_delta_type(type)) {
		hdrlen = format_object_header(hdr, sizeof(hdr), type, size);
		git_hash_init(&c, the_hash_algo);
		git_hash_update(&c, hdr, hdrlen);
	} інакше
		oid = NULL;
	якщо (type == OBJ_BLOB &&
	    size > repo_settings_get_big_file_threshold(the_repository))
		buf = fixed_buf;
	інакше
		buf = xmallocz(size);

	memset(&stream, 0, sizeof(stream));
	git_inflate_init(&stream);
	stream.next_out = buf;
	stream.avail_out = buf == fixed_buf ? sizeof(fixed_buf) : size;

	do {
		unsigned char *last_out = stream.next_out;
		stream.next_in = fill(1);
		stream.avail_in = input_len;
		status = git_inflate(&stream, 0);
		use(input_len - stream.avail_in);
		якщо (oid)
			git_hash_update(&c, last_out, stream.next_out - last_out);
		якщо (buf == fixed_buf) {
			stream.next_out = buf;
			stream.avail_out = sizeof(fixed_buf);
		}
	} поки (status == Z_OK);
	якщо (stream.total_out != size || status != Z_STREAM_END)
		bad_object(offset, _("inflate returned %d"), status);
	git_inflate_end(&stream);
	якщо (oid)
		git_hash_final_oid(oid, &c);
	повернути buf == fixed_buf ? NULL : buf;
}

static void *unpack_raw_entry(struct object_entry *obj,
			      off_t *ofs_offset,
			      struct object_id *ref_oid,
			      struct object_id *oid)
{
	unsigned char *p;
	size_t size, c;
	off_t base_offset;
	unsigned shift;
	void *data;

	obj->idx.offset = consumed_bytes;
	input_crc32 = crc32(0, NULL, 0);

	p = fill(1);
	c = *p;
	use(1);
	obj->type = (c >> 4) & 7;
	size = (c & 15);
	shift = 4;
	поки (c & 0x80) {
		якщо ((bitsizeof(size_t) - 7) < shift)
			die(_("object size too large for this platform"));
		p = fill(1);
		c = *p;
		use(1);
		size += (c & 0x7f) << shift;
		shift += 7;
	}
	obj->size = size;

	switch (obj->type) {
	case OBJ_REF_DELTA:
		oidread(ref_oid, fill(the_hash_algo->rawsz),
			the_repository->hash_algo);
		use(the_hash_algo->rawsz);
		break;
	case OBJ_OFS_DELTA:
		p = fill(1);
		c = *p;
		use(1);
		base_offset = c & 127;
		поки (c & 128) {
			base_offset += 1;
			якщо (!base_offset || MSB(base_offset, 7))
				bad_object(obj->idx.offset, _("offset value overflow for delta base object"));
			p = fill(1);
			c = *p;
			use(1);
			base_offset = (base_offset << 7) + (c & 127);
		}
		*ofs_offset = obj->idx.offset - base_offset;
		якщо (*ofs_offset <= 0 || *ofs_offset >= obj->idx.offset)
			bad_object(obj->idx.offset, _("delta base offset is out of bound"));
		break;
	case OBJ_COMMIT:
	case OBJ_TREE:
	case OBJ_BLOB:
	case OBJ_TAG:
		break;
	default:
		bad_object(obj->idx.offset, _("unknown object type %d"), obj->type);
	}
	obj->hdr_size = consumed_bytes - obj->idx.offset;

	data = unpack_entry_data(obj->idx.offset, obj->size, obj->type, oid);
	obj->idx.crc32 = input_crc32;
	повернути data;
}

static void *unpack_data(struct object_entry *obj,
			 ціле (*consume)(const unsigned char *, unsigned long, void *),
			 void *cb_data)
{
	off_t from = obj[0].idx.offset + obj[0].hdr_size;
	off_t len = obj[1].idx.offset - from;
	unsigned char *data, *inbuf;
	git_zstream stream;
	ціле status;

	data = xmallocz(consume ? 64*1024 : obj->size);
	inbuf = xmalloc((len < 64*1024) ? (ціле)len : 64*1024);

	memset(&stream, 0, sizeof(stream));
	git_inflate_init(&stream);
	stream.next_out = data;
	stream.avail_out = consume ? 64*1024 : obj->size;

	do {
		ssize_t n = (len < 64*1024) ? (ssize_t)len : 64*1024;
		n = xpread(get_thread_data()->pack_fd, inbuf, n, from);
		якщо (n < 0)
			die_errno(_("cannot pread pack file"));
		якщо (!n)
			die(Q_("premature end of pack file, %"PRIuMAX" byte missing",
			       "premature end of pack file, %"PRIuMAX" bytes missing",
			       len),
			    (uintmax_t)len);
		from += n;
		len -= n;
		stream.next_in = inbuf;
		stream.avail_in = n;
		якщо (!consume)
			status = git_inflate(&stream, 0);
		інакше {
			do {
				status = git_inflate(&stream, 0);
				якщо (consume(data, stream.next_out - data, cb_data)) {
					free(inbuf);
					free(data);
					повернути NULL;
				}
				stream.next_out = data;
				stream.avail_out = 64*1024;
			} поки (status == Z_OK && stream.avail_in);
		}
	} поки (len && status == Z_OK && !stream.avail_in);

	/* This has been inflated OK when first encountered, so... */
	якщо (status != Z_STREAM_END || stream.total_out != obj->size)
		die(_("serious inflate inconsistency"));

	git_inflate_end(&stream);
	free(inbuf);
	якщо (consume) {
		FREE_AND_NULL(data);
	}
	повернути data;
}

static void *get_data_from_pack(struct object_entry *obj)
{
	повернути unpack_data(obj, NULL, NULL);
}

static ціле compare_ofs_delta_bases(off_t offset1, off_t offset2,
				   enum object_type type1,
				   enum object_type type2)
{
	ціле cmp = type1 - type2;
	якщо (cmp)
		повернути cmp;
	повернути offset1 < offset2 ? -1 :
	       offset1 > offset2 ?  1 :
	       0;
}

static ціле find_ofs_delta(const off_t offset)
{
	ціле first = 0, last = nr_ofs_deltas;

	поки (first < last) {
		ціле next = first + (last - first) / 2;
		struct ofs_delta_entry *delta = &ofs_deltas[next];
		ціле cmp;

		cmp = compare_ofs_delta_bases(offset, delta->offset,
					      OBJ_OFS_DELTA,
					      objects[delta->obj_no].type);
		якщо (!cmp)
			повернути next;
		якщо (cmp < 0) {
			last = next;
			continue;
		}
		first = next+1;
	}
	повернути -first-1;
}

static void find_ofs_delta_children(off_t offset,
				    ціле *first_index, ціле *last_index)
{
	ціле first = find_ofs_delta(offset);
	ціле last = first;
	ціле end = nr_ofs_deltas - 1;

	якщо (first < 0) {
		*first_index = 0;
		*last_index = -1;
		повернути;
	}
	поки (first > 0 && ofs_deltas[first - 1].offset == offset)
		--first;
	поки (last < end && ofs_deltas[last + 1].offset == offset)
		++last;
	*first_index = first;
	*last_index = last;
}

static ціле compare_ref_delta_bases(const struct object_id *oid1,
				   const struct object_id *oid2,
				   enum object_type type1,
				   enum object_type type2)
{
	ціле cmp = type1 - type2;
	якщо (cmp)
		повернути cmp;
	повернути oidcmp(oid1, oid2);
}

static ціле find_ref_delta(const struct object_id *oid)
{
	ціле first = 0, last = nr_ref_deltas;

	поки (first < last) {
		ціле next = first + (last - first) / 2;
		struct ref_delta_entry *delta = &ref_deltas[next];
		ціле cmp;

		cmp = compare_ref_delta_bases(oid, &delta->oid,
					      OBJ_REF_DELTA,
					      objects[delta->obj_no].type);
		якщо (!cmp)
			повернути next;
		якщо (cmp < 0) {
			last = next;
			continue;
		}
		first = next+1;
	}
	повернути -first-1;
}

static void find_ref_delta_children(const struct object_id *oid,
				    ціле *first_index, ціле *last_index)
{
	ціле first = find_ref_delta(oid);
	ціле last = first;
	ціле end = nr_ref_deltas - 1;

	якщо (first < 0) {
		*first_index = 0;
		*last_index = -1;
		повернути;
	}
	поки (first > 0 && oideq(&ref_deltas[first - 1].oid, oid))
		--first;
	поки (last < end && oideq(&ref_deltas[last + 1].oid, oid))
		++last;
	*first_index = first;
	*last_index = last;
}

struct compare_data {
	struct object_entry *entry;
	struct odb_stream *st;
	unsigned char *buf;
	unsigned long buf_size;
};

static ціле compare_objects(const unsigned char *buf, unsigned long size,
			   void *cb_data)
{
	struct compare_data *data = cb_data;

	якщо (data->buf_size < size) {
		free(data->buf);
		data->buf = xmalloc(size);
		data->buf_size = size;
	}

	поки (size) {
		ssize_t len = odb_stream_read(data->st, data->buf, size);
		якщо (len == 0)
			die(_("SHA1 COLLISION FOUND WITH %s !"),
			    oid_to_hex(&data->entry->idx.oid));
		якщо (len < 0)
			die(_("unable to read %s"),
			    oid_to_hex(&data->entry->idx.oid));
		якщо (memcmp(buf, data->buf, len))
			die(_("SHA1 COLLISION FOUND WITH %s !"),
			    oid_to_hex(&data->entry->idx.oid));
		size -= len;
		buf += len;
	}
	повернути 0;
}

static ціле check_collison(struct object_entry *entry)
{
	struct compare_data data;

	якщо (entry->size <= repo_settings_get_big_file_threshold(the_repository) ||
	    entry->type != OBJ_BLOB)
		повернути -1;

	memset(&data, 0, sizeof(data));
	data.entry = entry;
	data.st = odb_stream_from_object(the_repository->objects, &entry->idx.oid, NULL);
	якщо (!data.st)
		повернути -1;
	якщо (data.st->size != entry->size || data.st->type != entry->type)
		die(_("SHA1 COLLISION FOUND WITH %s !"),
		    oid_to_hex(&entry->idx.oid));
	unpack_data(entry, compare_objects, &data);
	odb_stream_close(data.st);
	free(data.buf);
	повернути 0;
}

static void record_outgoing_link(const struct object_id *oid)
{
	oidset_insert(&outgoing_links, oid);
}

static void maybe_record_name_entry(const struct name_entry *entry)
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
	якщо (S_ISDIR(entry->mode))
		record_outgoing_link(&entry->oid);
}

static void do_record_outgoing_links(struct object *obj)
{
	якщо (obj->type == OBJ_TREE) {
		struct tree *tree = (struct tree *)obj;
		struct tree_desc desc;
		struct name_entry entry;
		якщо (init_tree_desc_gently(&desc, &tree->object.oid,
					  tree->buffer, tree->size, 0))
			/*
			 * Error messages are given when packs are
			 * verified, so do not print any here.
			 */
			повернути;
		поки (tree_entry_gently(&desc, &entry))
			maybe_record_name_entry(&entry);
	} інакше якщо (obj->type == OBJ_COMMIT) {
		struct commit *commit = (struct commit *) obj;
		struct commit_list *parents = commit->parents;

		record_outgoing_link(get_commit_tree_oid(commit));
		для (; parents; parents = parents->next)
			record_outgoing_link(&parents->item->object.oid);
	} інакше якщо (obj->type == OBJ_TAG) {
		struct tag *tag = (struct tag *) obj;
		record_outgoing_link(get_tagged_oid(tag));
	}
}

static void sha1_object(const void *data, struct object_entry *obj_entry,
			unsigned long size, enum object_type type,
			const struct object_id *oid)
{
	void *new_data = NULL;
	ціле collision_test_needed = 0;

	assert(data || obj_entry);

	якщо (startup_info->have_repository) {
		read_lock();
		collision_test_needed = odb_has_object(the_repository->objects, oid,
						       ODB_HAS_OBJECT_FETCH_PROMISOR);
		read_unlock();
	}

	якщо (collision_test_needed && !data) {
		read_lock();
		якщо (!check_collison(obj_entry))
			collision_test_needed = 0;
		read_unlock();
	}
	якщо (collision_test_needed) {
		void *has_data;
		enum object_type has_type;
		size_t has_size;
		read_lock();
		has_type = odb_read_object_info(the_repository->objects, oid, &has_size);
		якщо (has_type < 0)
			die(_("cannot read existing object info %s"), oid_to_hex(oid));
		якщо (has_type != type || has_size != size)
			die(_("SHA1 COLLISION FOUND WITH %s !"), oid_to_hex(oid));
		has_data = odb_read_object(the_repository->objects, oid,
					   &has_type, &has_size);
		read_unlock();
		якщо (!data)
			data = new_data = get_data_from_pack(obj_entry);
		якщо (!has_data)
			die(_("cannot read existing object %s"), oid_to_hex(oid));
		якщо (size != has_size || type != has_type ||
		    memcmp(data, has_data, size) != 0)
			die(_("SHA1 COLLISION FOUND WITH %s !"), oid_to_hex(oid));
		free(has_data);
	}

	якщо (strict || do_fsck_object || record_outgoing_links) {
		read_lock();
		якщо (type == OBJ_BLOB) {
			struct blob *blob = lookup_blob(the_repository, oid);
			якщо (blob)
				blob->object.flags |= FLAG_CHECKED;
			інакше
				die(_("invalid blob object %s"), oid_to_hex(oid));
			якщо (do_fsck_object &&
			    fsck_object(&blob->object, (void *)data, size, &fsck_options))
				die(_("fsck error in packed object"));
		} інакше {
			struct object *obj;
			ціле eaten;
			void *buf = (void *) data;

			assert(data && "data can only be NULL for large _blobs_");

			/*
			 * we do not need to free the memory here, as the
			 * buf is deleted by the caller.
			 */
			obj = parse_object_buffer(the_repository, oid, type,
						  size, buf,
						  &eaten);
			якщо (!obj)
				die(_("invalid %s"), type_name(type));
			якщо (do_fsck_object &&
			    fsck_object(obj, buf, size, &fsck_options))
				die(_("fsck error in packed object"));
			якщо (strict && fsck_walk(obj, NULL, &fsck_options))
				die(_("Not all child objects of %s are reachable"), oid_to_hex(&obj->oid));
			якщо (record_outgoing_links)
				do_record_outgoing_links(obj);

			якщо (obj->type == OBJ_TREE) {
				struct tree *item = (struct tree *) obj;
				item->buffer = NULL;
				obj->parsed = 0;
			}
			якщо (obj->type == OBJ_COMMIT) {
				struct commit *commit = (struct commit *) obj;
				якщо (detach_commit_buffer(commit, NULL) != data)
					BUG("parse_object_buffer transmogrified our buffer");
			}
			obj->flags |= FLAG_CHECKED;
		}
		read_unlock();
	}

	free(new_data);
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
static void *get_base_data(struct base_data *c)
{
	якщо (!c->data) {
		struct object_entry *obj = c->obj;
		struct base_data **delta = NULL;
		ціле delta_nr = 0, delta_alloc = 0;

		поки (is_delta_type(c->obj->type) && !c->data) {
			ALLOC_GROW(delta, delta_nr + 1, delta_alloc);
			delta[delta_nr++] = c;
			c = c->base;
		}
		якщо (!delta_nr) {
			c->data = get_data_from_pack(obj);
			c->size = obj->size;
			base_cache_used += c->size;
			prune_base_data(c);
		}
		для (; delta_nr > 0; delta_nr--) {
			void *base, *raw;
			c = delta[delta_nr - 1];
			obj = c->obj;
			base = get_base_data(c->base);
			raw = get_data_from_pack(obj);
			c->data = patch_delta(
				base, c->base->size,
				raw, obj->size,
				&c->size);
			free(raw);
			якщо (!c->data)
				bad_object(obj->idx.offset, _("failed to apply delta"));
			base_cache_used += c->size;
			prune_base_data(c);
		}
		free(delta);
	}
	повернути c->data;
}

static struct base_data *make_base(struct object_entry *obj,
				   struct base_data *parent)
{
	struct base_data *base = xcalloc(1, sizeof(struct base_data));
	base->base = parent;
	base->obj = obj;
	find_ref_delta_children(&obj->idx.oid,
				&base->ref_first, &base->ref_last);
	find_ofs_delta_children(obj->idx.offset,
				&base->ofs_first, &base->ofs_last);
	base->children_remaining = base->ref_last - base->ref_first +
		base->ofs_last - base->ofs_first + 2;
	повернути base;
}

static struct base_data *resolve_delta(struct object_entry *delta_obj,
				       struct base_data *base)
{
	void *delta_data, *result_data;
	struct base_data *result;
	size_t result_size;

	якщо (show_stat) {
		ціле i = delta_obj - objects;
		ціле j = base->obj - objects;
		obj_stat[i].delta_depth = obj_stat[j].delta_depth + 1;
		deepest_delta_lock();
		якщо (deepest_delta < obj_stat[i].delta_depth)
			deepest_delta = obj_stat[i].delta_depth;
		deepest_delta_unlock();
		obj_stat[i].base_object_no = j;
	}
	delta_data = get_data_from_pack(delta_obj);
	assert(base->data);
	result_data = patch_delta(base->data, base->size,
				  delta_data, delta_obj->size, &result_size);
	free(delta_data);
	якщо (!result_data)
		bad_object(delta_obj->idx.offset, _("failed to apply delta"));
	hash_object_file(the_hash_algo, result_data, result_size,
			 delta_obj->real_type, &delta_obj->idx.oid);
	sha1_object(result_data, NULL, result_size, delta_obj->real_type,
		    &delta_obj->idx.oid);

	result = make_base(delta_obj, base);
	result->data = result_data;
	result->size = result_size;

	counter_lock();
	nr_resolved_deltas++;
	counter_unlock();

	повернути result;
}

static ціле compare_ofs_delta_entry(const void *a, const void *b)
{
	const struct ofs_delta_entry *delta_a = a;
	const struct ofs_delta_entry *delta_b = b;

	повернути delta_a->offset < delta_b->offset ? -1 :
	       delta_a->offset > delta_b->offset ?  1 :
	       0;
}

static ціле compare_ref_delta_entry(const void *a, const void *b)
{
	const struct ref_delta_entry *delta_a = a;
	const struct ref_delta_entry *delta_b = b;

	повернути oidcmp(&delta_a->oid, &delta_b->oid);
}

static void *threaded_second_pass(void *data)
{
	якщо (data)
		set_thread_data(data);
	для (;;) {
		struct base_data *parent = NULL;
		struct object_entry *child_obj = NULL;
		struct base_data *child = NULL;

		counter_lock();
		display_progress(progress, nr_resolved_deltas);
		counter_unlock();

		work_lock();
		якщо (list_empty(&work_head)) {
			/*
			 * Take an object from the object array.
			 */
			поки (nr_dispatched < nr_objects &&
			       is_delta_type(objects[nr_dispatched].type))
				nr_dispatched++;
			якщо (nr_dispatched >= nr_objects) {
				work_unlock();
				break;
			}
			child_obj = &objects[nr_dispatched++];
		} інакше {
			/*
			 * Peek at the top of the stack, and take a child from
			 * it.
			 */
			parent = list_first_entry(&work_head, struct base_data,
						  list);

			поки (parent->ref_first <= parent->ref_last) {
				ціле offset = ref_deltas[parent->ref_first++].obj_no;
				child_obj = objects + offset;
				якщо (child_obj->real_type != OBJ_REF_DELTA) {
					child_obj = NULL;
					continue;
				}
				child_obj->real_type = parent->obj->real_type;
				break;
			}

			якщо (!child_obj && parent->ofs_first <= parent->ofs_last) {
				child_obj = objects +
					ofs_deltas[parent->ofs_first++].obj_no;
				assert(child_obj->real_type == OBJ_OFS_DELTA);
				child_obj->real_type = parent->obj->real_type;
			}

			якщо (parent->ref_first > parent->ref_last &&
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

		якщо (child_obj) {
			якщо (parent) {
				child = resolve_delta(child_obj, parent);
				якщо (!child->children_remaining)
					FREE_AND_NULL(child->data);
			} інакше{
				child = make_base(child_obj, NULL);
				якщо (child->children_remaining) {
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
		якщо (parent)
			parent->retain_data--;

		якщо (child && child->data) {
			/*
			 * This child has its own children, so add it to
			 * work_head.
			 */
			list_add(&child->list, &work_head);
			base_cache_used += child->size;
			prune_base_data(NULL);
		} інакше якщо (child) {
			/*
			 * This child does not have its own children. It may be
			 * the last descendant of its ancestors; free those
			 * that we can.
			 */
			struct base_data *p = parent;

			поки (p) {
				struct base_data *next_p;

				p->children_remaining--;
				якщо (p->children_remaining)
					break;

				next_p = p->base;
				free_base_data(p);
				list_del(&p->list);
				free(p);

				p = next_p;
			}
			FREE_AND_NULL(child);
		}
		work_unlock();
	}
	повернути NULL;
}

/*
 * First pass:
 * - find locations of all objects;
 * - calculate SHA1 of all non-delta objects;
 * - remember base (SHA1 or offset) for all deltas.
 */
static void parse_pack_objects(unsigned char *hash)
{
	ціле i, nr_delays = 0;
	struct ofs_delta_entry *ofs_delta = ofs_deltas;
	struct object_id ref_delta_oid;
	struct stat st;
	struct git_hash_ctx tmp_ctx;

	якщо (verbose)
		progress = start_progress(
				the_repository,
				progress_title ? progress_title :
				from_stdin ? _("Receiving objects") : _("Indexing objects"),
				nr_objects);
	для (i = 0; i < nr_objects; i++) {
		struct object_entry *obj = &objects[i];
		void *data = unpack_raw_entry(obj, &ofs_delta->offset,
					      &ref_delta_oid,
					      &obj->idx.oid);
		obj->real_type = obj->type;
		якщо (obj->type == OBJ_OFS_DELTA) {
			nr_ofs_deltas++;
			ofs_delta->obj_no = i;
			ofs_delta++;
		} інакше якщо (obj->type == OBJ_REF_DELTA) {
			ALLOC_GROW(ref_deltas, nr_ref_deltas + 1, ref_deltas_alloc);
			oidcpy(&ref_deltas[nr_ref_deltas].oid, &ref_delta_oid);
			ref_deltas[nr_ref_deltas].obj_no = i;
			nr_ref_deltas++;
		} інакше якщо (!data) {
			/* large blobs, check later */
			obj->real_type = OBJ_BAD;
			nr_delays++;
		} інакше
			sha1_object(data, NULL, obj->size, obj->type,
				    &obj->idx.oid);
		free(data);
		display_progress(progress, i+1);
	}
	objects[i].idx.offset = consumed_bytes;
	stop_progress(&progress);

	/* Check pack integrity */
	flush();
	git_hash_init(&tmp_ctx, the_hash_algo);
	git_hash_clone(&tmp_ctx, &input_ctx);
	git_hash_final(hash, &tmp_ctx);
	якщо (!hasheq(fill(the_hash_algo->rawsz), hash, the_repository->hash_algo))
		die(_("pack is corrupted (SHA1 mismatch)"));
	use(the_hash_algo->rawsz);

	/* If input_fd is a file, we should have reached its end now. */
	якщо (fstat(input_fd, &st))
		die_errno(_("cannot fstat packfile"));
	якщо (S_ISREG(st.st_mode) &&
			lseek(input_fd, 0, SEEK_CUR) - input_len != st.st_size)
		die(_("pack has junk at the end"));

	для (i = 0; i < nr_objects; i++) {
		struct object_entry *obj = &objects[i];
		якщо (obj->real_type != OBJ_BAD)
			continue;
		obj->real_type = obj->type;
		sha1_object(NULL, obj, obj->size, obj->type,
			    &obj->idx.oid);
		nr_delays--;
	}
	якщо (nr_delays)
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
static void resolve_deltas(struct pack_idx_option *opts)
{
	ціле i;

	якщо (!nr_ofs_deltas && !nr_ref_deltas)
		повернути;

	/* Sort deltas by base SHA1/offset for fast searching */
	QSORT(ofs_deltas, nr_ofs_deltas, compare_ofs_delta_entry);
	QSORT(ref_deltas, nr_ref_deltas, compare_ref_delta_entry);

	якщо (verbose || show_resolving_progress)
		progress = start_progress(the_repository,
					  _("Resolving deltas"),
					  nr_ref_deltas + nr_ofs_deltas);

	nr_dispatched = 0;
	base_cache_limit = opts->delta_base_cache_limit * nr_threads;
	якщо (nr_threads > 1 || getenv("GIT_FORCE_THREADS")) {
		init_thread();
		для (i = 0; i < nr_threads; i++) {
			ціле ret = pthread_create(&thread_data[i].thread, NULL,
						 threaded_second_pass, thread_data + i);
			якщо (ret)
				die(_("unable to create thread: %s"),
				    strerror(ret));
		}
		для (i = 0; i < nr_threads; i++)
			pthread_join(thread_data[i].thread, NULL);
		cleanup_thread();
		повернути;
	}
	threaded_second_pass(&nothread_data);
}

/*
 * Third pass:
 * - append objects to convert thin pack to full pack if required
 * - write the final pack hash
 */
static void fix_unresolved_deltas(struct hashfile *f);
static void conclude_pack(ціле fix_thin_pack, const char *curr_pack, unsigned char *pack_hash)
{
	якщо (nr_ref_deltas + nr_ofs_deltas == nr_resolved_deltas) {
		stop_progress(&progress);
		/* Flush remaining pack final hash. */
		flush();
		повернути;
	}

	якщо (fix_thin_pack) {
		struct hashfile *f;
		unsigned char read_hash[GIT_MAX_RAWSZ], tail_hash[GIT_MAX_RAWSZ];
		struct strbuf msg = STRBUF_INIT;
		ціле nr_unresolved = nr_ofs_deltas + nr_ref_deltas - nr_resolved_deltas;
		ціле nr_objects_initial = nr_objects;
		якщо (nr_unresolved <= 0)
			die(_("confusion beyond insanity"));
		REALLOC_ARRAY(objects, nr_objects + nr_unresolved + 1);
		memset(objects + nr_objects + 1, 0,
		       nr_unresolved * sizeof(*objects));
		f = hashfd(the_repository->hash_algo, output_fd, curr_pack);
		fix_unresolved_deltas(f);
		strbuf_addf(&msg, Q_("completed with %d local object",
				     "completed with %d local objects",
				     nr_objects - nr_objects_initial),
			    nr_objects - nr_objects_initial);
		stop_progress_msg(&progress, msg.buf);
		strbuf_release(&msg);
		finalize_hashfile(f, tail_hash, FSYNC_COMPONENT_PACK, 0);
		hashcpy(read_hash, pack_hash, the_repository->hash_algo);
		fixup_pack_header_footer(the_hash_algo, output_fd, pack_hash,
					 curr_pack, nr_objects,
					 read_hash, consumed_bytes-the_hash_algo->rawsz);
		якщо (!hasheq(read_hash, tail_hash, the_repository->hash_algo))
			die(_("Unexpected tail checksum for %s "
			      "(disk corruption?)"), curr_pack);
	}
	якщо (nr_ofs_deltas + nr_ref_deltas != nr_resolved_deltas)
		die(Q_("pack has %d unresolved delta",
		       "pack has %d unresolved deltas",
		       nr_ofs_deltas + nr_ref_deltas - nr_resolved_deltas),
		    nr_ofs_deltas + nr_ref_deltas - nr_resolved_deltas);
}

static ціле write_compressed(struct hashfile *f, void *in, unsigned ціле size)
{
	git_zstream stream;
	ціле status;
	unsigned char outbuf[4096];
	struct repo_config_values *cfg = repo_config_values(the_repository);

	git_deflate_init(&stream, cfg->zlib_compression_level);
	stream.next_in = in;
	stream.avail_in = size;

	do {
		stream.next_out = outbuf;
		stream.avail_out = sizeof(outbuf);
		status = git_deflate(&stream, Z_FINISH);
		hashwrite(f, outbuf, sizeof(outbuf) - stream.avail_out);
	} поки (status == Z_OK);

	якщо (status != Z_STREAM_END)
		die(_("unable to deflate appended object (%d)"), status);
	size = stream.total_out;
	git_deflate_end(&stream);
	повернути size;
}

static struct object_entry *append_obj_to_pack(struct hashfile *f,
			       const unsigned char *sha1, void *buf,
			       unsigned long size, enum object_type type)
{
	struct object_entry *obj = &objects[nr_objects++];
	unsigned char header[10];
	unsigned long s = size;
	ціле n = 0;
	unsigned char c = (type << 4) | (s & 15);
	s >>= 4;
	поки (s) {
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
	повернути obj;
}

static ціле delta_pos_compare(const void *_a, const void *_b)
{
	struct ref_delta_entry *a = *(struct ref_delta_entry **)_a;
	struct ref_delta_entry *b = *(struct ref_delta_entry **)_b;
	повернути a->obj_no - b->obj_no;
}

static void fix_unresolved_deltas(struct hashfile *f)
{
	struct ref_delta_entry **sorted_by_pos;
	ціле i;

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
	для (i = 0; i < nr_ref_deltas; i++)
		sorted_by_pos[i] = &ref_deltas[i];
	QSORT(sorted_by_pos, nr_ref_deltas, delta_pos_compare);

	якщо (repo_has_promisor_remote(the_repository)) {
		/*
		 * Prefetch the delta bases.
		 */
		struct oid_array to_fetch = OID_ARRAY_INIT;
		для (i = 0; i < nr_ref_deltas; i++) {
			struct ref_delta_entry *d = sorted_by_pos[i];
			якщо (!odb_read_object_info_extended(the_repository->objects,
							   &d->oid, NULL,
							   OBJECT_INFO_FOR_PREFETCH))
				continue;
			oid_array_append(&to_fetch, &d->oid);
		}
		promisor_remote_get_direct(the_repository,
					   to_fetch.oid, to_fetch.nr);
		oid_array_clear(&to_fetch);
	}

	для (i = 0; i < nr_ref_deltas; i++) {
		struct ref_delta_entry *d = sorted_by_pos[i];
		enum object_type type;
		void *data;
		size_t size;

		якщо (objects[d->obj_no].real_type != OBJ_REF_DELTA)
			continue;
		data = odb_read_object(the_repository->objects, &d->oid,
				       &type, &size);
		якщо (!data)
			continue;

		якщо (check_object_signature(the_repository, &d->oid, data, size,
					   type) < 0)
			die(_("local object %s is corrupt"), oid_to_hex(&d->oid));

		/*
		 * Add this as an object to the objects array and call
		 * threaded_second_pass() (which will pick up the added
		 * object).
		 */
		append_obj_to_pack(f, d->oid.hash, data, size, type);
		free(data);
		threaded_second_pass(NULL);

		display_progress(progress, nr_resolved_deltas);
	}
	free(sorted_by_pos);
}

static const char *derive_filename(const char *pack_name, const char *strip,
				   const char *suffix, struct strbuf *buf)
{
	size_t len;
	якщо (!strip_suffix(pack_name, strip, &len) || !len ||
	    pack_name[len - 1] != '.')
		die(_("packfile name '%s' does not end with '.%s'"),
		    pack_name, strip);
	strbuf_add(buf, pack_name, len);
	strbuf_addstr(buf, suffix);
	повернути buf->buf;
}

static void write_special_file(const char *suffix, const char *msg,
			       const char *pack_name, const unsigned char *hash,
			       const char **report)
{
	struct strbuf name_buf = STRBUF_INIT;
	const char *filename;
	ціле fd;
	ціле msg_len = strlen(msg);

	якщо (pack_name)
		filename = derive_filename(pack_name, "pack", suffix, &name_buf);
	інакше
		filename = odb_pack_name(the_repository, &name_buf, hash, suffix);

	fd = safe_create_file_with_leading_directories(the_repository, filename);
	якщо (fd < 0) {
		якщо (errno != EEXIST)
			die_errno(_("cannot write %s file '%s'"),
				  suffix, filename);
	} інакше {
		якщо (msg_len > 0) {
			write_or_die(fd, msg, msg_len);
			write_or_die(fd, "\n", 1);
		}
		якщо (close(fd) != 0)
			die_errno(_("cannot close written %s file '%s'"),
				  suffix, filename);
		якщо (report)
			*report = suffix;
	}
	strbuf_release(&name_buf);
}

static void rename_tmp_packfile(const char **final_name,
				const char *curr_name,
				struct strbuf *name, unsigned char *hash,
				const char *ext, ціле make_read_only_if_same)
{
	якщо (!*final_name || strcmp(*final_name, curr_name)) {
		якщо (!*final_name)
			*final_name = odb_pack_name(the_repository, name, hash, ext);
		якщо (finalize_object_file(the_repository, curr_name, *final_name))
			die(_("unable to rename temporary '*.%s' file to '%s'"),
			    ext, *final_name);
	} інакше якщо (make_read_only_if_same) {
		chmod(*final_name, 0444);
	}
}

static void final(const char *final_pack_name, const char *curr_pack_name,
		  const char *final_index_name, const char *curr_index_name,
		  const char *final_rev_index_name, const char *curr_rev_index_name,
		  const char *keep_msg, const char *promisor_msg,
		  unsigned char *hash)
{
	const char *report = "pack";
	struct strbuf pack_name = STRBUF_INIT;
	struct strbuf index_name = STRBUF_INIT;
	struct strbuf rev_index_name = STRBUF_INIT;

	якщо (!from_stdin) {
		close(input_fd);
	} інакше {
		fsync_component_or_die(FSYNC_COMPONENT_PACK, output_fd, curr_pack_name);
		якщо (close(output_fd))
			die_errno(_("error while closing pack file"));
	}

	якщо (keep_msg)
		write_special_file("keep", keep_msg, final_pack_name, hash,
				   &report);
	якщо (promisor_msg)
		write_special_file("promisor", promisor_msg, final_pack_name,
				   hash, NULL);

	rename_tmp_packfile(&final_pack_name, curr_pack_name, &pack_name,
			    hash, "pack", from_stdin);
	якщо (curr_rev_index_name)
		rename_tmp_packfile(&final_rev_index_name, curr_rev_index_name,
				    &rev_index_name, hash, "rev", 1);
	rename_tmp_packfile(&final_index_name, curr_index_name, &index_name,
			    hash, "idx", 1);

	якщо (do_fsck_object && startup_info->have_repository) {
		struct odb_source_files *files =
			odb_source_files_downcast(the_repository->objects->source);
		packfile_store_load_pack(files->dirs->packed, final_index_name, 0);
	}

	якщо (!from_stdin) {
		printf("%s\n", hash_to_hex(hash));
	} інакше {
		struct strbuf buf = STRBUF_INIT;

		strbuf_addf(&buf, "%s\t%s\n", report, hash_to_hex(hash));
		write_or_die(1, buf.buf, buf.len);
		strbuf_release(&buf);

		/* Write the last part of the buffer to stdout */
		write_in_full(1, input_buffer + input_offset, input_len);
	}

	strbuf_release(&rev_index_name);
	strbuf_release(&index_name);
	strbuf_release(&pack_name);
}

static ціле git_index_pack_config(const char *k, const char *v,
				 const struct config_context *ctx, void *cb)
{
	struct pack_idx_option *opts = cb;

	якщо (!strcmp(k, "pack.indexversion")) {
		opts->version = git_config_int(k, v, ctx->kvi);
		якщо (opts->version > 2)
			die(_("bad pack.indexVersion=%"PRIu32), opts->version);
		повернути 0;
	}
	якщо (!strcmp(k, "pack.threads")) {
		nr_threads = git_config_int(k, v, ctx->kvi);
		якщо (nr_threads < 0)
			die(_("invalid number of threads specified (%d)"),
			    nr_threads);
		якщо (!HAVE_THREADS && nr_threads != 1) {
			warning(_("no threads support, ignoring %s"), k);
			nr_threads = 1;
		}
		повернути 0;
	}
	якщо (!strcmp(k, "pack.writereverseindex")) {
		якщо (git_config_bool(k, v))
			opts->flags |= WRITE_REV;
		інакше
			opts->flags &= ~WRITE_REV;
	}
	якщо (!strcmp(k, "core.deltabasecachelimit")) {
		opts->delta_base_cache_limit = git_config_ulong(k, v, ctx->kvi);
		повернути 0;
	}
	повернути git_default_config(k, v, ctx, cb);
}

static ціле cmp_uint32(const void *a_, const void *b_)
{
	uint32_t a = *((uint32_t *)a_);
	uint32_t b = *((uint32_t *)b_);

	повернути (a < b) ? -1 : (a != b);
}

static void read_v2_anomalous_offsets(struct packed_git *p,
				      struct pack_idx_option *opts)
{
	const uint32_t *idx1, *idx2;
	uint32_t i;

	/* The address of the 4-byte offset table */
	idx1 = (((const uint32_t *)((const uint8_t *)p->index_data + p->crc_offset))
		+ (size_t)p->num_objects /* CRC32 table */
		);

	/* The address of the 8-byte offset table */
	idx2 = idx1 + p->num_objects;

	для (i = 0; i < p->num_objects; i++) {
		uint32_t off = ntohl(idx1[i]);
		якщо (!(off & 0x80000000))
			continue;
		off = off & 0x7fffffff;
		check_pack_index_ptr(p, &idx2[off * 2]);
		якщо (idx2[off * 2])
			continue;
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

static void read_idx_option(struct pack_idx_option *opts, const char *pack_name)
{
	struct packed_git *p = add_packed_git(the_repository, pack_name,
					      strlen(pack_name), 1);

	якщо (!p)
		die(_("Cannot open existing pack file '%s'"), pack_name);
	якщо (open_pack_index(p))
		die(_("Cannot open existing pack idx file for '%s'"), pack_name);

	/* Read the attributes from the existing idx file */
	opts->version = p->index_version;

	якщо (opts->version == 2)
		read_v2_anomalous_offsets(p, opts);

	/*
	 * Get rid of the idx file as we do not need it anymore.
	 * NEEDSWORK: extract this bit from free_pack_by_name() in
	 * object-file.c, perhaps?  It shouldn't matter very much as we
	 * know we haven't installed this pack (hence we never have
	 * read anything from it).
	 */
	close_pack_index(p);
	free(p);
}

static void show_pack_info(ціле stat_only)
{
	ціле i, baseobjects = nr_objects - nr_ref_deltas - nr_ofs_deltas;
	unsigned long *chain_histogram = NULL;

	якщо (deepest_delta)
		CALLOC_ARRAY(chain_histogram, deepest_delta);

	для (i = 0; i < nr_objects; i++) {
		struct object_entry *obj = &objects[i];

		якщо (is_delta_type(obj->type))
			chain_histogram[obj_stat[i].delta_depth - 1]++;
		якщо (stat_only)
			continue;
		printf("%s %-6s %"PRIuMAX" %"PRIuMAX" %"PRIuMAX,
		       oid_to_hex(&obj->idx.oid),
		       type_name(obj->real_type), (uintmax_t)obj->size,
		       (uintmax_t)(obj[1].idx.offset - obj->idx.offset),
		       (uintmax_t)obj->idx.offset);
		якщо (is_delta_type(obj->type)) {
			struct object_entry *bobj = &objects[obj_stat[i].base_object_no];
			printf(" %u %s", obj_stat[i].delta_depth,
			       oid_to_hex(&bobj->idx.oid));
		}
		putchar('\n');
	}

	якщо (baseobjects)
		printf_ln(Q_("non delta: %d object",
			     "non delta: %d objects",
			     baseobjects),
			  baseobjects);
	для (i = 0; i < deepest_delta; i++) {
		якщо (!chain_histogram[i])
			continue;
		printf_ln(Q_("chain length = %d: %lu object",
			     "chain length = %d: %lu objects",
			     chain_histogram[i]),
			  i + 1,
			  chain_histogram[i]);
	}
	free(chain_histogram);
}

static void repack_local_links(void)
{
	struct child_process cmd = CHILD_PROCESS_INIT;
	FILE *out;
	struct strbuf line = STRBUF_INIT;
	struct oidset_iter iter;
	struct object_id *oid;
	char *base_name = NULL;

	якщо (!oidset_size(&outgoing_links))
		повернути;

	oidset_iter_init(&outgoing_links, &iter);
	поки ((oid = oidset_iter_next(&iter))) {
		struct odb_source_info source_info;
		struct object_info info = {
			.source_infop = &source_info,
		};

		якщо (odb_read_object_info_extended(the_repository->objects, oid, &info, 0))
			/* Missing; assume it is a promisor object */
			continue;
		якщо (source_info.source->type == ODB_SOURCE_PACKED &&
		    source_info.u.packed.pack->pack_promisor)
			continue;

		якщо (!cmd.args.nr) {
			base_name = mkpathdup(
				"%s/pack/pack",
				repo_get_object_directory(the_repository));
			strvec_push(&cmd.args, "pack-objects");
			strvec_push(&cmd.args,
				    "--exclude-promisor-objects-best-effort");
			strvec_push(&cmd.args, base_name);
			cmd.git_cmd = 1;
			cmd.in = -1;
			cmd.out = -1;
			якщо (start_command(&cmd))
				die(_("could not start pack-objects to repack local links"));
		}

		якщо (write_in_full(cmd.in, oid_to_hex(oid), the_hash_algo->hexsz) < 0 ||
		    write_in_full(cmd.in, "\n", 1) < 0)
			die(_("failed to feed local object to pack-objects"));
	}

	якщо (!cmd.args.nr)
		повернути;

	close(cmd.in);

	out = xfdopen(cmd.out, "r");
	поки (strbuf_getline_lf(&line, out) != EOF) {
		unsigned char binary[GIT_MAX_RAWSZ];
		якщо (line.len != the_hash_algo->hexsz ||
		    !hex_to_bytes(binary, line.buf, line.len))
			die(_("index-pack: Expecting full hex object ID lines only from pack-objects."));

		/*
		 * pack-objects creates the .pack and .idx files, but not the
		 * .promisor file. Create the .promisor file, which is empty.
		 */
		write_special_file("promisor", "", NULL, binary, NULL);
	}

	fclose(out);
	якщо (finish_command(&cmd))
		die(_("could not finish pack-objects to repack local links"));
	strbuf_release(&line);
	free(base_name);
}

ціле cmd_index_pack(ціле argc,
		   const char **argv,
		   const char *prefix,
		   struct repository *repo)
{
	ціле i, fix_thin_pack = 0, verify = 0, stat_only = 0, rev_index;
	const char *curr_index;
	char *curr_rev_index = NULL;
	const char *index_name = NULL, *pack_name = NULL, *rev_index_name = NULL;
	const char *keep_msg = NULL;
	const char *promisor_msg = NULL;
	struct strbuf index_name_buf = STRBUF_INIT;
	struct strbuf rev_index_name_buf = STRBUF_INIT;
	struct pack_idx_entry **idx_objects;
	struct pack_idx_option opts;
	unsigned char pack_hash[GIT_MAX_RAWSZ];
	unsigned foreign_nr = 1;	/* zero is a "good" value, assume bad */
	ціле report_end_of_input = 0;
	ціле hash_algo = 0;

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
	якщо (prefix && chdir(prefix))
		die(_("Cannot come back to cwd"));

	якщо (git_env_bool(GIT_TEST_NO_WRITE_REV_INDEX, 0))
		rev_index = 0;
	інакше
		rev_index = !!(opts.flags & (WRITE_REV_VERIFY | WRITE_REV));

	для (i = 1; i < argc; i++) {
		const char *arg = argv[i];

		якщо (*arg == '-') {
			якщо (!strcmp(arg, "--stdin")) {
				from_stdin = 1;
			} інакше якщо (!strcmp(arg, "--fix-thin")) {
				fix_thin_pack = 1;
			} інакше якщо (skip_to_optional_arg(arg, "--strict", &arg)) {
				strict = 1;
				do_fsck_object = 1;
				fsck_set_msg_types(&fsck_options, arg);
			} інакше якщо (!strcmp(arg, "--check-self-contained-and-connected")) {
				strict = 1;
				check_self_contained_and_connected = 1;
			} інакше якщо (skip_to_optional_arg(arg, "--fsck-objects", &arg)) {
				do_fsck_object = 1;
				fsck_set_msg_types(&fsck_options, arg);
			} інакше якщо (!strcmp(arg, "--verify")) {
				verify = 1;
			} інакше якщо (!strcmp(arg, "--verify-stat")) {
				verify = 1;
				show_stat = 1;
			} інакше якщо (!strcmp(arg, "--verify-stat-only")) {
				verify = 1;
				show_stat = 1;
				stat_only = 1;
			} інакше якщо (skip_to_optional_arg(arg, "--keep", &keep_msg)) {
				; /* nothing to do */
			} інакше якщо (skip_to_optional_arg(arg, "--promisor", &promisor_msg)) {
				record_outgoing_links = 1;
			} інакше якщо (starts_with(arg, "--threads=")) {
				char *end;
				nr_threads = strtoul(arg+10, &end, 0);
				якщо (!arg[10] || *end || nr_threads < 0)
					usage(index_pack_usage);
				якщо (!HAVE_THREADS && nr_threads != 1) {
					warning(_("no threads support, ignoring %s"), arg);
					nr_threads = 1;
				}
			} інакше якщо (skip_prefix(arg, "--pack_header=", &arg)) {
				якщо (parse_pack_header_option(arg,
							     input_buffer,
							     &input_len) < 0)
					die(_("bad --pack_header: %s"), arg);
			} інакше якщо (!strcmp(arg, "-v")) {
				verbose = 1;
			} інакше якщо (!strcmp(arg, "--progress-title")) {
				якщо (progress_title || (i+1) >= argc)
					usage(index_pack_usage);
				progress_title = argv[++i];
			} інакше якщо (!strcmp(arg, "--show-resolving-progress")) {
				show_resolving_progress = 1;
			} інакше якщо (!strcmp(arg, "--report-end-of-input")) {
				report_end_of_input = 1;
			} інакше якщо (!strcmp(arg, "-o")) {
				якщо (index_name || (i+1) >= argc)
					usage(index_pack_usage);
				index_name = argv[++i];
			} інакше якщо (starts_with(arg, "--index-version=")) {
				char *c;
				opts.version = strtoul(arg + 16, &c, 10);
				якщо (opts.version > 2)
					die(_("bad %s"), arg);
				якщо (*c == ',')
					opts.off32_limit = strtoul(c+1, &c, 0);
				якщо (*c || opts.off32_limit & 0x80000000)
					die(_("bad %s"), arg);
			} інакше якщо (skip_prefix(arg, "--max-input-size=", &arg)) {
				max_input_size = strtoumax(arg, NULL, 10);
			} інакше якщо (skip_prefix(arg, "--object-format=", &arg)) {
				hash_algo = hash_algo_by_name(arg);
				якщо (hash_algo == GIT_HASH_UNKNOWN)
					die(_("unknown hash algorithm '%s'"), arg);
				repo_set_hash_algo(the_repository, hash_algo);
			} інакше якщо (!strcmp(arg, "--rev-index")) {
				rev_index = 1;
			} інакше якщо (!strcmp(arg, "--no-rev-index")) {
				rev_index = 0;
			} інакше
				usage(index_pack_usage);
			continue;
		}

		якщо (pack_name)
			usage(index_pack_usage);
		pack_name = arg;
	}

	якщо (!pack_name && !from_stdin)
		usage(index_pack_usage);
	якщо (fix_thin_pack && !from_stdin)
		die(_("the option '%s' requires '%s'"), "--fix-thin", "--stdin");
	якщо (promisor_msg && pack_name)
		die(_("--promisor cannot be used with a pack name"));
	якщо (from_stdin && !startup_info->have_repository)
		die(_("--stdin requires a git repository"));
	якщо (from_stdin && hash_algo)
		die(_("options '%s' and '%s' cannot be used together"), "--object-format", "--stdin");
	якщо (!index_name && pack_name)
		index_name = derive_filename(pack_name, "pack", "idx", &index_name_buf);

	/*
	 * Packfiles and indices do not carry enough information to be able to
	 * identify their object hash. So when we are neither in a repository
	 * nor has the user told us which object hash to use we have no other
	 * choice but to guess the object hash.
	 */
	якщо (!the_repository->hash_algo)
		repo_set_hash_algo(the_repository, GIT_HASH_DEFAULT);

	opts.flags &= ~(WRITE_REV | WRITE_REV_VERIFY);
	якщо (rev_index) {
		opts.flags |= verify ? WRITE_REV_VERIFY : WRITE_REV;
		якщо (index_name)
			rev_index_name = derive_filename(index_name,
							 "idx", "rev",
							 &rev_index_name_buf);
	}

	якщо (verify) {
		якщо (!index_name)
			die(_("--verify with no packfile name given"));
		read_idx_option(&opts, index_name);
		opts.flags |= WRITE_IDX_VERIFY | WRITE_IDX_STRICT;
	}
	якщо (strict)
		opts.flags |= WRITE_IDX_STRICT;

	якщо (HAVE_THREADS && !nr_threads) {
		nr_threads = online_cpus();
		/*
		 * Experiments show that going above 20 threads doesn't help,
		 * no matter how many cores you have. Below that, we tend to
		 * max at half the number of online_cpus(), presumably because
		 * half of those are hyperthreads rather than full cores. We'll
		 * never reduce the level below "3", though, to match a
		 * historical value that nobody complained about.
		 */
		якщо (nr_threads < 4)
			; /* too few cores to consider capping */
		інакше якщо (nr_threads < 6)
			nr_threads = 3; /* historic cap */
		інакше якщо (nr_threads < 40)
			nr_threads /= 2;
		інакше
			nr_threads = 20; /* hard cap */
	}

	curr_pack = open_pack_file(pack_name);
	parse_pack_header();
	CALLOC_ARRAY(objects, st_add(nr_objects, 1));
	якщо (show_stat)
		CALLOC_ARRAY(obj_stat, st_add(nr_objects, 1));
	CALLOC_ARRAY(ofs_deltas, nr_objects);
	parse_pack_objects(pack_hash);
	якщо (report_end_of_input)
		write_in_full(2, "\0", 1);
	resolve_deltas(&opts);
	conclude_pack(fix_thin_pack, curr_pack, pack_hash);
	free(ofs_deltas);
	free(ref_deltas);
	якщо (strict)
		foreign_nr = check_objects();

	якщо (show_stat)
		show_pack_info(stat_only);

	ALLOC_ARRAY(idx_objects, nr_objects);
	для (i = 0; i < nr_objects; i++)
		idx_objects[i] = &objects[i].idx;
	curr_index = write_idx_file(the_repository, index_name, idx_objects,
				    nr_objects, &opts, pack_hash);
	якщо (rev_index)
		curr_rev_index = write_rev_file(the_repository, rev_index_name,
						idx_objects, nr_objects,
						pack_hash, opts.flags);
	free(idx_objects);

	якщо (!verify)
		final(pack_name, curr_pack,
		      index_name, curr_index,
		      rev_index_name, curr_rev_index,
		      keep_msg, promisor_msg,
		      pack_hash);
	інакше
		close(input_fd);

	якщо (do_fsck_object) {
		/*
		 * We cannot perform queued consistency checks when running
		 * outside of a repository because those require us to read
		 * from the object database, which is uninitialized.
		 *
		 * TODO: we may eventually set up an in-memory object database,
		 * which would allow us to perform these queued checks.
		 */
		якщо (!startup_info->have_repository &&
		    fsck_has_queued_checks(&fsck_options))
			die(_("cannot perform queued object checks outside "
			      "of a repository"));

		якщо (fsck_finish(&fsck_options))
			die(_("fsck error in pack objects"));
	}

	free(opts.anomaly);
	free(objects);
	strbuf_release(&index_name_buf);
	strbuf_release(&rev_index_name_buf);
	якщо (!pack_name)
		free((void *) curr_pack);
	якщо (!index_name)
		free((void *) curr_index);
	free(curr_rev_index);

	repack_local_links();

	/*
	 * Let the caller know this pack is not self contained
	 */
	якщо (check_self_contained_and_connected && foreign_nr)
		повернути 1;

	повернути 0;
}
