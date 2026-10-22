#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ "delta.h"
#અન્તર્ભાવ "environment.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "hex.h"
#અન્તર્ભાવ "pack.h"
#અન્તર્ભાવ "csum-file.h"
#અન્તર્ભાવ "blob.h"
#અન્તર્ભાવ "commit.h"
#અન્તર્ભાવ "tag.h"
#અન્તર્ભાવ "tree.h"
#અન્તર્ભાવ "progress.h"
#અન્તર્ભાવ "fsck.h"
#અન્તર્ભાવ "strbuf.h"
#અન્તર્ભાવ "thread-utils.h"
#અન્તર્ભાવ "packfile.h"
#અન્તર્ભાવ "pack-revindex.h"
#અન્તર્ભાવ "object-file.h"
#અન્તર્ભાવ "odb.h"
#અન્તર્ભાવ "odb/streaming.h"
#અન્તર્ભાવ "oid-array.h"
#અન્તર્ભાવ "oidset.h"
#અન્તર્ભાવ "path.h"
#અન્તર્ભાવ "replace-object.h"
#અન્તર્ભાવ "tree-walk.h"
#અન્તર્ભાવ "promisor-remote.h"
#અન્તર્ભાવ "run-command.h"
#અન્તર્ભાવ "setup.h"
#અન્તર્ભાવ "strvec.h"

સ્થિર અચલ અક્ષર index_pack_usage[] =
"git index-pack [-v] [-o <index-file>] [--keep | --keep=<msg>] [--[no-]rev-index] [--verify] [--strict[=<msg-id>=<severity>...]] [--fsck-objects[=<msg-id>=<severity>...]] (<pack-file> | --stdin [--fix-thin] [<pack-file>])";

સંરચના object_entry {
	સંરચના pack_idx_entry idx;
	size_t size;
	unsigned અક્ષર hdr_size;
	signed અક્ષર type;
	signed અક્ષર real_type;
};

સંરચના object_stat {
	unsigned delta_depth;
	પૂર્ણાંક base_object_no;
};

સંરચના base_data {
	/* Initialized by make_base(). */
	સંરચના base_data *base;
	સંરચના object_entry *obj;
	પૂર્ણાંક ref_first, ref_last;
	પૂર્ણાંક ofs_first, ofs_last;
	/*
	 * Threads should increment retain_data if they are about to call
	 * patch_delta() using this struct's data as a base, and decrement this
	 * when they are done. While retain_data is nonzero, this struct's data
	 * will not be freed even if the delta base cache limit is exceeded.
	 */
	પૂર્ણાંક retain_data;
	/*
	 * The number of direct children that have not been fully processed
	 * (entered work_head, entered done_head, left done_head). When this
	 * number reaches zero, this struct base_data can be freed.
	 */
	પૂર્ણાંક children_remaining;

	/* Not initialized by make_base(). */
	સંરચના list_head list;
	શૂન્ય *data;
	size_t size;
};

/*
 * Stack of struct base_data that have unprocessed children.
 * threaded_second_pass() uses this as a source of work (the other being the
 * objects array).
 *
 * Guarded by work_mutex.
 */
સ્થિર LIST_HEAD(work_head);

/*
 * Stack of struct base_data that have children, all of whom have been
 * processed or are being processed, and at least one child is being processed.
 * These struct base_data must be kept around until the last child is
 * processed.
 *
 * Guarded by work_mutex.
 */
સ્થિર LIST_HEAD(done_head);

/*
 * All threads share one delta base cache.
 *
 * base_cache_used is guarded by work_mutex, and base_cache_limit is read-only
 * in a thread.
 */
સ્થિર size_t base_cache_used;
સ્થિર size_t base_cache_limit;

સંરચના thread_local_data {
	pthread_t thread;
	પૂર્ણાંક pack_fd;
};

/* Remember to update object flag allocation in object.h */
#define FLAG_LINK (1u<<20)
#define FLAG_CHECKED (1u<<21)

સંરચના ofs_delta_entry {
	off_t offset;
	પૂર્ણાંક obj_no;
};

સંરચના ref_delta_entry {
	સંરચના object_id oid;
	પૂર્ણાંક obj_no;
};

સ્થિર સંરચના object_entry *objects;
સ્થિર સંરચના object_stat *obj_stat;
સ્થિર સંરચના ofs_delta_entry *ofs_deltas;
સ્થિર સંરચના ref_delta_entry *ref_deltas;
સ્થિર સંરચના thread_local_data nothread_data;
સ્થિર પૂર્ણાંક nr_objects;
સ્થિર પૂર્ણાંક nr_ofs_deltas;
સ્થિર પૂર્ણાંક nr_ref_deltas;
સ્થિર પૂર્ણાંક ref_deltas_alloc;
સ્થિર પૂર્ણાંક nr_resolved_deltas;
સ્થિર પૂર્ણાંક nr_threads;

સ્થિર પૂર્ણાંક from_stdin;
સ્થિર પૂર્ણાંક strict;
સ્થિર પૂર્ણાંક do_fsck_object;
સ્થિર સંરચના fsck_options fsck_options;
સ્થિર પૂર્ણાંક verbose;
સ્થિર અચલ અક્ષર *progress_title;
સ્થિર પૂર્ણાંક show_resolving_progress;
સ્થિર પૂર્ણાંક show_stat;
સ્થિર પૂર્ણાંક check_self_contained_and_connected;

સ્થિર સંરચના progress *progress;

સ્થિર unsigned અક્ષર આગત_buffer[DEFAULT_IO_BUFFER_SIZE];
સ્થિર unsigned પૂર્ણાંક આગત_offset, આગત_len;
સ્થિર off_t consumed_bytes;
સ્થિર off_t max_આગત_size;
સ્થિર unsigned deepest_delta;
સ્થિર સંરચના git_hash_ctx આગત_ctx;
સ્થિર uint32_t આગત_crc32;
સ્થિર પૂર્ણાંક આગત_fd, output_fd;
સ્થિર અચલ અક્ષર *curr_pack;

/*
 * outgoing_links is guarded by read_mutex, and record_outgoing_links is
 * read-only in a thread.
 */
સ્થિર સંરચના oidset outgoing_links = OIDSET_INIT;
સ્થિર પૂર્ણાંક record_outgoing_links;

સ્થિર સંરચના thread_local_data *thread_data;
સ્થિર પૂર્ણાંક nr_dispatched;
સ્થિર પૂર્ણાંક threads_active;

સ્થિર pthread_mutex_t read_mutex;
#define read_lock()		lock_mutex(&read_mutex)
#define read_unlock()		unlock_mutex(&read_mutex)

સ્થિર pthread_mutex_t counter_mutex;
#define counter_lock()		lock_mutex(&counter_mutex)
#define counter_unlock()	unlock_mutex(&counter_mutex)

સ્થિર pthread_mutex_t work_mutex;
#define work_lock()		lock_mutex(&work_mutex)
#define work_unlock()		unlock_mutex(&work_mutex)

સ્થિર pthread_mutex_t deepest_delta_mutex;
#define deepest_delta_lock()	lock_mutex(&deepest_delta_mutex)
#define deepest_delta_unlock()	unlock_mutex(&deepest_delta_mutex)

સ્થિર pthread_ચાવી_t ચાવી;

સ્થિર અંતઃરેખ શૂન્ય lock_mutex(pthread_mutex_t *mutex)
{
	જો (threads_active)
		pthread_mutex_lock(mutex);
}

સ્થિર અંતઃરેખ શૂન્ય unlock_mutex(pthread_mutex_t *mutex)
{
	જો (threads_active)
		pthread_mutex_unlock(mutex);
}

/*
 * Mutex and conditional variable can't be statically-initialized on Windows.
 */
સ્થિર શૂન્ય init_thread(શૂન્ય)
{
	પૂર્ણાંક i;
	init_recursive_mutex(&read_mutex);
	pthread_mutex_init(&counter_mutex, ખાલી);
	pthread_mutex_init(&work_mutex, ખાલી);
	જો (show_stat)
		pthread_mutex_init(&deepest_delta_mutex, ખાલી);
	pthread_ચાવી_create(&ચાવી, ખાલી);
	CALLOC_ARRAY(thread_data, nr_threads);
	માટે (i = 0; i < nr_threads; i++) {
		thread_data[i].pack_fd = xopen(curr_pack, O_RDONLY);
	}

	threads_active = 1;
}

સ્થિર શૂન્ય cleanup_thread(શૂન્ય)
{
	પૂર્ણાંક i;
	જો (!threads_active)
		પાછા;
	threads_active = 0;
	pthread_mutex_destroy(&read_mutex);
	pthread_mutex_destroy(&counter_mutex);
	pthread_mutex_destroy(&work_mutex);
	જો (show_stat)
		pthread_mutex_destroy(&deepest_delta_mutex);
	માટે (i = 0; i < nr_threads; i++)
		close(thread_data[i].pack_fd);
	pthread_ચાવી_delete(ચાવી);
	મુક્ત(thread_data);
}

સ્થિર પૂર્ણાંક mark_link(સંરચના object *obj, ગણના object_type type,
		     શૂન્ય *data UNUSED,
		     સંરચના fsck_options *options UNUSED)
{
	જો (!obj)
		પાછા -1;

	જો (type != OBJ_ANY && obj->type != type)
		die(_("object type mismatch at %s"), oid_to_hex(&obj->oid));

	obj->flags |= FLAG_LINK;
	પાછા 0;
}

/* The content of each linked object must have been checked
   or it must be already present in the object database */
