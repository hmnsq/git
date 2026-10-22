#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
#تعریف_کرو DISABLE_SIGN_COMPARE_WARNINGS

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

ساكن صحيح dry_لگاتار, quiet, recover, has_errors, strict;
ساكن ثابت حرف unpack_usage[] = "git unpack-objects [-n] [-q] [-r] [--strict]";

ساكن بنانشانی حرف بفر[DEFAULT_IO_بفر_ناپ];
ساكن بنانشانی صحيح وتھ, len;
ساكن off_t consumed_بائٹاں;
ساكن off_t max_داخلہ_ناپ;
ساكن بنية git_hash_ctx ctx;
ساكن بنية fsck_options fsck_options;
ساكن بنية ترقی *ترقی;

/*
 * When running under --strict mode, objects whose reachability are
 * suspect are kept in core without getting written in the object
 * store.
 */
بنية obj_بفر {
	حرف *بفر;
	بنانشانی لما ناپ;
};

ساكن بنية decoration obj_decorate;

ساكن بنية obj_بفر *lookup_object_بفر(بنية object *ادھار)
{
	أرجع lookup_decoration(&obj_decorate, ادھار);
}

ساكن فراغ جوڑو_object_بفر(بنية object *object, حرف *بفر, بنانشانی لما ناپ)
{
	بنية obj_بفر *obj;
	CALLOC_ARRAY(obj, 1);
	obj->بفر = بفر;
	obj->ناپ = ناپ;
	إذا (جوڑو_decoration(&obj_decorate, object, obj))
		die("object %s tried to add buffer twice!", oid_منزل_hex(&object->oid));
}

/*
 * Make sure at least "min" bytes are available in the buffer, and
 * return the pointer to the buffer.
 */
