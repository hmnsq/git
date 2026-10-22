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

स्थिर स्थिरांक अक्षर index_pack_usage[] =
"git index-pack [-v] [-o <index-file>] [--keep | --keep=<msg>] [--[no-]rev-index] [--verify] [--strict[=<msg-id>=<severity>...]] [--fsck-objects[=<msg-id>=<severity>...]] (<pack-file> | --stdin [--fix-thin] [<pack-file>])";

संरचना object_entry {
	संरचना pack_idx_entry idx;
	माप_प्रकार size;
	अचिन्हित अक्षर hdr_size;
	चिन्हित अक्षर type;
	चिन्हित अक्षर real_type;
};

संरचना object_stat {
	अचिन्हित delta_depth;
	पूर्णांक base_object_no;
};

संरचना base_data {
	/* Initialized by make_base(). */
	संरचना base_data *base;
	संरचना object_entry *obj;
	पूर्णांक ref_first, ref_last;
	पूर्णांक ofs_first, ofs_last;
	/*
	 * Threads should increment retain_data if they are about to call
	 * patch_delta() using this struct's data as a base, and decrement this
	 * when they are done. While retain_data is nonzero, this struct's data
	 * will not be freed even if the delta base cache limit is exceeded.
	 */
	पूर्णांक retain_data;
	/*
	 * The number of direct children that have not been fully processed
	 * (entered work_head, entered done_head, left done_head). When this
	 * number reaches zero, this struct base_data can be freed.
	 */
	पूर्णांक children_remaining;

	/* Not initialized by make_base(). */
	संरचना list_head list;
	शून्य *data;
	माप_प्रकार size;
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
स्थिर माप_प्रकार base_cache_used;
स्थिर माप_प्रकार base_cache_limit;

संरचना thread_local_data {
	pthread_t thread;
	पूर्णांक pack_fd;
};

/* Remember to update object flag allocation in object.h */
#define FLAG_LINK (1u<<20)
#define FLAG_CHECKED (1u<<21)

संरचना ofs_delta_entry {
	off_t offset;
	पूर्णांक obj_no;
};

संरचना ref_delta_entry {
	संरचना object_id oid;
	पूर्णांक obj_no;
};

स्थिर संरचना object_entry *objects;
स्थिर संरचना object_stat *obj_stat;
स्थिर संरचना ofs_delta_entry *ofs_deltas;
स्थिर संरचना ref_delta_entry *ref_deltas;
स्थिर संरचना thread_local_data nothread_data;
स्थिर पूर्णांक nr_objects;
स्थिर पूर्णांक nr_ofs_deltas;
स्थिर पूर्णांक nr_ref_deltas;
स्थिर पूर्णांक ref_deltas_alloc;
स्थिर पूर्णांक nr_resolved_deltas;
स्थिर पूर्णांक nr_threads;

स्थिर पूर्णांक from_stdin;
स्थिर पूर्णांक strict;
स्थिर पूर्णांक do_fsck_object;
स्थिर संरचना fsck_options fsck_options;
स्थिर पूर्णांक verbose;
स्थिर स्थिरांक अक्षर *progress_title;
स्थिर पूर्णांक show_resolving_progress;
स्थिर पूर्णांक show_stat;
स्थिर पूर्णांक check_self_contained_and_connected;

स्थिर संरचना progress *progress;

स्थिर अचिन्हित अक्षर आदान_buffer[DEFAULT_IO_BUFFER_SIZE];
स्थिर अचिन्हित पूर्णांक आदान_offset, आदान_len;
स्थिर off_t consumed_bytes;
स्थिर off_t max_आदान_size;
स्थिर अचिन्हित deepest_delta;
स्थिर संरचना git_hash_ctx आदान_ctx;
स्थिर uint32_t आदान_crc32;
स्थिर पूर्णांक आदान_fd, output_fd;
स्थिर स्थिरांक अक्षर *curr_pack;

/*
 * outgoing_links is guarded by read_mutex, and record_outgoing_links is
 * read-only in a thread.
 */
स्थिर संरचना oidset outgoing_links = OIDSET_INIT;
स्थिर पूर्णांक record_outgoing_links;

स्थिर संरचना thread_local_data *thread_data;
स्थिर पूर्णांक nr_dispatched;
स्थिर पूर्णांक threads_active;

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

स्थिर pthread_किल्ली_t किल्ली;

स्थिर ओळीत शून्य lock_mutex(pthread_mutex_t *mutex)
{
	जर (threads_active)
		pthread_mutex_lock(mutex);
}

स्थिर ओळीत शून्य unlock_mutex(pthread_mutex_t *mutex)
{
	जर (threads_active)
		pthread_mutex_unlock(mutex);
}

/*
 * Mutex and conditional variable can't be statically-initialized on Windows.
 */
स्थिर शून्य init_thread(शून्य)
{
	पूर्णांक i;
	init_recursive_mutex(&read_mutex);
	pthread_mutex_init(&counter_mutex, रिक्त);
	pthread_mutex_init(&work_mutex, रिक्त);
	जर (show_stat)
		pthread_mutex_init(&deepest_delta_mutex, रिक्त);
	pthread_किल्ली_create(&किल्ली, रिक्त);
	CALLOC_ARRAY(thread_data, nr_threads);
	साठी (i = 0; i < nr_threads; i++) {
		thread_data[i].pack_fd = xopen(curr_pack, O_RDONLY);
	}

	threads_active = 1;
}

स्थिर शून्य cleanup_thread(शून्य)
{
	पूर्णांक i;
	जर (!threads_active)
		परत;
	threads_active = 0;
	pthread_mutex_destroy(&read_mutex);
	pthread_mutex_destroy(&counter_mutex);
	pthread_mutex_destroy(&work_mutex);
	जर (show_stat)
		pthread_mutex_destroy(&deepest_delta_mutex);
	साठी (i = 0; i < nr_threads; i++)
		बंद(thread_data[i].pack_fd);
	pthread_किल्ली_delete(किल्ली);
	मुक्त(thread_data);
}

स्थिर पूर्णांक mark_link(संरचना object *obj, गणना object_type type,
		     शून्य *data UNUSED,
		     संरचना fsck_options *options UNUSED)
{
	जर (!obj)
		परत -1;

	जर (type != OBJ_ANY && obj->type != type)
		die(_("object type mismatch at %s"), oid_to_hex(&obj->oid));

	obj->flags |= FLAG_LINK;
	परत 0;
}

/* The content of each linked object must have been checked
   or it must be already present in the object database */
स्थिर अचिन्हित check_object(संरचना object *obj)
{
	जर (!obj)
		परत 0;

	जर (!(obj->flags & FLAG_LINK))
		परत 0;

	जर (!(obj->flags & FLAG_CHECKED)) {
		माप_प्रकार size;
		पूर्णांक type = odb_read_object_info(the_repository->objects,
						&obj->oid, &size);
		जर (type <= 0)
			die(_("did not receive expected object %s"),
			      oid_to_hex(&obj->oid));
		जर (type != obj->type)
			die(_("object %s: expected type %s, found %s"),
			    oid_to_hex(&obj->oid),
			    type_name(obj->type), type_name(type));
		obj->flags |= FLAG_CHECKED;
		परत 1;
	}

	परत 0;
}

स्थिर अचिन्हित check_objects(शून्य)
{
	अचिन्हित i, max, foreign_nr = 0;

	max = get_max_object_index(the_repository);

	जर (verbose)
		progress = start_delayed_progress(the_repository,
						  _("Checking objects"), max);

	साठी (i = 0; i < max; i++) {
		foreign_nr += check_object(get_indexed_object(the_repository, i));
		display_progress(progress, i + 1);
	}

	stop_progress(&progress);
	परत foreign_nr;
}


/* Discard current buffer used content. */
स्थिर शून्य flush(शून्य)
{
	जर (आदान_offset) {
		जर (output_fd >= 0)
			write_or_die(output_fd, आदान_buffer, आदान_offset);
		git_hash_update(&आदान_ctx, आदान_buffer, आदान_offset);
		स_हटाओ(आदान_buffer, आदान_buffer + आदान_offset, आदान_len);
		आदान_offset = 0;
	}
}

/*
 * Make sure at least "min" bytes are available in the buffer, and
 * return the pointer to the buffer.
 */
स्थिर शून्य *fill(पूर्णांक min)
{
	जर (min <= आदान_len)
		परत आदान_buffer + आदान_offset;
	जर (min > आकार(आदान_buffer))
		die(Q_("cannot fill %d byte",
		       "cannot fill %d bytes",
		       min),
		    min);
	flush();
	करा {
		ssize_t ret = xread(आदान_fd, आदान_buffer + आदान_len,
				आकार(आदान_buffer) - आदान_len);
		जर (ret <= 0) {
			जर (!ret)
				die(_("early EOF"));
			die_errno(_("read error on input"));
		}
		आदान_len += ret;
		जर (from_stdin)
			display_throughput(progress, consumed_bytes + आदान_len);
	} पर्यंत (आदान_len < min);
	परत आदान_buffer;
}

स्थिर शून्य use(पूर्णांक bytes)
{
	जर (bytes > आदान_len)
		die(_("used more bytes than were available"));
	आदान_crc32 = crc32(आदान_crc32, आदान_buffer + आदान_offset, bytes);
	आदान_len -= bytes;
	आदान_offset += bytes;

	/* make sure off_t is sufficiently large not to wrap */
	जर (signed_add_overflows(consumed_bytes, bytes))
		die(_("pack too large for current definition of off_t"));
	consumed_bytes += bytes;
	जर (max_आदान_size && consumed_bytes > max_आदान_size) {
		संरचना strbuf size_limit = STRBUF_INIT;
		strbuf_humanise_bytes(&size_limit, max_आदान_size);
		die(_("pack exceeds maximum allowed size (%s)"),
		    size_limit.buf);
	}
}

स्थिर स्थिरांक अक्षर *open_pack_file(स्थिरांक अक्षर *pack_name)
{
	जर (from_stdin) {
		आदान_fd = 0;
		जर (!pack_name) {
			संरचना strbuf tmp_file = STRBUF_INIT;
			output_fd = odb_mkstemp(the_repository->objects, &tmp_file,
						"pack/tmp_pack_XXXXXX");
			pack_name = strbuf_detach(&tmp_file, रिक्त);
		} अन्यथा {
			output_fd = xopen(pack_name, O_CREAT|O_EXCL|O_RDWR, 0600);
		}
		nothread_data.pack_fd = output_fd;
	} अन्यथा {
		आदान_fd = xopen(pack_name, O_RDONLY);
		output_fd = -1;
		nothread_data.pack_fd = आदान_fd;
	}
	git_hash_init(&आदान_ctx, the_hash_algo);
	परत pack_name;
}

स्थिर शून्य parse_pack_header(शून्य)
{
	अचिन्हित अक्षर *hdr = fill(आकार(संरचना pack_header));

	/* Header consistency check */
	जर (get_be32(hdr) != PACK_SIGNATURE)
		die(_("pack signature mismatch"));
	hdr += 4;
	जर (!pack_version_ok_native(get_be32(hdr)))
		die(_("pack version %"PRIu32" unsupported"),
		    get_be32(hdr));
	hdr += 4;

	nr_objects = get_be32(hdr);
	use(आकार(संरचना pack_header));
}

__attribute__((format (म_लिखो, 2, 3)))
स्थिर NORETURN शून्य bad_object(off_t offset, स्थिरांक अक्षर *format, ...)
{
	बहु_सूची params;
	अक्षर buf[1024];

	बहु_शुरू(params, format);
	vsnprintf(buf, आकार(buf), format, params);
	बहु_पूर्ण(params);
	die(_("pack has bad object at offset %"PRIuMAX": %s"),
	    (uintmax_t)offset, buf);
}

स्थिर ओळीत संरचना thread_local_data *get_thread_data(शून्य)
{
	जर (HAVE_THREADS) {
		जर (threads_active)
			परत pthread_getspecific(किल्ली);
		प्रतिज्ञा(!threads_active &&
		       "This should only be reached when all threads are gone");
	}
	परत &nothread_data;
}

स्थिर शून्य set_thread_data(संरचना thread_local_data *data)
{
	जर (threads_active)
		pthread_setspecific(किल्ली, data);
}

स्थिर शून्य free_base_data(संरचना base_data *c)
{
	जर (c->data) {
		FREE_AND_NULL(c->data);
		base_cache_used -= c->size;
	}
}

स्थिर शून्य prune_base_data(संरचना base_data *retain)
{
	संरचना list_head *pos;

	जर (base_cache_used <= base_cache_limit)
		परत;

	list_for_each_prev(pos, &done_head) {
		संरचना base_data *b = list_entry(pos, संरचना base_data, list);
		जर (b->retain_data || b == retain)
			सुरूठेवा;
		जर (b->data) {
			free_base_data(b);
			जर (base_cache_used <= base_cache_limit)
				परत;
		}
	}

	list_for_each_prev(pos, &work_head) {
		संरचना base_data *b = list_entry(pos, संरचना base_data, list);
		जर (b->retain_data || b == retain)
			सुरूठेवा;
		जर (b->data) {
			free_base_data(b);
			जर (base_cache_used <= base_cache_limit)
				परत;
		}
	}
}

स्थिर पूर्णांक is_delta_type(गणना object_type type)
{
	परत (type == OBJ_REF_DELTA || type == OBJ_OFS_DELTA);
}

स्थिर शून्य *unpack_entry_data(off_t offset, माप_प्रकार size,
			       गणना object_type type, संरचना object_id *oid)
{
	स्थिर अक्षर fixed_buf[8192];
	पूर्णांक status;
	git_zstream stream;
	शून्य *buf;
	संरचना git_hash_ctx c;
	अक्षर hdr[32];
	पूर्णांक hdrlen;

	जर (!is_delta_type(type)) {
		hdrlen = format_object_header(hdr, आकार(hdr), type, size);
		git_hash_init(&c, the_hash_algo);
		git_hash_update(&c, hdr, hdrlen);
	} अन्यथा
		oid = रिक्त;
	जर (type == OBJ_BLOB &&
	    size > repo_settings_get_big_file_threshold(the_repository))
		buf = fixed_buf;
	अन्यथा
		buf = xmallocz(size);

	स_रखो(&stream, 0, आकार(stream));
	git_inflate_init(&stream);
	stream.next_out = buf;
	stream.avail_out = buf == fixed_buf ? आकार(fixed_buf) : size;

	करा {
		अचिन्हित अक्षर *last_out = stream.next_out;
		stream.next_in = fill(1);
		stream.avail_in = आदान_len;
		status = git_inflate(&stream, 0);
		use(आदान_len - stream.avail_in);
		जर (oid)
			git_hash_update(&c, last_out, stream.next_out - last_out);
		जर (buf == fixed_buf) {
			stream.next_out = buf;
			stream.avail_out = आकार(fixed_buf);
		}
	} पर्यंत (status == Z_OK);
	जर (stream.total_out != size || status != Z_STREAM_END)
		bad_object(offset, _("inflate returned %d"), status);
	git_inflate_end(&stream);
	जर (oid)
		git_hash_final_oid(oid, &c);
	परत buf == fixed_buf ? रिक्त : buf;
}

स्थिर शून्य *unpack_raw_entry(संरचना object_entry *obj,
			      off_t *ofs_offset,
			      संरचना object_id *ref_oid,
			      संरचना object_id *oid)
{
	अचिन्हित अक्षर *p;
	माप_प्रकार size, c;
	off_t base_offset;
	अचिन्हित shift;
	शून्य *data;

	obj->idx.offset = consumed_bytes;
	आदान_crc32 = crc32(0, रिक्त, 0);

	p = fill(1);
	c = *p;
	use(1);
	obj->type = (c >> 4) & 7;
	size = (c & 15);
	shift = 4;
	पर्यंत (c & 0x80) {
		जर ((bitsizeof(माप_प्रकार) - 7) < shift)
			die(_("object size too large for this platform"));
		p = fill(1);
		c = *p;
		use(1);
		size += (c & 0x7f) << shift;
		shift += 7;
	}
	obj->size = size;

	निवड (obj->type) {
	स्थिती OBJ_REF_DELTA:
		oidread(ref_oid, fill(the_hash_algo->rawsz),
			the_repository->hash_algo);
		use(the_hash_algo->rawsz);
		थांबा;
	स्थिती OBJ_OFS_DELTA:
		p = fill(1);
		c = *p;
		use(1);
		base_offset = c & 127;
		पर्यंत (c & 128) {
			base_offset += 1;
			जर (!base_offset || MSB(base_offset, 7))
				bad_object(obj->idx.offset, _("offset value overflow for delta base object"));
			p = fill(1);
			c = *p;
			use(1);
			base_offset = (base_offset << 7) + (c & 127);
		}
		*ofs_offset = obj->idx.offset - base_offset;
		जर (*ofs_offset <= 0 || *ofs_offset >= obj->idx.offset)
			bad_object(obj->idx.offset, _("delta base offset is out of bound"));
		थांबा;
	स्थिती OBJ_COMMIT:
	स्थिती OBJ_वृक्ष:
	स्थिती OBJ_BLOB:
	स्थिती OBJ_TAG:
		थांबा;
	पूर्वनिर्धारित:
		bad_object(obj->idx.offset, _("unknown object type %d"), obj->type);
	}
	obj->hdr_size = consumed_bytes - obj->idx.offset;

	data = unpack_entry_data(obj->idx.offset, obj->size, obj->type, oid);
	obj->idx.crc32 = आदान_crc32;
	परत data;
}

स्थिर शून्य *unpack_data(संरचना object_entry *obj,
			 पूर्णांक (*consume)(स्थिरांक अचिन्हित अक्षर *, अचिन्हित दीर्घ, शून्य *),
			 शून्य *cb_data)
{
	off_t from = obj[0].idx.offset + obj[0].hdr_size;
	off_t len = obj[1].idx.offset - from;
	अचिन्हित अक्षर *data, *inbuf;
	git_zstream stream;
	पूर्णांक status;

	data = xmallocz(consume ? 64*1024 : obj->size);
	inbuf = xmalloc((len < 64*1024) ? (पूर्णांक)len : 64*1024);

	स_रखो(&stream, 0, आकार(stream));
	git_inflate_init(&stream);
	stream.next_out = data;
	stream.avail_out = consume ? 64*1024 : obj->size;

	करा {
		ssize_t n = (len < 64*1024) ? (ssize_t)len : 64*1024;
		n = xpread(get_thread_data()->pack_fd, inbuf, n, from);
		जर (n < 0)
			die_errno(_("cannot pread pack file"));
		जर (!n)
			die(Q_("premature end of pack file, %"PRIuMAX" byte missing",
			       "premature end of pack file, %"PRIuMAX" bytes missing",
			       len),
			    (uintmax_t)len);
		from += n;
		len -= n;
		stream.next_in = inbuf;
		stream.avail_in = n;
		जर (!consume)
			status = git_inflate(&stream, 0);
		अन्यथा {
			करा {
				status = git_inflate(&stream, 0);
				जर (consume(data, stream.next_out - data, cb_data)) {
					मुक्त(inbuf);
					मुक्त(data);
					परत रिक्त;
				}
				stream.next_out = data;
				stream.avail_out = 64*1024;
			} पर्यंत (status == Z_OK && stream.avail_in);
		}
	} पर्यंत (len && status == Z_OK && !stream.avail_in);

	/* This has been inflated OK when first encountered, so... */
	जर (status != Z_STREAM_END || stream.total_out != obj->size)
		die(_("serious inflate inconsistency"));

	git_inflate_end(&stream);
	मुक्त(inbuf);
	जर (consume) {
		FREE_AND_NULL(data);
	}
	परत data;
}

स्थिर शून्य *get_data_from_pack(संरचना object_entry *obj)
{
	परत unpack_data(obj, रिक्त, रिक्त);
}

स्थिर पूर्णांक compare_ofs_delta_bases(off_t offset1, off_t offset2,
				   गणना object_type type1,
				   गणना object_type type2)
{
	पूर्णांक cmp = type1 - type2;
	जर (cmp)
		परत cmp;
	परत offset1 < offset2 ? -1 :
	       offset1 > offset2 ?  1 :
	       0;
}

स्थिर पूर्णांक find_ofs_delta(स्थिरांक off_t offset)
{
	पूर्णांक first = 0, last = nr_ofs_deltas;

	पर्यंत (first < last) {
		पूर्णांक next = first + (last - first) / 2;
		संरचना ofs_delta_entry *delta = &ofs_deltas[next];
		पूर्णांक cmp;

		cmp = compare_ofs_delta_bases(offset, delta->offset,
					      OBJ_OFS_DELTA,
					      objects[delta->obj_no].type);
		जर (!cmp)
			परत next;
		जर (cmp < 0) {
			last = next;
			सुरूठेवा;
		}
		first = next+1;
	}
	परत -first-1;
}

स्थिर शून्य find_ofs_delta_children(off_t offset,
				    पूर्णांक *first_index, पूर्णांक *last_index)
{
	पूर्णांक first = find_ofs_delta(offset);
	पूर्णांक last = first;
	पूर्णांक end = nr_ofs_deltas - 1;

	जर (first < 0) {
		*first_index = 0;
		*last_index = -1;
		परत;
	}
	पर्यंत (first > 0 && ofs_deltas[first - 1].offset == offset)
		--first;
	पर्यंत (last < end && ofs_deltas[last + 1].offset == offset)
		++last;
	*first_index = first;
	*last_index = last;
}

स्थिर पूर्णांक compare_ref_delta_bases(स्थिरांक संरचना object_id *oid1,
				   स्थिरांक संरचना object_id *oid2,
				   गणना object_type type1,
				   गणना object_type type2)
{
	पूर्णांक cmp = type1 - type2;
	जर (cmp)
		परत cmp;
	परत oidcmp(oid1, oid2);
}

स्थिर पूर्णांक find_ref_delta(स्थिरांक संरचना object_id *oid)
{
	पूर्णांक first = 0, last = nr_ref_deltas;

	पर्यंत (first < last) {
		पूर्णांक next = first + (last - first) / 2;
		संरचना ref_delta_entry *delta = &ref_deltas[next];
		पूर्णांक cmp;

		cmp = compare_ref_delta_bases(oid, &delta->oid,
					      OBJ_REF_DELTA,
					      objects[delta->obj_no].type);
		जर (!cmp)
			परत next;
		जर (cmp < 0) {
			last = next;
			सुरूठेवा;
		}
		first = next+1;
	}
	परत -first-1;
}

स्थिर शून्य find_ref_delta_children(स्थिरांक संरचना object_id *oid,
				    पूर्णांक *first_index, पूर्णांक *last_index)
{
	पूर्णांक first = find_ref_delta(oid);
	पूर्णांक last = first;
	पूर्णांक end = nr_ref_deltas - 1;

	जर (first < 0) {
		*first_index = 0;
		*last_index = -1;
		परत;
	}
	पर्यंत (first > 0 && oideq(&ref_deltas[first - 1].oid, oid))
		--first;
	पर्यंत (last < end && oideq(&ref_deltas[last + 1].oid, oid))
		++last;
	*first_index = first;
	*last_index = last;
}

संरचना compare_data {
	संरचना object_entry *entry;
	संरचना odb_stream *st;
	अचिन्हित अक्षर *buf;
	अचिन्हित दीर्घ buf_size;
};

स्थिर पूर्णांक compare_objects(स्थिरांक अचिन्हित अक्षर *buf, अचिन्हित दीर्घ size,
			   शून्य *cb_data)
{
	संरचना compare_data *data = cb_data;

	जर (data->buf_size < size) {
		मुक्त(data->buf);
		data->buf = xmalloc(size);
		data->buf_size = size;
	}

	पर्यंत (size) {
		ssize_t len = odb_stream_read(data->st, data->buf, size);
		जर (len == 0)
			die(_("SHA1 COLLISION FOUND WITH %s !"),
			    oid_to_hex(&data->entry->idx.oid));
		जर (len < 0)
			die(_("unable to read %s"),
			    oid_to_hex(&data->entry->idx.oid));
		जर (स_भेद(buf, data->buf, len))
			die(_("SHA1 COLLISION FOUND WITH %s !"),
			    oid_to_hex(&data->entry->idx.oid));
		size -= len;
		buf += len;
	}
	परत 0;
}

स्थिर पूर्णांक check_collison(संरचना object_entry *entry)
{
	संरचना compare_data data;

	जर (entry->size <= repo_settings_get_big_file_threshold(the_repository) ||
	    entry->type != OBJ_BLOB)
		परत -1;

	स_रखो(&data, 0, आकार(data));
	data.entry = entry;
	data.st = odb_stream_from_object(the_repository->objects, &entry->idx.oid, रिक्त);
	जर (!data.st)
		परत -1;
	जर (data.st->size != entry->size || data.st->type != entry->type)
		die(_("SHA1 COLLISION FOUND WITH %s !"),
		    oid_to_hex(&entry->idx.oid));
	unpack_data(entry, compare_objects, &data);
	odb_stream_close(data.st);
	मुक्त(data.buf);
	परत 0;
}

स्थिर शून्य record_outgoing_link(स्थिरांक संरचना object_id *oid)
{
	oidset_घाला(&outgoing_links, oid);
}

स्थिर शून्य maybe_record_name_entry(स्थिरांक संरचना name_entry *entry)
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
	जर (S_ISDIR(entry->mode))
		record_outgoing_link(&entry->oid);
}

