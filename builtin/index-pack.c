#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "config.h"
#অন্তর্ভাব "delta.h"
#অন্তর্ভাব "environment.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "hex.h"
#অন্তর্ভাব "pack.h"
#অন্তর্ভাব "csum-file.h"
#অন্তর্ভাব "blob.h"
#অন্তর্ভাব "commit.h"
#অন্তর্ভাব "tag.h"
#অন্তর্ভাব "tree.h"
#অন্তর্ভাব "progress.h"
#অন্তর্ভাব "fsck.h"
#অন্তর্ভাব "strbuf.h"
#অন্তর্ভাব "thread-utils.h"
#অন্তর্ভাব "packfile.h"
#অন্তর্ভাব "pack-revindex.h"
#অন্তর্ভাব "object-file.h"
#অন্তর্ভাব "odb.h"
#অন্তর্ভাব "odb/streaming.h"
#অন্তর্ভাব "oid-array.h"
#অন্তর্ভাব "oidset.h"
#অন্তর্ভাব "path.h"
#অন্তর্ভাব "replace-object.h"
#অন্তর্ভাব "tree-walk.h"
#অন্তর্ভাব "promisor-remote.h"
#অন্তর্ভাব "run-command.h"
#অন্তর্ভাব "setup.h"
#অন্তর্ভাব "strvec.h"

স্থির ধ্রুবক অক্ষর সূচক_pack_usage[] =
"git index-pack [-v] [-o <index-file>] [--keep | --keep=<msg>] [--[no-]rev-index] [--verify] [--strict[=<msg-id>=<severity>...]] [--fsck-objects[=<msg-id>=<severity>...]] (<pack-file> | --stdin [--fix-thin] [<pack-file>])";

কাঠামো object_entry {
	কাঠামো pack_idx_entry idx;
	আকার_t আকার_2;
	unsigned অক্ষর hdr_আকার;
	signed অক্ষর type;
	signed অক্ষর real_type;
};

কাঠামো object_stat {
	unsigned delta_depth;
	পূর্ণসংখ্যা base_object_no;
};

কাঠামো base_তথ্য {
	/* Initialized by make_base(). */
	কাঠামো base_তথ্য *base;
	কাঠামো object_entry *obj;
	পূর্ণসংখ্যা ref_first, ref_last;
	পূর্ণসংখ্যা ofs_first, ofs_last;
	/*
	 * Threads should increment retain_data if they are about to call
	 * patch_delta() using this struct's data as a base, and decrement this
	 * when they are done. While retain_data is nonzero, this struct's data
	 * will not be freed even if the delta base cache limit is exceeded.
	 */
	পূর্ণসংখ্যা retain_তথ্য;
	/*
	 * The number of direct children that have not been fully processed
	 * (entered work_head, entered done_head, left done_head). When this
	 * number reaches zero, this struct base_data can be freed.
	 */
	পূর্ণসংখ্যা children_remaining;

	/* Not initialized by make_base(). */
	কাঠামো list_শীর্ষ list;
	শূন্য *তথ্য;
	আকার_t আকার_2;
};

/*
 * Stack of struct base_data that have unprocessed children.
 * threaded_second_pass() uses this as a source of work (the other being the
 * objects array).
 *
 * Guarded by work_mutex.
 */
স্থির LIST_শীর্ষ(work_শীর্ষ);

/*
 * Stack of struct base_data that have children, all of whom have been
 * processed or are being processed, and at least one child is being processed.
 * These struct base_data must be kept around until the last child is
 * processed.
 *
 * Guarded by work_mutex.
 */
স্থির LIST_শীর্ষ(done_শীর্ষ);

/*
 * All threads share one delta base cache.
 *
 * base_cache_used is guarded by work_mutex, and base_cache_limit is read-only
 * in a thread.
 */
স্থির আকার_t base_cache_used;
স্থির আকার_t base_cache_limit;

কাঠামো thread_local_তথ্য {
	pthread_t thread;
	পূর্ণসংখ্যা pack_fd;
};

/* Remember to update object flag allocation in object.h */
#define FLAG_LINK (1u<<20)
#define FLAG_CHECKED (1u<<21)

কাঠামো ofs_delta_entry {
	off_t offset;
	পূর্ণসংখ্যা obj_no;
};

কাঠামো ref_delta_entry {
	কাঠামো object_id oid;
	পূর্ণসংখ্যা obj_no;
};

স্থির কাঠামো object_entry *objects;
স্থির কাঠামো object_stat *obj_stat;
স্থির কাঠামো ofs_delta_entry *ofs_deltas;
স্থির কাঠামো ref_delta_entry *ref_deltas;
স্থির কাঠামো thread_local_তথ্য nothread_তথ্য;
স্থির পূর্ণসংখ্যা nr_objects;
স্থির পূর্ণসংখ্যা nr_ofs_deltas;
স্থির পূর্ণসংখ্যা nr_ref_deltas;
স্থির পূর্ণসংখ্যা ref_deltas_alloc;
স্থির পূর্ণসংখ্যা nr_resolved_deltas;
স্থির পূর্ণসংখ্যা nr_threads;

স্থির পূর্ণসংখ্যা from_stdin;
স্থির পূর্ণসংখ্যা strict;
স্থির পূর্ণসংখ্যা do_fsck_object;
স্থির কাঠামো fsck_options fsck_options;
স্থির পূর্ণসংখ্যা verbose;
স্থির ধ্রুবক অক্ষর *progress_title;
স্থির পূর্ণসংখ্যা show_resolving_progress;
স্থির পূর্ণসংখ্যা show_stat;
স্থির পূর্ণসংখ্যা check_self_contained_and_connected;

স্থির কাঠামো progress *progress;

স্থির unsigned অক্ষর প্রবেশ_বাফার[DEFAULT_IO_বাফার_আকার];
স্থির unsigned পূর্ণসংখ্যা প্রবেশ_offset, প্রবেশ_len;
স্থির off_t consumed_bytes;
স্থির off_t max_প্রবেশ_আকার;
স্থির unsigned deepest_delta;
স্থির কাঠামো git_hash_ctx প্রবেশ_ctx;
স্থির uint32_t প্রবেশ_crc32;
স্থির পূর্ণসংখ্যা প্রবেশ_fd, output_fd;
স্থির ধ্রুবক অক্ষর *curr_pack;

/*
 * outgoing_links is guarded by read_mutex, and record_outgoing_links is
 * read-only in a thread.
 */
স্থির কাঠামো oidset outgoing_links = OIDSET_INIT;
স্থির পূর্ণসংখ্যা record_outgoing_links;

স্থির কাঠামো thread_local_তথ্য *thread_তথ্য;
স্থির পূর্ণসংখ্যা nr_dispatched;
স্থির পূর্ণসংখ্যা threads_active;

স্থির pthread_mutex_t read_mutex;
#define read_lock()		lock_mutex(&read_mutex)
#define read_unlock()		unlock_mutex(&read_mutex)

স্থির pthread_mutex_t গণক_mutex;
#define গণক_lock()		lock_mutex(&গণক_mutex)
#define গণক_unlock()	unlock_mutex(&গণক_mutex)

স্থির pthread_mutex_t work_mutex;
#define work_lock()		lock_mutex(&work_mutex)
#define work_unlock()		unlock_mutex(&work_mutex)

স্থির pthread_mutex_t deepest_delta_mutex;
#define deepest_delta_lock()	lock_mutex(&deepest_delta_mutex)
#define deepest_delta_unlock()	unlock_mutex(&deepest_delta_mutex)

স্থির pthread_চাবি_t চাবি;

স্থির অন্তঃরেখ শূন্য lock_mutex(pthread_mutex_t *mutex)
{
	যদি (threads_active)
		pthread_mutex_lock(mutex);
}

স্থির অন্তঃরেখ শূন্য unlock_mutex(pthread_mutex_t *mutex)
{
	যদি (threads_active)
		pthread_mutex_unlock(mutex);
}

/*
 * Mutex and conditional variable can't be statically-initialized on Windows.
 */
স্থির শূন্য init_thread(শূন্য)
{
	পূর্ণসংখ্যা i;
	init_recursive_mutex(&read_mutex);
	pthread_mutex_init(&গণক_mutex, ফাঁকা);
	pthread_mutex_init(&work_mutex, ফাঁকা);
	যদি (show_stat)
		pthread_mutex_init(&deepest_delta_mutex, ফাঁকা);
	pthread_চাবি_create(&চাবি, ফাঁকা);
	CALLOC_ARRAY(thread_তথ্য, nr_threads);
	জন্য (i = 0; i < nr_threads; i++) {
		thread_তথ্য[i].pack_fd = xopen(curr_pack, O_RDONLY);
	}

	threads_active = 1;
}

স্থির শূন্য cleanup_thread(শূন্য)
{
	পূর্ণসংখ্যা i;
	যদি (!threads_active)
		ফেরত;
	threads_active = 0;
	pthread_mutex_destroy(&read_mutex);
	pthread_mutex_destroy(&গণক_mutex);
	pthread_mutex_destroy(&work_mutex);
	যদি (show_stat)
		pthread_mutex_destroy(&deepest_delta_mutex);
	জন্য (i = 0; i < nr_threads; i++)
		close(thread_তথ্য[i].pack_fd);
	pthread_চাবি_delete(চাবি);
	মুক্ত(thread_তথ্য);
}

