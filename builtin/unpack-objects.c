#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#include "builtin.h"
#include "config.h"
#include "environment.h"
#include "gettext.h"
#include "git-zlib.h"
#include "hex.h"
#include "object-file.h"
#include "odb.h"
#include "odb/streaming.h"
#include "odb/transaction.h"
#include "object.h"
#include "delta.h"
#include "pack.h"
#include "blob.h"
#include "replace-object.h"
#include "strbuf.h"
#include "progress.h"
#include "decorate.h"
#include "fsck.h"
#include "packfile.h"

static целое dry_run, quiet, recover, has_errors, strict;
static const char unpack_usage[] = "git unpack-objects [-n] [-q] [-r] [--strict]";

static unsigned char buffer[DEFAULT_IO_BUFFER_SIZE];
static unsigned целое offset, len;
static off_t consumed_bytes;
static off_t max_input_size;
static struct git_hash_ctx ctx;
static struct fsck_options fsck_options;
static struct progress *progress;

/*
 * When running under --strict mode, objects whose reachability are
 * suspect are kept in core without getting written in the object
 * store.
 */
struct obj_buffer {
	char *buffer;
	unsigned long size;
};

static struct decoration obj_decorate;

static struct obj_buffer *lookup_object_buffer(struct object *base)
{
	вернуть lookup_decoration(&obj_decorate, base);
}

static void add_object_buffer(struct object *object, char *buffer, unsigned long size)
{
	struct obj_buffer *obj;
	CALLOC_ARRAY(obj, 1);
	obj->buffer = buffer;
	obj->size = size;
	если (add_decoration(&obj_decorate, object, obj))
		die("object %s tried to add buffer twice!", oid_to_hex(&object->oid));
}

/*
 * Make sure at least "min" bytes are available in the buffer, and
 * return the pointer to the buffer.
 */
static void *fill(целое min)
{
	если (min <= len)
		вернуть buffer + offset;
	если (min > sizeof(buffer))
		die("cannot fill %d bytes", min);
	если (offset) {
		git_hash_update(&ctx, buffer, offset);
		memmove(buffer, buffer + offset, len);
		offset = 0;
	}
	do {
		ssize_t ret = xread(0, buffer + len, sizeof(buffer) - len);
		если (ret <= 0) {
			если (!ret)
				die("early EOF");
			die_errno("read error on input");
		}
		len += ret;
	} пока (len < min);
	вернуть buffer;
}

