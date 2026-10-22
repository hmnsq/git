#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "delta.h"
#அந்தர்பாவ "environment.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "hex.h"
#அந்தர்பாவ "pack.h"
#அந்தர்பாவ "csum-file.h"
#அந்தர்பாவ "blob.h"
#அந்தர்பாவ "commit.h"
#அந்தர்பாவ "tag.h"
#அந்தர்பாவ "tree.h"
#அந்தர்பாவ "progress.h"
#அந்தர்பாவ "fsck.h"
#அந்தர்பாவ "strbuf.h"
#அந்தர்பாவ "thread-utils.h"
#அந்தர்பாவ "packfile.h"
#அந்தர்பாவ "pack-revindex.h"
#அந்தர்பாவ "object-file.h"
#அந்தர்பாவ "odb.h"
#அந்தர்பாவ "odb/streaming.h"
#அந்தர்பாவ "oid-array.h"
#அந்தர்பாவ "oidset.h"
#அந்தர்பாவ "path.h"
#அந்தர்பாவ "replace-object.h"
#அந்தர்பாவ "tree-walk.h"
#அந்தர்பாவ "promisor-remote.h"
#அந்தர்பாவ "run-command.h"
#அந்தர்பாவ "setup.h"
#அந்தர்பாவ "strvec.h"

நிலையான மாறிலி எழுத்து index_pack_usage[] =
"git index-pack [-v] [-o <index-file>] [--keep | --keep=<msg>] [--[no-]rev-index] [--verify] [--strict[=<msg-id>=<severity>...]] [--fsck-objects[=<msg-id>=<severity>...]] (<pack-file> | --stdin [--fix-thin] [<pack-file>])";

கட்டமைப்பு object_entry {
	கட்டமைப்பு pack_idx_entry idx;
	size_t size;
	unsigned எழுத்து hdr_size;
	signed எழுத்து type;
	signed எழுத்து real_type;
};

கட்டமைப்பு object_stat {
	unsigned delta_depth;
	முழு base_object_no;
};

கட்டமைப்பு base_data {
	/* Initialized by make_base(). */
	கட்டமைப்பு base_data *base;
	கட்டமைப்பு object_entry *obj;
	முழு ref_first, ref_last;
	முழு ofs_first, ofs_last;
	/*
	 * Threads should increment retain_data if they are about to call
	 * patch_delta() using this struct's data as a base, and decrement this
	 * when they are done. While retain_data is nonzero, this struct's data
	 * will not be freed even if the delta base cache limit is exceeded.
	 */
	முழு retain_data;
	/*
	 * The number of direct children that have not been fully processed
	 * (entered work_head, entered done_head, left done_head). When this
	 * number reaches zero, this struct base_data can be freed.
	 */
	முழு children_remaining;

	/* Not initialized by make_base(). */
	கட்டமைப்பு list_head list;
	வெறுமை *data;
	size_t size;
};

/*
 * Stack of struct base_data that have unprocessed children.
 * threaded_second_pass() uses this as a source of work (the other being the
 * objects array).
 *
 * Guarded by work_mutex.
 */
நிலையான LIST_HEAD(work_head);

/*
 * Stack of struct base_data that have children, all of whom have been
 * processed or are being processed, and at least one child is being processed.
 * These struct base_data must be kept around until the last child is
 * processed.
 *
 * Guarded by work_mutex.
 */
நிலையான LIST_HEAD(done_head);

/*
 * All threads share one delta base cache.
 *
 * base_cache_used is guarded by work_mutex, and base_cache_limit is read-only
 * in a thread.
 */
நிலையான size_t base_cache_used;
நிலையான size_t base_cache_limit;

கட்டமைப்பு thread_local_data {
	pthread_t thread;
	முழு pack_fd;
};

/* Remember to update object flag allocation in object.h */
#define FLAG_LINK (1u<<20)
#define FLAG_CHECKED (1u<<21)

கட்டமைப்பு ofs_delta_entry {
	off_t offset;
	முழு obj_no;
};

கட்டமைப்பு ref_delta_entry {
	கட்டமைப்பு object_id oid;
	முழு obj_no;
};

நிலையான கட்டமைப்பு object_entry *objects;
நிலையான கட்டமைப்பு object_stat *obj_stat;
நிலையான கட்டமைப்பு ofs_delta_entry *ofs_deltas;
நிலையான கட்டமைப்பு ref_delta_entry *ref_deltas;
நிலையான கட்டமைப்பு thread_local_data nothread_data;
நிலையான முழு nr_objects;
நிலையான முழு nr_ofs_deltas;
நிலையான முழு nr_ref_deltas;
நிலையான முழு ref_deltas_alloc;
நிலையான முழு nr_resolved_deltas;
நிலையான முழு nr_threads;

நிலையான முழு from_stdin;
நிலையான முழு strict;
நிலையான முழு do_fsck_object;
நிலையான கட்டமைப்பு fsck_options fsck_options;
நிலையான முழு verbose;
நிலையான மாறிலி எழுத்து *progress_title;
நிலையான முழு show_resolving_progress;
நிலையான முழு show_stat;
நிலையான முழு check_self_contained_and_connected;

நிலையான கட்டமைப்பு progress *progress;

நிலையான unsigned எழுத்து உள்ளீடு_buffer[DEFAULT_IO_BUFFER_SIZE];
நிலையான unsigned முழு உள்ளீடு_offset, உள்ளீடு_len;
நிலையான off_t consumed_bytes;
நிலையான off_t max_உள்ளீடு_size;
நிலையான unsigned deepest_delta;
நிலையான கட்டமைப்பு git_hash_ctx உள்ளீடு_ctx;
நிலையான uint32_t உள்ளீடு_crc32;
நிலையான முழு உள்ளீடு_fd, output_fd;
நிலையான மாறிலி எழுத்து *curr_pack;

/*
 * outgoing_links is guarded by read_mutex, and record_outgoing_links is
 * read-only in a thread.
 */
நிலையான கட்டமைப்பு oidset outgoing_links = OIDSET_INIT;
நிலையான முழு record_outgoing_links;

நிலையான கட்டமைப்பு thread_local_data *thread_data;
நிலையான முழு nr_dispatched;
நிலையான முழு threads_active;

நிலையான pthread_mutex_t read_mutex;
#define read_lock()		lock_mutex(&read_mutex)
#define read_unlock()		unlock_mutex(&read_mutex)

நிலையான pthread_mutex_t counter_mutex;
#define counter_lock()		lock_mutex(&counter_mutex)
#define counter_unlock()	unlock_mutex(&counter_mutex)

நிலையான pthread_mutex_t work_mutex;
#define work_lock()		lock_mutex(&work_mutex)
#define work_unlock()		unlock_mutex(&work_mutex)

நிலையான pthread_mutex_t deepest_delta_mutex;
#define deepest_delta_lock()	lock_mutex(&deepest_delta_mutex)
#define deepest_delta_unlock()	unlock_mutex(&deepest_delta_mutex)

நிலையான pthread_சாவி_t சாவி;

நிலையான உள்வரி வெறுமை lock_mutex(pthread_mutex_t *mutex)
{
	எனில் (threads_active)
		pthread_mutex_lock(mutex);
}

நிலையான உள்வரி வெறுமை unlock_mutex(pthread_mutex_t *mutex)
{
	எனில் (threads_active)
		pthread_mutex_unlock(mutex);
}

/*
 * Mutex and conditional variable can't be statically-initialized on Windows.
 */
நிலையான வெறுமை init_thread(வெறுமை)
{
	முழு i;
	init_recursive_mutex(&read_mutex);
	pthread_mutex_init(&counter_mutex, வெற்று);
	pthread_mutex_init(&work_mutex, வெற்று);
	எனில் (show_stat)
		pthread_mutex_init(&deepest_delta_mutex, வெற்று);
	pthread_சாவி_create(&சாவி, வெற்று);
	CALLOC_ARRAY(thread_data, nr_threads);
	க்கு (i = 0; i < nr_threads; i++) {
		thread_data[i].pack_fd = xopen(curr_pack, O_RDONLY);
	}

	threads_active = 1;
}

நிலையான வெறுமை cleanup_thread(வெறுமை)
{
	முழு i;
	எனில் (!threads_active)
		திரும்பு;
	threads_active = 0;
	pthread_mutex_destroy(&read_mutex);
	pthread_mutex_destroy(&counter_mutex);
	pthread_mutex_destroy(&work_mutex);
	எனில் (show_stat)
		pthread_mutex_destroy(&deepest_delta_mutex);
	க்கு (i = 0; i < nr_threads; i++)
		close(thread_data[i].pack_fd);
	pthread_சாவி_delete(சாவி);
	விடுவி(thread_data);
}

நிலையான முழு mark_link(கட்டமைப்பு object *obj, எண்ணிக்கை object_type type,
		     வெறுமை *data UNUSED,
		     கட்டமைப்பு fsck_options *options UNUSED)
{
	எனில் (!obj)
		திரும்பு -1;

	எனில் (type != OBJ_ANY && obj->type != type)
		die(_("object type mismatch at %s"), oid_to_hex(&obj->oid));

	obj->flags |= FLAG_LINK;
	திரும்பு 0;
}

/* The content of each linked object must have been checked
   or it must be already present in the object database */
நிலையான unsigned check_object(கட்டமைப்பு object *obj)
{
	எனில் (!obj)
		திரும்பு 0;

	எனில் (!(obj->flags & FLAG_LINK))
		திரும்பு 0;

	எனில் (!(obj->flags & FLAG_CHECKED)) {
		size_t size;
		முழு type = odb_read_object_info(the_repository->objects,
						&obj->oid, &size);
		எனில் (type <= 0)
			die(_("did not receive expected object %s"),
			      oid_to_hex(&obj->oid));
		எனில் (type != obj->type)
			die(_("object %s: expected type %s, found %s"),
			    oid_to_hex(&obj->oid),
			    type_name(obj->type), type_name(type));
		obj->flags |= FLAG_CHECKED;
		திரும்பு 1;
	}

	திரும்பு 0;
}

