#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#অন্তর্ভাৱ "builtin.h"
#অন্তর্ভাৱ "config.h"
#অন্তর্ভাৱ "delta.h"
#অন্তর্ভাৱ "environment.h"
#অন্তর্ভাৱ "gettext.h"
#অন্তর্ভাৱ "hex.h"
#অন্তর্ভাৱ "pack.h"
#অন্তর্ভাৱ "csum-file.h"
#অন্তর্ভাৱ "blob.h"
#অন্তর্ভাৱ "commit.h"
#অন্তর্ভাৱ "tag.h"
#অন্তর্ভাৱ "tree.h"
#অন্তর্ভাৱ "progress.h"
#অন্তর্ভাৱ "fsck.h"
#অন্তর্ভাৱ "strbuf.h"
#অন্তর্ভাৱ "thread-utils.h"
#অন্তর্ভাৱ "packfile.h"
#অন্তর্ভাৱ "pack-revindex.h"
#অন্তর্ভাৱ "object-file.h"
#অন্তর্ভাৱ "odb.h"
#অন্তর্ভাৱ "odb/streaming.h"
#অন্তর্ভাৱ "oid-array.h"
#অন্তর্ভাৱ "oidset.h"
#অন্তর্ভাৱ "path.h"
#অন্তর্ভাৱ "replace-object.h"
#অন্তর্ভাৱ "tree-walk.h"
#অন্তর্ভাৱ "promisor-remote.h"
#অন্তর্ভাৱ "run-command.h"
#অন্তর্ভাৱ "setup.h"
#অন্তর্ভাৱ "strvec.h"

স্থির ধ্রুৱ ৱর্ণ index_pack_usage[] =
"git index-pack [-v] [-o <index-file>] [--keep | --keep=<msg>] [--[no-]rev-index] [--verify] [--strict[=<msg-id>=<severity>...]] [--fsck-objects[=<msg-id>=<severity>...]] (<pack-file> | --stdin [--fix-thin] [<pack-file>])";

সংরচনা object_entry {
	সংরচনা pack_idx_entry idx;
	size_t size;
	unsigned ৱর্ণ hdr_size;
	signed ৱর্ণ type;
	signed ৱর্ণ real_type;
};

সংরচনা object_stat {
	unsigned delta_depth;
	পূর্ণাঙ্ক base_object_no;
};

সংরচনা base_data {
	/* Initialized by make_base(). */
	সংরচনা base_data *base;
	সংরচনা object_entry *obj;
	পূর্ণাঙ্ক ref_first, ref_last;
	পূর্ণাঙ্ক ofs_first, ofs_last;
	/*
	 * Threads should increment retain_data if they are about to call
	 * patch_delta() using this struct's data as a base, and decrement this
	 * when they are done. While retain_data is nonzero, this struct's data
	 * will not be freed even if the delta base cache limit is exceeded.
	 */
	পূর্ণাঙ্ক retain_data;
	/*
	 * The number of direct children that have not been fully processed
	 * (entered work_head, entered done_head, left done_head). When this
	 * number reaches zero, this struct base_data can be freed.
	 */
	পূর্ণাঙ্ক children_remaining;

	/* Not initialized by make_base(). */
	সংরচনা list_head list;
	শূন্য *data;
	size_t size;
};

/*
 * Stack of struct base_data that have unprocessed children.
 * threaded_second_pass() uses this as a source of work (the other being the
 * objects array).
 *
 * Guarded by work_mutex.
 */
স্থির LIST_HEAD(work_head);

/*
 * Stack of struct base_data that have children, all of whom have been
 * processed or are being processed, and at least one child is being processed.
 * These struct base_data must be kept around until the last child is
 * processed.
 *
 * Guarded by work_mutex.
 */
স্থির LIST_HEAD(done_head);

/*
 * All threads share one delta base cache.
 *
 * base_cache_used is guarded by work_mutex, and base_cache_limit is read-only
 * in a thread.
 */
স্থির size_t base_cache_used;
স্থির size_t base_cache_limit;

সংরচনা thread_local_data {
	pthread_t thread;
	পূর্ণাঙ্ক pack_fd;
};

/* Remember to update object flag allocation in object.h */
#define FLAG_LINK (1u<<20)
#define FLAG_CHECKED (1u<<21)

সংরচনা ofs_delta_entry {
	off_t offset;
	পূর্ণাঙ্ক obj_no;
};

সংরচনা ref_delta_entry {
	সংরচনা object_id oid;
	পূর্ণাঙ্ক obj_no;
};

স্থির সংরচনা object_entry *objects;
স্থির সংরচনা object_stat *obj_stat;
স্থির সংরচনা ofs_delta_entry *ofs_deltas;
স্থির সংরচনা ref_delta_entry *ref_deltas;
স্থির সংরচনা thread_local_data nothread_data;
স্থির পূর্ণাঙ্ক nr_objects;
স্থির পূর্ণাঙ্ক nr_ofs_deltas;
স্থির পূর্ণাঙ্ক nr_ref_deltas;
স্থির পূর্ণাঙ্ক ref_deltas_alloc;
স্থির পূর্ণাঙ্ক nr_resolved_deltas;
স্থির পূর্ণাঙ্ক nr_threads;

স্থির পূর্ণাঙ্ক from_stdin;
স্থির পূর্ণাঙ্ক strict;
স্থির পূর্ণাঙ্ক do_fsck_object;
স্থির সংরচনা fsck_options fsck_options;
স্থির পূর্ণাঙ্ক verbose;
স্থির ধ্রুৱ ৱর্ণ *progress_title;
স্থির পূর্ণাঙ্ক show_resolving_progress;
স্থির পূর্ণাঙ্ক show_stat;
স্থির পূর্ণাঙ্ক check_self_contained_and_connected;

স্থির সংরচনা progress *progress;

স্থির unsigned ৱর্ণ প্ৰবেশ_buffer[DEFAULT_IO_BUFFER_SIZE];
স্থির unsigned পূর্ণাঙ্ক প্ৰবেশ_offset, প্ৰবেশ_len;
স্থির off_t consumed_bytes;
স্থির off_t max_প্ৰবেশ_size;
স্থির unsigned deepest_delta;
স্থির সংরচনা git_hash_ctx প্ৰবেশ_ctx;
স্থির uint32_t প্ৰবেশ_crc32;
স্থির পূর্ণাঙ্ক প্ৰবেশ_fd, output_fd;
স্থির ধ্রুৱ ৱর্ণ *curr_pack;

/*
 * outgoing_links is guarded by read_mutex, and record_outgoing_links is
 * read-only in a thread.
 */
স্থির সংরচনা oidset outgoing_links = OIDSET_INIT;
স্থির পূর্ণাঙ্ক record_outgoing_links;

স্থির সংরচনা thread_local_data *thread_data;
স্থির পূর্ণাঙ্ক nr_dispatched;
স্থির পূর্ণাঙ্ক threads_active;

স্থির pthread_mutex_t read_mutex;
#define read_lock()		lock_mutex(&read_mutex)
#define read_unlock()		unlock_mutex(&read_mutex)

স্থির pthread_mutex_t counter_mutex;
#define counter_lock()		lock_mutex(&counter_mutex)
#define counter_unlock()	unlock_mutex(&counter_mutex)

স্থির pthread_mutex_t work_mutex;
#define work_lock()		lock_mutex(&work_mutex)
#define work_unlock()		unlock_mutex(&work_mutex)

স্থির pthread_mutex_t deepest_delta_mutex;
#define deepest_delta_lock()	lock_mutex(&deepest_delta_mutex)
#define deepest_delta_unlock()	unlock_mutex(&deepest_delta_mutex)

স্থির pthread_চাবি_t চাবি;

স্থির অন্তঃস্থ শূন্য lock_mutex(pthread_mutex_t *mutex)
{
	যদি (threads_active)
		pthread_mutex_lock(mutex);
}

স্থির অন্তঃস্থ শূন্য unlock_mutex(pthread_mutex_t *mutex)
{
	যদি (threads_active)
		pthread_mutex_unlock(mutex);
}

/*
 * Mutex and conditional variable can't be statically-initialized on Windows.
 */
স্থির শূন্য init_thread(শূন্য)
{
	পূর্ণাঙ্ক i;
	init_recursive_mutex(&read_mutex);
	pthread_mutex_init(&counter_mutex, রিক্ত);
	pthread_mutex_init(&work_mutex, রিক্ত);
	যদি (show_stat)
		pthread_mutex_init(&deepest_delta_mutex, রিক্ত);
	pthread_চাবি_create(&চাবি, রিক্ত);
	CALLOC_ARRAY(thread_data, nr_threads);
	বাবে (i = 0; i < nr_threads; i++) {
		thread_data[i].pack_fd = xopen(curr_pack, O_RDONLY);
	}

	threads_active = 1;
}

স্থির শূন্য cleanup_thread(শূন্য)
{
	পূর্ণাঙ্ক i;
	যদি (!threads_active)
		ঘূৰাই;
	threads_active = 0;
	pthread_mutex_destroy(&read_mutex);
	pthread_mutex_destroy(&counter_mutex);
	pthread_mutex_destroy(&work_mutex);
	যদি (show_stat)
		pthread_mutex_destroy(&deepest_delta_mutex);
	বাবে (i = 0; i < nr_threads; i++)
		close(thread_data[i].pack_fd);
	pthread_চাবি_delete(চাবি);
	মুক্ত(thread_data);
}

স্থির পূর্ণাঙ্ক mark_link(সংরচনা object *obj, গণনা object_type type,
		     শূন্য *data UNUSED,
		     সংরচনা fsck_options *options UNUSED)
{
	যদি (!obj)
		ঘূৰাই -1;

	যদি (type != OBJ_ANY && obj->type != type)
		die(_("object type mismatch at %s"), oid_to_hex(&obj->oid));

	obj->flags |= FLAG_LINK;
	ঘূৰাই 0;
}

/* The content of each linked object must have been checked
   or it must be already present in the object database */
