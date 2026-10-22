#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "environment.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "git-zlib.h"
#அந்தர்பாவ "hex.h"
#அந்தர்பாவ "object-file.h"
#அந்தர்பாவ "odb.h"
#அந்தர்பாவ "odb/streaming.h"
#அந்தர்பாவ "odb/transaction.h"
#அந்தர்பாவ "object.h"
#அந்தர்பாவ "delta.h"
#அந்தர்பாவ "pack.h"
#அந்தர்பாவ "blob.h"
#அந்தர்பாவ "replace-object.h"
#அந்தர்பாவ "strbuf.h"
#அந்தர்பாவ "progress.h"
#அந்தர்பாவ "decorate.h"
#அந்தர்பாவ "fsck.h"
#அந்தர்பாவ "packfile.h"

நிலையான முழு dry_run, quiet, recover, has_errors, strict;
நிலையான மாறிலி எழுத்து unpack_usage[] = "git unpack-objects [-n] [-q] [-r] [--strict]";

நிலையான unsigned எழுத்து buffer[DEFAULT_IO_BUFFER_SIZE];
நிலையான unsigned முழு offset, len;
நிலையான off_t consumed_bytes;
நிலையான off_t max_உள்ளீடு_size;
நிலையான கட்டமைப்பு git_hash_ctx ctx;
நிலையான கட்டமைப்பு fsck_options fsck_options;
நிலையான கட்டமைப்பு progress *progress;

/*
 * When running under --strict mode, objects whose reachability are
 * suspect are kept in core without getting written in the object
 * store.
 */
கட்டமைப்பு obj_buffer {
	எழுத்து *buffer;
	unsigned long size;
};

நிலையான கட்டமைப்பு decoration obj_decorate;

நிலையான கட்டமைப்பு obj_buffer *lookup_object_buffer(கட்டமைப்பு object *base)
{
	திரும்பு lookup_decoration(&obj_decorate, base);
}

நிலையான வெறுமை add_object_buffer(கட்டமைப்பு object *object, எழுத்து *buffer, unsigned long size)
{
	கட்டமைப்பு obj_buffer *obj;
	CALLOC_ARRAY(obj, 1);
	obj->buffer = buffer;
	obj->size = size;
	எனில் (add_decoration(&obj_decorate, object, obj))
		die("object %s tried to add buffer twice!", oid_to_hex(&object->oid));
}

/*
 * Make sure at least "min" bytes are available in the buffer, and
 * return the pointer to the buffer.
 */
நிலையான வெறுமை *fill(முழு min)
{
	எனில் (min <= len)
		திரும்பு buffer + offset;
	எனில் (min > அளவு(buffer))
		die("cannot fill %d bytes", min);
	எனில் (offset) {
		git_hash_update(&ctx, buffer, offset);
		memmove(buffer, buffer + offset, len);
		offset = 0;
	}
	செய் {
		ssize_t ret = xread(0, buffer + len, அளவு(buffer) - len);
		எனில் (ret <= 0) {
			எனில் (!ret)
				die("early EOF");
			die_errno("read error on input");
		}
		len += ret;
	} போது (len < min);
	திரும்பு buffer;
}

