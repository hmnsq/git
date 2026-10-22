#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#अन्तर्भाव "builtin.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "delta.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "hex.h"
#अन्तर्भाव "pack.h"
#अन्तर्भाव "csum-file.h"
#अन्तर्भाव "blob.h"
#अन्तर्भाव "commit.h"
#अन्तर्भाव "tag.h"
#अन्तर्भाव "tree.h"
#अन्तर्भाव "progress.h"
#अन्तर्भाव "fsck.h"
#अन्तर्भाव "strbuf.h"
#अन्तर्भाव "thread-utils.h"
#अन्तर्भाव "packfile.h"
#अन्तर्भाव "pack-revindex.h"
#अन्तर्भाव "object-file.h"
#अन्तर्भाव "odb.h"
#अन्तर्भाव "odb/streaming.h"
#अन्तर्भाव "oid-array.h"
#अन्तर्भाव "oidset.h"
#अन्तर्भाव "path.h"
#अन्तर्भाव "replace-object.h"
#अन्तर्भाव "tree-walk.h"
#अन्तर्भाव "promisor-remote.h"
#अन्तर्भाव "run-command.h"
#अन्तर्भाव "setup.h"
#अन्तर्भाव "strvec.h"

स्थिर ध्रुव वर्ण index_pack_usage[] =
"git index-pack [-v] [-o <index-file>] [--keep | --keep=<msg>] [--[no-]rev-index] [--verify] [--strict[=<msg-id>=<severity>...]] [--fsck-objects[=<msg-id>=<severity>...]] (<pack-file> | --stdin [--fix-thin] [<pack-file>])";

संरचना object_entry {
	संरचना pack_idx_entry idx;
	size_t size;
	unsigned वर्ण hdr_size;
	signed वर्ण type;
	signed वर्ण real_type;
};

संरचना object_stat {
	unsigned delta_depth;
	पूर्णाङ्क base_object_no;
};

संरचना base_data {
	/* Initialized by make_base(). */
	संरचना base_data *base;
	संरचना object_entry *obj;
	पूर्णाङ्क ref_first, ref_last;
	पूर्णाङ्क ofs_first, ofs_last;
	/*
	 * Threads should increment retain_data if they are about to call
	 * patch_delta() using this struct's data as a base, and decrement this
	 * when they are done. While retain_data is nonzero, this struct's data
	 * will not be freed even if the delta base cache limit is exceeded.
	 */
	पूर्णाङ्क retain_data;
	/*
	 * The number of direct children that have not been fully processed
	 * (entered work_head, entered done_head, left done_head). When this
	 * number reaches zero, this struct base_data can be freed.
	 */
	पूर्णाङ्क children_remaining;

	/* Not initialized by make_base(). */
	संरचना list_head list;
	शून्य *data;
	size_t size;
};

/*
 * Stack of struct base_data that have unprocessed children.
 * threaded_second_pass() uses this as a source of work (the other being the
 * objects array).
 *
 * Guarded by work_mutex.
 */
स्थिर LIST_HEAD(work_head);

/*
 * Stack of struct base_data that have children, all of whom have been
 * processed or are being processed, and at least one child is being processed.
 * These struct base_data must be kept around until the last child is
 * processed.
 *
 * Guarded by work_mutex.
 */
स्थिर LIST_HEAD(done_head);

/*
 * All threads share one delta base cache.
 *
 * base_cache_used is guarded by work_mutex, and base_cache_limit is read-only
 * in a thread.
 */
स्थिर size_t base_cache_used;
स्थिर size_t base_cache_limit;

संरचना thread_local_data {
	pthread_t thread;
	पूर्णाङ्क pack_fd;
};

/* Remember to update object flag allocation in object.h */
#define FLAG_LINK (1u<<20)
#define FLAG_CHECKED (1u<<21)

संरचना ofs_delta_entry {
	off_t offset;
	पूर्णाङ्क obj_no;
};

संरचना ref_delta_entry {
	संरचना object_id oid;
	पूर्णाङ्क obj_no;
};

स्थिर संरचना object_entry *objects;
स्थिर संरचना object_stat *obj_stat;
स्थिर संरचना ofs_delta_entry *ofs_deltas;
स्थिर संरचना ref_delta_entry *ref_deltas;
स्थिर संरचना thread_local_data nothread_data;
स्थिर पूर्णाङ्क nr_objects;
स्थिर पूर्णाङ्क nr_ofs_deltas;
स्थिर पूर्णाङ्क nr_ref_deltas;
स्थिर पूर्णाङ्क ref_deltas_alloc;
स्थिर पूर्णाङ्क nr_resolved_deltas;
स्थिर पूर्णाङ्क nr_threads;

स्थिर पूर्णाङ्क from_stdin;
स्थिर पूर्णाङ्क strict;
स्थिर पूर्णाङ्क do_fsck_object;
स्थिर संरचना fsck_options fsck_options;
स्थिर पूर्णाङ्क verbose;
स्थिर ध्रुव वर्ण *progress_title;
स्थिर पूर्णाङ्क show_resolving_progress;
स्थिर पूर्णाङ्क show_stat;
स्थिर पूर्णाङ्क check_self_contained_and_connected;

स्थिर संरचना progress *progress;

स्थिर unsigned वर्ण निवेश_buffer[DEFAULT_IO_BUFFER_SIZE];
स्थिर unsigned पूर्णाङ्क निवेश_offset, निवेश_len;
स्थिर off_t consumed_bytes;
स्थिर off_t max_निवेश_size;
स्थिर unsigned deepest_delta;
स्थिर संरचना git_hash_ctx निवेश_ctx;
स्थिर uint32_t निवेश_crc32;
स्थिर पूर्णाङ्क निवेश_fd, output_fd;
स्थिर ध्रुव वर्ण *curr_pack;

/*
 * outgoing_links is guarded by read_mutex, and record_outgoing_links is
 * read-only in a thread.
 */
स्थिर संरचना oidset outgoing_links = OIDSET_INIT;
स्थिर पूर्णाङ्क record_outgoing_links;

स्थिर संरचना thread_local_data *thread_data;
स्थिर पूर्णाङ्क nr_dispatched;
स्थिर पूर्णाङ्क threads_active;

स्थिर pthread_mutex_t read_mutex;
#define read_lock()		lock_mutex(&read_mutex)
#define read_unlock()		unlock_mutex(&read_mutex)

स्थिर pthread_mutex_t counter_mutex;
#define counter_lock()		lock_mutex(&counter_mutex)
#define counter_unlock()	unlock_mutex(&counter_mutex)

स्थिर pthread_mutex_t work_mutex;
#define work_lock()		lock_mutex(&work_mutex)
#define work_unlock()		unlock_mutex(&work_mutex)

स्थिर pthread_mutex_t deepest_delta_mutex;
#define deepest_delta_lock()	lock_mutex(&deepest_delta_mutex)
#define deepest_delta_unlock()	unlock_mutex(&deepest_delta_mutex)

स्थिर pthread_कुंजी_t कुंजी;

स्थिर अन्तःस्थ शून्य lock_mutex(pthread_mutex_t *mutex)
{
	यदि (threads_active)
		pthread_mutex_lock(mutex);
}

स्थिर अन्तःस्थ शून्य unlock_mutex(pthread_mutex_t *mutex)
{
	यदि (threads_active)
		pthread_mutex_unlock(mutex);
}

/*
 * Mutex and conditional variable can't be statically-initialized on Windows.
 */
स्थिर शून्य init_thread(शून्य)
{
	पूर्णाङ्क i;
	init_recursive_mutex(&read_mutex);
	pthread_mutex_init(&counter_mutex, रिक्त);
	pthread_mutex_init(&work_mutex, रिक्त);
	यदि (show_stat)
		pthread_mutex_init(&deepest_delta_mutex, रिक्त);
	pthread_कुंजी_create(&कुंजी, रिक्त);
	CALLOC_ARRAY(thread_data, nr_threads);
	प्रति (i = 0; i < nr_threads; i++) {
		thread_data[i].pack_fd = xopen(curr_pack, O_RDONLY);
	}

	threads_active = 1;
}

स्थिर शून्य cleanup_thread(शून्य)
{
	पूर्णाङ्क i;
	यदि (!threads_active)
		प्रत्यागम;
	threads_active = 0;
	pthread_mutex_destroy(&read_mutex);
	pthread_mutex_destroy(&counter_mutex);
	pthread_mutex_destroy(&work_mutex);
	यदि (show_stat)
		pthread_mutex_destroy(&deepest_delta_mutex);
	प्रति (i = 0; i < nr_threads; i++)
		close(thread_data[i].pack_fd);
	pthread_कुंजी_delete(कुंजी);
	मुक्त(thread_data);
}

स्थिर पूर्णाङ्क mark_link(संरचना object *obj, गणना object_type type,
		     शून्य *data UNUSED,
		     संरचना fsck_options *options UNUSED)
{
	यदि (!obj)
		प्रत्यागम -1;

	यदि (type != OBJ_ANY && obj->type != type)
		die(_("object type mismatch at %s"), oid_to_hex(&obj->oid));

	obj->flags |= FLAG_LINK;
	प्रत्यागम 0;
}

/* The content of each linked object must have been checked
   or it must be already present in the object database */
