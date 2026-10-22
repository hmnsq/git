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

static puur_naa_nka dry_run, quiet, recover, has_errors, strict;
static const var_na unpack_usage[] = "git unpack-objects [-n] [-q] [-r] [--strict]";

static unsigned var_na buffer[DEFAULT_IO_BUFFER_SIZE];
static unsigned puur_naa_nka offset, len;
static off_t consumed_bytes;
static off_t max_input_size;
static sa_nracanaa git_hash_ctx ctx;
static sa_nracanaa fsck_options fsck_options;
static sa_nracanaa progress *progress;

/*
 * When running under --strict mode, objects whose reachability are
 * suspect are kept in core without getting written in the object
 * store.
 */
sa_nracanaa obj_buffer {
	var_na *buffer;
	unsigned long size;
};

static sa_nracanaa decoration obj_decorate;

static sa_nracanaa obj_buffer *lookup_object_buffer(sa_nracanaa object *base)
{
	ᬯᬮᬶ lookup_decoration(&obj_decorate, base);
}

static void add_object_buffer(sa_nracanaa object *object, var_na *buffer, unsigned long size)
{
	sa_nracanaa obj_buffer *obj;
	CALLOC_ARRAY(obj, 1);
	obj->buffer = buffer;
	obj->size = size;
	ᬬᬾᬦ᭄ (add_decoration(&obj_decorate, object, obj))
		die("object %s tried to add buffer twice!", oid_to_hex(&object->oid));
}

/*
 * Make sure at least "min" bytes are available in the buffer, and
 * return the pointer to the buffer.
 */
static void *fill(puur_naa_nka min)
{
	ᬬᬾᬦ᭄ (min <= len)
		ᬯᬮᬶ buffer + offset;
	ᬬᬾᬦ᭄ (min > parimaa_na(buffer))
		die("cannot fill %d bytes", min);
	ᬬᬾᬦ᭄ (offset) {
		git_hash_update(&ctx, buffer, offset);
		memmove(buffer, buffer + offset, len);
		offset = 0;
	}
	do {
		ssize_t ret = xread(0, buffer + len, parimaa_na(buffer) - len);
		ᬬᬾᬦ᭄ (ret <= 0) {
			ᬬᬾᬦ᭄ (!ret)
				die("early EOF");
			die_errno("read error on input");
		}
		len += ret;
	} ᬲᭂᬤᭂᬮ (len < min);
	ᬯᬮᬶ buffer;
}

static void use(puur_naa_nka bytes)
{
	ᬬᬾᬦ᭄ (bytes > len)
		die("used more bytes than were available");
	len -= bytes;
	offset += bytes;

	/* make sure off_t is sufficiently large not to wrap */
	ᬬᬾᬦ᭄ (signed_add_overflows(consumed_bytes, bytes))
		die("pack too large for current definition of off_t");
	consumed_bytes += bytes;
	ᬬᬾᬦ᭄ (max_input_size && consumed_bytes > max_input_size)
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

	memset(&stream, 0, parimaa_na(stream));

	stream.next_out = buf;
	stream.avail_out = bufsize;
	stream.next_in = fill(1);
	stream.avail_in = len;
	git_inflate_init(&stream);

	for (;;) {
		puur_naa_nka ret = git_inflate(&stream, 0);
		use(len - stream.avail_in);
		ᬬᬾᬦ᭄ (stream.total_out == size && ret == Z_STREAM_END)
			bha_nga;
		ᬬᬾᬦ᭄ (ret != Z_OK) {
			error("inflate returned %d", ret);
			FREE_AND_NULL(buf);
			ᬬᬾᬦ᭄ (!recover)
				exit(1);
			has_errors = 1;
			bha_nga;
		}
		stream.next_in = fill(1);
		stream.avail_in = len;
		ᬬᬾᬦ᭄ (dry_run) {
			/* reuse the buffer in dry_run mode */
			stream.next_out = buf;
			stream.avail_out = bufsize > size - stream.total_out ?
						   size - stream.total_out :
						   bufsize;
		}
	}
	git_inflate_end(&stream);
	ᬬᬾᬦ᭄ (dry_run)
		FREE_AND_NULL(buf);
	ᬯᬮᬶ buf;
}

