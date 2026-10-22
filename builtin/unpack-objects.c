#تعریف_کن USE_THE_REPOSITORY_VARIABLE
#تعریف_کن DISABLE_SIGN_COMPARE_WARNINGS

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

ساكن صحيح dry_پیاپی, quiet, recover, has_errors, strict;
ساكن ثابت حرف unpack_usage[] = "git unpack-objects [-n] [-q] [-r] [--strict]";

ساكن بیعلامت حرف میانگیر[DEFAULT_IO_میانگیر_اندازه];
ساكن بیعلامت صحيح جابه_جایی, len;
ساكن off_t consumed_بایتها;
ساكن off_t max_درون_داد_اندازه;
ساكن بنية git_hash_ctx ctx;
ساكن بنية fsck_options fsck_options;
ساكن بنية پیشرفت *پیشرفت;

/*
 * When running under --strict mode, objects whose reachability are
 * suspect are kept in core without getting written in the object
 * store.
 */
بنية obj_میانگیر {
	حرف *میانگیر;
	بیعلامت دراز اندازه;
};

ساكن بنية decoration obj_decorate;

ساكن بنية obj_میانگیر *lookup_object_میانگیر(بنية object *پایه)
{
	أرجع lookup_decoration(&obj_decorate, پایه);
}

ساكن فراغ جمع_کن_object_میانگیر(بنية object *object, حرف *میانگیر, بیعلامت دراز اندازه)
{
	بنية obj_میانگیر *obj;
	CALLOC_ARRAY(obj, 1);
	obj->میانگیر = میانگیر;
	obj->اندازه = اندازه;
	إذا (جمع_کن_decoration(&obj_decorate, object, obj))
		die("object %s tried to add buffer twice!", oid_مقصد_hex(&object->oid));
}

/*
 * Make sure at least "min" bytes are available in the buffer, and
 * return the pointer to the buffer.
 */
ساكن فراغ *fill(صحيح min)
{
	إذا (min <= len)
		أرجع میانگیر + جابه_جایی;
	إذا (min > حجم(میانگیر))
		die("cannot fill %d bytes", min);
	إذا (جابه_جایی) {
		git_hash_update(&ctx, میانگیر, جابه_جایی);
		memmove(میانگیر, میانگیر + جابه_جایی, len);
		جابه_جایی = 0;
	}
	افعل {
		ssize_t ret = xread(0, میانگیر + len, حجم(میانگیر) - len);
		إذا (ret <= 0) {
			إذا (!ret)
				die("early EOF");
			die_errno("read error on input");
		}
		len += ret;
	} طالما (len < min);
	أرجع میانگیر;
}