நிலையான unsigned check_objects(வெறுமை)
{
	unsigned i, max, foreign_nr = 0;

	max = get_max_object_index(the_repository);

	எனில் (verbose)
		progress = start_delayed_progress(the_repository,
						  _("Checking objects"), max);

	க்கு (i = 0; i < max; i++) {
		foreign_nr += check_object(get_indexed_object(the_repository, i));
		display_progress(progress, i + 1);
	}

	stop_progress(&progress);
	திரும்பு foreign_nr;
}


/* Discard current buffer used content. */
நிலையான வெறுமை flush(வெறுமை)
{
	எனில் (உள்ளீடு_offset) {
		எனில் (output_fd >= 0)
			write_or_die(output_fd, உள்ளீடு_buffer, உள்ளீடு_offset);
		git_hash_update(&உள்ளீடு_ctx, உள்ளீடு_buffer, உள்ளீடு_offset);
		memmove(உள்ளீடு_buffer, உள்ளீடு_buffer + உள்ளீடு_offset, உள்ளீடு_len);
		உள்ளீடு_offset = 0;
	}
}

/*
 * Make sure at least "min" bytes are available in the buffer, and
 * return the pointer to the buffer.
 */
நிலையான வெறுமை *fill(முழு min)
{
	எனில் (min <= உள்ளீடு_len)
		திரும்பு உள்ளீடு_buffer + உள்ளீடு_offset;
	எனில் (min > அளவு(உள்ளீடு_buffer))
		die(Q_("cannot fill %d byte",
		       "cannot fill %d bytes",
		       min),
		    min);
	flush();
	செய் {
		ssize_t ret = xread(உள்ளீடு_fd, உள்ளீடு_buffer + உள்ளீடு_len,
				அளவு(உள்ளீடு_buffer) - உள்ளீடு_len);
		எனில் (ret <= 0) {
			எனில் (!ret)
				die(_("early EOF"));
			die_errno(_("read error on input"));
		}
		உள்ளீடு_len += ret;
		எனில் (from_stdin)
			display_throughput(progress, consumed_bytes + உள்ளீடு_len);
	} போது (உள்ளீடு_len < min);
	திரும்பு உள்ளீடு_buffer;
}

நிலையான வெறுமை use(முழு bytes)
{
	எனில் (bytes > உள்ளீடு_len)
		die(_("used more bytes than were available"));
	உள்ளீடு_crc32 = crc32(உள்ளீடு_crc32, உள்ளீடு_buffer + உள்ளீடு_offset, bytes);
	உள்ளீடு_len -= bytes;
	உள்ளீடு_offset += bytes;

	/* make sure off_t is sufficiently large not to wrap */
	எனில் (signed_add_overflows(consumed_bytes, bytes))
		die(_("pack too large for current definition of off_t"));
	consumed_bytes += bytes;
	எனில் (max_உள்ளீடு_size && consumed_bytes > max_உள்ளீடு_size) {
		கட்டமைப்பு strbuf size_limit = STRBUF_INIT;
		strbuf_humanise_bytes(&size_limit, max_உள்ளீடு_size);
		die(_("pack exceeds maximum allowed size (%s)"),
		    size_limit.buf);
	}
}

நிலையான மாறிலி எழுத்து *open_pack_file(மாறிலி எழுத்து *pack_name)
{
	எனில் (from_stdin) {
		உள்ளீடு_fd = 0;
		எனில் (!pack_name) {
			கட்டமைப்பு strbuf tmp_file = STRBUF_INIT;
			output_fd = odb_mkstemp(the_repository->objects, &tmp_file,
						"pack/tmp_pack_XXXXXX");
			pack_name = strbuf_detach(&tmp_file, வெற்று);
		} இல்லையெனில் {
			output_fd = xopen(pack_name, O_CREAT|O_EXCL|O_RDWR, 0600);
		}
		nothread_data.pack_fd = output_fd;
	} இல்லையெனில் {
		உள்ளீடு_fd = xopen(pack_name, O_RDONLY);
		output_fd = -1;
		nothread_data.pack_fd = உள்ளீடு_fd;
	}
	git_hash_init(&உள்ளீடு_ctx, the_hash_algo);
	திரும்பு pack_name;
}

நிலையான வெறுமை parse_pack_header(வெறுமை)
{
	unsigned எழுத்து *hdr = fill(அளவு(கட்டமைப்பு pack_header));

	/* Header consistency check */
	எனில் (get_be32(hdr) != PACK_SIGNATURE)
		die(_("pack signature mismatch"));
	hdr += 4;
	எனில் (!pack_version_ok_native(get_be32(hdr)))
		die(_("pack version %"PRIu32" unsupported"),
		    get_be32(hdr));
	hdr += 4;

	nr_objects = get_be32(hdr);
	use(அளவு(கட்டமைப்பு pack_header));
}

__attribute__((format (printf, 2, 3)))
நிலையான NORETURN வெறுமை bad_object(off_t offset, மாறிலி எழுத்து *format, ...)
{
	va_list params;
	எழுத்து buf[1024];

	va_start(params, format);
	vsnprintf(buf, அளவு(buf), format, params);
	va_end(params);
	die(_("pack has bad object at offset %"PRIuMAX": %s"),
	    (uintmax_t)offset, buf);
}

நிலையான உள்வரி கட்டமைப்பு thread_local_data *get_thread_data(வெறுமை)
{
	எனில் (HAVE_THREADS) {
		எனில் (threads_active)
			திரும்பு pthread_getspecific(சாவி);
		ப்ரதிஜ்ஞா(!threads_active &&
		       "This should only be reached when all threads are gone");
	}
	திரும்பு &nothread_data;
}

நிலையான வெறுமை set_thread_data(கட்டமைப்பு thread_local_data *data)
{
	எனில் (threads_active)
		pthread_setspecific(சாவி, data);
}

நிலையான வெறுமை free_base_data(கட்டமைப்பு base_data *c)
{
	எனில் (c->data) {
		FREE_AND_NULL(c->data);
		base_cache_used -= c->size;
	}
}

நிலையான வெறுமை prune_base_data(கட்டமைப்பு base_data *retain)
{
	கட்டமைப்பு list_head *pos;

	எனில் (base_cache_used <= base_cache_limit)
		திரும்பு;

	list_for_each_prev(pos, &done_head) {
		கட்டமைப்பு base_data *b = list_entry(pos, கட்டமைப்பு base_data, list);
		எனில் (b->retain_data || b == retain)
			தொடர்;
		எனில் (b->data) {
			free_base_data(b);
			எனில் (base_cache_used <= base_cache_limit)
				திரும்பு;
		}
	}

	list_for_each_prev(pos, &work_head) {
		கட்டமைப்பு base_data *b = list_entry(pos, கட்டமைப்பு base_data, list);
		எனில் (b->retain_data || b == retain)
			தொடர்;
		எனில் (b->data) {
			free_base_data(b);
			எனில் (base_cache_used <= base_cache_limit)
				திரும்பு;
		}
	}
}

நிலையான முழு is_delta_type(எண்ணிக்கை object_type type)
{
	திரும்பு (type == OBJ_REF_DELTA || type == OBJ_OFS_DELTA);
}

நிலையான வெறுமை *unpack_entry_data(off_t offset, size_t size,
			       எண்ணிக்கை object_type type, கட்டமைப்பு object_id *oid)
{
	நிலையான எழுத்து fixed_buf[8192];
	முழு status;
	git_zstream stream;
	வெறுமை *buf;
	கட்டமைப்பு git_hash_ctx c;
	எழுத்து hdr[32];
	முழு hdrlen;

	எனில் (!is_delta_type(type)) {
		hdrlen = format_object_header(hdr, அளவு(hdr), type, size);
		git_hash_init(&c, the_hash_algo);
		git_hash_update(&c, hdr, hdrlen);
	} இல்லையெனில்
		oid = வெற்று;
	எனில் (type == OBJ_BLOB &&
	    size > repo_settings_get_big_file_threshold(the_repository))
		buf = fixed_buf;
	இல்லையெனில்
		buf = xmallocz(size);

	memset(&stream, 0, அளவு(stream));
	git_inflate_init(&stream);
	stream.next_out = buf;
	stream.avail_out = buf == fixed_buf ? அளவு(fixed_buf) : size;

	செய் {
		unsigned எழுத்து *last_out = stream.next_out;
		stream.next_in = fill(1);
		stream.avail_in = உள்ளீடு_len;
		status = git_inflate(&stream, 0);
		use(உள்ளீடு_len - stream.avail_in);
		எனில் (oid)
			git_hash_update(&c, last_out, stream.next_out - last_out);
		எனில் (buf == fixed_buf) {
			stream.next_out = buf;
			stream.avail_out = அளவு(fixed_buf);
		}
	} போது (status == Z_OK);
	எனில் (stream.total_out != size || status != Z_STREAM_END)
		bad_object(offset, _("inflate returned %d"), status);
	git_inflate_end(&stream);
	எனில் (oid)
		git_hash_final_oid(oid, &c);
	திரும்பு buf == fixed_buf ? வெற்று : buf;
}

