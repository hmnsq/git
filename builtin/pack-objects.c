#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "environment.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "hex.h"
#অন্তর্ভাব "config.h"
#অন্তর্ভাব "attr.h"
#অন্তর্ভাব "object.h"
#অন্তর্ভাব "commit.h"
#অন্তর্ভাব "tag.h"
#অন্তর্ভাব "delta.h"
#অন্তর্ভাব "pack.h"
#অন্তর্ভাব "pack-revindex.h"
#অন্তর্ভাব "csum-file.h"
#অন্তর্ভাব "tree-walk.h"
#অন্তর্ভাব "diff.h"
#অন্তর্ভাব "revision.h"
#অন্তর্ভাব "list-objects.h"
#অন্তর্ভাব "list-objects-filter-options.h"
#অন্তর্ভাব "pack-objects.h"
#অন্তর্ভাব "progress.h"
#অন্তর্ভাব "refs.h"
#অন্তর্ভাব "thread-utils.h"
#অন্তর্ভাব "pack-bitmap.h"
#অন্তর্ভাব "delta-islands.h"
#অন্তর্ভাব "reachable.h"
#অন্তর্ভাব "oid-array.h"
#অন্তর্ভাব "strvec.h"
#অন্তর্ভাব "strmap.h"
#অন্তর্ভাব "list.h"
#অন্তর্ভাব "packfile.h"
#অন্তর্ভাব "object-file.h"
#অন্তর্ভাব "object-file-convert.h"
#অন্তর্ভাব "odb.h"
#অন্তর্ভাব "odb/streaming.h"
#অন্তর্ভাব "replace-object.h"
#অন্তর্ভাব "dir.h"
#অন্তর্ভাব "midx.h"
#অন্তর্ভাব "trace2.h"
#অন্তর্ভাব "shallow.h"
#অন্তর্ভাব "promisor-remote.h"
#অন্তর্ভাব "pack-mtimes.h"
#অন্তর্ভাব "parse-options.h"
#অন্তর্ভাব "pkt-line.h"
#অন্তর্ভাব "blob.h"
#অন্তর্ভাব "tree.h"
#অন্তর্ভাব "path-walk.h"

/*
 * Objects we are going to pack are collected in the `to_pack` structure.
 * It contains an array (dynamically expanded) of the object data, and a map
 * that can resolve SHA1s to their position in the array.
 */
স্থির সংরচনা packing_data to_pack;

স্থির অন্তঃস্থ সংরচনা object_entry *oe_delta(
		ধ্রুব সংরচনা packing_data *pack,
		ধ্রুব সংরচনা object_entry *e)
{
	যদি (!e->delta_idx)
		প্রত্যাগম রিক্ত;
	যদি (e->ext_base)
		প্রত্যাগম &pack->ext_bases[e->delta_idx - 1];
	অন্যথা
		প্রত্যাগম &pack->objects[e->delta_idx - 1];
}

স্থির অন্তঃস্থ size_t oe_delta_size(সংরচনা packing_data *pack,
				   ধ্রুব সংরচনা object_entry *e)
{
	যদি (e->delta_size_valid)
		প্রত্যাগম e->delta_size_;

	/*
	 * pack->delta_size[] can't be NULL because oe_set_delta_size()
	 * must have been called when a new delta is saved with
	 * oe_set_delta().
	 * If oe_delta() returns NULL (i.e. default state, which means
	 * delta_size_valid is also false), then the caller must never
	 * call oe_delta_size().
	 */
	প্রত্যাগম pack->delta_size[e - pack->objects];
}

size_t oe_get_size_slow(সংরচনা packing_data *pack,
			ধ্রুব সংরচনা object_entry *e);

স্থির অন্তঃস্থ size_t oe_size(সংরচনা packing_data *pack,
			     ধ্রুব সংরচনা object_entry *e)
{
	যদি (e->size_valid)
		প্রত্যাগম e->size_;

	প্রত্যাগম oe_get_size_slow(pack, e);
}

স্থির অন্তঃস্থ শূন্য oe_set_delta(সংরচনা packing_data *pack,
				সংরচনা object_entry *e,
				সংরচনা object_entry *delta)
{
	যদি (delta)
		e->delta_idx = (delta - pack->objects) + 1;
	অন্যথা
		e->delta_idx = 0;
}

স্থির অন্তঃস্থ সংরচনা object_entry *oe_delta_sibling(
		ধ্রুব সংরচনা packing_data *pack,
		ধ্রুব সংরচনা object_entry *e)
{
	যদি (e->delta_sibling_idx)
		প্রত্যাগম &pack->objects[e->delta_sibling_idx - 1];
	প্রত্যাগম রিক্ত;
}

স্থির অন্তঃস্থ সংরচনা object_entry *oe_delta_child(
		ধ্রুব সংরচনা packing_data *pack,
		ধ্রুব সংরচনা object_entry *e)
{
	যদি (e->delta_child_idx)
		প্রত্যাগম &pack->objects[e->delta_child_idx - 1];
	প্রত্যাগম রিক্ত;
}

স্থির অন্তঃস্থ শূন্য oe_set_delta_child(সংরচনা packing_data *pack,
				      সংরচনা object_entry *e,
				      সংরচনা object_entry *delta)
{
	যদি (delta)
		e->delta_child_idx = (delta - pack->objects) + 1;
	অন্যথা
		e->delta_child_idx = 0;
}

স্থির অন্তঃস্থ শূন্য oe_set_delta_sibling(সংরচনা packing_data *pack,
					সংরচনা object_entry *e,
					সংরচনা object_entry *delta)
{
	যদি (delta)
		e->delta_sibling_idx = (delta - pack->objects) + 1;
	অন্যথা
		e->delta_sibling_idx = 0;
}

স্থির অন্তঃস্থ শূন্য oe_set_size(সংরচনা packing_data *pack,
			       সংরচনা object_entry *e,
			       size_t size)
{
	যদি (size < pack->oe_size_limit) {
		e->size_ = size;
		e->size_valid = 1;
	} অন্যথা {
		e->size_valid = 0;
		যদি (oe_get_size_slow(pack, e) != size)
			BUG("'size' is supposed to be the object size!");
	}
}

