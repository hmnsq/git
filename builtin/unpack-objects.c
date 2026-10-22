#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#अन्तर्भाव "builtin.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "git-zlib.h"
#अन्तर्भाव "hex.h"
#अन्तर्भाव "object-file.h"
#अन्तर्भाव "odb.h"
#अन्तर्भाव "odb/streaming.h"
#अन्तर्भाव "odb/transaction.h"
#अन्तर्भाव "object.h"
#अन्तर्भाव "delta.h"
#अन्तर्भाव "pack.h"
#अन्तर्भाव "blob.h"
#अन्तर्भाव "replace-object.h"
#अन्तर्भाव "strbuf.h"
#अन्तर्भाव "progress.h"
#अन्तर्भाव "decorate.h"
#अन्तर्भाव "fsck.h"
#अन्तर्भाव "packfile.h"

स्थिर पूर्णांक dry_run, quiet, recover, has_errors, strict;
स्थिर स्थिरांक अक्षर unpack_usage[] = "git unpack-objects [-n] [-q] [-r] [--strict]";

स्थिर अचिन्हित अक्षर buffer[DEFAULT_IO_BUFFER_SIZE];
स्थिर अचिन्हित पूर्णांक offset, len;
स्थिर off_t consumed_bytes;
स्थिर off_t max_आदान_size;
स्थिर संरचना git_hash_ctx ctx;
स्थिर संरचना fsck_options fsck_options;
स्थिर संरचना progress *progress;

/*
 * When running under --strict mode, objects whose reachability are
 * suspect are kept in core without getting written in the object
 * store.
 */
संरचना obj_buffer {
	अक्षर *buffer;
	अचिन्हित दीर्घ size;
};

स्थिर संरचना decoration obj_decorate;

स्थिर संरचना obj_buffer *lookup_object_buffer(संरचना object *base)
{
	परत lookup_decoration(&obj_decorate, base);
}

स्थिर शून्य add_object_buffer(संरचना object *object, अक्षर *buffer, अचिन्हित दीर्घ size)
{
	संरचना obj_buffer *obj;
	CALLOC_ARRAY(obj, 1);
	obj->buffer = buffer;
	obj->size = size;
	जर (add_decoration(&obj_decorate, object, obj))
		die("object %s tried to add buffer twice!", oid_to_hex(&object->oid));
}

/*
 * Make sure at least "min" bytes are available in the buffer, and
 * return the pointer to the buffer.
 */
स्थिर शून्य *fill(पूर्णांक min)
{
	जर (min <= len)
		परत buffer + offset;
	जर (min > आकार(buffer))
		die("cannot fill %d bytes", min);
	जर (offset) {
		git_hash_update(&ctx, buffer, offset);
		स_हटाओ(buffer, buffer + offset, len);
		offset = 0;
	}
	करा {
		ssize_t ret = xread(0, buffer + len, आकार(buffer) - len);
		जर (ret <= 0) {
			जर (!ret)
				die("early EOF");
			die_errno("read error on input");
		}
		len += ret;
	} पर्यंत (len < min);
	परत buffer;
}