ساكن فراغ use(صحيح بایتها)
{
	إذا (بایتها > len)
		die("used more bytes than were available");
	len -= بایتها;
	جابه_جایی += بایتها;

	/* make sure off_t is sufficiently large not to wrap */
	إذا (signed_جمع_کن_overflows(consumed_بایتها, بایتها))
		die("pack too large for current definition of off_t");
	consumed_بایتها += بایتها;
	إذا (max_درون_داد_اندازه && consumed_بایتها > max_درون_داد_اندازه)
		die(_("pack exceeds maximum allowed size"));
	display_throughput(پیشرفت, consumed_بایتها);
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
ساكن فراغ *get_داده(بیعلامت دراز اندازه)
{
	git_zstream جریان;
	بیعلامت دراز bufsize = dry_پیاپی && اندازه > 8192 ? 8192 : اندازه;
	فراغ *buf = xmallocz(bufsize);

	memset(&جریان, 0, حجم(جریان));

	جریان.بعدی_بیرون = buf;
	جریان.avail_بیرون = bufsize;
	جریان.بعدی_درون = fill(1);
	جریان.avail_درون = len;
	git_inflate_init(&جریان);

	لكل (;;) {
		صحيح ret = git_inflate(&جریان, 0);
		use(len - جریان.avail_درون);
		إذا (جریان.مجموع_بیرون == اندازه && ret == Z_جریان_انتها)
			اكسر;
		إذا (ret != Z_OK) {
			error("inflate returned %d", ret);
			FREE_AND_NULL(buf);
			إذا (!recover)
				خروج(1);
			has_errors = 1;
			اكسر;
		}
		جریان.بعدی_درون = fill(1);
		جریان.avail_درون = len;
		إذا (dry_پیاپی) {
			/* reuse the buffer in dry_run mode */
			جریان.بعدی_بیرون = buf;
			جریان.avail_بیرون = bufsize > اندازه - جریان.مجموع_بیرون ?
						   اندازه - جریان.مجموع_بیرون :
						   bufsize;
		}
	}
	git_inflate_انتها(&جریان);
	إذا (dry_پیاپی)
		FREE_AND_NULL(buf);
	أرجع buf;
}

بنية delta_info {
	بنية object_id پایه_oid;
	بیعلامت nr;
	off_t پایه_جابه_جایی;
	بیعلامت دراز اندازه;
	فراغ *delta;
	بنية delta_info *بعدی;
};

ساكن بنية delta_info *delta_list;

ساكن فراغ جمع_کن_delta_مقصد_list(بیعلامت nr, ثابت بنية object_id *پایه_oid,
			      off_t پایه_جابه_جایی,
			      فراغ *delta, بیعلامت دراز اندازه)
{
	بنية delta_info *info = xmalloc(حجم(*info));

	oidcpy(&info->پایه_oid, پایه_oid);
	info->پایه_جابه_جایی = پایه_جابه_جایی;
	info->اندازه = اندازه;
	info->delta = delta;
	info->nr = nr;
	info->بعدی = delta_list;
	delta_list = info;
}

بنية obj_info {
	off_t جابه_جایی;
	بنية object_id oid;
	بنية object *obj;
};

/* Remember to update object flag allocation in object.h */
#تعریف_کن FLAG_OPEN (1u<<20)
#تعریف_کن FLAG_WRITTEN (1u<<21)

ساكن بنية obj_info *obj_list;
ساكن بیعلامت nr_objects;

/*
 * Called only from check_object() after it verified this object
 * is Ok.
 */
ساكن فراغ write_cached_object(بنية object *obj, بنية obj_میانگیر *obj_buf)
{
	بنية object_id oid;

	إذا (odb_write_object(the_repository->objects, obj_buf->میانگیر, obj_buf->اندازه,
			     obj->type, &oid) < 0)
		die("failed to write object %s", oid_مقصد_hex(&obj->oid));
	obj->flags |= FLAG_WRITTEN;
}

/*
 * At the very end of the processing, write_rest() scans the objects
 * that have reachability requirements and calls this function.
 * Verify its reachability and validity recursively and write it out.
 */
ساكن صحيح check_object(بنية object *obj, تعداد object_type type,
			فراغ *داده استفادهنشده,
			بنية fsck_options *options استفادهنشده)
{
	بنية obj_میانگیر *obj_buf;

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

	obj_buf = lookup_object_میانگیر(obj);
	إذا (!obj_buf)
		die("Whoops! Cannot find object '%s'", oid_مقصد_hex(&obj->oid));
	إذا (fsck_object(obj, obj_buf->میانگیر, obj_buf->اندازه, &fsck_options))
		die("fsck error in packed object");
	fsck_options.walk = check_object;
	إذا (fsck_walk(obj, عدم, &fsck_options))
		die("Error on reachable objects of %s", oid_مقصد_hex(&obj->oid));
	write_cached_object(obj, obj_buf);
	أرجع 0;
}

ساكن فراغ write_rest(فراغ)
{
	بیعلامت i;
	لكل (i = 0; i < nr_objects; i++) {
		إذا (obj_list[i].obj)
			check_object(obj_list[i].obj, OBJ_ANY, عدم, عدم);
	}
}

ساكن فراغ added_object(بیعلامت nr, تعداد object_type type,
			 فراغ *داده, بیعلامت دراز اندازه);

/*
 * Write out nr-th object from the list, now we know the contents
 * of it.  Under --strict, this buffers structured objects in-core,
 * to be checked at the end.
 */
ساكن فراغ write_object(بیعلامت nr, تعداد object_type type,
			 فراغ *buf, بیعلامت دراز اندازه)
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
		obj = parse_object_میانگیر(the_repository, &obj_list[nr].oid,
					  type, اندازه, buf,
					  &eaten);
		إذا (!obj)
			die("invalid %s", type_name(type));
		جمع_کن_object_میانگیر(obj, buf, اندازه);
		obj->flags |= FLAG_OPEN;
		obj_list[nr].obj = obj;
	}
}

