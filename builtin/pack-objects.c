#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#ضمن "builtin.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "hex.h"
#ضمن "config.h"
#ضمن "attr.h"
#ضمن "object.h"
#ضمن "commit.h"
#ضمن "tag.h"
#ضمن "delta.h"
#ضمن "pack.h"
#ضمن "pack-revindex.h"
#ضمن "csum-file.h"
#ضمن "tree-walk.h"
#ضمن "diff.h"
#ضمن "revision.h"
#ضمن "list-objects.h"
#ضمن "list-objects-filter-options.h"
#ضمن "pack-objects.h"
#ضمن "progress.h"
#ضمن "refs.h"
#ضمن "thread-utils.h"
#ضمن "pack-bitmap.h"
#ضمن "delta-islands.h"
#ضمن "reachable.h"
#ضمن "oid-array.h"
#ضمن "strvec.h"
#ضمن "strmap.h"
#ضمن "list.h"
#ضمن "packfile.h"
#ضمن "object-file.h"
#ضمن "object-file-convert.h"
#ضمن "odb.h"
#ضمن "odb/streaming.h"
#ضمن "replace-object.h"
#ضمن "dir.h"
#ضمن "midx.h"
#ضمن "trace2.h"
#ضمن "shallow.h"
#ضمن "promisor-remote.h"
#ضمن "pack-mtimes.h"
#ضمن "parse-options.h"
#ضمن "pkt-line.h"
#ضمن "blob.h"
#ضمن "tree.h"
#ضمن "path-walk.h"

/*
 * Objects we are going to pack are collected in the `to_pack` structure.
 * It contains an array (dynamically expanded) of the object data, and a map
 * that can resolve SHA1s to their position in the array.
 */
ساكن بنية packing_data to_pack;

ساكن مضمن بنية object_entry *oe_delta(
		ثابت بنية packing_data *pack,
		ثابت بنية object_entry *e)
{
	𐡄𐡍 (!e->delta_idx)
		أرجع عدم;
	𐡄𐡍 (e->ext_base)
		أرجع &pack->ext_bases[e->delta_idx - 1];
	وإلا
		أرجع &pack->objects[e->delta_idx - 1];
}

ساكن مضمن size_t oe_delta_size(بنية packing_data *pack,
				   ثابت بنية object_entry *e)
{
	𐡄𐡍 (e->delta_size_valid)
		أرجع e->delta_size_;

	/*
	 * pack->delta_size[] can't be NULL because oe_set_delta_size()
	 * must have been called when a new delta is saved with
	 * oe_set_delta().
	 * If oe_delta() returns NULL (i.e. default state, which means
	 * delta_size_valid is also false), then the caller must never
	 * call oe_delta_size().
	 */
	أرجع pack->delta_size[e - pack->objects];
}

size_t oe_get_size_slow(بنية packing_data *pack,
			ثابت بنية object_entry *e);

ساكن مضمن size_t oe_size(بنية packing_data *pack,
			     ثابت بنية object_entry *e)
{
	𐡄𐡍 (e->size_valid)
		أرجع e->size_;

	أرجع oe_get_size_slow(pack, e);
}

ساكن مضمن فراغ oe_set_delta(بنية packing_data *pack,
				بنية object_entry *e,
				بنية object_entry *delta)
{
	𐡄𐡍 (delta)
		e->delta_idx = (delta - pack->objects) + 1;
	وإلا
		e->delta_idx = 0;
}

ساكن مضمن بنية object_entry *oe_delta_sibling(
		ثابت بنية packing_data *pack,
		ثابت بنية object_entry *e)
{
	𐡄𐡍 (e->delta_sibling_idx)
		أرجع &pack->objects[e->delta_sibling_idx - 1];
	أرجع عدم;
}

ساكن مضمن بنية object_entry *oe_delta_child(
		ثابت بنية packing_data *pack,
		ثابت بنية object_entry *e)
{
	𐡄𐡍 (e->delta_child_idx)
		أرجع &pack->objects[e->delta_child_idx - 1];
	أرجع عدم;
}

ساكن مضمن فراغ oe_set_delta_child(بنية packing_data *pack,
				      بنية object_entry *e,
				      بنية object_entry *delta)
{
	𐡄𐡍 (delta)
		e->delta_child_idx = (delta - pack->objects) + 1;
	وإلا
		e->delta_child_idx = 0;
}

ساكن مضمن فراغ oe_set_delta_sibling(بنية packing_data *pack,
					بنية object_entry *e,
					بنية object_entry *delta)
{
	𐡄𐡍 (delta)
		e->delta_sibling_idx = (delta - pack->objects) + 1;
	وإلا
		e->delta_sibling_idx = 0;
}

ساكن مضمن فراغ oe_set_size(بنية packing_data *pack,
			       بنية object_entry *e,
			       size_t size)
{
	𐡄𐡍 (size < pack->oe_size_limit) {
		e->size_ = size;
		e->size_valid = 1;
	} وإلا {
		e->size_valid = 0;
		𐡄𐡍 (oe_get_size_slow(pack, e) != size)
			BUG("'size' is supposed to be the object size!");
	}
}

