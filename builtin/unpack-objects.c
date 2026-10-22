#घोषणा USE_THE_REPOSITORY_VARIABLE
#घोषणा DISABLE_SIGN_COMPARE_WARNINGS

#समावेश "builtin.h"
#समावेश "config.h"
#समावेश "environment.h"
#समावेश "gettext.h"
#समावेश "git-zlib.h"
#समावेश "hex.h"
#समावेश "object-file.h"
#समावेश "odb.h"
#समावेश "odb/streaming.h"
#समावेश "odb/transaction.h"
#समावेश "object.h"
#समावेश "delta.h"
#समावेश "pack.h"
#समावेश "blob.h"
#समावेश "replace-object.h"
#समावेश "strbuf.h"
#समावेश "progress.h"
#समावेश "decorate.h"
#समावेश "fsck.h"
#समावेश "packfile.h"

अटल पूर्णांक dry_run, quiet, recover, has_errors, strict;
अटल स्थिर अक्षर unpack_usage[] = "git unpack-objects [-n] [-q] [-r] [--strict]";

अटल अचिन्हित अक्षर बफर[DEFAULT_IO_बफर_आकार];
अटल अचिन्हित पूर्णांक offset, len;
अटल off_t consumed_bytes;
अटल off_t max_निवेश_आकार;
अटल काष्ठा git_hash_ctx ctx;
अटल काष्ठा fsck_options fsck_options;
अटल काष्ठा progress *progress;

/*
 * When running under --strict mode, objects whose reachability are
 * suspect are kept in core without getting written in the object
 * store.
 */
काष्ठा obj_बफर {
	अक्षर *बफर;
	अचिन्हित दीर्घ आकार;
};

अटल काष्ठा decoration obj_decorate;

अटल काष्ठा obj_बफर *lookup_object_बफर(काष्ठा object *base)
{
	वापस lookup_decoration(&obj_decorate, base);
}

अटल व्योम add_object_बफर(काष्ठा object *object, अक्षर *बफर, अचिन्हित दीर्घ आकार)
{
	काष्ठा obj_बफर *obj;
	CALLOC_ARRAY(obj, 1);
	obj->बफर = बफर;
	obj->आकार = आकार;
	अगर (add_decoration(&obj_decorate, object, obj))
		die("object %s tried to add buffer twice!", oid_to_hex(&object->oid));
}

/*
 * Make sure at least "min" bytes are available in the buffer, and
 * return the pointer to the buffer.
 */
अटल व्योम *fill(पूर्णांक min)
{
	अगर (min <= len)
		वापस बफर + offset;
	अगर (min > माप(बफर))
		die("cannot fill %d bytes", min);
	अगर (offset) {
		git_hash_update(&ctx, बफर, offset);
		स_हटाओ(बफर, बफर + offset, len);
		offset = 0;
	}
	करो {
		ssize_t ret = xread(0, बफर + len, माप(बफर) - len);
		अगर (ret <= 0) {
			अगर (!ret)
				die("early EOF");
			die_errno("read error on input");
		}
		len += ret;
	} जबतक (len < min);
	वापस बफर;
}

