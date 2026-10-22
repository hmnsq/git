#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "config.h"
#অন্তর্ভাব "environment.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "git-zlib.h"
#অন্তর্ভাব "hex.h"
#অন্তর্ভাব "object-file.h"
#অন্তর্ভাব "odb.h"
#অন্তর্ভাব "odb/streaming.h"
#অন্তর্ভাব "odb/transaction.h"
#অন্তর্ভাব "object.h"
#অন্তর্ভাব "delta.h"
#অন্তর্ভাব "pack.h"
#অন্তর্ভাব "blob.h"
#অন্তর্ভাব "replace-object.h"
#অন্তর্ভাব "strbuf.h"
#অন্তর্ভাব "progress.h"
#অন্তর্ভাব "decorate.h"
#অন্তর্ভাব "fsck.h"
#অন্তর্ভাব "packfile.h"

স্থির পূর্ণসংখ্যা dry_run, quiet, recover, has_errors, strict;
স্থির ধ্রুবক অক্ষর unpack_usage[] = "git unpack-objects [-n] [-q] [-r] [--strict]";

স্থির unsigned অক্ষর বাফার[DEFAULT_IO_বাফার_আকার];
স্থির unsigned পূর্ণসংখ্যা offset, len;
স্থির off_t consumed_bytes;
স্থির off_t max_প্রবেশ_আকার;
স্থির কাঠামো git_hash_ctx ctx;
স্থির কাঠামো fsck_options fsck_options;
স্থির কাঠামো progress *progress;

/*
 * When running under --strict mode, objects whose reachability are
 * suspect are kept in core without getting written in the object
 * store.
 */
কাঠামো obj_বাফার {
	অক্ষর *বাফার;
	unsigned long আকার_2;
};

স্থির কাঠামো decoration obj_decorate;

স্থির কাঠামো obj_বাফার *lookup_object_বাফার(কাঠামো object *base)
{
	ফেরত lookup_decoration(&obj_decorate, base);
}

স্থির শূন্য add_object_বাফার(কাঠামো object *object, অক্ষর *বাফার, unsigned long আকার_2)
{
	কাঠামো obj_বাফার *obj;
	CALLOC_ARRAY(obj, 1);
	obj->বাফার = বাফার;
	obj->আকার_2 = আকার_2;
	যদি (add_decoration(&obj_decorate, object, obj))
		die("object %s tried to add buffer twice!", oid_to_hex(&object->oid));
}

/*
 * Make sure at least "min" bytes are available in the buffer, and
 * return the pointer to the buffer.
 */
স্থির শূন্য *fill(পূর্ণসংখ্যা min)
{
	যদি (min <= len)
		ফেরত বাফার + offset;
	যদি (min > আকার(বাফার))
		die("cannot fill %d bytes", min);
	যদি (offset) {
		git_hash_update(&ctx, বাফার, offset);
		memmove(বাফার, বাফার + offset, len);
		offset = 0;
	}
	করো {
		ssize_t ret = xread(0, বাফার + len, আকার(বাফার) - len);
		যদি (ret <= 0) {
			যদি (!ret)
				die("early EOF");
			die_errno("read error on input");
		}
		len += ret;
	} যখন (len < min);
	ফেরত বাফার;
}