ساكن مضمن فراغ oe_set_delta_size(بنية packing_data *pack,
				     بنية object_entry *e,
				     size_t size)
{
	𐡄𐡍 (size < pack->oe_delta_size_limit) {
		e->delta_size_ = size;
		e->delta_size_valid = 1;
	} وإلا {
		packing_data_lock(pack);
		𐡄𐡍 (!pack->delta_size)
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

ساكن ثابت حرف *ثابت pack_usage[] = {
	N_("git pack-objects [-q | --progress | --all-progress] [--all-progress-implied]\n"
	   "                 [--no-reuse-delta] [--delta-base-offset] [--non-empty]\n"
	   "                 [--local] [--incremental] [--window=<n>] [--depth=<n>]\n"
	   "                 [--revs [--unpacked | --all]] [--keep-pack=<pack-name>]\n"
	   "                 [--cruft] [--cruft-expiration=<time>]\n"
	   "                 [--stdout [--filter=<filter-spec>] | <base-name>]\n"
	   "                 [--shallow] [--keep-true-parents] [--[no-]sparse]\n"
	   "                 [--name-hash-version=<n>] [--path-walk] < <object-list>"),
	عدم
};

ساكن بنية pack_idx_entry **written_list;
ساكن uint32_t nr_result, nr_written, nr_seen;
ساكن بنية bitmap_index *bitmap_git;
ساكن uint32_t write_layer;

ساكن صحيح non_empty;
ساكن صحيح reuse_delta = 1, reuse_object = 1;
ساكن صحيح keep_unreachable, unpack_unreachable, include_tag;
ساكن timestamp_t unpack_unreachable_expiration;
ساكن صحيح pack_loose_unreachable;
ساكن صحيح cruft;
ساكن صحيح shallow = 0;
ساكن timestamp_t cruft_expiration;
ساكن صحيح local;
ساكن صحيح have_non_local_packs;
ساكن صحيح incremental;
ساكن صحيح ignore_packed_keep_on_disk;
ساكن صحيح ignore_packed_keep_in_core;
ساكن صحيح ignore_packed_keep_in_core_open;
ساكن صحيح ignore_packed_keep_in_core_has_cruft;
ساكن صحيح allow_ofs_delta;
ساكن بنية pack_idx_option pack_idx_opts;
ساكن ثابت حرف *base_name;
ساكن صحيح progress = 1;
ساكن صحيح window = 10;
ساكن unsigned long pack_size_limit;
ساكن صحيح depth = 50;
ساكن صحيح delta_search_threads;
ساكن صحيح pack_to_stdout;
ساكن صحيح sparse;
ساكن صحيح thin;
ساكن صحيح path_walk = -1;
ساكن صحيح num_preferred_base;
ساكن بنية progress *progress_state;

ساكن بنية bitmapped_pack *reuse_packfiles;
ساكن size_t reuse_packfiles_nr;
ساكن size_t reuse_packfiles_used_nr;
ساكن uint32_t reuse_packfile_objects;
ساكن بنية bitmap *reuse_packfile_bitmap;

ساكن صحيح use_bitmap_index_default = 1;
ساكن صحيح use_bitmap_index = -1;
ساكن تعداد {
	NO_PACK_REUSE = 0,
	SINGLE_PACK_REUSE,
	MULTI_PACK_REUSE,
} allow_pack_reuse = SINGLE_PACK_REUSE;
ساكن تعداد {
	WRITE_BITMAP_FALSE = 0,
	WRITE_BITMAP_QUIET,
	WRITE_BITMAP_TRUE,
} write_bitmap_index;
ساكن uint16_t write_bitmap_options = BITMAP_OPT_HASH_CACHE;

ساكن صحيح exclude_promisor_objects;
ساكن صحيح exclude_promisor_objects_best_effort;

ساكن صحيح use_delta_islands;

ساكن size_t delta_cache_size = 0;
ساكن size_t max_delta_cache_size = DEFAULT_DELTA_CACHE_SIZE;
ساكن unsigned long cache_max_small_delta_size = 1000;

ساكن unsigned long window_memory_limit = 0;

ساكن بنية string_list uri_protocols = STRING_LIST_INIT_NODUP;

تعداد missing_action {
	MA_ERROR = 0,      /* fail if any missing objects are encountered */
	MA_ALLOW_ANY,      /* silently allow ALL missing objects */
	MA_ALLOW_PROMISOR, /* silently allow all missing PROMISOR objects */
};
ساكن تعداد missing_action arg_missing_action;
ساكن show_object_fn fn_show_object;

بنية configured_exclusion {
	بنية oidmap_entry e;
	حرف *pack_hash_hex;
	حرف *uri;
};
ساكن بنية oidmap configured_exclusions;

ساكن بنية oidset excluded_by_config;
ساكن صحيح name_hash_version = -1;

تعداد stdin_packs_mode {
	STDIN_PACKS_MODE_NONE,
	STDIN_PACKS_MODE_STANDARD,
	STDIN_PACKS_MODE_FOLLOW,
};

/**
 * Check whether the name_hash_version chosen by user input is appropriate,
 * and also validate whether it is compatible with other features.
 */
ساكن فراغ validate_name_hash_version(فراغ)
{
	𐡄𐡍 (name_hash_version < 1 || name_hash_version > 2)
		die(_("invalid --name-hash-version option: %d"), name_hash_version);
	𐡄𐡍 (write_bitmap_index && name_hash_version != 1) {
		warning(_("currently, --write-bitmap-index requires --name-hash-version=1"));
		name_hash_version = 1;
	}
}

ساكن مضمن uint32_t pack_name_hash_fn(ثابت حرف *name)
{
	ساكن صحيح seen_version = -1;

	𐡄𐡍 (seen_version < 0)
		seen_version = name_hash_version;
	وإلا 𐡄𐡍 (seen_version != name_hash_version)
		BUG("name hash version changed from %d to %d mid-process",
		    seen_version, name_hash_version);

	بدل (name_hash_version) {
	حالة 1:
		أرجع pack_name_hash(name);

	حالة 2:
		أرجع pack_name_hash_v2((ثابت unsigned حرف *)name);

	افتراضي:
		BUG("invalid name-hash version: %d", name_hash_version);
	}
}

/*
 * stats
 */
ساكن uint32_t written, written_delta;
ساكن uint32_t reused, reused_delta;

/*
 * Indexed commits
 */
ساكن بنية commit **indexed_commits;
ساكن unsigned صحيح indexed_commits_nr;
ساكن unsigned صحيح indexed_commits_alloc;

ساكن فراغ index_commit_for_bitmap(بنية commit *commit)
{
	𐡄𐡍 (indexed_commits_nr >= indexed_commits_alloc) {
		indexed_commits_alloc = (indexed_commits_alloc + 32) * 2;
		REALLOC_ARRAY(indexed_commits, indexed_commits_alloc);
	}

	indexed_commits[indexed_commits_nr++] = commit;
}

ساكن فراغ *get_delta(بنية object_entry *entry)
{
	unsigned long size, base_size;
	size_t delta_size;
	فراغ *buf, *base_buf, *delta_buf;
	تعداد object_type type;
	size_t size_st = 0, base_size_st = 0;

	buf = odb_read_object(the_repository->objects, &entry->idx.oid,
			      &type, &size_st);
	size = cast_size_t_to_ulong(size_st);
	𐡄𐡍 (!buf)
		die(_("unable to read %s"), oid_to_hex(&entry->idx.oid));
	base_buf = odb_read_object(the_repository->objects,
				   &DELTA(entry)->idx.oid, &type,
				   &base_size_st);
	base_size = cast_size_t_to_ulong(base_size_st);
	𐡄𐡍 (!base_buf)
		die("unable to read %s",
		    oid_to_hex(&DELTA(entry)->idx.oid));
	delta_buf = diff_delta(base_buf, base_size,
			       buf, size, &delta_size, 0);
	/*
	 * We successfully computed this delta once but dropped it for
	 * memory reasons. Something is very wrong if this time we
	 * recompute and create a different delta.
	 */
	𐡄𐡍 (!delta_buf || delta_size != DELTA_SIZE(entry))
		BUG("delta size changed");
	حرر(buf);
	حرر(base_buf);
	أرجع delta_buf;
}

ساكن unsigned long do_compress(فراغ **pptr, unsigned long size)
{
	git_zstream stream;
	فراغ *in, *out;
	unsigned long maxsize;
	بنية repo_config_values *cfg = repo_config_values(the_repository);

	git_deflate_init(&stream, cfg->pack_compression_level);
	maxsize = git_deflate_bound(&stream, size);

	in = *pptr;
	out = xmalloc(maxsize);
	*pptr = out;

	stream.next_in = in;
	stream.avail_in = size;
	stream.next_out = out;
	stream.avail_out = maxsize;
	طالما (git_deflate(&stream, Z_FINISH) == Z_OK)
		; /* nothing */
	git_deflate_end(&stream);

	حرر(in);
	أرجع stream.total_out;
}

ساكن unsigned long write_large_blob_data(بنية odb_stream *st, بنية hashfile *f,
					   ثابت بنية object_id *oid)
{
	git_zstream stream;
	unsigned حرف ibuf[1024 * 16];
	unsigned حرف obuf[1024 * 16];
	unsigned long olen = 0;
	بنية repo_config_values *cfg = repo_config_values(the_repository);

	git_deflate_init(&stream, cfg->pack_compression_level);

	لكل (;;) {
		ssize_t readlen;
		صحيح zret = Z_OK;
		readlen = odb_stream_read(st, ibuf, حجم(ibuf));
		𐡄𐡍 (readlen == -1)
			die(_("unable to read %s"), oid_to_hex(oid));

		stream.next_in = ibuf;
		stream.avail_in = readlen;
		طالما ((stream.avail_in || readlen == 0) &&
		       (zret == Z_OK || zret == Z_BUF_ERROR)) {
			stream.next_out = obuf;
			stream.avail_out = حجم(obuf);
			zret = git_deflate(&stream, readlen ? 0 : Z_FINISH);
			hashwrite(f, obuf, stream.next_out - obuf);
			olen += stream.next_out - obuf;
		}
		𐡄𐡍 (stream.avail_in)
			die(_("deflate error (%d)"), zret);
		𐡄𐡍 (readlen == 0) {
			𐡄𐡍 (zret != Z_STREAM_END)
				die(_("deflate error (%d)"), zret);
			اكسر;
		}
	}
	git_deflate_end(&stream);
	أرجع olen;
}

/*
 * we are going to reuse the existing object data as is.  make
 * sure it is not corrupt.
 */
ساكن صحيح check_pack_inflate(بنية packed_git *p,
		بنية pack_window **w_curs,
		off_t offset,
		off_t len,
		size_t expect)
{
	git_zstream stream;
	unsigned حرف fakebuf[4096], *in;
	صحيح st;

	memset(&stream, 0, حجم(stream));
	git_inflate_init(&stream);
	𐡏𐡁𐡃 {
		in = use_pack(p, w_curs, offset, &stream.avail_in);
		stream.next_in = in;
		stream.next_out = fakebuf;
		stream.avail_out = حجم(fakebuf);
		st = git_inflate(&stream, Z_FINISH);
		offset += stream.next_in - in;
	} طالما (st == Z_OK || st == Z_BUF_ERROR);
	git_inflate_end(&stream);
	أرجع (st == Z_STREAM_END &&
		stream.total_out == expect &&
		stream.total_in == len) ? 0 : -1;
}

ساكن فراغ copy_pack_data(بنية hashfile *f,
		بنية packed_git *p,
		بنية pack_window **w_curs,
		off_t offset,
		off_t len)
{
	unsigned حرف *in;
	size_t avail;

	طالما (len) {
		in = use_pack(p, w_curs, offset, &avail);
		𐡄𐡍 (avail > len)
			avail = (unsigned long)len;
		hashwrite(f, in, avail);
		offset += avail;
		len -= avail;
	}
}

ساكن مضمن صحيح oe_size_greater_than(بنية packing_data *pack,
				       ثابت بنية object_entry *lhs,
				       size_t rhs)
{
	𐡄𐡍 (lhs->size_valid)
		أرجع lhs->size_ > rhs;
	𐡄𐡍 (rhs < pack->oe_size_limit) /* rhs < 2^x <= lhs ? */
		أرجع 1;
	أرجع oe_get_size_slow(pack, lhs) > rhs;
}

/* Return 0 if we will bust the pack-size limit */
ساكن unsigned long write_no_reuse_object(بنية hashfile *f, بنية object_entry *entry,
					   unsigned long limit, صحيح usable_delta)
{
	unsigned long size, datalen;
	unsigned حرف header[MAX_PACK_OBJECT_HEADER],
		      dheader[MAX_PACK_OBJECT_HEADER];
	unsigned hdrlen;
	تعداد object_type type;
	فراغ *buf;
	بنية odb_stream *st = عدم;
	ثابت unsigned hashsz = the_hash_algo->rawsz;

	𐡄𐡍 (!usable_delta) {
		𐡄𐡍 (oe_type(entry) == OBJ_BLOB &&
		    oe_size_greater_than(&to_pack, entry,
					 repo_settings_get_big_file_threshold(the_repository)) &&
		    (st = odb_stream_from_object(the_repository->objects, &entry->idx.oid,
						 عدم)) != عدم) {
			buf = عدم;
			type = st->type;
			size = st->size;
		} وإلا {
			size_t size_st = 0;
			buf = odb_read_object(the_repository->objects,
					      &entry->idx.oid, &type,
					      &size_st);
			size = cast_size_t_to_ulong(size_st);
			𐡄𐡍 (!buf)
				die(_("unable to read %s"),
				    oid_to_hex(&entry->idx.oid));
		}
		/*
		 * make sure no cached delta data remains from a
		 * previous attempt before a pack split occurred.
		 */
		FREE_AND_NULL(entry->delta_data);
		entry->z_delta_size = 0;
	} وإلا 𐡄𐡍 (entry->delta_data) {
		size = DELTA_SIZE(entry);
		buf = entry->delta_data;
		entry->delta_data = عدم;
		type = (allow_ofs_delta && DELTA(entry)->idx.offset) ?
			OBJ_OFS_DELTA : OBJ_REF_DELTA;
	} وإلا {
		buf = get_delta(entry);
		size = DELTA_SIZE(entry);
		type = (allow_ofs_delta && DELTA(entry)->idx.offset) ?
			OBJ_OFS_DELTA : OBJ_REF_DELTA;
	}

	𐡄𐡍 (st)	/* large blob case, just assume we don't compress well */
		datalen = size;
	وإلا 𐡄𐡍 (entry->z_delta_size)
		datalen = entry->z_delta_size;
	وإلا
		datalen = do_compress(&buf, size);

	/*
	 * The object header is a byte of 'type' followed by zero or
	 * more bytes of length.
	 */
	hdrlen = encode_in_pack_object_header(header, حجم(header),
					      type, size);

	𐡄𐡍 (type == OBJ_OFS_DELTA) {
		/*
		 * Deltas with relative base contain an additional
		 * encoding of the relative offset for the delta
		 * base from this object's position in the pack.
		 */
		off_t ofs = entry->idx.offset - DELTA(entry)->idx.offset;
		unsigned pos = حجم(dheader) - 1;
		dheader[pos] = ofs & 127;
		طالما (ofs >>= 7)
			dheader[--pos] = 128 | (--ofs & 127);
		𐡄𐡍 (limit && hdrlen + حجم(dheader) - pos + datalen + hashsz >= limit) {
			𐡄𐡍 (st)
				odb_stream_close(st);
			حرر(buf);
			أرجع 0;
		}
		hashwrite(f, header, hdrlen);
		hashwrite(f, dheader + pos, حجم(dheader) - pos);
		hdrlen += حجم(dheader) - pos;
	} وإلا 𐡄𐡍 (type == OBJ_REF_DELTA) {
		/*
		 * Deltas with a base reference contain
		 * additional bytes for the base object ID.
		 */
		𐡄𐡍 (limit && hdrlen + hashsz + datalen + hashsz >= limit) {
			𐡄𐡍 (st)
				odb_stream_close(st);
			حرر(buf);
			أرجع 0;
		}
		hashwrite(f, header, hdrlen);
		hashwrite(f, DELTA(entry)->idx.oid.hash, hashsz);
		hdrlen += hashsz;
	} وإلا {
		𐡄𐡍 (limit && hdrlen + datalen + hashsz >= limit) {
			𐡄𐡍 (st)
				odb_stream_close(st);
			حرر(buf);
			أرجع 0;
		}
		hashwrite(f, header, hdrlen);
	}
	𐡄𐡍 (st) {
		datalen = write_large_blob_data(st, f, &entry->idx.oid);
		odb_stream_close(st);
	} وإلا {
		hashwrite(f, buf, datalen);
		حرر(buf);
	}

	أرجع hdrlen + datalen;
}

/* Return 0 if we will bust the pack-size limit */
ساكن off_t write_reuse_object(بنية hashfile *f, بنية object_entry *entry,
				unsigned long limit, صحيح usable_delta)
{
	بنية packed_git *p = IN_PACK(entry);
	بنية pack_window *w_curs = عدم;
	uint32_t pos;
	off_t offset, cur;
	تعداد object_type type = oe_type(entry);
	تعداد object_type in_pack_type;
	off_t datalen;
	unsigned حرف header[MAX_PACK_OBJECT_HEADER],
		      dheader[MAX_PACK_OBJECT_HEADER];
	unsigned hdrlen;
	ثابت unsigned hashsz = the_hash_algo->rawsz;
	size_t entry_size;

	cur = entry->in_pack_offset;
	in_pack_type = unpack_object_header(p, &w_curs, &cur, &entry_size);
	𐡄𐡍 (in_pack_type < 0)
		die(_("write_reuse_object: unable to parse object header of %s"),
		    oid_to_hex(&entry->idx.oid));

	𐡄𐡍 (DELTA(entry))
		type = (allow_ofs_delta && DELTA(entry)->idx.offset) ?
			OBJ_OFS_DELTA : OBJ_REF_DELTA;
	hdrlen = encode_in_pack_object_header(header, حجم(header),
					      type, entry_size);

	offset = entry->in_pack_offset;
	𐡄𐡍 (offset_to_pack_pos(p, offset, &pos) < 0)
		die(_("write_reuse_object: could not locate %s, expected at "
		      "offset %"PRIuMAX" in pack %s"),
		    oid_to_hex(&entry->idx.oid), (uintmax_t)offset,
		    p->pack_name);
	datalen = pack_pos_to_offset(p, pos + 1) - offset;
	𐡄𐡍 (!pack_to_stdout && p->index_version > 1 &&
	    check_pack_crc(p, &w_curs, offset, datalen,
			   pack_pos_to_index(p, pos))) {
		error(_("bad packed object CRC for %s"),
		      oid_to_hex(&entry->idx.oid));
		unuse_pack(&w_curs);
		أرجع write_no_reuse_object(f, entry, limit, usable_delta);
	}

	offset += entry->in_pack_header_size;
	datalen -= entry->in_pack_header_size;

	𐡄𐡍 (!pack_to_stdout && p->index_version == 1 &&
	    check_pack_inflate(p, &w_curs, offset, datalen, entry_size)) {
		error(_("corrupt packed object for %s"),
		      oid_to_hex(&entry->idx.oid));
		unuse_pack(&w_curs);
		أرجع write_no_reuse_object(f, entry, limit, usable_delta);
	}

	𐡄𐡍 (type == OBJ_OFS_DELTA) {
		off_t ofs = entry->idx.offset - DELTA(entry)->idx.offset;
		unsigned pos = حجم(dheader) - 1;
		dheader[pos] = ofs & 127;
		طالما (ofs >>= 7)
			dheader[--pos] = 128 | (--ofs & 127);
		𐡄𐡍 (limit && hdrlen + حجم(dheader) - pos + datalen + hashsz >= limit) {
			unuse_pack(&w_curs);
			أرجع 0;
		}
		hashwrite(f, header, hdrlen);
		hashwrite(f, dheader + pos, حجم(dheader) - pos);
		hdrlen += حجم(dheader) - pos;
		reused_delta++;
	} وإلا 𐡄𐡍 (type == OBJ_REF_DELTA) {
		𐡄𐡍 (limit && hdrlen + hashsz + datalen + hashsz >= limit) {
			unuse_pack(&w_curs);
			أرجع 0;
		}
		hashwrite(f, header, hdrlen);
		hashwrite(f, DELTA(entry)->idx.oid.hash, hashsz);
		hdrlen += hashsz;
		reused_delta++;
	} وإلا {
		𐡄𐡍 (limit && hdrlen + datalen + hashsz >= limit) {
			unuse_pack(&w_curs);
			أرجع 0;
		}
		hashwrite(f, header, hdrlen);
	}
	copy_pack_data(f, p, &w_curs, offset, datalen);
	unuse_pack(&w_curs);
	reused++;
	أرجع hdrlen + datalen;
}

/* Return 0 if we will bust the pack-size limit */
ساكن off_t write_object(بنية hashfile *f,
			  بنية object_entry *entry,
			  off_t write_offset)
{
	unsigned long limit;
	off_t len;
	صحيح usable_delta, to_reuse;

	𐡄𐡍 (!pack_to_stdout)
		crc32_begin(f);

	/* apply size limit if limited packsize and not first object */
	𐡄𐡍 (!pack_size_limit || !nr_written)
		limit = 0;
	وإلا 𐡄𐡍 (pack_size_limit <= write_offset)
		/*
		 * the earlier object did not fit the limit; avoid
		 * mistaking this with unlimited (i.e. limit = 0).
		 */
		limit = 1;
	وإلا
		limit = pack_size_limit - write_offset;

	𐡄𐡍 (!DELTA(entry))
		usable_delta = 0;	/* no delta */
	وإلا 𐡄𐡍 (!pack_size_limit)
	       usable_delta = 1;	/* unlimited packfile */
	وإلا 𐡄𐡍 (DELTA(entry)->idx.offset == (off_t)-1)
		usable_delta = 0;	/* base was written to another pack */
	وإلا 𐡄𐡍 (DELTA(entry)->idx.offset)
		usable_delta = 1;	/* base already exists in this pack */
	وإلا
		usable_delta = 0;	/* base could end up in another pack */

	𐡄𐡍 (!reuse_object)
		to_reuse = 0;	/* explicit */
	وإلا 𐡄𐡍 (!IN_PACK(entry))
		to_reuse = 0;	/* can't reuse what we don't have */
	وإلا 𐡄𐡍 (oe_type(entry) == OBJ_REF_DELTA ||
		 oe_type(entry) == OBJ_OFS_DELTA)
				/* check_object() decided it for us ... */
		to_reuse = usable_delta;
				/* ... but pack split may override that */
	وإلا 𐡄𐡍 (oe_type(entry) != entry->in_pack_type)
		to_reuse = 0;	/* pack has delta which is unusable */
	وإلا 𐡄𐡍 (DELTA(entry))
		to_reuse = 0;	/* we want to pack afresh */
	وإلا
		to_reuse = 1;	/* we have it in-pack undeltified,
				 * and we do not need to deltify it.
				 */

	𐡄𐡍 (!to_reuse)
		len = write_no_reuse_object(f, entry, limit, usable_delta);
	وإلا
		len = write_reuse_object(f, entry, limit, usable_delta);
	𐡄𐡍 (!len)
		أرجع 0;

	𐡄𐡍 (usable_delta)
		written_delta++;
	written++;
	𐡄𐡍 (!pack_to_stdout)
		entry->idx.crc32 = crc32_end(f);
	أرجع len;
}

تعداد write_one_status {
	WRITE_ONE_SKIP = -1, /* already written */
	WRITE_ONE_BREAK = 0, /* writing this will bust the limit; not written */
	WRITE_ONE_WRITTEN = 1, /* normal */
	WRITE_ONE_RECURSIVE = 2 /* already scheduled to be written */
};

ساكن تعداد write_one_status write_one(بنية hashfile *f,
				       بنية object_entry *e,
				       off_t *offset)
{
	off_t size;
	صحيح recursing;

	/*
	 * we set offset to 1 (which is an impossible value) to mark
	 * the fact that this object is involved in "write its base
	 * first before writing a deltified object" recursion.
	 */
	recursing = (e->idx.offset == 1);
	𐡄𐡍 (recursing) {
		warning(_("recursive delta detected for object %s"),
			oid_to_hex(&e->idx.oid));
		أرجع WRITE_ONE_RECURSIVE;
	} وإلا 𐡄𐡍 (e->idx.offset || e->preferred_base) {
		/* offset is non zero if object is written already. */
		أرجع WRITE_ONE_SKIP;
	}

	/* if we are deltified, write out base object first. */
	𐡄𐡍 (DELTA(e)) {
		e->idx.offset = 1; /* now recurse */
		بدل (write_one(f, DELTA(e), offset)) {
		حالة WRITE_ONE_RECURSIVE:
			/* we cannot depend on this one */
			SET_DELTA(e, عدم);
			اكسر;
		افتراضي:
			اكسر;
		حالة WRITE_ONE_BREAK:
			e->idx.offset = recursing;
			أرجع WRITE_ONE_BREAK;
		}
	}

	e->idx.offset = *offset;
	size = write_object(f, e, *offset);
	𐡄𐡍 (!size) {
		e->idx.offset = recursing;
		أرجع WRITE_ONE_BREAK;
	}
	written_list[nr_written++] = &e->idx;

	/* make sure off_t is sufficiently large not to wrap */
	𐡄𐡍 (signed_add_overflows(*offset, size))
		die(_("pack too large for current definition of off_t"));
	*offset += size;
	أرجع WRITE_ONE_WRITTEN;
}

ساكن صحيح mark_tagged(ثابت بنية reference *ref, فراغ *cb_data UNUSED)
{
	بنية object_id peeled;
	بنية object_entry *entry = packlist_find(&to_pack, ref->oid);

	𐡄𐡍 (entry)
		entry->tagged = 1;
	𐡄𐡍 (!reference_get_peeled_oid(the_repository, ref, &peeled)) {
		entry = packlist_find(&to_pack, &peeled);
		𐡄𐡍 (entry)
			entry->tagged = 1;
	}
	أرجع 0;
}

ساكن مضمن unsigned حرف oe_layer(بنية packing_data *pack,
				     بنية object_entry *e)
{
	𐡄𐡍 (!pack->layer)
		أرجع 0;
	أرجع pack->layer[e - pack->objects];
}

ساكن مضمن فراغ add_to_write_order(بنية object_entry **wo,
			       unsigned صحيح *endp,
			       بنية object_entry *e)
{
	𐡄𐡍 (e->filled || oe_layer(&to_pack, e) != write_layer)
		أرجع;
	wo[(*endp)++] = e;
	e->filled = 1;
}

ساكن فراغ add_descendants_to_write_order(بنية object_entry **wo,
					   unsigned صحيح *endp,
					   بنية object_entry *e)
{
	صحيح add_to_order = 1;
	طالما (e) {
		𐡄𐡍 (add_to_order) {
			بنية object_entry *s;
			/* add this node... */
			add_to_write_order(wo, endp, e);
			/* all its siblings... */
			لكل (s = DELTA_SIBLING(e); s; s = DELTA_SIBLING(s)) {
				add_to_write_order(wo, endp, s);
			}
		}
		/* drop down a level to add left subtree nodes if possible */
		𐡄𐡍 (DELTA_CHILD(e)) {
			add_to_order = 1;
			e = DELTA_CHILD(e);
		} وإلا {
			add_to_order = 0;
			/* our sibling might have some children, it is next */
			𐡄𐡍 (DELTA_SIBLING(e)) {
				e = DELTA_SIBLING(e);
				استمر;
			}
			/* go back to our parent node */
			e = DELTA(e);
			طالما (e && !DELTA_SIBLING(e)) {
				/* we're on the right side of a subtree, keep
				 * going up until we can go right again */
				e = DELTA(e);
			}
			𐡄𐡍 (!e) {
				/* done- we hit our original root node */
				أرجع;
			}
			/* pass it off to sibling at this level */
			e = DELTA_SIBLING(e);
		}
	};
}

ساكن فراغ add_family_to_write_order(بنية object_entry **wo,
				      unsigned صحيح *endp,
				      بنية object_entry *e)
{
	بنية object_entry *root;

	لكل (root = e; DELTA(root); root = DELTA(root))
		; /* nothing */
	add_descendants_to_write_order(wo, endp, root);
}

ساكن فراغ compute_layer_order(بنية object_entry **wo, unsigned صحيح *wo_end)
{
	unsigned صحيح i, last_untagged;
	بنية object_entry *objects = to_pack.objects;

	لكل (i = 0; i < to_pack.nr_objects; i++) {
		𐡄𐡍 (objects[i].tagged)
			اكسر;
		add_to_write_order(wo, wo_end, &objects[i]);
	}
	last_untagged = i;

	/*
	 * Then fill all the tagged tips.
	 */
	لكل (; i < to_pack.nr_objects; i++) {
		𐡄𐡍 (objects[i].tagged)
			add_to_write_order(wo, wo_end, &objects[i]);
	}

	/*
	 * And then all remaining commits and tags.
	 */
	لكل (i = last_untagged; i < to_pack.nr_objects; i++) {
		𐡄𐡍 (oe_type(&objects[i]) != OBJ_COMMIT &&
		    oe_type(&objects[i]) != OBJ_TAG)
			استمر;
		add_to_write_order(wo, wo_end, &objects[i]);
	}

	/*
	 * And then all the trees.
	 */
	لكل (i = last_untagged; i < to_pack.nr_objects; i++) {
		𐡄𐡍 (oe_type(&objects[i]) != OBJ_TREE)
			استمر;
		add_to_write_order(wo, wo_end, &objects[i]);
	}

	/*
	 * Finally all the rest in really tight order
	 */
	لكل (i = last_untagged; i < to_pack.nr_objects; i++) {
		𐡄𐡍 (!objects[i].filled && oe_layer(&to_pack, &objects[i]) == write_layer)
			add_family_to_write_order(wo, wo_end, &objects[i]);
	}
}

ساكن بنية object_entry **compute_write_order(فراغ)
{
	uint32_t max_layers = 1;
	unsigned صحيح i, wo_end;

	بنية object_entry **wo;
	بنية object_entry *objects = to_pack.objects;

	لكل (i = 0; i < to_pack.nr_objects; i++) {
		objects[i].tagged = 0;
		objects[i].filled = 0;
		SET_DELTA_CHILD(&objects[i], عدم);
		SET_DELTA_SIBLING(&objects[i], عدم);
	}

	/*
	 * Fully connect delta_child/delta_sibling network.
	 * Make sure delta_sibling is sorted in the original
	 * recency order.
	 */
	لكل (i = to_pack.nr_objects; i > 0;) {
		بنية object_entry *e = &objects[--i];
		𐡄𐡍 (!DELTA(e))
			استمر;
		/* Mark me as the first child */
		e->delta_sibling_idx = DELTA(e)->delta_child_idx;
		SET_DELTA_CHILD(DELTA(e), e);
	}

	/*
	 * Mark objects that are at the tip of tags.
	 */
	refs_for_each_tag_ref(get_main_ref_store(the_repository), mark_tagged,
			      عدم);

	𐡄𐡍 (use_delta_islands) {
		max_layers = compute_pack_layers(&to_pack);
		free_island_marks();
	}

	ALLOC_ARRAY(wo, to_pack.nr_objects);
	wo_end = 0;

	لكل (; write_layer < max_layers; ++write_layer)
		compute_layer_order(wo, &wo_end);

	𐡄𐡍 (wo_end != to_pack.nr_objects)
		die(_("ordered %u objects, expected %"PRIu32),
		    wo_end, to_pack.nr_objects);

	أرجع wo;
}


/*
 * A reused set of objects. All objects in a chunk have the same
 * relative position in the original packfile and the generated
 * packfile.
 */

ساكن بنية reused_chunk {
	/* The offset of the first object of this chunk in the original
	 * packfile. */
	off_t original;
	/* The difference for "original" minus the offset of the first object of
	 * this chunk in the generated packfile. */
	off_t difference;
} *reused_chunks;
ساكن صحيح reused_chunks_nr;
ساكن صحيح reused_chunks_alloc;

ساكن فراغ record_reused_object(off_t where, off_t offset)
{
	𐡄𐡍 (reused_chunks_nr && reused_chunks[reused_chunks_nr-1].difference == offset)
		أرجع;

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
ساكن off_t find_reused_offset(off_t where)
{
	صحيح lo = 0, hi = reused_chunks_nr;
	طالما (lo < hi) {
		صحيح mi = lo + ((hi - lo) / 2);
		𐡄𐡍 (where == reused_chunks[mi].original)
			أرجع reused_chunks[mi].difference;
		𐡄𐡍 (where < reused_chunks[mi].original)
			hi = mi;
		وإلا
			lo = mi + 1;
	}

	/*
	 * The first chunk starts at zero, so we can't have gone below
	 * there.
	 */
	أكد(lo);
	أرجع reused_chunks[lo-1].difference;
}

ساكن فراغ write_reused_pack_one(بنية packed_git *reuse_packfile,
				  size_t pos, بنية hashfile *out,
				  off_t pack_start,
				  بنية pack_window **w_curs)
{
	off_t offset, next, cur;
	تعداد object_type type;
	size_t size;

	offset = pack_pos_to_offset(reuse_packfile, pos);
	next = pack_pos_to_offset(reuse_packfile, pos + 1);

	record_reused_object(offset,
			     offset - (hashfile_total(out) - pack_start));

	cur = offset;
	type = unpack_object_header(reuse_packfile, w_curs, &cur, &size);
	أكد(type >= 0);

	𐡄𐡍 (type == OBJ_OFS_DELTA) {
		off_t base_offset;
		off_t fixup;

		unsigned حرف header[MAX_PACK_OBJECT_HEADER];
		unsigned len;

		base_offset = get_delta_base(reuse_packfile, w_curs, &cur, type, offset);
		أكد(base_offset != 0);

		/* Convert to REF_DELTA if we must... */
		𐡄𐡍 (!allow_ofs_delta) {
			uint32_t base_pos;
			بنية object_id base_oid;

			𐡄𐡍 (offset_to_pack_pos(reuse_packfile, base_offset, &base_pos) < 0)
				die(_("expected object at offset %"PRIuMAX" "
				      "in pack %s"),
				    (uintmax_t)base_offset,
				    reuse_packfile->pack_name);

			nth_packed_object_id(&base_oid, reuse_packfile,
					     pack_pos_to_index(reuse_packfile, base_pos));

			len = encode_in_pack_object_header(header, حجم(header),
							   OBJ_REF_DELTA, size);
			hashwrite(out, header, len);
			hashwrite(out, base_oid.hash, the_hash_algo->rawsz);
			copy_pack_data(out, reuse_packfile, w_curs, cur, next - cur);
			أرجع;
		}

		/* Otherwise see if we need to rewrite the offset... */
		fixup = find_reused_offset(offset) -
			find_reused_offset(base_offset);
		𐡄𐡍 (fixup) {
			unsigned حرف ofs_header[MAX_PACK_OBJECT_HEADER];
			unsigned i, ofs_len;
			off_t ofs = offset - base_offset - fixup;

			len = encode_in_pack_object_header(header, حجم(header),
							   OBJ_OFS_DELTA, size);

			i = حجم(ofs_header) - 1;
			ofs_header[i] = ofs & 127;
			طالما (ofs >>= 7)
				ofs_header[--i] = 128 | (--ofs & 127);

			ofs_len = حجم(ofs_header) - i;

			hashwrite(out, header, len);
			hashwrite(out, ofs_header + حجم(ofs_header) - ofs_len, ofs_len);
			copy_pack_data(out, reuse_packfile, w_curs, cur, next - cur);
			أرجع;
		}

		/* ...otherwise we have no fixup, and can write it verbatim */
	}

	copy_pack_data(out, reuse_packfile, w_curs, offset, next - offset);
}

ساكن size_t write_reused_pack_verbatim(بنية bitmapped_pack *reuse_packfile,
					 بنية hashfile *out,
					 بنية pack_window **w_curs)
{
	size_t pos = 0;
	size_t end;

	𐡄𐡍 (reuse_packfile->bitmap_pos) {
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
		أرجع reuse_packfile->bitmap_pos / BITS_IN_EWORD;
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
	𐡄𐡍 (reuse_packfile_bitmap->word_alloc < end)
		BUG("fewer words than expected in reuse_packfile_bitmap");

	طالما (pos < end && reuse_packfile_bitmap->words[pos] == (eword_t)~0)
		pos++;

	𐡄𐡍 (pos) {
		off_t to_write;

		written = (pos * BITS_IN_EWORD);
		to_write = pack_pos_to_offset(reuse_packfile->p, written)
			- حجم(بنية pack_header);

		/* We're recording one chunk, not one object. */
		record_reused_object(حجم(بنية pack_header), 0);
		hashflush(out);
		copy_pack_data(out, reuse_packfile->p, w_curs,
			حجم(بنية pack_header), to_write);

		display_progress(progress_state, written);
	}
	أرجع pos;
}

ساكن فراغ write_reused_pack(بنية bitmapped_pack *reuse_packfile,
			      بنية hashfile *f)
{
	size_t i = reuse_packfile->bitmap_pos / BITS_IN_EWORD;
	uint32_t offset;
	off_t pack_start = hashfile_total(f) - حجم(بنية pack_header);
	بنية pack_window *w_curs = عدم;

	𐡄𐡍 (allow_ofs_delta)
		i = write_reused_pack_verbatim(reuse_packfile, f, &w_curs);

	لكل (; i < reuse_packfile_bitmap->word_alloc; ++i) {
		eword_t word = reuse_packfile_bitmap->words[i];
		size_t pos = (i * BITS_IN_EWORD);

		لكل (offset = 0; offset < BITS_IN_EWORD; ++offset) {
			uint32_t pack_pos;
			𐡄𐡍 ((word >> offset) == 0)
				اكسر;

			offset += ewah_bit_ctz64(word >> offset);
			𐡄𐡍 (pos + offset < reuse_packfile->bitmap_pos)
				استمر;
			𐡄𐡍 (pos + offset >= reuse_packfile->bitmap_pos + reuse_packfile->bitmap_nr)
				اذهب done;

			𐡄𐡍 (reuse_packfile->bitmap_pos) {
				/*
				 * When doing multi-pack reuse on a
				 * non-preferred pack, translate bit positions
				 * from the MIDX pseudo-pack order back to their
				 * pack-relative positions before attempting
				 * reuse.
				 */
				بنية multi_pack_index *m = reuse_packfile->from_midx;
				uint32_t midx_pos;
				off_t pack_ofs;

				𐡄𐡍 (!m)
					BUG("non-zero bitmap position without MIDX");

				midx_pos = pack_pos_to_midx(m, pos + offset);
				pack_ofs = nth_midxed_offset(m, midx_pos);

				𐡄𐡍 (offset_to_pack_pos(reuse_packfile->p,
						       pack_ofs, &pack_pos) < 0)
					BUG("could not find expected object at offset %"PRIuMAX" in pack %s",
					    (uintmax_t)pack_ofs,
					    pack_basename(reuse_packfile->p));
			} وإلا {
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

ساكن فراغ write_excluded_by_configs(فراغ)
{
	بنية oidset_iter iter;
	ثابت بنية object_id *oid;

	oidset_iter_init(&excluded_by_config, &iter);
	طالما ((oid = oidset_iter_next(&iter))) {
		بنية configured_exclusion *ex =
			oidmap_get(&configured_exclusions, oid);

		𐡄𐡍 (!ex)
			BUG("configured exclusion wasn't configured");
		write_in_full(1, ex->pack_hash_hex, strlen(ex->pack_hash_hex));
		write_in_full(1, " ", 1);
		write_in_full(1, ex->uri, strlen(ex->uri));
		write_in_full(1, "\n", 1);
	}
}

ساكن ثابت حرف no_split_warning[] = N_(
"disabling bitmap writing, packs are split due to pack.packSizeLimit"
);

ساكن فراغ write_pack_file(فراغ)
{
	uint32_t i = 0, j;
	بنية hashfile *f;
	off_t offset;
	uint32_t nr_remaining = nr_result;
	time_t last_mtime = 0;
	بنية object_entry **write_order;
	off_t bytes_written = 0;

	𐡄𐡍 (progress > pack_to_stdout)
		progress_state = start_progress(the_repository,
						_("Writing objects"), nr_result);
	ALLOC_ARRAY(written_list, to_pack.nr_objects);
	write_order = compute_write_order();

	𐡏𐡁𐡃 {
		unsigned حرف hash[GIT_MAX_RAWSZ];
		حرف *pack_tmp_name = عدم;

		𐡄𐡍 (pack_to_stdout) {
			/*
			 * This command is most often invoked via
			 * git-upload-pack(1), which will typically chunk data
			 * into pktlines. As such, we use the maximum data
			 * length of them as buffer length.
			 *
			 * Note that we need to subtract one though to
			 * accommodate for the sideband byte.
			 */
			بنية hashfd_options opts = {
				.progress = progress_state,
				.buffer_len = LARGE_PACKET_DATA_MAX - 1,
			};
			f = hashfd_ext(the_repository->hash_algo, 1,
				       "<stdout>", &opts);
		} وإلا {
			f = create_tmp_packfile(the_repository, &pack_tmp_name);
		}

		offset = write_pack_header(f, nr_remaining);

		𐡄𐡍 (reuse_packfiles_nr) {
			أكد(pack_to_stdout);
			لكل (j = 0; j < reuse_packfiles_nr; j++) {
				reused_chunks_nr = 0;
				write_reused_pack(&reuse_packfiles[j], f);
				𐡄𐡍 (reused_chunks_nr)
					reuse_packfiles_used_nr++;
			}
			offset = hashfile_total(f);
		}

		nr_written = 0;
		لكل (; i < to_pack.nr_objects; i++) {
			بنية object_entry *e = write_order[i];
			𐡄𐡍 (write_one(f, e, &offset) == WRITE_ONE_BREAK)
				اكسر;
			display_progress(progress_state, written);
		}

		bytes_written += hashfile_total(f) +
			the_repository->hash_algo->rawsz;
		𐡄𐡍 (pack_to_stdout) {
			/*
			 * We never fsync when writing to stdout since we may
			 * not be writing to an actual pack file. For instance,
			 * the upload-pack code passes a pipe here. Calling
			 * fsync on a pipe results in unnecessary
			 * synchronization with the reader on some platforms.
			 */
			finalize_hashfile(f, hash, FSYNC_COMPONENT_NONE,
					  CSUM_HASH_IN_STREAM | CSUM_CLOSE);
		} وإلا 𐡄𐡍 (nr_written == nr_remaining) {
			finalize_hashfile(f, hash, FSYNC_COMPONENT_PACK,
					  CSUM_HASH_IN_STREAM | CSUM_FSYNC | CSUM_CLOSE);
		} وإلا {
			/*
			 * If we wrote the wrong number of entries in the
			 * header, rewrite it like in fast-import.
			 */

			صحيح fd = finalize_hashfile(f, hash, FSYNC_COMPONENT_PACK, 0);
			fixup_pack_header_footer(the_hash_algo, fd, hash,
						 pack_tmp_name, nr_written,
						 hash, offset);
			close(fd);
			𐡄𐡍 (write_bitmap_index) {
				𐡄𐡍 (write_bitmap_index != WRITE_BITMAP_QUIET)
					warning(_(no_split_warning));
				write_bitmap_index = 0;
			}
		}

		𐡄𐡍 (!pack_to_stdout) {
			بنية stat st;
			بنية strbuf tmpname = STRBUF_INIT;
			بنية bitmap_writer bitmap_writer;
			حرف *idx_tmp_name = عدم;

			/*
			 * Packs are runtime accessed in their mtime
			 * order since newer packs are more likely to contain
			 * younger objects.  So if we are creating multiple
			 * packs then we should modify the mtime of later ones
			 * to preserve this property.
			 */
			𐡄𐡍 (stat(pack_tmp_name, &st) < 0) {
				warning_errno(_("failed to stat %s"), pack_tmp_name);
			} وإلا 𐡄𐡍 (!last_mtime) {
				last_mtime = st.st_mtime;
			} وإلا {
				بنية utimbuf utb;
				utb.actime = st.st_atime;
				utb.modtime = --last_mtime;
				𐡄𐡍 (utime(pack_tmp_name, &utb) < 0)
					warning_errno(_("failed utime() on %s"), pack_tmp_name);
			}

			strbuf_addf(&tmpname, "%s-%s.", base_name,
				    hash_to_hex(hash));

			𐡄𐡍 (write_bitmap_index) {
				bitmap_writer_init(&bitmap_writer,
						   the_repository, &to_pack,
						   عدم);
				bitmap_writer_set_checksum(&bitmap_writer, hash);
				bitmap_writer_build_type_index(&bitmap_writer,
							       written_list);
			}

			𐡄𐡍 (cruft)
				pack_idx_opts.flags |= WRITE_MTIMES;

			stage_tmp_packfiles(the_repository, &tmpname,
					    pack_tmp_name, written_list,
					    nr_written, &to_pack,
					    &pack_idx_opts, hash,
					    &idx_tmp_name);

			𐡄𐡍 (write_bitmap_index) {
				size_t tmpname_len = tmpname.len;

				strbuf_addstr(&tmpname, "bitmap");
				stop_progress(&progress_state);

				bitmap_writer_show_progress(&bitmap_writer,
							    progress);
				bitmap_writer_select_commits(&bitmap_writer,
							     indexed_commits,
							     indexed_commits_nr);
				𐡄𐡍 (bitmap_writer_build(&bitmap_writer) < 0)
					die(_("failed to write bitmap index"));
				bitmap_writer_finish(&bitmap_writer,
						     written_list,
						     tmpname.buf, write_bitmap_options);
				bitmap_writer_free(&bitmap_writer);
				write_bitmap_index = 0;
				strbuf_setlen(&tmpname, tmpname_len);
			}

			rename_tmp_packfile_idx(the_repository, &tmpname, &idx_tmp_name);

			حرر(idx_tmp_name);
			strbuf_release(&tmpname);
			حرر(pack_tmp_name);
			puts(hash_to_hex(hash));
		}

		/* mark written objects as written to previous pack */
		لكل (j = 0; j < nr_written; j++) {
			written_list[j]->offset = (off_t)-1;
		}
		nr_remaining -= nr_written;
	} طالما (nr_remaining && i < to_pack.nr_objects);

	حرر(written_list);
	حرر(write_order);
	stop_progress(&progress_state);
	𐡄𐡍 (written != nr_result)
		die(_("wrote %"PRIu32" objects while expecting %"PRIu32),
		    written, nr_result);
	trace2_data_intmax("pack-objects", the_repository,
			   "write_pack_file/wrote", nr_result);
	trace2_data_intmax("pack-objects", the_repository,
			   "write_pack_file/wrote_bytes", bytes_written);
}

ساكن صحيح no_try_delta(ثابت حرف *path)
{
	ساكن بنية attr_check *check;

	𐡄𐡍 (!check)
		check = attr_check_initl("delta", عدم);
	git_check_attr(the_repository->index, path, check);
	𐡄𐡍 (ATTR_FALSE(check->items[0].value))
		أرجع 1;
	أرجع 0;
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
ساكن صحيح have_duplicate_entry(ثابت بنية object_id *oid,
				صحيح exclude)
{
	بنية object_entry *entry;

	𐡄𐡍 (reuse_packfile_bitmap &&
	    bitmap_walk_contains(bitmap_git, reuse_packfile_bitmap, oid))
		أرجع 1;

	entry = packlist_find(&to_pack, oid);
	𐡄𐡍 (!entry)
		أرجع 0;

	𐡄𐡍 (exclude) {
		𐡄𐡍 (!entry->preferred_base)
			nr_result--;
		entry->preferred_base = 1;
	}

	أرجع 1;
}

ساكن صحيح want_cruft_object_mtime(بنية repository *r,
				   ثابت بنية object_id *oid,
				   unsigned flags, uint32_t mtime)
{
	بنية odb_source_files *files = odb_source_files_downcast(r->objects->source);

	لكل (بنية odb_files_dir *dir = files->dirs; dir; dir = dir->next) {
		بنية packed_git **cache = packfile_store_get_kept_pack_cache(dir->packed, flags);

		لكل (; *cache; cache++) {
			بنية packed_git *p = *cache;
			off_t ofs;
			uint32_t candidate_mtime;

			ofs = find_pack_entry_one(oid, p);
			𐡄𐡍 (!ofs)
				استمر;

			/*
			 * We have a copy of the object 'oid' in a non-cruft
			 * pack. We can avoid packing an additional copy
			 * regardless of what the existing copy's mtime is since
			 * it is outside of a cruft pack.
			 */
			𐡄𐡍 (!p->is_cruft)
				أرجع 0;

			/*
			 * If we have a copy of the object 'oid' in a cruft
			 * pack, then either read the cruft pack's mtime for
			 * that object, or, if that can't be loaded, assume the
			 * pack's mtime itself.
			 */
			𐡄𐡍 (!load_pack_mtimes(p)) {
				uint32_t pos;
				𐡄𐡍 (offset_to_pack_pos(p, ofs, &pos) < 0)
					استمر;
				candidate_mtime = nth_packed_mtime(p, pos);
			} وإلا {
				candidate_mtime = p->mtime;
			}

			/*
			 * We have a surviving copy of the object in a cruft
			 * pack whose mtime is greater than or equal to the one
			 * we are considering. We can thus avoid packing an
			 * additional copy of that object.
			 */
			𐡄𐡍 (mtime <= candidate_mtime)
				أرجع 0;
		}
	}

	أرجع -1;
}

ساكن صحيح want_found_object(ثابت بنية object_id *oid, صحيح exclude,
			     بنية packed_git *p, uint32_t mtime)
{
	𐡄𐡍 (exclude)
		أرجع 1;
	𐡄𐡍 (incremental)
		أرجع 0;

	𐡄𐡍 (!is_pack_valid(p))
		أرجع -1;

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
	𐡄𐡍 (local && !p->pack_local)
		أرجع 0;

	/*
	 * Then handle .keep first, as we have a fast(er) path there.
	 */
	𐡄𐡍 (ignore_packed_keep_on_disk || ignore_packed_keep_in_core ||
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
		𐡄𐡍 (ignore_packed_keep_on_disk)
			flags |= KEPT_PACK_ON_DISK;
		𐡄𐡍 (ignore_packed_keep_in_core)
			flags |= KEPT_PACK_IN_CORE;
		𐡄𐡍 (ignore_packed_keep_in_core_open)
			flags |= KEPT_PACK_IN_CORE_OPEN;

		/*
		 * If the object is in a pack that we want to ignore, *and* we
		 * don't have any cruft packs that are being retained, we can
		 * abort quickly.
		 */
		𐡄𐡍 (!ignore_packed_keep_in_core_has_cruft) {
			𐡄𐡍 (ignore_packed_keep_on_disk && p->pack_keep)
				أرجع 0;
			𐡄𐡍 (ignore_packed_keep_in_core && p->pack_keep_in_core)
				أرجع 0;
			𐡄𐡍 (ignore_packed_keep_in_core_open && p->pack_keep_in_core_open)
				أرجع 0;
			𐡄𐡍 (has_object_kept_pack(p->repo, oid, flags))
				أرجع 0;
		} وإلا {
			/*
			 * But if there is at least one cruft pack which
			 * is being kept, we only want to include the
			 * provided object if it has a strictly greater
			 * mtime than any existing cruft copy.
			 */
			𐡄𐡍 (!want_cruft_object_mtime(p->repo, oid, flags,
						     mtime))
				أرجع 0;
		}
	}

	/*
	 * At this point we know definitively that either we don't care about
	 * keep-packs, or the object is not in one. Keep checking other
	 * conditions...
	 */
	𐡄𐡍 (!local || !have_non_local_packs)
		أرجع 1;

	/* we don't know yet; keep looking for more packs */
	أرجع -1;
}

ساكن صحيح want_object_in_pack_one(بنية packed_git *p,
				   ثابت بنية object_id *oid,
				   صحيح exclude,
				   بنية packed_git **found_pack,
				   off_t *found_offset,
				   uint32_t found_mtime)
{
	off_t offset;

	𐡄𐡍 (p == *found_pack)
		offset = *found_offset;
	وإلا
		offset = find_pack_entry_one(oid, p);

	𐡄𐡍 (offset) {
		𐡄𐡍 (!*found_pack) {
			𐡄𐡍 (!is_pack_valid(p))
				أرجع -1;
			*found_offset = offset;
			*found_pack = p;
		}
		أرجع want_found_object(oid, exclude, p, found_mtime);
	}
	أرجع -1;
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
ساكن صحيح want_object_in_pack_mtime(ثابت بنية object_id *oid,
				     صحيح exclude,
				     بنية packed_git **found_pack,
				     off_t *found_offset,
				     uint32_t found_mtime)
{
	بنية odb_source_files *files =
		odb_source_files_downcast(the_repository->objects->source);
	صحيح want;
	بنية packfile_list_entry *e;

	𐡄𐡍 (!exclude && local) {
		/*
		 * Note that we start iterating at `dirs->next` so that we
		 * skip the local object directory.
		 */
		لكل (بنية odb_files_dir *dir = files->dirs->next; dir; dir = dir->next)
			𐡄𐡍 (!odb_source_read_object_info(&dir->loose->base, oid, عدم, 0, عدم))
				أرجع 0;
	}

	/*
	 * If we already know the pack object lives in, start checks from that
	 * pack - in the usual case when neither --local was given nor .keep files
	 * are present we will determine the answer right now.
	 */
	𐡄𐡍 (*found_pack) {
		want = want_found_object(oid, exclude, *found_pack,
					 found_mtime);
		𐡄𐡍 (want != -1)
			أرجع want;

		*found_pack = عدم;
		*found_offset = 0;
	}

	لكل (بنية odb_files_dir *dir = files->dirs; dir; dir = dir->next) {
		بنية multi_pack_index *m = get_multi_pack_index(dir->packed);
		بنية pack_entry e;

		𐡄𐡍 (m && midx_fill_entry(m, oid, &e, عدم) == MIDX_FILL_HIT) {
			want = want_object_in_pack_one(e.p, oid, exclude, found_pack, found_offset, found_mtime);
			𐡄𐡍 (want != -1)
				أرجع want;
		}
	}

	لكل (بنية odb_files_dir *dir = files->dirs; dir; dir = dir->next) {
		لكل (e = dir->packed->packs.head; e; e = e->next) {
			بنية packed_git *p = e->pack;
			want = want_object_in_pack_one(p, oid, exclude, found_pack, found_offset, found_mtime);
			𐡄𐡍 (!exclude && want > 0)
				packfile_list_prepend(&dir->packed->packs, p);
			𐡄𐡍 (want != -1)
				أرجع want;
		}
	}

	𐡄𐡍 (uri_protocols.nr) {
		بنية configured_exclusion *ex =
			oidmap_get(&configured_exclusions, oid);
		صحيح i;
		ثابت حرف *p;

		𐡄𐡍 (ex) {
			لكل (i = 0; i < uri_protocols.nr; i++) {
				𐡄𐡍 (skip_prefix(ex->uri,
						uri_protocols.items[i].string,
						&p) &&
				    *p == ':') {
					oidset_insert(&excluded_by_config, oid);
					أرجع 0;
				}
			}
		}
	}

	أرجع 1;
}

ساكن مضمن صحيح want_object_in_pack(ثابت بنية object_id *oid,
				      صحيح exclude,
				      بنية packed_git **found_pack,
				      off_t *found_offset)
{
	أرجع want_object_in_pack_mtime(oid, exclude, found_pack, found_offset,
					 0);
}

ساكن بنية object_entry *create_object_entry(ثابت بنية object_id *oid,
						تعداد object_type type,
						uint32_t hash,
						صحيح exclude,
						صحيح no_try_delta,
						بنية packed_git *found_pack,
						off_t found_offset)
{
	بنية object_entry *entry;

	entry = packlist_alloc(&to_pack, oid);
	entry->hash = hash;
	oe_set_type(entry, type);
	𐡄𐡍 (exclude)
		entry->preferred_base = 1;
	وإلا
		nr_result++;
	𐡄𐡍 (found_pack) {
		oe_set_in_pack(&to_pack, entry, found_pack);
		entry->in_pack_offset = found_offset;
	}

	entry->no_try_delta = no_try_delta;

	أرجع entry;
}

ساكن ثابت حرف no_closure_warning[] = N_(
"disabling bitmap writing, as some objects are not being packed"
);

ساكن فراغ add_object_entry(ثابت بنية object_id *oid, تعداد object_type type,
			     ثابت حرف *name, صحيح exclude)
{
	بنية packed_git *found_pack = عدم;
	off_t found_offset = 0;

	display_progress(progress_state, ++nr_seen);

	𐡄𐡍 (have_duplicate_entry(oid, exclude))
		أرجع;

	𐡄𐡍 (!want_object_in_pack(oid, exclude, &found_pack, &found_offset)) {
		/* The pack is missing an object, so it will not have closure */
		𐡄𐡍 (write_bitmap_index) {
			𐡄𐡍 (write_bitmap_index != WRITE_BITMAP_QUIET)
				warning(_(no_closure_warning));
			write_bitmap_index = 0;
		}
		أرجع;
	}

	create_object_entry(oid, type, pack_name_hash_fn(name),
			    exclude, name && no_try_delta(name),
			    found_pack, found_offset);
}

ساكن صحيح add_object_entry_from_bitmap(ثابت بنية object_id *oid,
					تعداد object_type type,
					صحيح flags UNUSED, uint32_t name_hash,
					بنية packed_git *pack, off_t offset,
					فراغ *payload UNUSED)
{
	display_progress(progress_state, ++nr_seen);

	𐡄𐡍 (have_duplicate_entry(oid, 0))
		أرجع 0;

	𐡄𐡍 (!want_object_in_pack(oid, 0, &pack, &offset))
		أرجع 0;

	create_object_entry(oid, type, name_hash, 0, 0, pack, offset);
	أرجع 0;
}

بنية pbase_tree_cache {
	بنية object_id oid;
	صحيح ref;
	صحيح temporary;
	فراغ *tree_data;
	unsigned long tree_size;
};

ساكن بنية pbase_tree_cache *(pbase_tree_cache[256]);
ساكن صحيح pbase_tree_cache_ix(ثابت بنية object_id *oid)
{
	أرجع oid->hash[0] % ARRAY_SIZE(pbase_tree_cache);
}
ساكن صحيح pbase_tree_cache_ix_incr(صحيح ix)
{
	أرجع (ix+1) % ARRAY_SIZE(pbase_tree_cache);
}

ساكن بنية pbase_tree {
	بنية pbase_tree *next;
	/* This is a phony "cache" entry; we are not
	 * going to evict it or find it through _get()
	 * mechanism -- this is for the toplevel node that
	 * would almost always change with any commit.
	 */
	بنية pbase_tree_cache pcache;
} *pbase_tree;

ساكن بنية pbase_tree_cache *pbase_tree_get(ثابت بنية object_id *oid)
{
	بنية pbase_tree_cache *ent, *nent;
	فراغ *data;
	unsigned long size;
	size_t size_st = 0;
	تعداد object_type type;
	صحيح neigh;
	صحيح my_ix = pbase_tree_cache_ix(oid);
	صحيح available_ix = -1;

	/* pbase-tree-cache acts as a limited hashtable.
	 * your object will be found at your index or within a few
	 * slots after that slot if it is cached.
	 */
	لكل (neigh = 0; neigh < 8; neigh++) {
		ent = pbase_tree_cache[my_ix];
		𐡄𐡍 (ent && oideq(&ent->oid, oid)) {
			ent->ref++;
			أرجع ent;
		}
		وإلا 𐡄𐡍 (((available_ix < 0) && (!ent || !ent->ref)) ||
			 ((0 <= available_ix) &&
			  (!ent && pbase_tree_cache[available_ix])))
			available_ix = my_ix;
		𐡄𐡍 (!ent)
			اكسر;
		my_ix = pbase_tree_cache_ix_incr(my_ix);
	}

	/* Did not find one.  Either we got a bogus request or
	 * we need to read and perhaps cache.
	 */
	data = odb_read_object(the_repository->objects, oid, &type, &size_st);
	size = cast_size_t_to_ulong(size_st);
	𐡄𐡍 (!data)
		أرجع عدم;
	𐡄𐡍 (type != OBJ_TREE) {
		حرر(data);
		أرجع عدم;
	}

	/* We need to either cache or return a throwaway copy */

	𐡄𐡍 (available_ix < 0)
		ent = عدم;
	وإلا {
		ent = pbase_tree_cache[available_ix];
		my_ix = available_ix;
	}

	𐡄𐡍 (!ent) {
		nent = xmalloc(حجم(*nent));
		nent->temporary = (available_ix < 0);
	}
	وإلا {
		/* evict and reuse */
		حرر(ent->tree_data);
		nent = ent;
	}
	oidcpy(&nent->oid, oid);
	nent->tree_data = data;
	nent->tree_size = size;
	nent->ref = 1;
	𐡄𐡍 (!nent->temporary)
		pbase_tree_cache[my_ix] = nent;
	أرجع nent;
}

ساكن فراغ pbase_tree_put(بنية pbase_tree_cache *cache)
{
	𐡄𐡍 (!cache->temporary) {
		cache->ref--;
		أرجع;
	}
	حرر(cache->tree_data);
	حرر(cache);
}

ساكن size_t name_cmp_len(ثابت حرف *name)
{
	أرجع strcspn(name, "\n/");
}

ساكن فراغ add_pbase_object(بنية tree_desc *tree,
			     ثابت حرف *name,
			     size_t cmplen,
			     ثابت حرف *fullname)
{
	بنية name_entry entry;
	صحيح cmp;

	طالما (tree_entry(tree,&entry)) {
		𐡄𐡍 (S_ISGITLINK(entry.mode))
			استمر;
		cmp = tree_entry_len(&entry) != cmplen ? 1 :
		      memcmp(name, entry.path, cmplen);
		𐡄𐡍 (cmp > 0)
			استمر;
		𐡄𐡍 (cmp < 0)
			أرجع;
		𐡄𐡍 (name[cmplen] != '/') {
			add_object_entry(&entry.oid,
					 object_type(entry.mode),
					 fullname, 1);
			أرجع;
		}
		𐡄𐡍 (S_ISDIR(entry.mode)) {
			بنية tree_desc sub;
			بنية pbase_tree_cache *tree;
			ثابت حرف *down = name+cmplen+1;
			size_t downlen = name_cmp_len(down);

			tree = pbase_tree_get(&entry.oid);
			𐡄𐡍 (!tree)
				أرجع;
			init_tree_desc(&sub, &tree->oid,
				       tree->tree_data, tree->tree_size);

			add_pbase_object(&sub, down, downlen, fullname);
			pbase_tree_put(tree);
		}
	}
}

ساكن unsigned *done_pbase_paths;
ساكن صحيح done_pbase_paths_num;
ساكن صحيح done_pbase_paths_alloc;
ساكن صحيح done_pbase_path_pos(unsigned hash)
{
	صحيح lo = 0;
	صحيح hi = done_pbase_paths_num;
	طالما (lo < hi) {
		صحيح mi = lo + (hi - lo) / 2;
		𐡄𐡍 (done_pbase_paths[mi] == hash)
			أرجع mi;
		𐡄𐡍 (done_pbase_paths[mi] < hash)
			hi = mi;
		وإلا
			lo = mi + 1;
	}
	أرجع -lo-1;
}

ساكن صحيح check_pbase_path(unsigned hash)
{
	صحيح pos = done_pbase_path_pos(hash);
	𐡄𐡍 (0 <= pos)
		أرجع 1;
	pos = -pos - 1;
	ALLOC_GROW(done_pbase_paths,
		   done_pbase_paths_num + 1,
		   done_pbase_paths_alloc);
	done_pbase_paths_num++;
	𐡄𐡍 (pos < done_pbase_paths_num)
		MOVE_ARRAY(done_pbase_paths + pos + 1, done_pbase_paths + pos,
			   done_pbase_paths_num - pos - 1);
	done_pbase_paths[pos] = hash;
	أرجع 0;
}

ساكن فراغ add_preferred_base_object(ثابت حرف *name)
{
	بنية pbase_tree *it;
	size_t cmplen;
	unsigned hash = pack_name_hash_fn(name);

	𐡄𐡍 (!num_preferred_base || check_pbase_path(hash))
		أرجع;

	cmplen = name_cmp_len(name);
	لكل (it = pbase_tree; it; it = it->next) {
		𐡄𐡍 (cmplen == 0) {
			add_object_entry(&it->pcache.oid, OBJ_TREE, عدم, 1);
		}
		وإلا {
			بنية tree_desc tree;
			init_tree_desc(&tree, &it->pcache.oid,
				       it->pcache.tree_data, it->pcache.tree_size);
			add_pbase_object(&tree, name, cmplen, name);
		}
	}
}

ساكن فراغ add_preferred_base(بنية object_id *oid)
{
	بنية pbase_tree *it;
	فراغ *data;
	unsigned long size;
	size_t size_st = 0;
	بنية object_id tree_oid;

	𐡄𐡍 (window <= num_preferred_base++)
		أرجع;

	data = odb_read_object_peeled(the_repository->objects, oid,
				      OBJ_TREE, &size_st, &tree_oid);
	size = cast_size_t_to_ulong(size_st);
	𐡄𐡍 (!data)
		أرجع;

	لكل (it = pbase_tree; it; it = it->next) {
		𐡄𐡍 (oideq(&it->pcache.oid, &tree_oid)) {
			حرر(data);
			أرجع;
		}
	}

	CALLOC_ARRAY(it, 1);
	it->next = pbase_tree;
	pbase_tree = it;

	oidcpy(&it->pcache.oid, &tree_oid);
	it->pcache.tree_data = data;
	it->pcache.tree_size = size;
}

ساكن فراغ cleanup_preferred_base(فراغ)
{
	بنية pbase_tree *it;
	unsigned i;

	it = pbase_tree;
	pbase_tree = عدم;
	طالما (it) {
		بنية pbase_tree *tmp = it;
		it = tmp->next;
		حرر(tmp->pcache.tree_data);
		حرر(tmp);
	}

	لكل (i = 0; i < ARRAY_SIZE(pbase_tree_cache); i++) {
		𐡄𐡍 (!pbase_tree_cache[i])
			استمر;
		حرر(pbase_tree_cache[i]->tree_data);
		FREE_AND_NULL(pbase_tree_cache[i]);
	}

	FREE_AND_NULL(done_pbase_paths);
	done_pbase_paths_num = done_pbase_paths_alloc = 0;
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
ساكن صحيح can_reuse_delta(ثابت بنية object_id *base_oid,
			   بنية object_entry *delta,
			   بنية object_entry **base_out)
{
	بنية object_entry *base;

	/*
	 * First see if we're already sending the base (or it's explicitly in
	 * our "excluded" list).
	 */
	base = packlist_find(&to_pack, base_oid);
	𐡄𐡍 (base) {
		𐡄𐡍 (!in_same_island(&delta->idx.oid, &base->idx.oid))
			أرجع 0;
		*base_out = base;
		أرجع 1;
	}

	/*
	 * Otherwise, reachability bitmaps may tell us if the receiver has it,
	 * even if it was buried too deep in history to make it into the
	 * packing list.
	 */
	𐡄𐡍 (thin && bitmap_has_oid_in_uninteresting(bitmap_git, base_oid)) {
		𐡄𐡍 (use_delta_islands) {
			𐡄𐡍 (!in_same_island(&delta->idx.oid, base_oid))
				أرجع 0;
		}
		*base_out = عدم;
		أرجع 1;
	}

	أرجع 0;
}

ساكن فراغ prefetch_to_pack(uint32_t object_index_start) {
	بنية oid_array to_fetch = OID_ARRAY_INIT;
	uint32_t i;

	لكل (i = object_index_start; i < to_pack.nr_objects; i++) {
		بنية object_entry *entry = to_pack.objects + i;

		𐡄𐡍 (!odb_read_object_info_extended(the_repository->objects,
						   &entry->idx.oid,
						   عدم,
						   OBJECT_INFO_FOR_PREFETCH))
			استمر;
		oid_array_append(&to_fetch, &entry->idx.oid);
	}
	promisor_remote_get_direct(the_repository,
				   to_fetch.oid, to_fetch.nr);
	oid_array_clear(&to_fetch);
}

ساكن فراغ check_object(بنية object_entry *entry, uint32_t object_index)
{
	size_t canonical_size;
	تعداد object_type type;
	بنية object_info oi = {.typep = &type, .sizep = &canonical_size};

	𐡄𐡍 (IN_PACK(entry)) {
		بنية packed_git *p = IN_PACK(entry);
		بنية pack_window *w_curs = عدم;
		صحيح have_base = 0;
		بنية object_id base_ref;
		بنية object_entry *base_entry;
		size_t used, used_0, avail;
		off_t ofs;
		unsigned حرف *buf, c;
		تعداد object_type type;
		size_t in_pack_size;

		buf = use_pack(p, &w_curs, entry->in_pack_offset, &avail);

		/*
		 * We want in_pack_type even if we do not reuse delta
		 * since non-delta representations could still be reused.
		 */
		used = unpack_object_header_buffer(buf, avail,
						   &type,
						   &in_pack_size);
		𐡄𐡍 (used == 0)
			اذهب give_up;

		𐡄𐡍 (type < 0)
			BUG("invalid type %d", type);
		entry->in_pack_type = type;

		/*
		 * Determine if this is a delta and if so whether we can
		 * reuse it or not.  Otherwise let's find out as cheaply as
		 * possible what the actual type and size for this object is.
		 */
		بدل (entry->in_pack_type) {
		افتراضي:
			/* Not a delta hence we've already got all we need. */
			oe_set_type(entry, entry->in_pack_type);
			SET_SIZE(entry, in_pack_size);
			entry->in_pack_header_size = used;
			𐡄𐡍 (oe_type(entry) < OBJ_COMMIT || oe_type(entry) > OBJ_BLOB)
				اذهب give_up;
			unuse_pack(&w_curs);
			أرجع;
		حالة OBJ_REF_DELTA:
			𐡄𐡍 (reuse_delta && !entry->preferred_base) {
				oidread(&base_ref,
					use_pack(p, &w_curs,
						 entry->in_pack_offset + used,
						 عدم),
					the_repository->hash_algo);
				have_base = 1;
			}
			entry->in_pack_header_size = used + the_hash_algo->rawsz;
			اكسر;
		حالة OBJ_OFS_DELTA:
			buf = use_pack(p, &w_curs,
				       entry->in_pack_offset + used, عدم);
			used_0 = 0;
			c = buf[used_0++];
			ofs = c & 127;
			طالما (c & 128) {
				ofs += 1;
				𐡄𐡍 (!ofs || MSB(ofs, 7)) {
					error(_("delta base offset overflow in pack for %s"),
					      oid_to_hex(&entry->idx.oid));
					اذهب give_up;
				}
				c = buf[used_0++];
				ofs = (ofs << 7) + (c & 127);
			}
			ofs = entry->in_pack_offset - ofs;
			𐡄𐡍 (ofs <= 0 || ofs >= entry->in_pack_offset) {
				error(_("delta base offset out of bound for %s"),
				      oid_to_hex(&entry->idx.oid));
				اذهب give_up;
			}
			𐡄𐡍 (reuse_delta && !entry->preferred_base) {
				uint32_t pos;
				𐡄𐡍 (offset_to_pack_pos(p, ofs, &pos) < 0)
					اذهب give_up;
				𐡄𐡍 (!nth_packed_object_id(&base_ref, p,
							  pack_pos_to_index(p, pos)))
					have_base = 1;
			}
			entry->in_pack_header_size = used + used_0;
			اكسر;
		}

		𐡄𐡍 (have_base &&
		    can_reuse_delta(&base_ref, entry, &base_entry)) {
			oe_set_type(entry, entry->in_pack_type);
			SET_SIZE(entry, in_pack_size); /* delta size */
			SET_DELTA_SIZE(entry, in_pack_size);

			𐡄𐡍 (base_entry) {
				SET_DELTA(entry, base_entry);
				entry->delta_sibling_idx = base_entry->delta_child_idx;
				SET_DELTA_CHILD(base_entry, entry);
			} وإلا {
				SET_DELTA_EXT(entry, &base_ref);
			}

			unuse_pack(&w_curs);
			أرجع;
		}

		𐡄𐡍 (oe_type(entry)) {
			off_t delta_pos;

			/*
			 * This must be a delta and we already know what the
			 * final object type is.  Let's extract the actual
			 * object size from the delta header.
			 */
			delta_pos = entry->in_pack_offset + entry->in_pack_header_size;
			canonical_size = get_size_from_delta(p, &w_curs,
							     delta_pos);
			𐡄𐡍 (canonical_size == 0)
				اذهب give_up;
			SET_SIZE(entry, canonical_size);
			unuse_pack(&w_curs);
			أرجع;
		}

		/*
		 * No choice but to fall back to the recursive delta walk
		 * with odb_read_object_info() to find about the object type
		 * at this point...
		 */
		give_up:
		unuse_pack(&w_curs);
	}

	𐡄𐡍 (odb_read_object_info_extended(the_repository->objects, &entry->idx.oid, &oi,
					  OBJECT_INFO_SKIP_FETCH_OBJECT | OBJECT_INFO_LOOKUP_REPLACE) < 0) {
		𐡄𐡍 (repo_has_promisor_remote(the_repository)) {
			prefetch_to_pack(object_index);
			𐡄𐡍 (odb_read_object_info_extended(the_repository->objects, &entry->idx.oid, &oi,
							  OBJECT_INFO_SKIP_FETCH_OBJECT | OBJECT_INFO_LOOKUP_REPLACE) < 0)
				type = -1;
		} وإلا {
			type = -1;
		}
	}
	oe_set_type(entry, type);
	𐡄𐡍 (entry->type_valid) {
		SET_SIZE(entry, canonical_size);
	} وإلا {
		/*
		 * Bad object type is checked in prepare_pack().  This is
		 * to permit a missing preferred base object to be ignored
		 * as a preferred base.  Doing so can result in a larger
		 * pack file, but the transfer will still take place.
		 */
	}
}

ساكن صحيح pack_offset_sort(ثابت فراغ *_a, ثابت فراغ *_b)
{
	ثابت بنية object_entry *a = *(بنية object_entry **)_a;
	ثابت بنية object_entry *b = *(بنية object_entry **)_b;
	ثابت بنية packed_git *a_in_pack = IN_PACK(a);
	ثابت بنية packed_git *b_in_pack = IN_PACK(b);

	/* avoid filesystem trashing with loose objects */
	𐡄𐡍 (!a_in_pack && !b_in_pack)
		أرجع oidcmp(&a->idx.oid, &b->idx.oid);

	𐡄𐡍 (a_in_pack < b_in_pack)
		أرجع -1;
	𐡄𐡍 (a_in_pack > b_in_pack)
		أرجع 1;
	أرجع a->in_pack_offset < b->in_pack_offset ? -1 :
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
ساكن فراغ drop_reused_delta(بنية object_entry *entry)
{
	unsigned *idx = &to_pack.objects[entry->delta_idx - 1].delta_child_idx;
	بنية object_info oi = OBJECT_INFO_INIT;
	تعداد object_type type;
	size_t size;

	طالما (*idx) {
		بنية object_entry *oe = &to_pack.objects[*idx - 1];

		𐡄𐡍 (oe == entry)
			*idx = oe->delta_sibling_idx;
		وإلا
			idx = &oe->delta_sibling_idx;
	}
	SET_DELTA(entry, عدم);
	entry->depth = 0;

	oi.sizep = &size;
	oi.typep = &type;
	𐡄𐡍 (packed_object_info(عدم, IN_PACK(entry), entry->in_pack_offset, &oi) < 0) {
		/*
		 * We failed to get the info from this pack for some reason;
		 * fall back to odb_read_object_info, which may find another copy.
		 * And if that fails, the error will be recorded in oe_type(entry)
		 * and dealt with in prepare_pack().
		 */
		oe_set_type(entry,
			    odb_read_object_info(the_repository->objects,
						 &entry->idx.oid, &size));
	} وإلا {
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
ساكن فراغ break_delta_chains(بنية object_entry *entry)
{
	/*
	 * The actual depth of each object we will write is stored as an int,
	 * as it cannot exceed our int "depth" limit. But before we break
	 * changes based no that limit, we may potentially go as deep as the
	 * number of objects, which is elsewhere bounded to a uint32_t.
	 */
	uint32_t total_depth;
	بنية object_entry *cur, *next;

	لكل (cur = entry, total_depth = 0;
	     cur;
	     cur = DELTA(cur), total_depth++) {
		𐡄𐡍 (cur->dfs_state == DFS_DONE) {
			/*
			 * We've already seen this object and know it isn't
			 * part of a cycle. We do need to append its depth
			 * to our count.
			 */
			total_depth += cur->depth;
			اكسر;
		}

		/*
		 * We break cycles before looping, so an ACTIVE state (or any
		 * other cruft which made its way into the state variable)
		 * is a bug.
		 */
		𐡄𐡍 (cur->dfs_state != DFS_NONE)
			BUG("confusing delta dfs state in first pass: %d",
			    cur->dfs_state);

		/*
		 * Now we know this is the first time we've seen the object. If
		 * it's not a delta, we're done traversing, but we'll mark it
		 * done to save time on future traversals.
		 */
		𐡄𐡍 (!DELTA(cur)) {
			cur->dfs_state = DFS_DONE;
			اكسر;
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
		𐡄𐡍 (DELTA(cur)->dfs_state == DFS_ACTIVE) {
			drop_reused_delta(cur);
			cur->dfs_state = DFS_DONE;
			اكسر;
		}
	}

	/*
	 * And now that we've gone all the way to the bottom of the chain, we
	 * need to clear the active flags and set the depth fields as
	 * appropriate. Unlike the loop above, which can quit when it drops a
	 * delta, we need to keep going to look for more depth cuts. So we need
	 * an extra "next" pointer to keep going after we reset cur->delta.
	 */
	لكل (cur = entry; cur; cur = next) {
		next = DELTA(cur);

		/*
		 * We should have a chain of zero or more ACTIVE states down to
		 * a final DONE. We can quit after the DONE, because either it
		 * has no bases, or we've already handled them in a previous
		 * call.
		 */
		𐡄𐡍 (cur->dfs_state == DFS_DONE)
			اكسر;
		وإلا 𐡄𐡍 (cur->dfs_state != DFS_ACTIVE)
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
		𐡄𐡍 (!cur->depth)
			drop_reused_delta(cur);

		cur->dfs_state = DFS_DONE;
	}
}

ساكن فراغ get_object_details(فراغ)
{
	uint32_t i;
	بنية object_entry **sorted_by_offset;

	𐡄𐡍 (progress)
		progress_state = start_progress(the_repository,
						_("Counting objects"),
						to_pack.nr_objects);

	CALLOC_ARRAY(sorted_by_offset, to_pack.nr_objects);
	لكل (i = 0; i < to_pack.nr_objects; i++)
		sorted_by_offset[i] = to_pack.objects + i;
	QSORT(sorted_by_offset, to_pack.nr_objects, pack_offset_sort);

	لكل (i = 0; i < to_pack.nr_objects; i++) {
		بنية object_entry *entry = sorted_by_offset[i];
		check_object(entry, i);
		𐡄𐡍 (entry->type_valid &&
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
	لكل (i = 0; i < to_pack.nr_objects; i++)
		break_delta_chains(&to_pack.objects[i]);

	حرر(sorted_by_offset);
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
ساكن صحيح type_size_sort(ثابت فراغ *_a, ثابت فراغ *_b)
{
	ثابت بنية object_entry *a = *(بنية object_entry **)_a;
	ثابت بنية object_entry *b = *(بنية object_entry **)_b;
	ثابت تعداد object_type a_type = oe_type(a);
	ثابت تعداد object_type b_type = oe_type(b);
	ثابت unsigned long a_size = SIZE(a);
	ثابت unsigned long b_size = SIZE(b);

	𐡄𐡍 (a_type > b_type)
		أرجع -1;
	𐡄𐡍 (a_type < b_type)
		أرجع 1;
	𐡄𐡍 (a->hash > b->hash)
		أرجع -1;
	𐡄𐡍 (a->hash < b->hash)
		أرجع 1;
	𐡄𐡍 (a->preferred_base > b->preferred_base)
		أرجع -1;
	𐡄𐡍 (a->preferred_base < b->preferred_base)
		أرجع 1;
	𐡄𐡍 (use_delta_islands) {
		ثابت صحيح island_cmp = island_delta_cmp(&a->idx.oid, &b->idx.oid);
		𐡄𐡍 (island_cmp)
			أرجع island_cmp;
	}
	𐡄𐡍 (a_size > b_size)
		أرجع -1;
	𐡄𐡍 (a_size < b_size)
		أرجع 1;
	أرجع a < b ? -1 : (a > b);  /* newest first */
}

بنية unpacked {
	بنية object_entry *entry;
	فراغ *data;
	بنية delta_index *index;
	unsigned depth;
};

ساكن صحيح delta_cacheable(size_t src_size, size_t trg_size,
			   size_t delta_size)
{
	𐡄𐡍 (max_delta_cache_size && delta_cache_size + delta_size > max_delta_cache_size)
		أرجع 0;

	𐡄𐡍 (delta_size < cache_max_small_delta_size)
		أرجع 1;

	/* cache delta, if objects are large enough compared to delta size */
	𐡄𐡍 ((src_size >> 20) + (trg_size >> 21) > (delta_size >> 10))
		أرجع 1;

	أرجع 0;
}

/* Protect delta_cache_size */
ساكن pthread_mutex_t cache_mutex;
#define cache_lock()		pthread_mutex_lock(&cache_mutex)
#define cache_unlock()		pthread_mutex_unlock(&cache_mutex)

/*
 * Protect object list partitioning (e.g. struct thread_param) and
 * progress_state
 */
ساكن pthread_mutex_t progress_mutex;
#define progress_lock()		pthread_mutex_lock(&progress_mutex)
#define progress_unlock()	pthread_mutex_unlock(&progress_mutex)

/*
 * Access to struct object_entry is unprotected since each thread owns
 * a portion of the main object list. Just don't access object entries
 * ahead in the list because they can be stolen and would need
 * progress_mutex for protection.
 */

ساكن مضمن صحيح oe_size_less_than(بنية packing_data *pack,
				    ثابت بنية object_entry *lhs,
				    size_t rhs)
{
	𐡄𐡍 (lhs->size_valid)
		أرجع lhs->size_ < rhs;
	𐡄𐡍 (rhs < pack->oe_size_limit) /* rhs < 2^x <= lhs ? */
		أرجع 0;
	أرجع oe_get_size_slow(pack, lhs) < rhs;
}

ساكن مضمن فراغ oe_set_tree_depth(بنية packing_data *pack,
				     بنية object_entry *e,
				     unsigned صحيح tree_depth)
{
	𐡄𐡍 (!pack->tree_depth)
		CALLOC_ARRAY(pack->tree_depth, pack->nr_alloc);
	pack->tree_depth[e - pack->objects] = tree_depth;
}

ساكن فراغ record_tree_depth(ثابت بنية object_id *oid, ثابت حرف *name)
{
	ثابت حرف *p;
	unsigned depth;
	بنية object_entry *ent;

	/* the empty string is a root tree, which is depth 0 */
	depth = *name ? 1 : 0;
	لكل (p = strchr(name, '/'); p; p = strchr(p + 1, '/'))
		depth++;

	ent = packlist_find(&to_pack, oid);
	𐡄𐡍 (ent && depth > oe_tree_depth(&to_pack, ent))
		oe_set_tree_depth(&to_pack, ent, depth);
}

/*
 * Return the size of the object without doing any delta
 * reconstruction (so non-deltas are true object sizes, but deltas
 * return the size of the delta data).
 */
size_t oe_get_size_slow(بنية packing_data *pack,
			ثابت بنية object_entry *e)
{
	بنية packed_git *p;
	بنية pack_window *w_curs;
	unsigned حرف *buf;
	تعداد object_type type;
	size_t used, avail, size;

	𐡄𐡍 (e->type_ != OBJ_OFS_DELTA && e->type_ != OBJ_REF_DELTA) {
		size_t sz;
		packing_data_lock(&to_pack);
		𐡄𐡍 (odb_read_object_info(the_repository->objects,
					 &e->idx.oid, &sz) < 0)
			die(_("unable to get size of %s"),
			    oid_to_hex(&e->idx.oid));
		packing_data_unlock(&to_pack);
		أرجع sz;
	}

	p = oe_in_pack(pack, e);
	𐡄𐡍 (!p)
		BUG("when e->type is a delta, it must belong to a pack");

	packing_data_lock(&to_pack);
	w_curs = عدم;
	buf = use_pack(p, &w_curs, e->in_pack_offset, &avail);
	used = unpack_object_header_buffer(buf, avail, &type, &size);
	𐡄𐡍 (used == 0)
		die(_("unable to parse object header of %s"),
		    oid_to_hex(&e->idx.oid));

	unuse_pack(&w_curs);
	packing_data_unlock(&to_pack);
	أرجع size;
}

ساكن صحيح try_delta(بنية unpacked *trg, بنية unpacked *src,
		     unsigned max_depth, size_t *mem_usage)
{
	بنية object_entry *trg_entry = trg->entry;
	بنية object_entry *src_entry = src->entry;
	unsigned long trg_size, src_size, sizediff, max_size, sz;
	size_t delta_size;
	unsigned ref_depth;
	تعداد object_type type;
	فراغ *delta_buf;

	/* Don't bother doing diffs between different types */
	𐡄𐡍 (oe_type(trg_entry) != oe_type(src_entry))
		أرجع -1;

	/*
	 * We do not bother to try a delta that we discarded on an
	 * earlier try, but only when reusing delta data.  Note that
	 * src_entry that is marked as the preferred_base should always
	 * be considered, as even if we produce a suboptimal delta against
	 * it, we will still save the transfer cost, as we already know
	 * the other side has it and we won't send src_entry at all.
	 */
	𐡄𐡍 (reuse_delta && IN_PACK(trg_entry) &&
	    IN_PACK(trg_entry) == IN_PACK(src_entry) &&
	    !src_entry->preferred_base &&
	    trg_entry->in_pack_type != OBJ_REF_DELTA &&
	    trg_entry->in_pack_type != OBJ_OFS_DELTA)
		أرجع 0;

	/* Let's not bust the allowed depth. */
	𐡄𐡍 (src->depth >= max_depth)
		أرجع 0;

	/* Now some size filtering heuristics. */
	trg_size = SIZE(trg_entry);
	𐡄𐡍 (!DELTA(trg_entry)) {
		max_size = trg_size/2 - the_hash_algo->rawsz;
		ref_depth = 1;
	} وإلا {
		max_size = DELTA_SIZE(trg_entry);
		ref_depth = trg->depth;
	}
	max_size = (uint64_t)max_size * (max_depth - src->depth) /
						(max_depth - ref_depth + 1);
	𐡄𐡍 (max_size == 0)
		أرجع 0;
	src_size = SIZE(src_entry);
	sizediff = src_size < trg_size ? trg_size - src_size : 0;
	𐡄𐡍 (sizediff >= max_size)
		أرجع 0;
	𐡄𐡍 (trg_size < src_size / 32)
		أرجع 0;

	𐡄𐡍 (!in_same_island(&trg->entry->idx.oid, &src->entry->idx.oid))
		أرجع 0;

	/* Load data if not already done */
	𐡄𐡍 (!trg->data) {
		size_t sz_st = 0;
		packing_data_lock(&to_pack);
		trg->data = odb_read_object(the_repository->objects,
					    &trg_entry->idx.oid, &type,
					    &sz_st);
		sz = cast_size_t_to_ulong(sz_st);
		packing_data_unlock(&to_pack);
		𐡄𐡍 (!trg->data)
			die(_("object %s cannot be read"),
			    oid_to_hex(&trg_entry->idx.oid));
		𐡄𐡍 (sz != trg_size)
			die(_("object %s inconsistent object length (%"PRIuMAX" vs %"PRIuMAX")"),
			    oid_to_hex(&trg_entry->idx.oid), (uintmax_t)sz,
			    (uintmax_t)trg_size);
		*mem_usage += sz;
	}
	𐡄𐡍 (!src->data) {
		size_t sz_st = 0;
		packing_data_lock(&to_pack);
		src->data = odb_read_object(the_repository->objects,
					    &src_entry->idx.oid, &type,
					    &sz_st);
		sz = cast_size_t_to_ulong(sz_st);
		packing_data_unlock(&to_pack);
		𐡄𐡍 (!src->data) {
			𐡄𐡍 (src_entry->preferred_base) {
				ساكن صحيح warned = 0;
				𐡄𐡍 (!warned++)
					warning(_("object %s cannot be read"),
						oid_to_hex(&src_entry->idx.oid));
				/*
				 * Those objects are not included in the
				 * resulting pack.  Be resilient and ignore
				 * them if they can't be read, in case the
				 * pack could be created nevertheless.
				 */
				أرجع 0;
			}
			die(_("object %s cannot be read"),
			    oid_to_hex(&src_entry->idx.oid));
		}
		𐡄𐡍 (sz != src_size)
			die(_("object %s inconsistent object length (%"PRIuMAX" vs %"PRIuMAX")"),
			    oid_to_hex(&src_entry->idx.oid), (uintmax_t)sz,
			    (uintmax_t)src_size);
		*mem_usage += sz;
	}
	𐡄𐡍 (!src->index) {
		src->index = create_delta_index(src->data, src_size);
		𐡄𐡍 (!src->index) {
			ساكن صحيح warned = 0;
			𐡄𐡍 (!warned++)
				warning(_("suboptimal pack - out of memory"));
			أرجع 0;
		}
		*mem_usage += sizeof_delta_index(src->index);
	}

	delta_buf = create_delta(src->index, trg->data, trg_size, &delta_size, max_size);
	𐡄𐡍 (!delta_buf)
		أرجع 0;

	𐡄𐡍 (DELTA(trg_entry)) {
		/* Prefer only shallower same-sized deltas. */
		𐡄𐡍 (delta_size == DELTA_SIZE(trg_entry) &&
		    src->depth + 1 >= trg->depth) {
			حرر(delta_buf);
			أرجع 0;
		}
	}

	/*
	 * Handle memory allocation outside of the cache
	 * accounting lock.  Compiler will optimize the strangeness
	 * away when NO_PTHREADS is defined.
	 */
	حرر(trg_entry->delta_data);
	cache_lock();
	𐡄𐡍 (trg_entry->delta_data) {
		delta_cache_size -= DELTA_SIZE(trg_entry);
		trg_entry->delta_data = عدم;
	}
	𐡄𐡍 (delta_cacheable(src_size, trg_size, delta_size)) {
		delta_cache_size += delta_size;
		cache_unlock();
		trg_entry->delta_data = xrealloc(delta_buf, delta_size);
	} وإلا {
		cache_unlock();
		حرر(delta_buf);
	}

	SET_DELTA(trg_entry, src_entry);
	SET_DELTA_SIZE(trg_entry, delta_size);
	trg->depth = src->depth + 1;

	أرجع 1;
}

ساكن unsigned صحيح check_delta_limit(بنية object_entry *me, unsigned صحيح n)
{
	بنية object_entry *child = DELTA_CHILD(me);
	unsigned صحيح m = n;
	طالما (child) {
		ثابت unsigned صحيح c = check_delta_limit(child, n + 1);
		𐡄𐡍 (m < c)
			m = c;
		child = DELTA_SIBLING(child);
	}
	أرجع m;
}

ساكن size_t free_unpacked(بنية unpacked *n)
{
	size_t freed_mem = sizeof_delta_index(n->index);
	free_delta_index(n->index);
	n->index = عدم;
	𐡄𐡍 (n->data) {
		freed_mem += SIZE(n->entry);
		FREE_AND_NULL(n->data);
	}
	n->entry = عدم;
	n->depth = 0;
	أرجع freed_mem;
}

ساكن فراغ find_deltas(بنية object_entry **list, unsigned *list_size,
			صحيح window, صحيح depth, unsigned *processed)
{
	uint32_t i, idx = 0, count = 0;
	بنية unpacked *array;
	size_t mem_usage = 0;

	CALLOC_ARRAY(array, window);

	لكل (;;) {
		بنية object_entry *entry;
		بنية unpacked *n = array + idx;
		صحيح j, max_depth, best_base = -1;

		progress_lock();
		𐡄𐡍 (!*list_size) {
			progress_unlock();
			اكسر;
		}
		entry = *list++;
		(*list_size)--;
		𐡄𐡍 (!entry->preferred_base) {
			(*processed)++;
			display_progress(progress_state, *processed);
		}
		progress_unlock();

		mem_usage -= free_unpacked(n);
		n->entry = entry;

		طالما (window_memory_limit &&
		       mem_usage > window_memory_limit &&
		       count > 1) {
			ثابت uint32_t tail = (idx + window - count) % window;
			mem_usage -= free_unpacked(array + tail);
			count--;
		}

		/* We do not compute delta to *create* objects we are not
		 * going to pack.
		 */
		𐡄𐡍 (entry->preferred_base)
			اذهب next;

		/*
		 * If the current object is at pack edge, take the depth the
		 * objects that depend on the current object into account
		 * otherwise they would become too deep.
		 */
		max_depth = depth;
		𐡄𐡍 (DELTA_CHILD(entry)) {
			max_depth -= check_delta_limit(entry, 0);
			𐡄𐡍 (max_depth <= 0)
				اذهب next;
		}

		j = window;
		طالما (--j > 0) {
			صحيح ret;
			uint32_t other_idx = idx + j;
			بنية unpacked *m;
			𐡄𐡍 (other_idx >= window)
				other_idx -= window;
			m = array + other_idx;
			𐡄𐡍 (!m->entry)
				اكسر;
			ret = try_delta(n, m, max_depth, &mem_usage);
			𐡄𐡍 (ret < 0)
				اكسر;
			وإلا 𐡄𐡍 (ret > 0)
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
		𐡄𐡍 (entry->delta_data && !pack_to_stdout) {
			unsigned long size;

			size = do_compress(&entry->delta_data, DELTA_SIZE(entry));
			𐡄𐡍 (size < (1U << OE_Z_DELTA_BITS)) {
				entry->z_delta_size = size;
				cache_lock();
				delta_cache_size -= DELTA_SIZE(entry);
				delta_cache_size += entry->z_delta_size;
				cache_unlock();
			} وإلا {
				FREE_AND_NULL(entry->delta_data);
				entry->z_delta_size = 0;
			}
		}

		/* if we made n a delta, and if n is already at max
		 * depth, leaving it in the window is pointless.  we
		 * should evict it first.
		 */
		𐡄𐡍 (DELTA(entry) && max_depth <= n->depth)
			استمر;

		/*
		 * Move the best delta base up in the window, after the
		 * currently deltified object, to keep it longer.  It will
		 * be the first base object to be attempted next.
		 */
		𐡄𐡍 (DELTA(entry)) {
			بنية unpacked swap = array[best_base];
			صحيح dist = (window + idx - best_base) % window;
			صحيح dst = best_base;
			طالما (dist--) {
				صحيح src = (dst + 1) % window;
				array[dst] = array[src];
				dst = src;
			}
			array[dst] = swap;
		}

		next:
		idx++;
		𐡄𐡍 (count + 1 < window)
			count++;
		𐡄𐡍 (idx >= window)
			idx = 0;
	}

	لكل (i = 0; i < window; ++i) {
		free_delta_index(array[i].index);
		حرر(array[i].data);
	}
	حرر(array);
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

بنية thread_params {
	pthread_t thread;
	بنية object_entry **list;
	بنية packing_region *regions;
	unsigned list_size;
	unsigned remaining;
	صحيح window;
	صحيح depth;
	صحيح working;
	صحيح data_ready;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	unsigned *processed;
};

ساكن pthread_cond_t progress_cond;

/*
 * Mutex and conditional variable can't be statically-initialized on Windows.
 */
ساكن فراغ init_threaded_search(فراغ)
{
	pthread_mutex_init(&cache_mutex, عدم);
	pthread_mutex_init(&progress_mutex, عدم);
	pthread_cond_init(&progress_cond, عدم);
}

ساكن فراغ cleanup_threaded_search(فراغ)
{
	pthread_cond_destroy(&progress_cond);
	pthread_mutex_destroy(&cache_mutex);
	pthread_mutex_destroy(&progress_mutex);
}

ساكن فراغ *threaded_find_deltas(فراغ *arg)
{
	بنية thread_params *me = arg;

	progress_lock();
	طالما (me->remaining) {
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
		طالما (!me->data_ready)
			pthread_cond_wait(&me->cond, &me->mutex);
		me->data_ready = 0;
		pthread_mutex_unlock(&me->mutex);

		progress_lock();
	}
	progress_unlock();
	/* leave ->working 1 so that this doesn't get more work assigned */
	أرجع عدم;
}

ساكن فراغ ll_find_deltas(بنية object_entry **list, unsigned list_size,
			   صحيح window, صحيح depth, unsigned *processed)
{
	بنية thread_params *p;
	صحيح i, ret, active_threads = 0;

	init_threaded_search();

	𐡄𐡍 (delta_search_threads <= 1) {
		find_deltas(list, &list_size, window, depth, processed);
		cleanup_threaded_search();
		أرجع;
	}
	𐡄𐡍 (progress > pack_to_stdout)
		fprintf_ln(stderr, _("Delta compression using up to %d threads"),
			   delta_search_threads);
	CALLOC_ARRAY(p, delta_search_threads);

	/* Partition the work amongst work threads. */
	لكل (i = 0; i < delta_search_threads; i++) {
		unsigned sub_size = list_size / (delta_search_threads - i);

		/* don't use too small segments or no deltas will be found */
		𐡄𐡍 (sub_size < 2*window && i+1 < delta_search_threads)
			sub_size = 0;

		p[i].window = window;
		p[i].depth = depth;
		p[i].processed = processed;
		p[i].working = 1;
		p[i].data_ready = 0;

		/* try to split chunks on "path" boundaries */
		طالما (sub_size && sub_size < list_size &&
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
	لكل (i = 0; i < delta_search_threads; i++) {
		𐡄𐡍 (!p[i].list_size)
			استمر;
		pthread_mutex_init(&p[i].mutex, عدم);
		pthread_cond_init(&p[i].cond, عدم);
		ret = pthread_create(&p[i].thread, عدم,
				     threaded_find_deltas, &p[i]);
		𐡄𐡍 (ret)
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
	طالما (active_threads) {
		بنية thread_params *target = عدم;
		بنية thread_params *victim = عدم;
		unsigned sub_size = 0;

		progress_lock();
		لكل (;;) {
			لكل (i = 0; !target && i < delta_search_threads; i++)
				𐡄𐡍 (!p[i].working)
					target = &p[i];
			𐡄𐡍 (target)
				اكسر;
			pthread_cond_wait(&progress_cond, &progress_mutex);
		}

		لكل (i = 0; i < delta_search_threads; i++)
			𐡄𐡍 (p[i].remaining > 2*window &&
			    (!victim || victim->remaining < p[i].remaining))
				victim = &p[i];
		𐡄𐡍 (victim) {
			sub_size = victim->remaining / 2;
			list = victim->list + victim->list_size - sub_size;
			طالما (sub_size && list[0]->hash &&
			       list[0]->hash == list[-1]->hash) {
				list++;
				sub_size--;
			}
			𐡄𐡍 (!sub_size) {
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

		𐡄𐡍 (!sub_size) {
			pthread_join(target->thread, عدم);
			pthread_cond_destroy(&target->cond);
			pthread_mutex_destroy(&target->mutex);
			active_threads--;
		}
	}
	cleanup_threaded_search();
	حرر(p);
}

ساكن صحيح obj_is_packed(ثابت بنية object_id *oid)
{
	أرجع packlist_find(&to_pack, oid) ||
		(reuse_packfile_bitmap &&
		 bitmap_walk_contains(bitmap_git, reuse_packfile_bitmap, oid));
}

ساكن فراغ add_tag_chain(ثابت بنية object_id *oid)
{
	بنية tag *tag;

	/*
	 * We catch duplicates already in add_object_entry(), but we'd
	 * prefer to do this extra check to avoid having to parse the
	 * tag at all if we already know that it's being packed (e.g., if
	 * it was included via bitmaps, we would not have parsed it
	 * previously).
	 */
	𐡄𐡍 (obj_is_packed(oid))
		أرجع;

	tag = lookup_tag(the_repository, oid);
	طالما (1) {
		𐡄𐡍 (!tag || parse_tag(the_repository, tag) || !tag->tagged)
			die(_("unable to pack objects reachable from tag %s"),
			    oid_to_hex(oid));

		add_object_entry(&tag->object.oid, OBJ_TAG, عدم, 0);

		𐡄𐡍 (tag->tagged->type != OBJ_TAG)
			أرجع;

		tag = (بنية tag *)tag->tagged;
	}
}

ساكن صحيح add_ref_tag(ثابت بنية reference *ref, فراغ *cb_data UNUSED)
{
	بنية object_id peeled;

	𐡄𐡍 (!reference_get_peeled_oid(the_repository, ref, &peeled) &&
	    obj_is_packed(&peeled))
		add_tag_chain(ref->oid);
	أرجع 0;
}

ساكن صحيح should_attempt_deltas(بنية object_entry *entry)
{
	𐡄𐡍 (DELTA(entry))
		/* This happens if we decided to reuse existing
		 * delta from a pack. "reuse_delta &&" is implied.
		 */
		أرجع 0;

	𐡄𐡍 (!entry->type_valid ||
	    oe_size_less_than(&to_pack, entry, 50))
		أرجع 0;

	𐡄𐡍 (entry->no_try_delta)
		أرجع 0;

	𐡄𐡍 (!entry->preferred_base) {
		𐡄𐡍 (oe_type(entry) < 0)
			die(_("unable to get type of object %s"),
				oid_to_hex(&entry->idx.oid));
	} وإلا 𐡄𐡍 (oe_type(entry) < 0) {
		/*
		 * This object is not found, but we
		 * don't have to include it anyway.
		 */
		أرجع 0;
	}

	أرجع 1;
}

ساكن فراغ find_deltas_for_region(بنية object_entry *list,
				   بنية packing_region *region,
				   unsigned صحيح *processed)
{
	بنية object_entry **delta_list;
	unsigned صحيح delta_list_nr = 0;

	ALLOC_ARRAY(delta_list, region->nr);
	لكل (size_t i = 0; i < region->nr; i++) {
		بنية object_entry *entry = list + region->start + i;
		𐡄𐡍 (should_attempt_deltas(entry))
			delta_list[delta_list_nr++] = entry;
	}

	QSORT(delta_list, delta_list_nr, type_size_sort);
	find_deltas(delta_list, &delta_list_nr, window, depth, processed);
	حرر(delta_list);
}

ساكن فراغ find_deltas_by_region(بنية object_entry *list,
				  بنية packing_region *regions,
				  size_t start, size_t nr)
{
	unsigned صحيح processed = 0;
	size_t progress_nr;

	𐡄𐡍 (!nr)
		أرجع;

	progress_nr = regions[nr - 1].start + regions[nr - 1].nr;

	𐡄𐡍 (progress)
		progress_state = start_progress(the_repository,
						_("Compressing objects by path"),
						progress_nr);

	طالما (nr--)
		find_deltas_for_region(list,
				       &regions[start++],
				       &processed);

	display_progress(progress_state, progress_nr);
	stop_progress(&progress_state);
}

ساكن فراغ *threaded_find_deltas_by_path(فراغ *arg)
{
	بنية thread_params *me = arg;

	progress_lock();
	طالما (me->remaining) {
		طالما (me->remaining) {
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
		طالما (!me->data_ready)
			pthread_cond_wait(&me->cond, &me->mutex);
		me->data_ready = 0;
		pthread_mutex_unlock(&me->mutex);

		progress_lock();
	}
	progress_unlock();
	/* leave ->working 1 so that this doesn't get more work assigned */
	أرجع عدم;
}

ساكن فراغ ll_find_deltas_by_region(بنية object_entry *list,
				     بنية packing_region *regions,
				     uint32_t start, uint32_t nr)
{
	بنية thread_params *p;
	صحيح i, ret, active_threads = 0;
	unsigned صحيح processed = 0;
	uint32_t progress_nr;
	init_threaded_search();

	𐡄𐡍 (!nr)
		أرجع;

	progress_nr =  regions[nr - 1].start + regions[nr - 1].nr;
	𐡄𐡍 (delta_search_threads <= 1) {
		find_deltas_by_region(list, regions, start, nr);
		cleanup_threaded_search();
		أرجع;
	}

	𐡄𐡍 (progress > pack_to_stdout)
		fprintf_ln(stderr,
			   Q_("Path-based delta compression using up to %d thread",
			      "Path-based delta compression using up to %d threads",
			      delta_search_threads),
			   delta_search_threads);
	CALLOC_ARRAY(p, delta_search_threads);

	𐡄𐡍 (progress)
		progress_state = start_progress(the_repository,
						_("Compressing objects by path"),
						progress_nr);
	/* Partition the work amongst work threads. */
	لكل (i = 0; i < delta_search_threads; i++) {
		unsigned sub_size = nr / (delta_search_threads - i);

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
	لكل (i = 0; i < delta_search_threads; i++) {
		𐡄𐡍 (!p[i].list_size)
			استمر;
		pthread_mutex_init(&p[i].mutex, عدم);
		pthread_cond_init(&p[i].cond, عدم);
		ret = pthread_create(&p[i].thread, عدم,
				     threaded_find_deltas_by_path, &p[i]);
		𐡄𐡍 (ret)
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
	طالما (active_threads) {
		بنية thread_params *target = عدم;
		بنية thread_params *victim = عدم;
		unsigned sub_size = 0;

		progress_lock();
		لكل (;;) {
			لكل (i = 0; !target && i < delta_search_threads; i++)
				𐡄𐡍 (!p[i].working)
					target = &p[i];
			𐡄𐡍 (target)
				اكسر;
			pthread_cond_wait(&progress_cond, &progress_mutex);
		}

		لكل (i = 0; i < delta_search_threads; i++)
			𐡄𐡍 (p[i].remaining > 2*window &&
			    (!victim || victim->remaining < p[i].remaining))
				victim = &p[i];
		𐡄𐡍 (victim) {
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

		𐡄𐡍 (!sub_size) {
			pthread_join(target->thread, عدم);
			pthread_cond_destroy(&target->cond);
			pthread_mutex_destroy(&target->mutex);
			active_threads--;
		}
	}
	cleanup_threaded_search();
	حرر(p);

	display_progress(progress_state, progress_nr);
	stop_progress(&progress_state);
}

ساكن فراغ prepare_pack(صحيح window, صحيح depth)
{
	بنية object_entry **delta_list;
	uint32_t i, nr_deltas;
	unsigned n;

	𐡄𐡍 (use_delta_islands)
		resolve_tree_islands(the_repository, progress, &to_pack);

	get_object_details();

	/*
	 * If we're locally repacking then we need to be doubly careful
	 * from now on in order to make sure no stealth corruption gets
	 * propagated to the new pack.  Clients receiving streamed packs
	 * should validate everything they get anyway so no need to incur
	 * the additional cost here in that case.
	 */
	𐡄𐡍 (!pack_to_stdout)
		do_check_packed_object_crc = 1;

	𐡄𐡍 (!to_pack.nr_objects || !window || !depth)
		أرجع;

	𐡄𐡍 (path_walk)
		ll_find_deltas_by_region(to_pack.objects, to_pack.regions,
					 0, to_pack.nr_regions);

	ALLOC_ARRAY(delta_list, to_pack.nr_objects);
	nr_deltas = n = 0;

	لكل (i = 0; i < to_pack.nr_objects; i++) {
		بنية object_entry *entry = to_pack.objects + i;

		𐡄𐡍 (!should_attempt_deltas(entry))
			استمر;

		𐡄𐡍 (!entry->preferred_base)
			nr_deltas++;

		delta_list[n++] = entry;
	}

	𐡄𐡍 (nr_deltas && n > 1) {
		unsigned nr_done = 0;

		𐡄𐡍 (progress)
			progress_state = start_progress(the_repository,
							_("Compressing objects"),
							nr_deltas);
		QSORT(delta_list, n, type_size_sort);
		ll_find_deltas(delta_list, n, window+1, depth, &nr_done);
		stop_progress(&progress_state);
		𐡄𐡍 (nr_done != nr_deltas)
			die(_("inconsistency with delta count"));
	}
	حرر(delta_list);
}

ساكن صحيح git_pack_config(ثابت حرف *k, ثابت حرف *v,
			   ثابت بنية config_context *ctx, فراغ *cb)
{
	𐡄𐡍 (!strcmp(k, "pack.window")) {
		window = git_config_int(k, v, ctx->kvi);
		أرجع 0;
	}
	𐡄𐡍 (!strcmp(k, "pack.windowmemory")) {
		window_memory_limit = git_config_ulong(k, v, ctx->kvi);
		أرجع 0;
	}
	𐡄𐡍 (!strcmp(k, "pack.depth")) {
		depth = git_config_int(k, v, ctx->kvi);
		أرجع 0;
	}
	𐡄𐡍 (!strcmp(k, "pack.deltacachesize")) {
		max_delta_cache_size = git_config_size_t(k, v, ctx->kvi);
		أرجع 0;
	}
	𐡄𐡍 (!strcmp(k, "pack.deltacachelimit")) {
		cache_max_small_delta_size = git_config_int(k, v, ctx->kvi);
		أرجع 0;
	}
	𐡄𐡍 (!strcmp(k, "pack.writebitmaphashcache")) {
		𐡄𐡍 (git_config_bool(k, v))
			write_bitmap_options |= BITMAP_OPT_HASH_CACHE;
		وإلا
			write_bitmap_options &= ~BITMAP_OPT_HASH_CACHE;
	}

	𐡄𐡍 (!strcmp(k, "pack.writebitmaplookuptable")) {
		𐡄𐡍 (git_config_bool(k, v))
			write_bitmap_options |= BITMAP_OPT_LOOKUP_TABLE;
		وإلا
			write_bitmap_options &= ~BITMAP_OPT_LOOKUP_TABLE;
	}

	𐡄𐡍 (!strcmp(k, "pack.usebitmaps")) {
		use_bitmap_index_default = git_config_bool(k, v);
		أرجع 0;
	}
	𐡄𐡍 (!strcmp(k, "pack.allowpackreuse")) {
		صحيح res = git_parse_maybe_bool_text(v);
		𐡄𐡍 (res < 0) {
			𐡄𐡍 (!strcasecmp(v, "single"))
				allow_pack_reuse = SINGLE_PACK_REUSE;
			وإلا 𐡄𐡍 (!strcasecmp(v, "multi"))
				allow_pack_reuse = MULTI_PACK_REUSE;
			وإلا
				die(_("invalid pack.allowPackReuse value: '%s'"), v);
		} وإلا 𐡄𐡍 (res) {
			allow_pack_reuse = SINGLE_PACK_REUSE;
		} وإلا {
			allow_pack_reuse = NO_PACK_REUSE;
		}
		أرجع 0;
	}
	𐡄𐡍 (!strcmp(k, "pack.threads")) {
		delta_search_threads = git_config_int(k, v, ctx->kvi);
		𐡄𐡍 (delta_search_threads < 0)
			die(_("invalid number of threads specified (%d)"),
			    delta_search_threads);
		𐡄𐡍 (!HAVE_THREADS && delta_search_threads != 1) {
			warning(_("no threads support, ignoring %s"), k);
			delta_search_threads = 0;
		}
		أرجع 0;
	}
	𐡄𐡍 (!strcmp(k, "pack.indexversion")) {
		pack_idx_opts.version = git_config_int(k, v, ctx->kvi);
		𐡄𐡍 (pack_idx_opts.version > 2)
			die(_("bad pack.indexVersion=%"PRIu32),
			    pack_idx_opts.version);
		أرجع 0;
	}
	𐡄𐡍 (!strcmp(k, "pack.writereverseindex")) {
		𐡄𐡍 (git_config_bool(k, v))
			pack_idx_opts.flags |= WRITE_REV;
		وإلا
			pack_idx_opts.flags &= ~WRITE_REV;
		أرجع 0;
	}
	𐡄𐡍 (!strcmp(k, "uploadpack.blobpackfileuri")) {
		بنية configured_exclusion *ex;
		ثابت حرف *oid_end, *pack_end;
		/*
		 * Stores the pack hash. This is not a true object ID, but is
		 * of the same form.
		 */
		بنية object_id pack_hash;

		𐡄𐡍 (!v)
			أرجع config_error_nonbool(k);

		ex = xmalloc(حجم(*ex));
		𐡄𐡍 (parse_oid_hex(v, &ex->e.oid, &oid_end) ||
		    *oid_end != ' ' ||
		    parse_oid_hex(oid_end + 1, &pack_hash, &pack_end) ||
		    *pack_end != ' ')
			die(_("value of uploadpack.blobpackfileuri must be "
			      "of the form '<object-hash> <pack-hash> <uri>' (got '%s')"), v);
		𐡄𐡍 (oidmap_get(&configured_exclusions, &ex->e.oid))
			die(_("object already configured in another "
			      "uploadpack.blobpackfileuri (got '%s')"), v);
		ex->pack_hash_hex = xcalloc(1, pack_end - oid_end);
		memcpy(ex->pack_hash_hex, oid_end + 1, pack_end - oid_end - 1);
		ex->uri = xstrdup(pack_end + 1);
		oidmap_put(&configured_exclusions, ex);
	}
	أرجع git_default_config(k, v, ctx, cb);
}

/* Counters for trace2 output when in --stdin-packs mode. */
ساكن صحيح stdin_packs_found_nr;
ساكن صحيح stdin_packs_hints_nr;

ساكن صحيح add_object_entry_from_pack(ثابت بنية object_id *oid,
				      بنية packed_git *p,
				      uint32_t pos,
				      فراغ *_data)
{
	off_t ofs;
	بنية object_info oi = OBJECT_INFO_INIT;
	تعداد object_type type = OBJ_NONE;

	display_progress(progress_state, ++nr_seen);

	𐡄𐡍 (have_duplicate_entry(oid, 0))
		أرجع 0;

	stdin_packs_found_nr++;

	ofs = nth_packed_object_offset(p, pos);

	oi.typep = &type;
	𐡄𐡍 (packed_object_info(عدم, p, ofs, &oi) < 0) {
		die(_("could not get type of object %s in pack %s"),
		    oid_to_hex(oid), p->pack_name);
	} وإلا 𐡄𐡍 (type == OBJ_COMMIT) {
		بنية rev_info *revs = _data;
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
		add_pending_oid(revs, عدم, oid, 0);
	}

	𐡄𐡍 (!want_object_in_pack(oid, 0, &p, &ofs))
		أرجع 0;

	create_object_entry(oid, type, 0, 0, 0, p, ofs);

	أرجع 0;
}

ساكن فراغ show_object_pack_hint(بنية object *object, ثابت حرف *name,
				  فراغ *data)
{
	تعداد stdin_packs_mode mode = *(تعداد stdin_packs_mode *)data;
	𐡄𐡍 (mode == STDIN_PACKS_MODE_FOLLOW) {
		𐡄𐡍 (object->type == OBJ_BLOB &&
		    !odb_has_object(the_repository->objects, &object->oid, 0))
			أرجع;
		add_object_entry(&object->oid, object->type, name, 0);
	} وإلا {
		بنية object_entry *oe = packlist_find(&to_pack, &object->oid);
		𐡄𐡍 (!oe)
			أرجع;

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

ساكن فراغ show_commit_pack_hint(بنية commit *commit, فراغ *data)
{
	تعداد stdin_packs_mode mode = *(تعداد stdin_packs_mode *)data;

	𐡄𐡍 (mode == STDIN_PACKS_MODE_FOLLOW) {
		show_object_pack_hint((بنية object *)commit, "", data);
		أرجع;
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
تعداد stdin_pack_info_kind {
	STDIN_PACK_INCLUDE = (1<<0),
	STDIN_PACK_EXCLUDE_CLOSED = (1<<1),
	STDIN_PACK_EXCLUDE_OPEN = (1<<2),
};

بنية stdin_pack_info {
	بنية packed_git *p;
	تعداد stdin_pack_info_kind kind;
};

ساكن صحيح pack_mtime_cmp(ثابت فراغ *_a, ثابت فراغ *_b)
{
	بنية stdin_pack_info *a = ((ثابت بنية string_list_item*)_a)->util;
	بنية stdin_pack_info *b = ((ثابت بنية string_list_item*)_b)->util;

	/*
	 * order packs by descending mtime so that objects are laid out
	 * roughly as newest-to-oldest
	 */
	𐡄𐡍 (a->p->mtime < b->p->mtime)
		أرجع 1;
	وإلا 𐡄𐡍 (b->p->mtime < a->p->mtime)
		أرجع -1;
	وإلا
		أرجع 0;
}

ساكن صحيح stdin_packs_include_check_obj(بنية object *obj, فراغ *data UNUSED)
{
	أرجع !has_object_kept_pack(to_pack.repo, &obj->oid,
				     KEPT_PACK_IN_CORE);
}

ساكن صحيح stdin_packs_include_check(بنية commit *commit, فراغ *data)
{
	أرجع stdin_packs_include_check_obj((بنية object *)commit, data);
}

ساكن فراغ stdin_packs_add_pack_entries(بنية strmap *packs,
					 بنية rev_info *revs)
{
	بنية string_list keys = STRING_LIST_INIT_NODUP;
	بنية string_list_item *item;
	بنية hashmap_iter iter;
	بنية strmap_entry *entry;

	strmap_for_each_entry(packs, &iter, entry) {
		بنية stdin_pack_info *info = entry->value;
		𐡄𐡍 (!info->p)
			die(_("could not find pack '%s'"), entry->key);

		string_list_append(&keys, entry->key)->util = info;
	}

	/*
	 * Order packs by ascending mtime; use QSORT directly to access the
	 * string_list_item's ->util pointer, which string_list_sort() does not
	 * provide.
	 */
	QSORT(keys.items, keys.nr, pack_mtime_cmp);

	for_each_string_list_item(item, &keys) {
		بنية stdin_pack_info *info = item->util;

		𐡄𐡍 (info->kind & STDIN_PACK_EXCLUDE_OPEN) {
			/*
			 * When open-excluded packs ("!") are present, stop
			 * the parent walk at closed-excluded ("^") packs.
			 * Objects behind a "^" boundary are guaranteed to
			 * have closure and should not be rescued.
			 */
			revs->include_check = stdin_packs_include_check;
			revs->include_check_obj = stdin_packs_include_check_obj;
		}

		𐡄𐡍 ((info->kind & STDIN_PACK_INCLUDE) ||
		    (info->kind & STDIN_PACK_EXCLUDE_OPEN))
			for_each_object_in_pack(info->p,
						add_object_entry_from_pack,
						revs,
						ODB_FOR_EACH_OBJECT_PACK_ORDER);
	}

	string_list_clear(&keys, 0);
}

ساكن فراغ stdin_packs_read_input(بنية rev_info *revs,
				   تعداد stdin_packs_mode mode)
{
	بنية strbuf buf = STRBUF_INIT;
	بنية strmap packs = STRMAP_INIT;
	بنية packed_git *p;

	طالما (strbuf_getline(&buf, stdin) != EOF) {
		بنية stdin_pack_info *info;
		تعداد stdin_pack_info_kind kind = STDIN_PACK_INCLUDE;
		ثابت حرف *key = buf.buf;

		𐡄𐡍 (!*key)
			استمر;
		وإلا 𐡄𐡍 (*key == '^')
			kind = STDIN_PACK_EXCLUDE_CLOSED;
		وإلا 𐡄𐡍 (*key == '!' && mode == STDIN_PACKS_MODE_FOLLOW)
			kind = STDIN_PACK_EXCLUDE_OPEN;

		𐡄𐡍 (kind != STDIN_PACK_INCLUDE)
			key++;

		info = strmap_get(&packs, key);
		𐡄𐡍 (!info) {
			CALLOC_ARRAY(info, 1);
			strmap_put(&packs, key, info);
		}

		info->kind |= kind;

		strbuf_reset(&buf);
	}

	repo_for_each_pack(the_repository, p) {
		بنية stdin_pack_info *info;

		info = strmap_get(&packs, pack_basename(p));
		𐡄𐡍 (!info)
			استمر;

		𐡄𐡍 (info->kind & STDIN_PACK_INCLUDE) {
			𐡄𐡍 (exclude_promisor_objects && p->pack_promisor)
				die(_("packfile %s is a promisor but --exclude-promisor-objects was given"), p->pack_name);

			/*
			 * Arguments we got on stdin may not even be
			 * packs. First check that to avoid segfaulting
			 * later on in e.g.  pack_mtime_cmp(), excluded
			 * packs are handled below.
			 */
			𐡄𐡍 (!is_pack_valid(p))
				die(_("packfile %s cannot be accessed"), p->pack_name);
		}

		𐡄𐡍 (info->kind & STDIN_PACK_EXCLUDE_CLOSED) {
			/*
			 * Marking excluded packs as kept in-core so
			 * that later calls to add_object_entry()
			 * discards any objects that are also found in
			 * excluded packs.
			 */
			p->pack_keep_in_core = 1;
		}

		𐡄𐡍 (info->kind & STDIN_PACK_EXCLUDE_OPEN) {
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

ساكن فراغ add_unreachable_loose_objects(بنية rev_info *revs);

ساكن فراغ read_stdin_packs(بنية repository *repo,
			     تعداد stdin_packs_mode mode, صحيح rev_list_unpacked)
{
	صحيح prev_fetch_if_missing = repo->fetch_if_missing;
	بنية rev_info revs;

	/*
	 * The revision walk may hit objects that are promised, only. As the
	 * walk is best-effort though we don't want to perform backfill fetches
	 * for them.
	 */
	repo->fetch_if_missing = 0;

	repo_init_revisions(repo, &revs, عدم);
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
	revs.tree_objects = 1;
	revs.tag_objects = 1;
	revs.ignore_missing_links = 1;
	revs.exclude_promisor_objects = exclude_promisor_objects;

	/* avoids adding objects in excluded packs */
	ignore_packed_keep_in_core = 1;
	𐡄𐡍 (mode == STDIN_PACKS_MODE_FOLLOW) {
		/*
		 * In '--stdin-packs=follow' mode, additionally ignore
		 * objects in excluded-open packs to prevent them from
		 * appearing in the resulting pack.
		 */
		ignore_packed_keep_in_core_open = 1;
	}
	stdin_packs_read_input(&revs, mode);
	𐡄𐡍 (rev_list_unpacked)
		add_unreachable_loose_objects(&revs);

	𐡄𐡍 (prepare_revision_walk(&revs))
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

ساكن فراغ add_cruft_object_entry(ثابت بنية object_id *oid, تعداد object_type type,
				   بنية packed_git *pack, off_t offset,
				   ثابت حرف *name, uint32_t mtime)
{
	بنية object_entry *entry;

	display_progress(progress_state, ++nr_seen);

	entry = packlist_find(&to_pack, oid);
	𐡄𐡍 (entry) {
		𐡄𐡍 (name) {
			entry->hash = pack_name_hash_fn(name);
			entry->no_try_delta = no_try_delta(name);
		}
	} وإلا {
		𐡄𐡍 (!want_object_in_pack_mtime(oid, 0, &pack, &offset, mtime))
			أرجع;
		𐡄𐡍 (!pack && type == OBJ_BLOB) {
			بنية odb_source_files *files =
				odb_source_files_downcast(the_repository->objects->source);
			صحيح found = 0;

			لكل (بنية odb_files_dir *dir = files->dirs; !found && dir; dir = dir->next)
				𐡄𐡍 (!odb_source_read_object_info(&dir->loose->base, oid, عدم, 0, عدم))
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
			𐡄𐡍 (!found)
				أرجع;
		}

		entry = create_object_entry(oid, type, pack_name_hash_fn(name),
					    0, name && no_try_delta(name),
					    pack, offset);
	}

	𐡄𐡍 (mtime > oe_cruft_mtime(&to_pack, entry))
		oe_set_cruft_mtime(&to_pack, entry, mtime);
	أرجع;
}

ساكن فراغ show_cruft_object(بنية object *obj, ثابت حرف *name, فراغ *data UNUSED)
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
	add_cruft_object_entry(&obj->oid, obj->type, عدم, 0, name, cruft_expiration);
}

ساكن فراغ show_cruft_commit(بنية commit *commit, فراغ *data)
{
	show_cruft_object((بنية object*)commit, عدم, data);
}

ساكن صحيح cruft_include_check_obj(بنية object *obj, فراغ *data UNUSED)
{
	أرجع !has_object_kept_pack(to_pack.repo, &obj->oid, KEPT_PACK_IN_CORE);
}

ساكن صحيح cruft_include_check(بنية commit *commit, فراغ *data)
{
	أرجع cruft_include_check_obj((بنية object*)commit, data);
}

ساكن فراغ set_cruft_mtime(ثابت بنية object *object,
			    بنية packed_git *pack,
			    off_t offset, time_t mtime)
{
	add_cruft_object_entry(&object->oid, object->type, pack, offset, عدم,
			       mtime);
}

ساكن فراغ mark_pack_kept_in_core(بنية string_list *packs, unsigned keep)
{
	بنية string_list_item *item = عدم;
	for_each_string_list_item(item, packs) {
		بنية packed_git *p = item->util;
		𐡄𐡍 (!p)
			die(_("could not find pack '%s'"), item->string);
		𐡄𐡍 (p->is_cruft && keep)
			ignore_packed_keep_in_core_has_cruft = 1;
		p->pack_keep_in_core = keep;
	}
}

ساكن فراغ add_objects_in_unpacked_packs(فراغ);

ساكن فراغ enumerate_cruft_objects(فراغ)
{
	𐡄𐡍 (progress)
		progress_state = start_progress(the_repository,
						_("Enumerating cruft objects"), 0);

	add_objects_in_unpacked_packs();
	add_unreachable_loose_objects(عدم);

	stop_progress(&progress_state);
}

ساكن فراغ enumerate_and_traverse_cruft_objects(بنية string_list *fresh_packs)
{
	بنية packed_git *p;
	بنية rev_info revs;
	صحيح ret;

	repo_init_revisions(the_repository, &revs, عدم);

	revs.tag_objects = 1;
	revs.tree_objects = 1;
	revs.blob_objects = 1;

	revs.include_check = cruft_include_check;
	revs.include_check_obj = cruft_include_check_obj;

	revs.ignore_missing_links = 1;

	𐡄𐡍 (progress)
		progress_state = start_progress(the_repository,
						_("Enumerating cruft objects"), 0);
	ret = add_unseen_recent_objects_to_traversal(&revs, cruft_expiration,
						     set_cruft_mtime, 1);
	stop_progress(&progress_state);

	𐡄𐡍 (ret)
		die(_("unable to add cruft objects"));

	/*
	 * Re-mark only the fresh packs as kept so that objects in
	 * unknown packs do not halt the reachability traversal early.
	 */
	repo_for_each_pack(the_repository, p)
		p->pack_keep_in_core = 0;
	mark_pack_kept_in_core(fresh_packs, 1);

	𐡄𐡍 (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	𐡄𐡍 (progress)
		progress_state = start_progress(the_repository,
						_("Traversing cruft objects"), 0);
	nr_seen = 0;
	traverse_commit_list(&revs, show_cruft_commit, show_cruft_object, عدم);

	stop_progress(&progress_state);
	release_revisions(&revs);
}

ساكن فراغ read_cruft_objects(فراغ)
{
	بنية strbuf buf = STRBUF_INIT;
	بنية string_list discard_packs = STRING_LIST_INIT_DUP;
	بنية string_list fresh_packs = STRING_LIST_INIT_DUP;
	بنية packed_git *p;

	ignore_packed_keep_in_core = 1;

	طالما (strbuf_getline(&buf, stdin) != EOF) {
		𐡄𐡍 (!buf.len)
			استمر;

		𐡄𐡍 (*buf.buf == '-')
			string_list_append(&discard_packs, buf.buf + 1);
		وإلا
			string_list_append(&fresh_packs, buf.buf);
	}

	string_list_sort(&discard_packs);
	string_list_sort(&fresh_packs);

	repo_for_each_pack(the_repository, p) {
		ثابت حرف *pack_name = pack_basename(p);
		بنية string_list_item *item;

		item = string_list_lookup(&fresh_packs, pack_name);
		𐡄𐡍 (!item)
			item = string_list_lookup(&discard_packs, pack_name);

		𐡄𐡍 (item) {
			item->util = p;
		} وإلا {
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

	𐡄𐡍 (cruft_expiration)
		enumerate_and_traverse_cruft_objects(&fresh_packs);
	وإلا
		enumerate_cruft_objects();

	strbuf_release(&buf);
	string_list_clear(&discard_packs, 0);
	string_list_clear(&fresh_packs, 0);
}

ساكن فراغ read_object_list_from_stdin(فراغ)
{
	حرف line[GIT_MAX_HEXSZ + 1 + PATH_MAX + 2];
	بنية object_id oid;
	ثابت حرف *p;

	لكل (;;) {
		𐡄𐡍 (!fgets(line, حجم(line), stdin)) {
			𐡄𐡍 (feof(stdin))
				اكسر;
			𐡄𐡍 (!ferror(stdin))
				BUG("fgets returned NULL, not EOF, not error!");
			𐡄𐡍 (errno != EINTR)
				die_errno("fgets");
			clearerr(stdin);
			استمر;
		}
		𐡄𐡍 (line[0] == '-') {
			𐡄𐡍 (get_oid_hex(line+1, &oid))
				die(_("expected edge object ID, got garbage:\n %s"),
				    line);
			add_preferred_base(&oid);
			استمر;
		}
		𐡄𐡍 (parse_oid_hex(line, &oid, &p))
			die(_("expected object ID, got garbage:\n %s"), line);

		add_preferred_base_object(p + 1);
		add_object_entry(&oid, OBJ_NONE, p + 1, 0);
	}
}

ساكن فراغ show_commit(بنية commit *commit, فراغ *data UNUSED)
{
	add_object_entry(&commit->object.oid, OBJ_COMMIT, عدم, 0);

	𐡄𐡍 (write_bitmap_index)
		index_commit_for_bitmap(commit);

	𐡄𐡍 (use_delta_islands)
		propagate_island_marks(the_repository, commit);
}

ساكن فراغ show_object(بنية object *obj, ثابت حرف *name,
			فراغ *data UNUSED)
{
	add_preferred_base_object(name);
	add_object_entry(&obj->oid, obj->type, name, 0);

	𐡄𐡍 (use_delta_islands)
		record_tree_depth(&obj->oid, name);
}

ساكن فراغ show_object__ma_allow_any(بنية object *obj, ثابت حرف *name, فراغ *data)
{
	أكد(arg_missing_action == MA_ALLOW_ANY);

	/*
	 * Quietly ignore ALL missing objects.  This avoids problems with
	 * staging them now and getting an odd error later.
	 */
	𐡄𐡍 (!odb_has_object(the_repository->objects, &obj->oid, 0))
		أرجع;

	show_object(obj, name, data);
}

ساكن فراغ show_object__ma_allow_promisor(بنية object *obj, ثابت حرف *name, فراغ *data)
{
	أكد(arg_missing_action == MA_ALLOW_PROMISOR);

	/*
	 * Quietly ignore EXPECTED missing objects.  This avoids problems with
	 * staging them now and getting an odd error later.
	 */
	𐡄𐡍 (!odb_has_object(the_repository->objects, &obj->oid, 0) &&
	    is_promisor_object(to_pack.repo, &obj->oid))
		أرجع;

	show_object(obj, name, data);
}

ساكن صحيح option_parse_missing_action(ثابت بنية option *opt,
				       ثابت حرف *arg, صحيح unset)
{
	بنية repository *repo = opt->value;

	أكد(arg);
	أكد(!unset);

	𐡄𐡍 (!strcmp(arg, "error")) {
		arg_missing_action = MA_ERROR;
		fn_show_object = show_object;
		أرجع 0;
	}

	𐡄𐡍 (!strcmp(arg, "allow-any")) {
		arg_missing_action = MA_ALLOW_ANY;
		repo->fetch_if_missing = 0;
		fn_show_object = show_object__ma_allow_any;
		أرجع 0;
	}

	𐡄𐡍 (!strcmp(arg, "allow-promisor")) {
		arg_missing_action = MA_ALLOW_PROMISOR;
		repo->fetch_if_missing = 0;
		fn_show_object = show_object__ma_allow_promisor;
		أرجع 0;
	}

	die(_("invalid value for '%s': '%s'"), "--missing", arg);
	أرجع 0;
}

ساكن فراغ show_edge(بنية commit *commit)
{
	add_preferred_base(&commit->object.oid);
}

ساكن صحيح add_object_in_unpacked_pack(ثابت بنية object_id *oid,
				       بنية object_info *oi,
				       فراغ *data UNUSED)
{
	𐡄𐡍 (cruft) {
		add_cruft_object_entry(oid, OBJ_NONE, oi->source_infop->u.packed.pack,
				       oi->source_infop->u.packed.offset, عدم,
				       *oi->mtimep);
	} وإلا {
		add_object_entry(oid, OBJ_NONE, "", 0);
	}
	أرجع 0;
}

ساكن فراغ add_objects_in_unpacked_packs(فراغ)
{
	بنية odb_source_files *files =
		odb_source_files_downcast(to_pack.repo->objects->source);
	time_t mtime;
	بنية odb_for_each_object_options opts = {
		.flags = ODB_FOR_EACH_OBJECT_PACK_ORDER |
			 ODB_FOR_EACH_OBJECT_LOCAL_ONLY |
			 ODB_FOR_EACH_OBJECT_SKIP_IN_CORE_KEPT_PACKS |
			 ODB_FOR_EACH_OBJECT_SKIP_ON_DISK_KEPT_PACKS,
	};
	بنية odb_source_info source_info;
	بنية object_info oi = {
		.mtimep = &mtime,
		.source_infop = &source_info,
	};

	لكل (بنية odb_files_dir *dir = files->dirs; dir; dir = dir->next) {
		𐡄𐡍 (!dir->local)
			استمر;

		𐡄𐡍 (odb_source_for_each_object(&dir->packed->base, &oi,
					       add_object_in_unpacked_pack, عدم, &opts))
			die(_("cannot open pack index"));
	}
}

ساكن صحيح add_loose_object(ثابت بنية object_id *oid, ثابت حرف *path,
			    فراغ *data)
{
	بنية rev_info *revs = data;
	تعداد object_type type = odb_read_object_info(the_repository->objects, oid, عدم);

	𐡄𐡍 (type < 0) {
		warning(_("loose object at %s could not be examined"), path);
		أرجع 0;
	}

	𐡄𐡍 (cruft) {
		بنية stat st;
		𐡄𐡍 (stat(path, &st) < 0) {
			𐡄𐡍 (errno == ENOENT)
				أرجع 0;
			أرجع error_errno("unable to stat %s", oid_to_hex(oid));
		}

		add_cruft_object_entry(oid, type, عدم, 0, عدم,
				       st.st_mtime);
	} وإلا {
		add_object_entry(oid, type, "", 0);
	}

	𐡄𐡍 (revs && type == OBJ_COMMIT)
		add_pending_oid(revs, عدم, oid, 0);

	أرجع 0;
}

/*
 * We actually don't even have to worry about reachability here.
 * add_object_entry will weed out duplicates, so we just add every
 * loose object we find.
 */
ساكن فراغ add_unreachable_loose_objects(بنية rev_info *revs)
{
	for_each_loose_file_in_source(the_repository->objects->source,
				      add_loose_object, عدم, عدم, revs);
}

ساكن صحيح has_sha1_pack_kept_or_nonlocal(ثابت بنية object_id *oid)
{
	ساكن بنية packed_git *last_found = عدم;
	بنية packed_git *p;

	𐡄𐡍 (last_found && find_pack_entry_one(oid, last_found))
		أرجع 1;

	repo_for_each_pack(the_repository, p) {
		/*
		 * We have already checked `last_found`, so there is no need to
		 * re-check here.
		 */
		𐡄𐡍 (p == last_found)
			استمر;

		𐡄𐡍 ((!p->pack_local || p->pack_keep || p->pack_keep_in_core) &&
		    find_pack_entry_one(oid, p)) {
			last_found = p;
			أرجع 1;
		}
	}

	أرجع 0;
}

/*
 * Store a list of sha1s that are should not be discarded
 * because they are either written too recently, or are
 * reachable from another object that was.
 *
 * This is filled by get_object_list.
 */
ساكن بنية oid_array recent_objects;

ساكن صحيح loosened_object_can_be_discarded(ثابت بنية object_id *oid,
					    timestamp_t mtime)
{
	𐡄𐡍 (!unpack_unreachable_expiration)
		أرجع 0;
	𐡄𐡍 (mtime > unpack_unreachable_expiration)
		أرجع 0;
	𐡄𐡍 (oid_array_lookup(&recent_objects, oid) >= 0)
		أرجع 0;
	أرجع 1;
}

ساكن صحيح force_object_loose(بنية odb_source *source,
			      ثابت بنية object_id *oid,
			      ثابت time_t *mtime)
{
	بنية odb_source_files *files = odb_source_files_downcast(source);
	ثابت بنية git_hash_algo *compat = source->odb->repo->compat_hash_algo;
	بنية object_info oi = OBJECT_INFO_INIT;
	بنية object_id compat_oid, *compat_oid_p = عدم;
	تعداد object_type type;
	فراغ *buf = عدم;
	size_t len;
	صحيح ret;

	لكل (بنية odb_files_dir *dir = files->dirs; dir; dir = dir->next)
		𐡄𐡍 (!odb_source_read_object_info(&dir->loose->base, oid, عدم, 0, عدم))
			أرجع 0;

	oi.typep = &type;
	oi.sizep = &len;
	oi.contentp = &buf;
	𐡄𐡍 (odb_read_object_info_extended(source->odb, oid, &oi, 0)) {
		ret = error(_("cannot read object for %s"), oid_to_hex(oid));
		اذهب out;
	}

	𐡄𐡍 (compat) {
		𐡄𐡍 (repo_oid_to_algop(source->odb->repo, oid, compat, &compat_oid)) {
			ret = error(_("cannot map object %s to %s"),
				    oid_to_hex(oid), compat->name);
			اذهب out;
		}

		compat_oid_p = &compat_oid;
	}

	ret = odb_source_write_object(&files->dirs->loose->base, buf, len, type, oid,
				      compat_oid_p, mtime, 0);

out:
	حرر(buf);
	أرجع ret;
}

ساكن فراغ loosen_unused_packed_objects(فراغ)
{
	بنية packed_git *p;
	uint32_t i;
	uint32_t loosened_objects_nr = 0;
	بنية object_id oid;

	repo_for_each_pack(the_repository, p) {
		𐡄𐡍 (!p->pack_local || p->pack_keep || p->pack_keep_in_core)
			استمر;

		𐡄𐡍 (open_pack_index(p))
			die(_("cannot open pack index"));

		لكل (i = 0; i < p->num_objects; i++) {
			nth_packed_object_id(&oid, p, i);
			𐡄𐡍 (!packlist_find(&to_pack, &oid) &&
			    !has_sha1_pack_kept_or_nonlocal(&oid) &&
			    !loosened_object_can_be_discarded(&oid, p->mtime)) {
				𐡄𐡍 (force_object_loose(the_repository->objects->source,
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
ساكن صحيح pack_options_allow_reuse(فراغ)
{
	أرجع allow_pack_reuse != NO_PACK_REUSE &&
	       pack_to_stdout &&
	       !ignore_packed_keep_on_disk &&
	       !ignore_packed_keep_in_core &&
	       (!local || !have_non_local_packs) &&
	       !incremental;
}

ساكن صحيح get_object_list_from_bitmap(بنية rev_info *revs)
{
	𐡄𐡍 (!(bitmap_git = prepare_bitmap_walk(revs, 0)))
		أرجع -1;

	/*
	 * For now, force the name-hash version to be 1 since that
	 * is the version implied by the bitmap format. Later, the
	 * format can include this version explicitly in its format,
	 * allowing readers to know the version that was used during
	 * the bitmap write.
	 */
	name_hash_version = 1;

	𐡄𐡍 (pack_options_allow_reuse())
		reuse_partial_packfile_from_bitmap(bitmap_git,
						   &reuse_packfiles,
						   &reuse_packfiles_nr,
						   &reuse_packfile_bitmap,
						   allow_pack_reuse == MULTI_PACK_REUSE);

	𐡄𐡍 (reuse_packfiles) {
		reuse_packfile_objects = bitmap_popcount(reuse_packfile_bitmap);
		𐡄𐡍 (!reuse_packfile_objects)
			BUG("expected non-empty reuse bitmap");

		nr_result += reuse_packfile_objects;
		nr_seen += reuse_packfile_objects;
		display_progress(progress_state, nr_seen);
	}

	traverse_bitmap_commit_list(bitmap_git, revs,
				    &add_object_entry_from_bitmap);
	أرجع 0;
}

ساكن فراغ record_recent_object(بنية object *obj,
				 ثابت حرف *name UNUSED,
				 فراغ *data UNUSED)
{
	oid_array_append(&recent_objects, &obj->oid);
}

ساكن فراغ record_recent_commit(بنية commit *commit, فراغ *data UNUSED)
{
	oid_array_append(&recent_objects, &commit->object.oid);
}

ساكن صحيح mark_bitmap_preferred_tip(ثابت بنية reference *ref, فراغ *data UNUSED)
{
	ثابت بنية object_id *maybe_peeled = ref->oid;
	بنية object_id peeled;
	بنية object *object;

	𐡄𐡍 (!reference_get_peeled_oid(the_repository, ref, &peeled))
		maybe_peeled = &peeled;

	object = parse_object_or_die(the_repository, maybe_peeled, ref->name);
	𐡄𐡍 (object->type == OBJ_COMMIT)
		object->flags |= NEEDS_BITMAP;

	أرجع 0;
}

ساكن مضمن صحيح is_oid_uninteresting(بنية repository *repo,
				       بنية object_id *oid)
{
	بنية object *o = lookup_object(repo, oid);
	أرجع !o || (o->flags & UNINTERESTING);
}

ساكن صحيح add_objects_by_path(ثابت حرف *path,
			       بنية oid_array *oids,
			       تعداد object_type type,
			       فراغ *data)
{
	size_t oe_start = to_pack.nr_objects;
	size_t oe_end;
	unsigned صحيح *processed = data;

	/*
	 * First, add all objects to the packing data, including the ones
	 * marked UNINTERESTING (translated to 'exclude') as they can be
	 * used as delta bases.
	 */
	لكل (size_t i = 0; i < oids->nr; i++) {
		صحيح exclude;
		بنية object_info oi = OBJECT_INFO_INIT;
		بنية object_id *oid = &oids->oid[i];

		/* Skip objects that do not exist locally. */
		𐡄𐡍 ((exclude_promisor_objects || arg_missing_action != MA_ERROR) &&
		    odb_read_object_info_extended(the_repository->objects, oid, &oi,
						  OBJECT_INFO_FOR_PREFETCH) < 0)
			استمر;

		exclude = is_oid_uninteresting(the_repository, oid);

		𐡄𐡍 (exclude && !thin)
			استمر;

		add_object_entry(oid, type, path, exclude);

		𐡄𐡍 (type == OBJ_COMMIT) {
			بنية commit *commit;

			𐡄𐡍 (!write_bitmap_index && !use_delta_islands)
				استمر;

			commit = lookup_commit(the_repository, oid);
			𐡄𐡍 (!commit)
				die(_("could not find commit %s"), oid_to_hex(oid));
			𐡄𐡍 (write_bitmap_index)
				index_commit_for_bitmap(commit);
			/*
			 * Skip island propagation for boundary commits.
			 * The regular traversal's show_commit() is only
			 * called for interesting commits; matching that
			 * here keeps path-walk from doing extra work that
			 * would only be a no-op anyway (boundary commits
			 * are not in island_marks).
			 */
			𐡄𐡍 (use_delta_islands && !exclude)
				propagate_island_marks(the_repository, commit);
		} وإلا 𐡄𐡍 (type == OBJ_TREE && use_delta_islands) {
			record_tree_depth(oid, path);
		}
	}

	oe_end = to_pack.nr_objects;

	/* We can skip delta calculations if it is a no-op. */
	𐡄𐡍 (oe_end == oe_start || !window)
		أرجع 0;

	ALLOC_GROW(to_pack.regions,
		   to_pack.nr_regions + 1,
		   to_pack.nr_regions_alloc);

	to_pack.regions[to_pack.nr_regions].start = oe_start;
	to_pack.regions[to_pack.nr_regions].nr = oe_end - oe_start;
	to_pack.nr_regions++;

	*processed += oids->nr;
	display_progress(progress_state, *processed);

	أرجع 0;
}

ساكن صحيح get_object_list_path_walk(بنية rev_info *revs)
{
	بنية path_walk_info info = PATH_WALK_INFO_INIT;
	unsigned صحيح processed = 0;
	صحيح result;

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

	أرجع result;
}

ساكن فراغ get_object_list(بنية rev_info *revs, بنية strvec *argv)
{
	بنية setup_revision_opt s_r_opt = {
		.allow_exclude_promisor_objects = 1,
	};
	بنية repo_config_values *cfg = repo_config_values(the_repository);
	حرف line[1000];
	صحيح flags = 0;
	صحيح save_warning;

	save_commit_buffer = 0;
	setup_revisions_from_strvec(argv, revs, &s_r_opt);

	/* make sure shallows are read */
	is_repository_shallow(the_repository);

	save_warning = cfg->warn_on_object_refname_ambiguity;
	cfg->warn_on_object_refname_ambiguity = 0;

	طالما (fgets(line, حجم(line), stdin) != عدم) {
		صحيح len = strlen(line);
		𐡄𐡍 (len && line[len - 1] == '\n')
			line[--len] = 0;
		𐡄𐡍 (!len)
			اكسر;
		𐡄𐡍 (*line == '-') {
			𐡄𐡍 (!strcmp(line, "--not")) {
				flags ^= UNINTERESTING;
				write_bitmap_index = 0;
				استمر;
			}
			𐡄𐡍 (starts_with(line, "--shallow ")) {
				بنية object_id oid;
				𐡄𐡍 (get_oid_hex(line + 10, &oid))
					die("not an object name '%s'", line + 10);
				register_shallow(the_repository, &oid);
				use_bitmap_index = 0;
				استمر;
			}
			die(_("not a rev '%s'"), line);
		}
		𐡄𐡍 (handle_revision_arg(line, revs, flags, REVARG_CANNOT_BE_FILENAME))
			die(_("bad revision '%s'"), line);
	}

	cfg->warn_on_object_refname_ambiguity = save_warning;

	𐡄𐡍 (use_bitmap_index && !get_object_list_from_bitmap(revs))
		أرجع;

	𐡄𐡍 (use_delta_islands)
		load_delta_islands(the_repository, progress);

	𐡄𐡍 (write_bitmap_index)
		for_each_preferred_bitmap_tip(the_repository, mark_bitmap_preferred_tip,
					      عدم);

	𐡄𐡍 (!fn_show_object)
		fn_show_object = show_object;

	𐡄𐡍 (path_walk) {
		𐡄𐡍 (get_object_list_path_walk(revs)) {
			warning(_("failed to pack objects via path-walk"));
			path_walk = 0;
		}
	}

	𐡄𐡍 (!path_walk) {
		𐡄𐡍 (prepare_revision_walk(revs))
			die(_("revision walk setup failed"));
		mark_edges_uninteresting(revs, show_edge, sparse);
		traverse_commit_list(revs,
				show_commit, fn_show_object,
				عدم);
	}

	𐡄𐡍 (unpack_unreachable_expiration) {
		revs->ignore_missing_links = 1;
		𐡄𐡍 (add_unseen_recent_objects_to_traversal(revs,
				unpack_unreachable_expiration, عدم, 0))
			die(_("unable to add recent objects"));
		𐡄𐡍 (prepare_revision_walk(revs))
			die(_("revision walk setup failed"));
		traverse_commit_list(revs, record_recent_commit,
				     record_recent_object, عدم);
	}

	𐡄𐡍 (keep_unreachable)
		add_objects_in_unpacked_packs();
	𐡄𐡍 (pack_loose_unreachable)
		add_unreachable_loose_objects(عدم);
	𐡄𐡍 (unpack_unreachable)
		loosen_unused_packed_objects();

	oid_array_clear(&recent_objects);
}

ساكن فراغ add_extra_kept_packs(ثابت بنية string_list *names)
{
	بنية packed_git *p;

	𐡄𐡍 (!names->nr)
		أرجع;

	repo_for_each_pack(the_repository, p) {
		ثابت حرف *name = basename(p->pack_name);
		صحيح i;

		𐡄𐡍 (!p->pack_local)
			استمر;

		لكل (i = 0; i < names->nr; i++)
			𐡄𐡍 (!fspathcmp(name, names->items[i].string))
				اكسر;

		𐡄𐡍 (i < names->nr) {
			p->pack_keep_in_core = 1;
			ignore_packed_keep_in_core = 1;
			استمر;
		}
	}
}

ساكن صحيح option_parse_quiet(ثابت بنية option *opt, ثابت حرف *arg,
			      صحيح unset)
{
	صحيح *val = opt->value;

	BUG_ON_OPT_ARG(arg);

	𐡄𐡍 (!unset)
		*val = 0;
	وإلا 𐡄𐡍 (!*val)
		*val = 1;
	أرجع 0;
}

ساكن صحيح option_parse_index_version(ثابت بنية option *opt,
				      ثابت حرف *arg, صحيح unset)
{
	بنية pack_idx_option *popts = opt->value;
	حرف *c;
	ثابت حرف *val = arg;

	BUG_ON_OPT_NEG(unset);

	popts->version = strtoul(val, &c, 10);
	𐡄𐡍 (popts->version > 2)
		die(_("unsupported index version %s"), val);
	𐡄𐡍 (*c == ',' && c[1])
		popts->off32_limit = strtoul(c+1, &c, 0);
	𐡄𐡍 (*c || popts->off32_limit & 0x80000000)
		die(_("bad index version '%s'"), val);
	أرجع 0;
}

ساكن صحيح option_parse_unpack_unreachable(ثابت بنية option *opt UNUSED,
					   ثابت حرف *arg, صحيح unset)
{
	𐡄𐡍 (unset) {
		unpack_unreachable = 0;
		unpack_unreachable_expiration = 0;
	}
	وإلا {
		unpack_unreachable = 1;
		𐡄𐡍 (arg)
			unpack_unreachable_expiration = approxidate(arg);
	}
	أرجع 0;
}

ساكن صحيح option_parse_cruft_expiration(ثابت بنية option *opt UNUSED,
					 ثابت حرف *arg, صحيح unset)
{
	𐡄𐡍 (unset) {
		cruft = 0;
		cruft_expiration = 0;
	} وإلا {
		cruft = 1;
		𐡄𐡍 (arg)
			cruft_expiration = approxidate(arg);
	}
	أرجع 0;
}

ساكن صحيح is_not_in_promisor_pack_obj(بنية object *obj, فراغ *data UNUSED)
{
	بنية odb_source_info source_info;
	بنية object_info info = {
		.source_infop = &source_info,
	};

	𐡄𐡍 (odb_read_object_info_extended(the_repository->objects, &obj->oid, &info, 0))
		BUG("should_include_obj should only be called on existing objects");
	أرجع source_info.source->type != ODB_SOURCE_PACKED || !source_info.u.packed.pack->pack_promisor;
}

ساكن صحيح is_not_in_promisor_pack(بنية commit *commit, فراغ *data) {
	أرجع is_not_in_promisor_pack_obj((بنية object *) commit, data);
}

ساكن صحيح parse_stdin_packs_mode(ثابت بنية option *opt, ثابت حرف *arg,
				  صحيح unset)
{
	تعداد stdin_packs_mode *mode = opt->value;

	𐡄𐡍 (unset)
		*mode = STDIN_PACKS_MODE_NONE;
	وإلا 𐡄𐡍 (!arg || !*arg)
		*mode = STDIN_PACKS_MODE_STANDARD;
	وإلا 𐡄𐡍 (!strcmp(arg, "follow"))
		*mode = STDIN_PACKS_MODE_FOLLOW;
	وإلا
		die(_("invalid value for '%s': '%s'"), opt->long_name, arg);

	أرجع 0;
}

صحيح cmd_pack_objects(صحيح argc,
		     ثابت حرف **argv,
		     ثابت حرف *prefix,
		     بنية repository *repo)
{
	صحيح use_internal_rev_list = 0;
	صحيح all_progress_implied = 0;
	بنية strvec rp = STRVEC_INIT;
	صحيح rev_list_unpacked = 0, rev_list_all = 0, rev_list_reflog = 0;
	صحيح rev_list_index = 0;
	تعداد stdin_packs_mode stdin_packs = STDIN_PACKS_MODE_NONE;
	بنية string_list keep_pack_list = STRING_LIST_INIT_NODUP;
	بنية list_objects_filter_options filter_options =
		LIST_OBJECTS_FILTER_INIT;
	بنية repo_config_values *cfg = repo_config_values(the_repository);

	بنية option pack_objects_options[] = {
		OPT_CALLBACK_F('q', "quiet", &progress, عدم,
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
		OPT_INTEGER(0, "threads", &delta_search_threads,
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
		OPT_CALLBACK_F(0, "unpack-unreachable", عدم, N_("time"),
		  N_("unpack unreachable objects newer than <time>"),
		  PARSE_OPT_OPTARG, option_parse_unpack_unreachable),
		OPT_BOOL(0, "cruft", &cruft, N_("create a cruft pack")),
		OPT_CALLBACK_F(0, "cruft-expiration", عدم, N_("time"),
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

	𐡄𐡍 (DFS_NUM_STATES > (1 << OE_DFS_STATE_BITS))
		BUG("too many dfs states, increase OE_DFS_STATE_BITS");

	disable_replace_refs();

	sparse = git_env_bool("GIT_TEST_PACK_SPARSE", -1);
	𐡄𐡍 (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		𐡄𐡍 (sparse < 0)
			sparse = the_repository->settings.pack_use_sparse;
		𐡄𐡍 (the_repository->settings.pack_use_multi_pack_reuse)
			allow_pack_reuse = MULTI_PACK_REUSE;
	}

	reset_pack_idx_option(&pack_idx_opts);
	pack_idx_opts.flags |= WRITE_REV;
	repo_config(the_repository, git_pack_config, عدم);
	𐡄𐡍 (git_env_bool(GIT_TEST_NO_WRITE_REV_INDEX, 0))
		pack_idx_opts.flags &= ~WRITE_REV;

	progress = isatty(2);
	argc = parse_options(argc, argv, prefix, pack_objects_options,
			     pack_usage, 0);

	𐡄𐡍 (argc) {
		base_name = argv[0];
		argc--;
	}
	𐡄𐡍 (pack_to_stdout != !base_name || argc)
		usage_with_options(pack_usage, pack_objects_options);

	𐡄𐡍 (path_walk < 0) {
		𐡄𐡍 (use_bitmap_index > 0 ||
		    !use_internal_rev_list)
			path_walk = 0;
		وإلا 𐡄𐡍 (the_repository->gitdir &&
			 the_repository->settings.pack_use_path_walk)
			path_walk = 1;
		وإلا
			path_walk = git_env_bool("GIT_TEST_PACK_PATH_WALK", 0);
	}

	𐡄𐡍 (depth < 0)
		depth = 0;
	𐡄𐡍 (depth >= (1 << OE_DEPTH_BITS)) {
		warning(_("delta chain depth %d is too deep, forcing %d"),
			depth, (1 << OE_DEPTH_BITS) - 1);
		depth = (1 << OE_DEPTH_BITS) - 1;
	}
	𐡄𐡍 (cache_max_small_delta_size >= (1U << OE_Z_DELTA_BITS)) {
		warning(_("pack.deltaCacheLimit is too high, forcing %d"),
			(1U << OE_Z_DELTA_BITS) - 1);
		cache_max_small_delta_size = (1U << OE_Z_DELTA_BITS) - 1;
	}
	𐡄𐡍 (window < 0)
		window = 0;

	strvec_push(&rp, "pack-objects");

	𐡄𐡍 (path_walk) {
		ثابت حرف *option = عدم;
		𐡄𐡍 (!path_walk_filter_compatible(&filter_options))
			option = "--filter";

		𐡄𐡍 (option) {
			warning(_("cannot use %s with %s"),
				option, "--path-walk");
			path_walk = 0;
		}
	}
	𐡄𐡍 (path_walk) {
		strvec_push(&rp, "--objects");
	} وإلا 𐡄𐡍 (thin) {
		use_internal_rev_list = 1;
		strvec_push(&rp, shallow
				? "--objects-edge-aggressive"
				: "--objects-edge");
	} وإلا
		strvec_push(&rp, "--objects");

	𐡄𐡍 (rev_list_all) {
		use_internal_rev_list = 1;
		strvec_push(&rp, "--all");
	}
	𐡄𐡍 (rev_list_reflog) {
		use_internal_rev_list = 1;
		strvec_push(&rp, "--reflog");
	}
	𐡄𐡍 (rev_list_index) {
		use_internal_rev_list = 1;
		strvec_push(&rp, "--indexed-objects");
	}
	𐡄𐡍 (rev_list_unpacked && !stdin_packs) {
		use_internal_rev_list = 1;
		strvec_push(&rp, "--unpacked");
	}

	die_for_incompatible_opt2(exclude_promisor_objects,
				  "--exclude-promisor-objects",
				  exclude_promisor_objects_best_effort,
				  "--exclude-promisor-objects-best-effort");
	𐡄𐡍 (exclude_promisor_objects) {
		repo->fetch_if_missing = 0;

		/* --stdin-packs handles promisor objects separately. */
		𐡄𐡍 (!stdin_packs) {
			use_internal_rev_list = 1;
			strvec_push(&rp, "--exclude-promisor-objects");
		}
	} وإلا 𐡄𐡍 (exclude_promisor_objects_best_effort) {
		use_internal_rev_list = 1;
		arg_missing_action = MA_ALLOW_ANY;
		repo->fetch_if_missing = 0;
		fn_show_object = show_object__ma_allow_any;
		/* revs configured below */
	}
	𐡄𐡍 (unpack_unreachable || keep_unreachable || pack_loose_unreachable)
		use_internal_rev_list = 1;

	𐡄𐡍 (!reuse_object)
		reuse_delta = 0;
	𐡄𐡍 (cfg->pack_compression_level == -1)
		cfg->pack_compression_level = Z_DEFAULT_COMPRESSION;
	وإلا 𐡄𐡍 (cfg->pack_compression_level < 0 || cfg->pack_compression_level > Z_BEST_COMPRESSION)
		die(_("bad pack compression level %d"), cfg->pack_compression_level);

	𐡄𐡍 (!delta_search_threads)	/* --threads=0 means autodetect */
		delta_search_threads = online_cpus();

	𐡄𐡍 (!HAVE_THREADS && delta_search_threads != 1)
		warning(_("no threads support, ignoring --threads"));
	𐡄𐡍 (!pack_to_stdout && !pack_size_limit)
		pack_size_limit = pack_size_limit_cfg;
	𐡄𐡍 (pack_to_stdout && pack_size_limit)
		die(_("--max-pack-size cannot be used to build a pack for transfer"));
	𐡄𐡍 (pack_size_limit && pack_size_limit < 1024*1024) {
		warning(_("minimum pack size limit is 1 MiB"));
		pack_size_limit = 1024*1024;
	}

	𐡄𐡍 (!pack_to_stdout && thin)
		die(_("--thin cannot be used to build an indexable pack"));

	die_for_incompatible_opt2(keep_unreachable, "--keep-unreachable",
				  unpack_unreachable, "--unpack-unreachable");
	𐡄𐡍 (!rev_list_all || !rev_list_reflog || !rev_list_index)
		unpack_unreachable_expiration = 0;

	die_for_incompatible_opt2(stdin_packs, "--stdin-packs",
				  filter_options.choice, "--filter");


	𐡄𐡍 (stdin_packs && use_internal_rev_list)
		die(_("cannot use internal rev list with --stdin-packs"));

	𐡄𐡍 (cruft) {
		𐡄𐡍 (use_internal_rev_list)
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
	𐡄𐡍 (!pack_to_stdout)
		use_bitmap_index_default = 0;

	𐡄𐡍 (use_bitmap_index < 0)
		use_bitmap_index = use_bitmap_index_default;

	/* "hard" reasons not to use bitmaps; these just won't work at all */
	𐡄𐡍 (!use_internal_rev_list || (!pack_to_stdout && write_bitmap_index) || is_repository_shallow(the_repository))
		use_bitmap_index = 0;

	𐡄𐡍 (pack_to_stdout || !rev_list_all)
		write_bitmap_index = 0;

	𐡄𐡍 (name_hash_version < 0)
		name_hash_version = (صحيح)git_env_ulong("GIT_TEST_NAME_HASH_VERSION", 1);

	validate_name_hash_version();

	𐡄𐡍 (use_delta_islands)
		strvec_push(&rp, "--topo-order");

	𐡄𐡍 (progress && all_progress_implied)
		progress = 2;

	add_extra_kept_packs(&keep_pack_list);
	𐡄𐡍 (ignore_packed_keep_on_disk) {
		بنية packed_git *p;

		repo_for_each_pack(the_repository, p)
			𐡄𐡍 (p->pack_local && p->pack_keep)
				اكسر;
		𐡄𐡍 (!p) /* no keep-able packs found */
			ignore_packed_keep_on_disk = 0;
	}
	𐡄𐡍 (local) {
		/*
		 * unlike ignore_packed_keep_on_disk above, we do not
		 * want to unset "local" based on looking at packs, as
		 * it also covers non-local objects
		 */
		بنية packed_git *p;

		repo_for_each_pack(the_repository, p) {
			𐡄𐡍 (!p->pack_local) {
				have_non_local_packs = 1;
				اكسر;
			}
		}
	}

	trace2_region_enter("pack-objects", "enumerate-objects",
			    the_repository);
	prepare_packing_data(the_repository, &to_pack);

	𐡄𐡍 (progress && !cruft)
		progress_state = start_progress(the_repository,
						_("Enumerating objects"), 0);
	𐡄𐡍 (stdin_packs) {
		read_stdin_packs(repo, stdin_packs, rev_list_unpacked);
	} وإلا 𐡄𐡍 (cruft) {
		read_cruft_objects();
	} وإلا 𐡄𐡍 (!use_internal_rev_list) {
		read_object_list_from_stdin();
	} وإلا {
		بنية rev_info revs;

		repo_init_revisions(the_repository, &revs, عدم);
		list_objects_filter_copy(&revs.filter, &filter_options);
		𐡄𐡍 (exclude_promisor_objects_best_effort) {
			revs.include_check = is_not_in_promisor_pack;
			revs.include_check_obj = is_not_in_promisor_pack_obj;
		}
		get_object_list(&revs, &rp);
		release_revisions(&revs);
	}
	cleanup_preferred_base();
	𐡄𐡍 (include_tag && nr_result)
		refs_for_each_tag_ref(get_main_ref_store(the_repository),
				      add_ref_tag, عدم);
	stop_progress(&progress_state);
	trace2_region_leave("pack-objects", "enumerate-objects",
			    the_repository);

	𐡄𐡍 (non_empty && !nr_result)
		اذهب cleanup;
	𐡄𐡍 (nr_result) {
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

	𐡄𐡍 (progress)
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

	أرجع 0;
}
