#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "environment.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "hex.h"
#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ "attr.h"
#ଅନ୍ତର୍ଭାଵ "object.h"
#ଅନ୍ତର୍ଭାଵ "commit.h"
#ଅନ୍ତର୍ଭାଵ "tag.h"
#ଅନ୍ତର୍ଭାଵ "delta.h"
#ଅନ୍ତର୍ଭାଵ "pack.h"
#ଅନ୍ତର୍ଭାଵ "pack-revindex.h"
#ଅନ୍ତର୍ଭାଵ "csum-file.h"
#ଅନ୍ତର୍ଭାଵ "tree-walk.h"
#ଅନ୍ତର୍ଭାଵ "diff.h"
#ଅନ୍ତର୍ଭାଵ "revision.h"
#ଅନ୍ତର୍ଭାଵ "list-objects.h"
#ଅନ୍ତର୍ଭାଵ "list-objects-filter-options.h"
#ଅନ୍ତର୍ଭାଵ "pack-objects.h"
#ଅନ୍ତର୍ଭାଵ "progress.h"
#ଅନ୍ତର୍ଭାଵ "refs.h"
#ଅନ୍ତର୍ଭାଵ "thread-utils.h"
#ଅନ୍ତର୍ଭାଵ "pack-bitmap.h"
#ଅନ୍ତର୍ଭାଵ "delta-islands.h"
#ଅନ୍ତର୍ଭାଵ "reachable.h"
#ଅନ୍ତର୍ଭାଵ "oid-array.h"
#ଅନ୍ତର୍ଭାଵ "strvec.h"
#ଅନ୍ତର୍ଭାଵ "strmap.h"
#ଅନ୍ତର୍ଭାଵ "list.h"
#ଅନ୍ତର୍ଭାଵ "packfile.h"
#ଅନ୍ତର୍ଭାଵ "object-file.h"
#ଅନ୍ତର୍ଭାଵ "object-file-convert.h"
#ଅନ୍ତର୍ଭାଵ "odb.h"
#ଅନ୍ତର୍ଭାଵ "odb/streaming.h"
#ଅନ୍ତର୍ଭାଵ "replace-object.h"
#ଅନ୍ତର୍ଭାଵ "dir.h"
#ଅନ୍ତର୍ଭାଵ "midx.h"
#ଅନ୍ତର୍ଭାଵ "trace2.h"
#ଅନ୍ତର୍ଭାଵ "shallow.h"
#ଅନ୍ତର୍ଭାଵ "promisor-remote.h"
#ଅନ୍ତର୍ଭାଵ "pack-mtimes.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"
#ଅନ୍ତର୍ଭାଵ "pkt-line.h"
#ଅନ୍ତର୍ଭାଵ "blob.h"
#ଅନ୍ତର୍ଭାଵ "tree.h"
#ଅନ୍ତର୍ଭାଵ "path-walk.h"

/*
 * Objects we are going to pack are collected in the `to_pack` structure.
 * It contains an array (dynamically expanded) of the object data, and a map
 * that can resolve SHA1s to their position in the array.
 */
