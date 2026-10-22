#परिभाषय USE_THE_REPOSITORY_VARIABLE
#परिभाषय DISABLE_SIGN_COMPARE_WARNINGS

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

स्थिर पूर्णाङ्क dry_सततम्, quiet, recover, has_errors, strict;
स्थिर ध्रुव वर्ण unpack_usage[] = "git unpack-objects [-n] [-q] [-r] [--strict]";

स्थिर अचिह्नित वर्ण बफर[DEFAULT_IO_बफर_आकार];
स्थिर अचिह्नित पूर्णाङ्क विस्थापनम्, len;
स्थिर off_t consumed_अष्टकानि;
स्थिर off_t max_निवेशः_आकार;
स्थिर संरचना git_hash_ctx ctx;
स्थिर संरचना fsck_options fsck_options;
स्थिर संरचना प्रगतिः *प्रगतिः;

/*
 * When running under --strict mode, objects whose reachability are
 * suspect are kept in core without getting written in the object
 * store.
 */
संरचना obj_बफर {
	वर्ण *बफर;
	अचिह्नित दीर्घ आकार;
};

स्थिर संरचना decoration obj_decorate;

स्थिर संरचना obj_बफर *lookup_object_बफर(संरचना object *आधारः)
{
	प्रत्यागम lookup_decoration(&obj_decorate, आधारः);
}

स्थिर शून्य सङ्कलय_object_बफर(संरचना object *object, वर्ण *बफर, अचिह्नित दीर्घ आकार)
{
	संरचना obj_बफर *obj;
	CALLOC_ARRAY(obj, 1);
	obj->बफर = बफर;
	obj->आकार = आकार;
	यदि (सङ्कलय_decoration(&obj_decorate, object, obj))
		die("object %s tried to add buffer twice!", oid_गन्तव्यम्_hex(&object->oid));
}

/*
 * Make sure at least "min" bytes are available in the buffer, and
 * return the pointer to the buffer.
 */
स्थिर शून्य *fill(पूर्णाङ्क min)
{
	यदि (min <= len)
		प्रत्यागम बफर + विस्थापनम्;
	यदि (min > परिमाण(बफर))
		die("cannot fill %d bytes", min);
	यदि (विस्थापनम्) {
		git_hash_update(&ctx, बफर, विस्थापनम्);
		memmove(बफर, बफर + विस्थापनम्, len);
		विस्थापनम् = 0;
	}
	कुरु {
		ssize_t ret = xread(0, बफर + len, परिमाण(बफर) - len);
		यदि (ret <= 0) {
			यदि (!ret)
				die("early EOF");
			die_errno("read error on input");
		}
		len += ret;
	} यावत् (len < min);
	प्रत्यागम बफर;
}

