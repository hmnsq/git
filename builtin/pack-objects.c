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
স্থির কাঠামো packing_তথ্য to_pack;

স্থির অন্তঃরেখ কাঠামো object_entry *oe_delta(
		ধ্রুবক কাঠামো packing_তথ্য *pack,
		ধ্রুবক কাঠামো object_entry *e)
{
	যদি (!e->delta_idx)
		ফেরত ফাঁকা;
	যদি (e->ext_base)
		ফেরত &pack->ext_bases[e->delta_idx - 1];
	অন্যথা
		ফেরত &pack->objects[e->delta_idx - 1];
}

স্থির অন্তঃরেখ আকার_t oe_delta_আকার(কাঠামো packing_তথ্য *pack,
				   ধ্রুবক কাঠামো object_entry *e)
{
	যদি (e->delta_আকার_valid)
		ফেরত e->delta_আকার;

	/*
	 * pack->delta_size[] can't be NULL because oe_set_delta_size()
	 * must have been called when a new delta is saved with
	 * oe_set_delta().
	 * If oe_delta() returns NULL (i.e. default state, which means
	 * delta_size_valid is also false), then the caller must never
	 * call oe_delta_size().
	 */
	ফেরত pack->delta_আকার_2[e - pack->objects];
}

আকার_t oe_get_আকার_slow(কাঠামো packing_তথ্য *pack,
			ধ্রুবক কাঠামো object_entry *e);

স্থির অন্তঃরেখ আকার_t oe_আকার(কাঠামো packing_তথ্য *pack,
			     ধ্রুবক কাঠামো object_entry *e)
{
	যদি (e->আকার_valid)
		ফেরত e->আকার_3;

	ফেরত oe_get_আকার_slow(pack, e);
}

স্থির অন্তঃরেখ শূন্য oe_set_delta(কাঠামো packing_তথ্য *pack,
				কাঠামো object_entry *e,
				কাঠামো object_entry *delta)
{
	যদি (delta)
		e->delta_idx = (delta - pack->objects) + 1;
	অন্যথা
		e->delta_idx = 0;
}

স্থির অন্তঃরেখ কাঠামো object_entry *oe_delta_sibling(
		ধ্রুবক কাঠামো packing_তথ্য *pack,
		ধ্রুবক কাঠামো object_entry *e)
{
	যদি (e->delta_sibling_idx)
		ফেরত &pack->objects[e->delta_sibling_idx - 1];
	ফেরত ফাঁকা;
}

স্থির অন্তঃরেখ কাঠামো object_entry *oe_delta_child(
		ধ্রুবক কাঠামো packing_তথ্য *pack,
		ধ্রুবক কাঠামো object_entry *e)
{
	যদি (e->delta_child_idx)
		ফেরত &pack->objects[e->delta_child_idx - 1];
	ফেরত ফাঁকা;
}

স্থির অন্তঃরেখ শূন্য oe_set_delta_child(কাঠামো packing_তথ্য *pack,
				      কাঠামো object_entry *e,
				      কাঠামো object_entry *delta)
{
	যদি (delta)
		e->delta_child_idx = (delta - pack->objects) + 1;
	অন্যথা
		e->delta_child_idx = 0;
}

স্থির অন্তঃরেখ শূন্য oe_set_delta_sibling(কাঠামো packing_তথ্য *pack,
					কাঠামো object_entry *e,
					কাঠামো object_entry *delta)
{
	যদি (delta)
		e->delta_sibling_idx = (delta - pack->objects) + 1;
	অন্যথা
		e->delta_sibling_idx = 0;
}

স্থির অন্তঃরেখ শূন্য oe_set_আকার(কাঠামো packing_তথ্য *pack,
			       কাঠামো object_entry *e,
			       আকার_t আকার_2)
{
	যদি (আকার_2 < pack->oe_আকার_limit) {
		e->আকার_3 = আকার_2;
		e->আকার_valid = 1;
	} অন্যথা {
		e->আকার_valid = 0;
		যদি (oe_get_আকার_slow(pack, e) != আকার_2)
			BUG("'size' is supposed to be the object size!");
	}
}

স্থির অন্তঃরেখ শূন্য oe_set_delta_আকার(কাঠামো packing_তথ্য *pack,
				     কাঠামো object_entry *e,
				     আকার_t আকার_2)
{
	যদি (আকার_2 < pack->oe_delta_আকার_limit) {
		e->delta_আকার = আকার_2;
		e->delta_আকার_valid = 1;
	} অন্যথা {
		packing_তথ্য_lock(pack);
		যদি (!pack->delta_আকার_2)
			ALLOC_ARRAY(pack->delta_আকার_2, pack->nr_alloc);
		packing_তথ্য_unlock(pack);

		pack->delta_আকার_2[e - pack->objects] = আকার_2;
		e->delta_আকার_valid = 0;
	}
}

#define IN_PACK(obj) oe_in_pack(&to_pack, obj)
#define আকার_4(obj) oe_আকার(&to_pack, obj)
#define SET_আকার(obj,আকার_2) oe_set_আকার(&to_pack, obj, আকার_2)
#define DELTA_আকার(obj) oe_delta_আকার(&to_pack, obj)
#define DELTA(obj) oe_delta(&to_pack, obj)
#define DELTA_CHILD(obj) oe_delta_child(&to_pack, obj)
#define DELTA_SIBLING(obj) oe_delta_sibling(&to_pack, obj)
#define SET_DELTA(obj, val) oe_set_delta(&to_pack, obj, val)
#define SET_DELTA_EXT(obj, oid) oe_set_delta_ext(&to_pack, obj, oid)
#define SET_DELTA_আকার(obj, val) oe_set_delta_আকার(&to_pack, obj, val)
#define SET_DELTA_CHILD(obj, val) oe_set_delta_child(&to_pack, obj, val)
#define SET_DELTA_SIBLING(obj, val) oe_set_delta_sibling(&to_pack, obj, val)

স্থির ধ্রুবক অক্ষর *ধ্রুবক pack_usage[] = {
	N_("git pack-objects [-q | --progress | --all-progress] [--all-progress-implied]\n"
	   "                 [--no-reuse-delta] [--delta-base-offset] [--non-empty]\n"
	   "                 [--local] [--incremental] [--window=<n>] [--depth=<n>]\n"
	   "                 [--revs [--unpacked | --all]] [--keep-pack=<pack-name>]\n"
	   "                 [--cruft] [--cruft-expiration=<time>]\n"
	   "                 [--stdout [--filter=<filter-spec>] | <base-name>]\n"
	   "                 [--shallow] [--keep-true-parents] [--[no-]sparse]\n"
	   "                 [--name-hash-version=<n>] [--path-walk] < <object-list>"),
	ফাঁকা
};

স্থির কাঠামো pack_idx_entry **written_list;
স্থির uint32_t nr_ফলাফল, nr_written, nr_seen;
স্থির কাঠামো bitmap_সূচক *bitmap_git;
স্থির uint32_t write_layer;

স্থির পূর্ণসংখ্যা non_empty;
স্থির পূর্ণসংখ্যা reuse_delta = 1, reuse_object = 1;
স্থির পূর্ণসংখ্যা keep_unreachable, unpack_unreachable, include_tag;
স্থির timestamp_t unpack_unreachable_expiration;
স্থির পূর্ণসংখ্যা pack_loose_unreachable;
স্থির পূর্ণসংখ্যা cruft;
স্থির পূর্ণসংখ্যা shallow = 0;
স্থির timestamp_t cruft_expiration;
স্থির পূর্ণসংখ্যা local;
স্থির পূর্ণসংখ্যা have_non_local_packs;
স্থির পূর্ণসংখ্যা incremental;
স্থির পূর্ণসংখ্যা ignore_packed_keep_on_disk;
স্থির পূর্ণসংখ্যা ignore_packed_keep_in_core;
স্থির পূর্ণসংখ্যা ignore_packed_keep_in_core_open;
স্থির পূর্ণসংখ্যা ignore_packed_keep_in_core_has_cruft;
স্থির পূর্ণসংখ্যা allow_ofs_delta;
স্থির কাঠামো pack_idx_option pack_idx_opts;
স্থির ধ্রুবক অক্ষর *base_name;
স্থির পূর্ণসংখ্যা progress = 1;
স্থির পূর্ণসংখ্যা window = 10;
স্থির unsigned long pack_আকার_limit;
স্থির পূর্ণসংখ্যা depth = 50;
স্থির পূর্ণসংখ্যা delta_অনুসন্ধান_threads;
স্থির পূর্ণসংখ্যা pack_to_stdout;
স্থির পূর্ণসংখ্যা sparse;
স্থির পূর্ণসংখ্যা thin;
স্থির পূর্ণসংখ্যা path_walk = -1;
স্থির পূর্ণসংখ্যা অঙ্ক_preferred_base;
স্থির কাঠামো progress *progress_state;

স্থির কাঠামো bitmapped_pack *reuse_packfiles;
স্থির আকার_t reuse_packfiles_nr;
স্থির আকার_t reuse_packfiles_used_nr;
স্থির uint32_t reuse_packfile_objects;
স্থির কাঠামো bitmap *reuse_packfile_bitmap;

স্থির পূর্ণসংখ্যা use_bitmap_সূচক_default = 1;
স্থির পূর্ণসংখ্যা use_bitmap_সূচক = -1;
স্থির গণনা {
	NO_PACK_REUSE = 0,
	SINGLE_PACK_REUSE,
	MULTI_PACK_REUSE,
} allow_pack_reuse = SINGLE_PACK_REUSE;
স্থির গণনা {
	WRITE_BITMAP_FALSE = 0,
	WRITE_BITMAP_QUIET,
	WRITE_BITMAP_TRUE,
} write_bitmap_সূচক;
স্থির uint16_t write_bitmap_options = BITMAP_OPT_HASH_CACHE;

স্থির পূর্ণসংখ্যা exclude_promisor_objects;
স্থির পূর্ণসংখ্যা exclude_promisor_objects_best_effort;

স্থির পূর্ণসংখ্যা use_delta_islands;

স্থির আকার_t delta_cache_আকার = 0;
স্থির আকার_t max_delta_cache_আকার = DEFAULT_DELTA_CACHE_আকার;
স্থির unsigned long cache_max_small_delta_আকার = 1000;

স্থির unsigned long window_memory_limit = 0;

স্থির কাঠামো string_list uri_protocols = STRING_LIST_INIT_NODUP;

গণনা missing_action {
	MA_ERROR = 0,      /* fail if any missing objects are encountered */
	MA_ALLOW_ANY,      /* silently allow ALL missing objects */
	MA_ALLOW_PROMISOR, /* silently allow all missing PROMISOR objects */
};
স্থির গণনা missing_action arg_missing_action;
স্থির show_object_fn fn_show_object;

কাঠামো configured_exclusion {
	কাঠামো oidmap_entry e;
	অক্ষর *pack_hash_hex;
	অক্ষর *uri;
};
স্থির কাঠামো oidmap configured_exclusions;

স্থির কাঠামো oidset excluded_by_config;
স্থির পূর্ণসংখ্যা name_hash_version = -1;

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
	যদি (write_bitmap_সূচক && name_hash_version != 1) {
		warning(_("currently, --write-bitmap-index requires --name-hash-version=1"));
		name_hash_version = 1;
	}
}

