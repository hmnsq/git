#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DISABLE_SIGN_COMPARE_WARNINGS

#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "git-zlib.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hex.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object-file.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "odb.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "odb/streaming.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "odb/transaction.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "delta.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "pack.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "blob.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "replace-object.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "strbuf.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "progress.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "decorate.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "fsck.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "packfile.h"

ਸਥਿਰ ਪੂਰਨ dry_ਲੜੀਵਾਰ, quiet, recover, has_errors, strict;
ਸਥਿਰ ਅਚਲ ਅੱਖਰ unpack_usage[] = "git unpack-objects [-n] [-q] [-r] [--strict]";

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ ਬਫਰ[DEFAULT_IO_ਬਫਰ_ਮਾਪ];
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਵਿੱਥ, len;
ਸਥਿਰ off_t consumed_ਬਾਈਟਾਂ;
ਸਥਿਰ off_t max_ਨਿਵੇਸ਼_ਮਾਪ;
ਸਥਿਰ ਬਣਤਰ git_hash_ctx ctx;
ਸਥਿਰ ਬਣਤਰ fsck_options fsck_options;
ਸਥਿਰ ਬਣਤਰ ਤਰੱਕੀ *ਤਰੱਕੀ;

/*
 * When running under --strict mode, objects whose reachability are
 * suspect are kept in core without getting written in the object
 * store.
 */
ਬਣਤਰ obj_ਬਫਰ {
	ਅੱਖਰ *ਬਫਰ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਮਾਪ;
};

ਸਥਿਰ ਬਣਤਰ decoration obj_decorate;

ਸਥਿਰ ਬਣਤਰ obj_ਬਫਰ *lookup_object_ਬਫਰ(ਬਣਤਰ object *ਅਧਾਰ)
{
	ਵਾਪਸ lookup_decoration(&obj_decorate, ਅਧਾਰ);
}

ਸਥਿਰ ਖਾਲੀ ਜੋੜੋ_object_ਬਫਰ(ਬਣਤਰ object *object, ਅੱਖਰ *ਬਫਰ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਮਾਪ)
{
	ਬਣਤਰ obj_ਬਫਰ *obj;
	CALLOC_ARRAY(obj, 1);
	obj->ਬਫਰ = ਬਫਰ;
	obj->ਮਾਪ = ਮਾਪ;
	ਜੇ (ਜੋੜੋ_decoration(&obj_decorate, object, obj))
		die("object %s tried to add buffer twice!", oid_ਮੰਜ਼ਿਲ_hex(&object->oid));
}

/*
 * Make sure at least "min" bytes are available in the buffer, and
 * return the pointer to the buffer.
 */
ਸਥਿਰ ਖਾਲੀ *fill(ਪੂਰਨ min)
{
	ਜੇ (min <= len)
		ਵਾਪਸ ਬਫਰ + ਵਿੱਥ;
	ਜੇ (min > ਆਕਾਰ(ਬਫਰ))
		die("cannot fill %d bytes", min);
	ਜੇ (ਵਿੱਥ) {
		git_hash_update(&ctx, ਬਫਰ, ਵਿੱਥ);
		memmove(ਬਫਰ, ਬਫਰ + ਵਿੱਥ, len);
		ਵਿੱਥ = 0;
	}
	ਕਰੋ {
		ssize_t ret = xread(0, ਬਫਰ + len, ਆਕਾਰ(ਬਫਰ) - len);
		ਜੇ (ret <= 0) {
			ਜੇ (!ret)
				die("early EOF");
			die_errno("read error on input");
		}
		len += ret;
	} ਜਦੋਂ (len < min);
	ਵਾਪਸ ਬਫਰ;
}