સ્થિર unsigned check_object(સંરચના object *obj)
{
	જો (!obj)
		પાછા 0;

	જો (!(obj->flags & FLAG_LINK))
		પાછા 0;

	જો (!(obj->flags & FLAG_CHECKED)) {
		size_t size;
		પૂર્ણાંક type = odb_read_object_info(the_repository->objects,
						&obj->oid, &size);
		જો (type <= 0)
			die(_("did not receive expected object %s"),
			      oid_to_hex(&obj->oid));
		જો (type != obj->type)
			die(_("object %s: expected type %s, found %s"),
			    oid_to_hex(&obj->oid),
			    type_name(obj->type), type_name(type));
		obj->flags |= FLAG_CHECKED;
		પાછા 1;
	}

	પાછા 0;
}

સ્થિર unsigned check_objects(શૂન્ય)
{
	unsigned i, max, foreign_nr = 0;

	max = get_max_object_index(the_repository);

	જો (verbose)
		progress = start_delayed_progress(the_repository,
						  _("Checking objects"), max);

	માટે (i = 0; i < max; i++) {
		foreign_nr += check_object(get_indexed_object(the_repository, i));
		display_progress(progress, i + 1);
	}

	stop_progress(&progress);
	પાછા foreign_nr;
}


/* Discard current buffer used content. */
સ્થિર શૂન્ય flush(શૂન્ય)
{
	જો (આગત_offset) {
		જો (output_fd >= 0)
			write_or_die(output_fd, આગત_buffer, આગત_offset);
		git_hash_update(&આગત_ctx, આગત_buffer, આગત_offset);
		memmove(આગત_buffer, આગત_buffer + આગત_offset, આગત_len);
		આગત_offset = 0;
	}
}

/*
 * Make sure at least "min" bytes are available in the buffer, and
 * return the pointer to the buffer.
 */
સ્થિર શૂન્ય *fill(પૂર્ણાંક min)
{
	જો (min <= આગત_len)
		પાછા આગત_buffer + આગત_offset;
	જો (min > કદ(આગત_buffer))
		die(Q_("cannot fill %d byte",
		       "cannot fill %d bytes",
		       min),
		    min);
	flush();
	કરો {
		ssize_t ret = xread(આગત_fd, આગત_buffer + આગત_len,
				કદ(આગત_buffer) - આગત_len);
		જો (ret <= 0) {
			જો (!ret)
				die(_("early EOF"));
			die_errno(_("read error on input"));
		}
		આગત_len += ret;
		જો (from_stdin)
			display_throughput(progress, consumed_bytes + આગત_len);
	} જ્યારે (આગત_len < min);
	પાછા આગત_buffer;
}

સ્થિર શૂન્ય use(પૂર્ણાંક bytes)
{
	જો (bytes > આગત_len)
		die(_("used more bytes than were available"));
	આગત_crc32 = crc32(આગત_crc32, આગત_buffer + આગત_offset, bytes);
	આગત_len -= bytes;
	આગત_offset += bytes;

	/* make sure off_t is sufficiently large not to wrap */
	જો (signed_add_overflows(consumed_bytes, bytes))
		die(_("pack too large for current definition of off_t"));
	consumed_bytes += bytes;
	જો (max_આગત_size && consumed_bytes > max_આગત_size) {
		સંરચના strbuf size_limit = STRBUF_INIT;
		strbuf_humanise_bytes(&size_limit, max_આગત_size);
		die(_("pack exceeds maximum allowed size (%s)"),
		    size_limit.buf);
	}
}

સ્થિર અચલ અક્ષર *open_pack_file(અચલ અક્ષર *pack_name)
{
	જો (from_stdin) {
		આગત_fd = 0;
		જો (!pack_name) {
			સંરચના strbuf tmp_file = STRBUF_INIT;
			output_fd = odb_mkstemp(the_repository->objects, &tmp_file,
						"pack/tmp_pack_XXXXXX");
			pack_name = strbuf_detach(&tmp_file, ખાલી);
		} નહીંતર {
			output_fd = xopen(pack_name, O_CREAT|O_EXCL|O_RDWR, 0600);
		}
		nothread_data.pack_fd = output_fd;
	} નહીંતર {
		આગત_fd = xopen(pack_name, O_RDONLY);
		output_fd = -1;
		nothread_data.pack_fd = આગત_fd;
	}
	git_hash_init(&આગત_ctx, the_hash_algo);
	પાછા pack_name;
}

સ્થિર શૂન્ય parse_pack_header(શૂન્ય)
{
	unsigned અક્ષર *hdr = fill(કદ(સંરચના pack_header));

	/* Header consistency check */
	જો (get_be32(hdr) != PACK_SIGNATURE)
		die(_("pack signature mismatch"));
	hdr += 4;
	જો (!pack_version_ok_native(get_be32(hdr)))
		die(_("pack version %"PRIu32" unsupported"),
		    get_be32(hdr));
	hdr += 4;

	nr_objects = get_be32(hdr);
	use(કદ(સંરચના pack_header));
}

__attribute__((format (printf, 2, 3)))
સ્થિર NORETURN શૂન્ય bad_object(off_t offset, અચલ અક્ષર *format, ...)
{
	va_list params;
	અક્ષર buf[1024];

	va_start(params, format);
	vsnprintf(buf, કદ(buf), format, params);
	va_end(params);
	die(_("pack has bad object at offset %"PRIuMAX": %s"),
	    (uintmax_t)offset, buf);
}

સ્થિર અંતઃરેખ સંરચના thread_local_data *get_thread_data(શૂન્ય)
{
	જો (HAVE_THREADS) {
		જો (threads_active)
			પાછા pthread_getspecific(ચાવી);
		પ્રતિજ્ઞા(!threads_active &&
		       "This should only be reached when all threads are gone");
	}
	પાછા &nothread_data;
}

સ્થિર શૂન્ય set_thread_data(સંરચના thread_local_data *data)
{
	જો (threads_active)
		pthread_setspecific(ચાવી, data);
}

સ્થિર શૂન્ય free_base_data(સંરચના base_data *c)
{
	જો (c->data) {
		FREE_AND_NULL(c->data);
		base_cache_used -= c->size;
	}
}

સ્થિર શૂન્ય prune_base_data(સંરચના base_data *retain)
{
	સંરચના list_head *pos;

	જો (base_cache_used <= base_cache_limit)
		પાછા;

	list_for_each_prev(pos, &done_head) {
		સંરચના base_data *b = list_entry(pos, સંરચના base_data, list);
		જો (b->retain_data || b == retain)
			ચાલુ;
		જો (b->data) {
			free_base_data(b);
			જો (base_cache_used <= base_cache_limit)
				પાછા;
		}
	}

	list_for_each_prev(pos, &work_head) {
		સંરચના base_data *b = list_entry(pos, સંરચના base_data, list);
		જો (b->retain_data || b == retain)
			ચાલુ;
		જો (b->data) {
			free_base_data(b);
			જો (base_cache_used <= base_cache_limit)
				પાછા;
		}
	}
}

સ્થિર પૂર્ણાંક is_delta_type(ગણના object_type type)
{
	પાછા (type == OBJ_REF_DELTA || type == OBJ_OFS_DELTA);
}

સ્થિર શૂન્ય *unpack_entry_data(off_t offset, size_t size,
			       ગણના object_type type, સંરચના object_id *oid)
{
	સ્થિર અક્ષર fixed_buf[8192];
	પૂર્ણાંક status;
	git_zstream stream;
	શૂન્ય *buf;
	સંરચના git_hash_ctx c;
	અક્ષર hdr[32];
	પૂર્ણાંક hdrlen;

	જો (!is_delta_type(type)) {
		hdrlen = format_object_header(hdr, કદ(hdr), type, size);
		git_hash_init(&c, the_hash_algo);
		git_hash_update(&c, hdr, hdrlen);
	} નહીંતર
		oid = ખાલી;
	જો (type == OBJ_BLOB &&
	    size > repo_settings_get_big_file_threshold(the_repository))
		buf = fixed_buf;
	નહીંતર
		buf = xmallocz(size);

	memset(&stream, 0, કદ(stream));
	git_inflate_init(&stream);
	stream.next_out = buf;
	stream.avail_out = buf == fixed_buf ? કદ(fixed_buf) : size;

	કરો {
		unsigned અક્ષર *last_out = stream.next_out;
		stream.next_in = fill(1);
		stream.avail_in = આગત_len;
		status = git_inflate(&stream, 0);
		use(આગત_len - stream.avail_in);
		જો (oid)
			git_hash_update(&c, last_out, stream.next_out - last_out);
		જો (buf == fixed_buf) {
			stream.next_out = buf;
			stream.avail_out = કદ(fixed_buf);
		}
	} જ્યારે (status == Z_OK);
	જો (stream.total_out != size || status != Z_STREAM_END)
		bad_object(offset, _("inflate returned %d"), status);
	git_inflate_end(&stream);
	જો (oid)
		git_hash_final_oid(oid, &c);
	પાછા buf == fixed_buf ? ખાલી : buf;
}

