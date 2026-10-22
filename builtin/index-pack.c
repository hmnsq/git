#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#ضمن "builtin.h"
#ضمن "config.h"
#ضمن "delta.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "hex.h"
#ضمن "pack.h"
#ضمن "csum-file.h"
#ضمن "blob.h"
#ضمن "commit.h"
#ضمن "tag.h"
#ضمن "tree.h"
#ضمن "progress.h"
#ضمن "fsck.h"
#ضمن "strbuf.h"
#ضمن "thread-utils.h"
#ضمن "packfile.h"
#ضمن "pack-revindex.h"
#ضمن "object-file.h"
#ضمن "odb.h"
#ضمن "odb/streaming.h"
#ضمن "oid-array.h"
#ضمن "oidset.h"
#ضمن "path.h"
#ضمن "replace-object.h"
#ضمن "tree-walk.h"
#ضمن "promisor-remote.h"
#ضمن "run-command.h"
#ضمن "setup.h"
#ضمن "strvec.h"

ساكن ثابت حرف index_pack_usage[] =
"git index-pack [-v] [-o <index-file>] [--keep | --keep=<msg>] [--[no-]rev-index] [--verify] [--strict[=<msg-id>=<severity>...]] [--fsck-objects[=<msg-id>=<severity>...]] (<pack-file> | --stdin [--fix-thin] [<pack-file>])";

بنية object_entry {
	بنية pack_idx_entry idx;
	size_t size;
	unsigned حرف hdr_size;
	signed حرف type;
	signed حرف real_type;
};

بنية object_stat {
	unsigned delta_depth;
	صحيح base_object_no;
};

بنية base_data {
	/* Initialized by make_base(). */
	بنية base_data *base;
	بنية object_entry *obj;
	صحيح ref_first, ref_last;
	صحيح ofs_first, ofs_last;
	/*
	 * Threads should increment retain_data if they are about to call
	 * patch_delta() using this struct's data as a base, and decrement this
	 * when they are done. While retain_data is nonzero, this struct's data
	 * will not be freed even if the delta base cache limit is exceeded.
	 */
	صحيح retain_data;
	/*
	 * The number of direct children that have not been fully processed
	 * (entered work_head, entered done_head, left done_head). When this
	 * number reaches zero, this struct base_data can be freed.
	 */
	صحيح children_remaining;

	/* Not initialized by make_base(). */
	بنية list_head list;
	فراغ *data;
	size_t size;
};

/*
 * Stack of struct base_data that have unprocessed children.
 * threaded_second_pass() uses this as a source of work (the other being the
 * objects array).
 *
 * Guarded by work_mutex.
 */
ساكن LIST_HEAD(work_head);

/*
 * Stack of struct base_data that have children, all of whom have been
 * processed or are being processed, and at least one child is being processed.
 * These struct base_data must be kept around until the last child is
 * processed.
 *
 * Guarded by work_mutex.
 */
ساكن LIST_HEAD(done_head);

/*
 * All threads share one delta base cache.
 *
 * base_cache_used is guarded by work_mutex, and base_cache_limit is read-only
 * in a thread.
 */
ساكن size_t base_cache_used;
ساكن size_t base_cache_limit;

بنية thread_local_data {
	pthread_t thread;
	صحيح pack_fd;
};

/* Remember to update object flag allocation in object.h */
#define FLAG_LINK (1u<<20)
#define FLAG_CHECKED (1u<<21)

بنية ofs_delta_entry {
	off_t offset;
	صحيح obj_no;
};

بنية ref_delta_entry {
	بنية object_id oid;
	صحيح obj_no;
};

ساكن بنية object_entry *objects;
ساكن بنية object_stat *obj_stat;
ساكن بنية ofs_delta_entry *ofs_deltas;
ساكن بنية ref_delta_entry *ref_deltas;
ساكن بنية thread_local_data nothread_data;
ساكن صحيح nr_objects;
ساكن صحيح nr_ofs_deltas;
ساكن صحيح nr_ref_deltas;
ساكن صحيح ref_deltas_alloc;
ساكن صحيح nr_resolved_deltas;
ساكن صحيح nr_threads;

ساكن صحيح from_stdin;
ساكن صحيح strict;
ساكن صحيح do_fsck_object;
ساكن بنية fsck_options fsck_options;
ساكن صحيح verbose;
ساكن ثابت حرف *progress_title;
ساكن صحيح show_resolving_progress;
ساكن صحيح show_stat;
ساكن صحيح check_self_contained_and_connected;

ساكن بنية progress *progress;

ساكن unsigned حرف input_buffer[DEFAULT_IO_BUFFER_SIZE];
ساكن unsigned صحيح input_offset, input_len;
ساكن off_t consumed_bytes;
ساكن off_t max_input_size;
ساكن unsigned deepest_delta;
ساكن بنية git_hash_ctx input_ctx;
ساكن uint32_t input_crc32;
ساكن صحيح input_fd, output_fd;
ساكن ثابت حرف *curr_pack;

/*
 * outgoing_links is guarded by read_mutex, and record_outgoing_links is
 * read-only in a thread.
 */
ساكن بنية oidset outgoing_links = OIDSET_INIT;
ساكن صحيح record_outgoing_links;

ساكن بنية thread_local_data *thread_data;
ساكن صحيح nr_dispatched;
ساكن صحيح threads_active;

ساكن pthread_mutex_t read_mutex;
#define read_lock()		lock_mutex(&read_mutex)
#define read_unlock()		unlock_mutex(&read_mutex)

ساكن pthread_mutex_t counter_mutex;
#define counter_lock()		lock_mutex(&counter_mutex)
#define counter_unlock()	unlock_mutex(&counter_mutex)

ساكن pthread_mutex_t work_mutex;
#define work_lock()		lock_mutex(&work_mutex)
#define work_unlock()		unlock_mutex(&work_mutex)

ساكن pthread_mutex_t deepest_delta_mutex;
#define deepest_delta_lock()	lock_mutex(&deepest_delta_mutex)
#define deepest_delta_unlock()	unlock_mutex(&deepest_delta_mutex)

ساكن pthread_key_t key;

ساكن مضمن فراغ lock_mutex(pthread_mutex_t *mutex)
{
	ܐܢ (threads_active)
		pthread_mutex_lock(mutex);
}

ساكن مضمن فراغ unlock_mutex(pthread_mutex_t *mutex)
{
	ܐܢ (threads_active)
		pthread_mutex_unlock(mutex);
}

/*
 * Mutex and conditional variable can't be statically-initialized on Windows.
 */
ساكن فراغ init_thread(فراغ)
{
	صحيح i;
	init_recursive_mutex(&read_mutex);
	pthread_mutex_init(&counter_mutex, عدم);
	pthread_mutex_init(&work_mutex, عدم);
	ܐܢ (show_stat)
		pthread_mutex_init(&deepest_delta_mutex, عدم);
	pthread_key_create(&key, عدم);
	CALLOC_ARRAY(thread_data, nr_threads);
	لكل (i = 0; i < nr_threads; i++) {
		thread_data[i].pack_fd = xopen(curr_pack, O_RDONLY);
	}

	threads_active = 1;
}

ساكن فراغ cleanup_thread(فراغ)
{
	صحيح i;
	ܐܢ (!threads_active)
		ܦܢܐ;
	threads_active = 0;
	pthread_mutex_destroy(&read_mutex);
	pthread_mutex_destroy(&counter_mutex);
	pthread_mutex_destroy(&work_mutex);
	ܐܢ (show_stat)
		pthread_mutex_destroy(&deepest_delta_mutex);
	لكل (i = 0; i < nr_threads; i++)
		close(thread_data[i].pack_fd);
	pthread_key_delete(key);
	حرر(thread_data);
}

ساكن صحيح mark_link(بنية object *obj, تعداد object_type type,
		     فراغ *data UNUSED,
		     بنية fsck_options *options UNUSED)
{
	ܐܢ (!obj)
		ܦܢܐ -1;

	ܐܢ (type != OBJ_ANY && obj->type != type)
		die(_("object type mismatch at %s"), oid_to_hex(&obj->oid));

	obj->flags |= FLAG_LINK;
	ܦܢܐ 0;
}

/* The content of each linked object must have been checked
   or it must be already present in the object database */
ساكن unsigned check_object(بنية object *obj)
{
	ܐܢ (!obj)
		ܦܢܐ 0;

	ܐܢ (!(obj->flags & FLAG_LINK))
		ܦܢܐ 0;

	ܐܢ (!(obj->flags & FLAG_CHECKED)) {
		size_t size;
		صحيح type = odb_read_object_info(the_repository->objects,
						&obj->oid, &size);
		ܐܢ (type <= 0)
			die(_("did not receive expected object %s"),
			      oid_to_hex(&obj->oid));
		ܐܢ (type != obj->type)
			die(_("object %s: expected type %s, found %s"),
			    oid_to_hex(&obj->oid),
			    type_name(obj->type), type_name(type));
		obj->flags |= FLAG_CHECKED;
		ܦܢܐ 1;
	}

	ܦܢܐ 0;
}