स्थिर unsigned check_object(संरचना object *obj)
{
	यदि (!obj)
		प्रत्यागम 0;

	यदि (!(obj->flags & FLAG_LINK))
		प्रत्यागम 0;

	यदि (!(obj->flags & FLAG_CHECKED)) {
		size_t size;
		पूर्णाङ्क type = odb_read_object_info(the_repository->objects,
						&obj->oid, &size);
		यदि (type <= 0)
			die(_("did not receive expected object %s"),
			      oid_to_hex(&obj->oid));
		यदि (type != obj->type)
			die(_("object %s: expected type %s, found %s"),
			    oid_to_hex(&obj->oid),
			    type_name(obj->type), type_name(type));
		obj->flags |= FLAG_CHECKED;
		प्रत्यागम 1;
	}

	प्रत्यागम 0;
}

स्थिर unsigned check_objects(शून्य)
{
	unsigned i, max, foreign_nr = 0;

	max = get_max_object_index(the_repository);

	यदि (verbose)
		progress = start_delayed_progress(the_repository,
						  _("Checking objects"), max);

	प्रति (i = 0; i < max; i++) {
		foreign_nr += check_object(get_indexed_object(the_repository, i));
		display_progress(progress, i + 1);
	}

	stop_progress(&progress);
	प्रत्यागम foreign_nr;
}


/* Discard current buffer used content. */
स्थिर शून्य flush(शून्य)
{
	यदि (निवेश_offset) {
		यदि (output_fd >= 0)
			write_or_die(output_fd, निवेश_buffer, निवेश_offset);
		git_hash_update(&निवेश_ctx, निवेश_buffer, निवेश_offset);
		memmove(निवेश_buffer, निवेश_buffer + निवेश_offset, निवेश_len);
		निवेश_offset = 0;
	}
}

/*
 * Make sure at least "min" bytes are available in the buffer, and
 * return the pointer to the buffer.
 */
स्थिर शून्य *fill(पूर्णाङ्क min)
{
	यदि (min <= निवेश_len)
		प्रत्यागम निवेश_buffer + निवेश_offset;
	यदि (min > परिमाण(निवेश_buffer))
		die(Q_("cannot fill %d byte",
		       "cannot fill %d bytes",
		       min),
		    min);
	flush();
	कुरु {
		ssize_t ret = xread(निवेश_fd, निवेश_buffer + निवेश_len,
				परिमाण(निवेश_buffer) - निवेश_len);
		यदि (ret <= 0) {
			यदि (!ret)
				die(_("early EOF"));
			die_errno(_("read error on input"));
		}
		निवेश_len += ret;
		यदि (from_stdin)
			display_throughput(progress, consumed_bytes + निवेश_len);
	} यावत् (निवेश_len < min);
	प्रत्यागम निवेश_buffer;
}

स्थिर शून्य use(पूर्णाङ्क bytes)
{
	यदि (bytes > निवेश_len)
		die(_("used more bytes than were available"));
	निवेश_crc32 = crc32(निवेश_crc32, निवेश_buffer + निवेश_offset, bytes);
	निवेश_len -= bytes;
	निवेश_offset += bytes;

	/* make sure off_t is sufficiently large not to wrap */
	यदि (signed_add_overflows(consumed_bytes, bytes))
		die(_("pack too large for current definition of off_t"));
	consumed_bytes += bytes;
	यदि (max_निवेश_size && consumed_bytes > max_निवेश_size) {
		संरचना strbuf size_limit = STRBUF_INIT;
		strbuf_humanise_bytes(&size_limit, max_निवेश_size);
		die(_("pack exceeds maximum allowed size (%s)"),
		    size_limit.buf);
	}
}

स्थिर ध्रुव वर्ण *open_pack_file(ध्रुव वर्ण *pack_name)
{
	यदि (from_stdin) {
		निवेश_fd = 0;
		यदि (!pack_name) {
			संरचना strbuf tmp_file = STRBUF_INIT;
			output_fd = odb_mkstemp(the_repository->objects, &tmp_file,
						"pack/tmp_pack_XXXXXX");
			pack_name = strbuf_detach(&tmp_file, रिक्त);
		} अन्यथा {
			output_fd = xopen(pack_name, O_CREAT|O_EXCL|O_RDWR, 0600);
		}
		nothread_data.pack_fd = output_fd;
	} अन्यथा {
		निवेश_fd = xopen(pack_name, O_RDONLY);
		output_fd = -1;
		nothread_data.pack_fd = निवेश_fd;
	}
	git_hash_init(&निवेश_ctx, the_hash_algo);
	प्रत्यागम pack_name;
}

स्थिर शून्य parse_pack_header(शून्य)
{
	unsigned वर्ण *hdr = fill(परिमाण(संरचना pack_header));

	/* Header consistency check */
	यदि (get_be32(hdr) != PACK_SIGNATURE)
		die(_("pack signature mismatch"));
	hdr += 4;
	यदि (!pack_version_ok_native(get_be32(hdr)))
		die(_("pack version %"PRIu32" unsupported"),
		    get_be32(hdr));
	hdr += 4;

	nr_objects = get_be32(hdr);
	use(परिमाण(संरचना pack_header));
}

__attribute__((format (printf, 2, 3)))
स्थिर NORETURN शून्य bad_object(off_t offset, ध्रुव वर्ण *format, ...)
{
	va_list params;
	वर्ण buf[1024];

	va_start(params, format);
	vsnprintf(buf, परिमाण(buf), format, params);
	va_end(params);
	die(_("pack has bad object at offset %"PRIuMAX": %s"),
	    (uintmax_t)offset, buf);
}

स्थिर अन्तःस्थ संरचना thread_local_data *get_thread_data(शून्य)
{
	यदि (HAVE_THREADS) {
		यदि (threads_active)
			प्रत्यागम pthread_getspecific(कुंजी);
		प्रतिज्ञा(!threads_active &&
		       "This should only be reached when all threads are gone");
	}
	प्रत्यागम &nothread_data;
}

स्थिर शून्य set_thread_data(संरचना thread_local_data *data)
{
	यदि (threads_active)
		pthread_setspecific(कुंजी, data);
}

स्थिर शून्य free_base_data(संरचना base_data *c)
{
	यदि (c->data) {
		FREE_AND_NULL(c->data);
		base_cache_used -= c->size;
	}
}

स्थिर शून्य prune_base_data(संरचना base_data *retain)
{
	संरचना list_head *pos;

	यदि (base_cache_used <= base_cache_limit)
		प्रत्यागम;

	list_for_each_prev(pos, &done_head) {
		संरचना base_data *b = list_entry(pos, संरचना base_data, list);
		यदि (b->retain_data || b == retain)
			अनुवर्त;
		यदि (b->data) {
			free_base_data(b);
			यदि (base_cache_used <= base_cache_limit)
				प्रत्यागम;
		}
	}

	list_for_each_prev(pos, &work_head) {
		संरचना base_data *b = list_entry(pos, संरचना base_data, list);
		यदि (b->retain_data || b == retain)
			अनुवर्त;
		यदि (b->data) {
			free_base_data(b);
			यदि (base_cache_used <= base_cache_limit)
				प्रत्यागम;
		}
	}
}

स्थिर पूर्णाङ्क is_delta_type(गणना object_type type)
{
	प्रत्यागम (type == OBJ_REF_DELTA || type == OBJ_OFS_DELTA);
}

स्थिर शून्य *unpack_entry_data(off_t offset, size_t size,
			       गणना object_type type, संरचना object_id *oid)
{
	स्थिर वर्ण fixed_buf[8192];
	पूर्णाङ्क status;
	git_zstream stream;
	शून्य *buf;
	संरचना git_hash_ctx c;
	वर्ण hdr[32];
	पूर्णाङ्क hdrlen;

	यदि (!is_delta_type(type)) {
		hdrlen = format_object_header(hdr, परिमाण(hdr), type, size);
		git_hash_init(&c, the_hash_algo);
		git_hash_update(&c, hdr, hdrlen);
	} अन्यथा
		oid = रिक्त;
	यदि (type == OBJ_BLOB &&
	    size > repo_settings_get_big_file_threshold(the_repository))
		buf = fixed_buf;
	अन्यथा
		buf = xmallocz(size);

	memset(&stream, 0, परिमाण(stream));
	git_inflate_init(&stream);
	stream.next_out = buf;
	stream.avail_out = buf == fixed_buf ? परिमाण(fixed_buf) : size;

	कुरु {
		unsigned वर्ण *last_out = stream.next_out;
		stream.next_in = fill(1);
		stream.avail_in = निवेश_len;
		status = git_inflate(&stream, 0);
		use(निवेश_len - stream.avail_in);
		यदि (oid)
			git_hash_update(&c, last_out, stream.next_out - last_out);
		यदि (buf == fixed_buf) {
			stream.next_out = buf;
			stream.avail_out = परिमाण(fixed_buf);
		}
	} यावत् (status == Z_OK);
	यदि (stream.total_out != size || status != Z_STREAM_END)
		bad_object(offset, _("inflate returned %d"), status);
	git_inflate_end(&stream);
	यदि (oid)
		git_hash_final_oid(oid, &c);
	प्रत्यागम buf == fixed_buf ? रिक्त : buf;
}

