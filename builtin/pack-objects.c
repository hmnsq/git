#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#अन्तर्भाव "builtin.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "hex.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "attr.h"
#अन्तर्भाव "object.h"
#अन्तर्भाव "commit.h"
#अन्तर्भाव "tag.h"
#अन्तर्भाव "delta.h"
#अन्तर्भाव "pack.h"
#अन्तर्भाव "pack-revindex.h"
#अन्तर्भाव "csum-file.h"
#अन्तर्भाव "tree-walk.h"
#अन्तर्भाव "diff.h"
#अन्तर्भाव "revision.h"
#अन्तर्भाव "list-objects.h"
#अन्तर्भाव "list-objects-filter-options.h"
#अन्तर्भाव "pack-objects.h"
#अन्तर्भाव "progress.h"
#अन्तर्भाव "refs.h"
#अन्तर्भाव "thread-utils.h"
#अन्तर्भाव "pack-bitmap.h"
#अन्तर्भाव "delta-islands.h"
#अन्तर्भाव "reachable.h"
#अन्तर्भाव "oid-array.h"
#अन्तर्भाव "strvec.h"
#अन्तर्भाव "strmap.h"
#अन्तर्भाव "list.h"
#अन्तर्भाव "packfile.h"
#अन्तर्भाव "object-file.h"
#अन्तर्भाव "object-file-convert.h"
#अन्तर्भाव "odb.h"
#अन्तर्भाव "odb/streaming.h"
#अन्तर्भाव "replace-object.h"
#अन्तर्भाव "dir.h"
#अन्तर्भाव "midx.h"
#अन्तर्भाव "trace2.h"
#अन्तर्भाव "shallow.h"
#अन्तर्भाव "promisor-remote.h"
#अन्तर्भाव "pack-mtimes.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "pkt-line.h"
#अन्तर्भाव "blob.h"
#अन्तर्भाव "tree.h"
#अन्तर्भाव "path-walk.h"

/*
 * Objects we are going to pack are collected in the `to_pack` structure.
 * It contains an array (dynamically expanded) of the object data, and a map
 * that can resolve SHA1s to their position in the array.
 */
स्थिर संरचना packing_data to_pack;

स्थिर अन्तःस्थ संरचना object_entry *oe_delta(
		ध्रुव संरचना packing_data *pack,
		ध्रुव संरचना object_entry *e)
{
	यदि (!e->delta_idx)
		प्रत्यागम रिक्त;
	यदि (e->ext_base)
		प्रत्यागम &pack->ext_bases[e->delta_idx - 1];
	अन्यथा
		प्रत्यागम &pack->objects[e->delta_idx - 1];
}

स्थिर अन्तःस्थ size_t oe_delta_size(संरचना packing_data *pack,
				   ध्रुव संरचना object_entry *e)
{
	यदि (e->delta_size_valid)
		प्रत्यागम e->delta_size_;

	/*
	 * pack->delta_size[] can't be NULL because oe_set_delta_size()
	 * must have been called when a new delta is saved with
	 * oe_set_delta().
	 * If oe_delta() returns NULL (i.e. default state, which means
	 * delta_size_valid is also false), then the caller must never
	 * call oe_delta_size().
	 */
	प्रत्यागम pack->delta_size[e - pack->objects];
}

size_t oe_get_size_slow(संरचना packing_data *pack,
			ध्रुव संरचना object_entry *e);

स्थिर अन्तःस्थ size_t oe_size(संरचना packing_data *pack,
			     ध्रुव संरचना object_entry *e)
{
	यदि (e->size_valid)
		प्रत्यागम e->size_;

	प्रत्यागम oe_get_size_slow(pack, e);
}

स्थिर अन्तःस्थ शून्य oe_set_delta(संरचना packing_data *pack,
				संरचना object_entry *e,
				संरचना object_entry *delta)
{
	यदि (delta)
		e->delta_idx = (delta - pack->objects) + 1;
	अन्यथा
		e->delta_idx = 0;
}

स्थिर अन्तःस्थ संरचना object_entry *oe_delta_sibling(
		ध्रुव संरचना packing_data *pack,
		ध्रुव संरचना object_entry *e)
{
	यदि (e->delta_sibling_idx)
		प्रत्यागम &pack->objects[e->delta_sibling_idx - 1];
	प्रत्यागम रिक्त;
}

स्थिर अन्तःस्थ संरचना object_entry *oe_delta_child(
		ध्रुव संरचना packing_data *pack,
		ध्रुव संरचना object_entry *e)
{
	यदि (e->delta_child_idx)
		प्रत्यागम &pack->objects[e->delta_child_idx - 1];
	प्रत्यागम रिक्त;
}

स्थिर अन्तःस्थ शून्य oe_set_delta_child(संरचना packing_data *pack,
				      संरचना object_entry *e,
				      संरचना object_entry *delta)
{
	यदि (delta)
		e->delta_child_idx = (delta - pack->objects) + 1;
	अन्यथा
		e->delta_child_idx = 0;
}

स्थिर अन्तःस्थ शून्य oe_set_delta_sibling(संरचना packing_data *pack,
					संरचना object_entry *e,
					संरचना object_entry *delta)
{
	यदि (delta)
		e->delta_sibling_idx = (delta - pack->objects) + 1;
	अन्यथा
		e->delta_sibling_idx = 0;
}

स्थिर अन्तःस्थ शून्य oe_set_size(संरचना packing_data *pack,
			       संरचना object_entry *e,
			       size_t size)
{
	यदि (size < pack->oe_size_limit) {
		e->size_ = size;
		e->size_valid = 1;
	} अन्यथा {
		e->size_valid = 0;
		यदि (oe_get_size_slow(pack, e) != size)
			BUG("'size' is supposed to be the object size!");
	}
}

स्थिर अन्तःस्थ शून्य oe_set_delta_size(संरचना packing_data *pack,
				     संरचना object_entry *e,
				     size_t size)
{
	यदि (size < pack->oe_delta_size_limit) {
		e->delta_size_ = size;
		e->delta_size_valid = 1;
	} अन्यथा {
		packing_data_lock(pack);
		यदि (!pack->delta_size)
			ALLOC_ARRAY(pack->delta_size, pack->nr_alloc);
		packing_data_unlock(pack);

		pack->delta_size[e - pack->objects] = size;
		e->delta_size_valid = 0;
	}
}

#define IN_PACK(obj) oe_in_pack(&to_pack, obj)
#define SIZE(obj) oe_size(&to_pack, obj)
#define SET_SIZE(obj,size) oe_set_size(&to_pack, obj, size)
#define DELTA_SIZE(obj) oe_delta_size(&to_pack, obj)
#define DELTA(obj) oe_delta(&to_pack, obj)
#define DELTA_CHILD(obj) oe_delta_child(&to_pack, obj)
#define DELTA_SIBLING(obj) oe_delta_sibling(&to_pack, obj)
#define SET_DELTA(obj, val) oe_set_delta(&to_pack, obj, val)
#define SET_DELTA_EXT(obj, oid) oe_set_delta_ext(&to_pack, obj, oid)
#define SET_DELTA_SIZE(obj, val) oe_set_delta_size(&to_pack, obj, val)
#define SET_DELTA_CHILD(obj, val) oe_set_delta_child(&to_pack, obj, val)
#define SET_DELTA_SIBLING(obj, val) oe_set_delta_sibling(&to_pack, obj, val)

स्थिर ध्रुव वर्ण *ध्रुव pack_usage[] = {
	N_("git pack-objects [-q | --progress | --all-progress] [--all-progress-implied]\n"
	   "                 [--no-reuse-delta] [--delta-base-offset] [--non-empty]\n"
	   "                 [--local] [--incremental] [--window=<n>] [--depth=<n>]\n"
	   "                 [--revs [--unpacked | --all]] [--keep-pack=<pack-name>]\n"
	   "                 [--cruft] [--cruft-expiration=<time>]\n"
	   "                 [--stdout [--filter=<filter-spec>] | <base-name>]\n"
	   "                 [--shallow] [--keep-true-parents] [--[no-]sparse]\n"
	   "                 [--name-hash-version=<n>] [--path-walk] < <object-list>"),
	रिक्त
};

स्थिर संरचना pack_idx_entry **written_list;
स्थिर uint32_t nr_result, nr_written, nr_seen;
स्थिर संरचना bitmap_index *bitmap_git;
स्थिर uint32_t write_layer;

स्थिर पूर्णाङ्क non_empty;
स्थिर पूर्णाङ्क reuse_delta = 1, reuse_object = 1;
स्थिर पूर्णाङ्क keep_unreachable, unpack_unreachable, include_tag;
स्थिर timestamp_t unpack_unreachable_expiration;
स्थिर पूर्णाङ्क pack_loose_unreachable;
स्थिर पूर्णाङ्क cruft;
स्थिर पूर्णाङ्क shallow = 0;
स्थिर timestamp_t cruft_expiration;
स्थिर पूर्णाङ्क local;
स्थिर पूर्णाङ्क have_non_local_packs;
स्थिर पूर्णाङ्क incremental;
स्थिर पूर्णाङ्क ignore_packed_keep_on_disk;
स्थिर पूर्णाङ्क ignore_packed_keep_in_core;
स्थिर पूर्णाङ्क ignore_packed_keep_in_core_open;
स्थिर पूर्णाङ्क ignore_packed_keep_in_core_has_cruft;
स्थिर पूर्णाङ्क allow_ofs_delta;
स्थिर संरचना pack_idx_option pack_idx_opts;
स्थिर ध्रुव वर्ण *base_name;
स्थिर पूर्णाङ्क progress = 1;
स्थिर पूर्णाङ्क window = 10;
स्थिर unsigned long pack_size_limit;
स्थिर पूर्णाङ्क depth = 50;
स्थिर पूर्णाङ्क delta_शोध_threads;
स्थिर पूर्णाङ्क pack_to_stdout;
स्थिर पूर्णाङ्क sparse;
स्थिर पूर्णाङ्क thin;
स्थिर पूर्णाङ्क path_walk = -1;
स्थिर पूर्णाङ्क अंक_preferred_base;
स्थिर संरचना progress *progress_state;

स्थिर संरचना bitmapped_pack *reuse_packfiles;
स्थिर size_t reuse_packfiles_nr;
स्थिर size_t reuse_packfiles_used_nr;
स्थिर uint32_t reuse_packfile_objects;
स्थिर संरचना bitmap *reuse_packfile_bitmap;

स्थिर पूर्णाङ्क use_bitmap_index_default = 1;
स्थिर पूर्णाङ्क use_bitmap_index = -1;
स्थिर गणना {
	NO_PACK_REUSE = 0,
	SINGLE_PACK_REUSE,
	MULTI_PACK_REUSE,
} allow_pack_reuse = SINGLE_PACK_REUSE;
स्थिर गणना {
	WRITE_BITMAP_FALSE = 0,
	WRITE_BITMAP_QUIET,
	WRITE_BITMAP_TRUE,
} write_bitmap_index;
स्थिर uint16_t write_bitmap_options = BITMAP_OPT_HASH_CACHE;

स्थिर पूर्णाङ्क exclude_promisor_objects;
स्थिर पूर्णाङ्क exclude_promisor_objects_best_effort;

स्थिर पूर्णाङ्क use_delta_islands;

स्थिर size_t delta_cache_size = 0;
स्थिर size_t max_delta_cache_size = DEFAULT_DELTA_CACHE_SIZE;
स्थिर unsigned long cache_max_small_delta_size = 1000;

स्थिर unsigned long window_memory_limit = 0;

स्थिर संरचना string_list uri_protocols = STRING_LIST_INIT_NODUP;

गणना missing_action {
	MA_ERROR = 0,      /* fail if any missing objects are encountered */
	MA_ALLOW_ANY,      /* silently allow ALL missing objects */
	MA_ALLOW_PROMISOR, /* silently allow all missing PROMISOR objects */
};
स्थिर गणना missing_action arg_missing_action;
स्थिर show_object_fn fn_show_object;

संरचना configured_exclusion {
	संरचना oidmap_entry e;
	वर्ण *pack_hash_hex;
	वर्ण *uri;
};
स्थिर संरचना oidmap configured_exclusions;

स्थिर संरचना oidset excluded_by_config;
स्थिर पूर्णाङ्क name_hash_version = -1;

गणना stdin_packs_mode {
	STDIN_PACKS_MODE_NONE,
	STDIN_PACKS_MODE_STANDARD,
	STDIN_PACKS_MODE_FOLLOW,
};

/**
 * Check whether the name_hash_version chosen by user input is appropriate,
 * and also validate whether it is compatible with other features.
 */
स्थिर शून्य validate_name_hash_version(शून्य)
{
	यदि (name_hash_version < 1 || name_hash_version > 2)
		die(_("invalid --name-hash-version option: %d"), name_hash_version);
	यदि (write_bitmap_index && name_hash_version != 1) {
		warning(_("currently, --write-bitmap-index requires --name-hash-version=1"));
		name_hash_version = 1;
	}
}

स्थिर अन्तःस्थ uint32_t pack_name_hash_fn(ध्रुव वर्ण *name)
{
	स्थिर पूर्णाङ्क seen_version = -1;

	यदि (seen_version < 0)
		seen_version = name_hash_version;
	अन्यथा यदि (seen_version != name_hash_version)
		BUG("name hash version changed from %d to %d mid-process",
		    seen_version, name_hash_version);

	विकल्प (name_hash_version) {
	पक्ष 1:
		प्रत्यागम pack_name_hash(name);

	पक्ष 2:
		प्रत्यागम pack_name_hash_v2((ध्रुव unsigned वर्ण *)name);

	स्वभाव:
		BUG("invalid name-hash version: %d", name_hash_version);
	}
}

/*
 * stats
 */
स्थिर uint32_t written, written_delta;
स्थिर uint32_t reused, reused_delta;

/*
 * Indexed commits
 */
स्थिर संरचना commit **indexed_commits;
स्थिर unsigned पूर्णाङ्क indexed_commits_nr;
स्थिर unsigned पूर्णाङ्क indexed_commits_alloc;

स्थिर शून्य index_commit_for_bitmap(संरचना commit *commit)
{
	यदि (indexed_commits_nr >= indexed_commits_alloc) {
		indexed_commits_alloc = (indexed_commits_alloc + 32) * 2;
		REALLOC_ARRAY(indexed_commits, indexed_commits_alloc);
	}

	indexed_commits[indexed_commits_nr++] = commit;
}

स्थिर शून्य *get_delta(संरचना object_entry *entry)
{
	unsigned long size, base_size;
	size_t delta_size;
	शून्य *buf, *base_buf, *delta_buf;
	गणना object_type type;
	size_t size_st = 0, base_size_st = 0;

	buf = odb_read_object(the_repository->objects, &entry->idx.oid,
			      &type, &size_st);
	size = cast_size_t_to_ulong(size_st);
	यदि (!buf)
		die(_("unable to read %s"), oid_to_hex(&entry->idx.oid));
	base_buf = odb_read_object(the_repository->objects,
				   &DELTA(entry)->idx.oid, &type,
				   &base_size_st);
	base_size = cast_size_t_to_ulong(base_size_st);
	यदि (!base_buf)
		die("unable to read %s",
		    oid_to_hex(&DELTA(entry)->idx.oid));
	delta_buf = diff_delta(base_buf, base_size,
			       buf, size, &delta_size, 0);
	/*
	 * We successfully computed this delta once but dropped it for
	 * memory reasons. Something is very wrong if this time we
	 * recompute and create a different delta.
	 */
	यदि (!delta_buf || delta_size != DELTA_SIZE(entry))
		BUG("delta size changed");
	मुक्त(buf);
	मुक्त(base_buf);
	प्रत्यागम delta_buf;
}

स्थिर unsigned long do_compress(शून्य **pptr, unsigned long size)
{
	git_zstream stream;
	शून्य *in, *out;
	unsigned long maxsize;
	संरचना repo_config_मूल्ये *cfg = repo_config_मूल्ये(the_repository);

	git_deflate_init(&stream, cfg->pack_compression_level);
	maxsize = git_deflate_bound(&stream, size);

	in = *pptr;
	out = xmalloc(maxsize);
	*pptr = out;

	stream.next_in = in;
	stream.avail_in = size;
	stream.next_out = out;
	stream.avail_out = maxsize;
	यावत् (git_deflate(&stream, Z_FINISH) == Z_OK)
		; /* nothing */
	git_deflate_end(&stream);

	मुक्त(in);
	प्रत्यागम stream.total_out;
}

स्थिर unsigned long write_large_blob_data(संरचना odb_stream *st, संरचना hashfile *f,
					   ध्रुव संरचना object_id *oid)
{
	git_zstream stream;
	unsigned वर्ण ibuf[1024 * 16];
	unsigned वर्ण obuf[1024 * 16];
	unsigned long olen = 0;
	संरचना repo_config_मूल्ये *cfg = repo_config_मूल्ये(the_repository);

	git_deflate_init(&stream, cfg->pack_compression_level);

	प्रति (;;) {
		ssize_t readlen;
		पूर्णाङ्क zret = Z_OK;
		readlen = odb_stream_read(st, ibuf, परिमाण(ibuf));
		यदि (readlen == -1)
			die(_("unable to read %s"), oid_to_hex(oid));

		stream.next_in = ibuf;
		stream.avail_in = readlen;
		यावत् ((stream.avail_in || readlen == 0) &&
		       (zret == Z_OK || zret == Z_BUF_ERROR)) {
			stream.next_out = obuf;
			stream.avail_out = परिमाण(obuf);
			zret = git_deflate(&stream, readlen ? 0 : Z_FINISH);
			hashwrite(f, obuf, stream.next_out - obuf);
			olen += stream.next_out - obuf;
		}
		यदि (stream.avail_in)
			die(_("deflate error (%d)"), zret);
		यदि (readlen == 0) {
			यदि (zret != Z_STREAM_END)
				die(_("deflate error (%d)"), zret);
			भङ्ग;
		}
	}
	git_deflate_end(&stream);
	प्रत्यागम olen;
}

/*
 * we are going to reuse the existing object data as is.  make
 * sure it is not corrupt.
 */
स्थिर पूर्णाङ्क check_pack_inflate(संरचना packed_git *p,
		संरचना pack_window **w_curs,
		off_t offset,
		off_t len,
		size_t expect)
{
	git_zstream stream;
	unsigned वर्ण fakebuf[4096], *in;
	पूर्णाङ्क st;

	memset(&stream, 0, परिमाण(stream));
	git_inflate_init(&stream);
	कुरु {
		in = use_pack(p, w_curs, offset, &stream.avail_in);
		stream.next_in = in;
		stream.next_out = fakebuf;
		stream.avail_out = परिमाण(fakebuf);
		st = git_inflate(&stream, Z_FINISH);
		offset += stream.next_in - in;
	} यावत् (st == Z_OK || st == Z_BUF_ERROR);
	git_inflate_end(&stream);
	प्रत्यागम (st == Z_STREAM_END &&
		stream.total_out == expect &&
		stream.total_in == len) ? 0 : -1;
}

स्थिर शून्य copy_pack_data(संरचना hashfile *f,
		संरचना packed_git *p,
		संरचना pack_window **w_curs,
		off_t offset,
		off_t len)
{
	unsigned वर्ण *in;
	size_t avail;

	यावत् (len) {
		in = use_pack(p, w_curs, offset, &avail);
		यदि (avail > len)
			avail = (unsigned long)len;
		hashwrite(f, in, avail);
		offset += avail;
		len -= avail;
	}
}

स्थिर अन्तःस्थ पूर्णाङ्क oe_size_greater_than(संरचना packing_data *pack,
				       ध्रुव संरचना object_entry *lhs,
				       size_t rhs)
{
	यदि (lhs->size_valid)
		प्रत्यागम lhs->size_ > rhs;
	यदि (rhs < pack->oe_size_limit) /* rhs < 2^x <= lhs ? */
		प्रत्यागम 1;
	प्रत्यागम oe_get_size_slow(pack, lhs) > rhs;
}

/* Return 0 if we will bust the pack-size limit */
स्थिर unsigned long write_no_reuse_object(संरचना hashfile *f, संरचना object_entry *entry,
					   unsigned long limit, पूर्णाङ्क usable_delta)
{
	unsigned long size, datalen;
	unsigned वर्ण header[MAX_PACK_OBJECT_HEADER],
		      dheader[MAX_PACK_OBJECT_HEADER];
	unsigned hdrlen;
	गणना object_type type;
	शून्य *buf;
	संरचना odb_stream *st = रिक्त;
	ध्रुव unsigned hashsz = the_hash_algo->rawsz;

	यदि (!usable_delta) {
		यदि (oe_type(entry) == OBJ_BLOB &&
		    oe_size_greater_than(&to_pack, entry,
					 repo_settings_get_big_file_threshold(the_repository)) &&
		    (st = odb_stream_from_object(the_repository->objects, &entry->idx.oid,
						 रिक्त)) != रिक्त) {
			buf = रिक्त;
			type = st->type;
			size = st->size;
		} अन्यथा {
			size_t size_st = 0;
			buf = odb_read_object(the_repository->objects,
					      &entry->idx.oid, &type,
					      &size_st);
			size = cast_size_t_to_ulong(size_st);
			यदि (!buf)
				die(_("unable to read %s"),
				    oid_to_hex(&entry->idx.oid));
		}
		/*
		 * make sure no cached delta data remains from a
		 * previous attempt before a pack split occurred.
		 */
		FREE_AND_NULL(entry->delta_data);
		entry->z_delta_size = 0;
	} अन्यथा यदि (entry->delta_data) {
		size = DELTA_SIZE(entry);
		buf = entry->delta_data;
		entry->delta_data = रिक्त;
		type = (allow_ofs_delta && DELTA(entry)->idx.offset) ?
			OBJ_OFS_DELTA : OBJ_REF_DELTA;
	} अन्यथा {
		buf = get_delta(entry);
		size = DELTA_SIZE(entry);
		type = (allow_ofs_delta && DELTA(entry)->idx.offset) ?
			OBJ_OFS_DELTA : OBJ_REF_DELTA;
	}

	यदि (st)	/* large blob case, just assume we don't compress well */
		datalen = size;
	अन्यथा यदि (entry->z_delta_size)
		datalen = entry->z_delta_size;
	अन्यथा
		datalen = do_compress(&buf, size);

	/*
	 * The object header is a byte of 'type' followed by zero or
	 * more bytes of length.
	 */
	hdrlen = encode_in_pack_object_header(header, परिमाण(header),
					      type, size);

	यदि (type == OBJ_OFS_DELTA) {
		/*
		 * Deltas with relative base contain an additional
		 * encoding of the relative offset for the delta
		 * base from this object's position in the pack.
		 */
		off_t ofs = entry->idx.offset - DELTA(entry)->idx.offset;
		unsigned pos = परिमाण(dheader) - 1;
		dheader[pos] = ofs & 127;
		यावत् (ofs >>= 7)
			dheader[--pos] = 128 | (--ofs & 127);
		यदि (limit && hdrlen + परिमाण(dheader) - pos + datalen + hashsz >= limit) {
			यदि (st)
				odb_stream_close(st);
			मुक्त(buf);
			प्रत्यागम 0;
		}
		hashwrite(f, header, hdrlen);
		hashwrite(f, dheader + pos, परिमाण(dheader) - pos);
		hdrlen += परिमाण(dheader) - pos;
	} अन्यथा यदि (type == OBJ_REF_DELTA) {
		/*
		 * Deltas with a base reference contain
		 * additional bytes for the base object ID.
		 */
		यदि (limit && hdrlen + hashsz + datalen + hashsz >= limit) {
			यदि (st)
				odb_stream_close(st);
			मुक्त(buf);
			प्रत्यागम 0;
		}
		hashwrite(f, header, hdrlen);
		hashwrite(f, DELTA(entry)->idx.oid.hash, hashsz);
		hdrlen += hashsz;
	} अन्यथा {
		यदि (limit && hdrlen + datalen + hashsz >= limit) {
			यदि (st)
				odb_stream_close(st);
			मुक्त(buf);
			प्रत्यागम 0;
		}
		hashwrite(f, header, hdrlen);
	}
	यदि (st) {
		datalen = write_large_blob_data(st, f, &entry->idx.oid);
		odb_stream_close(st);
	} अन्यथा {
		hashwrite(f, buf, datalen);
		मुक्त(buf);
	}

	प्रत्यागम hdrlen + datalen;
}

