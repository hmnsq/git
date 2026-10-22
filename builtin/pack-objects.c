#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "environment.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "hex.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "attr.h"
#அந்தர்பாவ "object.h"
#அந்தர்பாவ "commit.h"
#அந்தர்பாவ "tag.h"
#அந்தர்பாவ "delta.h"
#அந்தர்பாவ "pack.h"
#அந்தர்பாவ "pack-revindex.h"
#அந்தர்பாவ "csum-file.h"
#அந்தர்பாவ "tree-walk.h"
#அந்தர்பாவ "diff.h"
#அந்தர்பாவ "revision.h"
#அந்தர்பாவ "list-objects.h"
#அந்தர்பாவ "list-objects-filter-options.h"
#அந்தர்பாவ "pack-objects.h"
#அந்தர்பாவ "progress.h"
#அந்தர்பாவ "refs.h"
#அந்தர்பாவ "thread-utils.h"
#அந்தர்பாவ "pack-bitmap.h"
#அந்தர்பாவ "delta-islands.h"
#அந்தர்பாவ "reachable.h"
#அந்தர்பாவ "oid-array.h"
#அந்தர்பாவ "strvec.h"
#அந்தர்பாவ "strmap.h"
#அந்தர்பாவ "list.h"
#அந்தர்பாவ "packfile.h"
#அந்தர்பாவ "object-file.h"
#அந்தர்பாவ "object-file-convert.h"
#அந்தர்பாவ "odb.h"
#அந்தர்பாவ "odb/streaming.h"
#அந்தர்பாவ "replace-object.h"
#அந்தர்பாவ "dir.h"
#அந்தர்பாவ "midx.h"
#அந்தர்பாவ "trace2.h"
#அந்தர்பாவ "shallow.h"
#அந்தர்பாவ "promisor-remote.h"
#அந்தர்பாவ "pack-mtimes.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "pkt-line.h"
#அந்தர்பாவ "blob.h"
#அந்தர்பாவ "tree.h"
#அந்தர்பாவ "path-walk.h"

/*
 * Objects we are going to pack are collected in the `to_pack` structure.
 * It contains an array (dynamically expanded) of the object data, and a map
 * that can resolve SHA1s to their position in the array.
 */
நிலையான கட்டமைப்பு packing_data to_pack;

நிலையான உள்வரி கட்டமைப்பு object_entry *oe_delta(
		மாறிலி கட்டமைப்பு packing_data *pack,
		மாறிலி கட்டமைப்பு object_entry *e)
{
	எனில் (!e->delta_idx)
		திரும்பு வெற்று;
	எனில் (e->ext_base)
		திரும்பு &pack->ext_bases[e->delta_idx - 1];
	இல்லையெனில்
		திரும்பு &pack->objects[e->delta_idx - 1];
}

நிலையான உள்வரி size_t oe_delta_size(கட்டமைப்பு packing_data *pack,
				   மாறிலி கட்டமைப்பு object_entry *e)
{
	எனில் (e->delta_size_valid)
		திரும்பு e->delta_size_;

	/*
	 * pack->delta_size[] can't be NULL because oe_set_delta_size()
	 * must have been called when a new delta is saved with
	 * oe_set_delta().
	 * If oe_delta() returns NULL (i.e. default state, which means
	 * delta_size_valid is also false), then the caller must never
	 * call oe_delta_size().
	 */
	திரும்பு pack->delta_size[e - pack->objects];
}

size_t oe_get_size_slow(கட்டமைப்பு packing_data *pack,
			மாறிலி கட்டமைப்பு object_entry *e);

நிலையான உள்வரி size_t oe_size(கட்டமைப்பு packing_data *pack,
			     மாறிலி கட்டமைப்பு object_entry *e)
{
	எனில் (e->size_valid)
		திரும்பு e->size_;

	திரும்பு oe_get_size_slow(pack, e);
}

நிலையான உள்வரி வெறுமை oe_set_delta(கட்டமைப்பு packing_data *pack,
				கட்டமைப்பு object_entry *e,
				கட்டமைப்பு object_entry *delta)
{
	எனில் (delta)
		e->delta_idx = (delta - pack->objects) + 1;
	இல்லையெனில்
		e->delta_idx = 0;
}

நிலையான உள்வரி கட்டமைப்பு object_entry *oe_delta_sibling(
		மாறிலி கட்டமைப்பு packing_data *pack,
		மாறிலி கட்டமைப்பு object_entry *e)
{
	எனில் (e->delta_sibling_idx)
		திரும்பு &pack->objects[e->delta_sibling_idx - 1];
	திரும்பு வெற்று;
}

நிலையான உள்வரி கட்டமைப்பு object_entry *oe_delta_child(
		மாறிலி கட்டமைப்பு packing_data *pack,
		மாறிலி கட்டமைப்பு object_entry *e)
{
	எனில் (e->delta_child_idx)
		திரும்பு &pack->objects[e->delta_child_idx - 1];
	திரும்பு வெற்று;
}

நிலையான உள்வரி வெறுமை oe_set_delta_child(கட்டமைப்பு packing_data *pack,
				      கட்டமைப்பு object_entry *e,
				      கட்டமைப்பு object_entry *delta)
{
	எனில் (delta)
		e->delta_child_idx = (delta - pack->objects) + 1;
	இல்லையெனில்
		e->delta_child_idx = 0;
}

நிலையான உள்வரி வெறுமை oe_set_delta_sibling(கட்டமைப்பு packing_data *pack,
					கட்டமைப்பு object_entry *e,
					கட்டமைப்பு object_entry *delta)
{
	எனில் (delta)
		e->delta_sibling_idx = (delta - pack->objects) + 1;
	இல்லையெனில்
		e->delta_sibling_idx = 0;
}

நிலையான உள்வரி வெறுமை oe_set_size(கட்டமைப்பு packing_data *pack,
			       கட்டமைப்பு object_entry *e,
			       size_t size)
{
	எனில் (size < pack->oe_size_limit) {
		e->size_ = size;
		e->size_valid = 1;
	} இல்லையெனில் {
		e->size_valid = 0;
		எனில் (oe_get_size_slow(pack, e) != size)
			BUG("'size' is supposed to be the object size!");
	}
}