ساكن فراغ resolve_delta(بیعلامت nr, تعداد object_type type,
			  فراغ *پایه, بیعلامت دراز پایه_اندازه,
			  فراغ *delta, بیعلامت دراز delta_اندازه_2)
{
	فراغ *نتیجه;
	اندازه_t نتیجه_اندازه;

	نتیجه = patch_delta(پایه, پایه_اندازه,
			     delta, delta_اندازه_2,
			     &نتیجه_اندازه);
	إذا (!نتیجه)
		die("failed to apply delta");
	حرر(delta);
	write_object(nr, type, نتیجه, نتیجه_اندازه);
}

/*
 * We now know the contents of an object (which is nr-th in the pack);
 * resolve all the deltified objects that are based on it.
 */
ساكن فراغ added_object(بیعلامت nr, تعداد object_type type,
			 فراغ *داده, بیعلامت دراز اندازه)
{
	بنية delta_info **p = &delta_list;
	بنية delta_info *info;

	طالما ((info = *p) != عدم) {
		إذا (oideq(&info->پایه_oid, &obj_list[nr].oid) ||
		    info->پایه_جابه_جایی == obj_list[nr].جابه_جایی) {
			*p = info->بعدی;
			p = &delta_list;
			resolve_delta(info->nr, type, داده, اندازه,
				      info->delta, info->اندازه);
			حرر(info);
			استمر;
		}
		p = &info->بعدی;
	}
}

ساكن فراغ unpack_non_delta_مدخل(تعداد object_type type, بیعلامت دراز اندازه,
				   بیعلامت nr)
{
	فراغ *buf = get_داده(اندازه);

	إذا (buf)
		write_object(nr, type, buf, اندازه);
}

بنية zlib_جریان {
	بنية odb_جریان پایه;
	git_zstream *zstream;
	صحيح وضعیت;
};

ساكن ssize_t zlib_جریان_read(بنية odb_جریان *درون_جریان,
				حرف *buf, اندازه_t buf_len)
{
	بنية zlib_جریان *داده = container_از(درون_جریان, بنية zlib_جریان, پایه);
	git_zstream *zstream = داده->zstream;

	إذا (داده->وضعیت != Z_OK)
		أرجع 0;

	zstream->بعدی_بیرون = (بیعلامت حرف *) buf;
	zstream->avail_بیرون = buf_len;

	طالما (داده->وضعیت == Z_OK && zstream->avail_بیرون == buf_len) {
		zstream->بعدی_درون = fill(1);
		zstream->avail_درون = len;
		داده->وضعیت = git_inflate(zstream, 0);
		use(len - zstream->avail_درون);
	}

	أرجع buf_len - zstream->avail_بیرون;
}

ساكن فراغ جریان_blob(بیعلامت دراز اندازه, بیعلامت nr)
{
	git_zstream zstream = { 0 };
	بنية zlib_جریان درون_جریان = {
		.پایه = {
			.اقرأ = zlib_جریان_read,
			.اندازه = اندازه,
			.type = OBJ_BLOB,
		},
		.zstream = &zstream,
		.وضعیت = Z_OK,
	};
	بنية obj_info *info = &obj_list[nr];

	git_inflate_init(&zstream);

	إذا (odb_write_object_جریان(the_repository->objects, &درون_جریان.پایه, &info->oid))
		die(_("failed to write object in stream"));

	إذا (درون_جریان.وضعیت != Z_جریان_انتها)
		die(_("inflate returned (%d)"), درون_جریان.وضعیت);
	git_inflate_انتها(&zstream);

	إذا (strict) {
		بنية blob *blob = lookup_blob(the_repository, &info->oid);

		إذا (!blob)
			die(_("invalid blob object from stream"));
		blob->object.flags |= FLAG_WRITTEN;
	}
	info->obj = عدم;
}

ساكن صحيح resolve_against_held(بیعلامت nr, ثابت بنية object_id *پایه,
				فراغ *delta_داده, بیعلامت دراز delta_اندازه_2)
{
	بنية object *obj;
	بنية obj_میانگیر *obj_میانگیر;
	obj = lookup_object(the_repository, پایه);
	إذا (!obj)
		أرجع 0;
	obj_میانگیر = lookup_object_میانگیر(obj);
	إذا (!obj_میانگیر)
		أرجع 0;
	resolve_delta(nr, obj->type, obj_میانگیر->میانگیر,
		      obj_میانگیر->اندازه, delta_داده, delta_اندازه_2);
	أرجع 1;
}

