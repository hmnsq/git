#నిర్వచించు USE_THE_REPOSITORY_VARIABLE
#నిర్వచించు DISABLE_SIGN_COMPARE_WARNINGS

#అన్తర్భావ "builtin.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "environment.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "git-zlib.h"
#అన్తర్భావ "hex.h"
#అన్తర్భావ "object-file.h"
#అన్తర్భావ "odb.h"
#అన్తర్భావ "odb/streaming.h"
#అన్తర్భావ "odb/transaction.h"
#అన్తర్భావ "object.h"
#అన్తర్భావ "delta.h"
#అన్తర్భావ "pack.h"
#అన్తర్భావ "blob.h"
#అన్తర్భావ "replace-object.h"
#అన్తర్భావ "strbuf.h"
#అన్తర్భావ "progress.h"
#అన్తర్భావ "decorate.h"
#అన్తర్భావ "fsck.h"
#అన్తర్భావ "packfile.h"

స్థిర పూర్ణాంకం dry_నిరంతరం, quiet, recover, has_errors, strict;
స్థిర స్థిరరాశి అక్షరం unpack_usage[] = "git unpack-objects [-n] [-q] [-r] [--strict]";

స్థిర గుర్తులేని అక్షరం బఫరు[DEFAULT_IO_బఫరు_కొలత];
స్థిర గుర్తులేని పూర్ణాంకం స్థానభ్రంశం, len;
స్థిర off_t consumed_బైట్లు;
స్థిర off_t max_ఆదానం_కొలత;
స్థిర నిర్మాణం git_hash_ctx ctx;
స్థిర నిర్మాణం fsck_options fsck_options;
స్థిర నిర్మాణం పురోగతి *పురోగతి;

/*
 * When running under --strict mode, objects whose reachability are
 * suspect are kept in core without getting written in the object
 * store.
 */
నిర్మాణం obj_బఫరు {
	అక్షరం *బఫరు;
	గుర్తులేని దీర్ఘ కొలత;
};

స్థిర నిర్మాణం decoration obj_decorate;

స్థిర నిర్మాణం obj_బఫరు *lookup_object_బఫరు(నిర్మాణం object *పునాది)
{
	తిరిగి lookup_decoration(&obj_decorate, పునాది);
}

స్థిర శూన్య కూడు_object_బఫరు(నిర్మాణం object *object, అక్షరం *బఫరు, గుర్తులేని దీర్ఘ కొలత)
{
	నిర్మాణం obj_బఫరు *obj;
	CALLOC_ARRAY(obj, 1);
	obj->బఫరు = బఫరు;
	obj->కొలత = కొలత;
	ఒకవేళ (కూడు_decoration(&obj_decorate, object, obj))
		die("object %s tried to add buffer twice!", oid_గమ్యం_hex(&object->oid));
}

/*
 * Make sure at least "min" bytes are available in the buffer, and
 * return the pointer to the buffer.
 */
స్థిర శూన్య *fill(పూర్ణాంకం min)
{
	ఒకవేళ (min <= len)
		తిరిగి బఫరు + స్థానభ్రంశం;
	ఒకవేళ (min > పరిమాణం(బఫరు))
		die("cannot fill %d bytes", min);
	ఒకవేళ (స్థానభ్రంశం) {
		git_hash_update(&ctx, బఫరు, స్థానభ్రంశం);
		memmove(బఫరు, బఫరు + స్థానభ్రంశం, len);
		స్థానభ్రంశం = 0;
	}
	చేయి {
		ssize_t ret = xread(0, బఫరు + len, పరిమాణం(బఫరు) - len);
		ఒకవేళ (ret <= 0) {
			ఒకవేళ (!ret)
				die("early EOF");
			die_errno("read error on input");
		}
		len += ret;
	} అయితే (len < min);
	తిరిగి బఫరు;
}