நிலையான உள்வரி வெறுமை oe_set_delta_size(கட்டமைப்பு packing_data *pack,
				     கட்டமைப்பு object_entry *e,
				     size_t size)
{
	எனில் (size < pack->oe_delta_size_limit) {
		e->delta_size_ = size;
		e->delta_size_valid = 1;
	} இல்லையெனில் {
		packing_data_lock(pack);
		எனில் (!pack->delta_size)
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

நிலையான மாறிலி எழுத்து *மாறிலி pack_usage[] = {
	N_("git pack-objects [-q | --progress | --all-progress] [--all-progress-implied]\n"
	   "                 [--no-reuse-delta] [--delta-base-offset] [--non-empty]\n"
	   "                 [--local] [--incremental] [--window=<n>] [--depth=<n>]\n"
	   "                 [--revs [--unpacked | --all]] [--keep-pack=<pack-name>]\n"
	   "                 [--cruft] [--cruft-expiration=<time>]\n"
	   "                 [--stdout [--filter=<filter-spec>] | <base-name>]\n"
	   "                 [--shallow] [--keep-true-parents] [--[no-]sparse]\n"
	   "                 [--name-hash-version=<n>] [--path-walk] < <object-list>"),
	வெற்று
};

நிலையான கட்டமைப்பு pack_idx_entry **written_list;
நிலையான uint32_t nr_result, nr_written, nr_seen;
நிலையான கட்டமைப்பு bitmap_index *bitmap_git;
நிலையான uint32_t write_layer;

நிலையான முழு non_empty;
நிலையான முழு reuse_delta = 1, reuse_object = 1;
நிலையான முழு keep_unreachable, unpack_unreachable, include_tag;
நிலையான timestamp_t unpack_unreachable_expiration;
நிலையான முழு pack_loose_unreachable;
நிலையான முழு cruft;
நிலையான முழு shallow = 0;
நிலையான timestamp_t cruft_expiration;
நிலையான முழு local;
நிலையான முழு have_non_local_packs;
நிலையான முழு incremental;
நிலையான முழு ignore_packed_keep_on_disk;
நிலையான முழு ignore_packed_keep_in_core;
நிலையான முழு ignore_packed_keep_in_core_open;
நிலையான முழு ignore_packed_keep_in_core_has_cruft;
நிலையான முழு allow_ofs_delta;
நிலையான கட்டமைப்பு pack_idx_option pack_idx_opts;
நிலையான மாறிலி எழுத்து *base_name;
நிலையான முழு progress = 1;
நிலையான முழு window = 10;
நிலையான unsigned long pack_size_limit;
நிலையான முழு depth = 50;
நிலையான முழு delta_தேடல்_threads;
நிலையான முழு pack_to_stdout;
நிலையான முழு sparse;
நிலையான முழு thin;
நிலையான முழு path_walk = -1;
நிலையான முழு இலக்கம்_preferred_base;
நிலையான கட்டமைப்பு progress *progress_state;

நிலையான கட்டமைப்பு bitmapped_pack *reuse_packfiles;
நிலையான size_t reuse_packfiles_nr;
நிலையான size_t reuse_packfiles_used_nr;
நிலையான uint32_t reuse_packfile_objects;
நிலையான கட்டமைப்பு bitmap *reuse_packfile_bitmap;

நிலையான முழு use_bitmap_index_default = 1;
நிலையான முழு use_bitmap_index = -1;
நிலையான எண்ணிக்கை {
	NO_PACK_REUSE = 0,
	SINGLE_PACK_REUSE,
	MULTI_PACK_REUSE,
} allow_pack_reuse = SINGLE_PACK_REUSE;
நிலையான எண்ணிக்கை {
	WRITE_BITMAP_FALSE = 0,
	WRITE_BITMAP_QUIET,
	WRITE_BITMAP_TRUE,
} write_bitmap_index;
நிலையான uint16_t write_bitmap_options = BITMAP_OPT_HASH_CACHE;

நிலையான முழு exclude_promisor_objects;
நிலையான முழு exclude_promisor_objects_best_effort;

நிலையான முழு use_delta_islands;

நிலையான size_t delta_cache_size = 0;
நிலையான size_t max_delta_cache_size = DEFAULT_DELTA_CACHE_SIZE;
நிலையான unsigned long cache_max_small_delta_size = 1000;

நிலையான unsigned long window_memory_limit = 0;

நிலையான கட்டமைப்பு string_list uri_protocols = STRING_LIST_INIT_NODUP;

எண்ணிக்கை missing_action {
	MA_ERROR = 0,      /* fail if any missing objects are encountered */
	MA_ALLOW_ANY,      /* silently allow ALL missing objects */
	MA_ALLOW_PROMISOR, /* silently allow all missing PROMISOR objects */
};
நிலையான எண்ணிக்கை missing_action arg_missing_action;
நிலையான show_object_fn fn_show_object;

கட்டமைப்பு configured_exclusion {
	கட்டமைப்பு oidmap_entry e;
	எழுத்து *pack_hash_hex;
	எழுத்து *uri;
};
நிலையான கட்டமைப்பு oidmap configured_exclusions;

நிலையான கட்டமைப்பு oidset excluded_by_config;
நிலையான முழு name_hash_version = -1;

எண்ணிக்கை stdin_packs_mode {
	STDIN_PACKS_MODE_NONE,
	STDIN_PACKS_MODE_STANDARD,
	STDIN_PACKS_MODE_FOLLOW,
};

/**
 * Check whether the name_hash_version chosen by user input is appropriate,
 * and also validate whether it is compatible with other features.
 */
நிலையான வெறுமை validate_name_hash_version(வெறுமை)
{
	எனில் (name_hash_version < 1 || name_hash_version > 2)
		die(_("invalid --name-hash-version option: %d"), name_hash_version);
	எனில் (write_bitmap_index && name_hash_version != 1) {
		warning(_("currently, --write-bitmap-index requires --name-hash-version=1"));
		name_hash_version = 1;
	}
}

நிலையான உள்வரி uint32_t pack_name_hash_fn(மாறிலி எழுத்து *name)
{
	நிலையான முழு seen_version = -1;

	எனில் (seen_version < 0)
		seen_version = name_hash_version;
	இல்லையெனில் எனில் (seen_version != name_hash_version)
		BUG("name hash version changed from %d to %d mid-process",
		    seen_version, name_hash_version);

	தேர்வு (name_hash_version) {
	நிலை 1:
		திரும்பு pack_name_hash(name);

	நிலை 2:
		திரும்பு pack_name_hash_v2((மாறிலி unsigned எழுத்து *)name);

	இயல்பு:
		BUG("invalid name-hash version: %d", name_hash_version);
	}
}

/*
 * stats
 */
நிலையான uint32_t written, written_delta;
நிலையான uint32_t reused, reused_delta;

/*
 * Indexed commits
 */
நிலையான கட்டமைப்பு commit **indexed_commits;
நிலையான unsigned முழு indexed_commits_nr;
நிலையான unsigned முழு indexed_commits_alloc;

நிலையான வெறுமை index_commit_for_bitmap(கட்டமைப்பு commit *commit)
{
	எனில் (indexed_commits_nr >= indexed_commits_alloc) {
		indexed_commits_alloc = (indexed_commits_alloc + 32) * 2;
		REALLOC_ARRAY(indexed_commits, indexed_commits_alloc);
	}

	indexed_commits[indexed_commits_nr++] = commit;
}

நிலையான வெறுமை *get_delta(கட்டமைப்பு object_entry *entry)
{
	unsigned long size, base_size;
	size_t delta_size;
	வெறுமை *buf, *base_buf, *delta_buf;
	எண்ணிக்கை object_type type;
	size_t size_st = 0, base_size_st = 0;

	buf = odb_read_object(the_repository->objects, &entry->idx.oid,
			      &type, &size_st);
	size = cast_size_t_to_ulong(size_st);
	எனில் (!buf)
		die(_("unable to read %s"), oid_to_hex(&entry->idx.oid));
	base_buf = odb_read_object(the_repository->objects,
				   &DELTA(entry)->idx.oid, &type,
				   &base_size_st);
	base_size = cast_size_t_to_ulong(base_size_st);
	எனில் (!base_buf)
		die("unable to read %s",
		    oid_to_hex(&DELTA(entry)->idx.oid));
	delta_buf = diff_delta(base_buf, base_size,
			       buf, size, &delta_size, 0);
	/*
	 * We successfully computed this delta once but dropped it for
	 * memory reasons. Something is very wrong if this time we
	 * recompute and create a different delta.
	 */
	எனில் (!delta_buf || delta_size != DELTA_SIZE(entry))
		BUG("delta size changed");
	விடுவி(buf);
	விடுவி(base_buf);
	திரும்பு delta_buf;
}

நிலையான unsigned long do_compress(வெறுமை **pptr, unsigned long size)
{
	git_zstream stream;
	வெறுமை *in, *out;
	unsigned long maxsize;
	கட்டமைப்பு repo_config_மதிப்புகள் *cfg = repo_config_மதிப்புகள்(the_repository);

	git_deflate_init(&stream, cfg->pack_compression_level);
	maxsize = git_deflate_bound(&stream, size);

	in = *pptr;
	out = xmalloc(maxsize);
	*pptr = out;

	stream.next_in = in;
	stream.avail_in = size;
	stream.next_out = out;
	stream.avail_out = maxsize;
	போது (git_deflate(&stream, Z_FINISH) == Z_OK)
		; /* nothing */
	git_deflate_end(&stream);

	விடுவி(in);
	திரும்பு stream.total_out;
}

நிலையான unsigned long write_large_blob_data(கட்டமைப்பு odb_stream *st, கட்டமைப்பு hashfile *f,
					   மாறிலி கட்டமைப்பு object_id *oid)
{
	git_zstream stream;
	unsigned எழுத்து ibuf[1024 * 16];
	unsigned எழுத்து obuf[1024 * 16];
	unsigned long olen = 0;
	கட்டமைப்பு repo_config_மதிப்புகள் *cfg = repo_config_மதிப்புகள்(the_repository);

	git_deflate_init(&stream, cfg->pack_compression_level);

	க்கு (;;) {
		ssize_t readlen;
		முழு zret = Z_OK;
		readlen = odb_stream_read(st, ibuf, அளவு(ibuf));
		எனில் (readlen == -1)
			die(_("unable to read %s"), oid_to_hex(oid));

		stream.next_in = ibuf;
		stream.avail_in = readlen;
		போது ((stream.avail_in || readlen == 0) &&
		       (zret == Z_OK || zret == Z_BUF_ERROR)) {
			stream.next_out = obuf;
			stream.avail_out = அளவு(obuf);
			zret = git_deflate(&stream, readlen ? 0 : Z_FINISH);
			hashwrite(f, obuf, stream.next_out - obuf);
			olen += stream.next_out - obuf;
		}
		எனில் (stream.avail_in)
			die(_("deflate error (%d)"), zret);
		எனில் (readlen == 0) {
			எனில் (zret != Z_STREAM_END)
				die(_("deflate error (%d)"), zret);
			நிறுத்து;
		}
	}
	git_deflate_end(&stream);
	திரும்பு olen;
}

/*
 * we are going to reuse the existing object data as is.  make
 * sure it is not corrupt.
 */
நிலையான முழு check_pack_inflate(கட்டமைப்பு packed_git *p,
		கட்டமைப்பு pack_window **w_curs,
		off_t offset,
		off_t len,
		size_t expect)
{
	git_zstream stream;
	unsigned எழுத்து fakebuf[4096], *in;
	முழு st;

	memset(&stream, 0, அளவு(stream));
	git_inflate_init(&stream);
	செய் {
		in = use_pack(p, w_curs, offset, &stream.avail_in);
		stream.next_in = in;
		stream.next_out = fakebuf;
		stream.avail_out = அளவு(fakebuf);
		st = git_inflate(&stream, Z_FINISH);
		offset += stream.next_in - in;
	} போது (st == Z_OK || st == Z_BUF_ERROR);
	git_inflate_end(&stream);
	திரும்பு (st == Z_STREAM_END &&
		stream.total_out == expect &&
		stream.total_in == len) ? 0 : -1;
}

நிலையான வெறுமை copy_pack_data(கட்டமைப்பு hashfile *f,
		கட்டமைப்பு packed_git *p,
		கட்டமைப்பு pack_window **w_curs,
		off_t offset,
		off_t len)
{
	unsigned எழுத்து *in;
	size_t avail;

	போது (len) {
		in = use_pack(p, w_curs, offset, &avail);
		எனில் (avail > len)
			avail = (unsigned long)len;
		hashwrite(f, in, avail);
		offset += avail;
		len -= avail;
	}
}

நிலையான உள்வரி முழு oe_size_greater_than(கட்டமைப்பு packing_data *pack,
				       மாறிலி கட்டமைப்பு object_entry *lhs,
				       size_t rhs)
{
	எனில் (lhs->size_valid)
		திரும்பு lhs->size_ > rhs;
	எனில் (rhs < pack->oe_size_limit) /* rhs < 2^x <= lhs ? */
		திரும்பு 1;
	திரும்பு oe_get_size_slow(pack, lhs) > rhs;
}

/* Return 0 if we will bust the pack-size limit */
நிலையான unsigned long write_no_reuse_object(கட்டமைப்பு hashfile *f, கட்டமைப்பு object_entry *entry,
					   unsigned long limit, முழு usable_delta)
{
	unsigned long size, datalen;
	unsigned எழுத்து header[MAX_PACK_OBJECT_HEADER],
		      dheader[MAX_PACK_OBJECT_HEADER];
	unsigned hdrlen;
	எண்ணிக்கை object_type type;
	வெறுமை *buf;
	கட்டமைப்பு odb_stream *st = வெற்று;
	மாறிலி unsigned hashsz = the_hash_algo->rawsz;

	எனில் (!usable_delta) {
		எனில் (oe_type(entry) == OBJ_BLOB &&
		    oe_size_greater_than(&to_pack, entry,
					 repo_settings_get_big_file_threshold(the_repository)) &&
		    (st = odb_stream_from_object(the_repository->objects, &entry->idx.oid,
						 வெற்று)) != வெற்று) {
			buf = வெற்று;
			type = st->type;
			size = st->size;
		} இல்லையெனில் {
			size_t size_st = 0;
			buf = odb_read_object(the_repository->objects,
					      &entry->idx.oid, &type,
					      &size_st);
			size = cast_size_t_to_ulong(size_st);
			எனில் (!buf)
				die(_("unable to read %s"),
				    oid_to_hex(&entry->idx.oid));
		}
		/*
		 * make sure no cached delta data remains from a
		 * previous attempt before a pack split occurred.
		 */
		FREE_AND_NULL(entry->delta_data);
		entry->z_delta_size = 0;
	} இல்லையெனில் எனில் (entry->delta_data) {
		size = DELTA_SIZE(entry);
		buf = entry->delta_data;
		entry->delta_data = வெற்று;
		type = (allow_ofs_delta && DELTA(entry)->idx.offset) ?
			OBJ_OFS_DELTA : OBJ_REF_DELTA;
	} இல்லையெனில் {
		buf = get_delta(entry);
		size = DELTA_SIZE(entry);
		type = (allow_ofs_delta && DELTA(entry)->idx.offset) ?
			OBJ_OFS_DELTA : OBJ_REF_DELTA;
	}

	எனில் (st)	/* large blob case, just assume we don't compress well */
		datalen = size;
	இல்லையெனில் எனில் (entry->z_delta_size)
		datalen = entry->z_delta_size;
	இல்லையெனில்
		datalen = do_compress(&buf, size);

	/*
	 * The object header is a byte of 'type' followed by zero or
	 * more bytes of length.
	 */
	hdrlen = encode_in_pack_object_header(header, அளவு(header),
					      type, size);

	எனில் (type == OBJ_OFS_DELTA) {
		/*
		 * Deltas with relative base contain an additional
		 * encoding of the relative offset for the delta
		 * base from this object's position in the pack.
		 */
		off_t ofs = entry->idx.offset - DELTA(entry)->idx.offset;
		unsigned pos = அளவு(dheader) - 1;
		dheader[pos] = ofs & 127;
		போது (ofs >>= 7)
			dheader[--pos] = 128 | (--ofs & 127);
		எனில் (limit && hdrlen + அளவு(dheader) - pos + datalen + hashsz >= limit) {
			எனில் (st)
				odb_stream_close(st);
			விடுவி(buf);
			திரும்பு 0;
		}
		hashwrite(f, header, hdrlen);
		hashwrite(f, dheader + pos, அளவு(dheader) - pos);
		hdrlen += அளவு(dheader) - pos;
	} இல்லையெனில் எனில் (type == OBJ_REF_DELTA) {
		/*
		 * Deltas with a base reference contain
		 * additional bytes for the base object ID.
		 */
		எனில் (limit && hdrlen + hashsz + datalen + hashsz >= limit) {
			எனில் (st)
				odb_stream_close(st);
			விடுவி(buf);
			திரும்பு 0;
		}
		hashwrite(f, header, hdrlen);
		hashwrite(f, DELTA(entry)->idx.oid.hash, hashsz);
		hdrlen += hashsz;
	} இல்லையெனில் {
		எனில் (limit && hdrlen + datalen + hashsz >= limit) {
			எனில் (st)
				odb_stream_close(st);
			விடுவி(buf);
			திரும்பு 0;
		}
		hashwrite(f, header, hdrlen);
	}
	எனில் (st) {
		datalen = write_large_blob_data(st, f, &entry->idx.oid);
		odb_stream_close(st);
	} இல்லையெனில் {
		hashwrite(f, buf, datalen);
		விடுவி(buf);
	}

	திரும்பு hdrlen + datalen;
}

/* Return 0 if we will bust the pack-size limit */
நிலையான off_t write_reuse_object(கட்டமைப்பு hashfile *f, கட்டமைப்பு object_entry *entry,
				unsigned long limit, முழு usable_delta)
{
	கட்டமைப்பு packed_git *p = IN_PACK(entry);
	கட்டமைப்பு pack_window *w_curs = வெற்று;
	uint32_t pos;
	off_t offset, cur;
	எண்ணிக்கை object_type type = oe_type(entry);
	எண்ணிக்கை object_type in_pack_type;
	off_t datalen;
	unsigned எழுத்து header[MAX_PACK_OBJECT_HEADER],
		      dheader[MAX_PACK_OBJECT_HEADER];
	unsigned hdrlen;
	மாறிலி unsigned hashsz = the_hash_algo->rawsz;
	size_t entry_size;

	cur = entry->in_pack_offset;
	in_pack_type = unpack_object_header(p, &w_curs, &cur, &entry_size);
	எனில் (in_pack_type < 0)
		die(_("write_reuse_object: unable to parse object header of %s"),
		    oid_to_hex(&entry->idx.oid));

	எனில் (DELTA(entry))
		type = (allow_ofs_delta && DELTA(entry)->idx.offset) ?
			OBJ_OFS_DELTA : OBJ_REF_DELTA;
	hdrlen = encode_in_pack_object_header(header, அளவு(header),
					      type, entry_size);

	offset = entry->in_pack_offset;
	எனில் (offset_to_pack_pos(p, offset, &pos) < 0)
		die(_("write_reuse_object: could not locate %s, expected at "
		      "offset %"PRIuMAX" in pack %s"),
		    oid_to_hex(&entry->idx.oid), (uintmax_t)offset,
		    p->pack_name);
	datalen = pack_pos_to_offset(p, pos + 1) - offset;
	எனில் (!pack_to_stdout && p->index_version > 1 &&
	    check_pack_crc(p, &w_curs, offset, datalen,
			   pack_pos_to_index(p, pos))) {
		error(_("bad packed object CRC for %s"),
		      oid_to_hex(&entry->idx.oid));
		unuse_pack(&w_curs);
		திரும்பு write_no_reuse_object(f, entry, limit, usable_delta);
	}

	offset += entry->in_pack_header_size;
	datalen -= entry->in_pack_header_size;

	எனில் (!pack_to_stdout && p->index_version == 1 &&
	    check_pack_inflate(p, &w_curs, offset, datalen, entry_size)) {
		error(_("corrupt packed object for %s"),
		      oid_to_hex(&entry->idx.oid));
		unuse_pack(&w_curs);
		திரும்பு write_no_reuse_object(f, entry, limit, usable_delta);
	}

	எனில் (type == OBJ_OFS_DELTA) {
		off_t ofs = entry->idx.offset - DELTA(entry)->idx.offset;
		unsigned pos = அளவு(dheader) - 1;
		dheader[pos] = ofs & 127;
		போது (ofs >>= 7)
			dheader[--pos] = 128 | (--ofs & 127);
		எனில் (limit && hdrlen + அளவு(dheader) - pos + datalen + hashsz >= limit) {
			unuse_pack(&w_curs);
			திரும்பு 0;
		}
		hashwrite(f, header, hdrlen);
		hashwrite(f, dheader + pos, அளவு(dheader) - pos);
		hdrlen += அளவு(dheader) - pos;
		reused_delta++;
	} இல்லையெனில் எனில் (type == OBJ_REF_DELTA) {
		எனில் (limit && hdrlen + hashsz + datalen + hashsz >= limit) {
			unuse_pack(&w_curs);
			திரும்பு 0;
		}
		hashwrite(f, header, hdrlen);
		hashwrite(f, DELTA(entry)->idx.oid.hash, hashsz);
		hdrlen += hashsz;
		reused_delta++;
	} இல்லையெனில் {
		எனில் (limit && hdrlen + datalen + hashsz >= limit) {
			unuse_pack(&w_curs);
			திரும்பு 0;
		}
		hashwrite(f, header, hdrlen);
	}
	copy_pack_data(f, p, &w_curs, offset, datalen);
	unuse_pack(&w_curs);
	reused++;
	திரும்பு hdrlen + datalen;
}

/* Return 0 if we will bust the pack-size limit */
நிலையான off_t write_object(கட்டமைப்பு hashfile *f,
			  கட்டமைப்பு object_entry *entry,
			  off_t write_offset)
{
	unsigned long limit;
	off_t len;
	முழு usable_delta, to_reuse;

	எனில் (!pack_to_stdout)
		crc32_begin(f);

	/* apply size limit if limited packsize and not first object */
	எனில் (!pack_size_limit || !nr_written)
		limit = 0;
	இல்லையெனில் எனில் (pack_size_limit <= write_offset)
		/*
		 * the earlier object did not fit the limit; avoid
		 * mistaking this with unlimited (i.e. limit = 0).
		 */
		limit = 1;
	இல்லையெனில்
		limit = pack_size_limit - write_offset;

	எனில் (!DELTA(entry))
		usable_delta = 0;	/* no delta */
	இல்லையெனில் எனில் (!pack_size_limit)
	       usable_delta = 1;	/* unlimited packfile */
	இல்லையெனில் எனில் (DELTA(entry)->idx.offset == (off_t)-1)
		usable_delta = 0;	/* base was written to another pack */
	இல்லையெனில் எனில் (DELTA(entry)->idx.offset)
		usable_delta = 1;	/* base already exists in this pack */
	இல்லையெனில்
		usable_delta = 0;	/* base could end up in another pack */

	எனில் (!reuse_object)
		to_reuse = 0;	/* explicit */
	இல்லையெனில் எனில் (!IN_PACK(entry))
		to_reuse = 0;	/* can't reuse what we don't have */
	இல்லையெனில் எனில் (oe_type(entry) == OBJ_REF_DELTA ||
		 oe_type(entry) == OBJ_OFS_DELTA)
				/* check_object() decided it for us ... */
		to_reuse = usable_delta;
				/* ... but pack split may override that */
	இல்லையெனில் எனில் (oe_type(entry) != entry->in_pack_type)
		to_reuse = 0;	/* pack has delta which is unusable */
	இல்லையெனில் எனில் (DELTA(entry))
		to_reuse = 0;	/* we want to pack afresh */
	இல்லையெனில்
		to_reuse = 1;	/* we have it in-pack undeltified,
				 * and we do not need to deltify it.
				 */

	எனில் (!to_reuse)
		len = write_no_reuse_object(f, entry, limit, usable_delta);
	இல்லையெனில்
		len = write_reuse_object(f, entry, limit, usable_delta);
	எனில் (!len)
		திரும்பு 0;

	எனில் (usable_delta)
		written_delta++;
	written++;
	எனில் (!pack_to_stdout)
		entry->idx.crc32 = crc32_end(f);
	திரும்பு len;
}

எண்ணிக்கை write_one_status {
	WRITE_ONE_SKIP = -1, /* already written */
	WRITE_ONE_BREAK = 0, /* writing this will bust the limit; not written */
	WRITE_ONE_WRITTEN = 1, /* normal */
	WRITE_ONE_RECURSIVE = 2 /* already scheduled to be written */
};

நிலையான எண்ணிக்கை write_one_status write_one(கட்டமைப்பு hashfile *f,
				       கட்டமைப்பு object_entry *e,
				       off_t *offset)
{
	off_t size;
	முழு recursing;

	/*
	 * we set offset to 1 (which is an impossible value) to mark
	 * the fact that this object is involved in "write its base
	 * first before writing a deltified object" recursion.
	 */
	recursing = (e->idx.offset == 1);
	எனில் (recursing) {
		warning(_("recursive delta detected for object %s"),
			oid_to_hex(&e->idx.oid));
		திரும்பு WRITE_ONE_RECURSIVE;
	} இல்லையெனில் எனில் (e->idx.offset || e->preferred_base) {
		/* offset is non zero if object is written already. */
		திரும்பு WRITE_ONE_SKIP;
	}

	/* if we are deltified, write out base object first. */
	எனில் (DELTA(e)) {
		e->idx.offset = 1; /* now recurse */
		தேர்வு (write_one(f, DELTA(e), offset)) {
		நிலை WRITE_ONE_RECURSIVE:
			/* we cannot depend on this one */
			SET_DELTA(e, வெற்று);
			நிறுத்து;
		இயல்பு:
			நிறுத்து;
		நிலை WRITE_ONE_BREAK:
			e->idx.offset = recursing;
			திரும்பு WRITE_ONE_BREAK;
		}
	}

	e->idx.offset = *offset;
	size = write_object(f, e, *offset);
	எனில் (!size) {
		e->idx.offset = recursing;
		திரும்பு WRITE_ONE_BREAK;
	}
	written_list[nr_written++] = &e->idx;

	/* make sure off_t is sufficiently large not to wrap */
	எனில் (signed_add_overflows(*offset, size))
		die(_("pack too large for current definition of off_t"));
	*offset += size;
	திரும்பு WRITE_ONE_WRITTEN;
}

நிலையான முழு mark_tagged(மாறிலி கட்டமைப்பு reference *ref, வெறுமை *cb_data UNUSED)
{
	கட்டமைப்பு object_id peeled;
	கட்டமைப்பு object_entry *entry = packlist_find(&to_pack, ref->oid);

	எனில் (entry)
		entry->tagged = 1;
	எனில் (!reference_get_peeled_oid(the_repository, ref, &peeled)) {
		entry = packlist_find(&to_pack, &peeled);
		எனில் (entry)
			entry->tagged = 1;
	}
	திரும்பு 0;
}

நிலையான உள்வரி unsigned எழுத்து oe_layer(கட்டமைப்பு packing_data *pack,
				     கட்டமைப்பு object_entry *e)
{
	எனில் (!pack->layer)
		திரும்பு 0;
	திரும்பு pack->layer[e - pack->objects];
}

நிலையான உள்வரி வெறுமை add_to_write_order(கட்டமைப்பு object_entry **wo,
			       unsigned முழு *endp,
			       கட்டமைப்பு object_entry *e)
{
	எனில் (e->filled || oe_layer(&to_pack, e) != write_layer)
		திரும்பு;
	wo[(*endp)++] = e;
	e->filled = 1;
}

நிலையான வெறுமை add_descendants_to_write_order(கட்டமைப்பு object_entry **wo,
					   unsigned முழு *endp,
					   கட்டமைப்பு object_entry *e)
{
	முழு add_to_order = 1;
	போது (e) {
		எனில் (add_to_order) {
			கட்டமைப்பு object_entry *s;
			/* add this node... */
			add_to_write_order(wo, endp, e);
			/* all its siblings... */
			க்கு (s = DELTA_SIBLING(e); s; s = DELTA_SIBLING(s)) {
				add_to_write_order(wo, endp, s);
			}
		}
		/* drop down a level to add left subtree nodes if possible */
		எனில் (DELTA_CHILD(e)) {
			add_to_order = 1;
			e = DELTA_CHILD(e);
		} இல்லையெனில் {
			add_to_order = 0;
			/* our sibling might have some children, it is next */
			எனில் (DELTA_SIBLING(e)) {
				e = DELTA_SIBLING(e);
				தொடர்;
			}
			/* go back to our parent node */
			e = DELTA(e);
			போது (e && !DELTA_SIBLING(e)) {
				/* we're on the right side of a subtree, keep
				 * going up until we can go right again */
				e = DELTA(e);
			}
			எனில் (!e) {
				/* done- we hit our original root node */
				திரும்பு;
			}
			/* pass it off to sibling at this level */
			e = DELTA_SIBLING(e);
		}
	};
}

நிலையான வெறுமை add_family_to_write_order(கட்டமைப்பு object_entry **wo,
				      unsigned முழு *endp,
				      கட்டமைப்பு object_entry *e)
{
	கட்டமைப்பு object_entry *வேர்;

	க்கு (வேர் = e; DELTA(வேர்); வேர் = DELTA(வேர்))
		; /* nothing */
	add_descendants_to_write_order(wo, endp, வேர்);
}

நிலையான வெறுமை compute_layer_order(கட்டமைப்பு object_entry **wo, unsigned முழு *wo_end)
{
	unsigned முழு i, last_untagged;
	கட்டமைப்பு object_entry *objects = to_pack.objects;

	க்கு (i = 0; i < to_pack.nr_objects; i++) {
		எனில் (objects[i].tagged)
			நிறுத்து;
		add_to_write_order(wo, wo_end, &objects[i]);
	}
	last_untagged = i;

	/*
	 * Then fill all the tagged tips.
	 */
	க்கு (; i < to_pack.nr_objects; i++) {
		எனில் (objects[i].tagged)
			add_to_write_order(wo, wo_end, &objects[i]);
	}

	/*
	 * And then all remaining commits and tags.
	 */
	க்கு (i = last_untagged; i < to_pack.nr_objects; i++) {
		எனில் (oe_type(&objects[i]) != OBJ_COMMIT &&
		    oe_type(&objects[i]) != OBJ_TAG)
			தொடர்;
		add_to_write_order(wo, wo_end, &objects[i]);
	}

	/*
	 * And then all the trees.
	 */
	க்கு (i = last_untagged; i < to_pack.nr_objects; i++) {
		எனில் (oe_type(&objects[i]) != OBJ_மரம்)
			தொடர்;
		add_to_write_order(wo, wo_end, &objects[i]);
	}

	/*
	 * Finally all the rest in really tight order
	 */
	க்கு (i = last_untagged; i < to_pack.nr_objects; i++) {
		எனில் (!objects[i].filled && oe_layer(&to_pack, &objects[i]) == write_layer)
			add_family_to_write_order(wo, wo_end, &objects[i]);
	}
}

நிலையான கட்டமைப்பு object_entry **compute_write_order(வெறுமை)
{
	uint32_t max_layers = 1;
	unsigned முழு i, wo_end;

	கட்டமைப்பு object_entry **wo;
	கட்டமைப்பு object_entry *objects = to_pack.objects;

	க்கு (i = 0; i < to_pack.nr_objects; i++) {
		objects[i].tagged = 0;
		objects[i].filled = 0;
		SET_DELTA_CHILD(&objects[i], வெற்று);
		SET_DELTA_SIBLING(&objects[i], வெற்று);
	}

	/*
	 * Fully connect delta_child/delta_sibling network.
	 * Make sure delta_sibling is sorted in the original
	 * recency order.
	 */
	க்கு (i = to_pack.nr_objects; i > 0;) {
		கட்டமைப்பு object_entry *e = &objects[--i];
		எனில் (!DELTA(e))
			தொடர்;
		/* Mark me as the first child */
		e->delta_sibling_idx = DELTA(e)->delta_child_idx;
		SET_DELTA_CHILD(DELTA(e), e);
	}

	/*
	 * Mark objects that are at the tip of tags.
	 */
	refs_for_each_tag_ref(get_main_ref_store(the_repository), mark_tagged,
			      வெற்று);

	எனில் (use_delta_islands) {
		max_layers = compute_pack_layers(&to_pack);
		free_island_marks();
	}

	ALLOC_ARRAY(wo, to_pack.nr_objects);
	wo_end = 0;

	க்கு (; write_layer < max_layers; ++write_layer)
		compute_layer_order(wo, &wo_end);

	எனில் (wo_end != to_pack.nr_objects)
		die(_("ordered %u objects, expected %"PRIu32),
		    wo_end, to_pack.nr_objects);

	திரும்பு wo;
}


/*
 * A reused set of objects. All objects in a chunk have the same
 * relative position in the original packfile and the generated
 * packfile.
 */

நிலையான கட்டமைப்பு reused_chunk {
	/* The offset of the first object of this chunk in the original
	 * packfile. */
	off_t original;
	/* The difference for "original" minus the offset of the first object of
	 * this chunk in the generated packfile. */
	off_t difference;
} *reused_chunks;
நிலையான முழு reused_chunks_nr;
நிலையான முழு reused_chunks_alloc;

நிலையான வெறுமை record_reused_object(off_t where, off_t offset)
{
	எனில் (reused_chunks_nr && reused_chunks[reused_chunks_nr-1].difference == offset)
		திரும்பு;

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
நிலையான off_t find_reused_offset(off_t where)
{
	முழு lo = 0, hi = reused_chunks_nr;
	போது (lo < hi) {
		முழு mi = lo + ((hi - lo) / 2);
		எனில் (where == reused_chunks[mi].original)
			திரும்பு reused_chunks[mi].difference;
		எனில் (where < reused_chunks[mi].original)
			hi = mi;
		இல்லையெனில்
			lo = mi + 1;
	}

	/*
	 * The first chunk starts at zero, so we can't have gone below
	 * there.
	 */
	ப்ரதிஜ்ஞா(lo);
	திரும்பு reused_chunks[lo-1].difference;
}

நிலையான வெறுமை write_reused_pack_one(கட்டமைப்பு packed_git *reuse_packfile,
				  size_t pos, கட்டமைப்பு hashfile *out,
				  off_t pack_start,
				  கட்டமைப்பு pack_window **w_curs)
{
	off_t offset, next, cur;
	எண்ணிக்கை object_type type;
	size_t size;

	offset = pack_pos_to_offset(reuse_packfile, pos);
	next = pack_pos_to_offset(reuse_packfile, pos + 1);

	record_reused_object(offset,
			     offset - (hashfile_total(out) - pack_start));

	cur = offset;
	type = unpack_object_header(reuse_packfile, w_curs, &cur, &size);
	ப்ரதிஜ்ஞா(type >= 0);

	எனில் (type == OBJ_OFS_DELTA) {
		off_t base_offset;
		off_t fixup;

		unsigned எழுத்து header[MAX_PACK_OBJECT_HEADER];
		unsigned len;

		base_offset = get_delta_base(reuse_packfile, w_curs, &cur, type, offset);
		ப்ரதிஜ்ஞா(base_offset != 0);

		/* Convert to REF_DELTA if we must... */
		எனில் (!allow_ofs_delta) {
			uint32_t base_pos;
			கட்டமைப்பு object_id base_oid;

			எனில் (offset_to_pack_pos(reuse_packfile, base_offset, &base_pos) < 0)
				die(_("expected object at offset %"PRIuMAX" "
				      "in pack %s"),
				    (uintmax_t)base_offset,
				    reuse_packfile->pack_name);

			nth_packed_object_id(&base_oid, reuse_packfile,
					     pack_pos_to_index(reuse_packfile, base_pos));

			len = encode_in_pack_object_header(header, அளவு(header),
							   OBJ_REF_DELTA, size);
			hashwrite(out, header, len);
			hashwrite(out, base_oid.hash, the_hash_algo->rawsz);
			copy_pack_data(out, reuse_packfile, w_curs, cur, next - cur);
			திரும்பு;
		}

		/* Otherwise see if we need to rewrite the offset... */
		fixup = find_reused_offset(offset) -
			find_reused_offset(base_offset);
		எனில் (fixup) {
			unsigned எழுத்து ofs_header[MAX_PACK_OBJECT_HEADER];
			unsigned i, ofs_len;
			off_t ofs = offset - base_offset - fixup;

			len = encode_in_pack_object_header(header, அளவு(header),
							   OBJ_OFS_DELTA, size);

			i = அளவு(ofs_header) - 1;
			ofs_header[i] = ofs & 127;
			போது (ofs >>= 7)
				ofs_header[--i] = 128 | (--ofs & 127);

			ofs_len = அளவு(ofs_header) - i;

			hashwrite(out, header, len);
			hashwrite(out, ofs_header + அளவு(ofs_header) - ofs_len, ofs_len);
			copy_pack_data(out, reuse_packfile, w_curs, cur, next - cur);
			திரும்பு;
		}

		/* ...otherwise we have no fixup, and can write it verbatim */
	}

	copy_pack_data(out, reuse_packfile, w_curs, offset, next - offset);
}

நிலையான size_t write_reused_pack_verbatim(கட்டமைப்பு bitmapped_pack *reuse_packfile,
					 கட்டமைப்பு hashfile *out,
					 கட்டமைப்பு pack_window **w_curs)
{
	size_t pos = 0;
	size_t end;

	எனில் (reuse_packfile->bitmap_pos) {
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
		திரும்பு reuse_packfile->bitmap_pos / BITS_IN_EWORD;
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
	எனில் (reuse_packfile_bitmap->word_alloc < end)
		BUG("fewer words than expected in reuse_packfile_bitmap");

	போது (pos < end && reuse_packfile_bitmap->சொற்கள்[pos] == (eword_t)~0)
		pos++;

	எனில் (pos) {
		off_t to_write;

		written = (pos * BITS_IN_EWORD);
		to_write = pack_pos_to_offset(reuse_packfile->p, written)
			- அளவு(கட்டமைப்பு pack_header);

		/* We're recording one chunk, not one object. */
		record_reused_object(அளவு(கட்டமைப்பு pack_header), 0);
		hashflush(out);
		copy_pack_data(out, reuse_packfile->p, w_curs,
			அளவு(கட்டமைப்பு pack_header), to_write);

		display_progress(progress_state, written);
	}
	திரும்பு pos;
}

நிலையான வெறுமை write_reused_pack(கட்டமைப்பு bitmapped_pack *reuse_packfile,
			      கட்டமைப்பு hashfile *f)
{
	size_t i = reuse_packfile->bitmap_pos / BITS_IN_EWORD;
	uint32_t offset;
	off_t pack_start = hashfile_total(f) - அளவு(கட்டமைப்பு pack_header);
	கட்டமைப்பு pack_window *w_curs = வெற்று;

	எனில் (allow_ofs_delta)
		i = write_reused_pack_verbatim(reuse_packfile, f, &w_curs);

	க்கு (; i < reuse_packfile_bitmap->word_alloc; ++i) {
		eword_t word = reuse_packfile_bitmap->சொற்கள்[i];
		size_t pos = (i * BITS_IN_EWORD);

		க்கு (offset = 0; offset < BITS_IN_EWORD; ++offset) {
			uint32_t pack_pos;
			எனில் ((word >> offset) == 0)
				நிறுத்து;

			offset += ewah_bit_ctz64(word >> offset);
			எனில் (pos + offset < reuse_packfile->bitmap_pos)
				தொடர்;
			எனில் (pos + offset >= reuse_packfile->bitmap_pos + reuse_packfile->bitmap_nr)
				செல் done;

			எனில் (reuse_packfile->bitmap_pos) {
				/*
				 * When doing multi-pack reuse on a
				 * non-preferred pack, translate bit positions
				 * from the MIDX pseudo-pack order back to their
				 * pack-relative positions before attempting
				 * reuse.
				 */
				கட்டமைப்பு multi_pack_index *m = reuse_packfile->from_midx;
				uint32_t midx_pos;
				off_t pack_ofs;

				எனில் (!m)
					BUG("non-zero bitmap position without MIDX");

				midx_pos = pack_pos_to_midx(m, pos + offset);
				pack_ofs = nth_midxed_offset(m, midx_pos);

				எனில் (offset_to_pack_pos(reuse_packfile->p,
						       pack_ofs, &pack_pos) < 0)
					BUG("could not find expected object at offset %"PRIuMAX" in pack %s",
					    (uintmax_t)pack_ofs,
					    pack_basename(reuse_packfile->p));
			} இல்லையெனில் {
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

நிலையான வெறுமை write_excluded_by_configs(வெறுமை)
{
	கட்டமைப்பு oidset_iter iter;
	மாறிலி கட்டமைப்பு object_id *oid;

	oidset_iter_init(&excluded_by_config, &iter);
	போது ((oid = oidset_iter_next(&iter))) {
		கட்டமைப்பு configured_exclusion *ex =
			oidmap_get(&configured_exclusions, oid);

		எனில் (!ex)
			BUG("configured exclusion wasn't configured");
		write_in_full(1, ex->pack_hash_hex, strlen(ex->pack_hash_hex));
		write_in_full(1, " ", 1);
		write_in_full(1, ex->uri, strlen(ex->uri));
		write_in_full(1, "\n", 1);
	}
}

நிலையான மாறிலி எழுத்து no_split_warning[] = N_(
"disabling bitmap writing, packs are split due to pack.packSizeLimit"
);

நிலையான வெறுமை write_pack_file(வெறுமை)
{
	uint32_t i = 0, j;
	கட்டமைப்பு hashfile *f;
	off_t offset;
	uint32_t nr_remaining = nr_result;
	time_t last_mtime = 0;
	கட்டமைப்பு object_entry **write_order;
	off_t bytes_written = 0;

	எனில் (progress > pack_to_stdout)
		progress_state = start_progress(the_repository,
						_("Writing objects"), nr_result);
	ALLOC_ARRAY(written_list, to_pack.nr_objects);
	write_order = compute_write_order();

	செய் {
		unsigned எழுத்து hash[GIT_MAX_RAWSZ];
		எழுத்து *pack_tmp_name = வெற்று;

		எனில் (pack_to_stdout) {
			/*
			 * This command is most often invoked via
			 * git-upload-pack(1), which will typically chunk data
			 * into pktlines. As such, we use the maximum data
			 * length of them as buffer length.
			 *
			 * Note that we need to subtract one though to
			 * accommodate for the sideband byte.
			 */
			கட்டமைப்பு hashfd_options opts = {
				.progress = progress_state,
				.buffer_len = LARGE_PACKET_DATA_MAX - 1,
			};
			f = hashfd_ext(the_repository->hash_algo, 1,
				       "<stdout>", &opts);
		} இல்லையெனில் {
			f = create_tmp_packfile(the_repository, &pack_tmp_name);
		}

		offset = write_pack_header(f, nr_remaining);

		எனில் (reuse_packfiles_nr) {
			ப்ரதிஜ்ஞா(pack_to_stdout);
			க்கு (j = 0; j < reuse_packfiles_nr; j++) {
				reused_chunks_nr = 0;
				write_reused_pack(&reuse_packfiles[j], f);
				எனில் (reused_chunks_nr)
					reuse_packfiles_used_nr++;
			}
			offset = hashfile_total(f);
		}

		nr_written = 0;
		க்கு (; i < to_pack.nr_objects; i++) {
			கட்டமைப்பு object_entry *e = write_order[i];
			எனில் (write_one(f, e, &offset) == WRITE_ONE_BREAK)
				நிறுத்து;
			display_progress(progress_state, written);
		}

		bytes_written += hashfile_total(f) +
			the_repository->hash_algo->rawsz;
		எனில் (pack_to_stdout) {
			/*
			 * We never fsync when writing to stdout since we may
			 * not be writing to an actual pack file. For instance,
			 * the upload-pack code passes a pipe here. Calling
			 * fsync on a pipe results in unnecessary
			 * synchronization with the reader on some platforms.
			 */
			finalize_hashfile(f, hash, FSYNC_COMPONENT_NONE,
					  CSUM_HASH_IN_STREAM | CSUM_CLOSE);
		} இல்லையெனில் எனில் (nr_written == nr_remaining) {
			finalize_hashfile(f, hash, FSYNC_COMPONENT_PACK,
					  CSUM_HASH_IN_STREAM | CSUM_FSYNC | CSUM_CLOSE);
		} இல்லையெனில் {
			/*
			 * If we wrote the wrong number of entries in the
			 * header, rewrite it like in fast-import.
			 */

			முழு fd = finalize_hashfile(f, hash, FSYNC_COMPONENT_PACK, 0);
			fixup_pack_header_footer(the_hash_algo, fd, hash,
						 pack_tmp_name, nr_written,
						 hash, offset);
			close(fd);
			எனில் (write_bitmap_index) {
				எனில் (write_bitmap_index != WRITE_BITMAP_QUIET)
					warning(_(no_split_warning));
				write_bitmap_index = 0;
			}
		}

		எனில் (!pack_to_stdout) {
			கட்டமைப்பு stat st;
			கட்டமைப்பு strbuf tmpname = STRBUF_INIT;
			கட்டமைப்பு bitmap_writer bitmap_writer;
			எழுத்து *idx_tmp_name = வெற்று;

			/*
			 * Packs are runtime accessed in their mtime
			 * order since newer packs are more likely to contain
			 * younger objects.  So if we are creating multiple
			 * packs then we should modify the mtime of later ones
			 * to preserve this property.
			 */
			எனில் (stat(pack_tmp_name, &st) < 0) {
				warning_errno(_("failed to stat %s"), pack_tmp_name);
			} இல்லையெனில் எனில் (!last_mtime) {
				last_mtime = st.st_mtime;
			} இல்லையெனில் {
				கட்டமைப்பு utimbuf utb;
				utb.actime = st.st_atime;
				utb.modtime = --last_mtime;
				எனில் (utime(pack_tmp_name, &utb) < 0)
					warning_errno(_("failed utime() on %s"), pack_tmp_name);
			}

			strbuf_addf(&tmpname, "%s-%s.", base_name,
				    hash_to_hex(hash));

			எனில் (write_bitmap_index) {
				bitmap_writer_init(&bitmap_writer,
						   the_repository, &to_pack,
						   வெற்று);
				bitmap_writer_set_checksum(&bitmap_writer, hash);
				bitmap_writer_build_type_index(&bitmap_writer,
							       written_list);
			}

			எனில் (cruft)
				pack_idx_opts.flags |= WRITE_MTIMES;

			stage_tmp_packfiles(the_repository, &tmpname,
					    pack_tmp_name, written_list,
					    nr_written, &to_pack,
					    &pack_idx_opts, hash,
					    &idx_tmp_name);

			எனில் (write_bitmap_index) {
				size_t tmpname_len = tmpname.len;

				strbuf_addstr(&tmpname, "bitmap");
				stop_progress(&progress_state);

				bitmap_writer_show_progress(&bitmap_writer,
							    progress);
				bitmap_writer_select_commits(&bitmap_writer,
							     indexed_commits,
							     indexed_commits_nr);
				எனில் (bitmap_writer_build(&bitmap_writer) < 0)
					die(_("failed to write bitmap index"));
				bitmap_writer_finish(&bitmap_writer,
						     written_list,
						     tmpname.buf, write_bitmap_options);
				bitmap_writer_free(&bitmap_writer);
				write_bitmap_index = 0;
				strbuf_setlen(&tmpname, tmpname_len);
			}

			rename_tmp_packfile_idx(the_repository, &tmpname, &idx_tmp_name);

			விடுவி(idx_tmp_name);
			strbuf_release(&tmpname);
			விடுவி(pack_tmp_name);
			puts(hash_to_hex(hash));
		}

		/* mark written objects as written to previous pack */
		க்கு (j = 0; j < nr_written; j++) {
			written_list[j]->offset = (off_t)-1;
		}
		nr_remaining -= nr_written;
	} போது (nr_remaining && i < to_pack.nr_objects);

	விடுவி(written_list);
	விடுவி(write_order);
	stop_progress(&progress_state);
	எனில் (written != nr_result)
		die(_("wrote %"PRIu32" objects while expecting %"PRIu32),
		    written, nr_result);
	trace2_data_intmax("pack-objects", the_repository,
			   "write_pack_file/wrote", nr_result);
	trace2_data_intmax("pack-objects", the_repository,
			   "write_pack_file/wrote_bytes", bytes_written);
}

நிலையான முழு no_try_delta(மாறிலி எழுத்து *path)
{
	நிலையான கட்டமைப்பு attr_check *check;

	எனில் (!check)
		check = attr_check_initl("delta", வெற்று);
	git_check_attr(the_repository->index, path, check);
	எனில் (ATTR_FALSE(check->பொருட்கள்[0].மதிப்பு))
		திரும்பு 1;
	திரும்பு 0;
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
நிலையான முழு have_duplicate_entry(மாறிலி கட்டமைப்பு object_id *oid,
				முழு exclude)
{
	கட்டமைப்பு object_entry *entry;

	எனில் (reuse_packfile_bitmap &&
	    bitmap_walk_contains(bitmap_git, reuse_packfile_bitmap, oid))
		திரும்பு 1;

	entry = packlist_find(&to_pack, oid);
	எனில் (!entry)
		திரும்பு 0;

	எனில் (exclude) {
		எனில் (!entry->preferred_base)
			nr_result--;
		entry->preferred_base = 1;
	}

	திரும்பு 1;
}

நிலையான முழு want_cruft_object_mtime(கட்டமைப்பு repository *r,
				   மாறிலி கட்டமைப்பு object_id *oid,
				   unsigned flags, uint32_t mtime)
{
	கட்டமைப்பு odb_source_files *files = odb_source_files_downcast(r->objects->source);

	க்கு (கட்டமைப்பு odb_files_dir *dir = files->dirs; dir; dir = dir->next) {
		கட்டமைப்பு packed_git **cache = packfile_store_get_kept_pack_cache(dir->packed, flags);

		க்கு (; *cache; cache++) {
			கட்டமைப்பு packed_git *p = *cache;
			off_t ofs;
			uint32_t candidate_mtime;

			ofs = find_pack_entry_one(oid, p);
			எனில் (!ofs)
				தொடர்;

			/*
			 * We have a copy of the object 'oid' in a non-cruft
			 * pack. We can avoid packing an additional copy
			 * regardless of what the existing copy's mtime is since
			 * it is outside of a cruft pack.
			 */
			எனில் (!p->is_cruft)
				திரும்பு 0;

			/*
			 * If we have a copy of the object 'oid' in a cruft
			 * pack, then either read the cruft pack's mtime for
			 * that object, or, if that can't be loaded, assume the
			 * pack's mtime itself.
			 */
			எனில் (!load_pack_mtimes(p)) {
				uint32_t pos;
				எனில் (offset_to_pack_pos(p, ofs, &pos) < 0)
					தொடர்;
				candidate_mtime = nth_packed_mtime(p, pos);
			} இல்லையெனில் {
				candidate_mtime = p->mtime;
			}

			/*
			 * We have a surviving copy of the object in a cruft
			 * pack whose mtime is greater than or equal to the one
			 * we are considering. We can thus avoid packing an
			 * additional copy of that object.
			 */
			எனில் (mtime <= candidate_mtime)
				திரும்பு 0;
		}
	}

	திரும்பு -1;
}

நிலையான முழு want_found_object(மாறிலி கட்டமைப்பு object_id *oid, முழு exclude,
			     கட்டமைப்பு packed_git *p, uint32_t mtime)
{
	எனில் (exclude)
		திரும்பு 1;
	எனில் (incremental)
		திரும்பு 0;

	எனில் (!is_pack_valid(p))
		திரும்பு -1;

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
	எனில் (local && !p->pack_local)
		திரும்பு 0;

	/*
	 * Then handle .keep first, as we have a fast(er) path there.
	 */
	எனில் (ignore_packed_keep_on_disk || ignore_packed_keep_in_core ||
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
		எனில் (ignore_packed_keep_on_disk)
			flags |= KEPT_PACK_ON_DISK;
		எனில் (ignore_packed_keep_in_core)
			flags |= KEPT_PACK_IN_CORE;
		எனில் (ignore_packed_keep_in_core_open)
			flags |= KEPT_PACK_IN_CORE_OPEN;

		/*
		 * If the object is in a pack that we want to ignore, *and* we
		 * don't have any cruft packs that are being retained, we can
		 * abort quickly.
		 */
		எனில் (!ignore_packed_keep_in_core_has_cruft) {
			எனில் (ignore_packed_keep_on_disk && p->pack_keep)
				திரும்பு 0;
			எனில் (ignore_packed_keep_in_core && p->pack_keep_in_core)
				திரும்பு 0;
			எனில் (ignore_packed_keep_in_core_open && p->pack_keep_in_core_open)
				திரும்பு 0;
			எனில் (has_object_kept_pack(p->repo, oid, flags))
				திரும்பு 0;
		} இல்லையெனில் {
			/*
			 * But if there is at least one cruft pack which
			 * is being kept, we only want to include the
			 * provided object if it has a strictly greater
			 * mtime than any existing cruft copy.
			 */
			எனில் (!want_cruft_object_mtime(p->repo, oid, flags,
						     mtime))
				திரும்பு 0;
		}
	}

	/*
	 * At this point we know definitively that either we don't care about
	 * keep-packs, or the object is not in one. Keep checking other
	 * conditions...
	 */
	எனில் (!local || !have_non_local_packs)
		திரும்பு 1;

	/* we don't know yet; keep looking for more packs */
	திரும்பு -1;
}

நிலையான முழு want_object_in_pack_one(கட்டமைப்பு packed_git *p,
				   மாறிலி கட்டமைப்பு object_id *oid,
				   முழு exclude,
				   கட்டமைப்பு packed_git **found_pack,
				   off_t *found_offset,
				   uint32_t found_mtime)
{
	off_t offset;

	எனில் (p == *found_pack)
		offset = *found_offset;
	இல்லையெனில்
		offset = find_pack_entry_one(oid, p);

	எனில் (offset) {
		எனில் (!*found_pack) {
			எனில் (!is_pack_valid(p))
				திரும்பு -1;
			*found_offset = offset;
			*found_pack = p;
		}
		திரும்பு want_found_object(oid, exclude, p, found_mtime);
	}
	திரும்பு -1;
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
நிலையான முழு want_object_in_pack_mtime(மாறிலி கட்டமைப்பு object_id *oid,
				     முழு exclude,
				     கட்டமைப்பு packed_git **found_pack,
				     off_t *found_offset,
				     uint32_t found_mtime)
{
	கட்டமைப்பு odb_source_files *files =
		odb_source_files_downcast(the_repository->objects->source);
	முழு want;
	கட்டமைப்பு packfile_list_entry *e;

	எனில் (!exclude && local) {
		/*
		 * Note that we start iterating at `dirs->next` so that we
		 * skip the local object directory.
		 */
		க்கு (கட்டமைப்பு odb_files_dir *dir = files->dirs->next; dir; dir = dir->next)
			எனில் (!odb_source_read_object_info(&dir->loose->base, oid, வெற்று, 0, வெற்று))
				திரும்பு 0;
	}

	/*
	 * If we already know the pack object lives in, start checks from that
	 * pack - in the usual case when neither --local was given nor .keep files
	 * are present we will determine the answer right now.
	 */
	எனில் (*found_pack) {
		want = want_found_object(oid, exclude, *found_pack,
					 found_mtime);
		எனில் (want != -1)
			திரும்பு want;

		*found_pack = வெற்று;
		*found_offset = 0;
	}

	க்கு (கட்டமைப்பு odb_files_dir *dir = files->dirs; dir; dir = dir->next) {
		கட்டமைப்பு multi_pack_index *m = get_multi_pack_index(dir->packed);
		கட்டமைப்பு pack_entry e;

		எனில் (m && midx_fill_entry(m, oid, &e, வெற்று) == MIDX_FILL_HIT) {
			want = want_object_in_pack_one(e.p, oid, exclude, found_pack, found_offset, found_mtime);
			எனில் (want != -1)
				திரும்பு want;
		}
	}

	க்கு (கட்டமைப்பு odb_files_dir *dir = files->dirs; dir; dir = dir->next) {
		க்கு (e = dir->packed->packs.head; e; e = e->next) {
			கட்டமைப்பு packed_git *p = e->pack;
			want = want_object_in_pack_one(p, oid, exclude, found_pack, found_offset, found_mtime);
			எனில் (!exclude && want > 0)
				packfile_list_prepend(&dir->packed->packs, p);
			எனில் (want != -1)
				திரும்பு want;
		}
	}

	எனில் (uri_protocols.nr) {
		கட்டமைப்பு configured_exclusion *ex =
			oidmap_get(&configured_exclusions, oid);
		முழு i;
		மாறிலி எழுத்து *p;

		எனில் (ex) {
			க்கு (i = 0; i < uri_protocols.nr; i++) {
				எனில் (skip_prefix(ex->uri,
						uri_protocols.பொருட்கள்[i].string,
						&p) &&
				    *p == ':') {
					oidset_செருகு(&excluded_by_config, oid);
					திரும்பு 0;
				}
			}
		}
	}

	திரும்பு 1;
}

நிலையான உள்வரி முழு want_object_in_pack(மாறிலி கட்டமைப்பு object_id *oid,
				      முழு exclude,
				      கட்டமைப்பு packed_git **found_pack,
				      off_t *found_offset)
{
	திரும்பு want_object_in_pack_mtime(oid, exclude, found_pack, found_offset,
					 0);
}

நிலையான கட்டமைப்பு object_entry *create_object_entry(மாறிலி கட்டமைப்பு object_id *oid,
						எண்ணிக்கை object_type type,
						uint32_t hash,
						முழு exclude,
						முழு no_try_delta,
						கட்டமைப்பு packed_git *found_pack,
						off_t found_offset)
{
	கட்டமைப்பு object_entry *entry;

	entry = packlist_alloc(&to_pack, oid);
	entry->hash = hash;
	oe_set_type(entry, type);
	எனில் (exclude)
		entry->preferred_base = 1;
	இல்லையெனில்
		nr_result++;
	எனில் (found_pack) {
		oe_set_in_pack(&to_pack, entry, found_pack);
		entry->in_pack_offset = found_offset;
	}

	entry->no_try_delta = no_try_delta;

	திரும்பு entry;
}

நிலையான மாறிலி எழுத்து no_closure_warning[] = N_(
"disabling bitmap writing, as some objects are not being packed"
);

நிலையான வெறுமை add_object_entry(மாறிலி கட்டமைப்பு object_id *oid, எண்ணிக்கை object_type type,
			     மாறிலி எழுத்து *name, முழு exclude)
{
	கட்டமைப்பு packed_git *found_pack = வெற்று;
	off_t found_offset = 0;

	display_progress(progress_state, ++nr_seen);

	எனில் (have_duplicate_entry(oid, exclude))
		திரும்பு;

	எனில் (!want_object_in_pack(oid, exclude, &found_pack, &found_offset)) {
		/* The pack is missing an object, so it will not have closure */
		எனில் (write_bitmap_index) {
			எனில் (write_bitmap_index != WRITE_BITMAP_QUIET)
				warning(_(no_closure_warning));
			write_bitmap_index = 0;
		}
		திரும்பு;
	}

	create_object_entry(oid, type, pack_name_hash_fn(name),
			    exclude, name && no_try_delta(name),
			    found_pack, found_offset);
}

நிலையான முழு add_object_entry_from_bitmap(மாறிலி கட்டமைப்பு object_id *oid,
					எண்ணிக்கை object_type type,
					முழு flags UNUSED, uint32_t name_hash,
					கட்டமைப்பு packed_git *pack, off_t offset,
					வெறுமை *payload UNUSED)
{
	display_progress(progress_state, ++nr_seen);

	எனில் (have_duplicate_entry(oid, 0))
		திரும்பு 0;

	எனில் (!want_object_in_pack(oid, 0, &pack, &offset))
		திரும்பு 0;

	create_object_entry(oid, type, name_hash, 0, 0, pack, offset);
	திரும்பு 0;
}

கட்டமைப்பு pbase_மரம்_cache {
	கட்டமைப்பு object_id oid;
	முழு ref;
	முழு temporary;
	வெறுமை *மரம்_data;
	unsigned long மரம்_size;
};

நிலையான கட்டமைப்பு pbase_மரம்_cache *(pbase_மரம்_cache[256]);
நிலையான முழு pbase_மரம்_cache_ix(மாறிலி கட்டமைப்பு object_id *oid)
{
	திரும்பு oid->hash[0] % ARRAY_SIZE(pbase_மரம்_cache);
}
நிலையான முழு pbase_மரம்_cache_ix_incr(முழு ix)
{
	திரும்பு (ix+1) % ARRAY_SIZE(pbase_மரம்_cache);
}

நிலையான கட்டமைப்பு pbase_மரம் {
	கட்டமைப்பு pbase_மரம் *next;
	/* This is a phony "cache" entry; we are not
	 * going to evict it or find it through _get()
	 * mechanism -- this is for the toplevel node that
	 * would almost always change with any commit.
	 */
	கட்டமைப்பு pbase_மரம்_cache pcache;
} *pbase_மரம்;

நிலையான கட்டமைப்பு pbase_மரம்_cache *pbase_மரம்_get(மாறிலி கட்டமைப்பு object_id *oid)
{
	கட்டமைப்பு pbase_மரம்_cache *ent, *nent;
	வெறுமை *data;
	unsigned long size;
	size_t size_st = 0;
	எண்ணிக்கை object_type type;
	முழு neigh;
	முழு my_ix = pbase_மரம்_cache_ix(oid);
	முழு available_ix = -1;

	/* pbase-tree-cache acts as a limited hashtable.
	 * your object will be found at your index or within a few
	 * slots after that slot if it is cached.
	 */
	க்கு (neigh = 0; neigh < 8; neigh++) {
		ent = pbase_மரம்_cache[my_ix];
		எனில் (ent && oideq(&ent->oid, oid)) {
			ent->ref++;
			திரும்பு ent;
		}
		இல்லையெனில் எனில் (((available_ix < 0) && (!ent || !ent->ref)) ||
			 ((0 <= available_ix) &&
			  (!ent && pbase_மரம்_cache[available_ix])))
			available_ix = my_ix;
		எனில் (!ent)
			நிறுத்து;
		my_ix = pbase_மரம்_cache_ix_incr(my_ix);
	}

	/* Did not find one.  Either we got a bogus request or
	 * we need to read and perhaps cache.
	 */
	data = odb_read_object(the_repository->objects, oid, &type, &size_st);
	size = cast_size_t_to_ulong(size_st);
	எனில் (!data)
		திரும்பு வெற்று;
	எனில் (type != OBJ_மரம்) {
		விடுவி(data);
		திரும்பு வெற்று;
	}

	/* We need to either cache or return a throwaway copy */

	எனில் (available_ix < 0)
		ent = வெற்று;
	இல்லையெனில் {
		ent = pbase_மரம்_cache[available_ix];
		my_ix = available_ix;
	}

	எனில் (!ent) {
		nent = xmalloc(அளவு(*nent));
		nent->temporary = (available_ix < 0);
	}
	இல்லையெனில் {
		/* evict and reuse */
		விடுவி(ent->மரம்_data);
		nent = ent;
	}
	oidcpy(&nent->oid, oid);
	nent->மரம்_data = data;
	nent->மரம்_size = size;
	nent->ref = 1;
	எனில் (!nent->temporary)
		pbase_மரம்_cache[my_ix] = nent;
	திரும்பு nent;
}

நிலையான வெறுமை pbase_மரம்_put(கட்டமைப்பு pbase_மரம்_cache *cache)
{
	எனில் (!cache->temporary) {
		cache->ref--;
		திரும்பு;
	}
	விடுவி(cache->மரம்_data);
	விடுவி(cache);
}

நிலையான size_t name_cmp_len(மாறிலி எழுத்து *name)
{
	திரும்பு strcspn(name, "\n/");
}

நிலையான வெறுமை add_pbase_object(கட்டமைப்பு மரம்_desc *மரம்,
			     மாறிலி எழுத்து *name,
			     size_t cmplen,
			     மாறிலி எழுத்து *fullname)
{
	கட்டமைப்பு name_entry entry;
	முழு cmp;

	போது (மரம்_entry(மரம்,&entry)) {
		எனில் (S_ISGITLINK(entry.mode))
			தொடர்;
		cmp = மரம்_entry_len(&entry) != cmplen ? 1 :
		      memcmp(name, entry.path, cmplen);
		எனில் (cmp > 0)
			தொடர்;
		எனில் (cmp < 0)
			திரும்பு;
		எனில் (name[cmplen] != '/') {
			add_object_entry(&entry.oid,
					 object_type(entry.mode),
					 fullname, 1);
			திரும்பு;
		}
		எனில் (S_ISDIR(entry.mode)) {
			கட்டமைப்பு மரம்_desc sub;
			கட்டமைப்பு pbase_மரம்_cache *மரம்;
			மாறிலி எழுத்து *down = name+cmplen+1;
			size_t downlen = name_cmp_len(down);

			மரம் = pbase_மரம்_get(&entry.oid);
			எனில் (!மரம்)
				திரும்பு;
			init_மரம்_desc(&sub, &மரம்->oid,
				       மரம்->மரம்_data, மரம்->மரம்_size);

			add_pbase_object(&sub, down, downlen, fullname);
			pbase_மரம்_put(மரம்);
		}
	}
}

நிலையான unsigned *done_pbase_paths;
நிலையான முழு done_pbase_paths_இலக்கம்;
நிலையான முழு done_pbase_paths_alloc;
நிலையான முழு done_pbase_path_pos(unsigned hash)
{
	முழு lo = 0;
	முழு hi = done_pbase_paths_இலக்கம்;
	போது (lo < hi) {
		முழு mi = lo + (hi - lo) / 2;
		எனில் (done_pbase_paths[mi] == hash)
			திரும்பு mi;
		எனில் (done_pbase_paths[mi] < hash)
			hi = mi;
		இல்லையெனில்
			lo = mi + 1;
	}
	திரும்பு -lo-1;
}

நிலையான முழு check_pbase_path(unsigned hash)
{
	முழு pos = done_pbase_path_pos(hash);
	எனில் (0 <= pos)
		திரும்பு 1;
	pos = -pos - 1;
	ALLOC_GROW(done_pbase_paths,
		   done_pbase_paths_இலக்கம் + 1,
		   done_pbase_paths_alloc);
	done_pbase_paths_இலக்கம்++;
	எனில் (pos < done_pbase_paths_இலக்கம்)
		MOVE_ARRAY(done_pbase_paths + pos + 1, done_pbase_paths + pos,
			   done_pbase_paths_இலக்கம் - pos - 1);
	done_pbase_paths[pos] = hash;
	திரும்பு 0;
}

நிலையான வெறுமை add_preferred_base_object(மாறிலி எழுத்து *name)
{
	கட்டமைப்பு pbase_மரம் *it;
	size_t cmplen;
	unsigned hash = pack_name_hash_fn(name);

	எனில் (!இலக்கம்_preferred_base || check_pbase_path(hash))
		திரும்பு;

	cmplen = name_cmp_len(name);
	க்கு (it = pbase_மரம்; it; it = it->next) {
		எனில் (cmplen == 0) {
			add_object_entry(&it->pcache.oid, OBJ_மரம், வெற்று, 1);
		}
		இல்லையெனில் {
			கட்டமைப்பு மரம்_desc மரம்;
			init_மரம்_desc(&மரம், &it->pcache.oid,
				       it->pcache.மரம்_data, it->pcache.மரம்_size);
			add_pbase_object(&மரம், name, cmplen, name);
		}
	}
}

நிலையான வெறுமை add_preferred_base(கட்டமைப்பு object_id *oid)
{
	கட்டமைப்பு pbase_மரம் *it;
	வெறுமை *data;
	unsigned long size;
	size_t size_st = 0;
	கட்டமைப்பு object_id மரம்_oid;

	எனில் (window <= இலக்கம்_preferred_base++)
		திரும்பு;

	data = odb_read_object_peeled(the_repository->objects, oid,
				      OBJ_மரம், &size_st, &மரம்_oid);
	size = cast_size_t_to_ulong(size_st);
	எனில் (!data)
		திரும்பு;

	க்கு (it = pbase_மரம்; it; it = it->next) {
		எனில் (oideq(&it->pcache.oid, &மரம்_oid)) {
			விடுவி(data);
			திரும்பு;
		}
	}

	CALLOC_ARRAY(it, 1);
	it->next = pbase_மரம்;
	pbase_மரம் = it;

	oidcpy(&it->pcache.oid, &மரம்_oid);
	it->pcache.மரம்_data = data;
	it->pcache.மரம்_size = size;
}

நிலையான வெறுமை cleanup_preferred_base(வெறுமை)
{
	கட்டமைப்பு pbase_மரம் *it;
	unsigned i;

	it = pbase_மரம்;
	pbase_மரம் = வெற்று;
	போது (it) {
		கட்டமைப்பு pbase_மரம் *tmp = it;
		it = tmp->next;
		விடுவி(tmp->pcache.மரம்_data);
		விடுவி(tmp);
	}

	க்கு (i = 0; i < ARRAY_SIZE(pbase_மரம்_cache); i++) {
		எனில் (!pbase_மரம்_cache[i])
			தொடர்;
		விடுவி(pbase_மரம்_cache[i]->மரம்_data);
		FREE_AND_NULL(pbase_மரம்_cache[i]);
	}

	FREE_AND_NULL(done_pbase_paths);
	done_pbase_paths_இலக்கம் = done_pbase_paths_alloc = 0;
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
நிலையான முழு can_reuse_delta(மாறிலி கட்டமைப்பு object_id *base_oid,
			   கட்டமைப்பு object_entry *delta,
			   கட்டமைப்பு object_entry **base_out)
{
	கட்டமைப்பு object_entry *base;

	/*
	 * First see if we're already sending the base (or it's explicitly in
	 * our "excluded" list).
	 */
	base = packlist_find(&to_pack, base_oid);
	எனில் (base) {
		எனில் (!in_same_island(&delta->idx.oid, &base->idx.oid))
			திரும்பு 0;
		*base_out = base;
		திரும்பு 1;
	}

	/*
	 * Otherwise, reachability bitmaps may tell us if the receiver has it,
	 * even if it was buried too deep in history to make it into the
	 * packing list.
	 */
	எனில் (thin && bitmap_has_oid_in_uninteresting(bitmap_git, base_oid)) {
		எனில் (use_delta_islands) {
			எனில் (!in_same_island(&delta->idx.oid, base_oid))
				திரும்பு 0;
		}
		*base_out = வெற்று;
		திரும்பு 1;
	}

	திரும்பு 0;
}

நிலையான வெறுமை prefetch_to_pack(uint32_t object_index_start) {
	கட்டமைப்பு oid_array to_fetch = OID_ARRAY_INIT;
	uint32_t i;

	க்கு (i = object_index_start; i < to_pack.nr_objects; i++) {
		கட்டமைப்பு object_entry *entry = to_pack.objects + i;

		எனில் (!odb_read_object_info_extended(the_repository->objects,
						   &entry->idx.oid,
						   வெற்று,
						   OBJECT_INFO_FOR_PREFETCH))
			தொடர்;
		oid_array_append(&to_fetch, &entry->idx.oid);
	}
	promisor_remote_get_direct(the_repository,
				   to_fetch.oid, to_fetch.nr);
	oid_array_clear(&to_fetch);
}

நிலையான வெறுமை check_object(கட்டமைப்பு object_entry *entry, uint32_t object_index)
{
	size_t canonical_size;
	எண்ணிக்கை object_type type;
	கட்டமைப்பு object_info oi = {.typep = &type, .sizep = &canonical_size};

	எனில் (IN_PACK(entry)) {
		கட்டமைப்பு packed_git *p = IN_PACK(entry);
		கட்டமைப்பு pack_window *w_curs = வெற்று;
		முழு have_base = 0;
		கட்டமைப்பு object_id base_ref;
		கட்டமைப்பு object_entry *base_entry;
		size_t used, used_0, avail;
		off_t ofs;
		unsigned எழுத்து *buf, c;
		எண்ணிக்கை object_type type;
		size_t in_pack_size;

		buf = use_pack(p, &w_curs, entry->in_pack_offset, &avail);

		/*
		 * We want in_pack_type even if we do not reuse delta
		 * since non-delta representations could still be reused.
		 */
		used = unpack_object_header_buffer(buf, avail,
						   &type,
						   &in_pack_size);
		எனில் (used == 0)
			செல் give_up;

		எனில் (type < 0)
			BUG("invalid type %d", type);
		entry->in_pack_type = type;

		/*
		 * Determine if this is a delta and if so whether we can
		 * reuse it or not.  Otherwise let's find out as cheaply as
		 * possible what the actual type and size for this object is.
		 */
		தேர்வு (entry->in_pack_type) {
		இயல்பு:
			/* Not a delta hence we've already got all we need. */
			oe_set_type(entry, entry->in_pack_type);
			SET_SIZE(entry, in_pack_size);
			entry->in_pack_header_size = used;
			எனில் (oe_type(entry) < OBJ_COMMIT || oe_type(entry) > OBJ_BLOB)
				செல் give_up;
			unuse_pack(&w_curs);
			திரும்பு;
		நிலை OBJ_REF_DELTA:
			எனில் (reuse_delta && !entry->preferred_base) {
				oidread(&base_ref,
					use_pack(p, &w_curs,
						 entry->in_pack_offset + used,
						 வெற்று),
					the_repository->hash_algo);
				have_base = 1;
			}
			entry->in_pack_header_size = used + the_hash_algo->rawsz;
			நிறுத்து;
		நிலை OBJ_OFS_DELTA:
			buf = use_pack(p, &w_curs,
				       entry->in_pack_offset + used, வெற்று);
			used_0 = 0;
			c = buf[used_0++];
			ofs = c & 127;
			போது (c & 128) {
				ofs += 1;
				எனில் (!ofs || MSB(ofs, 7)) {
					error(_("delta base offset overflow in pack for %s"),
					      oid_to_hex(&entry->idx.oid));
					செல் give_up;
				}
				c = buf[used_0++];
				ofs = (ofs << 7) + (c & 127);
			}
			ofs = entry->in_pack_offset - ofs;
			எனில் (ofs <= 0 || ofs >= entry->in_pack_offset) {
				error(_("delta base offset out of bound for %s"),
				      oid_to_hex(&entry->idx.oid));
				செல் give_up;
			}
			எனில் (reuse_delta && !entry->preferred_base) {
				uint32_t pos;
				எனில் (offset_to_pack_pos(p, ofs, &pos) < 0)
					செல் give_up;
				எனில் (!nth_packed_object_id(&base_ref, p,
							  pack_pos_to_index(p, pos)))
					have_base = 1;
			}
			entry->in_pack_header_size = used + used_0;
			நிறுத்து;
		}

		எனில் (have_base &&
		    can_reuse_delta(&base_ref, entry, &base_entry)) {
			oe_set_type(entry, entry->in_pack_type);
			SET_SIZE(entry, in_pack_size); /* delta size */
			SET_DELTA_SIZE(entry, in_pack_size);

			எனில் (base_entry) {
				SET_DELTA(entry, base_entry);
				entry->delta_sibling_idx = base_entry->delta_child_idx;
				SET_DELTA_CHILD(base_entry, entry);
			} இல்லையெனில் {
				SET_DELTA_EXT(entry, &base_ref);
			}

			unuse_pack(&w_curs);
			திரும்பு;
		}

		எனில் (oe_type(entry)) {
			off_t delta_pos;

			/*
			 * This must be a delta and we already know what the
			 * final object type is.  Let's extract the actual
			 * object size from the delta header.
			 */
			delta_pos = entry->in_pack_offset + entry->in_pack_header_size;
			canonical_size = get_size_from_delta(p, &w_curs,
							     delta_pos);
			எனில் (canonical_size == 0)
				செல் give_up;
			SET_SIZE(entry, canonical_size);
			unuse_pack(&w_curs);
			திரும்பு;
		}

		/*
		 * No choice but to fall back to the recursive delta walk
		 * with odb_read_object_info() to find about the object type
		 * at this point...
		 */
		give_up:
		unuse_pack(&w_curs);
	}

	எனில் (odb_read_object_info_extended(the_repository->objects, &entry->idx.oid, &oi,
					  OBJECT_INFO_SKIP_FETCH_OBJECT | OBJECT_INFO_LOOKUP_REPLACE) < 0) {
		எனில் (repo_has_promisor_remote(the_repository)) {
			prefetch_to_pack(object_index);
			எனில் (odb_read_object_info_extended(the_repository->objects, &entry->idx.oid, &oi,
							  OBJECT_INFO_SKIP_FETCH_OBJECT | OBJECT_INFO_LOOKUP_REPLACE) < 0)
				type = -1;
		} இல்லையெனில் {
			type = -1;
		}
	}
	oe_set_type(entry, type);
	எனில் (entry->type_valid) {
		SET_SIZE(entry, canonical_size);
	} இல்லையெனில் {
		/*
		 * Bad object type is checked in prepare_pack().  This is
		 * to permit a missing preferred base object to be ignored
		 * as a preferred base.  Doing so can result in a larger
		 * pack file, but the transfer will still take place.
		 */
	}
}

நிலையான முழு pack_offset_sort(மாறிலி வெறுமை *_a, மாறிலி வெறுமை *_b)
{
	மாறிலி கட்டமைப்பு object_entry *a = *(கட்டமைப்பு object_entry **)_a;
	மாறிலி கட்டமைப்பு object_entry *b = *(கட்டமைப்பு object_entry **)_b;
	மாறிலி கட்டமைப்பு packed_git *a_in_pack = IN_PACK(a);
	மாறிலி கட்டமைப்பு packed_git *b_in_pack = IN_PACK(b);

	/* avoid filesystem trashing with loose objects */
	எனில் (!a_in_pack && !b_in_pack)
		திரும்பு oidcmp(&a->idx.oid, &b->idx.oid);

	எனில் (a_in_pack < b_in_pack)
		திரும்பு -1;
	எனில் (a_in_pack > b_in_pack)
		திரும்பு 1;
	திரும்பு a->in_pack_offset < b->in_pack_offset ? -1 :
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
நிலையான வெறுமை drop_reused_delta(கட்டமைப்பு object_entry *entry)
{
	unsigned *idx = &to_pack.objects[entry->delta_idx - 1].delta_child_idx;
	கட்டமைப்பு object_info oi = OBJECT_INFO_INIT;
	எண்ணிக்கை object_type type;
	size_t size;

	போது (*idx) {
		கட்டமைப்பு object_entry *oe = &to_pack.objects[*idx - 1];

		எனில் (oe == entry)
			*idx = oe->delta_sibling_idx;
		இல்லையெனில்
			idx = &oe->delta_sibling_idx;
	}
	SET_DELTA(entry, வெற்று);
	entry->depth = 0;

	oi.sizep = &size;
	oi.typep = &type;
	எனில் (packed_object_info(வெற்று, IN_PACK(entry), entry->in_pack_offset, &oi) < 0) {
		/*
		 * We failed to get the info from this pack for some reason;
		 * fall back to odb_read_object_info, which may find another copy.
		 * And if that fails, the error will be recorded in oe_type(entry)
		 * and dealt with in prepare_pack().
		 */
		oe_set_type(entry,
			    odb_read_object_info(the_repository->objects,
						 &entry->idx.oid, &size));
	} இல்லையெனில் {
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
நிலையான வெறுமை break_delta_chains(கட்டமைப்பு object_entry *entry)
{
	/*
	 * The actual depth of each object we will write is stored as an int,
	 * as it cannot exceed our int "depth" limit. But before we break
	 * changes based no that limit, we may potentially go as deep as the
	 * number of objects, which is elsewhere bounded to a uint32_t.
	 */
	uint32_t total_depth;
	கட்டமைப்பு object_entry *cur, *next;

	க்கு (cur = entry, total_depth = 0;
	     cur;
	     cur = DELTA(cur), total_depth++) {
		எனில் (cur->dfs_state == DFS_DONE) {
			/*
			 * We've already seen this object and know it isn't
			 * part of a cycle. We do need to append its depth
			 * to our count.
			 */
			total_depth += cur->depth;
			நிறுத்து;
		}

		/*
		 * We break cycles before looping, so an ACTIVE state (or any
		 * other cruft which made its way into the state variable)
		 * is a bug.
		 */
		எனில் (cur->dfs_state != DFS_NONE)
			BUG("confusing delta dfs state in first pass: %d",
			    cur->dfs_state);

		/*
		 * Now we know this is the first time we've seen the object. If
		 * it's not a delta, we're done traversing, but we'll mark it
		 * done to save time on future traversals.
		 */
		எனில் (!DELTA(cur)) {
			cur->dfs_state = DFS_DONE;
			நிறுத்து;
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
		எனில் (DELTA(cur)->dfs_state == DFS_ACTIVE) {
			drop_reused_delta(cur);
			cur->dfs_state = DFS_DONE;
			நிறுத்து;
		}
	}

	/*
	 * And now that we've gone all the way to the bottom of the chain, we
	 * need to clear the active flags and set the depth fields as
	 * appropriate. Unlike the loop above, which can quit when it drops a
	 * delta, we need to keep going to look for more depth cuts. So we need
	 * an extra "next" pointer to keep going after we reset cur->delta.
	 */
	க்கு (cur = entry; cur; cur = next) {
		next = DELTA(cur);

		/*
		 * We should have a chain of zero or more ACTIVE states down to
		 * a final DONE. We can quit after the DONE, because either it
		 * has no bases, or we've already handled them in a previous
		 * call.
		 */
		எனில் (cur->dfs_state == DFS_DONE)
			நிறுத்து;
		இல்லையெனில் எனில் (cur->dfs_state != DFS_ACTIVE)
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
		எனில் (!cur->depth)
			drop_reused_delta(cur);

		cur->dfs_state = DFS_DONE;
	}
}

நிலையான வெறுமை get_object_details(வெறுமை)
{
	uint32_t i;
	கட்டமைப்பு object_entry **sorted_by_offset;

	எனில் (progress)
		progress_state = start_progress(the_repository,
						_("Counting objects"),
						to_pack.nr_objects);

	CALLOC_ARRAY(sorted_by_offset, to_pack.nr_objects);
	க்கு (i = 0; i < to_pack.nr_objects; i++)
		sorted_by_offset[i] = to_pack.objects + i;
	QSORT(sorted_by_offset, to_pack.nr_objects, pack_offset_sort);

	க்கு (i = 0; i < to_pack.nr_objects; i++) {
		கட்டமைப்பு object_entry *entry = sorted_by_offset[i];
		check_object(entry, i);
		எனில் (entry->type_valid &&
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
	க்கு (i = 0; i < to_pack.nr_objects; i++)
		break_delta_chains(&to_pack.objects[i]);

	விடுவி(sorted_by_offset);
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
நிலையான முழு type_size_sort(மாறிலி வெறுமை *_a, மாறிலி வெறுமை *_b)
{
	மாறிலி கட்டமைப்பு object_entry *a = *(கட்டமைப்பு object_entry **)_a;
	மாறிலி கட்டமைப்பு object_entry *b = *(கட்டமைப்பு object_entry **)_b;
	மாறிலி எண்ணிக்கை object_type a_type = oe_type(a);
	மாறிலி எண்ணிக்கை object_type b_type = oe_type(b);
	மாறிலி unsigned long a_size = SIZE(a);
	மாறிலி unsigned long b_size = SIZE(b);

	எனில் (a_type > b_type)
		திரும்பு -1;
	எனில் (a_type < b_type)
		திரும்பு 1;
	எனில் (a->hash > b->hash)
		திரும்பு -1;
	எனில் (a->hash < b->hash)
		திரும்பு 1;
	எனில் (a->preferred_base > b->preferred_base)
		திரும்பு -1;
	எனில் (a->preferred_base < b->preferred_base)
		திரும்பு 1;
	எனில் (use_delta_islands) {
		மாறிலி முழு island_cmp = island_delta_cmp(&a->idx.oid, &b->idx.oid);
		எனில் (island_cmp)
			திரும்பு island_cmp;
	}
	எனில் (a_size > b_size)
		திரும்பு -1;
	எனில் (a_size < b_size)
		திரும்பு 1;
	திரும்பு a < b ? -1 : (a > b);  /* newest first */
}

கட்டமைப்பு unpacked {
	கட்டமைப்பு object_entry *entry;
	வெறுமை *data;
	கட்டமைப்பு delta_index *index;
	unsigned depth;
};

நிலையான முழு delta_cacheable(size_t src_size, size_t trg_size,
			   size_t delta_size)
{
	எனில் (max_delta_cache_size && delta_cache_size + delta_size > max_delta_cache_size)
		திரும்பு 0;

	எனில் (delta_size < cache_max_small_delta_size)
		திரும்பு 1;

	/* cache delta, if objects are large enough compared to delta size */
	எனில் ((src_size >> 20) + (trg_size >> 21) > (delta_size >> 10))
		திரும்பு 1;

	திரும்பு 0;
}

/* Protect delta_cache_size */
நிலையான pthread_mutex_t cache_mutex;
#define cache_lock()		pthread_mutex_lock(&cache_mutex)
#define cache_unlock()		pthread_mutex_unlock(&cache_mutex)

/*
 * Protect object list partitioning (e.g. struct thread_param) and
 * progress_state
 */
நிலையான pthread_mutex_t progress_mutex;
#define progress_lock()		pthread_mutex_lock(&progress_mutex)
#define progress_unlock()	pthread_mutex_unlock(&progress_mutex)

/*
 * Access to struct object_entry is unprotected since each thread owns
 * a portion of the main object list. Just don't access object entries
 * ahead in the list because they can be stolen and would need
 * progress_mutex for protection.
 */

நிலையான உள்வரி முழு oe_size_less_than(கட்டமைப்பு packing_data *pack,
				    மாறிலி கட்டமைப்பு object_entry *lhs,
				    size_t rhs)
{
	எனில் (lhs->size_valid)
		திரும்பு lhs->size_ < rhs;
	எனில் (rhs < pack->oe_size_limit) /* rhs < 2^x <= lhs ? */
		திரும்பு 0;
	திரும்பு oe_get_size_slow(pack, lhs) < rhs;
}

நிலையான உள்வரி வெறுமை oe_set_மரம்_depth(கட்டமைப்பு packing_data *pack,
				     கட்டமைப்பு object_entry *e,
				     unsigned முழு மரம்_depth)
{
	எனில் (!pack->மரம்_depth)
		CALLOC_ARRAY(pack->மரம்_depth, pack->nr_alloc);
	pack->மரம்_depth[e - pack->objects] = மரம்_depth;
}

நிலையான வெறுமை record_மரம்_depth(மாறிலி கட்டமைப்பு object_id *oid, மாறிலி எழுத்து *name)
{
	மாறிலி எழுத்து *p;
	unsigned depth;
	கட்டமைப்பு object_entry *ent;

	/* the empty string is a root tree, which is depth 0 */
	depth = *name ? 1 : 0;
	க்கு (p = strchr(name, '/'); p; p = strchr(p + 1, '/'))
		depth++;

	ent = packlist_find(&to_pack, oid);
	எனில் (ent && depth > oe_மரம்_depth(&to_pack, ent))
		oe_set_மரம்_depth(&to_pack, ent, depth);
}

/*
 * Return the size of the object without doing any delta
 * reconstruction (so non-deltas are true object sizes, but deltas
 * return the size of the delta data).
 */
size_t oe_get_size_slow(கட்டமைப்பு packing_data *pack,
			மாறிலி கட்டமைப்பு object_entry *e)
{
	கட்டமைப்பு packed_git *p;
	கட்டமைப்பு pack_window *w_curs;
	unsigned எழுத்து *buf;
	எண்ணிக்கை object_type type;
	size_t used, avail, size;

	எனில் (e->type_ != OBJ_OFS_DELTA && e->type_ != OBJ_REF_DELTA) {
		size_t sz;
		packing_data_lock(&to_pack);
		எனில் (odb_read_object_info(the_repository->objects,
					 &e->idx.oid, &sz) < 0)
			die(_("unable to get size of %s"),
			    oid_to_hex(&e->idx.oid));
		packing_data_unlock(&to_pack);
		திரும்பு sz;
	}

	p = oe_in_pack(pack, e);
	எனில் (!p)
		BUG("when e->type is a delta, it must belong to a pack");

	packing_data_lock(&to_pack);
	w_curs = வெற்று;
	buf = use_pack(p, &w_curs, e->in_pack_offset, &avail);
	used = unpack_object_header_buffer(buf, avail, &type, &size);
	எனில் (used == 0)
		die(_("unable to parse object header of %s"),
		    oid_to_hex(&e->idx.oid));

	unuse_pack(&w_curs);
	packing_data_unlock(&to_pack);
	திரும்பு size;
}

நிலையான முழு try_delta(கட்டமைப்பு unpacked *trg, கட்டமைப்பு unpacked *src,
		     unsigned max_depth, size_t *mem_usage)
{
	கட்டமைப்பு object_entry *trg_entry = trg->entry;
	கட்டமைப்பு object_entry *src_entry = src->entry;
	unsigned long trg_size, src_size, sizediff, max_size, sz;
	size_t delta_size;
	unsigned ref_depth;
	எண்ணிக்கை object_type type;
	வெறுமை *delta_buf;

	/* Don't bother doing diffs between different types */
	எனில் (oe_type(trg_entry) != oe_type(src_entry))
		திரும்பு -1;

	/*
	 * We do not bother to try a delta that we discarded on an
	 * earlier try, but only when reusing delta data.  Note that
	 * src_entry that is marked as the preferred_base should always
	 * be considered, as even if we produce a suboptimal delta against
	 * it, we will still save the transfer cost, as we already know
	 * the other side has it and we won't send src_entry at all.
	 */
	எனில் (reuse_delta && IN_PACK(trg_entry) &&
	    IN_PACK(trg_entry) == IN_PACK(src_entry) &&
	    !src_entry->preferred_base &&
	    trg_entry->in_pack_type != OBJ_REF_DELTA &&
	    trg_entry->in_pack_type != OBJ_OFS_DELTA)
		திரும்பு 0;

	/* Let's not bust the allowed depth. */
	எனில் (src->depth >= max_depth)
		திரும்பு 0;

	/* Now some size filtering heuristics. */
	trg_size = SIZE(trg_entry);
	எனில் (!DELTA(trg_entry)) {
		max_size = trg_size/2 - the_hash_algo->rawsz;
		ref_depth = 1;
	} இல்லையெனில் {
		max_size = DELTA_SIZE(trg_entry);
		ref_depth = trg->depth;
	}
	max_size = (uint64_t)max_size * (max_depth - src->depth) /
						(max_depth - ref_depth + 1);
	எனில் (max_size == 0)
		திரும்பு 0;
	src_size = SIZE(src_entry);
	sizediff = src_size < trg_size ? trg_size - src_size : 0;
	எனில் (sizediff >= max_size)
		திரும்பு 0;
	எனில் (trg_size < src_size / 32)
		திரும்பு 0;

	எனில் (!in_same_island(&trg->entry->idx.oid, &src->entry->idx.oid))
		திரும்பு 0;

	/* Load data if not already done */
	எனில் (!trg->data) {
		size_t sz_st = 0;
		packing_data_lock(&to_pack);
		trg->data = odb_read_object(the_repository->objects,
					    &trg_entry->idx.oid, &type,
					    &sz_st);
		sz = cast_size_t_to_ulong(sz_st);
		packing_data_unlock(&to_pack);
		எனில் (!trg->data)
			die(_("object %s cannot be read"),
			    oid_to_hex(&trg_entry->idx.oid));
		எனில் (sz != trg_size)
			die(_("object %s inconsistent object length (%"PRIuMAX" vs %"PRIuMAX")"),
			    oid_to_hex(&trg_entry->idx.oid), (uintmax_t)sz,
			    (uintmax_t)trg_size);
		*mem_usage += sz;
	}
	எனில் (!src->data) {
		size_t sz_st = 0;
		packing_data_lock(&to_pack);
		src->data = odb_read_object(the_repository->objects,
					    &src_entry->idx.oid, &type,
					    &sz_st);
		sz = cast_size_t_to_ulong(sz_st);
		packing_data_unlock(&to_pack);
		எனில் (!src->data) {
			எனில் (src_entry->preferred_base) {
				நிலையான முழு warned = 0;
				எனில் (!warned++)
					warning(_("object %s cannot be read"),
						oid_to_hex(&src_entry->idx.oid));
				/*
				 * Those objects are not included in the
				 * resulting pack.  Be resilient and ignore
				 * them if they can't be read, in case the
				 * pack could be created nevertheless.
				 */
				திரும்பு 0;
			}
			die(_("object %s cannot be read"),
			    oid_to_hex(&src_entry->idx.oid));
		}
		எனில் (sz != src_size)
			die(_("object %s inconsistent object length (%"PRIuMAX" vs %"PRIuMAX")"),
			    oid_to_hex(&src_entry->idx.oid), (uintmax_t)sz,
			    (uintmax_t)src_size);
		*mem_usage += sz;
	}
	எனில் (!src->index) {
		src->index = create_delta_index(src->data, src_size);
		எனில் (!src->index) {
			நிலையான முழு warned = 0;
			எனில் (!warned++)
				warning(_("suboptimal pack - out of memory"));
			திரும்பு 0;
		}
		*mem_usage += sizeof_delta_index(src->index);
	}

	delta_buf = create_delta(src->index, trg->data, trg_size, &delta_size, max_size);
	எனில் (!delta_buf)
		திரும்பு 0;

	எனில் (DELTA(trg_entry)) {
		/* Prefer only shallower same-sized deltas. */
		எனில் (delta_size == DELTA_SIZE(trg_entry) &&
		    src->depth + 1 >= trg->depth) {
			விடுவி(delta_buf);
			திரும்பு 0;
		}
	}

	/*
	 * Handle memory allocation outside of the cache
	 * accounting lock.  Compiler will optimize the strangeness
	 * away when NO_PTHREADS is defined.
	 */
	விடுவி(trg_entry->delta_data);
	cache_lock();
	எனில் (trg_entry->delta_data) {
		delta_cache_size -= DELTA_SIZE(trg_entry);
		trg_entry->delta_data = வெற்று;
	}
	எனில் (delta_cacheable(src_size, trg_size, delta_size)) {
		delta_cache_size += delta_size;
		cache_unlock();
		trg_entry->delta_data = xrealloc(delta_buf, delta_size);
	} இல்லையெனில் {
		cache_unlock();
		விடுவி(delta_buf);
	}

	SET_DELTA(trg_entry, src_entry);
	SET_DELTA_SIZE(trg_entry, delta_size);
	trg->depth = src->depth + 1;

	திரும்பு 1;
}

நிலையான unsigned முழு check_delta_limit(கட்டமைப்பு object_entry *me, unsigned முழு n)
{
	கட்டமைப்பு object_entry *child = DELTA_CHILD(me);
	unsigned முழு m = n;
	போது (child) {
		மாறிலி unsigned முழு c = check_delta_limit(child, n + 1);
		எனில் (m < c)
			m = c;
		child = DELTA_SIBLING(child);
	}
	திரும்பு m;
}

நிலையான size_t free_unpacked(கட்டமைப்பு unpacked *n)
{
	size_t freed_mem = sizeof_delta_index(n->index);
	free_delta_index(n->index);
	n->index = வெற்று;
	எனில் (n->data) {
		freed_mem += SIZE(n->entry);
		FREE_AND_NULL(n->data);
	}
	n->entry = வெற்று;
	n->depth = 0;
	திரும்பு freed_mem;
}

நிலையான வெறுமை find_deltas(கட்டமைப்பு object_entry **list, unsigned *list_size,
			முழு window, முழு depth, unsigned *processed)
{
	uint32_t i, idx = 0, மொத்தம் = 0;
	கட்டமைப்பு unpacked *array;
	size_t mem_usage = 0;

	CALLOC_ARRAY(array, window);

	க்கு (;;) {
		கட்டமைப்பு object_entry *entry;
		கட்டமைப்பு unpacked *n = array + idx;
		முழு j, max_depth, best_base = -1;

		progress_lock();
		எனில் (!*list_size) {
			progress_unlock();
			நிறுத்து;
		}
		entry = *list++;
		(*list_size)--;
		எனில் (!entry->preferred_base) {
			(*processed)++;
			display_progress(progress_state, *processed);
		}
		progress_unlock();

		mem_usage -= free_unpacked(n);
		n->entry = entry;

		போது (window_memory_limit &&
		       mem_usage > window_memory_limit &&
		       மொத்தம் > 1) {
			மாறிலி uint32_t tail = (idx + window - மொத்தம்) % window;
			mem_usage -= free_unpacked(array + tail);
			மொத்தம்--;
		}

		/* We do not compute delta to *create* objects we are not
		 * going to pack.
		 */
		எனில் (entry->preferred_base)
			செல் next;

		/*
		 * If the current object is at pack edge, take the depth the
		 * objects that depend on the current object into account
		 * otherwise they would become too deep.
		 */
		max_depth = depth;
		எனில் (DELTA_CHILD(entry)) {
			max_depth -= check_delta_limit(entry, 0);
			எனில் (max_depth <= 0)
				செல் next;
		}

		j = window;
		போது (--j > 0) {
			முழு ret;
			uint32_t other_idx = idx + j;
			கட்டமைப்பு unpacked *m;
			எனில் (other_idx >= window)
				other_idx -= window;
			m = array + other_idx;
			எனில் (!m->entry)
				நிறுத்து;
			ret = try_delta(n, m, max_depth, &mem_usage);
			எனில் (ret < 0)
				நிறுத்து;
			இல்லையெனில் எனில் (ret > 0)
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
		எனில் (entry->delta_data && !pack_to_stdout) {
			unsigned long size;

			size = do_compress(&entry->delta_data, DELTA_SIZE(entry));
			எனில் (size < (1U << OE_Z_DELTA_BITS)) {
				entry->z_delta_size = size;
				cache_lock();
				delta_cache_size -= DELTA_SIZE(entry);
				delta_cache_size += entry->z_delta_size;
				cache_unlock();
			} இல்லையெனில் {
				FREE_AND_NULL(entry->delta_data);
				entry->z_delta_size = 0;
			}
		}

		/* if we made n a delta, and if n is already at max
		 * depth, leaving it in the window is pointless.  we
		 * should evict it first.
		 */
		எனில் (DELTA(entry) && max_depth <= n->depth)
			தொடர்;

		/*
		 * Move the best delta base up in the window, after the
		 * currently deltified object, to keep it longer.  It will
		 * be the first base object to be attempted next.
		 */
		எனில் (DELTA(entry)) {
			கட்டமைப்பு unpacked swap = array[best_base];
			முழு dist = (window + idx - best_base) % window;
			முழு dst = best_base;
			போது (dist--) {
				முழு src = (dst + 1) % window;
				array[dst] = array[src];
				dst = src;
			}
			array[dst] = swap;
		}

		next:
		idx++;
		எனில் (மொத்தம் + 1 < window)
			மொத்தம்++;
		எனில் (idx >= window)
			idx = 0;
	}

	க்கு (i = 0; i < window; ++i) {
		free_delta_index(array[i].index);
		விடுவி(array[i].data);
	}
	விடுவி(array);
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

கட்டமைப்பு thread_params {
	pthread_t thread;
	கட்டமைப்பு object_entry **list;
	கட்டமைப்பு packing_region *regions;
	unsigned list_size;
	unsigned remaining;
	முழு window;
	முழு depth;
	முழு working;
	முழு data_ready;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	unsigned *processed;
};

நிலையான pthread_cond_t progress_cond;

/*
 * Mutex and conditional variable can't be statically-initialized on Windows.
 */
நிலையான வெறுமை init_threaded_தேடல்(வெறுமை)
{
	pthread_mutex_init(&cache_mutex, வெற்று);
	pthread_mutex_init(&progress_mutex, வெற்று);
	pthread_cond_init(&progress_cond, வெற்று);
}

நிலையான வெறுமை cleanup_threaded_தேடல்(வெறுமை)
{
	pthread_cond_destroy(&progress_cond);
	pthread_mutex_destroy(&cache_mutex);
	pthread_mutex_destroy(&progress_mutex);
}

நிலையான வெறுமை *threaded_find_deltas(வெறுமை *arg)
{
	கட்டமைப்பு thread_params *me = arg;

	progress_lock();
	போது (me->remaining) {
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
		போது (!me->data_ready)
			pthread_cond_wait(&me->cond, &me->mutex);
		me->data_ready = 0;
		pthread_mutex_unlock(&me->mutex);

		progress_lock();
	}
	progress_unlock();
	/* leave ->working 1 so that this doesn't get more work assigned */
	திரும்பு வெற்று;
}

நிலையான வெறுமை ll_find_deltas(கட்டமைப்பு object_entry **list, unsigned list_size,
			   முழு window, முழு depth, unsigned *processed)
{
	கட்டமைப்பு thread_params *p;
	முழு i, ret, active_threads = 0;

	init_threaded_தேடல்();

	எனில் (delta_தேடல்_threads <= 1) {
		find_deltas(list, &list_size, window, depth, processed);
		cleanup_threaded_தேடல்();
		திரும்பு;
	}
	எனில் (progress > pack_to_stdout)
		fprintf_ln(stderr, _("Delta compression using up to %d threads"),
			   delta_தேடல்_threads);
	CALLOC_ARRAY(p, delta_தேடல்_threads);

	/* Partition the work amongst work threads. */
	க்கு (i = 0; i < delta_தேடல்_threads; i++) {
		unsigned sub_size = list_size / (delta_தேடல்_threads - i);

		/* don't use too small segments or no deltas will be found */
		எனில் (sub_size < 2*window && i+1 < delta_தேடல்_threads)
			sub_size = 0;

		p[i].window = window;
		p[i].depth = depth;
		p[i].processed = processed;
		p[i].working = 1;
		p[i].data_ready = 0;

		/* try to split chunks on "path" boundaries */
		போது (sub_size && sub_size < list_size &&
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
	க்கு (i = 0; i < delta_தேடல்_threads; i++) {
		எனில் (!p[i].list_size)
			தொடர்;
		pthread_mutex_init(&p[i].mutex, வெற்று);
		pthread_cond_init(&p[i].cond, வெற்று);
		ret = pthread_create(&p[i].thread, வெற்று,
				     threaded_find_deltas, &p[i]);
		எனில் (ret)
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
	போது (active_threads) {
		கட்டமைப்பு thread_params *target = வெற்று;
		கட்டமைப்பு thread_params *victim = வெற்று;
		unsigned sub_size = 0;

		progress_lock();
		க்கு (;;) {
			க்கு (i = 0; !target && i < delta_தேடல்_threads; i++)
				எனில் (!p[i].working)
					target = &p[i];
			எனில் (target)
				நிறுத்து;
			pthread_cond_wait(&progress_cond, &progress_mutex);
		}

		க்கு (i = 0; i < delta_தேடல்_threads; i++)
			எனில் (p[i].remaining > 2*window &&
			    (!victim || victim->remaining < p[i].remaining))
				victim = &p[i];
		எனில் (victim) {
			sub_size = victim->remaining / 2;
			list = victim->list + victim->list_size - sub_size;
			போது (sub_size && list[0]->hash &&
			       list[0]->hash == list[-1]->hash) {
				list++;
				sub_size--;
			}
			எனில் (!sub_size) {
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

		எனில் (!sub_size) {
			pthread_join(target->thread, வெற்று);
			pthread_cond_destroy(&target->cond);
			pthread_mutex_destroy(&target->mutex);
			active_threads--;
		}
	}
	cleanup_threaded_தேடல்();
	விடுவி(p);
}

நிலையான முழு obj_is_packed(மாறிலி கட்டமைப்பு object_id *oid)
{
	திரும்பு packlist_find(&to_pack, oid) ||
		(reuse_packfile_bitmap &&
		 bitmap_walk_contains(bitmap_git, reuse_packfile_bitmap, oid));
}

நிலையான வெறுமை add_tag_chain(மாறிலி கட்டமைப்பு object_id *oid)
{
	கட்டமைப்பு tag *tag;

	/*
	 * We catch duplicates already in add_object_entry(), but we'd
	 * prefer to do this extra check to avoid having to parse the
	 * tag at all if we already know that it's being packed (e.g., if
	 * it was included via bitmaps, we would not have parsed it
	 * previously).
	 */
	எனில் (obj_is_packed(oid))
		திரும்பு;

	tag = lookup_tag(the_repository, oid);
	போது (1) {
		எனில் (!tag || parse_tag(the_repository, tag) || !tag->tagged)
			die(_("unable to pack objects reachable from tag %s"),
			    oid_to_hex(oid));

		add_object_entry(&tag->object.oid, OBJ_TAG, வெற்று, 0);

		எனில் (tag->tagged->type != OBJ_TAG)
			திரும்பு;

		tag = (கட்டமைப்பு tag *)tag->tagged;
	}
}

நிலையான முழு add_ref_tag(மாறிலி கட்டமைப்பு reference *ref, வெறுமை *cb_data UNUSED)
{
	கட்டமைப்பு object_id peeled;

	எனில் (!reference_get_peeled_oid(the_repository, ref, &peeled) &&
	    obj_is_packed(&peeled))
		add_tag_chain(ref->oid);
	திரும்பு 0;
}

நிலையான முழு should_attempt_deltas(கட்டமைப்பு object_entry *entry)
{
	எனில் (DELTA(entry))
		/* This happens if we decided to reuse existing
		 * delta from a pack. "reuse_delta &&" is implied.
		 */
		திரும்பு 0;

	எனில் (!entry->type_valid ||
	    oe_size_less_than(&to_pack, entry, 50))
		திரும்பு 0;

	எனில் (entry->no_try_delta)
		திரும்பு 0;

	எனில் (!entry->preferred_base) {
		எனில் (oe_type(entry) < 0)
			die(_("unable to get type of object %s"),
				oid_to_hex(&entry->idx.oid));
	} இல்லையெனில் எனில் (oe_type(entry) < 0) {
		/*
		 * This object is not found, but we
		 * don't have to include it anyway.
		 */
		திரும்பு 0;
	}

	திரும்பு 1;
}

நிலையான வெறுமை find_deltas_for_region(கட்டமைப்பு object_entry *list,
				   கட்டமைப்பு packing_region *region,
				   unsigned முழு *processed)
{
	கட்டமைப்பு object_entry **delta_list;
	unsigned முழு delta_list_nr = 0;

	ALLOC_ARRAY(delta_list, region->nr);
	க்கு (size_t i = 0; i < region->nr; i++) {
		கட்டமைப்பு object_entry *entry = list + region->start + i;
		எனில் (should_attempt_deltas(entry))
			delta_list[delta_list_nr++] = entry;
	}

	QSORT(delta_list, delta_list_nr, type_size_sort);
	find_deltas(delta_list, &delta_list_nr, window, depth, processed);
	விடுவி(delta_list);
}

நிலையான வெறுமை find_deltas_by_region(கட்டமைப்பு object_entry *list,
				  கட்டமைப்பு packing_region *regions,
				  size_t start, size_t nr)
{
	unsigned முழு processed = 0;
	size_t progress_nr;

	எனில் (!nr)
		திரும்பு;

	progress_nr = regions[nr - 1].start + regions[nr - 1].nr;

	எனில் (progress)
		progress_state = start_progress(the_repository,
						_("Compressing objects by path"),
						progress_nr);

	போது (nr--)
		find_deltas_for_region(list,
				       &regions[start++],
				       &processed);

	display_progress(progress_state, progress_nr);
	stop_progress(&progress_state);
}

நிலையான வெறுமை *threaded_find_deltas_by_path(வெறுமை *arg)
{
	கட்டமைப்பு thread_params *me = arg;

	progress_lock();
	போது (me->remaining) {
		போது (me->remaining) {
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
		போது (!me->data_ready)
			pthread_cond_wait(&me->cond, &me->mutex);
		me->data_ready = 0;
		pthread_mutex_unlock(&me->mutex);

		progress_lock();
	}
	progress_unlock();
	/* leave ->working 1 so that this doesn't get more work assigned */
	திரும்பு வெற்று;
}

நிலையான வெறுமை ll_find_deltas_by_region(கட்டமைப்பு object_entry *list,
				     கட்டமைப்பு packing_region *regions,
				     uint32_t start, uint32_t nr)
{
	கட்டமைப்பு thread_params *p;
	முழு i, ret, active_threads = 0;
	unsigned முழு processed = 0;
	uint32_t progress_nr;
	init_threaded_தேடல்();

	எனில் (!nr)
		திரும்பு;

	progress_nr =  regions[nr - 1].start + regions[nr - 1].nr;
	எனில் (delta_தேடல்_threads <= 1) {
		find_deltas_by_region(list, regions, start, nr);
		cleanup_threaded_தேடல்();
		திரும்பு;
	}

	எனில் (progress > pack_to_stdout)
		fprintf_ln(stderr,
			   Q_("Path-based delta compression using up to %d thread",
			      "Path-based delta compression using up to %d threads",
			      delta_தேடல்_threads),
			   delta_தேடல்_threads);
	CALLOC_ARRAY(p, delta_தேடல்_threads);

	எனில் (progress)
		progress_state = start_progress(the_repository,
						_("Compressing objects by path"),
						progress_nr);
	/* Partition the work amongst work threads. */
	க்கு (i = 0; i < delta_தேடல்_threads; i++) {
		unsigned sub_size = nr / (delta_தேடல்_threads - i);

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
	க்கு (i = 0; i < delta_தேடல்_threads; i++) {
		எனில் (!p[i].list_size)
			தொடர்;
		pthread_mutex_init(&p[i].mutex, வெற்று);
		pthread_cond_init(&p[i].cond, வெற்று);
		ret = pthread_create(&p[i].thread, வெற்று,
				     threaded_find_deltas_by_path, &p[i]);
		எனில் (ret)
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
	போது (active_threads) {
		கட்டமைப்பு thread_params *target = வெற்று;
		கட்டமைப்பு thread_params *victim = வெற்று;
		unsigned sub_size = 0;

		progress_lock();
		க்கு (;;) {
			க்கு (i = 0; !target && i < delta_தேடல்_threads; i++)
				எனில் (!p[i].working)
					target = &p[i];
			எனில் (target)
				நிறுத்து;
			pthread_cond_wait(&progress_cond, &progress_mutex);
		}

		க்கு (i = 0; i < delta_தேடல்_threads; i++)
			எனில் (p[i].remaining > 2*window &&
			    (!victim || victim->remaining < p[i].remaining))
				victim = &p[i];
		எனில் (victim) {
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

		எனில் (!sub_size) {
			pthread_join(target->thread, வெற்று);
			pthread_cond_destroy(&target->cond);
			pthread_mutex_destroy(&target->mutex);
			active_threads--;
		}
	}
	cleanup_threaded_தேடல்();
	விடுவி(p);

	display_progress(progress_state, progress_nr);
	stop_progress(&progress_state);
}

நிலையான வெறுமை prepare_pack(முழு window, முழு depth)
{
	கட்டமைப்பு object_entry **delta_list;
	uint32_t i, nr_deltas;
	unsigned n;

	எனில் (use_delta_islands)
		resolve_மரம்_islands(the_repository, progress, &to_pack);

	get_object_details();

	/*
	 * If we're locally repacking then we need to be doubly careful
	 * from now on in order to make sure no stealth corruption gets
	 * propagated to the new pack.  Clients receiving streamed packs
	 * should validate everything they get anyway so no need to incur
	 * the additional cost here in that case.
	 */
	எனில் (!pack_to_stdout)
		do_check_packed_object_crc = 1;

	எனில் (!to_pack.nr_objects || !window || !depth)
		திரும்பு;

	எனில் (path_walk)
		ll_find_deltas_by_region(to_pack.objects, to_pack.regions,
					 0, to_pack.nr_regions);

	ALLOC_ARRAY(delta_list, to_pack.nr_objects);
	nr_deltas = n = 0;

	க்கு (i = 0; i < to_pack.nr_objects; i++) {
		கட்டமைப்பு object_entry *entry = to_pack.objects + i;

		எனில் (!should_attempt_deltas(entry))
			தொடர்;

		எனில் (!entry->preferred_base)
			nr_deltas++;

		delta_list[n++] = entry;
	}

	எனில் (nr_deltas && n > 1) {
		unsigned nr_done = 0;

		எனில் (progress)
			progress_state = start_progress(the_repository,
							_("Compressing objects"),
							nr_deltas);
		QSORT(delta_list, n, type_size_sort);
		ll_find_deltas(delta_list, n, window+1, depth, &nr_done);
		stop_progress(&progress_state);
		எனில் (nr_done != nr_deltas)
			die(_("inconsistency with delta count"));
	}
	விடுவி(delta_list);
}

நிலையான முழு git_pack_config(மாறிலி எழுத்து *k, மாறிலி எழுத்து *v,
			   மாறிலி கட்டமைப்பு config_context *ctx, வெறுமை *cb)
{
	எனில் (!strcmp(k, "pack.window")) {
		window = git_config_int(k, v, ctx->kvi);
		திரும்பு 0;
	}
	எனில் (!strcmp(k, "pack.windowmemory")) {
		window_memory_limit = git_config_ulong(k, v, ctx->kvi);
		திரும்பு 0;
	}
	எனில் (!strcmp(k, "pack.depth")) {
		depth = git_config_int(k, v, ctx->kvi);
		திரும்பு 0;
	}
	எனில் (!strcmp(k, "pack.deltacachesize")) {
		max_delta_cache_size = git_config_size_t(k, v, ctx->kvi);
		திரும்பு 0;
	}
	எனில் (!strcmp(k, "pack.deltacachelimit")) {
		cache_max_small_delta_size = git_config_int(k, v, ctx->kvi);
		திரும்பு 0;
	}
	எனில் (!strcmp(k, "pack.writebitmaphashcache")) {
		எனில் (git_config_bool(k, v))
			write_bitmap_options |= BITMAP_OPT_HASH_CACHE;
		இல்லையெனில்
			write_bitmap_options &= ~BITMAP_OPT_HASH_CACHE;
	}

	எனில் (!strcmp(k, "pack.writebitmaplookuptable")) {
		எனில் (git_config_bool(k, v))
			write_bitmap_options |= BITMAP_OPT_LOOKUP_TABLE;
		இல்லையெனில்
			write_bitmap_options &= ~BITMAP_OPT_LOOKUP_TABLE;
	}

	எனில் (!strcmp(k, "pack.usebitmaps")) {
		use_bitmap_index_default = git_config_bool(k, v);
		திரும்பு 0;
	}
	எனில் (!strcmp(k, "pack.allowpackreuse")) {
		முழு res = git_parse_maybe_bool_text(v);
		எனில் (res < 0) {
			எனில் (!strcasecmp(v, "single"))
				allow_pack_reuse = SINGLE_PACK_REUSE;
			இல்லையெனில் எனில் (!strcasecmp(v, "multi"))
				allow_pack_reuse = MULTI_PACK_REUSE;
			இல்லையெனில்
				die(_("invalid pack.allowPackReuse value: '%s'"), v);
		} இல்லையெனில் எனில் (res) {
			allow_pack_reuse = SINGLE_PACK_REUSE;
		} இல்லையெனில் {
			allow_pack_reuse = NO_PACK_REUSE;
		}
		திரும்பு 0;
	}
	எனில் (!strcmp(k, "pack.threads")) {
		delta_தேடல்_threads = git_config_int(k, v, ctx->kvi);
		எனில் (delta_தேடல்_threads < 0)
			die(_("invalid number of threads specified (%d)"),
			    delta_தேடல்_threads);
		எனில் (!HAVE_THREADS && delta_தேடல்_threads != 1) {
			warning(_("no threads support, ignoring %s"), k);
			delta_தேடல்_threads = 0;
		}
		திரும்பு 0;
	}
	எனில் (!strcmp(k, "pack.indexversion")) {
		pack_idx_opts.version = git_config_int(k, v, ctx->kvi);
		எனில் (pack_idx_opts.version > 2)
			die(_("bad pack.indexVersion=%"PRIu32),
			    pack_idx_opts.version);
		திரும்பு 0;
	}
	எனில் (!strcmp(k, "pack.writereverseindex")) {
		எனில் (git_config_bool(k, v))
			pack_idx_opts.flags |= WRITE_REV;
		இல்லையெனில்
			pack_idx_opts.flags &= ~WRITE_REV;
		திரும்பு 0;
	}
	எனில் (!strcmp(k, "uploadpack.blobpackfileuri")) {
		கட்டமைப்பு configured_exclusion *ex;
		மாறிலி எழுத்து *oid_end, *pack_end;
		/*
		 * Stores the pack hash. This is not a true object ID, but is
		 * of the same form.
		 */
		கட்டமைப்பு object_id pack_hash;

		எனில் (!v)
			திரும்பு config_error_nonbool(k);

		ex = xmalloc(அளவு(*ex));
		எனில் (parse_oid_hex(v, &ex->e.oid, &oid_end) ||
		    *oid_end != ' ' ||
		    parse_oid_hex(oid_end + 1, &pack_hash, &pack_end) ||
		    *pack_end != ' ')
			die(_("value of uploadpack.blobpackfileuri must be "
			      "of the form '<object-hash> <pack-hash> <uri>' (got '%s')"), v);
		எனில் (oidmap_get(&configured_exclusions, &ex->e.oid))
			die(_("object already configured in another "
			      "uploadpack.blobpackfileuri (got '%s')"), v);
		ex->pack_hash_hex = xcalloc(1, pack_end - oid_end);
		memcpy(ex->pack_hash_hex, oid_end + 1, pack_end - oid_end - 1);
		ex->uri = xstrdup(pack_end + 1);
		oidmap_put(&configured_exclusions, ex);
	}
	திரும்பு git_default_config(k, v, ctx, cb);
}

/* Counters for trace2 output when in --stdin-packs mode. */
நிலையான முழு stdin_packs_found_nr;
நிலையான முழு stdin_packs_hints_nr;

நிலையான முழு add_object_entry_from_pack(மாறிலி கட்டமைப்பு object_id *oid,
				      கட்டமைப்பு packed_git *p,
				      uint32_t pos,
				      வெறுமை *_data)
{
	off_t ofs;
	கட்டமைப்பு object_info oi = OBJECT_INFO_INIT;
	எண்ணிக்கை object_type type = OBJ_NONE;

	display_progress(progress_state, ++nr_seen);

	எனில் (have_duplicate_entry(oid, 0))
		திரும்பு 0;

	stdin_packs_found_nr++;

	ofs = nth_packed_object_offset(p, pos);

	oi.typep = &type;
	எனில் (packed_object_info(வெற்று, p, ofs, &oi) < 0) {
		die(_("could not get type of object %s in pack %s"),
		    oid_to_hex(oid), p->pack_name);
	} இல்லையெனில் எனில் (type == OBJ_COMMIT) {
		கட்டமைப்பு rev_info *revs = _data;
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
		add_pending_oid(revs, வெற்று, oid, 0);
	}

	எனில் (!want_object_in_pack(oid, 0, &p, &ofs))
		திரும்பு 0;

	create_object_entry(oid, type, 0, 0, 0, p, ofs);

	திரும்பு 0;
}

நிலையான வெறுமை show_object_pack_hint(கட்டமைப்பு object *object, மாறிலி எழுத்து *name,
				  வெறுமை *data)
{
	எண்ணிக்கை stdin_packs_mode mode = *(எண்ணிக்கை stdin_packs_mode *)data;
	எனில் (mode == STDIN_PACKS_MODE_FOLLOW) {
		எனில் (object->type == OBJ_BLOB &&
		    !odb_has_object(the_repository->objects, &object->oid, 0))
			திரும்பு;
		add_object_entry(&object->oid, object->type, name, 0);
	} இல்லையெனில் {
		கட்டமைப்பு object_entry *oe = packlist_find(&to_pack, &object->oid);
		எனில் (!oe)
			திரும்பு;

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

நிலையான வெறுமை show_commit_pack_hint(கட்டமைப்பு commit *commit, வெறுமை *data)
{
	எண்ணிக்கை stdin_packs_mode mode = *(எண்ணிக்கை stdin_packs_mode *)data;

	எனில் (mode == STDIN_PACKS_MODE_FOLLOW) {
		show_object_pack_hint((கட்டமைப்பு object *)commit, "", data);
		திரும்பு;
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
எண்ணிக்கை stdin_pack_info_kind {
	STDIN_PACK_INCLUDE = (1<<0),
	STDIN_PACK_EXCLUDE_CLOSED = (1<<1),
	STDIN_PACK_EXCLUDE_OPEN = (1<<2),
};

கட்டமைப்பு stdin_pack_info {
	கட்டமைப்பு packed_git *p;
	எண்ணிக்கை stdin_pack_info_kind kind;
};

நிலையான முழு pack_mtime_cmp(மாறிலி வெறுமை *_a, மாறிலி வெறுமை *_b)
{
	கட்டமைப்பு stdin_pack_info *a = ((மாறிலி கட்டமைப்பு string_list_item*)_a)->util;
	கட்டமைப்பு stdin_pack_info *b = ((மாறிலி கட்டமைப்பு string_list_item*)_b)->util;

	/*
	 * order packs by descending mtime so that objects are laid out
	 * roughly as newest-to-oldest
	 */
	எனில் (a->p->mtime < b->p->mtime)
		திரும்பு 1;
	இல்லையெனில் எனில் (b->p->mtime < a->p->mtime)
		திரும்பு -1;
	இல்லையெனில்
		திரும்பு 0;
}

நிலையான முழு stdin_packs_include_check_obj(கட்டமைப்பு object *obj, வெறுமை *data UNUSED)
{
	திரும்பு !has_object_kept_pack(to_pack.repo, &obj->oid,
				     KEPT_PACK_IN_CORE);
}

நிலையான முழு stdin_packs_include_check(கட்டமைப்பு commit *commit, வெறுமை *data)
{
	திரும்பு stdin_packs_include_check_obj((கட்டமைப்பு object *)commit, data);
}

நிலையான வெறுமை stdin_packs_add_pack_entries(கட்டமைப்பு strmap *packs,
					 கட்டமைப்பு rev_info *revs)
{
	கட்டமைப்பு string_list சாவிகள் = STRING_LIST_INIT_NODUP;
	கட்டமைப்பு string_list_item *item;
	கட்டமைப்பு hashmap_iter iter;
	கட்டமைப்பு strmap_entry *entry;

	strmap_for_each_entry(packs, &iter, entry) {
		கட்டமைப்பு stdin_pack_info *info = entry->மதிப்பு;
		எனில் (!info->p)
			die(_("could not find pack '%s'"), entry->சாவி);

		string_list_append(&சாவிகள், entry->சாவி)->util = info;
	}

	/*
	 * Order packs by ascending mtime; use QSORT directly to access the
	 * string_list_item's ->util pointer, which string_list_sort() does not
	 * provide.
	 */
	QSORT(சாவிகள்.பொருட்கள், சாவிகள்.nr, pack_mtime_cmp);

	for_each_string_list_item(item, &சாவிகள்) {
		கட்டமைப்பு stdin_pack_info *info = item->util;

		எனில் (info->kind & STDIN_PACK_EXCLUDE_OPEN) {
			/*
			 * When open-excluded packs ("!") are present, stop
			 * the parent walk at closed-excluded ("^") packs.
			 * Objects behind a "^" boundary are guaranteed to
			 * have closure and should not be rescued.
			 */
			revs->include_check = stdin_packs_include_check;
			revs->include_check_obj = stdin_packs_include_check_obj;
		}

		எனில் ((info->kind & STDIN_PACK_INCLUDE) ||
		    (info->kind & STDIN_PACK_EXCLUDE_OPEN))
			for_each_object_in_pack(info->p,
						add_object_entry_from_pack,
						revs,
						ODB_FOR_EACH_OBJECT_PACK_ORDER);
	}

	string_list_clear(&சாவிகள், 0);
}

நிலையான வெறுமை stdin_packs_read_உள்ளீடு(கட்டமைப்பு rev_info *revs,
				   எண்ணிக்கை stdin_packs_mode mode)
{
	கட்டமைப்பு strbuf buf = STRBUF_INIT;
	கட்டமைப்பு strmap packs = STRMAP_INIT;
	கட்டமைப்பு packed_git *p;

	போது (strbuf_getline(&buf, stdin) != EOF) {
		கட்டமைப்பு stdin_pack_info *info;
		எண்ணிக்கை stdin_pack_info_kind kind = STDIN_PACK_INCLUDE;
		மாறிலி எழுத்து *சாவி = buf.buf;

		எனில் (!*சாவி)
			தொடர்;
		இல்லையெனில் எனில் (*சாவி == '^')
			kind = STDIN_PACK_EXCLUDE_CLOSED;
		இல்லையெனில் எனில் (*சாவி == '!' && mode == STDIN_PACKS_MODE_FOLLOW)
			kind = STDIN_PACK_EXCLUDE_OPEN;

		எனில் (kind != STDIN_PACK_INCLUDE)
			சாவி++;

		info = strmap_get(&packs, சாவி);
		எனில் (!info) {
			CALLOC_ARRAY(info, 1);
			strmap_put(&packs, சாவி, info);
		}

		info->kind |= kind;

		strbuf_reset(&buf);
	}

	repo_for_each_pack(the_repository, p) {
		கட்டமைப்பு stdin_pack_info *info;

		info = strmap_get(&packs, pack_basename(p));
		எனில் (!info)
			தொடர்;

		எனில் (info->kind & STDIN_PACK_INCLUDE) {
			எனில் (exclude_promisor_objects && p->pack_promisor)
				die(_("packfile %s is a promisor but --exclude-promisor-objects was given"), p->pack_name);

			/*
			 * Arguments we got on stdin may not even be
			 * packs. First check that to avoid segfaulting
			 * later on in e.g.  pack_mtime_cmp(), excluded
			 * packs are handled below.
			 */
			எனில் (!is_pack_valid(p))
				die(_("packfile %s cannot be accessed"), p->pack_name);
		}

		எனில் (info->kind & STDIN_PACK_EXCLUDE_CLOSED) {
			/*
			 * Marking excluded packs as kept in-core so
			 * that later calls to add_object_entry()
			 * discards any objects that are also found in
			 * excluded packs.
			 */
			p->pack_keep_in_core = 1;
		}

		எனில் (info->kind & STDIN_PACK_EXCLUDE_OPEN) {
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

நிலையான வெறுமை add_unreachable_loose_objects(கட்டமைப்பு rev_info *revs);

நிலையான வெறுமை read_stdin_packs(கட்டமைப்பு repository *repo,
			     எண்ணிக்கை stdin_packs_mode mode, முழு rev_list_unpacked)
{
	முழு prev_fetch_if_missing = repo->fetch_if_missing;
	கட்டமைப்பு rev_info revs;

	/*
	 * The revision walk may hit objects that are promised, only. As the
	 * walk is best-effort though we don't want to perform backfill fetches
	 * for them.
	 */
	repo->fetch_if_missing = 0;

	repo_init_revisions(repo, &revs, வெற்று);
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
	revs.மரம்_objects = 1;
	revs.tag_objects = 1;
	revs.ignore_missing_links = 1;
	revs.exclude_promisor_objects = exclude_promisor_objects;

	/* avoids adding objects in excluded packs */
	ignore_packed_keep_in_core = 1;
	எனில் (mode == STDIN_PACKS_MODE_FOLLOW) {
		/*
		 * In '--stdin-packs=follow' mode, additionally ignore
		 * objects in excluded-open packs to prevent them from
		 * appearing in the resulting pack.
		 */
		ignore_packed_keep_in_core_open = 1;
	}
	stdin_packs_read_உள்ளீடு(&revs, mode);
	எனில் (rev_list_unpacked)
		add_unreachable_loose_objects(&revs);

	எனில் (prepare_revision_walk(&revs))
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

நிலையான வெறுமை add_cruft_object_entry(மாறிலி கட்டமைப்பு object_id *oid, எண்ணிக்கை object_type type,
				   கட்டமைப்பு packed_git *pack, off_t offset,
				   மாறிலி எழுத்து *name, uint32_t mtime)
{
	கட்டமைப்பு object_entry *entry;

	display_progress(progress_state, ++nr_seen);

	entry = packlist_find(&to_pack, oid);
	எனில் (entry) {
		எனில் (name) {
			entry->hash = pack_name_hash_fn(name);
			entry->no_try_delta = no_try_delta(name);
		}
	} இல்லையெனில் {
		எனில் (!want_object_in_pack_mtime(oid, 0, &pack, &offset, mtime))
			திரும்பு;
		எனில் (!pack && type == OBJ_BLOB) {
			கட்டமைப்பு odb_source_files *files =
				odb_source_files_downcast(the_repository->objects->source);
			முழு found = 0;

			க்கு (கட்டமைப்பு odb_files_dir *dir = files->dirs; !found && dir; dir = dir->next)
				எனில் (!odb_source_read_object_info(&dir->loose->base, oid, வெற்று, 0, வெற்று))
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
			எனில் (!found)
				திரும்பு;
		}

		entry = create_object_entry(oid, type, pack_name_hash_fn(name),
					    0, name && no_try_delta(name),
					    pack, offset);
	}

	எனில் (mtime > oe_cruft_mtime(&to_pack, entry))
		oe_set_cruft_mtime(&to_pack, entry, mtime);
	திரும்பு;
}

நிலையான வெறுமை show_cruft_object(கட்டமைப்பு object *obj, மாறிலி எழுத்து *name, வெறுமை *data UNUSED)
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
	add_cruft_object_entry(&obj->oid, obj->type, வெற்று, 0, name, cruft_expiration);
}

நிலையான வெறுமை show_cruft_commit(கட்டமைப்பு commit *commit, வெறுமை *data)
{
	show_cruft_object((கட்டமைப்பு object*)commit, வெற்று, data);
}

நிலையான முழு cruft_include_check_obj(கட்டமைப்பு object *obj, வெறுமை *data UNUSED)
{
	திரும்பு !has_object_kept_pack(to_pack.repo, &obj->oid, KEPT_PACK_IN_CORE);
}

நிலையான முழு cruft_include_check(கட்டமைப்பு commit *commit, வெறுமை *data)
{
	திரும்பு cruft_include_check_obj((கட்டமைப்பு object*)commit, data);
}

நிலையான வெறுமை set_cruft_mtime(மாறிலி கட்டமைப்பு object *object,
			    கட்டமைப்பு packed_git *pack,
			    off_t offset, time_t mtime)
{
	add_cruft_object_entry(&object->oid, object->type, pack, offset, வெற்று,
			       mtime);
}

நிலையான வெறுமை mark_pack_kept_in_core(கட்டமைப்பு string_list *packs, unsigned keep)
{
	கட்டமைப்பு string_list_item *item = வெற்று;
	for_each_string_list_item(item, packs) {
		கட்டமைப்பு packed_git *p = item->util;
		எனில் (!p)
			die(_("could not find pack '%s'"), item->string);
		எனில் (p->is_cruft && keep)
			ignore_packed_keep_in_core_has_cruft = 1;
		p->pack_keep_in_core = keep;
	}
}

நிலையான வெறுமை add_objects_in_unpacked_packs(வெறுமை);

நிலையான வெறுமை enumerate_cruft_objects(வெறுமை)
{
	எனில் (progress)
		progress_state = start_progress(the_repository,
						_("Enumerating cruft objects"), 0);

	add_objects_in_unpacked_packs();
	add_unreachable_loose_objects(வெற்று);

	stop_progress(&progress_state);
}

நிலையான வெறுமை enumerate_and_traverse_cruft_objects(கட்டமைப்பு string_list *fresh_packs)
{
	கட்டமைப்பு packed_git *p;
	கட்டமைப்பு rev_info revs;
	முழு ret;

	repo_init_revisions(the_repository, &revs, வெற்று);

	revs.tag_objects = 1;
	revs.மரம்_objects = 1;
	revs.blob_objects = 1;

	revs.include_check = cruft_include_check;
	revs.include_check_obj = cruft_include_check_obj;

	revs.ignore_missing_links = 1;

	எனில் (progress)
		progress_state = start_progress(the_repository,
						_("Enumerating cruft objects"), 0);
	ret = add_unseen_recent_objects_to_traversal(&revs, cruft_expiration,
						     set_cruft_mtime, 1);
	stop_progress(&progress_state);

	எனில் (ret)
		die(_("unable to add cruft objects"));

	/*
	 * Re-mark only the fresh packs as kept so that objects in
	 * unknown packs do not halt the reachability traversal early.
	 */
	repo_for_each_pack(the_repository, p)
		p->pack_keep_in_core = 0;
	mark_pack_kept_in_core(fresh_packs, 1);

	எனில் (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	எனில் (progress)
		progress_state = start_progress(the_repository,
						_("Traversing cruft objects"), 0);
	nr_seen = 0;
	traverse_commit_list(&revs, show_cruft_commit, show_cruft_object, வெற்று);

	stop_progress(&progress_state);
	release_revisions(&revs);
}

நிலையான வெறுமை read_cruft_objects(வெறுமை)
{
	கட்டமைப்பு strbuf buf = STRBUF_INIT;
	கட்டமைப்பு string_list discard_packs = STRING_LIST_INIT_DUP;
	கட்டமைப்பு string_list fresh_packs = STRING_LIST_INIT_DUP;
	கட்டமைப்பு packed_git *p;

	ignore_packed_keep_in_core = 1;

	போது (strbuf_getline(&buf, stdin) != EOF) {
		எனில் (!buf.len)
			தொடர்;

		எனில் (*buf.buf == '-')
			string_list_append(&discard_packs, buf.buf + 1);
		இல்லையெனில்
			string_list_append(&fresh_packs, buf.buf);
	}

	string_list_sort(&discard_packs);
	string_list_sort(&fresh_packs);

	repo_for_each_pack(the_repository, p) {
		மாறிலி எழுத்து *pack_name = pack_basename(p);
		கட்டமைப்பு string_list_item *item;

		item = string_list_lookup(&fresh_packs, pack_name);
		எனில் (!item)
			item = string_list_lookup(&discard_packs, pack_name);

		எனில் (item) {
			item->util = p;
		} இல்லையெனில் {
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

	எனில் (cruft_expiration)
		enumerate_and_traverse_cruft_objects(&fresh_packs);
	இல்லையெனில்
		enumerate_cruft_objects();

	strbuf_release(&buf);
	string_list_clear(&discard_packs, 0);
	string_list_clear(&fresh_packs, 0);
}

நிலையான வெறுமை read_object_list_from_stdin(வெறுமை)
{
	எழுத்து வரி[GIT_MAX_HEXSZ + 1 + PATH_MAX + 2];
	கட்டமைப்பு object_id oid;
	மாறிலி எழுத்து *p;

	க்கு (;;) {
		எனில் (!fgets(வரி, அளவு(வரி), stdin)) {
			எனில் (feof(stdin))
				நிறுத்து;
			எனில் (!ferror(stdin))
				BUG("fgets returned NULL, not EOF, not error!");
			எனில் (errno != EINTR)
				die_errno("fgets");
			clearerr(stdin);
			தொடர்;
		}
		எனில் (வரி[0] == '-') {
			எனில் (get_oid_hex(வரி+1, &oid))
				die(_("expected edge object ID, got garbage:\n %s"),
				    வரி);
			add_preferred_base(&oid);
			தொடர்;
		}
		எனில் (parse_oid_hex(வரி, &oid, &p))
			die(_("expected object ID, got garbage:\n %s"), வரி);

		add_preferred_base_object(p + 1);
		add_object_entry(&oid, OBJ_NONE, p + 1, 0);
	}
}

நிலையான வெறுமை show_commit(கட்டமைப்பு commit *commit, வெறுமை *data UNUSED)
{
	add_object_entry(&commit->object.oid, OBJ_COMMIT, வெற்று, 0);

	எனில் (write_bitmap_index)
		index_commit_for_bitmap(commit);

	எனில் (use_delta_islands)
		propagate_island_marks(the_repository, commit);
}

நிலையான வெறுமை show_object(கட்டமைப்பு object *obj, மாறிலி எழுத்து *name,
			வெறுமை *data UNUSED)
{
	add_preferred_base_object(name);
	add_object_entry(&obj->oid, obj->type, name, 0);

	எனில் (use_delta_islands)
		record_மரம்_depth(&obj->oid, name);
}

நிலையான வெறுமை show_object__ma_allow_any(கட்டமைப்பு object *obj, மாறிலி எழுத்து *name, வெறுமை *data)
{
	ப்ரதிஜ்ஞா(arg_missing_action == MA_ALLOW_ANY);

	/*
	 * Quietly ignore ALL missing objects.  This avoids problems with
	 * staging them now and getting an odd error later.
	 */
	எனில் (!odb_has_object(the_repository->objects, &obj->oid, 0))
		திரும்பு;

	show_object(obj, name, data);
}

நிலையான வெறுமை show_object__ma_allow_promisor(கட்டமைப்பு object *obj, மாறிலி எழுத்து *name, வெறுமை *data)
{
	ப்ரதிஜ்ஞா(arg_missing_action == MA_ALLOW_PROMISOR);

	/*
	 * Quietly ignore EXPECTED missing objects.  This avoids problems with
	 * staging them now and getting an odd error later.
	 */
	எனில் (!odb_has_object(the_repository->objects, &obj->oid, 0) &&
	    is_promisor_object(to_pack.repo, &obj->oid))
		திரும்பு;

	show_object(obj, name, data);
}

நிலையான முழு option_parse_missing_action(மாறிலி கட்டமைப்பு option *opt,
				       மாறிலி எழுத்து *arg, முழு unset)
{
	கட்டமைப்பு repository *repo = opt->மதிப்பு;

	ப்ரதிஜ்ஞா(arg);
	ப்ரதிஜ்ஞா(!unset);

	எனில் (!strcmp(arg, "error")) {
		arg_missing_action = MA_ERROR;
		fn_show_object = show_object;
		திரும்பு 0;
	}

	எனில் (!strcmp(arg, "allow-any")) {
		arg_missing_action = MA_ALLOW_ANY;
		repo->fetch_if_missing = 0;
		fn_show_object = show_object__ma_allow_any;
		திரும்பு 0;
	}

	எனில் (!strcmp(arg, "allow-promisor")) {
		arg_missing_action = MA_ALLOW_PROMISOR;
		repo->fetch_if_missing = 0;
		fn_show_object = show_object__ma_allow_promisor;
		திரும்பு 0;
	}

	die(_("invalid value for '%s': '%s'"), "--missing", arg);
	திரும்பு 0;
}

நிலையான வெறுமை show_edge(கட்டமைப்பு commit *commit)
{
	add_preferred_base(&commit->object.oid);
}

நிலையான முழு add_object_in_unpacked_pack(மாறிலி கட்டமைப்பு object_id *oid,
				       கட்டமைப்பு object_info *oi,
				       வெறுமை *data UNUSED)
{
	எனில் (cruft) {
		add_cruft_object_entry(oid, OBJ_NONE, oi->source_infop->u.packed.pack,
				       oi->source_infop->u.packed.offset, வெற்று,
				       *oi->mtimep);
	} இல்லையெனில் {
		add_object_entry(oid, OBJ_NONE, "", 0);
	}
	திரும்பு 0;
}

நிலையான வெறுமை add_objects_in_unpacked_packs(வெறுமை)
{
	கட்டமைப்பு odb_source_files *files =
		odb_source_files_downcast(to_pack.repo->objects->source);
	time_t mtime;
	கட்டமைப்பு odb_for_each_object_options opts = {
		.flags = ODB_FOR_EACH_OBJECT_PACK_ORDER |
			 ODB_FOR_EACH_OBJECT_LOCAL_ONLY |
			 ODB_FOR_EACH_OBJECT_SKIP_IN_CORE_KEPT_PACKS |
			 ODB_FOR_EACH_OBJECT_SKIP_ON_DISK_KEPT_PACKS,
	};
	கட்டமைப்பு odb_source_info source_info;
	கட்டமைப்பு object_info oi = {
		.mtimep = &mtime,
		.source_infop = &source_info,
	};

	க்கு (கட்டமைப்பு odb_files_dir *dir = files->dirs; dir; dir = dir->next) {
		எனில் (!dir->local)
			தொடர்;

		எனில் (odb_source_for_each_object(&dir->packed->base, &oi,
					       add_object_in_unpacked_pack, வெற்று, &opts))
			die(_("cannot open pack index"));
	}
}

நிலையான முழு add_loose_object(மாறிலி கட்டமைப்பு object_id *oid, மாறிலி எழுத்து *path,
			    வெறுமை *data)
{
	கட்டமைப்பு rev_info *revs = data;
	எண்ணிக்கை object_type type = odb_read_object_info(the_repository->objects, oid, வெற்று);

	எனில் (type < 0) {
		warning(_("loose object at %s could not be examined"), path);
		திரும்பு 0;
	}

	எனில் (cruft) {
		கட்டமைப்பு stat st;
		எனில் (stat(path, &st) < 0) {
			எனில் (errno == ENOENT)
				திரும்பு 0;
			திரும்பு error_errno("unable to stat %s", oid_to_hex(oid));
		}

		add_cruft_object_entry(oid, type, வெற்று, 0, வெற்று,
				       st.st_mtime);
	} இல்லையெனில் {
		add_object_entry(oid, type, "", 0);
	}

	எனில் (revs && type == OBJ_COMMIT)
		add_pending_oid(revs, வெற்று, oid, 0);

	திரும்பு 0;
}

/*
 * We actually don't even have to worry about reachability here.
 * add_object_entry will weed out duplicates, so we just add every
 * loose object we find.
 */
நிலையான வெறுமை add_unreachable_loose_objects(கட்டமைப்பு rev_info *revs)
{
	for_each_loose_file_in_source(the_repository->objects->source,
				      add_loose_object, வெற்று, வெற்று, revs);
}

நிலையான முழு has_sha1_pack_kept_or_nonlocal(மாறிலி கட்டமைப்பு object_id *oid)
{
	நிலையான கட்டமைப்பு packed_git *last_found = வெற்று;
	கட்டமைப்பு packed_git *p;

	எனில் (last_found && find_pack_entry_one(oid, last_found))
		திரும்பு 1;

	repo_for_each_pack(the_repository, p) {
		/*
		 * We have already checked `last_found`, so there is no need to
		 * re-check here.
		 */
		எனில் (p == last_found)
			தொடர்;

		எனில் ((!p->pack_local || p->pack_keep || p->pack_keep_in_core) &&
		    find_pack_entry_one(oid, p)) {
			last_found = p;
			திரும்பு 1;
		}
	}

	திரும்பு 0;
}

/*
 * Store a list of sha1s that are should not be discarded
 * because they are either written too recently, or are
 * reachable from another object that was.
 *
 * This is filled by get_object_list.
 */
நிலையான கட்டமைப்பு oid_array recent_objects;

நிலையான முழு loosened_object_can_be_discarded(மாறிலி கட்டமைப்பு object_id *oid,
					    timestamp_t mtime)
{
	எனில் (!unpack_unreachable_expiration)
		திரும்பு 0;
	எனில் (mtime > unpack_unreachable_expiration)
		திரும்பு 0;
	எனில் (oid_array_lookup(&recent_objects, oid) >= 0)
		திரும்பு 0;
	திரும்பு 1;
}

நிலையான முழு force_object_loose(கட்டமைப்பு odb_source *source,
			      மாறிலி கட்டமைப்பு object_id *oid,
			      மாறிலி time_t *mtime)
{
	கட்டமைப்பு odb_source_files *files = odb_source_files_downcast(source);
	மாறிலி கட்டமைப்பு git_hash_algo *compat = source->odb->repo->compat_hash_algo;
	கட்டமைப்பு object_info oi = OBJECT_INFO_INIT;
	கட்டமைப்பு object_id compat_oid, *compat_oid_p = வெற்று;
	எண்ணிக்கை object_type type;
	வெறுமை *buf = வெற்று;
	size_t len;
	முழு ret;

	க்கு (கட்டமைப்பு odb_files_dir *dir = files->dirs; dir; dir = dir->next)
		எனில் (!odb_source_read_object_info(&dir->loose->base, oid, வெற்று, 0, வெற்று))
			திரும்பு 0;

	oi.typep = &type;
	oi.sizep = &len;
	oi.contentp = &buf;
	எனில் (odb_read_object_info_extended(source->odb, oid, &oi, 0)) {
		ret = error(_("cannot read object for %s"), oid_to_hex(oid));
		செல் out;
	}

	எனில் (compat) {
		எனில் (repo_oid_to_algop(source->odb->repo, oid, compat, &compat_oid)) {
			ret = error(_("cannot map object %s to %s"),
				    oid_to_hex(oid), compat->name);
			செல் out;
		}

		compat_oid_p = &compat_oid;
	}

	ret = odb_source_write_object(&files->dirs->loose->base, buf, len, type, oid,
				      compat_oid_p, mtime, 0);

out:
	விடுவி(buf);
	திரும்பு ret;
}

நிலையான வெறுமை loosen_unused_packed_objects(வெறுமை)
{
	கட்டமைப்பு packed_git *p;
	uint32_t i;
	uint32_t loosened_objects_nr = 0;
	கட்டமைப்பு object_id oid;

	repo_for_each_pack(the_repository, p) {
		எனில் (!p->pack_local || p->pack_keep || p->pack_keep_in_core)
			தொடர்;

		எனில் (open_pack_index(p))
			die(_("cannot open pack index"));

		க்கு (i = 0; i < p->இலக்கம்_objects; i++) {
			nth_packed_object_id(&oid, p, i);
			எனில் (!packlist_find(&to_pack, &oid) &&
			    !has_sha1_pack_kept_or_nonlocal(&oid) &&
			    !loosened_object_can_be_discarded(&oid, p->mtime)) {
				எனில் (force_object_loose(the_repository->objects->source,
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
நிலையான முழு pack_options_allow_reuse(வெறுமை)
{
	திரும்பு allow_pack_reuse != NO_PACK_REUSE &&
	       pack_to_stdout &&
	       !ignore_packed_keep_on_disk &&
	       !ignore_packed_keep_in_core &&
	       (!local || !have_non_local_packs) &&
	       !incremental;
}

நிலையான முழு get_object_list_from_bitmap(கட்டமைப்பு rev_info *revs)
{
	எனில் (!(bitmap_git = prepare_bitmap_walk(revs, 0)))
		திரும்பு -1;

	/*
	 * For now, force the name-hash version to be 1 since that
	 * is the version implied by the bitmap format. Later, the
	 * format can include this version explicitly in its format,
	 * allowing readers to know the version that was used during
	 * the bitmap write.
	 */
	name_hash_version = 1;

	எனில் (pack_options_allow_reuse())
		reuse_partial_packfile_from_bitmap(bitmap_git,
						   &reuse_packfiles,
						   &reuse_packfiles_nr,
						   &reuse_packfile_bitmap,
						   allow_pack_reuse == MULTI_PACK_REUSE);

	எனில் (reuse_packfiles) {
		reuse_packfile_objects = bitmap_popcount(reuse_packfile_bitmap);
		எனில் (!reuse_packfile_objects)
			BUG("expected non-empty reuse bitmap");

		nr_result += reuse_packfile_objects;
		nr_seen += reuse_packfile_objects;
		display_progress(progress_state, nr_seen);
	}

	traverse_bitmap_commit_list(bitmap_git, revs,
				    &add_object_entry_from_bitmap);
	திரும்பு 0;
}

நிலையான வெறுமை record_recent_object(கட்டமைப்பு object *obj,
				 மாறிலி எழுத்து *name UNUSED,
				 வெறுமை *data UNUSED)
{
	oid_array_append(&recent_objects, &obj->oid);
}

நிலையான வெறுமை record_recent_commit(கட்டமைப்பு commit *commit, வெறுமை *data UNUSED)
{
	oid_array_append(&recent_objects, &commit->object.oid);
}

நிலையான முழு mark_bitmap_preferred_tip(மாறிலி கட்டமைப்பு reference *ref, வெறுமை *data UNUSED)
{
	மாறிலி கட்டமைப்பு object_id *maybe_peeled = ref->oid;
	கட்டமைப்பு object_id peeled;
	கட்டமைப்பு object *object;

	எனில் (!reference_get_peeled_oid(the_repository, ref, &peeled))
		maybe_peeled = &peeled;

	object = parse_object_or_die(the_repository, maybe_peeled, ref->name);
	எனில் (object->type == OBJ_COMMIT)
		object->flags |= NEEDS_BITMAP;

	திரும்பு 0;
}

நிலையான உள்வரி முழு is_oid_uninteresting(கட்டமைப்பு repository *repo,
				       கட்டமைப்பு object_id *oid)
{
	கட்டமைப்பு object *o = lookup_object(repo, oid);
	திரும்பு !o || (o->flags & UNINTERESTING);
}

நிலையான முழு add_objects_by_path(மாறிலி எழுத்து *path,
			       கட்டமைப்பு oid_array *oids,
			       எண்ணிக்கை object_type type,
			       வெறுமை *data)
{
	size_t oe_start = to_pack.nr_objects;
	size_t oe_end;
	unsigned முழு *processed = data;

	/*
	 * First, add all objects to the packing data, including the ones
	 * marked UNINTERESTING (translated to 'exclude') as they can be
	 * used as delta bases.
	 */
	க்கு (size_t i = 0; i < oids->nr; i++) {
		முழு exclude;
		கட்டமைப்பு object_info oi = OBJECT_INFO_INIT;
		கட்டமைப்பு object_id *oid = &oids->oid[i];

		/* Skip objects that do not exist locally. */
		எனில் ((exclude_promisor_objects || arg_missing_action != MA_ERROR) &&
		    odb_read_object_info_extended(the_repository->objects, oid, &oi,
						  OBJECT_INFO_FOR_PREFETCH) < 0)
			தொடர்;

		exclude = is_oid_uninteresting(the_repository, oid);

		எனில் (exclude && !thin)
			தொடர்;

		add_object_entry(oid, type, path, exclude);

		எனில் (type == OBJ_COMMIT) {
			கட்டமைப்பு commit *commit;

			எனில் (!write_bitmap_index && !use_delta_islands)
				தொடர்;

			commit = lookup_commit(the_repository, oid);
			எனில் (!commit)
				die(_("could not find commit %s"), oid_to_hex(oid));
			எனில் (write_bitmap_index)
				index_commit_for_bitmap(commit);
			/*
			 * Skip island propagation for boundary commits.
			 * The regular traversal's show_commit() is only
			 * called for interesting commits; matching that
			 * here keeps path-walk from doing extra work that
			 * would only be a no-op anyway (boundary commits
			 * are not in island_marks).
			 */
			எனில் (use_delta_islands && !exclude)
				propagate_island_marks(the_repository, commit);
		} இல்லையெனில் எனில் (type == OBJ_மரம் && use_delta_islands) {
			record_மரம்_depth(oid, path);
		}
	}

	oe_end = to_pack.nr_objects;

	/* We can skip delta calculations if it is a no-op. */
	எனில் (oe_end == oe_start || !window)
		திரும்பு 0;

	ALLOC_GROW(to_pack.regions,
		   to_pack.nr_regions + 1,
		   to_pack.nr_regions_alloc);

	to_pack.regions[to_pack.nr_regions].start = oe_start;
	to_pack.regions[to_pack.nr_regions].nr = oe_end - oe_start;
	to_pack.nr_regions++;

	*processed += oids->nr;
	display_progress(progress_state, *processed);

	திரும்பு 0;
}

நிலையான முழு get_object_list_path_walk(கட்டமைப்பு rev_info *revs)
{
	கட்டமைப்பு path_walk_info info = PATH_WALK_INFO_INIT;
	unsigned முழு processed = 0;
	முழு result;

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

	திரும்பு result;
}

நிலையான வெறுமை get_object_list(கட்டமைப்பு rev_info *revs, கட்டமைப்பு strvec *argv)
{
	கட்டமைப்பு setup_revision_opt s_r_opt = {
		.allow_exclude_promisor_objects = 1,
	};
	கட்டமைப்பு repo_config_மதிப்புகள் *cfg = repo_config_மதிப்புகள்(the_repository);
	எழுத்து வரி[1000];
	முழு flags = 0;
	முழு save_warning;

	save_commit_buffer = 0;
	setup_revisions_from_strvec(argv, revs, &s_r_opt);

	/* make sure shallows are read */
	is_repository_shallow(the_repository);

	save_warning = cfg->warn_on_object_refname_ambiguity;
	cfg->warn_on_object_refname_ambiguity = 0;

	போது (fgets(வரி, அளவு(வரி), stdin) != வெற்று) {
		முழு len = strlen(வரி);
		எனில் (len && வரி[len - 1] == '\n')
			வரி[--len] = 0;
		எனில் (!len)
			நிறுத்து;
		எனில் (*வரி == '-') {
			எனில் (!strcmp(வரி, "--not")) {
				flags ^= UNINTERESTING;
				write_bitmap_index = 0;
				தொடர்;
			}
			எனில் (starts_with(வரி, "--shallow ")) {
				கட்டமைப்பு object_id oid;
				எனில் (get_oid_hex(வரி + 10, &oid))
					die("not an object name '%s'", வரி + 10);
				register_shallow(the_repository, &oid);
				use_bitmap_index = 0;
				தொடர்;
			}
			die(_("not a rev '%s'"), வரி);
		}
		எனில் (handle_revision_arg(வரி, revs, flags, REVARG_CANNOT_BE_FILENAME))
			die(_("bad revision '%s'"), வரி);
	}

	cfg->warn_on_object_refname_ambiguity = save_warning;

	எனில் (use_bitmap_index && !get_object_list_from_bitmap(revs))
		திரும்பு;

	எனில் (use_delta_islands)
		load_delta_islands(the_repository, progress);

	எனில் (write_bitmap_index)
		for_each_preferred_bitmap_tip(the_repository, mark_bitmap_preferred_tip,
					      வெற்று);

	எனில் (!fn_show_object)
		fn_show_object = show_object;

	எனில் (path_walk) {
		எனில் (get_object_list_path_walk(revs)) {
			warning(_("failed to pack objects via path-walk"));
			path_walk = 0;
		}
	}

	எனில் (!path_walk) {
		எனில் (prepare_revision_walk(revs))
			die(_("revision walk setup failed"));
		mark_edges_uninteresting(revs, show_edge, sparse);
		traverse_commit_list(revs,
				show_commit, fn_show_object,
				வெற்று);
	}

	எனில் (unpack_unreachable_expiration) {
		revs->ignore_missing_links = 1;
		எனில் (add_unseen_recent_objects_to_traversal(revs,
				unpack_unreachable_expiration, வெற்று, 0))
			die(_("unable to add recent objects"));
		எனில் (prepare_revision_walk(revs))
			die(_("revision walk setup failed"));
		traverse_commit_list(revs, record_recent_commit,
				     record_recent_object, வெற்று);
	}

	எனில் (keep_unreachable)
		add_objects_in_unpacked_packs();
	எனில் (pack_loose_unreachable)
		add_unreachable_loose_objects(வெற்று);
	எனில் (unpack_unreachable)
		loosen_unused_packed_objects();

	oid_array_clear(&recent_objects);
}

நிலையான வெறுமை add_extra_kept_packs(மாறிலி கட்டமைப்பு string_list *names)
{
	கட்டமைப்பு packed_git *p;

	எனில் (!names->nr)
		திரும்பு;

	repo_for_each_pack(the_repository, p) {
		மாறிலி எழுத்து *name = basename(p->pack_name);
		முழு i;

		எனில் (!p->pack_local)
			தொடர்;

		க்கு (i = 0; i < names->nr; i++)
			எனில் (!fspathcmp(name, names->பொருட்கள்[i].string))
				நிறுத்து;

		எனில் (i < names->nr) {
			p->pack_keep_in_core = 1;
			ignore_packed_keep_in_core = 1;
			தொடர்;
		}
	}
}

நிலையான முழு option_parse_quiet(மாறிலி கட்டமைப்பு option *opt, மாறிலி எழுத்து *arg,
			      முழு unset)
{
	முழு *val = opt->மதிப்பு;

	BUG_ON_OPT_ARG(arg);

	எனில் (!unset)
		*val = 0;
	இல்லையெனில் எனில் (!*val)
		*val = 1;
	திரும்பு 0;
}

நிலையான முழு option_parse_index_version(மாறிலி கட்டமைப்பு option *opt,
				      மாறிலி எழுத்து *arg, முழு unset)
{
	கட்டமைப்பு pack_idx_option *popts = opt->மதிப்பு;
	எழுத்து *c;
	மாறிலி எழுத்து *val = arg;

	BUG_ON_OPT_NEG(unset);

	popts->version = strtoul(val, &c, 10);
	எனில் (popts->version > 2)
		die(_("unsupported index version %s"), val);
	எனில் (*c == ',' && c[1])
		popts->off32_limit = strtoul(c+1, &c, 0);
	எனில் (*c || popts->off32_limit & 0x80000000)
		die(_("bad index version '%s'"), val);
	திரும்பு 0;
}

நிலையான முழு option_parse_unpack_unreachable(மாறிலி கட்டமைப்பு option *opt UNUSED,
					   மாறிலி எழுத்து *arg, முழு unset)
{
	எனில் (unset) {
		unpack_unreachable = 0;
		unpack_unreachable_expiration = 0;
	}
	இல்லையெனில் {
		unpack_unreachable = 1;
		எனில் (arg)
			unpack_unreachable_expiration = approxidate(arg);
	}
	திரும்பு 0;
}

நிலையான முழு option_parse_cruft_expiration(மாறிலி கட்டமைப்பு option *opt UNUSED,
					 மாறிலி எழுத்து *arg, முழு unset)
{
	எனில் (unset) {
		cruft = 0;
		cruft_expiration = 0;
	} இல்லையெனில் {
		cruft = 1;
		எனில் (arg)
			cruft_expiration = approxidate(arg);
	}
	திரும்பு 0;
}

நிலையான முழு is_not_in_promisor_pack_obj(கட்டமைப்பு object *obj, வெறுமை *data UNUSED)
{
	கட்டமைப்பு odb_source_info source_info;
	கட்டமைப்பு object_info info = {
		.source_infop = &source_info,
	};

	எனில் (odb_read_object_info_extended(the_repository->objects, &obj->oid, &info, 0))
		BUG("should_include_obj should only be called on existing objects");
	திரும்பு source_info.source->type != ODB_SOURCE_PACKED || !source_info.u.packed.pack->pack_promisor;
}

நிலையான முழு is_not_in_promisor_pack(கட்டமைப்பு commit *commit, வெறுமை *data) {
	திரும்பு is_not_in_promisor_pack_obj((கட்டமைப்பு object *) commit, data);
}

நிலையான முழு parse_stdin_packs_mode(மாறிலி கட்டமைப்பு option *opt, மாறிலி எழுத்து *arg,
				  முழு unset)
{
	எண்ணிக்கை stdin_packs_mode *mode = opt->மதிப்பு;

	எனில் (unset)
		*mode = STDIN_PACKS_MODE_NONE;
	இல்லையெனில் எனில் (!arg || !*arg)
		*mode = STDIN_PACKS_MODE_STANDARD;
	இல்லையெனில் எனில் (!strcmp(arg, "follow"))
		*mode = STDIN_PACKS_MODE_FOLLOW;
	இல்லையெனில்
		die(_("invalid value for '%s': '%s'"), opt->long_name, arg);

	திரும்பு 0;
}

முழு cmd_pack_objects(முழு argc,
		     மாறிலி எழுத்து **argv,
		     மாறிலி எழுத்து *prefix,
		     கட்டமைப்பு repository *repo)
{
	முழு use_internal_rev_list = 0;
	முழு all_progress_implied = 0;
	கட்டமைப்பு strvec rp = STRVEC_INIT;
	முழு rev_list_unpacked = 0, rev_list_all = 0, rev_list_reflog = 0;
	முழு rev_list_index = 0;
	எண்ணிக்கை stdin_packs_mode stdin_packs = STDIN_PACKS_MODE_NONE;
	கட்டமைப்பு string_list keep_pack_list = STRING_LIST_INIT_NODUP;
	கட்டமைப்பு list_objects_filter_options filter_options =
		LIST_OBJECTS_FILTER_INIT;
	கட்டமைப்பு repo_config_மதிப்புகள் *cfg = repo_config_மதிப்புகள்(the_repository);

	கட்டமைப்பு option pack_objects_options[] = {
		OPT_CALLBACK_F('q', "quiet", &progress, வெற்று,
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
		OPT_INTEGER(0, "threads", &delta_தேடல்_threads,
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
		OPT_CALLBACK_F(0, "unpack-unreachable", வெற்று, N_("time"),
		  N_("unpack unreachable objects newer than <time>"),
		  PARSE_OPT_OPTARG, option_parse_unpack_unreachable),
		OPT_BOOL(0, "cruft", &cruft, N_("create a cruft pack")),
		OPT_CALLBACK_F(0, "cruft-expiration", வெற்று, N_("time"),
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

	எனில் (DFS_இலக்கம்_STATES > (1 << OE_DFS_STATE_BITS))
		BUG("too many dfs states, increase OE_DFS_STATE_BITS");

	disable_replace_refs();

	sparse = git_env_bool("GIT_TEST_PACK_SPARSE", -1);
	எனில் (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		எனில் (sparse < 0)
			sparse = the_repository->settings.pack_use_sparse;
		எனில் (the_repository->settings.pack_use_multi_pack_reuse)
			allow_pack_reuse = MULTI_PACK_REUSE;
	}

	reset_pack_idx_option(&pack_idx_opts);
	pack_idx_opts.flags |= WRITE_REV;
	repo_config(the_repository, git_pack_config, வெற்று);
	எனில் (git_env_bool(GIT_TEST_NO_WRITE_REV_INDEX, 0))
		pack_idx_opts.flags &= ~WRITE_REV;

	progress = isatty(2);
	argc = parse_options(argc, argv, prefix, pack_objects_options,
			     pack_usage, 0);

	எனில் (argc) {
		base_name = argv[0];
		argc--;
	}
	எனில் (pack_to_stdout != !base_name || argc)
		usage_with_options(pack_usage, pack_objects_options);

	எனில் (path_walk < 0) {
		எனில் (use_bitmap_index > 0 ||
		    !use_internal_rev_list)
			path_walk = 0;
		இல்லையெனில் எனில் (the_repository->gitdir &&
			 the_repository->settings.pack_use_path_walk)
			path_walk = 1;
		இல்லையெனில்
			path_walk = git_env_bool("GIT_TEST_PACK_PATH_WALK", 0);
	}

	எனில் (depth < 0)
		depth = 0;
	எனில் (depth >= (1 << OE_DEPTH_BITS)) {
		warning(_("delta chain depth %d is too deep, forcing %d"),
			depth, (1 << OE_DEPTH_BITS) - 1);
		depth = (1 << OE_DEPTH_BITS) - 1;
	}
	எனில் (cache_max_small_delta_size >= (1U << OE_Z_DELTA_BITS)) {
		warning(_("pack.deltaCacheLimit is too high, forcing %d"),
			(1U << OE_Z_DELTA_BITS) - 1);
		cache_max_small_delta_size = (1U << OE_Z_DELTA_BITS) - 1;
	}
	எனில் (window < 0)
		window = 0;

	strvec_தள்ளு(&rp, "pack-objects");

	எனில் (path_walk) {
		மாறிலி எழுத்து *option = வெற்று;
		எனில் (!path_walk_filter_compatible(&filter_options))
			option = "--filter";

		எனில் (option) {
			warning(_("cannot use %s with %s"),
				option, "--path-walk");
			path_walk = 0;
		}
	}
	எனில் (path_walk) {
		strvec_தள்ளு(&rp, "--objects");
	} இல்லையெனில் எனில் (thin) {
		use_internal_rev_list = 1;
		strvec_தள்ளு(&rp, shallow
				? "--objects-edge-aggressive"
				: "--objects-edge");
	} இல்லையெனில்
		strvec_தள்ளு(&rp, "--objects");

	எனில் (rev_list_all) {
		use_internal_rev_list = 1;
		strvec_தள்ளு(&rp, "--all");
	}
	எனில் (rev_list_reflog) {
		use_internal_rev_list = 1;
		strvec_தள்ளு(&rp, "--reflog");
	}
	எனில் (rev_list_index) {
		use_internal_rev_list = 1;
		strvec_தள்ளு(&rp, "--indexed-objects");
	}
	எனில் (rev_list_unpacked && !stdin_packs) {
		use_internal_rev_list = 1;
		strvec_தள்ளு(&rp, "--unpacked");
	}

	die_for_incompatible_opt2(exclude_promisor_objects,
				  "--exclude-promisor-objects",
				  exclude_promisor_objects_best_effort,
				  "--exclude-promisor-objects-best-effort");
	எனில் (exclude_promisor_objects) {
		repo->fetch_if_missing = 0;

		/* --stdin-packs handles promisor objects separately. */
		எனில் (!stdin_packs) {
			use_internal_rev_list = 1;
			strvec_தள்ளு(&rp, "--exclude-promisor-objects");
		}
	} இல்லையெனில் எனில் (exclude_promisor_objects_best_effort) {
		use_internal_rev_list = 1;
		arg_missing_action = MA_ALLOW_ANY;
		repo->fetch_if_missing = 0;
		fn_show_object = show_object__ma_allow_any;
		/* revs configured below */
	}
	எனில் (unpack_unreachable || keep_unreachable || pack_loose_unreachable)
		use_internal_rev_list = 1;

	எனில் (!reuse_object)
		reuse_delta = 0;
	எனில் (cfg->pack_compression_level == -1)
		cfg->pack_compression_level = Z_DEFAULT_COMPRESSION;
	இல்லையெனில் எனில் (cfg->pack_compression_level < 0 || cfg->pack_compression_level > Z_BEST_COMPRESSION)
		die(_("bad pack compression level %d"), cfg->pack_compression_level);

	எனில் (!delta_தேடல்_threads)	/* --threads=0 means autodetect */
		delta_தேடல்_threads = online_cpus();

	எனில் (!HAVE_THREADS && delta_தேடல்_threads != 1)
		warning(_("no threads support, ignoring --threads"));
	எனில் (!pack_to_stdout && !pack_size_limit)
		pack_size_limit = pack_size_limit_cfg;
	எனில் (pack_to_stdout && pack_size_limit)
		die(_("--max-pack-size cannot be used to build a pack for transfer"));
	எனில் (pack_size_limit && pack_size_limit < 1024*1024) {
		warning(_("minimum pack size limit is 1 MiB"));
		pack_size_limit = 1024*1024;
	}

	எனில் (!pack_to_stdout && thin)
		die(_("--thin cannot be used to build an indexable pack"));

	die_for_incompatible_opt2(keep_unreachable, "--keep-unreachable",
				  unpack_unreachable, "--unpack-unreachable");
	எனில் (!rev_list_all || !rev_list_reflog || !rev_list_index)
		unpack_unreachable_expiration = 0;

	die_for_incompatible_opt2(stdin_packs, "--stdin-packs",
				  filter_options.choice, "--filter");


	எனில் (stdin_packs && use_internal_rev_list)
		die(_("cannot use internal rev list with --stdin-packs"));

	எனில் (cruft) {
		எனில் (use_internal_rev_list)
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
	எனில் (!pack_to_stdout)
		use_bitmap_index_default = 0;

	எனில் (use_bitmap_index < 0)
		use_bitmap_index = use_bitmap_index_default;

	/* "hard" reasons not to use bitmaps; these just won't work at all */
	எனில் (!use_internal_rev_list || (!pack_to_stdout && write_bitmap_index) || is_repository_shallow(the_repository))
		use_bitmap_index = 0;

	எனில் (pack_to_stdout || !rev_list_all)
		write_bitmap_index = 0;

	எனில் (name_hash_version < 0)
		name_hash_version = (முழு)git_env_ulong("GIT_TEST_NAME_HASH_VERSION", 1);

	validate_name_hash_version();

	எனில் (use_delta_islands)
		strvec_தள்ளு(&rp, "--topo-order");

	எனில் (progress && all_progress_implied)
		progress = 2;

	add_extra_kept_packs(&keep_pack_list);
	எனில் (ignore_packed_keep_on_disk) {
		கட்டமைப்பு packed_git *p;

		repo_for_each_pack(the_repository, p)
			எனில் (p->pack_local && p->pack_keep)
				நிறுத்து;
		எனில் (!p) /* no keep-able packs found */
			ignore_packed_keep_on_disk = 0;
	}
	எனில் (local) {
		/*
		 * unlike ignore_packed_keep_on_disk above, we do not
		 * want to unset "local" based on looking at packs, as
		 * it also covers non-local objects
		 */
		கட்டமைப்பு packed_git *p;

		repo_for_each_pack(the_repository, p) {
			எனில் (!p->pack_local) {
				have_non_local_packs = 1;
				நிறுத்து;
			}
		}
	}

	trace2_region_enter("pack-objects", "enumerate-objects",
			    the_repository);
	prepare_packing_data(the_repository, &to_pack);

	எனில் (progress && !cruft)
		progress_state = start_progress(the_repository,
						_("Enumerating objects"), 0);
	எனில் (stdin_packs) {
		read_stdin_packs(repo, stdin_packs, rev_list_unpacked);
	} இல்லையெனில் எனில் (cruft) {
		read_cruft_objects();
	} இல்லையெனில் எனில் (!use_internal_rev_list) {
		read_object_list_from_stdin();
	} இல்லையெனில் {
		கட்டமைப்பு rev_info revs;

		repo_init_revisions(the_repository, &revs, வெற்று);
		list_objects_filter_copy(&revs.filter, &filter_options);
		எனில் (exclude_promisor_objects_best_effort) {
			revs.include_check = is_not_in_promisor_pack;
			revs.include_check_obj = is_not_in_promisor_pack_obj;
		}
		get_object_list(&revs, &rp);
		release_revisions(&revs);
	}
	cleanup_preferred_base();
	எனில் (include_tag && nr_result)
		refs_for_each_tag_ref(get_main_ref_store(the_repository),
				      add_ref_tag, வெற்று);
	stop_progress(&progress_state);
	trace2_region_leave("pack-objects", "enumerate-objects",
			    the_repository);

	எனில் (non_empty && !nr_result)
		செல் cleanup;
	எனில் (nr_result) {
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

	எனில் (progress)
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

	திரும்பு 0;
}