ساكن unsigned check_objects(فراغ)
{
	unsigned i, max, foreign_nr = 0;

	max = get_max_object_index(the_repository);

	ܐܢ (verbose)
		progress = start_delayed_progress(the_repository,
						  _("Checking objects"), max);

	لكل (i = 0; i < max; i++) {
		foreign_nr += check_object(get_indexed_object(the_repository, i));
		display_progress(progress, i + 1);
	}

	stop_progress(&progress);
	ܦܢܐ foreign_nr;
}


/* Discard current buffer used content. */
ساكن فراغ flush(فراغ)
{
	ܐܢ (input_offset) {
		ܐܢ (output_fd >= 0)
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
ساكن فراغ *fill(صحيح min)
{
	ܐܢ (min <= input_len)
		ܦܢܐ input_buffer + input_offset;
	ܐܢ (min > حجم(input_buffer))
		die(Q_("cannot fill %d byte",
		       "cannot fill %d bytes",
		       min),
		    min);
	flush();
	ܥܒܕ {
		ssize_t ret = xread(input_fd, input_buffer + input_len,
				حجم(input_buffer) - input_len);
		ܐܢ (ret <= 0) {
			ܐܢ (!ret)
				die(_("early EOF"));
			die_errno(_("read error on input"));
		}
		input_len += ret;
		ܐܢ (from_stdin)
			display_throughput(progress, consumed_bytes + input_len);
	} ܟܡܐ (input_len < min);
	ܦܢܐ input_buffer;
}

ساكن فراغ use(صحيح bytes)
{
	ܐܢ (bytes > input_len)
		die(_("used more bytes than were available"));
	input_crc32 = crc32(input_crc32, input_buffer + input_offset, bytes);
	input_len -= bytes;
	input_offset += bytes;

	/* make sure off_t is sufficiently large not to wrap */
	ܐܢ (signed_add_overflows(consumed_bytes, bytes))
		die(_("pack too large for current definition of off_t"));
	consumed_bytes += bytes;
	ܐܢ (max_input_size && consumed_bytes > max_input_size) {
		بنية strbuf size_limit = STRBUF_INIT;
		strbuf_humanise_bytes(&size_limit, max_input_size);
		die(_("pack exceeds maximum allowed size (%s)"),
		    size_limit.buf);
	}
}

ساكن ثابت حرف *open_pack_file(ثابت حرف *pack_name)
{
	ܐܢ (from_stdin) {
		input_fd = 0;
		ܐܢ (!pack_name) {
			بنية strbuf tmp_file = STRBUF_INIT;
			output_fd = odb_mkstemp(the_repository->objects, &tmp_file,
						"pack/tmp_pack_XXXXXX");
			pack_name = strbuf_detach(&tmp_file, عدم);
		} ܐܚܪܢܐ {
			output_fd = xopen(pack_name, O_CREAT|O_EXCL|O_RDWR, 0600);
		}
		nothread_data.pack_fd = output_fd;
	} ܐܚܪܢܐ {
		input_fd = xopen(pack_name, O_RDONLY);
		output_fd = -1;
		nothread_data.pack_fd = input_fd;
	}
	git_hash_init(&input_ctx, the_hash_algo);
	ܦܢܐ pack_name;
}

ساكن فراغ parse_pack_header(فراغ)
{
	unsigned حرف *hdr = fill(حجم(بنية pack_header));

	/* Header consistency check */
	ܐܢ (get_be32(hdr) != PACK_SIGNATURE)
		die(_("pack signature mismatch"));
	hdr += 4;
	ܐܢ (!pack_version_ok_native(get_be32(hdr)))
		die(_("pack version %"PRIu32" unsupported"),
		    get_be32(hdr));
	hdr += 4;

	nr_objects = get_be32(hdr);
	use(حجم(بنية pack_header));
}

__attribute__((format (printf, 2, 3)))
ساكن NORETURN فراغ bad_object(off_t offset, ثابت حرف *format, ...)
{
	va_list params;
	حرف buf[1024];

	va_start(params, format);
	vsnprintf(buf, حجم(buf), format, params);
	va_end(params);
	die(_("pack has bad object at offset %"PRIuMAX": %s"),
	    (uintmax_t)offset, buf);
}

ساكن مضمن بنية thread_local_data *get_thread_data(فراغ)
{
	ܐܢ (HAVE_THREADS) {
		ܐܢ (threads_active)
			ܦܢܐ pthread_getspecific(key);
		أكد(!threads_active &&
		       "This should only be reached when all threads are gone");
	}
	ܦܢܐ &nothread_data;
}

ساكن فراغ set_thread_data(بنية thread_local_data *data)
{
	ܐܢ (threads_active)
		pthread_setspecific(key, data);
}

ساكن فراغ free_base_data(بنية base_data *c)
{
	ܐܢ (c->data) {
		FREE_AND_NULL(c->data);
		base_cache_used -= c->size;
	}
}

ساكن فراغ prune_base_data(بنية base_data *retain)
{
	بنية list_head *pos;

	ܐܢ (base_cache_used <= base_cache_limit)
		ܦܢܐ;

	list_for_each_prev(pos, &done_head) {
		بنية base_data *b = list_entry(pos, بنية base_data, list);
		ܐܢ (b->retain_data || b == retain)
			استمر;
		ܐܢ (b->data) {
			free_base_data(b);
			ܐܢ (base_cache_used <= base_cache_limit)
				ܦܢܐ;
		}
	}

	list_for_each_prev(pos, &work_head) {
		بنية base_data *b = list_entry(pos, بنية base_data, list);
		ܐܢ (b->retain_data || b == retain)
			استمر;
		ܐܢ (b->data) {
			free_base_data(b);
			ܐܢ (base_cache_used <= base_cache_limit)
				ܦܢܐ;
		}
	}
}

ساكن صحيح is_delta_type(تعداد object_type type)
{
	ܦܢܐ (type == OBJ_REF_DELTA || type == OBJ_OFS_DELTA);
}

ساكن فراغ *unpack_entry_data(off_t offset, size_t size,
			       تعداد object_type type, بنية object_id *oid)
{
	ساكن حرف fixed_buf[8192];
	صحيح status;
	git_zstream stream;
	فراغ *buf;
	بنية git_hash_ctx c;
	حرف hdr[32];
	صحيح hdrlen;

	ܐܢ (!is_delta_type(type)) {
		hdrlen = format_object_header(hdr, حجم(hdr), type, size);
		git_hash_init(&c, the_hash_algo);
		git_hash_update(&c, hdr, hdrlen);
	} ܐܚܪܢܐ
		oid = عدم;
	ܐܢ (type == OBJ_BLOB &&
	    size > repo_settings_get_big_file_threshold(the_repository))
		buf = fixed_buf;
	ܐܚܪܢܐ
		buf = xmallocz(size);

	memset(&stream, 0, حجم(stream));
	git_inflate_init(&stream);
	stream.next_out = buf;
	stream.avail_out = buf == fixed_buf ? حجم(fixed_buf) : size;

	ܥܒܕ {
		unsigned حرف *last_out = stream.next_out;
		stream.next_in = fill(1);
		stream.avail_in = input_len;
		status = git_inflate(&stream, 0);
		use(input_len - stream.avail_in);
		ܐܢ (oid)
			git_hash_update(&c, last_out, stream.next_out - last_out);
		ܐܢ (buf == fixed_buf) {
			stream.next_out = buf;
			stream.avail_out = حجم(fixed_buf);
		}
	} ܟܡܐ (status == Z_OK);
	ܐܢ (stream.total_out != size || status != Z_STREAM_END)
		bad_object(offset, _("inflate returned %d"), status);
	git_inflate_end(&stream);
	ܐܢ (oid)
		git_hash_final_oid(oid, &c);
	ܦܢܐ buf == fixed_buf ? عدم : buf;
}

ساكن فراغ *unpack_raw_entry(بنية object_entry *obj,
			      off_t *ofs_offset,
			      بنية object_id *ref_oid,
			      بنية object_id *oid)
{
	unsigned حرف *p;
	size_t size, c;
	off_t base_offset;
	unsigned shift;
	فراغ *data;

	obj->idx.offset = consumed_bytes;
	input_crc32 = crc32(0, عدم, 0);

	p = fill(1);
	c = *p;
	use(1);
	obj->type = (c >> 4) & 7;
	size = (c & 15);
	shift = 4;
	ܟܡܐ (c & 0x80) {
		ܐܢ ((bitsizeof(size_t) - 7) < shift)
			die(_("object size too large for this platform"));
		p = fill(1);
		c = *p;
		use(1);
		size += (c & 0x7f) << shift;
		shift += 7;
	}
	obj->size = size;

	بدل (obj->type) {
	حالة OBJ_REF_DELTA:
		oidread(ref_oid, fill(the_hash_algo->rawsz),
			the_repository->hash_algo);
		use(the_hash_algo->rawsz);
		اكسر;
	حالة OBJ_OFS_DELTA:
		p = fill(1);
		c = *p;
		use(1);
		base_offset = c & 127;
		ܟܡܐ (c & 128) {
			base_offset += 1;
			ܐܢ (!base_offset || MSB(base_offset, 7))
				bad_object(obj->idx.offset, _("offset value overflow for delta base object"));
			p = fill(1);
			c = *p;
			use(1);
			base_offset = (base_offset << 7) + (c & 127);
		}
		*ofs_offset = obj->idx.offset - base_offset;
		ܐܢ (*ofs_offset <= 0 || *ofs_offset >= obj->idx.offset)
			bad_object(obj->idx.offset, _("delta base offset is out of bound"));
		اكسر;
	حالة OBJ_COMMIT:
	حالة OBJ_TREE:
	حالة OBJ_BLOB:
	حالة OBJ_TAG:
		اكسر;
	افتراضي:
		bad_object(obj->idx.offset, _("unknown object type %d"), obj->type);
	}
	obj->hdr_size = consumed_bytes - obj->idx.offset;

	data = unpack_entry_data(obj->idx.offset, obj->size, obj->type, oid);
	obj->idx.crc32 = input_crc32;
	ܦܢܐ data;
}

ساكن فراغ *unpack_data(بنية object_entry *obj,
			 صحيح (*consume)(ثابت unsigned حرف *, unsigned long, فراغ *),
			 فراغ *cb_data)
{
	off_t from = obj[0].idx.offset + obj[0].hdr_size;
	off_t len = obj[1].idx.offset - from;
	unsigned حرف *data, *inbuf;
	git_zstream stream;
	صحيح status;

	data = xmallocz(consume ? 64*1024 : obj->size);
	inbuf = xmalloc((len < 64*1024) ? (صحيح)len : 64*1024);

	memset(&stream, 0, حجم(stream));
	git_inflate_init(&stream);
	stream.next_out = data;
	stream.avail_out = consume ? 64*1024 : obj->size;

	ܥܒܕ {
		ssize_t n = (len < 64*1024) ? (ssize_t)len : 64*1024;
		n = xpread(get_thread_data()->pack_fd, inbuf, n, from);
		ܐܢ (n < 0)
			die_errno(_("cannot pread pack file"));
		ܐܢ (!n)
			die(Q_("premature end of pack file, %"PRIuMAX" byte missing",
			       "premature end of pack file, %"PRIuMAX" bytes missing",
			       len),
			    (uintmax_t)len);
		from += n;
		len -= n;
		stream.next_in = inbuf;
		stream.avail_in = n;
		ܐܢ (!consume)
			status = git_inflate(&stream, 0);
		ܐܚܪܢܐ {
			ܥܒܕ {
				status = git_inflate(&stream, 0);
				ܐܢ (consume(data, stream.next_out - data, cb_data)) {
					حرر(inbuf);
					حرر(data);
					ܦܢܐ عدم;
				}
				stream.next_out = data;
				stream.avail_out = 64*1024;
			} ܟܡܐ (status == Z_OK && stream.avail_in);
		}
	} ܟܡܐ (len && status == Z_OK && !stream.avail_in);

	/* This has been inflated OK when first encountered, so... */
	ܐܢ (status != Z_STREAM_END || stream.total_out != obj->size)
		die(_("serious inflate inconsistency"));

	git_inflate_end(&stream);
	حرر(inbuf);
	ܐܢ (consume) {
		FREE_AND_NULL(data);
	}
	ܦܢܐ data;
}

ساكن فراغ *get_data_from_pack(بنية object_entry *obj)
{
	ܦܢܐ unpack_data(obj, عدم, عدم);
}

ساكن صحيح compare_ofs_delta_bases(off_t offset1, off_t offset2,
				   تعداد object_type type1,
				   تعداد object_type type2)
{
	صحيح cmp = type1 - type2;
	ܐܢ (cmp)
		ܦܢܐ cmp;
	ܦܢܐ offset1 < offset2 ? -1 :
	       offset1 > offset2 ?  1 :
	       0;
}

ساكن صحيح find_ofs_delta(ثابت off_t offset)
{
	صحيح first = 0, last = nr_ofs_deltas;

	ܟܡܐ (first < last) {
		صحيح next = first + (last - first) / 2;
		بنية ofs_delta_entry *delta = &ofs_deltas[next];
		صحيح cmp;

		cmp = compare_ofs_delta_bases(offset, delta->offset,
					      OBJ_OFS_DELTA,
					      objects[delta->obj_no].type);
		ܐܢ (!cmp)
			ܦܢܐ next;
		ܐܢ (cmp < 0) {
			last = next;
			استمر;
		}
		first = next+1;
	}
	ܦܢܐ -first-1;
}

ساكن فراغ find_ofs_delta_children(off_t offset,
				    صحيح *first_index, صحيح *last_index)
{
	صحيح first = find_ofs_delta(offset);
	صحيح last = first;
	صحيح end = nr_ofs_deltas - 1;

	ܐܢ (first < 0) {
		*first_index = 0;
		*last_index = -1;
		ܦܢܐ;
	}
	ܟܡܐ (first > 0 && ofs_deltas[first - 1].offset == offset)
		--first;
	ܟܡܐ (last < end && ofs_deltas[last + 1].offset == offset)
		++last;
	*first_index = first;
	*last_index = last;
}

ساكن صحيح compare_ref_delta_bases(ثابت بنية object_id *oid1,
				   ثابت بنية object_id *oid2,
				   تعداد object_type type1,
				   تعداد object_type type2)
{
	صحيح cmp = type1 - type2;
	ܐܢ (cmp)
		ܦܢܐ cmp;
	ܦܢܐ oidcmp(oid1, oid2);
}

ساكن صحيح find_ref_delta(ثابت بنية object_id *oid)
{
	صحيح first = 0, last = nr_ref_deltas;

	ܟܡܐ (first < last) {
		صحيح next = first + (last - first) / 2;
		بنية ref_delta_entry *delta = &ref_deltas[next];
		صحيح cmp;

		cmp = compare_ref_delta_bases(oid, &delta->oid,
					      OBJ_REF_DELTA,
					      objects[delta->obj_no].type);
		ܐܢ (!cmp)
			ܦܢܐ next;
		ܐܢ (cmp < 0) {
			last = next;
			استمر;
		}
		first = next+1;
	}
	ܦܢܐ -first-1;
}

ساكن فراغ find_ref_delta_children(ثابت بنية object_id *oid,
				    صحيح *first_index, صحيح *last_index)
{
	صحيح first = find_ref_delta(oid);
	صحيح last = first;
	صحيح end = nr_ref_deltas - 1;

	ܐܢ (first < 0) {
		*first_index = 0;
		*last_index = -1;
		ܦܢܐ;
	}
	ܟܡܐ (first > 0 && oideq(&ref_deltas[first - 1].oid, oid))
		--first;
	ܟܡܐ (last < end && oideq(&ref_deltas[last + 1].oid, oid))
		++last;
	*first_index = first;
	*last_index = last;
}

بنية compare_data {
	بنية object_entry *entry;
	بنية odb_stream *st;
	unsigned حرف *buf;
	unsigned long buf_size;
};

ساكن صحيح compare_objects(ثابت unsigned حرف *buf, unsigned long size,
			   فراغ *cb_data)
{
	بنية compare_data *data = cb_data;

	ܐܢ (data->buf_size < size) {
		حرر(data->buf);
		data->buf = xmalloc(size);
		data->buf_size = size;
	}

	ܟܡܐ (size) {
		ssize_t len = odb_stream_read(data->st, data->buf, size);
		ܐܢ (len == 0)
			die(_("SHA1 COLLISION FOUND WITH %s !"),
			    oid_to_hex(&data->entry->idx.oid));
		ܐܢ (len < 0)
			die(_("unable to read %s"),
			    oid_to_hex(&data->entry->idx.oid));
		ܐܢ (memcmp(buf, data->buf, len))
			die(_("SHA1 COLLISION FOUND WITH %s !"),
			    oid_to_hex(&data->entry->idx.oid));
		size -= len;
		buf += len;
	}
	ܦܢܐ 0;
}

ساكن صحيح check_collison(بنية object_entry *entry)
{
	بنية compare_data data;

	ܐܢ (entry->size <= repo_settings_get_big_file_threshold(the_repository) ||
	    entry->type != OBJ_BLOB)
		ܦܢܐ -1;

	memset(&data, 0, حجم(data));
	data.entry = entry;
	data.st = odb_stream_from_object(the_repository->objects, &entry->idx.oid, عدم);
	ܐܢ (!data.st)
		ܦܢܐ -1;
	ܐܢ (data.st->size != entry->size || data.st->type != entry->type)
		die(_("SHA1 COLLISION FOUND WITH %s !"),
		    oid_to_hex(&entry->idx.oid));
	unpack_data(entry, compare_objects, &data);
	odb_stream_close(data.st);
	حرر(data.buf);
	ܦܢܐ 0;
}

ساكن فراغ record_outgoing_link(ثابت بنية object_id *oid)
{
	oidset_insert(&outgoing_links, oid);
}

ساكن فراغ maybe_record_name_entry(ثابت بنية name_entry *entry)
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
	ܐܢ (S_ISDIR(entry->mode))
		record_outgoing_link(&entry->oid);
}

ساكن فراغ do_record_outgoing_links(بنية object *obj)
{
	ܐܢ (obj->type == OBJ_TREE) {
		بنية tree *tree = (بنية tree *)obj;
		بنية tree_desc desc;
		بنية name_entry entry;
		ܐܢ (init_tree_desc_gently(&desc, &tree->object.oid,
					  tree->buffer, tree->size, 0))
			/*
			 * Error messages are given when packs are
			 * verified, so do not print any here.
			 */
			ܦܢܐ;
		ܟܡܐ (tree_entry_gently(&desc, &entry))
			maybe_record_name_entry(&entry);
	} ܐܚܪܢܐ ܐܢ (obj->type == OBJ_COMMIT) {
		بنية commit *commit = (بنية commit *) obj;
		بنية commit_list *parents = commit->parents;

		record_outgoing_link(get_commit_tree_oid(commit));
		لكل (; parents; parents = parents->next)
			record_outgoing_link(&parents->item->object.oid);
	} ܐܚܪܢܐ ܐܢ (obj->type == OBJ_TAG) {
		بنية tag *tag = (بنية tag *) obj;
		record_outgoing_link(get_tagged_oid(tag));
	}
}

ساكن فراغ sha1_object(ثابت فراغ *data, بنية object_entry *obj_entry,
			unsigned long size, تعداد object_type type,
			ثابت بنية object_id *oid)
{
	فراغ *new_data = عدم;
	صحيح collision_test_needed = 0;

	أكد(data || obj_entry);

	ܐܢ (startup_info->have_repository) {
		read_lock();
		collision_test_needed = odb_has_object(the_repository->objects, oid,
						       ODB_HAS_OBJECT_FETCH_PROMISOR);
		read_unlock();
	}

	ܐܢ (collision_test_needed && !data) {
		read_lock();
		ܐܢ (!check_collison(obj_entry))
			collision_test_needed = 0;
		read_unlock();
	}
	ܐܢ (collision_test_needed) {
		فراغ *has_data;
		تعداد object_type has_type;
		size_t has_size;
		read_lock();
		has_type = odb_read_object_info(the_repository->objects, oid, &has_size);
		ܐܢ (has_type < 0)
			die(_("cannot read existing object info %s"), oid_to_hex(oid));
		ܐܢ (has_type != type || has_size != size)
			die(_("SHA1 COLLISION FOUND WITH %s !"), oid_to_hex(oid));
		has_data = odb_read_object(the_repository->objects, oid,
					   &has_type, &has_size);
		read_unlock();
		ܐܢ (!data)
			data = new_data = get_data_from_pack(obj_entry);
		ܐܢ (!has_data)
			die(_("cannot read existing object %s"), oid_to_hex(oid));
		ܐܢ (size != has_size || type != has_type ||
		    memcmp(data, has_data, size) != 0)
			die(_("SHA1 COLLISION FOUND WITH %s !"), oid_to_hex(oid));
		حرر(has_data);
	}

	ܐܢ (strict || do_fsck_object || record_outgoing_links) {
		read_lock();
		ܐܢ (type == OBJ_BLOB) {
			بنية blob *blob = lookup_blob(the_repository, oid);
			ܐܢ (blob)
				blob->object.flags |= FLAG_CHECKED;
			ܐܚܪܢܐ
				die(_("invalid blob object %s"), oid_to_hex(oid));
			ܐܢ (do_fsck_object &&
			    fsck_object(&blob->object, (فراغ *)data, size, &fsck_options))
				die(_("fsck error in packed object"));
		} ܐܚܪܢܐ {
			بنية object *obj;
			صحيح eaten;
			فراغ *buf = (فراغ *) data;

			أكد(data && "data can only be NULL for large _blobs_");

			/*
			 * we do not need to free the memory here, as the
			 * buf is deleted by the caller.
			 */
			obj = parse_object_buffer(the_repository, oid, type,
						  size, buf,
						  &eaten);
			ܐܢ (!obj)
				die(_("invalid %s"), type_name(type));
			ܐܢ (do_fsck_object &&
			    fsck_object(obj, buf, size, &fsck_options))
				die(_("fsck error in packed object"));
			ܐܢ (strict && fsck_walk(obj, عدم, &fsck_options))
				die(_("Not all child objects of %s are reachable"), oid_to_hex(&obj->oid));
			ܐܢ (record_outgoing_links)
				do_record_outgoing_links(obj);

			ܐܢ (obj->type == OBJ_TREE) {
				بنية tree *item = (بنية tree *) obj;
				item->buffer = عدم;
				obj->parsed = 0;
			}
			ܐܢ (obj->type == OBJ_COMMIT) {
				بنية commit *commit = (بنية commit *) obj;
				ܐܢ (detach_commit_buffer(commit, عدم) != data)
					BUG("parse_object_buffer transmogrified our buffer");
			}
			obj->flags |= FLAG_CHECKED;
		}
		read_unlock();
	}

	حرر(new_data);
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
ساكن فراغ *get_base_data(بنية base_data *c)
{
	ܐܢ (!c->data) {
		بنية object_entry *obj = c->obj;
		بنية base_data **delta = عدم;
		صحيح delta_nr = 0, delta_alloc = 0;

		ܟܡܐ (is_delta_type(c->obj->type) && !c->data) {
			ALLOC_GROW(delta, delta_nr + 1, delta_alloc);
			delta[delta_nr++] = c;
			c = c->base;
		}
		ܐܢ (!delta_nr) {
			c->data = get_data_from_pack(obj);
			c->size = obj->size;
			base_cache_used += c->size;
			prune_base_data(c);
		}
		لكل (; delta_nr > 0; delta_nr--) {
			فراغ *base, *raw;
			c = delta[delta_nr - 1];
			obj = c->obj;
			base = get_base_data(c->base);
			raw = get_data_from_pack(obj);
			c->data = patch_delta(
				base, c->base->size,
				raw, obj->size,
				&c->size);
			حرر(raw);
			ܐܢ (!c->data)
				bad_object(obj->idx.offset, _("failed to apply delta"));
			base_cache_used += c->size;
			prune_base_data(c);
		}
		حرر(delta);
	}
	ܦܢܐ c->data;
}

ساكن بنية base_data *make_base(بنية object_entry *obj,
				   بنية base_data *parent)
{
	بنية base_data *base = xcalloc(1, حجم(بنية base_data));
	base->base = parent;
	base->obj = obj;
	find_ref_delta_children(&obj->idx.oid,
				&base->ref_first, &base->ref_last);
	find_ofs_delta_children(obj->idx.offset,
				&base->ofs_first, &base->ofs_last);
	base->children_remaining = base->ref_last - base->ref_first +
		base->ofs_last - base->ofs_first + 2;
	ܦܢܐ base;
}

ساكن بنية base_data *resolve_delta(بنية object_entry *delta_obj,
				       بنية base_data *base)
{
	فراغ *delta_data, *result_data;
	بنية base_data *result;
	size_t result_size;

	ܐܢ (show_stat) {
		صحيح i = delta_obj - objects;
		صحيح j = base->obj - objects;
		obj_stat[i].delta_depth = obj_stat[j].delta_depth + 1;
		deepest_delta_lock();
		ܐܢ (deepest_delta < obj_stat[i].delta_depth)
			deepest_delta = obj_stat[i].delta_depth;
		deepest_delta_unlock();
		obj_stat[i].base_object_no = j;
	}
	delta_data = get_data_from_pack(delta_obj);
	أكد(base->data);
	result_data = patch_delta(base->data, base->size,
				  delta_data, delta_obj->size, &result_size);
	حرر(delta_data);
	ܐܢ (!result_data)
		bad_object(delta_obj->idx.offset, _("failed to apply delta"));
	hash_object_file(the_hash_algo, result_data, result_size,
			 delta_obj->real_type, &delta_obj->idx.oid);
	sha1_object(result_data, عدم, result_size, delta_obj->real_type,
		    &delta_obj->idx.oid);

	result = make_base(delta_obj, base);
	result->data = result_data;
	result->size = result_size;

	counter_lock();
	nr_resolved_deltas++;
	counter_unlock();

	ܦܢܐ result;
}

ساكن صحيح compare_ofs_delta_entry(ثابت فراغ *a, ثابت فراغ *b)
{
	ثابت بنية ofs_delta_entry *delta_a = a;
	ثابت بنية ofs_delta_entry *delta_b = b;

	ܦܢܐ delta_a->offset < delta_b->offset ? -1 :
	       delta_a->offset > delta_b->offset ?  1 :
	       0;
}

ساكن صحيح compare_ref_delta_entry(ثابت فراغ *a, ثابت فراغ *b)
{
	ثابت بنية ref_delta_entry *delta_a = a;
	ثابت بنية ref_delta_entry *delta_b = b;

	ܦܢܐ oidcmp(&delta_a->oid, &delta_b->oid);
}

ساكن فراغ *threaded_second_pass(فراغ *data)
{
	ܐܢ (data)
		set_thread_data(data);
	لكل (;;) {
		بنية base_data *parent = عدم;
		بنية object_entry *child_obj = عدم;
		بنية base_data *child = عدم;

		counter_lock();
		display_progress(progress, nr_resolved_deltas);
		counter_unlock();

		work_lock();
		ܐܢ (list_empty(&work_head)) {
			/*
			 * Take an object from the object array.
			 */
			ܟܡܐ (nr_dispatched < nr_objects &&
			       is_delta_type(objects[nr_dispatched].type))
				nr_dispatched++;
			ܐܢ (nr_dispatched >= nr_objects) {
				work_unlock();
				اكسر;
			}
			child_obj = &objects[nr_dispatched++];
		} ܐܚܪܢܐ {
			/*
			 * Peek at the top of the stack, and take a child from
			 * it.
			 */
			parent = list_first_entry(&work_head, بنية base_data,
						  list);

			ܟܡܐ (parent->ref_first <= parent->ref_last) {
				صحيح offset = ref_deltas[parent->ref_first++].obj_no;
				child_obj = objects + offset;
				ܐܢ (child_obj->real_type != OBJ_REF_DELTA) {
					child_obj = عدم;
					استمر;
				}
				child_obj->real_type = parent->obj->real_type;
				اكسر;
			}

			ܐܢ (!child_obj && parent->ofs_first <= parent->ofs_last) {
				child_obj = objects +
					ofs_deltas[parent->ofs_first++].obj_no;
				أكد(child_obj->real_type == OBJ_OFS_DELTA);
				child_obj->real_type = parent->obj->real_type;
			}

			ܐܢ (parent->ref_first > parent->ref_last &&
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

		ܐܢ (child_obj) {
			ܐܢ (parent) {
				child = resolve_delta(child_obj, parent);
				ܐܢ (!child->children_remaining)
					FREE_AND_NULL(child->data);
			} ܐܚܪܢܐ{
				child = make_base(child_obj, عدم);
				ܐܢ (child->children_remaining) {
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
		ܐܢ (parent)
			parent->retain_data--;

		ܐܢ (child && child->data) {
			/*
			 * This child has its own children, so add it to
			 * work_head.
			 */
			list_add(&child->list, &work_head);
			base_cache_used += child->size;
			prune_base_data(عدم);
		} ܐܚܪܢܐ ܐܢ (child) {
			/*
			 * This child does not have its own children. It may be
			 * the last descendant of its ancestors; free those
			 * that we can.
			 */
			بنية base_data *p = parent;

			ܟܡܐ (p) {
				بنية base_data *next_p;

				p->children_remaining--;
				ܐܢ (p->children_remaining)
					اكسر;

				next_p = p->base;
				free_base_data(p);
				list_del(&p->list);
				حرر(p);

				p = next_p;
			}
			FREE_AND_NULL(child);
		}
		work_unlock();
	}
	ܦܢܐ عدم;
}

/*
 * First pass:
 * - find locations of all objects;
 * - calculate SHA1 of all non-delta objects;
 * - remember base (SHA1 or offset) for all deltas.
 */
ساكن فراغ parse_pack_objects(unsigned حرف *hash)
{
	صحيح i, nr_delays = 0;
	بنية ofs_delta_entry *ofs_delta = ofs_deltas;
	بنية object_id ref_delta_oid;
	بنية stat st;
	بنية git_hash_ctx tmp_ctx;

	ܐܢ (verbose)
		progress = start_progress(
				the_repository,
				progress_title ? progress_title :
				from_stdin ? _("Receiving objects") : _("Indexing objects"),
				nr_objects);
	لكل (i = 0; i < nr_objects; i++) {
		بنية object_entry *obj = &objects[i];
		فراغ *data = unpack_raw_entry(obj, &ofs_delta->offset,
					      &ref_delta_oid,
					      &obj->idx.oid);
		obj->real_type = obj->type;
		ܐܢ (obj->type == OBJ_OFS_DELTA) {
			nr_ofs_deltas++;
			ofs_delta->obj_no = i;
			ofs_delta++;
		} ܐܚܪܢܐ ܐܢ (obj->type == OBJ_REF_DELTA) {
			ALLOC_GROW(ref_deltas, nr_ref_deltas + 1, ref_deltas_alloc);
			oidcpy(&ref_deltas[nr_ref_deltas].oid, &ref_delta_oid);
			ref_deltas[nr_ref_deltas].obj_no = i;
			nr_ref_deltas++;
		} ܐܚܪܢܐ ܐܢ (!data) {
			/* large blobs, check later */
			obj->real_type = OBJ_BAD;
			nr_delays++;
		} ܐܚܪܢܐ
			sha1_object(data, عدم, obj->size, obj->type,
				    &obj->idx.oid);
		حرر(data);
		display_progress(progress, i+1);
	}
	objects[i].idx.offset = consumed_bytes;
	stop_progress(&progress);

	/* Check pack integrity */
	flush();
	git_hash_init(&tmp_ctx, the_hash_algo);
	git_hash_clone(&tmp_ctx, &input_ctx);
	git_hash_final(hash, &tmp_ctx);
	ܐܢ (!hasheq(fill(the_hash_algo->rawsz), hash, the_repository->hash_algo))
		die(_("pack is corrupted (SHA1 mismatch)"));
	use(the_hash_algo->rawsz);

	/* If input_fd is a file, we should have reached its end now. */
	ܐܢ (fstat(input_fd, &st))
		die_errno(_("cannot fstat packfile"));
	ܐܢ (S_ISREG(st.st_mode) &&
			lseek(input_fd, 0, SEEK_CUR) - input_len != st.st_size)
		die(_("pack has junk at the end"));

	لكل (i = 0; i < nr_objects; i++) {
		بنية object_entry *obj = &objects[i];
		ܐܢ (obj->real_type != OBJ_BAD)
			استمر;
		obj->real_type = obj->type;
		sha1_object(عدم, obj, obj->size, obj->type,
			    &obj->idx.oid);
		nr_delays--;
	}
	ܐܢ (nr_delays)
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
ساكن فراغ resolve_deltas(بنية pack_idx_option *opts)
{
	صحيح i;

	ܐܢ (!nr_ofs_deltas && !nr_ref_deltas)
		ܦܢܐ;

	/* Sort deltas by base SHA1/offset for fast searching */
	QSORT(ofs_deltas, nr_ofs_deltas, compare_ofs_delta_entry);
	QSORT(ref_deltas, nr_ref_deltas, compare_ref_delta_entry);

	ܐܢ (verbose || show_resolving_progress)
		progress = start_progress(the_repository,
					  _("Resolving deltas"),
					  nr_ref_deltas + nr_ofs_deltas);

	nr_dispatched = 0;
	base_cache_limit = opts->delta_base_cache_limit * nr_threads;
	ܐܢ (nr_threads > 1 || getenv("GIT_FORCE_THREADS")) {
		init_thread();
		لكل (i = 0; i < nr_threads; i++) {
			صحيح ret = pthread_create(&thread_data[i].thread, عدم,
						 threaded_second_pass, thread_data + i);
			ܐܢ (ret)
				die(_("unable to create thread: %s"),
				    strerror(ret));
		}
		لكل (i = 0; i < nr_threads; i++)
			pthread_join(thread_data[i].thread, عدم);
		cleanup_thread();
		ܦܢܐ;
	}
	threaded_second_pass(&nothread_data);
}

/*
 * Third pass:
 * - append objects to convert thin pack to full pack if required
 * - write the final pack hash
 */
ساكن فراغ fix_unresolved_deltas(بنية hashfile *f);
ساكن فراغ conclude_pack(صحيح fix_thin_pack, ثابت حرف *curr_pack, unsigned حرف *pack_hash)
{
	ܐܢ (nr_ref_deltas + nr_ofs_deltas == nr_resolved_deltas) {
		stop_progress(&progress);
		/* Flush remaining pack final hash. */
		flush();
		ܦܢܐ;
	}

	ܐܢ (fix_thin_pack) {
		بنية hashfile *f;
		unsigned حرف read_hash[GIT_MAX_RAWSZ], tail_hash[GIT_MAX_RAWSZ];
		بنية strbuf msg = STRBUF_INIT;
		صحيح nr_unresolved = nr_ofs_deltas + nr_ref_deltas - nr_resolved_deltas;
		صحيح nr_objects_initial = nr_objects;
		ܐܢ (nr_unresolved <= 0)
			die(_("confusion beyond insanity"));
		REALLOC_ARRAY(objects, nr_objects + nr_unresolved + 1);
		memset(objects + nr_objects + 1, 0,
		       nr_unresolved * حجم(*objects));
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
		ܐܢ (!hasheq(read_hash, tail_hash, the_repository->hash_algo))
			die(_("Unexpected tail checksum for %s "
			      "(disk corruption?)"), curr_pack);
	}
	ܐܢ (nr_ofs_deltas + nr_ref_deltas != nr_resolved_deltas)
		die(Q_("pack has %d unresolved delta",
		       "pack has %d unresolved deltas",
		       nr_ofs_deltas + nr_ref_deltas - nr_resolved_deltas),
		    nr_ofs_deltas + nr_ref_deltas - nr_resolved_deltas);
}

ساكن صحيح write_compressed(بنية hashfile *f, فراغ *in, unsigned صحيح size)
{
	git_zstream stream;
	صحيح status;
	unsigned حرف outbuf[4096];
	بنية repo_config_values *cfg = repo_config_values(the_repository);

	git_deflate_init(&stream, cfg->zlib_compression_level);
	stream.next_in = in;
	stream.avail_in = size;

	ܥܒܕ {
		stream.next_out = outbuf;
		stream.avail_out = حجم(outbuf);
		status = git_deflate(&stream, Z_FINISH);
		hashwrite(f, outbuf, حجم(outbuf) - stream.avail_out);
	} ܟܡܐ (status == Z_OK);

	ܐܢ (status != Z_STREAM_END)
		die(_("unable to deflate appended object (%d)"), status);
	size = stream.total_out;
	git_deflate_end(&stream);
	ܦܢܐ size;
}

ساكن بنية object_entry *append_obj_to_pack(بنية hashfile *f,
			       ثابت unsigned حرف *sha1, فراغ *buf,
			       unsigned long size, تعداد object_type type)
{
	بنية object_entry *obj = &objects[nr_objects++];
	unsigned حرف header[10];
	unsigned long s = size;
	صحيح n = 0;
	unsigned حرف c = (type << 4) | (s & 15);
	s >>= 4;
	ܟܡܐ (s) {
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
	ܦܢܐ obj;
}

ساكن صحيح delta_pos_compare(ثابت فراغ *_a, ثابت فراغ *_b)
{
	بنية ref_delta_entry *a = *(بنية ref_delta_entry **)_a;
	بنية ref_delta_entry *b = *(بنية ref_delta_entry **)_b;
	ܦܢܐ a->obj_no - b->obj_no;
}

ساكن فراغ fix_unresolved_deltas(بنية hashfile *f)
{
	بنية ref_delta_entry **sorted_by_pos;
	صحيح i;

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
	لكل (i = 0; i < nr_ref_deltas; i++)
		sorted_by_pos[i] = &ref_deltas[i];
	QSORT(sorted_by_pos, nr_ref_deltas, delta_pos_compare);

	ܐܢ (repo_has_promisor_remote(the_repository)) {
		/*
		 * Prefetch the delta bases.
		 */
		بنية oid_array to_fetch = OID_ARRAY_INIT;
		لكل (i = 0; i < nr_ref_deltas; i++) {
			بنية ref_delta_entry *d = sorted_by_pos[i];
			ܐܢ (!odb_read_object_info_extended(the_repository->objects,
							   &d->oid, عدم,
							   OBJECT_INFO_FOR_PREFETCH))
				استمر;
			oid_array_append(&to_fetch, &d->oid);
		}
		promisor_remote_get_direct(the_repository,
					   to_fetch.oid, to_fetch.nr);
		oid_array_clear(&to_fetch);
	}

	لكل (i = 0; i < nr_ref_deltas; i++) {
		بنية ref_delta_entry *d = sorted_by_pos[i];
		تعداد object_type type;
		فراغ *data;
		size_t size;

		ܐܢ (objects[d->obj_no].real_type != OBJ_REF_DELTA)
			استمر;
		data = odb_read_object(the_repository->objects, &d->oid,
				       &type, &size);
		ܐܢ (!data)
			استمر;

		ܐܢ (check_object_signature(the_repository, &d->oid, data, size,
					   type) < 0)
			die(_("local object %s is corrupt"), oid_to_hex(&d->oid));

		/*
		 * Add this as an object to the objects array and call
		 * threaded_second_pass() (which will pick up the added
		 * object).
		 */
		append_obj_to_pack(f, d->oid.hash, data, size, type);
		حرر(data);
		threaded_second_pass(عدم);

		display_progress(progress, nr_resolved_deltas);
	}
	حرر(sorted_by_pos);
}

ساكن ثابت حرف *derive_filename(ثابت حرف *pack_name, ثابت حرف *strip,
				   ثابت حرف *suffix, بنية strbuf *buf)
{
	size_t len;
	ܐܢ (!strip_suffix(pack_name, strip, &len) || !len ||
	    pack_name[len - 1] != '.')
		die(_("packfile name '%s' does not end with '.%s'"),
		    pack_name, strip);
	strbuf_add(buf, pack_name, len);
	strbuf_addstr(buf, suffix);
	ܦܢܐ buf->buf;
}

ساكن فراغ write_special_file(ثابت حرف *suffix, ثابت حرف *msg,
			       ثابت حرف *pack_name, ثابت unsigned حرف *hash,
			       ثابت حرف **report)
{
	بنية strbuf name_buf = STRBUF_INIT;
	ثابت حرف *filename;
	صحيح fd;
	صحيح msg_len = strlen(msg);

	ܐܢ (pack_name)
		filename = derive_filename(pack_name, "pack", suffix, &name_buf);
	ܐܚܪܢܐ
		filename = odb_pack_name(the_repository, &name_buf, hash, suffix);

	fd = safe_create_file_with_leading_directories(the_repository, filename);
	ܐܢ (fd < 0) {
		ܐܢ (errno != EEXIST)
			die_errno(_("cannot write %s file '%s'"),
				  suffix, filename);
	} ܐܚܪܢܐ {
		ܐܢ (msg_len > 0) {
			write_or_die(fd, msg, msg_len);
			write_or_die(fd, "\n", 1);
		}
		ܐܢ (close(fd) != 0)
			die_errno(_("cannot close written %s file '%s'"),
				  suffix, filename);
		ܐܢ (report)
			*report = suffix;
	}
	strbuf_release(&name_buf);
}

ساكن فراغ rename_tmp_packfile(ثابت حرف **final_name,
				ثابت حرف *curr_name,
				بنية strbuf *name, unsigned حرف *hash,
				ثابت حرف *ext, صحيح make_read_only_if_same)
{
	ܐܢ (!*final_name || strcmp(*final_name, curr_name)) {
		ܐܢ (!*final_name)
			*final_name = odb_pack_name(the_repository, name, hash, ext);
		ܐܢ (finalize_object_file(the_repository, curr_name, *final_name))
			die(_("unable to rename temporary '*.%s' file to '%s'"),
			    ext, *final_name);
	} ܐܚܪܢܐ ܐܢ (make_read_only_if_same) {
		chmod(*final_name, 0444);
	}
}

ساكن فراغ final(ثابت حرف *final_pack_name, ثابت حرف *curr_pack_name,
		  ثابت حرف *final_index_name, ثابت حرف *curr_index_name,
		  ثابت حرف *final_rev_index_name, ثابت حرف *curr_rev_index_name,
		  ثابت حرف *keep_msg, ثابت حرف *promisor_msg,
		  unsigned حرف *hash)
{
	ثابت حرف *report = "pack";
	بنية strbuf pack_name = STRBUF_INIT;
	بنية strbuf index_name = STRBUF_INIT;
	بنية strbuf rev_index_name = STRBUF_INIT;

	ܐܢ (!from_stdin) {
		close(input_fd);
	} ܐܚܪܢܐ {
		fsync_component_or_die(FSYNC_COMPONENT_PACK, output_fd, curr_pack_name);
		ܐܢ (close(output_fd))
			die_errno(_("error while closing pack file"));
	}

	ܐܢ (keep_msg)
		write_special_file("keep", keep_msg, final_pack_name, hash,
				   &report);
	ܐܢ (promisor_msg)
		write_special_file("promisor", promisor_msg, final_pack_name,
				   hash, عدم);

	rename_tmp_packfile(&final_pack_name, curr_pack_name, &pack_name,
			    hash, "pack", from_stdin);
	ܐܢ (curr_rev_index_name)
		rename_tmp_packfile(&final_rev_index_name, curr_rev_index_name,
				    &rev_index_name, hash, "rev", 1);
	rename_tmp_packfile(&final_index_name, curr_index_name, &index_name,
			    hash, "idx", 1);

	ܐܢ (do_fsck_object && startup_info->have_repository) {
		بنية odb_source_files *files =
			odb_source_files_downcast(the_repository->objects->source);
		packfile_store_load_pack(files->dirs->packed, final_index_name, 0);
	}

	ܐܢ (!from_stdin) {
		printf("%s\n", hash_to_hex(hash));
	} ܐܚܪܢܐ {
		بنية strbuf buf = STRBUF_INIT;

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

ساكن صحيح git_index_pack_config(ثابت حرف *k, ثابت حرف *v,
				 ثابت بنية config_context *ctx, فراغ *cb)
{
	بنية pack_idx_option *opts = cb;

	ܐܢ (!strcmp(k, "pack.indexversion")) {
		opts->version = git_config_int(k, v, ctx->kvi);
		ܐܢ (opts->version > 2)
			die(_("bad pack.indexVersion=%"PRIu32), opts->version);
		ܦܢܐ 0;
	}
	ܐܢ (!strcmp(k, "pack.threads")) {
		nr_threads = git_config_int(k, v, ctx->kvi);
		ܐܢ (nr_threads < 0)
			die(_("invalid number of threads specified (%d)"),
			    nr_threads);
		ܐܢ (!HAVE_THREADS && nr_threads != 1) {
			warning(_("no threads support, ignoring %s"), k);
			nr_threads = 1;
		}
		ܦܢܐ 0;
	}
	ܐܢ (!strcmp(k, "pack.writereverseindex")) {
		ܐܢ (git_config_bool(k, v))
			opts->flags |= WRITE_REV;
		ܐܚܪܢܐ
			opts->flags &= ~WRITE_REV;
	}
	ܐܢ (!strcmp(k, "core.deltabasecachelimit")) {
		opts->delta_base_cache_limit = git_config_ulong(k, v, ctx->kvi);
		ܦܢܐ 0;
	}
	ܦܢܐ git_default_config(k, v, ctx, cb);
}

ساكن صحيح cmp_uint32(ثابت فراغ *a_, ثابت فراغ *b_)
{
	uint32_t a = *((uint32_t *)a_);
	uint32_t b = *((uint32_t *)b_);

	ܦܢܐ (a < b) ? -1 : (a != b);
}

ساكن فراغ read_v2_anomalous_offsets(بنية packed_git *p,
				      بنية pack_idx_option *opts)
{
	ثابت uint32_t *idx1, *idx2;
	uint32_t i;

	/* The address of the 4-byte offset table */
	idx1 = (((ثابت uint32_t *)((ثابت uint8_t *)p->index_data + p->crc_offset))
		+ (size_t)p->num_objects /* CRC32 table */
		);

	/* The address of the 8-byte offset table */
	idx2 = idx1 + p->num_objects;

	لكل (i = 0; i < p->num_objects; i++) {
		uint32_t off = ntohl(idx1[i]);
		ܐܢ (!(off & 0x80000000))
			استمر;
		off = off & 0x7fffffff;
		check_pack_index_ptr(p, &idx2[off * 2]);
		ܐܢ (idx2[off * 2])
			استمر;
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

ساكن فراغ read_idx_option(بنية pack_idx_option *opts, ثابت حرف *pack_name)
{
	بنية packed_git *p = add_packed_git(the_repository, pack_name,
					      strlen(pack_name), 1);

	ܐܢ (!p)
		die(_("Cannot open existing pack file '%s'"), pack_name);
	ܐܢ (open_pack_index(p))
		die(_("Cannot open existing pack idx file for '%s'"), pack_name);

	/* Read the attributes from the existing idx file */
	opts->version = p->index_version;

	ܐܢ (opts->version == 2)
		read_v2_anomalous_offsets(p, opts);

	/*
	 * Get rid of the idx file as we do not need it anymore.
	 * NEEDSWORK: extract this bit from free_pack_by_name() in
	 * object-file.c, perhaps?  It shouldn't matter very much as we
	 * know we haven't installed this pack (hence we never have
	 * read anything from it).
	 */
	close_pack_index(p);
	حرر(p);
}

ساكن فراغ show_pack_info(صحيح stat_only)
{
	صحيح i, baseobjects = nr_objects - nr_ref_deltas - nr_ofs_deltas;
	unsigned long *chain_histogram = عدم;

	ܐܢ (deepest_delta)
		CALLOC_ARRAY(chain_histogram, deepest_delta);

	لكل (i = 0; i < nr_objects; i++) {
		بنية object_entry *obj = &objects[i];

		ܐܢ (is_delta_type(obj->type))
			chain_histogram[obj_stat[i].delta_depth - 1]++;
		ܐܢ (stat_only)
			استمر;
		printf("%s %-6s %"PRIuMAX" %"PRIuMAX" %"PRIuMAX,
		       oid_to_hex(&obj->idx.oid),
		       type_name(obj->real_type), (uintmax_t)obj->size,
		       (uintmax_t)(obj[1].idx.offset - obj->idx.offset),
		       (uintmax_t)obj->idx.offset);
		ܐܢ (is_delta_type(obj->type)) {
			بنية object_entry *bobj = &objects[obj_stat[i].base_object_no];
			printf(" %u %s", obj_stat[i].delta_depth,
			       oid_to_hex(&bobj->idx.oid));
		}
		putchar('\n');
	}

	ܐܢ (baseobjects)
		printf_ln(Q_("non delta: %d object",
			     "non delta: %d objects",
			     baseobjects),
			  baseobjects);
	لكل (i = 0; i < deepest_delta; i++) {
		ܐܢ (!chain_histogram[i])
			استمر;
		printf_ln(Q_("chain length = %d: %lu object",
			     "chain length = %d: %lu objects",
			     chain_histogram[i]),
			  i + 1,
			  chain_histogram[i]);
	}
	حرر(chain_histogram);
}

ساكن فراغ repack_local_links(فراغ)
{
	بنية child_process cmd = CHILD_PROCESS_INIT;
	FILE *out;
	بنية strbuf line = STRBUF_INIT;
	بنية oidset_iter iter;
	بنية object_id *oid;
	حرف *base_name = عدم;

	ܐܢ (!oidset_size(&outgoing_links))
		ܦܢܐ;

	oidset_iter_init(&outgoing_links, &iter);
	ܟܡܐ ((oid = oidset_iter_next(&iter))) {
		بنية odb_source_info source_info;
		بنية object_info info = {
			.source_infop = &source_info,
		};

		ܐܢ (odb_read_object_info_extended(the_repository->objects, oid, &info, 0))
			/* Missing; assume it is a promisor object */
			استمر;
		ܐܢ (source_info.source->type == ODB_SOURCE_PACKED &&
		    source_info.u.packed.pack->pack_promisor)
			استمر;

		ܐܢ (!cmd.args.nr) {
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
			ܐܢ (start_command(&cmd))
				die(_("could not start pack-objects to repack local links"));
		}

		ܐܢ (write_in_full(cmd.in, oid_to_hex(oid), the_hash_algo->hexsz) < 0 ||
		    write_in_full(cmd.in, "\n", 1) < 0)
			die(_("failed to feed local object to pack-objects"));
	}

	ܐܢ (!cmd.args.nr)
		ܦܢܐ;

	close(cmd.in);

	out = xfdopen(cmd.out, "r");
	ܟܡܐ (strbuf_getline_lf(&line, out) != EOF) {
		unsigned حرف binary[GIT_MAX_RAWSZ];
		ܐܢ (line.len != the_hash_algo->hexsz ||
		    !hex_to_bytes(binary, line.buf, line.len))
			die(_("index-pack: Expecting full hex object ID lines only from pack-objects."));

		/*
		 * pack-objects creates the .pack and .idx files, but not the
		 * .promisor file. Create the .promisor file, which is empty.
		 */
		write_special_file("promisor", "", عدم, binary, عدم);
	}

	fclose(out);
	ܐܢ (finish_command(&cmd))
		die(_("could not finish pack-objects to repack local links"));
	strbuf_release(&line);
	حرر(base_name);
}

صحيح cmd_index_pack(صحيح argc,
		   ثابت حرف **argv,
		   ثابت حرف *prefix,
		   بنية repository *repo)
{
	صحيح i, fix_thin_pack = 0, verify = 0, stat_only = 0, rev_index;
	ثابت حرف *curr_index;
	حرف *curr_rev_index = عدم;
	ثابت حرف *index_name = عدم, *pack_name = عدم, *rev_index_name = عدم;
	ثابت حرف *keep_msg = عدم;
	ثابت حرف *promisor_msg = عدم;
	بنية strbuf index_name_buf = STRBUF_INIT;
	بنية strbuf rev_index_name_buf = STRBUF_INIT;
	بنية pack_idx_entry **idx_objects;
	بنية pack_idx_option opts;
	unsigned حرف pack_hash[GIT_MAX_RAWSZ];
	unsigned foreign_nr = 1;	/* zero is a "good" value, assume bad */
	صحيح report_end_of_input = 0;
	صحيح hash_algo = 0;

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
	ܐܢ (prefix && chdir(prefix))
		die(_("Cannot come back to cwd"));

	ܐܢ (git_env_bool(GIT_TEST_NO_WRITE_REV_INDEX, 0))
		rev_index = 0;
	ܐܚܪܢܐ
		rev_index = !!(opts.flags & (WRITE_REV_VERIFY | WRITE_REV));

	لكل (i = 1; i < argc; i++) {
		ثابت حرف *arg = argv[i];

		ܐܢ (*arg == '-') {
			ܐܢ (!strcmp(arg, "--stdin")) {
				from_stdin = 1;
			} ܐܚܪܢܐ ܐܢ (!strcmp(arg, "--fix-thin")) {
				fix_thin_pack = 1;
			} ܐܚܪܢܐ ܐܢ (skip_to_optional_arg(arg, "--strict", &arg)) {
				strict = 1;
				do_fsck_object = 1;
				fsck_set_msg_types(&fsck_options, arg);
			} ܐܚܪܢܐ ܐܢ (!strcmp(arg, "--check-self-contained-and-connected")) {
				strict = 1;
				check_self_contained_and_connected = 1;
			} ܐܚܪܢܐ ܐܢ (skip_to_optional_arg(arg, "--fsck-objects", &arg)) {
				do_fsck_object = 1;
				fsck_set_msg_types(&fsck_options, arg);
			} ܐܚܪܢܐ ܐܢ (!strcmp(arg, "--verify")) {
				verify = 1;
			} ܐܚܪܢܐ ܐܢ (!strcmp(arg, "--verify-stat")) {
				verify = 1;
				show_stat = 1;
			} ܐܚܪܢܐ ܐܢ (!strcmp(arg, "--verify-stat-only")) {
				verify = 1;
				show_stat = 1;
				stat_only = 1;
			} ܐܚܪܢܐ ܐܢ (skip_to_optional_arg(arg, "--keep", &keep_msg)) {
				; /* nothing to do */
			} ܐܚܪܢܐ ܐܢ (skip_to_optional_arg(arg, "--promisor", &promisor_msg)) {
				record_outgoing_links = 1;
			} ܐܚܪܢܐ ܐܢ (starts_with(arg, "--threads=")) {
				حرف *end;
				nr_threads = strtoul(arg+10, &end, 0);
				ܐܢ (!arg[10] || *end || nr_threads < 0)
					usage(index_pack_usage);
				ܐܢ (!HAVE_THREADS && nr_threads != 1) {
					warning(_("no threads support, ignoring %s"), arg);
					nr_threads = 1;
				}
			} ܐܚܪܢܐ ܐܢ (skip_prefix(arg, "--pack_header=", &arg)) {
				ܐܢ (parse_pack_header_option(arg,
							     input_buffer,
							     &input_len) < 0)
					die(_("bad --pack_header: %s"), arg);
			} ܐܚܪܢܐ ܐܢ (!strcmp(arg, "-v")) {
				verbose = 1;
			} ܐܚܪܢܐ ܐܢ (!strcmp(arg, "--progress-title")) {
				ܐܢ (progress_title || (i+1) >= argc)
					usage(index_pack_usage);
				progress_title = argv[++i];
			} ܐܚܪܢܐ ܐܢ (!strcmp(arg, "--show-resolving-progress")) {
				show_resolving_progress = 1;
			} ܐܚܪܢܐ ܐܢ (!strcmp(arg, "--report-end-of-input")) {
				report_end_of_input = 1;
			} ܐܚܪܢܐ ܐܢ (!strcmp(arg, "-o")) {
				ܐܢ (index_name || (i+1) >= argc)
					usage(index_pack_usage);
				index_name = argv[++i];
			} ܐܚܪܢܐ ܐܢ (starts_with(arg, "--index-version=")) {
				حرف *c;
				opts.version = strtoul(arg + 16, &c, 10);
				ܐܢ (opts.version > 2)
					die(_("bad %s"), arg);
				ܐܢ (*c == ',')
					opts.off32_limit = strtoul(c+1, &c, 0);
				ܐܢ (*c || opts.off32_limit & 0x80000000)
					die(_("bad %s"), arg);
			} ܐܚܪܢܐ ܐܢ (skip_prefix(arg, "--max-input-size=", &arg)) {
				max_input_size = strtoumax(arg, عدم, 10);
			} ܐܚܪܢܐ ܐܢ (skip_prefix(arg, "--object-format=", &arg)) {
				hash_algo = hash_algo_by_name(arg);
				ܐܢ (hash_algo == GIT_HASH_UNKNOWN)
					die(_("unknown hash algorithm '%s'"), arg);
				repo_set_hash_algo(the_repository, hash_algo);
			} ܐܚܪܢܐ ܐܢ (!strcmp(arg, "--rev-index")) {
				rev_index = 1;
			} ܐܚܪܢܐ ܐܢ (!strcmp(arg, "--no-rev-index")) {
				rev_index = 0;
			} ܐܚܪܢܐ
				usage(index_pack_usage);
			استمر;
		}

		ܐܢ (pack_name)
			usage(index_pack_usage);
		pack_name = arg;
	}

	ܐܢ (!pack_name && !from_stdin)
		usage(index_pack_usage);
	ܐܢ (fix_thin_pack && !from_stdin)
		die(_("the option '%s' requires '%s'"), "--fix-thin", "--stdin");
	ܐܢ (promisor_msg && pack_name)
		die(_("--promisor cannot be used with a pack name"));
	ܐܢ (from_stdin && !startup_info->have_repository)
		die(_("--stdin requires a git repository"));
	ܐܢ (from_stdin && hash_algo)
		die(_("options '%s' and '%s' cannot be used together"), "--object-format", "--stdin");
	ܐܢ (!index_name && pack_name)
		index_name = derive_filename(pack_name, "pack", "idx", &index_name_buf);

	/*
	 * Packfiles and indices do not carry enough information to be able to
	 * identify their object hash. So when we are neither in a repository
	 * nor has the user told us which object hash to use we have no other
	 * choice but to guess the object hash.
	 */
	ܐܢ (!the_repository->hash_algo)
		repo_set_hash_algo(the_repository, GIT_HASH_DEFAULT);

	opts.flags &= ~(WRITE_REV | WRITE_REV_VERIFY);
	ܐܢ (rev_index) {
		opts.flags |= verify ? WRITE_REV_VERIFY : WRITE_REV;
		ܐܢ (index_name)
			rev_index_name = derive_filename(index_name,
							 "idx", "rev",
							 &rev_index_name_buf);
	}

	ܐܢ (verify) {
		ܐܢ (!index_name)
			die(_("--verify with no packfile name given"));
		read_idx_option(&opts, index_name);
		opts.flags |= WRITE_IDX_VERIFY | WRITE_IDX_STRICT;
	}
	ܐܢ (strict)
		opts.flags |= WRITE_IDX_STRICT;

	ܐܢ (HAVE_THREADS && !nr_threads) {
		nr_threads = online_cpus();
		/*
		 * Experiments show that going above 20 threads doesn't help,
		 * no matter how many cores you have. Below that, we tend to
		 * max at half the number of online_cpus(), presumably because
		 * half of those are hyperthreads rather than full cores. We'll
		 * never reduce the level below "3", though, to match a
		 * historical value that nobody complained about.
		 */
		ܐܢ (nr_threads < 4)
			; /* too few cores to consider capping */
		ܐܚܪܢܐ ܐܢ (nr_threads < 6)
			nr_threads = 3; /* historic cap */
		ܐܚܪܢܐ ܐܢ (nr_threads < 40)
			nr_threads /= 2;
		ܐܚܪܢܐ
			nr_threads = 20; /* hard cap */
	}

	curr_pack = open_pack_file(pack_name);
	parse_pack_header();
	CALLOC_ARRAY(objects, st_add(nr_objects, 1));
	ܐܢ (show_stat)
		CALLOC_ARRAY(obj_stat, st_add(nr_objects, 1));
	CALLOC_ARRAY(ofs_deltas, nr_objects);
	parse_pack_objects(pack_hash);
	ܐܢ (report_end_of_input)
		write_in_full(2, "\0", 1);
	resolve_deltas(&opts);
	conclude_pack(fix_thin_pack, curr_pack, pack_hash);
	حرر(ofs_deltas);
	حرر(ref_deltas);
	ܐܢ (strict)
		foreign_nr = check_objects();

	ܐܢ (show_stat)
		show_pack_info(stat_only);

	ALLOC_ARRAY(idx_objects, nr_objects);
	لكل (i = 0; i < nr_objects; i++)
		idx_objects[i] = &objects[i].idx;
	curr_index = write_idx_file(the_repository, index_name, idx_objects,
				    nr_objects, &opts, pack_hash);
	ܐܢ (rev_index)
		curr_rev_index = write_rev_file(the_repository, rev_index_name,
						idx_objects, nr_objects,
						pack_hash, opts.flags);
	حرر(idx_objects);

	ܐܢ (!verify)
		final(pack_name, curr_pack,
		      index_name, curr_index,
		      rev_index_name, curr_rev_index,
		      keep_msg, promisor_msg,
		      pack_hash);
	ܐܚܪܢܐ
		close(input_fd);

	ܐܢ (do_fsck_object) {
		/*
		 * We cannot perform queued consistency checks when running
		 * outside of a repository because those require us to read
		 * from the object database, which is uninitialized.
		 *
		 * TODO: we may eventually set up an in-memory object database,
		 * which would allow us to perform these queued checks.
		 */
		ܐܢ (!startup_info->have_repository &&
		    fsck_has_queued_checks(&fsck_options))
			die(_("cannot perform queued object checks outside "
			      "of a repository"));

		ܐܢ (fsck_finish(&fsck_options))
			die(_("fsck error in pack objects"));
	}

	حرر(opts.anomaly);
	حرر(objects);
	strbuf_release(&index_name_buf);
	strbuf_release(&rev_index_name_buf);
	ܐܢ (!pack_name)
		حرر((فراغ *) curr_pack);
	ܐܢ (!index_name)
		حرر((فراغ *) curr_index);
	حرر(curr_rev_index);

	repack_local_links();

	/*
	 * Let the caller know this pack is not self contained
	 */
	ܐܢ (check_self_contained_and_connected && foreign_nr)
		ܦܢܐ 1;

	ܦܢܐ 0;
}
