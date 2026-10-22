#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#شامل "builtin.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "hex.h"
#شامل "config.h"
#شامل "attr.h"
#شامل "object.h"
#شامل "commit.h"
#شامل "tag.h"
#شامل "delta.h"
#شامل "pack.h"
#شامل "pack-revindex.h"
#شامل "csum-file.h"
#شامل "tree-walk.h"
#شامل "diff.h"
#شامل "revision.h"
#شامل "list-objects.h"
#شامل "list-objects-filter-options.h"
#شامل "pack-objects.h"
#شامل "progress.h"
#شامل "refs.h"
#شامل "thread-utils.h"
#شامل "pack-bitmap.h"
#شامل "delta-islands.h"
#شامل "reachable.h"
#شامل "oid-array.h"
#شامل "strvec.h"
#شامل "strmap.h"
#شامل "list.h"
#شامل "packfile.h"
#شامل "object-file.h"
#شامل "object-file-convert.h"
#شامل "odb.h"
#شامل "odb/streaming.h"
#شامل "replace-object.h"
#شامل "dir.h"
#شامل "midx.h"
#شامل "trace2.h"
#شامل "shallow.h"
#شامل "promisor-remote.h"
#شامل "pack-mtimes.h"
#شامل "parse-options.h"
#شامل "pkt-line.h"
#شامل "blob.h"
#شامل "tree.h"
#شامل "path-walk.h"

/*
 * Objects we are going to pack are collected in the `to_pack` structure.
 * It contains an array (dynamically expanded) of the object data, and a map
 * that can resolve SHA1s to their position in the array.
 */
ایستا ساختار packing_data to_pack;

ایستا درونخطی ساختار object_entry *oe_delta(
		ثابت ساختار packing_data *pack,
		ثابت ساختار object_entry *e)
{
	اگر (!e->delta_idx)
		بازگشت پوچ;
	اگر (e->ext_base)
		بازگشت &pack->ext_bases[e->delta_idx - 1];
	وگرنه
		بازگشت &pack->objects[e->delta_idx - 1];
}

ایستا درونخطی size_t oe_delta_size(ساختار packing_data *pack,
				   ثابت ساختار object_entry *e)
{
	اگر (e->delta_size_valid)
		بازگشت e->delta_size_;

	/*
	 * pack->delta_size[] can't be NULL because oe_set_delta_size()
	 * must have been called when a new delta is saved with
	 * oe_set_delta().
	 * If oe_delta() returns NULL (i.e. default state, which means
	 * delta_size_valid is also false), then the caller must never
	 * call oe_delta_size().
	 */
	بازگشت pack->delta_size[e - pack->objects];
}

size_t oe_get_size_slow(ساختار packing_data *pack,
			ثابت ساختار object_entry *e);

ایستا درونخطی size_t oe_size(ساختار packing_data *pack,
			     ثابت ساختار object_entry *e)
{
	اگر (e->size_valid)
		بازگشت e->size_;

	بازگشت oe_get_size_slow(pack, e);
}

ایستا درونخطی تهی oe_set_delta(ساختار packing_data *pack,
				ساختار object_entry *e,
				ساختار object_entry *delta)
{
	اگر (delta)
		e->delta_idx = (delta - pack->objects) + 1;
	وگرنه
		e->delta_idx = 0;
}

ایستا درونخطی ساختار object_entry *oe_delta_sibling(
		ثابت ساختار packing_data *pack,
		ثابت ساختار object_entry *e)
{
	اگر (e->delta_sibling_idx)
		بازگشت &pack->objects[e->delta_sibling_idx - 1];
	بازگشت پوچ;
}

ایستا درونخطی ساختار object_entry *oe_delta_child(
		ثابت ساختار packing_data *pack,
		ثابت ساختار object_entry *e)
{
	اگر (e->delta_child_idx)
		بازگشت &pack->objects[e->delta_child_idx - 1];
	بازگشت پوچ;
}

ایستا درونخطی تهی oe_set_delta_child(ساختار packing_data *pack,
				      ساختار object_entry *e,
				      ساختار object_entry *delta)
{
	اگر (delta)
		e->delta_child_idx = (delta - pack->objects) + 1;
	وگرنه
		e->delta_child_idx = 0;
}

ایستا درونخطی تهی oe_set_delta_sibling(ساختار packing_data *pack,
					ساختار object_entry *e,
					ساختار object_entry *delta)
{
	اگر (delta)
		e->delta_sibling_idx = (delta - pack->objects) + 1;
	وگرنه
		e->delta_sibling_idx = 0;
}

ایستا درونخطی تهی oe_set_size(ساختار packing_data *pack,
			       ساختار object_entry *e,
			       size_t size)
{
	اگر (size < pack->oe_size_limit) {
		e->size_ = size;
		e->size_valid = 1;
	} وگرنه {
		e->size_valid = 0;
		اگر (oe_get_size_slow(pack, e) != size)
			BUG("'size' is supposed to be the object size!");
	}
}

