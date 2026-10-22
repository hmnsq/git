#تعریف_کړه USE_THE_REPOSITORY_VARIABLE
#تعریف_کړه DISABLE_SIGN_COMPARE_WARNINGS

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

ساكن صحيح dry_پرلپسې, quiet, recover, has_errors, strict;
ساكن ثابت حرف unpack_usage[] = "git unpack-objects [-n] [-q] [-r] [--strict]";

ساكن بینښه حرف بفر[DEFAULT_IO_بفر_اندازه];
ساكن بینښه صحيح دننه_واټن, len;
ساكن off_t consumed_بایټونه;
ساكن off_t max_ننوت_اندازه;
ساكن بنية git_hash_ctx ctx;
ساكن بنية fsck_options fsck_options;
ساكن بنية پرمختګ *پرمختګ;

/*
 * When running under --strict mode, objects whose reachability are
 * suspect are kept in core without getting written in the object
 * store.
 */
بنية obj_بفر {
	حرف *بفر;
	بینښه اوږد اندازه;
};

ساكن بنية decoration obj_decorate;

ساكن بنية obj_بفر *lookup_object_بفر(بنية object *بنسټ)
{
	أرجع lookup_decoration(&obj_decorate, بنسټ);
}

ساكن فراغ جمع_کړه_object_بفر(بنية object *object, حرف *بفر, بینښه اوږد اندازه)
{
	بنية obj_بفر *obj;
	CALLOC_ARRAY(obj, 1);
	obj->بفر = بفر;
	obj->اندازه = اندازه;
	إذا (جمع_کړه_decoration(&obj_decorate, object, obj))
		die("object %s tried to add buffer twice!", oid_منزل_hex(&object->oid));
}

/*
 * Make sure at least "min" bytes are available in the buffer, and
 * return the pointer to the buffer.
 */