স্থির পূর্ণসংখ্যা mark_link(কাঠামো object *obj, গণনা object_type type,
		     শূন্য *তথ্য UNUSED,
		     কাঠামো fsck_options *options UNUSED)
{
	যদি (!obj)
		ফেরত -1;

	যদি (type != OBJ_ANY && obj->type != type)
		die(_("object type mismatch at %s"), oid_to_hex(&obj->oid));

	obj->flags |= FLAG_LINK;
	ফেরত 0;
}

/* The content of each linked object must have been checked
   or it must be already present in the object database */
স্থির unsigned check_object(কাঠামো object *obj)
{
	যদি (!obj)
		ফেরত 0;

	যদি (!(obj->flags & FLAG_LINK))
		ফেরত 0;

	যদি (!(obj->flags & FLAG_CHECKED)) {
		আকার_t আকার_2;
		পূর্ণসংখ্যা type = odb_read_object_info(the_repository->objects,
						&obj->oid, &আকার_2);
		যদি (type <= 0)
			die(_("did not receive expected object %s"),
			      oid_to_hex(&obj->oid));
		যদি (type != obj->type)
			die(_("object %s: expected type %s, found %s"),
			    oid_to_hex(&obj->oid),
			    type_name(obj->type), type_name(type));
		obj->flags |= FLAG_CHECKED;
		ফেরত 1;
	}

	ফেরত 0;
}

স্থির unsigned check_objects(শূন্য)
{
	unsigned i, max, foreign_nr = 0;

	max = get_max_object_সূচক(the_repository);

	যদি (verbose)
		progress = start_delayed_progress(the_repository,
						  _("Checking objects"), max);

	জন্য (i = 0; i < max; i++) {
		foreign_nr += check_object(get_indexed_object(the_repository, i));
		display_progress(progress, i + 1);
	}

	stop_progress(&progress);
	ফেরত foreign_nr;
}


/* Discard current buffer used content. */
স্থির শূন্য flush(শূন্য)
{
	যদি (প্রবেশ_offset) {
		যদি (output_fd >= 0)
			write_or_die(output_fd, প্রবেশ_বাফার, প্রবেশ_offset);
		git_hash_update(&প্রবেশ_ctx, প্রবেশ_বাফার, প্রবেশ_offset);
		memmove(প্রবেশ_বাফার, প্রবেশ_বাফার + প্রবেশ_offset, প্রবেশ_len);
		প্রবেশ_offset = 0;
	}
}

/*
 * Make sure at least "min" bytes are available in the buffer, and
 * return the pointer to the buffer.
 */
স্থির শূন্য *fill(পূর্ণসংখ্যা min)
{
	যদি (min <= প্রবেশ_len)
		ফেরত প্রবেশ_বাফার + প্রবেশ_offset;
	যদি (min > আকার(প্রবেশ_বাফার))
		die(Q_("cannot fill %d byte",
		       "cannot fill %d bytes",
		       min),
		    min);
	flush();
	করো {
		ssize_t ret = xread(প্রবেশ_fd, প্রবেশ_বাফার + প্রবেশ_len,
				আকার(প্রবেশ_বাফার) - প্রবেশ_len);
		যদি (ret <= 0) {
			যদি (!ret)
				die(_("early EOF"));
			die_errno(_("read error on input"));
		}
		প্রবেশ_len += ret;
		যদি (from_stdin)
			display_throughput(progress, consumed_bytes + প্রবেশ_len);
	} যখন (প্রবেশ_len < min);
	ফেরত প্রবেশ_বাফার;
}

স্থির শূন্য use(পূর্ণসংখ্যা bytes)
{
	যদি (bytes > প্রবেশ_len)
		die(_("used more bytes than were available"));
	প্রবেশ_crc32 = crc32(প্রবেশ_crc32, প্রবেশ_বাফার + প্রবেশ_offset, bytes);
	প্রবেশ_len -= bytes;
	প্রবেশ_offset += bytes;

	/* make sure off_t is sufficiently large not to wrap */
	যদি (signed_add_overflows(consumed_bytes, bytes))
		die(_("pack too large for current definition of off_t"));
	consumed_bytes += bytes;
	যদি (max_প্রবেশ_আকার && consumed_bytes > max_প্রবেশ_আকার) {
		কাঠামো strbuf আকার_limit = STRBUF_INIT;
		strbuf_humanise_bytes(&আকার_limit, max_প্রবেশ_আকার);
		die(_("pack exceeds maximum allowed size (%s)"),
		    আকার_limit.buf);
	}
}

স্থির ধ্রুবক অক্ষর *open_pack_file(ধ্রুবক অক্ষর *pack_name)
{
	যদি (from_stdin) {
		প্রবেশ_fd = 0;
		যদি (!pack_name) {
			কাঠামো strbuf tmp_file = STRBUF_INIT;
			output_fd = odb_mkstemp(the_repository->objects, &tmp_file,
						"pack/tmp_pack_XXXXXX");
			pack_name = strbuf_detach(&tmp_file, ফাঁকা);
		} অন্যথা {
			output_fd = xopen(pack_name, O_CREAT|O_EXCL|O_RDWR, 0600);
		}
		nothread_তথ্য.pack_fd = output_fd;
	} অন্যথা {
		প্রবেশ_fd = xopen(pack_name, O_RDONLY);
		output_fd = -1;
		nothread_তথ্য.pack_fd = প্রবেশ_fd;
	}
	git_hash_init(&প্রবেশ_ctx, the_hash_algo);
	ফেরত pack_name;
}

স্থির শূন্য parse_pack_header(শূন্য)
{
	unsigned অক্ষর *hdr = fill(আকার(কাঠামো pack_header));

	/* Header consistency check */
	যদি (get_be32(hdr) != PACK_SIGNATURE)
		die(_("pack signature mismatch"));
	hdr += 4;
	যদি (!pack_version_ok_native(get_be32(hdr)))
		die(_("pack version %"PRIu32" unsupported"),
		    get_be32(hdr));
	hdr += 4;

	nr_objects = get_be32(hdr);
	use(আকার(কাঠামো pack_header));
}

__attribute__((format (printf, 2, 3)))
স্থির NORETURN শূন্য bad_object(off_t offset, ধ্রুবক অক্ষর *format, ...)
{
	va_list params;
	অক্ষর buf[1024];

	va_start(params, format);
	vsnprintf(buf, আকার(buf), format, params);
	va_end(params);
	die(_("pack has bad object at offset %"PRIuMAX": %s"),
	    (uintmax_t)offset, buf);
}

স্থির অন্তঃরেখ কাঠামো thread_local_তথ্য *get_thread_তথ্য(শূন্য)
{
	যদি (HAVE_THREADS) {
		যদি (threads_active)
			ফেরত pthread_getspecific(চাবি);
		প্রতিজ্ঞা(!threads_active &&
		       "This should only be reached when all threads are gone");
	}
	ফেরত &nothread_তথ্য;
}

স্থির শূন্য set_thread_তথ্য(কাঠামো thread_local_তথ্য *তথ্য)
{
	যদি (threads_active)
		pthread_setspecific(চাবি, তথ্য);
}

স্থির শূন্য free_base_তথ্য(কাঠামো base_তথ্য *c)
{
	যদি (c->তথ্য) {
		FREE_AND_NULL(c->তথ্য);
		base_cache_used -= c->আকার_2;
	}
}

স্থির শূন্য prune_base_তথ্য(কাঠামো base_তথ্য *retain)
{
	কাঠামো list_শীর্ষ *pos;

	যদি (base_cache_used <= base_cache_limit)
		ফেরত;

	list_for_each_পূর্ব(pos, &done_শীর্ষ) {
		কাঠামো base_তথ্য *b = list_entry(pos, কাঠামো base_তথ্য, list);
		যদি (b->retain_তথ্য || b == retain)
			চালু;
		যদি (b->তথ্য) {
			free_base_তথ্য(b);
			যদি (base_cache_used <= base_cache_limit)
				ফেরত;
		}
	}

	list_for_each_পূর্ব(pos, &work_শীর্ষ) {
		কাঠামো base_তথ্য *b = list_entry(pos, কাঠামো base_তথ্য, list);
		যদি (b->retain_তথ্য || b == retain)
			চালু;
		যদি (b->তথ্য) {
			free_base_তথ্য(b);
			যদি (base_cache_used <= base_cache_limit)
				ফেরত;
		}
	}
}

স্থির পূর্ণসংখ্যা is_delta_type(গণনা object_type type)
{
	ফেরত (type == OBJ_REF_DELTA || type == OBJ_OFS_DELTA);
}

স্থির শূন্য *unpack_entry_তথ্য(off_t offset, আকার_t আকার_2,
			       গণনা object_type type, কাঠামো object_id *oid)
{
	স্থির অক্ষর fixed_buf[8192];
	পূর্ণসংখ্যা status;
	git_zstream stream;
	শূন্য *buf;
	কাঠামো git_hash_ctx c;
	অক্ষর hdr[32];
	পূর্ণসংখ্যা hdrlen;

	যদি (!is_delta_type(type)) {
		hdrlen = format_object_header(hdr, আকার(hdr), type, আকার_2);
		git_hash_init(&c, the_hash_algo);
		git_hash_update(&c, hdr, hdrlen);
	} অন্যথা
		oid = ফাঁকা;
	যদি (type == OBJ_BLOB &&
	    আকার_2 > repo_settings_get_big_file_threshold(the_repository))
		buf = fixed_buf;
	অন্যথা
		buf = xmallocz(আকার_2);

	memset(&stream, 0, আকার(stream));
	git_inflate_init(&stream);
	stream.পরবর্তী_out = buf;
	stream.avail_out = buf == fixed_buf ? আকার(fixed_buf) : আকার_2;

	করো {
		unsigned অক্ষর *last_out = stream.পরবর্তী_out;
		stream.পরবর্তী_in = fill(1);
		stream.avail_in = প্রবেশ_len;
		status = git_inflate(&stream, 0);
		use(প্রবেশ_len - stream.avail_in);
		যদি (oid)
			git_hash_update(&c, last_out, stream.পরবর্তী_out - last_out);
		যদি (buf == fixed_buf) {
			stream.পরবর্তী_out = buf;
			stream.avail_out = আকার(fixed_buf);
		}
	} যখন (status == Z_OK);
	যদি (stream.যোগ_out != আকার_2 || status != Z_STREAM_END)
		bad_object(offset, _("inflate returned %d"), status);
	git_inflate_end(&stream);
	যদি (oid)
		git_hash_final_oid(oid, &c);
	ফেরত buf == fixed_buf ? ফাঁকা : buf;
}

