#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#شامل "builtin.h"
#شامل "config.h"
#شامل "delta.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "hex.h"
#شامل "pack.h"
#شامل "csum-file.h"
#شامل "blob.h"
#شامل "commit.h"
#شامل "tag.h"
#شامل "tree.h"
#شامل "progress.h"
#شامل "fsck.h"
#شامل "strbuf.h"
#شامل "thread-utils.h"
#شامل "packfile.h"
#شامل "pack-revindex.h"
#شامل "object-file.h"
#شامل "odb.h"
#شامل "odb/streaming.h"
#شامل "oid-array.h"
#شامل "oidset.h"
#شامل "path.h"
#شامل "replace-object.h"
#شامل "tree-walk.h"
#شامل "promisor-remote.h"
#شامل "run-command.h"
#شامل "setup.h"
#شامل "strvec.h"

ایستا ثابت نویسه index_pack_usage[] =
"git index-pack [-v] [-o <index-file>] [--keep | --keep=<msg>] [--[no-]rev-index] [--verify] [--strict[=<msg-id>=<severity>...]] [--fsck-objects[=<msg-id>=<severity>...]] (<pack-file> | --stdin [--fix-thin] [<pack-file>])";

ساختار object_entry {
	ساختار pack_idx_entry idx;
	size_t size;
	unsigned نویسه hdr_size;
	signed نویسه type;
	signed نویسه real_type;
};

ساختار object_stat {
	unsigned delta_depth;
	صحیح base_object_no;
};

ساختار base_data {
	/* Initialized by make_base(). */
	ساختار base_data *base;
	ساختار object_entry *obj;
	صحیح ref_first, ref_last;
	صحیح ofs_first, ofs_last;
	/*
	 * Threads should increment retain_data if they are about to call
	 * patch_delta() using this struct's data as a base, and decrement this
	 * when they are done. While retain_data is nonzero, this struct's data
	 * will not be freed even if the delta base cache limit is exceeded.
	 */
	صحیح retain_data;
	/*
	 * The number of direct children that have not been fully processed
	 * (entered work_head, entered done_head, left done_head). When this
	 * number reaches zero, this struct base_data can be freed.
	 */
	صحیح children_remaining;

	/* Not initialized by make_base(). */
	ساختار list_head list;
	تهی *data;
	size_t size;
};

/*
 * Stack of struct base_data that have unprocessed children.
 * threaded_second_pass() uses this as a source of work (the other being the
 * objects array).
 *
 * Guarded by work_mutex.
 */
ایستا LIST_HEAD(work_head);

/*
 * Stack of struct base_data that have children, all of whom have been
 * processed or are being processed, and at least one child is being processed.
 * These struct base_data must be kept around until the last child is
 * processed.
 *
 * Guarded by work_mutex.
 */
ایستا LIST_HEAD(done_head);

/*
 * All threads share one delta base cache.
 *
 * base_cache_used is guarded by work_mutex, and base_cache_limit is read-only
 * in a thread.
 */
ایستا size_t base_cache_used;
ایستا size_t base_cache_limit;

ساختار thread_local_data {
	pthread_t thread;
	صحیح pack_fd;
};

/* Remember to update object flag allocation in object.h */
#define FLAG_LINK (1u<<20)
#define FLAG_CHECKED (1u<<21)

ساختار ofs_delta_entry {
	off_t offset;
	صحیح obj_no;
};

ساختار ref_delta_entry {
	ساختار object_id oid;
	صحیح obj_no;
};

ایستا ساختار object_entry *objects;
ایستا ساختار object_stat *obj_stat;
ایستا ساختار ofs_delta_entry *ofs_deltas;
ایستا ساختار ref_delta_entry *ref_deltas;
ایستا ساختار thread_local_data nothread_data;
ایستا صحیح nr_objects;
ایستا صحیح nr_ofs_deltas;
ایستا صحیح nr_ref_deltas;
ایستا صحیح ref_deltas_alloc;
ایستا صحیح nr_resolved_deltas;
ایستا صحیح nr_threads;

ایستا صحیح from_stdin;
ایستا صحیح strict;
ایستا صحیح do_fsck_object;
ایستا ساختار fsck_options fsck_options;
ایستا صحیح verbose;
ایستا ثابت نویسه *progress_title;
ایستا صحیح show_resolving_progress;
ایستا صحیح show_stat;
ایستا صحیح check_self_contained_and_connected;

ایستا ساختار progress *progress;

ایستا unsigned نویسه درون_داد_buffer[DEFAULT_IO_BUFFER_SIZE];
ایستا unsigned صحیح درون_داد_offset, درون_داد_len;
ایستا off_t consumed_bytes;
ایستا off_t max_درون_داد_size;
ایستا unsigned deepest_delta;
ایستا ساختار git_hash_ctx درون_داد_ctx;
ایستا uint32_t درون_داد_crc32;
ایستا صحیح درون_داد_fd, output_fd;
ایستا ثابت نویسه *curr_pack;

/*
 * outgoing_links is guarded by read_mutex, and record_outgoing_links is
 * read-only in a thread.
 */
ایستا ساختار oidset outgoing_links = OIDSET_INIT;
ایستا صحیح record_outgoing_links;

ایستا ساختار thread_local_data *thread_data;
ایستا صحیح nr_dispatched;
ایستا صحیح threads_active;

ایستا pthread_mutex_t read_mutex;
#define read_lock()		lock_mutex(&read_mutex)
#define read_unlock()		unlock_mutex(&read_mutex)

ایستا pthread_mutex_t counter_mutex;
#define counter_lock()		lock_mutex(&counter_mutex)
#define counter_unlock()	unlock_mutex(&counter_mutex)

ایستا pthread_mutex_t work_mutex;
#define work_lock()		lock_mutex(&work_mutex)
#define work_unlock()		unlock_mutex(&work_mutex)

ایستا pthread_mutex_t deepest_delta_mutex;
#define deepest_delta_lock()	lock_mutex(&deepest_delta_mutex)
#define deepest_delta_unlock()	unlock_mutex(&deepest_delta_mutex)

ایستا pthread_کلید_t کلید;

ایستا درونخطی تهی lock_mutex(pthread_mutex_t *mutex)
{
	اگر (threads_active)
		pthread_mutex_lock(mutex);
}

ایستا درونخطی تهی unlock_mutex(pthread_mutex_t *mutex)
{
	اگر (threads_active)
		pthread_mutex_unlock(mutex);
}

/*
 * Mutex and conditional variable can't be statically-initialized on Windows.
 */
ایستا تهی init_thread(تهی)
{
	صحیح i;
	init_recursive_mutex(&read_mutex);
	pthread_mutex_init(&counter_mutex, پوچ);
	pthread_mutex_init(&work_mutex, پوچ);
	اگر (show_stat)
		pthread_mutex_init(&deepest_delta_mutex, پوچ);
	pthread_کلید_create(&کلید, پوچ);
	CALLOC_ARRAY(thread_data, nr_threads);
	برای (i = 0; i < nr_threads; i++) {
		thread_data[i].pack_fd = xopen(curr_pack, O_RDONLY);
	}

	threads_active = 1;
}

ایستا تهی cleanup_thread(تهی)
{
	صحیح i;
	اگر (!threads_active)
		بازگشت;
	threads_active = 0;
	pthread_mutex_destroy(&read_mutex);
	pthread_mutex_destroy(&counter_mutex);
	pthread_mutex_destroy(&work_mutex);
	اگر (show_stat)
		pthread_mutex_destroy(&deepest_delta_mutex);
	برای (i = 0; i < nr_threads; i++)
		close(thread_data[i].pack_fd);
	pthread_کلید_delete(کلید);
	آزاد(thread_data);
}

ایستا صحیح mark_link(ساختار object *obj, شمارش object_type type,
		     تهی *data UNUSED,
		     ساختار fsck_options *options UNUSED)
{
	اگر (!obj)
		بازگشت -1;

	اگر (type != OBJ_ANY && obj->type != type)
		die(_("object type mismatch at %s"), oid_to_hex(&obj->oid));

	obj->flags |= FLAG_LINK;
	بازگشت 0;
}

/* The content of each linked object must have been checked
   or it must be already present in the object database */
ایستا unsigned check_object(ساختار object *obj)
{
	اگر (!obj)
		بازگشت 0;

	اگر (!(obj->flags & FLAG_LINK))
		بازگشت 0;

	اگر (!(obj->flags & FLAG_CHECKED)) {
		size_t size;
		صحیح type = odb_read_object_info(the_repository->objects,
						&obj->oid, &size);
		اگر (type <= 0)
			die(_("did not receive expected object %s"),
			      oid_to_hex(&obj->oid));
		اگر (type != obj->type)
			die(_("object %s: expected type %s, found %s"),
			    oid_to_hex(&obj->oid),
			    type_name(obj->type), type_name(type));
		obj->flags |= FLAG_CHECKED;
		بازگشت 1;
	}

	بازگشت 0;
}