স্থির unsigned check_object(সংরচনা object *obj)
{
	যদি (!obj)
		ঘূৰাই 0;

	যদি (!(obj->flags & FLAG_LINK))
		ঘূৰাই 0;

	যদি (!(obj->flags & FLAG_CHECKED)) {
		size_t size;
		পূর্ণাঙ্ক type = odb_read_object_info(the_repository->objects,
						&obj->oid, &size);
		যদি (type <= 0)
			die(_("did not receive expected object %s"),
			      oid_to_hex(&obj->oid));
		যদি (type != obj->type)
			die(_("object %s: expected type %s, found %s"),
			    oid_to_hex(&obj->oid),
			    type_name(obj->type), type_name(type));
		obj->flags |= FLAG_CHECKED;
		ঘূৰাই 1;
	}

	ঘূৰাই 0;
}

স্থির unsigned check_objects(শূন্য)
{
	unsigned i, max, foreign_nr = 0;

	max = get_max_object_index(the_repository);

	যদি (verbose)
		progress = start_delayed_progress(the_repository,
						  _("Checking objects"), max);

	বাবে (i = 0; i < max; i++) {
		foreign_nr += check_object(get_indexed_object(the_repository, i));
		display_progress(progress, i + 1);
	}

	stop_progress(&progress);
	ঘূৰাই foreign_nr;
}


/* Discard current buffer used content. */
স্থির শূন্য flush(শূন্য)
{
	যদি (প্ৰবেশ_offset) {
		যদি (output_fd >= 0)
			write_or_die(output_fd, প্ৰবেশ_buffer, প্ৰবেশ_offset);
		git_hash_update(&প্ৰবেশ_ctx, প্ৰবেশ_buffer, প্ৰবেশ_offset);
		memmove(প্ৰবেশ_buffer, প্ৰবেশ_buffer + প্ৰবেশ_offset, প্ৰবেশ_len);
		প্ৰবেশ_offset = 0;
	}
}

/*
 * Make sure at least "min" bytes are available in the buffer, and
 * return the pointer to the buffer.
 */
স্থির শূন্য *fill(পূর্ণাঙ্ক min)
{
	যদি (min <= প্ৰবেশ_len)
		ঘূৰাই প্ৰবেশ_buffer + প্ৰবেশ_offset;
	যদি (min > পরিমাণ(প্ৰবেশ_buffer))
		die(Q_("cannot fill %d byte",
		       "cannot fill %d bytes",
		       min),
		    min);
	flush();
	কৰা {
		ssize_t ret = xread(প্ৰবেশ_fd, প্ৰবেশ_buffer + প্ৰবেশ_len,
				পরিমাণ(প্ৰবেশ_buffer) - প্ৰবেশ_len);
		যদি (ret <= 0) {
			যদি (!ret)
				die(_("early EOF"));
			die_errno(_("read error on input"));
		}
		প্ৰবেশ_len += ret;
		যদি (from_stdin)
			display_throughput(progress, consumed_bytes + প্ৰবেশ_len);
	} যেতিয়ালৈ (প্ৰবেশ_len < min);
	ঘূৰাই প্ৰবেশ_buffer;
}

স্থির শূন্য use(পূর্ণাঙ্ক bytes)
{
	যদি (bytes > প্ৰবেশ_len)
		die(_("used more bytes than were available"));
	প্ৰবেশ_crc32 = crc32(প্ৰবেশ_crc32, প্ৰবেশ_buffer + প্ৰবেশ_offset, bytes);
	প্ৰবেশ_len -= bytes;
	প্ৰবেশ_offset += bytes;

	/* make sure off_t is sufficiently large not to wrap */
	যদি (signed_add_overflows(consumed_bytes, bytes))
		die(_("pack too large for current definition of off_t"));
	consumed_bytes += bytes;
	যদি (max_প্ৰবেশ_size && consumed_bytes > max_প্ৰবেশ_size) {
		সংরচনা strbuf size_limit = STRBUF_INIT;
		strbuf_humanise_bytes(&size_limit, max_প্ৰবেশ_size);
		die(_("pack exceeds maximum allowed size (%s)"),
		    size_limit.buf);
	}
}

স্থির ধ্রুৱ ৱর্ণ *open_pack_file(ধ্রুৱ ৱর্ণ *pack_name)
{
	যদি (from_stdin) {
		প্ৰবেশ_fd = 0;
		যদি (!pack_name) {
			সংরচনা strbuf tmp_file = STRBUF_INIT;
			output_fd = odb_mkstemp(the_repository->objects, &tmp_file,
						"pack/tmp_pack_XXXXXX");
			pack_name = strbuf_detach(&tmp_file, রিক্ত);
		} অন্যথা {
			output_fd = xopen(pack_name, O_CREAT|O_EXCL|O_RDWR, 0600);
		}
		nothread_data.pack_fd = output_fd;
	} অন্যথা {
		প্ৰবেশ_fd = xopen(pack_name, O_RDONLY);
		output_fd = -1;
		nothread_data.pack_fd = প্ৰবেশ_fd;
	}
	git_hash_init(&প্ৰবেশ_ctx, the_hash_algo);
	ঘূৰাই pack_name;
}

স্থির শূন্য parse_pack_header(শূন্য)
{
	unsigned ৱর্ণ *hdr = fill(পরিমাণ(সংরচনা pack_header));

	/* Header consistency check */
	যদি (get_be32(hdr) != PACK_SIGNATURE)
		die(_("pack signature mismatch"));
	hdr += 4;
	যদি (!pack_version_ok_native(get_be32(hdr)))
		die(_("pack version %"PRIu32" unsupported"),
		    get_be32(hdr));
	hdr += 4;

	nr_objects = get_be32(hdr);
	use(পরিমাণ(সংরচনা pack_header));
}

__attribute__((format (printf, 2, 3)))
স্থির NORETURN শূন্য bad_object(off_t offset, ধ্রুৱ ৱর্ণ *format, ...)
{
	va_list params;
	ৱর্ণ buf[1024];

	va_start(params, format);
	vsnprintf(buf, পরিমাণ(buf), format, params);
	va_end(params);
	die(_("pack has bad object at offset %"PRIuMAX": %s"),
	    (uintmax_t)offset, buf);
}

স্থির অন্তঃস্থ সংরচনা thread_local_data *get_thread_data(শূন্য)
{
	যদি (HAVE_THREADS) {
		যদি (threads_active)
			ঘূৰাই pthread_getspecific(চাবি);
		প্রতিজ্ঞা(!threads_active &&
		       "This should only be reached when all threads are gone");
	}
	ঘূৰাই &nothread_data;
}

স্থির শূন্য set_thread_data(সংরচনা thread_local_data *data)
{
	যদি (threads_active)
		pthread_setspecific(চাবি, data);
}

স্থির শূন্য free_base_data(সংরচনা base_data *c)
{
	যদি (c->data) {
		FREE_AND_NULL(c->data);
		base_cache_used -= c->size;
	}
}

স্থির শূন্য prune_base_data(সংরচনা base_data *retain)
{
	সংরচনা list_head *pos;

	যদি (base_cache_used <= base_cache_limit)
		ঘূৰাই;

	list_for_each_prev(pos, &done_head) {
		সংরচনা base_data *b = list_entry(pos, সংরচনা base_data, list);
		যদি (b->retain_data || b == retain)
			অনুৱর্ত;
		যদি (b->data) {
			free_base_data(b);
			যদি (base_cache_used <= base_cache_limit)
				ঘূৰাই;
		}
	}

	list_for_each_prev(pos, &work_head) {
		সংরচনা base_data *b = list_entry(pos, সংরচনা base_data, list);
		যদি (b->retain_data || b == retain)
			অনুৱর্ত;
		যদি (b->data) {
			free_base_data(b);
			যদি (base_cache_used <= base_cache_limit)
				ঘূৰাই;
		}
	}
}

স্থির পূর্ণাঙ্ক is_delta_type(গণনা object_type type)
{
	ঘূৰাই (type == OBJ_REF_DELTA || type == OBJ_OFS_DELTA);
}

স্থির শূন্য *unpack_entry_data(off_t offset, size_t size,
			       গণনা object_type type, সংরচনা object_id *oid)
{
	স্থির ৱর্ণ fixed_buf[8192];
	পূর্ণাঙ্ক status;
	git_zstream stream;
	শূন্য *buf;
	সংরচনা git_hash_ctx c;
	ৱর্ণ hdr[32];
	পূর্ণাঙ্ক hdrlen;

	যদি (!is_delta_type(type)) {
		hdrlen = format_object_header(hdr, পরিমাণ(hdr), type, size);
		git_hash_init(&c, the_hash_algo);
		git_hash_update(&c, hdr, hdrlen);
	} অন্যথা
		oid = রিক্ত;
	যদি (type == OBJ_BLOB &&
	    size > repo_settings_get_big_file_threshold(the_repository))
		buf = fixed_buf;
	অন্যথা
		buf = xmallocz(size);

	memset(&stream, 0, পরিমাণ(stream));
	git_inflate_init(&stream);
	stream.next_out = buf;
	stream.avail_out = buf == fixed_buf ? পরিমাণ(fixed_buf) : size;

	কৰা {
		unsigned ৱর্ণ *last_out = stream.next_out;
		stream.next_in = fill(1);
		stream.avail_in = প্ৰবেশ_len;
		status = git_inflate(&stream, 0);
		use(প্ৰবেশ_len - stream.avail_in);
		যদি (oid)
			git_hash_update(&c, last_out, stream.next_out - last_out);
		যদি (buf == fixed_buf) {
			stream.next_out = buf;
			stream.avail_out = পরিমাণ(fixed_buf);
		}
	} যেতিয়ালৈ (status == Z_OK);
	যদি (stream.total_out != size || status != Z_STREAM_END)
		bad_object(offset, _("inflate returned %d"), status);
	git_inflate_end(&stream);
	যদি (oid)
		git_hash_final_oid(oid, &c);
	ঘূৰাই buf == fixed_buf ? রিক্ত : buf;
}