अटल व्योम use(पूर्णांक bytes)
{
	अगर (bytes > len)
		die("used more bytes than were available");
	len -= bytes;
	offset += bytes;

	/* make sure off_t is sufficiently large not to wrap */
	अगर (signed_add_overflows(consumed_bytes, bytes))
		die("pack too large for current definition of off_t");
	consumed_bytes += bytes;
	अगर (max_निवेश_आकार && consumed_bytes > max_निवेश_आकार)
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
अटल व्योम *get_तथ्य(अचिन्हित दीर्घ आकार)
{
	git_zstream stream;
	अचिन्हित दीर्घ bufsize = dry_run && आकार > 8192 ? 8192 : आकार;
	व्योम *buf = xmallocz(bufsize);

	स_रखो(&stream, 0, माप(stream));

	stream.अग्र_out = buf;
	stream.avail_out = bufsize;
	stream.अग्र_in = fill(1);
	stream.avail_in = len;
	git_inflate_init(&stream);

	क्रम (;;) {
		पूर्णांक ret = git_inflate(&stream, 0);
		use(len - stream.avail_in);
		अगर (stream.योग_out == आकार && ret == Z_STREAM_END)
			अवरोध;
		अगर (ret != Z_OK) {
			error("inflate returned %d", ret);
			FREE_AND_NULL(buf);
			अगर (!recover)
				निकास(1);
			has_errors = 1;
			अवरोध;
		}
		stream.अग्र_in = fill(1);
		stream.avail_in = len;
		अगर (dry_run) {
			/* reuse the buffer in dry_run mode */
			stream.अग्र_out = buf;
			stream.avail_out = bufsize > आकार - stream.योग_out ?
						   आकार - stream.योग_out :
						   bufsize;
		}
	}
	git_inflate_end(&stream);
	अगर (dry_run)
		FREE_AND_NULL(buf);
	वापस buf;
}

काष्ठा delta_info {
	काष्ठा object_id base_oid;
	अचिन्हित nr;
	off_t base_offset;
	अचिन्हित दीर्घ आकार;
	व्योम *delta;
	काष्ठा delta_info *अग्र;
};

अटल काष्ठा delta_info *delta_list;

अटल व्योम add_delta_to_list(अचिन्हित nr, स्थिर काष्ठा object_id *base_oid,
			      off_t base_offset,
			      व्योम *delta, अचिन्हित दीर्घ आकार)
{
	काष्ठा delta_info *info = xmalloc(माप(*info));

	oidcpy(&info->base_oid, base_oid);
	info->base_offset = base_offset;
	info->आकार = आकार;
	info->delta = delta;
	info->nr = nr;
	info->अग्र = delta_list;
	delta_list = info;
}

काष्ठा obj_info {
	off_t offset;
	काष्ठा object_id oid;
	काष्ठा object *obj;
};

/* Remember to update object flag allocation in object.h */
#घोषणा FLAG_OPEN (1u<<20)
#घोषणा FLAG_WRITTEN (1u<<21)

अटल काष्ठा obj_info *obj_list;
अटल अचिन्हित nr_objects;

/*
 * Called only from check_object() after it verified this object
 * is Ok.
 */
अटल व्योम write_cached_object(काष्ठा object *obj, काष्ठा obj_बफर *obj_buf)
{
	काष्ठा object_id oid;

	अगर (odb_write_object(the_repository->objects, obj_buf->बफर, obj_buf->आकार,
			     obj->type, &oid) < 0)
		die("failed to write object %s", oid_to_hex(&obj->oid));
	obj->flags |= FLAG_WRITTEN;
}

/*
 * At the very end of the processing, write_rest() scans the objects
 * that have reachability requirements and calls this function.
 * Verify its reachability and validity recursively and write it out.
 */
अटल पूर्णांक check_object(काष्ठा object *obj, क्रमागत object_type type,
			व्योम *तथ्य UNUSED,
			काष्ठा fsck_options *options UNUSED)
{
	काष्ठा obj_बफर *obj_buf;

	अगर (!obj)
		वापस 1;

	अगर (obj->flags & FLAG_WRITTEN)
		वापस 0;

	अगर (type != OBJ_ANY && obj->type != type)
		die("object type mismatch");

	अगर (!(obj->flags & FLAG_OPEN)) {
		माप_प्रकार आकार;
		पूर्णांक type = odb_read_object_info(the_repository->objects, &obj->oid, &आकार);
		अगर (type != obj->type || type <= 0)
			die("object of unexpected type");
		obj->flags |= FLAG_WRITTEN;
		वापस 0;
	}

	obj_buf = lookup_object_बफर(obj);
	अगर (!obj_buf)
		die("Whoops! Cannot find object '%s'", oid_to_hex(&obj->oid));
	अगर (fsck_object(obj, obj_buf->बफर, obj_buf->आकार, &fsck_options))
		die("fsck error in packed object");
	fsck_options.walk = check_object;
	अगर (fsck_walk(obj, शून्य, &fsck_options))
		die("Error on reachable objects of %s", oid_to_hex(&obj->oid));
	write_cached_object(obj, obj_buf);
	वापस 0;
}

अटल व्योम write_rest(व्योम)
{
	अचिन्हित i;
	क्रम (i = 0; i < nr_objects; i++) {
		अगर (obj_list[i].obj)
			check_object(obj_list[i].obj, OBJ_ANY, शून्य, शून्य);
	}
}

अटल व्योम added_object(अचिन्हित nr, क्रमागत object_type type,
			 व्योम *तथ्य, अचिन्हित दीर्घ आकार);

/*
 * Write out nr-th object from the list, now we know the contents
 * of it.  Under --strict, this buffers structured objects in-core,
 * to be checked at the end.
 */
अटल व्योम write_object(अचिन्हित nr, क्रमागत object_type type,
			 व्योम *buf, अचिन्हित दीर्घ आकार)
{
	अगर (!strict) {
		अगर (odb_write_object(the_repository->objects, buf, आकार, type,
				     &obj_list[nr].oid) < 0)
			die("failed to write object");
		added_object(nr, type, buf, आकार);
		मुक्त(buf);
		obj_list[nr].obj = शून्य;
	} अन्यथा अगर (type == OBJ_BLOB) {
		काष्ठा blob *blob;
		अगर (odb_write_object(the_repository->objects, buf, आकार, type,
				     &obj_list[nr].oid) < 0)
			die("failed to write object");
		added_object(nr, type, buf, आकार);
		मुक्त(buf);

		blob = lookup_blob(the_repository, &obj_list[nr].oid);
		अगर (blob)
			blob->object.flags |= FLAG_WRITTEN;
		अन्यथा
			die("invalid blob object");
		obj_list[nr].obj = शून्य;
	} अन्यथा {
		काष्ठा object *obj;
		पूर्णांक eaten;
		hash_object_file(the_hash_algo, buf, आकार, type,
				 &obj_list[nr].oid);
		added_object(nr, type, buf, आकार);
		obj = parse_object_बफर(the_repository, &obj_list[nr].oid,
					  type, आकार, buf,
					  &eaten);
		अगर (!obj)
			die("invalid %s", type_name(type));
		add_object_बफर(obj, buf, आकार);
		obj->flags |= FLAG_OPEN;
		obj_list[nr].obj = obj;
	}
}

अटल व्योम resolve_delta(अचिन्हित nr, क्रमागत object_type type,
			  व्योम *base, अचिन्हित दीर्घ base_आकार,
			  व्योम *delta, अचिन्हित दीर्घ delta_आकार_2)
{
	व्योम *परिणाम;
	माप_प्रकार परिणाम_आकार;

	परिणाम = patch_delta(base, base_आकार,
			     delta, delta_आकार_2,
			     &परिणाम_आकार);
	अगर (!परिणाम)
		die("failed to apply delta");
	मुक्त(delta);
	write_object(nr, type, परिणाम, परिणाम_आकार);
}

/*
 * We now know the contents of an object (which is nr-th in the pack);
 * resolve all the deltified objects that are based on it.
 */
अटल व्योम added_object(अचिन्हित nr, क्रमागत object_type type,
			 व्योम *तथ्य, अचिन्हित दीर्घ आकार)
{
	काष्ठा delta_info **p = &delta_list;
	काष्ठा delta_info *info;

	जबतक ((info = *p) != शून्य) {
		अगर (oideq(&info->base_oid, &obj_list[nr].oid) ||
		    info->base_offset == obj_list[nr].offset) {
			*p = info->अग्र;
			p = &delta_list;
			resolve_delta(info->nr, type, तथ्य, आकार,
				      info->delta, info->आकार);
			मुक्त(info);
			जारी;
		}
		p = &info->अग्र;
	}
}

अटल व्योम unpack_non_delta_entry(क्रमागत object_type type, अचिन्हित दीर्घ आकार,
				   अचिन्हित nr)
{
	व्योम *buf = get_तथ्य(आकार);

	अगर (buf)
		write_object(nr, type, buf, आकार);
}

काष्ठा zlib_stream {
	काष्ठा odb_stream base;
	git_zstream *zstream;
	पूर्णांक status;
};

अटल ssize_t zlib_stream_read(काष्ठा odb_stream *in_stream,
				अक्षर *buf, माप_प्रकार buf_len)
{
	काष्ठा zlib_stream *तथ्य = container_of(in_stream, काष्ठा zlib_stream, base);
	git_zstream *zstream = तथ्य->zstream;

	अगर (तथ्य->status != Z_OK)
		वापस 0;

	zstream->अग्र_out = (अचिन्हित अक्षर *) buf;
	zstream->avail_out = buf_len;

	जबतक (तथ्य->status == Z_OK && zstream->avail_out == buf_len) {
		zstream->अग्र_in = fill(1);
		zstream->avail_in = len;
		तथ्य->status = git_inflate(zstream, 0);
		use(len - zstream->avail_in);
	}

	वापस buf_len - zstream->avail_out;
}

अटल व्योम stream_blob(अचिन्हित दीर्घ आकार, अचिन्हित nr)
{
	git_zstream zstream = { 0 };
	काष्ठा zlib_stream in_stream = {
		.base = {
			.पढ़ो = zlib_stream_read,
			.आकार = आकार,
			.type = OBJ_BLOB,
		},
		.zstream = &zstream,
		.status = Z_OK,
	};
	काष्ठा obj_info *info = &obj_list[nr];

	git_inflate_init(&zstream);

	अगर (odb_write_object_stream(the_repository->objects, &in_stream.base, &info->oid))
		die(_("failed to write object in stream"));

	अगर (in_stream.status != Z_STREAM_END)
		die(_("inflate returned (%d)"), in_stream.status);
	git_inflate_end(&zstream);

	अगर (strict) {
		काष्ठा blob *blob = lookup_blob(the_repository, &info->oid);

		अगर (!blob)
			die(_("invalid blob object from stream"));
		blob->object.flags |= FLAG_WRITTEN;
	}
	info->obj = शून्य;
}

अटल पूर्णांक resolve_against_held(अचिन्हित nr, स्थिर काष्ठा object_id *base,
				व्योम *delta_तथ्य, अचिन्हित दीर्घ delta_आकार_2)
{
	काष्ठा object *obj;
	काष्ठा obj_बफर *obj_बफर;
	obj = lookup_object(the_repository, base);
	अगर (!obj)
		वापस 0;
	obj_बफर = lookup_object_बफर(obj);
	अगर (!obj_बफर)
		वापस 0;
	resolve_delta(nr, obj->type, obj_बफर->बफर,
		      obj_बफर->आकार, delta_तथ्य, delta_आकार_2);
	वापस 1;
}

अटल व्योम unpack_delta_entry(क्रमागत object_type type, अचिन्हित दीर्घ delta_आकार_2,
			       अचिन्हित nr)
{
	व्योम *delta_तथ्य, *base;
	अचिन्हित दीर्घ base_आकार;
	माप_प्रकार base_आकार_st = 0;
	काष्ठा object_id base_oid;

	अगर (type == OBJ_REF_DELTA) {
		oidread(&base_oid, fill(the_hash_algo->rawsz), the_repository->hash_algo);
		use(the_hash_algo->rawsz);
		delta_तथ्य = get_तथ्य(delta_आकार_2);
		अगर (!delta_तथ्य)
			वापस;
		अगर (odb_has_object(the_repository->objects, &base_oid,
				   ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR))
			; /* Ok we have this one */
		अन्यथा अगर (resolve_against_held(nr, &base_oid,
					      delta_तथ्य, delta_आकार_2))
			वापस; /* we are done */
		अन्यथा {
			/* cannot resolve yet --- queue it */
			oidclr(&obj_list[nr].oid, the_repository->hash_algo);
			add_delta_to_list(nr, &base_oid, 0, delta_तथ्य, delta_आकार_2);
			वापस;
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
		जबतक (c & 128) {
			base_offset += 1;
			अगर (!base_offset || MSB(base_offset, 7))
				die("offset value overflow for delta base object");
			pack = fill(1);
			c = *pack;
			use(1);
			base_offset = (base_offset << 7) + (c & 127);
		}
		base_offset = obj_list[nr].offset - base_offset;
		अगर (base_offset <= 0 || base_offset >= obj_list[nr].offset)
			die("offset value out of bound for delta base object");

		delta_तथ्य = get_तथ्य(delta_आकार_2);
		अगर (!delta_तथ्य)
			वापस;
		lo = 0;
		hi = nr;
		जबतक (lo < hi) {
			mid = lo + (hi - lo) / 2;
			अगर (base_offset < obj_list[mid].offset) {
				hi = mid;
			} अन्यथा अगर (base_offset > obj_list[mid].offset) {
				lo = mid + 1;
			} अन्यथा {
				oidcpy(&base_oid, &obj_list[mid].oid);
				base_found = !is_null_oid(&base_oid);
				अवरोध;
			}
		}
		अगर (!base_found) {
			/*
			 * The delta base object is itself a delta that
			 * has not been resolved yet.
			 */
			oidclr(&obj_list[nr].oid, the_repository->hash_algo);
			add_delta_to_list(nr, null_oid(the_hash_algo), base_offset,
					  delta_तथ्य, delta_आकार_2);
			वापस;
		}
	}

	अगर (resolve_against_held(nr, &base_oid, delta_तथ्य, delta_आकार_2))
		वापस;

	base = odb_read_object(the_repository->objects, &base_oid,
			       &type, &base_आकार_st);
	base_आकार = cast_आकार_t_to_ulong(base_आकार_st);
	अगर (!base) {
		error("failed to read delta-pack base object %s",
		      oid_to_hex(&base_oid));
		अगर (!recover)
			निकास(1);
		has_errors = 1;
		वापस;
	}
	resolve_delta(nr, type, base, base_आकार, delta_तथ्य, delta_आकार_2);
	मुक्त(base);
}

अटल व्योम unpack_one(अचिन्हित nr)
{
	अचिन्हित shift;
	अचिन्हित अक्षर *pack;
	माप_प्रकार आकार, c;
	क्रमागत object_type type;

	obj_list[nr].offset = consumed_bytes;

	pack = fill(1);
	c = *pack;
	use(1);
	type = (c >> 4) & 7;
	आकार = (c & 15);
	shift = 4;
	जबतक (c & 0x80) {
		अगर ((bitsizeof(माप_प्रकार) - 7) < shift)
			die(_("object size too large for this platform"));
		pack = fill(1);
		c = *pack;
		use(1);
		आकार += (c & 0x7f) << shift;
		shift += 7;
	}

	चयन (type) {
	हाल OBJ_BLOB:
		अगर (!dry_run &&
		    आकार > repo_settings_get_big_file_threshold(the_repository)) {
			stream_blob(आकार, nr);
			वापस;
		}
		/* fallthrough */
	हाल OBJ_COMMIT:
	हाल OBJ_वृक्ष:
	हाल OBJ_TAG:
		unpack_non_delta_entry(type, आकार, nr);
		वापस;
	हाल OBJ_REF_DELTA:
	हाल OBJ_OFS_DELTA:
		unpack_delta_entry(type, आकार, nr);
		वापस;
	शेष:
		error("bad object type %d", type);
		has_errors = 1;
		अगर (recover)
			वापस;
		निकास(1);
	}
}

अटल व्योम unpack_all(व्योम)
{
	पूर्णांक i;
	अचिन्हित अक्षर *hdr = fill(माप(काष्ठा pack_header));
	काष्ठा odb_transaction *transaction;

	अगर (get_be32(hdr) != PACK_SIGNATURE)
		die("bad pack file");
	hdr += 4;
	अगर (!pack_version_ok_native(get_be32(hdr)))
		die("unknown pack file version %"PRIu32,
		    get_be32(hdr));
	hdr += 4;
	nr_objects = get_be32(hdr);
	use(माप(काष्ठा pack_header));

	अगर (!quiet)
		progress = start_progress(the_repository,
					  _("Unpacking objects"), nr_objects);
	CALLOC_ARRAY(obj_list, nr_objects);
	odb_transaction_begin_or_die(the_repository->objects, &transaction, 0);
	क्रम (i = 0; i < nr_objects; i++) {
		unpack_one(i);
		display_progress(progress, i + 1);
	}
	odb_transaction_commit_and_finalize_or_die(transaction);
	stop_progress(&progress);

	अगर (delta_list)
		die("unresolved deltas left after unpacking");
}

पूर्णांक cmd_unpack_objects(पूर्णांक argc,
		       स्थिर अक्षर **argv,
		       स्थिर अक्षर *prefix UNUSED,
		       काष्ठा repository *repo)
{
	पूर्णांक i;
	काष्ठा object_id oid;
	काष्ठा git_hash_ctx tmp_ctx;

	disable_replace_refs();

	repo_config(the_repository, git_default_config, शून्य);

	quiet = !isatty(2);

	show_usage_if_asked(argc, argv, unpack_usage);

	fsck_options_init(&fsck_options, repo, FSCK_OPTIONS_STRICT);

	क्रम (i = 1 ; i < argc; i++) {
		स्थिर अक्षर *arg = argv[i];

		अगर (*arg == '-') {
			अगर (!म_भेद(arg, "-n")) {
				dry_run = 1;
				जारी;
			}
			अगर (!म_भेद(arg, "-q")) {
				quiet = 1;
				जारी;
			}
			अगर (!म_भेद(arg, "-r")) {
				recover = 1;
				जारी;
			}
			अगर (!म_भेद(arg, "--strict")) {
				strict = 1;
				जारी;
			}
			अगर (skip_prefix(arg, "--strict=", &arg)) {
				strict = 1;
				fsck_set_संदेश_types(&fsck_options, arg);
				जारी;
			}
			अगर (skip_prefix(arg, "--pack_header=", &arg)) {
				अगर (parse_pack_header_option(arg,
							     बफर, &len) < 0)
					die(_("bad --pack_header: %s"), arg);
				जारी;
			}
			अगर (skip_prefix(arg, "--max-input-size=", &arg)) {
				max_निवेश_आकार = strtoumax(arg, शून्य, 10);
				जारी;
			}
			usage(unpack_usage);
		}

		/* We don't take any non-flag arguments now.. Maybe some day */
		usage(unpack_usage);
	}
	git_hash_init(&ctx, the_hash_algo);
	unpack_all();
	git_hash_update(&ctx, बफर, offset);
	git_hash_init(&tmp_ctx, the_hash_algo);
	git_hash_clone(&tmp_ctx, &ctx);
	git_hash_final_oid(&oid, &tmp_ctx);
	अगर (strict) {
		write_rest();
		अगर (fsck_finish(&fsck_options))
			die(_("fsck error in pack objects"));
	}
	अगर (!hasheq(fill(the_hash_algo->rawsz), oid.hash,
		    the_repository->hash_algo))
		die("final sha1 did not match");
	use(the_hash_algo->rawsz);

	/* Write the last part of the buffer to stdout */
	write_in_full(1, बफर + offset, len);

	/* All done */
	वापस has_errors;
}