স্থির শূন্য *unpack_raw_entry(কাঠামো object_entry *obj,
			      off_t *ofs_offset,
			      কাঠামো object_id *ref_oid,
			      কাঠামো object_id *oid)
{
	unsigned অক্ষর *p;
	আকার_t আকার_2, c;
	off_t base_offset;
	unsigned shift;
	শূন্য *তথ্য;

	obj->idx.offset = consumed_bytes;
	প্রবেশ_crc32 = crc32(0, ফাঁকা, 0);

	p = fill(1);
	c = *p;
	use(1);
	obj->type = (c >> 4) & 7;
	আকার_2 = (c & 15);
	shift = 4;
	যখন (c & 0x80) {
		যদি ((bitsizeof(আকার_t) - 7) < shift)
			die(_("object size too large for this platform"));
		p = fill(1);
		c = *p;
		use(1);
		আকার_2 += (c & 0x7f) << shift;
		shift += 7;
	}
	obj->আকার_2 = আকার_2;

	নির্বাচন (obj->type) {
	ক্ষেত্র OBJ_REF_DELTA:
		oidread(ref_oid, fill(the_hash_algo->rawsz),
			the_repository->hash_algo);
		use(the_hash_algo->rawsz);
		বিরতি;
	ক্ষেত্র OBJ_OFS_DELTA:
		p = fill(1);
		c = *p;
		use(1);
		base_offset = c & 127;
		যখন (c & 128) {
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
		বিরতি;
	ক্ষেত্র OBJ_COMMIT:
	ক্ষেত্র OBJ_বৃক্ষ:
	ক্ষেত্র OBJ_BLOB:
	ক্ষেত্র OBJ_TAG:
		বিরতি;
	পূর্বনির্ধারিত:
		bad_object(obj->idx.offset, _("unknown object type %d"), obj->type);
	}
	obj->hdr_আকার = consumed_bytes - obj->idx.offset;

	তথ্য = unpack_entry_তথ্য(obj->idx.offset, obj->আকার_2, obj->type, oid);
	obj->idx.crc32 = প্রবেশ_crc32;
	ফেরত তথ্য;
}

স্থির শূন্য *unpack_তথ্য(কাঠামো object_entry *obj,
			 পূর্ণসংখ্যা (*consume)(ধ্রুবক unsigned অক্ষর *, unsigned long, শূন্য *),
			 শূন্য *cb_তথ্য)
{
	off_t from = obj[0].idx.offset + obj[0].hdr_আকার;
	off_t len = obj[1].idx.offset - from;
	unsigned অক্ষর *তথ্য, *inbuf;
	git_zstream stream;
	পূর্ণসংখ্যা status;

	তথ্য = xmallocz(consume ? 64*1024 : obj->আকার_2);
	inbuf = xmalloc((len < 64*1024) ? (পূর্ণসংখ্যা)len : 64*1024);

	memset(&stream, 0, আকার(stream));
	git_inflate_init(&stream);
	stream.পরবর্তী_out = তথ্য;
	stream.avail_out = consume ? 64*1024 : obj->আকার_2;

	করো {
		ssize_t n = (len < 64*1024) ? (ssize_t)len : 64*1024;
		n = xpread(get_thread_তথ্য()->pack_fd, inbuf, n, from);
		যদি (n < 0)
			die_errno(_("cannot pread pack file"));
		যদি (!n)
			die(Q_("premature end of pack file, %"PRIuMAX" byte missing",
			       "premature end of pack file, %"PRIuMAX" bytes missing",
			       len),
			    (uintmax_t)len);
		from += n;
		len -= n;
		stream.পরবর্তী_in = inbuf;
		stream.avail_in = n;
		যদি (!consume)
			status = git_inflate(&stream, 0);
		অন্যথা {
			করো {
				status = git_inflate(&stream, 0);
				যদি (consume(তথ্য, stream.পরবর্তী_out - তথ্য, cb_তথ্য)) {
					মুক্ত(inbuf);
					মুক্ত(তথ্য);
					ফেরত ফাঁকা;
				}
				stream.পরবর্তী_out = তথ্য;
				stream.avail_out = 64*1024;
			} যখন (status == Z_OK && stream.avail_in);
		}
	} যখন (len && status == Z_OK && !stream.avail_in);

	/* This has been inflated OK when first encountered, so... */
	যদি (status != Z_STREAM_END || stream.যোগ_out != obj->আকার_2)
		die(_("serious inflate inconsistency"));

	git_inflate_end(&stream);
	মুক্ত(inbuf);
	যদি (consume) {
		FREE_AND_NULL(তথ্য);
	}
	ফেরত তথ্য;
}

স্থির শূন্য *get_তথ্য_from_pack(কাঠামো object_entry *obj)
{
	ফেরত unpack_তথ্য(obj, ফাঁকা, ফাঁকা);
}

স্থির পূর্ণসংখ্যা compare_ofs_delta_bases(off_t offset1, off_t offset2,
				   গণনা object_type type1,
				   গণনা object_type type2)
{
	পূর্ণসংখ্যা cmp = type1 - type2;
	যদি (cmp)
		ফেরত cmp;
	ফেরত offset1 < offset2 ? -1 :
	       offset1 > offset2 ?  1 :
	       0;
}

স্থির পূর্ণসংখ্যা find_ofs_delta(ধ্রুবক off_t offset)
{
	পূর্ণসংখ্যা first = 0, last = nr_ofs_deltas;

	যখন (first < last) {
		পূর্ণসংখ্যা পরবর্তী = first + (last - first) / 2;
		কাঠামো ofs_delta_entry *delta = &ofs_deltas[পরবর্তী];
		পূর্ণসংখ্যা cmp;

		cmp = compare_ofs_delta_bases(offset, delta->offset,
					      OBJ_OFS_DELTA,
					      objects[delta->obj_no].type);
		যদি (!cmp)
			ফেরত পরবর্তী;
		যদি (cmp < 0) {
			last = পরবর্তী;
			চালু;
		}
		first = পরবর্তী+1;
	}
	ফেরত -first-1;
}

স্থির শূন্য find_ofs_delta_children(off_t offset,
				    পূর্ণসংখ্যা *first_সূচক, পূর্ণসংখ্যা *last_সূচক)
{
	পূর্ণসংখ্যা first = find_ofs_delta(offset);
	পূর্ণসংখ্যা last = first;
	পূর্ণসংখ্যা end = nr_ofs_deltas - 1;

	যদি (first < 0) {
		*first_সূচক = 0;
		*last_সূচক = -1;
		ফেরত;
	}
	যখন (first > 0 && ofs_deltas[first - 1].offset == offset)
		--first;
	যখন (last < end && ofs_deltas[last + 1].offset == offset)
		++last;
	*first_সূচক = first;
	*last_সূচক = last;
}

স্থির পূর্ণসংখ্যা compare_ref_delta_bases(ধ্রুবক কাঠামো object_id *oid1,
				   ধ্রুবক কাঠামো object_id *oid2,
				   গণনা object_type type1,
				   গণনা object_type type2)
{
	পূর্ণসংখ্যা cmp = type1 - type2;
	যদি (cmp)
		ফেরত cmp;
	ফেরত oidcmp(oid1, oid2);
}

স্থির পূর্ণসংখ্যা find_ref_delta(ধ্রুবক কাঠামো object_id *oid)
{
	পূর্ণসংখ্যা first = 0, last = nr_ref_deltas;

	যখন (first < last) {
		পূর্ণসংখ্যা পরবর্তী = first + (last - first) / 2;
		কাঠামো ref_delta_entry *delta = &ref_deltas[পরবর্তী];
		পূর্ণসংখ্যা cmp;

		cmp = compare_ref_delta_bases(oid, &delta->oid,
					      OBJ_REF_DELTA,
					      objects[delta->obj_no].type);
		যদি (!cmp)
			ফেরত পরবর্তী;
		যদি (cmp < 0) {
			last = পরবর্তী;
			চালু;
		}
		first = পরবর্তী+1;
	}
	ফেরত -first-1;
}

স্থির শূন্য find_ref_delta_children(ধ্রুবক কাঠামো object_id *oid,
				    পূর্ণসংখ্যা *first_সূচক, পূর্ণসংখ্যা *last_সূচক)
{
	পূর্ণসংখ্যা first = find_ref_delta(oid);
	পূর্ণসংখ্যা last = first;
	পূর্ণসংখ্যা end = nr_ref_deltas - 1;

	যদি (first < 0) {
		*first_সূচক = 0;
		*last_সূচক = -1;
		ফেরত;
	}
	যখন (first > 0 && oideq(&ref_deltas[first - 1].oid, oid))
		--first;
	যখন (last < end && oideq(&ref_deltas[last + 1].oid, oid))
		++last;
	*first_সূচক = first;
	*last_সূচক = last;
}

কাঠামো compare_তথ্য {
	কাঠামো object_entry *entry;
	কাঠামো odb_stream *st;
	unsigned অক্ষর *buf;
	unsigned long buf_আকার;
};

স্থির পূর্ণসংখ্যা compare_objects(ধ্রুবক unsigned অক্ষর *buf, unsigned long আকার_2,
			   শূন্য *cb_তথ্য)
{
	কাঠামো compare_তথ্য *তথ্য = cb_তথ্য;

	যদি (তথ্য->buf_আকার < আকার_2) {
		মুক্ত(তথ্য->buf);
		তথ্য->buf = xmalloc(আকার_2);
		তথ্য->buf_আকার = আকার_2;
	}

	যখন (আকার_2) {
		ssize_t len = odb_stream_read(তথ্য->st, তথ্য->buf, আকার_2);
		যদি (len == 0)
			die(_("SHA1 COLLISION FOUND WITH %s !"),
			    oid_to_hex(&তথ্য->entry->idx.oid));
		যদি (len < 0)
			die(_("unable to read %s"),
			    oid_to_hex(&তথ্য->entry->idx.oid));
		যদি (memcmp(buf, তথ্য->buf, len))
			die(_("SHA1 COLLISION FOUND WITH %s !"),
			    oid_to_hex(&তথ্য->entry->idx.oid));
		আকার_2 -= len;
		buf += len;
	}
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা check_collison(কাঠামো object_entry *entry)
{
	কাঠামো compare_তথ্য তথ্য;

	যদি (entry->আকার_2 <= repo_settings_get_big_file_threshold(the_repository) ||
	    entry->type != OBJ_BLOB)
		ফেরত -1;

	memset(&তথ্য, 0, আকার(তথ্য));
	তথ্য.entry = entry;
	তথ্য.st = odb_stream_from_object(the_repository->objects, &entry->idx.oid, ফাঁকা);
	যদি (!তথ্য.st)
		ফেরত -1;
	যদি (তথ্য.st->আকার_2 != entry->আকার_2 || তথ্য.st->type != entry->type)
		die(_("SHA1 COLLISION FOUND WITH %s !"),
		    oid_to_hex(&entry->idx.oid));
	unpack_তথ্য(entry, compare_objects, &তথ্য);
	odb_stream_close(তথ্য.st);
	মুক্ত(তথ্য.buf);
	ফেরত 0;
}

স্থির শূন্য record_outgoing_link(ধ্রুবক কাঠামো object_id *oid)
{
	oidset_ঢোকাও(&outgoing_links, oid);
}

স্থির শূন্য maybe_record_name_entry(ধ্রুবক কাঠামো name_entry *entry)
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

স্থির শূন্য do_record_outgoing_links(কাঠামো object *obj)
{
	যদি (obj->type == OBJ_বৃক্ষ) {
		কাঠামো বৃক্ষ *বৃক্ষ = (কাঠামো বৃক্ষ *)obj;
		কাঠামো বৃক্ষ_desc desc;
		কাঠামো name_entry entry;
		যদি (init_বৃক্ষ_desc_gently(&desc, &বৃক্ষ->object.oid,
					  বৃক্ষ->বাফার, বৃক্ষ->আকার_2, 0))
			/*
			 * Error messages are given when packs are
			 * verified, so do not print any here.
			 */
			ফেরত;
		যখন (বৃক্ষ_entry_gently(&desc, &entry))
			maybe_record_name_entry(&entry);
	} অন্যথা যদি (obj->type == OBJ_COMMIT) {
		কাঠামো commit *commit = (কাঠামো commit *) obj;
		কাঠামো commit_list *parents = commit->parents;

		record_outgoing_link(get_commit_বৃক্ষ_oid(commit));
		জন্য (; parents; parents = parents->পরবর্তী)
			record_outgoing_link(&parents->item->object.oid);
	} অন্যথা যদি (obj->type == OBJ_TAG) {
		কাঠামো tag *tag = (কাঠামো tag *) obj;
		record_outgoing_link(get_tagged_oid(tag));
	}
}

স্থির শূন্য sha1_object(ধ্রুবক শূন্য *তথ্য, কাঠামো object_entry *obj_entry,
			unsigned long আকার_2, গণনা object_type type,
			ধ্রুবক কাঠামো object_id *oid)
{
	শূন্য *new_তথ্য = ফাঁকা;
	পূর্ণসংখ্যা collision_test_needed = 0;

	প্রতিজ্ঞা(তথ্য || obj_entry);

	যদি (startup_info->have_repository) {
		read_lock();
		collision_test_needed = odb_has_object(the_repository->objects, oid,
						       ODB_HAS_OBJECT_FETCH_PROMISOR);
		read_unlock();
	}

	যদি (collision_test_needed && !তথ্য) {
		read_lock();
		যদি (!check_collison(obj_entry))
			collision_test_needed = 0;
		read_unlock();
	}
	যদি (collision_test_needed) {
		শূন্য *has_তথ্য;
		গণনা object_type has_type;
		আকার_t has_আকার;
		read_lock();
		has_type = odb_read_object_info(the_repository->objects, oid, &has_আকার);
		যদি (has_type < 0)
			die(_("cannot read existing object info %s"), oid_to_hex(oid));
		যদি (has_type != type || has_আকার != আকার_2)
			die(_("SHA1 COLLISION FOUND WITH %s !"), oid_to_hex(oid));
		has_তথ্য = odb_read_object(the_repository->objects, oid,
					   &has_type, &has_আকার);
		read_unlock();
		যদি (!তথ্য)
			তথ্য = new_তথ্য = get_তথ্য_from_pack(obj_entry);
		যদি (!has_তথ্য)
			die(_("cannot read existing object %s"), oid_to_hex(oid));
		যদি (আকার_2 != has_আকার || type != has_type ||
		    memcmp(তথ্য, has_তথ্য, আকার_2) != 0)
			die(_("SHA1 COLLISION FOUND WITH %s !"), oid_to_hex(oid));
		মুক্ত(has_তথ্য);
	}

	যদি (strict || do_fsck_object || record_outgoing_links) {
		read_lock();
		যদি (type == OBJ_BLOB) {
			কাঠামো blob *blob = lookup_blob(the_repository, oid);
			যদি (blob)
				blob->object.flags |= FLAG_CHECKED;
			অন্যথা
				die(_("invalid blob object %s"), oid_to_hex(oid));
			যদি (do_fsck_object &&
			    fsck_object(&blob->object, (শূন্য *)তথ্য, আকার_2, &fsck_options))
				die(_("fsck error in packed object"));
		} অন্যথা {
			কাঠামো object *obj;
			পূর্ণসংখ্যা eaten;
			শূন্য *buf = (শূন্য *) তথ্য;

			প্রতিজ্ঞা(তথ্য && "data can only be NULL for large _blobs_");

			/*
			 * we do not need to free the memory here, as the
			 * buf is deleted by the caller.
			 */
			obj = parse_object_বাফার(the_repository, oid, type,
						  আকার_2, buf,
						  &eaten);
			যদি (!obj)
				die(_("invalid %s"), type_name(type));
			যদি (do_fsck_object &&
			    fsck_object(obj, buf, আকার_2, &fsck_options))
				die(_("fsck error in packed object"));
			যদি (strict && fsck_walk(obj, ফাঁকা, &fsck_options))
				die(_("Not all child objects of %s are reachable"), oid_to_hex(&obj->oid));
			যদি (record_outgoing_links)
				do_record_outgoing_links(obj);

			যদি (obj->type == OBJ_বৃক্ষ) {
				কাঠামো বৃক্ষ *item = (কাঠামো বৃক্ষ *) obj;
				item->বাফার = ফাঁকা;
				obj->parsed = 0;
			}
			যদি (obj->type == OBJ_COMMIT) {
				কাঠামো commit *commit = (কাঠামো commit *) obj;
				যদি (detach_commit_বাফার(commit, ফাঁকা) != তথ্য)
					BUG("parse_object_buffer transmogrified our buffer");
			}
			obj->flags |= FLAG_CHECKED;
		}
		read_unlock();
	}

	মুক্ত(new_তথ্য);
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
স্থির শূন্য *get_base_তথ্য(কাঠামো base_তথ্য *c)
{
	যদি (!c->তথ্য) {
		কাঠামো object_entry *obj = c->obj;
		কাঠামো base_তথ্য **delta = ফাঁকা;
		পূর্ণসংখ্যা delta_nr = 0, delta_alloc = 0;

		যখন (is_delta_type(c->obj->type) && !c->তথ্য) {
			ALLOC_GROW(delta, delta_nr + 1, delta_alloc);
			delta[delta_nr++] = c;
			c = c->base;
		}
		যদি (!delta_nr) {
			c->তথ্য = get_তথ্য_from_pack(obj);
			c->আকার_2 = obj->আকার_2;
			base_cache_used += c->আকার_2;
			prune_base_তথ্য(c);
		}
		জন্য (; delta_nr > 0; delta_nr--) {
			শূন্য *base, *raw;
			c = delta[delta_nr - 1];
			obj = c->obj;
			base = get_base_তথ্য(c->base);
			raw = get_তথ্য_from_pack(obj);
			c->তথ্য = patch_delta(
				base, c->base->আকার_2,
				raw, obj->আকার_2,
				&c->আকার_2);
			মুক্ত(raw);
			যদি (!c->তথ্য)
				bad_object(obj->idx.offset, _("failed to apply delta"));
			base_cache_used += c->আকার_2;
			prune_base_তথ্য(c);
		}
		মুক্ত(delta);
	}
	ফেরত c->তথ্য;
}

স্থির কাঠামো base_তথ্য *make_base(কাঠামো object_entry *obj,
				   কাঠামো base_তথ্য *parent)
{
	কাঠামো base_তথ্য *base = xcalloc(1, আকার(কাঠামো base_তথ্য));
	base->base = parent;
	base->obj = obj;
	find_ref_delta_children(&obj->idx.oid,
				&base->ref_first, &base->ref_last);
	find_ofs_delta_children(obj->idx.offset,
				&base->ofs_first, &base->ofs_last);
	base->children_remaining = base->ref_last - base->ref_first +
		base->ofs_last - base->ofs_first + 2;
	ফেরত base;
}

স্থির কাঠামো base_তথ্য *resolve_delta(কাঠামো object_entry *delta_obj,
				       কাঠামো base_তথ্য *base)
{
	শূন্য *delta_তথ্য, *ফলাফল_তথ্য;
	কাঠামো base_তথ্য *ফলাফল;
	আকার_t ফলাফল_আকার;

	যদি (show_stat) {
		পূর্ণসংখ্যা i = delta_obj - objects;
		পূর্ণসংখ্যা j = base->obj - objects;
		obj_stat[i].delta_depth = obj_stat[j].delta_depth + 1;
		deepest_delta_lock();
		যদি (deepest_delta < obj_stat[i].delta_depth)
			deepest_delta = obj_stat[i].delta_depth;
		deepest_delta_unlock();
		obj_stat[i].base_object_no = j;
	}
	delta_তথ্য = get_তথ্য_from_pack(delta_obj);
	প্রতিজ্ঞা(base->তথ্য);
	ফলাফল_তথ্য = patch_delta(base->তথ্য, base->আকার_2,
				  delta_তথ্য, delta_obj->আকার_2, &ফলাফল_আকার);
	মুক্ত(delta_তথ্য);
	যদি (!ফলাফল_তথ্য)
		bad_object(delta_obj->idx.offset, _("failed to apply delta"));
	hash_object_file(the_hash_algo, ফলাফল_তথ্য, ফলাফল_আকার,
			 delta_obj->real_type, &delta_obj->idx.oid);
	sha1_object(ফলাফল_তথ্য, ফাঁকা, ফলাফল_আকার, delta_obj->real_type,
		    &delta_obj->idx.oid);

	ফলাফল = make_base(delta_obj, base);
	ফলাফল->তথ্য = ফলাফল_তথ্য;
	ফলাফল->আকার_2 = ফলাফল_আকার;

	গণক_lock();
	nr_resolved_deltas++;
	গণক_unlock();

	ফেরত ফলাফল;
}

স্থির পূর্ণসংখ্যা compare_ofs_delta_entry(ধ্রুবক শূন্য *a, ধ্রুবক শূন্য *b)
{
	ধ্রুবক কাঠামো ofs_delta_entry *delta_a = a;
	ধ্রুবক কাঠামো ofs_delta_entry *delta_b = b;

	ফেরত delta_a->offset < delta_b->offset ? -1 :
	       delta_a->offset > delta_b->offset ?  1 :
	       0;
}

স্থির পূর্ণসংখ্যা compare_ref_delta_entry(ধ্রুবক শূন্য *a, ধ্রুবক শূন্য *b)
{
	ধ্রুবক কাঠামো ref_delta_entry *delta_a = a;
	ধ্রুবক কাঠামো ref_delta_entry *delta_b = b;

	ফেরত oidcmp(&delta_a->oid, &delta_b->oid);
}

স্থির শূন্য *threaded_second_pass(শূন্য *তথ্য)
{
	যদি (তথ্য)
		set_thread_তথ্য(তথ্য);
	জন্য (;;) {
		কাঠামো base_তথ্য *parent = ফাঁকা;
		কাঠামো object_entry *child_obj = ফাঁকা;
		কাঠামো base_তথ্য *child = ফাঁকা;

		গণক_lock();
		display_progress(progress, nr_resolved_deltas);
		গণক_unlock();

		work_lock();
		যদি (list_empty(&work_শীর্ষ)) {
			/*
			 * Take an object from the object array.
			 */
			যখন (nr_dispatched < nr_objects &&
			       is_delta_type(objects[nr_dispatched].type))
				nr_dispatched++;
			যদি (nr_dispatched >= nr_objects) {
				work_unlock();
				বিরতি;
			}
			child_obj = &objects[nr_dispatched++];
		} অন্যথা {
			/*
			 * Peek at the top of the stack, and take a child from
			 * it.
			 */
			parent = list_first_entry(&work_শীর্ষ, কাঠামো base_তথ্য,
						  list);

			যখন (parent->ref_first <= parent->ref_last) {
				পূর্ণসংখ্যা offset = ref_deltas[parent->ref_first++].obj_no;
				child_obj = objects + offset;
				যদি (child_obj->real_type != OBJ_REF_DELTA) {
					child_obj = ফাঁকা;
					চালু;
				}
				child_obj->real_type = parent->obj->real_type;
				বিরতি;
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
				list_add(&parent->list, &done_শীর্ষ);
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
			get_base_তথ্য(parent);
			parent->retain_তথ্য++;
		}
		work_unlock();

		যদি (child_obj) {
			যদি (parent) {
				child = resolve_delta(child_obj, parent);
				যদি (!child->children_remaining)
					FREE_AND_NULL(child->তথ্য);
			} অন্যথা{
				child = make_base(child_obj, ফাঁকা);
				যদি (child->children_remaining) {
					/*
					 * Since this child has its own delta children,
					 * we will need this data in the future.
					 * Inflate now so that future iterations will
					 * have access to this object's data while
					 * outside the work mutex.
					 */
					child->তথ্য = get_তথ্য_from_pack(child_obj);
					child->আকার_2 = child_obj->আকার_2;
				}
			}
		}

		work_lock();
		যদি (parent)
			parent->retain_তথ্য--;

		যদি (child && child->তথ্য) {
			/*
			 * This child has its own children, so add it to
			 * work_head.
			 */
			list_add(&child->list, &work_শীর্ষ);
			base_cache_used += child->আকার_2;
			prune_base_তথ্য(ফাঁকা);
		} অন্যথা যদি (child) {
			/*
			 * This child does not have its own children. It may be
			 * the last descendant of its ancestors; free those
			 * that we can.
			 */
			কাঠামো base_তথ্য *p = parent;

			যখন (p) {
				কাঠামো base_তথ্য *পরবর্তী_p;

				p->children_remaining--;
				যদি (p->children_remaining)
					বিরতি;

				পরবর্তী_p = p->base;
				free_base_তথ্য(p);
				list_del(&p->list);
				মুক্ত(p);

				p = পরবর্তী_p;
			}
			FREE_AND_NULL(child);
		}
		work_unlock();
	}
	ফেরত ফাঁকা;
}

/*
 * First pass:
 * - find locations of all objects;
 * - calculate SHA1 of all non-delta objects;
 * - remember base (SHA1 or offset) for all deltas.
 */
স্থির শূন্য parse_pack_objects(unsigned অক্ষর *hash)
{
	পূর্ণসংখ্যা i, nr_delays = 0;
	কাঠামো ofs_delta_entry *ofs_delta = ofs_deltas;
	কাঠামো object_id ref_delta_oid;
	কাঠামো stat st;
	কাঠামো git_hash_ctx tmp_ctx;

	যদি (verbose)
		progress = start_progress(
				the_repository,
				progress_title ? progress_title :
				from_stdin ? _("Receiving objects") : _("Indexing objects"),
				nr_objects);
	জন্য (i = 0; i < nr_objects; i++) {
		কাঠামো object_entry *obj = &objects[i];
		শূন্য *তথ্য = unpack_raw_entry(obj, &ofs_delta->offset,
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
		} অন্যথা যদি (!তথ্য) {
			/* large blobs, check later */
			obj->real_type = OBJ_BAD;
			nr_delays++;
		} অন্যথা
			sha1_object(তথ্য, ফাঁকা, obj->আকার_2, obj->type,
				    &obj->idx.oid);
		মুক্ত(তথ্য);
		display_progress(progress, i+1);
	}
	objects[i].idx.offset = consumed_bytes;
	stop_progress(&progress);

	/* Check pack integrity */
	flush();
	git_hash_init(&tmp_ctx, the_hash_algo);
	git_hash_clone(&tmp_ctx, &প্রবেশ_ctx);
	git_hash_final(hash, &tmp_ctx);
	যদি (!hasheq(fill(the_hash_algo->rawsz), hash, the_repository->hash_algo))
		die(_("pack is corrupted (SHA1 mismatch)"));
	use(the_hash_algo->rawsz);

	/* If input_fd is a file, we should have reached its end now. */
	যদি (fstat(প্রবেশ_fd, &st))
		die_errno(_("cannot fstat packfile"));
	যদি (S_ISREG(st.st_mode) &&
			lseek(প্রবেশ_fd, 0, SEEK_CUR) - প্রবেশ_len != st.st_আকার)
		die(_("pack has junk at the end"));

	জন্য (i = 0; i < nr_objects; i++) {
		কাঠামো object_entry *obj = &objects[i];
		যদি (obj->real_type != OBJ_BAD)
			চালু;
		obj->real_type = obj->type;
		sha1_object(ফাঁকা, obj, obj->আকার_2, obj->type,
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
স্থির শূন্য resolve_deltas(কাঠামো pack_idx_option *opts)
{
	পূর্ণসংখ্যা i;

	যদি (!nr_ofs_deltas && !nr_ref_deltas)
		ফেরত;

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
		জন্য (i = 0; i < nr_threads; i++) {
			পূর্ণসংখ্যা ret = pthread_create(&thread_তথ্য[i].thread, ফাঁকা,
						 threaded_second_pass, thread_তথ্য + i);
			যদি (ret)
				die(_("unable to create thread: %s"),
				    strerror(ret));
		}
		জন্য (i = 0; i < nr_threads; i++)
			pthread_join(thread_তথ্য[i].thread, ফাঁকা);
		cleanup_thread();
		ফেরত;
	}
	threaded_second_pass(&nothread_তথ্য);
}

/*
 * Third pass:
 * - append objects to convert thin pack to full pack if required
 * - write the final pack hash
 */
স্থির শূন্য fix_unresolved_deltas(কাঠামো hashfile *f);
স্থির শূন্য conclude_pack(পূর্ণসংখ্যা fix_thin_pack, ধ্রুবক অক্ষর *curr_pack, unsigned অক্ষর *pack_hash)
{
	যদি (nr_ref_deltas + nr_ofs_deltas == nr_resolved_deltas) {
		stop_progress(&progress);
		/* Flush remaining pack final hash. */
		flush();
		ফেরত;
	}

	যদি (fix_thin_pack) {
		কাঠামো hashfile *f;
		unsigned অক্ষর read_hash[GIT_MAX_RAWSZ], লেজ_hash[GIT_MAX_RAWSZ];
		কাঠামো strbuf বার্তা = STRBUF_INIT;
		পূর্ণসংখ্যা nr_unresolved = nr_ofs_deltas + nr_ref_deltas - nr_resolved_deltas;
		পূর্ণসংখ্যা nr_objects_initial = nr_objects;
		যদি (nr_unresolved <= 0)
			die(_("confusion beyond insanity"));
		REALLOC_ARRAY(objects, nr_objects + nr_unresolved + 1);
		memset(objects + nr_objects + 1, 0,
		       nr_unresolved * আকার(*objects));
		f = hashfd(the_repository->hash_algo, output_fd, curr_pack);
		fix_unresolved_deltas(f);
		strbuf_addf(&বার্তা, Q_("completed with %d local object",
				     "completed with %d local objects",
				     nr_objects - nr_objects_initial),
			    nr_objects - nr_objects_initial);
		stop_progress_বার্তা(&progress, বার্তা.buf);
		strbuf_release(&বার্তা);
		finalize_hashfile(f, লেজ_hash, FSYNC_COMPONENT_PACK, 0);
		hashcpy(read_hash, pack_hash, the_repository->hash_algo);
		fixup_pack_header_footer(the_hash_algo, output_fd, pack_hash,
					 curr_pack, nr_objects,
					 read_hash, consumed_bytes-the_hash_algo->rawsz);
		যদি (!hasheq(read_hash, লেজ_hash, the_repository->hash_algo))
			die(_("Unexpected tail checksum for %s "
			      "(disk corruption?)"), curr_pack);
	}
	যদি (nr_ofs_deltas + nr_ref_deltas != nr_resolved_deltas)
		die(Q_("pack has %d unresolved delta",
		       "pack has %d unresolved deltas",
		       nr_ofs_deltas + nr_ref_deltas - nr_resolved_deltas),
		    nr_ofs_deltas + nr_ref_deltas - nr_resolved_deltas);
}

স্থির পূর্ণসংখ্যা write_compressed(কাঠামো hashfile *f, শূন্য *in, unsigned পূর্ণসংখ্যা আকার_2)
{
	git_zstream stream;
	পূর্ণসংখ্যা status;
	unsigned অক্ষর outbuf[4096];
	কাঠামো repo_config_মানসমূহ *cfg = repo_config_মানসমূহ(the_repository);

	git_deflate_init(&stream, cfg->zlib_compression_level);
	stream.পরবর্তী_in = in;
	stream.avail_in = আকার_2;

	করো {
		stream.পরবর্তী_out = outbuf;
		stream.avail_out = আকার(outbuf);
		status = git_deflate(&stream, Z_FINISH);
		hashwrite(f, outbuf, আকার(outbuf) - stream.avail_out);
	} যখন (status == Z_OK);

	যদি (status != Z_STREAM_END)
		die(_("unable to deflate appended object (%d)"), status);
	আকার_2 = stream.যোগ_out;
	git_deflate_end(&stream);
	ফেরত আকার_2;
}

স্থির কাঠামো object_entry *append_obj_to_pack(কাঠামো hashfile *f,
			       ধ্রুবক unsigned অক্ষর *sha1, শূন্য *buf,
			       unsigned long আকার_2, গণনা object_type type)
{
	কাঠামো object_entry *obj = &objects[nr_objects++];
	unsigned অক্ষর header[10];
	unsigned long s = আকার_2;
	পূর্ণসংখ্যা n = 0;
	unsigned অক্ষর c = (type << 4) | (s & 15);
	s >>= 4;
	যখন (s) {
		header[n++] = c | 0x80;
		c = s & 0x7f;
		s >>= 7;
	}
	header[n++] = c;
	crc32_begin(f);
	hashwrite(f, header, n);
	obj[0].আকার_2 = আকার_2;
	obj[0].hdr_আকার = n;
	obj[0].type = type;
	obj[0].real_type = type;
	obj[1].idx.offset = obj[0].idx.offset + n;
	obj[1].idx.offset += write_compressed(f, buf, আকার_2);
	obj[0].idx.crc32 = crc32_end(f);
	hashflush(f);
	oidread(&obj->idx.oid, sha1, the_repository->hash_algo);
	ফেরত obj;
}

স্থির পূর্ণসংখ্যা delta_pos_compare(ধ্রুবক শূন্য *_a, ধ্রুবক শূন্য *_b)
{
	কাঠামো ref_delta_entry *a = *(কাঠামো ref_delta_entry **)_a;
	কাঠামো ref_delta_entry *b = *(কাঠামো ref_delta_entry **)_b;
	ফেরত a->obj_no - b->obj_no;
}

স্থির শূন্য fix_unresolved_deltas(কাঠামো hashfile *f)
{
	কাঠামো ref_delta_entry **sorted_by_pos;
	পূর্ণসংখ্যা i;

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
	জন্য (i = 0; i < nr_ref_deltas; i++)
		sorted_by_pos[i] = &ref_deltas[i];
	QSORT(sorted_by_pos, nr_ref_deltas, delta_pos_compare);

	যদি (repo_has_promisor_remote(the_repository)) {
		/*
		 * Prefetch the delta bases.
		 */
		কাঠামো oid_array to_fetch = OID_ARRAY_INIT;
		জন্য (i = 0; i < nr_ref_deltas; i++) {
			কাঠামো ref_delta_entry *d = sorted_by_pos[i];
			যদি (!odb_read_object_info_extended(the_repository->objects,
							   &d->oid, ফাঁকা,
							   OBJECT_INFO_FOR_PREFETCH))
				চালু;
			oid_array_append(&to_fetch, &d->oid);
		}
		promisor_remote_get_direct(the_repository,
					   to_fetch.oid, to_fetch.nr);
		oid_array_clear(&to_fetch);
	}

	জন্য (i = 0; i < nr_ref_deltas; i++) {
		কাঠামো ref_delta_entry *d = sorted_by_pos[i];
		গণনা object_type type;
		শূন্য *তথ্য;
		আকার_t আকার_2;

		যদি (objects[d->obj_no].real_type != OBJ_REF_DELTA)
			চালু;
		তথ্য = odb_read_object(the_repository->objects, &d->oid,
				       &type, &আকার_2);
		যদি (!তথ্য)
			চালু;

		যদি (check_object_signature(the_repository, &d->oid, তথ্য, আকার_2,
					   type) < 0)
			die(_("local object %s is corrupt"), oid_to_hex(&d->oid));

		/*
		 * Add this as an object to the objects array and call
		 * threaded_second_pass() (which will pick up the added
		 * object).
		 */
		append_obj_to_pack(f, d->oid.hash, তথ্য, আকার_2, type);
		মুক্ত(তথ্য);
		threaded_second_pass(ফাঁকা);

		display_progress(progress, nr_resolved_deltas);
	}
	মুক্ত(sorted_by_pos);
}

স্থির ধ্রুবক অক্ষর *derive_filename(ধ্রুবক অক্ষর *pack_name, ধ্রুবক অক্ষর *strip,
				   ধ্রুবক অক্ষর *suffix, কাঠামো strbuf *buf)
{
	আকার_t len;
	যদি (!strip_suffix(pack_name, strip, &len) || !len ||
	    pack_name[len - 1] != '.')
		die(_("packfile name '%s' does not end with '.%s'"),
		    pack_name, strip);
	strbuf_add(buf, pack_name, len);
	strbuf_addstr(buf, suffix);
	ফেরত buf->buf;
}

স্থির শূন্য write_special_file(ধ্রুবক অক্ষর *suffix, ধ্রুবক অক্ষর *বার্তা,
			       ধ্রুবক অক্ষর *pack_name, ধ্রুবক unsigned অক্ষর *hash,
			       ধ্রুবক অক্ষর **report)
{
	কাঠামো strbuf name_buf = STRBUF_INIT;
	ধ্রুবক অক্ষর *filename;
	পূর্ণসংখ্যা fd;
	পূর্ণসংখ্যা বার্তা_len = strlen(বার্তা);

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
		যদি (বার্তা_len > 0) {
			write_or_die(fd, বার্তা, বার্তা_len);
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

স্থির শূন্য rename_tmp_packfile(ধ্রুবক অক্ষর **final_name,
				ধ্রুবক অক্ষর *curr_name,
				কাঠামো strbuf *name, unsigned অক্ষর *hash,
				ধ্রুবক অক্ষর *ext, পূর্ণসংখ্যা make_read_only_if_same)
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

স্থির শূন্য final(ধ্রুবক অক্ষর *final_pack_name, ধ্রুবক অক্ষর *curr_pack_name,
		  ধ্রুবক অক্ষর *final_সূচক_name, ধ্রুবক অক্ষর *curr_সূচক_name,
		  ধ্রুবক অক্ষর *final_rev_সূচক_name, ধ্রুবক অক্ষর *curr_rev_সূচক_name,
		  ধ্রুবক অক্ষর *keep_বার্তা, ধ্রুবক অক্ষর *promisor_বার্তা,
		  unsigned অক্ষর *hash)
{
	ধ্রুবক অক্ষর *report = "pack";
	কাঠামো strbuf pack_name = STRBUF_INIT;
	কাঠামো strbuf সূচক_name = STRBUF_INIT;
	কাঠামো strbuf rev_সূচক_name = STRBUF_INIT;

	যদি (!from_stdin) {
		close(প্রবেশ_fd);
	} অন্যথা {
		fsync_component_or_die(FSYNC_COMPONENT_PACK, output_fd, curr_pack_name);
		যদি (close(output_fd))
			die_errno(_("error while closing pack file"));
	}

	যদি (keep_বার্তা)
		write_special_file("keep", keep_বার্তা, final_pack_name, hash,
				   &report);
	যদি (promisor_বার্তা)
		write_special_file("promisor", promisor_বার্তা, final_pack_name,
				   hash, ফাঁকা);

	rename_tmp_packfile(&final_pack_name, curr_pack_name, &pack_name,
			    hash, "pack", from_stdin);
	যদি (curr_rev_সূচক_name)
		rename_tmp_packfile(&final_rev_সূচক_name, curr_rev_সূচক_name,
				    &rev_সূচক_name, hash, "rev", 1);
	rename_tmp_packfile(&final_সূচক_name, curr_সূচক_name, &সূচক_name,
			    hash, "idx", 1);

	যদি (do_fsck_object && startup_info->have_repository) {
		কাঠামো odb_source_files *files =
			odb_source_files_downcast(the_repository->objects->source);
		packfile_store_load_pack(files->dirs->packed, final_সূচক_name, 0);
	}

	যদি (!from_stdin) {
		printf("%s\n", hash_to_hex(hash));
	} অন্যথা {
		কাঠামো strbuf buf = STRBUF_INIT;

		strbuf_addf(&buf, "%s\t%s\n", report, hash_to_hex(hash));
		write_or_die(1, buf.buf, buf.len);
		strbuf_release(&buf);

		/* Write the last part of the buffer to stdout */
		write_in_full(1, প্রবেশ_বাফার + প্রবেশ_offset, প্রবেশ_len);
	}

	strbuf_release(&rev_সূচক_name);
	strbuf_release(&সূচক_name);
	strbuf_release(&pack_name);
}

স্থির পূর্ণসংখ্যা git_সূচক_pack_config(ধ্রুবক অক্ষর *k, ধ্রুবক অক্ষর *v,
				 ধ্রুবক কাঠামো config_context *ctx, শূন্য *cb)
{
	কাঠামো pack_idx_option *opts = cb;

	যদি (!strcmp(k, "pack.indexversion")) {
		opts->version = git_config_int(k, v, ctx->kvi);
		যদি (opts->version > 2)
			die(_("bad pack.indexVersion=%"PRIu32), opts->version);
		ফেরত 0;
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
		ফেরত 0;
	}
	যদি (!strcmp(k, "pack.writereverseindex")) {
		যদি (git_config_bool(k, v))
			opts->flags |= WRITE_REV;
		অন্যথা
			opts->flags &= ~WRITE_REV;
	}
	যদি (!strcmp(k, "core.deltabasecachelimit")) {
		opts->delta_base_cache_limit = git_config_ulong(k, v, ctx->kvi);
		ফেরত 0;
	}
	ফেরত git_default_config(k, v, ctx, cb);
}

স্থির পূর্ণসংখ্যা cmp_uint32(ধ্রুবক শূন্য *a_, ধ্রুবক শূন্য *b_)
{
	uint32_t a = *((uint32_t *)a_);
	uint32_t b = *((uint32_t *)b_);

	ফেরত (a < b) ? -1 : (a != b);
}

স্থির শূন্য read_v2_anomalous_offsets(কাঠামো packed_git *p,
				      কাঠামো pack_idx_option *opts)
{
	ধ্রুবক uint32_t *idx1, *idx2;
	uint32_t i;

	/* The address of the 4-byte offset table */
	idx1 = (((ধ্রুবক uint32_t *)((ধ্রুবক uint8_t *)p->সূচক_তথ্য + p->crc_offset))
		+ (আকার_t)p->অঙ্ক_objects /* CRC32 table */
		);

	/* The address of the 8-byte offset table */
	idx2 = idx1 + p->অঙ্ক_objects;

	জন্য (i = 0; i < p->অঙ্ক_objects; i++) {
		uint32_t off = ntohl(idx1[i]);
		যদি (!(off & 0x80000000))
			চালু;
		off = off & 0x7fffffff;
		check_pack_সূচক_ptr(p, &idx2[off * 2]);
		যদি (idx2[off * 2])
			চালু;
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

স্থির শূন্য read_idx_option(কাঠামো pack_idx_option *opts, ধ্রুবক অক্ষর *pack_name)
{
	কাঠামো packed_git *p = add_packed_git(the_repository, pack_name,
					      strlen(pack_name), 1);

	যদি (!p)
		die(_("Cannot open existing pack file '%s'"), pack_name);
	যদি (open_pack_সূচক(p))
		die(_("Cannot open existing pack idx file for '%s'"), pack_name);

	/* Read the attributes from the existing idx file */
	opts->version = p->সূচক_version;

	যদি (opts->version == 2)
		read_v2_anomalous_offsets(p, opts);

	/*
	 * Get rid of the idx file as we do not need it anymore.
	 * NEEDSWORK: extract this bit from free_pack_by_name() in
	 * object-file.c, perhaps?  It shouldn't matter very much as we
	 * know we haven't installed this pack (hence we never have
	 * read anything from it).
	 */
	close_pack_সূচক(p);
	মুক্ত(p);
}

স্থির শূন্য show_pack_info(পূর্ণসংখ্যা stat_only)
{
	পূর্ণসংখ্যা i, baseobjects = nr_objects - nr_ref_deltas - nr_ofs_deltas;
	unsigned long *chain_histogram = ফাঁকা;

	যদি (deepest_delta)
		CALLOC_ARRAY(chain_histogram, deepest_delta);

	জন্য (i = 0; i < nr_objects; i++) {
		কাঠামো object_entry *obj = &objects[i];

		যদি (is_delta_type(obj->type))
			chain_histogram[obj_stat[i].delta_depth - 1]++;
		যদি (stat_only)
			চালু;
		printf("%s %-6s %"PRIuMAX" %"PRIuMAX" %"PRIuMAX,
		       oid_to_hex(&obj->idx.oid),
		       type_name(obj->real_type), (uintmax_t)obj->আকার_2,
		       (uintmax_t)(obj[1].idx.offset - obj->idx.offset),
		       (uintmax_t)obj->idx.offset);
		যদি (is_delta_type(obj->type)) {
			কাঠামো object_entry *bobj = &objects[obj_stat[i].base_object_no];
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
	জন্য (i = 0; i < deepest_delta; i++) {
		যদি (!chain_histogram[i])
			চালু;
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
	কাঠামো child_process cmd = CHILD_PROCESS_INIT;
	FILE *out;
	কাঠামো strbuf সারি = STRBUF_INIT;
	কাঠামো oidset_iter iter;
	কাঠামো object_id *oid;
	অক্ষর *base_name = ফাঁকা;

	যদি (!oidset_আকার(&outgoing_links))
		ফেরত;

	oidset_iter_init(&outgoing_links, &iter);
	যখন ((oid = oidset_iter_পরবর্তী(&iter))) {
		কাঠামো odb_source_info source_info;
		কাঠামো object_info info = {
			.source_infop = &source_info,
		};

		যদি (odb_read_object_info_extended(the_repository->objects, oid, &info, 0))
			/* Missing; assume it is a promisor object */
			চালু;
		যদি (source_info.source->type == ODB_SOURCE_PACKED &&
		    source_info.u.packed.pack->pack_promisor)
			চালু;

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
		ফেরত;

	close(cmd.in);

	out = xfdopen(cmd.out, "r");
	যখন (strbuf_getline_lf(&সারি, out) != EOF) {
		unsigned অক্ষর দ্বিমিক[GIT_MAX_RAWSZ];
		যদি (সারি.len != the_hash_algo->hexsz ||
		    !hex_to_bytes(দ্বিমিক, সারি.buf, সারি.len))
			die(_("index-pack: Expecting full hex object ID lines only from pack-objects."));

		/*
		 * pack-objects creates the .pack and .idx files, but not the
		 * .promisor file. Create the .promisor file, which is empty.
		 */
		write_special_file("promisor", "", ফাঁকা, দ্বিমিক, ফাঁকা);
	}

	fclose(out);
	যদি (finish_command(&cmd))
		die(_("could not finish pack-objects to repack local links"));
	strbuf_release(&সারি);
	মুক্ত(base_name);
}

পূর্ণসংখ্যা cmd_সূচক_pack(পূর্ণসংখ্যা argc,
		   ধ্রুবক অক্ষর **argv,
		   ধ্রুবক অক্ষর *prefix,
		   কাঠামো repository *repo)
{
	পূর্ণসংখ্যা i, fix_thin_pack = 0, verify = 0, stat_only = 0, rev_সূচক;
	ধ্রুবক অক্ষর *curr_সূচক;
	অক্ষর *curr_rev_সূচক = ফাঁকা;
	ধ্রুবক অক্ষর *সূচক_name = ফাঁকা, *pack_name = ফাঁকা, *rev_সূচক_name = ফাঁকা;
	ধ্রুবক অক্ষর *keep_বার্তা = ফাঁকা;
	ধ্রুবক অক্ষর *promisor_বার্তা = ফাঁকা;
	কাঠামো strbuf সূচক_name_buf = STRBUF_INIT;
	কাঠামো strbuf rev_সূচক_name_buf = STRBUF_INIT;
	কাঠামো pack_idx_entry **idx_objects;
	কাঠামো pack_idx_option opts;
	unsigned অক্ষর pack_hash[GIT_MAX_RAWSZ];
	unsigned foreign_nr = 1;	/* zero is a "good" value, assume bad */
	পূর্ণসংখ্যা report_end_of_প্রবেশ = 0;
	পূর্ণসংখ্যা hash_algo = 0;

	show_usage_if_asked(argc, argv, সূচক_pack_usage);

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
	repo_config(the_repository, git_সূচক_pack_config, &opts);
	যদি (prefix && chdir(prefix))
		die(_("Cannot come back to cwd"));

	যদি (git_env_bool(GIT_TEST_NO_WRITE_REV_সূচক, 0))
		rev_সূচক = 0;
	অন্যথা
		rev_সূচক = !!(opts.flags & (WRITE_REV_VERIFY | WRITE_REV));

	জন্য (i = 1; i < argc; i++) {
		ধ্রুবক অক্ষর *arg = argv[i];

		যদি (*arg == '-') {
			যদি (!strcmp(arg, "--stdin")) {
				from_stdin = 1;
			} অন্যথা যদি (!strcmp(arg, "--fix-thin")) {
				fix_thin_pack = 1;
			} অন্যথা যদি (skip_to_optional_arg(arg, "--strict", &arg)) {
				strict = 1;
				do_fsck_object = 1;
				fsck_set_বার্তা_types(&fsck_options, arg);
			} অন্যথা যদি (!strcmp(arg, "--check-self-contained-and-connected")) {
				strict = 1;
				check_self_contained_and_connected = 1;
			} অন্যথা যদি (skip_to_optional_arg(arg, "--fsck-objects", &arg)) {
				do_fsck_object = 1;
				fsck_set_বার্তা_types(&fsck_options, arg);
			} অন্যথা যদি (!strcmp(arg, "--verify")) {
				verify = 1;
			} অন্যথা যদি (!strcmp(arg, "--verify-stat")) {
				verify = 1;
				show_stat = 1;
			} অন্যথা যদি (!strcmp(arg, "--verify-stat-only")) {
				verify = 1;
				show_stat = 1;
				stat_only = 1;
			} অন্যথা যদি (skip_to_optional_arg(arg, "--keep", &keep_বার্তা)) {
				; /* nothing to do */
			} অন্যথা যদি (skip_to_optional_arg(arg, "--promisor", &promisor_বার্তা)) {
				record_outgoing_links = 1;
			} অন্যথা যদি (starts_with(arg, "--threads=")) {
				অক্ষর *end;
				nr_threads = strtoul(arg+10, &end, 0);
				যদি (!arg[10] || *end || nr_threads < 0)
					usage(সূচক_pack_usage);
				যদি (!HAVE_THREADS && nr_threads != 1) {
					warning(_("no threads support, ignoring %s"), arg);
					nr_threads = 1;
				}
			} অন্যথা যদি (skip_prefix(arg, "--pack_header=", &arg)) {
				যদি (parse_pack_header_option(arg,
							     প্রবেশ_বাফার,
							     &প্রবেশ_len) < 0)
					die(_("bad --pack_header: %s"), arg);
			} অন্যথা যদি (!strcmp(arg, "-v")) {
				verbose = 1;
			} অন্যথা যদি (!strcmp(arg, "--progress-title")) {
				যদি (progress_title || (i+1) >= argc)
					usage(সূচক_pack_usage);
				progress_title = argv[++i];
			} অন্যথা যদি (!strcmp(arg, "--show-resolving-progress")) {
				show_resolving_progress = 1;
			} অন্যথা যদি (!strcmp(arg, "--report-end-of-input")) {
				report_end_of_প্রবেশ = 1;
			} অন্যথা যদি (!strcmp(arg, "-o")) {
				যদি (সূচক_name || (i+1) >= argc)
					usage(সূচক_pack_usage);
				সূচক_name = argv[++i];
			} অন্যথা যদি (starts_with(arg, "--index-version=")) {
				অক্ষর *c;
				opts.version = strtoul(arg + 16, &c, 10);
				যদি (opts.version > 2)
					die(_("bad %s"), arg);
				যদি (*c == ',')
					opts.off32_limit = strtoul(c+1, &c, 0);
				যদি (*c || opts.off32_limit & 0x80000000)
					die(_("bad %s"), arg);
			} অন্যথা যদি (skip_prefix(arg, "--max-input-size=", &arg)) {
				max_প্রবেশ_আকার = strtoumax(arg, ফাঁকা, 10);
			} অন্যথা যদি (skip_prefix(arg, "--object-format=", &arg)) {
				hash_algo = hash_algo_by_name(arg);
				যদি (hash_algo == GIT_HASH_UNKNOWN)
					die(_("unknown hash algorithm '%s'"), arg);
				repo_set_hash_algo(the_repository, hash_algo);
			} অন্যথা যদি (!strcmp(arg, "--rev-index")) {
				rev_সূচক = 1;
			} অন্যথা যদি (!strcmp(arg, "--no-rev-index")) {
				rev_সূচক = 0;
			} অন্যথা
				usage(সূচক_pack_usage);
			চালু;
		}

		যদি (pack_name)
			usage(সূচক_pack_usage);
		pack_name = arg;
	}

	যদি (!pack_name && !from_stdin)
		usage(সূচক_pack_usage);
	যদি (fix_thin_pack && !from_stdin)
		die(_("the option '%s' requires '%s'"), "--fix-thin", "--stdin");
	যদি (promisor_বার্তা && pack_name)
		die(_("--promisor cannot be used with a pack name"));
	যদি (from_stdin && !startup_info->have_repository)
		die(_("--stdin requires a git repository"));
	যদি (from_stdin && hash_algo)
		die(_("options '%s' and '%s' cannot be used together"), "--object-format", "--stdin");
	যদি (!সূচক_name && pack_name)
		সূচক_name = derive_filename(pack_name, "pack", "idx", &সূচক_name_buf);

	/*
	 * Packfiles and indices do not carry enough information to be able to
	 * identify their object hash. So when we are neither in a repository
	 * nor has the user told us which object hash to use we have no other
	 * choice but to guess the object hash.
	 */
	যদি (!the_repository->hash_algo)
		repo_set_hash_algo(the_repository, GIT_HASH_DEFAULT);

	opts.flags &= ~(WRITE_REV | WRITE_REV_VERIFY);
	যদি (rev_সূচক) {
		opts.flags |= verify ? WRITE_REV_VERIFY : WRITE_REV;
		যদি (সূচক_name)
			rev_সূচক_name = derive_filename(সূচক_name,
							 "idx", "rev",
							 &rev_সূচক_name_buf);
	}

	যদি (verify) {
		যদি (!সূচক_name)
			die(_("--verify with no packfile name given"));
		read_idx_option(&opts, সূচক_name);
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
	যদি (report_end_of_প্রবেশ)
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
	জন্য (i = 0; i < nr_objects; i++)
		idx_objects[i] = &objects[i].idx;
	curr_সূচক = write_idx_file(the_repository, সূচক_name, idx_objects,
				    nr_objects, &opts, pack_hash);
	যদি (rev_সূচক)
		curr_rev_সূচক = write_rev_file(the_repository, rev_সূচক_name,
						idx_objects, nr_objects,
						pack_hash, opts.flags);
	মুক্ত(idx_objects);

	যদি (!verify)
		final(pack_name, curr_pack,
		      সূচক_name, curr_সূচক,
		      rev_সূচক_name, curr_rev_সূচক,
		      keep_বার্তা, promisor_বার্তা,
		      pack_hash);
	অন্যথা
		close(প্রবেশ_fd);

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
	strbuf_release(&সূচক_name_buf);
	strbuf_release(&rev_সূচক_name_buf);
	যদি (!pack_name)
		মুক্ত((শূন্য *) curr_pack);
	যদি (!সূচক_name)
		মুক্ত((শূন্য *) curr_সূচক);
	মুক্ত(curr_rev_সূচক);

	repack_local_links();

	/*
	 * Let the caller know this pack is not self contained
	 */
	যদি (check_self_contained_and_connected && foreign_nr)
		ফেরত 1;

	ফেরত 0;
}