স্থির অন্তঃরেখ uint32_t pack_name_hash_fn(ধ্রুবক অক্ষর *name)
{
	স্থির পূর্ণসংখ্যা seen_version = -1;

	যদি (seen_version < 0)
		seen_version = name_hash_version;
	অন্যথা যদি (seen_version != name_hash_version)
		BUG("name hash version changed from %d to %d mid-process",
		    seen_version, name_hash_version);

	নির্বাচন (name_hash_version) {
	ক্ষেত্র 1:
		ফেরত pack_name_hash(name);

	ক্ষেত্র 2:
		ফেরত pack_name_hash_v2((ধ্রুবক unsigned অক্ষর *)name);

	পূর্বনির্ধারিত:
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
স্থির কাঠামো commit **indexed_commits;
স্থির unsigned পূর্ণসংখ্যা indexed_commits_nr;
স্থির unsigned পূর্ণসংখ্যা indexed_commits_alloc;

স্থির শূন্য সূচক_commit_for_bitmap(কাঠামো commit *commit)
{
	যদি (indexed_commits_nr >= indexed_commits_alloc) {
		indexed_commits_alloc = (indexed_commits_alloc + 32) * 2;
		REALLOC_ARRAY(indexed_commits, indexed_commits_alloc);
	}

	indexed_commits[indexed_commits_nr++] = commit;
}

স্থির শূন্য *get_delta(কাঠামো object_entry *entry)
{
	unsigned long আকার_2, base_আকার;
	আকার_t delta_আকার_2;
	শূন্য *buf, *base_buf, *delta_buf;
	গণনা object_type type;
	আকার_t আকার_st = 0, base_আকার_st = 0;

	buf = odb_read_object(the_repository->objects, &entry->idx.oid,
			      &type, &আকার_st);
	আকার_2 = cast_আকার_t_to_ulong(আকার_st);
	যদি (!buf)
		die(_("unable to read %s"), oid_to_hex(&entry->idx.oid));
	base_buf = odb_read_object(the_repository->objects,
				   &DELTA(entry)->idx.oid, &type,
				   &base_আকার_st);
	base_আকার = cast_আকার_t_to_ulong(base_আকার_st);
	যদি (!base_buf)
		die("unable to read %s",
		    oid_to_hex(&DELTA(entry)->idx.oid));
	delta_buf = diff_delta(base_buf, base_আকার,
			       buf, আকার_2, &delta_আকার_2, 0);
	/*
	 * We successfully computed this delta once but dropped it for
	 * memory reasons. Something is very wrong if this time we
	 * recompute and create a different delta.
	 */
	যদি (!delta_buf || delta_আকার_2 != DELTA_আকার(entry))
		BUG("delta size changed");
	মুক্ত(buf);
	মুক্ত(base_buf);
	ফেরত delta_buf;
}

স্থির unsigned long do_compress(শূন্য **pptr, unsigned long আকার_2)
{
	git_zstream stream;
	শূন্য *in, *out;
	unsigned long maxsize;
	কাঠামো repo_config_মানসমূহ *cfg = repo_config_মানসমূহ(the_repository);

	git_deflate_init(&stream, cfg->pack_compression_level);
	maxsize = git_deflate_bound(&stream, আকার_2);

	in = *pptr;
	out = xmalloc(maxsize);
	*pptr = out;

	stream.পরবর্তী_in = in;
	stream.avail_in = আকার_2;
	stream.পরবর্তী_out = out;
	stream.avail_out = maxsize;
	যখন (git_deflate(&stream, Z_FINISH) == Z_OK)
		; /* nothing */
	git_deflate_end(&stream);

	মুক্ত(in);
	ফেরত stream.যোগ_out;
}

স্থির unsigned long write_large_blob_তথ্য(কাঠামো odb_stream *st, কাঠামো hashfile *f,
					   ধ্রুবক কাঠামো object_id *oid)
{
	git_zstream stream;
	unsigned অক্ষর ibuf[1024 * 16];
	unsigned অক্ষর obuf[1024 * 16];
	unsigned long olen = 0;
	কাঠামো repo_config_মানসমূহ *cfg = repo_config_মানসমূহ(the_repository);

	git_deflate_init(&stream, cfg->pack_compression_level);

	জন্য (;;) {
		ssize_t readlen;
		পূর্ণসংখ্যা zret = Z_OK;
		readlen = odb_stream_read(st, ibuf, আকার(ibuf));
		যদি (readlen == -1)
			die(_("unable to read %s"), oid_to_hex(oid));

		stream.পরবর্তী_in = ibuf;
		stream.avail_in = readlen;
		যখন ((stream.avail_in || readlen == 0) &&
		       (zret == Z_OK || zret == Z_BUF_ERROR)) {
			stream.পরবর্তী_out = obuf;
			stream.avail_out = আকার(obuf);
			zret = git_deflate(&stream, readlen ? 0 : Z_FINISH);
			hashwrite(f, obuf, stream.পরবর্তী_out - obuf);
			olen += stream.পরবর্তী_out - obuf;
		}
		যদি (stream.avail_in)
			die(_("deflate error (%d)"), zret);
		যদি (readlen == 0) {
			যদি (zret != Z_STREAM_END)
				die(_("deflate error (%d)"), zret);
			বিরতি;
		}
	}
	git_deflate_end(&stream);
	ফেরত olen;
}

/*
 * we are going to reuse the existing object data as is.  make
 * sure it is not corrupt.
 */
স্থির পূর্ণসংখ্যা check_pack_inflate(কাঠামো packed_git *p,
		কাঠামো pack_window **w_curs,
		off_t offset,
		off_t len,
		আকার_t expect)
{
	git_zstream stream;
	unsigned অক্ষর fakebuf[4096], *in;
	পূর্ণসংখ্যা st;

	memset(&stream, 0, আকার(stream));
	git_inflate_init(&stream);
	করো {
		in = use_pack(p, w_curs, offset, &stream.avail_in);
		stream.পরবর্তী_in = in;
		stream.পরবর্তী_out = fakebuf;
		stream.avail_out = আকার(fakebuf);
		st = git_inflate(&stream, Z_FINISH);
		offset += stream.পরবর্তী_in - in;
	} যখন (st == Z_OK || st == Z_BUF_ERROR);
	git_inflate_end(&stream);
	ফেরত (st == Z_STREAM_END &&
		stream.যোগ_out == expect &&
		stream.যোগ_in == len) ? 0 : -1;
}

স্থির শূন্য copy_pack_তথ্য(কাঠামো hashfile *f,
		কাঠামো packed_git *p,
		কাঠামো pack_window **w_curs,
		off_t offset,
		off_t len)
{
	unsigned অক্ষর *in;
	আকার_t avail;

	যখন (len) {
		in = use_pack(p, w_curs, offset, &avail);
		যদি (avail > len)
			avail = (unsigned long)len;
		hashwrite(f, in, avail);
		offset += avail;
		len -= avail;
	}
}

স্থির অন্তঃরেখ পূর্ণসংখ্যা oe_আকার_greater_than(কাঠামো packing_তথ্য *pack,
				       ধ্রুবক কাঠামো object_entry *lhs,
				       আকার_t rhs)
{
	যদি (lhs->আকার_valid)
		ফেরত lhs->আকার_3 > rhs;
	যদি (rhs < pack->oe_আকার_limit) /* rhs < 2^x <= lhs ? */
		ফেরত 1;
	ফেরত oe_get_আকার_slow(pack, lhs) > rhs;
}

/* Return 0 if we will bust the pack-size limit */
স্থির unsigned long write_no_reuse_object(কাঠামো hashfile *f, কাঠামো object_entry *entry,
					   unsigned long limit, পূর্ণসংখ্যা usable_delta)
{
	unsigned long আকার_2, datalen;
	unsigned অক্ষর header[MAX_PACK_OBJECT_HEADER],
		      dheader[MAX_PACK_OBJECT_HEADER];
	unsigned hdrlen;
	গণনা object_type type;
	শূন্য *buf;
	কাঠামো odb_stream *st = ফাঁকা;
	ধ্রুবক unsigned hashsz = the_hash_algo->rawsz;

	যদি (!usable_delta) {
		যদি (oe_type(entry) == OBJ_BLOB &&
		    oe_আকার_greater_than(&to_pack, entry,
					 repo_settings_get_big_file_threshold(the_repository)) &&
		    (st = odb_stream_from_object(the_repository->objects, &entry->idx.oid,
						 ফাঁকা)) != ফাঁকা) {
			buf = ফাঁকা;
			type = st->type;
			আকার_2 = st->আকার_2;
		} অন্যথা {
			আকার_t আকার_st = 0;
			buf = odb_read_object(the_repository->objects,
					      &entry->idx.oid, &type,
					      &আকার_st);
			আকার_2 = cast_আকার_t_to_ulong(আকার_st);
			যদি (!buf)
				die(_("unable to read %s"),
				    oid_to_hex(&entry->idx.oid));
		}
		/*
		 * make sure no cached delta data remains from a
		 * previous attempt before a pack split occurred.
		 */
		FREE_AND_NULL(entry->delta_তথ্য);
		entry->z_delta_আকার = 0;
	} অন্যথা যদি (entry->delta_তথ্য) {
		আকার_2 = DELTA_আকার(entry);
		buf = entry->delta_তথ্য;
		entry->delta_তথ্য = ফাঁকা;
		type = (allow_ofs_delta && DELTA(entry)->idx.offset) ?
			OBJ_OFS_DELTA : OBJ_REF_DELTA;
	} অন্যথা {
		buf = get_delta(entry);
		আকার_2 = DELTA_আকার(entry);
		type = (allow_ofs_delta && DELTA(entry)->idx.offset) ?
			OBJ_OFS_DELTA : OBJ_REF_DELTA;
	}

	যদি (st)	/* large blob case, just assume we don't compress well */
		datalen = আকার_2;
	অন্যথা যদি (entry->z_delta_আকার)
		datalen = entry->z_delta_আকার;
	অন্যথা
		datalen = do_compress(&buf, আকার_2);

	/*
	 * The object header is a byte of 'type' followed by zero or
	 * more bytes of length.
	 */
	hdrlen = encode_in_pack_object_header(header, আকার(header),
					      type, আকার_2);

	যদি (type == OBJ_OFS_DELTA) {
		/*
		 * Deltas with relative base contain an additional
		 * encoding of the relative offset for the delta
		 * base from this object's position in the pack.
		 */
		off_t ofs = entry->idx.offset - DELTA(entry)->idx.offset;
		unsigned pos = আকার(dheader) - 1;
		dheader[pos] = ofs & 127;
		যখন (ofs >>= 7)
			dheader[--pos] = 128 | (--ofs & 127);
		যদি (limit && hdrlen + আকার(dheader) - pos + datalen + hashsz >= limit) {
			যদি (st)
				odb_stream_close(st);
			মুক্ত(buf);
			ফেরত 0;
		}
		hashwrite(f, header, hdrlen);
		hashwrite(f, dheader + pos, আকার(dheader) - pos);
		hdrlen += আকার(dheader) - pos;
	} অন্যথা যদি (type == OBJ_REF_DELTA) {
		/*
		 * Deltas with a base reference contain
		 * additional bytes for the base object ID.
		 */
		যদি (limit && hdrlen + hashsz + datalen + hashsz >= limit) {
			যদি (st)
				odb_stream_close(st);
			মুক্ত(buf);
			ফেরত 0;
		}
		hashwrite(f, header, hdrlen);
		hashwrite(f, DELTA(entry)->idx.oid.hash, hashsz);
		hdrlen += hashsz;
	} অন্যথা {
		যদি (limit && hdrlen + datalen + hashsz >= limit) {
			যদি (st)
				odb_stream_close(st);
			মুক্ত(buf);
			ফেরত 0;
		}
		hashwrite(f, header, hdrlen);
	}
	যদি (st) {
		datalen = write_large_blob_তথ্য(st, f, &entry->idx.oid);
		odb_stream_close(st);
	} অন্যথা {
		hashwrite(f, buf, datalen);
		মুক্ত(buf);
	}

	ফেরত hdrlen + datalen;
}

/* Return 0 if we will bust the pack-size limit */
স্থির off_t write_reuse_object(কাঠামো hashfile *f, কাঠামো object_entry *entry,
				unsigned long limit, পূর্ণসংখ্যা usable_delta)
{
	কাঠামো packed_git *p = IN_PACK(entry);
	কাঠামো pack_window *w_curs = ফাঁকা;
	uint32_t pos;
	off_t offset, cur;
	গণনা object_type type = oe_type(entry);
	গণনা object_type in_pack_type;
	off_t datalen;
	unsigned অক্ষর header[MAX_PACK_OBJECT_HEADER],
		      dheader[MAX_PACK_OBJECT_HEADER];
	unsigned hdrlen;
	ধ্রুবক unsigned hashsz = the_hash_algo->rawsz;
	আকার_t entry_আকার;

	cur = entry->in_pack_offset;
	in_pack_type = unpack_object_header(p, &w_curs, &cur, &entry_আকার);
	যদি (in_pack_type < 0)
		die(_("write_reuse_object: unable to parse object header of %s"),
		    oid_to_hex(&entry->idx.oid));

	যদি (DELTA(entry))
		type = (allow_ofs_delta && DELTA(entry)->idx.offset) ?
			OBJ_OFS_DELTA : OBJ_REF_DELTA;
	hdrlen = encode_in_pack_object_header(header, আকার(header),
					      type, entry_আকার);

	offset = entry->in_pack_offset;
	যদি (offset_to_pack_pos(p, offset, &pos) < 0)
		die(_("write_reuse_object: could not locate %s, expected at "
		      "offset %"PRIuMAX" in pack %s"),
		    oid_to_hex(&entry->idx.oid), (uintmax_t)offset,
		    p->pack_name);
	datalen = pack_pos_to_offset(p, pos + 1) - offset;
	যদি (!pack_to_stdout && p->সূচক_version > 1 &&
	    check_pack_crc(p, &w_curs, offset, datalen,
			   pack_pos_to_সূচক(p, pos))) {
		error(_("bad packed object CRC for %s"),
		      oid_to_hex(&entry->idx.oid));
		unuse_pack(&w_curs);
		ফেরত write_no_reuse_object(f, entry, limit, usable_delta);
	}

	offset += entry->in_pack_header_আকার;
	datalen -= entry->in_pack_header_আকার;

	যদি (!pack_to_stdout && p->সূচক_version == 1 &&
	    check_pack_inflate(p, &w_curs, offset, datalen, entry_আকার)) {
		error(_("corrupt packed object for %s"),
		      oid_to_hex(&entry->idx.oid));
		unuse_pack(&w_curs);
		ফেরত write_no_reuse_object(f, entry, limit, usable_delta);
	}

	যদি (type == OBJ_OFS_DELTA) {
		off_t ofs = entry->idx.offset - DELTA(entry)->idx.offset;
		unsigned pos = আকার(dheader) - 1;
		dheader[pos] = ofs & 127;
		যখন (ofs >>= 7)
			dheader[--pos] = 128 | (--ofs & 127);
		যদি (limit && hdrlen + আকার(dheader) - pos + datalen + hashsz >= limit) {
			unuse_pack(&w_curs);
			ফেরত 0;
		}
		hashwrite(f, header, hdrlen);
		hashwrite(f, dheader + pos, আকার(dheader) - pos);
		hdrlen += আকার(dheader) - pos;
		reused_delta++;
	} অন্যথা যদি (type == OBJ_REF_DELTA) {
		যদি (limit && hdrlen + hashsz + datalen + hashsz >= limit) {
			unuse_pack(&w_curs);
			ফেরত 0;
		}
		hashwrite(f, header, hdrlen);
		hashwrite(f, DELTA(entry)->idx.oid.hash, hashsz);
		hdrlen += hashsz;
		reused_delta++;
	} অন্যথা {
		যদি (limit && hdrlen + datalen + hashsz >= limit) {
			unuse_pack(&w_curs);
			ফেরত 0;
		}
		hashwrite(f, header, hdrlen);
	}
	copy_pack_তথ্য(f, p, &w_curs, offset, datalen);
	unuse_pack(&w_curs);
	reused++;
	ফেরত hdrlen + datalen;
}

/* Return 0 if we will bust the pack-size limit */
স্থির off_t write_object(কাঠামো hashfile *f,
			  কাঠামো object_entry *entry,
			  off_t write_offset)
{
	unsigned long limit;
	off_t len;
	পূর্ণসংখ্যা usable_delta, to_reuse;

	যদি (!pack_to_stdout)
		crc32_begin(f);

	/* apply size limit if limited packsize and not first object */
	যদি (!pack_আকার_limit || !nr_written)
		limit = 0;
	অন্যথা যদি (pack_আকার_limit <= write_offset)
		/*
		 * the earlier object did not fit the limit; avoid
		 * mistaking this with unlimited (i.e. limit = 0).
		 */
		limit = 1;
	অন্যথা
		limit = pack_আকার_limit - write_offset;

	যদি (!DELTA(entry))
		usable_delta = 0;	/* no delta */
	অন্যথা যদি (!pack_আকার_limit)
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
		ফেরত 0;

	যদি (usable_delta)
		written_delta++;
	written++;
	যদি (!pack_to_stdout)
		entry->idx.crc32 = crc32_end(f);
	ফেরত len;
}

গণনা write_one_status {
	WRITE_ONE_SKIP = -1, /* already written */
	WRITE_ONE_BREAK = 0, /* writing this will bust the limit; not written */
	WRITE_ONE_WRITTEN = 1, /* normal */
	WRITE_ONE_RECURSIVE = 2 /* already scheduled to be written */
};

স্থির গণনা write_one_status write_one(কাঠামো hashfile *f,
				       কাঠামো object_entry *e,
				       off_t *offset)
{
	off_t আকার_2;
	পূর্ণসংখ্যা recursing;

	/*
	 * we set offset to 1 (which is an impossible value) to mark
	 * the fact that this object is involved in "write its base
	 * first before writing a deltified object" recursion.
	 */
	recursing = (e->idx.offset == 1);
	যদি (recursing) {
		warning(_("recursive delta detected for object %s"),
			oid_to_hex(&e->idx.oid));
		ফেরত WRITE_ONE_RECURSIVE;
	} অন্যথা যদি (e->idx.offset || e->preferred_base) {
		/* offset is non zero if object is written already. */
		ফেরত WRITE_ONE_SKIP;
	}

	/* if we are deltified, write out base object first. */
	যদি (DELTA(e)) {
		e->idx.offset = 1; /* now recurse */
		নির্বাচন (write_one(f, DELTA(e), offset)) {
		ক্ষেত্র WRITE_ONE_RECURSIVE:
			/* we cannot depend on this one */
			SET_DELTA(e, ফাঁকা);
			বিরতি;
		পূর্বনির্ধারিত:
			বিরতি;
		ক্ষেত্র WRITE_ONE_BREAK:
			e->idx.offset = recursing;
			ফেরত WRITE_ONE_BREAK;
		}
	}

	e->idx.offset = *offset;
	আকার_2 = write_object(f, e, *offset);
	যদি (!আকার_2) {
		e->idx.offset = recursing;
		ফেরত WRITE_ONE_BREAK;
	}
	written_list[nr_written++] = &e->idx;

	/* make sure off_t is sufficiently large not to wrap */
	যদি (signed_add_overflows(*offset, আকার_2))
		die(_("pack too large for current definition of off_t"));
	*offset += আকার_2;
	ফেরত WRITE_ONE_WRITTEN;
}

স্থির পূর্ণসংখ্যা mark_tagged(ধ্রুবক কাঠামো reference *ref, শূন্য *cb_তথ্য UNUSED)
{
	কাঠামো object_id peeled;
	কাঠামো object_entry *entry = packlist_find(&to_pack, ref->oid);

	যদি (entry)
		entry->tagged = 1;
	যদি (!reference_get_peeled_oid(the_repository, ref, &peeled)) {
		entry = packlist_find(&to_pack, &peeled);
		যদি (entry)
			entry->tagged = 1;
	}
	ফেরত 0;
}

স্থির অন্তঃরেখ unsigned অক্ষর oe_layer(কাঠামো packing_তথ্য *pack,
				     কাঠামো object_entry *e)
{
	যদি (!pack->layer)
		ফেরত 0;
	ফেরত pack->layer[e - pack->objects];
}

স্থির অন্তঃরেখ শূন্য add_to_write_order(কাঠামো object_entry **wo,
			       unsigned পূর্ণসংখ্যা *endp,
			       কাঠামো object_entry *e)
{
	যদি (e->filled || oe_layer(&to_pack, e) != write_layer)
		ফেরত;
	wo[(*endp)++] = e;
	e->filled = 1;
}

স্থির শূন্য add_descendants_to_write_order(কাঠামো object_entry **wo,
					   unsigned পূর্ণসংখ্যা *endp,
					   কাঠামো object_entry *e)
{
	পূর্ণসংখ্যা add_to_order = 1;
	যখন (e) {
		যদি (add_to_order) {
			কাঠামো object_entry *s;
			/* add this node... */
			add_to_write_order(wo, endp, e);
			/* all its siblings... */
			জন্য (s = DELTA_SIBLING(e); s; s = DELTA_SIBLING(s)) {
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
				চালু;
			}
			/* go back to our parent node */
			e = DELTA(e);
			যখন (e && !DELTA_SIBLING(e)) {
				/* we're on the right side of a subtree, keep
				 * going up until we can go right again */
				e = DELTA(e);
			}
			যদি (!e) {
				/* done- we hit our original root node */
				ফেরত;
			}
			/* pass it off to sibling at this level */
			e = DELTA_SIBLING(e);
		}
	};
}

স্থির শূন্য add_family_to_write_order(কাঠামো object_entry **wo,
				      unsigned পূর্ণসংখ্যা *endp,
				      কাঠামো object_entry *e)
{
	কাঠামো object_entry *মূল;

	জন্য (মূল = e; DELTA(মূল); মূল = DELTA(মূল))
		; /* nothing */
	add_descendants_to_write_order(wo, endp, মূল);
}

স্থির শূন্য compute_layer_order(কাঠামো object_entry **wo, unsigned পূর্ণসংখ্যা *wo_end)
{
	unsigned পূর্ণসংখ্যা i, last_untagged;
	কাঠামো object_entry *objects = to_pack.objects;

	জন্য (i = 0; i < to_pack.nr_objects; i++) {
		যদি (objects[i].tagged)
			বিরতি;
		add_to_write_order(wo, wo_end, &objects[i]);
	}
	last_untagged = i;

	/*
	 * Then fill all the tagged tips.
	 */
	জন্য (; i < to_pack.nr_objects; i++) {
		যদি (objects[i].tagged)
			add_to_write_order(wo, wo_end, &objects[i]);
	}

	/*
	 * And then all remaining commits and tags.
	 */
	জন্য (i = last_untagged; i < to_pack.nr_objects; i++) {
		যদি (oe_type(&objects[i]) != OBJ_COMMIT &&
		    oe_type(&objects[i]) != OBJ_TAG)
			চালু;
		add_to_write_order(wo, wo_end, &objects[i]);
	}

	/*
	 * And then all the trees.
	 */
	জন্য (i = last_untagged; i < to_pack.nr_objects; i++) {
		যদি (oe_type(&objects[i]) != OBJ_বৃক্ষ)
			চালু;
		add_to_write_order(wo, wo_end, &objects[i]);
	}

	/*
	 * Finally all the rest in really tight order
	 */
	জন্য (i = last_untagged; i < to_pack.nr_objects; i++) {
		যদি (!objects[i].filled && oe_layer(&to_pack, &objects[i]) == write_layer)
			add_family_to_write_order(wo, wo_end, &objects[i]);
	}
}

স্থির কাঠামো object_entry **compute_write_order(শূন্য)
{
	uint32_t max_layers = 1;
	unsigned পূর্ণসংখ্যা i, wo_end;

	কাঠামো object_entry **wo;
	কাঠামো object_entry *objects = to_pack.objects;

	জন্য (i = 0; i < to_pack.nr_objects; i++) {
		objects[i].tagged = 0;
		objects[i].filled = 0;
		SET_DELTA_CHILD(&objects[i], ফাঁকা);
		SET_DELTA_SIBLING(&objects[i], ফাঁকা);
	}

	/*
	 * Fully connect delta_child/delta_sibling network.
	 * Make sure delta_sibling is sorted in the original
	 * recency order.
	 */
	জন্য (i = to_pack.nr_objects; i > 0;) {
		কাঠামো object_entry *e = &objects[--i];
		যদি (!DELTA(e))
			চালু;
		/* Mark me as the first child */
		e->delta_sibling_idx = DELTA(e)->delta_child_idx;
		SET_DELTA_CHILD(DELTA(e), e);
	}

	/*
	 * Mark objects that are at the tip of tags.
	 */
	refs_for_each_tag_ref(get_main_ref_store(the_repository), mark_tagged,
			      ফাঁকা);

	যদি (use_delta_islands) {
		max_layers = compute_pack_layers(&to_pack);
		free_island_marks();
	}

	ALLOC_ARRAY(wo, to_pack.nr_objects);
	wo_end = 0;

	জন্য (; write_layer < max_layers; ++write_layer)
		compute_layer_order(wo, &wo_end);

	যদি (wo_end != to_pack.nr_objects)
		die(_("ordered %u objects, expected %"PRIu32),
		    wo_end, to_pack.nr_objects);

	ফেরত wo;
}


/*
 * A reused set of objects. All objects in a chunk have the same
 * relative position in the original packfile and the generated
 * packfile.
 */

স্থির কাঠামো reused_chunk {
	/* The offset of the first object of this chunk in the original
	 * packfile. */
	off_t original;
	/* The difference for "original" minus the offset of the first object of
	 * this chunk in the generated packfile. */
	off_t difference;
} *reused_chunks;
স্থির পূর্ণসংখ্যা reused_chunks_nr;
স্থির পূর্ণসংখ্যা reused_chunks_alloc;

স্থির শূন্য record_reused_object(off_t where, off_t offset)
{
	যদি (reused_chunks_nr && reused_chunks[reused_chunks_nr-1].difference == offset)
		ফেরত;

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
	পূর্ণসংখ্যা lo = 0, hi = reused_chunks_nr;
	যখন (lo < hi) {
		পূর্ণসংখ্যা mi = lo + ((hi - lo) / 2);
		যদি (where == reused_chunks[mi].original)
			ফেরত reused_chunks[mi].difference;
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
	ফেরত reused_chunks[lo-1].difference;
}

স্থির শূন্য write_reused_pack_one(কাঠামো packed_git *reuse_packfile,
				  আকার_t pos, কাঠামো hashfile *out,
				  off_t pack_start,
				  কাঠামো pack_window **w_curs)
{
	off_t offset, পরবর্তী, cur;
	গণনা object_type type;
	আকার_t আকার_2;

	offset = pack_pos_to_offset(reuse_packfile, pos);
	পরবর্তী = pack_pos_to_offset(reuse_packfile, pos + 1);

	record_reused_object(offset,
			     offset - (hashfile_যোগ(out) - pack_start));

	cur = offset;
	type = unpack_object_header(reuse_packfile, w_curs, &cur, &আকার_2);
	প্রতিজ্ঞা(type >= 0);

	যদি (type == OBJ_OFS_DELTA) {
		off_t base_offset;
		off_t fixup;

		unsigned অক্ষর header[MAX_PACK_OBJECT_HEADER];
		unsigned len;

		base_offset = get_delta_base(reuse_packfile, w_curs, &cur, type, offset);
		প্রতিজ্ঞা(base_offset != 0);

		/* Convert to REF_DELTA if we must... */
		যদি (!allow_ofs_delta) {
			uint32_t base_pos;
			কাঠামো object_id base_oid;

			যদি (offset_to_pack_pos(reuse_packfile, base_offset, &base_pos) < 0)
				die(_("expected object at offset %"PRIuMAX" "
				      "in pack %s"),
				    (uintmax_t)base_offset,
				    reuse_packfile->pack_name);

			nth_packed_object_id(&base_oid, reuse_packfile,
					     pack_pos_to_সূচক(reuse_packfile, base_pos));

			len = encode_in_pack_object_header(header, আকার(header),
							   OBJ_REF_DELTA, আকার_2);
			hashwrite(out, header, len);
			hashwrite(out, base_oid.hash, the_hash_algo->rawsz);
			copy_pack_তথ্য(out, reuse_packfile, w_curs, cur, পরবর্তী - cur);
			ফেরত;
		}

		/* Otherwise see if we need to rewrite the offset... */
		fixup = find_reused_offset(offset) -
			find_reused_offset(base_offset);
		যদি (fixup) {
			unsigned অক্ষর ofs_header[MAX_PACK_OBJECT_HEADER];
			unsigned i, ofs_len;
			off_t ofs = offset - base_offset - fixup;

			len = encode_in_pack_object_header(header, আকার(header),
							   OBJ_OFS_DELTA, আকার_2);

			i = আকার(ofs_header) - 1;
			ofs_header[i] = ofs & 127;
			যখন (ofs >>= 7)
				ofs_header[--i] = 128 | (--ofs & 127);

			ofs_len = আকার(ofs_header) - i;

			hashwrite(out, header, len);
			hashwrite(out, ofs_header + আকার(ofs_header) - ofs_len, ofs_len);
			copy_pack_তথ্য(out, reuse_packfile, w_curs, cur, পরবর্তী - cur);
			ফেরত;
		}

		/* ...otherwise we have no fixup, and can write it verbatim */
	}

	copy_pack_তথ্য(out, reuse_packfile, w_curs, offset, পরবর্তী - offset);
}

স্থির আকার_t write_reused_pack_verbatim(কাঠামো bitmapped_pack *reuse_packfile,
					 কাঠামো hashfile *out,
					 কাঠামো pack_window **w_curs)
{
	আকার_t pos = 0;
	আকার_t end;

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
		ফেরত reuse_packfile->bitmap_pos / BITS_IN_EWORD;
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

	যখন (pos < end && reuse_packfile_bitmap->শব্দগুলি[pos] == (eword_t)~0)
		pos++;

	যদি (pos) {
		off_t to_write;

		written = (pos * BITS_IN_EWORD);
		to_write = pack_pos_to_offset(reuse_packfile->p, written)
			- আকার(কাঠামো pack_header);

		/* We're recording one chunk, not one object. */
		record_reused_object(আকার(কাঠামো pack_header), 0);
		hashflush(out);
		copy_pack_তথ্য(out, reuse_packfile->p, w_curs,
			আকার(কাঠামো pack_header), to_write);

		display_progress(progress_state, written);
	}
	ফেরত pos;
}

স্থির শূন্য write_reused_pack(কাঠামো bitmapped_pack *reuse_packfile,
			      কাঠামো hashfile *f)
{
	আকার_t i = reuse_packfile->bitmap_pos / BITS_IN_EWORD;
	uint32_t offset;
	off_t pack_start = hashfile_যোগ(f) - আকার(কাঠামো pack_header);
	কাঠামো pack_window *w_curs = ফাঁকা;

	যদি (allow_ofs_delta)
		i = write_reused_pack_verbatim(reuse_packfile, f, &w_curs);

	জন্য (; i < reuse_packfile_bitmap->word_alloc; ++i) {
		eword_t word = reuse_packfile_bitmap->শব্দগুলি[i];
		আকার_t pos = (i * BITS_IN_EWORD);

		জন্য (offset = 0; offset < BITS_IN_EWORD; ++offset) {
			uint32_t pack_pos;
			যদি ((word >> offset) == 0)
				বিরতি;

			offset += ewah_bit_ctz64(word >> offset);
			যদি (pos + offset < reuse_packfile->bitmap_pos)
				চালু;
			যদি (pos + offset >= reuse_packfile->bitmap_pos + reuse_packfile->bitmap_nr)
				যাও done;

			যদি (reuse_packfile->bitmap_pos) {
				/*
				 * When doing multi-pack reuse on a
				 * non-preferred pack, translate bit positions
				 * from the MIDX pseudo-pack order back to their
				 * pack-relative positions before attempting
				 * reuse.
				 */
				কাঠামো multi_pack_সূচক *m = reuse_packfile->from_midx;
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
	কাঠামো oidset_iter iter;
	ধ্রুবক কাঠামো object_id *oid;

	oidset_iter_init(&excluded_by_config, &iter);
	যখন ((oid = oidset_iter_পরবর্তী(&iter))) {
		কাঠামো configured_exclusion *ex =
			oidmap_get(&configured_exclusions, oid);

		যদি (!ex)
			BUG("configured exclusion wasn't configured");
		write_in_full(1, ex->pack_hash_hex, strlen(ex->pack_hash_hex));
		write_in_full(1, " ", 1);
		write_in_full(1, ex->uri, strlen(ex->uri));
		write_in_full(1, "\n", 1);
	}
}

স্থির ধ্রুবক অক্ষর no_split_warning[] = N_(
"disabling bitmap writing, packs are split due to pack.packSizeLimit"
);

স্থির শূন্য write_pack_file(শূন্য)
{
	uint32_t i = 0, j;
	কাঠামো hashfile *f;
	off_t offset;
	uint32_t nr_remaining = nr_ফলাফল;
	time_t last_mtime = 0;
	কাঠামো object_entry **write_order;
	off_t bytes_written = 0;

	যদি (progress > pack_to_stdout)
		progress_state = start_progress(the_repository,
						_("Writing objects"), nr_ফলাফল);
	ALLOC_ARRAY(written_list, to_pack.nr_objects);
	write_order = compute_write_order();

	করো {
		unsigned অক্ষর hash[GIT_MAX_RAWSZ];
		অক্ষর *pack_tmp_name = ফাঁকা;

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
			কাঠামো hashfd_options opts = {
				.progress = progress_state,
				.বাফার_len = LARGE_PACKET_তথ্য_MAX - 1,
			};
			f = hashfd_ext(the_repository->hash_algo, 1,
				       "<stdout>", &opts);
		} অন্যথা {
			f = create_tmp_packfile(the_repository, &pack_tmp_name);
		}

		offset = write_pack_header(f, nr_remaining);

		যদি (reuse_packfiles_nr) {
			প্রতিজ্ঞা(pack_to_stdout);
			জন্য (j = 0; j < reuse_packfiles_nr; j++) {
				reused_chunks_nr = 0;
				write_reused_pack(&reuse_packfiles[j], f);
				যদি (reused_chunks_nr)
					reuse_packfiles_used_nr++;
			}
			offset = hashfile_যোগ(f);
		}

		nr_written = 0;
		জন্য (; i < to_pack.nr_objects; i++) {
			কাঠামো object_entry *e = write_order[i];
			যদি (write_one(f, e, &offset) == WRITE_ONE_BREAK)
				বিরতি;
			display_progress(progress_state, written);
		}

		bytes_written += hashfile_যোগ(f) +
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

			পূর্ণসংখ্যা fd = finalize_hashfile(f, hash, FSYNC_COMPONENT_PACK, 0);
			fixup_pack_header_footer(the_hash_algo, fd, hash,
						 pack_tmp_name, nr_written,
						 hash, offset);
			close(fd);
			যদি (write_bitmap_সূচক) {
				যদি (write_bitmap_সূচক != WRITE_BITMAP_QUIET)
					warning(_(no_split_warning));
				write_bitmap_সূচক = 0;
			}
		}

		যদি (!pack_to_stdout) {
			কাঠামো stat st;
			কাঠামো strbuf tmpname = STRBUF_INIT;
			কাঠামো bitmap_writer bitmap_writer;
			অক্ষর *idx_tmp_name = ফাঁকা;

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
				কাঠামো utimbuf utb;
				utb.actime = st.st_atime;
				utb.modtime = --last_mtime;
				যদি (utime(pack_tmp_name, &utb) < 0)
					warning_errno(_("failed utime() on %s"), pack_tmp_name);
			}

			strbuf_addf(&tmpname, "%s-%s.", base_name,
				    hash_to_hex(hash));

			যদি (write_bitmap_সূচক) {
				bitmap_writer_init(&bitmap_writer,
						   the_repository, &to_pack,
						   ফাঁকা);
				bitmap_writer_set_checksum(&bitmap_writer, hash);
				bitmap_writer_build_type_সূচক(&bitmap_writer,
							       written_list);
			}

			যদি (cruft)
				pack_idx_opts.flags |= WRITE_MTIMES;

			stage_tmp_packfiles(the_repository, &tmpname,
					    pack_tmp_name, written_list,
					    nr_written, &to_pack,
					    &pack_idx_opts, hash,
					    &idx_tmp_name);

			যদি (write_bitmap_সূচক) {
				আকার_t tmpname_len = tmpname.len;

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
				write_bitmap_সূচক = 0;
				strbuf_setlen(&tmpname, tmpname_len);
			}

			rename_tmp_packfile_idx(the_repository, &tmpname, &idx_tmp_name);

			মুক্ত(idx_tmp_name);
			strbuf_release(&tmpname);
			মুক্ত(pack_tmp_name);
			puts(hash_to_hex(hash));
		}

		/* mark written objects as written to previous pack */
		জন্য (j = 0; j < nr_written; j++) {
			written_list[j]->offset = (off_t)-1;
		}
		nr_remaining -= nr_written;
	} যখন (nr_remaining && i < to_pack.nr_objects);

	মুক্ত(written_list);
	মুক্ত(write_order);
	stop_progress(&progress_state);
	যদি (written != nr_ফলাফল)
		die(_("wrote %"PRIu32" objects while expecting %"PRIu32),
		    written, nr_ফলাফল);
	trace2_তথ্য_intmax("pack-objects", the_repository,
			   "write_pack_file/wrote", nr_ফলাফল);
	trace2_তথ্য_intmax("pack-objects", the_repository,
			   "write_pack_file/wrote_bytes", bytes_written);
}

স্থির পূর্ণসংখ্যা no_try_delta(ধ্রুবক অক্ষর *path)
{
	স্থির কাঠামো attr_check *check;

	যদি (!check)
		check = attr_check_initl("delta", ফাঁকা);
	git_check_attr(the_repository->সূচক, path, check);
	যদি (ATTR_FALSE(check->বস্তুসমূহ[0].মান))
		ফেরত 1;
	ফেরত 0;
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
স্থির পূর্ণসংখ্যা have_duplicate_entry(ধ্রুবক কাঠামো object_id *oid,
				পূর্ণসংখ্যা exclude)
{
	কাঠামো object_entry *entry;

	যদি (reuse_packfile_bitmap &&
	    bitmap_walk_contains(bitmap_git, reuse_packfile_bitmap, oid))
		ফেরত 1;

	entry = packlist_find(&to_pack, oid);
	যদি (!entry)
		ফেরত 0;

	যদি (exclude) {
		যদি (!entry->preferred_base)
			nr_ফলাফল--;
		entry->preferred_base = 1;
	}

	ফেরত 1;
}

স্থির পূর্ণসংখ্যা want_cruft_object_mtime(কাঠামো repository *r,
				   ধ্রুবক কাঠামো object_id *oid,
				   unsigned flags, uint32_t mtime)
{
	কাঠামো odb_source_files *files = odb_source_files_downcast(r->objects->source);

	জন্য (কাঠামো odb_files_dir *dir = files->dirs; dir; dir = dir->পরবর্তী) {
		কাঠামো packed_git **cache = packfile_store_get_kept_pack_cache(dir->packed, flags);

		জন্য (; *cache; cache++) {
			কাঠামো packed_git *p = *cache;
			off_t ofs;
			uint32_t candidate_mtime;

			ofs = find_pack_entry_one(oid, p);
			যদি (!ofs)
				চালু;

			/*
			 * We have a copy of the object 'oid' in a non-cruft
			 * pack. We can avoid packing an additional copy
			 * regardless of what the existing copy's mtime is since
			 * it is outside of a cruft pack.
			 */
			যদি (!p->is_cruft)
				ফেরত 0;

			/*
			 * If we have a copy of the object 'oid' in a cruft
			 * pack, then either read the cruft pack's mtime for
			 * that object, or, if that can't be loaded, assume the
			 * pack's mtime itself.
			 */
			যদি (!load_pack_mtimes(p)) {
				uint32_t pos;
				যদি (offset_to_pack_pos(p, ofs, &pos) < 0)
					চালু;
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
				ফেরত 0;
		}
	}

	ফেরত -1;
}

স্থির পূর্ণসংখ্যা want_found_object(ধ্রুবক কাঠামো object_id *oid, পূর্ণসংখ্যা exclude,
			     কাঠামো packed_git *p, uint32_t mtime)
{
	যদি (exclude)
		ফেরত 1;
	যদি (incremental)
		ফেরত 0;

	যদি (!is_pack_valid(p))
		ফেরত -1;

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
		ফেরত 0;

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
				ফেরত 0;
			যদি (ignore_packed_keep_in_core && p->pack_keep_in_core)
				ফেরত 0;
			যদি (ignore_packed_keep_in_core_open && p->pack_keep_in_core_open)
				ফেরত 0;
			যদি (has_object_kept_pack(p->repo, oid, flags))
				ফেরত 0;
		} অন্যথা {
			/*
			 * But if there is at least one cruft pack which
			 * is being kept, we only want to include the
			 * provided object if it has a strictly greater
			 * mtime than any existing cruft copy.
			 */
			যদি (!want_cruft_object_mtime(p->repo, oid, flags,
						     mtime))
				ফেরত 0;
		}
	}

	/*
	 * At this point we know definitively that either we don't care about
	 * keep-packs, or the object is not in one. Keep checking other
	 * conditions...
	 */
	যদি (!local || !have_non_local_packs)
		ফেরত 1;

	/* we don't know yet; keep looking for more packs */
	ফেরত -1;
}

স্থির পূর্ণসংখ্যা want_object_in_pack_one(কাঠামো packed_git *p,
				   ধ্রুবক কাঠামো object_id *oid,
				   পূর্ণসংখ্যা exclude,
				   কাঠামো packed_git **found_pack,
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
				ফেরত -1;
			*found_offset = offset;
			*found_pack = p;
		}
		ফেরত want_found_object(oid, exclude, p, found_mtime);
	}
	ফেরত -1;
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
স্থির পূর্ণসংখ্যা want_object_in_pack_mtime(ধ্রুবক কাঠামো object_id *oid,
				     পূর্ণসংখ্যা exclude,
				     কাঠামো packed_git **found_pack,
				     off_t *found_offset,
				     uint32_t found_mtime)
{
	কাঠামো odb_source_files *files =
		odb_source_files_downcast(the_repository->objects->source);
	পূর্ণসংখ্যা want;
	কাঠামো packfile_list_entry *e;

	যদি (!exclude && local) {
		/*
		 * Note that we start iterating at `dirs->next` so that we
		 * skip the local object directory.
		 */
		জন্য (কাঠামো odb_files_dir *dir = files->dirs->পরবর্তী; dir; dir = dir->পরবর্তী)
			যদি (!odb_source_read_object_info(&dir->loose->base, oid, ফাঁকা, 0, ফাঁকা))
				ফেরত 0;
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
			ফেরত want;

		*found_pack = ফাঁকা;
		*found_offset = 0;
	}

	জন্য (কাঠামো odb_files_dir *dir = files->dirs; dir; dir = dir->পরবর্তী) {
		কাঠামো multi_pack_সূচক *m = get_multi_pack_সূচক(dir->packed);
		কাঠামো pack_entry e;

		যদি (m && midx_fill_entry(m, oid, &e, ফাঁকা) == MIDX_FILL_HIT) {
			want = want_object_in_pack_one(e.p, oid, exclude, found_pack, found_offset, found_mtime);
			যদি (want != -1)
				ফেরত want;
		}
	}

	জন্য (কাঠামো odb_files_dir *dir = files->dirs; dir; dir = dir->পরবর্তী) {
		জন্য (e = dir->packed->packs.শীর্ষ; e; e = e->পরবর্তী) {
			কাঠামো packed_git *p = e->pack;
			want = want_object_in_pack_one(p, oid, exclude, found_pack, found_offset, found_mtime);
			যদি (!exclude && want > 0)
				packfile_list_prepend(&dir->packed->packs, p);
			যদি (want != -1)
				ফেরত want;
		}
	}

	যদি (uri_protocols.nr) {
		কাঠামো configured_exclusion *ex =
			oidmap_get(&configured_exclusions, oid);
		পূর্ণসংখ্যা i;
		ধ্রুবক অক্ষর *p;

		যদি (ex) {
			জন্য (i = 0; i < uri_protocols.nr; i++) {
				যদি (skip_prefix(ex->uri,
						uri_protocols.বস্তুসমূহ[i].string,
						&p) &&
				    *p == ':') {
					oidset_ঢোকাও(&excluded_by_config, oid);
					ফেরত 0;
				}
			}
		}
	}

	ফেরত 1;
}

স্থির অন্তঃরেখ পূর্ণসংখ্যা want_object_in_pack(ধ্রুবক কাঠামো object_id *oid,
				      পূর্ণসংখ্যা exclude,
				      কাঠামো packed_git **found_pack,
				      off_t *found_offset)
{
	ফেরত want_object_in_pack_mtime(oid, exclude, found_pack, found_offset,
					 0);
}

স্থির কাঠামো object_entry *create_object_entry(ধ্রুবক কাঠামো object_id *oid,
						গণনা object_type type,
						uint32_t hash,
						পূর্ণসংখ্যা exclude,
						পূর্ণসংখ্যা no_try_delta,
						কাঠামো packed_git *found_pack,
						off_t found_offset)
{
	কাঠামো object_entry *entry;

	entry = packlist_alloc(&to_pack, oid);
	entry->hash = hash;
	oe_set_type(entry, type);
	যদি (exclude)
		entry->preferred_base = 1;
	অন্যথা
		nr_ফলাফল++;
	যদি (found_pack) {
		oe_set_in_pack(&to_pack, entry, found_pack);
		entry->in_pack_offset = found_offset;
	}

	entry->no_try_delta = no_try_delta;

	ফেরত entry;
}

স্থির ধ্রুবক অক্ষর no_closure_warning[] = N_(
"disabling bitmap writing, as some objects are not being packed"
);

স্থির শূন্য add_object_entry(ধ্রুবক কাঠামো object_id *oid, গণনা object_type type,
			     ধ্রুবক অক্ষর *name, পূর্ণসংখ্যা exclude)
{
	কাঠামো packed_git *found_pack = ফাঁকা;
	off_t found_offset = 0;

	display_progress(progress_state, ++nr_seen);

	যদি (have_duplicate_entry(oid, exclude))
		ফেরত;

	যদি (!want_object_in_pack(oid, exclude, &found_pack, &found_offset)) {
		/* The pack is missing an object, so it will not have closure */
		যদি (write_bitmap_সূচক) {
			যদি (write_bitmap_সূচক != WRITE_BITMAP_QUIET)
				warning(_(no_closure_warning));
			write_bitmap_সূচক = 0;
		}
		ফেরত;
	}

	create_object_entry(oid, type, pack_name_hash_fn(name),
			    exclude, name && no_try_delta(name),
			    found_pack, found_offset);
}

স্থির পূর্ণসংখ্যা add_object_entry_from_bitmap(ধ্রুবক কাঠামো object_id *oid,
					গণনা object_type type,
					পূর্ণসংখ্যা flags UNUSED, uint32_t name_hash,
					কাঠামো packed_git *pack, off_t offset,
					শূন্য *payload UNUSED)
{
	display_progress(progress_state, ++nr_seen);

	যদি (have_duplicate_entry(oid, 0))
		ফেরত 0;

	যদি (!want_object_in_pack(oid, 0, &pack, &offset))
		ফেরত 0;

	create_object_entry(oid, type, name_hash, 0, 0, pack, offset);
	ফেরত 0;
}

কাঠামো pbase_বৃক্ষ_cache {
	কাঠামো object_id oid;
	পূর্ণসংখ্যা ref;
	পূর্ণসংখ্যা temporary;
	শূন্য *বৃক্ষ_তথ্য;
	unsigned long বৃক্ষ_আকার;
};

স্থির কাঠামো pbase_বৃক্ষ_cache *(pbase_বৃক্ষ_cache[256]);
স্থির পূর্ণসংখ্যা pbase_বৃক্ষ_cache_ix(ধ্রুবক কাঠামো object_id *oid)
{
	ফেরত oid->hash[0] % ARRAY_আকার(pbase_বৃক্ষ_cache);
}
স্থির পূর্ণসংখ্যা pbase_বৃক্ষ_cache_ix_incr(পূর্ণসংখ্যা ix)
{
	ফেরত (ix+1) % ARRAY_আকার(pbase_বৃক্ষ_cache);
}

স্থির কাঠামো pbase_বৃক্ষ {
	কাঠামো pbase_বৃক্ষ *পরবর্তী;
	/* This is a phony "cache" entry; we are not
	 * going to evict it or find it through _get()
	 * mechanism -- this is for the toplevel node that
	 * would almost always change with any commit.
	 */
	কাঠামো pbase_বৃক্ষ_cache pcache;
} *pbase_বৃক্ষ;

স্থির কাঠামো pbase_বৃক্ষ_cache *pbase_বৃক্ষ_get(ধ্রুবক কাঠামো object_id *oid)
{
	কাঠামো pbase_বৃক্ষ_cache *ent, *nent;
	শূন্য *তথ্য;
	unsigned long আকার_2;
	আকার_t আকার_st = 0;
	গণনা object_type type;
	পূর্ণসংখ্যা neigh;
	পূর্ণসংখ্যা my_ix = pbase_বৃক্ষ_cache_ix(oid);
	পূর্ণসংখ্যা available_ix = -1;

	/* pbase-tree-cache acts as a limited hashtable.
	 * your object will be found at your index or within a few
	 * slots after that slot if it is cached.
	 */
	জন্য (neigh = 0; neigh < 8; neigh++) {
		ent = pbase_বৃক্ষ_cache[my_ix];
		যদি (ent && oideq(&ent->oid, oid)) {
			ent->ref++;
			ফেরত ent;
		}
		অন্যথা যদি (((available_ix < 0) && (!ent || !ent->ref)) ||
			 ((0 <= available_ix) &&
			  (!ent && pbase_বৃক্ষ_cache[available_ix])))
			available_ix = my_ix;
		যদি (!ent)
			বিরতি;
		my_ix = pbase_বৃক্ষ_cache_ix_incr(my_ix);
	}

	/* Did not find one.  Either we got a bogus request or
	 * we need to read and perhaps cache.
	 */
	তথ্য = odb_read_object(the_repository->objects, oid, &type, &আকার_st);
	আকার_2 = cast_আকার_t_to_ulong(আকার_st);
	যদি (!তথ্য)
		ফেরত ফাঁকা;
	যদি (type != OBJ_বৃক্ষ) {
		মুক্ত(তথ্য);
		ফেরত ফাঁকা;
	}

	/* We need to either cache or return a throwaway copy */

	যদি (available_ix < 0)
		ent = ফাঁকা;
	অন্যথা {
		ent = pbase_বৃক্ষ_cache[available_ix];
		my_ix = available_ix;
	}

	যদি (!ent) {
		nent = xmalloc(আকার(*nent));
		nent->temporary = (available_ix < 0);
	}
	অন্যথা {
		/* evict and reuse */
		মুক্ত(ent->বৃক্ষ_তথ্য);
		nent = ent;
	}
	oidcpy(&nent->oid, oid);
	nent->বৃক্ষ_তথ্য = তথ্য;
	nent->বৃক্ষ_আকার = আকার_2;
	nent->ref = 1;
	যদি (!nent->temporary)
		pbase_বৃক্ষ_cache[my_ix] = nent;
	ফেরত nent;
}

স্থির শূন্য pbase_বৃক্ষ_put(কাঠামো pbase_বৃক্ষ_cache *cache)
{
	যদি (!cache->temporary) {
		cache->ref--;
		ফেরত;
	}
	মুক্ত(cache->বৃক্ষ_তথ্য);
	মুক্ত(cache);
}

স্থির আকার_t name_cmp_len(ধ্রুবক অক্ষর *name)
{
	ফেরত strcspn(name, "\n/");
}

স্থির শূন্য add_pbase_object(কাঠামো বৃক্ষ_desc *বৃক্ষ,
			     ধ্রুবক অক্ষর *name,
			     আকার_t cmplen,
			     ধ্রুবক অক্ষর *fullname)
{
	কাঠামো name_entry entry;
	পূর্ণসংখ্যা cmp;

	যখন (বৃক্ষ_entry(বৃক্ষ,&entry)) {
		যদি (S_ISGITLINK(entry.mode))
			চালু;
		cmp = বৃক্ষ_entry_len(&entry) != cmplen ? 1 :
		      memcmp(name, entry.path, cmplen);
		যদি (cmp > 0)
			চালু;
		যদি (cmp < 0)
			ফেরত;
		যদি (name[cmplen] != '/') {
			add_object_entry(&entry.oid,
					 object_type(entry.mode),
					 fullname, 1);
			ফেরত;
		}
		যদি (S_ISDIR(entry.mode)) {
			কাঠামো বৃক্ষ_desc sub;
			কাঠামো pbase_বৃক্ষ_cache *বৃক্ষ;
			ধ্রুবক অক্ষর *down = name+cmplen+1;
			আকার_t downlen = name_cmp_len(down);

			বৃক্ষ = pbase_বৃক্ষ_get(&entry.oid);
			যদি (!বৃক্ষ)
				ফেরত;
			init_বৃক্ষ_desc(&sub, &বৃক্ষ->oid,
				       বৃক্ষ->বৃক্ষ_তথ্য, বৃক্ষ->বৃক্ষ_আকার);

			add_pbase_object(&sub, down, downlen, fullname);
			pbase_বৃক্ষ_put(বৃক্ষ);
		}
	}
}

স্থির unsigned *done_pbase_paths;
স্থির পূর্ণসংখ্যা done_pbase_paths_অঙ্ক;
স্থির পূর্ণসংখ্যা done_pbase_paths_alloc;
স্থির পূর্ণসংখ্যা done_pbase_path_pos(unsigned hash)
{
	পূর্ণসংখ্যা lo = 0;
	পূর্ণসংখ্যা hi = done_pbase_paths_অঙ্ক;
	যখন (lo < hi) {
		পূর্ণসংখ্যা mi = lo + (hi - lo) / 2;
		যদি (done_pbase_paths[mi] == hash)
			ফেরত mi;
		যদি (done_pbase_paths[mi] < hash)
			hi = mi;
		অন্যথা
			lo = mi + 1;
	}
	ফেরত -lo-1;
}

স্থির পূর্ণসংখ্যা check_pbase_path(unsigned hash)
{
	পূর্ণসংখ্যা pos = done_pbase_path_pos(hash);
	যদি (0 <= pos)
		ফেরত 1;
	pos = -pos - 1;
	ALLOC_GROW(done_pbase_paths,
		   done_pbase_paths_অঙ্ক + 1,
		   done_pbase_paths_alloc);
	done_pbase_paths_অঙ্ক++;
	যদি (pos < done_pbase_paths_অঙ্ক)
		MOVE_ARRAY(done_pbase_paths + pos + 1, done_pbase_paths + pos,
			   done_pbase_paths_অঙ্ক - pos - 1);
	done_pbase_paths[pos] = hash;
	ফেরত 0;
}

স্থির শূন্য add_preferred_base_object(ধ্রুবক অক্ষর *name)
{
	কাঠামো pbase_বৃক্ষ *it;
	আকার_t cmplen;
	unsigned hash = pack_name_hash_fn(name);

	যদি (!অঙ্ক_preferred_base || check_pbase_path(hash))
		ফেরত;

	cmplen = name_cmp_len(name);
	জন্য (it = pbase_বৃক্ষ; it; it = it->পরবর্তী) {
		যদি (cmplen == 0) {
			add_object_entry(&it->pcache.oid, OBJ_বৃক্ষ, ফাঁকা, 1);
		}
		অন্যথা {
			কাঠামো বৃক্ষ_desc বৃক্ষ;
			init_বৃক্ষ_desc(&বৃক্ষ, &it->pcache.oid,
				       it->pcache.বৃক্ষ_তথ্য, it->pcache.বৃক্ষ_আকার);
			add_pbase_object(&বৃক্ষ, name, cmplen, name);
		}
	}
}

স্থির শূন্য add_preferred_base(কাঠামো object_id *oid)
{
	কাঠামো pbase_বৃক্ষ *it;
	শূন্য *তথ্য;
	unsigned long আকার_2;
	আকার_t আকার_st = 0;
	কাঠামো object_id বৃক্ষ_oid;

	যদি (window <= অঙ্ক_preferred_base++)
		ফেরত;

	তথ্য = odb_read_object_peeled(the_repository->objects, oid,
				      OBJ_বৃক্ষ, &আকার_st, &বৃক্ষ_oid);
	আকার_2 = cast_আকার_t_to_ulong(আকার_st);
	যদি (!তথ্য)
		ফেরত;

	জন্য (it = pbase_বৃক্ষ; it; it = it->পরবর্তী) {
		যদি (oideq(&it->pcache.oid, &বৃক্ষ_oid)) {
			মুক্ত(তথ্য);
			ফেরত;
		}
	}

	CALLOC_ARRAY(it, 1);
	it->পরবর্তী = pbase_বৃক্ষ;
	pbase_বৃক্ষ = it;

	oidcpy(&it->pcache.oid, &বৃক্ষ_oid);
	it->pcache.বৃক্ষ_তথ্য = তথ্য;
	it->pcache.বৃক্ষ_আকার = আকার_2;
}

স্থির শূন্য cleanup_preferred_base(শূন্য)
{
	কাঠামো pbase_বৃক্ষ *it;
	unsigned i;

	it = pbase_বৃক্ষ;
	pbase_বৃক্ষ = ফাঁকা;
	যখন (it) {
		কাঠামো pbase_বৃক্ষ *tmp = it;
		it = tmp->পরবর্তী;
		মুক্ত(tmp->pcache.বৃক্ষ_তথ্য);
		মুক্ত(tmp);
	}

	জন্য (i = 0; i < ARRAY_আকার(pbase_বৃক্ষ_cache); i++) {
		যদি (!pbase_বৃক্ষ_cache[i])
			চালু;
		মুক্ত(pbase_বৃক্ষ_cache[i]->বৃক্ষ_তথ্য);
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
স্থির পূর্ণসংখ্যা can_reuse_delta(ধ্রুবক কাঠামো object_id *base_oid,
			   কাঠামো object_entry *delta,
			   কাঠামো object_entry **base_out)
{
	কাঠামো object_entry *base;

	/*
	 * First see if we're already sending the base (or it's explicitly in
	 * our "excluded" list).
	 */
	base = packlist_find(&to_pack, base_oid);
	যদি (base) {
		যদি (!in_same_island(&delta->idx.oid, &base->idx.oid))
			ফেরত 0;
		*base_out = base;
		ফেরত 1;
	}

	/*
	 * Otherwise, reachability bitmaps may tell us if the receiver has it,
	 * even if it was buried too deep in history to make it into the
	 * packing list.
	 */
	যদি (thin && bitmap_has_oid_in_uninteresting(bitmap_git, base_oid)) {
		যদি (use_delta_islands) {
			যদি (!in_same_island(&delta->idx.oid, base_oid))
				ফেরত 0;
		}
		*base_out = ফাঁকা;
		ফেরত 1;
	}

	ফেরত 0;
}

স্থির শূন্য prefetch_to_pack(uint32_t object_সূচক_start) {
	কাঠামো oid_array to_fetch = OID_ARRAY_INIT;
	uint32_t i;

	জন্য (i = object_সূচক_start; i < to_pack.nr_objects; i++) {
		কাঠামো object_entry *entry = to_pack.objects + i;

		যদি (!odb_read_object_info_extended(the_repository->objects,
						   &entry->idx.oid,
						   ফাঁকা,
						   OBJECT_INFO_FOR_PREFETCH))
			চালু;
		oid_array_append(&to_fetch, &entry->idx.oid);
	}
	promisor_remote_get_direct(the_repository,
				   to_fetch.oid, to_fetch.nr);
	oid_array_clear(&to_fetch);
}

স্থির শূন্য check_object(কাঠামো object_entry *entry, uint32_t object_সূচক)
{
	আকার_t canonical_আকার;
	গণনা object_type type;
	কাঠামো object_info oi = {.typep = &type, .sizep = &canonical_আকার};

	যদি (IN_PACK(entry)) {
		কাঠামো packed_git *p = IN_PACK(entry);
		কাঠামো pack_window *w_curs = ফাঁকা;
		পূর্ণসংখ্যা have_base = 0;
		কাঠামো object_id base_ref;
		কাঠামো object_entry *base_entry;
		আকার_t used, used_0, avail;
		off_t ofs;
		unsigned অক্ষর *buf, c;
		গণনা object_type type;
		আকার_t in_pack_আকার;

		buf = use_pack(p, &w_curs, entry->in_pack_offset, &avail);

		/*
		 * We want in_pack_type even if we do not reuse delta
		 * since non-delta representations could still be reused.
		 */
		used = unpack_object_header_বাফার(buf, avail,
						   &type,
						   &in_pack_আকার);
		যদি (used == 0)
			যাও give_up;

		যদি (type < 0)
			BUG("invalid type %d", type);
		entry->in_pack_type = type;

		/*
		 * Determine if this is a delta and if so whether we can
		 * reuse it or not.  Otherwise let's find out as cheaply as
		 * possible what the actual type and size for this object is.
		 */
		নির্বাচন (entry->in_pack_type) {
		পূর্বনির্ধারিত:
			/* Not a delta hence we've already got all we need. */
			oe_set_type(entry, entry->in_pack_type);
			SET_আকার(entry, in_pack_আকার);
			entry->in_pack_header_আকার = used;
			যদি (oe_type(entry) < OBJ_COMMIT || oe_type(entry) > OBJ_BLOB)
				যাও give_up;
			unuse_pack(&w_curs);
			ফেরত;
		ক্ষেত্র OBJ_REF_DELTA:
			যদি (reuse_delta && !entry->preferred_base) {
				oidread(&base_ref,
					use_pack(p, &w_curs,
						 entry->in_pack_offset + used,
						 ফাঁকা),
					the_repository->hash_algo);
				have_base = 1;
			}
			entry->in_pack_header_আকার = used + the_hash_algo->rawsz;
			বিরতি;
		ক্ষেত্র OBJ_OFS_DELTA:
			buf = use_pack(p, &w_curs,
				       entry->in_pack_offset + used, ফাঁকা);
			used_0 = 0;
			c = buf[used_0++];
			ofs = c & 127;
			যখন (c & 128) {
				ofs += 1;
				যদি (!ofs || MSB(ofs, 7)) {
					error(_("delta base offset overflow in pack for %s"),
					      oid_to_hex(&entry->idx.oid));
					যাও give_up;
				}
				c = buf[used_0++];
				ofs = (ofs << 7) + (c & 127);
			}
			ofs = entry->in_pack_offset - ofs;
			যদি (ofs <= 0 || ofs >= entry->in_pack_offset) {
				error(_("delta base offset out of bound for %s"),
				      oid_to_hex(&entry->idx.oid));
				যাও give_up;
			}
			যদি (reuse_delta && !entry->preferred_base) {
				uint32_t pos;
				যদি (offset_to_pack_pos(p, ofs, &pos) < 0)
					যাও give_up;
				যদি (!nth_packed_object_id(&base_ref, p,
							  pack_pos_to_সূচক(p, pos)))
					have_base = 1;
			}
			entry->in_pack_header_আকার = used + used_0;
			বিরতি;
		}

		যদি (have_base &&
		    can_reuse_delta(&base_ref, entry, &base_entry)) {
			oe_set_type(entry, entry->in_pack_type);
			SET_আকার(entry, in_pack_আকার); /* delta size */
			SET_DELTA_আকার(entry, in_pack_আকার);

			যদি (base_entry) {
				SET_DELTA(entry, base_entry);
				entry->delta_sibling_idx = base_entry->delta_child_idx;
				SET_DELTA_CHILD(base_entry, entry);
			} অন্যথা {
				SET_DELTA_EXT(entry, &base_ref);
			}

			unuse_pack(&w_curs);
			ফেরত;
		}

		যদি (oe_type(entry)) {
			off_t delta_pos;

			/*
			 * This must be a delta and we already know what the
			 * final object type is.  Let's extract the actual
			 * object size from the delta header.
			 */
			delta_pos = entry->in_pack_offset + entry->in_pack_header_আকার;
			canonical_আকার = get_আকার_from_delta(p, &w_curs,
							     delta_pos);
			যদি (canonical_আকার == 0)
				যাও give_up;
			SET_আকার(entry, canonical_আকার);
			unuse_pack(&w_curs);
			ফেরত;
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
			prefetch_to_pack(object_সূচক);
			যদি (odb_read_object_info_extended(the_repository->objects, &entry->idx.oid, &oi,
							  OBJECT_INFO_SKIP_FETCH_OBJECT | OBJECT_INFO_LOOKUP_REPLACE) < 0)
				type = -1;
		} অন্যথা {
			type = -1;
		}
	}
	oe_set_type(entry, type);
	যদি (entry->type_valid) {
		SET_আকার(entry, canonical_আকার);
	} অন্যথা {
		/*
		 * Bad object type is checked in prepare_pack().  This is
		 * to permit a missing preferred base object to be ignored
		 * as a preferred base.  Doing so can result in a larger
		 * pack file, but the transfer will still take place.
		 */
	}
}

স্থির পূর্ণসংখ্যা pack_offset_sort(ধ্রুবক শূন্য *_a, ধ্রুবক শূন্য *_b)
{
	ধ্রুবক কাঠামো object_entry *a = *(কাঠামো object_entry **)_a;
	ধ্রুবক কাঠামো object_entry *b = *(কাঠামো object_entry **)_b;
	ধ্রুবক কাঠামো packed_git *a_in_pack = IN_PACK(a);
	ধ্রুবক কাঠামো packed_git *b_in_pack = IN_PACK(b);

	/* avoid filesystem trashing with loose objects */
	যদি (!a_in_pack && !b_in_pack)
		ফেরত oidcmp(&a->idx.oid, &b->idx.oid);

	যদি (a_in_pack < b_in_pack)
		ফেরত -1;
	যদি (a_in_pack > b_in_pack)
		ফেরত 1;
	ফেরত a->in_pack_offset < b->in_pack_offset ? -1 :
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
স্থির শূন্য drop_reused_delta(কাঠামো object_entry *entry)
{
	unsigned *idx = &to_pack.objects[entry->delta_idx - 1].delta_child_idx;
	কাঠামো object_info oi = OBJECT_INFO_INIT;
	গণনা object_type type;
	আকার_t আকার_2;

	যখন (*idx) {
		কাঠামো object_entry *oe = &to_pack.objects[*idx - 1];

		যদি (oe == entry)
			*idx = oe->delta_sibling_idx;
		অন্যথা
			idx = &oe->delta_sibling_idx;
	}
	SET_DELTA(entry, ফাঁকা);
	entry->depth = 0;

	oi.sizep = &আকার_2;
	oi.typep = &type;
	যদি (packed_object_info(ফাঁকা, IN_PACK(entry), entry->in_pack_offset, &oi) < 0) {
		/*
		 * We failed to get the info from this pack for some reason;
		 * fall back to odb_read_object_info, which may find another copy.
		 * And if that fails, the error will be recorded in oe_type(entry)
		 * and dealt with in prepare_pack().
		 */
		oe_set_type(entry,
			    odb_read_object_info(the_repository->objects,
						 &entry->idx.oid, &আকার_2));
	} অন্যথা {
		oe_set_type(entry, type);
	}
	SET_আকার(entry, আকার_2);
}

/*
 * Follow the chain of deltas from this entry onward, throwing away any links
 * that cause us to hit a cycle (as determined by the DFS state flags in
 * the entries).
 *
 * We also detect too-long reused chains that would violate our --depth
 * limit.
 */
স্থির শূন্য break_delta_chains(কাঠামো object_entry *entry)
{
	/*
	 * The actual depth of each object we will write is stored as an int,
	 * as it cannot exceed our int "depth" limit. But before we break
	 * changes based no that limit, we may potentially go as deep as the
	 * number of objects, which is elsewhere bounded to a uint32_t.
	 */
	uint32_t যোগ_depth;
	কাঠামো object_entry *cur, *পরবর্তী;

	জন্য (cur = entry, যোগ_depth = 0;
	     cur;
	     cur = DELTA(cur), যোগ_depth++) {
		যদি (cur->dfs_state == DFS_DONE) {
			/*
			 * We've already seen this object and know it isn't
			 * part of a cycle. We do need to append its depth
			 * to our count.
			 */
			যোগ_depth += cur->depth;
			বিরতি;
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
			বিরতি;
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
			বিরতি;
		}
	}

	/*
	 * And now that we've gone all the way to the bottom of the chain, we
	 * need to clear the active flags and set the depth fields as
	 * appropriate. Unlike the loop above, which can quit when it drops a
	 * delta, we need to keep going to look for more depth cuts. So we need
	 * an extra "next" pointer to keep going after we reset cur->delta.
	 */
	জন্য (cur = entry; cur; cur = পরবর্তী) {
		পরবর্তী = DELTA(cur);

		/*
		 * We should have a chain of zero or more ACTIVE states down to
		 * a final DONE. We can quit after the DONE, because either it
		 * has no bases, or we've already handled them in a previous
		 * call.
		 */
		যদি (cur->dfs_state == DFS_DONE)
			বিরতি;
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
		cur->depth = (যোগ_depth--) % (depth + 1);
		যদি (!cur->depth)
			drop_reused_delta(cur);

		cur->dfs_state = DFS_DONE;
	}
}

স্থির শূন্য get_object_details(শূন্য)
{
	uint32_t i;
	কাঠামো object_entry **sorted_by_offset;

	যদি (progress)
		progress_state = start_progress(the_repository,
						_("Counting objects"),
						to_pack.nr_objects);

	CALLOC_ARRAY(sorted_by_offset, to_pack.nr_objects);
	জন্য (i = 0; i < to_pack.nr_objects; i++)
		sorted_by_offset[i] = to_pack.objects + i;
	QSORT(sorted_by_offset, to_pack.nr_objects, pack_offset_sort);

	জন্য (i = 0; i < to_pack.nr_objects; i++) {
		কাঠামো object_entry *entry = sorted_by_offset[i];
		check_object(entry, i);
		যদি (entry->type_valid &&
		    oe_আকার_greater_than(&to_pack, entry,
					 repo_settings_get_big_file_threshold(the_repository)))
			entry->no_try_delta = 1;
		display_progress(progress_state, i + 1);
	}
	stop_progress(&progress_state);

	/*
	 * This must happen in a second pass, since we rely on the delta
	 * information for the whole list being completed.
	 */
	জন্য (i = 0; i < to_pack.nr_objects; i++)
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
স্থির পূর্ণসংখ্যা type_আকার_sort(ধ্রুবক শূন্য *_a, ধ্রুবক শূন্য *_b)
{
	ধ্রুবক কাঠামো object_entry *a = *(কাঠামো object_entry **)_a;
	ধ্রুবক কাঠামো object_entry *b = *(কাঠামো object_entry **)_b;
	ধ্রুবক গণনা object_type a_type = oe_type(a);
	ধ্রুবক গণনা object_type b_type = oe_type(b);
	ধ্রুবক unsigned long a_আকার = আকার_4(a);
	ধ্রুবক unsigned long b_আকার = আকার_4(b);

	যদি (a_type > b_type)
		ফেরত -1;
	যদি (a_type < b_type)
		ফেরত 1;
	যদি (a->hash > b->hash)
		ফেরত -1;
	যদি (a->hash < b->hash)
		ফেরত 1;
	যদি (a->preferred_base > b->preferred_base)
		ফেরত -1;
	যদি (a->preferred_base < b->preferred_base)
		ফেরত 1;
	যদি (use_delta_islands) {
		ধ্রুবক পূর্ণসংখ্যা island_cmp = island_delta_cmp(&a->idx.oid, &b->idx.oid);
		যদি (island_cmp)
			ফেরত island_cmp;
	}
	যদি (a_আকার > b_আকার)
		ফেরত -1;
	যদি (a_আকার < b_আকার)
		ফেরত 1;
	ফেরত a < b ? -1 : (a > b);  /* newest first */
}

কাঠামো unpacked {
	কাঠামো object_entry *entry;
	শূন্য *তথ্য;
	কাঠামো delta_সূচক *সূচক;
	unsigned depth;
};

স্থির পূর্ণসংখ্যা delta_cacheable(আকার_t src_আকার, আকার_t trg_আকার,
			   আকার_t delta_আকার_2)
{
	যদি (max_delta_cache_আকার && delta_cache_আকার + delta_আকার_2 > max_delta_cache_আকার)
		ফেরত 0;

	যদি (delta_আকার_2 < cache_max_small_delta_আকার)
		ফেরত 1;

	/* cache delta, if objects are large enough compared to delta size */
	যদি ((src_আকার >> 20) + (trg_আকার >> 21) > (delta_আকার_2 >> 10))
		ফেরত 1;

	ফেরত 0;
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

স্থির অন্তঃরেখ পূর্ণসংখ্যা oe_আকার_less_than(কাঠামো packing_তথ্য *pack,
				    ধ্রুবক কাঠামো object_entry *lhs,
				    আকার_t rhs)
{
	যদি (lhs->আকার_valid)
		ফেরত lhs->আকার_3 < rhs;
	যদি (rhs < pack->oe_আকার_limit) /* rhs < 2^x <= lhs ? */
		ফেরত 0;
	ফেরত oe_get_আকার_slow(pack, lhs) < rhs;
}

স্থির অন্তঃরেখ শূন্য oe_set_বৃক্ষ_depth(কাঠামো packing_তথ্য *pack,
				     কাঠামো object_entry *e,
				     unsigned পূর্ণসংখ্যা বৃক্ষ_depth)
{
	যদি (!pack->বৃক্ষ_depth)
		CALLOC_ARRAY(pack->বৃক্ষ_depth, pack->nr_alloc);
	pack->বৃক্ষ_depth[e - pack->objects] = বৃক্ষ_depth;
}

স্থির শূন্য record_বৃক্ষ_depth(ধ্রুবক কাঠামো object_id *oid, ধ্রুবক অক্ষর *name)
{
	ধ্রুবক অক্ষর *p;
	unsigned depth;
	কাঠামো object_entry *ent;

	/* the empty string is a root tree, which is depth 0 */
	depth = *name ? 1 : 0;
	জন্য (p = strchr(name, '/'); p; p = strchr(p + 1, '/'))
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
আকার_t oe_get_আকার_slow(কাঠামো packing_তথ্য *pack,
			ধ্রুবক কাঠামো object_entry *e)
{
	কাঠামো packed_git *p;
	কাঠামো pack_window *w_curs;
	unsigned অক্ষর *buf;
	গণনা object_type type;
	আকার_t used, avail, আকার_2;

	যদি (e->type_ != OBJ_OFS_DELTA && e->type_ != OBJ_REF_DELTA) {
		আকার_t sz;
		packing_তথ্য_lock(&to_pack);
		যদি (odb_read_object_info(the_repository->objects,
					 &e->idx.oid, &sz) < 0)
			die(_("unable to get size of %s"),
			    oid_to_hex(&e->idx.oid));
		packing_তথ্য_unlock(&to_pack);
		ফেরত sz;
	}

	p = oe_in_pack(pack, e);
	যদি (!p)
		BUG("when e->type is a delta, it must belong to a pack");

	packing_তথ্য_lock(&to_pack);
	w_curs = ফাঁকা;
	buf = use_pack(p, &w_curs, e->in_pack_offset, &avail);
	used = unpack_object_header_বাফার(buf, avail, &type, &আকার_2);
	যদি (used == 0)
		die(_("unable to parse object header of %s"),
		    oid_to_hex(&e->idx.oid));

	unuse_pack(&w_curs);
	packing_তথ্য_unlock(&to_pack);
	ফেরত আকার_2;
}

স্থির পূর্ণসংখ্যা try_delta(কাঠামো unpacked *trg, কাঠামো unpacked *src,
		     unsigned max_depth, আকার_t *mem_usage)
{
	কাঠামো object_entry *trg_entry = trg->entry;
	কাঠামো object_entry *src_entry = src->entry;
	unsigned long trg_আকার, src_আকার, sizediff, max_আকার, sz;
	আকার_t delta_আকার_2;
	unsigned ref_depth;
	গণনা object_type type;
	শূন্য *delta_buf;

	/* Don't bother doing diffs between different types */
	যদি (oe_type(trg_entry) != oe_type(src_entry))
		ফেরত -1;

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
		ফেরত 0;

	/* Let's not bust the allowed depth. */
	যদি (src->depth >= max_depth)
		ফেরত 0;

	/* Now some size filtering heuristics. */
	trg_আকার = আকার_4(trg_entry);
	যদি (!DELTA(trg_entry)) {
		max_আকার = trg_আকার/2 - the_hash_algo->rawsz;
		ref_depth = 1;
	} অন্যথা {
		max_আকার = DELTA_আকার(trg_entry);
		ref_depth = trg->depth;
	}
	max_আকার = (uint64_t)max_আকার * (max_depth - src->depth) /
						(max_depth - ref_depth + 1);
	যদি (max_আকার == 0)
		ফেরত 0;
	src_আকার = আকার_4(src_entry);
	sizediff = src_আকার < trg_আকার ? trg_আকার - src_আকার : 0;
	যদি (sizediff >= max_আকার)
		ফেরত 0;
	যদি (trg_আকার < src_আকার / 32)
		ফেরত 0;

	যদি (!in_same_island(&trg->entry->idx.oid, &src->entry->idx.oid))
		ফেরত 0;

	/* Load data if not already done */
	যদি (!trg->তথ্য) {
		আকার_t sz_st = 0;
		packing_তথ্য_lock(&to_pack);
		trg->তথ্য = odb_read_object(the_repository->objects,
					    &trg_entry->idx.oid, &type,
					    &sz_st);
		sz = cast_আকার_t_to_ulong(sz_st);
		packing_তথ্য_unlock(&to_pack);
		যদি (!trg->তথ্য)
			die(_("object %s cannot be read"),
			    oid_to_hex(&trg_entry->idx.oid));
		যদি (sz != trg_আকার)
			die(_("object %s inconsistent object length (%"PRIuMAX" vs %"PRIuMAX")"),
			    oid_to_hex(&trg_entry->idx.oid), (uintmax_t)sz,
			    (uintmax_t)trg_আকার);
		*mem_usage += sz;
	}
	যদি (!src->তথ্য) {
		আকার_t sz_st = 0;
		packing_তথ্য_lock(&to_pack);
		src->তথ্য = odb_read_object(the_repository->objects,
					    &src_entry->idx.oid, &type,
					    &sz_st);
		sz = cast_আকার_t_to_ulong(sz_st);
		packing_তথ্য_unlock(&to_pack);
		যদি (!src->তথ্য) {
			যদি (src_entry->preferred_base) {
				স্থির পূর্ণসংখ্যা warned = 0;
				যদি (!warned++)
					warning(_("object %s cannot be read"),
						oid_to_hex(&src_entry->idx.oid));
				/*
				 * Those objects are not included in the
				 * resulting pack.  Be resilient and ignore
				 * them if they can't be read, in case the
				 * pack could be created nevertheless.
				 */
				ফেরত 0;
			}
			die(_("object %s cannot be read"),
			    oid_to_hex(&src_entry->idx.oid));
		}
		যদি (sz != src_আকার)
			die(_("object %s inconsistent object length (%"PRIuMAX" vs %"PRIuMAX")"),
			    oid_to_hex(&src_entry->idx.oid), (uintmax_t)sz,
			    (uintmax_t)src_আকার);
		*mem_usage += sz;
	}
	যদি (!src->সূচক) {
		src->সূচক = create_delta_সূচক(src->তথ্য, src_আকার);
		যদি (!src->সূচক) {
			স্থির পূর্ণসংখ্যা warned = 0;
			যদি (!warned++)
				warning(_("suboptimal pack - out of memory"));
			ফেরত 0;
		}
		*mem_usage += sizeof_delta_সূচক(src->সূচক);
	}

	delta_buf = create_delta(src->সূচক, trg->তথ্য, trg_আকার, &delta_আকার_2, max_আকার);
	যদি (!delta_buf)
		ফেরত 0;

	যদি (DELTA(trg_entry)) {
		/* Prefer only shallower same-sized deltas. */
		যদি (delta_আকার_2 == DELTA_আকার(trg_entry) &&
		    src->depth + 1 >= trg->depth) {
			মুক্ত(delta_buf);
			ফেরত 0;
		}
	}

	/*
	 * Handle memory allocation outside of the cache
	 * accounting lock.  Compiler will optimize the strangeness
	 * away when NO_PTHREADS is defined.
	 */
	মুক্ত(trg_entry->delta_তথ্য);
	cache_lock();
	যদি (trg_entry->delta_তথ্য) {
		delta_cache_আকার -= DELTA_আকার(trg_entry);
		trg_entry->delta_তথ্য = ফাঁকা;
	}
	যদি (delta_cacheable(src_আকার, trg_আকার, delta_আকার_2)) {
		delta_cache_আকার += delta_আকার_2;
		cache_unlock();
		trg_entry->delta_তথ্য = xrealloc(delta_buf, delta_আকার_2);
	} অন্যথা {
		cache_unlock();
		মুক্ত(delta_buf);
	}

	SET_DELTA(trg_entry, src_entry);
	SET_DELTA_আকার(trg_entry, delta_আকার_2);
	trg->depth = src->depth + 1;

	ফেরত 1;
}

স্থির unsigned পূর্ণসংখ্যা check_delta_limit(কাঠামো object_entry *me, unsigned পূর্ণসংখ্যা n)
{
	কাঠামো object_entry *child = DELTA_CHILD(me);
	unsigned পূর্ণসংখ্যা m = n;
	যখন (child) {
		ধ্রুবক unsigned পূর্ণসংখ্যা c = check_delta_limit(child, n + 1);
		যদি (m < c)
			m = c;
		child = DELTA_SIBLING(child);
	}
	ফেরত m;
}

স্থির আকার_t free_unpacked(কাঠামো unpacked *n)
{
	আকার_t freed_mem = sizeof_delta_সূচক(n->সূচক);
	free_delta_সূচক(n->সূচক);
	n->সূচক = ফাঁকা;
	যদি (n->তথ্য) {
		freed_mem += আকার_4(n->entry);
		FREE_AND_NULL(n->তথ্য);
	}
	n->entry = ফাঁকা;
	n->depth = 0;
	ফেরত freed_mem;
}

স্থির শূন্য find_deltas(কাঠামো object_entry **list, unsigned *list_আকার,
			পূর্ণসংখ্যা window, পূর্ণসংখ্যা depth, unsigned *processed)
{
	uint32_t i, idx = 0, গুনতি = 0;
	কাঠামো unpacked *array;
	আকার_t mem_usage = 0;

	CALLOC_ARRAY(array, window);

	জন্য (;;) {
		কাঠামো object_entry *entry;
		কাঠামো unpacked *n = array + idx;
		পূর্ণসংখ্যা j, max_depth, best_base = -1;

		progress_lock();
		যদি (!*list_আকার) {
			progress_unlock();
			বিরতি;
		}
		entry = *list++;
		(*list_আকার)--;
		যদি (!entry->preferred_base) {
			(*processed)++;
			display_progress(progress_state, *processed);
		}
		progress_unlock();

		mem_usage -= free_unpacked(n);
		n->entry = entry;

		যখন (window_memory_limit &&
		       mem_usage > window_memory_limit &&
		       গুনতি > 1) {
			ধ্রুবক uint32_t লেজ = (idx + window - গুনতি) % window;
			mem_usage -= free_unpacked(array + লেজ);
			গুনতি--;
		}

		/* We do not compute delta to *create* objects we are not
		 * going to pack.
		 */
		যদি (entry->preferred_base)
			যাও পরবর্তী;

		/*
		 * If the current object is at pack edge, take the depth the
		 * objects that depend on the current object into account
		 * otherwise they would become too deep.
		 */
		max_depth = depth;
		যদি (DELTA_CHILD(entry)) {
			max_depth -= check_delta_limit(entry, 0);
			যদি (max_depth <= 0)
				যাও পরবর্তী;
		}

		j = window;
		যখন (--j > 0) {
			পূর্ণসংখ্যা ret;
			uint32_t other_idx = idx + j;
			কাঠামো unpacked *m;
			যদি (other_idx >= window)
				other_idx -= window;
			m = array + other_idx;
			যদি (!m->entry)
				বিরতি;
			ret = try_delta(n, m, max_depth, &mem_usage);
			যদি (ret < 0)
				বিরতি;
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
		যদি (entry->delta_তথ্য && !pack_to_stdout) {
			unsigned long আকার_2;

			আকার_2 = do_compress(&entry->delta_তথ্য, DELTA_আকার(entry));
			যদি (আকার_2 < (1U << OE_Z_DELTA_BITS)) {
				entry->z_delta_আকার = আকার_2;
				cache_lock();
				delta_cache_আকার -= DELTA_আকার(entry);
				delta_cache_আকার += entry->z_delta_আকার;
				cache_unlock();
			} অন্যথা {
				FREE_AND_NULL(entry->delta_তথ্য);
				entry->z_delta_আকার = 0;
			}
		}

		/* if we made n a delta, and if n is already at max
		 * depth, leaving it in the window is pointless.  we
		 * should evict it first.
		 */
		যদি (DELTA(entry) && max_depth <= n->depth)
			চালু;

		/*
		 * Move the best delta base up in the window, after the
		 * currently deltified object, to keep it longer.  It will
		 * be the first base object to be attempted next.
		 */
		যদি (DELTA(entry)) {
			কাঠামো unpacked swap = array[best_base];
			পূর্ণসংখ্যা dist = (window + idx - best_base) % window;
			পূর্ণসংখ্যা dst = best_base;
			যখন (dist--) {
				পূর্ণসংখ্যা src = (dst + 1) % window;
				array[dst] = array[src];
				dst = src;
			}
			array[dst] = swap;
		}

		পরবর্তী:
		idx++;
		যদি (গুনতি + 1 < window)
			গুনতি++;
		যদি (idx >= window)
			idx = 0;
	}

	জন্য (i = 0; i < window; ++i) {
		free_delta_সূচক(array[i].সূচক);
		মুক্ত(array[i].তথ্য);
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

কাঠামো thread_params {
	pthread_t thread;
	কাঠামো object_entry **list;
	কাঠামো packing_region *regions;
	unsigned list_আকার;
	unsigned remaining;
	পূর্ণসংখ্যা window;
	পূর্ণসংখ্যা depth;
	পূর্ণসংখ্যা working;
	পূর্ণসংখ্যা তথ্য_ready;
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
	pthread_mutex_init(&cache_mutex, ফাঁকা);
	pthread_mutex_init(&progress_mutex, ফাঁকা);
	pthread_cond_init(&progress_cond, ফাঁকা);
}

স্থির শূন্য cleanup_threaded_অনুসন্ধান(শূন্য)
{
	pthread_cond_destroy(&progress_cond);
	pthread_mutex_destroy(&cache_mutex);
	pthread_mutex_destroy(&progress_mutex);
}

স্থির শূন্য *threaded_find_deltas(শূন্য *arg)
{
	কাঠামো thread_params *me = arg;

	progress_lock();
	যখন (me->remaining) {
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
		যখন (!me->তথ্য_ready)
			pthread_cond_wait(&me->cond, &me->mutex);
		me->তথ্য_ready = 0;
		pthread_mutex_unlock(&me->mutex);

		progress_lock();
	}
	progress_unlock();
	/* leave ->working 1 so that this doesn't get more work assigned */
	ফেরত ফাঁকা;
}

স্থির শূন্য ll_find_deltas(কাঠামো object_entry **list, unsigned list_আকার,
			   পূর্ণসংখ্যা window, পূর্ণসংখ্যা depth, unsigned *processed)
{
	কাঠামো thread_params *p;
	পূর্ণসংখ্যা i, ret, active_threads = 0;

	init_threaded_অনুসন্ধান();

	যদি (delta_অনুসন্ধান_threads <= 1) {
		find_deltas(list, &list_আকার, window, depth, processed);
		cleanup_threaded_অনুসন্ধান();
		ফেরত;
	}
	যদি (progress > pack_to_stdout)
		fprintf_ln(stderr, _("Delta compression using up to %d threads"),
			   delta_অনুসন্ধান_threads);
	CALLOC_ARRAY(p, delta_অনুসন্ধান_threads);

	/* Partition the work amongst work threads. */
	জন্য (i = 0; i < delta_অনুসন্ধান_threads; i++) {
		unsigned sub_আকার = list_আকার / (delta_অনুসন্ধান_threads - i);

		/* don't use too small segments or no deltas will be found */
		যদি (sub_আকার < 2*window && i+1 < delta_অনুসন্ধান_threads)
			sub_আকার = 0;

		p[i].window = window;
		p[i].depth = depth;
		p[i].processed = processed;
		p[i].working = 1;
		p[i].তথ্য_ready = 0;

		/* try to split chunks on "path" boundaries */
		যখন (sub_আকার && sub_আকার < list_আকার &&
		       list[sub_আকার]->hash &&
		       list[sub_আকার]->hash == list[sub_আকার-1]->hash)
			sub_আকার++;

		p[i].list = list;
		p[i].list_আকার = sub_আকার;
		p[i].remaining = sub_আকার;

		list += sub_আকার;
		list_আকার -= sub_আকার;
	}

	/* Start work threads. */
	জন্য (i = 0; i < delta_অনুসন্ধান_threads; i++) {
		যদি (!p[i].list_আকার)
			চালু;
		pthread_mutex_init(&p[i].mutex, ফাঁকা);
		pthread_cond_init(&p[i].cond, ফাঁকা);
		ret = pthread_create(&p[i].thread, ফাঁকা,
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
	যখন (active_threads) {
		কাঠামো thread_params *target = ফাঁকা;
		কাঠামো thread_params *victim = ফাঁকা;
		unsigned sub_আকার = 0;

		progress_lock();
		জন্য (;;) {
			জন্য (i = 0; !target && i < delta_অনুসন্ধান_threads; i++)
				যদি (!p[i].working)
					target = &p[i];
			যদি (target)
				বিরতি;
			pthread_cond_wait(&progress_cond, &progress_mutex);
		}

		জন্য (i = 0; i < delta_অনুসন্ধান_threads; i++)
			যদি (p[i].remaining > 2*window &&
			    (!victim || victim->remaining < p[i].remaining))
				victim = &p[i];
		যদি (victim) {
			sub_আকার = victim->remaining / 2;
			list = victim->list + victim->list_আকার - sub_আকার;
			যখন (sub_আকার && list[0]->hash &&
			       list[0]->hash == list[-1]->hash) {
				list++;
				sub_আকার--;
			}
			যদি (!sub_আকার) {
				/*
				 * It is possible for some "paths" to have
				 * so many objects that no hash boundary
				 * might be found.  Let's just steal the
				 * exact half in that case.
				 */
				sub_আকার = victim->remaining / 2;
				list -= sub_আকার;
			}
			target->list = list;
			victim->list_আকার -= sub_আকার;
			victim->remaining -= sub_আকার;
		}
		target->list_আকার = sub_আকার;
		target->remaining = sub_আকার;
		target->working = 1;
		progress_unlock();

		pthread_mutex_lock(&target->mutex);
		target->তথ্য_ready = 1;
		pthread_cond_signal(&target->cond);
		pthread_mutex_unlock(&target->mutex);

		যদি (!sub_আকার) {
			pthread_join(target->thread, ফাঁকা);
			pthread_cond_destroy(&target->cond);
			pthread_mutex_destroy(&target->mutex);
			active_threads--;
		}
	}
	cleanup_threaded_অনুসন্ধান();
	মুক্ত(p);
}

স্থির পূর্ণসংখ্যা obj_is_packed(ধ্রুবক কাঠামো object_id *oid)
{
	ফেরত packlist_find(&to_pack, oid) ||
		(reuse_packfile_bitmap &&
		 bitmap_walk_contains(bitmap_git, reuse_packfile_bitmap, oid));
}

স্থির শূন্য add_tag_chain(ধ্রুবক কাঠামো object_id *oid)
{
	কাঠামো tag *tag;

	/*
	 * We catch duplicates already in add_object_entry(), but we'd
	 * prefer to do this extra check to avoid having to parse the
	 * tag at all if we already know that it's being packed (e.g., if
	 * it was included via bitmaps, we would not have parsed it
	 * previously).
	 */
	যদি (obj_is_packed(oid))
		ফেরত;

	tag = lookup_tag(the_repository, oid);
	যখন (1) {
		যদি (!tag || parse_tag(the_repository, tag) || !tag->tagged)
			die(_("unable to pack objects reachable from tag %s"),
			    oid_to_hex(oid));

		add_object_entry(&tag->object.oid, OBJ_TAG, ফাঁকা, 0);

		যদি (tag->tagged->type != OBJ_TAG)
			ফেরত;

		tag = (কাঠামো tag *)tag->tagged;
	}
}

স্থির পূর্ণসংখ্যা add_ref_tag(ধ্রুবক কাঠামো reference *ref, শূন্য *cb_তথ্য UNUSED)
{
	কাঠামো object_id peeled;

	যদি (!reference_get_peeled_oid(the_repository, ref, &peeled) &&
	    obj_is_packed(&peeled))
		add_tag_chain(ref->oid);
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা should_attempt_deltas(কাঠামো object_entry *entry)
{
	যদি (DELTA(entry))
		/* This happens if we decided to reuse existing
		 * delta from a pack. "reuse_delta &&" is implied.
		 */
		ফেরত 0;

	যদি (!entry->type_valid ||
	    oe_আকার_less_than(&to_pack, entry, 50))
		ফেরত 0;

	যদি (entry->no_try_delta)
		ফেরত 0;

	যদি (!entry->preferred_base) {
		যদি (oe_type(entry) < 0)
			die(_("unable to get type of object %s"),
				oid_to_hex(&entry->idx.oid));
	} অন্যথা যদি (oe_type(entry) < 0) {
		/*
		 * This object is not found, but we
		 * don't have to include it anyway.
		 */
		ফেরত 0;
	}

	ফেরত 1;
}

স্থির শূন্য find_deltas_for_region(কাঠামো object_entry *list,
				   কাঠামো packing_region *region,
				   unsigned পূর্ণসংখ্যা *processed)
{
	কাঠামো object_entry **delta_list;
	unsigned পূর্ণসংখ্যা delta_list_nr = 0;

	ALLOC_ARRAY(delta_list, region->nr);
	জন্য (আকার_t i = 0; i < region->nr; i++) {
		কাঠামো object_entry *entry = list + region->start + i;
		যদি (should_attempt_deltas(entry))
			delta_list[delta_list_nr++] = entry;
	}

	QSORT(delta_list, delta_list_nr, type_আকার_sort);
	find_deltas(delta_list, &delta_list_nr, window, depth, processed);
	মুক্ত(delta_list);
}

স্থির শূন্য find_deltas_by_region(কাঠামো object_entry *list,
				  কাঠামো packing_region *regions,
				  আকার_t start, আকার_t nr)
{
	unsigned পূর্ণসংখ্যা processed = 0;
	আকার_t progress_nr;

	যদি (!nr)
		ফেরত;

	progress_nr = regions[nr - 1].start + regions[nr - 1].nr;

	যদি (progress)
		progress_state = start_progress(the_repository,
						_("Compressing objects by path"),
						progress_nr);

	যখন (nr--)
		find_deltas_for_region(list,
				       &regions[start++],
				       &processed);

	display_progress(progress_state, progress_nr);
	stop_progress(&progress_state);
}

স্থির শূন্য *threaded_find_deltas_by_path(শূন্য *arg)
{
	কাঠামো thread_params *me = arg;

	progress_lock();
	যখন (me->remaining) {
		যখন (me->remaining) {
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
		যখন (!me->তথ্য_ready)
			pthread_cond_wait(&me->cond, &me->mutex);
		me->তথ্য_ready = 0;
		pthread_mutex_unlock(&me->mutex);

		progress_lock();
	}
	progress_unlock();
	/* leave ->working 1 so that this doesn't get more work assigned */
	ফেরত ফাঁকা;
}

স্থির শূন্য ll_find_deltas_by_region(কাঠামো object_entry *list,
				     কাঠামো packing_region *regions,
				     uint32_t start, uint32_t nr)
{
	কাঠামো thread_params *p;
	পূর্ণসংখ্যা i, ret, active_threads = 0;
	unsigned পূর্ণসংখ্যা processed = 0;
	uint32_t progress_nr;
	init_threaded_অনুসন্ধান();

	যদি (!nr)
		ফেরত;

	progress_nr =  regions[nr - 1].start + regions[nr - 1].nr;
	যদি (delta_অনুসন্ধান_threads <= 1) {
		find_deltas_by_region(list, regions, start, nr);
		cleanup_threaded_অনুসন্ধান();
		ফেরত;
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
	জন্য (i = 0; i < delta_অনুসন্ধান_threads; i++) {
		unsigned sub_আকার = nr / (delta_অনুসন্ধান_threads - i);

		p[i].window = window;
		p[i].depth = depth;
		p[i].processed = &processed;
		p[i].working = 1;
		p[i].তথ্য_ready = 0;

		p[i].regions = regions;
		p[i].list_আকার = sub_আকার;
		p[i].remaining = sub_আকার;

		regions += sub_আকার;
		nr -= sub_আকার;
	}

	/* Start work threads. */
	জন্য (i = 0; i < delta_অনুসন্ধান_threads; i++) {
		যদি (!p[i].list_আকার)
			চালু;
		pthread_mutex_init(&p[i].mutex, ফাঁকা);
		pthread_cond_init(&p[i].cond, ফাঁকা);
		ret = pthread_create(&p[i].thread, ফাঁকা,
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
	যখন (active_threads) {
		কাঠামো thread_params *target = ফাঁকা;
		কাঠামো thread_params *victim = ফাঁকা;
		unsigned sub_আকার = 0;

		progress_lock();
		জন্য (;;) {
			জন্য (i = 0; !target && i < delta_অনুসন্ধান_threads; i++)
				যদি (!p[i].working)
					target = &p[i];
			যদি (target)
				বিরতি;
			pthread_cond_wait(&progress_cond, &progress_mutex);
		}

		জন্য (i = 0; i < delta_অনুসন্ধান_threads; i++)
			যদি (p[i].remaining > 2*window &&
			    (!victim || victim->remaining < p[i].remaining))
				victim = &p[i];
		যদি (victim) {
			sub_আকার = victim->remaining / 2;
			target->regions = victim->regions + victim->remaining - sub_আকার;
			victim->list_আকার -= sub_আকার;
			victim->remaining -= sub_আকার;
		}
		target->list_আকার = sub_আকার;
		target->remaining = sub_আকার;
		target->working = 1;
		progress_unlock();

		pthread_mutex_lock(&target->mutex);
		target->তথ্য_ready = 1;
		pthread_cond_signal(&target->cond);
		pthread_mutex_unlock(&target->mutex);

		যদি (!sub_আকার) {
			pthread_join(target->thread, ফাঁকা);
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

স্থির শূন্য prepare_pack(পূর্ণসংখ্যা window, পূর্ণসংখ্যা depth)
{
	কাঠামো object_entry **delta_list;
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
		ফেরত;

	যদি (path_walk)
		ll_find_deltas_by_region(to_pack.objects, to_pack.regions,
					 0, to_pack.nr_regions);

	ALLOC_ARRAY(delta_list, to_pack.nr_objects);
	nr_deltas = n = 0;

	জন্য (i = 0; i < to_pack.nr_objects; i++) {
		কাঠামো object_entry *entry = to_pack.objects + i;

		যদি (!should_attempt_deltas(entry))
			চালু;

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
		QSORT(delta_list, n, type_আকার_sort);
		ll_find_deltas(delta_list, n, window+1, depth, &nr_done);
		stop_progress(&progress_state);
		যদি (nr_done != nr_deltas)
			die(_("inconsistency with delta count"));
	}
	মুক্ত(delta_list);
}

স্থির পূর্ণসংখ্যা git_pack_config(ধ্রুবক অক্ষর *k, ধ্রুবক অক্ষর *v,
			   ধ্রুবক কাঠামো config_context *ctx, শূন্য *cb)
{
	যদি (!strcmp(k, "pack.window")) {
		window = git_config_int(k, v, ctx->kvi);
		ফেরত 0;
	}
	যদি (!strcmp(k, "pack.windowmemory")) {
		window_memory_limit = git_config_ulong(k, v, ctx->kvi);
		ফেরত 0;
	}
	যদি (!strcmp(k, "pack.depth")) {
		depth = git_config_int(k, v, ctx->kvi);
		ফেরত 0;
	}
	যদি (!strcmp(k, "pack.deltacachesize")) {
		max_delta_cache_আকার = git_config_আকার_t(k, v, ctx->kvi);
		ফেরত 0;
	}
	যদি (!strcmp(k, "pack.deltacachelimit")) {
		cache_max_small_delta_আকার = git_config_int(k, v, ctx->kvi);
		ফেরত 0;
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
		use_bitmap_সূচক_default = git_config_bool(k, v);
		ফেরত 0;
	}
	যদি (!strcmp(k, "pack.allowpackreuse")) {
		পূর্ণসংখ্যা res = git_parse_maybe_bool_text(v);
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
		ফেরত 0;
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
		ফেরত 0;
	}
	যদি (!strcmp(k, "pack.indexversion")) {
		pack_idx_opts.version = git_config_int(k, v, ctx->kvi);
		যদি (pack_idx_opts.version > 2)
			die(_("bad pack.indexVersion=%"PRIu32),
			    pack_idx_opts.version);
		ফেরত 0;
	}
	যদি (!strcmp(k, "pack.writereverseindex")) {
		যদি (git_config_bool(k, v))
			pack_idx_opts.flags |= WRITE_REV;
		অন্যথা
			pack_idx_opts.flags &= ~WRITE_REV;
		ফেরত 0;
	}
	যদি (!strcmp(k, "uploadpack.blobpackfileuri")) {
		কাঠামো configured_exclusion *ex;
		ধ্রুবক অক্ষর *oid_end, *pack_end;
		/*
		 * Stores the pack hash. This is not a true object ID, but is
		 * of the same form.
		 */
		কাঠামো object_id pack_hash;

		যদি (!v)
			ফেরত config_error_nonbool(k);

		ex = xmalloc(আকার(*ex));
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
	ফেরত git_default_config(k, v, ctx, cb);
}

/* Counters for trace2 output when in --stdin-packs mode. */
স্থির পূর্ণসংখ্যা stdin_packs_found_nr;
স্থির পূর্ণসংখ্যা stdin_packs_hints_nr;

স্থির পূর্ণসংখ্যা add_object_entry_from_pack(ধ্রুবক কাঠামো object_id *oid,
				      কাঠামো packed_git *p,
				      uint32_t pos,
				      শূন্য *তথ্য_2)
{
	off_t ofs;
	কাঠামো object_info oi = OBJECT_INFO_INIT;
	গণনা object_type type = OBJ_NONE;

	display_progress(progress_state, ++nr_seen);

	যদি (have_duplicate_entry(oid, 0))
		ফেরত 0;

	stdin_packs_found_nr++;

	ofs = nth_packed_object_offset(p, pos);

	oi.typep = &type;
	যদি (packed_object_info(ফাঁকা, p, ofs, &oi) < 0) {
		die(_("could not get type of object %s in pack %s"),
		    oid_to_hex(oid), p->pack_name);
	} অন্যথা যদি (type == OBJ_COMMIT) {
		কাঠামো rev_info *revs = তথ্য_2;
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
		add_pending_oid(revs, ফাঁকা, oid, 0);
	}

	যদি (!want_object_in_pack(oid, 0, &p, &ofs))
		ফেরত 0;

	create_object_entry(oid, type, 0, 0, 0, p, ofs);

	ফেরত 0;
}

স্থির শূন্য show_object_pack_hint(কাঠামো object *object, ধ্রুবক অক্ষর *name,
				  শূন্য *তথ্য)
{
	গণনা stdin_packs_mode mode = *(গণনা stdin_packs_mode *)তথ্য;
	যদি (mode == STDIN_PACKS_MODE_FOLLOW) {
		যদি (object->type == OBJ_BLOB &&
		    !odb_has_object(the_repository->objects, &object->oid, 0))
			ফেরত;
		add_object_entry(&object->oid, object->type, name, 0);
	} অন্যথা {
		কাঠামো object_entry *oe = packlist_find(&to_pack, &object->oid);
		যদি (!oe)
			ফেরত;

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

স্থির শূন্য show_commit_pack_hint(কাঠামো commit *commit, শূন্য *তথ্য)
{
	গণনা stdin_packs_mode mode = *(গণনা stdin_packs_mode *)তথ্য;

	যদি (mode == STDIN_PACKS_MODE_FOLLOW) {
		show_object_pack_hint((কাঠামো object *)commit, "", তথ্য);
		ফেরত;
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

কাঠামো stdin_pack_info {
	কাঠামো packed_git *p;
	গণনা stdin_pack_info_kind kind;
};

স্থির পূর্ণসংখ্যা pack_mtime_cmp(ধ্রুবক শূন্য *_a, ধ্রুবক শূন্য *_b)
{
	কাঠামো stdin_pack_info *a = ((ধ্রুবক কাঠামো string_list_item*)_a)->util;
	কাঠামো stdin_pack_info *b = ((ধ্রুবক কাঠামো string_list_item*)_b)->util;

	/*
	 * order packs by descending mtime so that objects are laid out
	 * roughly as newest-to-oldest
	 */
	যদি (a->p->mtime < b->p->mtime)
		ফেরত 1;
	অন্যথা যদি (b->p->mtime < a->p->mtime)
		ফেরত -1;
	অন্যথা
		ফেরত 0;
}

স্থির পূর্ণসংখ্যা stdin_packs_include_check_obj(কাঠামো object *obj, শূন্য *তথ্য UNUSED)
{
	ফেরত !has_object_kept_pack(to_pack.repo, &obj->oid,
				     KEPT_PACK_IN_CORE);
}

স্থির পূর্ণসংখ্যা stdin_packs_include_check(কাঠামো commit *commit, শূন্য *তথ্য)
{
	ফেরত stdin_packs_include_check_obj((কাঠামো object *)commit, তথ্য);
}

স্থির শূন্য stdin_packs_add_pack_entries(কাঠামো strmap *packs,
					 কাঠামো rev_info *revs)
{
	কাঠামো string_list চাবিগুলি = STRING_LIST_INIT_NODUP;
	কাঠামো string_list_item *item;
	কাঠামো hashmap_iter iter;
	কাঠামো strmap_entry *entry;

	strmap_for_each_entry(packs, &iter, entry) {
		কাঠামো stdin_pack_info *info = entry->মান;
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
		কাঠামো stdin_pack_info *info = item->util;

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

স্থির শূন্য stdin_packs_read_প্রবেশ(কাঠামো rev_info *revs,
				   গণনা stdin_packs_mode mode)
{
	কাঠামো strbuf buf = STRBUF_INIT;
	কাঠামো strmap packs = STRMAP_INIT;
	কাঠামো packed_git *p;

	যখন (strbuf_getline(&buf, stdin) != EOF) {
		কাঠামো stdin_pack_info *info;
		গণনা stdin_pack_info_kind kind = STDIN_PACK_INCLUDE;
		ধ্রুবক অক্ষর *চাবি = buf.buf;

		যদি (!*চাবি)
			চালু;
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
		কাঠামো stdin_pack_info *info;

		info = strmap_get(&packs, pack_basename(p));
		যদি (!info)
			চালু;

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

স্থির শূন্য add_unreachable_loose_objects(কাঠামো rev_info *revs);

স্থির শূন্য read_stdin_packs(কাঠামো repository *repo,
			     গণনা stdin_packs_mode mode, পূর্ণসংখ্যা rev_list_unpacked)
{
	পূর্ণসংখ্যা পূর্ব_fetch_if_missing = repo->fetch_if_missing;
	কাঠামো rev_info revs;

	/*
	 * The revision walk may hit objects that are promised, only. As the
	 * walk is best-effort though we don't want to perform backfill fetches
	 * for them.
	 */
	repo->fetch_if_missing = 0;

	repo_init_revisions(repo, &revs, ফাঁকা);
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

	trace2_তথ্য_intmax("pack-objects", the_repository, "stdin_packs_found",
			   stdin_packs_found_nr);
	trace2_তথ্য_intmax("pack-objects", the_repository, "stdin_packs_hints",
			   stdin_packs_hints_nr);

	repo->fetch_if_missing = পূর্ব_fetch_if_missing;
}

স্থির শূন্য add_cruft_object_entry(ধ্রুবক কাঠামো object_id *oid, গণনা object_type type,
				   কাঠামো packed_git *pack, off_t offset,
				   ধ্রুবক অক্ষর *name, uint32_t mtime)
{
	কাঠামো object_entry *entry;

	display_progress(progress_state, ++nr_seen);

	entry = packlist_find(&to_pack, oid);
	যদি (entry) {
		যদি (name) {
			entry->hash = pack_name_hash_fn(name);
			entry->no_try_delta = no_try_delta(name);
		}
	} অন্যথা {
		যদি (!want_object_in_pack_mtime(oid, 0, &pack, &offset, mtime))
			ফেরত;
		যদি (!pack && type == OBJ_BLOB) {
			কাঠামো odb_source_files *files =
				odb_source_files_downcast(the_repository->objects->source);
			পূর্ণসংখ্যা found = 0;

			জন্য (কাঠামো odb_files_dir *dir = files->dirs; !found && dir; dir = dir->পরবর্তী)
				যদি (!odb_source_read_object_info(&dir->loose->base, oid, ফাঁকা, 0, ফাঁকা))
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
				ফেরত;
		}

		entry = create_object_entry(oid, type, pack_name_hash_fn(name),
					    0, name && no_try_delta(name),
					    pack, offset);
	}

	যদি (mtime > oe_cruft_mtime(&to_pack, entry))
		oe_set_cruft_mtime(&to_pack, entry, mtime);
	ফেরত;
}

স্থির শূন্য show_cruft_object(কাঠামো object *obj, ধ্রুবক অক্ষর *name, শূন্য *তথ্য UNUSED)
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
	add_cruft_object_entry(&obj->oid, obj->type, ফাঁকা, 0, name, cruft_expiration);
}

স্থির শূন্য show_cruft_commit(কাঠামো commit *commit, শূন্য *তথ্য)
{
	show_cruft_object((কাঠামো object*)commit, ফাঁকা, তথ্য);
}

স্থির পূর্ণসংখ্যা cruft_include_check_obj(কাঠামো object *obj, শূন্য *তথ্য UNUSED)
{
	ফেরত !has_object_kept_pack(to_pack.repo, &obj->oid, KEPT_PACK_IN_CORE);
}

স্থির পূর্ণসংখ্যা cruft_include_check(কাঠামো commit *commit, শূন্য *তথ্য)
{
	ফেরত cruft_include_check_obj((কাঠামো object*)commit, তথ্য);
}

স্থির শূন্য set_cruft_mtime(ধ্রুবক কাঠামো object *object,
			    কাঠামো packed_git *pack,
			    off_t offset, time_t mtime)
{
	add_cruft_object_entry(&object->oid, object->type, pack, offset, ফাঁকা,
			       mtime);
}

স্থির শূন্য mark_pack_kept_in_core(কাঠামো string_list *packs, unsigned keep)
{
	কাঠামো string_list_item *item = ফাঁকা;
	for_each_string_list_item(item, packs) {
		কাঠামো packed_git *p = item->util;
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
	add_unreachable_loose_objects(ফাঁকা);

	stop_progress(&progress_state);
}

স্থির শূন্য enumerate_and_traverse_cruft_objects(কাঠামো string_list *fresh_packs)
{
	কাঠামো packed_git *p;
	কাঠামো rev_info revs;
	পূর্ণসংখ্যা ret;

	repo_init_revisions(the_repository, &revs, ফাঁকা);

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
	traverse_commit_list(&revs, show_cruft_commit, show_cruft_object, ফাঁকা);

	stop_progress(&progress_state);
	release_revisions(&revs);
}

স্থির শূন্য read_cruft_objects(শূন্য)
{
	কাঠামো strbuf buf = STRBUF_INIT;
	কাঠামো string_list discard_packs = STRING_LIST_INIT_DUP;
	কাঠামো string_list fresh_packs = STRING_LIST_INIT_DUP;
	কাঠামো packed_git *p;

	ignore_packed_keep_in_core = 1;

	যখন (strbuf_getline(&buf, stdin) != EOF) {
		যদি (!buf.len)
			চালু;

		যদি (*buf.buf == '-')
			string_list_append(&discard_packs, buf.buf + 1);
		অন্যথা
			string_list_append(&fresh_packs, buf.buf);
	}

	string_list_sort(&discard_packs);
	string_list_sort(&fresh_packs);

	repo_for_each_pack(the_repository, p) {
		ধ্রুবক অক্ষর *pack_name = pack_basename(p);
		কাঠামো string_list_item *item;

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
	অক্ষর সারি[GIT_MAX_HEXSZ + 1 + PATH_MAX + 2];
	কাঠামো object_id oid;
	ধ্রুবক অক্ষর *p;

	জন্য (;;) {
		যদি (!fgets(সারি, আকার(সারি), stdin)) {
			যদি (feof(stdin))
				বিরতি;
			যদি (!ferror(stdin))
				BUG("fgets returned NULL, not EOF, not error!");
			যদি (errno != EINTR)
				die_errno("fgets");
			clearerr(stdin);
			চালু;
		}
		যদি (সারি[0] == '-') {
			যদি (get_oid_hex(সারি+1, &oid))
				die(_("expected edge object ID, got garbage:\n %s"),
				    সারি);
			add_preferred_base(&oid);
			চালু;
		}
		যদি (parse_oid_hex(সারি, &oid, &p))
			die(_("expected object ID, got garbage:\n %s"), সারি);

		add_preferred_base_object(p + 1);
		add_object_entry(&oid, OBJ_NONE, p + 1, 0);
	}
}

স্থির শূন্য show_commit(কাঠামো commit *commit, শূন্য *তথ্য UNUSED)
{
	add_object_entry(&commit->object.oid, OBJ_COMMIT, ফাঁকা, 0);

	যদি (write_bitmap_সূচক)
		সূচক_commit_for_bitmap(commit);

	যদি (use_delta_islands)
		propagate_island_marks(the_repository, commit);
}

স্থির শূন্য show_object(কাঠামো object *obj, ধ্রুবক অক্ষর *name,
			শূন্য *তথ্য UNUSED)
{
	add_preferred_base_object(name);
	add_object_entry(&obj->oid, obj->type, name, 0);

	যদি (use_delta_islands)
		record_বৃক্ষ_depth(&obj->oid, name);
}

স্থির শূন্য show_object__ma_allow_any(কাঠামো object *obj, ধ্রুবক অক্ষর *name, শূন্য *তথ্য)
{
	প্রতিজ্ঞা(arg_missing_action == MA_ALLOW_ANY);

	/*
	 * Quietly ignore ALL missing objects.  This avoids problems with
	 * staging them now and getting an odd error later.
	 */
	যদি (!odb_has_object(the_repository->objects, &obj->oid, 0))
		ফেরত;

	show_object(obj, name, তথ্য);
}

স্থির শূন্য show_object__ma_allow_promisor(কাঠামো object *obj, ধ্রুবক অক্ষর *name, শূন্য *তথ্য)
{
	প্রতিজ্ঞা(arg_missing_action == MA_ALLOW_PROMISOR);

	/*
	 * Quietly ignore EXPECTED missing objects.  This avoids problems with
	 * staging them now and getting an odd error later.
	 */
	যদি (!odb_has_object(the_repository->objects, &obj->oid, 0) &&
	    is_promisor_object(to_pack.repo, &obj->oid))
		ফেরত;

	show_object(obj, name, তথ্য);
}

স্থির পূর্ণসংখ্যা option_parse_missing_action(ধ্রুবক কাঠামো option *opt,
				       ধ্রুবক অক্ষর *arg, পূর্ণসংখ্যা unset)
{
	কাঠামো repository *repo = opt->মান;

	প্রতিজ্ঞা(arg);
	প্রতিজ্ঞা(!unset);

	যদি (!strcmp(arg, "error")) {
		arg_missing_action = MA_ERROR;
		fn_show_object = show_object;
		ফেরত 0;
	}

	যদি (!strcmp(arg, "allow-any")) {
		arg_missing_action = MA_ALLOW_ANY;
		repo->fetch_if_missing = 0;
		fn_show_object = show_object__ma_allow_any;
		ফেরত 0;
	}

	যদি (!strcmp(arg, "allow-promisor")) {
		arg_missing_action = MA_ALLOW_PROMISOR;
		repo->fetch_if_missing = 0;
		fn_show_object = show_object__ma_allow_promisor;
		ফেরত 0;
	}

	die(_("invalid value for '%s': '%s'"), "--missing", arg);
	ফেরত 0;
}

স্থির শূন্য show_edge(কাঠামো commit *commit)
{
	add_preferred_base(&commit->object.oid);
}

স্থির পূর্ণসংখ্যা add_object_in_unpacked_pack(ধ্রুবক কাঠামো object_id *oid,
				       কাঠামো object_info *oi,
				       শূন্য *তথ্য UNUSED)
{
	যদি (cruft) {
		add_cruft_object_entry(oid, OBJ_NONE, oi->source_infop->u.packed.pack,
				       oi->source_infop->u.packed.offset, ফাঁকা,
				       *oi->mtimep);
	} অন্যথা {
		add_object_entry(oid, OBJ_NONE, "", 0);
	}
	ফেরত 0;
}

স্থির শূন্য add_objects_in_unpacked_packs(শূন্য)
{
	কাঠামো odb_source_files *files =
		odb_source_files_downcast(to_pack.repo->objects->source);
	time_t mtime;
	কাঠামো odb_for_each_object_options opts = {
		.flags = ODB_FOR_EACH_OBJECT_PACK_ORDER |
			 ODB_FOR_EACH_OBJECT_LOCAL_ONLY |
			 ODB_FOR_EACH_OBJECT_SKIP_IN_CORE_KEPT_PACKS |
			 ODB_FOR_EACH_OBJECT_SKIP_ON_DISK_KEPT_PACKS,
	};
	কাঠামো odb_source_info source_info;
	কাঠামো object_info oi = {
		.mtimep = &mtime,
		.source_infop = &source_info,
	};

	জন্য (কাঠামো odb_files_dir *dir = files->dirs; dir; dir = dir->পরবর্তী) {
		যদি (!dir->local)
			চালু;

		যদি (odb_source_for_each_object(&dir->packed->base, &oi,
					       add_object_in_unpacked_pack, ফাঁকা, &opts))
			die(_("cannot open pack index"));
	}
}

স্থির পূর্ণসংখ্যা add_loose_object(ধ্রুবক কাঠামো object_id *oid, ধ্রুবক অক্ষর *path,
			    শূন্য *তথ্য)
{
	কাঠামো rev_info *revs = তথ্য;
	গণনা object_type type = odb_read_object_info(the_repository->objects, oid, ফাঁকা);

	যদি (type < 0) {
		warning(_("loose object at %s could not be examined"), path);
		ফেরত 0;
	}

	যদি (cruft) {
		কাঠামো stat st;
		যদি (stat(path, &st) < 0) {
			যদি (errno == ENOENT)
				ফেরত 0;
			ফেরত error_errno("unable to stat %s", oid_to_hex(oid));
		}

		add_cruft_object_entry(oid, type, ফাঁকা, 0, ফাঁকা,
				       st.st_mtime);
	} অন্যথা {
		add_object_entry(oid, type, "", 0);
	}

	যদি (revs && type == OBJ_COMMIT)
		add_pending_oid(revs, ফাঁকা, oid, 0);

	ফেরত 0;
}

/*
 * We actually don't even have to worry about reachability here.
 * add_object_entry will weed out duplicates, so we just add every
 * loose object we find.
 */
স্থির শূন্য add_unreachable_loose_objects(কাঠামো rev_info *revs)
{
	for_each_loose_file_in_source(the_repository->objects->source,
				      add_loose_object, ফাঁকা, ফাঁকা, revs);
}

স্থির পূর্ণসংখ্যা has_sha1_pack_kept_or_nonlocal(ধ্রুবক কাঠামো object_id *oid)
{
	স্থির কাঠামো packed_git *last_found = ফাঁকা;
	কাঠামো packed_git *p;

	যদি (last_found && find_pack_entry_one(oid, last_found))
		ফেরত 1;

	repo_for_each_pack(the_repository, p) {
		/*
		 * We have already checked `last_found`, so there is no need to
		 * re-check here.
		 */
		যদি (p == last_found)
			চালু;

		যদি ((!p->pack_local || p->pack_keep || p->pack_keep_in_core) &&
		    find_pack_entry_one(oid, p)) {
			last_found = p;
			ফেরত 1;
		}
	}

	ফেরত 0;
}

/*
 * Store a list of sha1s that are should not be discarded
 * because they are either written too recently, or are
 * reachable from another object that was.
 *
 * This is filled by get_object_list.
 */
স্থির কাঠামো oid_array recent_objects;

স্থির পূর্ণসংখ্যা loosened_object_can_be_discarded(ধ্রুবক কাঠামো object_id *oid,
					    timestamp_t mtime)
{
	যদি (!unpack_unreachable_expiration)
		ফেরত 0;
	যদি (mtime > unpack_unreachable_expiration)
		ফেরত 0;
	যদি (oid_array_lookup(&recent_objects, oid) >= 0)
		ফেরত 0;
	ফেরত 1;
}

স্থির পূর্ণসংখ্যা force_object_loose(কাঠামো odb_source *source,
			      ধ্রুবক কাঠামো object_id *oid,
			      ধ্রুবক time_t *mtime)
{
	কাঠামো odb_source_files *files = odb_source_files_downcast(source);
	ধ্রুবক কাঠামো git_hash_algo *compat = source->odb->repo->compat_hash_algo;
	কাঠামো object_info oi = OBJECT_INFO_INIT;
	কাঠামো object_id compat_oid, *compat_oid_p = ফাঁকা;
	গণনা object_type type;
	শূন্য *buf = ফাঁকা;
	আকার_t len;
	পূর্ণসংখ্যা ret;

	জন্য (কাঠামো odb_files_dir *dir = files->dirs; dir; dir = dir->পরবর্তী)
		যদি (!odb_source_read_object_info(&dir->loose->base, oid, ফাঁকা, 0, ফাঁকা))
			ফেরত 0;

	oi.typep = &type;
	oi.sizep = &len;
	oi.contentp = &buf;
	যদি (odb_read_object_info_extended(source->odb, oid, &oi, 0)) {
		ret = error(_("cannot read object for %s"), oid_to_hex(oid));
		যাও out;
	}

	যদি (compat) {
		যদি (repo_oid_to_algop(source->odb->repo, oid, compat, &compat_oid)) {
			ret = error(_("cannot map object %s to %s"),
				    oid_to_hex(oid), compat->name);
			যাও out;
		}

		compat_oid_p = &compat_oid;
	}

	ret = odb_source_write_object(&files->dirs->loose->base, buf, len, type, oid,
				      compat_oid_p, mtime, 0);

out:
	মুক্ত(buf);
	ফেরত ret;
}

স্থির শূন্য loosen_unused_packed_objects(শূন্য)
{
	কাঠামো packed_git *p;
	uint32_t i;
	uint32_t loosened_objects_nr = 0;
	কাঠামো object_id oid;

	repo_for_each_pack(the_repository, p) {
		যদি (!p->pack_local || p->pack_keep || p->pack_keep_in_core)
			চালু;

		যদি (open_pack_সূচক(p))
			die(_("cannot open pack index"));

		জন্য (i = 0; i < p->অঙ্ক_objects; i++) {
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

	trace2_তথ্য_intmax("pack-objects", the_repository,
			   "loosen_unused_packed_objects/loosened", loosened_objects_nr);
}

/*
 * This tracks any options which pack-reuse code expects to be on, or which a
 * reader of the pack might not understand, and which would therefore prevent
 * blind reuse of what we have on disk.
 */
স্থির পূর্ণসংখ্যা pack_options_allow_reuse(শূন্য)
{
	ফেরত allow_pack_reuse != NO_PACK_REUSE &&
	       pack_to_stdout &&
	       !ignore_packed_keep_on_disk &&
	       !ignore_packed_keep_in_core &&
	       (!local || !have_non_local_packs) &&
	       !incremental;
}

স্থির পূর্ণসংখ্যা get_object_list_from_bitmap(কাঠামো rev_info *revs)
{
	যদি (!(bitmap_git = prepare_bitmap_walk(revs, 0)))
		ফেরত -1;

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

		nr_ফলাফল += reuse_packfile_objects;
		nr_seen += reuse_packfile_objects;
		display_progress(progress_state, nr_seen);
	}

	traverse_bitmap_commit_list(bitmap_git, revs,
				    &add_object_entry_from_bitmap);
	ফেরত 0;
}

স্থির শূন্য record_recent_object(কাঠামো object *obj,
				 ধ্রুবক অক্ষর *name UNUSED,
				 শূন্য *তথ্য UNUSED)
{
	oid_array_append(&recent_objects, &obj->oid);
}

স্থির শূন্য record_recent_commit(কাঠামো commit *commit, শূন্য *তথ্য UNUSED)
{
	oid_array_append(&recent_objects, &commit->object.oid);
}

স্থির পূর্ণসংখ্যা mark_bitmap_preferred_tip(ধ্রুবক কাঠামো reference *ref, শূন্য *তথ্য UNUSED)
{
	ধ্রুবক কাঠামো object_id *maybe_peeled = ref->oid;
	কাঠামো object_id peeled;
	কাঠামো object *object;

	যদি (!reference_get_peeled_oid(the_repository, ref, &peeled))
		maybe_peeled = &peeled;

	object = parse_object_or_die(the_repository, maybe_peeled, ref->name);
	যদি (object->type == OBJ_COMMIT)
		object->flags |= NEEDS_BITMAP;

	ফেরত 0;
}

স্থির অন্তঃরেখ পূর্ণসংখ্যা is_oid_uninteresting(কাঠামো repository *repo,
				       কাঠামো object_id *oid)
{
	কাঠামো object *o = lookup_object(repo, oid);
	ফেরত !o || (o->flags & UNINTERESTING);
}

স্থির পূর্ণসংখ্যা add_objects_by_path(ধ্রুবক অক্ষর *path,
			       কাঠামো oid_array *oids,
			       গণনা object_type type,
			       শূন্য *তথ্য)
{
	আকার_t oe_start = to_pack.nr_objects;
	আকার_t oe_end;
	unsigned পূর্ণসংখ্যা *processed = তথ্য;

	/*
	 * First, add all objects to the packing data, including the ones
	 * marked UNINTERESTING (translated to 'exclude') as they can be
	 * used as delta bases.
	 */
	জন্য (আকার_t i = 0; i < oids->nr; i++) {
		পূর্ণসংখ্যা exclude;
		কাঠামো object_info oi = OBJECT_INFO_INIT;
		কাঠামো object_id *oid = &oids->oid[i];

		/* Skip objects that do not exist locally. */
		যদি ((exclude_promisor_objects || arg_missing_action != MA_ERROR) &&
		    odb_read_object_info_extended(the_repository->objects, oid, &oi,
						  OBJECT_INFO_FOR_PREFETCH) < 0)
			চালু;

		exclude = is_oid_uninteresting(the_repository, oid);

		যদি (exclude && !thin)
			চালু;

		add_object_entry(oid, type, path, exclude);

		যদি (type == OBJ_COMMIT) {
			কাঠামো commit *commit;

			যদি (!write_bitmap_সূচক && !use_delta_islands)
				চালু;

			commit = lookup_commit(the_repository, oid);
			যদি (!commit)
				die(_("could not find commit %s"), oid_to_hex(oid));
			যদি (write_bitmap_সূচক)
				সূচক_commit_for_bitmap(commit);
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
		ফেরত 0;

	ALLOC_GROW(to_pack.regions,
		   to_pack.nr_regions + 1,
		   to_pack.nr_regions_alloc);

	to_pack.regions[to_pack.nr_regions].start = oe_start;
	to_pack.regions[to_pack.nr_regions].nr = oe_end - oe_start;
	to_pack.nr_regions++;

	*processed += oids->nr;
	display_progress(progress_state, *processed);

	ফেরত 0;
}

স্থির পূর্ণসংখ্যা get_object_list_path_walk(কাঠামো rev_info *revs)
{
	কাঠামো path_walk_info info = PATH_WALK_INFO_INIT;
	unsigned পূর্ণসংখ্যা processed = 0;
	পূর্ণসংখ্যা ফলাফল;

	info.revs = revs;
	info.path_fn = add_objects_by_path;
	info.path_fn_তথ্য = &processed;

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
	ফলাফল = walk_objects_by_path(&info);
	trace2_region_leave("pack-objects", "path-walk", revs->repo);

	path_walk_info_clear(&info);

	ফেরত ফলাফল;
}

স্থির শূন্য get_object_list(কাঠামো rev_info *revs, কাঠামো strvec *argv)
{
	কাঠামো setup_revision_opt s_r_opt = {
		.allow_exclude_promisor_objects = 1,
	};
	কাঠামো repo_config_মানসমূহ *cfg = repo_config_মানসমূহ(the_repository);
	অক্ষর সারি[1000];
	পূর্ণসংখ্যা flags = 0;
	পূর্ণসংখ্যা save_warning;

	save_commit_বাফার = 0;
	setup_revisions_from_strvec(argv, revs, &s_r_opt);

	/* make sure shallows are read */
	is_repository_shallow(the_repository);

	save_warning = cfg->warn_on_object_refname_ambiguity;
	cfg->warn_on_object_refname_ambiguity = 0;

	যখন (fgets(সারি, আকার(সারি), stdin) != ফাঁকা) {
		পূর্ণসংখ্যা len = strlen(সারি);
		যদি (len && সারি[len - 1] == '\n')
			সারি[--len] = 0;
		যদি (!len)
			বিরতি;
		যদি (*সারি == '-') {
			যদি (!strcmp(সারি, "--not")) {
				flags ^= UNINTERESTING;
				write_bitmap_সূচক = 0;
				চালু;
			}
			যদি (starts_with(সারি, "--shallow ")) {
				কাঠামো object_id oid;
				যদি (get_oid_hex(সারি + 10, &oid))
					die("not an object name '%s'", সারি + 10);
				register_shallow(the_repository, &oid);
				use_bitmap_সূচক = 0;
				চালু;
			}
			die(_("not a rev '%s'"), সারি);
		}
		যদি (handle_revision_arg(সারি, revs, flags, REVARG_CANNOT_BE_FILENAME))
			die(_("bad revision '%s'"), সারি);
	}

	cfg->warn_on_object_refname_ambiguity = save_warning;

	যদি (use_bitmap_সূচক && !get_object_list_from_bitmap(revs))
		ফেরত;

	যদি (use_delta_islands)
		load_delta_islands(the_repository, progress);

	যদি (write_bitmap_সূচক)
		for_each_preferred_bitmap_tip(the_repository, mark_bitmap_preferred_tip,
					      ফাঁকা);

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
				ফাঁকা);
	}

	যদি (unpack_unreachable_expiration) {
		revs->ignore_missing_links = 1;
		যদি (add_unseen_recent_objects_to_traversal(revs,
				unpack_unreachable_expiration, ফাঁকা, 0))
			die(_("unable to add recent objects"));
		যদি (prepare_revision_walk(revs))
			die(_("revision walk setup failed"));
		traverse_commit_list(revs, record_recent_commit,
				     record_recent_object, ফাঁকা);
	}

	যদি (keep_unreachable)
		add_objects_in_unpacked_packs();
	যদি (pack_loose_unreachable)
		add_unreachable_loose_objects(ফাঁকা);
	যদি (unpack_unreachable)
		loosen_unused_packed_objects();

	oid_array_clear(&recent_objects);
}

স্থির শূন্য add_extra_kept_packs(ধ্রুবক কাঠামো string_list *names)
{
	কাঠামো packed_git *p;

	যদি (!names->nr)
		ফেরত;

	repo_for_each_pack(the_repository, p) {
		ধ্রুবক অক্ষর *name = basename(p->pack_name);
		পূর্ণসংখ্যা i;

		যদি (!p->pack_local)
			চালু;

		জন্য (i = 0; i < names->nr; i++)
			যদি (!fspathcmp(name, names->বস্তুসমূহ[i].string))
				বিরতি;

		যদি (i < names->nr) {
			p->pack_keep_in_core = 1;
			ignore_packed_keep_in_core = 1;
			চালু;
		}
	}
}

স্থির পূর্ণসংখ্যা option_parse_quiet(ধ্রুবক কাঠামো option *opt, ধ্রুবক অক্ষর *arg,
			      পূর্ণসংখ্যা unset)
{
	পূর্ণসংখ্যা *val = opt->মান;

	BUG_ON_OPT_ARG(arg);

	যদি (!unset)
		*val = 0;
	অন্যথা যদি (!*val)
		*val = 1;
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা option_parse_সূচক_version(ধ্রুবক কাঠামো option *opt,
				      ধ্রুবক অক্ষর *arg, পূর্ণসংখ্যা unset)
{
	কাঠামো pack_idx_option *popts = opt->মান;
	অক্ষর *c;
	ধ্রুবক অক্ষর *val = arg;

	BUG_ON_OPT_NEG(unset);

	popts->version = strtoul(val, &c, 10);
	যদি (popts->version > 2)
		die(_("unsupported index version %s"), val);
	যদি (*c == ',' && c[1])
		popts->off32_limit = strtoul(c+1, &c, 0);
	যদি (*c || popts->off32_limit & 0x80000000)
		die(_("bad index version '%s'"), val);
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা option_parse_unpack_unreachable(ধ্রুবক কাঠামো option *opt UNUSED,
					   ধ্রুবক অক্ষর *arg, পূর্ণসংখ্যা unset)
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
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা option_parse_cruft_expiration(ধ্রুবক কাঠামো option *opt UNUSED,
					 ধ্রুবক অক্ষর *arg, পূর্ণসংখ্যা unset)
{
	যদি (unset) {
		cruft = 0;
		cruft_expiration = 0;
	} অন্যথা {
		cruft = 1;
		যদি (arg)
			cruft_expiration = approxidate(arg);
	}
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা is_not_in_promisor_pack_obj(কাঠামো object *obj, শূন্য *তথ্য UNUSED)
{
	কাঠামো odb_source_info source_info;
	কাঠামো object_info info = {
		.source_infop = &source_info,
	};

	যদি (odb_read_object_info_extended(the_repository->objects, &obj->oid, &info, 0))
		BUG("should_include_obj should only be called on existing objects");
	ফেরত source_info.source->type != ODB_SOURCE_PACKED || !source_info.u.packed.pack->pack_promisor;
}

স্থির পূর্ণসংখ্যা is_not_in_promisor_pack(কাঠামো commit *commit, শূন্য *তথ্য) {
	ফেরত is_not_in_promisor_pack_obj((কাঠামো object *) commit, তথ্য);
}

স্থির পূর্ণসংখ্যা parse_stdin_packs_mode(ধ্রুবক কাঠামো option *opt, ধ্রুবক অক্ষর *arg,
				  পূর্ণসংখ্যা unset)
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

	ফেরত 0;
}

পূর্ণসংখ্যা cmd_pack_objects(পূর্ণসংখ্যা argc,
		     ধ্রুবক অক্ষর **argv,
		     ধ্রুবক অক্ষর *prefix,
		     কাঠামো repository *repo)
{
	পূর্ণসংখ্যা use_internal_rev_list = 0;
	পূর্ণসংখ্যা all_progress_implied = 0;
	কাঠামো strvec rp = STRVEC_INIT;
	পূর্ণসংখ্যা rev_list_unpacked = 0, rev_list_all = 0, rev_list_reflog = 0;
	পূর্ণসংখ্যা rev_list_সূচক = 0;
	গণনা stdin_packs_mode stdin_packs = STDIN_PACKS_MODE_NONE;
	কাঠামো string_list keep_pack_list = STRING_LIST_INIT_NODUP;
	কাঠামো list_objects_filter_options filter_options =
		LIST_OBJECTS_FILTER_INIT;
	কাঠামো repo_config_মানসমূহ *cfg = repo_config_মানসমূহ(the_repository);

	কাঠামো option pack_objects_options[] = {
		OPT_CALLBACK_F('q', "quiet", &progress, ফাঁকা,
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
		  PARSE_OPT_NONEG, option_parse_সূচক_version),
		OPT_UNSIGNED(0, "max-pack-size", &pack_আকার_limit,
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
		OPT_SET_INT_F(0, "indexed-objects", &rev_list_সূচক,
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
		OPT_CALLBACK_F(0, "unpack-unreachable", ফাঁকা, N_("time"),
		  N_("unpack unreachable objects newer than <time>"),
		  PARSE_OPT_OPTARG, option_parse_unpack_unreachable),
		OPT_BOOL(0, "cruft", &cruft, N_("create a cruft pack")),
		OPT_CALLBACK_F(0, "cruft-expiration", ফাঁকা, N_("time"),
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
		OPT_BOOL(0, "use-bitmap-index", &use_bitmap_সূচক,
			 N_("use a bitmap index if available to speed up counting objects")),
		OPT_SET_INT(0, "write-bitmap-index", &write_bitmap_সূচক,
			    N_("write a bitmap index together with the pack index"),
			    WRITE_BITMAP_TRUE),
		OPT_SET_INT_F(0, "write-bitmap-index-quiet",
			      &write_bitmap_সূচক,
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
	repo_config(the_repository, git_pack_config, ফাঁকা);
	যদি (git_env_bool(GIT_TEST_NO_WRITE_REV_সূচক, 0))
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
		যদি (use_bitmap_সূচক > 0 ||
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
	যদি (cache_max_small_delta_আকার >= (1U << OE_Z_DELTA_BITS)) {
		warning(_("pack.deltaCacheLimit is too high, forcing %d"),
			(1U << OE_Z_DELTA_BITS) - 1);
		cache_max_small_delta_আকার = (1U << OE_Z_DELTA_BITS) - 1;
	}
	যদি (window < 0)
		window = 0;

	strvec_ঠেলো(&rp, "pack-objects");

	যদি (path_walk) {
		ধ্রুবক অক্ষর *option = ফাঁকা;
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
	যদি (rev_list_সূচক) {
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
	যদি (!pack_to_stdout && !pack_আকার_limit)
		pack_আকার_limit = pack_আকার_limit_cfg;
	যদি (pack_to_stdout && pack_আকার_limit)
		die(_("--max-pack-size cannot be used to build a pack for transfer"));
	যদি (pack_আকার_limit && pack_আকার_limit < 1024*1024) {
		warning(_("minimum pack size limit is 1 MiB"));
		pack_আকার_limit = 1024*1024;
	}

	যদি (!pack_to_stdout && thin)
		die(_("--thin cannot be used to build an indexable pack"));

	die_for_incompatible_opt2(keep_unreachable, "--keep-unreachable",
				  unpack_unreachable, "--unpack-unreachable");
	যদি (!rev_list_all || !rev_list_reflog || !rev_list_সূচক)
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
		use_bitmap_সূচক_default = 0;

	যদি (use_bitmap_সূচক < 0)
		use_bitmap_সূচক = use_bitmap_সূচক_default;

	/* "hard" reasons not to use bitmaps; these just won't work at all */
	যদি (!use_internal_rev_list || (!pack_to_stdout && write_bitmap_সূচক) || is_repository_shallow(the_repository))
		use_bitmap_সূচক = 0;

	যদি (pack_to_stdout || !rev_list_all)
		write_bitmap_সূচক = 0;

	যদি (name_hash_version < 0)
		name_hash_version = (পূর্ণসংখ্যা)git_env_ulong("GIT_TEST_NAME_HASH_VERSION", 1);

	validate_name_hash_version();

	যদি (use_delta_islands)
		strvec_ঠেলো(&rp, "--topo-order");

	যদি (progress && all_progress_implied)
		progress = 2;

	add_extra_kept_packs(&keep_pack_list);
	যদি (ignore_packed_keep_on_disk) {
		কাঠামো packed_git *p;

		repo_for_each_pack(the_repository, p)
			যদি (p->pack_local && p->pack_keep)
				বিরতি;
		যদি (!p) /* no keep-able packs found */
			ignore_packed_keep_on_disk = 0;
	}
	যদি (local) {
		/*
		 * unlike ignore_packed_keep_on_disk above, we do not
		 * want to unset "local" based on looking at packs, as
		 * it also covers non-local objects
		 */
		কাঠামো packed_git *p;

		repo_for_each_pack(the_repository, p) {
			যদি (!p->pack_local) {
				have_non_local_packs = 1;
				বিরতি;
			}
		}
	}

	trace2_region_enter("pack-objects", "enumerate-objects",
			    the_repository);
	prepare_packing_তথ্য(the_repository, &to_pack);

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
		কাঠামো rev_info revs;

		repo_init_revisions(the_repository, &revs, ফাঁকা);
		list_objects_filter_copy(&revs.filter, &filter_options);
		যদি (exclude_promisor_objects_best_effort) {
			revs.include_check = is_not_in_promisor_pack;
			revs.include_check_obj = is_not_in_promisor_pack_obj;
		}
		get_object_list(&revs, &rp);
		release_revisions(&revs);
	}
	cleanup_preferred_base();
	যদি (include_tag && nr_ফলাফল)
		refs_for_each_tag_ref(get_main_ref_store(the_repository),
				      add_ref_tag, ফাঁকা);
	stop_progress(&progress_state);
	trace2_region_leave("pack-objects", "enumerate-objects",
			    the_repository);

	যদি (non_empty && !nr_ফলাফল)
		যাও cleanup;
	যদি (nr_ফলাফল) {
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

	trace2_তথ্য_intmax("pack-objects", the_repository, "written", written);
	trace2_তথ্য_intmax("pack-objects", the_repository, "written/delta", written_delta);
	trace2_তথ্য_intmax("pack-objects", the_repository, "reused", reused);
	trace2_তথ্য_intmax("pack-objects", the_repository, "reused/delta", reused_delta);
	trace2_তথ্য_intmax("pack-objects", the_repository, "pack-reused", reuse_packfile_objects);
	trace2_তথ্য_intmax("pack-objects", the_repository, "packs-reused", reuse_packfiles_used_nr);

cleanup:
	clear_packing_তথ্য(&to_pack);
	list_objects_filter_release(&filter_options);
	string_list_clear(&keep_pack_list, 0);
	strvec_clear(&rp);

	ফেরত 0;
}