நிலையான வெறுமை *unpack_raw_entry(கட்டமைப்பு object_entry *obj,
			      off_t *ofs_offset,
			      கட்டமைப்பு object_id *ref_oid,
			      கட்டமைப்பு object_id *oid)
{
	unsigned எழுத்து *p;
	size_t size, c;
	off_t base_offset;
	unsigned shift;
	வெறுமை *data;

	obj->idx.offset = consumed_bytes;
	உள்ளீடு_crc32 = crc32(0, வெற்று, 0);

	p = fill(1);
	c = *p;
	use(1);
	obj->type = (c >> 4) & 7;
	size = (c & 15);
	shift = 4;
	போது (c & 0x80) {
		எனில் ((bitsizeof(size_t) - 7) < shift)
			die(_("object size too large for this platform"));
		p = fill(1);
		c = *p;
		use(1);
		size += (c & 0x7f) << shift;
		shift += 7;
	}
	obj->size = size;

	தேர்வு (obj->type) {
	நிலை OBJ_REF_DELTA:
		oidread(ref_oid, fill(the_hash_algo->rawsz),
			the_repository->hash_algo);
		use(the_hash_algo->rawsz);
		நிறுத்து;
	நிலை OBJ_OFS_DELTA:
		p = fill(1);
		c = *p;
		use(1);
		base_offset = c & 127;
		போது (c & 128) {
			base_offset += 1;
			எனில் (!base_offset || MSB(base_offset, 7))
				bad_object(obj->idx.offset, _("offset value overflow for delta base object"));
			p = fill(1);
			c = *p;
			use(1);
			base_offset = (base_offset << 7) + (c & 127);
		}
		*ofs_offset = obj->idx.offset - base_offset;
		எனில் (*ofs_offset <= 0 || *ofs_offset >= obj->idx.offset)
			bad_object(obj->idx.offset, _("delta base offset is out of bound"));
		நிறுத்து;
	நிலை OBJ_COMMIT:
	நிலை OBJ_மரம்:
	நிலை OBJ_BLOB:
	நிலை OBJ_TAG:
		நிறுத்து;
	இயல்பு:
		bad_object(obj->idx.offset, _("unknown object type %d"), obj->type);
	}
	obj->hdr_size = consumed_bytes - obj->idx.offset;

	data = unpack_entry_data(obj->idx.offset, obj->size, obj->type, oid);
	obj->idx.crc32 = உள்ளீடு_crc32;
	திரும்பு data;
}

நிலையான வெறுமை *unpack_data(கட்டமைப்பு object_entry *obj,
			 முழு (*consume)(மாறிலி unsigned எழுத்து *, unsigned long, வெறுமை *),
			 வெறுமை *cb_data)
{
	off_t from = obj[0].idx.offset + obj[0].hdr_size;
	off_t len = obj[1].idx.offset - from;
	unsigned எழுத்து *data, *inbuf;
	git_zstream stream;
	முழு status;

	data = xmallocz(consume ? 64*1024 : obj->size);
	inbuf = xmalloc((len < 64*1024) ? (முழு)len : 64*1024);

	memset(&stream, 0, அளவு(stream));
	git_inflate_init(&stream);
	stream.next_out = data;
	stream.avail_out = consume ? 64*1024 : obj->size;

	செய் {
		ssize_t n = (len < 64*1024) ? (ssize_t)len : 64*1024;
		n = xpread(get_thread_data()->pack_fd, inbuf, n, from);
		எனில் (n < 0)
			die_errno(_("cannot pread pack file"));
		எனில் (!n)
			die(Q_("premature end of pack file, %"PRIuMAX" byte missing",
			       "premature end of pack file, %"PRIuMAX" bytes missing",
			       len),
			    (uintmax_t)len);
		from += n;
		len -= n;
		stream.next_in = inbuf;
		stream.avail_in = n;
		எனில் (!consume)
			status = git_inflate(&stream, 0);
		இல்லையெனில் {
			செய் {
				status = git_inflate(&stream, 0);
				எனில் (consume(data, stream.next_out - data, cb_data)) {
					விடுவி(inbuf);
					விடுவி(data);
					திரும்பு வெற்று;
				}
				stream.next_out = data;
				stream.avail_out = 64*1024;
			} போது (status == Z_OK && stream.avail_in);
		}
	} போது (len && status == Z_OK && !stream.avail_in);

	/* This has been inflated OK when first encountered, so... */
	எனில் (status != Z_STREAM_END || stream.total_out != obj->size)
		die(_("serious inflate inconsistency"));

	git_inflate_end(&stream);
	விடுவி(inbuf);
	எனில் (consume) {
		FREE_AND_NULL(data);
	}
	திரும்பு data;
}

நிலையான வெறுமை *get_data_from_pack(கட்டமைப்பு object_entry *obj)
{
	திரும்பு unpack_data(obj, வெற்று, வெற்று);
}

நிலையான முழு compare_ofs_delta_bases(off_t offset1, off_t offset2,
				   எண்ணிக்கை object_type type1,
				   எண்ணிக்கை object_type type2)
{
	முழு cmp = type1 - type2;
	எனில் (cmp)
		திரும்பு cmp;
	திரும்பு offset1 < offset2 ? -1 :
	       offset1 > offset2 ?  1 :
	       0;
}

நிலையான முழு find_ofs_delta(மாறிலி off_t offset)
{
	முழு first = 0, last = nr_ofs_deltas;

	போது (first < last) {
		முழு next = first + (last - first) / 2;
		கட்டமைப்பு ofs_delta_entry *delta = &ofs_deltas[next];
		முழு cmp;

		cmp = compare_ofs_delta_bases(offset, delta->offset,
					      OBJ_OFS_DELTA,
					      objects[delta->obj_no].type);
		எனில் (!cmp)
			திரும்பு next;
		எனில் (cmp < 0) {
			last = next;
			தொடர்;
		}
		first = next+1;
	}
	திரும்பு -first-1;
}

நிலையான வெறுமை find_ofs_delta_children(off_t offset,
				    முழு *first_index, முழு *last_index)
{
	முழு first = find_ofs_delta(offset);
	முழு last = first;
	முழு end = nr_ofs_deltas - 1;

	எனில் (first < 0) {
		*first_index = 0;
		*last_index = -1;
		திரும்பு;
	}
	போது (first > 0 && ofs_deltas[first - 1].offset == offset)
		--first;
	போது (last < end && ofs_deltas[last + 1].offset == offset)
		++last;
	*first_index = first;
	*last_index = last;
}

நிலையான முழு compare_ref_delta_bases(மாறிலி கட்டமைப்பு object_id *oid1,
				   மாறிலி கட்டமைப்பு object_id *oid2,
				   எண்ணிக்கை object_type type1,
				   எண்ணிக்கை object_type type2)
{
	முழு cmp = type1 - type2;
	எனில் (cmp)
		திரும்பு cmp;
	திரும்பு oidcmp(oid1, oid2);
}

நிலையான முழு find_ref_delta(மாறிலி கட்டமைப்பு object_id *oid)
{
	முழு first = 0, last = nr_ref_deltas;

	போது (first < last) {
		முழு next = first + (last - first) / 2;
		கட்டமைப்பு ref_delta_entry *delta = &ref_deltas[next];
		முழு cmp;

		cmp = compare_ref_delta_bases(oid, &delta->oid,
					      OBJ_REF_DELTA,
					      objects[delta->obj_no].type);
		எனில் (!cmp)
			திரும்பு next;
		எனில் (cmp < 0) {
			last = next;
			தொடர்;
		}
		first = next+1;
	}
	திரும்பு -first-1;
}

நிலையான வெறுமை find_ref_delta_children(மாறிலி கட்டமைப்பு object_id *oid,
				    முழு *first_index, முழு *last_index)
{
	முழு first = find_ref_delta(oid);
	முழு last = first;
	முழு end = nr_ref_deltas - 1;

	எனில் (first < 0) {
		*first_index = 0;
		*last_index = -1;
		திரும்பு;
	}
	போது (first > 0 && oideq(&ref_deltas[first - 1].oid, oid))
		--first;
	போது (last < end && oideq(&ref_deltas[last + 1].oid, oid))
		++last;
	*first_index = first;
	*last_index = last;
}

கட்டமைப்பு compare_data {
	கட்டமைப்பு object_entry *entry;
	கட்டமைப்பு odb_stream *st;
	unsigned எழுத்து *buf;
	unsigned long buf_size;
};

நிலையான முழு compare_objects(மாறிலி unsigned எழுத்து *buf, unsigned long size,
			   வெறுமை *cb_data)
{
	கட்டமைப்பு compare_data *data = cb_data;

	எனில் (data->buf_size < size) {
		விடுவி(data->buf);
		data->buf = xmalloc(size);
		data->buf_size = size;
	}

	போது (size) {
		ssize_t len = odb_stream_read(data->st, data->buf, size);
		எனில் (len == 0)
			die(_("SHA1 COLLISION FOUND WITH %s !"),
			    oid_to_hex(&data->entry->idx.oid));
		எனில் (len < 0)
			die(_("unable to read %s"),
			    oid_to_hex(&data->entry->idx.oid));
		எனில் (memcmp(buf, data->buf, len))
			die(_("SHA1 COLLISION FOUND WITH %s !"),
			    oid_to_hex(&data->entry->idx.oid));
		size -= len;
		buf += len;
	}
	திரும்பு 0;
}

நிலையான முழு check_collison(கட்டமைப்பு object_entry *entry)
{
	கட்டமைப்பு compare_data data;

	எனில் (entry->size <= repo_settings_get_big_file_threshold(the_repository) ||
	    entry->type != OBJ_BLOB)
		திரும்பு -1;

	memset(&data, 0, அளவு(data));
	data.entry = entry;
	data.st = odb_stream_from_object(the_repository->objects, &entry->idx.oid, வெற்று);
	எனில் (!data.st)
		திரும்பு -1;
	எனில் (data.st->size != entry->size || data.st->type != entry->type)
		die(_("SHA1 COLLISION FOUND WITH %s !"),
		    oid_to_hex(&entry->idx.oid));
	unpack_data(entry, compare_objects, &data);
	odb_stream_close(data.st);
	விடுவி(data.buf);
	திரும்பு 0;
}

நிலையான வெறுமை record_outgoing_link(மாறிலி கட்டமைப்பு object_id *oid)
{
	oidset_செருகு(&outgoing_links, oid);
}

