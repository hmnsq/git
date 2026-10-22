#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ "environment.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "git-zlib.h"
#അന്തര്ഭാവ "hex.h"
#അന്തര്ഭാവ "object-file.h"
#അന്തര്ഭാവ "odb.h"
#അന്തര്ഭാവ "odb/streaming.h"
#അന്തര്ഭാവ "odb/transaction.h"
#അന്തര്ഭാവ "object.h"
#അന്തര്ഭാവ "delta.h"
#അന്തര്ഭാവ "pack.h"
#അന്തര്ഭാവ "blob.h"
#അന്തര്ഭാവ "replace-object.h"
#അന്തര്ഭാവ "strbuf.h"
#അന്തര്ഭാവ "progress.h"
#അന്തര്ഭാവ "decorate.h"
#അന്തര്ഭാവ "fsck.h"
#അന്തര്ഭാവ "packfile.h"

സ്ഥിതം പൂർണ്ണസംഖ്യ dry_run, quiet, recover, has_errors, strict;
സ്ഥിതം സ്ഥിരം അക്ഷരം unpack_usage[] = "git unpack-objects [-n] [-q] [-r] [--strict]";

സ്ഥിതം unsigned അക്ഷരം buffer[DEFAULT_IO_BUFFER_SIZE];
സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ offset, len;
സ്ഥിതം off_t consumed_bytes;
സ്ഥിതം off_t max_നിവേശം_size;
സ്ഥിതം ഘടന git_hash_ctx ctx;
സ്ഥിതം ഘടന fsck_options fsck_options;
സ്ഥിതം ഘടന progress *progress;

/*
 * When running under --strict mode, objects whose reachability are
 * suspect are kept in core without getting written in the object
 * store.
 */
ഘടന obj_buffer {
	അക്ഷരം *buffer;
	unsigned long size;
};

സ്ഥിതം ഘടന decoration obj_decorate;

സ്ഥിതം ഘടന obj_buffer *lookup_object_buffer(ഘടന object *base)
{
	തിരികെ lookup_decoration(&obj_decorate, base);
}

സ്ഥിതം ശൂന്യം add_object_buffer(ഘടന object *object, അക്ഷരം *buffer, unsigned long size)
{
	ഘടന obj_buffer *obj;
	CALLOC_ARRAY(obj, 1);
	obj->buffer = buffer;
	obj->size = size;
	എങ്കിൽ (add_decoration(&obj_decorate, object, obj))
		die("object %s tried to add buffer twice!", oid_to_hex(&object->oid));
}

/*
 * Make sure at least "min" bytes are available in the buffer, and
 * return the pointer to the buffer.
 */
സ്ഥിതം ശൂന്യം *fill(പൂർണ്ണസംഖ്യ min)
{
	എങ്കിൽ (min <= len)
		തിരികെ buffer + offset;
	എങ്കിൽ (min > വലുപ്പം(buffer))
		die("cannot fill %d bytes", min);
	എങ്കിൽ (offset) {
		git_hash_update(&ctx, buffer, offset);
		memmove(buffer, buffer + offset, len);
		offset = 0;
	}
	ചെയ്യുക {
		ssize_t ret = xread(0, buffer + len, വലുപ്പം(buffer) - len);
		എങ്കിൽ (ret <= 0) {
			എങ്കിൽ (!ret)
				die("early EOF");
			die_errno("read error on input");
		}
		len += ret;
	} ആകുമ്പോൾ (len < min);
	തിരികെ buffer;
}

സ്ഥിതം ശൂന്യം use(പൂർണ്ണസംഖ്യ bytes)
{
	എങ്കിൽ (bytes > len)
		die("used more bytes than were available");
	len -= bytes;
	offset += bytes;

	/* make sure off_t is sufficiently large not to wrap */
	എങ്കിൽ (signed_add_overflows(consumed_bytes, bytes))
		die("pack too large for current definition of off_t");
	consumed_bytes += bytes;
	എങ്കിൽ (max_നിവേശം_size && consumed_bytes > max_നിവേശം_size)
		die(_("pack exceeds maximum allowed size"));
	display_throughput(progress, consumed_bytes);
}