ਸਥਿਰ ਖਾਲੀ use(ਪੂਰਨ ਬਾਈਟਾਂ)
{
	ਜੇ (ਬਾਈਟਾਂ > len)
		die("used more bytes than were available");
	len -= ਬਾਈਟਾਂ;
	ਵਿੱਥ += ਬਾਈਟਾਂ;

	/* make sure off_t is sufficiently large not to wrap */
	ਜੇ (signed_ਜੋੜੋ_overflows(consumed_ਬਾਈਟਾਂ, ਬਾਈਟਾਂ))
		die("pack too large for current definition of off_t");
	consumed_ਬਾਈਟਾਂ += ਬਾਈਟਾਂ;
	ਜੇ (max_ਨਿਵੇਸ਼_ਮਾਪ && consumed_ਬਾਈਟਾਂ > max_ਨਿਵੇਸ਼_ਮਾਪ)
		die(_("pack exceeds maximum allowed size"));
	display_throughput(ਤਰੱਕੀ, consumed_ਬਾਈਟਾਂ);
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
ਸਥਿਰ ਖਾਲੀ *get_ਅੰਕੜੇ(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਮਾਪ)
{
	git_zstream ਧਾਰਾ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ bufsize = dry_ਲੜੀਵਾਰ && ਮਾਪ > 8192 ? 8192 : ਮਾਪ;
	ਖਾਲੀ *buf = xmallocz(bufsize);

	memset(&ਧਾਰਾ, 0, ਆਕਾਰ(ਧਾਰਾ));

	ਧਾਰਾ.ਅਗਲਾ_ਬਾਹਰਲਾ = buf;
	ਧਾਰਾ.avail_ਬਾਹਰਲਾ = bufsize;
	ਧਾਰਾ.ਅਗਲਾ_ਵਿੱਚ = fill(1);
	ਧਾਰਾ.avail_ਵਿੱਚ = len;
	git_inflate_init(&ਧਾਰਾ);

	ਲਈ (;;) {
		ਪੂਰਨ ret = git_inflate(&ਧਾਰਾ, 0);
		use(len - ਧਾਰਾ.avail_ਵਿੱਚ);
		ਜੇ (ਧਾਰਾ.ਕੁੱਲ_ਬਾਹਰਲਾ == ਮਾਪ && ret == Z_ਧਾਰਾ_ਸਿਰਾ)
			ਰੋਕੋ;
		ਜੇ (ret != Z_OK) {
			error("inflate returned %d", ret);
			FREE_AND_NULL(buf);
			ਜੇ (!recover)
				ਬਾਹਰ_ਨਿਕਲੋ(1);
			has_errors = 1;
			ਰੋਕੋ;
		}
		ਧਾਰਾ.ਅਗਲਾ_ਵਿੱਚ = fill(1);
		ਧਾਰਾ.avail_ਵਿੱਚ = len;
		ਜੇ (dry_ਲੜੀਵਾਰ) {
			/* reuse the buffer in dry_run mode */
			ਧਾਰਾ.ਅਗਲਾ_ਬਾਹਰਲਾ = buf;
			ਧਾਰਾ.avail_ਬਾਹਰਲਾ = bufsize > ਮਾਪ - ਧਾਰਾ.ਕੁੱਲ_ਬਾਹਰਲਾ ?
						   ਮਾਪ - ਧਾਰਾ.ਕੁੱਲ_ਬਾਹਰਲਾ :
						   bufsize;
		}
	}
	git_inflate_ਸਿਰਾ(&ਧਾਰਾ);
	ਜੇ (dry_ਲੜੀਵਾਰ)
		FREE_AND_NULL(buf);
	ਵਾਪਸ buf;
}

ਬਣਤਰ delta_info {
	ਬਣਤਰ object_id ਅਧਾਰ_oid;
	ਬਿਨਾਨਿਸ਼ਾਨੀ nr;
	off_t ਅਧਾਰ_ਵਿੱਥ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਮਾਪ;
	ਖਾਲੀ *delta;
	ਬਣਤਰ delta_info *ਅਗਲਾ;
};

ਸਥਿਰ ਬਣਤਰ delta_info *delta_list;

ਸਥਿਰ ਖਾਲੀ ਜੋੜੋ_delta_ਮੰਜ਼ਿਲ_list(ਬਿਨਾਨਿਸ਼ਾਨੀ nr, ਅਚਲ ਬਣਤਰ object_id *ਅਧਾਰ_oid,
			      off_t ਅਧਾਰ_ਵਿੱਥ,
			      ਖਾਲੀ *delta, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਮਾਪ)
{
	ਬਣਤਰ delta_info *info = xmalloc(ਆਕਾਰ(*info));

	oidcpy(&info->ਅਧਾਰ_oid, ਅਧਾਰ_oid);
	info->ਅਧਾਰ_ਵਿੱਥ = ਅਧਾਰ_ਵਿੱਥ;
	info->ਮਾਪ = ਮਾਪ;
	info->delta = delta;
	info->nr = nr;
	info->ਅਗਲਾ = delta_list;
	delta_list = info;
}

ਬਣਤਰ obj_info {
	off_t ਵਿੱਥ;
	ਬਣਤਰ object_id oid;
	ਬਣਤਰ object *obj;
};

/* Remember to update object flag allocation in object.h */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FLAG_OPEN (1u<<20)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FLAG_WRITTEN (1u<<21)

ਸਥਿਰ ਬਣਤਰ obj_info *obj_list;
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ nr_objects;

/*
 * Called only from check_object() after it verified this object
 * is Ok.
 */
ਸਥਿਰ ਖਾਲੀ write_cached_object(ਬਣਤਰ object *obj, ਬਣਤਰ obj_ਬਫਰ *obj_buf)
{
	ਬਣਤਰ object_id oid;

	ਜੇ (odb_write_object(the_repository->objects, obj_buf->ਬਫਰ, obj_buf->ਮਾਪ,
			     obj->type, &oid) < 0)
		die("failed to write object %s", oid_ਮੰਜ਼ਿਲ_hex(&obj->oid));
	obj->flags |= FLAG_WRITTEN;
}

/*
 * At the very end of the processing, write_rest() scans the objects
 * that have reachability requirements and calls this function.
 * Verify its reachability and validity recursively and write it out.
 */
ਸਥਿਰ ਪੂਰਨ check_object(ਬਣਤਰ object *obj, ਗਿਣਤੀ object_type type,
			ਖਾਲੀ *ਅੰਕੜੇ ਅਣਵਰਤਿਆ,
			ਬਣਤਰ fsck_options *options ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ obj_ਬਫਰ *obj_buf;

	ਜੇ (!obj)
		ਵਾਪਸ 1;

	ਜੇ (obj->flags & FLAG_WRITTEN)
		ਵਾਪਸ 0;

	ਜੇ (type != OBJ_ANY && obj->type != type)
		die("object type mismatch");

	ਜੇ (!(obj->flags & FLAG_OPEN)) {
		ਮਾਪ_t ਮਾਪ;
		ਪੂਰਨ type = odb_read_object_info(the_repository->objects, &obj->oid, &ਮਾਪ);
		ਜੇ (type != obj->type || type <= 0)
			die("object of unexpected type");
		obj->flags |= FLAG_WRITTEN;
		ਵਾਪਸ 0;
	}

	obj_buf = lookup_object_ਬਫਰ(obj);
	ਜੇ (!obj_buf)
		die("Whoops! Cannot find object '%s'", oid_ਮੰਜ਼ਿਲ_hex(&obj->oid));
	ਜੇ (fsck_object(obj, obj_buf->ਬਫਰ, obj_buf->ਮਾਪ, &fsck_options))
		die("fsck error in packed object");
	fsck_options.walk = check_object;
	ਜੇ (fsck_walk(obj, ਨਿਰਾ, &fsck_options))
		die("Error on reachable objects of %s", oid_ਮੰਜ਼ਿਲ_hex(&obj->oid));
	write_cached_object(obj, obj_buf);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ write_rest(ਖਾਲੀ)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ i;
	ਲਈ (i = 0; i < nr_objects; i++) {
		ਜੇ (obj_list[i].obj)
			check_object(obj_list[i].obj, OBJ_ANY, ਨਿਰਾ, ਨਿਰਾ);
	}
}

ਸਥਿਰ ਖਾਲੀ added_object(ਬਿਨਾਨਿਸ਼ਾਨੀ nr, ਗਿਣਤੀ object_type type,
			 ਖਾਲੀ *ਅੰਕੜੇ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਮਾਪ);

/*
 * Write out nr-th object from the list, now we know the contents
 * of it.  Under --strict, this buffers structured objects in-core,
 * to be checked at the end.
 */
ਸਥਿਰ ਖਾਲੀ write_object(ਬਿਨਾਨਿਸ਼ਾਨੀ nr, ਗਿਣਤੀ object_type type,
			 ਖਾਲੀ *buf, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਮਾਪ)
{
	ਜੇ (!strict) {
		ਜੇ (odb_write_object(the_repository->objects, buf, ਮਾਪ, type,
				     &obj_list[nr].oid) < 0)
			die("failed to write object");
		added_object(nr, type, buf, ਮਾਪ);
		ਮੁਕਤ(buf);
		obj_list[nr].obj = ਨਿਰਾ;
	} ਨਹੀਂਤਾਂ ਜੇ (type == OBJ_BLOB) {
		ਬਣਤਰ blob *blob;
		ਜੇ (odb_write_object(the_repository->objects, buf, ਮਾਪ, type,
				     &obj_list[nr].oid) < 0)
			die("failed to write object");
		added_object(nr, type, buf, ਮਾਪ);
		ਮੁਕਤ(buf);

		blob = lookup_blob(the_repository, &obj_list[nr].oid);
		ਜੇ (blob)
			blob->object.flags |= FLAG_WRITTEN;
		ਨਹੀਂਤਾਂ
			die("invalid blob object");
		obj_list[nr].obj = ਨਿਰਾ;
	} ਨਹੀਂਤਾਂ {
		ਬਣਤਰ object *obj;
		ਪੂਰਨ eaten;
		hash_object_file(the_hash_algo, buf, ਮਾਪ, type,
				 &obj_list[nr].oid);
		added_object(nr, type, buf, ਮਾਪ);
		obj = parse_object_ਬਫਰ(the_repository, &obj_list[nr].oid,
					  type, ਮਾਪ, buf,
					  &eaten);
		ਜੇ (!obj)
			die("invalid %s", type_name(type));
		ਜੋੜੋ_object_ਬਫਰ(obj, buf, ਮਾਪ);
		obj->flags |= FLAG_OPEN;
		obj_list[nr].obj = obj;
	}
}

ਸਥਿਰ ਖਾਲੀ resolve_delta(ਬਿਨਾਨਿਸ਼ਾਨੀ nr, ਗਿਣਤੀ object_type type,
			  ਖਾਲੀ *ਅਧਾਰ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਅਧਾਰ_ਮਾਪ,
			  ਖਾਲੀ *delta, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ delta_ਮਾਪ_2)
{
	ਖਾਲੀ *ਨਤੀਜਾ;
	ਮਾਪ_t ਨਤੀਜਾ_ਮਾਪ;

	ਨਤੀਜਾ = patch_delta(ਅਧਾਰ, ਅਧਾਰ_ਮਾਪ,
			     delta, delta_ਮਾਪ_2,
			     &ਨਤੀਜਾ_ਮਾਪ);
	ਜੇ (!ਨਤੀਜਾ)
		die("failed to apply delta");
	ਮੁਕਤ(delta);
	write_object(nr, type, ਨਤੀਜਾ, ਨਤੀਜਾ_ਮਾਪ);
}

/*
 * We now know the contents of an object (which is nr-th in the pack);
 * resolve all the deltified objects that are based on it.
 */
ਸਥਿਰ ਖਾਲੀ added_object(ਬਿਨਾਨਿਸ਼ਾਨੀ nr, ਗਿਣਤੀ object_type type,
			 ਖਾਲੀ *ਅੰਕੜੇ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਮਾਪ)
{
	ਬਣਤਰ delta_info **p = &delta_list;
	ਬਣਤਰ delta_info *info;

	ਜਦੋਂ ((info = *p) != ਨਿਰਾ) {
		ਜੇ (oideq(&info->ਅਧਾਰ_oid, &obj_list[nr].oid) ||
		    info->ਅਧਾਰ_ਵਿੱਥ == obj_list[nr].ਵਿੱਥ) {
			*p = info->ਅਗਲਾ;
			p = &delta_list;
			resolve_delta(info->nr, type, ਅੰਕੜੇ, ਮਾਪ,
				      info->delta, info->ਮਾਪ);
			ਮੁਕਤ(info);
			ਜਾਰੀ;
		}
		p = &info->ਅਗਲਾ;
	}
}

ਸਥਿਰ ਖਾਲੀ unpack_non_delta_ਇੰਦਰਾਜ(ਗਿਣਤੀ object_type type, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਮਾਪ,
				   ਬਿਨਾਨਿਸ਼ਾਨੀ nr)
{
	ਖਾਲੀ *buf = get_ਅੰਕੜੇ(ਮਾਪ);

	ਜੇ (buf)
		write_object(nr, type, buf, ਮਾਪ);
}

ਬਣਤਰ zlib_ਧਾਰਾ {
	ਬਣਤਰ odb_ਧਾਰਾ ਅਧਾਰ;
	git_zstream *zstream;
	ਪੂਰਨ ਸਥਿਤੀ;
};

ਸਥਿਰ ssize_t zlib_ਧਾਰਾ_read(ਬਣਤਰ odb_ਧਾਰਾ *ਵਿੱਚ_ਧਾਰਾ,
				ਅੱਖਰ *buf, ਮਾਪ_t buf_len)
{
	ਬਣਤਰ zlib_ਧਾਰਾ *ਅੰਕੜੇ = container_ਦਾ(ਵਿੱਚ_ਧਾਰਾ, ਬਣਤਰ zlib_ਧਾਰਾ, ਅਧਾਰ);
	git_zstream *zstream = ਅੰਕੜੇ->zstream;

	ਜੇ (ਅੰਕੜੇ->ਸਥਿਤੀ != Z_OK)
		ਵਾਪਸ 0;

	zstream->ਅਗਲਾ_ਬਾਹਰਲਾ = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *) buf;
	zstream->avail_ਬਾਹਰਲਾ = buf_len;

	ਜਦੋਂ (ਅੰਕੜੇ->ਸਥਿਤੀ == Z_OK && zstream->avail_ਬਾਹਰਲਾ == buf_len) {
		zstream->ਅਗਲਾ_ਵਿੱਚ = fill(1);
		zstream->avail_ਵਿੱਚ = len;
		ਅੰਕੜੇ->ਸਥਿਤੀ = git_inflate(zstream, 0);
		use(len - zstream->avail_ਵਿੱਚ);
	}

	ਵਾਪਸ buf_len - zstream->avail_ਬਾਹਰਲਾ;
}

