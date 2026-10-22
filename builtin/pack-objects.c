#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "environment.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "hex.h"
#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ "attr.h"
#અન્તર્ભાવ "object.h"
#અન્તર્ભાવ "commit.h"
#અન્તર્ભાવ "tag.h"
#અન્તર્ભાવ "delta.h"
#અન્તર્ભાવ "pack.h"
#અન્તર્ભાવ "pack-revindex.h"
#અન્તર્ભાવ "csum-file.h"
#અન્તર્ભાવ "tree-walk.h"
#અન્તર્ભાવ "diff.h"
#અન્તર્ભાવ "revision.h"
#અન્તર્ભાવ "list-objects.h"
#અન્તર્ભાવ "list-objects-filter-options.h"
#અન્તર્ભાવ "pack-objects.h"
#અન્તર્ભાવ "progress.h"
#અન્તર્ભાવ "refs.h"
#અન્તર્ભાવ "thread-utils.h"
#અન્તર્ભાવ "pack-bitmap.h"
#અન્તર્ભાવ "delta-islands.h"
#અન્તર્ભાવ "reachable.h"
#અન્તર્ભાવ "oid-array.h"
#અન્તર્ભાવ "strvec.h"
#અન્તર્ભાવ "strmap.h"
#અન્તર્ભાવ "list.h"
#અન્તર્ભાવ "packfile.h"
#અન્તર્ભાવ "object-file.h"
#અન્તર્ભાવ "object-file-convert.h"
#અન્તર્ભાવ "odb.h"
#અન્તર્ભાવ "odb/streaming.h"
#અન્તર્ભાવ "replace-object.h"
#અન્તર્ભાવ "dir.h"
#અન્તર્ભાવ "midx.h"
#અન્તર્ભાવ "trace2.h"
#અન્તર્ભાવ "shallow.h"
#અન્તર્ભાવ "promisor-remote.h"
#અન્તર્ભાવ "pack-mtimes.h"
#અન્તર્ભાવ "parse-options.h"
#અન્તર્ભાવ "pkt-line.h"
#અન્તર્ભાવ "blob.h"
#અન્તર્ભાવ "tree.h"
#અન્તર્ભાવ "path-walk.h"

/*
 * Objects we are going to pack are collected in the `to_pack` structure.
 * It contains an array (dynamically expanded) of the object data, and a map
 * that can resolve SHA1s to their position in the array.
 */
સ્થિર સંરચના packing_data to_pack;

સ્થિર અંતઃરેખ સંરચના object_entry *oe_delta(
		અચલ સંરચના packing_data *pack,
		અચલ સંરચના object_entry *e)
{
	જો (!e->delta_idx)
		પાછા ખાલી;
	જો (e->ext_base)
		પાછા &pack->ext_bases[e->delta_idx - 1];
	નહીંતર
		પાછા &pack->objects[e->delta_idx - 1];
}

સ્થિર અંતઃરેખ size_t oe_delta_size(સંરચના packing_data *pack,
				   અચલ સંરચના object_entry *e)
{
	જો (e->delta_size_valid)
		પાછા e->delta_size_;

	/*
	 * pack->delta_size[] can't be NULL because oe_set_delta_size()
	 * must have been called when a new delta is saved with
	 * oe_set_delta().
	 * If oe_delta() returns NULL (i.e. default state, which means
	 * delta_size_valid is also false), then the caller must never
	 * call oe_delta_size().
	 */
	પાછા pack->delta_size[e - pack->objects];
}

size_t oe_get_size_slow(સંરચના packing_data *pack,
			અચલ સંરચના object_entry *e);

સ્થિર અંતઃરેખ size_t oe_size(સંરચના packing_data *pack,
			     અચલ સંરચના object_entry *e)
{
	જો (e->size_valid)
		પાછા e->size_;

	પાછા oe_get_size_slow(pack, e);
}

સ્થિર અંતઃરેખ શૂન્ય oe_set_delta(સંરચના packing_data *pack,
				સંરચના object_entry *e,
				સંરચના object_entry *delta)
{
	જો (delta)
		e->delta_idx = (delta - pack->objects) + 1;
	નહીંતર
		e->delta_idx = 0;
}

સ્થિર અંતઃરેખ સંરચના object_entry *oe_delta_sibling(
		અચલ સંરચના packing_data *pack,
		અચલ સંરચના object_entry *e)
{
	જો (e->delta_sibling_idx)
		પાછા &pack->objects[e->delta_sibling_idx - 1];
	પાછા ખાલી;
}

સ્થિર અંતઃરેખ સંરચના object_entry *oe_delta_child(
		અચલ સંરચના packing_data *pack,
		અચલ સંરચના object_entry *e)
{
	જો (e->delta_child_idx)
		પાછા &pack->objects[e->delta_child_idx - 1];
	પાછા ખાલી;
}

સ્થિર અંતઃરેખ શૂન્ય oe_set_delta_child(સંરચના packing_data *pack,
				      સંરચના object_entry *e,
				      સંરચના object_entry *delta)
{
	જો (delta)
		e->delta_child_idx = (delta - pack->objects) + 1;
	નહીંતર
		e->delta_child_idx = 0;
}

સ્થિર અંતઃરેખ શૂન્ય oe_set_delta_sibling(સંરચના packing_data *pack,
					સંરચના object_entry *e,
					સંરચના object_entry *delta)
{
	જો (delta)
		e->delta_sibling_idx = (delta - pack->objects) + 1;
	નહીંતર
		e->delta_sibling_idx = 0;
}

સ્થિર અંતઃરેખ શૂન્ય oe_set_size(સંરચના packing_data *pack,
			       સંરચના object_entry *e,
			       size_t size)
{
	જો (size < pack->oe_size_limit) {
		e->size_ = size;
		e->size_valid = 1;
	} નહીંતર {
		e->size_valid = 0;
		જો (oe_get_size_slow(pack, e) != size)
			BUG("'size' is supposed to be the object size!");
	}
}