/*
 * Decompress zstream from the standard input into a newly
 * allocated buffer of specified size and return the buffer.
 * The caller is responsible to free the returned buffer.
 *
 * But for dry_run mode, "get_data()" is only used to check the
 * integrity of data, and the returned buffer is not used at all.
 * Therefore, in dry_run mode, "get_data()" will release the small
 * allocated buffer which is reused to hold temporary zstream output
 * and return NULL instead of returning garbage data.
 */
സ്ഥിതം ശൂന്യം *get_data(unsigned long size)
{
	git_zstream stream;
	unsigned long bufsize = dry_run && size > 8192 ? 8192 : size;
	ശൂന്യം *buf = xmallocz(bufsize);

	memset(&stream, 0, വലുപ്പം(stream));

	stream.next_out = buf;
	stream.avail_out = bufsize;
	stream.next_in = fill(1);
	stream.avail_in = len;
	git_inflate_init(&stream);

	വേണ്ടി (;;) {
		പൂർണ്ണസംഖ്യ ret = git_inflate(&stream, 0);
		use(len - stream.avail_in);
		എങ്കിൽ (stream.total_out == size && ret == Z_STREAM_END)
			നിർത്തുക;
		എങ്കിൽ (ret != Z_OK) {
			error("inflate returned %d", ret);
			FREE_AND_NULL(buf);
			എങ്കിൽ (!recover)
				exit(1);
			has_errors = 1;
			നിർത്തുക;
		}
		stream.next_in = fill(1);
		stream.avail_in = len;
		എങ്കിൽ (dry_run) {
			/* reuse the buffer in dry_run mode */
			stream.next_out = buf;
			stream.avail_out = bufsize > size - stream.total_out ?
						   size - stream.total_out :
						   bufsize;
		}
	}
	git_inflate_end(&stream);
	എങ്കിൽ (dry_run)
		FREE_AND_NULL(buf);
	തിരികെ buf;
}

ഘടന delta_info {
	ഘടന object_id base_oid;
	unsigned nr;
	off_t base_offset;
	unsigned long size;
	ശൂന്യം *delta;
	ഘടന delta_info *next;
};

സ്ഥിതം ഘടന delta_info *delta_list;

സ്ഥിതം ശൂന്യം add_delta_to_list(unsigned nr, സ്ഥിരം ഘടന object_id *base_oid,
			      off_t base_offset,
			      ശൂന്യം *delta, unsigned long size)
{
	ഘടന delta_info *info = xmalloc(വലുപ്പം(*info));

	oidcpy(&info->base_oid, base_oid);
	info->base_offset = base_offset;
	info->size = size;
	info->delta = delta;
	info->nr = nr;
	info->next = delta_list;
	delta_list = info;
}

ഘടന obj_info {
	off_t offset;
	ഘടന object_id oid;
	ഘടന object *obj;
};

/* Remember to update object flag allocation in object.h */
#define FLAG_OPEN (1u<<20)
#define FLAG_WRITTEN (1u<<21)

സ്ഥിതം ഘടന obj_info *obj_list;
സ്ഥിതം unsigned nr_objects;

/*
 * Called only from check_object() after it verified this object
 * is Ok.
 */
സ്ഥിതം ശൂന്യം write_cached_object(ഘടന object *obj, ഘടന obj_buffer *obj_buf)
{
	ഘടന object_id oid;

	എങ്കിൽ (odb_write_object(the_repository->objects, obj_buf->buffer, obj_buf->size,
			     obj->type, &oid) < 0)
		die("failed to write object %s", oid_to_hex(&obj->oid));
	obj->flags |= FLAG_WRITTEN;
}