स्थिर शून्य do_record_outgoing_links(संरचना object *obj)
{
	जर (obj->type == OBJ_वृक्ष) {
		संरचना वृक्ष *वृक्ष = (संरचना वृक्ष *)obj;
		संरचना वृक्ष_desc desc;
		संरचना name_entry entry;
		जर (init_वृक्ष_desc_gently(&desc, &वृक्ष->object.oid,
					  वृक्ष->buffer, वृक्ष->size, 0))
			/*
			 * Error messages are given when packs are
			 * verified, so do not print any here.
			 */
			परत;
		पर्यंत (वृक्ष_entry_gently(&desc, &entry))
			maybe_record_name_entry(&entry);
	} अन्यथा जर (obj->type == OBJ_COMMIT) {
		संरचना commit *commit = (संरचना commit *) obj;
		संरचना commit_list *parents = commit->parents;

		record_outgoing_link(get_commit_वृक्ष_oid(commit));
		साठी (; parents; parents = parents->next)
			record_outgoing_link(&parents->item->object.oid);
	} अन्यथा जर (obj->type == OBJ_TAG) {
		संरचना tag *tag = (संरचना tag *) obj;
		record_outgoing_link(get_tagged_oid(tag));
	}
}

स्थिर शून्य sha1_object(स्थिरांक शून्य *data, संरचना object_entry *obj_entry,
			अचिन्हित दीर्घ size, गणना object_type type,
			स्थिरांक संरचना object_id *oid)
{
	शून्य *new_data = रिक्त;
	पूर्णांक collision_test_needed = 0;

	प्रतिज्ञा(data || obj_entry);

	जर (startup_info->have_repository) {
		read_lock();
		collision_test_needed = odb_has_object(the_repository->objects, oid,
						       ODB_HAS_OBJECT_FETCH_PROMISOR);
		read_unlock();
	}

	जर (collision_test_needed && !data) {
		read_lock();
		जर (!check_collison(obj_entry))
			collision_test_needed = 0;
		read_unlock();
	}
	जर (collision_test_needed) {
		शून्य *has_data;
		गणना object_type has_type;
		माप_प्रकार has_size;
		read_lock();
		has_type = odb_read_object_info(the_repository->objects, oid, &has_size);
		जर (has_type < 0)
			die(_("cannot read existing object info %s"), oid_to_hex(oid));
		जर (has_type != type || has_size != size)
			die(_("SHA1 COLLISION FOUND WITH %s !"), oid_to_hex(oid));
		has_data = odb_read_object(the_repository->objects, oid,
					   &has_type, &has_size);
		read_unlock();
		जर (!data)
			data = new_data = get_data_from_pack(obj_entry);
		जर (!has_data)
			die(_("cannot read existing object %s"), oid_to_hex(oid));
		जर (size != has_size || type != has_type ||
		    स_भेद(data, has_data, size) != 0)
			die(_("SHA1 COLLISION FOUND WITH %s !"), oid_to_hex(oid));
		मुक्त(has_data);
	}

	जर (strict || do_fsck_object || record_outgoing_links) {
		read_lock();
		जर (type == OBJ_BLOB) {
			संरचना blob *blob = lookup_blob(the_repository, oid);
			जर (blob)
				blob->object.flags |= FLAG_CHECKED;
			अन्यथा
				die(_("invalid blob object %s"), oid_to_hex(oid));
			जर (do_fsck_object &&
			    fsck_object(&blob->object, (शून्य *)data, size, &fsck_options))
				die(_("fsck error in packed object"));
		} अन्यथा {
			संरचना object *obj;
			पूर्णांक eaten;
			शून्य *buf = (शून्य *) data;

			प्रतिज्ञा(data && "data can only be NULL for large _blobs_");

			/*
			 * we do not need to free the memory here, as the
			 * buf is deleted by the caller.
			 */
			obj = parse_object_buffer(the_repository, oid, type,
						  size, buf,
						  &eaten);
			जर (!obj)
				die(_("invalid %s"), type_name(type));
			जर (do_fsck_object &&
			    fsck_object(obj, buf, size, &fsck_options))
				die(_("fsck error in packed object"));
			जर (strict && fsck_walk(obj, रिक्त, &fsck_options))
				die(_("Not all child objects of %s are reachable"), oid_to_hex(&obj->oid));
			जर (record_outgoing_links)
				do_record_outgoing_links(obj);

			जर (obj->type == OBJ_वृक्ष) {
				संरचना वृक्ष *item = (संरचना वृक्ष *) obj;
				item->buffer = रिक्त;
				obj->parsed = 0;
			}
			जर (obj->type == OBJ_COMMIT) {
				संरचना commit *commit = (संरचना commit *) obj;
				जर (detach_commit_buffer(commit, रिक्त) != data)
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
	जर (!c->data) {
		संरचना object_entry *obj = c->obj;
		संरचना base_data **delta = रिक्त;
		पूर्णांक delta_nr = 0, delta_alloc = 0;

		पर्यंत (is_delta_type(c->obj->type) && !c->data) {
			ALLOC_GROW(delta, delta_nr + 1, delta_alloc);
			delta[delta_nr++] = c;
			c = c->base;
		}
		जर (!delta_nr) {
			c->data = get_data_from_pack(obj);
			c->size = obj->size;
			base_cache_used += c->size;
			prune_base_data(c);
		}
		साठी (; delta_nr > 0; delta_nr--) {
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
			जर (!c->data)
				bad_object(obj->idx.offset, _("failed to apply delta"));
			base_cache_used += c->size;
			prune_base_data(c);
		}
		मुक्त(delta);
	}
	परत c->data;
}

स्थिर संरचना base_data *make_base(संरचना object_entry *obj,
				   संरचना base_data *parent)
{
	संरचना base_data *base = xcalloc(1, आकार(संरचना base_data));
	base->base = parent;
	base->obj = obj;
	find_ref_delta_children(&obj->idx.oid,
				&base->ref_first, &base->ref_last);
	find_ofs_delta_children(obj->idx.offset,
				&base->ofs_first, &base->ofs_last);
	base->children_remaining = base->ref_last - base->ref_first +
		base->ofs_last - base->ofs_first + 2;
	परत base;
}

स्थिर संरचना base_data *resolve_delta(संरचना object_entry *delta_obj,
				       संरचना base_data *base)
{
	शून्य *delta_data, *result_data;
	संरचना base_data *result;
	माप_प्रकार result_size;

	जर (show_stat) {
		पूर्णांक i = delta_obj - objects;
		पूर्णांक j = base->obj - objects;
		obj_stat[i].delta_depth = obj_stat[j].delta_depth + 1;
		deepest_delta_lock();
		जर (deepest_delta < obj_stat[i].delta_depth)
			deepest_delta = obj_stat[i].delta_depth;
		deepest_delta_unlock();
		obj_stat[i].base_object_no = j;
	}
	delta_data = get_data_from_pack(delta_obj);
	प्रतिज्ञा(base->data);
	result_data = patch_delta(base->data, base->size,
				  delta_data, delta_obj->size, &result_size);
	मुक्त(delta_data);
	जर (!result_data)
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

	परत result;
}

स्थिर पूर्णांक compare_ofs_delta_entry(स्थिरांक शून्य *a, स्थिरांक शून्य *b)
{
	स्थिरांक संरचना ofs_delta_entry *delta_a = a;
	स्थिरांक संरचना ofs_delta_entry *delta_b = b;

	परत delta_a->offset < delta_b->offset ? -1 :
	       delta_a->offset > delta_b->offset ?  1 :
	       0;
}

स्थिर पूर्णांक compare_ref_delta_entry(स्थिरांक शून्य *a, स्थिरांक शून्य *b)
{
	स्थिरांक संरचना ref_delta_entry *delta_a = a;
	स्थिरांक संरचना ref_delta_entry *delta_b = b;

	परत oidcmp(&delta_a->oid, &delta_b->oid);
}

स्थिर शून्य *threaded_second_pass(शून्य *data)
{
	जर (data)
		set_thread_data(data);
	साठी (;;) {
		संरचना base_data *parent = रिक्त;
		संरचना object_entry *child_obj = रिक्त;
		संरचना base_data *child = रिक्त;

		counter_lock();
		display_progress(progress, nr_resolved_deltas);
		counter_unlock();

		work_lock();
		जर (list_empty(&work_head)) {
			/*
			 * Take an object from the object array.
			 */
			पर्यंत (nr_dispatched < nr_objects &&
			       is_delta_type(objects[nr_dispatched].type))
				nr_dispatched++;
			जर (nr_dispatched >= nr_objects) {
				work_unlock();
				थांबा;
			}
			child_obj = &objects[nr_dispatched++];
		} अन्यथा {
			/*
			 * Peek at the top of the stack, and take a child from
			 * it.
			 */
			parent = list_first_entry(&work_head, संरचना base_data,
						  list);

			पर्यंत (parent->ref_first <= parent->ref_last) {
				पूर्णांक offset = ref_deltas[parent->ref_first++].obj_no;
				child_obj = objects + offset;
				जर (child_obj->real_type != OBJ_REF_DELTA) {
					child_obj = रिक्त;
					सुरूठेवा;
				}
				child_obj->real_type = parent->obj->real_type;
				थांबा;
			}

			जर (!child_obj && parent->ofs_first <= parent->ofs_last) {
				child_obj = objects +
					ofs_deltas[parent->ofs_first++].obj_no;
				प्रतिज्ञा(child_obj->real_type == OBJ_OFS_DELTA);
				child_obj->real_type = parent->obj->real_type;
			}

			जर (parent->ref_first > parent->ref_last &&
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

		जर (child_obj) {
			जर (parent) {
				child = resolve_delta(child_obj, parent);
				जर (!child->children_remaining)
					FREE_AND_NULL(child->data);
			} अन्यथा{
				child = make_base(child_obj, रिक्त);
				जर (child->children_remaining) {
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
		जर (parent)
			parent->retain_data--;

		जर (child && child->data) {
			/*
			 * This child has its own children, so add it to
			 * work_head.
			 */
			list_add(&child->list, &work_head);
			base_cache_used += child->size;
			prune_base_data(रिक्त);
		} अन्यथा जर (child) {
			/*
			 * This child does not have its own children. It may be
			 * the last descendant of its ancestors; free those
			 * that we can.
			 */
			संरचना base_data *p = parent;

			पर्यंत (p) {
				संरचना base_data *next_p;

				p->children_remaining--;
				जर (p->children_remaining)
					थांबा;

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
	परत रिक्त;
}

/*
 * First pass:
 * - find locations of all objects;
 * - calculate SHA1 of all non-delta objects;
 * - remember base (SHA1 or offset) for all deltas.
 */
स्थिर शून्य parse_pack_objects(अचिन्हित अक्षर *hash)
{
	पूर्णांक i, nr_delays = 0;
	संरचना ofs_delta_entry *ofs_delta = ofs_deltas;
	संरचना object_id ref_delta_oid;
	संरचना stat st;
	संरचना git_hash_ctx tmp_ctx;

	जर (verbose)
		progress = start_progress(
				the_repository,
				progress_title ? progress_title :
				from_stdin ? _("Receiving objects") : _("Indexing objects"),
				nr_objects);
	साठी (i = 0; i < nr_objects; i++) {
		संरचना object_entry *obj = &objects[i];
		शून्य *data = unpack_raw_entry(obj, &ofs_delta->offset,
					      &ref_delta_oid,
					      &obj->idx.oid);
		obj->real_type = obj->type;
		जर (obj->type == OBJ_OFS_DELTA) {
			nr_ofs_deltas++;
			ofs_delta->obj_no = i;
			ofs_delta++;
		} अन्यथा जर (obj->type == OBJ_REF_DELTA) {
			ALLOC_GROW(ref_deltas, nr_ref_deltas + 1, ref_deltas_alloc);
			oidcpy(&ref_deltas[nr_ref_deltas].oid, &ref_delta_oid);
			ref_deltas[nr_ref_deltas].obj_no = i;
			nr_ref_deltas++;
		} अन्यथा जर (!data) {
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
	git_hash_clone(&tmp_ctx, &आदान_ctx);
	git_hash_final(hash, &tmp_ctx);
	जर (!hasheq(fill(the_hash_algo->rawsz), hash, the_repository->hash_algo))
		die(_("pack is corrupted (SHA1 mismatch)"));
	use(the_hash_algo->rawsz);

	/* If input_fd is a file, we should have reached its end now. */
	जर (ख_स्थिति(आदान_fd, &st))
		die_errno(_("cannot fstat packfile"));
	जर (S_ISREG(st.st_mode) &&
			lseek(आदान_fd, 0, प्रस्तुत_से) - आदान_len != st.st_size)
		die(_("pack has junk at the end"));

	साठी (i = 0; i < nr_objects; i++) {
		संरचना object_entry *obj = &objects[i];
		जर (obj->real_type != OBJ_BAD)
			सुरूठेवा;
		obj->real_type = obj->type;
		sha1_object(रिक्त, obj, obj->size, obj->type,
			    &obj->idx.oid);
		nr_delays--;
	}
	जर (nr_delays)
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
	पूर्णांक i;

	जर (!nr_ofs_deltas && !nr_ref_deltas)
		परत;

	/* Sort deltas by base SHA1/offset for fast searching */
	QSORT(ofs_deltas, nr_ofs_deltas, compare_ofs_delta_entry);
	QSORT(ref_deltas, nr_ref_deltas, compare_ref_delta_entry);

	जर (verbose || show_resolving_progress)
		progress = start_progress(the_repository,
					  _("Resolving deltas"),
					  nr_ref_deltas + nr_ofs_deltas);

	nr_dispatched = 0;
	base_cache_limit = opts->delta_base_cache_limit * nr_threads;
	जर (nr_threads > 1 || दो_पर्या("GIT_FORCE_THREADS")) {
		init_thread();
		साठी (i = 0; i < nr_threads; i++) {
			पूर्णांक ret = pthread_create(&thread_data[i].thread, रिक्त,
						 threaded_second_pass, thread_data + i);
			जर (ret)
				die(_("unable to create thread: %s"),
				    म_त्रुटि(ret));
		}
		साठी (i = 0; i < nr_threads; i++)
			pthread_join(thread_data[i].thread, रिक्त);
		cleanup_thread();
		परत;
	}
	threaded_second_pass(&nothread_data);
}

/*
 * Third pass:
 * - append objects to convert thin pack to full pack if required
 * - write the final pack hash
 */
स्थिर शून्य fix_unresolved_deltas(संरचना hashfile *f);
स्थिर शून्य conclude_pack(पूर्णांक fix_thin_pack, स्थिरांक अक्षर *curr_pack, अचिन्हित अक्षर *pack_hash)
{
	जर (nr_ref_deltas + nr_ofs_deltas == nr_resolved_deltas) {
		stop_progress(&progress);
		/* Flush remaining pack final hash. */
		flush();
		परत;
	}

	जर (fix_thin_pack) {
		संरचना hashfile *f;
		अचिन्हित अक्षर read_hash[GIT_MAX_RAWSZ], tail_hash[GIT_MAX_RAWSZ];
		संरचना strbuf संदेश = STRBUF_INIT;
		पूर्णांक nr_unresolved = nr_ofs_deltas + nr_ref_deltas - nr_resolved_deltas;
		पूर्णांक nr_objects_initial = nr_objects;
		जर (nr_unresolved <= 0)
			die(_("confusion beyond insanity"));
		REALLOC_ARRAY(objects, nr_objects + nr_unresolved + 1);
		स_रखो(objects + nr_objects + 1, 0,
		       nr_unresolved * आकार(*objects));
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
		जर (!hasheq(read_hash, tail_hash, the_repository->hash_algo))
			die(_("Unexpected tail checksum for %s "
			      "(disk corruption?)"), curr_pack);
	}
	जर (nr_ofs_deltas + nr_ref_deltas != nr_resolved_deltas)
		die(Q_("pack has %d unresolved delta",
		       "pack has %d unresolved deltas",
		       nr_ofs_deltas + nr_ref_deltas - nr_resolved_deltas),
		    nr_ofs_deltas + nr_ref_deltas - nr_resolved_deltas);
}

स्थिर पूर्णांक write_compressed(संरचना hashfile *f, शून्य *in, अचिन्हित पूर्णांक size)
{
	git_zstream stream;
	पूर्णांक status;
	अचिन्हित अक्षर outbuf[4096];
	संरचना repo_config_मूल्ये *cfg = repo_config_मूल्ये(the_repository);

	git_deflate_init(&stream, cfg->zlib_compression_level);
	stream.next_in = in;
	stream.avail_in = size;

	करा {
		stream.next_out = outbuf;
		stream.avail_out = आकार(outbuf);
		status = git_deflate(&stream, Z_FINISH);
		hashwrite(f, outbuf, आकार(outbuf) - stream.avail_out);
	} पर्यंत (status == Z_OK);

	जर (status != Z_STREAM_END)
		die(_("unable to deflate appended object (%d)"), status);
	size = stream.total_out;
	git_deflate_end(&stream);
	परत size;
}

स्थिर संरचना object_entry *append_obj_to_pack(संरचना hashfile *f,
			       स्थिरांक अचिन्हित अक्षर *sha1, शून्य *buf,
			       अचिन्हित दीर्घ size, गणना object_type type)
{
	संरचना object_entry *obj = &objects[nr_objects++];
	अचिन्हित अक्षर header[10];
	अचिन्हित दीर्घ s = size;
	पूर्णांक n = 0;
	अचिन्हित अक्षर c = (type << 4) | (s & 15);
	s >>= 4;
	पर्यंत (s) {
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
	परत obj;
}

स्थिर पूर्णांक delta_pos_compare(स्थिरांक शून्य *_a, स्थिरांक शून्य *_b)
{
	संरचना ref_delta_entry *a = *(संरचना ref_delta_entry **)_a;
	संरचना ref_delta_entry *b = *(संरचना ref_delta_entry **)_b;
	परत a->obj_no - b->obj_no;
}

स्थिर शून्य fix_unresolved_deltas(संरचना hashfile *f)
{
	संरचना ref_delta_entry **sorted_by_pos;
	पूर्णांक i;

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
	साठी (i = 0; i < nr_ref_deltas; i++)
		sorted_by_pos[i] = &ref_deltas[i];
	QSORT(sorted_by_pos, nr_ref_deltas, delta_pos_compare);

	जर (repo_has_promisor_remote(the_repository)) {
		/*
		 * Prefetch the delta bases.
		 */
		संरचना oid_array to_fetch = OID_ARRAY_INIT;
		साठी (i = 0; i < nr_ref_deltas; i++) {
			संरचना ref_delta_entry *d = sorted_by_pos[i];
			जर (!odb_read_object_info_extended(the_repository->objects,
							   &d->oid, रिक्त,
							   OBJECT_INFO_FOR_PREFETCH))
				सुरूठेवा;
			oid_array_append(&to_fetch, &d->oid);
		}
		promisor_remote_get_direct(the_repository,
					   to_fetch.oid, to_fetch.nr);
		oid_array_clear(&to_fetch);
	}

	साठी (i = 0; i < nr_ref_deltas; i++) {
		संरचना ref_delta_entry *d = sorted_by_pos[i];
		गणना object_type type;
		शून्य *data;
		माप_प्रकार size;

		जर (objects[d->obj_no].real_type != OBJ_REF_DELTA)
			सुरूठेवा;
		data = odb_read_object(the_repository->objects, &d->oid,
				       &type, &size);
		जर (!data)
			सुरूठेवा;

		जर (check_object_signature(the_repository, &d->oid, data, size,
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

स्थिर स्थिरांक अक्षर *derive_filename(स्थिरांक अक्षर *pack_name, स्थिरांक अक्षर *strip,
				   स्थिरांक अक्षर *suffix, संरचना strbuf *buf)
{
	माप_प्रकार len;
	जर (!strip_suffix(pack_name, strip, &len) || !len ||
	    pack_name[len - 1] != '.')
		die(_("packfile name '%s' does not end with '.%s'"),
		    pack_name, strip);
	strbuf_add(buf, pack_name, len);
	strbuf_addstr(buf, suffix);
	परत buf->buf;
}

स्थिर शून्य write_special_file(स्थिरांक अक्षर *suffix, स्थिरांक अक्षर *संदेश,
			       स्थिरांक अक्षर *pack_name, स्थिरांक अचिन्हित अक्षर *hash,
			       स्थिरांक अक्षर **report)
{
	संरचना strbuf name_buf = STRBUF_INIT;
	स्थिरांक अक्षर *filename;
	पूर्णांक fd;
	पूर्णांक संदेश_len = म_माप(संदेश);

	जर (pack_name)
		filename = derive_filename(pack_name, "pack", suffix, &name_buf);
	अन्यथा
		filename = odb_pack_name(the_repository, &name_buf, hash, suffix);

	fd = safe_create_file_with_leading_directories(the_repository, filename);
	जर (fd < 0) {
		जर (त्रुटि_सं != EEXIST)
			die_errno(_("cannot write %s file '%s'"),
				  suffix, filename);
	} अन्यथा {
		जर (संदेश_len > 0) {
			write_or_die(fd, संदेश, संदेश_len);
			write_or_die(fd, "\n", 1);
		}
		जर (बंद(fd) != 0)
			die_errno(_("cannot close written %s file '%s'"),
				  suffix, filename);
		जर (report)
			*report = suffix;
	}
	strbuf_release(&name_buf);
}

स्थिर शून्य rename_tmp_packfile(स्थिरांक अक्षर **final_name,
				स्थिरांक अक्षर *curr_name,
				संरचना strbuf *name, अचिन्हित अक्षर *hash,
				स्थिरांक अक्षर *ext, पूर्णांक make_read_only_if_same)
{
	जर (!*final_name || म_भेद(*final_name, curr_name)) {
		जर (!*final_name)
			*final_name = odb_pack_name(the_repository, name, hash, ext);
		जर (finalize_object_file(the_repository, curr_name, *final_name))
			die(_("unable to rename temporary '*.%s' file to '%s'"),
			    ext, *final_name);
	} अन्यथा जर (make_read_only_if_same) {
		chmod(*final_name, 0444);
	}
}

स्थिर शून्य final(स्थिरांक अक्षर *final_pack_name, स्थिरांक अक्षर *curr_pack_name,
		  स्थिरांक अक्षर *final_index_name, स्थिरांक अक्षर *curr_index_name,
		  स्थिरांक अक्षर *final_rev_index_name, स्थिरांक अक्षर *curr_rev_index_name,
		  स्थिरांक अक्षर *keep_संदेश, स्थिरांक अक्षर *promisor_संदेश,
		  अचिन्हित अक्षर *hash)
{
	स्थिरांक अक्षर *report = "pack";
	संरचना strbuf pack_name = STRBUF_INIT;
	संरचना strbuf index_name = STRBUF_INIT;
	संरचना strbuf rev_index_name = STRBUF_INIT;

	जर (!from_stdin) {
		बंद(आदान_fd);
	} अन्यथा {
		fsync_component_or_die(FSYNC_COMPONENT_PACK, output_fd, curr_pack_name);
		जर (बंद(output_fd))
			die_errno(_("error while closing pack file"));
	}

	जर (keep_संदेश)
		write_special_file("keep", keep_संदेश, final_pack_name, hash,
				   &report);
	जर (promisor_संदेश)
		write_special_file("promisor", promisor_संदेश, final_pack_name,
				   hash, रिक्त);

	rename_tmp_packfile(&final_pack_name, curr_pack_name, &pack_name,
			    hash, "pack", from_stdin);
	जर (curr_rev_index_name)
		rename_tmp_packfile(&final_rev_index_name, curr_rev_index_name,
				    &rev_index_name, hash, "rev", 1);
	rename_tmp_packfile(&final_index_name, curr_index_name, &index_name,
			    hash, "idx", 1);

	जर (do_fsck_object && startup_info->have_repository) {
		संरचना odb_source_files *files =
			odb_source_files_downcast(the_repository->objects->source);
		packfile_store_load_pack(files->dirs->packed, final_index_name, 0);
	}

	जर (!from_stdin) {
		म_लिखो("%s\n", hash_to_hex(hash));
	} अन्यथा {
		संरचना strbuf buf = STRBUF_INIT;

		strbuf_addf(&buf, "%s\t%s\n", report, hash_to_hex(hash));
		write_or_die(1, buf.buf, buf.len);
		strbuf_release(&buf);

		/* Write the last part of the buffer to stdout */
		write_in_full(1, आदान_buffer + आदान_offset, आदान_len);
	}

	strbuf_release(&rev_index_name);
	strbuf_release(&index_name);
	strbuf_release(&pack_name);
}

स्थिर पूर्णांक git_index_pack_config(स्थिरांक अक्षर *k, स्थिरांक अक्षर *v,
				 स्थिरांक संरचना config_context *ctx, शून्य *cb)
{
	संरचना pack_idx_option *opts = cb;

	जर (!म_भेद(k, "pack.indexversion")) {
		opts->version = git_config_int(k, v, ctx->kvi);
		जर (opts->version > 2)
			die(_("bad pack.indexVersion=%"PRIu32), opts->version);
		परत 0;
	}
	जर (!म_भेद(k, "pack.threads")) {
		nr_threads = git_config_int(k, v, ctx->kvi);
		जर (nr_threads < 0)
			die(_("invalid number of threads specified (%d)"),
			    nr_threads);
		जर (!HAVE_THREADS && nr_threads != 1) {
			warning(_("no threads support, ignoring %s"), k);
			nr_threads = 1;
		}
		परत 0;
	}
	जर (!म_भेद(k, "pack.writereverseindex")) {
		जर (git_config_bool(k, v))
			opts->flags |= WRITE_REV;
		अन्यथा
			opts->flags &= ~WRITE_REV;
	}
	जर (!म_भेद(k, "core.deltabasecachelimit")) {
		opts->delta_base_cache_limit = git_config_ulong(k, v, ctx->kvi);
		परत 0;
	}
	परत git_default_config(k, v, ctx, cb);
}

स्थिर पूर्णांक cmp_uint32(स्थिरांक शून्य *a_, स्थिरांक शून्य *b_)
{
	uint32_t a = *((uint32_t *)a_);
	uint32_t b = *((uint32_t *)b_);

	परत (a < b) ? -1 : (a != b);
}

स्थिर शून्य read_v2_anomalous_offsets(संरचना packed_git *p,
				      संरचना pack_idx_option *opts)
{
	स्थिरांक uint32_t *idx1, *idx2;
	uint32_t i;

	/* The address of the 4-byte offset table */
	idx1 = (((स्थिरांक uint32_t *)((स्थिरांक uint8_t *)p->index_data + p->crc_offset))
		+ (माप_प्रकार)p->अंक_objects /* CRC32 table */
		);

	/* The address of the 8-byte offset table */
	idx2 = idx1 + p->अंक_objects;

	साठी (i = 0; i < p->अंक_objects; i++) {
		uint32_t off = ntohl(idx1[i]);
		जर (!(off & 0x80000000))
			सुरूठेवा;
		off = off & 0x7fffffff;
		check_pack_index_ptr(p, &idx2[off * 2]);
		जर (idx2[off * 2])
			सुरूठेवा;
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

स्थिर शून्य read_idx_option(संरचना pack_idx_option *opts, स्थिरांक अक्षर *pack_name)
{
	संरचना packed_git *p = add_packed_git(the_repository, pack_name,
					      म_माप(pack_name), 1);

	जर (!p)
		die(_("Cannot open existing pack file '%s'"), pack_name);
	जर (open_pack_index(p))
		die(_("Cannot open existing pack idx file for '%s'"), pack_name);

	/* Read the attributes from the existing idx file */
	opts->version = p->index_version;

	जर (opts->version == 2)
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

स्थिर शून्य show_pack_info(पूर्णांक stat_only)
{
	पूर्णांक i, baseobjects = nr_objects - nr_ref_deltas - nr_ofs_deltas;
	अचिन्हित दीर्घ *chain_histogram = रिक्त;

	जर (deepest_delta)
		CALLOC_ARRAY(chain_histogram, deepest_delta);

	साठी (i = 0; i < nr_objects; i++) {
		संरचना object_entry *obj = &objects[i];

		जर (is_delta_type(obj->type))
			chain_histogram[obj_stat[i].delta_depth - 1]++;
		जर (stat_only)
			सुरूठेवा;
		म_लिखो("%s %-6s %"PRIuMAX" %"PRIuMAX" %"PRIuMAX,
		       oid_to_hex(&obj->idx.oid),
		       type_name(obj->real_type), (uintmax_t)obj->size,
		       (uintmax_t)(obj[1].idx.offset - obj->idx.offset),
		       (uintmax_t)obj->idx.offset);
		जर (is_delta_type(obj->type)) {
			संरचना object_entry *bobj = &objects[obj_stat[i].base_object_no];
			म_लिखो(" %u %s", obj_stat[i].delta_depth,
			       oid_to_hex(&bobj->idx.oid));
		}
		अक्षर_दो('\n');
	}

	जर (baseobjects)
		printf_ln(Q_("non delta: %d object",
			     "non delta: %d objects",
			     baseobjects),
			  baseobjects);
	साठी (i = 0; i < deepest_delta; i++) {
		जर (!chain_histogram[i])
			सुरूठेवा;
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
	खाता *out;
	संरचना strbuf ओळ = STRBUF_INIT;
	संरचना oidset_iter iter;
	संरचना object_id *oid;
	अक्षर *base_name = रिक्त;

	जर (!oidset_size(&outgoing_links))
		परत;

	oidset_iter_init(&outgoing_links, &iter);
	पर्यंत ((oid = oidset_iter_next(&iter))) {
		संरचना odb_source_info source_info;
		संरचना object_info info = {
			.source_infop = &source_info,
		};

		जर (odb_read_object_info_extended(the_repository->objects, oid, &info, 0))
			/* Missing; assume it is a promisor object */
			सुरूठेवा;
		जर (source_info.source->type == ODB_SOURCE_PACKED &&
		    source_info.u.packed.pack->pack_promisor)
			सुरूठेवा;

		जर (!cmd.args.nr) {
			base_name = mkpathdup(
				"%s/pack/pack",
				repo_get_object_directory(the_repository));
			strvec_ढकला(&cmd.args, "pack-objects");
			strvec_ढकला(&cmd.args,
				    "--exclude-promisor-objects-best-effort");
			strvec_ढकला(&cmd.args, base_name);
			cmd.git_cmd = 1;
			cmd.in = -1;
			cmd.out = -1;
			जर (start_command(&cmd))
				die(_("could not start pack-objects to repack local links"));
		}

		जर (write_in_full(cmd.in, oid_to_hex(oid), the_hash_algo->hexsz) < 0 ||
		    write_in_full(cmd.in, "\n", 1) < 0)
			die(_("failed to feed local object to pack-objects"));
	}

	जर (!cmd.args.nr)
		परत;

	बंद(cmd.in);

	out = xfdopen(cmd.out, "r");
	पर्यंत (strbuf_getline_lf(&ओळ, out) != खातापूर्ण) {
		अचिन्हित अक्षर द्विमान[GIT_MAX_RAWSZ];
		जर (ओळ.len != the_hash_algo->hexsz ||
		    !hex_to_bytes(द्विमान, ओळ.buf, ओळ.len))
			die(_("index-pack: Expecting full hex object ID lines only from pack-objects."));

		/*
		 * pack-objects creates the .pack and .idx files, but not the
		 * .promisor file. Create the .promisor file, which is empty.
		 */
		write_special_file("promisor", "", रिक्त, द्विमान, रिक्त);
	}

	ख_बंद(out);
	जर (finish_command(&cmd))
		die(_("could not finish pack-objects to repack local links"));
	strbuf_release(&ओळ);
	मुक्त(base_name);
}

पूर्णांक cmd_index_pack(पूर्णांक argc,
		   स्थिरांक अक्षर **argv,
		   स्थिरांक अक्षर *prefix,
		   संरचना repository *repo)
{
	पूर्णांक i, fix_thin_pack = 0, verify = 0, stat_only = 0, rev_index;
	स्थिरांक अक्षर *curr_index;
	अक्षर *curr_rev_index = रिक्त;
	स्थिरांक अक्षर *index_name = रिक्त, *pack_name = रिक्त, *rev_index_name = रिक्त;
	स्थिरांक अक्षर *keep_संदेश = रिक्त;
	स्थिरांक अक्षर *promisor_संदेश = रिक्त;
	संरचना strbuf index_name_buf = STRBUF_INIT;
	संरचना strbuf rev_index_name_buf = STRBUF_INIT;
	संरचना pack_idx_entry **idx_objects;
	संरचना pack_idx_option opts;
	अचिन्हित अक्षर pack_hash[GIT_MAX_RAWSZ];
	अचिन्हित foreign_nr = 1;	/* zero is a "good" value, assume bad */
	पूर्णांक report_end_of_आदान = 0;
	पूर्णांक hash_algo = 0;

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
	जर (prefix && स_बदलो(prefix))
		die(_("Cannot come back to cwd"));

	जर (git_env_bool(GIT_TEST_NO_WRITE_REV_INDEX, 0))
		rev_index = 0;
	अन्यथा
		rev_index = !!(opts.flags & (WRITE_REV_VERIFY | WRITE_REV));

	साठी (i = 1; i < argc; i++) {
		स्थिरांक अक्षर *arg = argv[i];

		जर (*arg == '-') {
			जर (!म_भेद(arg, "--stdin")) {
				from_stdin = 1;
			} अन्यथा जर (!म_भेद(arg, "--fix-thin")) {
				fix_thin_pack = 1;
			} अन्यथा जर (skip_to_optional_arg(arg, "--strict", &arg)) {
				strict = 1;
				do_fsck_object = 1;
				fsck_set_संदेश_types(&fsck_options, arg);
			} अन्यथा जर (!म_भेद(arg, "--check-self-contained-and-connected")) {
				strict = 1;
				check_self_contained_and_connected = 1;
			} अन्यथा जर (skip_to_optional_arg(arg, "--fsck-objects", &arg)) {
				do_fsck_object = 1;
				fsck_set_संदेश_types(&fsck_options, arg);
			} अन्यथा जर (!म_भेद(arg, "--verify")) {
				verify = 1;
			} अन्यथा जर (!म_भेद(arg, "--verify-stat")) {
				verify = 1;
				show_stat = 1;
			} अन्यथा जर (!म_भेद(arg, "--verify-stat-only")) {
				verify = 1;
				show_stat = 1;
				stat_only = 1;
			} अन्यथा जर (skip_to_optional_arg(arg, "--keep", &keep_संदेश)) {
				; /* nothing to do */
			} अन्यथा जर (skip_to_optional_arg(arg, "--promisor", &promisor_संदेश)) {
				record_outgoing_links = 1;
			} अन्यथा जर (starts_with(arg, "--threads=")) {
				अक्षर *end;
				nr_threads = म_से_अदीर्घ(arg+10, &end, 0);
				जर (!arg[10] || *end || nr_threads < 0)
					usage(index_pack_usage);
				जर (!HAVE_THREADS && nr_threads != 1) {
					warning(_("no threads support, ignoring %s"), arg);
					nr_threads = 1;
				}
			} अन्यथा जर (skip_prefix(arg, "--pack_header=", &arg)) {
				जर (parse_pack_header_option(arg,
							     आदान_buffer,
							     &आदान_len) < 0)
					die(_("bad --pack_header: %s"), arg);
			} अन्यथा जर (!म_भेद(arg, "-v")) {
				verbose = 1;
			} अन्यथा जर (!म_भेद(arg, "--progress-title")) {
				जर (progress_title || (i+1) >= argc)
					usage(index_pack_usage);
				progress_title = argv[++i];
			} अन्यथा जर (!म_भेद(arg, "--show-resolving-progress")) {
				show_resolving_progress = 1;
			} अन्यथा जर (!म_भेद(arg, "--report-end-of-input")) {
				report_end_of_आदान = 1;
			} अन्यथा जर (!म_भेद(arg, "-o")) {
				जर (index_name || (i+1) >= argc)
					usage(index_pack_usage);
				index_name = argv[++i];
			} अन्यथा जर (starts_with(arg, "--index-version=")) {
				अक्षर *c;
				opts.version = म_से_अदीर्घ(arg + 16, &c, 10);
				जर (opts.version > 2)
					die(_("bad %s"), arg);
				जर (*c == ',')
					opts.off32_limit = म_से_अदीर्घ(c+1, &c, 0);
				जर (*c || opts.off32_limit & 0x80000000)
					die(_("bad %s"), arg);
			} अन्यथा जर (skip_prefix(arg, "--max-input-size=", &arg)) {
				max_आदान_size = strtoumax(arg, रिक्त, 10);
			} अन्यथा जर (skip_prefix(arg, "--object-format=", &arg)) {
				hash_algo = hash_algo_by_name(arg);
				जर (hash_algo == GIT_HASH_UNKNOWN)
					die(_("unknown hash algorithm '%s'"), arg);
				repo_set_hash_algo(the_repository, hash_algo);
			} अन्यथा जर (!म_भेद(arg, "--rev-index")) {
				rev_index = 1;
			} अन्यथा जर (!म_भेद(arg, "--no-rev-index")) {
				rev_index = 0;
			} अन्यथा
				usage(index_pack_usage);
			सुरूठेवा;
		}

		जर (pack_name)
			usage(index_pack_usage);
		pack_name = arg;
	}

	जर (!pack_name && !from_stdin)
		usage(index_pack_usage);
	जर (fix_thin_pack && !from_stdin)
		die(_("the option '%s' requires '%s'"), "--fix-thin", "--stdin");
	जर (promisor_संदेश && pack_name)
		die(_("--promisor cannot be used with a pack name"));
	जर (from_stdin && !startup_info->have_repository)
		die(_("--stdin requires a git repository"));
	जर (from_stdin && hash_algo)
		die(_("options '%s' and '%s' cannot be used together"), "--object-format", "--stdin");
	जर (!index_name && pack_name)
		index_name = derive_filename(pack_name, "pack", "idx", &index_name_buf);

	/*
	 * Packfiles and indices do not carry enough information to be able to
	 * identify their object hash. So when we are neither in a repository
	 * nor has the user told us which object hash to use we have no other
	 * choice but to guess the object hash.
	 */
	जर (!the_repository->hash_algo)
		repo_set_hash_algo(the_repository, GIT_HASH_DEFAULT);

	opts.flags &= ~(WRITE_REV | WRITE_REV_VERIFY);
	जर (rev_index) {
		opts.flags |= verify ? WRITE_REV_VERIFY : WRITE_REV;
		जर (index_name)
			rev_index_name = derive_filename(index_name,
							 "idx", "rev",
							 &rev_index_name_buf);
	}

	जर (verify) {
		जर (!index_name)
			die(_("--verify with no packfile name given"));
		read_idx_option(&opts, index_name);
		opts.flags |= WRITE_IDX_VERIFY | WRITE_IDX_STRICT;
	}
	जर (strict)
		opts.flags |= WRITE_IDX_STRICT;

	जर (HAVE_THREADS && !nr_threads) {
		nr_threads = online_cpus();
		/*
		 * Experiments show that going above 20 threads doesn't help,
		 * no matter how many cores you have. Below that, we tend to
		 * max at half the number of online_cpus(), presumably because
		 * half of those are hyperthreads rather than full cores. We'll
		 * never reduce the level below "3", though, to match a
		 * historical value that nobody complained about.
		 */
		जर (nr_threads < 4)
			; /* too few cores to consider capping */
		अन्यथा जर (nr_threads < 6)
			nr_threads = 3; /* historic cap */
		अन्यथा जर (nr_threads < 40)
			nr_threads /= 2;
		अन्यथा
			nr_threads = 20; /* hard cap */
	}

	curr_pack = open_pack_file(pack_name);
	parse_pack_header();
	CALLOC_ARRAY(objects, st_add(nr_objects, 1));
	जर (show_stat)
		CALLOC_ARRAY(obj_stat, st_add(nr_objects, 1));
	CALLOC_ARRAY(ofs_deltas, nr_objects);
	parse_pack_objects(pack_hash);
	जर (report_end_of_आदान)
		write_in_full(2, "\0", 1);
	resolve_deltas(&opts);
	conclude_pack(fix_thin_pack, curr_pack, pack_hash);
	मुक्त(ofs_deltas);
	मुक्त(ref_deltas);
	जर (strict)
		foreign_nr = check_objects();

	जर (show_stat)
		show_pack_info(stat_only);

	ALLOC_ARRAY(idx_objects, nr_objects);
	साठी (i = 0; i < nr_objects; i++)
		idx_objects[i] = &objects[i].idx;
	curr_index = write_idx_file(the_repository, index_name, idx_objects,
				    nr_objects, &opts, pack_hash);
	जर (rev_index)
		curr_rev_index = write_rev_file(the_repository, rev_index_name,
						idx_objects, nr_objects,
						pack_hash, opts.flags);
	मुक्त(idx_objects);

	जर (!verify)
		final(pack_name, curr_pack,
		      index_name, curr_index,
		      rev_index_name, curr_rev_index,
		      keep_संदेश, promisor_संदेश,
		      pack_hash);
	अन्यथा
		बंद(आदान_fd);

	जर (do_fsck_object) {
		/*
		 * We cannot perform queued consistency checks when running
		 * outside of a repository because those require us to read
		 * from the object database, which is uninitialized.
		 *
		 * TODO: we may eventually set up an in-memory object database,
		 * which would allow us to perform these queued checks.
		 */
		जर (!startup_info->have_repository &&
		    fsck_has_queued_checks(&fsck_options))
			die(_("cannot perform queued object checks outside "
			      "of a repository"));

		जर (fsck_finish(&fsck_options))
			die(_("fsck error in pack objects"));
	}

	मुक्त(opts.anomaly);
	मुक्त(objects);
	strbuf_release(&index_name_buf);
	strbuf_release(&rev_index_name_buf);
	जर (!pack_name)
		मुक्त((शून्य *) curr_pack);
	जर (!index_name)
		मुक्त((शून्य *) curr_index);
	मुक्त(curr_rev_index);

	repack_local_links();

	/*
	 * Let the caller know this pack is not self contained
	 */
	जर (check_self_contained_and_connected && foreign_nr)
		परत 1;

	परत 0;
}