স্থির শূন্য *unpack_raw_entry(সংরচনা object_entry *obj,
			      off_t *ofs_offset,
			      সংরচনা object_id *ref_oid,
			      সংরচনা object_id *oid)
{
	unsigned ৱর্ণ *p;
	size_t size, c;
	off_t base_offset;
	unsigned shift;
	শূন্য *data;

	obj->idx.offset = consumed_bytes;
	প্ৰবেশ_crc32 = crc32(0, রিক্ত, 0);

	p = fill(1);
	c = *p;
	use(1);
	obj->type = (c >> 4) & 7;
	size = (c & 15);
	shift = 4;
	যেতিয়ালৈ (c & 0x80) {
		যদি ((bitsizeof(size_t) - 7) < shift)
			die(_("object size too large for this platform"));
		p = fill(1);
		c = *p;
		use(1);
		size += (c & 0x7f) << shift;
		shift += 7;
	}
	obj->size = size;

	ৱিকল্প (obj->type) {
	পক্ষ OBJ_REF_DELTA:
		oidread(ref_oid, fill(the_hash_algo->rawsz),
			the_repository->hash_algo);
		use(the_hash_algo->rawsz);
		ভঙ্গ;
	পক্ষ OBJ_OFS_DELTA:
		p = fill(1);
		c = *p;
		use(1);
		base_offset = c & 127;
		যেতিয়ালৈ (c & 128) {
			base_offset += 1;
			যদি (!base_offset || MSB(base_offset, 7))
				bad_object(obj->idx.offset, _("offset value overflow for delta base object"));
			p = fill(1);
			c = *p;
			use(1);
			base_offset = (base_offset << 7) + (c & 127);
		}
		*ofs_offset = obj->idx.offset - base_offset;
		যদি (*ofs_offset <= 0 || *ofs_offset >= obj->idx.offset)
			bad_object(obj->idx.offset, _("delta base offset is out of bound"));
		ভঙ্গ;
	পক্ষ OBJ_COMMIT:
	পক্ষ OBJ_বৃক্ষ:
	পক্ষ OBJ_BLOB:
	পক্ষ OBJ_TAG:
		ভঙ্গ;
	স্ৱভাৱ:
		bad_object(obj->idx.offset, _("unknown object type %d"), obj->type);
	}
	obj->hdr_size = consumed_bytes - obj->idx.offset;

	data = unpack_entry_data(obj->idx.offset, obj->size, obj->type, oid);
	obj->idx.crc32 = প্ৰবেশ_crc32;
	ঘূৰাই data;
}

স্থির শূন্য *unpack_data(সংরচনা object_entry *obj,
			 পূর্ণাঙ্ক (*consume)(ধ্রুৱ unsigned ৱর্ণ *, unsigned long, শূন্য *),
			 শূন্য *cb_data)
{
	off_t from = obj[0].idx.offset + obj[0].hdr_size;
	off_t len = obj[1].idx.offset - from;
	unsigned ৱর্ণ *data, *inbuf;
	git_zstream stream;
	পূর্ণাঙ্ক status;

	data = xmallocz(consume ? 64*1024 : obj->size);
	inbuf = xmalloc((len < 64*1024) ? (পূর্ণাঙ্ক)len : 64*1024);

	memset(&stream, 0, পরিমাণ(stream));
	git_inflate_init(&stream);
	stream.next_out = data;
	stream.avail_out = consume ? 64*1024 : obj->size;

	কৰা {
		ssize_t n = (len < 64*1024) ? (ssize_t)len : 64*1024;
		n = xpread(get_thread_data()->pack_fd, inbuf, n, from);
		যদি (n < 0)
			die_errno(_("cannot pread pack file"));
		যদি (!n)
			die(Q_("premature end of pack file, %"PRIuMAX" byte missing",
			       "premature end of pack file, %"PRIuMAX" bytes missing",
			       len),
			    (uintmax_t)len);
		from += n;
		len -= n;
		stream.next_in = inbuf;
		stream.avail_in = n;
		যদি (!consume)
			status = git_inflate(&stream, 0);
		অন্যথা {
			কৰা {
				status = git_inflate(&stream, 0);
				যদি (consume(data, stream.next_out - data, cb_data)) {
					মুক্ত(inbuf);
					মুক্ত(data);
					ঘূৰাই রিক্ত;
				}
				stream.next_out = data;
				stream.avail_out = 64*1024;
			} যেতিয়ালৈ (status == Z_OK && stream.avail_in);
		}
	} যেতিয়ালৈ (len && status == Z_OK && !stream.avail_in);

	/* This has been inflated OK when first encountered, so... */
	যদি (status != Z_STREAM_END || stream.total_out != obj->size)
		die(_("serious inflate inconsistency"));

	git_inflate_end(&stream);
	মুক্ত(inbuf);
	যদি (consume) {
		FREE_AND_NULL(data);
	}
	ঘূৰাই data;
}

স্থির শূন্য *get_data_from_pack(সংরচনা object_entry *obj)
{
	ঘূৰাই unpack_data(obj, রিক্ত, রিক্ত);
}

স্থির পূর্ণাঙ্ক compare_ofs_delta_bases(off_t offset1, off_t offset2,
				   গণনা object_type type1,
				   গণনা object_type type2)
{
	পূর্ণাঙ্ক cmp = type1 - type2;
	যদি (cmp)
		ঘূৰাই cmp;
	ঘূৰাই offset1 < offset2 ? -1 :
	       offset1 > offset2 ?  1 :
	       0;
}

স্থির পূর্ণাঙ্ক find_ofs_delta(ধ্রুৱ off_t offset)
{
	পূর্ণাঙ্ক first = 0, last = nr_ofs_deltas;

	যেতিয়ালৈ (first < last) {
		পূর্ণাঙ্ক next = first + (last - first) / 2;
		সংরচনা ofs_delta_entry *delta = &ofs_deltas[next];
		পূর্ণাঙ্ক cmp;

		cmp = compare_ofs_delta_bases(offset, delta->offset,
					      OBJ_OFS_DELTA,
					      objects[delta->obj_no].type);
		যদি (!cmp)
			ঘূৰাই next;
		যদি (cmp < 0) {
			last = next;
			অনুৱর্ত;
		}
		first = next+1;
	}
	ঘূৰাই -first-1;
}

স্থির শূন্য find_ofs_delta_children(off_t offset,
				    পূর্ণাঙ্ক *first_index, পূর্ণাঙ্ক *last_index)
{
	পূর্ণাঙ্ক first = find_ofs_delta(offset);
	পূর্ণাঙ্ক last = first;
	পূর্ণাঙ্ক end = nr_ofs_deltas - 1;

	যদি (first < 0) {
		*first_index = 0;
		*last_index = -1;
		ঘূৰাই;
	}
	যেতিয়ালৈ (first > 0 && ofs_deltas[first - 1].offset == offset)
		--first;
	যেতিয়ালৈ (last < end && ofs_deltas[last + 1].offset == offset)
		++last;
	*first_index = first;
	*last_index = last;
}

স্থির পূর্ণাঙ্ক compare_ref_delta_bases(ধ্রুৱ সংরচনা object_id *oid1,
				   ধ্রুৱ সংরচনা object_id *oid2,
				   গণনা object_type type1,
				   গণনা object_type type2)
{
	পূর্ণাঙ্ক cmp = type1 - type2;
	যদি (cmp)
		ঘূৰাই cmp;
	ঘূৰাই oidcmp(oid1, oid2);
}

স্থির পূর্ণাঙ্ক find_ref_delta(ধ্রুৱ সংরচনা object_id *oid)
{
	পূর্ণাঙ্ক first = 0, last = nr_ref_deltas;

	যেতিয়ালৈ (first < last) {
		পূর্ণাঙ্ক next = first + (last - first) / 2;
		সংরচনা ref_delta_entry *delta = &ref_deltas[next];
		পূর্ণাঙ্ক cmp;

		cmp = compare_ref_delta_bases(oid, &delta->oid,
					      OBJ_REF_DELTA,
					      objects[delta->obj_no].type);
		যদি (!cmp)
			ঘূৰাই next;
		যদি (cmp < 0) {
			last = next;
			অনুৱর্ত;
		}
		first = next+1;
	}
	ঘূৰাই -first-1;
}

স্থির শূন্য find_ref_delta_children(ধ্রুৱ সংরচনা object_id *oid,
				    পূর্ণাঙ্ক *first_index, পূর্ণাঙ্ক *last_index)
{
	পূর্ণাঙ্ক first = find_ref_delta(oid);
	পূর্ণাঙ্ক last = first;
	পূর্ণাঙ্ক end = nr_ref_deltas - 1;

	যদি (first < 0) {
		*first_index = 0;
		*last_index = -1;
		ঘূৰাই;
	}
	যেতিয়ালৈ (first > 0 && oideq(&ref_deltas[first - 1].oid, oid))
		--first;
	যেতিয়ালৈ (last < end && oideq(&ref_deltas[last + 1].oid, oid))
		++last;
	*first_index = first;
	*last_index = last;
}

সংরচনা compare_data {
	সংরচনা object_entry *entry;
	সংরচনা odb_stream *st;
	unsigned ৱর্ণ *buf;
	unsigned long buf_size;
};

স্থির পূর্ণাঙ্ক compare_objects(ধ্রুৱ unsigned ৱর্ণ *buf, unsigned long size,
			   শূন্য *cb_data)
{
	সংরচনা compare_data *data = cb_data;

	যদি (data->buf_size < size) {
		মুক্ত(data->buf);
		data->buf = xmalloc(size);
		data->buf_size = size;
	}

	যেতিয়ালৈ (size) {
		ssize_t len = odb_stream_read(data->st, data->buf, size);
		যদি (len == 0)
			die(_("SHA1 COLLISION FOUND WITH %s !"),
			    oid_to_hex(&data->entry->idx.oid));
		যদি (len < 0)
			die(_("unable to read %s"),
			    oid_to_hex(&data->entry->idx.oid));
		যদি (memcmp(buf, data->buf, len))
			die(_("SHA1 COLLISION FOUND WITH %s !"),
			    oid_to_hex(&data->entry->idx.oid));
		size -= len;
		buf += len;
	}
	ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক check_collison(সংরচনা object_entry *entry)
{
	সংরচনা compare_data data;

	যদি (entry->size <= repo_settings_get_big_file_threshold(the_repository) ||
	    entry->type != OBJ_BLOB)
		ঘূৰাই -1;

	memset(&data, 0, পরিমাণ(data));
	data.entry = entry;
	data.st = odb_stream_from_object(the_repository->objects, &entry->idx.oid, রিক্ত);
	যদি (!data.st)
		ঘূৰাই -1;
	যদি (data.st->size != entry->size || data.st->type != entry->type)
		die(_("SHA1 COLLISION FOUND WITH %s !"),
		    oid_to_hex(&entry->idx.oid));
	unpack_data(entry, compare_objects, &data);
	odb_stream_close(data.st);
	মুক্ত(data.buf);
	ঘূৰাই 0;
}

স্থির শূন্য record_outgoing_link(ধ্রুৱ সংরচনা object_id *oid)
{
	oidset_ঢোকাও(&outgoing_links, oid);
}

স্থির শূন্য maybe_record_name_entry(ধ্রুৱ সংরচনা name_entry *entry)
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
	যদি (S_ISDIR(entry->mode))
		record_outgoing_link(&entry->oid);
}