/*
 * At the very end of the processing, write_rest() scans the objects
 * that have reachability requirements and calls this function.
 * Verify its reachability and validity recursively and write it out.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ check_object(ഘടന object *obj, എണ്ണൽ object_type type,
			ശൂന്യം *data UNUSED,
			ഘടന fsck_options *options UNUSED)
{
	ഘടന obj_buffer *obj_buf;

	എങ്കിൽ (!obj)
		തിരികെ 1;

	എങ്കിൽ (obj->flags & FLAG_WRITTEN)
		തിരികെ 0;

	എങ്കിൽ (type != OBJ_ANY && obj->type != type)
		die("object type mismatch");

	എങ്കിൽ (!(obj->flags & FLAG_OPEN)) {
		size_t size;
		പൂർണ്ണസംഖ്യ type = odb_read_object_info(the_repository->objects, &obj->oid, &size);
		എങ്കിൽ (type != obj->type || type <= 0)
			die("object of unexpected type");
		obj->flags |= FLAG_WRITTEN;
		തിരികെ 0;
	}

	obj_buf = lookup_object_buffer(obj);
	എങ്കിൽ (!obj_buf)
		die("Whoops! Cannot find object '%s'", oid_to_hex(&obj->oid));
	എങ്കിൽ (fsck_object(obj, obj_buf->buffer, obj_buf->size, &fsck_options))
		die("fsck error in packed object");
	fsck_options.walk = check_object;
	എങ്കിൽ (fsck_walk(obj, ശൂന്യ, &fsck_options))
		die("Error on reachable objects of %s", oid_to_hex(&obj->oid));
	write_cached_object(obj, obj_buf);
	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം write_rest(ശൂന്യം)
{
	unsigned i;
	വേണ്ടി (i = 0; i < nr_objects; i++) {
		എങ്കിൽ (obj_list[i].obj)
			check_object(obj_list[i].obj, OBJ_ANY, ശൂന്യ, ശൂന്യ);
	}
}

സ്ഥിതം ശൂന്യം added_object(unsigned nr, എണ്ണൽ object_type type,
			 ശൂന്യം *data, unsigned long size);

/*
 * Write out nr-th object from the list, now we know the contents
 * of it.  Under --strict, this buffers structured objects in-core,
 * to be checked at the end.
 */
സ്ഥിതം ശൂന്യം write_object(unsigned nr, എണ്ണൽ object_type type,
			 ശൂന്യം *buf, unsigned long size)
{
	എങ്കിൽ (!strict) {
		എങ്കിൽ (odb_write_object(the_repository->objects, buf, size, type,
				     &obj_list[nr].oid) < 0)
			die("failed to write object");
		added_object(nr, type, buf, size);
		സ്വതന്ത്രം(buf);
		obj_list[nr].obj = ശൂന്യ;
	} അല്ലെങ്കിൽ എങ്കിൽ (type == OBJ_BLOB) {
		ഘടന blob *blob;
		എങ്കിൽ (odb_write_object(the_repository->objects, buf, size, type,
				     &obj_list[nr].oid) < 0)
			die("failed to write object");
		added_object(nr, type, buf, size);
		സ്വതന്ത്രം(buf);

		blob = lookup_blob(the_repository, &obj_list[nr].oid);
		എങ്കിൽ (blob)
			blob->object.flags |= FLAG_WRITTEN;
		അല്ലെങ്കിൽ
			die("invalid blob object");
		obj_list[nr].obj = ശൂന്യ;
	} അല്ലെങ്കിൽ {
		ഘടന object *obj;
		പൂർണ്ണസംഖ്യ eaten;
		hash_object_file(the_hash_algo, buf, size, type,
				 &obj_list[nr].oid);
		added_object(nr, type, buf, size);
		obj = parse_object_buffer(the_repository, &obj_list[nr].oid,
					  type, size, buf,
					  &eaten);
		എങ്കിൽ (!obj)
			die("invalid %s", type_name(type));
		add_object_buffer(obj, buf, size);
		obj->flags |= FLAG_OPEN;
		obj_list[nr].obj = obj;
	}
}

സ്ഥിതം ശൂന്യം resolve_delta(unsigned nr, എണ്ണൽ object_type type,
			  ശൂന്യം *base, unsigned long base_size,
			  ശൂന്യം *delta, unsigned long delta_size)
{
	ശൂന്യം *result;
	size_t result_size;

	result = patch_delta(base, base_size,
			     delta, delta_size,
			     &result_size);
	എങ്കിൽ (!result)
		die("failed to apply delta");
	സ്വതന്ത്രം(delta);
	write_object(nr, type, result, result_size);
}

/*
 * We now know the contents of an object (which is nr-th in the pack);
 * resolve all the deltified objects that are based on it.
 */
സ്ഥിതം ശൂന്യം added_object(unsigned nr, എണ്ണൽ object_type type,
			 ശൂന്യം *data, unsigned long size)
{
	ഘടന delta_info **p = &delta_list;
	ഘടന delta_info *info;

	ആകുമ്പോൾ ((info = *p) != ശൂന്യ) {
		എങ്കിൽ (oideq(&info->base_oid, &obj_list[nr].oid) ||
		    info->base_offset == obj_list[nr].offset) {
			*p = info->next;
			p = &delta_list;
			resolve_delta(info->nr, type, data, size,
				      info->delta, info->size);
			സ്വതന്ത്രം(info);
			തുടരുക;
		}
		p = &info->next;
	}
}

