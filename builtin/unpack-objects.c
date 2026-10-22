#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ "environment.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "git-zlib.h"
#ଅନ୍ତର୍ଭାଵ "hex.h"
#ଅନ୍ତର୍ଭାଵ "object-file.h"
#ଅନ୍ତର୍ଭାଵ "odb.h"
#ଅନ୍ତର୍ଭାଵ "odb/streaming.h"
#ଅନ୍ତର୍ଭାଵ "odb/transaction.h"
#ଅନ୍ତର୍ଭାଵ "object.h"
#ଅନ୍ତର୍ଭାଵ "delta.h"
#ଅନ୍ତର୍ଭାଵ "pack.h"
#ଅନ୍ତର୍ଭାଵ "blob.h"
#ଅନ୍ତର୍ଭାଵ "replace-object.h"
#ଅନ୍ତର୍ଭାଵ "strbuf.h"
#ଅନ୍ତର୍ଭାଵ "progress.h"
#ଅନ୍ତର୍ଭାଵ "decorate.h"
#ଅନ୍ତର୍ଭାଵ "fsck.h"
#ଅନ୍ତର୍ଭାଵ "packfile.h"

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା dry_run, quiet, recover, has_errors, strict;
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର unpack_usage[] = "git unpack-objects [-n] [-q] [-r] [--strict]";

ସ୍ଥିର unsigned ଅକ୍ଷର buffer[DEFAULT_IO_BUFFER_SIZE];
ସ୍ଥିର unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା offset, len;
ସ୍ଥିର off_t consumed_bytes;
ସ୍ଥିର off_t max_ନିବେଶ_size;
ସ୍ଥିର ସଂରଚନା git_hash_ctx ctx;
ସ୍ଥିର ସଂରଚନା fsck_options fsck_options;
ସ୍ଥିର ସଂରଚନା progress *progress;

/*
 * When running under --strict mode, objects whose reachability are
 * suspect are kept in core without getting written in the object
 * store.
 */
ସଂରଚନା obj_buffer {
	ଅକ୍ଷର *buffer;
	unsigned long size;
};

ସ୍ଥିର ସଂରଚନା decoration obj_decorate;

ସ୍ଥିର ସଂରଚନା obj_buffer *lookup_object_buffer(ସଂରଚନା object *base)
{
	ଫେରନ୍ତୁ lookup_decoration(&obj_decorate, base);
}

ସ୍ଥିର ଶୂନ୍ୟ add_object_buffer(ସଂରଚନା object *object, ଅକ୍ଷର *buffer, unsigned long size)
{
	ସଂରଚନା obj_buffer *obj;
	CALLOC_ARRAY(obj, 1);
	obj->buffer = buffer;
	obj->size = size;
	ଯଦି (add_decoration(&obj_decorate, object, obj))
		die("object %s tried to add buffer twice!", oid_to_hex(&object->oid));
}

/*
 * Make sure at least "min" bytes are available in the buffer, and
 * return the pointer to the buffer.
 */
ସ୍ଥିର ଶୂନ୍ୟ *fill(ପୂର୍ଣ୍ଣସଂଖ୍ୟା min)
{
	ଯଦି (min <= len)
		ଫେରନ୍ତୁ buffer + offset;
	ଯଦି (min > ଆକାର(buffer))
		die("cannot fill %d bytes", min);
	ଯଦି (offset) {
		git_hash_update(&ctx, buffer, offset);
		memmove(buffer, buffer + offset, len);
		offset = 0;
	}
	କର {
		ssize_t ret = xread(0, buffer + len, ଆକାର(buffer) - len);
		ଯଦି (ret <= 0) {
			ଯଦି (!ret)
				die("early EOF");
			die_errno("read error on input");
		}
		len += ret;
	} ଯେତେବେଳେ (len < min);
	ଫେରନ୍ତୁ buffer;
}