/* Return 0 if we will bust the pack-size limit */
स्थिर off_t write_reuse_object(संरचना hashfile *f, संरचना object_entry *entry,
				unsigned long limit, पूर्णाङ्क usable_delta)
{
	संरचना packed_git *p = IN_PACK(entry);
	संरचना pack_window *w_curs = रिक्त;
	uint32_t pos;
	off_t offset, cur;
	गणना object_type type = oe_type(entry);
	गणना object_type in_pack_type;
	off_t datalen;
	unsigned वर्ण header[MAX_PACK_OBJECT_HEADER],
		      dheader[MAX_PACK_OBJECT_HEADER];
	unsigned hdrlen;
	ध्रुव unsigned hashsz = the_hash_algo->rawsz;
	size_t entry_size;

	cur = entry->in_pack_offset;
	in_pack_type = unpack_object_header(p, &w_curs, &cur, &entry_size);
	यदि (in_pack_type < 0)
		die(_("write_reuse_object: unable to parse object header of %s"),
		    oid_to_hex(&entry->idx.oid));

	यदि (DELTA(entry))
		type = (allow_ofs_delta && DELTA(entry)->idx.offset) ?
			OBJ_OFS_DELTA : OBJ_REF_DELTA;
	hdrlen = encode_in_pack_object_header(header, परिमाण(header),
					      type, entry_size);

	offset = entry->in_pack_offset;
	यदि (offset_to_pack_pos(p, offset, &pos) < 0)
		die(_("write_reuse_object: could not locate %s, expected at "
		      "offset %"PRIuMAX" in pack %s"),
		    oid_to_hex(&entry->idx.oid), (uintmax_t)offset,
		    p->pack_name);
	datalen = pack_pos_to_offset(p, pos + 1) - offset;
	यदि (!pack_to_stdout && p->index_version > 1 &&
	    check_pack_crc(p, &w_curs, offset, datalen,
			   pack_pos_to_index(p, pos))) {
		error(_("bad packed object CRC for %s"),
		      oid_to_hex(&entry->idx.oid));
		unuse_pack(&w_curs);
		प्रत्यागम write_no_reuse_object(f, entry, limit, usable_delta);
	}

	offset += entry->in_pack_header_size;
	datalen -= entry->in_pack_header_size;

	यदि (!pack_to_stdout && p->index_version == 1 &&
	    check_pack_inflate(p, &w_curs, offset, datalen, entry_size)) {
		error(_("corrupt packed object for %s"),
		      oid_to_hex(&entry->idx.oid));
		unuse_pack(&w_curs);
		प्रत्यागम write_no_reuse_object(f, entry, limit, usable_delta);
	}

	यदि (type == OBJ_OFS_DELTA) {
		off_t ofs = entry->idx.offset - DELTA(entry)->idx.offset;
		unsigned pos = परिमाण(dheader) - 1;
		dheader[pos] = ofs & 127;
		यावत् (ofs >>= 7)
			dheader[--pos] = 128 | (--ofs & 127);
		यदि (limit && hdrlen + परिमाण(dheader) - pos + datalen + hashsz >= limit) {
			unuse_pack(&w_curs);
			प्रत्यागम 0;
		}
		hashwrite(f, header, hdrlen);
		hashwrite(f, dheader + pos, परिमाण(dheader) - pos);
		hdrlen += परिमाण(dheader) - pos;
		reused_delta++;
	} अन्यथा यदि (type == OBJ_REF_DELTA) {
		यदि (limit && hdrlen + hashsz + datalen + hashsz >= limit) {
			unuse_pack(&w_curs);
			प्रत्यागम 0;
		}
		hashwrite(f, header, hdrlen);
		hashwrite(f, DELTA(entry)->idx.oid.hash, hashsz);
		hdrlen += hashsz;
		reused_delta++;
	} अन्यथा {
		यदि (limit && hdrlen + datalen + hashsz >= limit) {
			unuse_pack(&w_curs);
			प्रत्यागम 0;
		}
		hashwrite(f, header, hdrlen);
	}
	copy_pack_data(f, p, &w_curs, offset, datalen);
	unuse_pack(&w_curs);
	reused++;
	प्रत्यागम hdrlen + datalen;
}

/* Return 0 if we will bust the pack-size limit */
स्थिर off_t write_object(संरचना hashfile *f,
			  संरचना object_entry *entry,
			  off_t write_offset)
{
	unsigned long limit;
	off_t len;
	पूर्णाङ्क usable_delta, to_reuse;

	यदि (!pack_to_stdout)
		crc32_begin(f);

	/* apply size limit if limited packsize and not first object */
	यदि (!pack_size_limit || !nr_written)
		limit = 0;
	अन्यथा यदि (pack_size_limit <= write_offset)
		/*
		 * the earlier object did not fit the limit; avoid
		 * mistaking this with unlimited (i.e. limit = 0).
		 */
		limit = 1;
	अन्यथा
		limit = pack_size_limit - write_offset;

	यदि (!DELTA(entry))
		usable_delta = 0;	/* no delta */
	अन्यथा यदि (!pack_size_limit)
	       usable_delta = 1;	/* unlimited packfile */
	अन्यथा यदि (DELTA(entry)->idx.offset == (off_t)-1)
		usable_delta = 0;	/* base was written to another pack */
	अन्यथा यदि (DELTA(entry)->idx.offset)
		usable_delta = 1;	/* base already exists in this pack */
	अन्यथा
		usable_delta = 0;	/* base could end up in another pack */

	यदि (!reuse_object)
		to_reuse = 0;	/* explicit */
	अन्यथा यदि (!IN_PACK(entry))
		to_reuse = 0;	/* can't reuse what we don't have */
	अन्यथा यदि (oe_type(entry) == OBJ_REF_DELTA ||
		 oe_type(entry) == OBJ_OFS_DELTA)
				/* check_object() decided it for us ... */
		to_reuse = usable_delta;
				/* ... but pack split may override that */
	अन्यथा यदि (oe_type(entry) != entry->in_pack_type)
		to_reuse = 0;	/* pack has delta which is unusable */
	अन्यथा यदि (DELTA(entry))
		to_reuse = 0;	/* we want to pack afresh */
	अन्यथा
		to_reuse = 1;	/* we have it in-pack undeltified,
				 * and we do not need to deltify it.
				 */

	यदि (!to_reuse)
		len = write_no_reuse_object(f, entry, limit, usable_delta);
	अन्यथा
		len = write_reuse_object(f, entry, limit, usable_delta);
	यदि (!len)
		प्रत्यागम 0;

	यदि (usable_delta)
		written_delta++;
	written++;
	यदि (!pack_to_stdout)
		entry->idx.crc32 = crc32_end(f);
	प्रत्यागम len;
}

गणना write_one_status {
	WRITE_ONE_SKIP = -1, /* already written */
	WRITE_ONE_BREAK = 0, /* writing this will bust the limit; not written */
	WRITE_ONE_WRITTEN = 1, /* normal */
	WRITE_ONE_RECURSIVE = 2 /* already scheduled to be written */
};

स्थिर गणना write_one_status write_one(संरचना hashfile *f,
				       संरचना object_entry *e,
				       off_t *offset)
{
	off_t size;
	पूर्णाङ्क recursing;

	/*
	 * we set offset to 1 (which is an impossible value) to mark
	 * the fact that this object is involved in "write its base
	 * first before writing a deltified object" recursion.
	 */
	recursing = (e->idx.offset == 1);
	यदि (recursing) {
		warning(_("recursive delta detected for object %s"),
			oid_to_hex(&e->idx.oid));
		प्रत्यागम WRITE_ONE_RECURSIVE;
	} अन्यथा यदि (e->idx.offset || e->preferred_base) {
		/* offset is non zero if object is written already. */
		प्रत्यागम WRITE_ONE_SKIP;
	}

	/* if we are deltified, write out base object first. */
	यदि (DELTA(e)) {
		e->idx.offset = 1; /* now recurse */
		विकल्प (write_one(f, DELTA(e), offset)) {
		पक्ष WRITE_ONE_RECURSIVE:
			/* we cannot depend on this one */
			SET_DELTA(e, रिक्त);
			भङ्ग;
		स्वभाव:
			भङ्ग;
		पक्ष WRITE_ONE_BREAK:
			e->idx.offset = recursing;
			प्रत्यागम WRITE_ONE_BREAK;
		}
	}

	e->idx.offset = *offset;
	size = write_object(f, e, *offset);
	यदि (!size) {
		e->idx.offset = recursing;
		प्रत्यागम WRITE_ONE_BREAK;
	}
	written_list[nr_written++] = &e->idx;

	/* make sure off_t is sufficiently large not to wrap */
	यदि (signed_add_overflows(*offset, size))
		die(_("pack too large for current definition of off_t"));
	*offset += size;
	प्रत्यागम WRITE_ONE_WRITTEN;
}

स्थिर पूर्णाङ्क mark_tagged(ध्रुव संरचना reference *ref, शून्य *cb_data UNUSED)
{
	संरचना object_id peeled;
	संरचना object_entry *entry = packlist_find(&to_pack, ref->oid);

	यदि (entry)
		entry->tagged = 1;
	यदि (!reference_get_peeled_oid(the_repository, ref, &peeled)) {
		entry = packlist_find(&to_pack, &peeled);
		यदि (entry)
			entry->tagged = 1;
	}
	प्रत्यागम 0;
}

स्थिर अन्तःस्थ unsigned वर्ण oe_layer(संरचना packing_data *pack,
				     संरचना object_entry *e)
{
	यदि (!pack->layer)
		प्रत्यागम 0;
	प्रत्यागम pack->layer[e - pack->objects];
}

स्थिर अन्तःस्थ शून्य add_to_write_order(संरचना object_entry **wo,
			       unsigned पूर्णाङ्क *endp,
			       संरचना object_entry *e)
{
	यदि (e->filled || oe_layer(&to_pack, e) != write_layer)
		प्रत्यागम;
	wo[(*endp)++] = e;
	e->filled = 1;
}

स्थिर शून्य add_descendants_to_write_order(संरचना object_entry **wo,
					   unsigned पूर्णाङ्क *endp,
					   संरचना object_entry *e)
{
	पूर्णाङ्क add_to_order = 1;
	यावत् (e) {
		यदि (add_to_order) {
			संरचना object_entry *s;
			/* add this node... */
			add_to_write_order(wo, endp, e);
			/* all its siblings... */
			प्रति (s = DELTA_SIBLING(e); s; s = DELTA_SIBLING(s)) {
				add_to_write_order(wo, endp, s);
			}
		}
		/* drop down a level to add left subtree nodes if possible */
		यदि (DELTA_CHILD(e)) {
			add_to_order = 1;
			e = DELTA_CHILD(e);
		} अन्यथा {
			add_to_order = 0;
			/* our sibling might have some children, it is next */
			यदि (DELTA_SIBLING(e)) {
				e = DELTA_SIBLING(e);
				अनुवर्त;
			}
			/* go back to our parent node */
			e = DELTA(e);
			यावत् (e && !DELTA_SIBLING(e)) {
				/* we're on the right side of a subtree, keep
				 * going up until we can go right again */
				e = DELTA(e);
			}
			यदि (!e) {
				/* done- we hit our original root node */
				प्रत्यागम;
			}
			/* pass it off to sibling at this level */
			e = DELTA_SIBLING(e);
		}
	};
}

स्थिर शून्य add_family_to_write_order(संरचना object_entry **wo,
				      unsigned पूर्णाङ्क *endp,
				      संरचना object_entry *e)
{
	संरचना object_entry *मूळ;

	प्रति (मूळ = e; DELTA(मूळ); मूळ = DELTA(मूळ))
		; /* nothing */
	add_descendants_to_write_order(wo, endp, मूळ);
}

स्थिर शून्य compute_layer_order(संरचना object_entry **wo, unsigned पूर्णाङ्क *wo_end)
{
	unsigned पूर्णाङ्क i, last_untagged;
	संरचना object_entry *objects = to_pack.objects;

	प्रति (i = 0; i < to_pack.nr_objects; i++) {
		यदि (objects[i].tagged)
			भङ्ग;
		add_to_write_order(wo, wo_end, &objects[i]);
	}
	last_untagged = i;

	/*
	 * Then fill all the tagged tips.
	 */
	प्रति (; i < to_pack.nr_objects; i++) {
		यदि (objects[i].tagged)
			add_to_write_order(wo, wo_end, &objects[i]);
	}

	/*
	 * And then all remaining commits and tags.
	 */
	प्रति (i = last_untagged; i < to_pack.nr_objects; i++) {
		यदि (oe_type(&objects[i]) != OBJ_COMMIT &&
		    oe_type(&objects[i]) != OBJ_TAG)
			अनुवर्त;
		add_to_write_order(wo, wo_end, &objects[i]);
	}

	/*
	 * And then all the trees.
	 */
	प्रति (i = last_untagged; i < to_pack.nr_objects; i++) {
		यदि (oe_type(&objects[i]) != OBJ_वृक्ष)
			अनुवर्त;
		add_to_write_order(wo, wo_end, &objects[i]);
	}

	/*
	 * Finally all the rest in really tight order
	 */
	प्रति (i = last_untagged; i < to_pack.nr_objects; i++) {
		यदि (!objects[i].filled && oe_layer(&to_pack, &objects[i]) == write_layer)
			add_family_to_write_order(wo, wo_end, &objects[i]);
	}
}

स्थिर संरचना object_entry **compute_write_order(शून्य)
{
	uint32_t max_layers = 1;
	unsigned पूर्णाङ्क i, wo_end;

	संरचना object_entry **wo;
	संरचना object_entry *objects = to_pack.objects;

	प्रति (i = 0; i < to_pack.nr_objects; i++) {
		objects[i].tagged = 0;
		objects[i].filled = 0;
		SET_DELTA_CHILD(&objects[i], रिक्त);
		SET_DELTA_SIBLING(&objects[i], रिक्त);
	}

	/*
	 * Fully connect delta_child/delta_sibling network.
	 * Make sure delta_sibling is sorted in the original
	 * recency order.
	 */
	प्रति (i = to_pack.nr_objects; i > 0;) {
		संरचना object_entry *e = &objects[--i];
		यदि (!DELTA(e))
			अनुवर्त;
		/* Mark me as the first child */
		e->delta_sibling_idx = DELTA(e)->delta_child_idx;
		SET_DELTA_CHILD(DELTA(e), e);
	}

	/*
	 * Mark objects that are at the tip of tags.
	 */
	refs_for_each_tag_ref(get_main_ref_store(the_repository), mark_tagged,
			      रिक्त);

	यदि (use_delta_islands) {
		max_layers = compute_pack_layers(&to_pack);
		free_island_marks();
	}

	ALLOC_ARRAY(wo, to_pack.nr_objects);
	wo_end = 0;

	प्रति (; write_layer < max_layers; ++write_layer)
		compute_layer_order(wo, &wo_end);

	यदि (wo_end != to_pack.nr_objects)
		die(_("ordered %u objects, expected %"PRIu32),
		    wo_end, to_pack.nr_objects);

	प्रत्यागम wo;
}


/*
 * A reused set of objects. All objects in a chunk have the same
 * relative position in the original packfile and the generated
 * packfile.
 */

स्थिर संरचना reused_chunk {
	/* The offset of the first object of this chunk in the original
	 * packfile. */
	off_t original;
	/* The difference for "original" minus the offset of the first object of
	 * this chunk in the generated packfile. */
	off_t difference;
} *reused_chunks;
स्थिर पूर्णाङ्क reused_chunks_nr;
स्थिर पूर्णाङ्क reused_chunks_alloc;

स्थिर शून्य record_reused_object(off_t where, off_t offset)
{
	यदि (reused_chunks_nr && reused_chunks[reused_chunks_nr-1].difference == offset)
		प्रत्यागम;

	ALLOC_GROW(reused_chunks, reused_chunks_nr + 1,
		   reused_chunks_alloc);
	reused_chunks[reused_chunks_nr].original = where;
	reused_chunks[reused_chunks_nr].difference = offset;
	reused_chunks_nr++;
}

/*
 * Binary search to find the chunk that "where" is in. Note
 * that we're not looking for an exact match, just the first
 * chunk that contains it (which implicitly ends at the start
 * of the next chunk.
 */
स्थिर off_t find_reused_offset(off_t where)
{
	पूर्णाङ्क lo = 0, hi = reused_chunks_nr;
	यावत् (lo < hi) {
		पूर्णाङ्क mi = lo + ((hi - lo) / 2);
		यदि (where == reused_chunks[mi].original)
			प्रत्यागम reused_chunks[mi].difference;
		यदि (where < reused_chunks[mi].original)
			hi = mi;
		अन्यथा
			lo = mi + 1;
	}

	/*
	 * The first chunk starts at zero, so we can't have gone below
	 * there.
	 */
	प्रतिज्ञा(lo);
	प्रत्यागम reused_chunks[lo-1].difference;
}

स्थिर शून्य write_reused_pack_one(संरचना packed_git *reuse_packfile,
				  size_t pos, संरचना hashfile *out,
				  off_t pack_start,
				  संरचना pack_window **w_curs)
{
	off_t offset, next, cur;
	गणना object_type type;
	size_t size;

	offset = pack_pos_to_offset(reuse_packfile, pos);
	next = pack_pos_to_offset(reuse_packfile, pos + 1);

	record_reused_object(offset,
			     offset - (hashfile_total(out) - pack_start));

	cur = offset;
	type = unpack_object_header(reuse_packfile, w_curs, &cur, &size);
	प्रतिज्ञा(type >= 0);

	यदि (type == OBJ_OFS_DELTA) {
		off_t base_offset;
		off_t fixup;

		unsigned वर्ण header[MAX_PACK_OBJECT_HEADER];
		unsigned len;

		base_offset = get_delta_base(reuse_packfile, w_curs, &cur, type, offset);
		प्रतिज्ञा(base_offset != 0);

		/* Convert to REF_DELTA if we must... */
		यदि (!allow_ofs_delta) {
			uint32_t base_pos;
			संरचना object_id base_oid;

			यदि (offset_to_pack_pos(reuse_packfile, base_offset, &base_pos) < 0)
				die(_("expected object at offset %"PRIuMAX" "
				      "in pack %s"),
				    (uintmax_t)base_offset,
				    reuse_packfile->pack_name);

			nth_packed_object_id(&base_oid, reuse_packfile,
					     pack_pos_to_index(reuse_packfile, base_pos));

			len = encode_in_pack_object_header(header, परिमाण(header),
							   OBJ_REF_DELTA, size);
			hashwrite(out, header, len);
			hashwrite(out, base_oid.hash, the_hash_algo->rawsz);
			copy_pack_data(out, reuse_packfile, w_curs, cur, next - cur);
			प्रत्यागम;
		}

		/* Otherwise see if we need to rewrite the offset... */
		fixup = find_reused_offset(offset) -
			find_reused_offset(base_offset);
		यदि (fixup) {
			unsigned वर्ण ofs_header[MAX_PACK_OBJECT_HEADER];
			unsigned i, ofs_len;
			off_t ofs = offset - base_offset - fixup;

			len = encode_in_pack_object_header(header, परिमाण(header),
							   OBJ_OFS_DELTA, size);

			i = परिमाण(ofs_header) - 1;
			ofs_header[i] = ofs & 127;
			यावत् (ofs >>= 7)
				ofs_header[--i] = 128 | (--ofs & 127);

			ofs_len = परिमाण(ofs_header) - i;

			hashwrite(out, header, len);
			hashwrite(out, ofs_header + परिमाण(ofs_header) - ofs_len, ofs_len);
			copy_pack_data(out, reuse_packfile, w_curs, cur, next - cur);
			प्रत्यागम;
		}

		/* ...otherwise we have no fixup, and can write it verbatim */
	}

	copy_pack_data(out, reuse_packfile, w_curs, offset, next - offset);
}

स्थिर size_t write_reused_pack_verbatim(संरचना bitmapped_pack *reuse_packfile,
					 संरचना hashfile *out,
					 संरचना pack_window **w_curs)
{
	size_t pos = 0;
	size_t end;

	यदि (reuse_packfile->bitmap_pos) {
		/*
		 * We can't reuse whole chunks verbatim out of
		 * non-preferred packs since we can't guarantee that
		 * all duplicate objects were resolved in favor of
		 * that pack.
		 *
		 * Even if we have a whole eword_t worth of bits that
		 * could be reused, there may be objects between the
		 * objects corresponding to the first and last bit of
		 * that word which were selected from a different
		 * pack, causing us to send duplicate or unwanted
		 * objects.
		 *
		 * Handle non-preferred packs from within
		 * write_reused_pack(), which inspects and reuses
		 * individual bits.
		 */
		प्रत्यागम reuse_packfile->bitmap_pos / BITS_IN_EWORD;
	}

	/*
	 * Only read through the last word whose bits all correspond
	 * to objects in the given packfile, since we must stop at a
	 * word boundary.
	 *
	 * If there is no whole word to read (i.e. the packfile
	 * contains fewer than BITS_IN_EWORD objects), then we'll
	 * inspect bits one-by-one in write_reused_pack().
	 */
	end = reuse_packfile->bitmap_nr / BITS_IN_EWORD;
	यदि (reuse_packfile_bitmap->word_alloc < end)
		BUG("fewer words than expected in reuse_packfile_bitmap");

	यावत् (pos < end && reuse_packfile_bitmap->शब्द[pos] == (eword_t)~0)
		pos++;

	यदि (pos) {
		off_t to_write;

		written = (pos * BITS_IN_EWORD);
		to_write = pack_pos_to_offset(reuse_packfile->p, written)
			- परिमाण(संरचना pack_header);

		/* We're recording one chunk, not one object. */
		record_reused_object(परिमाण(संरचना pack_header), 0);
		hashflush(out);
		copy_pack_data(out, reuse_packfile->p, w_curs,
			परिमाण(संरचना pack_header), to_write);

		display_progress(progress_state, written);
	}
	प्रत्यागम pos;
}

स्थिर शून्य write_reused_pack(संरचना bitmapped_pack *reuse_packfile,
			      संरचना hashfile *f)
{
	size_t i = reuse_packfile->bitmap_pos / BITS_IN_EWORD;
	uint32_t offset;
	off_t pack_start = hashfile_total(f) - परिमाण(संरचना pack_header);
	संरचना pack_window *w_curs = रिक्त;

	यदि (allow_ofs_delta)
		i = write_reused_pack_verbatim(reuse_packfile, f, &w_curs);

	प्रति (; i < reuse_packfile_bitmap->word_alloc; ++i) {
		eword_t word = reuse_packfile_bitmap->शब्द[i];
		size_t pos = (i * BITS_IN_EWORD);

		प्रति (offset = 0; offset < BITS_IN_EWORD; ++offset) {
			uint32_t pack_pos;
			यदि ((word >> offset) == 0)
				भङ्ग;

			offset += ewah_bit_ctz64(word >> offset);
			यदि (pos + offset < reuse_packfile->bitmap_pos)
				अनुवर्त;
			यदि (pos + offset >= reuse_packfile->bitmap_pos + reuse_packfile->bitmap_nr)
				गच्छ done;

			यदि (reuse_packfile->bitmap_pos) {
				/*
				 * When doing multi-pack reuse on a
				 * non-preferred pack, translate bit positions
				 * from the MIDX pseudo-pack order back to their
				 * pack-relative positions before attempting
				 * reuse.
				 */
				संरचना multi_pack_index *m = reuse_packfile->from_midx;
				uint32_t midx_pos;
				off_t pack_ofs;

				यदि (!m)
					BUG("non-zero bitmap position without MIDX");

				midx_pos = pack_pos_to_midx(m, pos + offset);
				pack_ofs = nth_midxed_offset(m, midx_pos);

				यदि (offset_to_pack_pos(reuse_packfile->p,
						       pack_ofs, &pack_pos) < 0)
					BUG("could not find expected object at offset %"PRIuMAX" in pack %s",
					    (uintmax_t)pack_ofs,
					    pack_basename(reuse_packfile->p));
			} अन्यथा {
				/*
				 * Can use bit positions directly, even for MIDX
				 * bitmaps. See comment in try_partial_reuse()
				 * for why.
				 */
				pack_pos = pos + offset;
			}

			write_reused_pack_one(reuse_packfile->p, pack_pos, f,
					      pack_start, &w_curs);
			display_progress(progress_state, ++written);
		}
	}

done:
	unuse_pack(&w_curs);
}

स्थिर शून्य write_excluded_by_configs(शून्य)
{
	संरचना oidset_iter iter;
	ध्रुव संरचना object_id *oid;

	oidset_iter_init(&excluded_by_config, &iter);
	यावत् ((oid = oidset_iter_next(&iter))) {
		संरचना configured_exclusion *ex =
			oidmap_get(&configured_exclusions, oid);

		यदि (!ex)
			BUG("configured exclusion wasn't configured");
		write_in_full(1, ex->pack_hash_hex, strlen(ex->pack_hash_hex));
		write_in_full(1, " ", 1);
		write_in_full(1, ex->uri, strlen(ex->uri));
		write_in_full(1, "\n", 1);
	}
}

स्थिर ध्रुव वर्ण no_split_warning[] = N_(
"disabling bitmap writing, packs are split due to pack.packSizeLimit"
);