ਸਥਿਰ ਖਾਲੀ ਧਾਰਾ_blob(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਮਾਪ, ਬਿਨਾਨਿਸ਼ਾਨੀ nr)
{
	git_zstream zstream = { 0 };
	ਬਣਤਰ zlib_ਧਾਰਾ ਵਿੱਚ_ਧਾਰਾ = {
		.ਅਧਾਰ = {
			.ਪਠ = zlib_ਧਾਰਾ_read,
			.ਮਾਪ = ਮਾਪ,
			.type = OBJ_BLOB,
		},
		.zstream = &zstream,
		.ਸਥਿਤੀ = Z_OK,
	};
	ਬਣਤਰ obj_info *info = &obj_list[nr];

	git_inflate_init(&zstream);

	ਜੇ (odb_write_object_ਧਾਰਾ(the_repository->objects, &ਵਿੱਚ_ਧਾਰਾ.ਅਧਾਰ, &info->oid))
		die(_("failed to write object in stream"));

	ਜੇ (ਵਿੱਚ_ਧਾਰਾ.ਸਥਿਤੀ != Z_ਧਾਰਾ_ਸਿਰਾ)
		die(_("inflate returned (%d)"), ਵਿੱਚ_ਧਾਰਾ.ਸਥਿਤੀ);
	git_inflate_ਸਿਰਾ(&zstream);

	ਜੇ (strict) {
		ਬਣਤਰ blob *blob = lookup_blob(the_repository, &info->oid);

		ਜੇ (!blob)
			die(_("invalid blob object from stream"));
		blob->object.flags |= FLAG_WRITTEN;
	}
	info->obj = ਨਿਰਾ;
}