ସ୍ଥିର ଶୂନ୍ୟ use(ପୂର୍ଣ୍ଣସଂଖ୍ୟା bytes)
{
	ଯଦି (bytes > len)
		die("used more bytes than were available");
	len -= bytes;
	offset += bytes;

	/* make sure off_t is sufficiently large not to wrap */
	ଯଦି (signed_add_overflows(consumed_bytes, bytes))
		die("pack too large for current definition of off_t");
	consumed_bytes += bytes;
	ଯଦି (max_ନିବେଶ_size && consumed_bytes > max_ନିବେଶ_size)
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
ସ୍ଥିର ଶୂନ୍ୟ *get_data(unsigned long size)
{
	git_zstream stream;
	unsigned long bufsize = dry_run && size > 8192 ? 8192 : size;
	ଶୂନ୍ୟ *buf = xmallocz(bufsize);

	memset(&stream, 0, ଆକାର(stream));

	stream.next_out = buf;
	stream.avail_out = bufsize;
	stream.next_in = fill(1);
	stream.avail_in = len;
	git_inflate_init(&stream);

	ପାଇଁ (;;) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = git_inflate(&stream, 0);
		use(len - stream.avail_in);
		ଯଦି (stream.total_out == size && ret == Z_STREAM_END)
			ଭାଙ୍ଗ;
		ଯଦି (ret != Z_OK) {
			error("inflate returned %d", ret);
			FREE_AND_NULL(buf);
			ଯଦି (!recover)
				exit(1);
			has_errors = 1;
			ଭାଙ୍ଗ;
		}
		stream.next_in = fill(1);
		stream.avail_in = len;
		ଯଦି (dry_run) {
			/* reuse the buffer in dry_run mode */
			stream.next_out = buf;
			stream.avail_out = bufsize > size - stream.total_out ?
						   size - stream.total_out :
						   bufsize;
		}
	}
	git_inflate_end(&stream);
	ଯଦି (dry_run)
		FREE_AND_NULL(buf);
	ଫେରନ୍ତୁ buf;
}

ସଂରଚନା delta_info {
	ସଂରଚନା object_id base_oid;
	unsigned nr;
	off_t base_offset;
	unsigned long size;
	ଶୂନ୍ୟ *delta;
	ସଂରଚନା delta_info *next;
};

ସ୍ଥିର ସଂରଚନା delta_info *delta_list;

ସ୍ଥିର ଶୂନ୍ୟ add_delta_to_list(unsigned nr, ଅଚଳ ସଂରଚନା object_id *base_oid,
			      off_t base_offset,
			      ଶୂନ୍ୟ *delta, unsigned long size)
{
	ସଂରଚନା delta_info *info = xmalloc(ଆକାର(*info));

	oidcpy(&info->base_oid, base_oid);
	info->base_offset = base_offset;
	info->size = size;
	info->delta = delta;
	info->nr = nr;
	info->next = delta_list;
	delta_list = info;
}

ସଂରଚନା obj_info {
	off_t offset;
	ସଂରଚନା object_id oid;
	ସଂରଚନା object *obj;
};

/* Remember to update object flag allocation in object.h */
#define FLAG_OPEN (1u<<20)
#define FLAG_WRITTEN (1u<<21)

ସ୍ଥିର ସଂରଚନା obj_info *obj_list;
ସ୍ଥିର unsigned nr_objects;

/*
 * Called only from check_object() after it verified this object
 * is Ok.
 */
ସ୍ଥିର ଶୂନ୍ୟ write_cached_object(ସଂରଚନା object *obj, ସଂରଚନା obj_buffer *obj_buf)
{
	ସଂରଚନା object_id oid;

	ଯଦି (odb_write_object(the_repository->objects, obj_buf->buffer, obj_buf->size,
			     obj->type, &oid) < 0)
		die("failed to write object %s", oid_to_hex(&obj->oid));
	obj->flags |= FLAG_WRITTEN;
}