स्थिर शून्य use(पूर्णांक bytes)
{
	जर (bytes > len)
		die("used more bytes than were available");
	len -= bytes;
	offset += bytes;

	/* make sure off_t is sufficiently large not to wrap */
	जर (signed_add_overflows(consumed_bytes, bytes))
		die("pack too large for current definition of off_t");
	consumed_bytes += bytes;
	जर (max_आदान_size && consumed_bytes > max_आदान_size)
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
स्थिर शून्य *get_data(अचिन्हित दीर्घ size)
{
	git_zstream stream;
	अचिन्हित दीर्घ bufsize = dry_run && size > 8192 ? 8192 : size;
	शून्य *buf = xmallocz(bufsize);

	स_रखो(&stream, 0, आकार(stream));

	stream.next_out = buf;
	stream.avail_out = bufsize;
	stream.next_in = fill(1);
	stream.avail_in = len;
	git_inflate_init(&stream);

	साठी (;;) {
		पूर्णांक ret = git_inflate(&stream, 0);
		use(len - stream.avail_in);
		जर (stream.total_out == size && ret == Z_STREAM_END)
			थांबा;
		जर (ret != Z_OK) {
			error("inflate returned %d", ret);
			FREE_AND_NULL(buf);
			जर (!recover)
				निकास(1);
			has_errors = 1;
			थांबा;
		}
		stream.next_in = fill(1);
		stream.avail_in = len;
		जर (dry_run) {
			/* reuse the buffer in dry_run mode */
			stream.next_out = buf;
			stream.avail_out = bufsize > size - stream.total_out ?
						   size - stream.total_out :
						   bufsize;
		}
	}
	git_inflate_end(&stream);
	जर (dry_run)
		FREE_AND_NULL(buf);
	परत buf;
}

संरचना delta_info {
	संरचना object_id base_oid;
	अचिन्हित nr;
	off_t base_offset;
	अचिन्हित दीर्घ size;
	शून्य *delta;
	संरचना delta_info *next;
};

स्थिर संरचना delta_info *delta_list;

स्थिर शून्य add_delta_to_list(अचिन्हित nr, स्थिरांक संरचना object_id *base_oid,
			      off_t base_offset,
			      शून्य *delta, अचिन्हित दीर्घ size)
{
	संरचना delta_info *info = xmalloc(आकार(*info));

	oidcpy(&info->base_oid, base_oid);
	info->base_offset = base_offset;
	info->size = size;
	info->delta = delta;
	info->nr = nr;
	info->next = delta_list;
	delta_list = info;
}

संरचना obj_info {
	off_t offset;
	संरचना object_id oid;
	संरचना object *obj;
};

/* Remember to update object flag allocation in object.h */
#define FLAG_OPEN (1u<<20)
#define FLAG_WRITTEN (1u<<21)

स्थिर संरचना obj_info *obj_list;
स्थिर अचिन्हित nr_objects;

/*
 * Called only from check_object() after it verified this object
 * is Ok.
 */
स्थिर शून्य write_cached_object(संरचना object *obj, संरचना obj_buffer *obj_buf)
{
	संरचना object_id oid;

	जर (odb_write_object(the_repository->objects, obj_buf->buffer, obj_buf->size,
			     obj->type, &oid) < 0)
		die("failed to write object %s", oid_to_hex(&obj->oid));
	obj->flags |= FLAG_WRITTEN;
}

/*
 * At the very end of the processing, write_rest() scans the objects
 * that have reachability requirements and calls this function.
 * Verify its reachability and validity recursively and write it out.
 */
स्थिर पूर्णांक check_object(संरचना object *obj, गणना object_type type,
			शून्य *data UNUSED,
			संरचना fsck_options *options UNUSED)
{
	संरचना obj_buffer *obj_buf;

	जर (!obj)
		परत 1;

	जर (obj->flags & FLAG_WRITTEN)
		परत 0;

	जर (type != OBJ_ANY && obj->type != type)
		die("object type mismatch");

	जर (!(obj->flags & FLAG_OPEN)) {
		माप_प्रकार size;
		पूर्णांक type = odb_read_object_info(the_repository->objects, &obj->oid, &size);
		जर (type != obj->type || type <= 0)
			die("object of unexpected type");
		obj->flags |= FLAG_WRITTEN;
		परत 0;
	}

	obj_buf = lookup_object_buffer(obj);
	जर (!obj_buf)
		die("Whoops! Cannot find object '%s'", oid_to_hex(&obj->oid));
	जर (fsck_object(obj, obj_buf->buffer, obj_buf->size, &fsck_options))
		die("fsck error in packed object");
	fsck_options.walk = check_object;
	जर (fsck_walk(obj, रिक्त, &fsck_options))
		die("Error on reachable objects of %s", oid_to_hex(&obj->oid));
	write_cached_object(obj, obj_buf);
	परत 0;
}

स्थिर शून्य write_rest(शून्य)
{
	अचिन्हित i;
	साठी (i = 0; i < nr_objects; i++) {
		जर (obj_list[i].obj)
			check_object(obj_list[i].obj, OBJ_ANY, रिक्त, रिक्त);
	}
}

स्थिर शून्य added_object(अचिन्हित nr, गणना object_type type,
			 शून्य *data, अचिन्हित दीर्घ size);

/*
 * Write out nr-th object from the list, now we know the contents
 * of it.  Under --strict, this buffers structured objects in-core,
 * to be checked at the end.
 */
स्थिर शून्य write_object(अचिन्हित nr, गणना object_type type,
			 शून्य *buf, अचिन्हित दीर्घ size)
{
	जर (!strict) {
		जर (odb_write_object(the_repository->objects, buf, size, type,
				     &obj_list[nr].oid) < 0)
			die("failed to write object");
		added_object(nr, type, buf, size);
		मुक्त(buf);
		obj_list[nr].obj = रिक्त;
	} अन्यथा जर (type == OBJ_BLOB) {
		संरचना blob *blob;
		जर (odb_write_object(the_repository->objects, buf, size, type,
				     &obj_list[nr].oid) < 0)
			die("failed to write object");
		added_object(nr, type, buf, size);
		मुक्त(buf);

		blob = lookup_blob(the_repository, &obj_list[nr].oid);
		जर (blob)
			blob->object.flags |= FLAG_WRITTEN;
		अन्यथा
			die("invalid blob object");
		obj_list[nr].obj = रिक्त;
	} अन्यथा {
		संरचना object *obj;
		पूर्णांक eaten;
		hash_object_file(the_hash_algo, buf, size, type,
				 &obj_list[nr].oid);
		added_object(nr, type, buf, size);
		obj = parse_object_buffer(the_repository, &obj_list[nr].oid,
					  type, size, buf,
					  &eaten);
		जर (!obj)
			die("invalid %s", type_name(type));
		add_object_buffer(obj, buf, size);
		obj->flags |= FLAG_OPEN;
		obj_list[nr].obj = obj;
	}
}

स्थिर शून्य resolve_delta(अचिन्हित nr, गणना object_type type,
			  शून्य *base, अचिन्हित दीर्घ base_size,
			  शून्य *delta, अचिन्हित दीर्घ delta_size)
{
	शून्य *result;
	माप_प्रकार result_size;

	result = patch_delta(base, base_size,
			     delta, delta_size,
			     &result_size);
	जर (!result)
		die("failed to apply delta");
	मुक्त(delta);
	write_object(nr, type, result, result_size);
}

/*
 * We now know the contents of an object (which is nr-th in the pack);
 * resolve all the deltified objects that are based on it.
 */
स्थिर शून्य added_object(अचिन्हित nr, गणना object_type type,
			 शून्य *data, अचिन्हित दीर्घ size)
{
	संरचना delta_info **p = &delta_list;
	संरचना delta_info *info;

	पर्यंत ((info = *p) != रिक्त) {
		जर (oideq(&info->base_oid, &obj_list[nr].oid) ||
		    info->base_offset == obj_list[nr].offset) {
			*p = info->next;
			p = &delta_list;
			resolve_delta(info->nr, type, data, size,
				      info->delta, info->size);
			मुक्त(info);
			सुरूठेवा;
		}
		p = &info->next;
	}
}

स्थिर शून्य unpack_non_delta_entry(गणना object_type type, अचिन्हित दीर्घ size,
				   अचिन्हित nr)
{
	शून्य *buf = get_data(size);

	जर (buf)
		write_object(nr, type, buf, size);
}

संरचना zlib_stream {
	संरचना odb_stream base;
	git_zstream *zstream;
	पूर्णांक status;
};

स्थिर ssize_t zlib_stream_read(संरचना odb_stream *in_stream,
				अक्षर *buf, माप_प्रकार buf_len)
{
	संरचना zlib_stream *data = container_of(in_stream, संरचना zlib_stream, base);
	git_zstream *zstream = data->zstream;

	जर (data->status != Z_OK)
		परत 0;

	zstream->next_out = (अचिन्हित अक्षर *) buf;
	zstream->avail_out = buf_len;

	पर्यंत (data->status == Z_OK && zstream->avail_out == buf_len) {
		zstream->next_in = fill(1);
		zstream->avail_in = len;
		data->status = git_inflate(zstream, 0);
		use(len - zstream->avail_in);
	}

	परत buf_len - zstream->avail_out;
}

स्थिर शून्य stream_blob(अचिन्हित दीर्घ size, अचिन्हित nr)
{
	git_zstream zstream = { 0 };
	संरचना zlib_stream in_stream = {
		.base = {
			.पठ = zlib_stream_read,
			.size = size,
			.type = OBJ_BLOB,
		},
		.zstream = &zstream,
		.status = Z_OK,
	};
	संरचना obj_info *info = &obj_list[nr];

	git_inflate_init(&zstream);

	जर (odb_write_object_stream(the_repository->objects, &in_stream.base, &info->oid))
		die(_("failed to write object in stream"));

	जर (in_stream.status != Z_STREAM_END)
		die(_("inflate returned (%d)"), in_stream.status);
	git_inflate_end(&zstream);

	जर (strict) {
		संरचना blob *blob = lookup_blob(the_repository, &info->oid);

		जर (!blob)
			die(_("invalid blob object from stream"));
		blob->object.flags |= FLAG_WRITTEN;
	}
	info->obj = रिक्त;
}

स्थिर पूर्णांक resolve_against_held(अचिन्हित nr, स्थिरांक संरचना object_id *base,
				शून्य *delta_data, अचिन्हित दीर्घ delta_size)
{
	संरचना object *obj;
	संरचना obj_buffer *obj_buffer;
	obj = lookup_object(the_repository, base);
	जर (!obj)
		परत 0;
	obj_buffer = lookup_object_buffer(obj);
	जर (!obj_buffer)
		परत 0;
	resolve_delta(nr, obj->type, obj_buffer->buffer,
		      obj_buffer->size, delta_data, delta_size);
	परत 1;
}

स्थिर शून्य unpack_delta_entry(गणना object_type type, अचिन्हित दीर्घ delta_size,
			       अचिन्हित nr)
{
	शून्य *delta_data, *base;
	अचिन्हित दीर्घ base_size;
	माप_प्रकार base_size_st = 0;
	संरचना object_id base_oid;

	जर (type == OBJ_REF_DELTA) {
		oidread(&base_oid, fill(the_hash_algo->rawsz), the_repository->hash_algo);
		use(the_hash_algo->rawsz);
		delta_data = get_data(delta_size);
		जर (!delta_data)
			परत;
		जर (odb_has_object(the_repository->objects, &base_oid,
				   ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR))
			; /* Ok we have this one */
		अन्यथा जर (resolve_against_held(nr, &base_oid,
					      delta_data, delta_size))
			परत; /* we are done */
		अन्यथा {
			/* cannot resolve yet --- queue it */
			oidclr(&obj_list[nr].oid, the_repository->hash_algo);
			add_delta_to_list(nr, &base_oid, 0, delta_data, delta_size);
			परत;
		}
	} अन्यथा {
		अचिन्हित base_found = 0;
		अचिन्हित अक्षर *pack, c;
		off_t base_offset;
		अचिन्हित lo, mid, hi;

		pack = fill(1);
		c = *pack;
		use(1);
		base_offset = c & 127;
		पर्यंत (c & 128) {
			base_offset += 1;
			जर (!base_offset || MSB(base_offset, 7))
				die("offset value overflow for delta base object");
			pack = fill(1);
			c = *pack;
			use(1);
			base_offset = (base_offset << 7) + (c & 127);
		}
		base_offset = obj_list[nr].offset - base_offset;
		जर (base_offset <= 0 || base_offset >= obj_list[nr].offset)
			die("offset value out of bound for delta base object");

		delta_data = get_data(delta_size);
		जर (!delta_data)
			परत;
		lo = 0;
		hi = nr;
		पर्यंत (lo < hi) {
			mid = lo + (hi - lo) / 2;
			जर (base_offset < obj_list[mid].offset) {
				hi = mid;
			} अन्यथा जर (base_offset > obj_list[mid].offset) {
				lo = mid + 1;
			} अन्यथा {
				oidcpy(&base_oid, &obj_list[mid].oid);
				base_found = !is_null_oid(&base_oid);
				थांबा;
			}
		}
		जर (!base_found) {
			/*
			 * The delta base object is itself a delta that
			 * has not been resolved yet.
			 */
			oidclr(&obj_list[nr].oid, the_repository->hash_algo);
			add_delta_to_list(nr, null_oid(the_hash_algo), base_offset,
					  delta_data, delta_size);
			परत;
		}
	}

	जर (resolve_against_held(nr, &base_oid, delta_data, delta_size))
		परत;

	base = odb_read_object(the_repository->objects, &base_oid,
			       &type, &base_size_st);
	base_size = cast_size_t_to_ulong(base_size_st);
	जर (!base) {
		error("failed to read delta-pack base object %s",
		      oid_to_hex(&base_oid));
		जर (!recover)
			निकास(1);
		has_errors = 1;
		परत;
	}
	resolve_delta(nr, type, base, base_size, delta_data, delta_size);
	मुक्त(base);
}

स्थिर शून्य unpack_one(अचिन्हित nr)
{
	अचिन्हित shift;
	अचिन्हित अक्षर *pack;
	माप_प्रकार size, c;
	गणना object_type type;

	obj_list[nr].offset = consumed_bytes;

	pack = fill(1);
	c = *pack;
	use(1);
	type = (c >> 4) & 7;
	size = (c & 15);
	shift = 4;
	पर्यंत (c & 0x80) {
		जर ((bitsizeof(माप_प्रकार) - 7) < shift)
			die(_("object size too large for this platform"));
		pack = fill(1);
		c = *pack;
		use(1);
		size += (c & 0x7f) << shift;
		shift += 7;
	}

	निवड (type) {
	स्थिती OBJ_BLOB:
		जर (!dry_run &&
		    size > repo_settings_get_big_file_threshold(the_repository)) {
			stream_blob(size, nr);
			परत;
		}
		/* fallthrough */
	स्थिती OBJ_COMMIT:
	स्थिती OBJ_वृक्ष:
	स्थिती OBJ_TAG:
		unpack_non_delta_entry(type, size, nr);
		परत;
	स्थिती OBJ_REF_DELTA:
	स्थिती OBJ_OFS_DELTA:
		unpack_delta_entry(type, size, nr);
		परत;
	पूर्वनिर्धारित:
		error("bad object type %d", type);
		has_errors = 1;
		जर (recover)
			परत;
		निकास(1);
	}
}

स्थिर शून्य unpack_all(शून्य)
{
	पूर्णांक i;
	अचिन्हित अक्षर *hdr = fill(आकार(संरचना pack_header));
	संरचना odb_transaction *transaction;

	जर (get_be32(hdr) != PACK_SIGNATURE)
		die("bad pack file");
	hdr += 4;
	जर (!pack_version_ok_native(get_be32(hdr)))
		die("unknown pack file version %"PRIu32,
		    get_be32(hdr));
	hdr += 4;
	nr_objects = get_be32(hdr);
	use(आकार(संरचना pack_header));

	जर (!quiet)
		progress = start_progress(the_repository,
					  _("Unpacking objects"), nr_objects);
	CALLOC_ARRAY(obj_list, nr_objects);
	odb_transaction_begin_or_die(the_repository->objects, &transaction, 0);
	साठी (i = 0; i < nr_objects; i++) {
		unpack_one(i);
		display_progress(progress, i + 1);
	}
	odb_transaction_commit_and_finalize_or_die(transaction);
	stop_progress(&progress);

	जर (delta_list)
		die("unresolved deltas left after unpacking");
}

पूर्णांक cmd_unpack_objects(पूर्णांक argc,
		       स्थिरांक अक्षर **argv,
		       स्थिरांक अक्षर *prefix UNUSED,
		       संरचना repository *repo)
{
	पूर्णांक i;
	संरचना object_id oid;
	संरचना git_hash_ctx tmp_ctx;

	disable_replace_refs();

	repo_config(the_repository, git_default_config, रिक्त);

	quiet = !isatty(2);

	show_usage_if_asked(argc, argv, unpack_usage);

	fsck_options_init(&fsck_options, repo, FSCK_OPTIONS_STRICT);

	साठी (i = 1 ; i < argc; i++) {
		स्थिरांक अक्षर *arg = argv[i];

		जर (*arg == '-') {
			जर (!म_भेद(arg, "-n")) {
				dry_run = 1;
				सुरूठेवा;
			}
			जर (!म_भेद(arg, "-q")) {
				quiet = 1;
				सुरूठेवा;
			}
			जर (!म_भेद(arg, "-r")) {
				recover = 1;
				सुरूठेवा;
			}
			जर (!म_भेद(arg, "--strict")) {
				strict = 1;
				सुरूठेवा;
			}
			जर (skip_prefix(arg, "--strict=", &arg)) {
				strict = 1;
				fsck_set_संदेश_types(&fsck_options, arg);
				सुरूठेवा;
			}
			जर (skip_prefix(arg, "--pack_header=", &arg)) {
				जर (parse_pack_header_option(arg,
							     buffer, &len) < 0)
					die(_("bad --pack_header: %s"), arg);
				सुरूठेवा;
			}
			जर (skip_prefix(arg, "--max-input-size=", &arg)) {
				max_आदान_size = strtoumax(arg, रिक्त, 10);
				सुरूठेवा;
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
	जर (strict) {
		write_rest();
		जर (fsck_finish(&fsck_options))
			die(_("fsck error in pack objects"));
	}
	जर (!hasheq(fill(the_hash_algo->rawsz), oid.hash,
		    the_repository->hash_algo))
		die("final sha1 did not match");
	use(the_hash_algo->rawsz);

	/* Write the last part of the buffer to stdout */
	write_in_full(1, buffer + offset, len);

	/* All done */
	परत has_errors;
}
