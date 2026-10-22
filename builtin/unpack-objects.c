#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
#تعریف_کرو DISABLE_SIGN_COMPARE_WARNINGS

#شامل "builtin.h"
#شامل "config.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "git-zlib.h"
#شامل "hex.h"
#شامل "object-file.h"
#شامل "odb.h"
#شامل "odb/streaming.h"
#شامل "odb/transaction.h"
#شامل "object.h"
#شامل "delta.h"
#شامل "pack.h"
#شامل "blob.h"
#شامل "replace-object.h"
#شامل "strbuf.h"
#شامل "progress.h"
#شامل "decorate.h"
#شامل "fsck.h"
#شامل "packfile.h"

ساکن صحیح dry_تسلسل, quiet, recover, has_errors, strict;
ساکن ثابت حرف unpack_usage[] = "git unpack-objects [-n] [-q] [-r] [--strict]";

ساکن غیرعلامتی حرف بفر[DEFAULT_IO_بفر_سائز];
ساکن غیرعلامتی صحیح انحراف, len;
ساکن off_t consumed_بائٹیں;
ساکن off_t max_داخلہ_سائز;
ساکن ساخت git_hash_ctx ctx;
ساکن ساخت fsck_options fsck_options;
ساکن ساخت پیش_رفت *پیش_رفت;

/*
 * When running under --strict mode, objects whose reachability are
 * suspect are kept in core without getting written in the object
 * store.
 */
ساخت obj_بفر {
	حرف *بفر;
	غیرعلامتی طویل سائز;
};

ساکن ساخت decoration obj_decorate;

ساکن ساخت obj_بفر *lookup_object_بفر(ساخت object *بنیاد)
{
	واپس lookup_decoration(&obj_decorate, بنیاد);
}

ساکن خالی جوڑو_object_بفر(ساخت object *object, حرف *بفر, غیرعلامتی طویل سائز)
{
	ساخت obj_بفر *obj;
	CALLOC_ARRAY(obj, 1);
	obj->بفر = بفر;
	obj->سائز = سائز;
	اگر (جوڑو_decoration(&obj_decorate, object, obj))
		die("object %s tried to add buffer twice!", oid_مقصد_hex(&object->oid));
}

/*
 * Make sure at least "min" bytes are available in the buffer, and
 * return the pointer to the buffer.
 */
ساکن خالی *fill(صحیح min)
{
	اگر (min <= len)
		واپس بفر + انحراف;
	اگر (min > حجم(بفر))
		die("cannot fill %d bytes", min);
	اگر (انحراف) {
		git_hash_update(&ctx, بفر, انحراف);
		memmove(بفر, بفر + انحراف, len);
		انحراف = 0;
	}
	کرو {
		ssize_t ret = xread(0, بفر + len, حجم(بفر) - len);
		اگر (ret <= 0) {
			اگر (!ret)
				die("early EOF");
			die_errno("read error on input");
		}
		len += ret;
	} جب (len < min);
	واپس بفر;
}