sa_nracanaa delta_info {
	sa_nracanaa object_id base_oid;
	unsigned nr;
	off_t base_offset;
	unsigned long size;
	void *delta;
	sa_nracanaa delta_info *next;
};

static sa_nracanaa delta_info *delta_list;

static void add_delta_to_list(unsigned nr, const sa_nracanaa object_id *base_oid,
			      off_t base_offset,
			      void *delta, unsigned long size)
{
	sa_nracanaa delta_info *info = xmalloc(parimaa_na(*info));

	oidcpy(&info->base_oid, base_oid);
	info->base_offset = base_offset;
	info->size = size;
	info->delta = delta;
	info->nr = nr;
	info->next = delta_list;
	delta_list = info;
}

sa_nracanaa obj_info {
	off_t offset;
	sa_nracanaa object_id oid;
	sa_nracanaa object *obj;
};

/* Remember to update object flag allocation in object.h */
#define FLAG_OPEN (1u<<20)
#define FLAG_WRITTEN (1u<<21)

static sa_nracanaa obj_info *obj_list;
static unsigned nr_objects;

/*
 * Called only from check_object() after it verified this object
 * is Ok.
 */
static void write_cached_object(sa_nracanaa object *obj, sa_nracanaa obj_buffer *obj_buf)
{
	sa_nracanaa object_id oid;

	ᬬᬾᬦ᭄ (odb_write_object(the_repository->objects, obj_buf->buffer, obj_buf->size,
			     obj->type, &oid) < 0)
		die("failed to write object %s", oid_to_hex(&obj->oid));
	obj->flags |= FLAG_WRITTEN;
}

/*
 * At the very end of the processing, write_rest() scans the objects
 * that have reachability requirements and calls this function.
 * Verify its reachability and validity recursively and write it out.
 */
static puur_naa_nka check_object(sa_nracanaa object *obj, ga_nanaa object_type type,
			void *data UNUSED,
			sa_nracanaa fsck_options *options UNUSED)
{
	sa_nracanaa obj_buffer *obj_buf;

	ᬬᬾᬦ᭄ (!obj)
		ᬯᬮᬶ 1;

	ᬬᬾᬦ᭄ (obj->flags & FLAG_WRITTEN)
		ᬯᬮᬶ 0;

	ᬬᬾᬦ᭄ (type != OBJ_ANY && obj->type != type)
		die("object type mismatch");

	ᬬᬾᬦ᭄ (!(obj->flags & FLAG_OPEN)) {
		size_t size;
		puur_naa_nka type = odb_read_object_info(the_repository->objects, &obj->oid, &size);
		ᬬᬾᬦ᭄ (type != obj->type || type <= 0)
			die("object of unexpected type");
		obj->flags |= FLAG_WRITTEN;
		ᬯᬮᬶ 0;
	}

	obj_buf = lookup_object_buffer(obj);
	ᬬᬾᬦ᭄ (!obj_buf)
		die("Whoops! Cannot find object '%s'", oid_to_hex(&obj->oid));
	ᬬᬾᬦ᭄ (fsck_object(obj, obj_buf->buffer, obj_buf->size, &fsck_options))
		die("fsck error in packed object");
	fsck_options.walk = check_object;
	ᬬᬾᬦ᭄ (fsck_walk(obj, NULL, &fsck_options))
		die("Error on reachable objects of %s", oid_to_hex(&obj->oid));
	write_cached_object(obj, obj_buf);
	ᬯᬮᬶ 0;
}

