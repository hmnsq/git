#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ "delta.h"
#അന്തര്ഭാവ "environment.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "hex.h"
#അന്തര്ഭാവ "pack.h"
#അന്തര്ഭാവ "csum-file.h"
#അന്തര്ഭാവ "blob.h"
#അന്തര്ഭാവ "commit.h"
#അന്തര്ഭാവ "tag.h"
#അന്തര്ഭാവ "tree.h"
#അന്തര്ഭാവ "progress.h"
#അന്തര്ഭാവ "fsck.h"
#അന്തര്ഭാവ "strbuf.h"
#അന്തര്ഭാവ "thread-utils.h"
#അന്തര്ഭാവ "packfile.h"
#അന്തര്ഭാവ "pack-revindex.h"
#അന്തര്ഭാവ "object-file.h"
#അന്തര്ഭാവ "odb.h"
#അന്തര്ഭാവ "odb/streaming.h"
#അന്തര്ഭാവ "oid-array.h"
#അന്തര്ഭാവ "oidset.h"
#അന്തര്ഭാവ "path.h"
#അന്തര്ഭാവ "replace-object.h"
#അന്തര്ഭാവ "tree-walk.h"
#അന്തര്ഭാവ "promisor-remote.h"
#അന്തര്ഭാവ "run-command.h"
#അന്തര്ഭാവ "setup.h"
#അന്തര്ഭാവ "strvec.h"

സ്ഥിതം സ്ഥിരം അക്ഷരം index_pack_usage[] =
"git index-pack [-v] [-o <index-file>] [--keep | --keep=<msg>] [--[no-]rev-index] [--verify] [--strict[=<msg-id>=<severity>...]] [--fsck-objects[=<msg-id>=<severity>...]] (<pack-file> | --stdin [--fix-thin] [<pack-file>])";

ഘടന object_entry {
	ഘടന pack_idx_entry idx;
	size_t size;
	unsigned അക്ഷരം hdr_size;
	signed അക്ഷരം type;
	signed അക്ഷരം real_type;
};

ഘടന object_stat {
	unsigned delta_depth;
	പൂർണ്ണസംഖ്യ base_object_no;
};

ഘടന base_data {
	/* Initialized by make_base(). */
	ഘടന base_data *base;
	ഘടന object_entry *obj;
	പൂർണ്ണസംഖ്യ ref_first, ref_last;
	പൂർണ്ണസംഖ്യ ofs_first, ofs_last;
	/*
	 * Threads should increment retain_data if they are about to call
	 * patch_delta() using this struct's data as a base, and decrement this
	 * when they are done. While retain_data is nonzero, this struct's data
	 * will not be freed even if the delta base cache limit is exceeded.
	 */
	പൂർണ്ണസംഖ്യ retain_data;
	/*
	 * The number of direct children that have not been fully processed
	 * (entered work_head, entered done_head, left done_head). When this
	 * number reaches zero, this struct base_data can be freed.
	 */
	പൂർണ്ണസംഖ്യ children_remaining;

	/* Not initialized by make_base(). */
	ഘടന list_head list;
	ശൂന്യം *data;
	size_t size;
};

/*
 * Stack of struct base_data that have unprocessed children.
 * threaded_second_pass() uses this as a source of work (the other being the
 * objects array).
 *
 * Guarded by work_mutex.
 */
സ്ഥിതം LIST_HEAD(work_head);

/*
 * Stack of struct base_data that have children, all of whom have been
 * processed or are being processed, and at least one child is being processed.
 * These struct base_data must be kept around until the last child is
 * processed.
 *
 * Guarded by work_mutex.
 */
സ്ഥിതം LIST_HEAD(done_head);

/*
 * All threads share one delta base cache.
 *
 * base_cache_used is guarded by work_mutex, and base_cache_limit is read-only
 * in a thread.
 */
സ്ഥിതം size_t base_cache_used;
സ്ഥിതം size_t base_cache_limit;

ഘടന thread_local_data {
	pthread_t thread;
	പൂർണ്ണസംഖ്യ pack_fd;
};

/* Remember to update object flag allocation in object.h */
#define FLAG_LINK (1u<<20)
#define FLAG_CHECKED (1u<<21)

ഘടന ofs_delta_entry {
	off_t offset;
	പൂർണ്ണസംഖ്യ obj_no;
};

ഘടന ref_delta_entry {
	ഘടന object_id oid;
	പൂർണ്ണസംഖ്യ obj_no;
};

സ്ഥിതം ഘടന object_entry *objects;
സ്ഥിതം ഘടന object_stat *obj_stat;
സ്ഥിതം ഘടന ofs_delta_entry *ofs_deltas;
സ്ഥിതം ഘടന ref_delta_entry *ref_deltas;
സ്ഥിതം ഘടന thread_local_data nothread_data;
സ്ഥിതം പൂർണ്ണസംഖ്യ nr_objects;
സ്ഥിതം പൂർണ്ണസംഖ്യ nr_ofs_deltas;
സ്ഥിതം പൂർണ്ണസംഖ്യ nr_ref_deltas;
സ്ഥിതം പൂർണ്ണസംഖ്യ ref_deltas_alloc;
സ്ഥിതം പൂർണ്ണസംഖ്യ nr_resolved_deltas;
സ്ഥിതം പൂർണ്ണസംഖ്യ nr_threads;

സ്ഥിതം പൂർണ്ണസംഖ്യ from_stdin;
സ്ഥിതം പൂർണ്ണസംഖ്യ strict;
സ്ഥിതം പൂർണ്ണസംഖ്യ do_fsck_object;
സ്ഥിതം ഘടന fsck_options fsck_options;
സ്ഥിതം പൂർണ്ണസംഖ്യ verbose;
സ്ഥിതം സ്ഥിരം അക്ഷരം *progress_title;
സ്ഥിതം പൂർണ്ണസംഖ്യ show_resolving_progress;
സ്ഥിതം പൂർണ്ണസംഖ്യ show_stat;
സ്ഥിതം പൂർണ്ണസംഖ്യ check_self_contained_and_connected;

സ്ഥിതം ഘടന progress *progress;

സ്ഥിതം unsigned അക്ഷരം നിവേശം_buffer[DEFAULT_IO_BUFFER_SIZE];
സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ നിവേശം_offset, നിവേശം_len;
സ്ഥിതം off_t consumed_bytes;
സ്ഥിതം off_t max_നിവേശം_size;
സ്ഥിതം unsigned deepest_delta;
സ്ഥിതം ഘടന git_hash_ctx നിവേശം_ctx;
സ്ഥിതം uint32_t നിവേശം_crc32;
സ്ഥിതം പൂർണ്ണസംഖ്യ നിവേശം_fd, output_fd;
സ്ഥിതം സ്ഥിരം അക്ഷരം *curr_pack;

/*
 * outgoing_links is guarded by read_mutex, and record_outgoing_links is
 * read-only in a thread.
 */
സ്ഥിതം ഘടന oidset outgoing_links = OIDSET_INIT;
സ്ഥിതം പൂർണ്ണസംഖ്യ record_outgoing_links;

സ്ഥിതം ഘടന thread_local_data *thread_data;
സ്ഥിതം പൂർണ്ണസംഖ്യ nr_dispatched;
സ്ഥിതം പൂർണ്ണസംഖ്യ threads_active;

സ്ഥിതം pthread_mutex_t read_mutex;
#define read_lock()		lock_mutex(&read_mutex)
#define read_unlock()		unlock_mutex(&read_mutex)

സ്ഥിതം pthread_mutex_t counter_mutex;
#define counter_lock()		lock_mutex(&counter_mutex)
#define counter_unlock()	unlock_mutex(&counter_mutex)

സ്ഥിതം pthread_mutex_t work_mutex;
#define work_lock()		lock_mutex(&work_mutex)
#define work_unlock()		unlock_mutex(&work_mutex)

സ്ഥിതം pthread_mutex_t deepest_delta_mutex;
#define deepest_delta_lock()	lock_mutex(&deepest_delta_mutex)
#define deepest_delta_unlock()	unlock_mutex(&deepest_delta_mutex)

സ്ഥിതം pthread_താക്കോൽ_t താക്കോൽ;

സ്ഥിതം വരിയിൽ ശൂന്യം lock_mutex(pthread_mutex_t *mutex)
{
	എങ്കിൽ (threads_active)
		pthread_mutex_lock(mutex);
}

സ്ഥിതം വരിയിൽ ശൂന്യം unlock_mutex(pthread_mutex_t *mutex)
{
	എങ്കിൽ (threads_active)
		pthread_mutex_unlock(mutex);
}

/*
 * Mutex and conditional variable can't be statically-initialized on Windows.
 */
സ്ഥിതം ശൂന്യം init_thread(ശൂന്യം)
{
	പൂർണ്ണസംഖ്യ i;
	init_recursive_mutex(&read_mutex);
	pthread_mutex_init(&counter_mutex, ശൂന്യ);
	pthread_mutex_init(&work_mutex, ശൂന്യ);
	എങ്കിൽ (show_stat)
		pthread_mutex_init(&deepest_delta_mutex, ശൂന്യ);
	pthread_താക്കോൽ_create(&താക്കോൽ, ശൂന്യ);
	CALLOC_ARRAY(thread_data, nr_threads);
	വേണ്ടി (i = 0; i < nr_threads; i++) {
		thread_data[i].pack_fd = xopen(curr_pack, O_RDONLY);
	}

	threads_active = 1;
}