স্থির অন্তঃস্থ শূন্য oe_set_delta_size(সংরচনা packing_data *pack,
				     সংরচনা object_entry *e,
				     size_t size)
{
	যদি (size < pack->oe_delta_size_limit) {
		e->delta_size_ = size;
		e->delta_size_valid = 1;
	} অন্যথা {
		packing_data_lock(pack);
		যদি (!pack->delta_size)
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

স্থির ধ্রুব বর্ণ *ধ্রুব pack_usage[] = {
	N_("git pack-objects [-q | --progress | --all-progress] [--all-progress-implied]\n"
	   "                 [--no-reuse-delta] [--delta-base-offset] [--non-empty]\n"
	   "                 [--local] [--incremental] [--window=<n>] [--depth=<n>]\n"
	   "                 [--revs [--unpacked | --all]] [--keep-pack=<pack-name>]\n"
	   "                 [--cruft] [--cruft-expiration=<time>]\n"
	   "                 [--stdout [--filter=<filter-spec>] | <base-name>]\n"
	   "                 [--shallow] [--keep-true-parents] [--[no-]sparse]\n"
	   "                 [--name-hash-version=<n>] [--path-walk] < <object-list>"),
	রিক্ত
};

স্থির সংরচনা pack_idx_entry **written_list;
স্থির uint32_t nr_result, nr_written, nr_seen;
স্থির সংরচনা bitmap_index *bitmap_git;
স্থির uint32_t write_layer;

স্থির পূর্ণাঙ্ক non_empty;
স্থির পূর্ণাঙ্ক reuse_delta = 1, reuse_object = 1;
স্থির পূর্ণাঙ্ক keep_unreachable, unpack_unreachable, include_tag;
স্থির timestamp_t unpack_unreachable_expiration;
স্থির পূর্ণাঙ্ক pack_loose_unreachable;
স্থির পূর্ণাঙ্ক cruft;
স্থির পূর্ণাঙ্ক shallow = 0;
স্থির timestamp_t cruft_expiration;
স্থির পূর্ণাঙ্ক local;
স্থির পূর্ণাঙ্ক have_non_local_packs;
স্থির পূর্ণাঙ্ক incremental;
স্থির পূর্ণাঙ্ক ignore_packed_keep_on_disk;
স্থির পূর্ণাঙ্ক ignore_packed_keep_in_core;
স্থির পূর্ণাঙ্ক ignore_packed_keep_in_core_open;
স্থির পূর্ণাঙ্ক ignore_packed_keep_in_core_has_cruft;
স্থির পূর্ণাঙ্ক allow_ofs_delta;
স্থির সংরচনা pack_idx_option pack_idx_opts;
স্থির ধ্রুব বর্ণ *base_name;
স্থির পূর্ণাঙ্ক progress = 1;
স্থির পূর্ণাঙ্ক window = 10;
স্থির unsigned long pack_size_limit;
স্থির পূর্ণাঙ্ক depth = 50;
স্থির পূর্ণাঙ্ক delta_অনুসন্ধান_threads;
স্থির পূর্ণাঙ্ক pack_to_stdout;
স্থির পূর্ণাঙ্ক sparse;
স্থির পূর্ণাঙ্ক thin;
স্থির পূর্ণাঙ্ক path_walk = -1;
স্থির পূর্ণাঙ্ক অঙ্ক_preferred_base;
স্থির সংরচনা progress *progress_state;

স্থির সংরচনা bitmapped_pack *reuse_packfiles;
স্থির size_t reuse_packfiles_nr;
স্থির size_t reuse_packfiles_used_nr;
স্থির uint32_t reuse_packfile_objects;
স্থির সংরচনা bitmap *reuse_packfile_bitmap;

স্থির পূর্ণাঙ্ক use_bitmap_index_default = 1;
স্থির পূর্ণাঙ্ক use_bitmap_index = -1;
স্থির গণনা {
	NO_PACK_REUSE = 0,
	SINGLE_PACK_REUSE,
	MULTI_PACK_REUSE,
} allow_pack_reuse = SINGLE_PACK_REUSE;
স্থির গণনা {
	WRITE_BITMAP_FALSE = 0,
	WRITE_BITMAP_QUIET,
	WRITE_BITMAP_TRUE,
} write_bitmap_index;
স্থির uint16_t write_bitmap_options = BITMAP_OPT_HASH_CACHE;

স্থির পূর্ণাঙ্ক exclude_promisor_objects;
স্থির পূর্ণাঙ্ক exclude_promisor_objects_best_effort;

স্থির পূর্ণাঙ্ক use_delta_islands;

স্থির size_t delta_cache_size = 0;
স্থির size_t max_delta_cache_size = DEFAULT_DELTA_CACHE_SIZE;
স্থির unsigned long cache_max_small_delta_size = 1000;

স্থির unsigned long window_memory_limit = 0;

স্থির সংরচনা string_list uri_protocols = STRING_LIST_INIT_NODUP;

গণনা missing_action {
	MA_ERROR = 0,      /* fail if any missing objects are encountered */
	MA_ALLOW_ANY,      /* silently allow ALL missing objects */
	MA_ALLOW_PROMISOR, /* silently allow all missing PROMISOR objects */
};
স্থির গণনা missing_action arg_missing_action;
স্থির show_object_fn fn_show_object;

সংরচনা configured_exclusion {
	সংরচনা oidmap_entry e;
	বর্ণ *pack_hash_hex;
	বর্ণ *uri;
};
স্থির সংরচনা oidmap configured_exclusions;

স্থির সংরচনা oidset excluded_by_config;
স্থির পূর্ণাঙ্ক name_hash_version = -1;

গণনা stdin_packs_mode {
	STDIN_PACKS_MODE_NONE,
	STDIN_PACKS_MODE_STANDARD,
	STDIN_PACKS_MODE_FOLLOW,
};

/**
 * Check whether the name_hash_version chosen by user input is appropriate,
 * and also validate whether it is compatible with other features.
 */
স্থির শূন্য validate_name_hash_version(শূন্য)
{
	যদি (name_hash_version < 1 || name_hash_version > 2)
		die(_("invalid --name-hash-version option: %d"), name_hash_version);
	যদি (write_bitmap_index && name_hash_version != 1) {
		warning(_("currently, --write-bitmap-index requires --name-hash-version=1"));
		name_hash_version = 1;
	}
}

স্থির অন্তঃস্থ uint32_t pack_name_hash_fn(ধ্রুব বর্ণ *name)
{
	স্থির পূর্ণাঙ্ক seen_version = -1;

	যদি (seen_version < 0)
		seen_version = name_hash_version;
	অন্যথা যদি (seen_version != name_hash_version)
		BUG("name hash version changed from %d to %d mid-process",
		    seen_version, name_hash_version);

	বিকল্প (name_hash_version) {
	পক্ষ 1:
		প্রত্যাগম pack_name_hash(name);

	পক্ষ 2:
		প্রত্যাগম pack_name_hash_v2((ধ্রুব unsigned বর্ণ *)name);

	স্বভাব:
		BUG("invalid name-hash version: %d", name_hash_version);
	}
}

/*
 * stats
 */
স্থির uint32_t written, written_delta;
স্থির uint32_t reused, reused_delta;

/*
 * Indexed commits
 */
স্থির সংরচনা commit **indexed_commits;
স্থির unsigned পূর্ণাঙ্ক indexed_commits_nr;
স্থির unsigned পূর্ণাঙ্ক indexed_commits_alloc;

স্থির শূন্য index_commit_for_bitmap(সংরচনা commit *commit)
{
	যদি (indexed_commits_nr >= indexed_commits_alloc) {
		indexed_commits_alloc = (indexed_commits_alloc + 32) * 2;
		REALLOC_ARRAY(indexed_commits, indexed_commits_alloc);
	}

	indexed_commits[indexed_commits_nr++] = commit;
}

স্থির শূন্য *get_delta(সংরচনা object_entry *entry)
{
	unsigned long size, base_size;
	size_t delta_size;
	শূন্য *buf, *base_buf, *delta_buf;
	গণনা object_type type;
	size_t size_st = 0, base_size_st = 0;

	buf = odb_read_object(the_repository->objects, &entry->idx.oid,
			      &type, &size_st);
	size = cast_size_t_to_ulong(size_st);
	যদি (!buf)
		die(_("unable to read %s"), oid_to_hex(&entry->idx.oid));
	base_buf = odb_read_object(the_repository->objects,
				   &DELTA(entry)->idx.oid, &type,
				   &base_size_st);
	base_size = cast_size_t_to_ulong(base_size_st);
	যদি (!base_buf)
		die("unable to read %s",
		    oid_to_hex(&DELTA(entry)->idx.oid));
	delta_buf = diff_delta(base_buf, base_size,
			       buf, size, &delta_size, 0);
	/*
	 * We successfully computed this delta once but dropped it for
	 * memory reasons. Something is very wrong if this time we
	 * recompute and create a different delta.
	 */
	যদি (!delta_buf || delta_size != DELTA_SIZE(entry))
		BUG("delta size changed");
	মুক্ত(buf);
	মুক্ত(base_buf);
	প্রত্যাগম delta_buf;
}

স্থির unsigned long do_compress(শূন্য **pptr, unsigned long size)
{
	git_zstream stream;
	শূন্য *in, *out;
	unsigned long maxsize;
	সংরচনা repo_config_মানসমূহ *cfg = repo_config_মানসমূহ(the_repository);

	git_deflate_init(&stream, cfg->pack_compression_level);
	maxsize = git_deflate_bound(&stream, size);

	in = *pptr;
	out = xmalloc(maxsize);
	*pptr = out;

	stream.next_in = in;
	stream.avail_in = size;
	stream.next_out = out;
	stream.avail_out = maxsize;
	যাবত্ (git_deflate(&stream, Z_FINISH) == Z_OK)
		; /* nothing */
	git_deflate_end(&stream);

	মুক্ত(in);
	প্রত্যাগম stream.total_out;
}

স্থির unsigned long write_large_blob_data(সংরচনা odb_stream *st, সংরচনা hashfile *f,
					   ধ্রুব সংরচনা object_id *oid)
{
	git_zstream stream;
	unsigned বর্ণ ibuf[1024 * 16];
	unsigned বর্ণ obuf[1024 * 16];
	unsigned long olen = 0;
	সংরচনা repo_config_মানসমূহ *cfg = repo_config_মানসমূহ(the_repository);

	git_deflate_init(&stream, cfg->pack_compression_level);

	প্রতি (;;) {
		ssize_t readlen;
		পূর্ণাঙ্ক zret = Z_OK;
		readlen = odb_stream_read(st, ibuf, পরিমাণ(ibuf));
		যদি (readlen == -1)
			die(_("unable to read %s"), oid_to_hex(oid));

		stream.next_in = ibuf;
		stream.avail_in = readlen;
		যাবত্ ((stream.avail_in || readlen == 0) &&
		       (zret == Z_OK || zret == Z_BUF_ERROR)) {
			stream.next_out = obuf;
			stream.avail_out = পরিমাণ(obuf);
			zret = git_deflate(&stream, readlen ? 0 : Z_FINISH);
			hashwrite(f, obuf, stream.next_out - obuf);
			olen += stream.next_out - obuf;
		}
		যদি (stream.avail_in)
			die(_("deflate error (%d)"), zret);
		যদি (readlen == 0) {
			যদি (zret != Z_STREAM_END)
				die(_("deflate error (%d)"), zret);
			ভঙ্গ;
		}
	}
	git_deflate_end(&stream);
	প্রত্যাগম olen;
}

/*
 * we are going to reuse the existing object data as is.  make
 * sure it is not corrupt.
 */
স্থির পূর্ণাঙ্ক check_pack_inflate(সংরচনা packed_git *p,
		সংরচনা pack_window **w_curs,
		off_t offset,
		off_t len,
		size_t expect)
{
	git_zstream stream;
	unsigned বর্ণ fakebuf[4096], *in;
	পূর্ণাঙ্ক st;

	memset(&stream, 0, পরিমাণ(stream));
	git_inflate_init(&stream);
	কুরু {
		in = use_pack(p, w_curs, offset, &stream.avail_in);
		stream.next_in = in;
		stream.next_out = fakebuf;
		stream.avail_out = পরিমাণ(fakebuf);
		st = git_inflate(&stream, Z_FINISH);
		offset += stream.next_in - in;
	} যাবত্ (st == Z_OK || st == Z_BUF_ERROR);
	git_inflate_end(&stream);
	প্রত্যাগম (st == Z_STREAM_END &&
		stream.total_out == expect &&
		stream.total_in == len) ? 0 : -1;
}

স্থির শূন্য copy_pack_data(সংরচনা hashfile *f,
		সংরচনা packed_git *p,
		সংরচনা pack_window **w_curs,
		off_t offset,
		off_t len)
{
	unsigned বর্ণ *in;
	size_t avail;

	যাবত্ (len) {
		in = use_pack(p, w_curs, offset, &avail);
		যদি (avail > len)
			avail = (unsigned long)len;
		hashwrite(f, in, avail);
		offset += avail;
		len -= avail;
	}
}

স্থির অন্তঃস্থ পূর্ণাঙ্ক oe_size_greater_than(সংরচনা packing_data *pack,
				       ধ্রুব সংরচনা object_entry *lhs,
				       size_t rhs)
{
	যদি (lhs->size_valid)
		প্রত্যাগম lhs->size_ > rhs;
	যদি (rhs < pack->oe_size_limit) /* rhs < 2^x <= lhs ? */
		প্রত্যাগম 1;
	প্রত্যাগম oe_get_size_slow(pack, lhs) > rhs;
}

/* Return 0 if we will bust the pack-size limit */
স্থির unsigned long write_no_reuse_object(সংরচনা hashfile *f, সংরচনা object_entry *entry,
					   unsigned long limit, পূর্ণাঙ্ক usable_delta)
{
	unsigned long size, datalen;
	unsigned বর্ণ header[MAX_PACK_OBJECT_HEADER],
		      dheader[MAX_PACK_OBJECT_HEADER];
	unsigned hdrlen;
	গণনা object_type type;
	শূন্য *buf;
	সংরচনা odb_stream *st = রিক্ত;
	ধ্রুব unsigned hashsz = the_hash_algo->rawsz;

	যদি (!usable_delta) {
		যদি (oe_type(entry) == OBJ_BLOB &&
		    oe_size_greater_than(&to_pack, entry,
					 repo_settings_get_big_file_threshold(the_repository)) &&
		    (st = odb_stream_from_object(the_repository->objects, &entry->idx.oid,
						 রিক্ত)) != রিক্ত) {
			buf = রিক্ত;
			type = st->type;
			size = st->size;
		} অন্যথা {
			size_t size_st = 0;
			buf = odb_read_object(the_repository->objects,
					      &entry->idx.oid, &type,
					      &size_st);
			size = cast_size_t_to_ulong(size_st);
			যদি (!buf)
				die(_("unable to read %s"),
				    oid_to_hex(&entry->idx.oid));
		}
		/*
		 * make sure no cached delta data remains from a
		 * previous attempt before a pack split occurred.
		 */
		FREE_AND_NULL(entry->delta_data);
		entry->z_delta_size = 0;
	} অন্যথা যদি (entry->delta_data) {
		size = DELTA_SIZE(entry);
		buf = entry->delta_data;
		entry->delta_data = রিক্ত;
		type = (allow_ofs_delta && DELTA(entry)->idx.offset) ?
			OBJ_OFS_DELTA : OBJ_REF_DELTA;
	} অন্যথা {
		buf = get_delta(entry);
		size = DELTA_SIZE(entry);
		type = (allow_ofs_delta && DELTA(entry)->idx.offset) ?
			OBJ_OFS_DELTA : OBJ_REF_DELTA;
	}

	যদি (st)	/* large blob case, just assume we don't compress well */
		datalen = size;
	অন্যথা যদি (entry->z_delta_size)
		datalen = entry->z_delta_size;
	অন্যথা
		datalen = do_compress(&buf, size);

	/*
	 * The object header is a byte of 'type' followed by zero or
	 * more bytes of length.
	 */
	hdrlen = encode_in_pack_object_header(header, পরিমাণ(header),
					      type, size);

	যদি (type == OBJ_OFS_DELTA) {
		/*
		 * Deltas with relative base contain an additional
		 * encoding of the relative offset for the delta
		 * base from this object's position in the pack.
		 */
		off_t ofs = entry->idx.offset - DELTA(entry)->idx.offset;
		unsigned pos = পরিমাণ(dheader) - 1;
		dheader[pos] = ofs & 127;
		যাবত্ (ofs >>= 7)
			dheader[--pos] = 128 | (--ofs & 127);
		যদি (limit && hdrlen + পরিমাণ(dheader) - pos + datalen + hashsz >= limit) {
			যদি (st)
				odb_stream_close(st);
			মুক্ত(buf);
			প্রত্যাগম 0;
		}
		hashwrite(f, header, hdrlen);
		hashwrite(f, dheader + pos, পরিমাণ(dheader) - pos);
		hdrlen += পরিমাণ(dheader) - pos;
	} অন্যথা যদি (type == OBJ_REF_DELTA) {
		/*
		 * Deltas with a base reference contain
		 * additional bytes for the base object ID.
		 */
		যদি (limit && hdrlen + hashsz + datalen + hashsz >= limit) {
			যদি (st)
				odb_stream_close(st);
			মুক্ত(buf);
			প্রত্যাগম 0;
		}
		hashwrite(f, header, hdrlen);
		hashwrite(f, DELTA(entry)->idx.oid.hash, hashsz);
		hdrlen += hashsz;
	} অন্যথা {
		যদি (limit && hdrlen + datalen + hashsz >= limit) {
			যদি (st)
				odb_stream_close(st);
			মুক্ত(buf);
			প্রত্যাগম 0;
		}
		hashwrite(f, header, hdrlen);
	}
	যদি (st) {
		datalen = write_large_blob_data(st, f, &entry->idx.oid);
		odb_stream_close(st);
	} অন্যথা {
		hashwrite(f, buf, datalen);
		মুক্ত(buf);
	}

	প্রত্যাগম hdrlen + datalen;
}

/* Return 0 if we will bust the pack-size limit */
স্থির off_t write_reuse_object(সংরচনা hashfile *f, সংরচনা object_entry *entry,
				unsigned long limit, পূর্ণাঙ্ক usable_delta)
{
	সংরচনা packed_git *p = IN_PACK(entry);
	সংরচনা pack_window *w_curs = রিক্ত;
	uint32_t pos;
	off_t offset, cur;
	গণনা object_type type = oe_type(entry);
	গণনা object_type in_pack_type;
	off_t datalen;
	unsigned বর্ণ header[MAX_PACK_OBJECT_HEADER],
		      dheader[MAX_PACK_OBJECT_HEADER];
	unsigned hdrlen;
	ধ্রুব unsigned hashsz = the_hash_algo->rawsz;
	size_t entry_size;

	cur = entry->in_pack_offset;
	in_pack_type = unpack_object_header(p, &w_curs, &cur, &entry_size);
	যদি (in_pack_type < 0)
		die(_("write_reuse_object: unable to parse object header of %s"),
		    oid_to_hex(&entry->idx.oid));

	যদি (DELTA(entry))
		type = (allow_ofs_delta && DELTA(entry)->idx.offset) ?
			OBJ_OFS_DELTA : OBJ_REF_DELTA;
	hdrlen = encode_in_pack_object_header(header, পরিমাণ(header),
					      type, entry_size);

	offset = entry->in_pack_offset;
	যদি (offset_to_pack_pos(p, offset, &pos) < 0)
		die(_("write_reuse_object: could not locate %s, expected at "
		      "offset %"PRIuMAX" in pack %s"),
		    oid_to_hex(&entry->idx.oid), (uintmax_t)offset,
		    p->pack_name);
	datalen = pack_pos_to_offset(p, pos + 1) - offset;
	যদি (!pack_to_stdout && p->index_version > 1 &&
	    check_pack_crc(p, &w_curs, offset, datalen,
			   pack_pos_to_index(p, pos))) {
		error(_("bad packed object CRC for %s"),
		      oid_to_hex(&entry->idx.oid));
		unuse_pack(&w_curs);
		প্রত্যাগম write_no_reuse_object(f, entry, limit, usable_delta);
	}

	offset += entry->in_pack_header_size;
	datalen -= entry->in_pack_header_size;

	যদি (!pack_to_stdout && p->index_version == 1 &&
	    check_pack_inflate(p, &w_curs, offset, datalen, entry_size)) {
		error(_("corrupt packed object for %s"),
		      oid_to_hex(&entry->idx.oid));
		unuse_pack(&w_curs);
		প্রত্যাগম write_no_reuse_object(f, entry, limit, usable_delta);
	}

	যদি (type == OBJ_OFS_DELTA) {
		off_t ofs = entry->idx.offset - DELTA(entry)->idx.offset;
		unsigned pos = পরিমাণ(dheader) - 1;
		dheader[pos] = ofs & 127;
		যাবত্ (ofs >>= 7)
			dheader[--pos] = 128 | (--ofs & 127);
		যদি (limit && hdrlen + পরিমাণ(dheader) - pos + datalen + hashsz >= limit) {
			unuse_pack(&w_curs);
			প্রত্যাগম 0;
		}
		hashwrite(f, header, hdrlen);
		hashwrite(f, dheader + pos, পরিমাণ(dheader) - pos);
		hdrlen += পরিমাণ(dheader) - pos;
		reused_delta++;
	} অন্যথা যদি (type == OBJ_REF_DELTA) {
		যদি (limit && hdrlen + hashsz + datalen + hashsz >= limit) {
			unuse_pack(&w_curs);
			প্রত্যাগম 0;
		}
		hashwrite(f, header, hdrlen);
		hashwrite(f, DELTA(entry)->idx.oid.hash, hashsz);
		hdrlen += hashsz;
		reused_delta++;
	} অন্যথা {
		যদি (limit && hdrlen + datalen + hashsz >= limit) {
			unuse_pack(&w_curs);
			প্রত্যাগম 0;
		}
		hashwrite(f, header, hdrlen);
	}
	copy_pack_data(f, p, &w_curs, offset, datalen);
	unuse_pack(&w_curs);
	reused++;
	প্রত্যাগম hdrlen + datalen;
}

/* Return 0 if we will bust the pack-size limit */
স্থির off_t write_object(সংরচনা hashfile *f,
			  সংরচনা object_entry *entry,
			  off_t write_offset)
{
	unsigned long limit;
	off_t len;
	পূর্ণাঙ্ক usable_delta, to_reuse;

	যদি (!pack_to_stdout)
		crc32_begin(f);

	/* apply size limit if limited packsize and not first object */
	যদি (!pack_size_limit || !nr_written)
		limit = 0;
	অন্যথা যদি (pack_size_limit <= write_offset)
		/*
		 * the earlier object did not fit the limit; avoid
		 * mistaking this with unlimited (i.e. limit = 0).
		 */
		limit = 1;
	অন্যথা
		limit = pack_size_limit - write_offset;

	যদি (!DELTA(entry))
		usable_delta = 0;	/* no delta */
	অন্যথা যদি (!pack_size_limit)
	       usable_delta = 1;	/* unlimited packfile */
	অন্যথা যদি (DELTA(entry)->idx.offset == (off_t)-1)
		usable_delta = 0;	/* base was written to another pack */
	অন্যথা যদি (DELTA(entry)->idx.offset)
		usable_delta = 1;	/* base already exists in this pack */
	অন্যথা
		usable_delta = 0;	/* base could end up in another pack */

	যদি (!reuse_object)
		to_reuse = 0;	/* explicit */
	অন্যথা যদি (!IN_PACK(entry))
		to_reuse = 0;	/* can't reuse what we don't have */
	অন্যথা যদি (oe_type(entry) == OBJ_REF_DELTA ||
		 oe_type(entry) == OBJ_OFS_DELTA)
				/* check_object() decided it for us ... */
		to_reuse = usable_delta;
				/* ... but pack split may override that */
	অন্যথা যদি (oe_type(entry) != entry->in_pack_type)
		to_reuse = 0;	/* pack has delta which is unusable */
	অন্যথা যদি (DELTA(entry))
		to_reuse = 0;	/* we want to pack afresh */
	অন্যথা
		to_reuse = 1;	/* we have it in-pack undeltified,
				 * and we do not need to deltify it.
				 */

	যদি (!to_reuse)
		len = write_no_reuse_object(f, entry, limit, usable_delta);
	অন্যথা
		len = write_reuse_object(f, entry, limit, usable_delta);
	যদি (!len)
		প্রত্যাগম 0;

	যদি (usable_delta)
		written_delta++;
	written++;
	যদি (!pack_to_stdout)
		entry->idx.crc32 = crc32_end(f);
	প্রত্যাগম len;
}

গণনা write_one_status {
	WRITE_ONE_SKIP = -1, /* already written */
	WRITE_ONE_BREAK = 0, /* writing this will bust the limit; not written */
	WRITE_ONE_WRITTEN = 1, /* normal */
	WRITE_ONE_RECURSIVE = 2 /* already scheduled to be written */
};

স্থির গণনা write_one_status write_one(সংরচনা hashfile *f,
				       সংরচনা object_entry *e,
				       off_t *offset)
{
	off_t size;
	পূর্ণাঙ্ক recursing;

	/*
	 * we set offset to 1 (which is an impossible value) to mark
	 * the fact that this object is involved in "write its base
	 * first before writing a deltified object" recursion.
	 */
	recursing = (e->idx.offset == 1);
	যদি (recursing) {
		warning(_("recursive delta detected for object %s"),
			oid_to_hex(&e->idx.oid));
		প্রত্যাগম WRITE_ONE_RECURSIVE;
	} অন্যথা যদি (e->idx.offset || e->preferred_base) {
		/* offset is non zero if object is written already. */
		প্রত্যাগম WRITE_ONE_SKIP;
	}

	/* if we are deltified, write out base object first. */
	যদি (DELTA(e)) {
		e->idx.offset = 1; /* now recurse */
		বিকল্প (write_one(f, DELTA(e), offset)) {
		পক্ষ WRITE_ONE_RECURSIVE:
			/* we cannot depend on this one */
			SET_DELTA(e, রিক্ত);
			ভঙ্গ;
		স্বভাব:
			ভঙ্গ;
		পক্ষ WRITE_ONE_BREAK:
			e->idx.offset = recursing;
			প্রত্যাগম WRITE_ONE_BREAK;
		}
	}

	e->idx.offset = *offset;
	size = write_object(f, e, *offset);
	যদি (!size) {
		e->idx.offset = recursing;
		প্রত্যাগম WRITE_ONE_BREAK;
	}
	written_list[nr_written++] = &e->idx;

	/* make sure off_t is sufficiently large not to wrap */
	যদি (signed_add_overflows(*offset, size))
		die(_("pack too large for current definition of off_t"));
	*offset += size;
	প্রত্যাগম WRITE_ONE_WRITTEN;
}

স্থির পূর্ণাঙ্ক mark_tagged(ধ্রুব সংরচনা reference *ref, শূন্য *cb_data UNUSED)
{
	সংরচনা object_id peeled;
	সংরচনা object_entry *entry = packlist_find(&to_pack, ref->oid);

	যদি (entry)
		entry->tagged = 1;
	যদি (!reference_get_peeled_oid(the_repository, ref, &peeled)) {
		entry = packlist_find(&to_pack, &peeled);
		যদি (entry)
			entry->tagged = 1;
	}
	প্রত্যাগম 0;
}

স্থির অন্তঃস্থ unsigned বর্ণ oe_layer(সংরচনা packing_data *pack,
				     সংরচনা object_entry *e)
{
	যদি (!pack->layer)
		প্রত্যাগম 0;
	প্রত্যাগম pack->layer[e - pack->objects];
}

স্থির অন্তঃস্থ শূন্য add_to_write_order(সংরচনা object_entry **wo,
			       unsigned পূর্ণাঙ্ক *endp,
			       সংরচনা object_entry *e)
{
	যদি (e->filled || oe_layer(&to_pack, e) != write_layer)
		প্রত্যাগম;
	wo[(*endp)++] = e;
	e->filled = 1;
}

স্থির শূন্য add_descendants_to_write_order(সংরচনা object_entry **wo,
					   unsigned পূর্ণাঙ্ক *endp,
					   সংরচনা object_entry *e)
{
	পূর্ণাঙ্ক add_to_order = 1;
	যাবত্ (e) {
		যদি (add_to_order) {
			সংরচনা object_entry *s;
			/* add this node... */
			add_to_write_order(wo, endp, e);
			/* all its siblings... */
			প্রতি (s = DELTA_SIBLING(e); s; s = DELTA_SIBLING(s)) {
				add_to_write_order(wo, endp, s);
			}
		}
		/* drop down a level to add left subtree nodes if possible */
		যদি (DELTA_CHILD(e)) {
			add_to_order = 1;
			e = DELTA_CHILD(e);
		} অন্যথা {
			add_to_order = 0;
			/* our sibling might have some children, it is next */
			যদি (DELTA_SIBLING(e)) {
				e = DELTA_SIBLING(e);
				অনুবর্ত;
			}
			/* go back to our parent node */
			e = DELTA(e);
			যাবত্ (e && !DELTA_SIBLING(e)) {
				/* we're on the right side of a subtree, keep
				 * going up until we can go right again */
				e = DELTA(e);
			}
			যদি (!e) {
				/* done- we hit our original root node */
				প্রত্যাগম;
			}
			/* pass it off to sibling at this level */
			e = DELTA_SIBLING(e);
		}
	};
}

স্থির শূন্য add_family_to_write_order(সংরচনা object_entry **wo,
				      unsigned পূর্ণাঙ্ক *endp,
				      সংরচনা object_entry *e)
{
	সংরচনা object_entry *মূল;

	প্রতি (মূল = e; DELTA(মূল); মূল = DELTA(মূল))
		; /* nothing */
	add_descendants_to_write_order(wo, endp, মূল);
}

স্থির শূন্য compute_layer_order(সংরচনা object_entry **wo, unsigned পূর্ণাঙ্ক *wo_end)
{
	unsigned পূর্ণাঙ্ক i, last_untagged;
	সংরচনা object_entry *objects = to_pack.objects;

	প্রতি (i = 0; i < to_pack.nr_objects; i++) {
		যদি (objects[i].tagged)
			ভঙ্গ;
		add_to_write_order(wo, wo_end, &objects[i]);
	}
	last_untagged = i;

	/*
	 * Then fill all the tagged tips.
	 */
	প্রতি (; i < to_pack.nr_objects; i++) {
		যদি (objects[i].tagged)
			add_to_write_order(wo, wo_end, &objects[i]);
	}

	/*
	 * And then all remaining commits and tags.
	 */
	প্রতি (i = last_untagged; i < to_pack.nr_objects; i++) {
		যদি (oe_type(&objects[i]) != OBJ_COMMIT &&
		    oe_type(&objects[i]) != OBJ_TAG)
			অনুবর্ত;
		add_to_write_order(wo, wo_end, &objects[i]);
	}

	/*
	 * And then all the trees.
	 */
	প্রতি (i = last_untagged; i < to_pack.nr_objects; i++) {
		যদি (oe_type(&objects[i]) != OBJ_বৃক্ষ)
			অনুবর্ত;
		add_to_write_order(wo, wo_end, &objects[i]);
	}

	/*
	 * Finally all the rest in really tight order
	 */
	প্রতি (i = last_untagged; i < to_pack.nr_objects; i++) {
		যদি (!objects[i].filled && oe_layer(&to_pack, &objects[i]) == write_layer)
			add_family_to_write_order(wo, wo_end, &objects[i]);
	}
}

স্থির সংরচনা object_entry **compute_write_order(শূন্য)
{
	uint32_t max_layers = 1;
	unsigned পূর্ণাঙ্ক i, wo_end;

	সংরচনা object_entry **wo;
	সংরচনা object_entry *objects = to_pack.objects;

	প্রতি (i = 0; i < to_pack.nr_objects; i++) {
		objects[i].tagged = 0;
		objects[i].filled = 0;
		SET_DELTA_CHILD(&objects[i], রিক্ত);
		SET_DELTA_SIBLING(&objects[i], রিক্ত);
	}

	/*
	 * Fully connect delta_child/delta_sibling network.
	 * Make sure delta_sibling is sorted in the original
	 * recency order.
	 */
	প্রতি (i = to_pack.nr_objects; i > 0;) {
		সংরচনা object_entry *e = &objects[--i];
		যদি (!DELTA(e))
			অনুবর্ত;
		/* Mark me as the first child */
		e->delta_sibling_idx = DELTA(e)->delta_child_idx;
		SET_DELTA_CHILD(DELTA(e), e);
	}

	/*
	 * Mark objects that are at the tip of tags.
	 */
	refs_for_each_tag_ref(get_main_ref_store(the_repository), mark_tagged,
			      রিক্ত);

	যদি (use_delta_islands) {
		max_layers = compute_pack_layers(&to_pack);
		free_island_marks();
	}

	ALLOC_ARRAY(wo, to_pack.nr_objects);
	wo_end = 0;

	প্রতি (; write_layer < max_layers; ++write_layer)
		compute_layer_order(wo, &wo_end);

	যদি (wo_end != to_pack.nr_objects)
		die(_("ordered %u objects, expected %"PRIu32),
		    wo_end, to_pack.nr_objects);

	প্রত্যাগম wo;
}


/*
 * A reused set of objects. All objects in a chunk have the same
 * relative position in the original packfile and the generated
 * packfile.
 */

স্থির সংরচনা reused_chunk {
	/* The offset of the first object of this chunk in the original
	 * packfile. */
	off_t original;
	/* The difference for "original" minus the offset of the first object of
	 * this chunk in the generated packfile. */
	off_t difference;
} *reused_chunks;
স্থির পূর্ণাঙ্ক reused_chunks_nr;
স্থির পূর্ণাঙ্ক reused_chunks_alloc;

স্থির শূন্য record_reused_object(off_t where, off_t offset)
{
	যদি (reused_chunks_nr && reused_chunks[reused_chunks_nr-1].difference == offset)
		প্রত্যাগম;

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
স্থির off_t find_reused_offset(off_t where)
{
	পূর্ণাঙ্ক lo = 0, hi = reused_chunks_nr;
	যাবত্ (lo < hi) {
		পূর্ণাঙ্ক mi = lo + ((hi - lo) / 2);
		যদি (where == reused_chunks[mi].original)
			প্রত্যাগম reused_chunks[mi].difference;
		যদি (where < reused_chunks[mi].original)
			hi = mi;
		অন্যথা
			lo = mi + 1;
	}

	/*
	 * The first chunk starts at zero, so we can't have gone below
	 * there.
	 */
	প্রতিজ্ঞা(lo);
	প্রত্যাগম reused_chunks[lo-1].difference;
}

স্থির শূন্য write_reused_pack_one(সংরচনা packed_git *reuse_packfile,
				  size_t pos, সংরচনা hashfile *out,
				  off_t pack_start,
				  সংরচনা pack_window **w_curs)
{
	off_t offset, next, cur;
	গণনা object_type type;
	size_t size;

	offset = pack_pos_to_offset(reuse_packfile, pos);
	next = pack_pos_to_offset(reuse_packfile, pos + 1);

	record_reused_object(offset,
			     offset - (hashfile_total(out) - pack_start));

	cur = offset;
	type = unpack_object_header(reuse_packfile, w_curs, &cur, &size);
	প্রতিজ্ঞা(type >= 0);

	যদি (type == OBJ_OFS_DELTA) {
		off_t base_offset;
		off_t fixup;

		unsigned বর্ণ header[MAX_PACK_OBJECT_HEADER];
		unsigned len;

		base_offset = get_delta_base(reuse_packfile, w_curs, &cur, type, offset);
		প্রতিজ্ঞা(base_offset != 0);

		/* Convert to REF_DELTA if we must... */
		যদি (!allow_ofs_delta) {
			uint32_t base_pos;
			সংরচনা object_id base_oid;

			যদি (offset_to_pack_pos(reuse_packfile, base_offset, &base_pos) < 0)
				die(_("expected object at offset %"PRIuMAX" "
				      "in pack %s"),
				    (uintmax_t)base_offset,
				    reuse_packfile->pack_name);

			nth_packed_object_id(&base_oid, reuse_packfile,
					     pack_pos_to_index(reuse_packfile, base_pos));

			len = encode_in_pack_object_header(header, পরিমাণ(header),
							   OBJ_REF_DELTA, size);
			hashwrite(out, header, len);
			hashwrite(out, base_oid.hash, the_hash_algo->rawsz);
			copy_pack_data(out, reuse_packfile, w_curs, cur, next - cur);
			প্রত্যাগম;
		}

		/* Otherwise see if we need to rewrite the offset... */
		fixup = find_reused_offset(offset) -
			find_reused_offset(base_offset);
		যদি (fixup) {
			unsigned বর্ণ ofs_header[MAX_PACK_OBJECT_HEADER];
			unsigned i, ofs_len;
			off_t ofs = offset - base_offset - fixup;

			len = encode_in_pack_object_header(header, পরিমাণ(header),
							   OBJ_OFS_DELTA, size);

			i = পরিমাণ(ofs_header) - 1;
			ofs_header[i] = ofs & 127;
			যাবত্ (ofs >>= 7)
				ofs_header[--i] = 128 | (--ofs & 127);

			ofs_len = পরিমাণ(ofs_header) - i;

			hashwrite(out, header, len);
			hashwrite(out, ofs_header + পরিমাণ(ofs_header) - ofs_len, ofs_len);
			copy_pack_data(out, reuse_packfile, w_curs, cur, next - cur);
			প্রত্যাগম;
		}

		/* ...otherwise we have no fixup, and can write it verbatim */
	}

	copy_pack_data(out, reuse_packfile, w_curs, offset, next - offset);
}

স্থির size_t write_reused_pack_verbatim(সংরচনা bitmapped_pack *reuse_packfile,
					 সংরচনা hashfile *out,
					 সংরচনা pack_window **w_curs)
{
	size_t pos = 0;
	size_t end;

	যদি (reuse_packfile->bitmap_pos) {
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
		প্রত্যাগম reuse_packfile->bitmap_pos / BITS_IN_EWORD;
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
	যদি (reuse_packfile_bitmap->word_alloc < end)
		BUG("fewer words than expected in reuse_packfile_bitmap");

	যাবত্ (pos < end && reuse_packfile_bitmap->শব্দগুলি[pos] == (eword_t)~0)
		pos++;

	যদি (pos) {
		off_t to_write;

		written = (pos * BITS_IN_EWORD);
		to_write = pack_pos_to_offset(reuse_packfile->p, written)
			- পরিমাণ(সংরচনা pack_header);

		/* We're recording one chunk, not one object. */
		record_reused_object(পরিমাণ(সংরচনা pack_header), 0);
		hashflush(out);
		copy_pack_data(out, reuse_packfile->p, w_curs,
			পরিমাণ(সংরচনা pack_header), to_write);

		display_progress(progress_state, written);
	}
	প্রত্যাগম pos;
}

স্থির শূন্য write_reused_pack(সংরচনা bitmapped_pack *reuse_packfile,
			      সংরচনা hashfile *f)
{
	size_t i = reuse_packfile->bitmap_pos / BITS_IN_EWORD;
	uint32_t offset;
	off_t pack_start = hashfile_total(f) - পরিমাণ(সংরচনা pack_header);
	সংরচনা pack_window *w_curs = রিক্ত;

	যদি (allow_ofs_delta)
		i = write_reused_pack_verbatim(reuse_packfile, f, &w_curs);

	প্রতি (; i < reuse_packfile_bitmap->word_alloc; ++i) {
		eword_t word = reuse_packfile_bitmap->শব্দগুলি[i];
		size_t pos = (i * BITS_IN_EWORD);

		প্রতি (offset = 0; offset < BITS_IN_EWORD; ++offset) {
			uint32_t pack_pos;
			যদি ((word >> offset) == 0)
				ভঙ্গ;

			offset += ewah_bit_ctz64(word >> offset);
			যদি (pos + offset < reuse_packfile->bitmap_pos)
				অনুবর্ত;
			যদি (pos + offset >= reuse_packfile->bitmap_pos + reuse_packfile->bitmap_nr)
				গচ্ছ done;

			যদি (reuse_packfile->bitmap_pos) {
				/*
				 * When doing multi-pack reuse on a
				 * non-preferred pack, translate bit positions
				 * from the MIDX pseudo-pack order back to their
				 * pack-relative positions before attempting
				 * reuse.
				 */
				সংরচনা multi_pack_index *m = reuse_packfile->from_midx;
				uint32_t midx_pos;
				off_t pack_ofs;

				যদি (!m)
					BUG("non-zero bitmap position without MIDX");

				midx_pos = pack_pos_to_midx(m, pos + offset);
				pack_ofs = nth_midxed_offset(m, midx_pos);

				যদি (offset_to_pack_pos(reuse_packfile->p,
						       pack_ofs, &pack_pos) < 0)
					BUG("could not find expected object at offset %"PRIuMAX" in pack %s",
					    (uintmax_t)pack_ofs,
					    pack_basename(reuse_packfile->p));
			} অন্যথা {
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

স্থির শূন্য write_excluded_by_configs(শূন্য)
{
	সংরচনা oidset_iter iter;
	ধ্রুব সংরচনা object_id *oid;

	oidset_iter_init(&excluded_by_config, &iter);
	যাবত্ ((oid = oidset_iter_next(&iter))) {
		সংরচনা configured_exclusion *ex =
			oidmap_get(&configured_exclusions, oid);

		যদি (!ex)
			BUG("configured exclusion wasn't configured");
		write_in_full(1, ex->pack_hash_hex, strlen(ex->pack_hash_hex));
		write_in_full(1, " ", 1);
		write_in_full(1, ex->uri, strlen(ex->uri));
		write_in_full(1, "\n", 1);
	}
}

স্থির ধ্রুব বর্ণ no_split_warning[] = N_(
"disabling bitmap writing, packs are split due to pack.packSizeLimit"
);

স্থির শূন্য write_pack_file(শূন্য)
{
	uint32_t i = 0, j;
	সংরচনা hashfile *f;
	off_t offset;
	uint32_t nr_remaining = nr_result;
	time_t last_mtime = 0;
	সংরচনা object_entry **write_order;
	off_t bytes_written = 0;

	যদি (progress > pack_to_stdout)
		progress_state = start_progress(the_repository,
						_("Writing objects"), nr_result);
	ALLOC_ARRAY(written_list, to_pack.nr_objects);
	write_order = compute_write_order();

	কুরু {
		unsigned বর্ণ hash[GIT_MAX_RAWSZ];
		বর্ণ *pack_tmp_name = রিক্ত;

		যদি (pack_to_stdout) {
			/*
			 * This command is most often invoked via
			 * git-upload-pack(1), which will typically chunk data
			 * into pktlines. As such, we use the maximum data
			 * length of them as buffer length.
			 *
			 * Note that we need to subtract one though to
			 * accommodate for the sideband byte.
			 */
			সংরচনা hashfd_options opts = {
				.progress = progress_state,
				.buffer_len = LARGE_PACKET_DATA_MAX - 1,
			};
			f = hashfd_ext(the_repository->hash_algo, 1,
				       "<stdout>", &opts);
		} অন্যথা {
			f = create_tmp_packfile(the_repository, &pack_tmp_name);
		}

		offset = write_pack_header(f, nr_remaining);

		যদি (reuse_packfiles_nr) {
			প্রতিজ্ঞা(pack_to_stdout);
			প্রতি (j = 0; j < reuse_packfiles_nr; j++) {
				reused_chunks_nr = 0;
				write_reused_pack(&reuse_packfiles[j], f);
				যদি (reused_chunks_nr)
					reuse_packfiles_used_nr++;
			}
			offset = hashfile_total(f);
		}

		nr_written = 0;
		প্রতি (; i < to_pack.nr_objects; i++) {
			সংরচনা object_entry *e = write_order[i];
			যদি (write_one(f, e, &offset) == WRITE_ONE_BREAK)
				ভঙ্গ;
			display_progress(progress_state, written);
		}

		bytes_written += hashfile_total(f) +
			the_repository->hash_algo->rawsz;
		যদি (pack_to_stdout) {
			/*
			 * We never fsync when writing to stdout since we may
			 * not be writing to an actual pack file. For instance,
			 * the upload-pack code passes a pipe here. Calling
			 * fsync on a pipe results in unnecessary
			 * synchronization with the reader on some platforms.
			 */
			finalize_hashfile(f, hash, FSYNC_COMPONENT_NONE,
					  CSUM_HASH_IN_STREAM | CSUM_CLOSE);
		} অন্যথা যদি (nr_written == nr_remaining) {
			finalize_hashfile(f, hash, FSYNC_COMPONENT_PACK,
					  CSUM_HASH_IN_STREAM | CSUM_FSYNC | CSUM_CLOSE);
		} অন্যথা {
			/*
			 * If we wrote the wrong number of entries in the
			 * header, rewrite it like in fast-import.
			 */

			পূর্ণাঙ্ক fd = finalize_hashfile(f, hash, FSYNC_COMPONENT_PACK, 0);
			fixup_pack_header_footer(the_hash_algo, fd, hash,
						 pack_tmp_name, nr_written,
						 hash, offset);
			close(fd);
			যদি (write_bitmap_index) {
				যদি (write_bitmap_index != WRITE_BITMAP_QUIET)
					warning(_(no_split_warning));
				write_bitmap_index = 0;
			}
		}

		যদি (!pack_to_stdout) {
			সংরচনা stat st;
			সংরচনা strbuf tmpname = STRBUF_INIT;
			সংরচনা bitmap_writer bitmap_writer;
			বর্ণ *idx_tmp_name = রিক্ত;

			/*
			 * Packs are runtime accessed in their mtime
			 * order since newer packs are more likely to contain
			 * younger objects.  So if we are creating multiple
			 * packs then we should modify the mtime of later ones
			 * to preserve this property.
			 */
			যদি (stat(pack_tmp_name, &st) < 0) {
				warning_errno(_("failed to stat %s"), pack_tmp_name);
			} অন্যথা যদি (!last_mtime) {
				last_mtime = st.st_mtime;
			} অন্যথা {
				সংরচনা utimbuf utb;
				utb.actime = st.st_atime;
				utb.modtime = --last_mtime;
				যদি (utime(pack_tmp_name, &utb) < 0)
					warning_errno(_("failed utime() on %s"), pack_tmp_name);
			}

			strbuf_addf(&tmpname, "%s-%s.", base_name,
				    hash_to_hex(hash));

			যদি (write_bitmap_index) {
				bitmap_writer_init(&bitmap_writer,
						   the_repository, &to_pack,
						   রিক্ত);
				bitmap_writer_set_checksum(&bitmap_writer, hash);
				bitmap_writer_build_type_index(&bitmap_writer,
							       written_list);
			}

			যদি (cruft)
				pack_idx_opts.flags |= WRITE_MTIMES;

			stage_tmp_packfiles(the_repository, &tmpname,
					    pack_tmp_name, written_list,
					    nr_written, &to_pack,
					    &pack_idx_opts, hash,
					    &idx_tmp_name);

			যদি (write_bitmap_index) {
				size_t tmpname_len = tmpname.len;

				strbuf_addstr(&tmpname, "bitmap");
				stop_progress(&progress_state);

				bitmap_writer_show_progress(&bitmap_writer,
							    progress);
				bitmap_writer_select_commits(&bitmap_writer,
							     indexed_commits,
							     indexed_commits_nr);
				যদি (bitmap_writer_build(&bitmap_writer) < 0)
					die(_("failed to write bitmap index"));
				bitmap_writer_finish(&bitmap_writer,
						     written_list,
						     tmpname.buf, write_bitmap_options);
				bitmap_writer_free(&bitmap_writer);
				write_bitmap_index = 0;
				strbuf_setlen(&tmpname, tmpname_len);
			}

			rename_tmp_packfile_idx(the_repository, &tmpname, &idx_tmp_name);

			মুক্ত(idx_tmp_name);
			strbuf_release(&tmpname);
			মুক্ত(pack_tmp_name);
			puts(hash_to_hex(hash));
		}

		/* mark written objects as written to previous pack */
		প্রতি (j = 0; j < nr_written; j++) {
			written_list[j]->offset = (off_t)-1;
		}
		nr_remaining -= nr_written;
	} যাবত্ (nr_remaining && i < to_pack.nr_objects);

	মুক্ত(written_list);
	মুক্ত(write_order);
	stop_progress(&progress_state);
	যদি (written != nr_result)
		die(_("wrote %"PRIu32" objects while expecting %"PRIu32),
		    written, nr_result);
	trace2_data_intmax("pack-objects", the_repository,
			   "write_pack_file/wrote", nr_result);
	trace2_data_intmax("pack-objects", the_repository,
			   "write_pack_file/wrote_bytes", bytes_written);
}

স্থির পূর্ণাঙ্ক no_try_delta(ধ্রুব বর্ণ *path)
{
	স্থির সংরচনা attr_check *check;

	যদি (!check)
		check = attr_check_initl("delta", রিক্ত);
	git_check_attr(the_repository->index, path, check);
	যদি (ATTR_FALSE(check->বস্তুসমূহ[0].মান))
		প্রত্যাগম 1;
	প্রত্যাগম 0;
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
স্থির পূর্ণাঙ্ক have_duplicate_entry(ধ্রুব সংরচনা object_id *oid,
				পূর্ণাঙ্ক exclude)
{
	সংরচনা object_entry *entry;

	যদি (reuse_packfile_bitmap &&
	    bitmap_walk_contains(bitmap_git, reuse_packfile_bitmap, oid))
		প্রত্যাগম 1;

	entry = packlist_find(&to_pack, oid);
	যদি (!entry)
		প্রত্যাগম 0;

	যদি (exclude) {
		যদি (!entry->preferred_base)
			nr_result--;
		entry->preferred_base = 1;
	}

	প্রত্যাগম 1;
}

স্থির পূর্ণাঙ্ক want_cruft_object_mtime(সংরচনা repository *r,
				   ধ্রুব সংরচনা object_id *oid,
				   unsigned flags, uint32_t mtime)
{
	সংরচনা odb_source_files *files = odb_source_files_downcast(r->objects->source);

	প্রতি (সংরচনা odb_files_dir *dir = files->dirs; dir; dir = dir->next) {
		সংরচনা packed_git **cache = packfile_store_get_kept_pack_cache(dir->packed, flags);

		প্রতি (; *cache; cache++) {
			সংরচনা packed_git *p = *cache;
			off_t ofs;
			uint32_t candidate_mtime;

			ofs = find_pack_entry_one(oid, p);
			যদি (!ofs)
				অনুবর্ত;

			/*
			 * We have a copy of the object 'oid' in a non-cruft
			 * pack. We can avoid packing an additional copy
			 * regardless of what the existing copy's mtime is since
			 * it is outside of a cruft pack.
			 */
			যদি (!p->is_cruft)
				প্রত্যাগম 0;

			/*
			 * If we have a copy of the object 'oid' in a cruft
			 * pack, then either read the cruft pack's mtime for
			 * that object, or, if that can't be loaded, assume the
			 * pack's mtime itself.
			 */
			যদি (!load_pack_mtimes(p)) {
				uint32_t pos;
				যদি (offset_to_pack_pos(p, ofs, &pos) < 0)
					অনুবর্ত;
				candidate_mtime = nth_packed_mtime(p, pos);
			} অন্যথা {
				candidate_mtime = p->mtime;
			}

			/*
			 * We have a surviving copy of the object in a cruft
			 * pack whose mtime is greater than or equal to the one
			 * we are considering. We can thus avoid packing an
			 * additional copy of that object.
			 */
			যদি (mtime <= candidate_mtime)
				প্রত্যাগম 0;
		}
	}

	প্রত্যাগম -1;
}

স্থির পূর্ণাঙ্ক want_found_object(ধ্রুব সংরচনা object_id *oid, পূর্ণাঙ্ক exclude,
			     সংরচনা packed_git *p, uint32_t mtime)
{
	যদি (exclude)
		প্রত্যাগম 1;
	যদি (incremental)
		প্রত্যাগম 0;

	যদি (!is_pack_valid(p))
		প্রত্যাগম -1;

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
	যদি (local && !p->pack_local)
		প্রত্যাগম 0;

	/*
	 * Then handle .keep first, as we have a fast(er) path there.
	 */
	যদি (ignore_packed_keep_on_disk || ignore_packed_keep_in_core ||
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
		যদি (ignore_packed_keep_on_disk)
			flags |= KEPT_PACK_ON_DISK;
		যদি (ignore_packed_keep_in_core)
			flags |= KEPT_PACK_IN_CORE;
		যদি (ignore_packed_keep_in_core_open)
			flags |= KEPT_PACK_IN_CORE_OPEN;

		/*
		 * If the object is in a pack that we want to ignore, *and* we
		 * don't have any cruft packs that are being retained, we can
		 * abort quickly.
		 */
		যদি (!ignore_packed_keep_in_core_has_cruft) {
			যদি (ignore_packed_keep_on_disk && p->pack_keep)
				প্রত্যাগম 0;
			যদি (ignore_packed_keep_in_core && p->pack_keep_in_core)
				প্রত্যাগম 0;
			যদি (ignore_packed_keep_in_core_open && p->pack_keep_in_core_open)
				প্রত্যাগম 0;
			যদি (has_object_kept_pack(p->repo, oid, flags))
				প্রত্যাগম 0;
		} অন্যথা {
			/*
			 * But if there is at least one cruft pack which
			 * is being kept, we only want to include the
			 * provided object if it has a strictly greater
			 * mtime than any existing cruft copy.
			 */
			যদি (!want_cruft_object_mtime(p->repo, oid, flags,
						     mtime))
				প্রত্যাগম 0;
		}
	}

	/*
	 * At this point we know definitively that either we don't care about
	 * keep-packs, or the object is not in one. Keep checking other
	 * conditions...
	 */
	যদি (!local || !have_non_local_packs)
		প্রত্যাগম 1;

	/* we don't know yet; keep looking for more packs */
	প্রত্যাগম -1;
}

স্থির পূর্ণাঙ্ক want_object_in_pack_one(সংরচনা packed_git *p,
				   ধ্রুব সংরচনা object_id *oid,
				   পূর্ণাঙ্ক exclude,
				   সংরচনা packed_git **found_pack,
				   off_t *found_offset,
				   uint32_t found_mtime)
{
	off_t offset;

	যদি (p == *found_pack)
		offset = *found_offset;
	অন্যথা
		offset = find_pack_entry_one(oid, p);

	যদি (offset) {
		যদি (!*found_pack) {
			যদি (!is_pack_valid(p))
				প্রত্যাগম -1;
			*found_offset = offset;
			*found_pack = p;
		}
		প্রত্যাগম want_found_object(oid, exclude, p, found_mtime);
	}
	প্রত্যাগম -1;
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
স্থির পূর্ণাঙ্ক want_object_in_pack_mtime(ধ্রুব সংরচনা object_id *oid,
				     পূর্ণাঙ্ক exclude,
				     সংরচনা packed_git **found_pack,
				     off_t *found_offset,
				     uint32_t found_mtime)
{
	সংরচনা odb_source_files *files =
		odb_source_files_downcast(the_repository->objects->source);
	পূর্ণাঙ্ক want;
	সংরচনা packfile_list_entry *e;

	যদি (!exclude && local) {
		/*
		 * Note that we start iterating at `dirs->next` so that we
		 * skip the local object directory.
		 */
		প্রতি (সংরচনা odb_files_dir *dir = files->dirs->next; dir; dir = dir->next)
			যদি (!odb_source_read_object_info(&dir->loose->base, oid, রিক্ত, 0, রিক্ত))
				প্রত্যাগম 0;
	}

	/*
	 * If we already know the pack object lives in, start checks from that
	 * pack - in the usual case when neither --local was given nor .keep files
	 * are present we will determine the answer right now.
	 */
	যদি (*found_pack) {
		want = want_found_object(oid, exclude, *found_pack,
					 found_mtime);
		যদি (want != -1)
			প্রত্যাগম want;

		*found_pack = রিক্ত;
		*found_offset = 0;
	}

	প্রতি (সংরচনা odb_files_dir *dir = files->dirs; dir; dir = dir->next) {
		সংরচনা multi_pack_index *m = get_multi_pack_index(dir->packed);
		সংরচনা pack_entry e;

		যদি (m && midx_fill_entry(m, oid, &e, রিক্ত) == MIDX_FILL_HIT) {
			want = want_object_in_pack_one(e.p, oid, exclude, found_pack, found_offset, found_mtime);
			যদি (want != -1)
				প্রত্যাগম want;
		}
	}

	প্রতি (সংরচনা odb_files_dir *dir = files->dirs; dir; dir = dir->next) {
		প্রতি (e = dir->packed->packs.head; e; e = e->next) {
			সংরচনা packed_git *p = e->pack;
			want = want_object_in_pack_one(p, oid, exclude, found_pack, found_offset, found_mtime);
			যদি (!exclude && want > 0)
				packfile_list_prepend(&dir->packed->packs, p);
			যদি (want != -1)
				প্রত্যাগম want;
		}
	}

	যদি (uri_protocols.nr) {
		সংরচনা configured_exclusion *ex =
			oidmap_get(&configured_exclusions, oid);
		পূর্ণাঙ্ক i;
		ধ্রুব বর্ণ *p;

		যদি (ex) {
			প্রতি (i = 0; i < uri_protocols.nr; i++) {
				যদি (skip_prefix(ex->uri,
						uri_protocols.বস্তুসমূহ[i].string,
						&p) &&
				    *p == ':') {
					oidset_ঢোকাও(&excluded_by_config, oid);
					প্রত্যাগম 0;
				}
			}
		}
	}

	প্রত্যাগম 1;
}

স্থির অন্তঃস্থ পূর্ণাঙ্ক want_object_in_pack(ধ্রুব সংরচনা object_id *oid,
				      পূর্ণাঙ্ক exclude,
				      সংরচনা packed_git **found_pack,
				      off_t *found_offset)
{
	প্রত্যাগম want_object_in_pack_mtime(oid, exclude, found_pack, found_offset,
					 0);
}

স্থির সংরচনা object_entry *create_object_entry(ধ্রুব সংরচনা object_id *oid,
						গণনা object_type type,
						uint32_t hash,
						পূর্ণাঙ্ক exclude,
						পূর্ণাঙ্ক no_try_delta,
						সংরচনা packed_git *found_pack,
						off_t found_offset)
{
	সংরচনা object_entry *entry;

	entry = packlist_alloc(&to_pack, oid);
	entry->hash = hash;
	oe_set_type(entry, type);
	যদি (exclude)
		entry->preferred_base = 1;
	অন্যথা
		nr_result++;
	যদি (found_pack) {
		oe_set_in_pack(&to_pack, entry, found_pack);
		entry->in_pack_offset = found_offset;
	}

	entry->no_try_delta = no_try_delta;

	প্রত্যাগম entry;
}

স্থির ধ্রুব বর্ণ no_closure_warning[] = N_(
"disabling bitmap writing, as some objects are not being packed"
);

স্থির শূন্য add_object_entry(ধ্রুব সংরচনা object_id *oid, গণনা object_type type,
			     ধ্রুব বর্ণ *name, পূর্ণাঙ্ক exclude)
{
	সংরচনা packed_git *found_pack = রিক্ত;
	off_t found_offset = 0;

	display_progress(progress_state, ++nr_seen);

	যদি (have_duplicate_entry(oid, exclude))
		প্রত্যাগম;

	যদি (!want_object_in_pack(oid, exclude, &found_pack, &found_offset)) {
		/* The pack is missing an object, so it will not have closure */
		যদি (write_bitmap_index) {
			যদি (write_bitmap_index != WRITE_BITMAP_QUIET)
				warning(_(no_closure_warning));
			write_bitmap_index = 0;
		}
		প্রত্যাগম;
	}

	create_object_entry(oid, type, pack_name_hash_fn(name),
			    exclude, name && no_try_delta(name),
			    found_pack, found_offset);
}

স্থির পূর্ণাঙ্ক add_object_entry_from_bitmap(ধ্রুব সংরচনা object_id *oid,
					গণনা object_type type,
					পূর্ণাঙ্ক flags UNUSED, uint32_t name_hash,
					সংরচনা packed_git *pack, off_t offset,
					শূন্য *payload UNUSED)
{
	display_progress(progress_state, ++nr_seen);

	যদি (have_duplicate_entry(oid, 0))
		প্রত্যাগম 0;

	যদি (!want_object_in_pack(oid, 0, &pack, &offset))
		প্রত্যাগম 0;

	create_object_entry(oid, type, name_hash, 0, 0, pack, offset);
	প্রত্যাগম 0;
}

সংরচনা pbase_বৃক্ষ_cache {
	সংরচনা object_id oid;
	পূর্ণাঙ্ক ref;
	পূর্ণাঙ্ক temporary;
	শূন্য *বৃক্ষ_data;
	unsigned long বৃক্ষ_size;
};

স্থির সংরচনা pbase_বৃক্ষ_cache *(pbase_বৃক্ষ_cache[256]);
স্থির পূর্ণাঙ্ক pbase_বৃক্ষ_cache_ix(ধ্রুব সংরচনা object_id *oid)
{
	প্রত্যাগম oid->hash[0] % ARRAY_SIZE(pbase_বৃক্ষ_cache);
}
স্থির পূর্ণাঙ্ক pbase_বৃক্ষ_cache_ix_incr(পূর্ণাঙ্ক ix)
{
	প্রত্যাগম (ix+1) % ARRAY_SIZE(pbase_বৃক্ষ_cache);
}

স্থির সংরচনা pbase_বৃক্ষ {
	সংরচনা pbase_বৃক্ষ *next;
	/* This is a phony "cache" entry; we are not
	 * going to evict it or find it through _get()
	 * mechanism -- this is for the toplevel node that
	 * would almost always change with any commit.
	 */
	সংরচনা pbase_বৃক্ষ_cache pcache;
} *pbase_বৃক্ষ;

স্থির সংরচনা pbase_বৃক্ষ_cache *pbase_বৃক্ষ_get(ধ্রুব সংরচনা object_id *oid)
{
	সংরচনা pbase_বৃক্ষ_cache *ent, *nent;
	শূন্য *data;
	unsigned long size;
	size_t size_st = 0;
	গণনা object_type type;
	পূর্ণাঙ্ক neigh;
	পূর্ণাঙ্ক my_ix = pbase_বৃক্ষ_cache_ix(oid);
	পূর্ণাঙ্ক available_ix = -1;

	/* pbase-tree-cache acts as a limited hashtable.
	 * your object will be found at your index or within a few
	 * slots after that slot if it is cached.
	 */
	প্রতি (neigh = 0; neigh < 8; neigh++) {
		ent = pbase_বৃক্ষ_cache[my_ix];
		যদি (ent && oideq(&ent->oid, oid)) {
			ent->ref++;
			প্রত্যাগম ent;
		}
		অন্যথা যদি (((available_ix < 0) && (!ent || !ent->ref)) ||
			 ((0 <= available_ix) &&
			  (!ent && pbase_বৃক্ষ_cache[available_ix])))
			available_ix = my_ix;
		যদি (!ent)
			ভঙ্গ;
		my_ix = pbase_বৃক্ষ_cache_ix_incr(my_ix);
	}

	/* Did not find one.  Either we got a bogus request or
	 * we need to read and perhaps cache.
	 */
	data = odb_read_object(the_repository->objects, oid, &type, &size_st);
	size = cast_size_t_to_ulong(size_st);
	যদি (!data)
		প্রত্যাগম রিক্ত;
	যদি (type != OBJ_বৃক্ষ) {
		মুক্ত(data);
		প্রত্যাগম রিক্ত;
	}

	/* We need to either cache or return a throwaway copy */

	যদি (available_ix < 0)
		ent = রিক্ত;
	অন্যথা {
		ent = pbase_বৃক্ষ_cache[available_ix];
		my_ix = available_ix;
	}

	যদি (!ent) {
		nent = xmalloc(পরিমাণ(*nent));
		nent->temporary = (available_ix < 0);
	}
	অন্যথা {
		/* evict and reuse */
		মুক্ত(ent->বৃক্ষ_data);
		nent = ent;
	}
	oidcpy(&nent->oid, oid);
	nent->বৃক্ষ_data = data;
	nent->বৃক্ষ_size = size;
	nent->ref = 1;
	যদি (!nent->temporary)
		pbase_বৃক্ষ_cache[my_ix] = nent;
	প্রত্যাগম nent;
}

স্থির শূন্য pbase_বৃক্ষ_put(সংরচনা pbase_বৃক্ষ_cache *cache)
{
	যদি (!cache->temporary) {
		cache->ref--;
		প্রত্যাগম;
	}
	মুক্ত(cache->বৃক্ষ_data);
	মুক্ত(cache);
}

স্থির size_t name_cmp_len(ধ্রুব বর্ণ *name)
{
	প্রত্যাগম strcspn(name, "\n/");
}

স্থির শূন্য add_pbase_object(সংরচনা বৃক্ষ_desc *বৃক্ষ,
			     ধ্রুব বর্ণ *name,
			     size_t cmplen,
			     ধ্রুব বর্ণ *fullname)
{
	সংরচনা name_entry entry;
	পূর্ণাঙ্ক cmp;

	যাবত্ (বৃক্ষ_entry(বৃক্ষ,&entry)) {
		যদি (S_ISGITLINK(entry.mode))
			অনুবর্ত;
		cmp = বৃক্ষ_entry_len(&entry) != cmplen ? 1 :
		      memcmp(name, entry.path, cmplen);
		যদি (cmp > 0)
			অনুবর্ত;
		যদি (cmp < 0)
			প্রত্যাগম;
		যদি (name[cmplen] != '/') {
			add_object_entry(&entry.oid,
					 object_type(entry.mode),
					 fullname, 1);
			প্রত্যাগম;
		}
		যদি (S_ISDIR(entry.mode)) {
			সংরচনা বৃক্ষ_desc sub;
			সংরচনা pbase_বৃক্ষ_cache *বৃক্ষ;
			ধ্রুব বর্ণ *down = name+cmplen+1;
			size_t downlen = name_cmp_len(down);

			বৃক্ষ = pbase_বৃক্ষ_get(&entry.oid);
			যদি (!বৃক্ষ)
				প্রত্যাগম;
			init_বৃক্ষ_desc(&sub, &বৃক্ষ->oid,
				       বৃক্ষ->বৃক্ষ_data, বৃক্ষ->বৃক্ষ_size);

			add_pbase_object(&sub, down, downlen, fullname);
			pbase_বৃক্ষ_put(বৃক্ষ);
		}
	}
}

স্থির unsigned *done_pbase_paths;
স্থির পূর্ণাঙ্ক done_pbase_paths_অঙ্ক;
স্থির পূর্ণাঙ্ক done_pbase_paths_alloc;
স্থির পূর্ণাঙ্ক done_pbase_path_pos(unsigned hash)
{
	পূর্ণাঙ্ক lo = 0;
	পূর্ণাঙ্ক hi = done_pbase_paths_অঙ্ক;
	যাবত্ (lo < hi) {
		পূর্ণাঙ্ক mi = lo + (hi - lo) / 2;
		যদি (done_pbase_paths[mi] == hash)
			প্রত্যাগম mi;
		যদি (done_pbase_paths[mi] < hash)
			hi = mi;
		অন্যথা
			lo = mi + 1;
	}
	প্রত্যাগম -lo-1;
}

স্থির পূর্ণাঙ্ক check_pbase_path(unsigned hash)
{
	পূর্ণাঙ্ক pos = done_pbase_path_pos(hash);
	যদি (0 <= pos)
		প্রত্যাগম 1;
	pos = -pos - 1;
	ALLOC_GROW(done_pbase_paths,
		   done_pbase_paths_অঙ্ক + 1,
		   done_pbase_paths_alloc);
	done_pbase_paths_অঙ্ক++;
	যদি (pos < done_pbase_paths_অঙ্ক)
		MOVE_ARRAY(done_pbase_paths + pos + 1, done_pbase_paths + pos,
			   done_pbase_paths_অঙ্ক - pos - 1);
	done_pbase_paths[pos] = hash;
	প্রত্যাগম 0;
}

স্থির শূন্য add_preferred_base_object(ধ্রুব বর্ণ *name)
{
	সংরচনা pbase_বৃক্ষ *it;
	size_t cmplen;
	unsigned hash = pack_name_hash_fn(name);

	যদি (!অঙ্ক_preferred_base || check_pbase_path(hash))
		প্রত্যাগম;

	cmplen = name_cmp_len(name);
	প্রতি (it = pbase_বৃক্ষ; it; it = it->next) {
		যদি (cmplen == 0) {
			add_object_entry(&it->pcache.oid, OBJ_বৃক্ষ, রিক্ত, 1);
		}
		অন্যথা {
			সংরচনা বৃক্ষ_desc বৃক্ষ;
			init_বৃক্ষ_desc(&বৃক্ষ, &it->pcache.oid,
				       it->pcache.বৃক্ষ_data, it->pcache.বৃক্ষ_size);
			add_pbase_object(&বৃক্ষ, name, cmplen, name);
		}
	}
}

স্থির শূন্য add_preferred_base(সংরচনা object_id *oid)
{
	সংরচনা pbase_বৃক্ষ *it;
	শূন্য *data;
	unsigned long size;
	size_t size_st = 0;
	সংরচনা object_id বৃক্ষ_oid;

	যদি (window <= অঙ্ক_preferred_base++)
		প্রত্যাগম;

	data = odb_read_object_peeled(the_repository->objects, oid,
				      OBJ_বৃক্ষ, &size_st, &বৃক্ষ_oid);
	size = cast_size_t_to_ulong(size_st);
	যদি (!data)
		প্রত্যাগম;

	প্রতি (it = pbase_বৃক্ষ; it; it = it->next) {
		যদি (oideq(&it->pcache.oid, &বৃক্ষ_oid)) {
			মুক্ত(data);
			প্রত্যাগম;
		}
	}

	CALLOC_ARRAY(it, 1);
	it->next = pbase_বৃক্ষ;
	pbase_বৃক্ষ = it;

	oidcpy(&it->pcache.oid, &বৃক্ষ_oid);
	it->pcache.বৃক্ষ_data = data;
	it->pcache.বৃক্ষ_size = size;
}

স্থির শূন্য cleanup_preferred_base(শূন্য)
{
	সংরচনা pbase_বৃক্ষ *it;
	unsigned i;

	it = pbase_বৃক্ষ;
	pbase_বৃক্ষ = রিক্ত;
	যাবত্ (it) {
		সংরচনা pbase_বৃক্ষ *tmp = it;
		it = tmp->next;
		মুক্ত(tmp->pcache.বৃক্ষ_data);
		মুক্ত(tmp);
	}

	প্রতি (i = 0; i < ARRAY_SIZE(pbase_বৃক্ষ_cache); i++) {
		যদি (!pbase_বৃক্ষ_cache[i])
			অনুবর্ত;
		মুক্ত(pbase_বৃক্ষ_cache[i]->বৃক্ষ_data);
		FREE_AND_NULL(pbase_বৃক্ষ_cache[i]);
	}

	FREE_AND_NULL(done_pbase_paths);
	done_pbase_paths_অঙ্ক = done_pbase_paths_alloc = 0;
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
স্থির পূর্ণাঙ্ক can_reuse_delta(ধ্রুব সংরচনা object_id *base_oid,
			   সংরচনা object_entry *delta,
			   সংরচনা object_entry **base_out)
{
	সংরচনা object_entry *base;

	/*
	 * First see if we're already sending the base (or it's explicitly in
	 * our "excluded" list).
	 */
	base = packlist_find(&to_pack, base_oid);
	যদি (base) {
		যদি (!in_same_island(&delta->idx.oid, &base->idx.oid))
			প্রত্যাগম 0;
		*base_out = base;
		প্রত্যাগম 1;
	}

	/*
	 * Otherwise, reachability bitmaps may tell us if the receiver has it,
	 * even if it was buried too deep in history to make it into the
	 * packing list.
	 */
	যদি (thin && bitmap_has_oid_in_uninteresting(bitmap_git, base_oid)) {
		যদি (use_delta_islands) {
			যদি (!in_same_island(&delta->idx.oid, base_oid))
				প্রত্যাগম 0;
		}
		*base_out = রিক্ত;
		প্রত্যাগম 1;
	}

	প্রত্যাগম 0;
}

স্থির শূন্য prefetch_to_pack(uint32_t object_index_start) {
	সংরচনা oid_array to_fetch = OID_ARRAY_INIT;
	uint32_t i;

	প্রতি (i = object_index_start; i < to_pack.nr_objects; i++) {
		সংরচনা object_entry *entry = to_pack.objects + i;

		যদি (!odb_read_object_info_extended(the_repository->objects,
						   &entry->idx.oid,
						   রিক্ত,
						   OBJECT_INFO_FOR_PREFETCH))
			অনুবর্ত;
		oid_array_append(&to_fetch, &entry->idx.oid);
	}
	promisor_remote_get_direct(the_repository,
				   to_fetch.oid, to_fetch.nr);
	oid_array_clear(&to_fetch);
}

স্থির শূন্য check_object(সংরচনা object_entry *entry, uint32_t object_index)
{
	size_t canonical_size;
	গণনা object_type type;
	সংরচনা object_info oi = {.typep = &type, .sizep = &canonical_size};

	যদি (IN_PACK(entry)) {
		সংরচনা packed_git *p = IN_PACK(entry);
		সংরচনা pack_window *w_curs = রিক্ত;
		পূর্ণাঙ্ক have_base = 0;
		সংরচনা object_id base_ref;
		সংরচনা object_entry *base_entry;
		size_t used, used_0, avail;
		off_t ofs;
		unsigned বর্ণ *buf, c;
		গণনা object_type type;
		size_t in_pack_size;

		buf = use_pack(p, &w_curs, entry->in_pack_offset, &avail);

		/*
		 * We want in_pack_type even if we do not reuse delta
		 * since non-delta representations could still be reused.
		 */
		used = unpack_object_header_buffer(buf, avail,
						   &type,
						   &in_pack_size);
		যদি (used == 0)
			গচ্ছ give_up;

		যদি (type < 0)
			BUG("invalid type %d", type);
		entry->in_pack_type = type;

		/*
		 * Determine if this is a delta and if so whether we can
		 * reuse it or not.  Otherwise let's find out as cheaply as
		 * possible what the actual type and size for this object is.
		 */
		বিকল্প (entry->in_pack_type) {
		স্বভাব:
			/* Not a delta hence we've already got all we need. */
			oe_set_type(entry, entry->in_pack_type);
			SET_SIZE(entry, in_pack_size);
			entry->in_pack_header_size = used;
			যদি (oe_type(entry) < OBJ_COMMIT || oe_type(entry) > OBJ_BLOB)
				গচ্ছ give_up;
			unuse_pack(&w_curs);
			প্রত্যাগম;
		পক্ষ OBJ_REF_DELTA:
			যদি (reuse_delta && !entry->preferred_base) {
				oidread(&base_ref,
					use_pack(p, &w_curs,
						 entry->in_pack_offset + used,
						 রিক্ত),
					the_repository->hash_algo);
				have_base = 1;
			}
			entry->in_pack_header_size = used + the_hash_algo->rawsz;
			ভঙ্গ;
		পক্ষ OBJ_OFS_DELTA:
			buf = use_pack(p, &w_curs,
				       entry->in_pack_offset + used, রিক্ত);
			used_0 = 0;
			c = buf[used_0++];
			ofs = c & 127;
			যাবত্ (c & 128) {
				ofs += 1;
				যদি (!ofs || MSB(ofs, 7)) {
					error(_("delta base offset overflow in pack for %s"),
					      oid_to_hex(&entry->idx.oid));
					গচ্ছ give_up;
				}
				c = buf[used_0++];
				ofs = (ofs << 7) + (c & 127);
			}
			ofs = entry->in_pack_offset - ofs;
			যদি (ofs <= 0 || ofs >= entry->in_pack_offset) {
				error(_("delta base offset out of bound for %s"),
				      oid_to_hex(&entry->idx.oid));
				গচ্ছ give_up;
			}
			যদি (reuse_delta && !entry->preferred_base) {
				uint32_t pos;
				যদি (offset_to_pack_pos(p, ofs, &pos) < 0)
					গচ্ছ give_up;
				যদি (!nth_packed_object_id(&base_ref, p,
							  pack_pos_to_index(p, pos)))
					have_base = 1;
			}
			entry->in_pack_header_size = used + used_0;
			ভঙ্গ;
		}

		যদি (have_base &&
		    can_reuse_delta(&base_ref, entry, &base_entry)) {
			oe_set_type(entry, entry->in_pack_type);
			SET_SIZE(entry, in_pack_size); /* delta size */
			SET_DELTA_SIZE(entry, in_pack_size);

			যদি (base_entry) {
				SET_DELTA(entry, base_entry);
				entry->delta_sibling_idx = base_entry->delta_child_idx;
				SET_DELTA_CHILD(base_entry, entry);
			} অন্যথা {
				SET_DELTA_EXT(entry, &base_ref);
			}

			unuse_pack(&w_curs);
			প্রত্যাগম;
		}

		যদি (oe_type(entry)) {
			off_t delta_pos;

			/*
			 * This must be a delta and we already know what the
			 * final object type is.  Let's extract the actual
			 * object size from the delta header.
			 */
			delta_pos = entry->in_pack_offset + entry->in_pack_header_size;
			canonical_size = get_size_from_delta(p, &w_curs,
							     delta_pos);
			যদি (canonical_size == 0)
				গচ্ছ give_up;
			SET_SIZE(entry, canonical_size);
			unuse_pack(&w_curs);
			প্রত্যাগম;
		}

		/*
		 * No choice but to fall back to the recursive delta walk
		 * with odb_read_object_info() to find about the object type
		 * at this point...
		 */
		give_up:
		unuse_pack(&w_curs);
	}

	যদি (odb_read_object_info_extended(the_repository->objects, &entry->idx.oid, &oi,
					  OBJECT_INFO_SKIP_FETCH_OBJECT | OBJECT_INFO_LOOKUP_REPLACE) < 0) {
		যদি (repo_has_promisor_remote(the_repository)) {
			prefetch_to_pack(object_index);
			যদি (odb_read_object_info_extended(the_repository->objects, &entry->idx.oid, &oi,
							  OBJECT_INFO_SKIP_FETCH_OBJECT | OBJECT_INFO_LOOKUP_REPLACE) < 0)
				type = -1;
		} অন্যথা {
			type = -1;
		}
	}
	oe_set_type(entry, type);
	যদি (entry->type_valid) {
		SET_SIZE(entry, canonical_size);
	} অন্যথা {
		/*
		 * Bad object type is checked in prepare_pack().  This is
		 * to permit a missing preferred base object to be ignored
		 * as a preferred base.  Doing so can result in a larger
		 * pack file, but the transfer will still take place.
		 */
	}
}

স্থির পূর্ণাঙ্ক pack_offset_sort(ধ্রুব শূন্য *_a, ধ্রুব শূন্য *_b)
{
	ধ্রুব সংরচনা object_entry *a = *(সংরচনা object_entry **)_a;
	ধ্রুব সংরচনা object_entry *b = *(সংরচনা object_entry **)_b;
	ধ্রুব সংরচনা packed_git *a_in_pack = IN_PACK(a);
	ধ্রুব সংরচনা packed_git *b_in_pack = IN_PACK(b);

	/* avoid filesystem trashing with loose objects */
	যদি (!a_in_pack && !b_in_pack)
		প্রত্যাগম oidcmp(&a->idx.oid, &b->idx.oid);

	যদি (a_in_pack < b_in_pack)
		প্রত্যাগম -1;
	যদি (a_in_pack > b_in_pack)
		প্রত্যাগম 1;
	প্রত্যাগম a->in_pack_offset < b->in_pack_offset ? -1 :
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
স্থির শূন্য drop_reused_delta(সংরচনা object_entry *entry)
{
	unsigned *idx = &to_pack.objects[entry->delta_idx - 1].delta_child_idx;
	সংরচনা object_info oi = OBJECT_INFO_INIT;
	গণনা object_type type;
	size_t size;

	যাবত্ (*idx) {
		সংরচনা object_entry *oe = &to_pack.objects[*idx - 1];

		যদি (oe == entry)
			*idx = oe->delta_sibling_idx;
		অন্যথা
			idx = &oe->delta_sibling_idx;
	}
	SET_DELTA(entry, রিক্ত);
	entry->depth = 0;

	oi.sizep = &size;
	oi.typep = &type;
	যদি (packed_object_info(রিক্ত, IN_PACK(entry), entry->in_pack_offset, &oi) < 0) {
		/*
		 * We failed to get the info from this pack for some reason;
		 * fall back to odb_read_object_info, which may find another copy.
		 * And if that fails, the error will be recorded in oe_type(entry)
		 * and dealt with in prepare_pack().
		 */
		oe_set_type(entry,
			    odb_read_object_info(the_repository->objects,
						 &entry->idx.oid, &size));
	} অন্যথা {
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
স্থির শূন্য break_delta_chains(সংরচনা object_entry *entry)
{
	/*
	 * The actual depth of each object we will write is stored as an int,
	 * as it cannot exceed our int "depth" limit. But before we break
	 * changes based no that limit, we may potentially go as deep as the
	 * number of objects, which is elsewhere bounded to a uint32_t.
	 */
	uint32_t total_depth;
	সংরচনা object_entry *cur, *next;

	প্রতি (cur = entry, total_depth = 0;
	     cur;
	     cur = DELTA(cur), total_depth++) {
		যদি (cur->dfs_state == DFS_DONE) {
			/*
			 * We've already seen this object and know it isn't
			 * part of a cycle. We do need to append its depth
			 * to our count.
			 */
			total_depth += cur->depth;
			ভঙ্গ;
		}

		/*
		 * We break cycles before looping, so an ACTIVE state (or any
		 * other cruft which made its way into the state variable)
		 * is a bug.
		 */
		যদি (cur->dfs_state != DFS_NONE)
			BUG("confusing delta dfs state in first pass: %d",
			    cur->dfs_state);

		/*
		 * Now we know this is the first time we've seen the object. If
		 * it's not a delta, we're done traversing, but we'll mark it
		 * done to save time on future traversals.
		 */
		যদি (!DELTA(cur)) {
			cur->dfs_state = DFS_DONE;
			ভঙ্গ;
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
		যদি (DELTA(cur)->dfs_state == DFS_ACTIVE) {
			drop_reused_delta(cur);
			cur->dfs_state = DFS_DONE;
			ভঙ্গ;
		}
	}

	/*
	 * And now that we've gone all the way to the bottom of the chain, we
	 * need to clear the active flags and set the depth fields as
	 * appropriate. Unlike the loop above, which can quit when it drops a
	 * delta, we need to keep going to look for more depth cuts. So we need
	 * an extra "next" pointer to keep going after we reset cur->delta.
	 */
	প্রতি (cur = entry; cur; cur = next) {
		next = DELTA(cur);

		/*
		 * We should have a chain of zero or more ACTIVE states down to
		 * a final DONE. We can quit after the DONE, because either it
		 * has no bases, or we've already handled them in a previous
		 * call.
		 */
		যদি (cur->dfs_state == DFS_DONE)
			ভঙ্গ;
		অন্যথা যদি (cur->dfs_state != DFS_ACTIVE)
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
		যদি (!cur->depth)
			drop_reused_delta(cur);

		cur->dfs_state = DFS_DONE;
	}
}

স্থির শূন্য get_object_details(শূন্য)
{
	uint32_t i;
	সংরচনা object_entry **sorted_by_offset;

	যদি (progress)
		progress_state = start_progress(the_repository,
						_("Counting objects"),
						to_pack.nr_objects);

	CALLOC_ARRAY(sorted_by_offset, to_pack.nr_objects);
	প্রতি (i = 0; i < to_pack.nr_objects; i++)
		sorted_by_offset[i] = to_pack.objects + i;
	QSORT(sorted_by_offset, to_pack.nr_objects, pack_offset_sort);

	প্রতি (i = 0; i < to_pack.nr_objects; i++) {
		সংরচনা object_entry *entry = sorted_by_offset[i];
		check_object(entry, i);
		যদি (entry->type_valid &&
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
	প্রতি (i = 0; i < to_pack.nr_objects; i++)
		break_delta_chains(&to_pack.objects[i]);

	মুক্ত(sorted_by_offset);
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
স্থির পূর্ণাঙ্ক type_size_sort(ধ্রুব শূন্য *_a, ধ্রুব শূন্য *_b)
{
	ধ্রুব সংরচনা object_entry *a = *(সংরচনা object_entry **)_a;
	ধ্রুব সংরচনা object_entry *b = *(সংরচনা object_entry **)_b;
	ধ্রুব গণনা object_type a_type = oe_type(a);
	ধ্রুব গণনা object_type b_type = oe_type(b);
	ধ্রুব unsigned long a_size = SIZE(a);
	ধ্রুব unsigned long b_size = SIZE(b);

	যদি (a_type > b_type)
		প্রত্যাগম -1;
	যদি (a_type < b_type)
		প্রত্যাগম 1;
	যদি (a->hash > b->hash)
		প্রত্যাগম -1;
	যদি (a->hash < b->hash)
		প্রত্যাগম 1;
	যদি (a->preferred_base > b->preferred_base)
		প্রত্যাগম -1;
	যদি (a->preferred_base < b->preferred_base)
		প্রত্যাগম 1;
	যদি (use_delta_islands) {
		ধ্রুব পূর্ণাঙ্ক island_cmp = island_delta_cmp(&a->idx.oid, &b->idx.oid);
		যদি (island_cmp)
			প্রত্যাগম island_cmp;
	}
	যদি (a_size > b_size)
		প্রত্যাগম -1;
	যদি (a_size < b_size)
		প্রত্যাগম 1;
	প্রত্যাগম a < b ? -1 : (a > b);  /* newest first */
}

সংরচনা unpacked {
	সংরচনা object_entry *entry;
	শূন্য *data;
	সংরচনা delta_index *index;
	unsigned depth;
};

স্থির পূর্ণাঙ্ক delta_cacheable(size_t src_size, size_t trg_size,
			   size_t delta_size)
{
	যদি (max_delta_cache_size && delta_cache_size + delta_size > max_delta_cache_size)
		প্রত্যাগম 0;

	যদি (delta_size < cache_max_small_delta_size)
		প্রত্যাগম 1;

	/* cache delta, if objects are large enough compared to delta size */
	যদি ((src_size >> 20) + (trg_size >> 21) > (delta_size >> 10))
		প্রত্যাগম 1;

	প্রত্যাগম 0;
}

/* Protect delta_cache_size */
স্থির pthread_mutex_t cache_mutex;
#define cache_lock()		pthread_mutex_lock(&cache_mutex)
#define cache_unlock()		pthread_mutex_unlock(&cache_mutex)

/*
 * Protect object list partitioning (e.g. struct thread_param) and
 * progress_state
 */
স্থির pthread_mutex_t progress_mutex;
#define progress_lock()		pthread_mutex_lock(&progress_mutex)
#define progress_unlock()	pthread_mutex_unlock(&progress_mutex)

/*
 * Access to struct object_entry is unprotected since each thread owns
 * a portion of the main object list. Just don't access object entries
 * ahead in the list because they can be stolen and would need
 * progress_mutex for protection.
 */

স্থির অন্তঃস্থ পূর্ণাঙ্ক oe_size_less_than(সংরচনা packing_data *pack,
				    ধ্রুব সংরচনা object_entry *lhs,
				    size_t rhs)
{
	যদি (lhs->size_valid)
		প্রত্যাগম lhs->size_ < rhs;
	যদি (rhs < pack->oe_size_limit) /* rhs < 2^x <= lhs ? */
		প্রত্যাগম 0;
	প্রত্যাগম oe_get_size_slow(pack, lhs) < rhs;
}

স্থির অন্তঃস্থ শূন্য oe_set_বৃক্ষ_depth(সংরচনা packing_data *pack,
				     সংরচনা object_entry *e,
				     unsigned পূর্ণাঙ্ক বৃক্ষ_depth)
{
	যদি (!pack->বৃক্ষ_depth)
		CALLOC_ARRAY(pack->বৃক্ষ_depth, pack->nr_alloc);
	pack->বৃক্ষ_depth[e - pack->objects] = বৃক্ষ_depth;
}

স্থির শূন্য record_বৃক্ষ_depth(ধ্রুব সংরচনা object_id *oid, ধ্রুব বর্ণ *name)
{
	ধ্রুব বর্ণ *p;
	unsigned depth;
	সংরচনা object_entry *ent;

	/* the empty string is a root tree, which is depth 0 */
	depth = *name ? 1 : 0;
	প্রতি (p = strchr(name, '/'); p; p = strchr(p + 1, '/'))
		depth++;

	ent = packlist_find(&to_pack, oid);
	যদি (ent && depth > oe_বৃক্ষ_depth(&to_pack, ent))
		oe_set_বৃক্ষ_depth(&to_pack, ent, depth);
}

/*
 * Return the size of the object without doing any delta
 * reconstruction (so non-deltas are true object sizes, but deltas
 * return the size of the delta data).
 */
size_t oe_get_size_slow(সংরচনা packing_data *pack,
			ধ্রুব সংরচনা object_entry *e)
{
	সংরচনা packed_git *p;
	সংরচনা pack_window *w_curs;
	unsigned বর্ণ *buf;
	গণনা object_type type;
	size_t used, avail, size;

	যদি (e->type_ != OBJ_OFS_DELTA && e->type_ != OBJ_REF_DELTA) {
		size_t sz;
		packing_data_lock(&to_pack);
		যদি (odb_read_object_info(the_repository->objects,
					 &e->idx.oid, &sz) < 0)
			die(_("unable to get size of %s"),
			    oid_to_hex(&e->idx.oid));
		packing_data_unlock(&to_pack);
		প্রত্যাগম sz;
	}

	p = oe_in_pack(pack, e);
	যদি (!p)
		BUG("when e->type is a delta, it must belong to a pack");

	packing_data_lock(&to_pack);
	w_curs = রিক্ত;
	buf = use_pack(p, &w_curs, e->in_pack_offset, &avail);
	used = unpack_object_header_buffer(buf, avail, &type, &size);
	যদি (used == 0)
		die(_("unable to parse object header of %s"),
		    oid_to_hex(&e->idx.oid));

	unuse_pack(&w_curs);
	packing_data_unlock(&to_pack);
	প্রত্যাগম size;
}

স্থির পূর্ণাঙ্ক try_delta(সংরচনা unpacked *trg, সংরচনা unpacked *src,
		     unsigned max_depth, size_t *mem_usage)
{
	সংরচনা object_entry *trg_entry = trg->entry;
	সংরচনা object_entry *src_entry = src->entry;
	unsigned long trg_size, src_size, sizediff, max_size, sz;
	size_t delta_size;
	unsigned ref_depth;
	গণনা object_type type;
	শূন্য *delta_buf;

	/* Don't bother doing diffs between different types */
	যদি (oe_type(trg_entry) != oe_type(src_entry))
		প্রত্যাগম -1;

	/*
	 * We do not bother to try a delta that we discarded on an
	 * earlier try, but only when reusing delta data.  Note that
	 * src_entry that is marked as the preferred_base should always
	 * be considered, as even if we produce a suboptimal delta against
	 * it, we will still save the transfer cost, as we already know
	 * the other side has it and we won't send src_entry at all.
	 */
	যদি (reuse_delta && IN_PACK(trg_entry) &&
	    IN_PACK(trg_entry) == IN_PACK(src_entry) &&
	    !src_entry->preferred_base &&
	    trg_entry->in_pack_type != OBJ_REF_DELTA &&
	    trg_entry->in_pack_type != OBJ_OFS_DELTA)
		প্রত্যাগম 0;

	/* Let's not bust the allowed depth. */
	যদি (src->depth >= max_depth)
		প্রত্যাগম 0;

	/* Now some size filtering heuristics. */
	trg_size = SIZE(trg_entry);
	যদি (!DELTA(trg_entry)) {
		max_size = trg_size/2 - the_hash_algo->rawsz;
		ref_depth = 1;
	} অন্যথা {
		max_size = DELTA_SIZE(trg_entry);
		ref_depth = trg->depth;
	}
	max_size = (uint64_t)max_size * (max_depth - src->depth) /
						(max_depth - ref_depth + 1);
	যদি (max_size == 0)
		প্রত্যাগম 0;
	src_size = SIZE(src_entry);
	sizediff = src_size < trg_size ? trg_size - src_size : 0;
	যদি (sizediff >= max_size)
		প্রত্যাগম 0;
	যদি (trg_size < src_size / 32)
		প্রত্যাগম 0;

	যদি (!in_same_island(&trg->entry->idx.oid, &src->entry->idx.oid))
		প্রত্যাগম 0;

	/* Load data if not already done */
	যদি (!trg->data) {
		size_t sz_st = 0;
		packing_data_lock(&to_pack);
		trg->data = odb_read_object(the_repository->objects,
					    &trg_entry->idx.oid, &type,
					    &sz_st);
		sz = cast_size_t_to_ulong(sz_st);
		packing_data_unlock(&to_pack);
		যদি (!trg->data)
			die(_("object %s cannot be read"),
			    oid_to_hex(&trg_entry->idx.oid));
		যদি (sz != trg_size)
			die(_("object %s inconsistent object length (%"PRIuMAX" vs %"PRIuMAX")"),
			    oid_to_hex(&trg_entry->idx.oid), (uintmax_t)sz,
			    (uintmax_t)trg_size);
		*mem_usage += sz;
	}
	যদি (!src->data) {
		size_t sz_st = 0;
		packing_data_lock(&to_pack);
		src->data = odb_read_object(the_repository->objects,
					    &src_entry->idx.oid, &type,
					    &sz_st);
		sz = cast_size_t_to_ulong(sz_st);
		packing_data_unlock(&to_pack);
		যদি (!src->data) {
			যদি (src_entry->preferred_base) {
				স্থির পূর্ণাঙ্ক warned = 0;
				যদি (!warned++)
					warning(_("object %s cannot be read"),
						oid_to_hex(&src_entry->idx.oid));
				/*
				 * Those objects are not included in the
				 * resulting pack.  Be resilient and ignore
				 * them if they can't be read, in case the
				 * pack could be created nevertheless.
				 */
				প্রত্যাগম 0;
			}
			die(_("object %s cannot be read"),
			    oid_to_hex(&src_entry->idx.oid));
		}
		যদি (sz != src_size)
			die(_("object %s inconsistent object length (%"PRIuMAX" vs %"PRIuMAX")"),
			    oid_to_hex(&src_entry->idx.oid), (uintmax_t)sz,
			    (uintmax_t)src_size);
		*mem_usage += sz;
	}
	যদি (!src->index) {
		src->index = create_delta_index(src->data, src_size);
		যদি (!src->index) {
			স্থির পূর্ণাঙ্ক warned = 0;
			যদি (!warned++)
				warning(_("suboptimal pack - out of memory"));
			প্রত্যাগম 0;
		}
		*mem_usage += sizeof_delta_index(src->index);
	}

	delta_buf = create_delta(src->index, trg->data, trg_size, &delta_size, max_size);
	যদি (!delta_buf)
		প্রত্যাগম 0;

	যদি (DELTA(trg_entry)) {
		/* Prefer only shallower same-sized deltas. */
		যদি (delta_size == DELTA_SIZE(trg_entry) &&
		    src->depth + 1 >= trg->depth) {
			মুক্ত(delta_buf);
			প্রত্যাগম 0;
		}
	}

	/*
	 * Handle memory allocation outside of the cache
	 * accounting lock.  Compiler will optimize the strangeness
	 * away when NO_PTHREADS is defined.
	 */
	মুক্ত(trg_entry->delta_data);
	cache_lock();
	যদি (trg_entry->delta_data) {
		delta_cache_size -= DELTA_SIZE(trg_entry);
		trg_entry->delta_data = রিক্ত;
	}
	যদি (delta_cacheable(src_size, trg_size, delta_size)) {
		delta_cache_size += delta_size;
		cache_unlock();
		trg_entry->delta_data = xrealloc(delta_buf, delta_size);
	} অন্যথা {
		cache_unlock();
		মুক্ত(delta_buf);
	}

	SET_DELTA(trg_entry, src_entry);
	SET_DELTA_SIZE(trg_entry, delta_size);
	trg->depth = src->depth + 1;

	প্রত্যাগম 1;
}

স্থির unsigned পূর্ণাঙ্ক check_delta_limit(সংরচনা object_entry *me, unsigned পূর্ণাঙ্ক n)
{
	সংরচনা object_entry *child = DELTA_CHILD(me);
	unsigned পূর্ণাঙ্ক m = n;
	যাবত্ (child) {
		ধ্রুব unsigned পূর্ণাঙ্ক c = check_delta_limit(child, n + 1);
		যদি (m < c)
			m = c;
		child = DELTA_SIBLING(child);
	}
	প্রত্যাগম m;
}

স্থির size_t free_unpacked(সংরচনা unpacked *n)
{
	size_t freed_mem = sizeof_delta_index(n->index);
	free_delta_index(n->index);
	n->index = রিক্ত;
	যদি (n->data) {
		freed_mem += SIZE(n->entry);
		FREE_AND_NULL(n->data);
	}
	n->entry = রিক্ত;
	n->depth = 0;
	প্রত্যাগম freed_mem;
}

স্থির শূন্য find_deltas(সংরচনা object_entry **list, unsigned *list_size,
			পূর্ণাঙ্ক window, পূর্ণাঙ্ক depth, unsigned *processed)
{
	uint32_t i, idx = 0, গুনতি = 0;
	সংরচনা unpacked *array;
	size_t mem_usage = 0;

	CALLOC_ARRAY(array, window);

	প্রতি (;;) {
		সংরচনা object_entry *entry;
		সংরচনা unpacked *n = array + idx;
		পূর্ণাঙ্ক j, max_depth, best_base = -1;

		progress_lock();
		যদি (!*list_size) {
			progress_unlock();
			ভঙ্গ;
		}
		entry = *list++;
		(*list_size)--;
		যদি (!entry->preferred_base) {
			(*processed)++;
			display_progress(progress_state, *processed);
		}
		progress_unlock();

		mem_usage -= free_unpacked(n);
		n->entry = entry;

		যাবত্ (window_memory_limit &&
		       mem_usage > window_memory_limit &&
		       গুনতি > 1) {
			ধ্রুব uint32_t tail = (idx + window - গুনতি) % window;
			mem_usage -= free_unpacked(array + tail);
			গুনতি--;
		}

		/* We do not compute delta to *create* objects we are not
		 * going to pack.
		 */
		যদি (entry->preferred_base)
			গচ্ছ next;

		/*
		 * If the current object is at pack edge, take the depth the
		 * objects that depend on the current object into account
		 * otherwise they would become too deep.
		 */
		max_depth = depth;
		যদি (DELTA_CHILD(entry)) {
			max_depth -= check_delta_limit(entry, 0);
			যদি (max_depth <= 0)
				গচ্ছ next;
		}

		j = window;
		যাবত্ (--j > 0) {
			পূর্ণাঙ্ক ret;
			uint32_t other_idx = idx + j;
			সংরচনা unpacked *m;
			যদি (other_idx >= window)
				other_idx -= window;
			m = array + other_idx;
			যদি (!m->entry)
				ভঙ্গ;
			ret = try_delta(n, m, max_depth, &mem_usage);
			যদি (ret < 0)
				ভঙ্গ;
			অন্যথা যদি (ret > 0)
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
		যদি (entry->delta_data && !pack_to_stdout) {
			unsigned long size;

			size = do_compress(&entry->delta_data, DELTA_SIZE(entry));
			যদি (size < (1U << OE_Z_DELTA_BITS)) {
				entry->z_delta_size = size;
				cache_lock();
				delta_cache_size -= DELTA_SIZE(entry);
				delta_cache_size += entry->z_delta_size;
				cache_unlock();
			} অন্যথা {
				FREE_AND_NULL(entry->delta_data);
				entry->z_delta_size = 0;
			}
		}

		/* if we made n a delta, and if n is already at max
		 * depth, leaving it in the window is pointless.  we
		 * should evict it first.
		 */
		যদি (DELTA(entry) && max_depth <= n->depth)
			অনুবর্ত;

		/*
		 * Move the best delta base up in the window, after the
		 * currently deltified object, to keep it longer.  It will
		 * be the first base object to be attempted next.
		 */
		যদি (DELTA(entry)) {
			সংরচনা unpacked swap = array[best_base];
			পূর্ণাঙ্ক dist = (window + idx - best_base) % window;
			পূর্ণাঙ্ক dst = best_base;
			যাবত্ (dist--) {
				পূর্ণাঙ্ক src = (dst + 1) % window;
				array[dst] = array[src];
				dst = src;
			}
			array[dst] = swap;
		}

		next:
		idx++;
		যদি (গুনতি + 1 < window)
			গুনতি++;
		যদি (idx >= window)
			idx = 0;
	}

	প্রতি (i = 0; i < window; ++i) {
		free_delta_index(array[i].index);
		মুক্ত(array[i].data);
	}
	মুক্ত(array);
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

সংরচনা thread_params {
	pthread_t thread;
	সংরচনা object_entry **list;
	সংরচনা packing_region *regions;
	unsigned list_size;
	unsigned remaining;
	পূর্ণাঙ্ক window;
	পূর্ণাঙ্ক depth;
	পূর্ণাঙ্ক working;
	পূর্ণাঙ্ক data_ready;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	unsigned *processed;
};

স্থির pthread_cond_t progress_cond;

/*
 * Mutex and conditional variable can't be statically-initialized on Windows.
 */
স্থির শূন্য init_threaded_অনুসন্ধান(শূন্য)
{
	pthread_mutex_init(&cache_mutex, রিক্ত);
	pthread_mutex_init(&progress_mutex, রিক্ত);
	pthread_cond_init(&progress_cond, রিক্ত);
}

স্থির শূন্য cleanup_threaded_অনুসন্ধান(শূন্য)
{
	pthread_cond_destroy(&progress_cond);
	pthread_mutex_destroy(&cache_mutex);
	pthread_mutex_destroy(&progress_mutex);
}

স্থির শূন্য *threaded_find_deltas(শূন্য *arg)
{
	সংরচনা thread_params *me = arg;

	progress_lock();
	যাবত্ (me->remaining) {
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
		যাবত্ (!me->data_ready)
			pthread_cond_wait(&me->cond, &me->mutex);
		me->data_ready = 0;
		pthread_mutex_unlock(&me->mutex);

		progress_lock();
	}
	progress_unlock();
	/* leave ->working 1 so that this doesn't get more work assigned */
	প্রত্যাগম রিক্ত;
}

স্থির শূন্য ll_find_deltas(সংরচনা object_entry **list, unsigned list_size,
			   পূর্ণাঙ্ক window, পূর্ণাঙ্ক depth, unsigned *processed)
{
	সংরচনা thread_params *p;
	পূর্ণাঙ্ক i, ret, active_threads = 0;

	init_threaded_অনুসন্ধান();

	যদি (delta_অনুসন্ধান_threads <= 1) {
		find_deltas(list, &list_size, window, depth, processed);
		cleanup_threaded_অনুসন্ধান();
		প্রত্যাগম;
	}
	যদি (progress > pack_to_stdout)
		fprintf_ln(stderr, _("Delta compression using up to %d threads"),
			   delta_অনুসন্ধান_threads);
	CALLOC_ARRAY(p, delta_অনুসন্ধান_threads);

	/* Partition the work amongst work threads. */
	প্রতি (i = 0; i < delta_অনুসন্ধান_threads; i++) {
		unsigned sub_size = list_size / (delta_অনুসন্ধান_threads - i);

		/* don't use too small segments or no deltas will be found */
		যদি (sub_size < 2*window && i+1 < delta_অনুসন্ধান_threads)
			sub_size = 0;

		p[i].window = window;
		p[i].depth = depth;
		p[i].processed = processed;
		p[i].working = 1;
		p[i].data_ready = 0;

		/* try to split chunks on "path" boundaries */
		যাবত্ (sub_size && sub_size < list_size &&
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
	প্রতি (i = 0; i < delta_অনুসন্ধান_threads; i++) {
		যদি (!p[i].list_size)
			অনুবর্ত;
		pthread_mutex_init(&p[i].mutex, রিক্ত);
		pthread_cond_init(&p[i].cond, রিক্ত);
		ret = pthread_create(&p[i].thread, রিক্ত,
				     threaded_find_deltas, &p[i]);
		যদি (ret)
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
	যাবত্ (active_threads) {
		সংরচনা thread_params *target = রিক্ত;
		সংরচনা thread_params *victim = রিক্ত;
		unsigned sub_size = 0;

		progress_lock();
		প্রতি (;;) {
			প্রতি (i = 0; !target && i < delta_অনুসন্ধান_threads; i++)
				যদি (!p[i].working)
					target = &p[i];
			যদি (target)
				ভঙ্গ;
			pthread_cond_wait(&progress_cond, &progress_mutex);
		}

		প্রতি (i = 0; i < delta_অনুসন্ধান_threads; i++)
			যদি (p[i].remaining > 2*window &&
			    (!victim || victim->remaining < p[i].remaining))
				victim = &p[i];
		যদি (victim) {
			sub_size = victim->remaining / 2;
			list = victim->list + victim->list_size - sub_size;
			যাবত্ (sub_size && list[0]->hash &&
			       list[0]->hash == list[-1]->hash) {
				list++;
				sub_size--;
			}
			যদি (!sub_size) {
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

		যদি (!sub_size) {
			pthread_join(target->thread, রিক্ত);
			pthread_cond_destroy(&target->cond);
			pthread_mutex_destroy(&target->mutex);
			active_threads--;
		}
	}
	cleanup_threaded_অনুসন্ধান();
	মুক্ত(p);
}

স্থির পূর্ণাঙ্ক obj_is_packed(ধ্রুব সংরচনা object_id *oid)
{
	প্রত্যাগম packlist_find(&to_pack, oid) ||
		(reuse_packfile_bitmap &&
		 bitmap_walk_contains(bitmap_git, reuse_packfile_bitmap, oid));
}

স্থির শূন্য add_tag_chain(ধ্রুব সংরচনা object_id *oid)
{
	সংরচনা tag *tag;

	/*
	 * We catch duplicates already in add_object_entry(), but we'd
	 * prefer to do this extra check to avoid having to parse the
	 * tag at all if we already know that it's being packed (e.g., if
	 * it was included via bitmaps, we would not have parsed it
	 * previously).
	 */
	যদি (obj_is_packed(oid))
		প্রত্যাগম;

	tag = lookup_tag(the_repository, oid);
	যাবত্ (1) {
		যদি (!tag || parse_tag(the_repository, tag) || !tag->tagged)
			die(_("unable to pack objects reachable from tag %s"),
			    oid_to_hex(oid));

		add_object_entry(&tag->object.oid, OBJ_TAG, রিক্ত, 0);

		যদি (tag->tagged->type != OBJ_TAG)
			প্রত্যাগম;

		tag = (সংরচনা tag *)tag->tagged;
	}
}

স্থির পূর্ণাঙ্ক add_ref_tag(ধ্রুব সংরচনা reference *ref, শূন্য *cb_data UNUSED)
{
	সংরচনা object_id peeled;

	যদি (!reference_get_peeled_oid(the_repository, ref, &peeled) &&
	    obj_is_packed(&peeled))
		add_tag_chain(ref->oid);
	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক should_attempt_deltas(সংরচনা object_entry *entry)
{
	যদি (DELTA(entry))
		/* This happens if we decided to reuse existing
		 * delta from a pack. "reuse_delta &&" is implied.
		 */
		প্রত্যাগম 0;

	যদি (!entry->type_valid ||
	    oe_size_less_than(&to_pack, entry, 50))
		প্রত্যাগম 0;

	যদি (entry->no_try_delta)
		প্রত্যাগম 0;

	যদি (!entry->preferred_base) {
		যদি (oe_type(entry) < 0)
			die(_("unable to get type of object %s"),
				oid_to_hex(&entry->idx.oid));
	} অন্যথা যদি (oe_type(entry) < 0) {
		/*
		 * This object is not found, but we
		 * don't have to include it anyway.
		 */
		প্রত্যাগম 0;
	}

	প্রত্যাগম 1;
}

স্থির শূন্য find_deltas_for_region(সংরচনা object_entry *list,
				   সংরচনা packing_region *region,
				   unsigned পূর্ণাঙ্ক *processed)
{
	সংরচনা object_entry **delta_list;
	unsigned পূর্ণাঙ্ক delta_list_nr = 0;

	ALLOC_ARRAY(delta_list, region->nr);
	প্রতি (size_t i = 0; i < region->nr; i++) {
		সংরচনা object_entry *entry = list + region->start + i;
		যদি (should_attempt_deltas(entry))
			delta_list[delta_list_nr++] = entry;
	}

	QSORT(delta_list, delta_list_nr, type_size_sort);
	find_deltas(delta_list, &delta_list_nr, window, depth, processed);
	মুক্ত(delta_list);
}

স্থির শূন্য find_deltas_by_region(সংরচনা object_entry *list,
				  সংরচনা packing_region *regions,
				  size_t start, size_t nr)
{
	unsigned পূর্ণাঙ্ক processed = 0;
	size_t progress_nr;

	যদি (!nr)
		প্রত্যাগম;

	progress_nr = regions[nr - 1].start + regions[nr - 1].nr;

	যদি (progress)
		progress_state = start_progress(the_repository,
						_("Compressing objects by path"),
						progress_nr);

	যাবত্ (nr--)
		find_deltas_for_region(list,
				       &regions[start++],
				       &processed);

	display_progress(progress_state, progress_nr);
	stop_progress(&progress_state);
}

স্থির শূন্য *threaded_find_deltas_by_path(শূন্য *arg)
{
	সংরচনা thread_params *me = arg;

	progress_lock();
	যাবত্ (me->remaining) {
		যাবত্ (me->remaining) {
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
		যাবত্ (!me->data_ready)
			pthread_cond_wait(&me->cond, &me->mutex);
		me->data_ready = 0;
		pthread_mutex_unlock(&me->mutex);

		progress_lock();
	}
	progress_unlock();
	/* leave ->working 1 so that this doesn't get more work assigned */
	প্রত্যাগম রিক্ত;
}

স্থির শূন্য ll_find_deltas_by_region(সংরচনা object_entry *list,
				     সংরচনা packing_region *regions,
				     uint32_t start, uint32_t nr)
{
	সংরচনা thread_params *p;
	পূর্ণাঙ্ক i, ret, active_threads = 0;
	unsigned পূর্ণাঙ্ক processed = 0;
	uint32_t progress_nr;
	init_threaded_অনুসন্ধান();

	যদি (!nr)
		প্রত্যাগম;

	progress_nr =  regions[nr - 1].start + regions[nr - 1].nr;
	যদি (delta_অনুসন্ধান_threads <= 1) {
		find_deltas_by_region(list, regions, start, nr);
		cleanup_threaded_অনুসন্ধান();
		প্রত্যাগম;
	}

	যদি (progress > pack_to_stdout)
		fprintf_ln(stderr,
			   Q_("Path-based delta compression using up to %d thread",
			      "Path-based delta compression using up to %d threads",
			      delta_অনুসন্ধান_threads),
			   delta_অনুসন্ধান_threads);
	CALLOC_ARRAY(p, delta_অনুসন্ধান_threads);

	যদি (progress)
		progress_state = start_progress(the_repository,
						_("Compressing objects by path"),
						progress_nr);
	/* Partition the work amongst work threads. */
	প্রতি (i = 0; i < delta_অনুসন্ধান_threads; i++) {
		unsigned sub_size = nr / (delta_অনুসন্ধান_threads - i);

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
	প্রতি (i = 0; i < delta_অনুসন্ধান_threads; i++) {
		যদি (!p[i].list_size)
			অনুবর্ত;
		pthread_mutex_init(&p[i].mutex, রিক্ত);
		pthread_cond_init(&p[i].cond, রিক্ত);
		ret = pthread_create(&p[i].thread, রিক্ত,
				     threaded_find_deltas_by_path, &p[i]);
		যদি (ret)
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
	যাবত্ (active_threads) {
		সংরচনা thread_params *target = রিক্ত;
		সংরচনা thread_params *victim = রিক্ত;
		unsigned sub_size = 0;

		progress_lock();
		প্রতি (;;) {
			প্রতি (i = 0; !target && i < delta_অনুসন্ধান_threads; i++)
				যদি (!p[i].working)
					target = &p[i];
			যদি (target)
				ভঙ্গ;
			pthread_cond_wait(&progress_cond, &progress_mutex);
		}

		প্রতি (i = 0; i < delta_অনুসন্ধান_threads; i++)
			যদি (p[i].remaining > 2*window &&
			    (!victim || victim->remaining < p[i].remaining))
				victim = &p[i];
		যদি (victim) {
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

		যদি (!sub_size) {
			pthread_join(target->thread, রিক্ত);
			pthread_cond_destroy(&target->cond);
			pthread_mutex_destroy(&target->mutex);
			active_threads--;
		}
	}
	cleanup_threaded_অনুসন্ধান();
	মুক্ত(p);

	display_progress(progress_state, progress_nr);
	stop_progress(&progress_state);
}

স্থির শূন্য prepare_pack(পূর্ণাঙ্ক window, পূর্ণাঙ্ক depth)
{
	সংরচনা object_entry **delta_list;
	uint32_t i, nr_deltas;
	unsigned n;

	যদি (use_delta_islands)
		resolve_বৃক্ষ_islands(the_repository, progress, &to_pack);

	get_object_details();

	/*
	 * If we're locally repacking then we need to be doubly careful
	 * from now on in order to make sure no stealth corruption gets
	 * propagated to the new pack.  Clients receiving streamed packs
	 * should validate everything they get anyway so no need to incur
	 * the additional cost here in that case.
	 */
	যদি (!pack_to_stdout)
		do_check_packed_object_crc = 1;

	যদি (!to_pack.nr_objects || !window || !depth)
		প্রত্যাগম;

	যদি (path_walk)
		ll_find_deltas_by_region(to_pack.objects, to_pack.regions,
					 0, to_pack.nr_regions);

	ALLOC_ARRAY(delta_list, to_pack.nr_objects);
	nr_deltas = n = 0;

	প্রতি (i = 0; i < to_pack.nr_objects; i++) {
		সংরচনা object_entry *entry = to_pack.objects + i;

		যদি (!should_attempt_deltas(entry))
			অনুবর্ত;

		যদি (!entry->preferred_base)
			nr_deltas++;

		delta_list[n++] = entry;
	}

	যদি (nr_deltas && n > 1) {
		unsigned nr_done = 0;

		যদি (progress)
			progress_state = start_progress(the_repository,
							_("Compressing objects"),
							nr_deltas);
		QSORT(delta_list, n, type_size_sort);
		ll_find_deltas(delta_list, n, window+1, depth, &nr_done);
		stop_progress(&progress_state);
		যদি (nr_done != nr_deltas)
			die(_("inconsistency with delta count"));
	}
	মুক্ত(delta_list);
}

স্থির পূর্ণাঙ্ক git_pack_config(ধ্রুব বর্ণ *k, ধ্রুব বর্ণ *v,
			   ধ্রুব সংরচনা config_context *ctx, শূন্য *cb)
{
	যদি (!strcmp(k, "pack.window")) {
		window = git_config_int(k, v, ctx->kvi);
		প্রত্যাগম 0;
	}
	যদি (!strcmp(k, "pack.windowmemory")) {
		window_memory_limit = git_config_ulong(k, v, ctx->kvi);
		প্রত্যাগম 0;
	}
	যদি (!strcmp(k, "pack.depth")) {
		depth = git_config_int(k, v, ctx->kvi);
		প্রত্যাগম 0;
	}
	যদি (!strcmp(k, "pack.deltacachesize")) {
		max_delta_cache_size = git_config_size_t(k, v, ctx->kvi);
		প্রত্যাগম 0;
	}
	যদি (!strcmp(k, "pack.deltacachelimit")) {
		cache_max_small_delta_size = git_config_int(k, v, ctx->kvi);
		প্রত্যাগম 0;
	}
	যদি (!strcmp(k, "pack.writebitmaphashcache")) {
		যদি (git_config_bool(k, v))
			write_bitmap_options |= BITMAP_OPT_HASH_CACHE;
		অন্যথা
			write_bitmap_options &= ~BITMAP_OPT_HASH_CACHE;
	}

	যদি (!strcmp(k, "pack.writebitmaplookuptable")) {
		যদি (git_config_bool(k, v))
			write_bitmap_options |= BITMAP_OPT_LOOKUP_TABLE;
		অন্যথা
			write_bitmap_options &= ~BITMAP_OPT_LOOKUP_TABLE;
	}

	যদি (!strcmp(k, "pack.usebitmaps")) {
		use_bitmap_index_default = git_config_bool(k, v);
		প্রত্যাগম 0;
	}
	যদি (!strcmp(k, "pack.allowpackreuse")) {
		পূর্ণাঙ্ক res = git_parse_maybe_bool_text(v);
		যদি (res < 0) {
			যদি (!strcasecmp(v, "single"))
				allow_pack_reuse = SINGLE_PACK_REUSE;
			অন্যথা যদি (!strcasecmp(v, "multi"))
				allow_pack_reuse = MULTI_PACK_REUSE;
			অন্যথা
				die(_("invalid pack.allowPackReuse value: '%s'"), v);
		} অন্যথা যদি (res) {
			allow_pack_reuse = SINGLE_PACK_REUSE;
		} অন্যথা {
			allow_pack_reuse = NO_PACK_REUSE;
		}
		প্রত্যাগম 0;
	}
	যদি (!strcmp(k, "pack.threads")) {
		delta_অনুসন্ধান_threads = git_config_int(k, v, ctx->kvi);
		যদি (delta_অনুসন্ধান_threads < 0)
			die(_("invalid number of threads specified (%d)"),
			    delta_অনুসন্ধান_threads);
		যদি (!HAVE_THREADS && delta_অনুসন্ধান_threads != 1) {
			warning(_("no threads support, ignoring %s"), k);
			delta_অনুসন্ধান_threads = 0;
		}
		প্রত্যাগম 0;
	}
	যদি (!strcmp(k, "pack.indexversion")) {
		pack_idx_opts.version = git_config_int(k, v, ctx->kvi);
		যদি (pack_idx_opts.version > 2)
			die(_("bad pack.indexVersion=%"PRIu32),
			    pack_idx_opts.version);
		প্রত্যাগম 0;
	}
	যদি (!strcmp(k, "pack.writereverseindex")) {
		যদি (git_config_bool(k, v))
			pack_idx_opts.flags |= WRITE_REV;
		অন্যথা
			pack_idx_opts.flags &= ~WRITE_REV;
		প্রত্যাগম 0;
	}
	যদি (!strcmp(k, "uploadpack.blobpackfileuri")) {
		সংরচনা configured_exclusion *ex;
		ধ্রুব বর্ণ *oid_end, *pack_end;
		/*
		 * Stores the pack hash. This is not a true object ID, but is
		 * of the same form.
		 */
		সংরচনা object_id pack_hash;

		যদি (!v)
			প্রত্যাগম config_error_nonbool(k);

		ex = xmalloc(পরিমাণ(*ex));
		যদি (parse_oid_hex(v, &ex->e.oid, &oid_end) ||
		    *oid_end != ' ' ||
		    parse_oid_hex(oid_end + 1, &pack_hash, &pack_end) ||
		    *pack_end != ' ')
			die(_("value of uploadpack.blobpackfileuri must be "
			      "of the form '<object-hash> <pack-hash> <uri>' (got '%s')"), v);
		যদি (oidmap_get(&configured_exclusions, &ex->e.oid))
			die(_("object already configured in another "
			      "uploadpack.blobpackfileuri (got '%s')"), v);
		ex->pack_hash_hex = xcalloc(1, pack_end - oid_end);
		memcpy(ex->pack_hash_hex, oid_end + 1, pack_end - oid_end - 1);
		ex->uri = xstrdup(pack_end + 1);
		oidmap_put(&configured_exclusions, ex);
	}
	প্রত্যাগম git_default_config(k, v, ctx, cb);
}

/* Counters for trace2 output when in --stdin-packs mode. */
স্থির পূর্ণাঙ্ক stdin_packs_found_nr;
স্থির পূর্ণাঙ্ক stdin_packs_hints_nr;

স্থির পূর্ণাঙ্ক add_object_entry_from_pack(ধ্রুব সংরচনা object_id *oid,
				      সংরচনা packed_git *p,
				      uint32_t pos,
				      শূন্য *_data)
{
	off_t ofs;
	সংরচনা object_info oi = OBJECT_INFO_INIT;
	গণনা object_type type = OBJ_NONE;

	display_progress(progress_state, ++nr_seen);

	যদি (have_duplicate_entry(oid, 0))
		প্রত্যাগম 0;

	stdin_packs_found_nr++;

	ofs = nth_packed_object_offset(p, pos);

	oi.typep = &type;
	যদি (packed_object_info(রিক্ত, p, ofs, &oi) < 0) {
		die(_("could not get type of object %s in pack %s"),
		    oid_to_hex(oid), p->pack_name);
	} অন্যথা যদি (type == OBJ_COMMIT) {
		সংরচনা rev_info *revs = _data;
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
		add_pending_oid(revs, রিক্ত, oid, 0);
	}

	যদি (!want_object_in_pack(oid, 0, &p, &ofs))
		প্রত্যাগম 0;

	create_object_entry(oid, type, 0, 0, 0, p, ofs);

	প্রত্যাগম 0;
}

স্থির শূন্য show_object_pack_hint(সংরচনা object *object, ধ্রুব বর্ণ *name,
				  শূন্য *data)
{
	গণনা stdin_packs_mode mode = *(গণনা stdin_packs_mode *)data;
	যদি (mode == STDIN_PACKS_MODE_FOLLOW) {
		যদি (object->type == OBJ_BLOB &&
		    !odb_has_object(the_repository->objects, &object->oid, 0))
			প্রত্যাগম;
		add_object_entry(&object->oid, object->type, name, 0);
	} অন্যথা {
		সংরচনা object_entry *oe = packlist_find(&to_pack, &object->oid);
		যদি (!oe)
			প্রত্যাগম;

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

স্থির শূন্য show_commit_pack_hint(সংরচনা commit *commit, শূন্য *data)
{
	গণনা stdin_packs_mode mode = *(গণনা stdin_packs_mode *)data;

	যদি (mode == STDIN_PACKS_MODE_FOLLOW) {
		show_object_pack_hint((সংরচনা object *)commit, "", data);
		প্রত্যাগম;
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
গণনা stdin_pack_info_kind {
	STDIN_PACK_INCLUDE = (1<<0),
	STDIN_PACK_EXCLUDE_CLOSED = (1<<1),
	STDIN_PACK_EXCLUDE_OPEN = (1<<2),
};

সংরচনা stdin_pack_info {
	সংরচনা packed_git *p;
	গণনা stdin_pack_info_kind kind;
};

স্থির পূর্ণাঙ্ক pack_mtime_cmp(ধ্রুব শূন্য *_a, ধ্রুব শূন্য *_b)
{
	সংরচনা stdin_pack_info *a = ((ধ্রুব সংরচনা string_list_item*)_a)->util;
	সংরচনা stdin_pack_info *b = ((ধ্রুব সংরচনা string_list_item*)_b)->util;

	/*
	 * order packs by descending mtime so that objects are laid out
	 * roughly as newest-to-oldest
	 */
	যদি (a->p->mtime < b->p->mtime)
		প্রত্যাগম 1;
	অন্যথা যদি (b->p->mtime < a->p->mtime)
		প্রত্যাগম -1;
	অন্যথা
		প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক stdin_packs_include_check_obj(সংরচনা object *obj, শূন্য *data UNUSED)
{
	প্রত্যাগম !has_object_kept_pack(to_pack.repo, &obj->oid,
				     KEPT_PACK_IN_CORE);
}

স্থির পূর্ণাঙ্ক stdin_packs_include_check(সংরচনা commit *commit, শূন্য *data)
{
	প্রত্যাগম stdin_packs_include_check_obj((সংরচনা object *)commit, data);
}

স্থির শূন্য stdin_packs_add_pack_entries(সংরচনা strmap *packs,
					 সংরচনা rev_info *revs)
{
	সংরচনা string_list চাবিগুলি = STRING_LIST_INIT_NODUP;
	সংরচনা string_list_item *item;
	সংরচনা hashmap_iter iter;
	সংরচনা strmap_entry *entry;

	strmap_for_each_entry(packs, &iter, entry) {
		সংরচনা stdin_pack_info *info = entry->মান;
		যদি (!info->p)
			die(_("could not find pack '%s'"), entry->চাবি);

		string_list_append(&চাবিগুলি, entry->চাবি)->util = info;
	}

	/*
	 * Order packs by ascending mtime; use QSORT directly to access the
	 * string_list_item's ->util pointer, which string_list_sort() does not
	 * provide.
	 */
	QSORT(চাবিগুলি.বস্তুসমূহ, চাবিগুলি.nr, pack_mtime_cmp);

	for_each_string_list_item(item, &চাবিগুলি) {
		সংরচনা stdin_pack_info *info = item->util;

		যদি (info->kind & STDIN_PACK_EXCLUDE_OPEN) {
			/*
			 * When open-excluded packs ("!") are present, stop
			 * the parent walk at closed-excluded ("^") packs.
			 * Objects behind a "^" boundary are guaranteed to
			 * have closure and should not be rescued.
			 */
			revs->include_check = stdin_packs_include_check;
			revs->include_check_obj = stdin_packs_include_check_obj;
		}

		যদি ((info->kind & STDIN_PACK_INCLUDE) ||
		    (info->kind & STDIN_PACK_EXCLUDE_OPEN))
			for_each_object_in_pack(info->p,
						add_object_entry_from_pack,
						revs,
						ODB_FOR_EACH_OBJECT_PACK_ORDER);
	}

	string_list_clear(&চাবিগুলি, 0);
}

স্থির শূন্য stdin_packs_read_প্রবেশ(সংরচনা rev_info *revs,
				   গণনা stdin_packs_mode mode)
{
	সংরচনা strbuf buf = STRBUF_INIT;
	সংরচনা strmap packs = STRMAP_INIT;
	সংরচনা packed_git *p;

	যাবত্ (strbuf_getline(&buf, stdin) != EOF) {
		সংরচনা stdin_pack_info *info;
		গণনা stdin_pack_info_kind kind = STDIN_PACK_INCLUDE;
		ধ্রুব বর্ণ *চাবি = buf.buf;

		যদি (!*চাবি)
			অনুবর্ত;
		অন্যথা যদি (*চাবি == '^')
			kind = STDIN_PACK_EXCLUDE_CLOSED;
		অন্যথা যদি (*চাবি == '!' && mode == STDIN_PACKS_MODE_FOLLOW)
			kind = STDIN_PACK_EXCLUDE_OPEN;

		যদি (kind != STDIN_PACK_INCLUDE)
			চাবি++;

		info = strmap_get(&packs, চাবি);
		যদি (!info) {
			CALLOC_ARRAY(info, 1);
			strmap_put(&packs, চাবি, info);
		}

		info->kind |= kind;

		strbuf_reset(&buf);
	}

	repo_for_each_pack(the_repository, p) {
		সংরচনা stdin_pack_info *info;

		info = strmap_get(&packs, pack_basename(p));
		যদি (!info)
			অনুবর্ত;

		যদি (info->kind & STDIN_PACK_INCLUDE) {
			যদি (exclude_promisor_objects && p->pack_promisor)
				die(_("packfile %s is a promisor but --exclude-promisor-objects was given"), p->pack_name);

			/*
			 * Arguments we got on stdin may not even be
			 * packs. First check that to avoid segfaulting
			 * later on in e.g.  pack_mtime_cmp(), excluded
			 * packs are handled below.
			 */
			যদি (!is_pack_valid(p))
				die(_("packfile %s cannot be accessed"), p->pack_name);
		}

		যদি (info->kind & STDIN_PACK_EXCLUDE_CLOSED) {
			/*
			 * Marking excluded packs as kept in-core so
			 * that later calls to add_object_entry()
			 * discards any objects that are also found in
			 * excluded packs.
			 */
			p->pack_keep_in_core = 1;
		}

		যদি (info->kind & STDIN_PACK_EXCLUDE_OPEN) {
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

স্থির শূন্য add_unreachable_loose_objects(সংরচনা rev_info *revs);

স্থির শূন্য read_stdin_packs(সংরচনা repository *repo,
			     গণনা stdin_packs_mode mode, পূর্ণাঙ্ক rev_list_unpacked)
{
	পূর্ণাঙ্ক prev_fetch_if_missing = repo->fetch_if_missing;
	সংরচনা rev_info revs;

	/*
	 * The revision walk may hit objects that are promised, only. As the
	 * walk is best-effort though we don't want to perform backfill fetches
	 * for them.
	 */
	repo->fetch_if_missing = 0;

	repo_init_revisions(repo, &revs, রিক্ত);
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
	revs.বৃক্ষ_objects = 1;
	revs.tag_objects = 1;
	revs.ignore_missing_links = 1;
	revs.exclude_promisor_objects = exclude_promisor_objects;

	/* avoids adding objects in excluded packs */
	ignore_packed_keep_in_core = 1;
	যদি (mode == STDIN_PACKS_MODE_FOLLOW) {
		/*
		 * In '--stdin-packs=follow' mode, additionally ignore
		 * objects in excluded-open packs to prevent them from
		 * appearing in the resulting pack.
		 */
		ignore_packed_keep_in_core_open = 1;
	}
	stdin_packs_read_প্রবেশ(&revs, mode);
	যদি (rev_list_unpacked)
		add_unreachable_loose_objects(&revs);

	যদি (prepare_revision_walk(&revs))
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

স্থির শূন্য add_cruft_object_entry(ধ্রুব সংরচনা object_id *oid, গণনা object_type type,
				   সংরচনা packed_git *pack, off_t offset,
				   ধ্রুব বর্ণ *name, uint32_t mtime)
{
	সংরচনা object_entry *entry;

	display_progress(progress_state, ++nr_seen);

	entry = packlist_find(&to_pack, oid);
	যদি (entry) {
		যদি (name) {
			entry->hash = pack_name_hash_fn(name);
			entry->no_try_delta = no_try_delta(name);
		}
	} অন্যথা {
		যদি (!want_object_in_pack_mtime(oid, 0, &pack, &offset, mtime))
			প্রত্যাগম;
		যদি (!pack && type == OBJ_BLOB) {
			সংরচনা odb_source_files *files =
				odb_source_files_downcast(the_repository->objects->source);
			পূর্ণাঙ্ক found = 0;

			প্রতি (সংরচনা odb_files_dir *dir = files->dirs; !found && dir; dir = dir->next)
				যদি (!odb_source_read_object_info(&dir->loose->base, oid, রিক্ত, 0, রিক্ত))
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
			যদি (!found)
				প্রত্যাগম;
		}

		entry = create_object_entry(oid, type, pack_name_hash_fn(name),
					    0, name && no_try_delta(name),
					    pack, offset);
	}

	যদি (mtime > oe_cruft_mtime(&to_pack, entry))
		oe_set_cruft_mtime(&to_pack, entry, mtime);
	প্রত্যাগম;
}

স্থির শূন্য show_cruft_object(সংরচনা object *obj, ধ্রুব বর্ণ *name, শূন্য *data UNUSED)
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
	add_cruft_object_entry(&obj->oid, obj->type, রিক্ত, 0, name, cruft_expiration);
}

স্থির শূন্য show_cruft_commit(সংরচনা commit *commit, শূন্য *data)
{
	show_cruft_object((সংরচনা object*)commit, রিক্ত, data);
}

স্থির পূর্ণাঙ্ক cruft_include_check_obj(সংরচনা object *obj, শূন্য *data UNUSED)
{
	প্রত্যাগম !has_object_kept_pack(to_pack.repo, &obj->oid, KEPT_PACK_IN_CORE);
}

স্থির পূর্ণাঙ্ক cruft_include_check(সংরচনা commit *commit, শূন্য *data)
{
	প্রত্যাগম cruft_include_check_obj((সংরচনা object*)commit, data);
}

স্থির শূন্য set_cruft_mtime(ধ্রুব সংরচনা object *object,
			    সংরচনা packed_git *pack,
			    off_t offset, time_t mtime)
{
	add_cruft_object_entry(&object->oid, object->type, pack, offset, রিক্ত,
			       mtime);
}

স্থির শূন্য mark_pack_kept_in_core(সংরচনা string_list *packs, unsigned keep)
{
	সংরচনা string_list_item *item = রিক্ত;
	for_each_string_list_item(item, packs) {
		সংরচনা packed_git *p = item->util;
		যদি (!p)
			die(_("could not find pack '%s'"), item->string);
		যদি (p->is_cruft && keep)
			ignore_packed_keep_in_core_has_cruft = 1;
		p->pack_keep_in_core = keep;
	}
}

স্থির শূন্য add_objects_in_unpacked_packs(শূন্য);

স্থির শূন্য enumerate_cruft_objects(শূন্য)
{
	যদি (progress)
		progress_state = start_progress(the_repository,
						_("Enumerating cruft objects"), 0);

	add_objects_in_unpacked_packs();
	add_unreachable_loose_objects(রিক্ত);

	stop_progress(&progress_state);
}

স্থির শূন্য enumerate_and_traverse_cruft_objects(সংরচনা string_list *fresh_packs)
{
	সংরচনা packed_git *p;
	সংরচনা rev_info revs;
	পূর্ণাঙ্ক ret;

	repo_init_revisions(the_repository, &revs, রিক্ত);

	revs.tag_objects = 1;
	revs.বৃক্ষ_objects = 1;
	revs.blob_objects = 1;

	revs.include_check = cruft_include_check;
	revs.include_check_obj = cruft_include_check_obj;

	revs.ignore_missing_links = 1;

	যদি (progress)
		progress_state = start_progress(the_repository,
						_("Enumerating cruft objects"), 0);
	ret = add_unseen_recent_objects_to_traversal(&revs, cruft_expiration,
						     set_cruft_mtime, 1);
	stop_progress(&progress_state);

	যদি (ret)
		die(_("unable to add cruft objects"));

	/*
	 * Re-mark only the fresh packs as kept so that objects in
	 * unknown packs do not halt the reachability traversal early.
	 */
	repo_for_each_pack(the_repository, p)
		p->pack_keep_in_core = 0;
	mark_pack_kept_in_core(fresh_packs, 1);

	যদি (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	যদি (progress)
		progress_state = start_progress(the_repository,
						_("Traversing cruft objects"), 0);
	nr_seen = 0;
	traverse_commit_list(&revs, show_cruft_commit, show_cruft_object, রিক্ত);

	stop_progress(&progress_state);
	release_revisions(&revs);
}

স্থির শূন্য read_cruft_objects(শূন্য)
{
	সংরচনা strbuf buf = STRBUF_INIT;
	সংরচনা string_list discard_packs = STRING_LIST_INIT_DUP;
	সংরচনা string_list fresh_packs = STRING_LIST_INIT_DUP;
	সংরচনা packed_git *p;

	ignore_packed_keep_in_core = 1;

	যাবত্ (strbuf_getline(&buf, stdin) != EOF) {
		যদি (!buf.len)
			অনুবর্ত;

		যদি (*buf.buf == '-')
			string_list_append(&discard_packs, buf.buf + 1);
		অন্যথা
			string_list_append(&fresh_packs, buf.buf);
	}

	string_list_sort(&discard_packs);
	string_list_sort(&fresh_packs);

	repo_for_each_pack(the_repository, p) {
		ধ্রুব বর্ণ *pack_name = pack_basename(p);
		সংরচনা string_list_item *item;

		item = string_list_lookup(&fresh_packs, pack_name);
		যদি (!item)
			item = string_list_lookup(&discard_packs, pack_name);

		যদি (item) {
			item->util = p;
		} অন্যথা {
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

	যদি (cruft_expiration)
		enumerate_and_traverse_cruft_objects(&fresh_packs);
	অন্যথা
		enumerate_cruft_objects();

	strbuf_release(&buf);
	string_list_clear(&discard_packs, 0);
	string_list_clear(&fresh_packs, 0);
}

স্থির শূন্য read_object_list_from_stdin(শূন্য)
{
	বর্ণ সারি[GIT_MAX_HEXSZ + 1 + PATH_MAX + 2];
	সংরচনা object_id oid;
	ধ্রুব বর্ণ *p;

	প্রতি (;;) {
		যদি (!fgets(সারি, পরিমাণ(সারি), stdin)) {
			যদি (feof(stdin))
				ভঙ্গ;
			যদি (!ferror(stdin))
				BUG("fgets returned NULL, not EOF, not error!");
			যদি (errno != EINTR)
				die_errno("fgets");
			clearerr(stdin);
			অনুবর্ত;
		}
		যদি (সারি[0] == '-') {
			যদি (get_oid_hex(সারি+1, &oid))
				die(_("expected edge object ID, got garbage:\n %s"),
				    সারি);
			add_preferred_base(&oid);
			অনুবর্ত;
		}
		যদি (parse_oid_hex(সারি, &oid, &p))
			die(_("expected object ID, got garbage:\n %s"), সারি);

		add_preferred_base_object(p + 1);
		add_object_entry(&oid, OBJ_NONE, p + 1, 0);
	}
}

স্থির শূন্য show_commit(সংরচনা commit *commit, শূন্য *data UNUSED)
{
	add_object_entry(&commit->object.oid, OBJ_COMMIT, রিক্ত, 0);

	যদি (write_bitmap_index)
		index_commit_for_bitmap(commit);

	যদি (use_delta_islands)
		propagate_island_marks(the_repository, commit);
}

স্থির শূন্য show_object(সংরচনা object *obj, ধ্রুব বর্ণ *name,
			শূন্য *data UNUSED)
{
	add_preferred_base_object(name);
	add_object_entry(&obj->oid, obj->type, name, 0);

	যদি (use_delta_islands)
		record_বৃক্ষ_depth(&obj->oid, name);
}

স্থির শূন্য show_object__ma_allow_any(সংরচনা object *obj, ধ্রুব বর্ণ *name, শূন্য *data)
{
	প্রতিজ্ঞা(arg_missing_action == MA_ALLOW_ANY);

	/*
	 * Quietly ignore ALL missing objects.  This avoids problems with
	 * staging them now and getting an odd error later.
	 */
	যদি (!odb_has_object(the_repository->objects, &obj->oid, 0))
		প্রত্যাগম;

	show_object(obj, name, data);
}

স্থির শূন্য show_object__ma_allow_promisor(সংরচনা object *obj, ধ্রুব বর্ণ *name, শূন্য *data)
{
	প্রতিজ্ঞা(arg_missing_action == MA_ALLOW_PROMISOR);

	/*
	 * Quietly ignore EXPECTED missing objects.  This avoids problems with
	 * staging them now and getting an odd error later.
	 */
	যদি (!odb_has_object(the_repository->objects, &obj->oid, 0) &&
	    is_promisor_object(to_pack.repo, &obj->oid))
		প্রত্যাগম;

	show_object(obj, name, data);
}

স্থির পূর্ণাঙ্ক option_parse_missing_action(ধ্রুব সংরচনা option *opt,
				       ধ্রুব বর্ণ *arg, পূর্ণাঙ্ক unset)
{
	সংরচনা repository *repo = opt->মান;

	প্রতিজ্ঞা(arg);
	প্রতিজ্ঞা(!unset);

	যদি (!strcmp(arg, "error")) {
		arg_missing_action = MA_ERROR;
		fn_show_object = show_object;
		প্রত্যাগম 0;
	}

	যদি (!strcmp(arg, "allow-any")) {
		arg_missing_action = MA_ALLOW_ANY;
		repo->fetch_if_missing = 0;
		fn_show_object = show_object__ma_allow_any;
		প্রত্যাগম 0;
	}

	যদি (!strcmp(arg, "allow-promisor")) {
		arg_missing_action = MA_ALLOW_PROMISOR;
		repo->fetch_if_missing = 0;
		fn_show_object = show_object__ma_allow_promisor;
		প্রত্যাগম 0;
	}

	die(_("invalid value for '%s': '%s'"), "--missing", arg);
	প্রত্যাগম 0;
}

স্থির শূন্য show_edge(সংরচনা commit *commit)
{
	add_preferred_base(&commit->object.oid);
}

স্থির পূর্ণাঙ্ক add_object_in_unpacked_pack(ধ্রুব সংরচনা object_id *oid,
				       সংরচনা object_info *oi,
				       শূন্য *data UNUSED)
{
	যদি (cruft) {
		add_cruft_object_entry(oid, OBJ_NONE, oi->source_infop->u.packed.pack,
				       oi->source_infop->u.packed.offset, রিক্ত,
				       *oi->mtimep);
	} অন্যথা {
		add_object_entry(oid, OBJ_NONE, "", 0);
	}
	প্রত্যাগম 0;
}

স্থির শূন্য add_objects_in_unpacked_packs(শূন্য)
{
	সংরচনা odb_source_files *files =
		odb_source_files_downcast(to_pack.repo->objects->source);
	time_t mtime;
	সংরচনা odb_for_each_object_options opts = {
		.flags = ODB_FOR_EACH_OBJECT_PACK_ORDER |
			 ODB_FOR_EACH_OBJECT_LOCAL_ONLY |
			 ODB_FOR_EACH_OBJECT_SKIP_IN_CORE_KEPT_PACKS |
			 ODB_FOR_EACH_OBJECT_SKIP_ON_DISK_KEPT_PACKS,
	};
	সংরচনা odb_source_info source_info;
	সংরচনা object_info oi = {
		.mtimep = &mtime,
		.source_infop = &source_info,
	};

	প্রতি (সংরচনা odb_files_dir *dir = files->dirs; dir; dir = dir->next) {
		যদি (!dir->local)
			অনুবর্ত;

		যদি (odb_source_for_each_object(&dir->packed->base, &oi,
					       add_object_in_unpacked_pack, রিক্ত, &opts))
			die(_("cannot open pack index"));
	}
}

স্থির পূর্ণাঙ্ক add_loose_object(ধ্রুব সংরচনা object_id *oid, ধ্রুব বর্ণ *path,
			    শূন্য *data)
{
	সংরচনা rev_info *revs = data;
	গণনা object_type type = odb_read_object_info(the_repository->objects, oid, রিক্ত);

	যদি (type < 0) {
		warning(_("loose object at %s could not be examined"), path);
		প্রত্যাগম 0;
	}

	যদি (cruft) {
		সংরচনা stat st;
		যদি (stat(path, &st) < 0) {
			যদি (errno == ENOENT)
				প্রত্যাগম 0;
			প্রত্যাগম error_errno("unable to stat %s", oid_to_hex(oid));
		}

		add_cruft_object_entry(oid, type, রিক্ত, 0, রিক্ত,
				       st.st_mtime);
	} অন্যথা {
		add_object_entry(oid, type, "", 0);
	}

	যদি (revs && type == OBJ_COMMIT)
		add_pending_oid(revs, রিক্ত, oid, 0);

	প্রত্যাগম 0;
}

/*
 * We actually don't even have to worry about reachability here.
 * add_object_entry will weed out duplicates, so we just add every
 * loose object we find.
 */
স্থির শূন্য add_unreachable_loose_objects(সংরচনা rev_info *revs)
{
	for_each_loose_file_in_source(the_repository->objects->source,
				      add_loose_object, রিক্ত, রিক্ত, revs);
}

স্থির পূর্ণাঙ্ক has_sha1_pack_kept_or_nonlocal(ধ্রুব সংরচনা object_id *oid)
{
	স্থির সংরচনা packed_git *last_found = রিক্ত;
	সংরচনা packed_git *p;

	যদি (last_found && find_pack_entry_one(oid, last_found))
		প্রত্যাগম 1;

	repo_for_each_pack(the_repository, p) {
		/*
		 * We have already checked `last_found`, so there is no need to
		 * re-check here.
		 */
		যদি (p == last_found)
			অনুবর্ত;

		যদি ((!p->pack_local || p->pack_keep || p->pack_keep_in_core) &&
		    find_pack_entry_one(oid, p)) {
			last_found = p;
			প্রত্যাগম 1;
		}
	}

	প্রত্যাগম 0;
}

/*
 * Store a list of sha1s that are should not be discarded
 * because they are either written too recently, or are
 * reachable from another object that was.
 *
 * This is filled by get_object_list.
 */
স্থির সংরচনা oid_array recent_objects;

স্থির পূর্ণাঙ্ক loosened_object_can_be_discarded(ধ্রুব সংরচনা object_id *oid,
					    timestamp_t mtime)
{
	যদি (!unpack_unreachable_expiration)
		প্রত্যাগম 0;
	যদি (mtime > unpack_unreachable_expiration)
		প্রত্যাগম 0;
	যদি (oid_array_lookup(&recent_objects, oid) >= 0)
		প্রত্যাগম 0;
	প্রত্যাগম 1;
}

স্থির পূর্ণাঙ্ক force_object_loose(সংরচনা odb_source *source,
			      ধ্রুব সংরচনা object_id *oid,
			      ধ্রুব time_t *mtime)
{
	সংরচনা odb_source_files *files = odb_source_files_downcast(source);
	ধ্রুব সংরচনা git_hash_algo *compat = source->odb->repo->compat_hash_algo;
	সংরচনা object_info oi = OBJECT_INFO_INIT;
	সংরচনা object_id compat_oid, *compat_oid_p = রিক্ত;
	গণনা object_type type;
	শূন্য *buf = রিক্ত;
	size_t len;
	পূর্ণাঙ্ক ret;

	প্রতি (সংরচনা odb_files_dir *dir = files->dirs; dir; dir = dir->next)
		যদি (!odb_source_read_object_info(&dir->loose->base, oid, রিক্ত, 0, রিক্ত))
			প্রত্যাগম 0;

	oi.typep = &type;
	oi.sizep = &len;
	oi.contentp = &buf;
	যদি (odb_read_object_info_extended(source->odb, oid, &oi, 0)) {
		ret = error(_("cannot read object for %s"), oid_to_hex(oid));
		গচ্ছ out;
	}

	যদি (compat) {
		যদি (repo_oid_to_algop(source->odb->repo, oid, compat, &compat_oid)) {
			ret = error(_("cannot map object %s to %s"),
				    oid_to_hex(oid), compat->name);
			গচ্ছ out;
		}

		compat_oid_p = &compat_oid;
	}

	ret = odb_source_write_object(&files->dirs->loose->base, buf, len, type, oid,
				      compat_oid_p, mtime, 0);

out:
	মুক্ত(buf);
	প্রত্যাগম ret;
}

স্থির শূন্য loosen_unused_packed_objects(শূন্য)
{
	সংরচনা packed_git *p;
	uint32_t i;
	uint32_t loosened_objects_nr = 0;
	সংরচনা object_id oid;

	repo_for_each_pack(the_repository, p) {
		যদি (!p->pack_local || p->pack_keep || p->pack_keep_in_core)
			অনুবর্ত;

		যদি (open_pack_index(p))
			die(_("cannot open pack index"));

		প্রতি (i = 0; i < p->অঙ্ক_objects; i++) {
			nth_packed_object_id(&oid, p, i);
			যদি (!packlist_find(&to_pack, &oid) &&
			    !has_sha1_pack_kept_or_nonlocal(&oid) &&
			    !loosened_object_can_be_discarded(&oid, p->mtime)) {
				যদি (force_object_loose(the_repository->objects->source,
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
স্থির পূর্ণাঙ্ক pack_options_allow_reuse(শূন্য)
{
	প্রত্যাগম allow_pack_reuse != NO_PACK_REUSE &&
	       pack_to_stdout &&
	       !ignore_packed_keep_on_disk &&
	       !ignore_packed_keep_in_core &&
	       (!local || !have_non_local_packs) &&
	       !incremental;
}

স্থির পূর্ণাঙ্ক get_object_list_from_bitmap(সংরচনা rev_info *revs)
{
	যদি (!(bitmap_git = prepare_bitmap_walk(revs, 0)))
		প্রত্যাগম -1;

	/*
	 * For now, force the name-hash version to be 1 since that
	 * is the version implied by the bitmap format. Later, the
	 * format can include this version explicitly in its format,
	 * allowing readers to know the version that was used during
	 * the bitmap write.
	 */
	name_hash_version = 1;

	যদি (pack_options_allow_reuse())
		reuse_partial_packfile_from_bitmap(bitmap_git,
						   &reuse_packfiles,
						   &reuse_packfiles_nr,
						   &reuse_packfile_bitmap,
						   allow_pack_reuse == MULTI_PACK_REUSE);

	যদি (reuse_packfiles) {
		reuse_packfile_objects = bitmap_popcount(reuse_packfile_bitmap);
		যদি (!reuse_packfile_objects)
			BUG("expected non-empty reuse bitmap");

		nr_result += reuse_packfile_objects;
		nr_seen += reuse_packfile_objects;
		display_progress(progress_state, nr_seen);
	}

	traverse_bitmap_commit_list(bitmap_git, revs,
				    &add_object_entry_from_bitmap);
	প্রত্যাগম 0;
}

স্থির শূন্য record_recent_object(সংরচনা object *obj,
				 ধ্রুব বর্ণ *name UNUSED,
				 শূন্য *data UNUSED)
{
	oid_array_append(&recent_objects, &obj->oid);
}

স্থির শূন্য record_recent_commit(সংরচনা commit *commit, শূন্য *data UNUSED)
{
	oid_array_append(&recent_objects, &commit->object.oid);
}

স্থির পূর্ণাঙ্ক mark_bitmap_preferred_tip(ধ্রুব সংরচনা reference *ref, শূন্য *data UNUSED)
{
	ধ্রুব সংরচনা object_id *maybe_peeled = ref->oid;
	সংরচনা object_id peeled;
	সংরচনা object *object;

	যদি (!reference_get_peeled_oid(the_repository, ref, &peeled))
		maybe_peeled = &peeled;

	object = parse_object_or_die(the_repository, maybe_peeled, ref->name);
	যদি (object->type == OBJ_COMMIT)
		object->flags |= NEEDS_BITMAP;

	প্রত্যাগম 0;
}

স্থির অন্তঃস্থ পূর্ণাঙ্ক is_oid_uninteresting(সংরচনা repository *repo,
				       সংরচনা object_id *oid)
{
	সংরচনা object *o = lookup_object(repo, oid);
	প্রত্যাগম !o || (o->flags & UNINTERESTING);
}

স্থির পূর্ণাঙ্ক add_objects_by_path(ধ্রুব বর্ণ *path,
			       সংরচনা oid_array *oids,
			       গণনা object_type type,
			       শূন্য *data)
{
	size_t oe_start = to_pack.nr_objects;
	size_t oe_end;
	unsigned পূর্ণাঙ্ক *processed = data;

	/*
	 * First, add all objects to the packing data, including the ones
	 * marked UNINTERESTING (translated to 'exclude') as they can be
	 * used as delta bases.
	 */
	প্রতি (size_t i = 0; i < oids->nr; i++) {
		পূর্ণাঙ্ক exclude;
		সংরচনা object_info oi = OBJECT_INFO_INIT;
		সংরচনা object_id *oid = &oids->oid[i];

		/* Skip objects that do not exist locally. */
		যদি ((exclude_promisor_objects || arg_missing_action != MA_ERROR) &&
		    odb_read_object_info_extended(the_repository->objects, oid, &oi,
						  OBJECT_INFO_FOR_PREFETCH) < 0)
			অনুবর্ত;

		exclude = is_oid_uninteresting(the_repository, oid);

		যদি (exclude && !thin)
			অনুবর্ত;

		add_object_entry(oid, type, path, exclude);

		যদি (type == OBJ_COMMIT) {
			সংরচনা commit *commit;

			যদি (!write_bitmap_index && !use_delta_islands)
				অনুবর্ত;

			commit = lookup_commit(the_repository, oid);
			যদি (!commit)
				die(_("could not find commit %s"), oid_to_hex(oid));
			যদি (write_bitmap_index)
				index_commit_for_bitmap(commit);
			/*
			 * Skip island propagation for boundary commits.
			 * The regular traversal's show_commit() is only
			 * called for interesting commits; matching that
			 * here keeps path-walk from doing extra work that
			 * would only be a no-op anyway (boundary commits
			 * are not in island_marks).
			 */
			যদি (use_delta_islands && !exclude)
				propagate_island_marks(the_repository, commit);
		} অন্যথা যদি (type == OBJ_বৃক্ষ && use_delta_islands) {
			record_বৃক্ষ_depth(oid, path);
		}
	}

	oe_end = to_pack.nr_objects;

	/* We can skip delta calculations if it is a no-op. */
	যদি (oe_end == oe_start || !window)
		প্রত্যাগম 0;

	ALLOC_GROW(to_pack.regions,
		   to_pack.nr_regions + 1,
		   to_pack.nr_regions_alloc);

	to_pack.regions[to_pack.nr_regions].start = oe_start;
	to_pack.regions[to_pack.nr_regions].nr = oe_end - oe_start;
	to_pack.nr_regions++;

	*processed += oids->nr;
	display_progress(progress_state, *processed);

	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক get_object_list_path_walk(সংরচনা rev_info *revs)
{
	সংরচনা path_walk_info info = PATH_WALK_INFO_INIT;
	unsigned পূর্ণাঙ্ক processed = 0;
	পূর্ণাঙ্ক result;

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

	প্রত্যাগম result;
}

স্থির শূন্য get_object_list(সংরচনা rev_info *revs, সংরচনা strvec *argv)
{
	সংরচনা setup_revision_opt s_r_opt = {
		.allow_exclude_promisor_objects = 1,
	};
	সংরচনা repo_config_মানসমূহ *cfg = repo_config_মানসমূহ(the_repository);
	বর্ণ সারি[1000];
	পূর্ণাঙ্ক flags = 0;
	পূর্ণাঙ্ক save_warning;

	save_commit_buffer = 0;
	setup_revisions_from_strvec(argv, revs, &s_r_opt);

	/* make sure shallows are read */
	is_repository_shallow(the_repository);

	save_warning = cfg->warn_on_object_refname_ambiguity;
	cfg->warn_on_object_refname_ambiguity = 0;

	যাবত্ (fgets(সারি, পরিমাণ(সারি), stdin) != রিক্ত) {
		পূর্ণাঙ্ক len = strlen(সারি);
		যদি (len && সারি[len - 1] == '\n')
			সারি[--len] = 0;
		যদি (!len)
			ভঙ্গ;
		যদি (*সারি == '-') {
			যদি (!strcmp(সারি, "--not")) {
				flags ^= UNINTERESTING;
				write_bitmap_index = 0;
				অনুবর্ত;
			}
			যদি (starts_with(সারি, "--shallow ")) {
				সংরচনা object_id oid;
				যদি (get_oid_hex(সারি + 10, &oid))
					die("not an object name '%s'", সারি + 10);
				register_shallow(the_repository, &oid);
				use_bitmap_index = 0;
				অনুবর্ত;
			}
			die(_("not a rev '%s'"), সারি);
		}
		যদি (handle_revision_arg(সারি, revs, flags, REVARG_CANNOT_BE_FILENAME))
			die(_("bad revision '%s'"), সারি);
	}

	cfg->warn_on_object_refname_ambiguity = save_warning;

	যদি (use_bitmap_index && !get_object_list_from_bitmap(revs))
		প্রত্যাগম;

	যদি (use_delta_islands)
		load_delta_islands(the_repository, progress);

	যদি (write_bitmap_index)
		for_each_preferred_bitmap_tip(the_repository, mark_bitmap_preferred_tip,
					      রিক্ত);

	যদি (!fn_show_object)
		fn_show_object = show_object;

	যদি (path_walk) {
		যদি (get_object_list_path_walk(revs)) {
			warning(_("failed to pack objects via path-walk"));
			path_walk = 0;
		}
	}

	যদি (!path_walk) {
		যদি (prepare_revision_walk(revs))
			die(_("revision walk setup failed"));
		mark_edges_uninteresting(revs, show_edge, sparse);
		traverse_commit_list(revs,
				show_commit, fn_show_object,
				রিক্ত);
	}

	যদি (unpack_unreachable_expiration) {
		revs->ignore_missing_links = 1;
		যদি (add_unseen_recent_objects_to_traversal(revs,
				unpack_unreachable_expiration, রিক্ত, 0))
			die(_("unable to add recent objects"));
		যদি (prepare_revision_walk(revs))
			die(_("revision walk setup failed"));
		traverse_commit_list(revs, record_recent_commit,
				     record_recent_object, রিক্ত);
	}

	যদি (keep_unreachable)
		add_objects_in_unpacked_packs();
	যদি (pack_loose_unreachable)
		add_unreachable_loose_objects(রিক্ত);
	যদি (unpack_unreachable)
		loosen_unused_packed_objects();

	oid_array_clear(&recent_objects);
}

স্থির শূন্য add_extra_kept_packs(ধ্রুব সংরচনা string_list *names)
{
	সংরচনা packed_git *p;

	যদি (!names->nr)
		প্রত্যাগম;

	repo_for_each_pack(the_repository, p) {
		ধ্রুব বর্ণ *name = basename(p->pack_name);
		পূর্ণাঙ্ক i;

		যদি (!p->pack_local)
			অনুবর্ত;

		প্রতি (i = 0; i < names->nr; i++)
			যদি (!fspathcmp(name, names->বস্তুসমূহ[i].string))
				ভঙ্গ;

		যদি (i < names->nr) {
			p->pack_keep_in_core = 1;
			ignore_packed_keep_in_core = 1;
			অনুবর্ত;
		}
	}
}

স্থির পূর্ণাঙ্ক option_parse_quiet(ধ্রুব সংরচনা option *opt, ধ্রুব বর্ণ *arg,
			      পূর্ণাঙ্ক unset)
{
	পূর্ণাঙ্ক *val = opt->মান;

	BUG_ON_OPT_ARG(arg);

	যদি (!unset)
		*val = 0;
	অন্যথা যদি (!*val)
		*val = 1;
	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক option_parse_index_version(ধ্রুব সংরচনা option *opt,
				      ধ্রুব বর্ণ *arg, পূর্ণাঙ্ক unset)
{
	সংরচনা pack_idx_option *popts = opt->মান;
	বর্ণ *c;
	ধ্রুব বর্ণ *val = arg;

	BUG_ON_OPT_NEG(unset);

	popts->version = strtoul(val, &c, 10);
	যদি (popts->version > 2)
		die(_("unsupported index version %s"), val);
	যদি (*c == ',' && c[1])
		popts->off32_limit = strtoul(c+1, &c, 0);
	যদি (*c || popts->off32_limit & 0x80000000)
		die(_("bad index version '%s'"), val);
	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক option_parse_unpack_unreachable(ধ্রুব সংরচনা option *opt UNUSED,
					   ধ্রুব বর্ণ *arg, পূর্ণাঙ্ক unset)
{
	যদি (unset) {
		unpack_unreachable = 0;
		unpack_unreachable_expiration = 0;
	}
	অন্যথা {
		unpack_unreachable = 1;
		যদি (arg)
			unpack_unreachable_expiration = approxidate(arg);
	}
	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক option_parse_cruft_expiration(ধ্রুব সংরচনা option *opt UNUSED,
					 ধ্রুব বর্ণ *arg, পূর্ণাঙ্ক unset)
{
	যদি (unset) {
		cruft = 0;
		cruft_expiration = 0;
	} অন্যথা {
		cruft = 1;
		যদি (arg)
			cruft_expiration = approxidate(arg);
	}
	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক is_not_in_promisor_pack_obj(সংরচনা object *obj, শূন্য *data UNUSED)
{
	সংরচনা odb_source_info source_info;
	সংরচনা object_info info = {
		.source_infop = &source_info,
	};

	যদি (odb_read_object_info_extended(the_repository->objects, &obj->oid, &info, 0))
		BUG("should_include_obj should only be called on existing objects");
	প্রত্যাগম source_info.source->type != ODB_SOURCE_PACKED || !source_info.u.packed.pack->pack_promisor;
}

স্থির পূর্ণাঙ্ক is_not_in_promisor_pack(সংরচনা commit *commit, শূন্য *data) {
	প্রত্যাগম is_not_in_promisor_pack_obj((সংরচনা object *) commit, data);
}

স্থির পূর্ণাঙ্ক parse_stdin_packs_mode(ধ্রুব সংরচনা option *opt, ধ্রুব বর্ণ *arg,
				  পূর্ণাঙ্ক unset)
{
	গণনা stdin_packs_mode *mode = opt->মান;

	যদি (unset)
		*mode = STDIN_PACKS_MODE_NONE;
	অন্যথা যদি (!arg || !*arg)
		*mode = STDIN_PACKS_MODE_STANDARD;
	অন্যথা যদি (!strcmp(arg, "follow"))
		*mode = STDIN_PACKS_MODE_FOLLOW;
	অন্যথা
		die(_("invalid value for '%s': '%s'"), opt->long_name, arg);

	প্রত্যাগম 0;
}

পূর্ণাঙ্ক cmd_pack_objects(পূর্ণাঙ্ক argc,
		     ধ্রুব বর্ণ **argv,
		     ধ্রুব বর্ণ *prefix,
		     সংরচনা repository *repo)
{
	পূর্ণাঙ্ক use_internal_rev_list = 0;
	পূর্ণাঙ্ক all_progress_implied = 0;
	সংরচনা strvec rp = STRVEC_INIT;
	পূর্ণাঙ্ক rev_list_unpacked = 0, rev_list_all = 0, rev_list_reflog = 0;
	পূর্ণাঙ্ক rev_list_index = 0;
	গণনা stdin_packs_mode stdin_packs = STDIN_PACKS_MODE_NONE;
	সংরচনা string_list keep_pack_list = STRING_LIST_INIT_NODUP;
	সংরচনা list_objects_filter_options filter_options =
		LIST_OBJECTS_FILTER_INIT;
	সংরচনা repo_config_মানসমূহ *cfg = repo_config_মানসমূহ(the_repository);

	সংরচনা option pack_objects_options[] = {
		OPT_CALLBACK_F('q', "quiet", &progress, রিক্ত,
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
		OPT_INTEGER(0, "threads", &delta_অনুসন্ধান_threads,
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
		OPT_CALLBACK_F(0, "unpack-unreachable", রিক্ত, N_("time"),
		  N_("unpack unreachable objects newer than <time>"),
		  PARSE_OPT_OPTARG, option_parse_unpack_unreachable),
		OPT_BOOL(0, "cruft", &cruft, N_("create a cruft pack")),
		OPT_CALLBACK_F(0, "cruft-expiration", রিক্ত, N_("time"),
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

	যদি (DFS_অঙ্ক_STATES > (1 << OE_DFS_STATE_BITS))
		BUG("too many dfs states, increase OE_DFS_STATE_BITS");

	disable_replace_refs();

	sparse = git_env_bool("GIT_TEST_PACK_SPARSE", -1);
	যদি (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		যদি (sparse < 0)
			sparse = the_repository->settings.pack_use_sparse;
		যদি (the_repository->settings.pack_use_multi_pack_reuse)
			allow_pack_reuse = MULTI_PACK_REUSE;
	}

	reset_pack_idx_option(&pack_idx_opts);
	pack_idx_opts.flags |= WRITE_REV;
	repo_config(the_repository, git_pack_config, রিক্ত);
	যদি (git_env_bool(GIT_TEST_NO_WRITE_REV_INDEX, 0))
		pack_idx_opts.flags &= ~WRITE_REV;

	progress = isatty(2);
	argc = parse_options(argc, argv, prefix, pack_objects_options,
			     pack_usage, 0);

	যদি (argc) {
		base_name = argv[0];
		argc--;
	}
	যদি (pack_to_stdout != !base_name || argc)
		usage_with_options(pack_usage, pack_objects_options);

	যদি (path_walk < 0) {
		যদি (use_bitmap_index > 0 ||
		    !use_internal_rev_list)
			path_walk = 0;
		অন্যথা যদি (the_repository->gitdir &&
			 the_repository->settings.pack_use_path_walk)
			path_walk = 1;
		অন্যথা
			path_walk = git_env_bool("GIT_TEST_PACK_PATH_WALK", 0);
	}

	যদি (depth < 0)
		depth = 0;
	যদি (depth >= (1 << OE_DEPTH_BITS)) {
		warning(_("delta chain depth %d is too deep, forcing %d"),
			depth, (1 << OE_DEPTH_BITS) - 1);
		depth = (1 << OE_DEPTH_BITS) - 1;
	}
	যদি (cache_max_small_delta_size >= (1U << OE_Z_DELTA_BITS)) {
		warning(_("pack.deltaCacheLimit is too high, forcing %d"),
			(1U << OE_Z_DELTA_BITS) - 1);
		cache_max_small_delta_size = (1U << OE_Z_DELTA_BITS) - 1;
	}
	যদি (window < 0)
		window = 0;

	strvec_ঠেলো(&rp, "pack-objects");

	যদি (path_walk) {
		ধ্রুব বর্ণ *option = রিক্ত;
		যদি (!path_walk_filter_compatible(&filter_options))
			option = "--filter";

		যদি (option) {
			warning(_("cannot use %s with %s"),
				option, "--path-walk");
			path_walk = 0;
		}
	}
	যদি (path_walk) {
		strvec_ঠেলো(&rp, "--objects");
	} অন্যথা যদি (thin) {
		use_internal_rev_list = 1;
		strvec_ঠেলো(&rp, shallow
				? "--objects-edge-aggressive"
				: "--objects-edge");
	} অন্যথা
		strvec_ঠেলো(&rp, "--objects");

	যদি (rev_list_all) {
		use_internal_rev_list = 1;
		strvec_ঠেলো(&rp, "--all");
	}
	যদি (rev_list_reflog) {
		use_internal_rev_list = 1;
		strvec_ঠেলো(&rp, "--reflog");
	}
	যদি (rev_list_index) {
		use_internal_rev_list = 1;
		strvec_ঠেলো(&rp, "--indexed-objects");
	}
	যদি (rev_list_unpacked && !stdin_packs) {
		use_internal_rev_list = 1;
		strvec_ঠেলো(&rp, "--unpacked");
	}

	die_for_incompatible_opt2(exclude_promisor_objects,
				  "--exclude-promisor-objects",
				  exclude_promisor_objects_best_effort,
				  "--exclude-promisor-objects-best-effort");
	যদি (exclude_promisor_objects) {
		repo->fetch_if_missing = 0;

		/* --stdin-packs handles promisor objects separately. */
		যদি (!stdin_packs) {
			use_internal_rev_list = 1;
			strvec_ঠেলো(&rp, "--exclude-promisor-objects");
		}
	} অন্যথা যদি (exclude_promisor_objects_best_effort) {
		use_internal_rev_list = 1;
		arg_missing_action = MA_ALLOW_ANY;
		repo->fetch_if_missing = 0;
		fn_show_object = show_object__ma_allow_any;
		/* revs configured below */
	}
	যদি (unpack_unreachable || keep_unreachable || pack_loose_unreachable)
		use_internal_rev_list = 1;

	যদি (!reuse_object)
		reuse_delta = 0;
	যদি (cfg->pack_compression_level == -1)
		cfg->pack_compression_level = Z_DEFAULT_COMPRESSION;
	অন্যথা যদি (cfg->pack_compression_level < 0 || cfg->pack_compression_level > Z_BEST_COMPRESSION)
		die(_("bad pack compression level %d"), cfg->pack_compression_level);

	যদি (!delta_অনুসন্ধান_threads)	/* --threads=0 means autodetect */
		delta_অনুসন্ধান_threads = online_cpus();

	যদি (!HAVE_THREADS && delta_অনুসন্ধান_threads != 1)
		warning(_("no threads support, ignoring --threads"));
	যদি (!pack_to_stdout && !pack_size_limit)
		pack_size_limit = pack_size_limit_cfg;
	যদি (pack_to_stdout && pack_size_limit)
		die(_("--max-pack-size cannot be used to build a pack for transfer"));
	যদি (pack_size_limit && pack_size_limit < 1024*1024) {
		warning(_("minimum pack size limit is 1 MiB"));
		pack_size_limit = 1024*1024;
	}

	যদি (!pack_to_stdout && thin)
		die(_("--thin cannot be used to build an indexable pack"));

	die_for_incompatible_opt2(keep_unreachable, "--keep-unreachable",
				  unpack_unreachable, "--unpack-unreachable");
	যদি (!rev_list_all || !rev_list_reflog || !rev_list_index)
		unpack_unreachable_expiration = 0;

	die_for_incompatible_opt2(stdin_packs, "--stdin-packs",
				  filter_options.choice, "--filter");


	যদি (stdin_packs && use_internal_rev_list)
		die(_("cannot use internal rev list with --stdin-packs"));

	যদি (cruft) {
		যদি (use_internal_rev_list)
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
	যদি (!pack_to_stdout)
		use_bitmap_index_default = 0;

	যদি (use_bitmap_index < 0)
		use_bitmap_index = use_bitmap_index_default;

	/* "hard" reasons not to use bitmaps; these just won't work at all */
	যদি (!use_internal_rev_list || (!pack_to_stdout && write_bitmap_index) || is_repository_shallow(the_repository))
		use_bitmap_index = 0;

	যদি (pack_to_stdout || !rev_list_all)
		write_bitmap_index = 0;

	যদি (name_hash_version < 0)
		name_hash_version = (পূর্ণাঙ্ক)git_env_ulong("GIT_TEST_NAME_HASH_VERSION", 1);

	validate_name_hash_version();

	যদি (use_delta_islands)
		strvec_ঠেলো(&rp, "--topo-order");

	যদি (progress && all_progress_implied)
		progress = 2;

	add_extra_kept_packs(&keep_pack_list);
	যদি (ignore_packed_keep_on_disk) {
		সংরচনা packed_git *p;

		repo_for_each_pack(the_repository, p)
			যদি (p->pack_local && p->pack_keep)
				ভঙ্গ;
		যদি (!p) /* no keep-able packs found */
			ignore_packed_keep_on_disk = 0;
	}
	যদি (local) {
		/*
		 * unlike ignore_packed_keep_on_disk above, we do not
		 * want to unset "local" based on looking at packs, as
		 * it also covers non-local objects
		 */
		সংরচনা packed_git *p;

		repo_for_each_pack(the_repository, p) {
			যদি (!p->pack_local) {
				have_non_local_packs = 1;
				ভঙ্গ;
			}
		}
	}

	trace2_region_enter("pack-objects", "enumerate-objects",
			    the_repository);
	prepare_packing_data(the_repository, &to_pack);

	যদি (progress && !cruft)
		progress_state = start_progress(the_repository,
						_("Enumerating objects"), 0);
	যদি (stdin_packs) {
		read_stdin_packs(repo, stdin_packs, rev_list_unpacked);
	} অন্যথা যদি (cruft) {
		read_cruft_objects();
	} অন্যথা যদি (!use_internal_rev_list) {
		read_object_list_from_stdin();
	} অন্যথা {
		সংরচনা rev_info revs;

		repo_init_revisions(the_repository, &revs, রিক্ত);
		list_objects_filter_copy(&revs.filter, &filter_options);
		যদি (exclude_promisor_objects_best_effort) {
			revs.include_check = is_not_in_promisor_pack;
			revs.include_check_obj = is_not_in_promisor_pack_obj;
		}
		get_object_list(&revs, &rp);
		release_revisions(&revs);
	}
	cleanup_preferred_base();
	যদি (include_tag && nr_result)
		refs_for_each_tag_ref(get_main_ref_store(the_repository),
				      add_ref_tag, রিক্ত);
	stop_progress(&progress_state);
	trace2_region_leave("pack-objects", "enumerate-objects",
			    the_repository);

	যদি (non_empty && !nr_result)
		গচ্ছ cleanup;
	যদি (nr_result) {
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

	যদি (progress)
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

	প্রত্যাগম 0;
}