நிலையான வெறுமை maybe_record_name_entry(மாறிலி கட்டமைப்பு name_entry *entry)
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
	எனில் (S_ISDIR(entry->mode))
		record_outgoing_link(&entry->oid);
}

நிலையான வெறுமை do_record_outgoing_links(கட்டமைப்பு object *obj)
{
	எனில் (obj->type == OBJ_மரம்) {
		கட்டமைப்பு மரம் *மரம் = (கட்டமைப்பு மரம் *)obj;
		கட்டமைப்பு மரம்_desc desc;
		கட்டமைப்பு name_entry entry;
		எனில் (init_மரம்_desc_gently(&desc, &மரம்->object.oid,
					  மரம்->buffer, மரம்->size, 0))
			/*
			 * Error messages are given when packs are
			 * verified, so do not print any here.
			 */
			திரும்பு;
		போது (மரம்_entry_gently(&desc, &entry))
			maybe_record_name_entry(&entry);
	} இல்லையெனில் எனில் (obj->type == OBJ_COMMIT) {
		கட்டமைப்பு commit *commit = (கட்டமைப்பு commit *) obj;
		கட்டமைப்பு commit_list *parents = commit->parents;

		record_outgoing_link(get_commit_மரம்_oid(commit));
		க்கு (; parents; parents = parents->next)
			record_outgoing_link(&parents->item->object.oid);
	} இல்லையெனில் எனில் (obj->type == OBJ_TAG) {
		கட்டமைப்பு tag *tag = (கட்டமைப்பு tag *) obj;
		record_outgoing_link(get_tagged_oid(tag));
	}
}