ਸਥਿਰ ਪੂਰਨ resolve_against_held(ਬਿਨਾਨਿਸ਼ਾਨੀ nr, ਅਚਲ ਬਣਤਰ object_id *ਅਧਾਰ,
				ਖਾਲੀ *delta_ਅੰਕੜੇ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ delta_ਮਾਪ_2)
{
	ਬਣਤਰ object *obj;
	ਬਣਤਰ obj_ਬਫਰ *obj_ਬਫਰ;
	obj = lookup_object(the_repository, ਅਧਾਰ);
	ਜੇ (!obj)
		ਵਾਪਸ 0;
	obj_ਬਫਰ = lookup_object_ਬਫਰ(obj);
	ਜੇ (!obj_ਬਫਰ)
		ਵਾਪਸ 0;
	resolve_delta(nr, obj->type, obj_ਬਫਰ->ਬਫਰ,
		      obj_ਬਫਰ->ਮਾਪ, delta_ਅੰਕੜੇ, delta_ਮਾਪ_2);
	ਵਾਪਸ 1;
}

ਸਥਿਰ ਖਾਲੀ unpack_delta_ਇੰਦਰਾਜ(ਗਿਣਤੀ object_type type, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ delta_ਮਾਪ_2,
			       ਬਿਨਾਨਿਸ਼ਾਨੀ nr)
{
	ਖਾਲੀ *delta_ਅੰਕੜੇ, *ਅਧਾਰ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਅਧਾਰ_ਮਾਪ;
	ਮਾਪ_t ਅਧਾਰ_ਮਾਪ_st = 0;
	ਬਣਤਰ object_id ਅਧਾਰ_oid;

	ਜੇ (type == OBJ_REF_DELTA) {
		oidread(&ਅਧਾਰ_oid, fill(the_hash_algo->rawsz), the_repository->hash_algo);
		use(the_hash_algo->rawsz);
		delta_ਅੰਕੜੇ = get_ਅੰਕੜੇ(delta_ਮਾਪ_2);
		ਜੇ (!delta_ਅੰਕੜੇ)
			ਵਾਪਸ;
		ਜੇ (odb_has_object(the_repository->objects, &ਅਧਾਰ_oid,
				   ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR))
			; /* Ok we have this one */
		ਨਹੀਂਤਾਂ ਜੇ (resolve_against_held(nr, &ਅਧਾਰ_oid,
					      delta_ਅੰਕੜੇ, delta_ਮਾਪ_2))
			ਵਾਪਸ; /* we are done */
		ਨਹੀਂਤਾਂ {
			/* cannot resolve yet --- queue it */
			oidclr(&obj_list[nr].oid, the_repository->hash_algo);
			ਜੋੜੋ_delta_ਮੰਜ਼ਿਲ_list(nr, &ਅਧਾਰ_oid, 0, delta_ਅੰਕੜੇ, delta_ਮਾਪ_2);
			ਵਾਪਸ;
		}
	} ਨਹੀਂਤਾਂ {
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਅਧਾਰ_ਮਿਲਿਆ = 0;
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *pack, c;
		off_t ਅਧਾਰ_ਵਿੱਥ;
		ਬਿਨਾਨਿਸ਼ਾਨੀ lo, mid, hi;

		pack = fill(1);
		c = *pack;
		use(1);
		ਅਧਾਰ_ਵਿੱਥ = c & 127;
		ਜਦੋਂ (c & 128) {
			ਅਧਾਰ_ਵਿੱਥ += 1;
			ਜੇ (!ਅਧਾਰ_ਵਿੱਥ || MSB(ਅਧਾਰ_ਵਿੱਥ, 7))
				die("offset value overflow for delta base object");
			pack = fill(1);
			c = *pack;
			use(1);
			ਅਧਾਰ_ਵਿੱਥ = (ਅਧਾਰ_ਵਿੱਥ << 7) + (c & 127);
		}
		ਅਧਾਰ_ਵਿੱਥ = obj_list[nr].ਵਿੱਥ - ਅਧਾਰ_ਵਿੱਥ;
		ਜੇ (ਅਧਾਰ_ਵਿੱਥ <= 0 || ਅਧਾਰ_ਵਿੱਥ >= obj_list[nr].ਵਿੱਥ)
			die("offset value out of bound for delta base object");

		delta_ਅੰਕੜੇ = get_ਅੰਕੜੇ(delta_ਮਾਪ_2);
		ਜੇ (!delta_ਅੰਕੜੇ)
			ਵਾਪਸ;
		lo = 0;
		hi = nr;
		ਜਦੋਂ (lo < hi) {
			mid = lo + (hi - lo) / 2;
			ਜੇ (ਅਧਾਰ_ਵਿੱਥ < obj_list[mid].ਵਿੱਥ) {
				hi = mid;
			} ਨਹੀਂਤਾਂ ਜੇ (ਅਧਾਰ_ਵਿੱਥ > obj_list[mid].ਵਿੱਥ) {
				lo = mid + 1;
			} ਨਹੀਂਤਾਂ {
				oidcpy(&ਅਧਾਰ_oid, &obj_list[mid].oid);
				ਅਧਾਰ_ਮਿਲਿਆ = !is_null_oid(&ਅਧਾਰ_oid);
				ਰੋਕੋ;
			}
		}
		ਜੇ (!ਅਧਾਰ_ਮਿਲਿਆ) {
			/*
			 * The delta base object is itself a delta that
			 * has not been resolved yet.
			 */
			oidclr(&obj_list[nr].oid, the_repository->hash_algo);
			ਜੋੜੋ_delta_ਮੰਜ਼ਿਲ_list(nr, null_oid(the_hash_algo), ਅਧਾਰ_ਵਿੱਥ,
					  delta_ਅੰਕੜੇ, delta_ਮਾਪ_2);
			ਵਾਪਸ;
		}
	}

	ਜੇ (resolve_against_held(nr, &ਅਧਾਰ_oid, delta_ਅੰਕੜੇ, delta_ਮਾਪ_2))
		ਵਾਪਸ;

	ਅਧਾਰ = odb_read_object(the_repository->objects, &ਅਧਾਰ_oid,
			       &type, &ਅਧਾਰ_ਮਾਪ_st);
	ਅਧਾਰ_ਮਾਪ = cast_ਮਾਪ_t_ਮੰਜ਼ਿਲ_ulong(ਅਧਾਰ_ਮਾਪ_st);
	ਜੇ (!ਅਧਾਰ) {
		error("failed to read delta-pack base object %s",
		      oid_ਮੰਜ਼ਿਲ_hex(&ਅਧਾਰ_oid));
		ਜੇ (!recover)
			ਬਾਹਰ_ਨਿਕਲੋ(1);
		has_errors = 1;
		ਵਾਪਸ;
	}
	resolve_delta(nr, type, ਅਧਾਰ, ਅਧਾਰ_ਮਾਪ, delta_ਅੰਕੜੇ, delta_ਮਾਪ_2);
	ਮੁਕਤ(ਅਧਾਰ);
}

