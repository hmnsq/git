#وصف_ڪريو USE_THE_REPOSITORY_VARIABLE
#وصف_ڪريو DISABLE_SIGN_COMPARE_WARNINGS

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

ساكن صحيح dry_لاڳيتو, quiet, recover, has_errors, strict;
ساكن ثابت حرف unpack_usage[] = "git unpack-objects [-n] [-q] [-r] [--strict]";

ساكن بنانشان حرف بفر[DEFAULT_IO_بفر_سائيز];
ساكن بنانشان صحيح وٿي, len;
ساكن off_t consumed_بائيٽون;
ساكن off_t max_داخلا_سائيز;
ساكن بنية git_hash_ctx ctx;
ساكن بنية fsck_options fsck_options;
ساكن بنية ترقي *ترقي;

/*
 * When running under --strict mode, objects whose reachability are
 * suspect are kept in core without getting written in the object
 * store.
 */
بنية obj_بفر {
	حرف *بفر;
	بنانشان ڊگهو سائيز;
};

ساكن بنية decoration obj_decorate;

ساكن بنية obj_بفر *lookup_object_بفر(بنية object *بنياد)
{
	أرجع lookup_decoration(&obj_decorate, بنياد);
}

ساكن فراغ جوڙيو_object_بفر(بنية object *object, حرف *بفر, بنانشان ڊگهو سائيز)
{
	بنية obj_بفر *obj;
	CALLOC_ARRAY(obj, 1);
	obj->بفر = بفر;
	obj->سائيز = سائيز;
	إذا (جوڙيو_decoration(&obj_decorate, object, obj))
		die("object %s tried to add buffer twice!", oid_منزل_hex(&object->oid));
}

/*
 * Make sure at least "min" bytes are available in the buffer, and
 * return the pointer to the buffer.
 */
ساكن فراغ *fill(صحيح min)
{
	إذا (min <= len)
		أرجع بفر + وٿي;
	إذا (min > حجم(بفر))
		die("cannot fill %d bytes", min);
	إذا (وٿي) {
		git_hash_update(&ctx, بفر, وٿي);
		memmove(بفر, بفر + وٿي, len);
		وٿي = 0;
	}
	افعل {
		ssize_t ret = xread(0, بفر + len, حجم(بفر) - len);
		إذا (ret <= 0) {
			إذا (!ret)
				die("early EOF");
			die_errno("read error on input");
		}
		len += ret;
	} طالما (len < min);
	أرجع بفر;
}

