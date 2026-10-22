#घोषणा USE_THE_REPOSITORY_VARIABLE
#घोषणा DISABLE_SIGN_COMPARE_WARNINGS

#समावेश "builtin.h"
#समावेश "config.h"
#समावेश "delta.h"
#समावेश "environment.h"
#समावेश "gettext.h"
#समावेश "hex.h"
#समावेश "pack.h"
#समावेश "csum-file.h"
#समावेश "blob.h"
#समावेश "commit.h"
#समावेश "tag.h"
#समावेश "tree.h"
#समावेश "progress.h"
#समावेश "fsck.h"
#समावेश "strbuf.h"
#समावेश "thread-utils.h"
#समावेश "packfile.h"
#समावेश "pack-revindex.h"
#समावेश "object-file.h"
#समावेश "odb.h"
#समावेश "odb/streaming.h"
#समावेश "oid-array.h"
#समावेश "oidset.h"
#समावेश "path.h"
#समावेश "replace-object.h"
#समावेश "tree-walk.h"
#समावेश "promisor-remote.h"
#समावेश "run-command.h"
#समावेश "setup.h"
#समावेश "strvec.h"

अटल स्थिर अक्षर सूचकांक_pack_usage[] =
"git index-pack [-v] [-o <index-file>] [--keep | --keep=<msg>] [--[no-]rev-index] [--verify] [--strict[=<msg-id>=<severity>...]] [--fsck-objects[=<msg-id>=<severity>...]] (<pack-file> | --stdin [--fix-thin] [<pack-file>])";

काष्ठा object_entry {
	काष्ठा pack_idx_entry idx;
	माप_प्रकार आकार;
	अचिन्हित अक्षर hdr_आकार;
	चिन्हित अक्षर type;
	चिन्हित अक्षर real_type;
};

काष्ठा object_stat {
	अचिन्हित delta_depth;
	पूर्णांक base_object_no;
};

काष्ठा base_तथ्य {
	/* Initialized by make_base(). */
	काष्ठा base_तथ्य *base;
	काष्ठा object_entry *obj;
	पूर्णांक ref_first, ref_last;
	पूर्णांक ofs_first, ofs_last;
	/*
	 * Threads should increment retain_data if they are about to call
	 * patch_delta() using this struct's data as a base, and decrement this
	 * when they are done. While retain_data is nonzero, this struct's data
	 * will not be freed even if the delta base cache limit is exceeded.
	 */
	पूर्णांक retain_तथ्य;
	/*
	 * The number of direct children that have not been fully processed
	 * (entered work_head, entered done_head, left done_head). When this
	 * number reaches zero, this struct base_data can be freed.
	 */
	पूर्णांक children_remaining;

	/* Not initialized by make_base(). */
	काष्ठा list_शीर्ष list;
	व्योम *तथ्य;
	माप_प्रकार आकार;
};

/*
 * Stack of struct base_data that have unprocessed children.
 * threaded_second_pass() uses this as a source of work (the other being the
 * objects array).
 *
 * Guarded by work_mutex.
 */
अटल LIST_शीर्ष(work_शीर्ष);

/*
 * Stack of struct base_data that have children, all of whom have been
 * processed or are being processed, and at least one child is being processed.
 * These struct base_data must be kept around until the last child is
 * processed.
 *
 * Guarded by work_mutex.
 */
अटल LIST_शीर्ष(done_शीर्ष);

/*
 * All threads share one delta base cache.
 *
 * base_cache_used is guarded by work_mutex, and base_cache_limit is read-only
 * in a thread.
 */
अटल माप_प्रकार base_cache_used;
अटल माप_प्रकार base_cache_limit;

काष्ठा thread_local_तथ्य {
	pthread_t thread;
	पूर्णांक pack_fd;
};

/* Remember to update object flag allocation in object.h */
#घोषणा FLAG_LINK (1u<<20)
#घोषणा FLAG_CHECKED (1u<<21)

काष्ठा ofs_delta_entry {
	off_t offset;
	पूर्णांक obj_no;
};

काष्ठा ref_delta_entry {
	काष्ठा object_id oid;
	पूर्णांक obj_no;
};

अटल काष्ठा object_entry *objects;
अटल काष्ठा object_stat *obj_stat;
अटल काष्ठा ofs_delta_entry *ofs_deltas;
अटल काष्ठा ref_delta_entry *ref_deltas;
अटल काष्ठा thread_local_तथ्य nothread_तथ्य;
अटल पूर्णांक nr_objects;
अटल पूर्णांक nr_ofs_deltas;
अटल पूर्णांक nr_ref_deltas;
अटल पूर्णांक ref_deltas_alloc;
अटल पूर्णांक nr_resolved_deltas;
अटल पूर्णांक nr_threads;

अटल पूर्णांक from_stdin;
अटल पूर्णांक strict;
अटल पूर्णांक do_fsck_object;
अटल काष्ठा fsck_options fsck_options;
अटल पूर्णांक verbose;
अटल स्थिर अक्षर *progress_title;
अटल पूर्णांक show_resolving_progress;
अटल पूर्णांक show_stat;
अटल पूर्णांक check_self_contained_and_connected;

अटल काष्ठा progress *progress;

अटल अचिन्हित अक्षर निवेश_बफर[DEFAULT_IO_बफर_आकार];
अटल अचिन्हित पूर्णांक निवेश_offset, निवेश_len;
अटल off_t consumed_bytes;
अटल off_t max_निवेश_आकार;
अटल अचिन्हित deepest_delta;
अटल काष्ठा git_hash_ctx निवेश_ctx;
अटल uint32_t निवेश_crc32;
अटल पूर्णांक निवेश_fd, output_fd;
अटल स्थिर अक्षर *curr_pack;

/*
 * outgoing_links is guarded by read_mutex, and record_outgoing_links is
 * read-only in a thread.
 */
अटल काष्ठा oidset outgoing_links = OIDSET_INIT;
अटल पूर्णांक record_outgoing_links;

अटल काष्ठा thread_local_तथ्य *thread_तथ्य;
अटल पूर्णांक nr_dispatched;
अटल पूर्णांक threads_active;

अटल pthread_mutex_t read_mutex;
#घोषणा read_lock()		lock_mutex(&read_mutex)
#घोषणा read_unlock()		unlock_mutex(&read_mutex)

अटल pthread_mutex_t गणक_mutex;
#घोषणा गणक_lock()		lock_mutex(&गणक_mutex)
#घोषणा गणक_unlock()	unlock_mutex(&गणक_mutex)

अटल pthread_mutex_t work_mutex;
#घोषणा work_lock()		lock_mutex(&work_mutex)
#घोषणा work_unlock()		unlock_mutex(&work_mutex)

अटल pthread_mutex_t deepest_delta_mutex;
#घोषणा deepest_delta_lock()	lock_mutex(&deepest_delta_mutex)
#घोषणा deepest_delta_unlock()	unlock_mutex(&deepest_delta_mutex)

अटल pthread_कुंजी_t कुंजी;

अटल अंतरभूत व्योम lock_mutex(pthread_mutex_t *mutex)
{
	अगर (threads_active)
		pthread_mutex_lock(mutex);
}

अटल अंतरभूत व्योम unlock_mutex(pthread_mutex_t *mutex)
{
	अगर (threads_active)
		pthread_mutex_unlock(mutex);
}

/*
 * Mutex and conditional variable can't be statically-initialized on Windows.
 */
अटल व्योम init_thread(व्योम)
{
	पूर्णांक i;
	init_recursive_mutex(&read_mutex);
	pthread_mutex_init(&गणक_mutex, शून्य);
	pthread_mutex_init(&work_mutex, शून्य);
	अगर (show_stat)
		pthread_mutex_init(&deepest_delta_mutex, शून्य);
	pthread_कुंजी_create(&कुंजी, शून्य);
	CALLOC_ARRAY(thread_तथ्य, nr_threads);
	क्रम (i = 0; i < nr_threads; i++) {
		thread_तथ्य[i].pack_fd = xopen(curr_pack, O_RDONLY);
	}

	threads_active = 1;
}

अटल व्योम cleanup_thread(व्योम)
{
	पूर्णांक i;
	अगर (!threads_active)
		वापस;
	threads_active = 0;
	pthread_mutex_destroy(&read_mutex);
	pthread_mutex_destroy(&गणक_mutex);
	pthread_mutex_destroy(&work_mutex);
	अगर (show_stat)
		pthread_mutex_destroy(&deepest_delta_mutex);
	क्रम (i = 0; i < nr_threads; i++)
		बंद(thread_तथ्य[i].pack_fd);
	pthread_कुंजी_delete(कुंजी);
	मुक्त(thread_तथ्य);
}

अटल पूर्णांक mark_link(काष्ठा object *obj, क्रमागत object_type type,
		     व्योम *तथ्य UNUSED,
		     काष्ठा fsck_options *options UNUSED)
{
	अगर (!obj)
		वापस -1;

	अगर (type != OBJ_ANY && obj->type != type)
		die(_("object type mismatch at %s"), oid_to_hex(&obj->oid));

	obj->flags |= FLAG_LINK;
	वापस 0;
}

/* The content of each linked object must have been checked
   or it must be already present in the object database */
अटल अचिन्हित check_object(काष्ठा object *obj)
{
	अगर (!obj)
		वापस 0;

	अगर (!(obj->flags & FLAG_LINK))
		वापस 0;

	अगर (!(obj->flags & FLAG_CHECKED)) {
		माप_प्रकार आकार;
		पूर्णांक type = odb_read_object_info(the_repository->objects,
						&obj->oid, &आकार);
		अगर (type <= 0)
			die(_("did not receive expected object %s"),
			      oid_to_hex(&obj->oid));
		अगर (type != obj->type)
			die(_("object %s: expected type %s, found %s"),
			    oid_to_hex(&obj->oid),
			    type_name(obj->type), type_name(type));
		obj->flags |= FLAG_CHECKED;
		वापस 1;
	}

	वापस 0;
}