স্থির শূন্য use(পূর্ণসংখ্যা bytes)
{
	যদি (bytes > len)
		die("used more bytes than were available");
	len -= bytes;
	offset += bytes;

	/* make sure off_t is sufficiently large not to wrap */
	যদি (signed_add_overflows(consumed_bytes, bytes))
		die("pack too large for current definition of off_t");
	consumed_bytes += bytes;
	যদি (max_প্রবেশ_আকার && consumed_bytes > max_প্রবেশ_আকার)
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
স্থির শূন্য *get_তথ্য(unsigned long আকার_2)
{
	git_zstream stream;
	unsigned long bufsize = dry_run && আকার_2 > 8192 ? 8192 : আকার_2;
	শূন্য *buf = xmallocz(bufsize);

	memset(&stream, 0, আকার(stream));

	stream.পরবর্তী_out = buf;
	stream.avail_out = bufsize;
	stream.পরবর্তী_in = fill(1);
	stream.avail_in = len;
	git_inflate_init(&stream);

	জন্য (;;) {
		পূর্ণসংখ্যা ret = git_inflate(&stream, 0);
		use(len - stream.avail_in);
		যদি (stream.যোগ_out == আকার_2 && ret == Z_STREAM_END)
			বিরতি;
		যদি (ret != Z_OK) {
			error("inflate returned %d", ret);
			FREE_AND_NULL(buf);
			যদি (!recover)
				exit(1);
			has_errors = 1;
			বিরতি;
		}
		stream.পরবর্তী_in = fill(1);
		stream.avail_in = len;
		যদি (dry_run) {
			/* reuse the buffer in dry_run mode */
			stream.পরবর্তী_out = buf;
			stream.avail_out = bufsize > আকার_2 - stream.যোগ_out ?
						   আকার_2 - stream.যোগ_out :
						   bufsize;
		}
	}
	git_inflate_end(&stream);
	যদি (dry_run)
		FREE_AND_NULL(buf);
	ফেরত buf;
}

কাঠামো delta_info {
	কাঠামো object_id base_oid;
	unsigned nr;
	off_t base_offset;
	unsigned long আকার_2;
	শূন্য *delta;
	কাঠামো delta_info *পরবর্তী;
};

স্থির কাঠামো delta_info *delta_list;

স্থির শূন্য add_delta_to_list(unsigned nr, ধ্রুবক কাঠামো object_id *base_oid,
			      off_t base_offset,
			      শূন্য *delta, unsigned long আকার_2)
{
	কাঠামো delta_info *info = xmalloc(আকার(*info));

	oidcpy(&info->base_oid, base_oid);
	info->base_offset = base_offset;
	info->আকার_2 = আকার_2;
	info->delta = delta;
	info->nr = nr;
	info->পরবর্তী = delta_list;
	delta_list = info;
}

কাঠামো obj_info {
	off_t offset;
	কাঠামো object_id oid;
	কাঠামো object *obj;
};

/* Remember to update object flag allocation in object.h */
#define FLAG_OPEN (1u<<20)
#define FLAG_WRITTEN (1u<<21)

স্থির কাঠামো obj_info *obj_list;
স্থির unsigned nr_objects;

/*
 * Called only from check_object() after it verified this object
 * is Ok.
 */
স্থির শূন্য write_cached_object(কাঠামো object *obj, কাঠামো obj_বাফার *obj_buf)
{
	কাঠামো object_id oid;

	যদি (odb_write_object(the_repository->objects, obj_buf->বাফার, obj_buf->আকার_2,
			     obj->type, &oid) < 0)
		die("failed to write object %s", oid_to_hex(&obj->oid));
	obj->flags |= FLAG_WRITTEN;
}

/*
 * At the very end of the processing, write_rest() scans the objects
 * that have reachability requirements and calls this function.
 * Verify its reachability and validity recursively and write it out.
 */
স্থির পূর্ণসংখ্যা check_object(কাঠামো object *obj, গণনা object_type type,
			শূন্য *তথ্য UNUSED,
			কাঠামো fsck_options *options UNUSED)
{
	কাঠামো obj_বাফার *obj_buf;

	যদি (!obj)
		ফেরত 1;

	যদি (obj->flags & FLAG_WRITTEN)
		ফেরত 0;

	যদি (type != OBJ_ANY && obj->type != type)
		die("object type mismatch");

	যদি (!(obj->flags & FLAG_OPEN)) {
		আকার_t আকার_2;
		পূর্ণসংখ্যা type = odb_read_object_info(the_repository->objects, &obj->oid, &আকার_2);
		যদি (type != obj->type || type <= 0)
			die("object of unexpected type");
		obj->flags |= FLAG_WRITTEN;
		ফেরত 0;
	}

	obj_buf = lookup_object_বাফার(obj);
	যদি (!obj_buf)
		die("Whoops! Cannot find object '%s'", oid_to_hex(&obj->oid));
	যদি (fsck_object(obj, obj_buf->বাফার, obj_buf->আকার_2, &fsck_options))
		die("fsck error in packed object");
	fsck_options.walk = check_object;
	যদি (fsck_walk(obj, ফাঁকা, &fsck_options))
		die("Error on reachable objects of %s", oid_to_hex(&obj->oid));
	write_cached_object(obj, obj_buf);
	ফেরত 0;
}

স্থির শূন্য write_rest(শূন্য)
{
	unsigned i;
	জন্য (i = 0; i < nr_objects; i++) {
		যদি (obj_list[i].obj)
			check_object(obj_list[i].obj, OBJ_ANY, ফাঁকা, ফাঁকা);
	}
}

স্থির শূন্য added_object(unsigned nr, গণনা object_type type,
			 শূন্য *তথ্য, unsigned long আকার_2);

/*
 * Write out nr-th object from the list, now we know the contents
 * of it.  Under --strict, this buffers structured objects in-core,
 * to be checked at the end.
 */
স্থির শূন্য write_object(unsigned nr, গণনা object_type type,
			 শূন্য *buf, unsigned long আকার_2)
{
	যদি (!strict) {
		যদি (odb_write_object(the_repository->objects, buf, আকার_2, type,
				     &obj_list[nr].oid) < 0)
			die("failed to write object");
		added_object(nr, type, buf, আকার_2);
		মুক্ত(buf);
		obj_list[nr].obj = ফাঁকা;
	} অন্যথা যদি (type == OBJ_BLOB) {
		কাঠামো blob *blob;
		যদি (odb_write_object(the_repository->objects, buf, আকার_2, type,
				     &obj_list[nr].oid) < 0)
			die("failed to write object");
		added_object(nr, type, buf, আকার_2);
		মুক্ত(buf);

		blob = lookup_blob(the_repository, &obj_list[nr].oid);
		যদি (blob)
			blob->object.flags |= FLAG_WRITTEN;
		অন্যথা
			die("invalid blob object");
		obj_list[nr].obj = ফাঁকা;
	} অন্যথা {
		কাঠামো object *obj;
		পূর্ণসংখ্যা eaten;
		hash_object_file(the_hash_algo, buf, আকার_2, type,
				 &obj_list[nr].oid);
		added_object(nr, type, buf, আকার_2);
		obj = parse_object_বাফার(the_repository, &obj_list[nr].oid,
					  type, আকার_2, buf,
					  &eaten);
		যদি (!obj)
			die("invalid %s", type_name(type));
		add_object_বাফার(obj, buf, আকার_2);
		obj->flags |= FLAG_OPEN;
		obj_list[nr].obj = obj;
	}
}

স্থির শূন্য resolve_delta(unsigned nr, গণনা object_type type,
			  শূন্য *base, unsigned long base_আকার,
			  শূন্য *delta, unsigned long delta_আকার_2)
{
	শূন্য *ফলাফল;
	আকার_t ফলাফল_আকার;

	ফলাফল = patch_delta(base, base_আকার,
			     delta, delta_আকার_2,
			     &ফলাফল_আকার);
	যদি (!ফলাফল)
		die("failed to apply delta");
	মুক্ত(delta);
	write_object(nr, type, ফলাফল, ফলাফল_আকার);
}

/*
 * We now know the contents of an object (which is nr-th in the pack);
 * resolve all the deltified objects that are based on it.
 */
স্থির শূন্য added_object(unsigned nr, গণনা object_type type,
			 শূন্য *তথ্য, unsigned long আকার_2)
{
	কাঠামো delta_info **p = &delta_list;
	কাঠামো delta_info *info;

	যখন ((info = *p) != ফাঁকা) {
		যদি (oideq(&info->base_oid, &obj_list[nr].oid) ||
		    info->base_offset == obj_list[nr].offset) {
			*p = info->পরবর্তী;
			p = &delta_list;
			resolve_delta(info->nr, type, তথ্য, আকার_2,
				      info->delta, info->আকার_2);
			মুক্ত(info);
			চালু;
		}
		p = &info->পরবর্তী;
	}
}

স্থির শূন্য unpack_non_delta_entry(গণনা object_type type, unsigned long আকার_2,
				   unsigned nr)
{
	শূন্য *buf = get_তথ্য(আকার_2);

	যদি (buf)
		write_object(nr, type, buf, আকার_2);
}

কাঠামো zlib_stream {
	কাঠামো odb_stream base;
	git_zstream *zstream;
	পূর্ণসংখ্যা status;
};

স্থির ssize_t zlib_stream_read(কাঠামো odb_stream *in_stream,
				অক্ষর *buf, আকার_t buf_len)
{
	কাঠামো zlib_stream *তথ্য = container_of(in_stream, কাঠামো zlib_stream, base);
	git_zstream *zstream = তথ্য->zstream;

	যদি (তথ্য->status != Z_OK)
		ফেরত 0;

	zstream->পরবর্তী_out = (unsigned অক্ষর *) buf;
	zstream->avail_out = buf_len;

	যখন (তথ্য->status == Z_OK && zstream->avail_out == buf_len) {
		zstream->পরবর্তী_in = fill(1);
		zstream->avail_in = len;
		তথ্য->status = git_inflate(zstream, 0);
		use(len - zstream->avail_in);
	}

	ফেরত buf_len - zstream->avail_out;
}

স্থির শূন্য stream_blob(unsigned long আকার_2, unsigned nr)
{
	git_zstream zstream = { 0 };
	কাঠামো zlib_stream in_stream = {
		.base = {
			.পঠ = zlib_stream_read,
			.আকার_2 = আকার_2,
			.type = OBJ_BLOB,
		},
		.zstream = &zstream,
		.status = Z_OK,
	};
	কাঠামো obj_info *info = &obj_list[nr];

	git_inflate_init(&zstream);

	যদি (odb_write_object_stream(the_repository->objects, &in_stream.base, &info->oid))
		die(_("failed to write object in stream"));

	যদি (in_stream.status != Z_STREAM_END)
		die(_("inflate returned (%d)"), in_stream.status);
	git_inflate_end(&zstream);

	যদি (strict) {
		কাঠামো blob *blob = lookup_blob(the_repository, &info->oid);

		যদি (!blob)
			die(_("invalid blob object from stream"));
		blob->object.flags |= FLAG_WRITTEN;
	}
	info->obj = ফাঁকা;
}

স্থির পূর্ণসংখ্যা resolve_against_held(unsigned nr, ধ্রুবক কাঠামো object_id *base,
				শূন্য *delta_তথ্য, unsigned long delta_আকার_2)
{
	কাঠামো object *obj;
	কাঠামো obj_বাফার *obj_বাফার;
	obj = lookup_object(the_repository, base);
	যদি (!obj)
		ফেরত 0;
	obj_বাফার = lookup_object_বাফার(obj);
	যদি (!obj_বাফার)
		ফেরত 0;
	resolve_delta(nr, obj->type, obj_বাফার->বাফার,
		      obj_বাফার->আকার_2, delta_তথ্য, delta_আকার_2);
	ফেরত 1;
}

স্থির শূন্য unpack_delta_entry(গণনা object_type type, unsigned long delta_আকার_2,
			       unsigned nr)
{
	শূন্য *delta_তথ্য, *base;
	unsigned long base_আকার;
	আকার_t base_আকার_st = 0;
	কাঠামো object_id base_oid;

	যদি (type == OBJ_REF_DELTA) {
		oidread(&base_oid, fill(the_hash_algo->rawsz), the_repository->hash_algo);
		use(the_hash_algo->rawsz);
		delta_তথ্য = get_তথ্য(delta_আকার_2);
		যদি (!delta_তথ্য)
			ফেরত;
		যদি (odb_has_object(the_repository->objects, &base_oid,
				   ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR))
			; /* Ok we have this one */
		অন্যথা যদি (resolve_against_held(nr, &base_oid,
					      delta_তথ্য, delta_আকার_2))
			ফেরত; /* we are done */
		অন্যথা {
			/* cannot resolve yet --- queue it */
			oidclr(&obj_list[nr].oid, the_repository->hash_algo);
			add_delta_to_list(nr, &base_oid, 0, delta_তথ্য, delta_আকার_2);
			ফেরত;
		}
	} অন্যথা {
		unsigned base_found = 0;
		unsigned অক্ষর *pack, c;
		off_t base_offset;
		unsigned lo, mid, hi;

		pack = fill(1);
		c = *pack;
		use(1);
		base_offset = c & 127;
		যখন (c & 128) {
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

		delta_তথ্য = get_তথ্য(delta_আকার_2);
		যদি (!delta_তথ্য)
			ফেরত;
		lo = 0;
		hi = nr;
		যখন (lo < hi) {
			mid = lo + (hi - lo) / 2;
			যদি (base_offset < obj_list[mid].offset) {
				hi = mid;
			} অন্যথা যদি (base_offset > obj_list[mid].offset) {
				lo = mid + 1;
			} অন্যথা {
				oidcpy(&base_oid, &obj_list[mid].oid);
				base_found = !is_null_oid(&base_oid);
				বিরতি;
			}
		}
		যদি (!base_found) {
			/*
			 * The delta base object is itself a delta that
			 * has not been resolved yet.
			 */
			oidclr(&obj_list[nr].oid, the_repository->hash_algo);
			add_delta_to_list(nr, null_oid(the_hash_algo), base_offset,
					  delta_তথ্য, delta_আকার_2);
			ফেরত;
		}
	}

	যদি (resolve_against_held(nr, &base_oid, delta_তথ্য, delta_আকার_2))
		ফেরত;

	base = odb_read_object(the_repository->objects, &base_oid,
			       &type, &base_আকার_st);
	base_আকার = cast_আকার_t_to_ulong(base_আকার_st);
	যদি (!base) {
		error("failed to read delta-pack base object %s",
		      oid_to_hex(&base_oid));
		যদি (!recover)
			exit(1);
		has_errors = 1;
		ফেরত;
	}
	resolve_delta(nr, type, base, base_আকার, delta_তথ্য, delta_আকার_2);
	মুক্ত(base);
}

স্থির শূন্য unpack_one(unsigned nr)
{
	unsigned shift;
	unsigned অক্ষর *pack;
	আকার_t আকার_2, c;
	গণনা object_type type;

	obj_list[nr].offset = consumed_bytes;

	pack = fill(1);
	c = *pack;
	use(1);
	type = (c >> 4) & 7;
	আকার_2 = (c & 15);
	shift = 4;
	যখন (c & 0x80) {
		যদি ((bitsizeof(আকার_t) - 7) < shift)
			die(_("object size too large for this platform"));
		pack = fill(1);
		c = *pack;
		use(1);
		আকার_2 += (c & 0x7f) << shift;
		shift += 7;
	}

	নির্বাচন (type) {
	ক্ষেত্র OBJ_BLOB:
		যদি (!dry_run &&
		    আকার_2 > repo_settings_get_big_file_threshold(the_repository)) {
			stream_blob(আকার_2, nr);
			ফেরত;
		}
		/* fallthrough */
	ক্ষেত্র OBJ_COMMIT:
	ক্ষেত্র OBJ_বৃক্ষ:
	ক্ষেত্র OBJ_TAG:
		unpack_non_delta_entry(type, আকার_2, nr);
		ফেরত;
	ক্ষেত্র OBJ_REF_DELTA:
	ক্ষেত্র OBJ_OFS_DELTA:
		unpack_delta_entry(type, আকার_2, nr);
		ফেরত;
	পূর্বনির্ধারিত:
		error("bad object type %d", type);
		has_errors = 1;
		যদি (recover)
			ফেরত;
		exit(1);
	}
}

স্থির শূন্য unpack_all(শূন্য)
{
	পূর্ণসংখ্যা i;
	unsigned অক্ষর *hdr = fill(আকার(কাঠামো pack_header));
	কাঠামো odb_transaction *transaction;

	যদি (get_be32(hdr) != PACK_SIGNATURE)
		die("bad pack file");
	hdr += 4;
	যদি (!pack_version_ok_native(get_be32(hdr)))
		die("unknown pack file version %"PRIu32,
		    get_be32(hdr));
	hdr += 4;
	nr_objects = get_be32(hdr);
	use(আকার(কাঠামো pack_header));

	যদি (!quiet)
		progress = start_progress(the_repository,
					  _("Unpacking objects"), nr_objects);
	CALLOC_ARRAY(obj_list, nr_objects);
	odb_transaction_begin_or_die(the_repository->objects, &transaction, 0);
	জন্য (i = 0; i < nr_objects; i++) {
		unpack_one(i);
		display_progress(progress, i + 1);
	}
	odb_transaction_commit_and_finalize_or_die(transaction);
	stop_progress(&progress);

	যদি (delta_list)
		die("unresolved deltas left after unpacking");
}

পূর্ণসংখ্যা cmd_unpack_objects(পূর্ণসংখ্যা argc,
		       ধ্রুবক অক্ষর **argv,
		       ধ্রুবক অক্ষর *prefix UNUSED,
		       কাঠামো repository *repo)
{
	পূর্ণসংখ্যা i;
	কাঠামো object_id oid;
	কাঠামো git_hash_ctx tmp_ctx;

	disable_replace_refs();

	repo_config(the_repository, git_default_config, ফাঁকা);

	quiet = !isatty(2);

	show_usage_if_asked(argc, argv, unpack_usage);

	fsck_options_init(&fsck_options, repo, FSCK_OPTIONS_STRICT);

	জন্য (i = 1 ; i < argc; i++) {
		ধ্রুবক অক্ষর *arg = argv[i];

		যদি (*arg == '-') {
			যদি (!strcmp(arg, "-n")) {
				dry_run = 1;
				চালু;
			}
			যদি (!strcmp(arg, "-q")) {
				quiet = 1;
				চালু;
			}
			যদি (!strcmp(arg, "-r")) {
				recover = 1;
				চালু;
			}
			যদি (!strcmp(arg, "--strict")) {
				strict = 1;
				চালু;
			}
			যদি (skip_prefix(arg, "--strict=", &arg)) {
				strict = 1;
				fsck_set_বার্তা_types(&fsck_options, arg);
				চালু;
			}
			যদি (skip_prefix(arg, "--pack_header=", &arg)) {
				যদি (parse_pack_header_option(arg,
							     বাফার, &len) < 0)
					die(_("bad --pack_header: %s"), arg);
				চালু;
			}
			যদি (skip_prefix(arg, "--max-input-size=", &arg)) {
				max_প্রবেশ_আকার = strtoumax(arg, ফাঁকা, 10);
				চালু;
			}
			usage(unpack_usage);
		}

		/* We don't take any non-flag arguments now.. Maybe some day */
		usage(unpack_usage);
	}
	git_hash_init(&ctx, the_hash_algo);
	unpack_all();
	git_hash_update(&ctx, বাফার, offset);
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
	write_in_full(1, বাফার + offset, len);

	/* All done */
	ফেরত has_errors;
}