स्थिर शून्य write_pack_file(शून्य)
{
	uint32_t i = 0, j;
	संरचना hashfile *f;
	off_t offset;
	uint32_t nr_remaining = nr_result;
	time_t last_mtime = 0;
	संरचना object_entry **write_order;
	off_t bytes_written = 0;

	यदि (progress > pack_to_stdout)
		progress_state = start_progress(the_repository,
						_("Writing objects"), nr_result);
	ALLOC_ARRAY(written_list, to_pack.nr_objects);
	write_order = compute_write_order();

	कुरु {
		unsigned वर्ण hash[GIT_MAX_RAWSZ];
		वर्ण *pack_tmp_name = रिक्त;

		यदि (pack_to_stdout) {
			/*
			 * This command is most often invoked via
			 * git-upload-pack(1), which will typically chunk data
			 * into pktlines. As such, we use the maximum data
			 * length of them as buffer length.
			 *
			 * Note that we need to subtract one though to
			 * accommodate for the sideband byte.
			 */
			संरचना hashfd_options opts = {
				.progress = progress_state,
				.buffer_len = LARGE_PACKET_DATA_MAX - 1,
			};
			f = hashfd_ext(the_repository->hash_algo, 1,
				       "<stdout>", &opts);
		} अन्यथा {
			f = create_tmp_packfile(the_repository, &pack_tmp_name);
		}

		offset = write_pack_header(f, nr_remaining);

		यदि (reuse_packfiles_nr) {
			प्रतिज्ञा(pack_to_stdout);
			प्रति (j = 0; j < reuse_packfiles_nr; j++) {
				reused_chunks_nr = 0;
				write_reused_pack(&reuse_packfiles[j], f);
				यदि (reused_chunks_nr)
					reuse_packfiles_used_nr++;
			}
			offset = hashfile_total(f);
		}

		nr_written = 0;
		प्रति (; i < to_pack.nr_objects; i++) {
			संरचना object_entry *e = write_order[i];
			यदि (write_one(f, e, &offset) == WRITE_ONE_BREAK)
				भङ्ग;
			display_progress(progress_state, written);
		}

		bytes_written += hashfile_total(f) +
			the_repository->hash_algo->rawsz;
		यदि (pack_to_stdout) {
			/*
			 * We never fsync when writing to stdout since we may
			 * not be writing to an actual pack file. For instance,
			 * the upload-pack code passes a pipe here. Calling
			 * fsync on a pipe results in unnecessary
			 * synchronization with the reader on some platforms.
			 */
			finalize_hashfile(f, hash, FSYNC_COMPONENT_NONE,
					  CSUM_HASH_IN_STREAM | CSUM_CLOSE);
		} अन्यथा यदि (nr_written == nr_remaining) {
			finalize_hashfile(f, hash, FSYNC_COMPONENT_PACK,
					  CSUM_HASH_IN_STREAM | CSUM_FSYNC | CSUM_CLOSE);
		} अन्यथा {
			/*
			 * If we wrote the wrong number of entries in the
			 * header, rewrite it like in fast-import.
			 */

			पूर्णाङ्क fd = finalize_hashfile(f, hash, FSYNC_COMPONENT_PACK, 0);
			fixup_pack_header_footer(the_hash_algo, fd, hash,
						 pack_tmp_name, nr_written,
						 hash, offset);
			close(fd);
			यदि (write_bitmap_index) {
				यदि (write_bitmap_index != WRITE_BITMAP_QUIET)
					warning(_(no_split_warning));
				write_bitmap_index = 0;
			}
		}

		यदि (!pack_to_stdout) {
			संरचना stat st;
			संरचना strbuf tmpname = STRBUF_INIT;
			संरचना bitmap_writer bitmap_writer;
			वर्ण *idx_tmp_name = रिक्त;

			/*
			 * Packs are runtime accessed in their mtime
			 * order since newer packs are more likely to contain
			 * younger objects.  So if we are creating multiple
			 * packs then we should modify the mtime of later ones
			 * to preserve this property.
			 */
			यदि (stat(pack_tmp_name, &st) < 0) {
				warning_errno(_("failed to stat %s"), pack_tmp_name);
			} अन्यथा यदि (!last_mtime) {
				last_mtime = st.st_mtime;
			} अन्यथा {
				संरचना utimbuf utb;
				utb.actime = st.st_atime;
				utb.modtime = --last_mtime;
				यदि (utime(pack_tmp_name, &utb) < 0)
					warning_errno(_("failed utime() on %s"), pack_tmp_name);
			}

			strbuf_addf(&tmpname, "%s-%s.", base_name,
				    hash_to_hex(hash));

			यदि (write_bitmap_index) {
				bitmap_writer_init(&bitmap_writer,
						   the_repository, &to_pack,
						   रिक्त);
				bitmap_writer_set_checksum(&bitmap_writer, hash);
				bitmap_writer_build_type_index(&bitmap_writer,
							       written_list);
			}

			यदि (cruft)
				pack_idx_opts.flags |= WRITE_MTIMES;

			stage_tmp_packfiles(the_repository, &tmpname,
					    pack_tmp_name, written_list,
					    nr_written, &to_pack,
					    &pack_idx_opts, hash,
					    &idx_tmp_name);

			यदि (write_bitmap_index) {
				size_t tmpname_len = tmpname.len;

				strbuf_addstr(&tmpname, "bitmap");
				stop_progress(&progress_state);

				bitmap_writer_show_progress(&bitmap_writer,
							    progress);
				bitmap_writer_select_commits(&bitmap_writer,
							     indexed_commits,
							     indexed_commits_nr);
				यदि (bitmap_writer_build(&bitmap_writer) < 0)
					die(_("failed to write bitmap index"));
				bitmap_writer_finish(&bitmap_writer,
						     written_list,
						     tmpname.buf, write_bitmap_options);
				bitmap_writer_free(&bitmap_writer);
				write_bitmap_index = 0;
				strbuf_setlen(&tmpname, tmpname_len);
			}

			rename_tmp_packfile_idx(the_repository, &tmpname, &idx_tmp_name);

			मुक्त(idx_tmp_name);
			strbuf_release(&tmpname);
			मुक्त(pack_tmp_name);
			puts(hash_to_hex(hash));
		}

		/* mark written objects as written to previous pack */
		प्रति (j = 0; j < nr_written; j++) {
			written_list[j]->offset = (off_t)-1;
		}
		nr_remaining -= nr_written;
	} यावत् (nr_remaining && i < to_pack.nr_objects);

	मुक्त(written_list);
	मुक्त(write_order);
	stop_progress(&progress_state);
	यदि (written != nr_result)
		die(_("wrote %"PRIu32" objects while expecting %"PRIu32),
		    written, nr_result);
	trace2_data_intmax("pack-objects", the_repository,
			   "write_pack_file/wrote", nr_result);
	trace2_data_intmax("pack-objects", the_repository,
			   "write_pack_file/wrote_bytes", bytes_written);
}

स्थिर पूर्णाङ्क no_try_delta(ध्रुव वर्ण *path)
{
	स्थिर संरचना attr_check *check;

	यदि (!check)
		check = attr_check_initl("delta", रिक्त);
	git_check_attr(the_repository->index, path, check);
	यदि (ATTR_FALSE(check->वस्तुसंच[0].मूल्य))
		प्रत्यागम 1;
	प्रत्यागम 0;
}

/*
 * When adding an object, check whether we have already added it
 * to our packing list. If so, we can skip. However, if we are
 * being asked to excludei t, but the previous mention was to include
 * it, make sure to adjust its flags and tweak our numbers accordingly.
 *
 * As an optimization, we pass out the index position where we would have
 * found the item, since that saves us from having to look it up again a
 * few lines later when we want to add the new entry.
 */
स्थिर पूर्णाङ्क have_duplicate_entry(ध्रुव संरचना object_id *oid,
				पूर्णाङ्क exclude)
{
	संरचना object_entry *entry;

	यदि (reuse_packfile_bitmap &&
	    bitmap_walk_contains(bitmap_git, reuse_packfile_bitmap, oid))
		प्रत्यागम 1;

	entry = packlist_find(&to_pack, oid);
	यदि (!entry)
		प्रत्यागम 0;

	यदि (exclude) {
		यदि (!entry->preferred_base)
			nr_result--;
		entry->preferred_base = 1;
	}

	प्रत्यागम 1;
}

स्थिर पूर्णाङ्क want_cruft_object_mtime(संरचना repository *r,
				   ध्रुव संरचना object_id *oid,
				   unsigned flags, uint32_t mtime)
{
	संरचना odb_source_files *files = odb_source_files_downcast(r->objects->source);

	प्रति (संरचना odb_files_dir *dir = files->dirs; dir; dir = dir->next) {
		संरचना packed_git **cache = packfile_store_get_kept_pack_cache(dir->packed, flags);

		प्रति (; *cache; cache++) {
			संरचना packed_git *p = *cache;
			off_t ofs;
			uint32_t candidate_mtime;

			ofs = find_pack_entry_one(oid, p);
			यदि (!ofs)
				अनुवर्त;

			/*
			 * We have a copy of the object 'oid' in a non-cruft
			 * pack. We can avoid packing an additional copy
			 * regardless of what the existing copy's mtime is since
			 * it is outside of a cruft pack.
			 */
			यदि (!p->is_cruft)
				प्रत्यागम 0;

			/*
			 * If we have a copy of the object 'oid' in a cruft
			 * pack, then either read the cruft pack's mtime for
			 * that object, or, if that can't be loaded, assume the
			 * pack's mtime itself.
			 */
			यदि (!load_pack_mtimes(p)) {
				uint32_t pos;
				यदि (offset_to_pack_pos(p, ofs, &pos) < 0)
					अनुवर्त;
				candidate_mtime = nth_packed_mtime(p, pos);
			} अन्यथा {
				candidate_mtime = p->mtime;
			}

			/*
			 * We have a surviving copy of the object in a cruft
			 * pack whose mtime is greater than or equal to the one
			 * we are considering. We can thus avoid packing an
			 * additional copy of that object.
			 */
			यदि (mtime <= candidate_mtime)
				प्रत्यागम 0;
		}
	}

	प्रत्यागम -1;
}

स्थिर पूर्णाङ्क want_found_object(ध्रुव संरचना object_id *oid, पूर्णाङ्क exclude,
			     संरचना packed_git *p, uint32_t mtime)
{
	यदि (exclude)
		प्रत्यागम 1;
	यदि (incremental)
		प्रत्यागम 0;

	यदि (!is_pack_valid(p))
		प्रत्यागम -1;

	/*
	 * When asked to do --local (do not include an object that appears in a
	 * pack we borrow from elsewhere) or --honor-pack-keep (do not include
	 * an object that appears in a pack marked with .keep), finding a pack
	 * that matches the criteria is sufficient for us to decide to omit it.
	 * However, even if this pack does not satisfy the criteria, we need to
	 * make sure no copy of this object appears in _any_ pack that makes us
	 * to omit the object, so we need to check all the packs.
	 *
	 * We can however first check whether these options can possibly matter;
	 * if they do not matter we know we want the object in generated pack.
	 * Otherwise, we signal "-1" at the end to tell the caller that we do
	 * not know either way, and it needs to check more packs.
	 */

	/*
	 * Objects in packs borrowed from elsewhere are discarded regardless of
	 * if they appear in other packs that weren't borrowed.
	 */
	यदि (local && !p->pack_local)
		प्रत्यागम 0;

	/*
	 * Then handle .keep first, as we have a fast(er) path there.
	 */
	यदि (ignore_packed_keep_on_disk || ignore_packed_keep_in_core ||
	    ignore_packed_keep_in_core_open) {
		/*
		 * Set the flags for the kept-pack cache to be the ones we want
		 * to ignore.
		 *
		 * That is, if we are ignoring objects in on-disk keep packs,
		 * then we want to search through the on-disk keep and ignore
		 * the in-core ones.
		 */
		unsigned flags = 0;
		यदि (ignore_packed_keep_on_disk)
			flags |= KEPT_PACK_ON_DISK;
		यदि (ignore_packed_keep_in_core)
			flags |= KEPT_PACK_IN_CORE;
		यदि (ignore_packed_keep_in_core_open)
			flags |= KEPT_PACK_IN_CORE_OPEN;

		/*
		 * If the object is in a pack that we want to ignore, *and* we
		 * don't have any cruft packs that are being retained, we can
		 * abort quickly.
		 */
		यदि (!ignore_packed_keep_in_core_has_cruft) {
			यदि (ignore_packed_keep_on_disk && p->pack_keep)
				प्रत्यागम 0;
			यदि (ignore_packed_keep_in_core && p->pack_keep_in_core)
				प्रत्यागम 0;
			यदि (ignore_packed_keep_in_core_open && p->pack_keep_in_core_open)
				प्रत्यागम 0;
			यदि (has_object_kept_pack(p->repo, oid, flags))
				प्रत्यागम 0;
		} अन्यथा {
			/*
			 * But if there is at least one cruft pack which
			 * is being kept, we only want to include the
			 * provided object if it has a strictly greater
			 * mtime than any existing cruft copy.
			 */
			यदि (!want_cruft_object_mtime(p->repo, oid, flags,
						     mtime))
				प्रत्यागम 0;
		}
	}

	/*
	 * At this point we know definitively that either we don't care about
	 * keep-packs, or the object is not in one. Keep checking other
	 * conditions...
	 */
	यदि (!local || !have_non_local_packs)
		प्रत्यागम 1;

	/* we don't know yet; keep looking for more packs */
	प्रत्यागम -1;
}

स्थिर पूर्णाङ्क want_object_in_pack_one(संरचना packed_git *p,
				   ध्रुव संरचना object_id *oid,
				   पूर्णाङ्क exclude,
				   संरचना packed_git **found_pack,
				   off_t *found_offset,
				   uint32_t found_mtime)
{
	off_t offset;

	यदि (p == *found_pack)
		offset = *found_offset;
	अन्यथा
		offset = find_pack_entry_one(oid, p);

	यदि (offset) {
		यदि (!*found_pack) {
			यदि (!is_pack_valid(p))
				प्रत्यागम -1;
			*found_offset = offset;
			*found_pack = p;
		}
		प्रत्यागम want_found_object(oid, exclude, p, found_mtime);
	}
	प्रत्यागम -1;
}

/*
 * Check whether we want the object in the pack (e.g., we do not want
 * objects found in non-local stores if the "--local" option was used).
 *
 * If the caller already knows an existing pack it wants to take the object
 * from, that is passed in *found_pack and *found_offset; otherwise this
 * function finds if there is any pack that has the object and returns the pack
 * and its offset in these variables.
 */
स्थिर पूर्णाङ्क want_object_in_pack_mtime(ध्रुव संरचना object_id *oid,
				     पूर्णाङ्क exclude,
				     संरचना packed_git **found_pack,
				     off_t *found_offset,
				     uint32_t found_mtime)
{
	संरचना odb_source_files *files =
		odb_source_files_downcast(the_repository->objects->source);
	पूर्णाङ्क want;
	संरचना packfile_list_entry *e;

	यदि (!exclude && local) {
		/*
		 * Note that we start iterating at `dirs->next` so that we
		 * skip the local object directory.
		 */
		प्रति (संरचना odb_files_dir *dir = files->dirs->next; dir; dir = dir->next)
			यदि (!odb_source_read_object_info(&dir->loose->base, oid, रिक्त, 0, रिक्त))
				प्रत्यागम 0;
	}

	/*
	 * If we already know the pack object lives in, start checks from that
	 * pack - in the usual case when neither --local was given nor .keep files
	 * are present we will determine the answer right now.
	 */
	यदि (*found_pack) {
		want = want_found_object(oid, exclude, *found_pack,
					 found_mtime);
		यदि (want != -1)
			प्रत्यागम want;

		*found_pack = रिक्त;
		*found_offset = 0;
	}

	प्रति (संरचना odb_files_dir *dir = files->dirs; dir; dir = dir->next) {
		संरचना multi_pack_index *m = get_multi_pack_index(dir->packed);
		संरचना pack_entry e;

		यदि (m && midx_fill_entry(m, oid, &e, रिक्त) == MIDX_FILL_HIT) {
			want = want_object_in_pack_one(e.p, oid, exclude, found_pack, found_offset, found_mtime);
			यदि (want != -1)
				प्रत्यागम want;
		}
	}

	प्रति (संरचना odb_files_dir *dir = files->dirs; dir; dir = dir->next) {
		प्रति (e = dir->packed->packs.head; e; e = e->next) {
			संरचना packed_git *p = e->pack;
			want = want_object_in_pack_one(p, oid, exclude, found_pack, found_offset, found_mtime);
			यदि (!exclude && want > 0)
				packfile_list_prepend(&dir->packed->packs, p);
			यदि (want != -1)
				प्रत्यागम want;
		}
	}

	यदि (uri_protocols.nr) {
		संरचना configured_exclusion *ex =
			oidmap_get(&configured_exclusions, oid);
		पूर्णाङ्क i;
		ध्रुव वर्ण *p;

		यदि (ex) {
			प्रति (i = 0; i < uri_protocols.nr; i++) {
				यदि (skip_prefix(ex->uri,
						uri_protocols.वस्तुसंच[i].string,
						&p) &&
				    *p == ':') {
					oidset_घाला(&excluded_by_config, oid);
					प्रत्यागम 0;
				}
			}
		}
	}

	प्रत्यागम 1;
}

स्थिर अन्तःस्थ पूर्णाङ्क want_object_in_pack(ध्रुव संरचना object_id *oid,
				      पूर्णाङ्क exclude,
				      संरचना packed_git **found_pack,
				      off_t *found_offset)
{
	प्रत्यागम want_object_in_pack_mtime(oid, exclude, found_pack, found_offset,
					 0);
}

स्थिर संरचना object_entry *create_object_entry(ध्रुव संरचना object_id *oid,
						गणना object_type type,
						uint32_t hash,
						पूर्णाङ्क exclude,
						पूर्णाङ्क no_try_delta,
						संरचना packed_git *found_pack,
						off_t found_offset)
{
	संरचना object_entry *entry;

	entry = packlist_alloc(&to_pack, oid);
	entry->hash = hash;
	oe_set_type(entry, type);
	यदि (exclude)
		entry->preferred_base = 1;
	अन्यथा
		nr_result++;
	यदि (found_pack) {
		oe_set_in_pack(&to_pack, entry, found_pack);
		entry->in_pack_offset = found_offset;
	}

	entry->no_try_delta = no_try_delta;

	प्रत्यागम entry;
}

स्थिर ध्रुव वर्ण no_closure_warning[] = N_(
"disabling bitmap writing, as some objects are not being packed"
);

स्थिर शून्य add_object_entry(ध्रुव संरचना object_id *oid, गणना object_type type,
			     ध्रुव वर्ण *name, पूर्णाङ्क exclude)
{
	संरचना packed_git *found_pack = रिक्त;
	off_t found_offset = 0;

	display_progress(progress_state, ++nr_seen);

	यदि (have_duplicate_entry(oid, exclude))
		प्रत्यागम;

	यदि (!want_object_in_pack(oid, exclude, &found_pack, &found_offset)) {
		/* The pack is missing an object, so it will not have closure */
		यदि (write_bitmap_index) {
			यदि (write_bitmap_index != WRITE_BITMAP_QUIET)
				warning(_(no_closure_warning));
			write_bitmap_index = 0;
		}
		प्रत्यागम;
	}

	create_object_entry(oid, type, pack_name_hash_fn(name),
			    exclude, name && no_try_delta(name),
			    found_pack, found_offset);
}

स्थिर पूर्णाङ्क add_object_entry_from_bitmap(ध्रुव संरचना object_id *oid,
					गणना object_type type,
					पूर्णाङ्क flags UNUSED, uint32_t name_hash,
					संरचना packed_git *pack, off_t offset,
					शून्य *payload UNUSED)
{
	display_progress(progress_state, ++nr_seen);

	यदि (have_duplicate_entry(oid, 0))
		प्रत्यागम 0;

	यदि (!want_object_in_pack(oid, 0, &pack, &offset))
		प्रत्यागम 0;

	create_object_entry(oid, type, name_hash, 0, 0, pack, offset);
	प्रत्यागम 0;
}

संरचना pbase_वृक्ष_cache {
	संरचना object_id oid;
	पूर्णाङ्क ref;
	पूर्णाङ्क temporary;
	शून्य *वृक्ष_data;
	unsigned long वृक्ष_size;
};

स्थिर संरचना pbase_वृक्ष_cache *(pbase_वृक्ष_cache[256]);
स्थिर पूर्णाङ्क pbase_वृक्ष_cache_ix(ध्रुव संरचना object_id *oid)
{
	प्रत्यागम oid->hash[0] % ARRAY_SIZE(pbase_वृक्ष_cache);
}
स्थिर पूर्णाङ्क pbase_वृक्ष_cache_ix_incr(पूर्णाङ्क ix)
{
	प्रत्यागम (ix+1) % ARRAY_SIZE(pbase_वृक्ष_cache);
}

स्थिर संरचना pbase_वृक्ष {
	संरचना pbase_वृक्ष *next;
	/* This is a phony "cache" entry; we are not
	 * going to evict it or find it through _get()
	 * mechanism -- this is for the toplevel node that
	 * would almost always change with any commit.
	 */
	संरचना pbase_वृक्ष_cache pcache;
} *pbase_वृक्ष;

स्थिर संरचना pbase_वृक्ष_cache *pbase_वृक्ष_get(ध्रुव संरचना object_id *oid)
{
	संरचना pbase_वृक्ष_cache *ent, *nent;
	शून्य *data;
	unsigned long size;
	size_t size_st = 0;
	गणना object_type type;
	पूर्णाङ्क neigh;
	पूर्णाङ्क my_ix = pbase_वृक्ष_cache_ix(oid);
	पूर्णाङ्क available_ix = -1;

	/* pbase-tree-cache acts as a limited hashtable.
	 * your object will be found at your index or within a few
	 * slots after that slot if it is cached.
	 */
	प्रति (neigh = 0; neigh < 8; neigh++) {
		ent = pbase_वृक्ष_cache[my_ix];
		यदि (ent && oideq(&ent->oid, oid)) {
			ent->ref++;
			प्रत्यागम ent;
		}
		अन्यथा यदि (((available_ix < 0) && (!ent || !ent->ref)) ||
			 ((0 <= available_ix) &&
			  (!ent && pbase_वृक्ष_cache[available_ix])))
			available_ix = my_ix;
		यदि (!ent)
			भङ्ग;
		my_ix = pbase_वृक्ष_cache_ix_incr(my_ix);
	}

	/* Did not find one.  Either we got a bogus request or
	 * we need to read and perhaps cache.
	 */
	data = odb_read_object(the_repository->objects, oid, &type, &size_st);
	size = cast_size_t_to_ulong(size_st);
	यदि (!data)
		प्रत्यागम रिक्त;
	यदि (type != OBJ_वृक्ष) {
		मुक्त(data);
		प्रत्यागम रिक्त;
	}

	/* We need to either cache or return a throwaway copy */

	यदि (available_ix < 0)
		ent = रिक्त;
	अन्यथा {
		ent = pbase_वृक्ष_cache[available_ix];
		my_ix = available_ix;
	}

	यदि (!ent) {
		nent = xmalloc(परिमाण(*nent));
		nent->temporary = (available_ix < 0);
	}
	अन्यथा {
		/* evict and reuse */
		मुक्त(ent->वृक्ष_data);
		nent = ent;
	}
	oidcpy(&nent->oid, oid);
	nent->वृक्ष_data = data;
	nent->वृक्ष_size = size;
	nent->ref = 1;
	यदि (!nent->temporary)
		pbase_वृक्ष_cache[my_ix] = nent;
	प्रत्यागम nent;
}

स्थिर शून्य pbase_वृक्ष_put(संरचना pbase_वृक्ष_cache *cache)
{
	यदि (!cache->temporary) {
		cache->ref--;
		प्रत्यागम;
	}
	मुक्त(cache->वृक्ष_data);
	मुक्त(cache);
}

स्थिर size_t name_cmp_len(ध्रुव वर्ण *name)
{
	प्रत्यागम strcspn(name, "\n/");
}

स्थिर शून्य add_pbase_object(संरचना वृक्ष_desc *वृक्ष,
			     ध्रुव वर्ण *name,
			     size_t cmplen,
			     ध्रुव वर्ण *fullname)
{
	संरचना name_entry entry;
	पूर्णाङ्क cmp;

	यावत् (वृक्ष_entry(वृक्ष,&entry)) {
		यदि (S_ISGITLINK(entry.mode))
			अनुवर्त;
		cmp = वृक्ष_entry_len(&entry) != cmplen ? 1 :
		      memcmp(name, entry.path, cmplen);
		यदि (cmp > 0)
			अनुवर्त;
		यदि (cmp < 0)
			प्रत्यागम;
		यदि (name[cmplen] != '/') {
			add_object_entry(&entry.oid,
					 object_type(entry.mode),
					 fullname, 1);
			प्रत्यागम;
		}
		यदि (S_ISDIR(entry.mode)) {
			संरचना वृक्ष_desc sub;
			संरचना pbase_वृक्ष_cache *वृक्ष;
			ध्रुव वर्ण *down = name+cmplen+1;
			size_t downlen = name_cmp_len(down);

			वृक्ष = pbase_वृक्ष_get(&entry.oid);
			यदि (!वृक्ष)
				प्रत्यागम;
			init_वृक्ष_desc(&sub, &वृक्ष->oid,
				       वृक्ष->वृक्ष_data, वृक्ष->वृक्ष_size);

			add_pbase_object(&sub, down, downlen, fullname);
			pbase_वृक्ष_put(वृक्ष);
		}
	}
}