स्थिर शून्य *unpack_raw_entry(संरचना object_entry *obj,
			      off_t *ofs_offset,
			      संरचना object_id *ref_oid,
			      संरचना object_id *oid)
{
	unsigned वर्ण *p;
	size_t size, c;
	off_t base_offset;
	unsigned shift;
	शून्य *data;

	obj->idx.offset = consumed_bytes;
	निवेश_crc32 = crc32(0, रिक्त, 0);

	p = fill(1);
	c = *p;
	use(1);
	obj->type = (c >> 4) & 7;
	size = (c & 15);
	shift = 4;
	यावत् (c & 0x80) {
		यदि ((bitsizeof(size_t) - 7) < shift)
			die(_("object size too large for this platform"));
		p = fill(1);
		c = *p;
		use(1);
		size += (c & 0x7f) << shift;
		shift += 7;
	}
	obj->size = size;

	विकल्प (obj->type) {
	पक्ष OBJ_REF_DELTA:
		oidread(ref_oid, fill(the_hash_algo->rawsz),
			the_repository->hash_algo);
		use(the_hash_algo->rawsz);
		भङ्ग;
	पक्ष OBJ_OFS_DELTA:
		p = fill(1);
		c = *p;
		use(1);
		base_offset = c & 127;
		यावत् (c & 128) {
			base_offset += 1;
			यदि (!base_offset || MSB(base_offset, 7))
				bad_object(obj->idx.offset, _("offset value overflow for delta base object"));
			p = fill(1);
			c = *p;
			use(1);
			base_offset = (base_offset << 7) + (c & 127);
		}
		*ofs_offset = obj->idx.offset - base_offset;
		यदि (*ofs_offset <= 0 || *ofs_offset >= obj->idx.offset)
			bad_object(obj->idx.offset, _("delta base offset is out of bound"));
		भङ्ग;
	पक्ष OBJ_COMMIT:
	पक्ष OBJ_वृक्ष:
	पक्ष OBJ_BLOB:
	पक्ष OBJ_TAG:
		भङ्ग;
	स्वभाव:
		bad_object(obj->idx.offset, _("unknown object type %d"), obj->type);
	}
	obj->hdr_size = consumed_bytes - obj->idx.offset;

	data = unpack_entry_data(obj->idx.offset, obj->size, obj->type, oid);
	obj->idx.crc32 = निवेश_crc32;
	प्रत्यागम data;
}

स्थिर शून्य *unpack_data(संरचना object_entry *obj,
			 पूर्णाङ्क (*consume)(ध्रुव unsigned वर्ण *, unsigned long, शून्य *),
			 शून्य *cb_data)
{
	off_t from = obj[0].idx.offset + obj[0].hdr_size;
	off_t len = obj[1].idx.offset - from;
	unsigned वर्ण *data, *inbuf;
	git_zstream stream;
	पूर्णाङ्क status;

	data = xmallocz(consume ? 64*1024 : obj->size);
	inbuf = xmalloc((len < 64*1024) ? (पूर्णाङ्क)len : 64*1024);

	memset(&stream, 0, परिमाण(stream));
	git_inflate_init(&stream);
	stream.next_out = data;
	stream.avail_out = consume ? 64*1024 : obj->size;

	कुरु {
		ssize_t n = (len < 64*1024) ? (ssize_t)len : 64*1024;
		n = xpread(get_thread_data()->pack_fd, inbuf, n, from);
		यदि (n < 0)
			die_errno(_("cannot pread pack file"));
		यदि (!n)
			die(Q_("premature end of pack file, %"PRIuMAX" byte missing",
			       "premature end of pack file, %"PRIuMAX" bytes missing",
			       len),
			    (uintmax_t)len);
		from += n;
		len -= n;
		stream.next_in = inbuf;
		stream.avail_in = n;
		यदि (!consume)
			status = git_inflate(&stream, 0);
		अन्यथा {
			कुरु {
				status = git_inflate(&stream, 0);
				यदि (consume(data, stream.next_out - data, cb_data)) {
					मुक्त(inbuf);
					मुक्त(data);
					प्रत्यागम रिक्त;
				}
				stream.next_out = data;
				stream.avail_out = 64*1024;
			} यावत् (status == Z_OK && stream.avail_in);
		}
	} यावत् (len && status == Z_OK && !stream.avail_in);

	/* This has been inflated OK when first encountered, so... */
	यदि (status != Z_STREAM_END || stream.total_out != obj->size)
		die(_("serious inflate inconsistency"));

	git_inflate_end(&stream);
	मुक्त(inbuf);
	यदि (consume) {
		FREE_AND_NULL(data);
	}
	प्रत्यागम data;
}

स्थिर शून्य *get_data_from_pack(संरचना object_entry *obj)
{
	प्रत्यागम unpack_data(obj, रिक्त, रिक्त);
}

स्थिर पूर्णाङ्क compare_ofs_delta_bases(off_t offset1, off_t offset2,
				   गणना object_type type1,
				   गणना object_type type2)
{
	पूर्णाङ्क cmp = type1 - type2;
	यदि (cmp)
		प्रत्यागम cmp;
	प्रत्यागम offset1 < offset2 ? -1 :
	       offset1 > offset2 ?  1 :
	       0;
}

स्थिर पूर्णाङ्क find_ofs_delta(ध्रुव off_t offset)
{
	पूर्णाङ्क first = 0, last = nr_ofs_deltas;

	यावत् (first < last) {
		पूर्णाङ्क next = first + (last - first) / 2;
		संरचना ofs_delta_entry *delta = &ofs_deltas[next];
		पूर्णाङ्क cmp;

		cmp = compare_ofs_delta_bases(offset, delta->offset,
					      OBJ_OFS_DELTA,
					      objects[delta->obj_no].type);
		यदि (!cmp)
			प्रत्यागम next;
		यदि (cmp < 0) {
			last = next;
			अनुवर्त;
		}
		first = next+1;
	}
	प्रत्यागम -first-1;
}

स्थिर शून्य find_ofs_delta_children(off_t offset,
				    पूर्णाङ्क *first_index, पूर्णाङ्क *last_index)
{
	पूर्णाङ्क first = find_ofs_delta(offset);
	पूर्णाङ्क last = first;
	पूर्णाङ्क end = nr_ofs_deltas - 1;

	यदि (first < 0) {
		*first_index = 0;
		*last_index = -1;
		प्रत्यागम;
	}
	यावत् (first > 0 && ofs_deltas[first - 1].offset == offset)
		--first;
	यावत् (last < end && ofs_deltas[last + 1].offset == offset)
		++last;
	*first_index = first;
	*last_index = last;
}

स्थिर पूर्णाङ्क compare_ref_delta_bases(ध्रुव संरचना object_id *oid1,
				   ध्रुव संरचना object_id *oid2,
				   गणना object_type type1,
				   गणना object_type type2)
{
	पूर्णाङ्क cmp = type1 - type2;
	यदि (cmp)
		प्रत्यागम cmp;
	प्रत्यागम oidcmp(oid1, oid2);
}

स्थिर पूर्णाङ्क find_ref_delta(ध्रुव संरचना object_id *oid)
{
	पूर्णाङ्क first = 0, last = nr_ref_deltas;

	यावत् (first < last) {
		पूर्णाङ्क next = first + (last - first) / 2;
		संरचना ref_delta_entry *delta = &ref_deltas[next];
		पूर्णाङ्क cmp;

		cmp = compare_ref_delta_bases(oid, &delta->oid,
					      OBJ_REF_DELTA,
					      objects[delta->obj_no].type);
		यदि (!cmp)
			प्रत्यागम next;
		यदि (cmp < 0) {
			last = next;
			अनुवर्त;
		}
		first = next+1;
	}
	प्रत्यागम -first-1;
}

स्थिर शून्य find_ref_delta_children(ध्रुव संरचना object_id *oid,
				    पूर्णाङ्क *first_index, पूर्णाङ्क *last_index)
{
	पूर्णाङ्क first = find_ref_delta(oid);
	पूर्णाङ्क last = first;
	पूर्णाङ्क end = nr_ref_deltas - 1;

	यदि (first < 0) {
		*first_index = 0;
		*last_index = -1;
		प्रत्यागम;
	}
	यावत् (first > 0 && oideq(&ref_deltas[first - 1].oid, oid))
		--first;
	यावत् (last < end && oideq(&ref_deltas[last + 1].oid, oid))
		++last;
	*first_index = first;
	*last_index = last;
}

संरचना compare_data {
	संरचना object_entry *entry;
	संरचना odb_stream *st;
	unsigned वर्ण *buf;
	unsigned long buf_size;
};

स्थिर पूर्णाङ्क compare_objects(ध्रुव unsigned वर्ण *buf, unsigned long size,
			   शून्य *cb_data)
{
	संरचना compare_data *data = cb_data;

	यदि (data->buf_size < size) {
		मुक्त(data->buf);
		data->buf = xmalloc(size);
		data->buf_size = size;
	}

	यावत् (size) {
		ssize_t len = odb_stream_read(data->st, data->buf, size);
		यदि (len == 0)
			die(_("SHA1 COLLISION FOUND WITH %s !"),
			    oid_to_hex(&data->entry->idx.oid));
		यदि (len < 0)
			die(_("unable to read %s"),
			    oid_to_hex(&data->entry->idx.oid));
		यदि (memcmp(buf, data->buf, len))
			die(_("SHA1 COLLISION FOUND WITH %s !"),
			    oid_to_hex(&data->entry->idx.oid));
		size -= len;
		buf += len;
	}
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क check_collison(संरचना object_entry *entry)
{
	संरचना compare_data data;

	यदि (entry->size <= repo_settings_get_big_file_threshold(the_repository) ||
	    entry->type != OBJ_BLOB)
		प्रत्यागम -1;

	memset(&data, 0, परिमाण(data));
	data.entry = entry;
	data.st = odb_stream_from_object(the_repository->objects, &entry->idx.oid, रिक्त);
	यदि (!data.st)
		प्रत्यागम -1;
	यदि (data.st->size != entry->size || data.st->type != entry->type)
		die(_("SHA1 COLLISION FOUND WITH %s !"),
		    oid_to_hex(&entry->idx.oid));
	unpack_data(entry, compare_objects, &data);
	odb_stream_close(data.st);
	मुक्त(data.buf);
	प्रत्यागम 0;
}

स्थिर शून्य record_outgoing_link(ध्रुव संरचना object_id *oid)
{
	oidset_जोड़ो(&outgoing_links, oid);
}

स्थिर शून्य maybe_record_name_entry(ध्रुव संरचना name_entry *entry)
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
	यदि (S_ISDIR(entry->mode))
		record_outgoing_link(&entry->oid);
}

