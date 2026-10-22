#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#অন্তর্ভাৱ "builtin.h"
#অন্তর্ভাৱ "config.h"
#অন্তর্ভাৱ "environment.h"
#অন্তর্ভাৱ "gettext.h"
#অন্তর্ভাৱ "git-zlib.h"
#অন্তর্ভাৱ "hex.h"
#অন্তর্ভাৱ "object-file.h"
#অন্তর্ভাৱ "odb.h"
#অন্তর্ভাৱ "odb/streaming.h"
#অন্তর্ভাৱ "odb/transaction.h"
#অন্তর্ভাৱ "object.h"
#অন্তর্ভাৱ "delta.h"
#অন্তর্ভাৱ "pack.h"
#অন্তর্ভাৱ "blob.h"
#অন্তর্ভাৱ "replace-object.h"
#অন্তর্ভাৱ "strbuf.h"
#অন্তর্ভাৱ "progress.h"
#অন্তর্ভাৱ "decorate.h"
#অন্তর্ভাৱ "fsck.h"
#অন্তর্ভাৱ "packfile.h"

স্থির পূর্ণাঙ্ক dry_run, quiet, recover, has_errors, strict;
স্থির ধ্রুৱ ৱর্ণ unpack_usage[] = "git unpack-objects [-n] [-q] [-r] [--strict]";

স্থির unsigned ৱর্ণ buffer[DEFAULT_IO_BUFFER_SIZE];
স্থির unsigned পূর্ণাঙ্ক offset, len;
স্থির off_t consumed_bytes;
স্থির off_t max_প্ৰবেশ_size;
স্থির সংরচনা git_hash_ctx ctx;
স্থির সংরচনা fsck_options fsck_options;
স্থির সংরচনা progress *progress;

/*
 * When running under --strict mode, objects whose reachability are
 * suspect are kept in core without getting written in the object
 * store.
 */
সংরচনা obj_buffer {
	ৱর্ণ *buffer;
	unsigned long size;
};

স্থির সংরচনা decoration obj_decorate;

স্থির সংরচনা obj_buffer *lookup_object_buffer(সংরচনা object *base)
{
	ঘূৰাই lookup_decoration(&obj_decorate, base);
}

স্থির শূন্য add_object_buffer(সংরচনা object *object, ৱর্ণ *buffer, unsigned long size)
{
	সংরচনা obj_buffer *obj;
	CALLOC_ARRAY(obj, 1);
	obj->buffer = buffer;
	obj->size = size;
	যদি (add_decoration(&obj_decorate, object, obj))
		die("object %s tried to add buffer twice!", oid_to_hex(&object->oid));
}

/*
 * Make sure at least "min" bytes are available in the buffer, and
 * return the pointer to the buffer.
 */
স্থির শূন্য *fill(পূর্ণাঙ্ক min)
{
	যদি (min <= len)
		ঘূৰাই buffer + offset;
	যদি (min > পরিমাণ(buffer))
		die("cannot fill %d bytes", min);
	যদি (offset) {
		git_hash_update(&ctx, buffer, offset);
		memmove(buffer, buffer + offset, len);
		offset = 0;
	}
	কৰা {
		ssize_t ret = xread(0, buffer + len, পরিমাণ(buffer) - len);
		যদি (ret <= 0) {
			যদি (!ret)
				die("early EOF");
			die_errno("read error on input");
		}
		len += ret;
	} যেতিয়ালৈ (len < min);
	ঘূৰাই buffer;
}