স্থির শূন্য do_record_outgoing_links(সংরচনা object *obj)
{
	যদি (obj->type == OBJ_বৃক্ষ) {
		সংরচনা বৃক্ষ *বৃক্ষ = (সংরচনা বৃক্ষ *)obj;
		সংরচনা বৃক্ষ_desc desc;
		সংরচনা name_entry entry;
		যদি (init_বৃক্ষ_desc_gently(&desc, &বৃক্ষ->object.oid,
					  বৃক্ষ->buffer, বৃক্ষ->size, 0))
			/*
			 * Error messages are given when packs are
			 * verified, so do not print any here.
			 */
			ঘূৰাই;
		যেতিয়ালৈ (বৃক্ষ_entry_gently(&desc, &entry))
			maybe_record_name_entry(&entry);
	} অন্যথা যদি (obj->type == OBJ_COMMIT) {
		সংরচনা commit *commit = (সংরচনা commit *) obj;
		সংরচনা commit_list *parents = commit->parents;

		record_outgoing_link(get_commit_বৃক্ষ_oid(commit));
		বাবে (; parents; parents = parents->next)
			record_outgoing_link(&parents->item->object.oid);
	} অন্যথা যদি (obj->type == OBJ_TAG) {
		সংরচনা tag *tag = (সংরচনা tag *) obj;
		record_outgoing_link(get_tagged_oid(tag));
	}
}