સ્થિર અંતઃરેખ શૂન્ય oe_set_delta_size(સંરચના packing_data *pack,
				     સંરચના object_entry *e,
				     size_t size)
{
	જો (size < pack->oe_delta_size_limit) {
		e->delta_size_ = size;
		e->delta_size_valid = 1;
	} નહીંતર {
		packing_data_lock(pack);
		જો (!pack->delta_size)
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

સ્થિર અચલ અક્ષર *અચલ pack_usage[] = {
	N_("git pack-objects [-q | --progress | --all-progress] [--all-progress-implied]\n"
	   "                 [--no-reuse-delta] [--delta-base-offset] [--non-empty]\n"
	   "                 [--local] [--incremental] [--window=<n>] [--depth=<n>]\n"
	   "                 [--revs [--unpacked | --all]] [--keep-pack=<pack-name>]\n"
	   "                 [--cruft] [--cruft-expiration=<time>]\n"
	   "                 [--stdout [--filter=<filter-spec>] | <base-name>]\n"
	   "                 [--shallow] [--keep-true-parents] [--[no-]sparse]\n"
	   "                 [--name-hash-version=<n>] [--path-walk] < <object-list>"),
	ખાલી
};

સ્થિર સંરચના pack_idx_entry **written_list;
સ્થિર uint32_t nr_result, nr_written, nr_seen;
સ્થિર સંરચના bitmap_index *bitmap_git;
સ્થિર uint32_t write_layer;

સ્થિર પૂર્ણાંક non_empty;
સ્થિર પૂર્ણાંક reuse_delta = 1, reuse_object = 1;
સ્થિર પૂર્ણાંક keep_unreachable, unpack_unreachable, include_tag;
સ્થિર timestamp_t unpack_unreachable_expiration;
સ્થિર પૂર્ણાંક pack_loose_unreachable;
સ્થિર પૂર્ણાંક cruft;
સ્થિર પૂર્ણાંક shallow = 0;
સ્થિર timestamp_t cruft_expiration;
સ્થિર પૂર્ણાંક local;
સ્થિર પૂર્ણાંક have_non_local_packs;
સ્થિર પૂર્ણાંક incremental;
સ્થિર પૂર્ણાંક ignore_packed_keep_on_disk;
સ્થિર પૂર્ણાંક ignore_packed_keep_in_core;
સ્થિર પૂર્ણાંક ignore_packed_keep_in_core_open;
સ્થિર પૂર્ણાંક ignore_packed_keep_in_core_has_cruft;
સ્થિર પૂર્ણાંક allow_ofs_delta;
સ્થિર સંરચના pack_idx_option pack_idx_opts;
સ્થિર અચલ અક્ષર *base_name;
સ્થિર પૂર્ણાંક progress = 1;
સ્થિર પૂર્ણાંક window = 10;
સ્થિર unsigned long pack_size_limit;
સ્થિર પૂર્ણાંક depth = 50;
સ્થિર પૂર્ણાંક delta_શોધ_threads;
સ્થિર પૂર્ણાંક pack_to_stdout;
સ્થિર પૂર્ણાંક sparse;
સ્થિર પૂર્ણાંક thin;
સ્થિર પૂર્ણાંક path_walk = -1;
સ્થિર પૂર્ણાંક અંક_preferred_base;
સ્થિર સંરચના progress *progress_state;

સ્થિર સંરચના bitmapped_pack *reuse_packfiles;
સ્થિર size_t reuse_packfiles_nr;
સ્થિર size_t reuse_packfiles_used_nr;
સ્થિર uint32_t reuse_packfile_objects;
સ્થિર સંરચના bitmap *reuse_packfile_bitmap;

સ્થિર પૂર્ણાંક use_bitmap_index_default = 1;
સ્થિર પૂર્ણાંક use_bitmap_index = -1;
સ્થિર ગણના {
	NO_PACK_REUSE = 0,
	SINGLE_PACK_REUSE,
	MULTI_PACK_REUSE,
} allow_pack_reuse = SINGLE_PACK_REUSE;
સ્થિર ગણના {
	WRITE_BITMAP_FALSE = 0,
	WRITE_BITMAP_QUIET,
	WRITE_BITMAP_TRUE,
} write_bitmap_index;
સ્થિર uint16_t write_bitmap_options = BITMAP_OPT_HASH_CACHE;

સ્થિર પૂર્ણાંક exclude_promisor_objects;
સ્થિર પૂર્ણાંક exclude_promisor_objects_best_effort;

સ્થિર પૂર્ણાંક use_delta_islands;

સ્થિર size_t delta_cache_size = 0;
સ્થિર size_t max_delta_cache_size = DEFAULT_DELTA_CACHE_SIZE;
સ્થિર unsigned long cache_max_small_delta_size = 1000;

સ્થિર unsigned long window_memory_limit = 0;

સ્થિર સંરચના string_list uri_protocols = STRING_LIST_INIT_NODUP;

ગણના missing_action {
	MA_ERROR = 0,      /* fail if any missing objects are encountered */
	MA_ALLOW_ANY,      /* silently allow ALL missing objects */
	MA_ALLOW_PROMISOR, /* silently allow all missing PROMISOR objects */
};
સ્થિર ગણના missing_action arg_missing_action;
સ્થિર show_object_fn fn_show_object;

સંરચના configured_exclusion {
	સંરચના oidmap_entry e;
	અક્ષર *pack_hash_hex;
	અક્ષર *uri;
};
સ્થિર સંરચના oidmap configured_exclusions;

સ્થિર સંરચના oidset excluded_by_config;
સ્થિર પૂર્ણાંક name_hash_version = -1;

ગણના stdin_packs_mode {
	STDIN_PACKS_MODE_NONE,
	STDIN_PACKS_MODE_STANDARD,
	STDIN_PACKS_MODE_FOLLOW,
};

/**
 * Check whether the name_hash_version chosen by user input is appropriate,
 * and also validate whether it is compatible with other features.
 */
સ્થિર શૂન્ય validate_name_hash_version(શૂન્ય)
{
	જો (name_hash_version < 1 || name_hash_version > 2)
		die(_("invalid --name-hash-version option: %d"), name_hash_version);
	જો (write_bitmap_index && name_hash_version != 1) {
		warning(_("currently, --write-bitmap-index requires --name-hash-version=1"));
		name_hash_version = 1;
	}
}

સ્થિર અંતઃરેખ uint32_t pack_name_hash_fn(અચલ અક્ષર *name)
{
	સ્થિર પૂર્ણાંક seen_version = -1;

	જો (seen_version < 0)
		seen_version = name_hash_version;
	નહીંતર જો (seen_version != name_hash_version)
		BUG("name hash version changed from %d to %d mid-process",
		    seen_version, name_hash_version);

	પસંદગી (name_hash_version) {
	સ્થિતિ 1:
		પાછા pack_name_hash(name);

	સ્થિતિ 2:
		પાછા pack_name_hash_v2((અચલ unsigned અક્ષર *)name);

	મૂળભૂત:
		BUG("invalid name-hash version: %d", name_hash_version);
	}
}

/*
 * stats
 */
સ્થિર uint32_t written, written_delta;
સ્થિર uint32_t reused, reused_delta;

/*
 * Indexed commits
 */
સ્થિર સંરચના commit **indexed_commits;
સ્થિર unsigned પૂર્ણાંક indexed_commits_nr;
સ્થિર unsigned પૂર્ણાંક indexed_commits_alloc;

સ્થિર શૂન્ય index_commit_for_bitmap(સંરચના commit *commit)
{
	જો (indexed_commits_nr >= indexed_commits_alloc) {
		indexed_commits_alloc = (indexed_commits_alloc + 32) * 2;
		REALLOC_ARRAY(indexed_commits, indexed_commits_alloc);
	}

	indexed_commits[indexed_commits_nr++] = commit;
}

સ્થિર શૂન્ય *get_delta(સંરચના object_entry *entry)
{
	unsigned long size, base_size;
	size_t delta_size;
	શૂન્ય *buf, *base_buf, *delta_buf;
	ગણના object_type type;
	size_t size_st = 0, base_size_st = 0;

	buf = odb_read_object(the_repository->objects, &entry->idx.oid,
			      &type, &size_st);
	size = cast_size_t_to_ulong(size_st);
	જો (!buf)
		die(_("unable to read %s"), oid_to_hex(&entry->idx.oid));
	base_buf = odb_read_object(the_repository->objects,
				   &DELTA(entry)->idx.oid, &type,
				   &base_size_st);
	base_size = cast_size_t_to_ulong(base_size_st);
	જો (!base_buf)
		die("unable to read %s",
		    oid_to_hex(&DELTA(entry)->idx.oid));
	delta_buf = diff_delta(base_buf, base_size,
			       buf, size, &delta_size, 0);
	/*
	 * We successfully computed this delta once but dropped it for
	 * memory reasons. Something is very wrong if this time we
	 * recompute and create a different delta.
	 */
	જો (!delta_buf || delta_size != DELTA_SIZE(entry))
		BUG("delta size changed");
	મુક્ત(buf);
	મુક્ત(base_buf);
	પાછા delta_buf;
}

સ્થિર unsigned long do_compress(શૂન્ય **pptr, unsigned long size)
{
	git_zstream stream;
	શૂન્ય *in, *out;
	unsigned long maxsize;
	સંરચના repo_config_કિંમતો *cfg = repo_config_કિંમતો(the_repository);

	git_deflate_init(&stream, cfg->pack_compression_level);
	maxsize = git_deflate_bound(&stream, size);

	in = *pptr;
	out = xmalloc(maxsize);
	*pptr = out;

	stream.next_in = in;
	stream.avail_in = size;
	stream.next_out = out;
	stream.avail_out = maxsize;
	જ્યારે (git_deflate(&stream, Z_FINISH) == Z_OK)
		; /* nothing */
	git_deflate_end(&stream);

	મુક્ત(in);
	પાછા stream.total_out;
}

સ્થિર unsigned long write_large_blob_data(સંરચના odb_stream *st, સંરચના hashfile *f,
					   અચલ સંરચના object_id *oid)
{
	git_zstream stream;
	unsigned અક્ષર ibuf[1024 * 16];
	unsigned અક્ષર obuf[1024 * 16];
	unsigned long olen = 0;
	સંરચના repo_config_કિંમતો *cfg = repo_config_કિંમતો(the_repository);

	git_deflate_init(&stream, cfg->pack_compression_level);

	માટે (;;) {
		ssize_t readlen;
		પૂર્ણાંક zret = Z_OK;
		readlen = odb_stream_read(st, ibuf, કદ(ibuf));
		જો (readlen == -1)
			die(_("unable to read %s"), oid_to_hex(oid));

		stream.next_in = ibuf;
		stream.avail_in = readlen;
		જ્યારે ((stream.avail_in || readlen == 0) &&
		       (zret == Z_OK || zret == Z_BUF_ERROR)) {
			stream.next_out = obuf;
			stream.avail_out = કદ(obuf);
			zret = git_deflate(&stream, readlen ? 0 : Z_FINISH);
			hashwrite(f, obuf, stream.next_out - obuf);
			olen += stream.next_out - obuf;
		}
		જો (stream.avail_in)
			die(_("deflate error (%d)"), zret);
		જો (readlen == 0) {
			જો (zret != Z_STREAM_END)
				die(_("deflate error (%d)"), zret);
			તોડો;
		}
	}
	git_deflate_end(&stream);
	પાછા olen;
}

/*
 * we are going to reuse the existing object data as is.  make
 * sure it is not corrupt.
 */
સ્થિર પૂર્ણાંક check_pack_inflate(સંરચના packed_git *p,
		સંરચના pack_window **w_curs,
		off_t offset,
		off_t len,
		size_t expect)
{
	git_zstream stream;
	unsigned અક્ષર fakebuf[4096], *in;
	પૂર્ણાંક st;

	memset(&stream, 0, કદ(stream));
	git_inflate_init(&stream);
	કરો {
		in = use_pack(p, w_curs, offset, &stream.avail_in);
		stream.next_in = in;
		stream.next_out = fakebuf;
		stream.avail_out = કદ(fakebuf);
		st = git_inflate(&stream, Z_FINISH);
		offset += stream.next_in - in;
	} જ્યારે (st == Z_OK || st == Z_BUF_ERROR);
	git_inflate_end(&stream);
	પાછા (st == Z_STREAM_END &&
		stream.total_out == expect &&
		stream.total_in == len) ? 0 : -1;
}

સ્થિર શૂન્ય copy_pack_data(સંરચના hashfile *f,
		સંરચના packed_git *p,
		સંરચના pack_window **w_curs,
		off_t offset,
		off_t len)
{
	unsigned અક્ષર *in;
	size_t avail;

	જ્યારે (len) {
		in = use_pack(p, w_curs, offset, &avail);
		જો (avail > len)
			avail = (unsigned long)len;
		hashwrite(f, in, avail);
		offset += avail;
		len -= avail;
	}
}

સ્થિર અંતઃરેખ પૂર્ણાંક oe_size_greater_than(સંરચના packing_data *pack,
				       અચલ સંરચના object_entry *lhs,
				       size_t rhs)
{
	જો (lhs->size_valid)
		પાછા lhs->size_ > rhs;
	જો (rhs < pack->oe_size_limit) /* rhs < 2^x <= lhs ? */
		પાછા 1;
	પાછા oe_get_size_slow(pack, lhs) > rhs;
}

/* Return 0 if we will bust the pack-size limit */
સ્થિર unsigned long write_no_reuse_object(સંરચના hashfile *f, સંરચના object_entry *entry,
					   unsigned long limit, પૂર્ણાંક usable_delta)
{
	unsigned long size, datalen;
	unsigned અક્ષર header[MAX_PACK_OBJECT_HEADER],
		      dheader[MAX_PACK_OBJECT_HEADER];
	unsigned hdrlen;
	ગણના object_type type;
	શૂન્ય *buf;
	સંરચના odb_stream *st = ખાલી;
	અચલ unsigned hashsz = the_hash_algo->rawsz;

	જો (!usable_delta) {
		જો (oe_type(entry) == OBJ_BLOB &&
		    oe_size_greater_than(&to_pack, entry,
					 repo_settings_get_big_file_threshold(the_repository)) &&
		    (st = odb_stream_from_object(the_repository->objects, &entry->idx.oid,
						 ખાલી)) != ખાલી) {
			buf = ખાલી;
			type = st->type;
			size = st->size;
		} નહીંતર {
			size_t size_st = 0;
			buf = odb_read_object(the_repository->objects,
					      &entry->idx.oid, &type,
					      &size_st);
			size = cast_size_t_to_ulong(size_st);
			જો (!buf)
				die(_("unable to read %s"),
				    oid_to_hex(&entry->idx.oid));
		}
		/*
		 * make sure no cached delta data remains from a
		 * previous attempt before a pack split occurred.
		 */
		FREE_AND_NULL(entry->delta_data);
		entry->z_delta_size = 0;
	} નહીંતર જો (entry->delta_data) {
		size = DELTA_SIZE(entry);
		buf = entry->delta_data;
		entry->delta_data = ખાલી;
		type = (allow_ofs_delta && DELTA(entry)->idx.offset) ?
			OBJ_OFS_DELTA : OBJ_REF_DELTA;
	} નહીંતર {
		buf = get_delta(entry);
		size = DELTA_SIZE(entry);
		type = (allow_ofs_delta && DELTA(entry)->idx.offset) ?
			OBJ_OFS_DELTA : OBJ_REF_DELTA;
	}

	જો (st)	/* large blob case, just assume we don't compress well */
		datalen = size;
	નહીંતર જો (entry->z_delta_size)
		datalen = entry->z_delta_size;
	નહીંતર
		datalen = do_compress(&buf, size);

	/*
	 * The object header is a byte of 'type' followed by zero or
	 * more bytes of length.
	 */
	hdrlen = encode_in_pack_object_header(header, કદ(header),
					      type, size);

	જો (type == OBJ_OFS_DELTA) {
		/*
		 * Deltas with relative base contain an additional
		 * encoding of the relative offset for the delta
		 * base from this object's position in the pack.
		 */
		off_t ofs = entry->idx.offset - DELTA(entry)->idx.offset;
		unsigned pos = કદ(dheader) - 1;
		dheader[pos] = ofs & 127;
		જ્યારે (ofs >>= 7)
			dheader[--pos] = 128 | (--ofs & 127);
		જો (limit && hdrlen + કદ(dheader) - pos + datalen + hashsz >= limit) {
			જો (st)
				odb_stream_close(st);
			મુક્ત(buf);
			પાછા 0;
		}
		hashwrite(f, header, hdrlen);
		hashwrite(f, dheader + pos, કદ(dheader) - pos);
		hdrlen += કદ(dheader) - pos;
	} નહીંતર જો (type == OBJ_REF_DELTA) {
		/*
		 * Deltas with a base reference contain
		 * additional bytes for the base object ID.
		 */
		જો (limit && hdrlen + hashsz + datalen + hashsz >= limit) {
			જો (st)
				odb_stream_close(st);
			મુક્ત(buf);
			પાછા 0;
		}
		hashwrite(f, header, hdrlen);
		hashwrite(f, DELTA(entry)->idx.oid.hash, hashsz);
		hdrlen += hashsz;
	} નહીંતર {
		જો (limit && hdrlen + datalen + hashsz >= limit) {
			જો (st)
				odb_stream_close(st);
			મુક્ત(buf);
			પાછા 0;
		}
		hashwrite(f, header, hdrlen);
	}
	જો (st) {
		datalen = write_large_blob_data(st, f, &entry->idx.oid);
		odb_stream_close(st);
	} નહીંતર {
		hashwrite(f, buf, datalen);
		મુક્ત(buf);
	}

	પાછા hdrlen + datalen;
}

/* Return 0 if we will bust the pack-size limit */
સ્થિર off_t write_reuse_object(સંરચના hashfile *f, સંરચના object_entry *entry,
				unsigned long limit, પૂર્ણાંક usable_delta)
{
	સંરચના packed_git *p = IN_PACK(entry);
	સંરચના pack_window *w_curs = ખાલી;
	uint32_t pos;
	off_t offset, cur;
	ગણના object_type type = oe_type(entry);
	ગણના object_type in_pack_type;
	off_t datalen;
	unsigned અક્ષર header[MAX_PACK_OBJECT_HEADER],
		      dheader[MAX_PACK_OBJECT_HEADER];
	unsigned hdrlen;
	અચલ unsigned hashsz = the_hash_algo->rawsz;
	size_t entry_size;

	cur = entry->in_pack_offset;
	in_pack_type = unpack_object_header(p, &w_curs, &cur, &entry_size);
	જો (in_pack_type < 0)
		die(_("write_reuse_object: unable to parse object header of %s"),
		    oid_to_hex(&entry->idx.oid));

	જો (DELTA(entry))
		type = (allow_ofs_delta && DELTA(entry)->idx.offset) ?
			OBJ_OFS_DELTA : OBJ_REF_DELTA;
	hdrlen = encode_in_pack_object_header(header, કદ(header),
					      type, entry_size);

	offset = entry->in_pack_offset;
	જો (offset_to_pack_pos(p, offset, &pos) < 0)
		die(_("write_reuse_object: could not locate %s, expected at "
		      "offset %"PRIuMAX" in pack %s"),
		    oid_to_hex(&entry->idx.oid), (uintmax_t)offset,
		    p->pack_name);
	datalen = pack_pos_to_offset(p, pos + 1) - offset;
	જો (!pack_to_stdout && p->index_version > 1 &&
	    check_pack_crc(p, &w_curs, offset, datalen,
			   pack_pos_to_index(p, pos))) {
		error(_("bad packed object CRC for %s"),
		      oid_to_hex(&entry->idx.oid));
		unuse_pack(&w_curs);
		પાછા write_no_reuse_object(f, entry, limit, usable_delta);
	}

	offset += entry->in_pack_header_size;
	datalen -= entry->in_pack_header_size;

	જો (!pack_to_stdout && p->index_version == 1 &&
	    check_pack_inflate(p, &w_curs, offset, datalen, entry_size)) {
		error(_("corrupt packed object for %s"),
		      oid_to_hex(&entry->idx.oid));
		unuse_pack(&w_curs);
		પાછા write_no_reuse_object(f, entry, limit, usable_delta);
	}

	જો (type == OBJ_OFS_DELTA) {
		off_t ofs = entry->idx.offset - DELTA(entry)->idx.offset;
		unsigned pos = કદ(dheader) - 1;
		dheader[pos] = ofs & 127;
		જ્યારે (ofs >>= 7)
			dheader[--pos] = 128 | (--ofs & 127);
		જો (limit && hdrlen + કદ(dheader) - pos + datalen + hashsz >= limit) {
			unuse_pack(&w_curs);
			પાછા 0;
		}
		hashwrite(f, header, hdrlen);
		hashwrite(f, dheader + pos, કદ(dheader) - pos);
		hdrlen += કદ(dheader) - pos;
		reused_delta++;
	} નહીંતર જો (type == OBJ_REF_DELTA) {
		જો (limit && hdrlen + hashsz + datalen + hashsz >= limit) {
			unuse_pack(&w_curs);
			પાછા 0;
		}
		hashwrite(f, header, hdrlen);
		hashwrite(f, DELTA(entry)->idx.oid.hash, hashsz);
		hdrlen += hashsz;
		reused_delta++;
	} નહીંતર {
		જો (limit && hdrlen + datalen + hashsz >= limit) {
			unuse_pack(&w_curs);
			પાછા 0;
		}
		hashwrite(f, header, hdrlen);
	}
	copy_pack_data(f, p, &w_curs, offset, datalen);
	unuse_pack(&w_curs);
	reused++;
	પાછા hdrlen + datalen;
}

/* Return 0 if we will bust the pack-size limit */
સ્થિર off_t write_object(સંરચના hashfile *f,
			  સંરચના object_entry *entry,
			  off_t write_offset)
{
	unsigned long limit;
	off_t len;
	પૂર્ણાંક usable_delta, to_reuse;

	જો (!pack_to_stdout)
		crc32_begin(f);

	/* apply size limit if limited packsize and not first object */
	જો (!pack_size_limit || !nr_written)
		limit = 0;
	નહીંતર જો (pack_size_limit <= write_offset)
		/*
		 * the earlier object did not fit the limit; avoid
		 * mistaking this with unlimited (i.e. limit = 0).
		 */
		limit = 1;
	નહીંતર
		limit = pack_size_limit - write_offset;

	જો (!DELTA(entry))
		usable_delta = 0;	/* no delta */
	નહીંતર જો (!pack_size_limit)
	       usable_delta = 1;	/* unlimited packfile */
	નહીંતર જો (DELTA(entry)->idx.offset == (off_t)-1)
		usable_delta = 0;	/* base was written to another pack */
	નહીંતર જો (DELTA(entry)->idx.offset)
		usable_delta = 1;	/* base already exists in this pack */
	નહીંતર
		usable_delta = 0;	/* base could end up in another pack */

	જો (!reuse_object)
		to_reuse = 0;	/* explicit */
	નહીંતર જો (!IN_PACK(entry))
		to_reuse = 0;	/* can't reuse what we don't have */
	નહીંતર જો (oe_type(entry) == OBJ_REF_DELTA ||
		 oe_type(entry) == OBJ_OFS_DELTA)
				/* check_object() decided it for us ... */
		to_reuse = usable_delta;
				/* ... but pack split may override that */
	નહીંતર જો (oe_type(entry) != entry->in_pack_type)
		to_reuse = 0;	/* pack has delta which is unusable */
	નહીંતર જો (DELTA(entry))
		to_reuse = 0;	/* we want to pack afresh */
	નહીંતર
		to_reuse = 1;	/* we have it in-pack undeltified,
				 * and we do not need to deltify it.
				 */

	જો (!to_reuse)
		len = write_no_reuse_object(f, entry, limit, usable_delta);
	નહીંતર
		len = write_reuse_object(f, entry, limit, usable_delta);
	જો (!len)
		પાછા 0;

	જો (usable_delta)
		written_delta++;
	written++;
	જો (!pack_to_stdout)
		entry->idx.crc32 = crc32_end(f);
	પાછા len;
}

ગણના write_one_status {
	WRITE_ONE_SKIP = -1, /* already written */
	WRITE_ONE_BREAK = 0, /* writing this will bust the limit; not written */
	WRITE_ONE_WRITTEN = 1, /* normal */
	WRITE_ONE_RECURSIVE = 2 /* already scheduled to be written */
};

સ્થિર ગણના write_one_status write_one(સંરચના hashfile *f,
				       સંરચના object_entry *e,
				       off_t *offset)
{
	off_t size;
	પૂર્ણાંક recursing;

	/*
	 * we set offset to 1 (which is an impossible value) to mark
	 * the fact that this object is involved in "write its base
	 * first before writing a deltified object" recursion.
	 */
	recursing = (e->idx.offset == 1);
	જો (recursing) {
		warning(_("recursive delta detected for object %s"),
			oid_to_hex(&e->idx.oid));
		પાછા WRITE_ONE_RECURSIVE;
	} નહીંતર જો (e->idx.offset || e->preferred_base) {
		/* offset is non zero if object is written already. */
		પાછા WRITE_ONE_SKIP;
	}

	/* if we are deltified, write out base object first. */
	જો (DELTA(e)) {
		e->idx.offset = 1; /* now recurse */
		પસંદગી (write_one(f, DELTA(e), offset)) {
		સ્થિતિ WRITE_ONE_RECURSIVE:
			/* we cannot depend on this one */
			SET_DELTA(e, ખાલી);
			તોડો;
		મૂળભૂત:
			તોડો;
		સ્થિતિ WRITE_ONE_BREAK:
			e->idx.offset = recursing;
			પાછા WRITE_ONE_BREAK;
		}
	}

	e->idx.offset = *offset;
	size = write_object(f, e, *offset);
	જો (!size) {
		e->idx.offset = recursing;
		પાછા WRITE_ONE_BREAK;
	}
	written_list[nr_written++] = &e->idx;

	/* make sure off_t is sufficiently large not to wrap */
	જો (signed_add_overflows(*offset, size))
		die(_("pack too large for current definition of off_t"));
	*offset += size;
	પાછા WRITE_ONE_WRITTEN;
}

સ્થિર પૂર્ણાંક mark_tagged(અચલ સંરચના reference *ref, શૂન્ય *cb_data UNUSED)
{
	સંરચના object_id peeled;
	સંરચના object_entry *entry = packlist_find(&to_pack, ref->oid);

	જો (entry)
		entry->tagged = 1;
	જો (!reference_get_peeled_oid(the_repository, ref, &peeled)) {
		entry = packlist_find(&to_pack, &peeled);
		જો (entry)
			entry->tagged = 1;
	}
	પાછા 0;
}

સ્થિર અંતઃરેખ unsigned અક્ષર oe_layer(સંરચના packing_data *pack,
				     સંરચના object_entry *e)
{
	જો (!pack->layer)
		પાછા 0;
	પાછા pack->layer[e - pack->objects];
}

સ્થિર અંતઃરેખ શૂન્ય add_to_write_order(સંરચના object_entry **wo,
			       unsigned પૂર્ણાંક *endp,
			       સંરચના object_entry *e)
{
	જો (e->filled || oe_layer(&to_pack, e) != write_layer)
		પાછા;
	wo[(*endp)++] = e;
	e->filled = 1;
}

સ્થિર શૂન્ય add_descendants_to_write_order(સંરચના object_entry **wo,
					   unsigned પૂર્ણાંક *endp,
					   સંરચના object_entry *e)
{
	પૂર્ણાંક add_to_order = 1;
	જ્યારે (e) {
		જો (add_to_order) {
			સંરચના object_entry *s;
			/* add this node... */
			add_to_write_order(wo, endp, e);
			/* all its siblings... */
			માટે (s = DELTA_SIBLING(e); s; s = DELTA_SIBLING(s)) {
				add_to_write_order(wo, endp, s);
			}
		}
		/* drop down a level to add left subtree nodes if possible */
		જો (DELTA_CHILD(e)) {
			add_to_order = 1;
			e = DELTA_CHILD(e);
		} નહીંતર {
			add_to_order = 0;
			/* our sibling might have some children, it is next */
			જો (DELTA_SIBLING(e)) {
				e = DELTA_SIBLING(e);
				ચાલુ;
			}
			/* go back to our parent node */
			e = DELTA(e);
			જ્યારે (e && !DELTA_SIBLING(e)) {
				/* we're on the right side of a subtree, keep
				 * going up until we can go right again */
				e = DELTA(e);
			}
			જો (!e) {
				/* done- we hit our original root node */
				પાછા;
			}
			/* pass it off to sibling at this level */
			e = DELTA_SIBLING(e);
		}
	};
}

સ્થિર શૂન્ય add_family_to_write_order(સંરચના object_entry **wo,
				      unsigned પૂર્ણાંક *endp,
				      સંરચના object_entry *e)
{
	સંરચના object_entry *મૂળ;

	માટે (મૂળ = e; DELTA(મૂળ); મૂળ = DELTA(મૂળ))
		; /* nothing */
	add_descendants_to_write_order(wo, endp, મૂળ);
}

સ્થિર શૂન્ય compute_layer_order(સંરચના object_entry **wo, unsigned પૂર્ણાંક *wo_end)
{
	unsigned પૂર્ણાંક i, last_untagged;
	સંરચના object_entry *objects = to_pack.objects;

	માટે (i = 0; i < to_pack.nr_objects; i++) {
		જો (objects[i].tagged)
			તોડો;
		add_to_write_order(wo, wo_end, &objects[i]);
	}
	last_untagged = i;

	/*
	 * Then fill all the tagged tips.
	 */
	માટે (; i < to_pack.nr_objects; i++) {
		જો (objects[i].tagged)
			add_to_write_order(wo, wo_end, &objects[i]);
	}

	/*
	 * And then all remaining commits and tags.
	 */
	માટે (i = last_untagged; i < to_pack.nr_objects; i++) {
		જો (oe_type(&objects[i]) != OBJ_COMMIT &&
		    oe_type(&objects[i]) != OBJ_TAG)
			ચાલુ;
		add_to_write_order(wo, wo_end, &objects[i]);
	}

	/*
	 * And then all the trees.
	 */
	માટે (i = last_untagged; i < to_pack.nr_objects; i++) {
		જો (oe_type(&objects[i]) != OBJ_વૃક્ષ)
			ચાલુ;
		add_to_write_order(wo, wo_end, &objects[i]);
	}

	/*
	 * Finally all the rest in really tight order
	 */
	માટે (i = last_untagged; i < to_pack.nr_objects; i++) {
		જો (!objects[i].filled && oe_layer(&to_pack, &objects[i]) == write_layer)
			add_family_to_write_order(wo, wo_end, &objects[i]);
	}
}

સ્થિર સંરચના object_entry **compute_write_order(શૂન્ય)
{
	uint32_t max_layers = 1;
	unsigned પૂર્ણાંક i, wo_end;

	સંરચના object_entry **wo;
	સંરચના object_entry *objects = to_pack.objects;

	માટે (i = 0; i < to_pack.nr_objects; i++) {
		objects[i].tagged = 0;
		objects[i].filled = 0;
		SET_DELTA_CHILD(&objects[i], ખાલી);
		SET_DELTA_SIBLING(&objects[i], ખાલી);
	}

	/*
	 * Fully connect delta_child/delta_sibling network.
	 * Make sure delta_sibling is sorted in the original
	 * recency order.
	 */
	માટે (i = to_pack.nr_objects; i > 0;) {
		સંરચના object_entry *e = &objects[--i];
		જો (!DELTA(e))
			ચાલુ;
		/* Mark me as the first child */
		e->delta_sibling_idx = DELTA(e)->delta_child_idx;
		SET_DELTA_CHILD(DELTA(e), e);
	}

	/*
	 * Mark objects that are at the tip of tags.
	 */
	refs_for_each_tag_ref(get_main_ref_store(the_repository), mark_tagged,
			      ખાલી);

	જો (use_delta_islands) {
		max_layers = compute_pack_layers(&to_pack);
		free_island_marks();
	}

	ALLOC_ARRAY(wo, to_pack.nr_objects);
	wo_end = 0;

	માટે (; write_layer < max_layers; ++write_layer)
		compute_layer_order(wo, &wo_end);

	જો (wo_end != to_pack.nr_objects)
		die(_("ordered %u objects, expected %"PRIu32),
		    wo_end, to_pack.nr_objects);

	પાછા wo;
}


/*
 * A reused set of objects. All objects in a chunk have the same
 * relative position in the original packfile and the generated
 * packfile.
 */

સ્થિર સંરચના reused_chunk {
	/* The offset of the first object of this chunk in the original
	 * packfile. */
	off_t original;
	/* The difference for "original" minus the offset of the first object of
	 * this chunk in the generated packfile. */
	off_t difference;
} *reused_chunks;
સ્થિર પૂર્ણાંક reused_chunks_nr;
સ્થિર પૂર્ણાંક reused_chunks_alloc;

સ્થિર શૂન્ય record_reused_object(off_t where, off_t offset)
{
	જો (reused_chunks_nr && reused_chunks[reused_chunks_nr-1].difference == offset)
		પાછા;

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
સ્થિર off_t find_reused_offset(off_t where)
{
	પૂર્ણાંક lo = 0, hi = reused_chunks_nr;
	જ્યારે (lo < hi) {
		પૂર્ણાંક mi = lo + ((hi - lo) / 2);
		જો (where == reused_chunks[mi].original)
			પાછા reused_chunks[mi].difference;
		જો (where < reused_chunks[mi].original)
			hi = mi;
		નહીંતર
			lo = mi + 1;
	}

	/*
	 * The first chunk starts at zero, so we can't have gone below
	 * there.
	 */
	પ્રતિજ્ઞા(lo);
	પાછા reused_chunks[lo-1].difference;
}

સ્થિર શૂન્ય write_reused_pack_one(સંરચના packed_git *reuse_packfile,
				  size_t pos, સંરચના hashfile *out,
				  off_t pack_start,
				  સંરચના pack_window **w_curs)
{
	off_t offset, next, cur;
	ગણના object_type type;
	size_t size;

	offset = pack_pos_to_offset(reuse_packfile, pos);
	next = pack_pos_to_offset(reuse_packfile, pos + 1);

	record_reused_object(offset,
			     offset - (hashfile_total(out) - pack_start));

	cur = offset;
	type = unpack_object_header(reuse_packfile, w_curs, &cur, &size);
	પ્રતિજ્ઞા(type >= 0);

	જો (type == OBJ_OFS_DELTA) {
		off_t base_offset;
		off_t fixup;

		unsigned અક્ષર header[MAX_PACK_OBJECT_HEADER];
		unsigned len;

		base_offset = get_delta_base(reuse_packfile, w_curs, &cur, type, offset);
		પ્રતિજ્ઞા(base_offset != 0);

		/* Convert to REF_DELTA if we must... */
		જો (!allow_ofs_delta) {
			uint32_t base_pos;
			સંરચના object_id base_oid;

			જો (offset_to_pack_pos(reuse_packfile, base_offset, &base_pos) < 0)
				die(_("expected object at offset %"PRIuMAX" "
				      "in pack %s"),
				    (uintmax_t)base_offset,
				    reuse_packfile->pack_name);

			nth_packed_object_id(&base_oid, reuse_packfile,
					     pack_pos_to_index(reuse_packfile, base_pos));

			len = encode_in_pack_object_header(header, કદ(header),
							   OBJ_REF_DELTA, size);
			hashwrite(out, header, len);
			hashwrite(out, base_oid.hash, the_hash_algo->rawsz);
			copy_pack_data(out, reuse_packfile, w_curs, cur, next - cur);
			પાછા;
		}

		/* Otherwise see if we need to rewrite the offset... */
		fixup = find_reused_offset(offset) -
			find_reused_offset(base_offset);
		જો (fixup) {
			unsigned અક્ષર ofs_header[MAX_PACK_OBJECT_HEADER];
			unsigned i, ofs_len;
			off_t ofs = offset - base_offset - fixup;

			len = encode_in_pack_object_header(header, કદ(header),
							   OBJ_OFS_DELTA, size);

			i = કદ(ofs_header) - 1;
			ofs_header[i] = ofs & 127;
			જ્યારે (ofs >>= 7)
				ofs_header[--i] = 128 | (--ofs & 127);

			ofs_len = કદ(ofs_header) - i;

			hashwrite(out, header, len);
			hashwrite(out, ofs_header + કદ(ofs_header) - ofs_len, ofs_len);
			copy_pack_data(out, reuse_packfile, w_curs, cur, next - cur);
			પાછા;
		}

		/* ...otherwise we have no fixup, and can write it verbatim */
	}

	copy_pack_data(out, reuse_packfile, w_curs, offset, next - offset);
}

સ્થિર size_t write_reused_pack_verbatim(સંરચના bitmapped_pack *reuse_packfile,
					 સંરચના hashfile *out,
					 સંરચના pack_window **w_curs)
{
	size_t pos = 0;
	size_t end;

	જો (reuse_packfile->bitmap_pos) {
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
		પાછા reuse_packfile->bitmap_pos / BITS_IN_EWORD;
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
	જો (reuse_packfile_bitmap->word_alloc < end)
		BUG("fewer words than expected in reuse_packfile_bitmap");

	જ્યારે (pos < end && reuse_packfile_bitmap->શબ્દો[pos] == (eword_t)~0)
		pos++;

	જો (pos) {
		off_t to_write;

		written = (pos * BITS_IN_EWORD);
		to_write = pack_pos_to_offset(reuse_packfile->p, written)
			- કદ(સંરચના pack_header);

		/* We're recording one chunk, not one object. */
		record_reused_object(કદ(સંરચના pack_header), 0);
		hashflush(out);
		copy_pack_data(out, reuse_packfile->p, w_curs,
			કદ(સંરચના pack_header), to_write);

		display_progress(progress_state, written);
	}
	પાછા pos;
}

સ્થિર શૂન્ય write_reused_pack(સંરચના bitmapped_pack *reuse_packfile,
			      સંરચના hashfile *f)
{
	size_t i = reuse_packfile->bitmap_pos / BITS_IN_EWORD;
	uint32_t offset;
	off_t pack_start = hashfile_total(f) - કદ(સંરચના pack_header);
	સંરચના pack_window *w_curs = ખાલી;

	જો (allow_ofs_delta)
		i = write_reused_pack_verbatim(reuse_packfile, f, &w_curs);

	માટે (; i < reuse_packfile_bitmap->word_alloc; ++i) {
		eword_t word = reuse_packfile_bitmap->શબ્દો[i];
		size_t pos = (i * BITS_IN_EWORD);

		માટે (offset = 0; offset < BITS_IN_EWORD; ++offset) {
			uint32_t pack_pos;
			જો ((word >> offset) == 0)
				તોડો;

			offset += ewah_bit_ctz64(word >> offset);
			જો (pos + offset < reuse_packfile->bitmap_pos)
				ચાલુ;
			જો (pos + offset >= reuse_packfile->bitmap_pos + reuse_packfile->bitmap_nr)
				જાઓ done;

			જો (reuse_packfile->bitmap_pos) {
				/*
				 * When doing multi-pack reuse on a
				 * non-preferred pack, translate bit positions
				 * from the MIDX pseudo-pack order back to their
				 * pack-relative positions before attempting
				 * reuse.
				 */
				સંરચના multi_pack_index *m = reuse_packfile->from_midx;
				uint32_t midx_pos;
				off_t pack_ofs;

				જો (!m)
					BUG("non-zero bitmap position without MIDX");

				midx_pos = pack_pos_to_midx(m, pos + offset);
				pack_ofs = nth_midxed_offset(m, midx_pos);

				જો (offset_to_pack_pos(reuse_packfile->p,
						       pack_ofs, &pack_pos) < 0)
					BUG("could not find expected object at offset %"PRIuMAX" in pack %s",
					    (uintmax_t)pack_ofs,
					    pack_basename(reuse_packfile->p));
			} નહીંતર {
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

સ્થિર શૂન્ય write_excluded_by_configs(શૂન્ય)
{
	સંરચના oidset_iter iter;
	અચલ સંરચના object_id *oid;

	oidset_iter_init(&excluded_by_config, &iter);
	જ્યારે ((oid = oidset_iter_next(&iter))) {
		સંરચના configured_exclusion *ex =
			oidmap_get(&configured_exclusions, oid);

		જો (!ex)
			BUG("configured exclusion wasn't configured");
		write_in_full(1, ex->pack_hash_hex, strlen(ex->pack_hash_hex));
		write_in_full(1, " ", 1);
		write_in_full(1, ex->uri, strlen(ex->uri));
		write_in_full(1, "\n", 1);
	}
}

સ્થિર અચલ અક્ષર no_split_warning[] = N_(
"disabling bitmap writing, packs are split due to pack.packSizeLimit"
);

સ્થિર શૂન્ય write_pack_file(શૂન્ય)
{
	uint32_t i = 0, j;
	સંરચના hashfile *f;
	off_t offset;
	uint32_t nr_remaining = nr_result;
	time_t last_mtime = 0;
	સંરચના object_entry **write_order;
	off_t bytes_written = 0;

	જો (progress > pack_to_stdout)
		progress_state = start_progress(the_repository,
						_("Writing objects"), nr_result);
	ALLOC_ARRAY(written_list, to_pack.nr_objects);
	write_order = compute_write_order();

	કરો {
		unsigned અક્ષર hash[GIT_MAX_RAWSZ];
		અક્ષર *pack_tmp_name = ખાલી;

		જો (pack_to_stdout) {
			/*
			 * This command is most often invoked via
			 * git-upload-pack(1), which will typically chunk data
			 * into pktlines. As such, we use the maximum data
			 * length of them as buffer length.
			 *
			 * Note that we need to subtract one though to
			 * accommodate for the sideband byte.
			 */
			સંરચના hashfd_options opts = {
				.progress = progress_state,
				.buffer_len = LARGE_PACKET_DATA_MAX - 1,
			};
			f = hashfd_ext(the_repository->hash_algo, 1,
				       "<stdout>", &opts);
		} નહીંતર {
			f = create_tmp_packfile(the_repository, &pack_tmp_name);
		}

		offset = write_pack_header(f, nr_remaining);

		જો (reuse_packfiles_nr) {
			પ્રતિજ્ઞા(pack_to_stdout);
			માટે (j = 0; j < reuse_packfiles_nr; j++) {
				reused_chunks_nr = 0;
				write_reused_pack(&reuse_packfiles[j], f);
				જો (reused_chunks_nr)
					reuse_packfiles_used_nr++;
			}
			offset = hashfile_total(f);
		}

		nr_written = 0;
		માટે (; i < to_pack.nr_objects; i++) {
			સંરચના object_entry *e = write_order[i];
			જો (write_one(f, e, &offset) == WRITE_ONE_BREAK)
				તોડો;
			display_progress(progress_state, written);
		}

		bytes_written += hashfile_total(f) +
			the_repository->hash_algo->rawsz;
		જો (pack_to_stdout) {
			/*
			 * We never fsync when writing to stdout since we may
			 * not be writing to an actual pack file. For instance,
			 * the upload-pack code passes a pipe here. Calling
			 * fsync on a pipe results in unnecessary
			 * synchronization with the reader on some platforms.
			 */
			finalize_hashfile(f, hash, FSYNC_COMPONENT_NONE,
					  CSUM_HASH_IN_STREAM | CSUM_CLOSE);
		} નહીંતર જો (nr_written == nr_remaining) {
			finalize_hashfile(f, hash, FSYNC_COMPONENT_PACK,
					  CSUM_HASH_IN_STREAM | CSUM_FSYNC | CSUM_CLOSE);
		} નહીંતર {
			/*
			 * If we wrote the wrong number of entries in the
			 * header, rewrite it like in fast-import.
			 */

			પૂર્ણાંક fd = finalize_hashfile(f, hash, FSYNC_COMPONENT_PACK, 0);
			fixup_pack_header_footer(the_hash_algo, fd, hash,
						 pack_tmp_name, nr_written,
						 hash, offset);
			close(fd);
			જો (write_bitmap_index) {
				જો (write_bitmap_index != WRITE_BITMAP_QUIET)
					warning(_(no_split_warning));
				write_bitmap_index = 0;
			}
		}

		જો (!pack_to_stdout) {
			સંરચના stat st;
			સંરચના strbuf tmpname = STRBUF_INIT;
			સંરચના bitmap_writer bitmap_writer;
			અક્ષર *idx_tmp_name = ખાલી;

			/*
			 * Packs are runtime accessed in their mtime
			 * order since newer packs are more likely to contain
			 * younger objects.  So if we are creating multiple
			 * packs then we should modify the mtime of later ones
			 * to preserve this property.
			 */
			જો (stat(pack_tmp_name, &st) < 0) {
				warning_errno(_("failed to stat %s"), pack_tmp_name);
			} નહીંતર જો (!last_mtime) {
				last_mtime = st.st_mtime;
			} નહીંતર {
				સંરચના utimbuf utb;
				utb.actime = st.st_atime;
				utb.modtime = --last_mtime;
				જો (utime(pack_tmp_name, &utb) < 0)
					warning_errno(_("failed utime() on %s"), pack_tmp_name);
			}

			strbuf_addf(&tmpname, "%s-%s.", base_name,
				    hash_to_hex(hash));

			જો (write_bitmap_index) {
				bitmap_writer_init(&bitmap_writer,
						   the_repository, &to_pack,
						   ખાલી);
				bitmap_writer_set_checksum(&bitmap_writer, hash);
				bitmap_writer_build_type_index(&bitmap_writer,
							       written_list);
			}

			જો (cruft)
				pack_idx_opts.flags |= WRITE_MTIMES;

			stage_tmp_packfiles(the_repository, &tmpname,
					    pack_tmp_name, written_list,
					    nr_written, &to_pack,
					    &pack_idx_opts, hash,
					    &idx_tmp_name);

			જો (write_bitmap_index) {
				size_t tmpname_len = tmpname.len;

				strbuf_addstr(&tmpname, "bitmap");
				stop_progress(&progress_state);

				bitmap_writer_show_progress(&bitmap_writer,
							    progress);
				bitmap_writer_select_commits(&bitmap_writer,
							     indexed_commits,
							     indexed_commits_nr);
				જો (bitmap_writer_build(&bitmap_writer) < 0)
					die(_("failed to write bitmap index"));
				bitmap_writer_finish(&bitmap_writer,
						     written_list,
						     tmpname.buf, write_bitmap_options);
				bitmap_writer_free(&bitmap_writer);
				write_bitmap_index = 0;
				strbuf_setlen(&tmpname, tmpname_len);
			}

			rename_tmp_packfile_idx(the_repository, &tmpname, &idx_tmp_name);

			મુક્ત(idx_tmp_name);
			strbuf_release(&tmpname);
			મુક્ત(pack_tmp_name);
			puts(hash_to_hex(hash));
		}

		/* mark written objects as written to previous pack */
		માટે (j = 0; j < nr_written; j++) {
			written_list[j]->offset = (off_t)-1;
		}
		nr_remaining -= nr_written;
	} જ્યારે (nr_remaining && i < to_pack.nr_objects);

	મુક્ત(written_list);
	મુક્ત(write_order);
	stop_progress(&progress_state);
	જો (written != nr_result)
		die(_("wrote %"PRIu32" objects while expecting %"PRIu32),
		    written, nr_result);
	trace2_data_intmax("pack-objects", the_repository,
			   "write_pack_file/wrote", nr_result);
	trace2_data_intmax("pack-objects", the_repository,
			   "write_pack_file/wrote_bytes", bytes_written);
}

સ્થિર પૂર્ણાંક no_try_delta(અચલ અક્ષર *path)
{
	સ્થિર સંરચના attr_check *check;

	જો (!check)
		check = attr_check_initl("delta", ખાલી);
	git_check_attr(the_repository->index, path, check);
	જો (ATTR_FALSE(check->વસ્તુઓ[0].કિંમત))
		પાછા 1;
	પાછા 0;
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
સ્થિર પૂર્ણાંક have_duplicate_entry(અચલ સંરચના object_id *oid,
				પૂર્ણાંક exclude)
{
	સંરચના object_entry *entry;

	જો (reuse_packfile_bitmap &&
	    bitmap_walk_contains(bitmap_git, reuse_packfile_bitmap, oid))
		પાછા 1;

	entry = packlist_find(&to_pack, oid);
	જો (!entry)
		પાછા 0;

	જો (exclude) {
		જો (!entry->preferred_base)
			nr_result--;
		entry->preferred_base = 1;
	}

	પાછા 1;
}

સ્થિર પૂર્ણાંક want_cruft_object_mtime(સંરચના repository *r,
				   અચલ સંરચના object_id *oid,
				   unsigned flags, uint32_t mtime)
{
	સંરચના odb_source_files *files = odb_source_files_downcast(r->objects->source);

	માટે (સંરચના odb_files_dir *dir = files->dirs; dir; dir = dir->next) {
		સંરચના packed_git **cache = packfile_store_get_kept_pack_cache(dir->packed, flags);

		માટે (; *cache; cache++) {
			સંરચના packed_git *p = *cache;
			off_t ofs;
			uint32_t candidate_mtime;

			ofs = find_pack_entry_one(oid, p);
			જો (!ofs)
				ચાલુ;

			/*
			 * We have a copy of the object 'oid' in a non-cruft
			 * pack. We can avoid packing an additional copy
			 * regardless of what the existing copy's mtime is since
			 * it is outside of a cruft pack.
			 */
			જો (!p->is_cruft)
				પાછા 0;

			/*
			 * If we have a copy of the object 'oid' in a cruft
			 * pack, then either read the cruft pack's mtime for
			 * that object, or, if that can't be loaded, assume the
			 * pack's mtime itself.
			 */
			જો (!load_pack_mtimes(p)) {
				uint32_t pos;
				જો (offset_to_pack_pos(p, ofs, &pos) < 0)
					ચાલુ;
				candidate_mtime = nth_packed_mtime(p, pos);
			} નહીંતર {
				candidate_mtime = p->mtime;
			}

			/*
			 * We have a surviving copy of the object in a cruft
			 * pack whose mtime is greater than or equal to the one
			 * we are considering. We can thus avoid packing an
			 * additional copy of that object.
			 */
			જો (mtime <= candidate_mtime)
				પાછા 0;
		}
	}

	પાછા -1;
}

સ્થિર પૂર્ણાંક want_found_object(અચલ સંરચના object_id *oid, પૂર્ણાંક exclude,
			     સંરચના packed_git *p, uint32_t mtime)
{
	જો (exclude)
		પાછા 1;
	જો (incremental)
		પાછા 0;

	જો (!is_pack_valid(p))
		પાછા -1;

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
	જો (local && !p->pack_local)
		પાછા 0;

	/*
	 * Then handle .keep first, as we have a fast(er) path there.
	 */
	જો (ignore_packed_keep_on_disk || ignore_packed_keep_in_core ||
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
		જો (ignore_packed_keep_on_disk)
			flags |= KEPT_PACK_ON_DISK;
		જો (ignore_packed_keep_in_core)
			flags |= KEPT_PACK_IN_CORE;
		જો (ignore_packed_keep_in_core_open)
			flags |= KEPT_PACK_IN_CORE_OPEN;

		/*
		 * If the object is in a pack that we want to ignore, *and* we
		 * don't have any cruft packs that are being retained, we can
		 * abort quickly.
		 */
		જો (!ignore_packed_keep_in_core_has_cruft) {
			જો (ignore_packed_keep_on_disk && p->pack_keep)
				પાછા 0;
			જો (ignore_packed_keep_in_core && p->pack_keep_in_core)
				પાછા 0;
			જો (ignore_packed_keep_in_core_open && p->pack_keep_in_core_open)
				પાછા 0;
			જો (has_object_kept_pack(p->repo, oid, flags))
				પાછા 0;
		} નહીંતર {
			/*
			 * But if there is at least one cruft pack which
			 * is being kept, we only want to include the
			 * provided object if it has a strictly greater
			 * mtime than any existing cruft copy.
			 */
			જો (!want_cruft_object_mtime(p->repo, oid, flags,
						     mtime))
				પાછા 0;
		}
	}

	/*
	 * At this point we know definitively that either we don't care about
	 * keep-packs, or the object is not in one. Keep checking other
	 * conditions...
	 */
	જો (!local || !have_non_local_packs)
		પાછા 1;

	/* we don't know yet; keep looking for more packs */
	પાછા -1;
}

સ્થિર પૂર્ણાંક want_object_in_pack_one(સંરચના packed_git *p,
				   અચલ સંરચના object_id *oid,
				   પૂર્ણાંક exclude,
				   સંરચના packed_git **found_pack,
				   off_t *found_offset,
				   uint32_t found_mtime)
{
	off_t offset;

	જો (p == *found_pack)
		offset = *found_offset;
	નહીંતર
		offset = find_pack_entry_one(oid, p);

	જો (offset) {
		જો (!*found_pack) {
			જો (!is_pack_valid(p))
				પાછા -1;
			*found_offset = offset;
			*found_pack = p;
		}
		પાછા want_found_object(oid, exclude, p, found_mtime);
	}
	પાછા -1;
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
સ્થિર પૂર્ણાંક want_object_in_pack_mtime(અચલ સંરચના object_id *oid,
				     પૂર્ણાંક exclude,
				     સંરચના packed_git **found_pack,
				     off_t *found_offset,
				     uint32_t found_mtime)
{
	સંરચના odb_source_files *files =
		odb_source_files_downcast(the_repository->objects->source);
	પૂર્ણાંક want;
	સંરચના packfile_list_entry *e;

	જો (!exclude && local) {
		/*
		 * Note that we start iterating at `dirs->next` so that we
		 * skip the local object directory.
		 */
		માટે (સંરચના odb_files_dir *dir = files->dirs->next; dir; dir = dir->next)
			જો (!odb_source_read_object_info(&dir->loose->base, oid, ખાલી, 0, ખાલી))
				પાછા 0;
	}

	/*
	 * If we already know the pack object lives in, start checks from that
	 * pack - in the usual case when neither --local was given nor .keep files
	 * are present we will determine the answer right now.
	 */
	જો (*found_pack) {
		want = want_found_object(oid, exclude, *found_pack,
					 found_mtime);
		જો (want != -1)
			પાછા want;

		*found_pack = ખાલી;
		*found_offset = 0;
	}

	માટે (સંરચના odb_files_dir *dir = files->dirs; dir; dir = dir->next) {
		સંરચના multi_pack_index *m = get_multi_pack_index(dir->packed);
		સંરચના pack_entry e;

		જો (m && midx_fill_entry(m, oid, &e, ખાલી) == MIDX_FILL_HIT) {
			want = want_object_in_pack_one(e.p, oid, exclude, found_pack, found_offset, found_mtime);
			જો (want != -1)
				પાછા want;
		}
	}

	માટે (સંરચના odb_files_dir *dir = files->dirs; dir; dir = dir->next) {
		માટે (e = dir->packed->packs.head; e; e = e->next) {
			સંરચના packed_git *p = e->pack;
			want = want_object_in_pack_one(p, oid, exclude, found_pack, found_offset, found_mtime);
			જો (!exclude && want > 0)
				packfile_list_prepend(&dir->packed->packs, p);
			જો (want != -1)
				પાછા want;
		}
	}

	જો (uri_protocols.nr) {
		સંરચના configured_exclusion *ex =
			oidmap_get(&configured_exclusions, oid);
		પૂર્ણાંક i;
		અચલ અક્ષર *p;

		જો (ex) {
			માટે (i = 0; i < uri_protocols.nr; i++) {
				જો (skip_prefix(ex->uri,
						uri_protocols.વસ્તુઓ[i].string,
						&p) &&
				    *p == ':') {
					oidset_ઉમેરો(&excluded_by_config, oid);
					પાછા 0;
				}
			}
		}
	}

	પાછા 1;
}

સ્થિર અંતઃરેખ પૂર્ણાંક want_object_in_pack(અચલ સંરચના object_id *oid,
				      પૂર્ણાંક exclude,
				      સંરચના packed_git **found_pack,
				      off_t *found_offset)
{
	પાછા want_object_in_pack_mtime(oid, exclude, found_pack, found_offset,
					 0);
}

સ્થિર સંરચના object_entry *create_object_entry(અચલ સંરચના object_id *oid,
						ગણના object_type type,
						uint32_t hash,
						પૂર્ણાંક exclude,
						પૂર્ણાંક no_try_delta,
						સંરચના packed_git *found_pack,
						off_t found_offset)
{
	સંરચના object_entry *entry;

	entry = packlist_alloc(&to_pack, oid);
	entry->hash = hash;
	oe_set_type(entry, type);
	જો (exclude)
		entry->preferred_base = 1;
	નહીંતર
		nr_result++;
	જો (found_pack) {
		oe_set_in_pack(&to_pack, entry, found_pack);
		entry->in_pack_offset = found_offset;
	}

	entry->no_try_delta = no_try_delta;

	પાછા entry;
}

સ્થિર અચલ અક્ષર no_closure_warning[] = N_(
"disabling bitmap writing, as some objects are not being packed"
);

સ્થિર શૂન્ય add_object_entry(અચલ સંરચના object_id *oid, ગણના object_type type,
			     અચલ અક્ષર *name, પૂર્ણાંક exclude)
{
	સંરચના packed_git *found_pack = ખાલી;
	off_t found_offset = 0;

	display_progress(progress_state, ++nr_seen);

	જો (have_duplicate_entry(oid, exclude))
		પાછા;

	જો (!want_object_in_pack(oid, exclude, &found_pack, &found_offset)) {
		/* The pack is missing an object, so it will not have closure */
		જો (write_bitmap_index) {
			જો (write_bitmap_index != WRITE_BITMAP_QUIET)
				warning(_(no_closure_warning));
			write_bitmap_index = 0;
		}
		પાછા;
	}

	create_object_entry(oid, type, pack_name_hash_fn(name),
			    exclude, name && no_try_delta(name),
			    found_pack, found_offset);
}

સ્થિર પૂર્ણાંક add_object_entry_from_bitmap(અચલ સંરચના object_id *oid,
					ગણના object_type type,
					પૂર્ણાંક flags UNUSED, uint32_t name_hash,
					સંરચના packed_git *pack, off_t offset,
					શૂન્ય *payload UNUSED)
{
	display_progress(progress_state, ++nr_seen);

	જો (have_duplicate_entry(oid, 0))
		પાછા 0;

	જો (!want_object_in_pack(oid, 0, &pack, &offset))
		પાછા 0;

	create_object_entry(oid, type, name_hash, 0, 0, pack, offset);
	પાછા 0;
}

સંરચના pbase_વૃક્ષ_cache {
	સંરચના object_id oid;
	પૂર્ણાંક ref;
	પૂર્ણાંક temporary;
	શૂન્ય *વૃક્ષ_data;
	unsigned long વૃક્ષ_size;
};

સ્થિર સંરચના pbase_વૃક્ષ_cache *(pbase_વૃક્ષ_cache[256]);
સ્થિર પૂર્ણાંક pbase_વૃક્ષ_cache_ix(અચલ સંરચના object_id *oid)
{
	પાછા oid->hash[0] % ARRAY_SIZE(pbase_વૃક્ષ_cache);
}
સ્થિર પૂર્ણાંક pbase_વૃક્ષ_cache_ix_incr(પૂર્ણાંક ix)
{
	પાછા (ix+1) % ARRAY_SIZE(pbase_વૃક્ષ_cache);
}

સ્થિર સંરચના pbase_વૃક્ષ {
	સંરચના pbase_વૃક્ષ *next;
	/* This is a phony "cache" entry; we are not
	 * going to evict it or find it through _get()
	 * mechanism -- this is for the toplevel node that
	 * would almost always change with any commit.
	 */
	સંરચના pbase_વૃક્ષ_cache pcache;
} *pbase_વૃક્ષ;

સ્થિર સંરચના pbase_વૃક્ષ_cache *pbase_વૃક્ષ_get(અચલ સંરચના object_id *oid)
{
	સંરચના pbase_વૃક્ષ_cache *ent, *nent;
	શૂન્ય *data;
	unsigned long size;
	size_t size_st = 0;
	ગણના object_type type;
	પૂર્ણાંક neigh;
	પૂર્ણાંક my_ix = pbase_વૃક્ષ_cache_ix(oid);
	પૂર્ણાંક available_ix = -1;

	/* pbase-tree-cache acts as a limited hashtable.
	 * your object will be found at your index or within a few
	 * slots after that slot if it is cached.
	 */
	માટે (neigh = 0; neigh < 8; neigh++) {
		ent = pbase_વૃક્ષ_cache[my_ix];
		જો (ent && oideq(&ent->oid, oid)) {
			ent->ref++;
			પાછા ent;
		}
		નહીંતર જો (((available_ix < 0) && (!ent || !ent->ref)) ||
			 ((0 <= available_ix) &&
			  (!ent && pbase_વૃક્ષ_cache[available_ix])))
			available_ix = my_ix;
		જો (!ent)
			તોડો;
		my_ix = pbase_વૃક્ષ_cache_ix_incr(my_ix);
	}

	/* Did not find one.  Either we got a bogus request or
	 * we need to read and perhaps cache.
	 */
	data = odb_read_object(the_repository->objects, oid, &type, &size_st);
	size = cast_size_t_to_ulong(size_st);
	જો (!data)
		પાછા ખાલી;
	જો (type != OBJ_વૃક્ષ) {
		મુક્ત(data);
		પાછા ખાલી;
	}

	/* We need to either cache or return a throwaway copy */

	જો (available_ix < 0)
		ent = ખાલી;
	નહીંતર {
		ent = pbase_વૃક્ષ_cache[available_ix];
		my_ix = available_ix;
	}

	જો (!ent) {
		nent = xmalloc(કદ(*nent));
		nent->temporary = (available_ix < 0);
	}
	નહીંતર {
		/* evict and reuse */
		મુક્ત(ent->વૃક્ષ_data);
		nent = ent;
	}
	oidcpy(&nent->oid, oid);
	nent->વૃક્ષ_data = data;
	nent->વૃક્ષ_size = size;
	nent->ref = 1;
	જો (!nent->temporary)
		pbase_વૃક્ષ_cache[my_ix] = nent;
	પાછા nent;
}

સ્થિર શૂન્ય pbase_વૃક્ષ_put(સંરચના pbase_વૃક્ષ_cache *cache)
{
	જો (!cache->temporary) {
		cache->ref--;
		પાછા;
	}
	મુક્ત(cache->વૃક્ષ_data);
	મુક્ત(cache);
}

સ્થિર size_t name_cmp_len(અચલ અક્ષર *name)
{
	પાછા strcspn(name, "\n/");
}

સ્થિર શૂન્ય add_pbase_object(સંરચના વૃક્ષ_desc *વૃક્ષ,
			     અચલ અક્ષર *name,
			     size_t cmplen,
			     અચલ અક્ષર *fullname)
{
	સંરચના name_entry entry;
	પૂર્ણાંક cmp;

	જ્યારે (વૃક્ષ_entry(વૃક્ષ,&entry)) {
		જો (S_ISGITLINK(entry.mode))
			ચાલુ;
		cmp = વૃક્ષ_entry_len(&entry) != cmplen ? 1 :
		      memcmp(name, entry.path, cmplen);
		જો (cmp > 0)
			ચાલુ;
		જો (cmp < 0)
			પાછા;
		જો (name[cmplen] != '/') {
			add_object_entry(&entry.oid,
					 object_type(entry.mode),
					 fullname, 1);
			પાછા;
		}
		જો (S_ISDIR(entry.mode)) {
			સંરચના વૃક્ષ_desc sub;
			સંરચના pbase_વૃક્ષ_cache *વૃક્ષ;
			અચલ અક્ષર *down = name+cmplen+1;
			size_t downlen = name_cmp_len(down);

			વૃક્ષ = pbase_વૃક્ષ_get(&entry.oid);
			જો (!વૃક્ષ)
				પાછા;
			init_વૃક્ષ_desc(&sub, &વૃક્ષ->oid,
				       વૃક્ષ->વૃક્ષ_data, વૃક્ષ->વૃક્ષ_size);

			add_pbase_object(&sub, down, downlen, fullname);
			pbase_વૃક્ષ_put(વૃક્ષ);
		}
	}
}

સ્થિર unsigned *done_pbase_paths;
સ્થિર પૂર્ણાંક done_pbase_paths_અંક;
સ્થિર પૂર્ણાંક done_pbase_paths_alloc;
સ્થિર પૂર્ણાંક done_pbase_path_pos(unsigned hash)
{
	પૂર્ણાંક lo = 0;
	પૂર્ણાંક hi = done_pbase_paths_અંક;
	જ્યારે (lo < hi) {
		પૂર્ણાંક mi = lo + (hi - lo) / 2;
		જો (done_pbase_paths[mi] == hash)
			પાછા mi;
		જો (done_pbase_paths[mi] < hash)
			hi = mi;
		નહીંતર
			lo = mi + 1;
	}
	પાછા -lo-1;
}

સ્થિર પૂર્ણાંક check_pbase_path(unsigned hash)
{
	પૂર્ણાંક pos = done_pbase_path_pos(hash);
	જો (0 <= pos)
		પાછા 1;
	pos = -pos - 1;
	ALLOC_GROW(done_pbase_paths,
		   done_pbase_paths_અંક + 1,
		   done_pbase_paths_alloc);
	done_pbase_paths_અંક++;
	જો (pos < done_pbase_paths_અંક)
		MOVE_ARRAY(done_pbase_paths + pos + 1, done_pbase_paths + pos,
			   done_pbase_paths_અંક - pos - 1);
	done_pbase_paths[pos] = hash;
	પાછા 0;
}

સ્થિર શૂન્ય add_preferred_base_object(અચલ અક્ષર *name)
{
	સંરચના pbase_વૃક્ષ *it;
	size_t cmplen;
	unsigned hash = pack_name_hash_fn(name);

	જો (!અંક_preferred_base || check_pbase_path(hash))
		પાછા;

	cmplen = name_cmp_len(name);
	માટે (it = pbase_વૃક્ષ; it; it = it->next) {
		જો (cmplen == 0) {
			add_object_entry(&it->pcache.oid, OBJ_વૃક્ષ, ખાલી, 1);
		}
		નહીંતર {
			સંરચના વૃક્ષ_desc વૃક્ષ;
			init_વૃક્ષ_desc(&વૃક્ષ, &it->pcache.oid,
				       it->pcache.વૃક્ષ_data, it->pcache.વૃક્ષ_size);
			add_pbase_object(&વૃક્ષ, name, cmplen, name);
		}
	}
}

સ્થિર શૂન્ય add_preferred_base(સંરચના object_id *oid)
{
	સંરચના pbase_વૃક્ષ *it;
	શૂન્ય *data;
	unsigned long size;
	size_t size_st = 0;
	સંરચના object_id વૃક્ષ_oid;

	જો (window <= અંક_preferred_base++)
		પાછા;

	data = odb_read_object_peeled(the_repository->objects, oid,
				      OBJ_વૃક્ષ, &size_st, &વૃક્ષ_oid);
	size = cast_size_t_to_ulong(size_st);
	જો (!data)
		પાછા;

	માટે (it = pbase_વૃક્ષ; it; it = it->next) {
		જો (oideq(&it->pcache.oid, &વૃક્ષ_oid)) {
			મુક્ત(data);
			પાછા;
		}
	}

	CALLOC_ARRAY(it, 1);
	it->next = pbase_વૃક્ષ;
	pbase_વૃક્ષ = it;

	oidcpy(&it->pcache.oid, &વૃક્ષ_oid);
	it->pcache.વૃક્ષ_data = data;
	it->pcache.વૃક્ષ_size = size;
}

સ્થિર શૂન્ય cleanup_preferred_base(શૂન્ય)
{
	સંરચના pbase_વૃક્ષ *it;
	unsigned i;

	it = pbase_વૃક્ષ;
	pbase_વૃક્ષ = ખાલી;
	જ્યારે (it) {
		સંરચના pbase_વૃક્ષ *tmp = it;
		it = tmp->next;
		મુક્ત(tmp->pcache.વૃક્ષ_data);
		મુક્ત(tmp);
	}

	માટે (i = 0; i < ARRAY_SIZE(pbase_વૃક્ષ_cache); i++) {
		જો (!pbase_વૃક્ષ_cache[i])
			ચાલુ;
		મુક્ત(pbase_વૃક્ષ_cache[i]->વૃક્ષ_data);
		FREE_AND_NULL(pbase_વૃક્ષ_cache[i]);
	}

	FREE_AND_NULL(done_pbase_paths);
	done_pbase_paths_અંક = done_pbase_paths_alloc = 0;
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
સ્થિર પૂર્ણાંક can_reuse_delta(અચલ સંરચના object_id *base_oid,
			   સંરચના object_entry *delta,
			   સંરચના object_entry **base_out)
{
	સંરચના object_entry *base;

	/*
	 * First see if we're already sending the base (or it's explicitly in
	 * our "excluded" list).
	 */
	base = packlist_find(&to_pack, base_oid);
	જો (base) {
		જો (!in_same_island(&delta->idx.oid, &base->idx.oid))
			પાછા 0;
		*base_out = base;
		પાછા 1;
	}

	/*
	 * Otherwise, reachability bitmaps may tell us if the receiver has it,
	 * even if it was buried too deep in history to make it into the
	 * packing list.
	 */
	જો (thin && bitmap_has_oid_in_uninteresting(bitmap_git, base_oid)) {
		જો (use_delta_islands) {
			જો (!in_same_island(&delta->idx.oid, base_oid))
				પાછા 0;
		}
		*base_out = ખાલી;
		પાછા 1;
	}

	પાછા 0;
}

સ્થિર શૂન્ય prefetch_to_pack(uint32_t object_index_start) {
	સંરચના oid_array to_fetch = OID_ARRAY_INIT;
	uint32_t i;

	માટે (i = object_index_start; i < to_pack.nr_objects; i++) {
		સંરચના object_entry *entry = to_pack.objects + i;

		જો (!odb_read_object_info_extended(the_repository->objects,
						   &entry->idx.oid,
						   ખાલી,
						   OBJECT_INFO_FOR_PREFETCH))
			ચાલુ;
		oid_array_append(&to_fetch, &entry->idx.oid);
	}
	promisor_remote_get_direct(the_repository,
				   to_fetch.oid, to_fetch.nr);
	oid_array_clear(&to_fetch);
}

સ્થિર શૂન્ય check_object(સંરચના object_entry *entry, uint32_t object_index)
{
	size_t canonical_size;
	ગણના object_type type;
	સંરચના object_info oi = {.typep = &type, .sizep = &canonical_size};

	જો (IN_PACK(entry)) {
		સંરચના packed_git *p = IN_PACK(entry);
		સંરચના pack_window *w_curs = ખાલી;
		પૂર્ણાંક have_base = 0;
		સંરચના object_id base_ref;
		સંરચના object_entry *base_entry;
		size_t used, used_0, avail;
		off_t ofs;
		unsigned અક્ષર *buf, c;
		ગણના object_type type;
		size_t in_pack_size;

		buf = use_pack(p, &w_curs, entry->in_pack_offset, &avail);

		/*
		 * We want in_pack_type even if we do not reuse delta
		 * since non-delta representations could still be reused.
		 */
		used = unpack_object_header_buffer(buf, avail,
						   &type,
						   &in_pack_size);
		જો (used == 0)
			જાઓ give_up;

		જો (type < 0)
			BUG("invalid type %d", type);
		entry->in_pack_type = type;

		/*
		 * Determine if this is a delta and if so whether we can
		 * reuse it or not.  Otherwise let's find out as cheaply as
		 * possible what the actual type and size for this object is.
		 */
		પસંદગી (entry->in_pack_type) {
		મૂળભૂત:
			/* Not a delta hence we've already got all we need. */
			oe_set_type(entry, entry->in_pack_type);
			SET_SIZE(entry, in_pack_size);
			entry->in_pack_header_size = used;
			જો (oe_type(entry) < OBJ_COMMIT || oe_type(entry) > OBJ_BLOB)
				જાઓ give_up;
			unuse_pack(&w_curs);
			પાછા;
		સ્થિતિ OBJ_REF_DELTA:
			જો (reuse_delta && !entry->preferred_base) {
				oidread(&base_ref,
					use_pack(p, &w_curs,
						 entry->in_pack_offset + used,
						 ખાલી),
					the_repository->hash_algo);
				have_base = 1;
			}
			entry->in_pack_header_size = used + the_hash_algo->rawsz;
			તોડો;
		સ્થિતિ OBJ_OFS_DELTA:
			buf = use_pack(p, &w_curs,
				       entry->in_pack_offset + used, ખાલી);
			used_0 = 0;
			c = buf[used_0++];
			ofs = c & 127;
			જ્યારે (c & 128) {
				ofs += 1;
				જો (!ofs || MSB(ofs, 7)) {
					error(_("delta base offset overflow in pack for %s"),
					      oid_to_hex(&entry->idx.oid));
					જાઓ give_up;
				}
				c = buf[used_0++];
				ofs = (ofs << 7) + (c & 127);
			}
			ofs = entry->in_pack_offset - ofs;
			જો (ofs <= 0 || ofs >= entry->in_pack_offset) {
				error(_("delta base offset out of bound for %s"),
				      oid_to_hex(&entry->idx.oid));
				જાઓ give_up;
			}
			જો (reuse_delta && !entry->preferred_base) {
				uint32_t pos;
				જો (offset_to_pack_pos(p, ofs, &pos) < 0)
					જાઓ give_up;
				જો (!nth_packed_object_id(&base_ref, p,
							  pack_pos_to_index(p, pos)))
					have_base = 1;
			}
			entry->in_pack_header_size = used + used_0;
			તોડો;
		}

		જો (have_base &&
		    can_reuse_delta(&base_ref, entry, &base_entry)) {
			oe_set_type(entry, entry->in_pack_type);
			SET_SIZE(entry, in_pack_size); /* delta size */
			SET_DELTA_SIZE(entry, in_pack_size);

			જો (base_entry) {
				SET_DELTA(entry, base_entry);
				entry->delta_sibling_idx = base_entry->delta_child_idx;
				SET_DELTA_CHILD(base_entry, entry);
			} નહીંતર {
				SET_DELTA_EXT(entry, &base_ref);
			}

			unuse_pack(&w_curs);
			પાછા;
		}

		જો (oe_type(entry)) {
			off_t delta_pos;

			/*
			 * This must be a delta and we already know what the
			 * final object type is.  Let's extract the actual
			 * object size from the delta header.
			 */
			delta_pos = entry->in_pack_offset + entry->in_pack_header_size;
			canonical_size = get_size_from_delta(p, &w_curs,
							     delta_pos);
			જો (canonical_size == 0)
				જાઓ give_up;
			SET_SIZE(entry, canonical_size);
			unuse_pack(&w_curs);
			પાછા;
		}

		/*
		 * No choice but to fall back to the recursive delta walk
		 * with odb_read_object_info() to find about the object type
		 * at this point...
		 */
		give_up:
		unuse_pack(&w_curs);
	}

	જો (odb_read_object_info_extended(the_repository->objects, &entry->idx.oid, &oi,
					  OBJECT_INFO_SKIP_FETCH_OBJECT | OBJECT_INFO_LOOKUP_REPLACE) < 0) {
		જો (repo_has_promisor_remote(the_repository)) {
			prefetch_to_pack(object_index);
			જો (odb_read_object_info_extended(the_repository->objects, &entry->idx.oid, &oi,
							  OBJECT_INFO_SKIP_FETCH_OBJECT | OBJECT_INFO_LOOKUP_REPLACE) < 0)
				type = -1;
		} નહીંતર {
			type = -1;
		}
	}
	oe_set_type(entry, type);
	જો (entry->type_valid) {
		SET_SIZE(entry, canonical_size);
	} નહીંતર {
		/*
		 * Bad object type is checked in prepare_pack().  This is
		 * to permit a missing preferred base object to be ignored
		 * as a preferred base.  Doing so can result in a larger
		 * pack file, but the transfer will still take place.
		 */
	}
}

સ્થિર પૂર્ણાંક pack_offset_sort(અચલ શૂન્ય *_a, અચલ શૂન્ય *_b)
{
	અચલ સંરચના object_entry *a = *(સંરચના object_entry **)_a;
	અચલ સંરચના object_entry *b = *(સંરચના object_entry **)_b;
	અચલ સંરચના packed_git *a_in_pack = IN_PACK(a);
	અચલ સંરચના packed_git *b_in_pack = IN_PACK(b);

	/* avoid filesystem trashing with loose objects */
	જો (!a_in_pack && !b_in_pack)
		પાછા oidcmp(&a->idx.oid, &b->idx.oid);

	જો (a_in_pack < b_in_pack)
		પાછા -1;
	જો (a_in_pack > b_in_pack)
		પાછા 1;
	પાછા a->in_pack_offset < b->in_pack_offset ? -1 :
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
સ્થિર શૂન્ય drop_reused_delta(સંરચના object_entry *entry)
{
	unsigned *idx = &to_pack.objects[entry->delta_idx - 1].delta_child_idx;
	સંરચના object_info oi = OBJECT_INFO_INIT;
	ગણના object_type type;
	size_t size;

	જ્યારે (*idx) {
		સંરચના object_entry *oe = &to_pack.objects[*idx - 1];

		જો (oe == entry)
			*idx = oe->delta_sibling_idx;
		નહીંતર
			idx = &oe->delta_sibling_idx;
	}
	SET_DELTA(entry, ખાલી);
	entry->depth = 0;

	oi.sizep = &size;
	oi.typep = &type;
	જો (packed_object_info(ખાલી, IN_PACK(entry), entry->in_pack_offset, &oi) < 0) {
		/*
		 * We failed to get the info from this pack for some reason;
		 * fall back to odb_read_object_info, which may find another copy.
		 * And if that fails, the error will be recorded in oe_type(entry)
		 * and dealt with in prepare_pack().
		 */
		oe_set_type(entry,
			    odb_read_object_info(the_repository->objects,
						 &entry->idx.oid, &size));
	} નહીંતર {
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
સ્થિર શૂન્ય break_delta_chains(સંરચના object_entry *entry)
{
	/*
	 * The actual depth of each object we will write is stored as an int,
	 * as it cannot exceed our int "depth" limit. But before we break
	 * changes based no that limit, we may potentially go as deep as the
	 * number of objects, which is elsewhere bounded to a uint32_t.
	 */
	uint32_t total_depth;
	સંરચના object_entry *cur, *next;

	માટે (cur = entry, total_depth = 0;
	     cur;
	     cur = DELTA(cur), total_depth++) {
		જો (cur->dfs_state == DFS_DONE) {
			/*
			 * We've already seen this object and know it isn't
			 * part of a cycle. We do need to append its depth
			 * to our count.
			 */
			total_depth += cur->depth;
			તોડો;
		}

		/*
		 * We break cycles before looping, so an ACTIVE state (or any
		 * other cruft which made its way into the state variable)
		 * is a bug.
		 */
		જો (cur->dfs_state != DFS_NONE)
			BUG("confusing delta dfs state in first pass: %d",
			    cur->dfs_state);

		/*
		 * Now we know this is the first time we've seen the object. If
		 * it's not a delta, we're done traversing, but we'll mark it
		 * done to save time on future traversals.
		 */
		જો (!DELTA(cur)) {
			cur->dfs_state = DFS_DONE;
			તોડો;
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
		જો (DELTA(cur)->dfs_state == DFS_ACTIVE) {
			drop_reused_delta(cur);
			cur->dfs_state = DFS_DONE;
			તોડો;
		}
	}

	/*
	 * And now that we've gone all the way to the bottom of the chain, we
	 * need to clear the active flags and set the depth fields as
	 * appropriate. Unlike the loop above, which can quit when it drops a
	 * delta, we need to keep going to look for more depth cuts. So we need
	 * an extra "next" pointer to keep going after we reset cur->delta.
	 */
	માટે (cur = entry; cur; cur = next) {
		next = DELTA(cur);

		/*
		 * We should have a chain of zero or more ACTIVE states down to
		 * a final DONE. We can quit after the DONE, because either it
		 * has no bases, or we've already handled them in a previous
		 * call.
		 */
		જો (cur->dfs_state == DFS_DONE)
			તોડો;
		નહીંતર જો (cur->dfs_state != DFS_ACTIVE)
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
		જો (!cur->depth)
			drop_reused_delta(cur);

		cur->dfs_state = DFS_DONE;
	}
}

સ્થિર શૂન્ય get_object_details(શૂન્ય)
{
	uint32_t i;
	સંરચના object_entry **sorted_by_offset;

	જો (progress)
		progress_state = start_progress(the_repository,
						_("Counting objects"),
						to_pack.nr_objects);

	CALLOC_ARRAY(sorted_by_offset, to_pack.nr_objects);
	માટે (i = 0; i < to_pack.nr_objects; i++)
		sorted_by_offset[i] = to_pack.objects + i;
	QSORT(sorted_by_offset, to_pack.nr_objects, pack_offset_sort);

	માટે (i = 0; i < to_pack.nr_objects; i++) {
		સંરચના object_entry *entry = sorted_by_offset[i];
		check_object(entry, i);
		જો (entry->type_valid &&
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
	માટે (i = 0; i < to_pack.nr_objects; i++)
		break_delta_chains(&to_pack.objects[i]);

	મુક્ત(sorted_by_offset);
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
સ્થિર પૂર્ણાંક type_size_sort(અચલ શૂન્ય *_a, અચલ શૂન્ય *_b)
{
	અચલ સંરચના object_entry *a = *(સંરચના object_entry **)_a;
	અચલ સંરચના object_entry *b = *(સંરચના object_entry **)_b;
	અચલ ગણના object_type a_type = oe_type(a);
	અચલ ગણના object_type b_type = oe_type(b);
	અચલ unsigned long a_size = SIZE(a);
	અચલ unsigned long b_size = SIZE(b);

	જો (a_type > b_type)
		પાછા -1;
	જો (a_type < b_type)
		પાછા 1;
	જો (a->hash > b->hash)
		પાછા -1;
	જો (a->hash < b->hash)
		પાછા 1;
	જો (a->preferred_base > b->preferred_base)
		પાછા -1;
	જો (a->preferred_base < b->preferred_base)
		પાછા 1;
	જો (use_delta_islands) {
		અચલ પૂર્ણાંક island_cmp = island_delta_cmp(&a->idx.oid, &b->idx.oid);
		જો (island_cmp)
			પાછા island_cmp;
	}
	જો (a_size > b_size)
		પાછા -1;
	જો (a_size < b_size)
		પાછા 1;
	પાછા a < b ? -1 : (a > b);  /* newest first */
}

સંરચના unpacked {
	સંરચના object_entry *entry;
	શૂન્ય *data;
	સંરચના delta_index *index;
	unsigned depth;
};

સ્થિર પૂર્ણાંક delta_cacheable(size_t src_size, size_t trg_size,
			   size_t delta_size)
{
	જો (max_delta_cache_size && delta_cache_size + delta_size > max_delta_cache_size)
		પાછા 0;

	જો (delta_size < cache_max_small_delta_size)
		પાછા 1;

	/* cache delta, if objects are large enough compared to delta size */
	જો ((src_size >> 20) + (trg_size >> 21) > (delta_size >> 10))
		પાછા 1;

	પાછા 0;
}

/* Protect delta_cache_size */
સ્થિર pthread_mutex_t cache_mutex;
#define cache_lock()		pthread_mutex_lock(&cache_mutex)
#define cache_unlock()		pthread_mutex_unlock(&cache_mutex)

/*
 * Protect object list partitioning (e.g. struct thread_param) and
 * progress_state
 */
સ્થિર pthread_mutex_t progress_mutex;
#define progress_lock()		pthread_mutex_lock(&progress_mutex)
#define progress_unlock()	pthread_mutex_unlock(&progress_mutex)

/*
 * Access to struct object_entry is unprotected since each thread owns
 * a portion of the main object list. Just don't access object entries
 * ahead in the list because they can be stolen and would need
 * progress_mutex for protection.
 */

સ્થિર અંતઃરેખ પૂર્ણાંક oe_size_less_than(સંરચના packing_data *pack,
				    અચલ સંરચના object_entry *lhs,
				    size_t rhs)
{
	જો (lhs->size_valid)
		પાછા lhs->size_ < rhs;
	જો (rhs < pack->oe_size_limit) /* rhs < 2^x <= lhs ? */
		પાછા 0;
	પાછા oe_get_size_slow(pack, lhs) < rhs;
}

સ્થિર અંતઃરેખ શૂન્ય oe_set_વૃક્ષ_depth(સંરચના packing_data *pack,
				     સંરચના object_entry *e,
				     unsigned પૂર્ણાંક વૃક્ષ_depth)
{
	જો (!pack->વૃક્ષ_depth)
		CALLOC_ARRAY(pack->વૃક્ષ_depth, pack->nr_alloc);
	pack->વૃક્ષ_depth[e - pack->objects] = વૃક્ષ_depth;
}

સ્થિર શૂન્ય record_વૃક્ષ_depth(અચલ સંરચના object_id *oid, અચલ અક્ષર *name)
{
	અચલ અક્ષર *p;
	unsigned depth;
	સંરચના object_entry *ent;

	/* the empty string is a root tree, which is depth 0 */
	depth = *name ? 1 : 0;
	માટે (p = strchr(name, '/'); p; p = strchr(p + 1, '/'))
		depth++;

	ent = packlist_find(&to_pack, oid);
	જો (ent && depth > oe_વૃક્ષ_depth(&to_pack, ent))
		oe_set_વૃક્ષ_depth(&to_pack, ent, depth);
}

/*
 * Return the size of the object without doing any delta
 * reconstruction (so non-deltas are true object sizes, but deltas
 * return the size of the delta data).
 */
size_t oe_get_size_slow(સંરચના packing_data *pack,
			અચલ સંરચના object_entry *e)
{
	સંરચના packed_git *p;
	સંરચના pack_window *w_curs;
	unsigned અક્ષર *buf;
	ગણના object_type type;
	size_t used, avail, size;

	જો (e->type_ != OBJ_OFS_DELTA && e->type_ != OBJ_REF_DELTA) {
		size_t sz;
		packing_data_lock(&to_pack);
		જો (odb_read_object_info(the_repository->objects,
					 &e->idx.oid, &sz) < 0)
			die(_("unable to get size of %s"),
			    oid_to_hex(&e->idx.oid));
		packing_data_unlock(&to_pack);
		પાછા sz;
	}

	p = oe_in_pack(pack, e);
	જો (!p)
		BUG("when e->type is a delta, it must belong to a pack");

	packing_data_lock(&to_pack);
	w_curs = ખાલી;
	buf = use_pack(p, &w_curs, e->in_pack_offset, &avail);
	used = unpack_object_header_buffer(buf, avail, &type, &size);
	જો (used == 0)
		die(_("unable to parse object header of %s"),
		    oid_to_hex(&e->idx.oid));

	unuse_pack(&w_curs);
	packing_data_unlock(&to_pack);
	પાછા size;
}

સ્થિર પૂર્ણાંક try_delta(સંરચના unpacked *trg, સંરચના unpacked *src,
		     unsigned max_depth, size_t *mem_usage)
{
	સંરચના object_entry *trg_entry = trg->entry;
	સંરચના object_entry *src_entry = src->entry;
	unsigned long trg_size, src_size, sizediff, max_size, sz;
	size_t delta_size;
	unsigned ref_depth;
	ગણના object_type type;
	શૂન્ય *delta_buf;

	/* Don't bother doing diffs between different types */
	જો (oe_type(trg_entry) != oe_type(src_entry))
		પાછા -1;

	/*
	 * We do not bother to try a delta that we discarded on an
	 * earlier try, but only when reusing delta data.  Note that
	 * src_entry that is marked as the preferred_base should always
	 * be considered, as even if we produce a suboptimal delta against
	 * it, we will still save the transfer cost, as we already know
	 * the other side has it and we won't send src_entry at all.
	 */
	જો (reuse_delta && IN_PACK(trg_entry) &&
	    IN_PACK(trg_entry) == IN_PACK(src_entry) &&
	    !src_entry->preferred_base &&
	    trg_entry->in_pack_type != OBJ_REF_DELTA &&
	    trg_entry->in_pack_type != OBJ_OFS_DELTA)
		પાછા 0;

	/* Let's not bust the allowed depth. */
	જો (src->depth >= max_depth)
		પાછા 0;

	/* Now some size filtering heuristics. */
	trg_size = SIZE(trg_entry);
	જો (!DELTA(trg_entry)) {
		max_size = trg_size/2 - the_hash_algo->rawsz;
		ref_depth = 1;
	} નહીંતર {
		max_size = DELTA_SIZE(trg_entry);
		ref_depth = trg->depth;
	}
	max_size = (uint64_t)max_size * (max_depth - src->depth) /
						(max_depth - ref_depth + 1);
	જો (max_size == 0)
		પાછા 0;
	src_size = SIZE(src_entry);
	sizediff = src_size < trg_size ? trg_size - src_size : 0;
	જો (sizediff >= max_size)
		પાછા 0;
	જો (trg_size < src_size / 32)
		પાછા 0;

	જો (!in_same_island(&trg->entry->idx.oid, &src->entry->idx.oid))
		પાછા 0;

	/* Load data if not already done */
	જો (!trg->data) {
		size_t sz_st = 0;
		packing_data_lock(&to_pack);
		trg->data = odb_read_object(the_repository->objects,
					    &trg_entry->idx.oid, &type,
					    &sz_st);
		sz = cast_size_t_to_ulong(sz_st);
		packing_data_unlock(&to_pack);
		જો (!trg->data)
			die(_("object %s cannot be read"),
			    oid_to_hex(&trg_entry->idx.oid));
		જો (sz != trg_size)
			die(_("object %s inconsistent object length (%"PRIuMAX" vs %"PRIuMAX")"),
			    oid_to_hex(&trg_entry->idx.oid), (uintmax_t)sz,
			    (uintmax_t)trg_size);
		*mem_usage += sz;
	}
	જો (!src->data) {
		size_t sz_st = 0;
		packing_data_lock(&to_pack);
		src->data = odb_read_object(the_repository->objects,
					    &src_entry->idx.oid, &type,
					    &sz_st);
		sz = cast_size_t_to_ulong(sz_st);
		packing_data_unlock(&to_pack);
		જો (!src->data) {
			જો (src_entry->preferred_base) {
				સ્થિર પૂર્ણાંક warned = 0;
				જો (!warned++)
					warning(_("object %s cannot be read"),
						oid_to_hex(&src_entry->idx.oid));
				/*
				 * Those objects are not included in the
				 * resulting pack.  Be resilient and ignore
				 * them if they can't be read, in case the
				 * pack could be created nevertheless.
				 */
				પાછા 0;
			}
			die(_("object %s cannot be read"),
			    oid_to_hex(&src_entry->idx.oid));
		}
		જો (sz != src_size)
			die(_("object %s inconsistent object length (%"PRIuMAX" vs %"PRIuMAX")"),
			    oid_to_hex(&src_entry->idx.oid), (uintmax_t)sz,
			    (uintmax_t)src_size);
		*mem_usage += sz;
	}
	જો (!src->index) {
		src->index = create_delta_index(src->data, src_size);
		જો (!src->index) {
			સ્થિર પૂર્ણાંક warned = 0;
			જો (!warned++)
				warning(_("suboptimal pack - out of memory"));
			પાછા 0;
		}
		*mem_usage += sizeof_delta_index(src->index);
	}

	delta_buf = create_delta(src->index, trg->data, trg_size, &delta_size, max_size);
	જો (!delta_buf)
		પાછા 0;

	જો (DELTA(trg_entry)) {
		/* Prefer only shallower same-sized deltas. */
		જો (delta_size == DELTA_SIZE(trg_entry) &&
		    src->depth + 1 >= trg->depth) {
			મુક્ત(delta_buf);
			પાછા 0;
		}
	}

	/*
	 * Handle memory allocation outside of the cache
	 * accounting lock.  Compiler will optimize the strangeness
	 * away when NO_PTHREADS is defined.
	 */
	મુક્ત(trg_entry->delta_data);
	cache_lock();
	જો (trg_entry->delta_data) {
		delta_cache_size -= DELTA_SIZE(trg_entry);
		trg_entry->delta_data = ખાલી;
	}
	જો (delta_cacheable(src_size, trg_size, delta_size)) {
		delta_cache_size += delta_size;
		cache_unlock();
		trg_entry->delta_data = xrealloc(delta_buf, delta_size);
	} નહીંતર {
		cache_unlock();
		મુક્ત(delta_buf);
	}

	SET_DELTA(trg_entry, src_entry);
	SET_DELTA_SIZE(trg_entry, delta_size);
	trg->depth = src->depth + 1;

	પાછા 1;
}

સ્થિર unsigned પૂર્ણાંક check_delta_limit(સંરચના object_entry *me, unsigned પૂર્ણાંક n)
{
	સંરચના object_entry *child = DELTA_CHILD(me);
	unsigned પૂર્ણાંક m = n;
	જ્યારે (child) {
		અચલ unsigned પૂર્ણાંક c = check_delta_limit(child, n + 1);
		જો (m < c)
			m = c;
		child = DELTA_SIBLING(child);
	}
	પાછા m;
}

સ્થિર size_t free_unpacked(સંરચના unpacked *n)
{
	size_t freed_mem = sizeof_delta_index(n->index);
	free_delta_index(n->index);
	n->index = ખાલી;
	જો (n->data) {
		freed_mem += SIZE(n->entry);
		FREE_AND_NULL(n->data);
	}
	n->entry = ખાલી;
	n->depth = 0;
	પાછા freed_mem;
}

સ્થિર શૂન્ય find_deltas(સંરચના object_entry **list, unsigned *list_size,
			પૂર્ણાંક window, પૂર્ણાંક depth, unsigned *processed)
{
	uint32_t i, idx = 0, ગણતરી = 0;
	સંરચના unpacked *array;
	size_t mem_usage = 0;

	CALLOC_ARRAY(array, window);

	માટે (;;) {
		સંરચના object_entry *entry;
		સંરચના unpacked *n = array + idx;
		પૂર્ણાંક j, max_depth, best_base = -1;

		progress_lock();
		જો (!*list_size) {
			progress_unlock();
			તોડો;
		}
		entry = *list++;
		(*list_size)--;
		જો (!entry->preferred_base) {
			(*processed)++;
			display_progress(progress_state, *processed);
		}
		progress_unlock();

		mem_usage -= free_unpacked(n);
		n->entry = entry;

		જ્યારે (window_memory_limit &&
		       mem_usage > window_memory_limit &&
		       ગણતરી > 1) {
			અચલ uint32_t tail = (idx + window - ગણતરી) % window;
			mem_usage -= free_unpacked(array + tail);
			ગણતરી--;
		}

		/* We do not compute delta to *create* objects we are not
		 * going to pack.
		 */
		જો (entry->preferred_base)
			જાઓ next;

		/*
		 * If the current object is at pack edge, take the depth the
		 * objects that depend on the current object into account
		 * otherwise they would become too deep.
		 */
		max_depth = depth;
		જો (DELTA_CHILD(entry)) {
			max_depth -= check_delta_limit(entry, 0);
			જો (max_depth <= 0)
				જાઓ next;
		}

		j = window;
		જ્યારે (--j > 0) {
			પૂર્ણાંક ret;
			uint32_t other_idx = idx + j;
			સંરચના unpacked *m;
			જો (other_idx >= window)
				other_idx -= window;
			m = array + other_idx;
			જો (!m->entry)
				તોડો;
			ret = try_delta(n, m, max_depth, &mem_usage);
			જો (ret < 0)
				તોડો;
			નહીંતર જો (ret > 0)
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
		જો (entry->delta_data && !pack_to_stdout) {
			unsigned long size;

			size = do_compress(&entry->delta_data, DELTA_SIZE(entry));
			જો (size < (1U << OE_Z_DELTA_BITS)) {
				entry->z_delta_size = size;
				cache_lock();
				delta_cache_size -= DELTA_SIZE(entry);
				delta_cache_size += entry->z_delta_size;
				cache_unlock();
			} નહીંતર {
				FREE_AND_NULL(entry->delta_data);
				entry->z_delta_size = 0;
			}
		}

		/* if we made n a delta, and if n is already at max
		 * depth, leaving it in the window is pointless.  we
		 * should evict it first.
		 */
		જો (DELTA(entry) && max_depth <= n->depth)
			ચાલુ;

		/*
		 * Move the best delta base up in the window, after the
		 * currently deltified object, to keep it longer.  It will
		 * be the first base object to be attempted next.
		 */
		જો (DELTA(entry)) {
			સંરચના unpacked swap = array[best_base];
			પૂર્ણાંક dist = (window + idx - best_base) % window;
			પૂર્ણાંક dst = best_base;
			જ્યારે (dist--) {
				પૂર્ણાંક src = (dst + 1) % window;
				array[dst] = array[src];
				dst = src;
			}
			array[dst] = swap;
		}

		next:
		idx++;
		જો (ગણતરી + 1 < window)
			ગણતરી++;
		જો (idx >= window)
			idx = 0;
	}

	માટે (i = 0; i < window; ++i) {
		free_delta_index(array[i].index);
		મુક્ત(array[i].data);
	}
	મુક્ત(array);
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

સંરચના thread_params {
	pthread_t thread;
	સંરચના object_entry **list;
	સંરચના packing_region *regions;
	unsigned list_size;
	unsigned remaining;
	પૂર્ણાંક window;
	પૂર્ણાંક depth;
	પૂર્ણાંક working;
	પૂર્ણાંક data_ready;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	unsigned *processed;
};

સ્થિર pthread_cond_t progress_cond;

/*
 * Mutex and conditional variable can't be statically-initialized on Windows.
 */
સ્થિર શૂન્ય init_threaded_શોધ(શૂન્ય)
{
	pthread_mutex_init(&cache_mutex, ખાલી);
	pthread_mutex_init(&progress_mutex, ખાલી);
	pthread_cond_init(&progress_cond, ખાલી);
}

સ્થિર શૂન્ય cleanup_threaded_શોધ(શૂન્ય)
{
	pthread_cond_destroy(&progress_cond);
	pthread_mutex_destroy(&cache_mutex);
	pthread_mutex_destroy(&progress_mutex);
}

સ્થિર શૂન્ય *threaded_find_deltas(શૂન્ય *arg)
{
	સંરચના thread_params *me = arg;

	progress_lock();
	જ્યારે (me->remaining) {
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
		જ્યારે (!me->data_ready)
			pthread_cond_wait(&me->cond, &me->mutex);
		me->data_ready = 0;
		pthread_mutex_unlock(&me->mutex);

		progress_lock();
	}
	progress_unlock();
	/* leave ->working 1 so that this doesn't get more work assigned */
	પાછા ખાલી;
}

સ્થિર શૂન્ય ll_find_deltas(સંરચના object_entry **list, unsigned list_size,
			   પૂર્ણાંક window, પૂર્ણાંક depth, unsigned *processed)
{
	સંરચના thread_params *p;
	પૂર્ણાંક i, ret, active_threads = 0;

	init_threaded_શોધ();

	જો (delta_શોધ_threads <= 1) {
		find_deltas(list, &list_size, window, depth, processed);
		cleanup_threaded_શોધ();
		પાછા;
	}
	જો (progress > pack_to_stdout)
		fprintf_ln(stderr, _("Delta compression using up to %d threads"),
			   delta_શોધ_threads);
	CALLOC_ARRAY(p, delta_શોધ_threads);

	/* Partition the work amongst work threads. */
	માટે (i = 0; i < delta_શોધ_threads; i++) {
		unsigned sub_size = list_size / (delta_શોધ_threads - i);

		/* don't use too small segments or no deltas will be found */
		જો (sub_size < 2*window && i+1 < delta_શોધ_threads)
			sub_size = 0;

		p[i].window = window;
		p[i].depth = depth;
		p[i].processed = processed;
		p[i].working = 1;
		p[i].data_ready = 0;

		/* try to split chunks on "path" boundaries */
		જ્યારે (sub_size && sub_size < list_size &&
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
	માટે (i = 0; i < delta_શોધ_threads; i++) {
		જો (!p[i].list_size)
			ચાલુ;
		pthread_mutex_init(&p[i].mutex, ખાલી);
		pthread_cond_init(&p[i].cond, ખાલી);
		ret = pthread_create(&p[i].thread, ખાલી,
				     threaded_find_deltas, &p[i]);
		જો (ret)
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
	જ્યારે (active_threads) {
		સંરચના thread_params *target = ખાલી;
		સંરચના thread_params *victim = ખાલી;
		unsigned sub_size = 0;

		progress_lock();
		માટે (;;) {
			માટે (i = 0; !target && i < delta_શોધ_threads; i++)
				જો (!p[i].working)
					target = &p[i];
			જો (target)
				તોડો;
			pthread_cond_wait(&progress_cond, &progress_mutex);
		}

		માટે (i = 0; i < delta_શોધ_threads; i++)
			જો (p[i].remaining > 2*window &&
			    (!victim || victim->remaining < p[i].remaining))
				victim = &p[i];
		જો (victim) {
			sub_size = victim->remaining / 2;
			list = victim->list + victim->list_size - sub_size;
			જ્યારે (sub_size && list[0]->hash &&
			       list[0]->hash == list[-1]->hash) {
				list++;
				sub_size--;
			}
			જો (!sub_size) {
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

		જો (!sub_size) {
			pthread_join(target->thread, ખાલી);
			pthread_cond_destroy(&target->cond);
			pthread_mutex_destroy(&target->mutex);
			active_threads--;
		}
	}
	cleanup_threaded_શોધ();
	મુક્ત(p);
}

સ્થિર પૂર્ણાંક obj_is_packed(અચલ સંરચના object_id *oid)
{
	પાછા packlist_find(&to_pack, oid) ||
		(reuse_packfile_bitmap &&
		 bitmap_walk_contains(bitmap_git, reuse_packfile_bitmap, oid));
}

સ્થિર શૂન્ય add_tag_chain(અચલ સંરચના object_id *oid)
{
	સંરચના tag *tag;

	/*
	 * We catch duplicates already in add_object_entry(), but we'd
	 * prefer to do this extra check to avoid having to parse the
	 * tag at all if we already know that it's being packed (e.g., if
	 * it was included via bitmaps, we would not have parsed it
	 * previously).
	 */
	જો (obj_is_packed(oid))
		પાછા;

	tag = lookup_tag(the_repository, oid);
	જ્યારે (1) {
		જો (!tag || parse_tag(the_repository, tag) || !tag->tagged)
			die(_("unable to pack objects reachable from tag %s"),
			    oid_to_hex(oid));

		add_object_entry(&tag->object.oid, OBJ_TAG, ખાલી, 0);

		જો (tag->tagged->type != OBJ_TAG)
			પાછા;

		tag = (સંરચના tag *)tag->tagged;
	}
}

સ્થિર પૂર્ણાંક add_ref_tag(અચલ સંરચના reference *ref, શૂન્ય *cb_data UNUSED)
{
	સંરચના object_id peeled;

	જો (!reference_get_peeled_oid(the_repository, ref, &peeled) &&
	    obj_is_packed(&peeled))
		add_tag_chain(ref->oid);
	પાછા 0;
}

સ્થિર પૂર્ણાંક should_attempt_deltas(સંરચના object_entry *entry)
{
	જો (DELTA(entry))
		/* This happens if we decided to reuse existing
		 * delta from a pack. "reuse_delta &&" is implied.
		 */
		પાછા 0;

	જો (!entry->type_valid ||
	    oe_size_less_than(&to_pack, entry, 50))
		પાછા 0;

	જો (entry->no_try_delta)
		પાછા 0;

	જો (!entry->preferred_base) {
		જો (oe_type(entry) < 0)
			die(_("unable to get type of object %s"),
				oid_to_hex(&entry->idx.oid));
	} નહીંતર જો (oe_type(entry) < 0) {
		/*
		 * This object is not found, but we
		 * don't have to include it anyway.
		 */
		પાછા 0;
	}

	પાછા 1;
}

સ્થિર શૂન્ય find_deltas_for_region(સંરચના object_entry *list,
				   સંરચના packing_region *region,
				   unsigned પૂર્ણાંક *processed)
{
	સંરચના object_entry **delta_list;
	unsigned પૂર્ણાંક delta_list_nr = 0;

	ALLOC_ARRAY(delta_list, region->nr);
	માટે (size_t i = 0; i < region->nr; i++) {
		સંરચના object_entry *entry = list + region->start + i;
		જો (should_attempt_deltas(entry))
			delta_list[delta_list_nr++] = entry;
	}

	QSORT(delta_list, delta_list_nr, type_size_sort);
	find_deltas(delta_list, &delta_list_nr, window, depth, processed);
	મુક્ત(delta_list);
}

સ્થિર શૂન્ય find_deltas_by_region(સંરચના object_entry *list,
				  સંરચના packing_region *regions,
				  size_t start, size_t nr)
{
	unsigned પૂર્ણાંક processed = 0;
	size_t progress_nr;

	જો (!nr)
		પાછા;

	progress_nr = regions[nr - 1].start + regions[nr - 1].nr;

	જો (progress)
		progress_state = start_progress(the_repository,
						_("Compressing objects by path"),
						progress_nr);

	જ્યારે (nr--)
		find_deltas_for_region(list,
				       &regions[start++],
				       &processed);

	display_progress(progress_state, progress_nr);
	stop_progress(&progress_state);
}

સ્થિર શૂન્ય *threaded_find_deltas_by_path(શૂન્ય *arg)
{
	સંરચના thread_params *me = arg;

	progress_lock();
	જ્યારે (me->remaining) {
		જ્યારે (me->remaining) {
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
		જ્યારે (!me->data_ready)
			pthread_cond_wait(&me->cond, &me->mutex);
		me->data_ready = 0;
		pthread_mutex_unlock(&me->mutex);

		progress_lock();
	}
	progress_unlock();
	/* leave ->working 1 so that this doesn't get more work assigned */
	પાછા ખાલી;
}

સ્થિર શૂન્ય ll_find_deltas_by_region(સંરચના object_entry *list,
				     સંરચના packing_region *regions,
				     uint32_t start, uint32_t nr)
{
	સંરચના thread_params *p;
	પૂર્ણાંક i, ret, active_threads = 0;
	unsigned પૂર્ણાંક processed = 0;
	uint32_t progress_nr;
	init_threaded_શોધ();

	જો (!nr)
		પાછા;

	progress_nr =  regions[nr - 1].start + regions[nr - 1].nr;
	જો (delta_શોધ_threads <= 1) {
		find_deltas_by_region(list, regions, start, nr);
		cleanup_threaded_શોધ();
		પાછા;
	}

	જો (progress > pack_to_stdout)
		fprintf_ln(stderr,
			   Q_("Path-based delta compression using up to %d thread",
			      "Path-based delta compression using up to %d threads",
			      delta_શોધ_threads),
			   delta_શોધ_threads);
	CALLOC_ARRAY(p, delta_શોધ_threads);

	જો (progress)
		progress_state = start_progress(the_repository,
						_("Compressing objects by path"),
						progress_nr);
	/* Partition the work amongst work threads. */
	માટે (i = 0; i < delta_શોધ_threads; i++) {
		unsigned sub_size = nr / (delta_શોધ_threads - i);

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
	માટે (i = 0; i < delta_શોધ_threads; i++) {
		જો (!p[i].list_size)
			ચાલુ;
		pthread_mutex_init(&p[i].mutex, ખાલી);
		pthread_cond_init(&p[i].cond, ખાલી);
		ret = pthread_create(&p[i].thread, ખાલી,
				     threaded_find_deltas_by_path, &p[i]);
		જો (ret)
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
	જ્યારે (active_threads) {
		સંરચના thread_params *target = ખાલી;
		સંરચના thread_params *victim = ખાલી;
		unsigned sub_size = 0;

		progress_lock();
		માટે (;;) {
			માટે (i = 0; !target && i < delta_શોધ_threads; i++)
				જો (!p[i].working)
					target = &p[i];
			જો (target)
				તોડો;
			pthread_cond_wait(&progress_cond, &progress_mutex);
		}

		માટે (i = 0; i < delta_શોધ_threads; i++)
			જો (p[i].remaining > 2*window &&
			    (!victim || victim->remaining < p[i].remaining))
				victim = &p[i];
		જો (victim) {
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

		જો (!sub_size) {
			pthread_join(target->thread, ખાલી);
			pthread_cond_destroy(&target->cond);
			pthread_mutex_destroy(&target->mutex);
			active_threads--;
		}
	}
	cleanup_threaded_શોધ();
	મુક્ત(p);

	display_progress(progress_state, progress_nr);
	stop_progress(&progress_state);
}

સ્થિર શૂન્ય prepare_pack(પૂર્ણાંક window, પૂર્ણાંક depth)
{
	સંરચના object_entry **delta_list;
	uint32_t i, nr_deltas;
	unsigned n;

	જો (use_delta_islands)
		resolve_વૃક્ષ_islands(the_repository, progress, &to_pack);

	get_object_details();

	/*
	 * If we're locally repacking then we need to be doubly careful
	 * from now on in order to make sure no stealth corruption gets
	 * propagated to the new pack.  Clients receiving streamed packs
	 * should validate everything they get anyway so no need to incur
	 * the additional cost here in that case.
	 */
	જો (!pack_to_stdout)
		do_check_packed_object_crc = 1;

	જો (!to_pack.nr_objects || !window || !depth)
		પાછા;

	જો (path_walk)
		ll_find_deltas_by_region(to_pack.objects, to_pack.regions,
					 0, to_pack.nr_regions);

	ALLOC_ARRAY(delta_list, to_pack.nr_objects);
	nr_deltas = n = 0;

	માટે (i = 0; i < to_pack.nr_objects; i++) {
		સંરચના object_entry *entry = to_pack.objects + i;

		જો (!should_attempt_deltas(entry))
			ચાલુ;

		જો (!entry->preferred_base)
			nr_deltas++;

		delta_list[n++] = entry;
	}

	જો (nr_deltas && n > 1) {
		unsigned nr_done = 0;

		જો (progress)
			progress_state = start_progress(the_repository,
							_("Compressing objects"),
							nr_deltas);
		QSORT(delta_list, n, type_size_sort);
		ll_find_deltas(delta_list, n, window+1, depth, &nr_done);
		stop_progress(&progress_state);
		જો (nr_done != nr_deltas)
			die(_("inconsistency with delta count"));
	}
	મુક્ત(delta_list);
}

સ્થિર પૂર્ણાંક git_pack_config(અચલ અક્ષર *k, અચલ અક્ષર *v,
			   અચલ સંરચના config_context *ctx, શૂન્ય *cb)
{
	જો (!strcmp(k, "pack.window")) {
		window = git_config_int(k, v, ctx->kvi);
		પાછા 0;
	}
	જો (!strcmp(k, "pack.windowmemory")) {
		window_memory_limit = git_config_ulong(k, v, ctx->kvi);
		પાછા 0;
	}
	જો (!strcmp(k, "pack.depth")) {
		depth = git_config_int(k, v, ctx->kvi);
		પાછા 0;
	}
	જો (!strcmp(k, "pack.deltacachesize")) {
		max_delta_cache_size = git_config_size_t(k, v, ctx->kvi);
		પાછા 0;
	}
	જો (!strcmp(k, "pack.deltacachelimit")) {
		cache_max_small_delta_size = git_config_int(k, v, ctx->kvi);
		પાછા 0;
	}
	જો (!strcmp(k, "pack.writebitmaphashcache")) {
		જો (git_config_bool(k, v))
			write_bitmap_options |= BITMAP_OPT_HASH_CACHE;
		નહીંતર
			write_bitmap_options &= ~BITMAP_OPT_HASH_CACHE;
	}

	જો (!strcmp(k, "pack.writebitmaplookuptable")) {
		જો (git_config_bool(k, v))
			write_bitmap_options |= BITMAP_OPT_LOOKUP_TABLE;
		નહીંતર
			write_bitmap_options &= ~BITMAP_OPT_LOOKUP_TABLE;
	}

	જો (!strcmp(k, "pack.usebitmaps")) {
		use_bitmap_index_default = git_config_bool(k, v);
		પાછા 0;
	}
	જો (!strcmp(k, "pack.allowpackreuse")) {
		પૂર્ણાંક res = git_parse_maybe_bool_text(v);
		જો (res < 0) {
			જો (!strcasecmp(v, "single"))
				allow_pack_reuse = SINGLE_PACK_REUSE;
			નહીંતર જો (!strcasecmp(v, "multi"))
				allow_pack_reuse = MULTI_PACK_REUSE;
			નહીંતર
				die(_("invalid pack.allowPackReuse value: '%s'"), v);
		} નહીંતર જો (res) {
			allow_pack_reuse = SINGLE_PACK_REUSE;
		} નહીંતર {
			allow_pack_reuse = NO_PACK_REUSE;
		}
		પાછા 0;
	}
	જો (!strcmp(k, "pack.threads")) {
		delta_શોધ_threads = git_config_int(k, v, ctx->kvi);
		જો (delta_શોધ_threads < 0)
			die(_("invalid number of threads specified (%d)"),
			    delta_શોધ_threads);
		જો (!HAVE_THREADS && delta_શોધ_threads != 1) {
			warning(_("no threads support, ignoring %s"), k);
			delta_શોધ_threads = 0;
		}
		પાછા 0;
	}
	જો (!strcmp(k, "pack.indexversion")) {
		pack_idx_opts.version = git_config_int(k, v, ctx->kvi);
		જો (pack_idx_opts.version > 2)
			die(_("bad pack.indexVersion=%"PRIu32),
			    pack_idx_opts.version);
		પાછા 0;
	}
	જો (!strcmp(k, "pack.writereverseindex")) {
		જો (git_config_bool(k, v))
			pack_idx_opts.flags |= WRITE_REV;
		નહીંતર
			pack_idx_opts.flags &= ~WRITE_REV;
		પાછા 0;
	}
	જો (!strcmp(k, "uploadpack.blobpackfileuri")) {
		સંરચના configured_exclusion *ex;
		અચલ અક્ષર *oid_end, *pack_end;
		/*
		 * Stores the pack hash. This is not a true object ID, but is
		 * of the same form.
		 */
		સંરચના object_id pack_hash;

		જો (!v)
			પાછા config_error_nonbool(k);

		ex = xmalloc(કદ(*ex));
		જો (parse_oid_hex(v, &ex->e.oid, &oid_end) ||
		    *oid_end != ' ' ||
		    parse_oid_hex(oid_end + 1, &pack_hash, &pack_end) ||
		    *pack_end != ' ')
			die(_("value of uploadpack.blobpackfileuri must be "
			      "of the form '<object-hash> <pack-hash> <uri>' (got '%s')"), v);
		જો (oidmap_get(&configured_exclusions, &ex->e.oid))
			die(_("object already configured in another "
			      "uploadpack.blobpackfileuri (got '%s')"), v);
		ex->pack_hash_hex = xcalloc(1, pack_end - oid_end);
		memcpy(ex->pack_hash_hex, oid_end + 1, pack_end - oid_end - 1);
		ex->uri = xstrdup(pack_end + 1);
		oidmap_put(&configured_exclusions, ex);
	}
	પાછા git_default_config(k, v, ctx, cb);
}

/* Counters for trace2 output when in --stdin-packs mode. */
સ્થિર પૂર્ણાંક stdin_packs_found_nr;
સ્થિર પૂર્ણાંક stdin_packs_hints_nr;

સ્થિર પૂર્ણાંક add_object_entry_from_pack(અચલ સંરચના object_id *oid,
				      સંરચના packed_git *p,
				      uint32_t pos,
				      શૂન્ય *_data)
{
	off_t ofs;
	સંરચના object_info oi = OBJECT_INFO_INIT;
	ગણના object_type type = OBJ_NONE;

	display_progress(progress_state, ++nr_seen);

	જો (have_duplicate_entry(oid, 0))
		પાછા 0;

	stdin_packs_found_nr++;

	ofs = nth_packed_object_offset(p, pos);

	oi.typep = &type;
	જો (packed_object_info(ખાલી, p, ofs, &oi) < 0) {
		die(_("could not get type of object %s in pack %s"),
		    oid_to_hex(oid), p->pack_name);
	} નહીંતર જો (type == OBJ_COMMIT) {
		સંરચના rev_info *revs = _data;
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
		add_pending_oid(revs, ખાલી, oid, 0);
	}

	જો (!want_object_in_pack(oid, 0, &p, &ofs))
		પાછા 0;

	create_object_entry(oid, type, 0, 0, 0, p, ofs);

	પાછા 0;
}

સ્થિર શૂન્ય show_object_pack_hint(સંરચના object *object, અચલ અક્ષર *name,
				  શૂન્ય *data)
{
	ગણના stdin_packs_mode mode = *(ગણના stdin_packs_mode *)data;
	જો (mode == STDIN_PACKS_MODE_FOLLOW) {
		જો (object->type == OBJ_BLOB &&
		    !odb_has_object(the_repository->objects, &object->oid, 0))
			પાછા;
		add_object_entry(&object->oid, object->type, name, 0);
	} નહીંતર {
		સંરચના object_entry *oe = packlist_find(&to_pack, &object->oid);
		જો (!oe)
			પાછા;

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

સ્થિર શૂન્ય show_commit_pack_hint(સંરચના commit *commit, શૂન્ય *data)
{
	ગણના stdin_packs_mode mode = *(ગણના stdin_packs_mode *)data;

	જો (mode == STDIN_PACKS_MODE_FOLLOW) {
		show_object_pack_hint((સંરચના object *)commit, "", data);
		પાછા;
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
ગણના stdin_pack_info_kind {
	STDIN_PACK_INCLUDE = (1<<0),
	STDIN_PACK_EXCLUDE_CLOSED = (1<<1),
	STDIN_PACK_EXCLUDE_OPEN = (1<<2),
};

સંરચના stdin_pack_info {
	સંરચના packed_git *p;
	ગણના stdin_pack_info_kind kind;
};

સ્થિર પૂર્ણાંક pack_mtime_cmp(અચલ શૂન્ય *_a, અચલ શૂન્ય *_b)
{
	સંરચના stdin_pack_info *a = ((અચલ સંરચના string_list_item*)_a)->util;
	સંરચના stdin_pack_info *b = ((અચલ સંરચના string_list_item*)_b)->util;

	/*
	 * order packs by descending mtime so that objects are laid out
	 * roughly as newest-to-oldest
	 */
	જો (a->p->mtime < b->p->mtime)
		પાછા 1;
	નહીંતર જો (b->p->mtime < a->p->mtime)
		પાછા -1;
	નહીંતર
		પાછા 0;
}

સ્થિર પૂર્ણાંક stdin_packs_include_check_obj(સંરચના object *obj, શૂન્ય *data UNUSED)
{
	પાછા !has_object_kept_pack(to_pack.repo, &obj->oid,
				     KEPT_PACK_IN_CORE);
}

સ્થિર પૂર્ણાંક stdin_packs_include_check(સંરચના commit *commit, શૂન્ય *data)
{
	પાછા stdin_packs_include_check_obj((સંરચના object *)commit, data);
}

સ્થિર શૂન્ય stdin_packs_add_pack_entries(સંરચના strmap *packs,
					 સંરચના rev_info *revs)
{
	સંરચના string_list ચાવીઓ = STRING_LIST_INIT_NODUP;
	સંરચના string_list_item *item;
	સંરચના hashmap_iter iter;
	સંરચના strmap_entry *entry;

	strmap_for_each_entry(packs, &iter, entry) {
		સંરચના stdin_pack_info *info = entry->કિંમત;
		જો (!info->p)
			die(_("could not find pack '%s'"), entry->ચાવી);

		string_list_append(&ચાવીઓ, entry->ચાવી)->util = info;
	}

	/*
	 * Order packs by ascending mtime; use QSORT directly to access the
	 * string_list_item's ->util pointer, which string_list_sort() does not
	 * provide.
	 */
	QSORT(ચાવીઓ.વસ્તુઓ, ચાવીઓ.nr, pack_mtime_cmp);

	for_each_string_list_item(item, &ચાવીઓ) {
		સંરચના stdin_pack_info *info = item->util;

		જો (info->kind & STDIN_PACK_EXCLUDE_OPEN) {
			/*
			 * When open-excluded packs ("!") are present, stop
			 * the parent walk at closed-excluded ("^") packs.
			 * Objects behind a "^" boundary are guaranteed to
			 * have closure and should not be rescued.
			 */
			revs->include_check = stdin_packs_include_check;
			revs->include_check_obj = stdin_packs_include_check_obj;
		}

		જો ((info->kind & STDIN_PACK_INCLUDE) ||
		    (info->kind & STDIN_PACK_EXCLUDE_OPEN))
			for_each_object_in_pack(info->p,
						add_object_entry_from_pack,
						revs,
						ODB_FOR_EACH_OBJECT_PACK_ORDER);
	}

	string_list_clear(&ચાવીઓ, 0);
}

સ્થિર શૂન્ય stdin_packs_read_આગત(સંરચના rev_info *revs,
				   ગણના stdin_packs_mode mode)
{
	સંરચના strbuf buf = STRBUF_INIT;
	સંરચના strmap packs = STRMAP_INIT;
	સંરચના packed_git *p;

	જ્યારે (strbuf_getline(&buf, stdin) != EOF) {
		સંરચના stdin_pack_info *info;
		ગણના stdin_pack_info_kind kind = STDIN_PACK_INCLUDE;
		અચલ અક્ષર *ચાવી = buf.buf;

		જો (!*ચાવી)
			ચાલુ;
		નહીંતર જો (*ચાવી == '^')
			kind = STDIN_PACK_EXCLUDE_CLOSED;
		નહીંતર જો (*ચાવી == '!' && mode == STDIN_PACKS_MODE_FOLLOW)
			kind = STDIN_PACK_EXCLUDE_OPEN;

		જો (kind != STDIN_PACK_INCLUDE)
			ચાવી++;

		info = strmap_get(&packs, ચાવી);
		જો (!info) {
			CALLOC_ARRAY(info, 1);
			strmap_put(&packs, ચાવી, info);
		}

		info->kind |= kind;

		strbuf_reset(&buf);
	}

	repo_for_each_pack(the_repository, p) {
		સંરચના stdin_pack_info *info;

		info = strmap_get(&packs, pack_basename(p));
		જો (!info)
			ચાલુ;

		જો (info->kind & STDIN_PACK_INCLUDE) {
			જો (exclude_promisor_objects && p->pack_promisor)
				die(_("packfile %s is a promisor but --exclude-promisor-objects was given"), p->pack_name);

			/*
			 * Arguments we got on stdin may not even be
			 * packs. First check that to avoid segfaulting
			 * later on in e.g.  pack_mtime_cmp(), excluded
			 * packs are handled below.
			 */
			જો (!is_pack_valid(p))
				die(_("packfile %s cannot be accessed"), p->pack_name);
		}

		જો (info->kind & STDIN_PACK_EXCLUDE_CLOSED) {
			/*
			 * Marking excluded packs as kept in-core so
			 * that later calls to add_object_entry()
			 * discards any objects that are also found in
			 * excluded packs.
			 */
			p->pack_keep_in_core = 1;
		}

		જો (info->kind & STDIN_PACK_EXCLUDE_OPEN) {
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

સ્થિર શૂન્ય add_unreachable_loose_objects(સંરચના rev_info *revs);

સ્થિર શૂન્ય read_stdin_packs(સંરચના repository *repo,
			     ગણના stdin_packs_mode mode, પૂર્ણાંક rev_list_unpacked)
{
	પૂર્ણાંક prev_fetch_if_missing = repo->fetch_if_missing;
	સંરચના rev_info revs;

	/*
	 * The revision walk may hit objects that are promised, only. As the
	 * walk is best-effort though we don't want to perform backfill fetches
	 * for them.
	 */
	repo->fetch_if_missing = 0;

	repo_init_revisions(repo, &revs, ખાલી);
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
	revs.વૃક્ષ_objects = 1;
	revs.tag_objects = 1;
	revs.ignore_missing_links = 1;
	revs.exclude_promisor_objects = exclude_promisor_objects;

	/* avoids adding objects in excluded packs */
	ignore_packed_keep_in_core = 1;
	જો (mode == STDIN_PACKS_MODE_FOLLOW) {
		/*
		 * In '--stdin-packs=follow' mode, additionally ignore
		 * objects in excluded-open packs to prevent them from
		 * appearing in the resulting pack.
		 */
		ignore_packed_keep_in_core_open = 1;
	}
	stdin_packs_read_આગત(&revs, mode);
	જો (rev_list_unpacked)
		add_unreachable_loose_objects(&revs);

	જો (prepare_revision_walk(&revs))
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

સ્થિર શૂન્ય add_cruft_object_entry(અચલ સંરચના object_id *oid, ગણના object_type type,
				   સંરચના packed_git *pack, off_t offset,
				   અચલ અક્ષર *name, uint32_t mtime)
{
	સંરચના object_entry *entry;

	display_progress(progress_state, ++nr_seen);

	entry = packlist_find(&to_pack, oid);
	જો (entry) {
		જો (name) {
			entry->hash = pack_name_hash_fn(name);
			entry->no_try_delta = no_try_delta(name);
		}
	} નહીંતર {
		જો (!want_object_in_pack_mtime(oid, 0, &pack, &offset, mtime))
			પાછા;
		જો (!pack && type == OBJ_BLOB) {
			સંરચના odb_source_files *files =
				odb_source_files_downcast(the_repository->objects->source);
			પૂર્ણાંક found = 0;

			માટે (સંરચના odb_files_dir *dir = files->dirs; !found && dir; dir = dir->next)
				જો (!odb_source_read_object_info(&dir->loose->base, oid, ખાલી, 0, ખાલી))
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
			જો (!found)
				પાછા;
		}

		entry = create_object_entry(oid, type, pack_name_hash_fn(name),
					    0, name && no_try_delta(name),
					    pack, offset);
	}

	જો (mtime > oe_cruft_mtime(&to_pack, entry))
		oe_set_cruft_mtime(&to_pack, entry, mtime);
	પાછા;
}

સ્થિર શૂન્ય show_cruft_object(સંરચના object *obj, અચલ અક્ષર *name, શૂન્ય *data UNUSED)
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
	add_cruft_object_entry(&obj->oid, obj->type, ખાલી, 0, name, cruft_expiration);
}

સ્થિર શૂન્ય show_cruft_commit(સંરચના commit *commit, શૂન્ય *data)
{
	show_cruft_object((સંરચના object*)commit, ખાલી, data);
}

સ્થિર પૂર્ણાંક cruft_include_check_obj(સંરચના object *obj, શૂન્ય *data UNUSED)
{
	પાછા !has_object_kept_pack(to_pack.repo, &obj->oid, KEPT_PACK_IN_CORE);
}

સ્થિર પૂર્ણાંક cruft_include_check(સંરચના commit *commit, શૂન્ય *data)
{
	પાછા cruft_include_check_obj((સંરચના object*)commit, data);
}

સ્થિર શૂન્ય set_cruft_mtime(અચલ સંરચના object *object,
			    સંરચના packed_git *pack,
			    off_t offset, time_t mtime)
{
	add_cruft_object_entry(&object->oid, object->type, pack, offset, ખાલી,
			       mtime);
}

સ્થિર શૂન્ય mark_pack_kept_in_core(સંરચના string_list *packs, unsigned keep)
{
	સંરચના string_list_item *item = ખાલી;
	for_each_string_list_item(item, packs) {
		સંરચના packed_git *p = item->util;
		જો (!p)
			die(_("could not find pack '%s'"), item->string);
		જો (p->is_cruft && keep)
			ignore_packed_keep_in_core_has_cruft = 1;
		p->pack_keep_in_core = keep;
	}
}

સ્થિર શૂન્ય add_objects_in_unpacked_packs(શૂન્ય);

સ્થિર શૂન્ય enumerate_cruft_objects(શૂન્ય)
{
	જો (progress)
		progress_state = start_progress(the_repository,
						_("Enumerating cruft objects"), 0);

	add_objects_in_unpacked_packs();
	add_unreachable_loose_objects(ખાલી);

	stop_progress(&progress_state);
}

સ્થિર શૂન્ય enumerate_and_traverse_cruft_objects(સંરચના string_list *fresh_packs)
{
	સંરચના packed_git *p;
	સંરચના rev_info revs;
	પૂર્ણાંક ret;

	repo_init_revisions(the_repository, &revs, ખાલી);

	revs.tag_objects = 1;
	revs.વૃક્ષ_objects = 1;
	revs.blob_objects = 1;

	revs.include_check = cruft_include_check;
	revs.include_check_obj = cruft_include_check_obj;

	revs.ignore_missing_links = 1;

	જો (progress)
		progress_state = start_progress(the_repository,
						_("Enumerating cruft objects"), 0);
	ret = add_unseen_recent_objects_to_traversal(&revs, cruft_expiration,
						     set_cruft_mtime, 1);
	stop_progress(&progress_state);

	જો (ret)
		die(_("unable to add cruft objects"));

	/*
	 * Re-mark only the fresh packs as kept so that objects in
	 * unknown packs do not halt the reachability traversal early.
	 */
	repo_for_each_pack(the_repository, p)
		p->pack_keep_in_core = 0;
	mark_pack_kept_in_core(fresh_packs, 1);

	જો (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	જો (progress)
		progress_state = start_progress(the_repository,
						_("Traversing cruft objects"), 0);
	nr_seen = 0;
	traverse_commit_list(&revs, show_cruft_commit, show_cruft_object, ખાલી);

	stop_progress(&progress_state);
	release_revisions(&revs);
}

સ્થિર શૂન્ય read_cruft_objects(શૂન્ય)
{
	સંરચના strbuf buf = STRBUF_INIT;
	સંરચના string_list discard_packs = STRING_LIST_INIT_DUP;
	સંરચના string_list fresh_packs = STRING_LIST_INIT_DUP;
	સંરચના packed_git *p;

	ignore_packed_keep_in_core = 1;

	જ્યારે (strbuf_getline(&buf, stdin) != EOF) {
		જો (!buf.len)
			ચાલુ;

		જો (*buf.buf == '-')
			string_list_append(&discard_packs, buf.buf + 1);
		નહીંતર
			string_list_append(&fresh_packs, buf.buf);
	}

	string_list_sort(&discard_packs);
	string_list_sort(&fresh_packs);

	repo_for_each_pack(the_repository, p) {
		અચલ અક્ષર *pack_name = pack_basename(p);
		સંરચના string_list_item *item;

		item = string_list_lookup(&fresh_packs, pack_name);
		જો (!item)
			item = string_list_lookup(&discard_packs, pack_name);

		જો (item) {
			item->util = p;
		} નહીંતર {
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

	જો (cruft_expiration)
		enumerate_and_traverse_cruft_objects(&fresh_packs);
	નહીંતર
		enumerate_cruft_objects();

	strbuf_release(&buf);
	string_list_clear(&discard_packs, 0);
	string_list_clear(&fresh_packs, 0);
}

સ્થિર શૂન્ય read_object_list_from_stdin(શૂન્ય)
{
	અક્ષર લીટી[GIT_MAX_HEXSZ + 1 + PATH_MAX + 2];
	સંરચના object_id oid;
	અચલ અક્ષર *p;

	માટે (;;) {
		જો (!fgets(લીટી, કદ(લીટી), stdin)) {
			જો (feof(stdin))
				તોડો;
			જો (!ferror(stdin))
				BUG("fgets returned NULL, not EOF, not error!");
			જો (errno != EINTR)
				die_errno("fgets");
			clearerr(stdin);
			ચાલુ;
		}
		જો (લીટી[0] == '-') {
			જો (get_oid_hex(લીટી+1, &oid))
				die(_("expected edge object ID, got garbage:\n %s"),
				    લીટી);
			add_preferred_base(&oid);
			ચાલુ;
		}
		જો (parse_oid_hex(લીટી, &oid, &p))
			die(_("expected object ID, got garbage:\n %s"), લીટી);

		add_preferred_base_object(p + 1);
		add_object_entry(&oid, OBJ_NONE, p + 1, 0);
	}
}

સ્થિર શૂન્ય show_commit(સંરચના commit *commit, શૂન્ય *data UNUSED)
{
	add_object_entry(&commit->object.oid, OBJ_COMMIT, ખાલી, 0);

	જો (write_bitmap_index)
		index_commit_for_bitmap(commit);

	જો (use_delta_islands)
		propagate_island_marks(the_repository, commit);
}

સ્થિર શૂન્ય show_object(સંરચના object *obj, અચલ અક્ષર *name,
			શૂન્ય *data UNUSED)
{
	add_preferred_base_object(name);
	add_object_entry(&obj->oid, obj->type, name, 0);

	જો (use_delta_islands)
		record_વૃક્ષ_depth(&obj->oid, name);
}

સ્થિર શૂન્ય show_object__ma_allow_any(સંરચના object *obj, અચલ અક્ષર *name, શૂન્ય *data)
{
	પ્રતિજ્ઞા(arg_missing_action == MA_ALLOW_ANY);

	/*
	 * Quietly ignore ALL missing objects.  This avoids problems with
	 * staging them now and getting an odd error later.
	 */
	જો (!odb_has_object(the_repository->objects, &obj->oid, 0))
		પાછા;

	show_object(obj, name, data);
}

સ્થિર શૂન્ય show_object__ma_allow_promisor(સંરચના object *obj, અચલ અક્ષર *name, શૂન્ય *data)
{
	પ્રતિજ્ઞા(arg_missing_action == MA_ALLOW_PROMISOR);

	/*
	 * Quietly ignore EXPECTED missing objects.  This avoids problems with
	 * staging them now and getting an odd error later.
	 */
	જો (!odb_has_object(the_repository->objects, &obj->oid, 0) &&
	    is_promisor_object(to_pack.repo, &obj->oid))
		પાછા;

	show_object(obj, name, data);
}

સ્થિર પૂર્ણાંક option_parse_missing_action(અચલ સંરચના option *opt,
				       અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	સંરચના repository *repo = opt->કિંમત;

	પ્રતિજ્ઞા(arg);
	પ્રતિજ્ઞા(!unset);

	જો (!strcmp(arg, "error")) {
		arg_missing_action = MA_ERROR;
		fn_show_object = show_object;
		પાછા 0;
	}

	જો (!strcmp(arg, "allow-any")) {
		arg_missing_action = MA_ALLOW_ANY;
		repo->fetch_if_missing = 0;
		fn_show_object = show_object__ma_allow_any;
		પાછા 0;
	}

	જો (!strcmp(arg, "allow-promisor")) {
		arg_missing_action = MA_ALLOW_PROMISOR;
		repo->fetch_if_missing = 0;
		fn_show_object = show_object__ma_allow_promisor;
		પાછા 0;
	}

	die(_("invalid value for '%s': '%s'"), "--missing", arg);
	પાછા 0;
}

સ્થિર શૂન્ય show_edge(સંરચના commit *commit)
{
	add_preferred_base(&commit->object.oid);
}

સ્થિર પૂર્ણાંક add_object_in_unpacked_pack(અચલ સંરચના object_id *oid,
				       સંરચના object_info *oi,
				       શૂન્ય *data UNUSED)
{
	જો (cruft) {
		add_cruft_object_entry(oid, OBJ_NONE, oi->source_infop->u.packed.pack,
				       oi->source_infop->u.packed.offset, ખાલી,
				       *oi->mtimep);
	} નહીંતર {
		add_object_entry(oid, OBJ_NONE, "", 0);
	}
	પાછા 0;
}

સ્થિર શૂન્ય add_objects_in_unpacked_packs(શૂન્ય)
{
	સંરચના odb_source_files *files =
		odb_source_files_downcast(to_pack.repo->objects->source);
	time_t mtime;
	સંરચના odb_for_each_object_options opts = {
		.flags = ODB_FOR_EACH_OBJECT_PACK_ORDER |
			 ODB_FOR_EACH_OBJECT_LOCAL_ONLY |
			 ODB_FOR_EACH_OBJECT_SKIP_IN_CORE_KEPT_PACKS |
			 ODB_FOR_EACH_OBJECT_SKIP_ON_DISK_KEPT_PACKS,
	};
	સંરચના odb_source_info source_info;
	સંરચના object_info oi = {
		.mtimep = &mtime,
		.source_infop = &source_info,
	};

	માટે (સંરચના odb_files_dir *dir = files->dirs; dir; dir = dir->next) {
		જો (!dir->local)
			ચાલુ;

		જો (odb_source_for_each_object(&dir->packed->base, &oi,
					       add_object_in_unpacked_pack, ખાલી, &opts))
			die(_("cannot open pack index"));
	}
}

સ્થિર પૂર્ણાંક add_loose_object(અચલ સંરચના object_id *oid, અચલ અક્ષર *path,
			    શૂન્ય *data)
{
	સંરચના rev_info *revs = data;
	ગણના object_type type = odb_read_object_info(the_repository->objects, oid, ખાલી);

	જો (type < 0) {
		warning(_("loose object at %s could not be examined"), path);
		પાછા 0;
	}

	જો (cruft) {
		સંરચના stat st;
		જો (stat(path, &st) < 0) {
			જો (errno == ENOENT)
				પાછા 0;
			પાછા error_errno("unable to stat %s", oid_to_hex(oid));
		}

		add_cruft_object_entry(oid, type, ખાલી, 0, ખાલી,
				       st.st_mtime);
	} નહીંતર {
		add_object_entry(oid, type, "", 0);
	}

	જો (revs && type == OBJ_COMMIT)
		add_pending_oid(revs, ખાલી, oid, 0);

	પાછા 0;
}

/*
 * We actually don't even have to worry about reachability here.
 * add_object_entry will weed out duplicates, so we just add every
 * loose object we find.
 */
સ્થિર શૂન્ય add_unreachable_loose_objects(સંરચના rev_info *revs)
{
	for_each_loose_file_in_source(the_repository->objects->source,
				      add_loose_object, ખાલી, ખાલી, revs);
}

સ્થિર પૂર્ણાંક has_sha1_pack_kept_or_nonlocal(અચલ સંરચના object_id *oid)
{
	સ્થિર સંરચના packed_git *last_found = ખાલી;
	સંરચના packed_git *p;

	જો (last_found && find_pack_entry_one(oid, last_found))
		પાછા 1;

	repo_for_each_pack(the_repository, p) {
		/*
		 * We have already checked `last_found`, so there is no need to
		 * re-check here.
		 */
		જો (p == last_found)
			ચાલુ;

		જો ((!p->pack_local || p->pack_keep || p->pack_keep_in_core) &&
		    find_pack_entry_one(oid, p)) {
			last_found = p;
			પાછા 1;
		}
	}

	પાછા 0;
}

/*
 * Store a list of sha1s that are should not be discarded
 * because they are either written too recently, or are
 * reachable from another object that was.
 *
 * This is filled by get_object_list.
 */
સ્થિર સંરચના oid_array recent_objects;

સ્થિર પૂર્ણાંક loosened_object_can_be_discarded(અચલ સંરચના object_id *oid,
					    timestamp_t mtime)
{
	જો (!unpack_unreachable_expiration)
		પાછા 0;
	જો (mtime > unpack_unreachable_expiration)
		પાછા 0;
	જો (oid_array_lookup(&recent_objects, oid) >= 0)
		પાછા 0;
	પાછા 1;
}

સ્થિર પૂર્ણાંક force_object_loose(સંરચના odb_source *source,
			      અચલ સંરચના object_id *oid,
			      અચલ time_t *mtime)
{
	સંરચના odb_source_files *files = odb_source_files_downcast(source);
	અચલ સંરચના git_hash_algo *compat = source->odb->repo->compat_hash_algo;
	સંરચના object_info oi = OBJECT_INFO_INIT;
	સંરચના object_id compat_oid, *compat_oid_p = ખાલી;
	ગણના object_type type;
	શૂન્ય *buf = ખાલી;
	size_t len;
	પૂર્ણાંક ret;

	માટે (સંરચના odb_files_dir *dir = files->dirs; dir; dir = dir->next)
		જો (!odb_source_read_object_info(&dir->loose->base, oid, ખાલી, 0, ખાલી))
			પાછા 0;

	oi.typep = &type;
	oi.sizep = &len;
	oi.contentp = &buf;
	જો (odb_read_object_info_extended(source->odb, oid, &oi, 0)) {
		ret = error(_("cannot read object for %s"), oid_to_hex(oid));
		જાઓ out;
	}

	જો (compat) {
		જો (repo_oid_to_algop(source->odb->repo, oid, compat, &compat_oid)) {
			ret = error(_("cannot map object %s to %s"),
				    oid_to_hex(oid), compat->name);
			જાઓ out;
		}

		compat_oid_p = &compat_oid;
	}

	ret = odb_source_write_object(&files->dirs->loose->base, buf, len, type, oid,
				      compat_oid_p, mtime, 0);

out:
	મુક્ત(buf);
	પાછા ret;
}

સ્થિર શૂન્ય loosen_unused_packed_objects(શૂન્ય)
{
	સંરચના packed_git *p;
	uint32_t i;
	uint32_t loosened_objects_nr = 0;
	સંરચના object_id oid;

	repo_for_each_pack(the_repository, p) {
		જો (!p->pack_local || p->pack_keep || p->pack_keep_in_core)
			ચાલુ;

		જો (open_pack_index(p))
			die(_("cannot open pack index"));

		માટે (i = 0; i < p->અંક_objects; i++) {
			nth_packed_object_id(&oid, p, i);
			જો (!packlist_find(&to_pack, &oid) &&
			    !has_sha1_pack_kept_or_nonlocal(&oid) &&
			    !loosened_object_can_be_discarded(&oid, p->mtime)) {
				જો (force_object_loose(the_repository->objects->source,
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
સ્થિર પૂર્ણાંક pack_options_allow_reuse(શૂન્ય)
{
	પાછા allow_pack_reuse != NO_PACK_REUSE &&
	       pack_to_stdout &&
	       !ignore_packed_keep_on_disk &&
	       !ignore_packed_keep_in_core &&
	       (!local || !have_non_local_packs) &&
	       !incremental;
}

સ્થિર પૂર્ણાંક get_object_list_from_bitmap(સંરચના rev_info *revs)
{
	જો (!(bitmap_git = prepare_bitmap_walk(revs, 0)))
		પાછા -1;

	/*
	 * For now, force the name-hash version to be 1 since that
	 * is the version implied by the bitmap format. Later, the
	 * format can include this version explicitly in its format,
	 * allowing readers to know the version that was used during
	 * the bitmap write.
	 */
	name_hash_version = 1;

	જો (pack_options_allow_reuse())
		reuse_partial_packfile_from_bitmap(bitmap_git,
						   &reuse_packfiles,
						   &reuse_packfiles_nr,
						   &reuse_packfile_bitmap,
						   allow_pack_reuse == MULTI_PACK_REUSE);

	જો (reuse_packfiles) {
		reuse_packfile_objects = bitmap_popcount(reuse_packfile_bitmap);
		જો (!reuse_packfile_objects)
			BUG("expected non-empty reuse bitmap");

		nr_result += reuse_packfile_objects;
		nr_seen += reuse_packfile_objects;
		display_progress(progress_state, nr_seen);
	}

	traverse_bitmap_commit_list(bitmap_git, revs,
				    &add_object_entry_from_bitmap);
	પાછા 0;
}

સ્થિર શૂન્ય record_recent_object(સંરચના object *obj,
				 અચલ અક્ષર *name UNUSED,
				 શૂન્ય *data UNUSED)
{
	oid_array_append(&recent_objects, &obj->oid);
}

સ્થિર શૂન્ય record_recent_commit(સંરચના commit *commit, શૂન્ય *data UNUSED)
{
	oid_array_append(&recent_objects, &commit->object.oid);
}

સ્થિર પૂર્ણાંક mark_bitmap_preferred_tip(અચલ સંરચના reference *ref, શૂન્ય *data UNUSED)
{
	અચલ સંરચના object_id *maybe_peeled = ref->oid;
	સંરચના object_id peeled;
	સંરચના object *object;

	જો (!reference_get_peeled_oid(the_repository, ref, &peeled))
		maybe_peeled = &peeled;

	object = parse_object_or_die(the_repository, maybe_peeled, ref->name);
	જો (object->type == OBJ_COMMIT)
		object->flags |= NEEDS_BITMAP;

	પાછા 0;
}

સ્થિર અંતઃરેખ પૂર્ણાંક is_oid_uninteresting(સંરચના repository *repo,
				       સંરચના object_id *oid)
{
	સંરચના object *o = lookup_object(repo, oid);
	પાછા !o || (o->flags & UNINTERESTING);
}

સ્થિર પૂર્ણાંક add_objects_by_path(અચલ અક્ષર *path,
			       સંરચના oid_array *oids,
			       ગણના object_type type,
			       શૂન્ય *data)
{
	size_t oe_start = to_pack.nr_objects;
	size_t oe_end;
	unsigned પૂર્ણાંક *processed = data;

	/*
	 * First, add all objects to the packing data, including the ones
	 * marked UNINTERESTING (translated to 'exclude') as they can be
	 * used as delta bases.
	 */
	માટે (size_t i = 0; i < oids->nr; i++) {
		પૂર્ણાંક exclude;
		સંરચના object_info oi = OBJECT_INFO_INIT;
		સંરચના object_id *oid = &oids->oid[i];

		/* Skip objects that do not exist locally. */
		જો ((exclude_promisor_objects || arg_missing_action != MA_ERROR) &&
		    odb_read_object_info_extended(the_repository->objects, oid, &oi,
						  OBJECT_INFO_FOR_PREFETCH) < 0)
			ચાલુ;

		exclude = is_oid_uninteresting(the_repository, oid);

		જો (exclude && !thin)
			ચાલુ;

		add_object_entry(oid, type, path, exclude);

		જો (type == OBJ_COMMIT) {
			સંરચના commit *commit;

			જો (!write_bitmap_index && !use_delta_islands)
				ચાલુ;

			commit = lookup_commit(the_repository, oid);
			જો (!commit)
				die(_("could not find commit %s"), oid_to_hex(oid));
			જો (write_bitmap_index)
				index_commit_for_bitmap(commit);
			/*
			 * Skip island propagation for boundary commits.
			 * The regular traversal's show_commit() is only
			 * called for interesting commits; matching that
			 * here keeps path-walk from doing extra work that
			 * would only be a no-op anyway (boundary commits
			 * are not in island_marks).
			 */
			જો (use_delta_islands && !exclude)
				propagate_island_marks(the_repository, commit);
		} નહીંતર જો (type == OBJ_વૃક્ષ && use_delta_islands) {
			record_વૃક્ષ_depth(oid, path);
		}
	}

	oe_end = to_pack.nr_objects;

	/* We can skip delta calculations if it is a no-op. */
	જો (oe_end == oe_start || !window)
		પાછા 0;

	ALLOC_GROW(to_pack.regions,
		   to_pack.nr_regions + 1,
		   to_pack.nr_regions_alloc);

	to_pack.regions[to_pack.nr_regions].start = oe_start;
	to_pack.regions[to_pack.nr_regions].nr = oe_end - oe_start;
	to_pack.nr_regions++;

	*processed += oids->nr;
	display_progress(progress_state, *processed);

	પાછા 0;
}

સ્થિર પૂર્ણાંક get_object_list_path_walk(સંરચના rev_info *revs)
{
	સંરચના path_walk_info info = PATH_WALK_INFO_INIT;
	unsigned પૂર્ણાંક processed = 0;
	પૂર્ણાંક result;

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

	પાછા result;
}

સ્થિર શૂન્ય get_object_list(સંરચના rev_info *revs, સંરચના strvec *argv)
{
	સંરચના setup_revision_opt s_r_opt = {
		.allow_exclude_promisor_objects = 1,
	};
	સંરચના repo_config_કિંમતો *cfg = repo_config_કિંમતો(the_repository);
	અક્ષર લીટી[1000];
	પૂર્ણાંક flags = 0;
	પૂર્ણાંક save_warning;

	save_commit_buffer = 0;
	setup_revisions_from_strvec(argv, revs, &s_r_opt);

	/* make sure shallows are read */
	is_repository_shallow(the_repository);

	save_warning = cfg->warn_on_object_refname_ambiguity;
	cfg->warn_on_object_refname_ambiguity = 0;

	જ્યારે (fgets(લીટી, કદ(લીટી), stdin) != ખાલી) {
		પૂર્ણાંક len = strlen(લીટી);
		જો (len && લીટી[len - 1] == '\n')
			લીટી[--len] = 0;
		જો (!len)
			તોડો;
		જો (*લીટી == '-') {
			જો (!strcmp(લીટી, "--not")) {
				flags ^= UNINTERESTING;
				write_bitmap_index = 0;
				ચાલુ;
			}
			જો (starts_with(લીટી, "--shallow ")) {
				સંરચના object_id oid;
				જો (get_oid_hex(લીટી + 10, &oid))
					die("not an object name '%s'", લીટી + 10);
				register_shallow(the_repository, &oid);
				use_bitmap_index = 0;
				ચાલુ;
			}
			die(_("not a rev '%s'"), લીટી);
		}
		જો (handle_revision_arg(લીટી, revs, flags, REVARG_CANNOT_BE_FILENAME))
			die(_("bad revision '%s'"), લીટી);
	}

	cfg->warn_on_object_refname_ambiguity = save_warning;

	જો (use_bitmap_index && !get_object_list_from_bitmap(revs))
		પાછા;

	જો (use_delta_islands)
		load_delta_islands(the_repository, progress);

	જો (write_bitmap_index)
		for_each_preferred_bitmap_tip(the_repository, mark_bitmap_preferred_tip,
					      ખાલી);

	જો (!fn_show_object)
		fn_show_object = show_object;

	જો (path_walk) {
		જો (get_object_list_path_walk(revs)) {
			warning(_("failed to pack objects via path-walk"));
			path_walk = 0;
		}
	}

	જો (!path_walk) {
		જો (prepare_revision_walk(revs))
			die(_("revision walk setup failed"));
		mark_edges_uninteresting(revs, show_edge, sparse);
		traverse_commit_list(revs,
				show_commit, fn_show_object,
				ખાલી);
	}

	જો (unpack_unreachable_expiration) {
		revs->ignore_missing_links = 1;
		જો (add_unseen_recent_objects_to_traversal(revs,
				unpack_unreachable_expiration, ખાલી, 0))
			die(_("unable to add recent objects"));
		જો (prepare_revision_walk(revs))
			die(_("revision walk setup failed"));
		traverse_commit_list(revs, record_recent_commit,
				     record_recent_object, ખાલી);
	}

	જો (keep_unreachable)
		add_objects_in_unpacked_packs();
	જો (pack_loose_unreachable)
		add_unreachable_loose_objects(ખાલી);
	જો (unpack_unreachable)
		loosen_unused_packed_objects();

	oid_array_clear(&recent_objects);
}

સ્થિર શૂન્ય add_extra_kept_packs(અચલ સંરચના string_list *names)
{
	સંરચના packed_git *p;

	જો (!names->nr)
		પાછા;

	repo_for_each_pack(the_repository, p) {
		અચલ અક્ષર *name = basename(p->pack_name);
		પૂર્ણાંક i;

		જો (!p->pack_local)
			ચાલુ;

		માટે (i = 0; i < names->nr; i++)
			જો (!fspathcmp(name, names->વસ્તુઓ[i].string))
				તોડો;

		જો (i < names->nr) {
			p->pack_keep_in_core = 1;
			ignore_packed_keep_in_core = 1;
			ચાલુ;
		}
	}
}

સ્થિર પૂર્ણાંક option_parse_quiet(અચલ સંરચના option *opt, અચલ અક્ષર *arg,
			      પૂર્ણાંક unset)
{
	પૂર્ણાંક *val = opt->કિંમત;

	BUG_ON_OPT_ARG(arg);

	જો (!unset)
		*val = 0;
	નહીંતર જો (!*val)
		*val = 1;
	પાછા 0;
}

સ્થિર પૂર્ણાંક option_parse_index_version(અચલ સંરચના option *opt,
				      અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	સંરચના pack_idx_option *popts = opt->કિંમત;
	અક્ષર *c;
	અચલ અક્ષર *val = arg;

	BUG_ON_OPT_NEG(unset);

	popts->version = strtoul(val, &c, 10);
	જો (popts->version > 2)
		die(_("unsupported index version %s"), val);
	જો (*c == ',' && c[1])
		popts->off32_limit = strtoul(c+1, &c, 0);
	જો (*c || popts->off32_limit & 0x80000000)
		die(_("bad index version '%s'"), val);
	પાછા 0;
}

સ્થિર પૂર્ણાંક option_parse_unpack_unreachable(અચલ સંરચના option *opt UNUSED,
					   અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	જો (unset) {
		unpack_unreachable = 0;
		unpack_unreachable_expiration = 0;
	}
	નહીંતર {
		unpack_unreachable = 1;
		જો (arg)
			unpack_unreachable_expiration = approxidate(arg);
	}
	પાછા 0;
}

સ્થિર પૂર્ણાંક option_parse_cruft_expiration(અચલ સંરચના option *opt UNUSED,
					 અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	જો (unset) {
		cruft = 0;
		cruft_expiration = 0;
	} નહીંતર {
		cruft = 1;
		જો (arg)
			cruft_expiration = approxidate(arg);
	}
	પાછા 0;
}

સ્થિર પૂર્ણાંક is_not_in_promisor_pack_obj(સંરચના object *obj, શૂન્ય *data UNUSED)
{
	સંરચના odb_source_info source_info;
	સંરચના object_info info = {
		.source_infop = &source_info,
	};

	જો (odb_read_object_info_extended(the_repository->objects, &obj->oid, &info, 0))
		BUG("should_include_obj should only be called on existing objects");
	પાછા source_info.source->type != ODB_SOURCE_PACKED || !source_info.u.packed.pack->pack_promisor;
}

સ્થિર પૂર્ણાંક is_not_in_promisor_pack(સંરચના commit *commit, શૂન્ય *data) {
	પાછા is_not_in_promisor_pack_obj((સંરચના object *) commit, data);
}

સ્થિર પૂર્ણાંક parse_stdin_packs_mode(અચલ સંરચના option *opt, અચલ અક્ષર *arg,
				  પૂર્ણાંક unset)
{
	ગણના stdin_packs_mode *mode = opt->કિંમત;

	જો (unset)
		*mode = STDIN_PACKS_MODE_NONE;
	નહીંતર જો (!arg || !*arg)
		*mode = STDIN_PACKS_MODE_STANDARD;
	નહીંતર જો (!strcmp(arg, "follow"))
		*mode = STDIN_PACKS_MODE_FOLLOW;
	નહીંતર
		die(_("invalid value for '%s': '%s'"), opt->long_name, arg);

	પાછા 0;
}

પૂર્ણાંક cmd_pack_objects(પૂર્ણાંક argc,
		     અચલ અક્ષર **argv,
		     અચલ અક્ષર *prefix,
		     સંરચના repository *repo)
{
	પૂર્ણાંક use_internal_rev_list = 0;
	પૂર્ણાંક all_progress_implied = 0;
	સંરચના strvec rp = STRVEC_INIT;
	પૂર્ણાંક rev_list_unpacked = 0, rev_list_all = 0, rev_list_reflog = 0;
	પૂર્ણાંક rev_list_index = 0;
	ગણના stdin_packs_mode stdin_packs = STDIN_PACKS_MODE_NONE;
	સંરચના string_list keep_pack_list = STRING_LIST_INIT_NODUP;
	સંરચના list_objects_filter_options filter_options =
		LIST_OBJECTS_FILTER_INIT;
	સંરચના repo_config_કિંમતો *cfg = repo_config_કિંમતો(the_repository);

	સંરચના option pack_objects_options[] = {
		OPT_CALLBACK_F('q', "quiet", &progress, ખાલી,
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
		OPT_INTEGER(0, "threads", &delta_શોધ_threads,
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
		OPT_CALLBACK_F(0, "unpack-unreachable", ખાલી, N_("time"),
		  N_("unpack unreachable objects newer than <time>"),
		  PARSE_OPT_OPTARG, option_parse_unpack_unreachable),
		OPT_BOOL(0, "cruft", &cruft, N_("create a cruft pack")),
		OPT_CALLBACK_F(0, "cruft-expiration", ખાલી, N_("time"),
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

	જો (DFS_અંક_STATES > (1 << OE_DFS_STATE_BITS))
		BUG("too many dfs states, increase OE_DFS_STATE_BITS");

	disable_replace_refs();

	sparse = git_env_bool("GIT_TEST_PACK_SPARSE", -1);
	જો (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		જો (sparse < 0)
			sparse = the_repository->settings.pack_use_sparse;
		જો (the_repository->settings.pack_use_multi_pack_reuse)
			allow_pack_reuse = MULTI_PACK_REUSE;
	}

	reset_pack_idx_option(&pack_idx_opts);
	pack_idx_opts.flags |= WRITE_REV;
	repo_config(the_repository, git_pack_config, ખાલી);
	જો (git_env_bool(GIT_TEST_NO_WRITE_REV_INDEX, 0))
		pack_idx_opts.flags &= ~WRITE_REV;

	progress = isatty(2);
	argc = parse_options(argc, argv, prefix, pack_objects_options,
			     pack_usage, 0);

	જો (argc) {
		base_name = argv[0];
		argc--;
	}
	જો (pack_to_stdout != !base_name || argc)
		usage_with_options(pack_usage, pack_objects_options);

	જો (path_walk < 0) {
		જો (use_bitmap_index > 0 ||
		    !use_internal_rev_list)
			path_walk = 0;
		નહીંતર જો (the_repository->gitdir &&
			 the_repository->settings.pack_use_path_walk)
			path_walk = 1;
		નહીંતર
			path_walk = git_env_bool("GIT_TEST_PACK_PATH_WALK", 0);
	}

	જો (depth < 0)
		depth = 0;
	જો (depth >= (1 << OE_DEPTH_BITS)) {
		warning(_("delta chain depth %d is too deep, forcing %d"),
			depth, (1 << OE_DEPTH_BITS) - 1);
		depth = (1 << OE_DEPTH_BITS) - 1;
	}
	જો (cache_max_small_delta_size >= (1U << OE_Z_DELTA_BITS)) {
		warning(_("pack.deltaCacheLimit is too high, forcing %d"),
			(1U << OE_Z_DELTA_BITS) - 1);
		cache_max_small_delta_size = (1U << OE_Z_DELTA_BITS) - 1;
	}
	જો (window < 0)
		window = 0;

	strvec_ધકેલો(&rp, "pack-objects");

	જો (path_walk) {
		અચલ અક્ષર *option = ખાલી;
		જો (!path_walk_filter_compatible(&filter_options))
			option = "--filter";

		જો (option) {
			warning(_("cannot use %s with %s"),
				option, "--path-walk");
			path_walk = 0;
		}
	}
	જો (path_walk) {
		strvec_ધકેલો(&rp, "--objects");
	} નહીંતર જો (thin) {
		use_internal_rev_list = 1;
		strvec_ધકેલો(&rp, shallow
				? "--objects-edge-aggressive"
				: "--objects-edge");
	} નહીંતર
		strvec_ધકેલો(&rp, "--objects");

	જો (rev_list_all) {
		use_internal_rev_list = 1;
		strvec_ધકેલો(&rp, "--all");
	}
	જો (rev_list_reflog) {
		use_internal_rev_list = 1;
		strvec_ધકેલો(&rp, "--reflog");
	}
	જો (rev_list_index) {
		use_internal_rev_list = 1;
		strvec_ધકેલો(&rp, "--indexed-objects");
	}
	જો (rev_list_unpacked && !stdin_packs) {
		use_internal_rev_list = 1;
		strvec_ધકેલો(&rp, "--unpacked");
	}

	die_for_incompatible_opt2(exclude_promisor_objects,
				  "--exclude-promisor-objects",
				  exclude_promisor_objects_best_effort,
				  "--exclude-promisor-objects-best-effort");
	જો (exclude_promisor_objects) {
		repo->fetch_if_missing = 0;

		/* --stdin-packs handles promisor objects separately. */
		જો (!stdin_packs) {
			use_internal_rev_list = 1;
			strvec_ધકેલો(&rp, "--exclude-promisor-objects");
		}
	} નહીંતર જો (exclude_promisor_objects_best_effort) {
		use_internal_rev_list = 1;
		arg_missing_action = MA_ALLOW_ANY;
		repo->fetch_if_missing = 0;
		fn_show_object = show_object__ma_allow_any;
		/* revs configured below */
	}
	જો (unpack_unreachable || keep_unreachable || pack_loose_unreachable)
		use_internal_rev_list = 1;

	જો (!reuse_object)
		reuse_delta = 0;
	જો (cfg->pack_compression_level == -1)
		cfg->pack_compression_level = Z_DEFAULT_COMPRESSION;
	નહીંતર જો (cfg->pack_compression_level < 0 || cfg->pack_compression_level > Z_BEST_COMPRESSION)
		die(_("bad pack compression level %d"), cfg->pack_compression_level);

	જો (!delta_શોધ_threads)	/* --threads=0 means autodetect */
		delta_શોધ_threads = online_cpus();

	જો (!HAVE_THREADS && delta_શોધ_threads != 1)
		warning(_("no threads support, ignoring --threads"));
	જો (!pack_to_stdout && !pack_size_limit)
		pack_size_limit = pack_size_limit_cfg;
	જો (pack_to_stdout && pack_size_limit)
		die(_("--max-pack-size cannot be used to build a pack for transfer"));
	જો (pack_size_limit && pack_size_limit < 1024*1024) {
		warning(_("minimum pack size limit is 1 MiB"));
		pack_size_limit = 1024*1024;
	}

	જો (!pack_to_stdout && thin)
		die(_("--thin cannot be used to build an indexable pack"));

	die_for_incompatible_opt2(keep_unreachable, "--keep-unreachable",
				  unpack_unreachable, "--unpack-unreachable");
	જો (!rev_list_all || !rev_list_reflog || !rev_list_index)
		unpack_unreachable_expiration = 0;

	die_for_incompatible_opt2(stdin_packs, "--stdin-packs",
				  filter_options.choice, "--filter");


	જો (stdin_packs && use_internal_rev_list)
		die(_("cannot use internal rev list with --stdin-packs"));

	જો (cruft) {
		જો (use_internal_rev_list)
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
	જો (!pack_to_stdout)
		use_bitmap_index_default = 0;

	જો (use_bitmap_index < 0)
		use_bitmap_index = use_bitmap_index_default;

	/* "hard" reasons not to use bitmaps; these just won't work at all */
	જો (!use_internal_rev_list || (!pack_to_stdout && write_bitmap_index) || is_repository_shallow(the_repository))
		use_bitmap_index = 0;

	જો (pack_to_stdout || !rev_list_all)
		write_bitmap_index = 0;

	જો (name_hash_version < 0)
		name_hash_version = (પૂર્ણાંક)git_env_ulong("GIT_TEST_NAME_HASH_VERSION", 1);

	validate_name_hash_version();

	જો (use_delta_islands)
		strvec_ધકેલો(&rp, "--topo-order");

	જો (progress && all_progress_implied)
		progress = 2;

	add_extra_kept_packs(&keep_pack_list);
	જો (ignore_packed_keep_on_disk) {
		સંરચના packed_git *p;

		repo_for_each_pack(the_repository, p)
			જો (p->pack_local && p->pack_keep)
				તોડો;
		જો (!p) /* no keep-able packs found */
			ignore_packed_keep_on_disk = 0;
	}
	જો (local) {
		/*
		 * unlike ignore_packed_keep_on_disk above, we do not
		 * want to unset "local" based on looking at packs, as
		 * it also covers non-local objects
		 */
		સંરચના packed_git *p;

		repo_for_each_pack(the_repository, p) {
			જો (!p->pack_local) {
				have_non_local_packs = 1;
				તોડો;
			}
		}
	}

	trace2_region_enter("pack-objects", "enumerate-objects",
			    the_repository);
	prepare_packing_data(the_repository, &to_pack);

	જો (progress && !cruft)
		progress_state = start_progress(the_repository,
						_("Enumerating objects"), 0);
	જો (stdin_packs) {
		read_stdin_packs(repo, stdin_packs, rev_list_unpacked);
	} નહીંતર જો (cruft) {
		read_cruft_objects();
	} નહીંતર જો (!use_internal_rev_list) {
		read_object_list_from_stdin();
	} નહીંતર {
		સંરચના rev_info revs;

		repo_init_revisions(the_repository, &revs, ખાલી);
		list_objects_filter_copy(&revs.filter, &filter_options);
		જો (exclude_promisor_objects_best_effort) {
			revs.include_check = is_not_in_promisor_pack;
			revs.include_check_obj = is_not_in_promisor_pack_obj;
		}
		get_object_list(&revs, &rp);
		release_revisions(&revs);
	}
	cleanup_preferred_base();
	જો (include_tag && nr_result)
		refs_for_each_tag_ref(get_main_ref_store(the_repository),
				      add_ref_tag, ખાલી);
	stop_progress(&progress_state);
	trace2_region_leave("pack-objects", "enumerate-objects",
			    the_repository);

	જો (non_empty && !nr_result)
		જાઓ cleanup;
	જો (nr_result) {
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

	જો (progress)
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

	પાછા 0;
}