స్థిర శూన్య use(పూర్ణాంకం బైట్లు)
{
	ఒకవేళ (బైట్లు > len)
		die("used more bytes than were available");
	len -= బైట్లు;
	స్థానభ్రంశం += బైట్లు;

	/* make sure off_t is sufficiently large not to wrap */
	ఒకవేళ (signed_కూడు_overflows(consumed_బైట్లు, బైట్లు))
		die("pack too large for current definition of off_t");
	consumed_బైట్లు += బైట్లు;
	ఒకవేళ (max_ఆదానం_కొలత && consumed_బైట్లు > max_ఆదానం_కొలత)
		die(_("pack exceeds maximum allowed size"));
	display_throughput(పురోగతి, consumed_బైట్లు);
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
స్థిర శూన్య *get_దత్తాంశం(గుర్తులేని దీర్ఘ కొలత)
{
	git_zstream ప్రవాహం;
	గుర్తులేని దీర్ఘ bufsize = dry_నిరంతరం && కొలత > 8192 ? 8192 : కొలత;
	శూన్య *buf = xmallocz(bufsize);

	memset(&ప్రవాహం, 0, పరిమాణం(ప్రవాహం));

	ప్రవాహం.తదుపరి_బహిర్గతం = buf;
	ప్రవాహం.avail_బహిర్గతం = bufsize;
	ప్రవాహం.తదుపరి_లోపల = fill(1);
	ప్రవాహం.avail_లోపల = len;
	git_inflate_init(&ప్రవాహం);

	కొరకు (;;) {
		పూర్ణాంకం ret = git_inflate(&ప్రవాహం, 0);
		use(len - ప్రవాహం.avail_లోపల);
		ఒకవేళ (ప్రవాహం.మొత్తం_బహిర్గతం == కొలత && ret == Z_ప్రవాహం_చివర)
			ఆపు;
		ఒకవేళ (ret != Z_OK) {
			error("inflate returned %d", ret);
			FREE_AND_NULL(buf);
			ఒకవేళ (!recover)
				నిష్క్రమించు(1);
			has_errors = 1;
			ఆపు;
		}
		ప్రవాహం.తదుపరి_లోపల = fill(1);
		ప్రవాహం.avail_లోపల = len;
		ఒకవేళ (dry_నిరంతరం) {
			/* reuse the buffer in dry_run mode */
			ప్రవాహం.తదుపరి_బహిర్గతం = buf;
			ప్రవాహం.avail_బహిర్గతం = bufsize > కొలత - ప్రవాహం.మొత్తం_బహిర్గతం ?
						   కొలత - ప్రవాహం.మొత్తం_బహిర్గతం :
						   bufsize;
		}
	}
	git_inflate_చివర(&ప్రవాహం);
	ఒకవేళ (dry_నిరంతరం)
		FREE_AND_NULL(buf);
	తిరిగి buf;
}

నిర్మాణం delta_info {
	నిర్మాణం object_id పునాది_oid;
	గుర్తులేని nr;
	off_t పునాది_స్థానభ్రంశం;
	గుర్తులేని దీర్ఘ కొలత;
	శూన్య *delta;
	నిర్మాణం delta_info *తదుపరి;
};

స్థిర నిర్మాణం delta_info *delta_list;

స్థిర శూన్య కూడు_delta_గమ్యం_list(గుర్తులేని nr, స్థిరరాశి నిర్మాణం object_id *పునాది_oid,
			      off_t పునాది_స్థానభ్రంశం,
			      శూన్య *delta, గుర్తులేని దీర్ఘ కొలత)
{
	నిర్మాణం delta_info *info = xmalloc(పరిమాణం(*info));

	oidcpy(&info->పునాది_oid, పునాది_oid);
	info->పునాది_స్థానభ్రంశం = పునాది_స్థానభ్రంశం;
	info->కొలత = కొలత;
	info->delta = delta;
	info->nr = nr;
	info->తదుపరి = delta_list;
	delta_list = info;
}

నిర్మాణం obj_info {
	off_t స్థానభ్రంశం;
	నిర్మాణం object_id oid;
	నిర్మాణం object *obj;
};

/* Remember to update object flag allocation in object.h */
#నిర్వచించు FLAG_OPEN (1u<<20)
#నిర్వచించు FLAG_WRITTEN (1u<<21)

స్థిర నిర్మాణం obj_info *obj_list;
స్థిర గుర్తులేని nr_objects;

/*
 * Called only from check_object() after it verified this object
 * is Ok.
 */
స్థిర శూన్య write_cached_object(నిర్మాణం object *obj, నిర్మాణం obj_బఫరు *obj_buf)
{
	నిర్మాణం object_id oid;

	ఒకవేళ (odb_write_object(the_repository->objects, obj_buf->బఫరు, obj_buf->కొలత,
			     obj->type, &oid) < 0)
		die("failed to write object %s", oid_గమ్యం_hex(&obj->oid));
	obj->flags |= FLAG_WRITTEN;
}

/*
 * At the very end of the processing, write_rest() scans the objects
 * that have reachability requirements and calls this function.
 * Verify its reachability and validity recursively and write it out.
 */
స్థిర పూర్ణాంకం check_object(నిర్మాణం object *obj, గణన object_type type,
			శూన్య *దత్తాంశం వాడని,
			నిర్మాణం fsck_options *options వాడని)
{
	నిర్మాణం obj_బఫరు *obj_buf;

	ఒకవేళ (!obj)
		తిరిగి 1;

	ఒకవేళ (obj->flags & FLAG_WRITTEN)
		తిరిగి 0;

	ఒకవేళ (type != OBJ_ANY && obj->type != type)
		die("object type mismatch");

	ఒకవేళ (!(obj->flags & FLAG_OPEN)) {
		కొలత_t కొలత;
		పూర్ణాంకం type = odb_read_object_info(the_repository->objects, &obj->oid, &కొలత);
		ఒకవేళ (type != obj->type || type <= 0)
			die("object of unexpected type");
		obj->flags |= FLAG_WRITTEN;
		తిరిగి 0;
	}

	obj_buf = lookup_object_బఫరు(obj);
	ఒకవేళ (!obj_buf)
		die("Whoops! Cannot find object '%s'", oid_గమ్యం_hex(&obj->oid));
	ఒకవేళ (fsck_object(obj, obj_buf->బఫరు, obj_buf->కొలత, &fsck_options))
		die("fsck error in packed object");
	fsck_options.walk = check_object;
	ఒకవేళ (fsck_walk(obj, ఖాళీ, &fsck_options))
		die("Error on reachable objects of %s", oid_గమ్యం_hex(&obj->oid));
	write_cached_object(obj, obj_buf);
	తిరిగి 0;
}

స్థిర శూన్య write_rest(శూన్య)
{
	గుర్తులేని i;
	కొరకు (i = 0; i < nr_objects; i++) {
		ఒకవేళ (obj_list[i].obj)
			check_object(obj_list[i].obj, OBJ_ANY, ఖాళీ, ఖాళీ);
	}
}

స్థిర శూన్య added_object(గుర్తులేని nr, గణన object_type type,
			 శూన్య *దత్తాంశం, గుర్తులేని దీర్ఘ కొలత);

/*
 * Write out nr-th object from the list, now we know the contents
 * of it.  Under --strict, this buffers structured objects in-core,
 * to be checked at the end.
 */
స్థిర శూన్య write_object(గుర్తులేని nr, గణన object_type type,
			 శూన్య *buf, గుర్తులేని దీర్ఘ కొలత)
{
	ఒకవేళ (!strict) {
		ఒకవేళ (odb_write_object(the_repository->objects, buf, కొలత, type,
				     &obj_list[nr].oid) < 0)
			die("failed to write object");
		added_object(nr, type, buf, కొలత);
		విడుదల(buf);
		obj_list[nr].obj = ఖాళీ;
	} లేకపోతే ఒకవేళ (type == OBJ_BLOB) {
		నిర్మాణం blob *blob;
		ఒకవేళ (odb_write_object(the_repository->objects, buf, కొలత, type,
				     &obj_list[nr].oid) < 0)
			die("failed to write object");
		added_object(nr, type, buf, కొలత);
		విడుదల(buf);

		blob = lookup_blob(the_repository, &obj_list[nr].oid);
		ఒకవేళ (blob)
			blob->object.flags |= FLAG_WRITTEN;
		లేకపోతే
			die("invalid blob object");
		obj_list[nr].obj = ఖాళీ;
	} లేకపోతే {
		నిర్మాణం object *obj;
		పూర్ణాంకం eaten;
		hash_object_file(the_hash_algo, buf, కొలత, type,
				 &obj_list[nr].oid);
		added_object(nr, type, buf, కొలత);
		obj = parse_object_బఫరు(the_repository, &obj_list[nr].oid,
					  type, కొలత, buf,
					  &eaten);
		ఒకవేళ (!obj)
			die("invalid %s", type_name(type));
		కూడు_object_బఫరు(obj, buf, కొలత);
		obj->flags |= FLAG_OPEN;
		obj_list[nr].obj = obj;
	}
}

స్థిర శూన్య resolve_delta(గుర్తులేని nr, గణన object_type type,
			  శూన్య *పునాది, గుర్తులేని దీర్ఘ పునాది_కొలత,
			  శూన్య *delta, గుర్తులేని దీర్ఘ delta_కొలత_2)
{
	శూన్య *ఫలితం;
	కొలత_t ఫలితం_కొలత;

	ఫలితం = patch_delta(పునాది, పునాది_కొలత,
			     delta, delta_కొలత_2,
			     &ఫలితం_కొలత);
	ఒకవేళ (!ఫలితం)
		die("failed to apply delta");
	విడుదల(delta);
	write_object(nr, type, ఫలితం, ఫలితం_కొలత);
}

/*
 * We now know the contents of an object (which is nr-th in the pack);
 * resolve all the deltified objects that are based on it.
 */
స్థిర శూన్య added_object(గుర్తులేని nr, గణన object_type type,
			 శూన్య *దత్తాంశం, గుర్తులేని దీర్ఘ కొలత)
{
	నిర్మాణం delta_info **p = &delta_list;
	నిర్మాణం delta_info *info;

	అయితే ((info = *p) != ఖాళీ) {
		ఒకవేళ (oideq(&info->పునాది_oid, &obj_list[nr].oid) ||
		    info->పునాది_స్థానభ్రంశం == obj_list[nr].స్థానభ్రంశం) {
			*p = info->తదుపరి;
			p = &delta_list;
			resolve_delta(info->nr, type, దత్తాంశం, కొలత,
				      info->delta, info->కొలత);
			విడుదల(info);
			కొనసాగు;
		}
		p = &info->తదుపరి;
	}
}

స్థిర శూన్య unpack_non_delta_నమోదు(గణన object_type type, గుర్తులేని దీర్ఘ కొలత,
				   గుర్తులేని nr)
{
	శూన్య *buf = get_దత్తాంశం(కొలత);

	ఒకవేళ (buf)
		write_object(nr, type, buf, కొలత);
}

నిర్మాణం zlib_ప్రవాహం {
	నిర్మాణం odb_ప్రవాహం పునాది;
	git_zstream *zstream;
	పూర్ణాంకం స్థితి_2;
};

స్థిర ssize_t zlib_ప్రవాహం_read(నిర్మాణం odb_ప్రవాహం *లోపల_ప్రవాహం,
				అక్షరం *buf, కొలత_t buf_len)
{
	నిర్మాణం zlib_ప్రవాహం *దత్తాంశం = container_యొక్క(లోపల_ప్రవాహం, నిర్మాణం zlib_ప్రవాహం, పునాది);
	git_zstream *zstream = దత్తాంశం->zstream;

	ఒకవేళ (దత్తాంశం->స్థితి_2 != Z_OK)
		తిరిగి 0;

	zstream->తదుపరి_బహిర్గతం = (గుర్తులేని అక్షరం *) buf;
	zstream->avail_బహిర్గతం = buf_len;

	అయితే (దత్తాంశం->స్థితి_2 == Z_OK && zstream->avail_బహిర్గతం == buf_len) {
		zstream->తదుపరి_లోపల = fill(1);
		zstream->avail_లోపల = len;
		దత్తాంశం->స్థితి_2 = git_inflate(zstream, 0);
		use(len - zstream->avail_లోపల);
	}

	తిరిగి buf_len - zstream->avail_బహిర్గతం;
}

స్థిర శూన్య ప్రవాహం_blob(గుర్తులేని దీర్ఘ కొలత, గుర్తులేని nr)
{
	git_zstream zstream = { 0 };
	నిర్మాణం zlib_ప్రవాహం లోపల_ప్రవాహం = {
		.పునాది = {
			.పఠ = zlib_ప్రవాహం_read,
			.కొలత = కొలత,
			.type = OBJ_BLOB,
		},
		.zstream = &zstream,
		.స్థితి_2 = Z_OK,
	};
	నిర్మాణం obj_info *info = &obj_list[nr];

	git_inflate_init(&zstream);

	ఒకవేళ (odb_write_object_ప్రవాహం(the_repository->objects, &లోపల_ప్రవాహం.పునాది, &info->oid))
		die(_("failed to write object in stream"));

	ఒకవేళ (లోపల_ప్రవాహం.స్థితి_2 != Z_ప్రవాహం_చివర)
		die(_("inflate returned (%d)"), లోపల_ప్రవాహం.స్థితి_2);
	git_inflate_చివర(&zstream);

	ఒకవేళ (strict) {
		నిర్మాణం blob *blob = lookup_blob(the_repository, &info->oid);

		ఒకవేళ (!blob)
			die(_("invalid blob object from stream"));
		blob->object.flags |= FLAG_WRITTEN;
	}
	info->obj = ఖాళీ;
}

స్థిర పూర్ణాంకం resolve_against_held(గుర్తులేని nr, స్థిరరాశి నిర్మాణం object_id *పునాది,
				శూన్య *delta_దత్తాంశం, గుర్తులేని దీర్ఘ delta_కొలత_2)
{
	నిర్మాణం object *obj;
	నిర్మాణం obj_బఫరు *obj_బఫరు;
	obj = lookup_object(the_repository, పునాది);
	ఒకవేళ (!obj)
		తిరిగి 0;
	obj_బఫరు = lookup_object_బఫరు(obj);
	ఒకవేళ (!obj_బఫరు)
		తిరిగి 0;
	resolve_delta(nr, obj->type, obj_బఫరు->బఫరు,
		      obj_బఫరు->కొలత, delta_దత్తాంశం, delta_కొలత_2);
	తిరిగి 1;
}

స్థిర శూన్య unpack_delta_నమోదు(గణన object_type type, గుర్తులేని దీర్ఘ delta_కొలత_2,
			       గుర్తులేని nr)
{
	శూన్య *delta_దత్తాంశం, *పునాది;
	గుర్తులేని దీర్ఘ పునాది_కొలత;
	కొలత_t పునాది_కొలత_st = 0;
	నిర్మాణం object_id పునాది_oid;

	ఒకవేళ (type == OBJ_REF_DELTA) {
		oidread(&పునాది_oid, fill(the_hash_algo->rawsz), the_repository->hash_algo);
		use(the_hash_algo->rawsz);
		delta_దత్తాంశం = get_దత్తాంశం(delta_కొలత_2);
		ఒకవేళ (!delta_దత్తాంశం)
			తిరిగి;
		ఒకవేళ (odb_has_object(the_repository->objects, &పునాది_oid,
				   ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR))
			; /* Ok we have this one */
		లేకపోతే ఒకవేళ (resolve_against_held(nr, &పునాది_oid,
					      delta_దత్తాంశం, delta_కొలత_2))
			తిరిగి; /* we are done */
		లేకపోతే {
			/* cannot resolve yet --- queue it */
			oidclr(&obj_list[nr].oid, the_repository->hash_algo);
			కూడు_delta_గమ్యం_list(nr, &పునాది_oid, 0, delta_దత్తాంశం, delta_కొలత_2);
			తిరిగి;
		}
	} లేకపోతే {
		గుర్తులేని పునాది_దొరికింది = 0;
		గుర్తులేని అక్షరం *pack, c;
		off_t పునాది_స్థానభ్రంశం;
		గుర్తులేని lo, mid, hi;

		pack = fill(1);
		c = *pack;
		use(1);
		పునాది_స్థానభ్రంశం = c & 127;
		అయితే (c & 128) {
			పునాది_స్థానభ్రంశం += 1;
			ఒకవేళ (!పునాది_స్థానభ్రంశం || MSB(పునాది_స్థానభ్రంశం, 7))
				die("offset value overflow for delta base object");
			pack = fill(1);
			c = *pack;
			use(1);
			పునాది_స్థానభ్రంశం = (పునాది_స్థానభ్రంశం << 7) + (c & 127);
		}
		పునాది_స్థానభ్రంశం = obj_list[nr].స్థానభ్రంశం - పునాది_స్థానభ్రంశం;
		ఒకవేళ (పునాది_స్థానభ్రంశం <= 0 || పునాది_స్థానభ్రంశం >= obj_list[nr].స్థానభ్రంశం)
			die("offset value out of bound for delta base object");

		delta_దత్తాంశం = get_దత్తాంశం(delta_కొలత_2);
		ఒకవేళ (!delta_దత్తాంశం)
			తిరిగి;
		lo = 0;
		hi = nr;
		అయితే (lo < hi) {
			mid = lo + (hi - lo) / 2;
			ఒకవేళ (పునాది_స్థానభ్రంశం < obj_list[mid].స్థానభ్రంశం) {
				hi = mid;
			} లేకపోతే ఒకవేళ (పునాది_స్థానభ్రంశం > obj_list[mid].స్థానభ్రంశం) {
				lo = mid + 1;
			} లేకపోతే {
				oidcpy(&పునాది_oid, &obj_list[mid].oid);
				పునాది_దొరికింది = !is_null_oid(&పునాది_oid);
				ఆపు;
			}
		}
		ఒకవేళ (!పునాది_దొరికింది) {
			/*
			 * The delta base object is itself a delta that
			 * has not been resolved yet.
			 */
			oidclr(&obj_list[nr].oid, the_repository->hash_algo);
			కూడు_delta_గమ్యం_list(nr, null_oid(the_hash_algo), పునాది_స్థానభ్రంశం,
					  delta_దత్తాంశం, delta_కొలత_2);
			తిరిగి;
		}
	}

	ఒకవేళ (resolve_against_held(nr, &పునాది_oid, delta_దత్తాంశం, delta_కొలత_2))
		తిరిగి;

	పునాది = odb_read_object(the_repository->objects, &పునాది_oid,
			       &type, &పునాది_కొలత_st);
	పునాది_కొలత = cast_కొలత_t_గమ్యం_ulong(పునాది_కొలత_st);
	ఒకవేళ (!పునాది) {
		error("failed to read delta-pack base object %s",
		      oid_గమ్యం_hex(&పునాది_oid));
		ఒకవేళ (!recover)
			నిష్క్రమించు(1);
		has_errors = 1;
		తిరిగి;
	}
	resolve_delta(nr, type, పునాది, పునాది_కొలత, delta_దత్తాంశం, delta_కొలత_2);
	విడుదల(పునాది);
}

స్థిర శూన్య unpack_one(గుర్తులేని nr)
{
	గుర్తులేని జరుపు;
	గుర్తులేని అక్షరం *pack;
	కొలత_t కొలత, c;
	గణన object_type type;

	obj_list[nr].స్థానభ్రంశం = consumed_బైట్లు;

	pack = fill(1);
	c = *pack;
	use(1);
	type = (c >> 4) & 7;
	కొలత = (c & 15);
	జరుపు = 4;
	అయితే (c & 0x80) {
		ఒకవేళ ((bitsizeof(కొలత_t) - 7) < జరుపు)
			die(_("object size too large for this platform"));
		pack = fill(1);
		c = *pack;
		use(1);
		కొలత += (c & 0x7f) << జరుపు;
		జరుపు += 7;
	}

	ఎంపిక (type) {
	స్థితి OBJ_BLOB:
		ఒకవేళ (!dry_నిరంతరం &&
		    కొలత > repo_settings_get_big_file_పరిమితి(the_repository)) {
			ప్రవాహం_blob(కొలత, nr);
			తిరిగి;
		}
		/* fallthrough */
	స్థితి OBJ_COMMIT:
	స్థితి OBJ_వృక్షం:
	స్థితి OBJ_ట్యాగ్:
		unpack_non_delta_నమోదు(type, కొలత, nr);
		తిరిగి;
	స్థితి OBJ_REF_DELTA:
	స్థితి OBJ_OFS_DELTA:
		unpack_delta_నమోదు(type, కొలత, nr);
		తిరిగి;
	అప్రమేయ:
		error("bad object type %d", type);
		has_errors = 1;
		ఒకవేళ (recover)
			తిరిగి;
		నిష్క్రమించు(1);
	}
}

స్థిర శూన్య unpack_all(శూన్య)
{
	పూర్ణాంకం i;
	గుర్తులేని అక్షరం *hdr = fill(పరిమాణం(నిర్మాణం pack_శీర్షిక));
	నిర్మాణం odb_transaction *transaction;

	ఒకవేళ (get_be32(hdr) != PACK_SIGNATURE)
		die("bad pack file");
	hdr += 4;
	ఒకవేళ (!pack_సంస్కరణ_ok_native(get_be32(hdr)))
		die("unknown pack file version %"PRIu32,
		    get_be32(hdr));
	hdr += 4;
	nr_objects = get_be32(hdr);
	use(పరిమాణం(నిర్మాణం pack_శీర్షిక));

	ఒకవేళ (!quiet)
		పురోగతి = ప్రారంభం_పురోగతి(the_repository,
					  _("Unpacking objects"), nr_objects);
	CALLOC_ARRAY(obj_list, nr_objects);
	odb_transaction_begin_or_die(the_repository->objects, &transaction, 0);
	కొరకు (i = 0; i < nr_objects; i++) {
		unpack_one(i);
		display_పురోగతి(పురోగతి, i + 1);
	}
	odb_transaction_commit_and_finalize_or_die(transaction);
	stop_పురోగతి(&పురోగతి);

	ఒకవేళ (delta_list)
		die("unresolved deltas left after unpacking");
}

పూర్ణాంకం cmd_unpack_objects(పూర్ణాంకం argc,
		       స్థిరరాశి అక్షరం **argv,
		       స్థిరరాశి అక్షరం *ఉపసర్గ వాడని,
		       నిర్మాణం repository *repo)
{
	పూర్ణాంకం i;
	నిర్మాణం object_id oid;
	నిర్మాణం git_hash_ctx tmp_ctx;

	disable_replace_refs();

	repo_config(the_repository, git_default_config, ఖాళీ);

	quiet = !isatty(2);

	show_usage_if_asked(argc, argv, unpack_usage);

	fsck_options_init(&fsck_options, repo, FSCK_OPTIONS_STRICT);

	కొరకు (i = 1 ; i < argc; i++) {
		స్థిరరాశి అక్షరం *arg = argv[i];

		ఒకవేళ (*arg == '-') {
			ఒకవేళ (!strcmp(arg, "-n")) {
				dry_నిరంతరం = 1;
				కొనసాగు;
			}
			ఒకవేళ (!strcmp(arg, "-q")) {
				quiet = 1;
				కొనసాగు;
			}
			ఒకవేళ (!strcmp(arg, "-r")) {
				recover = 1;
				కొనసాగు;
			}
			ఒకవేళ (!strcmp(arg, "--strict")) {
				strict = 1;
				కొనసాగు;
			}
			ఒకవేళ (skip_ఉపసర్గ(arg, "--strict=", &arg)) {
				strict = 1;
				fsck_సమితి_సందేశం_types(&fsck_options, arg);
				కొనసాగు;
			}
			ఒకవేళ (skip_ఉపసర్గ(arg, "--pack_header=", &arg)) {
				ఒకవేళ (parse_pack_శీర్షిక_option(arg,
							     బఫరు, &len) < 0)
					die(_("bad --pack_header: %s"), arg);
				కొనసాగు;
			}
			ఒకవేళ (skip_ఉపసర్గ(arg, "--max-input-size=", &arg)) {
				max_ఆదానం_కొలత = strtoumax(arg, ఖాళీ, 10);
				కొనసాగు;
			}
			usage(unpack_usage);
		}

		/* We don't take any non-flag arguments now.. Maybe some day */
		usage(unpack_usage);
	}
	git_hash_init(&ctx, the_hash_algo);
	unpack_all();
	git_hash_update(&ctx, బఫరు, స్థానభ్రంశం);
	git_hash_init(&tmp_ctx, the_hash_algo);
	git_hash_clone(&tmp_ctx, &ctx);
	git_hash_final_oid(&oid, &tmp_ctx);
	ఒకవేళ (strict) {
		write_rest();
		ఒకవేళ (fsck_ముగింపు(&fsck_options))
			die(_("fsck error in pack objects"));
	}
	ఒకవేళ (!hasheq(fill(the_hash_algo->rawsz), oid.hash,
		    the_repository->hash_algo))
		die("final sha1 did not match");
	use(the_hash_algo->rawsz);

	/* Write the last part of the buffer to stdout */
	write_లోపల_నిండిన(1, బఫరు + స్థానభ్రంశం, len);

	/* All done */
	తిరిగి has_errors;
}