ایستا unsigned check_objects(تهی)
{
	unsigned i, max, foreign_nr = 0;

	max = get_max_object_index(the_repository);

	اگر (verbose)
		progress = start_delayed_progress(the_repository,
						  _("Checking objects"), max);

	برای (i = 0; i < max; i++) {
		foreign_nr += check_object(get_indexed_object(the_repository, i));
		display_progress(progress, i + 1);
	}

	stop_progress(&progress);
	بازگشت foreign_nr;
}


/* Discard current buffer used content. */
ایستا تهی flush(تهی)
{
	اگر (درون_داد_offset) {
		اگر (output_fd >= 0)
			write_or_die(output_fd, درون_داد_buffer, درون_داد_offset);
		git_hash_update(&درون_داد_ctx, درون_داد_buffer, درون_داد_offset);
		memmove(درون_داد_buffer, درون_داد_buffer + درون_داد_offset, درون_داد_len);
		درون_داد_offset = 0;
	}
}

/*
 * Make sure at least "min" bytes are available in the buffer, and
 * return the pointer to the buffer.
 */
ایستا تهی *fill(صحیح min)
{
	اگر (min <= درون_داد_len)
		بازگشت درون_داد_buffer + درون_داد_offset;
	اگر (min > اندازه(درون_داد_buffer))
		die(Q_("cannot fill %d byte",
		       "cannot fill %d bytes",
		       min),
		    min);
	flush();
	انجامبده {
		ssize_t ret = xread(درون_داد_fd, درون_داد_buffer + درون_داد_len,
				اندازه(درون_داد_buffer) - درون_داد_len);
		اگر (ret <= 0) {
			اگر (!ret)
				die(_("early EOF"));
			die_errno(_("read error on input"));
		}
		درون_داد_len += ret;
		اگر (from_stdin)
			display_throughput(progress, consumed_bytes + درون_داد_len);
	} وقتی (درون_داد_len < min);
	بازگشت درون_داد_buffer;
}

ایستا تهی use(صحیح bytes)
{
	اگر (bytes > درون_داد_len)
		die(_("used more bytes than were available"));
	درون_داد_crc32 = crc32(درون_داد_crc32, درون_داد_buffer + درون_داد_offset, bytes);
	درون_داد_len -= bytes;
	درون_داد_offset += bytes;

	/* make sure off_t is sufficiently large not to wrap */
	اگر (signed_add_overflows(consumed_bytes, bytes))
		die(_("pack too large for current definition of off_t"));
	consumed_bytes += bytes;
	اگر (max_درون_داد_size && consumed_bytes > max_درون_داد_size) {
		ساختار strbuf size_limit = STRBUF_INIT;
		strbuf_humanise_bytes(&size_limit, max_درون_داد_size);
		die(_("pack exceeds maximum allowed size (%s)"),
		    size_limit.buf);
	}
}

ایستا ثابت نویسه *open_pack_file(ثابت نویسه *pack_name)
{
	اگر (from_stdin) {
		درون_داد_fd = 0;
		اگر (!pack_name) {
			ساختار strbuf tmp_file = STRBUF_INIT;
			output_fd = odb_mkstemp(the_repository->objects, &tmp_file,
						"pack/tmp_pack_XXXXXX");
			pack_name = strbuf_detach(&tmp_file, پوچ);
		} وگرنه {
			output_fd = xopen(pack_name, O_CREAT|O_EXCL|O_RDWR, 0600);
		}
		nothread_data.pack_fd = output_fd;
	} وگرنه {
		درون_داد_fd = xopen(pack_name, O_RDONLY);
		output_fd = -1;
		nothread_data.pack_fd = درون_داد_fd;
	}
	git_hash_init(&درون_داد_ctx, the_hash_algo);
	بازگشت pack_name;
}

ایستا تهی parse_pack_header(تهی)
{
	unsigned نویسه *hdr = fill(اندازه(ساختار pack_header));

	/* Header consistency check */
	اگر (get_be32(hdr) != PACK_SIGNATURE)
		die(_("pack signature mismatch"));
	hdr += 4;
	اگر (!pack_version_ok_native(get_be32(hdr)))
		die(_("pack version %"PRIu32" unsupported"),
		    get_be32(hdr));
	hdr += 4;

	nr_objects = get_be32(hdr);
	use(اندازه(ساختار pack_header));
}

__attribute__((format (printf, 2, 3)))
ایستا NORETURN تهی bad_object(off_t offset, ثابت نویسه *format, ...)
{
	va_list params;
	نویسه buf[1024];

	va_start(params, format);
	vsnprintf(buf, اندازه(buf), format, params);
	va_end(params);
	die(_("pack has bad object at offset %"PRIuMAX": %s"),
	    (uintmax_t)offset, buf);
}

ایستا درونخطی ساختار thread_local_data *get_thread_data(تهی)
{
	اگر (HAVE_THREADS) {
		اگر (threads_active)
			بازگشت pthread_getspecific(کلید);
		أكد(!threads_active &&
		       "This should only be reached when all threads are gone");
	}
	بازگشت &nothread_data;
}

ایستا تهی set_thread_data(ساختار thread_local_data *data)
{
	اگر (threads_active)
		pthread_setspecific(کلید, data);
}

ایستا تهی free_base_data(ساختار base_data *c)
{
	اگر (c->data) {
		FREE_AND_NULL(c->data);
		base_cache_used -= c->size;
	}
}

ایستا تهی prune_base_data(ساختار base_data *retain)
{
	ساختار list_head *pos;

	اگر (base_cache_used <= base_cache_limit)
		بازگشت;

	list_for_each_prev(pos, &done_head) {
		ساختار base_data *b = list_entry(pos, ساختار base_data, list);
		اگر (b->retain_data || b == retain)
			ادامه;
		اگر (b->data) {
			free_base_data(b);
			اگر (base_cache_used <= base_cache_limit)
				بازگشت;
		}
	}

	list_for_each_prev(pos, &work_head) {
		ساختار base_data *b = list_entry(pos, ساختار base_data, list);
		اگر (b->retain_data || b == retain)
			ادامه;
		اگر (b->data) {
			free_base_data(b);
			اگر (base_cache_used <= base_cache_limit)
				بازگشت;
		}
	}
}

ایستا صحیح is_delta_type(شمارش object_type type)
{
	بازگشت (type == OBJ_REF_DELTA || type == OBJ_OFS_DELTA);
}

ایستا تهی *unpack_entry_data(off_t offset, size_t size,
			       شمارش object_type type, ساختار object_id *oid)
{
	ایستا نویسه fixed_buf[8192];
	صحیح status;
	git_zstream stream;
	تهی *buf;
	ساختار git_hash_ctx c;
	نویسه hdr[32];
	صحیح hdrlen;

	اگر (!is_delta_type(type)) {
		hdrlen = format_object_header(hdr, اندازه(hdr), type, size);
		git_hash_init(&c, the_hash_algo);
		git_hash_update(&c, hdr, hdrlen);
	} وگرنه
		oid = پوچ;
	اگر (type == OBJ_BLOB &&
	    size > repo_settings_get_big_file_threshold(the_repository))
		buf = fixed_buf;
	وگرنه
		buf = xmallocz(size);

	memset(&stream, 0, اندازه(stream));
	git_inflate_init(&stream);
	stream.next_out = buf;
	stream.avail_out = buf == fixed_buf ? اندازه(fixed_buf) : size;

	انجامبده {
		unsigned نویسه *last_out = stream.next_out;
		stream.next_in = fill(1);
		stream.avail_in = درون_داد_len;
		status = git_inflate(&stream, 0);
		use(درون_داد_len - stream.avail_in);
		اگر (oid)
			git_hash_update(&c, last_out, stream.next_out - last_out);
		اگر (buf == fixed_buf) {
			stream.next_out = buf;
			stream.avail_out = اندازه(fixed_buf);
		}
	} وقتی (status == Z_OK);
	اگر (stream.total_out != size || status != Z_STREAM_END)
		bad_object(offset, _("inflate returned %d"), status);
	git_inflate_end(&stream);
	اگر (oid)
		git_hash_final_oid(oid, &c);
	بازگشت buf == fixed_buf ? پوچ : buf;
}

