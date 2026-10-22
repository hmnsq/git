#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#ضمن "builtin.h"
#ضمن "config.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "git-zlib.h"
#ضمن "hex.h"
#ضمن "object-file.h"
#ضمن "odb.h"
#ضمن "odb/streaming.h"
#ضمن "odb/transaction.h"
#ضمن "object.h"
#ضمن "delta.h"
#ضمن "pack.h"
#ضمن "blob.h"
#ضمن "replace-object.h"
#ضمن "strbuf.h"
#ضمن "progress.h"
#ضمن "decorate.h"
#ضمن "fsck.h"
#ضمن "packfile.h"

ساكن صحيح dry_run, quiet, recover, has_errors, strict;
ساكن ثابت حرف unpack_usage[] = "git unpack-objects [-n] [-q] [-r] [--strict]";

ساكن unsigned حرف buffer[DEFAULT_IO_BUFFER_SIZE];
ساكن unsigned صحيح offset, len;
ساكن off_t consumed_bytes;
ساكن off_t max_داخلہ_size;
ساكن بنية git_hash_ctx ctx;
ساكن بنية fsck_options fsck_options;
ساكن بنية progress *progress;

/*
 * When running under --strict mode, objects whose reachability are
 * suspect are kept in core without getting written in the object
 * store.
 */
بنية obj_buffer {
	حرف *buffer;
	unsigned long size;
};

ساكن بنية decoration obj_decorate;

ساكن بنية obj_buffer *lookup_object_buffer(بنية object *base)
{
	أرجع lookup_decoration(&obj_decorate, base);
}

ساكن فراغ add_object_buffer(بنية object *object, حرف *buffer, unsigned long size)
{
	بنية obj_buffer *obj;
	CALLOC_ARRAY(obj, 1);
	obj->buffer = buffer;
	obj->size = size;
	إذا (add_decoration(&obj_decorate, object, obj))
		die("object %s tried to add buffer twice!", oid_to_hex(&object->oid));
}

/*
 * Make sure at least "min" bytes are available in the buffer, and
 * return the pointer to the buffer.
 */
ساكن فراغ *fill(صحيح min)
{
	إذا (min <= len)
		أرجع buffer + offset;
	إذا (min > حجم(buffer))
		die("cannot fill %d bytes", min);
	إذا (offset) {
		git_hash_update(&ctx, buffer, offset);
		memmove(buffer, buffer + offset, len);
		offset = 0;
	}
	افعل {
		ssize_t ret = xread(0, buffer + len, حجم(buffer) - len);
		إذا (ret <= 0) {
			إذا (!ret)
				die("early EOF");
			die_errno("read error on input");
		}
		len += ret;
	} طالما (len < min);
	أرجع buffer;
}

