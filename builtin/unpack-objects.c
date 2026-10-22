#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#شامل "builtin.h"
#شامل "config.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "git-zlib.h"
#شامل "hex.h"
#شامل "object-file.h"
#شامل "odb.h"
#شامل "odb/streaming.h"
#شامل "odb/transaction.h"
#شامل "object.h"
#شامل "delta.h"
#شامل "pack.h"
#شامل "blob.h"
#شامل "replace-object.h"
#شامل "strbuf.h"
#شامل "progress.h"
#شامل "decorate.h"
#شامل "fsck.h"
#شامل "packfile.h"

ایستا صحیح dry_run, quiet, recover, has_errors, strict;
ایستا ثابت نویسه unpack_usage[] = "git unpack-objects [-n] [-q] [-r] [--strict]";

ایستا unsigned نویسه buffer[DEFAULT_IO_BUFFER_SIZE];
ایستا unsigned صحیح offset, len;
ایستا off_t consumed_bytes;
ایستا off_t max_درون_داد_size;
ایستا ساختار git_hash_ctx ctx;
ایستا ساختار fsck_options fsck_options;
ایستا ساختار progress *progress;

/*
 * When running under --strict mode, objects whose reachability are
 * suspect are kept in core without getting written in the object
 * store.
 */
ساختار obj_buffer {
	نویسه *buffer;
	unsigned long size;
};

ایستا ساختار decoration obj_decorate;

ایستا ساختار obj_buffer *lookup_object_buffer(ساختار object *base)
{
	بازگشت lookup_decoration(&obj_decorate, base);
}

ایستا تهی add_object_buffer(ساختار object *object, نویسه *buffer, unsigned long size)
{
	ساختار obj_buffer *obj;
	CALLOC_ARRAY(obj, 1);
	obj->buffer = buffer;
	obj->size = size;
	اگر (add_decoration(&obj_decorate, object, obj))
		die("object %s tried to add buffer twice!", oid_to_hex(&object->oid));
}

/*
 * Make sure at least "min" bytes are available in the buffer, and
 * return the pointer to the buffer.
 */
ایستا تهی *fill(صحیح min)
{
	اگر (min <= len)
		بازگشت buffer + offset;
	اگر (min > اندازه(buffer))
		die("cannot fill %d bytes", min);
	اگر (offset) {
		git_hash_update(&ctx, buffer, offset);
		memmove(buffer, buffer + offset, len);
		offset = 0;
	}
	انجامبده {
		ssize_t ret = xread(0, buffer + len, اندازه(buffer) - len);
		اگر (ret <= 0) {
			اگر (!ret)
				die("early EOF");
			die_errno("read error on input");
		}
		len += ret;
	} وقتی (len < min);
	بازگشت buffer;
}