ایستا تهی *unpack_raw_entry(ساختار object_entry *obj,
			      off_t *ofs_offset,
			      ساختار object_id *ref_oid,
			      ساختار object_id *oid)
{
	unsigned نویسه *p;
	size_t size, c;
	off_t base_offset;
	unsigned shift;
	تهی *data;

	obj->idx.offset = consumed_bytes;
	درون_داد_crc32 = crc32(0, پوچ, 0);

	p = fill(1);
	c = *p;
	use(1);
	obj->type = (c >> 4) & 7;
	size = (c & 15);
	shift = 4;
	وقتی (c & 0x80) {
		اگر ((bitsizeof(size_t) - 7) < shift)
			die(_("object size too large for this platform"));
		p = fill(1);
		c = *p;
		use(1);
		size += (c & 0x7f) << shift;
		shift += 7;
	}
	obj->size = size;

	انتخاب (obj->type) {
	حالت OBJ_REF_DELTA:
		oidread(ref_oid, fill(the_hash_algo->rawsz),
			the_repository->hash_algo);
		use(the_hash_algo->rawsz);
		بشکن;
	حالت OBJ_OFS_DELTA:
		p = fill(1);
		c = *p;
		use(1);
		base_offset = c & 127;
		وقتی (c & 128) {
			base_offset += 1;
			اگر (!base_offset || MSB(base_offset, 7))
				bad_object(obj->idx.offset, _("offset value overflow for delta base object"));
			p = fill(1);
			c = *p;
			use(1);
			base_offset = (base_offset << 7) + (c & 127);
		}
		*ofs_offset = obj->idx.offset - base_offset;
		اگر (*ofs_offset <= 0 || *ofs_offset >= obj->idx.offset)
			bad_object(obj->idx.offset, _("delta base offset is out of bound"));
		بشکن;
	حالت OBJ_COMMIT:
	حالت OBJ_درخت:
	حالت OBJ_BLOB:
	حالت OBJ_TAG:
		بشکن;
	پیشفرض:
		bad_object(obj->idx.offset, _("unknown object type %d"), obj->type);
	}
	obj->hdr_size = consumed_bytes - obj->idx.offset;

	data = unpack_entry_data(obj->idx.offset, obj->size, obj->type, oid);
	obj->idx.crc32 = درون_داد_crc32;
	بازگشت data;
}

ایستا تهی *unpack_data(ساختار object_entry *obj,
			 صحیح (*consume)(ثابت unsigned نویسه *, unsigned long, تهی *),
			 تهی *cb_data)
{
	off_t from = obj[0].idx.offset + obj[0].hdr_size;
	off_t len = obj[1].idx.offset - from;
	unsigned نویسه *data, *inbuf;
	git_zstream stream;
	صحیح status;

	data = xmallocz(consume ? 64*1024 : obj->size);
	inbuf = xmalloc((len < 64*1024) ? (صحیح)len : 64*1024);

	memset(&stream, 0, اندازه(stream));
	git_inflate_init(&stream);
	stream.next_out = data;
	stream.avail_out = consume ? 64*1024 : obj->size;

	انجامبده {
		ssize_t n = (len < 64*1024) ? (ssize_t)len : 64*1024;
		n = xpread(get_thread_data()->pack_fd, inbuf, n, from);
		اگر (n < 0)
			die_errno(_("cannot pread pack file"));
		اگر (!n)
			die(Q_("premature end of pack file, %"PRIuMAX" byte missing",
			       "premature end of pack file, %"PRIuMAX" bytes missing",
			       len),
			    (uintmax_t)len);
		from += n;
		len -= n;
		stream.next_in = inbuf;
		stream.avail_in = n;
		اگر (!consume)
			status = git_inflate(&stream, 0);
		وگرنه {
			انجامبده {
				status = git_inflate(&stream, 0);
				اگر (consume(data, stream.next_out - data, cb_data)) {
					آزاد(inbuf);
					آزاد(data);
					بازگشت پوچ;
				}
				stream.next_out = data;
				stream.avail_out = 64*1024;
			} وقتی (status == Z_OK && stream.avail_in);
		}
	} وقتی (len && status == Z_OK && !stream.avail_in);

	/* This has been inflated OK when first encountered, so... */
	اگر (status != Z_STREAM_END || stream.total_out != obj->size)
		die(_("serious inflate inconsistency"));

	git_inflate_end(&stream);
	آزاد(inbuf);
	اگر (consume) {
		FREE_AND_NULL(data);
	}
	بازگشت data;
}

ایستا تهی *get_data_from_pack(ساختار object_entry *obj)
{
	بازگشت unpack_data(obj, پوچ, پوچ);
}

ایستا صحیح compare_ofs_delta_bases(off_t offset1, off_t offset2,
				   شمارش object_type type1,
				   شمارش object_type type2)
{
	صحیح cmp = type1 - type2;
	اگر (cmp)
		بازگشت cmp;
	بازگشت offset1 < offset2 ? -1 :
	       offset1 > offset2 ?  1 :
	       0;
}

ایستا صحیح find_ofs_delta(ثابت off_t offset)
{
	صحیح first = 0, last = nr_ofs_deltas;

	وقتی (first < last) {
		صحیح next = first + (last - first) / 2;
		ساختار ofs_delta_entry *delta = &ofs_deltas[next];
		صحیح cmp;

		cmp = compare_ofs_delta_bases(offset, delta->offset,
					      OBJ_OFS_DELTA,
					      objects[delta->obj_no].type);
		اگر (!cmp)
			بازگشت next;
		اگر (cmp < 0) {
			last = next;
			ادامه;
		}
		first = next+1;
	}
	بازگشت -first-1;
}

ایستا تهی find_ofs_delta_children(off_t offset,
				    صحیح *first_index, صحیح *last_index)
{
	صحیح first = find_ofs_delta(offset);
	صحیح last = first;
	صحیح end = nr_ofs_deltas - 1;

	اگر (first < 0) {
		*first_index = 0;
		*last_index = -1;
		بازگشت;
	}
	وقتی (first > 0 && ofs_deltas[first - 1].offset == offset)
		--first;
	وقتی (last < end && ofs_deltas[last + 1].offset == offset)
		++last;
	*first_index = first;
	*last_index = last;
}

ایستا صحیح compare_ref_delta_bases(ثابت ساختار object_id *oid1,
				   ثابت ساختار object_id *oid2,
				   شمارش object_type type1,
				   شمارش object_type type2)
{
	صحیح cmp = type1 - type2;
	اگر (cmp)
		بازگشت cmp;
	بازگشت oidcmp(oid1, oid2);
}

ایستا صحیح find_ref_delta(ثابت ساختار object_id *oid)
{
	صحیح first = 0, last = nr_ref_deltas;

	وقتی (first < last) {
		صحیح next = first + (last - first) / 2;
		ساختار ref_delta_entry *delta = &ref_deltas[next];
		صحیح cmp;

		cmp = compare_ref_delta_bases(oid, &delta->oid,
					      OBJ_REF_DELTA,
					      objects[delta->obj_no].type);
		اگر (!cmp)
			بازگشت next;
		اگر (cmp < 0) {
			last = next;
			ادامه;
		}
		first = next+1;
	}
	بازگشت -first-1;
}

ایستا تهی find_ref_delta_children(ثابت ساختار object_id *oid,
				    صحیح *first_index, صحیح *last_index)
{
	صحیح first = find_ref_delta(oid);
	صحیح last = first;
	صحیح end = nr_ref_deltas - 1;

	اگر (first < 0) {
		*first_index = 0;
		*last_index = -1;
		بازگشت;
	}
	وقتی (first > 0 && oideq(&ref_deltas[first - 1].oid, oid))
		--first;
	وقتی (last < end && oideq(&ref_deltas[last + 1].oid, oid))
		++last;
	*first_index = first;
	*last_index = last;
}

ساختار compare_data {
	ساختار object_entry *entry;
	ساختار odb_stream *st;
	unsigned نویسه *buf;
	unsigned long buf_size;
};

ایستا صحیح compare_objects(ثابت unsigned نویسه *buf, unsigned long size,
			   تهی *cb_data)
{
	ساختار compare_data *data = cb_data;

	اگر (data->buf_size < size) {
		آزاد(data->buf);
		data->buf = xmalloc(size);
		data->buf_size = size;
	}

	وقتی (size) {
		ssize_t len = odb_stream_read(data->st, data->buf, size);
		اگر (len == 0)
			die(_("SHA1 COLLISION FOUND WITH %s !"),
			    oid_to_hex(&data->entry->idx.oid));
		اگر (len < 0)
			die(_("unable to read %s"),
			    oid_to_hex(&data->entry->idx.oid));
		اگر (memcmp(buf, data->buf, len))
			die(_("SHA1 COLLISION FOUND WITH %s !"),
			    oid_to_hex(&data->entry->idx.oid));
		size -= len;
		buf += len;
	}
	بازگشت 0;
}

ایستا صحیح check_collison(ساختار object_entry *entry)
{
	ساختار compare_data data;

	اگر (entry->size <= repo_settings_get_big_file_threshold(the_repository) ||
	    entry->type != OBJ_BLOB)
		بازگشت -1;

	memset(&data, 0, اندازه(data));
	data.entry = entry;
	data.st = odb_stream_from_object(the_repository->objects, &entry->idx.oid, پوچ);
	اگر (!data.st)
		بازگشت -1;
	اگر (data.st->size != entry->size || data.st->type != entry->type)
		die(_("SHA1 COLLISION FOUND WITH %s !"),
		    oid_to_hex(&entry->idx.oid));
	unpack_data(entry, compare_objects, &data);
	odb_stream_close(data.st);
	آزاد(data.buf);
	بازگشت 0;
}

ایستا تهی record_outgoing_link(ثابت ساختار object_id *oid)
{
	oidset_درج(&outgoing_links, oid);
}

ایستا تهی maybe_record_name_entry(ثابت ساختار name_entry *entry)
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
	اگر (S_ISDIR(entry->mode))
		record_outgoing_link(&entry->oid);
}

