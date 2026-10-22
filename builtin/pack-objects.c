#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "environment.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "hex.h"
#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ "attr.h"
#അന്തര്ഭാവ "object.h"
#അന്തര്ഭാവ "commit.h"
#അന്തര്ഭാവ "tag.h"
#അന്തര്ഭാവ "delta.h"
#അന്തര്ഭാവ "pack.h"
#അന്തര്ഭാവ "pack-revindex.h"
#അന്തര്ഭാവ "csum-file.h"
#അന്തര്ഭാവ "tree-walk.h"
#അന്തര്ഭാവ "diff.h"
#അന്തര്ഭാവ "revision.h"
#അന്തര്ഭാവ "list-objects.h"
#അന്തര്ഭാവ "list-objects-filter-options.h"
#അന്തര്ഭാവ "pack-objects.h"
#അന്തര്ഭാവ "progress.h"
#അന്തര്ഭാവ "refs.h"
#അന്തര്ഭാവ "thread-utils.h"
#അന്തര്ഭാവ "pack-bitmap.h"
#അന്തര്ഭാവ "delta-islands.h"
#അന്തര്ഭാവ "reachable.h"
#അന്തര്ഭാവ "oid-array.h"
#അന്തര്ഭാവ "strvec.h"
#അന്തര്ഭാവ "strmap.h"
#അന്തര്ഭാവ "list.h"
#അന്തര്ഭാവ "packfile.h"
#അന്തര്ഭാവ "object-file.h"
#അന്തര്ഭാവ "object-file-convert.h"
#അന്തര്ഭാവ "odb.h"
#അന്തര്ഭാവ "odb/streaming.h"
#അന്തര്ഭാവ "replace-object.h"
#അന്തര്ഭാവ "dir.h"
#അന്തര്ഭാവ "midx.h"
#അന്തര്ഭാവ "trace2.h"
#അന്തര്ഭാവ "shallow.h"
#അന്തര്ഭാവ "promisor-remote.h"
#അന്തര്ഭാവ "pack-mtimes.h"
#അന്തര്ഭാവ "parse-options.h"
#അന്തര്ഭാവ "pkt-line.h"
#അന്തര്ഭാവ "blob.h"
#അന്തര്ഭാവ "tree.h"
#അന്തര്ഭാവ "path-walk.h"

/*
 * Objects we are going to pack are collected in the `to_pack` structure.
 * It contains an array (dynamically expanded) of the object data, and a map
 * that can resolve SHA1s to their position in the array.
 */
സ്ഥിതം ഘടന packing_data to_pack;

സ്ഥിതം വരിയിൽ ഘടന object_entry *oe_delta(
		സ്ഥിരം ഘടന packing_data *pack,
		സ്ഥിരം ഘടന object_entry *e)
{
	എങ്കിൽ (!e->delta_idx)
		തിരികെ ശൂന്യ;
	എങ്കിൽ (e->ext_base)
		തിരികെ &pack->ext_bases[e->delta_idx - 1];
	അല്ലെങ്കിൽ
		തിരികെ &pack->objects[e->delta_idx - 1];
}

സ്ഥിതം വരിയിൽ size_t oe_delta_size(ഘടന packing_data *pack,
				   സ്ഥിരം ഘടന object_entry *e)
{
	എങ്കിൽ (e->delta_size_valid)
		തിരികെ e->delta_size_;

	/*
	 * pack->delta_size[] can't be NULL because oe_set_delta_size()
	 * must have been called when a new delta is saved with
	 * oe_set_delta().
	 * If oe_delta() returns NULL (i.e. default state, which means
	 * delta_size_valid is also false), then the caller must never
	 * call oe_delta_size().
	 */
	തിരികെ pack->delta_size[e - pack->objects];
}

size_t oe_get_size_slow(ഘടന packing_data *pack,
			സ്ഥിരം ഘടന object_entry *e);

സ്ഥിതം വരിയിൽ size_t oe_size(ഘടന packing_data *pack,
			     സ്ഥിരം ഘടന object_entry *e)
{
	എങ്കിൽ (e->size_valid)
		തിരികെ e->size_;

	തിരികെ oe_get_size_slow(pack, e);
}

സ്ഥിതം വരിയിൽ ശൂന്യം oe_set_delta(ഘടന packing_data *pack,
				ഘടന object_entry *e,
				ഘടന object_entry *delta)
{
	എങ്കിൽ (delta)
		e->delta_idx = (delta - pack->objects) + 1;
	അല്ലെങ്കിൽ
		e->delta_idx = 0;
}

സ്ഥിതം വരിയിൽ ഘടന object_entry *oe_delta_sibling(
		സ്ഥിരം ഘടന packing_data *pack,
		സ്ഥിരം ഘടന object_entry *e)
{
	എങ്കിൽ (e->delta_sibling_idx)
		തിരികെ &pack->objects[e->delta_sibling_idx - 1];
	തിരികെ ശൂന്യ;
}

സ്ഥിതം വരിയിൽ ഘടന object_entry *oe_delta_child(
		സ്ഥിരം ഘടന packing_data *pack,
		സ്ഥിരം ഘടന object_entry *e)
{
	എങ്കിൽ (e->delta_child_idx)
		തിരികെ &pack->objects[e->delta_child_idx - 1];
	തിരികെ ശൂന്യ;
}

സ്ഥിതം വരിയിൽ ശൂന്യം oe_set_delta_child(ഘടന packing_data *pack,
				      ഘടന object_entry *e,
				      ഘടന object_entry *delta)
{
	എങ്കിൽ (delta)
		e->delta_child_idx = (delta - pack->objects) + 1;
	അല്ലെങ്കിൽ
		e->delta_child_idx = 0;
}

സ്ഥിതം വരിയിൽ ശൂന്യം oe_set_delta_sibling(ഘടന packing_data *pack,
					ഘടന object_entry *e,
					ഘടന object_entry *delta)
{
	എങ്കിൽ (delta)
		e->delta_sibling_idx = (delta - pack->objects) + 1;
	അല്ലെങ്കിൽ
		e->delta_sibling_idx = 0;
}

സ്ഥിതം വരിയിൽ ശൂന്യം oe_set_size(ഘടന packing_data *pack,
			       ഘടന object_entry *e,
			       size_t size)
{
	എങ്കിൽ (size < pack->oe_size_limit) {
		e->size_ = size;
		e->size_valid = 1;
	} അല്ലെങ്കിൽ {
		e->size_valid = 0;
		എങ്കിൽ (oe_get_size_slow(pack, e) != size)
			BUG("'size' is supposed to be the object size!");
	}
}