நிலையான வெறுமை sha1_object(மாறிலி வெறுமை *data, கட்டமைப்பு object_entry *obj_entry,
			unsigned long size, எண்ணிக்கை object_type type,
			மாறிலி கட்டமைப்பு object_id *oid)
{
	வெறுமை *new_data = வெற்று;
	முழு collision_test_needed = 0;

	ப்ரதிஜ்ஞா(data || obj_entry);

	எனில் (startup_info->have_repository) {
		read_lock();
		collision_test_needed = odb_has_object(the_repository->objects, oid,
						       ODB_HAS_OBJECT_FETCH_PROMISOR);
		read_unlock();
	}

	எனில் (collision_test_needed && !data) {
		read_lock();
		எனில் (!check_collison(obj_entry))
			collision_test_needed = 0;
		read_unlock();
	}
	எனில் (collision_test_needed) {
		வெறுமை *has_data;
		எண்ணிக்கை object_type has_type;
		size_t has_size;
		read_lock();
		has_type = odb_read_object_info(the_repository->objects, oid, &has_size);
		எனில் (has_type < 0)
			die(_("cannot read existing object info %s"), oid_to_hex(oid));
		எனில் (has_type != type || has_size != size)
			die(_("SHA1 COLLISION FOUND WITH %s !"), oid_to_hex(oid));
		has_data = odb_read_object(the_repository->objects, oid,
					   &has_type, &has_size);
		read_unlock();
		எனில் (!data)
			data = new_data = get_data_from_pack(obj_entry);
		எனில் (!has_data)
			die(_("cannot read existing object %s"), oid_to_hex(oid));
		எனில் (size != has_size || type != has_type ||
		    memcmp(data, has_data, size) != 0)
			die(_("SHA1 COLLISION FOUND WITH %s !"), oid_to_hex(oid));
		விடுவி(has_data);
	}

	எனில் (strict || do_fsck_object || record_outgoing_links) {
		read_lock();
		எனில் (type == OBJ_BLOB) {
			கட்டமைப்பு blob *blob = lookup_blob(the_repository, oid);
			எனில் (blob)
				blob->object.flags |= FLAG_CHECKED;
			இல்லையெனில்
				die(_("invalid blob object %s"), oid_to_hex(oid));
			எனில் (do_fsck_object &&
			    fsck_object(&blob->object, (வெறுமை *)data, size, &fsck_options))
				die(_("fsck error in packed object"));
		} இல்லையெனில் {
			கட்டமைப்பு object *obj;
			முழு eaten;
			வெறுமை *buf = (வெறுமை *) data;

			ப்ரதிஜ்ஞா(data && "data can only be NULL for large _blobs_");

			/*
			 * we do not need to free the memory here, as the
			 * buf is deleted by the caller.
			 */
			obj = parse_object_buffer(the_repository, oid, type,
						  size, buf,
						  &eaten);
			எனில் (!obj)
				die(_("invalid %s"), type_name(type));
			எனில் (do_fsck_object &&
			    fsck_object(obj, buf, size, &fsck_options))
				die(_("fsck error in packed object"));
			எனில் (strict && fsck_walk(obj, வெற்று, &fsck_options))
				die(_("Not all child objects of %s are reachable"), oid_to_hex(&obj->oid));
			எனில் (record_outgoing_links)
				do_record_outgoing_links(obj);

			எனில் (obj->type == OBJ_மரம்) {
				கட்டமைப்பு மரம் *item = (கட்டமைப்பு மரம் *) obj;
				item->buffer = வெற்று;
				obj->parsed = 0;
			}
			எனில் (obj->type == OBJ_COMMIT) {
				கட்டமைப்பு commit *commit = (கட்டமைப்பு commit *) obj;
				எனில் (detach_commit_buffer(commit, வெற்று) != data)
					BUG("parse_object_buffer transmogrified our buffer");
			}
			obj->flags |= FLAG_CHECKED;
		}
		read_unlock();
	}

	விடுவி(new_data);
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
நிலையான வெறுமை *get_base_data(கட்டமைப்பு base_data *c)
{
	எனில் (!c->data) {
		கட்டமைப்பு object_entry *obj = c->obj;
		கட்டமைப்பு base_data **delta = வெற்று;
		முழு delta_nr = 0, delta_alloc = 0;

		போது (is_delta_type(c->obj->type) && !c->data) {
			ALLOC_GROW(delta, delta_nr + 1, delta_alloc);
			delta[delta_nr++] = c;
			c = c->base;
		}
		எனில் (!delta_nr) {
			c->data = get_data_from_pack(obj);
			c->size = obj->size;
			base_cache_used += c->size;
			prune_base_data(c);
		}
		க்கு (; delta_nr > 0; delta_nr--) {
			வெறுமை *base, *raw;
			c = delta[delta_nr - 1];
			obj = c->obj;
			base = get_base_data(c->base);
			raw = get_data_from_pack(obj);
			c->data = patch_delta(
				base, c->base->size,
				raw, obj->size,
				&c->size);
			விடுவி(raw);
			எனில் (!c->data)
				bad_object(obj->idx.offset, _("failed to apply delta"));
			base_cache_used += c->size;
			prune_base_data(c);
		}
		விடுவி(delta);
	}
	திரும்பு c->data;
}

நிலையான கட்டமைப்பு base_data *make_base(கட்டமைப்பு object_entry *obj,
				   கட்டமைப்பு base_data *parent)
{
	கட்டமைப்பு base_data *base = xcalloc(1, அளவு(கட்டமைப்பு base_data));
	base->base = parent;
	base->obj = obj;
	find_ref_delta_children(&obj->idx.oid,
				&base->ref_first, &base->ref_last);
	find_ofs_delta_children(obj->idx.offset,
				&base->ofs_first, &base->ofs_last);
	base->children_remaining = base->ref_last - base->ref_first +
		base->ofs_last - base->ofs_first + 2;
	திரும்பு base;
}

நிலையான கட்டமைப்பு base_data *resolve_delta(கட்டமைப்பு object_entry *delta_obj,
				       கட்டமைப்பு base_data *base)
{
	வெறுமை *delta_data, *result_data;
	கட்டமைப்பு base_data *result;
	size_t result_size;

	எனில் (show_stat) {
		முழு i = delta_obj - objects;
		முழு j = base->obj - objects;
		obj_stat[i].delta_depth = obj_stat[j].delta_depth + 1;
		deepest_delta_lock();
		எனில் (deepest_delta < obj_stat[i].delta_depth)
			deepest_delta = obj_stat[i].delta_depth;
		deepest_delta_unlock();
		obj_stat[i].base_object_no = j;
	}
	delta_data = get_data_from_pack(delta_obj);
	ப்ரதிஜ்ஞா(base->data);
	result_data = patch_delta(base->data, base->size,
				  delta_data, delta_obj->size, &result_size);
	விடுவி(delta_data);
	எனில் (!result_data)
		bad_object(delta_obj->idx.offset, _("failed to apply delta"));
	hash_object_file(the_hash_algo, result_data, result_size,
			 delta_obj->real_type, &delta_obj->idx.oid);
	sha1_object(result_data, வெற்று, result_size, delta_obj->real_type,
		    &delta_obj->idx.oid);

	result = make_base(delta_obj, base);
	result->data = result_data;
	result->size = result_size;

	counter_lock();
	nr_resolved_deltas++;
	counter_unlock();

	திரும்பு result;
}

நிலையான முழு compare_ofs_delta_entry(மாறிலி வெறுமை *a, மாறிலி வெறுமை *b)
{
	மாறிலி கட்டமைப்பு ofs_delta_entry *delta_a = a;
	மாறிலி கட்டமைப்பு ofs_delta_entry *delta_b = b;

	திரும்பு delta_a->offset < delta_b->offset ? -1 :
	       delta_a->offset > delta_b->offset ?  1 :
	       0;
}

நிலையான முழு compare_ref_delta_entry(மாறிலி வெறுமை *a, மாறிலி வெறுமை *b)
{
	மாறிலி கட்டமைப்பு ref_delta_entry *delta_a = a;
	மாறிலி கட்டமைப்பு ref_delta_entry *delta_b = b;

	திரும்பு oidcmp(&delta_a->oid, &delta_b->oid);
}

நிலையான வெறுமை *threaded_second_pass(வெறுமை *data)
{
	எனில் (data)
		set_thread_data(data);
	க்கு (;;) {
		கட்டமைப்பு base_data *parent = வெற்று;
		கட்டமைப்பு object_entry *child_obj = வெற்று;
		கட்டமைப்பு base_data *child = வெற்று;

		counter_lock();
		display_progress(progress, nr_resolved_deltas);
		counter_unlock();

		work_lock();
		எனில் (list_empty(&work_head)) {
			/*
			 * Take an object from the object array.
			 */
			போது (nr_dispatched < nr_objects &&
			       is_delta_type(objects[nr_dispatched].type))
				nr_dispatched++;
			எனில் (nr_dispatched >= nr_objects) {
				work_unlock();
				நிறுத்து;
			}
			child_obj = &objects[nr_dispatched++];
		} இல்லையெனில் {
			/*
			 * Peek at the top of the stack, and take a child from
			 * it.
			 */
			parent = list_first_entry(&work_head, கட்டமைப்பு base_data,
						  list);

			போது (parent->ref_first <= parent->ref_last) {
				முழு offset = ref_deltas[parent->ref_first++].obj_no;
				child_obj = objects + offset;
				எனில் (child_obj->real_type != OBJ_REF_DELTA) {
					child_obj = வெற்று;
					தொடர்;
				}
				child_obj->real_type = parent->obj->real_type;
				நிறுத்து;
			}

			எனில் (!child_obj && parent->ofs_first <= parent->ofs_last) {
				child_obj = objects +
					ofs_deltas[parent->ofs_first++].obj_no;
				ப்ரதிஜ்ஞா(child_obj->real_type == OBJ_OFS_DELTA);
				child_obj->real_type = parent->obj->real_type;
			}

			எனில் (parent->ref_first > parent->ref_last &&
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

		எனில் (child_obj) {
			எனில் (parent) {
				child = resolve_delta(child_obj, parent);
				எனில் (!child->children_remaining)
					FREE_AND_NULL(child->data);
			} இல்லையெனில்{
				child = make_base(child_obj, வெற்று);
				எனில் (child->children_remaining) {
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
		எனில் (parent)
			parent->retain_data--;

		எனில் (child && child->data) {
			/*
			 * This child has its own children, so add it to
			 * work_head.
			 */
			list_add(&child->list, &work_head);
			base_cache_used += child->size;
			prune_base_data(வெற்று);
		} இல்லையெனில் எனில் (child) {
			/*
			 * This child does not have its own children. It may be
			 * the last descendant of its ancestors; free those
			 * that we can.
			 */
			கட்டமைப்பு base_data *p = parent;

			போது (p) {
				கட்டமைப்பு base_data *next_p;

				p->children_remaining--;
				எனில் (p->children_remaining)
					நிறுத்து;

				next_p = p->base;
				free_base_data(p);
				list_del(&p->list);
				விடுவி(p);

				p = next_p;
			}
			FREE_AND_NULL(child);
		}
		work_unlock();
	}
	திரும்பு வெற்று;
}

/*
 * First pass:
 * - find locations of all objects;
 * - calculate SHA1 of all non-delta objects;
 * - remember base (SHA1 or offset) for all deltas.
 */
நிலையான வெறுமை parse_pack_objects(unsigned எழுத்து *hash)
{
	முழு i, nr_delays = 0;
	கட்டமைப்பு ofs_delta_entry *ofs_delta = ofs_deltas;
	கட்டமைப்பு object_id ref_delta_oid;
	கட்டமைப்பு stat st;
	கட்டமைப்பு git_hash_ctx tmp_ctx;

	எனில் (verbose)
		progress = start_progress(
				the_repository,
				progress_title ? progress_title :
				from_stdin ? _("Receiving objects") : _("Indexing objects"),
				nr_objects);
	க்கு (i = 0; i < nr_objects; i++) {
		கட்டமைப்பு object_entry *obj = &objects[i];
		வெறுமை *data = unpack_raw_entry(obj, &ofs_delta->offset,
					      &ref_delta_oid,
					      &obj->idx.oid);
		obj->real_type = obj->type;
		எனில் (obj->type == OBJ_OFS_DELTA) {
			nr_ofs_deltas++;
			ofs_delta->obj_no = i;
			ofs_delta++;
		} இல்லையெனில் எனில் (obj->type == OBJ_REF_DELTA) {
			ALLOC_GROW(ref_deltas, nr_ref_deltas + 1, ref_deltas_alloc);
			oidcpy(&ref_deltas[nr_ref_deltas].oid, &ref_delta_oid);
			ref_deltas[nr_ref_deltas].obj_no = i;
			nr_ref_deltas++;
		} இல்லையெனில் எனில் (!data) {
			/* large blobs, check later */
			obj->real_type = OBJ_BAD;
			nr_delays++;
		} இல்லையெனில்
			sha1_object(data, வெற்று, obj->size, obj->type,
				    &obj->idx.oid);
		விடுவி(data);
		display_progress(progress, i+1);
	}
	objects[i].idx.offset = consumed_bytes;
	stop_progress(&progress);

	/* Check pack integrity */
	flush();
	git_hash_init(&tmp_ctx, the_hash_algo);
	git_hash_clone(&tmp_ctx, &உள்ளீடு_ctx);
	git_hash_final(hash, &tmp_ctx);
	எனில் (!hasheq(fill(the_hash_algo->rawsz), hash, the_repository->hash_algo))
		die(_("pack is corrupted (SHA1 mismatch)"));
	use(the_hash_algo->rawsz);

	/* If input_fd is a file, we should have reached its end now. */
	எனில் (fstat(உள்ளீடு_fd, &st))
		die_errno(_("cannot fstat packfile"));
	எனில் (S_ISREG(st.st_mode) &&
			lseek(உள்ளீடு_fd, 0, SEEK_CUR) - உள்ளீடு_len != st.st_size)
		die(_("pack has junk at the end"));

	க்கு (i = 0; i < nr_objects; i++) {
		கட்டமைப்பு object_entry *obj = &objects[i];
		எனில் (obj->real_type != OBJ_BAD)
			தொடர்;
		obj->real_type = obj->type;
		sha1_object(வெற்று, obj, obj->size, obj->type,
			    &obj->idx.oid);
		nr_delays--;
	}
	எனில் (nr_delays)
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
நிலையான வெறுமை resolve_deltas(கட்டமைப்பு pack_idx_option *opts)
{
	முழு i;

	எனில் (!nr_ofs_deltas && !nr_ref_deltas)
		திரும்பு;

	/* Sort deltas by base SHA1/offset for fast searching */
	QSORT(ofs_deltas, nr_ofs_deltas, compare_ofs_delta_entry);
	QSORT(ref_deltas, nr_ref_deltas, compare_ref_delta_entry);

	எனில் (verbose || show_resolving_progress)
		progress = start_progress(the_repository,
					  _("Resolving deltas"),
					  nr_ref_deltas + nr_ofs_deltas);

	nr_dispatched = 0;
	base_cache_limit = opts->delta_base_cache_limit * nr_threads;
	எனில் (nr_threads > 1 || getenv("GIT_FORCE_THREADS")) {
		init_thread();
		க்கு (i = 0; i < nr_threads; i++) {
			முழு ret = pthread_create(&thread_data[i].thread, வெற்று,
						 threaded_second_pass, thread_data + i);
			எனில் (ret)
				die(_("unable to create thread: %s"),
				    strerror(ret));
		}
		க்கு (i = 0; i < nr_threads; i++)
			pthread_join(thread_data[i].thread, வெற்று);
		cleanup_thread();
		திரும்பு;
	}
	threaded_second_pass(&nothread_data);
}

/*
 * Third pass:
 * - append objects to convert thin pack to full pack if required
 * - write the final pack hash
 */
நிலையான வெறுமை fix_unresolved_deltas(கட்டமைப்பு hashfile *f);
நிலையான வெறுமை conclude_pack(முழு fix_thin_pack, மாறிலி எழுத்து *curr_pack, unsigned எழுத்து *pack_hash)
{
	எனில் (nr_ref_deltas + nr_ofs_deltas == nr_resolved_deltas) {
		stop_progress(&progress);
		/* Flush remaining pack final hash. */
		flush();
		திரும்பு;
	}

	எனில் (fix_thin_pack) {
		கட்டமைப்பு hashfile *f;
		unsigned எழுத்து read_hash[GIT_MAX_RAWSZ], tail_hash[GIT_MAX_RAWSZ];
		கட்டமைப்பு strbuf செய்தி = STRBUF_INIT;
		முழு nr_unresolved = nr_ofs_deltas + nr_ref_deltas - nr_resolved_deltas;
		முழு nr_objects_initial = nr_objects;
		எனில் (nr_unresolved <= 0)
			die(_("confusion beyond insanity"));
		REALLOC_ARRAY(objects, nr_objects + nr_unresolved + 1);
		memset(objects + nr_objects + 1, 0,
		       nr_unresolved * அளவு(*objects));
		f = hashfd(the_repository->hash_algo, output_fd, curr_pack);
		fix_unresolved_deltas(f);
		strbuf_addf(&செய்தி, Q_("completed with %d local object",
				     "completed with %d local objects",
				     nr_objects - nr_objects_initial),
			    nr_objects - nr_objects_initial);
		stop_progress_செய்தி(&progress, செய்தி.buf);
		strbuf_release(&செய்தி);
		finalize_hashfile(f, tail_hash, FSYNC_COMPONENT_PACK, 0);
		hashcpy(read_hash, pack_hash, the_repository->hash_algo);
		fixup_pack_header_footer(the_hash_algo, output_fd, pack_hash,
					 curr_pack, nr_objects,
					 read_hash, consumed_bytes-the_hash_algo->rawsz);
		எனில் (!hasheq(read_hash, tail_hash, the_repository->hash_algo))
			die(_("Unexpected tail checksum for %s "
			      "(disk corruption?)"), curr_pack);
	}
	எனில் (nr_ofs_deltas + nr_ref_deltas != nr_resolved_deltas)
		die(Q_("pack has %d unresolved delta",
		       "pack has %d unresolved deltas",
		       nr_ofs_deltas + nr_ref_deltas - nr_resolved_deltas),
		    nr_ofs_deltas + nr_ref_deltas - nr_resolved_deltas);
}

நிலையான முழு write_compressed(கட்டமைப்பு hashfile *f, வெறுமை *in, unsigned முழு size)
{
	git_zstream stream;
	முழு status;
	unsigned எழுத்து outbuf[4096];
	கட்டமைப்பு repo_config_மதிப்புகள் *cfg = repo_config_மதிப்புகள்(the_repository);

	git_deflate_init(&stream, cfg->zlib_compression_level);
	stream.next_in = in;
	stream.avail_in = size;

	செய் {
		stream.next_out = outbuf;
		stream.avail_out = அளவு(outbuf);
		status = git_deflate(&stream, Z_FINISH);
		hashwrite(f, outbuf, அளவு(outbuf) - stream.avail_out);
	} போது (status == Z_OK);

	எனில் (status != Z_STREAM_END)
		die(_("unable to deflate appended object (%d)"), status);
	size = stream.total_out;
	git_deflate_end(&stream);
	திரும்பு size;
}

நிலையான கட்டமைப்பு object_entry *append_obj_to_pack(கட்டமைப்பு hashfile *f,
			       மாறிலி unsigned எழுத்து *sha1, வெறுமை *buf,
			       unsigned long size, எண்ணிக்கை object_type type)
{
	கட்டமைப்பு object_entry *obj = &objects[nr_objects++];
	unsigned எழுத்து header[10];
	unsigned long s = size;
	முழு n = 0;
	unsigned எழுத்து c = (type << 4) | (s & 15);
	s >>= 4;
	போது (s) {
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
	திரும்பு obj;
}

நிலையான முழு delta_pos_compare(மாறிலி வெறுமை *_a, மாறிலி வெறுமை *_b)
{
	கட்டமைப்பு ref_delta_entry *a = *(கட்டமைப்பு ref_delta_entry **)_a;
	கட்டமைப்பு ref_delta_entry *b = *(கட்டமைப்பு ref_delta_entry **)_b;
	திரும்பு a->obj_no - b->obj_no;
}

நிலையான வெறுமை fix_unresolved_deltas(கட்டமைப்பு hashfile *f)
{
	கட்டமைப்பு ref_delta_entry **sorted_by_pos;
	முழு i;

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
	க்கு (i = 0; i < nr_ref_deltas; i++)
		sorted_by_pos[i] = &ref_deltas[i];
	QSORT(sorted_by_pos, nr_ref_deltas, delta_pos_compare);

	எனில் (repo_has_promisor_remote(the_repository)) {
		/*
		 * Prefetch the delta bases.
		 */
		கட்டமைப்பு oid_array to_fetch = OID_ARRAY_INIT;
		க்கு (i = 0; i < nr_ref_deltas; i++) {
			கட்டமைப்பு ref_delta_entry *d = sorted_by_pos[i];
			எனில் (!odb_read_object_info_extended(the_repository->objects,
							   &d->oid, வெற்று,
							   OBJECT_INFO_FOR_PREFETCH))
				தொடர்;
			oid_array_append(&to_fetch, &d->oid);
		}
		promisor_remote_get_direct(the_repository,
					   to_fetch.oid, to_fetch.nr);
		oid_array_clear(&to_fetch);
	}

	க்கு (i = 0; i < nr_ref_deltas; i++) {
		கட்டமைப்பு ref_delta_entry *d = sorted_by_pos[i];
		எண்ணிக்கை object_type type;
		வெறுமை *data;
		size_t size;

		எனில் (objects[d->obj_no].real_type != OBJ_REF_DELTA)
			தொடர்;
		data = odb_read_object(the_repository->objects, &d->oid,
				       &type, &size);
		எனில் (!data)
			தொடர்;

		எனில் (check_object_signature(the_repository, &d->oid, data, size,
					   type) < 0)
			die(_("local object %s is corrupt"), oid_to_hex(&d->oid));

		/*
		 * Add this as an object to the objects array and call
		 * threaded_second_pass() (which will pick up the added
		 * object).
		 */
		append_obj_to_pack(f, d->oid.hash, data, size, type);
		விடுவி(data);
		threaded_second_pass(வெற்று);

		display_progress(progress, nr_resolved_deltas);
	}
	விடுவி(sorted_by_pos);
}

நிலையான மாறிலி எழுத்து *derive_filename(மாறிலி எழுத்து *pack_name, மாறிலி எழுத்து *strip,
				   மாறிலி எழுத்து *suffix, கட்டமைப்பு strbuf *buf)
{
	size_t len;
	எனில் (!strip_suffix(pack_name, strip, &len) || !len ||
	    pack_name[len - 1] != '.')
		die(_("packfile name '%s' does not end with '.%s'"),
		    pack_name, strip);
	strbuf_add(buf, pack_name, len);
	strbuf_addstr(buf, suffix);
	திரும்பு buf->buf;
}

நிலையான வெறுமை write_special_file(மாறிலி எழுத்து *suffix, மாறிலி எழுத்து *செய்தி,
			       மாறிலி எழுத்து *pack_name, மாறிலி unsigned எழுத்து *hash,
			       மாறிலி எழுத்து **report)
{
	கட்டமைப்பு strbuf name_buf = STRBUF_INIT;
	மாறிலி எழுத்து *filename;
	முழு fd;
	முழு செய்தி_len = strlen(செய்தி);

	எனில் (pack_name)
		filename = derive_filename(pack_name, "pack", suffix, &name_buf);
	இல்லையெனில்
		filename = odb_pack_name(the_repository, &name_buf, hash, suffix);

	fd = safe_create_file_with_leading_directories(the_repository, filename);
	எனில் (fd < 0) {
		எனில் (errno != EEXIST)
			die_errno(_("cannot write %s file '%s'"),
				  suffix, filename);
	} இல்லையெனில் {
		எனில் (செய்தி_len > 0) {
			write_or_die(fd, செய்தி, செய்தி_len);
			write_or_die(fd, "\n", 1);
		}
		எனில் (close(fd) != 0)
			die_errno(_("cannot close written %s file '%s'"),
				  suffix, filename);
		எனில் (report)
			*report = suffix;
	}
	strbuf_release(&name_buf);
}

நிலையான வெறுமை rename_tmp_packfile(மாறிலி எழுத்து **final_name,
				மாறிலி எழுத்து *curr_name,
				கட்டமைப்பு strbuf *name, unsigned எழுத்து *hash,
				மாறிலி எழுத்து *ext, முழு make_read_only_if_same)
{
	எனில் (!*final_name || strcmp(*final_name, curr_name)) {
		எனில் (!*final_name)
			*final_name = odb_pack_name(the_repository, name, hash, ext);
		எனில் (finalize_object_file(the_repository, curr_name, *final_name))
			die(_("unable to rename temporary '*.%s' file to '%s'"),
			    ext, *final_name);
	} இல்லையெனில் எனில் (make_read_only_if_same) {
		chmod(*final_name, 0444);
	}
}

நிலையான வெறுமை final(மாறிலி எழுத்து *final_pack_name, மாறிலி எழுத்து *curr_pack_name,
		  மாறிலி எழுத்து *final_index_name, மாறிலி எழுத்து *curr_index_name,
		  மாறிலி எழுத்து *final_rev_index_name, மாறிலி எழுத்து *curr_rev_index_name,
		  மாறிலி எழுத்து *keep_செய்தி, மாறிலி எழுத்து *promisor_செய்தி,
		  unsigned எழுத்து *hash)
{
	மாறிலி எழுத்து *report = "pack";
	கட்டமைப்பு strbuf pack_name = STRBUF_INIT;
	கட்டமைப்பு strbuf index_name = STRBUF_INIT;
	கட்டமைப்பு strbuf rev_index_name = STRBUF_INIT;

	எனில் (!from_stdin) {
		close(உள்ளீடு_fd);
	} இல்லையெனில் {
		fsync_component_or_die(FSYNC_COMPONENT_PACK, output_fd, curr_pack_name);
		எனில் (close(output_fd))
			die_errno(_("error while closing pack file"));
	}

	எனில் (keep_செய்தி)
		write_special_file("keep", keep_செய்தி, final_pack_name, hash,
				   &report);
	எனில் (promisor_செய்தி)
		write_special_file("promisor", promisor_செய்தி, final_pack_name,
				   hash, வெற்று);

	rename_tmp_packfile(&final_pack_name, curr_pack_name, &pack_name,
			    hash, "pack", from_stdin);
	எனில் (curr_rev_index_name)
		rename_tmp_packfile(&final_rev_index_name, curr_rev_index_name,
				    &rev_index_name, hash, "rev", 1);
	rename_tmp_packfile(&final_index_name, curr_index_name, &index_name,
			    hash, "idx", 1);

	எனில் (do_fsck_object && startup_info->have_repository) {
		கட்டமைப்பு odb_source_files *files =
			odb_source_files_downcast(the_repository->objects->source);
		packfile_store_load_pack(files->dirs->packed, final_index_name, 0);
	}

	எனில் (!from_stdin) {
		printf("%s\n", hash_to_hex(hash));
	} இல்லையெனில் {
		கட்டமைப்பு strbuf buf = STRBUF_INIT;

		strbuf_addf(&buf, "%s\t%s\n", report, hash_to_hex(hash));
		write_or_die(1, buf.buf, buf.len);
		strbuf_release(&buf);

		/* Write the last part of the buffer to stdout */
		write_in_full(1, உள்ளீடு_buffer + உள்ளீடு_offset, உள்ளீடு_len);
	}

	strbuf_release(&rev_index_name);
	strbuf_release(&index_name);
	strbuf_release(&pack_name);
}

நிலையான முழு git_index_pack_config(மாறிலி எழுத்து *k, மாறிலி எழுத்து *v,
				 மாறிலி கட்டமைப்பு config_context *ctx, வெறுமை *cb)
{
	கட்டமைப்பு pack_idx_option *opts = cb;

	எனில் (!strcmp(k, "pack.indexversion")) {
		opts->version = git_config_int(k, v, ctx->kvi);
		எனில் (opts->version > 2)
			die(_("bad pack.indexVersion=%"PRIu32), opts->version);
		திரும்பு 0;
	}
	எனில் (!strcmp(k, "pack.threads")) {
		nr_threads = git_config_int(k, v, ctx->kvi);
		எனில் (nr_threads < 0)
			die(_("invalid number of threads specified (%d)"),
			    nr_threads);
		எனில் (!HAVE_THREADS && nr_threads != 1) {
			warning(_("no threads support, ignoring %s"), k);
			nr_threads = 1;
		}
		திரும்பு 0;
	}
	எனில் (!strcmp(k, "pack.writereverseindex")) {
		எனில் (git_config_bool(k, v))
			opts->flags |= WRITE_REV;
		இல்லையெனில்
			opts->flags &= ~WRITE_REV;
	}
	எனில் (!strcmp(k, "core.deltabasecachelimit")) {
		opts->delta_base_cache_limit = git_config_ulong(k, v, ctx->kvi);
		திரும்பு 0;
	}
	திரும்பு git_default_config(k, v, ctx, cb);
}

நிலையான முழு cmp_uint32(மாறிலி வெறுமை *a_, மாறிலி வெறுமை *b_)
{
	uint32_t a = *((uint32_t *)a_);
	uint32_t b = *((uint32_t *)b_);

	திரும்பு (a < b) ? -1 : (a != b);
}

நிலையான வெறுமை read_v2_anomalous_offsets(கட்டமைப்பு packed_git *p,
				      கட்டமைப்பு pack_idx_option *opts)
{
	மாறிலி uint32_t *idx1, *idx2;
	uint32_t i;

	/* The address of the 4-byte offset table */
	idx1 = (((மாறிலி uint32_t *)((மாறிலி uint8_t *)p->index_data + p->crc_offset))
		+ (size_t)p->இலக்கம்_objects /* CRC32 table */
		);

	/* The address of the 8-byte offset table */
	idx2 = idx1 + p->இலக்கம்_objects;

	க்கு (i = 0; i < p->இலக்கம்_objects; i++) {
		uint32_t off = ntohl(idx1[i]);
		எனில் (!(off & 0x80000000))
			தொடர்;
		off = off & 0x7fffffff;
		check_pack_index_ptr(p, &idx2[off * 2]);
		எனில் (idx2[off * 2])
			தொடர்;
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

நிலையான வெறுமை read_idx_option(கட்டமைப்பு pack_idx_option *opts, மாறிலி எழுத்து *pack_name)
{
	கட்டமைப்பு packed_git *p = add_packed_git(the_repository, pack_name,
					      strlen(pack_name), 1);

	எனில் (!p)
		die(_("Cannot open existing pack file '%s'"), pack_name);
	எனில் (open_pack_index(p))
		die(_("Cannot open existing pack idx file for '%s'"), pack_name);

	/* Read the attributes from the existing idx file */
	opts->version = p->index_version;

	எனில் (opts->version == 2)
		read_v2_anomalous_offsets(p, opts);

	/*
	 * Get rid of the idx file as we do not need it anymore.
	 * NEEDSWORK: extract this bit from free_pack_by_name() in
	 * object-file.c, perhaps?  It shouldn't matter very much as we
	 * know we haven't installed this pack (hence we never have
	 * read anything from it).
	 */
	close_pack_index(p);
	விடுவி(p);
}

நிலையான வெறுமை show_pack_info(முழு stat_only)
{
	முழு i, baseobjects = nr_objects - nr_ref_deltas - nr_ofs_deltas;
	unsigned long *chain_histogram = வெற்று;

	எனில் (deepest_delta)
		CALLOC_ARRAY(chain_histogram, deepest_delta);

	க்கு (i = 0; i < nr_objects; i++) {
		கட்டமைப்பு object_entry *obj = &objects[i];

		எனில் (is_delta_type(obj->type))
			chain_histogram[obj_stat[i].delta_depth - 1]++;
		எனில் (stat_only)
			தொடர்;
		printf("%s %-6s %"PRIuMAX" %"PRIuMAX" %"PRIuMAX,
		       oid_to_hex(&obj->idx.oid),
		       type_name(obj->real_type), (uintmax_t)obj->size,
		       (uintmax_t)(obj[1].idx.offset - obj->idx.offset),
		       (uintmax_t)obj->idx.offset);
		எனில் (is_delta_type(obj->type)) {
			கட்டமைப்பு object_entry *bobj = &objects[obj_stat[i].base_object_no];
			printf(" %u %s", obj_stat[i].delta_depth,
			       oid_to_hex(&bobj->idx.oid));
		}
		putchar('\n');
	}

	எனில் (baseobjects)
		printf_ln(Q_("non delta: %d object",
			     "non delta: %d objects",
			     baseobjects),
			  baseobjects);
	க்கு (i = 0; i < deepest_delta; i++) {
		எனில் (!chain_histogram[i])
			தொடர்;
		printf_ln(Q_("chain length = %d: %lu object",
			     "chain length = %d: %lu objects",
			     chain_histogram[i]),
			  i + 1,
			  chain_histogram[i]);
	}
	விடுவி(chain_histogram);
}

நிலையான வெறுமை repack_local_links(வெறுமை)
{
	கட்டமைப்பு child_process cmd = CHILD_PROCESS_INIT;
	FILE *out;
	கட்டமைப்பு strbuf வரி = STRBUF_INIT;
	கட்டமைப்பு oidset_iter iter;
	கட்டமைப்பு object_id *oid;
	எழுத்து *base_name = வெற்று;

	எனில் (!oidset_size(&outgoing_links))
		திரும்பு;

	oidset_iter_init(&outgoing_links, &iter);
	போது ((oid = oidset_iter_next(&iter))) {
		கட்டமைப்பு odb_source_info source_info;
		கட்டமைப்பு object_info info = {
			.source_infop = &source_info,
		};

		எனில் (odb_read_object_info_extended(the_repository->objects, oid, &info, 0))
			/* Missing; assume it is a promisor object */
			தொடர்;
		எனில் (source_info.source->type == ODB_SOURCE_PACKED &&
		    source_info.u.packed.pack->pack_promisor)
			தொடர்;

		எனில் (!cmd.args.nr) {
			base_name = mkpathdup(
				"%s/pack/pack",
				repo_get_object_directory(the_repository));
			strvec_தள்ளு(&cmd.args, "pack-objects");
			strvec_தள்ளு(&cmd.args,
				    "--exclude-promisor-objects-best-effort");
			strvec_தள்ளு(&cmd.args, base_name);
			cmd.git_cmd = 1;
			cmd.in = -1;
			cmd.out = -1;
			எனில் (start_command(&cmd))
				die(_("could not start pack-objects to repack local links"));
		}

		எனில் (write_in_full(cmd.in, oid_to_hex(oid), the_hash_algo->hexsz) < 0 ||
		    write_in_full(cmd.in, "\n", 1) < 0)
			die(_("failed to feed local object to pack-objects"));
	}

	எனில் (!cmd.args.nr)
		திரும்பு;

	close(cmd.in);

	out = xfdopen(cmd.out, "r");
	போது (strbuf_getline_lf(&வரி, out) != EOF) {
		unsigned எழுத்து இருமம்[GIT_MAX_RAWSZ];
		எனில் (வரி.len != the_hash_algo->hexsz ||
		    !hex_to_bytes(இருமம், வரி.buf, வரி.len))
			die(_("index-pack: Expecting full hex object ID lines only from pack-objects."));

		/*
		 * pack-objects creates the .pack and .idx files, but not the
		 * .promisor file. Create the .promisor file, which is empty.
		 */
		write_special_file("promisor", "", வெற்று, இருமம், வெற்று);
	}

	fclose(out);
	எனில் (finish_command(&cmd))
		die(_("could not finish pack-objects to repack local links"));
	strbuf_release(&வரி);
	விடுவி(base_name);
}

முழு cmd_index_pack(முழு argc,
		   மாறிலி எழுத்து **argv,
		   மாறிலி எழுத்து *prefix,
		   கட்டமைப்பு repository *repo)
{
	முழு i, fix_thin_pack = 0, verify = 0, stat_only = 0, rev_index;
	மாறிலி எழுத்து *curr_index;
	எழுத்து *curr_rev_index = வெற்று;
	மாறிலி எழுத்து *index_name = வெற்று, *pack_name = வெற்று, *rev_index_name = வெற்று;
	மாறிலி எழுத்து *keep_செய்தி = வெற்று;
	மாறிலி எழுத்து *promisor_செய்தி = வெற்று;
	கட்டமைப்பு strbuf index_name_buf = STRBUF_INIT;
	கட்டமைப்பு strbuf rev_index_name_buf = STRBUF_INIT;
	கட்டமைப்பு pack_idx_entry **idx_objects;
	கட்டமைப்பு pack_idx_option opts;
	unsigned எழுத்து pack_hash[GIT_MAX_RAWSZ];
	unsigned foreign_nr = 1;	/* zero is a "good" value, assume bad */
	முழு report_end_of_உள்ளீடு = 0;
	முழு hash_algo = 0;

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
	எனில் (prefix && chdir(prefix))
		die(_("Cannot come back to cwd"));

	எனில் (git_env_bool(GIT_TEST_NO_WRITE_REV_INDEX, 0))
		rev_index = 0;
	இல்லையெனில்
		rev_index = !!(opts.flags & (WRITE_REV_VERIFY | WRITE_REV));

	க்கு (i = 1; i < argc; i++) {
		மாறிலி எழுத்து *arg = argv[i];

		எனில் (*arg == '-') {
			எனில் (!strcmp(arg, "--stdin")) {
				from_stdin = 1;
			} இல்லையெனில் எனில் (!strcmp(arg, "--fix-thin")) {
				fix_thin_pack = 1;
			} இல்லையெனில் எனில் (skip_to_optional_arg(arg, "--strict", &arg)) {
				strict = 1;
				do_fsck_object = 1;
				fsck_set_செய்தி_types(&fsck_options, arg);
			} இல்லையெனில் எனில் (!strcmp(arg, "--check-self-contained-and-connected")) {
				strict = 1;
				check_self_contained_and_connected = 1;
			} இல்லையெனில் எனில் (skip_to_optional_arg(arg, "--fsck-objects", &arg)) {
				do_fsck_object = 1;
				fsck_set_செய்தி_types(&fsck_options, arg);
			} இல்லையெனில் எனில் (!strcmp(arg, "--verify")) {
				verify = 1;
			} இல்லையெனில் எனில் (!strcmp(arg, "--verify-stat")) {
				verify = 1;
				show_stat = 1;
			} இல்லையெனில் எனில் (!strcmp(arg, "--verify-stat-only")) {
				verify = 1;
				show_stat = 1;
				stat_only = 1;
			} இல்லையெனில் எனில் (skip_to_optional_arg(arg, "--keep", &keep_செய்தி)) {
				; /* nothing to do */
			} இல்லையெனில் எனில் (skip_to_optional_arg(arg, "--promisor", &promisor_செய்தி)) {
				record_outgoing_links = 1;
			} இல்லையெனில் எனில் (starts_with(arg, "--threads=")) {
				எழுத்து *end;
				nr_threads = strtoul(arg+10, &end, 0);
				எனில் (!arg[10] || *end || nr_threads < 0)
					usage(index_pack_usage);
				எனில் (!HAVE_THREADS && nr_threads != 1) {
					warning(_("no threads support, ignoring %s"), arg);
					nr_threads = 1;
				}
			} இல்லையெனில் எனில் (skip_prefix(arg, "--pack_header=", &arg)) {
				எனில் (parse_pack_header_option(arg,
							     உள்ளீடு_buffer,
							     &உள்ளீடு_len) < 0)
					die(_("bad --pack_header: %s"), arg);
			} இல்லையெனில் எனில் (!strcmp(arg, "-v")) {
				verbose = 1;
			} இல்லையெனில் எனில் (!strcmp(arg, "--progress-title")) {
				எனில் (progress_title || (i+1) >= argc)
					usage(index_pack_usage);
				progress_title = argv[++i];
			} இல்லையெனில் எனில் (!strcmp(arg, "--show-resolving-progress")) {
				show_resolving_progress = 1;
			} இல்லையெனில் எனில் (!strcmp(arg, "--report-end-of-input")) {
				report_end_of_உள்ளீடு = 1;
			} இல்லையெனில் எனில் (!strcmp(arg, "-o")) {
				எனில் (index_name || (i+1) >= argc)
					usage(index_pack_usage);
				index_name = argv[++i];
			} இல்லையெனில் எனில் (starts_with(arg, "--index-version=")) {
				எழுத்து *c;
				opts.version = strtoul(arg + 16, &c, 10);
				எனில் (opts.version > 2)
					die(_("bad %s"), arg);
				எனில் (*c == ',')
					opts.off32_limit = strtoul(c+1, &c, 0);
				எனில் (*c || opts.off32_limit & 0x80000000)
					die(_("bad %s"), arg);
			} இல்லையெனில் எனில் (skip_prefix(arg, "--max-input-size=", &arg)) {
				max_உள்ளீடு_size = strtoumax(arg, வெற்று, 10);
			} இல்லையெனில் எனில் (skip_prefix(arg, "--object-format=", &arg)) {
				hash_algo = hash_algo_by_name(arg);
				எனில் (hash_algo == GIT_HASH_UNKNOWN)
					die(_("unknown hash algorithm '%s'"), arg);
				repo_set_hash_algo(the_repository, hash_algo);
			} இல்லையெனில் எனில் (!strcmp(arg, "--rev-index")) {
				rev_index = 1;
			} இல்லையெனில் எனில் (!strcmp(arg, "--no-rev-index")) {
				rev_index = 0;
			} இல்லையெனில்
				usage(index_pack_usage);
			தொடர்;
		}

		எனில் (pack_name)
			usage(index_pack_usage);
		pack_name = arg;
	}

	எனில் (!pack_name && !from_stdin)
		usage(index_pack_usage);
	எனில் (fix_thin_pack && !from_stdin)
		die(_("the option '%s' requires '%s'"), "--fix-thin", "--stdin");
	எனில் (promisor_செய்தி && pack_name)
		die(_("--promisor cannot be used with a pack name"));
	எனில் (from_stdin && !startup_info->have_repository)
		die(_("--stdin requires a git repository"));
	எனில் (from_stdin && hash_algo)
		die(_("options '%s' and '%s' cannot be used together"), "--object-format", "--stdin");
	எனில் (!index_name && pack_name)
		index_name = derive_filename(pack_name, "pack", "idx", &index_name_buf);

	/*
	 * Packfiles and indices do not carry enough information to be able to
	 * identify their object hash. So when we are neither in a repository
	 * nor has the user told us which object hash to use we have no other
	 * choice but to guess the object hash.
	 */
	எனில் (!the_repository->hash_algo)
		repo_set_hash_algo(the_repository, GIT_HASH_DEFAULT);

	opts.flags &= ~(WRITE_REV | WRITE_REV_VERIFY);
	எனில் (rev_index) {
		opts.flags |= verify ? WRITE_REV_VERIFY : WRITE_REV;
		எனில் (index_name)
			rev_index_name = derive_filename(index_name,
							 "idx", "rev",
							 &rev_index_name_buf);
	}

	எனில் (verify) {
		எனில் (!index_name)
			die(_("--verify with no packfile name given"));
		read_idx_option(&opts, index_name);
		opts.flags |= WRITE_IDX_VERIFY | WRITE_IDX_STRICT;
	}
	எனில் (strict)
		opts.flags |= WRITE_IDX_STRICT;

	எனில் (HAVE_THREADS && !nr_threads) {
		nr_threads = online_cpus();
		/*
		 * Experiments show that going above 20 threads doesn't help,
		 * no matter how many cores you have. Below that, we tend to
		 * max at half the number of online_cpus(), presumably because
		 * half of those are hyperthreads rather than full cores. We'll
		 * never reduce the level below "3", though, to match a
		 * historical value that nobody complained about.
		 */
		எனில் (nr_threads < 4)
			; /* too few cores to consider capping */
		இல்லையெனில் எனில் (nr_threads < 6)
			nr_threads = 3; /* historic cap */
		இல்லையெனில் எனில் (nr_threads < 40)
			nr_threads /= 2;
		இல்லையெனில்
			nr_threads = 20; /* hard cap */
	}

	curr_pack = open_pack_file(pack_name);
	parse_pack_header();
	CALLOC_ARRAY(objects, st_add(nr_objects, 1));
	எனில் (show_stat)
		CALLOC_ARRAY(obj_stat, st_add(nr_objects, 1));
	CALLOC_ARRAY(ofs_deltas, nr_objects);
	parse_pack_objects(pack_hash);
	எனில் (report_end_of_உள்ளீடு)
		write_in_full(2, "\0", 1);
	resolve_deltas(&opts);
	conclude_pack(fix_thin_pack, curr_pack, pack_hash);
	விடுவி(ofs_deltas);
	விடுவி(ref_deltas);
	எனில் (strict)
		foreign_nr = check_objects();

	எனில் (show_stat)
		show_pack_info(stat_only);

	ALLOC_ARRAY(idx_objects, nr_objects);
	க்கு (i = 0; i < nr_objects; i++)
		idx_objects[i] = &objects[i].idx;
	curr_index = write_idx_file(the_repository, index_name, idx_objects,
				    nr_objects, &opts, pack_hash);
	எனில் (rev_index)
		curr_rev_index = write_rev_file(the_repository, rev_index_name,
						idx_objects, nr_objects,
						pack_hash, opts.flags);
	விடுவி(idx_objects);

	எனில் (!verify)
		final(pack_name, curr_pack,
		      index_name, curr_index,
		      rev_index_name, curr_rev_index,
		      keep_செய்தி, promisor_செய்தி,
		      pack_hash);
	இல்லையெனில்
		close(உள்ளீடு_fd);

	எனில் (do_fsck_object) {
		/*
		 * We cannot perform queued consistency checks when running
		 * outside of a repository because those require us to read
		 * from the object database, which is uninitialized.
		 *
		 * TODO: we may eventually set up an in-memory object database,
		 * which would allow us to perform these queued checks.
		 */
		எனில் (!startup_info->have_repository &&
		    fsck_has_queued_checks(&fsck_options))
			die(_("cannot perform queued object checks outside "
			      "of a repository"));

		எனில் (fsck_finish(&fsck_options))
			die(_("fsck error in pack objects"));
	}

	விடுவி(opts.anomaly);
	விடுவி(objects);
	strbuf_release(&index_name_buf);
	strbuf_release(&rev_index_name_buf);
	எனில் (!pack_name)
		விடுவி((வெறுமை *) curr_pack);
	எனில் (!index_name)
		விடுவி((வெறுமை *) curr_index);
	விடுவி(curr_rev_index);

	repack_local_links();

	/*
	 * Let the caller know this pack is not self contained
	 */
	எனில் (check_self_contained_and_connected && foreign_nr)
		திரும்பு 1;

	திரும்பு 0;
}