अटल अचिन्हित check_objects(व्योम)
{
	अचिन्हित i, max, foreign_nr = 0;

	max = get_max_object_सूचकांक(the_repository);

	अगर (verbose)
		progress = start_delayed_progress(the_repository,
						  _("Checking objects"), max);

	क्रम (i = 0; i < max; i++) {
		foreign_nr += check_object(get_indexed_object(the_repository, i));
		display_progress(progress, i + 1);
	}

	stop_progress(&progress);
	वापस foreign_nr;
}


/* Discard current buffer used content. */
अटल व्योम flush(व्योम)
{
	अगर (निवेश_offset) {
		अगर (output_fd >= 0)
			write_or_die(output_fd, निवेश_बफर, निवेश_offset);
		git_hash_update(&निवेश_ctx, निवेश_बफर, निवेश_offset);
		स_हटाओ(निवेश_बफर, निवेश_बफर + निवेश_offset, निवेश_len);
		निवेश_offset = 0;
	}
}

/*
 * Make sure at least "min" bytes are available in the buffer, and
 * return the pointer to the buffer.
 */
अटल व्योम *fill(पूर्णांक min)
{
	अगर (min <= निवेश_len)
		वापस निवेश_बफर + निवेश_offset;
	अगर (min > माप(निवेश_बफर))
		die(Q_("cannot fill %d byte",
		       "cannot fill %d bytes",
		       min),
		    min);
	flush();
	करो {
		ssize_t ret = xread(निवेश_fd, निवेश_बफर + निवेश_len,
				माप(निवेश_बफर) - निवेश_len);
		अगर (ret <= 0) {
			अगर (!ret)
				die(_("early EOF"));
			die_errno(_("read error on input"));
		}
		निवेश_len += ret;
		अगर (from_stdin)
			display_throughput(progress, consumed_bytes + निवेश_len);
	} जबतक (निवेश_len < min);
	वापस निवेश_बफर;
}

अटल व्योम use(पूर्णांक bytes)
{
	अगर (bytes > निवेश_len)
		die(_("used more bytes than were available"));
	निवेश_crc32 = crc32(निवेश_crc32, निवेश_बफर + निवेश_offset, bytes);
	निवेश_len -= bytes;
	निवेश_offset += bytes;

	/* make sure off_t is sufficiently large not to wrap */
	अगर (signed_add_overflows(consumed_bytes, bytes))
		die(_("pack too large for current definition of off_t"));
	consumed_bytes += bytes;
	अगर (max_निवेश_आकार && consumed_bytes > max_निवेश_आकार) {
		काष्ठा strbuf आकार_limit = STRBUF_INIT;
		strbuf_humanise_bytes(&आकार_limit, max_निवेश_आकार);
		die(_("pack exceeds maximum allowed size (%s)"),
		    आकार_limit.buf);
	}
}

अटल स्थिर अक्षर *open_pack_file(स्थिर अक्षर *pack_name)
{
	अगर (from_stdin) {
		निवेश_fd = 0;
		अगर (!pack_name) {
			काष्ठा strbuf tmp_file = STRBUF_INIT;
			output_fd = odb_mkstemp(the_repository->objects, &tmp_file,
						"pack/tmp_pack_XXXXXX");
			pack_name = strbuf_detach(&tmp_file, शून्य);
		} अन्यथा {
			output_fd = xopen(pack_name, O_CREAT|O_EXCL|O_RDWR, 0600);
		}
		nothread_तथ्य.pack_fd = output_fd;
	} अन्यथा {
		निवेश_fd = xopen(pack_name, O_RDONLY);
		output_fd = -1;
		nothread_तथ्य.pack_fd = निवेश_fd;
	}
	git_hash_init(&निवेश_ctx, the_hash_algo);
	वापस pack_name;
}

अटल व्योम parse_pack_header(व्योम)
{
	अचिन्हित अक्षर *hdr = fill(माप(काष्ठा pack_header));

	/* Header consistency check */
	अगर (get_be32(hdr) != PACK_SIGNATURE)
		die(_("pack signature mismatch"));
	hdr += 4;
	अगर (!pack_version_ok_native(get_be32(hdr)))
		die(_("pack version %"PRIu32" unsupported"),
		    get_be32(hdr));
	hdr += 4;

	nr_objects = get_be32(hdr);
	use(माप(काष्ठा pack_header));
}

__attribute__((format (म_लिखो, 2, 3)))
अटल NORETURN व्योम bad_object(off_t offset, स्थिर अक्षर *format, ...)
{
	बहु_सूची params;
	अक्षर buf[1024];

	बहु_शुरू(params, format);
	vsnprintf(buf, माप(buf), format, params);
	बहु_पूर्ण(params);
	die(_("pack has bad object at offset %"PRIuMAX": %s"),
	    (uintmax_t)offset, buf);
}

अटल अंतरभूत काष्ठा thread_local_तथ्य *get_thread_तथ्य(व्योम)
{
	अगर (HAVE_THREADS) {
		अगर (threads_active)
			वापस pthread_getspecific(कुंजी);
		निश्चित(!threads_active &&
		       "This should only be reached when all threads are gone");
	}
	वापस &nothread_तथ्य;
}

अटल व्योम set_thread_तथ्य(काष्ठा thread_local_तथ्य *तथ्य)
{
	अगर (threads_active)
		pthread_setspecific(कुंजी, तथ्य);
}

अटल व्योम free_base_तथ्य(काष्ठा base_तथ्य *c)
{
	अगर (c->तथ्य) {
		FREE_AND_NULL(c->तथ्य);
		base_cache_used -= c->आकार;
	}
}

अटल व्योम prune_base_तथ्य(काष्ठा base_तथ्य *retain)
{
	काष्ठा list_शीर्ष *pos;

	अगर (base_cache_used <= base_cache_limit)
		वापस;

	list_for_each_पूर्व(pos, &done_शीर्ष) {
		काष्ठा base_तथ्य *b = list_entry(pos, काष्ठा base_तथ्य, list);
		अगर (b->retain_तथ्य || b == retain)
			जारी;
		अगर (b->तथ्य) {
			free_base_तथ्य(b);
			अगर (base_cache_used <= base_cache_limit)
				वापस;
		}
	}

	list_for_each_पूर्व(pos, &work_शीर्ष) {
		काष्ठा base_तथ्य *b = list_entry(pos, काष्ठा base_तथ्य, list);
		अगर (b->retain_तथ्य || b == retain)
			जारी;
		अगर (b->तथ्य) {
			free_base_तथ्य(b);
			अगर (base_cache_used <= base_cache_limit)
				वापस;
		}
	}
}

अटल पूर्णांक is_delta_type(क्रमागत object_type type)
{
	वापस (type == OBJ_REF_DELTA || type == OBJ_OFS_DELTA);
}

अटल व्योम *unpack_entry_तथ्य(off_t offset, माप_प्रकार आकार,
			       क्रमागत object_type type, काष्ठा object_id *oid)
{
	अटल अक्षर fixed_buf[8192];
	पूर्णांक status;
	git_zstream stream;
	व्योम *buf;
	काष्ठा git_hash_ctx c;
	अक्षर hdr[32];
	पूर्णांक hdrlen;

	अगर (!is_delta_type(type)) {
		hdrlen = format_object_header(hdr, माप(hdr), type, आकार);
		git_hash_init(&c, the_hash_algo);
		git_hash_update(&c, hdr, hdrlen);
	} अन्यथा
		oid = शून्य;
	अगर (type == OBJ_BLOB &&
	    आकार > repo_settings_get_big_file_threshold(the_repository))
		buf = fixed_buf;
	अन्यथा
		buf = xmallocz(आकार);

	स_रखो(&stream, 0, माप(stream));
	git_inflate_init(&stream);
	stream.अग्र_out = buf;
	stream.avail_out = buf == fixed_buf ? माप(fixed_buf) : आकार;

	करो {
		अचिन्हित अक्षर *last_out = stream.अग्र_out;
		stream.अग्र_in = fill(1);
		stream.avail_in = निवेश_len;
		status = git_inflate(&stream, 0);
		use(निवेश_len - stream.avail_in);
		अगर (oid)
			git_hash_update(&c, last_out, stream.अग्र_out - last_out);
		अगर (buf == fixed_buf) {
			stream.अग्र_out = buf;
			stream.avail_out = माप(fixed_buf);
		}
	} जबतक (status == Z_OK);
	अगर (stream.योग_out != आकार || status != Z_STREAM_END)
		bad_object(offset, _("inflate returned %d"), status);
	git_inflate_end(&stream);
	अगर (oid)
		git_hash_final_oid(oid, &c);
	वापस buf == fixed_buf ? शून्य : buf;
}

अटल व्योम *unpack_raw_entry(काष्ठा object_entry *obj,
			      off_t *ofs_offset,
			      काष्ठा object_id *ref_oid,
			      काष्ठा object_id *oid)
{
	अचिन्हित अक्षर *p;
	माप_प्रकार आकार, c;
	off_t base_offset;
	अचिन्हित shift;
	व्योम *तथ्य;

	obj->idx.offset = consumed_bytes;
	निवेश_crc32 = crc32(0, शून्य, 0);

	p = fill(1);
	c = *p;
	use(1);
	obj->type = (c >> 4) & 7;
	आकार = (c & 15);
	shift = 4;
	जबतक (c & 0x80) {
		अगर ((bitsizeof(माप_प्रकार) - 7) < shift)
			die(_("object size too large for this platform"));
		p = fill(1);
		c = *p;
		use(1);
		आकार += (c & 0x7f) << shift;
		shift += 7;
	}
	obj->आकार = आकार;

	चयन (obj->type) {
	हाल OBJ_REF_DELTA:
		oidread(ref_oid, fill(the_hash_algo->rawsz),
			the_repository->hash_algo);
		use(the_hash_algo->rawsz);
		अवरोध;
	हाल OBJ_OFS_DELTA:
		p = fill(1);
		c = *p;
		use(1);
		base_offset = c & 127;
		जबतक (c & 128) {
			base_offset += 1;
			अगर (!base_offset || MSB(base_offset, 7))
				bad_object(obj->idx.offset, _("offset value overflow for delta base object"));
			p = fill(1);
			c = *p;
			use(1);
			base_offset = (base_offset << 7) + (c & 127);
		}
		*ofs_offset = obj->idx.offset - base_offset;
		अगर (*ofs_offset <= 0 || *ofs_offset >= obj->idx.offset)
			bad_object(obj->idx.offset, _("delta base offset is out of bound"));
		अवरोध;
	हाल OBJ_COMMIT:
	हाल OBJ_वृक्ष:
	हाल OBJ_BLOB:
	हाल OBJ_TAG:
		अवरोध;
	शेष:
		bad_object(obj->idx.offset, _("unknown object type %d"), obj->type);
	}
	obj->hdr_आकार = consumed_bytes - obj->idx.offset;

	तथ्य = unpack_entry_तथ्य(obj->idx.offset, obj->आकार, obj->type, oid);
	obj->idx.crc32 = निवेश_crc32;
	वापस तथ्य;
}