স্থির শূন্য sha1_object(ধ্রুৱ শূন্য *data, সংরচনা object_entry *obj_entry,
			unsigned long size, গণনা object_type type,
			ধ্রুৱ সংরচনা object_id *oid)
{
	শূন্য *new_data = রিক্ত;
	পূর্ণাঙ্ক collision_test_needed = 0;

	প্রতিজ্ঞা(data || obj_entry);

	যদি (startup_info->have_repository) {
		read_lock();
		collision_test_needed = odb_has_object(the_repository->objects, oid,
						       ODB_HAS_OBJECT_FETCH_PROMISOR);
		read_unlock();
	}

	যদি (collision_test_needed && !data) {
		read_lock();
		যদি (!check_collison(obj_entry))
			collision_test_needed = 0;
		read_unlock();
	}
	যদি (collision_test_needed) {
		শূন্য *has_data;
		গণনা object_type has_type;
		size_t has_size;
		read_lock();
		has_type = odb_read_object_info(the_repository->objects, oid, &has_size);
		যদি (has_type < 0)
			die(_("cannot read existing object info %s"), oid_to_hex(oid));
		যদি (has_type != type || has_size != size)
			die(_("SHA1 COLLISION FOUND WITH %s !"), oid_to_hex(oid));
		has_data = odb_read_object(the_repository->objects, oid,
					   &has_type, &has_size);
		read_unlock();
		যদি (!data)
			data = new_data = get_data_from_pack(obj_entry);
		যদি (!has_data)
			die(_("cannot read existing object %s"), oid_to_hex(oid));
		যদি (size != has_size || type != has_type ||
		    memcmp(data, has_data, size) != 0)
			die(_("SHA1 COLLISION FOUND WITH %s !"), oid_to_hex(oid));
		মুক্ত(has_data);
	}

	যদি (strict || do_fsck_object || record_outgoing_links) {
		read_lock();
		যদি (type == OBJ_BLOB) {
			সংরচনা blob *blob = lookup_blob(the_repository, oid);
			যদি (blob)
				blob->object.flags |= FLAG_CHECKED;
			অন্যথা
				die(_("invalid blob object %s"), oid_to_hex(oid));
			যদি (do_fsck_object &&
			    fsck_object(&blob->object, (শূন্য *)data, size, &fsck_options))
				die(_("fsck error in packed object"));
		} অন্যথা {
			সংরচনা object *obj;
			পূর্ণাঙ্ক eaten;
			শূন্য *buf = (শূন্য *) data;

			প্রতিজ্ঞা(data && "data can only be NULL for large _blobs_");

			/*
			 * we do not need to free the memory here, as the
			 * buf is deleted by the caller.
			 */
			obj = parse_object_buffer(the_repository, oid, type,
						  size, buf,
						  &eaten);
			যদি (!obj)
				die(_("invalid %s"), type_name(type));
			যদি (do_fsck_object &&
			    fsck_object(obj, buf, size, &fsck_options))
				die(_("fsck error in packed object"));
			যদি (strict && fsck_walk(obj, রিক্ত, &fsck_options))
				die(_("Not all child objects of %s are reachable"), oid_to_hex(&obj->oid));
			যদি (record_outgoing_links)
				do_record_outgoing_links(obj);

			যদি (obj->type == OBJ_বৃক্ষ) {
				সংরচনা বৃক্ষ *item = (সংরচনা বৃক্ষ *) obj;
				item->buffer = রিক্ত;
				obj->parsed = 0;
			}
			যদি (obj->type == OBJ_COMMIT) {
				সংরচনা commit *commit = (সংরচনা commit *) obj;
				যদি (detach_commit_buffer(commit, রিক্ত) != data)
					BUG("parse_object_buffer transmogrified our buffer");
			}
			obj->flags |= FLAG_CHECKED;
		}
		read_unlock();
	}

	মুক্ত(new_data);
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
স্থির শূন্য *get_base_data(সংরচনা base_data *c)
{
	যদি (!c->data) {
		সংরচনা object_entry *obj = c->obj;
		সংরচনা base_data **delta = রিক্ত;
		পূর্ণাঙ্ক delta_nr = 0, delta_alloc = 0;

		যেতিয়ালৈ (is_delta_type(c->obj->type) && !c->data) {
			ALLOC_GROW(delta, delta_nr + 1, delta_alloc);
			delta[delta_nr++] = c;
			c = c->base;
		}
		যদি (!delta_nr) {
			c->data = get_data_from_pack(obj);
			c->size = obj->size;
			base_cache_used += c->size;
			prune_base_data(c);
		}
		বাবে (; delta_nr > 0; delta_nr--) {
			শূন্য *base, *raw;
			c = delta[delta_nr - 1];
			obj = c->obj;
			base = get_base_data(c->base);
			raw = get_data_from_pack(obj);
			c->data = patch_delta(
				base, c->base->size,
				raw, obj->size,
				&c->size);
			মুক্ত(raw);
			যদি (!c->data)
				bad_object(obj->idx.offset, _("failed to apply delta"));
			base_cache_used += c->size;
			prune_base_data(c);
		}
		মুক্ত(delta);
	}
	ঘূৰাই c->data;
}

স্থির সংরচনা base_data *make_base(সংরচনা object_entry *obj,
				   সংরচনা base_data *parent)
{
	সংরচনা base_data *base = xcalloc(1, পরিমাণ(সংরচনা base_data));
	base->base = parent;
	base->obj = obj;
	find_ref_delta_children(&obj->idx.oid,
				&base->ref_first, &base->ref_last);
	find_ofs_delta_children(obj->idx.offset,
				&base->ofs_first, &base->ofs_last);
	base->children_remaining = base->ref_last - base->ref_first +
		base->ofs_last - base->ofs_first + 2;
	ঘূৰাই base;
}

স্থির সংরচনা base_data *resolve_delta(সংরচনা object_entry *delta_obj,
				       সংরচনা base_data *base)
{
	শূন্য *delta_data, *result_data;
	সংরচনা base_data *result;
	size_t result_size;

	যদি (show_stat) {
		পূর্ণাঙ্ক i = delta_obj - objects;
		পূর্ণাঙ্ক j = base->obj - objects;
		obj_stat[i].delta_depth = obj_stat[j].delta_depth + 1;
		deepest_delta_lock();
		যদি (deepest_delta < obj_stat[i].delta_depth)
			deepest_delta = obj_stat[i].delta_depth;
		deepest_delta_unlock();
		obj_stat[i].base_object_no = j;
	}
	delta_data = get_data_from_pack(delta_obj);
	প্রতিজ্ঞা(base->data);
	result_data = patch_delta(base->data, base->size,
				  delta_data, delta_obj->size, &result_size);
	মুক্ত(delta_data);
	যদি (!result_data)
		bad_object(delta_obj->idx.offset, _("failed to apply delta"));
	hash_object_file(the_hash_algo, result_data, result_size,
			 delta_obj->real_type, &delta_obj->idx.oid);
	sha1_object(result_data, রিক্ত, result_size, delta_obj->real_type,
		    &delta_obj->idx.oid);

	result = make_base(delta_obj, base);
	result->data = result_data;
	result->size = result_size;

	counter_lock();
	nr_resolved_deltas++;
	counter_unlock();

	ঘূৰাই result;
}

স্থির পূর্ণাঙ্ক compare_ofs_delta_entry(ধ্রুৱ শূন্য *a, ধ্রুৱ শূন্য *b)
{
	ধ্রুৱ সংরচনা ofs_delta_entry *delta_a = a;
	ধ্রুৱ সংরচনা ofs_delta_entry *delta_b = b;

	ঘূৰাই delta_a->offset < delta_b->offset ? -1 :
	       delta_a->offset > delta_b->offset ?  1 :
	       0;
}

স্থির পূর্ণাঙ্ক compare_ref_delta_entry(ধ্রুৱ শূন্য *a, ধ্রুৱ শূন্য *b)
{
	ধ্রুৱ সংরচনা ref_delta_entry *delta_a = a;
	ধ্রুৱ সংরচনা ref_delta_entry *delta_b = b;

	ঘূৰাই oidcmp(&delta_a->oid, &delta_b->oid);
}

স্থির শূন্য *threaded_second_pass(শূন্য *data)
{
	যদি (data)
		set_thread_data(data);
	বাবে (;;) {
		সংরচনা base_data *parent = রিক্ত;
		সংরচনা object_entry *child_obj = রিক্ত;
		সংরচনা base_data *child = রিক্ত;

		counter_lock();
		display_progress(progress, nr_resolved_deltas);
		counter_unlock();

		work_lock();
		যদি (list_empty(&work_head)) {
			/*
			 * Take an object from the object array.
			 */
			যেতিয়ালৈ (nr_dispatched < nr_objects &&
			       is_delta_type(objects[nr_dispatched].type))
				nr_dispatched++;
			যদি (nr_dispatched >= nr_objects) {
				work_unlock();
				ভঙ্গ;
			}
			child_obj = &objects[nr_dispatched++];
		} অন্যথা {
			/*
			 * Peek at the top of the stack, and take a child from
			 * it.
			 */
			parent = list_first_entry(&work_head, সংরচনা base_data,
						  list);

			যেতিয়ালৈ (parent->ref_first <= parent->ref_last) {
				পূর্ণাঙ্ক offset = ref_deltas[parent->ref_first++].obj_no;
				child_obj = objects + offset;
				যদি (child_obj->real_type != OBJ_REF_DELTA) {
					child_obj = রিক্ত;
					অনুৱর্ত;
				}
				child_obj->real_type = parent->obj->real_type;
				ভঙ্গ;
			}

			যদি (!child_obj && parent->ofs_first <= parent->ofs_last) {
				child_obj = objects +
					ofs_deltas[parent->ofs_first++].obj_no;
				প্রতিজ্ঞা(child_obj->real_type == OBJ_OFS_DELTA);
				child_obj->real_type = parent->obj->real_type;
			}

			যদি (parent->ref_first > parent->ref_last &&
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

		যদি (child_obj) {
			যদি (parent) {
				child = resolve_delta(child_obj, parent);
				যদি (!child->children_remaining)
					FREE_AND_NULL(child->data);
			} অন্যথা{
				child = make_base(child_obj, রিক্ত);
				যদি (child->children_remaining) {
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
		যদি (parent)
			parent->retain_data--;

		যদি (child && child->data) {
			/*
			 * This child has its own children, so add it to
			 * work_head.
			 */
			list_add(&child->list, &work_head);
			base_cache_used += child->size;
			prune_base_data(রিক্ত);
		} অন্যথা যদি (child) {
			/*
			 * This child does not have its own children. It may be
			 * the last descendant of its ancestors; free those
			 * that we can.
			 */
			সংরচনা base_data *p = parent;

			যেতিয়ালৈ (p) {
				সংরচনা base_data *next_p;

				p->children_remaining--;
				যদি (p->children_remaining)
					ভঙ্গ;

				next_p = p->base;
				free_base_data(p);
				list_del(&p->list);
				মুক্ত(p);

				p = next_p;
			}
			FREE_AND_NULL(child);
		}
		work_unlock();
	}
	ঘূৰাই রিক্ত;
}

/*
 * First pass:
 * - find locations of all objects;
 * - calculate SHA1 of all non-delta objects;
 * - remember base (SHA1 or offset) for all deltas.
 */
স্থির শূন্য parse_pack_objects(unsigned ৱর্ণ *hash)
{
	পূর্ণাঙ্ক i, nr_delays = 0;
	সংরচনা ofs_delta_entry *ofs_delta = ofs_deltas;
	সংরচনা object_id ref_delta_oid;
	সংরচনা stat st;
	সংরচনা git_hash_ctx tmp_ctx;

	যদি (verbose)
		progress = start_progress(
				the_repository,
				progress_title ? progress_title :
				from_stdin ? _("Receiving objects") : _("Indexing objects"),
				nr_objects);
	বাবে (i = 0; i < nr_objects; i++) {
		সংরচনা object_entry *obj = &objects[i];
		শূন্য *data = unpack_raw_entry(obj, &ofs_delta->offset,
					      &ref_delta_oid,
					      &obj->idx.oid);
		obj->real_type = obj->type;
		যদি (obj->type == OBJ_OFS_DELTA) {
			nr_ofs_deltas++;
			ofs_delta->obj_no = i;
			ofs_delta++;
		} অন্যথা যদি (obj->type == OBJ_REF_DELTA) {
			ALLOC_GROW(ref_deltas, nr_ref_deltas + 1, ref_deltas_alloc);
			oidcpy(&ref_deltas[nr_ref_deltas].oid, &ref_delta_oid);
			ref_deltas[nr_ref_deltas].obj_no = i;
			nr_ref_deltas++;
		} অন্যথা যদি (!data) {
			/* large blobs, check later */
			obj->real_type = OBJ_BAD;
			nr_delays++;
		} অন্যথা
			sha1_object(data, রিক্ত, obj->size, obj->type,
				    &obj->idx.oid);
		মুক্ত(data);
		display_progress(progress, i+1);
	}
	objects[i].idx.offset = consumed_bytes;
	stop_progress(&progress);

	/* Check pack integrity */
	flush();
	git_hash_init(&tmp_ctx, the_hash_algo);
	git_hash_clone(&tmp_ctx, &প্ৰবেশ_ctx);
	git_hash_final(hash, &tmp_ctx);
	যদি (!hasheq(fill(the_hash_algo->rawsz), hash, the_repository->hash_algo))
		die(_("pack is corrupted (SHA1 mismatch)"));
	use(the_hash_algo->rawsz);

	/* If input_fd is a file, we should have reached its end now. */
	যদি (fstat(প্ৰবেশ_fd, &st))
		die_errno(_("cannot fstat packfile"));
	যদি (S_ISREG(st.st_mode) &&
			lseek(প্ৰবেশ_fd, 0, SEEK_CUR) - প্ৰবেশ_len != st.st_size)
		die(_("pack has junk at the end"));

	বাবে (i = 0; i < nr_objects; i++) {
		সংরচনা object_entry *obj = &objects[i];
		যদি (obj->real_type != OBJ_BAD)
			অনুৱর্ত;
		obj->real_type = obj->type;
		sha1_object(রিক্ত, obj, obj->size, obj->type,
			    &obj->idx.oid);
		nr_delays--;
	}
	যদি (nr_delays)
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
স্থির শূন্য resolve_deltas(সংরচনা pack_idx_option *opts)
{
	পূর্ণাঙ্ক i;

	যদি (!nr_ofs_deltas && !nr_ref_deltas)
		ঘূৰাই;

	/* Sort deltas by base SHA1/offset for fast searching */
	QSORT(ofs_deltas, nr_ofs_deltas, compare_ofs_delta_entry);
	QSORT(ref_deltas, nr_ref_deltas, compare_ref_delta_entry);

	যদি (verbose || show_resolving_progress)
		progress = start_progress(the_repository,
					  _("Resolving deltas"),
					  nr_ref_deltas + nr_ofs_deltas);

	nr_dispatched = 0;
	base_cache_limit = opts->delta_base_cache_limit * nr_threads;
	যদি (nr_threads > 1 || getenv("GIT_FORCE_THREADS")) {
		init_thread();
		বাবে (i = 0; i < nr_threads; i++) {
			পূর্ণাঙ্ক ret = pthread_create(&thread_data[i].thread, রিক্ত,
						 threaded_second_pass, thread_data + i);
			যদি (ret)
				die(_("unable to create thread: %s"),
				    strerror(ret));
		}
		বাবে (i = 0; i < nr_threads; i++)
			pthread_join(thread_data[i].thread, রিক্ত);
		cleanup_thread();
		ঘূৰাই;
	}
	threaded_second_pass(&nothread_data);
}

/*
 * Third pass:
 * - append objects to convert thin pack to full pack if required
 * - write the final pack hash
 */
স্থির শূন্য fix_unresolved_deltas(সংরচনা hashfile *f);
স্থির শূন্য conclude_pack(পূর্ণাঙ্ক fix_thin_pack, ধ্রুৱ ৱর্ণ *curr_pack, unsigned ৱর্ণ *pack_hash)
{
	যদি (nr_ref_deltas + nr_ofs_deltas == nr_resolved_deltas) {
		stop_progress(&progress);
		/* Flush remaining pack final hash. */
		flush();
		ঘূৰাই;
	}

	যদি (fix_thin_pack) {
		সংরচনা hashfile *f;
		unsigned ৱর্ণ read_hash[GIT_MAX_RAWSZ], tail_hash[GIT_MAX_RAWSZ];
		সংরচনা strbuf বাৰ্তা = STRBUF_INIT;
		পূর্ণাঙ্ক nr_unresolved = nr_ofs_deltas + nr_ref_deltas - nr_resolved_deltas;
		পূর্ণাঙ্ক nr_objects_initial = nr_objects;
		যদি (nr_unresolved <= 0)
			die(_("confusion beyond insanity"));
		REALLOC_ARRAY(objects, nr_objects + nr_unresolved + 1);
		memset(objects + nr_objects + 1, 0,
		       nr_unresolved * পরিমাণ(*objects));
		f = hashfd(the_repository->hash_algo, output_fd, curr_pack);
		fix_unresolved_deltas(f);
		strbuf_addf(&বাৰ্তা, Q_("completed with %d local object",
				     "completed with %d local objects",
				     nr_objects - nr_objects_initial),
			    nr_objects - nr_objects_initial);
		stop_progress_বাৰ্তা(&progress, বাৰ্তা.buf);
		strbuf_release(&বাৰ্তা);
		finalize_hashfile(f, tail_hash, FSYNC_COMPONENT_PACK, 0);
		hashcpy(read_hash, pack_hash, the_repository->hash_algo);
		fixup_pack_header_footer(the_hash_algo, output_fd, pack_hash,
					 curr_pack, nr_objects,
					 read_hash, consumed_bytes-the_hash_algo->rawsz);
		যদি (!hasheq(read_hash, tail_hash, the_repository->hash_algo))
			die(_("Unexpected tail checksum for %s "
			      "(disk corruption?)"), curr_pack);
	}
	যদি (nr_ofs_deltas + nr_ref_deltas != nr_resolved_deltas)
		die(Q_("pack has %d unresolved delta",
		       "pack has %d unresolved deltas",
		       nr_ofs_deltas + nr_ref_deltas - nr_resolved_deltas),
		    nr_ofs_deltas + nr_ref_deltas - nr_resolved_deltas);
}

স্থির পূর্ণাঙ্ক write_compressed(সংরচনা hashfile *f, শূন্য *in, unsigned পূর্ণাঙ্ক size)
{
	git_zstream stream;
	পূর্ণাঙ্ক status;
	unsigned ৱর্ণ outbuf[4096];
	সংরচনা repo_config_মানসমূহ *cfg = repo_config_মানসমূহ(the_repository);

	git_deflate_init(&stream, cfg->zlib_compression_level);
	stream.next_in = in;
	stream.avail_in = size;

	কৰা {
		stream.next_out = outbuf;
		stream.avail_out = পরিমাণ(outbuf);
		status = git_deflate(&stream, Z_FINISH);
		hashwrite(f, outbuf, পরিমাণ(outbuf) - stream.avail_out);
	} যেতিয়ালৈ (status == Z_OK);

	যদি (status != Z_STREAM_END)
		die(_("unable to deflate appended object (%d)"), status);
	size = stream.total_out;
	git_deflate_end(&stream);
	ঘূৰাই size;
}

স্থির সংরচনা object_entry *append_obj_to_pack(সংরচনা hashfile *f,
			       ধ্রুৱ unsigned ৱর্ণ *sha1, শূন্য *buf,
			       unsigned long size, গণনা object_type type)
{
	সংরচনা object_entry *obj = &objects[nr_objects++];
	unsigned ৱর্ণ header[10];
	unsigned long s = size;
	পূর্ণাঙ্ক n = 0;
	unsigned ৱর্ণ c = (type << 4) | (s & 15);
	s >>= 4;
	যেতিয়ালৈ (s) {
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
	ঘূৰাই obj;
}

স্থির পূর্ণাঙ্ক delta_pos_compare(ধ্রুৱ শূন্য *_a, ধ্রুৱ শূন্য *_b)
{
	সংরচনা ref_delta_entry *a = *(সংরচনা ref_delta_entry **)_a;
	সংরচনা ref_delta_entry *b = *(সংরচনা ref_delta_entry **)_b;
	ঘূৰাই a->obj_no - b->obj_no;
}

স্থির শূন্য fix_unresolved_deltas(সংরচনা hashfile *f)
{
	সংরচনা ref_delta_entry **sorted_by_pos;
	পূর্ণাঙ্ক i;

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
	বাবে (i = 0; i < nr_ref_deltas; i++)
		sorted_by_pos[i] = &ref_deltas[i];
	QSORT(sorted_by_pos, nr_ref_deltas, delta_pos_compare);

	যদি (repo_has_promisor_remote(the_repository)) {
		/*
		 * Prefetch the delta bases.
		 */
		সংরচনা oid_array to_fetch = OID_ARRAY_INIT;
		বাবে (i = 0; i < nr_ref_deltas; i++) {
			সংরচনা ref_delta_entry *d = sorted_by_pos[i];
			যদি (!odb_read_object_info_extended(the_repository->objects,
							   &d->oid, রিক্ত,
							   OBJECT_INFO_FOR_PREFETCH))
				অনুৱর্ত;
			oid_array_append(&to_fetch, &d->oid);
		}
		promisor_remote_get_direct(the_repository,
					   to_fetch.oid, to_fetch.nr);
		oid_array_clear(&to_fetch);
	}

	বাবে (i = 0; i < nr_ref_deltas; i++) {
		সংরচনা ref_delta_entry *d = sorted_by_pos[i];
		গণনা object_type type;
		শূন্য *data;
		size_t size;

		যদি (objects[d->obj_no].real_type != OBJ_REF_DELTA)
			অনুৱর্ত;
		data = odb_read_object(the_repository->objects, &d->oid,
				       &type, &size);
		যদি (!data)
			অনুৱর্ত;

		যদি (check_object_signature(the_repository, &d->oid, data, size,
					   type) < 0)
			die(_("local object %s is corrupt"), oid_to_hex(&d->oid));

		/*
		 * Add this as an object to the objects array and call
		 * threaded_second_pass() (which will pick up the added
		 * object).
		 */
		append_obj_to_pack(f, d->oid.hash, data, size, type);
		মুক্ত(data);
		threaded_second_pass(রিক্ত);

		display_progress(progress, nr_resolved_deltas);
	}
	মুক্ত(sorted_by_pos);
}

স্থির ধ্রুৱ ৱর্ণ *derive_filename(ধ্রুৱ ৱর্ণ *pack_name, ধ্রুৱ ৱর্ণ *strip,
				   ধ্রুৱ ৱর্ণ *suffix, সংরচনা strbuf *buf)
{
	size_t len;
	যদি (!strip_suffix(pack_name, strip, &len) || !len ||
	    pack_name[len - 1] != '.')
		die(_("packfile name '%s' does not end with '.%s'"),
		    pack_name, strip);
	strbuf_add(buf, pack_name, len);
	strbuf_addstr(buf, suffix);
	ঘূৰাই buf->buf;
}

স্থির শূন্য write_special_file(ধ্রুৱ ৱর্ণ *suffix, ধ্রুৱ ৱর্ণ *বাৰ্তা,
			       ধ্রুৱ ৱর্ণ *pack_name, ধ্রুৱ unsigned ৱর্ণ *hash,
			       ধ্রুৱ ৱর্ণ **report)
{
	সংরচনা strbuf name_buf = STRBUF_INIT;
	ধ্রুৱ ৱর্ণ *filename;
	পূর্ণাঙ্ক fd;
	পূর্ণাঙ্ক বাৰ্তা_len = strlen(বাৰ্তা);

	যদি (pack_name)
		filename = derive_filename(pack_name, "pack", suffix, &name_buf);
	অন্যথা
		filename = odb_pack_name(the_repository, &name_buf, hash, suffix);

	fd = safe_create_file_with_leading_directories(the_repository, filename);
	যদি (fd < 0) {
		যদি (errno != EEXIST)
			die_errno(_("cannot write %s file '%s'"),
				  suffix, filename);
	} অন্যথা {
		যদি (বাৰ্তা_len > 0) {
			write_or_die(fd, বাৰ্তা, বাৰ্তা_len);
			write_or_die(fd, "\n", 1);
		}
		যদি (close(fd) != 0)
			die_errno(_("cannot close written %s file '%s'"),
				  suffix, filename);
		যদি (report)
			*report = suffix;
	}
	strbuf_release(&name_buf);
}

স্থির শূন্য rename_tmp_packfile(ধ্রুৱ ৱর্ণ **final_name,
				ধ্রুৱ ৱর্ণ *curr_name,
				সংরচনা strbuf *name, unsigned ৱর্ণ *hash,
				ধ্রুৱ ৱর্ণ *ext, পূর্ণাঙ্ক make_read_only_if_same)
{
	যদি (!*final_name || strcmp(*final_name, curr_name)) {
		যদি (!*final_name)
			*final_name = odb_pack_name(the_repository, name, hash, ext);
		যদি (finalize_object_file(the_repository, curr_name, *final_name))
			die(_("unable to rename temporary '*.%s' file to '%s'"),
			    ext, *final_name);
	} অন্যথা যদি (make_read_only_if_same) {
		chmod(*final_name, 0444);
	}
}

স্থির শূন্য final(ধ্রুৱ ৱর্ণ *final_pack_name, ধ্রুৱ ৱর্ণ *curr_pack_name,
		  ধ্রুৱ ৱর্ণ *final_index_name, ধ্রুৱ ৱর্ণ *curr_index_name,
		  ধ্রুৱ ৱর্ণ *final_rev_index_name, ধ্রুৱ ৱর্ণ *curr_rev_index_name,
		  ধ্রুৱ ৱর্ণ *keep_বাৰ্তা, ধ্রুৱ ৱর্ণ *promisor_বাৰ্তা,
		  unsigned ৱর্ণ *hash)
{
	ধ্রুৱ ৱর্ণ *report = "pack";
	সংরচনা strbuf pack_name = STRBUF_INIT;
	সংরচনা strbuf index_name = STRBUF_INIT;
	সংরচনা strbuf rev_index_name = STRBUF_INIT;

	যদি (!from_stdin) {
		close(প্ৰবেশ_fd);
	} অন্যথা {
		fsync_component_or_die(FSYNC_COMPONENT_PACK, output_fd, curr_pack_name);
		যদি (close(output_fd))
			die_errno(_("error while closing pack file"));
	}

	যদি (keep_বাৰ্তা)
		write_special_file("keep", keep_বাৰ্তা, final_pack_name, hash,
				   &report);
	যদি (promisor_বাৰ্তা)
		write_special_file("promisor", promisor_বাৰ্তা, final_pack_name,
				   hash, রিক্ত);

	rename_tmp_packfile(&final_pack_name, curr_pack_name, &pack_name,
			    hash, "pack", from_stdin);
	যদি (curr_rev_index_name)
		rename_tmp_packfile(&final_rev_index_name, curr_rev_index_name,
				    &rev_index_name, hash, "rev", 1);
	rename_tmp_packfile(&final_index_name, curr_index_name, &index_name,
			    hash, "idx", 1);

	যদি (do_fsck_object && startup_info->have_repository) {
		সংরচনা odb_source_files *files =
			odb_source_files_downcast(the_repository->objects->source);
		packfile_store_load_pack(files->dirs->packed, final_index_name, 0);
	}

	যদি (!from_stdin) {
		printf("%s\n", hash_to_hex(hash));
	} অন্যথা {
		সংরচনা strbuf buf = STRBUF_INIT;

		strbuf_addf(&buf, "%s\t%s\n", report, hash_to_hex(hash));
		write_or_die(1, buf.buf, buf.len);
		strbuf_release(&buf);

		/* Write the last part of the buffer to stdout */
		write_in_full(1, প্ৰবেশ_buffer + প্ৰবেশ_offset, প্ৰবেশ_len);
	}

	strbuf_release(&rev_index_name);
	strbuf_release(&index_name);
	strbuf_release(&pack_name);
}

স্থির পূর্ণাঙ্ক git_index_pack_config(ধ্রুৱ ৱর্ণ *k, ধ্রুৱ ৱর্ণ *v,
				 ধ্রুৱ সংরচনা config_context *ctx, শূন্য *cb)
{
	সংরচনা pack_idx_option *opts = cb;

	যদি (!strcmp(k, "pack.indexversion")) {
		opts->version = git_config_int(k, v, ctx->kvi);
		যদি (opts->version > 2)
			die(_("bad pack.indexVersion=%"PRIu32), opts->version);
		ঘূৰাই 0;
	}
	যদি (!strcmp(k, "pack.threads")) {
		nr_threads = git_config_int(k, v, ctx->kvi);
		যদি (nr_threads < 0)
			die(_("invalid number of threads specified (%d)"),
			    nr_threads);
		যদি (!HAVE_THREADS && nr_threads != 1) {
			warning(_("no threads support, ignoring %s"), k);
			nr_threads = 1;
		}
		ঘূৰাই 0;
	}
	যদি (!strcmp(k, "pack.writereverseindex")) {
		যদি (git_config_bool(k, v))
			opts->flags |= WRITE_REV;
		অন্যথা
			opts->flags &= ~WRITE_REV;
	}
	যদি (!strcmp(k, "core.deltabasecachelimit")) {
		opts->delta_base_cache_limit = git_config_ulong(k, v, ctx->kvi);
		ঘূৰাই 0;
	}
	ঘূৰাই git_default_config(k, v, ctx, cb);
}

স্থির পূর্ণাঙ্ক cmp_uint32(ধ্রুৱ শূন্য *a_, ধ্রুৱ শূন্য *b_)
{
	uint32_t a = *((uint32_t *)a_);
	uint32_t b = *((uint32_t *)b_);

	ঘূৰাই (a < b) ? -1 : (a != b);
}

স্থির শূন্য read_v2_anomalous_offsets(সংরচনা packed_git *p,
				      সংরচনা pack_idx_option *opts)
{
	ধ্রুৱ uint32_t *idx1, *idx2;
	uint32_t i;

	/* The address of the 4-byte offset table */
	idx1 = (((ধ্রুৱ uint32_t *)((ধ্রুৱ uint8_t *)p->index_data + p->crc_offset))
		+ (size_t)p->অঙ্ক_objects /* CRC32 table */
		);

	/* The address of the 8-byte offset table */
	idx2 = idx1 + p->অঙ্ক_objects;

	বাবে (i = 0; i < p->অঙ্ক_objects; i++) {
		uint32_t off = ntohl(idx1[i]);
		যদি (!(off & 0x80000000))
			অনুৱর্ত;
		off = off & 0x7fffffff;
		check_pack_index_ptr(p, &idx2[off * 2]);
		যদি (idx2[off * 2])
			অনুৱর্ত;
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

স্থির শূন্য read_idx_option(সংরচনা pack_idx_option *opts, ধ্রুৱ ৱর্ণ *pack_name)
{
	সংরচনা packed_git *p = add_packed_git(the_repository, pack_name,
					      strlen(pack_name), 1);

	যদি (!p)
		die(_("Cannot open existing pack file '%s'"), pack_name);
	যদি (open_pack_index(p))
		die(_("Cannot open existing pack idx file for '%s'"), pack_name);

	/* Read the attributes from the existing idx file */
	opts->version = p->index_version;

	যদি (opts->version == 2)
		read_v2_anomalous_offsets(p, opts);

	/*
	 * Get rid of the idx file as we do not need it anymore.
	 * NEEDSWORK: extract this bit from free_pack_by_name() in
	 * object-file.c, perhaps?  It shouldn't matter very much as we
	 * know we haven't installed this pack (hence we never have
	 * read anything from it).
	 */
	close_pack_index(p);
	মুক্ত(p);
}

স্থির শূন্য show_pack_info(পূর্ণাঙ্ক stat_only)
{
	পূর্ণাঙ্ক i, baseobjects = nr_objects - nr_ref_deltas - nr_ofs_deltas;
	unsigned long *chain_histogram = রিক্ত;

	যদি (deepest_delta)
		CALLOC_ARRAY(chain_histogram, deepest_delta);

	বাবে (i = 0; i < nr_objects; i++) {
		সংরচনা object_entry *obj = &objects[i];

		যদি (is_delta_type(obj->type))
			chain_histogram[obj_stat[i].delta_depth - 1]++;
		যদি (stat_only)
			অনুৱর্ত;
		printf("%s %-6s %"PRIuMAX" %"PRIuMAX" %"PRIuMAX,
		       oid_to_hex(&obj->idx.oid),
		       type_name(obj->real_type), (uintmax_t)obj->size,
		       (uintmax_t)(obj[1].idx.offset - obj->idx.offset),
		       (uintmax_t)obj->idx.offset);
		যদি (is_delta_type(obj->type)) {
			সংরচনা object_entry *bobj = &objects[obj_stat[i].base_object_no];
			printf(" %u %s", obj_stat[i].delta_depth,
			       oid_to_hex(&bobj->idx.oid));
		}
		putchar('\n');
	}

	যদি (baseobjects)
		printf_ln(Q_("non delta: %d object",
			     "non delta: %d objects",
			     baseobjects),
			  baseobjects);
	বাবে (i = 0; i < deepest_delta; i++) {
		যদি (!chain_histogram[i])
			অনুৱর্ত;
		printf_ln(Q_("chain length = %d: %lu object",
			     "chain length = %d: %lu objects",
			     chain_histogram[i]),
			  i + 1,
			  chain_histogram[i]);
	}
	মুক্ত(chain_histogram);
}

স্থির শূন্য repack_local_links(শূন্য)
{
	সংরচনা child_process cmd = CHILD_PROCESS_INIT;
	FILE *out;
	সংরচনা strbuf সাৰি = STRBUF_INIT;
	সংরচনা oidset_iter iter;
	সংরচনা object_id *oid;
	ৱর্ণ *base_name = রিক্ত;

	যদি (!oidset_size(&outgoing_links))
		ঘূৰাই;

	oidset_iter_init(&outgoing_links, &iter);
	যেতিয়ালৈ ((oid = oidset_iter_next(&iter))) {
		সংরচনা odb_source_info source_info;
		সংরচনা object_info info = {
			.source_infop = &source_info,
		};

		যদি (odb_read_object_info_extended(the_repository->objects, oid, &info, 0))
			/* Missing; assume it is a promisor object */
			অনুৱর্ত;
		যদি (source_info.source->type == ODB_SOURCE_PACKED &&
		    source_info.u.packed.pack->pack_promisor)
			অনুৱর্ত;

		যদি (!cmd.args.nr) {
			base_name = mkpathdup(
				"%s/pack/pack",
				repo_get_object_directory(the_repository));
			strvec_ঠেলো(&cmd.args, "pack-objects");
			strvec_ঠেলো(&cmd.args,
				    "--exclude-promisor-objects-best-effort");
			strvec_ঠেলো(&cmd.args, base_name);
			cmd.git_cmd = 1;
			cmd.in = -1;
			cmd.out = -1;
			যদি (start_command(&cmd))
				die(_("could not start pack-objects to repack local links"));
		}

		যদি (write_in_full(cmd.in, oid_to_hex(oid), the_hash_algo->hexsz) < 0 ||
		    write_in_full(cmd.in, "\n", 1) < 0)
			die(_("failed to feed local object to pack-objects"));
	}

	যদি (!cmd.args.nr)
		ঘূৰাই;

	close(cmd.in);

	out = xfdopen(cmd.out, "r");
	যেতিয়ালৈ (strbuf_getline_lf(&সাৰি, out) != EOF) {
		unsigned ৱর্ণ দ্বিমিক[GIT_MAX_RAWSZ];
		যদি (সাৰি.len != the_hash_algo->hexsz ||
		    !hex_to_bytes(দ্বিমিক, সাৰি.buf, সাৰি.len))
			die(_("index-pack: Expecting full hex object ID lines only from pack-objects."));

		/*
		 * pack-objects creates the .pack and .idx files, but not the
		 * .promisor file. Create the .promisor file, which is empty.
		 */
		write_special_file("promisor", "", রিক্ত, দ্বিমিক, রিক্ত);
	}

	fclose(out);
	যদি (finish_command(&cmd))
		die(_("could not finish pack-objects to repack local links"));
	strbuf_release(&সাৰি);
	মুক্ত(base_name);
}

পূর্ণাঙ্ক cmd_index_pack(পূর্ণাঙ্ক argc,
		   ধ্রুৱ ৱর্ণ **argv,
		   ধ্রুৱ ৱর্ণ *prefix,
		   সংরচনা repository *repo)
{
	পূর্ণাঙ্ক i, fix_thin_pack = 0, verify = 0, stat_only = 0, rev_index;
	ধ্রুৱ ৱর্ণ *curr_index;
	ৱর্ণ *curr_rev_index = রিক্ত;
	ধ্রুৱ ৱর্ণ *index_name = রিক্ত, *pack_name = রিক্ত, *rev_index_name = রিক্ত;
	ধ্রুৱ ৱর্ণ *keep_বাৰ্তা = রিক্ত;
	ধ্রুৱ ৱর্ণ *promisor_বাৰ্তা = রিক্ত;
	সংরচনা strbuf index_name_buf = STRBUF_INIT;
	সংরচনা strbuf rev_index_name_buf = STRBUF_INIT;
	সংরচনা pack_idx_entry **idx_objects;
	সংরচনা pack_idx_option opts;
	unsigned ৱর্ণ pack_hash[GIT_MAX_RAWSZ];
	unsigned foreign_nr = 1;	/* zero is a "good" value, assume bad */
	পূর্ণাঙ্ক report_end_of_প্ৰবেশ = 0;
	পূর্ণাঙ্ক hash_algo = 0;

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
	যদি (prefix && chdir(prefix))
		die(_("Cannot come back to cwd"));

	যদি (git_env_bool(GIT_TEST_NO_WRITE_REV_INDEX, 0))
		rev_index = 0;
	অন্যথা
		rev_index = !!(opts.flags & (WRITE_REV_VERIFY | WRITE_REV));

	বাবে (i = 1; i < argc; i++) {
		ধ্রুৱ ৱর্ণ *arg = argv[i];

		যদি (*arg == '-') {
			যদি (!strcmp(arg, "--stdin")) {
				from_stdin = 1;
			} অন্যথা যদি (!strcmp(arg, "--fix-thin")) {
				fix_thin_pack = 1;
			} অন্যথা যদি (skip_to_optional_arg(arg, "--strict", &arg)) {
				strict = 1;
				do_fsck_object = 1;
				fsck_set_বাৰ্তা_types(&fsck_options, arg);
			} অন্যথা যদি (!strcmp(arg, "--check-self-contained-and-connected")) {
				strict = 1;
				check_self_contained_and_connected = 1;
			} অন্যথা যদি (skip_to_optional_arg(arg, "--fsck-objects", &arg)) {
				do_fsck_object = 1;
				fsck_set_বাৰ্তা_types(&fsck_options, arg);
			} অন্যথা যদি (!strcmp(arg, "--verify")) {
				verify = 1;
			} অন্যথা যদি (!strcmp(arg, "--verify-stat")) {
				verify = 1;
				show_stat = 1;
			} অন্যথা যদি (!strcmp(arg, "--verify-stat-only")) {
				verify = 1;
				show_stat = 1;
				stat_only = 1;
			} অন্যথা যদি (skip_to_optional_arg(arg, "--keep", &keep_বাৰ্তা)) {
				; /* nothing to do */
			} অন্যথা যদি (skip_to_optional_arg(arg, "--promisor", &promisor_বাৰ্তা)) {
				record_outgoing_links = 1;
			} অন্যথা যদি (starts_with(arg, "--threads=")) {
				ৱর্ণ *end;
				nr_threads = strtoul(arg+10, &end, 0);
				যদি (!arg[10] || *end || nr_threads < 0)
					usage(index_pack_usage);
				যদি (!HAVE_THREADS && nr_threads != 1) {
					warning(_("no threads support, ignoring %s"), arg);
					nr_threads = 1;
				}
			} অন্যথা যদি (skip_prefix(arg, "--pack_header=", &arg)) {
				যদি (parse_pack_header_option(arg,
							     প্ৰবেশ_buffer,
							     &প্ৰবেশ_len) < 0)
					die(_("bad --pack_header: %s"), arg);
			} অন্যথা যদি (!strcmp(arg, "-v")) {
				verbose = 1;
			} অন্যথা যদি (!strcmp(arg, "--progress-title")) {
				যদি (progress_title || (i+1) >= argc)
					usage(index_pack_usage);
				progress_title = argv[++i];
			} অন্যথা যদি (!strcmp(arg, "--show-resolving-progress")) {
				show_resolving_progress = 1;
			} অন্যথা যদি (!strcmp(arg, "--report-end-of-input")) {
				report_end_of_প্ৰবেশ = 1;
			} অন্যথা যদি (!strcmp(arg, "-o")) {
				যদি (index_name || (i+1) >= argc)
					usage(index_pack_usage);
				index_name = argv[++i];
			} অন্যথা যদি (starts_with(arg, "--index-version=")) {
				ৱর্ণ *c;
				opts.version = strtoul(arg + 16, &c, 10);
				যদি (opts.version > 2)
					die(_("bad %s"), arg);
				যদি (*c == ',')
					opts.off32_limit = strtoul(c+1, &c, 0);
				যদি (*c || opts.off32_limit & 0x80000000)
					die(_("bad %s"), arg);
			} অন্যথা যদি (skip_prefix(arg, "--max-input-size=", &arg)) {
				max_প্ৰবেশ_size = strtoumax(arg, রিক্ত, 10);
			} অন্যথা যদি (skip_prefix(arg, "--object-format=", &arg)) {
				hash_algo = hash_algo_by_name(arg);
				যদি (hash_algo == GIT_HASH_UNKNOWN)
					die(_("unknown hash algorithm '%s'"), arg);
				repo_set_hash_algo(the_repository, hash_algo);
			} অন্যথা যদি (!strcmp(arg, "--rev-index")) {
				rev_index = 1;
			} অন্যথা যদি (!strcmp(arg, "--no-rev-index")) {
				rev_index = 0;
			} অন্যথা
				usage(index_pack_usage);
			অনুৱর্ত;
		}

		যদি (pack_name)
			usage(index_pack_usage);
		pack_name = arg;
	}

	যদি (!pack_name && !from_stdin)
		usage(index_pack_usage);
	যদি (fix_thin_pack && !from_stdin)
		die(_("the option '%s' requires '%s'"), "--fix-thin", "--stdin");
	যদি (promisor_বাৰ্তা && pack_name)
		die(_("--promisor cannot be used with a pack name"));
	যদি (from_stdin && !startup_info->have_repository)
		die(_("--stdin requires a git repository"));
	যদি (from_stdin && hash_algo)
		die(_("options '%s' and '%s' cannot be used together"), "--object-format", "--stdin");
	যদি (!index_name && pack_name)
		index_name = derive_filename(pack_name, "pack", "idx", &index_name_buf);

	/*
	 * Packfiles and indices do not carry enough information to be able to
	 * identify their object hash. So when we are neither in a repository
	 * nor has the user told us which object hash to use we have no other
	 * choice but to guess the object hash.
	 */
	যদি (!the_repository->hash_algo)
		repo_set_hash_algo(the_repository, GIT_HASH_DEFAULT);

	opts.flags &= ~(WRITE_REV | WRITE_REV_VERIFY);
	যদি (rev_index) {
		opts.flags |= verify ? WRITE_REV_VERIFY : WRITE_REV;
		যদি (index_name)
			rev_index_name = derive_filename(index_name,
							 "idx", "rev",
							 &rev_index_name_buf);
	}

	যদি (verify) {
		যদি (!index_name)
			die(_("--verify with no packfile name given"));
		read_idx_option(&opts, index_name);
		opts.flags |= WRITE_IDX_VERIFY | WRITE_IDX_STRICT;
	}
	যদি (strict)
		opts.flags |= WRITE_IDX_STRICT;

	যদি (HAVE_THREADS && !nr_threads) {
		nr_threads = online_cpus();
		/*
		 * Experiments show that going above 20 threads doesn't help,
		 * no matter how many cores you have. Below that, we tend to
		 * max at half the number of online_cpus(), presumably because
		 * half of those are hyperthreads rather than full cores. We'll
		 * never reduce the level below "3", though, to match a
		 * historical value that nobody complained about.
		 */
		যদি (nr_threads < 4)
			; /* too few cores to consider capping */
		অন্যথা যদি (nr_threads < 6)
			nr_threads = 3; /* historic cap */
		অন্যথা যদি (nr_threads < 40)
			nr_threads /= 2;
		অন্যথা
			nr_threads = 20; /* hard cap */
	}

	curr_pack = open_pack_file(pack_name);
	parse_pack_header();
	CALLOC_ARRAY(objects, st_add(nr_objects, 1));
	যদি (show_stat)
		CALLOC_ARRAY(obj_stat, st_add(nr_objects, 1));
	CALLOC_ARRAY(ofs_deltas, nr_objects);
	parse_pack_objects(pack_hash);
	যদি (report_end_of_প্ৰবেশ)
		write_in_full(2, "\0", 1);
	resolve_deltas(&opts);
	conclude_pack(fix_thin_pack, curr_pack, pack_hash);
	মুক্ত(ofs_deltas);
	মুক্ত(ref_deltas);
	যদি (strict)
		foreign_nr = check_objects();

	যদি (show_stat)
		show_pack_info(stat_only);

	ALLOC_ARRAY(idx_objects, nr_objects);
	বাবে (i = 0; i < nr_objects; i++)
		idx_objects[i] = &objects[i].idx;
	curr_index = write_idx_file(the_repository, index_name, idx_objects,
				    nr_objects, &opts, pack_hash);
	যদি (rev_index)
		curr_rev_index = write_rev_file(the_repository, rev_index_name,
						idx_objects, nr_objects,
						pack_hash, opts.flags);
	মুক্ত(idx_objects);

	যদি (!verify)
		final(pack_name, curr_pack,
		      index_name, curr_index,
		      rev_index_name, curr_rev_index,
		      keep_বাৰ্তা, promisor_বাৰ্তা,
		      pack_hash);
	অন্যথা
		close(প্ৰবেশ_fd);

	যদি (do_fsck_object) {
		/*
		 * We cannot perform queued consistency checks when running
		 * outside of a repository because those require us to read
		 * from the object database, which is uninitialized.
		 *
		 * TODO: we may eventually set up an in-memory object database,
		 * which would allow us to perform these queued checks.
		 */
		যদি (!startup_info->have_repository &&
		    fsck_has_queued_checks(&fsck_options))
			die(_("cannot perform queued object checks outside "
			      "of a repository"));

		যদি (fsck_finish(&fsck_options))
			die(_("fsck error in pack objects"));
	}

	মুক্ত(opts.anomaly);
	মুক্ত(objects);
	strbuf_release(&index_name_buf);
	strbuf_release(&rev_index_name_buf);
	যদি (!pack_name)
		মুক্ত((শূন্য *) curr_pack);
	যদি (!index_name)
		মুক্ত((শূন্য *) curr_index);
	মুক্ত(curr_rev_index);

	repack_local_links();

	/*
	 * Let the caller know this pack is not self contained
	 */
	যদি (check_self_contained_and_connected && foreign_nr)
		ঘূৰাই 1;

	ঘূৰাই 0;
}