ایستا درونخطی تهی oe_set_delta_size(ساختار packing_data *pack,
				     ساختار object_entry *e,
				     size_t size)
{
	اگر (size < pack->oe_delta_size_limit) {
		e->delta_size_ = size;
		e->delta_size_valid = 1;
	} وگرنه {
		packing_data_lock(pack);
		اگر (!pack->delta_size)
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

ایستا ثابت نویسه *ثابت pack_usage[] = {
	N_("git pack-objects [-q | --progress | --all-progress] [--all-progress-implied]\n"
	   "                 [--no-reuse-delta] [--delta-base-offset] [--non-empty]\n"
	   "                 [--local] [--incremental] [--window=<n>] [--depth=<n>]\n"
	   "                 [--revs [--unpacked | --all]] [--keep-pack=<pack-name>]\n"
	   "                 [--cruft] [--cruft-expiration=<time>]\n"
	   "                 [--stdout [--filter=<filter-spec>] | <base-name>]\n"
	   "                 [--shallow] [--keep-true-parents] [--[no-]sparse]\n"
	   "                 [--name-hash-version=<n>] [--path-walk] < <object-list>"),
	پوچ
};

ایستا ساختار pack_idx_entry **written_list;
ایستا uint32_t nr_result, nr_written, nr_seen;
ایستا ساختار bitmap_index *bitmap_git;
ایستا uint32_t write_layer;

ایستا صحیح non_empty;
ایستا صحیح reuse_delta = 1, reuse_object = 1;
ایستا صحیح keep_unreachable, unpack_unreachable, include_tag;
ایستا timestamp_t unpack_unreachable_expiration;
ایستا صحیح pack_loose_unreachable;
ایستا صحیح cruft;
ایستا صحیح shallow = 0;
ایستا timestamp_t cruft_expiration;
ایستا صحیح local;
ایستا صحیح have_non_local_packs;
ایستا صحیح incremental;
ایستا صحیح ignore_packed_keep_on_disk;
ایستا صحیح ignore_packed_keep_in_core;
ایستا صحیح ignore_packed_keep_in_core_open;
ایستا صحیح ignore_packed_keep_in_core_has_cruft;
ایستا صحیح allow_ofs_delta;
ایستا ساختار pack_idx_option pack_idx_opts;
ایستا ثابت نویسه *base_name;
ایستا صحیح progress = 1;
ایستا صحیح window = 10;
ایستا unsigned long pack_size_limit;
ایستا صحیح depth = 50;
ایستا صحیح delta_جستجو_threads;
ایستا صحیح pack_to_stdout;
ایستا صحیح sparse;
ایستا صحیح thin;
ایستا صحیح path_walk = -1;
ایستا صحیح رقم_preferred_base;
ایستا ساختار progress *progress_state;

ایستا ساختار bitmapped_pack *reuse_packfiles;
ایستا size_t reuse_packfiles_nr;
ایستا size_t reuse_packfiles_used_nr;
ایستا uint32_t reuse_packfile_objects;
ایستا ساختار bitmap *reuse_packfile_bitmap;

ایستا صحیح use_bitmap_index_default = 1;
ایستا صحیح use_bitmap_index = -1;
ایستا شمارش {
	NO_PACK_REUSE = 0,
	SINGLE_PACK_REUSE,
	MULTI_PACK_REUSE,
} allow_pack_reuse = SINGLE_PACK_REUSE;
ایستا شمارش {
	WRITE_BITMAP_FALSE = 0,
	WRITE_BITMAP_QUIET,
	WRITE_BITMAP_TRUE,
} write_bitmap_index;
ایستا uint16_t write_bitmap_options = BITMAP_OPT_HASH_CACHE;

ایستا صحیح exclude_promisor_objects;
ایستا صحیح exclude_promisor_objects_best_effort;

ایستا صحیح use_delta_islands;

ایستا size_t delta_cache_size = 0;
ایستا size_t max_delta_cache_size = DEFAULT_DELTA_CACHE_SIZE;
ایستا unsigned long cache_max_small_delta_size = 1000;

ایستا unsigned long window_memory_limit = 0;

ایستا ساختار string_list uri_protocols = STRING_LIST_INIT_NODUP;

شمارش missing_action {
	MA_ERROR = 0,      /* fail if any missing objects are encountered */
	MA_ALLOW_ANY,      /* silently allow ALL missing objects */
	MA_ALLOW_PROMISOR, /* silently allow all missing PROMISOR objects */
};
ایستا شمارش missing_action arg_missing_action;
ایستا show_object_fn fn_show_object;

ساختار configured_exclusion {
	ساختار oidmap_entry e;
	نویسه *pack_hash_hex;
	نویسه *uri;
};
ایستا ساختار oidmap configured_exclusions;

ایستا ساختار oidset excluded_by_config;
ایستا صحیح name_hash_version = -1;

شمارش stdin_packs_mode {
	STDIN_PACKS_MODE_NONE,
	STDIN_PACKS_MODE_STANDARD,
	STDIN_PACKS_MODE_FOLLOW,
};

/**
 * Check whether the name_hash_version chosen by user input is appropriate,
 * and also validate whether it is compatible with other features.
 */
ایستا تهی validate_name_hash_version(تهی)
{
	اگر (name_hash_version < 1 || name_hash_version > 2)
		die(_("invalid --name-hash-version option: %d"), name_hash_version);
	اگر (write_bitmap_index && name_hash_version != 1) {
		warning(_("currently, --write-bitmap-index requires --name-hash-version=1"));
		name_hash_version = 1;
	}
}

ایستا درونخطی uint32_t pack_name_hash_fn(ثابت نویسه *name)
{
	ایستا صحیح seen_version = -1;

	اگر (seen_version < 0)
		seen_version = name_hash_version;
	وگرنه اگر (seen_version != name_hash_version)
		BUG("name hash version changed from %d to %d mid-process",
		    seen_version, name_hash_version);

	انتخاب (name_hash_version) {
	حالت 1:
		بازگشت pack_name_hash(name);

	حالت 2:
		بازگشت pack_name_hash_v2((ثابت unsigned نویسه *)name);

	پیشفرض:
		BUG("invalid name-hash version: %d", name_hash_version);
	}
}

/*
 * stats
 */
ایستا uint32_t written, written_delta;
ایستا uint32_t reused, reused_delta;

/*
 * Indexed commits
 */
ایستا ساختار commit **indexed_commits;
ایستا unsigned صحیح indexed_commits_nr;
ایستا unsigned صحیح indexed_commits_alloc;

ایستا تهی index_commit_for_bitmap(ساختار commit *commit)
{
	اگر (indexed_commits_nr >= indexed_commits_alloc) {
		indexed_commits_alloc = (indexed_commits_alloc + 32) * 2;
		REALLOC_ARRAY(indexed_commits, indexed_commits_alloc);
	}

	indexed_commits[indexed_commits_nr++] = commit;
}

ایستا تهی *get_delta(ساختار object_entry *entry)
{
	unsigned long size, base_size;
	size_t delta_size;
	تهی *buf, *base_buf, *delta_buf;
	شمارش object_type type;
	size_t size_st = 0, base_size_st = 0;

	buf = odb_read_object(the_repository->objects, &entry->idx.oid,
			      &type, &size_st);
	size = cast_size_t_to_ulong(size_st);
	اگر (!buf)
		die(_("unable to read %s"), oid_to_hex(&entry->idx.oid));
	base_buf = odb_read_object(the_repository->objects,
				   &DELTA(entry)->idx.oid, &type,
				   &base_size_st);
	base_size = cast_size_t_to_ulong(base_size_st);
	اگر (!base_buf)
		die("unable to read %s",
		    oid_to_hex(&DELTA(entry)->idx.oid));
	delta_buf = diff_delta(base_buf, base_size,
			       buf, size, &delta_size, 0);
	/*
	 * We successfully computed this delta once but dropped it for
	 * memory reasons. Something is very wrong if this time we
	 * recompute and create a different delta.
	 */
	اگر (!delta_buf || delta_size != DELTA_SIZE(entry))
		BUG("delta size changed");
	آزاد(buf);
	آزاد(base_buf);
	بازگشت delta_buf;
}

ایستا unsigned long do_compress(تهی **pptr, unsigned long size)
{
	git_zstream stream;
	تهی *in, *out;
	unsigned long maxsize;
	ساختار repo_config_مقادیر *cfg = repo_config_مقادیر(the_repository);

	git_deflate_init(&stream, cfg->pack_compression_level);
	maxsize = git_deflate_bound(&stream, size);

	in = *pptr;
	out = xmalloc(maxsize);
	*pptr = out;

	stream.next_in = in;
	stream.avail_in = size;
	stream.next_out = out;
	stream.avail_out = maxsize;
	وقتی (git_deflate(&stream, Z_FINISH) == Z_OK)
		; /* nothing */
	git_deflate_end(&stream);

	آزاد(in);
	بازگشت stream.total_out;
}

ایستا unsigned long write_large_blob_data(ساختار odb_stream *st, ساختار hashfile *f,
					   ثابت ساختار object_id *oid)
{
	git_zstream stream;
	unsigned نویسه ibuf[1024 * 16];
	unsigned نویسه obuf[1024 * 16];
	unsigned long olen = 0;
	ساختار repo_config_مقادیر *cfg = repo_config_مقادیر(the_repository);

	git_deflate_init(&stream, cfg->pack_compression_level);

	برای (;;) {
		ssize_t readlen;
		صحیح zret = Z_OK;
		readlen = odb_stream_read(st, ibuf, اندازه(ibuf));
		اگر (readlen == -1)
			die(_("unable to read %s"), oid_to_hex(oid));

		stream.next_in = ibuf;
		stream.avail_in = readlen;
		وقتی ((stream.avail_in || readlen == 0) &&
		       (zret == Z_OK || zret == Z_BUF_ERROR)) {
			stream.next_out = obuf;
			stream.avail_out = اندازه(obuf);
			zret = git_deflate(&stream, readlen ? 0 : Z_FINISH);
			hashwrite(f, obuf, stream.next_out - obuf);
			olen += stream.next_out - obuf;
		}
		اگر (stream.avail_in)
			die(_("deflate error (%d)"), zret);
		اگر (readlen == 0) {
			اگر (zret != Z_STREAM_END)
				die(_("deflate error (%d)"), zret);
			بشکن;
		}
	}
	git_deflate_end(&stream);
	بازگشت olen;
}

/*
 * we are going to reuse the existing object data as is.  make
 * sure it is not corrupt.
 */
ایستا صحیح check_pack_inflate(ساختار packed_git *p,
		ساختار pack_window **w_curs,
		off_t offset,
		off_t len,
		size_t expect)
{
	git_zstream stream;
	unsigned نویسه fakebuf[4096], *in;
	صحیح st;

	memset(&stream, 0, اندازه(stream));
	git_inflate_init(&stream);
	انجامبده {
		in = use_pack(p, w_curs, offset, &stream.avail_in);
		stream.next_in = in;
		stream.next_out = fakebuf;
		stream.avail_out = اندازه(fakebuf);
		st = git_inflate(&stream, Z_FINISH);
		offset += stream.next_in - in;
	} وقتی (st == Z_OK || st == Z_BUF_ERROR);
	git_inflate_end(&stream);
	بازگشت (st == Z_STREAM_END &&
		stream.total_out == expect &&
		stream.total_in == len) ? 0 : -1;
}

ایستا تهی copy_pack_data(ساختار hashfile *f,
		ساختار packed_git *p,
		ساختار pack_window **w_curs,
		off_t offset,
		off_t len)
{
	unsigned نویسه *in;
	size_t avail;

	وقتی (len) {
		in = use_pack(p, w_curs, offset, &avail);
		اگر (avail > len)
			avail = (unsigned long)len;
		hashwrite(f, in, avail);
		offset += avail;
		len -= avail;
	}
}

ایستا درونخطی صحیح oe_size_greater_than(ساختار packing_data *pack,
				       ثابت ساختار object_entry *lhs,
				       size_t rhs)
{
	اگر (lhs->size_valid)
		بازگشت lhs->size_ > rhs;
	اگر (rhs < pack->oe_size_limit) /* rhs < 2^x <= lhs ? */
		بازگشت 1;
	بازگشت oe_get_size_slow(pack, lhs) > rhs;
}

/* Return 0 if we will bust the pack-size limit */
ایستا unsigned long write_no_reuse_object(ساختار hashfile *f, ساختار object_entry *entry,
					   unsigned long limit, صحیح usable_delta)
{
	unsigned long size, datalen;
	unsigned نویسه header[MAX_PACK_OBJECT_HEADER],
		      dheader[MAX_PACK_OBJECT_HEADER];
	unsigned hdrlen;
	شمارش object_type type;
	تهی *buf;
	ساختار odb_stream *st = پوچ;
	ثابت unsigned hashsz = the_hash_algo->rawsz;

	اگر (!usable_delta) {
		اگر (oe_type(entry) == OBJ_BLOB &&
		    oe_size_greater_than(&to_pack, entry,
					 repo_settings_get_big_file_threshold(the_repository)) &&
		    (st = odb_stream_from_object(the_repository->objects, &entry->idx.oid,
						 پوچ)) != پوچ) {
			buf = پوچ;
			type = st->type;
			size = st->size;
		} وگرنه {
			size_t size_st = 0;
			buf = odb_read_object(the_repository->objects,
					      &entry->idx.oid, &type,
					      &size_st);
			size = cast_size_t_to_ulong(size_st);
			اگر (!buf)
				die(_("unable to read %s"),
				    oid_to_hex(&entry->idx.oid));
		}
		/*
		 * make sure no cached delta data remains from a
		 * previous attempt before a pack split occurred.
		 */
		FREE_AND_NULL(entry->delta_data);
		entry->z_delta_size = 0;
	} وگرنه اگر (entry->delta_data) {
		size = DELTA_SIZE(entry);
		buf = entry->delta_data;
		entry->delta_data = پوچ;
		type = (allow_ofs_delta && DELTA(entry)->idx.offset) ?
			OBJ_OFS_DELTA : OBJ_REF_DELTA;
	} وگرنه {
		buf = get_delta(entry);
		size = DELTA_SIZE(entry);
		type = (allow_ofs_delta && DELTA(entry)->idx.offset) ?
			OBJ_OFS_DELTA : OBJ_REF_DELTA;
	}

	اگر (st)	/* large blob case, just assume we don't compress well */
		datalen = size;
	وگرنه اگر (entry->z_delta_size)
		datalen = entry->z_delta_size;
	وگرنه
		datalen = do_compress(&buf, size);

	/*
	 * The object header is a byte of 'type' followed by zero or
	 * more bytes of length.
	 */
	hdrlen = encode_in_pack_object_header(header, اندازه(header),
					      type, size);

	اگر (type == OBJ_OFS_DELTA) {
		/*
		 * Deltas with relative base contain an additional
		 * encoding of the relative offset for the delta
		 * base from this object's position in the pack.
		 */
		off_t ofs = entry->idx.offset - DELTA(entry)->idx.offset;
		unsigned pos = اندازه(dheader) - 1;
		dheader[pos] = ofs & 127;
		وقتی (ofs >>= 7)
			dheader[--pos] = 128 | (--ofs & 127);
		اگر (limit && hdrlen + اندازه(dheader) - pos + datalen + hashsz >= limit) {
			اگر (st)
				odb_stream_close(st);
			آزاد(buf);
			بازگشت 0;
		}
		hashwrite(f, header, hdrlen);
		hashwrite(f, dheader + pos, اندازه(dheader) - pos);
		hdrlen += اندازه(dheader) - pos;
	} وگرنه اگر (type == OBJ_REF_DELTA) {
		/*
		 * Deltas with a base reference contain
		 * additional bytes for the base object ID.
		 */
		اگر (limit && hdrlen + hashsz + datalen + hashsz >= limit) {
			اگر (st)
				odb_stream_close(st);
			آزاد(buf);
			بازگشت 0;
		}
		hashwrite(f, header, hdrlen);
		hashwrite(f, DELTA(entry)->idx.oid.hash, hashsz);
		hdrlen += hashsz;
	} وگرنه {
		اگر (limit && hdrlen + datalen + hashsz >= limit) {
			اگر (st)
				odb_stream_close(st);
			آزاد(buf);
			بازگشت 0;
		}
		hashwrite(f, header, hdrlen);
	}
	اگر (st) {
		datalen = write_large_blob_data(st, f, &entry->idx.oid);
		odb_stream_close(st);
	} وگرنه {
		hashwrite(f, buf, datalen);
		آزاد(buf);
	}

	بازگشت hdrlen + datalen;
}

/* Return 0 if we will bust the pack-size limit */
ایستا off_t write_reuse_object(ساختار hashfile *f, ساختار object_entry *entry,
				unsigned long limit, صحیح usable_delta)
{
	ساختار packed_git *p = IN_PACK(entry);
	ساختار pack_window *w_curs = پوچ;
	uint32_t pos;
	off_t offset, cur;
	شمارش object_type type = oe_type(entry);
	شمارش object_type in_pack_type;
	off_t datalen;
	unsigned نویسه header[MAX_PACK_OBJECT_HEADER],
		      dheader[MAX_PACK_OBJECT_HEADER];
	unsigned hdrlen;
	ثابت unsigned hashsz = the_hash_algo->rawsz;
	size_t entry_size;

	cur = entry->in_pack_offset;
	in_pack_type = unpack_object_header(p, &w_curs, &cur, &entry_size);
	اگر (in_pack_type < 0)
		die(_("write_reuse_object: unable to parse object header of %s"),
		    oid_to_hex(&entry->idx.oid));

	اگر (DELTA(entry))
		type = (allow_ofs_delta && DELTA(entry)->idx.offset) ?
			OBJ_OFS_DELTA : OBJ_REF_DELTA;
	hdrlen = encode_in_pack_object_header(header, اندازه(header),
					      type, entry_size);

	offset = entry->in_pack_offset;
	اگر (offset_to_pack_pos(p, offset, &pos) < 0)
		die(_("write_reuse_object: could not locate %s, expected at "
		      "offset %"PRIuMAX" in pack %s"),
		    oid_to_hex(&entry->idx.oid), (uintmax_t)offset,
		    p->pack_name);
	datalen = pack_pos_to_offset(p, pos + 1) - offset;
	اگر (!pack_to_stdout && p->index_version > 1 &&
	    check_pack_crc(p, &w_curs, offset, datalen,
			   pack_pos_to_index(p, pos))) {
		error(_("bad packed object CRC for %s"),
		      oid_to_hex(&entry->idx.oid));
		unuse_pack(&w_curs);
		بازگشت write_no_reuse_object(f, entry, limit, usable_delta);
	}

	offset += entry->in_pack_header_size;
	datalen -= entry->in_pack_header_size;

	اگر (!pack_to_stdout && p->index_version == 1 &&
	    check_pack_inflate(p, &w_curs, offset, datalen, entry_size)) {
		error(_("corrupt packed object for %s"),
		      oid_to_hex(&entry->idx.oid));
		unuse_pack(&w_curs);
		بازگشت write_no_reuse_object(f, entry, limit, usable_delta);
	}

	اگر (type == OBJ_OFS_DELTA) {
		off_t ofs = entry->idx.offset - DELTA(entry)->idx.offset;
		unsigned pos = اندازه(dheader) - 1;
		dheader[pos] = ofs & 127;
		وقتی (ofs >>= 7)
			dheader[--pos] = 128 | (--ofs & 127);
		اگر (limit && hdrlen + اندازه(dheader) - pos + datalen + hashsz >= limit) {
			unuse_pack(&w_curs);
			بازگشت 0;
		}
		hashwrite(f, header, hdrlen);
		hashwrite(f, dheader + pos, اندازه(dheader) - pos);
		hdrlen += اندازه(dheader) - pos;
		reused_delta++;
	} وگرنه اگر (type == OBJ_REF_DELTA) {
		اگر (limit && hdrlen + hashsz + datalen + hashsz >= limit) {
			unuse_pack(&w_curs);
			بازگشت 0;
		}
		hashwrite(f, header, hdrlen);
		hashwrite(f, DELTA(entry)->idx.oid.hash, hashsz);
		hdrlen += hashsz;
		reused_delta++;
	} وگرنه {
		اگر (limit && hdrlen + datalen + hashsz >= limit) {
			unuse_pack(&w_curs);
			بازگشت 0;
		}
		hashwrite(f, header, hdrlen);
	}
	copy_pack_data(f, p, &w_curs, offset, datalen);
	unuse_pack(&w_curs);
	reused++;
	بازگشت hdrlen + datalen;
}

/* Return 0 if we will bust the pack-size limit */
ایستا off_t write_object(ساختار hashfile *f,
			  ساختار object_entry *entry,
			  off_t write_offset)
{
	unsigned long limit;
	off_t len;
	صحیح usable_delta, to_reuse;

	اگر (!pack_to_stdout)
		crc32_begin(f);

	/* apply size limit if limited packsize and not first object */
	اگر (!pack_size_limit || !nr_written)
		limit = 0;
	وگرنه اگر (pack_size_limit <= write_offset)
		/*
		 * the earlier object did not fit the limit; avoid
		 * mistaking this with unlimited (i.e. limit = 0).
		 */
		limit = 1;
	وگرنه
		limit = pack_size_limit - write_offset;

	اگر (!DELTA(entry))
		usable_delta = 0;	/* no delta */
	وگرنه اگر (!pack_size_limit)
	       usable_delta = 1;	/* unlimited packfile */
	وگرنه اگر (DELTA(entry)->idx.offset == (off_t)-1)
		usable_delta = 0;	/* base was written to another pack */
	وگرنه اگر (DELTA(entry)->idx.offset)
		usable_delta = 1;	/* base already exists in this pack */
	وگرنه
		usable_delta = 0;	/* base could end up in another pack */

	اگر (!reuse_object)
		to_reuse = 0;	/* explicit */
	وگرنه اگر (!IN_PACK(entry))
		to_reuse = 0;	/* can't reuse what we don't have */
	وگرنه اگر (oe_type(entry) == OBJ_REF_DELTA ||
		 oe_type(entry) == OBJ_OFS_DELTA)
				/* check_object() decided it for us ... */
		to_reuse = usable_delta;
				/* ... but pack split may override that */
	وگرنه اگر (oe_type(entry) != entry->in_pack_type)
		to_reuse = 0;	/* pack has delta which is unusable */
	وگرنه اگر (DELTA(entry))
		to_reuse = 0;	/* we want to pack afresh */
	وگرنه
		to_reuse = 1;	/* we have it in-pack undeltified,
				 * and we do not need to deltify it.
				 */

	اگر (!to_reuse)
		len = write_no_reuse_object(f, entry, limit, usable_delta);
	وگرنه
		len = write_reuse_object(f, entry, limit, usable_delta);
	اگر (!len)
		بازگشت 0;

	اگر (usable_delta)
		written_delta++;
	written++;
	اگر (!pack_to_stdout)
		entry->idx.crc32 = crc32_end(f);
	بازگشت len;
}

شمارش write_one_status {
	WRITE_ONE_SKIP = -1, /* already written */
	WRITE_ONE_BREAK = 0, /* writing this will bust the limit; not written */
	WRITE_ONE_WRITTEN = 1, /* normal */
	WRITE_ONE_RECURSIVE = 2 /* already scheduled to be written */
};

ایستا شمارش write_one_status write_one(ساختار hashfile *f,
				       ساختار object_entry *e,
				       off_t *offset)
{
	off_t size;
	صحیح recursing;

	/*
	 * we set offset to 1 (which is an impossible value) to mark
	 * the fact that this object is involved in "write its base
	 * first before writing a deltified object" recursion.
	 */
	recursing = (e->idx.offset == 1);
	اگر (recursing) {
		warning(_("recursive delta detected for object %s"),
			oid_to_hex(&e->idx.oid));
		بازگشت WRITE_ONE_RECURSIVE;
	} وگرنه اگر (e->idx.offset || e->preferred_base) {
		/* offset is non zero if object is written already. */
		بازگشت WRITE_ONE_SKIP;
	}

	/* if we are deltified, write out base object first. */
	اگر (DELTA(e)) {
		e->idx.offset = 1; /* now recurse */
		انتخاب (write_one(f, DELTA(e), offset)) {
		حالت WRITE_ONE_RECURSIVE:
			/* we cannot depend on this one */
			SET_DELTA(e, پوچ);
			بشکن;
		پیشفرض:
			بشکن;
		حالت WRITE_ONE_BREAK:
			e->idx.offset = recursing;
			بازگشت WRITE_ONE_BREAK;
		}
	}

	e->idx.offset = *offset;
	size = write_object(f, e, *offset);
	اگر (!size) {
		e->idx.offset = recursing;
		بازگشت WRITE_ONE_BREAK;
	}
	written_list[nr_written++] = &e->idx;

	/* make sure off_t is sufficiently large not to wrap */
	اگر (signed_add_overflows(*offset, size))
		die(_("pack too large for current definition of off_t"));
	*offset += size;
	بازگشت WRITE_ONE_WRITTEN;
}

ایستا صحیح mark_tagged(ثابت ساختار reference *ref, تهی *cb_data UNUSED)
{
	ساختار object_id peeled;
	ساختار object_entry *entry = packlist_find(&to_pack, ref->oid);

	اگر (entry)
		entry->tagged = 1;
	اگر (!reference_get_peeled_oid(the_repository, ref, &peeled)) {
		entry = packlist_find(&to_pack, &peeled);
		اگر (entry)
			entry->tagged = 1;
	}
	بازگشت 0;
}

ایستا درونخطی unsigned نویسه oe_layer(ساختار packing_data *pack,
				     ساختار object_entry *e)
{
	اگر (!pack->layer)
		بازگشت 0;
	بازگشت pack->layer[e - pack->objects];
}

ایستا درونخطی تهی add_to_write_order(ساختار object_entry **wo,
			       unsigned صحیح *endp,
			       ساختار object_entry *e)
{
	اگر (e->filled || oe_layer(&to_pack, e) != write_layer)
		بازگشت;
	wo[(*endp)++] = e;
	e->filled = 1;
}

ایستا تهی add_descendants_to_write_order(ساختار object_entry **wo,
					   unsigned صحیح *endp,
					   ساختار object_entry *e)
{
	صحیح add_to_order = 1;
	وقتی (e) {
		اگر (add_to_order) {
			ساختار object_entry *s;
			/* add this node... */
			add_to_write_order(wo, endp, e);
			/* all its siblings... */
			برای (s = DELTA_SIBLING(e); s; s = DELTA_SIBLING(s)) {
				add_to_write_order(wo, endp, s);
			}
		}
		/* drop down a level to add left subtree nodes if possible */
		اگر (DELTA_CHILD(e)) {
			add_to_order = 1;
			e = DELTA_CHILD(e);
		} وگرنه {
			add_to_order = 0;
			/* our sibling might have some children, it is next */
			اگر (DELTA_SIBLING(e)) {
				e = DELTA_SIBLING(e);
				ادامه;
			}
			/* go back to our parent node */
			e = DELTA(e);
			وقتی (e && !DELTA_SIBLING(e)) {
				/* we're on the right side of a subtree, keep
				 * going up until we can go right again */
				e = DELTA(e);
			}
			اگر (!e) {
				/* done- we hit our original root node */
				بازگشت;
			}
			/* pass it off to sibling at this level */
			e = DELTA_SIBLING(e);
		}
	};
}

ایستا تهی add_family_to_write_order(ساختار object_entry **wo,
				      unsigned صحیح *endp,
				      ساختار object_entry *e)
{
	ساختار object_entry *ریشه;

	برای (ریشه = e; DELTA(ریشه); ریشه = DELTA(ریشه))
		; /* nothing */
	add_descendants_to_write_order(wo, endp, ریشه);
}

ایستا تهی compute_layer_order(ساختار object_entry **wo, unsigned صحیح *wo_end)
{
	unsigned صحیح i, last_untagged;
	ساختار object_entry *objects = to_pack.objects;

	برای (i = 0; i < to_pack.nr_objects; i++) {
		اگر (objects[i].tagged)
			بشکن;
		add_to_write_order(wo, wo_end, &objects[i]);
	}
	last_untagged = i;

	/*
	 * Then fill all the tagged tips.
	 */
	برای (; i < to_pack.nr_objects; i++) {
		اگر (objects[i].tagged)
			add_to_write_order(wo, wo_end, &objects[i]);
	}

	/*
	 * And then all remaining commits and tags.
	 */
	برای (i = last_untagged; i < to_pack.nr_objects; i++) {
		اگر (oe_type(&objects[i]) != OBJ_COMMIT &&
		    oe_type(&objects[i]) != OBJ_TAG)
			ادامه;
		add_to_write_order(wo, wo_end, &objects[i]);
	}

	/*
	 * And then all the trees.
	 */
	برای (i = last_untagged; i < to_pack.nr_objects; i++) {
		اگر (oe_type(&objects[i]) != OBJ_درخت)
			ادامه;
		add_to_write_order(wo, wo_end, &objects[i]);
	}

	/*
	 * Finally all the rest in really tight order
	 */
	برای (i = last_untagged; i < to_pack.nr_objects; i++) {
		اگر (!objects[i].filled && oe_layer(&to_pack, &objects[i]) == write_layer)
			add_family_to_write_order(wo, wo_end, &objects[i]);
	}
}

ایستا ساختار object_entry **compute_write_order(تهی)
{
	uint32_t max_layers = 1;
	unsigned صحیح i, wo_end;

	ساختار object_entry **wo;
	ساختار object_entry *objects = to_pack.objects;

	برای (i = 0; i < to_pack.nr_objects; i++) {
		objects[i].tagged = 0;
		objects[i].filled = 0;
		SET_DELTA_CHILD(&objects[i], پوچ);
		SET_DELTA_SIBLING(&objects[i], پوچ);
	}

	/*
	 * Fully connect delta_child/delta_sibling network.
	 * Make sure delta_sibling is sorted in the original
	 * recency order.
	 */
	برای (i = to_pack.nr_objects; i > 0;) {
		ساختار object_entry *e = &objects[--i];
		اگر (!DELTA(e))
			ادامه;
		/* Mark me as the first child */
		e->delta_sibling_idx = DELTA(e)->delta_child_idx;
		SET_DELTA_CHILD(DELTA(e), e);
	}

	/*
	 * Mark objects that are at the tip of tags.
	 */
	refs_for_each_tag_ref(get_main_ref_store(the_repository), mark_tagged,
			      پوچ);

	اگر (use_delta_islands) {
		max_layers = compute_pack_layers(&to_pack);
		free_island_marks();
	}

	ALLOC_ARRAY(wo, to_pack.nr_objects);
	wo_end = 0;

	برای (; write_layer < max_layers; ++write_layer)
		compute_layer_order(wo, &wo_end);

	اگر (wo_end != to_pack.nr_objects)
		die(_("ordered %u objects, expected %"PRIu32),
		    wo_end, to_pack.nr_objects);

	بازگشت wo;
}


/*
 * A reused set of objects. All objects in a chunk have the same
 * relative position in the original packfile and the generated
 * packfile.
 */

ایستا ساختار reused_chunk {
	/* The offset of the first object of this chunk in the original
	 * packfile. */
	off_t original;
	/* The difference for "original" minus the offset of the first object of
	 * this chunk in the generated packfile. */
	off_t difference;
} *reused_chunks;
ایستا صحیح reused_chunks_nr;
ایستا صحیح reused_chunks_alloc;

ایستا تهی record_reused_object(off_t where, off_t offset)
{
	اگر (reused_chunks_nr && reused_chunks[reused_chunks_nr-1].difference == offset)
		بازگشت;

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
ایستا off_t find_reused_offset(off_t where)
{
	صحیح lo = 0, hi = reused_chunks_nr;
	وقتی (lo < hi) {
		صحیح mi = lo + ((hi - lo) / 2);
		اگر (where == reused_chunks[mi].original)
			بازگشت reused_chunks[mi].difference;
		اگر (where < reused_chunks[mi].original)
			hi = mi;
		وگرنه
			lo = mi + 1;
	}

	/*
	 * The first chunk starts at zero, so we can't have gone below
	 * there.
	 */
	أكد(lo);
	بازگشت reused_chunks[lo-1].difference;
}

ایستا تهی write_reused_pack_one(ساختار packed_git *reuse_packfile,
				  size_t pos, ساختار hashfile *out,
				  off_t pack_start,
				  ساختار pack_window **w_curs)
{
	off_t offset, next, cur;
	شمارش object_type type;
	size_t size;

	offset = pack_pos_to_offset(reuse_packfile, pos);
	next = pack_pos_to_offset(reuse_packfile, pos + 1);

	record_reused_object(offset,
			     offset - (hashfile_total(out) - pack_start));

	cur = offset;
	type = unpack_object_header(reuse_packfile, w_curs, &cur, &size);
	أكد(type >= 0);

	اگر (type == OBJ_OFS_DELTA) {
		off_t base_offset;
		off_t fixup;

		unsigned نویسه header[MAX_PACK_OBJECT_HEADER];
		unsigned len;

		base_offset = get_delta_base(reuse_packfile, w_curs, &cur, type, offset);
		أكد(base_offset != 0);

		/* Convert to REF_DELTA if we must... */
		اگر (!allow_ofs_delta) {
			uint32_t base_pos;
			ساختار object_id base_oid;

			اگر (offset_to_pack_pos(reuse_packfile, base_offset, &base_pos) < 0)
				die(_("expected object at offset %"PRIuMAX" "
				      "in pack %s"),
				    (uintmax_t)base_offset,
				    reuse_packfile->pack_name);

			nth_packed_object_id(&base_oid, reuse_packfile,
					     pack_pos_to_index(reuse_packfile, base_pos));

			len = encode_in_pack_object_header(header, اندازه(header),
							   OBJ_REF_DELTA, size);
			hashwrite(out, header, len);
			hashwrite(out, base_oid.hash, the_hash_algo->rawsz);
			copy_pack_data(out, reuse_packfile, w_curs, cur, next - cur);
			بازگشت;
		}

		/* Otherwise see if we need to rewrite the offset... */
		fixup = find_reused_offset(offset) -
			find_reused_offset(base_offset);
		اگر (fixup) {
			unsigned نویسه ofs_header[MAX_PACK_OBJECT_HEADER];
			unsigned i, ofs_len;
			off_t ofs = offset - base_offset - fixup;

			len = encode_in_pack_object_header(header, اندازه(header),
							   OBJ_OFS_DELTA, size);

			i = اندازه(ofs_header) - 1;
			ofs_header[i] = ofs & 127;
			وقتی (ofs >>= 7)
				ofs_header[--i] = 128 | (--ofs & 127);

			ofs_len = اندازه(ofs_header) - i;

			hashwrite(out, header, len);
			hashwrite(out, ofs_header + اندازه(ofs_header) - ofs_len, ofs_len);
			copy_pack_data(out, reuse_packfile, w_curs, cur, next - cur);
			بازگشت;
		}

		/* ...otherwise we have no fixup, and can write it verbatim */
	}

	copy_pack_data(out, reuse_packfile, w_curs, offset, next - offset);
}

ایستا size_t write_reused_pack_verbatim(ساختار bitmapped_pack *reuse_packfile,
					 ساختار hashfile *out,
					 ساختار pack_window **w_curs)
{
	size_t pos = 0;
	size_t end;

	اگر (reuse_packfile->bitmap_pos) {
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
		بازگشت reuse_packfile->bitmap_pos / BITS_IN_EWORD;
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
	اگر (reuse_packfile_bitmap->word_alloc < end)
		BUG("fewer words than expected in reuse_packfile_bitmap");

	وقتی (pos < end && reuse_packfile_bitmap->واژگان[pos] == (eword_t)~0)
		pos++;

	اگر (pos) {
		off_t to_write;

		written = (pos * BITS_IN_EWORD);
		to_write = pack_pos_to_offset(reuse_packfile->p, written)
			- اندازه(ساختار pack_header);

		/* We're recording one chunk, not one object. */
		record_reused_object(اندازه(ساختار pack_header), 0);
		hashflush(out);
		copy_pack_data(out, reuse_packfile->p, w_curs,
			اندازه(ساختار pack_header), to_write);

		display_progress(progress_state, written);
	}
	بازگشت pos;
}

ایستا تهی write_reused_pack(ساختار bitmapped_pack *reuse_packfile,
			      ساختار hashfile *f)
{
	size_t i = reuse_packfile->bitmap_pos / BITS_IN_EWORD;
	uint32_t offset;
	off_t pack_start = hashfile_total(f) - اندازه(ساختار pack_header);
	ساختار pack_window *w_curs = پوچ;

	اگر (allow_ofs_delta)
		i = write_reused_pack_verbatim(reuse_packfile, f, &w_curs);

	برای (; i < reuse_packfile_bitmap->word_alloc; ++i) {
		eword_t word = reuse_packfile_bitmap->واژگان[i];
		size_t pos = (i * BITS_IN_EWORD);

		برای (offset = 0; offset < BITS_IN_EWORD; ++offset) {
			uint32_t pack_pos;
			اگر ((word >> offset) == 0)
				بشکن;

			offset += ewah_bit_ctz64(word >> offset);
			اگر (pos + offset < reuse_packfile->bitmap_pos)
				ادامه;
			اگر (pos + offset >= reuse_packfile->bitmap_pos + reuse_packfile->bitmap_nr)
				برو done;

			اگر (reuse_packfile->bitmap_pos) {
				/*
				 * When doing multi-pack reuse on a
				 * non-preferred pack, translate bit positions
				 * from the MIDX pseudo-pack order back to their
				 * pack-relative positions before attempting
				 * reuse.
				 */
				ساختار multi_pack_index *m = reuse_packfile->from_midx;
				uint32_t midx_pos;
				off_t pack_ofs;

				اگر (!m)
					BUG("non-zero bitmap position without MIDX");

				midx_pos = pack_pos_to_midx(m, pos + offset);
				pack_ofs = nth_midxed_offset(m, midx_pos);

				اگر (offset_to_pack_pos(reuse_packfile->p,
						       pack_ofs, &pack_pos) < 0)
					BUG("could not find expected object at offset %"PRIuMAX" in pack %s",
					    (uintmax_t)pack_ofs,
					    pack_basename(reuse_packfile->p));
			} وگرنه {
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

ایستا تهی write_excluded_by_configs(تهی)
{
	ساختار oidset_iter iter;
	ثابت ساختار object_id *oid;

	oidset_iter_init(&excluded_by_config, &iter);
	وقتی ((oid = oidset_iter_next(&iter))) {
		ساختار configured_exclusion *ex =
			oidmap_get(&configured_exclusions, oid);

		اگر (!ex)
			BUG("configured exclusion wasn't configured");
		write_in_full(1, ex->pack_hash_hex, strlen(ex->pack_hash_hex));
		write_in_full(1, " ", 1);
		write_in_full(1, ex->uri, strlen(ex->uri));
		write_in_full(1, "\n", 1);
	}
}

ایستا ثابت نویسه no_split_warning[] = N_(
"disabling bitmap writing, packs are split due to pack.packSizeLimit"
);

ایستا تهی write_pack_file(تهی)
{
	uint32_t i = 0, j;
	ساختار hashfile *f;
	off_t offset;
	uint32_t nr_remaining = nr_result;
	time_t last_mtime = 0;
	ساختار object_entry **write_order;
	off_t bytes_written = 0;

	اگر (progress > pack_to_stdout)
		progress_state = start_progress(the_repository,
						_("Writing objects"), nr_result);
	ALLOC_ARRAY(written_list, to_pack.nr_objects);
	write_order = compute_write_order();

	انجامبده {
		unsigned نویسه hash[GIT_MAX_RAWSZ];
		نویسه *pack_tmp_name = پوچ;

		اگر (pack_to_stdout) {
			/*
			 * This command is most often invoked via
			 * git-upload-pack(1), which will typically chunk data
			 * into pktlines. As such, we use the maximum data
			 * length of them as buffer length.
			 *
			 * Note that we need to subtract one though to
			 * accommodate for the sideband byte.
			 */
			ساختار hashfd_options opts = {
				.progress = progress_state,
				.buffer_len = LARGE_PACKET_DATA_MAX - 1,
			};
			f = hashfd_ext(the_repository->hash_algo, 1,
				       "<stdout>", &opts);
		} وگرنه {
			f = create_tmp_packfile(the_repository, &pack_tmp_name);
		}

		offset = write_pack_header(f, nr_remaining);

		اگر (reuse_packfiles_nr) {
			أكد(pack_to_stdout);
			برای (j = 0; j < reuse_packfiles_nr; j++) {
				reused_chunks_nr = 0;
				write_reused_pack(&reuse_packfiles[j], f);
				اگر (reused_chunks_nr)
					reuse_packfiles_used_nr++;
			}
			offset = hashfile_total(f);
		}

		nr_written = 0;
		برای (; i < to_pack.nr_objects; i++) {
			ساختار object_entry *e = write_order[i];
			اگر (write_one(f, e, &offset) == WRITE_ONE_BREAK)
				بشکن;
			display_progress(progress_state, written);
		}

		bytes_written += hashfile_total(f) +
			the_repository->hash_algo->rawsz;
		اگر (pack_to_stdout) {
			/*
			 * We never fsync when writing to stdout since we may
			 * not be writing to an actual pack file. For instance,
			 * the upload-pack code passes a pipe here. Calling
			 * fsync on a pipe results in unnecessary
			 * synchronization with the reader on some platforms.
			 */
			finalize_hashfile(f, hash, FSYNC_COMPONENT_NONE,
					  CSUM_HASH_IN_STREAM | CSUM_CLOSE);
		} وگرنه اگر (nr_written == nr_remaining) {
			finalize_hashfile(f, hash, FSYNC_COMPONENT_PACK,
					  CSUM_HASH_IN_STREAM | CSUM_FSYNC | CSUM_CLOSE);
		} وگرنه {
			/*
			 * If we wrote the wrong number of entries in the
			 * header, rewrite it like in fast-import.
			 */

			صحیح fd = finalize_hashfile(f, hash, FSYNC_COMPONENT_PACK, 0);
			fixup_pack_header_footer(the_hash_algo, fd, hash,
						 pack_tmp_name, nr_written,
						 hash, offset);
			close(fd);
			اگر (write_bitmap_index) {
				اگر (write_bitmap_index != WRITE_BITMAP_QUIET)
					warning(_(no_split_warning));
				write_bitmap_index = 0;
			}
		}

		اگر (!pack_to_stdout) {
			ساختار stat st;
			ساختار strbuf tmpname = STRBUF_INIT;
			ساختار bitmap_writer bitmap_writer;
			نویسه *idx_tmp_name = پوچ;

			/*
			 * Packs are runtime accessed in their mtime
			 * order since newer packs are more likely to contain
			 * younger objects.  So if we are creating multiple
			 * packs then we should modify the mtime of later ones
			 * to preserve this property.
			 */
			اگر (stat(pack_tmp_name, &st) < 0) {
				warning_errno(_("failed to stat %s"), pack_tmp_name);
			} وگرنه اگر (!last_mtime) {
				last_mtime = st.st_mtime;
			} وگرنه {
				ساختار utimbuf utb;
				utb.actime = st.st_atime;
				utb.modtime = --last_mtime;
				اگر (utime(pack_tmp_name, &utb) < 0)
					warning_errno(_("failed utime() on %s"), pack_tmp_name);
			}

			strbuf_addf(&tmpname, "%s-%s.", base_name,
				    hash_to_hex(hash));

			اگر (write_bitmap_index) {
				bitmap_writer_init(&bitmap_writer,
						   the_repository, &to_pack,
						   پوچ);
				bitmap_writer_set_checksum(&bitmap_writer, hash);
				bitmap_writer_build_type_index(&bitmap_writer,
							       written_list);
			}

			اگر (cruft)
				pack_idx_opts.flags |= WRITE_MTIMES;

			stage_tmp_packfiles(the_repository, &tmpname,
					    pack_tmp_name, written_list,
					    nr_written, &to_pack,
					    &pack_idx_opts, hash,
					    &idx_tmp_name);

			اگر (write_bitmap_index) {
				size_t tmpname_len = tmpname.len;

				strbuf_addstr(&tmpname, "bitmap");
				stop_progress(&progress_state);

				bitmap_writer_show_progress(&bitmap_writer,
							    progress);
				bitmap_writer_select_commits(&bitmap_writer,
							     indexed_commits,
							     indexed_commits_nr);
				اگر (bitmap_writer_build(&bitmap_writer) < 0)
					die(_("failed to write bitmap index"));
				bitmap_writer_finish(&bitmap_writer,
						     written_list,
						     tmpname.buf, write_bitmap_options);
				bitmap_writer_free(&bitmap_writer);
				write_bitmap_index = 0;
				strbuf_setlen(&tmpname, tmpname_len);
			}

			rename_tmp_packfile_idx(the_repository, &tmpname, &idx_tmp_name);

			آزاد(idx_tmp_name);
			strbuf_release(&tmpname);
			آزاد(pack_tmp_name);
			puts(hash_to_hex(hash));
		}

		/* mark written objects as written to previous pack */
		برای (j = 0; j < nr_written; j++) {
			written_list[j]->offset = (off_t)-1;
		}
		nr_remaining -= nr_written;
	} وقتی (nr_remaining && i < to_pack.nr_objects);

	آزاد(written_list);
	آزاد(write_order);
	stop_progress(&progress_state);
	اگر (written != nr_result)
		die(_("wrote %"PRIu32" objects while expecting %"PRIu32),
		    written, nr_result);
	trace2_data_intmax("pack-objects", the_repository,
			   "write_pack_file/wrote", nr_result);
	trace2_data_intmax("pack-objects", the_repository,
			   "write_pack_file/wrote_bytes", bytes_written);
}

ایستا صحیح no_try_delta(ثابت نویسه *path)
{
	ایستا ساختار attr_check *check;

	اگر (!check)
		check = attr_check_initl("delta", پوچ);
	git_check_attr(the_repository->index, path, check);
	اگر (ATTR_FALSE(check->اقلام[0].مقدار))
		بازگشت 1;
	بازگشت 0;
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
ایستا صحیح have_duplicate_entry(ثابت ساختار object_id *oid,
				صحیح exclude)
{
	ساختار object_entry *entry;

	اگر (reuse_packfile_bitmap &&
	    bitmap_walk_contains(bitmap_git, reuse_packfile_bitmap, oid))
		بازگشت 1;

	entry = packlist_find(&to_pack, oid);
	اگر (!entry)
		بازگشت 0;

	اگر (exclude) {
		اگر (!entry->preferred_base)
			nr_result--;
		entry->preferred_base = 1;
	}

	بازگشت 1;
}

ایستا صحیح want_cruft_object_mtime(ساختار repository *r,
				   ثابت ساختار object_id *oid,
				   unsigned flags, uint32_t mtime)
{
	ساختار odb_source_files *files = odb_source_files_downcast(r->objects->source);

	برای (ساختار odb_files_dir *dir = files->dirs; dir; dir = dir->next) {
		ساختار packed_git **cache = packfile_store_get_kept_pack_cache(dir->packed, flags);

		برای (; *cache; cache++) {
			ساختار packed_git *p = *cache;
			off_t ofs;
			uint32_t candidate_mtime;

			ofs = find_pack_entry_one(oid, p);
			اگر (!ofs)
				ادامه;

			/*
			 * We have a copy of the object 'oid' in a non-cruft
			 * pack. We can avoid packing an additional copy
			 * regardless of what the existing copy's mtime is since
			 * it is outside of a cruft pack.
			 */
			اگر (!p->is_cruft)
				بازگشت 0;

			/*
			 * If we have a copy of the object 'oid' in a cruft
			 * pack, then either read the cruft pack's mtime for
			 * that object, or, if that can't be loaded, assume the
			 * pack's mtime itself.
			 */
			اگر (!load_pack_mtimes(p)) {
				uint32_t pos;
				اگر (offset_to_pack_pos(p, ofs, &pos) < 0)
					ادامه;
				candidate_mtime = nth_packed_mtime(p, pos);
			} وگرنه {
				candidate_mtime = p->mtime;
			}

			/*
			 * We have a surviving copy of the object in a cruft
			 * pack whose mtime is greater than or equal to the one
			 * we are considering. We can thus avoid packing an
			 * additional copy of that object.
			 */
			اگر (mtime <= candidate_mtime)
				بازگشت 0;
		}
	}

	بازگشت -1;
}

ایستا صحیح want_found_object(ثابت ساختار object_id *oid, صحیح exclude,
			     ساختار packed_git *p, uint32_t mtime)
{
	اگر (exclude)
		بازگشت 1;
	اگر (incremental)
		بازگشت 0;

	اگر (!is_pack_valid(p))
		بازگشت -1;

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
	اگر (local && !p->pack_local)
		بازگشت 0;

	/*
	 * Then handle .keep first, as we have a fast(er) path there.
	 */
	اگر (ignore_packed_keep_on_disk || ignore_packed_keep_in_core ||
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
		اگر (ignore_packed_keep_on_disk)
			flags |= KEPT_PACK_ON_DISK;
		اگر (ignore_packed_keep_in_core)
			flags |= KEPT_PACK_IN_CORE;
		اگر (ignore_packed_keep_in_core_open)
			flags |= KEPT_PACK_IN_CORE_OPEN;

		/*
		 * If the object is in a pack that we want to ignore, *and* we
		 * don't have any cruft packs that are being retained, we can
		 * abort quickly.
		 */
		اگر (!ignore_packed_keep_in_core_has_cruft) {
			اگر (ignore_packed_keep_on_disk && p->pack_keep)
				بازگشت 0;
			اگر (ignore_packed_keep_in_core && p->pack_keep_in_core)
				بازگشت 0;
			اگر (ignore_packed_keep_in_core_open && p->pack_keep_in_core_open)
				بازگشت 0;
			اگر (has_object_kept_pack(p->repo, oid, flags))
				بازگشت 0;
		} وگرنه {
			/*
			 * But if there is at least one cruft pack which
			 * is being kept, we only want to include the
			 * provided object if it has a strictly greater
			 * mtime than any existing cruft copy.
			 */
			اگر (!want_cruft_object_mtime(p->repo, oid, flags,
						     mtime))
				بازگشت 0;
		}
	}

	/*
	 * At this point we know definitively that either we don't care about
	 * keep-packs, or the object is not in one. Keep checking other
	 * conditions...
	 */
	اگر (!local || !have_non_local_packs)
		بازگشت 1;

	/* we don't know yet; keep looking for more packs */
	بازگشت -1;
}

ایستا صحیح want_object_in_pack_one(ساختار packed_git *p,
				   ثابت ساختار object_id *oid,
				   صحیح exclude,
				   ساختار packed_git **found_pack,
				   off_t *found_offset,
				   uint32_t found_mtime)
{
	off_t offset;

	اگر (p == *found_pack)
		offset = *found_offset;
	وگرنه
		offset = find_pack_entry_one(oid, p);

	اگر (offset) {
		اگر (!*found_pack) {
			اگر (!is_pack_valid(p))
				بازگشت -1;
			*found_offset = offset;
			*found_pack = p;
		}
		بازگشت want_found_object(oid, exclude, p, found_mtime);
	}
	بازگشت -1;
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
ایستا صحیح want_object_in_pack_mtime(ثابت ساختار object_id *oid,
				     صحیح exclude,
				     ساختار packed_git **found_pack,
				     off_t *found_offset,
				     uint32_t found_mtime)
{
	ساختار odb_source_files *files =
		odb_source_files_downcast(the_repository->objects->source);
	صحیح want;
	ساختار packfile_list_entry *e;

	اگر (!exclude && local) {
		/*
		 * Note that we start iterating at `dirs->next` so that we
		 * skip the local object directory.
		 */
		برای (ساختار odb_files_dir *dir = files->dirs->next; dir; dir = dir->next)
			اگر (!odb_source_read_object_info(&dir->loose->base, oid, پوچ, 0, پوچ))
				بازگشت 0;
	}

	/*
	 * If we already know the pack object lives in, start checks from that
	 * pack - in the usual case when neither --local was given nor .keep files
	 * are present we will determine the answer right now.
	 */
	اگر (*found_pack) {
		want = want_found_object(oid, exclude, *found_pack,
					 found_mtime);
		اگر (want != -1)
			بازگشت want;

		*found_pack = پوچ;
		*found_offset = 0;
	}

	برای (ساختار odb_files_dir *dir = files->dirs; dir; dir = dir->next) {
		ساختار multi_pack_index *m = get_multi_pack_index(dir->packed);
		ساختار pack_entry e;

		اگر (m && midx_fill_entry(m, oid, &e, پوچ) == MIDX_FILL_HIT) {
			want = want_object_in_pack_one(e.p, oid, exclude, found_pack, found_offset, found_mtime);
			اگر (want != -1)
				بازگشت want;
		}
	}

	برای (ساختار odb_files_dir *dir = files->dirs; dir; dir = dir->next) {
		برای (e = dir->packed->packs.head; e; e = e->next) {
			ساختار packed_git *p = e->pack;
			want = want_object_in_pack_one(p, oid, exclude, found_pack, found_offset, found_mtime);
			اگر (!exclude && want > 0)
				packfile_list_prepend(&dir->packed->packs, p);
			اگر (want != -1)
				بازگشت want;
		}
	}

	اگر (uri_protocols.nr) {
		ساختار configured_exclusion *ex =
			oidmap_get(&configured_exclusions, oid);
		صحیح i;
		ثابت نویسه *p;

		اگر (ex) {
			برای (i = 0; i < uri_protocols.nr; i++) {
				اگر (skip_prefix(ex->uri,
						uri_protocols.اقلام[i].string,
						&p) &&
				    *p == ':') {
					oidset_درج(&excluded_by_config, oid);
					بازگشت 0;
				}
			}
		}
	}

	بازگشت 1;
}

ایستا درونخطی صحیح want_object_in_pack(ثابت ساختار object_id *oid,
				      صحیح exclude,
				      ساختار packed_git **found_pack,
				      off_t *found_offset)
{
	بازگشت want_object_in_pack_mtime(oid, exclude, found_pack, found_offset,
					 0);
}

ایستا ساختار object_entry *create_object_entry(ثابت ساختار object_id *oid,
						شمارش object_type type,
						uint32_t hash,
						صحیح exclude,
						صحیح no_try_delta,
						ساختار packed_git *found_pack,
						off_t found_offset)
{
	ساختار object_entry *entry;

	entry = packlist_alloc(&to_pack, oid);
	entry->hash = hash;
	oe_set_type(entry, type);
	اگر (exclude)
		entry->preferred_base = 1;
	وگرنه
		nr_result++;
	اگر (found_pack) {
		oe_set_in_pack(&to_pack, entry, found_pack);
		entry->in_pack_offset = found_offset;
	}

	entry->no_try_delta = no_try_delta;

	بازگشت entry;
}

ایستا ثابت نویسه no_closure_warning[] = N_(
"disabling bitmap writing, as some objects are not being packed"
);

ایستا تهی add_object_entry(ثابت ساختار object_id *oid, شمارش object_type type,
			     ثابت نویسه *name, صحیح exclude)
{
	ساختار packed_git *found_pack = پوچ;
	off_t found_offset = 0;

	display_progress(progress_state, ++nr_seen);

	اگر (have_duplicate_entry(oid, exclude))
		بازگشت;

	اگر (!want_object_in_pack(oid, exclude, &found_pack, &found_offset)) {
		/* The pack is missing an object, so it will not have closure */
		اگر (write_bitmap_index) {
			اگر (write_bitmap_index != WRITE_BITMAP_QUIET)
				warning(_(no_closure_warning));
			write_bitmap_index = 0;
		}
		بازگشت;
	}

	create_object_entry(oid, type, pack_name_hash_fn(name),
			    exclude, name && no_try_delta(name),
			    found_pack, found_offset);
}

ایستا صحیح add_object_entry_from_bitmap(ثابت ساختار object_id *oid,
					شمارش object_type type,
					صحیح flags UNUSED, uint32_t name_hash,
					ساختار packed_git *pack, off_t offset,
					تهی *payload UNUSED)
{
	display_progress(progress_state, ++nr_seen);

	اگر (have_duplicate_entry(oid, 0))
		بازگشت 0;

	اگر (!want_object_in_pack(oid, 0, &pack, &offset))
		بازگشت 0;

	create_object_entry(oid, type, name_hash, 0, 0, pack, offset);
	بازگشت 0;
}

ساختار pbase_درخت_cache {
	ساختار object_id oid;
	صحیح ref;
	صحیح temporary;
	تهی *درخت_data;
	unsigned long درخت_size;
};

ایستا ساختار pbase_درخت_cache *(pbase_درخت_cache[256]);
ایستا صحیح pbase_درخت_cache_ix(ثابت ساختار object_id *oid)
{
	بازگشت oid->hash[0] % ARRAY_SIZE(pbase_درخت_cache);
}
ایستا صحیح pbase_درخت_cache_ix_incr(صحیح ix)
{
	بازگشت (ix+1) % ARRAY_SIZE(pbase_درخت_cache);
}

ایستا ساختار pbase_درخت {
	ساختار pbase_درخت *next;
	/* This is a phony "cache" entry; we are not
	 * going to evict it or find it through _get()
	 * mechanism -- this is for the toplevel node that
	 * would almost always change with any commit.
	 */
	ساختار pbase_درخت_cache pcache;
} *pbase_درخت;

ایستا ساختار pbase_درخت_cache *pbase_درخت_get(ثابت ساختار object_id *oid)
{
	ساختار pbase_درخت_cache *ent, *nent;
	تهی *data;
	unsigned long size;
	size_t size_st = 0;
	شمارش object_type type;
	صحیح neigh;
	صحیح my_ix = pbase_درخت_cache_ix(oid);
	صحیح available_ix = -1;

	/* pbase-tree-cache acts as a limited hashtable.
	 * your object will be found at your index or within a few
	 * slots after that slot if it is cached.
	 */
	برای (neigh = 0; neigh < 8; neigh++) {
		ent = pbase_درخت_cache[my_ix];
		اگر (ent && oideq(&ent->oid, oid)) {
			ent->ref++;
			بازگشت ent;
		}
		وگرنه اگر (((available_ix < 0) && (!ent || !ent->ref)) ||
			 ((0 <= available_ix) &&
			  (!ent && pbase_درخت_cache[available_ix])))
			available_ix = my_ix;
		اگر (!ent)
			بشکن;
		my_ix = pbase_درخت_cache_ix_incr(my_ix);
	}

	/* Did not find one.  Either we got a bogus request or
	 * we need to read and perhaps cache.
	 */
	data = odb_read_object(the_repository->objects, oid, &type, &size_st);
	size = cast_size_t_to_ulong(size_st);
	اگر (!data)
		بازگشت پوچ;
	اگر (type != OBJ_درخت) {
		آزاد(data);
		بازگشت پوچ;
	}

	/* We need to either cache or return a throwaway copy */

	اگر (available_ix < 0)
		ent = پوچ;
	وگرنه {
		ent = pbase_درخت_cache[available_ix];
		my_ix = available_ix;
	}

	اگر (!ent) {
		nent = xmalloc(اندازه(*nent));
		nent->temporary = (available_ix < 0);
	}
	وگرنه {
		/* evict and reuse */
		آزاد(ent->درخت_data);
		nent = ent;
	}
	oidcpy(&nent->oid, oid);
	nent->درخت_data = data;
	nent->درخت_size = size;
	nent->ref = 1;
	اگر (!nent->temporary)
		pbase_درخت_cache[my_ix] = nent;
	بازگشت nent;
}

ایستا تهی pbase_درخت_put(ساختار pbase_درخت_cache *cache)
{
	اگر (!cache->temporary) {
		cache->ref--;
		بازگشت;
	}
	آزاد(cache->درخت_data);
	آزاد(cache);
}

ایستا size_t name_cmp_len(ثابت نویسه *name)
{
	بازگشت strcspn(name, "\n/");
}

ایستا تهی add_pbase_object(ساختار درخت_desc *درخت,
			     ثابت نویسه *name,
			     size_t cmplen,
			     ثابت نویسه *fullname)
{
	ساختار name_entry entry;
	صحیح cmp;

	وقتی (درخت_entry(درخت,&entry)) {
		اگر (S_ISGITLINK(entry.mode))
			ادامه;
		cmp = درخت_entry_len(&entry) != cmplen ? 1 :
		      memcmp(name, entry.path, cmplen);
		اگر (cmp > 0)
			ادامه;
		اگر (cmp < 0)
			بازگشت;
		اگر (name[cmplen] != '/') {
			add_object_entry(&entry.oid,
					 object_type(entry.mode),
					 fullname, 1);
			بازگشت;
		}
		اگر (S_ISDIR(entry.mode)) {
			ساختار درخت_desc sub;
			ساختار pbase_درخت_cache *درخت;
			ثابت نویسه *down = name+cmplen+1;
			size_t downlen = name_cmp_len(down);

			درخت = pbase_درخت_get(&entry.oid);
			اگر (!درخت)
				بازگشت;
			init_درخت_desc(&sub, &درخت->oid,
				       درخت->درخت_data, درخت->درخت_size);

			add_pbase_object(&sub, down, downlen, fullname);
			pbase_درخت_put(درخت);
		}
	}
}

ایستا unsigned *done_pbase_paths;
ایستا صحیح done_pbase_paths_رقم;
ایستا صحیح done_pbase_paths_alloc;
ایستا صحیح done_pbase_path_pos(unsigned hash)
{
	صحیح lo = 0;
	صحیح hi = done_pbase_paths_رقم;
	وقتی (lo < hi) {
		صحیح mi = lo + (hi - lo) / 2;
		اگر (done_pbase_paths[mi] == hash)
			بازگشت mi;
		اگر (done_pbase_paths[mi] < hash)
			hi = mi;
		وگرنه
			lo = mi + 1;
	}
	بازگشت -lo-1;
}

ایستا صحیح check_pbase_path(unsigned hash)
{
	صحیح pos = done_pbase_path_pos(hash);
	اگر (0 <= pos)
		بازگشت 1;
	pos = -pos - 1;
	ALLOC_GROW(done_pbase_paths,
		   done_pbase_paths_رقم + 1,
		   done_pbase_paths_alloc);
	done_pbase_paths_رقم++;
	اگر (pos < done_pbase_paths_رقم)
		MOVE_ARRAY(done_pbase_paths + pos + 1, done_pbase_paths + pos,
			   done_pbase_paths_رقم - pos - 1);
	done_pbase_paths[pos] = hash;
	بازگشت 0;
}

ایستا تهی add_preferred_base_object(ثابت نویسه *name)
{
	ساختار pbase_درخت *it;
	size_t cmplen;
	unsigned hash = pack_name_hash_fn(name);

	اگر (!رقم_preferred_base || check_pbase_path(hash))
		بازگشت;

	cmplen = name_cmp_len(name);
	برای (it = pbase_درخت; it; it = it->next) {
		اگر (cmplen == 0) {
			add_object_entry(&it->pcache.oid, OBJ_درخت, پوچ, 1);
		}
		وگرنه {
			ساختار درخت_desc درخت;
			init_درخت_desc(&درخت, &it->pcache.oid,
				       it->pcache.درخت_data, it->pcache.درخت_size);
			add_pbase_object(&درخت, name, cmplen, name);
		}
	}
}

ایستا تهی add_preferred_base(ساختار object_id *oid)
{
	ساختار pbase_درخت *it;
	تهی *data;
	unsigned long size;
	size_t size_st = 0;
	ساختار object_id درخت_oid;

	اگر (window <= رقم_preferred_base++)
		بازگشت;

	data = odb_read_object_peeled(the_repository->objects, oid,
				      OBJ_درخت, &size_st, &درخت_oid);
	size = cast_size_t_to_ulong(size_st);
	اگر (!data)
		بازگشت;

	برای (it = pbase_درخت; it; it = it->next) {
		اگر (oideq(&it->pcache.oid, &درخت_oid)) {
			آزاد(data);
			بازگشت;
		}
	}

	CALLOC_ARRAY(it, 1);
	it->next = pbase_درخت;
	pbase_درخت = it;

	oidcpy(&it->pcache.oid, &درخت_oid);
	it->pcache.درخت_data = data;
	it->pcache.درخت_size = size;
}

ایستا تهی cleanup_preferred_base(تهی)
{
	ساختار pbase_درخت *it;
	unsigned i;

	it = pbase_درخت;
	pbase_درخت = پوچ;
	وقتی (it) {
		ساختار pbase_درخت *tmp = it;
		it = tmp->next;
		آزاد(tmp->pcache.درخت_data);
		آزاد(tmp);
	}

	برای (i = 0; i < ARRAY_SIZE(pbase_درخت_cache); i++) {
		اگر (!pbase_درخت_cache[i])
			ادامه;
		آزاد(pbase_درخت_cache[i]->درخت_data);
		FREE_AND_NULL(pbase_درخت_cache[i]);
	}

	FREE_AND_NULL(done_pbase_paths);
	done_pbase_paths_رقم = done_pbase_paths_alloc = 0;
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
ایستا صحیح can_reuse_delta(ثابت ساختار object_id *base_oid,
			   ساختار object_entry *delta,
			   ساختار object_entry **base_out)
{
	ساختار object_entry *base;

	/*
	 * First see if we're already sending the base (or it's explicitly in
	 * our "excluded" list).
	 */
	base = packlist_find(&to_pack, base_oid);
	اگر (base) {
		اگر (!in_same_island(&delta->idx.oid, &base->idx.oid))
			بازگشت 0;
		*base_out = base;
		بازگشت 1;
	}

	/*
	 * Otherwise, reachability bitmaps may tell us if the receiver has it,
	 * even if it was buried too deep in history to make it into the
	 * packing list.
	 */
	اگر (thin && bitmap_has_oid_in_uninteresting(bitmap_git, base_oid)) {
		اگر (use_delta_islands) {
			اگر (!in_same_island(&delta->idx.oid, base_oid))
				بازگشت 0;
		}
		*base_out = پوچ;
		بازگشت 1;
	}

	بازگشت 0;
}

ایستا تهی prefetch_to_pack(uint32_t object_index_start) {
	ساختار oid_array to_fetch = OID_ARRAY_INIT;
	uint32_t i;

	برای (i = object_index_start; i < to_pack.nr_objects; i++) {
		ساختار object_entry *entry = to_pack.objects + i;

		اگر (!odb_read_object_info_extended(the_repository->objects,
						   &entry->idx.oid,
						   پوچ,
						   OBJECT_INFO_FOR_PREFETCH))
			ادامه;
		oid_array_append(&to_fetch, &entry->idx.oid);
	}
	promisor_remote_get_direct(the_repository,
				   to_fetch.oid, to_fetch.nr);
	oid_array_clear(&to_fetch);
}

ایستا تهی check_object(ساختار object_entry *entry, uint32_t object_index)
{
	size_t canonical_size;
	شمارش object_type type;
	ساختار object_info oi = {.typep = &type, .sizep = &canonical_size};

	اگر (IN_PACK(entry)) {
		ساختار packed_git *p = IN_PACK(entry);
		ساختار pack_window *w_curs = پوچ;
		صحیح have_base = 0;
		ساختار object_id base_ref;
		ساختار object_entry *base_entry;
		size_t used, used_0, avail;
		off_t ofs;
		unsigned نویسه *buf, c;
		شمارش object_type type;
		size_t in_pack_size;

		buf = use_pack(p, &w_curs, entry->in_pack_offset, &avail);

		/*
		 * We want in_pack_type even if we do not reuse delta
		 * since non-delta representations could still be reused.
		 */
		used = unpack_object_header_buffer(buf, avail,
						   &type,
						   &in_pack_size);
		اگر (used == 0)
			برو give_up;

		اگر (type < 0)
			BUG("invalid type %d", type);
		entry->in_pack_type = type;

		/*
		 * Determine if this is a delta and if so whether we can
		 * reuse it or not.  Otherwise let's find out as cheaply as
		 * possible what the actual type and size for this object is.
		 */
		انتخاب (entry->in_pack_type) {
		پیشفرض:
			/* Not a delta hence we've already got all we need. */
			oe_set_type(entry, entry->in_pack_type);
			SET_SIZE(entry, in_pack_size);
			entry->in_pack_header_size = used;
			اگر (oe_type(entry) < OBJ_COMMIT || oe_type(entry) > OBJ_BLOB)
				برو give_up;
			unuse_pack(&w_curs);
			بازگشت;
		حالت OBJ_REF_DELTA:
			اگر (reuse_delta && !entry->preferred_base) {
				oidread(&base_ref,
					use_pack(p, &w_curs,
						 entry->in_pack_offset + used,
						 پوچ),
					the_repository->hash_algo);
				have_base = 1;
			}
			entry->in_pack_header_size = used + the_hash_algo->rawsz;
			بشکن;
		حالت OBJ_OFS_DELTA:
			buf = use_pack(p, &w_curs,
				       entry->in_pack_offset + used, پوچ);
			used_0 = 0;
			c = buf[used_0++];
			ofs = c & 127;
			وقتی (c & 128) {
				ofs += 1;
				اگر (!ofs || MSB(ofs, 7)) {
					error(_("delta base offset overflow in pack for %s"),
					      oid_to_hex(&entry->idx.oid));
					برو give_up;
				}
				c = buf[used_0++];
				ofs = (ofs << 7) + (c & 127);
			}
			ofs = entry->in_pack_offset - ofs;
			اگر (ofs <= 0 || ofs >= entry->in_pack_offset) {
				error(_("delta base offset out of bound for %s"),
				      oid_to_hex(&entry->idx.oid));
				برو give_up;
			}
			اگر (reuse_delta && !entry->preferred_base) {
				uint32_t pos;
				اگر (offset_to_pack_pos(p, ofs, &pos) < 0)
					برو give_up;
				اگر (!nth_packed_object_id(&base_ref, p,
							  pack_pos_to_index(p, pos)))
					have_base = 1;
			}
			entry->in_pack_header_size = used + used_0;
			بشکن;
		}

		اگر (have_base &&
		    can_reuse_delta(&base_ref, entry, &base_entry)) {
			oe_set_type(entry, entry->in_pack_type);
			SET_SIZE(entry, in_pack_size); /* delta size */
			SET_DELTA_SIZE(entry, in_pack_size);

			اگر (base_entry) {
				SET_DELTA(entry, base_entry);
				entry->delta_sibling_idx = base_entry->delta_child_idx;
				SET_DELTA_CHILD(base_entry, entry);
			} وگرنه {
				SET_DELTA_EXT(entry, &base_ref);
			}

			unuse_pack(&w_curs);
			بازگشت;
		}

		اگر (oe_type(entry)) {
			off_t delta_pos;

			/*
			 * This must be a delta and we already know what the
			 * final object type is.  Let's extract the actual
			 * object size from the delta header.
			 */
			delta_pos = entry->in_pack_offset + entry->in_pack_header_size;
			canonical_size = get_size_from_delta(p, &w_curs,
							     delta_pos);
			اگر (canonical_size == 0)
				برو give_up;
			SET_SIZE(entry, canonical_size);
			unuse_pack(&w_curs);
			بازگشت;
		}

		/*
		 * No choice but to fall back to the recursive delta walk
		 * with odb_read_object_info() to find about the object type
		 * at this point...
		 */
		give_up:
		unuse_pack(&w_curs);
	}

	اگر (odb_read_object_info_extended(the_repository->objects, &entry->idx.oid, &oi,
					  OBJECT_INFO_SKIP_FETCH_OBJECT | OBJECT_INFO_LOOKUP_REPLACE) < 0) {
		اگر (repo_has_promisor_remote(the_repository)) {
			prefetch_to_pack(object_index);
			اگر (odb_read_object_info_extended(the_repository->objects, &entry->idx.oid, &oi,
							  OBJECT_INFO_SKIP_FETCH_OBJECT | OBJECT_INFO_LOOKUP_REPLACE) < 0)
				type = -1;
		} وگرنه {
			type = -1;
		}
	}
	oe_set_type(entry, type);
	اگر (entry->type_valid) {
		SET_SIZE(entry, canonical_size);
	} وگرنه {
		/*
		 * Bad object type is checked in prepare_pack().  This is
		 * to permit a missing preferred base object to be ignored
		 * as a preferred base.  Doing so can result in a larger
		 * pack file, but the transfer will still take place.
		 */
	}
}

ایستا صحیح pack_offset_sort(ثابت تهی *_a, ثابت تهی *_b)
{
	ثابت ساختار object_entry *a = *(ساختار object_entry **)_a;
	ثابت ساختار object_entry *b = *(ساختار object_entry **)_b;
	ثابت ساختار packed_git *a_in_pack = IN_PACK(a);
	ثابت ساختار packed_git *b_in_pack = IN_PACK(b);

	/* avoid filesystem trashing with loose objects */
	اگر (!a_in_pack && !b_in_pack)
		بازگشت oidcmp(&a->idx.oid, &b->idx.oid);

	اگر (a_in_pack < b_in_pack)
		بازگشت -1;
	اگر (a_in_pack > b_in_pack)
		بازگشت 1;
	بازگشت a->in_pack_offset < b->in_pack_offset ? -1 :
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
ایستا تهی drop_reused_delta(ساختار object_entry *entry)
{
	unsigned *idx = &to_pack.objects[entry->delta_idx - 1].delta_child_idx;
	ساختار object_info oi = OBJECT_INFO_INIT;
	شمارش object_type type;
	size_t size;

	وقتی (*idx) {
		ساختار object_entry *oe = &to_pack.objects[*idx - 1];

		اگر (oe == entry)
			*idx = oe->delta_sibling_idx;
		وگرنه
			idx = &oe->delta_sibling_idx;
	}
	SET_DELTA(entry, پوچ);
	entry->depth = 0;

	oi.sizep = &size;
	oi.typep = &type;
	اگر (packed_object_info(پوچ, IN_PACK(entry), entry->in_pack_offset, &oi) < 0) {
		/*
		 * We failed to get the info from this pack for some reason;
		 * fall back to odb_read_object_info, which may find another copy.
		 * And if that fails, the error will be recorded in oe_type(entry)
		 * and dealt with in prepare_pack().
		 */
		oe_set_type(entry,
			    odb_read_object_info(the_repository->objects,
						 &entry->idx.oid, &size));
	} وگرنه {
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
ایستا تهی break_delta_chains(ساختار object_entry *entry)
{
	/*
	 * The actual depth of each object we will write is stored as an int,
	 * as it cannot exceed our int "depth" limit. But before we break
	 * changes based no that limit, we may potentially go as deep as the
	 * number of objects, which is elsewhere bounded to a uint32_t.
	 */
	uint32_t total_depth;
	ساختار object_entry *cur, *next;

	برای (cur = entry, total_depth = 0;
	     cur;
	     cur = DELTA(cur), total_depth++) {
		اگر (cur->dfs_state == DFS_DONE) {
			/*
			 * We've already seen this object and know it isn't
			 * part of a cycle. We do need to append its depth
			 * to our count.
			 */
			total_depth += cur->depth;
			بشکن;
		}

		/*
		 * We break cycles before looping, so an ACTIVE state (or any
		 * other cruft which made its way into the state variable)
		 * is a bug.
		 */
		اگر (cur->dfs_state != DFS_NONE)
			BUG("confusing delta dfs state in first pass: %d",
			    cur->dfs_state);

		/*
		 * Now we know this is the first time we've seen the object. If
		 * it's not a delta, we're done traversing, but we'll mark it
		 * done to save time on future traversals.
		 */
		اگر (!DELTA(cur)) {
			cur->dfs_state = DFS_DONE;
			بشکن;
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
		اگر (DELTA(cur)->dfs_state == DFS_ACTIVE) {
			drop_reused_delta(cur);
			cur->dfs_state = DFS_DONE;
			بشکن;
		}
	}

	/*
	 * And now that we've gone all the way to the bottom of the chain, we
	 * need to clear the active flags and set the depth fields as
	 * appropriate. Unlike the loop above, which can quit when it drops a
	 * delta, we need to keep going to look for more depth cuts. So we need
	 * an extra "next" pointer to keep going after we reset cur->delta.
	 */
	برای (cur = entry; cur; cur = next) {
		next = DELTA(cur);

		/*
		 * We should have a chain of zero or more ACTIVE states down to
		 * a final DONE. We can quit after the DONE, because either it
		 * has no bases, or we've already handled them in a previous
		 * call.
		 */
		اگر (cur->dfs_state == DFS_DONE)
			بشکن;
		وگرنه اگر (cur->dfs_state != DFS_ACTIVE)
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
		اگر (!cur->depth)
			drop_reused_delta(cur);

		cur->dfs_state = DFS_DONE;
	}
}

ایستا تهی get_object_details(تهی)
{
	uint32_t i;
	ساختار object_entry **sorted_by_offset;

	اگر (progress)
		progress_state = start_progress(the_repository,
						_("Counting objects"),
						to_pack.nr_objects);

	CALLOC_ARRAY(sorted_by_offset, to_pack.nr_objects);
	برای (i = 0; i < to_pack.nr_objects; i++)
		sorted_by_offset[i] = to_pack.objects + i;
	QSORT(sorted_by_offset, to_pack.nr_objects, pack_offset_sort);

	برای (i = 0; i < to_pack.nr_objects; i++) {
		ساختار object_entry *entry = sorted_by_offset[i];
		check_object(entry, i);
		اگر (entry->type_valid &&
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
	برای (i = 0; i < to_pack.nr_objects; i++)
		break_delta_chains(&to_pack.objects[i]);

	آزاد(sorted_by_offset);
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
ایستا صحیح type_size_sort(ثابت تهی *_a, ثابت تهی *_b)
{
	ثابت ساختار object_entry *a = *(ساختار object_entry **)_a;
	ثابت ساختار object_entry *b = *(ساختار object_entry **)_b;
	ثابت شمارش object_type a_type = oe_type(a);
	ثابت شمارش object_type b_type = oe_type(b);
	ثابت unsigned long a_size = SIZE(a);
	ثابت unsigned long b_size = SIZE(b);

	اگر (a_type > b_type)
		بازگشت -1;
	اگر (a_type < b_type)
		بازگشت 1;
	اگر (a->hash > b->hash)
		بازگشت -1;
	اگر (a->hash < b->hash)
		بازگشت 1;
	اگر (a->preferred_base > b->preferred_base)
		بازگشت -1;
	اگر (a->preferred_base < b->preferred_base)
		بازگشت 1;
	اگر (use_delta_islands) {
		ثابت صحیح island_cmp = island_delta_cmp(&a->idx.oid, &b->idx.oid);
		اگر (island_cmp)
			بازگشت island_cmp;
	}
	اگر (a_size > b_size)
		بازگشت -1;
	اگر (a_size < b_size)
		بازگشت 1;
	بازگشت a < b ? -1 : (a > b);  /* newest first */
}

ساختار unpacked {
	ساختار object_entry *entry;
	تهی *data;
	ساختار delta_index *index;
	unsigned depth;
};

ایستا صحیح delta_cacheable(size_t src_size, size_t trg_size,
			   size_t delta_size)
{
	اگر (max_delta_cache_size && delta_cache_size + delta_size > max_delta_cache_size)
		بازگشت 0;

	اگر (delta_size < cache_max_small_delta_size)
		بازگشت 1;

	/* cache delta, if objects are large enough compared to delta size */
	اگر ((src_size >> 20) + (trg_size >> 21) > (delta_size >> 10))
		بازگشت 1;

	بازگشت 0;
}

/* Protect delta_cache_size */
ایستا pthread_mutex_t cache_mutex;
#define cache_lock()		pthread_mutex_lock(&cache_mutex)
#define cache_unlock()		pthread_mutex_unlock(&cache_mutex)

/*
 * Protect object list partitioning (e.g. struct thread_param) and
 * progress_state
 */
ایستا pthread_mutex_t progress_mutex;
#define progress_lock()		pthread_mutex_lock(&progress_mutex)
#define progress_unlock()	pthread_mutex_unlock(&progress_mutex)

/*
 * Access to struct object_entry is unprotected since each thread owns
 * a portion of the main object list. Just don't access object entries
 * ahead in the list because they can be stolen and would need
 * progress_mutex for protection.
 */

ایستا درونخطی صحیح oe_size_less_than(ساختار packing_data *pack,
				    ثابت ساختار object_entry *lhs,
				    size_t rhs)
{
	اگر (lhs->size_valid)
		بازگشت lhs->size_ < rhs;
	اگر (rhs < pack->oe_size_limit) /* rhs < 2^x <= lhs ? */
		بازگشت 0;
	بازگشت oe_get_size_slow(pack, lhs) < rhs;
}

ایستا درونخطی تهی oe_set_درخت_depth(ساختار packing_data *pack,
				     ساختار object_entry *e,
				     unsigned صحیح درخت_depth)
{
	اگر (!pack->درخت_depth)
		CALLOC_ARRAY(pack->درخت_depth, pack->nr_alloc);
	pack->درخت_depth[e - pack->objects] = درخت_depth;
}

ایستا تهی record_درخت_depth(ثابت ساختار object_id *oid, ثابت نویسه *name)
{
	ثابت نویسه *p;
	unsigned depth;
	ساختار object_entry *ent;

	/* the empty string is a root tree, which is depth 0 */
	depth = *name ? 1 : 0;
	برای (p = strchr(name, '/'); p; p = strchr(p + 1, '/'))
		depth++;

	ent = packlist_find(&to_pack, oid);
	اگر (ent && depth > oe_درخت_depth(&to_pack, ent))
		oe_set_درخت_depth(&to_pack, ent, depth);
}

/*
 * Return the size of the object without doing any delta
 * reconstruction (so non-deltas are true object sizes, but deltas
 * return the size of the delta data).
 */
size_t oe_get_size_slow(ساختار packing_data *pack,
			ثابت ساختار object_entry *e)
{
	ساختار packed_git *p;
	ساختار pack_window *w_curs;
	unsigned نویسه *buf;
	شمارش object_type type;
	size_t used, avail, size;

	اگر (e->type_ != OBJ_OFS_DELTA && e->type_ != OBJ_REF_DELTA) {
		size_t sz;
		packing_data_lock(&to_pack);
		اگر (odb_read_object_info(the_repository->objects,
					 &e->idx.oid, &sz) < 0)
			die(_("unable to get size of %s"),
			    oid_to_hex(&e->idx.oid));
		packing_data_unlock(&to_pack);
		بازگشت sz;
	}

	p = oe_in_pack(pack, e);
	اگر (!p)
		BUG("when e->type is a delta, it must belong to a pack");

	packing_data_lock(&to_pack);
	w_curs = پوچ;
	buf = use_pack(p, &w_curs, e->in_pack_offset, &avail);
	used = unpack_object_header_buffer(buf, avail, &type, &size);
	اگر (used == 0)
		die(_("unable to parse object header of %s"),
		    oid_to_hex(&e->idx.oid));

	unuse_pack(&w_curs);
	packing_data_unlock(&to_pack);
	بازگشت size;
}

ایستا صحیح try_delta(ساختار unpacked *trg, ساختار unpacked *src,
		     unsigned max_depth, size_t *mem_usage)
{
	ساختار object_entry *trg_entry = trg->entry;
	ساختار object_entry *src_entry = src->entry;
	unsigned long trg_size, src_size, sizediff, max_size, sz;
	size_t delta_size;
	unsigned ref_depth;
	شمارش object_type type;
	تهی *delta_buf;

	/* Don't bother doing diffs between different types */
	اگر (oe_type(trg_entry) != oe_type(src_entry))
		بازگشت -1;

	/*
	 * We do not bother to try a delta that we discarded on an
	 * earlier try, but only when reusing delta data.  Note that
	 * src_entry that is marked as the preferred_base should always
	 * be considered, as even if we produce a suboptimal delta against
	 * it, we will still save the transfer cost, as we already know
	 * the other side has it and we won't send src_entry at all.
	 */
	اگر (reuse_delta && IN_PACK(trg_entry) &&
	    IN_PACK(trg_entry) == IN_PACK(src_entry) &&
	    !src_entry->preferred_base &&
	    trg_entry->in_pack_type != OBJ_REF_DELTA &&
	    trg_entry->in_pack_type != OBJ_OFS_DELTA)
		بازگشت 0;

	/* Let's not bust the allowed depth. */
	اگر (src->depth >= max_depth)
		بازگشت 0;

	/* Now some size filtering heuristics. */
	trg_size = SIZE(trg_entry);
	اگر (!DELTA(trg_entry)) {
		max_size = trg_size/2 - the_hash_algo->rawsz;
		ref_depth = 1;
	} وگرنه {
		max_size = DELTA_SIZE(trg_entry);
		ref_depth = trg->depth;
	}
	max_size = (uint64_t)max_size * (max_depth - src->depth) /
						(max_depth - ref_depth + 1);
	اگر (max_size == 0)
		بازگشت 0;
	src_size = SIZE(src_entry);
	sizediff = src_size < trg_size ? trg_size - src_size : 0;
	اگر (sizediff >= max_size)
		بازگشت 0;
	اگر (trg_size < src_size / 32)
		بازگشت 0;

	اگر (!in_same_island(&trg->entry->idx.oid, &src->entry->idx.oid))
		بازگشت 0;

	/* Load data if not already done */
	اگر (!trg->data) {
		size_t sz_st = 0;
		packing_data_lock(&to_pack);
		trg->data = odb_read_object(the_repository->objects,
					    &trg_entry->idx.oid, &type,
					    &sz_st);
		sz = cast_size_t_to_ulong(sz_st);
		packing_data_unlock(&to_pack);
		اگر (!trg->data)
			die(_("object %s cannot be read"),
			    oid_to_hex(&trg_entry->idx.oid));
		اگر (sz != trg_size)
			die(_("object %s inconsistent object length (%"PRIuMAX" vs %"PRIuMAX")"),
			    oid_to_hex(&trg_entry->idx.oid), (uintmax_t)sz,
			    (uintmax_t)trg_size);
		*mem_usage += sz;
	}
	اگر (!src->data) {
		size_t sz_st = 0;
		packing_data_lock(&to_pack);
		src->data = odb_read_object(the_repository->objects,
					    &src_entry->idx.oid, &type,
					    &sz_st);
		sz = cast_size_t_to_ulong(sz_st);
		packing_data_unlock(&to_pack);
		اگر (!src->data) {
			اگر (src_entry->preferred_base) {
				ایستا صحیح warned = 0;
				اگر (!warned++)
					warning(_("object %s cannot be read"),
						oid_to_hex(&src_entry->idx.oid));
				/*
				 * Those objects are not included in the
				 * resulting pack.  Be resilient and ignore
				 * them if they can't be read, in case the
				 * pack could be created nevertheless.
				 */
				بازگشت 0;
			}
			die(_("object %s cannot be read"),
			    oid_to_hex(&src_entry->idx.oid));
		}
		اگر (sz != src_size)
			die(_("object %s inconsistent object length (%"PRIuMAX" vs %"PRIuMAX")"),
			    oid_to_hex(&src_entry->idx.oid), (uintmax_t)sz,
			    (uintmax_t)src_size);
		*mem_usage += sz;
	}
	اگر (!src->index) {
		src->index = create_delta_index(src->data, src_size);
		اگر (!src->index) {
			ایستا صحیح warned = 0;
			اگر (!warned++)
				warning(_("suboptimal pack - out of memory"));
			بازگشت 0;
		}
		*mem_usage += sizeof_delta_index(src->index);
	}

	delta_buf = create_delta(src->index, trg->data, trg_size, &delta_size, max_size);
	اگر (!delta_buf)
		بازگشت 0;

	اگر (DELTA(trg_entry)) {
		/* Prefer only shallower same-sized deltas. */
		اگر (delta_size == DELTA_SIZE(trg_entry) &&
		    src->depth + 1 >= trg->depth) {
			آزاد(delta_buf);
			بازگشت 0;
		}
	}

	/*
	 * Handle memory allocation outside of the cache
	 * accounting lock.  Compiler will optimize the strangeness
	 * away when NO_PTHREADS is defined.
	 */
	آزاد(trg_entry->delta_data);
	cache_lock();
	اگر (trg_entry->delta_data) {
		delta_cache_size -= DELTA_SIZE(trg_entry);
		trg_entry->delta_data = پوچ;
	}
	اگر (delta_cacheable(src_size, trg_size, delta_size)) {
		delta_cache_size += delta_size;
		cache_unlock();
		trg_entry->delta_data = xrealloc(delta_buf, delta_size);
	} وگرنه {
		cache_unlock();
		آزاد(delta_buf);
	}

	SET_DELTA(trg_entry, src_entry);
	SET_DELTA_SIZE(trg_entry, delta_size);
	trg->depth = src->depth + 1;

	بازگشت 1;
}

ایستا unsigned صحیح check_delta_limit(ساختار object_entry *me, unsigned صحیح n)
{
	ساختار object_entry *child = DELTA_CHILD(me);
	unsigned صحیح m = n;
	وقتی (child) {
		ثابت unsigned صحیح c = check_delta_limit(child, n + 1);
		اگر (m < c)
			m = c;
		child = DELTA_SIBLING(child);
	}
	بازگشت m;
}

ایستا size_t free_unpacked(ساختار unpacked *n)
{
	size_t freed_mem = sizeof_delta_index(n->index);
	free_delta_index(n->index);
	n->index = پوچ;
	اگر (n->data) {
		freed_mem += SIZE(n->entry);
		FREE_AND_NULL(n->data);
	}
	n->entry = پوچ;
	n->depth = 0;
	بازگشت freed_mem;
}

ایستا تهی find_deltas(ساختار object_entry **list, unsigned *list_size,
			صحیح window, صحیح depth, unsigned *processed)
{
	uint32_t i, idx = 0, شمار = 0;
	ساختار unpacked *array;
	size_t mem_usage = 0;

	CALLOC_ARRAY(array, window);

	برای (;;) {
		ساختار object_entry *entry;
		ساختار unpacked *n = array + idx;
		صحیح j, max_depth, best_base = -1;

		progress_lock();
		اگر (!*list_size) {
			progress_unlock();
			بشکن;
		}
		entry = *list++;
		(*list_size)--;
		اگر (!entry->preferred_base) {
			(*processed)++;
			display_progress(progress_state, *processed);
		}
		progress_unlock();

		mem_usage -= free_unpacked(n);
		n->entry = entry;

		وقتی (window_memory_limit &&
		       mem_usage > window_memory_limit &&
		       شمار > 1) {
			ثابت uint32_t tail = (idx + window - شمار) % window;
			mem_usage -= free_unpacked(array + tail);
			شمار--;
		}

		/* We do not compute delta to *create* objects we are not
		 * going to pack.
		 */
		اگر (entry->preferred_base)
			برو next;

		/*
		 * If the current object is at pack edge, take the depth the
		 * objects that depend on the current object into account
		 * otherwise they would become too deep.
		 */
		max_depth = depth;
		اگر (DELTA_CHILD(entry)) {
			max_depth -= check_delta_limit(entry, 0);
			اگر (max_depth <= 0)
				برو next;
		}

		j = window;
		وقتی (--j > 0) {
			صحیح ret;
			uint32_t other_idx = idx + j;
			ساختار unpacked *m;
			اگر (other_idx >= window)
				other_idx -= window;
			m = array + other_idx;
			اگر (!m->entry)
				بشکن;
			ret = try_delta(n, m, max_depth, &mem_usage);
			اگر (ret < 0)
				بشکن;
			وگرنه اگر (ret > 0)
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
		اگر (entry->delta_data && !pack_to_stdout) {
			unsigned long size;

			size = do_compress(&entry->delta_data, DELTA_SIZE(entry));
			اگر (size < (1U << OE_Z_DELTA_BITS)) {
				entry->z_delta_size = size;
				cache_lock();
				delta_cache_size -= DELTA_SIZE(entry);
				delta_cache_size += entry->z_delta_size;
				cache_unlock();
			} وگرنه {
				FREE_AND_NULL(entry->delta_data);
				entry->z_delta_size = 0;
			}
		}

		/* if we made n a delta, and if n is already at max
		 * depth, leaving it in the window is pointless.  we
		 * should evict it first.
		 */
		اگر (DELTA(entry) && max_depth <= n->depth)
			ادامه;

		/*
		 * Move the best delta base up in the window, after the
		 * currently deltified object, to keep it longer.  It will
		 * be the first base object to be attempted next.
		 */
		اگر (DELTA(entry)) {
			ساختار unpacked swap = array[best_base];
			صحیح dist = (window + idx - best_base) % window;
			صحیح dst = best_base;
			وقتی (dist--) {
				صحیح src = (dst + 1) % window;
				array[dst] = array[src];
				dst = src;
			}
			array[dst] = swap;
		}

		next:
		idx++;
		اگر (شمار + 1 < window)
			شمار++;
		اگر (idx >= window)
			idx = 0;
	}

	برای (i = 0; i < window; ++i) {
		free_delta_index(array[i].index);
		آزاد(array[i].data);
	}
	آزاد(array);
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

ساختار thread_params {
	pthread_t thread;
	ساختار object_entry **list;
	ساختار packing_region *regions;
	unsigned list_size;
	unsigned remaining;
	صحیح window;
	صحیح depth;
	صحیح working;
	صحیح data_ready;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	unsigned *processed;
};

ایستا pthread_cond_t progress_cond;

/*
 * Mutex and conditional variable can't be statically-initialized on Windows.
 */
ایستا تهی init_threaded_جستجو(تهی)
{
	pthread_mutex_init(&cache_mutex, پوچ);
	pthread_mutex_init(&progress_mutex, پوچ);
	pthread_cond_init(&progress_cond, پوچ);
}

ایستا تهی cleanup_threaded_جستجو(تهی)
{
	pthread_cond_destroy(&progress_cond);
	pthread_mutex_destroy(&cache_mutex);
	pthread_mutex_destroy(&progress_mutex);
}

ایستا تهی *threaded_find_deltas(تهی *arg)
{
	ساختار thread_params *me = arg;

	progress_lock();
	وقتی (me->remaining) {
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
		وقتی (!me->data_ready)
			pthread_cond_wait(&me->cond, &me->mutex);
		me->data_ready = 0;
		pthread_mutex_unlock(&me->mutex);

		progress_lock();
	}
	progress_unlock();
	/* leave ->working 1 so that this doesn't get more work assigned */
	بازگشت پوچ;
}

ایستا تهی ll_find_deltas(ساختار object_entry **list, unsigned list_size,
			   صحیح window, صحیح depth, unsigned *processed)
{
	ساختار thread_params *p;
	صحیح i, ret, active_threads = 0;

	init_threaded_جستجو();

	اگر (delta_جستجو_threads <= 1) {
		find_deltas(list, &list_size, window, depth, processed);
		cleanup_threaded_جستجو();
		بازگشت;
	}
	اگر (progress > pack_to_stdout)
		fprintf_ln(stderr, _("Delta compression using up to %d threads"),
			   delta_جستجو_threads);
	CALLOC_ARRAY(p, delta_جستجو_threads);

	/* Partition the work amongst work threads. */
	برای (i = 0; i < delta_جستجو_threads; i++) {
		unsigned sub_size = list_size / (delta_جستجو_threads - i);

		/* don't use too small segments or no deltas will be found */
		اگر (sub_size < 2*window && i+1 < delta_جستجو_threads)
			sub_size = 0;

		p[i].window = window;
		p[i].depth = depth;
		p[i].processed = processed;
		p[i].working = 1;
		p[i].data_ready = 0;

		/* try to split chunks on "path" boundaries */
		وقتی (sub_size && sub_size < list_size &&
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
	برای (i = 0; i < delta_جستجو_threads; i++) {
		اگر (!p[i].list_size)
			ادامه;
		pthread_mutex_init(&p[i].mutex, پوچ);
		pthread_cond_init(&p[i].cond, پوچ);
		ret = pthread_create(&p[i].thread, پوچ,
				     threaded_find_deltas, &p[i]);
		اگر (ret)
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
	وقتی (active_threads) {
		ساختار thread_params *target = پوچ;
		ساختار thread_params *victim = پوچ;
		unsigned sub_size = 0;

		progress_lock();
		برای (;;) {
			برای (i = 0; !target && i < delta_جستجو_threads; i++)
				اگر (!p[i].working)
					target = &p[i];
			اگر (target)
				بشکن;
			pthread_cond_wait(&progress_cond, &progress_mutex);
		}

		برای (i = 0; i < delta_جستجو_threads; i++)
			اگر (p[i].remaining > 2*window &&
			    (!victim || victim->remaining < p[i].remaining))
				victim = &p[i];
		اگر (victim) {
			sub_size = victim->remaining / 2;
			list = victim->list + victim->list_size - sub_size;
			وقتی (sub_size && list[0]->hash &&
			       list[0]->hash == list[-1]->hash) {
				list++;
				sub_size--;
			}
			اگر (!sub_size) {
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

		اگر (!sub_size) {
			pthread_join(target->thread, پوچ);
			pthread_cond_destroy(&target->cond);
			pthread_mutex_destroy(&target->mutex);
			active_threads--;
		}
	}
	cleanup_threaded_جستجو();
	آزاد(p);
}

ایستا صحیح obj_is_packed(ثابت ساختار object_id *oid)
{
	بازگشت packlist_find(&to_pack, oid) ||
		(reuse_packfile_bitmap &&
		 bitmap_walk_contains(bitmap_git, reuse_packfile_bitmap, oid));
}

ایستا تهی add_tag_chain(ثابت ساختار object_id *oid)
{
	ساختار tag *tag;

	/*
	 * We catch duplicates already in add_object_entry(), but we'd
	 * prefer to do this extra check to avoid having to parse the
	 * tag at all if we already know that it's being packed (e.g., if
	 * it was included via bitmaps, we would not have parsed it
	 * previously).
	 */
	اگر (obj_is_packed(oid))
		بازگشت;

	tag = lookup_tag(the_repository, oid);
	وقتی (1) {
		اگر (!tag || parse_tag(the_repository, tag) || !tag->tagged)
			die(_("unable to pack objects reachable from tag %s"),
			    oid_to_hex(oid));

		add_object_entry(&tag->object.oid, OBJ_TAG, پوچ, 0);

		اگر (tag->tagged->type != OBJ_TAG)
			بازگشت;

		tag = (ساختار tag *)tag->tagged;
	}
}

ایستا صحیح add_ref_tag(ثابت ساختار reference *ref, تهی *cb_data UNUSED)
{
	ساختار object_id peeled;

	اگر (!reference_get_peeled_oid(the_repository, ref, &peeled) &&
	    obj_is_packed(&peeled))
		add_tag_chain(ref->oid);
	بازگشت 0;
}

ایستا صحیح should_attempt_deltas(ساختار object_entry *entry)
{
	اگر (DELTA(entry))
		/* This happens if we decided to reuse existing
		 * delta from a pack. "reuse_delta &&" is implied.
		 */
		بازگشت 0;

	اگر (!entry->type_valid ||
	    oe_size_less_than(&to_pack, entry, 50))
		بازگشت 0;

	اگر (entry->no_try_delta)
		بازگشت 0;

	اگر (!entry->preferred_base) {
		اگر (oe_type(entry) < 0)
			die(_("unable to get type of object %s"),
				oid_to_hex(&entry->idx.oid));
	} وگرنه اگر (oe_type(entry) < 0) {
		/*
		 * This object is not found, but we
		 * don't have to include it anyway.
		 */
		بازگشت 0;
	}

	بازگشت 1;
}

ایستا تهی find_deltas_for_region(ساختار object_entry *list,
				   ساختار packing_region *region,
				   unsigned صحیح *processed)
{
	ساختار object_entry **delta_list;
	unsigned صحیح delta_list_nr = 0;

	ALLOC_ARRAY(delta_list, region->nr);
	برای (size_t i = 0; i < region->nr; i++) {
		ساختار object_entry *entry = list + region->start + i;
		اگر (should_attempt_deltas(entry))
			delta_list[delta_list_nr++] = entry;
	}

	QSORT(delta_list, delta_list_nr, type_size_sort);
	find_deltas(delta_list, &delta_list_nr, window, depth, processed);
	آزاد(delta_list);
}

ایستا تهی find_deltas_by_region(ساختار object_entry *list,
				  ساختار packing_region *regions,
				  size_t start, size_t nr)
{
	unsigned صحیح processed = 0;
	size_t progress_nr;

	اگر (!nr)
		بازگشت;

	progress_nr = regions[nr - 1].start + regions[nr - 1].nr;

	اگر (progress)
		progress_state = start_progress(the_repository,
						_("Compressing objects by path"),
						progress_nr);

	وقتی (nr--)
		find_deltas_for_region(list,
				       &regions[start++],
				       &processed);

	display_progress(progress_state, progress_nr);
	stop_progress(&progress_state);
}

ایستا تهی *threaded_find_deltas_by_path(تهی *arg)
{
	ساختار thread_params *me = arg;

	progress_lock();
	وقتی (me->remaining) {
		وقتی (me->remaining) {
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
		وقتی (!me->data_ready)
			pthread_cond_wait(&me->cond, &me->mutex);
		me->data_ready = 0;
		pthread_mutex_unlock(&me->mutex);

		progress_lock();
	}
	progress_unlock();
	/* leave ->working 1 so that this doesn't get more work assigned */
	بازگشت پوچ;
}

ایستا تهی ll_find_deltas_by_region(ساختار object_entry *list,
				     ساختار packing_region *regions,
				     uint32_t start, uint32_t nr)
{
	ساختار thread_params *p;
	صحیح i, ret, active_threads = 0;
	unsigned صحیح processed = 0;
	uint32_t progress_nr;
	init_threaded_جستجو();

	اگر (!nr)
		بازگشت;

	progress_nr =  regions[nr - 1].start + regions[nr - 1].nr;
	اگر (delta_جستجو_threads <= 1) {
		find_deltas_by_region(list, regions, start, nr);
		cleanup_threaded_جستجو();
		بازگشت;
	}

	اگر (progress > pack_to_stdout)
		fprintf_ln(stderr,
			   Q_("Path-based delta compression using up to %d thread",
			      "Path-based delta compression using up to %d threads",
			      delta_جستجو_threads),
			   delta_جستجو_threads);
	CALLOC_ARRAY(p, delta_جستجو_threads);

	اگر (progress)
		progress_state = start_progress(the_repository,
						_("Compressing objects by path"),
						progress_nr);
	/* Partition the work amongst work threads. */
	برای (i = 0; i < delta_جستجو_threads; i++) {
		unsigned sub_size = nr / (delta_جستجو_threads - i);

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
	برای (i = 0; i < delta_جستجو_threads; i++) {
		اگر (!p[i].list_size)
			ادامه;
		pthread_mutex_init(&p[i].mutex, پوچ);
		pthread_cond_init(&p[i].cond, پوچ);
		ret = pthread_create(&p[i].thread, پوچ,
				     threaded_find_deltas_by_path, &p[i]);
		اگر (ret)
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
	وقتی (active_threads) {
		ساختار thread_params *target = پوچ;
		ساختار thread_params *victim = پوچ;
		unsigned sub_size = 0;

		progress_lock();
		برای (;;) {
			برای (i = 0; !target && i < delta_جستجو_threads; i++)
				اگر (!p[i].working)
					target = &p[i];
			اگر (target)
				بشکن;
			pthread_cond_wait(&progress_cond, &progress_mutex);
		}

		برای (i = 0; i < delta_جستجو_threads; i++)
			اگر (p[i].remaining > 2*window &&
			    (!victim || victim->remaining < p[i].remaining))
				victim = &p[i];
		اگر (victim) {
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

		اگر (!sub_size) {
			pthread_join(target->thread, پوچ);
			pthread_cond_destroy(&target->cond);
			pthread_mutex_destroy(&target->mutex);
			active_threads--;
		}
	}
	cleanup_threaded_جستجو();
	آزاد(p);

	display_progress(progress_state, progress_nr);
	stop_progress(&progress_state);
}

ایستا تهی prepare_pack(صحیح window, صحیح depth)
{
	ساختار object_entry **delta_list;
	uint32_t i, nr_deltas;
	unsigned n;

	اگر (use_delta_islands)
		resolve_درخت_islands(the_repository, progress, &to_pack);

	get_object_details();

	/*
	 * If we're locally repacking then we need to be doubly careful
	 * from now on in order to make sure no stealth corruption gets
	 * propagated to the new pack.  Clients receiving streamed packs
	 * should validate everything they get anyway so no need to incur
	 * the additional cost here in that case.
	 */
	اگر (!pack_to_stdout)
		do_check_packed_object_crc = 1;

	اگر (!to_pack.nr_objects || !window || !depth)
		بازگشت;

	اگر (path_walk)
		ll_find_deltas_by_region(to_pack.objects, to_pack.regions,
					 0, to_pack.nr_regions);

	ALLOC_ARRAY(delta_list, to_pack.nr_objects);
	nr_deltas = n = 0;

	برای (i = 0; i < to_pack.nr_objects; i++) {
		ساختار object_entry *entry = to_pack.objects + i;

		اگر (!should_attempt_deltas(entry))
			ادامه;

		اگر (!entry->preferred_base)
			nr_deltas++;

		delta_list[n++] = entry;
	}

	اگر (nr_deltas && n > 1) {
		unsigned nr_done = 0;

		اگر (progress)
			progress_state = start_progress(the_repository,
							_("Compressing objects"),
							nr_deltas);
		QSORT(delta_list, n, type_size_sort);
		ll_find_deltas(delta_list, n, window+1, depth, &nr_done);
		stop_progress(&progress_state);
		اگر (nr_done != nr_deltas)
			die(_("inconsistency with delta count"));
	}
	آزاد(delta_list);
}

ایستا صحیح git_pack_config(ثابت نویسه *k, ثابت نویسه *v,
			   ثابت ساختار config_context *ctx, تهی *cb)
{
	اگر (!strcmp(k, "pack.window")) {
		window = git_config_int(k, v, ctx->kvi);
		بازگشت 0;
	}
	اگر (!strcmp(k, "pack.windowmemory")) {
		window_memory_limit = git_config_ulong(k, v, ctx->kvi);
		بازگشت 0;
	}
	اگر (!strcmp(k, "pack.depth")) {
		depth = git_config_int(k, v, ctx->kvi);
		بازگشت 0;
	}
	اگر (!strcmp(k, "pack.deltacachesize")) {
		max_delta_cache_size = git_config_size_t(k, v, ctx->kvi);
		بازگشت 0;
	}
	اگر (!strcmp(k, "pack.deltacachelimit")) {
		cache_max_small_delta_size = git_config_int(k, v, ctx->kvi);
		بازگشت 0;
	}
	اگر (!strcmp(k, "pack.writebitmaphashcache")) {
		اگر (git_config_bool(k, v))
			write_bitmap_options |= BITMAP_OPT_HASH_CACHE;
		وگرنه
			write_bitmap_options &= ~BITMAP_OPT_HASH_CACHE;
	}

	اگر (!strcmp(k, "pack.writebitmaplookuptable")) {
		اگر (git_config_bool(k, v))
			write_bitmap_options |= BITMAP_OPT_LOOKUP_TABLE;
		وگرنه
			write_bitmap_options &= ~BITMAP_OPT_LOOKUP_TABLE;
	}

	اگر (!strcmp(k, "pack.usebitmaps")) {
		use_bitmap_index_default = git_config_bool(k, v);
		بازگشت 0;
	}
	اگر (!strcmp(k, "pack.allowpackreuse")) {
		صحیح res = git_parse_maybe_bool_text(v);
		اگر (res < 0) {
			اگر (!strcasecmp(v, "single"))
				allow_pack_reuse = SINGLE_PACK_REUSE;
			وگرنه اگر (!strcasecmp(v, "multi"))
				allow_pack_reuse = MULTI_PACK_REUSE;
			وگرنه
				die(_("invalid pack.allowPackReuse value: '%s'"), v);
		} وگرنه اگر (res) {
			allow_pack_reuse = SINGLE_PACK_REUSE;
		} وگرنه {
			allow_pack_reuse = NO_PACK_REUSE;
		}
		بازگشت 0;
	}
	اگر (!strcmp(k, "pack.threads")) {
		delta_جستجو_threads = git_config_int(k, v, ctx->kvi);
		اگر (delta_جستجو_threads < 0)
			die(_("invalid number of threads specified (%d)"),
			    delta_جستجو_threads);
		اگر (!HAVE_THREADS && delta_جستجو_threads != 1) {
			warning(_("no threads support, ignoring %s"), k);
			delta_جستجو_threads = 0;
		}
		بازگشت 0;
	}
	اگر (!strcmp(k, "pack.indexversion")) {
		pack_idx_opts.version = git_config_int(k, v, ctx->kvi);
		اگر (pack_idx_opts.version > 2)
			die(_("bad pack.indexVersion=%"PRIu32),
			    pack_idx_opts.version);
		بازگشت 0;
	}
	اگر (!strcmp(k, "pack.writereverseindex")) {
		اگر (git_config_bool(k, v))
			pack_idx_opts.flags |= WRITE_REV;
		وگرنه
			pack_idx_opts.flags &= ~WRITE_REV;
		بازگشت 0;
	}
	اگر (!strcmp(k, "uploadpack.blobpackfileuri")) {
		ساختار configured_exclusion *ex;
		ثابت نویسه *oid_end, *pack_end;
		/*
		 * Stores the pack hash. This is not a true object ID, but is
		 * of the same form.
		 */
		ساختار object_id pack_hash;

		اگر (!v)
			بازگشت config_error_nonbool(k);

		ex = xmalloc(اندازه(*ex));
		اگر (parse_oid_hex(v, &ex->e.oid, &oid_end) ||
		    *oid_end != ' ' ||
		    parse_oid_hex(oid_end + 1, &pack_hash, &pack_end) ||
		    *pack_end != ' ')
			die(_("value of uploadpack.blobpackfileuri must be "
			      "of the form '<object-hash> <pack-hash> <uri>' (got '%s')"), v);
		اگر (oidmap_get(&configured_exclusions, &ex->e.oid))
			die(_("object already configured in another "
			      "uploadpack.blobpackfileuri (got '%s')"), v);
		ex->pack_hash_hex = xcalloc(1, pack_end - oid_end);
		memcpy(ex->pack_hash_hex, oid_end + 1, pack_end - oid_end - 1);
		ex->uri = xstrdup(pack_end + 1);
		oidmap_put(&configured_exclusions, ex);
	}
	بازگشت git_default_config(k, v, ctx, cb);
}

/* Counters for trace2 output when in --stdin-packs mode. */
ایستا صحیح stdin_packs_found_nr;
ایستا صحیح stdin_packs_hints_nr;

ایستا صحیح add_object_entry_from_pack(ثابت ساختار object_id *oid,
				      ساختار packed_git *p,
				      uint32_t pos,
				      تهی *_data)
{
	off_t ofs;
	ساختار object_info oi = OBJECT_INFO_INIT;
	شمارش object_type type = OBJ_NONE;

	display_progress(progress_state, ++nr_seen);

	اگر (have_duplicate_entry(oid, 0))
		بازگشت 0;

	stdin_packs_found_nr++;

	ofs = nth_packed_object_offset(p, pos);

	oi.typep = &type;
	اگر (packed_object_info(پوچ, p, ofs, &oi) < 0) {
		die(_("could not get type of object %s in pack %s"),
		    oid_to_hex(oid), p->pack_name);
	} وگرنه اگر (type == OBJ_COMMIT) {
		ساختار rev_info *revs = _data;
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
		add_pending_oid(revs, پوچ, oid, 0);
	}

	اگر (!want_object_in_pack(oid, 0, &p, &ofs))
		بازگشت 0;

	create_object_entry(oid, type, 0, 0, 0, p, ofs);

	بازگشت 0;
}

ایستا تهی show_object_pack_hint(ساختار object *object, ثابت نویسه *name,
				  تهی *data)
{
	شمارش stdin_packs_mode mode = *(شمارش stdin_packs_mode *)data;
	اگر (mode == STDIN_PACKS_MODE_FOLLOW) {
		اگر (object->type == OBJ_BLOB &&
		    !odb_has_object(the_repository->objects, &object->oid, 0))
			بازگشت;
		add_object_entry(&object->oid, object->type, name, 0);
	} وگرنه {
		ساختار object_entry *oe = packlist_find(&to_pack, &object->oid);
		اگر (!oe)
			بازگشت;

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

ایستا تهی show_commit_pack_hint(ساختار commit *commit, تهی *data)
{
	شمارش stdin_packs_mode mode = *(شمارش stdin_packs_mode *)data;

	اگر (mode == STDIN_PACKS_MODE_FOLLOW) {
		show_object_pack_hint((ساختار object *)commit, "", data);
		بازگشت;
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
شمارش stdin_pack_info_kind {
	STDIN_PACK_INCLUDE = (1<<0),
	STDIN_PACK_EXCLUDE_CLOSED = (1<<1),
	STDIN_PACK_EXCLUDE_OPEN = (1<<2),
};

ساختار stdin_pack_info {
	ساختار packed_git *p;
	شمارش stdin_pack_info_kind kind;
};

ایستا صحیح pack_mtime_cmp(ثابت تهی *_a, ثابت تهی *_b)
{
	ساختار stdin_pack_info *a = ((ثابت ساختار string_list_item*)_a)->util;
	ساختار stdin_pack_info *b = ((ثابت ساختار string_list_item*)_b)->util;

	/*
	 * order packs by descending mtime so that objects are laid out
	 * roughly as newest-to-oldest
	 */
	اگر (a->p->mtime < b->p->mtime)
		بازگشت 1;
	وگرنه اگر (b->p->mtime < a->p->mtime)
		بازگشت -1;
	وگرنه
		بازگشت 0;
}

ایستا صحیح stdin_packs_include_check_obj(ساختار object *obj, تهی *data UNUSED)
{
	بازگشت !has_object_kept_pack(to_pack.repo, &obj->oid,
				     KEPT_PACK_IN_CORE);
}

ایستا صحیح stdin_packs_include_check(ساختار commit *commit, تهی *data)
{
	بازگشت stdin_packs_include_check_obj((ساختار object *)commit, data);
}

ایستا تهی stdin_packs_add_pack_entries(ساختار strmap *packs,
					 ساختار rev_info *revs)
{
	ساختار string_list کلیدها = STRING_LIST_INIT_NODUP;
	ساختار string_list_item *item;
	ساختار hashmap_iter iter;
	ساختار strmap_entry *entry;

	strmap_for_each_entry(packs, &iter, entry) {
		ساختار stdin_pack_info *info = entry->مقدار;
		اگر (!info->p)
			die(_("could not find pack '%s'"), entry->کلید);

		string_list_append(&کلیدها, entry->کلید)->util = info;
	}

	/*
	 * Order packs by ascending mtime; use QSORT directly to access the
	 * string_list_item's ->util pointer, which string_list_sort() does not
	 * provide.
	 */
	QSORT(کلیدها.اقلام, کلیدها.nr, pack_mtime_cmp);

	for_each_string_list_item(item, &کلیدها) {
		ساختار stdin_pack_info *info = item->util;

		اگر (info->kind & STDIN_PACK_EXCLUDE_OPEN) {
			/*
			 * When open-excluded packs ("!") are present, stop
			 * the parent walk at closed-excluded ("^") packs.
			 * Objects behind a "^" boundary are guaranteed to
			 * have closure and should not be rescued.
			 */
			revs->include_check = stdin_packs_include_check;
			revs->include_check_obj = stdin_packs_include_check_obj;
		}

		اگر ((info->kind & STDIN_PACK_INCLUDE) ||
		    (info->kind & STDIN_PACK_EXCLUDE_OPEN))
			for_each_object_in_pack(info->p,
						add_object_entry_from_pack,
						revs,
						ODB_FOR_EACH_OBJECT_PACK_ORDER);
	}

	string_list_clear(&کلیدها, 0);
}

ایستا تهی stdin_packs_read_درون_داد(ساختار rev_info *revs,
				   شمارش stdin_packs_mode mode)
{
	ساختار strbuf buf = STRBUF_INIT;
	ساختار strmap packs = STRMAP_INIT;
	ساختار packed_git *p;

	وقتی (strbuf_getline(&buf, stdin) != EOF) {
		ساختار stdin_pack_info *info;
		شمارش stdin_pack_info_kind kind = STDIN_PACK_INCLUDE;
		ثابت نویسه *کلید = buf.buf;

		اگر (!*کلید)
			ادامه;
		وگرنه اگر (*کلید == '^')
			kind = STDIN_PACK_EXCLUDE_CLOSED;
		وگرنه اگر (*کلید == '!' && mode == STDIN_PACKS_MODE_FOLLOW)
			kind = STDIN_PACK_EXCLUDE_OPEN;

		اگر (kind != STDIN_PACK_INCLUDE)
			کلید++;

		info = strmap_get(&packs, کلید);
		اگر (!info) {
			CALLOC_ARRAY(info, 1);
			strmap_put(&packs, کلید, info);
		}

		info->kind |= kind;

		strbuf_reset(&buf);
	}

	repo_for_each_pack(the_repository, p) {
		ساختار stdin_pack_info *info;

		info = strmap_get(&packs, pack_basename(p));
		اگر (!info)
			ادامه;

		اگر (info->kind & STDIN_PACK_INCLUDE) {
			اگر (exclude_promisor_objects && p->pack_promisor)
				die(_("packfile %s is a promisor but --exclude-promisor-objects was given"), p->pack_name);

			/*
			 * Arguments we got on stdin may not even be
			 * packs. First check that to avoid segfaulting
			 * later on in e.g.  pack_mtime_cmp(), excluded
			 * packs are handled below.
			 */
			اگر (!is_pack_valid(p))
				die(_("packfile %s cannot be accessed"), p->pack_name);
		}

		اگر (info->kind & STDIN_PACK_EXCLUDE_CLOSED) {
			/*
			 * Marking excluded packs as kept in-core so
			 * that later calls to add_object_entry()
			 * discards any objects that are also found in
			 * excluded packs.
			 */
			p->pack_keep_in_core = 1;
		}

		اگر (info->kind & STDIN_PACK_EXCLUDE_OPEN) {
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

ایستا تهی add_unreachable_loose_objects(ساختار rev_info *revs);

ایستا تهی read_stdin_packs(ساختار repository *repo,
			     شمارش stdin_packs_mode mode, صحیح rev_list_unpacked)
{
	صحیح prev_fetch_if_missing = repo->fetch_if_missing;
	ساختار rev_info revs;

	/*
	 * The revision walk may hit objects that are promised, only. As the
	 * walk is best-effort though we don't want to perform backfill fetches
	 * for them.
	 */
	repo->fetch_if_missing = 0;

	repo_init_revisions(repo, &revs, پوچ);
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
	revs.درخت_objects = 1;
	revs.tag_objects = 1;
	revs.ignore_missing_links = 1;
	revs.exclude_promisor_objects = exclude_promisor_objects;

	/* avoids adding objects in excluded packs */
	ignore_packed_keep_in_core = 1;
	اگر (mode == STDIN_PACKS_MODE_FOLLOW) {
		/*
		 * In '--stdin-packs=follow' mode, additionally ignore
		 * objects in excluded-open packs to prevent them from
		 * appearing in the resulting pack.
		 */
		ignore_packed_keep_in_core_open = 1;
	}
	stdin_packs_read_درون_داد(&revs, mode);
	اگر (rev_list_unpacked)
		add_unreachable_loose_objects(&revs);

	اگر (prepare_revision_walk(&revs))
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

ایستا تهی add_cruft_object_entry(ثابت ساختار object_id *oid, شمارش object_type type,
				   ساختار packed_git *pack, off_t offset,
				   ثابت نویسه *name, uint32_t mtime)
{
	ساختار object_entry *entry;

	display_progress(progress_state, ++nr_seen);

	entry = packlist_find(&to_pack, oid);
	اگر (entry) {
		اگر (name) {
			entry->hash = pack_name_hash_fn(name);
			entry->no_try_delta = no_try_delta(name);
		}
	} وگرنه {
		اگر (!want_object_in_pack_mtime(oid, 0, &pack, &offset, mtime))
			بازگشت;
		اگر (!pack && type == OBJ_BLOB) {
			ساختار odb_source_files *files =
				odb_source_files_downcast(the_repository->objects->source);
			صحیح found = 0;

			برای (ساختار odb_files_dir *dir = files->dirs; !found && dir; dir = dir->next)
				اگر (!odb_source_read_object_info(&dir->loose->base, oid, پوچ, 0, پوچ))
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
			اگر (!found)
				بازگشت;
		}

		entry = create_object_entry(oid, type, pack_name_hash_fn(name),
					    0, name && no_try_delta(name),
					    pack, offset);
	}

	اگر (mtime > oe_cruft_mtime(&to_pack, entry))
		oe_set_cruft_mtime(&to_pack, entry, mtime);
	بازگشت;
}

ایستا تهی show_cruft_object(ساختار object *obj, ثابت نویسه *name, تهی *data UNUSED)
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
	add_cruft_object_entry(&obj->oid, obj->type, پوچ, 0, name, cruft_expiration);
}

ایستا تهی show_cruft_commit(ساختار commit *commit, تهی *data)
{
	show_cruft_object((ساختار object*)commit, پوچ, data);
}

ایستا صحیح cruft_include_check_obj(ساختار object *obj, تهی *data UNUSED)
{
	بازگشت !has_object_kept_pack(to_pack.repo, &obj->oid, KEPT_PACK_IN_CORE);
}

ایستا صحیح cruft_include_check(ساختار commit *commit, تهی *data)
{
	بازگشت cruft_include_check_obj((ساختار object*)commit, data);
}

ایستا تهی set_cruft_mtime(ثابت ساختار object *object,
			    ساختار packed_git *pack,
			    off_t offset, time_t mtime)
{
	add_cruft_object_entry(&object->oid, object->type, pack, offset, پوچ,
			       mtime);
}

ایستا تهی mark_pack_kept_in_core(ساختار string_list *packs, unsigned keep)
{
	ساختار string_list_item *item = پوچ;
	for_each_string_list_item(item, packs) {
		ساختار packed_git *p = item->util;
		اگر (!p)
			die(_("could not find pack '%s'"), item->string);
		اگر (p->is_cruft && keep)
			ignore_packed_keep_in_core_has_cruft = 1;
		p->pack_keep_in_core = keep;
	}
}

ایستا تهی add_objects_in_unpacked_packs(تهی);

ایستا تهی enumerate_cruft_objects(تهی)
{
	اگر (progress)
		progress_state = start_progress(the_repository,
						_("Enumerating cruft objects"), 0);

	add_objects_in_unpacked_packs();
	add_unreachable_loose_objects(پوچ);

	stop_progress(&progress_state);
}

ایستا تهی enumerate_and_traverse_cruft_objects(ساختار string_list *fresh_packs)
{
	ساختار packed_git *p;
	ساختار rev_info revs;
	صحیح ret;

	repo_init_revisions(the_repository, &revs, پوچ);

	revs.tag_objects = 1;
	revs.درخت_objects = 1;
	revs.blob_objects = 1;

	revs.include_check = cruft_include_check;
	revs.include_check_obj = cruft_include_check_obj;

	revs.ignore_missing_links = 1;

	اگر (progress)
		progress_state = start_progress(the_repository,
						_("Enumerating cruft objects"), 0);
	ret = add_unseen_recent_objects_to_traversal(&revs, cruft_expiration,
						     set_cruft_mtime, 1);
	stop_progress(&progress_state);

	اگر (ret)
		die(_("unable to add cruft objects"));

	/*
	 * Re-mark only the fresh packs as kept so that objects in
	 * unknown packs do not halt the reachability traversal early.
	 */
	repo_for_each_pack(the_repository, p)
		p->pack_keep_in_core = 0;
	mark_pack_kept_in_core(fresh_packs, 1);

	اگر (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	اگر (progress)
		progress_state = start_progress(the_repository,
						_("Traversing cruft objects"), 0);
	nr_seen = 0;
	traverse_commit_list(&revs, show_cruft_commit, show_cruft_object, پوچ);

	stop_progress(&progress_state);
	release_revisions(&revs);
}

ایستا تهی read_cruft_objects(تهی)
{
	ساختار strbuf buf = STRBUF_INIT;
	ساختار string_list discard_packs = STRING_LIST_INIT_DUP;
	ساختار string_list fresh_packs = STRING_LIST_INIT_DUP;
	ساختار packed_git *p;

	ignore_packed_keep_in_core = 1;

	وقتی (strbuf_getline(&buf, stdin) != EOF) {
		اگر (!buf.len)
			ادامه;

		اگر (*buf.buf == '-')
			string_list_append(&discard_packs, buf.buf + 1);
		وگرنه
			string_list_append(&fresh_packs, buf.buf);
	}

	string_list_sort(&discard_packs);
	string_list_sort(&fresh_packs);

	repo_for_each_pack(the_repository, p) {
		ثابت نویسه *pack_name = pack_basename(p);
		ساختار string_list_item *item;

		item = string_list_lookup(&fresh_packs, pack_name);
		اگر (!item)
			item = string_list_lookup(&discard_packs, pack_name);

		اگر (item) {
			item->util = p;
		} وگرنه {
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

	اگر (cruft_expiration)
		enumerate_and_traverse_cruft_objects(&fresh_packs);
	وگرنه
		enumerate_cruft_objects();

	strbuf_release(&buf);
	string_list_clear(&discard_packs, 0);
	string_list_clear(&fresh_packs, 0);
}

ایستا تهی read_object_list_from_stdin(تهی)
{
	نویسه سطر[GIT_MAX_HEXSZ + 1 + PATH_MAX + 2];
	ساختار object_id oid;
	ثابت نویسه *p;

	برای (;;) {
		اگر (!fgets(سطر, اندازه(سطر), stdin)) {
			اگر (feof(stdin))
				بشکن;
			اگر (!ferror(stdin))
				BUG("fgets returned NULL, not EOF, not error!");
			اگر (errno != EINTR)
				die_errno("fgets");
			clearerr(stdin);
			ادامه;
		}
		اگر (سطر[0] == '-') {
			اگر (get_oid_hex(سطر+1, &oid))
				die(_("expected edge object ID, got garbage:\n %s"),
				    سطر);
			add_preferred_base(&oid);
			ادامه;
		}
		اگر (parse_oid_hex(سطر, &oid, &p))
			die(_("expected object ID, got garbage:\n %s"), سطر);

		add_preferred_base_object(p + 1);
		add_object_entry(&oid, OBJ_NONE, p + 1, 0);
	}
}

ایستا تهی show_commit(ساختار commit *commit, تهی *data UNUSED)
{
	add_object_entry(&commit->object.oid, OBJ_COMMIT, پوچ, 0);

	اگر (write_bitmap_index)
		index_commit_for_bitmap(commit);

	اگر (use_delta_islands)
		propagate_island_marks(the_repository, commit);
}

ایستا تهی show_object(ساختار object *obj, ثابت نویسه *name,
			تهی *data UNUSED)
{
	add_preferred_base_object(name);
	add_object_entry(&obj->oid, obj->type, name, 0);

	اگر (use_delta_islands)
		record_درخت_depth(&obj->oid, name);
}

ایستا تهی show_object__ma_allow_any(ساختار object *obj, ثابت نویسه *name, تهی *data)
{
	أكد(arg_missing_action == MA_ALLOW_ANY);

	/*
	 * Quietly ignore ALL missing objects.  This avoids problems with
	 * staging them now and getting an odd error later.
	 */
	اگر (!odb_has_object(the_repository->objects, &obj->oid, 0))
		بازگشت;

	show_object(obj, name, data);
}

ایستا تهی show_object__ma_allow_promisor(ساختار object *obj, ثابت نویسه *name, تهی *data)
{
	أكد(arg_missing_action == MA_ALLOW_PROMISOR);

	/*
	 * Quietly ignore EXPECTED missing objects.  This avoids problems with
	 * staging them now and getting an odd error later.
	 */
	اگر (!odb_has_object(the_repository->objects, &obj->oid, 0) &&
	    is_promisor_object(to_pack.repo, &obj->oid))
		بازگشت;

	show_object(obj, name, data);
}

ایستا صحیح option_parse_missing_action(ثابت ساختار option *opt,
				       ثابت نویسه *arg, صحیح unset)
{
	ساختار repository *repo = opt->مقدار;

	أكد(arg);
	أكد(!unset);

	اگر (!strcmp(arg, "error")) {
		arg_missing_action = MA_ERROR;
		fn_show_object = show_object;
		بازگشت 0;
	}

	اگر (!strcmp(arg, "allow-any")) {
		arg_missing_action = MA_ALLOW_ANY;
		repo->fetch_if_missing = 0;
		fn_show_object = show_object__ma_allow_any;
		بازگشت 0;
	}

	اگر (!strcmp(arg, "allow-promisor")) {
		arg_missing_action = MA_ALLOW_PROMISOR;
		repo->fetch_if_missing = 0;
		fn_show_object = show_object__ma_allow_promisor;
		بازگشت 0;
	}

	die(_("invalid value for '%s': '%s'"), "--missing", arg);
	بازگشت 0;
}

ایستا تهی show_edge(ساختار commit *commit)
{
	add_preferred_base(&commit->object.oid);
}

ایستا صحیح add_object_in_unpacked_pack(ثابت ساختار object_id *oid,
				       ساختار object_info *oi,
				       تهی *data UNUSED)
{
	اگر (cruft) {
		add_cruft_object_entry(oid, OBJ_NONE, oi->source_infop->u.packed.pack,
				       oi->source_infop->u.packed.offset, پوچ,
				       *oi->mtimep);
	} وگرنه {
		add_object_entry(oid, OBJ_NONE, "", 0);
	}
	بازگشت 0;
}

ایستا تهی add_objects_in_unpacked_packs(تهی)
{
	ساختار odb_source_files *files =
		odb_source_files_downcast(to_pack.repo->objects->source);
	time_t mtime;
	ساختار odb_for_each_object_options opts = {
		.flags = ODB_FOR_EACH_OBJECT_PACK_ORDER |
			 ODB_FOR_EACH_OBJECT_LOCAL_ONLY |
			 ODB_FOR_EACH_OBJECT_SKIP_IN_CORE_KEPT_PACKS |
			 ODB_FOR_EACH_OBJECT_SKIP_ON_DISK_KEPT_PACKS,
	};
	ساختار odb_source_info source_info;
	ساختار object_info oi = {
		.mtimep = &mtime,
		.source_infop = &source_info,
	};

	برای (ساختار odb_files_dir *dir = files->dirs; dir; dir = dir->next) {
		اگر (!dir->local)
			ادامه;

		اگر (odb_source_for_each_object(&dir->packed->base, &oi,
					       add_object_in_unpacked_pack, پوچ, &opts))
			die(_("cannot open pack index"));
	}
}

ایستا صحیح add_loose_object(ثابت ساختار object_id *oid, ثابت نویسه *path,
			    تهی *data)
{
	ساختار rev_info *revs = data;
	شمارش object_type type = odb_read_object_info(the_repository->objects, oid, پوچ);

	اگر (type < 0) {
		warning(_("loose object at %s could not be examined"), path);
		بازگشت 0;
	}

	اگر (cruft) {
		ساختار stat st;
		اگر (stat(path, &st) < 0) {
			اگر (errno == ENOENT)
				بازگشت 0;
			بازگشت error_errno("unable to stat %s", oid_to_hex(oid));
		}

		add_cruft_object_entry(oid, type, پوچ, 0, پوچ,
				       st.st_mtime);
	} وگرنه {
		add_object_entry(oid, type, "", 0);
	}

	اگر (revs && type == OBJ_COMMIT)
		add_pending_oid(revs, پوچ, oid, 0);

	بازگشت 0;
}

/*
 * We actually don't even have to worry about reachability here.
 * add_object_entry will weed out duplicates, so we just add every
 * loose object we find.
 */
ایستا تهی add_unreachable_loose_objects(ساختار rev_info *revs)
{
	for_each_loose_file_in_source(the_repository->objects->source,
				      add_loose_object, پوچ, پوچ, revs);
}

ایستا صحیح has_sha1_pack_kept_or_nonlocal(ثابت ساختار object_id *oid)
{
	ایستا ساختار packed_git *last_found = پوچ;
	ساختار packed_git *p;

	اگر (last_found && find_pack_entry_one(oid, last_found))
		بازگشت 1;

	repo_for_each_pack(the_repository, p) {
		/*
		 * We have already checked `last_found`, so there is no need to
		 * re-check here.
		 */
		اگر (p == last_found)
			ادامه;

		اگر ((!p->pack_local || p->pack_keep || p->pack_keep_in_core) &&
		    find_pack_entry_one(oid, p)) {
			last_found = p;
			بازگشت 1;
		}
	}

	بازگشت 0;
}

/*
 * Store a list of sha1s that are should not be discarded
 * because they are either written too recently, or are
 * reachable from another object that was.
 *
 * This is filled by get_object_list.
 */
ایستا ساختار oid_array recent_objects;

ایستا صحیح loosened_object_can_be_discarded(ثابت ساختار object_id *oid,
					    timestamp_t mtime)
{
	اگر (!unpack_unreachable_expiration)
		بازگشت 0;
	اگر (mtime > unpack_unreachable_expiration)
		بازگشت 0;
	اگر (oid_array_lookup(&recent_objects, oid) >= 0)
		بازگشت 0;
	بازگشت 1;
}

ایستا صحیح force_object_loose(ساختار odb_source *source,
			      ثابت ساختار object_id *oid,
			      ثابت time_t *mtime)
{
	ساختار odb_source_files *files = odb_source_files_downcast(source);
	ثابت ساختار git_hash_algo *compat = source->odb->repo->compat_hash_algo;
	ساختار object_info oi = OBJECT_INFO_INIT;
	ساختار object_id compat_oid, *compat_oid_p = پوچ;
	شمارش object_type type;
	تهی *buf = پوچ;
	size_t len;
	صحیح ret;

	برای (ساختار odb_files_dir *dir = files->dirs; dir; dir = dir->next)
		اگر (!odb_source_read_object_info(&dir->loose->base, oid, پوچ, 0, پوچ))
			بازگشت 0;

	oi.typep = &type;
	oi.sizep = &len;
	oi.contentp = &buf;
	اگر (odb_read_object_info_extended(source->odb, oid, &oi, 0)) {
		ret = error(_("cannot read object for %s"), oid_to_hex(oid));
		برو out;
	}

	اگر (compat) {
		اگر (repo_oid_to_algop(source->odb->repo, oid, compat, &compat_oid)) {
			ret = error(_("cannot map object %s to %s"),
				    oid_to_hex(oid), compat->name);
			برو out;
		}

		compat_oid_p = &compat_oid;
	}

	ret = odb_source_write_object(&files->dirs->loose->base, buf, len, type, oid,
				      compat_oid_p, mtime, 0);

out:
	آزاد(buf);
	بازگشت ret;
}

ایستا تهی loosen_unused_packed_objects(تهی)
{
	ساختار packed_git *p;
	uint32_t i;
	uint32_t loosened_objects_nr = 0;
	ساختار object_id oid;

	repo_for_each_pack(the_repository, p) {
		اگر (!p->pack_local || p->pack_keep || p->pack_keep_in_core)
			ادامه;

		اگر (open_pack_index(p))
			die(_("cannot open pack index"));

		برای (i = 0; i < p->رقم_objects; i++) {
			nth_packed_object_id(&oid, p, i);
			اگر (!packlist_find(&to_pack, &oid) &&
			    !has_sha1_pack_kept_or_nonlocal(&oid) &&
			    !loosened_object_can_be_discarded(&oid, p->mtime)) {
				اگر (force_object_loose(the_repository->objects->source,
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
ایستا صحیح pack_options_allow_reuse(تهی)
{
	بازگشت allow_pack_reuse != NO_PACK_REUSE &&
	       pack_to_stdout &&
	       !ignore_packed_keep_on_disk &&
	       !ignore_packed_keep_in_core &&
	       (!local || !have_non_local_packs) &&
	       !incremental;
}

ایستا صحیح get_object_list_from_bitmap(ساختار rev_info *revs)
{
	اگر (!(bitmap_git = prepare_bitmap_walk(revs, 0)))
		بازگشت -1;

	/*
	 * For now, force the name-hash version to be 1 since that
	 * is the version implied by the bitmap format. Later, the
	 * format can include this version explicitly in its format,
	 * allowing readers to know the version that was used during
	 * the bitmap write.
	 */
	name_hash_version = 1;

	اگر (pack_options_allow_reuse())
		reuse_partial_packfile_from_bitmap(bitmap_git,
						   &reuse_packfiles,
						   &reuse_packfiles_nr,
						   &reuse_packfile_bitmap,
						   allow_pack_reuse == MULTI_PACK_REUSE);

	اگر (reuse_packfiles) {
		reuse_packfile_objects = bitmap_popcount(reuse_packfile_bitmap);
		اگر (!reuse_packfile_objects)
			BUG("expected non-empty reuse bitmap");

		nr_result += reuse_packfile_objects;
		nr_seen += reuse_packfile_objects;
		display_progress(progress_state, nr_seen);
	}

	traverse_bitmap_commit_list(bitmap_git, revs,
				    &add_object_entry_from_bitmap);
	بازگشت 0;
}

ایستا تهی record_recent_object(ساختار object *obj,
				 ثابت نویسه *name UNUSED,
				 تهی *data UNUSED)
{
	oid_array_append(&recent_objects, &obj->oid);
}

ایستا تهی record_recent_commit(ساختار commit *commit, تهی *data UNUSED)
{
	oid_array_append(&recent_objects, &commit->object.oid);
}

ایستا صحیح mark_bitmap_preferred_tip(ثابت ساختار reference *ref, تهی *data UNUSED)
{
	ثابت ساختار object_id *maybe_peeled = ref->oid;
	ساختار object_id peeled;
	ساختار object *object;

	اگر (!reference_get_peeled_oid(the_repository, ref, &peeled))
		maybe_peeled = &peeled;

	object = parse_object_or_die(the_repository, maybe_peeled, ref->name);
	اگر (object->type == OBJ_COMMIT)
		object->flags |= NEEDS_BITMAP;

	بازگشت 0;
}

ایستا درونخطی صحیح is_oid_uninteresting(ساختار repository *repo,
				       ساختار object_id *oid)
{
	ساختار object *o = lookup_object(repo, oid);
	بازگشت !o || (o->flags & UNINTERESTING);
}

ایستا صحیح add_objects_by_path(ثابت نویسه *path,
			       ساختار oid_array *oids,
			       شمارش object_type type,
			       تهی *data)
{
	size_t oe_start = to_pack.nr_objects;
	size_t oe_end;
	unsigned صحیح *processed = data;

	/*
	 * First, add all objects to the packing data, including the ones
	 * marked UNINTERESTING (translated to 'exclude') as they can be
	 * used as delta bases.
	 */
	برای (size_t i = 0; i < oids->nr; i++) {
		صحیح exclude;
		ساختار object_info oi = OBJECT_INFO_INIT;
		ساختار object_id *oid = &oids->oid[i];

		/* Skip objects that do not exist locally. */
		اگر ((exclude_promisor_objects || arg_missing_action != MA_ERROR) &&
		    odb_read_object_info_extended(the_repository->objects, oid, &oi,
						  OBJECT_INFO_FOR_PREFETCH) < 0)
			ادامه;

		exclude = is_oid_uninteresting(the_repository, oid);

		اگر (exclude && !thin)
			ادامه;

		add_object_entry(oid, type, path, exclude);

		اگر (type == OBJ_COMMIT) {
			ساختار commit *commit;

			اگر (!write_bitmap_index && !use_delta_islands)
				ادامه;

			commit = lookup_commit(the_repository, oid);
			اگر (!commit)
				die(_("could not find commit %s"), oid_to_hex(oid));
			اگر (write_bitmap_index)
				index_commit_for_bitmap(commit);
			/*
			 * Skip island propagation for boundary commits.
			 * The regular traversal's show_commit() is only
			 * called for interesting commits; matching that
			 * here keeps path-walk from doing extra work that
			 * would only be a no-op anyway (boundary commits
			 * are not in island_marks).
			 */
			اگر (use_delta_islands && !exclude)
				propagate_island_marks(the_repository, commit);
		} وگرنه اگر (type == OBJ_درخت && use_delta_islands) {
			record_درخت_depth(oid, path);
		}
	}

	oe_end = to_pack.nr_objects;

	/* We can skip delta calculations if it is a no-op. */
	اگر (oe_end == oe_start || !window)
		بازگشت 0;

	ALLOC_GROW(to_pack.regions,
		   to_pack.nr_regions + 1,
		   to_pack.nr_regions_alloc);

	to_pack.regions[to_pack.nr_regions].start = oe_start;
	to_pack.regions[to_pack.nr_regions].nr = oe_end - oe_start;
	to_pack.nr_regions++;

	*processed += oids->nr;
	display_progress(progress_state, *processed);

	بازگشت 0;
}

ایستا صحیح get_object_list_path_walk(ساختار rev_info *revs)
{
	ساختار path_walk_info info = PATH_WALK_INFO_INIT;
	unsigned صحیح processed = 0;
	صحیح result;

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

	بازگشت result;
}

ایستا تهی get_object_list(ساختار rev_info *revs, ساختار strvec *argv)
{
	ساختار setup_revision_opt s_r_opt = {
		.allow_exclude_promisor_objects = 1,
	};
	ساختار repo_config_مقادیر *cfg = repo_config_مقادیر(the_repository);
	نویسه سطر[1000];
	صحیح flags = 0;
	صحیح save_warning;

	save_commit_buffer = 0;
	setup_revisions_from_strvec(argv, revs, &s_r_opt);

	/* make sure shallows are read */
	is_repository_shallow(the_repository);

	save_warning = cfg->warn_on_object_refname_ambiguity;
	cfg->warn_on_object_refname_ambiguity = 0;

	وقتی (fgets(سطر, اندازه(سطر), stdin) != پوچ) {
		صحیح len = strlen(سطر);
		اگر (len && سطر[len - 1] == '\n')
			سطر[--len] = 0;
		اگر (!len)
			بشکن;
		اگر (*سطر == '-') {
			اگر (!strcmp(سطر, "--not")) {
				flags ^= UNINTERESTING;
				write_bitmap_index = 0;
				ادامه;
			}
			اگر (starts_with(سطر, "--shallow ")) {
				ساختار object_id oid;
				اگر (get_oid_hex(سطر + 10, &oid))
					die("not an object name '%s'", سطر + 10);
				register_shallow(the_repository, &oid);
				use_bitmap_index = 0;
				ادامه;
			}
			die(_("not a rev '%s'"), سطر);
		}
		اگر (handle_revision_arg(سطر, revs, flags, REVARG_CANNOT_BE_FILENAME))
			die(_("bad revision '%s'"), سطر);
	}

	cfg->warn_on_object_refname_ambiguity = save_warning;

	اگر (use_bitmap_index && !get_object_list_from_bitmap(revs))
		بازگشت;

	اگر (use_delta_islands)
		load_delta_islands(the_repository, progress);

	اگر (write_bitmap_index)
		for_each_preferred_bitmap_tip(the_repository, mark_bitmap_preferred_tip,
					      پوچ);

	اگر (!fn_show_object)
		fn_show_object = show_object;

	اگر (path_walk) {
		اگر (get_object_list_path_walk(revs)) {
			warning(_("failed to pack objects via path-walk"));
			path_walk = 0;
		}
	}

	اگر (!path_walk) {
		اگر (prepare_revision_walk(revs))
			die(_("revision walk setup failed"));
		mark_edges_uninteresting(revs, show_edge, sparse);
		traverse_commit_list(revs,
				show_commit, fn_show_object,
				پوچ);
	}

	اگر (unpack_unreachable_expiration) {
		revs->ignore_missing_links = 1;
		اگر (add_unseen_recent_objects_to_traversal(revs,
				unpack_unreachable_expiration, پوچ, 0))
			die(_("unable to add recent objects"));
		اگر (prepare_revision_walk(revs))
			die(_("revision walk setup failed"));
		traverse_commit_list(revs, record_recent_commit,
				     record_recent_object, پوچ);
	}

	اگر (keep_unreachable)
		add_objects_in_unpacked_packs();
	اگر (pack_loose_unreachable)
		add_unreachable_loose_objects(پوچ);
	اگر (unpack_unreachable)
		loosen_unused_packed_objects();

	oid_array_clear(&recent_objects);
}

ایستا تهی add_extra_kept_packs(ثابت ساختار string_list *names)
{
	ساختار packed_git *p;

	اگر (!names->nr)
		بازگشت;

	repo_for_each_pack(the_repository, p) {
		ثابت نویسه *name = basename(p->pack_name);
		صحیح i;

		اگر (!p->pack_local)
			ادامه;

		برای (i = 0; i < names->nr; i++)
			اگر (!fspathcmp(name, names->اقلام[i].string))
				بشکن;

		اگر (i < names->nr) {
			p->pack_keep_in_core = 1;
			ignore_packed_keep_in_core = 1;
			ادامه;
		}
	}
}

ایستا صحیح option_parse_quiet(ثابت ساختار option *opt, ثابت نویسه *arg,
			      صحیح unset)
{
	صحیح *val = opt->مقدار;

	BUG_ON_OPT_ARG(arg);

	اگر (!unset)
		*val = 0;
	وگرنه اگر (!*val)
		*val = 1;
	بازگشت 0;
}

ایستا صحیح option_parse_index_version(ثابت ساختار option *opt,
				      ثابت نویسه *arg, صحیح unset)
{
	ساختار pack_idx_option *popts = opt->مقدار;
	نویسه *c;
	ثابت نویسه *val = arg;

	BUG_ON_OPT_NEG(unset);

	popts->version = strtoul(val, &c, 10);
	اگر (popts->version > 2)
		die(_("unsupported index version %s"), val);
	اگر (*c == ',' && c[1])
		popts->off32_limit = strtoul(c+1, &c, 0);
	اگر (*c || popts->off32_limit & 0x80000000)
		die(_("bad index version '%s'"), val);
	بازگشت 0;
}

ایستا صحیح option_parse_unpack_unreachable(ثابت ساختار option *opt UNUSED,
					   ثابت نویسه *arg, صحیح unset)
{
	اگر (unset) {
		unpack_unreachable = 0;
		unpack_unreachable_expiration = 0;
	}
	وگرنه {
		unpack_unreachable = 1;
		اگر (arg)
			unpack_unreachable_expiration = approxidate(arg);
	}
	بازگشت 0;
}

ایستا صحیح option_parse_cruft_expiration(ثابت ساختار option *opt UNUSED,
					 ثابت نویسه *arg, صحیح unset)
{
	اگر (unset) {
		cruft = 0;
		cruft_expiration = 0;
	} وگرنه {
		cruft = 1;
		اگر (arg)
			cruft_expiration = approxidate(arg);
	}
	بازگشت 0;
}

ایستا صحیح is_not_in_promisor_pack_obj(ساختار object *obj, تهی *data UNUSED)
{
	ساختار odb_source_info source_info;
	ساختار object_info info = {
		.source_infop = &source_info,
	};

	اگر (odb_read_object_info_extended(the_repository->objects, &obj->oid, &info, 0))
		BUG("should_include_obj should only be called on existing objects");
	بازگشت source_info.source->type != ODB_SOURCE_PACKED || !source_info.u.packed.pack->pack_promisor;
}

ایستا صحیح is_not_in_promisor_pack(ساختار commit *commit, تهی *data) {
	بازگشت is_not_in_promisor_pack_obj((ساختار object *) commit, data);
}

ایستا صحیح parse_stdin_packs_mode(ثابت ساختار option *opt, ثابت نویسه *arg,
				  صحیح unset)
{
	شمارش stdin_packs_mode *mode = opt->مقدار;

	اگر (unset)
		*mode = STDIN_PACKS_MODE_NONE;
	وگرنه اگر (!arg || !*arg)
		*mode = STDIN_PACKS_MODE_STANDARD;
	وگرنه اگر (!strcmp(arg, "follow"))
		*mode = STDIN_PACKS_MODE_FOLLOW;
	وگرنه
		die(_("invalid value for '%s': '%s'"), opt->long_name, arg);

	بازگشت 0;
}

صحیح cmd_pack_objects(صحیح argc,
		     ثابت نویسه **argv,
		     ثابت نویسه *prefix,
		     ساختار repository *repo)
{
	صحیح use_internal_rev_list = 0;
	صحیح all_progress_implied = 0;
	ساختار strvec rp = STRVEC_INIT;
	صحیح rev_list_unpacked = 0, rev_list_all = 0, rev_list_reflog = 0;
	صحیح rev_list_index = 0;
	شمارش stdin_packs_mode stdin_packs = STDIN_PACKS_MODE_NONE;
	ساختار string_list keep_pack_list = STRING_LIST_INIT_NODUP;
	ساختار list_objects_filter_options filter_options =
		LIST_OBJECTS_FILTER_INIT;
	ساختار repo_config_مقادیر *cfg = repo_config_مقادیر(the_repository);

	ساختار option pack_objects_options[] = {
		OPT_CALLBACK_F('q', "quiet", &progress, پوچ,
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
		OPT_INTEGER(0, "threads", &delta_جستجو_threads,
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
		OPT_CALLBACK_F(0, "unpack-unreachable", پوچ, N_("time"),
		  N_("unpack unreachable objects newer than <time>"),
		  PARSE_OPT_OPTARG, option_parse_unpack_unreachable),
		OPT_BOOL(0, "cruft", &cruft, N_("create a cruft pack")),
		OPT_CALLBACK_F(0, "cruft-expiration", پوچ, N_("time"),
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

	اگر (DFS_رقم_STATES > (1 << OE_DFS_STATE_BITS))
		BUG("too many dfs states, increase OE_DFS_STATE_BITS");

	disable_replace_refs();

	sparse = git_env_bool("GIT_TEST_PACK_SPARSE", -1);
	اگر (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		اگر (sparse < 0)
			sparse = the_repository->settings.pack_use_sparse;
		اگر (the_repository->settings.pack_use_multi_pack_reuse)
			allow_pack_reuse = MULTI_PACK_REUSE;
	}

	reset_pack_idx_option(&pack_idx_opts);
	pack_idx_opts.flags |= WRITE_REV;
	repo_config(the_repository, git_pack_config, پوچ);
	اگر (git_env_bool(GIT_TEST_NO_WRITE_REV_INDEX, 0))
		pack_idx_opts.flags &= ~WRITE_REV;

	progress = isatty(2);
	argc = parse_options(argc, argv, prefix, pack_objects_options,
			     pack_usage, 0);

	اگر (argc) {
		base_name = argv[0];
		argc--;
	}
	اگر (pack_to_stdout != !base_name || argc)
		usage_with_options(pack_usage, pack_objects_options);

	اگر (path_walk < 0) {
		اگر (use_bitmap_index > 0 ||
		    !use_internal_rev_list)
			path_walk = 0;
		وگرنه اگر (the_repository->gitdir &&
			 the_repository->settings.pack_use_path_walk)
			path_walk = 1;
		وگرنه
			path_walk = git_env_bool("GIT_TEST_PACK_PATH_WALK", 0);
	}

	اگر (depth < 0)
		depth = 0;
	اگر (depth >= (1 << OE_DEPTH_BITS)) {
		warning(_("delta chain depth %d is too deep, forcing %d"),
			depth, (1 << OE_DEPTH_BITS) - 1);
		depth = (1 << OE_DEPTH_BITS) - 1;
	}
	اگر (cache_max_small_delta_size >= (1U << OE_Z_DELTA_BITS)) {
		warning(_("pack.deltaCacheLimit is too high, forcing %d"),
			(1U << OE_Z_DELTA_BITS) - 1);
		cache_max_small_delta_size = (1U << OE_Z_DELTA_BITS) - 1;
	}
	اگر (window < 0)
		window = 0;

	strvec_افزودن(&rp, "pack-objects");

	اگر (path_walk) {
		ثابت نویسه *option = پوچ;
		اگر (!path_walk_filter_compatible(&filter_options))
			option = "--filter";

		اگر (option) {
			warning(_("cannot use %s with %s"),
				option, "--path-walk");
			path_walk = 0;
		}
	}
	اگر (path_walk) {
		strvec_افزودن(&rp, "--objects");
	} وگرنه اگر (thin) {
		use_internal_rev_list = 1;
		strvec_افزودن(&rp, shallow
				? "--objects-edge-aggressive"
				: "--objects-edge");
	} وگرنه
		strvec_افزودن(&rp, "--objects");

	اگر (rev_list_all) {
		use_internal_rev_list = 1;
		strvec_افزودن(&rp, "--all");
	}
	اگر (rev_list_reflog) {
		use_internal_rev_list = 1;
		strvec_افزودن(&rp, "--reflog");
	}
	اگر (rev_list_index) {
		use_internal_rev_list = 1;
		strvec_افزودن(&rp, "--indexed-objects");
	}
	اگر (rev_list_unpacked && !stdin_packs) {
		use_internal_rev_list = 1;
		strvec_افزودن(&rp, "--unpacked");
	}

	die_for_incompatible_opt2(exclude_promisor_objects,
				  "--exclude-promisor-objects",
				  exclude_promisor_objects_best_effort,
				  "--exclude-promisor-objects-best-effort");
	اگر (exclude_promisor_objects) {
		repo->fetch_if_missing = 0;

		/* --stdin-packs handles promisor objects separately. */
		اگر (!stdin_packs) {
			use_internal_rev_list = 1;
			strvec_افزودن(&rp, "--exclude-promisor-objects");
		}
	} وگرنه اگر (exclude_promisor_objects_best_effort) {
		use_internal_rev_list = 1;
		arg_missing_action = MA_ALLOW_ANY;
		repo->fetch_if_missing = 0;
		fn_show_object = show_object__ma_allow_any;
		/* revs configured below */
	}
	اگر (unpack_unreachable || keep_unreachable || pack_loose_unreachable)
		use_internal_rev_list = 1;

	اگر (!reuse_object)
		reuse_delta = 0;
	اگر (cfg->pack_compression_level == -1)
		cfg->pack_compression_level = Z_DEFAULT_COMPRESSION;
	وگرنه اگر (cfg->pack_compression_level < 0 || cfg->pack_compression_level > Z_BEST_COMPRESSION)
		die(_("bad pack compression level %d"), cfg->pack_compression_level);

	اگر (!delta_جستجو_threads)	/* --threads=0 means autodetect */
		delta_جستجو_threads = online_cpus();

	اگر (!HAVE_THREADS && delta_جستجو_threads != 1)
		warning(_("no threads support, ignoring --threads"));
	اگر (!pack_to_stdout && !pack_size_limit)
		pack_size_limit = pack_size_limit_cfg;
	اگر (pack_to_stdout && pack_size_limit)
		die(_("--max-pack-size cannot be used to build a pack for transfer"));
	اگر (pack_size_limit && pack_size_limit < 1024*1024) {
		warning(_("minimum pack size limit is 1 MiB"));
		pack_size_limit = 1024*1024;
	}

	اگر (!pack_to_stdout && thin)
		die(_("--thin cannot be used to build an indexable pack"));

	die_for_incompatible_opt2(keep_unreachable, "--keep-unreachable",
				  unpack_unreachable, "--unpack-unreachable");
	اگر (!rev_list_all || !rev_list_reflog || !rev_list_index)
		unpack_unreachable_expiration = 0;

	die_for_incompatible_opt2(stdin_packs, "--stdin-packs",
				  filter_options.choice, "--filter");


	اگر (stdin_packs && use_internal_rev_list)
		die(_("cannot use internal rev list with --stdin-packs"));

	اگر (cruft) {
		اگر (use_internal_rev_list)
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
	اگر (!pack_to_stdout)
		use_bitmap_index_default = 0;

	اگر (use_bitmap_index < 0)
		use_bitmap_index = use_bitmap_index_default;

	/* "hard" reasons not to use bitmaps; these just won't work at all */
	اگر (!use_internal_rev_list || (!pack_to_stdout && write_bitmap_index) || is_repository_shallow(the_repository))
		use_bitmap_index = 0;

	اگر (pack_to_stdout || !rev_list_all)
		write_bitmap_index = 0;

	اگر (name_hash_version < 0)
		name_hash_version = (صحیح)git_env_ulong("GIT_TEST_NAME_HASH_VERSION", 1);

	validate_name_hash_version();

	اگر (use_delta_islands)
		strvec_افزودن(&rp, "--topo-order");

	اگر (progress && all_progress_implied)
		progress = 2;

	add_extra_kept_packs(&keep_pack_list);
	اگر (ignore_packed_keep_on_disk) {
		ساختار packed_git *p;

		repo_for_each_pack(the_repository, p)
			اگر (p->pack_local && p->pack_keep)
				بشکن;
		اگر (!p) /* no keep-able packs found */
			ignore_packed_keep_on_disk = 0;
	}
	اگر (local) {
		/*
		 * unlike ignore_packed_keep_on_disk above, we do not
		 * want to unset "local" based on looking at packs, as
		 * it also covers non-local objects
		 */
		ساختار packed_git *p;

		repo_for_each_pack(the_repository, p) {
			اگر (!p->pack_local) {
				have_non_local_packs = 1;
				بشکن;
			}
		}
	}

	trace2_region_enter("pack-objects", "enumerate-objects",
			    the_repository);
	prepare_packing_data(the_repository, &to_pack);

	اگر (progress && !cruft)
		progress_state = start_progress(the_repository,
						_("Enumerating objects"), 0);
	اگر (stdin_packs) {
		read_stdin_packs(repo, stdin_packs, rev_list_unpacked);
	} وگرنه اگر (cruft) {
		read_cruft_objects();
	} وگرنه اگر (!use_internal_rev_list) {
		read_object_list_from_stdin();
	} وگرنه {
		ساختار rev_info revs;

		repo_init_revisions(the_repository, &revs, پوچ);
		list_objects_filter_copy(&revs.filter, &filter_options);
		اگر (exclude_promisor_objects_best_effort) {
			revs.include_check = is_not_in_promisor_pack;
			revs.include_check_obj = is_not_in_promisor_pack_obj;
		}
		get_object_list(&revs, &rp);
		release_revisions(&revs);
	}
	cleanup_preferred_base();
	اگر (include_tag && nr_result)
		refs_for_each_tag_ref(get_main_ref_store(the_repository),
				      add_ref_tag, پوچ);
	stop_progress(&progress_state);
	trace2_region_leave("pack-objects", "enumerate-objects",
			    the_repository);

	اگر (non_empty && !nr_result)
		برو cleanup;
	اگر (nr_result) {
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

	اگر (progress)
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

	بازگشت 0;
}