अटल व्योम *unpack_तथ्य(काष्ठा object_entry *obj,
			 पूर्णांक (*consume)(स्थिर अचिन्हित अक्षर *, अचिन्हित दीर्घ, व्योम *),
			 व्योम *cb_तथ्य)
{
	off_t from = obj[0].idx.offset + obj[0].hdr_आकार;
	off_t len = obj[1].idx.offset - from;
	अचिन्हित अक्षर *तथ्य, *inbuf;
	git_zstream stream;
	पूर्णांक status;

	तथ्य = xmallocz(consume ? 64*1024 : obj->आकार);
	inbuf = xmalloc((len < 64*1024) ? (पूर्णांक)len : 64*1024);

	स_रखो(&stream, 0, माप(stream));
	git_inflate_init(&stream);
	stream.अग्र_out = तथ्य;
	stream.avail_out = consume ? 64*1024 : obj->आकार;

	करो {
		ssize_t n = (len < 64*1024) ? (ssize_t)len : 64*1024;
		n = xpread(get_thread_तथ्य()->pack_fd, inbuf, n, from);
		अगर (n < 0)
			die_errno(_("cannot pread pack file"));
		अगर (!n)
			die(Q_("premature end of pack file, %"PRIuMAX" byte missing",
			       "premature end of pack file, %"PRIuMAX" bytes missing",
			       len),
			    (uintmax_t)len);
		from += n;
		len -= n;
		stream.अग्र_in = inbuf;
		stream.avail_in = n;
		अगर (!consume)
			status = git_inflate(&stream, 0);
		अन्यथा {
			करो {
				status = git_inflate(&stream, 0);
				अगर (consume(तथ्य, stream.अग्र_out - तथ्य, cb_तथ्य)) {
					मुक्त(inbuf);
					मुक्त(तथ्य);
					वापस शून्य;
				}
				stream.अग्र_out = तथ्य;
				stream.avail_out = 64*1024;
			} जबतक (status == Z_OK && stream.avail_in);
		}
	} जबतक (len && status == Z_OK && !stream.avail_in);

	/* This has been inflated OK when first encountered, so... */
	अगर (status != Z_STREAM_END || stream.योग_out != obj->आकार)
		die(_("serious inflate inconsistency"));

	git_inflate_end(&stream);
	मुक्त(inbuf);
	अगर (consume) {
		FREE_AND_NULL(तथ्य);
	}
	वापस तथ्य;
}

अटल व्योम *get_तथ्य_from_pack(काष्ठा object_entry *obj)
{
	वापस unpack_तथ्य(obj, शून्य, शून्य);
}

अटल पूर्णांक compare_ofs_delta_bases(off_t offset1, off_t offset2,
				   क्रमागत object_type type1,
				   क्रमागत object_type type2)
{
	पूर्णांक cmp = type1 - type2;
	अगर (cmp)
		वापस cmp;
	वापस offset1 < offset2 ? -1 :
	       offset1 > offset2 ?  1 :
	       0;
}

अटल पूर्णांक find_ofs_delta(स्थिर off_t offset)
{
	पूर्णांक first = 0, last = nr_ofs_deltas;

	जबतक (first < last) {
		पूर्णांक अग्र = first + (last - first) / 2;
		काष्ठा ofs_delta_entry *delta = &ofs_deltas[अग्र];
		पूर्णांक cmp;

		cmp = compare_ofs_delta_bases(offset, delta->offset,
					      OBJ_OFS_DELTA,
					      objects[delta->obj_no].type);
		अगर (!cmp)
			वापस अग्र;
		अगर (cmp < 0) {
			last = अग्र;
			जारी;
		}
		first = अग्र+1;
	}
	वापस -first-1;
}

अटल व्योम find_ofs_delta_children(off_t offset,
				    पूर्णांक *first_सूचकांक, पूर्णांक *last_सूचकांक)
{
	पूर्णांक first = find_ofs_delta(offset);
	पूर्णांक last = first;
	पूर्णांक end = nr_ofs_deltas - 1;

	अगर (first < 0) {
		*first_सूचकांक = 0;
		*last_सूचकांक = -1;
		वापस;
	}
	जबतक (first > 0 && ofs_deltas[first - 1].offset == offset)
		--first;
	जबतक (last < end && ofs_deltas[last + 1].offset == offset)
		++last;
	*first_सूचकांक = first;
	*last_सूचकांक = last;
}

अटल पूर्णांक compare_ref_delta_bases(स्थिर काष्ठा object_id *oid1,
				   स्थिर काष्ठा object_id *oid2,
				   क्रमागत object_type type1,
				   क्रमागत object_type type2)
{
	पूर्णांक cmp = type1 - type2;
	अगर (cmp)
		वापस cmp;
	वापस oidcmp(oid1, oid2);
}

अटल पूर्णांक find_ref_delta(स्थिर काष्ठा object_id *oid)
{
	पूर्णांक first = 0, last = nr_ref_deltas;

	जबतक (first < last) {
		पूर्णांक अग्र = first + (last - first) / 2;
		काष्ठा ref_delta_entry *delta = &ref_deltas[अग्र];
		पूर्णांक cmp;

		cmp = compare_ref_delta_bases(oid, &delta->oid,
					      OBJ_REF_DELTA,
					      objects[delta->obj_no].type);
		अगर (!cmp)
			वापस अग्र;
		अगर (cmp < 0) {
			last = अग्र;
			जारी;
		}
		first = अग्र+1;
	}
	वापस -first-1;
}

अटल व्योम find_ref_delta_children(स्थिर काष्ठा object_id *oid,
				    पूर्णांक *first_सूचकांक, पूर्णांक *last_सूचकांक)
{
	पूर्णांक first = find_ref_delta(oid);
	पूर्णांक last = first;
	पूर्णांक end = nr_ref_deltas - 1;

	अगर (first < 0) {
		*first_सूचकांक = 0;
		*last_सूचकांक = -1;
		वापस;
	}
	जबतक (first > 0 && oideq(&ref_deltas[first - 1].oid, oid))
		--first;
	जबतक (last < end && oideq(&ref_deltas[last + 1].oid, oid))
		++last;
	*first_सूचकांक = first;
	*last_सूचकांक = last;
}

काष्ठा compare_तथ्य {
	काष्ठा object_entry *entry;
	काष्ठा odb_stream *st;
	अचिन्हित अक्षर *buf;
	अचिन्हित दीर्घ buf_आकार;
};

अटल पूर्णांक compare_objects(स्थिर अचिन्हित अक्षर *buf, अचिन्हित दीर्घ आकार,
			   व्योम *cb_तथ्य)
{
	काष्ठा compare_तथ्य *तथ्य = cb_तथ्य;

	अगर (तथ्य->buf_आकार < आकार) {
		मुक्त(तथ्य->buf);
		तथ्य->buf = xmalloc(आकार);
		तथ्य->buf_आकार = आकार;
	}

	जबतक (आकार) {
		ssize_t len = odb_stream_read(तथ्य->st, तथ्य->buf, आकार);
		अगर (len == 0)
			die(_("SHA1 COLLISION FOUND WITH %s !"),
			    oid_to_hex(&तथ्य->entry->idx.oid));
		अगर (len < 0)
			die(_("unable to read %s"),
			    oid_to_hex(&तथ्य->entry->idx.oid));
		अगर (स_भेद(buf, तथ्य->buf, len))
			die(_("SHA1 COLLISION FOUND WITH %s !"),
			    oid_to_hex(&तथ्य->entry->idx.oid));
		आकार -= len;
		buf += len;
	}
	वापस 0;
}

अटल पूर्णांक check_collison(काष्ठा object_entry *entry)
{
	काष्ठा compare_तथ्य तथ्य;

	अगर (entry->आकार <= repo_settings_get_big_file_threshold(the_repository) ||
	    entry->type != OBJ_BLOB)
		वापस -1;

	स_रखो(&तथ्य, 0, माप(तथ्य));
	तथ्य.entry = entry;
	तथ्य.st = odb_stream_from_object(the_repository->objects, &entry->idx.oid, शून्य);
	अगर (!तथ्य.st)
		वापस -1;
	अगर (तथ्य.st->आकार != entry->आकार || तथ्य.st->type != entry->type)
		die(_("SHA1 COLLISION FOUND WITH %s !"),
		    oid_to_hex(&entry->idx.oid));
	unpack_तथ्य(entry, compare_objects, &तथ्य);
	odb_stream_close(तथ्य.st);
	मुक्त(तथ्य.buf);
	वापस 0;
}

अटल व्योम record_outgoing_link(स्थिर काष्ठा object_id *oid)
{
	oidset_जोड़ो(&outgoing_links, oid);
}

अटल व्योम maybe_record_name_entry(स्थिर काष्ठा name_entry *entry)
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
	अगर (S_ISDIR(entry->mode))
		record_outgoing_link(&entry->oid);
}