static void use(целое bytes)
{
	если (bytes > len)
		die("used more bytes than were available");
	len -= bytes;
	offset += bytes;

	/* make sure off_t is sufficiently large not to wrap */
	если (signed_add_overflows(consumed_bytes, bytes))
		die("pack too large for current definition of off_t");
	consumed_bytes += bytes;
	если (max_input_size && consumed_bytes > max_input_size)
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
static void *get_data(unsigned long size)
{
	git_zstream stream;
	unsigned long bufsize = dry_run && size > 8192 ? 8192 : size;
	void *buf = xmallocz(bufsize);

	memset(&stream, 0, sizeof(stream));

	stream.next_out = buf;
	stream.avail_out = bufsize;
	stream.next_in = fill(1);
	stream.avail_in = len;
	git_inflate_init(&stream);

	для (;;) {
		целое ret = git_inflate(&stream, 0);
		use(len - stream.avail_in);
		если (stream.total_out == size && ret == Z_STREAM_END)
			break;
		если (ret != Z_OK) {
			error("inflate returned %d", ret);
			FREE_AND_NULL(buf);
			если (!recover)
				exit(1);
			has_errors = 1;
			break;
		}
		stream.next_in = fill(1);
		stream.avail_in = len;
		если (dry_run) {
			/* reuse the buffer in dry_run mode */
			stream.next_out = buf;
			stream.avail_out = bufsize > size - stream.total_out ?
						   size - stream.total_out :
						   bufsize;
		}
	}
	git_inflate_end(&stream);
	если (dry_run)
		FREE_AND_NULL(buf);
	вернуть buf;
}

struct delta_info {
	struct object_id base_oid;
	unsigned nr;
	off_t base_offset;
	unsigned long size;
	void *delta;
	struct delta_info *next;
};

static struct delta_info *delta_list;

static void add_delta_to_list(unsigned nr, const struct object_id *base_oid,
			      off_t base_offset,
			      void *delta, unsigned long size)
{
	struct delta_info *info = xmalloc(sizeof(*info));

	oidcpy(&info->base_oid, base_oid);
	info->base_offset = base_offset;
	info->size = size;
	info->delta = delta;
	info->nr = nr;
	info->next = delta_list;
	delta_list = info;
}

struct obj_info {
	off_t offset;
	struct object_id oid;
	struct object *obj;
};

/* Remember to update object flag allocation in object.h */
#define FLAG_OPEN (1u<<20)
#define FLAG_WRITTEN (1u<<21)

static struct obj_info *obj_list;
static unsigned nr_objects;

/*
 * Called only from check_object() after it verified this object
 * is Ok.
 */
static void write_cached_object(struct object *obj, struct obj_buffer *obj_buf)
{
	struct object_id oid;

	если (odb_write_object(the_repository->objects, obj_buf->buffer, obj_buf->size,
			     obj->type, &oid) < 0)
		die("failed to write object %s", oid_to_hex(&obj->oid));
	obj->flags |= FLAG_WRITTEN;
}

/*
 * At the very end of the processing, write_rest() scans the objects
 * that have reachability requirements and calls this function.
 * Verify its reachability and validity recursively and write it out.
 */
static целое check_object(struct object *obj, enum object_type type,
			void *data UNUSED,
			struct fsck_options *options UNUSED)
{
	struct obj_buffer *obj_buf;

	если (!obj)
		вернуть 1;

	если (obj->flags & FLAG_WRITTEN)
		вернуть 0;

	если (type != OBJ_ANY && obj->type != type)
		die("object type mismatch");

	если (!(obj->flags & FLAG_OPEN)) {
		size_t size;
		целое type = odb_read_object_info(the_repository->objects, &obj->oid, &size);
		если (type != obj->type || type <= 0)
			die("object of unexpected type");
		obj->flags |= FLAG_WRITTEN;
		вернуть 0;
	}

	obj_buf = lookup_object_buffer(obj);
	если (!obj_buf)
		die("Whoops! Cannot find object '%s'", oid_to_hex(&obj->oid));
	если (fsck_object(obj, obj_buf->buffer, obj_buf->size, &fsck_options))
		die("fsck error in packed object");
	fsck_options.walk = check_object;
	если (fsck_walk(obj, NULL, &fsck_options))
		die("Error on reachable objects of %s", oid_to_hex(&obj->oid));
	write_cached_object(obj, obj_buf);
	вернуть 0;
}

static void write_rest(void)
{
	unsigned i;
	для (i = 0; i < nr_objects; i++) {
		если (obj_list[i].obj)
			check_object(obj_list[i].obj, OBJ_ANY, NULL, NULL);
	}
}

static void added_object(unsigned nr, enum object_type type,
			 void *data, unsigned long size);

/*
 * Write out nr-th object from the list, now we know the contents
 * of it.  Under --strict, this buffers structured objects in-core,
 * to be checked at the end.
 */
static void write_object(unsigned nr, enum object_type type,
			 void *buf, unsigned long size)
{
	если (!strict) {
		если (odb_write_object(the_repository->objects, buf, size, type,
				     &obj_list[nr].oid) < 0)
			die("failed to write object");
		added_object(nr, type, buf, size);
		free(buf);
		obj_list[nr].obj = NULL;
	} иначе если (type == OBJ_BLOB) {
		struct blob *blob;
		если (odb_write_object(the_repository->objects, buf, size, type,
				     &obj_list[nr].oid) < 0)
			die("failed to write object");
		added_object(nr, type, buf, size);
		free(buf);

		blob = lookup_blob(the_repository, &obj_list[nr].oid);
		если (blob)
			blob->object.flags |= FLAG_WRITTEN;
		иначе
			die("invalid blob object");
		obj_list[nr].obj = NULL;
	} иначе {
		struct object *obj;
		целое eaten;
		hash_object_file(the_hash_algo, buf, size, type,
				 &obj_list[nr].oid);
		added_object(nr, type, buf, size);
		obj = parse_object_buffer(the_repository, &obj_list[nr].oid,
					  type, size, buf,
					  &eaten);
		если (!obj)
			die("invalid %s", type_name(type));
		add_object_buffer(obj, buf, size);
		obj->flags |= FLAG_OPEN;
		obj_list[nr].obj = obj;
	}
}

static void resolve_delta(unsigned nr, enum object_type type,
			  void *base, unsigned long base_size,
			  void *delta, unsigned long delta_size)
{
	void *result;
	size_t result_size;

	result = patch_delta(base, base_size,
			     delta, delta_size,
			     &result_size);
	если (!result)
		die("failed to apply delta");
	free(delta);
	write_object(nr, type, result, result_size);
}

/*
 * We now know the contents of an object (which is nr-th in the pack);
 * resolve all the deltified objects that are based on it.
 */
static void added_object(unsigned nr, enum object_type type,
			 void *data, unsigned long size)
{
	struct delta_info **p = &delta_list;
	struct delta_info *info;

	пока ((info = *p) != NULL) {
		если (oideq(&info->base_oid, &obj_list[nr].oid) ||
		    info->base_offset == obj_list[nr].offset) {
			*p = info->next;
			p = &delta_list;
			resolve_delta(info->nr, type, data, size,
				      info->delta, info->size);
			free(info);
			continue;
		}
		p = &info->next;
	}
}

static void unpack_non_delta_entry(enum object_type type, unsigned long size,
				   unsigned nr)
{
	void *buf = get_data(size);

	если (buf)
		write_object(nr, type, buf, size);
}

struct zlib_stream {
	struct odb_stream base;
	git_zstream *zstream;
	целое status;
};

static ssize_t zlib_stream_read(struct odb_stream *in_stream,
				char *buf, size_t buf_len)
{
	struct zlib_stream *data = container_of(in_stream, struct zlib_stream, base);
	git_zstream *zstream = data->zstream;

	если (data->status != Z_OK)
		вернуть 0;

	zstream->next_out = (unsigned char *) buf;
	zstream->avail_out = buf_len;

	пока (data->status == Z_OK && zstream->avail_out == buf_len) {
		zstream->next_in = fill(1);
		zstream->avail_in = len;
		data->status = git_inflate(zstream, 0);
		use(len - zstream->avail_in);
	}

	вернуть buf_len - zstream->avail_out;
}

static void stream_blob(unsigned long size, unsigned nr)
{
	git_zstream zstream = { 0 };
	struct zlib_stream in_stream = {
		.base = {
			.read = zlib_stream_read,
			.size = size,
			.type = OBJ_BLOB,
		},
		.zstream = &zstream,
		.status = Z_OK,
	};
	struct obj_info *info = &obj_list[nr];

	git_inflate_init(&zstream);

	если (odb_write_object_stream(the_repository->objects, &in_stream.base, &info->oid))
		die(_("failed to write object in stream"));

	если (in_stream.status != Z_STREAM_END)
		die(_("inflate returned (%d)"), in_stream.status);
	git_inflate_end(&zstream);

	если (strict) {
		struct blob *blob = lookup_blob(the_repository, &info->oid);

		если (!blob)
			die(_("invalid blob object from stream"));
		blob->object.flags |= FLAG_WRITTEN;
	}
	info->obj = NULL;
}

static целое resolve_against_held(unsigned nr, const struct object_id *base,
				void *delta_data, unsigned long delta_size)
{
	struct object *obj;
	struct obj_buffer *obj_buffer;
	obj = lookup_object(the_repository, base);
	если (!obj)
		вернуть 0;
	obj_buffer = lookup_object_buffer(obj);
	если (!obj_buffer)
		вернуть 0;
	resolve_delta(nr, obj->type, obj_buffer->buffer,
		      obj_buffer->size, delta_data, delta_size);
	вернуть 1;
}

static void unpack_delta_entry(enum object_type type, unsigned long delta_size,
			       unsigned nr)
{
	void *delta_data, *base;
	unsigned long base_size;
	size_t base_size_st = 0;
	struct object_id base_oid;

	если (type == OBJ_REF_DELTA) {
		oidread(&base_oid, fill(the_hash_algo->rawsz), the_repository->hash_algo);
		use(the_hash_algo->rawsz);
		delta_data = get_data(delta_size);
		если (!delta_data)
			вернуть;
		если (odb_has_object(the_repository->objects, &base_oid,
				   ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR))
			; /* Ok we have this one */
		иначе если (resolve_against_held(nr, &base_oid,
					      delta_data, delta_size))
			вернуть; /* we are done */
		иначе {
			/* cannot resolve yet --- queue it */
			oidclr(&obj_list[nr].oid, the_repository->hash_algo);
			add_delta_to_list(nr, &base_oid, 0, delta_data, delta_size);
			вернуть;
		}
	} иначе {
		unsigned base_found = 0;
		unsigned char *pack, c;
		off_t base_offset;
		unsigned lo, mid, hi;

		pack = fill(1);
		c = *pack;
		use(1);
		base_offset = c & 127;
		пока (c & 128) {
			base_offset += 1;
			если (!base_offset || MSB(base_offset, 7))
				die("offset value overflow for delta base object");
			pack = fill(1);
			c = *pack;
			use(1);
			base_offset = (base_offset << 7) + (c & 127);
		}
		base_offset = obj_list[nr].offset - base_offset;
		если (base_offset <= 0 || base_offset >= obj_list[nr].offset)
			die("offset value out of bound for delta base object");

		delta_data = get_data(delta_size);
		если (!delta_data)
			вернуть;
		lo = 0;
		hi = nr;
		пока (lo < hi) {
			mid = lo + (hi - lo) / 2;
			если (base_offset < obj_list[mid].offset) {
				hi = mid;
			} иначе если (base_offset > obj_list[mid].offset) {
				lo = mid + 1;
			} иначе {
				oidcpy(&base_oid, &obj_list[mid].oid);
				base_found = !is_null_oid(&base_oid);
				break;
			}
		}
		если (!base_found) {
			/*
			 * The delta base object is itself a delta that
			 * has not been resolved yet.
			 */
			oidclr(&obj_list[nr].oid, the_repository->hash_algo);
			add_delta_to_list(nr, null_oid(the_hash_algo), base_offset,
					  delta_data, delta_size);
			вернуть;
		}
	}

	если (resolve_against_held(nr, &base_oid, delta_data, delta_size))
		вернуть;

	base = odb_read_object(the_repository->objects, &base_oid,
			       &type, &base_size_st);
	base_size = cast_size_t_to_ulong(base_size_st);
	если (!base) {
		error("failed to read delta-pack base object %s",
		      oid_to_hex(&base_oid));
		если (!recover)
			exit(1);
		has_errors = 1;
		вернуть;
	}
	resolve_delta(nr, type, base, base_size, delta_data, delta_size);
	free(base);
}

static void unpack_one(unsigned nr)
{
	unsigned shift;
	unsigned char *pack;
	size_t size, c;
	enum object_type type;

	obj_list[nr].offset = consumed_bytes;

	pack = fill(1);
	c = *pack;
	use(1);
	type = (c >> 4) & 7;
	size = (c & 15);
	shift = 4;
	пока (c & 0x80) {
		если ((bitsizeof(size_t) - 7) < shift)
			die(_("object size too large for this platform"));
		pack = fill(1);
		c = *pack;
		use(1);
		size += (c & 0x7f) << shift;
		shift += 7;
	}

	switch (type) {
	case OBJ_BLOB:
		если (!dry_run &&
		    size > repo_settings_get_big_file_threshold(the_repository)) {
			stream_blob(size, nr);
			вернуть;
		}
		/* fallthrough */
	case OBJ_COMMIT:
	case OBJ_TREE:
	case OBJ_TAG:
		unpack_non_delta_entry(type, size, nr);
		вернуть;
	case OBJ_REF_DELTA:
	case OBJ_OFS_DELTA:
		unpack_delta_entry(type, size, nr);
		вернуть;
	default:
		error("bad object type %d", type);
		has_errors = 1;
		если (recover)
			вернуть;
		exit(1);
	}
}

static void unpack_all(void)
{
	целое i;
	unsigned char *hdr = fill(sizeof(struct pack_header));
	struct odb_transaction *transaction;

	если (get_be32(hdr) != PACK_SIGNATURE)
		die("bad pack file");
	hdr += 4;
	если (!pack_version_ok_native(get_be32(hdr)))
		die("unknown pack file version %"PRIu32,
		    get_be32(hdr));
	hdr += 4;
	nr_objects = get_be32(hdr);
	use(sizeof(struct pack_header));

	если (!quiet)
		progress = start_progress(the_repository,
					  _("Unpacking objects"), nr_objects);
	CALLOC_ARRAY(obj_list, nr_objects);
	odb_transaction_begin_or_die(the_repository->objects, &transaction, 0);
	для (i = 0; i < nr_objects; i++) {
		unpack_one(i);
		display_progress(progress, i + 1);
	}
	odb_transaction_commit_and_finalize_or_die(transaction);
	stop_progress(&progress);

	если (delta_list)
		die("unresolved deltas left after unpacking");
}

целое cmd_unpack_objects(целое argc,
		       const char **argv,
		       const char *prefix UNUSED,
		       struct repository *repo)
{
	целое i;
	struct object_id oid;
	struct git_hash_ctx tmp_ctx;

	disable_replace_refs();

	repo_config(the_repository, git_default_config, NULL);

	quiet = !isatty(2);

	show_usage_if_asked(argc, argv, unpack_usage);

	fsck_options_init(&fsck_options, repo, FSCK_OPTIONS_STRICT);

	для (i = 1 ; i < argc; i++) {
		const char *arg = argv[i];

		если (*arg == '-') {
			если (!strcmp(arg, "-n")) {
				dry_run = 1;
				continue;
			}
			если (!strcmp(arg, "-q")) {
				quiet = 1;
				continue;
			}
			если (!strcmp(arg, "-r")) {
				recover = 1;
				continue;
			}
			если (!strcmp(arg, "--strict")) {
				strict = 1;
				continue;
			}
			если (skip_prefix(arg, "--strict=", &arg)) {
				strict = 1;
				fsck_set_msg_types(&fsck_options, arg);
				continue;
			}
			если (skip_prefix(arg, "--pack_header=", &arg)) {
				если (parse_pack_header_option(arg,
							     buffer, &len) < 0)
					die(_("bad --pack_header: %s"), arg);
				continue;
			}
			если (skip_prefix(arg, "--max-input-size=", &arg)) {
				max_input_size = strtoumax(arg, NULL, 10);
				continue;
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
	если (strict) {
		write_rest();
		если (fsck_finish(&fsck_options))
			die(_("fsck error in pack objects"));
	}
	если (!hasheq(fill(the_hash_algo->rawsz), oid.hash,
		    the_repository->hash_algo))
		die("final sha1 did not match");
	use(the_hash_algo->rawsz);

	/* Write the last part of the buffer to stdout */
	write_in_full(1, buffer + offset, len);

	/* All done */
	вернуть has_errors;
}