ساكن فراغ unpack_delta_مدخل(تعداد object_type type, بیعلامت دراز delta_اندازه_2,
			       بیعلامت nr)
{
	فراغ *delta_داده, *پایه;
	بیعلامت دراز پایه_اندازه;
	اندازه_t پایه_اندازه_st = 0;
	بنية object_id پایه_oid;

	إذا (type == OBJ_REF_DELTA) {
		oidread(&پایه_oid, fill(the_hash_algo->rawsz), the_repository->hash_algo);
		use(the_hash_algo->rawsz);
		delta_داده = get_داده(delta_اندازه_2);
		إذا (!delta_داده)
			أرجع;
		إذا (odb_has_object(the_repository->objects, &پایه_oid,
				   ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR))
			; /* Ok we have this one */
		وإلا إذا (resolve_against_held(nr, &پایه_oid,
					      delta_داده, delta_اندازه_2))
			أرجع; /* we are done */
		وإلا {
			/* cannot resolve yet --- queue it */
			oidclr(&obj_list[nr].oid, the_repository->hash_algo);
			جمع_کن_delta_مقصد_list(nr, &پایه_oid, 0, delta_داده, delta_اندازه_2);
			أرجع;
		}
	} وإلا {
		بیعلامت پایه_یافت = 0;
		بیعلامت حرف *pack, c;
		off_t پایه_جابه_جایی;
		بیعلامت lo, mid, hi;

		pack = fill(1);
		c = *pack;
		use(1);
		پایه_جابه_جایی = c & 127;
		طالما (c & 128) {
			پایه_جابه_جایی += 1;
			إذا (!پایه_جابه_جایی || MSB(پایه_جابه_جایی, 7))
				die("offset value overflow for delta base object");
			pack = fill(1);
			c = *pack;
			use(1);
			پایه_جابه_جایی = (پایه_جابه_جایی << 7) + (c & 127);
		}
		پایه_جابه_جایی = obj_list[nr].جابه_جایی - پایه_جابه_جایی;
		إذا (پایه_جابه_جایی <= 0 || پایه_جابه_جایی >= obj_list[nr].جابه_جایی)
			die("offset value out of bound for delta base object");

		delta_داده = get_داده(delta_اندازه_2);
		إذا (!delta_داده)
			أرجع;
		lo = 0;
		hi = nr;
		طالما (lo < hi) {
			mid = lo + (hi - lo) / 2;
			إذا (پایه_جابه_جایی < obj_list[mid].جابه_جایی) {
				hi = mid;
			} وإلا إذا (پایه_جابه_جایی > obj_list[mid].جابه_جایی) {
				lo = mid + 1;
			} وإلا {
				oidcpy(&پایه_oid, &obj_list[mid].oid);
				پایه_یافت = !is_null_oid(&پایه_oid);
				اكسر;
			}
		}
		إذا (!پایه_یافت) {
			/*
			 * The delta base object is itself a delta that
			 * has not been resolved yet.
			 */
			oidclr(&obj_list[nr].oid, the_repository->hash_algo);
			جمع_کن_delta_مقصد_list(nr, null_oid(the_hash_algo), پایه_جابه_جایی,
					  delta_داده, delta_اندازه_2);
			أرجع;
		}
	}

	إذا (resolve_against_held(nr, &پایه_oid, delta_داده, delta_اندازه_2))
		أرجع;

	پایه = odb_read_object(the_repository->objects, &پایه_oid,
			       &type, &پایه_اندازه_st);
	پایه_اندازه = cast_اندازه_t_مقصد_ulong(پایه_اندازه_st);
	إذا (!پایه) {
		error("failed to read delta-pack base object %s",
		      oid_مقصد_hex(&پایه_oid));
		إذا (!recover)
			خروج(1);
		has_errors = 1;
		أرجع;
	}
	resolve_delta(nr, type, پایه, پایه_اندازه, delta_داده, delta_اندازه_2);
	حرر(پایه);
}