સ્થિર શૂન્ય *unpack_raw_entry(સંરચના object_entry *obj,
			      off_t *ofs_offset,
			      સંરચના object_id *ref_oid,
			      સંરચના object_id *oid)
{
	unsigned અક્ષર *p;
	size_t size, c;
	off_t base_offset;
	unsigned shift;
	શૂન્ય *data;

	obj->idx.offset = consumed_bytes;
	આગત_crc32 = crc32(0, ખાલી, 0);

	p = fill(1);
	c = *p;
	use(1);
	obj->type = (c >> 4) & 7;
	size = (c & 15);
	shift = 4;
	જ્યારે (c & 0x80) {
		જો ((bitsizeof(size_t) - 7) < shift)
			die(_("object size too large for this platform"));
		p = fill(1);
		c = *p;
		use(1);
		size += (c & 0x7f) << shift;
		shift += 7;
	}
	obj->size = size;

	પસંદગી (obj->type) {
	સ્થિતિ OBJ_REF_DELTA:
		oidread(ref_oid, fill(the_hash_algo->rawsz),
			the_repository->hash_algo);
		use(the_hash_algo->rawsz);
		તોડો;
	સ્થિતિ OBJ_OFS_DELTA:
		p = fill(1);
		c = *p;
		use(1);
		base_offset = c & 127;
		જ્યારે (c & 128) {
			base_offset += 1;
			જો (!base_offset || MSB(base_offset, 7))
				bad_object(obj->idx.offset, _("offset value overflow for delta base object"));
			p = fill(1);
			c = *p;
			use(1);
			base_offset = (base_offset << 7) + (c & 127);
		}
		*ofs_offset = obj->idx.offset - base_offset;
		જો (*ofs_offset <= 0 || *ofs_offset >= obj->idx.offset)
			bad_object(obj->idx.offset, _("delta base offset is out of bound"));
		તોડો;
	સ્થિતિ OBJ_COMMIT:
	સ્થિતિ OBJ_વૃક્ષ:
	સ્થિતિ OBJ_BLOB:
	સ્થિતિ OBJ_TAG:
		તોડો;
	મૂળભૂત:
		bad_object(obj->idx.offset, _("unknown object type %d"), obj->type);
	}
	obj->hdr_size = consumed_bytes - obj->idx.offset;

	data = unpack_entry_data(obj->idx.offset, obj->size, obj->type, oid);
	obj->idx.crc32 = આગત_crc32;
	પાછા data;
}

સ્થિર શૂન્ય *unpack_data(સંરચના object_entry *obj,
			 પૂર્ણાંક (*consume)(અચલ unsigned અક્ષર *, unsigned long, શૂન્ય *),
			 શૂન્ય *cb_data)
{
	off_t from = obj[0].idx.offset + obj[0].hdr_size;
	off_t len = obj[1].idx.offset - from;
	unsigned અક્ષર *data, *inbuf;
	git_zstream stream;
	પૂર્ણાંક status;

	data = xmallocz(consume ? 64*1024 : obj->size);
	inbuf = xmalloc((len < 64*1024) ? (પૂર્ણાંક)len : 64*1024);

	memset(&stream, 0, કદ(stream));
	git_inflate_init(&stream);
	stream.next_out = data;
	stream.avail_out = consume ? 64*1024 : obj->size;

	કરો {
		ssize_t n = (len < 64*1024) ? (ssize_t)len : 64*1024;
		n = xpread(get_thread_data()->pack_fd, inbuf, n, from);
		જો (n < 0)
			die_errno(_("cannot pread pack file"));
		જો (!n)
			die(Q_("premature end of pack file, %"PRIuMAX" byte missing",
			       "premature end of pack file, %"PRIuMAX" bytes missing",
			       len),
			    (uintmax_t)len);
		from += n;
		len -= n;
		stream.next_in = inbuf;
		stream.avail_in = n;
		જો (!consume)
			status = git_inflate(&stream, 0);
		નહીંતર {
			કરો {
				status = git_inflate(&stream, 0);
				જો (consume(data, stream.next_out - data, cb_data)) {
					મુક્ત(inbuf);
					મુક્ત(data);
					પાછા ખાલી;
				}
				stream.next_out = data;
				stream.avail_out = 64*1024;
			} જ્યારે (status == Z_OK && stream.avail_in);
		}
	} જ્યારે (len && status == Z_OK && !stream.avail_in);

	/* This has been inflated OK when first encountered, so... */
	જો (status != Z_STREAM_END || stream.total_out != obj->size)
		die(_("serious inflate inconsistency"));

	git_inflate_end(&stream);
	મુક્ત(inbuf);
	જો (consume) {
		FREE_AND_NULL(data);
	}
	પાછા data;
}

સ્થિર શૂન્ય *get_data_from_pack(સંરચના object_entry *obj)
{
	પાછા unpack_data(obj, ખાલી, ખાલી);
}

સ્થિર પૂર્ણાંક compare_ofs_delta_bases(off_t offset1, off_t offset2,
				   ગણના object_type type1,
				   ગણના object_type type2)
{
	પૂર્ણાંક cmp = type1 - type2;
	જો (cmp)
		પાછા cmp;
	પાછા offset1 < offset2 ? -1 :
	       offset1 > offset2 ?  1 :
	       0;
}

સ્થિર પૂર્ણાંક find_ofs_delta(અચલ off_t offset)
{
	પૂર્ણાંક first = 0, last = nr_ofs_deltas;

	જ્યારે (first < last) {
		પૂર્ણાંક next = first + (last - first) / 2;
		સંરચના ofs_delta_entry *delta = &ofs_deltas[next];
		પૂર્ણાંક cmp;

		cmp = compare_ofs_delta_bases(offset, delta->offset,
					      OBJ_OFS_DELTA,
					      objects[delta->obj_no].type);
		જો (!cmp)
			પાછા next;
		જો (cmp < 0) {
			last = next;
			ચાલુ;
		}
		first = next+1;
	}
	પાછા -first-1;
}

સ્થિર શૂન્ય find_ofs_delta_children(off_t offset,
				    પૂર્ણાંક *first_index, પૂર્ણાંક *last_index)
{
	પૂર્ણાંક first = find_ofs_delta(offset);
	પૂર્ણાંક last = first;
	પૂર્ણાંક end = nr_ofs_deltas - 1;

	જો (first < 0) {
		*first_index = 0;
		*last_index = -1;
		પાછા;
	}
	જ્યારે (first > 0 && ofs_deltas[first - 1].offset == offset)
		--first;
	જ્યારે (last < end && ofs_deltas[last + 1].offset == offset)
		++last;
	*first_index = first;
	*last_index = last;
}

સ્થિર પૂર્ણાંક compare_ref_delta_bases(અચલ સંરચના object_id *oid1,
				   અચલ સંરચના object_id *oid2,
				   ગણના object_type type1,
				   ગણના object_type type2)
{
	પૂર્ણાંક cmp = type1 - type2;
	જો (cmp)
		પાછા cmp;
	પાછા oidcmp(oid1, oid2);
}

સ્થિર પૂર્ણાંક find_ref_delta(અચલ સંરચના object_id *oid)
{
	પૂર્ણાંક first = 0, last = nr_ref_deltas;

	જ્યારે (first < last) {
		પૂર્ણાંક next = first + (last - first) / 2;
		સંરચના ref_delta_entry *delta = &ref_deltas[next];
		પૂર્ણાંક cmp;

		cmp = compare_ref_delta_bases(oid, &delta->oid,
					      OBJ_REF_DELTA,
					      objects[delta->obj_no].type);
		જો (!cmp)
			પાછા next;
		જો (cmp < 0) {
			last = next;
			ચાલુ;
		}
		first = next+1;
	}
	પાછા -first-1;
}

સ્થિર શૂન્ય find_ref_delta_children(અચલ સંરચના object_id *oid,
				    પૂર્ણાંક *first_index, પૂર્ણાંક *last_index)
{
	પૂર્ણાંક first = find_ref_delta(oid);
	પૂર્ણાંક last = first;
	પૂર્ણાંક end = nr_ref_deltas - 1;

	જો (first < 0) {
		*first_index = 0;
		*last_index = -1;
		પાછા;
	}
	જ્યારે (first > 0 && oideq(&ref_deltas[first - 1].oid, oid))
		--first;
	જ્યારે (last < end && oideq(&ref_deltas[last + 1].oid, oid))
		++last;
	*first_index = first;
	*last_index = last;
}

સંરચના compare_data {
	સંરચના object_entry *entry;
	સંરચના odb_stream *st;
	unsigned અક્ષર *buf;
	unsigned long buf_size;
};

સ્થિર પૂર્ણાંક compare_objects(અચલ unsigned અક્ષર *buf, unsigned long size,
			   શૂન્ય *cb_data)
{
	સંરચના compare_data *data = cb_data;

	જો (data->buf_size < size) {
		મુક્ત(data->buf);
		data->buf = xmalloc(size);
		data->buf_size = size;
	}

	જ્યારે (size) {
		ssize_t len = odb_stream_read(data->st, data->buf, size);
		જો (len == 0)
			die(_("SHA1 COLLISION FOUND WITH %s !"),
			    oid_to_hex(&data->entry->idx.oid));
		જો (len < 0)
			die(_("unable to read %s"),
			    oid_to_hex(&data->entry->idx.oid));
		જો (memcmp(buf, data->buf, len))
			die(_("SHA1 COLLISION FOUND WITH %s !"),
			    oid_to_hex(&data->entry->idx.oid));
		size -= len;
		buf += len;
	}
	પાછા 0;
}

સ્થિર પૂર્ણાંક check_collison(સંરચના object_entry *entry)
{
	સંરચના compare_data data;

	જો (entry->size <= repo_settings_get_big_file_threshold(the_repository) ||
	    entry->type != OBJ_BLOB)
		પાછા -1;

	memset(&data, 0, કદ(data));
	data.entry = entry;
	data.st = odb_stream_from_object(the_repository->objects, &entry->idx.oid, ખાલી);
	જો (!data.st)
		પાછા -1;
	જો (data.st->size != entry->size || data.st->type != entry->type)
		die(_("SHA1 COLLISION FOUND WITH %s !"),
		    oid_to_hex(&entry->idx.oid));
	unpack_data(entry, compare_objects, &data);
	odb_stream_close(data.st);
	મુક્ત(data.buf);
	પાછા 0;
}