ایستا تهی use(صحیح bytes)
{
	اگر (bytes > len)
		die("used more bytes than were available");
	len -= bytes;
	offset += bytes;

	/* make sure off_t is sufficiently large not to wrap */
	اگر (signed_add_overflows(consumed_bytes, bytes))
		die("pack too large for current definition of off_t");
	consumed_bytes += bytes;
	اگر (max_درون_داد_size && consumed_bytes > max_درون_داد_size)
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
ایستا تهی *get_data(unsigned long size)
{
	git_zstream stream;
	unsigned long bufsize = dry_run && size > 8192 ? 8192 : size;
	تهی *buf = xmallocz(bufsize);

	memset(&stream, 0, اندازه(stream));

	stream.next_out = buf;
	stream.avail_out = bufsize;
	stream.next_in = fill(1);
	stream.avail_in = len;
	git_inflate_init(&stream);

	برای (;;) {
		صحیح ret = git_inflate(&stream, 0);
		use(len - stream.avail_in);
		اگر (stream.total_out == size && ret == Z_STREAM_END)
			بشکن;
		اگر (ret != Z_OK) {
			error("inflate returned %d", ret);
			FREE_AND_NULL(buf);
			اگر (!recover)
				exit(1);
			has_errors = 1;
			بشکن;
		}
		stream.next_in = fill(1);
		stream.avail_in = len;
		اگر (dry_run) {
			/* reuse the buffer in dry_run mode */
			stream.next_out = buf;
			stream.avail_out = bufsize > size - stream.total_out ?
						   size - stream.total_out :
						   bufsize;
		}
	}
	git_inflate_end(&stream);
	اگر (dry_run)
		FREE_AND_NULL(buf);
	بازگشت buf;
}

ساختار delta_info {
	ساختار object_id base_oid;
	unsigned nr;
	off_t base_offset;
	unsigned long size;
	تهی *delta;
	ساختار delta_info *next;
};

ایستا ساختار delta_info *delta_list;

ایستا تهی add_delta_to_list(unsigned nr, ثابت ساختار object_id *base_oid,
			      off_t base_offset,
			      تهی *delta, unsigned long size)
{
	ساختار delta_info *info = xmalloc(اندازه(*info));

	oidcpy(&info->base_oid, base_oid);
	info->base_offset = base_offset;
	info->size = size;
	info->delta = delta;
	info->nr = nr;
	info->next = delta_list;
	delta_list = info;
}

ساختار obj_info {
	off_t offset;
	ساختار object_id oid;
	ساختار object *obj;
};

/* Remember to update object flag allocation in object.h */
#define FLAG_OPEN (1u<<20)
#define FLAG_WRITTEN (1u<<21)

ایستا ساختار obj_info *obj_list;
ایستا unsigned nr_objects;

/*
 * Called only from check_object() after it verified this object
 * is Ok.
 */
ایستا تهی write_cached_object(ساختار object *obj, ساختار obj_buffer *obj_buf)
{
	ساختار object_id oid;

	اگر (odb_write_object(the_repository->objects, obj_buf->buffer, obj_buf->size,
			     obj->type, &oid) < 0)
		die("failed to write object %s", oid_to_hex(&obj->oid));
	obj->flags |= FLAG_WRITTEN;
}

/*
 * At the very end of the processing, write_rest() scans the objects
 * that have reachability requirements and calls this function.
 * Verify its reachability and validity recursively and write it out.
 */
ایستا صحیح check_object(ساختار object *obj, شمارش object_type type,
			تهی *data UNUSED,
			ساختار fsck_options *options UNUSED)
{
	ساختار obj_buffer *obj_buf;

	اگر (!obj)
		بازگشت 1;

	اگر (obj->flags & FLAG_WRITTEN)
		بازگشت 0;

	اگر (type != OBJ_ANY && obj->type != type)
		die("object type mismatch");

	اگر (!(obj->flags & FLAG_OPEN)) {
		size_t size;
		صحیح type = odb_read_object_info(the_repository->objects, &obj->oid, &size);
		اگر (type != obj->type || type <= 0)
			die("object of unexpected type");
		obj->flags |= FLAG_WRITTEN;
		بازگشت 0;
	}

	obj_buf = lookup_object_buffer(obj);
	اگر (!obj_buf)
		die("Whoops! Cannot find object '%s'", oid_to_hex(&obj->oid));
	اگر (fsck_object(obj, obj_buf->buffer, obj_buf->size, &fsck_options))
		die("fsck error in packed object");
	fsck_options.walk = check_object;
	اگر (fsck_walk(obj, پوچ, &fsck_options))
		die("Error on reachable objects of %s", oid_to_hex(&obj->oid));
	write_cached_object(obj, obj_buf);
	بازگشت 0;
}

ایستا تهی write_rest(تهی)
{
	unsigned i;
	برای (i = 0; i < nr_objects; i++) {
		اگر (obj_list[i].obj)
			check_object(obj_list[i].obj, OBJ_ANY, پوچ, پوچ);
	}
}

ایستا تهی added_object(unsigned nr, شمارش object_type type,
			 تهی *data, unsigned long size);

/*
 * Write out nr-th object from the list, now we know the contents
 * of it.  Under --strict, this buffers structured objects in-core,
 * to be checked at the end.
 */
ایستا تهی write_object(unsigned nr, شمارش object_type type,
			 تهی *buf, unsigned long size)
{
	اگر (!strict) {
		اگر (odb_write_object(the_repository->objects, buf, size, type,
				     &obj_list[nr].oid) < 0)
			die("failed to write object");
		added_object(nr, type, buf, size);
		آزاد(buf);
		obj_list[nr].obj = پوچ;
	} وگرنه اگر (type == OBJ_BLOB) {
		ساختار blob *blob;
		اگر (odb_write_object(the_repository->objects, buf, size, type,
				     &obj_list[nr].oid) < 0)
			die("failed to write object");
		added_object(nr, type, buf, size);
		آزاد(buf);

		blob = lookup_blob(the_repository, &obj_list[nr].oid);
		اگر (blob)
			blob->object.flags |= FLAG_WRITTEN;
		وگرنه
			die("invalid blob object");
		obj_list[nr].obj = پوچ;
	} وگرنه {
		ساختار object *obj;
		صحیح eaten;
		hash_object_file(the_hash_algo, buf, size, type,
				 &obj_list[nr].oid);
		added_object(nr, type, buf, size);
		obj = parse_object_buffer(the_repository, &obj_list[nr].oid,
					  type, size, buf,
					  &eaten);
		اگر (!obj)
			die("invalid %s", type_name(type));
		add_object_buffer(obj, buf, size);
		obj->flags |= FLAG_OPEN;
		obj_list[nr].obj = obj;
	}
}

ایستا تهی resolve_delta(unsigned nr, شمارش object_type type,
			  تهی *base, unsigned long base_size,
			  تهی *delta, unsigned long delta_size)
{
	تهی *result;
	size_t result_size;

	result = patch_delta(base, base_size,
			     delta, delta_size,
			     &result_size);
	اگر (!result)
		die("failed to apply delta");
	آزاد(delta);
	write_object(nr, type, result, result_size);
}

/*
 * We now know the contents of an object (which is nr-th in the pack);
 * resolve all the deltified objects that are based on it.
 */
ایستا تهی added_object(unsigned nr, شمارش object_type type,
			 تهی *data, unsigned long size)
{
	ساختار delta_info **p = &delta_list;
	ساختار delta_info *info;

	وقتی ((info = *p) != پوچ) {
		اگر (oideq(&info->base_oid, &obj_list[nr].oid) ||
		    info->base_offset == obj_list[nr].offset) {
			*p = info->next;
			p = &delta_list;
			resolve_delta(info->nr, type, data, size,
				      info->delta, info->size);
			آزاد(info);
			ادامه;
		}
		p = &info->next;
	}
}

ایستا تهی unpack_non_delta_entry(شمارش object_type type, unsigned long size,
				   unsigned nr)
{
	تهی *buf = get_data(size);

	اگر (buf)
		write_object(nr, type, buf, size);
}

ساختار zlib_stream {
	ساختار odb_stream base;
	git_zstream *zstream;
	صحیح status;
};

ایستا ssize_t zlib_stream_read(ساختار odb_stream *in_stream,
				نویسه *buf, size_t buf_len)
{
	ساختار zlib_stream *data = container_of(in_stream, ساختار zlib_stream, base);
	git_zstream *zstream = data->zstream;

	اگر (data->status != Z_OK)
		بازگشت 0;

	zstream->next_out = (unsigned نویسه *) buf;
	zstream->avail_out = buf_len;

	وقتی (data->status == Z_OK && zstream->avail_out == buf_len) {
		zstream->next_in = fill(1);
		zstream->avail_in = len;
		data->status = git_inflate(zstream, 0);
		use(len - zstream->avail_in);
	}

	بازگشت buf_len - zstream->avail_out;
}

ایستا تهی stream_blob(unsigned long size, unsigned nr)
{
	git_zstream zstream = { 0 };
	ساختار zlib_stream in_stream = {
		.base = {
			.اقرأ = zlib_stream_read,
			.size = size,
			.type = OBJ_BLOB,
		},
		.zstream = &zstream,
		.status = Z_OK,
	};
	ساختار obj_info *info = &obj_list[nr];

	git_inflate_init(&zstream);

	اگر (odb_write_object_stream(the_repository->objects, &in_stream.base, &info->oid))
		die(_("failed to write object in stream"));

	اگر (in_stream.status != Z_STREAM_END)
		die(_("inflate returned (%d)"), in_stream.status);
	git_inflate_end(&zstream);

	اگر (strict) {
		ساختار blob *blob = lookup_blob(the_repository, &info->oid);

		اگر (!blob)
			die(_("invalid blob object from stream"));
		blob->object.flags |= FLAG_WRITTEN;
	}
	info->obj = پوچ;
}

ایستا صحیح resolve_against_held(unsigned nr, ثابت ساختار object_id *base,
				تهی *delta_data, unsigned long delta_size)
{
	ساختار object *obj;
	ساختار obj_buffer *obj_buffer;
	obj = lookup_object(the_repository, base);
	اگر (!obj)
		بازگشت 0;
	obj_buffer = lookup_object_buffer(obj);
	اگر (!obj_buffer)
		بازگشت 0;
	resolve_delta(nr, obj->type, obj_buffer->buffer,
		      obj_buffer->size, delta_data, delta_size);
	بازگشت 1;
}

ایستا تهی unpack_delta_entry(شمارش object_type type, unsigned long delta_size,
			       unsigned nr)
{
	تهی *delta_data, *base;
	unsigned long base_size;
	size_t base_size_st = 0;
	ساختار object_id base_oid;

	اگر (type == OBJ_REF_DELTA) {
		oidread(&base_oid, fill(the_hash_algo->rawsz), the_repository->hash_algo);
		use(the_hash_algo->rawsz);
		delta_data = get_data(delta_size);
		اگر (!delta_data)
			بازگشت;
		اگر (odb_has_object(the_repository->objects, &base_oid,
				   ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR))
			; /* Ok we have this one */
		وگرنه اگر (resolve_against_held(nr, &base_oid,
					      delta_data, delta_size))
			بازگشت; /* we are done */
		وگرنه {
			/* cannot resolve yet --- queue it */
			oidclr(&obj_list[nr].oid, the_repository->hash_algo);
			add_delta_to_list(nr, &base_oid, 0, delta_data, delta_size);
			بازگشت;
		}
	} وگرنه {
		unsigned base_found = 0;
		unsigned نویسه *pack, c;
		off_t base_offset;
		unsigned lo, mid, hi;

		pack = fill(1);
		c = *pack;
		use(1);
		base_offset = c & 127;
		وقتی (c & 128) {
			base_offset += 1;
			اگر (!base_offset || MSB(base_offset, 7))
				die("offset value overflow for delta base object");
			pack = fill(1);
			c = *pack;
			use(1);
			base_offset = (base_offset << 7) + (c & 127);
		}
		base_offset = obj_list[nr].offset - base_offset;
		اگر (base_offset <= 0 || base_offset >= obj_list[nr].offset)
			die("offset value out of bound for delta base object");

		delta_data = get_data(delta_size);
		اگر (!delta_data)
			بازگشت;
		lo = 0;
		hi = nr;
		وقتی (lo < hi) {
			mid = lo + (hi - lo) / 2;
			اگر (base_offset < obj_list[mid].offset) {
				hi = mid;
			} وگرنه اگر (base_offset > obj_list[mid].offset) {
				lo = mid + 1;
			} وگرنه {
				oidcpy(&base_oid, &obj_list[mid].oid);
				base_found = !is_null_oid(&base_oid);
				بشکن;
			}
		}
		اگر (!base_found) {
			/*
			 * The delta base object is itself a delta that
			 * has not been resolved yet.
			 */
			oidclr(&obj_list[nr].oid, the_repository->hash_algo);
			add_delta_to_list(nr, null_oid(the_hash_algo), base_offset,
					  delta_data, delta_size);
			بازگشت;
		}
	}

	اگر (resolve_against_held(nr, &base_oid, delta_data, delta_size))
		بازگشت;

	base = odb_read_object(the_repository->objects, &base_oid,
			       &type, &base_size_st);
	base_size = cast_size_t_to_ulong(base_size_st);
	اگر (!base) {
		error("failed to read delta-pack base object %s",
		      oid_to_hex(&base_oid));
		اگر (!recover)
			exit(1);
		has_errors = 1;
		بازگشت;
	}
	resolve_delta(nr, type, base, base_size, delta_data, delta_size);
	آزاد(base);
}

ایستا تهی unpack_one(unsigned nr)
{
	unsigned shift;
	unsigned نویسه *pack;
	size_t size, c;
	شمارش object_type type;

	obj_list[nr].offset = consumed_bytes;

	pack = fill(1);
	c = *pack;
	use(1);
	type = (c >> 4) & 7;
	size = (c & 15);
	shift = 4;
	وقتی (c & 0x80) {
		اگر ((bitsizeof(size_t) - 7) < shift)
			die(_("object size too large for this platform"));
		pack = fill(1);
		c = *pack;
		use(1);
		size += (c & 0x7f) << shift;
		shift += 7;
	}

	انتخاب (type) {
	حالت OBJ_BLOB:
		اگر (!dry_run &&
		    size > repo_settings_get_big_file_threshold(the_repository)) {
			stream_blob(size, nr);
			بازگشت;
		}
		/* fallthrough */
	حالت OBJ_COMMIT:
	حالت OBJ_درخت:
	حالت OBJ_TAG:
		unpack_non_delta_entry(type, size, nr);
		بازگشت;
	حالت OBJ_REF_DELTA:
	حالت OBJ_OFS_DELTA:
		unpack_delta_entry(type, size, nr);
		بازگشت;
	پیشفرض:
		error("bad object type %d", type);
		has_errors = 1;
		اگر (recover)
			بازگشت;
		exit(1);
	}
}

ایستا تهی unpack_all(تهی)
{
	صحیح i;
	unsigned نویسه *hdr = fill(اندازه(ساختار pack_header));
	ساختار odb_transaction *transaction;

	اگر (get_be32(hdr) != PACK_SIGNATURE)
		die("bad pack file");
	hdr += 4;
	اگر (!pack_version_ok_native(get_be32(hdr)))
		die("unknown pack file version %"PRIu32,
		    get_be32(hdr));
	hdr += 4;
	nr_objects = get_be32(hdr);
	use(اندازه(ساختار pack_header));

	اگر (!quiet)
		progress = start_progress(the_repository,
					  _("Unpacking objects"), nr_objects);
	CALLOC_ARRAY(obj_list, nr_objects);
	odb_transaction_begin_or_die(the_repository->objects, &transaction, 0);
	برای (i = 0; i < nr_objects; i++) {
		unpack_one(i);
		display_progress(progress, i + 1);
	}
	odb_transaction_commit_and_finalize_or_die(transaction);
	stop_progress(&progress);

	اگر (delta_list)
		die("unresolved deltas left after unpacking");
}

صحیح cmd_unpack_objects(صحیح argc,
		       ثابت نویسه **argv,
		       ثابت نویسه *prefix UNUSED,
		       ساختار repository *repo)
{
	صحیح i;
	ساختار object_id oid;
	ساختار git_hash_ctx tmp_ctx;

	disable_replace_refs();

	repo_config(the_repository, git_default_config, پوچ);

	quiet = !isatty(2);

	show_usage_if_asked(argc, argv, unpack_usage);

	fsck_options_init(&fsck_options, repo, FSCK_OPTIONS_STRICT);

	برای (i = 1 ; i < argc; i++) {
		ثابت نویسه *arg = argv[i];

		اگر (*arg == '-') {
			اگر (!strcmp(arg, "-n")) {
				dry_run = 1;
				ادامه;
			}
			اگر (!strcmp(arg, "-q")) {
				quiet = 1;
				ادامه;
			}
			اگر (!strcmp(arg, "-r")) {
				recover = 1;
				ادامه;
			}
			اگر (!strcmp(arg, "--strict")) {
				strict = 1;
				ادامه;
			}
			اگر (skip_prefix(arg, "--strict=", &arg)) {
				strict = 1;
				fsck_set_پیام_types(&fsck_options, arg);
				ادامه;
			}
			اگر (skip_prefix(arg, "--pack_header=", &arg)) {
				اگر (parse_pack_header_option(arg,
							     buffer, &len) < 0)
					die(_("bad --pack_header: %s"), arg);
				ادامه;
			}
			اگر (skip_prefix(arg, "--max-input-size=", &arg)) {
				max_درون_داد_size = strtoumax(arg, پوچ, 10);
				ادامه;
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
	اگر (strict) {
		write_rest();
		اگر (fsck_finish(&fsck_options))
			die(_("fsck error in pack objects"));
	}
	اگر (!hasheq(fill(the_hash_algo->rawsz), oid.hash,
		    the_repository->hash_algo))
		die("final sha1 did not match");
	use(the_hash_algo->rawsz);

	/* Write the last part of the buffer to stdout */
	write_in_full(1, buffer + offset, len);

	/* All done */
	بازگشت has_errors;
}