स्थिर शून्य use(पूर्णाङ्क अष्टकानि)
{
	यदि (अष्टकानि > len)
		die("used more bytes than were available");
	len -= अष्टकानि;
	विस्थापनम् += अष्टकानि;

	/* make sure off_t is sufficiently large not to wrap */
	यदि (signed_सङ्कलय_overflows(consumed_अष्टकानि, अष्टकानि))
		die("pack too large for current definition of off_t");
	consumed_अष्टकानि += अष्टकानि;
	यदि (max_निवेशः_आकार && consumed_अष्टकानि > max_निवेशः_आकार)
		die(_("pack exceeds maximum allowed size"));
	display_throughput(प्रगतिः, consumed_अष्टकानि);
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
स्थिर शून्य *get_दत्त(अचिह्नित दीर्घ आकार)
{
	git_zstream प्रवाहः;
	अचिह्नित दीर्घ bufsize = dry_सततम् && आकार > 8192 ? 8192 : आकार;
	शून्य *buf = xmallocz(bufsize);

	स्मृतिं_पूरय(&प्रवाहः, 0, परिमाण(प्रवाहः));

	प्रवाहः.अग्र_बहिर्गमनम् = buf;
	प्रवाहः.avail_बहिर्गमनम् = bufsize;
	प्रवाहः.अग्र_अन्तर् = fill(1);
	प्रवाहः.avail_अन्तर् = len;
	git_inflate_init(&प्रवाहः);

	प्रति (;;) {
		पूर्णाङ्क ret = git_inflate(&प्रवाहः, 0);
		use(len - प्रवाहः.avail_अन्तर्);
		यदि (प्रवाहः.योग_बहिर्गमनम् == आकार && ret == Z_प्रवाहः_अन्तः)
			भङ्ग;
		यदि (ret != Z_OK) {
			error("inflate returned %d", ret);
			FREE_AND_NULL(buf);
			यदि (!recover)
				निर्गच्छ(1);
			has_errors = 1;
			भङ्ग;
		}
		प्रवाहः.अग्र_अन्तर् = fill(1);
		प्रवाहः.avail_अन्तर् = len;
		यदि (dry_सततम्) {
			/* reuse the buffer in dry_run mode */
			प्रवाहः.अग्र_बहिर्गमनम् = buf;
			प्रवाहः.avail_बहिर्गमनम् = bufsize > आकार - प्रवाहः.योग_बहिर्गमनम् ?
						   आकार - प्रवाहः.योग_बहिर्गमनम् :
						   bufsize;
		}
	}
	git_inflate_अन्तः(&प्रवाहः);
	यदि (dry_सततम्)
		FREE_AND_NULL(buf);
	प्रत्यागम buf;
}

संरचना delta_info {
	संरचना object_id आधारः_oid;
	अचिह्नित nr;
	off_t आधारः_विस्थापनम्;
	अचिह्नित दीर्घ आकार;
	शून्य *delta;
	संरचना delta_info *अग्र;
};

स्थिर संरचना delta_info *delta_list;

स्थिर शून्य सङ्कलय_delta_गन्तव्यम्_list(अचिह्नित nr, ध्रुव संरचना object_id *आधारः_oid,
			      off_t आधारः_विस्थापनम्,
			      शून्य *delta, अचिह्नित दीर्घ आकार)
{
	संरचना delta_info *info = xmalloc(परिमाण(*info));

	oidcpy(&info->आधारः_oid, आधारः_oid);
	info->आधारः_विस्थापनम् = आधारः_विस्थापनम्;
	info->आकार = आकार;
	info->delta = delta;
	info->nr = nr;
	info->अग्र = delta_list;
	delta_list = info;
}

संरचना obj_info {
	off_t विस्थापनम्;
	संरचना object_id oid;
	संरचना object *obj;
};

/* Remember to update object flag allocation in object.h */
#परिभाषय FLAG_OPEN (1u<<20)
#परिभाषय FLAG_WRITTEN (1u<<21)

स्थिर संरचना obj_info *obj_list;
स्थिर अचिह्नित nr_objects;

/*
 * Called only from check_object() after it verified this object
 * is Ok.
 */
स्थिर शून्य write_cached_object(संरचना object *obj, संरचना obj_बफर *obj_buf)
{
	संरचना object_id oid;

	यदि (odb_write_object(the_repository->objects, obj_buf->बफर, obj_buf->आकार,
			     obj->type, &oid) < 0)
		die("failed to write object %s", oid_गन्तव्यम्_hex(&obj->oid));
	obj->flags |= FLAG_WRITTEN;
}

/*
 * At the very end of the processing, write_rest() scans the objects
 * that have reachability requirements and calls this function.
 * Verify its reachability and validity recursively and write it out.
 */
स्थिर पूर्णाङ्क check_object(संरचना object *obj, गणना object_type type,
			शून्य *दत्त अप्रयुक्तम्,
			संरचना fsck_options *options अप्रयुक्तम्)
{
	संरचना obj_बफर *obj_buf;

	यदि (!obj)
		प्रत्यागम 1;

	यदि (obj->flags & FLAG_WRITTEN)
		प्रत्यागम 0;

	यदि (type != OBJ_ANY && obj->type != type)
		die("object type mismatch");

	यदि (!(obj->flags & FLAG_OPEN)) {
		आकार_t आकार;
		पूर्णाङ्क type = odb_read_object_info(the_repository->objects, &obj->oid, &आकार);
		यदि (type != obj->type || type <= 0)
			die("object of unexpected type");
		obj->flags |= FLAG_WRITTEN;
		प्रत्यागम 0;
	}

	obj_buf = lookup_object_बफर(obj);
	यदि (!obj_buf)
		die("Whoops! Cannot find object '%s'", oid_गन्तव्यम्_hex(&obj->oid));
	यदि (fsck_object(obj, obj_buf->बफर, obj_buf->आकार, &fsck_options))
		die("fsck error in packed object");
	fsck_options.walk = check_object;
	यदि (fsck_walk(obj, रिक्त, &fsck_options))
		die("Error on reachable objects of %s", oid_गन्तव्यम्_hex(&obj->oid));
	write_cached_object(obj, obj_buf);
	प्रत्यागम 0;
}

स्थिर शून्य write_rest(शून्य)
{
	अचिह्नित i;
	प्रति (i = 0; i < nr_objects; i++) {
		यदि (obj_list[i].obj)
			check_object(obj_list[i].obj, OBJ_ANY, रिक्त, रिक्त);
	}
}

स्थिर शून्य added_object(अचिह्नित nr, गणना object_type type,
			 शून्य *दत्त, अचिह्नित दीर्घ आकार);

/*
 * Write out nr-th object from the list, now we know the contents
 * of it.  Under --strict, this buffers structured objects in-core,
 * to be checked at the end.
 */
स्थिर शून्य write_object(अचिह्नित nr, गणना object_type type,
			 शून्य *buf, अचिह्नित दीर्घ आकार)
{
	यदि (!strict) {
		यदि (odb_write_object(the_repository->objects, buf, आकार, type,
				     &obj_list[nr].oid) < 0)
			die("failed to write object");
		added_object(nr, type, buf, आकार);
		मुक्त(buf);
		obj_list[nr].obj = रिक्त;
	} अन्यथा यदि (type == OBJ_BLOB) {
		संरचना blob *blob;
		यदि (odb_write_object(the_repository->objects, buf, आकार, type,
				     &obj_list[nr].oid) < 0)
			die("failed to write object");
		added_object(nr, type, buf, आकार);
		मुक्त(buf);

		blob = lookup_blob(the_repository, &obj_list[nr].oid);
		यदि (blob)
			blob->object.flags |= FLAG_WRITTEN;
		अन्यथा
			die("invalid blob object");
		obj_list[nr].obj = रिक्त;
	} अन्यथा {
		संरचना object *obj;
		पूर्णाङ्क eaten;
		hash_object_file(the_hash_algo, buf, आकार, type,
				 &obj_list[nr].oid);
		added_object(nr, type, buf, आकार);
		obj = parse_object_बफर(the_repository, &obj_list[nr].oid,
					  type, आकार, buf,
					  &eaten);
		यदि (!obj)
			die("invalid %s", type_name(type));
		सङ्कलय_object_बफर(obj, buf, आकार);
		obj->flags |= FLAG_OPEN;
		obj_list[nr].obj = obj;
	}
}

स्थिर शून्य resolve_delta(अचिह्नित nr, गणना object_type type,
			  शून्य *आधारः, अचिह्नित दीर्घ आधारः_आकार,
			  शून्य *delta, अचिह्नित दीर्घ delta_आकार_2)
{
	शून्य *परिणाम;
	आकार_t परिणाम_आकार;

	परिणाम = patch_delta(आधारः, आधारः_आकार,
			     delta, delta_आकार_2,
			     &परिणाम_आकार);
	यदि (!परिणाम)
		die("failed to apply delta");
	मुक्त(delta);
	write_object(nr, type, परिणाम, परिणाम_आकार);
}

/*
 * We now know the contents of an object (which is nr-th in the pack);
 * resolve all the deltified objects that are based on it.
 */
स्थिर शून्य added_object(अचिह्नित nr, गणना object_type type,
			 शून्य *दत्त, अचिह्नित दीर्घ आकार)
{
	संरचना delta_info **p = &delta_list;
	संरचना delta_info *info;

	यावत् ((info = *p) != रिक्त) {
		यदि (oideq(&info->आधारः_oid, &obj_list[nr].oid) ||
		    info->आधारः_विस्थापनम् == obj_list[nr].विस्थापनम्) {
			*p = info->अग्र;
			p = &delta_list;
			resolve_delta(info->nr, type, दत्त, आकार,
				      info->delta, info->आकार);
			मुक्त(info);
			अनुवर्त;
		}
		p = &info->अग्र;
	}
}

स्थिर शून्य unpack_non_delta_प्रविष्टिः(गणना object_type type, अचिह्नित दीर्घ आकार,
				   अचिह्नित nr)
{
	शून्य *buf = get_दत्त(आकार);

	यदि (buf)
		write_object(nr, type, buf, आकार);
}

संरचना zlib_प्रवाहः {
	संरचना odb_प्रवाहः आधारः;
	git_zstream *zstream;
	पूर्णाङ्क स्थितिः;
};

स्थिर ssize_t zlib_प्रवाहः_read(संरचना odb_प्रवाहः *अन्तर्_प्रवाहः,
				वर्ण *buf, आकार_t buf_len)
{
	संरचना zlib_प्रवाहः *दत्त = container_सम्बन्धि(अन्तर्_प्रवाहः, संरचना zlib_प्रवाहः, आधारः);
	git_zstream *zstream = दत्त->zstream;

	यदि (दत्त->स्थितिः != Z_OK)
		प्रत्यागम 0;

	zstream->अग्र_बहिर्गमनम् = (अचिह्नित वर्ण *) buf;
	zstream->avail_बहिर्गमनम् = buf_len;

	यावत् (दत्त->स्थितिः == Z_OK && zstream->avail_बहिर्गमनम् == buf_len) {
		zstream->अग्र_अन्तर् = fill(1);
		zstream->avail_अन्तर् = len;
		दत्त->स्थितिः = git_inflate(zstream, 0);
		use(len - zstream->avail_अन्तर्);
	}

	प्रत्यागम buf_len - zstream->avail_बहिर्गमनम्;
}

स्थिर शून्य प्रवाहः_blob(अचिह्नित दीर्घ आकार, अचिह्नित nr)
{
	git_zstream zstream = { 0 };
	संरचना zlib_प्रवाहः अन्तर्_प्रवाहः = {
		.आधारः = {
			.पठ = zlib_प्रवाहः_read,
			.आकार = आकार,
			.type = OBJ_BLOB,
		},
		.zstream = &zstream,
		.स्थितिः = Z_OK,
	};
	संरचना obj_info *info = &obj_list[nr];

	git_inflate_init(&zstream);

	यदि (odb_write_object_प्रवाहः(the_repository->objects, &अन्तर्_प्रवाहः.आधारः, &info->oid))
		die(_("failed to write object in stream"));

	यदि (अन्तर्_प्रवाहः.स्थितिः != Z_प्रवाहः_अन्तः)
		die(_("inflate returned (%d)"), अन्तर्_प्रवाहः.स्थितिः);
	git_inflate_अन्तः(&zstream);

	यदि (strict) {
		संरचना blob *blob = lookup_blob(the_repository, &info->oid);

		यदि (!blob)
			die(_("invalid blob object from stream"));
		blob->object.flags |= FLAG_WRITTEN;
	}
	info->obj = रिक्त;
}

स्थिर पूर्णाङ्क resolve_against_held(अचिह्नित nr, ध्रुव संरचना object_id *आधारः,
				शून्य *delta_दत्त, अचिह्नित दीर्घ delta_आकार_2)
{
	संरचना object *obj;
	संरचना obj_बफर *obj_बफर;
	obj = lookup_object(the_repository, आधारः);
	यदि (!obj)
		प्रत्यागम 0;
	obj_बफर = lookup_object_बफर(obj);
	यदि (!obj_बफर)
		प्रत्यागम 0;
	resolve_delta(nr, obj->type, obj_बफर->बफर,
		      obj_बफर->आकार, delta_दत्त, delta_आकार_2);
	प्रत्यागम 1;
}

स्थिर शून्य unpack_delta_प्रविष्टिः(गणना object_type type, अचिह्नित दीर्घ delta_आकार_2,
			       अचिह्नित nr)
{
	शून्य *delta_दत्त, *आधारः;
	अचिह्नित दीर्घ आधारः_आकार;
	आकार_t आधारः_आकार_st = 0;
	संरचना object_id आधारः_oid;

	यदि (type == OBJ_REF_DELTA) {
		oidread(&आधारः_oid, fill(the_hash_algo->rawsz), the_repository->hash_algo);
		use(the_hash_algo->rawsz);
		delta_दत्त = get_दत्त(delta_आकार_2);
		यदि (!delta_दत्त)
			प्रत्यागम;
		यदि (odb_has_object(the_repository->objects, &आधारः_oid,
				   ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR))
			; /* Ok we have this one */
		अन्यथा यदि (resolve_against_held(nr, &आधारः_oid,
					      delta_दत्त, delta_आकार_2))
			प्रत्यागम; /* we are done */
		अन्यथा {
			/* cannot resolve yet --- queue it */
			oidclr(&obj_list[nr].oid, the_repository->hash_algo);
			सङ्कलय_delta_गन्तव्यम्_list(nr, &आधारः_oid, 0, delta_दत्त, delta_आकार_2);
			प्रत्यागम;
		}
	} अन्यथा {
		अचिह्नित आधारः_प्राप्तम् = 0;
		अचिह्नित वर्ण *pack, c;
		off_t आधारः_विस्थापनम्;
		अचिह्नित lo, mid, hi;

		pack = fill(1);
		c = *pack;
		use(1);
		आधारः_विस्थापनम् = c & 127;
		यावत् (c & 128) {
			आधारः_विस्थापनम् += 1;
			यदि (!आधारः_विस्थापनम् || MSB(आधारः_विस्थापनम्, 7))
				die("offset value overflow for delta base object");
			pack = fill(1);
			c = *pack;
			use(1);
			आधारः_विस्थापनम् = (आधारः_विस्थापनम् << 7) + (c & 127);
		}
		आधारः_विस्थापनम् = obj_list[nr].विस्थापनम् - आधारः_विस्थापनम्;
		यदि (आधारः_विस्थापनम् <= 0 || आधारः_विस्थापनम् >= obj_list[nr].विस्थापनम्)
			die("offset value out of bound for delta base object");

		delta_दत्त = get_दत्त(delta_आकार_2);
		यदि (!delta_दत्त)
			प्रत्यागम;
		lo = 0;
		hi = nr;
		यावत् (lo < hi) {
			mid = lo + (hi - lo) / 2;
			यदि (आधारः_विस्थापनम् < obj_list[mid].विस्थापनम्) {
				hi = mid;
			} अन्यथा यदि (आधारः_विस्थापनम् > obj_list[mid].विस्थापनम्) {
				lo = mid + 1;
			} अन्यथा {
				oidcpy(&आधारः_oid, &obj_list[mid].oid);
				आधारः_प्राप्तम् = !is_null_oid(&आधारः_oid);
				भङ्ग;
			}
		}
		यदि (!आधारः_प्राप्तम्) {
			/*
			 * The delta base object is itself a delta that
			 * has not been resolved yet.
			 */
			oidclr(&obj_list[nr].oid, the_repository->hash_algo);
			सङ्कलय_delta_गन्तव्यम्_list(nr, null_oid(the_hash_algo), आधारः_विस्थापनम्,
					  delta_दत्त, delta_आकार_2);
			प्रत्यागम;
		}
	}

	यदि (resolve_against_held(nr, &आधारः_oid, delta_दत्त, delta_आकार_2))
		प्रत्यागम;

	आधारः = odb_read_object(the_repository->objects, &आधारः_oid,
			       &type, &आधारः_आकार_st);
	आधारः_आकार = cast_आकार_t_गन्तव्यम्_ulong(आधारः_आकार_st);
	यदि (!आधारः) {
		error("failed to read delta-pack base object %s",
		      oid_गन्तव्यम्_hex(&आधारः_oid));
		यदि (!recover)
			निर्गच्छ(1);
		has_errors = 1;
		प्रत्यागम;
	}
	resolve_delta(nr, type, आधारः, आधारः_आकार, delta_दत्त, delta_आकार_2);
	मुक्त(आधारः);
}

स्थिर शून्य unpack_one(अचिह्नित nr)
{
	अचिह्नित सरणम्;
	अचिह्नित वर्ण *pack;
	आकार_t आकार, c;
	गणना object_type type;

	obj_list[nr].विस्थापनम् = consumed_अष्टकानि;

	pack = fill(1);
	c = *pack;
	use(1);
	type = (c >> 4) & 7;
	आकार = (c & 15);
	सरणम् = 4;
	यावत् (c & 0x80) {
		यदि ((bitsizeof(आकार_t) - 7) < सरणम्)
			die(_("object size too large for this platform"));
		pack = fill(1);
		c = *pack;
		use(1);
		आकार += (c & 0x7f) << सरणम्;
		सरणम् += 7;
	}

	विकल्प (type) {
	पक्ष OBJ_BLOB:
		यदि (!dry_सततम् &&
		    आकार > repo_settings_get_big_file_देहली(the_repository)) {
			प्रवाहः_blob(आकार, nr);
			प्रत्यागम;
		}
		/* fallthrough */
	पक्ष OBJ_COMMIT:
	पक्ष OBJ_वृक्षः:
	पक्ष OBJ_चिह्नकम्:
		unpack_non_delta_प्रविष्टिः(type, आकार, nr);
		प्रत्यागम;
	पक्ष OBJ_REF_DELTA:
	पक्ष OBJ_OFS_DELTA:
		unpack_delta_प्रविष्टिः(type, आकार, nr);
		प्रत्यागम;
	स्वभाव:
		error("bad object type %d", type);
		has_errors = 1;
		यदि (recover)
			प्रत्यागम;
		निर्गच्छ(1);
	}
}

स्थिर शून्य unpack_all(शून्य)
{
	पूर्णाङ्क i;
	अचिह्नित वर्ण *hdr = fill(परिमाण(संरचना pack_शीर्षकम्));
	संरचना odb_transaction *transaction;

	यदि (get_be32(hdr) != PACK_SIGNATURE)
		die("bad pack file");
	hdr += 4;
	यदि (!pack_संस्करणम्_ok_native(get_be32(hdr)))
		die("unknown pack file version %"PRIu32,
		    get_be32(hdr));
	hdr += 4;
	nr_objects = get_be32(hdr);
	use(परिमाण(संरचना pack_शीर्षकम्));

	यदि (!quiet)
		प्रगतिः = start_प्रगतिः(the_repository,
					  _("Unpacking objects"), nr_objects);
	CALLOC_ARRAY(obj_list, nr_objects);
	odb_transaction_begin_or_die(the_repository->objects, &transaction, 0);
	प्रति (i = 0; i < nr_objects; i++) {
		unpack_one(i);
		display_प्रगतिः(प्रगतिः, i + 1);
	}
	odb_transaction_commit_and_finalize_or_die(transaction);
	stop_प्रगतिः(&प्रगतिः);

	यदि (delta_list)
		die("unresolved deltas left after unpacking");
}

पूर्णाङ्क cmd_unpack_objects(पूर्णाङ्क argc,
		       ध्रुव वर्ण **argv,
		       ध्रुव वर्ण *उपसर्गः अप्रयुक्तम्,
		       संरचना repository *repo)
{
	पूर्णाङ्क i;
	संरचना object_id oid;
	संरचना git_hash_ctx tmp_ctx;

	disable_replace_refs();

	repo_config(the_repository, git_default_config, रिक्त);

	quiet = !isatty(2);

	show_usage_if_asked(argc, argv, unpack_usage);

	fsck_options_init(&fsck_options, repo, FSCK_OPTIONS_STRICT);

	प्रति (i = 1 ; i < argc; i++) {
		ध्रुव वर्ण *arg = argv[i];

		यदि (*arg == '-') {
			यदि (!वाक्यं_तुलय(arg, "-n")) {
				dry_सततम् = 1;
				अनुवर्त;
			}
			यदि (!वाक्यं_तुलय(arg, "-q")) {
				quiet = 1;
				अनुवर्त;
			}
			यदि (!वाक्यं_तुलय(arg, "-r")) {
				recover = 1;
				अनुवर्त;
			}
			यदि (!वाक्यं_तुलय(arg, "--strict")) {
				strict = 1;
				अनुवर्त;
			}
			यदि (skip_उपसर्गः(arg, "--strict=", &arg)) {
				strict = 1;
				fsck_समुच्चयः_सन्देशः_types(&fsck_options, arg);
				अनुवर्त;
			}
			यदि (skip_उपसर्गः(arg, "--pack_header=", &arg)) {
				यदि (parse_pack_शीर्षकम्_option(arg,
							     बफर, &len) < 0)
					die(_("bad --pack_header: %s"), arg);
				अनुवर्त;
			}
			यदि (skip_उपसर्गः(arg, "--max-input-size=", &arg)) {
				max_निवेशः_आकार = strtoumax(arg, रिक्त, 10);
				अनुवर्त;
			}
			usage(unpack_usage);
		}

		/* We don't take any non-flag arguments now.. Maybe some day */
		usage(unpack_usage);
	}
	git_hash_init(&ctx, the_hash_algo);
	unpack_all();
	git_hash_update(&ctx, बफर, विस्थापनम्);
	git_hash_init(&tmp_ctx, the_hash_algo);
	git_hash_clone(&tmp_ctx, &ctx);
	git_hash_final_oid(&oid, &tmp_ctx);
	यदि (strict) {
		write_rest();
		यदि (fsck_finish(&fsck_options))
			die(_("fsck error in pack objects"));
	}
	यदि (!hasheq(fill(the_hash_algo->rawsz), oid.hash,
		    the_repository->hash_algo))
		die("final sha1 did not match");
	use(the_hash_algo->rawsz);

	/* Write the last part of the buffer to stdout */
	write_अन्तर्_पूर्णम्(1, बफर + विस्थापनम्, len);

	/* All done */
	प्रत्यागम has_errors;
}