સ્થિર શૂન્ય record_outgoing_link(અચલ સંરચના object_id *oid)
{
	oidset_ઉમેરો(&outgoing_links, oid);
}

સ્થિર શૂન્ય maybe_record_name_entry(અચલ સંરચના name_entry *entry)
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
	જો (S_ISDIR(entry->mode))
		record_outgoing_link(&entry->oid);
}

સ્થિર શૂન્ય do_record_outgoing_links(સંરચના object *obj)
{
	જો (obj->type == OBJ_વૃક્ષ) {
		સંરચના વૃક્ષ *વૃક્ષ = (સંરચના વૃક્ષ *)obj;
		સંરચના વૃક્ષ_desc desc;
		સંરચના name_entry entry;
		જો (init_વૃક્ષ_desc_gently(&desc, &વૃક્ષ->object.oid,
					  વૃક્ષ->buffer, વૃક્ષ->size, 0))
			/*
			 * Error messages are given when packs are
			 * verified, so do not print any here.
			 */
			પાછા;
		જ્યારે (વૃક્ષ_entry_gently(&desc, &entry))
			maybe_record_name_entry(&entry);
	} નહીંતર જો (obj->type == OBJ_COMMIT) {
		સંરચના commit *commit = (સંરચના commit *) obj;
		સંરચના commit_list *parents = commit->parents;

		record_outgoing_link(get_commit_વૃક્ષ_oid(commit));
		માટે (; parents; parents = parents->next)
			record_outgoing_link(&parents->item->object.oid);
	} નહીંતર જો (obj->type == OBJ_TAG) {
		સંરચના tag *tag = (સંરચના tag *) obj;
		record_outgoing_link(get_tagged_oid(tag));
	}
}