സ്ഥിതം ശൂന്യം cleanup_thread(ശൂന്യം)
{
	പൂർണ്ണസംഖ്യ i;
	എങ്കിൽ (!threads_active)
		തിരികെ;
	threads_active = 0;
	pthread_mutex_destroy(&read_mutex);
	pthread_mutex_destroy(&counter_mutex);
	pthread_mutex_destroy(&work_mutex);
	എങ്കിൽ (show_stat)
		pthread_mutex_destroy(&deepest_delta_mutex);
	വേണ്ടി (i = 0; i < nr_threads; i++)
		close(thread_data[i].pack_fd);
	pthread_താക്കോൽ_delete(താക്കോൽ);
	സ്വതന്ത്രം(thread_data);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ mark_link(ഘടന object *obj, എണ്ണൽ object_type type,
		     ശൂന്യം *data UNUSED,
		     ഘടന fsck_options *options UNUSED)
{
	എങ്കിൽ (!obj)
		തിരികെ -1;

	എങ്കിൽ (type != OBJ_ANY && obj->type != type)
		die(_("object type mismatch at %s"), oid_to_hex(&obj->oid));

	obj->flags |= FLAG_LINK;
	തിരികെ 0;
}

/* The content of each linked object must have been checked
   or it must be already present in the object database */
സ്ഥിതം unsigned check_object(ഘടന object *obj)
{
	എങ്കിൽ (!obj)
		തിരികെ 0;

	എങ്കിൽ (!(obj->flags & FLAG_LINK))
		തിരികെ 0;

	എങ്കിൽ (!(obj->flags & FLAG_CHECKED)) {
		size_t size;
		പൂർണ്ണസംഖ്യ type = odb_read_object_info(the_repository->objects,
						&obj->oid, &size);
		എങ്കിൽ (type <= 0)
			die(_("did not receive expected object %s"),
			      oid_to_hex(&obj->oid));
		എങ്കിൽ (type != obj->type)
			die(_("object %s: expected type %s, found %s"),
			    oid_to_hex(&obj->oid),
			    type_name(obj->type), type_name(type));
		obj->flags |= FLAG_CHECKED;
		തിരികെ 1;
	}

	തിരികെ 0;
}

സ്ഥിതം unsigned check_objects(ശൂന്യം)
{
	unsigned i, max, foreign_nr = 0;

	max = get_max_object_index(the_repository);

	എങ്കിൽ (verbose)
		progress = start_delayed_progress(the_repository,
						  _("Checking objects"), max);

	വേണ്ടി (i = 0; i < max; i++) {
		foreign_nr += check_object(get_indexed_object(the_repository, i));
		display_progress(progress, i + 1);
	}

	stop_progress(&progress);
	തിരികെ foreign_nr;
}


/* Discard current buffer used content. */
സ്ഥിതം ശൂന്യം flush(ശൂന്യം)
{
	എങ്കിൽ (നിവേശം_offset) {
		എങ്കിൽ (output_fd >= 0)
			write_or_die(output_fd, നിവേശം_buffer, നിവേശം_offset);
		git_hash_update(&നിവേശം_ctx, നിവേശം_buffer, നിവേശം_offset);
		memmove(നിവേശം_buffer, നിവേശം_buffer + നിവേശം_offset, നിവേശം_len);
		നിവേശം_offset = 0;
	}
}

/*
 * Make sure at least "min" bytes are available in the buffer, and
 * return the pointer to the buffer.
 */
സ്ഥിതം ശൂന്യം *fill(പൂർണ്ണസംഖ്യ min)
{
	എങ്കിൽ (min <= നിവേശം_len)
		തിരികെ നിവേശം_buffer + നിവേശം_offset;
	എങ്കിൽ (min > വലുപ്പം(നിവേശം_buffer))
		die(Q_("cannot fill %d byte",
		       "cannot fill %d bytes",
		       min),
		    min);
	flush();
	ചെയ്യുക {
		ssize_t ret = xread(നിവേശം_fd, നിവേശം_buffer + നിവേശം_len,
				വലുപ്പം(നിവേശം_buffer) - നിവേശം_len);
		എങ്കിൽ (ret <= 0) {
			എങ്കിൽ (!ret)
				die(_("early EOF"));
			die_errno(_("read error on input"));
		}
		നിവേശം_len += ret;
		എങ്കിൽ (from_stdin)
			display_throughput(progress, consumed_bytes + നിവേശം_len);
	} ആകുമ്പോൾ (നിവേശം_len < min);
	തിരികെ നിവേശം_buffer;
}

സ്ഥിതം ശൂന്യം use(പൂർണ്ണസംഖ്യ bytes)
{
	എങ്കിൽ (bytes > നിവേശം_len)
		die(_("used more bytes than were available"));
	നിവേശം_crc32 = crc32(നിവേശം_crc32, നിവേശം_buffer + നിവേശം_offset, bytes);
	നിവേശം_len -= bytes;
	നിവേശം_offset += bytes;

	/* make sure off_t is sufficiently large not to wrap */
	എങ്കിൽ (signed_add_overflows(consumed_bytes, bytes))
		die(_("pack too large for current definition of off_t"));
	consumed_bytes += bytes;
	എങ്കിൽ (max_നിവേശം_size && consumed_bytes > max_നിവേശം_size) {
		ഘടന strbuf size_limit = STRBUF_INIT;
		strbuf_humanise_bytes(&size_limit, max_നിവേശം_size);
		die(_("pack exceeds maximum allowed size (%s)"),
		    size_limit.buf);
	}
}

സ്ഥിതം സ്ഥിരം അക്ഷരം *open_pack_file(സ്ഥിരം അക്ഷരം *pack_name)
{
	എങ്കിൽ (from_stdin) {
		നിവേശം_fd = 0;
		എങ്കിൽ (!pack_name) {
			ഘടന strbuf tmp_file = STRBUF_INIT;
			output_fd = odb_mkstemp(the_repository->objects, &tmp_file,
						"pack/tmp_pack_XXXXXX");
			pack_name = strbuf_detach(&tmp_file, ശൂന്യ);
		} അല്ലെങ്കിൽ {
			output_fd = xopen(pack_name, O_CREAT|O_EXCL|O_RDWR, 0600);
		}
		nothread_data.pack_fd = output_fd;
	} അല്ലെങ്കിൽ {
		നിവേശം_fd = xopen(pack_name, O_RDONLY);
		output_fd = -1;
		nothread_data.pack_fd = നിവേശം_fd;
	}
	git_hash_init(&നിവേശം_ctx, the_hash_algo);
	തിരികെ pack_name;
}

സ്ഥിതം ശൂന്യം parse_pack_header(ശൂന്യം)
{
	unsigned അക്ഷരം *hdr = fill(വലുപ്പം(ഘടന pack_header));

	/* Header consistency check */
	എങ്കിൽ (get_be32(hdr) != PACK_SIGNATURE)
		die(_("pack signature mismatch"));
	hdr += 4;
	എങ്കിൽ (!pack_version_ok_native(get_be32(hdr)))
		die(_("pack version %"PRIu32" unsupported"),
		    get_be32(hdr));
	hdr += 4;

	nr_objects = get_be32(hdr);
	use(വലുപ്പം(ഘടന pack_header));
}

__attribute__((format (printf, 2, 3)))
സ്ഥിതം NORETURN ശൂന്യം bad_object(off_t offset, സ്ഥിരം അക്ഷരം *format, ...)
{
	va_list params;
	അക്ഷരം buf[1024];

	va_start(params, format);
	vsnprintf(buf, വലുപ്പം(buf), format, params);
	va_end(params);
	die(_("pack has bad object at offset %"PRIuMAX": %s"),
	    (uintmax_t)offset, buf);
}

സ്ഥിതം വരിയിൽ ഘടന thread_local_data *get_thread_data(ശൂന്യം)
{
	എങ്കിൽ (HAVE_THREADS) {
		എങ്കിൽ (threads_active)
			തിരികെ pthread_getspecific(താക്കോൽ);
		പ്രതിജ്ഞാ(!threads_active &&
		       "This should only be reached when all threads are gone");
	}
	തിരികെ &nothread_data;
}

സ്ഥിതം ശൂന്യം set_thread_data(ഘടന thread_local_data *data)
{
	എങ്കിൽ (threads_active)
		pthread_setspecific(താക്കോൽ, data);
}

സ്ഥിതം ശൂന്യം free_base_data(ഘടന base_data *c)
{
	എങ്കിൽ (c->data) {
		FREE_AND_NULL(c->data);
		base_cache_used -= c->size;
	}
}

സ്ഥിതം ശൂന്യം prune_base_data(ഘടന base_data *retain)
{
	ഘടന list_head *pos;

	എങ്കിൽ (base_cache_used <= base_cache_limit)
		തിരികെ;

	list_for_each_prev(pos, &done_head) {
		ഘടന base_data *b = list_entry(pos, ഘടന base_data, list);
		എങ്കിൽ (b->retain_data || b == retain)
			തുടരുക;
		എങ്കിൽ (b->data) {
			free_base_data(b);
			എങ്കിൽ (base_cache_used <= base_cache_limit)
				തിരികെ;
		}
	}

	list_for_each_prev(pos, &work_head) {
		ഘടന base_data *b = list_entry(pos, ഘടന base_data, list);
		എങ്കിൽ (b->retain_data || b == retain)
			തുടരുക;
		എങ്കിൽ (b->data) {
			free_base_data(b);
			എങ്കിൽ (base_cache_used <= base_cache_limit)
				തിരികെ;
		}
	}
}

സ്ഥിതം പൂർണ്ണസംഖ്യ is_delta_type(എണ്ണൽ object_type type)
{
	തിരികെ (type == OBJ_REF_DELTA || type == OBJ_OFS_DELTA);
}

സ്ഥിതം ശൂന്യം *unpack_entry_data(off_t offset, size_t size,
			       എണ്ണൽ object_type type, ഘടന object_id *oid)
{
	സ്ഥിതം അക്ഷരം fixed_buf[8192];
	പൂർണ്ണസംഖ്യ status;
	git_zstream stream;
	ശൂന്യം *buf;
	ഘടന git_hash_ctx c;
	അക്ഷരം hdr[32];
	പൂർണ്ണസംഖ്യ hdrlen;

	എങ്കിൽ (!is_delta_type(type)) {
		hdrlen = format_object_header(hdr, വലുപ്പം(hdr), type, size);
		git_hash_init(&c, the_hash_algo);
		git_hash_update(&c, hdr, hdrlen);
	} അല്ലെങ്കിൽ
		oid = ശൂന്യ;
	എങ്കിൽ (type == OBJ_BLOB &&
	    size > repo_settings_get_big_file_threshold(the_repository))
		buf = fixed_buf;
	അല്ലെങ്കിൽ
		buf = xmallocz(size);

	memset(&stream, 0, വലുപ്പം(stream));
	git_inflate_init(&stream);
	stream.next_out = buf;
	stream.avail_out = buf == fixed_buf ? വലുപ്പം(fixed_buf) : size;

	ചെയ്യുക {
		unsigned അക്ഷരം *last_out = stream.next_out;
		stream.next_in = fill(1);
		stream.avail_in = നിവേശം_len;
		status = git_inflate(&stream, 0);
		use(നിവേശം_len - stream.avail_in);
		എങ്കിൽ (oid)
			git_hash_update(&c, last_out, stream.next_out - last_out);
		എങ്കിൽ (buf == fixed_buf) {
			stream.next_out = buf;
			stream.avail_out = വലുപ്പം(fixed_buf);
		}
	} ആകുമ്പോൾ (status == Z_OK);
	എങ്കിൽ (stream.total_out != size || status != Z_STREAM_END)
		bad_object(offset, _("inflate returned %d"), status);
	git_inflate_end(&stream);
	എങ്കിൽ (oid)
		git_hash_final_oid(oid, &c);
	തിരികെ buf == fixed_buf ? ശൂന്യ : buf;
}

സ്ഥിതം ശൂന്യം *unpack_raw_entry(ഘടന object_entry *obj,
			      off_t *ofs_offset,
			      ഘടന object_id *ref_oid,
			      ഘടന object_id *oid)
{
	unsigned അക്ഷരം *p;
	size_t size, c;
	off_t base_offset;
	unsigned shift;
	ശൂന്യം *data;

	obj->idx.offset = consumed_bytes;
	നിവേശം_crc32 = crc32(0, ശൂന്യ, 0);

	p = fill(1);
	c = *p;
	use(1);
	obj->type = (c >> 4) & 7;
	size = (c & 15);
	shift = 4;
	ആകുമ്പോൾ (c & 0x80) {
		എങ്കിൽ ((bitsizeof(size_t) - 7) < shift)
			die(_("object size too large for this platform"));
		p = fill(1);
		c = *p;
		use(1);
		size += (c & 0x7f) << shift;
		shift += 7;
	}
	obj->size = size;

	തിരഞ്ഞെടുപ്പ് (obj->type) {
	അവസ്ഥ OBJ_REF_DELTA:
		oidread(ref_oid, fill(the_hash_algo->rawsz),
			the_repository->hash_algo);
		use(the_hash_algo->rawsz);
		നിർത്തുക;
	അവസ്ഥ OBJ_OFS_DELTA:
		p = fill(1);
		c = *p;
		use(1);
		base_offset = c & 127;
		ആകുമ്പോൾ (c & 128) {
			base_offset += 1;
			എങ്കിൽ (!base_offset || MSB(base_offset, 7))
				bad_object(obj->idx.offset, _("offset value overflow for delta base object"));
			p = fill(1);
			c = *p;
			use(1);
			base_offset = (base_offset << 7) + (c & 127);
		}
		*ofs_offset = obj->idx.offset - base_offset;
		എങ്കിൽ (*ofs_offset <= 0 || *ofs_offset >= obj->idx.offset)
			bad_object(obj->idx.offset, _("delta base offset is out of bound"));
		നിർത്തുക;
	അവസ്ഥ OBJ_COMMIT:
	അവസ്ഥ OBJ_വൃക്ഷം:
	അവസ്ഥ OBJ_BLOB:
	അവസ്ഥ OBJ_TAG:
		നിർത്തുക;
	സ്ഥിരസ്ഥിതി:
		bad_object(obj->idx.offset, _("unknown object type %d"), obj->type);
	}
	obj->hdr_size = consumed_bytes - obj->idx.offset;

	data = unpack_entry_data(obj->idx.offset, obj->size, obj->type, oid);
	obj->idx.crc32 = നിവേശം_crc32;
	തിരികെ data;
}

സ്ഥിതം ശൂന്യം *unpack_data(ഘടന object_entry *obj,
			 പൂർണ്ണസംഖ്യ (*consume)(സ്ഥിരം unsigned അക്ഷരം *, unsigned long, ശൂന്യം *),
			 ശൂന്യം *cb_data)
{
	off_t from = obj[0].idx.offset + obj[0].hdr_size;
	off_t len = obj[1].idx.offset - from;
	unsigned അക്ഷരം *data, *inbuf;
	git_zstream stream;
	പൂർണ്ണസംഖ്യ status;

	data = xmallocz(consume ? 64*1024 : obj->size);
	inbuf = xmalloc((len < 64*1024) ? (പൂർണ്ണസംഖ്യ)len : 64*1024);

	memset(&stream, 0, വലുപ്പം(stream));
	git_inflate_init(&stream);
	stream.next_out = data;
	stream.avail_out = consume ? 64*1024 : obj->size;

	ചെയ്യുക {
		ssize_t n = (len < 64*1024) ? (ssize_t)len : 64*1024;
		n = xpread(get_thread_data()->pack_fd, inbuf, n, from);
		എങ്കിൽ (n < 0)
			die_errno(_("cannot pread pack file"));
		എങ്കിൽ (!n)
			die(Q_("premature end of pack file, %"PRIuMAX" byte missing",
			       "premature end of pack file, %"PRIuMAX" bytes missing",
			       len),
			    (uintmax_t)len);
		from += n;
		len -= n;
		stream.next_in = inbuf;
		stream.avail_in = n;
		എങ്കിൽ (!consume)
			status = git_inflate(&stream, 0);
		അല്ലെങ്കിൽ {
			ചെയ്യുക {
				status = git_inflate(&stream, 0);
				എങ്കിൽ (consume(data, stream.next_out - data, cb_data)) {
					സ്വതന്ത്രം(inbuf);
					സ്വതന്ത്രം(data);
					തിരികെ ശൂന്യ;
				}
				stream.next_out = data;
				stream.avail_out = 64*1024;
			} ആകുമ്പോൾ (status == Z_OK && stream.avail_in);
		}
	} ആകുമ്പോൾ (len && status == Z_OK && !stream.avail_in);

	/* This has been inflated OK when first encountered, so... */
	എങ്കിൽ (status != Z_STREAM_END || stream.total_out != obj->size)
		die(_("serious inflate inconsistency"));

	git_inflate_end(&stream);
	സ്വതന്ത്രം(inbuf);
	എങ്കിൽ (consume) {
		FREE_AND_NULL(data);
	}
	തിരികെ data;
}

സ്ഥിതം ശൂന്യം *get_data_from_pack(ഘടന object_entry *obj)
{
	തിരികെ unpack_data(obj, ശൂന്യ, ശൂന്യ);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ compare_ofs_delta_bases(off_t offset1, off_t offset2,
				   എണ്ണൽ object_type type1,
				   എണ്ണൽ object_type type2)
{
	പൂർണ്ണസംഖ്യ cmp = type1 - type2;
	എങ്കിൽ (cmp)
		തിരികെ cmp;
	തിരികെ offset1 < offset2 ? -1 :
	       offset1 > offset2 ?  1 :
	       0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ find_ofs_delta(സ്ഥിരം off_t offset)
{
	പൂർണ്ണസംഖ്യ first = 0, last = nr_ofs_deltas;

	ആകുമ്പോൾ (first < last) {
		പൂർണ്ണസംഖ്യ next = first + (last - first) / 2;
		ഘടന ofs_delta_entry *delta = &ofs_deltas[next];
		പൂർണ്ണസംഖ്യ cmp;

		cmp = compare_ofs_delta_bases(offset, delta->offset,
					      OBJ_OFS_DELTA,
					      objects[delta->obj_no].type);
		എങ്കിൽ (!cmp)
			തിരികെ next;
		എങ്കിൽ (cmp < 0) {
			last = next;
			തുടരുക;
		}
		first = next+1;
	}
	തിരികെ -first-1;
}

സ്ഥിതം ശൂന്യം find_ofs_delta_children(off_t offset,
				    പൂർണ്ണസംഖ്യ *first_index, പൂർണ്ണസംഖ്യ *last_index)
{
	പൂർണ്ണസംഖ്യ first = find_ofs_delta(offset);
	പൂർണ്ണസംഖ്യ last = first;
	പൂർണ്ണസംഖ്യ end = nr_ofs_deltas - 1;

	എങ്കിൽ (first < 0) {
		*first_index = 0;
		*last_index = -1;
		തിരികെ;
	}
	ആകുമ്പോൾ (first > 0 && ofs_deltas[first - 1].offset == offset)
		--first;
	ആകുമ്പോൾ (last < end && ofs_deltas[last + 1].offset == offset)
		++last;
	*first_index = first;
	*last_index = last;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ compare_ref_delta_bases(സ്ഥിരം ഘടന object_id *oid1,
				   സ്ഥിരം ഘടന object_id *oid2,
				   എണ്ണൽ object_type type1,
				   എണ്ണൽ object_type type2)
{
	പൂർണ്ണസംഖ്യ cmp = type1 - type2;
	എങ്കിൽ (cmp)
		തിരികെ cmp;
	തിരികെ oidcmp(oid1, oid2);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ find_ref_delta(സ്ഥിരം ഘടന object_id *oid)
{
	പൂർണ്ണസംഖ്യ first = 0, last = nr_ref_deltas;

	ആകുമ്പോൾ (first < last) {
		പൂർണ്ണസംഖ്യ next = first + (last - first) / 2;
		ഘടന ref_delta_entry *delta = &ref_deltas[next];
		പൂർണ്ണസംഖ്യ cmp;

		cmp = compare_ref_delta_bases(oid, &delta->oid,
					      OBJ_REF_DELTA,
					      objects[delta->obj_no].type);
		എങ്കിൽ (!cmp)
			തിരികെ next;
		എങ്കിൽ (cmp < 0) {
			last = next;
			തുടരുക;
		}
		first = next+1;
	}
	തിരികെ -first-1;
}

സ്ഥിതം ശൂന്യം find_ref_delta_children(സ്ഥിരം ഘടന object_id *oid,
				    പൂർണ്ണസംഖ്യ *first_index, പൂർണ്ണസംഖ്യ *last_index)
{
	പൂർണ്ണസംഖ്യ first = find_ref_delta(oid);
	പൂർണ്ണസംഖ്യ last = first;
	പൂർണ്ണസംഖ്യ end = nr_ref_deltas - 1;

	എങ്കിൽ (first < 0) {
		*first_index = 0;
		*last_index = -1;
		തിരികെ;
	}
	ആകുമ്പോൾ (first > 0 && oideq(&ref_deltas[first - 1].oid, oid))
		--first;
	ആകുമ്പോൾ (last < end && oideq(&ref_deltas[last + 1].oid, oid))
		++last;
	*first_index = first;
	*last_index = last;
}

ഘടന compare_data {
	ഘടന object_entry *entry;
	ഘടന odb_stream *st;
	unsigned അക്ഷരം *buf;
	unsigned long buf_size;
};

സ്ഥിതം പൂർണ്ണസംഖ്യ compare_objects(സ്ഥിരം unsigned അക്ഷരം *buf, unsigned long size,
			   ശൂന്യം *cb_data)
{
	ഘടന compare_data *data = cb_data;

	എങ്കിൽ (data->buf_size < size) {
		സ്വതന്ത്രം(data->buf);
		data->buf = xmalloc(size);
		data->buf_size = size;
	}

	ആകുമ്പോൾ (size) {
		ssize_t len = odb_stream_read(data->st, data->buf, size);
		എങ്കിൽ (len == 0)
			die(_("SHA1 COLLISION FOUND WITH %s !"),
			    oid_to_hex(&data->entry->idx.oid));
		എങ്കിൽ (len < 0)
			die(_("unable to read %s"),
			    oid_to_hex(&data->entry->idx.oid));
		എങ്കിൽ (memcmp(buf, data->buf, len))
			die(_("SHA1 COLLISION FOUND WITH %s !"),
			    oid_to_hex(&data->entry->idx.oid));
		size -= len;
		buf += len;
	}
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ check_collison(ഘടന object_entry *entry)
{
	ഘടന compare_data data;

	എങ്കിൽ (entry->size <= repo_settings_get_big_file_threshold(the_repository) ||
	    entry->type != OBJ_BLOB)
		തിരികെ -1;

	memset(&data, 0, വലുപ്പം(data));
	data.entry = entry;
	data.st = odb_stream_from_object(the_repository->objects, &entry->idx.oid, ശൂന്യ);
	എങ്കിൽ (!data.st)
		തിരികെ -1;
	എങ്കിൽ (data.st->size != entry->size || data.st->type != entry->type)
		die(_("SHA1 COLLISION FOUND WITH %s !"),
		    oid_to_hex(&entry->idx.oid));
	unpack_data(entry, compare_objects, &data);
	odb_stream_close(data.st);
	സ്വതന്ത്രം(data.buf);
	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം record_outgoing_link(സ്ഥിരം ഘടന object_id *oid)
{
	oidset_തിരുകുക(&outgoing_links, oid);
}

സ്ഥിതം ശൂന്യം maybe_record_name_entry(സ്ഥിരം ഘടന name_entry *entry)
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
	എങ്കിൽ (S_ISDIR(entry->mode))
		record_outgoing_link(&entry->oid);
}

സ്ഥിതം ശൂന്യം do_record_outgoing_links(ഘടന object *obj)
{
	എങ്കിൽ (obj->type == OBJ_വൃക്ഷം) {
		ഘടന വൃക്ഷം *വൃക്ഷം = (ഘടന വൃക്ഷം *)obj;
		ഘടന വൃക്ഷം_desc desc;
		ഘടന name_entry entry;
		എങ്കിൽ (init_വൃക്ഷം_desc_gently(&desc, &വൃക്ഷം->object.oid,
					  വൃക്ഷം->buffer, വൃക്ഷം->size, 0))
			/*
			 * Error messages are given when packs are
			 * verified, so do not print any here.
			 */
			തിരികെ;
		ആകുമ്പോൾ (വൃക്ഷം_entry_gently(&desc, &entry))
			maybe_record_name_entry(&entry);
	} അല്ലെങ്കിൽ എങ്കിൽ (obj->type == OBJ_COMMIT) {
		ഘടന commit *commit = (ഘടന commit *) obj;
		ഘടന commit_list *parents = commit->parents;

		record_outgoing_link(get_commit_വൃക്ഷം_oid(commit));
		വേണ്ടി (; parents; parents = parents->next)
			record_outgoing_link(&parents->item->object.oid);
	} അല്ലെങ്കിൽ എങ്കിൽ (obj->type == OBJ_TAG) {
		ഘടന tag *tag = (ഘടന tag *) obj;
		record_outgoing_link(get_tagged_oid(tag));
	}
}

സ്ഥിതം ശൂന്യം sha1_object(സ്ഥിരം ശൂന്യം *data, ഘടന object_entry *obj_entry,
			unsigned long size, എണ്ണൽ object_type type,
			സ്ഥിരം ഘടന object_id *oid)
{
	ശൂന്യം *new_data = ശൂന്യ;
	പൂർണ്ണസംഖ്യ collision_test_needed = 0;

	പ്രതിജ്ഞാ(data || obj_entry);

	എങ്കിൽ (startup_info->have_repository) {
		read_lock();
		collision_test_needed = odb_has_object(the_repository->objects, oid,
						       ODB_HAS_OBJECT_FETCH_PROMISOR);
		read_unlock();
	}

	എങ്കിൽ (collision_test_needed && !data) {
		read_lock();
		എങ്കിൽ (!check_collison(obj_entry))
			collision_test_needed = 0;
		read_unlock();
	}
	എങ്കിൽ (collision_test_needed) {
		ശൂന്യം *has_data;
		എണ്ണൽ object_type has_type;
		size_t has_size;
		read_lock();
		has_type = odb_read_object_info(the_repository->objects, oid, &has_size);
		എങ്കിൽ (has_type < 0)
			die(_("cannot read existing object info %s"), oid_to_hex(oid));
		എങ്കിൽ (has_type != type || has_size != size)
			die(_("SHA1 COLLISION FOUND WITH %s !"), oid_to_hex(oid));
		has_data = odb_read_object(the_repository->objects, oid,
					   &has_type, &has_size);
		read_unlock();
		എങ്കിൽ (!data)
			data = new_data = get_data_from_pack(obj_entry);
		എങ്കിൽ (!has_data)
			die(_("cannot read existing object %s"), oid_to_hex(oid));
		എങ്കിൽ (size != has_size || type != has_type ||
		    memcmp(data, has_data, size) != 0)
			die(_("SHA1 COLLISION FOUND WITH %s !"), oid_to_hex(oid));
		സ്വതന്ത്രം(has_data);
	}

	എങ്കിൽ (strict || do_fsck_object || record_outgoing_links) {
		read_lock();
		എങ്കിൽ (type == OBJ_BLOB) {
			ഘടന blob *blob = lookup_blob(the_repository, oid);
			എങ്കിൽ (blob)
				blob->object.flags |= FLAG_CHECKED;
			അല്ലെങ്കിൽ
				die(_("invalid blob object %s"), oid_to_hex(oid));
			എങ്കിൽ (do_fsck_object &&
			    fsck_object(&blob->object, (ശൂന്യം *)data, size, &fsck_options))
				die(_("fsck error in packed object"));
		} അല്ലെങ്കിൽ {
			ഘടന object *obj;
			പൂർണ്ണസംഖ്യ eaten;
			ശൂന്യം *buf = (ശൂന്യം *) data;

			പ്രതിജ്ഞാ(data && "data can only be NULL for large _blobs_");

			/*
			 * we do not need to free the memory here, as the
			 * buf is deleted by the caller.
			 */
			obj = parse_object_buffer(the_repository, oid, type,
						  size, buf,
						  &eaten);
			എങ്കിൽ (!obj)
				die(_("invalid %s"), type_name(type));
			എങ്കിൽ (do_fsck_object &&
			    fsck_object(obj, buf, size, &fsck_options))
				die(_("fsck error in packed object"));
			എങ്കിൽ (strict && fsck_walk(obj, ശൂന്യ, &fsck_options))
				die(_("Not all child objects of %s are reachable"), oid_to_hex(&obj->oid));
			എങ്കിൽ (record_outgoing_links)
				do_record_outgoing_links(obj);

			എങ്കിൽ (obj->type == OBJ_വൃക്ഷം) {
				ഘടന വൃക്ഷം *item = (ഘടന വൃക്ഷം *) obj;
				item->buffer = ശൂന്യ;
				obj->parsed = 0;
			}
			എങ്കിൽ (obj->type == OBJ_COMMIT) {
				ഘടന commit *commit = (ഘടന commit *) obj;
				എങ്കിൽ (detach_commit_buffer(commit, ശൂന്യ) != data)
					BUG("parse_object_buffer transmogrified our buffer");
			}
			obj->flags |= FLAG_CHECKED;
		}
		read_unlock();
	}

	സ്വതന്ത്രം(new_data);
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
സ്ഥിതം ശൂന്യം *get_base_data(ഘടന base_data *c)
{
	എങ്കിൽ (!c->data) {
		ഘടന object_entry *obj = c->obj;
		ഘടന base_data **delta = ശൂന്യ;
		പൂർണ്ണസംഖ്യ delta_nr = 0, delta_alloc = 0;

		ആകുമ്പോൾ (is_delta_type(c->obj->type) && !c->data) {
			ALLOC_GROW(delta, delta_nr + 1, delta_alloc);
			delta[delta_nr++] = c;
			c = c->base;
		}
		എങ്കിൽ (!delta_nr) {
			c->data = get_data_from_pack(obj);
			c->size = obj->size;
			base_cache_used += c->size;
			prune_base_data(c);
		}
		വേണ്ടി (; delta_nr > 0; delta_nr--) {
			ശൂന്യം *base, *raw;
			c = delta[delta_nr - 1];
			obj = c->obj;
			base = get_base_data(c->base);
			raw = get_data_from_pack(obj);
			c->data = patch_delta(
				base, c->base->size,
				raw, obj->size,
				&c->size);
			സ്വതന്ത്രം(raw);
			എങ്കിൽ (!c->data)
				bad_object(obj->idx.offset, _("failed to apply delta"));
			base_cache_used += c->size;
			prune_base_data(c);
		}
		സ്വതന്ത്രം(delta);
	}
	തിരികെ c->data;
}

സ്ഥിതം ഘടന base_data *make_base(ഘടന object_entry *obj,
				   ഘടന base_data *parent)
{
	ഘടന base_data *base = xcalloc(1, വലുപ്പം(ഘടന base_data));
	base->base = parent;
	base->obj = obj;
	find_ref_delta_children(&obj->idx.oid,
				&base->ref_first, &base->ref_last);
	find_ofs_delta_children(obj->idx.offset,
				&base->ofs_first, &base->ofs_last);
	base->children_remaining = base->ref_last - base->ref_first +
		base->ofs_last - base->ofs_first + 2;
	തിരികെ base;
}

സ്ഥിതം ഘടന base_data *resolve_delta(ഘടന object_entry *delta_obj,
				       ഘടന base_data *base)
{
	ശൂന്യം *delta_data, *result_data;
	ഘടന base_data *result;
	size_t result_size;

	എങ്കിൽ (show_stat) {
		പൂർണ്ണസംഖ്യ i = delta_obj - objects;
		പൂർണ്ണസംഖ്യ j = base->obj - objects;
		obj_stat[i].delta_depth = obj_stat[j].delta_depth + 1;
		deepest_delta_lock();
		എങ്കിൽ (deepest_delta < obj_stat[i].delta_depth)
			deepest_delta = obj_stat[i].delta_depth;
		deepest_delta_unlock();
		obj_stat[i].base_object_no = j;
	}
	delta_data = get_data_from_pack(delta_obj);
	പ്രതിജ്ഞാ(base->data);
	result_data = patch_delta(base->data, base->size,
				  delta_data, delta_obj->size, &result_size);
	സ്വതന്ത്രം(delta_data);
	എങ്കിൽ (!result_data)
		bad_object(delta_obj->idx.offset, _("failed to apply delta"));
	hash_object_file(the_hash_algo, result_data, result_size,
			 delta_obj->real_type, &delta_obj->idx.oid);
	sha1_object(result_data, ശൂന്യ, result_size, delta_obj->real_type,
		    &delta_obj->idx.oid);

	result = make_base(delta_obj, base);
	result->data = result_data;
	result->size = result_size;

	counter_lock();
	nr_resolved_deltas++;
	counter_unlock();

	തിരികെ result;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ compare_ofs_delta_entry(സ്ഥിരം ശൂന്യം *a, സ്ഥിരം ശൂന്യം *b)
{
	സ്ഥിരം ഘടന ofs_delta_entry *delta_a = a;
	സ്ഥിരം ഘടന ofs_delta_entry *delta_b = b;

	തിരികെ delta_a->offset < delta_b->offset ? -1 :
	       delta_a->offset > delta_b->offset ?  1 :
	       0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ compare_ref_delta_entry(സ്ഥിരം ശൂന്യം *a, സ്ഥിരം ശൂന്യം *b)
{
	സ്ഥിരം ഘടന ref_delta_entry *delta_a = a;
	സ്ഥിരം ഘടന ref_delta_entry *delta_b = b;

	തിരികെ oidcmp(&delta_a->oid, &delta_b->oid);
}

സ്ഥിതം ശൂന്യം *threaded_second_pass(ശൂന്യം *data)
{
	എങ്കിൽ (data)
		set_thread_data(data);
	വേണ്ടി (;;) {
		ഘടന base_data *parent = ശൂന്യ;
		ഘടന object_entry *child_obj = ശൂന്യ;
		ഘടന base_data *child = ശൂന്യ;

		counter_lock();
		display_progress(progress, nr_resolved_deltas);
		counter_unlock();

		work_lock();
		എങ്കിൽ (list_empty(&work_head)) {
			/*
			 * Take an object from the object array.
			 */
			ആകുമ്പോൾ (nr_dispatched < nr_objects &&
			       is_delta_type(objects[nr_dispatched].type))
				nr_dispatched++;
			എങ്കിൽ (nr_dispatched >= nr_objects) {
				work_unlock();
				നിർത്തുക;
			}
			child_obj = &objects[nr_dispatched++];
		} അല്ലെങ്കിൽ {
			/*
			 * Peek at the top of the stack, and take a child from
			 * it.
			 */
			parent = list_first_entry(&work_head, ഘടന base_data,
						  list);

			ആകുമ്പോൾ (parent->ref_first <= parent->ref_last) {
				പൂർണ്ണസംഖ്യ offset = ref_deltas[parent->ref_first++].obj_no;
				child_obj = objects + offset;
				എങ്കിൽ (child_obj->real_type != OBJ_REF_DELTA) {
					child_obj = ശൂന്യ;
					തുടരുക;
				}
				child_obj->real_type = parent->obj->real_type;
				നിർത്തുക;
			}

			എങ്കിൽ (!child_obj && parent->ofs_first <= parent->ofs_last) {
				child_obj = objects +
					ofs_deltas[parent->ofs_first++].obj_no;
				പ്രതിജ്ഞാ(child_obj->real_type == OBJ_OFS_DELTA);
				child_obj->real_type = parent->obj->real_type;
			}

			എങ്കിൽ (parent->ref_first > parent->ref_last &&
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

		എങ്കിൽ (child_obj) {
			എങ്കിൽ (parent) {
				child = resolve_delta(child_obj, parent);
				എങ്കിൽ (!child->children_remaining)
					FREE_AND_NULL(child->data);
			} അല്ലെങ്കിൽ{
				child = make_base(child_obj, ശൂന്യ);
				എങ്കിൽ (child->children_remaining) {
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
		എങ്കിൽ (parent)
			parent->retain_data--;

		എങ്കിൽ (child && child->data) {
			/*
			 * This child has its own children, so add it to
			 * work_head.
			 */
			list_add(&child->list, &work_head);
			base_cache_used += child->size;
			prune_base_data(ശൂന്യ);
		} അല്ലെങ്കിൽ എങ്കിൽ (child) {
			/*
			 * This child does not have its own children. It may be
			 * the last descendant of its ancestors; free those
			 * that we can.
			 */
			ഘടന base_data *p = parent;

			ആകുമ്പോൾ (p) {
				ഘടന base_data *next_p;

				p->children_remaining--;
				എങ്കിൽ (p->children_remaining)
					നിർത്തുക;

				next_p = p->base;
				free_base_data(p);
				list_del(&p->list);
				സ്വതന്ത്രം(p);

				p = next_p;
			}
			FREE_AND_NULL(child);
		}
		work_unlock();
	}
	തിരികെ ശൂന്യ;
}

/*
 * First pass:
 * - find locations of all objects;
 * - calculate SHA1 of all non-delta objects;
 * - remember base (SHA1 or offset) for all deltas.
 */
സ്ഥിതം ശൂന്യം parse_pack_objects(unsigned അക്ഷരം *hash)
{
	പൂർണ്ണസംഖ്യ i, nr_delays = 0;
	ഘടന ofs_delta_entry *ofs_delta = ofs_deltas;
	ഘടന object_id ref_delta_oid;
	ഘടന stat st;
	ഘടന git_hash_ctx tmp_ctx;

	എങ്കിൽ (verbose)
		progress = start_progress(
				the_repository,
				progress_title ? progress_title :
				from_stdin ? _("Receiving objects") : _("Indexing objects"),
				nr_objects);
	വേണ്ടി (i = 0; i < nr_objects; i++) {
		ഘടന object_entry *obj = &objects[i];
		ശൂന്യം *data = unpack_raw_entry(obj, &ofs_delta->offset,
					      &ref_delta_oid,
					      &obj->idx.oid);
		obj->real_type = obj->type;
		എങ്കിൽ (obj->type == OBJ_OFS_DELTA) {
			nr_ofs_deltas++;
			ofs_delta->obj_no = i;
			ofs_delta++;
		} അല്ലെങ്കിൽ എങ്കിൽ (obj->type == OBJ_REF_DELTA) {
			ALLOC_GROW(ref_deltas, nr_ref_deltas + 1, ref_deltas_alloc);
			oidcpy(&ref_deltas[nr_ref_deltas].oid, &ref_delta_oid);
			ref_deltas[nr_ref_deltas].obj_no = i;
			nr_ref_deltas++;
		} അല്ലെങ്കിൽ എങ്കിൽ (!data) {
			/* large blobs, check later */
			obj->real_type = OBJ_BAD;
			nr_delays++;
		} അല്ലെങ്കിൽ
			sha1_object(data, ശൂന്യ, obj->size, obj->type,
				    &obj->idx.oid);
		സ്വതന്ത്രം(data);
		display_progress(progress, i+1);
	}
	objects[i].idx.offset = consumed_bytes;
	stop_progress(&progress);

	/* Check pack integrity */
	flush();
	git_hash_init(&tmp_ctx, the_hash_algo);
	git_hash_clone(&tmp_ctx, &നിവേശം_ctx);
	git_hash_final(hash, &tmp_ctx);
	എങ്കിൽ (!hasheq(fill(the_hash_algo->rawsz), hash, the_repository->hash_algo))
		die(_("pack is corrupted (SHA1 mismatch)"));
	use(the_hash_algo->rawsz);

	/* If input_fd is a file, we should have reached its end now. */
	എങ്കിൽ (fstat(നിവേശം_fd, &st))
		die_errno(_("cannot fstat packfile"));
	എങ്കിൽ (S_ISREG(st.st_mode) &&
			lseek(നിവേശം_fd, 0, SEEK_CUR) - നിവേശം_len != st.st_size)
		die(_("pack has junk at the end"));

	വേണ്ടി (i = 0; i < nr_objects; i++) {
		ഘടന object_entry *obj = &objects[i];
		എങ്കിൽ (obj->real_type != OBJ_BAD)
			തുടരുക;
		obj->real_type = obj->type;
		sha1_object(ശൂന്യ, obj, obj->size, obj->type,
			    &obj->idx.oid);
		nr_delays--;
	}
	എങ്കിൽ (nr_delays)
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
സ്ഥിതം ശൂന്യം resolve_deltas(ഘടന pack_idx_option *opts)
{
	പൂർണ്ണസംഖ്യ i;

	എങ്കിൽ (!nr_ofs_deltas && !nr_ref_deltas)
		തിരികെ;

	/* Sort deltas by base SHA1/offset for fast searching */
	QSORT(ofs_deltas, nr_ofs_deltas, compare_ofs_delta_entry);
	QSORT(ref_deltas, nr_ref_deltas, compare_ref_delta_entry);

	എങ്കിൽ (verbose || show_resolving_progress)
		progress = start_progress(the_repository,
					  _("Resolving deltas"),
					  nr_ref_deltas + nr_ofs_deltas);

	nr_dispatched = 0;
	base_cache_limit = opts->delta_base_cache_limit * nr_threads;
	എങ്കിൽ (nr_threads > 1 || getenv("GIT_FORCE_THREADS")) {
		init_thread();
		വേണ്ടി (i = 0; i < nr_threads; i++) {
			പൂർണ്ണസംഖ്യ ret = pthread_create(&thread_data[i].thread, ശൂന്യ,
						 threaded_second_pass, thread_data + i);
			എങ്കിൽ (ret)
				die(_("unable to create thread: %s"),
				    strerror(ret));
		}
		വേണ്ടി (i = 0; i < nr_threads; i++)
			pthread_join(thread_data[i].thread, ശൂന്യ);
		cleanup_thread();
		തിരികെ;
	}
	threaded_second_pass(&nothread_data);
}

/*
 * Third pass:
 * - append objects to convert thin pack to full pack if required
 * - write the final pack hash
 */
സ്ഥിതം ശൂന്യം fix_unresolved_deltas(ഘടന hashfile *f);
സ്ഥിതം ശൂന്യം conclude_pack(പൂർണ്ണസംഖ്യ fix_thin_pack, സ്ഥിരം അക്ഷരം *curr_pack, unsigned അക്ഷരം *pack_hash)
{
	എങ്കിൽ (nr_ref_deltas + nr_ofs_deltas == nr_resolved_deltas) {
		stop_progress(&progress);
		/* Flush remaining pack final hash. */
		flush();
		തിരികെ;
	}

	എങ്കിൽ (fix_thin_pack) {
		ഘടന hashfile *f;
		unsigned അക്ഷരം read_hash[GIT_MAX_RAWSZ], tail_hash[GIT_MAX_RAWSZ];
		ഘടന strbuf സന്ദേശം = STRBUF_INIT;
		പൂർണ്ണസംഖ്യ nr_unresolved = nr_ofs_deltas + nr_ref_deltas - nr_resolved_deltas;
		പൂർണ്ണസംഖ്യ nr_objects_initial = nr_objects;
		എങ്കിൽ (nr_unresolved <= 0)
			die(_("confusion beyond insanity"));
		REALLOC_ARRAY(objects, nr_objects + nr_unresolved + 1);
		memset(objects + nr_objects + 1, 0,
		       nr_unresolved * വലുപ്പം(*objects));
		f = hashfd(the_repository->hash_algo, output_fd, curr_pack);
		fix_unresolved_deltas(f);
		strbuf_addf(&സന്ദേശം, Q_("completed with %d local object",
				     "completed with %d local objects",
				     nr_objects - nr_objects_initial),
			    nr_objects - nr_objects_initial);
		stop_progress_സന്ദേശം(&progress, സന്ദേശം.buf);
		strbuf_release(&സന്ദേശം);
		finalize_hashfile(f, tail_hash, FSYNC_COMPONENT_PACK, 0);
		hashcpy(read_hash, pack_hash, the_repository->hash_algo);
		fixup_pack_header_footer(the_hash_algo, output_fd, pack_hash,
					 curr_pack, nr_objects,
					 read_hash, consumed_bytes-the_hash_algo->rawsz);
		എങ്കിൽ (!hasheq(read_hash, tail_hash, the_repository->hash_algo))
			die(_("Unexpected tail checksum for %s "
			      "(disk corruption?)"), curr_pack);
	}
	എങ്കിൽ (nr_ofs_deltas + nr_ref_deltas != nr_resolved_deltas)
		die(Q_("pack has %d unresolved delta",
		       "pack has %d unresolved deltas",
		       nr_ofs_deltas + nr_ref_deltas - nr_resolved_deltas),
		    nr_ofs_deltas + nr_ref_deltas - nr_resolved_deltas);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ write_compressed(ഘടന hashfile *f, ശൂന്യം *in, unsigned പൂർണ്ണസംഖ്യ size)
{
	git_zstream stream;
	പൂർണ്ണസംഖ്യ status;
	unsigned അക്ഷരം outbuf[4096];
	ഘടന repo_config_മൂല്യങ്ങൾ *cfg = repo_config_മൂല്യങ്ങൾ(the_repository);

	git_deflate_init(&stream, cfg->zlib_compression_level);
	stream.next_in = in;
	stream.avail_in = size;

	ചെയ്യുക {
		stream.next_out = outbuf;
		stream.avail_out = വലുപ്പം(outbuf);
		status = git_deflate(&stream, Z_FINISH);
		hashwrite(f, outbuf, വലുപ്പം(outbuf) - stream.avail_out);
	} ആകുമ്പോൾ (status == Z_OK);

	എങ്കിൽ (status != Z_STREAM_END)
		die(_("unable to deflate appended object (%d)"), status);
	size = stream.total_out;
	git_deflate_end(&stream);
	തിരികെ size;
}

സ്ഥിതം ഘടന object_entry *append_obj_to_pack(ഘടന hashfile *f,
			       സ്ഥിരം unsigned അക്ഷരം *sha1, ശൂന്യം *buf,
			       unsigned long size, എണ്ണൽ object_type type)
{
	ഘടന object_entry *obj = &objects[nr_objects++];
	unsigned അക്ഷരം header[10];
	unsigned long s = size;
	പൂർണ്ണസംഖ്യ n = 0;
	unsigned അക്ഷരം c = (type << 4) | (s & 15);
	s >>= 4;
	ആകുമ്പോൾ (s) {
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
	തിരികെ obj;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ delta_pos_compare(സ്ഥിരം ശൂന്യം *_a, സ്ഥിരം ശൂന്യം *_b)
{
	ഘടന ref_delta_entry *a = *(ഘടന ref_delta_entry **)_a;
	ഘടന ref_delta_entry *b = *(ഘടന ref_delta_entry **)_b;
	തിരികെ a->obj_no - b->obj_no;
}

സ്ഥിതം ശൂന്യം fix_unresolved_deltas(ഘടന hashfile *f)
{
	ഘടന ref_delta_entry **sorted_by_pos;
	പൂർണ്ണസംഖ്യ i;

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
	വേണ്ടി (i = 0; i < nr_ref_deltas; i++)
		sorted_by_pos[i] = &ref_deltas[i];
	QSORT(sorted_by_pos, nr_ref_deltas, delta_pos_compare);

	എങ്കിൽ (repo_has_promisor_remote(the_repository)) {
		/*
		 * Prefetch the delta bases.
		 */
		ഘടന oid_array to_fetch = OID_ARRAY_INIT;
		വേണ്ടി (i = 0; i < nr_ref_deltas; i++) {
			ഘടന ref_delta_entry *d = sorted_by_pos[i];
			എങ്കിൽ (!odb_read_object_info_extended(the_repository->objects,
							   &d->oid, ശൂന്യ,
							   OBJECT_INFO_FOR_PREFETCH))
				തുടരുക;
			oid_array_append(&to_fetch, &d->oid);
		}
		promisor_remote_get_direct(the_repository,
					   to_fetch.oid, to_fetch.nr);
		oid_array_clear(&to_fetch);
	}

	വേണ്ടി (i = 0; i < nr_ref_deltas; i++) {
		ഘടന ref_delta_entry *d = sorted_by_pos[i];
		എണ്ണൽ object_type type;
		ശൂന്യം *data;
		size_t size;

		എങ്കിൽ (objects[d->obj_no].real_type != OBJ_REF_DELTA)
			തുടരുക;
		data = odb_read_object(the_repository->objects, &d->oid,
				       &type, &size);
		എങ്കിൽ (!data)
			തുടരുക;

		എങ്കിൽ (check_object_signature(the_repository, &d->oid, data, size,
					   type) < 0)
			die(_("local object %s is corrupt"), oid_to_hex(&d->oid));

		/*
		 * Add this as an object to the objects array and call
		 * threaded_second_pass() (which will pick up the added
		 * object).
		 */
		append_obj_to_pack(f, d->oid.hash, data, size, type);
		സ്വതന്ത്രം(data);
		threaded_second_pass(ശൂന്യ);

		display_progress(progress, nr_resolved_deltas);
	}
	സ്വതന്ത്രം(sorted_by_pos);
}

സ്ഥിതം സ്ഥിരം അക്ഷരം *derive_filename(സ്ഥിരം അക്ഷരം *pack_name, സ്ഥിരം അക്ഷരം *strip,
				   സ്ഥിരം അക്ഷരം *suffix, ഘടന strbuf *buf)
{
	size_t len;
	എങ്കിൽ (!strip_suffix(pack_name, strip, &len) || !len ||
	    pack_name[len - 1] != '.')
		die(_("packfile name '%s' does not end with '.%s'"),
		    pack_name, strip);
	strbuf_add(buf, pack_name, len);
	strbuf_addstr(buf, suffix);
	തിരികെ buf->buf;
}

സ്ഥിതം ശൂന്യം write_special_file(സ്ഥിരം അക്ഷരം *suffix, സ്ഥിരം അക്ഷരം *സന്ദേശം,
			       സ്ഥിരം അക്ഷരം *pack_name, സ്ഥിരം unsigned അക്ഷരം *hash,
			       സ്ഥിരം അക്ഷരം **report)
{
	ഘടന strbuf name_buf = STRBUF_INIT;
	സ്ഥിരം അക്ഷരം *filename;
	പൂർണ്ണസംഖ്യ fd;
	പൂർണ്ണസംഖ്യ സന്ദേശം_len = strlen(സന്ദേശം);

	എങ്കിൽ (pack_name)
		filename = derive_filename(pack_name, "pack", suffix, &name_buf);
	അല്ലെങ്കിൽ
		filename = odb_pack_name(the_repository, &name_buf, hash, suffix);

	fd = safe_create_file_with_leading_directories(the_repository, filename);
	എങ്കിൽ (fd < 0) {
		എങ്കിൽ (errno != EEXIST)
			die_errno(_("cannot write %s file '%s'"),
				  suffix, filename);
	} അല്ലെങ്കിൽ {
		എങ്കിൽ (സന്ദേശം_len > 0) {
			write_or_die(fd, സന്ദേശം, സന്ദേശം_len);
			write_or_die(fd, "\n", 1);
		}
		എങ്കിൽ (close(fd) != 0)
			die_errno(_("cannot close written %s file '%s'"),
				  suffix, filename);
		എങ്കിൽ (report)
			*report = suffix;
	}
	strbuf_release(&name_buf);
}

സ്ഥിതം ശൂന്യം rename_tmp_packfile(സ്ഥിരം അക്ഷരം **final_name,
				സ്ഥിരം അക്ഷരം *curr_name,
				ഘടന strbuf *name, unsigned അക്ഷരം *hash,
				സ്ഥിരം അക്ഷരം *ext, പൂർണ്ണസംഖ്യ make_read_only_if_same)
{
	എങ്കിൽ (!*final_name || strcmp(*final_name, curr_name)) {
		എങ്കിൽ (!*final_name)
			*final_name = odb_pack_name(the_repository, name, hash, ext);
		എങ്കിൽ (finalize_object_file(the_repository, curr_name, *final_name))
			die(_("unable to rename temporary '*.%s' file to '%s'"),
			    ext, *final_name);
	} അല്ലെങ്കിൽ എങ്കിൽ (make_read_only_if_same) {
		chmod(*final_name, 0444);
	}
}

സ്ഥിതം ശൂന്യം final(സ്ഥിരം അക്ഷരം *final_pack_name, സ്ഥിരം അക്ഷരം *curr_pack_name,
		  സ്ഥിരം അക്ഷരം *final_index_name, സ്ഥിരം അക്ഷരം *curr_index_name,
		  സ്ഥിരം അക്ഷരം *final_rev_index_name, സ്ഥിരം അക്ഷരം *curr_rev_index_name,
		  സ്ഥിരം അക്ഷരം *keep_സന്ദേശം, സ്ഥിരം അക്ഷരം *promisor_സന്ദേശം,
		  unsigned അക്ഷരം *hash)
{
	സ്ഥിരം അക്ഷരം *report = "pack";
	ഘടന strbuf pack_name = STRBUF_INIT;
	ഘടന strbuf index_name = STRBUF_INIT;
	ഘടന strbuf rev_index_name = STRBUF_INIT;

	എങ്കിൽ (!from_stdin) {
		close(നിവേശം_fd);
	} അല്ലെങ്കിൽ {
		fsync_component_or_die(FSYNC_COMPONENT_PACK, output_fd, curr_pack_name);
		എങ്കിൽ (close(output_fd))
			die_errno(_("error while closing pack file"));
	}

	എങ്കിൽ (keep_സന്ദേശം)
		write_special_file("keep", keep_സന്ദേശം, final_pack_name, hash,
				   &report);
	എങ്കിൽ (promisor_സന്ദേശം)
		write_special_file("promisor", promisor_സന്ദേശം, final_pack_name,
				   hash, ശൂന്യ);

	rename_tmp_packfile(&final_pack_name, curr_pack_name, &pack_name,
			    hash, "pack", from_stdin);
	എങ്കിൽ (curr_rev_index_name)
		rename_tmp_packfile(&final_rev_index_name, curr_rev_index_name,
				    &rev_index_name, hash, "rev", 1);
	rename_tmp_packfile(&final_index_name, curr_index_name, &index_name,
			    hash, "idx", 1);

	എങ്കിൽ (do_fsck_object && startup_info->have_repository) {
		ഘടന odb_source_files *files =
			odb_source_files_downcast(the_repository->objects->source);
		packfile_store_load_pack(files->dirs->packed, final_index_name, 0);
	}

	എങ്കിൽ (!from_stdin) {
		printf("%s\n", hash_to_hex(hash));
	} അല്ലെങ്കിൽ {
		ഘടന strbuf buf = STRBUF_INIT;

		strbuf_addf(&buf, "%s\t%s\n", report, hash_to_hex(hash));
		write_or_die(1, buf.buf, buf.len);
		strbuf_release(&buf);

		/* Write the last part of the buffer to stdout */
		write_in_full(1, നിവേശം_buffer + നിവേശം_offset, നിവേശം_len);
	}

	strbuf_release(&rev_index_name);
	strbuf_release(&index_name);
	strbuf_release(&pack_name);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ git_index_pack_config(സ്ഥിരം അക്ഷരം *k, സ്ഥിരം അക്ഷരം *v,
				 സ്ഥിരം ഘടന config_context *ctx, ശൂന്യം *cb)
{
	ഘടന pack_idx_option *opts = cb;

	എങ്കിൽ (!strcmp(k, "pack.indexversion")) {
		opts->version = git_config_int(k, v, ctx->kvi);
		എങ്കിൽ (opts->version > 2)
			die(_("bad pack.indexVersion=%"PRIu32), opts->version);
		തിരികെ 0;
	}
	എങ്കിൽ (!strcmp(k, "pack.threads")) {
		nr_threads = git_config_int(k, v, ctx->kvi);
		എങ്കിൽ (nr_threads < 0)
			die(_("invalid number of threads specified (%d)"),
			    nr_threads);
		എങ്കിൽ (!HAVE_THREADS && nr_threads != 1) {
			warning(_("no threads support, ignoring %s"), k);
			nr_threads = 1;
		}
		തിരികെ 0;
	}
	എങ്കിൽ (!strcmp(k, "pack.writereverseindex")) {
		എങ്കിൽ (git_config_bool(k, v))
			opts->flags |= WRITE_REV;
		അല്ലെങ്കിൽ
			opts->flags &= ~WRITE_REV;
	}
	എങ്കിൽ (!strcmp(k, "core.deltabasecachelimit")) {
		opts->delta_base_cache_limit = git_config_ulong(k, v, ctx->kvi);
		തിരികെ 0;
	}
	തിരികെ git_default_config(k, v, ctx, cb);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ cmp_uint32(സ്ഥിരം ശൂന്യം *a_, സ്ഥിരം ശൂന്യം *b_)
{
	uint32_t a = *((uint32_t *)a_);
	uint32_t b = *((uint32_t *)b_);

	തിരികെ (a < b) ? -1 : (a != b);
}

സ്ഥിതം ശൂന്യം read_v2_anomalous_offsets(ഘടന packed_git *p,
				      ഘടന pack_idx_option *opts)
{
	സ്ഥിരം uint32_t *idx1, *idx2;
	uint32_t i;

	/* The address of the 4-byte offset table */
	idx1 = (((സ്ഥിരം uint32_t *)((സ്ഥിരം uint8_t *)p->index_data + p->crc_offset))
		+ (size_t)p->അക്കം_objects /* CRC32 table */
		);

	/* The address of the 8-byte offset table */
	idx2 = idx1 + p->അക്കം_objects;

	വേണ്ടി (i = 0; i < p->അക്കം_objects; i++) {
		uint32_t off = ntohl(idx1[i]);
		എങ്കിൽ (!(off & 0x80000000))
			തുടരുക;
		off = off & 0x7fffffff;
		check_pack_index_ptr(p, &idx2[off * 2]);
		എങ്കിൽ (idx2[off * 2])
			തുടരുക;
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

സ്ഥിതം ശൂന്യം read_idx_option(ഘടന pack_idx_option *opts, സ്ഥിരം അക്ഷരം *pack_name)
{
	ഘടന packed_git *p = add_packed_git(the_repository, pack_name,
					      strlen(pack_name), 1);

	എങ്കിൽ (!p)
		die(_("Cannot open existing pack file '%s'"), pack_name);
	എങ്കിൽ (open_pack_index(p))
		die(_("Cannot open existing pack idx file for '%s'"), pack_name);

	/* Read the attributes from the existing idx file */
	opts->version = p->index_version;

	എങ്കിൽ (opts->version == 2)
		read_v2_anomalous_offsets(p, opts);

	/*
	 * Get rid of the idx file as we do not need it anymore.
	 * NEEDSWORK: extract this bit from free_pack_by_name() in
	 * object-file.c, perhaps?  It shouldn't matter very much as we
	 * know we haven't installed this pack (hence we never have
	 * read anything from it).
	 */
	close_pack_index(p);
	സ്വതന്ത്രം(p);
}

സ്ഥിതം ശൂന്യം show_pack_info(പൂർണ്ണസംഖ്യ stat_only)
{
	പൂർണ്ണസംഖ്യ i, baseobjects = nr_objects - nr_ref_deltas - nr_ofs_deltas;
	unsigned long *chain_histogram = ശൂന്യ;

	എങ്കിൽ (deepest_delta)
		CALLOC_ARRAY(chain_histogram, deepest_delta);

	വേണ്ടി (i = 0; i < nr_objects; i++) {
		ഘടന object_entry *obj = &objects[i];

		എങ്കിൽ (is_delta_type(obj->type))
			chain_histogram[obj_stat[i].delta_depth - 1]++;
		എങ്കിൽ (stat_only)
			തുടരുക;
		printf("%s %-6s %"PRIuMAX" %"PRIuMAX" %"PRIuMAX,
		       oid_to_hex(&obj->idx.oid),
		       type_name(obj->real_type), (uintmax_t)obj->size,
		       (uintmax_t)(obj[1].idx.offset - obj->idx.offset),
		       (uintmax_t)obj->idx.offset);
		എങ്കിൽ (is_delta_type(obj->type)) {
			ഘടന object_entry *bobj = &objects[obj_stat[i].base_object_no];
			printf(" %u %s", obj_stat[i].delta_depth,
			       oid_to_hex(&bobj->idx.oid));
		}
		putchar('\n');
	}

	എങ്കിൽ (baseobjects)
		printf_ln(Q_("non delta: %d object",
			     "non delta: %d objects",
			     baseobjects),
			  baseobjects);
	വേണ്ടി (i = 0; i < deepest_delta; i++) {
		എങ്കിൽ (!chain_histogram[i])
			തുടരുക;
		printf_ln(Q_("chain length = %d: %lu object",
			     "chain length = %d: %lu objects",
			     chain_histogram[i]),
			  i + 1,
			  chain_histogram[i]);
	}
	സ്വതന്ത്രം(chain_histogram);
}

സ്ഥിതം ശൂന്യം repack_local_links(ശൂന്യം)
{
	ഘടന child_process cmd = CHILD_PROCESS_INIT;
	FILE *out;
	ഘടന strbuf വരി = STRBUF_INIT;
	ഘടന oidset_iter iter;
	ഘടന object_id *oid;
	അക്ഷരം *base_name = ശൂന്യ;

	എങ്കിൽ (!oidset_size(&outgoing_links))
		തിരികെ;

	oidset_iter_init(&outgoing_links, &iter);
	ആകുമ്പോൾ ((oid = oidset_iter_next(&iter))) {
		ഘടന odb_source_info source_info;
		ഘടന object_info info = {
			.source_infop = &source_info,
		};

		എങ്കിൽ (odb_read_object_info_extended(the_repository->objects, oid, &info, 0))
			/* Missing; assume it is a promisor object */
			തുടരുക;
		എങ്കിൽ (source_info.source->type == ODB_SOURCE_PACKED &&
		    source_info.u.packed.pack->pack_promisor)
			തുടരുക;

		എങ്കിൽ (!cmd.args.nr) {
			base_name = mkpathdup(
				"%s/pack/pack",
				repo_get_object_directory(the_repository));
			strvec_തള്ളുക(&cmd.args, "pack-objects");
			strvec_തള്ളുക(&cmd.args,
				    "--exclude-promisor-objects-best-effort");
			strvec_തള്ളുക(&cmd.args, base_name);
			cmd.git_cmd = 1;
			cmd.in = -1;
			cmd.out = -1;
			എങ്കിൽ (start_command(&cmd))
				die(_("could not start pack-objects to repack local links"));
		}

		എങ്കിൽ (write_in_full(cmd.in, oid_to_hex(oid), the_hash_algo->hexsz) < 0 ||
		    write_in_full(cmd.in, "\n", 1) < 0)
			die(_("failed to feed local object to pack-objects"));
	}

	എങ്കിൽ (!cmd.args.nr)
		തിരികെ;

	close(cmd.in);

	out = xfdopen(cmd.out, "r");
	ആകുമ്പോൾ (strbuf_getline_lf(&വരി, out) != EOF) {
		unsigned അക്ഷരം ദ്വിമാന[GIT_MAX_RAWSZ];
		എങ്കിൽ (വരി.len != the_hash_algo->hexsz ||
		    !hex_to_bytes(ദ്വിമാന, വരി.buf, വരി.len))
			die(_("index-pack: Expecting full hex object ID lines only from pack-objects."));

		/*
		 * pack-objects creates the .pack and .idx files, but not the
		 * .promisor file. Create the .promisor file, which is empty.
		 */
		write_special_file("promisor", "", ശൂന്യ, ദ്വിമാന, ശൂന്യ);
	}

	fclose(out);
	എങ്കിൽ (finish_command(&cmd))
		die(_("could not finish pack-objects to repack local links"));
	strbuf_release(&വരി);
	സ്വതന്ത്രം(base_name);
}

പൂർണ്ണസംഖ്യ cmd_index_pack(പൂർണ്ണസംഖ്യ argc,
		   സ്ഥിരം അക്ഷരം **argv,
		   സ്ഥിരം അക്ഷരം *prefix,
		   ഘടന repository *repo)
{
	പൂർണ്ണസംഖ്യ i, fix_thin_pack = 0, verify = 0, stat_only = 0, rev_index;
	സ്ഥിരം അക്ഷരം *curr_index;
	അക്ഷരം *curr_rev_index = ശൂന്യ;
	സ്ഥിരം അക്ഷരം *index_name = ശൂന്യ, *pack_name = ശൂന്യ, *rev_index_name = ശൂന്യ;
	സ്ഥിരം അക്ഷരം *keep_സന്ദേശം = ശൂന്യ;
	സ്ഥിരം അക്ഷരം *promisor_സന്ദേശം = ശൂന്യ;
	ഘടന strbuf index_name_buf = STRBUF_INIT;
	ഘടന strbuf rev_index_name_buf = STRBUF_INIT;
	ഘടന pack_idx_entry **idx_objects;
	ഘടന pack_idx_option opts;
	unsigned അക്ഷരം pack_hash[GIT_MAX_RAWSZ];
	unsigned foreign_nr = 1;	/* zero is a "good" value, assume bad */
	പൂർണ്ണസംഖ്യ report_end_of_നിവേശം = 0;
	പൂർണ്ണസംഖ്യ hash_algo = 0;

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
	എങ്കിൽ (prefix && chdir(prefix))
		die(_("Cannot come back to cwd"));

	എങ്കിൽ (git_env_bool(GIT_TEST_NO_WRITE_REV_INDEX, 0))
		rev_index = 0;
	അല്ലെങ്കിൽ
		rev_index = !!(opts.flags & (WRITE_REV_VERIFY | WRITE_REV));

	വേണ്ടി (i = 1; i < argc; i++) {
		സ്ഥിരം അക്ഷരം *arg = argv[i];

		എങ്കിൽ (*arg == '-') {
			എങ്കിൽ (!strcmp(arg, "--stdin")) {
				from_stdin = 1;
			} അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(arg, "--fix-thin")) {
				fix_thin_pack = 1;
			} അല്ലെങ്കിൽ എങ്കിൽ (skip_to_optional_arg(arg, "--strict", &arg)) {
				strict = 1;
				do_fsck_object = 1;
				fsck_set_സന്ദേശം_types(&fsck_options, arg);
			} അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(arg, "--check-self-contained-and-connected")) {
				strict = 1;
				check_self_contained_and_connected = 1;
			} അല്ലെങ്കിൽ എങ്കിൽ (skip_to_optional_arg(arg, "--fsck-objects", &arg)) {
				do_fsck_object = 1;
				fsck_set_സന്ദേശം_types(&fsck_options, arg);
			} അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(arg, "--verify")) {
				verify = 1;
			} അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(arg, "--verify-stat")) {
				verify = 1;
				show_stat = 1;
			} അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(arg, "--verify-stat-only")) {
				verify = 1;
				show_stat = 1;
				stat_only = 1;
			} അല്ലെങ്കിൽ എങ്കിൽ (skip_to_optional_arg(arg, "--keep", &keep_സന്ദേശം)) {
				; /* nothing to do */
			} അല്ലെങ്കിൽ എങ്കിൽ (skip_to_optional_arg(arg, "--promisor", &promisor_സന്ദേശം)) {
				record_outgoing_links = 1;
			} അല്ലെങ്കിൽ എങ്കിൽ (starts_with(arg, "--threads=")) {
				അക്ഷരം *end;
				nr_threads = strtoul(arg+10, &end, 0);
				എങ്കിൽ (!arg[10] || *end || nr_threads < 0)
					usage(index_pack_usage);
				എങ്കിൽ (!HAVE_THREADS && nr_threads != 1) {
					warning(_("no threads support, ignoring %s"), arg);
					nr_threads = 1;
				}
			} അല്ലെങ്കിൽ എങ്കിൽ (skip_prefix(arg, "--pack_header=", &arg)) {
				എങ്കിൽ (parse_pack_header_option(arg,
							     നിവേശം_buffer,
							     &നിവേശം_len) < 0)
					die(_("bad --pack_header: %s"), arg);
			} അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(arg, "-v")) {
				verbose = 1;
			} അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(arg, "--progress-title")) {
				എങ്കിൽ (progress_title || (i+1) >= argc)
					usage(index_pack_usage);
				progress_title = argv[++i];
			} അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(arg, "--show-resolving-progress")) {
				show_resolving_progress = 1;
			} അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(arg, "--report-end-of-input")) {
				report_end_of_നിവേശം = 1;
			} അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(arg, "-o")) {
				എങ്കിൽ (index_name || (i+1) >= argc)
					usage(index_pack_usage);
				index_name = argv[++i];
			} അല്ലെങ്കിൽ എങ്കിൽ (starts_with(arg, "--index-version=")) {
				അക്ഷരം *c;
				opts.version = strtoul(arg + 16, &c, 10);
				എങ്കിൽ (opts.version > 2)
					die(_("bad %s"), arg);
				എങ്കിൽ (*c == ',')
					opts.off32_limit = strtoul(c+1, &c, 0);
				എങ്കിൽ (*c || opts.off32_limit & 0x80000000)
					die(_("bad %s"), arg);
			} അല്ലെങ്കിൽ എങ്കിൽ (skip_prefix(arg, "--max-input-size=", &arg)) {
				max_നിവേശം_size = strtoumax(arg, ശൂന്യ, 10);
			} അല്ലെങ്കിൽ എങ്കിൽ (skip_prefix(arg, "--object-format=", &arg)) {
				hash_algo = hash_algo_by_name(arg);
				എങ്കിൽ (hash_algo == GIT_HASH_UNKNOWN)
					die(_("unknown hash algorithm '%s'"), arg);
				repo_set_hash_algo(the_repository, hash_algo);
			} അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(arg, "--rev-index")) {
				rev_index = 1;
			} അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(arg, "--no-rev-index")) {
				rev_index = 0;
			} അല്ലെങ്കിൽ
				usage(index_pack_usage);
			തുടരുക;
		}

		എങ്കിൽ (pack_name)
			usage(index_pack_usage);
		pack_name = arg;
	}

	എങ്കിൽ (!pack_name && !from_stdin)
		usage(index_pack_usage);
	എങ്കിൽ (fix_thin_pack && !from_stdin)
		die(_("the option '%s' requires '%s'"), "--fix-thin", "--stdin");
	എങ്കിൽ (promisor_സന്ദേശം && pack_name)
		die(_("--promisor cannot be used with a pack name"));
	എങ്കിൽ (from_stdin && !startup_info->have_repository)
		die(_("--stdin requires a git repository"));
	എങ്കിൽ (from_stdin && hash_algo)
		die(_("options '%s' and '%s' cannot be used together"), "--object-format", "--stdin");
	എങ്കിൽ (!index_name && pack_name)
		index_name = derive_filename(pack_name, "pack", "idx", &index_name_buf);

	/*
	 * Packfiles and indices do not carry enough information to be able to
	 * identify their object hash. So when we are neither in a repository
	 * nor has the user told us which object hash to use we have no other
	 * choice but to guess the object hash.
	 */
	എങ്കിൽ (!the_repository->hash_algo)
		repo_set_hash_algo(the_repository, GIT_HASH_DEFAULT);

	opts.flags &= ~(WRITE_REV | WRITE_REV_VERIFY);
	എങ്കിൽ (rev_index) {
		opts.flags |= verify ? WRITE_REV_VERIFY : WRITE_REV;
		എങ്കിൽ (index_name)
			rev_index_name = derive_filename(index_name,
							 "idx", "rev",
							 &rev_index_name_buf);
	}

	എങ്കിൽ (verify) {
		എങ്കിൽ (!index_name)
			die(_("--verify with no packfile name given"));
		read_idx_option(&opts, index_name);
		opts.flags |= WRITE_IDX_VERIFY | WRITE_IDX_STRICT;
	}
	എങ്കിൽ (strict)
		opts.flags |= WRITE_IDX_STRICT;

	എങ്കിൽ (HAVE_THREADS && !nr_threads) {
		nr_threads = online_cpus();
		/*
		 * Experiments show that going above 20 threads doesn't help,
		 * no matter how many cores you have. Below that, we tend to
		 * max at half the number of online_cpus(), presumably because
		 * half of those are hyperthreads rather than full cores. We'll
		 * never reduce the level below "3", though, to match a
		 * historical value that nobody complained about.
		 */
		എങ്കിൽ (nr_threads < 4)
			; /* too few cores to consider capping */
		അല്ലെങ്കിൽ എങ്കിൽ (nr_threads < 6)
			nr_threads = 3; /* historic cap */
		അല്ലെങ്കിൽ എങ്കിൽ (nr_threads < 40)
			nr_threads /= 2;
		അല്ലെങ്കിൽ
			nr_threads = 20; /* hard cap */
	}

	curr_pack = open_pack_file(pack_name);
	parse_pack_header();
	CALLOC_ARRAY(objects, st_add(nr_objects, 1));
	എങ്കിൽ (show_stat)
		CALLOC_ARRAY(obj_stat, st_add(nr_objects, 1));
	CALLOC_ARRAY(ofs_deltas, nr_objects);
	parse_pack_objects(pack_hash);
	എങ്കിൽ (report_end_of_നിവേശം)
		write_in_full(2, "\0", 1);
	resolve_deltas(&opts);
	conclude_pack(fix_thin_pack, curr_pack, pack_hash);
	സ്വതന്ത്രം(ofs_deltas);
	സ്വതന്ത്രം(ref_deltas);
	എങ്കിൽ (strict)
		foreign_nr = check_objects();

	എങ്കിൽ (show_stat)
		show_pack_info(stat_only);

	ALLOC_ARRAY(idx_objects, nr_objects);
	വേണ്ടി (i = 0; i < nr_objects; i++)
		idx_objects[i] = &objects[i].idx;
	curr_index = write_idx_file(the_repository, index_name, idx_objects,
				    nr_objects, &opts, pack_hash);
	എങ്കിൽ (rev_index)
		curr_rev_index = write_rev_file(the_repository, rev_index_name,
						idx_objects, nr_objects,
						pack_hash, opts.flags);
	സ്വതന്ത്രം(idx_objects);

	എങ്കിൽ (!verify)
		final(pack_name, curr_pack,
		      index_name, curr_index,
		      rev_index_name, curr_rev_index,
		      keep_സന്ദേശം, promisor_സന്ദേശം,
		      pack_hash);
	അല്ലെങ്കിൽ
		close(നിവേശം_fd);

	എങ്കിൽ (do_fsck_object) {
		/*
		 * We cannot perform queued consistency checks when running
		 * outside of a repository because those require us to read
		 * from the object database, which is uninitialized.
		 *
		 * TODO: we may eventually set up an in-memory object database,
		 * which would allow us to perform these queued checks.
		 */
		എങ്കിൽ (!startup_info->have_repository &&
		    fsck_has_queued_checks(&fsck_options))
			die(_("cannot perform queued object checks outside "
			      "of a repository"));

		എങ്കിൽ (fsck_finish(&fsck_options))
			die(_("fsck error in pack objects"));
	}

	സ്വതന്ത്രം(opts.anomaly);
	സ്വതന്ത്രം(objects);
	strbuf_release(&index_name_buf);
	strbuf_release(&rev_index_name_buf);
	എങ്കിൽ (!pack_name)
		സ്വതന്ത്രം((ശൂന്യം *) curr_pack);
	എങ്കിൽ (!index_name)
		സ്വതന്ത്രം((ശൂന്യം *) curr_index);
	സ്വതന്ത്രം(curr_rev_index);

	repack_local_links();

	/*
	 * Let the caller know this pack is not self contained
	 */
	എങ്കിൽ (check_self_contained_and_connected && foreign_nr)
		തിരികെ 1;

	തിരികെ 0;
}
