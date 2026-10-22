#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "hex.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "attr.h"
#ಅನ್ತರ್ಭಾವ "object.h"
#ಅನ್ತರ್ಭಾವ "commit.h"
#ಅನ್ತರ್ಭಾವ "tag.h"
#ಅನ್ತರ್ಭಾವ "delta.h"
#ಅನ್ತರ್ಭಾವ "pack.h"
#ಅನ್ತರ್ಭಾವ "pack-revindex.h"
#ಅನ್ತರ್ಭಾವ "csum-file.h"
#ಅನ್ತರ್ಭಾವ "tree-walk.h"
#ಅನ್ತರ್ಭಾವ "diff.h"
#ಅನ್ತರ್ಭಾವ "revision.h"
#ಅನ್ತರ್ಭಾವ "list-objects.h"
#ಅನ್ತರ್ಭಾವ "list-objects-filter-options.h"
#ಅನ್ತರ್ಭಾವ "pack-objects.h"
#ಅನ್ತರ್ಭಾವ "progress.h"
#ಅನ್ತರ್ಭಾವ "refs.h"
#ಅನ್ತರ್ಭಾವ "thread-utils.h"
#ಅನ್ತರ್ಭಾವ "pack-bitmap.h"
#ಅನ್ತರ್ಭಾವ "delta-islands.h"
#ಅನ್ತರ್ಭಾವ "reachable.h"
#ಅನ್ತರ್ಭಾವ "oid-array.h"
#ಅನ್ತರ್ಭಾವ "strvec.h"
#ಅನ್ತರ್ಭಾವ "strmap.h"
#ಅನ್ತರ್ಭಾವ "list.h"
#ಅನ್ತರ್ಭಾವ "packfile.h"
#ಅನ್ತರ್ಭಾವ "object-file.h"
#ಅನ್ತರ್ಭಾವ "object-file-convert.h"
#ಅನ್ತರ್ಭಾವ "odb.h"
#ಅನ್ತರ್ಭಾವ "odb/streaming.h"
#ಅನ್ತರ್ಭಾವ "replace-object.h"
#ಅನ್ತರ್ಭಾವ "dir.h"
#ಅನ್ತರ್ಭಾವ "midx.h"
#ಅನ್ತರ್ಭಾವ "trace2.h"
#ಅನ್ತರ್ಭಾವ "shallow.h"
#ಅನ್ತರ್ಭಾವ "promisor-remote.h"
#ಅನ್ತರ್ಭಾವ "pack-mtimes.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "pkt-line.h"
#ಅನ್ತರ್ಭಾವ "blob.h"
#ಅನ್ತರ್ಭಾವ "tree.h"
#ಅನ್ತರ್ಭಾವ "path-walk.h"

/*
 * Objects we are going to pack are collected in the `to_pack` structure.
 * It contains an array (dynamically expanded) of the object data, and a map
 * that can resolve SHA1s to their position in the array.
 */