स्थिर unsigned *done_pbase_paths;
स्थिर पूर्णाङ्क done_pbase_paths_अंक;
स्थिर पूर्णाङ्क done_pbase_paths_alloc;
स्थिर पूर्णाङ्क done_pbase_path_pos(unsigned hash)
{
	पूर्णाङ्क lo = 0;
	पूर्णाङ्क hi = done_pbase_paths_अंक;
	यावत् (lo < hi) {
		पूर्णाङ्क mi = lo + (hi - lo) / 2;
		यदि (done_pbase_paths[mi] == hash)
			प्रत्यागम mi;
		यदि (done_pbase_paths[mi] < hash)
			hi = mi;
		अन्यथा
			lo = mi + 1;
	}
	प्रत्यागम -lo-1;
}

स्थिर पूर्णाङ्क check_pbase_path(unsigned hash)
{
	पूर्णाङ्क pos = done_pbase_path_pos(hash);
	यदि (0 <= pos)
		प्रत्यागम 1;
	pos = -pos - 1;
	ALLOC_GROW(done_pbase_paths,
		   done_pbase_paths_अंक + 1,
		   done_pbase_paths_alloc);
	done_pbase_paths_अंक++;
	यदि (pos < done_pbase_paths_अंक)
		MOVE_ARRAY(done_pbase_paths + pos + 1, done_pbase_paths + pos,
			   done_pbase_paths_अंक - pos - 1);
	done_pbase_paths[pos] = hash;
	प्रत्यागम 0;
}

स्थिर शून्य add_preferred_base_object(ध्रुव वर्ण *name)
{
	संरचना pbase_वृक्ष *it;
	size_t cmplen;
	unsigned hash = pack_name_hash_fn(name);

	यदि (!अंक_preferred_base || check_pbase_path(hash))
		प्रत्यागम;

	cmplen = name_cmp_len(name);
	प्रति (it = pbase_वृक्ष; it; it = it->next) {
		यदि (cmplen == 0) {
			add_object_entry(&it->pcache.oid, OBJ_वृक्ष, रिक्त, 1);
		}
		अन्यथा {
			संरचना वृक्ष_desc वृक्ष;
			init_वृक्ष_desc(&वृक्ष, &it->pcache.oid,
				       it->pcache.वृक्ष_data, it->pcache.वृक्ष_size);
			add_pbase_object(&वृक्ष, name, cmplen, name);
		}
	}
}

स्थिर शून्य add_preferred_base(संरचना object_id *oid)
{
	संरचना pbase_वृक्ष *it;
	शून्य *data;
	unsigned long size;
	size_t size_st = 0;
	संरचना object_id वृक्ष_oid;

	यदि (window <= अंक_preferred_base++)
		प्रत्यागम;

	data = odb_read_object_peeled(the_repository->objects, oid,
				      OBJ_वृक्ष, &size_st, &वृक्ष_oid);
	size = cast_size_t_to_ulong(size_st);
	यदि (!data)
		प्रत्यागम;

	प्रति (it = pbase_वृक्ष; it; it = it->next) {
		यदि (oideq(&it->pcache.oid, &वृक्ष_oid)) {
			मुक्त(data);
			प्रत्यागम;
		}
	}

	CALLOC_ARRAY(it, 1);
	it->next = pbase_वृक्ष;
	pbase_वृक्ष = it;

	oidcpy(&it->pcache.oid, &वृक्ष_oid);
	it->pcache.वृक्ष_data = data;
	it->pcache.वृक्ष_size = size;
}

स्थिर शून्य cleanup_preferred_base(शून्य)
{
	संरचना pbase_वृक्ष *it;
	unsigned i;

	it = pbase_वृक्ष;
	pbase_वृक्ष = रिक्त;
	यावत् (it) {
		संरचना pbase_वृक्ष *tmp = it;
		it = tmp->next;
		मुक्त(tmp->pcache.वृक्ष_data);
		मुक्त(tmp);
	}

	प्रति (i = 0; i < ARRAY_SIZE(pbase_वृक्ष_cache); i++) {
		यदि (!pbase_वृक्ष_cache[i])
			अनुवर्त;
		मुक्त(pbase_वृक्ष_cache[i]->वृक्ष_data);
		FREE_AND_NULL(pbase_वृक्ष_cache[i]);
	}

	FREE_AND_NULL(done_pbase_paths);
	done_pbase_paths_अंक = done_pbase_paths_alloc = 0;
}

/*
 * Return 1 iff the object specified by "delta" can be sent
 * literally as a delta against the base in "base_sha1". If
 * so, then *base_out will point to the entry in our packing
 * list, or NULL if we must use the external-base list.
 *
 * Depth value does not matter - find_deltas() will
 * never consider reused delta as the base object to
 * deltify other objects against, in order to avoid
 * circular deltas.
 */
स्थिर पूर्णाङ्क can_reuse_delta(ध्रुव संरचना object_id *base_oid,
			   संरचना object_entry *delta,
			   संरचना object_entry **base_out)
{
	संरचना object_entry *base;

	/*
	 * First see if we're already sending the base (or it's explicitly in
	 * our "excluded" list).
	 */
	base = packlist_find(&to_pack, base_oid);
	यदि (base) {
		यदि (!in_same_island(&delta->idx.oid, &base->idx.oid))
			प्रत्यागम 0;
		*base_out = base;
		प्रत्यागम 1;
	}

	/*
	 * Otherwise, reachability bitmaps may tell us if the receiver has it,
	 * even if it was buried too deep in history to make it into the
	 * packing list.
	 */
	यदि (thin && bitmap_has_oid_in_uninteresting(bitmap_git, base_oid)) {
		यदि (use_delta_islands) {
			यदि (!in_same_island(&delta->idx.oid, base_oid))
				प्रत्यागम 0;
		}
		*base_out = रिक्त;
		प्रत्यागम 1;
	}

	प्रत्यागम 0;
}

स्थिर शून्य prefetch_to_pack(uint32_t object_index_start) {
	संरचना oid_array to_fetch = OID_ARRAY_INIT;
	uint32_t i;

	प्रति (i = object_index_start; i < to_pack.nr_objects; i++) {
		संरचना object_entry *entry = to_pack.objects + i;

		यदि (!odb_read_object_info_extended(the_repository->objects,
						   &entry->idx.oid,
						   रिक्त,
						   OBJECT_INFO_FOR_PREFETCH))
			अनुवर्त;
		oid_array_append(&to_fetch, &entry->idx.oid);
	}
	promisor_remote_get_direct(the_repository,
				   to_fetch.oid, to_fetch.nr);
	oid_array_clear(&to_fetch);
}

स्थिर शून्य check_object(संरचना object_entry *entry, uint32_t object_index)
{
	size_t canonical_size;
	गणना object_type type;
	संरचना object_info oi = {.typep = &type, .sizep = &canonical_size};

	यदि (IN_PACK(entry)) {
		संरचना packed_git *p = IN_PACK(entry);
		संरचना pack_window *w_curs = रिक्त;
		पूर्णाङ्क have_base = 0;
		संरचना object_id base_ref;
		संरचना object_entry *base_entry;
		size_t used, used_0, avail;
		off_t ofs;
		unsigned वर्ण *buf, c;
		गणना object_type type;
		size_t in_pack_size;

		buf = use_pack(p, &w_curs, entry->in_pack_offset, &avail);

		/*
		 * We want in_pack_type even if we do not reuse delta
		 * since non-delta representations could still be reused.
		 */
		used = unpack_object_header_buffer(buf, avail,
						   &type,
						   &in_pack_size);
		यदि (used == 0)
			गच्छ give_up;

		यदि (type < 0)
			BUG("invalid type %d", type);
		entry->in_pack_type = type;

		/*
		 * Determine if this is a delta and if so whether we can
		 * reuse it or not.  Otherwise let's find out as cheaply as
		 * possible what the actual type and size for this object is.
		 */
		विकल्प (entry->in_pack_type) {
		स्वभाव:
			/* Not a delta hence we've already got all we need. */
			oe_set_type(entry, entry->in_pack_type);
			SET_SIZE(entry, in_pack_size);
			entry->in_pack_header_size = used;
			यदि (oe_type(entry) < OBJ_COMMIT || oe_type(entry) > OBJ_BLOB)
				गच्छ give_up;
			unuse_pack(&w_curs);
			प्रत्यागम;
		पक्ष OBJ_REF_DELTA:
			यदि (reuse_delta && !entry->preferred_base) {
				oidread(&base_ref,
					use_pack(p, &w_curs,
						 entry->in_pack_offset + used,
						 रिक्त),
					the_repository->hash_algo);
				have_base = 1;
			}
			entry->in_pack_header_size = used + the_hash_algo->rawsz;
			भङ्ग;
		पक्ष OBJ_OFS_DELTA:
			buf = use_pack(p, &w_curs,
				       entry->in_pack_offset + used, रिक्त);
			used_0 = 0;
			c = buf[used_0++];
			ofs = c & 127;
			यावत् (c & 128) {
				ofs += 1;
				यदि (!ofs || MSB(ofs, 7)) {
					error(_("delta base offset overflow in pack for %s"),
					      oid_to_hex(&entry->idx.oid));
					गच्छ give_up;
				}
				c = buf[used_0++];
				ofs = (ofs << 7) + (c & 127);
			}
			ofs = entry->in_pack_offset - ofs;
			यदि (ofs <= 0 || ofs >= entry->in_pack_offset) {
				error(_("delta base offset out of bound for %s"),
				      oid_to_hex(&entry->idx.oid));
				गच्छ give_up;
			}
			यदि (reuse_delta && !entry->preferred_base) {
				uint32_t pos;
				यदि (offset_to_pack_pos(p, ofs, &pos) < 0)
					गच्छ give_up;
				यदि (!nth_packed_object_id(&base_ref, p,
							  pack_pos_to_index(p, pos)))
					have_base = 1;
			}
			entry->in_pack_header_size = used + used_0;
			भङ्ग;
		}

		यदि (have_base &&
		    can_reuse_delta(&base_ref, entry, &base_entry)) {
			oe_set_type(entry, entry->in_pack_type);
			SET_SIZE(entry, in_pack_size); /* delta size */
			SET_DELTA_SIZE(entry, in_pack_size);

			यदि (base_entry) {
				SET_DELTA(entry, base_entry);
				entry->delta_sibling_idx = base_entry->delta_child_idx;
				SET_DELTA_CHILD(base_entry, entry);
			} अन्यथा {
				SET_DELTA_EXT(entry, &base_ref);
			}

			unuse_pack(&w_curs);
			प्रत्यागम;
		}

		यदि (oe_type(entry)) {
			off_t delta_pos;

			/*
			 * This must be a delta and we already know what the
			 * final object type is.  Let's extract the actual
			 * object size from the delta header.
			 */
			delta_pos = entry->in_pack_offset + entry->in_pack_header_size;
			canonical_size = get_size_from_delta(p, &w_curs,
							     delta_pos);
			यदि (canonical_size == 0)
				गच्छ give_up;
			SET_SIZE(entry, canonical_size);
			unuse_pack(&w_curs);
			प्रत्यागम;
		}

		/*
		 * No choice but to fall back to the recursive delta walk
		 * with odb_read_object_info() to find about the object type
		 * at this point...
		 */
		give_up:
		unuse_pack(&w_curs);
	}

	यदि (odb_read_object_info_extended(the_repository->objects, &entry->idx.oid, &oi,
					  OBJECT_INFO_SKIP_FETCH_OBJECT | OBJECT_INFO_LOOKUP_REPLACE) < 0) {
		यदि (repo_has_promisor_remote(the_repository)) {
			prefetch_to_pack(object_index);
			यदि (odb_read_object_info_extended(the_repository->objects, &entry->idx.oid, &oi,
							  OBJECT_INFO_SKIP_FETCH_OBJECT | OBJECT_INFO_LOOKUP_REPLACE) < 0)
				type = -1;
		} अन्यथा {
			type = -1;
		}
	}
	oe_set_type(entry, type);
	यदि (entry->type_valid) {
		SET_SIZE(entry, canonical_size);
	} अन्यथा {
		/*
		 * Bad object type is checked in prepare_pack().  This is
		 * to permit a missing preferred base object to be ignored
		 * as a preferred base.  Doing so can result in a larger
		 * pack file, but the transfer will still take place.
		 */
	}
}

स्थिर पूर्णाङ्क pack_offset_sort(ध्रुव शून्य *_a, ध्रुव शून्य *_b)
{
	ध्रुव संरचना object_entry *a = *(संरचना object_entry **)_a;
	ध्रुव संरचना object_entry *b = *(संरचना object_entry **)_b;
	ध्रुव संरचना packed_git *a_in_pack = IN_PACK(a);
	ध्रुव संरचना packed_git *b_in_pack = IN_PACK(b);

	/* avoid filesystem trashing with loose objects */
	यदि (!a_in_pack && !b_in_pack)
		प्रत्यागम oidcmp(&a->idx.oid, &b->idx.oid);

	यदि (a_in_pack < b_in_pack)
		प्रत्यागम -1;
	यदि (a_in_pack > b_in_pack)
		प्रत्यागम 1;
	प्रत्यागम a->in_pack_offset < b->in_pack_offset ? -1 :
			(a->in_pack_offset > b->in_pack_offset);
}

/*
 * Drop an on-disk delta we were planning to reuse. Naively, this would
 * just involve blanking out the "delta" field, but we have to deal
 * with some extra book-keeping:
 *
 *   1. Removing ourselves from the delta_sibling linked list.
 *
 *   2. Updating our size/type to the non-delta representation. These were
 *      either not recorded initially (size) or overwritten with the delta type
 *      (type) when check_object() decided to reuse the delta.
 *
 *   3. Resetting our delta depth, as we are now a base object.
 */
स्थिर शून्य drop_reused_delta(संरचना object_entry *entry)
{
	unsigned *idx = &to_pack.objects[entry->delta_idx - 1].delta_child_idx;
	संरचना object_info oi = OBJECT_INFO_INIT;
	गणना object_type type;
	size_t size;

	यावत् (*idx) {
		संरचना object_entry *oe = &to_pack.objects[*idx - 1];

		यदि (oe == entry)
			*idx = oe->delta_sibling_idx;
		अन्यथा
			idx = &oe->delta_sibling_idx;
	}
	SET_DELTA(entry, रिक्त);
	entry->depth = 0;

	oi.sizep = &size;
	oi.typep = &type;
	यदि (packed_object_info(रिक्त, IN_PACK(entry), entry->in_pack_offset, &oi) < 0) {
		/*
		 * We failed to get the info from this pack for some reason;
		 * fall back to odb_read_object_info, which may find another copy.
		 * And if that fails, the error will be recorded in oe_type(entry)
		 * and dealt with in prepare_pack().
		 */
		oe_set_type(entry,
			    odb_read_object_info(the_repository->objects,
						 &entry->idx.oid, &size));
	} अन्यथा {
		oe_set_type(entry, type);
	}
	SET_SIZE(entry, size);
}

/*
 * Follow the chain of deltas from this entry onward, throwing away any links
 * that cause us to hit a cycle (as determined by the DFS state flags in
 * the entries).
 *
 * We also detect too-long reused chains that would violate our --depth
 * limit.
 */
स्थिर शून्य break_delta_chains(संरचना object_entry *entry)
{
	/*
	 * The actual depth of each object we will write is stored as an int,
	 * as it cannot exceed our int "depth" limit. But before we break
	 * changes based no that limit, we may potentially go as deep as the
	 * number of objects, which is elsewhere bounded to a uint32_t.
	 */
	uint32_t total_depth;
	संरचना object_entry *cur, *next;

	प्रति (cur = entry, total_depth = 0;
	     cur;
	     cur = DELTA(cur), total_depth++) {
		यदि (cur->dfs_state == DFS_DONE) {
			/*
			 * We've already seen this object and know it isn't
			 * part of a cycle. We do need to append its depth
			 * to our count.
			 */
			total_depth += cur->depth;
			भङ्ग;
		}

		/*
		 * We break cycles before looping, so an ACTIVE state (or any
		 * other cruft which made its way into the state variable)
		 * is a bug.
		 */
		यदि (cur->dfs_state != DFS_NONE)
			BUG("confusing delta dfs state in first pass: %d",
			    cur->dfs_state);

		/*
		 * Now we know this is the first time we've seen the object. If
		 * it's not a delta, we're done traversing, but we'll mark it
		 * done to save time on future traversals.
		 */
		यदि (!DELTA(cur)) {
			cur->dfs_state = DFS_DONE;
			भङ्ग;
		}

		/*
		 * Mark ourselves as active and see if the next step causes
		 * us to cycle to another active object. It's important to do
		 * this _before_ we loop, because it impacts where we make the
		 * cut, and thus how our total_depth counter works.
		 * E.g., We may see a partial loop like:
		 *
		 *   A -> B -> C -> D -> B
		 *
		 * Cutting B->C breaks the cycle. But now the depth of A is
		 * only 1, and our total_depth counter is at 3. The size of the
		 * error is always one less than the size of the cycle we
		 * broke. Commits C and D were "lost" from A's chain.
		 *
		 * If we instead cut D->B, then the depth of A is correct at 3.
		 * We keep all commits in the chain that we examined.
		 */
		cur->dfs_state = DFS_ACTIVE;
		यदि (DELTA(cur)->dfs_state == DFS_ACTIVE) {
			drop_reused_delta(cur);
			cur->dfs_state = DFS_DONE;
			भङ्ग;
		}
	}

	/*
	 * And now that we've gone all the way to the bottom of the chain, we
	 * need to clear the active flags and set the depth fields as
	 * appropriate. Unlike the loop above, which can quit when it drops a
	 * delta, we need to keep going to look for more depth cuts. So we need
	 * an extra "next" pointer to keep going after we reset cur->delta.
	 */
	प्रति (cur = entry; cur; cur = next) {
		next = DELTA(cur);

		/*
		 * We should have a chain of zero or more ACTIVE states down to
		 * a final DONE. We can quit after the DONE, because either it
		 * has no bases, or we've already handled them in a previous
		 * call.
		 */
		यदि (cur->dfs_state == DFS_DONE)
			भङ्ग;
		अन्यथा यदि (cur->dfs_state != DFS_ACTIVE)
			BUG("confusing delta dfs state in second pass: %d",
			    cur->dfs_state);

		/*
		 * If the total_depth is more than depth, then we need to snip
		 * the chain into two or more smaller chains that don't exceed
		 * the maximum depth. Most of the resulting chains will contain
		 * (depth + 1) entries (i.e., depth deltas plus one base), and
		 * the last chain (i.e., the one containing entry) will contain
		 * whatever entries are left over, namely
		 * (total_depth % (depth + 1)) of them.
		 *
		 * Since we are iterating towards decreasing depth, we need to
		 * decrement total_depth as we go, and we need to write to the
		 * entry what its final depth will be after all of the
		 * snipping. Since we're snipping into chains of length (depth
		 * + 1) entries, the final depth of an entry will be its
		 * original depth modulo (depth + 1). Any time we encounter an
		 * entry whose final depth is supposed to be zero, we snip it
		 * from its delta base, thereby making it so.
		 */
		cur->depth = (total_depth--) % (depth + 1);
		यदि (!cur->depth)
			drop_reused_delta(cur);

		cur->dfs_state = DFS_DONE;
	}
}

स्थिर शून्य get_object_details(शून्य)
{
	uint32_t i;
	संरचना object_entry **sorted_by_offset;

	यदि (progress)
		progress_state = start_progress(the_repository,
						_("Counting objects"),
						to_pack.nr_objects);

	CALLOC_ARRAY(sorted_by_offset, to_pack.nr_objects);
	प्रति (i = 0; i < to_pack.nr_objects; i++)
		sorted_by_offset[i] = to_pack.objects + i;
	QSORT(sorted_by_offset, to_pack.nr_objects, pack_offset_sort);

	प्रति (i = 0; i < to_pack.nr_objects; i++) {
		संरचना object_entry *entry = sorted_by_offset[i];
		check_object(entry, i);
		यदि (entry->type_valid &&
		    oe_size_greater_than(&to_pack, entry,
					 repo_settings_get_big_file_threshold(the_repository)))
			entry->no_try_delta = 1;
		display_progress(progress_state, i + 1);
	}
	stop_progress(&progress_state);

	/*
	 * This must happen in a second pass, since we rely on the delta
	 * information for the whole list being completed.
	 */
	प्रति (i = 0; i < to_pack.nr_objects; i++)
		break_delta_chains(&to_pack.objects[i]);

	मुक्त(sorted_by_offset);
}

/*
 * We search for deltas in a list sorted by type, by filename hash, and then
 * by size, so that we see progressively smaller and smaller files.
 * That's because we prefer deltas to be from the bigger file
 * to the smaller -- deletes are potentially cheaper, but perhaps
 * more importantly, the bigger file is likely the more recent
 * one.  The deepest deltas are therefore the oldest objects which are
 * less susceptible to be accessed often.
 */
स्थिर पूर्णाङ्क type_size_sort(ध्रुव शून्य *_a, ध्रुव शून्य *_b)
{
	ध्रुव संरचना object_entry *a = *(संरचना object_entry **)_a;
	ध्रुव संरचना object_entry *b = *(संरचना object_entry **)_b;
	ध्रुव गणना object_type a_type = oe_type(a);
	ध्रुव गणना object_type b_type = oe_type(b);
	ध्रुव unsigned long a_size = SIZE(a);
	ध्रुव unsigned long b_size = SIZE(b);

	यदि (a_type > b_type)
		प्रत्यागम -1;
	यदि (a_type < b_type)
		प्रत्यागम 1;
	यदि (a->hash > b->hash)
		प्रत्यागम -1;
	यदि (a->hash < b->hash)
		प्रत्यागम 1;
	यदि (a->preferred_base > b->preferred_base)
		प्रत्यागम -1;
	यदि (a->preferred_base < b->preferred_base)
		प्रत्यागम 1;
	यदि (use_delta_islands) {
		ध्रुव पूर्णाङ्क island_cmp = island_delta_cmp(&a->idx.oid, &b->idx.oid);
		यदि (island_cmp)
			प्रत्यागम island_cmp;
	}
	यदि (a_size > b_size)
		प्रत्यागम -1;
	यदि (a_size < b_size)
		प्रत्यागम 1;
	प्रत्यागम a < b ? -1 : (a > b);  /* newest first */
}

संरचना unpacked {
	संरचना object_entry *entry;
	शून्य *data;
	संरचना delta_index *index;
	unsigned depth;
};

स्थिर पूर्णाङ्क delta_cacheable(size_t src_size, size_t trg_size,
			   size_t delta_size)
{
	यदि (max_delta_cache_size && delta_cache_size + delta_size > max_delta_cache_size)
		प्रत्यागम 0;

	यदि (delta_size < cache_max_small_delta_size)
		प्रत्यागम 1;

	/* cache delta, if objects are large enough compared to delta size */
	यदि ((src_size >> 20) + (trg_size >> 21) > (delta_size >> 10))
		प्रत्यागम 1;

	प्रत्यागम 0;
}

/* Protect delta_cache_size */
स्थिर pthread_mutex_t cache_mutex;
#define cache_lock()		pthread_mutex_lock(&cache_mutex)
#define cache_unlock()		pthread_mutex_unlock(&cache_mutex)

/*
 * Protect object list partitioning (e.g. struct thread_param) and
 * progress_state
 */
स्थिर pthread_mutex_t progress_mutex;
#define progress_lock()		pthread_mutex_lock(&progress_mutex)
#define progress_unlock()	pthread_mutex_unlock(&progress_mutex)

/*
 * Access to struct object_entry is unprotected since each thread owns
 * a portion of the main object list. Just don't access object entries
 * ahead in the list because they can be stolen and would need
 * progress_mutex for protection.
 */

स्थिर अन्तःस्थ पूर्णाङ्क oe_size_less_than(संरचना packing_data *pack,
				    ध्रुव संरचना object_entry *lhs,
				    size_t rhs)
{
	यदि (lhs->size_valid)
		प्रत्यागम lhs->size_ < rhs;
	यदि (rhs < pack->oe_size_limit) /* rhs < 2^x <= lhs ? */
		प्रत्यागम 0;
	प्रत्यागम oe_get_size_slow(pack, lhs) < rhs;
}