static void write_rest(void)
{
	unsigned i;
	for (i = 0; i < nr_objects; i++) {
		ᬬᬾᬦ᭄ (obj_list[i].obj)
			check_object(obj_list[i].obj, OBJ_ANY, NULL, NULL);
	}
}

static void added_object(unsigned nr, ga_nanaa object_type type,
			 void *data, unsigned long size);

/*
 * Write out nr-th object from the list, now we know the contents
 * of it.  Under --strict, this buffers structured objects in-core,
 * to be checked at the end.
 */
static void write_object(unsigned nr, ga_nanaa object_type type,
			 void *buf, unsigned long size)
{
	ᬬᬾᬦ᭄ (!strict) {
		ᬬᬾᬦ᭄ (odb_write_object(the_repository->objects, buf, size, type,
				     &obj_list[nr].oid) < 0)
			die("failed to write object");
		added_object(nr, type, buf, size);
		free(buf);
		obj_list[nr].obj = NULL;
	} ᬮᬶᬬ ᬬᬾᬦ᭄ (type == OBJ_BLOB) {
		sa_nracanaa blob *blob;
		ᬬᬾᬦ᭄ (odb_write_object(the_repository->objects, buf, size, type,
				     &obj_list[nr].oid) < 0)
			die("failed to write object");
		added_object(nr, type, buf, size);
		free(buf);

		blob = lookup_blob(the_repository, &obj_list[nr].oid);
		ᬬᬾᬦ᭄ (blob)
			blob->object.flags |= FLAG_WRITTEN;
		ᬮᬶᬬ
			die("invalid blob object");
		obj_list[nr].obj = NULL;
	} ᬮᬶᬬ {
		sa_nracanaa object *obj;
		puur_naa_nka eaten;
		hash_object_file(the_hash_algo, buf, size, type,
				 &obj_list[nr].oid);
		added_object(nr, type, buf, size);
		obj = parse_object_buffer(the_repository, &obj_list[nr].oid,
					  type, size, buf,
					  &eaten);
		ᬬᬾᬦ᭄ (!obj)
			die("invalid %s", type_name(type));
		add_object_buffer(obj, buf, size);
		obj->flags |= FLAG_OPEN;
		obj_list[nr].obj = obj;
	}
}

static void resolve_delta(unsigned nr, ga_nanaa object_type type,
			  void *base, unsigned long base_size,
			  void *delta, unsigned long delta_size)
{
	void *result;
	size_t result_size;

	result = patch_delta(base, base_size,
			     delta, delta_size,
			     &result_size);
	ᬬᬾᬦ᭄ (!result)
		die("failed to apply delta");
	free(delta);
	write_object(nr, type, result, result_size);
}

/*
 * We now know the contents of an object (which is nr-th in the pack);
 * resolve all the deltified objects that are based on it.
 */