ಸ್ಥಿರ ಸಂರಚನಾ packing_data to_pack;

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಸಂರಚನಾ object_entry *oe_delta(
		ಧ್ರುವ ಸಂರಚನಾ packing_data *pack,
		ಧ್ರುವ ಸಂರಚನಾ object_entry *e)
{
	ಯದಿ (!e->delta_idx)
		ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
	ಯದಿ (e->ext_base)
		ಪ್ರತ್ಯಾಗಮ &pack->ext_bases[e->delta_idx - 1];
	ಅನ್ಯಥಾ
		ಪ್ರತ್ಯಾಗಮ &pack->objects[e->delta_idx - 1];
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ size_t oe_delta_size(ಸಂರಚನಾ packing_data *pack,
				   ಧ್ರುವ ಸಂರಚನಾ object_entry *e)
{
	ಯದಿ (e->delta_size_valid)
		ಪ್ರತ್ಯಾಗಮ e->delta_size_;

	/*
	 * pack->delta_size[] can't be NULL because oe_set_delta_size()
	 * must have been called when a new delta is saved with
	 * oe_set_delta().
	 * If oe_delta() returns NULL (i.e. default state, which means
	 * delta_size_valid is also false), then the caller must never
	 * call oe_delta_size().
	 */
	ಪ್ರತ್ಯಾಗಮ pack->delta_size[e - pack->objects];
}

size_t oe_get_size_slow(ಸಂರಚನಾ packing_data *pack,
			ಧ್ರುವ ಸಂರಚನಾ object_entry *e);

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ size_t oe_size(ಸಂರಚನಾ packing_data *pack,
			     ಧ್ರುವ ಸಂರಚನಾ object_entry *e)
{
	ಯದಿ (e->size_valid)
		ಪ್ರತ್ಯಾಗಮ e->size_;

	ಪ್ರತ್ಯಾಗಮ oe_get_size_slow(pack, e);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ oe_set_delta(ಸಂರಚನಾ packing_data *pack,
				ಸಂರಚನಾ object_entry *e,
				ಸಂರಚನಾ object_entry *delta)
{
	ಯದಿ (delta)
		e->delta_idx = (delta - pack->objects) + 1;
	ಅನ್ಯಥಾ
		e->delta_idx = 0;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಸಂರಚನಾ object_entry *oe_delta_sibling(
		ಧ್ರುವ ಸಂರಚನಾ packing_data *pack,
		ಧ್ರುವ ಸಂರಚನಾ object_entry *e)
{
	ಯದಿ (e->delta_sibling_idx)
		ಪ್ರತ್ಯಾಗಮ &pack->objects[e->delta_sibling_idx - 1];
	ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಸಂರಚನಾ object_entry *oe_delta_child(
		ಧ್ರುವ ಸಂರಚನಾ packing_data *pack,
		ಧ್ರುವ ಸಂರಚನಾ object_entry *e)
{
	ಯದಿ (e->delta_child_idx)
		ಪ್ರತ್ಯಾಗಮ &pack->objects[e->delta_child_idx - 1];
	ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ oe_set_delta_child(ಸಂರಚನಾ packing_data *pack,
				      ಸಂರಚನಾ object_entry *e,
				      ಸಂರಚನಾ object_entry *delta)
{
	ಯದಿ (delta)
		e->delta_child_idx = (delta - pack->objects) + 1;
	ಅನ್ಯಥಾ
		e->delta_child_idx = 0;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ oe_set_delta_sibling(ಸಂರಚನಾ packing_data *pack,
					ಸಂರಚನಾ object_entry *e,
					ಸಂರಚನಾ object_entry *delta)
{
	ಯದಿ (delta)
		e->delta_sibling_idx = (delta - pack->objects) + 1;
	ಅನ್ಯಥಾ
		e->delta_sibling_idx = 0;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ oe_set_size(ಸಂರಚನಾ packing_data *pack,
			       ಸಂರಚನಾ object_entry *e,
			       size_t size)
{
	ಯದಿ (size < pack->oe_size_limit) {
		e->size_ = size;
		e->size_valid = 1;
	} ಅನ್ಯಥಾ {
		e->size_valid = 0;
		ಯದಿ (oe_get_size_slow(pack, e) != size)
			BUG("'size' is supposed to be the object size!");
	}
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ oe_set_delta_size(ಸಂರಚನಾ packing_data *pack,
				     ಸಂರಚನಾ object_entry *e,
				     size_t size)
{
	ಯದಿ (size < pack->oe_delta_size_limit) {
		e->delta_size_ = size;
		e->delta_size_valid = 1;
	} ಅನ್ಯಥಾ {
		packing_data_lock(pack);
		ಯದಿ (!pack->delta_size)
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

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *ಧ್ರುವ pack_usage[] = {
	N_("git pack-objects [-q | --progress | --all-progress] [--all-progress-implied]\n"
	   "                 [--no-reuse-delta] [--delta-base-offset] [--non-empty]\n"
	   "                 [--local] [--incremental] [--window=<n>] [--depth=<n>]\n"
	   "                 [--revs [--unpacked | --all]] [--keep-pack=<pack-name>]\n"
	   "                 [--cruft] [--cruft-expiration=<time>]\n"
	   "                 [--stdout [--filter=<filter-spec>] | <base-name>]\n"
	   "                 [--shallow] [--keep-true-parents] [--[no-]sparse]\n"
	   "                 [--name-hash-version=<n>] [--path-walk] < <object-list>"),
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಸಂರಚನಾ pack_idx_entry **written_list;
ಸ್ಥಿರ uint32_t nr_result, nr_written, nr_seen;
ಸ್ಥಿರ ಸಂರಚನಾ bitmap_index *bitmap_git;
ಸ್ಥಿರ uint32_t write_layer;

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ non_empty;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ reuse_delta = 1, reuse_object = 1;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ keep_unreachable, unpack_unreachable, include_tag;
ಸ್ಥಿರ timestamp_t unpack_unreachable_expiration;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ pack_loose_unreachable;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cruft;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ shallow = 0;
ಸ್ಥಿರ timestamp_t cruft_expiration;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ local;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ have_non_local_packs;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ incremental;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ ignore_packed_keep_on_disk;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ ignore_packed_keep_in_core;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ ignore_packed_keep_in_core_open;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ ignore_packed_keep_in_core_has_cruft;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ allow_ofs_delta;
ಸ್ಥಿರ ಸಂರಚನಾ pack_idx_option pack_idx_opts;
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *base_name;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ progress = 1;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ window = 10;
ಸ್ಥಿರ unsigned long pack_size_limit;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ depth = 50;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ delta_ಹುಡುಕು_threads;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ pack_to_stdout;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ sparse;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ thin;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ path_walk = -1;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_preferred_base;
ಸ್ಥಿರ ಸಂರಚನಾ progress *progress_state;

ಸ್ಥಿರ ಸಂರಚನಾ bitmapped_pack *reuse_packfiles;
ಸ್ಥಿರ size_t reuse_packfiles_nr;
ಸ್ಥಿರ size_t reuse_packfiles_used_nr;
ಸ್ಥಿರ uint32_t reuse_packfile_objects;
ಸ್ಥಿರ ಸಂರಚನಾ bitmap *reuse_packfile_bitmap;

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ use_bitmap_index_default = 1;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ use_bitmap_index = -1;
ಸ್ಥಿರ ಗಣನಾ {
	NO_PACK_REUSE = 0,
	SINGLE_PACK_REUSE,
	MULTI_PACK_REUSE,
} allow_pack_reuse = SINGLE_PACK_REUSE;
ಸ್ಥಿರ ಗಣನಾ {
	WRITE_BITMAP_FALSE = 0,
	WRITE_BITMAP_QUIET,
	WRITE_BITMAP_TRUE,
} write_bitmap_index;
ಸ್ಥಿರ uint16_t write_bitmap_options = BITMAP_OPT_HASH_CACHE;

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ exclude_promisor_objects;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ exclude_promisor_objects_best_effort;

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ use_delta_islands;

ಸ್ಥಿರ size_t delta_cache_size = 0;
ಸ್ಥಿರ size_t max_delta_cache_size = DEFAULT_DELTA_CACHE_SIZE;
ಸ್ಥಿರ unsigned long cache_max_small_delta_size = 1000;

ಸ್ಥಿರ unsigned long window_memory_limit = 0;

ಸ್ಥಿರ ಸಂರಚನಾ string_list uri_protocols = STRING_LIST_INIT_NODUP;

ಗಣನಾ missing_action {
	MA_ERROR = 0,      /* fail if any missing objects are encountered */
	MA_ALLOW_ANY,      /* silently allow ALL missing objects */
	MA_ALLOW_PROMISOR, /* silently allow all missing PROMISOR objects */
};
ಸ್ಥಿರ ಗಣನಾ missing_action arg_missing_action;
ಸ್ಥಿರ show_object_fn fn_show_object;

ಸಂರಚನಾ configured_exclusion {
	ಸಂರಚನಾ oidmap_entry e;
	ವರ್ಣ *pack_hash_hex;
	ವರ್ಣ *uri;
};
ಸ್ಥಿರ ಸಂರಚನಾ oidmap configured_exclusions;

ಸ್ಥಿರ ಸಂರಚನಾ oidset excluded_by_config;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ name_hash_version = -1;

ಗಣನಾ stdin_packs_mode {
	STDIN_PACKS_MODE_NONE,
	STDIN_PACKS_MODE_STANDARD,
	STDIN_PACKS_MODE_FOLLOW,
};

/**
 * Check whether the name_hash_version chosen by user input is appropriate,
 * and also validate whether it is compatible with other features.
 */
ಸ್ಥಿರ ಶೂನ್ಯ validate_name_hash_version(ಶೂನ್ಯ)
{
	ಯದಿ (name_hash_version < 1 || name_hash_version > 2)
		die(_("invalid --name-hash-version option: %d"), name_hash_version);
	ಯದಿ (write_bitmap_index && name_hash_version != 1) {
		warning(_("currently, --write-bitmap-index requires --name-hash-version=1"));
		name_hash_version = 1;
	}
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ uint32_t pack_name_hash_fn(ಧ್ರುವ ವರ್ಣ *name)
{
	ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ seen_version = -1;

	ಯದಿ (seen_version < 0)
		seen_version = name_hash_version;
	ಅನ್ಯಥಾ ಯದಿ (seen_version != name_hash_version)
		BUG("name hash version changed from %d to %d mid-process",
		    seen_version, name_hash_version);

	ವಿಕಲ್ಪ (name_hash_version) {
	ಪಕ್ಷ 1:
		ಪ್ರತ್ಯಾಗಮ pack_name_hash(name);

	ಪಕ್ಷ 2:
		ಪ್ರತ್ಯಾಗಮ pack_name_hash_v2((ಧ್ರುವ unsigned ವರ್ಣ *)name);

	ಸ್ವಭಾವ:
		BUG("invalid name-hash version: %d", name_hash_version);
	}
}

/*
 * stats
 */
ಸ್ಥಿರ uint32_t written, written_delta;
ಸ್ಥಿರ uint32_t reused, reused_delta;

/*
 * Indexed commits
 */
ಸ್ಥಿರ ಸಂರಚನಾ commit **indexed_commits;
ಸ್ಥಿರ unsigned ಪೂರ್ಣಾಙ್ಕ indexed_commits_nr;
ಸ್ಥಿರ unsigned ಪೂರ್ಣಾಙ್ಕ indexed_commits_alloc;

ಸ್ಥಿರ ಶೂನ್ಯ index_commit_for_bitmap(ಸಂರಚನಾ commit *commit)
{
	ಯದಿ (indexed_commits_nr >= indexed_commits_alloc) {
		indexed_commits_alloc = (indexed_commits_alloc + 32) * 2;
		REALLOC_ARRAY(indexed_commits, indexed_commits_alloc);
	}

	indexed_commits[indexed_commits_nr++] = commit;
}

ಸ್ಥಿರ ಶೂನ್ಯ *get_delta(ಸಂರಚನಾ object_entry *entry)
{
	unsigned long size, base_size;
	size_t delta_size;
	ಶೂನ್ಯ *buf, *base_buf, *delta_buf;
	ಗಣನಾ object_type type;
	size_t size_st = 0, base_size_st = 0;

	buf = odb_read_object(the_repository->objects, &entry->idx.oid,
			      &type, &size_st);
	size = cast_size_t_to_ulong(size_st);
	ಯದಿ (!buf)
		die(_("unable to read %s"), oid_to_hex(&entry->idx.oid));
	base_buf = odb_read_object(the_repository->objects,
				   &DELTA(entry)->idx.oid, &type,
				   &base_size_st);
	base_size = cast_size_t_to_ulong(base_size_st);
	ಯದಿ (!base_buf)
		die("unable to read %s",
		    oid_to_hex(&DELTA(entry)->idx.oid));
	delta_buf = diff_delta(base_buf, base_size,
			       buf, size, &delta_size, 0);
	/*
	 * We successfully computed this delta once but dropped it for
	 * memory reasons. Something is very wrong if this time we
	 * recompute and create a different delta.
	 */
	ಯದಿ (!delta_buf || delta_size != DELTA_SIZE(entry))
		BUG("delta size changed");
	ಮುಕ್ತ(buf);
	ಮುಕ್ತ(base_buf);
	ಪ್ರತ್ಯಾಗಮ delta_buf;
}

ಸ್ಥಿರ unsigned long do_compress(ಶೂನ್ಯ **pptr, unsigned long size)
{
	git_zstream stream;
	ಶೂನ್ಯ *in, *out;
	unsigned long maxsize;
	ಸಂರಚನಾ repo_config_ಮೌಲ್ಯಗಳು *cfg = repo_config_ಮೌಲ್ಯಗಳು(the_repository);

	git_deflate_init(&stream, cfg->pack_compression_level);
	maxsize = git_deflate_bound(&stream, size);

	in = *pptr;
	out = xmalloc(maxsize);
	*pptr = out;

	stream.next_in = in;
	stream.avail_in = size;
	stream.next_out = out;
	stream.avail_out = maxsize;
	ಯಾವತ್ (git_deflate(&stream, Z_FINISH) == Z_OK)
		; /* nothing */
	git_deflate_end(&stream);

	ಮುಕ್ತ(in);
	ಪ್ರತ್ಯಾಗಮ stream.total_out;
}

ಸ್ಥಿರ unsigned long write_large_blob_data(ಸಂರಚನಾ odb_stream *st, ಸಂರಚನಾ hashfile *f,
					   ಧ್ರುವ ಸಂರಚನಾ object_id *oid)
{
	git_zstream stream;
	unsigned ವರ್ಣ ibuf[1024 * 16];
	unsigned ವರ್ಣ obuf[1024 * 16];
	unsigned long olen = 0;
	ಸಂರಚನಾ repo_config_ಮೌಲ್ಯಗಳು *cfg = repo_config_ಮೌಲ್ಯಗಳು(the_repository);

	git_deflate_init(&stream, cfg->pack_compression_level);

	ಪ್ರತಿ (;;) {
		ssize_t readlen;
		ಪೂರ್ಣಾಙ್ಕ zret = Z_OK;
		readlen = odb_stream_read(st, ibuf, ಪರಿಮಾಣ(ibuf));
		ಯದಿ (readlen == -1)
			die(_("unable to read %s"), oid_to_hex(oid));

		stream.next_in = ibuf;
		stream.avail_in = readlen;
		ಯಾವತ್ ((stream.avail_in || readlen == 0) &&
		       (zret == Z_OK || zret == Z_BUF_ERROR)) {
			stream.next_out = obuf;
			stream.avail_out = ಪರಿಮಾಣ(obuf);
			zret = git_deflate(&stream, readlen ? 0 : Z_FINISH);
			hashwrite(f, obuf, stream.next_out - obuf);
			olen += stream.next_out - obuf;
		}
		ಯದಿ (stream.avail_in)
			die(_("deflate error (%d)"), zret);
		ಯದಿ (readlen == 0) {
			ಯದಿ (zret != Z_STREAM_END)
				die(_("deflate error (%d)"), zret);
			ಭಙ್ಗ;
		}
	}
	git_deflate_end(&stream);
	ಪ್ರತ್ಯಾಗಮ olen;
}

/*
 * we are going to reuse the existing object data as is.  make
 * sure it is not corrupt.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ check_pack_inflate(ಸಂರಚನಾ packed_git *p,
		ಸಂರಚನಾ pack_window **w_curs,
		off_t offset,
		off_t len,
		size_t expect)
{
	git_zstream stream;
	unsigned ವರ್ಣ fakebuf[4096], *in;
	ಪೂರ್ಣಾಙ್ಕ st;

	memset(&stream, 0, ಪರಿಮಾಣ(stream));
	git_inflate_init(&stream);
	ಕುರು {
		in = use_pack(p, w_curs, offset, &stream.avail_in);
		stream.next_in = in;
		stream.next_out = fakebuf;
		stream.avail_out = ಪರಿಮಾಣ(fakebuf);
		st = git_inflate(&stream, Z_FINISH);
		offset += stream.next_in - in;
	} ಯಾವತ್ (st == Z_OK || st == Z_BUF_ERROR);
	git_inflate_end(&stream);
	ಪ್ರತ್ಯಾಗಮ (st == Z_STREAM_END &&
		stream.total_out == expect &&
		stream.total_in == len) ? 0 : -1;
}

ಸ್ಥಿರ ಶೂನ್ಯ copy_pack_data(ಸಂರಚನಾ hashfile *f,
		ಸಂರಚನಾ packed_git *p,
		ಸಂರಚನಾ pack_window **w_curs,
		off_t offset,
		off_t len)
{
	unsigned ವರ್ಣ *in;
	size_t avail;

	ಯಾವತ್ (len) {
		in = use_pack(p, w_curs, offset, &avail);
		ಯದಿ (avail > len)
			avail = (unsigned long)len;
		hashwrite(f, in, avail);
		offset += avail;
		len -= avail;
	}
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ oe_size_greater_than(ಸಂರಚನಾ packing_data *pack,
				       ಧ್ರುವ ಸಂರಚನಾ object_entry *lhs,
				       size_t rhs)
{
	ಯದಿ (lhs->size_valid)
		ಪ್ರತ್ಯಾಗಮ lhs->size_ > rhs;
	ಯದಿ (rhs < pack->oe_size_limit) /* rhs < 2^x <= lhs ? */
		ಪ್ರತ್ಯಾಗಮ 1;
	ಪ್ರತ್ಯಾಗಮ oe_get_size_slow(pack, lhs) > rhs;
}

/* Return 0 if we will bust the pack-size limit */
ಸ್ಥಿರ unsigned long write_no_reuse_object(ಸಂರಚನಾ hashfile *f, ಸಂರಚನಾ object_entry *entry,
					   unsigned long limit, ಪೂರ್ಣಾಙ್ಕ usable_delta)
{
	unsigned long size, datalen;
	unsigned ವರ್ಣ header[MAX_PACK_OBJECT_HEADER],
		      dheader[MAX_PACK_OBJECT_HEADER];
	unsigned hdrlen;
	ಗಣನಾ object_type type;
	ಶೂನ್ಯ *buf;
	ಸಂರಚನಾ odb_stream *st = ರಿಕ್ತ;
	ಧ್ರುವ unsigned hashsz = the_hash_algo->rawsz;

	ಯದಿ (!usable_delta) {
		ಯದಿ (oe_type(entry) == OBJ_BLOB &&
		    oe_size_greater_than(&to_pack, entry,
					 repo_settings_get_big_file_threshold(the_repository)) &&
		    (st = odb_stream_from_object(the_repository->objects, &entry->idx.oid,
						 ರಿಕ್ತ)) != ರಿಕ್ತ) {
			buf = ರಿಕ್ತ;
			type = st->type;
			size = st->size;
		} ಅನ್ಯಥಾ {
			size_t size_st = 0;
			buf = odb_read_object(the_repository->objects,
					      &entry->idx.oid, &type,
					      &size_st);
			size = cast_size_t_to_ulong(size_st);
			ಯದಿ (!buf)
				die(_("unable to read %s"),
				    oid_to_hex(&entry->idx.oid));
		}
		/*
		 * make sure no cached delta data remains from a
		 * previous attempt before a pack split occurred.
		 */
		FREE_AND_NULL(entry->delta_data);
		entry->z_delta_size = 0;
	} ಅನ್ಯಥಾ ಯದಿ (entry->delta_data) {
		size = DELTA_SIZE(entry);
		buf = entry->delta_data;
		entry->delta_data = ರಿಕ್ತ;
		type = (allow_ofs_delta && DELTA(entry)->idx.offset) ?
			OBJ_OFS_DELTA : OBJ_REF_DELTA;
	} ಅನ್ಯಥಾ {
		buf = get_delta(entry);
		size = DELTA_SIZE(entry);
		type = (allow_ofs_delta && DELTA(entry)->idx.offset) ?
			OBJ_OFS_DELTA : OBJ_REF_DELTA;
	}

	ಯದಿ (st)	/* large blob case, just assume we don't compress well */
		datalen = size;
	ಅನ್ಯಥಾ ಯದಿ (entry->z_delta_size)
		datalen = entry->z_delta_size;
	ಅನ್ಯಥಾ
		datalen = do_compress(&buf, size);

	/*
	 * The object header is a byte of 'type' followed by zero or
	 * more bytes of length.
	 */
	hdrlen = encode_in_pack_object_header(header, ಪರಿಮಾಣ(header),
					      type, size);

	ಯದಿ (type == OBJ_OFS_DELTA) {
		/*
		 * Deltas with relative base contain an additional
		 * encoding of the relative offset for the delta
		 * base from this object's position in the pack.
		 */
		off_t ofs = entry->idx.offset - DELTA(entry)->idx.offset;
		unsigned pos = ಪರಿಮಾಣ(dheader) - 1;
		dheader[pos] = ofs & 127;
		ಯಾವತ್ (ofs >>= 7)
			dheader[--pos] = 128 | (--ofs & 127);
		ಯದಿ (limit && hdrlen + ಪರಿಮಾಣ(dheader) - pos + datalen + hashsz >= limit) {
			ಯದಿ (st)
				odb_stream_close(st);
			ಮುಕ್ತ(buf);
			ಪ್ರತ್ಯಾಗಮ 0;
		}
		hashwrite(f, header, hdrlen);
		hashwrite(f, dheader + pos, ಪರಿಮಾಣ(dheader) - pos);
		hdrlen += ಪರಿಮಾಣ(dheader) - pos;
	} ಅನ್ಯಥಾ ಯದಿ (type == OBJ_REF_DELTA) {
		/*
		 * Deltas with a base reference contain
		 * additional bytes for the base object ID.
		 */
		ಯದಿ (limit && hdrlen + hashsz + datalen + hashsz >= limit) {
			ಯದಿ (st)
				odb_stream_close(st);
			ಮುಕ್ತ(buf);
			ಪ್ರತ್ಯಾಗಮ 0;
		}
		hashwrite(f, header, hdrlen);
		hashwrite(f, DELTA(entry)->idx.oid.hash, hashsz);
		hdrlen += hashsz;
	} ಅನ್ಯಥಾ {
		ಯದಿ (limit && hdrlen + datalen + hashsz >= limit) {
			ಯದಿ (st)
				odb_stream_close(st);
			ಮುಕ್ತ(buf);
			ಪ್ರತ್ಯಾಗಮ 0;
		}
		hashwrite(f, header, hdrlen);
	}
	ಯದಿ (st) {
		datalen = write_large_blob_data(st, f, &entry->idx.oid);
		odb_stream_close(st);
	} ಅನ್ಯಥಾ {
		hashwrite(f, buf, datalen);
		ಮುಕ್ತ(buf);
	}

	ಪ್ರತ್ಯಾಗಮ hdrlen + datalen;
}

/* Return 0 if we will bust the pack-size limit */
ಸ್ಥಿರ off_t write_reuse_object(ಸಂರಚನಾ hashfile *f, ಸಂರಚನಾ object_entry *entry,
				unsigned long limit, ಪೂರ್ಣಾಙ್ಕ usable_delta)
{
	ಸಂರಚನಾ packed_git *p = IN_PACK(entry);
	ಸಂರಚನಾ pack_window *w_curs = ರಿಕ್ತ;
	uint32_t pos;
	off_t offset, cur;
	ಗಣನಾ object_type type = oe_type(entry);
	ಗಣನಾ object_type in_pack_type;
	off_t datalen;
	unsigned ವರ್ಣ header[MAX_PACK_OBJECT_HEADER],
		      dheader[MAX_PACK_OBJECT_HEADER];
	unsigned hdrlen;
	ಧ್ರುವ unsigned hashsz = the_hash_algo->rawsz;
	size_t entry_size;

	cur = entry->in_pack_offset;
	in_pack_type = unpack_object_header(p, &w_curs, &cur, &entry_size);
	ಯದಿ (in_pack_type < 0)
		die(_("write_reuse_object: unable to parse object header of %s"),
		    oid_to_hex(&entry->idx.oid));

	ಯದಿ (DELTA(entry))
		type = (allow_ofs_delta && DELTA(entry)->idx.offset) ?
			OBJ_OFS_DELTA : OBJ_REF_DELTA;
	hdrlen = encode_in_pack_object_header(header, ಪರಿಮಾಣ(header),
					      type, entry_size);

	offset = entry->in_pack_offset;
	ಯದಿ (offset_to_pack_pos(p, offset, &pos) < 0)
		die(_("write_reuse_object: could not locate %s, expected at "
		      "offset %"PRIuMAX" in pack %s"),
		    oid_to_hex(&entry->idx.oid), (uintmax_t)offset,
		    p->pack_name);
	datalen = pack_pos_to_offset(p, pos + 1) - offset;
	ಯದಿ (!pack_to_stdout && p->index_version > 1 &&
	    check_pack_crc(p, &w_curs, offset, datalen,
			   pack_pos_to_index(p, pos))) {
		error(_("bad packed object CRC for %s"),
		      oid_to_hex(&entry->idx.oid));
		unuse_pack(&w_curs);
		ಪ್ರತ್ಯಾಗಮ write_no_reuse_object(f, entry, limit, usable_delta);
	}

	offset += entry->in_pack_header_size;
	datalen -= entry->in_pack_header_size;

	ಯದಿ (!pack_to_stdout && p->index_version == 1 &&
	    check_pack_inflate(p, &w_curs, offset, datalen, entry_size)) {
		error(_("corrupt packed object for %s"),
		      oid_to_hex(&entry->idx.oid));
		unuse_pack(&w_curs);
		ಪ್ರತ್ಯಾಗಮ write_no_reuse_object(f, entry, limit, usable_delta);
	}

	ಯದಿ (type == OBJ_OFS_DELTA) {
		off_t ofs = entry->idx.offset - DELTA(entry)->idx.offset;
		unsigned pos = ಪರಿಮಾಣ(dheader) - 1;
		dheader[pos] = ofs & 127;
		ಯಾವತ್ (ofs >>= 7)
			dheader[--pos] = 128 | (--ofs & 127);
		ಯದಿ (limit && hdrlen + ಪರಿಮಾಣ(dheader) - pos + datalen + hashsz >= limit) {
			unuse_pack(&w_curs);
			ಪ್ರತ್ಯಾಗಮ 0;
		}
		hashwrite(f, header, hdrlen);
		hashwrite(f, dheader + pos, ಪರಿಮಾಣ(dheader) - pos);
		hdrlen += ಪರಿಮಾಣ(dheader) - pos;
		reused_delta++;
	} ಅನ್ಯಥಾ ಯದಿ (type == OBJ_REF_DELTA) {
		ಯದಿ (limit && hdrlen + hashsz + datalen + hashsz >= limit) {
			unuse_pack(&w_curs);
			ಪ್ರತ್ಯಾಗಮ 0;
		}
		hashwrite(f, header, hdrlen);
		hashwrite(f, DELTA(entry)->idx.oid.hash, hashsz);
		hdrlen += hashsz;
		reused_delta++;
	} ಅನ್ಯಥಾ {
		ಯದಿ (limit && hdrlen + datalen + hashsz >= limit) {
			unuse_pack(&w_curs);
			ಪ್ರತ್ಯಾಗಮ 0;
		}
		hashwrite(f, header, hdrlen);
	}
	copy_pack_data(f, p, &w_curs, offset, datalen);
	unuse_pack(&w_curs);
	reused++;
	ಪ್ರತ್ಯಾಗಮ hdrlen + datalen;
}

/* Return 0 if we will bust the pack-size limit */
ಸ್ಥಿರ off_t write_object(ಸಂರಚನಾ hashfile *f,
			  ಸಂರಚನಾ object_entry *entry,
			  off_t write_offset)
{
	unsigned long limit;
	off_t len;
	ಪೂರ್ಣಾಙ್ಕ usable_delta, to_reuse;

	ಯದಿ (!pack_to_stdout)
		crc32_begin(f);

	/* apply size limit if limited packsize and not first object */
	ಯದಿ (!pack_size_limit || !nr_written)
		limit = 0;
	ಅನ್ಯಥಾ ಯದಿ (pack_size_limit <= write_offset)
		/*
		 * the earlier object did not fit the limit; avoid
		 * mistaking this with unlimited (i.e. limit = 0).
		 */
		limit = 1;
	ಅನ್ಯಥಾ
		limit = pack_size_limit - write_offset;

	ಯದಿ (!DELTA(entry))
		usable_delta = 0;	/* no delta */
	ಅನ್ಯಥಾ ಯದಿ (!pack_size_limit)
	       usable_delta = 1;	/* unlimited packfile */
	ಅನ್ಯಥಾ ಯದಿ (DELTA(entry)->idx.offset == (off_t)-1)
		usable_delta = 0;	/* base was written to another pack */
	ಅನ್ಯಥಾ ಯದಿ (DELTA(entry)->idx.offset)
		usable_delta = 1;	/* base already exists in this pack */
	ಅನ್ಯಥಾ
		usable_delta = 0;	/* base could end up in another pack */

	ಯದಿ (!reuse_object)
		to_reuse = 0;	/* explicit */
	ಅನ್ಯಥಾ ಯದಿ (!IN_PACK(entry))
		to_reuse = 0;	/* can't reuse what we don't have */
	ಅನ್ಯಥಾ ಯದಿ (oe_type(entry) == OBJ_REF_DELTA ||
		 oe_type(entry) == OBJ_OFS_DELTA)
				/* check_object() decided it for us ... */
		to_reuse = usable_delta;
				/* ... but pack split may override that */
	ಅನ್ಯಥಾ ಯದಿ (oe_type(entry) != entry->in_pack_type)
		to_reuse = 0;	/* pack has delta which is unusable */
	ಅನ್ಯಥಾ ಯದಿ (DELTA(entry))
		to_reuse = 0;	/* we want to pack afresh */
	ಅನ್ಯಥಾ
		to_reuse = 1;	/* we have it in-pack undeltified,
				 * and we do not need to deltify it.
				 */

	ಯದಿ (!to_reuse)
		len = write_no_reuse_object(f, entry, limit, usable_delta);
	ಅನ್ಯಥಾ
		len = write_reuse_object(f, entry, limit, usable_delta);
	ಯದಿ (!len)
		ಪ್ರತ್ಯಾಗಮ 0;

	ಯದಿ (usable_delta)
		written_delta++;
	written++;
	ಯದಿ (!pack_to_stdout)
		entry->idx.crc32 = crc32_end(f);
	ಪ್ರತ್ಯಾಗಮ len;
}

ಗಣನಾ write_one_status {
	WRITE_ONE_SKIP = -1, /* already written */
	WRITE_ONE_BREAK = 0, /* writing this will bust the limit; not written */
	WRITE_ONE_WRITTEN = 1, /* normal */
	WRITE_ONE_RECURSIVE = 2 /* already scheduled to be written */
};

ಸ್ಥಿರ ಗಣನಾ write_one_status write_one(ಸಂರಚನಾ hashfile *f,
				       ಸಂರಚನಾ object_entry *e,
				       off_t *offset)
{
	off_t size;
	ಪೂರ್ಣಾಙ್ಕ recursing;

	/*
	 * we set offset to 1 (which is an impossible value) to mark
	 * the fact that this object is involved in "write its base
	 * first before writing a deltified object" recursion.
	 */
	recursing = (e->idx.offset == 1);
	ಯದಿ (recursing) {
		warning(_("recursive delta detected for object %s"),
			oid_to_hex(&e->idx.oid));
		ಪ್ರತ್ಯಾಗಮ WRITE_ONE_RECURSIVE;
	} ಅನ್ಯಥಾ ಯದಿ (e->idx.offset || e->preferred_base) {
		/* offset is non zero if object is written already. */
		ಪ್ರತ್ಯಾಗಮ WRITE_ONE_SKIP;
	}

	/* if we are deltified, write out base object first. */
	ಯದಿ (DELTA(e)) {
		e->idx.offset = 1; /* now recurse */
		ವಿಕಲ್ಪ (write_one(f, DELTA(e), offset)) {
		ಪಕ್ಷ WRITE_ONE_RECURSIVE:
			/* we cannot depend on this one */
			SET_DELTA(e, ರಿಕ್ತ);
			ಭಙ್ಗ;
		ಸ್ವಭಾವ:
			ಭಙ್ಗ;
		ಪಕ್ಷ WRITE_ONE_BREAK:
			e->idx.offset = recursing;
			ಪ್ರತ್ಯಾಗಮ WRITE_ONE_BREAK;
		}
	}

	e->idx.offset = *offset;
	size = write_object(f, e, *offset);
	ಯದಿ (!size) {
		e->idx.offset = recursing;
		ಪ್ರತ್ಯಾಗಮ WRITE_ONE_BREAK;
	}
	written_list[nr_written++] = &e->idx;

	/* make sure off_t is sufficiently large not to wrap */
	ಯದಿ (signed_add_overflows(*offset, size))
		die(_("pack too large for current definition of off_t"));
	*offset += size;
	ಪ್ರತ್ಯಾಗಮ WRITE_ONE_WRITTEN;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ mark_tagged(ಧ್ರುವ ಸಂರಚನಾ reference *ref, ಶೂನ್ಯ *cb_data UNUSED)
{
	ಸಂರಚನಾ object_id peeled;
	ಸಂರಚನಾ object_entry *entry = packlist_find(&to_pack, ref->oid);

	ಯದಿ (entry)
		entry->tagged = 1;
	ಯದಿ (!reference_get_peeled_oid(the_repository, ref, &peeled)) {
		entry = packlist_find(&to_pack, &peeled);
		ಯದಿ (entry)
			entry->tagged = 1;
	}
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ unsigned ವರ್ಣ oe_layer(ಸಂರಚನಾ packing_data *pack,
				     ಸಂರಚನಾ object_entry *e)
{
	ಯದಿ (!pack->layer)
		ಪ್ರತ್ಯಾಗಮ 0;
	ಪ್ರತ್ಯಾಗಮ pack->layer[e - pack->objects];
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ add_to_write_order(ಸಂರಚನಾ object_entry **wo,
			       unsigned ಪೂರ್ಣಾಙ್ಕ *endp,
			       ಸಂರಚನಾ object_entry *e)
{
	ಯದಿ (e->filled || oe_layer(&to_pack, e) != write_layer)
		ಪ್ರತ್ಯಾಗಮ;
	wo[(*endp)++] = e;
	e->filled = 1;
}

ಸ್ಥಿರ ಶೂನ್ಯ add_descendants_to_write_order(ಸಂರಚನಾ object_entry **wo,
					   unsigned ಪೂರ್ಣಾಙ್ಕ *endp,
					   ಸಂರಚನಾ object_entry *e)
{
	ಪೂರ್ಣಾಙ್ಕ add_to_order = 1;
	ಯಾವತ್ (e) {
		ಯದಿ (add_to_order) {
			ಸಂರಚನಾ object_entry *s;
			/* add this node... */
			add_to_write_order(wo, endp, e);
			/* all its siblings... */
			ಪ್ರತಿ (s = DELTA_SIBLING(e); s; s = DELTA_SIBLING(s)) {
				add_to_write_order(wo, endp, s);
			}
		}
		/* drop down a level to add left subtree nodes if possible */
		ಯದಿ (DELTA_CHILD(e)) {
			add_to_order = 1;
			e = DELTA_CHILD(e);
		} ಅನ್ಯಥಾ {
			add_to_order = 0;
			/* our sibling might have some children, it is next */
			ಯದಿ (DELTA_SIBLING(e)) {
				e = DELTA_SIBLING(e);
				ಅನುವರ್ತ;
			}
			/* go back to our parent node */
			e = DELTA(e);
			ಯಾವತ್ (e && !DELTA_SIBLING(e)) {
				/* we're on the right side of a subtree, keep
				 * going up until we can go right again */
				e = DELTA(e);
			}
			ಯದಿ (!e) {
				/* done- we hit our original root node */
				ಪ್ರತ್ಯಾಗಮ;
			}
			/* pass it off to sibling at this level */
			e = DELTA_SIBLING(e);
		}
	};
}

ಸ್ಥಿರ ಶೂನ್ಯ add_family_to_write_order(ಸಂರಚನಾ object_entry **wo,
				      unsigned ಪೂರ್ಣಾಙ್ಕ *endp,
				      ಸಂರಚನಾ object_entry *e)
{
	ಸಂರಚನಾ object_entry *ಬೇರು;

	ಪ್ರತಿ (ಬೇರು = e; DELTA(ಬೇರು); ಬೇರು = DELTA(ಬೇರು))
		; /* nothing */
	add_descendants_to_write_order(wo, endp, ಬೇರು);
}

ಸ್ಥಿರ ಶೂನ್ಯ compute_layer_order(ಸಂರಚನಾ object_entry **wo, unsigned ಪೂರ್ಣಾಙ್ಕ *wo_end)
{
	unsigned ಪೂರ್ಣಾಙ್ಕ i, last_untagged;
	ಸಂರಚನಾ object_entry *objects = to_pack.objects;

	ಪ್ರತಿ (i = 0; i < to_pack.nr_objects; i++) {
		ಯದಿ (objects[i].tagged)
			ಭಙ್ಗ;
		add_to_write_order(wo, wo_end, &objects[i]);
	}
	last_untagged = i;

	/*
	 * Then fill all the tagged tips.
	 */
	ಪ್ರತಿ (; i < to_pack.nr_objects; i++) {
		ಯದಿ (objects[i].tagged)
			add_to_write_order(wo, wo_end, &objects[i]);
	}

	/*
	 * And then all remaining commits and tags.
	 */
	ಪ್ರತಿ (i = last_untagged; i < to_pack.nr_objects; i++) {
		ಯದಿ (oe_type(&objects[i]) != OBJ_COMMIT &&
		    oe_type(&objects[i]) != OBJ_TAG)
			ಅನುವರ್ತ;
		add_to_write_order(wo, wo_end, &objects[i]);
	}

	/*
	 * And then all the trees.
	 */
	ಪ್ರತಿ (i = last_untagged; i < to_pack.nr_objects; i++) {
		ಯದಿ (oe_type(&objects[i]) != OBJ_ವೃಕ್ಷ)
			ಅನುವರ್ತ;
		add_to_write_order(wo, wo_end, &objects[i]);
	}

	/*
	 * Finally all the rest in really tight order
	 */
	ಪ್ರತಿ (i = last_untagged; i < to_pack.nr_objects; i++) {
		ಯದಿ (!objects[i].filled && oe_layer(&to_pack, &objects[i]) == write_layer)
			add_family_to_write_order(wo, wo_end, &objects[i]);
	}
}

ಸ್ಥಿರ ಸಂರಚನಾ object_entry **compute_write_order(ಶೂನ್ಯ)
{
	uint32_t max_layers = 1;
	unsigned ಪೂರ್ಣಾಙ್ಕ i, wo_end;

	ಸಂರಚನಾ object_entry **wo;
	ಸಂರಚನಾ object_entry *objects = to_pack.objects;

	ಪ್ರತಿ (i = 0; i < to_pack.nr_objects; i++) {
		objects[i].tagged = 0;
		objects[i].filled = 0;
		SET_DELTA_CHILD(&objects[i], ರಿಕ್ತ);
		SET_DELTA_SIBLING(&objects[i], ರಿಕ್ತ);
	}

	/*
	 * Fully connect delta_child/delta_sibling network.
	 * Make sure delta_sibling is sorted in the original
	 * recency order.
	 */
	ಪ್ರತಿ (i = to_pack.nr_objects; i > 0;) {
		ಸಂರಚನಾ object_entry *e = &objects[--i];
		ಯದಿ (!DELTA(e))
			ಅನುವರ್ತ;
		/* Mark me as the first child */
		e->delta_sibling_idx = DELTA(e)->delta_child_idx;
		SET_DELTA_CHILD(DELTA(e), e);
	}

	/*
	 * Mark objects that are at the tip of tags.
	 */
	refs_for_each_tag_ref(get_main_ref_store(the_repository), mark_tagged,
			      ರಿಕ್ತ);

	ಯದಿ (use_delta_islands) {
		max_layers = compute_pack_layers(&to_pack);
		free_island_marks();
	}

	ALLOC_ARRAY(wo, to_pack.nr_objects);
	wo_end = 0;

	ಪ್ರತಿ (; write_layer < max_layers; ++write_layer)
		compute_layer_order(wo, &wo_end);

	ಯದಿ (wo_end != to_pack.nr_objects)
		die(_("ordered %u objects, expected %"PRIu32),
		    wo_end, to_pack.nr_objects);

	ಪ್ರತ್ಯಾಗಮ wo;
}


/*
 * A reused set of objects. All objects in a chunk have the same
 * relative position in the original packfile and the generated
 * packfile.
 */

ಸ್ಥಿರ ಸಂರಚನಾ reused_chunk {
	/* The offset of the first object of this chunk in the original
	 * packfile. */
	off_t original;
	/* The difference for "original" minus the offset of the first object of
	 * this chunk in the generated packfile. */
	off_t difference;
} *reused_chunks;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ reused_chunks_nr;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ reused_chunks_alloc;

ಸ್ಥಿರ ಶೂನ್ಯ record_reused_object(off_t where, off_t offset)
{
	ಯದಿ (reused_chunks_nr && reused_chunks[reused_chunks_nr-1].difference == offset)
		ಪ್ರತ್ಯಾಗಮ;

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
ಸ್ಥಿರ off_t find_reused_offset(off_t where)
{
	ಪೂರ್ಣಾಙ್ಕ lo = 0, hi = reused_chunks_nr;
	ಯಾವತ್ (lo < hi) {
		ಪೂರ್ಣಾಙ್ಕ mi = lo + ((hi - lo) / 2);
		ಯದಿ (where == reused_chunks[mi].original)
			ಪ್ರತ್ಯಾಗಮ reused_chunks[mi].difference;
		ಯದಿ (where < reused_chunks[mi].original)
			hi = mi;
		ಅನ್ಯಥಾ
			lo = mi + 1;
	}

	/*
	 * The first chunk starts at zero, so we can't have gone below
	 * there.
	 */
	ಪ್ರತಿಜ್ಞಾ(lo);
	ಪ್ರತ್ಯಾಗಮ reused_chunks[lo-1].difference;
}

ಸ್ಥಿರ ಶೂನ್ಯ write_reused_pack_one(ಸಂರಚನಾ packed_git *reuse_packfile,
				  size_t pos, ಸಂರಚನಾ hashfile *out,
				  off_t pack_start,
				  ಸಂರಚನಾ pack_window **w_curs)
{
	off_t offset, next, cur;
	ಗಣನಾ object_type type;
	size_t size;

	offset = pack_pos_to_offset(reuse_packfile, pos);
	next = pack_pos_to_offset(reuse_packfile, pos + 1);

	record_reused_object(offset,
			     offset - (hashfile_total(out) - pack_start));

	cur = offset;
	type = unpack_object_header(reuse_packfile, w_curs, &cur, &size);
	ಪ್ರತಿಜ್ಞಾ(type >= 0);

	ಯದಿ (type == OBJ_OFS_DELTA) {
		off_t base_offset;
		off_t fixup;

		unsigned ವರ್ಣ header[MAX_PACK_OBJECT_HEADER];
		unsigned len;

		base_offset = get_delta_base(reuse_packfile, w_curs, &cur, type, offset);
		ಪ್ರತಿಜ್ಞಾ(base_offset != 0);

		/* Convert to REF_DELTA if we must... */
		ಯದಿ (!allow_ofs_delta) {
			uint32_t base_pos;
			ಸಂರಚನಾ object_id base_oid;

			ಯದಿ (offset_to_pack_pos(reuse_packfile, base_offset, &base_pos) < 0)
				die(_("expected object at offset %"PRIuMAX" "
				      "in pack %s"),
				    (uintmax_t)base_offset,
				    reuse_packfile->pack_name);

			nth_packed_object_id(&base_oid, reuse_packfile,
					     pack_pos_to_index(reuse_packfile, base_pos));

			len = encode_in_pack_object_header(header, ಪರಿಮಾಣ(header),
							   OBJ_REF_DELTA, size);
			hashwrite(out, header, len);
			hashwrite(out, base_oid.hash, the_hash_algo->rawsz);
			copy_pack_data(out, reuse_packfile, w_curs, cur, next - cur);
			ಪ್ರತ್ಯಾಗಮ;
		}

		/* Otherwise see if we need to rewrite the offset... */
		fixup = find_reused_offset(offset) -
			find_reused_offset(base_offset);
		ಯದಿ (fixup) {
			unsigned ವರ್ಣ ofs_header[MAX_PACK_OBJECT_HEADER];
			unsigned i, ofs_len;
			off_t ofs = offset - base_offset - fixup;

			len = encode_in_pack_object_header(header, ಪರಿಮಾಣ(header),
							   OBJ_OFS_DELTA, size);

			i = ಪರಿಮಾಣ(ofs_header) - 1;
			ofs_header[i] = ofs & 127;
			ಯಾವತ್ (ofs >>= 7)
				ofs_header[--i] = 128 | (--ofs & 127);

			ofs_len = ಪರಿಮಾಣ(ofs_header) - i;

			hashwrite(out, header, len);
			hashwrite(out, ofs_header + ಪರಿಮಾಣ(ofs_header) - ofs_len, ofs_len);
			copy_pack_data(out, reuse_packfile, w_curs, cur, next - cur);
			ಪ್ರತ್ಯಾಗಮ;
		}

		/* ...otherwise we have no fixup, and can write it verbatim */
	}

	copy_pack_data(out, reuse_packfile, w_curs, offset, next - offset);
}

ಸ್ಥಿರ size_t write_reused_pack_verbatim(ಸಂರಚನಾ bitmapped_pack *reuse_packfile,
					 ಸಂರಚನಾ hashfile *out,
					 ಸಂರಚನಾ pack_window **w_curs)
{
	size_t pos = 0;
	size_t end;

	ಯದಿ (reuse_packfile->bitmap_pos) {
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
		ಪ್ರತ್ಯಾಗಮ reuse_packfile->bitmap_pos / BITS_IN_EWORD;
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
	ಯದಿ (reuse_packfile_bitmap->word_alloc < end)
		BUG("fewer words than expected in reuse_packfile_bitmap");

	ಯಾವತ್ (pos < end && reuse_packfile_bitmap->ಪದಗಳು[pos] == (eword_t)~0)
		pos++;

	ಯದಿ (pos) {
		off_t to_write;

		written = (pos * BITS_IN_EWORD);
		to_write = pack_pos_to_offset(reuse_packfile->p, written)
			- ಪರಿಮಾಣ(ಸಂರಚನಾ pack_header);

		/* We're recording one chunk, not one object. */
		record_reused_object(ಪರಿಮಾಣ(ಸಂರಚನಾ pack_header), 0);
		hashflush(out);
		copy_pack_data(out, reuse_packfile->p, w_curs,
			ಪರಿಮಾಣ(ಸಂರಚನಾ pack_header), to_write);

		display_progress(progress_state, written);
	}
	ಪ್ರತ್ಯಾಗಮ pos;
}

ಸ್ಥಿರ ಶೂನ್ಯ write_reused_pack(ಸಂರಚನಾ bitmapped_pack *reuse_packfile,
			      ಸಂರಚನಾ hashfile *f)
{
	size_t i = reuse_packfile->bitmap_pos / BITS_IN_EWORD;
	uint32_t offset;
	off_t pack_start = hashfile_total(f) - ಪರಿಮಾಣ(ಸಂರಚನಾ pack_header);
	ಸಂರಚನಾ pack_window *w_curs = ರಿಕ್ತ;

	ಯದಿ (allow_ofs_delta)
		i = write_reused_pack_verbatim(reuse_packfile, f, &w_curs);

	ಪ್ರತಿ (; i < reuse_packfile_bitmap->word_alloc; ++i) {
		eword_t word = reuse_packfile_bitmap->ಪದಗಳು[i];
		size_t pos = (i * BITS_IN_EWORD);

		ಪ್ರತಿ (offset = 0; offset < BITS_IN_EWORD; ++offset) {
			uint32_t pack_pos;
			ಯದಿ ((word >> offset) == 0)
				ಭಙ್ಗ;

			offset += ewah_bit_ctz64(word >> offset);
			ಯದಿ (pos + offset < reuse_packfile->bitmap_pos)
				ಅನುವರ್ತ;
			ಯದಿ (pos + offset >= reuse_packfile->bitmap_pos + reuse_packfile->bitmap_nr)
				ಗಚ್ಛ done;

			ಯದಿ (reuse_packfile->bitmap_pos) {
				/*
				 * When doing multi-pack reuse on a
				 * non-preferred pack, translate bit positions
				 * from the MIDX pseudo-pack order back to their
				 * pack-relative positions before attempting
				 * reuse.
				 */
				ಸಂರಚನಾ multi_pack_index *m = reuse_packfile->from_midx;
				uint32_t midx_pos;
				off_t pack_ofs;

				ಯದಿ (!m)
					BUG("non-zero bitmap position without MIDX");

				midx_pos = pack_pos_to_midx(m, pos + offset);
				pack_ofs = nth_midxed_offset(m, midx_pos);

				ಯದಿ (offset_to_pack_pos(reuse_packfile->p,
						       pack_ofs, &pack_pos) < 0)
					BUG("could not find expected object at offset %"PRIuMAX" in pack %s",
					    (uintmax_t)pack_ofs,
					    pack_basename(reuse_packfile->p));
			} ಅನ್ಯಥಾ {
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

ಸ್ಥಿರ ಶೂನ್ಯ write_excluded_by_configs(ಶೂನ್ಯ)
{
	ಸಂರಚನಾ oidset_iter iter;
	ಧ್ರುವ ಸಂರಚನಾ object_id *oid;

	oidset_iter_init(&excluded_by_config, &iter);
	ಯಾವತ್ ((oid = oidset_iter_next(&iter))) {
		ಸಂರಚನಾ configured_exclusion *ex =
			oidmap_get(&configured_exclusions, oid);

		ಯದಿ (!ex)
			BUG("configured exclusion wasn't configured");
		write_in_full(1, ex->pack_hash_hex, strlen(ex->pack_hash_hex));
		write_in_full(1, " ", 1);
		write_in_full(1, ex->uri, strlen(ex->uri));
		write_in_full(1, "\n", 1);
	}
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ no_split_warning[] = N_(
"disabling bitmap writing, packs are split due to pack.packSizeLimit"
);

ಸ್ಥಿರ ಶೂನ್ಯ write_pack_file(ಶೂನ್ಯ)
{
	uint32_t i = 0, j;
	ಸಂರಚನಾ hashfile *f;
	off_t offset;
	uint32_t nr_remaining = nr_result;
	time_t last_mtime = 0;
	ಸಂರಚನಾ object_entry **write_order;
	off_t bytes_written = 0;

	ಯದಿ (progress > pack_to_stdout)
		progress_state = start_progress(the_repository,
						_("Writing objects"), nr_result);
	ALLOC_ARRAY(written_list, to_pack.nr_objects);
	write_order = compute_write_order();

	ಕುರು {
		unsigned ವರ್ಣ hash[GIT_MAX_RAWSZ];
		ವರ್ಣ *pack_tmp_name = ರಿಕ್ತ;

		ಯದಿ (pack_to_stdout) {
			/*
			 * This command is most often invoked via
			 * git-upload-pack(1), which will typically chunk data
			 * into pktlines. As such, we use the maximum data
			 * length of them as buffer length.
			 *
			 * Note that we need to subtract one though to
			 * accommodate for the sideband byte.
			 */
			ಸಂರಚನಾ hashfd_options opts = {
				.progress = progress_state,
				.buffer_len = LARGE_PACKET_DATA_MAX - 1,
			};
			f = hashfd_ext(the_repository->hash_algo, 1,
				       "<stdout>", &opts);
		} ಅನ್ಯಥಾ {
			f = create_tmp_packfile(the_repository, &pack_tmp_name);
		}

		offset = write_pack_header(f, nr_remaining);

		ಯದಿ (reuse_packfiles_nr) {
			ಪ್ರತಿಜ್ಞಾ(pack_to_stdout);
			ಪ್ರತಿ (j = 0; j < reuse_packfiles_nr; j++) {
				reused_chunks_nr = 0;
				write_reused_pack(&reuse_packfiles[j], f);
				ಯದಿ (reused_chunks_nr)
					reuse_packfiles_used_nr++;
			}
			offset = hashfile_total(f);
		}

		nr_written = 0;
		ಪ್ರತಿ (; i < to_pack.nr_objects; i++) {
			ಸಂರಚನಾ object_entry *e = write_order[i];
			ಯದಿ (write_one(f, e, &offset) == WRITE_ONE_BREAK)
				ಭಙ್ಗ;
			display_progress(progress_state, written);
		}

		bytes_written += hashfile_total(f) +
			the_repository->hash_algo->rawsz;
		ಯದಿ (pack_to_stdout) {
			/*
			 * We never fsync when writing to stdout since we may
			 * not be writing to an actual pack file. For instance,
			 * the upload-pack code passes a pipe here. Calling
			 * fsync on a pipe results in unnecessary
			 * synchronization with the reader on some platforms.
			 */
			finalize_hashfile(f, hash, FSYNC_COMPONENT_NONE,
					  CSUM_HASH_IN_STREAM | CSUM_CLOSE);
		} ಅನ್ಯಥಾ ಯದಿ (nr_written == nr_remaining) {
			finalize_hashfile(f, hash, FSYNC_COMPONENT_PACK,
					  CSUM_HASH_IN_STREAM | CSUM_FSYNC | CSUM_CLOSE);
		} ಅನ್ಯಥಾ {
			/*
			 * If we wrote the wrong number of entries in the
			 * header, rewrite it like in fast-import.
			 */

			ಪೂರ್ಣಾಙ್ಕ fd = finalize_hashfile(f, hash, FSYNC_COMPONENT_PACK, 0);
			fixup_pack_header_footer(the_hash_algo, fd, hash,
						 pack_tmp_name, nr_written,
						 hash, offset);
			close(fd);
			ಯದಿ (write_bitmap_index) {
				ಯದಿ (write_bitmap_index != WRITE_BITMAP_QUIET)
					warning(_(no_split_warning));
				write_bitmap_index = 0;
			}
		}

		ಯದಿ (!pack_to_stdout) {
			ಸಂರಚನಾ stat st;
			ಸಂರಚನಾ strbuf tmpname = STRBUF_INIT;
			ಸಂರಚನಾ bitmap_writer bitmap_writer;
			ವರ್ಣ *idx_tmp_name = ರಿಕ್ತ;

			/*
			 * Packs are runtime accessed in their mtime
			 * order since newer packs are more likely to contain
			 * younger objects.  So if we are creating multiple
			 * packs then we should modify the mtime of later ones
			 * to preserve this property.
			 */
			ಯದಿ (stat(pack_tmp_name, &st) < 0) {
				warning_errno(_("failed to stat %s"), pack_tmp_name);
			} ಅನ್ಯಥಾ ಯದಿ (!last_mtime) {
				last_mtime = st.st_mtime;
			} ಅನ್ಯಥಾ {
				ಸಂರಚನಾ utimbuf utb;
				utb.actime = st.st_atime;
				utb.modtime = --last_mtime;
				ಯದಿ (utime(pack_tmp_name, &utb) < 0)
					warning_errno(_("failed utime() on %s"), pack_tmp_name);
			}

			strbuf_addf(&tmpname, "%s-%s.", base_name,
				    hash_to_hex(hash));

			ಯದಿ (write_bitmap_index) {
				bitmap_writer_init(&bitmap_writer,
						   the_repository, &to_pack,
						   ರಿಕ್ತ);
				bitmap_writer_set_checksum(&bitmap_writer, hash);
				bitmap_writer_build_type_index(&bitmap_writer,
							       written_list);
			}

			ಯದಿ (cruft)
				pack_idx_opts.flags |= WRITE_MTIMES;

			stage_tmp_packfiles(the_repository, &tmpname,
					    pack_tmp_name, written_list,
					    nr_written, &to_pack,
					    &pack_idx_opts, hash,
					    &idx_tmp_name);

			ಯದಿ (write_bitmap_index) {
				size_t tmpname_len = tmpname.len;

				strbuf_addstr(&tmpname, "bitmap");
				stop_progress(&progress_state);

				bitmap_writer_show_progress(&bitmap_writer,
							    progress);
				bitmap_writer_select_commits(&bitmap_writer,
							     indexed_commits,
							     indexed_commits_nr);
				ಯದಿ (bitmap_writer_build(&bitmap_writer) < 0)
					die(_("failed to write bitmap index"));
				bitmap_writer_finish(&bitmap_writer,
						     written_list,
						     tmpname.buf, write_bitmap_options);
				bitmap_writer_free(&bitmap_writer);
				write_bitmap_index = 0;
				strbuf_setlen(&tmpname, tmpname_len);
			}

			rename_tmp_packfile_idx(the_repository, &tmpname, &idx_tmp_name);

			ಮುಕ್ತ(idx_tmp_name);
			strbuf_release(&tmpname);
			ಮುಕ್ತ(pack_tmp_name);
			puts(hash_to_hex(hash));
		}

		/* mark written objects as written to previous pack */
		ಪ್ರತಿ (j = 0; j < nr_written; j++) {
			written_list[j]->offset = (off_t)-1;
		}
		nr_remaining -= nr_written;
	} ಯಾವತ್ (nr_remaining && i < to_pack.nr_objects);

	ಮುಕ್ತ(written_list);
	ಮುಕ್ತ(write_order);
	stop_progress(&progress_state);
	ಯದಿ (written != nr_result)
		die(_("wrote %"PRIu32" objects while expecting %"PRIu32),
		    written, nr_result);
	trace2_data_intmax("pack-objects", the_repository,
			   "write_pack_file/wrote", nr_result);
	trace2_data_intmax("pack-objects", the_repository,
			   "write_pack_file/wrote_bytes", bytes_written);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ no_try_delta(ಧ್ರುವ ವರ್ಣ *path)
{
	ಸ್ಥಿರ ಸಂರಚನಾ attr_check *check;

	ಯದಿ (!check)
		check = attr_check_initl("delta", ರಿಕ್ತ);
	git_check_attr(the_repository->index, path, check);
	ಯದಿ (ATTR_FALSE(check->ವಸ್ತುಗಳು[0].ಮೌಲ್ಯ))
		ಪ್ರತ್ಯಾಗಮ 1;
	ಪ್ರತ್ಯಾಗಮ 0;
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
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ have_duplicate_entry(ಧ್ರುವ ಸಂರಚನಾ object_id *oid,
				ಪೂರ್ಣಾಙ್ಕ exclude)
{
	ಸಂರಚನಾ object_entry *entry;

	ಯದಿ (reuse_packfile_bitmap &&
	    bitmap_walk_contains(bitmap_git, reuse_packfile_bitmap, oid))
		ಪ್ರತ್ಯಾಗಮ 1;

	entry = packlist_find(&to_pack, oid);
	ಯದಿ (!entry)
		ಪ್ರತ್ಯಾಗಮ 0;

	ಯದಿ (exclude) {
		ಯದಿ (!entry->preferred_base)
			nr_result--;
		entry->preferred_base = 1;
	}

	ಪ್ರತ್ಯಾಗಮ 1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ want_cruft_object_mtime(ಸಂರಚನಾ repository *r,
				   ಧ್ರುವ ಸಂರಚನಾ object_id *oid,
				   unsigned flags, uint32_t mtime)
{
	ಸಂರಚನಾ odb_source_files *files = odb_source_files_downcast(r->objects->source);

	ಪ್ರತಿ (ಸಂರಚನಾ odb_files_dir *dir = files->dirs; dir; dir = dir->next) {
		ಸಂರಚನಾ packed_git **cache = packfile_store_get_kept_pack_cache(dir->packed, flags);

		ಪ್ರತಿ (; *cache; cache++) {
			ಸಂರಚನಾ packed_git *p = *cache;
			off_t ofs;
			uint32_t candidate_mtime;

			ofs = find_pack_entry_one(oid, p);
			ಯದಿ (!ofs)
				ಅನುವರ್ತ;

			/*
			 * We have a copy of the object 'oid' in a non-cruft
			 * pack. We can avoid packing an additional copy
			 * regardless of what the existing copy's mtime is since
			 * it is outside of a cruft pack.
			 */
			ಯದಿ (!p->is_cruft)
				ಪ್ರತ್ಯಾಗಮ 0;

			/*
			 * If we have a copy of the object 'oid' in a cruft
			 * pack, then either read the cruft pack's mtime for
			 * that object, or, if that can't be loaded, assume the
			 * pack's mtime itself.
			 */
			ಯದಿ (!load_pack_mtimes(p)) {
				uint32_t pos;
				ಯದಿ (offset_to_pack_pos(p, ofs, &pos) < 0)
					ಅನುವರ್ತ;
				candidate_mtime = nth_packed_mtime(p, pos);
			} ಅನ್ಯಥಾ {
				candidate_mtime = p->mtime;
			}

			/*
			 * We have a surviving copy of the object in a cruft
			 * pack whose mtime is greater than or equal to the one
			 * we are considering. We can thus avoid packing an
			 * additional copy of that object.
			 */
			ಯದಿ (mtime <= candidate_mtime)
				ಪ್ರತ್ಯಾಗಮ 0;
		}
	}

	ಪ್ರತ್ಯಾಗಮ -1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ want_found_object(ಧ್ರುವ ಸಂರಚನಾ object_id *oid, ಪೂರ್ಣಾಙ್ಕ exclude,
			     ಸಂರಚನಾ packed_git *p, uint32_t mtime)
{
	ಯದಿ (exclude)
		ಪ್ರತ್ಯಾಗಮ 1;
	ಯದಿ (incremental)
		ಪ್ರತ್ಯಾಗಮ 0;

	ಯದಿ (!is_pack_valid(p))
		ಪ್ರತ್ಯಾಗಮ -1;

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
	ಯದಿ (local && !p->pack_local)
		ಪ್ರತ್ಯಾಗಮ 0;

	/*
	 * Then handle .keep first, as we have a fast(er) path there.
	 */
	ಯದಿ (ignore_packed_keep_on_disk || ignore_packed_keep_in_core ||
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
		ಯದಿ (ignore_packed_keep_on_disk)
			flags |= KEPT_PACK_ON_DISK;
		ಯದಿ (ignore_packed_keep_in_core)
			flags |= KEPT_PACK_IN_CORE;
		ಯದಿ (ignore_packed_keep_in_core_open)
			flags |= KEPT_PACK_IN_CORE_OPEN;

		/*
		 * If the object is in a pack that we want to ignore, *and* we
		 * don't have any cruft packs that are being retained, we can
		 * abort quickly.
		 */
		ಯದಿ (!ignore_packed_keep_in_core_has_cruft) {
			ಯದಿ (ignore_packed_keep_on_disk && p->pack_keep)
				ಪ್ರತ್ಯಾಗಮ 0;
			ಯದಿ (ignore_packed_keep_in_core && p->pack_keep_in_core)
				ಪ್ರತ್ಯಾಗಮ 0;
			ಯದಿ (ignore_packed_keep_in_core_open && p->pack_keep_in_core_open)
				ಪ್ರತ್ಯಾಗಮ 0;
			ಯದಿ (has_object_kept_pack(p->repo, oid, flags))
				ಪ್ರತ್ಯಾಗಮ 0;
		} ಅನ್ಯಥಾ {
			/*
			 * But if there is at least one cruft pack which
			 * is being kept, we only want to include the
			 * provided object if it has a strictly greater
			 * mtime than any existing cruft copy.
			 */
			ಯದಿ (!want_cruft_object_mtime(p->repo, oid, flags,
						     mtime))
				ಪ್ರತ್ಯಾಗಮ 0;
		}
	}

	/*
	 * At this point we know definitively that either we don't care about
	 * keep-packs, or the object is not in one. Keep checking other
	 * conditions...
	 */
	ಯದಿ (!local || !have_non_local_packs)
		ಪ್ರತ್ಯಾಗಮ 1;

	/* we don't know yet; keep looking for more packs */
	ಪ್ರತ್ಯಾಗಮ -1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ want_object_in_pack_one(ಸಂರಚನಾ packed_git *p,
				   ಧ್ರುವ ಸಂರಚನಾ object_id *oid,
				   ಪೂರ್ಣಾಙ್ಕ exclude,
				   ಸಂರಚನಾ packed_git **found_pack,
				   off_t *found_offset,
				   uint32_t found_mtime)
{
	off_t offset;

	ಯದಿ (p == *found_pack)
		offset = *found_offset;
	ಅನ್ಯಥಾ
		offset = find_pack_entry_one(oid, p);

	ಯದಿ (offset) {
		ಯದಿ (!*found_pack) {
			ಯದಿ (!is_pack_valid(p))
				ಪ್ರತ್ಯಾಗಮ -1;
			*found_offset = offset;
			*found_pack = p;
		}
		ಪ್ರತ್ಯಾಗಮ want_found_object(oid, exclude, p, found_mtime);
	}
	ಪ್ರತ್ಯಾಗಮ -1;
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
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ want_object_in_pack_mtime(ಧ್ರುವ ಸಂರಚನಾ object_id *oid,
				     ಪೂರ್ಣಾಙ್ಕ exclude,
				     ಸಂರಚನಾ packed_git **found_pack,
				     off_t *found_offset,
				     uint32_t found_mtime)
{
	ಸಂರಚನಾ odb_source_files *files =
		odb_source_files_downcast(the_repository->objects->source);
	ಪೂರ್ಣಾಙ್ಕ want;
	ಸಂರಚನಾ packfile_list_entry *e;

	ಯದಿ (!exclude && local) {
		/*
		 * Note that we start iterating at `dirs->next` so that we
		 * skip the local object directory.
		 */
		ಪ್ರತಿ (ಸಂರಚನಾ odb_files_dir *dir = files->dirs->next; dir; dir = dir->next)
			ಯದಿ (!odb_source_read_object_info(&dir->loose->base, oid, ರಿಕ್ತ, 0, ರಿಕ್ತ))
				ಪ್ರತ್ಯಾಗಮ 0;
	}

	/*
	 * If we already know the pack object lives in, start checks from that
	 * pack - in the usual case when neither --local was given nor .keep files
	 * are present we will determine the answer right now.
	 */
	ಯದಿ (*found_pack) {
		want = want_found_object(oid, exclude, *found_pack,
					 found_mtime);
		ಯದಿ (want != -1)
			ಪ್ರತ್ಯಾಗಮ want;

		*found_pack = ರಿಕ್ತ;
		*found_offset = 0;
	}

	ಪ್ರತಿ (ಸಂರಚನಾ odb_files_dir *dir = files->dirs; dir; dir = dir->next) {
		ಸಂರಚನಾ multi_pack_index *m = get_multi_pack_index(dir->packed);
		ಸಂರಚನಾ pack_entry e;

		ಯದಿ (m && midx_fill_entry(m, oid, &e, ರಿಕ್ತ) == MIDX_FILL_HIT) {
			want = want_object_in_pack_one(e.p, oid, exclude, found_pack, found_offset, found_mtime);
			ಯದಿ (want != -1)
				ಪ್ರತ್ಯಾಗಮ want;
		}
	}

	ಪ್ರತಿ (ಸಂರಚನಾ odb_files_dir *dir = files->dirs; dir; dir = dir->next) {
		ಪ್ರತಿ (e = dir->packed->packs.head; e; e = e->next) {
			ಸಂರಚನಾ packed_git *p = e->pack;
			want = want_object_in_pack_one(p, oid, exclude, found_pack, found_offset, found_mtime);
			ಯದಿ (!exclude && want > 0)
				packfile_list_prepend(&dir->packed->packs, p);
			ಯದಿ (want != -1)
				ಪ್ರತ್ಯಾಗಮ want;
		}
	}

	ಯದಿ (uri_protocols.nr) {
		ಸಂರಚನಾ configured_exclusion *ex =
			oidmap_get(&configured_exclusions, oid);
		ಪೂರ್ಣಾಙ್ಕ i;
		ಧ್ರುವ ವರ್ಣ *p;

		ಯದಿ (ex) {
			ಪ್ರತಿ (i = 0; i < uri_protocols.nr; i++) {
				ಯದಿ (skip_prefix(ex->uri,
						uri_protocols.ವಸ್ತುಗಳು[i].string,
						&p) &&
				    *p == ':') {
					oidset_ಸೇರಿಸು(&excluded_by_config, oid);
					ಪ್ರತ್ಯಾಗಮ 0;
				}
			}
		}
	}

	ಪ್ರತ್ಯಾಗಮ 1;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ want_object_in_pack(ಧ್ರುವ ಸಂರಚನಾ object_id *oid,
				      ಪೂರ್ಣಾಙ್ಕ exclude,
				      ಸಂರಚನಾ packed_git **found_pack,
				      off_t *found_offset)
{
	ಪ್ರತ್ಯಾಗಮ want_object_in_pack_mtime(oid, exclude, found_pack, found_offset,
					 0);
}

ಸ್ಥಿರ ಸಂರಚನಾ object_entry *create_object_entry(ಧ್ರುವ ಸಂರಚನಾ object_id *oid,
						ಗಣನಾ object_type type,
						uint32_t hash,
						ಪೂರ್ಣಾಙ್ಕ exclude,
						ಪೂರ್ಣಾಙ್ಕ no_try_delta,
						ಸಂರಚನಾ packed_git *found_pack,
						off_t found_offset)
{
	ಸಂರಚನಾ object_entry *entry;

	entry = packlist_alloc(&to_pack, oid);
	entry->hash = hash;
	oe_set_type(entry, type);
	ಯದಿ (exclude)
		entry->preferred_base = 1;
	ಅನ್ಯಥಾ
		nr_result++;
	ಯದಿ (found_pack) {
		oe_set_in_pack(&to_pack, entry, found_pack);
		entry->in_pack_offset = found_offset;
	}

	entry->no_try_delta = no_try_delta;

	ಪ್ರತ್ಯಾಗಮ entry;
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ no_closure_warning[] = N_(
"disabling bitmap writing, as some objects are not being packed"
);

ಸ್ಥಿರ ಶೂನ್ಯ add_object_entry(ಧ್ರುವ ಸಂರಚನಾ object_id *oid, ಗಣನಾ object_type type,
			     ಧ್ರುವ ವರ್ಣ *name, ಪೂರ್ಣಾಙ್ಕ exclude)
{
	ಸಂರಚನಾ packed_git *found_pack = ರಿಕ್ತ;
	off_t found_offset = 0;

	display_progress(progress_state, ++nr_seen);

	ಯದಿ (have_duplicate_entry(oid, exclude))
		ಪ್ರತ್ಯಾಗಮ;

	ಯದಿ (!want_object_in_pack(oid, exclude, &found_pack, &found_offset)) {
		/* The pack is missing an object, so it will not have closure */
		ಯದಿ (write_bitmap_index) {
			ಯದಿ (write_bitmap_index != WRITE_BITMAP_QUIET)
				warning(_(no_closure_warning));
			write_bitmap_index = 0;
		}
		ಪ್ರತ್ಯಾಗಮ;
	}

	create_object_entry(oid, type, pack_name_hash_fn(name),
			    exclude, name && no_try_delta(name),
			    found_pack, found_offset);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ add_object_entry_from_bitmap(ಧ್ರುವ ಸಂರಚನಾ object_id *oid,
					ಗಣನಾ object_type type,
					ಪೂರ್ಣಾಙ್ಕ flags UNUSED, uint32_t name_hash,
					ಸಂರಚನಾ packed_git *pack, off_t offset,
					ಶೂನ್ಯ *payload UNUSED)
{
	display_progress(progress_state, ++nr_seen);

	ಯದಿ (have_duplicate_entry(oid, 0))
		ಪ್ರತ್ಯಾಗಮ 0;

	ಯದಿ (!want_object_in_pack(oid, 0, &pack, &offset))
		ಪ್ರತ್ಯಾಗಮ 0;

	create_object_entry(oid, type, name_hash, 0, 0, pack, offset);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸಂರಚನಾ pbase_ವೃಕ್ಷ_cache {
	ಸಂರಚನಾ object_id oid;
	ಪೂರ್ಣಾಙ್ಕ ref;
	ಪೂರ್ಣಾಙ್ಕ temporary;
	ಶೂನ್ಯ *ವೃಕ್ಷ_data;
	unsigned long ವೃಕ್ಷ_size;
};

ಸ್ಥಿರ ಸಂರಚನಾ pbase_ವೃಕ್ಷ_cache *(pbase_ವೃಕ್ಷ_cache[256]);
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ pbase_ವೃಕ್ಷ_cache_ix(ಧ್ರುವ ಸಂರಚನಾ object_id *oid)
{
	ಪ್ರತ್ಯಾಗಮ oid->hash[0] % ARRAY_SIZE(pbase_ವೃಕ್ಷ_cache);
}
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ pbase_ವೃಕ್ಷ_cache_ix_incr(ಪೂರ್ಣಾಙ್ಕ ix)
{
	ಪ್ರತ್ಯಾಗಮ (ix+1) % ARRAY_SIZE(pbase_ವೃಕ್ಷ_cache);
}

ಸ್ಥಿರ ಸಂರಚನಾ pbase_ವೃಕ್ಷ {
	ಸಂರಚನಾ pbase_ವೃಕ್ಷ *next;
	/* This is a phony "cache" entry; we are not
	 * going to evict it or find it through _get()
	 * mechanism -- this is for the toplevel node that
	 * would almost always change with any commit.
	 */
	ಸಂರಚನಾ pbase_ವೃಕ್ಷ_cache pcache;
} *pbase_ವೃಕ್ಷ;

ಸ್ಥಿರ ಸಂರಚನಾ pbase_ವೃಕ್ಷ_cache *pbase_ವೃಕ್ಷ_get(ಧ್ರುವ ಸಂರಚನಾ object_id *oid)
{
	ಸಂರಚನಾ pbase_ವೃಕ್ಷ_cache *ent, *nent;
	ಶೂನ್ಯ *data;
	unsigned long size;
	size_t size_st = 0;
	ಗಣನಾ object_type type;
	ಪೂರ್ಣಾಙ್ಕ neigh;
	ಪೂರ್ಣಾಙ್ಕ my_ix = pbase_ವೃಕ್ಷ_cache_ix(oid);
	ಪೂರ್ಣಾಙ್ಕ available_ix = -1;

	/* pbase-tree-cache acts as a limited hashtable.
	 * your object will be found at your index or within a few
	 * slots after that slot if it is cached.
	 */
	ಪ್ರತಿ (neigh = 0; neigh < 8; neigh++) {
		ent = pbase_ವೃಕ್ಷ_cache[my_ix];
		ಯದಿ (ent && oideq(&ent->oid, oid)) {
			ent->ref++;
			ಪ್ರತ್ಯಾಗಮ ent;
		}
		ಅನ್ಯಥಾ ಯದಿ (((available_ix < 0) && (!ent || !ent->ref)) ||
			 ((0 <= available_ix) &&
			  (!ent && pbase_ವೃಕ್ಷ_cache[available_ix])))
			available_ix = my_ix;
		ಯದಿ (!ent)
			ಭಙ್ಗ;
		my_ix = pbase_ವೃಕ್ಷ_cache_ix_incr(my_ix);
	}

	/* Did not find one.  Either we got a bogus request or
	 * we need to read and perhaps cache.
	 */
	data = odb_read_object(the_repository->objects, oid, &type, &size_st);
	size = cast_size_t_to_ulong(size_st);
	ಯದಿ (!data)
		ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
	ಯದಿ (type != OBJ_ವೃಕ್ಷ) {
		ಮುಕ್ತ(data);
		ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
	}

	/* We need to either cache or return a throwaway copy */

	ಯದಿ (available_ix < 0)
		ent = ರಿಕ್ತ;
	ಅನ್ಯಥಾ {
		ent = pbase_ವೃಕ್ಷ_cache[available_ix];
		my_ix = available_ix;
	}

	ಯದಿ (!ent) {
		nent = xmalloc(ಪರಿಮಾಣ(*nent));
		nent->temporary = (available_ix < 0);
	}
	ಅನ್ಯಥಾ {
		/* evict and reuse */
		ಮುಕ್ತ(ent->ವೃಕ್ಷ_data);
		nent = ent;
	}
	oidcpy(&nent->oid, oid);
	nent->ವೃಕ್ಷ_data = data;
	nent->ವೃಕ್ಷ_size = size;
	nent->ref = 1;
	ಯದಿ (!nent->temporary)
		pbase_ವೃಕ್ಷ_cache[my_ix] = nent;
	ಪ್ರತ್ಯಾಗಮ nent;
}

ಸ್ಥಿರ ಶೂನ್ಯ pbase_ವೃಕ್ಷ_put(ಸಂರಚನಾ pbase_ವೃಕ್ಷ_cache *cache)
{
	ಯದಿ (!cache->temporary) {
		cache->ref--;
		ಪ್ರತ್ಯಾಗಮ;
	}
	ಮುಕ್ತ(cache->ವೃಕ್ಷ_data);
	ಮುಕ್ತ(cache);
}

ಸ್ಥಿರ size_t name_cmp_len(ಧ್ರುವ ವರ್ಣ *name)
{
	ಪ್ರತ್ಯಾಗಮ strcspn(name, "\n/");
}

ಸ್ಥಿರ ಶೂನ್ಯ add_pbase_object(ಸಂರಚನಾ ವೃಕ್ಷ_desc *ವೃಕ್ಷ,
			     ಧ್ರುವ ವರ್ಣ *name,
			     size_t cmplen,
			     ಧ್ರುವ ವರ್ಣ *fullname)
{
	ಸಂರಚನಾ name_entry entry;
	ಪೂರ್ಣಾಙ್ಕ cmp;

	ಯಾವತ್ (ವೃಕ್ಷ_entry(ವೃಕ್ಷ,&entry)) {
		ಯದಿ (S_ISGITLINK(entry.mode))
			ಅನುವರ್ತ;
		cmp = ವೃಕ್ಷ_entry_len(&entry) != cmplen ? 1 :
		      memcmp(name, entry.path, cmplen);
		ಯದಿ (cmp > 0)
			ಅನುವರ್ತ;
		ಯದಿ (cmp < 0)
			ಪ್ರತ್ಯಾಗಮ;
		ಯದಿ (name[cmplen] != '/') {
			add_object_entry(&entry.oid,
					 object_type(entry.mode),
					 fullname, 1);
			ಪ್ರತ್ಯಾಗಮ;
		}
		ಯದಿ (S_ISDIR(entry.mode)) {
			ಸಂರಚನಾ ವೃಕ್ಷ_desc sub;
			ಸಂರಚನಾ pbase_ವೃಕ್ಷ_cache *ವೃಕ್ಷ;
			ಧ್ರುವ ವರ್ಣ *down = name+cmplen+1;
			size_t downlen = name_cmp_len(down);

			ವೃಕ್ಷ = pbase_ವೃಕ್ಷ_get(&entry.oid);
			ಯದಿ (!ವೃಕ್ಷ)
				ಪ್ರತ್ಯಾಗಮ;
			init_ವೃಕ್ಷ_desc(&sub, &ವೃಕ್ಷ->oid,
				       ವೃಕ್ಷ->ವೃಕ್ಷ_data, ವೃಕ್ಷ->ವೃಕ್ಷ_size);

			add_pbase_object(&sub, down, downlen, fullname);
			pbase_ವೃಕ್ಷ_put(ವೃಕ್ಷ);
		}
	}
}

ಸ್ಥಿರ unsigned *done_pbase_paths;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ done_pbase_paths_ಅಂಕೆ;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ done_pbase_paths_alloc;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ done_pbase_path_pos(unsigned hash)
{
	ಪೂರ್ಣಾಙ್ಕ lo = 0;
	ಪೂರ್ಣಾಙ್ಕ hi = done_pbase_paths_ಅಂಕೆ;
	ಯಾವತ್ (lo < hi) {
		ಪೂರ್ಣಾಙ್ಕ mi = lo + (hi - lo) / 2;
		ಯದಿ (done_pbase_paths[mi] == hash)
			ಪ್ರತ್ಯಾಗಮ mi;
		ಯದಿ (done_pbase_paths[mi] < hash)
			hi = mi;
		ಅನ್ಯಥಾ
			lo = mi + 1;
	}
	ಪ್ರತ್ಯಾಗಮ -lo-1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ check_pbase_path(unsigned hash)
{
	ಪೂರ್ಣಾಙ್ಕ pos = done_pbase_path_pos(hash);
	ಯದಿ (0 <= pos)
		ಪ್ರತ್ಯಾಗಮ 1;
	pos = -pos - 1;
	ALLOC_GROW(done_pbase_paths,
		   done_pbase_paths_ಅಂಕೆ + 1,
		   done_pbase_paths_alloc);
	done_pbase_paths_ಅಂಕೆ++;
	ಯದಿ (pos < done_pbase_paths_ಅಂಕೆ)
		MOVE_ARRAY(done_pbase_paths + pos + 1, done_pbase_paths + pos,
			   done_pbase_paths_ಅಂಕೆ - pos - 1);
	done_pbase_paths[pos] = hash;
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ add_preferred_base_object(ಧ್ರುವ ವರ್ಣ *name)
{
	ಸಂರಚನಾ pbase_ವೃಕ್ಷ *it;
	size_t cmplen;
	unsigned hash = pack_name_hash_fn(name);

	ಯದಿ (!ಅಂಕೆ_preferred_base || check_pbase_path(hash))
		ಪ್ರತ್ಯಾಗಮ;

	cmplen = name_cmp_len(name);
	ಪ್ರತಿ (it = pbase_ವೃಕ್ಷ; it; it = it->next) {
		ಯದಿ (cmplen == 0) {
			add_object_entry(&it->pcache.oid, OBJ_ವೃಕ್ಷ, ರಿಕ್ತ, 1);
		}
		ಅನ್ಯಥಾ {
			ಸಂರಚನಾ ವೃಕ್ಷ_desc ವೃಕ್ಷ;
			init_ವೃಕ್ಷ_desc(&ವೃಕ್ಷ, &it->pcache.oid,
				       it->pcache.ವೃಕ್ಷ_data, it->pcache.ವೃಕ್ಷ_size);
			add_pbase_object(&ವೃಕ್ಷ, name, cmplen, name);
		}
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ add_preferred_base(ಸಂರಚನಾ object_id *oid)
{
	ಸಂರಚನಾ pbase_ವೃಕ್ಷ *it;
	ಶೂನ್ಯ *data;
	unsigned long size;
	size_t size_st = 0;
	ಸಂರಚನಾ object_id ವೃಕ್ಷ_oid;

	ಯದಿ (window <= ಅಂಕೆ_preferred_base++)
		ಪ್ರತ್ಯಾಗಮ;

	data = odb_read_object_peeled(the_repository->objects, oid,
				      OBJ_ವೃಕ್ಷ, &size_st, &ವೃಕ್ಷ_oid);
	size = cast_size_t_to_ulong(size_st);
	ಯದಿ (!data)
		ಪ್ರತ್ಯಾಗಮ;

	ಪ್ರತಿ (it = pbase_ವೃಕ್ಷ; it; it = it->next) {
		ಯದಿ (oideq(&it->pcache.oid, &ವೃಕ್ಷ_oid)) {
			ಮುಕ್ತ(data);
			ಪ್ರತ್ಯಾಗಮ;
		}
	}

	CALLOC_ARRAY(it, 1);
	it->next = pbase_ವೃಕ್ಷ;
	pbase_ವೃಕ್ಷ = it;

	oidcpy(&it->pcache.oid, &ವೃಕ್ಷ_oid);
	it->pcache.ವೃಕ್ಷ_data = data;
	it->pcache.ವೃಕ್ಷ_size = size;
}

ಸ್ಥಿರ ಶೂನ್ಯ cleanup_preferred_base(ಶೂನ್ಯ)
{
	ಸಂರಚನಾ pbase_ವೃಕ್ಷ *it;
	unsigned i;

	it = pbase_ವೃಕ್ಷ;
	pbase_ವೃಕ್ಷ = ರಿಕ್ತ;
	ಯಾವತ್ (it) {
		ಸಂರಚನಾ pbase_ವೃಕ್ಷ *tmp = it;
		it = tmp->next;
		ಮುಕ್ತ(tmp->pcache.ವೃಕ್ಷ_data);
		ಮುಕ್ತ(tmp);
	}

	ಪ್ರತಿ (i = 0; i < ARRAY_SIZE(pbase_ವೃಕ್ಷ_cache); i++) {
		ಯದಿ (!pbase_ವೃಕ್ಷ_cache[i])
			ಅನುವರ್ತ;
		ಮುಕ್ತ(pbase_ವೃಕ್ಷ_cache[i]->ವೃಕ್ಷ_data);
		FREE_AND_NULL(pbase_ವೃಕ್ಷ_cache[i]);
	}

	FREE_AND_NULL(done_pbase_paths);
	done_pbase_paths_ಅಂಕೆ = done_pbase_paths_alloc = 0;
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
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ can_reuse_delta(ಧ್ರುವ ಸಂರಚನಾ object_id *base_oid,
			   ಸಂರಚನಾ object_entry *delta,
			   ಸಂರಚನಾ object_entry **base_out)
{
	ಸಂರಚನಾ object_entry *base;

	/*
	 * First see if we're already sending the base (or it's explicitly in
	 * our "excluded" list).
	 */
	base = packlist_find(&to_pack, base_oid);
	ಯದಿ (base) {
		ಯದಿ (!in_same_island(&delta->idx.oid, &base->idx.oid))
			ಪ್ರತ್ಯಾಗಮ 0;
		*base_out = base;
		ಪ್ರತ್ಯಾಗಮ 1;
	}

	/*
	 * Otherwise, reachability bitmaps may tell us if the receiver has it,
	 * even if it was buried too deep in history to make it into the
	 * packing list.
	 */
	ಯದಿ (thin && bitmap_has_oid_in_uninteresting(bitmap_git, base_oid)) {
		ಯದಿ (use_delta_islands) {
			ಯದಿ (!in_same_island(&delta->idx.oid, base_oid))
				ಪ್ರತ್ಯಾಗಮ 0;
		}
		*base_out = ರಿಕ್ತ;
		ಪ್ರತ್ಯಾಗಮ 1;
	}

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ prefetch_to_pack(uint32_t object_index_start) {
	ಸಂರಚನಾ oid_array to_fetch = OID_ARRAY_INIT;
	uint32_t i;

	ಪ್ರತಿ (i = object_index_start; i < to_pack.nr_objects; i++) {
		ಸಂರಚನಾ object_entry *entry = to_pack.objects + i;

		ಯದಿ (!odb_read_object_info_extended(the_repository->objects,
						   &entry->idx.oid,
						   ರಿಕ್ತ,
						   OBJECT_INFO_FOR_PREFETCH))
			ಅನುವರ್ತ;
		oid_array_append(&to_fetch, &entry->idx.oid);
	}
	promisor_remote_get_direct(the_repository,
				   to_fetch.oid, to_fetch.nr);
	oid_array_clear(&to_fetch);
}

ಸ್ಥಿರ ಶೂನ್ಯ check_object(ಸಂರಚನಾ object_entry *entry, uint32_t object_index)
{
	size_t canonical_size;
	ಗಣನಾ object_type type;
	ಸಂರಚನಾ object_info oi = {.typep = &type, .sizep = &canonical_size};

	ಯದಿ (IN_PACK(entry)) {
		ಸಂರಚನಾ packed_git *p = IN_PACK(entry);
		ಸಂರಚನಾ pack_window *w_curs = ರಿಕ್ತ;
		ಪೂರ್ಣಾಙ್ಕ have_base = 0;
		ಸಂರಚನಾ object_id base_ref;
		ಸಂರಚನಾ object_entry *base_entry;
		size_t used, used_0, avail;
		off_t ofs;
		unsigned ವರ್ಣ *buf, c;
		ಗಣನಾ object_type type;
		size_t in_pack_size;

		buf = use_pack(p, &w_curs, entry->in_pack_offset, &avail);

		/*
		 * We want in_pack_type even if we do not reuse delta
		 * since non-delta representations could still be reused.
		 */
		used = unpack_object_header_buffer(buf, avail,
						   &type,
						   &in_pack_size);
		ಯದಿ (used == 0)
			ಗಚ್ಛ give_up;

		ಯದಿ (type < 0)
			BUG("invalid type %d", type);
		entry->in_pack_type = type;

		/*
		 * Determine if this is a delta and if so whether we can
		 * reuse it or not.  Otherwise let's find out as cheaply as
		 * possible what the actual type and size for this object is.
		 */
		ವಿಕಲ್ಪ (entry->in_pack_type) {
		ಸ್ವಭಾವ:
			/* Not a delta hence we've already got all we need. */
			oe_set_type(entry, entry->in_pack_type);
			SET_SIZE(entry, in_pack_size);
			entry->in_pack_header_size = used;
			ಯದಿ (oe_type(entry) < OBJ_COMMIT || oe_type(entry) > OBJ_BLOB)
				ಗಚ್ಛ give_up;
			unuse_pack(&w_curs);
			ಪ್ರತ್ಯಾಗಮ;
		ಪಕ್ಷ OBJ_REF_DELTA:
			ಯದಿ (reuse_delta && !entry->preferred_base) {
				oidread(&base_ref,
					use_pack(p, &w_curs,
						 entry->in_pack_offset + used,
						 ರಿಕ್ತ),
					the_repository->hash_algo);
				have_base = 1;
			}
			entry->in_pack_header_size = used + the_hash_algo->rawsz;
			ಭಙ್ಗ;
		ಪಕ್ಷ OBJ_OFS_DELTA:
			buf = use_pack(p, &w_curs,
				       entry->in_pack_offset + used, ರಿಕ್ತ);
			used_0 = 0;
			c = buf[used_0++];
			ofs = c & 127;
			ಯಾವತ್ (c & 128) {
				ofs += 1;
				ಯದಿ (!ofs || MSB(ofs, 7)) {
					error(_("delta base offset overflow in pack for %s"),
					      oid_to_hex(&entry->idx.oid));
					ಗಚ್ಛ give_up;
				}
				c = buf[used_0++];
				ofs = (ofs << 7) + (c & 127);
			}
			ofs = entry->in_pack_offset - ofs;
			ಯದಿ (ofs <= 0 || ofs >= entry->in_pack_offset) {
				error(_("delta base offset out of bound for %s"),
				      oid_to_hex(&entry->idx.oid));
				ಗಚ್ಛ give_up;
			}
			ಯದಿ (reuse_delta && !entry->preferred_base) {
				uint32_t pos;
				ಯದಿ (offset_to_pack_pos(p, ofs, &pos) < 0)
					ಗಚ್ಛ give_up;
				ಯದಿ (!nth_packed_object_id(&base_ref, p,
							  pack_pos_to_index(p, pos)))
					have_base = 1;
			}
			entry->in_pack_header_size = used + used_0;
			ಭಙ್ಗ;
		}

		ಯದಿ (have_base &&
		    can_reuse_delta(&base_ref, entry, &base_entry)) {
			oe_set_type(entry, entry->in_pack_type);
			SET_SIZE(entry, in_pack_size); /* delta size */
			SET_DELTA_SIZE(entry, in_pack_size);

			ಯದಿ (base_entry) {
				SET_DELTA(entry, base_entry);
				entry->delta_sibling_idx = base_entry->delta_child_idx;
				SET_DELTA_CHILD(base_entry, entry);
			} ಅನ್ಯಥಾ {
				SET_DELTA_EXT(entry, &base_ref);
			}

			unuse_pack(&w_curs);
			ಪ್ರತ್ಯಾಗಮ;
		}

		ಯದಿ (oe_type(entry)) {
			off_t delta_pos;

			/*
			 * This must be a delta and we already know what the
			 * final object type is.  Let's extract the actual
			 * object size from the delta header.
			 */
			delta_pos = entry->in_pack_offset + entry->in_pack_header_size;
			canonical_size = get_size_from_delta(p, &w_curs,
							     delta_pos);
			ಯದಿ (canonical_size == 0)
				ಗಚ್ಛ give_up;
			SET_SIZE(entry, canonical_size);
			unuse_pack(&w_curs);
			ಪ್ರತ್ಯಾಗಮ;
		}

		/*
		 * No choice but to fall back to the recursive delta walk
		 * with odb_read_object_info() to find about the object type
		 * at this point...
		 */
		give_up:
		unuse_pack(&w_curs);
	}

	ಯದಿ (odb_read_object_info_extended(the_repository->objects, &entry->idx.oid, &oi,
					  OBJECT_INFO_SKIP_FETCH_OBJECT | OBJECT_INFO_LOOKUP_REPLACE) < 0) {
		ಯದಿ (repo_has_promisor_remote(the_repository)) {
			prefetch_to_pack(object_index);
			ಯದಿ (odb_read_object_info_extended(the_repository->objects, &entry->idx.oid, &oi,
							  OBJECT_INFO_SKIP_FETCH_OBJECT | OBJECT_INFO_LOOKUP_REPLACE) < 0)
				type = -1;
		} ಅನ್ಯಥಾ {
			type = -1;
		}
	}
	oe_set_type(entry, type);
	ಯದಿ (entry->type_valid) {
		SET_SIZE(entry, canonical_size);
	} ಅನ್ಯಥಾ {
		/*
		 * Bad object type is checked in prepare_pack().  This is
		 * to permit a missing preferred base object to be ignored
		 * as a preferred base.  Doing so can result in a larger
		 * pack file, but the transfer will still take place.
		 */
	}
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ pack_offset_sort(ಧ್ರುವ ಶೂನ್ಯ *_a, ಧ್ರುವ ಶೂನ್ಯ *_b)
{
	ಧ್ರುವ ಸಂರಚನಾ object_entry *a = *(ಸಂರಚನಾ object_entry **)_a;
	ಧ್ರುವ ಸಂರಚನಾ object_entry *b = *(ಸಂರಚನಾ object_entry **)_b;
	ಧ್ರುವ ಸಂರಚನಾ packed_git *a_in_pack = IN_PACK(a);
	ಧ್ರುವ ಸಂರಚನಾ packed_git *b_in_pack = IN_PACK(b);

	/* avoid filesystem trashing with loose objects */
	ಯದಿ (!a_in_pack && !b_in_pack)
		ಪ್ರತ್ಯಾಗಮ oidcmp(&a->idx.oid, &b->idx.oid);

	ಯದಿ (a_in_pack < b_in_pack)
		ಪ್ರತ್ಯಾಗಮ -1;
	ಯದಿ (a_in_pack > b_in_pack)
		ಪ್ರತ್ಯಾಗಮ 1;
	ಪ್ರತ್ಯಾಗಮ a->in_pack_offset < b->in_pack_offset ? -1 :
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
ಸ್ಥಿರ ಶೂನ್ಯ drop_reused_delta(ಸಂರಚನಾ object_entry *entry)
{
	unsigned *idx = &to_pack.objects[entry->delta_idx - 1].delta_child_idx;
	ಸಂರಚನಾ object_info oi = OBJECT_INFO_INIT;
	ಗಣನಾ object_type type;
	size_t size;

	ಯಾವತ್ (*idx) {
		ಸಂರಚನಾ object_entry *oe = &to_pack.objects[*idx - 1];

		ಯದಿ (oe == entry)
			*idx = oe->delta_sibling_idx;
		ಅನ್ಯಥಾ
			idx = &oe->delta_sibling_idx;
	}
	SET_DELTA(entry, ರಿಕ್ತ);
	entry->depth = 0;

	oi.sizep = &size;
	oi.typep = &type;
	ಯದಿ (packed_object_info(ರಿಕ್ತ, IN_PACK(entry), entry->in_pack_offset, &oi) < 0) {
		/*
		 * We failed to get the info from this pack for some reason;
		 * fall back to odb_read_object_info, which may find another copy.
		 * And if that fails, the error will be recorded in oe_type(entry)
		 * and dealt with in prepare_pack().
		 */
		oe_set_type(entry,
			    odb_read_object_info(the_repository->objects,
						 &entry->idx.oid, &size));
	} ಅನ್ಯಥಾ {
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
ಸ್ಥಿರ ಶೂನ್ಯ break_delta_chains(ಸಂರಚನಾ object_entry *entry)
{
	/*
	 * The actual depth of each object we will write is stored as an int,
	 * as it cannot exceed our int "depth" limit. But before we break
	 * changes based no that limit, we may potentially go as deep as the
	 * number of objects, which is elsewhere bounded to a uint32_t.
	 */
	uint32_t total_depth;
	ಸಂರಚನಾ object_entry *cur, *next;

	ಪ್ರತಿ (cur = entry, total_depth = 0;
	     cur;
	     cur = DELTA(cur), total_depth++) {
		ಯದಿ (cur->dfs_state == DFS_DONE) {
			/*
			 * We've already seen this object and know it isn't
			 * part of a cycle. We do need to append its depth
			 * to our count.
			 */
			total_depth += cur->depth;
			ಭಙ್ಗ;
		}

		/*
		 * We break cycles before looping, so an ACTIVE state (or any
		 * other cruft which made its way into the state variable)
		 * is a bug.
		 */
		ಯದಿ (cur->dfs_state != DFS_NONE)
			BUG("confusing delta dfs state in first pass: %d",
			    cur->dfs_state);

		/*
		 * Now we know this is the first time we've seen the object. If
		 * it's not a delta, we're done traversing, but we'll mark it
		 * done to save time on future traversals.
		 */
		ಯದಿ (!DELTA(cur)) {
			cur->dfs_state = DFS_DONE;
			ಭಙ್ಗ;
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
		ಯದಿ (DELTA(cur)->dfs_state == DFS_ACTIVE) {
			drop_reused_delta(cur);
			cur->dfs_state = DFS_DONE;
			ಭಙ್ಗ;
		}
	}

	/*
	 * And now that we've gone all the way to the bottom of the chain, we
	 * need to clear the active flags and set the depth fields as
	 * appropriate. Unlike the loop above, which can quit when it drops a
	 * delta, we need to keep going to look for more depth cuts. So we need
	 * an extra "next" pointer to keep going after we reset cur->delta.
	 */
	ಪ್ರತಿ (cur = entry; cur; cur = next) {
		next = DELTA(cur);

		/*
		 * We should have a chain of zero or more ACTIVE states down to
		 * a final DONE. We can quit after the DONE, because either it
		 * has no bases, or we've already handled them in a previous
		 * call.
		 */
		ಯದಿ (cur->dfs_state == DFS_DONE)
			ಭಙ್ಗ;
		ಅನ್ಯಥಾ ಯದಿ (cur->dfs_state != DFS_ACTIVE)
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
		ಯದಿ (!cur->depth)
			drop_reused_delta(cur);

		cur->dfs_state = DFS_DONE;
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ get_object_details(ಶೂನ್ಯ)
{
	uint32_t i;
	ಸಂರಚನಾ object_entry **sorted_by_offset;

	ಯದಿ (progress)
		progress_state = start_progress(the_repository,
						_("Counting objects"),
						to_pack.nr_objects);

	CALLOC_ARRAY(sorted_by_offset, to_pack.nr_objects);
	ಪ್ರತಿ (i = 0; i < to_pack.nr_objects; i++)
		sorted_by_offset[i] = to_pack.objects + i;
	QSORT(sorted_by_offset, to_pack.nr_objects, pack_offset_sort);

	ಪ್ರತಿ (i = 0; i < to_pack.nr_objects; i++) {
		ಸಂರಚನಾ object_entry *entry = sorted_by_offset[i];
		check_object(entry, i);
		ಯದಿ (entry->type_valid &&
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
	ಪ್ರತಿ (i = 0; i < to_pack.nr_objects; i++)
		break_delta_chains(&to_pack.objects[i]);

	ಮುಕ್ತ(sorted_by_offset);
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
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ type_size_sort(ಧ್ರುವ ಶೂನ್ಯ *_a, ಧ್ರುವ ಶೂನ್ಯ *_b)
{
	ಧ್ರುವ ಸಂರಚನಾ object_entry *a = *(ಸಂರಚನಾ object_entry **)_a;
	ಧ್ರುವ ಸಂರಚನಾ object_entry *b = *(ಸಂರಚನಾ object_entry **)_b;
	ಧ್ರುವ ಗಣನಾ object_type a_type = oe_type(a);
	ಧ್ರುವ ಗಣನಾ object_type b_type = oe_type(b);
	ಧ್ರುವ unsigned long a_size = SIZE(a);
	ಧ್ರುವ unsigned long b_size = SIZE(b);

	ಯದಿ (a_type > b_type)
		ಪ್ರತ್ಯಾಗಮ -1;
	ಯದಿ (a_type < b_type)
		ಪ್ರತ್ಯಾಗಮ 1;
	ಯದಿ (a->hash > b->hash)
		ಪ್ರತ್ಯಾಗಮ -1;
	ಯದಿ (a->hash < b->hash)
		ಪ್ರತ್ಯಾಗಮ 1;
	ಯದಿ (a->preferred_base > b->preferred_base)
		ಪ್ರತ್ಯಾಗಮ -1;
	ಯದಿ (a->preferred_base < b->preferred_base)
		ಪ್ರತ್ಯಾಗಮ 1;
	ಯದಿ (use_delta_islands) {
		ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ island_cmp = island_delta_cmp(&a->idx.oid, &b->idx.oid);
		ಯದಿ (island_cmp)
			ಪ್ರತ್ಯಾಗಮ island_cmp;
	}
	ಯದಿ (a_size > b_size)
		ಪ್ರತ್ಯಾಗಮ -1;
	ಯದಿ (a_size < b_size)
		ಪ್ರತ್ಯಾಗಮ 1;
	ಪ್ರತ್ಯಾಗಮ a < b ? -1 : (a > b);  /* newest first */
}

ಸಂರಚನಾ unpacked {
	ಸಂರಚನಾ object_entry *entry;
	ಶೂನ್ಯ *data;
	ಸಂರಚನಾ delta_index *index;
	unsigned depth;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ delta_cacheable(size_t src_size, size_t trg_size,
			   size_t delta_size)
{
	ಯದಿ (max_delta_cache_size && delta_cache_size + delta_size > max_delta_cache_size)
		ಪ್ರತ್ಯಾಗಮ 0;

	ಯದಿ (delta_size < cache_max_small_delta_size)
		ಪ್ರತ್ಯಾಗಮ 1;

	/* cache delta, if objects are large enough compared to delta size */
	ಯದಿ ((src_size >> 20) + (trg_size >> 21) > (delta_size >> 10))
		ಪ್ರತ್ಯಾಗಮ 1;

	ಪ್ರತ್ಯಾಗಮ 0;
}

/* Protect delta_cache_size */
ಸ್ಥಿರ pthread_mutex_t cache_mutex;
#define cache_lock()		pthread_mutex_lock(&cache_mutex)
#define cache_unlock()		pthread_mutex_unlock(&cache_mutex)

/*
 * Protect object list partitioning (e.g. struct thread_param) and
 * progress_state
 */
ಸ್ಥಿರ pthread_mutex_t progress_mutex;
#define progress_lock()		pthread_mutex_lock(&progress_mutex)
#define progress_unlock()	pthread_mutex_unlock(&progress_mutex)

/*
 * Access to struct object_entry is unprotected since each thread owns
 * a portion of the main object list. Just don't access object entries
 * ahead in the list because they can be stolen and would need
 * progress_mutex for protection.
 */

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ oe_size_less_than(ಸಂರಚನಾ packing_data *pack,
				    ಧ್ರುವ ಸಂರಚನಾ object_entry *lhs,
				    size_t rhs)
{
	ಯದಿ (lhs->size_valid)
		ಪ್ರತ್ಯಾಗಮ lhs->size_ < rhs;
	ಯದಿ (rhs < pack->oe_size_limit) /* rhs < 2^x <= lhs ? */
		ಪ್ರತ್ಯಾಗಮ 0;
	ಪ್ರತ್ಯಾಗಮ oe_get_size_slow(pack, lhs) < rhs;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ oe_set_ವೃಕ್ಷ_depth(ಸಂರಚನಾ packing_data *pack,
				     ಸಂರಚನಾ object_entry *e,
				     unsigned ಪೂರ್ಣಾಙ್ಕ ವೃಕ್ಷ_depth)
{
	ಯದಿ (!pack->ವೃಕ್ಷ_depth)
		CALLOC_ARRAY(pack->ವೃಕ್ಷ_depth, pack->nr_alloc);
	pack->ವೃಕ್ಷ_depth[e - pack->objects] = ವೃಕ್ಷ_depth;
}

ಸ್ಥಿರ ಶೂನ್ಯ record_ವೃಕ್ಷ_depth(ಧ್ರುವ ಸಂರಚನಾ object_id *oid, ಧ್ರುವ ವರ್ಣ *name)
{
	ಧ್ರುವ ವರ್ಣ *p;
	unsigned depth;
	ಸಂರಚನಾ object_entry *ent;

	/* the empty string is a root tree, which is depth 0 */
	depth = *name ? 1 : 0;
	ಪ್ರತಿ (p = strchr(name, '/'); p; p = strchr(p + 1, '/'))
		depth++;

	ent = packlist_find(&to_pack, oid);
	ಯದಿ (ent && depth > oe_ವೃಕ್ಷ_depth(&to_pack, ent))
		oe_set_ವೃಕ್ಷ_depth(&to_pack, ent, depth);
}

/*
 * Return the size of the object without doing any delta
 * reconstruction (so non-deltas are true object sizes, but deltas
 * return the size of the delta data).
 */
size_t oe_get_size_slow(ಸಂರಚನಾ packing_data *pack,
			ಧ್ರುವ ಸಂರಚನಾ object_entry *e)
{
	ಸಂರಚನಾ packed_git *p;
	ಸಂರಚನಾ pack_window *w_curs;
	unsigned ವರ್ಣ *buf;
	ಗಣನಾ object_type type;
	size_t used, avail, size;

	ಯದಿ (e->type_ != OBJ_OFS_DELTA && e->type_ != OBJ_REF_DELTA) {
		size_t sz;
		packing_data_lock(&to_pack);
		ಯದಿ (odb_read_object_info(the_repository->objects,
					 &e->idx.oid, &sz) < 0)
			die(_("unable to get size of %s"),
			    oid_to_hex(&e->idx.oid));
		packing_data_unlock(&to_pack);
		ಪ್ರತ್ಯಾಗಮ sz;
	}

	p = oe_in_pack(pack, e);
	ಯದಿ (!p)
		BUG("when e->type is a delta, it must belong to a pack");

	packing_data_lock(&to_pack);
	w_curs = ರಿಕ್ತ;
	buf = use_pack(p, &w_curs, e->in_pack_offset, &avail);
	used = unpack_object_header_buffer(buf, avail, &type, &size);
	ಯದಿ (used == 0)
		die(_("unable to parse object header of %s"),
		    oid_to_hex(&e->idx.oid));

	unuse_pack(&w_curs);
	packing_data_unlock(&to_pack);
	ಪ್ರತ್ಯಾಗಮ size;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ try_delta(ಸಂರಚನಾ unpacked *trg, ಸಂರಚನಾ unpacked *src,
		     unsigned max_depth, size_t *mem_usage)
{
	ಸಂರಚನಾ object_entry *trg_entry = trg->entry;
	ಸಂರಚನಾ object_entry *src_entry = src->entry;
	unsigned long trg_size, src_size, sizediff, max_size, sz;
	size_t delta_size;
	unsigned ref_depth;
	ಗಣನಾ object_type type;
	ಶೂನ್ಯ *delta_buf;

	/* Don't bother doing diffs between different types */
	ಯದಿ (oe_type(trg_entry) != oe_type(src_entry))
		ಪ್ರತ್ಯಾಗಮ -1;

	/*
	 * We do not bother to try a delta that we discarded on an
	 * earlier try, but only when reusing delta data.  Note that
	 * src_entry that is marked as the preferred_base should always
	 * be considered, as even if we produce a suboptimal delta against
	 * it, we will still save the transfer cost, as we already know
	 * the other side has it and we won't send src_entry at all.
	 */
	ಯದಿ (reuse_delta && IN_PACK(trg_entry) &&
	    IN_PACK(trg_entry) == IN_PACK(src_entry) &&
	    !src_entry->preferred_base &&
	    trg_entry->in_pack_type != OBJ_REF_DELTA &&
	    trg_entry->in_pack_type != OBJ_OFS_DELTA)
		ಪ್ರತ್ಯಾಗಮ 0;

	/* Let's not bust the allowed depth. */
	ಯದಿ (src->depth >= max_depth)
		ಪ್ರತ್ಯಾಗಮ 0;

	/* Now some size filtering heuristics. */
	trg_size = SIZE(trg_entry);
	ಯದಿ (!DELTA(trg_entry)) {
		max_size = trg_size/2 - the_hash_algo->rawsz;
		ref_depth = 1;
	} ಅನ್ಯಥಾ {
		max_size = DELTA_SIZE(trg_entry);
		ref_depth = trg->depth;
	}
	max_size = (uint64_t)max_size * (max_depth - src->depth) /
						(max_depth - ref_depth + 1);
	ಯದಿ (max_size == 0)
		ಪ್ರತ್ಯಾಗಮ 0;
	src_size = SIZE(src_entry);
	sizediff = src_size < trg_size ? trg_size - src_size : 0;
	ಯದಿ (sizediff >= max_size)
		ಪ್ರತ್ಯಾಗಮ 0;
	ಯದಿ (trg_size < src_size / 32)
		ಪ್ರತ್ಯಾಗಮ 0;

	ಯದಿ (!in_same_island(&trg->entry->idx.oid, &src->entry->idx.oid))
		ಪ್ರತ್ಯಾಗಮ 0;

	/* Load data if not already done */
	ಯದಿ (!trg->data) {
		size_t sz_st = 0;
		packing_data_lock(&to_pack);
		trg->data = odb_read_object(the_repository->objects,
					    &trg_entry->idx.oid, &type,
					    &sz_st);
		sz = cast_size_t_to_ulong(sz_st);
		packing_data_unlock(&to_pack);
		ಯದಿ (!trg->data)
			die(_("object %s cannot be read"),
			    oid_to_hex(&trg_entry->idx.oid));
		ಯದಿ (sz != trg_size)
			die(_("object %s inconsistent object length (%"PRIuMAX" vs %"PRIuMAX")"),
			    oid_to_hex(&trg_entry->idx.oid), (uintmax_t)sz,
			    (uintmax_t)trg_size);
		*mem_usage += sz;
	}
	ಯದಿ (!src->data) {
		size_t sz_st = 0;
		packing_data_lock(&to_pack);
		src->data = odb_read_object(the_repository->objects,
					    &src_entry->idx.oid, &type,
					    &sz_st);
		sz = cast_size_t_to_ulong(sz_st);
		packing_data_unlock(&to_pack);
		ಯದಿ (!src->data) {
			ಯದಿ (src_entry->preferred_base) {
				ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ warned = 0;
				ಯದಿ (!warned++)
					warning(_("object %s cannot be read"),
						oid_to_hex(&src_entry->idx.oid));
				/*
				 * Those objects are not included in the
				 * resulting pack.  Be resilient and ignore
				 * them if they can't be read, in case the
				 * pack could be created nevertheless.
				 */
				ಪ್ರತ್ಯಾಗಮ 0;
			}
			die(_("object %s cannot be read"),
			    oid_to_hex(&src_entry->idx.oid));
		}
		ಯದಿ (sz != src_size)
			die(_("object %s inconsistent object length (%"PRIuMAX" vs %"PRIuMAX")"),
			    oid_to_hex(&src_entry->idx.oid), (uintmax_t)sz,
			    (uintmax_t)src_size);
		*mem_usage += sz;
	}
	ಯದಿ (!src->index) {
		src->index = create_delta_index(src->data, src_size);
		ಯದಿ (!src->index) {
			ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ warned = 0;
			ಯದಿ (!warned++)
				warning(_("suboptimal pack - out of memory"));
			ಪ್ರತ್ಯಾಗಮ 0;
		}
		*mem_usage += sizeof_delta_index(src->index);
	}

	delta_buf = create_delta(src->index, trg->data, trg_size, &delta_size, max_size);
	ಯದಿ (!delta_buf)
		ಪ್ರತ್ಯಾಗಮ 0;

	ಯದಿ (DELTA(trg_entry)) {
		/* Prefer only shallower same-sized deltas. */
		ಯದಿ (delta_size == DELTA_SIZE(trg_entry) &&
		    src->depth + 1 >= trg->depth) {
			ಮುಕ್ತ(delta_buf);
			ಪ್ರತ್ಯಾಗಮ 0;
		}
	}

	/*
	 * Handle memory allocation outside of the cache
	 * accounting lock.  Compiler will optimize the strangeness
	 * away when NO_PTHREADS is defined.
	 */
	ಮುಕ್ತ(trg_entry->delta_data);
	cache_lock();
	ಯದಿ (trg_entry->delta_data) {
		delta_cache_size -= DELTA_SIZE(trg_entry);
		trg_entry->delta_data = ರಿಕ್ತ;
	}
	ಯದಿ (delta_cacheable(src_size, trg_size, delta_size)) {
		delta_cache_size += delta_size;
		cache_unlock();
		trg_entry->delta_data = xrealloc(delta_buf, delta_size);
	} ಅನ್ಯಥಾ {
		cache_unlock();
		ಮುಕ್ತ(delta_buf);
	}

	SET_DELTA(trg_entry, src_entry);
	SET_DELTA_SIZE(trg_entry, delta_size);
	trg->depth = src->depth + 1;

	ಪ್ರತ್ಯಾಗಮ 1;
}

ಸ್ಥಿರ unsigned ಪೂರ್ಣಾಙ್ಕ check_delta_limit(ಸಂರಚನಾ object_entry *me, unsigned ಪೂರ್ಣಾಙ್ಕ n)
{
	ಸಂರಚನಾ object_entry *child = DELTA_CHILD(me);
	unsigned ಪೂರ್ಣಾಙ್ಕ m = n;
	ಯಾವತ್ (child) {
		ಧ್ರುವ unsigned ಪೂರ್ಣಾಙ್ಕ c = check_delta_limit(child, n + 1);
		ಯದಿ (m < c)
			m = c;
		child = DELTA_SIBLING(child);
	}
	ಪ್ರತ್ಯಾಗಮ m;
}

ಸ್ಥಿರ size_t free_unpacked(ಸಂರಚನಾ unpacked *n)
{
	size_t freed_mem = sizeof_delta_index(n->index);
	free_delta_index(n->index);
	n->index = ರಿಕ್ತ;
	ಯದಿ (n->data) {
		freed_mem += SIZE(n->entry);
		FREE_AND_NULL(n->data);
	}
	n->entry = ರಿಕ್ತ;
	n->depth = 0;
	ಪ್ರತ್ಯಾಗಮ freed_mem;
}

ಸ್ಥಿರ ಶೂನ್ಯ find_deltas(ಸಂರಚನಾ object_entry **list, unsigned *list_size,
			ಪೂರ್ಣಾಙ್ಕ window, ಪೂರ್ಣಾಙ್ಕ depth, unsigned *processed)
{
	uint32_t i, idx = 0, ಎಣಿಕೆ = 0;
	ಸಂರಚನಾ unpacked *array;
	size_t mem_usage = 0;

	CALLOC_ARRAY(array, window);

	ಪ್ರತಿ (;;) {
		ಸಂರಚನಾ object_entry *entry;
		ಸಂರಚನಾ unpacked *n = array + idx;
		ಪೂರ್ಣಾಙ್ಕ j, max_depth, best_base = -1;

		progress_lock();
		ಯದಿ (!*list_size) {
			progress_unlock();
			ಭಙ್ಗ;
		}
		entry = *list++;
		(*list_size)--;
		ಯದಿ (!entry->preferred_base) {
			(*processed)++;
			display_progress(progress_state, *processed);
		}
		progress_unlock();

		mem_usage -= free_unpacked(n);
		n->entry = entry;

		ಯಾವತ್ (window_memory_limit &&
		       mem_usage > window_memory_limit &&
		       ಎಣಿಕೆ > 1) {
			ಧ್ರುವ uint32_t tail = (idx + window - ಎಣಿಕೆ) % window;
			mem_usage -= free_unpacked(array + tail);
			ಎಣಿಕೆ--;
		}

		/* We do not compute delta to *create* objects we are not
		 * going to pack.
		 */
		ಯದಿ (entry->preferred_base)
			ಗಚ್ಛ next;

		/*
		 * If the current object is at pack edge, take the depth the
		 * objects that depend on the current object into account
		 * otherwise they would become too deep.
		 */
		max_depth = depth;
		ಯದಿ (DELTA_CHILD(entry)) {
			max_depth -= check_delta_limit(entry, 0);
			ಯದಿ (max_depth <= 0)
				ಗಚ್ಛ next;
		}

		j = window;
		ಯಾವತ್ (--j > 0) {
			ಪೂರ್ಣಾಙ್ಕ ret;
			uint32_t other_idx = idx + j;
			ಸಂರಚನಾ unpacked *m;
			ಯದಿ (other_idx >= window)
				other_idx -= window;
			m = array + other_idx;
			ಯದಿ (!m->entry)
				ಭಙ್ಗ;
			ret = try_delta(n, m, max_depth, &mem_usage);
			ಯದಿ (ret < 0)
				ಭಙ್ಗ;
			ಅನ್ಯಥಾ ಯದಿ (ret > 0)
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
		ಯದಿ (entry->delta_data && !pack_to_stdout) {
			unsigned long size;

			size = do_compress(&entry->delta_data, DELTA_SIZE(entry));
			ಯದಿ (size < (1U << OE_Z_DELTA_BITS)) {
				entry->z_delta_size = size;
				cache_lock();
				delta_cache_size -= DELTA_SIZE(entry);
				delta_cache_size += entry->z_delta_size;
				cache_unlock();
			} ಅನ್ಯಥಾ {
				FREE_AND_NULL(entry->delta_data);
				entry->z_delta_size = 0;
			}
		}

		/* if we made n a delta, and if n is already at max
		 * depth, leaving it in the window is pointless.  we
		 * should evict it first.
		 */
		ಯದಿ (DELTA(entry) && max_depth <= n->depth)
			ಅನುವರ್ತ;

		/*
		 * Move the best delta base up in the window, after the
		 * currently deltified object, to keep it longer.  It will
		 * be the first base object to be attempted next.
		 */
		ಯದಿ (DELTA(entry)) {
			ಸಂರಚನಾ unpacked swap = array[best_base];
			ಪೂರ್ಣಾಙ್ಕ dist = (window + idx - best_base) % window;
			ಪೂರ್ಣಾಙ್ಕ dst = best_base;
			ಯಾವತ್ (dist--) {
				ಪೂರ್ಣಾಙ್ಕ src = (dst + 1) % window;
				array[dst] = array[src];
				dst = src;
			}
			array[dst] = swap;
		}

		next:
		idx++;
		ಯದಿ (ಎಣಿಕೆ + 1 < window)
			ಎಣಿಕೆ++;
		ಯದಿ (idx >= window)
			idx = 0;
	}

	ಪ್ರತಿ (i = 0; i < window; ++i) {
		free_delta_index(array[i].index);
		ಮುಕ್ತ(array[i].data);
	}
	ಮುಕ್ತ(array);
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

ಸಂರಚನಾ thread_params {
	pthread_t thread;
	ಸಂರಚನಾ object_entry **list;
	ಸಂರಚನಾ packing_region *regions;
	unsigned list_size;
	unsigned remaining;
	ಪೂರ್ಣಾಙ್ಕ window;
	ಪೂರ್ಣಾಙ್ಕ depth;
	ಪೂರ್ಣಾಙ್ಕ working;
	ಪೂರ್ಣಾಙ್ಕ data_ready;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	unsigned *processed;
};

ಸ್ಥಿರ pthread_cond_t progress_cond;

/*
 * Mutex and conditional variable can't be statically-initialized on Windows.
 */
ಸ್ಥಿರ ಶೂನ್ಯ init_threaded_ಹುಡುಕು(ಶೂನ್ಯ)
{
	pthread_mutex_init(&cache_mutex, ರಿಕ್ತ);
	pthread_mutex_init(&progress_mutex, ರಿಕ್ತ);
	pthread_cond_init(&progress_cond, ರಿಕ್ತ);
}

ಸ್ಥಿರ ಶೂನ್ಯ cleanup_threaded_ಹುಡುಕು(ಶೂನ್ಯ)
{
	pthread_cond_destroy(&progress_cond);
	pthread_mutex_destroy(&cache_mutex);
	pthread_mutex_destroy(&progress_mutex);
}

ಸ್ಥಿರ ಶೂನ್ಯ *threaded_find_deltas(ಶೂನ್ಯ *arg)
{
	ಸಂರಚನಾ thread_params *me = arg;

	progress_lock();
	ಯಾವತ್ (me->remaining) {
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
		ಯಾವತ್ (!me->data_ready)
			pthread_cond_wait(&me->cond, &me->mutex);
		me->data_ready = 0;
		pthread_mutex_unlock(&me->mutex);

		progress_lock();
	}
	progress_unlock();
	/* leave ->working 1 so that this doesn't get more work assigned */
	ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

ಸ್ಥಿರ ಶೂನ್ಯ ll_find_deltas(ಸಂರಚನಾ object_entry **list, unsigned list_size,
			   ಪೂರ್ಣಾಙ್ಕ window, ಪೂರ್ಣಾಙ್ಕ depth, unsigned *processed)
{
	ಸಂರಚನಾ thread_params *p;
	ಪೂರ್ಣಾಙ್ಕ i, ret, active_threads = 0;

	init_threaded_ಹುಡುಕು();

	ಯದಿ (delta_ಹುಡುಕು_threads <= 1) {
		find_deltas(list, &list_size, window, depth, processed);
		cleanup_threaded_ಹುಡುಕು();
		ಪ್ರತ್ಯಾಗಮ;
	}
	ಯದಿ (progress > pack_to_stdout)
		fprintf_ln(stderr, _("Delta compression using up to %d threads"),
			   delta_ಹುಡುಕು_threads);
	CALLOC_ARRAY(p, delta_ಹುಡುಕು_threads);

	/* Partition the work amongst work threads. */
	ಪ್ರತಿ (i = 0; i < delta_ಹುಡುಕು_threads; i++) {
		unsigned sub_size = list_size / (delta_ಹುಡುಕು_threads - i);

		/* don't use too small segments or no deltas will be found */
		ಯದಿ (sub_size < 2*window && i+1 < delta_ಹುಡುಕು_threads)
			sub_size = 0;

		p[i].window = window;
		p[i].depth = depth;
		p[i].processed = processed;
		p[i].working = 1;
		p[i].data_ready = 0;

		/* try to split chunks on "path" boundaries */
		ಯಾವತ್ (sub_size && sub_size < list_size &&
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
	ಪ್ರತಿ (i = 0; i < delta_ಹುಡುಕು_threads; i++) {
		ಯದಿ (!p[i].list_size)
			ಅನುವರ್ತ;
		pthread_mutex_init(&p[i].mutex, ರಿಕ್ತ);
		pthread_cond_init(&p[i].cond, ರಿಕ್ತ);
		ret = pthread_create(&p[i].thread, ರಿಕ್ತ,
				     threaded_find_deltas, &p[i]);
		ಯದಿ (ret)
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
	ಯಾವತ್ (active_threads) {
		ಸಂರಚನಾ thread_params *target = ರಿಕ್ತ;
		ಸಂರಚನಾ thread_params *victim = ರಿಕ್ತ;
		unsigned sub_size = 0;

		progress_lock();
		ಪ್ರತಿ (;;) {
			ಪ್ರತಿ (i = 0; !target && i < delta_ಹುಡುಕು_threads; i++)
				ಯದಿ (!p[i].working)
					target = &p[i];
			ಯದಿ (target)
				ಭಙ್ಗ;
			pthread_cond_wait(&progress_cond, &progress_mutex);
		}

		ಪ್ರತಿ (i = 0; i < delta_ಹುಡುಕು_threads; i++)
			ಯದಿ (p[i].remaining > 2*window &&
			    (!victim || victim->remaining < p[i].remaining))
				victim = &p[i];
		ಯದಿ (victim) {
			sub_size = victim->remaining / 2;
			list = victim->list + victim->list_size - sub_size;
			ಯಾವತ್ (sub_size && list[0]->hash &&
			       list[0]->hash == list[-1]->hash) {
				list++;
				sub_size--;
			}
			ಯದಿ (!sub_size) {
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

		ಯದಿ (!sub_size) {
			pthread_join(target->thread, ರಿಕ್ತ);
			pthread_cond_destroy(&target->cond);
			pthread_mutex_destroy(&target->mutex);
			active_threads--;
		}
	}
	cleanup_threaded_ಹುಡುಕು();
	ಮುಕ್ತ(p);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ obj_is_packed(ಧ್ರುವ ಸಂರಚನಾ object_id *oid)
{
	ಪ್ರತ್ಯಾಗಮ packlist_find(&to_pack, oid) ||
		(reuse_packfile_bitmap &&
		 bitmap_walk_contains(bitmap_git, reuse_packfile_bitmap, oid));
}

ಸ್ಥಿರ ಶೂನ್ಯ add_tag_chain(ಧ್ರುವ ಸಂರಚನಾ object_id *oid)
{
	ಸಂರಚನಾ tag *tag;

	/*
	 * We catch duplicates already in add_object_entry(), but we'd
	 * prefer to do this extra check to avoid having to parse the
	 * tag at all if we already know that it's being packed (e.g., if
	 * it was included via bitmaps, we would not have parsed it
	 * previously).
	 */
	ಯದಿ (obj_is_packed(oid))
		ಪ್ರತ್ಯಾಗಮ;

	tag = lookup_tag(the_repository, oid);
	ಯಾವತ್ (1) {
		ಯದಿ (!tag || parse_tag(the_repository, tag) || !tag->tagged)
			die(_("unable to pack objects reachable from tag %s"),
			    oid_to_hex(oid));

		add_object_entry(&tag->object.oid, OBJ_TAG, ರಿಕ್ತ, 0);

		ಯದಿ (tag->tagged->type != OBJ_TAG)
			ಪ್ರತ್ಯಾಗಮ;

		tag = (ಸಂರಚನಾ tag *)tag->tagged;
	}
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ add_ref_tag(ಧ್ರುವ ಸಂರಚನಾ reference *ref, ಶೂನ್ಯ *cb_data UNUSED)
{
	ಸಂರಚನಾ object_id peeled;

	ಯದಿ (!reference_get_peeled_oid(the_repository, ref, &peeled) &&
	    obj_is_packed(&peeled))
		add_tag_chain(ref->oid);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ should_attempt_deltas(ಸಂರಚನಾ object_entry *entry)
{
	ಯದಿ (DELTA(entry))
		/* This happens if we decided to reuse existing
		 * delta from a pack. "reuse_delta &&" is implied.
		 */
		ಪ್ರತ್ಯಾಗಮ 0;

	ಯದಿ (!entry->type_valid ||
	    oe_size_less_than(&to_pack, entry, 50))
		ಪ್ರತ್ಯಾಗಮ 0;

	ಯದಿ (entry->no_try_delta)
		ಪ್ರತ್ಯಾಗಮ 0;

	ಯದಿ (!entry->preferred_base) {
		ಯದಿ (oe_type(entry) < 0)
			die(_("unable to get type of object %s"),
				oid_to_hex(&entry->idx.oid));
	} ಅನ್ಯಥಾ ಯದಿ (oe_type(entry) < 0) {
		/*
		 * This object is not found, but we
		 * don't have to include it anyway.
		 */
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಪ್ರತ್ಯಾಗಮ 1;
}

ಸ್ಥಿರ ಶೂನ್ಯ find_deltas_for_region(ಸಂರಚನಾ object_entry *list,
				   ಸಂರಚನಾ packing_region *region,
				   unsigned ಪೂರ್ಣಾಙ್ಕ *processed)
{
	ಸಂರಚನಾ object_entry **delta_list;
	unsigned ಪೂರ್ಣಾಙ್ಕ delta_list_nr = 0;

	ALLOC_ARRAY(delta_list, region->nr);
	ಪ್ರತಿ (size_t i = 0; i < region->nr; i++) {
		ಸಂರಚನಾ object_entry *entry = list + region->start + i;
		ಯದಿ (should_attempt_deltas(entry))
			delta_list[delta_list_nr++] = entry;
	}

	QSORT(delta_list, delta_list_nr, type_size_sort);
	find_deltas(delta_list, &delta_list_nr, window, depth, processed);
	ಮುಕ್ತ(delta_list);
}

ಸ್ಥಿರ ಶೂನ್ಯ find_deltas_by_region(ಸಂರಚನಾ object_entry *list,
				  ಸಂರಚನಾ packing_region *regions,
				  size_t start, size_t nr)
{
	unsigned ಪೂರ್ಣಾಙ್ಕ processed = 0;
	size_t progress_nr;

	ಯದಿ (!nr)
		ಪ್ರತ್ಯಾಗಮ;

	progress_nr = regions[nr - 1].start + regions[nr - 1].nr;

	ಯದಿ (progress)
		progress_state = start_progress(the_repository,
						_("Compressing objects by path"),
						progress_nr);

	ಯಾವತ್ (nr--)
		find_deltas_for_region(list,
				       &regions[start++],
				       &processed);

	display_progress(progress_state, progress_nr);
	stop_progress(&progress_state);
}

ಸ್ಥಿರ ಶೂನ್ಯ *threaded_find_deltas_by_path(ಶೂನ್ಯ *arg)
{
	ಸಂರಚನಾ thread_params *me = arg;

	progress_lock();
	ಯಾವತ್ (me->remaining) {
		ಯಾವತ್ (me->remaining) {
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
		ಯಾವತ್ (!me->data_ready)
			pthread_cond_wait(&me->cond, &me->mutex);
		me->data_ready = 0;
		pthread_mutex_unlock(&me->mutex);

		progress_lock();
	}
	progress_unlock();
	/* leave ->working 1 so that this doesn't get more work assigned */
	ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

ಸ್ಥಿರ ಶೂನ್ಯ ll_find_deltas_by_region(ಸಂರಚನಾ object_entry *list,
				     ಸಂರಚನಾ packing_region *regions,
				     uint32_t start, uint32_t nr)
{
	ಸಂರಚನಾ thread_params *p;
	ಪೂರ್ಣಾಙ್ಕ i, ret, active_threads = 0;
	unsigned ಪೂರ್ಣಾಙ್ಕ processed = 0;
	uint32_t progress_nr;
	init_threaded_ಹುಡುಕು();

	ಯದಿ (!nr)
		ಪ್ರತ್ಯಾಗಮ;

	progress_nr =  regions[nr - 1].start + regions[nr - 1].nr;
	ಯದಿ (delta_ಹುಡುಕು_threads <= 1) {
		find_deltas_by_region(list, regions, start, nr);
		cleanup_threaded_ಹುಡುಕು();
		ಪ್ರತ್ಯಾಗಮ;
	}

	ಯದಿ (progress > pack_to_stdout)
		fprintf_ln(stderr,
			   Q_("Path-based delta compression using up to %d thread",
			      "Path-based delta compression using up to %d threads",
			      delta_ಹುಡುಕು_threads),
			   delta_ಹುಡುಕು_threads);
	CALLOC_ARRAY(p, delta_ಹುಡುಕು_threads);

	ಯದಿ (progress)
		progress_state = start_progress(the_repository,
						_("Compressing objects by path"),
						progress_nr);
	/* Partition the work amongst work threads. */
	ಪ್ರತಿ (i = 0; i < delta_ಹುಡುಕು_threads; i++) {
		unsigned sub_size = nr / (delta_ಹುಡುಕು_threads - i);

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
	ಪ್ರತಿ (i = 0; i < delta_ಹುಡುಕು_threads; i++) {
		ಯದಿ (!p[i].list_size)
			ಅನುವರ್ತ;
		pthread_mutex_init(&p[i].mutex, ರಿಕ್ತ);
		pthread_cond_init(&p[i].cond, ರಿಕ್ತ);
		ret = pthread_create(&p[i].thread, ರಿಕ್ತ,
				     threaded_find_deltas_by_path, &p[i]);
		ಯದಿ (ret)
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
	ಯಾವತ್ (active_threads) {
		ಸಂರಚನಾ thread_params *target = ರಿಕ್ತ;
		ಸಂರಚನಾ thread_params *victim = ರಿಕ್ತ;
		unsigned sub_size = 0;

		progress_lock();
		ಪ್ರತಿ (;;) {
			ಪ್ರತಿ (i = 0; !target && i < delta_ಹುಡುಕು_threads; i++)
				ಯದಿ (!p[i].working)
					target = &p[i];
			ಯದಿ (target)
				ಭಙ್ಗ;
			pthread_cond_wait(&progress_cond, &progress_mutex);
		}

		ಪ್ರತಿ (i = 0; i < delta_ಹುಡುಕು_threads; i++)
			ಯದಿ (p[i].remaining > 2*window &&
			    (!victim || victim->remaining < p[i].remaining))
				victim = &p[i];
		ಯದಿ (victim) {
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

		ಯದಿ (!sub_size) {
			pthread_join(target->thread, ರಿಕ್ತ);
			pthread_cond_destroy(&target->cond);
			pthread_mutex_destroy(&target->mutex);
			active_threads--;
		}
	}
	cleanup_threaded_ಹುಡುಕು();
	ಮುಕ್ತ(p);

	display_progress(progress_state, progress_nr);
	stop_progress(&progress_state);
}

ಸ್ಥಿರ ಶೂನ್ಯ prepare_pack(ಪೂರ್ಣಾಙ್ಕ window, ಪೂರ್ಣಾಙ್ಕ depth)
{
	ಸಂರಚನಾ object_entry **delta_list;
	uint32_t i, nr_deltas;
	unsigned n;

	ಯದಿ (use_delta_islands)
		resolve_ವೃಕ್ಷ_islands(the_repository, progress, &to_pack);

	get_object_details();

	/*
	 * If we're locally repacking then we need to be doubly careful
	 * from now on in order to make sure no stealth corruption gets
	 * propagated to the new pack.  Clients receiving streamed packs
	 * should validate everything they get anyway so no need to incur
	 * the additional cost here in that case.
	 */
	ಯದಿ (!pack_to_stdout)
		do_check_packed_object_crc = 1;

	ಯದಿ (!to_pack.nr_objects || !window || !depth)
		ಪ್ರತ್ಯಾಗಮ;

	ಯದಿ (path_walk)
		ll_find_deltas_by_region(to_pack.objects, to_pack.regions,
					 0, to_pack.nr_regions);

	ALLOC_ARRAY(delta_list, to_pack.nr_objects);
	nr_deltas = n = 0;

	ಪ್ರತಿ (i = 0; i < to_pack.nr_objects; i++) {
		ಸಂರಚನಾ object_entry *entry = to_pack.objects + i;

		ಯದಿ (!should_attempt_deltas(entry))
			ಅನುವರ್ತ;

		ಯದಿ (!entry->preferred_base)
			nr_deltas++;

		delta_list[n++] = entry;
	}

	ಯದಿ (nr_deltas && n > 1) {
		unsigned nr_done = 0;

		ಯದಿ (progress)
			progress_state = start_progress(the_repository,
							_("Compressing objects"),
							nr_deltas);
		QSORT(delta_list, n, type_size_sort);
		ll_find_deltas(delta_list, n, window+1, depth, &nr_done);
		stop_progress(&progress_state);
		ಯದಿ (nr_done != nr_deltas)
			die(_("inconsistency with delta count"));
	}
	ಮುಕ್ತ(delta_list);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ git_pack_config(ಧ್ರುವ ವರ್ಣ *k, ಧ್ರುವ ವರ್ಣ *v,
			   ಧ್ರುವ ಸಂರಚನಾ config_context *ctx, ಶೂನ್ಯ *cb)
{
	ಯದಿ (!strcmp(k, "pack.window")) {
		window = git_config_int(k, v, ctx->kvi);
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (!strcmp(k, "pack.windowmemory")) {
		window_memory_limit = git_config_ulong(k, v, ctx->kvi);
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (!strcmp(k, "pack.depth")) {
		depth = git_config_int(k, v, ctx->kvi);
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (!strcmp(k, "pack.deltacachesize")) {
		max_delta_cache_size = git_config_size_t(k, v, ctx->kvi);
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (!strcmp(k, "pack.deltacachelimit")) {
		cache_max_small_delta_size = git_config_int(k, v, ctx->kvi);
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (!strcmp(k, "pack.writebitmaphashcache")) {
		ಯದಿ (git_config_bool(k, v))
			write_bitmap_options |= BITMAP_OPT_HASH_CACHE;
		ಅನ್ಯಥಾ
			write_bitmap_options &= ~BITMAP_OPT_HASH_CACHE;
	}

	ಯದಿ (!strcmp(k, "pack.writebitmaplookuptable")) {
		ಯದಿ (git_config_bool(k, v))
			write_bitmap_options |= BITMAP_OPT_LOOKUP_TABLE;
		ಅನ್ಯಥಾ
			write_bitmap_options &= ~BITMAP_OPT_LOOKUP_TABLE;
	}

	ಯದಿ (!strcmp(k, "pack.usebitmaps")) {
		use_bitmap_index_default = git_config_bool(k, v);
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (!strcmp(k, "pack.allowpackreuse")) {
		ಪೂರ್ಣಾಙ್ಕ res = git_parse_maybe_bool_text(v);
		ಯದಿ (res < 0) {
			ಯದಿ (!strcasecmp(v, "single"))
				allow_pack_reuse = SINGLE_PACK_REUSE;
			ಅನ್ಯಥಾ ಯದಿ (!strcasecmp(v, "multi"))
				allow_pack_reuse = MULTI_PACK_REUSE;
			ಅನ್ಯಥಾ
				die(_("invalid pack.allowPackReuse value: '%s'"), v);
		} ಅನ್ಯಥಾ ಯದಿ (res) {
			allow_pack_reuse = SINGLE_PACK_REUSE;
		} ಅನ್ಯಥಾ {
			allow_pack_reuse = NO_PACK_REUSE;
		}
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (!strcmp(k, "pack.threads")) {
		delta_ಹುಡುಕು_threads = git_config_int(k, v, ctx->kvi);
		ಯದಿ (delta_ಹುಡುಕು_threads < 0)
			die(_("invalid number of threads specified (%d)"),
			    delta_ಹುಡುಕು_threads);
		ಯದಿ (!HAVE_THREADS && delta_ಹುಡುಕು_threads != 1) {
			warning(_("no threads support, ignoring %s"), k);
			delta_ಹುಡುಕು_threads = 0;
		}
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (!strcmp(k, "pack.indexversion")) {
		pack_idx_opts.version = git_config_int(k, v, ctx->kvi);
		ಯದಿ (pack_idx_opts.version > 2)
			die(_("bad pack.indexVersion=%"PRIu32),
			    pack_idx_opts.version);
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (!strcmp(k, "pack.writereverseindex")) {
		ಯದಿ (git_config_bool(k, v))
			pack_idx_opts.flags |= WRITE_REV;
		ಅನ್ಯಥಾ
			pack_idx_opts.flags &= ~WRITE_REV;
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (!strcmp(k, "uploadpack.blobpackfileuri")) {
		ಸಂರಚನಾ configured_exclusion *ex;
		ಧ್ರುವ ವರ್ಣ *oid_end, *pack_end;
		/*
		 * Stores the pack hash. This is not a true object ID, but is
		 * of the same form.
		 */
		ಸಂರಚನಾ object_id pack_hash;

		ಯದಿ (!v)
			ಪ್ರತ್ಯಾಗಮ config_error_nonbool(k);

		ex = xmalloc(ಪರಿಮಾಣ(*ex));
		ಯದಿ (parse_oid_hex(v, &ex->e.oid, &oid_end) ||
		    *oid_end != ' ' ||
		    parse_oid_hex(oid_end + 1, &pack_hash, &pack_end) ||
		    *pack_end != ' ')
			die(_("value of uploadpack.blobpackfileuri must be "
			      "of the form '<object-hash> <pack-hash> <uri>' (got '%s')"), v);
		ಯದಿ (oidmap_get(&configured_exclusions, &ex->e.oid))
			die(_("object already configured in another "
			      "uploadpack.blobpackfileuri (got '%s')"), v);
		ex->pack_hash_hex = xcalloc(1, pack_end - oid_end);
		memcpy(ex->pack_hash_hex, oid_end + 1, pack_end - oid_end - 1);
		ex->uri = xstrdup(pack_end + 1);
		oidmap_put(&configured_exclusions, ex);
	}
	ಪ್ರತ್ಯಾಗಮ git_default_config(k, v, ctx, cb);
}

/* Counters for trace2 output when in --stdin-packs mode. */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ stdin_packs_found_nr;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ stdin_packs_hints_nr;

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ add_object_entry_from_pack(ಧ್ರುವ ಸಂರಚನಾ object_id *oid,
				      ಸಂರಚನಾ packed_git *p,
				      uint32_t pos,
				      ಶೂನ್ಯ *_data)
{
	off_t ofs;
	ಸಂರಚನಾ object_info oi = OBJECT_INFO_INIT;
	ಗಣನಾ object_type type = OBJ_NONE;

	display_progress(progress_state, ++nr_seen);

	ಯದಿ (have_duplicate_entry(oid, 0))
		ಪ್ರತ್ಯಾಗಮ 0;

	stdin_packs_found_nr++;

	ofs = nth_packed_object_offset(p, pos);

	oi.typep = &type;
	ಯದಿ (packed_object_info(ರಿಕ್ತ, p, ofs, &oi) < 0) {
		die(_("could not get type of object %s in pack %s"),
		    oid_to_hex(oid), p->pack_name);
	} ಅನ್ಯಥಾ ಯದಿ (type == OBJ_COMMIT) {
		ಸಂರಚನಾ rev_info *revs = _data;
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
		add_pending_oid(revs, ರಿಕ್ತ, oid, 0);
	}

	ಯದಿ (!want_object_in_pack(oid, 0, &p, &ofs))
		ಪ್ರತ್ಯಾಗಮ 0;

	create_object_entry(oid, type, 0, 0, 0, p, ofs);

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ show_object_pack_hint(ಸಂರಚನಾ object *object, ಧ್ರುವ ವರ್ಣ *name,
				  ಶೂನ್ಯ *data)
{
	ಗಣನಾ stdin_packs_mode mode = *(ಗಣನಾ stdin_packs_mode *)data;
	ಯದಿ (mode == STDIN_PACKS_MODE_FOLLOW) {
		ಯದಿ (object->type == OBJ_BLOB &&
		    !odb_has_object(the_repository->objects, &object->oid, 0))
			ಪ್ರತ್ಯಾಗಮ;
		add_object_entry(&object->oid, object->type, name, 0);
	} ಅನ್ಯಥಾ {
		ಸಂರಚನಾ object_entry *oe = packlist_find(&to_pack, &object->oid);
		ಯದಿ (!oe)
			ಪ್ರತ್ಯಾಗಮ;

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

ಸ್ಥಿರ ಶೂನ್ಯ show_commit_pack_hint(ಸಂರಚನಾ commit *commit, ಶೂನ್ಯ *data)
{
	ಗಣನಾ stdin_packs_mode mode = *(ಗಣನಾ stdin_packs_mode *)data;

	ಯದಿ (mode == STDIN_PACKS_MODE_FOLLOW) {
		show_object_pack_hint((ಸಂರಚನಾ object *)commit, "", data);
		ಪ್ರತ್ಯಾಗಮ;
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
ಗಣನಾ stdin_pack_info_kind {
	STDIN_PACK_INCLUDE = (1<<0),
	STDIN_PACK_EXCLUDE_CLOSED = (1<<1),
	STDIN_PACK_EXCLUDE_OPEN = (1<<2),
};

ಸಂರಚನಾ stdin_pack_info {
	ಸಂರಚನಾ packed_git *p;
	ಗಣನಾ stdin_pack_info_kind kind;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ pack_mtime_cmp(ಧ್ರುವ ಶೂನ್ಯ *_a, ಧ್ರುವ ಶೂನ್ಯ *_b)
{
	ಸಂರಚನಾ stdin_pack_info *a = ((ಧ್ರುವ ಸಂರಚನಾ string_list_item*)_a)->util;
	ಸಂರಚನಾ stdin_pack_info *b = ((ಧ್ರುವ ಸಂರಚನಾ string_list_item*)_b)->util;

	/*
	 * order packs by descending mtime so that objects are laid out
	 * roughly as newest-to-oldest
	 */
	ಯದಿ (a->p->mtime < b->p->mtime)
		ಪ್ರತ್ಯಾಗಮ 1;
	ಅನ್ಯಥಾ ಯದಿ (b->p->mtime < a->p->mtime)
		ಪ್ರತ್ಯಾಗಮ -1;
	ಅನ್ಯಥಾ
		ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ stdin_packs_include_check_obj(ಸಂರಚನಾ object *obj, ಶೂನ್ಯ *data UNUSED)
{
	ಪ್ರತ್ಯಾಗಮ !has_object_kept_pack(to_pack.repo, &obj->oid,
				     KEPT_PACK_IN_CORE);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ stdin_packs_include_check(ಸಂರಚನಾ commit *commit, ಶೂನ್ಯ *data)
{
	ಪ್ರತ್ಯಾಗಮ stdin_packs_include_check_obj((ಸಂರಚನಾ object *)commit, data);
}

ಸ್ಥಿರ ಶೂನ್ಯ stdin_packs_add_pack_entries(ಸಂರಚನಾ strmap *packs,
					 ಸಂರಚನಾ rev_info *revs)
{
	ಸಂರಚನಾ string_list ಕೀಲಿಗಳು = STRING_LIST_INIT_NODUP;
	ಸಂರಚನಾ string_list_item *item;
	ಸಂರಚನಾ hashmap_iter iter;
	ಸಂರಚನಾ strmap_entry *entry;

	strmap_for_each_entry(packs, &iter, entry) {
		ಸಂರಚನಾ stdin_pack_info *info = entry->ಮೌಲ್ಯ;
		ಯದಿ (!info->p)
			die(_("could not find pack '%s'"), entry->ಕೀಲಿ);

		string_list_append(&ಕೀಲಿಗಳು, entry->ಕೀಲಿ)->util = info;
	}

	/*
	 * Order packs by ascending mtime; use QSORT directly to access the
	 * string_list_item's ->util pointer, which string_list_sort() does not
	 * provide.
	 */
	QSORT(ಕೀಲಿಗಳು.ವಸ್ತುಗಳು, ಕೀಲಿಗಳು.nr, pack_mtime_cmp);

	for_each_string_list_item(item, &ಕೀಲಿಗಳು) {
		ಸಂರಚನಾ stdin_pack_info *info = item->util;

		ಯದಿ (info->kind & STDIN_PACK_EXCLUDE_OPEN) {
			/*
			 * When open-excluded packs ("!") are present, stop
			 * the parent walk at closed-excluded ("^") packs.
			 * Objects behind a "^" boundary are guaranteed to
			 * have closure and should not be rescued.
			 */
			revs->include_check = stdin_packs_include_check;
			revs->include_check_obj = stdin_packs_include_check_obj;
		}

		ಯದಿ ((info->kind & STDIN_PACK_INCLUDE) ||
		    (info->kind & STDIN_PACK_EXCLUDE_OPEN))
			for_each_object_in_pack(info->p,
						add_object_entry_from_pack,
						revs,
						ODB_FOR_EACH_OBJECT_PACK_ORDER);
	}

	string_list_clear(&ಕೀಲಿಗಳು, 0);
}

ಸ್ಥಿರ ಶೂನ್ಯ stdin_packs_read_ಒಳಹರಿವು(ಸಂರಚನಾ rev_info *revs,
				   ಗಣನಾ stdin_packs_mode mode)
{
	ಸಂರಚನಾ strbuf buf = STRBUF_INIT;
	ಸಂರಚನಾ strmap packs = STRMAP_INIT;
	ಸಂರಚನಾ packed_git *p;

	ಯಾವತ್ (strbuf_getline(&buf, stdin) != EOF) {
		ಸಂರಚನಾ stdin_pack_info *info;
		ಗಣನಾ stdin_pack_info_kind kind = STDIN_PACK_INCLUDE;
		ಧ್ರುವ ವರ್ಣ *ಕೀಲಿ = buf.buf;

		ಯದಿ (!*ಕೀಲಿ)
			ಅನುವರ್ತ;
		ಅನ್ಯಥಾ ಯದಿ (*ಕೀಲಿ == '^')
			kind = STDIN_PACK_EXCLUDE_CLOSED;
		ಅನ್ಯಥಾ ಯದಿ (*ಕೀಲಿ == '!' && mode == STDIN_PACKS_MODE_FOLLOW)
			kind = STDIN_PACK_EXCLUDE_OPEN;

		ಯದಿ (kind != STDIN_PACK_INCLUDE)
			ಕೀಲಿ++;

		info = strmap_get(&packs, ಕೀಲಿ);
		ಯದಿ (!info) {
			CALLOC_ARRAY(info, 1);
			strmap_put(&packs, ಕೀಲಿ, info);
		}

		info->kind |= kind;

		strbuf_reset(&buf);
	}

	repo_for_each_pack(the_repository, p) {
		ಸಂರಚನಾ stdin_pack_info *info;

		info = strmap_get(&packs, pack_basename(p));
		ಯದಿ (!info)
			ಅನುವರ್ತ;

		ಯದಿ (info->kind & STDIN_PACK_INCLUDE) {
			ಯದಿ (exclude_promisor_objects && p->pack_promisor)
				die(_("packfile %s is a promisor but --exclude-promisor-objects was given"), p->pack_name);

			/*
			 * Arguments we got on stdin may not even be
			 * packs. First check that to avoid segfaulting
			 * later on in e.g.  pack_mtime_cmp(), excluded
			 * packs are handled below.
			 */
			ಯದಿ (!is_pack_valid(p))
				die(_("packfile %s cannot be accessed"), p->pack_name);
		}

		ಯದಿ (info->kind & STDIN_PACK_EXCLUDE_CLOSED) {
			/*
			 * Marking excluded packs as kept in-core so
			 * that later calls to add_object_entry()
			 * discards any objects that are also found in
			 * excluded packs.
			 */
			p->pack_keep_in_core = 1;
		}

		ಯದಿ (info->kind & STDIN_PACK_EXCLUDE_OPEN) {
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

ಸ್ಥಿರ ಶೂನ್ಯ add_unreachable_loose_objects(ಸಂರಚನಾ rev_info *revs);

ಸ್ಥಿರ ಶೂನ್ಯ read_stdin_packs(ಸಂರಚನಾ repository *repo,
			     ಗಣನಾ stdin_packs_mode mode, ಪೂರ್ಣಾಙ್ಕ rev_list_unpacked)
{
	ಪೂರ್ಣಾಙ್ಕ prev_fetch_if_missing = repo->fetch_if_missing;
	ಸಂರಚನಾ rev_info revs;

	/*
	 * The revision walk may hit objects that are promised, only. As the
	 * walk is best-effort though we don't want to perform backfill fetches
	 * for them.
	 */
	repo->fetch_if_missing = 0;

	repo_init_revisions(repo, &revs, ರಿಕ್ತ);
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
	revs.ವೃಕ್ಷ_objects = 1;
	revs.tag_objects = 1;
	revs.ignore_missing_links = 1;
	revs.exclude_promisor_objects = exclude_promisor_objects;

	/* avoids adding objects in excluded packs */
	ignore_packed_keep_in_core = 1;
	ಯದಿ (mode == STDIN_PACKS_MODE_FOLLOW) {
		/*
		 * In '--stdin-packs=follow' mode, additionally ignore
		 * objects in excluded-open packs to prevent them from
		 * appearing in the resulting pack.
		 */
		ignore_packed_keep_in_core_open = 1;
	}
	stdin_packs_read_ಒಳಹರಿವು(&revs, mode);
	ಯದಿ (rev_list_unpacked)
		add_unreachable_loose_objects(&revs);

	ಯದಿ (prepare_revision_walk(&revs))
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

ಸ್ಥಿರ ಶೂನ್ಯ add_cruft_object_entry(ಧ್ರುವ ಸಂರಚನಾ object_id *oid, ಗಣನಾ object_type type,
				   ಸಂರಚನಾ packed_git *pack, off_t offset,
				   ಧ್ರುವ ವರ್ಣ *name, uint32_t mtime)
{
	ಸಂರಚನಾ object_entry *entry;

	display_progress(progress_state, ++nr_seen);

	entry = packlist_find(&to_pack, oid);
	ಯದಿ (entry) {
		ಯದಿ (name) {
			entry->hash = pack_name_hash_fn(name);
			entry->no_try_delta = no_try_delta(name);
		}
	} ಅನ್ಯಥಾ {
		ಯದಿ (!want_object_in_pack_mtime(oid, 0, &pack, &offset, mtime))
			ಪ್ರತ್ಯಾಗಮ;
		ಯದಿ (!pack && type == OBJ_BLOB) {
			ಸಂರಚನಾ odb_source_files *files =
				odb_source_files_downcast(the_repository->objects->source);
			ಪೂರ್ಣಾಙ್ಕ found = 0;

			ಪ್ರತಿ (ಸಂರಚನಾ odb_files_dir *dir = files->dirs; !found && dir; dir = dir->next)
				ಯದಿ (!odb_source_read_object_info(&dir->loose->base, oid, ರಿಕ್ತ, 0, ರಿಕ್ತ))
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
			ಯದಿ (!found)
				ಪ್ರತ್ಯಾಗಮ;
		}

		entry = create_object_entry(oid, type, pack_name_hash_fn(name),
					    0, name && no_try_delta(name),
					    pack, offset);
	}

	ಯದಿ (mtime > oe_cruft_mtime(&to_pack, entry))
		oe_set_cruft_mtime(&to_pack, entry, mtime);
	ಪ್ರತ್ಯಾಗಮ;
}

ಸ್ಥಿರ ಶೂನ್ಯ show_cruft_object(ಸಂರಚನಾ object *obj, ಧ್ರುವ ವರ್ಣ *name, ಶೂನ್ಯ *data UNUSED)
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
	add_cruft_object_entry(&obj->oid, obj->type, ರಿಕ್ತ, 0, name, cruft_expiration);
}

ಸ್ಥಿರ ಶೂನ್ಯ show_cruft_commit(ಸಂರಚನಾ commit *commit, ಶೂನ್ಯ *data)
{
	show_cruft_object((ಸಂರಚನಾ object*)commit, ರಿಕ್ತ, data);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cruft_include_check_obj(ಸಂರಚನಾ object *obj, ಶೂನ್ಯ *data UNUSED)
{
	ಪ್ರತ್ಯಾಗಮ !has_object_kept_pack(to_pack.repo, &obj->oid, KEPT_PACK_IN_CORE);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cruft_include_check(ಸಂರಚನಾ commit *commit, ಶೂನ್ಯ *data)
{
	ಪ್ರತ್ಯಾಗಮ cruft_include_check_obj((ಸಂರಚನಾ object*)commit, data);
}

ಸ್ಥಿರ ಶೂನ್ಯ set_cruft_mtime(ಧ್ರುವ ಸಂರಚನಾ object *object,
			    ಸಂರಚನಾ packed_git *pack,
			    off_t offset, time_t mtime)
{
	add_cruft_object_entry(&object->oid, object->type, pack, offset, ರಿಕ್ತ,
			       mtime);
}

ಸ್ಥಿರ ಶೂನ್ಯ mark_pack_kept_in_core(ಸಂರಚನಾ string_list *packs, unsigned keep)
{
	ಸಂರಚನಾ string_list_item *item = ರಿಕ್ತ;
	for_each_string_list_item(item, packs) {
		ಸಂರಚನಾ packed_git *p = item->util;
		ಯದಿ (!p)
			die(_("could not find pack '%s'"), item->string);
		ಯದಿ (p->is_cruft && keep)
			ignore_packed_keep_in_core_has_cruft = 1;
		p->pack_keep_in_core = keep;
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ add_objects_in_unpacked_packs(ಶೂನ್ಯ);

ಸ್ಥಿರ ಶೂನ್ಯ enumerate_cruft_objects(ಶೂನ್ಯ)
{
	ಯದಿ (progress)
		progress_state = start_progress(the_repository,
						_("Enumerating cruft objects"), 0);

	add_objects_in_unpacked_packs();
	add_unreachable_loose_objects(ರಿಕ್ತ);

	stop_progress(&progress_state);
}

ಸ್ಥಿರ ಶೂನ್ಯ enumerate_and_traverse_cruft_objects(ಸಂರಚನಾ string_list *fresh_packs)
{
	ಸಂರಚನಾ packed_git *p;
	ಸಂರಚನಾ rev_info revs;
	ಪೂರ್ಣಾಙ್ಕ ret;

	repo_init_revisions(the_repository, &revs, ರಿಕ್ತ);

	revs.tag_objects = 1;
	revs.ವೃಕ್ಷ_objects = 1;
	revs.blob_objects = 1;

	revs.include_check = cruft_include_check;
	revs.include_check_obj = cruft_include_check_obj;

	revs.ignore_missing_links = 1;

	ಯದಿ (progress)
		progress_state = start_progress(the_repository,
						_("Enumerating cruft objects"), 0);
	ret = add_unseen_recent_objects_to_traversal(&revs, cruft_expiration,
						     set_cruft_mtime, 1);
	stop_progress(&progress_state);

	ಯದಿ (ret)
		die(_("unable to add cruft objects"));

	/*
	 * Re-mark only the fresh packs as kept so that objects in
	 * unknown packs do not halt the reachability traversal early.
	 */
	repo_for_each_pack(the_repository, p)
		p->pack_keep_in_core = 0;
	mark_pack_kept_in_core(fresh_packs, 1);

	ಯದಿ (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	ಯದಿ (progress)
		progress_state = start_progress(the_repository,
						_("Traversing cruft objects"), 0);
	nr_seen = 0;
	traverse_commit_list(&revs, show_cruft_commit, show_cruft_object, ರಿಕ್ತ);

	stop_progress(&progress_state);
	release_revisions(&revs);
}

ಸ್ಥಿರ ಶೂನ್ಯ read_cruft_objects(ಶೂನ್ಯ)
{
	ಸಂರಚನಾ strbuf buf = STRBUF_INIT;
	ಸಂರಚನಾ string_list discard_packs = STRING_LIST_INIT_DUP;
	ಸಂರಚನಾ string_list fresh_packs = STRING_LIST_INIT_DUP;
	ಸಂರಚನಾ packed_git *p;

	ignore_packed_keep_in_core = 1;

	ಯಾವತ್ (strbuf_getline(&buf, stdin) != EOF) {
		ಯದಿ (!buf.len)
			ಅನುವರ್ತ;

		ಯದಿ (*buf.buf == '-')
			string_list_append(&discard_packs, buf.buf + 1);
		ಅನ್ಯಥಾ
			string_list_append(&fresh_packs, buf.buf);
	}

	string_list_sort(&discard_packs);
	string_list_sort(&fresh_packs);

	repo_for_each_pack(the_repository, p) {
		ಧ್ರುವ ವರ್ಣ *pack_name = pack_basename(p);
		ಸಂರಚನಾ string_list_item *item;

		item = string_list_lookup(&fresh_packs, pack_name);
		ಯದಿ (!item)
			item = string_list_lookup(&discard_packs, pack_name);

		ಯದಿ (item) {
			item->util = p;
		} ಅನ್ಯಥಾ {
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

	ಯದಿ (cruft_expiration)
		enumerate_and_traverse_cruft_objects(&fresh_packs);
	ಅನ್ಯಥಾ
		enumerate_cruft_objects();

	strbuf_release(&buf);
	string_list_clear(&discard_packs, 0);
	string_list_clear(&fresh_packs, 0);
}

ಸ್ಥಿರ ಶೂನ್ಯ read_object_list_from_stdin(ಶೂನ್ಯ)
{
	ವರ್ಣ ಸಾಲು[GIT_MAX_HEXSZ + 1 + PATH_MAX + 2];
	ಸಂರಚನಾ object_id oid;
	ಧ್ರುವ ವರ್ಣ *p;

	ಪ್ರತಿ (;;) {
		ಯದಿ (!fgets(ಸಾಲು, ಪರಿಮಾಣ(ಸಾಲು), stdin)) {
			ಯದಿ (feof(stdin))
				ಭಙ್ಗ;
			ಯದಿ (!ferror(stdin))
				BUG("fgets returned NULL, not EOF, not error!");
			ಯದಿ (errno != EINTR)
				die_errno("fgets");
			clearerr(stdin);
			ಅನುವರ್ತ;
		}
		ಯದಿ (ಸಾಲು[0] == '-') {
			ಯದಿ (get_oid_hex(ಸಾಲು+1, &oid))
				die(_("expected edge object ID, got garbage:\n %s"),
				    ಸಾಲು);
			add_preferred_base(&oid);
			ಅನುವರ್ತ;
		}
		ಯದಿ (parse_oid_hex(ಸಾಲು, &oid, &p))
			die(_("expected object ID, got garbage:\n %s"), ಸಾಲು);

		add_preferred_base_object(p + 1);
		add_object_entry(&oid, OBJ_NONE, p + 1, 0);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ show_commit(ಸಂರಚನಾ commit *commit, ಶೂನ್ಯ *data UNUSED)
{
	add_object_entry(&commit->object.oid, OBJ_COMMIT, ರಿಕ್ತ, 0);

	ಯದಿ (write_bitmap_index)
		index_commit_for_bitmap(commit);

	ಯದಿ (use_delta_islands)
		propagate_island_marks(the_repository, commit);
}

ಸ್ಥಿರ ಶೂನ್ಯ show_object(ಸಂರಚನಾ object *obj, ಧ್ರುವ ವರ್ಣ *name,
			ಶೂನ್ಯ *data UNUSED)
{
	add_preferred_base_object(name);
	add_object_entry(&obj->oid, obj->type, name, 0);

	ಯದಿ (use_delta_islands)
		record_ವೃಕ್ಷ_depth(&obj->oid, name);
}

ಸ್ಥಿರ ಶೂನ್ಯ show_object__ma_allow_any(ಸಂರಚನಾ object *obj, ಧ್ರುವ ವರ್ಣ *name, ಶೂನ್ಯ *data)
{
	ಪ್ರತಿಜ್ಞಾ(arg_missing_action == MA_ALLOW_ANY);

	/*
	 * Quietly ignore ALL missing objects.  This avoids problems with
	 * staging them now and getting an odd error later.
	 */
	ಯದಿ (!odb_has_object(the_repository->objects, &obj->oid, 0))
		ಪ್ರತ್ಯಾಗಮ;

	show_object(obj, name, data);
}

ಸ್ಥಿರ ಶೂನ್ಯ show_object__ma_allow_promisor(ಸಂರಚನಾ object *obj, ಧ್ರುವ ವರ್ಣ *name, ಶೂನ್ಯ *data)
{
	ಪ್ರತಿಜ್ಞಾ(arg_missing_action == MA_ALLOW_PROMISOR);

	/*
	 * Quietly ignore EXPECTED missing objects.  This avoids problems with
	 * staging them now and getting an odd error later.
	 */
	ಯದಿ (!odb_has_object(the_repository->objects, &obj->oid, 0) &&
	    is_promisor_object(to_pack.repo, &obj->oid))
		ಪ್ರತ್ಯಾಗಮ;

	show_object(obj, name, data);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ option_parse_missing_action(ಧ್ರುವ ಸಂರಚನಾ option *opt,
				       ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಸಂರಚನಾ repository *repo = opt->ಮೌಲ್ಯ;

	ಪ್ರತಿಜ್ಞಾ(arg);
	ಪ್ರತಿಜ್ಞಾ(!unset);

	ಯದಿ (!strcmp(arg, "error")) {
		arg_missing_action = MA_ERROR;
		fn_show_object = show_object;
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (!strcmp(arg, "allow-any")) {
		arg_missing_action = MA_ALLOW_ANY;
		repo->fetch_if_missing = 0;
		fn_show_object = show_object__ma_allow_any;
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (!strcmp(arg, "allow-promisor")) {
		arg_missing_action = MA_ALLOW_PROMISOR;
		repo->fetch_if_missing = 0;
		fn_show_object = show_object__ma_allow_promisor;
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	die(_("invalid value for '%s': '%s'"), "--missing", arg);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ show_edge(ಸಂರಚನಾ commit *commit)
{
	add_preferred_base(&commit->object.oid);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ add_object_in_unpacked_pack(ಧ್ರುವ ಸಂರಚನಾ object_id *oid,
				       ಸಂರಚನಾ object_info *oi,
				       ಶೂನ್ಯ *data UNUSED)
{
	ಯದಿ (cruft) {
		add_cruft_object_entry(oid, OBJ_NONE, oi->source_infop->u.packed.pack,
				       oi->source_infop->u.packed.offset, ರಿಕ್ತ,
				       *oi->mtimep);
	} ಅನ್ಯಥಾ {
		add_object_entry(oid, OBJ_NONE, "", 0);
	}
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ add_objects_in_unpacked_packs(ಶೂನ್ಯ)
{
	ಸಂರಚನಾ odb_source_files *files =
		odb_source_files_downcast(to_pack.repo->objects->source);
	time_t mtime;
	ಸಂರಚನಾ odb_for_each_object_options opts = {
		.flags = ODB_FOR_EACH_OBJECT_PACK_ORDER |
			 ODB_FOR_EACH_OBJECT_LOCAL_ONLY |
			 ODB_FOR_EACH_OBJECT_SKIP_IN_CORE_KEPT_PACKS |
			 ODB_FOR_EACH_OBJECT_SKIP_ON_DISK_KEPT_PACKS,
	};
	ಸಂರಚನಾ odb_source_info source_info;
	ಸಂರಚನಾ object_info oi = {
		.mtimep = &mtime,
		.source_infop = &source_info,
	};

	ಪ್ರತಿ (ಸಂರಚನಾ odb_files_dir *dir = files->dirs; dir; dir = dir->next) {
		ಯದಿ (!dir->local)
			ಅನುವರ್ತ;

		ಯದಿ (odb_source_for_each_object(&dir->packed->base, &oi,
					       add_object_in_unpacked_pack, ರಿಕ್ತ, &opts))
			die(_("cannot open pack index"));
	}
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ add_loose_object(ಧ್ರುವ ಸಂರಚನಾ object_id *oid, ಧ್ರುವ ವರ್ಣ *path,
			    ಶೂನ್ಯ *data)
{
	ಸಂರಚನಾ rev_info *revs = data;
	ಗಣನಾ object_type type = odb_read_object_info(the_repository->objects, oid, ರಿಕ್ತ);

	ಯದಿ (type < 0) {
		warning(_("loose object at %s could not be examined"), path);
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (cruft) {
		ಸಂರಚನಾ stat st;
		ಯದಿ (stat(path, &st) < 0) {
			ಯದಿ (errno == ENOENT)
				ಪ್ರತ್ಯಾಗಮ 0;
			ಪ್ರತ್ಯಾಗಮ error_errno("unable to stat %s", oid_to_hex(oid));
		}

		add_cruft_object_entry(oid, type, ರಿಕ್ತ, 0, ರಿಕ್ತ,
				       st.st_mtime);
	} ಅನ್ಯಥಾ {
		add_object_entry(oid, type, "", 0);
	}

	ಯದಿ (revs && type == OBJ_COMMIT)
		add_pending_oid(revs, ರಿಕ್ತ, oid, 0);

	ಪ್ರತ್ಯಾಗಮ 0;
}

/*
 * We actually don't even have to worry about reachability here.
 * add_object_entry will weed out duplicates, so we just add every
 * loose object we find.
 */
ಸ್ಥಿರ ಶೂನ್ಯ add_unreachable_loose_objects(ಸಂರಚನಾ rev_info *revs)
{
	for_each_loose_file_in_source(the_repository->objects->source,
				      add_loose_object, ರಿಕ್ತ, ರಿಕ್ತ, revs);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ has_sha1_pack_kept_or_nonlocal(ಧ್ರುವ ಸಂರಚನಾ object_id *oid)
{
	ಸ್ಥಿರ ಸಂರಚನಾ packed_git *last_found = ರಿಕ್ತ;
	ಸಂರಚನಾ packed_git *p;

	ಯದಿ (last_found && find_pack_entry_one(oid, last_found))
		ಪ್ರತ್ಯಾಗಮ 1;

	repo_for_each_pack(the_repository, p) {
		/*
		 * We have already checked `last_found`, so there is no need to
		 * re-check here.
		 */
		ಯದಿ (p == last_found)
			ಅನುವರ್ತ;

		ಯದಿ ((!p->pack_local || p->pack_keep || p->pack_keep_in_core) &&
		    find_pack_entry_one(oid, p)) {
			last_found = p;
			ಪ್ರತ್ಯಾಗಮ 1;
		}
	}

	ಪ್ರತ್ಯಾಗಮ 0;
}

/*
 * Store a list of sha1s that are should not be discarded
 * because they are either written too recently, or are
 * reachable from another object that was.
 *
 * This is filled by get_object_list.
 */
ಸ್ಥಿರ ಸಂರಚನಾ oid_array recent_objects;

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ loosened_object_can_be_discarded(ಧ್ರುವ ಸಂರಚನಾ object_id *oid,
					    timestamp_t mtime)
{
	ಯದಿ (!unpack_unreachable_expiration)
		ಪ್ರತ್ಯಾಗಮ 0;
	ಯದಿ (mtime > unpack_unreachable_expiration)
		ಪ್ರತ್ಯಾಗಮ 0;
	ಯದಿ (oid_array_lookup(&recent_objects, oid) >= 0)
		ಪ್ರತ್ಯಾಗಮ 0;
	ಪ್ರತ್ಯಾಗಮ 1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ force_object_loose(ಸಂರಚನಾ odb_source *source,
			      ಧ್ರುವ ಸಂರಚನಾ object_id *oid,
			      ಧ್ರುವ time_t *mtime)
{
	ಸಂರಚನಾ odb_source_files *files = odb_source_files_downcast(source);
	ಧ್ರುವ ಸಂರಚನಾ git_hash_algo *compat = source->odb->repo->compat_hash_algo;
	ಸಂರಚನಾ object_info oi = OBJECT_INFO_INIT;
	ಸಂರಚನಾ object_id compat_oid, *compat_oid_p = ರಿಕ್ತ;
	ಗಣನಾ object_type type;
	ಶೂನ್ಯ *buf = ರಿಕ್ತ;
	size_t len;
	ಪೂರ್ಣಾಙ್ಕ ret;

	ಪ್ರತಿ (ಸಂರಚನಾ odb_files_dir *dir = files->dirs; dir; dir = dir->next)
		ಯದಿ (!odb_source_read_object_info(&dir->loose->base, oid, ರಿಕ್ತ, 0, ರಿಕ್ತ))
			ಪ್ರತ್ಯಾಗಮ 0;

	oi.typep = &type;
	oi.sizep = &len;
	oi.contentp = &buf;
	ಯದಿ (odb_read_object_info_extended(source->odb, oid, &oi, 0)) {
		ret = error(_("cannot read object for %s"), oid_to_hex(oid));
		ಗಚ್ಛ out;
	}

	ಯದಿ (compat) {
		ಯದಿ (repo_oid_to_algop(source->odb->repo, oid, compat, &compat_oid)) {
			ret = error(_("cannot map object %s to %s"),
				    oid_to_hex(oid), compat->name);
			ಗಚ್ಛ out;
		}

		compat_oid_p = &compat_oid;
	}

	ret = odb_source_write_object(&files->dirs->loose->base, buf, len, type, oid,
				      compat_oid_p, mtime, 0);

out:
	ಮುಕ್ತ(buf);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಶೂನ್ಯ loosen_unused_packed_objects(ಶೂನ್ಯ)
{
	ಸಂರಚನಾ packed_git *p;
	uint32_t i;
	uint32_t loosened_objects_nr = 0;
	ಸಂರಚನಾ object_id oid;

	repo_for_each_pack(the_repository, p) {
		ಯದಿ (!p->pack_local || p->pack_keep || p->pack_keep_in_core)
			ಅನುವರ್ತ;

		ಯದಿ (open_pack_index(p))
			die(_("cannot open pack index"));

		ಪ್ರತಿ (i = 0; i < p->ಅಂಕೆ_objects; i++) {
			nth_packed_object_id(&oid, p, i);
			ಯದಿ (!packlist_find(&to_pack, &oid) &&
			    !has_sha1_pack_kept_or_nonlocal(&oid) &&
			    !loosened_object_can_be_discarded(&oid, p->mtime)) {
				ಯದಿ (force_object_loose(the_repository->objects->source,
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
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ pack_options_allow_reuse(ಶೂನ್ಯ)
{
	ಪ್ರತ್ಯಾಗಮ allow_pack_reuse != NO_PACK_REUSE &&
	       pack_to_stdout &&
	       !ignore_packed_keep_on_disk &&
	       !ignore_packed_keep_in_core &&
	       (!local || !have_non_local_packs) &&
	       !incremental;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ get_object_list_from_bitmap(ಸಂರಚನಾ rev_info *revs)
{
	ಯದಿ (!(bitmap_git = prepare_bitmap_walk(revs, 0)))
		ಪ್ರತ್ಯಾಗಮ -1;

	/*
	 * For now, force the name-hash version to be 1 since that
	 * is the version implied by the bitmap format. Later, the
	 * format can include this version explicitly in its format,
	 * allowing readers to know the version that was used during
	 * the bitmap write.
	 */
	name_hash_version = 1;

	ಯದಿ (pack_options_allow_reuse())
		reuse_partial_packfile_from_bitmap(bitmap_git,
						   &reuse_packfiles,
						   &reuse_packfiles_nr,
						   &reuse_packfile_bitmap,
						   allow_pack_reuse == MULTI_PACK_REUSE);

	ಯದಿ (reuse_packfiles) {
		reuse_packfile_objects = bitmap_popcount(reuse_packfile_bitmap);
		ಯದಿ (!reuse_packfile_objects)
			BUG("expected non-empty reuse bitmap");

		nr_result += reuse_packfile_objects;
		nr_seen += reuse_packfile_objects;
		display_progress(progress_state, nr_seen);
	}

	traverse_bitmap_commit_list(bitmap_git, revs,
				    &add_object_entry_from_bitmap);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ record_recent_object(ಸಂರಚನಾ object *obj,
				 ಧ್ರುವ ವರ್ಣ *name UNUSED,
				 ಶೂನ್ಯ *data UNUSED)
{
	oid_array_append(&recent_objects, &obj->oid);
}

ಸ್ಥಿರ ಶೂನ್ಯ record_recent_commit(ಸಂರಚನಾ commit *commit, ಶೂನ್ಯ *data UNUSED)
{
	oid_array_append(&recent_objects, &commit->object.oid);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ mark_bitmap_preferred_tip(ಧ್ರುವ ಸಂರಚನಾ reference *ref, ಶೂನ್ಯ *data UNUSED)
{
	ಧ್ರುವ ಸಂರಚನಾ object_id *maybe_peeled = ref->oid;
	ಸಂರಚನಾ object_id peeled;
	ಸಂರಚನಾ object *object;

	ಯದಿ (!reference_get_peeled_oid(the_repository, ref, &peeled))
		maybe_peeled = &peeled;

	object = parse_object_or_die(the_repository, maybe_peeled, ref->name);
	ಯದಿ (object->type == OBJ_COMMIT)
		object->flags |= NEEDS_BITMAP;

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ is_oid_uninteresting(ಸಂರಚನಾ repository *repo,
				       ಸಂರಚನಾ object_id *oid)
{
	ಸಂರಚನಾ object *o = lookup_object(repo, oid);
	ಪ್ರತ್ಯಾಗಮ !o || (o->flags & UNINTERESTING);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ add_objects_by_path(ಧ್ರುವ ವರ್ಣ *path,
			       ಸಂರಚನಾ oid_array *oids,
			       ಗಣನಾ object_type type,
			       ಶೂನ್ಯ *data)
{
	size_t oe_start = to_pack.nr_objects;
	size_t oe_end;
	unsigned ಪೂರ್ಣಾಙ್ಕ *processed = data;

	/*
	 * First, add all objects to the packing data, including the ones
	 * marked UNINTERESTING (translated to 'exclude') as they can be
	 * used as delta bases.
	 */
	ಪ್ರತಿ (size_t i = 0; i < oids->nr; i++) {
		ಪೂರ್ಣಾಙ್ಕ exclude;
		ಸಂರಚನಾ object_info oi = OBJECT_INFO_INIT;
		ಸಂರಚನಾ object_id *oid = &oids->oid[i];

		/* Skip objects that do not exist locally. */
		ಯದಿ ((exclude_promisor_objects || arg_missing_action != MA_ERROR) &&
		    odb_read_object_info_extended(the_repository->objects, oid, &oi,
						  OBJECT_INFO_FOR_PREFETCH) < 0)
			ಅನುವರ್ತ;

		exclude = is_oid_uninteresting(the_repository, oid);

		ಯದಿ (exclude && !thin)
			ಅನುವರ್ತ;

		add_object_entry(oid, type, path, exclude);

		ಯದಿ (type == OBJ_COMMIT) {
			ಸಂರಚನಾ commit *commit;

			ಯದಿ (!write_bitmap_index && !use_delta_islands)
				ಅನುವರ್ತ;

			commit = lookup_commit(the_repository, oid);
			ಯದಿ (!commit)
				die(_("could not find commit %s"), oid_to_hex(oid));
			ಯದಿ (write_bitmap_index)
				index_commit_for_bitmap(commit);
			/*
			 * Skip island propagation for boundary commits.
			 * The regular traversal's show_commit() is only
			 * called for interesting commits; matching that
			 * here keeps path-walk from doing extra work that
			 * would only be a no-op anyway (boundary commits
			 * are not in island_marks).
			 */
			ಯದಿ (use_delta_islands && !exclude)
				propagate_island_marks(the_repository, commit);
		} ಅನ್ಯಥಾ ಯದಿ (type == OBJ_ವೃಕ್ಷ && use_delta_islands) {
			record_ವೃಕ್ಷ_depth(oid, path);
		}
	}

	oe_end = to_pack.nr_objects;

	/* We can skip delta calculations if it is a no-op. */
	ಯದಿ (oe_end == oe_start || !window)
		ಪ್ರತ್ಯಾಗಮ 0;

	ALLOC_GROW(to_pack.regions,
		   to_pack.nr_regions + 1,
		   to_pack.nr_regions_alloc);

	to_pack.regions[to_pack.nr_regions].start = oe_start;
	to_pack.regions[to_pack.nr_regions].nr = oe_end - oe_start;
	to_pack.nr_regions++;

	*processed += oids->nr;
	display_progress(progress_state, *processed);

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ get_object_list_path_walk(ಸಂರಚನಾ rev_info *revs)
{
	ಸಂರಚನಾ path_walk_info info = PATH_WALK_INFO_INIT;
	unsigned ಪೂರ್ಣಾಙ್ಕ processed = 0;
	ಪೂರ್ಣಾಙ್ಕ result;

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

	ಪ್ರತ್ಯಾಗಮ result;
}

ಸ್ಥಿರ ಶೂನ್ಯ get_object_list(ಸಂರಚನಾ rev_info *revs, ಸಂರಚನಾ strvec *argv)
{
	ಸಂರಚನಾ setup_revision_opt s_r_opt = {
		.allow_exclude_promisor_objects = 1,
	};
	ಸಂರಚನಾ repo_config_ಮೌಲ್ಯಗಳು *cfg = repo_config_ಮೌಲ್ಯಗಳು(the_repository);
	ವರ್ಣ ಸಾಲು[1000];
	ಪೂರ್ಣಾಙ್ಕ flags = 0;
	ಪೂರ್ಣಾಙ್ಕ save_warning;

	save_commit_buffer = 0;
	setup_revisions_from_strvec(argv, revs, &s_r_opt);

	/* make sure shallows are read */
	is_repository_shallow(the_repository);

	save_warning = cfg->warn_on_object_refname_ambiguity;
	cfg->warn_on_object_refname_ambiguity = 0;

	ಯಾವತ್ (fgets(ಸಾಲು, ಪರಿಮಾಣ(ಸಾಲು), stdin) != ರಿಕ್ತ) {
		ಪೂರ್ಣಾಙ್ಕ len = strlen(ಸಾಲು);
		ಯದಿ (len && ಸಾಲು[len - 1] == '\n')
			ಸಾಲು[--len] = 0;
		ಯದಿ (!len)
			ಭಙ್ಗ;
		ಯದಿ (*ಸಾಲು == '-') {
			ಯದಿ (!strcmp(ಸಾಲು, "--not")) {
				flags ^= UNINTERESTING;
				write_bitmap_index = 0;
				ಅನುವರ್ತ;
			}
			ಯದಿ (starts_with(ಸಾಲು, "--shallow ")) {
				ಸಂರಚನಾ object_id oid;
				ಯದಿ (get_oid_hex(ಸಾಲು + 10, &oid))
					die("not an object name '%s'", ಸಾಲು + 10);
				register_shallow(the_repository, &oid);
				use_bitmap_index = 0;
				ಅನುವರ್ತ;
			}
			die(_("not a rev '%s'"), ಸಾಲು);
		}
		ಯದಿ (handle_revision_arg(ಸಾಲು, revs, flags, REVARG_CANNOT_BE_FILENAME))
			die(_("bad revision '%s'"), ಸಾಲು);
	}

	cfg->warn_on_object_refname_ambiguity = save_warning;

	ಯದಿ (use_bitmap_index && !get_object_list_from_bitmap(revs))
		ಪ್ರತ್ಯಾಗಮ;

	ಯದಿ (use_delta_islands)
		load_delta_islands(the_repository, progress);

	ಯದಿ (write_bitmap_index)
		for_each_preferred_bitmap_tip(the_repository, mark_bitmap_preferred_tip,
					      ರಿಕ್ತ);

	ಯದಿ (!fn_show_object)
		fn_show_object = show_object;

	ಯದಿ (path_walk) {
		ಯದಿ (get_object_list_path_walk(revs)) {
			warning(_("failed to pack objects via path-walk"));
			path_walk = 0;
		}
	}

	ಯದಿ (!path_walk) {
		ಯದಿ (prepare_revision_walk(revs))
			die(_("revision walk setup failed"));
		mark_edges_uninteresting(revs, show_edge, sparse);
		traverse_commit_list(revs,
				show_commit, fn_show_object,
				ರಿಕ್ತ);
	}

	ಯದಿ (unpack_unreachable_expiration) {
		revs->ignore_missing_links = 1;
		ಯದಿ (add_unseen_recent_objects_to_traversal(revs,
				unpack_unreachable_expiration, ರಿಕ್ತ, 0))
			die(_("unable to add recent objects"));
		ಯದಿ (prepare_revision_walk(revs))
			die(_("revision walk setup failed"));
		traverse_commit_list(revs, record_recent_commit,
				     record_recent_object, ರಿಕ್ತ);
	}

	ಯದಿ (keep_unreachable)
		add_objects_in_unpacked_packs();
	ಯದಿ (pack_loose_unreachable)
		add_unreachable_loose_objects(ರಿಕ್ತ);
	ಯದಿ (unpack_unreachable)
		loosen_unused_packed_objects();

	oid_array_clear(&recent_objects);
}

ಸ್ಥಿರ ಶೂನ್ಯ add_extra_kept_packs(ಧ್ರುವ ಸಂರಚನಾ string_list *names)
{
	ಸಂರಚನಾ packed_git *p;

	ಯದಿ (!names->nr)
		ಪ್ರತ್ಯಾಗಮ;

	repo_for_each_pack(the_repository, p) {
		ಧ್ರುವ ವರ್ಣ *name = basename(p->pack_name);
		ಪೂರ್ಣಾಙ್ಕ i;

		ಯದಿ (!p->pack_local)
			ಅನುವರ್ತ;

		ಪ್ರತಿ (i = 0; i < names->nr; i++)
			ಯದಿ (!fspathcmp(name, names->ವಸ್ತುಗಳು[i].string))
				ಭಙ್ಗ;

		ಯದಿ (i < names->nr) {
			p->pack_keep_in_core = 1;
			ignore_packed_keep_in_core = 1;
			ಅನುವರ್ತ;
		}
	}
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ option_parse_quiet(ಧ್ರುವ ಸಂರಚನಾ option *opt, ಧ್ರುವ ವರ್ಣ *arg,
			      ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಪೂರ್ಣಾಙ್ಕ *val = opt->ಮೌಲ್ಯ;

	BUG_ON_OPT_ARG(arg);

	ಯದಿ (!unset)
		*val = 0;
	ಅನ್ಯಥಾ ಯದಿ (!*val)
		*val = 1;
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ option_parse_index_version(ಧ್ರುವ ಸಂರಚನಾ option *opt,
				      ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಸಂರಚನಾ pack_idx_option *popts = opt->ಮೌಲ್ಯ;
	ವರ್ಣ *c;
	ಧ್ರುವ ವರ್ಣ *val = arg;

	BUG_ON_OPT_NEG(unset);

	popts->version = strtoul(val, &c, 10);
	ಯದಿ (popts->version > 2)
		die(_("unsupported index version %s"), val);
	ಯದಿ (*c == ',' && c[1])
		popts->off32_limit = strtoul(c+1, &c, 0);
	ಯದಿ (*c || popts->off32_limit & 0x80000000)
		die(_("bad index version '%s'"), val);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ option_parse_unpack_unreachable(ಧ್ರುವ ಸಂರಚನಾ option *opt UNUSED,
					   ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಯದಿ (unset) {
		unpack_unreachable = 0;
		unpack_unreachable_expiration = 0;
	}
	ಅನ್ಯಥಾ {
		unpack_unreachable = 1;
		ಯದಿ (arg)
			unpack_unreachable_expiration = approxidate(arg);
	}
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ option_parse_cruft_expiration(ಧ್ರುವ ಸಂರಚನಾ option *opt UNUSED,
					 ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಯದಿ (unset) {
		cruft = 0;
		cruft_expiration = 0;
	} ಅನ್ಯಥಾ {
		cruft = 1;
		ಯದಿ (arg)
			cruft_expiration = approxidate(arg);
	}
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ is_not_in_promisor_pack_obj(ಸಂರಚನಾ object *obj, ಶೂನ್ಯ *data UNUSED)
{
	ಸಂರಚನಾ odb_source_info source_info;
	ಸಂರಚನಾ object_info info = {
		.source_infop = &source_info,
	};

	ಯದಿ (odb_read_object_info_extended(the_repository->objects, &obj->oid, &info, 0))
		BUG("should_include_obj should only be called on existing objects");
	ಪ್ರತ್ಯಾಗಮ source_info.source->type != ODB_SOURCE_PACKED || !source_info.u.packed.pack->pack_promisor;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ is_not_in_promisor_pack(ಸಂರಚನಾ commit *commit, ಶೂನ್ಯ *data) {
	ಪ್ರತ್ಯಾಗಮ is_not_in_promisor_pack_obj((ಸಂರಚನಾ object *) commit, data);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ parse_stdin_packs_mode(ಧ್ರುವ ಸಂರಚನಾ option *opt, ಧ್ರುವ ವರ್ಣ *arg,
				  ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಗಣನಾ stdin_packs_mode *mode = opt->ಮೌಲ್ಯ;

	ಯದಿ (unset)
		*mode = STDIN_PACKS_MODE_NONE;
	ಅನ್ಯಥಾ ಯದಿ (!arg || !*arg)
		*mode = STDIN_PACKS_MODE_STANDARD;
	ಅನ್ಯಥಾ ಯದಿ (!strcmp(arg, "follow"))
		*mode = STDIN_PACKS_MODE_FOLLOW;
	ಅನ್ಯಥಾ
		die(_("invalid value for '%s': '%s'"), opt->long_name, arg);

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ cmd_pack_objects(ಪೂರ್ಣಾಙ್ಕ argc,
		     ಧ್ರುವ ವರ್ಣ **argv,
		     ಧ್ರುವ ವರ್ಣ *prefix,
		     ಸಂರಚನಾ repository *repo)
{
	ಪೂರ್ಣಾಙ್ಕ use_internal_rev_list = 0;
	ಪೂರ್ಣಾಙ್ಕ all_progress_implied = 0;
	ಸಂರಚನಾ strvec rp = STRVEC_INIT;
	ಪೂರ್ಣಾಙ್ಕ rev_list_unpacked = 0, rev_list_all = 0, rev_list_reflog = 0;
	ಪೂರ್ಣಾಙ್ಕ rev_list_index = 0;
	ಗಣನಾ stdin_packs_mode stdin_packs = STDIN_PACKS_MODE_NONE;
	ಸಂರಚನಾ string_list keep_pack_list = STRING_LIST_INIT_NODUP;
	ಸಂರಚನಾ list_objects_filter_options filter_options =
		LIST_OBJECTS_FILTER_INIT;
	ಸಂರಚನಾ repo_config_ಮೌಲ್ಯಗಳು *cfg = repo_config_ಮೌಲ್ಯಗಳು(the_repository);

	ಸಂರಚನಾ option pack_objects_options[] = {
		OPT_CALLBACK_F('q', "quiet", &progress, ರಿಕ್ತ,
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
		OPT_INTEGER(0, "threads", &delta_ಹುಡುಕು_threads,
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
		OPT_CALLBACK_F(0, "unpack-unreachable", ರಿಕ್ತ, N_("time"),
		  N_("unpack unreachable objects newer than <time>"),
		  PARSE_OPT_OPTARG, option_parse_unpack_unreachable),
		OPT_BOOL(0, "cruft", &cruft, N_("create a cruft pack")),
		OPT_CALLBACK_F(0, "cruft-expiration", ರಿಕ್ತ, N_("time"),
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

	ಯದಿ (DFS_ಅಂಕೆ_STATES > (1 << OE_DFS_STATE_BITS))
		BUG("too many dfs states, increase OE_DFS_STATE_BITS");

	disable_replace_refs();

	sparse = git_env_bool("GIT_TEST_PACK_SPARSE", -1);
	ಯದಿ (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		ಯದಿ (sparse < 0)
			sparse = the_repository->settings.pack_use_sparse;
		ಯದಿ (the_repository->settings.pack_use_multi_pack_reuse)
			allow_pack_reuse = MULTI_PACK_REUSE;
	}

	reset_pack_idx_option(&pack_idx_opts);
	pack_idx_opts.flags |= WRITE_REV;
	repo_config(the_repository, git_pack_config, ರಿಕ್ತ);
	ಯದಿ (git_env_bool(GIT_TEST_NO_WRITE_REV_INDEX, 0))
		pack_idx_opts.flags &= ~WRITE_REV;

	progress = isatty(2);
	argc = parse_options(argc, argv, prefix, pack_objects_options,
			     pack_usage, 0);

	ಯದಿ (argc) {
		base_name = argv[0];
		argc--;
	}
	ಯದಿ (pack_to_stdout != !base_name || argc)
		usage_with_options(pack_usage, pack_objects_options);

	ಯದಿ (path_walk < 0) {
		ಯದಿ (use_bitmap_index > 0 ||
		    !use_internal_rev_list)
			path_walk = 0;
		ಅನ್ಯಥಾ ಯದಿ (the_repository->gitdir &&
			 the_repository->settings.pack_use_path_walk)
			path_walk = 1;
		ಅನ್ಯಥಾ
			path_walk = git_env_bool("GIT_TEST_PACK_PATH_WALK", 0);
	}

	ಯದಿ (depth < 0)
		depth = 0;
	ಯದಿ (depth >= (1 << OE_DEPTH_BITS)) {
		warning(_("delta chain depth %d is too deep, forcing %d"),
			depth, (1 << OE_DEPTH_BITS) - 1);
		depth = (1 << OE_DEPTH_BITS) - 1;
	}
	ಯದಿ (cache_max_small_delta_size >= (1U << OE_Z_DELTA_BITS)) {
		warning(_("pack.deltaCacheLimit is too high, forcing %d"),
			(1U << OE_Z_DELTA_BITS) - 1);
		cache_max_small_delta_size = (1U << OE_Z_DELTA_BITS) - 1;
	}
	ಯದಿ (window < 0)
		window = 0;

	strvec_ತಳ್ಳು(&rp, "pack-objects");

	ಯದಿ (path_walk) {
		ಧ್ರುವ ವರ್ಣ *option = ರಿಕ್ತ;
		ಯದಿ (!path_walk_filter_compatible(&filter_options))
			option = "--filter";

		ಯದಿ (option) {
			warning(_("cannot use %s with %s"),
				option, "--path-walk");
			path_walk = 0;
		}
	}
	ಯದಿ (path_walk) {
		strvec_ತಳ್ಳು(&rp, "--objects");
	} ಅನ್ಯಥಾ ಯದಿ (thin) {
		use_internal_rev_list = 1;
		strvec_ತಳ್ಳು(&rp, shallow
				? "--objects-edge-aggressive"
				: "--objects-edge");
	} ಅನ್ಯಥಾ
		strvec_ತಳ್ಳು(&rp, "--objects");

	ಯದಿ (rev_list_all) {
		use_internal_rev_list = 1;
		strvec_ತಳ್ಳು(&rp, "--all");
	}
	ಯದಿ (rev_list_reflog) {
		use_internal_rev_list = 1;
		strvec_ತಳ್ಳು(&rp, "--reflog");
	}
	ಯದಿ (rev_list_index) {
		use_internal_rev_list = 1;
		strvec_ತಳ್ಳು(&rp, "--indexed-objects");
	}
	ಯದಿ (rev_list_unpacked && !stdin_packs) {
		use_internal_rev_list = 1;
		strvec_ತಳ್ಳು(&rp, "--unpacked");
	}

	die_for_incompatible_opt2(exclude_promisor_objects,
				  "--exclude-promisor-objects",
				  exclude_promisor_objects_best_effort,
				  "--exclude-promisor-objects-best-effort");
	ಯದಿ (exclude_promisor_objects) {
		repo->fetch_if_missing = 0;

		/* --stdin-packs handles promisor objects separately. */
		ಯದಿ (!stdin_packs) {
			use_internal_rev_list = 1;
			strvec_ತಳ್ಳು(&rp, "--exclude-promisor-objects");
		}
	} ಅನ್ಯಥಾ ಯದಿ (exclude_promisor_objects_best_effort) {
		use_internal_rev_list = 1;
		arg_missing_action = MA_ALLOW_ANY;
		repo->fetch_if_missing = 0;
		fn_show_object = show_object__ma_allow_any;
		/* revs configured below */
	}
	ಯದಿ (unpack_unreachable || keep_unreachable || pack_loose_unreachable)
		use_internal_rev_list = 1;

	ಯದಿ (!reuse_object)
		reuse_delta = 0;
	ಯದಿ (cfg->pack_compression_level == -1)
		cfg->pack_compression_level = Z_DEFAULT_COMPRESSION;
	ಅನ್ಯಥಾ ಯದಿ (cfg->pack_compression_level < 0 || cfg->pack_compression_level > Z_BEST_COMPRESSION)
		die(_("bad pack compression level %d"), cfg->pack_compression_level);

	ಯದಿ (!delta_ಹುಡುಕು_threads)	/* --threads=0 means autodetect */
		delta_ಹುಡುಕು_threads = online_cpus();

	ಯದಿ (!HAVE_THREADS && delta_ಹುಡುಕು_threads != 1)
		warning(_("no threads support, ignoring --threads"));
	ಯದಿ (!pack_to_stdout && !pack_size_limit)
		pack_size_limit = pack_size_limit_cfg;
	ಯದಿ (pack_to_stdout && pack_size_limit)
		die(_("--max-pack-size cannot be used to build a pack for transfer"));
	ಯದಿ (pack_size_limit && pack_size_limit < 1024*1024) {
		warning(_("minimum pack size limit is 1 MiB"));
		pack_size_limit = 1024*1024;
	}

	ಯದಿ (!pack_to_stdout && thin)
		die(_("--thin cannot be used to build an indexable pack"));

	die_for_incompatible_opt2(keep_unreachable, "--keep-unreachable",
				  unpack_unreachable, "--unpack-unreachable");
	ಯದಿ (!rev_list_all || !rev_list_reflog || !rev_list_index)
		unpack_unreachable_expiration = 0;

	die_for_incompatible_opt2(stdin_packs, "--stdin-packs",
				  filter_options.choice, "--filter");


	ಯದಿ (stdin_packs && use_internal_rev_list)
		die(_("cannot use internal rev list with --stdin-packs"));

	ಯದಿ (cruft) {
		ಯದಿ (use_internal_rev_list)
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
	ಯದಿ (!pack_to_stdout)
		use_bitmap_index_default = 0;

	ಯದಿ (use_bitmap_index < 0)
		use_bitmap_index = use_bitmap_index_default;

	/* "hard" reasons not to use bitmaps; these just won't work at all */
	ಯದಿ (!use_internal_rev_list || (!pack_to_stdout && write_bitmap_index) || is_repository_shallow(the_repository))
		use_bitmap_index = 0;

	ಯದಿ (pack_to_stdout || !rev_list_all)
		write_bitmap_index = 0;

	ಯದಿ (name_hash_version < 0)
		name_hash_version = (ಪೂರ್ಣಾಙ್ಕ)git_env_ulong("GIT_TEST_NAME_HASH_VERSION", 1);

	validate_name_hash_version();

	ಯದಿ (use_delta_islands)
		strvec_ತಳ್ಳು(&rp, "--topo-order");

	ಯದಿ (progress && all_progress_implied)
		progress = 2;

	add_extra_kept_packs(&keep_pack_list);
	ಯದಿ (ignore_packed_keep_on_disk) {
		ಸಂರಚನಾ packed_git *p;

		repo_for_each_pack(the_repository, p)
			ಯದಿ (p->pack_local && p->pack_keep)
				ಭಙ್ಗ;
		ಯದಿ (!p) /* no keep-able packs found */
			ignore_packed_keep_on_disk = 0;
	}
	ಯದಿ (local) {
		/*
		 * unlike ignore_packed_keep_on_disk above, we do not
		 * want to unset "local" based on looking at packs, as
		 * it also covers non-local objects
		 */
		ಸಂರಚನಾ packed_git *p;

		repo_for_each_pack(the_repository, p) {
			ಯದಿ (!p->pack_local) {
				have_non_local_packs = 1;
				ಭಙ್ಗ;
			}
		}
	}

	trace2_region_enter("pack-objects", "enumerate-objects",
			    the_repository);
	prepare_packing_data(the_repository, &to_pack);

	ಯದಿ (progress && !cruft)
		progress_state = start_progress(the_repository,
						_("Enumerating objects"), 0);
	ಯದಿ (stdin_packs) {
		read_stdin_packs(repo, stdin_packs, rev_list_unpacked);
	} ಅನ್ಯಥಾ ಯದಿ (cruft) {
		read_cruft_objects();
	} ಅನ್ಯಥಾ ಯದಿ (!use_internal_rev_list) {
		read_object_list_from_stdin();
	} ಅನ್ಯಥಾ {
		ಸಂರಚನಾ rev_info revs;

		repo_init_revisions(the_repository, &revs, ರಿಕ್ತ);
		list_objects_filter_copy(&revs.filter, &filter_options);
		ಯದಿ (exclude_promisor_objects_best_effort) {
			revs.include_check = is_not_in_promisor_pack;
			revs.include_check_obj = is_not_in_promisor_pack_obj;
		}
		get_object_list(&revs, &rp);
		release_revisions(&revs);
	}
	cleanup_preferred_base();
	ಯದಿ (include_tag && nr_result)
		refs_for_each_tag_ref(get_main_ref_store(the_repository),
				      add_ref_tag, ರಿಕ್ತ);
	stop_progress(&progress_state);
	trace2_region_leave("pack-objects", "enumerate-objects",
			    the_repository);

	ಯದಿ (non_empty && !nr_result)
		ಗಚ್ಛ cleanup;
	ಯದಿ (nr_result) {
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

	ಯದಿ (progress)
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

	ಪ್ರತ್ಯಾಗಮ 0;
}