ساكن فراغ use(صحيح bytes)
{
	إذا (bytes > len)
		die("used more bytes than were available");
	len -= bytes;
	offset += bytes;

	/* make sure off_t is sufficiently large not to wrap */
	إذا (signed_add_overflows(consumed_bytes, bytes))
		die("pack too large for current definition of off_t");
	consumed_bytes += bytes;
	إذا (max_داخلہ_size && consumed_bytes > max_داخلہ_size)
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
ساكن فراغ *get_data(unsigned long size)
{
	git_zstream stream;
	unsigned long bufsize = dry_run && size > 8192 ? 8192 : size;
	فراغ *buf = xmallocz(bufsize);

	memset(&stream, 0, حجم(stream));

	stream.next_out = buf;
	stream.avail_out = bufsize;
	stream.next_in = fill(1);
	stream.avail_in = len;
	git_inflate_init(&stream);

	لكل (;;) {
		صحيح ret = git_inflate(&stream, 0);
		use(len - stream.avail_in);
		إذا (stream.total_out == size && ret == Z_STREAM_END)
			اكسر;
		إذا (ret != Z_OK) {
			error("inflate returned %d", ret);
			FREE_AND_NULL(buf);
			إذا (!recover)
				exit(1);
			has_errors = 1;
			اكسر;
		}
		stream.next_in = fill(1);
		stream.avail_in = len;
		إذا (dry_run) {
			/* reuse the buffer in dry_run mode */
			stream.next_out = buf;
			stream.avail_out = bufsize > size - stream.total_out ?
						   size - stream.total_out :
						   bufsize;
		}
	}
	git_inflate_end(&stream);
	إذا (dry_run)
		FREE_AND_NULL(buf);
	أرجع buf;
}

بنية delta_info {
	بنية object_id base_oid;
	unsigned nr;
	off_t base_offset;
	unsigned long size;
	فراغ *delta;
	بنية delta_info *next;
};

ساكن بنية delta_info *delta_list;

ساكن فراغ add_delta_to_list(unsigned nr, ثابت بنية object_id *base_oid,
			      off_t base_offset,
			      فراغ *delta, unsigned long size)
{
	بنية delta_info *info = xmalloc(حجم(*info));

	oidcpy(&info->base_oid, base_oid);
	info->base_offset = base_offset;
	info->size = size;
	info->delta = delta;
	info->nr = nr;
	info->next = delta_list;
	delta_list = info;
}

بنية obj_info {
	off_t offset;
	بنية object_id oid;
	بنية object *obj;
};

/* Remember to update object flag allocation in object.h */
#define FLAG_OPEN (1u<<20)
#define FLAG_WRITTEN (1u<<21)

ساكن بنية obj_info *obj_list;
ساكن unsigned nr_objects;

/*
 * Called only from check_object() after it verified this object
 * is Ok.
 */
ساكن فراغ write_cached_object(بنية object *obj, بنية obj_buffer *obj_buf)
{
	بنية object_id oid;

	إذا (odb_write_object(the_repository->objects, obj_buf->buffer, obj_buf->size,
			     obj->type, &oid) < 0)
		die("failed to write object %s", oid_to_hex(&obj->oid));
	obj->flags |= FLAG_WRITTEN;
}

/*
 * At the very end of the processing, write_rest() scans the objects
 * that have reachability requirements and calls this function.
 * Verify its reachability and validity recursively and write it out.
 */
ساكن صحيح check_object(بنية object *obj, تعداد object_type type,
			فراغ *data UNUSED,
			بنية fsck_options *options UNUSED)
{
	بنية obj_buffer *obj_buf;

	إذا (!obj)
		أرجع 1;

	إذا (obj->flags & FLAG_WRITTEN)
		أرجع 0;

	إذا (type != OBJ_ANY && obj->type != type)
		die("object type mismatch");

	إذا (!(obj->flags & FLAG_OPEN)) {
		size_t size;
		صحيح type = odb_read_object_info(the_repository->objects, &obj->oid, &size);
		إذا (type != obj->type || type <= 0)
			die("object of unexpected type");
		obj->flags |= FLAG_WRITTEN;
		أرجع 0;
	}

	obj_buf = lookup_object_buffer(obj);
	إذا (!obj_buf)
		die("Whoops! Cannot find object '%s'", oid_to_hex(&obj->oid));
	إذا (fsck_object(obj, obj_buf->buffer, obj_buf->size, &fsck_options))
		die("fsck error in packed object");
	fsck_options.walk = check_object;
	إذا (fsck_walk(obj, عدم, &fsck_options))
		die("Error on reachable objects of %s", oid_to_hex(&obj->oid));
	write_cached_object(obj, obj_buf);
	أرجع 0;
}

ساكن فراغ write_rest(فراغ)
{
	unsigned i;
	لكل (i = 0; i < nr_objects; i++) {
		إذا (obj_list[i].obj)
			check_object(obj_list[i].obj, OBJ_ANY, عدم, عدم);
	}
}

ساكن فراغ added_object(unsigned nr, تعداد object_type type,
			 فراغ *data, unsigned long size);

/*
 * Write out nr-th object from the list, now we know the contents
 * of it.  Under --strict, this buffers structured objects in-core,
 * to be checked at the end.
 */
ساكن فراغ write_object(unsigned nr, تعداد object_type type,
			 فراغ *buf, unsigned long size)
{
	إذا (!strict) {
		إذا (odb_write_object(the_repository->objects, buf, size, type,
				     &obj_list[nr].oid) < 0)
			die("failed to write object");
		added_object(nr, type, buf, size);
		حرر(buf);
		obj_list[nr].obj = عدم;
	} وإلا إذا (type == OBJ_BLOB) {
		بنية blob *blob;
		إذا (odb_write_object(the_repository->objects, buf, size, type,
				     &obj_list[nr].oid) < 0)
			die("failed to write object");
		added_object(nr, type, buf, size);
		حرر(buf);

		blob = lookup_blob(the_repository, &obj_list[nr].oid);
		إذا (blob)
			blob->object.flags |= FLAG_WRITTEN;
		وإلا
			die("invalid blob object");
		obj_list[nr].obj = عدم;
	} وإلا {
		بنية object *obj;
		صحيح eaten;
		hash_object_file(the_hash_algo, buf, size, type,
				 &obj_list[nr].oid);
		added_object(nr, type, buf, size);
		obj = parse_object_buffer(the_repository, &obj_list[nr].oid,
					  type, size, buf,
					  &eaten);
		إذا (!obj)
			die("invalid %s", type_name(type));
		add_object_buffer(obj, buf, size);
		obj->flags |= FLAG_OPEN;
		obj_list[nr].obj = obj;
	}
}

ساكن فراغ resolve_delta(unsigned nr, تعداد object_type type,
			  فراغ *base, unsigned long base_size,
			  فراغ *delta, unsigned long delta_size)
{
	فراغ *result;
	size_t result_size;

	result = patch_delta(base, base_size,
			     delta, delta_size,
			     &result_size);
	إذا (!result)
		die("failed to apply delta");
	حرر(delta);
	write_object(nr, type, result, result_size);
}

/*
 * We now know the contents of an object (which is nr-th in the pack);
 * resolve all the deltified objects that are based on it.
 */
ساكن فراغ added_object(unsigned nr, تعداد object_type type,
			 فراغ *data, unsigned long size)
{
	بنية delta_info **p = &delta_list;
	بنية delta_info *info;

	طالما ((info = *p) != عدم) {
		إذا (oideq(&info->base_oid, &obj_list[nr].oid) ||
		    info->base_offset == obj_list[nr].offset) {
			*p = info->next;
			p = &delta_list;
			resolve_delta(info->nr, type, data, size,
				      info->delta, info->size);
			حرر(info);
			استمر;
		}
		p = &info->next;
	}
}

ساكن فراغ unpack_non_delta_entry(تعداد object_type type, unsigned long size,
				   unsigned nr)
{
	فراغ *buf = get_data(size);

	إذا (buf)
		write_object(nr, type, buf, size);
}

بنية zlib_stream {
	بنية odb_stream base;
	git_zstream *zstream;
	صحيح status;
};

ساكن ssize_t zlib_stream_read(بنية odb_stream *in_stream,
				حرف *buf, size_t buf_len)
{
	بنية zlib_stream *data = container_of(in_stream, بنية zlib_stream, base);
	git_zstream *zstream = data->zstream;

	إذا (data->status != Z_OK)
		أرجع 0;

	zstream->next_out = (unsigned حرف *) buf;
	zstream->avail_out = buf_len;

	طالما (data->status == Z_OK && zstream->avail_out == buf_len) {
		zstream->next_in = fill(1);
		zstream->avail_in = len;
		data->status = git_inflate(zstream, 0);
		use(len - zstream->avail_in);
	}

	أرجع buf_len - zstream->avail_out;
}

ساكن فراغ stream_blob(unsigned long size, unsigned nr)
{
	git_zstream zstream = { 0 };
	بنية zlib_stream in_stream = {
		.base = {
			.اقرأ = zlib_stream_read,
			.size = size,
			.type = OBJ_BLOB,
		},
		.zstream = &zstream,
		.status = Z_OK,
	};
	بنية obj_info *info = &obj_list[nr];

	git_inflate_init(&zstream);

	إذا (odb_write_object_stream(the_repository->objects, &in_stream.base, &info->oid))
		die(_("failed to write object in stream"));

	إذا (in_stream.status != Z_STREAM_END)
		die(_("inflate returned (%d)"), in_stream.status);
	git_inflate_end(&zstream);

	إذا (strict) {
		بنية blob *blob = lookup_blob(the_repository, &info->oid);

		إذا (!blob)
			die(_("invalid blob object from stream"));
		blob->object.flags |= FLAG_WRITTEN;
	}
	info->obj = عدم;
}

ساكن صحيح resolve_against_held(unsigned nr, ثابت بنية object_id *base,
				فراغ *delta_data, unsigned long delta_size)
{
	بنية object *obj;
	بنية obj_buffer *obj_buffer;
	obj = lookup_object(the_repository, base);
	إذا (!obj)
		أرجع 0;
	obj_buffer = lookup_object_buffer(obj);
	إذا (!obj_buffer)
		أرجع 0;
	resolve_delta(nr, obj->type, obj_buffer->buffer,
		      obj_buffer->size, delta_data, delta_size);
	أرجع 1;
}

ساكن فراغ unpack_delta_entry(تعداد object_type type, unsigned long delta_size,
			       unsigned nr)
{
	فراغ *delta_data, *base;
	unsigned long base_size;
	size_t base_size_st = 0;
	بنية object_id base_oid;

	إذا (type == OBJ_REF_DELTA) {
		oidread(&base_oid, fill(the_hash_algo->rawsz), the_repository->hash_algo);
		use(the_hash_algo->rawsz);
		delta_data = get_data(delta_size);
		إذا (!delta_data)
			أرجع;
		إذا (odb_has_object(the_repository->objects, &base_oid,
				   ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR))
			; /* Ok we have this one */
		وإلا إذا (resolve_against_held(nr, &base_oid,
					      delta_data, delta_size))
			أرجع; /* we are done */
		وإلا {
			/* cannot resolve yet --- queue it */
			oidclr(&obj_list[nr].oid, the_repository->hash_algo);
			add_delta_to_list(nr, &base_oid, 0, delta_data, delta_size);
			أرجع;
		}
	} وإلا {
		unsigned base_found = 0;
		unsigned حرف *pack, c;
		off_t base_offset;
		unsigned lo, mid, hi;

		pack = fill(1);
		c = *pack;
		use(1);
		base_offset = c & 127;
		طالما (c & 128) {
			base_offset += 1;
			إذا (!base_offset || MSB(base_offset, 7))
				die("offset value overflow for delta base object");
			pack = fill(1);
			c = *pack;
			use(1);
			base_offset = (base_offset << 7) + (c & 127);
		}
		base_offset = obj_list[nr].offset - base_offset;
		إذا (base_offset <= 0 || base_offset >= obj_list[nr].offset)
			die("offset value out of bound for delta base object");

		delta_data = get_data(delta_size);
		إذا (!delta_data)
			أرجع;
		lo = 0;
		hi = nr;
		طالما (lo < hi) {
			mid = lo + (hi - lo) / 2;
			إذا (base_offset < obj_list[mid].offset) {
				hi = mid;
			} وإلا إذا (base_offset > obj_list[mid].offset) {
				lo = mid + 1;
			} وإلا {
				oidcpy(&base_oid, &obj_list[mid].oid);
				base_found = !is_null_oid(&base_oid);
				اكسر;
			}
		}
		إذا (!base_found) {
			/*
			 * The delta base object is itself a delta that
			 * has not been resolved yet.
			 */
			oidclr(&obj_list[nr].oid, the_repository->hash_algo);
			add_delta_to_list(nr, null_oid(the_hash_algo), base_offset,
					  delta_data, delta_size);
			أرجع;
		}
	}

	إذا (resolve_against_held(nr, &base_oid, delta_data, delta_size))
		أرجع;

	base = odb_read_object(the_repository->objects, &base_oid,
			       &type, &base_size_st);
	base_size = cast_size_t_to_ulong(base_size_st);
	إذا (!base) {
		error("failed to read delta-pack base object %s",
		      oid_to_hex(&base_oid));
		إذا (!recover)
			exit(1);
		has_errors = 1;
		أرجع;
	}
	resolve_delta(nr, type, base, base_size, delta_data, delta_size);
	حرر(base);
}

ساكن فراغ unpack_one(unsigned nr)
{
	unsigned shift;
	unsigned حرف *pack;
	size_t size, c;
	تعداد object_type type;

	obj_list[nr].offset = consumed_bytes;

	pack = fill(1);
	c = *pack;
	use(1);
	type = (c >> 4) & 7;
	size = (c & 15);
	shift = 4;
	طالما (c & 0x80) {
		إذا ((bitsizeof(size_t) - 7) < shift)
			die(_("object size too large for this platform"));
		pack = fill(1);
		c = *pack;
		use(1);
		size += (c & 0x7f) << shift;
		shift += 7;
	}

	بدل (type) {
	حالة OBJ_BLOB:
		إذا (!dry_run &&
		    size > repo_settings_get_big_file_threshold(the_repository)) {
			stream_blob(size, nr);
			أرجع;
		}
		/* fallthrough */
	حالة OBJ_COMMIT:
	حالة OBJ_درخت:
	حالة OBJ_TAG:
		unpack_non_delta_entry(type, size, nr);
		أرجع;
	حالة OBJ_REF_DELTA:
	حالة OBJ_OFS_DELTA:
		unpack_delta_entry(type, size, nr);
		أرجع;
	افتراضي:
		error("bad object type %d", type);
		has_errors = 1;
		إذا (recover)
			أرجع;
		exit(1);
	}
}

ساكن فراغ unpack_all(فراغ)
{
	صحيح i;
	unsigned حرف *hdr = fill(حجم(بنية pack_header));
	بنية odb_transaction *transaction;

	إذا (get_be32(hdr) != PACK_SIGNATURE)
		die("bad pack file");
	hdr += 4;
	إذا (!pack_version_ok_native(get_be32(hdr)))
		die("unknown pack file version %"PRIu32,
		    get_be32(hdr));
	hdr += 4;
	nr_objects = get_be32(hdr);
	use(حجم(بنية pack_header));

	إذا (!quiet)
		progress = start_progress(the_repository,
					  _("Unpacking objects"), nr_objects);
	CALLOC_ARRAY(obj_list, nr_objects);
	odb_transaction_begin_or_die(the_repository->objects, &transaction, 0);
	لكل (i = 0; i < nr_objects; i++) {
		unpack_one(i);
		display_progress(progress, i + 1);
	}
	odb_transaction_commit_and_finalize_or_die(transaction);
	stop_progress(&progress);

	إذا (delta_list)
		die("unresolved deltas left after unpacking");
}

صحيح cmd_unpack_objects(صحيح argc,
		       ثابت حرف **argv,
		       ثابت حرف *prefix UNUSED,
		       بنية repository *repo)
{
	صحيح i;
	بنية object_id oid;
	بنية git_hash_ctx tmp_ctx;

	disable_replace_refs();

	repo_config(the_repository, git_default_config, عدم);

	quiet = !isatty(2);

	show_usage_if_asked(argc, argv, unpack_usage);

	fsck_options_init(&fsck_options, repo, FSCK_OPTIONS_STRICT);

	لكل (i = 1 ; i < argc; i++) {
		ثابت حرف *arg = argv[i];

		إذا (*arg == '-') {
			إذا (!strcmp(arg, "-n")) {
				dry_run = 1;
				استمر;
			}
			إذا (!strcmp(arg, "-q")) {
				quiet = 1;
				استمر;
			}
			إذا (!strcmp(arg, "-r")) {
				recover = 1;
				استمر;
			}
			إذا (!strcmp(arg, "--strict")) {
				strict = 1;
				استمر;
			}
			إذا (skip_prefix(arg, "--strict=", &arg)) {
				strict = 1;
				fsck_set_پیغام_types(&fsck_options, arg);
				استمر;
			}
			إذا (skip_prefix(arg, "--pack_header=", &arg)) {
				إذا (parse_pack_header_option(arg,
							     buffer, &len) < 0)
					die(_("bad --pack_header: %s"), arg);
				استمر;
			}
			إذا (skip_prefix(arg, "--max-input-size=", &arg)) {
				max_داخلہ_size = strtoumax(arg, عدم, 10);
				استمر;
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
	إذا (strict) {
		write_rest();
		إذا (fsck_finish(&fsck_options))
			die(_("fsck error in pack objects"));
	}
	إذا (!hasheq(fill(the_hash_algo->rawsz), oid.hash,
		    the_repository->hash_algo))
		die("final sha1 did not match");
	use(the_hash_algo->rawsz);

	/* Write the last part of the buffer to stdout */
	write_in_full(1, buffer + offset, len);

	/* All done */
	أرجع has_errors;
}