अटल व्योम do_record_outgoing_links(काष्ठा object *obj)
{
	अगर (obj->type == OBJ_वृक्ष) {
		काष्ठा वृक्ष *वृक्ष = (काष्ठा वृक्ष *)obj;
		काष्ठा वृक्ष_desc desc;
		काष्ठा name_entry entry;
		अगर (init_वृक्ष_desc_gently(&desc, &वृक्ष->object.oid,
					  वृक्ष->बफर, वृक्ष->आकार, 0))
			/*
			 * Error messages are given when packs are
			 * verified, so do not print any here.
			 */
			वापस;
		जबतक (वृक्ष_entry_gently(&desc, &entry))
			maybe_record_name_entry(&entry);
	} अन्यथा अगर (obj->type == OBJ_COMMIT) {
		काष्ठा commit *commit = (काष्ठा commit *) obj;
		काष्ठा commit_list *parents = commit->parents;

		record_outgoing_link(get_commit_वृक्ष_oid(commit));
		क्रम (; parents; parents = parents->अग्र)
			record_outgoing_link(&parents->item->object.oid);
	} अन्यथा अगर (obj->type == OBJ_TAG) {
		काष्ठा tag *tag = (काष्ठा tag *) obj;
		record_outgoing_link(get_tagged_oid(tag));
	}
}