/*
 * At the very end of the processing, write_rest() scans the objects
 * that have reachability requirements and calls this function.
 * Verify its reachability and validity recursively and write it out.
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା check_object(ସଂରଚନା object *obj, ଗଣନା object_type type,
			ଶୂନ୍ୟ *data UNUSED,
			ସଂରଚନା fsck_options *options UNUSED)
{
	ସଂରଚନା obj_buffer *obj_buf;

	ଯଦି (!obj)
		ଫେରନ୍ତୁ 1;

	ଯଦି (obj->flags & FLAG_WRITTEN)
		ଫେରନ୍ତୁ 0;

	ଯଦି (type != OBJ_ANY && obj->type != type)
		die("object type mismatch");

	ଯଦି (!(obj->flags & FLAG_OPEN)) {
		size_t size;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା type = odb_read_object_info(the_repository->objects, &obj->oid, &size);
		ଯଦି (type != obj->type || type <= 0)
			die("object of unexpected type");
		obj->flags |= FLAG_WRITTEN;
		ଫେରନ୍ତୁ 0;
	}

	obj_buf = lookup_object_buffer(obj);
	ଯଦି (!obj_buf)
		die("Whoops! Cannot find object '%s'", oid_to_hex(&obj->oid));
	ଯଦି (fsck_object(obj, obj_buf->buffer, obj_buf->size, &fsck_options))
		die("fsck error in packed object");
	fsck_options.walk = check_object;
	ଯଦି (fsck_walk(obj, ଖାଲି, &fsck_options))
		die("Error on reachable objects of %s", oid_to_hex(&obj->oid));
	write_cached_object(obj, obj_buf);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ write_rest(ଶୂନ୍ୟ)
{
	unsigned i;
	ପାଇଁ (i = 0; i < nr_objects; i++) {
		ଯଦି (obj_list[i].obj)
			check_object(obj_list[i].obj, OBJ_ANY, ଖାଲି, ଖାଲି);
	}
}

ସ୍ଥିର ଶୂନ୍ୟ added_object(unsigned nr, ଗଣନା object_type type,
			 ଶୂନ୍ୟ *data, unsigned long size);

/*
 * Write out nr-th object from the list, now we know the contents
 * of it.  Under --strict, this buffers structured objects in-core,
 * to be checked at the end.
 */
ସ୍ଥିର ଶୂନ୍ୟ write_object(unsigned nr, ଗଣନା object_type type,
			 ଶୂନ୍ୟ *buf, unsigned long size)
{
	ଯଦି (!strict) {
		ଯଦି (odb_write_object(the_repository->objects, buf, size, type,
				     &obj_list[nr].oid) < 0)
			die("failed to write object");
		added_object(nr, type, buf, size);
		ମୁକ୍ତ(buf);
		obj_list[nr].obj = ଖାଲି;
	} ଅନ୍ୟଥା ଯଦି (type == OBJ_BLOB) {
		ସଂରଚନା blob *blob;
		ଯଦି (odb_write_object(the_repository->objects, buf, size, type,
				     &obj_list[nr].oid) < 0)
			die("failed to write object");
		added_object(nr, type, buf, size);
		ମୁକ୍ତ(buf);

		blob = lookup_blob(the_repository, &obj_list[nr].oid);
		ଯଦି (blob)
			blob->object.flags |= FLAG_WRITTEN;
		ଅନ୍ୟଥା
			die("invalid blob object");
		obj_list[nr].obj = ଖାଲି;
	} ଅନ୍ୟଥା {
		ସଂରଚନା object *obj;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା eaten;
		hash_object_file(the_hash_algo, buf, size, type,
				 &obj_list[nr].oid);
		added_object(nr, type, buf, size);
		obj = parse_object_buffer(the_repository, &obj_list[nr].oid,
					  type, size, buf,
					  &eaten);
		ଯଦି (!obj)
			die("invalid %s", type_name(type));
		add_object_buffer(obj, buf, size);
		obj->flags |= FLAG_OPEN;
		obj_list[nr].obj = obj;
	}
}

ସ୍ଥିର ଶୂନ୍ୟ resolve_delta(unsigned nr, ଗଣନା object_type type,
			  ଶୂନ୍ୟ *base, unsigned long base_size,
			  ଶୂନ୍ୟ *delta, unsigned long delta_size)
{
	ଶୂନ୍ୟ *result;
	size_t result_size;

	result = patch_delta(base, base_size,
			     delta, delta_size,
			     &result_size);
	ଯଦି (!result)
		die("failed to apply delta");
	ମୁକ୍ତ(delta);
	write_object(nr, type, result, result_size);
}

/*
 * We now know the contents of an object (which is nr-th in the pack);
 * resolve all the deltified objects that are based on it.
 */