ایستا تهی do_record_outgoing_links(ساختار object *obj)
{
	اگر (obj->type == OBJ_درخت) {
		ساختار درخت *درخت = (ساختار درخت *)obj;
		ساختار درخت_desc desc;
		ساختار name_entry entry;
		اگر (init_درخت_desc_gently(&desc, &درخت->object.oid,
					  درخت->buffer, درخت->size, 0))
			/*
			 * Error messages are given when packs are
			 * verified, so do not print any here.
			 */
			بازگشت;
		وقتی (درخت_entry_gently(&desc, &entry))
			maybe_record_name_entry(&entry);
	} وگرنه اگر (obj->type == OBJ_COMMIT) {
		ساختار commit *commit = (ساختار commit *) obj;
		ساختار commit_list *parents = commit->parents;

		record_outgoing_link(get_commit_درخت_oid(commit));
		برای (; parents; parents = parents->next)
			record_outgoing_link(&parents->item->object.oid);
	} وگرنه اگر (obj->type == OBJ_TAG) {
		ساختار tag *tag = (ساختار tag *) obj;
		record_outgoing_link(get_tagged_oid(tag));
	}
}

ایستا تهی sha1_object(ثابت تهی *data, ساختار object_entry *obj_entry,
			unsigned long size, شمارش object_type type,
			ثابت ساختار object_id *oid)
{
	تهی *new_data = پوچ;
	صحیح collision_test_needed = 0;

	أكد(data || obj_entry);

	اگر (startup_info->have_repository) {
		read_lock();
		collision_test_needed = odb_has_object(the_repository->objects, oid,
						       ODB_HAS_OBJECT_FETCH_PROMISOR);
		read_unlock();
	}

	اگر (collision_test_needed && !data) {
		read_lock();
		اگر (!check_collison(obj_entry))
			collision_test_needed = 0;
		read_unlock();
	}
	اگر (collision_test_needed) {
		تهی *has_data;
		شمارش object_type has_type;
		size_t has_size;
		read_lock();
		has_type = odb_read_object_info(the_repository->objects, oid, &has_size);
		اگر (has_type < 0)
			die(_("cannot read existing object info %s"), oid_to_hex(oid));
		اگر (has_type != type || has_size != size)
			die(_("SHA1 COLLISION FOUND WITH %s !"), oid_to_hex(oid));
		has_data = odb_read_object(the_repository->objects, oid,
					   &has_type, &has_size);
		read_unlock();
		اگر (!data)
			data = new_data = get_data_from_pack(obj_entry);
		اگر (!has_data)
			die(_("cannot read existing object %s"), oid_to_hex(oid));
		اگر (size != has_size || type != has_type ||
		    memcmp(data, has_data, size) != 0)
			die(_("SHA1 COLLISION FOUND WITH %s !"), oid_to_hex(oid));
		آزاد(has_data);
	}

	اگر (strict || do_fsck_object || record_outgoing_links) {
		read_lock();
		اگر (type == OBJ_BLOB) {
			ساختار blob *blob = lookup_blob(the_repository, oid);
			اگر (blob)
				blob->object.flags |= FLAG_CHECKED;
			وگرنه
				die(_("invalid blob object %s"), oid_to_hex(oid));
			اگر (do_fsck_object &&
			    fsck_object(&blob->object, (تهی *)data, size, &fsck_options))
				die(_("fsck error in packed object"));
		} وگرنه {
			ساختار object *obj;
			صحیح eaten;
			تهی *buf = (تهی *) data;

			أكد(data && "data can only be NULL for large _blobs_");

			/*
			 * we do not need to free the memory here, as the
			 * buf is deleted by the caller.
			 */
			obj = parse_object_buffer(the_repository, oid, type,
						  size, buf,
						  &eaten);
			اگر (!obj)
				die(_("invalid %s"), type_name(type));
			اگر (do_fsck_object &&
			    fsck_object(obj, buf, size, &fsck_options))
				die(_("fsck error in packed object"));
			اگر (strict && fsck_walk(obj, پوچ, &fsck_options))
				die(_("Not all child objects of %s are reachable"), oid_to_hex(&obj->oid));
			اگر (record_outgoing_links)
				do_record_outgoing_links(obj);

			اگر (obj->type == OBJ_درخت) {
				ساختار درخت *item = (ساختار درخت *) obj;
				item->buffer = پوچ;
				obj->parsed = 0;
			}
			اگر (obj->type == OBJ_COMMIT) {
				ساختار commit *commit = (ساختار commit *) obj;
				اگر (detach_commit_buffer(commit, پوچ) != data)
					BUG("parse_object_buffer transmogrified our buffer");
			}
			obj->flags |= FLAG_CHECKED;
		}
		read_unlock();
	}

	آزاد(new_data);
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
ایستا تهی *get_base_data(ساختار base_data *c)
{
	اگر (!c->data) {
		ساختار object_entry *obj = c->obj;
		ساختار base_data **delta = پوچ;
		صحیح delta_nr = 0, delta_alloc = 0;

		وقتی (is_delta_type(c->obj->type) && !c->data) {
			ALLOC_GROW(delta, delta_nr + 1, delta_alloc);
			delta[delta_nr++] = c;
			c = c->base;
		}
		اگر (!delta_nr) {
			c->data = get_data_from_pack(obj);
			c->size = obj->size;
			base_cache_used += c->size;
			prune_base_data(c);
		}
		برای (; delta_nr > 0; delta_nr--) {
			تهی *base, *raw;
			c = delta[delta_nr - 1];
			obj = c->obj;
			base = get_base_data(c->base);
			raw = get_data_from_pack(obj);
			c->data = patch_delta(
				base, c->base->size,
				raw, obj->size,
				&c->size);
			آزاد(raw);
			اگر (!c->data)
				bad_object(obj->idx.offset, _("failed to apply delta"));
			base_cache_used += c->size;
			prune_base_data(c);
		}
		آزاد(delta);
	}
	بازگشت c->data;
}

ایستا ساختار base_data *make_base(ساختار object_entry *obj,
				   ساختار base_data *parent)
{
	ساختار base_data *base = xcalloc(1, اندازه(ساختار base_data));
	base->base = parent;
	base->obj = obj;
	find_ref_delta_children(&obj->idx.oid,
				&base->ref_first, &base->ref_last);
	find_ofs_delta_children(obj->idx.offset,
				&base->ofs_first, &base->ofs_last);
	base->children_remaining = base->ref_last - base->ref_first +
		base->ofs_last - base->ofs_first + 2;
	بازگشت base;
}

ایستا ساختار base_data *resolve_delta(ساختار object_entry *delta_obj,
				       ساختار base_data *base)
{
	تهی *delta_data, *result_data;
	ساختار base_data *result;
	size_t result_size;

	اگر (show_stat) {
		صحیح i = delta_obj - objects;
		صحیح j = base->obj - objects;
		obj_stat[i].delta_depth = obj_stat[j].delta_depth + 1;
		deepest_delta_lock();
		اگر (deepest_delta < obj_stat[i].delta_depth)
			deepest_delta = obj_stat[i].delta_depth;
		deepest_delta_unlock();
		obj_stat[i].base_object_no = j;
	}
	delta_data = get_data_from_pack(delta_obj);
	أكد(base->data);
	result_data = patch_delta(base->data, base->size,
				  delta_data, delta_obj->size, &result_size);
	آزاد(delta_data);
	اگر (!result_data)
		bad_object(delta_obj->idx.offset, _("failed to apply delta"));
	hash_object_file(the_hash_algo, result_data, result_size,
			 delta_obj->real_type, &delta_obj->idx.oid);
	sha1_object(result_data, پوچ, result_size, delta_obj->real_type,
		    &delta_obj->idx.oid);

	result = make_base(delta_obj, base);
	result->data = result_data;
	result->size = result_size;

	counter_lock();
	nr_resolved_deltas++;
	counter_unlock();

	بازگشت result;
}

ایستا صحیح compare_ofs_delta_entry(ثابت تهی *a, ثابت تهی *b)
{
	ثابت ساختار ofs_delta_entry *delta_a = a;
	ثابت ساختار ofs_delta_entry *delta_b = b;

	بازگشت delta_a->offset < delta_b->offset ? -1 :
	       delta_a->offset > delta_b->offset ?  1 :
	       0;
}

ایستا صحیح compare_ref_delta_entry(ثابت تهی *a, ثابت تهی *b)
{
	ثابت ساختار ref_delta_entry *delta_a = a;
	ثابت ساختار ref_delta_entry *delta_b = b;

	بازگشت oidcmp(&delta_a->oid, &delta_b->oid);
}

ایستا تهی *threaded_second_pass(تهی *data)
{
	اگر (data)
		set_thread_data(data);
	برای (;;) {
		ساختار base_data *parent = پوچ;
		ساختار object_entry *child_obj = پوچ;
		ساختار base_data *child = پوچ;

		counter_lock();
		display_progress(progress, nr_resolved_deltas);
		counter_unlock();

		work_lock();
		اگر (list_empty(&work_head)) {
			/*
			 * Take an object from the object array.
			 */
			وقتی (nr_dispatched < nr_objects &&
			       is_delta_type(objects[nr_dispatched].type))
				nr_dispatched++;
			اگر (nr_dispatched >= nr_objects) {
				work_unlock();
				بشکن;
			}
			child_obj = &objects[nr_dispatched++];
		} وگرنه {
			/*
			 * Peek at the top of the stack, and take a child from
			 * it.
			 */
			parent = list_first_entry(&work_head, ساختار base_data,
						  list);

			وقتی (parent->ref_first <= parent->ref_last) {
				صحیح offset = ref_deltas[parent->ref_first++].obj_no;
				child_obj = objects + offset;
				اگر (child_obj->real_type != OBJ_REF_DELTA) {
					child_obj = پوچ;
					ادامه;
				}
				child_obj->real_type = parent->obj->real_type;
				بشکن;
			}

			اگر (!child_obj && parent->ofs_first <= parent->ofs_last) {
				child_obj = objects +
					ofs_deltas[parent->ofs_first++].obj_no;
				أكد(child_obj->real_type == OBJ_OFS_DELTA);
				child_obj->real_type = parent->obj->real_type;
			}

			اگر (parent->ref_first > parent->ref_last &&
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

		اگر (child_obj) {
			اگر (parent) {
				child = resolve_delta(child_obj, parent);
				اگر (!child->children_remaining)
					FREE_AND_NULL(child->data);
			} وگرنه{
				child = make_base(child_obj, پوچ);
				اگر (child->children_remaining) {
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
		اگر (parent)
			parent->retain_data--;

		اگر (child && child->data) {
			/*
			 * This child has its own children, so add it to
			 * work_head.
			 */
			list_add(&child->list, &work_head);
			base_cache_used += child->size;
			prune_base_data(پوچ);
		} وگرنه اگر (child) {
			/*
			 * This child does not have its own children. It may be
			 * the last descendant of its ancestors; free those
			 * that we can.
			 */
			ساختار base_data *p = parent;

			وقتی (p) {
				ساختار base_data *next_p;

				p->children_remaining--;
				اگر (p->children_remaining)
					بشکن;

				next_p = p->base;
				free_base_data(p);
				list_del(&p->list);
				آزاد(p);

				p = next_p;
			}
			FREE_AND_NULL(child);
		}
		work_unlock();
	}
	بازگشت پوچ;
}

/*
 * First pass:
 * - find locations of all objects;
 * - calculate SHA1 of all non-delta objects;
 * - remember base (SHA1 or offset) for all deltas.
 */
ایستا تهی parse_pack_objects(unsigned نویسه *hash)
{
	صحیح i, nr_delays = 0;
	ساختار ofs_delta_entry *ofs_delta = ofs_deltas;
	ساختار object_id ref_delta_oid;
	ساختار stat st;
	ساختار git_hash_ctx tmp_ctx;

	اگر (verbose)
		progress = start_progress(
				the_repository,
				progress_title ? progress_title :
				from_stdin ? _("Receiving objects") : _("Indexing objects"),
				nr_objects);
	برای (i = 0; i < nr_objects; i++) {
		ساختار object_entry *obj = &objects[i];
		تهی *data = unpack_raw_entry(obj, &ofs_delta->offset,
					      &ref_delta_oid,
					      &obj->idx.oid);
		obj->real_type = obj->type;
		اگر (obj->type == OBJ_OFS_DELTA) {
			nr_ofs_deltas++;
			ofs_delta->obj_no = i;
			ofs_delta++;
		} وگرنه اگر (obj->type == OBJ_REF_DELTA) {
			ALLOC_GROW(ref_deltas, nr_ref_deltas + 1, ref_deltas_alloc);
			oidcpy(&ref_deltas[nr_ref_deltas].oid, &ref_delta_oid);
			ref_deltas[nr_ref_deltas].obj_no = i;
			nr_ref_deltas++;
		} وگرنه اگر (!data) {
			/* large blobs, check later */
			obj->real_type = OBJ_BAD;
			nr_delays++;
		} وگرنه
			sha1_object(data, پوچ, obj->size, obj->type,
				    &obj->idx.oid);
		آزاد(data);
		display_progress(progress, i+1);
	}
	objects[i].idx.offset = consumed_bytes;
	stop_progress(&progress);

	/* Check pack integrity */
	flush();
	git_hash_init(&tmp_ctx, the_hash_algo);
	git_hash_clone(&tmp_ctx, &درون_داد_ctx);
	git_hash_final(hash, &tmp_ctx);
	اگر (!hasheq(fill(the_hash_algo->rawsz), hash, the_repository->hash_algo))
		die(_("pack is corrupted (SHA1 mismatch)"));
	use(the_hash_algo->rawsz);

	/* If input_fd is a file, we should have reached its end now. */
	اگر (fstat(درون_داد_fd, &st))
		die_errno(_("cannot fstat packfile"));
	اگر (S_ISREG(st.st_mode) &&
			lseek(درون_داد_fd, 0, SEEK_CUR) - درون_داد_len != st.st_size)
		die(_("pack has junk at the end"));

	برای (i = 0; i < nr_objects; i++) {
		ساختار object_entry *obj = &objects[i];
		اگر (obj->real_type != OBJ_BAD)
			ادامه;
		obj->real_type = obj->type;
		sha1_object(پوچ, obj, obj->size, obj->type,
			    &obj->idx.oid);
		nr_delays--;
	}
	اگر (nr_delays)
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
ایستا تهی resolve_deltas(ساختار pack_idx_option *opts)
{
	صحیح i;

	اگر (!nr_ofs_deltas && !nr_ref_deltas)
		بازگشت;

	/* Sort deltas by base SHA1/offset for fast searching */
	QSORT(ofs_deltas, nr_ofs_deltas, compare_ofs_delta_entry);
	QSORT(ref_deltas, nr_ref_deltas, compare_ref_delta_entry);

	اگر (verbose || show_resolving_progress)
		progress = start_progress(the_repository,
					  _("Resolving deltas"),
					  nr_ref_deltas + nr_ofs_deltas);

	nr_dispatched = 0;
	base_cache_limit = opts->delta_base_cache_limit * nr_threads;
	اگر (nr_threads > 1 || getenv("GIT_FORCE_THREADS")) {
		init_thread();
		برای (i = 0; i < nr_threads; i++) {
			صحیح ret = pthread_create(&thread_data[i].thread, پوچ,
						 threaded_second_pass, thread_data + i);
			اگر (ret)
				die(_("unable to create thread: %s"),
				    strerror(ret));
		}
		برای (i = 0; i < nr_threads; i++)
			pthread_join(thread_data[i].thread, پوچ);
		cleanup_thread();
		بازگشت;
	}
	threaded_second_pass(&nothread_data);
}

/*
 * Third pass:
 * - append objects to convert thin pack to full pack if required
 * - write the final pack hash
 */
ایستا تهی fix_unresolved_deltas(ساختار hashfile *f);
ایستا تهی conclude_pack(صحیح fix_thin_pack, ثابت نویسه *curr_pack, unsigned نویسه *pack_hash)
{
	اگر (nr_ref_deltas + nr_ofs_deltas == nr_resolved_deltas) {
		stop_progress(&progress);
		/* Flush remaining pack final hash. */
		flush();
		بازگشت;
	}

	اگر (fix_thin_pack) {
		ساختار hashfile *f;
		unsigned نویسه read_hash[GIT_MAX_RAWSZ], tail_hash[GIT_MAX_RAWSZ];
		ساختار strbuf پیام = STRBUF_INIT;
		صحیح nr_unresolved = nr_ofs_deltas + nr_ref_deltas - nr_resolved_deltas;
		صحیح nr_objects_initial = nr_objects;
		اگر (nr_unresolved <= 0)
			die(_("confusion beyond insanity"));
		REALLOC_ARRAY(objects, nr_objects + nr_unresolved + 1);
		memset(objects + nr_objects + 1, 0,
		       nr_unresolved * اندازه(*objects));
		f = hashfd(the_repository->hash_algo, output_fd, curr_pack);
		fix_unresolved_deltas(f);
		strbuf_addf(&پیام, Q_("completed with %d local object",
				     "completed with %d local objects",
				     nr_objects - nr_objects_initial),
			    nr_objects - nr_objects_initial);
		stop_progress_پیام(&progress, پیام.buf);
		strbuf_release(&پیام);
		finalize_hashfile(f, tail_hash, FSYNC_COMPONENT_PACK, 0);
		hashcpy(read_hash, pack_hash, the_repository->hash_algo);
		fixup_pack_header_footer(the_hash_algo, output_fd, pack_hash,
					 curr_pack, nr_objects,
					 read_hash, consumed_bytes-the_hash_algo->rawsz);
		اگر (!hasheq(read_hash, tail_hash, the_repository->hash_algo))
			die(_("Unexpected tail checksum for %s "
			      "(disk corruption?)"), curr_pack);
	}
	اگر (nr_ofs_deltas + nr_ref_deltas != nr_resolved_deltas)
		die(Q_("pack has %d unresolved delta",
		       "pack has %d unresolved deltas",
		       nr_ofs_deltas + nr_ref_deltas - nr_resolved_deltas),
		    nr_ofs_deltas + nr_ref_deltas - nr_resolved_deltas);
}

ایستا صحیح write_compressed(ساختار hashfile *f, تهی *in, unsigned صحیح size)
{
	git_zstream stream;
	صحیح status;
	unsigned نویسه outbuf[4096];
	ساختار repo_config_مقادیر *cfg = repo_config_مقادیر(the_repository);

	git_deflate_init(&stream, cfg->zlib_compression_level);
	stream.next_in = in;
	stream.avail_in = size;

	انجامبده {
		stream.next_out = outbuf;
		stream.avail_out = اندازه(outbuf);
		status = git_deflate(&stream, Z_FINISH);
		hashwrite(f, outbuf, اندازه(outbuf) - stream.avail_out);
	} وقتی (status == Z_OK);

	اگر (status != Z_STREAM_END)
		die(_("unable to deflate appended object (%d)"), status);
	size = stream.total_out;
	git_deflate_end(&stream);
	بازگشت size;
}

ایستا ساختار object_entry *append_obj_to_pack(ساختار hashfile *f,
			       ثابت unsigned نویسه *sha1, تهی *buf,
			       unsigned long size, شمارش object_type type)
{
	ساختار object_entry *obj = &objects[nr_objects++];
	unsigned نویسه header[10];
	unsigned long s = size;
	صحیح n = 0;
	unsigned نویسه c = (type << 4) | (s & 15);
	s >>= 4;
	وقتی (s) {
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
	بازگشت obj;
}

ایستا صحیح delta_pos_compare(ثابت تهی *_a, ثابت تهی *_b)
{
	ساختار ref_delta_entry *a = *(ساختار ref_delta_entry **)_a;
	ساختار ref_delta_entry *b = *(ساختار ref_delta_entry **)_b;
	بازگشت a->obj_no - b->obj_no;
}

ایستا تهی fix_unresolved_deltas(ساختار hashfile *f)
{
	ساختار ref_delta_entry **sorted_by_pos;
	صحیح i;

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
	برای (i = 0; i < nr_ref_deltas; i++)
		sorted_by_pos[i] = &ref_deltas[i];
	QSORT(sorted_by_pos, nr_ref_deltas, delta_pos_compare);

	اگر (repo_has_promisor_remote(the_repository)) {
		/*
		 * Prefetch the delta bases.
		 */
		ساختار oid_array to_fetch = OID_ARRAY_INIT;
		برای (i = 0; i < nr_ref_deltas; i++) {
			ساختار ref_delta_entry *d = sorted_by_pos[i];
			اگر (!odb_read_object_info_extended(the_repository->objects,
							   &d->oid, پوچ,
							   OBJECT_INFO_FOR_PREFETCH))
				ادامه;
			oid_array_append(&to_fetch, &d->oid);
		}
		promisor_remote_get_direct(the_repository,
					   to_fetch.oid, to_fetch.nr);
		oid_array_clear(&to_fetch);
	}

	برای (i = 0; i < nr_ref_deltas; i++) {
		ساختار ref_delta_entry *d = sorted_by_pos[i];
		شمارش object_type type;
		تهی *data;
		size_t size;

		اگر (objects[d->obj_no].real_type != OBJ_REF_DELTA)
			ادامه;
		data = odb_read_object(the_repository->objects, &d->oid,
				       &type, &size);
		اگر (!data)
			ادامه;

		اگر (check_object_signature(the_repository, &d->oid, data, size,
					   type) < 0)
			die(_("local object %s is corrupt"), oid_to_hex(&d->oid));

		/*
		 * Add this as an object to the objects array and call
		 * threaded_second_pass() (which will pick up the added
		 * object).
		 */
		append_obj_to_pack(f, d->oid.hash, data, size, type);
		آزاد(data);
		threaded_second_pass(پوچ);

		display_progress(progress, nr_resolved_deltas);
	}
	آزاد(sorted_by_pos);
}

ایستا ثابت نویسه *derive_filename(ثابت نویسه *pack_name, ثابت نویسه *strip,
				   ثابت نویسه *suffix, ساختار strbuf *buf)
{
	size_t len;
	اگر (!strip_suffix(pack_name, strip, &len) || !len ||
	    pack_name[len - 1] != '.')
		die(_("packfile name '%s' does not end with '.%s'"),
		    pack_name, strip);
	strbuf_add(buf, pack_name, len);
	strbuf_addstr(buf, suffix);
	بازگشت buf->buf;
}

ایستا تهی write_special_file(ثابت نویسه *suffix, ثابت نویسه *پیام,
			       ثابت نویسه *pack_name, ثابت unsigned نویسه *hash,
			       ثابت نویسه **report)
{
	ساختار strbuf name_buf = STRBUF_INIT;
	ثابت نویسه *filename;
	صحیح fd;
	صحیح پیام_len = strlen(پیام);

	اگر (pack_name)
		filename = derive_filename(pack_name, "pack", suffix, &name_buf);
	وگرنه
		filename = odb_pack_name(the_repository, &name_buf, hash, suffix);

	fd = safe_create_file_with_leading_directories(the_repository, filename);
	اگر (fd < 0) {
		اگر (errno != EEXIST)
			die_errno(_("cannot write %s file '%s'"),
				  suffix, filename);
	} وگرنه {
		اگر (پیام_len > 0) {
			write_or_die(fd, پیام, پیام_len);
			write_or_die(fd, "\n", 1);
		}
		اگر (close(fd) != 0)
			die_errno(_("cannot close written %s file '%s'"),
				  suffix, filename);
		اگر (report)
			*report = suffix;
	}
	strbuf_release(&name_buf);
}

ایستا تهی rename_tmp_packfile(ثابت نویسه **final_name,
				ثابت نویسه *curr_name,
				ساختار strbuf *name, unsigned نویسه *hash,
				ثابت نویسه *ext, صحیح make_read_only_if_same)
{
	اگر (!*final_name || strcmp(*final_name, curr_name)) {
		اگر (!*final_name)
			*final_name = odb_pack_name(the_repository, name, hash, ext);
		اگر (finalize_object_file(the_repository, curr_name, *final_name))
			die(_("unable to rename temporary '*.%s' file to '%s'"),
			    ext, *final_name);
	} وگرنه اگر (make_read_only_if_same) {
		chmod(*final_name, 0444);
	}
}

ایستا تهی final(ثابت نویسه *final_pack_name, ثابت نویسه *curr_pack_name,
		  ثابت نویسه *final_index_name, ثابت نویسه *curr_index_name,
		  ثابت نویسه *final_rev_index_name, ثابت نویسه *curr_rev_index_name,
		  ثابت نویسه *keep_پیام, ثابت نویسه *promisor_پیام,
		  unsigned نویسه *hash)
{
	ثابت نویسه *report = "pack";
	ساختار strbuf pack_name = STRBUF_INIT;
	ساختار strbuf index_name = STRBUF_INIT;
	ساختار strbuf rev_index_name = STRBUF_INIT;

	اگر (!from_stdin) {
		close(درون_داد_fd);
	} وگرنه {
		fsync_component_or_die(FSYNC_COMPONENT_PACK, output_fd, curr_pack_name);
		اگر (close(output_fd))
			die_errno(_("error while closing pack file"));
	}

	اگر (keep_پیام)
		write_special_file("keep", keep_پیام, final_pack_name, hash,
				   &report);
	اگر (promisor_پیام)
		write_special_file("promisor", promisor_پیام, final_pack_name,
				   hash, پوچ);

	rename_tmp_packfile(&final_pack_name, curr_pack_name, &pack_name,
			    hash, "pack", from_stdin);
	اگر (curr_rev_index_name)
		rename_tmp_packfile(&final_rev_index_name, curr_rev_index_name,
				    &rev_index_name, hash, "rev", 1);
	rename_tmp_packfile(&final_index_name, curr_index_name, &index_name,
			    hash, "idx", 1);

	اگر (do_fsck_object && startup_info->have_repository) {
		ساختار odb_source_files *files =
			odb_source_files_downcast(the_repository->objects->source);
		packfile_store_load_pack(files->dirs->packed, final_index_name, 0);
	}

	اگر (!from_stdin) {
		printf("%s\n", hash_to_hex(hash));
	} وگرنه {
		ساختار strbuf buf = STRBUF_INIT;

		strbuf_addf(&buf, "%s\t%s\n", report, hash_to_hex(hash));
		write_or_die(1, buf.buf, buf.len);
		strbuf_release(&buf);

		/* Write the last part of the buffer to stdout */
		write_in_full(1, درون_داد_buffer + درون_داد_offset, درون_داد_len);
	}

	strbuf_release(&rev_index_name);
	strbuf_release(&index_name);
	strbuf_release(&pack_name);
}

ایستا صحیح git_index_pack_config(ثابت نویسه *k, ثابت نویسه *v,
				 ثابت ساختار config_context *ctx, تهی *cb)
{
	ساختار pack_idx_option *opts = cb;

	اگر (!strcmp(k, "pack.indexversion")) {
		opts->version = git_config_int(k, v, ctx->kvi);
		اگر (opts->version > 2)
			die(_("bad pack.indexVersion=%"PRIu32), opts->version);
		بازگشت 0;
	}
	اگر (!strcmp(k, "pack.threads")) {
		nr_threads = git_config_int(k, v, ctx->kvi);
		اگر (nr_threads < 0)
			die(_("invalid number of threads specified (%d)"),
			    nr_threads);
		اگر (!HAVE_THREADS && nr_threads != 1) {
			warning(_("no threads support, ignoring %s"), k);
			nr_threads = 1;
		}
		بازگشت 0;
	}
	اگر (!strcmp(k, "pack.writereverseindex")) {
		اگر (git_config_bool(k, v))
			opts->flags |= WRITE_REV;
		وگرنه
			opts->flags &= ~WRITE_REV;
	}
	اگر (!strcmp(k, "core.deltabasecachelimit")) {
		opts->delta_base_cache_limit = git_config_ulong(k, v, ctx->kvi);
		بازگشت 0;
	}
	بازگشت git_default_config(k, v, ctx, cb);
}

ایستا صحیح cmp_uint32(ثابت تهی *a_, ثابت تهی *b_)
{
	uint32_t a = *((uint32_t *)a_);
	uint32_t b = *((uint32_t *)b_);

	بازگشت (a < b) ? -1 : (a != b);
}

ایستا تهی read_v2_anomalous_offsets(ساختار packed_git *p,
				      ساختار pack_idx_option *opts)
{
	ثابت uint32_t *idx1, *idx2;
	uint32_t i;

	/* The address of the 4-byte offset table */
	idx1 = (((ثابت uint32_t *)((ثابت uint8_t *)p->index_data + p->crc_offset))
		+ (size_t)p->رقم_objects /* CRC32 table */
		);

	/* The address of the 8-byte offset table */
	idx2 = idx1 + p->رقم_objects;

	برای (i = 0; i < p->رقم_objects; i++) {
		uint32_t off = ntohl(idx1[i]);
		اگر (!(off & 0x80000000))
			ادامه;
		off = off & 0x7fffffff;
		check_pack_index_ptr(p, &idx2[off * 2]);
		اگر (idx2[off * 2])
			ادامه;
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

ایستا تهی read_idx_option(ساختار pack_idx_option *opts, ثابت نویسه *pack_name)
{
	ساختار packed_git *p = add_packed_git(the_repository, pack_name,
					      strlen(pack_name), 1);

	اگر (!p)
		die(_("Cannot open existing pack file '%s'"), pack_name);
	اگر (open_pack_index(p))
		die(_("Cannot open existing pack idx file for '%s'"), pack_name);

	/* Read the attributes from the existing idx file */
	opts->version = p->index_version;

	اگر (opts->version == 2)
		read_v2_anomalous_offsets(p, opts);

	/*
	 * Get rid of the idx file as we do not need it anymore.
	 * NEEDSWORK: extract this bit from free_pack_by_name() in
	 * object-file.c, perhaps?  It shouldn't matter very much as we
	 * know we haven't installed this pack (hence we never have
	 * read anything from it).
	 */
	close_pack_index(p);
	آزاد(p);
}

ایستا تهی show_pack_info(صحیح stat_only)
{
	صحیح i, baseobjects = nr_objects - nr_ref_deltas - nr_ofs_deltas;
	unsigned long *chain_histogram = پوچ;

	اگر (deepest_delta)
		CALLOC_ARRAY(chain_histogram, deepest_delta);

	برای (i = 0; i < nr_objects; i++) {
		ساختار object_entry *obj = &objects[i];

		اگر (is_delta_type(obj->type))
			chain_histogram[obj_stat[i].delta_depth - 1]++;
		اگر (stat_only)
			ادامه;
		printf("%s %-6s %"PRIuMAX" %"PRIuMAX" %"PRIuMAX,
		       oid_to_hex(&obj->idx.oid),
		       type_name(obj->real_type), (uintmax_t)obj->size,
		       (uintmax_t)(obj[1].idx.offset - obj->idx.offset),
		       (uintmax_t)obj->idx.offset);
		اگر (is_delta_type(obj->type)) {
			ساختار object_entry *bobj = &objects[obj_stat[i].base_object_no];
			printf(" %u %s", obj_stat[i].delta_depth,
			       oid_to_hex(&bobj->idx.oid));
		}
		putchar('\n');
	}

	اگر (baseobjects)
		printf_ln(Q_("non delta: %d object",
			     "non delta: %d objects",
			     baseobjects),
			  baseobjects);
	برای (i = 0; i < deepest_delta; i++) {
		اگر (!chain_histogram[i])
			ادامه;
		printf_ln(Q_("chain length = %d: %lu object",
			     "chain length = %d: %lu objects",
			     chain_histogram[i]),
			  i + 1,
			  chain_histogram[i]);
	}
	آزاد(chain_histogram);
}

ایستا تهی repack_local_links(تهی)
{
	ساختار child_process cmd = CHILD_PROCESS_INIT;
	FILE *out;
	ساختار strbuf سطر = STRBUF_INIT;
	ساختار oidset_iter iter;
	ساختار object_id *oid;
	نویسه *base_name = پوچ;

	اگر (!oidset_size(&outgoing_links))
		بازگشت;

	oidset_iter_init(&outgoing_links, &iter);
	وقتی ((oid = oidset_iter_next(&iter))) {
		ساختار odb_source_info source_info;
		ساختار object_info info = {
			.source_infop = &source_info,
		};

		اگر (odb_read_object_info_extended(the_repository->objects, oid, &info, 0))
			/* Missing; assume it is a promisor object */
			ادامه;
		اگر (source_info.source->type == ODB_SOURCE_PACKED &&
		    source_info.u.packed.pack->pack_promisor)
			ادامه;

		اگر (!cmd.args.nr) {
			base_name = mkpathdup(
				"%s/pack/pack",
				repo_get_object_directory(the_repository));
			strvec_افزودن(&cmd.args, "pack-objects");
			strvec_افزودن(&cmd.args,
				    "--exclude-promisor-objects-best-effort");
			strvec_افزودن(&cmd.args, base_name);
			cmd.git_cmd = 1;
			cmd.in = -1;
			cmd.out = -1;
			اگر (start_command(&cmd))
				die(_("could not start pack-objects to repack local links"));
		}

		اگر (write_in_full(cmd.in, oid_to_hex(oid), the_hash_algo->hexsz) < 0 ||
		    write_in_full(cmd.in, "\n", 1) < 0)
			die(_("failed to feed local object to pack-objects"));
	}

	اگر (!cmd.args.nr)
		بازگشت;

	close(cmd.in);

	out = xfdopen(cmd.out, "r");
	وقتی (strbuf_getline_lf(&سطر, out) != EOF) {
		unsigned نویسه دودویی[GIT_MAX_RAWSZ];
		اگر (سطر.len != the_hash_algo->hexsz ||
		    !hex_to_bytes(دودویی, سطر.buf, سطر.len))
			die(_("index-pack: Expecting full hex object ID lines only from pack-objects."));

		/*
		 * pack-objects creates the .pack and .idx files, but not the
		 * .promisor file. Create the .promisor file, which is empty.
		 */
		write_special_file("promisor", "", پوچ, دودویی, پوچ);
	}

	fclose(out);
	اگر (finish_command(&cmd))
		die(_("could not finish pack-objects to repack local links"));
	strbuf_release(&سطر);
	آزاد(base_name);
}

صحیح cmd_index_pack(صحیح argc,
		   ثابت نویسه **argv,
		   ثابت نویسه *prefix,
		   ساختار repository *repo)
{
	صحیح i, fix_thin_pack = 0, verify = 0, stat_only = 0, rev_index;
	ثابت نویسه *curr_index;
	نویسه *curr_rev_index = پوچ;
	ثابت نویسه *index_name = پوچ, *pack_name = پوچ, *rev_index_name = پوچ;
	ثابت نویسه *keep_پیام = پوچ;
	ثابت نویسه *promisor_پیام = پوچ;
	ساختار strbuf index_name_buf = STRBUF_INIT;
	ساختار strbuf rev_index_name_buf = STRBUF_INIT;
	ساختار pack_idx_entry **idx_objects;
	ساختار pack_idx_option opts;
	unsigned نویسه pack_hash[GIT_MAX_RAWSZ];
	unsigned foreign_nr = 1;	/* zero is a "good" value, assume bad */
	صحیح report_end_of_درون_داد = 0;
	صحیح hash_algo = 0;

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
	اگر (prefix && chdir(prefix))
		die(_("Cannot come back to cwd"));

	اگر (git_env_bool(GIT_TEST_NO_WRITE_REV_INDEX, 0))
		rev_index = 0;
	وگرنه
		rev_index = !!(opts.flags & (WRITE_REV_VERIFY | WRITE_REV));

	برای (i = 1; i < argc; i++) {
		ثابت نویسه *arg = argv[i];

		اگر (*arg == '-') {
			اگر (!strcmp(arg, "--stdin")) {
				from_stdin = 1;
			} وگرنه اگر (!strcmp(arg, "--fix-thin")) {
				fix_thin_pack = 1;
			} وگرنه اگر (skip_to_optional_arg(arg, "--strict", &arg)) {
				strict = 1;
				do_fsck_object = 1;
				fsck_set_پیام_types(&fsck_options, arg);
			} وگرنه اگر (!strcmp(arg, "--check-self-contained-and-connected")) {
				strict = 1;
				check_self_contained_and_connected = 1;
			} وگرنه اگر (skip_to_optional_arg(arg, "--fsck-objects", &arg)) {
				do_fsck_object = 1;
				fsck_set_پیام_types(&fsck_options, arg);
			} وگرنه اگر (!strcmp(arg, "--verify")) {
				verify = 1;
			} وگرنه اگر (!strcmp(arg, "--verify-stat")) {
				verify = 1;
				show_stat = 1;
			} وگرنه اگر (!strcmp(arg, "--verify-stat-only")) {
				verify = 1;
				show_stat = 1;
				stat_only = 1;
			} وگرنه اگر (skip_to_optional_arg(arg, "--keep", &keep_پیام)) {
				; /* nothing to do */
			} وگرنه اگر (skip_to_optional_arg(arg, "--promisor", &promisor_پیام)) {
				record_outgoing_links = 1;
			} وگرنه اگر (starts_with(arg, "--threads=")) {
				نویسه *end;
				nr_threads = strtoul(arg+10, &end, 0);
				اگر (!arg[10] || *end || nr_threads < 0)
					usage(index_pack_usage);
				اگر (!HAVE_THREADS && nr_threads != 1) {
					warning(_("no threads support, ignoring %s"), arg);
					nr_threads = 1;
				}
			} وگرنه اگر (skip_prefix(arg, "--pack_header=", &arg)) {
				اگر (parse_pack_header_option(arg,
							     درون_داد_buffer,
							     &درون_داد_len) < 0)
					die(_("bad --pack_header: %s"), arg);
			} وگرنه اگر (!strcmp(arg, "-v")) {
				verbose = 1;
			} وگرنه اگر (!strcmp(arg, "--progress-title")) {
				اگر (progress_title || (i+1) >= argc)
					usage(index_pack_usage);
				progress_title = argv[++i];
			} وگرنه اگر (!strcmp(arg, "--show-resolving-progress")) {
				show_resolving_progress = 1;
			} وگرنه اگر (!strcmp(arg, "--report-end-of-input")) {
				report_end_of_درون_داد = 1;
			} وگرنه اگر (!strcmp(arg, "-o")) {
				اگر (index_name || (i+1) >= argc)
					usage(index_pack_usage);
				index_name = argv[++i];
			} وگرنه اگر (starts_with(arg, "--index-version=")) {
				نویسه *c;
				opts.version = strtoul(arg + 16, &c, 10);
				اگر (opts.version > 2)
					die(_("bad %s"), arg);
				اگر (*c == ',')
					opts.off32_limit = strtoul(c+1, &c, 0);
				اگر (*c || opts.off32_limit & 0x80000000)
					die(_("bad %s"), arg);
			} وگرنه اگر (skip_prefix(arg, "--max-input-size=", &arg)) {
				max_درون_داد_size = strtoumax(arg, پوچ, 10);
			} وگرنه اگر (skip_prefix(arg, "--object-format=", &arg)) {
				hash_algo = hash_algo_by_name(arg);
				اگر (hash_algo == GIT_HASH_UNKNOWN)
					die(_("unknown hash algorithm '%s'"), arg);
				repo_set_hash_algo(the_repository, hash_algo);
			} وگرنه اگر (!strcmp(arg, "--rev-index")) {
				rev_index = 1;
			} وگرنه اگر (!strcmp(arg, "--no-rev-index")) {
				rev_index = 0;
			} وگرنه
				usage(index_pack_usage);
			ادامه;
		}

		اگر (pack_name)
			usage(index_pack_usage);
		pack_name = arg;
	}

	اگر (!pack_name && !from_stdin)
		usage(index_pack_usage);
	اگر (fix_thin_pack && !from_stdin)
		die(_("the option '%s' requires '%s'"), "--fix-thin", "--stdin");
	اگر (promisor_پیام && pack_name)
		die(_("--promisor cannot be used with a pack name"));
	اگر (from_stdin && !startup_info->have_repository)
		die(_("--stdin requires a git repository"));
	اگر (from_stdin && hash_algo)
		die(_("options '%s' and '%s' cannot be used together"), "--object-format", "--stdin");
	اگر (!index_name && pack_name)
		index_name = derive_filename(pack_name, "pack", "idx", &index_name_buf);

	/*
	 * Packfiles and indices do not carry enough information to be able to
	 * identify their object hash. So when we are neither in a repository
	 * nor has the user told us which object hash to use we have no other
	 * choice but to guess the object hash.
	 */
	اگر (!the_repository->hash_algo)
		repo_set_hash_algo(the_repository, GIT_HASH_DEFAULT);

	opts.flags &= ~(WRITE_REV | WRITE_REV_VERIFY);
	اگر (rev_index) {
		opts.flags |= verify ? WRITE_REV_VERIFY : WRITE_REV;
		اگر (index_name)
			rev_index_name = derive_filename(index_name,
							 "idx", "rev",
							 &rev_index_name_buf);
	}

	اگر (verify) {
		اگر (!index_name)
			die(_("--verify with no packfile name given"));
		read_idx_option(&opts, index_name);
		opts.flags |= WRITE_IDX_VERIFY | WRITE_IDX_STRICT;
	}
	اگر (strict)
		opts.flags |= WRITE_IDX_STRICT;

	اگر (HAVE_THREADS && !nr_threads) {
		nr_threads = online_cpus();
		/*
		 * Experiments show that going above 20 threads doesn't help,
		 * no matter how many cores you have. Below that, we tend to
		 * max at half the number of online_cpus(), presumably because
		 * half of those are hyperthreads rather than full cores. We'll
		 * never reduce the level below "3", though, to match a
		 * historical value that nobody complained about.
		 */
		اگر (nr_threads < 4)
			; /* too few cores to consider capping */
		وگرنه اگر (nr_threads < 6)
			nr_threads = 3; /* historic cap */
		وگرنه اگر (nr_threads < 40)
			nr_threads /= 2;
		وگرنه
			nr_threads = 20; /* hard cap */
	}

	curr_pack = open_pack_file(pack_name);
	parse_pack_header();
	CALLOC_ARRAY(objects, st_add(nr_objects, 1));
	اگر (show_stat)
		CALLOC_ARRAY(obj_stat, st_add(nr_objects, 1));
	CALLOC_ARRAY(ofs_deltas, nr_objects);
	parse_pack_objects(pack_hash);
	اگر (report_end_of_درون_داد)
		write_in_full(2, "\0", 1);
	resolve_deltas(&opts);
	conclude_pack(fix_thin_pack, curr_pack, pack_hash);
	آزاد(ofs_deltas);
	آزاد(ref_deltas);
	اگر (strict)
		foreign_nr = check_objects();

	اگر (show_stat)
		show_pack_info(stat_only);

	ALLOC_ARRAY(idx_objects, nr_objects);
	برای (i = 0; i < nr_objects; i++)
		idx_objects[i] = &objects[i].idx;
	curr_index = write_idx_file(the_repository, index_name, idx_objects,
				    nr_objects, &opts, pack_hash);
	اگر (rev_index)
		curr_rev_index = write_rev_file(the_repository, rev_index_name,
						idx_objects, nr_objects,
						pack_hash, opts.flags);
	آزاد(idx_objects);

	اگر (!verify)
		final(pack_name, curr_pack,
		      index_name, curr_index,
		      rev_index_name, curr_rev_index,
		      keep_پیام, promisor_پیام,
		      pack_hash);
	وگرنه
		close(درون_داد_fd);

	اگر (do_fsck_object) {
		/*
		 * We cannot perform queued consistency checks when running
		 * outside of a repository because those require us to read
		 * from the object database, which is uninitialized.
		 *
		 * TODO: we may eventually set up an in-memory object database,
		 * which would allow us to perform these queued checks.
		 */
		اگر (!startup_info->have_repository &&
		    fsck_has_queued_checks(&fsck_options))
			die(_("cannot perform queued object checks outside "
			      "of a repository"));

		اگر (fsck_finish(&fsck_options))
			die(_("fsck error in pack objects"));
	}

	آزاد(opts.anomaly);
	آزاد(objects);
	strbuf_release(&index_name_buf);
	strbuf_release(&rev_index_name_buf);
	اگر (!pack_name)
		آزاد((تهی *) curr_pack);
	اگر (!index_name)
		آزاد((تهی *) curr_index);
	آزاد(curr_rev_index);

	repack_local_links();

	/*
	 * Let the caller know this pack is not self contained
	 */
	اگر (check_self_contained_and_connected && foreign_nr)
		بازگشت 1;

	بازگشت 0;
}