স্থির শূন্য use(পূর্ণাঙ্ক bytes)
{
	যদি (bytes > len)
		die("used more bytes than were available");
	len -= bytes;
	offset += bytes;

	/* make sure off_t is sufficiently large not to wrap */
	যদি (signed_add_overflows(consumed_bytes, bytes))
		die("pack too large for current definition of off_t");
	consumed_bytes += bytes;
	যদি (max_প্ৰবেশ_size && consumed_bytes > max_প্ৰবেশ_size)
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
স্থির শূন্য *get_data(unsigned long size)
{
	git_zstream stream;
	unsigned long bufsize = dry_run && size > 8192 ? 8192 : size;
	শূন্য *buf = xmallocz(bufsize);

	memset(&stream, 0, পরিমাণ(stream));

	stream.next_out = buf;
	stream.avail_out = bufsize;
	stream.next_in = fill(1);
	stream.avail_in = len;
	git_inflate_init(&stream);

	বাবে (;;) {
		পূর্ণাঙ্ক ret = git_inflate(&stream, 0);
		use(len - stream.avail_in);
		যদি (stream.total_out == size && ret == Z_STREAM_END)
			ভঙ্গ;
		যদি (ret != Z_OK) {
			error("inflate returned %d", ret);
			FREE_AND_NULL(buf);
			যদি (!recover)
				exit(1);
			has_errors = 1;
			ভঙ্গ;
		}
		stream.next_in = fill(1);
		stream.avail_in = len;
		যদি (dry_run) {
			/* reuse the buffer in dry_run mode */
			stream.next_out = buf;
			stream.avail_out = bufsize > size - stream.total_out ?
						   size - stream.total_out :
						   bufsize;
		}
	}
	git_inflate_end(&stream);
	যদি (dry_run)
		FREE_AND_NULL(buf);
	ঘূৰাই buf;
}

সংরচনা delta_info {
	সংরচনা object_id base_oid;
	unsigned nr;
	off_t base_offset;
	unsigned long size;
	শূন্য *delta;
	সংরচনা delta_info *next;
};

স্থির সংরচনা delta_info *delta_list;

স্থির শূন্য add_delta_to_list(unsigned nr, ধ্রুৱ সংরচনা object_id *base_oid,
			      off_t base_offset,
			      শূন্য *delta, unsigned long size)
{
	সংরচনা delta_info *info = xmalloc(পরিমাণ(*info));

	oidcpy(&info->base_oid, base_oid);
	info->base_offset = base_offset;
	info->size = size;
	info->delta = delta;
	info->nr = nr;
	info->next = delta_list;
	delta_list = info;
}

সংরচনা obj_info {
	off_t offset;
	সংরচনা object_id oid;
	সংরচনা object *obj;
};

/* Remember to update object flag allocation in object.h */
#define FLAG_OPEN (1u<<20)
#define FLAG_WRITTEN (1u<<21)

স্থির সংরচনা obj_info *obj_list;
স্থির unsigned nr_objects;

/*
 * Called only from check_object() after it verified this object
 * is Ok.
 */
স্থির শূন্য write_cached_object(সংরচনা object *obj, সংরচনা obj_buffer *obj_buf)
{
	সংরচনা object_id oid;

	যদি (odb_write_object(the_repository->objects, obj_buf->buffer, obj_buf->size,
			     obj->type, &oid) < 0)
		die("failed to write object %s", oid_to_hex(&obj->oid));
	obj->flags |= FLAG_WRITTEN;
}

/*
 * At the very end of the processing, write_rest() scans the objects
 * that have reachability requirements and calls this function.
 * Verify its reachability and validity recursively and write it out.
 */
স্থির পূর্ণাঙ্ক check_object(সংরচনা object *obj, গণনা object_type type,
			শূন্য *data UNUSED,
			সংরচনা fsck_options *options UNUSED)
{
	সংরচনা obj_buffer *obj_buf;

	যদি (!obj)
		ঘূৰাই 1;

	যদি (obj->flags & FLAG_WRITTEN)
		ঘূৰাই 0;

	যদি (type != OBJ_ANY && obj->type != type)
		die("object type mismatch");

	যদি (!(obj->flags & FLAG_OPEN)) {
		size_t size;
		পূর্ণাঙ্ক type = odb_read_object_info(the_repository->objects, &obj->oid, &size);
		যদি (type != obj->type || type <= 0)
			die("object of unexpected type");
		obj->flags |= FLAG_WRITTEN;
		ঘূৰাই 0;
	}

	obj_buf = lookup_object_buffer(obj);
	যদি (!obj_buf)
		die("Whoops! Cannot find object '%s'", oid_to_hex(&obj->oid));
	যদি (fsck_object(obj, obj_buf->buffer, obj_buf->size, &fsck_options))
		die("fsck error in packed object");
	fsck_options.walk = check_object;
	যদি (fsck_walk(obj, রিক্ত, &fsck_options))
		die("Error on reachable objects of %s", oid_to_hex(&obj->oid));
	write_cached_object(obj, obj_buf);
	ঘূৰাই 0;
}

স্থির শূন্য write_rest(শূন্য)
{
	unsigned i;
	বাবে (i = 0; i < nr_objects; i++) {
		যদি (obj_list[i].obj)
			check_object(obj_list[i].obj, OBJ_ANY, রিক্ত, রিক্ত);
	}
}

স্থির শূন্য added_object(unsigned nr, গণনা object_type type,
			 শূন্য *data, unsigned long size);

/*
 * Write out nr-th object from the list, now we know the contents
 * of it.  Under --strict, this buffers structured objects in-core,
 * to be checked at the end.
 */
স্থির শূন্য write_object(unsigned nr, গণনা object_type type,
			 শূন্য *buf, unsigned long size)
{
	যদি (!strict) {
		যদি (odb_write_object(the_repository->objects, buf, size, type,
				     &obj_list[nr].oid) < 0)
			die("failed to write object");
		added_object(nr, type, buf, size);
		মুক্ত(buf);
		obj_list[nr].obj = রিক্ত;
	} অন্যথা যদি (type == OBJ_BLOB) {
		সংরচনা blob *blob;
		যদি (odb_write_object(the_repository->objects, buf, size, type,
				     &obj_list[nr].oid) < 0)
			die("failed to write object");
		added_object(nr, type, buf, size);
		মুক্ত(buf);

		blob = lookup_blob(the_repository, &obj_list[nr].oid);
		যদি (blob)
			blob->object.flags |= FLAG_WRITTEN;
		অন্যথা
			die("invalid blob object");
		obj_list[nr].obj = রিক্ত;
	} অন্যথা {
		সংরচনা object *obj;
		পূর্ণাঙ্ক eaten;
		hash_object_file(the_hash_algo, buf, size, type,
				 &obj_list[nr].oid);
		added_object(nr, type, buf, size);
		obj = parse_object_buffer(the_repository, &obj_list[nr].oid,
					  type, size, buf,
					  &eaten);
		যদি (!obj)
			die("invalid %s", type_name(type));
		add_object_buffer(obj, buf, size);
		obj->flags |= FLAG_OPEN;
		obj_list[nr].obj = obj;
	}
}

স্থির শূন্য resolve_delta(unsigned nr, গণনা object_type type,
			  শূন্য *base, unsigned long base_size,
			  শূন্য *delta, unsigned long delta_size)
{
	শূন্য *result;
	size_t result_size;

	result = patch_delta(base, base_size,
			     delta, delta_size,
			     &result_size);
	যদি (!result)
		die("failed to apply delta");
	মুক্ত(delta);
	write_object(nr, type, result, result_size);
}

/*
 * We now know the contents of an object (which is nr-th in the pack);
 * resolve all the deltified objects that are based on it.
 */
স্থির শূন্য added_object(unsigned nr, গণনা object_type type,
			 শূন্য *data, unsigned long size)
{
	সংরচনা delta_info **p = &delta_list;
	সংরচনা delta_info *info;

	যেতিয়ালৈ ((info = *p) != রিক্ত) {
		যদি (oideq(&info->base_oid, &obj_list[nr].oid) ||
		    info->base_offset == obj_list[nr].offset) {
			*p = info->next;
			p = &delta_list;
			resolve_delta(info->nr, type, data, size,
				      info->delta, info->size);
			মুক্ত(info);
			অনুৱর্ত;
		}
		p = &info->next;
	}
}

স্থির শূন্য unpack_non_delta_entry(গণনা object_type type, unsigned long size,
				   unsigned nr)
{
	শূন্য *buf = get_data(size);

	যদি (buf)
		write_object(nr, type, buf, size);
}

সংরচনা zlib_stream {
	সংরচনা odb_stream base;
	git_zstream *zstream;
	পূর্ণাঙ্ক status;
};

স্থির ssize_t zlib_stream_read(সংরচনা odb_stream *in_stream,
				ৱর্ণ *buf, size_t buf_len)
{
	সংরচনা zlib_stream *data = container_of(in_stream, সংরচনা zlib_stream, base);
	git_zstream *zstream = data->zstream;

	যদি (data->status != Z_OK)
		ঘূৰাই 0;

	zstream->next_out = (unsigned ৱর্ণ *) buf;
	zstream->avail_out = buf_len;

	যেতিয়ালৈ (data->status == Z_OK && zstream->avail_out == buf_len) {
		zstream->next_in = fill(1);
		zstream->avail_in = len;
		data->status = git_inflate(zstream, 0);
		use(len - zstream->avail_in);
	}

	ঘূৰাই buf_len - zstream->avail_out;
}

স্থির শূন্য stream_blob(unsigned long size, unsigned nr)
{
	git_zstream zstream = { 0 };
	সংরচনা zlib_stream in_stream = {
		.base = {
			.পঠ = zlib_stream_read,
			.size = size,
			.type = OBJ_BLOB,
		},
		.zstream = &zstream,
		.status = Z_OK,
	};
	সংরচনা obj_info *info = &obj_list[nr];

	git_inflate_init(&zstream);

	যদি (odb_write_object_stream(the_repository->objects, &in_stream.base, &info->oid))
		die(_("failed to write object in stream"));

	যদি (in_stream.status != Z_STREAM_END)
		die(_("inflate returned (%d)"), in_stream.status);
	git_inflate_end(&zstream);

	যদি (strict) {
		সংরচনা blob *blob = lookup_blob(the_repository, &info->oid);

		যদি (!blob)
			die(_("invalid blob object from stream"));
		blob->object.flags |= FLAG_WRITTEN;
	}
	info->obj = রিক্ত;
}

স্থির পূর্ণাঙ্ক resolve_against_held(unsigned nr, ধ্রুৱ সংরচনা object_id *base,
				শূন্য *delta_data, unsigned long delta_size)
{
	সংরচনা object *obj;
	সংরচনা obj_buffer *obj_buffer;
	obj = lookup_object(the_repository, base);
	যদি (!obj)
		ঘূৰাই 0;
	obj_buffer = lookup_object_buffer(obj);
	যদি (!obj_buffer)
		ঘূৰাই 0;
	resolve_delta(nr, obj->type, obj_buffer->buffer,
		      obj_buffer->size, delta_data, delta_size);
	ঘূৰাই 1;
}

স্থির শূন্য unpack_delta_entry(গণনা object_type type, unsigned long delta_size,
			       unsigned nr)
{
	শূন্য *delta_data, *base;
	unsigned long base_size;
	size_t base_size_st = 0;
	সংরচনা object_id base_oid;

	যদি (type == OBJ_REF_DELTA) {
		oidread(&base_oid, fill(the_hash_algo->rawsz), the_repository->hash_algo);
		use(the_hash_algo->rawsz);
		delta_data = get_data(delta_size);
		যদি (!delta_data)
			ঘূৰাই;
		যদি (odb_has_object(the_repository->objects, &base_oid,
				   ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR))
			; /* Ok we have this one */
		অন্যথা যদি (resolve_against_held(nr, &base_oid,
					      delta_data, delta_size))
			ঘূৰাই; /* we are done */
		অন্যথা {
			/* cannot resolve yet --- queue it */
			oidclr(&obj_list[nr].oid, the_repository->hash_algo);
			add_delta_to_list(nr, &base_oid, 0, delta_data, delta_size);
			ঘূৰাই;
		}
	} অন্যথা {
		unsigned base_found = 0;
		unsigned ৱর্ণ *pack, c;
		off_t base_offset;
		unsigned lo, mid, hi;

		pack = fill(1);
		c = *pack;
		use(1);
		base_offset = c & 127;
		যেতিয়ালৈ (c & 128) {
			base_offset += 1;
			যদি (!base_offset || MSB(base_offset, 7))
				die("offset value overflow for delta base object");
			pack = fill(1);
			c = *pack;
			use(1);
			base_offset = (base_offset << 7) + (c & 127);
		}
		base_offset = obj_list[nr].offset - base_offset;
		যদি (base_offset <= 0 || base_offset >= obj_list[nr].offset)
			die("offset value out of bound for delta base object");

		delta_data = get_data(delta_size);
		যদি (!delta_data)
			ঘূৰাই;
		lo = 0;
		hi = nr;
		যেতিয়ালৈ (lo < hi) {
			mid = lo + (hi - lo) / 2;
			যদি (base_offset < obj_list[mid].offset) {
				hi = mid;
			} অন্যথা যদি (base_offset > obj_list[mid].offset) {
				lo = mid + 1;
			} অন্যথা {
				oidcpy(&base_oid, &obj_list[mid].oid);
				base_found = !is_null_oid(&base_oid);
				ভঙ্গ;
			}
		}
		যদি (!base_found) {
			/*
			 * The delta base object is itself a delta that
			 * has not been resolved yet.
			 */
			oidclr(&obj_list[nr].oid, the_repository->hash_algo);
			add_delta_to_list(nr, null_oid(the_hash_algo), base_offset,
					  delta_data, delta_size);
			ঘূৰাই;
		}
	}

	যদি (resolve_against_held(nr, &base_oid, delta_data, delta_size))
		ঘূৰাই;

	base = odb_read_object(the_repository->objects, &base_oid,
			       &type, &base_size_st);
	base_size = cast_size_t_to_ulong(base_size_st);
	যদি (!base) {
		error("failed to read delta-pack base object %s",
		      oid_to_hex(&base_oid));
		যদি (!recover)
			exit(1);
		has_errors = 1;
		ঘূৰাই;
	}
	resolve_delta(nr, type, base, base_size, delta_data, delta_size);
	মুক্ত(base);
}

স্থির শূন্য unpack_one(unsigned nr)
{
	unsigned shift;
	unsigned ৱর্ণ *pack;
	size_t size, c;
	গণনা object_type type;

	obj_list[nr].offset = consumed_bytes;

	pack = fill(1);
	c = *pack;
	use(1);
	type = (c >> 4) & 7;
	size = (c & 15);
	shift = 4;
	যেতিয়ালৈ (c & 0x80) {
		যদি ((bitsizeof(size_t) - 7) < shift)
			die(_("object size too large for this platform"));
		pack = fill(1);
		c = *pack;
		use(1);
		size += (c & 0x7f) << shift;
		shift += 7;
	}

	ৱিকল্প (type) {
	পক্ষ OBJ_BLOB:
		যদি (!dry_run &&
		    size > repo_settings_get_big_file_threshold(the_repository)) {
			stream_blob(size, nr);
			ঘূৰাই;
		}
		/* fallthrough */
	পক্ষ OBJ_COMMIT:
	পক্ষ OBJ_বৃক্ষ:
	পক্ষ OBJ_TAG:
		unpack_non_delta_entry(type, size, nr);
		ঘূৰাই;
	পক্ষ OBJ_REF_DELTA:
	পক্ষ OBJ_OFS_DELTA:
		unpack_delta_entry(type, size, nr);
		ঘূৰাই;
	স্ৱভাৱ:
		error("bad object type %d", type);
		has_errors = 1;
		যদি (recover)
			ঘূৰাই;
		exit(1);
	}
}

স্থির শূন্য unpack_all(শূন্য)
{
	পূর্ণাঙ্ক i;
	unsigned ৱর্ণ *hdr = fill(পরিমাণ(সংরচনা pack_header));
	সংরচনা odb_transaction *transaction;

	যদি (get_be32(hdr) != PACK_SIGNATURE)
		die("bad pack file");
	hdr += 4;
	যদি (!pack_version_ok_native(get_be32(hdr)))
		die("unknown pack file version %"PRIu32,
		    get_be32(hdr));
	hdr += 4;
	nr_objects = get_be32(hdr);
	use(পরিমাণ(সংরচনা pack_header));

	যদি (!quiet)
		progress = start_progress(the_repository,
					  _("Unpacking objects"), nr_objects);
	CALLOC_ARRAY(obj_list, nr_objects);
	odb_transaction_begin_or_die(the_repository->objects, &transaction, 0);
	বাবে (i = 0; i < nr_objects; i++) {
		unpack_one(i);
		display_progress(progress, i + 1);
	}
	odb_transaction_commit_and_finalize_or_die(transaction);
	stop_progress(&progress);

	যদি (delta_list)
		die("unresolved deltas left after unpacking");
}

পূর্ণাঙ্ক cmd_unpack_objects(পূর্ণাঙ্ক argc,
		       ধ্রুৱ ৱর্ণ **argv,
		       ধ্রুৱ ৱর্ণ *prefix UNUSED,
		       সংরচনা repository *repo)
{
	পূর্ণাঙ্ক i;
	সংরচনা object_id oid;
	সংরচনা git_hash_ctx tmp_ctx;

	disable_replace_refs();

	repo_config(the_repository, git_default_config, রিক্ত);

	quiet = !isatty(2);

	show_usage_if_asked(argc, argv, unpack_usage);

	fsck_options_init(&fsck_options, repo, FSCK_OPTIONS_STRICT);

	বাবে (i = 1 ; i < argc; i++) {
		ধ্রুৱ ৱর্ণ *arg = argv[i];

		যদি (*arg == '-') {
			যদি (!strcmp(arg, "-n")) {
				dry_run = 1;
				অনুৱর্ত;
			}
			যদি (!strcmp(arg, "-q")) {
				quiet = 1;
				অনুৱর্ত;
			}
			যদি (!strcmp(arg, "-r")) {
				recover = 1;
				অনুৱর্ত;
			}
			যদি (!strcmp(arg, "--strict")) {
				strict = 1;
				অনুৱর্ত;
			}
			যদি (skip_prefix(arg, "--strict=", &arg)) {
				strict = 1;
				fsck_set_বাৰ্তা_types(&fsck_options, arg);
				অনুৱর্ত;
			}
			যদি (skip_prefix(arg, "--pack_header=", &arg)) {
				যদি (parse_pack_header_option(arg,
							     buffer, &len) < 0)
					die(_("bad --pack_header: %s"), arg);
				অনুৱর্ত;
			}
			যদি (skip_prefix(arg, "--max-input-size=", &arg)) {
				max_প্ৰবেশ_size = strtoumax(arg, রিক্ত, 10);
				অনুৱর্ত;
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
	যদি (strict) {
		write_rest();
		যদি (fsck_finish(&fsck_options))
			die(_("fsck error in pack objects"));
	}
	যদি (!hasheq(fill(the_hash_algo->rawsz), oid.hash,
		    the_repository->hash_algo))
		die("final sha1 did not match");
	use(the_hash_algo->rawsz);

	/* Write the last part of the buffer to stdout */
	write_in_full(1, buffer + offset, len);

	/* All done */
	ঘূৰাই has_errors;
}