स्थिर अन्तःस्थ शून्य oe_set_वृक्ष_depth(संरचना packing_data *pack,
				     संरचना object_entry *e,
				     unsigned पूर्णाङ्क वृक्ष_depth)
{
	यदि (!pack->वृक्ष_depth)
		CALLOC_ARRAY(pack->वृक्ष_depth, pack->nr_alloc);
	pack->वृक्ष_depth[e - pack->objects] = वृक्ष_depth;
}

स्थिर शून्य record_वृक्ष_depth(ध्रुव संरचना object_id *oid, ध्रुव वर्ण *name)
{
	ध्रुव वर्ण *p;
	unsigned depth;
	संरचना object_entry *ent;

	/* the empty string is a root tree, which is depth 0 */
	depth = *name ? 1 : 0;
	प्रति (p = strchr(name, '/'); p; p = strchr(p + 1, '/'))
		depth++;

	ent = packlist_find(&to_pack, oid);
	यदि (ent && depth > oe_वृक्ष_depth(&to_pack, ent))
		oe_set_वृक्ष_depth(&to_pack, ent, depth);
}

/*
 * Return the size of the object without doing any delta
 * reconstruction (so non-deltas are true object sizes, but deltas
 * return the size of the delta data).
 */
size_t oe_get_size_slow(संरचना packing_data *pack,
			ध्रुव संरचना object_entry *e)
{
	संरचना packed_git *p;
	संरचना pack_window *w_curs;
	unsigned वर्ण *buf;
	गणना object_type type;
	size_t used, avail, size;

	यदि (e->type_ != OBJ_OFS_DELTA && e->type_ != OBJ_REF_DELTA) {
		size_t sz;
		packing_data_lock(&to_pack);
		यदि (odb_read_object_info(the_repository->objects,
					 &e->idx.oid, &sz) < 0)
			die(_("unable to get size of %s"),
			    oid_to_hex(&e->idx.oid));
		packing_data_unlock(&to_pack);
		प्रत्यागम sz;
	}

	p = oe_in_pack(pack, e);
	यदि (!p)
		BUG("when e->type is a delta, it must belong to a pack");

	packing_data_lock(&to_pack);
	w_curs = रिक्त;
	buf = use_pack(p, &w_curs, e->in_pack_offset, &avail);
	used = unpack_object_header_buffer(buf, avail, &type, &size);
	यदि (used == 0)
		die(_("unable to parse object header of %s"),
		    oid_to_hex(&e->idx.oid));

	unuse_pack(&w_curs);
	packing_data_unlock(&to_pack);
	प्रत्यागम size;
}

स्थिर पूर्णाङ्क try_delta(संरचना unpacked *trg, संरचना unpacked *src,
		     unsigned max_depth, size_t *mem_usage)
{
	संरचना object_entry *trg_entry = trg->entry;
	संरचना object_entry *src_entry = src->entry;
	unsigned long trg_size, src_size, sizediff, max_size, sz;
	size_t delta_size;
	unsigned ref_depth;
	गणना object_type type;
	शून्य *delta_buf;

	/* Don't bother doing diffs between different types */
	यदि (oe_type(trg_entry) != oe_type(src_entry))
		प्रत्यागम -1;

	/*
	 * We do not bother to try a delta that we discarded on an
	 * earlier try, but only when reusing delta data.  Note that
	 * src_entry that is marked as the preferred_base should always
	 * be considered, as even if we produce a suboptimal delta against
	 * it, we will still save the transfer cost, as we already know
	 * the other side has it and we won't send src_entry at all.
	 */
	यदि (reuse_delta && IN_PACK(trg_entry) &&
	    IN_PACK(trg_entry) == IN_PACK(src_entry) &&
	    !src_entry->preferred_base &&
	    trg_entry->in_pack_type != OBJ_REF_DELTA &&
	    trg_entry->in_pack_type != OBJ_OFS_DELTA)
		प्रत्यागम 0;

	/* Let's not bust the allowed depth. */
	यदि (src->depth >= max_depth)
		प्रत्यागम 0;

	/* Now some size filtering heuristics. */
	trg_size = SIZE(trg_entry);
	यदि (!DELTA(trg_entry)) {
		max_size = trg_size/2 - the_hash_algo->rawsz;
		ref_depth = 1;
	} अन्यथा {
		max_size = DELTA_SIZE(trg_entry);
		ref_depth = trg->depth;
	}
	max_size = (uint64_t)max_size * (max_depth - src->depth) /
						(max_depth - ref_depth + 1);
	यदि (max_size == 0)
		प्रत्यागम 0;
	src_size = SIZE(src_entry);
	sizediff = src_size < trg_size ? trg_size - src_size : 0;
	यदि (sizediff >= max_size)
		प्रत्यागम 0;
	यदि (trg_size < src_size / 32)
		प्रत्यागम 0;

	यदि (!in_same_island(&trg->entry->idx.oid, &src->entry->idx.oid))
		प्रत्यागम 0;

	/* Load data if not already done */
	यदि (!trg->data) {
		size_t sz_st = 0;
		packing_data_lock(&to_pack);
		trg->data = odb_read_object(the_repository->objects,
					    &trg_entry->idx.oid, &type,
					    &sz_st);
		sz = cast_size_t_to_ulong(sz_st);
		packing_data_unlock(&to_pack);
		यदि (!trg->data)
			die(_("object %s cannot be read"),
			    oid_to_hex(&trg_entry->idx.oid));
		यदि (sz != trg_size)
			die(_("object %s inconsistent object length (%"PRIuMAX" vs %"PRIuMAX")"),
			    oid_to_hex(&trg_entry->idx.oid), (uintmax_t)sz,
			    (uintmax_t)trg_size);
		*mem_usage += sz;
	}
	यदि (!src->data) {
		size_t sz_st = 0;
		packing_data_lock(&to_pack);
		src->data = odb_read_object(the_repository->objects,
					    &src_entry->idx.oid, &type,
					    &sz_st);
		sz = cast_size_t_to_ulong(sz_st);
		packing_data_unlock(&to_pack);
		यदि (!src->data) {
			यदि (src_entry->preferred_base) {
				स्थिर पूर्णाङ्क warned = 0;
				यदि (!warned++)
					warning(_("object %s cannot be read"),
						oid_to_hex(&src_entry->idx.oid));
				/*
				 * Those objects are not included in the
				 * resulting pack.  Be resilient and ignore
				 * them if they can't be read, in case the
				 * pack could be created nevertheless.
				 */
				प्रत्यागम 0;
			}
			die(_("object %s cannot be read"),
			    oid_to_hex(&src_entry->idx.oid));
		}
		यदि (sz != src_size)
			die(_("object %s inconsistent object length (%"PRIuMAX" vs %"PRIuMAX")"),
			    oid_to_hex(&src_entry->idx.oid), (uintmax_t)sz,
			    (uintmax_t)src_size);
		*mem_usage += sz;
	}
	यदि (!src->index) {
		src->index = create_delta_index(src->data, src_size);
		यदि (!src->index) {
			स्थिर पूर्णाङ्क warned = 0;
			यदि (!warned++)
				warning(_("suboptimal pack - out of memory"));
			प्रत्यागम 0;
		}
		*mem_usage += sizeof_delta_index(src->index);
	}

	delta_buf = create_delta(src->index, trg->data, trg_size, &delta_size, max_size);
	यदि (!delta_buf)
		प्रत्यागम 0;

	यदि (DELTA(trg_entry)) {
		/* Prefer only shallower same-sized deltas. */
		यदि (delta_size == DELTA_SIZE(trg_entry) &&
		    src->depth + 1 >= trg->depth) {
			मुक्त(delta_buf);
			प्रत्यागम 0;
		}
	}

	/*
	 * Handle memory allocation outside of the cache
	 * accounting lock.  Compiler will optimize the strangeness
	 * away when NO_PTHREADS is defined.
	 */
	मुक्त(trg_entry->delta_data);
	cache_lock();
	यदि (trg_entry->delta_data) {
		delta_cache_size -= DELTA_SIZE(trg_entry);
		trg_entry->delta_data = रिक्त;
	}
	यदि (delta_cacheable(src_size, trg_size, delta_size)) {
		delta_cache_size += delta_size;
		cache_unlock();
		trg_entry->delta_data = xrealloc(delta_buf, delta_size);
	} अन्यथा {
		cache_unlock();
		मुक्त(delta_buf);
	}

	SET_DELTA(trg_entry, src_entry);
	SET_DELTA_SIZE(trg_entry, delta_size);
	trg->depth = src->depth + 1;

	प्रत्यागम 1;
}

स्थिर unsigned पूर्णाङ्क check_delta_limit(संरचना object_entry *me, unsigned पूर्णाङ्क n)
{
	संरचना object_entry *child = DELTA_CHILD(me);
	unsigned पूर्णाङ्क m = n;
	यावत् (child) {
		ध्रुव unsigned पूर्णाङ्क c = check_delta_limit(child, n + 1);
		यदि (m < c)
			m = c;
		child = DELTA_SIBLING(child);
	}
	प्रत्यागम m;
}

स्थिर size_t free_unpacked(संरचना unpacked *n)
{
	size_t freed_mem = sizeof_delta_index(n->index);
	free_delta_index(n->index);
	n->index = रिक्त;
	यदि (n->data) {
		freed_mem += SIZE(n->entry);
		FREE_AND_NULL(n->data);
	}
	n->entry = रिक्त;
	n->depth = 0;
	प्रत्यागम freed_mem;
}

स्थिर शून्य find_deltas(संरचना object_entry **list, unsigned *list_size,
			पूर्णाङ्क window, पूर्णाङ्क depth, unsigned *processed)
{
	uint32_t i, idx = 0, मोजणी = 0;
	संरचना unpacked *array;
	size_t mem_usage = 0;

	CALLOC_ARRAY(array, window);

	प्रति (;;) {
		संरचना object_entry *entry;
		संरचना unpacked *n = array + idx;
		पूर्णाङ्क j, max_depth, best_base = -1;

		progress_lock();
		यदि (!*list_size) {
			progress_unlock();
			भङ्ग;
		}
		entry = *list++;
		(*list_size)--;
		यदि (!entry->preferred_base) {
			(*processed)++;
			display_progress(progress_state, *processed);
		}
		progress_unlock();

		mem_usage -= free_unpacked(n);
		n->entry = entry;

		यावत् (window_memory_limit &&
		       mem_usage > window_memory_limit &&
		       मोजणी > 1) {
			ध्रुव uint32_t tail = (idx + window - मोजणी) % window;
			mem_usage -= free_unpacked(array + tail);
			मोजणी--;
		}

		/* We do not compute delta to *create* objects we are not
		 * going to pack.
		 */
		यदि (entry->preferred_base)
			गच्छ next;

		/*
		 * If the current object is at pack edge, take the depth the
		 * objects that depend on the current object into account
		 * otherwise they would become too deep.
		 */
		max_depth = depth;
		यदि (DELTA_CHILD(entry)) {
			max_depth -= check_delta_limit(entry, 0);
			यदि (max_depth <= 0)
				गच्छ next;
		}

		j = window;
		यावत् (--j > 0) {
			पूर्णाङ्क ret;
			uint32_t other_idx = idx + j;
			संरचना unpacked *m;
			यदि (other_idx >= window)
				other_idx -= window;
			m = array + other_idx;
			यदि (!m->entry)
				भङ्ग;
			ret = try_delta(n, m, max_depth, &mem_usage);
			यदि (ret < 0)
				भङ्ग;
			अन्यथा यदि (ret > 0)
				best_base = other_idx;
		}

		/*
		 * If we decided to cache the delta data, then it is best
		 * to compress it right away.  First because we have to do
		 * it anyway, and doing it here while we're threaded will
		 * save a lot of time in the non threaded write phase,
		 * as well as allow for caching more deltas within
		 * the same cache size limit.
		 * ...
		 * But only if not writing to stdout, since in that case
		 * the network is most likely throttling writes anyway,
		 * and therefore it is best to go to the write phase ASAP
		 * instead, as we can afford spending more time compressing
		 * between writes at that moment.
		 */
		यदि (entry->delta_data && !pack_to_stdout) {
			unsigned long size;

			size = do_compress(&entry->delta_data, DELTA_SIZE(entry));
			यदि (size < (1U << OE_Z_DELTA_BITS)) {
				entry->z_delta_size = size;
				cache_lock();
				delta_cache_size -= DELTA_SIZE(entry);
				delta_cache_size += entry->z_delta_size;
				cache_unlock();
			} अन्यथा {
				FREE_AND_NULL(entry->delta_data);
				entry->z_delta_size = 0;
			}
		}

		/* if we made n a delta, and if n is already at max
		 * depth, leaving it in the window is pointless.  we
		 * should evict it first.
		 */
		यदि (DELTA(entry) && max_depth <= n->depth)
			अनुवर्त;

		/*
		 * Move the best delta base up in the window, after the
		 * currently deltified object, to keep it longer.  It will
		 * be the first base object to be attempted next.
		 */
		यदि (DELTA(entry)) {
			संरचना unpacked swap = array[best_base];
			पूर्णाङ्क dist = (window + idx - best_base) % window;
			पूर्णाङ्क dst = best_base;
			यावत् (dist--) {
				पूर्णाङ्क src = (dst + 1) % window;
				array[dst] = array[src];
				dst = src;
			}
			array[dst] = swap;
		}

		next:
		idx++;
		यदि (मोजणी + 1 < window)
			मोजणी++;
		यदि (idx >= window)
			idx = 0;
	}

	प्रति (i = 0; i < window; ++i) {
		free_delta_index(array[i].index);
		मुक्त(array[i].data);
	}
	मुक्त(array);
}

/*
 * The main object list is split into smaller lists, each is handed to
 * one worker.
 *
 * The main thread waits on the condition that (at least) one of the workers
 * has stopped working (which is indicated in the .working member of
 * struct thread_params).
 *
 * When a work thread has completed its work, it sets .working to 0 and
 * signals the main thread and waits on the condition that .data_ready
 * becomes 1.
 *
 * The main thread steals half of the work from the worker that has
 * most work left to hand it to the idle worker.
 */

संरचना thread_params {
	pthread_t thread;
	संरचना object_entry **list;
	संरचना packing_region *regions;
	unsigned list_size;
	unsigned remaining;
	पूर्णाङ्क window;
	पूर्णाङ्क depth;
	पूर्णाङ्क working;
	पूर्णाङ्क data_ready;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	unsigned *processed;
};

स्थिर pthread_cond_t progress_cond;

/*
 * Mutex and conditional variable can't be statically-initialized on Windows.
 */
स्थिर शून्य init_threaded_शोध(शून्य)
{
	pthread_mutex_init(&cache_mutex, रिक्त);
	pthread_mutex_init(&progress_mutex, रिक्त);
	pthread_cond_init(&progress_cond, रिक्त);
}

स्थिर शून्य cleanup_threaded_शोध(शून्य)
{
	pthread_cond_destroy(&progress_cond);
	pthread_mutex_destroy(&cache_mutex);
	pthread_mutex_destroy(&progress_mutex);
}

स्थिर शून्य *threaded_find_deltas(शून्य *arg)
{
	संरचना thread_params *me = arg;

	progress_lock();
	यावत् (me->remaining) {
		progress_unlock();

		find_deltas(me->list, &me->remaining,
			    me->window, me->depth, me->processed);

		progress_lock();
		me->working = 0;
		pthread_cond_signal(&progress_cond);
		progress_unlock();

		/*
		 * We must not set ->data_ready before we wait on the
		 * condition because the main thread may have set it to 1
		 * before we get here. In order to be sure that new
		 * work is available if we see 1 in ->data_ready, it
		 * was initialized to 0 before this thread was spawned
		 * and we reset it to 0 right away.
		 */
		pthread_mutex_lock(&me->mutex);
		यावत् (!me->data_ready)
			pthread_cond_wait(&me->cond, &me->mutex);
		me->data_ready = 0;
		pthread_mutex_unlock(&me->mutex);

		progress_lock();
	}
	progress_unlock();
	/* leave ->working 1 so that this doesn't get more work assigned */
	प्रत्यागम रिक्त;
}

स्थिर शून्य ll_find_deltas(संरचना object_entry **list, unsigned list_size,
			   पूर्णाङ्क window, पूर्णाङ्क depth, unsigned *processed)
{
	संरचना thread_params *p;
	पूर्णाङ्क i, ret, active_threads = 0;

	init_threaded_शोध();

	यदि (delta_शोध_threads <= 1) {
		find_deltas(list, &list_size, window, depth, processed);
		cleanup_threaded_शोध();
		प्रत्यागम;
	}
	यदि (progress > pack_to_stdout)
		fprintf_ln(stderr, _("Delta compression using up to %d threads"),
			   delta_शोध_threads);
	CALLOC_ARRAY(p, delta_शोध_threads);

	/* Partition the work amongst work threads. */
	प्रति (i = 0; i < delta_शोध_threads; i++) {
		unsigned sub_size = list_size / (delta_शोध_threads - i);

		/* don't use too small segments or no deltas will be found */
		यदि (sub_size < 2*window && i+1 < delta_शोध_threads)
			sub_size = 0;

		p[i].window = window;
		p[i].depth = depth;
		p[i].processed = processed;
		p[i].working = 1;
		p[i].data_ready = 0;

		/* try to split chunks on "path" boundaries */
		यावत् (sub_size && sub_size < list_size &&
		       list[sub_size]->hash &&
		       list[sub_size]->hash == list[sub_size-1]->hash)
			sub_size++;

		p[i].list = list;
		p[i].list_size = sub_size;
		p[i].remaining = sub_size;

		list += sub_size;
		list_size -= sub_size;
	}

	/* Start work threads. */
	प्रति (i = 0; i < delta_शोध_threads; i++) {
		यदि (!p[i].list_size)
			अनुवर्त;
		pthread_mutex_init(&p[i].mutex, रिक्त);
		pthread_cond_init(&p[i].cond, रिक्त);
		ret = pthread_create(&p[i].thread, रिक्त,
				     threaded_find_deltas, &p[i]);
		यदि (ret)
			die(_("unable to create thread: %s"), strerror(ret));
		active_threads++;
	}

	/*
	 * Now let's wait for work completion.  Each time a thread is done
	 * with its work, we steal half of the remaining work from the
	 * thread with the largest number of unprocessed objects and give
	 * it to that newly idle thread.  This ensure good load balancing
	 * until the remaining object list segments are simply too short
	 * to be worth splitting anymore.
	 */
	यावत् (active_threads) {
		संरचना thread_params *target = रिक्त;
		संरचना thread_params *victim = रिक्त;
		unsigned sub_size = 0;

		progress_lock();
		प्रति (;;) {
			प्रति (i = 0; !target && i < delta_शोध_threads; i++)
				यदि (!p[i].working)
					target = &p[i];
			यदि (target)
				भङ्ग;
			pthread_cond_wait(&progress_cond, &progress_mutex);
		}

		प्रति (i = 0; i < delta_शोध_threads; i++)
			यदि (p[i].remaining > 2*window &&
			    (!victim || victim->remaining < p[i].remaining))
				victim = &p[i];
		यदि (victim) {
			sub_size = victim->remaining / 2;
			list = victim->list + victim->list_size - sub_size;
			यावत् (sub_size && list[0]->hash &&
			       list[0]->hash == list[-1]->hash) {
				list++;
				sub_size--;
			}
			यदि (!sub_size) {
				/*
				 * It is possible for some "paths" to have
				 * so many objects that no hash boundary
				 * might be found.  Let's just steal the
				 * exact half in that case.
				 */
				sub_size = victim->remaining / 2;
				list -= sub_size;
			}
			target->list = list;
			victim->list_size -= sub_size;
			victim->remaining -= sub_size;
		}
		target->list_size = sub_size;
		target->remaining = sub_size;
		target->working = 1;
		progress_unlock();

		pthread_mutex_lock(&target->mutex);
		target->data_ready = 1;
		pthread_cond_signal(&target->cond);
		pthread_mutex_unlock(&target->mutex);

		यदि (!sub_size) {
			pthread_join(target->thread, रिक्त);
			pthread_cond_destroy(&target->cond);
			pthread_mutex_destroy(&target->mutex);
			active_threads--;
		}
	}
	cleanup_threaded_शोध();
	मुक्त(p);
}

स्थिर पूर्णाङ्क obj_is_packed(ध्रुव संरचना object_id *oid)
{
	प्रत्यागम packlist_find(&to_pack, oid) ||
		(reuse_packfile_bitmap &&
		 bitmap_walk_contains(bitmap_git, reuse_packfile_bitmap, oid));
}

स्थिर शून्य add_tag_chain(ध्रुव संरचना object_id *oid)
{
	संरचना tag *tag;

	/*
	 * We catch duplicates already in add_object_entry(), but we'd
	 * prefer to do this extra check to avoid having to parse the
	 * tag at all if we already know that it's being packed (e.g., if
	 * it was included via bitmaps, we would not have parsed it
	 * previously).
	 */
	यदि (obj_is_packed(oid))
		प्रत्यागम;

	tag = lookup_tag(the_repository, oid);
	यावत् (1) {
		यदि (!tag || parse_tag(the_repository, tag) || !tag->tagged)
			die(_("unable to pack objects reachable from tag %s"),
			    oid_to_hex(oid));

		add_object_entry(&tag->object.oid, OBJ_TAG, रिक्त, 0);

		यदि (tag->tagged->type != OBJ_TAG)
			प्रत्यागम;

		tag = (संरचना tag *)tag->tagged;
	}
}