സ്ഥിതം വരിയിൽ ശൂന്യം oe_set_delta_size(ഘടന packing_data *pack,
				     ഘടന object_entry *e,
				     size_t size)
{
	എങ്കിൽ (size < pack->oe_delta_size_limit) {
		e->delta_size_ = size;
		e->delta_size_valid = 1;
	} അല്ലെങ്കിൽ {
		packing_data_lock(pack);
		എങ്കിൽ (!pack->delta_size)
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

സ്ഥിതം സ്ഥിരം അക്ഷരം *സ്ഥിരം pack_usage[] = {
	N_("git pack-objects [-q | --progress | --all-progress] [--all-progress-implied]\n"
	   "                 [--no-reuse-delta] [--delta-base-offset] [--non-empty]\n"
	   "                 [--local] [--incremental] [--window=<n>] [--depth=<n>]\n"
	   "                 [--revs [--unpacked | --all]] [--keep-pack=<pack-name>]\n"
	   "                 [--cruft] [--cruft-expiration=<time>]\n"
	   "                 [--stdout [--filter=<filter-spec>] | <base-name>]\n"
	   "                 [--shallow] [--keep-true-parents] [--[no-]sparse]\n"
	   "                 [--name-hash-version=<n>] [--path-walk] < <object-list>"),
	ശൂന്യ
};

സ്ഥിതം ഘടന pack_idx_entry **written_list;
സ്ഥിതം uint32_t nr_result, nr_written, nr_seen;
സ്ഥിതം ഘടന bitmap_index *bitmap_git;
സ്ഥിതം uint32_t write_layer;

സ്ഥിതം പൂർണ്ണസംഖ്യ non_empty;
സ്ഥിതം പൂർണ്ണസംഖ്യ reuse_delta = 1, reuse_object = 1;
സ്ഥിതം പൂർണ്ണസംഖ്യ keep_unreachable, unpack_unreachable, include_tag;
സ്ഥിതം timestamp_t unpack_unreachable_expiration;
സ്ഥിതം പൂർണ്ണസംഖ്യ pack_loose_unreachable;
സ്ഥിതം പൂർണ്ണസംഖ്യ cruft;
സ്ഥിതം പൂർണ്ണസംഖ്യ shallow = 0;
സ്ഥിതം timestamp_t cruft_expiration;
സ്ഥിതം പൂർണ്ണസംഖ്യ local;
സ്ഥിതം പൂർണ്ണസംഖ്യ have_non_local_packs;
സ്ഥിതം പൂർണ്ണസംഖ്യ incremental;
സ്ഥിതം പൂർണ്ണസംഖ്യ ignore_packed_keep_on_disk;
സ്ഥിതം പൂർണ്ണസംഖ്യ ignore_packed_keep_in_core;
സ്ഥിതം പൂർണ്ണസംഖ്യ ignore_packed_keep_in_core_open;
സ്ഥിതം പൂർണ്ണസംഖ്യ ignore_packed_keep_in_core_has_cruft;
സ്ഥിതം പൂർണ്ണസംഖ്യ allow_ofs_delta;
സ്ഥിതം ഘടന pack_idx_option pack_idx_opts;
സ്ഥിതം സ്ഥിരം അക്ഷരം *base_name;
സ്ഥിതം പൂർണ്ണസംഖ്യ progress = 1;
സ്ഥിതം പൂർണ്ണസംഖ്യ window = 10;
സ്ഥിതം unsigned long pack_size_limit;
സ്ഥിതം പൂർണ്ണസംഖ്യ depth = 50;
സ്ഥിതം പൂർണ്ണസംഖ്യ delta_തിരയൽ_threads;
സ്ഥിതം പൂർണ്ണസംഖ്യ pack_to_stdout;
സ്ഥിതം പൂർണ്ണസംഖ്യ sparse;
സ്ഥിതം പൂർണ്ണസംഖ്യ thin;
സ്ഥിതം പൂർണ്ണസംഖ്യ path_walk = -1;
സ്ഥിതം പൂർണ്ണസംഖ്യ അക്കം_preferred_base;
സ്ഥിതം ഘടന progress *progress_state;

സ്ഥിതം ഘടന bitmapped_pack *reuse_packfiles;
സ്ഥിതം size_t reuse_packfiles_nr;
സ്ഥിതം size_t reuse_packfiles_used_nr;
സ്ഥിതം uint32_t reuse_packfile_objects;
സ്ഥിതം ഘടന bitmap *reuse_packfile_bitmap;

സ്ഥിതം പൂർണ്ണസംഖ്യ use_bitmap_index_default = 1;
സ്ഥിതം പൂർണ്ണസംഖ്യ use_bitmap_index = -1;
സ്ഥിതം എണ്ണൽ {
	NO_PACK_REUSE = 0,
	SINGLE_PACK_REUSE,
	MULTI_PACK_REUSE,
} allow_pack_reuse = SINGLE_PACK_REUSE;
സ്ഥിതം എണ്ണൽ {
	WRITE_BITMAP_FALSE = 0,
	WRITE_BITMAP_QUIET,
	WRITE_BITMAP_TRUE,
} write_bitmap_index;
സ്ഥിതം uint16_t write_bitmap_options = BITMAP_OPT_HASH_CACHE;

സ്ഥിതം പൂർണ്ണസംഖ്യ exclude_promisor_objects;
സ്ഥിതം പൂർണ്ണസംഖ്യ exclude_promisor_objects_best_effort;

സ്ഥിതം പൂർണ്ണസംഖ്യ use_delta_islands;

സ്ഥിതം size_t delta_cache_size = 0;
സ്ഥിതം size_t max_delta_cache_size = DEFAULT_DELTA_CACHE_SIZE;
സ്ഥിതം unsigned long cache_max_small_delta_size = 1000;

സ്ഥിതം unsigned long window_memory_limit = 0;

സ്ഥിതം ഘടന string_list uri_protocols = STRING_LIST_INIT_NODUP;

എണ്ണൽ missing_action {
	MA_ERROR = 0,      /* fail if any missing objects are encountered */
	MA_ALLOW_ANY,      /* silently allow ALL missing objects */
	MA_ALLOW_PROMISOR, /* silently allow all missing PROMISOR objects */
};
സ്ഥിതം എണ്ണൽ missing_action arg_missing_action;
സ്ഥിതം show_object_fn fn_show_object;

ഘടന configured_exclusion {
	ഘടന oidmap_entry e;
	അക്ഷരം *pack_hash_hex;
	അക്ഷരം *uri;
};
സ്ഥിതം ഘടന oidmap configured_exclusions;

സ്ഥിതം ഘടന oidset excluded_by_config;
സ്ഥിതം പൂർണ്ണസംഖ്യ name_hash_version = -1;

എണ്ണൽ stdin_packs_mode {
	STDIN_PACKS_MODE_NONE,
	STDIN_PACKS_MODE_STANDARD,
	STDIN_PACKS_MODE_FOLLOW,
};

/**
 * Check whether the name_hash_version chosen by user input is appropriate,
 * and also validate whether it is compatible with other features.
 */
സ്ഥിതം ശൂന്യം validate_name_hash_version(ശൂന്യം)
{
	എങ്കിൽ (name_hash_version < 1 || name_hash_version > 2)
		die(_("invalid --name-hash-version option: %d"), name_hash_version);
	എങ്കിൽ (write_bitmap_index && name_hash_version != 1) {
		warning(_("currently, --write-bitmap-index requires --name-hash-version=1"));
		name_hash_version = 1;
	}
}

സ്ഥിതം വരിയിൽ uint32_t pack_name_hash_fn(സ്ഥിരം അക്ഷരം *name)
{
	സ്ഥിതം പൂർണ്ണസംഖ്യ seen_version = -1;

	എങ്കിൽ (seen_version < 0)
		seen_version = name_hash_version;
	അല്ലെങ്കിൽ എങ്കിൽ (seen_version != name_hash_version)
		BUG("name hash version changed from %d to %d mid-process",
		    seen_version, name_hash_version);

	തിരഞ്ഞെടുപ്പ് (name_hash_version) {
	അവസ്ഥ 1:
		തിരികെ pack_name_hash(name);

	അവസ്ഥ 2:
		തിരികെ pack_name_hash_v2((സ്ഥിരം unsigned അക്ഷരം *)name);

	സ്ഥിരസ്ഥിതി:
		BUG("invalid name-hash version: %d", name_hash_version);
	}
}

/*
 * stats
 */
സ്ഥിതം uint32_t written, written_delta;
സ്ഥിതം uint32_t reused, reused_delta;

/*
 * Indexed commits
 */
സ്ഥിതം ഘടന commit **indexed_commits;
സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ indexed_commits_nr;
സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ indexed_commits_alloc;

സ്ഥിതം ശൂന്യം index_commit_for_bitmap(ഘടന commit *commit)
{
	എങ്കിൽ (indexed_commits_nr >= indexed_commits_alloc) {
		indexed_commits_alloc = (indexed_commits_alloc + 32) * 2;
		REALLOC_ARRAY(indexed_commits, indexed_commits_alloc);
	}

	indexed_commits[indexed_commits_nr++] = commit;
}

സ്ഥിതം ശൂന്യം *get_delta(ഘടന object_entry *entry)
{
	unsigned long size, base_size;
	size_t delta_size;
	ശൂന്യം *buf, *base_buf, *delta_buf;
	എണ്ണൽ object_type type;
	size_t size_st = 0, base_size_st = 0;

	buf = odb_read_object(the_repository->objects, &entry->idx.oid,
			      &type, &size_st);
	size = cast_size_t_to_ulong(size_st);
	എങ്കിൽ (!buf)
		die(_("unable to read %s"), oid_to_hex(&entry->idx.oid));
	base_buf = odb_read_object(the_repository->objects,
				   &DELTA(entry)->idx.oid, &type,
				   &base_size_st);
	base_size = cast_size_t_to_ulong(base_size_st);
	എങ്കിൽ (!base_buf)
		die("unable to read %s",
		    oid_to_hex(&DELTA(entry)->idx.oid));
	delta_buf = diff_delta(base_buf, base_size,
			       buf, size, &delta_size, 0);
	/*
	 * We successfully computed this delta once but dropped it for
	 * memory reasons. Something is very wrong if this time we
	 * recompute and create a different delta.
	 */
	എങ്കിൽ (!delta_buf || delta_size != DELTA_SIZE(entry))
		BUG("delta size changed");
	സ്വതന്ത്രം(buf);
	സ്വതന്ത്രം(base_buf);
	തിരികെ delta_buf;
}

സ്ഥിതം unsigned long do_compress(ശൂന്യം **pptr, unsigned long size)
{
	git_zstream stream;
	ശൂന്യം *in, *out;
	unsigned long maxsize;
	ഘടന repo_config_മൂല്യങ്ങൾ *cfg = repo_config_മൂല്യങ്ങൾ(the_repository);

	git_deflate_init(&stream, cfg->pack_compression_level);
	maxsize = git_deflate_bound(&stream, size);

	in = *pptr;
	out = xmalloc(maxsize);
	*pptr = out;

	stream.next_in = in;
	stream.avail_in = size;
	stream.next_out = out;
	stream.avail_out = maxsize;
	ആകുമ്പോൾ (git_deflate(&stream, Z_FINISH) == Z_OK)
		; /* nothing */
	git_deflate_end(&stream);

	സ്വതന്ത്രം(in);
	തിരികെ stream.total_out;
}

സ്ഥിതം unsigned long write_large_blob_data(ഘടന odb_stream *st, ഘടന hashfile *f,
					   സ്ഥിരം ഘടന object_id *oid)
{
	git_zstream stream;
	unsigned അക്ഷരം ibuf[1024 * 16];
	unsigned അക്ഷരം obuf[1024 * 16];
	unsigned long olen = 0;
	ഘടന repo_config_മൂല്യങ്ങൾ *cfg = repo_config_മൂല്യങ്ങൾ(the_repository);

	git_deflate_init(&stream, cfg->pack_compression_level);

	വേണ്ടി (;;) {
		ssize_t readlen;
		പൂർണ്ണസംഖ്യ zret = Z_OK;
		readlen = odb_stream_read(st, ibuf, വലുപ്പം(ibuf));
		എങ്കിൽ (readlen == -1)
			die(_("unable to read %s"), oid_to_hex(oid));

		stream.next_in = ibuf;
		stream.avail_in = readlen;
		ആകുമ്പോൾ ((stream.avail_in || readlen == 0) &&
		       (zret == Z_OK || zret == Z_BUF_ERROR)) {
			stream.next_out = obuf;
			stream.avail_out = വലുപ്പം(obuf);
			zret = git_deflate(&stream, readlen ? 0 : Z_FINISH);
			hashwrite(f, obuf, stream.next_out - obuf);
			olen += stream.next_out - obuf;
		}
		എങ്കിൽ (stream.avail_in)
			die(_("deflate error (%d)"), zret);
		എങ്കിൽ (readlen == 0) {
			എങ്കിൽ (zret != Z_STREAM_END)
				die(_("deflate error (%d)"), zret);
			നിർത്തുക;
		}
	}
	git_deflate_end(&stream);
	തിരികെ olen;
}

/*
 * we are going to reuse the existing object data as is.  make
 * sure it is not corrupt.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ check_pack_inflate(ഘടന packed_git *p,
		ഘടന pack_window **w_curs,
		off_t offset,
		off_t len,
		size_t expect)
{
	git_zstream stream;
	unsigned അക്ഷരം fakebuf[4096], *in;
	പൂർണ്ണസംഖ്യ st;

	memset(&stream, 0, വലുപ്പം(stream));
	git_inflate_init(&stream);
	ചെയ്യുക {
		in = use_pack(p, w_curs, offset, &stream.avail_in);
		stream.next_in = in;
		stream.next_out = fakebuf;
		stream.avail_out = വലുപ്പം(fakebuf);
		st = git_inflate(&stream, Z_FINISH);
		offset += stream.next_in - in;
	} ആകുമ്പോൾ (st == Z_OK || st == Z_BUF_ERROR);
	git_inflate_end(&stream);
	തിരികെ (st == Z_STREAM_END &&
		stream.total_out == expect &&
		stream.total_in == len) ? 0 : -1;
}

സ്ഥിതം ശൂന്യം copy_pack_data(ഘടന hashfile *f,
		ഘടന packed_git *p,
		ഘടന pack_window **w_curs,
		off_t offset,
		off_t len)
{
	unsigned അക്ഷരം *in;
	size_t avail;

	ആകുമ്പോൾ (len) {
		in = use_pack(p, w_curs, offset, &avail);
		എങ്കിൽ (avail > len)
			avail = (unsigned long)len;
		hashwrite(f, in, avail);
		offset += avail;
		len -= avail;
	}
}

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ oe_size_greater_than(ഘടന packing_data *pack,
				       സ്ഥിരം ഘടന object_entry *lhs,
				       size_t rhs)
{
	എങ്കിൽ (lhs->size_valid)
		തിരികെ lhs->size_ > rhs;
	എങ്കിൽ (rhs < pack->oe_size_limit) /* rhs < 2^x <= lhs ? */
		തിരികെ 1;
	തിരികെ oe_get_size_slow(pack, lhs) > rhs;
}

/* Return 0 if we will bust the pack-size limit */
സ്ഥിതം unsigned long write_no_reuse_object(ഘടന hashfile *f, ഘടന object_entry *entry,
					   unsigned long limit, പൂർണ്ണസംഖ്യ usable_delta)
{
	unsigned long size, datalen;
	unsigned അക്ഷരം header[MAX_PACK_OBJECT_HEADER],
		      dheader[MAX_PACK_OBJECT_HEADER];
	unsigned hdrlen;
	എണ്ണൽ object_type type;
	ശൂന്യം *buf;
	ഘടന odb_stream *st = ശൂന്യ;
	സ്ഥിരം unsigned hashsz = the_hash_algo->rawsz;

	എങ്കിൽ (!usable_delta) {
		എങ്കിൽ (oe_type(entry) == OBJ_BLOB &&
		    oe_size_greater_than(&to_pack, entry,
					 repo_settings_get_big_file_threshold(the_repository)) &&
		    (st = odb_stream_from_object(the_repository->objects, &entry->idx.oid,
						 ശൂന്യ)) != ശൂന്യ) {
			buf = ശൂന്യ;
			type = st->type;
			size = st->size;
		} അല്ലെങ്കിൽ {
			size_t size_st = 0;
			buf = odb_read_object(the_repository->objects,
					      &entry->idx.oid, &type,
					      &size_st);
			size = cast_size_t_to_ulong(size_st);
			എങ്കിൽ (!buf)
				die(_("unable to read %s"),
				    oid_to_hex(&entry->idx.oid));
		}
		/*
		 * make sure no cached delta data remains from a
		 * previous attempt before a pack split occurred.
		 */
		FREE_AND_NULL(entry->delta_data);
		entry->z_delta_size = 0;
	} അല്ലെങ്കിൽ എങ്കിൽ (entry->delta_data) {
		size = DELTA_SIZE(entry);
		buf = entry->delta_data;
		entry->delta_data = ശൂന്യ;
		type = (allow_ofs_delta && DELTA(entry)->idx.offset) ?
			OBJ_OFS_DELTA : OBJ_REF_DELTA;
	} അല്ലെങ്കിൽ {
		buf = get_delta(entry);
		size = DELTA_SIZE(entry);
		type = (allow_ofs_delta && DELTA(entry)->idx.offset) ?
			OBJ_OFS_DELTA : OBJ_REF_DELTA;
	}

	എങ്കിൽ (st)	/* large blob case, just assume we don't compress well */
		datalen = size;
	അല്ലെങ്കിൽ എങ്കിൽ (entry->z_delta_size)
		datalen = entry->z_delta_size;
	അല്ലെങ്കിൽ
		datalen = do_compress(&buf, size);

	/*
	 * The object header is a byte of 'type' followed by zero or
	 * more bytes of length.
	 */
	hdrlen = encode_in_pack_object_header(header, വലുപ്പം(header),
					      type, size);

	എങ്കിൽ (type == OBJ_OFS_DELTA) {
		/*
		 * Deltas with relative base contain an additional
		 * encoding of the relative offset for the delta
		 * base from this object's position in the pack.
		 */
		off_t ofs = entry->idx.offset - DELTA(entry)->idx.offset;
		unsigned pos = വലുപ്പം(dheader) - 1;
		dheader[pos] = ofs & 127;
		ആകുമ്പോൾ (ofs >>= 7)
			dheader[--pos] = 128 | (--ofs & 127);
		എങ്കിൽ (limit && hdrlen + വലുപ്പം(dheader) - pos + datalen + hashsz >= limit) {
			എങ്കിൽ (st)
				odb_stream_close(st);
			സ്വതന്ത്രം(buf);
			തിരികെ 0;
		}
		hashwrite(f, header, hdrlen);
		hashwrite(f, dheader + pos, വലുപ്പം(dheader) - pos);
		hdrlen += വലുപ്പം(dheader) - pos;
	} അല്ലെങ്കിൽ എങ്കിൽ (type == OBJ_REF_DELTA) {
		/*
		 * Deltas with a base reference contain
		 * additional bytes for the base object ID.
		 */
		എങ്കിൽ (limit && hdrlen + hashsz + datalen + hashsz >= limit) {
			എങ്കിൽ (st)
				odb_stream_close(st);
			സ്വതന്ത്രം(buf);
			തിരികെ 0;
		}
		hashwrite(f, header, hdrlen);
		hashwrite(f, DELTA(entry)->idx.oid.hash, hashsz);
		hdrlen += hashsz;
	} അല്ലെങ്കിൽ {
		എങ്കിൽ (limit && hdrlen + datalen + hashsz >= limit) {
			എങ്കിൽ (st)
				odb_stream_close(st);
			സ്വതന്ത്രം(buf);
			തിരികെ 0;
		}
		hashwrite(f, header, hdrlen);
	}
	എങ്കിൽ (st) {
		datalen = write_large_blob_data(st, f, &entry->idx.oid);
		odb_stream_close(st);
	} അല്ലെങ്കിൽ {
		hashwrite(f, buf, datalen);
		സ്വതന്ത്രം(buf);
	}

	തിരികെ hdrlen + datalen;
}

/* Return 0 if we will bust the pack-size limit */
സ്ഥിതം off_t write_reuse_object(ഘടന hashfile *f, ഘടന object_entry *entry,
				unsigned long limit, പൂർണ്ണസംഖ്യ usable_delta)
{
	ഘടന packed_git *p = IN_PACK(entry);
	ഘടന pack_window *w_curs = ശൂന്യ;
	uint32_t pos;
	off_t offset, cur;
	എണ്ണൽ object_type type = oe_type(entry);
	എണ്ണൽ object_type in_pack_type;
	off_t datalen;
	unsigned അക്ഷരം header[MAX_PACK_OBJECT_HEADER],
		      dheader[MAX_PACK_OBJECT_HEADER];
	unsigned hdrlen;
	സ്ഥിരം unsigned hashsz = the_hash_algo->rawsz;
	size_t entry_size;

	cur = entry->in_pack_offset;
	in_pack_type = unpack_object_header(p, &w_curs, &cur, &entry_size);
	എങ്കിൽ (in_pack_type < 0)
		die(_("write_reuse_object: unable to parse object header of %s"),
		    oid_to_hex(&entry->idx.oid));

	എങ്കിൽ (DELTA(entry))
		type = (allow_ofs_delta && DELTA(entry)->idx.offset) ?
			OBJ_OFS_DELTA : OBJ_REF_DELTA;
	hdrlen = encode_in_pack_object_header(header, വലുപ്പം(header),
					      type, entry_size);

	offset = entry->in_pack_offset;
	എങ്കിൽ (offset_to_pack_pos(p, offset, &pos) < 0)
		die(_("write_reuse_object: could not locate %s, expected at "
		      "offset %"PRIuMAX" in pack %s"),
		    oid_to_hex(&entry->idx.oid), (uintmax_t)offset,
		    p->pack_name);
	datalen = pack_pos_to_offset(p, pos + 1) - offset;
	എങ്കിൽ (!pack_to_stdout && p->index_version > 1 &&
	    check_pack_crc(p, &w_curs, offset, datalen,
			   pack_pos_to_index(p, pos))) {
		error(_("bad packed object CRC for %s"),
		      oid_to_hex(&entry->idx.oid));
		unuse_pack(&w_curs);
		തിരികെ write_no_reuse_object(f, entry, limit, usable_delta);
	}

	offset += entry->in_pack_header_size;
	datalen -= entry->in_pack_header_size;

	എങ്കിൽ (!pack_to_stdout && p->index_version == 1 &&
	    check_pack_inflate(p, &w_curs, offset, datalen, entry_size)) {
		error(_("corrupt packed object for %s"),
		      oid_to_hex(&entry->idx.oid));
		unuse_pack(&w_curs);
		തിരികെ write_no_reuse_object(f, entry, limit, usable_delta);
	}

	എങ്കിൽ (type == OBJ_OFS_DELTA) {
		off_t ofs = entry->idx.offset - DELTA(entry)->idx.offset;
		unsigned pos = വലുപ്പം(dheader) - 1;
		dheader[pos] = ofs & 127;
		ആകുമ്പോൾ (ofs >>= 7)
			dheader[--pos] = 128 | (--ofs & 127);
		എങ്കിൽ (limit && hdrlen + വലുപ്പം(dheader) - pos + datalen + hashsz >= limit) {
			unuse_pack(&w_curs);
			തിരികെ 0;
		}
		hashwrite(f, header, hdrlen);
		hashwrite(f, dheader + pos, വലുപ്പം(dheader) - pos);
		hdrlen += വലുപ്പം(dheader) - pos;
		reused_delta++;
	} അല്ലെങ്കിൽ എങ്കിൽ (type == OBJ_REF_DELTA) {
		എങ്കിൽ (limit && hdrlen + hashsz + datalen + hashsz >= limit) {
			unuse_pack(&w_curs);
			തിരികെ 0;
		}
		hashwrite(f, header, hdrlen);
		hashwrite(f, DELTA(entry)->idx.oid.hash, hashsz);
		hdrlen += hashsz;
		reused_delta++;
	} അല്ലെങ്കിൽ {
		എങ്കിൽ (limit && hdrlen + datalen + hashsz >= limit) {
			unuse_pack(&w_curs);
			തിരികെ 0;
		}
		hashwrite(f, header, hdrlen);
	}
	copy_pack_data(f, p, &w_curs, offset, datalen);
	unuse_pack(&w_curs);
	reused++;
	തിരികെ hdrlen + datalen;
}

/* Return 0 if we will bust the pack-size limit */
സ്ഥിതം off_t write_object(ഘടന hashfile *f,
			  ഘടന object_entry *entry,
			  off_t write_offset)
{
	unsigned long limit;
	off_t len;
	പൂർണ്ണസംഖ്യ usable_delta, to_reuse;

	എങ്കിൽ (!pack_to_stdout)
		crc32_begin(f);

	/* apply size limit if limited packsize and not first object */
	എങ്കിൽ (!pack_size_limit || !nr_written)
		limit = 0;
	അല്ലെങ്കിൽ എങ്കിൽ (pack_size_limit <= write_offset)
		/*
		 * the earlier object did not fit the limit; avoid
		 * mistaking this with unlimited (i.e. limit = 0).
		 */
		limit = 1;
	അല്ലെങ്കിൽ
		limit = pack_size_limit - write_offset;

	എങ്കിൽ (!DELTA(entry))
		usable_delta = 0;	/* no delta */
	അല്ലെങ്കിൽ എങ്കിൽ (!pack_size_limit)
	       usable_delta = 1;	/* unlimited packfile */
	അല്ലെങ്കിൽ എങ്കിൽ (DELTA(entry)->idx.offset == (off_t)-1)
		usable_delta = 0;	/* base was written to another pack */
	അല്ലെങ്കിൽ എങ്കിൽ (DELTA(entry)->idx.offset)
		usable_delta = 1;	/* base already exists in this pack */
	അല്ലെങ്കിൽ
		usable_delta = 0;	/* base could end up in another pack */

	എങ്കിൽ (!reuse_object)
		to_reuse = 0;	/* explicit */
	അല്ലെങ്കിൽ എങ്കിൽ (!IN_PACK(entry))
		to_reuse = 0;	/* can't reuse what we don't have */
	അല്ലെങ്കിൽ എങ്കിൽ (oe_type(entry) == OBJ_REF_DELTA ||
		 oe_type(entry) == OBJ_OFS_DELTA)
				/* check_object() decided it for us ... */
		to_reuse = usable_delta;
				/* ... but pack split may override that */
	അല്ലെങ്കിൽ എങ്കിൽ (oe_type(entry) != entry->in_pack_type)
		to_reuse = 0;	/* pack has delta which is unusable */
	അല്ലെങ്കിൽ എങ്കിൽ (DELTA(entry))
		to_reuse = 0;	/* we want to pack afresh */
	അല്ലെങ്കിൽ
		to_reuse = 1;	/* we have it in-pack undeltified,
				 * and we do not need to deltify it.
				 */

	എങ്കിൽ (!to_reuse)
		len = write_no_reuse_object(f, entry, limit, usable_delta);
	അല്ലെങ്കിൽ
		len = write_reuse_object(f, entry, limit, usable_delta);
	എങ്കിൽ (!len)
		തിരികെ 0;

	എങ്കിൽ (usable_delta)
		written_delta++;
	written++;
	എങ്കിൽ (!pack_to_stdout)
		entry->idx.crc32 = crc32_end(f);
	തിരികെ len;
}

എണ്ണൽ write_one_status {
	WRITE_ONE_SKIP = -1, /* already written */
	WRITE_ONE_BREAK = 0, /* writing this will bust the limit; not written */
	WRITE_ONE_WRITTEN = 1, /* normal */
	WRITE_ONE_RECURSIVE = 2 /* already scheduled to be written */
};

സ്ഥിതം എണ്ണൽ write_one_status write_one(ഘടന hashfile *f,
				       ഘടന object_entry *e,
				       off_t *offset)
{
	off_t size;
	പൂർണ്ണസംഖ്യ recursing;

	/*
	 * we set offset to 1 (which is an impossible value) to mark
	 * the fact that this object is involved in "write its base
	 * first before writing a deltified object" recursion.
	 */
	recursing = (e->idx.offset == 1);
	എങ്കിൽ (recursing) {
		warning(_("recursive delta detected for object %s"),
			oid_to_hex(&e->idx.oid));
		തിരികെ WRITE_ONE_RECURSIVE;
	} അല്ലെങ്കിൽ എങ്കിൽ (e->idx.offset || e->preferred_base) {
		/* offset is non zero if object is written already. */
		തിരികെ WRITE_ONE_SKIP;
	}

	/* if we are deltified, write out base object first. */
	എങ്കിൽ (DELTA(e)) {
		e->idx.offset = 1; /* now recurse */
		തിരഞ്ഞെടുപ്പ് (write_one(f, DELTA(e), offset)) {
		അവസ്ഥ WRITE_ONE_RECURSIVE:
			/* we cannot depend on this one */
			SET_DELTA(e, ശൂന്യ);
			നിർത്തുക;
		സ്ഥിരസ്ഥിതി:
			നിർത്തുക;
		അവസ്ഥ WRITE_ONE_BREAK:
			e->idx.offset = recursing;
			തിരികെ WRITE_ONE_BREAK;
		}
	}

	e->idx.offset = *offset;
	size = write_object(f, e, *offset);
	എങ്കിൽ (!size) {
		e->idx.offset = recursing;
		തിരികെ WRITE_ONE_BREAK;
	}
	written_list[nr_written++] = &e->idx;

	/* make sure off_t is sufficiently large not to wrap */
	എങ്കിൽ (signed_add_overflows(*offset, size))
		die(_("pack too large for current definition of off_t"));
	*offset += size;
	തിരികെ WRITE_ONE_WRITTEN;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ mark_tagged(സ്ഥിരം ഘടന reference *ref, ശൂന്യം *cb_data UNUSED)
{
	ഘടന object_id peeled;
	ഘടന object_entry *entry = packlist_find(&to_pack, ref->oid);

	എങ്കിൽ (entry)
		entry->tagged = 1;
	എങ്കിൽ (!reference_get_peeled_oid(the_repository, ref, &peeled)) {
		entry = packlist_find(&to_pack, &peeled);
		എങ്കിൽ (entry)
			entry->tagged = 1;
	}
	തിരികെ 0;
}

സ്ഥിതം വരിയിൽ unsigned അക്ഷരം oe_layer(ഘടന packing_data *pack,
				     ഘടന object_entry *e)
{
	എങ്കിൽ (!pack->layer)
		തിരികെ 0;
	തിരികെ pack->layer[e - pack->objects];
}

സ്ഥിതം വരിയിൽ ശൂന്യം add_to_write_order(ഘടന object_entry **wo,
			       unsigned പൂർണ്ണസംഖ്യ *endp,
			       ഘടന object_entry *e)
{
	എങ്കിൽ (e->filled || oe_layer(&to_pack, e) != write_layer)
		തിരികെ;
	wo[(*endp)++] = e;
	e->filled = 1;
}

സ്ഥിതം ശൂന്യം add_descendants_to_write_order(ഘടന object_entry **wo,
					   unsigned പൂർണ്ണസംഖ്യ *endp,
					   ഘടന object_entry *e)
{
	പൂർണ്ണസംഖ്യ add_to_order = 1;
	ആകുമ്പോൾ (e) {
		എങ്കിൽ (add_to_order) {
			ഘടന object_entry *s;
			/* add this node... */
			add_to_write_order(wo, endp, e);
			/* all its siblings... */
			വേണ്ടി (s = DELTA_SIBLING(e); s; s = DELTA_SIBLING(s)) {
				add_to_write_order(wo, endp, s);
			}
		}
		/* drop down a level to add left subtree nodes if possible */
		എങ്കിൽ (DELTA_CHILD(e)) {
			add_to_order = 1;
			e = DELTA_CHILD(e);
		} അല്ലെങ്കിൽ {
			add_to_order = 0;
			/* our sibling might have some children, it is next */
			എങ്കിൽ (DELTA_SIBLING(e)) {
				e = DELTA_SIBLING(e);
				തുടരുക;
			}
			/* go back to our parent node */
			e = DELTA(e);
			ആകുമ്പോൾ (e && !DELTA_SIBLING(e)) {
				/* we're on the right side of a subtree, keep
				 * going up until we can go right again */
				e = DELTA(e);
			}
			എങ്കിൽ (!e) {
				/* done- we hit our original root node */
				തിരികെ;
			}
			/* pass it off to sibling at this level */
			e = DELTA_SIBLING(e);
		}
	};
}

സ്ഥിതം ശൂന്യം add_family_to_write_order(ഘടന object_entry **wo,
				      unsigned പൂർണ്ണസംഖ്യ *endp,
				      ഘടന object_entry *e)
{
	ഘടന object_entry *വേര്;

	വേണ്ടി (വേര് = e; DELTA(വേര്); വേര് = DELTA(വേര്))
		; /* nothing */
	add_descendants_to_write_order(wo, endp, വേര്);
}

സ്ഥിതം ശൂന്യം compute_layer_order(ഘടന object_entry **wo, unsigned പൂർണ്ണസംഖ്യ *wo_end)
{
	unsigned പൂർണ്ണസംഖ്യ i, last_untagged;
	ഘടന object_entry *objects = to_pack.objects;

	വേണ്ടി (i = 0; i < to_pack.nr_objects; i++) {
		എങ്കിൽ (objects[i].tagged)
			നിർത്തുക;
		add_to_write_order(wo, wo_end, &objects[i]);
	}
	last_untagged = i;

	/*
	 * Then fill all the tagged tips.
	 */
	വേണ്ടി (; i < to_pack.nr_objects; i++) {
		എങ്കിൽ (objects[i].tagged)
			add_to_write_order(wo, wo_end, &objects[i]);
	}

	/*
	 * And then all remaining commits and tags.
	 */
	വേണ്ടി (i = last_untagged; i < to_pack.nr_objects; i++) {
		എങ്കിൽ (oe_type(&objects[i]) != OBJ_COMMIT &&
		    oe_type(&objects[i]) != OBJ_TAG)
			തുടരുക;
		add_to_write_order(wo, wo_end, &objects[i]);
	}

	/*
	 * And then all the trees.
	 */
	വേണ്ടി (i = last_untagged; i < to_pack.nr_objects; i++) {
		എങ്കിൽ (oe_type(&objects[i]) != OBJ_വൃക്ഷം)
			തുടരുക;
		add_to_write_order(wo, wo_end, &objects[i]);
	}

	/*
	 * Finally all the rest in really tight order
	 */
	വേണ്ടി (i = last_untagged; i < to_pack.nr_objects; i++) {
		എങ്കിൽ (!objects[i].filled && oe_layer(&to_pack, &objects[i]) == write_layer)
			add_family_to_write_order(wo, wo_end, &objects[i]);
	}
}

സ്ഥിതം ഘടന object_entry **compute_write_order(ശൂന്യം)
{
	uint32_t max_layers = 1;
	unsigned പൂർണ്ണസംഖ്യ i, wo_end;

	ഘടന object_entry **wo;
	ഘടന object_entry *objects = to_pack.objects;

	വേണ്ടി (i = 0; i < to_pack.nr_objects; i++) {
		objects[i].tagged = 0;
		objects[i].filled = 0;
		SET_DELTA_CHILD(&objects[i], ശൂന്യ);
		SET_DELTA_SIBLING(&objects[i], ശൂന്യ);
	}

	/*
	 * Fully connect delta_child/delta_sibling network.
	 * Make sure delta_sibling is sorted in the original
	 * recency order.
	 */
	വേണ്ടി (i = to_pack.nr_objects; i > 0;) {
		ഘടന object_entry *e = &objects[--i];
		എങ്കിൽ (!DELTA(e))
			തുടരുക;
		/* Mark me as the first child */
		e->delta_sibling_idx = DELTA(e)->delta_child_idx;
		SET_DELTA_CHILD(DELTA(e), e);
	}

	/*
	 * Mark objects that are at the tip of tags.
	 */
	refs_for_each_tag_ref(get_main_ref_store(the_repository), mark_tagged,
			      ശൂന്യ);

	എങ്കിൽ (use_delta_islands) {
		max_layers = compute_pack_layers(&to_pack);
		free_island_marks();
	}

	ALLOC_ARRAY(wo, to_pack.nr_objects);
	wo_end = 0;

	വേണ്ടി (; write_layer < max_layers; ++write_layer)
		compute_layer_order(wo, &wo_end);

	എങ്കിൽ (wo_end != to_pack.nr_objects)
		die(_("ordered %u objects, expected %"PRIu32),
		    wo_end, to_pack.nr_objects);

	തിരികെ wo;
}


/*
 * A reused set of objects. All objects in a chunk have the same
 * relative position in the original packfile and the generated
 * packfile.
 */

സ്ഥിതം ഘടന reused_chunk {
	/* The offset of the first object of this chunk in the original
	 * packfile. */
	off_t original;
	/* The difference for "original" minus the offset of the first object of
	 * this chunk in the generated packfile. */
	off_t difference;
} *reused_chunks;
സ്ഥിതം പൂർണ്ണസംഖ്യ reused_chunks_nr;
സ്ഥിതം പൂർണ്ണസംഖ്യ reused_chunks_alloc;

സ്ഥിതം ശൂന്യം record_reused_object(off_t where, off_t offset)
{
	എങ്കിൽ (reused_chunks_nr && reused_chunks[reused_chunks_nr-1].difference == offset)
		തിരികെ;

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
സ്ഥിതം off_t find_reused_offset(off_t where)
{
	പൂർണ്ണസംഖ്യ lo = 0, hi = reused_chunks_nr;
	ആകുമ്പോൾ (lo < hi) {
		പൂർണ്ണസംഖ്യ mi = lo + ((hi - lo) / 2);
		എങ്കിൽ (where == reused_chunks[mi].original)
			തിരികെ reused_chunks[mi].difference;
		എങ്കിൽ (where < reused_chunks[mi].original)
			hi = mi;
		അല്ലെങ്കിൽ
			lo = mi + 1;
	}

	/*
	 * The first chunk starts at zero, so we can't have gone below
	 * there.
	 */
	പ്രതിജ്ഞാ(lo);
	തിരികെ reused_chunks[lo-1].difference;
}

സ്ഥിതം ശൂന്യം write_reused_pack_one(ഘടന packed_git *reuse_packfile,
				  size_t pos, ഘടന hashfile *out,
				  off_t pack_start,
				  ഘടന pack_window **w_curs)
{
	off_t offset, next, cur;
	എണ്ണൽ object_type type;
	size_t size;

	offset = pack_pos_to_offset(reuse_packfile, pos);
	next = pack_pos_to_offset(reuse_packfile, pos + 1);

	record_reused_object(offset,
			     offset - (hashfile_total(out) - pack_start));

	cur = offset;
	type = unpack_object_header(reuse_packfile, w_curs, &cur, &size);
	പ്രതിജ്ഞാ(type >= 0);

	എങ്കിൽ (type == OBJ_OFS_DELTA) {
		off_t base_offset;
		off_t fixup;

		unsigned അക്ഷരം header[MAX_PACK_OBJECT_HEADER];
		unsigned len;

		base_offset = get_delta_base(reuse_packfile, w_curs, &cur, type, offset);
		പ്രതിജ്ഞാ(base_offset != 0);

		/* Convert to REF_DELTA if we must... */
		എങ്കിൽ (!allow_ofs_delta) {
			uint32_t base_pos;
			ഘടന object_id base_oid;

			എങ്കിൽ (offset_to_pack_pos(reuse_packfile, base_offset, &base_pos) < 0)
				die(_("expected object at offset %"PRIuMAX" "
				      "in pack %s"),
				    (uintmax_t)base_offset,
				    reuse_packfile->pack_name);

			nth_packed_object_id(&base_oid, reuse_packfile,
					     pack_pos_to_index(reuse_packfile, base_pos));

			len = encode_in_pack_object_header(header, വലുപ്പം(header),
							   OBJ_REF_DELTA, size);
			hashwrite(out, header, len);
			hashwrite(out, base_oid.hash, the_hash_algo->rawsz);
			copy_pack_data(out, reuse_packfile, w_curs, cur, next - cur);
			തിരികെ;
		}

		/* Otherwise see if we need to rewrite the offset... */
		fixup = find_reused_offset(offset) -
			find_reused_offset(base_offset);
		എങ്കിൽ (fixup) {
			unsigned അക്ഷരം ofs_header[MAX_PACK_OBJECT_HEADER];
			unsigned i, ofs_len;
			off_t ofs = offset - base_offset - fixup;

			len = encode_in_pack_object_header(header, വലുപ്പം(header),
							   OBJ_OFS_DELTA, size);

			i = വലുപ്പം(ofs_header) - 1;
			ofs_header[i] = ofs & 127;
			ആകുമ്പോൾ (ofs >>= 7)
				ofs_header[--i] = 128 | (--ofs & 127);

			ofs_len = വലുപ്പം(ofs_header) - i;

			hashwrite(out, header, len);
			hashwrite(out, ofs_header + വലുപ്പം(ofs_header) - ofs_len, ofs_len);
			copy_pack_data(out, reuse_packfile, w_curs, cur, next - cur);
			തിരികെ;
		}

		/* ...otherwise we have no fixup, and can write it verbatim */
	}

	copy_pack_data(out, reuse_packfile, w_curs, offset, next - offset);
}

സ്ഥിതം size_t write_reused_pack_verbatim(ഘടന bitmapped_pack *reuse_packfile,
					 ഘടന hashfile *out,
					 ഘടന pack_window **w_curs)
{
	size_t pos = 0;
	size_t end;

	എങ്കിൽ (reuse_packfile->bitmap_pos) {
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
		തിരികെ reuse_packfile->bitmap_pos / BITS_IN_EWORD;
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
	എങ്കിൽ (reuse_packfile_bitmap->word_alloc < end)
		BUG("fewer words than expected in reuse_packfile_bitmap");

	ആകുമ്പോൾ (pos < end && reuse_packfile_bitmap->വാക്കുകൾ[pos] == (eword_t)~0)
		pos++;

	എങ്കിൽ (pos) {
		off_t to_write;

		written = (pos * BITS_IN_EWORD);
		to_write = pack_pos_to_offset(reuse_packfile->p, written)
			- വലുപ്പം(ഘടന pack_header);

		/* We're recording one chunk, not one object. */
		record_reused_object(വലുപ്പം(ഘടന pack_header), 0);
		hashflush(out);
		copy_pack_data(out, reuse_packfile->p, w_curs,
			വലുപ്പം(ഘടന pack_header), to_write);

		display_progress(progress_state, written);
	}
	തിരികെ pos;
}

സ്ഥിതം ശൂന്യം write_reused_pack(ഘടന bitmapped_pack *reuse_packfile,
			      ഘടന hashfile *f)
{
	size_t i = reuse_packfile->bitmap_pos / BITS_IN_EWORD;
	uint32_t offset;
	off_t pack_start = hashfile_total(f) - വലുപ്പം(ഘടന pack_header);
	ഘടന pack_window *w_curs = ശൂന്യ;

	എങ്കിൽ (allow_ofs_delta)
		i = write_reused_pack_verbatim(reuse_packfile, f, &w_curs);

	വേണ്ടി (; i < reuse_packfile_bitmap->word_alloc; ++i) {
		eword_t word = reuse_packfile_bitmap->വാക്കുകൾ[i];
		size_t pos = (i * BITS_IN_EWORD);

		വേണ്ടി (offset = 0; offset < BITS_IN_EWORD; ++offset) {
			uint32_t pack_pos;
			എങ്കിൽ ((word >> offset) == 0)
				നിർത്തുക;

			offset += ewah_bit_ctz64(word >> offset);
			എങ്കിൽ (pos + offset < reuse_packfile->bitmap_pos)
				തുടരുക;
			എങ്കിൽ (pos + offset >= reuse_packfile->bitmap_pos + reuse_packfile->bitmap_nr)
				പോകുക done;

			എങ്കിൽ (reuse_packfile->bitmap_pos) {
				/*
				 * When doing multi-pack reuse on a
				 * non-preferred pack, translate bit positions
				 * from the MIDX pseudo-pack order back to their
				 * pack-relative positions before attempting
				 * reuse.
				 */
				ഘടന multi_pack_index *m = reuse_packfile->from_midx;
				uint32_t midx_pos;
				off_t pack_ofs;

				എങ്കിൽ (!m)
					BUG("non-zero bitmap position without MIDX");

				midx_pos = pack_pos_to_midx(m, pos + offset);
				pack_ofs = nth_midxed_offset(m, midx_pos);

				എങ്കിൽ (offset_to_pack_pos(reuse_packfile->p,
						       pack_ofs, &pack_pos) < 0)
					BUG("could not find expected object at offset %"PRIuMAX" in pack %s",
					    (uintmax_t)pack_ofs,
					    pack_basename(reuse_packfile->p));
			} അല്ലെങ്കിൽ {
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

സ്ഥിതം ശൂന്യം write_excluded_by_configs(ശൂന്യം)
{
	ഘടന oidset_iter iter;
	സ്ഥിരം ഘടന object_id *oid;

	oidset_iter_init(&excluded_by_config, &iter);
	ആകുമ്പോൾ ((oid = oidset_iter_next(&iter))) {
		ഘടന configured_exclusion *ex =
			oidmap_get(&configured_exclusions, oid);

		എങ്കിൽ (!ex)
			BUG("configured exclusion wasn't configured");
		write_in_full(1, ex->pack_hash_hex, strlen(ex->pack_hash_hex));
		write_in_full(1, " ", 1);
		write_in_full(1, ex->uri, strlen(ex->uri));
		write_in_full(1, "\n", 1);
	}
}

സ്ഥിതം സ്ഥിരം അക്ഷരം no_split_warning[] = N_(
"disabling bitmap writing, packs are split due to pack.packSizeLimit"
);

സ്ഥിതം ശൂന്യം write_pack_file(ശൂന്യം)
{
	uint32_t i = 0, j;
	ഘടന hashfile *f;
	off_t offset;
	uint32_t nr_remaining = nr_result;
	time_t last_mtime = 0;
	ഘടന object_entry **write_order;
	off_t bytes_written = 0;

	എങ്കിൽ (progress > pack_to_stdout)
		progress_state = start_progress(the_repository,
						_("Writing objects"), nr_result);
	ALLOC_ARRAY(written_list, to_pack.nr_objects);
	write_order = compute_write_order();

	ചെയ്യുക {
		unsigned അക്ഷരം hash[GIT_MAX_RAWSZ];
		അക്ഷരം *pack_tmp_name = ശൂന്യ;

		എങ്കിൽ (pack_to_stdout) {
			/*
			 * This command is most often invoked via
			 * git-upload-pack(1), which will typically chunk data
			 * into pktlines. As such, we use the maximum data
			 * length of them as buffer length.
			 *
			 * Note that we need to subtract one though to
			 * accommodate for the sideband byte.
			 */
			ഘടന hashfd_options opts = {
				.progress = progress_state,
				.buffer_len = LARGE_PACKET_DATA_MAX - 1,
			};
			f = hashfd_ext(the_repository->hash_algo, 1,
				       "<stdout>", &opts);
		} അല്ലെങ്കിൽ {
			f = create_tmp_packfile(the_repository, &pack_tmp_name);
		}

		offset = write_pack_header(f, nr_remaining);

		എങ്കിൽ (reuse_packfiles_nr) {
			പ്രതിജ്ഞാ(pack_to_stdout);
			വേണ്ടി (j = 0; j < reuse_packfiles_nr; j++) {
				reused_chunks_nr = 0;
				write_reused_pack(&reuse_packfiles[j], f);
				എങ്കിൽ (reused_chunks_nr)
					reuse_packfiles_used_nr++;
			}
			offset = hashfile_total(f);
		}

		nr_written = 0;
		വേണ്ടി (; i < to_pack.nr_objects; i++) {
			ഘടന object_entry *e = write_order[i];
			എങ്കിൽ (write_one(f, e, &offset) == WRITE_ONE_BREAK)
				നിർത്തുക;
			display_progress(progress_state, written);
		}

		bytes_written += hashfile_total(f) +
			the_repository->hash_algo->rawsz;
		എങ്കിൽ (pack_to_stdout) {
			/*
			 * We never fsync when writing to stdout since we may
			 * not be writing to an actual pack file. For instance,
			 * the upload-pack code passes a pipe here. Calling
			 * fsync on a pipe results in unnecessary
			 * synchronization with the reader on some platforms.
			 */
			finalize_hashfile(f, hash, FSYNC_COMPONENT_NONE,
					  CSUM_HASH_IN_STREAM | CSUM_CLOSE);
		} അല്ലെങ്കിൽ എങ്കിൽ (nr_written == nr_remaining) {
			finalize_hashfile(f, hash, FSYNC_COMPONENT_PACK,
					  CSUM_HASH_IN_STREAM | CSUM_FSYNC | CSUM_CLOSE);
		} അല്ലെങ്കിൽ {
			/*
			 * If we wrote the wrong number of entries in the
			 * header, rewrite it like in fast-import.
			 */

			പൂർണ്ണസംഖ്യ fd = finalize_hashfile(f, hash, FSYNC_COMPONENT_PACK, 0);
			fixup_pack_header_footer(the_hash_algo, fd, hash,
						 pack_tmp_name, nr_written,
						 hash, offset);
			close(fd);
			എങ്കിൽ (write_bitmap_index) {
				എങ്കിൽ (write_bitmap_index != WRITE_BITMAP_QUIET)
					warning(_(no_split_warning));
				write_bitmap_index = 0;
			}
		}

		എങ്കിൽ (!pack_to_stdout) {
			ഘടന stat st;
			ഘടന strbuf tmpname = STRBUF_INIT;
			ഘടന bitmap_writer bitmap_writer;
			അക്ഷരം *idx_tmp_name = ശൂന്യ;

			/*
			 * Packs are runtime accessed in their mtime
			 * order since newer packs are more likely to contain
			 * younger objects.  So if we are creating multiple
			 * packs then we should modify the mtime of later ones
			 * to preserve this property.
			 */
			എങ്കിൽ (stat(pack_tmp_name, &st) < 0) {
				warning_errno(_("failed to stat %s"), pack_tmp_name);
			} അല്ലെങ്കിൽ എങ്കിൽ (!last_mtime) {
				last_mtime = st.st_mtime;
			} അല്ലെങ്കിൽ {
				ഘടന utimbuf utb;
				utb.actime = st.st_atime;
				utb.modtime = --last_mtime;
				എങ്കിൽ (utime(pack_tmp_name, &utb) < 0)
					warning_errno(_("failed utime() on %s"), pack_tmp_name);
			}

			strbuf_addf(&tmpname, "%s-%s.", base_name,
				    hash_to_hex(hash));

			എങ്കിൽ (write_bitmap_index) {
				bitmap_writer_init(&bitmap_writer,
						   the_repository, &to_pack,
						   ശൂന്യ);
				bitmap_writer_set_checksum(&bitmap_writer, hash);
				bitmap_writer_build_type_index(&bitmap_writer,
							       written_list);
			}

			എങ്കിൽ (cruft)
				pack_idx_opts.flags |= WRITE_MTIMES;

			stage_tmp_packfiles(the_repository, &tmpname,
					    pack_tmp_name, written_list,
					    nr_written, &to_pack,
					    &pack_idx_opts, hash,
					    &idx_tmp_name);

			എങ്കിൽ (write_bitmap_index) {
				size_t tmpname_len = tmpname.len;

				strbuf_addstr(&tmpname, "bitmap");
				stop_progress(&progress_state);

				bitmap_writer_show_progress(&bitmap_writer,
							    progress);
				bitmap_writer_select_commits(&bitmap_writer,
							     indexed_commits,
							     indexed_commits_nr);
				എങ്കിൽ (bitmap_writer_build(&bitmap_writer) < 0)
					die(_("failed to write bitmap index"));
				bitmap_writer_finish(&bitmap_writer,
						     written_list,
						     tmpname.buf, write_bitmap_options);
				bitmap_writer_free(&bitmap_writer);
				write_bitmap_index = 0;
				strbuf_setlen(&tmpname, tmpname_len);
			}

			rename_tmp_packfile_idx(the_repository, &tmpname, &idx_tmp_name);

			സ്വതന്ത്രം(idx_tmp_name);
			strbuf_release(&tmpname);
			സ്വതന്ത്രം(pack_tmp_name);
			puts(hash_to_hex(hash));
		}

		/* mark written objects as written to previous pack */
		വേണ്ടി (j = 0; j < nr_written; j++) {
			written_list[j]->offset = (off_t)-1;
		}
		nr_remaining -= nr_written;
	} ആകുമ്പോൾ (nr_remaining && i < to_pack.nr_objects);

	സ്വതന്ത്രം(written_list);
	സ്വതന്ത്രം(write_order);
	stop_progress(&progress_state);
	എങ്കിൽ (written != nr_result)
		die(_("wrote %"PRIu32" objects while expecting %"PRIu32),
		    written, nr_result);
	trace2_data_intmax("pack-objects", the_repository,
			   "write_pack_file/wrote", nr_result);
	trace2_data_intmax("pack-objects", the_repository,
			   "write_pack_file/wrote_bytes", bytes_written);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ no_try_delta(സ്ഥിരം അക്ഷരം *path)
{
	സ്ഥിതം ഘടന attr_check *check;

	എങ്കിൽ (!check)
		check = attr_check_initl("delta", ശൂന്യ);
	git_check_attr(the_repository->index, path, check);
	എങ്കിൽ (ATTR_FALSE(check->ഇനങ്ങൾ[0].മൂല്യം))
		തിരികെ 1;
	തിരികെ 0;
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
സ്ഥിതം പൂർണ്ണസംഖ്യ have_duplicate_entry(സ്ഥിരം ഘടന object_id *oid,
				പൂർണ്ണസംഖ്യ exclude)
{
	ഘടന object_entry *entry;

	എങ്കിൽ (reuse_packfile_bitmap &&
	    bitmap_walk_contains(bitmap_git, reuse_packfile_bitmap, oid))
		തിരികെ 1;

	entry = packlist_find(&to_pack, oid);
	എങ്കിൽ (!entry)
		തിരികെ 0;

	എങ്കിൽ (exclude) {
		എങ്കിൽ (!entry->preferred_base)
			nr_result--;
		entry->preferred_base = 1;
	}

	തിരികെ 1;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ want_cruft_object_mtime(ഘടന repository *r,
				   സ്ഥിരം ഘടന object_id *oid,
				   unsigned flags, uint32_t mtime)
{
	ഘടന odb_source_files *files = odb_source_files_downcast(r->objects->source);

	വേണ്ടി (ഘടന odb_files_dir *dir = files->dirs; dir; dir = dir->next) {
		ഘടന packed_git **cache = packfile_store_get_kept_pack_cache(dir->packed, flags);

		വേണ്ടി (; *cache; cache++) {
			ഘടന packed_git *p = *cache;
			off_t ofs;
			uint32_t candidate_mtime;

			ofs = find_pack_entry_one(oid, p);
			എങ്കിൽ (!ofs)
				തുടരുക;

			/*
			 * We have a copy of the object 'oid' in a non-cruft
			 * pack. We can avoid packing an additional copy
			 * regardless of what the existing copy's mtime is since
			 * it is outside of a cruft pack.
			 */
			എങ്കിൽ (!p->is_cruft)
				തിരികെ 0;

			/*
			 * If we have a copy of the object 'oid' in a cruft
			 * pack, then either read the cruft pack's mtime for
			 * that object, or, if that can't be loaded, assume the
			 * pack's mtime itself.
			 */
			എങ്കിൽ (!load_pack_mtimes(p)) {
				uint32_t pos;
				എങ്കിൽ (offset_to_pack_pos(p, ofs, &pos) < 0)
					തുടരുക;
				candidate_mtime = nth_packed_mtime(p, pos);
			} അല്ലെങ്കിൽ {
				candidate_mtime = p->mtime;
			}

			/*
			 * We have a surviving copy of the object in a cruft
			 * pack whose mtime is greater than or equal to the one
			 * we are considering. We can thus avoid packing an
			 * additional copy of that object.
			 */
			എങ്കിൽ (mtime <= candidate_mtime)
				തിരികെ 0;
		}
	}

	തിരികെ -1;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ want_found_object(സ്ഥിരം ഘടന object_id *oid, പൂർണ്ണസംഖ്യ exclude,
			     ഘടന packed_git *p, uint32_t mtime)
{
	എങ്കിൽ (exclude)
		തിരികെ 1;
	എങ്കിൽ (incremental)
		തിരികെ 0;

	എങ്കിൽ (!is_pack_valid(p))
		തിരികെ -1;

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
	എങ്കിൽ (local && !p->pack_local)
		തിരികെ 0;

	/*
	 * Then handle .keep first, as we have a fast(er) path there.
	 */
	എങ്കിൽ (ignore_packed_keep_on_disk || ignore_packed_keep_in_core ||
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
		എങ്കിൽ (ignore_packed_keep_on_disk)
			flags |= KEPT_PACK_ON_DISK;
		എങ്കിൽ (ignore_packed_keep_in_core)
			flags |= KEPT_PACK_IN_CORE;
		എങ്കിൽ (ignore_packed_keep_in_core_open)
			flags |= KEPT_PACK_IN_CORE_OPEN;

		/*
		 * If the object is in a pack that we want to ignore, *and* we
		 * don't have any cruft packs that are being retained, we can
		 * abort quickly.
		 */
		എങ്കിൽ (!ignore_packed_keep_in_core_has_cruft) {
			എങ്കിൽ (ignore_packed_keep_on_disk && p->pack_keep)
				തിരികെ 0;
			എങ്കിൽ (ignore_packed_keep_in_core && p->pack_keep_in_core)
				തിരികെ 0;
			എങ്കിൽ (ignore_packed_keep_in_core_open && p->pack_keep_in_core_open)
				തിരികെ 0;
			എങ്കിൽ (has_object_kept_pack(p->repo, oid, flags))
				തിരികെ 0;
		} അല്ലെങ്കിൽ {
			/*
			 * But if there is at least one cruft pack which
			 * is being kept, we only want to include the
			 * provided object if it has a strictly greater
			 * mtime than any existing cruft copy.
			 */
			എങ്കിൽ (!want_cruft_object_mtime(p->repo, oid, flags,
						     mtime))
				തിരികെ 0;
		}
	}

	/*
	 * At this point we know definitively that either we don't care about
	 * keep-packs, or the object is not in one. Keep checking other
	 * conditions...
	 */
	എങ്കിൽ (!local || !have_non_local_packs)
		തിരികെ 1;

	/* we don't know yet; keep looking for more packs */
	തിരികെ -1;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ want_object_in_pack_one(ഘടന packed_git *p,
				   സ്ഥിരം ഘടന object_id *oid,
				   പൂർണ്ണസംഖ്യ exclude,
				   ഘടന packed_git **found_pack,
				   off_t *found_offset,
				   uint32_t found_mtime)
{
	off_t offset;

	എങ്കിൽ (p == *found_pack)
		offset = *found_offset;
	അല്ലെങ്കിൽ
		offset = find_pack_entry_one(oid, p);

	എങ്കിൽ (offset) {
		എങ്കിൽ (!*found_pack) {
			എങ്കിൽ (!is_pack_valid(p))
				തിരികെ -1;
			*found_offset = offset;
			*found_pack = p;
		}
		തിരികെ want_found_object(oid, exclude, p, found_mtime);
	}
	തിരികെ -1;
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
സ്ഥിതം പൂർണ്ണസംഖ്യ want_object_in_pack_mtime(സ്ഥിരം ഘടന object_id *oid,
				     പൂർണ്ണസംഖ്യ exclude,
				     ഘടന packed_git **found_pack,
				     off_t *found_offset,
				     uint32_t found_mtime)
{
	ഘടന odb_source_files *files =
		odb_source_files_downcast(the_repository->objects->source);
	പൂർണ്ണസംഖ്യ want;
	ഘടന packfile_list_entry *e;

	എങ്കിൽ (!exclude && local) {
		/*
		 * Note that we start iterating at `dirs->next` so that we
		 * skip the local object directory.
		 */
		വേണ്ടി (ഘടന odb_files_dir *dir = files->dirs->next; dir; dir = dir->next)
			എങ്കിൽ (!odb_source_read_object_info(&dir->loose->base, oid, ശൂന്യ, 0, ശൂന്യ))
				തിരികെ 0;
	}

	/*
	 * If we already know the pack object lives in, start checks from that
	 * pack - in the usual case when neither --local was given nor .keep files
	 * are present we will determine the answer right now.
	 */
	എങ്കിൽ (*found_pack) {
		want = want_found_object(oid, exclude, *found_pack,
					 found_mtime);
		എങ്കിൽ (want != -1)
			തിരികെ want;

		*found_pack = ശൂന്യ;
		*found_offset = 0;
	}

	വേണ്ടി (ഘടന odb_files_dir *dir = files->dirs; dir; dir = dir->next) {
		ഘടന multi_pack_index *m = get_multi_pack_index(dir->packed);
		ഘടന pack_entry e;

		എങ്കിൽ (m && midx_fill_entry(m, oid, &e, ശൂന്യ) == MIDX_FILL_HIT) {
			want = want_object_in_pack_one(e.p, oid, exclude, found_pack, found_offset, found_mtime);
			എങ്കിൽ (want != -1)
				തിരികെ want;
		}
	}

	വേണ്ടി (ഘടന odb_files_dir *dir = files->dirs; dir; dir = dir->next) {
		വേണ്ടി (e = dir->packed->packs.head; e; e = e->next) {
			ഘടന packed_git *p = e->pack;
			want = want_object_in_pack_one(p, oid, exclude, found_pack, found_offset, found_mtime);
			എങ്കിൽ (!exclude && want > 0)
				packfile_list_prepend(&dir->packed->packs, p);
			എങ്കിൽ (want != -1)
				തിരികെ want;
		}
	}

	എങ്കിൽ (uri_protocols.nr) {
		ഘടന configured_exclusion *ex =
			oidmap_get(&configured_exclusions, oid);
		പൂർണ്ണസംഖ്യ i;
		സ്ഥിരം അക്ഷരം *p;

		എങ്കിൽ (ex) {
			വേണ്ടി (i = 0; i < uri_protocols.nr; i++) {
				എങ്കിൽ (skip_prefix(ex->uri,
						uri_protocols.ഇനങ്ങൾ[i].string,
						&p) &&
				    *p == ':') {
					oidset_തിരുകുക(&excluded_by_config, oid);
					തിരികെ 0;
				}
			}
		}
	}

	തിരികെ 1;
}

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ want_object_in_pack(സ്ഥിരം ഘടന object_id *oid,
				      പൂർണ്ണസംഖ്യ exclude,
				      ഘടന packed_git **found_pack,
				      off_t *found_offset)
{
	തിരികെ want_object_in_pack_mtime(oid, exclude, found_pack, found_offset,
					 0);
}

സ്ഥിതം ഘടന object_entry *create_object_entry(സ്ഥിരം ഘടന object_id *oid,
						എണ്ണൽ object_type type,
						uint32_t hash,
						പൂർണ്ണസംഖ്യ exclude,
						പൂർണ്ണസംഖ്യ no_try_delta,
						ഘടന packed_git *found_pack,
						off_t found_offset)
{
	ഘടന object_entry *entry;

	entry = packlist_alloc(&to_pack, oid);
	entry->hash = hash;
	oe_set_type(entry, type);
	എങ്കിൽ (exclude)
		entry->preferred_base = 1;
	അല്ലെങ്കിൽ
		nr_result++;
	എങ്കിൽ (found_pack) {
		oe_set_in_pack(&to_pack, entry, found_pack);
		entry->in_pack_offset = found_offset;
	}

	entry->no_try_delta = no_try_delta;

	തിരികെ entry;
}

സ്ഥിതം സ്ഥിരം അക്ഷരം no_closure_warning[] = N_(
"disabling bitmap writing, as some objects are not being packed"
);

സ്ഥിതം ശൂന്യം add_object_entry(സ്ഥിരം ഘടന object_id *oid, എണ്ണൽ object_type type,
			     സ്ഥിരം അക്ഷരം *name, പൂർണ്ണസംഖ്യ exclude)
{
	ഘടന packed_git *found_pack = ശൂന്യ;
	off_t found_offset = 0;

	display_progress(progress_state, ++nr_seen);

	എങ്കിൽ (have_duplicate_entry(oid, exclude))
		തിരികെ;

	എങ്കിൽ (!want_object_in_pack(oid, exclude, &found_pack, &found_offset)) {
		/* The pack is missing an object, so it will not have closure */
		എങ്കിൽ (write_bitmap_index) {
			എങ്കിൽ (write_bitmap_index != WRITE_BITMAP_QUIET)
				warning(_(no_closure_warning));
			write_bitmap_index = 0;
		}
		തിരികെ;
	}

	create_object_entry(oid, type, pack_name_hash_fn(name),
			    exclude, name && no_try_delta(name),
			    found_pack, found_offset);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ add_object_entry_from_bitmap(സ്ഥിരം ഘടന object_id *oid,
					എണ്ണൽ object_type type,
					പൂർണ്ണസംഖ്യ flags UNUSED, uint32_t name_hash,
					ഘടന packed_git *pack, off_t offset,
					ശൂന്യം *payload UNUSED)
{
	display_progress(progress_state, ++nr_seen);

	എങ്കിൽ (have_duplicate_entry(oid, 0))
		തിരികെ 0;

	എങ്കിൽ (!want_object_in_pack(oid, 0, &pack, &offset))
		തിരികെ 0;

	create_object_entry(oid, type, name_hash, 0, 0, pack, offset);
	തിരികെ 0;
}

ഘടന pbase_വൃക്ഷം_cache {
	ഘടന object_id oid;
	പൂർണ്ണസംഖ്യ ref;
	പൂർണ്ണസംഖ്യ temporary;
	ശൂന്യം *വൃക്ഷം_data;
	unsigned long വൃക്ഷം_size;
};

സ്ഥിതം ഘടന pbase_വൃക്ഷം_cache *(pbase_വൃക്ഷം_cache[256]);
സ്ഥിതം പൂർണ്ണസംഖ്യ pbase_വൃക്ഷം_cache_ix(സ്ഥിരം ഘടന object_id *oid)
{
	തിരികെ oid->hash[0] % ARRAY_SIZE(pbase_വൃക്ഷം_cache);
}
സ്ഥിതം പൂർണ്ണസംഖ്യ pbase_വൃക്ഷം_cache_ix_incr(പൂർണ്ണസംഖ്യ ix)
{
	തിരികെ (ix+1) % ARRAY_SIZE(pbase_വൃക്ഷം_cache);
}

സ്ഥിതം ഘടന pbase_വൃക്ഷം {
	ഘടന pbase_വൃക്ഷം *next;
	/* This is a phony "cache" entry; we are not
	 * going to evict it or find it through _get()
	 * mechanism -- this is for the toplevel node that
	 * would almost always change with any commit.
	 */
	ഘടന pbase_വൃക്ഷം_cache pcache;
} *pbase_വൃക്ഷം;

സ്ഥിതം ഘടന pbase_വൃക്ഷം_cache *pbase_വൃക്ഷം_get(സ്ഥിരം ഘടന object_id *oid)
{
	ഘടന pbase_വൃക്ഷം_cache *ent, *nent;
	ശൂന്യം *data;
	unsigned long size;
	size_t size_st = 0;
	എണ്ണൽ object_type type;
	പൂർണ്ണസംഖ്യ neigh;
	പൂർണ്ണസംഖ്യ my_ix = pbase_വൃക്ഷം_cache_ix(oid);
	പൂർണ്ണസംഖ്യ available_ix = -1;

	/* pbase-tree-cache acts as a limited hashtable.
	 * your object will be found at your index or within a few
	 * slots after that slot if it is cached.
	 */
	വേണ്ടി (neigh = 0; neigh < 8; neigh++) {
		ent = pbase_വൃക്ഷം_cache[my_ix];
		എങ്കിൽ (ent && oideq(&ent->oid, oid)) {
			ent->ref++;
			തിരികെ ent;
		}
		അല്ലെങ്കിൽ എങ്കിൽ (((available_ix < 0) && (!ent || !ent->ref)) ||
			 ((0 <= available_ix) &&
			  (!ent && pbase_വൃക്ഷം_cache[available_ix])))
			available_ix = my_ix;
		എങ്കിൽ (!ent)
			നിർത്തുക;
		my_ix = pbase_വൃക്ഷം_cache_ix_incr(my_ix);
	}

	/* Did not find one.  Either we got a bogus request or
	 * we need to read and perhaps cache.
	 */
	data = odb_read_object(the_repository->objects, oid, &type, &size_st);
	size = cast_size_t_to_ulong(size_st);
	എങ്കിൽ (!data)
		തിരികെ ശൂന്യ;
	എങ്കിൽ (type != OBJ_വൃക്ഷം) {
		സ്വതന്ത്രം(data);
		തിരികെ ശൂന്യ;
	}

	/* We need to either cache or return a throwaway copy */

	എങ്കിൽ (available_ix < 0)
		ent = ശൂന്യ;
	അല്ലെങ്കിൽ {
		ent = pbase_വൃക്ഷം_cache[available_ix];
		my_ix = available_ix;
	}

	എങ്കിൽ (!ent) {
		nent = xmalloc(വലുപ്പം(*nent));
		nent->temporary = (available_ix < 0);
	}
	അല്ലെങ്കിൽ {
		/* evict and reuse */
		സ്വതന്ത്രം(ent->വൃക്ഷം_data);
		nent = ent;
	}
	oidcpy(&nent->oid, oid);
	nent->വൃക്ഷം_data = data;
	nent->വൃക്ഷം_size = size;
	nent->ref = 1;
	എങ്കിൽ (!nent->temporary)
		pbase_വൃക്ഷം_cache[my_ix] = nent;
	തിരികെ nent;
}

സ്ഥിതം ശൂന്യം pbase_വൃക്ഷം_put(ഘടന pbase_വൃക്ഷം_cache *cache)
{
	എങ്കിൽ (!cache->temporary) {
		cache->ref--;
		തിരികെ;
	}
	സ്വതന്ത്രം(cache->വൃക്ഷം_data);
	സ്വതന്ത്രം(cache);
}

സ്ഥിതം size_t name_cmp_len(സ്ഥിരം അക്ഷരം *name)
{
	തിരികെ strcspn(name, "\n/");
}

സ്ഥിതം ശൂന്യം add_pbase_object(ഘടന വൃക്ഷം_desc *വൃക്ഷം,
			     സ്ഥിരം അക്ഷരം *name,
			     size_t cmplen,
			     സ്ഥിരം അക്ഷരം *fullname)
{
	ഘടന name_entry entry;
	പൂർണ്ണസംഖ്യ cmp;

	ആകുമ്പോൾ (വൃക്ഷം_entry(വൃക്ഷം,&entry)) {
		എങ്കിൽ (S_ISGITLINK(entry.mode))
			തുടരുക;
		cmp = വൃക്ഷം_entry_len(&entry) != cmplen ? 1 :
		      memcmp(name, entry.path, cmplen);
		എങ്കിൽ (cmp > 0)
			തുടരുക;
		എങ്കിൽ (cmp < 0)
			തിരികെ;
		എങ്കിൽ (name[cmplen] != '/') {
			add_object_entry(&entry.oid,
					 object_type(entry.mode),
					 fullname, 1);
			തിരികെ;
		}
		എങ്കിൽ (S_ISDIR(entry.mode)) {
			ഘടന വൃക്ഷം_desc sub;
			ഘടന pbase_വൃക്ഷം_cache *വൃക്ഷം;
			സ്ഥിരം അക്ഷരം *down = name+cmplen+1;
			size_t downlen = name_cmp_len(down);

			വൃക്ഷം = pbase_വൃക്ഷം_get(&entry.oid);
			എങ്കിൽ (!വൃക്ഷം)
				തിരികെ;
			init_വൃക്ഷം_desc(&sub, &വൃക്ഷം->oid,
				       വൃക്ഷം->വൃക്ഷം_data, വൃക്ഷം->വൃക്ഷം_size);

			add_pbase_object(&sub, down, downlen, fullname);
			pbase_വൃക്ഷം_put(വൃക്ഷം);
		}
	}
}

സ്ഥിതം unsigned *done_pbase_paths;
സ്ഥിതം പൂർണ്ണസംഖ്യ done_pbase_paths_അക്കം;
സ്ഥിതം പൂർണ്ണസംഖ്യ done_pbase_paths_alloc;
സ്ഥിതം പൂർണ്ണസംഖ്യ done_pbase_path_pos(unsigned hash)
{
	പൂർണ്ണസംഖ്യ lo = 0;
	പൂർണ്ണസംഖ്യ hi = done_pbase_paths_അക്കം;
	ആകുമ്പോൾ (lo < hi) {
		പൂർണ്ണസംഖ്യ mi = lo + (hi - lo) / 2;
		എങ്കിൽ (done_pbase_paths[mi] == hash)
			തിരികെ mi;
		എങ്കിൽ (done_pbase_paths[mi] < hash)
			hi = mi;
		അല്ലെങ്കിൽ
			lo = mi + 1;
	}
	തിരികെ -lo-1;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ check_pbase_path(unsigned hash)
{
	പൂർണ്ണസംഖ്യ pos = done_pbase_path_pos(hash);
	എങ്കിൽ (0 <= pos)
		തിരികെ 1;
	pos = -pos - 1;
	ALLOC_GROW(done_pbase_paths,
		   done_pbase_paths_അക്കം + 1,
		   done_pbase_paths_alloc);
	done_pbase_paths_അക്കം++;
	എങ്കിൽ (pos < done_pbase_paths_അക്കം)
		MOVE_ARRAY(done_pbase_paths + pos + 1, done_pbase_paths + pos,
			   done_pbase_paths_അക്കം - pos - 1);
	done_pbase_paths[pos] = hash;
	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം add_preferred_base_object(സ്ഥിരം അക്ഷരം *name)
{
	ഘടന pbase_വൃക്ഷം *it;
	size_t cmplen;
	unsigned hash = pack_name_hash_fn(name);

	എങ്കിൽ (!അക്കം_preferred_base || check_pbase_path(hash))
		തിരികെ;

	cmplen = name_cmp_len(name);
	വേണ്ടി (it = pbase_വൃക്ഷം; it; it = it->next) {
		എങ്കിൽ (cmplen == 0) {
			add_object_entry(&it->pcache.oid, OBJ_വൃക്ഷം, ശൂന്യ, 1);
		}
		അല്ലെങ്കിൽ {
			ഘടന വൃക്ഷം_desc വൃക്ഷം;
			init_വൃക്ഷം_desc(&വൃക്ഷം, &it->pcache.oid,
				       it->pcache.വൃക്ഷം_data, it->pcache.വൃക്ഷം_size);
			add_pbase_object(&വൃക്ഷം, name, cmplen, name);
		}
	}
}

സ്ഥിതം ശൂന്യം add_preferred_base(ഘടന object_id *oid)
{
	ഘടന pbase_വൃക്ഷം *it;
	ശൂന്യം *data;
	unsigned long size;
	size_t size_st = 0;
	ഘടന object_id വൃക്ഷം_oid;

	എങ്കിൽ (window <= അക്കം_preferred_base++)
		തിരികെ;

	data = odb_read_object_peeled(the_repository->objects, oid,
				      OBJ_വൃക്ഷം, &size_st, &വൃക്ഷം_oid);
	size = cast_size_t_to_ulong(size_st);
	എങ്കിൽ (!data)
		തിരികെ;

	വേണ്ടി (it = pbase_വൃക്ഷം; it; it = it->next) {
		എങ്കിൽ (oideq(&it->pcache.oid, &വൃക്ഷം_oid)) {
			സ്വതന്ത്രം(data);
			തിരികെ;
		}
	}

	CALLOC_ARRAY(it, 1);
	it->next = pbase_വൃക്ഷം;
	pbase_വൃക്ഷം = it;

	oidcpy(&it->pcache.oid, &വൃക്ഷം_oid);
	it->pcache.വൃക്ഷം_data = data;
	it->pcache.വൃക്ഷം_size = size;
}

സ്ഥിതം ശൂന്യം cleanup_preferred_base(ശൂന്യം)
{
	ഘടന pbase_വൃക്ഷം *it;
	unsigned i;

	it = pbase_വൃക്ഷം;
	pbase_വൃക്ഷം = ശൂന്യ;
	ആകുമ്പോൾ (it) {
		ഘടന pbase_വൃക്ഷം *tmp = it;
		it = tmp->next;
		സ്വതന്ത്രം(tmp->pcache.വൃക്ഷം_data);
		സ്വതന്ത്രം(tmp);
	}

	വേണ്ടി (i = 0; i < ARRAY_SIZE(pbase_വൃക്ഷം_cache); i++) {
		എങ്കിൽ (!pbase_വൃക്ഷം_cache[i])
			തുടരുക;
		സ്വതന്ത്രം(pbase_വൃക്ഷം_cache[i]->വൃക്ഷം_data);
		FREE_AND_NULL(pbase_വൃക്ഷം_cache[i]);
	}

	FREE_AND_NULL(done_pbase_paths);
	done_pbase_paths_അക്കം = done_pbase_paths_alloc = 0;
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
സ്ഥിതം പൂർണ്ണസംഖ്യ can_reuse_delta(സ്ഥിരം ഘടന object_id *base_oid,
			   ഘടന object_entry *delta,
			   ഘടന object_entry **base_out)
{
	ഘടന object_entry *base;

	/*
	 * First see if we're already sending the base (or it's explicitly in
	 * our "excluded" list).
	 */
	base = packlist_find(&to_pack, base_oid);
	എങ്കിൽ (base) {
		എങ്കിൽ (!in_same_island(&delta->idx.oid, &base->idx.oid))
			തിരികെ 0;
		*base_out = base;
		തിരികെ 1;
	}

	/*
	 * Otherwise, reachability bitmaps may tell us if the receiver has it,
	 * even if it was buried too deep in history to make it into the
	 * packing list.
	 */
	എങ്കിൽ (thin && bitmap_has_oid_in_uninteresting(bitmap_git, base_oid)) {
		എങ്കിൽ (use_delta_islands) {
			എങ്കിൽ (!in_same_island(&delta->idx.oid, base_oid))
				തിരികെ 0;
		}
		*base_out = ശൂന്യ;
		തിരികെ 1;
	}

	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം prefetch_to_pack(uint32_t object_index_start) {
	ഘടന oid_array to_fetch = OID_ARRAY_INIT;
	uint32_t i;

	വേണ്ടി (i = object_index_start; i < to_pack.nr_objects; i++) {
		ഘടന object_entry *entry = to_pack.objects + i;

		എങ്കിൽ (!odb_read_object_info_extended(the_repository->objects,
						   &entry->idx.oid,
						   ശൂന്യ,
						   OBJECT_INFO_FOR_PREFETCH))
			തുടരുക;
		oid_array_append(&to_fetch, &entry->idx.oid);
	}
	promisor_remote_get_direct(the_repository,
				   to_fetch.oid, to_fetch.nr);
	oid_array_clear(&to_fetch);
}

സ്ഥിതം ശൂന്യം check_object(ഘടന object_entry *entry, uint32_t object_index)
{
	size_t canonical_size;
	എണ്ണൽ object_type type;
	ഘടന object_info oi = {.typep = &type, .sizep = &canonical_size};

	എങ്കിൽ (IN_PACK(entry)) {
		ഘടന packed_git *p = IN_PACK(entry);
		ഘടന pack_window *w_curs = ശൂന്യ;
		പൂർണ്ണസംഖ്യ have_base = 0;
		ഘടന object_id base_ref;
		ഘടന object_entry *base_entry;
		size_t used, used_0, avail;
		off_t ofs;
		unsigned അക്ഷരം *buf, c;
		എണ്ണൽ object_type type;
		size_t in_pack_size;

		buf = use_pack(p, &w_curs, entry->in_pack_offset, &avail);

		/*
		 * We want in_pack_type even if we do not reuse delta
		 * since non-delta representations could still be reused.
		 */
		used = unpack_object_header_buffer(buf, avail,
						   &type,
						   &in_pack_size);
		എങ്കിൽ (used == 0)
			പോകുക give_up;

		എങ്കിൽ (type < 0)
			BUG("invalid type %d", type);
		entry->in_pack_type = type;

		/*
		 * Determine if this is a delta and if so whether we can
		 * reuse it or not.  Otherwise let's find out as cheaply as
		 * possible what the actual type and size for this object is.
		 */
		തിരഞ്ഞെടുപ്പ് (entry->in_pack_type) {
		സ്ഥിരസ്ഥിതി:
			/* Not a delta hence we've already got all we need. */
			oe_set_type(entry, entry->in_pack_type);
			SET_SIZE(entry, in_pack_size);
			entry->in_pack_header_size = used;
			എങ്കിൽ (oe_type(entry) < OBJ_COMMIT || oe_type(entry) > OBJ_BLOB)
				പോകുക give_up;
			unuse_pack(&w_curs);
			തിരികെ;
		അവസ്ഥ OBJ_REF_DELTA:
			എങ്കിൽ (reuse_delta && !entry->preferred_base) {
				oidread(&base_ref,
					use_pack(p, &w_curs,
						 entry->in_pack_offset + used,
						 ശൂന്യ),
					the_repository->hash_algo);
				have_base = 1;
			}
			entry->in_pack_header_size = used + the_hash_algo->rawsz;
			നിർത്തുക;
		അവസ്ഥ OBJ_OFS_DELTA:
			buf = use_pack(p, &w_curs,
				       entry->in_pack_offset + used, ശൂന്യ);
			used_0 = 0;
			c = buf[used_0++];
			ofs = c & 127;
			ആകുമ്പോൾ (c & 128) {
				ofs += 1;
				എങ്കിൽ (!ofs || MSB(ofs, 7)) {
					error(_("delta base offset overflow in pack for %s"),
					      oid_to_hex(&entry->idx.oid));
					പോകുക give_up;
				}
				c = buf[used_0++];
				ofs = (ofs << 7) + (c & 127);
			}
			ofs = entry->in_pack_offset - ofs;
			എങ്കിൽ (ofs <= 0 || ofs >= entry->in_pack_offset) {
				error(_("delta base offset out of bound for %s"),
				      oid_to_hex(&entry->idx.oid));
				പോകുക give_up;
			}
			എങ്കിൽ (reuse_delta && !entry->preferred_base) {
				uint32_t pos;
				എങ്കിൽ (offset_to_pack_pos(p, ofs, &pos) < 0)
					പോകുക give_up;
				എങ്കിൽ (!nth_packed_object_id(&base_ref, p,
							  pack_pos_to_index(p, pos)))
					have_base = 1;
			}
			entry->in_pack_header_size = used + used_0;
			നിർത്തുക;
		}

		എങ്കിൽ (have_base &&
		    can_reuse_delta(&base_ref, entry, &base_entry)) {
			oe_set_type(entry, entry->in_pack_type);
			SET_SIZE(entry, in_pack_size); /* delta size */
			SET_DELTA_SIZE(entry, in_pack_size);

			എങ്കിൽ (base_entry) {
				SET_DELTA(entry, base_entry);
				entry->delta_sibling_idx = base_entry->delta_child_idx;
				SET_DELTA_CHILD(base_entry, entry);
			} അല്ലെങ്കിൽ {
				SET_DELTA_EXT(entry, &base_ref);
			}

			unuse_pack(&w_curs);
			തിരികെ;
		}

		എങ്കിൽ (oe_type(entry)) {
			off_t delta_pos;

			/*
			 * This must be a delta and we already know what the
			 * final object type is.  Let's extract the actual
			 * object size from the delta header.
			 */
			delta_pos = entry->in_pack_offset + entry->in_pack_header_size;
			canonical_size = get_size_from_delta(p, &w_curs,
							     delta_pos);
			എങ്കിൽ (canonical_size == 0)
				പോകുക give_up;
			SET_SIZE(entry, canonical_size);
			unuse_pack(&w_curs);
			തിരികെ;
		}

		/*
		 * No choice but to fall back to the recursive delta walk
		 * with odb_read_object_info() to find about the object type
		 * at this point...
		 */
		give_up:
		unuse_pack(&w_curs);
	}

	എങ്കിൽ (odb_read_object_info_extended(the_repository->objects, &entry->idx.oid, &oi,
					  OBJECT_INFO_SKIP_FETCH_OBJECT | OBJECT_INFO_LOOKUP_REPLACE) < 0) {
		എങ്കിൽ (repo_has_promisor_remote(the_repository)) {
			prefetch_to_pack(object_index);
			എങ്കിൽ (odb_read_object_info_extended(the_repository->objects, &entry->idx.oid, &oi,
							  OBJECT_INFO_SKIP_FETCH_OBJECT | OBJECT_INFO_LOOKUP_REPLACE) < 0)
				type = -1;
		} അല്ലെങ്കിൽ {
			type = -1;
		}
	}
	oe_set_type(entry, type);
	എങ്കിൽ (entry->type_valid) {
		SET_SIZE(entry, canonical_size);
	} അല്ലെങ്കിൽ {
		/*
		 * Bad object type is checked in prepare_pack().  This is
		 * to permit a missing preferred base object to be ignored
		 * as a preferred base.  Doing so can result in a larger
		 * pack file, but the transfer will still take place.
		 */
	}
}

സ്ഥിതം പൂർണ്ണസംഖ്യ pack_offset_sort(സ്ഥിരം ശൂന്യം *_a, സ്ഥിരം ശൂന്യം *_b)
{
	സ്ഥിരം ഘടന object_entry *a = *(ഘടന object_entry **)_a;
	സ്ഥിരം ഘടന object_entry *b = *(ഘടന object_entry **)_b;
	സ്ഥിരം ഘടന packed_git *a_in_pack = IN_PACK(a);
	സ്ഥിരം ഘടന packed_git *b_in_pack = IN_PACK(b);

	/* avoid filesystem trashing with loose objects */
	എങ്കിൽ (!a_in_pack && !b_in_pack)
		തിരികെ oidcmp(&a->idx.oid, &b->idx.oid);

	എങ്കിൽ (a_in_pack < b_in_pack)
		തിരികെ -1;
	എങ്കിൽ (a_in_pack > b_in_pack)
		തിരികെ 1;
	തിരികെ a->in_pack_offset < b->in_pack_offset ? -1 :
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
സ്ഥിതം ശൂന്യം drop_reused_delta(ഘടന object_entry *entry)
{
	unsigned *idx = &to_pack.objects[entry->delta_idx - 1].delta_child_idx;
	ഘടന object_info oi = OBJECT_INFO_INIT;
	എണ്ണൽ object_type type;
	size_t size;

	ആകുമ്പോൾ (*idx) {
		ഘടന object_entry *oe = &to_pack.objects[*idx - 1];

		എങ്കിൽ (oe == entry)
			*idx = oe->delta_sibling_idx;
		അല്ലെങ്കിൽ
			idx = &oe->delta_sibling_idx;
	}
	SET_DELTA(entry, ശൂന്യ);
	entry->depth = 0;

	oi.sizep = &size;
	oi.typep = &type;
	എങ്കിൽ (packed_object_info(ശൂന്യ, IN_PACK(entry), entry->in_pack_offset, &oi) < 0) {
		/*
		 * We failed to get the info from this pack for some reason;
		 * fall back to odb_read_object_info, which may find another copy.
		 * And if that fails, the error will be recorded in oe_type(entry)
		 * and dealt with in prepare_pack().
		 */
		oe_set_type(entry,
			    odb_read_object_info(the_repository->objects,
						 &entry->idx.oid, &size));
	} അല്ലെങ്കിൽ {
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
സ്ഥിതം ശൂന്യം break_delta_chains(ഘടന object_entry *entry)
{
	/*
	 * The actual depth of each object we will write is stored as an int,
	 * as it cannot exceed our int "depth" limit. But before we break
	 * changes based no that limit, we may potentially go as deep as the
	 * number of objects, which is elsewhere bounded to a uint32_t.
	 */
	uint32_t total_depth;
	ഘടന object_entry *cur, *next;

	വേണ്ടി (cur = entry, total_depth = 0;
	     cur;
	     cur = DELTA(cur), total_depth++) {
		എങ്കിൽ (cur->dfs_state == DFS_DONE) {
			/*
			 * We've already seen this object and know it isn't
			 * part of a cycle. We do need to append its depth
			 * to our count.
			 */
			total_depth += cur->depth;
			നിർത്തുക;
		}

		/*
		 * We break cycles before looping, so an ACTIVE state (or any
		 * other cruft which made its way into the state variable)
		 * is a bug.
		 */
		എങ്കിൽ (cur->dfs_state != DFS_NONE)
			BUG("confusing delta dfs state in first pass: %d",
			    cur->dfs_state);

		/*
		 * Now we know this is the first time we've seen the object. If
		 * it's not a delta, we're done traversing, but we'll mark it
		 * done to save time on future traversals.
		 */
		എങ്കിൽ (!DELTA(cur)) {
			cur->dfs_state = DFS_DONE;
			നിർത്തുക;
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
		എങ്കിൽ (DELTA(cur)->dfs_state == DFS_ACTIVE) {
			drop_reused_delta(cur);
			cur->dfs_state = DFS_DONE;
			നിർത്തുക;
		}
	}

	/*
	 * And now that we've gone all the way to the bottom of the chain, we
	 * need to clear the active flags and set the depth fields as
	 * appropriate. Unlike the loop above, which can quit when it drops a
	 * delta, we need to keep going to look for more depth cuts. So we need
	 * an extra "next" pointer to keep going after we reset cur->delta.
	 */
	വേണ്ടി (cur = entry; cur; cur = next) {
		next = DELTA(cur);

		/*
		 * We should have a chain of zero or more ACTIVE states down to
		 * a final DONE. We can quit after the DONE, because either it
		 * has no bases, or we've already handled them in a previous
		 * call.
		 */
		എങ്കിൽ (cur->dfs_state == DFS_DONE)
			നിർത്തുക;
		അല്ലെങ്കിൽ എങ്കിൽ (cur->dfs_state != DFS_ACTIVE)
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
		എങ്കിൽ (!cur->depth)
			drop_reused_delta(cur);

		cur->dfs_state = DFS_DONE;
	}
}

സ്ഥിതം ശൂന്യം get_object_details(ശൂന്യം)
{
	uint32_t i;
	ഘടന object_entry **sorted_by_offset;

	എങ്കിൽ (progress)
		progress_state = start_progress(the_repository,
						_("Counting objects"),
						to_pack.nr_objects);

	CALLOC_ARRAY(sorted_by_offset, to_pack.nr_objects);
	വേണ്ടി (i = 0; i < to_pack.nr_objects; i++)
		sorted_by_offset[i] = to_pack.objects + i;
	QSORT(sorted_by_offset, to_pack.nr_objects, pack_offset_sort);

	വേണ്ടി (i = 0; i < to_pack.nr_objects; i++) {
		ഘടന object_entry *entry = sorted_by_offset[i];
		check_object(entry, i);
		എങ്കിൽ (entry->type_valid &&
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
	വേണ്ടി (i = 0; i < to_pack.nr_objects; i++)
		break_delta_chains(&to_pack.objects[i]);

	സ്വതന്ത്രം(sorted_by_offset);
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
സ്ഥിതം പൂർണ്ണസംഖ്യ type_size_sort(സ്ഥിരം ശൂന്യം *_a, സ്ഥിരം ശൂന്യം *_b)
{
	സ്ഥിരം ഘടന object_entry *a = *(ഘടന object_entry **)_a;
	സ്ഥിരം ഘടന object_entry *b = *(ഘടന object_entry **)_b;
	സ്ഥിരം എണ്ണൽ object_type a_type = oe_type(a);
	സ്ഥിരം എണ്ണൽ object_type b_type = oe_type(b);
	സ്ഥിരം unsigned long a_size = SIZE(a);
	സ്ഥിരം unsigned long b_size = SIZE(b);

	എങ്കിൽ (a_type > b_type)
		തിരികെ -1;
	എങ്കിൽ (a_type < b_type)
		തിരികെ 1;
	എങ്കിൽ (a->hash > b->hash)
		തിരികെ -1;
	എങ്കിൽ (a->hash < b->hash)
		തിരികെ 1;
	എങ്കിൽ (a->preferred_base > b->preferred_base)
		തിരികെ -1;
	എങ്കിൽ (a->preferred_base < b->preferred_base)
		തിരികെ 1;
	എങ്കിൽ (use_delta_islands) {
		സ്ഥിരം പൂർണ്ണസംഖ്യ island_cmp = island_delta_cmp(&a->idx.oid, &b->idx.oid);
		എങ്കിൽ (island_cmp)
			തിരികെ island_cmp;
	}
	എങ്കിൽ (a_size > b_size)
		തിരികെ -1;
	എങ്കിൽ (a_size < b_size)
		തിരികെ 1;
	തിരികെ a < b ? -1 : (a > b);  /* newest first */
}

ഘടന unpacked {
	ഘടന object_entry *entry;
	ശൂന്യം *data;
	ഘടന delta_index *index;
	unsigned depth;
};

സ്ഥിതം പൂർണ്ണസംഖ്യ delta_cacheable(size_t src_size, size_t trg_size,
			   size_t delta_size)
{
	എങ്കിൽ (max_delta_cache_size && delta_cache_size + delta_size > max_delta_cache_size)
		തിരികെ 0;

	എങ്കിൽ (delta_size < cache_max_small_delta_size)
		തിരികെ 1;

	/* cache delta, if objects are large enough compared to delta size */
	എങ്കിൽ ((src_size >> 20) + (trg_size >> 21) > (delta_size >> 10))
		തിരികെ 1;

	തിരികെ 0;
}

/* Protect delta_cache_size */
സ്ഥിതം pthread_mutex_t cache_mutex;
#define cache_lock()		pthread_mutex_lock(&cache_mutex)
#define cache_unlock()		pthread_mutex_unlock(&cache_mutex)

/*
 * Protect object list partitioning (e.g. struct thread_param) and
 * progress_state
 */
സ്ഥിതം pthread_mutex_t progress_mutex;
#define progress_lock()		pthread_mutex_lock(&progress_mutex)
#define progress_unlock()	pthread_mutex_unlock(&progress_mutex)

/*
 * Access to struct object_entry is unprotected since each thread owns
 * a portion of the main object list. Just don't access object entries
 * ahead in the list because they can be stolen and would need
 * progress_mutex for protection.
 */

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ oe_size_less_than(ഘടന packing_data *pack,
				    സ്ഥിരം ഘടന object_entry *lhs,
				    size_t rhs)
{
	എങ്കിൽ (lhs->size_valid)
		തിരികെ lhs->size_ < rhs;
	എങ്കിൽ (rhs < pack->oe_size_limit) /* rhs < 2^x <= lhs ? */
		തിരികെ 0;
	തിരികെ oe_get_size_slow(pack, lhs) < rhs;
}

സ്ഥിതം വരിയിൽ ശൂന്യം oe_set_വൃക്ഷം_depth(ഘടന packing_data *pack,
				     ഘടന object_entry *e,
				     unsigned പൂർണ്ണസംഖ്യ വൃക്ഷം_depth)
{
	എങ്കിൽ (!pack->വൃക്ഷം_depth)
		CALLOC_ARRAY(pack->വൃക്ഷം_depth, pack->nr_alloc);
	pack->വൃക്ഷം_depth[e - pack->objects] = വൃക്ഷം_depth;
}

സ്ഥിതം ശൂന്യം record_വൃക്ഷം_depth(സ്ഥിരം ഘടന object_id *oid, സ്ഥിരം അക്ഷരം *name)
{
	സ്ഥിരം അക്ഷരം *p;
	unsigned depth;
	ഘടന object_entry *ent;

	/* the empty string is a root tree, which is depth 0 */
	depth = *name ? 1 : 0;
	വേണ്ടി (p = strchr(name, '/'); p; p = strchr(p + 1, '/'))
		depth++;

	ent = packlist_find(&to_pack, oid);
	എങ്കിൽ (ent && depth > oe_വൃക്ഷം_depth(&to_pack, ent))
		oe_set_വൃക്ഷം_depth(&to_pack, ent, depth);
}

/*
 * Return the size of the object without doing any delta
 * reconstruction (so non-deltas are true object sizes, but deltas
 * return the size of the delta data).
 */
size_t oe_get_size_slow(ഘടന packing_data *pack,
			സ്ഥിരം ഘടന object_entry *e)
{
	ഘടന packed_git *p;
	ഘടന pack_window *w_curs;
	unsigned അക്ഷരം *buf;
	എണ്ണൽ object_type type;
	size_t used, avail, size;

	എങ്കിൽ (e->type_ != OBJ_OFS_DELTA && e->type_ != OBJ_REF_DELTA) {
		size_t sz;
		packing_data_lock(&to_pack);
		എങ്കിൽ (odb_read_object_info(the_repository->objects,
					 &e->idx.oid, &sz) < 0)
			die(_("unable to get size of %s"),
			    oid_to_hex(&e->idx.oid));
		packing_data_unlock(&to_pack);
		തിരികെ sz;
	}

	p = oe_in_pack(pack, e);
	എങ്കിൽ (!p)
		BUG("when e->type is a delta, it must belong to a pack");

	packing_data_lock(&to_pack);
	w_curs = ശൂന്യ;
	buf = use_pack(p, &w_curs, e->in_pack_offset, &avail);
	used = unpack_object_header_buffer(buf, avail, &type, &size);
	എങ്കിൽ (used == 0)
		die(_("unable to parse object header of %s"),
		    oid_to_hex(&e->idx.oid));

	unuse_pack(&w_curs);
	packing_data_unlock(&to_pack);
	തിരികെ size;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ try_delta(ഘടന unpacked *trg, ഘടന unpacked *src,
		     unsigned max_depth, size_t *mem_usage)
{
	ഘടന object_entry *trg_entry = trg->entry;
	ഘടന object_entry *src_entry = src->entry;
	unsigned long trg_size, src_size, sizediff, max_size, sz;
	size_t delta_size;
	unsigned ref_depth;
	എണ്ണൽ object_type type;
	ശൂന്യം *delta_buf;

	/* Don't bother doing diffs between different types */
	എങ്കിൽ (oe_type(trg_entry) != oe_type(src_entry))
		തിരികെ -1;

	/*
	 * We do not bother to try a delta that we discarded on an
	 * earlier try, but only when reusing delta data.  Note that
	 * src_entry that is marked as the preferred_base should always
	 * be considered, as even if we produce a suboptimal delta against
	 * it, we will still save the transfer cost, as we already know
	 * the other side has it and we won't send src_entry at all.
	 */
	എങ്കിൽ (reuse_delta && IN_PACK(trg_entry) &&
	    IN_PACK(trg_entry) == IN_PACK(src_entry) &&
	    !src_entry->preferred_base &&
	    trg_entry->in_pack_type != OBJ_REF_DELTA &&
	    trg_entry->in_pack_type != OBJ_OFS_DELTA)
		തിരികെ 0;

	/* Let's not bust the allowed depth. */
	എങ്കിൽ (src->depth >= max_depth)
		തിരികെ 0;

	/* Now some size filtering heuristics. */
	trg_size = SIZE(trg_entry);
	എങ്കിൽ (!DELTA(trg_entry)) {
		max_size = trg_size/2 - the_hash_algo->rawsz;
		ref_depth = 1;
	} അല്ലെങ്കിൽ {
		max_size = DELTA_SIZE(trg_entry);
		ref_depth = trg->depth;
	}
	max_size = (uint64_t)max_size * (max_depth - src->depth) /
						(max_depth - ref_depth + 1);
	എങ്കിൽ (max_size == 0)
		തിരികെ 0;
	src_size = SIZE(src_entry);
	sizediff = src_size < trg_size ? trg_size - src_size : 0;
	എങ്കിൽ (sizediff >= max_size)
		തിരികെ 0;
	എങ്കിൽ (trg_size < src_size / 32)
		തിരികെ 0;

	എങ്കിൽ (!in_same_island(&trg->entry->idx.oid, &src->entry->idx.oid))
		തിരികെ 0;

	/* Load data if not already done */
	എങ്കിൽ (!trg->data) {
		size_t sz_st = 0;
		packing_data_lock(&to_pack);
		trg->data = odb_read_object(the_repository->objects,
					    &trg_entry->idx.oid, &type,
					    &sz_st);
		sz = cast_size_t_to_ulong(sz_st);
		packing_data_unlock(&to_pack);
		എങ്കിൽ (!trg->data)
			die(_("object %s cannot be read"),
			    oid_to_hex(&trg_entry->idx.oid));
		എങ്കിൽ (sz != trg_size)
			die(_("object %s inconsistent object length (%"PRIuMAX" vs %"PRIuMAX")"),
			    oid_to_hex(&trg_entry->idx.oid), (uintmax_t)sz,
			    (uintmax_t)trg_size);
		*mem_usage += sz;
	}
	എങ്കിൽ (!src->data) {
		size_t sz_st = 0;
		packing_data_lock(&to_pack);
		src->data = odb_read_object(the_repository->objects,
					    &src_entry->idx.oid, &type,
					    &sz_st);
		sz = cast_size_t_to_ulong(sz_st);
		packing_data_unlock(&to_pack);
		എങ്കിൽ (!src->data) {
			എങ്കിൽ (src_entry->preferred_base) {
				സ്ഥിതം പൂർണ്ണസംഖ്യ warned = 0;
				എങ്കിൽ (!warned++)
					warning(_("object %s cannot be read"),
						oid_to_hex(&src_entry->idx.oid));
				/*
				 * Those objects are not included in the
				 * resulting pack.  Be resilient and ignore
				 * them if they can't be read, in case the
				 * pack could be created nevertheless.
				 */
				തിരികെ 0;
			}
			die(_("object %s cannot be read"),
			    oid_to_hex(&src_entry->idx.oid));
		}
		എങ്കിൽ (sz != src_size)
			die(_("object %s inconsistent object length (%"PRIuMAX" vs %"PRIuMAX")"),
			    oid_to_hex(&src_entry->idx.oid), (uintmax_t)sz,
			    (uintmax_t)src_size);
		*mem_usage += sz;
	}
	എങ്കിൽ (!src->index) {
		src->index = create_delta_index(src->data, src_size);
		എങ്കിൽ (!src->index) {
			സ്ഥിതം പൂർണ്ണസംഖ്യ warned = 0;
			എങ്കിൽ (!warned++)
				warning(_("suboptimal pack - out of memory"));
			തിരികെ 0;
		}
		*mem_usage += sizeof_delta_index(src->index);
	}

	delta_buf = create_delta(src->index, trg->data, trg_size, &delta_size, max_size);
	എങ്കിൽ (!delta_buf)
		തിരികെ 0;

	എങ്കിൽ (DELTA(trg_entry)) {
		/* Prefer only shallower same-sized deltas. */
		എങ്കിൽ (delta_size == DELTA_SIZE(trg_entry) &&
		    src->depth + 1 >= trg->depth) {
			സ്വതന്ത്രം(delta_buf);
			തിരികെ 0;
		}
	}

	/*
	 * Handle memory allocation outside of the cache
	 * accounting lock.  Compiler will optimize the strangeness
	 * away when NO_PTHREADS is defined.
	 */
	സ്വതന്ത്രം(trg_entry->delta_data);
	cache_lock();
	എങ്കിൽ (trg_entry->delta_data) {
		delta_cache_size -= DELTA_SIZE(trg_entry);
		trg_entry->delta_data = ശൂന്യ;
	}
	എങ്കിൽ (delta_cacheable(src_size, trg_size, delta_size)) {
		delta_cache_size += delta_size;
		cache_unlock();
		trg_entry->delta_data = xrealloc(delta_buf, delta_size);
	} അല്ലെങ്കിൽ {
		cache_unlock();
		സ്വതന്ത്രം(delta_buf);
	}

	SET_DELTA(trg_entry, src_entry);
	SET_DELTA_SIZE(trg_entry, delta_size);
	trg->depth = src->depth + 1;

	തിരികെ 1;
}

സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ check_delta_limit(ഘടന object_entry *me, unsigned പൂർണ്ണസംഖ്യ n)
{
	ഘടന object_entry *child = DELTA_CHILD(me);
	unsigned പൂർണ്ണസംഖ്യ m = n;
	ആകുമ്പോൾ (child) {
		സ്ഥിരം unsigned പൂർണ്ണസംഖ്യ c = check_delta_limit(child, n + 1);
		എങ്കിൽ (m < c)
			m = c;
		child = DELTA_SIBLING(child);
	}
	തിരികെ m;
}

സ്ഥിതം size_t free_unpacked(ഘടന unpacked *n)
{
	size_t freed_mem = sizeof_delta_index(n->index);
	free_delta_index(n->index);
	n->index = ശൂന്യ;
	എങ്കിൽ (n->data) {
		freed_mem += SIZE(n->entry);
		FREE_AND_NULL(n->data);
	}
	n->entry = ശൂന്യ;
	n->depth = 0;
	തിരികെ freed_mem;
}

സ്ഥിതം ശൂന്യം find_deltas(ഘടന object_entry **list, unsigned *list_size,
			പൂർണ്ണസംഖ്യ window, പൂർണ്ണസംഖ്യ depth, unsigned *processed)
{
	uint32_t i, idx = 0, എണ്ണം = 0;
	ഘടന unpacked *array;
	size_t mem_usage = 0;

	CALLOC_ARRAY(array, window);

	വേണ്ടി (;;) {
		ഘടന object_entry *entry;
		ഘടന unpacked *n = array + idx;
		പൂർണ്ണസംഖ്യ j, max_depth, best_base = -1;

		progress_lock();
		എങ്കിൽ (!*list_size) {
			progress_unlock();
			നിർത്തുക;
		}
		entry = *list++;
		(*list_size)--;
		എങ്കിൽ (!entry->preferred_base) {
			(*processed)++;
			display_progress(progress_state, *processed);
		}
		progress_unlock();

		mem_usage -= free_unpacked(n);
		n->entry = entry;

		ആകുമ്പോൾ (window_memory_limit &&
		       mem_usage > window_memory_limit &&
		       എണ്ണം > 1) {
			സ്ഥിരം uint32_t tail = (idx + window - എണ്ണം) % window;
			mem_usage -= free_unpacked(array + tail);
			എണ്ണം--;
		}

		/* We do not compute delta to *create* objects we are not
		 * going to pack.
		 */
		എങ്കിൽ (entry->preferred_base)
			പോകുക next;

		/*
		 * If the current object is at pack edge, take the depth the
		 * objects that depend on the current object into account
		 * otherwise they would become too deep.
		 */
		max_depth = depth;
		എങ്കിൽ (DELTA_CHILD(entry)) {
			max_depth -= check_delta_limit(entry, 0);
			എങ്കിൽ (max_depth <= 0)
				പോകുക next;
		}

		j = window;
		ആകുമ്പോൾ (--j > 0) {
			പൂർണ്ണസംഖ്യ ret;
			uint32_t other_idx = idx + j;
			ഘടന unpacked *m;
			എങ്കിൽ (other_idx >= window)
				other_idx -= window;
			m = array + other_idx;
			എങ്കിൽ (!m->entry)
				നിർത്തുക;
			ret = try_delta(n, m, max_depth, &mem_usage);
			എങ്കിൽ (ret < 0)
				നിർത്തുക;
			അല്ലെങ്കിൽ എങ്കിൽ (ret > 0)
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
		എങ്കിൽ (entry->delta_data && !pack_to_stdout) {
			unsigned long size;

			size = do_compress(&entry->delta_data, DELTA_SIZE(entry));
			എങ്കിൽ (size < (1U << OE_Z_DELTA_BITS)) {
				entry->z_delta_size = size;
				cache_lock();
				delta_cache_size -= DELTA_SIZE(entry);
				delta_cache_size += entry->z_delta_size;
				cache_unlock();
			} അല്ലെങ്കിൽ {
				FREE_AND_NULL(entry->delta_data);
				entry->z_delta_size = 0;
			}
		}

		/* if we made n a delta, and if n is already at max
		 * depth, leaving it in the window is pointless.  we
		 * should evict it first.
		 */
		എങ്കിൽ (DELTA(entry) && max_depth <= n->depth)
			തുടരുക;

		/*
		 * Move the best delta base up in the window, after the
		 * currently deltified object, to keep it longer.  It will
		 * be the first base object to be attempted next.
		 */
		എങ്കിൽ (DELTA(entry)) {
			ഘടന unpacked swap = array[best_base];
			പൂർണ്ണസംഖ്യ dist = (window + idx - best_base) % window;
			പൂർണ്ണസംഖ്യ dst = best_base;
			ആകുമ്പോൾ (dist--) {
				പൂർണ്ണസംഖ്യ src = (dst + 1) % window;
				array[dst] = array[src];
				dst = src;
			}
			array[dst] = swap;
		}

		next:
		idx++;
		എങ്കിൽ (എണ്ണം + 1 < window)
			എണ്ണം++;
		എങ്കിൽ (idx >= window)
			idx = 0;
	}

	വേണ്ടി (i = 0; i < window; ++i) {
		free_delta_index(array[i].index);
		സ്വതന്ത്രം(array[i].data);
	}
	സ്വതന്ത്രം(array);
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

ഘടന thread_params {
	pthread_t thread;
	ഘടന object_entry **list;
	ഘടന packing_region *regions;
	unsigned list_size;
	unsigned remaining;
	പൂർണ്ണസംഖ്യ window;
	പൂർണ്ണസംഖ്യ depth;
	പൂർണ്ണസംഖ്യ working;
	പൂർണ്ണസംഖ്യ data_ready;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	unsigned *processed;
};

സ്ഥിതം pthread_cond_t progress_cond;

/*
 * Mutex and conditional variable can't be statically-initialized on Windows.
 */
സ്ഥിതം ശൂന്യം init_threaded_തിരയൽ(ശൂന്യം)
{
	pthread_mutex_init(&cache_mutex, ശൂന്യ);
	pthread_mutex_init(&progress_mutex, ശൂന്യ);
	pthread_cond_init(&progress_cond, ശൂന്യ);
}

സ്ഥിതം ശൂന്യം cleanup_threaded_തിരയൽ(ശൂന്യം)
{
	pthread_cond_destroy(&progress_cond);
	pthread_mutex_destroy(&cache_mutex);
	pthread_mutex_destroy(&progress_mutex);
}

സ്ഥിതം ശൂന്യം *threaded_find_deltas(ശൂന്യം *arg)
{
	ഘടന thread_params *me = arg;

	progress_lock();
	ആകുമ്പോൾ (me->remaining) {
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
		ആകുമ്പോൾ (!me->data_ready)
			pthread_cond_wait(&me->cond, &me->mutex);
		me->data_ready = 0;
		pthread_mutex_unlock(&me->mutex);

		progress_lock();
	}
	progress_unlock();
	/* leave ->working 1 so that this doesn't get more work assigned */
	തിരികെ ശൂന്യ;
}

സ്ഥിതം ശൂന്യം ll_find_deltas(ഘടന object_entry **list, unsigned list_size,
			   പൂർണ്ണസംഖ്യ window, പൂർണ്ണസംഖ്യ depth, unsigned *processed)
{
	ഘടന thread_params *p;
	പൂർണ്ണസംഖ്യ i, ret, active_threads = 0;

	init_threaded_തിരയൽ();

	എങ്കിൽ (delta_തിരയൽ_threads <= 1) {
		find_deltas(list, &list_size, window, depth, processed);
		cleanup_threaded_തിരയൽ();
		തിരികെ;
	}
	എങ്കിൽ (progress > pack_to_stdout)
		fprintf_ln(stderr, _("Delta compression using up to %d threads"),
			   delta_തിരയൽ_threads);
	CALLOC_ARRAY(p, delta_തിരയൽ_threads);

	/* Partition the work amongst work threads. */
	വേണ്ടി (i = 0; i < delta_തിരയൽ_threads; i++) {
		unsigned sub_size = list_size / (delta_തിരയൽ_threads - i);

		/* don't use too small segments or no deltas will be found */
		എങ്കിൽ (sub_size < 2*window && i+1 < delta_തിരയൽ_threads)
			sub_size = 0;

		p[i].window = window;
		p[i].depth = depth;
		p[i].processed = processed;
		p[i].working = 1;
		p[i].data_ready = 0;

		/* try to split chunks on "path" boundaries */
		ആകുമ്പോൾ (sub_size && sub_size < list_size &&
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
	വേണ്ടി (i = 0; i < delta_തിരയൽ_threads; i++) {
		എങ്കിൽ (!p[i].list_size)
			തുടരുക;
		pthread_mutex_init(&p[i].mutex, ശൂന്യ);
		pthread_cond_init(&p[i].cond, ശൂന്യ);
		ret = pthread_create(&p[i].thread, ശൂന്യ,
				     threaded_find_deltas, &p[i]);
		എങ്കിൽ (ret)
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
	ആകുമ്പോൾ (active_threads) {
		ഘടന thread_params *target = ശൂന്യ;
		ഘടന thread_params *victim = ശൂന്യ;
		unsigned sub_size = 0;

		progress_lock();
		വേണ്ടി (;;) {
			വേണ്ടി (i = 0; !target && i < delta_തിരയൽ_threads; i++)
				എങ്കിൽ (!p[i].working)
					target = &p[i];
			എങ്കിൽ (target)
				നിർത്തുക;
			pthread_cond_wait(&progress_cond, &progress_mutex);
		}

		വേണ്ടി (i = 0; i < delta_തിരയൽ_threads; i++)
			എങ്കിൽ (p[i].remaining > 2*window &&
			    (!victim || victim->remaining < p[i].remaining))
				victim = &p[i];
		എങ്കിൽ (victim) {
			sub_size = victim->remaining / 2;
			list = victim->list + victim->list_size - sub_size;
			ആകുമ്പോൾ (sub_size && list[0]->hash &&
			       list[0]->hash == list[-1]->hash) {
				list++;
				sub_size--;
			}
			എങ്കിൽ (!sub_size) {
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

		എങ്കിൽ (!sub_size) {
			pthread_join(target->thread, ശൂന്യ);
			pthread_cond_destroy(&target->cond);
			pthread_mutex_destroy(&target->mutex);
			active_threads--;
		}
	}
	cleanup_threaded_തിരയൽ();
	സ്വതന്ത്രം(p);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ obj_is_packed(സ്ഥിരം ഘടന object_id *oid)
{
	തിരികെ packlist_find(&to_pack, oid) ||
		(reuse_packfile_bitmap &&
		 bitmap_walk_contains(bitmap_git, reuse_packfile_bitmap, oid));
}

സ്ഥിതം ശൂന്യം add_tag_chain(സ്ഥിരം ഘടന object_id *oid)
{
	ഘടന tag *tag;

	/*
	 * We catch duplicates already in add_object_entry(), but we'd
	 * prefer to do this extra check to avoid having to parse the
	 * tag at all if we already know that it's being packed (e.g., if
	 * it was included via bitmaps, we would not have parsed it
	 * previously).
	 */
	എങ്കിൽ (obj_is_packed(oid))
		തിരികെ;

	tag = lookup_tag(the_repository, oid);
	ആകുമ്പോൾ (1) {
		എങ്കിൽ (!tag || parse_tag(the_repository, tag) || !tag->tagged)
			die(_("unable to pack objects reachable from tag %s"),
			    oid_to_hex(oid));

		add_object_entry(&tag->object.oid, OBJ_TAG, ശൂന്യ, 0);

		എങ്കിൽ (tag->tagged->type != OBJ_TAG)
			തിരികെ;

		tag = (ഘടന tag *)tag->tagged;
	}
}

സ്ഥിതം പൂർണ്ണസംഖ്യ add_ref_tag(സ്ഥിരം ഘടന reference *ref, ശൂന്യം *cb_data UNUSED)
{
	ഘടന object_id peeled;

	എങ്കിൽ (!reference_get_peeled_oid(the_repository, ref, &peeled) &&
	    obj_is_packed(&peeled))
		add_tag_chain(ref->oid);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ should_attempt_deltas(ഘടന object_entry *entry)
{
	എങ്കിൽ (DELTA(entry))
		/* This happens if we decided to reuse existing
		 * delta from a pack. "reuse_delta &&" is implied.
		 */
		തിരികെ 0;

	എങ്കിൽ (!entry->type_valid ||
	    oe_size_less_than(&to_pack, entry, 50))
		തിരികെ 0;

	എങ്കിൽ (entry->no_try_delta)
		തിരികെ 0;

	എങ്കിൽ (!entry->preferred_base) {
		എങ്കിൽ (oe_type(entry) < 0)
			die(_("unable to get type of object %s"),
				oid_to_hex(&entry->idx.oid));
	} അല്ലെങ്കിൽ എങ്കിൽ (oe_type(entry) < 0) {
		/*
		 * This object is not found, but we
		 * don't have to include it anyway.
		 */
		തിരികെ 0;
	}

	തിരികെ 1;
}

സ്ഥിതം ശൂന്യം find_deltas_for_region(ഘടന object_entry *list,
				   ഘടന packing_region *region,
				   unsigned പൂർണ്ണസംഖ്യ *processed)
{
	ഘടന object_entry **delta_list;
	unsigned പൂർണ്ണസംഖ്യ delta_list_nr = 0;

	ALLOC_ARRAY(delta_list, region->nr);
	വേണ്ടി (size_t i = 0; i < region->nr; i++) {
		ഘടന object_entry *entry = list + region->start + i;
		എങ്കിൽ (should_attempt_deltas(entry))
			delta_list[delta_list_nr++] = entry;
	}

	QSORT(delta_list, delta_list_nr, type_size_sort);
	find_deltas(delta_list, &delta_list_nr, window, depth, processed);
	സ്വതന്ത്രം(delta_list);
}

സ്ഥിതം ശൂന്യം find_deltas_by_region(ഘടന object_entry *list,
				  ഘടന packing_region *regions,
				  size_t start, size_t nr)
{
	unsigned പൂർണ്ണസംഖ്യ processed = 0;
	size_t progress_nr;

	എങ്കിൽ (!nr)
		തിരികെ;

	progress_nr = regions[nr - 1].start + regions[nr - 1].nr;

	എങ്കിൽ (progress)
		progress_state = start_progress(the_repository,
						_("Compressing objects by path"),
						progress_nr);

	ആകുമ്പോൾ (nr--)
		find_deltas_for_region(list,
				       &regions[start++],
				       &processed);

	display_progress(progress_state, progress_nr);
	stop_progress(&progress_state);
}

സ്ഥിതം ശൂന്യം *threaded_find_deltas_by_path(ശൂന്യം *arg)
{
	ഘടന thread_params *me = arg;

	progress_lock();
	ആകുമ്പോൾ (me->remaining) {
		ആകുമ്പോൾ (me->remaining) {
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
		ആകുമ്പോൾ (!me->data_ready)
			pthread_cond_wait(&me->cond, &me->mutex);
		me->data_ready = 0;
		pthread_mutex_unlock(&me->mutex);

		progress_lock();
	}
	progress_unlock();
	/* leave ->working 1 so that this doesn't get more work assigned */
	തിരികെ ശൂന്യ;
}

സ്ഥിതം ശൂന്യം ll_find_deltas_by_region(ഘടന object_entry *list,
				     ഘടന packing_region *regions,
				     uint32_t start, uint32_t nr)
{
	ഘടന thread_params *p;
	പൂർണ്ണസംഖ്യ i, ret, active_threads = 0;
	unsigned പൂർണ്ണസംഖ്യ processed = 0;
	uint32_t progress_nr;
	init_threaded_തിരയൽ();

	എങ്കിൽ (!nr)
		തിരികെ;

	progress_nr =  regions[nr - 1].start + regions[nr - 1].nr;
	എങ്കിൽ (delta_തിരയൽ_threads <= 1) {
		find_deltas_by_region(list, regions, start, nr);
		cleanup_threaded_തിരയൽ();
		തിരികെ;
	}

	എങ്കിൽ (progress > pack_to_stdout)
		fprintf_ln(stderr,
			   Q_("Path-based delta compression using up to %d thread",
			      "Path-based delta compression using up to %d threads",
			      delta_തിരയൽ_threads),
			   delta_തിരയൽ_threads);
	CALLOC_ARRAY(p, delta_തിരയൽ_threads);

	എങ്കിൽ (progress)
		progress_state = start_progress(the_repository,
						_("Compressing objects by path"),
						progress_nr);
	/* Partition the work amongst work threads. */
	വേണ്ടി (i = 0; i < delta_തിരയൽ_threads; i++) {
		unsigned sub_size = nr / (delta_തിരയൽ_threads - i);

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
	വേണ്ടി (i = 0; i < delta_തിരയൽ_threads; i++) {
		എങ്കിൽ (!p[i].list_size)
			തുടരുക;
		pthread_mutex_init(&p[i].mutex, ശൂന്യ);
		pthread_cond_init(&p[i].cond, ശൂന്യ);
		ret = pthread_create(&p[i].thread, ശൂന്യ,
				     threaded_find_deltas_by_path, &p[i]);
		എങ്കിൽ (ret)
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
	ആകുമ്പോൾ (active_threads) {
		ഘടന thread_params *target = ശൂന്യ;
		ഘടന thread_params *victim = ശൂന്യ;
		unsigned sub_size = 0;

		progress_lock();
		വേണ്ടി (;;) {
			വേണ്ടി (i = 0; !target && i < delta_തിരയൽ_threads; i++)
				എങ്കിൽ (!p[i].working)
					target = &p[i];
			എങ്കിൽ (target)
				നിർത്തുക;
			pthread_cond_wait(&progress_cond, &progress_mutex);
		}

		വേണ്ടി (i = 0; i < delta_തിരയൽ_threads; i++)
			എങ്കിൽ (p[i].remaining > 2*window &&
			    (!victim || victim->remaining < p[i].remaining))
				victim = &p[i];
		എങ്കിൽ (victim) {
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

		എങ്കിൽ (!sub_size) {
			pthread_join(target->thread, ശൂന്യ);
			pthread_cond_destroy(&target->cond);
			pthread_mutex_destroy(&target->mutex);
			active_threads--;
		}
	}
	cleanup_threaded_തിരയൽ();
	സ്വതന്ത്രം(p);

	display_progress(progress_state, progress_nr);
	stop_progress(&progress_state);
}

സ്ഥിതം ശൂന്യം prepare_pack(പൂർണ്ണസംഖ്യ window, പൂർണ്ണസംഖ്യ depth)
{
	ഘടന object_entry **delta_list;
	uint32_t i, nr_deltas;
	unsigned n;

	എങ്കിൽ (use_delta_islands)
		resolve_വൃക്ഷം_islands(the_repository, progress, &to_pack);

	get_object_details();

	/*
	 * If we're locally repacking then we need to be doubly careful
	 * from now on in order to make sure no stealth corruption gets
	 * propagated to the new pack.  Clients receiving streamed packs
	 * should validate everything they get anyway so no need to incur
	 * the additional cost here in that case.
	 */
	എങ്കിൽ (!pack_to_stdout)
		do_check_packed_object_crc = 1;

	എങ്കിൽ (!to_pack.nr_objects || !window || !depth)
		തിരികെ;

	എങ്കിൽ (path_walk)
		ll_find_deltas_by_region(to_pack.objects, to_pack.regions,
					 0, to_pack.nr_regions);

	ALLOC_ARRAY(delta_list, to_pack.nr_objects);
	nr_deltas = n = 0;

	വേണ്ടി (i = 0; i < to_pack.nr_objects; i++) {
		ഘടന object_entry *entry = to_pack.objects + i;

		എങ്കിൽ (!should_attempt_deltas(entry))
			തുടരുക;

		എങ്കിൽ (!entry->preferred_base)
			nr_deltas++;

		delta_list[n++] = entry;
	}

	എങ്കിൽ (nr_deltas && n > 1) {
		unsigned nr_done = 0;

		എങ്കിൽ (progress)
			progress_state = start_progress(the_repository,
							_("Compressing objects"),
							nr_deltas);
		QSORT(delta_list, n, type_size_sort);
		ll_find_deltas(delta_list, n, window+1, depth, &nr_done);
		stop_progress(&progress_state);
		എങ്കിൽ (nr_done != nr_deltas)
			die(_("inconsistency with delta count"));
	}
	സ്വതന്ത്രം(delta_list);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ git_pack_config(സ്ഥിരം അക്ഷരം *k, സ്ഥിരം അക്ഷരം *v,
			   സ്ഥിരം ഘടന config_context *ctx, ശൂന്യം *cb)
{
	എങ്കിൽ (!strcmp(k, "pack.window")) {
		window = git_config_int(k, v, ctx->kvi);
		തിരികെ 0;
	}
	എങ്കിൽ (!strcmp(k, "pack.windowmemory")) {
		window_memory_limit = git_config_ulong(k, v, ctx->kvi);
		തിരികെ 0;
	}
	എങ്കിൽ (!strcmp(k, "pack.depth")) {
		depth = git_config_int(k, v, ctx->kvi);
		തിരികെ 0;
	}
	എങ്കിൽ (!strcmp(k, "pack.deltacachesize")) {
		max_delta_cache_size = git_config_size_t(k, v, ctx->kvi);
		തിരികെ 0;
	}
	എങ്കിൽ (!strcmp(k, "pack.deltacachelimit")) {
		cache_max_small_delta_size = git_config_int(k, v, ctx->kvi);
		തിരികെ 0;
	}
	എങ്കിൽ (!strcmp(k, "pack.writebitmaphashcache")) {
		എങ്കിൽ (git_config_bool(k, v))
			write_bitmap_options |= BITMAP_OPT_HASH_CACHE;
		അല്ലെങ്കിൽ
			write_bitmap_options &= ~BITMAP_OPT_HASH_CACHE;
	}

	എങ്കിൽ (!strcmp(k, "pack.writebitmaplookuptable")) {
		എങ്കിൽ (git_config_bool(k, v))
			write_bitmap_options |= BITMAP_OPT_LOOKUP_TABLE;
		അല്ലെങ്കിൽ
			write_bitmap_options &= ~BITMAP_OPT_LOOKUP_TABLE;
	}

	എങ്കിൽ (!strcmp(k, "pack.usebitmaps")) {
		use_bitmap_index_default = git_config_bool(k, v);
		തിരികെ 0;
	}
	എങ്കിൽ (!strcmp(k, "pack.allowpackreuse")) {
		പൂർണ്ണസംഖ്യ res = git_parse_maybe_bool_text(v);
		എങ്കിൽ (res < 0) {
			എങ്കിൽ (!strcasecmp(v, "single"))
				allow_pack_reuse = SINGLE_PACK_REUSE;
			അല്ലെങ്കിൽ എങ്കിൽ (!strcasecmp(v, "multi"))
				allow_pack_reuse = MULTI_PACK_REUSE;
			അല്ലെങ്കിൽ
				die(_("invalid pack.allowPackReuse value: '%s'"), v);
		} അല്ലെങ്കിൽ എങ്കിൽ (res) {
			allow_pack_reuse = SINGLE_PACK_REUSE;
		} അല്ലെങ്കിൽ {
			allow_pack_reuse = NO_PACK_REUSE;
		}
		തിരികെ 0;
	}
	എങ്കിൽ (!strcmp(k, "pack.threads")) {
		delta_തിരയൽ_threads = git_config_int(k, v, ctx->kvi);
		എങ്കിൽ (delta_തിരയൽ_threads < 0)
			die(_("invalid number of threads specified (%d)"),
			    delta_തിരയൽ_threads);
		എങ്കിൽ (!HAVE_THREADS && delta_തിരയൽ_threads != 1) {
			warning(_("no threads support, ignoring %s"), k);
			delta_തിരയൽ_threads = 0;
		}
		തിരികെ 0;
	}
	എങ്കിൽ (!strcmp(k, "pack.indexversion")) {
		pack_idx_opts.version = git_config_int(k, v, ctx->kvi);
		എങ്കിൽ (pack_idx_opts.version > 2)
			die(_("bad pack.indexVersion=%"PRIu32),
			    pack_idx_opts.version);
		തിരികെ 0;
	}
	എങ്കിൽ (!strcmp(k, "pack.writereverseindex")) {
		എങ്കിൽ (git_config_bool(k, v))
			pack_idx_opts.flags |= WRITE_REV;
		അല്ലെങ്കിൽ
			pack_idx_opts.flags &= ~WRITE_REV;
		തിരികെ 0;
	}
	എങ്കിൽ (!strcmp(k, "uploadpack.blobpackfileuri")) {
		ഘടന configured_exclusion *ex;
		സ്ഥിരം അക്ഷരം *oid_end, *pack_end;
		/*
		 * Stores the pack hash. This is not a true object ID, but is
		 * of the same form.
		 */
		ഘടന object_id pack_hash;

		എങ്കിൽ (!v)
			തിരികെ config_error_nonbool(k);

		ex = xmalloc(വലുപ്പം(*ex));
		എങ്കിൽ (parse_oid_hex(v, &ex->e.oid, &oid_end) ||
		    *oid_end != ' ' ||
		    parse_oid_hex(oid_end + 1, &pack_hash, &pack_end) ||
		    *pack_end != ' ')
			die(_("value of uploadpack.blobpackfileuri must be "
			      "of the form '<object-hash> <pack-hash> <uri>' (got '%s')"), v);
		എങ്കിൽ (oidmap_get(&configured_exclusions, &ex->e.oid))
			die(_("object already configured in another "
			      "uploadpack.blobpackfileuri (got '%s')"), v);
		ex->pack_hash_hex = xcalloc(1, pack_end - oid_end);
		memcpy(ex->pack_hash_hex, oid_end + 1, pack_end - oid_end - 1);
		ex->uri = xstrdup(pack_end + 1);
		oidmap_put(&configured_exclusions, ex);
	}
	തിരികെ git_default_config(k, v, ctx, cb);
}

/* Counters for trace2 output when in --stdin-packs mode. */
സ്ഥിതം പൂർണ്ണസംഖ്യ stdin_packs_found_nr;
സ്ഥിതം പൂർണ്ണസംഖ്യ stdin_packs_hints_nr;

സ്ഥിതം പൂർണ്ണസംഖ്യ add_object_entry_from_pack(സ്ഥിരം ഘടന object_id *oid,
				      ഘടന packed_git *p,
				      uint32_t pos,
				      ശൂന്യം *_data)
{
	off_t ofs;
	ഘടന object_info oi = OBJECT_INFO_INIT;
	എണ്ണൽ object_type type = OBJ_NONE;

	display_progress(progress_state, ++nr_seen);

	എങ്കിൽ (have_duplicate_entry(oid, 0))
		തിരികെ 0;

	stdin_packs_found_nr++;

	ofs = nth_packed_object_offset(p, pos);

	oi.typep = &type;
	എങ്കിൽ (packed_object_info(ശൂന്യ, p, ofs, &oi) < 0) {
		die(_("could not get type of object %s in pack %s"),
		    oid_to_hex(oid), p->pack_name);
	} അല്ലെങ്കിൽ എങ്കിൽ (type == OBJ_COMMIT) {
		ഘടന rev_info *revs = _data;
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
		add_pending_oid(revs, ശൂന്യ, oid, 0);
	}

	എങ്കിൽ (!want_object_in_pack(oid, 0, &p, &ofs))
		തിരികെ 0;

	create_object_entry(oid, type, 0, 0, 0, p, ofs);

	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം show_object_pack_hint(ഘടന object *object, സ്ഥിരം അക്ഷരം *name,
				  ശൂന്യം *data)
{
	എണ്ണൽ stdin_packs_mode mode = *(എണ്ണൽ stdin_packs_mode *)data;
	എങ്കിൽ (mode == STDIN_PACKS_MODE_FOLLOW) {
		എങ്കിൽ (object->type == OBJ_BLOB &&
		    !odb_has_object(the_repository->objects, &object->oid, 0))
			തിരികെ;
		add_object_entry(&object->oid, object->type, name, 0);
	} അല്ലെങ്കിൽ {
		ഘടന object_entry *oe = packlist_find(&to_pack, &object->oid);
		എങ്കിൽ (!oe)
			തിരികെ;

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

സ്ഥിതം ശൂന്യം show_commit_pack_hint(ഘടന commit *commit, ശൂന്യം *data)
{
	എണ്ണൽ stdin_packs_mode mode = *(എണ്ണൽ stdin_packs_mode *)data;

	എങ്കിൽ (mode == STDIN_PACKS_MODE_FOLLOW) {
		show_object_pack_hint((ഘടന object *)commit, "", data);
		തിരികെ;
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
എണ്ണൽ stdin_pack_info_kind {
	STDIN_PACK_INCLUDE = (1<<0),
	STDIN_PACK_EXCLUDE_CLOSED = (1<<1),
	STDIN_PACK_EXCLUDE_OPEN = (1<<2),
};

ഘടന stdin_pack_info {
	ഘടന packed_git *p;
	എണ്ണൽ stdin_pack_info_kind kind;
};

സ്ഥിതം പൂർണ്ണസംഖ്യ pack_mtime_cmp(സ്ഥിരം ശൂന്യം *_a, സ്ഥിരം ശൂന്യം *_b)
{
	ഘടന stdin_pack_info *a = ((സ്ഥിരം ഘടന string_list_item*)_a)->util;
	ഘടന stdin_pack_info *b = ((സ്ഥിരം ഘടന string_list_item*)_b)->util;

	/*
	 * order packs by descending mtime so that objects are laid out
	 * roughly as newest-to-oldest
	 */
	എങ്കിൽ (a->p->mtime < b->p->mtime)
		തിരികെ 1;
	അല്ലെങ്കിൽ എങ്കിൽ (b->p->mtime < a->p->mtime)
		തിരികെ -1;
	അല്ലെങ്കിൽ
		തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ stdin_packs_include_check_obj(ഘടന object *obj, ശൂന്യം *data UNUSED)
{
	തിരികെ !has_object_kept_pack(to_pack.repo, &obj->oid,
				     KEPT_PACK_IN_CORE);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ stdin_packs_include_check(ഘടന commit *commit, ശൂന്യം *data)
{
	തിരികെ stdin_packs_include_check_obj((ഘടന object *)commit, data);
}

സ്ഥിതം ശൂന്യം stdin_packs_add_pack_entries(ഘടന strmap *packs,
					 ഘടന rev_info *revs)
{
	ഘടന string_list താക്കോലുകൾ = STRING_LIST_INIT_NODUP;
	ഘടന string_list_item *item;
	ഘടന hashmap_iter iter;
	ഘടന strmap_entry *entry;

	strmap_for_each_entry(packs, &iter, entry) {
		ഘടന stdin_pack_info *info = entry->മൂല്യം;
		എങ്കിൽ (!info->p)
			die(_("could not find pack '%s'"), entry->താക്കോൽ);

		string_list_append(&താക്കോലുകൾ, entry->താക്കോൽ)->util = info;
	}

	/*
	 * Order packs by ascending mtime; use QSORT directly to access the
	 * string_list_item's ->util pointer, which string_list_sort() does not
	 * provide.
	 */
	QSORT(താക്കോലുകൾ.ഇനങ്ങൾ, താക്കോലുകൾ.nr, pack_mtime_cmp);

	for_each_string_list_item(item, &താക്കോലുകൾ) {
		ഘടന stdin_pack_info *info = item->util;

		എങ്കിൽ (info->kind & STDIN_PACK_EXCLUDE_OPEN) {
			/*
			 * When open-excluded packs ("!") are present, stop
			 * the parent walk at closed-excluded ("^") packs.
			 * Objects behind a "^" boundary are guaranteed to
			 * have closure and should not be rescued.
			 */
			revs->include_check = stdin_packs_include_check;
			revs->include_check_obj = stdin_packs_include_check_obj;
		}

		എങ്കിൽ ((info->kind & STDIN_PACK_INCLUDE) ||
		    (info->kind & STDIN_PACK_EXCLUDE_OPEN))
			for_each_object_in_pack(info->p,
						add_object_entry_from_pack,
						revs,
						ODB_FOR_EACH_OBJECT_PACK_ORDER);
	}

	string_list_clear(&താക്കോലുകൾ, 0);
}

സ്ഥിതം ശൂന്യം stdin_packs_read_നിവേശം(ഘടന rev_info *revs,
				   എണ്ണൽ stdin_packs_mode mode)
{
	ഘടന strbuf buf = STRBUF_INIT;
	ഘടന strmap packs = STRMAP_INIT;
	ഘടന packed_git *p;

	ആകുമ്പോൾ (strbuf_getline(&buf, stdin) != EOF) {
		ഘടന stdin_pack_info *info;
		എണ്ണൽ stdin_pack_info_kind kind = STDIN_PACK_INCLUDE;
		സ്ഥിരം അക്ഷരം *താക്കോൽ = buf.buf;

		എങ്കിൽ (!*താക്കോൽ)
			തുടരുക;
		അല്ലെങ്കിൽ എങ്കിൽ (*താക്കോൽ == '^')
			kind = STDIN_PACK_EXCLUDE_CLOSED;
		അല്ലെങ്കിൽ എങ്കിൽ (*താക്കോൽ == '!' && mode == STDIN_PACKS_MODE_FOLLOW)
			kind = STDIN_PACK_EXCLUDE_OPEN;

		എങ്കിൽ (kind != STDIN_PACK_INCLUDE)
			താക്കോൽ++;

		info = strmap_get(&packs, താക്കോൽ);
		എങ്കിൽ (!info) {
			CALLOC_ARRAY(info, 1);
			strmap_put(&packs, താക്കോൽ, info);
		}

		info->kind |= kind;

		strbuf_reset(&buf);
	}

	repo_for_each_pack(the_repository, p) {
		ഘടന stdin_pack_info *info;

		info = strmap_get(&packs, pack_basename(p));
		എങ്കിൽ (!info)
			തുടരുക;

		എങ്കിൽ (info->kind & STDIN_PACK_INCLUDE) {
			എങ്കിൽ (exclude_promisor_objects && p->pack_promisor)
				die(_("packfile %s is a promisor but --exclude-promisor-objects was given"), p->pack_name);

			/*
			 * Arguments we got on stdin may not even be
			 * packs. First check that to avoid segfaulting
			 * later on in e.g.  pack_mtime_cmp(), excluded
			 * packs are handled below.
			 */
			എങ്കിൽ (!is_pack_valid(p))
				die(_("packfile %s cannot be accessed"), p->pack_name);
		}

		എങ്കിൽ (info->kind & STDIN_PACK_EXCLUDE_CLOSED) {
			/*
			 * Marking excluded packs as kept in-core so
			 * that later calls to add_object_entry()
			 * discards any objects that are also found in
			 * excluded packs.
			 */
			p->pack_keep_in_core = 1;
		}

		എങ്കിൽ (info->kind & STDIN_PACK_EXCLUDE_OPEN) {
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

സ്ഥിതം ശൂന്യം add_unreachable_loose_objects(ഘടന rev_info *revs);

സ്ഥിതം ശൂന്യം read_stdin_packs(ഘടന repository *repo,
			     എണ്ണൽ stdin_packs_mode mode, പൂർണ്ണസംഖ്യ rev_list_unpacked)
{
	പൂർണ്ണസംഖ്യ prev_fetch_if_missing = repo->fetch_if_missing;
	ഘടന rev_info revs;

	/*
	 * The revision walk may hit objects that are promised, only. As the
	 * walk is best-effort though we don't want to perform backfill fetches
	 * for them.
	 */
	repo->fetch_if_missing = 0;

	repo_init_revisions(repo, &revs, ശൂന്യ);
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
	revs.വൃക്ഷം_objects = 1;
	revs.tag_objects = 1;
	revs.ignore_missing_links = 1;
	revs.exclude_promisor_objects = exclude_promisor_objects;

	/* avoids adding objects in excluded packs */
	ignore_packed_keep_in_core = 1;
	എങ്കിൽ (mode == STDIN_PACKS_MODE_FOLLOW) {
		/*
		 * In '--stdin-packs=follow' mode, additionally ignore
		 * objects in excluded-open packs to prevent them from
		 * appearing in the resulting pack.
		 */
		ignore_packed_keep_in_core_open = 1;
	}
	stdin_packs_read_നിവേശം(&revs, mode);
	എങ്കിൽ (rev_list_unpacked)
		add_unreachable_loose_objects(&revs);

	എങ്കിൽ (prepare_revision_walk(&revs))
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

സ്ഥിതം ശൂന്യം add_cruft_object_entry(സ്ഥിരം ഘടന object_id *oid, എണ്ണൽ object_type type,
				   ഘടന packed_git *pack, off_t offset,
				   സ്ഥിരം അക്ഷരം *name, uint32_t mtime)
{
	ഘടന object_entry *entry;

	display_progress(progress_state, ++nr_seen);

	entry = packlist_find(&to_pack, oid);
	എങ്കിൽ (entry) {
		എങ്കിൽ (name) {
			entry->hash = pack_name_hash_fn(name);
			entry->no_try_delta = no_try_delta(name);
		}
	} അല്ലെങ്കിൽ {
		എങ്കിൽ (!want_object_in_pack_mtime(oid, 0, &pack, &offset, mtime))
			തിരികെ;
		എങ്കിൽ (!pack && type == OBJ_BLOB) {
			ഘടന odb_source_files *files =
				odb_source_files_downcast(the_repository->objects->source);
			പൂർണ്ണസംഖ്യ found = 0;

			വേണ്ടി (ഘടന odb_files_dir *dir = files->dirs; !found && dir; dir = dir->next)
				എങ്കിൽ (!odb_source_read_object_info(&dir->loose->base, oid, ശൂന്യ, 0, ശൂന്യ))
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
			എങ്കിൽ (!found)
				തിരികെ;
		}

		entry = create_object_entry(oid, type, pack_name_hash_fn(name),
					    0, name && no_try_delta(name),
					    pack, offset);
	}

	എങ്കിൽ (mtime > oe_cruft_mtime(&to_pack, entry))
		oe_set_cruft_mtime(&to_pack, entry, mtime);
	തിരികെ;
}

സ്ഥിതം ശൂന്യം show_cruft_object(ഘടന object *obj, സ്ഥിരം അക്ഷരം *name, ശൂന്യം *data UNUSED)
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
	add_cruft_object_entry(&obj->oid, obj->type, ശൂന്യ, 0, name, cruft_expiration);
}

സ്ഥിതം ശൂന്യം show_cruft_commit(ഘടന commit *commit, ശൂന്യം *data)
{
	show_cruft_object((ഘടന object*)commit, ശൂന്യ, data);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ cruft_include_check_obj(ഘടന object *obj, ശൂന്യം *data UNUSED)
{
	തിരികെ !has_object_kept_pack(to_pack.repo, &obj->oid, KEPT_PACK_IN_CORE);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ cruft_include_check(ഘടന commit *commit, ശൂന്യം *data)
{
	തിരികെ cruft_include_check_obj((ഘടന object*)commit, data);
}

സ്ഥിതം ശൂന്യം set_cruft_mtime(സ്ഥിരം ഘടന object *object,
			    ഘടന packed_git *pack,
			    off_t offset, time_t mtime)
{
	add_cruft_object_entry(&object->oid, object->type, pack, offset, ശൂന്യ,
			       mtime);
}

സ്ഥിതം ശൂന്യം mark_pack_kept_in_core(ഘടന string_list *packs, unsigned keep)
{
	ഘടന string_list_item *item = ശൂന്യ;
	for_each_string_list_item(item, packs) {
		ഘടന packed_git *p = item->util;
		എങ്കിൽ (!p)
			die(_("could not find pack '%s'"), item->string);
		എങ്കിൽ (p->is_cruft && keep)
			ignore_packed_keep_in_core_has_cruft = 1;
		p->pack_keep_in_core = keep;
	}
}

സ്ഥിതം ശൂന്യം add_objects_in_unpacked_packs(ശൂന്യം);

സ്ഥിതം ശൂന്യം enumerate_cruft_objects(ശൂന്യം)
{
	എങ്കിൽ (progress)
		progress_state = start_progress(the_repository,
						_("Enumerating cruft objects"), 0);

	add_objects_in_unpacked_packs();
	add_unreachable_loose_objects(ശൂന്യ);

	stop_progress(&progress_state);
}

സ്ഥിതം ശൂന്യം enumerate_and_traverse_cruft_objects(ഘടന string_list *fresh_packs)
{
	ഘടന packed_git *p;
	ഘടന rev_info revs;
	പൂർണ്ണസംഖ്യ ret;

	repo_init_revisions(the_repository, &revs, ശൂന്യ);

	revs.tag_objects = 1;
	revs.വൃക്ഷം_objects = 1;
	revs.blob_objects = 1;

	revs.include_check = cruft_include_check;
	revs.include_check_obj = cruft_include_check_obj;

	revs.ignore_missing_links = 1;

	എങ്കിൽ (progress)
		progress_state = start_progress(the_repository,
						_("Enumerating cruft objects"), 0);
	ret = add_unseen_recent_objects_to_traversal(&revs, cruft_expiration,
						     set_cruft_mtime, 1);
	stop_progress(&progress_state);

	എങ്കിൽ (ret)
		die(_("unable to add cruft objects"));

	/*
	 * Re-mark only the fresh packs as kept so that objects in
	 * unknown packs do not halt the reachability traversal early.
	 */
	repo_for_each_pack(the_repository, p)
		p->pack_keep_in_core = 0;
	mark_pack_kept_in_core(fresh_packs, 1);

	എങ്കിൽ (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	എങ്കിൽ (progress)
		progress_state = start_progress(the_repository,
						_("Traversing cruft objects"), 0);
	nr_seen = 0;
	traverse_commit_list(&revs, show_cruft_commit, show_cruft_object, ശൂന്യ);

	stop_progress(&progress_state);
	release_revisions(&revs);
}

സ്ഥിതം ശൂന്യം read_cruft_objects(ശൂന്യം)
{
	ഘടന strbuf buf = STRBUF_INIT;
	ഘടന string_list discard_packs = STRING_LIST_INIT_DUP;
	ഘടന string_list fresh_packs = STRING_LIST_INIT_DUP;
	ഘടന packed_git *p;

	ignore_packed_keep_in_core = 1;

	ആകുമ്പോൾ (strbuf_getline(&buf, stdin) != EOF) {
		എങ്കിൽ (!buf.len)
			തുടരുക;

		എങ്കിൽ (*buf.buf == '-')
			string_list_append(&discard_packs, buf.buf + 1);
		അല്ലെങ്കിൽ
			string_list_append(&fresh_packs, buf.buf);
	}

	string_list_sort(&discard_packs);
	string_list_sort(&fresh_packs);

	repo_for_each_pack(the_repository, p) {
		സ്ഥിരം അക്ഷരം *pack_name = pack_basename(p);
		ഘടന string_list_item *item;

		item = string_list_lookup(&fresh_packs, pack_name);
		എങ്കിൽ (!item)
			item = string_list_lookup(&discard_packs, pack_name);

		എങ്കിൽ (item) {
			item->util = p;
		} അല്ലെങ്കിൽ {
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

	എങ്കിൽ (cruft_expiration)
		enumerate_and_traverse_cruft_objects(&fresh_packs);
	അല്ലെങ്കിൽ
		enumerate_cruft_objects();

	strbuf_release(&buf);
	string_list_clear(&discard_packs, 0);
	string_list_clear(&fresh_packs, 0);
}

സ്ഥിതം ശൂന്യം read_object_list_from_stdin(ശൂന്യം)
{
	അക്ഷരം വരി[GIT_MAX_HEXSZ + 1 + PATH_MAX + 2];
	ഘടന object_id oid;
	സ്ഥിരം അക്ഷരം *p;

	വേണ്ടി (;;) {
		എങ്കിൽ (!fgets(വരി, വലുപ്പം(വരി), stdin)) {
			എങ്കിൽ (feof(stdin))
				നിർത്തുക;
			എങ്കിൽ (!ferror(stdin))
				BUG("fgets returned NULL, not EOF, not error!");
			എങ്കിൽ (errno != EINTR)
				die_errno("fgets");
			clearerr(stdin);
			തുടരുക;
		}
		എങ്കിൽ (വരി[0] == '-') {
			എങ്കിൽ (get_oid_hex(വരി+1, &oid))
				die(_("expected edge object ID, got garbage:\n %s"),
				    വരി);
			add_preferred_base(&oid);
			തുടരുക;
		}
		എങ്കിൽ (parse_oid_hex(വരി, &oid, &p))
			die(_("expected object ID, got garbage:\n %s"), വരി);

		add_preferred_base_object(p + 1);
		add_object_entry(&oid, OBJ_NONE, p + 1, 0);
	}
}

സ്ഥിതം ശൂന്യം show_commit(ഘടന commit *commit, ശൂന്യം *data UNUSED)
{
	add_object_entry(&commit->object.oid, OBJ_COMMIT, ശൂന്യ, 0);

	എങ്കിൽ (write_bitmap_index)
		index_commit_for_bitmap(commit);

	എങ്കിൽ (use_delta_islands)
		propagate_island_marks(the_repository, commit);
}

സ്ഥിതം ശൂന്യം show_object(ഘടന object *obj, സ്ഥിരം അക്ഷരം *name,
			ശൂന്യം *data UNUSED)
{
	add_preferred_base_object(name);
	add_object_entry(&obj->oid, obj->type, name, 0);

	എങ്കിൽ (use_delta_islands)
		record_വൃക്ഷം_depth(&obj->oid, name);
}

സ്ഥിതം ശൂന്യം show_object__ma_allow_any(ഘടന object *obj, സ്ഥിരം അക്ഷരം *name, ശൂന്യം *data)
{
	പ്രതിജ്ഞാ(arg_missing_action == MA_ALLOW_ANY);

	/*
	 * Quietly ignore ALL missing objects.  This avoids problems with
	 * staging them now and getting an odd error later.
	 */
	എങ്കിൽ (!odb_has_object(the_repository->objects, &obj->oid, 0))
		തിരികെ;

	show_object(obj, name, data);
}

സ്ഥിതം ശൂന്യം show_object__ma_allow_promisor(ഘടന object *obj, സ്ഥിരം അക്ഷരം *name, ശൂന്യം *data)
{
	പ്രതിജ്ഞാ(arg_missing_action == MA_ALLOW_PROMISOR);

	/*
	 * Quietly ignore EXPECTED missing objects.  This avoids problems with
	 * staging them now and getting an odd error later.
	 */
	എങ്കിൽ (!odb_has_object(the_repository->objects, &obj->oid, 0) &&
	    is_promisor_object(to_pack.repo, &obj->oid))
		തിരികെ;

	show_object(obj, name, data);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ option_parse_missing_action(സ്ഥിരം ഘടന option *opt,
				       സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	ഘടന repository *repo = opt->മൂല്യം;

	പ്രതിജ്ഞാ(arg);
	പ്രതിജ്ഞാ(!unset);

	എങ്കിൽ (!strcmp(arg, "error")) {
		arg_missing_action = MA_ERROR;
		fn_show_object = show_object;
		തിരികെ 0;
	}

	എങ്കിൽ (!strcmp(arg, "allow-any")) {
		arg_missing_action = MA_ALLOW_ANY;
		repo->fetch_if_missing = 0;
		fn_show_object = show_object__ma_allow_any;
		തിരികെ 0;
	}

	എങ്കിൽ (!strcmp(arg, "allow-promisor")) {
		arg_missing_action = MA_ALLOW_PROMISOR;
		repo->fetch_if_missing = 0;
		fn_show_object = show_object__ma_allow_promisor;
		തിരികെ 0;
	}

	die(_("invalid value for '%s': '%s'"), "--missing", arg);
	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം show_edge(ഘടന commit *commit)
{
	add_preferred_base(&commit->object.oid);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ add_object_in_unpacked_pack(സ്ഥിരം ഘടന object_id *oid,
				       ഘടന object_info *oi,
				       ശൂന്യം *data UNUSED)
{
	എങ്കിൽ (cruft) {
		add_cruft_object_entry(oid, OBJ_NONE, oi->source_infop->u.packed.pack,
				       oi->source_infop->u.packed.offset, ശൂന്യ,
				       *oi->mtimep);
	} അല്ലെങ്കിൽ {
		add_object_entry(oid, OBJ_NONE, "", 0);
	}
	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം add_objects_in_unpacked_packs(ശൂന്യം)
{
	ഘടന odb_source_files *files =
		odb_source_files_downcast(to_pack.repo->objects->source);
	time_t mtime;
	ഘടന odb_for_each_object_options opts = {
		.flags = ODB_FOR_EACH_OBJECT_PACK_ORDER |
			 ODB_FOR_EACH_OBJECT_LOCAL_ONLY |
			 ODB_FOR_EACH_OBJECT_SKIP_IN_CORE_KEPT_PACKS |
			 ODB_FOR_EACH_OBJECT_SKIP_ON_DISK_KEPT_PACKS,
	};
	ഘടന odb_source_info source_info;
	ഘടന object_info oi = {
		.mtimep = &mtime,
		.source_infop = &source_info,
	};

	വേണ്ടി (ഘടന odb_files_dir *dir = files->dirs; dir; dir = dir->next) {
		എങ്കിൽ (!dir->local)
			തുടരുക;

		എങ്കിൽ (odb_source_for_each_object(&dir->packed->base, &oi,
					       add_object_in_unpacked_pack, ശൂന്യ, &opts))
			die(_("cannot open pack index"));
	}
}

സ്ഥിതം പൂർണ്ണസംഖ്യ add_loose_object(സ്ഥിരം ഘടന object_id *oid, സ്ഥിരം അക്ഷരം *path,
			    ശൂന്യം *data)
{
	ഘടന rev_info *revs = data;
	എണ്ണൽ object_type type = odb_read_object_info(the_repository->objects, oid, ശൂന്യ);

	എങ്കിൽ (type < 0) {
		warning(_("loose object at %s could not be examined"), path);
		തിരികെ 0;
	}

	എങ്കിൽ (cruft) {
		ഘടന stat st;
		എങ്കിൽ (stat(path, &st) < 0) {
			എങ്കിൽ (errno == ENOENT)
				തിരികെ 0;
			തിരികെ error_errno("unable to stat %s", oid_to_hex(oid));
		}

		add_cruft_object_entry(oid, type, ശൂന്യ, 0, ശൂന്യ,
				       st.st_mtime);
	} അല്ലെങ്കിൽ {
		add_object_entry(oid, type, "", 0);
	}

	എങ്കിൽ (revs && type == OBJ_COMMIT)
		add_pending_oid(revs, ശൂന്യ, oid, 0);

	തിരികെ 0;
}

/*
 * We actually don't even have to worry about reachability here.
 * add_object_entry will weed out duplicates, so we just add every
 * loose object we find.
 */
സ്ഥിതം ശൂന്യം add_unreachable_loose_objects(ഘടന rev_info *revs)
{
	for_each_loose_file_in_source(the_repository->objects->source,
				      add_loose_object, ശൂന്യ, ശൂന്യ, revs);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ has_sha1_pack_kept_or_nonlocal(സ്ഥിരം ഘടന object_id *oid)
{
	സ്ഥിതം ഘടന packed_git *last_found = ശൂന്യ;
	ഘടന packed_git *p;

	എങ്കിൽ (last_found && find_pack_entry_one(oid, last_found))
		തിരികെ 1;

	repo_for_each_pack(the_repository, p) {
		/*
		 * We have already checked `last_found`, so there is no need to
		 * re-check here.
		 */
		എങ്കിൽ (p == last_found)
			തുടരുക;

		എങ്കിൽ ((!p->pack_local || p->pack_keep || p->pack_keep_in_core) &&
		    find_pack_entry_one(oid, p)) {
			last_found = p;
			തിരികെ 1;
		}
	}

	തിരികെ 0;
}

/*
 * Store a list of sha1s that are should not be discarded
 * because they are either written too recently, or are
 * reachable from another object that was.
 *
 * This is filled by get_object_list.
 */
സ്ഥിതം ഘടന oid_array recent_objects;

സ്ഥിതം പൂർണ്ണസംഖ്യ loosened_object_can_be_discarded(സ്ഥിരം ഘടന object_id *oid,
					    timestamp_t mtime)
{
	എങ്കിൽ (!unpack_unreachable_expiration)
		തിരികെ 0;
	എങ്കിൽ (mtime > unpack_unreachable_expiration)
		തിരികെ 0;
	എങ്കിൽ (oid_array_lookup(&recent_objects, oid) >= 0)
		തിരികെ 0;
	തിരികെ 1;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ force_object_loose(ഘടന odb_source *source,
			      സ്ഥിരം ഘടന object_id *oid,
			      സ്ഥിരം time_t *mtime)
{
	ഘടന odb_source_files *files = odb_source_files_downcast(source);
	സ്ഥിരം ഘടന git_hash_algo *compat = source->odb->repo->compat_hash_algo;
	ഘടന object_info oi = OBJECT_INFO_INIT;
	ഘടന object_id compat_oid, *compat_oid_p = ശൂന്യ;
	എണ്ണൽ object_type type;
	ശൂന്യം *buf = ശൂന്യ;
	size_t len;
	പൂർണ്ണസംഖ്യ ret;

	വേണ്ടി (ഘടന odb_files_dir *dir = files->dirs; dir; dir = dir->next)
		എങ്കിൽ (!odb_source_read_object_info(&dir->loose->base, oid, ശൂന്യ, 0, ശൂന്യ))
			തിരികെ 0;

	oi.typep = &type;
	oi.sizep = &len;
	oi.contentp = &buf;
	എങ്കിൽ (odb_read_object_info_extended(source->odb, oid, &oi, 0)) {
		ret = error(_("cannot read object for %s"), oid_to_hex(oid));
		പോകുക out;
	}

	എങ്കിൽ (compat) {
		എങ്കിൽ (repo_oid_to_algop(source->odb->repo, oid, compat, &compat_oid)) {
			ret = error(_("cannot map object %s to %s"),
				    oid_to_hex(oid), compat->name);
			പോകുക out;
		}

		compat_oid_p = &compat_oid;
	}

	ret = odb_source_write_object(&files->dirs->loose->base, buf, len, type, oid,
				      compat_oid_p, mtime, 0);

out:
	സ്വതന്ത്രം(buf);
	തിരികെ ret;
}

സ്ഥിതം ശൂന്യം loosen_unused_packed_objects(ശൂന്യം)
{
	ഘടന packed_git *p;
	uint32_t i;
	uint32_t loosened_objects_nr = 0;
	ഘടന object_id oid;

	repo_for_each_pack(the_repository, p) {
		എങ്കിൽ (!p->pack_local || p->pack_keep || p->pack_keep_in_core)
			തുടരുക;

		എങ്കിൽ (open_pack_index(p))
			die(_("cannot open pack index"));

		വേണ്ടി (i = 0; i < p->അക്കം_objects; i++) {
			nth_packed_object_id(&oid, p, i);
			എങ്കിൽ (!packlist_find(&to_pack, &oid) &&
			    !has_sha1_pack_kept_or_nonlocal(&oid) &&
			    !loosened_object_can_be_discarded(&oid, p->mtime)) {
				എങ്കിൽ (force_object_loose(the_repository->objects->source,
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
സ്ഥിതം പൂർണ്ണസംഖ്യ pack_options_allow_reuse(ശൂന്യം)
{
	തിരികെ allow_pack_reuse != NO_PACK_REUSE &&
	       pack_to_stdout &&
	       !ignore_packed_keep_on_disk &&
	       !ignore_packed_keep_in_core &&
	       (!local || !have_non_local_packs) &&
	       !incremental;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ get_object_list_from_bitmap(ഘടന rev_info *revs)
{
	എങ്കിൽ (!(bitmap_git = prepare_bitmap_walk(revs, 0)))
		തിരികെ -1;

	/*
	 * For now, force the name-hash version to be 1 since that
	 * is the version implied by the bitmap format. Later, the
	 * format can include this version explicitly in its format,
	 * allowing readers to know the version that was used during
	 * the bitmap write.
	 */
	name_hash_version = 1;

	എങ്കിൽ (pack_options_allow_reuse())
		reuse_partial_packfile_from_bitmap(bitmap_git,
						   &reuse_packfiles,
						   &reuse_packfiles_nr,
						   &reuse_packfile_bitmap,
						   allow_pack_reuse == MULTI_PACK_REUSE);

	എങ്കിൽ (reuse_packfiles) {
		reuse_packfile_objects = bitmap_popcount(reuse_packfile_bitmap);
		എങ്കിൽ (!reuse_packfile_objects)
			BUG("expected non-empty reuse bitmap");

		nr_result += reuse_packfile_objects;
		nr_seen += reuse_packfile_objects;
		display_progress(progress_state, nr_seen);
	}

	traverse_bitmap_commit_list(bitmap_git, revs,
				    &add_object_entry_from_bitmap);
	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം record_recent_object(ഘടന object *obj,
				 സ്ഥിരം അക്ഷരം *name UNUSED,
				 ശൂന്യം *data UNUSED)
{
	oid_array_append(&recent_objects, &obj->oid);
}

സ്ഥിതം ശൂന്യം record_recent_commit(ഘടന commit *commit, ശൂന്യം *data UNUSED)
{
	oid_array_append(&recent_objects, &commit->object.oid);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ mark_bitmap_preferred_tip(സ്ഥിരം ഘടന reference *ref, ശൂന്യം *data UNUSED)
{
	സ്ഥിരം ഘടന object_id *maybe_peeled = ref->oid;
	ഘടന object_id peeled;
	ഘടന object *object;

	എങ്കിൽ (!reference_get_peeled_oid(the_repository, ref, &peeled))
		maybe_peeled = &peeled;

	object = parse_object_or_die(the_repository, maybe_peeled, ref->name);
	എങ്കിൽ (object->type == OBJ_COMMIT)
		object->flags |= NEEDS_BITMAP;

	തിരികെ 0;
}

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ is_oid_uninteresting(ഘടന repository *repo,
				       ഘടന object_id *oid)
{
	ഘടന object *o = lookup_object(repo, oid);
	തിരികെ !o || (o->flags & UNINTERESTING);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ add_objects_by_path(സ്ഥിരം അക്ഷരം *path,
			       ഘടന oid_array *oids,
			       എണ്ണൽ object_type type,
			       ശൂന്യം *data)
{
	size_t oe_start = to_pack.nr_objects;
	size_t oe_end;
	unsigned പൂർണ്ണസംഖ്യ *processed = data;

	/*
	 * First, add all objects to the packing data, including the ones
	 * marked UNINTERESTING (translated to 'exclude') as they can be
	 * used as delta bases.
	 */
	വേണ്ടി (size_t i = 0; i < oids->nr; i++) {
		പൂർണ്ണസംഖ്യ exclude;
		ഘടന object_info oi = OBJECT_INFO_INIT;
		ഘടന object_id *oid = &oids->oid[i];

		/* Skip objects that do not exist locally. */
		എങ്കിൽ ((exclude_promisor_objects || arg_missing_action != MA_ERROR) &&
		    odb_read_object_info_extended(the_repository->objects, oid, &oi,
						  OBJECT_INFO_FOR_PREFETCH) < 0)
			തുടരുക;

		exclude = is_oid_uninteresting(the_repository, oid);

		എങ്കിൽ (exclude && !thin)
			തുടരുക;

		add_object_entry(oid, type, path, exclude);

		എങ്കിൽ (type == OBJ_COMMIT) {
			ഘടന commit *commit;

			എങ്കിൽ (!write_bitmap_index && !use_delta_islands)
				തുടരുക;

			commit = lookup_commit(the_repository, oid);
			എങ്കിൽ (!commit)
				die(_("could not find commit %s"), oid_to_hex(oid));
			എങ്കിൽ (write_bitmap_index)
				index_commit_for_bitmap(commit);
			/*
			 * Skip island propagation for boundary commits.
			 * The regular traversal's show_commit() is only
			 * called for interesting commits; matching that
			 * here keeps path-walk from doing extra work that
			 * would only be a no-op anyway (boundary commits
			 * are not in island_marks).
			 */
			എങ്കിൽ (use_delta_islands && !exclude)
				propagate_island_marks(the_repository, commit);
		} അല്ലെങ്കിൽ എങ്കിൽ (type == OBJ_വൃക്ഷം && use_delta_islands) {
			record_വൃക്ഷം_depth(oid, path);
		}
	}

	oe_end = to_pack.nr_objects;

	/* We can skip delta calculations if it is a no-op. */
	എങ്കിൽ (oe_end == oe_start || !window)
		തിരികെ 0;

	ALLOC_GROW(to_pack.regions,
		   to_pack.nr_regions + 1,
		   to_pack.nr_regions_alloc);

	to_pack.regions[to_pack.nr_regions].start = oe_start;
	to_pack.regions[to_pack.nr_regions].nr = oe_end - oe_start;
	to_pack.nr_regions++;

	*processed += oids->nr;
	display_progress(progress_state, *processed);

	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ get_object_list_path_walk(ഘടന rev_info *revs)
{
	ഘടന path_walk_info info = PATH_WALK_INFO_INIT;
	unsigned പൂർണ്ണസംഖ്യ processed = 0;
	പൂർണ്ണസംഖ്യ result;

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

	തിരികെ result;
}

സ്ഥിതം ശൂന്യം get_object_list(ഘടന rev_info *revs, ഘടന strvec *argv)
{
	ഘടന setup_revision_opt s_r_opt = {
		.allow_exclude_promisor_objects = 1,
	};
	ഘടന repo_config_മൂല്യങ്ങൾ *cfg = repo_config_മൂല്യങ്ങൾ(the_repository);
	അക്ഷരം വരി[1000];
	പൂർണ്ണസംഖ്യ flags = 0;
	പൂർണ്ണസംഖ്യ save_warning;

	save_commit_buffer = 0;
	setup_revisions_from_strvec(argv, revs, &s_r_opt);

	/* make sure shallows are read */
	is_repository_shallow(the_repository);

	save_warning = cfg->warn_on_object_refname_ambiguity;
	cfg->warn_on_object_refname_ambiguity = 0;

	ആകുമ്പോൾ (fgets(വരി, വലുപ്പം(വരി), stdin) != ശൂന്യ) {
		പൂർണ്ണസംഖ്യ len = strlen(വരി);
		എങ്കിൽ (len && വരി[len - 1] == '\n')
			വരി[--len] = 0;
		എങ്കിൽ (!len)
			നിർത്തുക;
		എങ്കിൽ (*വരി == '-') {
			എങ്കിൽ (!strcmp(വരി, "--not")) {
				flags ^= UNINTERESTING;
				write_bitmap_index = 0;
				തുടരുക;
			}
			എങ്കിൽ (starts_with(വരി, "--shallow ")) {
				ഘടന object_id oid;
				എങ്കിൽ (get_oid_hex(വരി + 10, &oid))
					die("not an object name '%s'", വരി + 10);
				register_shallow(the_repository, &oid);
				use_bitmap_index = 0;
				തുടരുക;
			}
			die(_("not a rev '%s'"), വരി);
		}
		എങ്കിൽ (handle_revision_arg(വരി, revs, flags, REVARG_CANNOT_BE_FILENAME))
			die(_("bad revision '%s'"), വരി);
	}

	cfg->warn_on_object_refname_ambiguity = save_warning;

	എങ്കിൽ (use_bitmap_index && !get_object_list_from_bitmap(revs))
		തിരികെ;

	എങ്കിൽ (use_delta_islands)
		load_delta_islands(the_repository, progress);

	എങ്കിൽ (write_bitmap_index)
		for_each_preferred_bitmap_tip(the_repository, mark_bitmap_preferred_tip,
					      ശൂന്യ);

	എങ്കിൽ (!fn_show_object)
		fn_show_object = show_object;

	എങ്കിൽ (path_walk) {
		എങ്കിൽ (get_object_list_path_walk(revs)) {
			warning(_("failed to pack objects via path-walk"));
			path_walk = 0;
		}
	}

	എങ്കിൽ (!path_walk) {
		എങ്കിൽ (prepare_revision_walk(revs))
			die(_("revision walk setup failed"));
		mark_edges_uninteresting(revs, show_edge, sparse);
		traverse_commit_list(revs,
				show_commit, fn_show_object,
				ശൂന്യ);
	}

	എങ്കിൽ (unpack_unreachable_expiration) {
		revs->ignore_missing_links = 1;
		എങ്കിൽ (add_unseen_recent_objects_to_traversal(revs,
				unpack_unreachable_expiration, ശൂന്യ, 0))
			die(_("unable to add recent objects"));
		എങ്കിൽ (prepare_revision_walk(revs))
			die(_("revision walk setup failed"));
		traverse_commit_list(revs, record_recent_commit,
				     record_recent_object, ശൂന്യ);
	}

	എങ്കിൽ (keep_unreachable)
		add_objects_in_unpacked_packs();
	എങ്കിൽ (pack_loose_unreachable)
		add_unreachable_loose_objects(ശൂന്യ);
	എങ്കിൽ (unpack_unreachable)
		loosen_unused_packed_objects();

	oid_array_clear(&recent_objects);
}

സ്ഥിതം ശൂന്യം add_extra_kept_packs(സ്ഥിരം ഘടന string_list *names)
{
	ഘടന packed_git *p;

	എങ്കിൽ (!names->nr)
		തിരികെ;

	repo_for_each_pack(the_repository, p) {
		സ്ഥിരം അക്ഷരം *name = basename(p->pack_name);
		പൂർണ്ണസംഖ്യ i;

		എങ്കിൽ (!p->pack_local)
			തുടരുക;

		വേണ്ടി (i = 0; i < names->nr; i++)
			എങ്കിൽ (!fspathcmp(name, names->ഇനങ്ങൾ[i].string))
				നിർത്തുക;

		എങ്കിൽ (i < names->nr) {
			p->pack_keep_in_core = 1;
			ignore_packed_keep_in_core = 1;
			തുടരുക;
		}
	}
}

സ്ഥിതം പൂർണ്ണസംഖ്യ option_parse_quiet(സ്ഥിരം ഘടന option *opt, സ്ഥിരം അക്ഷരം *arg,
			      പൂർണ്ണസംഖ്യ unset)
{
	പൂർണ്ണസംഖ്യ *val = opt->മൂല്യം;

	BUG_ON_OPT_ARG(arg);

	എങ്കിൽ (!unset)
		*val = 0;
	അല്ലെങ്കിൽ എങ്കിൽ (!*val)
		*val = 1;
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ option_parse_index_version(സ്ഥിരം ഘടന option *opt,
				      സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	ഘടന pack_idx_option *popts = opt->മൂല്യം;
	അക്ഷരം *c;
	സ്ഥിരം അക്ഷരം *val = arg;

	BUG_ON_OPT_NEG(unset);

	popts->version = strtoul(val, &c, 10);
	എങ്കിൽ (popts->version > 2)
		die(_("unsupported index version %s"), val);
	എങ്കിൽ (*c == ',' && c[1])
		popts->off32_limit = strtoul(c+1, &c, 0);
	എങ്കിൽ (*c || popts->off32_limit & 0x80000000)
		die(_("bad index version '%s'"), val);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ option_parse_unpack_unreachable(സ്ഥിരം ഘടന option *opt UNUSED,
					   സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	എങ്കിൽ (unset) {
		unpack_unreachable = 0;
		unpack_unreachable_expiration = 0;
	}
	അല്ലെങ്കിൽ {
		unpack_unreachable = 1;
		എങ്കിൽ (arg)
			unpack_unreachable_expiration = approxidate(arg);
	}
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ option_parse_cruft_expiration(സ്ഥിരം ഘടന option *opt UNUSED,
					 സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	എങ്കിൽ (unset) {
		cruft = 0;
		cruft_expiration = 0;
	} അല്ലെങ്കിൽ {
		cruft = 1;
		എങ്കിൽ (arg)
			cruft_expiration = approxidate(arg);
	}
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ is_not_in_promisor_pack_obj(ഘടന object *obj, ശൂന്യം *data UNUSED)
{
	ഘടന odb_source_info source_info;
	ഘടന object_info info = {
		.source_infop = &source_info,
	};

	എങ്കിൽ (odb_read_object_info_extended(the_repository->objects, &obj->oid, &info, 0))
		BUG("should_include_obj should only be called on existing objects");
	തിരികെ source_info.source->type != ODB_SOURCE_PACKED || !source_info.u.packed.pack->pack_promisor;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ is_not_in_promisor_pack(ഘടന commit *commit, ശൂന്യം *data) {
	തിരികെ is_not_in_promisor_pack_obj((ഘടന object *) commit, data);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ parse_stdin_packs_mode(സ്ഥിരം ഘടന option *opt, സ്ഥിരം അക്ഷരം *arg,
				  പൂർണ്ണസംഖ്യ unset)
{
	എണ്ണൽ stdin_packs_mode *mode = opt->മൂല്യം;

	എങ്കിൽ (unset)
		*mode = STDIN_PACKS_MODE_NONE;
	അല്ലെങ്കിൽ എങ്കിൽ (!arg || !*arg)
		*mode = STDIN_PACKS_MODE_STANDARD;
	അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(arg, "follow"))
		*mode = STDIN_PACKS_MODE_FOLLOW;
	അല്ലെങ്കിൽ
		die(_("invalid value for '%s': '%s'"), opt->long_name, arg);

	തിരികെ 0;
}

പൂർണ്ണസംഖ്യ cmd_pack_objects(പൂർണ്ണസംഖ്യ argc,
		     സ്ഥിരം അക്ഷരം **argv,
		     സ്ഥിരം അക്ഷരം *prefix,
		     ഘടന repository *repo)
{
	പൂർണ്ണസംഖ്യ use_internal_rev_list = 0;
	പൂർണ്ണസംഖ്യ all_progress_implied = 0;
	ഘടന strvec rp = STRVEC_INIT;
	പൂർണ്ണസംഖ്യ rev_list_unpacked = 0, rev_list_all = 0, rev_list_reflog = 0;
	പൂർണ്ണസംഖ്യ rev_list_index = 0;
	എണ്ണൽ stdin_packs_mode stdin_packs = STDIN_PACKS_MODE_NONE;
	ഘടന string_list keep_pack_list = STRING_LIST_INIT_NODUP;
	ഘടന list_objects_filter_options filter_options =
		LIST_OBJECTS_FILTER_INIT;
	ഘടന repo_config_മൂല്യങ്ങൾ *cfg = repo_config_മൂല്യങ്ങൾ(the_repository);

	ഘടന option pack_objects_options[] = {
		OPT_CALLBACK_F('q', "quiet", &progress, ശൂന്യ,
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
		OPT_INTEGER(0, "threads", &delta_തിരയൽ_threads,
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
		OPT_CALLBACK_F(0, "unpack-unreachable", ശൂന്യ, N_("time"),
		  N_("unpack unreachable objects newer than <time>"),
		  PARSE_OPT_OPTARG, option_parse_unpack_unreachable),
		OPT_BOOL(0, "cruft", &cruft, N_("create a cruft pack")),
		OPT_CALLBACK_F(0, "cruft-expiration", ശൂന്യ, N_("time"),
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

	എങ്കിൽ (DFS_അക്കം_STATES > (1 << OE_DFS_STATE_BITS))
		BUG("too many dfs states, increase OE_DFS_STATE_BITS");

	disable_replace_refs();

	sparse = git_env_bool("GIT_TEST_PACK_SPARSE", -1);
	എങ്കിൽ (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		എങ്കിൽ (sparse < 0)
			sparse = the_repository->settings.pack_use_sparse;
		എങ്കിൽ (the_repository->settings.pack_use_multi_pack_reuse)
			allow_pack_reuse = MULTI_PACK_REUSE;
	}

	reset_pack_idx_option(&pack_idx_opts);
	pack_idx_opts.flags |= WRITE_REV;
	repo_config(the_repository, git_pack_config, ശൂന്യ);
	എങ്കിൽ (git_env_bool(GIT_TEST_NO_WRITE_REV_INDEX, 0))
		pack_idx_opts.flags &= ~WRITE_REV;

	progress = isatty(2);
	argc = parse_options(argc, argv, prefix, pack_objects_options,
			     pack_usage, 0);

	എങ്കിൽ (argc) {
		base_name = argv[0];
		argc--;
	}
	എങ്കിൽ (pack_to_stdout != !base_name || argc)
		usage_with_options(pack_usage, pack_objects_options);

	എങ്കിൽ (path_walk < 0) {
		എങ്കിൽ (use_bitmap_index > 0 ||
		    !use_internal_rev_list)
			path_walk = 0;
		അല്ലെങ്കിൽ എങ്കിൽ (the_repository->gitdir &&
			 the_repository->settings.pack_use_path_walk)
			path_walk = 1;
		അല്ലെങ്കിൽ
			path_walk = git_env_bool("GIT_TEST_PACK_PATH_WALK", 0);
	}

	എങ്കിൽ (depth < 0)
		depth = 0;
	എങ്കിൽ (depth >= (1 << OE_DEPTH_BITS)) {
		warning(_("delta chain depth %d is too deep, forcing %d"),
			depth, (1 << OE_DEPTH_BITS) - 1);
		depth = (1 << OE_DEPTH_BITS) - 1;
	}
	എങ്കിൽ (cache_max_small_delta_size >= (1U << OE_Z_DELTA_BITS)) {
		warning(_("pack.deltaCacheLimit is too high, forcing %d"),
			(1U << OE_Z_DELTA_BITS) - 1);
		cache_max_small_delta_size = (1U << OE_Z_DELTA_BITS) - 1;
	}
	എങ്കിൽ (window < 0)
		window = 0;

	strvec_തള്ളുക(&rp, "pack-objects");

	എങ്കിൽ (path_walk) {
		സ്ഥിരം അക്ഷരം *option = ശൂന്യ;
		എങ്കിൽ (!path_walk_filter_compatible(&filter_options))
			option = "--filter";

		എങ്കിൽ (option) {
			warning(_("cannot use %s with %s"),
				option, "--path-walk");
			path_walk = 0;
		}
	}
	എങ്കിൽ (path_walk) {
		strvec_തള്ളുക(&rp, "--objects");
	} അല്ലെങ്കിൽ എങ്കിൽ (thin) {
		use_internal_rev_list = 1;
		strvec_തള്ളുക(&rp, shallow
				? "--objects-edge-aggressive"
				: "--objects-edge");
	} അല്ലെങ്കിൽ
		strvec_തള്ളുക(&rp, "--objects");

	എങ്കിൽ (rev_list_all) {
		use_internal_rev_list = 1;
		strvec_തള്ളുക(&rp, "--all");
	}
	എങ്കിൽ (rev_list_reflog) {
		use_internal_rev_list = 1;
		strvec_തള്ളുക(&rp, "--reflog");
	}
	എങ്കിൽ (rev_list_index) {
		use_internal_rev_list = 1;
		strvec_തള്ളുക(&rp, "--indexed-objects");
	}
	എങ്കിൽ (rev_list_unpacked && !stdin_packs) {
		use_internal_rev_list = 1;
		strvec_തള്ളുക(&rp, "--unpacked");
	}

	die_for_incompatible_opt2(exclude_promisor_objects,
				  "--exclude-promisor-objects",
				  exclude_promisor_objects_best_effort,
				  "--exclude-promisor-objects-best-effort");
	എങ്കിൽ (exclude_promisor_objects) {
		repo->fetch_if_missing = 0;

		/* --stdin-packs handles promisor objects separately. */
		എങ്കിൽ (!stdin_packs) {
			use_internal_rev_list = 1;
			strvec_തള്ളുക(&rp, "--exclude-promisor-objects");
		}
	} അല്ലെങ്കിൽ എങ്കിൽ (exclude_promisor_objects_best_effort) {
		use_internal_rev_list = 1;
		arg_missing_action = MA_ALLOW_ANY;
		repo->fetch_if_missing = 0;
		fn_show_object = show_object__ma_allow_any;
		/* revs configured below */
	}
	എങ്കിൽ (unpack_unreachable || keep_unreachable || pack_loose_unreachable)
		use_internal_rev_list = 1;

	എങ്കിൽ (!reuse_object)
		reuse_delta = 0;
	എങ്കിൽ (cfg->pack_compression_level == -1)
		cfg->pack_compression_level = Z_DEFAULT_COMPRESSION;
	അല്ലെങ്കിൽ എങ്കിൽ (cfg->pack_compression_level < 0 || cfg->pack_compression_level > Z_BEST_COMPRESSION)
		die(_("bad pack compression level %d"), cfg->pack_compression_level);

	എങ്കിൽ (!delta_തിരയൽ_threads)	/* --threads=0 means autodetect */
		delta_തിരയൽ_threads = online_cpus();

	എങ്കിൽ (!HAVE_THREADS && delta_തിരയൽ_threads != 1)
		warning(_("no threads support, ignoring --threads"));
	എങ്കിൽ (!pack_to_stdout && !pack_size_limit)
		pack_size_limit = pack_size_limit_cfg;
	എങ്കിൽ (pack_to_stdout && pack_size_limit)
		die(_("--max-pack-size cannot be used to build a pack for transfer"));
	എങ്കിൽ (pack_size_limit && pack_size_limit < 1024*1024) {
		warning(_("minimum pack size limit is 1 MiB"));
		pack_size_limit = 1024*1024;
	}

	എങ്കിൽ (!pack_to_stdout && thin)
		die(_("--thin cannot be used to build an indexable pack"));

	die_for_incompatible_opt2(keep_unreachable, "--keep-unreachable",
				  unpack_unreachable, "--unpack-unreachable");
	എങ്കിൽ (!rev_list_all || !rev_list_reflog || !rev_list_index)
		unpack_unreachable_expiration = 0;

	die_for_incompatible_opt2(stdin_packs, "--stdin-packs",
				  filter_options.choice, "--filter");


	എങ്കിൽ (stdin_packs && use_internal_rev_list)
		die(_("cannot use internal rev list with --stdin-packs"));

	എങ്കിൽ (cruft) {
		എങ്കിൽ (use_internal_rev_list)
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
	എങ്കിൽ (!pack_to_stdout)
		use_bitmap_index_default = 0;

	എങ്കിൽ (use_bitmap_index < 0)
		use_bitmap_index = use_bitmap_index_default;

	/* "hard" reasons not to use bitmaps; these just won't work at all */
	എങ്കിൽ (!use_internal_rev_list || (!pack_to_stdout && write_bitmap_index) || is_repository_shallow(the_repository))
		use_bitmap_index = 0;

	എങ്കിൽ (pack_to_stdout || !rev_list_all)
		write_bitmap_index = 0;

	എങ്കിൽ (name_hash_version < 0)
		name_hash_version = (പൂർണ്ണസംഖ്യ)git_env_ulong("GIT_TEST_NAME_HASH_VERSION", 1);

	validate_name_hash_version();

	എങ്കിൽ (use_delta_islands)
		strvec_തള്ളുക(&rp, "--topo-order");

	എങ്കിൽ (progress && all_progress_implied)
		progress = 2;

	add_extra_kept_packs(&keep_pack_list);
	എങ്കിൽ (ignore_packed_keep_on_disk) {
		ഘടന packed_git *p;

		repo_for_each_pack(the_repository, p)
			എങ്കിൽ (p->pack_local && p->pack_keep)
				നിർത്തുക;
		എങ്കിൽ (!p) /* no keep-able packs found */
			ignore_packed_keep_on_disk = 0;
	}
	എങ്കിൽ (local) {
		/*
		 * unlike ignore_packed_keep_on_disk above, we do not
		 * want to unset "local" based on looking at packs, as
		 * it also covers non-local objects
		 */
		ഘടന packed_git *p;

		repo_for_each_pack(the_repository, p) {
			എങ്കിൽ (!p->pack_local) {
				have_non_local_packs = 1;
				നിർത്തുക;
			}
		}
	}

	trace2_region_enter("pack-objects", "enumerate-objects",
			    the_repository);
	prepare_packing_data(the_repository, &to_pack);

	എങ്കിൽ (progress && !cruft)
		progress_state = start_progress(the_repository,
						_("Enumerating objects"), 0);
	എങ്കിൽ (stdin_packs) {
		read_stdin_packs(repo, stdin_packs, rev_list_unpacked);
	} അല്ലെങ്കിൽ എങ്കിൽ (cruft) {
		read_cruft_objects();
	} അല്ലെങ്കിൽ എങ്കിൽ (!use_internal_rev_list) {
		read_object_list_from_stdin();
	} അല്ലെങ്കിൽ {
		ഘടന rev_info revs;

		repo_init_revisions(the_repository, &revs, ശൂന്യ);
		list_objects_filter_copy(&revs.filter, &filter_options);
		എങ്കിൽ (exclude_promisor_objects_best_effort) {
			revs.include_check = is_not_in_promisor_pack;
			revs.include_check_obj = is_not_in_promisor_pack_obj;
		}
		get_object_list(&revs, &rp);
		release_revisions(&revs);
	}
	cleanup_preferred_base();
	എങ്കിൽ (include_tag && nr_result)
		refs_for_each_tag_ref(get_main_ref_store(the_repository),
				      add_ref_tag, ശൂന്യ);
	stop_progress(&progress_state);
	trace2_region_leave("pack-objects", "enumerate-objects",
			    the_repository);

	എങ്കിൽ (non_empty && !nr_result)
		പോകുക cleanup;
	എങ്കിൽ (nr_result) {
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

	എങ്കിൽ (progress)
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

	തിരികെ 0;
}
