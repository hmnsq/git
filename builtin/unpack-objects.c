#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE
#ವ್ಯಾಖ್ಯಾನಿಸು DISABLE_SIGN_COMPARE_WARNINGS

#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "git-zlib.h"
#ಅನ್ತರ್ಭಾವ "hex.h"
#ಅನ್ತರ್ಭಾವ "object-file.h"
#ಅನ್ತರ್ಭಾವ "odb.h"
#ಅನ್ತರ್ಭಾವ "odb/streaming.h"
#ಅನ್ತರ್ಭಾವ "odb/transaction.h"
#ಅನ್ತರ್ಭಾವ "object.h"
#ಅನ್ತರ್ಭಾವ "delta.h"
#ಅನ್ತರ್ಭಾವ "pack.h"
#ಅನ್ತರ್ಭಾವ "blob.h"
#ಅನ್ತರ್ಭಾವ "replace-object.h"
#ಅನ್ತರ್ಭಾವ "strbuf.h"
#ಅನ್ತರ್ಭಾವ "progress.h"
#ಅನ್ತರ್ಭಾವ "decorate.h"
#ಅನ್ತರ್ಭಾವ "fsck.h"
#ಅನ್ತರ್ಭಾವ "packfile.h"

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ dry_ಸತತ, quiet, recover, has_errors, strict;
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ unpack_usage[] = "git unpack-objects [-n] [-q] [-r] [--strict]";

ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ ಬಫರ್[DEFAULT_IO_ಬಫರ್_ಗಾತ್ರ];
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಸ್ಥಾನಾಂತರ, len;
ಸ್ಥಿರ off_t consumed_ಬೈಟುಗಳು;
ಸ್ಥಿರ off_t max_ಒಳಹರಿವು_ಗಾತ್ರ;
ಸ್ಥಿರ ರಚನೆ git_hash_ctx ctx;
ಸ್ಥಿರ ರಚನೆ fsck_options fsck_options;
ಸ್ಥಿರ ರಚನೆ ಪ್ರಗತಿ *ಪ್ರಗತಿ;

/*
 * When running under --strict mode, objects whose reachability are
 * suspect are kept in core without getting written in the object
 * store.
 */
ರಚನೆ obj_ಬಫರ್ {
	ಅಕ್ಷರ *ಬಫರ್;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಗಾತ್ರ_2;
};

ಸ್ಥಿರ ರಚನೆ decoration obj_decorate;

ಸ್ಥಿರ ರಚನೆ obj_ಬಫರ್ *lookup_object_ಬಫರ್(ರಚನೆ object *ತಳ)
{
	ಹಿಂತಿರುಗಿ lookup_decoration(&obj_decorate, ತಳ);
}

ಸ್ಥಿರ ಶೂನ್ಯ ಕೂಡಿಸು_object_ಬಫರ್(ರಚನೆ object *object, ಅಕ್ಷರ *ಬಫರ್, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಗಾತ್ರ_2)
{
	ರಚನೆ obj_ಬಫರ್ *obj;
	CALLOC_ARRAY(obj, 1);
	obj->ಬಫರ್ = ಬಫರ್;
	obj->ಗಾತ್ರ_2 = ಗಾತ್ರ_2;
	ಆದರೆ (ಕೂಡಿಸು_decoration(&obj_decorate, object, obj))
		die("object %s tried to add buffer twice!", oid_ಗಮ್ಯ_hex(&object->oid));
}

/*
 * Make sure at least "min" bytes are available in the buffer, and
 * return the pointer to the buffer.
 */
ಸ್ಥಿರ ಶೂನ್ಯ *fill(ಪೂರ್ಣಾಂಕ min)
{
	ಆದರೆ (min <= len)
		ಹಿಂತಿರುಗಿ ಬಫರ್ + ಸ್ಥಾನಾಂತರ;
	ಆದರೆ (min > ಗಾತ್ರ(ಬಫರ್))
		die("cannot fill %d bytes", min);
	ಆದರೆ (ಸ್ಥಾನಾಂತರ) {
		git_hash_update(&ctx, ಬಫರ್, ಸ್ಥಾನಾಂತರ);
		memmove(ಬಫರ್, ಬಫರ್ + ಸ್ಥಾನಾಂತರ, len);
		ಸ್ಥಾನಾಂತರ = 0;
	}
	ಮಾಡು {
		ssize_t ret = xread(0, ಬಫರ್ + len, ಗಾತ್ರ(ಬಫರ್) - len);
		ಆದರೆ (ret <= 0) {
			ಆದರೆ (!ret)
				die("early EOF");
			die_errno("read error on input");
		}
		len += ret;
	} ಆಗ (len < min);
	ಹಿಂತಿರುಗಿ ಬಫರ್;
}