स्थिर पूर्णाङ्क add_ref_tag(ध्रुव संरचना reference *ref, शून्य *cb_data UNUSED)
{
	संरचना object_id peeled;

	यदि (!reference_get_peeled_oid(the_repository, ref, &peeled) &&
	    obj_is_packed(&peeled))
		add_tag_chain(ref->oid);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क should_attempt_deltas(संरचना object_entry *entry)
{
	यदि (DELTA(entry))
		/* This happens if we decided to reuse existing
		 * delta from a pack. "reuse_delta &&" is implied.
		 */
		प्रत्यागम 0;

	यदि (!entry->type_valid ||
	    oe_size_less_than(&to_pack, entry, 50))
		प्रत्यागम 0;

	यदि (entry->no_try_delta)
		प्रत्यागम 0;

	यदि (!entry->preferred_base) {
		यदि (oe_type(entry) < 0)
			die(_("unable to get type of object %s"),
				oid_to_hex(&entry->idx.oid));
	} अन्यथा यदि (oe_type(entry) < 0) {
		/*
		 * This object is not found, but we
		 * don't have to include it anyway.
		 */
		प्रत्यागम 0;
	}

	प्रत्यागम 1;
}

स्थिर शून्य find_deltas_for_region(संरचना object_entry *list,
				   संरचना packing_region *region,
				   unsigned पूर्णाङ्क *processed)
{
	संरचना object_entry **delta_list;
	unsigned पूर्णाङ्क delta_list_nr = 0;

	ALLOC_ARRAY(delta_list, region->nr);
	प्रति (size_t i = 0; i < region->nr; i++) {
		संरचना object_entry *entry = list + region->start + i;
		यदि (should_attempt_deltas(entry))
			delta_list[delta_list_nr++] = entry;
	}

	QSORT(delta_list, delta_list_nr, type_size_sort);
	find_deltas(delta_list, &delta_list_nr, window, depth, processed);
	मुक्त(delta_list);
}

स्थिर शून्य find_deltas_by_region(संरचना object_entry *list,
				  संरचना packing_region *regions,
				  size_t start, size_t nr)
{
	unsigned पूर्णाङ्क processed = 0;
	size_t progress_nr;

	यदि (!nr)
		प्रत्यागम;

	progress_nr = regions[nr - 1].start + regions[nr - 1].nr;

	यदि (progress)
		progress_state = start_progress(the_repository,
						_("Compressing objects by path"),
						progress_nr);

	यावत् (nr--)
		find_deltas_for_region(list,
				       &regions[start++],
				       &processed);

	display_progress(progress_state, progress_nr);
	stop_progress(&progress_state);
}

स्थिर शून्य *threaded_find_deltas_by_path(शून्य *arg)
{
	संरचना thread_params *me = arg;

	progress_lock();
	यावत् (me->remaining) {
		यावत् (me->remaining) {
			progress_unlock();
			find_deltas_for_region(to_pack.objects,
					       me->regions,
					       me->processed);
			progress_lock();
			me->remaining--;
			me->regions++;
		}

		me->working = 0;
		pthread_cond_signal(&progress_cond);
		progress_unlock();

		/*
		 * We must not set ->data_ready before we wait on the
		 * condition because the main thread may have set it to 1
		 * before we get here. In order to be sure that new
		 * work is available if we see 1 in ->data_ready, it
		 * was initialized to 0 before this thread was spawned
		 * and we reset it to 0 right away.
		 */
		pthread_mutex_lock(&me->mutex);
		यावत् (!me->data_ready)
			pthread_cond_wait(&me->cond, &me->mutex);
		me->data_ready = 0;
		pthread_mutex_unlock(&me->mutex);

		progress_lock();
	}
	progress_unlock();
	/* leave ->working 1 so that this doesn't get more work assigned */
	प्रत्यागम रिक्त;
}

स्थिर शून्य ll_find_deltas_by_region(संरचना object_entry *list,
				     संरचना packing_region *regions,
				     uint32_t start, uint32_t nr)
{
	संरचना thread_params *p;
	पूर्णाङ्क i, ret, active_threads = 0;
	unsigned पूर्णाङ्क processed = 0;
	uint32_t progress_nr;
	init_threaded_शोध();

	यदि (!nr)
		प्रत्यागम;

	progress_nr =  regions[nr - 1].start + regions[nr - 1].nr;
	यदि (delta_शोध_threads <= 1) {
		find_deltas_by_region(list, regions, start, nr);
		cleanup_threaded_शोध();
		प्रत्यागम;
	}

	यदि (progress > pack_to_stdout)
		fprintf_ln(stderr,
			   Q_("Path-based delta compression using up to %d thread",
			      "Path-based delta compression using up to %d threads",
			      delta_शोध_threads),
			   delta_शोध_threads);
	CALLOC_ARRAY(p, delta_शोध_threads);

	यदि (progress)
		progress_state = start_progress(the_repository,
						_("Compressing objects by path"),
						progress_nr);
	/* Partition the work amongst work threads. */
	प्रति (i = 0; i < delta_शोध_threads; i++) {
		unsigned sub_size = nr / (delta_शोध_threads - i);

		p[i].window = window;
		p[i].depth = depth;
		p[i].processed = &processed;
		p[i].working = 1;
		p[i].data_ready = 0;

		p[i].regions = regions;
		p[i].list_size = sub_size;
		p[i].remaining = sub_size;

		regions += sub_size;
		nr -= sub_size;
	}

	/* Start work threads. */
	प्रति (i = 0; i < delta_शोध_threads; i++) {
		यदि (!p[i].list_size)
			अनुवर्त;
		pthread_mutex_init(&p[i].mutex, रिक्त);
		pthread_cond_init(&p[i].cond, रिक्त);
		ret = pthread_create(&p[i].thread, रिक्त,
				     threaded_find_deltas_by_path, &p[i]);
		यदि (ret)
			die(_("unable to create thread: %s"), strerror(ret));
		active_threads++;
	}

	/*
	 * Now let's wait for work completion.  Each time a thread is done
	 * with its work, we steal half of the remaining work from the
	 * thread with the largest number of unprocessed objects and give
	 * it to that newly idle thread.  This ensure good load balancing
	 * until the remaining object list segments are simply too short
	 * to be worth splitting anymore.
	 */
	यावत् (active_threads) {
		संरचना thread_params *target = रिक्त;
		संरचना thread_params *victim = रिक्त;
		unsigned sub_size = 0;

		progress_lock();
		प्रति (;;) {
			प्रति (i = 0; !target && i < delta_शोध_threads; i++)
				यदि (!p[i].working)
					target = &p[i];
			यदि (target)
				भङ्ग;
			pthread_cond_wait(&progress_cond, &progress_mutex);
		}

		प्रति (i = 0; i < delta_शोध_threads; i++)
			यदि (p[i].remaining > 2*window &&
			    (!victim || victim->remaining < p[i].remaining))
				victim = &p[i];
		यदि (victim) {
			sub_size = victim->remaining / 2;
			target->regions = victim->regions + victim->remaining - sub_size;
			victim->list_size -= sub_size;
			victim->remaining -= sub_size;
		}
		target->list_size = sub_size;
		target->remaining = sub_size;
		target->working = 1;
		progress_unlock();

		pthread_mutex_lock(&target->mutex);
		target->data_ready = 1;
		pthread_cond_signal(&target->cond);
		pthread_mutex_unlock(&target->mutex);

		यदि (!sub_size) {
			pthread_join(target->thread, रिक्त);
			pthread_cond_destroy(&target->cond);
			pthread_mutex_destroy(&target->mutex);
			active_threads--;
		}
	}
	cleanup_threaded_शोध();
	मुक्त(p);

	display_progress(progress_state, progress_nr);
	stop_progress(&progress_state);
}

स्थिर शून्य prepare_pack(पूर्णाङ्क window, पूर्णाङ्क depth)
{
	संरचना object_entry **delta_list;
	uint32_t i, nr_deltas;
	unsigned n;

	यदि (use_delta_islands)
		resolve_वृक्ष_islands(the_repository, progress, &to_pack);

	get_object_details();

	/*
	 * If we're locally repacking then we need to be doubly careful
	 * from now on in order to make sure no stealth corruption gets
	 * propagated to the new pack.  Clients receiving streamed packs
	 * should validate everything they get anyway so no need to incur
	 * the additional cost here in that case.
	 */
	यदि (!pack_to_stdout)
		do_check_packed_object_crc = 1;

	यदि (!to_pack.nr_objects || !window || !depth)
		प्रत्यागम;

	यदि (path_walk)
		ll_find_deltas_by_region(to_pack.objects, to_pack.regions,
					 0, to_pack.nr_regions);

	ALLOC_ARRAY(delta_list, to_pack.nr_objects);
	nr_deltas = n = 0;

	प्रति (i = 0; i < to_pack.nr_objects; i++) {
		संरचना object_entry *entry = to_pack.objects + i;

		यदि (!should_attempt_deltas(entry))
			अनुवर्त;

		यदि (!entry->preferred_base)
			nr_deltas++;

		delta_list[n++] = entry;
	}

	यदि (nr_deltas && n > 1) {
		unsigned nr_done = 0;

		यदि (progress)
			progress_state = start_progress(the_repository,
							_("Compressing objects"),
							nr_deltas);
		QSORT(delta_list, n, type_size_sort);
		ll_find_deltas(delta_list, n, window+1, depth, &nr_done);
		stop_progress(&progress_state);
		यदि (nr_done != nr_deltas)
			die(_("inconsistency with delta count"));
	}
	मुक्त(delta_list);
}

स्थिर पूर्णाङ्क git_pack_config(ध्रुव वर्ण *k, ध्रुव वर्ण *v,
			   ध्रुव संरचना config_context *ctx, शून्य *cb)
{
	यदि (!strcmp(k, "pack.window")) {
		window = git_config_int(k, v, ctx->kvi);
		प्रत्यागम 0;
	}
	यदि (!strcmp(k, "pack.windowmemory")) {
		window_memory_limit = git_config_ulong(k, v, ctx->kvi);
		प्रत्यागम 0;
	}
	यदि (!strcmp(k, "pack.depth")) {
		depth = git_config_int(k, v, ctx->kvi);
		प्रत्यागम 0;
	}
	यदि (!strcmp(k, "pack.deltacachesize")) {
		max_delta_cache_size = git_config_size_t(k, v, ctx->kvi);
		प्रत्यागम 0;
	}
	यदि (!strcmp(k, "pack.deltacachelimit")) {
		cache_max_small_delta_size = git_config_int(k, v, ctx->kvi);
		प्रत्यागम 0;
	}
	यदि (!strcmp(k, "pack.writebitmaphashcache")) {
		यदि (git_config_bool(k, v))
			write_bitmap_options |= BITMAP_OPT_HASH_CACHE;
		अन्यथा
			write_bitmap_options &= ~BITMAP_OPT_HASH_CACHE;
	}

	यदि (!strcmp(k, "pack.writebitmaplookuptable")) {
		यदि (git_config_bool(k, v))
			write_bitmap_options |= BITMAP_OPT_LOOKUP_TABLE;
		अन्यथा
			write_bitmap_options &= ~BITMAP_OPT_LOOKUP_TABLE;
	}

	यदि (!strcmp(k, "pack.usebitmaps")) {
		use_bitmap_index_default = git_config_bool(k, v);
		प्रत्यागम 0;
	}
	यदि (!strcmp(k, "pack.allowpackreuse")) {
		पूर्णाङ्क res = git_parse_maybe_bool_text(v);
		यदि (res < 0) {
			यदि (!strcasecmp(v, "single"))
				allow_pack_reuse = SINGLE_PACK_REUSE;
			अन्यथा यदि (!strcasecmp(v, "multi"))
				allow_pack_reuse = MULTI_PACK_REUSE;
			अन्यथा
				die(_("invalid pack.allowPackReuse value: '%s'"), v);
		} अन्यथा यदि (res) {
			allow_pack_reuse = SINGLE_PACK_REUSE;
		} अन्यथा {
			allow_pack_reuse = NO_PACK_REUSE;
		}
		प्रत्यागम 0;
	}
	यदि (!strcmp(k, "pack.threads")) {
		delta_शोध_threads = git_config_int(k, v, ctx->kvi);
		यदि (delta_शोध_threads < 0)
			die(_("invalid number of threads specified (%d)"),
			    delta_शोध_threads);
		यदि (!HAVE_THREADS && delta_शोध_threads != 1) {
			warning(_("no threads support, ignoring %s"), k);
			delta_शोध_threads = 0;
		}
		प्रत्यागम 0;
	}
	यदि (!strcmp(k, "pack.indexversion")) {
		pack_idx_opts.version = git_config_int(k, v, ctx->kvi);
		यदि (pack_idx_opts.version > 2)
			die(_("bad pack.indexVersion=%"PRIu32),
			    pack_idx_opts.version);
		प्रत्यागम 0;
	}
	यदि (!strcmp(k, "pack.writereverseindex")) {
		यदि (git_config_bool(k, v))
			pack_idx_opts.flags |= WRITE_REV;
		अन्यथा
			pack_idx_opts.flags &= ~WRITE_REV;
		प्रत्यागम 0;
	}
	यदि (!strcmp(k, "uploadpack.blobpackfileuri")) {
		संरचना configured_exclusion *ex;
		ध्रुव वर्ण *oid_end, *pack_end;
		/*
		 * Stores the pack hash. This is not a true object ID, but is
		 * of the same form.
		 */
		संरचना object_id pack_hash;

		यदि (!v)
			प्रत्यागम config_error_nonbool(k);

		ex = xmalloc(परिमाण(*ex));
		यदि (parse_oid_hex(v, &ex->e.oid, &oid_end) ||
		    *oid_end != ' ' ||
		    parse_oid_hex(oid_end + 1, &pack_hash, &pack_end) ||
		    *pack_end != ' ')
			die(_("value of uploadpack.blobpackfileuri must be "
			      "of the form '<object-hash> <pack-hash> <uri>' (got '%s')"), v);
		यदि (oidmap_get(&configured_exclusions, &ex->e.oid))
			die(_("object already configured in another "
			      "uploadpack.blobpackfileuri (got '%s')"), v);
		ex->pack_hash_hex = xcalloc(1, pack_end - oid_end);
		memcpy(ex->pack_hash_hex, oid_end + 1, pack_end - oid_end - 1);
		ex->uri = xstrdup(pack_end + 1);
		oidmap_put(&configured_exclusions, ex);
	}
	प्रत्यागम git_default_config(k, v, ctx, cb);
}

/* Counters for trace2 output when in --stdin-packs mode. */
स्थिर पूर्णाङ्क stdin_packs_found_nr;
स्थिर पूर्णाङ्क stdin_packs_hints_nr;

स्थिर पूर्णाङ्क add_object_entry_from_pack(ध्रुव संरचना object_id *oid,
				      संरचना packed_git *p,
				      uint32_t pos,
				      शून्य *_data)
{
	off_t ofs;
	संरचना object_info oi = OBJECT_INFO_INIT;
	गणना object_type type = OBJ_NONE;

	display_progress(progress_state, ++nr_seen);

	यदि (have_duplicate_entry(oid, 0))
		प्रत्यागम 0;

	stdin_packs_found_nr++;

	ofs = nth_packed_object_offset(p, pos);

	oi.typep = &type;
	यदि (packed_object_info(रिक्त, p, ofs, &oi) < 0) {
		die(_("could not get type of object %s in pack %s"),
		    oid_to_hex(oid), p->pack_name);
	} अन्यथा यदि (type == OBJ_COMMIT) {
		संरचना rev_info *revs = _data;
		/*
		 * commits in included packs are used as starting points
		 * for the subsequent revision walk
		 *
		 * Note that we do want to walk through commits that are
		 * present in excluded-open ('!') packs to pick up any
		 * objects reachable from them not present in the
		 * excluded-closed ('^') packs.
		 *
		 * However, we'll only add those objects to the packing
		 * list after checking `want_object_in_pack()` below.
		 */
		add_pending_oid(revs, रिक्त, oid, 0);
	}

	यदि (!want_object_in_pack(oid, 0, &p, &ofs))
		प्रत्यागम 0;

	create_object_entry(oid, type, 0, 0, 0, p, ofs);

	प्रत्यागम 0;
}

स्थिर शून्य show_object_pack_hint(संरचना object *object, ध्रुव वर्ण *name,
				  शून्य *data)
{
	गणना stdin_packs_mode mode = *(गणना stdin_packs_mode *)data;
	यदि (mode == STDIN_PACKS_MODE_FOLLOW) {
		यदि (object->type == OBJ_BLOB &&
		    !odb_has_object(the_repository->objects, &object->oid, 0))
			प्रत्यागम;
		add_object_entry(&object->oid, object->type, name, 0);
	} अन्यथा {
		संरचना object_entry *oe = packlist_find(&to_pack, &object->oid);
		यदि (!oe)
			प्रत्यागम;

		/*
		 * Our 'to_pack' list was constructed by iterating all
		 * objects packed in included packs, and so doesn't have
		 * a non-zero hash field that you would typically pick
		 * up during a reachability traversal.
		 *
		 * Make a best-effort attempt to fill in the ->hash and
		 * ->no_try_delta fields here in order to perhaps
		 * improve the delta selection process.
		 */
		oe->hash = pack_name_hash_fn(name);
		oe->no_try_delta = name && no_try_delta(name);

		stdin_packs_hints_nr++;
	}
}

स्थिर शून्य show_commit_pack_hint(संरचना commit *commit, शून्य *data)
{
	गणना stdin_packs_mode mode = *(गणना stdin_packs_mode *)data;

	यदि (mode == STDIN_PACKS_MODE_FOLLOW) {
		show_object_pack_hint((संरचना object *)commit, "", data);
		प्रत्यागम;
	}

	/* nothing to do; commits don't have a namehash */

}

/*
 * stdin_pack_info_kind specifies how a pack specified over stdin
 * should be treated when pack-objects is invoked with --stdin-packs.
 *
 *  - STDIN_PACK_INCLUDE: objects in any packs with this flag bit set
 *    should be included in the output pack, unless they appear in an
 *    excluded pack.
 *
 *  - STDIN_PACK_EXCLUDE_CLOSED: objects in any packs with this flag
 *    bit set should be excluded from the output pack.
 *
 *  - STDIN_PACK_EXCLUDE_OPEN: objects in any packs with this flag
 *    bit set should be excluded from the output pack, but are not
 *    guaranteed to be closed under reachability.
 *
 * Objects in packs whose 'kind' bits include STDIN_PACK_INCLUDE or
 * STDIN_PACK_EXCLUDE_OPEN are used as traversal tips when invoked
 * with --stdin-packs=follow.
 */
गणना stdin_pack_info_kind {
	STDIN_PACK_INCLUDE = (1<<0),
	STDIN_PACK_EXCLUDE_CLOSED = (1<<1),
	STDIN_PACK_EXCLUDE_OPEN = (1<<2),
};

संरचना stdin_pack_info {
	संरचना packed_git *p;
	गणना stdin_pack_info_kind kind;
};

स्थिर पूर्णाङ्क pack_mtime_cmp(ध्रुव शून्य *_a, ध्रुव शून्य *_b)
{
	संरचना stdin_pack_info *a = ((ध्रुव संरचना string_list_item*)_a)->util;
	संरचना stdin_pack_info *b = ((ध्रुव संरचना string_list_item*)_b)->util;

	/*
	 * order packs by descending mtime so that objects are laid out
	 * roughly as newest-to-oldest
	 */
	यदि (a->p->mtime < b->p->mtime)
		प्रत्यागम 1;
	अन्यथा यदि (b->p->mtime < a->p->mtime)
		प्रत्यागम -1;
	अन्यथा
		प्रत्यागम 0;
}

स्थिर पूर्णाङ्क stdin_packs_include_check_obj(संरचना object *obj, शून्य *data UNUSED)
{
	प्रत्यागम !has_object_kept_pack(to_pack.repo, &obj->oid,
				     KEPT_PACK_IN_CORE);
}

स्थिर पूर्णाङ्क stdin_packs_include_check(संरचना commit *commit, शून्य *data)
{
	प्रत्यागम stdin_packs_include_check_obj((संरचना object *)commit, data);
}

स्थिर शून्य stdin_packs_add_pack_entries(संरचना strmap *packs,
					 संरचना rev_info *revs)
{
	संरचना string_list किल्ल्या = STRING_LIST_INIT_NODUP;
	संरचना string_list_item *item;
	संरचना hashmap_iter iter;
	संरचना strmap_entry *entry;

	strmap_for_each_entry(packs, &iter, entry) {
		संरचना stdin_pack_info *info = entry->मूल्य;
		यदि (!info->p)
			die(_("could not find pack '%s'"), entry->किल्ली);

		string_list_append(&किल्ल्या, entry->किल्ली)->util = info;
	}

	/*
	 * Order packs by ascending mtime; use QSORT directly to access the
	 * string_list_item's ->util pointer, which string_list_sort() does not
	 * provide.
	 */
	QSORT(किल्ल्या.वस्तुसंच, किल्ल्या.nr, pack_mtime_cmp);

	for_each_string_list_item(item, &किल्ल्या) {
		संरचना stdin_pack_info *info = item->util;

		यदि (info->kind & STDIN_PACK_EXCLUDE_OPEN) {
			/*
			 * When open-excluded packs ("!") are present, stop
			 * the parent walk at closed-excluded ("^") packs.
			 * Objects behind a "^" boundary are guaranteed to
			 * have closure and should not be rescued.
			 */
			revs->include_check = stdin_packs_include_check;
			revs->include_check_obj = stdin_packs_include_check_obj;
		}

		यदि ((info->kind & STDIN_PACK_INCLUDE) ||
		    (info->kind & STDIN_PACK_EXCLUDE_OPEN))
			for_each_object_in_pack(info->p,
						add_object_entry_from_pack,
						revs,
						ODB_FOR_EACH_OBJECT_PACK_ORDER);
	}

	string_list_clear(&किल्ल्या, 0);
}

स्थिर शून्य stdin_packs_read_आदान(संरचना rev_info *revs,
				   गणना stdin_packs_mode mode)
{
	संरचना strbuf buf = STRBUF_INIT;
	संरचना strmap packs = STRMAP_INIT;
	संरचना packed_git *p;

	यावत् (strbuf_getline(&buf, stdin) != EOF) {
		संरचना stdin_pack_info *info;
		गणना stdin_pack_info_kind kind = STDIN_PACK_INCLUDE;
		ध्रुव वर्ण *किल्ली = buf.buf;

		यदि (!*किल्ली)
			अनुवर्त;
		अन्यथा यदि (*किल्ली == '^')
			kind = STDIN_PACK_EXCLUDE_CLOSED;
		अन्यथा यदि (*किल्ली == '!' && mode == STDIN_PACKS_MODE_FOLLOW)
			kind = STDIN_PACK_EXCLUDE_OPEN;

		यदि (kind != STDIN_PACK_INCLUDE)
			किल्ली++;

		info = strmap_get(&packs, किल्ली);
		यदि (!info) {
			CALLOC_ARRAY(info, 1);
			strmap_put(&packs, किल्ली, info);
		}

		info->kind |= kind;

		strbuf_reset(&buf);
	}

	repo_for_each_pack(the_repository, p) {
		संरचना stdin_pack_info *info;

		info = strmap_get(&packs, pack_basename(p));
		यदि (!info)
			अनुवर्त;

		यदि (info->kind & STDIN_PACK_INCLUDE) {
			यदि (exclude_promisor_objects && p->pack_promisor)
				die(_("packfile %s is a promisor but --exclude-promisor-objects was given"), p->pack_name);

			/*
			 * Arguments we got on stdin may not even be
			 * packs. First check that to avoid segfaulting
			 * later on in e.g.  pack_mtime_cmp(), excluded
			 * packs are handled below.
			 */
			यदि (!is_pack_valid(p))
				die(_("packfile %s cannot be accessed"), p->pack_name);
		}

		यदि (info->kind & STDIN_PACK_EXCLUDE_CLOSED) {
			/*
			 * Marking excluded packs as kept in-core so
			 * that later calls to add_object_entry()
			 * discards any objects that are also found in
			 * excluded packs.
			 */
			p->pack_keep_in_core = 1;
		}

		यदि (info->kind & STDIN_PACK_EXCLUDE_OPEN) {
			/*
			 * Marking excluded open packs as kept in-core
			 * (open) for the same reason as we marked
			 * exclude closed packs as kept in-core.
			 *
			 * Use a separate flag here to ensure we don't
			 * halt our traversal at these packs, since they
			 * are not guaranteed to have closure.
			 *
			 */
			p->pack_keep_in_core_open = 1;
		}

		info->p = p;
	}

	stdin_packs_add_pack_entries(&packs, revs);

	strbuf_release(&buf);
	strmap_clear(&packs, 1);
}

स्थिर शून्य add_unreachable_loose_objects(संरचना rev_info *revs);

स्थिर शून्य read_stdin_packs(संरचना repository *repo,
			     गणना stdin_packs_mode mode, पूर्णाङ्क rev_list_unpacked)
{
	पूर्णाङ्क prev_fetch_if_missing = repo->fetch_if_missing;
	संरचना rev_info revs;

	/*
	 * The revision walk may hit objects that are promised, only. As the
	 * walk is best-effort though we don't want to perform backfill fetches
	 * for them.
	 */
	repo->fetch_if_missing = 0;

	repo_init_revisions(repo, &revs, रिक्त);
	/*
	 * Use a revision walk to fill in the namehash of objects in the include
	 * packs. To save time, we'll avoid traversing through objects that are
	 * in excluded packs.
	 *
	 * That may cause us to avoid populating all of the namehash fields of
	 * all included objects, but our goal is best-effort, since this is only
	 * an optimization during delta selection.
	 */
	revs.no_kept_objects = 1;
	revs.keep_pack_cache_flags |= KEPT_PACK_IN_CORE;
	revs.blob_objects = 1;
	revs.वृक्ष_objects = 1;
	revs.tag_objects = 1;
	revs.ignore_missing_links = 1;
	revs.exclude_promisor_objects = exclude_promisor_objects;

	/* avoids adding objects in excluded packs */
	ignore_packed_keep_in_core = 1;
	यदि (mode == STDIN_PACKS_MODE_FOLLOW) {
		/*
		 * In '--stdin-packs=follow' mode, additionally ignore
		 * objects in excluded-open packs to prevent them from
		 * appearing in the resulting pack.
		 */
		ignore_packed_keep_in_core_open = 1;
	}
	stdin_packs_read_आदान(&revs, mode);
	यदि (rev_list_unpacked)
		add_unreachable_loose_objects(&revs);

	यदि (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	traverse_commit_list(&revs,
			     show_commit_pack_hint,
			     show_object_pack_hint,
			     &mode);

	release_revisions(&revs);

	trace2_data_intmax("pack-objects", the_repository, "stdin_packs_found",
			   stdin_packs_found_nr);
	trace2_data_intmax("pack-objects", the_repository, "stdin_packs_hints",
			   stdin_packs_hints_nr);

	repo->fetch_if_missing = prev_fetch_if_missing;
}

स्थिर शून्य add_cruft_object_entry(ध्रुव संरचना object_id *oid, गणना object_type type,
				   संरचना packed_git *pack, off_t offset,
				   ध्रुव वर्ण *name, uint32_t mtime)
{
	संरचना object_entry *entry;

	display_progress(progress_state, ++nr_seen);

	entry = packlist_find(&to_pack, oid);
	यदि (entry) {
		यदि (name) {
			entry->hash = pack_name_hash_fn(name);
			entry->no_try_delta = no_try_delta(name);
		}
	} अन्यथा {
		यदि (!want_object_in_pack_mtime(oid, 0, &pack, &offset, mtime))
			प्रत्यागम;
		यदि (!pack && type == OBJ_BLOB) {
			संरचना odb_source_files *files =
				odb_source_files_downcast(the_repository->objects->source);
			पूर्णाङ्क found = 0;

			प्रति (संरचना odb_files_dir *dir = files->dirs; !found && dir; dir = dir->next)
				यदि (!odb_source_read_object_info(&dir->loose->base, oid, रिक्त, 0, रिक्त))
					found = 1;

			/*
			 * If a traversed tree has a missing blob then we want
			 * to avoid adding that missing object to our pack.
			 *
			 * This only applies to missing blobs, not trees,
			 * because the traversal needs to parse sub-trees but
			 * not blobs.
			 *
			 * Note we only perform this check when we couldn't
			 * already find the object in a pack, so we're really
			 * limited to "ensure non-tip blobs which don't exist in
			 * packs do exist via loose objects". Confused?
			 */
			यदि (!found)
				प्रत्यागम;
		}

		entry = create_object_entry(oid, type, pack_name_hash_fn(name),
					    0, name && no_try_delta(name),
					    pack, offset);
	}

	यदि (mtime > oe_cruft_mtime(&to_pack, entry))
		oe_set_cruft_mtime(&to_pack, entry, mtime);
	प्रत्यागम;
}

स्थिर शून्य show_cruft_object(संरचना object *obj, ध्रुव वर्ण *name, शून्य *data UNUSED)
{
	/*
	 * if we did not record it earlier, it's at least as old as our
	 * expiration value. Rather than find it exactly, just use that
	 * value.  This may bump it forward from its real mtime, but it
	 * will still be "too old" next time we run with the same
	 * expiration.
	 *
	 * if obj does appear in the packing list, this call is a noop (or may
	 * set the namehash).
	 */
	add_cruft_object_entry(&obj->oid, obj->type, रिक्त, 0, name, cruft_expiration);
}

स्थिर शून्य show_cruft_commit(संरचना commit *commit, शून्य *data)
{
	show_cruft_object((संरचना object*)commit, रिक्त, data);
}

स्थिर पूर्णाङ्क cruft_include_check_obj(संरचना object *obj, शून्य *data UNUSED)
{
	प्रत्यागम !has_object_kept_pack(to_pack.repo, &obj->oid, KEPT_PACK_IN_CORE);
}

स्थिर पूर्णाङ्क cruft_include_check(संरचना commit *commit, शून्य *data)
{
	प्रत्यागम cruft_include_check_obj((संरचना object*)commit, data);
}

स्थिर शून्य set_cruft_mtime(ध्रुव संरचना object *object,
			    संरचना packed_git *pack,
			    off_t offset, time_t mtime)
{
	add_cruft_object_entry(&object->oid, object->type, pack, offset, रिक्त,
			       mtime);
}

स्थिर शून्य mark_pack_kept_in_core(संरचना string_list *packs, unsigned keep)
{
	संरचना string_list_item *item = रिक्त;
	for_each_string_list_item(item, packs) {
		संरचना packed_git *p = item->util;
		यदि (!p)
			die(_("could not find pack '%s'"), item->string);
		यदि (p->is_cruft && keep)
			ignore_packed_keep_in_core_has_cruft = 1;
		p->pack_keep_in_core = keep;
	}
}

स्थिर शून्य add_objects_in_unpacked_packs(शून्य);

स्थिर शून्य enumerate_cruft_objects(शून्य)
{
	यदि (progress)
		progress_state = start_progress(the_repository,
						_("Enumerating cruft objects"), 0);

	add_objects_in_unpacked_packs();
	add_unreachable_loose_objects(रिक्त);

	stop_progress(&progress_state);
}

स्थिर शून्य enumerate_and_traverse_cruft_objects(संरचना string_list *fresh_packs)
{
	संरचना packed_git *p;
	संरचना rev_info revs;
	पूर्णाङ्क ret;

	repo_init_revisions(the_repository, &revs, रिक्त);

	revs.tag_objects = 1;
	revs.वृक्ष_objects = 1;
	revs.blob_objects = 1;

	revs.include_check = cruft_include_check;
	revs.include_check_obj = cruft_include_check_obj;

	revs.ignore_missing_links = 1;

	यदि (progress)
		progress_state = start_progress(the_repository,
						_("Enumerating cruft objects"), 0);
	ret = add_unseen_recent_objects_to_traversal(&revs, cruft_expiration,
						     set_cruft_mtime, 1);
	stop_progress(&progress_state);

	यदि (ret)
		die(_("unable to add cruft objects"));

	/*
	 * Re-mark only the fresh packs as kept so that objects in
	 * unknown packs do not halt the reachability traversal early.
	 */
	repo_for_each_pack(the_repository, p)
		p->pack_keep_in_core = 0;
	mark_pack_kept_in_core(fresh_packs, 1);

	यदि (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	यदि (progress)
		progress_state = start_progress(the_repository,
						_("Traversing cruft objects"), 0);
	nr_seen = 0;
	traverse_commit_list(&revs, show_cruft_commit, show_cruft_object, रिक्त);

	stop_progress(&progress_state);
	release_revisions(&revs);
}

स्थिर शून्य read_cruft_objects(शून्य)
{
	संरचना strbuf buf = STRBUF_INIT;
	संरचना string_list discard_packs = STRING_LIST_INIT_DUP;
	संरचना string_list fresh_packs = STRING_LIST_INIT_DUP;
	संरचना packed_git *p;

	ignore_packed_keep_in_core = 1;

	यावत् (strbuf_getline(&buf, stdin) != EOF) {
		यदि (!buf.len)
			अनुवर्त;

		यदि (*buf.buf == '-')
			string_list_append(&discard_packs, buf.buf + 1);
		अन्यथा
			string_list_append(&fresh_packs, buf.buf);
	}

	string_list_sort(&discard_packs);
	string_list_sort(&fresh_packs);

	repo_for_each_pack(the_repository, p) {
		ध्रुव वर्ण *pack_name = pack_basename(p);
		संरचना string_list_item *item;

		item = string_list_lookup(&fresh_packs, pack_name);
		यदि (!item)
			item = string_list_lookup(&discard_packs, pack_name);

		यदि (item) {
			item->util = p;
		} अन्यथा {
			/*
			 * This pack wasn't mentioned in either the "fresh" or
			 * "discard" list, so the caller didn't know about it.
			 *
			 * Mark it as kept so that its objects are ignored by
			 * add_unseen_recent_objects_to_traversal(). We'll
			 * unmark it before starting the traversal so it doesn't
			 * halt the traversal early.
			 */
			p->pack_keep_in_core = 1;
		}
	}

	mark_pack_kept_in_core(&fresh_packs, 1);
	mark_pack_kept_in_core(&discard_packs, 0);

	यदि (cruft_expiration)
		enumerate_and_traverse_cruft_objects(&fresh_packs);
	अन्यथा
		enumerate_cruft_objects();

	strbuf_release(&buf);
	string_list_clear(&discard_packs, 0);
	string_list_clear(&fresh_packs, 0);
}

स्थिर शून्य read_object_list_from_stdin(शून्य)
{
	वर्ण ओळ[GIT_MAX_HEXSZ + 1 + PATH_MAX + 2];
	संरचना object_id oid;
	ध्रुव वर्ण *p;

	प्रति (;;) {
		यदि (!fgets(ओळ, परिमाण(ओळ), stdin)) {
			यदि (feof(stdin))
				भङ्ग;
			यदि (!ferror(stdin))
				BUG("fgets returned NULL, not EOF, not error!");
			यदि (errno != EINTR)
				die_errno("fgets");
			clearerr(stdin);
			अनुवर्त;
		}
		यदि (ओळ[0] == '-') {
			यदि (get_oid_hex(ओळ+1, &oid))
				die(_("expected edge object ID, got garbage:\n %s"),
				    ओळ);
			add_preferred_base(&oid);
			अनुवर्त;
		}
		यदि (parse_oid_hex(ओळ, &oid, &p))
			die(_("expected object ID, got garbage:\n %s"), ओळ);

		add_preferred_base_object(p + 1);
		add_object_entry(&oid, OBJ_NONE, p + 1, 0);
	}
}

स्थिर शून्य show_commit(संरचना commit *commit, शून्य *data UNUSED)
{
	add_object_entry(&commit->object.oid, OBJ_COMMIT, रिक्त, 0);

	यदि (write_bitmap_index)
		index_commit_for_bitmap(commit);

	यदि (use_delta_islands)
		propagate_island_marks(the_repository, commit);
}

स्थिर शून्य show_object(संरचना object *obj, ध्रुव वर्ण *name,
			शून्य *data UNUSED)
{
	add_preferred_base_object(name);
	add_object_entry(&obj->oid, obj->type, name, 0);

	यदि (use_delta_islands)
		record_वृक्ष_depth(&obj->oid, name);
}

स्थिर शून्य show_object__ma_allow_any(संरचना object *obj, ध्रुव वर्ण *name, शून्य *data)
{
	प्रतिज्ञा(arg_missing_action == MA_ALLOW_ANY);

	/*
	 * Quietly ignore ALL missing objects.  This avoids problems with
	 * staging them now and getting an odd error later.
	 */
	यदि (!odb_has_object(the_repository->objects, &obj->oid, 0))
		प्रत्यागम;

	show_object(obj, name, data);
}

स्थिर शून्य show_object__ma_allow_promisor(संरचना object *obj, ध्रुव वर्ण *name, शून्य *data)
{
	प्रतिज्ञा(arg_missing_action == MA_ALLOW_PROMISOR);

	/*
	 * Quietly ignore EXPECTED missing objects.  This avoids problems with
	 * staging them now and getting an odd error later.
	 */
	यदि (!odb_has_object(the_repository->objects, &obj->oid, 0) &&
	    is_promisor_object(to_pack.repo, &obj->oid))
		प्रत्यागम;

	show_object(obj, name, data);
}

स्थिर पूर्णाङ्क option_parse_missing_action(ध्रुव संरचना option *opt,
				       ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	संरचना repository *repo = opt->मूल्य;

	प्रतिज्ञा(arg);
	प्रतिज्ञा(!unset);

	यदि (!strcmp(arg, "error")) {
		arg_missing_action = MA_ERROR;
		fn_show_object = show_object;
		प्रत्यागम 0;
	}

	यदि (!strcmp(arg, "allow-any")) {
		arg_missing_action = MA_ALLOW_ANY;
		repo->fetch_if_missing = 0;
		fn_show_object = show_object__ma_allow_any;
		प्रत्यागम 0;
	}

	यदि (!strcmp(arg, "allow-promisor")) {
		arg_missing_action = MA_ALLOW_PROMISOR;
		repo->fetch_if_missing = 0;
		fn_show_object = show_object__ma_allow_promisor;
		प्रत्यागम 0;
	}

	die(_("invalid value for '%s': '%s'"), "--missing", arg);
	प्रत्यागम 0;
}

स्थिर शून्य show_edge(संरचना commit *commit)
{
	add_preferred_base(&commit->object.oid);
}

स्थिर पूर्णाङ्क add_object_in_unpacked_pack(ध्रुव संरचना object_id *oid,
				       संरचना object_info *oi,
				       शून्य *data UNUSED)
{
	यदि (cruft) {
		add_cruft_object_entry(oid, OBJ_NONE, oi->source_infop->u.packed.pack,
				       oi->source_infop->u.packed.offset, रिक्त,
				       *oi->mtimep);
	} अन्यथा {
		add_object_entry(oid, OBJ_NONE, "", 0);
	}
	प्रत्यागम 0;
}

स्थिर शून्य add_objects_in_unpacked_packs(शून्य)
{
	संरचना odb_source_files *files =
		odb_source_files_downcast(to_pack.repo->objects->source);
	time_t mtime;
	संरचना odb_for_each_object_options opts = {
		.flags = ODB_FOR_EACH_OBJECT_PACK_ORDER |
			 ODB_FOR_EACH_OBJECT_LOCAL_ONLY |
			 ODB_FOR_EACH_OBJECT_SKIP_IN_CORE_KEPT_PACKS |
			 ODB_FOR_EACH_OBJECT_SKIP_ON_DISK_KEPT_PACKS,
	};
	संरचना odb_source_info source_info;
	संरचना object_info oi = {
		.mtimep = &mtime,
		.source_infop = &source_info,
	};

	प्रति (संरचना odb_files_dir *dir = files->dirs; dir; dir = dir->next) {
		यदि (!dir->local)
			अनुवर्त;

		यदि (odb_source_for_each_object(&dir->packed->base, &oi,
					       add_object_in_unpacked_pack, रिक्त, &opts))
			die(_("cannot open pack index"));
	}
}

स्थिर पूर्णाङ्क add_loose_object(ध्रुव संरचना object_id *oid, ध्रुव वर्ण *path,
			    शून्य *data)
{
	संरचना rev_info *revs = data;
	गणना object_type type = odb_read_object_info(the_repository->objects, oid, रिक्त);

	यदि (type < 0) {
		warning(_("loose object at %s could not be examined"), path);
		प्रत्यागम 0;
	}

	यदि (cruft) {
		संरचना stat st;
		यदि (stat(path, &st) < 0) {
			यदि (errno == ENOENT)
				प्रत्यागम 0;
			प्रत्यागम error_errno("unable to stat %s", oid_to_hex(oid));
		}

		add_cruft_object_entry(oid, type, रिक्त, 0, रिक्त,
				       st.st_mtime);
	} अन्यथा {
		add_object_entry(oid, type, "", 0);
	}

	यदि (revs && type == OBJ_COMMIT)
		add_pending_oid(revs, रिक्त, oid, 0);

	प्रत्यागम 0;
}

/*
 * We actually don't even have to worry about reachability here.
 * add_object_entry will weed out duplicates, so we just add every
 * loose object we find.
 */
स्थिर शून्य add_unreachable_loose_objects(संरचना rev_info *revs)
{
	for_each_loose_file_in_source(the_repository->objects->source,
				      add_loose_object, रिक्त, रिक्त, revs);
}

स्थिर पूर्णाङ्क has_sha1_pack_kept_or_nonlocal(ध्रुव संरचना object_id *oid)
{
	स्थिर संरचना packed_git *last_found = रिक्त;
	संरचना packed_git *p;

	यदि (last_found && find_pack_entry_one(oid, last_found))
		प्रत्यागम 1;

	repo_for_each_pack(the_repository, p) {
		/*
		 * We have already checked `last_found`, so there is no need to
		 * re-check here.
		 */
		यदि (p == last_found)
			अनुवर्त;

		यदि ((!p->pack_local || p->pack_keep || p->pack_keep_in_core) &&
		    find_pack_entry_one(oid, p)) {
			last_found = p;
			प्रत्यागम 1;
		}
	}

	प्रत्यागम 0;
}

/*
 * Store a list of sha1s that are should not be discarded
 * because they are either written too recently, or are
 * reachable from another object that was.
 *
 * This is filled by get_object_list.
 */
स्थिर संरचना oid_array recent_objects;

स्थिर पूर्णाङ्क loosened_object_can_be_discarded(ध्रुव संरचना object_id *oid,
					    timestamp_t mtime)
{
	यदि (!unpack_unreachable_expiration)
		प्रत्यागम 0;
	यदि (mtime > unpack_unreachable_expiration)
		प्रत्यागम 0;
	यदि (oid_array_lookup(&recent_objects, oid) >= 0)
		प्रत्यागम 0;
	प्रत्यागम 1;
}

स्थिर पूर्णाङ्क force_object_loose(संरचना odb_source *source,
			      ध्रुव संरचना object_id *oid,
			      ध्रुव time_t *mtime)
{
	संरचना odb_source_files *files = odb_source_files_downcast(source);
	ध्रुव संरचना git_hash_algo *compat = source->odb->repo->compat_hash_algo;
	संरचना object_info oi = OBJECT_INFO_INIT;
	संरचना object_id compat_oid, *compat_oid_p = रिक्त;
	गणना object_type type;
	शून्य *buf = रिक्त;
	size_t len;
	पूर्णाङ्क ret;

	प्रति (संरचना odb_files_dir *dir = files->dirs; dir; dir = dir->next)
		यदि (!odb_source_read_object_info(&dir->loose->base, oid, रिक्त, 0, रिक्त))
			प्रत्यागम 0;

	oi.typep = &type;
	oi.sizep = &len;
	oi.contentp = &buf;
	यदि (odb_read_object_info_extended(source->odb, oid, &oi, 0)) {
		ret = error(_("cannot read object for %s"), oid_to_hex(oid));
		गच्छ out;
	}

	यदि (compat) {
		यदि (repo_oid_to_algop(source->odb->repo, oid, compat, &compat_oid)) {
			ret = error(_("cannot map object %s to %s"),
				    oid_to_hex(oid), compat->name);
			गच्छ out;
		}

		compat_oid_p = &compat_oid;
	}

	ret = odb_source_write_object(&files->dirs->loose->base, buf, len, type, oid,
				      compat_oid_p, mtime, 0);

out:
	मुक्त(buf);
	प्रत्यागम ret;
}

स्थिर शून्य loosen_unused_packed_objects(शून्य)
{
	संरचना packed_git *p;
	uint32_t i;
	uint32_t loosened_objects_nr = 0;
	संरचना object_id oid;

	repo_for_each_pack(the_repository, p) {
		यदि (!p->pack_local || p->pack_keep || p->pack_keep_in_core)
			अनुवर्त;

		यदि (open_pack_index(p))
			die(_("cannot open pack index"));

		प्रति (i = 0; i < p->अंक_objects; i++) {
			nth_packed_object_id(&oid, p, i);
			यदि (!packlist_find(&to_pack, &oid) &&
			    !has_sha1_pack_kept_or_nonlocal(&oid) &&
			    !loosened_object_can_be_discarded(&oid, p->mtime)) {
				यदि (force_object_loose(the_repository->objects->source,
						       &oid, &p->mtime))
					die(_("unable to force loose object"));
				loosened_objects_nr++;
			}
		}
	}

	trace2_data_intmax("pack-objects", the_repository,
			   "loosen_unused_packed_objects/loosened", loosened_objects_nr);
}

/*
 * This tracks any options which pack-reuse code expects to be on, or which a
 * reader of the pack might not understand, and which would therefore prevent
 * blind reuse of what we have on disk.
 */
स्थिर पूर्णाङ्क pack_options_allow_reuse(शून्य)
{
	प्रत्यागम allow_pack_reuse != NO_PACK_REUSE &&
	       pack_to_stdout &&
	       !ignore_packed_keep_on_disk &&
	       !ignore_packed_keep_in_core &&
	       (!local || !have_non_local_packs) &&
	       !incremental;
}

स्थिर पूर्णाङ्क get_object_list_from_bitmap(संरचना rev_info *revs)
{
	यदि (!(bitmap_git = prepare_bitmap_walk(revs, 0)))
		प्रत्यागम -1;

	/*
	 * For now, force the name-hash version to be 1 since that
	 * is the version implied by the bitmap format. Later, the
	 * format can include this version explicitly in its format,
	 * allowing readers to know the version that was used during
	 * the bitmap write.
	 */
	name_hash_version = 1;

	यदि (pack_options_allow_reuse())
		reuse_partial_packfile_from_bitmap(bitmap_git,
						   &reuse_packfiles,
						   &reuse_packfiles_nr,
						   &reuse_packfile_bitmap,
						   allow_pack_reuse == MULTI_PACK_REUSE);

	यदि (reuse_packfiles) {
		reuse_packfile_objects = bitmap_popcount(reuse_packfile_bitmap);
		यदि (!reuse_packfile_objects)
			BUG("expected non-empty reuse bitmap");

		nr_result += reuse_packfile_objects;
		nr_seen += reuse_packfile_objects;
		display_progress(progress_state, nr_seen);
	}

	traverse_bitmap_commit_list(bitmap_git, revs,
				    &add_object_entry_from_bitmap);
	प्रत्यागम 0;
}

स्थिर शून्य record_recent_object(संरचना object *obj,
				 ध्रुव वर्ण *name UNUSED,
				 शून्य *data UNUSED)
{
	oid_array_append(&recent_objects, &obj->oid);
}

स्थिर शून्य record_recent_commit(संरचना commit *commit, शून्य *data UNUSED)
{
	oid_array_append(&recent_objects, &commit->object.oid);
}

स्थिर पूर्णाङ्क mark_bitmap_preferred_tip(ध्रुव संरचना reference *ref, शून्य *data UNUSED)
{
	ध्रुव संरचना object_id *maybe_peeled = ref->oid;
	संरचना object_id peeled;
	संरचना object *object;

	यदि (!reference_get_peeled_oid(the_repository, ref, &peeled))
		maybe_peeled = &peeled;

	object = parse_object_or_die(the_repository, maybe_peeled, ref->name);
	यदि (object->type == OBJ_COMMIT)
		object->flags |= NEEDS_BITMAP;

	प्रत्यागम 0;
}

स्थिर अन्तःस्थ पूर्णाङ्क is_oid_uninteresting(संरचना repository *repo,
				       संरचना object_id *oid)
{
	संरचना object *o = lookup_object(repo, oid);
	प्रत्यागम !o || (o->flags & UNINTERESTING);
}

स्थिर पूर्णाङ्क add_objects_by_path(ध्रुव वर्ण *path,
			       संरचना oid_array *oids,
			       गणना object_type type,
			       शून्य *data)
{
	size_t oe_start = to_pack.nr_objects;
	size_t oe_end;
	unsigned पूर्णाङ्क *processed = data;

	/*
	 * First, add all objects to the packing data, including the ones
	 * marked UNINTERESTING (translated to 'exclude') as they can be
	 * used as delta bases.
	 */
	प्रति (size_t i = 0; i < oids->nr; i++) {
		पूर्णाङ्क exclude;
		संरचना object_info oi = OBJECT_INFO_INIT;
		संरचना object_id *oid = &oids->oid[i];

		/* Skip objects that do not exist locally. */
		यदि ((exclude_promisor_objects || arg_missing_action != MA_ERROR) &&
		    odb_read_object_info_extended(the_repository->objects, oid, &oi,
						  OBJECT_INFO_FOR_PREFETCH) < 0)
			अनुवर्त;

		exclude = is_oid_uninteresting(the_repository, oid);

		यदि (exclude && !thin)
			अनुवर्त;

		add_object_entry(oid, type, path, exclude);

		यदि (type == OBJ_COMMIT) {
			संरचना commit *commit;

			यदि (!write_bitmap_index && !use_delta_islands)
				अनुवर्त;

			commit = lookup_commit(the_repository, oid);
			यदि (!commit)
				die(_("could not find commit %s"), oid_to_hex(oid));
			यदि (write_bitmap_index)
				index_commit_for_bitmap(commit);
			/*
			 * Skip island propagation for boundary commits.
			 * The regular traversal's show_commit() is only
			 * called for interesting commits; matching that
			 * here keeps path-walk from doing extra work that
			 * would only be a no-op anyway (boundary commits
			 * are not in island_marks).
			 */
			यदि (use_delta_islands && !exclude)
				propagate_island_marks(the_repository, commit);
		} अन्यथा यदि (type == OBJ_वृक्ष && use_delta_islands) {
			record_वृक्ष_depth(oid, path);
		}
	}

	oe_end = to_pack.nr_objects;

	/* We can skip delta calculations if it is a no-op. */
	यदि (oe_end == oe_start || !window)
		प्रत्यागम 0;

	ALLOC_GROW(to_pack.regions,
		   to_pack.nr_regions + 1,
		   to_pack.nr_regions_alloc);

	to_pack.regions[to_pack.nr_regions].start = oe_start;
	to_pack.regions[to_pack.nr_regions].nr = oe_end - oe_start;
	to_pack.nr_regions++;

	*processed += oids->nr;
	display_progress(progress_state, *processed);

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क get_object_list_path_walk(संरचना rev_info *revs)
{
	संरचना path_walk_info info = PATH_WALK_INFO_INIT;
	unsigned पूर्णाङ्क processed = 0;
	पूर्णाङ्क result;

	info.revs = revs;
	info.path_fn = add_objects_by_path;
	info.path_fn_data = &processed;

	/*
	 * Path-walk needs boundary commits to discover thin-pack bases, but
	 * bitmap traversal does not understand the boundary state. Set it
	 * here so any prior bitmap attempt sees the usual non-boundary walk.
	 */
	revs->boundary = 1;

	/*
	 * Allow the --[no-]sparse option to be interesting here, if only
	 * for testing purposes. Paths with no interesting objects will not
	 * contribute to the resulting pack, but only create noisy preferred
	 * base objects.
	 */
	info.prune_all_uninteresting = sparse;
	info.edge_aggressive = shallow;

	trace2_region_enter("pack-objects", "path-walk", revs->repo);
	result = walk_objects_by_path(&info);
	trace2_region_leave("pack-objects", "path-walk", revs->repo);

	path_walk_info_clear(&info);

	प्रत्यागम result;
}

स्थिर शून्य get_object_list(संरचना rev_info *revs, संरचना strvec *argv)
{
	संरचना setup_revision_opt s_r_opt = {
		.allow_exclude_promisor_objects = 1,
	};
	संरचना repo_config_मूल्ये *cfg = repo_config_मूल्ये(the_repository);
	वर्ण ओळ[1000];
	पूर्णाङ्क flags = 0;
	पूर्णाङ्क save_warning;

	save_commit_buffer = 0;
	setup_revisions_from_strvec(argv, revs, &s_r_opt);

	/* make sure shallows are read */
	is_repository_shallow(the_repository);

	save_warning = cfg->warn_on_object_refname_ambiguity;
	cfg->warn_on_object_refname_ambiguity = 0;

	यावत् (fgets(ओळ, परिमाण(ओळ), stdin) != रिक्त) {
		पूर्णाङ्क len = strlen(ओळ);
		यदि (len && ओळ[len - 1] == '\n')
			ओळ[--len] = 0;
		यदि (!len)
			भङ्ग;
		यदि (*ओळ == '-') {
			यदि (!strcmp(ओळ, "--not")) {
				flags ^= UNINTERESTING;
				write_bitmap_index = 0;
				अनुवर्त;
			}
			यदि (starts_with(ओळ, "--shallow ")) {
				संरचना object_id oid;
				यदि (get_oid_hex(ओळ + 10, &oid))
					die("not an object name '%s'", ओळ + 10);
				register_shallow(the_repository, &oid);
				use_bitmap_index = 0;
				अनुवर्त;
			}
			die(_("not a rev '%s'"), ओळ);
		}
		यदि (handle_revision_arg(ओळ, revs, flags, REVARG_CANNOT_BE_FILENAME))
			die(_("bad revision '%s'"), ओळ);
	}

	cfg->warn_on_object_refname_ambiguity = save_warning;

	यदि (use_bitmap_index && !get_object_list_from_bitmap(revs))
		प्रत्यागम;

	यदि (use_delta_islands)
		load_delta_islands(the_repository, progress);

	यदि (write_bitmap_index)
		for_each_preferred_bitmap_tip(the_repository, mark_bitmap_preferred_tip,
					      रिक्त);

	यदि (!fn_show_object)
		fn_show_object = show_object;

	यदि (path_walk) {
		यदि (get_object_list_path_walk(revs)) {
			warning(_("failed to pack objects via path-walk"));
			path_walk = 0;
		}
	}

	यदि (!path_walk) {
		यदि (prepare_revision_walk(revs))
			die(_("revision walk setup failed"));
		mark_edges_uninteresting(revs, show_edge, sparse);
		traverse_commit_list(revs,
				show_commit, fn_show_object,
				रिक्त);
	}

	यदि (unpack_unreachable_expiration) {
		revs->ignore_missing_links = 1;
		यदि (add_unseen_recent_objects_to_traversal(revs,
				unpack_unreachable_expiration, रिक्त, 0))
			die(_("unable to add recent objects"));
		यदि (prepare_revision_walk(revs))
			die(_("revision walk setup failed"));
		traverse_commit_list(revs, record_recent_commit,
				     record_recent_object, रिक्त);
	}

	यदि (keep_unreachable)
		add_objects_in_unpacked_packs();
	यदि (pack_loose_unreachable)
		add_unreachable_loose_objects(रिक्त);
	यदि (unpack_unreachable)
		loosen_unused_packed_objects();

	oid_array_clear(&recent_objects);
}

स्थिर शून्य add_extra_kept_packs(ध्रुव संरचना string_list *names)
{
	संरचना packed_git *p;

	यदि (!names->nr)
		प्रत्यागम;

	repo_for_each_pack(the_repository, p) {
		ध्रुव वर्ण *name = basename(p->pack_name);
		पूर्णाङ्क i;

		यदि (!p->pack_local)
			अनुवर्त;

		प्रति (i = 0; i < names->nr; i++)
			यदि (!fspathcmp(name, names->वस्तुसंच[i].string))
				भङ्ग;

		यदि (i < names->nr) {
			p->pack_keep_in_core = 1;
			ignore_packed_keep_in_core = 1;
			अनुवर्त;
		}
	}
}

स्थिर पूर्णाङ्क option_parse_quiet(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg,
			      पूर्णाङ्क unset)
{
	पूर्णाङ्क *val = opt->मूल्य;

	BUG_ON_OPT_ARG(arg);

	यदि (!unset)
		*val = 0;
	अन्यथा यदि (!*val)
		*val = 1;
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क option_parse_index_version(ध्रुव संरचना option *opt,
				      ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	संरचना pack_idx_option *popts = opt->मूल्य;
	वर्ण *c;
	ध्रुव वर्ण *val = arg;

	BUG_ON_OPT_NEG(unset);

	popts->version = strtoul(val, &c, 10);
	यदि (popts->version > 2)
		die(_("unsupported index version %s"), val);
	यदि (*c == ',' && c[1])
		popts->off32_limit = strtoul(c+1, &c, 0);
	यदि (*c || popts->off32_limit & 0x80000000)
		die(_("bad index version '%s'"), val);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क option_parse_unpack_unreachable(ध्रुव संरचना option *opt UNUSED,
					   ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	यदि (unset) {
		unpack_unreachable = 0;
		unpack_unreachable_expiration = 0;
	}
	अन्यथा {
		unpack_unreachable = 1;
		यदि (arg)
			unpack_unreachable_expiration = approxidate(arg);
	}
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क option_parse_cruft_expiration(ध्रुव संरचना option *opt UNUSED,
					 ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	यदि (unset) {
		cruft = 0;
		cruft_expiration = 0;
	} अन्यथा {
		cruft = 1;
		यदि (arg)
			cruft_expiration = approxidate(arg);
	}
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क is_not_in_promisor_pack_obj(संरचना object *obj, शून्य *data UNUSED)
{
	संरचना odb_source_info source_info;
	संरचना object_info info = {
		.source_infop = &source_info,
	};

	यदि (odb_read_object_info_extended(the_repository->objects, &obj->oid, &info, 0))
		BUG("should_include_obj should only be called on existing objects");
	प्रत्यागम source_info.source->type != ODB_SOURCE_PACKED || !source_info.u.packed.pack->pack_promisor;
}

स्थिर पूर्णाङ्क is_not_in_promisor_pack(संरचना commit *commit, शून्य *data) {
	प्रत्यागम is_not_in_promisor_pack_obj((संरचना object *) commit, data);
}

स्थिर पूर्णाङ्क parse_stdin_packs_mode(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg,
				  पूर्णाङ्क unset)
{
	गणना stdin_packs_mode *mode = opt->मूल्य;

	यदि (unset)
		*mode = STDIN_PACKS_MODE_NONE;
	अन्यथा यदि (!arg || !*arg)
		*mode = STDIN_PACKS_MODE_STANDARD;
	अन्यथा यदि (!strcmp(arg, "follow"))
		*mode = STDIN_PACKS_MODE_FOLLOW;
	अन्यथा
		die(_("invalid value for '%s': '%s'"), opt->long_name, arg);

	प्रत्यागम 0;
}

पूर्णाङ्क cmd_pack_objects(पूर्णाङ्क argc,
		     ध्रुव वर्ण **argv,
		     ध्रुव वर्ण *prefix,
		     संरचना repository *repo)
{
	पूर्णाङ्क use_internal_rev_list = 0;
	पूर्णाङ्क all_progress_implied = 0;
	संरचना strvec rp = STRVEC_INIT;
	पूर्णाङ्क rev_list_unpacked = 0, rev_list_all = 0, rev_list_reflog = 0;
	पूर्णाङ्क rev_list_index = 0;
	गणना stdin_packs_mode stdin_packs = STDIN_PACKS_MODE_NONE;
	संरचना string_list keep_pack_list = STRING_LIST_INIT_NODUP;
	संरचना list_objects_filter_options filter_options =
		LIST_OBJECTS_FILTER_INIT;
	संरचना repo_config_मूल्ये *cfg = repo_config_मूल्ये(the_repository);

	संरचना option pack_objects_options[] = {
		OPT_CALLBACK_F('q', "quiet", &progress, रिक्त,
			       N_("do not show progress meter"),
			       PARSE_OPT_NOARG, option_parse_quiet),
		OPT_SET_INT(0, "progress", &progress,
			    N_("show progress meter"), 1),
		OPT_SET_INT(0, "all-progress", &progress,
			    N_("show progress meter during object writing phase"), 2),
		OPT_BOOL(0, "all-progress-implied",
			 &all_progress_implied,
			 N_("similar to --all-progress when progress meter is shown")),
		OPT_CALLBACK_F(0, "index-version", &pack_idx_opts, N_("<version>[,<offset>]"),
		  N_("write the pack index file in the specified idx format version"),
		  PARSE_OPT_NONEG, option_parse_index_version),
		OPT_UNSIGNED(0, "max-pack-size", &pack_size_limit,
			     N_("maximum size of each output pack file")),
		OPT_BOOL(0, "local", &local,
			 N_("ignore borrowed objects from alternate object store")),
		OPT_BOOL(0, "incremental", &incremental,
			 N_("ignore packed objects")),
		OPT_INTEGER(0, "window", &window,
			    N_("limit pack window by objects")),
		OPT_UNSIGNED(0, "window-memory", &window_memory_limit,
			     N_("limit pack window by memory in addition to object limit")),
		OPT_INTEGER(0, "depth", &depth,
			    N_("maximum length of delta chain allowed in the resulting pack")),
		OPT_BOOL(0, "reuse-delta", &reuse_delta,
			 N_("reuse existing deltas")),
		OPT_BOOL(0, "reuse-object", &reuse_object,
			 N_("reuse existing objects")),
		OPT_BOOL(0, "delta-base-offset", &allow_ofs_delta,
			 N_("use OFS_DELTA objects")),
		OPT_INTEGER(0, "threads", &delta_शोध_threads,
			    N_("use threads when searching for best delta matches")),
		OPT_BOOL(0, "non-empty", &non_empty,
			 N_("do not create an empty pack output")),
		OPT_BOOL(0, "revs", &use_internal_rev_list,
			 N_("read revision arguments from standard input")),
		OPT_SET_INT_F(0, "unpacked", &rev_list_unpacked,
			      N_("limit the objects to those that are not yet packed"),
			      1, PARSE_OPT_NONEG),
		OPT_SET_INT_F(0, "all", &rev_list_all,
			      N_("include objects reachable from any reference"),
			      1, PARSE_OPT_NONEG),
		OPT_SET_INT_F(0, "reflog", &rev_list_reflog,
			      N_("include objects referred by reflog entries"),
			      1, PARSE_OPT_NONEG),
		OPT_SET_INT_F(0, "indexed-objects", &rev_list_index,
			      N_("include objects referred to by the index"),
			      1, PARSE_OPT_NONEG),
		OPT_CALLBACK_F(0, "stdin-packs", &stdin_packs, N_("mode"),
			     N_("read packs from stdin"),
			     PARSE_OPT_OPTARG, parse_stdin_packs_mode),
		OPT_BOOL(0, "stdout", &pack_to_stdout,
			 N_("output pack to stdout")),
		OPT_BOOL(0, "include-tag", &include_tag,
			 N_("include tag objects that refer to objects to be packed")),
		OPT_BOOL(0, "keep-unreachable", &keep_unreachable,
			 N_("keep unreachable objects")),
		OPT_BOOL(0, "pack-loose-unreachable", &pack_loose_unreachable,
			 N_("pack loose unreachable objects")),
		OPT_CALLBACK_F(0, "unpack-unreachable", रिक्त, N_("time"),
		  N_("unpack unreachable objects newer than <time>"),
		  PARSE_OPT_OPTARG, option_parse_unpack_unreachable),
		OPT_BOOL(0, "cruft", &cruft, N_("create a cruft pack")),
		OPT_CALLBACK_F(0, "cruft-expiration", रिक्त, N_("time"),
		  N_("expire cruft objects older than <time>"),
		  PARSE_OPT_OPTARG, option_parse_cruft_expiration),
		OPT_BOOL(0, "sparse", &sparse,
			 N_("use the sparse reachability algorithm")),
		OPT_BOOL(0, "thin", &thin,
			 N_("create thin packs")),
		OPT_BOOL(0, "path-walk", &path_walk,
			 N_("use the path-walk API to walk objects when possible")),
		OPT_BOOL(0, "shallow", &shallow,
			 N_("create packs suitable for shallow fetches")),
		OPT_BOOL(0, "honor-pack-keep", &ignore_packed_keep_on_disk,
			 N_("ignore packs that have companion .keep file")),
		OPT_STRING_LIST(0, "keep-pack", &keep_pack_list, N_("name"),
				N_("ignore this pack")),
		OPT_INTEGER(0, "compression", &cfg->pack_compression_level,
			    N_("pack compression level")),
		OPT_BOOL(0, "keep-true-parents", &grafts_keep_true_parents,
			 N_("do not hide commits by grafts")),
		OPT_BOOL(0, "use-bitmap-index", &use_bitmap_index,
			 N_("use a bitmap index if available to speed up counting objects")),
		OPT_SET_INT(0, "write-bitmap-index", &write_bitmap_index,
			    N_("write a bitmap index together with the pack index"),
			    WRITE_BITMAP_TRUE),
		OPT_SET_INT_F(0, "write-bitmap-index-quiet",
			      &write_bitmap_index,
			      N_("write a bitmap index if possible"),
			      WRITE_BITMAP_QUIET, PARSE_OPT_HIDDEN),
		OPT_PARSE_LIST_OBJECTS_FILTER(&filter_options),
		OPT_CALLBACK_F(0, "missing", repo, N_("action"),
		  N_("handling for missing objects"), PARSE_OPT_NONEG,
		  option_parse_missing_action),
		OPT_BOOL(0, "exclude-promisor-objects", &exclude_promisor_objects,
			 N_("do not pack objects in promisor packfiles")),
		OPT_BOOL(0, "exclude-promisor-objects-best-effort",
			 &exclude_promisor_objects_best_effort,
			 N_("implies --missing=allow-any")),
		OPT_BOOL(0, "delta-islands", &use_delta_islands,
			 N_("respect islands during delta compression")),
		OPT_STRING_LIST(0, "uri-protocol", &uri_protocols,
				N_("protocol"),
				N_("exclude any configured uploadpack.blobpackfileuri with this protocol")),
		OPT_INTEGER(0, "name-hash-version", &name_hash_version,
			 N_("use the specified name-hash function to group similar objects")),
		OPT_END(),
	};

	यदि (DFS_अंक_STATES > (1 << OE_DFS_STATE_BITS))
		BUG("too many dfs states, increase OE_DFS_STATE_BITS");

	disable_replace_refs();

	sparse = git_env_bool("GIT_TEST_PACK_SPARSE", -1);
	यदि (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		यदि (sparse < 0)
			sparse = the_repository->settings.pack_use_sparse;
		यदि (the_repository->settings.pack_use_multi_pack_reuse)
			allow_pack_reuse = MULTI_PACK_REUSE;
	}

	reset_pack_idx_option(&pack_idx_opts);
	pack_idx_opts.flags |= WRITE_REV;
	repo_config(the_repository, git_pack_config, रिक्त);
	यदि (git_env_bool(GIT_TEST_NO_WRITE_REV_INDEX, 0))
		pack_idx_opts.flags &= ~WRITE_REV;

	progress = isatty(2);
	argc = parse_options(argc, argv, prefix, pack_objects_options,
			     pack_usage, 0);

	यदि (argc) {
		base_name = argv[0];
		argc--;
	}
	यदि (pack_to_stdout != !base_name || argc)
		usage_with_options(pack_usage, pack_objects_options);

	यदि (path_walk < 0) {
		यदि (use_bitmap_index > 0 ||
		    !use_internal_rev_list)
			path_walk = 0;
		अन्यथा यदि (the_repository->gitdir &&
			 the_repository->settings.pack_use_path_walk)
			path_walk = 1;
		अन्यथा
			path_walk = git_env_bool("GIT_TEST_PACK_PATH_WALK", 0);
	}

	यदि (depth < 0)
		depth = 0;
	यदि (depth >= (1 << OE_DEPTH_BITS)) {
		warning(_("delta chain depth %d is too deep, forcing %d"),
			depth, (1 << OE_DEPTH_BITS) - 1);
		depth = (1 << OE_DEPTH_BITS) - 1;
	}
	यदि (cache_max_small_delta_size >= (1U << OE_Z_DELTA_BITS)) {
		warning(_("pack.deltaCacheLimit is too high, forcing %d"),
			(1U << OE_Z_DELTA_BITS) - 1);
		cache_max_small_delta_size = (1U << OE_Z_DELTA_BITS) - 1;
	}
	यदि (window < 0)
		window = 0;

	strvec_ढकला(&rp, "pack-objects");

	यदि (path_walk) {
		ध्रुव वर्ण *option = रिक्त;
		यदि (!path_walk_filter_compatible(&filter_options))
			option = "--filter";

		यदि (option) {
			warning(_("cannot use %s with %s"),
				option, "--path-walk");
			path_walk = 0;
		}
	}
	यदि (path_walk) {
		strvec_ढकला(&rp, "--objects");
	} अन्यथा यदि (thin) {
		use_internal_rev_list = 1;
		strvec_ढकला(&rp, shallow
				? "--objects-edge-aggressive"
				: "--objects-edge");
	} अन्यथा
		strvec_ढकला(&rp, "--objects");

	यदि (rev_list_all) {
		use_internal_rev_list = 1;
		strvec_ढकला(&rp, "--all");
	}
	यदि (rev_list_reflog) {
		use_internal_rev_list = 1;
		strvec_ढकला(&rp, "--reflog");
	}
	यदि (rev_list_index) {
		use_internal_rev_list = 1;
		strvec_ढकला(&rp, "--indexed-objects");
	}
	यदि (rev_list_unpacked && !stdin_packs) {
		use_internal_rev_list = 1;
		strvec_ढकला(&rp, "--unpacked");
	}

	die_for_incompatible_opt2(exclude_promisor_objects,
				  "--exclude-promisor-objects",
				  exclude_promisor_objects_best_effort,
				  "--exclude-promisor-objects-best-effort");
	यदि (exclude_promisor_objects) {
		repo->fetch_if_missing = 0;

		/* --stdin-packs handles promisor objects separately. */
		यदि (!stdin_packs) {
			use_internal_rev_list = 1;
			strvec_ढकला(&rp, "--exclude-promisor-objects");
		}
	} अन्यथा यदि (exclude_promisor_objects_best_effort) {
		use_internal_rev_list = 1;
		arg_missing_action = MA_ALLOW_ANY;
		repo->fetch_if_missing = 0;
		fn_show_object = show_object__ma_allow_any;
		/* revs configured below */
	}
	यदि (unpack_unreachable || keep_unreachable || pack_loose_unreachable)
		use_internal_rev_list = 1;

	यदि (!reuse_object)
		reuse_delta = 0;
	यदि (cfg->pack_compression_level == -1)
		cfg->pack_compression_level = Z_DEFAULT_COMPRESSION;
	अन्यथा यदि (cfg->pack_compression_level < 0 || cfg->pack_compression_level > Z_BEST_COMPRESSION)
		die(_("bad pack compression level %d"), cfg->pack_compression_level);

	यदि (!delta_शोध_threads)	/* --threads=0 means autodetect */
		delta_शोध_threads = online_cpus();

	यदि (!HAVE_THREADS && delta_शोध_threads != 1)
		warning(_("no threads support, ignoring --threads"));
	यदि (!pack_to_stdout && !pack_size_limit)
		pack_size_limit = pack_size_limit_cfg;
	यदि (pack_to_stdout && pack_size_limit)
		die(_("--max-pack-size cannot be used to build a pack for transfer"));
	यदि (pack_size_limit && pack_size_limit < 1024*1024) {
		warning(_("minimum pack size limit is 1 MiB"));
		pack_size_limit = 1024*1024;
	}

	यदि (!pack_to_stdout && thin)
		die(_("--thin cannot be used to build an indexable pack"));

	die_for_incompatible_opt2(keep_unreachable, "--keep-unreachable",
				  unpack_unreachable, "--unpack-unreachable");
	यदि (!rev_list_all || !rev_list_reflog || !rev_list_index)
		unpack_unreachable_expiration = 0;

	die_for_incompatible_opt2(stdin_packs, "--stdin-packs",
				  filter_options.choice, "--filter");


	यदि (stdin_packs && use_internal_rev_list)
		die(_("cannot use internal rev list with --stdin-packs"));

	यदि (cruft) {
		यदि (use_internal_rev_list)
			die(_("cannot use internal rev list with --cruft"));
		die_for_incompatible_opt2(stdin_packs, "--stdin-packs",
					  cruft, "--cruft");
	}

	/*
	 * "soft" reasons not to use bitmaps - for on-disk repack by default we want
	 *
	 * - to produce good pack (with bitmap index not-yet-packed objects are
	 *   packed in suboptimal order).
	 *
	 * - to use more robust pack-generation codepath (avoiding possible
	 *   bugs in bitmap code and possible bitmap index corruption).
	 */
	यदि (!pack_to_stdout)
		use_bitmap_index_default = 0;

	यदि (use_bitmap_index < 0)
		use_bitmap_index = use_bitmap_index_default;

	/* "hard" reasons not to use bitmaps; these just won't work at all */
	यदि (!use_internal_rev_list || (!pack_to_stdout && write_bitmap_index) || is_repository_shallow(the_repository))
		use_bitmap_index = 0;

	यदि (pack_to_stdout || !rev_list_all)
		write_bitmap_index = 0;

	यदि (name_hash_version < 0)
		name_hash_version = (पूर्णाङ्क)git_env_ulong("GIT_TEST_NAME_HASH_VERSION", 1);

	validate_name_hash_version();

	यदि (use_delta_islands)
		strvec_ढकला(&rp, "--topo-order");

	यदि (progress && all_progress_implied)
		progress = 2;

	add_extra_kept_packs(&keep_pack_list);
	यदि (ignore_packed_keep_on_disk) {
		संरचना packed_git *p;

		repo_for_each_pack(the_repository, p)
			यदि (p->pack_local && p->pack_keep)
				भङ्ग;
		यदि (!p) /* no keep-able packs found */
			ignore_packed_keep_on_disk = 0;
	}
	यदि (local) {
		/*
		 * unlike ignore_packed_keep_on_disk above, we do not
		 * want to unset "local" based on looking at packs, as
		 * it also covers non-local objects
		 */
		संरचना packed_git *p;

		repo_for_each_pack(the_repository, p) {
			यदि (!p->pack_local) {
				have_non_local_packs = 1;
				भङ्ग;
			}
		}
	}

	trace2_region_enter("pack-objects", "enumerate-objects",
			    the_repository);
	prepare_packing_data(the_repository, &to_pack);

	यदि (progress && !cruft)
		progress_state = start_progress(the_repository,
						_("Enumerating objects"), 0);
	यदि (stdin_packs) {
		read_stdin_packs(repo, stdin_packs, rev_list_unpacked);
	} अन्यथा यदि (cruft) {
		read_cruft_objects();
	} अन्यथा यदि (!use_internal_rev_list) {
		read_object_list_from_stdin();
	} अन्यथा {
		संरचना rev_info revs;

		repo_init_revisions(the_repository, &revs, रिक्त);
		list_objects_filter_copy(&revs.filter, &filter_options);
		यदि (exclude_promisor_objects_best_effort) {
			revs.include_check = is_not_in_promisor_pack;
			revs.include_check_obj = is_not_in_promisor_pack_obj;
		}
		get_object_list(&revs, &rp);
		release_revisions(&revs);
	}
	cleanup_preferred_base();
	यदि (include_tag && nr_result)
		refs_for_each_tag_ref(get_main_ref_store(the_repository),
				      add_ref_tag, रिक्त);
	stop_progress(&progress_state);
	trace2_region_leave("pack-objects", "enumerate-objects",
			    the_repository);

	यदि (non_empty && !nr_result)
		गच्छ cleanup;
	यदि (nr_result) {
		trace2_region_enter("pack-objects", "prepare-pack",
				    the_repository);
		prepare_pack(window, depth);
		trace2_region_leave("pack-objects", "prepare-pack",
				    the_repository);
	}

	trace2_region_enter("pack-objects", "write-pack-file", the_repository);
	write_excluded_by_configs();
	write_pack_file();
	trace2_region_leave("pack-objects", "write-pack-file", the_repository);

	यदि (progress)
		fprintf_ln(stderr,
			   _("Total %"PRIu32" (delta %"PRIu32"),"
			     " reused %"PRIu32" (delta %"PRIu32"),"
			     " pack-reused %"PRIu32" (from %"PRIuMAX")"),
			   written, written_delta, reused, reused_delta,
			   reuse_packfile_objects,
			   (uintmax_t)reuse_packfiles_used_nr);

	trace2_data_intmax("pack-objects", the_repository, "written", written);
	trace2_data_intmax("pack-objects", the_repository, "written/delta", written_delta);
	trace2_data_intmax("pack-objects", the_repository, "reused", reused);
	trace2_data_intmax("pack-objects", the_repository, "reused/delta", reused_delta);
	trace2_data_intmax("pack-objects", the_repository, "pack-reused", reuse_packfile_objects);
	trace2_data_intmax("pack-objects", the_repository, "packs-reused", reuse_packfiles_used_nr);

cleanup:
	clear_packing_data(&to_pack);
	list_objects_filter_release(&filter_options);
	string_list_clear(&keep_pack_list, 0);
	strvec_clear(&rp);

	प्रत्यागम 0;
}