ساكن فراغ use(صحيح بائيٽون)
{
	إذا (بائيٽون > len)
		die("used more bytes than were available");
	len -= بائيٽون;
	وٿي += بائيٽون;

	/* make sure off_t is sufficiently large not to wrap */
	إذا (signed_جوڙيو_overflows(consumed_بائيٽون, بائيٽون))
		die("pack too large for current definition of off_t");
	consumed_بائيٽون += بائيٽون;
	إذا (max_داخلا_سائيز && consumed_بائيٽون > max_داخلا_سائيز)
		die(_("pack exceeds maximum allowed size"));
	display_throughput(ترقي, consumed_بائيٽون);
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
ساكن فراغ *get_معلومات(بنانشان ڊگهو سائيز)
{
	git_zstream وهڪرو;
	بنانشان ڊگهو bufsize = dry_لاڳيتو && سائيز > 8192 ? 8192 : سائيز;
	فراغ *buf = xmallocz(bufsize);

	memset(&وهڪرو, 0, حجم(وهڪرو));

	وهڪرو.ايندڙ_ٻاهريون = buf;
	وهڪرو.avail_ٻاهريون = bufsize;
	وهڪرو.ايندڙ_اندريون = fill(1);
	وهڪرو.avail_اندريون = len;
	git_inflate_init(&وهڪرو);

	لكل (;;) {
		صحيح ret = git_inflate(&وهڪرو, 0);
		use(len - وهڪرو.avail_اندريون);
		إذا (وهڪرو.ڪل_ٻاهريون == سائيز && ret == Z_وهڪرو_پڇاڙي)
			اكسر;
		إذا (ret != Z_OK) {
			error("inflate returned %d", ret);
			FREE_AND_NULL(buf);
			إذا (!recover)
				ٻاهر_نڪرو(1);
			has_errors = 1;
			اكسر;
		}
		وهڪرو.ايندڙ_اندريون = fill(1);
		وهڪرو.avail_اندريون = len;
		إذا (dry_لاڳيتو) {
			/* reuse the buffer in dry_run mode */
			وهڪرو.ايندڙ_ٻاهريون = buf;
			وهڪرو.avail_ٻاهريون = bufsize > سائيز - وهڪرو.ڪل_ٻاهريون ?
						   سائيز - وهڪرو.ڪل_ٻاهريون :
						   bufsize;
		}
	}
	git_inflate_پڇاڙي(&وهڪرو);
	إذا (dry_لاڳيتو)
		FREE_AND_NULL(buf);
	أرجع buf;
}

بنية delta_info {
	بنية object_id بنياد_oid;
	بنانشان nr;
	off_t بنياد_وٿي;
	بنانشان ڊگهو سائيز;
	فراغ *delta;
	بنية delta_info *ايندڙ;
};

ساكن بنية delta_info *delta_list;

ساكن فراغ جوڙيو_delta_منزل_list(بنانشان nr, ثابت بنية object_id *بنياد_oid,
			      off_t بنياد_وٿي,
			      فراغ *delta, بنانشان ڊگهو سائيز)
{
	بنية delta_info *info = xmalloc(حجم(*info));

	oidcpy(&info->بنياد_oid, بنياد_oid);
	info->بنياد_وٿي = بنياد_وٿي;
	info->سائيز = سائيز;
	info->delta = delta;
	info->nr = nr;
	info->ايندڙ = delta_list;
	delta_list = info;
}

بنية obj_info {
	off_t وٿي;
	بنية object_id oid;
	بنية object *obj;
};

/* Remember to update object flag allocation in object.h */
#وصف_ڪريو FLAG_OPEN (1u<<20)
#وصف_ڪريو FLAG_WRITTEN (1u<<21)

ساكن بنية obj_info *obj_list;
ساكن بنانشان nr_objects;

/*
 * Called only from check_object() after it verified this object
 * is Ok.
 */
ساكن فراغ write_cached_object(بنية object *obj, بنية obj_بفر *obj_buf)
{
	بنية object_id oid;

	إذا (odb_write_object(the_repository->objects, obj_buf->بفر, obj_buf->سائيز,
			     obj->type, &oid) < 0)
		die("failed to write object %s", oid_منزل_hex(&obj->oid));
	obj->flags |= FLAG_WRITTEN;
}

/*
 * At the very end of the processing, write_rest() scans the objects
 * that have reachability requirements and calls this function.
 * Verify its reachability and validity recursively and write it out.
 */
ساكن صحيح check_object(بنية object *obj, تعداد object_type type,
			فراغ *معلومات اڻ_استعمال_ٿيل,
			بنية fsck_options *options اڻ_استعمال_ٿيل)
{
	بنية obj_بفر *obj_buf;

	إذا (!obj)
		أرجع 1;

	إذا (obj->flags & FLAG_WRITTEN)
		أرجع 0;

	إذا (type != OBJ_ANY && obj->type != type)
		die("object type mismatch");

	إذا (!(obj->flags & FLAG_OPEN)) {
		سائيز_t سائيز;
		صحيح type = odb_read_object_info(the_repository->objects, &obj->oid, &سائيز);
		إذا (type != obj->type || type <= 0)
			die("object of unexpected type");
		obj->flags |= FLAG_WRITTEN;
		أرجع 0;
	}

	obj_buf = lookup_object_بفر(obj);
	إذا (!obj_buf)
		die("Whoops! Cannot find object '%s'", oid_منزل_hex(&obj->oid));
	إذا (fsck_object(obj, obj_buf->بفر, obj_buf->سائيز, &fsck_options))
		die("fsck error in packed object");
	fsck_options.walk = check_object;
	إذا (fsck_walk(obj, عدم, &fsck_options))
		die("Error on reachable objects of %s", oid_منزل_hex(&obj->oid));
	write_cached_object(obj, obj_buf);
	أرجع 0;
}

ساكن فراغ write_rest(فراغ)
{
	بنانشان i;
	لكل (i = 0; i < nr_objects; i++) {
		إذا (obj_list[i].obj)
			check_object(obj_list[i].obj, OBJ_ANY, عدم, عدم);
	}
}

ساكن فراغ added_object(بنانشان nr, تعداد object_type type,
			 فراغ *معلومات, بنانشان ڊگهو سائيز);

/*
 * Write out nr-th object from the list, now we know the contents
 * of it.  Under --strict, this buffers structured objects in-core,
 * to be checked at the end.
 */
ساكن فراغ write_object(بنانشان nr, تعداد object_type type,
			 فراغ *buf, بنانشان ڊگهو سائيز)
{
	إذا (!strict) {
		إذا (odb_write_object(the_repository->objects, buf, سائيز, type,
				     &obj_list[nr].oid) < 0)
			die("failed to write object");
		added_object(nr, type, buf, سائيز);
		حرر(buf);
		obj_list[nr].obj = عدم;
	} وإلا إذا (type == OBJ_BLOB) {
		بنية blob *blob;
		إذا (odb_write_object(the_repository->objects, buf, سائيز, type,
				     &obj_list[nr].oid) < 0)
			die("failed to write object");
		added_object(nr, type, buf, سائيز);
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
		hash_object_file(the_hash_algo, buf, سائيز, type,
				 &obj_list[nr].oid);
		added_object(nr, type, buf, سائيز);
		obj = parse_object_بفر(the_repository, &obj_list[nr].oid,
					  type, سائيز, buf,
					  &eaten);
		إذا (!obj)
			die("invalid %s", type_name(type));
		جوڙيو_object_بفر(obj, buf, سائيز);
		obj->flags |= FLAG_OPEN;
		obj_list[nr].obj = obj;
	}
}

ساكن فراغ resolve_delta(بنانشان nr, تعداد object_type type,
			  فراغ *بنياد, بنانشان ڊگهو بنياد_سائيز,
			  فراغ *delta, بنانشان ڊگهو delta_سائيز_2)
{
	فراغ *نتيجو;
	سائيز_t نتيجو_سائيز;

	نتيجو = patch_delta(بنياد, بنياد_سائيز,
			     delta, delta_سائيز_2,
			     &نتيجو_سائيز);
	إذا (!نتيجو)
		die("failed to apply delta");
	حرر(delta);
	write_object(nr, type, نتيجو, نتيجو_سائيز);
}

/*
 * We now know the contents of an object (which is nr-th in the pack);
 * resolve all the deltified objects that are based on it.
 */
ساكن فراغ added_object(بنانشان nr, تعداد object_type type,
			 فراغ *معلومات, بنانشان ڊگهو سائيز)
{
	بنية delta_info **p = &delta_list;
	بنية delta_info *info;

	طالما ((info = *p) != عدم) {
		إذا (oideq(&info->بنياد_oid, &obj_list[nr].oid) ||
		    info->بنياد_وٿي == obj_list[nr].وٿي) {
			*p = info->ايندڙ;
			p = &delta_list;
			resolve_delta(info->nr, type, معلومات, سائيز,
				      info->delta, info->سائيز);
			حرر(info);
			استمر;
		}
		p = &info->ايندڙ;
	}
}

ساكن فراغ unpack_non_delta_داخلا(تعداد object_type type, بنانشان ڊگهو سائيز,
				   بنانشان nr)
{
	فراغ *buf = get_معلومات(سائيز);

	إذا (buf)
		write_object(nr, type, buf, سائيز);
}

بنية zlib_وهڪرو {
	بنية odb_وهڪرو بنياد;
	git_zstream *zstream;
	صحيح حالت;
};

ساكن ssize_t zlib_وهڪرو_read(بنية odb_وهڪرو *اندريون_وهڪرو,
				حرف *buf, سائيز_t buf_len)
{
	بنية zlib_وهڪرو *معلومات = container_جو(اندريون_وهڪرو, بنية zlib_وهڪرو, بنياد);
	git_zstream *zstream = معلومات->zstream;

	إذا (معلومات->حالت != Z_OK)
		أرجع 0;

	zstream->ايندڙ_ٻاهريون = (بنانشان حرف *) buf;
	zstream->avail_ٻاهريون = buf_len;

	طالما (معلومات->حالت == Z_OK && zstream->avail_ٻاهريون == buf_len) {
		zstream->ايندڙ_اندريون = fill(1);
		zstream->avail_اندريون = len;
		معلومات->حالت = git_inflate(zstream, 0);
		use(len - zstream->avail_اندريون);
	}

	أرجع buf_len - zstream->avail_ٻاهريون;
}

ساكن فراغ وهڪرو_blob(بنانشان ڊگهو سائيز, بنانشان nr)
{
	git_zstream zstream = { 0 };
	بنية zlib_وهڪرو اندريون_وهڪرو = {
		.بنياد = {
			.اقرأ = zlib_وهڪرو_read,
			.سائيز = سائيز,
			.type = OBJ_BLOB,
		},
		.zstream = &zstream,
		.حالت = Z_OK,
	};
	بنية obj_info *info = &obj_list[nr];

	git_inflate_init(&zstream);

	إذا (odb_write_object_وهڪرو(the_repository->objects, &اندريون_وهڪرو.بنياد, &info->oid))
		die(_("failed to write object in stream"));

	إذا (اندريون_وهڪرو.حالت != Z_وهڪرو_پڇاڙي)
		die(_("inflate returned (%d)"), اندريون_وهڪرو.حالت);
	git_inflate_پڇاڙي(&zstream);

	إذا (strict) {
		بنية blob *blob = lookup_blob(the_repository, &info->oid);

		إذا (!blob)
			die(_("invalid blob object from stream"));
		blob->object.flags |= FLAG_WRITTEN;
	}
	info->obj = عدم;
}

ساكن صحيح resolve_against_held(بنانشان nr, ثابت بنية object_id *بنياد,
				فراغ *delta_معلومات, بنانشان ڊگهو delta_سائيز_2)
{
	بنية object *obj;
	بنية obj_بفر *obj_بفر;
	obj = lookup_object(the_repository, بنياد);
	إذا (!obj)
		أرجع 0;
	obj_بفر = lookup_object_بفر(obj);
	إذا (!obj_بفر)
		أرجع 0;
	resolve_delta(nr, obj->type, obj_بفر->بفر,
		      obj_بفر->سائيز, delta_معلومات, delta_سائيز_2);
	أرجع 1;
}

ساكن فراغ unpack_delta_داخلا(تعداد object_type type, بنانشان ڊگهو delta_سائيز_2,
			       بنانشان nr)
{
	فراغ *delta_معلومات, *بنياد;
	بنانشان ڊگهو بنياد_سائيز;
	سائيز_t بنياد_سائيز_st = 0;
	بنية object_id بنياد_oid;

	إذا (type == OBJ_REF_DELTA) {
		oidread(&بنياد_oid, fill(the_hash_algo->rawsz), the_repository->hash_algo);
		use(the_hash_algo->rawsz);
		delta_معلومات = get_معلومات(delta_سائيز_2);
		إذا (!delta_معلومات)
			أرجع;
		إذا (odb_has_object(the_repository->objects, &بنياد_oid,
				   ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR))
			; /* Ok we have this one */
		وإلا إذا (resolve_against_held(nr, &بنياد_oid,
					      delta_معلومات, delta_سائيز_2))
			أرجع; /* we are done */
		وإلا {
			/* cannot resolve yet --- queue it */
			oidclr(&obj_list[nr].oid, the_repository->hash_algo);
			جوڙيو_delta_منزل_list(nr, &بنياد_oid, 0, delta_معلومات, delta_سائيز_2);
			أرجع;
		}
	} وإلا {
		بنانشان بنياد_مليو = 0;
		بنانشان حرف *pack, c;
		off_t بنياد_وٿي;
		بنانشان lo, mid, hi;

		pack = fill(1);
		c = *pack;
		use(1);
		بنياد_وٿي = c & 127;
		طالما (c & 128) {
			بنياد_وٿي += 1;
			إذا (!بنياد_وٿي || MSB(بنياد_وٿي, 7))
				die("offset value overflow for delta base object");
			pack = fill(1);
			c = *pack;
			use(1);
			بنياد_وٿي = (بنياد_وٿي << 7) + (c & 127);
		}
		بنياد_وٿي = obj_list[nr].وٿي - بنياد_وٿي;
		إذا (بنياد_وٿي <= 0 || بنياد_وٿي >= obj_list[nr].وٿي)
			die("offset value out of bound for delta base object");

		delta_معلومات = get_معلومات(delta_سائيز_2);
		إذا (!delta_معلومات)
			أرجع;
		lo = 0;
		hi = nr;
		طالما (lo < hi) {
			mid = lo + (hi - lo) / 2;
			إذا (بنياد_وٿي < obj_list[mid].وٿي) {
				hi = mid;
			} وإلا إذا (بنياد_وٿي > obj_list[mid].وٿي) {
				lo = mid + 1;
			} وإلا {
				oidcpy(&بنياد_oid, &obj_list[mid].oid);
				بنياد_مليو = !is_null_oid(&بنياد_oid);
				اكسر;
			}
		}
		إذا (!بنياد_مليو) {
			/*
			 * The delta base object is itself a delta that
			 * has not been resolved yet.
			 */
			oidclr(&obj_list[nr].oid, the_repository->hash_algo);
			جوڙيو_delta_منزل_list(nr, null_oid(the_hash_algo), بنياد_وٿي,
					  delta_معلومات, delta_سائيز_2);
			أرجع;
		}
	}

	إذا (resolve_against_held(nr, &بنياد_oid, delta_معلومات, delta_سائيز_2))
		أرجع;

	بنياد = odb_read_object(the_repository->objects, &بنياد_oid,
			       &type, &بنياد_سائيز_st);
	بنياد_سائيز = cast_سائيز_t_منزل_ulong(بنياد_سائيز_st);
	إذا (!بنياد) {
		error("failed to read delta-pack base object %s",
		      oid_منزل_hex(&بنياد_oid));
		إذا (!recover)
			ٻاهر_نڪرو(1);
		has_errors = 1;
		أرجع;
	}
	resolve_delta(nr, type, بنياد, بنياد_سائيز, delta_معلومات, delta_سائيز_2);
	حرر(بنياد);
}

ساكن فراغ unpack_one(بنانشان nr)
{
	بنانشان سرڪايو;
	بنانشان حرف *pack;
	سائيز_t سائيز, c;
	تعداد object_type type;

	obj_list[nr].وٿي = consumed_بائيٽون;

	pack = fill(1);
	c = *pack;
	use(1);
	type = (c >> 4) & 7;
	سائيز = (c & 15);
	سرڪايو = 4;
	طالما (c & 0x80) {
		إذا ((bitsizeof(سائيز_t) - 7) < سرڪايو)
			die(_("object size too large for this platform"));
		pack = fill(1);
		c = *pack;
		use(1);
		سائيز += (c & 0x7f) << سرڪايو;
		سرڪايو += 7;
	}

	بدل (type) {
	حالة OBJ_BLOB:
		إذا (!dry_لاڳيتو &&
		    سائيز > repo_settings_get_big_file_حد(the_repository)) {
			وهڪرو_blob(سائيز, nr);
			أرجع;
		}
		/* fallthrough */
	حالة OBJ_COMMIT:
	حالة OBJ_وڻ:
	حالة OBJ_ٽيگ:
		unpack_non_delta_داخلا(type, سائيز, nr);
		أرجع;
	حالة OBJ_REF_DELTA:
	حالة OBJ_OFS_DELTA:
		unpack_delta_داخلا(type, سائيز, nr);
		أرجع;
	افتراضي:
		error("bad object type %d", type);
		has_errors = 1;
		إذا (recover)
			أرجع;
		ٻاهر_نڪرو(1);
	}
}

ساكن فراغ unpack_all(فراغ)
{
	صحيح i;
	بنانشان حرف *hdr = fill(حجم(بنية pack_سرو));
	بنية odb_transaction *transaction;

	إذا (get_be32(hdr) != PACK_SIGNATURE)
		die("bad pack file");
	hdr += 4;
	إذا (!pack_نسخو_ok_native(get_be32(hdr)))
		die("unknown pack file version %"PRIu32,
		    get_be32(hdr));
	hdr += 4;
	nr_objects = get_be32(hdr);
	use(حجم(بنية pack_سرو));

	إذا (!quiet)
		ترقي = شروعات_ترقي(the_repository,
					  _("Unpacking objects"), nr_objects);
	CALLOC_ARRAY(obj_list, nr_objects);
	odb_transaction_begin_or_die(the_repository->objects, &transaction, 0);
	لكل (i = 0; i < nr_objects; i++) {
		unpack_one(i);
		display_ترقي(ترقي, i + 1);
	}
	odb_transaction_commit_and_finalize_or_die(transaction);
	stop_ترقي(&ترقي);

	إذا (delta_list)
		die("unresolved deltas left after unpacking");
}

صحيح cmd_unpack_objects(صحيح argc,
		       ثابت حرف **argv,
		       ثابت حرف *اڳياڙي اڻ_استعمال_ٿيل,
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
				dry_لاڳيتو = 1;
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
			إذا (skip_اڳياڙي(arg, "--strict=", &arg)) {
				strict = 1;
				fsck_سيٽ_نياپو_types(&fsck_options, arg);
				استمر;
			}
			إذا (skip_اڳياڙي(arg, "--pack_header=", &arg)) {
				إذا (parse_pack_سرو_option(arg,
							     بفر, &len) < 0)
					die(_("bad --pack_header: %s"), arg);
				استمر;
			}
			إذا (skip_اڳياڙي(arg, "--max-input-size=", &arg)) {
				max_داخلا_سائيز = strtoumax(arg, عدم, 10);
				استمر;
			}
			usage(unpack_usage);
		}

		/* We don't take any non-flag arguments now.. Maybe some day */
		usage(unpack_usage);
	}
	git_hash_init(&ctx, the_hash_algo);
	unpack_all();
	git_hash_update(&ctx, بفر, وٿي);
	git_hash_init(&tmp_ctx, the_hash_algo);
	git_hash_clone(&tmp_ctx, &ctx);
	git_hash_final_oid(&oid, &tmp_ctx);
	إذا (strict) {
		write_rest();
		إذا (fsck_پڄاڻي(&fsck_options))
			die(_("fsck error in pack objects"));
	}
	إذا (!hasheq(fill(the_hash_algo->rawsz), oid.hash,
		    the_repository->hash_algo))
		die("final sha1 did not match");
	use(the_hash_algo->rawsz);

	/* Write the last part of the buffer to stdout */
	write_اندريون_ڀريل(1, بفر + وٿي, len);

	/* All done */
	أرجع has_errors;
}