സ്ഥിതം ശൂന്യം unpack_non_delta_entry(എണ്ണൽ object_type type, unsigned long size,
				   unsigned nr)
{
	ശൂന്യം *buf = get_data(size);

	എങ്കിൽ (buf)
		write_object(nr, type, buf, size);
}

ഘടന zlib_stream {
	ഘടന odb_stream base;
	git_zstream *zstream;
	പൂർണ്ണസംഖ്യ status;
};

സ്ഥിതം ssize_t zlib_stream_read(ഘടന odb_stream *in_stream,
				അക്ഷരം *buf, size_t buf_len)
{
	ഘടന zlib_stream *data = container_of(in_stream, ഘടന zlib_stream, base);
	git_zstream *zstream = data->zstream;

	എങ്കിൽ (data->status != Z_OK)
		തിരികെ 0;

	zstream->next_out = (unsigned അക്ഷരം *) buf;
	zstream->avail_out = buf_len;

	ആകുമ്പോൾ (data->status == Z_OK && zstream->avail_out == buf_len) {
		zstream->next_in = fill(1);
		zstream->avail_in = len;
		data->status = git_inflate(zstream, 0);
		use(len - zstream->avail_in);
	}

	തിരികെ buf_len - zstream->avail_out;
}

സ്ഥിതം ശൂന്യം stream_blob(unsigned long size, unsigned nr)
{
	git_zstream zstream = { 0 };
	ഘടന zlib_stream in_stream = {
		.base = {
			.പഠ = zlib_stream_read,
			.size = size,
			.type = OBJ_BLOB,
		},
		.zstream = &zstream,
		.status = Z_OK,
	};
	ഘടന obj_info *info = &obj_list[nr];

	git_inflate_init(&zstream);

	എങ്കിൽ (odb_write_object_stream(the_repository->objects, &in_stream.base, &info->oid))
		die(_("failed to write object in stream"));

	എങ്കിൽ (in_stream.status != Z_STREAM_END)
		die(_("inflate returned (%d)"), in_stream.status);
	git_inflate_end(&zstream);

	എങ്കിൽ (strict) {
		ഘടന blob *blob = lookup_blob(the_repository, &info->oid);

		എങ്കിൽ (!blob)
			die(_("invalid blob object from stream"));
		blob->object.flags |= FLAG_WRITTEN;
	}
	info->obj = ശൂന്യ;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ resolve_against_held(unsigned nr, സ്ഥിരം ഘടന object_id *base,
				ശൂന്യം *delta_data, unsigned long delta_size)
{
	ഘടന object *obj;
	ഘടന obj_buffer *obj_buffer;
	obj = lookup_object(the_repository, base);
	എങ്കിൽ (!obj)
		തിരികെ 0;
	obj_buffer = lookup_object_buffer(obj);
	എങ്കിൽ (!obj_buffer)
		തിരികെ 0;
	resolve_delta(nr, obj->type, obj_buffer->buffer,
		      obj_buffer->size, delta_data, delta_size);
	തിരികെ 1;
}

സ്ഥിതം ശൂന്യം unpack_delta_entry(എണ്ണൽ object_type type, unsigned long delta_size,
			       unsigned nr)
{
	ശൂന്യം *delta_data, *base;
	unsigned long base_size;
	size_t base_size_st = 0;
	ഘടന object_id base_oid;

	എങ്കിൽ (type == OBJ_REF_DELTA) {
		oidread(&base_oid, fill(the_hash_algo->rawsz), the_repository->hash_algo);
		use(the_hash_algo->rawsz);
		delta_data = get_data(delta_size);
		എങ്കിൽ (!delta_data)
			തിരികെ;
		എങ്കിൽ (odb_has_object(the_repository->objects, &base_oid,
				   ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR))
			; /* Ok we have this one */
		അല്ലെങ്കിൽ എങ്കിൽ (resolve_against_held(nr, &base_oid,
					      delta_data, delta_size))
			തിരികെ; /* we are done */
		അല്ലെങ്കിൽ {
			/* cannot resolve yet --- queue it */
			oidclr(&obj_list[nr].oid, the_repository->hash_algo);
			add_delta_to_list(nr, &base_oid, 0, delta_data, delta_size);
			തിരികെ;
		}
	} അല്ലെങ്കിൽ {
		unsigned base_found = 0;
		unsigned അക്ഷരം *pack, c;
		off_t base_offset;
		unsigned lo, mid, hi;

		pack = fill(1);
		c = *pack;
		use(1);
		base_offset = c & 127;
		ആകുമ്പോൾ (c & 128) {
			base_offset += 1;
			എങ്കിൽ (!base_offset || MSB(base_offset, 7))
				die("offset value overflow for delta base object");
			pack = fill(1);
			c = *pack;
			use(1);
			base_offset = (base_offset << 7) + (c & 127);
		}
		base_offset = obj_list[nr].offset - base_offset;
		എങ്കിൽ (base_offset <= 0 || base_offset >= obj_list[nr].offset)
			die("offset value out of bound for delta base object");

		delta_data = get_data(delta_size);
		എങ്കിൽ (!delta_data)
			തിരികെ;
		lo = 0;
		hi = nr;
		ആകുമ്പോൾ (lo < hi) {
			mid = lo + (hi - lo) / 2;
			എങ്കിൽ (base_offset < obj_list[mid].offset) {
				hi = mid;
			} അല്ലെങ്കിൽ എങ്കിൽ (base_offset > obj_list[mid].offset) {
				lo = mid + 1;
			} അല്ലെങ്കിൽ {
				oidcpy(&base_oid, &obj_list[mid].oid);
				base_found = !is_null_oid(&base_oid);
				നിർത്തുക;
			}
		}
		എങ്കിൽ (!base_found) {
			/*
			 * The delta base object is itself a delta that
			 * has not been resolved yet.
			 */
			oidclr(&obj_list[nr].oid, the_repository->hash_algo);
			add_delta_to_list(nr, null_oid(the_hash_algo), base_offset,
					  delta_data, delta_size);
			തിരികെ;
		}
	}

	എങ്കിൽ (resolve_against_held(nr, &base_oid, delta_data, delta_size))
		തിരികെ;

	base = odb_read_object(the_repository->objects, &base_oid,
			       &type, &base_size_st);
	base_size = cast_size_t_to_ulong(base_size_st);
	എങ്കിൽ (!base) {
		error("failed to read delta-pack base object %s",
		      oid_to_hex(&base_oid));
		എങ്കിൽ (!recover)
			exit(1);
		has_errors = 1;
		തിരികെ;
	}
	resolve_delta(nr, type, base, base_size, delta_data, delta_size);
	സ്വതന്ത്രം(base);
}

സ്ഥിതം ശൂന്യം unpack_one(unsigned nr)
{
	unsigned shift;
	unsigned അക്ഷരം *pack;
	size_t size, c;
	എണ്ണൽ object_type type;

	obj_list[nr].offset = consumed_bytes;

	pack = fill(1);
	c = *pack;
	use(1);
	type = (c >> 4) & 7;
	size = (c & 15);
	shift = 4;
	ആകുമ്പോൾ (c & 0x80) {
		എങ്കിൽ ((bitsizeof(size_t) - 7) < shift)
			die(_("object size too large for this platform"));
		pack = fill(1);
		c = *pack;
		use(1);
		size += (c & 0x7f) << shift;
		shift += 7;
	}

	തിരഞ്ഞെടുപ്പ് (type) {
	അവസ്ഥ OBJ_BLOB:
		എങ്കിൽ (!dry_run &&
		    size > repo_settings_get_big_file_threshold(the_repository)) {
			stream_blob(size, nr);
			തിരികെ;
		}
		/* fallthrough */
	അവസ്ഥ OBJ_COMMIT:
	അവസ്ഥ OBJ_വൃക്ഷം:
	അവസ്ഥ OBJ_TAG:
		unpack_non_delta_entry(type, size, nr);
		തിരികെ;
	അവസ്ഥ OBJ_REF_DELTA:
	അവസ്ഥ OBJ_OFS_DELTA:
		unpack_delta_entry(type, size, nr);
		തിരികെ;
	സ്ഥിരസ്ഥിതി:
		error("bad object type %d", type);
		has_errors = 1;
		എങ്കിൽ (recover)
			തിരികെ;
		exit(1);
	}
}

സ്ഥിതം ശൂന്യം unpack_all(ശൂന്യം)
{
	പൂർണ്ണസംഖ്യ i;
	unsigned അക്ഷരം *hdr = fill(വലുപ്പം(ഘടന pack_header));
	ഘടന odb_transaction *transaction;

	എങ്കിൽ (get_be32(hdr) != PACK_SIGNATURE)
		die("bad pack file");
	hdr += 4;
	എങ്കിൽ (!pack_version_ok_native(get_be32(hdr)))
		die("unknown pack file version %"PRIu32,
		    get_be32(hdr));
	hdr += 4;
	nr_objects = get_be32(hdr);
	use(വലുപ്പം(ഘടന pack_header));

	എങ്കിൽ (!quiet)
		progress = start_progress(the_repository,
					  _("Unpacking objects"), nr_objects);
	CALLOC_ARRAY(obj_list, nr_objects);
	odb_transaction_begin_or_die(the_repository->objects, &transaction, 0);
	വേണ്ടി (i = 0; i < nr_objects; i++) {
		unpack_one(i);
		display_progress(progress, i + 1);
	}
	odb_transaction_commit_and_finalize_or_die(transaction);
	stop_progress(&progress);

	എങ്കിൽ (delta_list)
		die("unresolved deltas left after unpacking");
}

പൂർണ്ണസംഖ്യ cmd_unpack_objects(പൂർണ്ണസംഖ്യ argc,
		       സ്ഥിരം അക്ഷരം **argv,
		       സ്ഥിരം അക്ഷരം *prefix UNUSED,
		       ഘടന repository *repo)
{
	പൂർണ്ണസംഖ്യ i;
	ഘടന object_id oid;
	ഘടന git_hash_ctx tmp_ctx;

	disable_replace_refs();

	repo_config(the_repository, git_default_config, ശൂന്യ);

	quiet = !isatty(2);

	show_usage_if_asked(argc, argv, unpack_usage);

	fsck_options_init(&fsck_options, repo, FSCK_OPTIONS_STRICT);

	വേണ്ടി (i = 1 ; i < argc; i++) {
		സ്ഥിരം അക്ഷരം *arg = argv[i];

		എങ്കിൽ (*arg == '-') {
			എങ്കിൽ (!strcmp(arg, "-n")) {
				dry_run = 1;
				തുടരുക;
			}
			എങ്കിൽ (!strcmp(arg, "-q")) {
				quiet = 1;
				തുടരുക;
			}
			എങ്കിൽ (!strcmp(arg, "-r")) {
				recover = 1;
				തുടരുക;
			}
			എങ്കിൽ (!strcmp(arg, "--strict")) {
				strict = 1;
				തുടരുക;
			}
			എങ്കിൽ (skip_prefix(arg, "--strict=", &arg)) {
				strict = 1;
				fsck_set_സന്ദേശം_types(&fsck_options, arg);
				തുടരുക;
			}
			എങ്കിൽ (skip_prefix(arg, "--pack_header=", &arg)) {
				എങ്കിൽ (parse_pack_header_option(arg,
							     buffer, &len) < 0)
					die(_("bad --pack_header: %s"), arg);
				തുടരുക;
			}
			എങ്കിൽ (skip_prefix(arg, "--max-input-size=", &arg)) {
				max_നിവേശം_size = strtoumax(arg, ശൂന്യ, 10);
				തുടരുക;
			}
			usage(unpack_usage);
		}

		/* We don't take any non-flag arguments now.. Maybe some day */
		usage(unpack_usage);
	}
	git_hash_init(&ctx, the_hash_algo);
	unpack_all();
	git_hash_update(&ctx, buffer, offset);
	git_hash_init(&tmp_ctx, the_hash_algo);
	git_hash_clone(&tmp_ctx, &ctx);
	git_hash_final_oid(&oid, &tmp_ctx);
	എങ്കിൽ (strict) {
		write_rest();
		എങ്കിൽ (fsck_finish(&fsck_options))
			die(_("fsck error in pack objects"));
	}
	എങ്കിൽ (!hasheq(fill(the_hash_algo->rawsz), oid.hash,
		    the_repository->hash_algo))
		die("final sha1 did not match");
	use(the_hash_algo->rawsz);

	/* Write the last part of the buffer to stdout */
	write_in_full(1, buffer + offset, len);

	/* All done */
	തിരികെ has_errors;
}