अटल व्योम sha1_object(स्थिर व्योम *तथ्य, काष्ठा object_entry *obj_entry,
			अचिन्हित दीर्घ आकार, क्रमागत object_type type,
			स्थिर काष्ठा object_id *oid)
{
	व्योम *new_तथ्य = शून्य;
	पूर्णांक collision_test_needed = 0;

	निश्चित(तथ्य || obj_entry);

	अगर (startup_info->have_repository) {
		read_lock();
		collision_test_needed = odb_has_object(the_repository->objects, oid,
						       ODB_HAS_OBJECT_FETCH_PROMISOR);
		read_unlock();
	}

	अगर (collision_test_needed && !तथ्य) {
		read_lock();
		अगर (!check_collison(obj_entry))
			collision_test_needed = 0;
		read_unlock();
	}
	अगर (collision_test_needed) {
		व्योम *has_तथ्य;
		क्रमागत object_type has_type;
		माप_प्रकार has_आकार;
		read_lock();
		has_type = odb_read_object_info(the_repository->objects, oid, &has_आकार);
		अगर (has_type < 0)
			die(_("cannot read existing object info %s"), oid_to_hex(oid));
		अगर (has_type != type || has_आकार != आकार)
			die(_("SHA1 COLLISION FOUND WITH %s !"), oid_to_hex(oid));
		has_तथ्य = odb_read_object(the_repository->objects, oid,
					   &has_type, &has_आकार);
		read_unlock();
		अगर (!तथ्य)
			तथ्य = new_तथ्य = get_तथ्य_from_pack(obj_entry);
		अगर (!has_तथ्य)
			die(_("cannot read existing object %s"), oid_to_hex(oid));
		अगर (आकार != has_आकार || type != has_type ||
		    स_भेद(तथ्य, has_तथ्य, आकार) != 0)
			die(_("SHA1 COLLISION FOUND WITH %s !"), oid_to_hex(oid));
		मुक्त(has_तथ्य);
	}

	अगर (strict || do_fsck_object || record_outgoing_links) {
		read_lock();
		अगर (type == OBJ_BLOB) {
			काष्ठा blob *blob = lookup_blob(the_repository, oid);
			अगर (blob)
				blob->object.flags |= FLAG_CHECKED;
			अन्यथा
				die(_("invalid blob object %s"), oid_to_hex(oid));
			अगर (do_fsck_object &&
			    fsck_object(&blob->object, (व्योम *)तथ्य, आकार, &fsck_options))
				die(_("fsck error in packed object"));
		} अन्यथा {
			काष्ठा object *obj;
			पूर्णांक eaten;
			व्योम *buf = (व्योम *) तथ्य;

			निश्चित(तथ्य && "data can only be NULL for large _blobs_");

			/*
			 * we do not need to free the memory here, as the
			 * buf is deleted by the caller.
			 */
			obj = parse_object_बफर(the_repository, oid, type,
						  आकार, buf,
						  &eaten);
			अगर (!obj)
				die(_("invalid %s"), type_name(type));
			अगर (do_fsck_object &&
			    fsck_object(obj, buf, आकार, &fsck_options))
				die(_("fsck error in packed object"));
			अगर (strict && fsck_walk(obj, शून्य, &fsck_options))
				die(_("Not all child objects of %s are reachable"), oid_to_hex(&obj->oid));
			अगर (record_outgoing_links)
				do_record_outgoing_links(obj);

			अगर (obj->type == OBJ_वृक्ष) {
				काष्ठा वृक्ष *item = (काष्ठा वृक्ष *) obj;
				item->बफर = शून्य;
				obj->parsed = 0;
			}
			अगर (obj->type == OBJ_COMMIT) {
				काष्ठा commit *commit = (काष्ठा commit *) obj;
				अगर (detach_commit_बफर(commit, शून्य) != तथ्य)
					BUG("parse_object_buffer transmogrified our buffer");
			}
			obj->flags |= FLAG_CHECKED;
		}
		read_unlock();
	}

	मुक्त(new_तथ्य);
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
अटल व्योम *get_base_तथ्य(काष्ठा base_तथ्य *c)
{
	अगर (!c->तथ्य) {
		काष्ठा object_entry *obj = c->obj;
		काष्ठा base_तथ्य **delta = शून्य;
		पूर्णांक delta_nr = 0, delta_alloc = 0;

		जबतक (is_delta_type(c->obj->type) && !c->तथ्य) {
			ALLOC_GROW(delta, delta_nr + 1, delta_alloc);
			delta[delta_nr++] = c;
			c = c->base;
		}
		अगर (!delta_nr) {
			c->तथ्य = get_तथ्य_from_pack(obj);
			c->आकार = obj->आकार;
			base_cache_used += c->आकार;
			prune_base_तथ्य(c);
		}
		क्रम (; delta_nr > 0; delta_nr--) {
			व्योम *base, *raw;
			c = delta[delta_nr - 1];
			obj = c->obj;
			base = get_base_तथ्य(c->base);
			raw = get_तथ्य_from_pack(obj);
			c->तथ्य = patch_delta(
				base, c->base->आकार,
				raw, obj->आकार,
				&c->आकार);
			मुक्त(raw);
			अगर (!c->तथ्य)
				bad_object(obj->idx.offset, _("failed to apply delta"));
			base_cache_used += c->आकार;
			prune_base_तथ्य(c);
		}
		मुक्त(delta);
	}
	वापस c->तथ्य;
}

अटल काष्ठा base_तथ्य *make_base(काष्ठा object_entry *obj,
				   काष्ठा base_तथ्य *parent)
{
	काष्ठा base_तथ्य *base = xcalloc(1, माप(काष्ठा base_तथ्य));
	base->base = parent;
	base->obj = obj;
	find_ref_delta_children(&obj->idx.oid,
				&base->ref_first, &base->ref_last);
	find_ofs_delta_children(obj->idx.offset,
				&base->ofs_first, &base->ofs_last);
	base->children_remaining = base->ref_last - base->ref_first +
		base->ofs_last - base->ofs_first + 2;
	वापस base;
}

अटल काष्ठा base_तथ्य *resolve_delta(काष्ठा object_entry *delta_obj,
				       काष्ठा base_तथ्य *base)
{
	व्योम *delta_तथ्य, *परिणाम_तथ्य;
	काष्ठा base_तथ्य *परिणाम;
	माप_प्रकार परिणाम_आकार;

	अगर (show_stat) {
		पूर्णांक i = delta_obj - objects;
		पूर्णांक j = base->obj - objects;
		obj_stat[i].delta_depth = obj_stat[j].delta_depth + 1;
		deepest_delta_lock();
		अगर (deepest_delta < obj_stat[i].delta_depth)
			deepest_delta = obj_stat[i].delta_depth;
		deepest_delta_unlock();
		obj_stat[i].base_object_no = j;
	}
	delta_तथ्य = get_तथ्य_from_pack(delta_obj);
	निश्चित(base->तथ्य);
	परिणाम_तथ्य = patch_delta(base->तथ्य, base->आकार,
				  delta_तथ्य, delta_obj->आकार, &परिणाम_आकार);
	मुक्त(delta_तथ्य);
	अगर (!परिणाम_तथ्य)
		bad_object(delta_obj->idx.offset, _("failed to apply delta"));
	hash_object_file(the_hash_algo, परिणाम_तथ्य, परिणाम_आकार,
			 delta_obj->real_type, &delta_obj->idx.oid);
	sha1_object(परिणाम_तथ्य, शून्य, परिणाम_आकार, delta_obj->real_type,
		    &delta_obj->idx.oid);

	परिणाम = make_base(delta_obj, base);
	परिणाम->तथ्य = परिणाम_तथ्य;
	परिणाम->आकार = परिणाम_आकार;

	गणक_lock();
	nr_resolved_deltas++;
	गणक_unlock();

	वापस परिणाम;
}

अटल पूर्णांक compare_ofs_delta_entry(स्थिर व्योम *a, स्थिर व्योम *b)
{
	स्थिर काष्ठा ofs_delta_entry *delta_a = a;
	स्थिर काष्ठा ofs_delta_entry *delta_b = b;

	वापस delta_a->offset < delta_b->offset ? -1 :
	       delta_a->offset > delta_b->offset ?  1 :
	       0;
}

अटल पूर्णांक compare_ref_delta_entry(स्थिर व्योम *a, स्थिर व्योम *b)
{
	स्थिर काष्ठा ref_delta_entry *delta_a = a;
	स्थिर काष्ठा ref_delta_entry *delta_b = b;

	वापस oidcmp(&delta_a->oid, &delta_b->oid);
}

अटल व्योम *threaded_second_pass(व्योम *तथ्य)
{
	अगर (तथ्य)
		set_thread_तथ्य(तथ्य);
	क्रम (;;) {
		काष्ठा base_तथ्य *parent = शून्य;
		काष्ठा object_entry *child_obj = शून्य;
		काष्ठा base_तथ्य *child = शून्य;

		गणक_lock();
		display_progress(progress, nr_resolved_deltas);
		गणक_unlock();

		work_lock();
		अगर (list_empty(&work_शीर्ष)) {
			/*
			 * Take an object from the object array.
			 */
			जबतक (nr_dispatched < nr_objects &&
			       is_delta_type(objects[nr_dispatched].type))
				nr_dispatched++;
			अगर (nr_dispatched >= nr_objects) {
				work_unlock();
				अवरोध;
			}
			child_obj = &objects[nr_dispatched++];
		} अन्यथा {
			/*
			 * Peek at the top of the stack, and take a child from
			 * it.
			 */
			parent = list_first_entry(&work_शीर्ष, काष्ठा base_तथ्य,
						  list);

			जबतक (parent->ref_first <= parent->ref_last) {
				पूर्णांक offset = ref_deltas[parent->ref_first++].obj_no;
				child_obj = objects + offset;
				अगर (child_obj->real_type != OBJ_REF_DELTA) {
					child_obj = शून्य;
					जारी;
				}
				child_obj->real_type = parent->obj->real_type;
				अवरोध;
			}

			अगर (!child_obj && parent->ofs_first <= parent->ofs_last) {
				child_obj = objects +
					ofs_deltas[parent->ofs_first++].obj_no;
				निश्चित(child_obj->real_type == OBJ_OFS_DELTA);
				child_obj->real_type = parent->obj->real_type;
			}

			अगर (parent->ref_first > parent->ref_last &&
			    parent->ofs_first > parent->ofs_last) {
				/*
				 * This parent has run out of children, so move
				 * it to done_head.
				 */
				list_del(&parent->list);
				list_add(&parent->list, &done_शीर्ष);
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
			get_base_तथ्य(parent);
			parent->retain_तथ्य++;
		}
		work_unlock();

		अगर (child_obj) {
			अगर (parent) {
				child = resolve_delta(child_obj, parent);
				अगर (!child->children_remaining)
					FREE_AND_NULL(child->तथ्य);
			} अन्यथा{
				child = make_base(child_obj, शून्य);
				अगर (child->children_remaining) {
					/*
					 * Since this child has its own delta children,
					 * we will need this data in the future.
					 * Inflate now so that future iterations will
					 * have access to this object's data while
					 * outside the work mutex.
					 */
					child->तथ्य = get_तथ्य_from_pack(child_obj);
					child->आकार = child_obj->आकार;
				}
			}
		}

		work_lock();
		अगर (parent)
			parent->retain_तथ्य--;

		अगर (child && child->तथ्य) {
			/*
			 * This child has its own children, so add it to
			 * work_head.
			 */
			list_add(&child->list, &work_शीर्ष);
			base_cache_used += child->आकार;
			prune_base_तथ्य(शून्य);
		} अन्यथा अगर (child) {
			/*
			 * This child does not have its own children. It may be
			 * the last descendant of its ancestors; free those
			 * that we can.
			 */
			काष्ठा base_तथ्य *p = parent;

			जबतक (p) {
				काष्ठा base_तथ्य *अग्र_p;

				p->children_remaining--;
				अगर (p->children_remaining)
					अवरोध;

				अग्र_p = p->base;
				free_base_तथ्य(p);
				list_del(&p->list);
				मुक्त(p);

				p = अग्र_p;
			}
			FREE_AND_NULL(child);
		}
		work_unlock();
	}
	वापस शून्य;
}

/*
 * First pass:
 * - find locations of all objects;
 * - calculate SHA1 of all non-delta objects;
 * - remember base (SHA1 or offset) for all deltas.
 */
अटल व्योम parse_pack_objects(अचिन्हित अक्षर *hash)
{
	पूर्णांक i, nr_delays = 0;
	काष्ठा ofs_delta_entry *ofs_delta = ofs_deltas;
	काष्ठा object_id ref_delta_oid;
	काष्ठा stat st;
	काष्ठा git_hash_ctx tmp_ctx;

	अगर (verbose)
		progress = start_progress(
				the_repository,
				progress_title ? progress_title :
				from_stdin ? _("Receiving objects") : _("Indexing objects"),
				nr_objects);
	क्रम (i = 0; i < nr_objects; i++) {
		काष्ठा object_entry *obj = &objects[i];
		व्योम *तथ्य = unpack_raw_entry(obj, &ofs_delta->offset,
					      &ref_delta_oid,
					      &obj->idx.oid);
		obj->real_type = obj->type;
		अगर (obj->type == OBJ_OFS_DELTA) {
			nr_ofs_deltas++;
			ofs_delta->obj_no = i;
			ofs_delta++;
		} अन्यथा अगर (obj->type == OBJ_REF_DELTA) {
			ALLOC_GROW(ref_deltas, nr_ref_deltas + 1, ref_deltas_alloc);
			oidcpy(&ref_deltas[nr_ref_deltas].oid, &ref_delta_oid);
			ref_deltas[nr_ref_deltas].obj_no = i;
			nr_ref_deltas++;
		} अन्यथा अगर (!तथ्य) {
			/* large blobs, check later */
			obj->real_type = OBJ_BAD;
			nr_delays++;
		} अन्यथा
			sha1_object(तथ्य, शून्य, obj->आकार, obj->type,
				    &obj->idx.oid);
		मुक्त(तथ्य);
		display_progress(progress, i+1);
	}
	objects[i].idx.offset = consumed_bytes;
	stop_progress(&progress);

	/* Check pack integrity */
	flush();
	git_hash_init(&tmp_ctx, the_hash_algo);
	git_hash_clone(&tmp_ctx, &निवेश_ctx);
	git_hash_final(hash, &tmp_ctx);
	अगर (!hasheq(fill(the_hash_algo->rawsz), hash, the_repository->hash_algo))
		die(_("pack is corrupted (SHA1 mismatch)"));
	use(the_hash_algo->rawsz);

	/* If input_fd is a file, we should have reached its end now. */
	अगर (ख_स्थिति(निवेश_fd, &st))
		die_errno(_("cannot fstat packfile"));
	अगर (S_ISREG(st.st_mode) &&
			lseek(निवेश_fd, 0, प्रस्तुत_से) - निवेश_len != st.st_आकार)
		die(_("pack has junk at the end"));

	क्रम (i = 0; i < nr_objects; i++) {
		काष्ठा object_entry *obj = &objects[i];
		अगर (obj->real_type != OBJ_BAD)
			जारी;
		obj->real_type = obj->type;
		sha1_object(शून्य, obj, obj->आकार, obj->type,
			    &obj->idx.oid);
		nr_delays--;
	}
	अगर (nr_delays)
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
अटल व्योम resolve_deltas(काष्ठा pack_idx_option *opts)
{
	पूर्णांक i;

	अगर (!nr_ofs_deltas && !nr_ref_deltas)
		वापस;

	/* Sort deltas by base SHA1/offset for fast searching */
	QSORT(ofs_deltas, nr_ofs_deltas, compare_ofs_delta_entry);
	QSORT(ref_deltas, nr_ref_deltas, compare_ref_delta_entry);

	अगर (verbose || show_resolving_progress)
		progress = start_progress(the_repository,
					  _("Resolving deltas"),
					  nr_ref_deltas + nr_ofs_deltas);

	nr_dispatched = 0;
	base_cache_limit = opts->delta_base_cache_limit * nr_threads;
	अगर (nr_threads > 1 || दो_पर्या("GIT_FORCE_THREADS")) {
		init_thread();
		क्रम (i = 0; i < nr_threads; i++) {
			पूर्णांक ret = pthread_create(&thread_तथ्य[i].thread, शून्य,
						 threaded_second_pass, thread_तथ्य + i);
			अगर (ret)
				die(_("unable to create thread: %s"),
				    म_त्रुटि(ret));
		}
		क्रम (i = 0; i < nr_threads; i++)
			pthread_join(thread_तथ्य[i].thread, शून्य);
		cleanup_thread();
		वापस;
	}
	threaded_second_pass(&nothread_तथ्य);
}

/*
 * Third pass:
 * - append objects to convert thin pack to full pack if required
 * - write the final pack hash
 */
अटल व्योम fix_unresolved_deltas(काष्ठा hashfile *f);
अटल व्योम conclude_pack(पूर्णांक fix_thin_pack, स्थिर अक्षर *curr_pack, अचिन्हित अक्षर *pack_hash)
{
	अगर (nr_ref_deltas + nr_ofs_deltas == nr_resolved_deltas) {
		stop_progress(&progress);
		/* Flush remaining pack final hash. */
		flush();
		वापस;
	}

	अगर (fix_thin_pack) {
		काष्ठा hashfile *f;
		अचिन्हित अक्षर read_hash[GIT_MAX_RAWSZ], पुच्छ_hash[GIT_MAX_RAWSZ];
		काष्ठा strbuf संदेश = STRBUF_INIT;
		पूर्णांक nr_unresolved = nr_ofs_deltas + nr_ref_deltas - nr_resolved_deltas;
		पूर्णांक nr_objects_initial = nr_objects;
		अगर (nr_unresolved <= 0)
			die(_("confusion beyond insanity"));
		REALLOC_ARRAY(objects, nr_objects + nr_unresolved + 1);
		स_रखो(objects + nr_objects + 1, 0,
		       nr_unresolved * माप(*objects));
		f = hashfd(the_repository->hash_algo, output_fd, curr_pack);
		fix_unresolved_deltas(f);
		strbuf_addf(&संदेश, Q_("completed with %d local object",
				     "completed with %d local objects",
				     nr_objects - nr_objects_initial),
			    nr_objects - nr_objects_initial);
		stop_progress_संदेश(&progress, संदेश.buf);
		strbuf_release(&संदेश);
		finalize_hashfile(f, पुच्छ_hash, FSYNC_COMPONENT_PACK, 0);
		hashcpy(read_hash, pack_hash, the_repository->hash_algo);
		fixup_pack_header_footer(the_hash_algo, output_fd, pack_hash,
					 curr_pack, nr_objects,
					 read_hash, consumed_bytes-the_hash_algo->rawsz);
		अगर (!hasheq(read_hash, पुच्छ_hash, the_repository->hash_algo))
			die(_("Unexpected tail checksum for %s "
			      "(disk corruption?)"), curr_pack);
	}
	अगर (nr_ofs_deltas + nr_ref_deltas != nr_resolved_deltas)
		die(Q_("pack has %d unresolved delta",
		       "pack has %d unresolved deltas",
		       nr_ofs_deltas + nr_ref_deltas - nr_resolved_deltas),
		    nr_ofs_deltas + nr_ref_deltas - nr_resolved_deltas);
}

अटल पूर्णांक write_compressed(काष्ठा hashfile *f, व्योम *in, अचिन्हित पूर्णांक आकार)
{
	git_zstream stream;
	पूर्णांक status;
	अचिन्हित अक्षर outbuf[4096];
	काष्ठा repo_config_मूल्यावली *cfg = repo_config_मूल्यावली(the_repository);

	git_deflate_init(&stream, cfg->zlib_compression_level);
	stream.अग्र_in = in;
	stream.avail_in = आकार;

	करो {
		stream.अग्र_out = outbuf;
		stream.avail_out = माप(outbuf);
		status = git_deflate(&stream, Z_FINISH);
		hashwrite(f, outbuf, माप(outbuf) - stream.avail_out);
	} जबतक (status == Z_OK);

	अगर (status != Z_STREAM_END)
		die(_("unable to deflate appended object (%d)"), status);
	आकार = stream.योग_out;
	git_deflate_end(&stream);
	वापस आकार;
}

अटल काष्ठा object_entry *append_obj_to_pack(काष्ठा hashfile *f,
			       स्थिर अचिन्हित अक्षर *sha1, व्योम *buf,
			       अचिन्हित दीर्घ आकार, क्रमागत object_type type)
{
	काष्ठा object_entry *obj = &objects[nr_objects++];
	अचिन्हित अक्षर header[10];
	अचिन्हित दीर्घ s = आकार;
	पूर्णांक n = 0;
	अचिन्हित अक्षर c = (type << 4) | (s & 15);
	s >>= 4;
	जबतक (s) {
		header[n++] = c | 0x80;
		c = s & 0x7f;
		s >>= 7;
	}
	header[n++] = c;
	crc32_begin(f);
	hashwrite(f, header, n);
	obj[0].आकार = आकार;
	obj[0].hdr_आकार = n;
	obj[0].type = type;
	obj[0].real_type = type;
	obj[1].idx.offset = obj[0].idx.offset + n;
	obj[1].idx.offset += write_compressed(f, buf, आकार);
	obj[0].idx.crc32 = crc32_end(f);
	hashflush(f);
	oidread(&obj->idx.oid, sha1, the_repository->hash_algo);
	वापस obj;
}

अटल पूर्णांक delta_pos_compare(स्थिर व्योम *_a, स्थिर व्योम *_b)
{
	काष्ठा ref_delta_entry *a = *(काष्ठा ref_delta_entry **)_a;
	काष्ठा ref_delta_entry *b = *(काष्ठा ref_delta_entry **)_b;
	वापस a->obj_no - b->obj_no;
}

अटल व्योम fix_unresolved_deltas(काष्ठा hashfile *f)
{
	काष्ठा ref_delta_entry **sorted_by_pos;
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
	क्रम (i = 0; i < nr_ref_deltas; i++)
		sorted_by_pos[i] = &ref_deltas[i];
	QSORT(sorted_by_pos, nr_ref_deltas, delta_pos_compare);

	अगर (repo_has_promisor_remote(the_repository)) {
		/*
		 * Prefetch the delta bases.
		 */
		काष्ठा oid_array to_fetch = OID_ARRAY_INIT;
		क्रम (i = 0; i < nr_ref_deltas; i++) {
			काष्ठा ref_delta_entry *d = sorted_by_pos[i];
			अगर (!odb_read_object_info_extended(the_repository->objects,
							   &d->oid, शून्य,
							   OBJECT_INFO_FOR_PREFETCH))
				जारी;
			oid_array_append(&to_fetch, &d->oid);
		}
		promisor_remote_get_direct(the_repository,
					   to_fetch.oid, to_fetch.nr);
		oid_array_clear(&to_fetch);
	}

	क्रम (i = 0; i < nr_ref_deltas; i++) {
		काष्ठा ref_delta_entry *d = sorted_by_pos[i];
		क्रमागत object_type type;
		व्योम *तथ्य;
		माप_प्रकार आकार;

		अगर (objects[d->obj_no].real_type != OBJ_REF_DELTA)
			जारी;
		तथ्य = odb_read_object(the_repository->objects, &d->oid,
				       &type, &आकार);
		अगर (!तथ्य)
			जारी;

		अगर (check_object_signature(the_repository, &d->oid, तथ्य, आकार,
					   type) < 0)
			die(_("local object %s is corrupt"), oid_to_hex(&d->oid));

		/*
		 * Add this as an object to the objects array and call
		 * threaded_second_pass() (which will pick up the added
		 * object).
		 */
		append_obj_to_pack(f, d->oid.hash, तथ्य, आकार, type);
		मुक्त(तथ्य);
		threaded_second_pass(शून्य);

		display_progress(progress, nr_resolved_deltas);
	}
	मुक्त(sorted_by_pos);
}

अटल स्थिर अक्षर *derive_filename(स्थिर अक्षर *pack_name, स्थिर अक्षर *strip,
				   स्थिर अक्षर *suffix, काष्ठा strbuf *buf)
{
	माप_प्रकार len;
	अगर (!strip_suffix(pack_name, strip, &len) || !len ||
	    pack_name[len - 1] != '.')
		die(_("packfile name '%s' does not end with '.%s'"),
		    pack_name, strip);
	strbuf_add(buf, pack_name, len);
	strbuf_addstr(buf, suffix);
	वापस buf->buf;
}

अटल व्योम write_special_file(स्थिर अक्षर *suffix, स्थिर अक्षर *संदेश,
			       स्थिर अक्षर *pack_name, स्थिर अचिन्हित अक्षर *hash,
			       स्थिर अक्षर **report)
{
	काष्ठा strbuf name_buf = STRBUF_INIT;
	स्थिर अक्षर *filename;
	पूर्णांक fd;
	पूर्णांक संदेश_len = म_माप(संदेश);

	अगर (pack_name)
		filename = derive_filename(pack_name, "pack", suffix, &name_buf);
	अन्यथा
		filename = odb_pack_name(the_repository, &name_buf, hash, suffix);

	fd = safe_create_file_with_leading_directories(the_repository, filename);
	अगर (fd < 0) {
		अगर (त्रुटि_सं != EEXIST)
			die_errno(_("cannot write %s file '%s'"),
				  suffix, filename);
	} अन्यथा {
		अगर (संदेश_len > 0) {
			write_or_die(fd, संदेश, संदेश_len);
			write_or_die(fd, "\n", 1);
		}
		अगर (बंद(fd) != 0)
			die_errno(_("cannot close written %s file '%s'"),
				  suffix, filename);
		अगर (report)
			*report = suffix;
	}
	strbuf_release(&name_buf);
}

अटल व्योम rename_tmp_packfile(स्थिर अक्षर **final_name,
				स्थिर अक्षर *curr_name,
				काष्ठा strbuf *name, अचिन्हित अक्षर *hash,
				स्थिर अक्षर *ext, पूर्णांक make_read_only_if_same)
{
	अगर (!*final_name || म_भेद(*final_name, curr_name)) {
		अगर (!*final_name)
			*final_name = odb_pack_name(the_repository, name, hash, ext);
		अगर (finalize_object_file(the_repository, curr_name, *final_name))
			die(_("unable to rename temporary '*.%s' file to '%s'"),
			    ext, *final_name);
	} अन्यथा अगर (make_read_only_if_same) {
		chmod(*final_name, 0444);
	}
}

अटल व्योम final(स्थिर अक्षर *final_pack_name, स्थिर अक्षर *curr_pack_name,
		  स्थिर अक्षर *final_सूचकांक_name, स्थिर अक्षर *curr_सूचकांक_name,
		  स्थिर अक्षर *final_rev_सूचकांक_name, स्थिर अक्षर *curr_rev_सूचकांक_name,
		  स्थिर अक्षर *keep_संदेश, स्थिर अक्षर *promisor_संदेश,
		  अचिन्हित अक्षर *hash)
{
	स्थिर अक्षर *report = "pack";
	काष्ठा strbuf pack_name = STRBUF_INIT;
	काष्ठा strbuf सूचकांक_name = STRBUF_INIT;
	काष्ठा strbuf rev_सूचकांक_name = STRBUF_INIT;

	अगर (!from_stdin) {
		बंद(निवेश_fd);
	} अन्यथा {
		fsync_component_or_die(FSYNC_COMPONENT_PACK, output_fd, curr_pack_name);
		अगर (बंद(output_fd))
			die_errno(_("error while closing pack file"));
	}

	अगर (keep_संदेश)
		write_special_file("keep", keep_संदेश, final_pack_name, hash,
				   &report);
	अगर (promisor_संदेश)
		write_special_file("promisor", promisor_संदेश, final_pack_name,
				   hash, शून्य);

	rename_tmp_packfile(&final_pack_name, curr_pack_name, &pack_name,
			    hash, "pack", from_stdin);
	अगर (curr_rev_सूचकांक_name)
		rename_tmp_packfile(&final_rev_सूचकांक_name, curr_rev_सूचकांक_name,
				    &rev_सूचकांक_name, hash, "rev", 1);
	rename_tmp_packfile(&final_सूचकांक_name, curr_सूचकांक_name, &सूचकांक_name,
			    hash, "idx", 1);

	अगर (do_fsck_object && startup_info->have_repository) {
		काष्ठा odb_source_files *files =
			odb_source_files_downcast(the_repository->objects->source);
		packfile_store_load_pack(files->dirs->packed, final_सूचकांक_name, 0);
	}

	अगर (!from_stdin) {
		म_लिखो("%s\n", hash_to_hex(hash));
	} अन्यथा {
		काष्ठा strbuf buf = STRBUF_INIT;

		strbuf_addf(&buf, "%s\t%s\n", report, hash_to_hex(hash));
		write_or_die(1, buf.buf, buf.len);
		strbuf_release(&buf);

		/* Write the last part of the buffer to stdout */
		write_in_full(1, निवेश_बफर + निवेश_offset, निवेश_len);
	}

	strbuf_release(&rev_सूचकांक_name);
	strbuf_release(&सूचकांक_name);
	strbuf_release(&pack_name);
}

अटल पूर्णांक git_सूचकांक_pack_config(स्थिर अक्षर *k, स्थिर अक्षर *v,
				 स्थिर काष्ठा config_context *ctx, व्योम *cb)
{
	काष्ठा pack_idx_option *opts = cb;

	अगर (!म_भेद(k, "pack.indexversion")) {
		opts->version = git_config_int(k, v, ctx->kvi);
		अगर (opts->version > 2)
			die(_("bad pack.indexVersion=%"PRIu32), opts->version);
		वापस 0;
	}
	अगर (!म_भेद(k, "pack.threads")) {
		nr_threads = git_config_int(k, v, ctx->kvi);
		अगर (nr_threads < 0)
			die(_("invalid number of threads specified (%d)"),
			    nr_threads);
		अगर (!HAVE_THREADS && nr_threads != 1) {
			warning(_("no threads support, ignoring %s"), k);
			nr_threads = 1;
		}
		वापस 0;
	}
	अगर (!म_भेद(k, "pack.writereverseindex")) {
		अगर (git_config_bool(k, v))
			opts->flags |= WRITE_REV;
		अन्यथा
			opts->flags &= ~WRITE_REV;
	}
	अगर (!म_भेद(k, "core.deltabasecachelimit")) {
		opts->delta_base_cache_limit = git_config_ulong(k, v, ctx->kvi);
		वापस 0;
	}
	वापस git_default_config(k, v, ctx, cb);
}

अटल पूर्णांक cmp_uint32(स्थिर व्योम *a_, स्थिर व्योम *b_)
{
	uint32_t a = *((uint32_t *)a_);
	uint32_t b = *((uint32_t *)b_);

	वापस (a < b) ? -1 : (a != b);
}

अटल व्योम read_v2_anomalous_offsets(काष्ठा packed_git *p,
				      काष्ठा pack_idx_option *opts)
{
	स्थिर uint32_t *idx1, *idx2;
	uint32_t i;

	/* The address of the 4-byte offset table */
	idx1 = (((स्थिर uint32_t *)((स्थिर uint8_t *)p->सूचकांक_तथ्य + p->crc_offset))
		+ (माप_प्रकार)p->अंक_objects /* CRC32 table */
		);

	/* The address of the 8-byte offset table */
	idx2 = idx1 + p->अंक_objects;

	क्रम (i = 0; i < p->अंक_objects; i++) {
		uint32_t off = ntohl(idx1[i]);
		अगर (!(off & 0x80000000))
			जारी;
		off = off & 0x7fffffff;
		check_pack_सूचकांक_ptr(p, &idx2[off * 2]);
		अगर (idx2[off * 2])
			जारी;
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

अटल व्योम read_idx_option(काष्ठा pack_idx_option *opts, स्थिर अक्षर *pack_name)
{
	काष्ठा packed_git *p = add_packed_git(the_repository, pack_name,
					      म_माप(pack_name), 1);

	अगर (!p)
		die(_("Cannot open existing pack file '%s'"), pack_name);
	अगर (open_pack_सूचकांक(p))
		die(_("Cannot open existing pack idx file for '%s'"), pack_name);

	/* Read the attributes from the existing idx file */
	opts->version = p->सूचकांक_version;

	अगर (opts->version == 2)
		read_v2_anomalous_offsets(p, opts);

	/*
	 * Get rid of the idx file as we do not need it anymore.
	 * NEEDSWORK: extract this bit from free_pack_by_name() in
	 * object-file.c, perhaps?  It shouldn't matter very much as we
	 * know we haven't installed this pack (hence we never have
	 * read anything from it).
	 */
	close_pack_सूचकांक(p);
	मुक्त(p);
}

अटल व्योम show_pack_info(पूर्णांक stat_only)
{
	पूर्णांक i, baseobjects = nr_objects - nr_ref_deltas - nr_ofs_deltas;
	अचिन्हित दीर्घ *chain_histogram = शून्य;

	अगर (deepest_delta)
		CALLOC_ARRAY(chain_histogram, deepest_delta);

	क्रम (i = 0; i < nr_objects; i++) {
		काष्ठा object_entry *obj = &objects[i];

		अगर (is_delta_type(obj->type))
			chain_histogram[obj_stat[i].delta_depth - 1]++;
		अगर (stat_only)
			जारी;
		म_लिखो("%s %-6s %"PRIuMAX" %"PRIuMAX" %"PRIuMAX,
		       oid_to_hex(&obj->idx.oid),
		       type_name(obj->real_type), (uintmax_t)obj->आकार,
		       (uintmax_t)(obj[1].idx.offset - obj->idx.offset),
		       (uintmax_t)obj->idx.offset);
		अगर (is_delta_type(obj->type)) {
			काष्ठा object_entry *bobj = &objects[obj_stat[i].base_object_no];
			म_लिखो(" %u %s", obj_stat[i].delta_depth,
			       oid_to_hex(&bobj->idx.oid));
		}
		अक्षर_दो('\n');
	}

	अगर (baseobjects)
		printf_ln(Q_("non delta: %d object",
			     "non delta: %d objects",
			     baseobjects),
			  baseobjects);
	क्रम (i = 0; i < deepest_delta; i++) {
		अगर (!chain_histogram[i])
			जारी;
		printf_ln(Q_("chain length = %d: %lu object",
			     "chain length = %d: %lu objects",
			     chain_histogram[i]),
			  i + 1,
			  chain_histogram[i]);
	}
	मुक्त(chain_histogram);
}

अटल व्योम repack_local_links(व्योम)
{
	काष्ठा child_process cmd = CHILD_PROCESS_INIT;
	खाता *out;
	काष्ठा strbuf पंक्ति = STRBUF_INIT;
	काष्ठा oidset_iter iter;
	काष्ठा object_id *oid;
	अक्षर *base_name = शून्य;

	अगर (!oidset_आकार(&outgoing_links))
		वापस;

	oidset_iter_init(&outgoing_links, &iter);
	जबतक ((oid = oidset_iter_अग्र(&iter))) {
		काष्ठा odb_source_info source_info;
		काष्ठा object_info info = {
			.source_infop = &source_info,
		};

		अगर (odb_read_object_info_extended(the_repository->objects, oid, &info, 0))
			/* Missing; assume it is a promisor object */
			जारी;
		अगर (source_info.source->type == ODB_SOURCE_PACKED &&
		    source_info.u.packed.pack->pack_promisor)
			जारी;

		अगर (!cmd.args.nr) {
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
			अगर (start_command(&cmd))
				die(_("could not start pack-objects to repack local links"));
		}

		अगर (write_in_full(cmd.in, oid_to_hex(oid), the_hash_algo->hexsz) < 0 ||
		    write_in_full(cmd.in, "\n", 1) < 0)
			die(_("failed to feed local object to pack-objects"));
	}

	अगर (!cmd.args.nr)
		वापस;

	बंद(cmd.in);

	out = xfdopen(cmd.out, "r");
	जबतक (strbuf_getline_lf(&पंक्ति, out) != खातापूर्ण) {
		अचिन्हित अक्षर द्विआधारी[GIT_MAX_RAWSZ];
		अगर (पंक्ति.len != the_hash_algo->hexsz ||
		    !hex_to_bytes(द्विआधारी, पंक्ति.buf, पंक्ति.len))
			die(_("index-pack: Expecting full hex object ID lines only from pack-objects."));

		/*
		 * pack-objects creates the .pack and .idx files, but not the
		 * .promisor file. Create the .promisor file, which is empty.
		 */
		write_special_file("promisor", "", शून्य, द्विआधारी, शून्य);
	}

	ख_बंद(out);
	अगर (finish_command(&cmd))
		die(_("could not finish pack-objects to repack local links"));
	strbuf_release(&पंक्ति);
	मुक्त(base_name);
}

पूर्णांक cmd_सूचकांक_pack(पूर्णांक argc,
		   स्थिर अक्षर **argv,
		   स्थिर अक्षर *prefix,
		   काष्ठा repository *repo)
{
	पूर्णांक i, fix_thin_pack = 0, verify = 0, stat_only = 0, rev_सूचकांक;
	स्थिर अक्षर *curr_सूचकांक;
	अक्षर *curr_rev_सूचकांक = शून्य;
	स्थिर अक्षर *सूचकांक_name = शून्य, *pack_name = शून्य, *rev_सूचकांक_name = शून्य;
	स्थिर अक्षर *keep_संदेश = शून्य;
	स्थिर अक्षर *promisor_संदेश = शून्य;
	काष्ठा strbuf सूचकांक_name_buf = STRBUF_INIT;
	काष्ठा strbuf rev_सूचकांक_name_buf = STRBUF_INIT;
	काष्ठा pack_idx_entry **idx_objects;
	काष्ठा pack_idx_option opts;
	अचिन्हित अक्षर pack_hash[GIT_MAX_RAWSZ];
	अचिन्हित foreign_nr = 1;	/* zero is a "good" value, assume bad */
	पूर्णांक report_end_of_निवेश = 0;
	पूर्णांक hash_algo = 0;

	show_usage_if_asked(argc, argv, सूचकांक_pack_usage);

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
	repo_config(the_repository, git_सूचकांक_pack_config, &opts);
	अगर (prefix && स_बदलो(prefix))
		die(_("Cannot come back to cwd"));

	अगर (git_env_bool(GIT_TEST_NO_WRITE_REV_सूचकांक, 0))
		rev_सूचकांक = 0;
	अन्यथा
		rev_सूचकांक = !!(opts.flags & (WRITE_REV_VERIFY | WRITE_REV));

	क्रम (i = 1; i < argc; i++) {
		स्थिर अक्षर *arg = argv[i];

		अगर (*arg == '-') {
			अगर (!म_भेद(arg, "--stdin")) {
				from_stdin = 1;
			} अन्यथा अगर (!म_भेद(arg, "--fix-thin")) {
				fix_thin_pack = 1;
			} अन्यथा अगर (skip_to_optional_arg(arg, "--strict", &arg)) {
				strict = 1;
				do_fsck_object = 1;
				fsck_set_संदेश_types(&fsck_options, arg);
			} अन्यथा अगर (!म_भेद(arg, "--check-self-contained-and-connected")) {
				strict = 1;
				check_self_contained_and_connected = 1;
			} अन्यथा अगर (skip_to_optional_arg(arg, "--fsck-objects", &arg)) {
				do_fsck_object = 1;
				fsck_set_संदेश_types(&fsck_options, arg);
			} अन्यथा अगर (!म_भेद(arg, "--verify")) {
				verify = 1;
			} अन्यथा अगर (!म_भेद(arg, "--verify-stat")) {
				verify = 1;
				show_stat = 1;
			} अन्यथा अगर (!म_भेद(arg, "--verify-stat-only")) {
				verify = 1;
				show_stat = 1;
				stat_only = 1;
			} अन्यथा अगर (skip_to_optional_arg(arg, "--keep", &keep_संदेश)) {
				; /* nothing to do */
			} अन्यथा अगर (skip_to_optional_arg(arg, "--promisor", &promisor_संदेश)) {
				record_outgoing_links = 1;
			} अन्यथा अगर (starts_with(arg, "--threads=")) {
				अक्षर *end;
				nr_threads = म_से_अदीर्घ(arg+10, &end, 0);
				अगर (!arg[10] || *end || nr_threads < 0)
					usage(सूचकांक_pack_usage);
				अगर (!HAVE_THREADS && nr_threads != 1) {
					warning(_("no threads support, ignoring %s"), arg);
					nr_threads = 1;
				}
			} अन्यथा अगर (skip_prefix(arg, "--pack_header=", &arg)) {
				अगर (parse_pack_header_option(arg,
							     निवेश_बफर,
							     &निवेश_len) < 0)
					die(_("bad --pack_header: %s"), arg);
			} अन्यथा अगर (!म_भेद(arg, "-v")) {
				verbose = 1;
			} अन्यथा अगर (!म_भेद(arg, "--progress-title")) {
				अगर (progress_title || (i+1) >= argc)
					usage(सूचकांक_pack_usage);
				progress_title = argv[++i];
			} अन्यथा अगर (!म_भेद(arg, "--show-resolving-progress")) {
				show_resolving_progress = 1;
			} अन्यथा अगर (!म_भेद(arg, "--report-end-of-input")) {
				report_end_of_निवेश = 1;
			} अन्यथा अगर (!म_भेद(arg, "-o")) {
				अगर (सूचकांक_name || (i+1) >= argc)
					usage(सूचकांक_pack_usage);
				सूचकांक_name = argv[++i];
			} अन्यथा अगर (starts_with(arg, "--index-version=")) {
				अक्षर *c;
				opts.version = म_से_अदीर्घ(arg + 16, &c, 10);
				अगर (opts.version > 2)
					die(_("bad %s"), arg);
				अगर (*c == ',')
					opts.off32_limit = म_से_अदीर्घ(c+1, &c, 0);
				अगर (*c || opts.off32_limit & 0x80000000)
					die(_("bad %s"), arg);
			} अन्यथा अगर (skip_prefix(arg, "--max-input-size=", &arg)) {
				max_निवेश_आकार = strtoumax(arg, शून्य, 10);
			} अन्यथा अगर (skip_prefix(arg, "--object-format=", &arg)) {
				hash_algo = hash_algo_by_name(arg);
				अगर (hash_algo == GIT_HASH_UNKNOWN)
					die(_("unknown hash algorithm '%s'"), arg);
				repo_set_hash_algo(the_repository, hash_algo);
			} अन्यथा अगर (!म_भेद(arg, "--rev-index")) {
				rev_सूचकांक = 1;
			} अन्यथा अगर (!म_भेद(arg, "--no-rev-index")) {
				rev_सूचकांक = 0;
			} अन्यथा
				usage(सूचकांक_pack_usage);
			जारी;
		}

		अगर (pack_name)
			usage(सूचकांक_pack_usage);
		pack_name = arg;
	}

	अगर (!pack_name && !from_stdin)
		usage(सूचकांक_pack_usage);
	अगर (fix_thin_pack && !from_stdin)
		die(_("the option '%s' requires '%s'"), "--fix-thin", "--stdin");
	अगर (promisor_संदेश && pack_name)
		die(_("--promisor cannot be used with a pack name"));
	अगर (from_stdin && !startup_info->have_repository)
		die(_("--stdin requires a git repository"));
	अगर (from_stdin && hash_algo)
		die(_("options '%s' and '%s' cannot be used together"), "--object-format", "--stdin");
	अगर (!सूचकांक_name && pack_name)
		सूचकांक_name = derive_filename(pack_name, "pack", "idx", &सूचकांक_name_buf);

	/*
	 * Packfiles and indices do not carry enough information to be able to
	 * identify their object hash. So when we are neither in a repository
	 * nor has the user told us which object hash to use we have no other
	 * choice but to guess the object hash.
	 */
	अगर (!the_repository->hash_algo)
		repo_set_hash_algo(the_repository, GIT_HASH_DEFAULT);

	opts.flags &= ~(WRITE_REV | WRITE_REV_VERIFY);
	अगर (rev_सूचकांक) {
		opts.flags |= verify ? WRITE_REV_VERIFY : WRITE_REV;
		अगर (सूचकांक_name)
			rev_सूचकांक_name = derive_filename(सूचकांक_name,
							 "idx", "rev",
							 &rev_सूचकांक_name_buf);
	}

	अगर (verify) {
		अगर (!सूचकांक_name)
			die(_("--verify with no packfile name given"));
		read_idx_option(&opts, सूचकांक_name);
		opts.flags |= WRITE_IDX_VERIFY | WRITE_IDX_STRICT;
	}
	अगर (strict)
		opts.flags |= WRITE_IDX_STRICT;

	अगर (HAVE_THREADS && !nr_threads) {
		nr_threads = online_cpus();
		/*
		 * Experiments show that going above 20 threads doesn't help,
		 * no matter how many cores you have. Below that, we tend to
		 * max at half the number of online_cpus(), presumably because
		 * half of those are hyperthreads rather than full cores. We'll
		 * never reduce the level below "3", though, to match a
		 * historical value that nobody complained about.
		 */
		अगर (nr_threads < 4)
			; /* too few cores to consider capping */
		अन्यथा अगर (nr_threads < 6)
			nr_threads = 3; /* historic cap */
		अन्यथा अगर (nr_threads < 40)
			nr_threads /= 2;
		अन्यथा
			nr_threads = 20; /* hard cap */
	}

	curr_pack = open_pack_file(pack_name);
	parse_pack_header();
	CALLOC_ARRAY(objects, st_add(nr_objects, 1));
	अगर (show_stat)
		CALLOC_ARRAY(obj_stat, st_add(nr_objects, 1));
	CALLOC_ARRAY(ofs_deltas, nr_objects);
	parse_pack_objects(pack_hash);
	अगर (report_end_of_निवेश)
		write_in_full(2, "\0", 1);
	resolve_deltas(&opts);
	conclude_pack(fix_thin_pack, curr_pack, pack_hash);
	मुक्त(ofs_deltas);
	मुक्त(ref_deltas);
	अगर (strict)
		foreign_nr = check_objects();

	अगर (show_stat)
		show_pack_info(stat_only);

	ALLOC_ARRAY(idx_objects, nr_objects);
	क्रम (i = 0; i < nr_objects; i++)
		idx_objects[i] = &objects[i].idx;
	curr_सूचकांक = write_idx_file(the_repository, सूचकांक_name, idx_objects,
				    nr_objects, &opts, pack_hash);
	अगर (rev_सूचकांक)
		curr_rev_सूचकांक = write_rev_file(the_repository, rev_सूचकांक_name,
						idx_objects, nr_objects,
						pack_hash, opts.flags);
	मुक्त(idx_objects);

	अगर (!verify)
		final(pack_name, curr_pack,
		      सूचकांक_name, curr_सूचकांक,
		      rev_सूचकांक_name, curr_rev_सूचकांक,
		      keep_संदेश, promisor_संदेश,
		      pack_hash);
	अन्यथा
		बंद(निवेश_fd);

	अगर (do_fsck_object) {
		/*
		 * We cannot perform queued consistency checks when running
		 * outside of a repository because those require us to read
		 * from the object database, which is uninitialized.
		 *
		 * TODO: we may eventually set up an in-memory object database,
		 * which would allow us to perform these queued checks.
		 */
		अगर (!startup_info->have_repository &&
		    fsck_has_queued_checks(&fsck_options))
			die(_("cannot perform queued object checks outside "
			      "of a repository"));

		अगर (fsck_finish(&fsck_options))
			die(_("fsck error in pack objects"));
	}

	मुक्त(opts.anomaly);
	मुक्त(objects);
	strbuf_release(&सूचकांक_name_buf);
	strbuf_release(&rev_सूचकांक_name_buf);
	अगर (!pack_name)
		मुक्त((व्योम *) curr_pack);
	अगर (!सूचकांक_name)
		मुक्त((व्योम *) curr_सूचकांक);
	मुक्त(curr_rev_सूचकांक);

	repack_local_links();

	/*
	 * Let the caller know this pack is not self contained
	 */
	अगर (check_self_contained_and_connected && foreign_nr)
		वापस 1;

	वापस 0;
}