ସ୍ଥିର ଶୂନ୍ୟ added_object(unsigned nr, ଗଣନା object_type type,
			 ଶୂନ୍ୟ *data, unsigned long size)
{
	ସଂରଚନା delta_info **p = &delta_list;
	ସଂରଚନା delta_info *info;

	ଯେତେବେଳେ ((info = *p) != ଖାଲି) {
		ଯଦି (oideq(&info->base_oid, &obj_list[nr].oid) ||
		    info->base_offset == obj_list[nr].offset) {
			*p = info->next;
			p = &delta_list;
			resolve_delta(info->nr, type, data, size,
				      info->delta, info->size);
			ମୁକ୍ତ(info);
			ଜାରି;
		}
		p = &info->next;
	}
}

ସ୍ଥିର ଶୂନ୍ୟ unpack_non_delta_entry(ଗଣନା object_type type, unsigned long size,
				   unsigned nr)
{
	ଶୂନ୍ୟ *buf = get_data(size);

	ଯଦି (buf)
		write_object(nr, type, buf, size);
}

ସଂରଚନା zlib_stream {
	ସଂରଚନା odb_stream base;
	git_zstream *zstream;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା status;
};

ସ୍ଥିର ssize_t zlib_stream_read(ସଂରଚନା odb_stream *in_stream,
				ଅକ୍ଷର *buf, size_t buf_len)
{
	ସଂରଚନା zlib_stream *data = container_of(in_stream, ସଂରଚନା zlib_stream, base);
	git_zstream *zstream = data->zstream;

	ଯଦି (data->status != Z_OK)
		ଫେରନ୍ତୁ 0;

	zstream->next_out = (unsigned ଅକ୍ଷର *) buf;
	zstream->avail_out = buf_len;

	ଯେତେବେଳେ (data->status == Z_OK && zstream->avail_out == buf_len) {
		zstream->next_in = fill(1);
		zstream->avail_in = len;
		data->status = git_inflate(zstream, 0);
		use(len - zstream->avail_in);
	}

	ଫେରନ୍ତୁ buf_len - zstream->avail_out;
}