ਸਥਿਰ ਖਾਲੀ unpack_one(ਬਿਨਾਨਿਸ਼ਾਨੀ nr)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਖਿਸਕਾਓ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *pack;
	ਮਾਪ_t ਮਾਪ, c;
	ਗਿਣਤੀ object_type type;

	obj_list[nr].ਵਿੱਥ = consumed_ਬਾਈਟਾਂ;

	pack = fill(1);
	c = *pack;
	use(1);
	type = (c >> 4) & 7;
	ਮਾਪ = (c & 15);
	ਖਿਸਕਾਓ = 4;
	ਜਦੋਂ (c & 0x80) {
		ਜੇ ((bitsizeof(ਮਾਪ_t) - 7) < ਖਿਸਕਾਓ)
			die(_("object size too large for this platform"));
		pack = fill(1);
		c = *pack;
		use(1);
		ਮਾਪ += (c & 0x7f) << ਖਿਸਕਾਓ;
		ਖਿਸਕਾਓ += 7;
	}

	ਚੋਣ (type) {
	ਹਾਲਤ OBJ_BLOB:
		ਜੇ (!dry_ਲੜੀਵਾਰ &&
		    ਮਾਪ > repo_settings_get_big_file_ਹੱਦ(the_repository)) {
			ਧਾਰਾ_blob(ਮਾਪ, nr);
			ਵਾਪਸ;
		}
		/* fallthrough */
	ਹਾਲਤ OBJ_COMMIT:
	ਹਾਲਤ OBJ_ਰੁੱਖ:
	ਹਾਲਤ OBJ_ਟੈਗ:
		unpack_non_delta_ਇੰਦਰਾਜ(type, ਮਾਪ, nr);
		ਵਾਪਸ;
	ਹਾਲਤ OBJ_REF_DELTA:
	ਹਾਲਤ OBJ_OFS_DELTA:
		unpack_delta_ਇੰਦਰਾਜ(type, ਮਾਪ, nr);
		ਵਾਪਸ;
	ਮੂਲ:
		error("bad object type %d", type);
		has_errors = 1;
		ਜੇ (recover)
			ਵਾਪਸ;
		ਬਾਹਰ_ਨਿਕਲੋ(1);
	}
}