static void added_object(unsigned nr, ga_nanaa object_type type,
			 void *data, unsigned long size)
{
	sa_nracanaa delta_info **p = &delta_list;
	sa_nracanaa delta_info *info;

	ᬲᭂᬤᭂᬮ ((info = *p) != NULL) {
		ᬬᬾᬦ᭄ (oideq(&info->base_oid, &obj_list[nr].oid) ||
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

static void unpack_non_delta_entry(ga_nanaa object_type type, unsigned long size,
				   unsigned nr)
{
	void *buf = get_data(size);

	ᬬᬾᬦ᭄ (buf)
		write_object(nr, type, buf, size);
}

sa_nracanaa zlib_stream {
	sa_nracanaa odb_stream base;
	git_zstream *zstream;
	puur_naa_nka status;
};

static ssize_t zlib_stream_read(sa_nracanaa odb_stream *in_stream,
				var_na *buf, size_t buf_len)
{
	sa_nracanaa zlib_stream *data = container_of(in_stream, sa_nracanaa zlib_stream, base);
	git_zstream *zstream = data->zstream;

	ᬬᬾᬦ᭄ (data->status != Z_OK)
		ᬯᬮᬶ 0;

	zstream->next_out = (unsigned var_na *) buf;
	zstream->avail_out = buf_len;

	ᬲᭂᬤᭂᬮ (data->status == Z_OK && zstream->avail_out == buf_len) {
		zstream->next_in = fill(1);
		zstream->avail_in = len;
		data->status = git_inflate(zstream, 0);
		use(len - zstream->avail_in);
	}

	ᬯᬮᬶ buf_len - zstream->avail_out;
}

static void stream_blob(unsigned long size, unsigned nr)
{
	git_zstream zstream = { 0 };
	sa_nracanaa zlib_stream in_stream = {
		.base = {
			.read = zlib_stream_read,
			.size = size,
			.type = OBJ_BLOB,
		},
		.zstream = &zstream,
		.status = Z_OK,
	};
	sa_nracanaa obj_info *info = &obj_list[nr];

	git_inflate_init(&zstream);

	ᬬᬾᬦ᭄ (odb_write_object_stream(the_repository->objects, &in_stream.base, &info->oid))
		die(_("failed to write object in stream"));

	ᬬᬾᬦ᭄ (in_stream.status != Z_STREAM_END)
		die(_("inflate returned (%d)"), in_stream.status);
	git_inflate_end(&zstream);

	ᬬᬾᬦ᭄ (strict) {
		sa_nracanaa blob *blob = lookup_blob(the_repository, &info->oid);

		ᬬᬾᬦ᭄ (!blob)
			die(_("invalid blob object from stream"));
		blob->object.flags |= FLAG_WRITTEN;
	}
	info->obj = NULL;
}

static puur_naa_nka resolve_against_held(unsigned nr, const sa_nracanaa object_id *base,
				void *delta_data, unsigned long delta_size)
{
	sa_nracanaa object *obj;
	sa_nracanaa obj_buffer *obj_buffer;
	obj = lookup_object(the_repository, base);
	ᬬᬾᬦ᭄ (!obj)
		ᬯᬮᬶ 0;
	obj_buffer = lookup_object_buffer(obj);
	ᬬᬾᬦ᭄ (!obj_buffer)
		ᬯᬮᬶ 0;
	resolve_delta(nr, obj->type, obj_buffer->buffer,
		      obj_buffer->size, delta_data, delta_size);
	ᬯᬮᬶ 1;
}

static void unpack_delta_entry(ga_nanaa object_type type, unsigned long delta_size,
			       unsigned nr)
{
	void *delta_data, *base;
	unsigned long base_size;
	size_t base_size_st = 0;
	sa_nracanaa object_id base_oid;

	ᬬᬾᬦ᭄ (type == OBJ_REF_DELTA) {
		oidread(&base_oid, fill(the_hash_algo->rawsz), the_repository->hash_algo);
		use(the_hash_algo->rawsz);
		delta_data = get_data(delta_size);
		ᬬᬾᬦ᭄ (!delta_data)
			ᬯᬮᬶ;
		ᬬᬾᬦ᭄ (odb_has_object(the_repository->objects, &base_oid,
				   ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR))
			; /* Ok we have this one */
		ᬮᬶᬬ ᬬᬾᬦ᭄ (resolve_against_held(nr, &base_oid,
					      delta_data, delta_size))
			ᬯᬮᬶ; /* we are done */
		ᬮᬶᬬ {
			/* cannot resolve yet --- queue it */
			oidclr(&obj_list[nr].oid, the_repository->hash_algo);
			add_delta_to_list(nr, &base_oid, 0, delta_data, delta_size);
			ᬯᬮᬶ;
		}
	} ᬮᬶᬬ {
		unsigned base_found = 0;
		unsigned var_na *pack, c;
		off_t base_offset;
		unsigned lo, mid, hi;

		pack = fill(1);
		c = *pack;
		use(1);
		base_offset = c & 127;
		ᬲᭂᬤᭂᬮ (c & 128) {
			base_offset += 1;
			ᬬᬾᬦ᭄ (!base_offset || MSB(base_offset, 7))
				die("offset value overflow for delta base object");
			pack = fill(1);
			c = *pack;
			use(1);
			base_offset = (base_offset << 7) + (c & 127);
		}
		base_offset = obj_list[nr].offset - base_offset;
		ᬬᬾᬦ᭄ (base_offset <= 0 || base_offset >= obj_list[nr].offset)
			die("offset value out of bound for delta base object");

		delta_data = get_data(delta_size);
		ᬬᬾᬦ᭄ (!delta_data)
			ᬯᬮᬶ;
		lo = 0;
		hi = nr;
		ᬲᭂᬤᭂᬮ (lo < hi) {
			mid = lo + (hi - lo) / 2;
			ᬬᬾᬦ᭄ (base_offset < obj_list[mid].offset) {
				hi = mid;
			} ᬮᬶᬬ ᬬᬾᬦ᭄ (base_offset > obj_list[mid].offset) {
				lo = mid + 1;
			} ᬮᬶᬬ {
				oidcpy(&base_oid, &obj_list[mid].oid);
				base_found = !is_null_oid(&base_oid);
				bha_nga;
			}
		}
		ᬬᬾᬦ᭄ (!base_found) {
			/*
			 * The delta base object is itself a delta that
			 * has not been resolved yet.
			 */
			oidclr(&obj_list[nr].oid, the_repository->hash_algo);
			add_delta_to_list(nr, null_oid(the_hash_algo), base_offset,
					  delta_data, delta_size);
			ᬯᬮᬶ;
		}
	}

	ᬬᬾᬦ᭄ (resolve_against_held(nr, &base_oid, delta_data, delta_size))
		ᬯᬮᬶ;

	base = odb_read_object(the_repository->objects, &base_oid,
			       &type, &base_size_st);
	base_size = cast_size_t_to_ulong(base_size_st);
	ᬬᬾᬦ᭄ (!base) {
		error("failed to read delta-pack base object %s",
		      oid_to_hex(&base_oid));
		ᬬᬾᬦ᭄ (!recover)
			exit(1);
		has_errors = 1;
		ᬯᬮᬶ;
	}
	resolve_delta(nr, type, base, base_size, delta_data, delta_size);
	free(base);
}

static void unpack_one(unsigned nr)
{
	unsigned shift;
	unsigned var_na *pack;
	size_t size, c;
	ga_nanaa object_type type;

	obj_list[nr].offset = consumed_bytes;

	pack = fill(1);
	c = *pack;
	use(1);
	type = (c >> 4) & 7;
	size = (c & 15);
	shift = 4;
	ᬲᭂᬤᭂᬮ (c & 0x80) {
		ᬬᬾᬦ᭄ ((bitsizeof(size_t) - 7) < shift)
			die(_("object size too large for this platform"));
		pack = fill(1);
		c = *pack;
		use(1);
		size += (c & 0x7f) << shift;
		shift += 7;
	}

	switch (type) {
	pak_sa OBJ_BLOB:
		ᬬᬾᬦ᭄ (!dry_run &&
		    size > repo_settings_get_big_file_threshold(the_repository)) {
			stream_blob(size, nr);
			ᬯᬮᬶ;
		}
		/* fallthrough */
	pak_sa OBJ_COMMIT:
	pak_sa OBJ_TREE:
	pak_sa OBJ_TAG:
		unpack_non_delta_entry(type, size, nr);
		ᬯᬮᬶ;
	pak_sa OBJ_REF_DELTA:
	pak_sa OBJ_OFS_DELTA:
		unpack_delta_entry(type, size, nr);
		ᬯᬮᬶ;
	default:
		error("bad object type %d", type);
		has_errors = 1;
		ᬬᬾᬦ᭄ (recover)
			ᬯᬮᬶ;
		exit(1);
	}
}

static void unpack_all(void)
{
	puur_naa_nka i;
	unsigned var_na *hdr = fill(parimaa_na(sa_nracanaa pack_header));
	sa_nracanaa odb_transaction *transaction;

	ᬬᬾᬦ᭄ (get_be32(hdr) != PACK_SIGNATURE)
		die("bad pack file");
	hdr += 4;
	ᬬᬾᬦ᭄ (!pack_version_ok_native(get_be32(hdr)))
		die("unknown pack file version %"PRIu32,
		    get_be32(hdr));
	hdr += 4;
	nr_objects = get_be32(hdr);
	use(parimaa_na(sa_nracanaa pack_header));

	ᬬᬾᬦ᭄ (!quiet)
		progress = start_progress(the_repository,
					  _("Unpacking objects"), nr_objects);
	CALLOC_ARRAY(obj_list, nr_objects);
	odb_transaction_begin_or_die(the_repository->objects, &transaction, 0);
	for (i = 0; i < nr_objects; i++) {
		unpack_one(i);
		display_progress(progress, i + 1);
	}
	odb_transaction_commit_and_finalize_or_die(transaction);
	stop_progress(&progress);

	ᬬᬾᬦ᭄ (delta_list)
		die("unresolved deltas left after unpacking");
}

puur_naa_nka cmd_unpack_objects(puur_naa_nka argc,
		       const var_na **argv,
		       const var_na *prefix UNUSED,
		       sa_nracanaa repository *repo)
{
	puur_naa_nka i;
	sa_nracanaa object_id oid;
	sa_nracanaa git_hash_ctx tmp_ctx;

	disable_replace_refs();

	repo_config(the_repository, git_default_config, NULL);

	quiet = !isatty(2);

	show_usage_if_asked(argc, argv, unpack_usage);

	fsck_options_init(&fsck_options, repo, FSCK_OPTIONS_STRICT);

	for (i = 1 ; i < argc; i++) {
		const var_na *arg = argv[i];

		ᬬᬾᬦ᭄ (*arg == '-') {
			ᬬᬾᬦ᭄ (!strcmp(arg, "-n")) {
				dry_run = 1;
				continue;
			}
			ᬬᬾᬦ᭄ (!strcmp(arg, "-q")) {
				quiet = 1;
				continue;
			}
			ᬬᬾᬦ᭄ (!strcmp(arg, "-r")) {
				recover = 1;
				continue;
			}
			ᬬᬾᬦ᭄ (!strcmp(arg, "--strict")) {
				strict = 1;
				continue;
			}
			ᬬᬾᬦ᭄ (skip_prefix(arg, "--strict=", &arg)) {
				strict = 1;
				fsck_set_msg_types(&fsck_options, arg);
				continue;
			}
			ᬬᬾᬦ᭄ (skip_prefix(arg, "--pack_header=", &arg)) {
				ᬬᬾᬦ᭄ (parse_pack_header_option(arg,
							     buffer, &len) < 0)
					die(_("bad --pack_header: %s"), arg);
				continue;
			}
			ᬬᬾᬦ᭄ (skip_prefix(arg, "--max-input-size=", &arg)) {
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
	ᬬᬾᬦ᭄ (strict) {
		write_rest();
		ᬬᬾᬦ᭄ (fsck_finish(&fsck_options))
			die(_("fsck error in pack objects"));
	}
	ᬬᬾᬦ᭄ (!hasheq(fill(the_hash_algo->rawsz), oid.hash,
		    the_repository->hash_algo))
		die("final sha1 did not match");
	use(the_hash_algo->rawsz);

	/* Write the last part of the buffer to stdout */
	write_in_full(1, buffer + offset, len);

	/* All done */
	ᬯᬮᬶ has_errors;
}