ساكن فراغ *fill(صحيح min)
{
	إذا (min <= len)
		أرجع بفر + دننه_واټن;
	إذا (min > حجم(بفر))
		die("cannot fill %d bytes", min);
	إذا (دننه_واټن) {
		git_hash_update(&ctx, بفر, دننه_واټن);
		memmove(بفر, بفر + دننه_واټن, len);
		دننه_واټن = 0;
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

ساكن فراغ use(صحيح بایټونه)
{
	إذا (بایټونه > len)
		die("used more bytes than were available");
	len -= بایټونه;
	دننه_واټن += بایټونه;

	/* make sure off_t is sufficiently large not to wrap */
	إذا (signed_جمع_کړه_overflows(consumed_بایټونه, بایټونه))
		die("pack too large for current definition of off_t");
	consumed_بایټونه += بایټونه;
	إذا (max_ننوت_اندازه && consumed_بایټونه > max_ننوت_اندازه)
		die(_("pack exceeds maximum allowed size"));
	display_throughput(پرمختګ, consumed_بایټونه);
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
ساكن فراغ *get_معلومات(بینښه اوږد اندازه)
{
	git_zstream بهیر;
	بینښه اوږد bufsize = dry_پرلپسې && اندازه > 8192 ? 8192 : اندازه;
	فراغ *buf = xmallocz(bufsize);

	memset(&بهیر, 0, حجم(بهیر));

	بهیر.بل_بهر = buf;
	بهیر.avail_بهر = bufsize;
	بهیر.بل_دننه = fill(1);
	بهیر.avail_دننه = len;
	git_inflate_init(&بهیر);

	لكل (;;) {
		صحيح ret = git_inflate(&بهیر, 0);
		use(len - بهیر.avail_دننه);
		إذا (بهیر.ټول_بهر == اندازه && ret == Z_بهیر_پای_ټکی)
			اكسر;
		إذا (ret != Z_OK) {
			error("inflate returned %d", ret);
			FREE_AND_NULL(buf);
			إذا (!recover)
				وتل(1);
			has_errors = 1;
			اكسر;
		}
		بهیر.بل_دننه = fill(1);
		بهیر.avail_دننه = len;
		إذا (dry_پرلپسې) {
			/* reuse the buffer in dry_run mode */
			بهیر.بل_بهر = buf;
			بهیر.avail_بهر = bufsize > اندازه - بهیر.ټول_بهر ?
						   اندازه - بهیر.ټول_بهر :
						   bufsize;
		}
	}
	git_inflate_پای_ټکی(&بهیر);
	إذا (dry_پرلپسې)
		FREE_AND_NULL(buf);
	أرجع buf;
}

بنية delta_info {
	بنية object_id بنسټ_oid;
	بینښه nr;
	off_t بنسټ_دننه_واټن;
	بینښه اوږد اندازه;
	فراغ *delta;
	بنية delta_info *بل;
};

ساكن بنية delta_info *delta_list;

ساكن فراغ جمع_کړه_delta_منزل_list(بینښه nr, ثابت بنية object_id *بنسټ_oid,
			      off_t بنسټ_دننه_واټن,
			      فراغ *delta, بینښه اوږد اندازه)
{
	بنية delta_info *info = xmalloc(حجم(*info));

	oidcpy(&info->بنسټ_oid, بنسټ_oid);
	info->بنسټ_دننه_واټن = بنسټ_دننه_واټن;
	info->اندازه = اندازه;
	info->delta = delta;
	info->nr = nr;
	info->بل = delta_list;
	delta_list = info;
}

بنية obj_info {
	off_t دننه_واټن;
	بنية object_id oid;
	بنية object *obj;
};

/* Remember to update object flag allocation in object.h */
#تعریف_کړه FLAG_OPEN (1u<<20)
#تعریف_کړه FLAG_WRITTEN (1u<<21)

ساكن بنية obj_info *obj_list;
ساكن بینښه nr_objects;

/*
 * Called only from check_object() after it verified this object
 * is Ok.
 */
ساكن فراغ write_cached_object(بنية object *obj, بنية obj_بفر *obj_buf)
{
	بنية object_id oid;

	إذا (odb_write_object(the_repository->objects, obj_buf->بفر, obj_buf->اندازه,
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
			فراغ *معلومات ناکارول_شوی,
			بنية fsck_options *options ناکارول_شوی)
{
	بنية obj_بفر *obj_buf;

	إذا (!obj)
		أرجع 1;

	إذا (obj->flags & FLAG_WRITTEN)
		أرجع 0;

	إذا (type != OBJ_ANY && obj->type != type)
		die("object type mismatch");

	إذا (!(obj->flags & FLAG_OPEN)) {
		اندازه_t اندازه;
		صحيح type = odb_read_object_info(the_repository->objects, &obj->oid, &اندازه);
		إذا (type != obj->type || type <= 0)
			die("object of unexpected type");
		obj->flags |= FLAG_WRITTEN;
		أرجع 0;
	}

	obj_buf = lookup_object_بفر(obj);
	إذا (!obj_buf)
		die("Whoops! Cannot find object '%s'", oid_منزل_hex(&obj->oid));
	إذا (fsck_object(obj, obj_buf->بفر, obj_buf->اندازه, &fsck_options))
		die("fsck error in packed object");
	fsck_options.walk = check_object;
	إذا (fsck_walk(obj, عدم, &fsck_options))
		die("Error on reachable objects of %s", oid_منزل_hex(&obj->oid));
	write_cached_object(obj, obj_buf);
	أرجع 0;
}

ساكن فراغ write_rest(فراغ)
{
	بینښه i;
	لكل (i = 0; i < nr_objects; i++) {
		إذا (obj_list[i].obj)
			check_object(obj_list[i].obj, OBJ_ANY, عدم, عدم);
	}
}

ساكن فراغ added_object(بینښه nr, تعداد object_type type,
			 فراغ *معلومات, بینښه اوږد اندازه);

/*
 * Write out nr-th object from the list, now we know the contents
 * of it.  Under --strict, this buffers structured objects in-core,
 * to be checked at the end.
 */
ساكن فراغ write_object(بینښه nr, تعداد object_type type,
			 فراغ *buf, بینښه اوږد اندازه)
{
	إذا (!strict) {
		إذا (odb_write_object(the_repository->objects, buf, اندازه, type,
				     &obj_list[nr].oid) < 0)
			die("failed to write object");
		added_object(nr, type, buf, اندازه);
		حرر(buf);
		obj_list[nr].obj = عدم;
	} وإلا إذا (type == OBJ_BLOB) {
		بنية blob *blob;
		إذا (odb_write_object(the_repository->objects, buf, اندازه, type,
				     &obj_list[nr].oid) < 0)
			die("failed to write object");
		added_object(nr, type, buf, اندازه);
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
		hash_object_file(the_hash_algo, buf, اندازه, type,
				 &obj_list[nr].oid);
		added_object(nr, type, buf, اندازه);
		obj = parse_object_بفر(the_repository, &obj_list[nr].oid,
					  type, اندازه, buf,
					  &eaten);
		إذا (!obj)
			die("invalid %s", type_name(type));
		جمع_کړه_object_بفر(obj, buf, اندازه);
		obj->flags |= FLAG_OPEN;
		obj_list[nr].obj = obj;
	}
}

ساكن فراغ resolve_delta(بینښه nr, تعداد object_type type,
			  فراغ *بنسټ, بینښه اوږد بنسټ_اندازه,
			  فراغ *delta, بینښه اوږد delta_اندازه_2)
{
	فراغ *پایله;
	اندازه_t پایله_اندازه;

	پایله = patch_delta(بنسټ, بنسټ_اندازه,
			     delta, delta_اندازه_2,
			     &پایله_اندازه);
	إذا (!پایله)
		die("failed to apply delta");
	حرر(delta);
	write_object(nr, type, پایله, پایله_اندازه);
}

/*
 * We now know the contents of an object (which is nr-th in the pack);
 * resolve all the deltified objects that are based on it.
 */
ساكن فراغ added_object(بینښه nr, تعداد object_type type,
			 فراغ *معلومات, بینښه اوږد اندازه)
{
	بنية delta_info **p = &delta_list;
	بنية delta_info *info;

	طالما ((info = *p) != عدم) {
		إذا (oideq(&info->بنسټ_oid, &obj_list[nr].oid) ||
		    info->بنسټ_دننه_واټن == obj_list[nr].دننه_واټن) {
			*p = info->بل;
			p = &delta_list;
			resolve_delta(info->nr, type, معلومات, اندازه,
				      info->delta, info->اندازه);
			حرر(info);
			استمر;
		}
		p = &info->بل;
	}
}

ساكن فراغ unpack_non_delta_ثبت(تعداد object_type type, بینښه اوږد اندازه,
				   بینښه nr)
{
	فراغ *buf = get_معلومات(اندازه);

	إذا (buf)
		write_object(nr, type, buf, اندازه);
}

بنية zlib_بهیر {
	بنية odb_بهیر بنسټ;
	git_zstream *zstream;
	صحيح حالت;
};

ساكن ssize_t zlib_بهیر_read(بنية odb_بهیر *دننه_بهیر,
				حرف *buf, اندازه_t buf_len)
{
	بنية zlib_بهیر *معلومات = container_د(دننه_بهیر, بنية zlib_بهیر, بنسټ);
	git_zstream *zstream = معلومات->zstream;

	إذا (معلومات->حالت != Z_OK)
		أرجع 0;

	zstream->بل_بهر = (بینښه حرف *) buf;
	zstream->avail_بهر = buf_len;

	طالما (معلومات->حالت == Z_OK && zstream->avail_بهر == buf_len) {
		zstream->بل_دننه = fill(1);
		zstream->avail_دننه = len;
		معلومات->حالت = git_inflate(zstream, 0);
		use(len - zstream->avail_دننه);
	}

	أرجع buf_len - zstream->avail_بهر;
}

ساكن فراغ بهیر_blob(بینښه اوږد اندازه, بینښه nr)
{
	git_zstream zstream = { 0 };
	بنية zlib_بهیر دننه_بهیر = {
		.بنسټ = {
			.اقرأ = zlib_بهیر_read,
			.اندازه = اندازه,
			.type = OBJ_BLOB,
		},
		.zstream = &zstream,
		.حالت = Z_OK,
	};
	بنية obj_info *info = &obj_list[nr];

	git_inflate_init(&zstream);

	إذا (odb_write_object_بهیر(the_repository->objects, &دننه_بهیر.بنسټ, &info->oid))
		die(_("failed to write object in stream"));

	إذا (دننه_بهیر.حالت != Z_بهیر_پای_ټکی)
		die(_("inflate returned (%d)"), دننه_بهیر.حالت);
	git_inflate_پای_ټکی(&zstream);

	إذا (strict) {
		بنية blob *blob = lookup_blob(the_repository, &info->oid);

		إذا (!blob)
			die(_("invalid blob object from stream"));
		blob->object.flags |= FLAG_WRITTEN;
	}
	info->obj = عدم;
}

ساكن صحيح resolve_against_held(بینښه nr, ثابت بنية object_id *بنسټ,
				فراغ *delta_معلومات, بینښه اوږد delta_اندازه_2)
{
	بنية object *obj;
	بنية obj_بفر *obj_بفر;
	obj = lookup_object(the_repository, بنسټ);
	إذا (!obj)
		أرجع 0;
	obj_بفر = lookup_object_بفر(obj);
	إذا (!obj_بفر)
		أرجع 0;
	resolve_delta(nr, obj->type, obj_بفر->بفر,
		      obj_بفر->اندازه, delta_معلومات, delta_اندازه_2);
	أرجع 1;
}

ساكن فراغ unpack_delta_ثبت(تعداد object_type type, بینښه اوږد delta_اندازه_2,
			       بینښه nr)
{
	فراغ *delta_معلومات, *بنسټ;
	بینښه اوږد بنسټ_اندازه;
	اندازه_t بنسټ_اندازه_st = 0;
	بنية object_id بنسټ_oid;

	إذا (type == OBJ_REF_DELTA) {
		oidread(&بنسټ_oid, fill(the_hash_algo->rawsz), the_repository->hash_algo);
		use(the_hash_algo->rawsz);
		delta_معلومات = get_معلومات(delta_اندازه_2);
		إذا (!delta_معلومات)
			أرجع;
		إذا (odb_has_object(the_repository->objects, &بنسټ_oid,
				   ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR))
			; /* Ok we have this one */
		وإلا إذا (resolve_against_held(nr, &بنسټ_oid,
					      delta_معلومات, delta_اندازه_2))
			أرجع; /* we are done */
		وإلا {
			/* cannot resolve yet --- queue it */
			oidclr(&obj_list[nr].oid, the_repository->hash_algo);
			جمع_کړه_delta_منزل_list(nr, &بنسټ_oid, 0, delta_معلومات, delta_اندازه_2);
			أرجع;
		}
	} وإلا {
		بینښه بنسټ_وموندل = 0;
		بینښه حرف *pack, c;
		off_t بنسټ_دننه_واټن;
		بینښه lo, mid, hi;

		pack = fill(1);
		c = *pack;
		use(1);
		بنسټ_دننه_واټن = c & 127;
		طالما (c & 128) {
			بنسټ_دننه_واټن += 1;
			إذا (!بنسټ_دننه_واټن || MSB(بنسټ_دننه_واټن, 7))
				die("offset value overflow for delta base object");
			pack = fill(1);
			c = *pack;
			use(1);
			بنسټ_دننه_واټن = (بنسټ_دننه_واټن << 7) + (c & 127);
		}
		بنسټ_دننه_واټن = obj_list[nr].دننه_واټن - بنسټ_دننه_واټن;
		إذا (بنسټ_دننه_واټن <= 0 || بنسټ_دننه_واټن >= obj_list[nr].دننه_واټن)
			die("offset value out of bound for delta base object");

		delta_معلومات = get_معلومات(delta_اندازه_2);
		إذا (!delta_معلومات)
			أرجع;
		lo = 0;
		hi = nr;
		طالما (lo < hi) {
			mid = lo + (hi - lo) / 2;
			إذا (بنسټ_دننه_واټن < obj_list[mid].دننه_واټن) {
				hi = mid;
			} وإلا إذا (بنسټ_دننه_واټن > obj_list[mid].دننه_واټن) {
				lo = mid + 1;
			} وإلا {
				oidcpy(&بنسټ_oid, &obj_list[mid].oid);
				بنسټ_وموندل = !is_null_oid(&بنسټ_oid);
				اكسر;
			}
		}
		إذا (!بنسټ_وموندل) {
			/*
			 * The delta base object is itself a delta that
			 * has not been resolved yet.
			 */
			oidclr(&obj_list[nr].oid, the_repository->hash_algo);
			جمع_کړه_delta_منزل_list(nr, null_oid(the_hash_algo), بنسټ_دننه_واټن,
					  delta_معلومات, delta_اندازه_2);
			أرجع;
		}
	}

	إذا (resolve_against_held(nr, &بنسټ_oid, delta_معلومات, delta_اندازه_2))
		أرجع;

	بنسټ = odb_read_object(the_repository->objects, &بنسټ_oid,
			       &type, &بنسټ_اندازه_st);
	بنسټ_اندازه = cast_اندازه_t_منزل_ulong(بنسټ_اندازه_st);
	إذا (!بنسټ) {
		error("failed to read delta-pack base object %s",
		      oid_منزل_hex(&بنسټ_oid));
		إذا (!recover)
			وتل(1);
		has_errors = 1;
		أرجع;
	}
	resolve_delta(nr, type, بنسټ, بنسټ_اندازه, delta_معلومات, delta_اندازه_2);
	حرر(بنسټ);
}

ساكن فراغ unpack_one(بینښه nr)
{
	بینښه ښويدنه;
	بینښه حرف *pack;
	اندازه_t اندازه, c;
	تعداد object_type type;

	obj_list[nr].دننه_واټن = consumed_بایټونه;

	pack = fill(1);
	c = *pack;
	use(1);
	type = (c >> 4) & 7;
	اندازه = (c & 15);
	ښويدنه = 4;
	طالما (c & 0x80) {
		إذا ((bitsizeof(اندازه_t) - 7) < ښويدنه)
			die(_("object size too large for this platform"));
		pack = fill(1);
		c = *pack;
		use(1);
		اندازه += (c & 0x7f) << ښويدنه;
		ښويدنه += 7;
	}

	بدل (type) {
	حالة OBJ_BLOB:
		إذا (!dry_پرلپسې &&
		    اندازه > repo_settings_get_big_file_پوله(the_repository)) {
			بهیر_blob(اندازه, nr);
			أرجع;
		}
		/* fallthrough */
	حالة OBJ_COMMIT:
	حالة OBJ_ونه:
	حالة OBJ_نښان:
		unpack_non_delta_ثبت(type, اندازه, nr);
		أرجع;
	حالة OBJ_REF_DELTA:
	حالة OBJ_OFS_DELTA:
		unpack_delta_ثبت(type, اندازه, nr);
		أرجع;
	افتراضي:
		error("bad object type %d", type);
		has_errors = 1;
		إذا (recover)
			أرجع;
		وتل(1);
	}
}

ساكن فراغ unpack_all(فراغ)
{
	صحيح i;
	بینښه حرف *hdr = fill(حجم(بنية pack_سرلیک));
	بنية odb_transaction *transaction;

	إذا (get_be32(hdr) != PACK_SIGNATURE)
		die("bad pack file");
	hdr += 4;
	إذا (!pack_نسخه_ok_native(get_be32(hdr)))
		die("unknown pack file version %"PRIu32,
		    get_be32(hdr));
	hdr += 4;
	nr_objects = get_be32(hdr);
	use(حجم(بنية pack_سرلیک));

	إذا (!quiet)
		پرمختګ = پیل_پرمختګ(the_repository,
					  _("Unpacking objects"), nr_objects);
	CALLOC_ARRAY(obj_list, nr_objects);
	odb_transaction_begin_or_die(the_repository->objects, &transaction, 0);
	لكل (i = 0; i < nr_objects; i++) {
		unpack_one(i);
		display_پرمختګ(پرمختګ, i + 1);
	}
	odb_transaction_commit_and_finalize_or_die(transaction);
	stop_پرمختګ(&پرمختګ);

	إذا (delta_list)
		die("unresolved deltas left after unpacking");
}

صحيح cmd_unpack_objects(صحيح argc,
		       ثابت حرف **argv,
		       ثابت حرف *مختاړی ناکارول_شوی,
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
				dry_پرلپسې = 1;
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
			إذا (skip_مختاړی(arg, "--strict=", &arg)) {
				strict = 1;
				fsck_سېټ_پیغام_types(&fsck_options, arg);
				استمر;
			}
			إذا (skip_مختاړی(arg, "--pack_header=", &arg)) {
				إذا (parse_pack_سرلیک_option(arg,
							     بفر, &len) < 0)
					die(_("bad --pack_header: %s"), arg);
				استمر;
			}
			إذا (skip_مختاړی(arg, "--max-input-size=", &arg)) {
				max_ننوت_اندازه = strtoumax(arg, عدم, 10);
				استمر;
			}
			usage(unpack_usage);
		}

		/* We don't take any non-flag arguments now.. Maybe some day */
		usage(unpack_usage);
	}
	git_hash_init(&ctx, the_hash_algo);
	unpack_all();
	git_hash_update(&ctx, بفر, دننه_واټن);
	git_hash_init(&tmp_ctx, the_hash_algo);
	git_hash_clone(&tmp_ctx, &ctx);
	git_hash_final_oid(&oid, &tmp_ctx);
	إذا (strict) {
		write_rest();
		إذا (fsck_پای(&fsck_options))
			die(_("fsck error in pack objects"));
	}
	إذا (!hasheq(fill(the_hash_algo->rawsz), oid.hash,
		    the_repository->hash_algo))
		die("final sha1 did not match");
	use(the_hash_algo->rawsz);

	/* Write the last part of the buffer to stdout */
	write_دننه_ډک(1, بفر + دننه_واټن, len);

	/* All done */
	أرجع has_errors;
}