ساكن فراغ *fill(صحيح min)
{
	إذا (min <= len)
		أرجع بفر + وتھ;
	إذا (min > حجم(بفر))
		die("cannot fill %d bytes", min);
	إذا (وتھ) {
		git_hash_update(&ctx, بفر, وتھ);
		memmove(بفر, بفر + وتھ, len);
		وتھ = 0;
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

ساكن فراغ use(صحيح بائٹاں)
{
	إذا (بائٹاں > len)
		die("used more bytes than were available");
	len -= بائٹاں;
	وتھ += بائٹاں;

	/* make sure off_t is sufficiently large not to wrap */
	إذا (signed_جوڑو_overflows(consumed_بائٹاں, بائٹاں))
		die("pack too large for current definition of off_t");
	consumed_بائٹاں += بائٹاں;
	إذا (max_داخلہ_ناپ && consumed_بائٹاں > max_داخلہ_ناپ)
		die(_("pack exceeds maximum allowed size"));
	display_throughput(ترقی, consumed_بائٹاں);
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
ساكن فراغ *get_انکڑے(بنانشانی لما ناپ)
{
	git_zstream دھارا;
	بنانشانی لما bufsize = dry_لگاتار && ناپ > 8192 ? 8192 : ناپ;
	فراغ *buf = xmallocz(bufsize);

	memset(&دھارا, 0, حجم(دھارا));

	دھارا.اگلا_باہرلا = buf;
	دھارا.avail_باہرلا = bufsize;
	دھارا.اگلا_وچ = fill(1);
	دھارا.avail_وچ = len;
	git_inflate_init(&دھارا);

	لكل (;;) {
		صحيح ret = git_inflate(&دھارا, 0);
		use(len - دھارا.avail_وچ);
		إذا (دھارا.کل_باہرلا == ناپ && ret == Z_دھارا_سرا)
			اكسر;
		إذا (ret != Z_OK) {
			error("inflate returned %d", ret);
			FREE_AND_NULL(buf);
			إذا (!recover)
				باہر_نکلو(1);
			has_errors = 1;
			اكسر;
		}
		دھارا.اگلا_وچ = fill(1);
		دھارا.avail_وچ = len;
		إذا (dry_لگاتار) {
			/* reuse the buffer in dry_run mode */
			دھارا.اگلا_باہرلا = buf;
			دھارا.avail_باہرلا = bufsize > ناپ - دھارا.کل_باہرلا ?
						   ناپ - دھارا.کل_باہرلا :
						   bufsize;
		}
	}
	git_inflate_سرا(&دھارا);
	إذا (dry_لگاتار)
		FREE_AND_NULL(buf);
	أرجع buf;
}

بنية delta_info {
	بنية object_id ادھار_oid;
	بنانشانی nr;
	off_t ادھار_وتھ;
	بنانشانی لما ناپ;
	فراغ *delta;
	بنية delta_info *اگلا;
};

ساكن بنية delta_info *delta_list;

ساكن فراغ جوڑو_delta_منزل_list(بنانشانی nr, ثابت بنية object_id *ادھار_oid,
			      off_t ادھار_وتھ,
			      فراغ *delta, بنانشانی لما ناپ)
{
	بنية delta_info *info = xmalloc(حجم(*info));

	oidcpy(&info->ادھار_oid, ادھار_oid);
	info->ادھار_وتھ = ادھار_وتھ;
	info->ناپ = ناپ;
	info->delta = delta;
	info->nr = nr;
	info->اگلا = delta_list;
	delta_list = info;
}

بنية obj_info {
	off_t وتھ;
	بنية object_id oid;
	بنية object *obj;
};

/* Remember to update object flag allocation in object.h */
#تعریف_کرو FLAG_OPEN (1u<<20)
#تعریف_کرو FLAG_WRITTEN (1u<<21)

ساكن بنية obj_info *obj_list;
ساكن بنانشانی nr_objects;

/*
 * Called only from check_object() after it verified this object
 * is Ok.
 */
ساكن فراغ write_cached_object(بنية object *obj, بنية obj_بفر *obj_buf)
{
	بنية object_id oid;

	إذا (odb_write_object(the_repository->objects, obj_buf->بفر, obj_buf->ناپ,
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
			فراغ *انکڑے بن_ورتیا,
			بنية fsck_options *options بن_ورتیا)
{
	بنية obj_بفر *obj_buf;

	إذا (!obj)
		أرجع 1;

	إذا (obj->flags & FLAG_WRITTEN)
		أرجع 0;

	إذا (type != OBJ_ANY && obj->type != type)
		die("object type mismatch");

	إذا (!(obj->flags & FLAG_OPEN)) {
		ناپ_t ناپ;
		صحيح type = odb_read_object_info(the_repository->objects, &obj->oid, &ناپ);
		إذا (type != obj->type || type <= 0)
			die("object of unexpected type");
		obj->flags |= FLAG_WRITTEN;
		أرجع 0;
	}

	obj_buf = lookup_object_بفر(obj);
	إذا (!obj_buf)
		die("Whoops! Cannot find object '%s'", oid_منزل_hex(&obj->oid));
	إذا (fsck_object(obj, obj_buf->بفر, obj_buf->ناپ, &fsck_options))
		die("fsck error in packed object");
	fsck_options.walk = check_object;
	إذا (fsck_walk(obj, عدم, &fsck_options))
		die("Error on reachable objects of %s", oid_منزل_hex(&obj->oid));
	write_cached_object(obj, obj_buf);
	أرجع 0;
}

ساكن فراغ write_rest(فراغ)
{
	بنانشانی i;
	لكل (i = 0; i < nr_objects; i++) {
		إذا (obj_list[i].obj)
			check_object(obj_list[i].obj, OBJ_ANY, عدم, عدم);
	}
}

ساكن فراغ added_object(بنانشانی nr, تعداد object_type type,
			 فراغ *انکڑے, بنانشانی لما ناپ);

/*
 * Write out nr-th object from the list, now we know the contents
 * of it.  Under --strict, this buffers structured objects in-core,
 * to be checked at the end.
 */
ساكن فراغ write_object(بنانشانی nr, تعداد object_type type,
			 فراغ *buf, بنانشانی لما ناپ)
{
	إذا (!strict) {
		إذا (odb_write_object(the_repository->objects, buf, ناپ, type,
				     &obj_list[nr].oid) < 0)
			die("failed to write object");
		added_object(nr, type, buf, ناپ);
		حرر(buf);
		obj_list[nr].obj = عدم;
	} وإلا إذا (type == OBJ_BLOB) {
		بنية blob *blob;
		إذا (odb_write_object(the_repository->objects, buf, ناپ, type,
				     &obj_list[nr].oid) < 0)
			die("failed to write object");
		added_object(nr, type, buf, ناپ);
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
		hash_object_file(the_hash_algo, buf, ناپ, type,
				 &obj_list[nr].oid);
		added_object(nr, type, buf, ناپ);
		obj = parse_object_بفر(the_repository, &obj_list[nr].oid,
					  type, ناپ, buf,
					  &eaten);
		إذا (!obj)
			die("invalid %s", type_name(type));
		جوڑو_object_بفر(obj, buf, ناپ);
		obj->flags |= FLAG_OPEN;
		obj_list[nr].obj = obj;
	}
}

ساكن فراغ resolve_delta(بنانشانی nr, تعداد object_type type,
			  فراغ *ادھار, بنانشانی لما ادھار_ناپ,
			  فراغ *delta, بنانشانی لما delta_ناپ_2)
{
	فراغ *نتیجہ;
	ناپ_t نتیجہ_ناپ;

	نتیجہ = patch_delta(ادھار, ادھار_ناپ,
			     delta, delta_ناپ_2,
			     &نتیجہ_ناپ);
	إذا (!نتیجہ)
		die("failed to apply delta");
	حرر(delta);
	write_object(nr, type, نتیجہ, نتیجہ_ناپ);
}

/*
 * We now know the contents of an object (which is nr-th in the pack);
 * resolve all the deltified objects that are based on it.
 */
ساكن فراغ added_object(بنانشانی nr, تعداد object_type type,
			 فراغ *انکڑے, بنانشانی لما ناپ)
{
	بنية delta_info **p = &delta_list;
	بنية delta_info *info;

	طالما ((info = *p) != عدم) {
		إذا (oideq(&info->ادھار_oid, &obj_list[nr].oid) ||
		    info->ادھار_وتھ == obj_list[nr].وتھ) {
			*p = info->اگلا;
			p = &delta_list;
			resolve_delta(info->nr, type, انکڑے, ناپ,
				      info->delta, info->ناپ);
			حرر(info);
			استمر;
		}
		p = &info->اگلا;
	}
}

ساكن فراغ unpack_non_delta_اندراج(تعداد object_type type, بنانشانی لما ناپ,
				   بنانشانی nr)
{
	فراغ *buf = get_انکڑے(ناپ);

	إذا (buf)
		write_object(nr, type, buf, ناپ);
}

بنية zlib_دھارا {
	بنية odb_دھارا ادھار;
	git_zstream *zstream;
	صحيح حالت;
};

ساكن ssize_t zlib_دھارا_read(بنية odb_دھارا *وچ_دھارا,
				حرف *buf, ناپ_t buf_len)
{
	بنية zlib_دھارا *انکڑے = container_دا(وچ_دھارا, بنية zlib_دھارا, ادھار);
	git_zstream *zstream = انکڑے->zstream;

	إذا (انکڑے->حالت != Z_OK)
		أرجع 0;

	zstream->اگلا_باہرلا = (بنانشانی حرف *) buf;
	zstream->avail_باہرلا = buf_len;

	طالما (انکڑے->حالت == Z_OK && zstream->avail_باہرلا == buf_len) {
		zstream->اگلا_وچ = fill(1);
		zstream->avail_وچ = len;
		انکڑے->حالت = git_inflate(zstream, 0);
		use(len - zstream->avail_وچ);
	}

	أرجع buf_len - zstream->avail_باہرلا;
}

ساكن فراغ دھارا_blob(بنانشانی لما ناپ, بنانشانی nr)
{
	git_zstream zstream = { 0 };
	بنية zlib_دھارا وچ_دھارا = {
		.ادھار = {
			.اقرأ = zlib_دھارا_read,
			.ناپ = ناپ,
			.type = OBJ_BLOB,
		},
		.zstream = &zstream,
		.حالت = Z_OK,
	};
	بنية obj_info *info = &obj_list[nr];

	git_inflate_init(&zstream);

	إذا (odb_write_object_دھارا(the_repository->objects, &وچ_دھارا.ادھار, &info->oid))
		die(_("failed to write object in stream"));

	إذا (وچ_دھارا.حالت != Z_دھارا_سرا)
		die(_("inflate returned (%d)"), وچ_دھارا.حالت);
	git_inflate_سرا(&zstream);

	إذا (strict) {
		بنية blob *blob = lookup_blob(the_repository, &info->oid);

		إذا (!blob)
			die(_("invalid blob object from stream"));
		blob->object.flags |= FLAG_WRITTEN;
	}
	info->obj = عدم;
}

ساكن صحيح resolve_against_held(بنانشانی nr, ثابت بنية object_id *ادھار,
				فراغ *delta_انکڑے, بنانشانی لما delta_ناپ_2)
{
	بنية object *obj;
	بنية obj_بفر *obj_بفر;
	obj = lookup_object(the_repository, ادھار);
	إذا (!obj)
		أرجع 0;
	obj_بفر = lookup_object_بفر(obj);
	إذا (!obj_بفر)
		أرجع 0;
	resolve_delta(nr, obj->type, obj_بفر->بفر,
		      obj_بفر->ناپ, delta_انکڑے, delta_ناپ_2);
	أرجع 1;
}

ساكن فراغ unpack_delta_اندراج(تعداد object_type type, بنانشانی لما delta_ناپ_2,
			       بنانشانی nr)
{
	فراغ *delta_انکڑے, *ادھار;
	بنانشانی لما ادھار_ناپ;
	ناپ_t ادھار_ناپ_st = 0;
	بنية object_id ادھار_oid;

	إذا (type == OBJ_REF_DELTA) {
		oidread(&ادھار_oid, fill(the_hash_algo->rawsz), the_repository->hash_algo);
		use(the_hash_algo->rawsz);
		delta_انکڑے = get_انکڑے(delta_ناپ_2);
		إذا (!delta_انکڑے)
			أرجع;
		إذا (odb_has_object(the_repository->objects, &ادھار_oid,
				   ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR))
			; /* Ok we have this one */
		وإلا إذا (resolve_against_held(nr, &ادھار_oid,
					      delta_انکڑے, delta_ناپ_2))
			أرجع; /* we are done */
		وإلا {
			/* cannot resolve yet --- queue it */
			oidclr(&obj_list[nr].oid, the_repository->hash_algo);
			جوڑو_delta_منزل_list(nr, &ادھار_oid, 0, delta_انکڑے, delta_ناپ_2);
			أرجع;
		}
	} وإلا {
		بنانشانی ادھار_لبھیا = 0;
		بنانشانی حرف *pack, c;
		off_t ادھار_وتھ;
		بنانشانی lo, mid, hi;

		pack = fill(1);
		c = *pack;
		use(1);
		ادھار_وتھ = c & 127;
		طالما (c & 128) {
			ادھار_وتھ += 1;
			إذا (!ادھار_وتھ || MSB(ادھار_وتھ, 7))
				die("offset value overflow for delta base object");
			pack = fill(1);
			c = *pack;
			use(1);
			ادھار_وتھ = (ادھار_وتھ << 7) + (c & 127);
		}
		ادھار_وتھ = obj_list[nr].وتھ - ادھار_وتھ;
		إذا (ادھار_وتھ <= 0 || ادھار_وتھ >= obj_list[nr].وتھ)
			die("offset value out of bound for delta base object");

		delta_انکڑے = get_انکڑے(delta_ناپ_2);
		إذا (!delta_انکڑے)
			أرجع;
		lo = 0;
		hi = nr;
		طالما (lo < hi) {
			mid = lo + (hi - lo) / 2;
			إذا (ادھار_وتھ < obj_list[mid].وتھ) {
				hi = mid;
			} وإلا إذا (ادھار_وتھ > obj_list[mid].وتھ) {
				lo = mid + 1;
			} وإلا {
				oidcpy(&ادھار_oid, &obj_list[mid].oid);
				ادھار_لبھیا = !is_null_oid(&ادھار_oid);
				اكسر;
			}
		}
		إذا (!ادھار_لبھیا) {
			/*
			 * The delta base object is itself a delta that
			 * has not been resolved yet.
			 */
			oidclr(&obj_list[nr].oid, the_repository->hash_algo);
			جوڑو_delta_منزل_list(nr, null_oid(the_hash_algo), ادھار_وتھ,
					  delta_انکڑے, delta_ناپ_2);
			أرجع;
		}
	}

	إذا (resolve_against_held(nr, &ادھار_oid, delta_انکڑے, delta_ناپ_2))
		أرجع;

	ادھار = odb_read_object(the_repository->objects, &ادھار_oid,
			       &type, &ادھار_ناپ_st);
	ادھار_ناپ = cast_ناپ_t_منزل_ulong(ادھار_ناپ_st);
	إذا (!ادھار) {
		error("failed to read delta-pack base object %s",
		      oid_منزل_hex(&ادھار_oid));
		إذا (!recover)
			باہر_نکلو(1);
		has_errors = 1;
		أرجع;
	}
	resolve_delta(nr, type, ادھار, ادھار_ناپ, delta_انکڑے, delta_ناپ_2);
	حرر(ادھار);
}

ساكن فراغ unpack_one(بنانشانی nr)
{
	بنانشانی کھسکاؤ;
	بنانشانی حرف *pack;
	ناپ_t ناپ, c;
	تعداد object_type type;

	obj_list[nr].وتھ = consumed_بائٹاں;

	pack = fill(1);
	c = *pack;
	use(1);
	type = (c >> 4) & 7;
	ناپ = (c & 15);
	کھسکاؤ = 4;
	طالما (c & 0x80) {
		إذا ((bitsizeof(ناپ_t) - 7) < کھسکاؤ)
			die(_("object size too large for this platform"));
		pack = fill(1);
		c = *pack;
		use(1);
		ناپ += (c & 0x7f) << کھسکاؤ;
		کھسکاؤ += 7;
	}

	بدل (type) {
	حالة OBJ_BLOB:
		إذا (!dry_لگاتار &&
		    ناپ > repo_settings_get_big_file_حد(the_repository)) {
			دھارا_blob(ناپ, nr);
			أرجع;
		}
		/* fallthrough */
	حالة OBJ_COMMIT:
	حالة OBJ_درخت:
	حالة OBJ_ٹیگ:
		unpack_non_delta_اندراج(type, ناپ, nr);
		أرجع;
	حالة OBJ_REF_DELTA:
	حالة OBJ_OFS_DELTA:
		unpack_delta_اندراج(type, ناپ, nr);
		أرجع;
	افتراضي:
		error("bad object type %d", type);
		has_errors = 1;
		إذا (recover)
			أرجع;
		باہر_نکلو(1);
	}
}

ساكن فراغ unpack_all(فراغ)
{
	صحيح i;
	بنانشانی حرف *hdr = fill(حجم(بنية pack_سرخی));
	بنية odb_transaction *transaction;

	إذا (get_be32(hdr) != PACK_SIGNATURE)
		die("bad pack file");
	hdr += 4;
	إذا (!pack_نسخہ_ok_native(get_be32(hdr)))
		die("unknown pack file version %"PRIu32,
		    get_be32(hdr));
	hdr += 4;
	nr_objects = get_be32(hdr);
	use(حجم(بنية pack_سرخی));

	إذا (!quiet)
		ترقی = شروع_ترقی(the_repository,
					  _("Unpacking objects"), nr_objects);
	CALLOC_ARRAY(obj_list, nr_objects);
	odb_transaction_begin_or_die(the_repository->objects, &transaction, 0);
	لكل (i = 0; i < nr_objects; i++) {
		unpack_one(i);
		display_ترقی(ترقی, i + 1);
	}
	odb_transaction_commit_and_finalize_or_die(transaction);
	stop_ترقی(&ترقی);

	إذا (delta_list)
		die("unresolved deltas left after unpacking");
}

صحيح cmd_unpack_objects(صحيح argc,
		       ثابت حرف **argv,
		       ثابت حرف *اگیتر بن_ورتیا,
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
				dry_لگاتار = 1;
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
			إذا (skip_اگیتر(arg, "--strict=", &arg)) {
				strict = 1;
				fsck_سیٹ_پیغام_types(&fsck_options, arg);
				استمر;
			}
			إذا (skip_اگیتر(arg, "--pack_header=", &arg)) {
				إذا (parse_pack_سرخی_option(arg,
							     بفر, &len) < 0)
					die(_("bad --pack_header: %s"), arg);
				استمر;
			}
			إذا (skip_اگیتر(arg, "--max-input-size=", &arg)) {
				max_داخلہ_ناپ = strtoumax(arg, عدم, 10);
				استمر;
			}
			usage(unpack_usage);
		}

		/* We don't take any non-flag arguments now.. Maybe some day */
		usage(unpack_usage);
	}
	git_hash_init(&ctx, the_hash_algo);
	unpack_all();
	git_hash_update(&ctx, بفر, وتھ);
	git_hash_init(&tmp_ctx, the_hash_algo);
	git_hash_clone(&tmp_ctx, &ctx);
	git_hash_final_oid(&oid, &tmp_ctx);
	إذا (strict) {
		write_rest();
		إذا (fsck_انت(&fsck_options))
			die(_("fsck error in pack objects"));
	}
	إذا (!hasheq(fill(the_hash_algo->rawsz), oid.hash,
		    the_repository->hash_algo))
		die("final sha1 did not match");
	use(the_hash_algo->rawsz);

	/* Write the last part of the buffer to stdout */
	write_وچ_بھریا(1, بفر + وتھ, len);

	/* All done */
	أرجع has_errors;
}