સ્થિર શૂન્ય sha1_object(અચલ શૂન્ય *data, સંરચના object_entry *obj_entry,
			unsigned long size, ગણના object_type type,
			અચલ સંરચના object_id *oid)
{
	શૂન્ય *new_data = ખાલી;
	પૂર્ણાંક collision_test_needed = 0;

	પ્રતિજ્ઞા(data || obj_entry);

	જો (startup_info->have_repository) {
		read_lock();
		collision_test_needed = odb_has_object(the_repository->objects, oid,
						       ODB_HAS_OBJECT_FETCH_PROMISOR);
		read_unlock();
	}

	જો (collision_test_needed && !data) {
		read_lock();
		જો (!check_collison(obj_entry))
			collision_test_needed = 0;
		read_unlock();
	}
	જો (collision_test_needed) {
		શૂન્ય *has_data;
		ગણના object_type has_type;
		size_t has_size;
		read_lock();
		has_type = odb_read_object_info(the_repository->objects, oid, &has_size);
		જો (has_type < 0)
			die(_("cannot read existing object info %s"), oid_to_hex(oid));
		જો (has_type != type || has_size != size)
			die(_("SHA1 COLLISION FOUND WITH %s !"), oid_to_hex(oid));
		has_data = odb_read_object(the_repository->objects, oid,
					   &has_type, &has_size);
		read_unlock();
		જો (!data)
			data = new_data = get_data_from_pack(obj_entry);
		જો (!has_data)
			die(_("cannot read existing object %s"), oid_to_hex(oid));
		જો (size != has_size || type != has_type ||
		    memcmp(data, has_data, size) != 0)
			die(_("SHA1 COLLISION FOUND WITH %s !"), oid_to_hex(oid));
		મુક્ત(has_data);
	}

	જો (strict || do_fsck_object || record_outgoing_links) {
		read_lock();
		જો (type == OBJ_BLOB) {
			સંરચના blob *blob = lookup_blob(the_repository, oid);
			જો (blob)
				blob->object.flags |= FLAG_CHECKED;
			નહીંતર
				die(_("invalid blob object %s"), oid_to_hex(oid));
			જો (do_fsck_object &&
			    fsck_object(&blob->object, (શૂન્ય *)data, size, &fsck_options))
				die(_("fsck error in packed object"));
		} નહીંતર {
			સંરચના object *obj;
			પૂર્ણાંક eaten;
			શૂન્ય *buf = (શૂન્ય *) data;

			પ્રતિજ્ઞા(data && "data can only be NULL for large _blobs_");

			/*
			 * we do not need to free the memory here, as the
			 * buf is deleted by the caller.
			 */
			obj = parse_object_buffer(the_repository, oid, type,
						  size, buf,
						  &eaten);
			જો (!obj)
				die(_("invalid %s"), type_name(type));
			જો (do_fsck_object &&
			    fsck_object(obj, buf, size, &fsck_options))
				die(_("fsck error in packed object"));
			જો (strict && fsck_walk(obj, ખાલી, &fsck_options))
				die(_("Not all child objects of %s are reachable"), oid_to_hex(&obj->oid));
			જો (record_outgoing_links)
				do_record_outgoing_links(obj);

			જો (obj->type == OBJ_વૃક્ષ) {
				સંરચના વૃક્ષ *item = (સંરચના વૃક્ષ *) obj;
				item->buffer = ખાલી;
				obj->parsed = 0;
			}
			જો (obj->type == OBJ_COMMIT) {
				સંરચના commit *commit = (સંરચના commit *) obj;
				જો (detach_commit_buffer(commit, ખાલી) != data)
					BUG("parse_object_buffer transmogrified our buffer");
			}
			obj->flags |= FLAG_CHECKED;
		}
		read_unlock();
	}

	મુક્ત(new_data);
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
સ્થિર શૂન્ય *get_base_data(સંરચના base_data *c)
{
	જો (!c->data) {
		સંરચના object_entry *obj = c->obj;
		સંરચના base_data **delta = ખાલી;
		પૂર્ણાંક delta_nr = 0, delta_alloc = 0;

		જ્યારે (is_delta_type(c->obj->type) && !c->data) {
			ALLOC_GROW(delta, delta_nr + 1, delta_alloc);
			delta[delta_nr++] = c;
			c = c->base;
		}
		જો (!delta_nr) {
			c->data = get_data_from_pack(obj);
			c->size = obj->size;
			base_cache_used += c->size;
			prune_base_data(c);
		}
		માટે (; delta_nr > 0; delta_nr--) {
			શૂન્ય *base, *raw;
			c = delta[delta_nr - 1];
			obj = c->obj;
			base = get_base_data(c->base);
			raw = get_data_from_pack(obj);
			c->data = patch_delta(
				base, c->base->size,
				raw, obj->size,
				&c->size);
			મુક્ત(raw);
			જો (!c->data)
				bad_object(obj->idx.offset, _("failed to apply delta"));
			base_cache_used += c->size;
			prune_base_data(c);
		}
		મુક્ત(delta);
	}
	પાછા c->data;
}

સ્થિર સંરચના base_data *make_base(સંરચના object_entry *obj,
				   સંરચના base_data *parent)
{
	સંરચના base_data *base = xcalloc(1, કદ(સંરચના base_data));
	base->base = parent;
	base->obj = obj;
	find_ref_delta_children(&obj->idx.oid,
				&base->ref_first, &base->ref_last);
	find_ofs_delta_children(obj->idx.offset,
				&base->ofs_first, &base->ofs_last);
	base->children_remaining = base->ref_last - base->ref_first +
		base->ofs_last - base->ofs_first + 2;
	પાછા base;
}

સ્થિર સંરચના base_data *resolve_delta(સંરચના object_entry *delta_obj,
				       સંરચના base_data *base)
{
	શૂન્ય *delta_data, *result_data;
	સંરચના base_data *result;
	size_t result_size;

	જો (show_stat) {
		પૂર્ણાંક i = delta_obj - objects;
		પૂર્ણાંક j = base->obj - objects;
		obj_stat[i].delta_depth = obj_stat[j].delta_depth + 1;
		deepest_delta_lock();
		જો (deepest_delta < obj_stat[i].delta_depth)
			deepest_delta = obj_stat[i].delta_depth;
		deepest_delta_unlock();
		obj_stat[i].base_object_no = j;
	}
	delta_data = get_data_from_pack(delta_obj);
	પ્રતિજ્ઞા(base->data);
	result_data = patch_delta(base->data, base->size,
				  delta_data, delta_obj->size, &result_size);
	મુક્ત(delta_data);
	જો (!result_data)
		bad_object(delta_obj->idx.offset, _("failed to apply delta"));
	hash_object_file(the_hash_algo, result_data, result_size,
			 delta_obj->real_type, &delta_obj->idx.oid);
	sha1_object(result_data, ખાલી, result_size, delta_obj->real_type,
		    &delta_obj->idx.oid);

	result = make_base(delta_obj, base);
	result->data = result_data;
	result->size = result_size;

	counter_lock();
	nr_resolved_deltas++;
	counter_unlock();

	પાછા result;
}

સ્થિર પૂર્ણાંક compare_ofs_delta_entry(અચલ શૂન્ય *a, અચલ શૂન્ય *b)
{
	અચલ સંરચના ofs_delta_entry *delta_a = a;
	અચલ સંરચના ofs_delta_entry *delta_b = b;

	પાછા delta_a->offset < delta_b->offset ? -1 :
	       delta_a->offset > delta_b->offset ?  1 :
	       0;
}

સ્થિર પૂર્ણાંક compare_ref_delta_entry(અચલ શૂન્ય *a, અચલ શૂન્ય *b)
{
	અચલ સંરચના ref_delta_entry *delta_a = a;
	અચલ સંરચના ref_delta_entry *delta_b = b;

	પાછા oidcmp(&delta_a->oid, &delta_b->oid);
}

સ્થિર શૂન્ય *threaded_second_pass(શૂન્ય *data)
{
	જો (data)
		set_thread_data(data);
	માટે (;;) {
		સંરચના base_data *parent = ખાલી;
		સંરચના object_entry *child_obj = ખાલી;
		સંરચના base_data *child = ખાલી;

		counter_lock();
		display_progress(progress, nr_resolved_deltas);
		counter_unlock();

		work_lock();
		જો (list_empty(&work_head)) {
			/*
			 * Take an object from the object array.
			 */
			જ્યારે (nr_dispatched < nr_objects &&
			       is_delta_type(objects[nr_dispatched].type))
				nr_dispatched++;
			જો (nr_dispatched >= nr_objects) {
				work_unlock();
				તોડો;
			}
			child_obj = &objects[nr_dispatched++];
		} નહીંતર {
			/*
			 * Peek at the top of the stack, and take a child from
			 * it.
			 */
			parent = list_first_entry(&work_head, સંરચના base_data,
						  list);

			જ્યારે (parent->ref_first <= parent->ref_last) {
				પૂર્ણાંક offset = ref_deltas[parent->ref_first++].obj_no;
				child_obj = objects + offset;
				જો (child_obj->real_type != OBJ_REF_DELTA) {
					child_obj = ખાલી;
					ચાલુ;
				}
				child_obj->real_type = parent->obj->real_type;
				તોડો;
			}

			જો (!child_obj && parent->ofs_first <= parent->ofs_last) {
				child_obj = objects +
					ofs_deltas[parent->ofs_first++].obj_no;
				પ્રતિજ્ઞા(child_obj->real_type == OBJ_OFS_DELTA);
				child_obj->real_type = parent->obj->real_type;
			}

			જો (parent->ref_first > parent->ref_last &&
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

		જો (child_obj) {
			જો (parent) {
				child = resolve_delta(child_obj, parent);
				જો (!child->children_remaining)
					FREE_AND_NULL(child->data);
			} નહીંતર{
				child = make_base(child_obj, ખાલી);
				જો (child->children_remaining) {
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
		જો (parent)
			parent->retain_data--;

		જો (child && child->data) {
			/*
			 * This child has its own children, so add it to
			 * work_head.
			 */
			list_add(&child->list, &work_head);
			base_cache_used += child->size;
			prune_base_data(ખાલી);
		} નહીંતર જો (child) {
			/*
			 * This child does not have its own children. It may be
			 * the last descendant of its ancestors; free those
			 * that we can.
			 */
			સંરચના base_data *p = parent;

			જ્યારે (p) {
				સંરચના base_data *next_p;

				p->children_remaining--;
				જો (p->children_remaining)
					તોડો;

				next_p = p->base;
				free_base_data(p);
				list_del(&p->list);
				મુક્ત(p);

				p = next_p;
			}
			FREE_AND_NULL(child);
		}
		work_unlock();
	}
	પાછા ખાલી;
}

/*
 * First pass:
 * - find locations of all objects;
 * - calculate SHA1 of all non-delta objects;
 * - remember base (SHA1 or offset) for all deltas.
 */
સ્થિર શૂન્ય parse_pack_objects(unsigned અક્ષર *hash)
{
	પૂર્ણાંક i, nr_delays = 0;
	સંરચના ofs_delta_entry *ofs_delta = ofs_deltas;
	સંરચના object_id ref_delta_oid;
	સંરચના stat st;
	સંરચના git_hash_ctx tmp_ctx;

	જો (verbose)
		progress = start_progress(
				the_repository,
				progress_title ? progress_title :
				from_stdin ? _("Receiving objects") : _("Indexing objects"),
				nr_objects);
	માટે (i = 0; i < nr_objects; i++) {
		સંરચના object_entry *obj = &objects[i];
		શૂન્ય *data = unpack_raw_entry(obj, &ofs_delta->offset,
					      &ref_delta_oid,
					      &obj->idx.oid);
		obj->real_type = obj->type;
		જો (obj->type == OBJ_OFS_DELTA) {
			nr_ofs_deltas++;
			ofs_delta->obj_no = i;
			ofs_delta++;
		} નહીંતર જો (obj->type == OBJ_REF_DELTA) {
			ALLOC_GROW(ref_deltas, nr_ref_deltas + 1, ref_deltas_alloc);
			oidcpy(&ref_deltas[nr_ref_deltas].oid, &ref_delta_oid);
			ref_deltas[nr_ref_deltas].obj_no = i;
			nr_ref_deltas++;
		} નહીંતર જો (!data) {
			/* large blobs, check later */
			obj->real_type = OBJ_BAD;
			nr_delays++;
		} નહીંતર
			sha1_object(data, ખાલી, obj->size, obj->type,
				    &obj->idx.oid);
		મુક્ત(data);
		display_progress(progress, i+1);
	}
	objects[i].idx.offset = consumed_bytes;
	stop_progress(&progress);

	/* Check pack integrity */
	flush();
	git_hash_init(&tmp_ctx, the_hash_algo);
	git_hash_clone(&tmp_ctx, &આગત_ctx);
	git_hash_final(hash, &tmp_ctx);
	જો (!hasheq(fill(the_hash_algo->rawsz), hash, the_repository->hash_algo))
		die(_("pack is corrupted (SHA1 mismatch)"));
	use(the_hash_algo->rawsz);

	/* If input_fd is a file, we should have reached its end now. */
	જો (fstat(આગત_fd, &st))
		die_errno(_("cannot fstat packfile"));
	જો (S_ISREG(st.st_mode) &&
			lseek(આગત_fd, 0, SEEK_CUR) - આગત_len != st.st_size)
		die(_("pack has junk at the end"));

	માટે (i = 0; i < nr_objects; i++) {
		સંરચના object_entry *obj = &objects[i];
		જો (obj->real_type != OBJ_BAD)
			ચાલુ;
		obj->real_type = obj->type;
		sha1_object(ખાલી, obj, obj->size, obj->type,
			    &obj->idx.oid);
		nr_delays--;
	}
	જો (nr_delays)
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
સ્થિર શૂન્ય resolve_deltas(સંરચના pack_idx_option *opts)
{
	પૂર્ણાંક i;

	જો (!nr_ofs_deltas && !nr_ref_deltas)
		પાછા;

	/* Sort deltas by base SHA1/offset for fast searching */
	QSORT(ofs_deltas, nr_ofs_deltas, compare_ofs_delta_entry);
	QSORT(ref_deltas, nr_ref_deltas, compare_ref_delta_entry);

	જો (verbose || show_resolving_progress)
		progress = start_progress(the_repository,
					  _("Resolving deltas"),
					  nr_ref_deltas + nr_ofs_deltas);

	nr_dispatched = 0;
	base_cache_limit = opts->delta_base_cache_limit * nr_threads;
	જો (nr_threads > 1 || getenv("GIT_FORCE_THREADS")) {
		init_thread();
		માટે (i = 0; i < nr_threads; i++) {
			પૂર્ણાંક ret = pthread_create(&thread_data[i].thread, ખાલી,
						 threaded_second_pass, thread_data + i);
			જો (ret)
				die(_("unable to create thread: %s"),
				    strerror(ret));
		}
		માટે (i = 0; i < nr_threads; i++)
			pthread_join(thread_data[i].thread, ખાલી);
		cleanup_thread();
		પાછા;
	}
	threaded_second_pass(&nothread_data);
}

/*
 * Third pass:
 * - append objects to convert thin pack to full pack if required
 * - write the final pack hash
 */
સ્થિર શૂન્ય fix_unresolved_deltas(સંરચના hashfile *f);
સ્થિર શૂન્ય conclude_pack(પૂર્ણાંક fix_thin_pack, અચલ અક્ષર *curr_pack, unsigned અક્ષર *pack_hash)
{
	જો (nr_ref_deltas + nr_ofs_deltas == nr_resolved_deltas) {
		stop_progress(&progress);
		/* Flush remaining pack final hash. */
		flush();
		પાછા;
	}

	જો (fix_thin_pack) {
		સંરચના hashfile *f;
		unsigned અક્ષર read_hash[GIT_MAX_RAWSZ], tail_hash[GIT_MAX_RAWSZ];
		સંરચના strbuf સંદેશ = STRBUF_INIT;
		પૂર્ણાંક nr_unresolved = nr_ofs_deltas + nr_ref_deltas - nr_resolved_deltas;
		પૂર્ણાંક nr_objects_initial = nr_objects;
		જો (nr_unresolved <= 0)
			die(_("confusion beyond insanity"));
		REALLOC_ARRAY(objects, nr_objects + nr_unresolved + 1);
		memset(objects + nr_objects + 1, 0,
		       nr_unresolved * કદ(*objects));
		f = hashfd(the_repository->hash_algo, output_fd, curr_pack);
		fix_unresolved_deltas(f);
		strbuf_addf(&સંદેશ, Q_("completed with %d local object",
				     "completed with %d local objects",
				     nr_objects - nr_objects_initial),
			    nr_objects - nr_objects_initial);
		stop_progress_સંદેશ(&progress, સંદેશ.buf);
		strbuf_release(&સંદેશ);
		finalize_hashfile(f, tail_hash, FSYNC_COMPONENT_PACK, 0);
		hashcpy(read_hash, pack_hash, the_repository->hash_algo);
		fixup_pack_header_footer(the_hash_algo, output_fd, pack_hash,
					 curr_pack, nr_objects,
					 read_hash, consumed_bytes-the_hash_algo->rawsz);
		જો (!hasheq(read_hash, tail_hash, the_repository->hash_algo))
			die(_("Unexpected tail checksum for %s "
			      "(disk corruption?)"), curr_pack);
	}
	જો (nr_ofs_deltas + nr_ref_deltas != nr_resolved_deltas)
		die(Q_("pack has %d unresolved delta",
		       "pack has %d unresolved deltas",
		       nr_ofs_deltas + nr_ref_deltas - nr_resolved_deltas),
		    nr_ofs_deltas + nr_ref_deltas - nr_resolved_deltas);
}

સ્થિર પૂર્ણાંક write_compressed(સંરચના hashfile *f, શૂન્ય *in, unsigned પૂર્ણાંક size)
{
	git_zstream stream;
	પૂર્ણાંક status;
	unsigned અક્ષર outbuf[4096];
	સંરચના repo_config_કિંમતો *cfg = repo_config_કિંમતો(the_repository);

	git_deflate_init(&stream, cfg->zlib_compression_level);
	stream.next_in = in;
	stream.avail_in = size;

	કરો {
		stream.next_out = outbuf;
		stream.avail_out = કદ(outbuf);
		status = git_deflate(&stream, Z_FINISH);
		hashwrite(f, outbuf, કદ(outbuf) - stream.avail_out);
	} જ્યારે (status == Z_OK);

	જો (status != Z_STREAM_END)
		die(_("unable to deflate appended object (%d)"), status);
	size = stream.total_out;
	git_deflate_end(&stream);
	પાછા size;
}

સ્થિર સંરચના object_entry *append_obj_to_pack(સંરચના hashfile *f,
			       અચલ unsigned અક્ષર *sha1, શૂન્ય *buf,
			       unsigned long size, ગણના object_type type)
{
	સંરચના object_entry *obj = &objects[nr_objects++];
	unsigned અક્ષર header[10];
	unsigned long s = size;
	પૂર્ણાંક n = 0;
	unsigned અક્ષર c = (type << 4) | (s & 15);
	s >>= 4;
	જ્યારે (s) {
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
	પાછા obj;
}

સ્થિર પૂર્ણાંક delta_pos_compare(અચલ શૂન્ય *_a, અચલ શૂન્ય *_b)
{
	સંરચના ref_delta_entry *a = *(સંરચના ref_delta_entry **)_a;
	સંરચના ref_delta_entry *b = *(સંરચના ref_delta_entry **)_b;
	પાછા a->obj_no - b->obj_no;
}

સ્થિર શૂન્ય fix_unresolved_deltas(સંરચના hashfile *f)
{
	સંરચના ref_delta_entry **sorted_by_pos;
	પૂર્ણાંક i;

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
	માટે (i = 0; i < nr_ref_deltas; i++)
		sorted_by_pos[i] = &ref_deltas[i];
	QSORT(sorted_by_pos, nr_ref_deltas, delta_pos_compare);

	જો (repo_has_promisor_remote(the_repository)) {
		/*
		 * Prefetch the delta bases.
		 */
		સંરચના oid_array to_fetch = OID_ARRAY_INIT;
		માટે (i = 0; i < nr_ref_deltas; i++) {
			સંરચના ref_delta_entry *d = sorted_by_pos[i];
			જો (!odb_read_object_info_extended(the_repository->objects,
							   &d->oid, ખાલી,
							   OBJECT_INFO_FOR_PREFETCH))
				ચાલુ;
			oid_array_append(&to_fetch, &d->oid);
		}
		promisor_remote_get_direct(the_repository,
					   to_fetch.oid, to_fetch.nr);
		oid_array_clear(&to_fetch);
	}

	માટે (i = 0; i < nr_ref_deltas; i++) {
		સંરચના ref_delta_entry *d = sorted_by_pos[i];
		ગણના object_type type;
		શૂન્ય *data;
		size_t size;

		જો (objects[d->obj_no].real_type != OBJ_REF_DELTA)
			ચાલુ;
		data = odb_read_object(the_repository->objects, &d->oid,
				       &type, &size);
		જો (!data)
			ચાલુ;

		જો (check_object_signature(the_repository, &d->oid, data, size,
					   type) < 0)
			die(_("local object %s is corrupt"), oid_to_hex(&d->oid));

		/*
		 * Add this as an object to the objects array and call
		 * threaded_second_pass() (which will pick up the added
		 * object).
		 */
		append_obj_to_pack(f, d->oid.hash, data, size, type);
		મુક્ત(data);
		threaded_second_pass(ખાલી);

		display_progress(progress, nr_resolved_deltas);
	}
	મુક્ત(sorted_by_pos);
}

સ્થિર અચલ અક્ષર *derive_filename(અચલ અક્ષર *pack_name, અચલ અક્ષર *strip,
				   અચલ અક્ષર *suffix, સંરચના strbuf *buf)
{
	size_t len;
	જો (!strip_suffix(pack_name, strip, &len) || !len ||
	    pack_name[len - 1] != '.')
		die(_("packfile name '%s' does not end with '.%s'"),
		    pack_name, strip);
	strbuf_add(buf, pack_name, len);
	strbuf_addstr(buf, suffix);
	પાછા buf->buf;
}

સ્થિર શૂન્ય write_special_file(અચલ અક્ષર *suffix, અચલ અક્ષર *સંદેશ,
			       અચલ અક્ષર *pack_name, અચલ unsigned અક્ષર *hash,
			       અચલ અક્ષર **report)
{
	સંરચના strbuf name_buf = STRBUF_INIT;
	અચલ અક્ષર *filename;
	પૂર્ણાંક fd;
	પૂર્ણાંક સંદેશ_len = strlen(સંદેશ);

	જો (pack_name)
		filename = derive_filename(pack_name, "pack", suffix, &name_buf);
	નહીંતર
		filename = odb_pack_name(the_repository, &name_buf, hash, suffix);

	fd = safe_create_file_with_leading_directories(the_repository, filename);
	જો (fd < 0) {
		જો (errno != EEXIST)
			die_errno(_("cannot write %s file '%s'"),
				  suffix, filename);
	} નહીંતર {
		જો (સંદેશ_len > 0) {
			write_or_die(fd, સંદેશ, સંદેશ_len);
			write_or_die(fd, "\n", 1);
		}
		જો (close(fd) != 0)
			die_errno(_("cannot close written %s file '%s'"),
				  suffix, filename);
		જો (report)
			*report = suffix;
	}
	strbuf_release(&name_buf);
}

સ્થિર શૂન્ય rename_tmp_packfile(અચલ અક્ષર **final_name,
				અચલ અક્ષર *curr_name,
				સંરચના strbuf *name, unsigned અક્ષર *hash,
				અચલ અક્ષર *ext, પૂર્ણાંક make_read_only_if_same)
{
	જો (!*final_name || strcmp(*final_name, curr_name)) {
		જો (!*final_name)
			*final_name = odb_pack_name(the_repository, name, hash, ext);
		જો (finalize_object_file(the_repository, curr_name, *final_name))
			die(_("unable to rename temporary '*.%s' file to '%s'"),
			    ext, *final_name);
	} નહીંતર જો (make_read_only_if_same) {
		chmod(*final_name, 0444);
	}
}

સ્થિર શૂન્ય final(અચલ અક્ષર *final_pack_name, અચલ અક્ષર *curr_pack_name,
		  અચલ અક્ષર *final_index_name, અચલ અક્ષર *curr_index_name,
		  અચલ અક્ષર *final_rev_index_name, અચલ અક્ષર *curr_rev_index_name,
		  અચલ અક્ષર *keep_સંદેશ, અચલ અક્ષર *promisor_સંદેશ,
		  unsigned અક્ષર *hash)
{
	અચલ અક્ષર *report = "pack";
	સંરચના strbuf pack_name = STRBUF_INIT;
	સંરચના strbuf index_name = STRBUF_INIT;
	સંરચના strbuf rev_index_name = STRBUF_INIT;

	જો (!from_stdin) {
		close(આગત_fd);
	} નહીંતર {
		fsync_component_or_die(FSYNC_COMPONENT_PACK, output_fd, curr_pack_name);
		જો (close(output_fd))
			die_errno(_("error while closing pack file"));
	}

	જો (keep_સંદેશ)
		write_special_file("keep", keep_સંદેશ, final_pack_name, hash,
				   &report);
	જો (promisor_સંદેશ)
		write_special_file("promisor", promisor_સંદેશ, final_pack_name,
				   hash, ખાલી);

	rename_tmp_packfile(&final_pack_name, curr_pack_name, &pack_name,
			    hash, "pack", from_stdin);
	જો (curr_rev_index_name)
		rename_tmp_packfile(&final_rev_index_name, curr_rev_index_name,
				    &rev_index_name, hash, "rev", 1);
	rename_tmp_packfile(&final_index_name, curr_index_name, &index_name,
			    hash, "idx", 1);

	જો (do_fsck_object && startup_info->have_repository) {
		સંરચના odb_source_files *files =
			odb_source_files_downcast(the_repository->objects->source);
		packfile_store_load_pack(files->dirs->packed, final_index_name, 0);
	}

	જો (!from_stdin) {
		printf("%s\n", hash_to_hex(hash));
	} નહીંતર {
		સંરચના strbuf buf = STRBUF_INIT;

		strbuf_addf(&buf, "%s\t%s\n", report, hash_to_hex(hash));
		write_or_die(1, buf.buf, buf.len);
		strbuf_release(&buf);

		/* Write the last part of the buffer to stdout */
		write_in_full(1, આગત_buffer + આગત_offset, આગત_len);
	}

	strbuf_release(&rev_index_name);
	strbuf_release(&index_name);
	strbuf_release(&pack_name);
}

સ્થિર પૂર્ણાંક git_index_pack_config(અચલ અક્ષર *k, અચલ અક્ષર *v,
				 અચલ સંરચના config_context *ctx, શૂન્ય *cb)
{
	સંરચના pack_idx_option *opts = cb;

	જો (!strcmp(k, "pack.indexversion")) {
		opts->version = git_config_int(k, v, ctx->kvi);
		જો (opts->version > 2)
			die(_("bad pack.indexVersion=%"PRIu32), opts->version);
		પાછા 0;
	}
	જો (!strcmp(k, "pack.threads")) {
		nr_threads = git_config_int(k, v, ctx->kvi);
		જો (nr_threads < 0)
			die(_("invalid number of threads specified (%d)"),
			    nr_threads);
		જો (!HAVE_THREADS && nr_threads != 1) {
			warning(_("no threads support, ignoring %s"), k);
			nr_threads = 1;
		}
		પાછા 0;
	}
	જો (!strcmp(k, "pack.writereverseindex")) {
		જો (git_config_bool(k, v))
			opts->flags |= WRITE_REV;
		નહીંતર
			opts->flags &= ~WRITE_REV;
	}
	જો (!strcmp(k, "core.deltabasecachelimit")) {
		opts->delta_base_cache_limit = git_config_ulong(k, v, ctx->kvi);
		પાછા 0;
	}
	પાછા git_default_config(k, v, ctx, cb);
}

સ્થિર પૂર્ણાંક cmp_uint32(અચલ શૂન્ય *a_, અચલ શૂન્ય *b_)
{
	uint32_t a = *((uint32_t *)a_);
	uint32_t b = *((uint32_t *)b_);

	પાછા (a < b) ? -1 : (a != b);
}

સ્થિર શૂન્ય read_v2_anomalous_offsets(સંરચના packed_git *p,
				      સંરચના pack_idx_option *opts)
{
	અચલ uint32_t *idx1, *idx2;
	uint32_t i;

	/* The address of the 4-byte offset table */
	idx1 = (((અચલ uint32_t *)((અચલ uint8_t *)p->index_data + p->crc_offset))
		+ (size_t)p->અંક_objects /* CRC32 table */
		);

	/* The address of the 8-byte offset table */
	idx2 = idx1 + p->અંક_objects;

	માટે (i = 0; i < p->અંક_objects; i++) {
		uint32_t off = ntohl(idx1[i]);
		જો (!(off & 0x80000000))
			ચાલુ;
		off = off & 0x7fffffff;
		check_pack_index_ptr(p, &idx2[off * 2]);
		જો (idx2[off * 2])
			ચાલુ;
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

સ્થિર શૂન્ય read_idx_option(સંરચના pack_idx_option *opts, અચલ અક્ષર *pack_name)
{
	સંરચના packed_git *p = add_packed_git(the_repository, pack_name,
					      strlen(pack_name), 1);

	જો (!p)
		die(_("Cannot open existing pack file '%s'"), pack_name);
	જો (open_pack_index(p))
		die(_("Cannot open existing pack idx file for '%s'"), pack_name);

	/* Read the attributes from the existing idx file */
	opts->version = p->index_version;

	જો (opts->version == 2)
		read_v2_anomalous_offsets(p, opts);

	/*
	 * Get rid of the idx file as we do not need it anymore.
	 * NEEDSWORK: extract this bit from free_pack_by_name() in
	 * object-file.c, perhaps?  It shouldn't matter very much as we
	 * know we haven't installed this pack (hence we never have
	 * read anything from it).
	 */
	close_pack_index(p);
	મુક્ત(p);
}

સ્થિર શૂન્ય show_pack_info(પૂર્ણાંક stat_only)
{
	પૂર્ણાંક i, baseobjects = nr_objects - nr_ref_deltas - nr_ofs_deltas;
	unsigned long *chain_histogram = ખાલી;

	જો (deepest_delta)
		CALLOC_ARRAY(chain_histogram, deepest_delta);

	માટે (i = 0; i < nr_objects; i++) {
		સંરચના object_entry *obj = &objects[i];

		જો (is_delta_type(obj->type))
			chain_histogram[obj_stat[i].delta_depth - 1]++;
		જો (stat_only)
			ચાલુ;
		printf("%s %-6s %"PRIuMAX" %"PRIuMAX" %"PRIuMAX,
		       oid_to_hex(&obj->idx.oid),
		       type_name(obj->real_type), (uintmax_t)obj->size,
		       (uintmax_t)(obj[1].idx.offset - obj->idx.offset),
		       (uintmax_t)obj->idx.offset);
		જો (is_delta_type(obj->type)) {
			સંરચના object_entry *bobj = &objects[obj_stat[i].base_object_no];
			printf(" %u %s", obj_stat[i].delta_depth,
			       oid_to_hex(&bobj->idx.oid));
		}
		putchar('\n');
	}

	જો (baseobjects)
		printf_ln(Q_("non delta: %d object",
			     "non delta: %d objects",
			     baseobjects),
			  baseobjects);
	માટે (i = 0; i < deepest_delta; i++) {
		જો (!chain_histogram[i])
			ચાલુ;
		printf_ln(Q_("chain length = %d: %lu object",
			     "chain length = %d: %lu objects",
			     chain_histogram[i]),
			  i + 1,
			  chain_histogram[i]);
	}
	મુક્ત(chain_histogram);
}

સ્થિર શૂન્ય repack_local_links(શૂન્ય)
{
	સંરચના child_process cmd = CHILD_PROCESS_INIT;
	FILE *out;
	સંરચના strbuf લીટી = STRBUF_INIT;
	સંરચના oidset_iter iter;
	સંરચના object_id *oid;
	અક્ષર *base_name = ખાલી;

	જો (!oidset_size(&outgoing_links))
		પાછા;

	oidset_iter_init(&outgoing_links, &iter);
	જ્યારે ((oid = oidset_iter_next(&iter))) {
		સંરચના odb_source_info source_info;
		સંરચના object_info info = {
			.source_infop = &source_info,
		};

		જો (odb_read_object_info_extended(the_repository->objects, oid, &info, 0))
			/* Missing; assume it is a promisor object */
			ચાલુ;
		જો (source_info.source->type == ODB_SOURCE_PACKED &&
		    source_info.u.packed.pack->pack_promisor)
			ચાલુ;

		જો (!cmd.args.nr) {
			base_name = mkpathdup(
				"%s/pack/pack",
				repo_get_object_directory(the_repository));
			strvec_ધકેલો(&cmd.args, "pack-objects");
			strvec_ધકેલો(&cmd.args,
				    "--exclude-promisor-objects-best-effort");
			strvec_ધકેલો(&cmd.args, base_name);
			cmd.git_cmd = 1;
			cmd.in = -1;
			cmd.out = -1;
			જો (start_command(&cmd))
				die(_("could not start pack-objects to repack local links"));
		}

		જો (write_in_full(cmd.in, oid_to_hex(oid), the_hash_algo->hexsz) < 0 ||
		    write_in_full(cmd.in, "\n", 1) < 0)
			die(_("failed to feed local object to pack-objects"));
	}

	જો (!cmd.args.nr)
		પાછા;

	close(cmd.in);

	out = xfdopen(cmd.out, "r");
	જ્યારે (strbuf_getline_lf(&લીટી, out) != EOF) {
		unsigned અક્ષર દ્વિઅંકી[GIT_MAX_RAWSZ];
		જો (લીટી.len != the_hash_algo->hexsz ||
		    !hex_to_bytes(દ્વિઅંકી, લીટી.buf, લીટી.len))
			die(_("index-pack: Expecting full hex object ID lines only from pack-objects."));

		/*
		 * pack-objects creates the .pack and .idx files, but not the
		 * .promisor file. Create the .promisor file, which is empty.
		 */
		write_special_file("promisor", "", ખાલી, દ્વિઅંકી, ખાલી);
	}

	fclose(out);
	જો (finish_command(&cmd))
		die(_("could not finish pack-objects to repack local links"));
	strbuf_release(&લીટી);
	મુક્ત(base_name);
}

પૂર્ણાંક cmd_index_pack(પૂર્ણાંક argc,
		   અચલ અક્ષર **argv,
		   અચલ અક્ષર *prefix,
		   સંરચના repository *repo)
{
	પૂર્ણાંક i, fix_thin_pack = 0, verify = 0, stat_only = 0, rev_index;
	અચલ અક્ષર *curr_index;
	અક્ષર *curr_rev_index = ખાલી;
	અચલ અક્ષર *index_name = ખાલી, *pack_name = ખાલી, *rev_index_name = ખાલી;
	અચલ અક્ષર *keep_સંદેશ = ખાલી;
	અચલ અક્ષર *promisor_સંદેશ = ખાલી;
	સંરચના strbuf index_name_buf = STRBUF_INIT;
	સંરચના strbuf rev_index_name_buf = STRBUF_INIT;
	સંરચના pack_idx_entry **idx_objects;
	સંરચના pack_idx_option opts;
	unsigned અક્ષર pack_hash[GIT_MAX_RAWSZ];
	unsigned foreign_nr = 1;	/* zero is a "good" value, assume bad */
	પૂર્ણાંક report_end_of_આગત = 0;
	પૂર્ણાંક hash_algo = 0;

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
	જો (prefix && chdir(prefix))
		die(_("Cannot come back to cwd"));

	જો (git_env_bool(GIT_TEST_NO_WRITE_REV_INDEX, 0))
		rev_index = 0;
	નહીંતર
		rev_index = !!(opts.flags & (WRITE_REV_VERIFY | WRITE_REV));

	માટે (i = 1; i < argc; i++) {
		અચલ અક્ષર *arg = argv[i];

		જો (*arg == '-') {
			જો (!strcmp(arg, "--stdin")) {
				from_stdin = 1;
			} નહીંતર જો (!strcmp(arg, "--fix-thin")) {
				fix_thin_pack = 1;
			} નહીંતર જો (skip_to_optional_arg(arg, "--strict", &arg)) {
				strict = 1;
				do_fsck_object = 1;
				fsck_set_સંદેશ_types(&fsck_options, arg);
			} નહીંતર જો (!strcmp(arg, "--check-self-contained-and-connected")) {
				strict = 1;
				check_self_contained_and_connected = 1;
			} નહીંતર જો (skip_to_optional_arg(arg, "--fsck-objects", &arg)) {
				do_fsck_object = 1;
				fsck_set_સંદેશ_types(&fsck_options, arg);
			} નહીંતર જો (!strcmp(arg, "--verify")) {
				verify = 1;
			} નહીંતર જો (!strcmp(arg, "--verify-stat")) {
				verify = 1;
				show_stat = 1;
			} નહીંતર જો (!strcmp(arg, "--verify-stat-only")) {
				verify = 1;
				show_stat = 1;
				stat_only = 1;
			} નહીંતર જો (skip_to_optional_arg(arg, "--keep", &keep_સંદેશ)) {
				; /* nothing to do */
			} નહીંતર જો (skip_to_optional_arg(arg, "--promisor", &promisor_સંદેશ)) {
				record_outgoing_links = 1;
			} નહીંતર જો (starts_with(arg, "--threads=")) {
				અક્ષર *end;
				nr_threads = strtoul(arg+10, &end, 0);
				જો (!arg[10] || *end || nr_threads < 0)
					usage(index_pack_usage);
				જો (!HAVE_THREADS && nr_threads != 1) {
					warning(_("no threads support, ignoring %s"), arg);
					nr_threads = 1;
				}
			} નહીંતર જો (skip_prefix(arg, "--pack_header=", &arg)) {
				જો (parse_pack_header_option(arg,
							     આગત_buffer,
							     &આગત_len) < 0)
					die(_("bad --pack_header: %s"), arg);
			} નહીંતર જો (!strcmp(arg, "-v")) {
				verbose = 1;
			} નહીંતર જો (!strcmp(arg, "--progress-title")) {
				જો (progress_title || (i+1) >= argc)
					usage(index_pack_usage);
				progress_title = argv[++i];
			} નહીંતર જો (!strcmp(arg, "--show-resolving-progress")) {
				show_resolving_progress = 1;
			} નહીંતર જો (!strcmp(arg, "--report-end-of-input")) {
				report_end_of_આગત = 1;
			} નહીંતર જો (!strcmp(arg, "-o")) {
				જો (index_name || (i+1) >= argc)
					usage(index_pack_usage);
				index_name = argv[++i];
			} નહીંતર જો (starts_with(arg, "--index-version=")) {
				અક્ષર *c;
				opts.version = strtoul(arg + 16, &c, 10);
				જો (opts.version > 2)
					die(_("bad %s"), arg);
				જો (*c == ',')
					opts.off32_limit = strtoul(c+1, &c, 0);
				જો (*c || opts.off32_limit & 0x80000000)
					die(_("bad %s"), arg);
			} નહીંતર જો (skip_prefix(arg, "--max-input-size=", &arg)) {
				max_આગત_size = strtoumax(arg, ખાલી, 10);
			} નહીંતર જો (skip_prefix(arg, "--object-format=", &arg)) {
				hash_algo = hash_algo_by_name(arg);
				જો (hash_algo == GIT_HASH_UNKNOWN)
					die(_("unknown hash algorithm '%s'"), arg);
				repo_set_hash_algo(the_repository, hash_algo);
			} નહીંતર જો (!strcmp(arg, "--rev-index")) {
				rev_index = 1;
			} નહીંતર જો (!strcmp(arg, "--no-rev-index")) {
				rev_index = 0;
			} નહીંતર
				usage(index_pack_usage);
			ચાલુ;
		}

		જો (pack_name)
			usage(index_pack_usage);
		pack_name = arg;
	}

	જો (!pack_name && !from_stdin)
		usage(index_pack_usage);
	જો (fix_thin_pack && !from_stdin)
		die(_("the option '%s' requires '%s'"), "--fix-thin", "--stdin");
	જો (promisor_સંદેશ && pack_name)
		die(_("--promisor cannot be used with a pack name"));
	જો (from_stdin && !startup_info->have_repository)
		die(_("--stdin requires a git repository"));
	જો (from_stdin && hash_algo)
		die(_("options '%s' and '%s' cannot be used together"), "--object-format", "--stdin");
	જો (!index_name && pack_name)
		index_name = derive_filename(pack_name, "pack", "idx", &index_name_buf);

	/*
	 * Packfiles and indices do not carry enough information to be able to
	 * identify their object hash. So when we are neither in a repository
	 * nor has the user told us which object hash to use we have no other
	 * choice but to guess the object hash.
	 */
	જો (!the_repository->hash_algo)
		repo_set_hash_algo(the_repository, GIT_HASH_DEFAULT);

	opts.flags &= ~(WRITE_REV | WRITE_REV_VERIFY);
	જો (rev_index) {
		opts.flags |= verify ? WRITE_REV_VERIFY : WRITE_REV;
		જો (index_name)
			rev_index_name = derive_filename(index_name,
							 "idx", "rev",
							 &rev_index_name_buf);
	}

	જો (verify) {
		જો (!index_name)
			die(_("--verify with no packfile name given"));
		read_idx_option(&opts, index_name);
		opts.flags |= WRITE_IDX_VERIFY | WRITE_IDX_STRICT;
	}
	જો (strict)
		opts.flags |= WRITE_IDX_STRICT;

	જો (HAVE_THREADS && !nr_threads) {
		nr_threads = online_cpus();
		/*
		 * Experiments show that going above 20 threads doesn't help,
		 * no matter how many cores you have. Below that, we tend to
		 * max at half the number of online_cpus(), presumably because
		 * half of those are hyperthreads rather than full cores. We'll
		 * never reduce the level below "3", though, to match a
		 * historical value that nobody complained about.
		 */
		જો (nr_threads < 4)
			; /* too few cores to consider capping */
		નહીંતર જો (nr_threads < 6)
			nr_threads = 3; /* historic cap */
		નહીંતર જો (nr_threads < 40)
			nr_threads /= 2;
		નહીંતર
			nr_threads = 20; /* hard cap */
	}

	curr_pack = open_pack_file(pack_name);
	parse_pack_header();
	CALLOC_ARRAY(objects, st_add(nr_objects, 1));
	જો (show_stat)
		CALLOC_ARRAY(obj_stat, st_add(nr_objects, 1));
	CALLOC_ARRAY(ofs_deltas, nr_objects);
	parse_pack_objects(pack_hash);
	જો (report_end_of_આગત)
		write_in_full(2, "\0", 1);
	resolve_deltas(&opts);
	conclude_pack(fix_thin_pack, curr_pack, pack_hash);
	મુક્ત(ofs_deltas);
	મુક્ત(ref_deltas);
	જો (strict)
		foreign_nr = check_objects();

	જો (show_stat)
		show_pack_info(stat_only);

	ALLOC_ARRAY(idx_objects, nr_objects);
	માટે (i = 0; i < nr_objects; i++)
		idx_objects[i] = &objects[i].idx;
	curr_index = write_idx_file(the_repository, index_name, idx_objects,
				    nr_objects, &opts, pack_hash);
	જો (rev_index)
		curr_rev_index = write_rev_file(the_repository, rev_index_name,
						idx_objects, nr_objects,
						pack_hash, opts.flags);
	મુક્ત(idx_objects);

	જો (!verify)
		final(pack_name, curr_pack,
		      index_name, curr_index,
		      rev_index_name, curr_rev_index,
		      keep_સંદેશ, promisor_સંદેશ,
		      pack_hash);
	નહીંતર
		close(આગત_fd);

	જો (do_fsck_object) {
		/*
		 * We cannot perform queued consistency checks when running
		 * outside of a repository because those require us to read
		 * from the object database, which is uninitialized.
		 *
		 * TODO: we may eventually set up an in-memory object database,
		 * which would allow us to perform these queued checks.
		 */
		જો (!startup_info->have_repository &&
		    fsck_has_queued_checks(&fsck_options))
			die(_("cannot perform queued object checks outside "
			      "of a repository"));

		જો (fsck_finish(&fsck_options))
			die(_("fsck error in pack objects"));
	}

	મુક્ત(opts.anomaly);
	મુક્ત(objects);
	strbuf_release(&index_name_buf);
	strbuf_release(&rev_index_name_buf);
	જો (!pack_name)
		મુક્ત((શૂન્ય *) curr_pack);
	જો (!index_name)
		મુક્ત((શૂન્ય *) curr_index);
	મુક્ત(curr_rev_index);

	repack_local_links();

	/*
	 * Let the caller know this pack is not self contained
	 */
	જો (check_self_contained_and_connected && foreign_nr)
		પાછા 1;

	પાછા 0;
}