ସ୍ଥିର ସଂରଚନା packing_data to_pack;

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା object_entry *oe_delta(
		ଅଚଳ ସଂରଚନା packing_data *pack,
		ଅଚଳ ସଂରଚନା object_entry *e)
{
	ଯଦି (!e->delta_idx)
		ଫେରନ୍ତୁ ଖାଲି;
	ଯଦି (e->ext_base)
		ଫେରନ୍ତୁ &pack->ext_bases[e->delta_idx - 1];
	ଅନ୍ୟଥା
		ଫେରନ୍ତୁ &pack->objects[e->delta_idx - 1];
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ size_t oe_delta_size(ସଂରଚନା packing_data *pack,
				   ଅଚଳ ସଂରଚନା object_entry *e)
{
	ଯଦି (e->delta_size_valid)
		ଫେରନ୍ତୁ e->delta_size_;

	/*
	 * pack->delta_size[] can't be NULL because oe_set_delta_size()
	 * must have been called when a new delta is saved with
	 * oe_set_delta().
	 * If oe_delta() returns NULL (i.e. default state, which means
	 * delta_size_valid is also false), then the caller must never
	 * call oe_delta_size().
	 */
	ଫେରନ୍ତୁ pack->delta_size[e - pack->objects];
}

size_t oe_get_size_slow(ସଂରଚନା packing_data *pack,
			ଅଚଳ ସଂରଚନା object_entry *e);

ସ୍ଥିର ଧାଡ଼ିଭିତରେ size_t oe_size(ସଂରଚନା packing_data *pack,
			     ଅଚଳ ସଂରଚନା object_entry *e)
{
	ଯଦି (e->size_valid)
		ଫେରନ୍ତୁ e->size_;

	ଫେରନ୍ତୁ oe_get_size_slow(pack, e);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ oe_set_delta(ସଂରଚନା packing_data *pack,
				ସଂରଚନା object_entry *e,
				ସଂରଚନା object_entry *delta)
{
	ଯଦି (delta)
		e->delta_idx = (delta - pack->objects) + 1;
	ଅନ୍ୟଥା
		e->delta_idx = 0;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା object_entry *oe_delta_sibling(
		ଅଚଳ ସଂରଚନା packing_data *pack,
		ଅଚଳ ସଂରଚନା object_entry *e)
{
	ଯଦି (e->delta_sibling_idx)
		ଫେରନ୍ତୁ &pack->objects[e->delta_sibling_idx - 1];
	ଫେରନ୍ତୁ ଖାଲି;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା object_entry *oe_delta_child(
		ଅଚଳ ସଂରଚନା packing_data *pack,
		ଅଚଳ ସଂରଚନା object_entry *e)
{
	ଯଦି (e->delta_child_idx)
		ଫେରନ୍ତୁ &pack->objects[e->delta_child_idx - 1];
	ଫେରନ୍ତୁ ଖାଲି;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ oe_set_delta_child(ସଂରଚନା packing_data *pack,
				      ସଂରଚନା object_entry *e,
				      ସଂରଚନା object_entry *delta)
{
	ଯଦି (delta)
		e->delta_child_idx = (delta - pack->objects) + 1;
	ଅନ୍ୟଥା
		e->delta_child_idx = 0;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ oe_set_delta_sibling(ସଂରଚନା packing_data *pack,
					ସଂରଚନା object_entry *e,
					ସଂରଚନା object_entry *delta)
{
	ଯଦି (delta)
		e->delta_sibling_idx = (delta - pack->objects) + 1;
	ଅନ୍ୟଥା
		e->delta_sibling_idx = 0;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ oe_set_size(ସଂରଚନା packing_data *pack,
			       ସଂରଚନା object_entry *e,
			       size_t size)
{
	ଯଦି (size < pack->oe_size_limit) {
		e->size_ = size;
		e->size_valid = 1;
	} ଅନ୍ୟଥା {
		e->size_valid = 0;
		ଯଦି (oe_get_size_slow(pack, e) != size)
			BUG("'size' is supposed to be the object size!");
	}
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ oe_set_delta_size(ସଂରଚନା packing_data *pack,
				     ସଂରଚନା object_entry *e,
				     size_t size)
{
	ଯଦି (size < pack->oe_delta_size_limit) {
		e->delta_size_ = size;
		e->delta_size_valid = 1;
	} ଅନ୍ୟଥା {
		packing_data_lock(pack);
		ଯଦି (!pack->delta_size)
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

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *ଅଚଳ pack_usage[] = {
	N_("git pack-objects [-q | --progress | --all-progress] [--all-progress-implied]\n"
	   "                 [--no-reuse-delta] [--delta-base-offset] [--non-empty]\n"
	   "                 [--local] [--incremental] [--window=<n>] [--depth=<n>]\n"
	   "                 [--revs [--unpacked | --all]] [--keep-pack=<pack-name>]\n"
	   "                 [--cruft] [--cruft-expiration=<time>]\n"
	   "                 [--stdout [--filter=<filter-spec>] | <base-name>]\n"
	   "                 [--shallow] [--keep-true-parents] [--[no-]sparse]\n"
	   "                 [--name-hash-version=<n>] [--path-walk] < <object-list>"),
	ଖାଲି
};

ସ୍ଥିର ସଂରଚନା pack_idx_entry **written_list;
ସ୍ଥିର uint32_t nr_result, nr_written, nr_seen;
ସ୍ଥିର ସଂରଚନା bitmap_index *bitmap_git;
ସ୍ଥିର uint32_t write_layer;

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା non_empty;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା reuse_delta = 1, reuse_object = 1;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା keep_unreachable, unpack_unreachable, include_tag;
ସ୍ଥିର timestamp_t unpack_unreachable_expiration;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା pack_loose_unreachable;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cruft;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା shallow = 0;
ସ୍ଥିର timestamp_t cruft_expiration;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା local;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା have_non_local_packs;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା incremental;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା ignore_packed_keep_on_disk;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା ignore_packed_keep_in_core;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା ignore_packed_keep_in_core_open;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା ignore_packed_keep_in_core_has_cruft;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା allow_ofs_delta;
ସ୍ଥିର ସଂରଚନା pack_idx_option pack_idx_opts;
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *base_name;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା progress = 1;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା window = 10;
ସ୍ଥିର unsigned long pack_size_limit;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth = 50;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା delta_ସନ୍ଧାନ_threads;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା pack_to_stdout;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା sparse;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା thin;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା path_walk = -1;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_preferred_base;
ସ୍ଥିର ସଂରଚନା progress *progress_state;

ସ୍ଥିର ସଂରଚନା bitmapped_pack *reuse_packfiles;
ସ୍ଥିର size_t reuse_packfiles_nr;
ସ୍ଥିର size_t reuse_packfiles_used_nr;
ସ୍ଥିର uint32_t reuse_packfile_objects;
ସ୍ଥିର ସଂରଚନା bitmap *reuse_packfile_bitmap;

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା use_bitmap_index_default = 1;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା use_bitmap_index = -1;
ସ୍ଥିର ଗଣନା {
	NO_PACK_REUSE = 0,
	SINGLE_PACK_REUSE,
	MULTI_PACK_REUSE,
} allow_pack_reuse = SINGLE_PACK_REUSE;
ସ୍ଥିର ଗଣନା {
	WRITE_BITMAP_FALSE = 0,
	WRITE_BITMAP_QUIET,
	WRITE_BITMAP_TRUE,
} write_bitmap_index;
ସ୍ଥିର uint16_t write_bitmap_options = BITMAP_OPT_HASH_CACHE;

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା exclude_promisor_objects;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା exclude_promisor_objects_best_effort;

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା use_delta_islands;

ସ୍ଥିର size_t delta_cache_size = 0;
ସ୍ଥିର size_t max_delta_cache_size = DEFAULT_DELTA_CACHE_SIZE;
ସ୍ଥିର unsigned long cache_max_small_delta_size = 1000;

ସ୍ଥିର unsigned long window_memory_limit = 0;

ସ୍ଥିର ସଂରଚନା string_list uri_protocols = STRING_LIST_INIT_NODUP;

ଗଣନା missing_action {
	MA_ERROR = 0,      /* fail if any missing objects are encountered */
	MA_ALLOW_ANY,      /* silently allow ALL missing objects */
	MA_ALLOW_PROMISOR, /* silently allow all missing PROMISOR objects */
};
ସ୍ଥିର ଗଣନା missing_action arg_missing_action;
ସ୍ଥିର show_object_fn fn_show_object;

ସଂରଚନା configured_exclusion {
	ସଂରଚନା oidmap_entry e;
	ଅକ୍ଷର *pack_hash_hex;
	ଅକ୍ଷର *uri;
};
ସ୍ଥିର ସଂରଚନା oidmap configured_exclusions;

ସ୍ଥିର ସଂରଚନା oidset excluded_by_config;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା name_hash_version = -1;

ଗଣନା stdin_packs_mode {
	STDIN_PACKS_MODE_NONE,
	STDIN_PACKS_MODE_STANDARD,
	STDIN_PACKS_MODE_FOLLOW,
};

/**
 * Check whether the name_hash_version chosen by user input is appropriate,
 * and also validate whether it is compatible with other features.
 */
ସ୍ଥିର ଶୂନ୍ୟ validate_name_hash_version(ଶୂନ୍ୟ)
{
	ଯଦି (name_hash_version < 1 || name_hash_version > 2)
		die(_("invalid --name-hash-version option: %d"), name_hash_version);
	ଯଦି (write_bitmap_index && name_hash_version != 1) {
		warning(_("currently, --write-bitmap-index requires --name-hash-version=1"));
		name_hash_version = 1;
	}
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ uint32_t pack_name_hash_fn(ଅଚଳ ଅକ୍ଷର *name)
{
	ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା seen_version = -1;

	ଯଦି (seen_version < 0)
		seen_version = name_hash_version;
	ଅନ୍ୟଥା ଯଦି (seen_version != name_hash_version)
		BUG("name hash version changed from %d to %d mid-process",
		    seen_version, name_hash_version);

	ବାଛ (name_hash_version) {
	ସ୍ଥିତି 1:
		ଫେରନ୍ତୁ pack_name_hash(name);

	ସ୍ଥିତି 2:
		ଫେରନ୍ତୁ pack_name_hash_v2((ଅଚଳ unsigned ଅକ୍ଷର *)name);

	ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
		BUG("invalid name-hash version: %d", name_hash_version);
	}
}

/*
 * stats
 */
ସ୍ଥିର uint32_t written, written_delta;
ସ୍ଥିର uint32_t reused, reused_delta;

/*
 * Indexed commits
 */
ସ୍ଥିର ସଂରଚନା commit **indexed_commits;
ସ୍ଥିର unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା indexed_commits_nr;
ସ୍ଥିର unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା indexed_commits_alloc;

ସ୍ଥିର ଶୂନ୍ୟ index_commit_for_bitmap(ସଂରଚନା commit *commit)
{
	ଯଦି (indexed_commits_nr >= indexed_commits_alloc) {
		indexed_commits_alloc = (indexed_commits_alloc + 32) * 2;
		REALLOC_ARRAY(indexed_commits, indexed_commits_alloc);
	}

	indexed_commits[indexed_commits_nr++] = commit;
}

ସ୍ଥିର ଶୂନ୍ୟ *get_delta(ସଂରଚନା object_entry *entry)
{
	unsigned long size, base_size;
	size_t delta_size;
	ଶୂନ୍ୟ *buf, *base_buf, *delta_buf;
	ଗଣନା object_type type;
	size_t size_st = 0, base_size_st = 0;

	buf = odb_read_object(the_repository->objects, &entry->idx.oid,
			      &type, &size_st);
	size = cast_size_t_to_ulong(size_st);
	ଯଦି (!buf)
		die(_("unable to read %s"), oid_to_hex(&entry->idx.oid));
	base_buf = odb_read_object(the_repository->objects,
				   &DELTA(entry)->idx.oid, &type,
				   &base_size_st);
	base_size = cast_size_t_to_ulong(base_size_st);
	ଯଦି (!base_buf)
		die("unable to read %s",
		    oid_to_hex(&DELTA(entry)->idx.oid));
	delta_buf = diff_delta(base_buf, base_size,
			       buf, size, &delta_size, 0);
	/*
	 * We successfully computed this delta once but dropped it for
	 * memory reasons. Something is very wrong if this time we
	 * recompute and create a different delta.
	 */
	ଯଦି (!delta_buf || delta_size != DELTA_SIZE(entry))
		BUG("delta size changed");
	ମୁକ୍ତ(buf);
	ମୁକ୍ତ(base_buf);
	ଫେରନ୍ତୁ delta_buf;
}

ସ୍ଥିର unsigned long do_compress(ଶୂନ୍ୟ **pptr, unsigned long size)
{
	git_zstream stream;
	ଶୂନ୍ୟ *in, *out;
	unsigned long maxsize;
	ସଂରଚନା repo_config_ମୂଲ୍ୟଗୁଡ଼ିକ *cfg = repo_config_ମୂଲ୍ୟଗୁଡ଼ିକ(the_repository);

	git_deflate_init(&stream, cfg->pack_compression_level);
	maxsize = git_deflate_bound(&stream, size);

	in = *pptr;
	out = xmalloc(maxsize);
	*pptr = out;

	stream.next_in = in;
	stream.avail_in = size;
	stream.next_out = out;
	stream.avail_out = maxsize;
	ଯେତେବେଳେ (git_deflate(&stream, Z_FINISH) == Z_OK)
		; /* nothing */
	git_deflate_end(&stream);

	ମୁକ୍ତ(in);
	ଫେରନ୍ତୁ stream.total_out;
}

ସ୍ଥିର unsigned long write_large_blob_data(ସଂରଚନା odb_stream *st, ସଂରଚନା hashfile *f,
					   ଅଚଳ ସଂରଚନା object_id *oid)
{
	git_zstream stream;
	unsigned ଅକ୍ଷର ibuf[1024 * 16];
	unsigned ଅକ୍ଷର obuf[1024 * 16];
	unsigned long olen = 0;
	ସଂରଚନା repo_config_ମୂଲ୍ୟଗୁଡ଼ିକ *cfg = repo_config_ମୂଲ୍ୟଗୁଡ଼ିକ(the_repository);

	git_deflate_init(&stream, cfg->pack_compression_level);

	ପାଇଁ (;;) {
		ssize_t readlen;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା zret = Z_OK;
		readlen = odb_stream_read(st, ibuf, ଆକାର(ibuf));
		ଯଦି (readlen == -1)
			die(_("unable to read %s"), oid_to_hex(oid));

		stream.next_in = ibuf;
		stream.avail_in = readlen;
		ଯେତେବେଳେ ((stream.avail_in || readlen == 0) &&
		       (zret == Z_OK || zret == Z_BUF_ERROR)) {
			stream.next_out = obuf;
			stream.avail_out = ଆକାର(obuf);
			zret = git_deflate(&stream, readlen ? 0 : Z_FINISH);
			hashwrite(f, obuf, stream.next_out - obuf);
			olen += stream.next_out - obuf;
		}
		ଯଦି (stream.avail_in)
			die(_("deflate error (%d)"), zret);
		ଯଦି (readlen == 0) {
			ଯଦି (zret != Z_STREAM_END)
				die(_("deflate error (%d)"), zret);
			ଭାଙ୍ଗ;
		}
	}
	git_deflate_end(&stream);
	ଫେରନ୍ତୁ olen;
}

/*
 * we are going to reuse the existing object data as is.  make
 * sure it is not corrupt.
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା check_pack_inflate(ସଂରଚନା packed_git *p,
		ସଂରଚନା pack_window **w_curs,
		off_t offset,
		off_t len,
		size_t expect)
{
	git_zstream stream;
	unsigned ଅକ୍ଷର fakebuf[4096], *in;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା st;

	memset(&stream, 0, ଆକାର(stream));
	git_inflate_init(&stream);
	କର {
		in = use_pack(p, w_curs, offset, &stream.avail_in);
		stream.next_in = in;
		stream.next_out = fakebuf;
		stream.avail_out = ଆକାର(fakebuf);
		st = git_inflate(&stream, Z_FINISH);
		offset += stream.next_in - in;
	} ଯେତେବେଳେ (st == Z_OK || st == Z_BUF_ERROR);
	git_inflate_end(&stream);
	ଫେରନ୍ତୁ (st == Z_STREAM_END &&
		stream.total_out == expect &&
		stream.total_in == len) ? 0 : -1;
}

ସ୍ଥିର ଶୂନ୍ୟ copy_pack_data(ସଂରଚନା hashfile *f,
		ସଂରଚନା packed_git *p,
		ସଂରଚନା pack_window **w_curs,
		off_t offset,
		off_t len)
{
	unsigned ଅକ୍ଷର *in;
	size_t avail;

	ଯେତେବେଳେ (len) {
		in = use_pack(p, w_curs, offset, &avail);
		ଯଦି (avail > len)
			avail = (unsigned long)len;
		hashwrite(f, in, avail);
		offset += avail;
		len -= avail;
	}
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା oe_size_greater_than(ସଂରଚନା packing_data *pack,
				       ଅଚଳ ସଂରଚନା object_entry *lhs,
				       size_t rhs)
{
	ଯଦି (lhs->size_valid)
		ଫେରନ୍ତୁ lhs->size_ > rhs;
	ଯଦି (rhs < pack->oe_size_limit) /* rhs < 2^x <= lhs ? */
		ଫେରନ୍ତୁ 1;
	ଫେରନ୍ତୁ oe_get_size_slow(pack, lhs) > rhs;
}

/* Return 0 if we will bust the pack-size limit */
ସ୍ଥିର unsigned long write_no_reuse_object(ସଂରଚନା hashfile *f, ସଂରଚନା object_entry *entry,
					   unsigned long limit, ପୂର୍ଣ୍ଣସଂଖ୍ୟା usable_delta)
{
	unsigned long size, datalen;
	unsigned ଅକ୍ଷର header[MAX_PACK_OBJECT_HEADER],
		      dheader[MAX_PACK_OBJECT_HEADER];
	unsigned hdrlen;
	ଗଣନା object_type type;
	ଶୂନ୍ୟ *buf;
	ସଂରଚନା odb_stream *st = ଖାଲି;
	ଅଚଳ unsigned hashsz = the_hash_algo->rawsz;

	ଯଦି (!usable_delta) {
		ଯଦି (oe_type(entry) == OBJ_BLOB &&
		    oe_size_greater_than(&to_pack, entry,
					 repo_settings_get_big_file_threshold(the_repository)) &&
		    (st = odb_stream_from_object(the_repository->objects, &entry->idx.oid,
						 ଖାଲି)) != ଖାଲି) {
			buf = ଖାଲି;
			type = st->type;
			size = st->size;
		} ଅନ୍ୟଥା {
			size_t size_st = 0;
			buf = odb_read_object(the_repository->objects,
					      &entry->idx.oid, &type,
					      &size_st);
			size = cast_size_t_to_ulong(size_st);
			ଯଦି (!buf)
				die(_("unable to read %s"),
				    oid_to_hex(&entry->idx.oid));
		}
		/*
		 * make sure no cached delta data remains from a
		 * previous attempt before a pack split occurred.
		 */
		FREE_AND_NULL(entry->delta_data);
		entry->z_delta_size = 0;
	} ଅନ୍ୟଥା ଯଦି (entry->delta_data) {
		size = DELTA_SIZE(entry);
		buf = entry->delta_data;
		entry->delta_data = ଖାଲି;
		type = (allow_ofs_delta && DELTA(entry)->idx.offset) ?
			OBJ_OFS_DELTA : OBJ_REF_DELTA;
	} ଅନ୍ୟଥା {
		buf = get_delta(entry);
		size = DELTA_SIZE(entry);
		type = (allow_ofs_delta && DELTA(entry)->idx.offset) ?
			OBJ_OFS_DELTA : OBJ_REF_DELTA;
	}

	ଯଦି (st)	/* large blob case, just assume we don't compress well */
		datalen = size;
	ଅନ୍ୟଥା ଯଦି (entry->z_delta_size)
		datalen = entry->z_delta_size;
	ଅନ୍ୟଥା
		datalen = do_compress(&buf, size);

	/*
	 * The object header is a byte of 'type' followed by zero or
	 * more bytes of length.
	 */
	hdrlen = encode_in_pack_object_header(header, ଆକାର(header),
					      type, size);

	ଯଦି (type == OBJ_OFS_DELTA) {
		/*
		 * Deltas with relative base contain an additional
		 * encoding of the relative offset for the delta
		 * base from this object's position in the pack.
		 */
		off_t ofs = entry->idx.offset - DELTA(entry)->idx.offset;
		unsigned pos = ଆକାର(dheader) - 1;
		dheader[pos] = ofs & 127;
		ଯେତେବେଳେ (ofs >>= 7)
			dheader[--pos] = 128 | (--ofs & 127);
		ଯଦି (limit && hdrlen + ଆକାର(dheader) - pos + datalen + hashsz >= limit) {
			ଯଦି (st)
				odb_stream_close(st);
			ମୁକ୍ତ(buf);
			ଫେରନ୍ତୁ 0;
		}
		hashwrite(f, header, hdrlen);
		hashwrite(f, dheader + pos, ଆକାର(dheader) - pos);
		hdrlen += ଆକାର(dheader) - pos;
	} ଅନ୍ୟଥା ଯଦି (type == OBJ_REF_DELTA) {
		/*
		 * Deltas with a base reference contain
		 * additional bytes for the base object ID.
		 */
		ଯଦି (limit && hdrlen + hashsz + datalen + hashsz >= limit) {
			ଯଦି (st)
				odb_stream_close(st);
			ମୁକ୍ତ(buf);
			ଫେରନ୍ତୁ 0;
		}
		hashwrite(f, header, hdrlen);
		hashwrite(f, DELTA(entry)->idx.oid.hash, hashsz);
		hdrlen += hashsz;
	} ଅନ୍ୟଥା {
		ଯଦି (limit && hdrlen + datalen + hashsz >= limit) {
			ଯଦି (st)
				odb_stream_close(st);
			ମୁକ୍ତ(buf);
			ଫେରନ୍ତୁ 0;
		}
		hashwrite(f, header, hdrlen);
	}
	ଯଦି (st) {
		datalen = write_large_blob_data(st, f, &entry->idx.oid);
		odb_stream_close(st);
	} ଅନ୍ୟଥା {
		hashwrite(f, buf, datalen);
		ମୁକ୍ତ(buf);
	}

	ଫେରନ୍ତୁ hdrlen + datalen;
}

/* Return 0 if we will bust the pack-size limit */
ସ୍ଥିର off_t write_reuse_object(ସଂରଚନା hashfile *f, ସଂରଚନା object_entry *entry,
				unsigned long limit, ପୂର୍ଣ୍ଣସଂଖ୍ୟା usable_delta)
{
	ସଂରଚନା packed_git *p = IN_PACK(entry);
	ସଂରଚନା pack_window *w_curs = ଖାଲି;
	uint32_t pos;
	off_t offset, cur;
	ଗଣନା object_type type = oe_type(entry);
	ଗଣନା object_type in_pack_type;
	off_t datalen;
	unsigned ଅକ୍ଷର header[MAX_PACK_OBJECT_HEADER],
		      dheader[MAX_PACK_OBJECT_HEADER];
	unsigned hdrlen;
	ଅଚଳ unsigned hashsz = the_hash_algo->rawsz;
	size_t entry_size;

	cur = entry->in_pack_offset;
	in_pack_type = unpack_object_header(p, &w_curs, &cur, &entry_size);
	ଯଦି (in_pack_type < 0)
		die(_("write_reuse_object: unable to parse object header of %s"),
		    oid_to_hex(&entry->idx.oid));

	ଯଦି (DELTA(entry))
		type = (allow_ofs_delta && DELTA(entry)->idx.offset) ?
			OBJ_OFS_DELTA : OBJ_REF_DELTA;
	hdrlen = encode_in_pack_object_header(header, ଆକାର(header),
					      type, entry_size);

	offset = entry->in_pack_offset;
	ଯଦି (offset_to_pack_pos(p, offset, &pos) < 0)
		die(_("write_reuse_object: could not locate %s, expected at "
		      "offset %"PRIuMAX" in pack %s"),
		    oid_to_hex(&entry->idx.oid), (uintmax_t)offset,
		    p->pack_name);
	datalen = pack_pos_to_offset(p, pos + 1) - offset;
	ଯଦି (!pack_to_stdout && p->index_version > 1 &&
	    check_pack_crc(p, &w_curs, offset, datalen,
			   pack_pos_to_index(p, pos))) {
		error(_("bad packed object CRC for %s"),
		      oid_to_hex(&entry->idx.oid));
		unuse_pack(&w_curs);
		ଫେରନ୍ତୁ write_no_reuse_object(f, entry, limit, usable_delta);
	}

	offset += entry->in_pack_header_size;
	datalen -= entry->in_pack_header_size;

	ଯଦି (!pack_to_stdout && p->index_version == 1 &&
	    check_pack_inflate(p, &w_curs, offset, datalen, entry_size)) {
		error(_("corrupt packed object for %s"),
		      oid_to_hex(&entry->idx.oid));
		unuse_pack(&w_curs);
		ଫେରନ୍ତୁ write_no_reuse_object(f, entry, limit, usable_delta);
	}

	ଯଦି (type == OBJ_OFS_DELTA) {
		off_t ofs = entry->idx.offset - DELTA(entry)->idx.offset;
		unsigned pos = ଆକାର(dheader) - 1;
		dheader[pos] = ofs & 127;
		ଯେତେବେଳେ (ofs >>= 7)
			dheader[--pos] = 128 | (--ofs & 127);
		ଯଦି (limit && hdrlen + ଆକାର(dheader) - pos + datalen + hashsz >= limit) {
			unuse_pack(&w_curs);
			ଫେରନ୍ତୁ 0;
		}
		hashwrite(f, header, hdrlen);
		hashwrite(f, dheader + pos, ଆକାର(dheader) - pos);
		hdrlen += ଆକାର(dheader) - pos;
		reused_delta++;
	} ଅନ୍ୟଥା ଯଦି (type == OBJ_REF_DELTA) {
		ଯଦି (limit && hdrlen + hashsz + datalen + hashsz >= limit) {
			unuse_pack(&w_curs);
			ଫେରନ୍ତୁ 0;
		}
		hashwrite(f, header, hdrlen);
		hashwrite(f, DELTA(entry)->idx.oid.hash, hashsz);
		hdrlen += hashsz;
		reused_delta++;
	} ଅନ୍ୟଥା {
		ଯଦି (limit && hdrlen + datalen + hashsz >= limit) {
			unuse_pack(&w_curs);
			ଫେରନ୍ତୁ 0;
		}
		hashwrite(f, header, hdrlen);
	}
	copy_pack_data(f, p, &w_curs, offset, datalen);
	unuse_pack(&w_curs);
	reused++;
	ଫେରନ୍ତୁ hdrlen + datalen;
}

/* Return 0 if we will bust the pack-size limit */
ସ୍ଥିର off_t write_object(ସଂରଚନା hashfile *f,
			  ସଂରଚନା object_entry *entry,
			  off_t write_offset)
{
	unsigned long limit;
	off_t len;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା usable_delta, to_reuse;

	ଯଦି (!pack_to_stdout)
		crc32_begin(f);

	/* apply size limit if limited packsize and not first object */
	ଯଦି (!pack_size_limit || !nr_written)
		limit = 0;
	ଅନ୍ୟଥା ଯଦି (pack_size_limit <= write_offset)
		/*
		 * the earlier object did not fit the limit; avoid
		 * mistaking this with unlimited (i.e. limit = 0).
		 */
		limit = 1;
	ଅନ୍ୟଥା
		limit = pack_size_limit - write_offset;

	ଯଦି (!DELTA(entry))
		usable_delta = 0;	/* no delta */
	ଅନ୍ୟଥା ଯଦି (!pack_size_limit)
	       usable_delta = 1;	/* unlimited packfile */
	ଅନ୍ୟଥା ଯଦି (DELTA(entry)->idx.offset == (off_t)-1)
		usable_delta = 0;	/* base was written to another pack */
	ଅନ୍ୟଥା ଯଦି (DELTA(entry)->idx.offset)
		usable_delta = 1;	/* base already exists in this pack */
	ଅନ୍ୟଥା
		usable_delta = 0;	/* base could end up in another pack */

	ଯଦି (!reuse_object)
		to_reuse = 0;	/* explicit */
	ଅନ୍ୟଥା ଯଦି (!IN_PACK(entry))
		to_reuse = 0;	/* can't reuse what we don't have */
	ଅନ୍ୟଥା ଯଦି (oe_type(entry) == OBJ_REF_DELTA ||
		 oe_type(entry) == OBJ_OFS_DELTA)
				/* check_object() decided it for us ... */
		to_reuse = usable_delta;
				/* ... but pack split may override that */
	ଅନ୍ୟଥା ଯଦି (oe_type(entry) != entry->in_pack_type)
		to_reuse = 0;	/* pack has delta which is unusable */
	ଅନ୍ୟଥା ଯଦି (DELTA(entry))
		to_reuse = 0;	/* we want to pack afresh */
	ଅନ୍ୟଥା
		to_reuse = 1;	/* we have it in-pack undeltified,
				 * and we do not need to deltify it.
				 */

	ଯଦି (!to_reuse)
		len = write_no_reuse_object(f, entry, limit, usable_delta);
	ଅନ୍ୟଥା
		len = write_reuse_object(f, entry, limit, usable_delta);
	ଯଦି (!len)
		ଫେରନ୍ତୁ 0;

	ଯଦି (usable_delta)
		written_delta++;
	written++;
	ଯଦି (!pack_to_stdout)
		entry->idx.crc32 = crc32_end(f);
	ଫେରନ୍ତୁ len;
}

ଗଣନା write_one_status {
	WRITE_ONE_SKIP = -1, /* already written */
	WRITE_ONE_BREAK = 0, /* writing this will bust the limit; not written */
	WRITE_ONE_WRITTEN = 1, /* normal */
	WRITE_ONE_RECURSIVE = 2 /* already scheduled to be written */
};

ସ୍ଥିର ଗଣନା write_one_status write_one(ସଂରଚନା hashfile *f,
				       ସଂରଚନା object_entry *e,
				       off_t *offset)
{
	off_t size;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା recursing;

	/*
	 * we set offset to 1 (which is an impossible value) to mark
	 * the fact that this object is involved in "write its base
	 * first before writing a deltified object" recursion.
	 */
	recursing = (e->idx.offset == 1);
	ଯଦି (recursing) {
		warning(_("recursive delta detected for object %s"),
			oid_to_hex(&e->idx.oid));
		ଫେରନ୍ତୁ WRITE_ONE_RECURSIVE;
	} ଅନ୍ୟଥା ଯଦି (e->idx.offset || e->preferred_base) {
		/* offset is non zero if object is written already. */
		ଫେରନ୍ତୁ WRITE_ONE_SKIP;
	}

	/* if we are deltified, write out base object first. */
	ଯଦି (DELTA(e)) {
		e->idx.offset = 1; /* now recurse */
		ବାଛ (write_one(f, DELTA(e), offset)) {
		ସ୍ଥିତି WRITE_ONE_RECURSIVE:
			/* we cannot depend on this one */
			SET_DELTA(e, ଖାଲି);
			ଭାଙ୍ଗ;
		ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
			ଭାଙ୍ଗ;
		ସ୍ଥିତି WRITE_ONE_BREAK:
			e->idx.offset = recursing;
			ଫେରନ୍ତୁ WRITE_ONE_BREAK;
		}
	}

	e->idx.offset = *offset;
	size = write_object(f, e, *offset);
	ଯଦି (!size) {
		e->idx.offset = recursing;
		ଫେରନ୍ତୁ WRITE_ONE_BREAK;
	}
	written_list[nr_written++] = &e->idx;

	/* make sure off_t is sufficiently large not to wrap */
	ଯଦି (signed_add_overflows(*offset, size))
		die(_("pack too large for current definition of off_t"));
	*offset += size;
	ଫେରନ୍ତୁ WRITE_ONE_WRITTEN;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା mark_tagged(ଅଚଳ ସଂରଚନା reference *ref, ଶୂନ୍ୟ *cb_data UNUSED)
{
	ସଂରଚନା object_id peeled;
	ସଂରଚନା object_entry *entry = packlist_find(&to_pack, ref->oid);

	ଯଦି (entry)
		entry->tagged = 1;
	ଯଦି (!reference_get_peeled_oid(the_repository, ref, &peeled)) {
		entry = packlist_find(&to_pack, &peeled);
		ଯଦି (entry)
			entry->tagged = 1;
	}
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ unsigned ଅକ୍ଷର oe_layer(ସଂରଚନା packing_data *pack,
				     ସଂରଚନା object_entry *e)
{
	ଯଦି (!pack->layer)
		ଫେରନ୍ତୁ 0;
	ଫେରନ୍ତୁ pack->layer[e - pack->objects];
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ add_to_write_order(ସଂରଚନା object_entry **wo,
			       unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *endp,
			       ସଂରଚନା object_entry *e)
{
	ଯଦି (e->filled || oe_layer(&to_pack, e) != write_layer)
		ଫେରନ୍ତୁ;
	wo[(*endp)++] = e;
	e->filled = 1;
}

ସ୍ଥିର ଶୂନ୍ୟ add_descendants_to_write_order(ସଂରଚନା object_entry **wo,
					   unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *endp,
					   ସଂରଚନା object_entry *e)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା add_to_order = 1;
	ଯେତେବେଳେ (e) {
		ଯଦି (add_to_order) {
			ସଂରଚନା object_entry *s;
			/* add this node... */
			add_to_write_order(wo, endp, e);
			/* all its siblings... */
			ପାଇଁ (s = DELTA_SIBLING(e); s; s = DELTA_SIBLING(s)) {
				add_to_write_order(wo, endp, s);
			}
		}
		/* drop down a level to add left subtree nodes if possible */
		ଯଦି (DELTA_CHILD(e)) {
			add_to_order = 1;
			e = DELTA_CHILD(e);
		} ଅନ୍ୟଥା {
			add_to_order = 0;
			/* our sibling might have some children, it is next */
			ଯଦି (DELTA_SIBLING(e)) {
				e = DELTA_SIBLING(e);
				ଜାରି;
			}
			/* go back to our parent node */
			e = DELTA(e);
			ଯେତେବେଳେ (e && !DELTA_SIBLING(e)) {
				/* we're on the right side of a subtree, keep
				 * going up until we can go right again */
				e = DELTA(e);
			}
			ଯଦି (!e) {
				/* done- we hit our original root node */
				ଫେରନ୍ତୁ;
			}
			/* pass it off to sibling at this level */
			e = DELTA_SIBLING(e);
		}
	};
}

ସ୍ଥିର ଶୂନ୍ୟ add_family_to_write_order(ସଂରଚନା object_entry **wo,
				      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *endp,
				      ସଂରଚନା object_entry *e)
{
	ସଂରଚନା object_entry *ମୂଳ;

	ପାଇଁ (ମୂଳ = e; DELTA(ମୂଳ); ମୂଳ = DELTA(ମୂଳ))
		; /* nothing */
	add_descendants_to_write_order(wo, endp, ମୂଳ);
}

ସ୍ଥିର ଶୂନ୍ୟ compute_layer_order(ସଂରଚନା object_entry **wo, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *wo_end)
{
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, last_untagged;
	ସଂରଚନା object_entry *objects = to_pack.objects;

	ପାଇଁ (i = 0; i < to_pack.nr_objects; i++) {
		ଯଦି (objects[i].tagged)
			ଭାଙ୍ଗ;
		add_to_write_order(wo, wo_end, &objects[i]);
	}
	last_untagged = i;

	/*
	 * Then fill all the tagged tips.
	 */
	ପାଇଁ (; i < to_pack.nr_objects; i++) {
		ଯଦି (objects[i].tagged)
			add_to_write_order(wo, wo_end, &objects[i]);
	}

	/*
	 * And then all remaining commits and tags.
	 */
	ପାଇଁ (i = last_untagged; i < to_pack.nr_objects; i++) {
		ଯଦି (oe_type(&objects[i]) != OBJ_COMMIT &&
		    oe_type(&objects[i]) != OBJ_TAG)
			ଜାରି;
		add_to_write_order(wo, wo_end, &objects[i]);
	}

	/*
	 * And then all the trees.
	 */
	ପାଇଁ (i = last_untagged; i < to_pack.nr_objects; i++) {
		ଯଦି (oe_type(&objects[i]) != OBJ_ବୃକ୍ଷ)
			ଜାରି;
		add_to_write_order(wo, wo_end, &objects[i]);
	}

	/*
	 * Finally all the rest in really tight order
	 */
	ପାଇଁ (i = last_untagged; i < to_pack.nr_objects; i++) {
		ଯଦି (!objects[i].filled && oe_layer(&to_pack, &objects[i]) == write_layer)
			add_family_to_write_order(wo, wo_end, &objects[i]);
	}
}

ସ୍ଥିର ସଂରଚନା object_entry **compute_write_order(ଶୂନ୍ୟ)
{
	uint32_t max_layers = 1;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, wo_end;

	ସଂରଚନା object_entry **wo;
	ସଂରଚନା object_entry *objects = to_pack.objects;

	ପାଇଁ (i = 0; i < to_pack.nr_objects; i++) {
		objects[i].tagged = 0;
		objects[i].filled = 0;
		SET_DELTA_CHILD(&objects[i], ଖାଲି);
		SET_DELTA_SIBLING(&objects[i], ଖାଲି);
	}

	/*
	 * Fully connect delta_child/delta_sibling network.
	 * Make sure delta_sibling is sorted in the original
	 * recency order.
	 */
	ପାଇଁ (i = to_pack.nr_objects; i > 0;) {
		ସଂରଚନା object_entry *e = &objects[--i];
		ଯଦି (!DELTA(e))
			ଜାରି;
		/* Mark me as the first child */
		e->delta_sibling_idx = DELTA(e)->delta_child_idx;
		SET_DELTA_CHILD(DELTA(e), e);
	}

	/*
	 * Mark objects that are at the tip of tags.
	 */
	refs_for_each_tag_ref(get_main_ref_store(the_repository), mark_tagged,
			      ଖାଲି);

	ଯଦି (use_delta_islands) {
		max_layers = compute_pack_layers(&to_pack);
		free_island_marks();
	}

	ALLOC_ARRAY(wo, to_pack.nr_objects);
	wo_end = 0;

	ପାଇଁ (; write_layer < max_layers; ++write_layer)
		compute_layer_order(wo, &wo_end);

	ଯଦି (wo_end != to_pack.nr_objects)
		die(_("ordered %u objects, expected %"PRIu32),
		    wo_end, to_pack.nr_objects);

	ଫେରନ୍ତୁ wo;
}


/*
 * A reused set of objects. All objects in a chunk have the same
 * relative position in the original packfile and the generated
 * packfile.
 */

ସ୍ଥିର ସଂରଚନା reused_chunk {
	/* The offset of the first object of this chunk in the original
	 * packfile. */
	off_t original;
	/* The difference for "original" minus the offset of the first object of
	 * this chunk in the generated packfile. */
	off_t difference;
} *reused_chunks;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା reused_chunks_nr;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା reused_chunks_alloc;

ସ୍ଥିର ଶୂନ୍ୟ record_reused_object(off_t where, off_t offset)
{
	ଯଦି (reused_chunks_nr && reused_chunks[reused_chunks_nr-1].difference == offset)
		ଫେରନ୍ତୁ;

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
ସ୍ଥିର off_t find_reused_offset(off_t where)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା lo = 0, hi = reused_chunks_nr;
	ଯେତେବେଳେ (lo < hi) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା mi = lo + ((hi - lo) / 2);
		ଯଦି (where == reused_chunks[mi].original)
			ଫେରନ୍ତୁ reused_chunks[mi].difference;
		ଯଦି (where < reused_chunks[mi].original)
			hi = mi;
		ଅନ୍ୟଥା
			lo = mi + 1;
	}

	/*
	 * The first chunk starts at zero, so we can't have gone below
	 * there.
	 */
	ପ୍ରତିଜ୍ଞା(lo);
	ଫେରନ୍ତୁ reused_chunks[lo-1].difference;
}

ସ୍ଥିର ଶୂନ୍ୟ write_reused_pack_one(ସଂରଚନା packed_git *reuse_packfile,
				  size_t pos, ସଂରଚନା hashfile *out,
				  off_t pack_start,
				  ସଂରଚନା pack_window **w_curs)
{
	off_t offset, next, cur;
	ଗଣନା object_type type;
	size_t size;

	offset = pack_pos_to_offset(reuse_packfile, pos);
	next = pack_pos_to_offset(reuse_packfile, pos + 1);

	record_reused_object(offset,
			     offset - (hashfile_total(out) - pack_start));

	cur = offset;
	type = unpack_object_header(reuse_packfile, w_curs, &cur, &size);
	ପ୍ରତିଜ୍ଞା(type >= 0);

	ଯଦି (type == OBJ_OFS_DELTA) {
		off_t base_offset;
		off_t fixup;

		unsigned ଅକ୍ଷର header[MAX_PACK_OBJECT_HEADER];
		unsigned len;

		base_offset = get_delta_base(reuse_packfile, w_curs, &cur, type, offset);
		ପ୍ରତିଜ୍ଞା(base_offset != 0);

		/* Convert to REF_DELTA if we must... */
		ଯଦି (!allow_ofs_delta) {
			uint32_t base_pos;
			ସଂରଚନା object_id base_oid;

			ଯଦି (offset_to_pack_pos(reuse_packfile, base_offset, &base_pos) < 0)
				die(_("expected object at offset %"PRIuMAX" "
				      "in pack %s"),
				    (uintmax_t)base_offset,
				    reuse_packfile->pack_name);

			nth_packed_object_id(&base_oid, reuse_packfile,
					     pack_pos_to_index(reuse_packfile, base_pos));

			len = encode_in_pack_object_header(header, ଆକାର(header),
							   OBJ_REF_DELTA, size);
			hashwrite(out, header, len);
			hashwrite(out, base_oid.hash, the_hash_algo->rawsz);
			copy_pack_data(out, reuse_packfile, w_curs, cur, next - cur);
			ଫେରନ୍ତୁ;
		}

		/* Otherwise see if we need to rewrite the offset... */
		fixup = find_reused_offset(offset) -
			find_reused_offset(base_offset);
		ଯଦି (fixup) {
			unsigned ଅକ୍ଷର ofs_header[MAX_PACK_OBJECT_HEADER];
			unsigned i, ofs_len;
			off_t ofs = offset - base_offset - fixup;

			len = encode_in_pack_object_header(header, ଆକାର(header),
							   OBJ_OFS_DELTA, size);

			i = ଆକାର(ofs_header) - 1;
			ofs_header[i] = ofs & 127;
			ଯେତେବେଳେ (ofs >>= 7)
				ofs_header[--i] = 128 | (--ofs & 127);

			ofs_len = ଆକାର(ofs_header) - i;

			hashwrite(out, header, len);
			hashwrite(out, ofs_header + ଆକାର(ofs_header) - ofs_len, ofs_len);
			copy_pack_data(out, reuse_packfile, w_curs, cur, next - cur);
			ଫେରନ୍ତୁ;
		}

		/* ...otherwise we have no fixup, and can write it verbatim */
	}

	copy_pack_data(out, reuse_packfile, w_curs, offset, next - offset);
}

ସ୍ଥିର size_t write_reused_pack_verbatim(ସଂରଚନା bitmapped_pack *reuse_packfile,
					 ସଂରଚନା hashfile *out,
					 ସଂରଚନା pack_window **w_curs)
{
	size_t pos = 0;
	size_t end;

	ଯଦି (reuse_packfile->bitmap_pos) {
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
		ଫେରନ୍ତୁ reuse_packfile->bitmap_pos / BITS_IN_EWORD;
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
	ଯଦି (reuse_packfile_bitmap->word_alloc < end)
		BUG("fewer words than expected in reuse_packfile_bitmap");

	ଯେତେବେଳେ (pos < end && reuse_packfile_bitmap->ଶବ୍ଦଗୁଡ଼ିକ[pos] == (eword_t)~0)
		pos++;

	ଯଦି (pos) {
		off_t to_write;

		written = (pos * BITS_IN_EWORD);
		to_write = pack_pos_to_offset(reuse_packfile->p, written)
			- ଆକାର(ସଂରଚନା pack_header);

		/* We're recording one chunk, not one object. */
		record_reused_object(ଆକାର(ସଂରଚନା pack_header), 0);
		hashflush(out);
		copy_pack_data(out, reuse_packfile->p, w_curs,
			ଆକାର(ସଂରଚନା pack_header), to_write);

		display_progress(progress_state, written);
	}
	ଫେରନ୍ତୁ pos;
}

ସ୍ଥିର ଶୂନ୍ୟ write_reused_pack(ସଂରଚନା bitmapped_pack *reuse_packfile,
			      ସଂରଚନା hashfile *f)
{
	size_t i = reuse_packfile->bitmap_pos / BITS_IN_EWORD;
	uint32_t offset;
	off_t pack_start = hashfile_total(f) - ଆକାର(ସଂରଚନା pack_header);
	ସଂରଚନା pack_window *w_curs = ଖାଲି;

	ଯଦି (allow_ofs_delta)
		i = write_reused_pack_verbatim(reuse_packfile, f, &w_curs);

	ପାଇଁ (; i < reuse_packfile_bitmap->word_alloc; ++i) {
		eword_t word = reuse_packfile_bitmap->ଶବ୍ଦଗୁଡ଼ିକ[i];
		size_t pos = (i * BITS_IN_EWORD);

		ପାଇଁ (offset = 0; offset < BITS_IN_EWORD; ++offset) {
			uint32_t pack_pos;
			ଯଦି ((word >> offset) == 0)
				ଭାଙ୍ଗ;

			offset += ewah_bit_ctz64(word >> offset);
			ଯଦି (pos + offset < reuse_packfile->bitmap_pos)
				ଜାରି;
			ଯଦି (pos + offset >= reuse_packfile->bitmap_pos + reuse_packfile->bitmap_nr)
				ଯାଅ done;

			ଯଦି (reuse_packfile->bitmap_pos) {
				/*
				 * When doing multi-pack reuse on a
				 * non-preferred pack, translate bit positions
				 * from the MIDX pseudo-pack order back to their
				 * pack-relative positions before attempting
				 * reuse.
				 */
				ସଂରଚନା multi_pack_index *m = reuse_packfile->from_midx;
				uint32_t midx_pos;
				off_t pack_ofs;

				ଯଦି (!m)
					BUG("non-zero bitmap position without MIDX");

				midx_pos = pack_pos_to_midx(m, pos + offset);
				pack_ofs = nth_midxed_offset(m, midx_pos);

				ଯଦି (offset_to_pack_pos(reuse_packfile->p,
						       pack_ofs, &pack_pos) < 0)
					BUG("could not find expected object at offset %"PRIuMAX" in pack %s",
					    (uintmax_t)pack_ofs,
					    pack_basename(reuse_packfile->p));
			} ଅନ୍ୟଥା {
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

ସ୍ଥିର ଶୂନ୍ୟ write_excluded_by_configs(ଶୂନ୍ୟ)
{
	ସଂରଚନା oidset_iter iter;
	ଅଚଳ ସଂରଚନା object_id *oid;

	oidset_iter_init(&excluded_by_config, &iter);
	ଯେତେବେଳେ ((oid = oidset_iter_next(&iter))) {
		ସଂରଚନା configured_exclusion *ex =
			oidmap_get(&configured_exclusions, oid);

		ଯଦି (!ex)
			BUG("configured exclusion wasn't configured");
		write_in_full(1, ex->pack_hash_hex, strlen(ex->pack_hash_hex));
		write_in_full(1, " ", 1);
		write_in_full(1, ex->uri, strlen(ex->uri));
		write_in_full(1, "\n", 1);
	}
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର no_split_warning[] = N_(
"disabling bitmap writing, packs are split due to pack.packSizeLimit"
);

ସ୍ଥିର ଶୂନ୍ୟ write_pack_file(ଶୂନ୍ୟ)
{
	uint32_t i = 0, j;
	ସଂରଚନା hashfile *f;
	off_t offset;
	uint32_t nr_remaining = nr_result;
	time_t last_mtime = 0;
	ସଂରଚନା object_entry **write_order;
	off_t bytes_written = 0;

	ଯଦି (progress > pack_to_stdout)
		progress_state = start_progress(the_repository,
						_("Writing objects"), nr_result);
	ALLOC_ARRAY(written_list, to_pack.nr_objects);
	write_order = compute_write_order();

	କର {
		unsigned ଅକ୍ଷର hash[GIT_MAX_RAWSZ];
		ଅକ୍ଷର *pack_tmp_name = ଖାଲି;

		ଯଦି (pack_to_stdout) {
			/*
			 * This command is most often invoked via
			 * git-upload-pack(1), which will typically chunk data
			 * into pktlines. As such, we use the maximum data
			 * length of them as buffer length.
			 *
			 * Note that we need to subtract one though to
			 * accommodate for the sideband byte.
			 */
			ସଂରଚନା hashfd_options opts = {
				.progress = progress_state,
				.buffer_len = LARGE_PACKET_DATA_MAX - 1,
			};
			f = hashfd_ext(the_repository->hash_algo, 1,
				       "<stdout>", &opts);
		} ଅନ୍ୟଥା {
			f = create_tmp_packfile(the_repository, &pack_tmp_name);
		}

		offset = write_pack_header(f, nr_remaining);

		ଯଦି (reuse_packfiles_nr) {
			ପ୍ରତିଜ୍ଞା(pack_to_stdout);
			ପାଇଁ (j = 0; j < reuse_packfiles_nr; j++) {
				reused_chunks_nr = 0;
				write_reused_pack(&reuse_packfiles[j], f);
				ଯଦି (reused_chunks_nr)
					reuse_packfiles_used_nr++;
			}
			offset = hashfile_total(f);
		}

		nr_written = 0;
		ପାଇଁ (; i < to_pack.nr_objects; i++) {
			ସଂରଚନା object_entry *e = write_order[i];
			ଯଦି (write_one(f, e, &offset) == WRITE_ONE_BREAK)
				ଭାଙ୍ଗ;
			display_progress(progress_state, written);
		}

		bytes_written += hashfile_total(f) +
			the_repository->hash_algo->rawsz;
		ଯଦି (pack_to_stdout) {
			/*
			 * We never fsync when writing to stdout since we may
			 * not be writing to an actual pack file. For instance,
			 * the upload-pack code passes a pipe here. Calling
			 * fsync on a pipe results in unnecessary
			 * synchronization with the reader on some platforms.
			 */
			finalize_hashfile(f, hash, FSYNC_COMPONENT_NONE,
					  CSUM_HASH_IN_STREAM | CSUM_CLOSE);
		} ଅନ୍ୟଥା ଯଦି (nr_written == nr_remaining) {
			finalize_hashfile(f, hash, FSYNC_COMPONENT_PACK,
					  CSUM_HASH_IN_STREAM | CSUM_FSYNC | CSUM_CLOSE);
		} ଅନ୍ୟଥା {
			/*
			 * If we wrote the wrong number of entries in the
			 * header, rewrite it like in fast-import.
			 */

			ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = finalize_hashfile(f, hash, FSYNC_COMPONENT_PACK, 0);
			fixup_pack_header_footer(the_hash_algo, fd, hash,
						 pack_tmp_name, nr_written,
						 hash, offset);
			close(fd);
			ଯଦି (write_bitmap_index) {
				ଯଦି (write_bitmap_index != WRITE_BITMAP_QUIET)
					warning(_(no_split_warning));
				write_bitmap_index = 0;
			}
		}

		ଯଦି (!pack_to_stdout) {
			ସଂରଚନା stat st;
			ସଂରଚନା strbuf tmpname = STRBUF_INIT;
			ସଂରଚନା bitmap_writer bitmap_writer;
			ଅକ୍ଷର *idx_tmp_name = ଖାଲି;

			/*
			 * Packs are runtime accessed in their mtime
			 * order since newer packs are more likely to contain
			 * younger objects.  So if we are creating multiple
			 * packs then we should modify the mtime of later ones
			 * to preserve this property.
			 */
			ଯଦି (stat(pack_tmp_name, &st) < 0) {
				warning_errno(_("failed to stat %s"), pack_tmp_name);
			} ଅନ୍ୟଥା ଯଦି (!last_mtime) {
				last_mtime = st.st_mtime;
			} ଅନ୍ୟଥା {
				ସଂରଚନା utimbuf utb;
				utb.actime = st.st_atime;
				utb.modtime = --last_mtime;
				ଯଦି (utime(pack_tmp_name, &utb) < 0)
					warning_errno(_("failed utime() on %s"), pack_tmp_name);
			}

			strbuf_addf(&tmpname, "%s-%s.", base_name,
				    hash_to_hex(hash));

			ଯଦି (write_bitmap_index) {
				bitmap_writer_init(&bitmap_writer,
						   the_repository, &to_pack,
						   ଖାଲି);
				bitmap_writer_set_checksum(&bitmap_writer, hash);
				bitmap_writer_build_type_index(&bitmap_writer,
							       written_list);
			}

			ଯଦି (cruft)
				pack_idx_opts.flags |= WRITE_MTIMES;

			stage_tmp_packfiles(the_repository, &tmpname,
					    pack_tmp_name, written_list,
					    nr_written, &to_pack,
					    &pack_idx_opts, hash,
					    &idx_tmp_name);

			ଯଦି (write_bitmap_index) {
				size_t tmpname_len = tmpname.len;

				strbuf_addstr(&tmpname, "bitmap");
				stop_progress(&progress_state);

				bitmap_writer_show_progress(&bitmap_writer,
							    progress);
				bitmap_writer_select_commits(&bitmap_writer,
							     indexed_commits,
							     indexed_commits_nr);
				ଯଦି (bitmap_writer_build(&bitmap_writer) < 0)
					die(_("failed to write bitmap index"));
				bitmap_writer_finish(&bitmap_writer,
						     written_list,
						     tmpname.buf, write_bitmap_options);
				bitmap_writer_free(&bitmap_writer);
				write_bitmap_index = 0;
				strbuf_setlen(&tmpname, tmpname_len);
			}

			rename_tmp_packfile_idx(the_repository, &tmpname, &idx_tmp_name);

			ମୁକ୍ତ(idx_tmp_name);
			strbuf_release(&tmpname);
			ମୁକ୍ତ(pack_tmp_name);
			puts(hash_to_hex(hash));
		}

		/* mark written objects as written to previous pack */
		ପାଇଁ (j = 0; j < nr_written; j++) {
			written_list[j]->offset = (off_t)-1;
		}
		nr_remaining -= nr_written;
	} ଯେତେବେଳେ (nr_remaining && i < to_pack.nr_objects);

	ମୁକ୍ତ(written_list);
	ମୁକ୍ତ(write_order);
	stop_progress(&progress_state);
	ଯଦି (written != nr_result)
		die(_("wrote %"PRIu32" objects while expecting %"PRIu32),
		    written, nr_result);
	trace2_data_intmax("pack-objects", the_repository,
			   "write_pack_file/wrote", nr_result);
	trace2_data_intmax("pack-objects", the_repository,
			   "write_pack_file/wrote_bytes", bytes_written);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା no_try_delta(ଅଚଳ ଅକ୍ଷର *path)
{
	ସ୍ଥିର ସଂରଚନା attr_check *check;

	ଯଦି (!check)
		check = attr_check_initl("delta", ଖାଲି);
	git_check_attr(the_repository->index, path, check);
	ଯଦି (ATTR_FALSE(check->ବସ୍ତୁଗୁଡ଼ିକ[0].ମୂଲ୍ୟ))
		ଫେରନ୍ତୁ 1;
	ଫେରନ୍ତୁ 0;
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
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା have_duplicate_entry(ଅଚଳ ସଂରଚନା object_id *oid,
				ପୂର୍ଣ୍ଣସଂଖ୍ୟା exclude)
{
	ସଂରଚନା object_entry *entry;

	ଯଦି (reuse_packfile_bitmap &&
	    bitmap_walk_contains(bitmap_git, reuse_packfile_bitmap, oid))
		ଫେରନ୍ତୁ 1;

	entry = packlist_find(&to_pack, oid);
	ଯଦି (!entry)
		ଫେରନ୍ତୁ 0;

	ଯଦି (exclude) {
		ଯଦି (!entry->preferred_base)
			nr_result--;
		entry->preferred_base = 1;
	}

	ଫେରନ୍ତୁ 1;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା want_cruft_object_mtime(ସଂରଚନା repository *r,
				   ଅଚଳ ସଂରଚନା object_id *oid,
				   unsigned flags, uint32_t mtime)
{
	ସଂରଚନା odb_source_files *files = odb_source_files_downcast(r->objects->source);

	ପାଇଁ (ସଂରଚନା odb_files_dir *dir = files->dirs; dir; dir = dir->next) {
		ସଂରଚନା packed_git **cache = packfile_store_get_kept_pack_cache(dir->packed, flags);

		ପାଇଁ (; *cache; cache++) {
			ସଂରଚନା packed_git *p = *cache;
			off_t ofs;
			uint32_t candidate_mtime;

			ofs = find_pack_entry_one(oid, p);
			ଯଦି (!ofs)
				ଜାରି;

			/*
			 * We have a copy of the object 'oid' in a non-cruft
			 * pack. We can avoid packing an additional copy
			 * regardless of what the existing copy's mtime is since
			 * it is outside of a cruft pack.
			 */
			ଯଦି (!p->is_cruft)
				ଫେରନ୍ତୁ 0;

			/*
			 * If we have a copy of the object 'oid' in a cruft
			 * pack, then either read the cruft pack's mtime for
			 * that object, or, if that can't be loaded, assume the
			 * pack's mtime itself.
			 */
			ଯଦି (!load_pack_mtimes(p)) {
				uint32_t pos;
				ଯଦି (offset_to_pack_pos(p, ofs, &pos) < 0)
					ଜାରି;
				candidate_mtime = nth_packed_mtime(p, pos);
			} ଅନ୍ୟଥା {
				candidate_mtime = p->mtime;
			}

			/*
			 * We have a surviving copy of the object in a cruft
			 * pack whose mtime is greater than or equal to the one
			 * we are considering. We can thus avoid packing an
			 * additional copy of that object.
			 */
			ଯଦି (mtime <= candidate_mtime)
				ଫେରନ୍ତୁ 0;
		}
	}

	ଫେରନ୍ତୁ -1;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା want_found_object(ଅଚଳ ସଂରଚନା object_id *oid, ପୂର୍ଣ୍ଣସଂଖ୍ୟା exclude,
			     ସଂରଚନା packed_git *p, uint32_t mtime)
{
	ଯଦି (exclude)
		ଫେରନ୍ତୁ 1;
	ଯଦି (incremental)
		ଫେରନ୍ତୁ 0;

	ଯଦି (!is_pack_valid(p))
		ଫେରନ୍ତୁ -1;

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
	ଯଦି (local && !p->pack_local)
		ଫେରନ୍ତୁ 0;

	/*
	 * Then handle .keep first, as we have a fast(er) path there.
	 */
	ଯଦି (ignore_packed_keep_on_disk || ignore_packed_keep_in_core ||
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
		ଯଦି (ignore_packed_keep_on_disk)
			flags |= KEPT_PACK_ON_DISK;
		ଯଦି (ignore_packed_keep_in_core)
			flags |= KEPT_PACK_IN_CORE;
		ଯଦି (ignore_packed_keep_in_core_open)
			flags |= KEPT_PACK_IN_CORE_OPEN;

		/*
		 * If the object is in a pack that we want to ignore, *and* we
		 * don't have any cruft packs that are being retained, we can
		 * abort quickly.
		 */
		ଯଦି (!ignore_packed_keep_in_core_has_cruft) {
			ଯଦି (ignore_packed_keep_on_disk && p->pack_keep)
				ଫେରନ୍ତୁ 0;
			ଯଦି (ignore_packed_keep_in_core && p->pack_keep_in_core)
				ଫେରନ୍ତୁ 0;
			ଯଦି (ignore_packed_keep_in_core_open && p->pack_keep_in_core_open)
				ଫେରନ୍ତୁ 0;
			ଯଦି (has_object_kept_pack(p->repo, oid, flags))
				ଫେରନ୍ତୁ 0;
		} ଅନ୍ୟଥା {
			/*
			 * But if there is at least one cruft pack which
			 * is being kept, we only want to include the
			 * provided object if it has a strictly greater
			 * mtime than any existing cruft copy.
			 */
			ଯଦି (!want_cruft_object_mtime(p->repo, oid, flags,
						     mtime))
				ଫେରନ୍ତୁ 0;
		}
	}

	/*
	 * At this point we know definitively that either we don't care about
	 * keep-packs, or the object is not in one. Keep checking other
	 * conditions...
	 */
	ଯଦି (!local || !have_non_local_packs)
		ଫେରନ୍ତୁ 1;

	/* we don't know yet; keep looking for more packs */
	ଫେରନ୍ତୁ -1;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା want_object_in_pack_one(ସଂରଚନା packed_git *p,
				   ଅଚଳ ସଂରଚନା object_id *oid,
				   ପୂର୍ଣ୍ଣସଂଖ୍ୟା exclude,
				   ସଂରଚନା packed_git **found_pack,
				   off_t *found_offset,
				   uint32_t found_mtime)
{
	off_t offset;

	ଯଦି (p == *found_pack)
		offset = *found_offset;
	ଅନ୍ୟଥା
		offset = find_pack_entry_one(oid, p);

	ଯଦି (offset) {
		ଯଦି (!*found_pack) {
			ଯଦି (!is_pack_valid(p))
				ଫେରନ୍ତୁ -1;
			*found_offset = offset;
			*found_pack = p;
		}
		ଫେରନ୍ତୁ want_found_object(oid, exclude, p, found_mtime);
	}
	ଫେରନ୍ତୁ -1;
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
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା want_object_in_pack_mtime(ଅଚଳ ସଂରଚନା object_id *oid,
				     ପୂର୍ଣ୍ଣସଂଖ୍ୟା exclude,
				     ସଂରଚନା packed_git **found_pack,
				     off_t *found_offset,
				     uint32_t found_mtime)
{
	ସଂରଚନା odb_source_files *files =
		odb_source_files_downcast(the_repository->objects->source);
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା want;
	ସଂରଚନା packfile_list_entry *e;

	ଯଦି (!exclude && local) {
		/*
		 * Note that we start iterating at `dirs->next` so that we
		 * skip the local object directory.
		 */
		ପାଇଁ (ସଂରଚନା odb_files_dir *dir = files->dirs->next; dir; dir = dir->next)
			ଯଦି (!odb_source_read_object_info(&dir->loose->base, oid, ଖାଲି, 0, ଖାଲି))
				ଫେରନ୍ତୁ 0;
	}

	/*
	 * If we already know the pack object lives in, start checks from that
	 * pack - in the usual case when neither --local was given nor .keep files
	 * are present we will determine the answer right now.
	 */
	ଯଦି (*found_pack) {
		want = want_found_object(oid, exclude, *found_pack,
					 found_mtime);
		ଯଦି (want != -1)
			ଫେରନ୍ତୁ want;

		*found_pack = ଖାଲି;
		*found_offset = 0;
	}

	ପାଇଁ (ସଂରଚନା odb_files_dir *dir = files->dirs; dir; dir = dir->next) {
		ସଂରଚନା multi_pack_index *m = get_multi_pack_index(dir->packed);
		ସଂରଚନା pack_entry e;

		ଯଦି (m && midx_fill_entry(m, oid, &e, ଖାଲି) == MIDX_FILL_HIT) {
			want = want_object_in_pack_one(e.p, oid, exclude, found_pack, found_offset, found_mtime);
			ଯଦି (want != -1)
				ଫେରନ୍ତୁ want;
		}
	}

	ପାଇଁ (ସଂରଚନା odb_files_dir *dir = files->dirs; dir; dir = dir->next) {
		ପାଇଁ (e = dir->packed->packs.head; e; e = e->next) {
			ସଂରଚନା packed_git *p = e->pack;
			want = want_object_in_pack_one(p, oid, exclude, found_pack, found_offset, found_mtime);
			ଯଦି (!exclude && want > 0)
				packfile_list_prepend(&dir->packed->packs, p);
			ଯଦି (want != -1)
				ଫେରନ୍ତୁ want;
		}
	}

	ଯଦି (uri_protocols.nr) {
		ସଂରଚନା configured_exclusion *ex =
			oidmap_get(&configured_exclusions, oid);
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
		ଅଚଳ ଅକ୍ଷର *p;

		ଯଦି (ex) {
			ପାଇଁ (i = 0; i < uri_protocols.nr; i++) {
				ଯଦି (skip_prefix(ex->uri,
						uri_protocols.ବସ୍ତୁଗୁଡ଼ିକ[i].string,
						&p) &&
				    *p == ':') {
					oidset_ଭର୍ତ୍ତି(&excluded_by_config, oid);
					ଫେରନ୍ତୁ 0;
				}
			}
		}
	}

	ଫେରନ୍ତୁ 1;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା want_object_in_pack(ଅଚଳ ସଂରଚନା object_id *oid,
				      ପୂର୍ଣ୍ଣସଂଖ୍ୟା exclude,
				      ସଂରଚନା packed_git **found_pack,
				      off_t *found_offset)
{
	ଫେରନ୍ତୁ want_object_in_pack_mtime(oid, exclude, found_pack, found_offset,
					 0);
}

ସ୍ଥିର ସଂରଚନା object_entry *create_object_entry(ଅଚଳ ସଂରଚନା object_id *oid,
						ଗଣନା object_type type,
						uint32_t hash,
						ପୂର୍ଣ୍ଣସଂଖ୍ୟା exclude,
						ପୂର୍ଣ୍ଣସଂଖ୍ୟା no_try_delta,
						ସଂରଚନା packed_git *found_pack,
						off_t found_offset)
{
	ସଂରଚନା object_entry *entry;

	entry = packlist_alloc(&to_pack, oid);
	entry->hash = hash;
	oe_set_type(entry, type);
	ଯଦି (exclude)
		entry->preferred_base = 1;
	ଅନ୍ୟଥା
		nr_result++;
	ଯଦି (found_pack) {
		oe_set_in_pack(&to_pack, entry, found_pack);
		entry->in_pack_offset = found_offset;
	}

	entry->no_try_delta = no_try_delta;

	ଫେରନ୍ତୁ entry;
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର no_closure_warning[] = N_(
"disabling bitmap writing, as some objects are not being packed"
);

ସ୍ଥିର ଶୂନ୍ୟ add_object_entry(ଅଚଳ ସଂରଚନା object_id *oid, ଗଣନା object_type type,
			     ଅଚଳ ଅକ୍ଷର *name, ପୂର୍ଣ୍ଣସଂଖ୍ୟା exclude)
{
	ସଂରଚନା packed_git *found_pack = ଖାଲି;
	off_t found_offset = 0;

	display_progress(progress_state, ++nr_seen);

	ଯଦି (have_duplicate_entry(oid, exclude))
		ଫେରନ୍ତୁ;

	ଯଦି (!want_object_in_pack(oid, exclude, &found_pack, &found_offset)) {
		/* The pack is missing an object, so it will not have closure */
		ଯଦି (write_bitmap_index) {
			ଯଦି (write_bitmap_index != WRITE_BITMAP_QUIET)
				warning(_(no_closure_warning));
			write_bitmap_index = 0;
		}
		ଫେରନ୍ତୁ;
	}

	create_object_entry(oid, type, pack_name_hash_fn(name),
			    exclude, name && no_try_delta(name),
			    found_pack, found_offset);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା add_object_entry_from_bitmap(ଅଚଳ ସଂରଚନା object_id *oid,
					ଗଣନା object_type type,
					ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags UNUSED, uint32_t name_hash,
					ସଂରଚନା packed_git *pack, off_t offset,
					ଶୂନ୍ୟ *payload UNUSED)
{
	display_progress(progress_state, ++nr_seen);

	ଯଦି (have_duplicate_entry(oid, 0))
		ଫେରନ୍ତୁ 0;

	ଯଦି (!want_object_in_pack(oid, 0, &pack, &offset))
		ଫେରନ୍ତୁ 0;

	create_object_entry(oid, type, name_hash, 0, 0, pack, offset);
	ଫେରନ୍ତୁ 0;
}

ସଂରଚନା pbase_ବୃକ୍ଷ_cache {
	ସଂରଚନା object_id oid;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ref;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା temporary;
	ଶୂନ୍ୟ *ବୃକ୍ଷ_data;
	unsigned long ବୃକ୍ଷ_size;
};

ସ୍ଥିର ସଂରଚନା pbase_ବୃକ୍ଷ_cache *(pbase_ବୃକ୍ଷ_cache[256]);
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା pbase_ବୃକ୍ଷ_cache_ix(ଅଚଳ ସଂରଚନା object_id *oid)
{
	ଫେରନ୍ତୁ oid->hash[0] % ARRAY_SIZE(pbase_ବୃକ୍ଷ_cache);
}
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା pbase_ବୃକ୍ଷ_cache_ix_incr(ପୂର୍ଣ୍ଣସଂଖ୍ୟା ix)
{
	ଫେରନ୍ତୁ (ix+1) % ARRAY_SIZE(pbase_ବୃକ୍ଷ_cache);
}

ସ୍ଥିର ସଂରଚନା pbase_ବୃକ୍ଷ {
	ସଂରଚନା pbase_ବୃକ୍ଷ *next;
	/* This is a phony "cache" entry; we are not
	 * going to evict it or find it through _get()
	 * mechanism -- this is for the toplevel node that
	 * would almost always change with any commit.
	 */
	ସଂରଚନା pbase_ବୃକ୍ଷ_cache pcache;
} *pbase_ବୃକ୍ଷ;

ସ୍ଥିର ସଂରଚନା pbase_ବୃକ୍ଷ_cache *pbase_ବୃକ୍ଷ_get(ଅଚଳ ସଂରଚନା object_id *oid)
{
	ସଂରଚନା pbase_ବୃକ୍ଷ_cache *ent, *nent;
	ଶୂନ୍ୟ *data;
	unsigned long size;
	size_t size_st = 0;
	ଗଣନା object_type type;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା neigh;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା my_ix = pbase_ବୃକ୍ଷ_cache_ix(oid);
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା available_ix = -1;

	/* pbase-tree-cache acts as a limited hashtable.
	 * your object will be found at your index or within a few
	 * slots after that slot if it is cached.
	 */
	ପାଇଁ (neigh = 0; neigh < 8; neigh++) {
		ent = pbase_ବୃକ୍ଷ_cache[my_ix];
		ଯଦି (ent && oideq(&ent->oid, oid)) {
			ent->ref++;
			ଫେରନ୍ତୁ ent;
		}
		ଅନ୍ୟଥା ଯଦି (((available_ix < 0) && (!ent || !ent->ref)) ||
			 ((0 <= available_ix) &&
			  (!ent && pbase_ବୃକ୍ଷ_cache[available_ix])))
			available_ix = my_ix;
		ଯଦି (!ent)
			ଭାଙ୍ଗ;
		my_ix = pbase_ବୃକ୍ଷ_cache_ix_incr(my_ix);
	}

	/* Did not find one.  Either we got a bogus request or
	 * we need to read and perhaps cache.
	 */
	data = odb_read_object(the_repository->objects, oid, &type, &size_st);
	size = cast_size_t_to_ulong(size_st);
	ଯଦି (!data)
		ଫେରନ୍ତୁ ଖାଲି;
	ଯଦି (type != OBJ_ବୃକ୍ଷ) {
		ମୁକ୍ତ(data);
		ଫେରନ୍ତୁ ଖାଲି;
	}

	/* We need to either cache or return a throwaway copy */

	ଯଦି (available_ix < 0)
		ent = ଖାଲି;
	ଅନ୍ୟଥା {
		ent = pbase_ବୃକ୍ଷ_cache[available_ix];
		my_ix = available_ix;
	}

	ଯଦି (!ent) {
		nent = xmalloc(ଆକାର(*nent));
		nent->temporary = (available_ix < 0);
	}
	ଅନ୍ୟଥା {
		/* evict and reuse */
		ମୁକ୍ତ(ent->ବୃକ୍ଷ_data);
		nent = ent;
	}
	oidcpy(&nent->oid, oid);
	nent->ବୃକ୍ଷ_data = data;
	nent->ବୃକ୍ଷ_size = size;
	nent->ref = 1;
	ଯଦି (!nent->temporary)
		pbase_ବୃକ୍ଷ_cache[my_ix] = nent;
	ଫେରନ୍ତୁ nent;
}

ସ୍ଥିର ଶୂନ୍ୟ pbase_ବୃକ୍ଷ_put(ସଂରଚନା pbase_ବୃକ୍ଷ_cache *cache)
{
	ଯଦି (!cache->temporary) {
		cache->ref--;
		ଫେରନ୍ତୁ;
	}
	ମୁକ୍ତ(cache->ବୃକ୍ଷ_data);
	ମୁକ୍ତ(cache);
}

ସ୍ଥିର size_t name_cmp_len(ଅଚଳ ଅକ୍ଷର *name)
{
	ଫେରନ୍ତୁ strcspn(name, "\n/");
}

ସ୍ଥିର ଶୂନ୍ୟ add_pbase_object(ସଂରଚନା ବୃକ୍ଷ_desc *ବୃକ୍ଷ,
			     ଅଚଳ ଅକ୍ଷର *name,
			     size_t cmplen,
			     ଅଚଳ ଅକ୍ଷର *fullname)
{
	ସଂରଚନା name_entry entry;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp;

	ଯେତେବେଳେ (ବୃକ୍ଷ_entry(ବୃକ୍ଷ,&entry)) {
		ଯଦି (S_ISGITLINK(entry.mode))
			ଜାରି;
		cmp = ବୃକ୍ଷ_entry_len(&entry) != cmplen ? 1 :
		      memcmp(name, entry.path, cmplen);
		ଯଦି (cmp > 0)
			ଜାରି;
		ଯଦି (cmp < 0)
			ଫେରନ୍ତୁ;
		ଯଦି (name[cmplen] != '/') {
			add_object_entry(&entry.oid,
					 object_type(entry.mode),
					 fullname, 1);
			ଫେରନ୍ତୁ;
		}
		ଯଦି (S_ISDIR(entry.mode)) {
			ସଂରଚନା ବୃକ୍ଷ_desc sub;
			ସଂରଚନା pbase_ବୃକ୍ଷ_cache *ବୃକ୍ଷ;
			ଅଚଳ ଅକ୍ଷର *down = name+cmplen+1;
			size_t downlen = name_cmp_len(down);

			ବୃକ୍ଷ = pbase_ବୃକ୍ଷ_get(&entry.oid);
			ଯଦି (!ବୃକ୍ଷ)
				ଫେରନ୍ତୁ;
			init_ବୃକ୍ଷ_desc(&sub, &ବୃକ୍ଷ->oid,
				       ବୃକ୍ଷ->ବୃକ୍ଷ_data, ବୃକ୍ଷ->ବୃକ୍ଷ_size);

			add_pbase_object(&sub, down, downlen, fullname);
			pbase_ବୃକ୍ଷ_put(ବୃକ୍ଷ);
		}
	}
}

ସ୍ଥିର unsigned *done_pbase_paths;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା done_pbase_paths_ଅଙ୍କ;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା done_pbase_paths_alloc;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା done_pbase_path_pos(unsigned hash)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା lo = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା hi = done_pbase_paths_ଅଙ୍କ;
	ଯେତେବେଳେ (lo < hi) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା mi = lo + (hi - lo) / 2;
		ଯଦି (done_pbase_paths[mi] == hash)
			ଫେରନ୍ତୁ mi;
		ଯଦି (done_pbase_paths[mi] < hash)
			hi = mi;
		ଅନ୍ୟଥା
			lo = mi + 1;
	}
	ଫେରନ୍ତୁ -lo-1;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା check_pbase_path(unsigned hash)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା pos = done_pbase_path_pos(hash);
	ଯଦି (0 <= pos)
		ଫେରନ୍ତୁ 1;
	pos = -pos - 1;
	ALLOC_GROW(done_pbase_paths,
		   done_pbase_paths_ଅଙ୍କ + 1,
		   done_pbase_paths_alloc);
	done_pbase_paths_ଅଙ୍କ++;
	ଯଦି (pos < done_pbase_paths_ଅଙ୍କ)
		MOVE_ARRAY(done_pbase_paths + pos + 1, done_pbase_paths + pos,
			   done_pbase_paths_ଅଙ୍କ - pos - 1);
	done_pbase_paths[pos] = hash;
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ add_preferred_base_object(ଅଚଳ ଅକ୍ଷର *name)
{
	ସଂରଚନା pbase_ବୃକ୍ଷ *it;
	size_t cmplen;
	unsigned hash = pack_name_hash_fn(name);

	ଯଦି (!ଅଙ୍କ_preferred_base || check_pbase_path(hash))
		ଫେରନ୍ତୁ;

	cmplen = name_cmp_len(name);
	ପାଇଁ (it = pbase_ବୃକ୍ଷ; it; it = it->next) {
		ଯଦି (cmplen == 0) {
			add_object_entry(&it->pcache.oid, OBJ_ବୃକ୍ଷ, ଖାଲି, 1);
		}
		ଅନ୍ୟଥା {
			ସଂରଚନା ବୃକ୍ଷ_desc ବୃକ୍ଷ;
			init_ବୃକ୍ଷ_desc(&ବୃକ୍ଷ, &it->pcache.oid,
				       it->pcache.ବୃକ୍ଷ_data, it->pcache.ବୃକ୍ଷ_size);
			add_pbase_object(&ବୃକ୍ଷ, name, cmplen, name);
		}
	}
}

ସ୍ଥିର ଶୂନ୍ୟ add_preferred_base(ସଂରଚନା object_id *oid)
{
	ସଂରଚନା pbase_ବୃକ୍ଷ *it;
	ଶୂନ୍ୟ *data;
	unsigned long size;
	size_t size_st = 0;
	ସଂରଚନା object_id ବୃକ୍ଷ_oid;

	ଯଦି (window <= ଅଙ୍କ_preferred_base++)
		ଫେରନ୍ତୁ;

	data = odb_read_object_peeled(the_repository->objects, oid,
				      OBJ_ବୃକ୍ଷ, &size_st, &ବୃକ୍ଷ_oid);
	size = cast_size_t_to_ulong(size_st);
	ଯଦି (!data)
		ଫେରନ୍ତୁ;

	ପାଇଁ (it = pbase_ବୃକ୍ଷ; it; it = it->next) {
		ଯଦି (oideq(&it->pcache.oid, &ବୃକ୍ଷ_oid)) {
			ମୁକ୍ତ(data);
			ଫେରନ୍ତୁ;
		}
	}

	CALLOC_ARRAY(it, 1);
	it->next = pbase_ବୃକ୍ଷ;
	pbase_ବୃକ୍ଷ = it;

	oidcpy(&it->pcache.oid, &ବୃକ୍ଷ_oid);
	it->pcache.ବୃକ୍ଷ_data = data;
	it->pcache.ବୃକ୍ଷ_size = size;
}

ସ୍ଥିର ଶୂନ୍ୟ cleanup_preferred_base(ଶୂନ୍ୟ)
{
	ସଂରଚନା pbase_ବୃକ୍ଷ *it;
	unsigned i;

	it = pbase_ବୃକ୍ଷ;
	pbase_ବୃକ୍ଷ = ଖାଲି;
	ଯେତେବେଳେ (it) {
		ସଂରଚନା pbase_ବୃକ୍ଷ *tmp = it;
		it = tmp->next;
		ମୁକ୍ତ(tmp->pcache.ବୃକ୍ଷ_data);
		ମୁକ୍ତ(tmp);
	}

	ପାଇଁ (i = 0; i < ARRAY_SIZE(pbase_ବୃକ୍ଷ_cache); i++) {
		ଯଦି (!pbase_ବୃକ୍ଷ_cache[i])
			ଜାରି;
		ମୁକ୍ତ(pbase_ବୃକ୍ଷ_cache[i]->ବୃକ୍ଷ_data);
		FREE_AND_NULL(pbase_ବୃକ୍ଷ_cache[i]);
	}

	FREE_AND_NULL(done_pbase_paths);
	done_pbase_paths_ଅଙ୍କ = done_pbase_paths_alloc = 0;
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
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା can_reuse_delta(ଅଚଳ ସଂରଚନା object_id *base_oid,
			   ସଂରଚନା object_entry *delta,
			   ସଂରଚନା object_entry **base_out)
{
	ସଂରଚନା object_entry *base;

	/*
	 * First see if we're already sending the base (or it's explicitly in
	 * our "excluded" list).
	 */
	base = packlist_find(&to_pack, base_oid);
	ଯଦି (base) {
		ଯଦି (!in_same_island(&delta->idx.oid, &base->idx.oid))
			ଫେରନ୍ତୁ 0;
		*base_out = base;
		ଫେରନ୍ତୁ 1;
	}

	/*
	 * Otherwise, reachability bitmaps may tell us if the receiver has it,
	 * even if it was buried too deep in history to make it into the
	 * packing list.
	 */
	ଯଦି (thin && bitmap_has_oid_in_uninteresting(bitmap_git, base_oid)) {
		ଯଦି (use_delta_islands) {
			ଯଦି (!in_same_island(&delta->idx.oid, base_oid))
				ଫେରନ୍ତୁ 0;
		}
		*base_out = ଖାଲି;
		ଫେରନ୍ତୁ 1;
	}

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ prefetch_to_pack(uint32_t object_index_start) {
	ସଂରଚନା oid_array to_fetch = OID_ARRAY_INIT;
	uint32_t i;

	ପାଇଁ (i = object_index_start; i < to_pack.nr_objects; i++) {
		ସଂରଚନା object_entry *entry = to_pack.objects + i;

		ଯଦି (!odb_read_object_info_extended(the_repository->objects,
						   &entry->idx.oid,
						   ଖାଲି,
						   OBJECT_INFO_FOR_PREFETCH))
			ଜାରି;
		oid_array_append(&to_fetch, &entry->idx.oid);
	}
	promisor_remote_get_direct(the_repository,
				   to_fetch.oid, to_fetch.nr);
	oid_array_clear(&to_fetch);
}

ସ୍ଥିର ଶୂନ୍ୟ check_object(ସଂରଚନା object_entry *entry, uint32_t object_index)
{
	size_t canonical_size;
	ଗଣନା object_type type;
	ସଂରଚନା object_info oi = {.typep = &type, .sizep = &canonical_size};

	ଯଦି (IN_PACK(entry)) {
		ସଂରଚନା packed_git *p = IN_PACK(entry);
		ସଂରଚନା pack_window *w_curs = ଖାଲି;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା have_base = 0;
		ସଂରଚନା object_id base_ref;
		ସଂରଚନା object_entry *base_entry;
		size_t used, used_0, avail;
		off_t ofs;
		unsigned ଅକ୍ଷର *buf, c;
		ଗଣନା object_type type;
		size_t in_pack_size;

		buf = use_pack(p, &w_curs, entry->in_pack_offset, &avail);

		/*
		 * We want in_pack_type even if we do not reuse delta
		 * since non-delta representations could still be reused.
		 */
		used = unpack_object_header_buffer(buf, avail,
						   &type,
						   &in_pack_size);
		ଯଦି (used == 0)
			ଯାଅ give_up;

		ଯଦି (type < 0)
			BUG("invalid type %d", type);
		entry->in_pack_type = type;

		/*
		 * Determine if this is a delta and if so whether we can
		 * reuse it or not.  Otherwise let's find out as cheaply as
		 * possible what the actual type and size for this object is.
		 */
		ବାଛ (entry->in_pack_type) {
		ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
			/* Not a delta hence we've already got all we need. */
			oe_set_type(entry, entry->in_pack_type);
			SET_SIZE(entry, in_pack_size);
			entry->in_pack_header_size = used;
			ଯଦି (oe_type(entry) < OBJ_COMMIT || oe_type(entry) > OBJ_BLOB)
				ଯାଅ give_up;
			unuse_pack(&w_curs);
			ଫେରନ୍ତୁ;
		ସ୍ଥିତି OBJ_REF_DELTA:
			ଯଦି (reuse_delta && !entry->preferred_base) {
				oidread(&base_ref,
					use_pack(p, &w_curs,
						 entry->in_pack_offset + used,
						 ଖାଲି),
					the_repository->hash_algo);
				have_base = 1;
			}
			entry->in_pack_header_size = used + the_hash_algo->rawsz;
			ଭାଙ୍ଗ;
		ସ୍ଥିତି OBJ_OFS_DELTA:
			buf = use_pack(p, &w_curs,
				       entry->in_pack_offset + used, ଖାଲି);
			used_0 = 0;
			c = buf[used_0++];
			ofs = c & 127;
			ଯେତେବେଳେ (c & 128) {
				ofs += 1;
				ଯଦି (!ofs || MSB(ofs, 7)) {
					error(_("delta base offset overflow in pack for %s"),
					      oid_to_hex(&entry->idx.oid));
					ଯାଅ give_up;
				}
				c = buf[used_0++];
				ofs = (ofs << 7) + (c & 127);
			}
			ofs = entry->in_pack_offset - ofs;
			ଯଦି (ofs <= 0 || ofs >= entry->in_pack_offset) {
				error(_("delta base offset out of bound for %s"),
				      oid_to_hex(&entry->idx.oid));
				ଯାଅ give_up;
			}
			ଯଦି (reuse_delta && !entry->preferred_base) {
				uint32_t pos;
				ଯଦି (offset_to_pack_pos(p, ofs, &pos) < 0)
					ଯାଅ give_up;
				ଯଦି (!nth_packed_object_id(&base_ref, p,
							  pack_pos_to_index(p, pos)))
					have_base = 1;
			}
			entry->in_pack_header_size = used + used_0;
			ଭାଙ୍ଗ;
		}

		ଯଦି (have_base &&
		    can_reuse_delta(&base_ref, entry, &base_entry)) {
			oe_set_type(entry, entry->in_pack_type);
			SET_SIZE(entry, in_pack_size); /* delta size */
			SET_DELTA_SIZE(entry, in_pack_size);

			ଯଦି (base_entry) {
				SET_DELTA(entry, base_entry);
				entry->delta_sibling_idx = base_entry->delta_child_idx;
				SET_DELTA_CHILD(base_entry, entry);
			} ଅନ୍ୟଥା {
				SET_DELTA_EXT(entry, &base_ref);
			}

			unuse_pack(&w_curs);
			ଫେରନ୍ତୁ;
		}

		ଯଦି (oe_type(entry)) {
			off_t delta_pos;

			/*
			 * This must be a delta and we already know what the
			 * final object type is.  Let's extract the actual
			 * object size from the delta header.
			 */
			delta_pos = entry->in_pack_offset + entry->in_pack_header_size;
			canonical_size = get_size_from_delta(p, &w_curs,
							     delta_pos);
			ଯଦି (canonical_size == 0)
				ଯାଅ give_up;
			SET_SIZE(entry, canonical_size);
			unuse_pack(&w_curs);
			ଫେରନ୍ତୁ;
		}

		/*
		 * No choice but to fall back to the recursive delta walk
		 * with odb_read_object_info() to find about the object type
		 * at this point...
		 */
		give_up:
		unuse_pack(&w_curs);
	}

	ଯଦି (odb_read_object_info_extended(the_repository->objects, &entry->idx.oid, &oi,
					  OBJECT_INFO_SKIP_FETCH_OBJECT | OBJECT_INFO_LOOKUP_REPLACE) < 0) {
		ଯଦି (repo_has_promisor_remote(the_repository)) {
			prefetch_to_pack(object_index);
			ଯଦି (odb_read_object_info_extended(the_repository->objects, &entry->idx.oid, &oi,
							  OBJECT_INFO_SKIP_FETCH_OBJECT | OBJECT_INFO_LOOKUP_REPLACE) < 0)
				type = -1;
		} ଅନ୍ୟଥା {
			type = -1;
		}
	}
	oe_set_type(entry, type);
	ଯଦି (entry->type_valid) {
		SET_SIZE(entry, canonical_size);
	} ଅନ୍ୟଥା {
		/*
		 * Bad object type is checked in prepare_pack().  This is
		 * to permit a missing preferred base object to be ignored
		 * as a preferred base.  Doing so can result in a larger
		 * pack file, but the transfer will still take place.
		 */
	}
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା pack_offset_sort(ଅଚଳ ଶୂନ୍ୟ *_a, ଅଚଳ ଶୂନ୍ୟ *_b)
{
	ଅଚଳ ସଂରଚନା object_entry *a = *(ସଂରଚନା object_entry **)_a;
	ଅଚଳ ସଂରଚନା object_entry *b = *(ସଂରଚନା object_entry **)_b;
	ଅଚଳ ସଂରଚନା packed_git *a_in_pack = IN_PACK(a);
	ଅଚଳ ସଂରଚନା packed_git *b_in_pack = IN_PACK(b);

	/* avoid filesystem trashing with loose objects */
	ଯଦି (!a_in_pack && !b_in_pack)
		ଫେରନ୍ତୁ oidcmp(&a->idx.oid, &b->idx.oid);

	ଯଦି (a_in_pack < b_in_pack)
		ଫେରନ୍ତୁ -1;
	ଯଦି (a_in_pack > b_in_pack)
		ଫେରନ୍ତୁ 1;
	ଫେରନ୍ତୁ a->in_pack_offset < b->in_pack_offset ? -1 :
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
ସ୍ଥିର ଶୂନ୍ୟ drop_reused_delta(ସଂରଚନା object_entry *entry)
{
	unsigned *idx = &to_pack.objects[entry->delta_idx - 1].delta_child_idx;
	ସଂରଚନା object_info oi = OBJECT_INFO_INIT;
	ଗଣନା object_type type;
	size_t size;

	ଯେତେବେଳେ (*idx) {
		ସଂରଚନା object_entry *oe = &to_pack.objects[*idx - 1];

		ଯଦି (oe == entry)
			*idx = oe->delta_sibling_idx;
		ଅନ୍ୟଥା
			idx = &oe->delta_sibling_idx;
	}
	SET_DELTA(entry, ଖାଲି);
	entry->depth = 0;

	oi.sizep = &size;
	oi.typep = &type;
	ଯଦି (packed_object_info(ଖାଲି, IN_PACK(entry), entry->in_pack_offset, &oi) < 0) {
		/*
		 * We failed to get the info from this pack for some reason;
		 * fall back to odb_read_object_info, which may find another copy.
		 * And if that fails, the error will be recorded in oe_type(entry)
		 * and dealt with in prepare_pack().
		 */
		oe_set_type(entry,
			    odb_read_object_info(the_repository->objects,
						 &entry->idx.oid, &size));
	} ଅନ୍ୟଥା {
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
ସ୍ଥିର ଶୂନ୍ୟ break_delta_chains(ସଂରଚନା object_entry *entry)
{
	/*
	 * The actual depth of each object we will write is stored as an int,
	 * as it cannot exceed our int "depth" limit. But before we break
	 * changes based no that limit, we may potentially go as deep as the
	 * number of objects, which is elsewhere bounded to a uint32_t.
	 */
	uint32_t total_depth;
	ସଂରଚନା object_entry *cur, *next;

	ପାଇଁ (cur = entry, total_depth = 0;
	     cur;
	     cur = DELTA(cur), total_depth++) {
		ଯଦି (cur->dfs_state == DFS_DONE) {
			/*
			 * We've already seen this object and know it isn't
			 * part of a cycle. We do need to append its depth
			 * to our count.
			 */
			total_depth += cur->depth;
			ଭାଙ୍ଗ;
		}

		/*
		 * We break cycles before looping, so an ACTIVE state (or any
		 * other cruft which made its way into the state variable)
		 * is a bug.
		 */
		ଯଦି (cur->dfs_state != DFS_NONE)
			BUG("confusing delta dfs state in first pass: %d",
			    cur->dfs_state);

		/*
		 * Now we know this is the first time we've seen the object. If
		 * it's not a delta, we're done traversing, but we'll mark it
		 * done to save time on future traversals.
		 */
		ଯଦି (!DELTA(cur)) {
			cur->dfs_state = DFS_DONE;
			ଭାଙ୍ଗ;
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
		ଯଦି (DELTA(cur)->dfs_state == DFS_ACTIVE) {
			drop_reused_delta(cur);
			cur->dfs_state = DFS_DONE;
			ଭାଙ୍ଗ;
		}
	}

	/*
	 * And now that we've gone all the way to the bottom of the chain, we
	 * need to clear the active flags and set the depth fields as
	 * appropriate. Unlike the loop above, which can quit when it drops a
	 * delta, we need to keep going to look for more depth cuts. So we need
	 * an extra "next" pointer to keep going after we reset cur->delta.
	 */
	ପାଇଁ (cur = entry; cur; cur = next) {
		next = DELTA(cur);

		/*
		 * We should have a chain of zero or more ACTIVE states down to
		 * a final DONE. We can quit after the DONE, because either it
		 * has no bases, or we've already handled them in a previous
		 * call.
		 */
		ଯଦି (cur->dfs_state == DFS_DONE)
			ଭାଙ୍ଗ;
		ଅନ୍ୟଥା ଯଦି (cur->dfs_state != DFS_ACTIVE)
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
		ଯଦି (!cur->depth)
			drop_reused_delta(cur);

		cur->dfs_state = DFS_DONE;
	}
}

ସ୍ଥିର ଶୂନ୍ୟ get_object_details(ଶୂନ୍ୟ)
{
	uint32_t i;
	ସଂରଚନା object_entry **sorted_by_offset;

	ଯଦି (progress)
		progress_state = start_progress(the_repository,
						_("Counting objects"),
						to_pack.nr_objects);

	CALLOC_ARRAY(sorted_by_offset, to_pack.nr_objects);
	ପାଇଁ (i = 0; i < to_pack.nr_objects; i++)
		sorted_by_offset[i] = to_pack.objects + i;
	QSORT(sorted_by_offset, to_pack.nr_objects, pack_offset_sort);

	ପାଇଁ (i = 0; i < to_pack.nr_objects; i++) {
		ସଂରଚନା object_entry *entry = sorted_by_offset[i];
		check_object(entry, i);
		ଯଦି (entry->type_valid &&
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
	ପାଇଁ (i = 0; i < to_pack.nr_objects; i++)
		break_delta_chains(&to_pack.objects[i]);

	ମୁକ୍ତ(sorted_by_offset);
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
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା type_size_sort(ଅଚଳ ଶୂନ୍ୟ *_a, ଅଚଳ ଶୂନ୍ୟ *_b)
{
	ଅଚଳ ସଂରଚନା object_entry *a = *(ସଂରଚନା object_entry **)_a;
	ଅଚଳ ସଂରଚନା object_entry *b = *(ସଂରଚନା object_entry **)_b;
	ଅଚଳ ଗଣନା object_type a_type = oe_type(a);
	ଅଚଳ ଗଣନା object_type b_type = oe_type(b);
	ଅଚଳ unsigned long a_size = SIZE(a);
	ଅଚଳ unsigned long b_size = SIZE(b);

	ଯଦି (a_type > b_type)
		ଫେରନ୍ତୁ -1;
	ଯଦି (a_type < b_type)
		ଫେରନ୍ତୁ 1;
	ଯଦି (a->hash > b->hash)
		ଫେରନ୍ତୁ -1;
	ଯଦି (a->hash < b->hash)
		ଫେରନ୍ତୁ 1;
	ଯଦି (a->preferred_base > b->preferred_base)
		ଫେରନ୍ତୁ -1;
	ଯଦି (a->preferred_base < b->preferred_base)
		ଫେରନ୍ତୁ 1;
	ଯଦି (use_delta_islands) {
		ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା island_cmp = island_delta_cmp(&a->idx.oid, &b->idx.oid);
		ଯଦି (island_cmp)
			ଫେରନ୍ତୁ island_cmp;
	}
	ଯଦି (a_size > b_size)
		ଫେରନ୍ତୁ -1;
	ଯଦି (a_size < b_size)
		ଫେରନ୍ତୁ 1;
	ଫେରନ୍ତୁ a < b ? -1 : (a > b);  /* newest first */
}

ସଂରଚନା unpacked {
	ସଂରଚନା object_entry *entry;
	ଶୂନ୍ୟ *data;
	ସଂରଚନା delta_index *index;
	unsigned depth;
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା delta_cacheable(size_t src_size, size_t trg_size,
			   size_t delta_size)
{
	ଯଦି (max_delta_cache_size && delta_cache_size + delta_size > max_delta_cache_size)
		ଫେରନ୍ତୁ 0;

	ଯଦି (delta_size < cache_max_small_delta_size)
		ଫେରନ୍ତୁ 1;

	/* cache delta, if objects are large enough compared to delta size */
	ଯଦି ((src_size >> 20) + (trg_size >> 21) > (delta_size >> 10))
		ଫେରନ୍ତୁ 1;

	ଫେରନ୍ତୁ 0;
}

/* Protect delta_cache_size */
ସ୍ଥିର pthread_mutex_t cache_mutex;
#define cache_lock()		pthread_mutex_lock(&cache_mutex)
#define cache_unlock()		pthread_mutex_unlock(&cache_mutex)

/*
 * Protect object list partitioning (e.g. struct thread_param) and
 * progress_state
 */
ସ୍ଥିର pthread_mutex_t progress_mutex;
#define progress_lock()		pthread_mutex_lock(&progress_mutex)
#define progress_unlock()	pthread_mutex_unlock(&progress_mutex)

/*
 * Access to struct object_entry is unprotected since each thread owns
 * a portion of the main object list. Just don't access object entries
 * ahead in the list because they can be stolen and would need
 * progress_mutex for protection.
 */

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା oe_size_less_than(ସଂରଚନା packing_data *pack,
				    ଅଚଳ ସଂରଚନା object_entry *lhs,
				    size_t rhs)
{
	ଯଦି (lhs->size_valid)
		ଫେରନ୍ତୁ lhs->size_ < rhs;
	ଯଦି (rhs < pack->oe_size_limit) /* rhs < 2^x <= lhs ? */
		ଫେରନ୍ତୁ 0;
	ଫେରନ୍ତୁ oe_get_size_slow(pack, lhs) < rhs;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ oe_set_ବୃକ୍ଷ_depth(ସଂରଚନା packing_data *pack,
				     ସଂରଚନା object_entry *e,
				     unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ବୃକ୍ଷ_depth)
{
	ଯଦି (!pack->ବୃକ୍ଷ_depth)
		CALLOC_ARRAY(pack->ବୃକ୍ଷ_depth, pack->nr_alloc);
	pack->ବୃକ୍ଷ_depth[e - pack->objects] = ବୃକ୍ଷ_depth;
}

ସ୍ଥିର ଶୂନ୍ୟ record_ବୃକ୍ଷ_depth(ଅଚଳ ସଂରଚନା object_id *oid, ଅଚଳ ଅକ୍ଷର *name)
{
	ଅଚଳ ଅକ୍ଷର *p;
	unsigned depth;
	ସଂରଚନା object_entry *ent;

	/* the empty string is a root tree, which is depth 0 */
	depth = *name ? 1 : 0;
	ପାଇଁ (p = strchr(name, '/'); p; p = strchr(p + 1, '/'))
		depth++;

	ent = packlist_find(&to_pack, oid);
	ଯଦି (ent && depth > oe_ବୃକ୍ଷ_depth(&to_pack, ent))
		oe_set_ବୃକ୍ଷ_depth(&to_pack, ent, depth);
}

/*
 * Return the size of the object without doing any delta
 * reconstruction (so non-deltas are true object sizes, but deltas
 * return the size of the delta data).
 */
size_t oe_get_size_slow(ସଂରଚନା packing_data *pack,
			ଅଚଳ ସଂରଚନା object_entry *e)
{
	ସଂରଚନା packed_git *p;
	ସଂରଚନା pack_window *w_curs;
	unsigned ଅକ୍ଷର *buf;
	ଗଣନା object_type type;
	size_t used, avail, size;

	ଯଦି (e->type_ != OBJ_OFS_DELTA && e->type_ != OBJ_REF_DELTA) {
		size_t sz;
		packing_data_lock(&to_pack);
		ଯଦି (odb_read_object_info(the_repository->objects,
					 &e->idx.oid, &sz) < 0)
			die(_("unable to get size of %s"),
			    oid_to_hex(&e->idx.oid));
		packing_data_unlock(&to_pack);
		ଫେରନ୍ତୁ sz;
	}

	p = oe_in_pack(pack, e);
	ଯଦି (!p)
		BUG("when e->type is a delta, it must belong to a pack");

	packing_data_lock(&to_pack);
	w_curs = ଖାଲି;
	buf = use_pack(p, &w_curs, e->in_pack_offset, &avail);
	used = unpack_object_header_buffer(buf, avail, &type, &size);
	ଯଦି (used == 0)
		die(_("unable to parse object header of %s"),
		    oid_to_hex(&e->idx.oid));

	unuse_pack(&w_curs);
	packing_data_unlock(&to_pack);
	ଫେରନ୍ତୁ size;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା try_delta(ସଂରଚନା unpacked *trg, ସଂରଚନା unpacked *src,
		     unsigned max_depth, size_t *mem_usage)
{
	ସଂରଚନା object_entry *trg_entry = trg->entry;
	ସଂରଚନା object_entry *src_entry = src->entry;
	unsigned long trg_size, src_size, sizediff, max_size, sz;
	size_t delta_size;
	unsigned ref_depth;
	ଗଣନା object_type type;
	ଶୂନ୍ୟ *delta_buf;

	/* Don't bother doing diffs between different types */
	ଯଦି (oe_type(trg_entry) != oe_type(src_entry))
		ଫେରନ୍ତୁ -1;

	/*
	 * We do not bother to try a delta that we discarded on an
	 * earlier try, but only when reusing delta data.  Note that
	 * src_entry that is marked as the preferred_base should always
	 * be considered, as even if we produce a suboptimal delta against
	 * it, we will still save the transfer cost, as we already know
	 * the other side has it and we won't send src_entry at all.
	 */
	ଯଦି (reuse_delta && IN_PACK(trg_entry) &&
	    IN_PACK(trg_entry) == IN_PACK(src_entry) &&
	    !src_entry->preferred_base &&
	    trg_entry->in_pack_type != OBJ_REF_DELTA &&
	    trg_entry->in_pack_type != OBJ_OFS_DELTA)
		ଫେରନ୍ତୁ 0;

	/* Let's not bust the allowed depth. */
	ଯଦି (src->depth >= max_depth)
		ଫେରନ୍ତୁ 0;

	/* Now some size filtering heuristics. */
	trg_size = SIZE(trg_entry);
	ଯଦି (!DELTA(trg_entry)) {
		max_size = trg_size/2 - the_hash_algo->rawsz;
		ref_depth = 1;
	} ଅନ୍ୟଥା {
		max_size = DELTA_SIZE(trg_entry);
		ref_depth = trg->depth;
	}
	max_size = (uint64_t)max_size * (max_depth - src->depth) /
						(max_depth - ref_depth + 1);
	ଯଦି (max_size == 0)
		ଫେରନ୍ତୁ 0;
	src_size = SIZE(src_entry);
	sizediff = src_size < trg_size ? trg_size - src_size : 0;
	ଯଦି (sizediff >= max_size)
		ଫେରନ୍ତୁ 0;
	ଯଦି (trg_size < src_size / 32)
		ଫେରନ୍ତୁ 0;

	ଯଦି (!in_same_island(&trg->entry->idx.oid, &src->entry->idx.oid))
		ଫେରନ୍ତୁ 0;

	/* Load data if not already done */
	ଯଦି (!trg->data) {
		size_t sz_st = 0;
		packing_data_lock(&to_pack);
		trg->data = odb_read_object(the_repository->objects,
					    &trg_entry->idx.oid, &type,
					    &sz_st);
		sz = cast_size_t_to_ulong(sz_st);
		packing_data_unlock(&to_pack);
		ଯଦି (!trg->data)
			die(_("object %s cannot be read"),
			    oid_to_hex(&trg_entry->idx.oid));
		ଯଦି (sz != trg_size)
			die(_("object %s inconsistent object length (%"PRIuMAX" vs %"PRIuMAX")"),
			    oid_to_hex(&trg_entry->idx.oid), (uintmax_t)sz,
			    (uintmax_t)trg_size);
		*mem_usage += sz;
	}
	ଯଦି (!src->data) {
		size_t sz_st = 0;
		packing_data_lock(&to_pack);
		src->data = odb_read_object(the_repository->objects,
					    &src_entry->idx.oid, &type,
					    &sz_st);
		sz = cast_size_t_to_ulong(sz_st);
		packing_data_unlock(&to_pack);
		ଯଦି (!src->data) {
			ଯଦି (src_entry->preferred_base) {
				ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା warned = 0;
				ଯଦି (!warned++)
					warning(_("object %s cannot be read"),
						oid_to_hex(&src_entry->idx.oid));
				/*
				 * Those objects are not included in the
				 * resulting pack.  Be resilient and ignore
				 * them if they can't be read, in case the
				 * pack could be created nevertheless.
				 */
				ଫେରନ୍ତୁ 0;
			}
			die(_("object %s cannot be read"),
			    oid_to_hex(&src_entry->idx.oid));
		}
		ଯଦି (sz != src_size)
			die(_("object %s inconsistent object length (%"PRIuMAX" vs %"PRIuMAX")"),
			    oid_to_hex(&src_entry->idx.oid), (uintmax_t)sz,
			    (uintmax_t)src_size);
		*mem_usage += sz;
	}
	ଯଦି (!src->index) {
		src->index = create_delta_index(src->data, src_size);
		ଯଦି (!src->index) {
			ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା warned = 0;
			ଯଦି (!warned++)
				warning(_("suboptimal pack - out of memory"));
			ଫେରନ୍ତୁ 0;
		}
		*mem_usage += sizeof_delta_index(src->index);
	}

	delta_buf = create_delta(src->index, trg->data, trg_size, &delta_size, max_size);
	ଯଦି (!delta_buf)
		ଫେରନ୍ତୁ 0;

	ଯଦି (DELTA(trg_entry)) {
		/* Prefer only shallower same-sized deltas. */
		ଯଦି (delta_size == DELTA_SIZE(trg_entry) &&
		    src->depth + 1 >= trg->depth) {
			ମୁକ୍ତ(delta_buf);
			ଫେରନ୍ତୁ 0;
		}
	}

	/*
	 * Handle memory allocation outside of the cache
	 * accounting lock.  Compiler will optimize the strangeness
	 * away when NO_PTHREADS is defined.
	 */
	ମୁକ୍ତ(trg_entry->delta_data);
	cache_lock();
	ଯଦି (trg_entry->delta_data) {
		delta_cache_size -= DELTA_SIZE(trg_entry);
		trg_entry->delta_data = ଖାଲି;
	}
	ଯଦି (delta_cacheable(src_size, trg_size, delta_size)) {
		delta_cache_size += delta_size;
		cache_unlock();
		trg_entry->delta_data = xrealloc(delta_buf, delta_size);
	} ଅନ୍ୟଥା {
		cache_unlock();
		ମୁକ୍ତ(delta_buf);
	}

	SET_DELTA(trg_entry, src_entry);
	SET_DELTA_SIZE(trg_entry, delta_size);
	trg->depth = src->depth + 1;

	ଫେରନ୍ତୁ 1;
}

ସ୍ଥିର unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା check_delta_limit(ସଂରଚନା object_entry *me, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା n)
{
	ସଂରଚନା object_entry *child = DELTA_CHILD(me);
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା m = n;
	ଯେତେବେଳେ (child) {
		ଅଚଳ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା c = check_delta_limit(child, n + 1);
		ଯଦି (m < c)
			m = c;
		child = DELTA_SIBLING(child);
	}
	ଫେରନ୍ତୁ m;
}

ସ୍ଥିର size_t free_unpacked(ସଂରଚନା unpacked *n)
{
	size_t freed_mem = sizeof_delta_index(n->index);
	free_delta_index(n->index);
	n->index = ଖାଲି;
	ଯଦି (n->data) {
		freed_mem += SIZE(n->entry);
		FREE_AND_NULL(n->data);
	}
	n->entry = ଖାଲି;
	n->depth = 0;
	ଫେରନ୍ତୁ freed_mem;
}

ସ୍ଥିର ଶୂନ୍ୟ find_deltas(ସଂରଚନା object_entry **list, unsigned *list_size,
			ପୂର୍ଣ୍ଣସଂଖ୍ୟା window, ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth, unsigned *processed)
{
	uint32_t i, idx = 0, ଗଣନା_2 = 0;
	ସଂରଚନା unpacked *array;
	size_t mem_usage = 0;

	CALLOC_ARRAY(array, window);

	ପାଇଁ (;;) {
		ସଂରଚନା object_entry *entry;
		ସଂରଚନା unpacked *n = array + idx;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା j, max_depth, best_base = -1;

		progress_lock();
		ଯଦି (!*list_size) {
			progress_unlock();
			ଭାଙ୍ଗ;
		}
		entry = *list++;
		(*list_size)--;
		ଯଦି (!entry->preferred_base) {
			(*processed)++;
			display_progress(progress_state, *processed);
		}
		progress_unlock();

		mem_usage -= free_unpacked(n);
		n->entry = entry;

		ଯେତେବେଳେ (window_memory_limit &&
		       mem_usage > window_memory_limit &&
		       ଗଣନା_2 > 1) {
			ଅଚଳ uint32_t tail = (idx + window - ଗଣନା_2) % window;
			mem_usage -= free_unpacked(array + tail);
			ଗଣନା_2--;
		}

		/* We do not compute delta to *create* objects we are not
		 * going to pack.
		 */
		ଯଦି (entry->preferred_base)
			ଯାଅ next;

		/*
		 * If the current object is at pack edge, take the depth the
		 * objects that depend on the current object into account
		 * otherwise they would become too deep.
		 */
		max_depth = depth;
		ଯଦି (DELTA_CHILD(entry)) {
			max_depth -= check_delta_limit(entry, 0);
			ଯଦି (max_depth <= 0)
				ଯାଅ next;
		}

		j = window;
		ଯେତେବେଳେ (--j > 0) {
			ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
			uint32_t other_idx = idx + j;
			ସଂରଚନା unpacked *m;
			ଯଦି (other_idx >= window)
				other_idx -= window;
			m = array + other_idx;
			ଯଦି (!m->entry)
				ଭାଙ୍ଗ;
			ret = try_delta(n, m, max_depth, &mem_usage);
			ଯଦି (ret < 0)
				ଭାଙ୍ଗ;
			ଅନ୍ୟଥା ଯଦି (ret > 0)
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
		ଯଦି (entry->delta_data && !pack_to_stdout) {
			unsigned long size;

			size = do_compress(&entry->delta_data, DELTA_SIZE(entry));
			ଯଦି (size < (1U << OE_Z_DELTA_BITS)) {
				entry->z_delta_size = size;
				cache_lock();
				delta_cache_size -= DELTA_SIZE(entry);
				delta_cache_size += entry->z_delta_size;
				cache_unlock();
			} ଅନ୍ୟଥା {
				FREE_AND_NULL(entry->delta_data);
				entry->z_delta_size = 0;
			}
		}

		/* if we made n a delta, and if n is already at max
		 * depth, leaving it in the window is pointless.  we
		 * should evict it first.
		 */
		ଯଦି (DELTA(entry) && max_depth <= n->depth)
			ଜାରି;

		/*
		 * Move the best delta base up in the window, after the
		 * currently deltified object, to keep it longer.  It will
		 * be the first base object to be attempted next.
		 */
		ଯଦି (DELTA(entry)) {
			ସଂରଚନା unpacked swap = array[best_base];
			ପୂର୍ଣ୍ଣସଂଖ୍ୟା dist = (window + idx - best_base) % window;
			ପୂର୍ଣ୍ଣସଂଖ୍ୟା dst = best_base;
			ଯେତେବେଳେ (dist--) {
				ପୂର୍ଣ୍ଣସଂଖ୍ୟା src = (dst + 1) % window;
				array[dst] = array[src];
				dst = src;
			}
			array[dst] = swap;
		}

		next:
		idx++;
		ଯଦି (ଗଣନା_2 + 1 < window)
			ଗଣନା_2++;
		ଯଦି (idx >= window)
			idx = 0;
	}

	ପାଇଁ (i = 0; i < window; ++i) {
		free_delta_index(array[i].index);
		ମୁକ୍ତ(array[i].data);
	}
	ମୁକ୍ତ(array);
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

ସଂରଚନା thread_params {
	pthread_t thread;
	ସଂରଚନା object_entry **list;
	ସଂରଚନା packing_region *regions;
	unsigned list_size;
	unsigned remaining;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା window;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା working;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା data_ready;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	unsigned *processed;
};

ସ୍ଥିର pthread_cond_t progress_cond;

/*
 * Mutex and conditional variable can't be statically-initialized on Windows.
 */
ସ୍ଥିର ଶୂନ୍ୟ init_threaded_ସନ୍ଧାନ(ଶୂନ୍ୟ)
{
	pthread_mutex_init(&cache_mutex, ଖାଲି);
	pthread_mutex_init(&progress_mutex, ଖାଲି);
	pthread_cond_init(&progress_cond, ଖାଲି);
}

ସ୍ଥିର ଶୂନ୍ୟ cleanup_threaded_ସନ୍ଧାନ(ଶୂନ୍ୟ)
{
	pthread_cond_destroy(&progress_cond);
	pthread_mutex_destroy(&cache_mutex);
	pthread_mutex_destroy(&progress_mutex);
}

ସ୍ଥିର ଶୂନ୍ୟ *threaded_find_deltas(ଶୂନ୍ୟ *arg)
{
	ସଂରଚନା thread_params *me = arg;

	progress_lock();
	ଯେତେବେଳେ (me->remaining) {
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
		ଯେତେବେଳେ (!me->data_ready)
			pthread_cond_wait(&me->cond, &me->mutex);
		me->data_ready = 0;
		pthread_mutex_unlock(&me->mutex);

		progress_lock();
	}
	progress_unlock();
	/* leave ->working 1 so that this doesn't get more work assigned */
	ଫେରନ୍ତୁ ଖାଲି;
}

ସ୍ଥିର ଶୂନ୍ୟ ll_find_deltas(ସଂରଚନା object_entry **list, unsigned list_size,
			   ପୂର୍ଣ୍ଣସଂଖ୍ୟା window, ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth, unsigned *processed)
{
	ସଂରଚନା thread_params *p;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, ret, active_threads = 0;

	init_threaded_ସନ୍ଧାନ();

	ଯଦି (delta_ସନ୍ଧାନ_threads <= 1) {
		find_deltas(list, &list_size, window, depth, processed);
		cleanup_threaded_ସନ୍ଧାନ();
		ଫେରନ୍ତୁ;
	}
	ଯଦି (progress > pack_to_stdout)
		fprintf_ln(stderr, _("Delta compression using up to %d threads"),
			   delta_ସନ୍ଧାନ_threads);
	CALLOC_ARRAY(p, delta_ସନ୍ଧାନ_threads);

	/* Partition the work amongst work threads. */
	ପାଇଁ (i = 0; i < delta_ସନ୍ଧାନ_threads; i++) {
		unsigned sub_size = list_size / (delta_ସନ୍ଧାନ_threads - i);

		/* don't use too small segments or no deltas will be found */
		ଯଦି (sub_size < 2*window && i+1 < delta_ସନ୍ଧାନ_threads)
			sub_size = 0;

		p[i].window = window;
		p[i].depth = depth;
		p[i].processed = processed;
		p[i].working = 1;
		p[i].data_ready = 0;

		/* try to split chunks on "path" boundaries */
		ଯେତେବେଳେ (sub_size && sub_size < list_size &&
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
	ପାଇଁ (i = 0; i < delta_ସନ୍ଧାନ_threads; i++) {
		ଯଦି (!p[i].list_size)
			ଜାରି;
		pthread_mutex_init(&p[i].mutex, ଖାଲି);
		pthread_cond_init(&p[i].cond, ଖାଲି);
		ret = pthread_create(&p[i].thread, ଖାଲି,
				     threaded_find_deltas, &p[i]);
		ଯଦି (ret)
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
	ଯେତେବେଳେ (active_threads) {
		ସଂରଚନା thread_params *target = ଖାଲି;
		ସଂରଚନା thread_params *victim = ଖାଲି;
		unsigned sub_size = 0;

		progress_lock();
		ପାଇଁ (;;) {
			ପାଇଁ (i = 0; !target && i < delta_ସନ୍ଧାନ_threads; i++)
				ଯଦି (!p[i].working)
					target = &p[i];
			ଯଦି (target)
				ଭାଙ୍ଗ;
			pthread_cond_wait(&progress_cond, &progress_mutex);
		}

		ପାଇଁ (i = 0; i < delta_ସନ୍ଧାନ_threads; i++)
			ଯଦି (p[i].remaining > 2*window &&
			    (!victim || victim->remaining < p[i].remaining))
				victim = &p[i];
		ଯଦି (victim) {
			sub_size = victim->remaining / 2;
			list = victim->list + victim->list_size - sub_size;
			ଯେତେବେଳେ (sub_size && list[0]->hash &&
			       list[0]->hash == list[-1]->hash) {
				list++;
				sub_size--;
			}
			ଯଦି (!sub_size) {
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

		ଯଦି (!sub_size) {
			pthread_join(target->thread, ଖାଲି);
			pthread_cond_destroy(&target->cond);
			pthread_mutex_destroy(&target->mutex);
			active_threads--;
		}
	}
	cleanup_threaded_ସନ୍ଧାନ();
	ମୁକ୍ତ(p);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା obj_is_packed(ଅଚଳ ସଂରଚନା object_id *oid)
{
	ଫେରନ୍ତୁ packlist_find(&to_pack, oid) ||
		(reuse_packfile_bitmap &&
		 bitmap_walk_contains(bitmap_git, reuse_packfile_bitmap, oid));
}

ସ୍ଥିର ଶୂନ୍ୟ add_tag_chain(ଅଚଳ ସଂରଚନା object_id *oid)
{
	ସଂରଚନା tag *tag;

	/*
	 * We catch duplicates already in add_object_entry(), but we'd
	 * prefer to do this extra check to avoid having to parse the
	 * tag at all if we already know that it's being packed (e.g., if
	 * it was included via bitmaps, we would not have parsed it
	 * previously).
	 */
	ଯଦି (obj_is_packed(oid))
		ଫେରନ୍ତୁ;

	tag = lookup_tag(the_repository, oid);
	ଯେତେବେଳେ (1) {
		ଯଦି (!tag || parse_tag(the_repository, tag) || !tag->tagged)
			die(_("unable to pack objects reachable from tag %s"),
			    oid_to_hex(oid));

		add_object_entry(&tag->object.oid, OBJ_TAG, ଖାଲି, 0);

		ଯଦି (tag->tagged->type != OBJ_TAG)
			ଫେରନ୍ତୁ;

		tag = (ସଂରଚନା tag *)tag->tagged;
	}
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା add_ref_tag(ଅଚଳ ସଂରଚନା reference *ref, ଶୂନ୍ୟ *cb_data UNUSED)
{
	ସଂରଚନା object_id peeled;

	ଯଦି (!reference_get_peeled_oid(the_repository, ref, &peeled) &&
	    obj_is_packed(&peeled))
		add_tag_chain(ref->oid);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା should_attempt_deltas(ସଂରଚନା object_entry *entry)
{
	ଯଦି (DELTA(entry))
		/* This happens if we decided to reuse existing
		 * delta from a pack. "reuse_delta &&" is implied.
		 */
		ଫେରନ୍ତୁ 0;

	ଯଦି (!entry->type_valid ||
	    oe_size_less_than(&to_pack, entry, 50))
		ଫେରନ୍ତୁ 0;

	ଯଦି (entry->no_try_delta)
		ଫେରନ୍ତୁ 0;

	ଯଦି (!entry->preferred_base) {
		ଯଦି (oe_type(entry) < 0)
			die(_("unable to get type of object %s"),
				oid_to_hex(&entry->idx.oid));
	} ଅନ୍ୟଥା ଯଦି (oe_type(entry) < 0) {
		/*
		 * This object is not found, but we
		 * don't have to include it anyway.
		 */
		ଫେରନ୍ତୁ 0;
	}

	ଫେରନ୍ତୁ 1;
}

ସ୍ଥିର ଶୂନ୍ୟ find_deltas_for_region(ସଂରଚନା object_entry *list,
				   ସଂରଚନା packing_region *region,
				   unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *processed)
{
	ସଂରଚନା object_entry **delta_list;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା delta_list_nr = 0;

	ALLOC_ARRAY(delta_list, region->nr);
	ପାଇଁ (size_t i = 0; i < region->nr; i++) {
		ସଂରଚନା object_entry *entry = list + region->start + i;
		ଯଦି (should_attempt_deltas(entry))
			delta_list[delta_list_nr++] = entry;
	}

	QSORT(delta_list, delta_list_nr, type_size_sort);
	find_deltas(delta_list, &delta_list_nr, window, depth, processed);
	ମୁକ୍ତ(delta_list);
}

ସ୍ଥିର ଶୂନ୍ୟ find_deltas_by_region(ସଂରଚନା object_entry *list,
				  ସଂରଚନା packing_region *regions,
				  size_t start, size_t nr)
{
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା processed = 0;
	size_t progress_nr;

	ଯଦି (!nr)
		ଫେରନ୍ତୁ;

	progress_nr = regions[nr - 1].start + regions[nr - 1].nr;

	ଯଦି (progress)
		progress_state = start_progress(the_repository,
						_("Compressing objects by path"),
						progress_nr);

	ଯେତେବେଳେ (nr--)
		find_deltas_for_region(list,
				       &regions[start++],
				       &processed);

	display_progress(progress_state, progress_nr);
	stop_progress(&progress_state);
}

ସ୍ଥିର ଶୂନ୍ୟ *threaded_find_deltas_by_path(ଶୂନ୍ୟ *arg)
{
	ସଂରଚନା thread_params *me = arg;

	progress_lock();
	ଯେତେବେଳେ (me->remaining) {
		ଯେତେବେଳେ (me->remaining) {
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
		ଯେତେବେଳେ (!me->data_ready)
			pthread_cond_wait(&me->cond, &me->mutex);
		me->data_ready = 0;
		pthread_mutex_unlock(&me->mutex);

		progress_lock();
	}
	progress_unlock();
	/* leave ->working 1 so that this doesn't get more work assigned */
	ଫେରନ୍ତୁ ଖାଲି;
}

ସ୍ଥିର ଶୂନ୍ୟ ll_find_deltas_by_region(ସଂରଚନା object_entry *list,
				     ସଂରଚନା packing_region *regions,
				     uint32_t start, uint32_t nr)
{
	ସଂରଚନା thread_params *p;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, ret, active_threads = 0;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା processed = 0;
	uint32_t progress_nr;
	init_threaded_ସନ୍ଧାନ();

	ଯଦି (!nr)
		ଫେରନ୍ତୁ;

	progress_nr =  regions[nr - 1].start + regions[nr - 1].nr;
	ଯଦି (delta_ସନ୍ଧାନ_threads <= 1) {
		find_deltas_by_region(list, regions, start, nr);
		cleanup_threaded_ସନ୍ଧାନ();
		ଫେରନ୍ତୁ;
	}

	ଯଦି (progress > pack_to_stdout)
		fprintf_ln(stderr,
			   Q_("Path-based delta compression using up to %d thread",
			      "Path-based delta compression using up to %d threads",
			      delta_ସନ୍ଧାନ_threads),
			   delta_ସନ୍ଧାନ_threads);
	CALLOC_ARRAY(p, delta_ସନ୍ଧାନ_threads);

	ଯଦି (progress)
		progress_state = start_progress(the_repository,
						_("Compressing objects by path"),
						progress_nr);
	/* Partition the work amongst work threads. */
	ପାଇଁ (i = 0; i < delta_ସନ୍ଧାନ_threads; i++) {
		unsigned sub_size = nr / (delta_ସନ୍ଧାନ_threads - i);

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
	ପାଇଁ (i = 0; i < delta_ସନ୍ଧାନ_threads; i++) {
		ଯଦି (!p[i].list_size)
			ଜାରି;
		pthread_mutex_init(&p[i].mutex, ଖାଲି);
		pthread_cond_init(&p[i].cond, ଖାଲି);
		ret = pthread_create(&p[i].thread, ଖାଲି,
				     threaded_find_deltas_by_path, &p[i]);
		ଯଦି (ret)
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
	ଯେତେବେଳେ (active_threads) {
		ସଂରଚନା thread_params *target = ଖାଲି;
		ସଂରଚନା thread_params *victim = ଖାଲି;
		unsigned sub_size = 0;

		progress_lock();
		ପାଇଁ (;;) {
			ପାଇଁ (i = 0; !target && i < delta_ସନ୍ଧାନ_threads; i++)
				ଯଦି (!p[i].working)
					target = &p[i];
			ଯଦି (target)
				ଭାଙ୍ଗ;
			pthread_cond_wait(&progress_cond, &progress_mutex);
		}

		ପାଇଁ (i = 0; i < delta_ସନ୍ଧାନ_threads; i++)
			ଯଦି (p[i].remaining > 2*window &&
			    (!victim || victim->remaining < p[i].remaining))
				victim = &p[i];
		ଯଦି (victim) {
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

		ଯଦି (!sub_size) {
			pthread_join(target->thread, ଖାଲି);
			pthread_cond_destroy(&target->cond);
			pthread_mutex_destroy(&target->mutex);
			active_threads--;
		}
	}
	cleanup_threaded_ସନ୍ଧାନ();
	ମୁକ୍ତ(p);

	display_progress(progress_state, progress_nr);
	stop_progress(&progress_state);
}

ସ୍ଥିର ଶୂନ୍ୟ prepare_pack(ପୂର୍ଣ୍ଣସଂଖ୍ୟା window, ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth)
{
	ସଂରଚନା object_entry **delta_list;
	uint32_t i, nr_deltas;
	unsigned n;

	ଯଦି (use_delta_islands)
		resolve_ବୃକ୍ଷ_islands(the_repository, progress, &to_pack);

	get_object_details();

	/*
	 * If we're locally repacking then we need to be doubly careful
	 * from now on in order to make sure no stealth corruption gets
	 * propagated to the new pack.  Clients receiving streamed packs
	 * should validate everything they get anyway so no need to incur
	 * the additional cost here in that case.
	 */
	ଯଦି (!pack_to_stdout)
		do_check_packed_object_crc = 1;

	ଯଦି (!to_pack.nr_objects || !window || !depth)
		ଫେରନ୍ତୁ;

	ଯଦି (path_walk)
		ll_find_deltas_by_region(to_pack.objects, to_pack.regions,
					 0, to_pack.nr_regions);

	ALLOC_ARRAY(delta_list, to_pack.nr_objects);
	nr_deltas = n = 0;

	ପାଇଁ (i = 0; i < to_pack.nr_objects; i++) {
		ସଂରଚନା object_entry *entry = to_pack.objects + i;

		ଯଦି (!should_attempt_deltas(entry))
			ଜାରି;

		ଯଦି (!entry->preferred_base)
			nr_deltas++;

		delta_list[n++] = entry;
	}

	ଯଦି (nr_deltas && n > 1) {
		unsigned nr_done = 0;

		ଯଦି (progress)
			progress_state = start_progress(the_repository,
							_("Compressing objects"),
							nr_deltas);
		QSORT(delta_list, n, type_size_sort);
		ll_find_deltas(delta_list, n, window+1, depth, &nr_done);
		stop_progress(&progress_state);
		ଯଦି (nr_done != nr_deltas)
			die(_("inconsistency with delta count"));
	}
	ମୁକ୍ତ(delta_list);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା git_pack_config(ଅଚଳ ଅକ୍ଷର *k, ଅଚଳ ଅକ୍ଷର *v,
			   ଅଚଳ ସଂରଚନା config_context *ctx, ଶୂନ୍ୟ *cb)
{
	ଯଦି (!strcmp(k, "pack.window")) {
		window = git_config_int(k, v, ctx->kvi);
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (!strcmp(k, "pack.windowmemory")) {
		window_memory_limit = git_config_ulong(k, v, ctx->kvi);
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (!strcmp(k, "pack.depth")) {
		depth = git_config_int(k, v, ctx->kvi);
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (!strcmp(k, "pack.deltacachesize")) {
		max_delta_cache_size = git_config_size_t(k, v, ctx->kvi);
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (!strcmp(k, "pack.deltacachelimit")) {
		cache_max_small_delta_size = git_config_int(k, v, ctx->kvi);
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (!strcmp(k, "pack.writebitmaphashcache")) {
		ଯଦି (git_config_bool(k, v))
			write_bitmap_options |= BITMAP_OPT_HASH_CACHE;
		ଅନ୍ୟଥା
			write_bitmap_options &= ~BITMAP_OPT_HASH_CACHE;
	}

	ଯଦି (!strcmp(k, "pack.writebitmaplookuptable")) {
		ଯଦି (git_config_bool(k, v))
			write_bitmap_options |= BITMAP_OPT_LOOKUP_TABLE;
		ଅନ୍ୟଥା
			write_bitmap_options &= ~BITMAP_OPT_LOOKUP_TABLE;
	}

	ଯଦି (!strcmp(k, "pack.usebitmaps")) {
		use_bitmap_index_default = git_config_bool(k, v);
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (!strcmp(k, "pack.allowpackreuse")) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା res = git_parse_maybe_bool_text(v);
		ଯଦି (res < 0) {
			ଯଦି (!strcasecmp(v, "single"))
				allow_pack_reuse = SINGLE_PACK_REUSE;
			ଅନ୍ୟଥା ଯଦି (!strcasecmp(v, "multi"))
				allow_pack_reuse = MULTI_PACK_REUSE;
			ଅନ୍ୟଥା
				die(_("invalid pack.allowPackReuse value: '%s'"), v);
		} ଅନ୍ୟଥା ଯଦି (res) {
			allow_pack_reuse = SINGLE_PACK_REUSE;
		} ଅନ୍ୟଥା {
			allow_pack_reuse = NO_PACK_REUSE;
		}
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (!strcmp(k, "pack.threads")) {
		delta_ସନ୍ଧାନ_threads = git_config_int(k, v, ctx->kvi);
		ଯଦି (delta_ସନ୍ଧାନ_threads < 0)
			die(_("invalid number of threads specified (%d)"),
			    delta_ସନ୍ଧାନ_threads);
		ଯଦି (!HAVE_THREADS && delta_ସନ୍ଧାନ_threads != 1) {
			warning(_("no threads support, ignoring %s"), k);
			delta_ସନ୍ଧାନ_threads = 0;
		}
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (!strcmp(k, "pack.indexversion")) {
		pack_idx_opts.version = git_config_int(k, v, ctx->kvi);
		ଯଦି (pack_idx_opts.version > 2)
			die(_("bad pack.indexVersion=%"PRIu32),
			    pack_idx_opts.version);
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (!strcmp(k, "pack.writereverseindex")) {
		ଯଦି (git_config_bool(k, v))
			pack_idx_opts.flags |= WRITE_REV;
		ଅନ୍ୟଥା
			pack_idx_opts.flags &= ~WRITE_REV;
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (!strcmp(k, "uploadpack.blobpackfileuri")) {
		ସଂରଚନା configured_exclusion *ex;
		ଅଚଳ ଅକ୍ଷର *oid_end, *pack_end;
		/*
		 * Stores the pack hash. This is not a true object ID, but is
		 * of the same form.
		 */
		ସଂରଚନା object_id pack_hash;

		ଯଦି (!v)
			ଫେରନ୍ତୁ config_error_nonbool(k);

		ex = xmalloc(ଆକାର(*ex));
		ଯଦି (parse_oid_hex(v, &ex->e.oid, &oid_end) ||
		    *oid_end != ' ' ||
		    parse_oid_hex(oid_end + 1, &pack_hash, &pack_end) ||
		    *pack_end != ' ')
			die(_("value of uploadpack.blobpackfileuri must be "
			      "of the form '<object-hash> <pack-hash> <uri>' (got '%s')"), v);
		ଯଦି (oidmap_get(&configured_exclusions, &ex->e.oid))
			die(_("object already configured in another "
			      "uploadpack.blobpackfileuri (got '%s')"), v);
		ex->pack_hash_hex = xcalloc(1, pack_end - oid_end);
		memcpy(ex->pack_hash_hex, oid_end + 1, pack_end - oid_end - 1);
		ex->uri = xstrdup(pack_end + 1);
		oidmap_put(&configured_exclusions, ex);
	}
	ଫେରନ୍ତୁ git_default_config(k, v, ctx, cb);
}

/* Counters for trace2 output when in --stdin-packs mode. */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା stdin_packs_found_nr;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା stdin_packs_hints_nr;

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା add_object_entry_from_pack(ଅଚଳ ସଂରଚନା object_id *oid,
				      ସଂରଚନା packed_git *p,
				      uint32_t pos,
				      ଶୂନ୍ୟ *_data)
{
	off_t ofs;
	ସଂରଚନା object_info oi = OBJECT_INFO_INIT;
	ଗଣନା object_type type = OBJ_NONE;

	display_progress(progress_state, ++nr_seen);

	ଯଦି (have_duplicate_entry(oid, 0))
		ଫେରନ୍ତୁ 0;

	stdin_packs_found_nr++;

	ofs = nth_packed_object_offset(p, pos);

	oi.typep = &type;
	ଯଦି (packed_object_info(ଖାଲି, p, ofs, &oi) < 0) {
		die(_("could not get type of object %s in pack %s"),
		    oid_to_hex(oid), p->pack_name);
	} ଅନ୍ୟଥା ଯଦି (type == OBJ_COMMIT) {
		ସଂରଚନା rev_info *revs = _data;
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
		add_pending_oid(revs, ଖାଲି, oid, 0);
	}

	ଯଦି (!want_object_in_pack(oid, 0, &p, &ofs))
		ଫେରନ୍ତୁ 0;

	create_object_entry(oid, type, 0, 0, 0, p, ofs);

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ show_object_pack_hint(ସଂରଚନା object *object, ଅଚଳ ଅକ୍ଷର *name,
				  ଶୂନ୍ୟ *data)
{
	ଗଣନା stdin_packs_mode mode = *(ଗଣନା stdin_packs_mode *)data;
	ଯଦି (mode == STDIN_PACKS_MODE_FOLLOW) {
		ଯଦି (object->type == OBJ_BLOB &&
		    !odb_has_object(the_repository->objects, &object->oid, 0))
			ଫେରନ୍ତୁ;
		add_object_entry(&object->oid, object->type, name, 0);
	} ଅନ୍ୟଥା {
		ସଂରଚନା object_entry *oe = packlist_find(&to_pack, &object->oid);
		ଯଦି (!oe)
			ଫେରନ୍ତୁ;

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

ସ୍ଥିର ଶୂନ୍ୟ show_commit_pack_hint(ସଂରଚନା commit *commit, ଶୂନ୍ୟ *data)
{
	ଗଣନା stdin_packs_mode mode = *(ଗଣନା stdin_packs_mode *)data;

	ଯଦି (mode == STDIN_PACKS_MODE_FOLLOW) {
		show_object_pack_hint((ସଂରଚନା object *)commit, "", data);
		ଫେରନ୍ତୁ;
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
ଗଣନା stdin_pack_info_kind {
	STDIN_PACK_INCLUDE = (1<<0),
	STDIN_PACK_EXCLUDE_CLOSED = (1<<1),
	STDIN_PACK_EXCLUDE_OPEN = (1<<2),
};

ସଂରଚନା stdin_pack_info {
	ସଂରଚନା packed_git *p;
	ଗଣନା stdin_pack_info_kind kind;
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା pack_mtime_cmp(ଅଚଳ ଶୂନ୍ୟ *_a, ଅଚଳ ଶୂନ୍ୟ *_b)
{
	ସଂରଚନା stdin_pack_info *a = ((ଅଚଳ ସଂରଚନା string_list_item*)_a)->util;
	ସଂରଚନା stdin_pack_info *b = ((ଅଚଳ ସଂରଚନା string_list_item*)_b)->util;

	/*
	 * order packs by descending mtime so that objects are laid out
	 * roughly as newest-to-oldest
	 */
	ଯଦି (a->p->mtime < b->p->mtime)
		ଫେରନ୍ତୁ 1;
	ଅନ୍ୟଥା ଯଦି (b->p->mtime < a->p->mtime)
		ଫେରନ୍ତୁ -1;
	ଅନ୍ୟଥା
		ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା stdin_packs_include_check_obj(ସଂରଚନା object *obj, ଶୂନ୍ୟ *data UNUSED)
{
	ଫେରନ୍ତୁ !has_object_kept_pack(to_pack.repo, &obj->oid,
				     KEPT_PACK_IN_CORE);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା stdin_packs_include_check(ସଂରଚନା commit *commit, ଶୂନ୍ୟ *data)
{
	ଫେରନ୍ତୁ stdin_packs_include_check_obj((ସଂରଚନା object *)commit, data);
}

ସ୍ଥିର ଶୂନ୍ୟ stdin_packs_add_pack_entries(ସଂରଚନା strmap *packs,
					 ସଂରଚନା rev_info *revs)
{
	ସଂରଚନା string_list ଚାବିଗୁଡ଼ିକ = STRING_LIST_INIT_NODUP;
	ସଂରଚନା string_list_item *item;
	ସଂରଚନା hashmap_iter iter;
	ସଂରଚନା strmap_entry *entry;

	strmap_for_each_entry(packs, &iter, entry) {
		ସଂରଚନା stdin_pack_info *info = entry->ମୂଲ୍ୟ;
		ଯଦି (!info->p)
			die(_("could not find pack '%s'"), entry->ଚାବି);

		string_list_append(&ଚାବିଗୁଡ଼ିକ, entry->ଚାବି)->util = info;
	}

	/*
	 * Order packs by ascending mtime; use QSORT directly to access the
	 * string_list_item's ->util pointer, which string_list_sort() does not
	 * provide.
	 */
	QSORT(ଚାବିଗୁଡ଼ିକ.ବସ୍ତୁଗୁଡ଼ିକ, ଚାବିଗୁଡ଼ିକ.nr, pack_mtime_cmp);

	for_each_string_list_item(item, &ଚାବିଗୁଡ଼ିକ) {
		ସଂରଚନା stdin_pack_info *info = item->util;

		ଯଦି (info->kind & STDIN_PACK_EXCLUDE_OPEN) {
			/*
			 * When open-excluded packs ("!") are present, stop
			 * the parent walk at closed-excluded ("^") packs.
			 * Objects behind a "^" boundary are guaranteed to
			 * have closure and should not be rescued.
			 */
			revs->include_check = stdin_packs_include_check;
			revs->include_check_obj = stdin_packs_include_check_obj;
		}

		ଯଦି ((info->kind & STDIN_PACK_INCLUDE) ||
		    (info->kind & STDIN_PACK_EXCLUDE_OPEN))
			for_each_object_in_pack(info->p,
						add_object_entry_from_pack,
						revs,
						ODB_FOR_EACH_OBJECT_PACK_ORDER);
	}

	string_list_clear(&ଚାବିଗୁଡ଼ିକ, 0);
}

ସ୍ଥିର ଶୂନ୍ୟ stdin_packs_read_ନିବେଶ(ସଂରଚନା rev_info *revs,
				   ଗଣନା stdin_packs_mode mode)
{
	ସଂରଚନା strbuf buf = STRBUF_INIT;
	ସଂରଚନା strmap packs = STRMAP_INIT;
	ସଂରଚନା packed_git *p;

	ଯେତେବେଳେ (strbuf_getline(&buf, stdin) != EOF) {
		ସଂରଚନା stdin_pack_info *info;
		ଗଣନା stdin_pack_info_kind kind = STDIN_PACK_INCLUDE;
		ଅଚଳ ଅକ୍ଷର *ଚାବି = buf.buf;

		ଯଦି (!*ଚାବି)
			ଜାରି;
		ଅନ୍ୟଥା ଯଦି (*ଚାବି == '^')
			kind = STDIN_PACK_EXCLUDE_CLOSED;
		ଅନ୍ୟଥା ଯଦି (*ଚାବି == '!' && mode == STDIN_PACKS_MODE_FOLLOW)
			kind = STDIN_PACK_EXCLUDE_OPEN;

		ଯଦି (kind != STDIN_PACK_INCLUDE)
			ଚାବି++;

		info = strmap_get(&packs, ଚାବି);
		ଯଦି (!info) {
			CALLOC_ARRAY(info, 1);
			strmap_put(&packs, ଚାବି, info);
		}

		info->kind |= kind;

		strbuf_reset(&buf);
	}

	repo_for_each_pack(the_repository, p) {
		ସଂରଚନା stdin_pack_info *info;

		info = strmap_get(&packs, pack_basename(p));
		ଯଦି (!info)
			ଜାରି;

		ଯଦି (info->kind & STDIN_PACK_INCLUDE) {
			ଯଦି (exclude_promisor_objects && p->pack_promisor)
				die(_("packfile %s is a promisor but --exclude-promisor-objects was given"), p->pack_name);

			/*
			 * Arguments we got on stdin may not even be
			 * packs. First check that to avoid segfaulting
			 * later on in e.g.  pack_mtime_cmp(), excluded
			 * packs are handled below.
			 */
			ଯଦି (!is_pack_valid(p))
				die(_("packfile %s cannot be accessed"), p->pack_name);
		}

		ଯଦି (info->kind & STDIN_PACK_EXCLUDE_CLOSED) {
			/*
			 * Marking excluded packs as kept in-core so
			 * that later calls to add_object_entry()
			 * discards any objects that are also found in
			 * excluded packs.
			 */
			p->pack_keep_in_core = 1;
		}

		ଯଦି (info->kind & STDIN_PACK_EXCLUDE_OPEN) {
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

ସ୍ଥିର ଶୂନ୍ୟ add_unreachable_loose_objects(ସଂରଚନା rev_info *revs);

ସ୍ଥିର ଶୂନ୍ୟ read_stdin_packs(ସଂରଚନା repository *repo,
			     ଗଣନା stdin_packs_mode mode, ପୂର୍ଣ୍ଣସଂଖ୍ୟା rev_list_unpacked)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା prev_fetch_if_missing = repo->fetch_if_missing;
	ସଂରଚନା rev_info revs;

	/*
	 * The revision walk may hit objects that are promised, only. As the
	 * walk is best-effort though we don't want to perform backfill fetches
	 * for them.
	 */
	repo->fetch_if_missing = 0;

	repo_init_revisions(repo, &revs, ଖାଲି);
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
	revs.ବୃକ୍ଷ_objects = 1;
	revs.tag_objects = 1;
	revs.ignore_missing_links = 1;
	revs.exclude_promisor_objects = exclude_promisor_objects;

	/* avoids adding objects in excluded packs */
	ignore_packed_keep_in_core = 1;
	ଯଦି (mode == STDIN_PACKS_MODE_FOLLOW) {
		/*
		 * In '--stdin-packs=follow' mode, additionally ignore
		 * objects in excluded-open packs to prevent them from
		 * appearing in the resulting pack.
		 */
		ignore_packed_keep_in_core_open = 1;
	}
	stdin_packs_read_ନିବେଶ(&revs, mode);
	ଯଦି (rev_list_unpacked)
		add_unreachable_loose_objects(&revs);

	ଯଦି (prepare_revision_walk(&revs))
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

ସ୍ଥିର ଶୂନ୍ୟ add_cruft_object_entry(ଅଚଳ ସଂରଚନା object_id *oid, ଗଣନା object_type type,
				   ସଂରଚନା packed_git *pack, off_t offset,
				   ଅଚଳ ଅକ୍ଷର *name, uint32_t mtime)
{
	ସଂରଚନା object_entry *entry;

	display_progress(progress_state, ++nr_seen);

	entry = packlist_find(&to_pack, oid);
	ଯଦି (entry) {
		ଯଦି (name) {
			entry->hash = pack_name_hash_fn(name);
			entry->no_try_delta = no_try_delta(name);
		}
	} ଅନ୍ୟଥା {
		ଯଦି (!want_object_in_pack_mtime(oid, 0, &pack, &offset, mtime))
			ଫେରନ୍ତୁ;
		ଯଦି (!pack && type == OBJ_BLOB) {
			ସଂରଚନା odb_source_files *files =
				odb_source_files_downcast(the_repository->objects->source);
			ପୂର୍ଣ୍ଣସଂଖ୍ୟା found = 0;

			ପାଇଁ (ସଂରଚନା odb_files_dir *dir = files->dirs; !found && dir; dir = dir->next)
				ଯଦି (!odb_source_read_object_info(&dir->loose->base, oid, ଖାଲି, 0, ଖାଲି))
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
			ଯଦି (!found)
				ଫେରନ୍ତୁ;
		}

		entry = create_object_entry(oid, type, pack_name_hash_fn(name),
					    0, name && no_try_delta(name),
					    pack, offset);
	}

	ଯଦି (mtime > oe_cruft_mtime(&to_pack, entry))
		oe_set_cruft_mtime(&to_pack, entry, mtime);
	ଫେରନ୍ତୁ;
}

ସ୍ଥିର ଶୂନ୍ୟ show_cruft_object(ସଂରଚନା object *obj, ଅଚଳ ଅକ୍ଷର *name, ଶୂନ୍ୟ *data UNUSED)
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
	add_cruft_object_entry(&obj->oid, obj->type, ଖାଲି, 0, name, cruft_expiration);
}

ସ୍ଥିର ଶୂନ୍ୟ show_cruft_commit(ସଂରଚନା commit *commit, ଶୂନ୍ୟ *data)
{
	show_cruft_object((ସଂରଚନା object*)commit, ଖାଲି, data);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cruft_include_check_obj(ସଂରଚନା object *obj, ଶୂନ୍ୟ *data UNUSED)
{
	ଫେରନ୍ତୁ !has_object_kept_pack(to_pack.repo, &obj->oid, KEPT_PACK_IN_CORE);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cruft_include_check(ସଂରଚନା commit *commit, ଶୂନ୍ୟ *data)
{
	ଫେରନ୍ତୁ cruft_include_check_obj((ସଂରଚନା object*)commit, data);
}

ସ୍ଥିର ଶୂନ୍ୟ set_cruft_mtime(ଅଚଳ ସଂରଚନା object *object,
			    ସଂରଚନା packed_git *pack,
			    off_t offset, time_t mtime)
{
	add_cruft_object_entry(&object->oid, object->type, pack, offset, ଖାଲି,
			       mtime);
}

ସ୍ଥିର ଶୂନ୍ୟ mark_pack_kept_in_core(ସଂରଚନା string_list *packs, unsigned keep)
{
	ସଂରଚନା string_list_item *item = ଖାଲି;
	for_each_string_list_item(item, packs) {
		ସଂରଚନା packed_git *p = item->util;
		ଯଦି (!p)
			die(_("could not find pack '%s'"), item->string);
		ଯଦି (p->is_cruft && keep)
			ignore_packed_keep_in_core_has_cruft = 1;
		p->pack_keep_in_core = keep;
	}
}

ସ୍ଥିର ଶୂନ୍ୟ add_objects_in_unpacked_packs(ଶୂନ୍ୟ);

ସ୍ଥିର ଶୂନ୍ୟ enumerate_cruft_objects(ଶୂନ୍ୟ)
{
	ଯଦି (progress)
		progress_state = start_progress(the_repository,
						_("Enumerating cruft objects"), 0);

	add_objects_in_unpacked_packs();
	add_unreachable_loose_objects(ଖାଲି);

	stop_progress(&progress_state);
}

ସ୍ଥିର ଶୂନ୍ୟ enumerate_and_traverse_cruft_objects(ସଂରଚନା string_list *fresh_packs)
{
	ସଂରଚନା packed_git *p;
	ସଂରଚନା rev_info revs;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	repo_init_revisions(the_repository, &revs, ଖାଲି);

	revs.tag_objects = 1;
	revs.ବୃକ୍ଷ_objects = 1;
	revs.blob_objects = 1;

	revs.include_check = cruft_include_check;
	revs.include_check_obj = cruft_include_check_obj;

	revs.ignore_missing_links = 1;

	ଯଦି (progress)
		progress_state = start_progress(the_repository,
						_("Enumerating cruft objects"), 0);
	ret = add_unseen_recent_objects_to_traversal(&revs, cruft_expiration,
						     set_cruft_mtime, 1);
	stop_progress(&progress_state);

	ଯଦି (ret)
		die(_("unable to add cruft objects"));

	/*
	 * Re-mark only the fresh packs as kept so that objects in
	 * unknown packs do not halt the reachability traversal early.
	 */
	repo_for_each_pack(the_repository, p)
		p->pack_keep_in_core = 0;
	mark_pack_kept_in_core(fresh_packs, 1);

	ଯଦି (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	ଯଦି (progress)
		progress_state = start_progress(the_repository,
						_("Traversing cruft objects"), 0);
	nr_seen = 0;
	traverse_commit_list(&revs, show_cruft_commit, show_cruft_object, ଖାଲି);

	stop_progress(&progress_state);
	release_revisions(&revs);
}

ସ୍ଥିର ଶୂନ୍ୟ read_cruft_objects(ଶୂନ୍ୟ)
{
	ସଂରଚନା strbuf buf = STRBUF_INIT;
	ସଂରଚନା string_list discard_packs = STRING_LIST_INIT_DUP;
	ସଂରଚନା string_list fresh_packs = STRING_LIST_INIT_DUP;
	ସଂରଚନା packed_git *p;

	ignore_packed_keep_in_core = 1;

	ଯେତେବେଳେ (strbuf_getline(&buf, stdin) != EOF) {
		ଯଦି (!buf.len)
			ଜାରି;

		ଯଦି (*buf.buf == '-')
			string_list_append(&discard_packs, buf.buf + 1);
		ଅନ୍ୟଥା
			string_list_append(&fresh_packs, buf.buf);
	}

	string_list_sort(&discard_packs);
	string_list_sort(&fresh_packs);

	repo_for_each_pack(the_repository, p) {
		ଅଚଳ ଅକ୍ଷର *pack_name = pack_basename(p);
		ସଂରଚନା string_list_item *item;

		item = string_list_lookup(&fresh_packs, pack_name);
		ଯଦି (!item)
			item = string_list_lookup(&discard_packs, pack_name);

		ଯଦି (item) {
			item->util = p;
		} ଅନ୍ୟଥା {
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

	ଯଦି (cruft_expiration)
		enumerate_and_traverse_cruft_objects(&fresh_packs);
	ଅନ୍ୟଥା
		enumerate_cruft_objects();

	strbuf_release(&buf);
	string_list_clear(&discard_packs, 0);
	string_list_clear(&fresh_packs, 0);
}

ସ୍ଥିର ଶୂନ୍ୟ read_object_list_from_stdin(ଶୂନ୍ୟ)
{
	ଅକ୍ଷର ଧାଡ଼ି[GIT_MAX_HEXSZ + 1 + PATH_MAX + 2];
	ସଂରଚନା object_id oid;
	ଅଚଳ ଅକ୍ଷର *p;

	ପାଇଁ (;;) {
		ଯଦି (!fgets(ଧାଡ଼ି, ଆକାର(ଧାଡ଼ି), stdin)) {
			ଯଦି (feof(stdin))
				ଭାଙ୍ଗ;
			ଯଦି (!ferror(stdin))
				BUG("fgets returned NULL, not EOF, not error!");
			ଯଦି (errno != EINTR)
				die_errno("fgets");
			clearerr(stdin);
			ଜାରି;
		}
		ଯଦି (ଧାଡ଼ି[0] == '-') {
			ଯଦି (get_oid_hex(ଧାଡ଼ି+1, &oid))
				die(_("expected edge object ID, got garbage:\n %s"),
				    ଧାଡ଼ି);
			add_preferred_base(&oid);
			ଜାରି;
		}
		ଯଦି (parse_oid_hex(ଧାଡ଼ି, &oid, &p))
			die(_("expected object ID, got garbage:\n %s"), ଧାଡ଼ି);

		add_preferred_base_object(p + 1);
		add_object_entry(&oid, OBJ_NONE, p + 1, 0);
	}
}

ସ୍ଥିର ଶୂନ୍ୟ show_commit(ସଂରଚନା commit *commit, ଶୂନ୍ୟ *data UNUSED)
{
	add_object_entry(&commit->object.oid, OBJ_COMMIT, ଖାଲି, 0);

	ଯଦି (write_bitmap_index)
		index_commit_for_bitmap(commit);

	ଯଦି (use_delta_islands)
		propagate_island_marks(the_repository, commit);
}

ସ୍ଥିର ଶୂନ୍ୟ show_object(ସଂରଚନା object *obj, ଅଚଳ ଅକ୍ଷର *name,
			ଶୂନ୍ୟ *data UNUSED)
{
	add_preferred_base_object(name);
	add_object_entry(&obj->oid, obj->type, name, 0);

	ଯଦି (use_delta_islands)
		record_ବୃକ୍ଷ_depth(&obj->oid, name);
}

ସ୍ଥିର ଶୂନ୍ୟ show_object__ma_allow_any(ସଂରଚନା object *obj, ଅଚଳ ଅକ୍ଷର *name, ଶୂନ୍ୟ *data)
{
	ପ୍ରତିଜ୍ଞା(arg_missing_action == MA_ALLOW_ANY);

	/*
	 * Quietly ignore ALL missing objects.  This avoids problems with
	 * staging them now and getting an odd error later.
	 */
	ଯଦି (!odb_has_object(the_repository->objects, &obj->oid, 0))
		ଫେରନ୍ତୁ;

	show_object(obj, name, data);
}

ସ୍ଥିର ଶୂନ୍ୟ show_object__ma_allow_promisor(ସଂରଚନା object *obj, ଅଚଳ ଅକ୍ଷର *name, ଶୂନ୍ୟ *data)
{
	ପ୍ରତିଜ୍ଞା(arg_missing_action == MA_ALLOW_PROMISOR);

	/*
	 * Quietly ignore EXPECTED missing objects.  This avoids problems with
	 * staging them now and getting an odd error later.
	 */
	ଯଦି (!odb_has_object(the_repository->objects, &obj->oid, 0) &&
	    is_promisor_object(to_pack.repo, &obj->oid))
		ଫେରନ୍ତୁ;

	show_object(obj, name, data);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା option_parse_missing_action(ଅଚଳ ସଂରଚନା option *opt,
				       ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ସଂରଚନା repository *repo = opt->ମୂଲ୍ୟ;

	ପ୍ରତିଜ୍ଞା(arg);
	ପ୍ରତିଜ୍ଞା(!unset);

	ଯଦି (!strcmp(arg, "error")) {
		arg_missing_action = MA_ERROR;
		fn_show_object = show_object;
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (!strcmp(arg, "allow-any")) {
		arg_missing_action = MA_ALLOW_ANY;
		repo->fetch_if_missing = 0;
		fn_show_object = show_object__ma_allow_any;
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (!strcmp(arg, "allow-promisor")) {
		arg_missing_action = MA_ALLOW_PROMISOR;
		repo->fetch_if_missing = 0;
		fn_show_object = show_object__ma_allow_promisor;
		ଫେରନ୍ତୁ 0;
	}

	die(_("invalid value for '%s': '%s'"), "--missing", arg);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ show_edge(ସଂରଚନା commit *commit)
{
	add_preferred_base(&commit->object.oid);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା add_object_in_unpacked_pack(ଅଚଳ ସଂରଚନା object_id *oid,
				       ସଂରଚନା object_info *oi,
				       ଶୂନ୍ୟ *data UNUSED)
{
	ଯଦି (cruft) {
		add_cruft_object_entry(oid, OBJ_NONE, oi->source_infop->u.packed.pack,
				       oi->source_infop->u.packed.offset, ଖାଲି,
				       *oi->mtimep);
	} ଅନ୍ୟଥା {
		add_object_entry(oid, OBJ_NONE, "", 0);
	}
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ add_objects_in_unpacked_packs(ଶୂନ୍ୟ)
{
	ସଂରଚନା odb_source_files *files =
		odb_source_files_downcast(to_pack.repo->objects->source);
	time_t mtime;
	ସଂରଚନା odb_for_each_object_options opts = {
		.flags = ODB_FOR_EACH_OBJECT_PACK_ORDER |
			 ODB_FOR_EACH_OBJECT_LOCAL_ONLY |
			 ODB_FOR_EACH_OBJECT_SKIP_IN_CORE_KEPT_PACKS |
			 ODB_FOR_EACH_OBJECT_SKIP_ON_DISK_KEPT_PACKS,
	};
	ସଂରଚନା odb_source_info source_info;
	ସଂରଚନା object_info oi = {
		.mtimep = &mtime,
		.source_infop = &source_info,
	};

	ପାଇଁ (ସଂରଚନା odb_files_dir *dir = files->dirs; dir; dir = dir->next) {
		ଯଦି (!dir->local)
			ଜାରି;

		ଯଦି (odb_source_for_each_object(&dir->packed->base, &oi,
					       add_object_in_unpacked_pack, ଖାଲି, &opts))
			die(_("cannot open pack index"));
	}
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା add_loose_object(ଅଚଳ ସଂରଚନା object_id *oid, ଅଚଳ ଅକ୍ଷର *path,
			    ଶୂନ୍ୟ *data)
{
	ସଂରଚନା rev_info *revs = data;
	ଗଣନା object_type type = odb_read_object_info(the_repository->objects, oid, ଖାଲି);

	ଯଦି (type < 0) {
		warning(_("loose object at %s could not be examined"), path);
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (cruft) {
		ସଂରଚନା stat st;
		ଯଦି (stat(path, &st) < 0) {
			ଯଦି (errno == ENOENT)
				ଫେରନ୍ତୁ 0;
			ଫେରନ୍ତୁ error_errno("unable to stat %s", oid_to_hex(oid));
		}

		add_cruft_object_entry(oid, type, ଖାଲି, 0, ଖାଲି,
				       st.st_mtime);
	} ଅନ୍ୟଥା {
		add_object_entry(oid, type, "", 0);
	}

	ଯଦି (revs && type == OBJ_COMMIT)
		add_pending_oid(revs, ଖାଲି, oid, 0);

	ଫେରନ୍ତୁ 0;
}

/*
 * We actually don't even have to worry about reachability here.
 * add_object_entry will weed out duplicates, so we just add every
 * loose object we find.
 */
ସ୍ଥିର ଶୂନ୍ୟ add_unreachable_loose_objects(ସଂରଚନା rev_info *revs)
{
	for_each_loose_file_in_source(the_repository->objects->source,
				      add_loose_object, ଖାଲି, ଖାଲି, revs);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା has_sha1_pack_kept_or_nonlocal(ଅଚଳ ସଂରଚନା object_id *oid)
{
	ସ୍ଥିର ସଂରଚନା packed_git *last_found = ଖାଲି;
	ସଂରଚନା packed_git *p;

	ଯଦି (last_found && find_pack_entry_one(oid, last_found))
		ଫେରନ୍ତୁ 1;

	repo_for_each_pack(the_repository, p) {
		/*
		 * We have already checked `last_found`, so there is no need to
		 * re-check here.
		 */
		ଯଦି (p == last_found)
			ଜାରି;

		ଯଦି ((!p->pack_local || p->pack_keep || p->pack_keep_in_core) &&
		    find_pack_entry_one(oid, p)) {
			last_found = p;
			ଫେରନ୍ତୁ 1;
		}
	}

	ଫେରନ୍ତୁ 0;
}

/*
 * Store a list of sha1s that are should not be discarded
 * because they are either written too recently, or are
 * reachable from another object that was.
 *
 * This is filled by get_object_list.
 */
ସ୍ଥିର ସଂରଚନା oid_array recent_objects;

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା loosened_object_can_be_discarded(ଅଚଳ ସଂରଚନା object_id *oid,
					    timestamp_t mtime)
{
	ଯଦି (!unpack_unreachable_expiration)
		ଫେରନ୍ତୁ 0;
	ଯଦି (mtime > unpack_unreachable_expiration)
		ଫେରନ୍ତୁ 0;
	ଯଦି (oid_array_lookup(&recent_objects, oid) >= 0)
		ଫେରନ୍ତୁ 0;
	ଫେରନ୍ତୁ 1;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା force_object_loose(ସଂରଚନା odb_source *source,
			      ଅଚଳ ସଂରଚନା object_id *oid,
			      ଅଚଳ time_t *mtime)
{
	ସଂରଚନା odb_source_files *files = odb_source_files_downcast(source);
	ଅଚଳ ସଂରଚନା git_hash_algo *compat = source->odb->repo->compat_hash_algo;
	ସଂରଚନା object_info oi = OBJECT_INFO_INIT;
	ସଂରଚନା object_id compat_oid, *compat_oid_p = ଖାଲି;
	ଗଣନା object_type type;
	ଶୂନ୍ୟ *buf = ଖାଲି;
	size_t len;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	ପାଇଁ (ସଂରଚନା odb_files_dir *dir = files->dirs; dir; dir = dir->next)
		ଯଦି (!odb_source_read_object_info(&dir->loose->base, oid, ଖାଲି, 0, ଖାଲି))
			ଫେରନ୍ତୁ 0;

	oi.typep = &type;
	oi.sizep = &len;
	oi.contentp = &buf;
	ଯଦି (odb_read_object_info_extended(source->odb, oid, &oi, 0)) {
		ret = error(_("cannot read object for %s"), oid_to_hex(oid));
		ଯାଅ out;
	}

	ଯଦି (compat) {
		ଯଦି (repo_oid_to_algop(source->odb->repo, oid, compat, &compat_oid)) {
			ret = error(_("cannot map object %s to %s"),
				    oid_to_hex(oid), compat->name);
			ଯାଅ out;
		}

		compat_oid_p = &compat_oid;
	}

	ret = odb_source_write_object(&files->dirs->loose->base, buf, len, type, oid,
				      compat_oid_p, mtime, 0);

out:
	ମୁକ୍ତ(buf);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ଶୂନ୍ୟ loosen_unused_packed_objects(ଶୂନ୍ୟ)
{
	ସଂରଚନା packed_git *p;
	uint32_t i;
	uint32_t loosened_objects_nr = 0;
	ସଂରଚନା object_id oid;

	repo_for_each_pack(the_repository, p) {
		ଯଦି (!p->pack_local || p->pack_keep || p->pack_keep_in_core)
			ଜାରି;

		ଯଦି (open_pack_index(p))
			die(_("cannot open pack index"));

		ପାଇଁ (i = 0; i < p->ଅଙ୍କ_objects; i++) {
			nth_packed_object_id(&oid, p, i);
			ଯଦି (!packlist_find(&to_pack, &oid) &&
			    !has_sha1_pack_kept_or_nonlocal(&oid) &&
			    !loosened_object_can_be_discarded(&oid, p->mtime)) {
				ଯଦି (force_object_loose(the_repository->objects->source,
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
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା pack_options_allow_reuse(ଶୂନ୍ୟ)
{
	ଫେରନ୍ତୁ allow_pack_reuse != NO_PACK_REUSE &&
	       pack_to_stdout &&
	       !ignore_packed_keep_on_disk &&
	       !ignore_packed_keep_in_core &&
	       (!local || !have_non_local_packs) &&
	       !incremental;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_object_list_from_bitmap(ସଂରଚନା rev_info *revs)
{
	ଯଦି (!(bitmap_git = prepare_bitmap_walk(revs, 0)))
		ଫେରନ୍ତୁ -1;

	/*
	 * For now, force the name-hash version to be 1 since that
	 * is the version implied by the bitmap format. Later, the
	 * format can include this version explicitly in its format,
	 * allowing readers to know the version that was used during
	 * the bitmap write.
	 */
	name_hash_version = 1;

	ଯଦି (pack_options_allow_reuse())
		reuse_partial_packfile_from_bitmap(bitmap_git,
						   &reuse_packfiles,
						   &reuse_packfiles_nr,
						   &reuse_packfile_bitmap,
						   allow_pack_reuse == MULTI_PACK_REUSE);

	ଯଦି (reuse_packfiles) {
		reuse_packfile_objects = bitmap_popcount(reuse_packfile_bitmap);
		ଯଦି (!reuse_packfile_objects)
			BUG("expected non-empty reuse bitmap");

		nr_result += reuse_packfile_objects;
		nr_seen += reuse_packfile_objects;
		display_progress(progress_state, nr_seen);
	}

	traverse_bitmap_commit_list(bitmap_git, revs,
				    &add_object_entry_from_bitmap);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ record_recent_object(ସଂରଚନା object *obj,
				 ଅଚଳ ଅକ୍ଷର *name UNUSED,
				 ଶୂନ୍ୟ *data UNUSED)
{
	oid_array_append(&recent_objects, &obj->oid);
}

ସ୍ଥିର ଶୂନ୍ୟ record_recent_commit(ସଂରଚନା commit *commit, ଶୂନ୍ୟ *data UNUSED)
{
	oid_array_append(&recent_objects, &commit->object.oid);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା mark_bitmap_preferred_tip(ଅଚଳ ସଂରଚନା reference *ref, ଶୂନ୍ୟ *data UNUSED)
{
	ଅଚଳ ସଂରଚନା object_id *maybe_peeled = ref->oid;
	ସଂରଚନା object_id peeled;
	ସଂରଚନା object *object;

	ଯଦି (!reference_get_peeled_oid(the_repository, ref, &peeled))
		maybe_peeled = &peeled;

	object = parse_object_or_die(the_repository, maybe_peeled, ref->name);
	ଯଦି (object->type == OBJ_COMMIT)
		object->flags |= NEEDS_BITMAP;

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା is_oid_uninteresting(ସଂରଚନା repository *repo,
				       ସଂରଚନା object_id *oid)
{
	ସଂରଚନା object *o = lookup_object(repo, oid);
	ଫେରନ୍ତୁ !o || (o->flags & UNINTERESTING);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା add_objects_by_path(ଅଚଳ ଅକ୍ଷର *path,
			       ସଂରଚନା oid_array *oids,
			       ଗଣନା object_type type,
			       ଶୂନ୍ୟ *data)
{
	size_t oe_start = to_pack.nr_objects;
	size_t oe_end;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *processed = data;

	/*
	 * First, add all objects to the packing data, including the ones
	 * marked UNINTERESTING (translated to 'exclude') as they can be
	 * used as delta bases.
	 */
	ପାଇଁ (size_t i = 0; i < oids->nr; i++) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା exclude;
		ସଂରଚନା object_info oi = OBJECT_INFO_INIT;
		ସଂରଚନା object_id *oid = &oids->oid[i];

		/* Skip objects that do not exist locally. */
		ଯଦି ((exclude_promisor_objects || arg_missing_action != MA_ERROR) &&
		    odb_read_object_info_extended(the_repository->objects, oid, &oi,
						  OBJECT_INFO_FOR_PREFETCH) < 0)
			ଜାରି;

		exclude = is_oid_uninteresting(the_repository, oid);

		ଯଦି (exclude && !thin)
			ଜାରି;

		add_object_entry(oid, type, path, exclude);

		ଯଦି (type == OBJ_COMMIT) {
			ସଂରଚନା commit *commit;

			ଯଦି (!write_bitmap_index && !use_delta_islands)
				ଜାରି;

			commit = lookup_commit(the_repository, oid);
			ଯଦି (!commit)
				die(_("could not find commit %s"), oid_to_hex(oid));
			ଯଦି (write_bitmap_index)
				index_commit_for_bitmap(commit);
			/*
			 * Skip island propagation for boundary commits.
			 * The regular traversal's show_commit() is only
			 * called for interesting commits; matching that
			 * here keeps path-walk from doing extra work that
			 * would only be a no-op anyway (boundary commits
			 * are not in island_marks).
			 */
			ଯଦି (use_delta_islands && !exclude)
				propagate_island_marks(the_repository, commit);
		} ଅନ୍ୟଥା ଯଦି (type == OBJ_ବୃକ୍ଷ && use_delta_islands) {
			record_ବୃକ୍ଷ_depth(oid, path);
		}
	}

	oe_end = to_pack.nr_objects;

	/* We can skip delta calculations if it is a no-op. */
	ଯଦି (oe_end == oe_start || !window)
		ଫେରନ୍ତୁ 0;

	ALLOC_GROW(to_pack.regions,
		   to_pack.nr_regions + 1,
		   to_pack.nr_regions_alloc);

	to_pack.regions[to_pack.nr_regions].start = oe_start;
	to_pack.regions[to_pack.nr_regions].nr = oe_end - oe_start;
	to_pack.nr_regions++;

	*processed += oids->nr;
	display_progress(progress_state, *processed);

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_object_list_path_walk(ସଂରଚନା rev_info *revs)
{
	ସଂରଚନା path_walk_info info = PATH_WALK_INFO_INIT;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା processed = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା result;

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

	ଫେରନ୍ତୁ result;
}

ସ୍ଥିର ଶୂନ୍ୟ get_object_list(ସଂରଚନା rev_info *revs, ସଂରଚନା strvec *argv)
{
	ସଂରଚନା setup_revision_opt s_r_opt = {
		.allow_exclude_promisor_objects = 1,
	};
	ସଂରଚନା repo_config_ମୂଲ୍ୟଗୁଡ଼ିକ *cfg = repo_config_ମୂଲ୍ୟଗୁଡ଼ିକ(the_repository);
	ଅକ୍ଷର ଧାଡ଼ି[1000];
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା save_warning;

	save_commit_buffer = 0;
	setup_revisions_from_strvec(argv, revs, &s_r_opt);

	/* make sure shallows are read */
	is_repository_shallow(the_repository);

	save_warning = cfg->warn_on_object_refname_ambiguity;
	cfg->warn_on_object_refname_ambiguity = 0;

	ଯେତେବେଳେ (fgets(ଧାଡ଼ି, ଆକାର(ଧାଡ଼ି), stdin) != ଖାଲି) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା len = strlen(ଧାଡ଼ି);
		ଯଦି (len && ଧାଡ଼ି[len - 1] == '\n')
			ଧାଡ଼ି[--len] = 0;
		ଯଦି (!len)
			ଭାଙ୍ଗ;
		ଯଦି (*ଧାଡ଼ି == '-') {
			ଯଦି (!strcmp(ଧାଡ଼ି, "--not")) {
				flags ^= UNINTERESTING;
				write_bitmap_index = 0;
				ଜାରି;
			}
			ଯଦି (starts_with(ଧାଡ଼ି, "--shallow ")) {
				ସଂରଚନା object_id oid;
				ଯଦି (get_oid_hex(ଧାଡ଼ି + 10, &oid))
					die("not an object name '%s'", ଧାଡ଼ି + 10);
				register_shallow(the_repository, &oid);
				use_bitmap_index = 0;
				ଜାରି;
			}
			die(_("not a rev '%s'"), ଧାଡ଼ି);
		}
		ଯଦି (handle_revision_arg(ଧାଡ଼ି, revs, flags, REVARG_CANNOT_BE_FILENAME))
			die(_("bad revision '%s'"), ଧାଡ଼ି);
	}

	cfg->warn_on_object_refname_ambiguity = save_warning;

	ଯଦି (use_bitmap_index && !get_object_list_from_bitmap(revs))
		ଫେରନ୍ତୁ;

	ଯଦି (use_delta_islands)
		load_delta_islands(the_repository, progress);

	ଯଦି (write_bitmap_index)
		for_each_preferred_bitmap_tip(the_repository, mark_bitmap_preferred_tip,
					      ଖାଲି);

	ଯଦି (!fn_show_object)
		fn_show_object = show_object;

	ଯଦି (path_walk) {
		ଯଦି (get_object_list_path_walk(revs)) {
			warning(_("failed to pack objects via path-walk"));
			path_walk = 0;
		}
	}

	ଯଦି (!path_walk) {
		ଯଦି (prepare_revision_walk(revs))
			die(_("revision walk setup failed"));
		mark_edges_uninteresting(revs, show_edge, sparse);
		traverse_commit_list(revs,
				show_commit, fn_show_object,
				ଖାଲି);
	}

	ଯଦି (unpack_unreachable_expiration) {
		revs->ignore_missing_links = 1;
		ଯଦି (add_unseen_recent_objects_to_traversal(revs,
				unpack_unreachable_expiration, ଖାଲି, 0))
			die(_("unable to add recent objects"));
		ଯଦି (prepare_revision_walk(revs))
			die(_("revision walk setup failed"));
		traverse_commit_list(revs, record_recent_commit,
				     record_recent_object, ଖାଲି);
	}

	ଯଦି (keep_unreachable)
		add_objects_in_unpacked_packs();
	ଯଦି (pack_loose_unreachable)
		add_unreachable_loose_objects(ଖାଲି);
	ଯଦି (unpack_unreachable)
		loosen_unused_packed_objects();

	oid_array_clear(&recent_objects);
}

ସ୍ଥିର ଶୂନ୍ୟ add_extra_kept_packs(ଅଚଳ ସଂରଚନା string_list *names)
{
	ସଂରଚନା packed_git *p;

	ଯଦି (!names->nr)
		ଫେରନ୍ତୁ;

	repo_for_each_pack(the_repository, p) {
		ଅଚଳ ଅକ୍ଷର *name = basename(p->pack_name);
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

		ଯଦି (!p->pack_local)
			ଜାରି;

		ପାଇଁ (i = 0; i < names->nr; i++)
			ଯଦି (!fspathcmp(name, names->ବସ୍ତୁଗୁଡ଼ିକ[i].string))
				ଭାଙ୍ଗ;

		ଯଦି (i < names->nr) {
			p->pack_keep_in_core = 1;
			ignore_packed_keep_in_core = 1;
			ଜାରି;
		}
	}
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା option_parse_quiet(ଅଚଳ ସଂରଚନା option *opt, ଅଚଳ ଅକ୍ଷର *arg,
			      ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା *val = opt->ମୂଲ୍ୟ;

	BUG_ON_OPT_ARG(arg);

	ଯଦି (!unset)
		*val = 0;
	ଅନ୍ୟଥା ଯଦି (!*val)
		*val = 1;
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା option_parse_index_version(ଅଚଳ ସଂରଚନା option *opt,
				      ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ସଂରଚନା pack_idx_option *popts = opt->ମୂଲ୍ୟ;
	ଅକ୍ଷର *c;
	ଅଚଳ ଅକ୍ଷର *val = arg;

	BUG_ON_OPT_NEG(unset);

	popts->version = strtoul(val, &c, 10);
	ଯଦି (popts->version > 2)
		die(_("unsupported index version %s"), val);
	ଯଦି (*c == ',' && c[1])
		popts->off32_limit = strtoul(c+1, &c, 0);
	ଯଦି (*c || popts->off32_limit & 0x80000000)
		die(_("bad index version '%s'"), val);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା option_parse_unpack_unreachable(ଅଚଳ ସଂରଚନା option *opt UNUSED,
					   ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ଯଦି (unset) {
		unpack_unreachable = 0;
		unpack_unreachable_expiration = 0;
	}
	ଅନ୍ୟଥା {
		unpack_unreachable = 1;
		ଯଦି (arg)
			unpack_unreachable_expiration = approxidate(arg);
	}
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା option_parse_cruft_expiration(ଅଚଳ ସଂରଚନା option *opt UNUSED,
					 ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ଯଦି (unset) {
		cruft = 0;
		cruft_expiration = 0;
	} ଅନ୍ୟଥା {
		cruft = 1;
		ଯଦି (arg)
			cruft_expiration = approxidate(arg);
	}
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା is_not_in_promisor_pack_obj(ସଂରଚନା object *obj, ଶୂନ୍ୟ *data UNUSED)
{
	ସଂରଚନା odb_source_info source_info;
	ସଂରଚନା object_info info = {
		.source_infop = &source_info,
	};

	ଯଦି (odb_read_object_info_extended(the_repository->objects, &obj->oid, &info, 0))
		BUG("should_include_obj should only be called on existing objects");
	ଫେରନ୍ତୁ source_info.source->type != ODB_SOURCE_PACKED || !source_info.u.packed.pack->pack_promisor;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା is_not_in_promisor_pack(ସଂରଚନା commit *commit, ଶୂନ୍ୟ *data) {
	ଫେରନ୍ତୁ is_not_in_promisor_pack_obj((ସଂରଚନା object *) commit, data);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା parse_stdin_packs_mode(ଅଚଳ ସଂରଚନା option *opt, ଅଚଳ ଅକ୍ଷର *arg,
				  ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ଗଣନା stdin_packs_mode *mode = opt->ମୂଲ୍ୟ;

	ଯଦି (unset)
		*mode = STDIN_PACKS_MODE_NONE;
	ଅନ୍ୟଥା ଯଦି (!arg || !*arg)
		*mode = STDIN_PACKS_MODE_STANDARD;
	ଅନ୍ୟଥା ଯଦି (!strcmp(arg, "follow"))
		*mode = STDIN_PACKS_MODE_FOLLOW;
	ଅନ୍ୟଥା
		die(_("invalid value for '%s': '%s'"), opt->long_name, arg);

	ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_pack_objects(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
		     ଅଚଳ ଅକ୍ଷର **argv,
		     ଅଚଳ ଅକ୍ଷର *prefix,
		     ସଂରଚନା repository *repo)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା use_internal_rev_list = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା all_progress_implied = 0;
	ସଂରଚନା strvec rp = STRVEC_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା rev_list_unpacked = 0, rev_list_all = 0, rev_list_reflog = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା rev_list_index = 0;
	ଗଣନା stdin_packs_mode stdin_packs = STDIN_PACKS_MODE_NONE;
	ସଂରଚନା string_list keep_pack_list = STRING_LIST_INIT_NODUP;
	ସଂରଚନା list_objects_filter_options filter_options =
		LIST_OBJECTS_FILTER_INIT;
	ସଂରଚନା repo_config_ମୂଲ୍ୟଗୁଡ଼ିକ *cfg = repo_config_ମୂଲ୍ୟଗୁଡ଼ିକ(the_repository);

	ସଂରଚନା option pack_objects_options[] = {
		OPT_CALLBACK_F('q', "quiet", &progress, ଖାଲି,
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
		OPT_INTEGER(0, "threads", &delta_ସନ୍ଧାନ_threads,
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
		OPT_CALLBACK_F(0, "unpack-unreachable", ଖାଲି, N_("time"),
		  N_("unpack unreachable objects newer than <time>"),
		  PARSE_OPT_OPTARG, option_parse_unpack_unreachable),
		OPT_BOOL(0, "cruft", &cruft, N_("create a cruft pack")),
		OPT_CALLBACK_F(0, "cruft-expiration", ଖାଲି, N_("time"),
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

	ଯଦି (DFS_ଅଙ୍କ_STATES > (1 << OE_DFS_STATE_BITS))
		BUG("too many dfs states, increase OE_DFS_STATE_BITS");

	disable_replace_refs();

	sparse = git_env_bool("GIT_TEST_PACK_SPARSE", -1);
	ଯଦି (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		ଯଦି (sparse < 0)
			sparse = the_repository->settings.pack_use_sparse;
		ଯଦି (the_repository->settings.pack_use_multi_pack_reuse)
			allow_pack_reuse = MULTI_PACK_REUSE;
	}

	reset_pack_idx_option(&pack_idx_opts);
	pack_idx_opts.flags |= WRITE_REV;
	repo_config(the_repository, git_pack_config, ଖାଲି);
	ଯଦି (git_env_bool(GIT_TEST_NO_WRITE_REV_INDEX, 0))
		pack_idx_opts.flags &= ~WRITE_REV;

	progress = isatty(2);
	argc = parse_options(argc, argv, prefix, pack_objects_options,
			     pack_usage, 0);

	ଯଦି (argc) {
		base_name = argv[0];
		argc--;
	}
	ଯଦି (pack_to_stdout != !base_name || argc)
		usage_with_options(pack_usage, pack_objects_options);

	ଯଦି (path_walk < 0) {
		ଯଦି (use_bitmap_index > 0 ||
		    !use_internal_rev_list)
			path_walk = 0;
		ଅନ୍ୟଥା ଯଦି (the_repository->gitdir &&
			 the_repository->settings.pack_use_path_walk)
			path_walk = 1;
		ଅନ୍ୟଥା
			path_walk = git_env_bool("GIT_TEST_PACK_PATH_WALK", 0);
	}

	ଯଦି (depth < 0)
		depth = 0;
	ଯଦି (depth >= (1 << OE_DEPTH_BITS)) {
		warning(_("delta chain depth %d is too deep, forcing %d"),
			depth, (1 << OE_DEPTH_BITS) - 1);
		depth = (1 << OE_DEPTH_BITS) - 1;
	}
	ଯଦି (cache_max_small_delta_size >= (1U << OE_Z_DELTA_BITS)) {
		warning(_("pack.deltaCacheLimit is too high, forcing %d"),
			(1U << OE_Z_DELTA_BITS) - 1);
		cache_max_small_delta_size = (1U << OE_Z_DELTA_BITS) - 1;
	}
	ଯଦି (window < 0)
		window = 0;

	strvec_ଠେଲ(&rp, "pack-objects");

	ଯଦି (path_walk) {
		ଅଚଳ ଅକ୍ଷର *option = ଖାଲି;
		ଯଦି (!path_walk_filter_compatible(&filter_options))
			option = "--filter";

		ଯଦି (option) {
			warning(_("cannot use %s with %s"),
				option, "--path-walk");
			path_walk = 0;
		}
	}
	ଯଦି (path_walk) {
		strvec_ଠେଲ(&rp, "--objects");
	} ଅନ୍ୟଥା ଯଦି (thin) {
		use_internal_rev_list = 1;
		strvec_ଠେଲ(&rp, shallow
				? "--objects-edge-aggressive"
				: "--objects-edge");
	} ଅନ୍ୟଥା
		strvec_ଠେଲ(&rp, "--objects");

	ଯଦି (rev_list_all) {
		use_internal_rev_list = 1;
		strvec_ଠେଲ(&rp, "--all");
	}
	ଯଦି (rev_list_reflog) {
		use_internal_rev_list = 1;
		strvec_ଠେଲ(&rp, "--reflog");
	}
	ଯଦି (rev_list_index) {
		use_internal_rev_list = 1;
		strvec_ଠେଲ(&rp, "--indexed-objects");
	}
	ଯଦି (rev_list_unpacked && !stdin_packs) {
		use_internal_rev_list = 1;
		strvec_ଠେଲ(&rp, "--unpacked");
	}

	die_for_incompatible_opt2(exclude_promisor_objects,
				  "--exclude-promisor-objects",
				  exclude_promisor_objects_best_effort,
				  "--exclude-promisor-objects-best-effort");
	ଯଦି (exclude_promisor_objects) {
		repo->fetch_if_missing = 0;

		/* --stdin-packs handles promisor objects separately. */
		ଯଦି (!stdin_packs) {
			use_internal_rev_list = 1;
			strvec_ଠେଲ(&rp, "--exclude-promisor-objects");
		}
	} ଅନ୍ୟଥା ଯଦି (exclude_promisor_objects_best_effort) {
		use_internal_rev_list = 1;
		arg_missing_action = MA_ALLOW_ANY;
		repo->fetch_if_missing = 0;
		fn_show_object = show_object__ma_allow_any;
		/* revs configured below */
	}
	ଯଦି (unpack_unreachable || keep_unreachable || pack_loose_unreachable)
		use_internal_rev_list = 1;

	ଯଦି (!reuse_object)
		reuse_delta = 0;
	ଯଦି (cfg->pack_compression_level == -1)
		cfg->pack_compression_level = Z_DEFAULT_COMPRESSION;
	ଅନ୍ୟଥା ଯଦି (cfg->pack_compression_level < 0 || cfg->pack_compression_level > Z_BEST_COMPRESSION)
		die(_("bad pack compression level %d"), cfg->pack_compression_level);

	ଯଦି (!delta_ସନ୍ଧାନ_threads)	/* --threads=0 means autodetect */
		delta_ସନ୍ଧାନ_threads = online_cpus();

	ଯଦି (!HAVE_THREADS && delta_ସନ୍ଧାନ_threads != 1)
		warning(_("no threads support, ignoring --threads"));
	ଯଦି (!pack_to_stdout && !pack_size_limit)
		pack_size_limit = pack_size_limit_cfg;
	ଯଦି (pack_to_stdout && pack_size_limit)
		die(_("--max-pack-size cannot be used to build a pack for transfer"));
	ଯଦି (pack_size_limit && pack_size_limit < 1024*1024) {
		warning(_("minimum pack size limit is 1 MiB"));
		pack_size_limit = 1024*1024;
	}

	ଯଦି (!pack_to_stdout && thin)
		die(_("--thin cannot be used to build an indexable pack"));

	die_for_incompatible_opt2(keep_unreachable, "--keep-unreachable",
				  unpack_unreachable, "--unpack-unreachable");
	ଯଦି (!rev_list_all || !rev_list_reflog || !rev_list_index)
		unpack_unreachable_expiration = 0;

	die_for_incompatible_opt2(stdin_packs, "--stdin-packs",
				  filter_options.choice, "--filter");


	ଯଦି (stdin_packs && use_internal_rev_list)
		die(_("cannot use internal rev list with --stdin-packs"));

	ଯଦି (cruft) {
		ଯଦି (use_internal_rev_list)
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
	ଯଦି (!pack_to_stdout)
		use_bitmap_index_default = 0;

	ଯଦି (use_bitmap_index < 0)
		use_bitmap_index = use_bitmap_index_default;

	/* "hard" reasons not to use bitmaps; these just won't work at all */
	ଯଦି (!use_internal_rev_list || (!pack_to_stdout && write_bitmap_index) || is_repository_shallow(the_repository))
		use_bitmap_index = 0;

	ଯଦି (pack_to_stdout || !rev_list_all)
		write_bitmap_index = 0;

	ଯଦି (name_hash_version < 0)
		name_hash_version = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)git_env_ulong("GIT_TEST_NAME_HASH_VERSION", 1);

	validate_name_hash_version();

	ଯଦି (use_delta_islands)
		strvec_ଠେଲ(&rp, "--topo-order");

	ଯଦି (progress && all_progress_implied)
		progress = 2;

	add_extra_kept_packs(&keep_pack_list);
	ଯଦି (ignore_packed_keep_on_disk) {
		ସଂରଚନା packed_git *p;

		repo_for_each_pack(the_repository, p)
			ଯଦି (p->pack_local && p->pack_keep)
				ଭାଙ୍ଗ;
		ଯଦି (!p) /* no keep-able packs found */
			ignore_packed_keep_on_disk = 0;
	}
	ଯଦି (local) {
		/*
		 * unlike ignore_packed_keep_on_disk above, we do not
		 * want to unset "local" based on looking at packs, as
		 * it also covers non-local objects
		 */
		ସଂରଚନା packed_git *p;

		repo_for_each_pack(the_repository, p) {
			ଯଦି (!p->pack_local) {
				have_non_local_packs = 1;
				ଭାଙ୍ଗ;
			}
		}
	}

	trace2_region_enter("pack-objects", "enumerate-objects",
			    the_repository);
	prepare_packing_data(the_repository, &to_pack);

	ଯଦି (progress && !cruft)
		progress_state = start_progress(the_repository,
						_("Enumerating objects"), 0);
	ଯଦି (stdin_packs) {
		read_stdin_packs(repo, stdin_packs, rev_list_unpacked);
	} ଅନ୍ୟଥା ଯଦି (cruft) {
		read_cruft_objects();
	} ଅନ୍ୟଥା ଯଦି (!use_internal_rev_list) {
		read_object_list_from_stdin();
	} ଅନ୍ୟଥା {
		ସଂରଚନା rev_info revs;

		repo_init_revisions(the_repository, &revs, ଖାଲି);
		list_objects_filter_copy(&revs.filter, &filter_options);
		ଯଦି (exclude_promisor_objects_best_effort) {
			revs.include_check = is_not_in_promisor_pack;
			revs.include_check_obj = is_not_in_promisor_pack_obj;
		}
		get_object_list(&revs, &rp);
		release_revisions(&revs);
	}
	cleanup_preferred_base();
	ଯଦି (include_tag && nr_result)
		refs_for_each_tag_ref(get_main_ref_store(the_repository),
				      add_ref_tag, ଖାଲି);
	stop_progress(&progress_state);
	trace2_region_leave("pack-objects", "enumerate-objects",
			    the_repository);

	ଯଦି (non_empty && !nr_result)
		ଯାଅ cleanup;
	ଯଦି (nr_result) {
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

	ଯଦି (progress)
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

	ଫେରନ୍ତୁ 0;
}