ساکن خالی use(صحیح بائٹیں)
{
	اگر (بائٹیں > len)
		die("used more bytes than were available");
	len -= بائٹیں;
	انحراف += بائٹیں;

	/* make sure off_t is sufficiently large not to wrap */
	اگر (signed_جوڑو_overflows(consumed_بائٹیں, بائٹیں))
		die("pack too large for current definition of off_t");
	consumed_بائٹیں += بائٹیں;
	اگر (max_داخلہ_سائز && consumed_بائٹیں > max_داخلہ_سائز)
		die(_("pack exceeds maximum allowed size"));
	display_throughput(پیش_رفت, consumed_بائٹیں);
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
ساکن خالی *get_ڈیٹا(غیرعلامتی طویل سائز)
{
	git_zstream دھارا;
	غیرعلامتی طویل bufsize = dry_تسلسل && سائز > 8192 ? 8192 : سائز;
	خالی *buf = xmallocz(bufsize);

	memset(&دھارا, 0, حجم(دھارا));

	دھارا.اگلا_برآمد = buf;
	دھارا.avail_برآمد = bufsize;
	دھارا.اگلا_اندر = fill(1);
	دھارا.avail_اندر = len;
	git_inflate_init(&دھارا);

	برائے (;;) {
		صحیح ret = git_inflate(&دھارا, 0);
		use(len - دھارا.avail_اندر);
		اگر (دھارا.کل_برآمد == سائز && ret == Z_دھارا_انتہا)
			توڑو;
		اگر (ret != Z_OK) {
			error("inflate returned %d", ret);
			FREE_AND_NULL(buf);
			اگر (!recover)
				باہر_نکلو(1);
			has_errors = 1;
			توڑو;
		}
		دھارا.اگلا_اندر = fill(1);
		دھارا.avail_اندر = len;
		اگر (dry_تسلسل) {
			/* reuse the buffer in dry_run mode */
			دھارا.اگلا_برآمد = buf;
			دھارا.avail_برآمد = bufsize > سائز - دھارا.کل_برآمد ?
						   سائز - دھارا.کل_برآمد :
						   bufsize;
		}
	}
	git_inflate_انتہا(&دھارا);
	اگر (dry_تسلسل)
		FREE_AND_NULL(buf);
	واپس buf;
}

ساخت delta_info {
	ساخت object_id بنیاد_oid;
	غیرعلامتی nr;
	off_t بنیاد_انحراف;
	غیرعلامتی طویل سائز;
	خالی *delta;
	ساخت delta_info *اگلا;
};

ساکن ساخت delta_info *delta_list;

ساکن خالی جوڑو_delta_مقصد_list(غیرعلامتی nr, ثابت ساخت object_id *بنیاد_oid,
			      off_t بنیاد_انحراف,
			      خالی *delta, غیرعلامتی طویل سائز)
{
	ساخت delta_info *info = xmalloc(حجم(*info));

	oidcpy(&info->بنیاد_oid, بنیاد_oid);
	info->بنیاد_انحراف = بنیاد_انحراف;
	info->سائز = سائز;
	info->delta = delta;
	info->nr = nr;
	info->اگلا = delta_list;
	delta_list = info;
}

ساخت obj_info {
	off_t انحراف;
	ساخت object_id oid;
	ساخت object *obj;
};

/* Remember to update object flag allocation in object.h */
#تعریف_کرو FLAG_OPEN (1u<<20)
#تعریف_کرو FLAG_WRITTEN (1u<<21)

ساکن ساخت obj_info *obj_list;
ساکن غیرعلامتی nr_objects;

/*
 * Called only from check_object() after it verified this object
 * is Ok.
 */
ساکن خالی write_cached_object(ساخت object *obj, ساخت obj_بفر *obj_buf)
{
	ساخت object_id oid;

	اگر (odb_write_object(the_repository->objects, obj_buf->بفر, obj_buf->سائز,
			     obj->type, &oid) < 0)
		die("failed to write object %s", oid_مقصد_hex(&obj->oid));
	obj->flags |= FLAG_WRITTEN;
}

/*
 * At the very end of the processing, write_rest() scans the objects
 * that have reachability requirements and calls this function.
 * Verify its reachability and validity recursively and write it out.
 */
ساکن صحیح check_object(ساخت object *obj, شمار object_type type,
			خالی *ڈیٹا غیر_مستعمل,
			ساخت fsck_options *options غیر_مستعمل)
{
	ساخت obj_بفر *obj_buf;

	اگر (!obj)
		واپس 1;

	اگر (obj->flags & FLAG_WRITTEN)
		واپس 0;

	اگر (type != OBJ_ANY && obj->type != type)
		die("object type mismatch");

	اگر (!(obj->flags & FLAG_OPEN)) {
		سائز_t سائز;
		صحیح type = odb_read_object_info(the_repository->objects, &obj->oid, &سائز);
		اگر (type != obj->type || type <= 0)
			die("object of unexpected type");
		obj->flags |= FLAG_WRITTEN;
		واپس 0;
	}

	obj_buf = lookup_object_بفر(obj);
	اگر (!obj_buf)
		die("Whoops! Cannot find object '%s'", oid_مقصد_hex(&obj->oid));
	اگر (fsck_object(obj, obj_buf->بفر, obj_buf->سائز, &fsck_options))
		die("fsck error in packed object");
	fsck_options.walk = check_object;
	اگر (fsck_walk(obj, تہی, &fsck_options))
		die("Error on reachable objects of %s", oid_مقصد_hex(&obj->oid));
	write_cached_object(obj, obj_buf);
	واپس 0;
}

ساکن خالی write_rest(خالی)
{
	غیرعلامتی i;
	برائے (i = 0; i < nr_objects; i++) {
		اگر (obj_list[i].obj)
			check_object(obj_list[i].obj, OBJ_ANY, تہی, تہی);
	}
}

ساکن خالی added_object(غیرعلامتی nr, شمار object_type type,
			 خالی *ڈیٹا, غیرعلامتی طویل سائز);

/*
 * Write out nr-th object from the list, now we know the contents
 * of it.  Under --strict, this buffers structured objects in-core,
 * to be checked at the end.
 */
ساکن خالی write_object(غیرعلامتی nr, شمار object_type type,
			 خالی *buf, غیرعلامتی طویل سائز)
{
	اگر (!strict) {
		اگر (odb_write_object(the_repository->objects, buf, سائز, type,
				     &obj_list[nr].oid) < 0)
			die("failed to write object");
		added_object(nr, type, buf, سائز);
		آزاد(buf);
		obj_list[nr].obj = تہی;
	} ورنہ اگر (type == OBJ_BLOB) {
		ساخت blob *blob;
		اگر (odb_write_object(the_repository->objects, buf, سائز, type,
				     &obj_list[nr].oid) < 0)
			die("failed to write object");
		added_object(nr, type, buf, سائز);
		آزاد(buf);

		blob = lookup_blob(the_repository, &obj_list[nr].oid);
		اگر (blob)
			blob->object.flags |= FLAG_WRITTEN;
		ورنہ
			die("invalid blob object");
		obj_list[nr].obj = تہی;
	} ورنہ {
		ساخت object *obj;
		صحیح eaten;
		hash_object_file(the_hash_algo, buf, سائز, type,
				 &obj_list[nr].oid);
		added_object(nr, type, buf, سائز);
		obj = parse_object_بفر(the_repository, &obj_list[nr].oid,
					  type, سائز, buf,
					  &eaten);
		اگر (!obj)
			die("invalid %s", type_name(type));
		جوڑو_object_بفر(obj, buf, سائز);
		obj->flags |= FLAG_OPEN;
		obj_list[nr].obj = obj;
	}
}

ساکن خالی resolve_delta(غیرعلامتی nr, شمار object_type type,
			  خالی *بنیاد, غیرعلامتی طویل بنیاد_سائز,
			  خالی *delta, غیرعلامتی طویل delta_سائز_2)
{
	خالی *نتیجہ;
	سائز_t نتیجہ_سائز;

	نتیجہ = patch_delta(بنیاد, بنیاد_سائز,
			     delta, delta_سائز_2,
			     &نتیجہ_سائز);
	اگر (!نتیجہ)
		die("failed to apply delta");
	آزاد(delta);
	write_object(nr, type, نتیجہ, نتیجہ_سائز);
}

/*
 * We now know the contents of an object (which is nr-th in the pack);
 * resolve all the deltified objects that are based on it.
 */
ساکن خالی added_object(غیرعلامتی nr, شمار object_type type,
			 خالی *ڈیٹا, غیرعلامتی طویل سائز)
{
	ساخت delta_info **p = &delta_list;
	ساخت delta_info *info;

	جب ((info = *p) != تہی) {
		اگر (oideq(&info->بنیاد_oid, &obj_list[nr].oid) ||
		    info->بنیاد_انحراف == obj_list[nr].انحراف) {
			*p = info->اگلا;
			p = &delta_list;
			resolve_delta(info->nr, type, ڈیٹا, سائز,
				      info->delta, info->سائز);
			آزاد(info);
			جاری;
		}
		p = &info->اگلا;
	}
}

ساکن خالی unpack_non_delta_اندراج(شمار object_type type, غیرعلامتی طویل سائز,
				   غیرعلامتی nr)
{
	خالی *buf = get_ڈیٹا(سائز);

	اگر (buf)
		write_object(nr, type, buf, سائز);
}

ساخت zlib_دھارا {
	ساخت odb_دھارا بنیاد;
	git_zstream *zstream;
	صحیح حالت_2;
};

ساکن ssize_t zlib_دھارا_read(ساخت odb_دھارا *اندر_دھارا,
				حرف *buf, سائز_t buf_len)
{
	ساخت zlib_دھارا *ڈیٹا = container_کا(اندر_دھارا, ساخت zlib_دھارا, بنیاد);
	git_zstream *zstream = ڈیٹا->zstream;

	اگر (ڈیٹا->حالت_2 != Z_OK)
		واپس 0;

	zstream->اگلا_برآمد = (غیرعلامتی حرف *) buf;
	zstream->avail_برآمد = buf_len;

	جب (ڈیٹا->حالت_2 == Z_OK && zstream->avail_برآمد == buf_len) {
		zstream->اگلا_اندر = fill(1);
		zstream->avail_اندر = len;
		ڈیٹا->حالت_2 = git_inflate(zstream, 0);
		use(len - zstream->avail_اندر);
	}

	واپس buf_len - zstream->avail_برآمد;
}

ساکن خالی دھارا_blob(غیرعلامتی طویل سائز, غیرعلامتی nr)
{
	git_zstream zstream = { 0 };
	ساخت zlib_دھارا اندر_دھارا = {
		.بنیاد = {
			.اقرأ = zlib_دھارا_read,
			.سائز = سائز,
			.type = OBJ_BLOB,
		},
		.zstream = &zstream,
		.حالت_2 = Z_OK,
	};
	ساخت obj_info *info = &obj_list[nr];

	git_inflate_init(&zstream);

	اگر (odb_write_object_دھارا(the_repository->objects, &اندر_دھارا.بنیاد, &info->oid))
		die(_("failed to write object in stream"));

	اگر (اندر_دھارا.حالت_2 != Z_دھارا_انتہا)
		die(_("inflate returned (%d)"), اندر_دھارا.حالت_2);
	git_inflate_انتہا(&zstream);

	اگر (strict) {
		ساخت blob *blob = lookup_blob(the_repository, &info->oid);

		اگر (!blob)
			die(_("invalid blob object from stream"));
		blob->object.flags |= FLAG_WRITTEN;
	}
	info->obj = تہی;
}

ساکن صحیح resolve_against_held(غیرعلامتی nr, ثابت ساخت object_id *بنیاد,
				خالی *delta_ڈیٹا, غیرعلامتی طویل delta_سائز_2)
{
	ساخت object *obj;
	ساخت obj_بفر *obj_بفر;
	obj = lookup_object(the_repository, بنیاد);
	اگر (!obj)
		واپس 0;
	obj_بفر = lookup_object_بفر(obj);
	اگر (!obj_بفر)
		واپس 0;
	resolve_delta(nr, obj->type, obj_بفر->بفر,
		      obj_بفر->سائز, delta_ڈیٹا, delta_سائز_2);
	واپس 1;
}

ساکن خالی unpack_delta_اندراج(شمار object_type type, غیرعلامتی طویل delta_سائز_2,
			       غیرعلامتی nr)
{
	خالی *delta_ڈیٹا, *بنیاد;
	غیرعلامتی طویل بنیاد_سائز;
	سائز_t بنیاد_سائز_st = 0;
	ساخت object_id بنیاد_oid;

	اگر (type == OBJ_REF_DELTA) {
		oidread(&بنیاد_oid, fill(the_hash_algo->rawsz), the_repository->hash_algo);
		use(the_hash_algo->rawsz);
		delta_ڈیٹا = get_ڈیٹا(delta_سائز_2);
		اگر (!delta_ڈیٹا)
			واپس;
		اگر (odb_has_object(the_repository->objects, &بنیاد_oid,
				   ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR))
			; /* Ok we have this one */
		ورنہ اگر (resolve_against_held(nr, &بنیاد_oid,
					      delta_ڈیٹا, delta_سائز_2))
			واپس; /* we are done */
		ورنہ {
			/* cannot resolve yet --- queue it */
			oidclr(&obj_list[nr].oid, the_repository->hash_algo);
			جوڑو_delta_مقصد_list(nr, &بنیاد_oid, 0, delta_ڈیٹا, delta_سائز_2);
			واپس;
		}
	} ورنہ {
		غیرعلامتی بنیاد_ملا = 0;
		غیرعلامتی حرف *pack, c;
		off_t بنیاد_انحراف;
		غیرعلامتی lo, mid, hi;

		pack = fill(1);
		c = *pack;
		use(1);
		بنیاد_انحراف = c & 127;
		جب (c & 128) {
			بنیاد_انحراف += 1;
			اگر (!بنیاد_انحراف || MSB(بنیاد_انحراف, 7))
				die("offset value overflow for delta base object");
			pack = fill(1);
			c = *pack;
			use(1);
			بنیاد_انحراف = (بنیاد_انحراف << 7) + (c & 127);
		}
		بنیاد_انحراف = obj_list[nr].انحراف - بنیاد_انحراف;
		اگر (بنیاد_انحراف <= 0 || بنیاد_انحراف >= obj_list[nr].انحراف)
			die("offset value out of bound for delta base object");

		delta_ڈیٹا = get_ڈیٹا(delta_سائز_2);
		اگر (!delta_ڈیٹا)
			واپس;
		lo = 0;
		hi = nr;
		جب (lo < hi) {
			mid = lo + (hi - lo) / 2;
			اگر (بنیاد_انحراف < obj_list[mid].انحراف) {
				hi = mid;
			} ورنہ اگر (بنیاد_انحراف > obj_list[mid].انحراف) {
				lo = mid + 1;
			} ورنہ {
				oidcpy(&بنیاد_oid, &obj_list[mid].oid);
				بنیاد_ملا = !is_null_oid(&بنیاد_oid);
				توڑو;
			}
		}
		اگر (!بنیاد_ملا) {
			/*
			 * The delta base object is itself a delta that
			 * has not been resolved yet.
			 */
			oidclr(&obj_list[nr].oid, the_repository->hash_algo);
			جوڑو_delta_مقصد_list(nr, null_oid(the_hash_algo), بنیاد_انحراف,
					  delta_ڈیٹا, delta_سائز_2);
			واپس;
		}
	}

	اگر (resolve_against_held(nr, &بنیاد_oid, delta_ڈیٹا, delta_سائز_2))
		واپس;

	بنیاد = odb_read_object(the_repository->objects, &بنیاد_oid,
			       &type, &بنیاد_سائز_st);
	بنیاد_سائز = cast_سائز_t_مقصد_ulong(بنیاد_سائز_st);
	اگر (!بنیاد) {
		error("failed to read delta-pack base object %s",
		      oid_مقصد_hex(&بنیاد_oid));
		اگر (!recover)
			باہر_نکلو(1);
		has_errors = 1;
		واپس;
	}
	resolve_delta(nr, type, بنیاد, بنیاد_سائز, delta_ڈیٹا, delta_سائز_2);
	آزاد(بنیاد);
}

ساکن خالی unpack_one(غیرعلامتی nr)
{
	غیرعلامتی سرکاؤ;
	غیرعلامتی حرف *pack;
	سائز_t سائز, c;
	شمار object_type type;

	obj_list[nr].انحراف = consumed_بائٹیں;

	pack = fill(1);
	c = *pack;
	use(1);
	type = (c >> 4) & 7;
	سائز = (c & 15);
	سرکاؤ = 4;
	جب (c & 0x80) {
		اگر ((bitsizeof(سائز_t) - 7) < سرکاؤ)
			die(_("object size too large for this platform"));
		pack = fill(1);
		c = *pack;
		use(1);
		سائز += (c & 0x7f) << سرکاؤ;
		سرکاؤ += 7;
	}

	انتخاب (type) {
	حالت OBJ_BLOB:
		اگر (!dry_تسلسل &&
		    سائز > repo_settings_get_big_file_حد(the_repository)) {
			دھارا_blob(سائز, nr);
			واپس;
		}
		/* fallthrough */
	حالت OBJ_COMMIT:
	حالت OBJ_درخت:
	حالت OBJ_لیبل:
		unpack_non_delta_اندراج(type, سائز, nr);
		واپس;
	حالت OBJ_REF_DELTA:
	حالت OBJ_OFS_DELTA:
		unpack_delta_اندراج(type, سائز, nr);
		واپس;
	طےشدہ:
		error("bad object type %d", type);
		has_errors = 1;
		اگر (recover)
			واپس;
		باہر_نکلو(1);
	}
}

ساکن خالی unpack_all(خالی)
{
	صحیح i;
	غیرعلامتی حرف *hdr = fill(حجم(ساخت pack_سرخی));
	ساخت odb_transaction *transaction;

	اگر (get_be32(hdr) != PACK_SIGNATURE)
		die("bad pack file");
	hdr += 4;
	اگر (!pack_نسخہ_ok_native(get_be32(hdr)))
		die("unknown pack file version %"PRIu32,
		    get_be32(hdr));
	hdr += 4;
	nr_objects = get_be32(hdr);
	use(حجم(ساخت pack_سرخی));

	اگر (!quiet)
		پیش_رفت = آغاز_پیش_رفت(the_repository,
					  _("Unpacking objects"), nr_objects);
	CALLOC_ARRAY(obj_list, nr_objects);
	odb_transaction_begin_or_die(the_repository->objects, &transaction, 0);
	برائے (i = 0; i < nr_objects; i++) {
		unpack_one(i);
		display_پیش_رفت(پیش_رفت, i + 1);
	}
	odb_transaction_commit_and_finalize_or_die(transaction);
	stop_پیش_رفت(&پیش_رفت);

	اگر (delta_list)
		die("unresolved deltas left after unpacking");
}

صحیح cmd_unpack_objects(صحیح argc,
		       ثابت حرف **argv,
		       ثابت حرف *سابقہ غیر_مستعمل,
		       ساخت repository *repo)
{
	صحیح i;
	ساخت object_id oid;
	ساخت git_hash_ctx tmp_ctx;

	disable_replace_refs();

	repo_config(the_repository, git_default_config, تہی);

	quiet = !isatty(2);

	show_usage_if_asked(argc, argv, unpack_usage);

	fsck_options_init(&fsck_options, repo, FSCK_OPTIONS_STRICT);

	برائے (i = 1 ; i < argc; i++) {
		ثابت حرف *arg = argv[i];

		اگر (*arg == '-') {
			اگر (!strcmp(arg, "-n")) {
				dry_تسلسل = 1;
				جاری;
			}
			اگر (!strcmp(arg, "-q")) {
				quiet = 1;
				جاری;
			}
			اگر (!strcmp(arg, "-r")) {
				recover = 1;
				جاری;
			}
			اگر (!strcmp(arg, "--strict")) {
				strict = 1;
				جاری;
			}
			اگر (skip_سابقہ(arg, "--strict=", &arg)) {
				strict = 1;
				fsck_سیٹ_پیغام_types(&fsck_options, arg);
				جاری;
			}
			اگر (skip_سابقہ(arg, "--pack_header=", &arg)) {
				اگر (parse_pack_سرخی_option(arg,
							     بفر, &len) < 0)
					die(_("bad --pack_header: %s"), arg);
				جاری;
			}
			اگر (skip_سابقہ(arg, "--max-input-size=", &arg)) {
				max_داخلہ_سائز = strtoumax(arg, تہی, 10);
				جاری;
			}
			usage(unpack_usage);
		}

		/* We don't take any non-flag arguments now.. Maybe some day */
		usage(unpack_usage);
	}
	git_hash_init(&ctx, the_hash_algo);
	unpack_all();
	git_hash_update(&ctx, بفر, انحراف);
	git_hash_init(&tmp_ctx, the_hash_algo);
	git_hash_clone(&tmp_ctx, &ctx);
	git_hash_final_oid(&oid, &tmp_ctx);
	اگر (strict) {
		write_rest();
		اگر (fsck_اختتام(&fsck_options))
			die(_("fsck error in pack objects"));
	}
	اگر (!hasheq(fill(the_hash_algo->rawsz), oid.hash,
		    the_repository->hash_algo))
		die("final sha1 did not match");
	use(the_hash_algo->rawsz);

	/* Write the last part of the buffer to stdout */
	write_اندر_بھرا(1, بفر + انحراف, len);

	/* All done */
	واپس has_errors;
}