ਸਥਿਰ ਖਾਲੀ unpack_all(ਖਾਲੀ)
{
	ਪੂਰਨ i;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *hdr = fill(ਆਕਾਰ(ਬਣਤਰ pack_ਸਿਰਲੇਖ));
	ਬਣਤਰ odb_transaction *transaction;

	ਜੇ (get_be32(hdr) != PACK_SIGNATURE)
		die("bad pack file");
	hdr += 4;
	ਜੇ (!pack_ਸੰਸਕਰਣ_ok_native(get_be32(hdr)))
		die("unknown pack file version %"PRIu32,
		    get_be32(hdr));
	hdr += 4;
	nr_objects = get_be32(hdr);
	use(ਆਕਾਰ(ਬਣਤਰ pack_ਸਿਰਲੇਖ));

	ਜੇ (!quiet)
		ਤਰੱਕੀ = ਸ਼ੁਰੂ_ਤਰੱਕੀ(the_repository,
					  _("Unpacking objects"), nr_objects);
	CALLOC_ARRAY(obj_list, nr_objects);
	odb_transaction_begin_or_die(the_repository->objects, &transaction, 0);
	ਲਈ (i = 0; i < nr_objects; i++) {
		unpack_one(i);
		display_ਤਰੱਕੀ(ਤਰੱਕੀ, i + 1);
	}
	odb_transaction_commit_and_finalize_or_die(transaction);
	stop_ਤਰੱਕੀ(&ਤਰੱਕੀ);

	ਜੇ (delta_list)
		die("unresolved deltas left after unpacking");
}