ಸ್ಥಿರ ಶೂನ್ಯ use(ಪೂರ್ಣಾಂಕ ಬೈಟುಗಳು)
{
	ಆದರೆ (ಬೈಟುಗಳು > len)
		die("used more bytes than were available");
	len -= ಬೈಟುಗಳು;
	ಸ್ಥಾನಾಂತರ += ಬೈಟುಗಳು;

	/* make sure off_t is sufficiently large not to wrap */
	ಆದರೆ (signed_ಕೂಡಿಸು_overflows(consumed_ಬೈಟುಗಳು, ಬೈಟುಗಳು))
		die("pack too large for current definition of off_t");
	consumed_ಬೈಟುಗಳು += ಬೈಟುಗಳು;
	ಆದರೆ (max_ಒಳಹರಿವು_ಗಾತ್ರ && consumed_ಬೈಟುಗಳು > max_ಒಳಹರಿವು_ಗಾತ್ರ)
		die(_("pack exceeds maximum allowed size"));
	display_throughput(ಪ್ರಗತಿ, consumed_ಬೈಟುಗಳು);
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
ಸ್ಥಿರ ಶೂನ್ಯ *get_ದತ್ತಾಂಶ(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಗಾತ್ರ_2)
{
	git_zstream ಹರಿವು;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ bufsize = dry_ಸತತ && ಗಾತ್ರ_2 > 8192 ? 8192 : ಗಾತ್ರ_2;
	ಶೂನ್ಯ *buf = xmallocz(bufsize);

	memset(&ಹರಿವು, 0, ಗಾತ್ರ(ಹರಿವು));

	ಹರಿವು.ಮುಂದಿನ_ಹೊರಹೋಗು = buf;
	ಹರಿವು.avail_ಹೊರಹೋಗು = bufsize;
	ಹರಿವು.ಮುಂದಿನ_ಒಳ = fill(1);
	ಹರಿವು.avail_ಒಳ = len;
	git_inflate_init(&ಹರಿವು);

	ಗಾಗಿ (;;) {
		ಪೂರ್ಣಾಂಕ ret = git_inflate(&ಹರಿವು, 0);
		use(len - ಹರಿವು.avail_ಒಳ);
		ಆದರೆ (ಹರಿವು.ಒಟ್ಟು_ಹೊರಹೋಗು == ಗಾತ್ರ_2 && ret == Z_ಹರಿವು_ಕೊನೆ)
			ನಿಲ್ಲಿಸು;
		ಆದರೆ (ret != Z_OK) {
			error("inflate returned %d", ret);
			FREE_AND_NULL(buf);
			ಆದರೆ (!recover)
				ನಿರ್ಗಮಿಸು(1);
			has_errors = 1;
			ನಿಲ್ಲಿಸು;
		}
		ಹರಿವು.ಮುಂದಿನ_ಒಳ = fill(1);
		ಹರಿವು.avail_ಒಳ = len;
		ಆದರೆ (dry_ಸತತ) {
			/* reuse the buffer in dry_run mode */
			ಹರಿವು.ಮುಂದಿನ_ಹೊರಹೋಗು = buf;
			ಹರಿವು.avail_ಹೊರಹೋಗು = bufsize > ಗಾತ್ರ_2 - ಹರಿವು.ಒಟ್ಟು_ಹೊರಹೋಗು ?
						   ಗಾತ್ರ_2 - ಹರಿವು.ಒಟ್ಟು_ಹೊರಹೋಗು :
						   bufsize;
		}
	}
	git_inflate_ಕೊನೆ(&ಹರಿವು);
	ಆದರೆ (dry_ಸತತ)
		FREE_AND_NULL(buf);
	ಹಿಂತಿರುಗಿ buf;
}

ರಚನೆ delta_info {
	ರಚನೆ object_id ತಳ_oid;
	ಚಿಹ್ನೆರಹಿತ nr;
	off_t ತಳ_ಸ್ಥಾನಾಂತರ;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಗಾತ್ರ_2;
	ಶೂನ್ಯ *delta;
	ರಚನೆ delta_info *ಮುಂದಿನ;
};

ಸ್ಥಿರ ರಚನೆ delta_info *delta_list;

ಸ್ಥಿರ ಶೂನ್ಯ ಕೂಡಿಸು_delta_ಗಮ್ಯ_list(ಚಿಹ್ನೆರಹಿತ nr, ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *ತಳ_oid,
			      off_t ತಳ_ಸ್ಥಾನಾಂತರ,
			      ಶೂನ್ಯ *delta, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಗಾತ್ರ_2)
{
	ರಚನೆ delta_info *info = xmalloc(ಗಾತ್ರ(*info));

	oidcpy(&info->ತಳ_oid, ತಳ_oid);
	info->ತಳ_ಸ್ಥಾನಾಂತರ = ತಳ_ಸ್ಥಾನಾಂತರ;
	info->ಗಾತ್ರ_2 = ಗಾತ್ರ_2;
	info->delta = delta;
	info->nr = nr;
	info->ಮುಂದಿನ = delta_list;
	delta_list = info;
}

ರಚನೆ obj_info {
	off_t ಸ್ಥಾನಾಂತರ;
	ರಚನೆ object_id oid;
	ರಚನೆ object *obj;
};

/* Remember to update object flag allocation in object.h */
#ವ್ಯಾಖ್ಯಾನಿಸು FLAG_OPEN (1u<<20)
#ವ್ಯಾಖ್ಯಾನಿಸು FLAG_WRITTEN (1u<<21)

ಸ್ಥಿರ ರಚನೆ obj_info *obj_list;
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ nr_objects;

/*
 * Called only from check_object() after it verified this object
 * is Ok.
 */
ಸ್ಥಿರ ಶೂನ್ಯ write_cached_object(ರಚನೆ object *obj, ರಚನೆ obj_ಬಫರ್ *obj_buf)
{
	ರಚನೆ object_id oid;

	ಆದರೆ (odb_write_object(the_repository->objects, obj_buf->ಬಫರ್, obj_buf->ಗಾತ್ರ_2,
			     obj->type, &oid) < 0)
		die("failed to write object %s", oid_ಗಮ್ಯ_hex(&obj->oid));
	obj->flags |= FLAG_WRITTEN;
}

/*
 * At the very end of the processing, write_rest() scans the objects
 * that have reachability requirements and calls this function.
 * Verify its reachability and validity recursively and write it out.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ check_object(ರಚನೆ object *obj, ಎಣಿಕೆ object_type type,
			ಶೂನ್ಯ *ದತ್ತಾಂಶ ಬಳಸದ,
			ರಚನೆ fsck_options *options ಬಳಸದ)
{
	ರಚನೆ obj_ಬಫರ್ *obj_buf;

	ಆದರೆ (!obj)
		ಹಿಂತಿರುಗಿ 1;

	ಆದರೆ (obj->flags & FLAG_WRITTEN)
		ಹಿಂತಿರುಗಿ 0;

	ಆದರೆ (type != OBJ_ANY && obj->type != type)
		die("object type mismatch");

	ಆದರೆ (!(obj->flags & FLAG_OPEN)) {
		ಗಾತ್ರ_t ಗಾತ್ರ_2;
		ಪೂರ್ಣಾಂಕ type = odb_read_object_info(the_repository->objects, &obj->oid, &ಗಾತ್ರ_2);
		ಆದರೆ (type != obj->type || type <= 0)
			die("object of unexpected type");
		obj->flags |= FLAG_WRITTEN;
		ಹಿಂತಿರುಗಿ 0;
	}

	obj_buf = lookup_object_ಬಫರ್(obj);
	ಆದರೆ (!obj_buf)
		die("Whoops! Cannot find object '%s'", oid_ಗಮ್ಯ_hex(&obj->oid));
	ಆದರೆ (fsck_object(obj, obj_buf->ಬಫರ್, obj_buf->ಗಾತ್ರ_2, &fsck_options))
		die("fsck error in packed object");
	fsck_options.walk = check_object;
	ಆದರೆ (fsck_walk(obj, ಖಾಲಿ, &fsck_options))
		die("Error on reachable objects of %s", oid_ಗಮ್ಯ_hex(&obj->oid));
	write_cached_object(obj, obj_buf);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ write_rest(ಶೂನ್ಯ)
{
	ಚಿಹ್ನೆರಹಿತ i;
	ಗಾಗಿ (i = 0; i < nr_objects; i++) {
		ಆದರೆ (obj_list[i].obj)
			check_object(obj_list[i].obj, OBJ_ANY, ಖಾಲಿ, ಖಾಲಿ);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ added_object(ಚಿಹ್ನೆರಹಿತ nr, ಎಣಿಕೆ object_type type,
			 ಶೂನ್ಯ *ದತ್ತಾಂಶ, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಗಾತ್ರ_2);

/*
 * Write out nr-th object from the list, now we know the contents
 * of it.  Under --strict, this buffers structured objects in-core,
 * to be checked at the end.
 */
ಸ್ಥಿರ ಶೂನ್ಯ write_object(ಚಿಹ್ನೆರಹಿತ nr, ಎಣಿಕೆ object_type type,
			 ಶೂನ್ಯ *buf, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಗಾತ್ರ_2)
{
	ಆದರೆ (!strict) {
		ಆದರೆ (odb_write_object(the_repository->objects, buf, ಗಾತ್ರ_2, type,
				     &obj_list[nr].oid) < 0)
			die("failed to write object");
		added_object(nr, type, buf, ಗಾತ್ರ_2);
		ಬಿಡುಗಡೆ(buf);
		obj_list[nr].obj = ಖಾಲಿ;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (type == OBJ_BLOB) {
		ರಚನೆ blob *blob;
		ಆದರೆ (odb_write_object(the_repository->objects, buf, ಗಾತ್ರ_2, type,
				     &obj_list[nr].oid) < 0)
			die("failed to write object");
		added_object(nr, type, buf, ಗಾತ್ರ_2);
		ಬಿಡುಗಡೆ(buf);

		blob = lookup_blob(the_repository, &obj_list[nr].oid);
		ಆದರೆ (blob)
			blob->object.flags |= FLAG_WRITTEN;
		ಇಲ್ಲದಿದ್ದರೆ
			die("invalid blob object");
		obj_list[nr].obj = ಖಾಲಿ;
	} ಇಲ್ಲದಿದ್ದರೆ {
		ರಚನೆ object *obj;
		ಪೂರ್ಣಾಂಕ eaten;
		hash_object_file(the_hash_algo, buf, ಗಾತ್ರ_2, type,
				 &obj_list[nr].oid);
		added_object(nr, type, buf, ಗಾತ್ರ_2);
		obj = parse_object_ಬಫರ್(the_repository, &obj_list[nr].oid,
					  type, ಗಾತ್ರ_2, buf,
					  &eaten);
		ಆದರೆ (!obj)
			die("invalid %s", type_name(type));
		ಕೂಡಿಸು_object_ಬಫರ್(obj, buf, ಗಾತ್ರ_2);
		obj->flags |= FLAG_OPEN;
		obj_list[nr].obj = obj;
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ resolve_delta(ಚಿಹ್ನೆರಹಿತ nr, ಎಣಿಕೆ object_type type,
			  ಶೂನ್ಯ *ತಳ, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ತಳ_ಗಾತ್ರ,
			  ಶೂನ್ಯ *delta, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ delta_ಗಾತ್ರ_2)
{
	ಶೂನ್ಯ *ಫಲಿತಾಂಶ;
	ಗಾತ್ರ_t ಫಲಿತಾಂಶ_ಗಾತ್ರ;

	ಫಲಿತಾಂಶ = patch_delta(ತಳ, ತಳ_ಗಾತ್ರ,
			     delta, delta_ಗಾತ್ರ_2,
			     &ಫಲಿತಾಂಶ_ಗಾತ್ರ);
	ಆದರೆ (!ಫಲಿತಾಂಶ)
		die("failed to apply delta");
	ಬಿಡುಗಡೆ(delta);
	write_object(nr, type, ಫಲಿತಾಂಶ, ಫಲಿತಾಂಶ_ಗಾತ್ರ);
}

/*
 * We now know the contents of an object (which is nr-th in the pack);
 * resolve all the deltified objects that are based on it.
 */
ಸ್ಥಿರ ಶೂನ್ಯ added_object(ಚಿಹ್ನೆರಹಿತ nr, ಎಣಿಕೆ object_type type,
			 ಶೂನ್ಯ *ದತ್ತಾಂಶ, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಗಾತ್ರ_2)
{
	ರಚನೆ delta_info **p = &delta_list;
	ರಚನೆ delta_info *info;

	ಆಗ ((info = *p) != ಖಾಲಿ) {
		ಆದರೆ (oideq(&info->ತಳ_oid, &obj_list[nr].oid) ||
		    info->ತಳ_ಸ್ಥಾನಾಂತರ == obj_list[nr].ಸ್ಥಾನಾಂತರ) {
			*p = info->ಮುಂದಿನ;
			p = &delta_list;
			resolve_delta(info->nr, type, ದತ್ತಾಂಶ, ಗಾತ್ರ_2,
				      info->delta, info->ಗಾತ್ರ_2);
			ಬಿಡುಗಡೆ(info);
			ಮುಂದುವರಿ;
		}
		p = &info->ಮುಂದಿನ;
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ unpack_non_delta_ನಮೂದು(ಎಣಿಕೆ object_type type, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಗಾತ್ರ_2,
				   ಚಿಹ್ನೆರಹಿತ nr)
{
	ಶೂನ್ಯ *buf = get_ದತ್ತಾಂಶ(ಗಾತ್ರ_2);

	ಆದರೆ (buf)
		write_object(nr, type, buf, ಗಾತ್ರ_2);
}

ರಚನೆ zlib_ಹರಿವು {
	ರಚನೆ odb_ಹರಿವು ತಳ;
	git_zstream *zstream;
	ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2;
};

ಸ್ಥಿರ ssize_t zlib_ಹರಿವು_read(ರಚನೆ odb_ಹರಿವು *ಒಳ_ಹರಿವು,
				ಅಕ್ಷರ *buf, ಗಾತ್ರ_t buf_len)
{
	ರಚನೆ zlib_ಹರಿವು *ದತ್ತಾಂಶ = container_ದ(ಒಳ_ಹರಿವು, ರಚನೆ zlib_ಹರಿವು, ತಳ);
	git_zstream *zstream = ದತ್ತಾಂಶ->zstream;

	ಆದರೆ (ದತ್ತಾಂಶ->ಸ್ಥಿತಿ_2 != Z_OK)
		ಹಿಂತಿರುಗಿ 0;

	zstream->ಮುಂದಿನ_ಹೊರಹೋಗು = (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *) buf;
	zstream->avail_ಹೊರಹೋಗು = buf_len;

	ಆಗ (ದತ್ತಾಂಶ->ಸ್ಥಿತಿ_2 == Z_OK && zstream->avail_ಹೊರಹೋಗು == buf_len) {
		zstream->ಮುಂದಿನ_ಒಳ = fill(1);
		zstream->avail_ಒಳ = len;
		ದತ್ತಾಂಶ->ಸ್ಥಿತಿ_2 = git_inflate(zstream, 0);
		use(len - zstream->avail_ಒಳ);
	}

	ಹಿಂತಿರುಗಿ buf_len - zstream->avail_ಹೊರಹೋಗು;
}

ಸ್ಥಿರ ಶೂನ್ಯ ಹರಿವು_blob(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಗಾತ್ರ_2, ಚಿಹ್ನೆರಹಿತ nr)
{
	git_zstream zstream = { 0 };
	ರಚನೆ zlib_ಹರಿವು ಒಳ_ಹರಿವು = {
		.ತಳ = {
			.ಪಠ = zlib_ಹರಿವು_read,
			.ಗಾತ್ರ_2 = ಗಾತ್ರ_2,
			.type = OBJ_BLOB,
		},
		.zstream = &zstream,
		.ಸ್ಥಿತಿ_2 = Z_OK,
	};
	ರಚನೆ obj_info *info = &obj_list[nr];

	git_inflate_init(&zstream);

	ಆದರೆ (odb_write_object_ಹರಿವು(the_repository->objects, &ಒಳ_ಹರಿವು.ತಳ, &info->oid))
		die(_("failed to write object in stream"));

	ಆದರೆ (ಒಳ_ಹರಿವು.ಸ್ಥಿತಿ_2 != Z_ಹರಿವು_ಕೊನೆ)
		die(_("inflate returned (%d)"), ಒಳ_ಹರಿವು.ಸ್ಥಿತಿ_2);
	git_inflate_ಕೊನೆ(&zstream);

	ಆದರೆ (strict) {
		ರಚನೆ blob *blob = lookup_blob(the_repository, &info->oid);

		ಆದರೆ (!blob)
			die(_("invalid blob object from stream"));
		blob->object.flags |= FLAG_WRITTEN;
	}
	info->obj = ಖಾಲಿ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ resolve_against_held(ಚಿಹ್ನೆರಹಿತ nr, ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *ತಳ,
				ಶೂನ್ಯ *delta_ದತ್ತಾಂಶ, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ delta_ಗಾತ್ರ_2)
{
	ರಚನೆ object *obj;
	ರಚನೆ obj_ಬಫರ್ *obj_ಬಫರ್;
	obj = lookup_object(the_repository, ತಳ);
	ಆದರೆ (!obj)
		ಹಿಂತಿರುಗಿ 0;
	obj_ಬಫರ್ = lookup_object_ಬಫರ್(obj);
	ಆದರೆ (!obj_ಬಫರ್)
		ಹಿಂತಿರುಗಿ 0;
	resolve_delta(nr, obj->type, obj_ಬಫರ್->ಬಫರ್,
		      obj_ಬಫರ್->ಗಾತ್ರ_2, delta_ದತ್ತಾಂಶ, delta_ಗಾತ್ರ_2);
	ಹಿಂತಿರುಗಿ 1;
}

ಸ್ಥಿರ ಶೂನ್ಯ unpack_delta_ನಮೂದು(ಎಣಿಕೆ object_type type, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ delta_ಗಾತ್ರ_2,
			       ಚಿಹ್ನೆರಹಿತ nr)
{
	ಶೂನ್ಯ *delta_ದತ್ತಾಂಶ, *ತಳ;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ತಳ_ಗಾತ್ರ;
	ಗಾತ್ರ_t ತಳ_ಗಾತ್ರ_st = 0;
	ರಚನೆ object_id ತಳ_oid;

	ಆದರೆ (type == OBJ_REF_DELTA) {
		oidread(&ತಳ_oid, fill(the_hash_algo->rawsz), the_repository->hash_algo);
		use(the_hash_algo->rawsz);
		delta_ದತ್ತಾಂಶ = get_ದತ್ತಾಂಶ(delta_ಗಾತ್ರ_2);
		ಆದರೆ (!delta_ದತ್ತಾಂಶ)
			ಹಿಂತಿರುಗಿ;
		ಆದರೆ (odb_has_object(the_repository->objects, &ತಳ_oid,
				   ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR))
			; /* Ok we have this one */
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (resolve_against_held(nr, &ತಳ_oid,
					      delta_ದತ್ತಾಂಶ, delta_ಗಾತ್ರ_2))
			ಹಿಂತಿರುಗಿ; /* we are done */
		ಇಲ್ಲದಿದ್ದರೆ {
			/* cannot resolve yet --- queue it */
			oidclr(&obj_list[nr].oid, the_repository->hash_algo);
			ಕೂಡಿಸು_delta_ಗಮ್ಯ_list(nr, &ತಳ_oid, 0, delta_ದತ್ತಾಂಶ, delta_ಗಾತ್ರ_2);
			ಹಿಂತಿರುಗಿ;
		}
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಚಿಹ್ನೆರಹಿತ ತಳ_ಸಿಕ್ಕಿತು = 0;
		ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *pack, c;
		off_t ತಳ_ಸ್ಥಾನಾಂತರ;
		ಚಿಹ್ನೆರಹಿತ lo, mid, hi;

		pack = fill(1);
		c = *pack;
		use(1);
		ತಳ_ಸ್ಥಾನಾಂತರ = c & 127;
		ಆಗ (c & 128) {
			ತಳ_ಸ್ಥಾನಾಂತರ += 1;
			ಆದರೆ (!ತಳ_ಸ್ಥಾನಾಂತರ || MSB(ತಳ_ಸ್ಥಾನಾಂತರ, 7))
				die("offset value overflow for delta base object");
			pack = fill(1);
			c = *pack;
			use(1);
			ತಳ_ಸ್ಥಾನಾಂತರ = (ತಳ_ಸ್ಥಾನಾಂತರ << 7) + (c & 127);
		}
		ತಳ_ಸ್ಥಾನಾಂತರ = obj_list[nr].ಸ್ಥಾನಾಂತರ - ತಳ_ಸ್ಥಾನಾಂತರ;
		ಆದರೆ (ತಳ_ಸ್ಥಾನಾಂತರ <= 0 || ತಳ_ಸ್ಥಾನಾಂತರ >= obj_list[nr].ಸ್ಥಾನಾಂತರ)
			die("offset value out of bound for delta base object");

		delta_ದತ್ತಾಂಶ = get_ದತ್ತಾಂಶ(delta_ಗಾತ್ರ_2);
		ಆದರೆ (!delta_ದತ್ತಾಂಶ)
			ಹಿಂತಿರುಗಿ;
		lo = 0;
		hi = nr;
		ಆಗ (lo < hi) {
			mid = lo + (hi - lo) / 2;
			ಆದರೆ (ತಳ_ಸ್ಥಾನಾಂತರ < obj_list[mid].ಸ್ಥಾನಾಂತರ) {
				hi = mid;
			} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ತಳ_ಸ್ಥಾನಾಂತರ > obj_list[mid].ಸ್ಥಾನಾಂತರ) {
				lo = mid + 1;
			} ಇಲ್ಲದಿದ್ದರೆ {
				oidcpy(&ತಳ_oid, &obj_list[mid].oid);
				ತಳ_ಸಿಕ್ಕಿತು = !is_null_oid(&ತಳ_oid);
				ನಿಲ್ಲಿಸು;
			}
		}
		ಆದರೆ (!ತಳ_ಸಿಕ್ಕಿತು) {
			/*
			 * The delta base object is itself a delta that
			 * has not been resolved yet.
			 */
			oidclr(&obj_list[nr].oid, the_repository->hash_algo);
			ಕೂಡಿಸು_delta_ಗಮ್ಯ_list(nr, null_oid(the_hash_algo), ತಳ_ಸ್ಥಾನಾಂತರ,
					  delta_ದತ್ತಾಂಶ, delta_ಗಾತ್ರ_2);
			ಹಿಂತಿರುಗಿ;
		}
	}

	ಆದರೆ (resolve_against_held(nr, &ತಳ_oid, delta_ದತ್ತಾಂಶ, delta_ಗಾತ್ರ_2))
		ಹಿಂತಿರುಗಿ;

	ತಳ = odb_read_object(the_repository->objects, &ತಳ_oid,
			       &type, &ತಳ_ಗಾತ್ರ_st);
	ತಳ_ಗಾತ್ರ = cast_ಗಾತ್ರ_t_ಗಮ್ಯ_ulong(ತಳ_ಗಾತ್ರ_st);
	ಆದರೆ (!ತಳ) {
		error("failed to read delta-pack base object %s",
		      oid_ಗಮ್ಯ_hex(&ತಳ_oid));
		ಆದರೆ (!recover)
			ನಿರ್ಗಮಿಸು(1);
		has_errors = 1;
		ಹಿಂತಿರುಗಿ;
	}
	resolve_delta(nr, type, ತಳ, ತಳ_ಗಾತ್ರ, delta_ದತ್ತಾಂಶ, delta_ಗಾತ್ರ_2);
	ಬಿಡುಗಡೆ(ತಳ);
}

ಸ್ಥಿರ ಶೂನ್ಯ unpack_one(ಚಿಹ್ನೆರಹಿತ nr)
{
	ಚಿಹ್ನೆರಹಿತ ಸರಿಸು;
	ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *pack;
	ಗಾತ್ರ_t ಗಾತ್ರ_2, c;
	ಎಣಿಕೆ object_type type;

	obj_list[nr].ಸ್ಥಾನಾಂತರ = consumed_ಬೈಟುಗಳು;

	pack = fill(1);
	c = *pack;
	use(1);
	type = (c >> 4) & 7;
	ಗಾತ್ರ_2 = (c & 15);
	ಸರಿಸು = 4;
	ಆಗ (c & 0x80) {
		ಆದರೆ ((bitsizeof(ಗಾತ್ರ_t) - 7) < ಸರಿಸು)
			die(_("object size too large for this platform"));
		pack = fill(1);
		c = *pack;
		use(1);
		ಗಾತ್ರ_2 += (c & 0x7f) << ಸರಿಸು;
		ಸರಿಸು += 7;
	}

	ಆಯ್ಕೆ (type) {
	ಸ್ಥಿತಿ OBJ_BLOB:
		ಆದರೆ (!dry_ಸತತ &&
		    ಗಾತ್ರ_2 > repo_settings_get_big_file_ಮಿತಿ(the_repository)) {
			ಹರಿವು_blob(ಗಾತ್ರ_2, nr);
			ಹಿಂತಿರುಗಿ;
		}
		/* fallthrough */
	ಸ್ಥಿತಿ OBJ_COMMIT:
	ಸ್ಥಿತಿ OBJ_ವೃಕ್ಷ:
	ಸ್ಥಿತಿ OBJ_ಟ್ಯಾಗ್:
		unpack_non_delta_ನಮೂದು(type, ಗಾತ್ರ_2, nr);
		ಹಿಂತಿರುಗಿ;
	ಸ್ಥಿತಿ OBJ_REF_DELTA:
	ಸ್ಥಿತಿ OBJ_OFS_DELTA:
		unpack_delta_ನಮೂದು(type, ಗಾತ್ರ_2, nr);
		ಹಿಂತಿರುಗಿ;
	ಪೂರ್ವನಿಯೋಜಿತ:
		error("bad object type %d", type);
		has_errors = 1;
		ಆದರೆ (recover)
			ಹಿಂತಿರುಗಿ;
		ನಿರ್ಗಮಿಸು(1);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ unpack_all(ಶೂನ್ಯ)
{
	ಪೂರ್ಣಾಂಕ i;
	ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *hdr = fill(ಗಾತ್ರ(ರಚನೆ pack_ಶೀರ್ಷಿಕೆ));
	ರಚನೆ odb_transaction *transaction;

	ಆದರೆ (get_be32(hdr) != PACK_SIGNATURE)
		die("bad pack file");
	hdr += 4;
	ಆದರೆ (!pack_ಆವೃತ್ತಿ_ok_native(get_be32(hdr)))
		die("unknown pack file version %"PRIu32,
		    get_be32(hdr));
	hdr += 4;
	nr_objects = get_be32(hdr);
	use(ಗಾತ್ರ(ರಚನೆ pack_ಶೀರ್ಷಿಕೆ));

	ಆದರೆ (!quiet)
		ಪ್ರಗತಿ = ಆರಂಭ_ಪ್ರಗತಿ(the_repository,
					  _("Unpacking objects"), nr_objects);
	CALLOC_ARRAY(obj_list, nr_objects);
	odb_transaction_begin_or_die(the_repository->objects, &transaction, 0);
	ಗಾಗಿ (i = 0; i < nr_objects; i++) {
		unpack_one(i);
		display_ಪ್ರಗತಿ(ಪ್ರಗತಿ, i + 1);
	}
	odb_transaction_commit_and_finalize_or_die(transaction);
	stop_ಪ್ರಗತಿ(&ಪ್ರಗತಿ);

	ಆದರೆ (delta_list)
		die("unresolved deltas left after unpacking");
}

ಪೂರ್ಣಾಂಕ cmd_unpack_objects(ಪೂರ್ಣಾಂಕ argc,
		       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
		       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ ಬಳಸದ,
		       ರಚನೆ repository *repo)
{
	ಪೂರ್ಣಾಂಕ i;
	ರಚನೆ object_id oid;
	ರಚನೆ git_hash_ctx tmp_ctx;

	disable_replace_refs();

	repo_config(the_repository, git_default_config, ಖಾಲಿ);

	quiet = !isatty(2);

	show_usage_if_asked(argc, argv, unpack_usage);

	fsck_options_init(&fsck_options, repo, FSCK_OPTIONS_STRICT);

	ಗಾಗಿ (i = 1 ; i < argc; i++) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg = argv[i];

		ಆದರೆ (*arg == '-') {
			ಆದರೆ (!strcmp(arg, "-n")) {
				dry_ಸತತ = 1;
				ಮುಂದುವರಿ;
			}
			ಆದರೆ (!strcmp(arg, "-q")) {
				quiet = 1;
				ಮುಂದುವರಿ;
			}
			ಆದರೆ (!strcmp(arg, "-r")) {
				recover = 1;
				ಮುಂದುವರಿ;
			}
			ಆದರೆ (!strcmp(arg, "--strict")) {
				strict = 1;
				ಮುಂದುವರಿ;
			}
			ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(arg, "--strict=", &arg)) {
				strict = 1;
				fsck_ಗಣ_ಸಂದೇಶ_types(&fsck_options, arg);
				ಮುಂದುವರಿ;
			}
			ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(arg, "--pack_header=", &arg)) {
				ಆದರೆ (parse_pack_ಶೀರ್ಷಿಕೆ_option(arg,
							     ಬಫರ್, &len) < 0)
					die(_("bad --pack_header: %s"), arg);
				ಮುಂದುವರಿ;
			}
			ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(arg, "--max-input-size=", &arg)) {
				max_ಒಳಹರಿವು_ಗಾತ್ರ = strtoumax(arg, ಖಾಲಿ, 10);
				ಮುಂದುವರಿ;
			}
			usage(unpack_usage);
		}

		/* We don't take any non-flag arguments now.. Maybe some day */
		usage(unpack_usage);
	}
	git_hash_init(&ctx, the_hash_algo);
	unpack_all();
	git_hash_update(&ctx, ಬಫರ್, ಸ್ಥಾನಾಂತರ);
	git_hash_init(&tmp_ctx, the_hash_algo);
	git_hash_clone(&tmp_ctx, &ctx);
	git_hash_final_oid(&oid, &tmp_ctx);
	ಆದರೆ (strict) {
		write_rest();
		ಆದರೆ (fsck_ಮುಕ್ತಾಯ(&fsck_options))
			die(_("fsck error in pack objects"));
	}
	ಆದರೆ (!hasheq(fill(the_hash_algo->rawsz), oid.hash,
		    the_repository->hash_algo))
		die("final sha1 did not match");
	use(the_hash_algo->rawsz);

	/* Write the last part of the buffer to stdout */
	write_ಒಳ_ತುಂಬಿದ(1, ಬಫರ್ + ಸ್ಥಾನಾಂತರ, len);

	/* All done */
	ಹಿಂತಿರುಗಿ has_errors;
}