நிலையான வெறுமை use(முழு bytes)
{
	எனில் (bytes > len)
		die("used more bytes than were available");
	len -= bytes;
	offset += bytes;

	/* make sure off_t is sufficiently large not to wrap */
	எனில் (signed_add_overflows(consumed_bytes, bytes))
		die("pack too large for current definition of off_t");
	consumed_bytes += bytes;
	எனில் (max_உள்ளீடு_size && consumed_bytes > max_உள்ளீடு_size)
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
நிலையான வெறுமை *get_data(unsigned long size)
{
	git_zstream stream;
	unsigned long bufsize = dry_run && size > 8192 ? 8192 : size;
	வெறுமை *buf = xmallocz(bufsize);

	memset(&stream, 0, அளவு(stream));

	stream.next_out = buf;
	stream.avail_out = bufsize;
	stream.next_in = fill(1);
	stream.avail_in = len;
	git_inflate_init(&stream);

	க்கு (;;) {
		முழு ret = git_inflate(&stream, 0);
		use(len - stream.avail_in);
		எனில் (stream.total_out == size && ret == Z_STREAM_END)
			நிறுத்து;
		எனில் (ret != Z_OK) {
			error("inflate returned %d", ret);
			FREE_AND_NULL(buf);
			எனில் (!recover)
				exit(1);
			has_errors = 1;
			நிறுத்து;
		}
		stream.next_in = fill(1);
		stream.avail_in = len;
		எனில் (dry_run) {
			/* reuse the buffer in dry_run mode */
			stream.next_out = buf;
			stream.avail_out = bufsize > size - stream.total_out ?
						   size - stream.total_out :
						   bufsize;
		}
	}
	git_inflate_end(&stream);
	எனில் (dry_run)
		FREE_AND_NULL(buf);
	திரும்பு buf;
}

கட்டமைப்பு delta_info {
	கட்டமைப்பு object_id base_oid;
	unsigned nr;
	off_t base_offset;
	unsigned long size;
	வெறுமை *delta;
	கட்டமைப்பு delta_info *next;
};

நிலையான கட்டமைப்பு delta_info *delta_list;

நிலையான வெறுமை add_delta_to_list(unsigned nr, மாறிலி கட்டமைப்பு object_id *base_oid,
			      off_t base_offset,
			      வெறுமை *delta, unsigned long size)
{
	கட்டமைப்பு delta_info *info = xmalloc(அளவு(*info));

	oidcpy(&info->base_oid, base_oid);
	info->base_offset = base_offset;
	info->size = size;
	info->delta = delta;
	info->nr = nr;
	info->next = delta_list;
	delta_list = info;
}

கட்டமைப்பு obj_info {
	off_t offset;
	கட்டமைப்பு object_id oid;
	கட்டமைப்பு object *obj;
};

/* Remember to update object flag allocation in object.h */
#define FLAG_OPEN (1u<<20)
#define FLAG_WRITTEN (1u<<21)

நிலையான கட்டமைப்பு obj_info *obj_list;
நிலையான unsigned nr_objects;

/*
 * Called only from check_object() after it verified this object
 * is Ok.
 */
நிலையான வெறுமை write_cached_object(கட்டமைப்பு object *obj, கட்டமைப்பு obj_buffer *obj_buf)
{
	கட்டமைப்பு object_id oid;

	எனில் (odb_write_object(the_repository->objects, obj_buf->buffer, obj_buf->size,
			     obj->type, &oid) < 0)
		die("failed to write object %s", oid_to_hex(&obj->oid));
	obj->flags |= FLAG_WRITTEN;
}

/*
 * At the very end of the processing, write_rest() scans the objects
 * that have reachability requirements and calls this function.
 * Verify its reachability and validity recursively and write it out.
 */
நிலையான முழு check_object(கட்டமைப்பு object *obj, எண்ணிக்கை object_type type,
			வெறுமை *data UNUSED,
			கட்டமைப்பு fsck_options *options UNUSED)
{
	கட்டமைப்பு obj_buffer *obj_buf;

	எனில் (!obj)
		திரும்பு 1;

	எனில் (obj->flags & FLAG_WRITTEN)
		திரும்பு 0;

	எனில் (type != OBJ_ANY && obj->type != type)
		die("object type mismatch");

	எனில் (!(obj->flags & FLAG_OPEN)) {
		size_t size;
		முழு type = odb_read_object_info(the_repository->objects, &obj->oid, &size);
		எனில் (type != obj->type || type <= 0)
			die("object of unexpected type");
		obj->flags |= FLAG_WRITTEN;
		திரும்பு 0;
	}

	obj_buf = lookup_object_buffer(obj);
	எனில் (!obj_buf)
		die("Whoops! Cannot find object '%s'", oid_to_hex(&obj->oid));
	எனில் (fsck_object(obj, obj_buf->buffer, obj_buf->size, &fsck_options))
		die("fsck error in packed object");
	fsck_options.walk = check_object;
	எனில் (fsck_walk(obj, வெற்று, &fsck_options))
		die("Error on reachable objects of %s", oid_to_hex(&obj->oid));
	write_cached_object(obj, obj_buf);
	திரும்பு 0;
}

நிலையான வெறுமை write_rest(வெறுமை)
{
	unsigned i;
	க்கு (i = 0; i < nr_objects; i++) {
		எனில் (obj_list[i].obj)
			check_object(obj_list[i].obj, OBJ_ANY, வெற்று, வெற்று);
	}
}

நிலையான வெறுமை added_object(unsigned nr, எண்ணிக்கை object_type type,
			 வெறுமை *data, unsigned long size);

/*
 * Write out nr-th object from the list, now we know the contents
 * of it.  Under --strict, this buffers structured objects in-core,
 * to be checked at the end.
 */
நிலையான வெறுமை write_object(unsigned nr, எண்ணிக்கை object_type type,
			 வெறுமை *buf, unsigned long size)
{
	எனில் (!strict) {
		எனில் (odb_write_object(the_repository->objects, buf, size, type,
				     &obj_list[nr].oid) < 0)
			die("failed to write object");
		added_object(nr, type, buf, size);
		விடுவி(buf);
		obj_list[nr].obj = வெற்று;
	} இல்லையெனில் எனில் (type == OBJ_BLOB) {
		கட்டமைப்பு blob *blob;
		எனில் (odb_write_object(the_repository->objects, buf, size, type,
				     &obj_list[nr].oid) < 0)
			die("failed to write object");
		added_object(nr, type, buf, size);
		விடுவி(buf);

		blob = lookup_blob(the_repository, &obj_list[nr].oid);
		எனில் (blob)
			blob->object.flags |= FLAG_WRITTEN;
		இல்லையெனில்
			die("invalid blob object");
		obj_list[nr].obj = வெற்று;
	} இல்லையெனில் {
		கட்டமைப்பு object *obj;
		முழு eaten;
		hash_object_file(the_hash_algo, buf, size, type,
				 &obj_list[nr].oid);
		added_object(nr, type, buf, size);
		obj = parse_object_buffer(the_repository, &obj_list[nr].oid,
					  type, size, buf,
					  &eaten);
		எனில் (!obj)
			die("invalid %s", type_name(type));
		add_object_buffer(obj, buf, size);
		obj->flags |= FLAG_OPEN;
		obj_list[nr].obj = obj;
	}
}

நிலையான வெறுமை resolve_delta(unsigned nr, எண்ணிக்கை object_type type,
			  வெறுமை *base, unsigned long base_size,
			  வெறுமை *delta, unsigned long delta_size)
{
	வெறுமை *result;
	size_t result_size;

	result = patch_delta(base, base_size,
			     delta, delta_size,
			     &result_size);
	எனில் (!result)
		die("failed to apply delta");
	விடுவி(delta);
	write_object(nr, type, result, result_size);
}

/*
 * We now know the contents of an object (which is nr-th in the pack);
 * resolve all the deltified objects that are based on it.
 */
நிலையான வெறுமை added_object(unsigned nr, எண்ணிக்கை object_type type,
			 வெறுமை *data, unsigned long size)
{
	கட்டமைப்பு delta_info **p = &delta_list;
	கட்டமைப்பு delta_info *info;

	போது ((info = *p) != வெற்று) {
		எனில் (oideq(&info->base_oid, &obj_list[nr].oid) ||
		    info->base_offset == obj_list[nr].offset) {
			*p = info->next;
			p = &delta_list;
			resolve_delta(info->nr, type, data, size,
				      info->delta, info->size);
			விடுவி(info);
			தொடர்;
		}
		p = &info->next;
	}
}

நிலையான வெறுமை unpack_non_delta_entry(எண்ணிக்கை object_type type, unsigned long size,
				   unsigned nr)
{
	வெறுமை *buf = get_data(size);

	எனில் (buf)
		write_object(nr, type, buf, size);
}

கட்டமைப்பு zlib_stream {
	கட்டமைப்பு odb_stream base;
	git_zstream *zstream;
	முழு status;
};

நிலையான ssize_t zlib_stream_read(கட்டமைப்பு odb_stream *in_stream,
				எழுத்து *buf, size_t buf_len)
{
	கட்டமைப்பு zlib_stream *data = container_of(in_stream, கட்டமைப்பு zlib_stream, base);
	git_zstream *zstream = data->zstream;

	எனில் (data->status != Z_OK)
		திரும்பு 0;

	zstream->next_out = (unsigned எழுத்து *) buf;
	zstream->avail_out = buf_len;

	போது (data->status == Z_OK && zstream->avail_out == buf_len) {
		zstream->next_in = fill(1);
		zstream->avail_in = len;
		data->status = git_inflate(zstream, 0);
		use(len - zstream->avail_in);
	}

	திரும்பு buf_len - zstream->avail_out;
}

நிலையான வெறுமை stream_blob(unsigned long size, unsigned nr)
{
	git_zstream zstream = { 0 };
	கட்டமைப்பு zlib_stream in_stream = {
		.base = {
			.பட = zlib_stream_read,
			.size = size,
			.type = OBJ_BLOB,
		},
		.zstream = &zstream,
		.status = Z_OK,
	};
	கட்டமைப்பு obj_info *info = &obj_list[nr];

	git_inflate_init(&zstream);

	எனில் (odb_write_object_stream(the_repository->objects, &in_stream.base, &info->oid))
		die(_("failed to write object in stream"));

	எனில் (in_stream.status != Z_STREAM_END)
		die(_("inflate returned (%d)"), in_stream.status);
	git_inflate_end(&zstream);

	எனில் (strict) {
		கட்டமைப்பு blob *blob = lookup_blob(the_repository, &info->oid);

		எனில் (!blob)
			die(_("invalid blob object from stream"));
		blob->object.flags |= FLAG_WRITTEN;
	}
	info->obj = வெற்று;
}

நிலையான முழு resolve_against_held(unsigned nr, மாறிலி கட்டமைப்பு object_id *base,
				வெறுமை *delta_data, unsigned long delta_size)
{
	கட்டமைப்பு object *obj;
	கட்டமைப்பு obj_buffer *obj_buffer;
	obj = lookup_object(the_repository, base);
	எனில் (!obj)
		திரும்பு 0;
	obj_buffer = lookup_object_buffer(obj);
	எனில் (!obj_buffer)
		திரும்பு 0;
	resolve_delta(nr, obj->type, obj_buffer->buffer,
		      obj_buffer->size, delta_data, delta_size);
	திரும்பு 1;
}

நிலையான வெறுமை unpack_delta_entry(எண்ணிக்கை object_type type, unsigned long delta_size,
			       unsigned nr)
{
	வெறுமை *delta_data, *base;
	unsigned long base_size;
	size_t base_size_st = 0;
	கட்டமைப்பு object_id base_oid;

	எனில் (type == OBJ_REF_DELTA) {
		oidread(&base_oid, fill(the_hash_algo->rawsz), the_repository->hash_algo);
		use(the_hash_algo->rawsz);
		delta_data = get_data(delta_size);
		எனில் (!delta_data)
			திரும்பு;
		எனில் (odb_has_object(the_repository->objects, &base_oid,
				   ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR))
			; /* Ok we have this one */
		இல்லையெனில் எனில் (resolve_against_held(nr, &base_oid,
					      delta_data, delta_size))
			திரும்பு; /* we are done */
		இல்லையெனில் {
			/* cannot resolve yet --- queue it */
			oidclr(&obj_list[nr].oid, the_repository->hash_algo);
			add_delta_to_list(nr, &base_oid, 0, delta_data, delta_size);
			திரும்பு;
		}
	} இல்லையெனில் {
		unsigned base_found = 0;
		unsigned எழுத்து *pack, c;
		off_t base_offset;
		unsigned lo, mid, hi;

		pack = fill(1);
		c = *pack;
		use(1);
		base_offset = c & 127;
		போது (c & 128) {
			base_offset += 1;
			எனில் (!base_offset || MSB(base_offset, 7))
				die("offset value overflow for delta base object");
			pack = fill(1);
			c = *pack;
			use(1);
			base_offset = (base_offset << 7) + (c & 127);
		}
		base_offset = obj_list[nr].offset - base_offset;
		எனில் (base_offset <= 0 || base_offset >= obj_list[nr].offset)
			die("offset value out of bound for delta base object");

		delta_data = get_data(delta_size);
		எனில் (!delta_data)
			திரும்பு;
		lo = 0;
		hi = nr;
		போது (lo < hi) {
			mid = lo + (hi - lo) / 2;
			எனில் (base_offset < obj_list[mid].offset) {
				hi = mid;
			} இல்லையெனில் எனில் (base_offset > obj_list[mid].offset) {
				lo = mid + 1;
			} இல்லையெனில் {
				oidcpy(&base_oid, &obj_list[mid].oid);
				base_found = !is_null_oid(&base_oid);
				நிறுத்து;
			}
		}
		எனில் (!base_found) {
			/*
			 * The delta base object is itself a delta that
			 * has not been resolved yet.
			 */
			oidclr(&obj_list[nr].oid, the_repository->hash_algo);
			add_delta_to_list(nr, null_oid(the_hash_algo), base_offset,
					  delta_data, delta_size);
			திரும்பு;
		}
	}

	எனில் (resolve_against_held(nr, &base_oid, delta_data, delta_size))
		திரும்பு;

	base = odb_read_object(the_repository->objects, &base_oid,
			       &type, &base_size_st);
	base_size = cast_size_t_to_ulong(base_size_st);
	எனில் (!base) {
		error("failed to read delta-pack base object %s",
		      oid_to_hex(&base_oid));
		எனில் (!recover)
			exit(1);
		has_errors = 1;
		திரும்பு;
	}
	resolve_delta(nr, type, base, base_size, delta_data, delta_size);
	விடுவி(base);
}

நிலையான வெறுமை unpack_one(unsigned nr)
{
	unsigned shift;
	unsigned எழுத்து *pack;
	size_t size, c;
	எண்ணிக்கை object_type type;

	obj_list[nr].offset = consumed_bytes;

	pack = fill(1);
	c = *pack;
	use(1);
	type = (c >> 4) & 7;
	size = (c & 15);
	shift = 4;
	போது (c & 0x80) {
		எனில் ((bitsizeof(size_t) - 7) < shift)
			die(_("object size too large for this platform"));
		pack = fill(1);
		c = *pack;
		use(1);
		size += (c & 0x7f) << shift;
		shift += 7;
	}

	தேர்வு (type) {
	நிலை OBJ_BLOB:
		எனில் (!dry_run &&
		    size > repo_settings_get_big_file_threshold(the_repository)) {
			stream_blob(size, nr);
			திரும்பு;
		}
		/* fallthrough */
	நிலை OBJ_COMMIT:
	நிலை OBJ_மரம்:
	நிலை OBJ_TAG:
		unpack_non_delta_entry(type, size, nr);
		திரும்பு;
	நிலை OBJ_REF_DELTA:
	நிலை OBJ_OFS_DELTA:
		unpack_delta_entry(type, size, nr);
		திரும்பு;
	இயல்பு:
		error("bad object type %d", type);
		has_errors = 1;
		எனில் (recover)
			திரும்பு;
		exit(1);
	}
}

நிலையான வெறுமை unpack_all(வெறுமை)
{
	முழு i;
	unsigned எழுத்து *hdr = fill(அளவு(கட்டமைப்பு pack_header));
	கட்டமைப்பு odb_transaction *transaction;

	எனில் (get_be32(hdr) != PACK_SIGNATURE)
		die("bad pack file");
	hdr += 4;
	எனில் (!pack_version_ok_native(get_be32(hdr)))
		die("unknown pack file version %"PRIu32,
		    get_be32(hdr));
	hdr += 4;
	nr_objects = get_be32(hdr);
	use(அளவு(கட்டமைப்பு pack_header));

	எனில் (!quiet)
		progress = start_progress(the_repository,
					  _("Unpacking objects"), nr_objects);
	CALLOC_ARRAY(obj_list, nr_objects);
	odb_transaction_begin_or_die(the_repository->objects, &transaction, 0);
	க்கு (i = 0; i < nr_objects; i++) {
		unpack_one(i);
		display_progress(progress, i + 1);
	}
	odb_transaction_commit_and_finalize_or_die(transaction);
	stop_progress(&progress);

	எனில் (delta_list)
		die("unresolved deltas left after unpacking");
}

முழு cmd_unpack_objects(முழு argc,
		       மாறிலி எழுத்து **argv,
		       மாறிலி எழுத்து *prefix UNUSED,
		       கட்டமைப்பு repository *repo)
{
	முழு i;
	கட்டமைப்பு object_id oid;
	கட்டமைப்பு git_hash_ctx tmp_ctx;

	disable_replace_refs();

	repo_config(the_repository, git_default_config, வெற்று);

	quiet = !isatty(2);

	show_usage_if_asked(argc, argv, unpack_usage);

	fsck_options_init(&fsck_options, repo, FSCK_OPTIONS_STRICT);

	க்கு (i = 1 ; i < argc; i++) {
		மாறிலி எழுத்து *arg = argv[i];

		எனில் (*arg == '-') {
			எனில் (!strcmp(arg, "-n")) {
				dry_run = 1;
				தொடர்;
			}
			எனில் (!strcmp(arg, "-q")) {
				quiet = 1;
				தொடர்;
			}
			எனில் (!strcmp(arg, "-r")) {
				recover = 1;
				தொடர்;
			}
			எனில் (!strcmp(arg, "--strict")) {
				strict = 1;
				தொடர்;
			}
			எனில் (skip_prefix(arg, "--strict=", &arg)) {
				strict = 1;
				fsck_set_செய்தி_types(&fsck_options, arg);
				தொடர்;
			}
			எனில் (skip_prefix(arg, "--pack_header=", &arg)) {
				எனில் (parse_pack_header_option(arg,
							     buffer, &len) < 0)
					die(_("bad --pack_header: %s"), arg);
				தொடர்;
			}
			எனில் (skip_prefix(arg, "--max-input-size=", &arg)) {
				max_உள்ளீடு_size = strtoumax(arg, வெற்று, 10);
				தொடர்;
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
	எனில் (strict) {
		write_rest();
		எனில் (fsck_finish(&fsck_options))
			die(_("fsck error in pack objects"));
	}
	எனில் (!hasheq(fill(the_hash_algo->rawsz), oid.hash,
		    the_repository->hash_algo))
		die("final sha1 did not match");
	use(the_hash_algo->rawsz);

	/* Write the last part of the buffer to stdout */
	write_in_full(1, buffer + offset, len);

	/* All done */
	திரும்பு has_errors;
}