ਪੂਰਨ cmd_unpack_objects(ਪੂਰਨ argc,
		       ਅਚਲ ਅੱਖਰ **argv,
		       ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ ਅਣਵਰਤਿਆ,
		       ਬਣਤਰ repository *repo)
{
	ਪੂਰਨ i;
	ਬਣਤਰ object_id oid;
	ਬਣਤਰ git_hash_ctx tmp_ctx;

	disable_replace_refs();

	repo_config(the_repository, git_default_config, ਨਿਰਾ);

	quiet = !isatty(2);

	show_usage_if_asked(argc, argv, unpack_usage);

	fsck_options_init(&fsck_options, repo, FSCK_OPTIONS_STRICT);

	ਲਈ (i = 1 ; i < argc; i++) {
		ਅਚਲ ਅੱਖਰ *arg = argv[i];

		ਜੇ (*arg == '-') {
			ਜੇ (!strcmp(arg, "-n")) {
				dry_ਲੜੀਵਾਰ = 1;
				ਜਾਰੀ;
			}
			ਜੇ (!strcmp(arg, "-q")) {
				quiet = 1;
				ਜਾਰੀ;
			}
			ਜੇ (!strcmp(arg, "-r")) {
				recover = 1;
				ਜਾਰੀ;
			}
			ਜੇ (!strcmp(arg, "--strict")) {
				strict = 1;
				ਜਾਰੀ;
			}
			ਜੇ (skip_ਅਗੇਤਰ(arg, "--strict=", &arg)) {
				strict = 1;
				fsck_ਸੈੱਟ_ਸੁਨੇਹਾ_types(&fsck_options, arg);
				ਜਾਰੀ;
			}
			ਜੇ (skip_ਅਗੇਤਰ(arg, "--pack_header=", &arg)) {
				ਜੇ (parse_pack_ਸਿਰਲੇਖ_option(arg,
							     ਬਫਰ, &len) < 0)
					die(_("bad --pack_header: %s"), arg);
				ਜਾਰੀ;
			}
			ਜੇ (skip_ਅਗੇਤਰ(arg, "--max-input-size=", &arg)) {
				max_ਨਿਵੇਸ਼_ਮਾਪ = strtoumax(arg, ਨਿਰਾ, 10);
				ਜਾਰੀ;
			}
			usage(unpack_usage);
		}

		/* We don't take any non-flag arguments now.. Maybe some day */
		usage(unpack_usage);
	}
	git_hash_init(&ctx, the_hash_algo);
	unpack_all();
	git_hash_update(&ctx, ਬਫਰ, ਵਿੱਥ);
	git_hash_init(&tmp_ctx, the_hash_algo);
	git_hash_clone(&tmp_ctx, &ctx);
	git_hash_final_oid(&oid, &tmp_ctx);
	ਜੇ (strict) {
		write_rest();
		ਜੇ (fsck_ਅੰਤ(&fsck_options))
			die(_("fsck error in pack objects"));
	}
	ਜੇ (!hasheq(fill(the_hash_algo->rawsz), oid.hash,
		    the_repository->hash_algo))
		die("final sha1 did not match");
	use(the_hash_algo->rawsz);

	/* Write the last part of the buffer to stdout */
	write_ਵਿੱਚ_ਭਰਿਆ(1, ਬਫਰ + ਵਿੱਥ, len);

	/* All done */
	ਵਾਪਸ has_errors;
}