स्थिर शून्य do_record_outgoing_links(संरचना object *obj)
{
	यदि (obj->type == OBJ_वृक्ष) {
		संरचना वृक्ष *वृक्ष = (संरचना वृक्ष *)obj;
		संरचना वृक्ष_desc desc;
		संरचना name_entry entry;
		यदि (init_वृक्ष_desc_gently(&desc, &वृक्ष->object.oid,
					  वृक्ष->buffer, वृक्ष->size, 0))
			/*
			 * Error messages are given when packs are
			 * verified, so do not print any here.
			 */
			प्रत्यागम;
		यावत् (वृक्ष_entry_gently(&desc, &entry))
			maybe_record_name_entry(&entry);
	} अन्यथा यदि (obj->type == OBJ_COMMIT) {
		संरचना commit *commit = (संरचना commit *) obj;
		संरचना commit_list *parents = commit->parents;

		record_outgoing_link(get_commit_वृक्ष_oid(commit));
		प्रति (; parents; parents = parents->next)
			record_outgoing_link(&parents->item->object.oid);
	} अन्यथा यदि (obj->type == OBJ_TAG) {
		संरचना tag *tag = (संरचना tag *) obj;
		record_outgoing_link(get_tagged_oid(tag));
	}
}

स्थिर शून्य sha1_object(ध्रुव शून्य *data, संरचना object_entry *obj_entry,
			unsigned long size, गणना object_type type,
			ध्रुव संरचना object_id *oid)
{
	शून्य *new_data = रिक्त;
	पूर्णाङ्क collision_test_needed = 0;

	प्रतिज्ञा(data || obj_entry);

	यदि (startup_info->have_repository) {
		read_lock();
		collision_test_needed = odb_has_object(the_repository->objects, oid,
						       ODB_HAS_OBJECT_FETCH_PROMISOR);
		read_unlock();
	}

	यदि (collision_test_needed && !data) {
		read_lock();
		यदि (!check_collison(obj_entry))
			collision_test_needed = 0;
		read_unlock();
	}
	यदि (collision_test_needed) {
		शून्य *has_data;
		गणना object_type has_type;
		size_t has_size;
		read_lock();
		has_type = odb_read_object_info(the_repository->objects, oid, &has_size);
		यदि (has_type < 0)
			die(_("cannot read existing object info %s"), oid_to_hex(oid));
		यदि (has_type != type || has_size != size)
			die(_("SHA1 COLLISION FOUND WITH %s !"), oid_to_hex(oid));
		has_data = odb_read_object(the_repository->objects, oid,
					   &has_type, &has_size);
		read_unlock();
		यदि (!data)
			data = new_data = get_data_from_pack(obj_entry);
		यदि (!has_data)
			die(_("cannot read existing object %s"), oid_to_hex(oid));
		यदि (size != has_size || type != has_type ||
		    memcmp(data, has_data, size) != 0)
			die(_("SHA1 COLLISION FOUND WITH %s !"), oid_to_hex(oid));
		मुक्त(has_data);
	}

	यदि (strict || do_fsck_object || record_outgoing_links) {
		read_lock();
		यदि (type == OBJ_BLOB) {
			संरचना blob *blob = lookup_blob(the_repository, oid);
			यदि (blob)
				blob->object.flags |= FLAG_CHECKED;
			अन्यथा
				die(_("invalid blob object %s"), oid_to_hex(oid));
			यदि (do_fsck_object &&
			    fsck_object(&blob->object, (शून्य *)data, size, &fsck_options))
				die(_("fsck error in packed object"));
		} अन्यथा {
			संरचना object *obj;
			पूर्णाङ्क eaten;
			शून्य *buf = (शून्य *) data;

			प्रतिज्ञा(data && "data can only be NULL for large _blobs_");

			/*
			 * we do not need to free the memory here, as the
			 * buf is deleted by the caller.
			 */
			obj = parse_object_buffer(the_repository, oid, type,
						  size, buf,
						  &eaten);
			यदि (!obj)
				die(_("invalid %s"), type_name(type));
			यदि (do_fsck_object &&
			    fsck_object(obj, buf, size, &fsck_options))
				die(_("fsck error in packed object"));
			यदि (strict && fsck_walk(obj, रिक्त, &fsck_options))
				die(_("Not all child objects of %s are reachable"), oid_to_hex(&obj->oid));
			यदि (record_outgoing_links)
				do_record_outgoing_links(obj);

			यदि (obj->type == OBJ_वृक्ष) {
				संरचना वृक्ष *item = (संरचना वृक्ष *) obj;
				item->buffer = रिक्त;
				obj->parsed = 0;
			}
			यदि (obj->type == OBJ_COMMIT) {
				संरचना commit *commit = (संरचना commit *) obj;
				यदि (detach_commit_buffer(commit, रिक्त) != data)
					BUG("parse_object_buffer transmogrified our buffer");
			}
			obj->flags |= FLAG_CHECKED;
		}
		read_unlock();
	}

	मुक्त(new_data);
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
स्थिर शून्य *get_base_data(संरचना base_data *c)
{
	यदि (!c->data) {
		संरचना object_entry *obj = c->obj;
		संरचना base_data **delta = रिक्त;
		पूर्णाङ्क delta_nr = 0, delta_alloc = 0;

		यावत् (is_delta_type(c->obj->type) && !c->data) {
			ALLOC_GROW(delta, delta_nr + 1, delta_alloc);
			delta[delta_nr++] = c;
			c = c->base;
		}
		यदि (!delta_nr) {
			c->data = get_data_from_pack(obj);
			c->size = obj->size;
			base_cache_used += c->size;
			prune_base_data(c);
		}
		प्रति (; delta_nr > 0; delta_nr--) {
			शून्य *base, *raw;
			c = delta[delta_nr - 1];
			obj = c->obj;
			base = get_base_data(c->base);
			raw = get_data_from_pack(obj);
			c->data = patch_delta(
				base, c->base->size,
				raw, obj->size,
				&c->size);
			मुक्त(raw);
			यदि (!c->data)
				bad_object(obj->idx.offset, _("failed to apply delta"));
			base_cache_used += c->size;
			prune_base_data(c);
		}
		मुक्त(delta);
	}
	प्रत्यागम c->data;
}

स्थिर संरचना base_data *make_base(संरचना object_entry *obj,
				   संरचना base_data *parent)
{
	संरचना base_data *base = xcalloc(1, परिमाण(संरचना base_data));
	base->base = parent;
	base->obj = obj;
	find_ref_delta_children(&obj->idx.oid,
				&base->ref_first, &base->ref_last);
	find_ofs_delta_children(obj->idx.offset,
				&base->ofs_first, &base->ofs_last);
	base->children_remaining = base->ref_last - base->ref_first +
		base->ofs_last - base->ofs_first + 2;
	प्रत्यागम base;
}

स्थिर संरचना base_data *resolve_delta(संरचना object_entry *delta_obj,
				       संरचना base_data *base)
{
	शून्य *delta_data, *result_data;
	संरचना base_data *result;
	size_t result_size;

	यदि (show_stat) {
		पूर्णाङ्क i = delta_obj - objects;
		पूर्णाङ्क j = base->obj - objects;
		obj_stat[i].delta_depth = obj_stat[j].delta_depth + 1;
		deepest_delta_lock();
		यदि (deepest_delta < obj_stat[i].delta_depth)
			deepest_delta = obj_stat[i].delta_depth;
		deepest_delta_unlock();
		obj_stat[i].base_object_no = j;
	}
	delta_data = get_data_from_pack(delta_obj);
	प्रतिज्ञा(base->data);
	result_data = patch_delta(base->data, base->size,
				  delta_data, delta_obj->size, &result_size);
	मुक्त(delta_data);
	यदि (!result_data)
		bad_object(delta_obj->idx.offset, _("failed to apply delta"));
	hash_object_file(the_hash_algo, result_data, result_size,
			 delta_obj->real_type, &delta_obj->idx.oid);
	sha1_object(result_data, रिक्त, result_size, delta_obj->real_type,
		    &delta_obj->idx.oid);

	result = make_base(delta_obj, base);
	result->data = result_data;
	result->size = result_size;

	counter_lock();
	nr_resolved_deltas++;
	counter_unlock();

	प्रत्यागम result;
}

स्थिर पूर्णाङ्क compare_ofs_delta_entry(ध्रुव शून्य *a, ध्रुव शून्य *b)
{
	ध्रुव संरचना ofs_delta_entry *delta_a = a;
	ध्रुव संरचना ofs_delta_entry *delta_b = b;

	प्रत्यागम delta_a->offset < delta_b->offset ? -1 :
	       delta_a->offset > delta_b->offset ?  1 :
	       0;
}

स्थिर पूर्णाङ्क compare_ref_delta_entry(ध्रुव शून्य *a, ध्रुव शून्य *b)
{
	ध्रुव संरचना ref_delta_entry *delta_a = a;
	ध्रुव संरचना ref_delta_entry *delta_b = b;

	प्रत्यागम oidcmp(&delta_a->oid, &delta_b->oid);
}

स्थिर शून्य *threaded_second_pass(शून्य *data)
{
	यदि (data)
		set_thread_data(data);
	प्रति (;;) {
		संरचना base_data *parent = रिक्त;
		संरचना object_entry *child_obj = रिक्त;
		संरचना base_data *child = रिक्त;

		counter_lock();
		display_progress(progress, nr_resolved_deltas);
		counter_unlock();

		work_lock();
		यदि (list_empty(&work_head)) {
			/*
			 * Take an object from the object array.
			 */
			यावत् (nr_dispatched < nr_objects &&
			       is_delta_type(objects[nr_dispatched].type))
				nr_dispatched++;
			यदि (nr_dispatched >= nr_objects) {
				work_unlock();
				भङ्ग;
			}
			child_obj = &objects[nr_dispatched++];
		} अन्यथा {
			/*
			 * Peek at the top of the stack, and take a child from
			 * it.
			 */
			parent = list_first_entry(&work_head, संरचना base_data,
						  list);

			यावत् (parent->ref_first <= parent->ref_last) {
				पूर्णाङ्क offset = ref_deltas[parent->ref_first++].obj_no;
				child_obj = objects + offset;
				यदि (child_obj->real_type != OBJ_REF_DELTA) {
					child_obj = रिक्त;
					अनुवर्त;
				}
				child_obj->real_type = parent->obj->real_type;
				भङ्ग;
			}

			यदि (!child_obj && parent->ofs_first <= parent->ofs_last) {
				child_obj = objects +
					ofs_deltas[parent->ofs_first++].obj_no;
				प्रतिज्ञा(child_obj->real_type == OBJ_OFS_DELTA);
				child_obj->real_type = parent->obj->real_type;
			}

			यदि (parent->ref_first > parent->ref_last &&
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

		यदि (child_obj) {
			यदि (parent) {
				child = resolve_delta(child_obj, parent);
				यदि (!child->children_remaining)
					FREE_AND_NULL(child->data);
			} अन्यथा{
				child = make_base(child_obj, रिक्त);
				यदि (child->children_remaining) {
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
		यदि (parent)
			parent->retain_data--;

		यदि (child && child->data) {
			/*
			 * This child has its own children, so add it to
			 * work_head.
			 */
			list_add(&child->list, &work_head);
			base_cache_used += child->size;
			prune_base_data(रिक्त);
		} अन्यथा यदि (child) {
			/*
			 * This child does not have its own children. It may be
			 * the last descendant of its ancestors; free those
			 * that we can.
			 */
			संरचना base_data *p = parent;

			यावत् (p) {
				संरचना base_data *next_p;

				p->children_remaining--;
				यदि (p->children_remaining)
					भङ्ग;

				next_p = p->base;
				free_base_data(p);
				list_del(&p->list);
				मुक्त(p);

				p = next_p;
			}
			FREE_AND_NULL(child);
		}
		work_unlock();
	}
	प्रत्यागम रिक्त;
}

/*
 * First pass:
 * - find locations of all objects;
 * - calculate SHA1 of all non-delta objects;
 * - remember base (SHA1 or offset) for all deltas.
 */
स्थिर शून्य parse_pack_objects(unsigned वर्ण *hash)
{
	पूर्णाङ्क i, nr_delays = 0;
	संरचना ofs_delta_entry *ofs_delta = ofs_deltas;
	संरचना object_id ref_delta_oid;
	संरचना stat st;
	संरचना git_hash_ctx tmp_ctx;

	यदि (verbose)
		progress = start_progress(
				the_repository,
				progress_title ? progress_title :
				from_stdin ? _("Receiving objects") : _("Indexing objects"),
				nr_objects);
	प्रति (i = 0; i < nr_objects; i++) {
		संरचना object_entry *obj = &objects[i];
		शून्य *data = unpack_raw_entry(obj, &ofs_delta->offset,
					      &ref_delta_oid,
					      &obj->idx.oid);
		obj->real_type = obj->type;
		यदि (obj->type == OBJ_OFS_DELTA) {
			nr_ofs_deltas++;
			ofs_delta->obj_no = i;
			ofs_delta++;
		} अन्यथा यदि (obj->type == OBJ_REF_DELTA) {
			ALLOC_GROW(ref_deltas, nr_ref_deltas + 1, ref_deltas_alloc);
			oidcpy(&ref_deltas[nr_ref_deltas].oid, &ref_delta_oid);
			ref_deltas[nr_ref_deltas].obj_no = i;
			nr_ref_deltas++;
		} अन्यथा यदि (!data) {
			/* large blobs, check later */
			obj->real_type = OBJ_BAD;
			nr_delays++;
		} अन्यथा
			sha1_object(data, रिक्त, obj->size, obj->type,
				    &obj->idx.oid);
		मुक्त(data);
		display_progress(progress, i+1);
	}
	objects[i].idx.offset = consumed_bytes;
	stop_progress(&progress);

	/* Check pack integrity */
	flush();
	git_hash_init(&tmp_ctx, the_hash_algo);
	git_hash_clone(&tmp_ctx, &निवेश_ctx);
	git_hash_final(hash, &tmp_ctx);
	यदि (!hasheq(fill(the_hash_algo->rawsz), hash, the_repository->hash_algo))
		die(_("pack is corrupted (SHA1 mismatch)"));
	use(the_hash_algo->rawsz);

	/* If input_fd is a file, we should have reached its end now. */
	यदि (fstat(निवेश_fd, &st))
		die_errno(_("cannot fstat packfile"));
	यदि (S_ISREG(st.st_mode) &&
			lseek(निवेश_fd, 0, SEEK_CUR) - निवेश_len != st.st_size)
		die(_("pack has junk at the end"));

	प्रति (i = 0; i < nr_objects; i++) {
		संरचना object_entry *obj = &objects[i];
		यदि (obj->real_type != OBJ_BAD)
			अनुवर्त;
		obj->real_type = obj->type;
		sha1_object(रिक्त, obj, obj->size, obj->type,
			    &obj->idx.oid);
		nr_delays--;
	}
	यदि (nr_delays)
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
स्थिर शून्य resolve_deltas(संरचना pack_idx_option *opts)
{
	पूर्णाङ्क i;

	यदि (!nr_ofs_deltas && !nr_ref_deltas)
		प्रत्यागम;

	/* Sort deltas by base SHA1/offset for fast searching */
	QSORT(ofs_deltas, nr_ofs_deltas, compare_ofs_delta_entry);
	QSORT(ref_deltas, nr_ref_deltas, compare_ref_delta_entry);

	यदि (verbose || show_resolving_progress)
		progress = start_progress(the_repository,
					  _("Resolving deltas"),
					  nr_ref_deltas + nr_ofs_deltas);

	nr_dispatched = 0;
	base_cache_limit = opts->delta_base_cache_limit * nr_threads;
	यदि (nr_threads > 1 || getenv("GIT_FORCE_THREADS")) {
		init_thread();
		प्रति (i = 0; i < nr_threads; i++) {
			पूर्णाङ्क ret = pthread_create(&thread_data[i].thread, रिक्त,
						 threaded_second_pass, thread_data + i);
			यदि (ret)
				die(_("unable to create thread: %s"),
				    strerror(ret));
		}
		प्रति (i = 0; i < nr_threads; i++)
			pthread_join(thread_data[i].thread, रिक्त);
		cleanup_thread();
		प्रत्यागम;
	}
	threaded_second_pass(&nothread_data);
}

/*
 * Third pass:
 * - append objects to convert thin pack to full pack if required
 * - write the final pack hash
 */
स्थिर शून्य fix_unresolved_deltas(संरचना hashfile *f);
स्थिर शून्य conclude_pack(पूर्णाङ्क fix_thin_pack, ध्रुव वर्ण *curr_pack, unsigned वर्ण *pack_hash)
{
	यदि (nr_ref_deltas + nr_ofs_deltas == nr_resolved_deltas) {
		stop_progress(&progress);
		/* Flush remaining pack final hash. */
		flush();
		प्रत्यागम;
	}

	यदि (fix_thin_pack) {
		संरचना hashfile *f;
		unsigned वर्ण read_hash[GIT_MAX_RAWSZ], tail_hash[GIT_MAX_RAWSZ];
		संरचना strbuf संदेश = STRBUF_INIT;
		पूर्णाङ्क nr_unresolved = nr_ofs_deltas + nr_ref_deltas - nr_resolved_deltas;
		पूर्णाङ्क nr_objects_initial = nr_objects;
		यदि (nr_unresolved <= 0)
			die(_("confusion beyond insanity"));
		REALLOC_ARRAY(objects, nr_objects + nr_unresolved + 1);
		memset(objects + nr_objects + 1, 0,
		       nr_unresolved * परिमाण(*objects));
		f = hashfd(the_repository->hash_algo, output_fd, curr_pack);
		fix_unresolved_deltas(f);
		strbuf_addf(&संदेश, Q_("completed with %d local object",
				     "completed with %d local objects",
				     nr_objects - nr_objects_initial),
			    nr_objects - nr_objects_initial);
		stop_progress_संदेश(&progress, संदेश.buf);
		strbuf_release(&संदेश);
		finalize_hashfile(f, tail_hash, FSYNC_COMPONENT_PACK, 0);
		hashcpy(read_hash, pack_hash, the_repository->hash_algo);
		fixup_pack_header_footer(the_hash_algo, output_fd, pack_hash,
					 curr_pack, nr_objects,
					 read_hash, consumed_bytes-the_hash_algo->rawsz);
		यदि (!hasheq(read_hash, tail_hash, the_repository->hash_algo))
			die(_("Unexpected tail checksum for %s "
			      "(disk corruption?)"), curr_pack);
	}
	यदि (nr_ofs_deltas + nr_ref_deltas != nr_resolved_deltas)
		die(Q_("pack has %d unresolved delta",
		       "pack has %d unresolved deltas",
		       nr_ofs_deltas + nr_ref_deltas - nr_resolved_deltas),
		    nr_ofs_deltas + nr_ref_deltas - nr_resolved_deltas);
}

स्थिर पूर्णाङ्क write_compressed(संरचना hashfile *f, शून्य *in, unsigned पूर्णाङ्क size)
{
	git_zstream stream;
	पूर्णाङ्क status;
	unsigned वर्ण outbuf[4096];
	संरचना repo_config_मूल्यावली *cfg = repo_config_मूल्यावली(the_repository);

	git_deflate_init(&stream, cfg->zlib_compression_level);
	stream.next_in = in;
	stream.avail_in = size;

	कुरु {
		stream.next_out = outbuf;
		stream.avail_out = परिमाण(outbuf);
		status = git_deflate(&stream, Z_FINISH);
		hashwrite(f, outbuf, परिमाण(outbuf) - stream.avail_out);
	} यावत् (status == Z_OK);

	यदि (status != Z_STREAM_END)
		die(_("unable to deflate appended object (%d)"), status);
	size = stream.total_out;
	git_deflate_end(&stream);
	प्रत्यागम size;
}

स्थिर संरचना object_entry *append_obj_to_pack(संरचना hashfile *f,
			       ध्रुव unsigned वर्ण *sha1, शून्य *buf,
			       unsigned long size, गणना object_type type)
{
	संरचना object_entry *obj = &objects[nr_objects++];
	unsigned वर्ण header[10];
	unsigned long s = size;
	पूर्णाङ्क n = 0;
	unsigned वर्ण c = (type << 4) | (s & 15);
	s >>= 4;
	यावत् (s) {
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
	प्रत्यागम obj;
}

स्थिर पूर्णाङ्क delta_pos_compare(ध्रुव शून्य *_a, ध्रुव शून्य *_b)
{
	संरचना ref_delta_entry *a = *(संरचना ref_delta_entry **)_a;
	संरचना ref_delta_entry *b = *(संरचना ref_delta_entry **)_b;
	प्रत्यागम a->obj_no - b->obj_no;
}

स्थिर शून्य fix_unresolved_deltas(संरचना hashfile *f)
{
	संरचना ref_delta_entry **sorted_by_pos;
	पूर्णाङ्क i;

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
	प्रति (i = 0; i < nr_ref_deltas; i++)
		sorted_by_pos[i] = &ref_deltas[i];
	QSORT(sorted_by_pos, nr_ref_deltas, delta_pos_compare);

	यदि (repo_has_promisor_remote(the_repository)) {
		/*
		 * Prefetch the delta bases.
		 */
		संरचना oid_array to_fetch = OID_ARRAY_INIT;
		प्रति (i = 0; i < nr_ref_deltas; i++) {
			संरचना ref_delta_entry *d = sorted_by_pos[i];
			यदि (!odb_read_object_info_extended(the_repository->objects,
							   &d->oid, रिक्त,
							   OBJECT_INFO_FOR_PREFETCH))
				अनुवर्त;
			oid_array_append(&to_fetch, &d->oid);
		}
		promisor_remote_get_direct(the_repository,
					   to_fetch.oid, to_fetch.nr);
		oid_array_clear(&to_fetch);
	}

	प्रति (i = 0; i < nr_ref_deltas; i++) {
		संरचना ref_delta_entry *d = sorted_by_pos[i];
		गणना object_type type;
		शून्य *data;
		size_t size;

		यदि (objects[d->obj_no].real_type != OBJ_REF_DELTA)
			अनुवर्त;
		data = odb_read_object(the_repository->objects, &d->oid,
				       &type, &size);
		यदि (!data)
			अनुवर्त;

		यदि (check_object_signature(the_repository, &d->oid, data, size,
					   type) < 0)
			die(_("local object %s is corrupt"), oid_to_hex(&d->oid));

		/*
		 * Add this as an object to the objects array and call
		 * threaded_second_pass() (which will pick up the added
		 * object).
		 */
		append_obj_to_pack(f, d->oid.hash, data, size, type);
		मुक्त(data);
		threaded_second_pass(रिक्त);

		display_progress(progress, nr_resolved_deltas);
	}
	मुक्त(sorted_by_pos);
}

स्थिर ध्रुव वर्ण *derive_filename(ध्रुव वर्ण *pack_name, ध्रुव वर्ण *strip,
				   ध्रुव वर्ण *suffix, संरचना strbuf *buf)
{
	size_t len;
	यदि (!strip_suffix(pack_name, strip, &len) || !len ||
	    pack_name[len - 1] != '.')
		die(_("packfile name '%s' does not end with '.%s'"),
		    pack_name, strip);
	strbuf_add(buf, pack_name, len);
	strbuf_addstr(buf, suffix);
	प्रत्यागम buf->buf;
}

स्थिर शून्य write_special_file(ध्रुव वर्ण *suffix, ध्रुव वर्ण *संदेश,
			       ध्रुव वर्ण *pack_name, ध्रुव unsigned वर्ण *hash,
			       ध्रुव वर्ण **report)
{
	संरचना strbuf name_buf = STRBUF_INIT;
	ध्रुव वर्ण *filename;
	पूर्णाङ्क fd;
	पूर्णाङ्क संदेश_len = strlen(संदेश);

	यदि (pack_name)
		filename = derive_filename(pack_name, "pack", suffix, &name_buf);
	अन्यथा
		filename = odb_pack_name(the_repository, &name_buf, hash, suffix);

	fd = safe_create_file_with_leading_directories(the_repository, filename);
	यदि (fd < 0) {
		यदि (errno != EEXIST)
			die_errno(_("cannot write %s file '%s'"),
				  suffix, filename);
	} अन्यथा {
		यदि (संदेश_len > 0) {
			write_or_die(fd, संदेश, संदेश_len);
			write_or_die(fd, "\n", 1);
		}
		यदि (close(fd) != 0)
			die_errno(_("cannot close written %s file '%s'"),
				  suffix, filename);
		यदि (report)
			*report = suffix;
	}
	strbuf_release(&name_buf);
}

स्थिर शून्य rename_tmp_packfile(ध्रुव वर्ण **final_name,
				ध्रुव वर्ण *curr_name,
				संरचना strbuf *name, unsigned वर्ण *hash,
				ध्रुव वर्ण *ext, पूर्णाङ्क make_read_only_if_same)
{
	यदि (!*final_name || strcmp(*final_name, curr_name)) {
		यदि (!*final_name)
			*final_name = odb_pack_name(the_repository, name, hash, ext);
		यदि (finalize_object_file(the_repository, curr_name, *final_name))
			die(_("unable to rename temporary '*.%s' file to '%s'"),
			    ext, *final_name);
	} अन्यथा यदि (make_read_only_if_same) {
		chmod(*final_name, 0444);
	}
}

स्थिर शून्य final(ध्रुव वर्ण *final_pack_name, ध्रुव वर्ण *curr_pack_name,
		  ध्रुव वर्ण *final_index_name, ध्रुव वर्ण *curr_index_name,
		  ध्रुव वर्ण *final_rev_index_name, ध्रुव वर्ण *curr_rev_index_name,
		  ध्रुव वर्ण *keep_संदेश, ध्रुव वर्ण *promisor_संदेश,
		  unsigned वर्ण *hash)
{
	ध्रुव वर्ण *report = "pack";
	संरचना strbuf pack_name = STRBUF_INIT;
	संरचना strbuf index_name = STRBUF_INIT;
	संरचना strbuf rev_index_name = STRBUF_INIT;

	यदि (!from_stdin) {
		close(निवेश_fd);
	} अन्यथा {
		fsync_component_or_die(FSYNC_COMPONENT_PACK, output_fd, curr_pack_name);
		यदि (close(output_fd))
			die_errno(_("error while closing pack file"));
	}

	यदि (keep_संदेश)
		write_special_file("keep", keep_संदेश, final_pack_name, hash,
				   &report);
	यदि (promisor_संदेश)
		write_special_file("promisor", promisor_संदेश, final_pack_name,
				   hash, रिक्त);

	rename_tmp_packfile(&final_pack_name, curr_pack_name, &pack_name,
			    hash, "pack", from_stdin);
	यदि (curr_rev_index_name)
		rename_tmp_packfile(&final_rev_index_name, curr_rev_index_name,
				    &rev_index_name, hash, "rev", 1);
	rename_tmp_packfile(&final_index_name, curr_index_name, &index_name,
			    hash, "idx", 1);

	यदि (do_fsck_object && startup_info->have_repository) {
		संरचना odb_source_files *files =
			odb_source_files_downcast(the_repository->objects->source);
		packfile_store_load_pack(files->dirs->packed, final_index_name, 0);
	}

	यदि (!from_stdin) {
		printf("%s\n", hash_to_hex(hash));
	} अन्यथा {
		संरचना strbuf buf = STRBUF_INIT;

		strbuf_addf(&buf, "%s\t%s\n", report, hash_to_hex(hash));
		write_or_die(1, buf.buf, buf.len);
		strbuf_release(&buf);

		/* Write the last part of the buffer to stdout */
		write_in_full(1, निवेश_buffer + निवेश_offset, निवेश_len);
	}

	strbuf_release(&rev_index_name);
	strbuf_release(&index_name);
	strbuf_release(&pack_name);
}

स्थिर पूर्णाङ्क git_index_pack_config(ध्रुव वर्ण *k, ध्रुव वर्ण *v,
				 ध्रुव संरचना config_context *ctx, शून्य *cb)
{
	संरचना pack_idx_option *opts = cb;

	यदि (!strcmp(k, "pack.indexversion")) {
		opts->version = git_config_int(k, v, ctx->kvi);
		यदि (opts->version > 2)
			die(_("bad pack.indexVersion=%"PRIu32), opts->version);
		प्रत्यागम 0;
	}
	यदि (!strcmp(k, "pack.threads")) {
		nr_threads = git_config_int(k, v, ctx->kvi);
		यदि (nr_threads < 0)
			die(_("invalid number of threads specified (%d)"),
			    nr_threads);
		यदि (!HAVE_THREADS && nr_threads != 1) {
			warning(_("no threads support, ignoring %s"), k);
			nr_threads = 1;
		}
		प्रत्यागम 0;
	}
	यदि (!strcmp(k, "pack.writereverseindex")) {
		यदि (git_config_bool(k, v))
			opts->flags |= WRITE_REV;
		अन्यथा
			opts->flags &= ~WRITE_REV;
	}
	यदि (!strcmp(k, "core.deltabasecachelimit")) {
		opts->delta_base_cache_limit = git_config_ulong(k, v, ctx->kvi);
		प्रत्यागम 0;
	}
	प्रत्यागम git_default_config(k, v, ctx, cb);
}

स्थिर पूर्णाङ्क cmp_uint32(ध्रुव शून्य *a_, ध्रुव शून्य *b_)
{
	uint32_t a = *((uint32_t *)a_);
	uint32_t b = *((uint32_t *)b_);

	प्रत्यागम (a < b) ? -1 : (a != b);
}

स्थिर शून्य read_v2_anomalous_offsets(संरचना packed_git *p,
				      संरचना pack_idx_option *opts)
{
	ध्रुव uint32_t *idx1, *idx2;
	uint32_t i;

	/* The address of the 4-byte offset table */
	idx1 = (((ध्रुव uint32_t *)((ध्रुव uint8_t *)p->index_data + p->crc_offset))
		+ (size_t)p->अंक_objects /* CRC32 table */
		);

	/* The address of the 8-byte offset table */
	idx2 = idx1 + p->अंक_objects;

	प्रति (i = 0; i < p->अंक_objects; i++) {
		uint32_t off = ntohl(idx1[i]);
		यदि (!(off & 0x80000000))
			अनुवर्त;
		off = off & 0x7fffffff;
		check_pack_index_ptr(p, &idx2[off * 2]);
		यदि (idx2[off * 2])
			अनुवर्त;
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

स्थिर शून्य read_idx_option(संरचना pack_idx_option *opts, ध्रुव वर्ण *pack_name)
{
	संरचना packed_git *p = add_packed_git(the_repository, pack_name,
					      strlen(pack_name), 1);

	यदि (!p)
		die(_("Cannot open existing pack file '%s'"), pack_name);
	यदि (open_pack_index(p))
		die(_("Cannot open existing pack idx file for '%s'"), pack_name);

	/* Read the attributes from the existing idx file */
	opts->version = p->index_version;

	यदि (opts->version == 2)
		read_v2_anomalous_offsets(p, opts);

	/*
	 * Get rid of the idx file as we do not need it anymore.
	 * NEEDSWORK: extract this bit from free_pack_by_name() in
	 * object-file.c, perhaps?  It shouldn't matter very much as we
	 * know we haven't installed this pack (hence we never have
	 * read anything from it).
	 */
	close_pack_index(p);
	मुक्त(p);
}

स्थिर शून्य show_pack_info(पूर्णाङ्क stat_only)
{
	पूर्णाङ्क i, baseobjects = nr_objects - nr_ref_deltas - nr_ofs_deltas;
	unsigned long *chain_histogram = रिक्त;

	यदि (deepest_delta)
		CALLOC_ARRAY(chain_histogram, deepest_delta);

	प्रति (i = 0; i < nr_objects; i++) {
		संरचना object_entry *obj = &objects[i];

		यदि (is_delta_type(obj->type))
			chain_histogram[obj_stat[i].delta_depth - 1]++;
		यदि (stat_only)
			अनुवर्त;
		printf("%s %-6s %"PRIuMAX" %"PRIuMAX" %"PRIuMAX,
		       oid_to_hex(&obj->idx.oid),
		       type_name(obj->real_type), (uintmax_t)obj->size,
		       (uintmax_t)(obj[1].idx.offset - obj->idx.offset),
		       (uintmax_t)obj->idx.offset);
		यदि (is_delta_type(obj->type)) {
			संरचना object_entry *bobj = &objects[obj_stat[i].base_object_no];
			printf(" %u %s", obj_stat[i].delta_depth,
			       oid_to_hex(&bobj->idx.oid));
		}
		putchar('\n');
	}

	यदि (baseobjects)
		printf_ln(Q_("non delta: %d object",
			     "non delta: %d objects",
			     baseobjects),
			  baseobjects);
	प्रति (i = 0; i < deepest_delta; i++) {
		यदि (!chain_histogram[i])
			अनुवर्त;
		printf_ln(Q_("chain length = %d: %lu object",
			     "chain length = %d: %lu objects",
			     chain_histogram[i]),
			  i + 1,
			  chain_histogram[i]);
	}
	मुक्त(chain_histogram);
}

स्थिर शून्य repack_local_links(शून्य)
{
	संरचना child_process cmd = CHILD_PROCESS_INIT;
	FILE *out;
	संरचना strbuf पंक्ति = STRBUF_INIT;
	संरचना oidset_iter iter;
	संरचना object_id *oid;
	वर्ण *base_name = रिक्त;

	यदि (!oidset_size(&outgoing_links))
		प्रत्यागम;

	oidset_iter_init(&outgoing_links, &iter);
	यावत् ((oid = oidset_iter_next(&iter))) {
		संरचना odb_source_info source_info;
		संरचना object_info info = {
			.source_infop = &source_info,
		};

		यदि (odb_read_object_info_extended(the_repository->objects, oid, &info, 0))
			/* Missing; assume it is a promisor object */
			अनुवर्त;
		यदि (source_info.source->type == ODB_SOURCE_PACKED &&
		    source_info.u.packed.pack->pack_promisor)
			अनुवर्त;

		यदि (!cmd.args.nr) {
			base_name = mkpathdup(
				"%s/pack/pack",
				repo_get_object_directory(the_repository));
			strvec_धकेलो(&cmd.args, "pack-objects");
			strvec_धकेलो(&cmd.args,
				    "--exclude-promisor-objects-best-effort");
			strvec_धकेलो(&cmd.args, base_name);
			cmd.git_cmd = 1;
			cmd.in = -1;
			cmd.out = -1;
			यदि (start_command(&cmd))
				die(_("could not start pack-objects to repack local links"));
		}

		यदि (write_in_full(cmd.in, oid_to_hex(oid), the_hash_algo->hexsz) < 0 ||
		    write_in_full(cmd.in, "\n", 1) < 0)
			die(_("failed to feed local object to pack-objects"));
	}

	यदि (!cmd.args.nr)
		प्रत्यागम;

	close(cmd.in);

	out = xfdopen(cmd.out, "r");
	यावत् (strbuf_getline_lf(&पंक्ति, out) != EOF) {
		unsigned वर्ण द्विआधारी[GIT_MAX_RAWSZ];
		यदि (पंक्ति.len != the_hash_algo->hexsz ||
		    !hex_to_bytes(द्विआधारी, पंक्ति.buf, पंक्ति.len))
			die(_("index-pack: Expecting full hex object ID lines only from pack-objects."));

		/*
		 * pack-objects creates the .pack and .idx files, but not the
		 * .promisor file. Create the .promisor file, which is empty.
		 */
		write_special_file("promisor", "", रिक्त, द्विआधारी, रिक्त);
	}

	fclose(out);
	यदि (finish_command(&cmd))
		die(_("could not finish pack-objects to repack local links"));
	strbuf_release(&पंक्ति);
	मुक्त(base_name);
}

पूर्णाङ्क cmd_index_pack(पूर्णाङ्क argc,
		   ध्रुव वर्ण **argv,
		   ध्रुव वर्ण *prefix,
		   संरचना repository *repo)
{
	पूर्णाङ्क i, fix_thin_pack = 0, verify = 0, stat_only = 0, rev_index;
	ध्रुव वर्ण *curr_index;
	वर्ण *curr_rev_index = रिक्त;
	ध्रुव वर्ण *index_name = रिक्त, *pack_name = रिक्त, *rev_index_name = रिक्त;
	ध्रुव वर्ण *keep_संदेश = रिक्त;
	ध्रुव वर्ण *promisor_संदेश = रिक्त;
	संरचना strbuf index_name_buf = STRBUF_INIT;
	संरचना strbuf rev_index_name_buf = STRBUF_INIT;
	संरचना pack_idx_entry **idx_objects;
	संरचना pack_idx_option opts;
	unsigned वर्ण pack_hash[GIT_MAX_RAWSZ];
	unsigned foreign_nr = 1;	/* zero is a "good" value, assume bad */
	पूर्णाङ्क report_end_of_निवेश = 0;
	पूर्णाङ्क hash_algo = 0;

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
	यदि (prefix && chdir(prefix))
		die(_("Cannot come back to cwd"));

	यदि (git_env_bool(GIT_TEST_NO_WRITE_REV_INDEX, 0))
		rev_index = 0;
	अन्यथा
		rev_index = !!(opts.flags & (WRITE_REV_VERIFY | WRITE_REV));

	प्रति (i = 1; i < argc; i++) {
		ध्रुव वर्ण *arg = argv[i];

		यदि (*arg == '-') {
			यदि (!strcmp(arg, "--stdin")) {
				from_stdin = 1;
			} अन्यथा यदि (!strcmp(arg, "--fix-thin")) {
				fix_thin_pack = 1;
			} अन्यथा यदि (skip_to_optional_arg(arg, "--strict", &arg)) {
				strict = 1;
				do_fsck_object = 1;
				fsck_set_संदेश_types(&fsck_options, arg);
			} अन्यथा यदि (!strcmp(arg, "--check-self-contained-and-connected")) {
				strict = 1;
				check_self_contained_and_connected = 1;
			} अन्यथा यदि (skip_to_optional_arg(arg, "--fsck-objects", &arg)) {
				do_fsck_object = 1;
				fsck_set_संदेश_types(&fsck_options, arg);
			} अन्यथा यदि (!strcmp(arg, "--verify")) {
				verify = 1;
			} अन्यथा यदि (!strcmp(arg, "--verify-stat")) {
				verify = 1;
				show_stat = 1;
			} अन्यथा यदि (!strcmp(arg, "--verify-stat-only")) {
				verify = 1;
				show_stat = 1;
				stat_only = 1;
			} अन्यथा यदि (skip_to_optional_arg(arg, "--keep", &keep_संदेश)) {
				; /* nothing to do */
			} अन्यथा यदि (skip_to_optional_arg(arg, "--promisor", &promisor_संदेश)) {
				record_outgoing_links = 1;
			} अन्यथा यदि (starts_with(arg, "--threads=")) {
				वर्ण *end;
				nr_threads = strtoul(arg+10, &end, 0);
				यदि (!arg[10] || *end || nr_threads < 0)
					usage(index_pack_usage);
				यदि (!HAVE_THREADS && nr_threads != 1) {
					warning(_("no threads support, ignoring %s"), arg);
					nr_threads = 1;
				}
			} अन्यथा यदि (skip_prefix(arg, "--pack_header=", &arg)) {
				यदि (parse_pack_header_option(arg,
							     निवेश_buffer,
							     &निवेश_len) < 0)
					die(_("bad --pack_header: %s"), arg);
			} अन्यथा यदि (!strcmp(arg, "-v")) {
				verbose = 1;
			} अन्यथा यदि (!strcmp(arg, "--progress-title")) {
				यदि (progress_title || (i+1) >= argc)
					usage(index_pack_usage);
				progress_title = argv[++i];
			} अन्यथा यदि (!strcmp(arg, "--show-resolving-progress")) {
				show_resolving_progress = 1;
			} अन्यथा यदि (!strcmp(arg, "--report-end-of-input")) {
				report_end_of_निवेश = 1;
			} अन्यथा यदि (!strcmp(arg, "-o")) {
				यदि (index_name || (i+1) >= argc)
					usage(index_pack_usage);
				index_name = argv[++i];
			} अन्यथा यदि (starts_with(arg, "--index-version=")) {
				वर्ण *c;
				opts.version = strtoul(arg + 16, &c, 10);
				यदि (opts.version > 2)
					die(_("bad %s"), arg);
				यदि (*c == ',')
					opts.off32_limit = strtoul(c+1, &c, 0);
				यदि (*c || opts.off32_limit & 0x80000000)
					die(_("bad %s"), arg);
			} अन्यथा यदि (skip_prefix(arg, "--max-input-size=", &arg)) {
				max_निवेश_size = strtoumax(arg, रिक्त, 10);
			} अन्यथा यदि (skip_prefix(arg, "--object-format=", &arg)) {
				hash_algo = hash_algo_by_name(arg);
				यदि (hash_algo == GIT_HASH_UNKNOWN)
					die(_("unknown hash algorithm '%s'"), arg);
				repo_set_hash_algo(the_repository, hash_algo);
			} अन्यथा यदि (!strcmp(arg, "--rev-index")) {
				rev_index = 1;
			} अन्यथा यदि (!strcmp(arg, "--no-rev-index")) {
				rev_index = 0;
			} अन्यथा
				usage(index_pack_usage);
			अनुवर्त;
		}

		यदि (pack_name)
			usage(index_pack_usage);
		pack_name = arg;
	}

	यदि (!pack_name && !from_stdin)
		usage(index_pack_usage);
	यदि (fix_thin_pack && !from_stdin)
		die(_("the option '%s' requires '%s'"), "--fix-thin", "--stdin");
	यदि (promisor_संदेश && pack_name)
		die(_("--promisor cannot be used with a pack name"));
	यदि (from_stdin && !startup_info->have_repository)
		die(_("--stdin requires a git repository"));
	यदि (from_stdin && hash_algo)
		die(_("options '%s' and '%s' cannot be used together"), "--object-format", "--stdin");
	यदि (!index_name && pack_name)
		index_name = derive_filename(pack_name, "pack", "idx", &index_name_buf);

	/*
	 * Packfiles and indices do not carry enough information to be able to
	 * identify their object hash. So when we are neither in a repository
	 * nor has the user told us which object hash to use we have no other
	 * choice but to guess the object hash.
	 */
	यदि (!the_repository->hash_algo)
		repo_set_hash_algo(the_repository, GIT_HASH_DEFAULT);

	opts.flags &= ~(WRITE_REV | WRITE_REV_VERIFY);
	यदि (rev_index) {
		opts.flags |= verify ? WRITE_REV_VERIFY : WRITE_REV;
		यदि (index_name)
			rev_index_name = derive_filename(index_name,
							 "idx", "rev",
							 &rev_index_name_buf);
	}

	यदि (verify) {
		यदि (!index_name)
			die(_("--verify with no packfile name given"));
		read_idx_option(&opts, index_name);
		opts.flags |= WRITE_IDX_VERIFY | WRITE_IDX_STRICT;
	}
	यदि (strict)
		opts.flags |= WRITE_IDX_STRICT;

	यदि (HAVE_THREADS && !nr_threads) {
		nr_threads = online_cpus();
		/*
		 * Experiments show that going above 20 threads doesn't help,
		 * no matter how many cores you have. Below that, we tend to
		 * max at half the number of online_cpus(), presumably because
		 * half of those are hyperthreads rather than full cores. We'll
		 * never reduce the level below "3", though, to match a
		 * historical value that nobody complained about.
		 */
		यदि (nr_threads < 4)
			; /* too few cores to consider capping */
		अन्यथा यदि (nr_threads < 6)
			nr_threads = 3; /* historic cap */
		अन्यथा यदि (nr_threads < 40)
			nr_threads /= 2;
		अन्यथा
			nr_threads = 20; /* hard cap */
	}

	curr_pack = open_pack_file(pack_name);
	parse_pack_header();
	CALLOC_ARRAY(objects, st_add(nr_objects, 1));
	यदि (show_stat)
		CALLOC_ARRAY(obj_stat, st_add(nr_objects, 1));
	CALLOC_ARRAY(ofs_deltas, nr_objects);
	parse_pack_objects(pack_hash);
	यदि (report_end_of_निवेश)
		write_in_full(2, "\0", 1);
	resolve_deltas(&opts);
	conclude_pack(fix_thin_pack, curr_pack, pack_hash);
	मुक्त(ofs_deltas);
	मुक्त(ref_deltas);
	यदि (strict)
		foreign_nr = check_objects();

	यदि (show_stat)
		show_pack_info(stat_only);

	ALLOC_ARRAY(idx_objects, nr_objects);
	प्रति (i = 0; i < nr_objects; i++)
		idx_objects[i] = &objects[i].idx;
	curr_index = write_idx_file(the_repository, index_name, idx_objects,
				    nr_objects, &opts, pack_hash);
	यदि (rev_index)
		curr_rev_index = write_rev_file(the_repository, rev_index_name,
						idx_objects, nr_objects,
						pack_hash, opts.flags);
	मुक्त(idx_objects);

	यदि (!verify)
		final(pack_name, curr_pack,
		      index_name, curr_index,
		      rev_index_name, curr_rev_index,
		      keep_संदेश, promisor_संदेश,
		      pack_hash);
	अन्यथा
		close(निवेश_fd);

	यदि (do_fsck_object) {
		/*
		 * We cannot perform queued consistency checks when running
		 * outside of a repository because those require us to read
		 * from the object database, which is uninitialized.
		 *
		 * TODO: we may eventually set up an in-memory object database,
		 * which would allow us to perform these queued checks.
		 */
		यदि (!startup_info->have_repository &&
		    fsck_has_queued_checks(&fsck_options))
			die(_("cannot perform queued object checks outside "
			      "of a repository"));

		यदि (fsck_finish(&fsck_options))
			die(_("fsck error in pack objects"));
	}

	मुक्त(opts.anomaly);
	मुक्त(objects);
	strbuf_release(&index_name_buf);
	strbuf_release(&rev_index_name_buf);
	यदि (!pack_name)
		मुक्त((शून्य *) curr_pack);
	यदि (!index_name)
		मुक्त((शून्य *) curr_index);
	मुक्त(curr_rev_index);

	repack_local_links();

	/*
	 * Let the caller know this pack is not self contained
	 */
	यदि (check_self_contained_and_connected && foreign_nr)
		प्रत्यागम 1;

	प्रत्यागम 0;
}