ساكن فراغ unpack_one(بیعلامت nr)
{
	بیعلامت لغزش;
	بیعلامت حرف *pack;
	اندازه_t اندازه, c;
	تعداد object_type type;

	obj_list[nr].جابه_جایی = consumed_بایتها;

	pack = fill(1);
	c = *pack;
	use(1);
	type = (c >> 4) & 7;
	اندازه = (c & 15);
	لغزش = 4;
	طالما (c & 0x80) {
		إذا ((bitsizeof(اندازه_t) - 7) < لغزش)
			die(_("object size too large for this platform"));
		pack = fill(1);
		c = *pack;
		use(1);
		اندازه += (c & 0x7f) << لغزش;
		لغزش += 7;
	}

	بدل (type) {
	حالة OBJ_BLOB:
		إذا (!dry_پیاپی &&
		    اندازه > repo_settings_get_big_file_آستانه(the_repository)) {
			جریان_blob(اندازه, nr);
			أرجع;
		}
		/* fallthrough */
	حالة OBJ_COMMIT:
	حالة OBJ_درخت:
	حالة OBJ_برچسب:
		unpack_non_delta_مدخل(type, اندازه, nr);
		أرجع;
	حالة OBJ_REF_DELTA:
	حالة OBJ_OFS_DELTA:
		unpack_delta_مدخل(type, اندازه, nr);
		أرجع;
	افتراضي:
		error("bad object type %d", type);
		has_errors = 1;
		إذا (recover)
			أرجع;
		خروج(1);
	}
}

ساكن فراغ unpack_all(فراغ)
{
	صحيح i;
	بیعلامت حرف *hdr = fill(حجم(بنية pack_سرآیند));
	بنية odb_transaction *transaction;

	إذا (get_be32(hdr) != PACK_SIGNATURE)
		die("bad pack file");
	hdr += 4;
	إذا (!pack_نسخه_ok_native(get_be32(hdr)))
		die("unknown pack file version %"PRIu32,
		    get_be32(hdr));
	hdr += 4;
	nr_objects = get_be32(hdr);
	use(حجم(بنية pack_سرآیند));

	إذا (!quiet)
		پیشرفت = شروع_پیشرفت(the_repository,
					  _("Unpacking objects"), nr_objects);
	CALLOC_ARRAY(obj_list, nr_objects);
	odb_transaction_begin_or_die(the_repository->objects, &transaction, 0);
	لكل (i = 0; i < nr_objects; i++) {
		unpack_one(i);
		display_پیشرفت(پیشرفت, i + 1);
	}
	odb_transaction_commit_and_finalize_or_die(transaction);
	stop_پیشرفت(&پیشرفت);

	إذا (delta_list)
		die("unresolved deltas left after unpacking");
}

صحيح cmd_unpack_objects(صحيح argc,
		       ثابت حرف **argv,
		       ثابت حرف *پیشوند استفادهنشده,
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
				dry_پیاپی = 1;
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
			إذا (skip_پیشوند(arg, "--strict=", &arg)) {
				strict = 1;
				fsck_مجموعه_پیام_types(&fsck_options, arg);
				استمر;
			}
			إذا (skip_پیشوند(arg, "--pack_header=", &arg)) {
				إذا (parse_pack_سرآیند_option(arg,
							     میانگیر, &len) < 0)
					die(_("bad --pack_header: %s"), arg);
				استمر;
			}
			إذا (skip_پیشوند(arg, "--max-input-size=", &arg)) {
				max_درون_داد_اندازه = strtoumax(arg, عدم, 10);
				استمر;
			}
			usage(unpack_usage);
		}

		/* We don't take any non-flag arguments now.. Maybe some day */
		usage(unpack_usage);
	}
	git_hash_init(&ctx, the_hash_algo);
	unpack_all();
	git_hash_update(&ctx, میانگیر, جابه_جایی);
	git_hash_init(&tmp_ctx, the_hash_algo);
	git_hash_clone(&tmp_ctx, &ctx);
	git_hash_final_oid(&oid, &tmp_ctx);
	إذا (strict) {
		write_rest();
		إذا (fsck_پایان(&fsck_options))
			die(_("fsck error in pack objects"));
	}
	إذا (!hasheq(fill(the_hash_algo->rawsz), oid.hash,
		    the_repository->hash_algo))
		die("final sha1 did not match");
	use(the_hash_algo->rawsz);

	/* Write the last part of the buffer to stdout */
	write_درون_پر(1, میانگیر + جابه_جایی, len);

	/* All done */
	أرجع has_errors;
}