ସ୍ଥିର ଶୂନ୍ୟ stream_blob(unsigned long size, unsigned nr)
{
	git_zstream zstream = { 0 };
	ସଂରଚନା zlib_stream in_stream = {
		.base = {
			.ପଠ = zlib_stream_read,
			.size = size,
			.type = OBJ_BLOB,
		},
		.zstream = &zstream,
		.status = Z_OK,
	};
	ସଂରଚନା obj_info *info = &obj_list[nr];

	git_inflate_init(&zstream);

	ଯଦି (odb_write_object_stream(the_repository->objects, &in_stream.base, &info->oid))
		die(_("failed to write object in stream"));

	ଯଦି (in_stream.status != Z_STREAM_END)
		die(_("inflate returned (%d)"), in_stream.status);
	git_inflate_end(&zstream);

	ଯଦି (strict) {
		ସଂରଚନା blob *blob = lookup_blob(the_repository, &info->oid);

		ଯଦି (!blob)
			die(_("invalid blob object from stream"));
		blob->object.flags |= FLAG_WRITTEN;
	}
	info->obj = ଖାଲି;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା resolve_against_held(unsigned nr, ଅଚଳ ସଂରଚନା object_id *base,
				ଶୂନ୍ୟ *delta_data, unsigned long delta_size)
{
	ସଂରଚନା object *obj;
	ସଂରଚନା obj_buffer *obj_buffer;
	obj = lookup_object(the_repository, base);
	ଯଦି (!obj)
		ଫେରନ୍ତୁ 0;
	obj_buffer = lookup_object_buffer(obj);
	ଯଦି (!obj_buffer)
		ଫେରନ୍ତୁ 0;
	resolve_delta(nr, obj->type, obj_buffer->buffer,
		      obj_buffer->size, delta_data, delta_size);
	ଫେରନ୍ତୁ 1;
}

ସ୍ଥିର ଶୂନ୍ୟ unpack_delta_entry(ଗଣନା object_type type, unsigned long delta_size,
			       unsigned nr)
{
	ଶୂନ୍ୟ *delta_data, *base;
	unsigned long base_size;
	size_t base_size_st = 0;
	ସଂରଚନା object_id base_oid;

	ଯଦି (type == OBJ_REF_DELTA) {
		oidread(&base_oid, fill(the_hash_algo->rawsz), the_repository->hash_algo);
		use(the_hash_algo->rawsz);
		delta_data = get_data(delta_size);
		ଯଦି (!delta_data)
			ଫେରନ୍ତୁ;
		ଯଦି (odb_has_object(the_repository->objects, &base_oid,
				   ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR))
			; /* Ok we have this one */
		ଅନ୍ୟଥା ଯଦି (resolve_against_held(nr, &base_oid,
					      delta_data, delta_size))
			ଫେରନ୍ତୁ; /* we are done */
		ଅନ୍ୟଥା {
			/* cannot resolve yet --- queue it */
			oidclr(&obj_list[nr].oid, the_repository->hash_algo);
			add_delta_to_list(nr, &base_oid, 0, delta_data, delta_size);
			ଫେରନ୍ତୁ;
		}
	} ଅନ୍ୟଥା {
		unsigned base_found = 0;
		unsigned ଅକ୍ଷର *pack, c;
		off_t base_offset;
		unsigned lo, mid, hi;

		pack = fill(1);
		c = *pack;
		use(1);
		base_offset = c & 127;
		ଯେତେବେଳେ (c & 128) {
			base_offset += 1;
			ଯଦି (!base_offset || MSB(base_offset, 7))
				die("offset value overflow for delta base object");
			pack = fill(1);
			c = *pack;
			use(1);
			base_offset = (base_offset << 7) + (c & 127);
		}
		base_offset = obj_list[nr].offset - base_offset;
		ଯଦି (base_offset <= 0 || base_offset >= obj_list[nr].offset)
			die("offset value out of bound for delta base object");

		delta_data = get_data(delta_size);
		ଯଦି (!delta_data)
			ଫେରନ୍ତୁ;
		lo = 0;
		hi = nr;
		ଯେତେବେଳେ (lo < hi) {
			mid = lo + (hi - lo) / 2;
			ଯଦି (base_offset < obj_list[mid].offset) {
				hi = mid;
			} ଅନ୍ୟଥା ଯଦି (base_offset > obj_list[mid].offset) {
				lo = mid + 1;
			} ଅନ୍ୟଥା {
				oidcpy(&base_oid, &obj_list[mid].oid);
				base_found = !is_null_oid(&base_oid);
				ଭାଙ୍ଗ;
			}
		}
		ଯଦି (!base_found) {
			/*
			 * The delta base object is itself a delta that
			 * has not been resolved yet.
			 */
			oidclr(&obj_list[nr].oid, the_repository->hash_algo);
			add_delta_to_list(nr, null_oid(the_hash_algo), base_offset,
					  delta_data, delta_size);
			ଫେରନ୍ତୁ;
		}
	}

	ଯଦି (resolve_against_held(nr, &base_oid, delta_data, delta_size))
		ଫେରନ୍ତୁ;

	base = odb_read_object(the_repository->objects, &base_oid,
			       &type, &base_size_st);
	base_size = cast_size_t_to_ulong(base_size_st);
	ଯଦି (!base) {
		error("failed to read delta-pack base object %s",
		      oid_to_hex(&base_oid));
		ଯଦି (!recover)
			exit(1);
		has_errors = 1;
		ଫେରନ୍ତୁ;
	}
	resolve_delta(nr, type, base, base_size, delta_data, delta_size);
	ମୁକ୍ତ(base);
}

ସ୍ଥିର ଶୂନ୍ୟ unpack_one(unsigned nr)
{
	unsigned shift;
	unsigned ଅକ୍ଷର *pack;
	size_t size, c;
	ଗଣନା object_type type;

	obj_list[nr].offset = consumed_bytes;

	pack = fill(1);
	c = *pack;
	use(1);
	type = (c >> 4) & 7;
	size = (c & 15);
	shift = 4;
	ଯେତେବେଳେ (c & 0x80) {
		ଯଦି ((bitsizeof(size_t) - 7) < shift)
			die(_("object size too large for this platform"));
		pack = fill(1);
		c = *pack;
		use(1);
		size += (c & 0x7f) << shift;
		shift += 7;
	}

	ବାଛ (type) {
	ସ୍ଥିତି OBJ_BLOB:
		ଯଦି (!dry_run &&
		    size > repo_settings_get_big_file_threshold(the_repository)) {
			stream_blob(size, nr);
			ଫେରନ୍ତୁ;
		}
		/* fallthrough */
	ସ୍ଥିତି OBJ_COMMIT:
	ସ୍ଥିତି OBJ_ବୃକ୍ଷ:
	ସ୍ଥିତି OBJ_TAG:
		unpack_non_delta_entry(type, size, nr);
		ଫେରନ୍ତୁ;
	ସ୍ଥିତି OBJ_REF_DELTA:
	ସ୍ଥିତି OBJ_OFS_DELTA:
		unpack_delta_entry(type, size, nr);
		ଫେରନ୍ତୁ;
	ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
		error("bad object type %d", type);
		has_errors = 1;
		ଯଦି (recover)
			ଫେରନ୍ତୁ;
		exit(1);
	}
}

ସ୍ଥିର ଶୂନ୍ୟ unpack_all(ଶୂନ୍ୟ)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
	unsigned ଅକ୍ଷର *hdr = fill(ଆକାର(ସଂରଚନା pack_header));
	ସଂରଚନା odb_transaction *transaction;

	ଯଦି (get_be32(hdr) != PACK_SIGNATURE)
		die("bad pack file");
	hdr += 4;
	ଯଦି (!pack_version_ok_native(get_be32(hdr)))
		die("unknown pack file version %"PRIu32,
		    get_be32(hdr));
	hdr += 4;
	nr_objects = get_be32(hdr);
	use(ଆକାର(ସଂରଚନା pack_header));

	ଯଦି (!quiet)
		progress = start_progress(the_repository,
					  _("Unpacking objects"), nr_objects);
	CALLOC_ARRAY(obj_list, nr_objects);
	odb_transaction_begin_or_die(the_repository->objects, &transaction, 0);
	ପାଇଁ (i = 0; i < nr_objects; i++) {
		unpack_one(i);
		display_progress(progress, i + 1);
	}
	odb_transaction_commit_and_finalize_or_die(transaction);
	stop_progress(&progress);

	ଯଦି (delta_list)
		die("unresolved deltas left after unpacking");
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_unpack_objects(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
		       ଅଚଳ ଅକ୍ଷର **argv,
		       ଅଚଳ ଅକ୍ଷର *prefix UNUSED,
		       ସଂରଚନା repository *repo)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
	ସଂରଚନା object_id oid;
	ସଂରଚନା git_hash_ctx tmp_ctx;

	disable_replace_refs();

	repo_config(the_repository, git_default_config, ଖାଲି);

	quiet = !isatty(2);

	show_usage_if_asked(argc, argv, unpack_usage);

	fsck_options_init(&fsck_options, repo, FSCK_OPTIONS_STRICT);

	ପାଇଁ (i = 1 ; i < argc; i++) {
		ଅଚଳ ଅକ୍ଷର *arg = argv[i];

		ଯଦି (*arg == '-') {
			ଯଦି (!strcmp(arg, "-n")) {
				dry_run = 1;
				ଜାରି;
			}
			ଯଦି (!strcmp(arg, "-q")) {
				quiet = 1;
				ଜାରି;
			}
			ଯଦି (!strcmp(arg, "-r")) {
				recover = 1;
				ଜାରି;
			}
			ଯଦି (!strcmp(arg, "--strict")) {
				strict = 1;
				ଜାରି;
			}
			ଯଦି (skip_prefix(arg, "--strict=", &arg)) {
				strict = 1;
				fsck_set_ସନ୍ଦେଶ_types(&fsck_options, arg);
				ଜାରି;
			}
			ଯଦି (skip_prefix(arg, "--pack_header=", &arg)) {
				ଯଦି (parse_pack_header_option(arg,
							     buffer, &len) < 0)
					die(_("bad --pack_header: %s"), arg);
				ଜାରି;
			}
			ଯଦି (skip_prefix(arg, "--max-input-size=", &arg)) {
				max_ନିବେଶ_size = strtoumax(arg, ଖାଲି, 10);
				ଜାରି;
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
	ଯଦି (strict) {
		write_rest();
		ଯଦି (fsck_finish(&fsck_options))
			die(_("fsck error in pack objects"));
	}
	ଯଦି (!hasheq(fill(the_hash_algo->rawsz), oid.hash,
		    the_repository->hash_algo))
		die("final sha1 did not match");
	use(the_hash_algo->rawsz);

	/* Write the last part of the buffer to stdout */
	write_in_full(1, buffer + offset, len);

	/* All done */
	ଫେରନ୍ତୁ has_errors;
}
