#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE
#ವ್ಯಾಖ್ಯಾನಿಸು DISABLE_SIGN_COMPARE_WARNINGS

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
ಸ್ಥಿರ ರಚನೆ packing_ದತ್ತಾಂಶ ಗಮ್ಯ_pack;

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ object_ನಮೂದು *oe_delta(
		ಸ್ಥಿರಾಂಕ ರಚನೆ packing_ದತ್ತಾಂಶ *pack,
		ಸ್ಥಿರಾಂಕ ರಚನೆ object_ನಮೂದು *e)
{
	ಆದರೆ (!e->delta_idx)
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;
	ಆದರೆ (e->ext_ತಳ)
		ಹಿಂತಿರುಗಿ &pack->ext_bases[e->delta_idx - 1];
	ಇಲ್ಲದಿದ್ದರೆ
		ಹಿಂತಿರುಗಿ &pack->objects[e->delta_idx - 1];
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಗಾತ್ರ_t oe_delta_ಗಾತ್ರ(ರಚನೆ packing_ದತ್ತಾಂಶ *pack,
				   ಸ್ಥಿರಾಂಕ ರಚನೆ object_ನಮೂದು *e)
{
	ಆದರೆ (e->delta_ಗಾತ್ರ_valid)
		ಹಿಂತಿರುಗಿ e->delta_ಗಾತ್ರ;

	/*
	 * pack->delta_size[] can't be NULL because oe_set_delta_size()
	 * must have been called when a new delta is saved with
	 * oe_set_delta().
	 * If oe_delta() returns NULL (i.e. default state, which means
	 * delta_size_valid is also false), then the caller must never
	 * call oe_delta_size().
	 */
	ಹಿಂತಿರುಗಿ pack->delta_ಗಾತ್ರ_2[e - pack->objects];
}

ಗಾತ್ರ_t oe_get_ಗಾತ್ರ_slow(ರಚನೆ packing_ದತ್ತಾಂಶ *pack,
			ಸ್ಥಿರಾಂಕ ರಚನೆ object_ನಮೂದು *e);

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಗಾತ್ರ_t oe_ಗಾತ್ರ(ರಚನೆ packing_ದತ್ತಾಂಶ *pack,
			     ಸ್ಥಿರಾಂಕ ರಚನೆ object_ನಮೂದು *e)
{
	ಆದರೆ (e->ಗಾತ್ರ_valid)
		ಹಿಂತಿರುಗಿ e->ಗಾತ್ರ_3;

	ಹಿಂತಿರುಗಿ oe_get_ಗಾತ್ರ_slow(pack, e);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ oe_ಗಣ_delta(ರಚನೆ packing_ದತ್ತಾಂಶ *pack,
				ರಚನೆ object_ನಮೂದು *e,
				ರಚನೆ object_ನಮೂದು *delta)
{
	ಆದರೆ (delta)
		e->delta_idx = (delta - pack->objects) + 1;
	ಇಲ್ಲದಿದ್ದರೆ
		e->delta_idx = 0;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ object_ನಮೂದು *oe_delta_sibling(
		ಸ್ಥಿರಾಂಕ ರಚನೆ packing_ದತ್ತಾಂಶ *pack,
		ಸ್ಥಿರಾಂಕ ರಚನೆ object_ನಮೂದು *e)
{
	ಆದರೆ (e->delta_sibling_idx)
		ಹಿಂತಿರುಗಿ &pack->objects[e->delta_sibling_idx - 1];
	ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ object_ನಮೂದು *oe_delta_ಮಗು(
		ಸ್ಥಿರಾಂಕ ರಚನೆ packing_ದತ್ತಾಂಶ *pack,
		ಸ್ಥಿರಾಂಕ ರಚನೆ object_ನಮೂದು *e)
{
	ಆದರೆ (e->delta_ಮಗು_idx)
		ಹಿಂತಿರುಗಿ &pack->objects[e->delta_ಮಗು_idx - 1];
	ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ oe_ಗಣ_delta_ಮಗು(ರಚನೆ packing_ದತ್ತಾಂಶ *pack,
				      ರಚನೆ object_ನಮೂದು *e,
				      ರಚನೆ object_ನಮೂದು *delta)
{
	ಆದರೆ (delta)
		e->delta_ಮಗು_idx = (delta - pack->objects) + 1;
	ಇಲ್ಲದಿದ್ದರೆ
		e->delta_ಮಗು_idx = 0;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ oe_ಗಣ_delta_sibling(ರಚನೆ packing_ದತ್ತಾಂಶ *pack,
					ರಚನೆ object_ನಮೂದು *e,
					ರಚನೆ object_ನಮೂದು *delta)
{
	ಆದರೆ (delta)
		e->delta_sibling_idx = (delta - pack->objects) + 1;
	ಇಲ್ಲದಿದ್ದರೆ
		e->delta_sibling_idx = 0;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ oe_ಗಣ_ಗಾತ್ರ(ರಚನೆ packing_ದತ್ತಾಂಶ *pack,
			       ರಚನೆ object_ನಮೂದು *e,
			       ಗಾತ್ರ_t ಗಾತ್ರ_2)
{
	ಆದರೆ (ಗಾತ್ರ_2 < pack->oe_ಗಾತ್ರ_ಎಲ್ಲೆ) {
		e->ಗಾತ್ರ_3 = ಗಾತ್ರ_2;
		e->ಗಾತ್ರ_valid = 1;
	} ಇಲ್ಲದಿದ್ದರೆ {
		e->ಗಾತ್ರ_valid = 0;
		ಆದರೆ (oe_get_ಗಾತ್ರ_slow(pack, e) != ಗಾತ್ರ_2)
			BUG("'size' is supposed to be the object size!");
	}
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ oe_ಗಣ_delta_ಗಾತ್ರ(ರಚನೆ packing_ದತ್ತಾಂಶ *pack,
				     ರಚನೆ object_ನಮೂದು *e,
				     ಗಾತ್ರ_t ಗಾತ್ರ_2)
{
	ಆದರೆ (ಗಾತ್ರ_2 < pack->oe_delta_ಗಾತ್ರ_ಎಲ್ಲೆ) {
		e->delta_ಗಾತ್ರ = ಗಾತ್ರ_2;
		e->delta_ಗಾತ್ರ_valid = 1;
	} ಇಲ್ಲದಿದ್ದರೆ {
		packing_ದತ್ತಾಂಶ_ಬೀಗ(pack);
		ಆದರೆ (!pack->delta_ಗಾತ್ರ_2)
			ALLOC_ARRAY(pack->delta_ಗಾತ್ರ_2, pack->nr_alloc);
		packing_ದತ್ತಾಂಶ_unlock(pack);

		pack->delta_ಗಾತ್ರ_2[e - pack->objects] = ಗಾತ್ರ_2;
		e->delta_ಗಾತ್ರ_valid = 0;
	}
}

#ವ್ಯಾಖ್ಯಾನಿಸು ಒಳ_PACK(obj) oe_ಒಳ_pack(&ಗಮ್ಯ_pack, obj)
#ವ್ಯಾಖ್ಯಾನಿಸು ಗಾತ್ರ_4(obj) oe_ಗಾತ್ರ(&ಗಮ್ಯ_pack, obj)
#ವ್ಯಾಖ್ಯಾನಿಸು ಗಣ_ಗಾತ್ರ(obj,ಗಾತ್ರ_2) oe_ಗಣ_ಗಾತ್ರ(&ಗಮ್ಯ_pack, obj, ಗಾತ್ರ_2)
#ವ್ಯಾಖ್ಯಾನಿಸು DELTA_ಗಾತ್ರ(obj) oe_delta_ಗಾತ್ರ(&ಗಮ್ಯ_pack, obj)
#ವ್ಯಾಖ್ಯಾನಿಸು DELTA(obj) oe_delta(&ಗಮ್ಯ_pack, obj)
#ವ್ಯಾಖ್ಯಾನಿಸು DELTA_ಮಗು(obj) oe_delta_ಮಗು(&ಗಮ್ಯ_pack, obj)
#ವ್ಯಾಖ್ಯಾನಿಸು DELTA_SIBLING(obj) oe_delta_sibling(&ಗಮ್ಯ_pack, obj)
#ವ್ಯಾಖ್ಯಾನಿಸು ಗಣ_DELTA(obj, val) oe_ಗಣ_delta(&ಗಮ್ಯ_pack, obj, val)
#ವ್ಯಾಖ್ಯಾನಿಸು ಗಣ_DELTA_EXT(obj, oid) oe_ಗಣ_delta_ext(&ಗಮ್ಯ_pack, obj, oid)
#ವ್ಯಾಖ್ಯಾನಿಸು ಗಣ_DELTA_ಗಾತ್ರ(obj, val) oe_ಗಣ_delta_ಗಾತ್ರ(&ಗಮ್ಯ_pack, obj, val)
#ವ್ಯಾಖ್ಯಾನಿಸು ಗಣ_DELTA_ಮಗು(obj, val) oe_ಗಣ_delta_ಮಗು(&ಗಮ್ಯ_pack, obj, val)
#ವ್ಯಾಖ್ಯಾನಿಸು ಗಣ_DELTA_SIBLING(obj, val) oe_ಗಣ_delta_sibling(&ಗಮ್ಯ_pack, obj, val)

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ pack_usage[] = {
	N_("git pack-objects [-q | --progress | --all-progress] [--all-progress-implied]\n"
	   "                 [--no-reuse-delta] [--delta-base-offset] [--non-empty]\n"
	   "                 [--local] [--incremental] [--window=<n>] [--depth=<n>]\n"
	   "                 [--revs [--unpacked | --all]] [--keep-pack=<pack-name>]\n"
	   "                 [--cruft] [--cruft-expiration=<time>]\n"
	   "                 [--stdout [--filter=<filter-spec>] | <base-name>]\n"
	   "                 [--shallow] [--keep-true-parents] [--[no-]sparse]\n"
	   "                 [--name-hash-version=<n>] [--path-walk] < <object-list>"),
	ಖಾಲಿ
};

ಸ್ಥಿರ ರಚನೆ pack_idx_ನಮೂದು **written_list;
ಸ್ಥಿರ uint32_t nr_ಫಲಿತಾಂಶ, nr_written, nr_ನೋಡಿದ;
ಸ್ಥಿರ ರಚನೆ bitmap_ಸೂಚ್ಯಂಕ *bitmap_git;
ಸ್ಥಿರ uint32_t write_layer;

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ non_ಖಾಲಿಗಳು;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ reuse_delta = 1, reuse_object = 1;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ keep_unreachable, unpack_unreachable, include_ಟ್ಯಾಗ್;
ಸ್ಥಿರ timestamp_t unpack_unreachable_expiration;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ pack_loose_unreachable;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cruft;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ shallow = 0;
ಸ್ಥಿರ timestamp_t cruft_expiration;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ local;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ have_non_local_packs;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ incremental;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ignore_packed_keep_on_disk;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ignore_packed_keep_ಒಳ_core;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ignore_packed_keep_ಒಳ_core_open;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ignore_packed_keep_ಒಳ_core_has_cruft;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ allow_ofs_delta;
ಸ್ಥಿರ ರಚನೆ pack_idx_option pack_idx_opts;
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ತಳ_name;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಪ್ರಗತಿ = 1;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಕಿಟಕಿ = 10;
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ pack_ಗಾತ್ರ_ಎಲ್ಲೆ;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಆಳ = 50;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ delta_ಹುಡುಕು_ಎಳೆಗಳು;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ pack_ಗಮ್ಯ_stdout;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sparse;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ thin;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಮಾರ್ಗ_walk = -1;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಅಂಕೆ_preferred_ತಳ;
ಸ್ಥಿರ ರಚನೆ ಪ್ರಗತಿ *ಪ್ರಗತಿ_state;

ಸ್ಥಿರ ರಚನೆ bitmapped_pack *reuse_packfiles;
ಸ್ಥಿರ ಗಾತ್ರ_t reuse_packfiles_nr;
ಸ್ಥಿರ ಗಾತ್ರ_t reuse_packfiles_ಬಳಸಿದ_nr;
ಸ್ಥಿರ uint32_t reuse_packfile_objects;
ಸ್ಥಿರ ರಚನೆ bitmap *reuse_packfile_bitmap;

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ use_bitmap_ಸೂಚ್ಯಂಕ_default = 1;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ use_bitmap_ಸೂಚ್ಯಂಕ = -1;
ಸ್ಥಿರ ಎಣಿಕೆ {
	NO_PACK_REUSE = 0,
	SINGLE_PACK_REUSE,
	MULTI_PACK_REUSE,
} allow_pack_reuse = SINGLE_PACK_REUSE;
ಸ್ಥಿರ ಎಣಿಕೆ {
	WRITE_BITMAP_FALSE = 0,
	WRITE_BITMAP_QUIET,
	WRITE_BITMAP_TRUE,
} write_bitmap_ಸೂಚ್ಯಂಕ;
ಸ್ಥಿರ uint16_t write_bitmap_options = BITMAP_OPT_HASH_CACHE;

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ exclude_promisor_objects;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ exclude_promisor_objects_ಉತ್ತಮ_effort;

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ use_delta_islands;

ಸ್ಥಿರ ಗಾತ್ರ_t delta_cache_ಗಾತ್ರ = 0;
ಸ್ಥಿರ ಗಾತ್ರ_t max_delta_cache_ಗಾತ್ರ = DEFAULT_DELTA_CACHE_ಗಾತ್ರ;
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ cache_max_small_delta_ಗಾತ್ರ = 1000;

ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಕಿಟಕಿ_ಸ್ಮರಣೆ_ಎಲ್ಲೆ = 0;

ಸ್ಥಿರ ರಚನೆ string_list uri_protocols = STRING_LIST_INIT_NODUP;

ಎಣಿಕೆ missing_action {
	MA_ERROR = 0,      /* fail if any missing objects are encountered */
	MA_ALLOW_ANY,      /* silently allow ALL missing objects */
	MA_ALLOW_PROMISOR, /* silently allow all missing PROMISOR objects */
};
ಸ್ಥಿರ ಎಣಿಕೆ missing_action arg_missing_action;
ಸ್ಥಿರ show_object_fn fn_show_object;

ರಚನೆ configured_exclusion {
	ರಚನೆ oidmap_ನಮೂದು e;
	ಅಕ್ಷರ *pack_hash_hex;
	ಅಕ್ಷರ *uri;
};
ಸ್ಥಿರ ರಚನೆ oidmap configured_exclusions;

ಸ್ಥಿರ ರಚನೆ oidset excluded_by_config;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ name_hash_ಆವೃತ್ತಿ = -1;

ಎಣಿಕೆ stdin_packs_ಬಹುಲಕ {
	STDIN_PACKS_ಬಹುಲಕ_NONE,
	STDIN_PACKS_ಬಹುಲಕ_STANDARD,
	STDIN_PACKS_ಬಹುಲಕ_FOLLOW,
};

/**
 * Check whether the name_hash_version chosen by user input is appropriate,
 * and also validate whether it is compatible with other features.
 */
ಸ್ಥಿರ ಶೂನ್ಯ validate_name_hash_ಆವೃತ್ತಿ(ಶೂನ್ಯ)
{
	ಆದರೆ (name_hash_ಆವೃತ್ತಿ < 1 || name_hash_ಆವೃತ್ತಿ > 2)
		die(_("invalid --name-hash-version option: %d"), name_hash_ಆವೃತ್ತಿ);
	ಆದರೆ (write_bitmap_ಸೂಚ್ಯಂಕ && name_hash_ಆವೃತ್ತಿ != 1) {
		warning(_("currently, --write-bitmap-index requires --name-hash-version=1"));
		name_hash_ಆವೃತ್ತಿ = 1;
	}
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ uint32_t pack_name_hash_fn(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name)
{
	ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ನೋಡಿದ_ಆವೃತ್ತಿ = -1;

	ಆದರೆ (ನೋಡಿದ_ಆವೃತ್ತಿ < 0)
		ನೋಡಿದ_ಆವೃತ್ತಿ = name_hash_ಆವೃತ್ತಿ;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ನೋಡಿದ_ಆವೃತ್ತಿ != name_hash_ಆವೃತ್ತಿ)
		BUG("name hash version changed from %d to %d mid-process",
		    ನೋಡಿದ_ಆವೃತ್ತಿ, name_hash_ಆವೃತ್ತಿ);

	ಆಯ್ಕೆ (name_hash_ಆವೃತ್ತಿ) {
	ಸ್ಥಿತಿ 1:
		ಹಿಂತಿರುಗಿ pack_name_hash(name);

	ಸ್ಥಿತಿ 2:
		ಹಿಂತಿರುಗಿ pack_name_hash_v2((ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *)name);

	ಪೂರ್ವನಿಯೋಜಿತ:
		BUG("invalid name-hash version: %d", name_hash_ಆವೃತ್ತಿ);
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
ಸ್ಥಿರ ರಚನೆ commit **ಸೂಚ್ಯಂಕಿತ_commits;
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕಿತ_commits_nr;
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕಿತ_commits_alloc;

ಸ್ಥಿರ ಶೂನ್ಯ ಸೂಚ್ಯಂಕ_commit_for_bitmap(ರಚನೆ commit *commit)
{
	ಆದರೆ (ಸೂಚ್ಯಂಕಿತ_commits_nr >= ಸೂಚ್ಯಂಕಿತ_commits_alloc) {
		ಸೂಚ್ಯಂಕಿತ_commits_alloc = (ಸೂಚ್ಯಂಕಿತ_commits_alloc + 32) * 2;
		REALLOC_ARRAY(ಸೂಚ್ಯಂಕಿತ_commits, ಸೂಚ್ಯಂಕಿತ_commits_alloc);
	}

	ಸೂಚ್ಯಂಕಿತ_commits[ಸೂಚ್ಯಂಕಿತ_commits_nr++] = commit;
}

ಸ್ಥಿರ ಶೂನ್ಯ *get_delta(ರಚನೆ object_ನಮೂದು *ನಮೂದು)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಗಾತ್ರ_2, ತಳ_ಗಾತ್ರ;
	ಗಾತ್ರ_t delta_ಗಾತ್ರ_2;
	ಶೂನ್ಯ *buf, *ತಳ_buf, *delta_buf;
	ಎಣಿಕೆ object_type type;
	ಗಾತ್ರ_t ಗಾತ್ರ_st = 0, ತಳ_ಗಾತ್ರ_st = 0;

	buf = odb_read_object(the_repository->objects, &ನಮೂದು->idx.oid,
			      &type, &ಗಾತ್ರ_st);
	ಗಾತ್ರ_2 = cast_ಗಾತ್ರ_t_ಗಮ್ಯ_ulong(ಗಾತ್ರ_st);
	ಆದರೆ (!buf)
		die(_("unable to read %s"), oid_ಗಮ್ಯ_hex(&ನಮೂದು->idx.oid));
	ತಳ_buf = odb_read_object(the_repository->objects,
				   &DELTA(ನಮೂದು)->idx.oid, &type,
				   &ತಳ_ಗಾತ್ರ_st);
	ತಳ_ಗಾತ್ರ = cast_ಗಾತ್ರ_t_ಗಮ್ಯ_ulong(ತಳ_ಗಾತ್ರ_st);
	ಆದರೆ (!ತಳ_buf)
		die("unable to read %s",
		    oid_ಗಮ್ಯ_hex(&DELTA(ನಮೂದು)->idx.oid));
	delta_buf = diff_delta(ತಳ_buf, ತಳ_ಗಾತ್ರ,
			       buf, ಗಾತ್ರ_2, &delta_ಗಾತ್ರ_2, 0);
	/*
	 * We successfully computed this delta once but dropped it for
	 * memory reasons. Something is very wrong if this time we
	 * recompute and create a different delta.
	 */
	ಆದರೆ (!delta_buf || delta_ಗಾತ್ರ_2 != DELTA_ಗಾತ್ರ(ನಮೂದು))
		BUG("delta size changed");
	ಬಿಡುಗಡೆ(buf);
	ಬಿಡುಗಡೆ(ತಳ_buf);
	ಹಿಂತಿರುಗಿ delta_buf;
}

ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ do_compress(ಶೂನ್ಯ **pptr, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಗಾತ್ರ_2)
{
	git_zstream ಹರಿವು;
	ಶೂನ್ಯ *ಒಳ, *ಹೊರಹೋಗು;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ maxsize;
	ರಚನೆ repo_config_ಮೌಲ್ಯಗಳು *cfg = repo_config_ಮೌಲ್ಯಗಳು(the_repository);

	git_deflate_init(&ಹರಿವು, cfg->pack_compression_level);
	maxsize = git_deflate_bound(&ಹರಿವು, ಗಾತ್ರ_2);

	ಒಳ = *pptr;
	ಹೊರಹೋಗು = xmalloc(maxsize);
	*pptr = ಹೊರಹೋಗು;

	ಹರಿವು.ಮುಂದಿನ_ಒಳ = ಒಳ;
	ಹರಿವು.avail_ಒಳ = ಗಾತ್ರ_2;
	ಹರಿವು.ಮುಂದಿನ_ಹೊರಹೋಗು = ಹೊರಹೋಗು;
	ಹರಿವು.avail_ಹೊರಹೋಗು = maxsize;
	ಆಗ (git_deflate(&ಹರಿವು, Z_ಮುಕ್ತಾಯ) == Z_OK)
		; /* nothing */
	git_deflate_ಕೊನೆ(&ಹರಿವು);

	ಬಿಡುಗಡೆ(ಒಳ);
	ಹಿಂತಿರುಗಿ ಹರಿವು.ಒಟ್ಟು_ಹೊರಹೋಗು;
}

ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ write_large_blob_ದತ್ತಾಂಶ(ರಚನೆ odb_ಹರಿವು *st, ರಚನೆ hashfile *f,
					   ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid)
{
	git_zstream ಹರಿವು;
	ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ ibuf[1024 * 16];
	ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ obuf[1024 * 16];
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ olen = 0;
	ರಚನೆ repo_config_ಮೌಲ್ಯಗಳು *cfg = repo_config_ಮೌಲ್ಯಗಳು(the_repository);

	git_deflate_init(&ಹರಿವು, cfg->pack_compression_level);

	ಗಾಗಿ (;;) {
		ssize_t readlen;
		ಪೂರ್ಣಾಂಕ zret = Z_OK;
		readlen = odb_ಹರಿವು_read(st, ibuf, ಗಾತ್ರ(ibuf));
		ಆದರೆ (readlen == -1)
			die(_("unable to read %s"), oid_ಗಮ್ಯ_hex(oid));

		ಹರಿವು.ಮುಂದಿನ_ಒಳ = ibuf;
		ಹರಿವು.avail_ಒಳ = readlen;
		ಆಗ ((ಹರಿವು.avail_ಒಳ || readlen == 0) &&
		       (zret == Z_OK || zret == Z_BUF_ERROR)) {
			ಹರಿವು.ಮುಂದಿನ_ಹೊರಹೋಗು = obuf;
			ಹರಿವು.avail_ಹೊರಹೋಗು = ಗಾತ್ರ(obuf);
			zret = git_deflate(&ಹರಿವು, readlen ? 0 : Z_ಮುಕ್ತಾಯ);
			hashwrite(f, obuf, ಹರಿವು.ಮುಂದಿನ_ಹೊರಹೋಗು - obuf);
			olen += ಹರಿವು.ಮುಂದಿನ_ಹೊರಹೋಗು - obuf;
		}
		ಆದರೆ (ಹರಿವು.avail_ಒಳ)
			die(_("deflate error (%d)"), zret);
		ಆದರೆ (readlen == 0) {
			ಆದರೆ (zret != Z_ಹರಿವು_ಕೊನೆ)
				die(_("deflate error (%d)"), zret);
			ನಿಲ್ಲಿಸು;
		}
	}
	git_deflate_ಕೊನೆ(&ಹರಿವು);
	ಹಿಂತಿರುಗಿ olen;
}

/*
 * we are going to reuse the existing object data as is.  make
 * sure it is not corrupt.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ check_pack_inflate(ರಚನೆ packed_git *p,
		ರಚನೆ pack_ಕಿಟಕಿ **w_curs,
		off_t ಸ್ಥಾನಾಂತರ,
		off_t len,
		ಗಾತ್ರ_t expect)
{
	git_zstream ಹರಿವು;
	ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ fakebuf[4096], *ಒಳ;
	ಪೂರ್ಣಾಂಕ st;

	memset(&ಹರಿವು, 0, ಗಾತ್ರ(ಹರಿವು));
	git_inflate_init(&ಹರಿವು);
	ಮಾಡು {
		ಒಳ = use_pack(p, w_curs, ಸ್ಥಾನಾಂತರ, &ಹರಿವು.avail_ಒಳ);
		ಹರಿವು.ಮುಂದಿನ_ಒಳ = ಒಳ;
		ಹರಿವು.ಮುಂದಿನ_ಹೊರಹೋಗು = fakebuf;
		ಹರಿವು.avail_ಹೊರಹೋಗು = ಗಾತ್ರ(fakebuf);
		st = git_inflate(&ಹರಿವು, Z_ಮುಕ್ತಾಯ);
		ಸ್ಥಾನಾಂತರ += ಹರಿವು.ಮುಂದಿನ_ಒಳ - ಒಳ;
	} ಆಗ (st == Z_OK || st == Z_BUF_ERROR);
	git_inflate_ಕೊನೆ(&ಹರಿವು);
	ಹಿಂತಿರುಗಿ (st == Z_ಹರಿವು_ಕೊನೆ &&
		ಹರಿವು.ಒಟ್ಟು_ಹೊರಹೋಗು == expect &&
		ಹರಿವು.ಒಟ್ಟು_ಒಳ == len) ? 0 : -1;
}

ಸ್ಥಿರ ಶೂನ್ಯ copy_pack_ದತ್ತಾಂಶ(ರಚನೆ hashfile *f,
		ರಚನೆ packed_git *p,
		ರಚನೆ pack_ಕಿಟಕಿ **w_curs,
		off_t ಸ್ಥಾನಾಂತರ,
		off_t len)
{
	ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *ಒಳ;
	ಗಾತ್ರ_t avail;

	ಆಗ (len) {
		ಒಳ = use_pack(p, w_curs, ಸ್ಥಾನಾಂತರ, &avail);
		ಆದರೆ (avail > len)
			avail = (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ)len;
		hashwrite(f, ಒಳ, avail);
		ಸ್ಥಾನಾಂತರ += avail;
		len -= avail;
	}
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ oe_ಗಾತ್ರ_greater_than(ರಚನೆ packing_ದತ್ತಾಂಶ *pack,
				       ಸ್ಥಿರಾಂಕ ರಚನೆ object_ನಮೂದು *lhs,
				       ಗಾತ್ರ_t rhs)
{
	ಆದರೆ (lhs->ಗಾತ್ರ_valid)
		ಹಿಂತಿರುಗಿ lhs->ಗಾತ್ರ_3 > rhs;
	ಆದರೆ (rhs < pack->oe_ಗಾತ್ರ_ಎಲ್ಲೆ) /* rhs < 2^x <= lhs ? */
		ಹಿಂತಿರುಗಿ 1;
	ಹಿಂತಿರುಗಿ oe_get_ಗಾತ್ರ_slow(pack, lhs) > rhs;
}

/* Return 0 if we will bust the pack-size limit */
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ write_no_reuse_object(ರಚನೆ hashfile *f, ರಚನೆ object_ನಮೂದು *ನಮೂದು,
					   ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಎಲ್ಲೆ, ಪೂರ್ಣಾಂಕ usable_delta)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಗಾತ್ರ_2, datalen;
	ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ ಶೀರ್ಷಿಕೆ[MAX_PACK_OBJECT_ಶೀರ್ಷಿಕೆ],
		      dheader[MAX_PACK_OBJECT_ಶೀರ್ಷಿಕೆ];
	ಚಿಹ್ನೆರಹಿತ hdrlen;
	ಎಣಿಕೆ object_type type;
	ಶೂನ್ಯ *buf;
	ರಚನೆ odb_ಹರಿವು *st = ಖಾಲಿ;
	ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ hashsz = the_hash_algo->rawsz;

	ಆದರೆ (!usable_delta) {
		ಆದರೆ (oe_type(ನಮೂದು) == OBJ_BLOB &&
		    oe_ಗಾತ್ರ_greater_than(&ಗಮ್ಯ_pack, ನಮೂದು,
					 repo_settings_get_big_file_ಮಿತಿ(the_repository)) &&
		    (st = odb_ಹರಿವು_from_object(the_repository->objects, &ನಮೂದು->idx.oid,
						 ಖಾಲಿ)) != ಖಾಲಿ) {
			buf = ಖಾಲಿ;
			type = st->type;
			ಗಾತ್ರ_2 = st->ಗಾತ್ರ_2;
		} ಇಲ್ಲದಿದ್ದರೆ {
			ಗಾತ್ರ_t ಗಾತ್ರ_st = 0;
			buf = odb_read_object(the_repository->objects,
					      &ನಮೂದು->idx.oid, &type,
					      &ಗಾತ್ರ_st);
			ಗಾತ್ರ_2 = cast_ಗಾತ್ರ_t_ಗಮ್ಯ_ulong(ಗಾತ್ರ_st);
			ಆದರೆ (!buf)
				die(_("unable to read %s"),
				    oid_ಗಮ್ಯ_hex(&ನಮೂದು->idx.oid));
		}
		/*
		 * make sure no cached delta data remains from a
		 * previous attempt before a pack split occurred.
		 */
		FREE_AND_NULL(ನಮೂದು->delta_ದತ್ತಾಂಶ);
		ನಮೂದು->z_delta_ಗಾತ್ರ = 0;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ನಮೂದು->delta_ದತ್ತಾಂಶ) {
		ಗಾತ್ರ_2 = DELTA_ಗಾತ್ರ(ನಮೂದು);
		buf = ನಮೂದು->delta_ದತ್ತಾಂಶ;
		ನಮೂದು->delta_ದತ್ತಾಂಶ = ಖಾಲಿ;
		type = (allow_ofs_delta && DELTA(ನಮೂದು)->idx.ಸ್ಥಾನಾಂತರ) ?
			OBJ_OFS_DELTA : OBJ_REF_DELTA;
	} ಇಲ್ಲದಿದ್ದರೆ {
		buf = get_delta(ನಮೂದು);
		ಗಾತ್ರ_2 = DELTA_ಗಾತ್ರ(ನಮೂದು);
		type = (allow_ofs_delta && DELTA(ನಮೂದು)->idx.ಸ್ಥಾನಾಂತರ) ?
			OBJ_OFS_DELTA : OBJ_REF_DELTA;
	}

	ಆದರೆ (st)	/* large blob case, just assume we don't compress well */
		datalen = ಗಾತ್ರ_2;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ನಮೂದು->z_delta_ಗಾತ್ರ)
		datalen = ನಮೂದು->z_delta_ಗಾತ್ರ;
	ಇಲ್ಲದಿದ್ದರೆ
		datalen = do_compress(&buf, ಗಾತ್ರ_2);

	/*
	 * The object header is a byte of 'type' followed by zero or
	 * more bytes of length.
	 */
	hdrlen = encode_ಒಳ_pack_object_ಶೀರ್ಷಿಕೆ(ಶೀರ್ಷಿಕೆ, ಗಾತ್ರ(ಶೀರ್ಷಿಕೆ),
					      type, ಗಾತ್ರ_2);

	ಆದರೆ (type == OBJ_OFS_DELTA) {
		/*
		 * Deltas with relative base contain an additional
		 * encoding of the relative offset for the delta
		 * base from this object's position in the pack.
		 */
		off_t ofs = ನಮೂದು->idx.ಸ್ಥಾನಾಂತರ - DELTA(ನಮೂದು)->idx.ಸ್ಥಾನಾಂತರ;
		ಚಿಹ್ನೆರಹಿತ pos = ಗಾತ್ರ(dheader) - 1;
		dheader[pos] = ofs & 127;
		ಆಗ (ofs >>= 7)
			dheader[--pos] = 128 | (--ofs & 127);
		ಆದರೆ (ಎಲ್ಲೆ && hdrlen + ಗಾತ್ರ(dheader) - pos + datalen + hashsz >= ಎಲ್ಲೆ) {
			ಆದರೆ (st)
				odb_ಹರಿವು_close(st);
			ಬಿಡುಗಡೆ(buf);
			ಹಿಂತಿರುಗಿ 0;
		}
		hashwrite(f, ಶೀರ್ಷಿಕೆ, hdrlen);
		hashwrite(f, dheader + pos, ಗಾತ್ರ(dheader) - pos);
		hdrlen += ಗಾತ್ರ(dheader) - pos;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (type == OBJ_REF_DELTA) {
		/*
		 * Deltas with a base reference contain
		 * additional bytes for the base object ID.
		 */
		ಆದರೆ (ಎಲ್ಲೆ && hdrlen + hashsz + datalen + hashsz >= ಎಲ್ಲೆ) {
			ಆದರೆ (st)
				odb_ಹರಿವು_close(st);
			ಬಿಡುಗಡೆ(buf);
			ಹಿಂತಿರುಗಿ 0;
		}
		hashwrite(f, ಶೀರ್ಷಿಕೆ, hdrlen);
		hashwrite(f, DELTA(ನಮೂದು)->idx.oid.hash, hashsz);
		hdrlen += hashsz;
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಆದರೆ (ಎಲ್ಲೆ && hdrlen + datalen + hashsz >= ಎಲ್ಲೆ) {
			ಆದರೆ (st)
				odb_ಹರಿವು_close(st);
			ಬಿಡುಗಡೆ(buf);
			ಹಿಂತಿರುಗಿ 0;
		}
		hashwrite(f, ಶೀರ್ಷಿಕೆ, hdrlen);
	}
	ಆದರೆ (st) {
		datalen = write_large_blob_ದತ್ತಾಂಶ(st, f, &ನಮೂದು->idx.oid);
		odb_ಹರಿವು_close(st);
	} ಇಲ್ಲದಿದ್ದರೆ {
		hashwrite(f, buf, datalen);
		ಬಿಡುಗಡೆ(buf);
	}

	ಹಿಂತಿರುಗಿ hdrlen + datalen;
}

/* Return 0 if we will bust the pack-size limit */
ಸ್ಥಿರ off_t write_reuse_object(ರಚನೆ hashfile *f, ರಚನೆ object_ನಮೂದು *ನಮೂದು,
				ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಎಲ್ಲೆ, ಪೂರ್ಣಾಂಕ usable_delta)
{
	ರಚನೆ packed_git *p = ಒಳ_PACK(ನಮೂದು);
	ರಚನೆ pack_ಕಿಟಕಿ *w_curs = ಖಾಲಿ;
	uint32_t pos;
	off_t ಸ್ಥಾನಾಂತರ, cur;
	ಎಣಿಕೆ object_type type = oe_type(ನಮೂದು);
	ಎಣಿಕೆ object_type ಒಳ_pack_type;
	off_t datalen;
	ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ ಶೀರ್ಷಿಕೆ[MAX_PACK_OBJECT_ಶೀರ್ಷಿಕೆ],
		      dheader[MAX_PACK_OBJECT_ಶೀರ್ಷಿಕೆ];
	ಚಿಹ್ನೆರಹಿತ hdrlen;
	ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ hashsz = the_hash_algo->rawsz;
	ಗಾತ್ರ_t ನಮೂದು_ಗಾತ್ರ;

	cur = ನಮೂದು->ಒಳ_pack_ಸ್ಥಾನಾಂತರ;
	ಒಳ_pack_type = unpack_object_ಶೀರ್ಷಿಕೆ(p, &w_curs, &cur, &ನಮೂದು_ಗಾತ್ರ);
	ಆದರೆ (ಒಳ_pack_type < 0)
		die(_("write_reuse_object: unable to parse object header of %s"),
		    oid_ಗಮ್ಯ_hex(&ನಮೂದು->idx.oid));

	ಆದರೆ (DELTA(ನಮೂದು))
		type = (allow_ofs_delta && DELTA(ನಮೂದು)->idx.ಸ್ಥಾನಾಂತರ) ?
			OBJ_OFS_DELTA : OBJ_REF_DELTA;
	hdrlen = encode_ಒಳ_pack_object_ಶೀರ್ಷಿಕೆ(ಶೀರ್ಷಿಕೆ, ಗಾತ್ರ(ಶೀರ್ಷಿಕೆ),
					      type, ನಮೂದು_ಗಾತ್ರ);

	ಸ್ಥಾನಾಂತರ = ನಮೂದು->ಒಳ_pack_ಸ್ಥಾನಾಂತರ;
	ಆದರೆ (ಸ್ಥಾನಾಂತರ_ಗಮ್ಯ_pack_pos(p, ಸ್ಥಾನಾಂತರ, &pos) < 0)
		die(_("write_reuse_object: could not locate %s, expected at "
		      "offset %"PRIuMAX" in pack %s"),
		    oid_ಗಮ್ಯ_hex(&ನಮೂದು->idx.oid), (uintmax_t)ಸ್ಥಾನಾಂತರ,
		    p->pack_name);
	datalen = pack_pos_ಗಮ್ಯ_ಸ್ಥಾನಾಂತರ(p, pos + 1) - ಸ್ಥಾನಾಂತರ;
	ಆದರೆ (!pack_ಗಮ್ಯ_stdout && p->ಸೂಚ್ಯಂಕ_ಆವೃತ್ತಿ > 1 &&
	    check_pack_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ(p, &w_curs, ಸ್ಥಾನಾಂತರ, datalen,
			   pack_pos_ಗಮ್ಯ_ಸೂಚ್ಯಂಕ(p, pos))) {
		error(_("bad packed object CRC for %s"),
		      oid_ಗಮ್ಯ_hex(&ನಮೂದು->idx.oid));
		unuse_pack(&w_curs);
		ಹಿಂತಿರುಗಿ write_no_reuse_object(f, ನಮೂದು, ಎಲ್ಲೆ, usable_delta);
	}

	ಸ್ಥಾನಾಂತರ += ನಮೂದು->ಒಳ_pack_ಶೀರ್ಷಿಕೆ_ಗಾತ್ರ;
	datalen -= ನಮೂದು->ಒಳ_pack_ಶೀರ್ಷಿಕೆ_ಗಾತ್ರ;

	ಆದರೆ (!pack_ಗಮ್ಯ_stdout && p->ಸೂಚ್ಯಂಕ_ಆವೃತ್ತಿ == 1 &&
	    check_pack_inflate(p, &w_curs, ಸ್ಥಾನಾಂತರ, datalen, ನಮೂದು_ಗಾತ್ರ)) {
		error(_("corrupt packed object for %s"),
		      oid_ಗಮ್ಯ_hex(&ನಮೂದು->idx.oid));
		unuse_pack(&w_curs);
		ಹಿಂತಿರುಗಿ write_no_reuse_object(f, ನಮೂದು, ಎಲ್ಲೆ, usable_delta);
	}

	ಆದರೆ (type == OBJ_OFS_DELTA) {
		off_t ofs = ನಮೂದು->idx.ಸ್ಥಾನಾಂತರ - DELTA(ನಮೂದು)->idx.ಸ್ಥಾನಾಂತರ;
		ಚಿಹ್ನೆರಹಿತ pos = ಗಾತ್ರ(dheader) - 1;
		dheader[pos] = ofs & 127;
		ಆಗ (ofs >>= 7)
			dheader[--pos] = 128 | (--ofs & 127);
		ಆದರೆ (ಎಲ್ಲೆ && hdrlen + ಗಾತ್ರ(dheader) - pos + datalen + hashsz >= ಎಲ್ಲೆ) {
			unuse_pack(&w_curs);
			ಹಿಂತಿರುಗಿ 0;
		}
		hashwrite(f, ಶೀರ್ಷಿಕೆ, hdrlen);
		hashwrite(f, dheader + pos, ಗಾತ್ರ(dheader) - pos);
		hdrlen += ಗಾತ್ರ(dheader) - pos;
		reused_delta++;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (type == OBJ_REF_DELTA) {
		ಆದರೆ (ಎಲ್ಲೆ && hdrlen + hashsz + datalen + hashsz >= ಎಲ್ಲೆ) {
			unuse_pack(&w_curs);
			ಹಿಂತಿರುಗಿ 0;
		}
		hashwrite(f, ಶೀರ್ಷಿಕೆ, hdrlen);
		hashwrite(f, DELTA(ನಮೂದು)->idx.oid.hash, hashsz);
		hdrlen += hashsz;
		reused_delta++;
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಆದರೆ (ಎಲ್ಲೆ && hdrlen + datalen + hashsz >= ಎಲ್ಲೆ) {
			unuse_pack(&w_curs);
			ಹಿಂತಿರುಗಿ 0;
		}
		hashwrite(f, ಶೀರ್ಷಿಕೆ, hdrlen);
	}
	copy_pack_ದತ್ತಾಂಶ(f, p, &w_curs, ಸ್ಥಾನಾಂತರ, datalen);
	unuse_pack(&w_curs);
	reused++;
	ಹಿಂತಿರುಗಿ hdrlen + datalen;
}

/* Return 0 if we will bust the pack-size limit */
ಸ್ಥಿರ off_t write_object(ರಚನೆ hashfile *f,
			  ರಚನೆ object_ನಮೂದು *ನಮೂದು,
			  off_t write_ಸ್ಥಾನಾಂತರ)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಎಲ್ಲೆ;
	off_t len;
	ಪೂರ್ಣಾಂಕ usable_delta, ಗಮ್ಯ_reuse;

	ಆದರೆ (!pack_ಗಮ್ಯ_stdout)
		ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_32_begin(f);

	/* apply size limit if limited packsize and not first object */
	ಆದರೆ (!pack_ಗಾತ್ರ_ಎಲ್ಲೆ || !nr_written)
		ಎಲ್ಲೆ = 0;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (pack_ಗಾತ್ರ_ಎಲ್ಲೆ <= write_ಸ್ಥಾನಾಂತರ)
		/*
		 * the earlier object did not fit the limit; avoid
		 * mistaking this with unlimited (i.e. limit = 0).
		 */
		ಎಲ್ಲೆ = 1;
	ಇಲ್ಲದಿದ್ದರೆ
		ಎಲ್ಲೆ = pack_ಗಾತ್ರ_ಎಲ್ಲೆ - write_ಸ್ಥಾನಾಂತರ;

	ಆದರೆ (!DELTA(ನಮೂದು))
		usable_delta = 0;	/* no delta */
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!pack_ಗಾತ್ರ_ಎಲ್ಲೆ)
	       usable_delta = 1;	/* unlimited packfile */
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (DELTA(ನಮೂದು)->idx.ಸ್ಥಾನಾಂತರ == (off_t)-1)
		usable_delta = 0;	/* base was written to another pack */
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (DELTA(ನಮೂದು)->idx.ಸ್ಥಾನಾಂತರ)
		usable_delta = 1;	/* base already exists in this pack */
	ಇಲ್ಲದಿದ್ದರೆ
		usable_delta = 0;	/* base could end up in another pack */

	ಆದರೆ (!reuse_object)
		ಗಮ್ಯ_reuse = 0;	/* explicit */
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!ಒಳ_PACK(ನಮೂದು))
		ಗಮ್ಯ_reuse = 0;	/* can't reuse what we don't have */
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (oe_type(ನಮೂದು) == OBJ_REF_DELTA ||
		 oe_type(ನಮೂದು) == OBJ_OFS_DELTA)
				/* check_object() decided it for us ... */
		ಗಮ್ಯ_reuse = usable_delta;
				/* ... but pack split may override that */
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (oe_type(ನಮೂದು) != ನಮೂದು->ಒಳ_pack_type)
		ಗಮ್ಯ_reuse = 0;	/* pack has delta which is unusable */
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (DELTA(ನಮೂದು))
		ಗಮ್ಯ_reuse = 0;	/* we want to pack afresh */
	ಇಲ್ಲದಿದ್ದರೆ
		ಗಮ್ಯ_reuse = 1;	/* we have it in-pack undeltified,
				 * and we do not need to deltify it.
				 */

	ಆದರೆ (!ಗಮ್ಯ_reuse)
		len = write_no_reuse_object(f, ನಮೂದು, ಎಲ್ಲೆ, usable_delta);
	ಇಲ್ಲದಿದ್ದರೆ
		len = write_reuse_object(f, ನಮೂದು, ಎಲ್ಲೆ, usable_delta);
	ಆದರೆ (!len)
		ಹಿಂತಿರುಗಿ 0;

	ಆದರೆ (usable_delta)
		written_delta++;
	written++;
	ಆದರೆ (!pack_ಗಮ್ಯ_stdout)
		ನಮೂದು->idx.ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_32 = ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_32_ಕೊನೆ(f);
	ಹಿಂತಿರುಗಿ len;
}

ಎಣಿಕೆ write_one_ಸ್ಥಿತಿ {
	WRITE_ONE_SKIP = -1, /* already written */
	WRITE_ONE_BREAK = 0, /* writing this will bust the limit; not written */
	WRITE_ONE_WRITTEN = 1, /*ಸಾಮಾನ್ಯ*/
	WRITE_ONE_RECURSIVE = 2 /* already scheduled to be written */
};

ಸ್ಥಿರ ಎಣಿಕೆ write_one_ಸ್ಥಿತಿ write_one(ರಚನೆ hashfile *f,
				       ರಚನೆ object_ನಮೂದು *e,
				       off_t *ಸ್ಥಾನಾಂತರ)
{
	off_t ಗಾತ್ರ_2;
	ಪೂರ್ಣಾಂಕ recursing;

	/*
	 * we set offset to 1 (which is an impossible value) to mark
	 * the fact that this object is involved in "write its base
	 * first before writing a deltified object" recursion.
	 */
	recursing = (e->idx.ಸ್ಥಾನಾಂತರ == 1);
	ಆದರೆ (recursing) {
		warning(_("recursive delta detected for object %s"),
			oid_ಗಮ್ಯ_hex(&e->idx.oid));
		ಹಿಂತಿರುಗಿ WRITE_ONE_RECURSIVE;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (e->idx.ಸ್ಥಾನಾಂತರ || e->preferred_ತಳ) {
		/* offset is non zero if object is written already. */
		ಹಿಂತಿರುಗಿ WRITE_ONE_SKIP;
	}

	/* if we are deltified, write out base object first. */
	ಆದರೆ (DELTA(e)) {
		e->idx.ಸ್ಥಾನಾಂತರ = 1; /* now recurse */
		ಆಯ್ಕೆ (write_one(f, DELTA(e), ಸ್ಥಾನಾಂತರ)) {
		ಸ್ಥಿತಿ WRITE_ONE_RECURSIVE:
			/* we cannot depend on this one */
			ಗಣ_DELTA(e, ಖಾಲಿ);
			ನಿಲ್ಲಿಸು;
		ಪೂರ್ವನಿಯೋಜಿತ:
			ನಿಲ್ಲಿಸು;
		ಸ್ಥಿತಿ WRITE_ONE_BREAK:
			e->idx.ಸ್ಥಾನಾಂತರ = recursing;
			ಹಿಂತಿರುಗಿ WRITE_ONE_BREAK;
		}
	}

	e->idx.ಸ್ಥಾನಾಂತರ = *ಸ್ಥಾನಾಂತರ;
	ಗಾತ್ರ_2 = write_object(f, e, *ಸ್ಥಾನಾಂತರ);
	ಆದರೆ (!ಗಾತ್ರ_2) {
		e->idx.ಸ್ಥಾನಾಂತರ = recursing;
		ಹಿಂತಿರುಗಿ WRITE_ONE_BREAK;
	}
	written_list[nr_written++] = &e->idx;

	/* make sure off_t is sufficiently large not to wrap */
	ಆದರೆ (signed_ಕೂಡಿಸು_overflows(*ಸ್ಥಾನಾಂತರ, ಗಾತ್ರ_2))
		die(_("pack too large for current definition of off_t"));
	*ಸ್ಥಾನಾಂತರ += ಗಾತ್ರ_2;
	ಹಿಂತಿರುಗಿ WRITE_ONE_WRITTEN;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ mark_tagged(ಸ್ಥಿರಾಂಕ ರಚನೆ reference *ref, ಶೂನ್ಯ *cb_ದತ್ತಾಂಶ ಬಳಸದ)
{
	ರಚನೆ object_id peeled;
	ರಚನೆ object_ನಮೂದು *ನಮೂದು = packlist_ಕಂಡುಹಿಡಿ(&ಗಮ್ಯ_pack, ref->oid);

	ಆದರೆ (ನಮೂದು)
		ನಮೂದು->tagged = 1;
	ಆದರೆ (!reference_get_peeled_oid(the_repository, ref, &peeled)) {
		ನಮೂದು = packlist_ಕಂಡುಹಿಡಿ(&ಗಮ್ಯ_pack, &peeled);
		ಆದರೆ (ನಮೂದು)
			ನಮೂದು->tagged = 1;
	}
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ oe_layer(ರಚನೆ packing_ದತ್ತಾಂಶ *pack,
				     ರಚನೆ object_ನಮೂದು *e)
{
	ಆದರೆ (!pack->layer)
		ಹಿಂತಿರುಗಿ 0;
	ಹಿಂತಿರುಗಿ pack->layer[e - pack->objects];
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ ಕೂಡಿಸು_ಗಮ್ಯ_write_ಕ್ರಮ(ರಚನೆ object_ನಮೂದು **wo,
			       ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ *endp,
			       ರಚನೆ object_ನಮೂದು *e)
{
	ಆದರೆ (e->filled || oe_layer(&ಗಮ್ಯ_pack, e) != write_layer)
		ಹಿಂತಿರುಗಿ;
	wo[(*endp)++] = e;
	e->filled = 1;
}

ಸ್ಥಿರ ಶೂನ್ಯ ಕೂಡಿಸು_descendants_ಗಮ್ಯ_write_ಕ್ರಮ(ರಚನೆ object_ನಮೂದು **wo,
					   ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ *endp,
					   ರಚನೆ object_ನಮೂದು *e)
{
	ಪೂರ್ಣಾಂಕ ಕೂಡಿಸು_ಗಮ್ಯ_ಕ್ರಮ = 1;
	ಆಗ (e) {
		ಆದರೆ (ಕೂಡಿಸು_ಗಮ್ಯ_ಕ್ರಮ) {
			ರಚನೆ object_ನಮೂದು *s;
			/* add this node... */
			ಕೂಡಿಸು_ಗಮ್ಯ_write_ಕ್ರಮ(wo, endp, e);
			/* all its siblings... */
			ಗಾಗಿ (s = DELTA_SIBLING(e); s; s = DELTA_SIBLING(s)) {
				ಕೂಡಿಸು_ಗಮ್ಯ_write_ಕ್ರಮ(wo, endp, s);
			}
		}
		/* drop down a level to add left subtree nodes if possible */
		ಆದರೆ (DELTA_ಮಗು(e)) {
			ಕೂಡಿಸು_ಗಮ್ಯ_ಕ್ರಮ = 1;
			e = DELTA_ಮಗು(e);
		} ಇಲ್ಲದಿದ್ದರೆ {
			ಕೂಡಿಸು_ಗಮ್ಯ_ಕ್ರಮ = 0;
			/* our sibling might have some children, it is next */
			ಆದರೆ (DELTA_SIBLING(e)) {
				e = DELTA_SIBLING(e);
				ಮುಂದುವರಿ;
			}
			/* go back to our parent node */
			e = DELTA(e);
			ಆಗ (e && !DELTA_SIBLING(e)) {
				/* we're on the right side of a subtree, keep
				 * going up until we can go right again */
				e = DELTA(e);
			}
			ಆದರೆ (!e) {
				/* done- we hit our original root node */
				ಹಿಂತಿರುಗಿ;
			}
			/* pass it off to sibling at this level */
			e = DELTA_SIBLING(e);
		}
	};
}

ಸ್ಥಿರ ಶೂನ್ಯ ಕೂಡಿಸು_family_ಗಮ್ಯ_write_ಕ್ರಮ(ರಚನೆ object_ನಮೂದು **wo,
				      ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ *endp,
				      ರಚನೆ object_ನಮೂದು *e)
{
	ರಚನೆ object_ನಮೂದು *ಬೇರು;

	ಗಾಗಿ (ಬೇರು = e; DELTA(ಬೇರು); ಬೇರು = DELTA(ಬೇರು))
		; /* nothing */
	ಕೂಡಿಸು_descendants_ಗಮ್ಯ_write_ಕ್ರಮ(wo, endp, ಬೇರು);
}

ಸ್ಥಿರ ಶೂನ್ಯ compute_layer_ಕ್ರಮ(ರಚನೆ object_ನಮೂದು **wo, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ *wo_ಕೊನೆ)
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ i, ಕೊನೆಯ_untagged;
	ರಚನೆ object_ನಮೂದು *objects = ಗಮ್ಯ_pack.objects;

	ಗಾಗಿ (i = 0; i < ಗಮ್ಯ_pack.nr_objects; i++) {
		ಆದರೆ (objects[i].tagged)
			ನಿಲ್ಲಿಸು;
		ಕೂಡಿಸು_ಗಮ್ಯ_write_ಕ್ರಮ(wo, wo_ಕೊನೆ, &objects[i]);
	}
	ಕೊನೆಯ_untagged = i;

	/*
	 * Then fill all the tagged tips.
	 */
	ಗಾಗಿ (; i < ಗಮ್ಯ_pack.nr_objects; i++) {
		ಆದರೆ (objects[i].tagged)
			ಕೂಡಿಸು_ಗಮ್ಯ_write_ಕ್ರಮ(wo, wo_ಕೊನೆ, &objects[i]);
	}

	/*
	 * And then all remaining commits and tags.
	 */
	ಗಾಗಿ (i = ಕೊನೆಯ_untagged; i < ಗಮ್ಯ_pack.nr_objects; i++) {
		ಆದರೆ (oe_type(&objects[i]) != OBJ_COMMIT &&
		    oe_type(&objects[i]) != OBJ_ಟ್ಯಾಗ್)
			ಮುಂದುವರಿ;
		ಕೂಡಿಸು_ಗಮ್ಯ_write_ಕ್ರಮ(wo, wo_ಕೊನೆ, &objects[i]);
	}

	/*
	 * And then all the trees.
	 */
	ಗಾಗಿ (i = ಕೊನೆಯ_untagged; i < ಗಮ್ಯ_pack.nr_objects; i++) {
		ಆದರೆ (oe_type(&objects[i]) != OBJ_ವೃಕ್ಷ)
			ಮುಂದುವರಿ;
		ಕೂಡಿಸು_ಗಮ್ಯ_write_ಕ್ರಮ(wo, wo_ಕೊನೆ, &objects[i]);
	}

	/*
	 * Finally all the rest in really tight order
	 */
	ಗಾಗಿ (i = ಕೊನೆಯ_untagged; i < ಗಮ್ಯ_pack.nr_objects; i++) {
		ಆದರೆ (!objects[i].filled && oe_layer(&ಗಮ್ಯ_pack, &objects[i]) == write_layer)
			ಕೂಡಿಸು_family_ಗಮ್ಯ_write_ಕ್ರಮ(wo, wo_ಕೊನೆ, &objects[i]);
	}
}

ಸ್ಥಿರ ರಚನೆ object_ನಮೂದು **compute_write_ಕ್ರಮ(ಶೂನ್ಯ)
{
	uint32_t max_layers = 1;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ i, wo_ಕೊನೆ;

	ರಚನೆ object_ನಮೂದು **wo;
	ರಚನೆ object_ನಮೂದು *objects = ಗಮ್ಯ_pack.objects;

	ಗಾಗಿ (i = 0; i < ಗಮ್ಯ_pack.nr_objects; i++) {
		objects[i].tagged = 0;
		objects[i].filled = 0;
		ಗಣ_DELTA_ಮಗು(&objects[i], ಖಾಲಿ);
		ಗಣ_DELTA_SIBLING(&objects[i], ಖಾಲಿ);
	}

	/*
	 * Fully connect delta_child/delta_sibling network.
	 * Make sure delta_sibling is sorted in the original
	 * recency order.
	 */
	ಗಾಗಿ (i = ಗಮ್ಯ_pack.nr_objects; i > 0;) {
		ರಚನೆ object_ನಮೂದು *e = &objects[--i];
		ಆದರೆ (!DELTA(e))
			ಮುಂದುವರಿ;
		/* Mark me as the first child */
		e->delta_sibling_idx = DELTA(e)->delta_ಮಗು_idx;
		ಗಣ_DELTA_ಮಗು(DELTA(e), e);
	}

	/*
	 * Mark objects that are at the tip of tags.
	 */
	refs_for_each_ಟ್ಯಾಗ್_ref(get_main_ref_store(the_repository), mark_tagged,
			      ಖಾಲಿ);

	ಆದರೆ (use_delta_islands) {
		max_layers = compute_pack_layers(&ಗಮ್ಯ_pack);
		free_island_marks();
	}

	ALLOC_ARRAY(wo, ಗಮ್ಯ_pack.nr_objects);
	wo_ಕೊನೆ = 0;

	ಗಾಗಿ (; write_layer < max_layers; ++write_layer)
		compute_layer_ಕ್ರಮ(wo, &wo_ಕೊನೆ);

	ಆದರೆ (wo_ಕೊನೆ != ಗಮ್ಯ_pack.nr_objects)
		die(_("ordered %u objects, expected %"PRIu32),
		    wo_ಕೊನೆ, ಗಮ್ಯ_pack.nr_objects);

	ಹಿಂತಿರುಗಿ wo;
}


/*
 * A reused set of objects. All objects in a chunk have the same
 * relative position in the original packfile and the generated
 * packfile.
 */

ಸ್ಥಿರ ರಚನೆ reused_chunk {
	/* The offset of the first object of this chunk in the original
	 * packfile. */
	off_t original;
	/* The difference for "original" minus the offset of the first object of
	 * this chunk in the generated packfile. */
	off_t difference;
} *reused_chunks;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ reused_chunks_nr;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ reused_chunks_alloc;

ಸ್ಥಿರ ಶೂನ್ಯ ದಾಖಲೆ_reused_object(off_t where, off_t ಸ್ಥಾನಾಂತರ)
{
	ಆದರೆ (reused_chunks_nr && reused_chunks[reused_chunks_nr-1].difference == ಸ್ಥಾನಾಂತರ)
		ಹಿಂತಿರುಗಿ;

	ALLOC_GROW(reused_chunks, reused_chunks_nr + 1,
		   reused_chunks_alloc);
	reused_chunks[reused_chunks_nr].original = where;
	reused_chunks[reused_chunks_nr].difference = ಸ್ಥಾನಾಂತರ;
	reused_chunks_nr++;
}

/*
 * Binary search to find the chunk that "where" is in. Note
 * that we're not looking for an exact match, just the first
 * chunk that contains it (which implicitly ends at the start
 * of the next chunk.
 */
ಸ್ಥಿರ off_t ಕಂಡುಹಿಡಿ_reused_ಸ್ಥಾನಾಂತರ(off_t where)
{
	ಪೂರ್ಣಾಂಕ lo = 0, hi = reused_chunks_nr;
	ಆಗ (lo < hi) {
		ಪೂರ್ಣಾಂಕ mi = lo + ((hi - lo) / 2);
		ಆದರೆ (where == reused_chunks[mi].original)
			ಹಿಂತಿರುಗಿ reused_chunks[mi].difference;
		ಆದರೆ (where < reused_chunks[mi].original)
			hi = mi;
		ಇಲ್ಲದಿದ್ದರೆ
			lo = mi + 1;
	}

	/*
	 * The first chunk starts at zero, so we can't have gone below
	 * there.
	 */
	ಪ್ರತಿಜ್ಞಾ(lo);
	ಹಿಂತಿರುಗಿ reused_chunks[lo-1].difference;
}

ಸ್ಥಿರ ಶೂನ್ಯ write_reused_pack_one(ರಚನೆ packed_git *reuse_packfile,
				  ಗಾತ್ರ_t pos, ರಚನೆ hashfile *ಹೊರಹೋಗು,
				  off_t pack_ಆರಂಭ,
				  ರಚನೆ pack_ಕಿಟಕಿ **w_curs)
{
	off_t ಸ್ಥಾನಾಂತರ, ಮುಂದಿನ, cur;
	ಎಣಿಕೆ object_type type;
	ಗಾತ್ರ_t ಗಾತ್ರ_2;

	ಸ್ಥಾನಾಂತರ = pack_pos_ಗಮ್ಯ_ಸ್ಥಾನಾಂತರ(reuse_packfile, pos);
	ಮುಂದಿನ = pack_pos_ಗಮ್ಯ_ಸ್ಥಾನಾಂತರ(reuse_packfile, pos + 1);

	ದಾಖಲೆ_reused_object(ಸ್ಥಾನಾಂತರ,
			     ಸ್ಥಾನಾಂತರ - (hashfile_ಒಟ್ಟು(ಹೊರಹೋಗು) - pack_ಆರಂಭ));

	cur = ಸ್ಥಾನಾಂತರ;
	type = unpack_object_ಶೀರ್ಷಿಕೆ(reuse_packfile, w_curs, &cur, &ಗಾತ್ರ_2);
	ಪ್ರತಿಜ್ಞಾ(type >= 0);

	ಆದರೆ (type == OBJ_OFS_DELTA) {
		off_t ತಳ_ಸ್ಥಾನಾಂತರ;
		off_t fixup;

		ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ ಶೀರ್ಷಿಕೆ[MAX_PACK_OBJECT_ಶೀರ್ಷಿಕೆ];
		ಚಿಹ್ನೆರಹಿತ len;

		ತಳ_ಸ್ಥಾನಾಂತರ = get_delta_ತಳ(reuse_packfile, w_curs, &cur, type, ಸ್ಥಾನಾಂತರ);
		ಪ್ರತಿಜ್ಞಾ(ತಳ_ಸ್ಥಾನಾಂತರ != 0);

		/* Convert to REF_DELTA if we must... */
		ಆದರೆ (!allow_ofs_delta) {
			uint32_t ತಳ_pos;
			ರಚನೆ object_id ತಳ_oid;

			ಆದರೆ (ಸ್ಥಾನಾಂತರ_ಗಮ್ಯ_pack_pos(reuse_packfile, ತಳ_ಸ್ಥಾನಾಂತರ, &ತಳ_pos) < 0)
				die(_("expected object at offset %"PRIuMAX" "
				      "in pack %s"),
				    (uintmax_t)ತಳ_ಸ್ಥಾನಾಂತರ,
				    reuse_packfile->pack_name);

			nth_packed_object_id(&ತಳ_oid, reuse_packfile,
					     pack_pos_ಗಮ್ಯ_ಸೂಚ್ಯಂಕ(reuse_packfile, ತಳ_pos));

			len = encode_ಒಳ_pack_object_ಶೀರ್ಷಿಕೆ(ಶೀರ್ಷಿಕೆ, ಗಾತ್ರ(ಶೀರ್ಷಿಕೆ),
							   OBJ_REF_DELTA, ಗಾತ್ರ_2);
			hashwrite(ಹೊರಹೋಗು, ಶೀರ್ಷಿಕೆ, len);
			hashwrite(ಹೊರಹೋಗು, ತಳ_oid.hash, the_hash_algo->rawsz);
			copy_pack_ದತ್ತಾಂಶ(ಹೊರಹೋಗು, reuse_packfile, w_curs, cur, ಮುಂದಿನ - cur);
			ಹಿಂತಿರುಗಿ;
		}

		/* Otherwise see if we need to rewrite the offset... */
		fixup = ಕಂಡುಹಿಡಿ_reused_ಸ್ಥಾನಾಂತರ(ಸ್ಥಾನಾಂತರ) -
			ಕಂಡುಹಿಡಿ_reused_ಸ್ಥಾನಾಂತರ(ತಳ_ಸ್ಥಾನಾಂತರ);
		ಆದರೆ (fixup) {
			ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ ofs_ಶೀರ್ಷಿಕೆ[MAX_PACK_OBJECT_ಶೀರ್ಷಿಕೆ];
			ಚಿಹ್ನೆರಹಿತ i, ofs_len;
			off_t ofs = ಸ್ಥಾನಾಂತರ - ತಳ_ಸ್ಥಾನಾಂತರ - fixup;

			len = encode_ಒಳ_pack_object_ಶೀರ್ಷಿಕೆ(ಶೀರ್ಷಿಕೆ, ಗಾತ್ರ(ಶೀರ್ಷಿಕೆ),
							   OBJ_OFS_DELTA, ಗಾತ್ರ_2);

			i = ಗಾತ್ರ(ofs_ಶೀರ್ಷಿಕೆ) - 1;
			ofs_ಶೀರ್ಷಿಕೆ[i] = ofs & 127;
			ಆಗ (ofs >>= 7)
				ofs_ಶೀರ್ಷಿಕೆ[--i] = 128 | (--ofs & 127);

			ofs_len = ಗಾತ್ರ(ofs_ಶೀರ್ಷಿಕೆ) - i;

			hashwrite(ಹೊರಹೋಗು, ಶೀರ್ಷಿಕೆ, len);
			hashwrite(ಹೊರಹೋಗು, ofs_ಶೀರ್ಷಿಕೆ + ಗಾತ್ರ(ofs_ಶೀರ್ಷಿಕೆ) - ofs_len, ofs_len);
			copy_pack_ದತ್ತಾಂಶ(ಹೊರಹೋಗು, reuse_packfile, w_curs, cur, ಮುಂದಿನ - cur);
			ಹಿಂತಿರುಗಿ;
		}

		/* ...otherwise we have no fixup, and can write it verbatim */
	}

	copy_pack_ದತ್ತಾಂಶ(ಹೊರಹೋಗು, reuse_packfile, w_curs, ಸ್ಥಾನಾಂತರ, ಮುಂದಿನ - ಸ್ಥಾನಾಂತರ);
}

ಸ್ಥಿರ ಗಾತ್ರ_t write_reused_pack_verbatim(ರಚನೆ bitmapped_pack *reuse_packfile,
					 ರಚನೆ hashfile *ಹೊರಹೋಗು,
					 ರಚನೆ pack_ಕಿಟಕಿ **w_curs)
{
	ಗಾತ್ರ_t pos = 0;
	ಗಾತ್ರ_t ಕೊನೆ;

	ಆದರೆ (reuse_packfile->bitmap_pos) {
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
		ಹಿಂತಿರುಗಿ reuse_packfile->bitmap_pos / ಬಿಟ್ಟುಗಳು_ಒಳ_EWORD;
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
	ಕೊನೆ = reuse_packfile->bitmap_nr / ಬಿಟ್ಟುಗಳು_ಒಳ_EWORD;
	ಆದರೆ (reuse_packfile_bitmap->ಶಬ್ದ_alloc < ಕೊನೆ)
		BUG("fewer words than expected in reuse_packfile_bitmap");

	ಆಗ (pos < ಕೊನೆ && reuse_packfile_bitmap->ಪದಗಳು[pos] == (eword_t)~0)
		pos++;

	ಆದರೆ (pos) {
		off_t ಗಮ್ಯ_write;

		written = (pos * ಬಿಟ್ಟುಗಳು_ಒಳ_EWORD);
		ಗಮ್ಯ_write = pack_pos_ಗಮ್ಯ_ಸ್ಥಾನಾಂತರ(reuse_packfile->p, written)
			- ಗಾತ್ರ(ರಚನೆ pack_ಶೀರ್ಷಿಕೆ);

		/* We're recording one chunk, not one object. */
		ದಾಖಲೆ_reused_object(ಗಾತ್ರ(ರಚನೆ pack_ಶೀರ್ಷಿಕೆ), 0);
		hashflush(ಹೊರಹೋಗು);
		copy_pack_ದತ್ತಾಂಶ(ಹೊರಹೋಗು, reuse_packfile->p, w_curs,
			ಗಾತ್ರ(ರಚನೆ pack_ಶೀರ್ಷಿಕೆ), ಗಮ್ಯ_write);

		display_ಪ್ರಗತಿ(ಪ್ರಗತಿ_state, written);
	}
	ಹಿಂತಿರುಗಿ pos;
}

ಸ್ಥಿರ ಶೂನ್ಯ write_reused_pack(ರಚನೆ bitmapped_pack *reuse_packfile,
			      ರಚನೆ hashfile *f)
{
	ಗಾತ್ರ_t i = reuse_packfile->bitmap_pos / ಬಿಟ್ಟುಗಳು_ಒಳ_EWORD;
	uint32_t ಸ್ಥಾನಾಂತರ;
	off_t pack_ಆರಂಭ = hashfile_ಒಟ್ಟು(f) - ಗಾತ್ರ(ರಚನೆ pack_ಶೀರ್ಷಿಕೆ);
	ರಚನೆ pack_ಕಿಟಕಿ *w_curs = ಖಾಲಿ;

	ಆದರೆ (allow_ofs_delta)
		i = write_reused_pack_verbatim(reuse_packfile, f, &w_curs);

	ಗಾಗಿ (; i < reuse_packfile_bitmap->ಶಬ್ದ_alloc; ++i) {
		eword_t ಶಬ್ದ = reuse_packfile_bitmap->ಪದಗಳು[i];
		ಗಾತ್ರ_t pos = (i * ಬಿಟ್ಟುಗಳು_ಒಳ_EWORD);

		ಗಾಗಿ (ಸ್ಥಾನಾಂತರ = 0; ಸ್ಥಾನಾಂತರ < ಬಿಟ್ಟುಗಳು_ಒಳ_EWORD; ++ಸ್ಥಾನಾಂತರ) {
			uint32_t pack_pos;
			ಆದರೆ ((ಶಬ್ದ >> ಸ್ಥಾನಾಂತರ) == 0)
				ನಿಲ್ಲಿಸು;

			ಸ್ಥಾನಾಂತರ += ewah_ಬಿಟ್_ctz64(ಶಬ್ದ >> ಸ್ಥಾನಾಂತರ);
			ಆದರೆ (pos + ಸ್ಥಾನಾಂತರ < reuse_packfile->bitmap_pos)
				ಮುಂದುವರಿ;
			ಆದರೆ (pos + ಸ್ಥಾನಾಂತರ >= reuse_packfile->bitmap_pos + reuse_packfile->bitmap_nr)
				ಹೋಗು ಮುಗಿದ;

			ಆದರೆ (reuse_packfile->bitmap_pos) {
				/*
				 * When doing multi-pack reuse on a
				 * non-preferred pack, translate bit positions
				 * from the MIDX pseudo-pack order back to their
				 * pack-relative positions before attempting
				 * reuse.
				 */
				ರಚನೆ multi_pack_ಸೂಚ್ಯಂಕ *m = reuse_packfile->from_midx;
				uint32_t midx_pos;
				off_t pack_ofs;

				ಆದರೆ (!m)
					BUG("non-zero bitmap position without MIDX");

				midx_pos = pack_pos_ಗಮ್ಯ_midx(m, pos + ಸ್ಥಾನಾಂತರ);
				pack_ofs = nth_midxed_ಸ್ಥಾನಾಂತರ(m, midx_pos);

				ಆದರೆ (ಸ್ಥಾನಾಂತರ_ಗಮ್ಯ_pack_pos(reuse_packfile->p,
						       pack_ofs, &pack_pos) < 0)
					BUG("could not find expected object at offset %"PRIuMAX" in pack %s",
					    (uintmax_t)pack_ofs,
					    pack_basename(reuse_packfile->p));
			} ಇಲ್ಲದಿದ್ದರೆ {
				/*
				 * Can use bit positions directly, even for MIDX
				 * bitmaps. See comment in try_partial_reuse()
				 * for why.
				 */
				pack_pos = pos + ಸ್ಥಾನಾಂತರ;
			}

			write_reused_pack_one(reuse_packfile->p, pack_pos, f,
					      pack_ಆರಂಭ, &w_curs);
			display_ಪ್ರಗತಿ(ಪ್ರಗತಿ_state, ++written);
		}
	}

ಮುಗಿದ:
	unuse_pack(&w_curs);
}

ಸ್ಥಿರ ಶೂನ್ಯ write_excluded_by_configs(ಶೂನ್ಯ)
{
	ರಚನೆ oidset_iter iter;
	ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid;

	oidset_iter_init(&excluded_by_config, &iter);
	ಆಗ ((oid = oidset_iter_ಮುಂದಿನ(&iter))) {
		ರಚನೆ configured_exclusion *ex =
			oidmap_get(&configured_exclusions, oid);

		ಆದರೆ (!ex)
			BUG("configured exclusion wasn't configured");
		write_ಒಳ_ತುಂಬಿದ(1, ex->pack_hash_hex, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(ex->pack_hash_hex));
		write_ಒಳ_ತುಂಬಿದ(1, " ", 1);
		write_ಒಳ_ತುಂಬಿದ(1, ex->uri, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(ex->uri));
		write_ಒಳ_ತುಂಬಿದ(1, "\n", 1);
	}
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ no_ವಿಭಜಿಸು_warning[] = N_(
"disabling bitmap writing, packs are split due to pack.packSizeLimit"
);

ಸ್ಥಿರ ಶೂನ್ಯ write_pack_file(ಶೂನ್ಯ)
{
	uint32_t i = 0, j;
	ರಚನೆ hashfile *f;
	off_t ಸ್ಥಾನಾಂತರ;
	uint32_t nr_ಉಳಿದ = nr_ಫಲಿತಾಂಶ;
	ಸಮಯ_t ಕೊನೆಯ_mtime = 0;
	ರಚನೆ object_ನಮೂದು **write_ಕ್ರಮ;
	off_t ಬೈಟುಗಳು_written = 0;

	ಆದರೆ (ಪ್ರಗತಿ > pack_ಗಮ್ಯ_stdout)
		ಪ್ರಗತಿ_state = ಆರಂಭ_ಪ್ರಗತಿ(the_repository,
						_("Writing objects"), nr_ಫಲಿತಾಂಶ);
	ALLOC_ARRAY(written_list, ಗಮ್ಯ_pack.nr_objects);
	write_ಕ್ರಮ = compute_write_ಕ್ರಮ();

	ಮಾಡು {
		ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ hash[GIT_MAX_RAWSZ];
		ಅಕ್ಷರ *pack_tmp_name = ಖಾಲಿ;

		ಆದರೆ (pack_ಗಮ್ಯ_stdout) {
			/*
			 * This command is most often invoked via
			 * git-upload-pack(1), which will typically chunk data
			 * into pktlines. As such, we use the maximum data
			 * length of them as buffer length.
			 *
			 * Note that we need to subtract one though to
			 * accommodate for the sideband byte.
			 */
			ರಚನೆ hashfd_options opts = {
				.ಪ್ರಗತಿ = ಪ್ರಗತಿ_state,
				.ಬಫರ್_len = LARGE_PACKET_ದತ್ತಾಂಶ_MAX - 1,
			};
			f = hashfd_ext(the_repository->hash_algo, 1,
				       "<stdout>", &opts);
		} ಇಲ್ಲದಿದ್ದರೆ {
			f = create_tmp_packfile(the_repository, &pack_tmp_name);
		}

		ಸ್ಥಾನಾಂತರ = write_pack_ಶೀರ್ಷಿಕೆ(f, nr_ಉಳಿದ);

		ಆದರೆ (reuse_packfiles_nr) {
			ಪ್ರತಿಜ್ಞಾ(pack_ಗಮ್ಯ_stdout);
			ಗಾಗಿ (j = 0; j < reuse_packfiles_nr; j++) {
				reused_chunks_nr = 0;
				write_reused_pack(&reuse_packfiles[j], f);
				ಆದರೆ (reused_chunks_nr)
					reuse_packfiles_ಬಳಸಿದ_nr++;
			}
			ಸ್ಥಾನಾಂತರ = hashfile_ಒಟ್ಟು(f);
		}

		nr_written = 0;
		ಗಾಗಿ (; i < ಗಮ್ಯ_pack.nr_objects; i++) {
			ರಚನೆ object_ನಮೂದು *e = write_ಕ್ರಮ[i];
			ಆದರೆ (write_one(f, e, &ಸ್ಥಾನಾಂತರ) == WRITE_ONE_BREAK)
				ನಿಲ್ಲಿಸು;
			display_ಪ್ರಗತಿ(ಪ್ರಗತಿ_state, written);
		}

		ಬೈಟುಗಳು_written += hashfile_ಒಟ್ಟು(f) +
			the_repository->hash_algo->rawsz;
		ಆದರೆ (pack_ಗಮ್ಯ_stdout) {
			/*
			 * We never fsync when writing to stdout since we may
			 * not be writing to an actual pack file. For instance,
			 * the upload-pack code passes a pipe here. Calling
			 * fsync on a pipe results in unnecessary
			 * synchronization with the reader on some platforms.
			 */
			finalize_hashfile(f, hash, FSYNC_COMPONENT_NONE,
					  CSUM_HASH_ಒಳ_ಹರಿವು | CSUM_CLOSE);
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (nr_written == nr_ಉಳಿದ) {
			finalize_hashfile(f, hash, FSYNC_COMPONENT_PACK,
					  CSUM_HASH_ಒಳ_ಹರಿವು | CSUM_FSYNC | CSUM_CLOSE);
		} ಇಲ್ಲದಿದ್ದರೆ {
			/*
			 * If we wrote the wrong number of entries in the
			 * header, rewrite it like in fast-import.
			 */

			ಪೂರ್ಣಾಂಕ fd = finalize_hashfile(f, hash, FSYNC_COMPONENT_PACK, 0);
			fixup_pack_ಶೀರ್ಷಿಕೆ_footer(the_hash_algo, fd, hash,
						 pack_tmp_name, nr_written,
						 hash, ಸ್ಥಾನಾಂತರ);
			close(fd);
			ಆದರೆ (write_bitmap_ಸೂಚ್ಯಂಕ) {
				ಆದರೆ (write_bitmap_ಸೂಚ್ಯಂಕ != WRITE_BITMAP_QUIET)
					warning(_(no_ವಿಭಜಿಸು_warning));
				write_bitmap_ಸೂಚ್ಯಂಕ = 0;
			}
		}

		ಆದರೆ (!pack_ಗಮ್ಯ_stdout) {
			ರಚನೆ stat st;
			ರಚನೆ strbuf tmpname = STRBUF_INIT;
			ರಚನೆ bitmap_writer bitmap_writer;
			ಅಕ್ಷರ *idx_tmp_name = ಖಾಲಿ;

			/*
			 * Packs are runtime accessed in their mtime
			 * order since newer packs are more likely to contain
			 * younger objects.  So if we are creating multiple
			 * packs then we should modify the mtime of later ones
			 * to preserve this property.
			 */
			ಆದರೆ (stat(pack_tmp_name, &st) < 0) {
				warning_errno(_("failed to stat %s"), pack_tmp_name);
			} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!ಕೊನೆಯ_mtime) {
				ಕೊನೆಯ_mtime = st.st_mtime;
			} ಇಲ್ಲದಿದ್ದರೆ {
				ರಚನೆ utimbuf utb;
				utb.actime = st.st_atime;
				utb.modtime = --ಕೊನೆಯ_mtime;
				ಆದರೆ (utime(pack_tmp_name, &utb) < 0)
					warning_errno(_("failed utime() on %s"), pack_tmp_name);
			}

			strbuf_addf(&tmpname, "%s-%s.", ತಳ_name,
				    hash_ಗಮ್ಯ_hex(hash));

			ಆದರೆ (write_bitmap_ಸೂಚ್ಯಂಕ) {
				bitmap_writer_init(&bitmap_writer,
						   the_repository, &ಗಮ್ಯ_pack,
						   ಖಾಲಿ);
				bitmap_writer_ಗಣ_checksum(&bitmap_writer, hash);
				bitmap_writer_build_type_ಸೂಚ್ಯಂಕ(&bitmap_writer,
							       written_list);
			}

			ಆದರೆ (cruft)
				pack_idx_opts.flags |= WRITE_MTIMES;

			stage_tmp_packfiles(the_repository, &tmpname,
					    pack_tmp_name, written_list,
					    nr_written, &ಗಮ್ಯ_pack,
					    &pack_idx_opts, hash,
					    &idx_tmp_name);

			ಆದರೆ (write_bitmap_ಸೂಚ್ಯಂಕ) {
				ಗಾತ್ರ_t tmpname_len = tmpname.len;

				strbuf_addstr(&tmpname, "bitmap");
				stop_ಪ್ರಗತಿ(&ಪ್ರಗತಿ_state);

				bitmap_writer_show_ಪ್ರಗತಿ(&bitmap_writer,
							    ಪ್ರಗತಿ);
				bitmap_writer_select_commits(&bitmap_writer,
							     ಸೂಚ್ಯಂಕಿತ_commits,
							     ಸೂಚ್ಯಂಕಿತ_commits_nr);
				ಆದರೆ (bitmap_writer_build(&bitmap_writer) < 0)
					die(_("failed to write bitmap index"));
				bitmap_writer_ಮುಕ್ತಾಯ(&bitmap_writer,
						     written_list,
						     tmpname.buf, write_bitmap_options);
				bitmap_writer_free(&bitmap_writer);
				write_bitmap_ಸೂಚ್ಯಂಕ = 0;
				strbuf_setlen(&tmpname, tmpname_len);
			}

			rename_tmp_packfile_idx(the_repository, &tmpname, &idx_tmp_name);

			ಬಿಡುಗಡೆ(idx_tmp_name);
			strbuf_release(&tmpname);
			ಬಿಡುಗಡೆ(pack_tmp_name);
			puts(hash_ಗಮ್ಯ_hex(hash));
		}

		/* mark written objects as written to previous pack */
		ಗಾಗಿ (j = 0; j < nr_written; j++) {
			written_list[j]->ಸ್ಥಾನಾಂತರ = (off_t)-1;
		}
		nr_ಉಳಿದ -= nr_written;
	} ಆಗ (nr_ಉಳಿದ && i < ಗಮ್ಯ_pack.nr_objects);

	ಬಿಡುಗಡೆ(written_list);
	ಬಿಡುಗಡೆ(write_ಕ್ರಮ);
	stop_ಪ್ರಗತಿ(&ಪ್ರಗತಿ_state);
	ಆದರೆ (written != nr_ಫಲಿತಾಂಶ)
		die(_("wrote %"PRIu32" objects while expecting %"PRIu32),
		    written, nr_ಫಲಿತಾಂಶ);
	trace2_ದತ್ತಾಂಶ_intmax("pack-objects", the_repository,
			   "write_pack_file/wrote", nr_ಫಲಿತಾಂಶ);
	trace2_ದತ್ತಾಂಶ_intmax("pack-objects", the_repository,
			   "write_pack_file/wrote_bytes", ಬೈಟುಗಳು_written);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ no_try_delta(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ)
{
	ಸ್ಥಿರ ರಚನೆ attr_check *check;

	ಆದರೆ (!check)
		check = attr_check_initl("delta", ಖಾಲಿ);
	git_check_attr(the_repository->ಸೂಚ್ಯಂಕ, ಮಾರ್ಗ, check);
	ಆದರೆ (ATTR_FALSE(check->ವಸ್ತುಗಳು[0].ಮೌಲ್ಯ))
		ಹಿಂತಿರುಗಿ 1;
	ಹಿಂತಿರುಗಿ 0;
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
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ have_duplicate_ನಮೂದು(ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid,
				ಪೂರ್ಣಾಂಕ exclude)
{
	ರಚನೆ object_ನಮೂದು *ನಮೂದು;

	ಆದರೆ (reuse_packfile_bitmap &&
	    bitmap_walk_ಒಳಗೊಂಡಿದೆ(bitmap_git, reuse_packfile_bitmap, oid))
		ಹಿಂತಿರುಗಿ 1;

	ನಮೂದು = packlist_ಕಂಡುಹಿಡಿ(&ಗಮ್ಯ_pack, oid);
	ಆದರೆ (!ನಮೂದು)
		ಹಿಂತಿರುಗಿ 0;

	ಆದರೆ (exclude) {
		ಆದರೆ (!ನಮೂದು->preferred_ತಳ)
			nr_ಫಲಿತಾಂಶ--;
		ನಮೂದು->preferred_ತಳ = 1;
	}

	ಹಿಂತಿರುಗಿ 1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ want_cruft_object_mtime(ರಚನೆ repository *r,
				   ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid,
				   ಚಿಹ್ನೆರಹಿತ flags, uint32_t mtime)
{
	ರಚನೆ odb_ಮೂಲ_files *files = odb_ಮೂಲ_files_downcast(r->objects->ಮೂಲ);

	ಗಾಗಿ (ರಚನೆ odb_files_dir *dir = files->dirs; dir; dir = dir->ಮುಂದಿನ) {
		ರಚನೆ packed_git **cache = packfile_store_get_kept_pack_cache(dir->packed, flags);

		ಗಾಗಿ (; *cache; cache++) {
			ರಚನೆ packed_git *p = *cache;
			off_t ofs;
			uint32_t candidate_mtime;

			ofs = ಕಂಡುಹಿಡಿ_pack_ನಮೂದು_one(oid, p);
			ಆದರೆ (!ofs)
				ಮುಂದುವರಿ;

			/*
			 * We have a copy of the object 'oid' in a non-cruft
			 * pack. We can avoid packing an additional copy
			 * regardless of what the existing copy's mtime is since
			 * it is outside of a cruft pack.
			 */
			ಆದರೆ (!p->is_cruft)
				ಹಿಂತಿರುಗಿ 0;

			/*
			 * If we have a copy of the object 'oid' in a cruft
			 * pack, then either read the cruft pack's mtime for
			 * that object, or, if that can't be loaded, assume the
			 * pack's mtime itself.
			 */
			ಆದರೆ (!ತುಂಬು_pack_mtimes(p)) {
				uint32_t pos;
				ಆದರೆ (ಸ್ಥಾನಾಂತರ_ಗಮ್ಯ_pack_pos(p, ofs, &pos) < 0)
					ಮುಂದುವರಿ;
				candidate_mtime = nth_packed_mtime(p, pos);
			} ಇಲ್ಲದಿದ್ದರೆ {
				candidate_mtime = p->mtime;
			}

			/*
			 * We have a surviving copy of the object in a cruft
			 * pack whose mtime is greater than or equal to the one
			 * we are considering. We can thus avoid packing an
			 * additional copy of that object.
			 */
			ಆದರೆ (mtime <= candidate_mtime)
				ಹಿಂತಿರುಗಿ 0;
		}
	}

	ಹಿಂತಿರುಗಿ -1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ want_ಸಿಕ್ಕಿತು_object(ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid, ಪೂರ್ಣಾಂಕ exclude,
			     ರಚನೆ packed_git *p, uint32_t mtime)
{
	ಆದರೆ (exclude)
		ಹಿಂತಿರುಗಿ 1;
	ಆದರೆ (incremental)
		ಹಿಂತಿರುಗಿ 0;

	ಆದರೆ (!is_pack_valid(p))
		ಹಿಂತಿರುಗಿ -1;

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
	ಆದರೆ (local && !p->pack_local)
		ಹಿಂತಿರುಗಿ 0;

	/*
	 * Then handle .keep first, as we have a fast(er) path there.
	 */
	ಆದರೆ (ignore_packed_keep_on_disk || ignore_packed_keep_ಒಳ_core ||
	    ignore_packed_keep_ಒಳ_core_open) {
		/*
		 * Set the flags for the kept-pack cache to be the ones we want
		 * to ignore.
		 *
		 * That is, if we are ignoring objects in on-disk keep packs,
		 * then we want to search through the on-disk keep and ignore
		 * the in-core ones.
		 */
		ಚಿಹ್ನೆರಹಿತ flags = 0;
		ಆದರೆ (ignore_packed_keep_on_disk)
			flags |= KEPT_PACK_ON_DISK;
		ಆದರೆ (ignore_packed_keep_ಒಳ_core)
			flags |= KEPT_PACK_ಒಳ_CORE;
		ಆದರೆ (ignore_packed_keep_ಒಳ_core_open)
			flags |= KEPT_PACK_ಒಳ_CORE_OPEN;

		/*
		 * If the object is in a pack that we want to ignore, *and* we
		 * don't have any cruft packs that are being retained, we can
		 * abort quickly.
		 */
		ಆದರೆ (!ignore_packed_keep_ಒಳ_core_has_cruft) {
			ಆದರೆ (ignore_packed_keep_on_disk && p->pack_keep)
				ಹಿಂತಿರುಗಿ 0;
			ಆದರೆ (ignore_packed_keep_ಒಳ_core && p->pack_keep_ಒಳ_core)
				ಹಿಂತಿರುಗಿ 0;
			ಆದರೆ (ignore_packed_keep_ಒಳ_core_open && p->pack_keep_ಒಳ_core_open)
				ಹಿಂತಿರುಗಿ 0;
			ಆದರೆ (has_object_kept_pack(p->repo, oid, flags))
				ಹಿಂತಿರುಗಿ 0;
		} ಇಲ್ಲದಿದ್ದರೆ {
			/*
			 * But if there is at least one cruft pack which
			 * is being kept, we only want to include the
			 * provided object if it has a strictly greater
			 * mtime than any existing cruft copy.
			 */
			ಆದರೆ (!want_cruft_object_mtime(p->repo, oid, flags,
						     mtime))
				ಹಿಂತಿರುಗಿ 0;
		}
	}

	/*
	 * At this point we know definitively that either we don't care about
	 * keep-packs, or the object is not in one. Keep checking other
	 * conditions...
	 */
	ಆದರೆ (!local || !have_non_local_packs)
		ಹಿಂತಿರುಗಿ 1;

	/* we don't know yet; keep looking for more packs */
	ಹಿಂತಿರುಗಿ -1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ want_object_ಒಳ_pack_one(ರಚನೆ packed_git *p,
				   ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid,
				   ಪೂರ್ಣಾಂಕ exclude,
				   ರಚನೆ packed_git **ಸಿಕ್ಕಿತು_pack,
				   off_t *ಸಿಕ್ಕಿತು_ಸ್ಥಾನಾಂತರ,
				   uint32_t ಸಿಕ್ಕಿತು_mtime)
{
	off_t ಸ್ಥಾನಾಂತರ;

	ಆದರೆ (p == *ಸಿಕ್ಕಿತು_pack)
		ಸ್ಥಾನಾಂತರ = *ಸಿಕ್ಕಿತು_ಸ್ಥಾನಾಂತರ;
	ಇಲ್ಲದಿದ್ದರೆ
		ಸ್ಥಾನಾಂತರ = ಕಂಡುಹಿಡಿ_pack_ನಮೂದು_one(oid, p);

	ಆದರೆ (ಸ್ಥಾನಾಂತರ) {
		ಆದರೆ (!*ಸಿಕ್ಕಿತು_pack) {
			ಆದರೆ (!is_pack_valid(p))
				ಹಿಂತಿರುಗಿ -1;
			*ಸಿಕ್ಕಿತು_ಸ್ಥಾನಾಂತರ = ಸ್ಥಾನಾಂತರ;
			*ಸಿಕ್ಕಿತು_pack = p;
		}
		ಹಿಂತಿರುಗಿ want_ಸಿಕ್ಕಿತು_object(oid, exclude, p, ಸಿಕ್ಕಿತು_mtime);
	}
	ಹಿಂತಿರುಗಿ -1;
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
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ want_object_ಒಳ_pack_mtime(ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid,
				     ಪೂರ್ಣಾಂಕ exclude,
				     ರಚನೆ packed_git **ಸಿಕ್ಕಿತು_pack,
				     off_t *ಸಿಕ್ಕಿತು_ಸ್ಥಾನಾಂತರ,
				     uint32_t ಸಿಕ್ಕಿತು_mtime)
{
	ರಚನೆ odb_ಮೂಲ_files *files =
		odb_ಮೂಲ_files_downcast(the_repository->objects->ಮೂಲ);
	ಪೂರ್ಣಾಂಕ want;
	ರಚನೆ packfile_list_ನಮೂದು *e;

	ಆದರೆ (!exclude && local) {
		/*
		 * Note that we start iterating at `dirs->next` so that we
		 * skip the local object directory.
		 */
		ಗಾಗಿ (ರಚನೆ odb_files_dir *dir = files->dirs->ಮುಂದಿನ; dir; dir = dir->ಮುಂದಿನ)
			ಆದರೆ (!odb_ಮೂಲ_read_object_info(&dir->loose->ತಳ, oid, ಖಾಲಿ, 0, ಖಾಲಿ))
				ಹಿಂತಿರುಗಿ 0;
	}

	/*
	 * If we already know the pack object lives in, start checks from that
	 * pack - in the usual case when neither --local was given nor .keep files
	 * are present we will determine the answer right now.
	 */
	ಆದರೆ (*ಸಿಕ್ಕಿತು_pack) {
		want = want_ಸಿಕ್ಕಿತು_object(oid, exclude, *ಸಿಕ್ಕಿತು_pack,
					 ಸಿಕ್ಕಿತು_mtime);
		ಆದರೆ (want != -1)
			ಹಿಂತಿರುಗಿ want;

		*ಸಿಕ್ಕಿತು_pack = ಖಾಲಿ;
		*ಸಿಕ್ಕಿತು_ಸ್ಥಾನಾಂತರ = 0;
	}

	ಗಾಗಿ (ರಚನೆ odb_files_dir *dir = files->dirs; dir; dir = dir->ಮುಂದಿನ) {
		ರಚನೆ multi_pack_ಸೂಚ್ಯಂಕ *m = get_multi_pack_ಸೂಚ್ಯಂಕ(dir->packed);
		ರಚನೆ pack_ನಮೂದು e;

		ಆದರೆ (m && midx_fill_ನಮೂದು(m, oid, &e, ಖಾಲಿ) == MIDX_FILL_ದೊರಕಿತು) {
			want = want_object_ಒಳ_pack_one(e.p, oid, exclude, ಸಿಕ್ಕಿತು_pack, ಸಿಕ್ಕಿತು_ಸ್ಥಾನಾಂತರ, ಸಿಕ್ಕಿತು_mtime);
			ಆದರೆ (want != -1)
				ಹಿಂತಿರುಗಿ want;
		}
	}

	ಗಾಗಿ (ರಚನೆ odb_files_dir *dir = files->dirs; dir; dir = dir->ಮುಂದಿನ) {
		ಗಾಗಿ (e = dir->packed->packs.ತಲೆ; e; e = e->ಮುಂದಿನ) {
			ರಚನೆ packed_git *p = e->pack;
			want = want_object_ಒಳ_pack_one(p, oid, exclude, ಸಿಕ್ಕಿತು_pack, ಸಿಕ್ಕಿತು_ಸ್ಥಾನಾಂತರ, ಸಿಕ್ಕಿತು_mtime);
			ಆದರೆ (!exclude && want > 0)
				packfile_list_prepend(&dir->packed->packs, p);
			ಆದರೆ (want != -1)
				ಹಿಂತಿರುಗಿ want;
		}
	}

	ಆದರೆ (uri_protocols.nr) {
		ರಚನೆ configured_exclusion *ex =
			oidmap_get(&configured_exclusions, oid);
		ಪೂರ್ಣಾಂಕ i;
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *p;

		ಆದರೆ (ex) {
			ಗಾಗಿ (i = 0; i < uri_protocols.nr; i++) {
				ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(ex->uri,
						uri_protocols.ವಸ್ತುಗಳು[i].string,
						&p) &&
				    *p == ':') {
					oidset_ಸೇರಿಸು(&excluded_by_config, oid);
					ಹಿಂತಿರುಗಿ 0;
				}
			}
		}
	}

	ಹಿಂತಿರುಗಿ 1;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ want_object_ಒಳ_pack(ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid,
				      ಪೂರ್ಣಾಂಕ exclude,
				      ರಚನೆ packed_git **ಸಿಕ್ಕಿತು_pack,
				      off_t *ಸಿಕ್ಕಿತು_ಸ್ಥಾನಾಂತರ)
{
	ಹಿಂತಿರುಗಿ want_object_ಒಳ_pack_mtime(oid, exclude, ಸಿಕ್ಕಿತು_pack, ಸಿಕ್ಕಿತು_ಸ್ಥಾನಾಂತರ,
					 0);
}

ಸ್ಥಿರ ರಚನೆ object_ನಮೂದು *create_object_ನಮೂದು(ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid,
						ಎಣಿಕೆ object_type type,
						uint32_t hash,
						ಪೂರ್ಣಾಂಕ exclude,
						ಪೂರ್ಣಾಂಕ no_try_delta,
						ರಚನೆ packed_git *ಸಿಕ್ಕಿತು_pack,
						off_t ಸಿಕ್ಕಿತು_ಸ್ಥಾನಾಂತರ)
{
	ರಚನೆ object_ನಮೂದು *ನಮೂದು;

	ನಮೂದು = packlist_alloc(&ಗಮ್ಯ_pack, oid);
	ನಮೂದು->hash = hash;
	oe_ಗಣ_type(ನಮೂದು, type);
	ಆದರೆ (exclude)
		ನಮೂದು->preferred_ತಳ = 1;
	ಇಲ್ಲದಿದ್ದರೆ
		nr_ಫಲಿತಾಂಶ++;
	ಆದರೆ (ಸಿಕ್ಕಿತು_pack) {
		oe_ಗಣ_ಒಳ_pack(&ಗಮ್ಯ_pack, ನಮೂದು, ಸಿಕ್ಕಿತು_pack);
		ನಮೂದು->ಒಳ_pack_ಸ್ಥಾನಾಂತರ = ಸಿಕ್ಕಿತು_ಸ್ಥಾನಾಂತರ;
	}

	ನಮೂದು->no_try_delta = no_try_delta;

	ಹಿಂತಿರುಗಿ ನಮೂದು;
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ no_ಆವರಣ_warning[] = N_(
"disabling bitmap writing, as some objects are not being packed"
);

ಸ್ಥಿರ ಶೂನ್ಯ ಕೂಡಿಸು_object_ನಮೂದು(ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid, ಎಣಿಕೆ object_type type,
			     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಪೂರ್ಣಾಂಕ exclude)
{
	ರಚನೆ packed_git *ಸಿಕ್ಕಿತು_pack = ಖಾಲಿ;
	off_t ಸಿಕ್ಕಿತು_ಸ್ಥಾನಾಂತರ = 0;

	display_ಪ್ರಗತಿ(ಪ್ರಗತಿ_state, ++nr_ನೋಡಿದ);

	ಆದರೆ (have_duplicate_ನಮೂದು(oid, exclude))
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (!want_object_ಒಳ_pack(oid, exclude, &ಸಿಕ್ಕಿತು_pack, &ಸಿಕ್ಕಿತು_ಸ್ಥಾನಾಂತರ)) {
		/* The pack is missing an object, so it will not have closure */
		ಆದರೆ (write_bitmap_ಸೂಚ್ಯಂಕ) {
			ಆದರೆ (write_bitmap_ಸೂಚ್ಯಂಕ != WRITE_BITMAP_QUIET)
				warning(_(no_ಆವರಣ_warning));
			write_bitmap_ಸೂಚ್ಯಂಕ = 0;
		}
		ಹಿಂತಿರುಗಿ;
	}

	create_object_ನಮೂದು(oid, type, pack_name_hash_fn(name),
			    exclude, name && no_try_delta(name),
			    ಸಿಕ್ಕಿತು_pack, ಸಿಕ್ಕಿತು_ಸ್ಥಾನಾಂತರ);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಕೂಡಿಸು_object_ನಮೂದು_from_bitmap(ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid,
					ಎಣಿಕೆ object_type type,
					ಪೂರ್ಣಾಂಕ flags ಬಳಸದ, uint32_t name_hash,
					ರಚನೆ packed_git *pack, off_t ಸ್ಥಾನಾಂತರ,
					ಶೂನ್ಯ *payload ಬಳಸದ)
{
	display_ಪ್ರಗತಿ(ಪ್ರಗತಿ_state, ++nr_ನೋಡಿದ);

	ಆದರೆ (have_duplicate_ನಮೂದು(oid, 0))
		ಹಿಂತಿರುಗಿ 0;

	ಆದರೆ (!want_object_ಒಳ_pack(oid, 0, &pack, &ಸ್ಥಾನಾಂತರ))
		ಹಿಂತಿರುಗಿ 0;

	create_object_ನಮೂದು(oid, type, name_hash, 0, 0, pack, ಸ್ಥಾನಾಂತರ);
	ಹಿಂತಿರುಗಿ 0;
}

ರಚನೆ pbase_ವೃಕ್ಷ_cache {
	ರಚನೆ object_id oid;
	ಪೂರ್ಣಾಂಕ ref;
	ಪೂರ್ಣಾಂಕ temporary;
	ಶೂನ್ಯ *ವೃಕ್ಷ_ದತ್ತಾಂಶ;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ವೃಕ್ಷ_ಗಾತ್ರ;
};

ಸ್ಥಿರ ರಚನೆ pbase_ವೃಕ್ಷ_cache *(pbase_ವೃಕ್ಷ_cache[256]);
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ pbase_ವೃಕ್ಷ_cache_ix(ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid)
{
	ಹಿಂತಿರುಗಿ oid->hash[0] % ARRAY_ಗಾತ್ರ(pbase_ವೃಕ್ಷ_cache);
}
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ pbase_ವೃಕ್ಷ_cache_ix_incr(ಪೂರ್ಣಾಂಕ ix)
{
	ಹಿಂತಿರುಗಿ (ix+1) % ARRAY_ಗಾತ್ರ(pbase_ವೃಕ್ಷ_cache);
}

ಸ್ಥಿರ ರಚನೆ pbase_ವೃಕ್ಷ {
	ರಚನೆ pbase_ವೃಕ್ಷ *ಮುಂದಿನ;
	/* This is a phony "cache" entry; we are not
	 * going to evict it or find it through _get()
	 * mechanism -- this is for the toplevel node that
	 * would almost always change with any commit.
	 */
	ರಚನೆ pbase_ವೃಕ್ಷ_cache pcache;
} *pbase_ವೃಕ್ಷ;

ಸ್ಥಿರ ರಚನೆ pbase_ವೃಕ್ಷ_cache *pbase_ವೃಕ್ಷ_get(ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid)
{
	ರಚನೆ pbase_ವೃಕ್ಷ_cache *ent, *nent;
	ಶೂನ್ಯ *ದತ್ತಾಂಶ;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಗಾತ್ರ_2;
	ಗಾತ್ರ_t ಗಾತ್ರ_st = 0;
	ಎಣಿಕೆ object_type type;
	ಪೂರ್ಣಾಂಕ neigh;
	ಪೂರ್ಣಾಂಕ my_ix = pbase_ವೃಕ್ಷ_cache_ix(oid);
	ಪೂರ್ಣಾಂಕ ಲಭ್ಯ_ix = -1;

	/* pbase-tree-cache acts as a limited hashtable.
	 * your object will be found at your index or within a few
	 * slots after that slot if it is cached.
	 */
	ಗಾಗಿ (neigh = 0; neigh < 8; neigh++) {
		ent = pbase_ವೃಕ್ಷ_cache[my_ix];
		ಆದರೆ (ent && oideq(&ent->oid, oid)) {
			ent->ref++;
			ಹಿಂತಿರುಗಿ ent;
		}
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (((ಲಭ್ಯ_ix < 0) && (!ent || !ent->ref)) ||
			 ((0 <= ಲಭ್ಯ_ix) &&
			  (!ent && pbase_ವೃಕ್ಷ_cache[ಲಭ್ಯ_ix])))
			ಲಭ್ಯ_ix = my_ix;
		ಆದರೆ (!ent)
			ನಿಲ್ಲಿಸು;
		my_ix = pbase_ವೃಕ್ಷ_cache_ix_incr(my_ix);
	}

	/* Did not find one.  Either we got a bogus request or
	 * we need to read and perhaps cache.
	 */
	ದತ್ತಾಂಶ = odb_read_object(the_repository->objects, oid, &type, &ಗಾತ್ರ_st);
	ಗಾತ್ರ_2 = cast_ಗಾತ್ರ_t_ಗಮ್ಯ_ulong(ಗಾತ್ರ_st);
	ಆದರೆ (!ದತ್ತಾಂಶ)
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;
	ಆದರೆ (type != OBJ_ವೃಕ್ಷ) {
		ಬಿಡುಗಡೆ(ದತ್ತಾಂಶ);
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;
	}

	/* We need to either cache or return a throwaway copy */

	ಆದರೆ (ಲಭ್ಯ_ix < 0)
		ent = ಖಾಲಿ;
	ಇಲ್ಲದಿದ್ದರೆ {
		ent = pbase_ವೃಕ್ಷ_cache[ಲಭ್ಯ_ix];
		my_ix = ಲಭ್ಯ_ix;
	}

	ಆದರೆ (!ent) {
		nent = xmalloc(ಗಾತ್ರ(*nent));
		nent->temporary = (ಲಭ್ಯ_ix < 0);
	}
	ಇಲ್ಲದಿದ್ದರೆ {
		/* evict and reuse */
		ಬಿಡುಗಡೆ(ent->ವೃಕ್ಷ_ದತ್ತಾಂಶ);
		nent = ent;
	}
	oidcpy(&nent->oid, oid);
	nent->ವೃಕ್ಷ_ದತ್ತಾಂಶ = ದತ್ತಾಂಶ;
	nent->ವೃಕ್ಷ_ಗಾತ್ರ = ಗಾತ್ರ_2;
	nent->ref = 1;
	ಆದರೆ (!nent->temporary)
		pbase_ವೃಕ್ಷ_cache[my_ix] = nent;
	ಹಿಂತಿರುಗಿ nent;
}

ಸ್ಥಿರ ಶೂನ್ಯ pbase_ವೃಕ್ಷ_put(ರಚನೆ pbase_ವೃಕ್ಷ_cache *cache)
{
	ಆದರೆ (!cache->temporary) {
		cache->ref--;
		ಹಿಂತಿರುಗಿ;
	}
	ಬಿಡುಗಡೆ(cache->ವೃಕ್ಷ_ದತ್ತಾಂಶ);
	ಬಿಡುಗಡೆ(cache);
}

ಸ್ಥಿರ ಗಾತ್ರ_t name_cmp_len(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name)
{
	ಹಿಂತಿರುಗಿ strcspn(name, "\n/");
}

ಸ್ಥಿರ ಶೂನ್ಯ ಕೂಡಿಸು_pbase_object(ರಚನೆ ವೃಕ್ಷ_desc *ವೃಕ್ಷ,
			     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name,
			     ಗಾತ್ರ_t cmplen,
			     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fullname)
{
	ರಚನೆ name_ನಮೂದು ನಮೂದು;
	ಪೂರ್ಣಾಂಕ cmp;

	ಆಗ (ವೃಕ್ಷ_ನಮೂದು(ವೃಕ್ಷ,&ನಮೂದು)) {
		ಆದರೆ (S_ISGITLINK(ನಮೂದು.ಬಹುಲಕ))
			ಮುಂದುವರಿ;
		cmp = ವೃಕ್ಷ_ನಮೂದು_len(&ನಮೂದು) != cmplen ? 1 :
		      memcmp(name, ನಮೂದು.ಮಾರ್ಗ, cmplen);
		ಆದರೆ (cmp > 0)
			ಮುಂದುವರಿ;
		ಆದರೆ (cmp < 0)
			ಹಿಂತಿರುಗಿ;
		ಆದರೆ (name[cmplen] != '/') {
			ಕೂಡಿಸು_object_ನಮೂದು(&ನಮೂದು.oid,
					 object_type(ನಮೂದು.ಬಹುಲಕ),
					 fullname, 1);
			ಹಿಂತಿರುಗಿ;
		}
		ಆದರೆ (S_ISDIR(ನಮೂದು.ಬಹುಲಕ)) {
			ರಚನೆ ವೃಕ್ಷ_desc sub;
			ರಚನೆ pbase_ವೃಕ್ಷ_cache *ವೃಕ್ಷ;
			ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೆಳಕ್ಕೆ = name+cmplen+1;
			ಗಾತ್ರ_t downlen = name_cmp_len(ಕೆಳಕ್ಕೆ);

			ವೃಕ್ಷ = pbase_ವೃಕ್ಷ_get(&ನಮೂದು.oid);
			ಆದರೆ (!ವೃಕ್ಷ)
				ಹಿಂತಿರುಗಿ;
			init_ವೃಕ್ಷ_desc(&sub, &ವೃಕ್ಷ->oid,
				       ವೃಕ್ಷ->ವೃಕ್ಷ_ದತ್ತಾಂಶ, ವೃಕ್ಷ->ವೃಕ್ಷ_ಗಾತ್ರ);

			ಕೂಡಿಸು_pbase_object(&sub, ಕೆಳಕ್ಕೆ, downlen, fullname);
			pbase_ವೃಕ್ಷ_put(ವೃಕ್ಷ);
		}
	}
}

ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ *ಮುಗಿದ_pbase_paths;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಮುಗಿದ_pbase_paths_ಅಂಕೆ;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಮುಗಿದ_pbase_paths_alloc;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಮುಗಿದ_pbase_ಮಾರ್ಗ_pos(ಚಿಹ್ನೆರಹಿತ hash)
{
	ಪೂರ್ಣಾಂಕ lo = 0;
	ಪೂರ್ಣಾಂಕ hi = ಮುಗಿದ_pbase_paths_ಅಂಕೆ;
	ಆಗ (lo < hi) {
		ಪೂರ್ಣಾಂಕ mi = lo + (hi - lo) / 2;
		ಆದರೆ (ಮುಗಿದ_pbase_paths[mi] == hash)
			ಹಿಂತಿರುಗಿ mi;
		ಆದರೆ (ಮುಗಿದ_pbase_paths[mi] < hash)
			hi = mi;
		ಇಲ್ಲದಿದ್ದರೆ
			lo = mi + 1;
	}
	ಹಿಂತಿರುಗಿ -lo-1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ check_pbase_ಮಾರ್ಗ(ಚಿಹ್ನೆರಹಿತ hash)
{
	ಪೂರ್ಣಾಂಕ pos = ಮುಗಿದ_pbase_ಮಾರ್ಗ_pos(hash);
	ಆದರೆ (0 <= pos)
		ಹಿಂತಿರುಗಿ 1;
	pos = -pos - 1;
	ALLOC_GROW(ಮುಗಿದ_pbase_paths,
		   ಮುಗಿದ_pbase_paths_ಅಂಕೆ + 1,
		   ಮುಗಿದ_pbase_paths_alloc);
	ಮುಗಿದ_pbase_paths_ಅಂಕೆ++;
	ಆದರೆ (pos < ಮುಗಿದ_pbase_paths_ಅಂಕೆ)
		MOVE_ARRAY(ಮುಗಿದ_pbase_paths + pos + 1, ಮುಗಿದ_pbase_paths + pos,
			   ಮುಗಿದ_pbase_paths_ಅಂಕೆ - pos - 1);
	ಮುಗಿದ_pbase_paths[pos] = hash;
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ ಕೂಡಿಸು_preferred_ತಳ_object(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name)
{
	ರಚನೆ pbase_ವೃಕ್ಷ *it;
	ಗಾತ್ರ_t cmplen;
	ಚಿಹ್ನೆರಹಿತ hash = pack_name_hash_fn(name);

	ಆದರೆ (!ಅಂಕೆ_preferred_ತಳ || check_pbase_ಮಾರ್ಗ(hash))
		ಹಿಂತಿರುಗಿ;

	cmplen = name_cmp_len(name);
	ಗಾಗಿ (it = pbase_ವೃಕ್ಷ; it; it = it->ಮುಂದಿನ) {
		ಆದರೆ (cmplen == 0) {
			ಕೂಡಿಸು_object_ನಮೂದು(&it->pcache.oid, OBJ_ವೃಕ್ಷ, ಖಾಲಿ, 1);
		}
		ಇಲ್ಲದಿದ್ದರೆ {
			ರಚನೆ ವೃಕ್ಷ_desc ವೃಕ್ಷ;
			init_ವೃಕ್ಷ_desc(&ವೃಕ್ಷ, &it->pcache.oid,
				       it->pcache.ವೃಕ್ಷ_ದತ್ತಾಂಶ, it->pcache.ವೃಕ್ಷ_ಗಾತ್ರ);
			ಕೂಡಿಸು_pbase_object(&ವೃಕ್ಷ, name, cmplen, name);
		}
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ ಕೂಡಿಸು_preferred_ತಳ(ರಚನೆ object_id *oid)
{
	ರಚನೆ pbase_ವೃಕ್ಷ *it;
	ಶೂನ್ಯ *ದತ್ತಾಂಶ;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಗಾತ್ರ_2;
	ಗಾತ್ರ_t ಗಾತ್ರ_st = 0;
	ರಚನೆ object_id ವೃಕ್ಷ_oid;

	ಆದರೆ (ಕಿಟಕಿ <= ಅಂಕೆ_preferred_ತಳ++)
		ಹಿಂತಿರುಗಿ;

	ದತ್ತಾಂಶ = odb_read_object_peeled(the_repository->objects, oid,
				      OBJ_ವೃಕ್ಷ, &ಗಾತ್ರ_st, &ವೃಕ್ಷ_oid);
	ಗಾತ್ರ_2 = cast_ಗಾತ್ರ_t_ಗಮ್ಯ_ulong(ಗಾತ್ರ_st);
	ಆದರೆ (!ದತ್ತಾಂಶ)
		ಹಿಂತಿರುಗಿ;

	ಗಾಗಿ (it = pbase_ವೃಕ್ಷ; it; it = it->ಮುಂದಿನ) {
		ಆದರೆ (oideq(&it->pcache.oid, &ವೃಕ್ಷ_oid)) {
			ಬಿಡುಗಡೆ(ದತ್ತಾಂಶ);
			ಹಿಂತಿರುಗಿ;
		}
	}

	CALLOC_ARRAY(it, 1);
	it->ಮುಂದಿನ = pbase_ವೃಕ್ಷ;
	pbase_ವೃಕ್ಷ = it;

	oidcpy(&it->pcache.oid, &ವೃಕ್ಷ_oid);
	it->pcache.ವೃಕ್ಷ_ದತ್ತಾಂಶ = ದತ್ತಾಂಶ;
	it->pcache.ವೃಕ್ಷ_ಗಾತ್ರ = ಗಾತ್ರ_2;
}

ಸ್ಥಿರ ಶೂನ್ಯ cleanup_preferred_ತಳ(ಶೂನ್ಯ)
{
	ರಚನೆ pbase_ವೃಕ್ಷ *it;
	ಚಿಹ್ನೆರಹಿತ i;

	it = pbase_ವೃಕ್ಷ;
	pbase_ವೃಕ್ಷ = ಖಾಲಿ;
	ಆಗ (it) {
		ರಚನೆ pbase_ವೃಕ್ಷ *tmp = it;
		it = tmp->ಮುಂದಿನ;
		ಬಿಡುಗಡೆ(tmp->pcache.ವೃಕ್ಷ_ದತ್ತಾಂಶ);
		ಬಿಡುಗಡೆ(tmp);
	}

	ಗಾಗಿ (i = 0; i < ARRAY_ಗಾತ್ರ(pbase_ವೃಕ್ಷ_cache); i++) {
		ಆದರೆ (!pbase_ವೃಕ್ಷ_cache[i])
			ಮುಂದುವರಿ;
		ಬಿಡುಗಡೆ(pbase_ವೃಕ್ಷ_cache[i]->ವೃಕ್ಷ_ದತ್ತಾಂಶ);
		FREE_AND_NULL(pbase_ವೃಕ್ಷ_cache[i]);
	}

	FREE_AND_NULL(ಮುಗಿದ_pbase_paths);
	ಮುಗಿದ_pbase_paths_ಅಂಕೆ = ಮುಗಿದ_pbase_paths_alloc = 0;
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
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ can_reuse_delta(ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *ತಳ_oid,
			   ರಚನೆ object_ನಮೂದು *delta,
			   ರಚನೆ object_ನಮೂದು **ತಳ_ಹೊರಹೋಗು)
{
	ರಚನೆ object_ನಮೂದು *ತಳ;

	/*
	 * First see if we're already sending the base (or it's explicitly in
	 * our "excluded" list).
	 */
	ತಳ = packlist_ಕಂಡುಹಿಡಿ(&ಗಮ್ಯ_pack, ತಳ_oid);
	ಆದರೆ (ತಳ) {
		ಆದರೆ (!ಒಳ_same_island(&delta->idx.oid, &ತಳ->idx.oid))
			ಹಿಂತಿರುಗಿ 0;
		*ತಳ_ಹೊರಹೋಗು = ತಳ;
		ಹಿಂತಿರುಗಿ 1;
	}

	/*
	 * Otherwise, reachability bitmaps may tell us if the receiver has it,
	 * even if it was buried too deep in history to make it into the
	 * packing list.
	 */
	ಆದರೆ (thin && bitmap_has_oid_ಒಳ_uninteresting(bitmap_git, ತಳ_oid)) {
		ಆದರೆ (use_delta_islands) {
			ಆದರೆ (!ಒಳ_same_island(&delta->idx.oid, ತಳ_oid))
				ಹಿಂತಿರುಗಿ 0;
		}
		*ತಳ_ಹೊರಹೋಗು = ಖಾಲಿ;
		ಹಿಂತಿರುಗಿ 1;
	}

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ prefetch_ಗಮ್ಯ_pack(uint32_t object_ಸೂಚ್ಯಂಕ_ಆರಂಭ) {
	ರಚನೆ oid_array ಗಮ್ಯ_fetch = OID_ARRAY_INIT;
	uint32_t i;

	ಗಾಗಿ (i = object_ಸೂಚ್ಯಂಕ_ಆರಂಭ; i < ಗಮ್ಯ_pack.nr_objects; i++) {
		ರಚನೆ object_ನಮೂದು *ನಮೂದು = ಗಮ್ಯ_pack.objects + i;

		ಆದರೆ (!odb_read_object_info_extended(the_repository->objects,
						   &ನಮೂದು->idx.oid,
						   ಖಾಲಿ,
						   OBJECT_INFO_FOR_PREFETCH))
			ಮುಂದುವರಿ;
		oid_array_append(&ಗಮ್ಯ_fetch, &ನಮೂದು->idx.oid);
	}
	promisor_remote_get_ನೇರ(the_repository,
				   ಗಮ್ಯ_fetch.oid, ಗಮ್ಯ_fetch.nr);
	oid_array_clear(&ಗಮ್ಯ_fetch);
}

ಸ್ಥಿರ ಶೂನ್ಯ check_object(ರಚನೆ object_ನಮೂದು *ನಮೂದು, uint32_t object_ಸೂಚ್ಯಂಕ)
{
	ಗಾತ್ರ_t canonical_ಗಾತ್ರ;
	ಎಣಿಕೆ object_type type;
	ರಚನೆ object_info oi = {.typep = &type, .sizep = &canonical_ಗಾತ್ರ};

	ಆದರೆ (ಒಳ_PACK(ನಮೂದು)) {
		ರಚನೆ packed_git *p = ಒಳ_PACK(ನಮೂದು);
		ರಚನೆ pack_ಕಿಟಕಿ *w_curs = ಖಾಲಿ;
		ಪೂರ್ಣಾಂಕ have_ತಳ = 0;
		ರಚನೆ object_id ತಳ_ref;
		ರಚನೆ object_ನಮೂದು *ತಳ_ನಮೂದು;
		ಗಾತ್ರ_t ಬಳಸಿದ_2, ಬಳಸಿದ_0, avail;
		off_t ofs;
		ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *buf, c;
		ಎಣಿಕೆ object_type type;
		ಗಾತ್ರ_t ಒಳ_pack_ಗಾತ್ರ;

		buf = use_pack(p, &w_curs, ನಮೂದು->ಒಳ_pack_ಸ್ಥಾನಾಂತರ, &avail);

		/*
		 * We want in_pack_type even if we do not reuse delta
		 * since non-delta representations could still be reused.
		 */
		ಬಳಸಿದ_2 = unpack_object_ಶೀರ್ಷಿಕೆ_ಬಫರ್(buf, avail,
						   &type,
						   &ಒಳ_pack_ಗಾತ್ರ);
		ಆದರೆ (ಬಳಸಿದ_2 == 0)
			ಹೋಗು give_ಮೇಲೆ;

		ಆದರೆ (type < 0)
			BUG("invalid type %d", type);
		ನಮೂದು->ಒಳ_pack_type = type;

		/*
		 * Determine if this is a delta and if so whether we can
		 * reuse it or not.  Otherwise let's find out as cheaply as
		 * possible what the actual type and size for this object is.
		 */
		ಆಯ್ಕೆ (ನಮೂದು->ಒಳ_pack_type) {
		ಪೂರ್ವನಿಯೋಜಿತ:
			/* Not a delta hence we've already got all we need. */
			oe_ಗಣ_type(ನಮೂದು, ನಮೂದು->ಒಳ_pack_type);
			ಗಣ_ಗಾತ್ರ(ನಮೂದು, ಒಳ_pack_ಗಾತ್ರ);
			ನಮೂದು->ಒಳ_pack_ಶೀರ್ಷಿಕೆ_ಗಾತ್ರ = ಬಳಸಿದ_2;
			ಆದರೆ (oe_type(ನಮೂದು) < OBJ_COMMIT || oe_type(ನಮೂದು) > OBJ_BLOB)
				ಹೋಗು give_ಮೇಲೆ;
			unuse_pack(&w_curs);
			ಹಿಂತಿರುಗಿ;
		ಸ್ಥಿತಿ OBJ_REF_DELTA:
			ಆದರೆ (reuse_delta && !ನಮೂದು->preferred_ತಳ) {
				oidread(&ತಳ_ref,
					use_pack(p, &w_curs,
						 ನಮೂದು->ಒಳ_pack_ಸ್ಥಾನಾಂತರ + ಬಳಸಿದ_2,
						 ಖಾಲಿ),
					the_repository->hash_algo);
				have_ತಳ = 1;
			}
			ನಮೂದು->ಒಳ_pack_ಶೀರ್ಷಿಕೆ_ಗಾತ್ರ = ಬಳಸಿದ_2 + the_hash_algo->rawsz;
			ನಿಲ್ಲಿಸು;
		ಸ್ಥಿತಿ OBJ_OFS_DELTA:
			buf = use_pack(p, &w_curs,
				       ನಮೂದು->ಒಳ_pack_ಸ್ಥಾನಾಂತರ + ಬಳಸಿದ_2, ಖಾಲಿ);
			ಬಳಸಿದ_0 = 0;
			c = buf[ಬಳಸಿದ_0++];
			ofs = c & 127;
			ಆಗ (c & 128) {
				ofs += 1;
				ಆದರೆ (!ofs || MSB(ofs, 7)) {
					error(_("delta base offset overflow in pack for %s"),
					      oid_ಗಮ್ಯ_hex(&ನಮೂದು->idx.oid));
					ಹೋಗು give_ಮೇಲೆ;
				}
				c = buf[ಬಳಸಿದ_0++];
				ofs = (ofs << 7) + (c & 127);
			}
			ofs = ನಮೂದು->ಒಳ_pack_ಸ್ಥಾನಾಂತರ - ofs;
			ಆದರೆ (ofs <= 0 || ofs >= ನಮೂದು->ಒಳ_pack_ಸ್ಥಾನಾಂತರ) {
				error(_("delta base offset out of bound for %s"),
				      oid_ಗಮ್ಯ_hex(&ನಮೂದು->idx.oid));
				ಹೋಗು give_ಮೇಲೆ;
			}
			ಆದರೆ (reuse_delta && !ನಮೂದು->preferred_ತಳ) {
				uint32_t pos;
				ಆದರೆ (ಸ್ಥಾನಾಂತರ_ಗಮ್ಯ_pack_pos(p, ofs, &pos) < 0)
					ಹೋಗು give_ಮೇಲೆ;
				ಆದರೆ (!nth_packed_object_id(&ತಳ_ref, p,
							  pack_pos_ಗಮ್ಯ_ಸೂಚ್ಯಂಕ(p, pos)))
					have_ತಳ = 1;
			}
			ನಮೂದು->ಒಳ_pack_ಶೀರ್ಷಿಕೆ_ಗಾತ್ರ = ಬಳಸಿದ_2 + ಬಳಸಿದ_0;
			ನಿಲ್ಲಿಸು;
		}

		ಆದರೆ (have_ತಳ &&
		    can_reuse_delta(&ತಳ_ref, ನಮೂದು, &ತಳ_ನಮೂದು)) {
			oe_ಗಣ_type(ನಮೂದು, ನಮೂದು->ಒಳ_pack_type);
			ಗಣ_ಗಾತ್ರ(ನಮೂದು, ಒಳ_pack_ಗಾತ್ರ); /* delta size */
			ಗಣ_DELTA_ಗಾತ್ರ(ನಮೂದು, ಒಳ_pack_ಗಾತ್ರ);

			ಆದರೆ (ತಳ_ನಮೂದು) {
				ಗಣ_DELTA(ನಮೂದು, ತಳ_ನಮೂದು);
				ನಮೂದು->delta_sibling_idx = ತಳ_ನಮೂದು->delta_ಮಗು_idx;
				ಗಣ_DELTA_ಮಗು(ತಳ_ನಮೂದು, ನಮೂದು);
			} ಇಲ್ಲದಿದ್ದರೆ {
				ಗಣ_DELTA_EXT(ನಮೂದು, &ತಳ_ref);
			}

			unuse_pack(&w_curs);
			ಹಿಂತಿರುಗಿ;
		}

		ಆದರೆ (oe_type(ನಮೂದು)) {
			off_t delta_pos;

			/*
			 * This must be a delta and we already know what the
			 * final object type is.  Let's extract the actual
			 * object size from the delta header.
			 */
			delta_pos = ನಮೂದು->ಒಳ_pack_ಸ್ಥಾನಾಂತರ + ನಮೂದು->ಒಳ_pack_ಶೀರ್ಷಿಕೆ_ಗಾತ್ರ;
			canonical_ಗಾತ್ರ = get_ಗಾತ್ರ_from_delta(p, &w_curs,
							     delta_pos);
			ಆದರೆ (canonical_ಗಾತ್ರ == 0)
				ಹೋಗು give_ಮೇಲೆ;
			ಗಣ_ಗಾತ್ರ(ನಮೂದು, canonical_ಗಾತ್ರ);
			unuse_pack(&w_curs);
			ಹಿಂತಿರುಗಿ;
		}

		/*
		 * No choice but to fall back to the recursive delta walk
		 * with odb_read_object_info() to find about the object type
		 * at this point...
		 */
		give_ಮೇಲೆ:
		unuse_pack(&w_curs);
	}

	ಆದರೆ (odb_read_object_info_extended(the_repository->objects, &ನಮೂದು->idx.oid, &oi,
					  OBJECT_INFO_SKIP_FETCH_OBJECT | OBJECT_INFO_LOOKUP_REPLACE) < 0) {
		ಆದರೆ (repo_has_promisor_remote(the_repository)) {
			prefetch_ಗಮ್ಯ_pack(object_ಸೂಚ್ಯಂಕ);
			ಆದರೆ (odb_read_object_info_extended(the_repository->objects, &ನಮೂದು->idx.oid, &oi,
							  OBJECT_INFO_SKIP_FETCH_OBJECT | OBJECT_INFO_LOOKUP_REPLACE) < 0)
				type = -1;
		} ಇಲ್ಲದಿದ್ದರೆ {
			type = -1;
		}
	}
	oe_ಗಣ_type(ನಮೂದು, type);
	ಆದರೆ (ನಮೂದು->type_valid) {
		ಗಣ_ಗಾತ್ರ(ನಮೂದು, canonical_ಗಾತ್ರ);
	} ಇಲ್ಲದಿದ್ದರೆ {
		/*
		 * Bad object type is checked in prepare_pack().  This is
		 * to permit a missing preferred base object to be ignored
		 * as a preferred base.  Doing so can result in a larger
		 * pack file, but the transfer will still take place.
		 */
	}
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ pack_ಸ್ಥಾನಾಂತರ_ವಿಂಗಡಿಸು(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *_a, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *_b)
{
	ಸ್ಥಿರಾಂಕ ರಚನೆ object_ನಮೂದು *a = *(ರಚನೆ object_ನಮೂದು **)_a;
	ಸ್ಥಿರಾಂಕ ರಚನೆ object_ನಮೂದು *b = *(ರಚನೆ object_ನಮೂದು **)_b;
	ಸ್ಥಿರಾಂಕ ರಚನೆ packed_git *a_ಒಳ_pack = ಒಳ_PACK(a);
	ಸ್ಥಿರಾಂಕ ರಚನೆ packed_git *b_ಒಳ_pack = ಒಳ_PACK(b);

	/* avoid filesystem trashing with loose objects */
	ಆದರೆ (!a_ಒಳ_pack && !b_ಒಳ_pack)
		ಹಿಂತಿರುಗಿ oidcmp(&a->idx.oid, &b->idx.oid);

	ಆದರೆ (a_ಒಳ_pack < b_ಒಳ_pack)
		ಹಿಂತಿರುಗಿ -1;
	ಆದರೆ (a_ಒಳ_pack > b_ಒಳ_pack)
		ಹಿಂತಿರುಗಿ 1;
	ಹಿಂತಿರುಗಿ a->ಒಳ_pack_ಸ್ಥಾನಾಂತರ < b->ಒಳ_pack_ಸ್ಥಾನಾಂತರ ? -1 :
			(a->ಒಳ_pack_ಸ್ಥಾನಾಂತರ > b->ಒಳ_pack_ಸ್ಥಾನಾಂತರ);
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
ಸ್ಥಿರ ಶೂನ್ಯ drop_reused_delta(ರಚನೆ object_ನಮೂದು *ನಮೂದು)
{
	ಚಿಹ್ನೆರಹಿತ *idx = &ಗಮ್ಯ_pack.objects[ನಮೂದು->delta_idx - 1].delta_ಮಗು_idx;
	ರಚನೆ object_info oi = OBJECT_INFO_INIT;
	ಎಣಿಕೆ object_type type;
	ಗಾತ್ರ_t ಗಾತ್ರ_2;

	ಆಗ (*idx) {
		ರಚನೆ object_ನಮೂದು *oe = &ಗಮ್ಯ_pack.objects[*idx - 1];

		ಆದರೆ (oe == ನಮೂದು)
			*idx = oe->delta_sibling_idx;
		ಇಲ್ಲದಿದ್ದರೆ
			idx = &oe->delta_sibling_idx;
	}
	ಗಣ_DELTA(ನಮೂದು, ಖಾಲಿ);
	ನಮೂದು->ಆಳ = 0;

	oi.sizep = &ಗಾತ್ರ_2;
	oi.typep = &type;
	ಆದರೆ (packed_object_info(ಖಾಲಿ, ಒಳ_PACK(ನಮೂದು), ನಮೂದು->ಒಳ_pack_ಸ್ಥಾನಾಂತರ, &oi) < 0) {
		/*
		 * We failed to get the info from this pack for some reason;
		 * fall back to odb_read_object_info, which may find another copy.
		 * And if that fails, the error will be recorded in oe_type(entry)
		 * and dealt with in prepare_pack().
		 */
		oe_ಗಣ_type(ನಮೂದು,
			    odb_read_object_info(the_repository->objects,
						 &ನಮೂದು->idx.oid, &ಗಾತ್ರ_2));
	} ಇಲ್ಲದಿದ್ದರೆ {
		oe_ಗಣ_type(ನಮೂದು, type);
	}
	ಗಣ_ಗಾತ್ರ(ನಮೂದು, ಗಾತ್ರ_2);
}

/*
 * Follow the chain of deltas from this entry onward, throwing away any links
 * that cause us to hit a cycle (as determined by the DFS state flags in
 * the entries).
 *
 * We also detect too-long reused chains that would violate our --depth
 * limit.
 */
ಸ್ಥಿರ ಶೂನ್ಯ break_delta_chains(ರಚನೆ object_ನಮೂದು *ನಮೂದು)
{
	/*
	 * The actual depth of each object we will write is stored as an int,
	 * as it cannot exceed our int "depth" limit. But before we break
	 * changes based no that limit, we may potentially go as deep as the
	 * number of objects, which is elsewhere bounded to a uint32_t.
	 */
	uint32_t ಒಟ್ಟು_ಆಳ;
	ರಚನೆ object_ನಮೂದು *cur, *ಮುಂದಿನ;

	ಗಾಗಿ (cur = ನಮೂದು, ಒಟ್ಟು_ಆಳ = 0;
	     cur;
	     cur = DELTA(cur), ಒಟ್ಟು_ಆಳ++) {
		ಆದರೆ (cur->dfs_state == DFS_ಮುಗಿದ) {
			/*
			 * We've already seen this object and know it isn't
			 * part of a cycle. We do need to append its depth
			 * to our count.
			 */
			ಒಟ್ಟು_ಆಳ += cur->ಆಳ;
			ನಿಲ್ಲಿಸು;
		}

		/*
		 * We break cycles before looping, so an ACTIVE state (or any
		 * other cruft which made its way into the state variable)
		 * is a bug.
		 */
		ಆದರೆ (cur->dfs_state != DFS_NONE)
			BUG("confusing delta dfs state in first pass: %d",
			    cur->dfs_state);

		/*
		 * Now we know this is the first time we've seen the object. If
		 * it's not a delta, we're done traversing, but we'll mark it
		 * done to save time on future traversals.
		 */
		ಆದರೆ (!DELTA(cur)) {
			cur->dfs_state = DFS_ಮುಗಿದ;
			ನಿಲ್ಲಿಸು;
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
		cur->dfs_state = DFS_ಸಕ್ರಿಯ;
		ಆದರೆ (DELTA(cur)->dfs_state == DFS_ಸಕ್ರಿಯ) {
			drop_reused_delta(cur);
			cur->dfs_state = DFS_ಮುಗಿದ;
			ನಿಲ್ಲಿಸು;
		}
	}

	/*
	 * And now that we've gone all the way to the bottom of the chain, we
	 * need to clear the active flags and set the depth fields as
	 * appropriate. Unlike the loop above, which can quit when it drops a
	 * delta, we need to keep going to look for more depth cuts. So we need
	 * an extra "next" pointer to keep going after we reset cur->delta.
	 */
	ಗಾಗಿ (cur = ನಮೂದು; cur; cur = ಮುಂದಿನ) {
		ಮುಂದಿನ = DELTA(cur);

		/*
		 * We should have a chain of zero or more ACTIVE states down to
		 * a final DONE. We can quit after the DONE, because either it
		 * has no bases, or we've already handled them in a previous
		 * call.
		 */
		ಆದರೆ (cur->dfs_state == DFS_ಮುಗಿದ)
			ನಿಲ್ಲಿಸು;
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (cur->dfs_state != DFS_ಸಕ್ರಿಯ)
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
		cur->ಆಳ = (ಒಟ್ಟು_ಆಳ--) % (ಆಳ + 1);
		ಆದರೆ (!cur->ಆಳ)
			drop_reused_delta(cur);

		cur->dfs_state = DFS_ಮುಗಿದ;
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ get_object_details(ಶೂನ್ಯ)
{
	uint32_t i;
	ರಚನೆ object_ನಮೂದು **sorted_by_ಸ್ಥಾನಾಂತರ;

	ಆದರೆ (ಪ್ರಗತಿ)
		ಪ್ರಗತಿ_state = ಆರಂಭ_ಪ್ರಗತಿ(the_repository,
						_("Counting objects"),
						ಗಮ್ಯ_pack.nr_objects);

	CALLOC_ARRAY(sorted_by_ಸ್ಥಾನಾಂತರ, ಗಮ್ಯ_pack.nr_objects);
	ಗಾಗಿ (i = 0; i < ಗಮ್ಯ_pack.nr_objects; i++)
		sorted_by_ಸ್ಥಾನಾಂತರ[i] = ಗಮ್ಯ_pack.objects + i;
	QSORT(sorted_by_ಸ್ಥಾನಾಂತರ, ಗಮ್ಯ_pack.nr_objects, pack_ಸ್ಥಾನಾಂತರ_ವಿಂಗಡಿಸು);

	ಗಾಗಿ (i = 0; i < ಗಮ್ಯ_pack.nr_objects; i++) {
		ರಚನೆ object_ನಮೂದು *ನಮೂದು = sorted_by_ಸ್ಥಾನಾಂತರ[i];
		check_object(ನಮೂದು, i);
		ಆದರೆ (ನಮೂದು->type_valid &&
		    oe_ಗಾತ್ರ_greater_than(&ಗಮ್ಯ_pack, ನಮೂದು,
					 repo_settings_get_big_file_ಮಿತಿ(the_repository)))
			ನಮೂದು->no_try_delta = 1;
		display_ಪ್ರಗತಿ(ಪ್ರಗತಿ_state, i + 1);
	}
	stop_ಪ್ರಗತಿ(&ಪ್ರಗತಿ_state);

	/*
	 * This must happen in a second pass, since we rely on the delta
	 * information for the whole list being completed.
	 */
	ಗಾಗಿ (i = 0; i < ಗಮ್ಯ_pack.nr_objects; i++)
		break_delta_chains(&ಗಮ್ಯ_pack.objects[i]);

	ಬಿಡುಗಡೆ(sorted_by_ಸ್ಥಾನಾಂತರ);
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
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ type_ಗಾತ್ರ_ವಿಂಗಡಿಸು(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *_a, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *_b)
{
	ಸ್ಥಿರಾಂಕ ರಚನೆ object_ನಮೂದು *a = *(ರಚನೆ object_ನಮೂದು **)_a;
	ಸ್ಥಿರಾಂಕ ರಚನೆ object_ನಮೂದು *b = *(ರಚನೆ object_ನಮೂದು **)_b;
	ಸ್ಥಿರಾಂಕ ಎಣಿಕೆ object_type a_type = oe_type(a);
	ಸ್ಥಿರಾಂಕ ಎಣಿಕೆ object_type b_type = oe_type(b);
	ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ a_ಗಾತ್ರ = ಗಾತ್ರ_4(a);
	ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ b_ಗಾತ್ರ = ಗಾತ್ರ_4(b);

	ಆದರೆ (a_type > b_type)
		ಹಿಂತಿರುಗಿ -1;
	ಆದರೆ (a_type < b_type)
		ಹಿಂತಿರುಗಿ 1;
	ಆದರೆ (a->hash > b->hash)
		ಹಿಂತಿರುಗಿ -1;
	ಆದರೆ (a->hash < b->hash)
		ಹಿಂತಿರುಗಿ 1;
	ಆದರೆ (a->preferred_ತಳ > b->preferred_ತಳ)
		ಹಿಂತಿರುಗಿ -1;
	ಆದರೆ (a->preferred_ತಳ < b->preferred_ತಳ)
		ಹಿಂತಿರುಗಿ 1;
	ಆದರೆ (use_delta_islands) {
		ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ island_cmp = island_delta_cmp(&a->idx.oid, &b->idx.oid);
		ಆದರೆ (island_cmp)
			ಹಿಂತಿರುಗಿ island_cmp;
	}
	ಆದರೆ (a_ಗಾತ್ರ > b_ಗಾತ್ರ)
		ಹಿಂತಿರುಗಿ -1;
	ಆದರೆ (a_ಗಾತ್ರ < b_ಗಾತ್ರ)
		ಹಿಂತಿರುಗಿ 1;
	ಹಿಂತಿರುಗಿ a < b ? -1 : (a > b);  /* newest first */
}

ರಚನೆ unpacked {
	ರಚನೆ object_ನಮೂದು *ನಮೂದು;
	ಶೂನ್ಯ *ದತ್ತಾಂಶ;
	ರಚನೆ delta_ಸೂಚ್ಯಂಕ *ಸೂಚ್ಯಂಕ;
	ಚಿಹ್ನೆರಹಿತ ಆಳ;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ delta_cacheable(ಗಾತ್ರ_t src_ಗಾತ್ರ, ಗಾತ್ರ_t trg_ಗಾತ್ರ,
			   ಗಾತ್ರ_t delta_ಗಾತ್ರ_2)
{
	ಆದರೆ (max_delta_cache_ಗಾತ್ರ && delta_cache_ಗಾತ್ರ + delta_ಗಾತ್ರ_2 > max_delta_cache_ಗಾತ್ರ)
		ಹಿಂತಿರುಗಿ 0;

	ಆದರೆ (delta_ಗಾತ್ರ_2 < cache_max_small_delta_ಗಾತ್ರ)
		ಹಿಂತಿರುಗಿ 1;

	/* cache delta, if objects are large enough compared to delta size */
	ಆದರೆ ((src_ಗಾತ್ರ >> 20) + (trg_ಗಾತ್ರ >> 21) > (delta_ಗಾತ್ರ_2 >> 10))
		ಹಿಂತಿರುಗಿ 1;

	ಹಿಂತಿರುಗಿ 0;
}

/* Protect delta_cache_size */
ಸ್ಥಿರ pthread_mutex_t cache_mutex;
#ವ್ಯಾಖ್ಯಾನಿಸು cache_ಬೀಗ()		pthread_mutex_ಬೀಗ(&cache_mutex)
#ವ್ಯಾಖ್ಯಾನಿಸು cache_unlock()		pthread_mutex_unlock(&cache_mutex)

/*
 * Protect object list partitioning (e.g. struct thread_param) and
 * progress_state
 */
ಸ್ಥಿರ pthread_mutex_t ಪ್ರಗತಿ_mutex;
#ವ್ಯಾಖ್ಯಾನಿಸು ಪ್ರಗತಿ_ಬೀಗ()		pthread_mutex_ಬೀಗ(&ಪ್ರಗತಿ_mutex)
#ವ್ಯಾಖ್ಯಾನಿಸು ಪ್ರಗತಿ_unlock()	pthread_mutex_unlock(&ಪ್ರಗತಿ_mutex)

/*
 * Access to struct object_entry is unprotected since each thread owns
 * a portion of the main object list. Just don't access object entries
 * ahead in the list because they can be stolen and would need
 * progress_mutex for protection.
 */

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ oe_ಗಾತ್ರ_less_than(ರಚನೆ packing_ದತ್ತಾಂಶ *pack,
				    ಸ್ಥಿರಾಂಕ ರಚನೆ object_ನಮೂದು *lhs,
				    ಗಾತ್ರ_t rhs)
{
	ಆದರೆ (lhs->ಗಾತ್ರ_valid)
		ಹಿಂತಿರುಗಿ lhs->ಗಾತ್ರ_3 < rhs;
	ಆದರೆ (rhs < pack->oe_ಗಾತ್ರ_ಎಲ್ಲೆ) /* rhs < 2^x <= lhs ? */
		ಹಿಂತಿರುಗಿ 0;
	ಹಿಂತಿರುಗಿ oe_get_ಗಾತ್ರ_slow(pack, lhs) < rhs;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ oe_ಗಣ_ವೃಕ್ಷ_ಆಳ(ರಚನೆ packing_ದತ್ತಾಂಶ *pack,
				     ರಚನೆ object_ನಮೂದು *e,
				     ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ವೃಕ್ಷ_ಆಳ)
{
	ಆದರೆ (!pack->ವೃಕ್ಷ_ಆಳ)
		CALLOC_ARRAY(pack->ವೃಕ್ಷ_ಆಳ, pack->nr_alloc);
	pack->ವೃಕ್ಷ_ಆಳ[e - pack->objects] = ವೃಕ್ಷ_ಆಳ;
}

ಸ್ಥಿರ ಶೂನ್ಯ ದಾಖಲೆ_ವೃಕ್ಷ_ಆಳ(ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *p;
	ಚಿಹ್ನೆರಹಿತ ಆಳ;
	ರಚನೆ object_ನಮೂದು *ent;

	/* the empty string is a root tree, which is depth 0 */
	ಆಳ = *name ? 1 : 0;
	ಗಾಗಿ (p = strchr(name, '/'); p; p = strchr(p + 1, '/'))
		ಆಳ++;

	ent = packlist_ಕಂಡುಹಿಡಿ(&ಗಮ್ಯ_pack, oid);
	ಆದರೆ (ent && ಆಳ > oe_ವೃಕ್ಷ_ಆಳ(&ಗಮ್ಯ_pack, ent))
		oe_ಗಣ_ವೃಕ್ಷ_ಆಳ(&ಗಮ್ಯ_pack, ent, ಆಳ);
}

/*
 * Return the size of the object without doing any delta
 * reconstruction (so non-deltas are true object sizes, but deltas
 * return the size of the delta data).
 */
ಗಾತ್ರ_t oe_get_ಗಾತ್ರ_slow(ರಚನೆ packing_ದತ್ತಾಂಶ *pack,
			ಸ್ಥಿರಾಂಕ ರಚನೆ object_ನಮೂದು *e)
{
	ರಚನೆ packed_git *p;
	ರಚನೆ pack_ಕಿಟಕಿ *w_curs;
	ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *buf;
	ಎಣಿಕೆ object_type type;
	ಗಾತ್ರ_t ಬಳಸಿದ_2, avail, ಗಾತ್ರ_2;

	ಆದರೆ (e->type_ != OBJ_OFS_DELTA && e->type_ != OBJ_REF_DELTA) {
		ಗಾತ್ರ_t sz;
		packing_ದತ್ತಾಂಶ_ಬೀಗ(&ಗಮ್ಯ_pack);
		ಆದರೆ (odb_read_object_info(the_repository->objects,
					 &e->idx.oid, &sz) < 0)
			die(_("unable to get size of %s"),
			    oid_ಗಮ್ಯ_hex(&e->idx.oid));
		packing_ದತ್ತಾಂಶ_unlock(&ಗಮ್ಯ_pack);
		ಹಿಂತಿರುಗಿ sz;
	}

	p = oe_ಒಳ_pack(pack, e);
	ಆದರೆ (!p)
		BUG("when e->type is a delta, it must belong to a pack");

	packing_ದತ್ತಾಂಶ_ಬೀಗ(&ಗಮ್ಯ_pack);
	w_curs = ಖಾಲಿ;
	buf = use_pack(p, &w_curs, e->ಒಳ_pack_ಸ್ಥಾನಾಂತರ, &avail);
	ಬಳಸಿದ_2 = unpack_object_ಶೀರ್ಷಿಕೆ_ಬಫರ್(buf, avail, &type, &ಗಾತ್ರ_2);
	ಆದರೆ (ಬಳಸಿದ_2 == 0)
		die(_("unable to parse object header of %s"),
		    oid_ಗಮ್ಯ_hex(&e->idx.oid));

	unuse_pack(&w_curs);
	packing_ದತ್ತಾಂಶ_unlock(&ಗಮ್ಯ_pack);
	ಹಿಂತಿರುಗಿ ಗಾತ್ರ_2;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ try_delta(ರಚನೆ unpacked *trg, ರಚನೆ unpacked *src,
		     ಚಿಹ್ನೆರಹಿತ max_ಆಳ, ಗಾತ್ರ_t *mem_usage)
{
	ರಚನೆ object_ನಮೂದು *trg_ನಮೂದು = trg->ನಮೂದು;
	ರಚನೆ object_ನಮೂದು *src_ನಮೂದು = src->ನಮೂದು;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ trg_ಗಾತ್ರ, src_ಗಾತ್ರ, sizediff, max_ಗಾತ್ರ, sz;
	ಗಾತ್ರ_t delta_ಗಾತ್ರ_2;
	ಚಿಹ್ನೆರಹಿತ ref_ಆಳ;
	ಎಣಿಕೆ object_type type;
	ಶೂನ್ಯ *delta_buf;

	/* Don't bother doing diffs between different types */
	ಆದರೆ (oe_type(trg_ನಮೂದು) != oe_type(src_ನಮೂದು))
		ಹಿಂತಿರುಗಿ -1;

	/*
	 * We do not bother to try a delta that we discarded on an
	 * earlier try, but only when reusing delta data.  Note that
	 * src_entry that is marked as the preferred_base should always
	 * be considered, as even if we produce a suboptimal delta against
	 * it, we will still save the transfer cost, as we already know
	 * the other side has it and we won't send src_entry at all.
	 */
	ಆದರೆ (reuse_delta && ಒಳ_PACK(trg_ನಮೂದು) &&
	    ಒಳ_PACK(trg_ನಮೂದು) == ಒಳ_PACK(src_ನಮೂದು) &&
	    !src_ನಮೂದು->preferred_ತಳ &&
	    trg_ನಮೂದು->ಒಳ_pack_type != OBJ_REF_DELTA &&
	    trg_ನಮೂದು->ಒಳ_pack_type != OBJ_OFS_DELTA)
		ಹಿಂತಿರುಗಿ 0;

	/* Let's not bust the allowed depth. */
	ಆದರೆ (src->ಆಳ >= max_ಆಳ)
		ಹಿಂತಿರುಗಿ 0;

	/* Now some size filtering heuristics. */
	trg_ಗಾತ್ರ = ಗಾತ್ರ_4(trg_ನಮೂದು);
	ಆದರೆ (!DELTA(trg_ನಮೂದು)) {
		max_ಗಾತ್ರ = trg_ಗಾತ್ರ/2 - the_hash_algo->rawsz;
		ref_ಆಳ = 1;
	} ಇಲ್ಲದಿದ್ದರೆ {
		max_ಗಾತ್ರ = DELTA_ಗಾತ್ರ(trg_ನಮೂದು);
		ref_ಆಳ = trg->ಆಳ;
	}
	max_ಗಾತ್ರ = (uint64_t)max_ಗಾತ್ರ * (max_ಆಳ - src->ಆಳ) /
						(max_ಆಳ - ref_ಆಳ + 1);
	ಆದರೆ (max_ಗಾತ್ರ == 0)
		ಹಿಂತಿರುಗಿ 0;
	src_ಗಾತ್ರ = ಗಾತ್ರ_4(src_ನಮೂದು);
	sizediff = src_ಗಾತ್ರ < trg_ಗಾತ್ರ ? trg_ಗಾತ್ರ - src_ಗಾತ್ರ : 0;
	ಆದರೆ (sizediff >= max_ಗಾತ್ರ)
		ಹಿಂತಿರುಗಿ 0;
	ಆದರೆ (trg_ಗಾತ್ರ < src_ಗಾತ್ರ / 32)
		ಹಿಂತಿರುಗಿ 0;

	ಆದರೆ (!ಒಳ_same_island(&trg->ನಮೂದು->idx.oid, &src->ನಮೂದು->idx.oid))
		ಹಿಂತಿರುಗಿ 0;

	/* Load data if not already done */
	ಆದರೆ (!trg->ದತ್ತಾಂಶ) {
		ಗಾತ್ರ_t sz_st = 0;
		packing_ದತ್ತಾಂಶ_ಬೀಗ(&ಗಮ್ಯ_pack);
		trg->ದತ್ತಾಂಶ = odb_read_object(the_repository->objects,
					    &trg_ನಮೂದು->idx.oid, &type,
					    &sz_st);
		sz = cast_ಗಾತ್ರ_t_ಗಮ್ಯ_ulong(sz_st);
		packing_ದತ್ತಾಂಶ_unlock(&ಗಮ್ಯ_pack);
		ಆದರೆ (!trg->ದತ್ತಾಂಶ)
			die(_("object %s cannot be read"),
			    oid_ಗಮ್ಯ_hex(&trg_ನಮೂದು->idx.oid));
		ಆದರೆ (sz != trg_ಗಾತ್ರ)
			die(_("object %s inconsistent object length (%"PRIuMAX" vs %"PRIuMAX")"),
			    oid_ಗಮ್ಯ_hex(&trg_ನಮೂದು->idx.oid), (uintmax_t)sz,
			    (uintmax_t)trg_ಗಾತ್ರ);
		*mem_usage += sz;
	}
	ಆದರೆ (!src->ದತ್ತಾಂಶ) {
		ಗಾತ್ರ_t sz_st = 0;
		packing_ದತ್ತಾಂಶ_ಬೀಗ(&ಗಮ್ಯ_pack);
		src->ದತ್ತಾಂಶ = odb_read_object(the_repository->objects,
					    &src_ನಮೂದು->idx.oid, &type,
					    &sz_st);
		sz = cast_ಗಾತ್ರ_t_ಗಮ್ಯ_ulong(sz_st);
		packing_ದತ್ತಾಂಶ_unlock(&ಗಮ್ಯ_pack);
		ಆದರೆ (!src->ದತ್ತಾಂಶ) {
			ಆದರೆ (src_ನಮೂದು->preferred_ತಳ) {
				ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ warned = 0;
				ಆದರೆ (!warned++)
					warning(_("object %s cannot be read"),
						oid_ಗಮ್ಯ_hex(&src_ನಮೂದು->idx.oid));
				/*
				 * Those objects are not included in the
				 * resulting pack.  Be resilient and ignore
				 * them if they can't be read, in case the
				 * pack could be created nevertheless.
				 */
				ಹಿಂತಿರುಗಿ 0;
			}
			die(_("object %s cannot be read"),
			    oid_ಗಮ್ಯ_hex(&src_ನಮೂದು->idx.oid));
		}
		ಆದರೆ (sz != src_ಗಾತ್ರ)
			die(_("object %s inconsistent object length (%"PRIuMAX" vs %"PRIuMAX")"),
			    oid_ಗಮ್ಯ_hex(&src_ನಮೂದು->idx.oid), (uintmax_t)sz,
			    (uintmax_t)src_ಗಾತ್ರ);
		*mem_usage += sz;
	}
	ಆದರೆ (!src->ಸೂಚ್ಯಂಕ) {
		src->ಸೂಚ್ಯಂಕ = create_delta_ಸೂಚ್ಯಂಕ(src->ದತ್ತಾಂಶ, src_ಗಾತ್ರ);
		ಆದರೆ (!src->ಸೂಚ್ಯಂಕ) {
			ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ warned = 0;
			ಆದರೆ (!warned++)
				warning(_("suboptimal pack - out of memory"));
			ಹಿಂತಿರುಗಿ 0;
		}
		*mem_usage += sizeof_delta_ಸೂಚ್ಯಂಕ(src->ಸೂಚ್ಯಂಕ);
	}

	delta_buf = create_delta(src->ಸೂಚ್ಯಂಕ, trg->ದತ್ತಾಂಶ, trg_ಗಾತ್ರ, &delta_ಗಾತ್ರ_2, max_ಗಾತ್ರ);
	ಆದರೆ (!delta_buf)
		ಹಿಂತಿರುಗಿ 0;

	ಆದರೆ (DELTA(trg_ನಮೂದು)) {
		/* Prefer only shallower same-sized deltas. */
		ಆದರೆ (delta_ಗಾತ್ರ_2 == DELTA_ಗಾತ್ರ(trg_ನಮೂದು) &&
		    src->ಆಳ + 1 >= trg->ಆಳ) {
			ಬಿಡುಗಡೆ(delta_buf);
			ಹಿಂತಿರುಗಿ 0;
		}
	}

	/*
	 * Handle memory allocation outside of the cache
	 * accounting lock.  Compiler will optimize the strangeness
	 * away when NO_PTHREADS is defined.
	 */
	ಬಿಡುಗಡೆ(trg_ನಮೂದು->delta_ದತ್ತಾಂಶ);
	cache_ಬೀಗ();
	ಆದರೆ (trg_ನಮೂದು->delta_ದತ್ತಾಂಶ) {
		delta_cache_ಗಾತ್ರ -= DELTA_ಗಾತ್ರ(trg_ನಮೂದು);
		trg_ನಮೂದು->delta_ದತ್ತಾಂಶ = ಖಾಲಿ;
	}
	ಆದರೆ (delta_cacheable(src_ಗಾತ್ರ, trg_ಗಾತ್ರ, delta_ಗಾತ್ರ_2)) {
		delta_cache_ಗಾತ್ರ += delta_ಗಾತ್ರ_2;
		cache_unlock();
		trg_ನಮೂದು->delta_ದತ್ತಾಂಶ = xrealloc(delta_buf, delta_ಗಾತ್ರ_2);
	} ಇಲ್ಲದಿದ್ದರೆ {
		cache_unlock();
		ಬಿಡುಗಡೆ(delta_buf);
	}

	ಗಣ_DELTA(trg_ನಮೂದು, src_ನಮೂದು);
	ಗಣ_DELTA_ಗಾತ್ರ(trg_ನಮೂದು, delta_ಗಾತ್ರ_2);
	trg->ಆಳ = src->ಆಳ + 1;

	ಹಿಂತಿರುಗಿ 1;
}

ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ check_delta_ಎಲ್ಲೆ(ರಚನೆ object_ನಮೂದು *me, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ n)
{
	ರಚನೆ object_ನಮೂದು *ಮಗು = DELTA_ಮಗು(me);
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ m = n;
	ಆಗ (ಮಗು) {
		ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ c = check_delta_ಎಲ್ಲೆ(ಮಗು, n + 1);
		ಆದರೆ (m < c)
			m = c;
		ಮಗು = DELTA_SIBLING(ಮಗು);
	}
	ಹಿಂತಿರುಗಿ m;
}

ಸ್ಥಿರ ಗಾತ್ರ_t free_unpacked(ರಚನೆ unpacked *n)
{
	ಗಾತ್ರ_t freed_mem = sizeof_delta_ಸೂಚ್ಯಂಕ(n->ಸೂಚ್ಯಂಕ);
	free_delta_ಸೂಚ್ಯಂಕ(n->ಸೂಚ್ಯಂಕ);
	n->ಸೂಚ್ಯಂಕ = ಖಾಲಿ;
	ಆದರೆ (n->ದತ್ತಾಂಶ) {
		freed_mem += ಗಾತ್ರ_4(n->ನಮೂದು);
		FREE_AND_NULL(n->ದತ್ತಾಂಶ);
	}
	n->ನಮೂದು = ಖಾಲಿ;
	n->ಆಳ = 0;
	ಹಿಂತಿರುಗಿ freed_mem;
}

ಸ್ಥಿರ ಶೂನ್ಯ ಕಂಡುಹಿಡಿ_deltas(ರಚನೆ object_ನಮೂದು **list, ಚಿಹ್ನೆರಹಿತ *list_ಗಾತ್ರ,
			ಪೂರ್ಣಾಂಕ ಕಿಟಕಿ, ಪೂರ್ಣಾಂಕ ಆಳ, ಚಿಹ್ನೆರಹಿತ *processed)
{
	uint32_t i, idx = 0, ಎಣಿಕೆ_2 = 0;
	ರಚನೆ unpacked *array;
	ಗಾತ್ರ_t mem_usage = 0;

	CALLOC_ARRAY(array, ಕಿಟಕಿ);

	ಗಾಗಿ (;;) {
		ರಚನೆ object_ನಮೂದು *ನಮೂದು;
		ರಚನೆ unpacked *n = array + idx;
		ಪೂರ್ಣಾಂಕ j, max_ಆಳ, ಉತ್ತಮ_ತಳ = -1;

		ಪ್ರಗತಿ_ಬೀಗ();
		ಆದರೆ (!*list_ಗಾತ್ರ) {
			ಪ್ರಗತಿ_unlock();
			ನಿಲ್ಲಿಸು;
		}
		ನಮೂದು = *list++;
		(*list_ಗಾತ್ರ)--;
		ಆದರೆ (!ನಮೂದು->preferred_ತಳ) {
			(*processed)++;
			display_ಪ್ರಗತಿ(ಪ್ರಗತಿ_state, *processed);
		}
		ಪ್ರಗತಿ_unlock();

		mem_usage -= free_unpacked(n);
		n->ನಮೂದು = ನಮೂದು;

		ಆಗ (ಕಿಟಕಿ_ಸ್ಮರಣೆ_ಎಲ್ಲೆ &&
		       mem_usage > ಕಿಟಕಿ_ಸ್ಮರಣೆ_ಎಲ್ಲೆ &&
		       ಎಣಿಕೆ_2 > 1) {
			ಸ್ಥಿರಾಂಕ uint32_t ಬಾಲ = (idx + ಕಿಟಕಿ - ಎಣಿಕೆ_2) % ಕಿಟಕಿ;
			mem_usage -= free_unpacked(array + ಬಾಲ);
			ಎಣಿಕೆ_2--;
		}

		/* We do not compute delta to *create* objects we are not
		 * going to pack.
		 */
		ಆದರೆ (ನಮೂದು->preferred_ತಳ)
			ಹೋಗು ಮುಂದಿನ;

		/*
		 * If the current object is at pack edge, take the depth the
		 * objects that depend on the current object into account
		 * otherwise they would become too deep.
		 */
		max_ಆಳ = ಆಳ;
		ಆದರೆ (DELTA_ಮಗು(ನಮೂದು)) {
			max_ಆಳ -= check_delta_ಎಲ್ಲೆ(ನಮೂದು, 0);
			ಆದರೆ (max_ಆಳ <= 0)
				ಹೋಗು ಮುಂದಿನ;
		}

		j = ಕಿಟಕಿ;
		ಆಗ (--j > 0) {
			ಪೂರ್ಣಾಂಕ ret;
			uint32_t other_idx = idx + j;
			ರಚನೆ unpacked *m;
			ಆದರೆ (other_idx >= ಕಿಟಕಿ)
				other_idx -= ಕಿಟಕಿ;
			m = array + other_idx;
			ಆದರೆ (!m->ನಮೂದು)
				ನಿಲ್ಲಿಸು;
			ret = try_delta(n, m, max_ಆಳ, &mem_usage);
			ಆದರೆ (ret < 0)
				ನಿಲ್ಲಿಸು;
			ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ret > 0)
				ಉತ್ತಮ_ತಳ = other_idx;
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
		ಆದರೆ (ನಮೂದು->delta_ದತ್ತಾಂಶ && !pack_ಗಮ್ಯ_stdout) {
			ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಗಾತ್ರ_2;

			ಗಾತ್ರ_2 = do_compress(&ನಮೂದು->delta_ದತ್ತಾಂಶ, DELTA_ಗಾತ್ರ(ನಮೂದು));
			ಆದರೆ (ಗಾತ್ರ_2 < (1U << OE_Z_DELTA_ಬಿಟ್ಟುಗಳು)) {
				ನಮೂದು->z_delta_ಗಾತ್ರ = ಗಾತ್ರ_2;
				cache_ಬೀಗ();
				delta_cache_ಗಾತ್ರ -= DELTA_ಗಾತ್ರ(ನಮೂದು);
				delta_cache_ಗಾತ್ರ += ನಮೂದು->z_delta_ಗಾತ್ರ;
				cache_unlock();
			} ಇಲ್ಲದಿದ್ದರೆ {
				FREE_AND_NULL(ನಮೂದು->delta_ದತ್ತಾಂಶ);
				ನಮೂದು->z_delta_ಗಾತ್ರ = 0;
			}
		}

		/* if we made n a delta, and if n is already at max
		 * depth, leaving it in the window is pointless.  we
		 * should evict it first.
		 */
		ಆದರೆ (DELTA(ನಮೂದು) && max_ಆಳ <= n->ಆಳ)
			ಮುಂದುವರಿ;

		/*
		 * Move the best delta base up in the window, after the
		 * currently deltified object, to keep it longer.  It will
		 * be the first base object to be attempted next.
		 */
		ಆದರೆ (DELTA(ನಮೂದು)) {
			ರಚನೆ unpacked ಅದಲುಬದಲು_2 = array[ಉತ್ತಮ_ತಳ];
			ಪೂರ್ಣಾಂಕ dist = (ಕಿಟಕಿ + idx - ಉತ್ತಮ_ತಳ) % ಕಿಟಕಿ;
			ಪೂರ್ಣಾಂಕ dst = ಉತ್ತಮ_ತಳ;
			ಆಗ (dist--) {
				ಪೂರ್ಣಾಂಕ src = (dst + 1) % ಕಿಟಕಿ;
				array[dst] = array[src];
				dst = src;
			}
			array[dst] = ಅದಲುಬದಲು_2;
		}

		ಮುಂದಿನ:
		idx++;
		ಆದರೆ (ಎಣಿಕೆ_2 + 1 < ಕಿಟಕಿ)
			ಎಣಿಕೆ_2++;
		ಆದರೆ (idx >= ಕಿಟಕಿ)
			idx = 0;
	}

	ಗಾಗಿ (i = 0; i < ಕಿಟಕಿ; ++i) {
		free_delta_ಸೂಚ್ಯಂಕ(array[i].ಸೂಚ್ಯಂಕ);
		ಬಿಡುಗಡೆ(array[i].ದತ್ತಾಂಶ);
	}
	ಬಿಡುಗಡೆ(array);
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

ರಚನೆ thread_params {
	pthread_t thread;
	ರಚನೆ object_ನಮೂದು **list;
	ರಚನೆ packing_region *regions;
	ಚಿಹ್ನೆರಹಿತ list_ಗಾತ್ರ;
	ಚಿಹ್ನೆರಹಿತ ಉಳಿದ;
	ಪೂರ್ಣಾಂಕ ಕಿಟಕಿ;
	ಪೂರ್ಣಾಂಕ ಆಳ;
	ಪೂರ್ಣಾಂಕ working;
	ಪೂರ್ಣಾಂಕ ದತ್ತಾಂಶ_ready;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	ಚಿಹ್ನೆರಹಿತ *processed;
};

ಸ್ಥಿರ pthread_cond_t ಪ್ರಗತಿ_cond;

/*
 * Mutex and conditional variable can't be statically-initialized on Windows.
 */
ಸ್ಥಿರ ಶೂನ್ಯ init_threaded_ಹುಡುಕು(ಶೂನ್ಯ)
{
	pthread_mutex_init(&cache_mutex, ಖಾಲಿ);
	pthread_mutex_init(&ಪ್ರಗತಿ_mutex, ಖಾಲಿ);
	pthread_cond_init(&ಪ್ರಗತಿ_cond, ಖಾಲಿ);
}

ಸ್ಥಿರ ಶೂನ್ಯ cleanup_threaded_ಹುಡುಕು(ಶೂನ್ಯ)
{
	pthread_cond_destroy(&ಪ್ರಗತಿ_cond);
	pthread_mutex_destroy(&cache_mutex);
	pthread_mutex_destroy(&ಪ್ರಗತಿ_mutex);
}

ಸ್ಥಿರ ಶೂನ್ಯ *threaded_ಕಂಡುಹಿಡಿ_deltas(ಶೂನ್ಯ *arg)
{
	ರಚನೆ thread_params *me = arg;

	ಪ್ರಗತಿ_ಬೀಗ();
	ಆಗ (me->ಉಳಿದ) {
		ಪ್ರಗತಿ_unlock();

		ಕಂಡುಹಿಡಿ_deltas(me->list, &me->ಉಳಿದ,
			    me->ಕಿಟಕಿ, me->ಆಳ, me->processed);

		ಪ್ರಗತಿ_ಬೀಗ();
		me->working = 0;
		pthread_cond_signal(&ಪ್ರಗತಿ_cond);
		ಪ್ರಗತಿ_unlock();

		/*
		 * We must not set ->data_ready before we wait on the
		 * condition because the main thread may have set it to 1
		 * before we get here. In order to be sure that new
		 * work is available if we see 1 in ->data_ready, it
		 * was initialized to 0 before this thread was spawned
		 * and we reset it to 0 right away.
		 */
		pthread_mutex_ಬೀಗ(&me->mutex);
		ಆಗ (!me->ದತ್ತಾಂಶ_ready)
			pthread_cond_wait(&me->cond, &me->mutex);
		me->ದತ್ತಾಂಶ_ready = 0;
		pthread_mutex_unlock(&me->mutex);

		ಪ್ರಗತಿ_ಬೀಗ();
	}
	ಪ್ರಗತಿ_unlock();
	/* leave ->working 1 so that this doesn't get more work assigned */
	ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

ಸ್ಥಿರ ಶೂನ್ಯ ll_ಕಂಡುಹಿಡಿ_deltas(ರಚನೆ object_ನಮೂದು **list, ಚಿಹ್ನೆರಹಿತ list_ಗಾತ್ರ,
			   ಪೂರ್ಣಾಂಕ ಕಿಟಕಿ, ಪೂರ್ಣಾಂಕ ಆಳ, ಚಿಹ್ನೆರಹಿತ *processed)
{
	ರಚನೆ thread_params *p;
	ಪೂರ್ಣಾಂಕ i, ret, ಸಕ್ರಿಯ_ಎಳೆಗಳು = 0;

	init_threaded_ಹುಡುಕು();

	ಆದರೆ (delta_ಹುಡುಕು_ಎಳೆಗಳು <= 1) {
		ಕಂಡುಹಿಡಿ_deltas(list, &list_ಗಾತ್ರ, ಕಿಟಕಿ, ಆಳ, processed);
		cleanup_threaded_ಹುಡುಕು();
		ಹಿಂತಿರುಗಿ;
	}
	ಆದರೆ (ಪ್ರಗತಿ > pack_ಗಮ್ಯ_stdout)
		fprintf_ln(stderr, _("Delta compression using up to %d threads"),
			   delta_ಹುಡುಕು_ಎಳೆಗಳು);
	CALLOC_ARRAY(p, delta_ಹುಡುಕು_ಎಳೆಗಳು);

	/* Partition the work amongst work threads. */
	ಗಾಗಿ (i = 0; i < delta_ಹುಡುಕು_ಎಳೆಗಳು; i++) {
		ಚಿಹ್ನೆರಹಿತ sub_ಗಾತ್ರ = list_ಗಾತ್ರ / (delta_ಹುಡುಕು_ಎಳೆಗಳು - i);

		/* don't use too small segments or no deltas will be found */
		ಆದರೆ (sub_ಗಾತ್ರ < 2*ಕಿಟಕಿ && i+1 < delta_ಹುಡುಕು_ಎಳೆಗಳು)
			sub_ಗಾತ್ರ = 0;

		p[i].ಕಿಟಕಿ = ಕಿಟಕಿ;
		p[i].ಆಳ = ಆಳ;
		p[i].processed = processed;
		p[i].working = 1;
		p[i].ದತ್ತಾಂಶ_ready = 0;

		/* try to split chunks on "path" boundaries */
		ಆಗ (sub_ಗಾತ್ರ && sub_ಗಾತ್ರ < list_ಗಾತ್ರ &&
		       list[sub_ಗಾತ್ರ]->hash &&
		       list[sub_ಗಾತ್ರ]->hash == list[sub_ಗಾತ್ರ-1]->hash)
			sub_ಗಾತ್ರ++;

		p[i].list = list;
		p[i].list_ಗಾತ್ರ = sub_ಗಾತ್ರ;
		p[i].ಉಳಿದ = sub_ಗಾತ್ರ;

		list += sub_ಗಾತ್ರ;
		list_ಗಾತ್ರ -= sub_ಗಾತ್ರ;
	}

	/* Start work threads. */
	ಗಾಗಿ (i = 0; i < delta_ಹುಡುಕು_ಎಳೆಗಳು; i++) {
		ಆದರೆ (!p[i].list_ಗಾತ್ರ)
			ಮುಂದುವರಿ;
		pthread_mutex_init(&p[i].mutex, ಖಾಲಿ);
		pthread_cond_init(&p[i].cond, ಖಾಲಿ);
		ret = pthread_create(&p[i].thread, ಖಾಲಿ,
				     threaded_ಕಂಡುಹಿಡಿ_deltas, &p[i]);
		ಆದರೆ (ret)
			die(_("unable to create thread: %s"), strerror(ret));
		ಸಕ್ರಿಯ_ಎಳೆಗಳು++;
	}

	/*
	 * Now let's wait for work completion.  Each time a thread is done
	 * with its work, we steal half of the remaining work from the
	 * thread with the largest number of unprocessed objects and give
	 * it to that newly idle thread.  This ensure good load balancing
	 * until the remaining object list segments are simply too short
	 * to be worth splitting anymore.
	 */
	ಆಗ (ಸಕ್ರಿಯ_ಎಳೆಗಳು) {
		ರಚನೆ thread_params *ಗುರಿ = ಖಾಲಿ;
		ರಚನೆ thread_params *victim = ಖಾಲಿ;
		ಚಿಹ್ನೆರಹಿತ sub_ಗಾತ್ರ = 0;

		ಪ್ರಗತಿ_ಬೀಗ();
		ಗಾಗಿ (;;) {
			ಗಾಗಿ (i = 0; !ಗುರಿ && i < delta_ಹುಡುಕು_ಎಳೆಗಳು; i++)
				ಆದರೆ (!p[i].working)
					ಗುರಿ = &p[i];
			ಆದರೆ (ಗುರಿ)
				ನಿಲ್ಲಿಸು;
			pthread_cond_wait(&ಪ್ರಗತಿ_cond, &ಪ್ರಗತಿ_mutex);
		}

		ಗಾಗಿ (i = 0; i < delta_ಹುಡುಕು_ಎಳೆಗಳು; i++)
			ಆದರೆ (p[i].ಉಳಿದ > 2*ಕಿಟಕಿ &&
			    (!victim || victim->ಉಳಿದ < p[i].ಉಳಿದ))
				victim = &p[i];
		ಆದರೆ (victim) {
			sub_ಗಾತ್ರ = victim->ಉಳಿದ / 2;
			list = victim->list + victim->list_ಗಾತ್ರ - sub_ಗಾತ್ರ;
			ಆಗ (sub_ಗಾತ್ರ && list[0]->hash &&
			       list[0]->hash == list[-1]->hash) {
				list++;
				sub_ಗಾತ್ರ--;
			}
			ಆದರೆ (!sub_ಗಾತ್ರ) {
				/*
				 * It is possible for some "paths" to have
				 * so many objects that no hash boundary
				 * might be found.  Let's just steal the
				 * exact half in that case.
				 */
				sub_ಗಾತ್ರ = victim->ಉಳಿದ / 2;
				list -= sub_ಗಾತ್ರ;
			}
			ಗುರಿ->list = list;
			victim->list_ಗಾತ್ರ -= sub_ಗಾತ್ರ;
			victim->ಉಳಿದ -= sub_ಗಾತ್ರ;
		}
		ಗುರಿ->list_ಗಾತ್ರ = sub_ಗಾತ್ರ;
		ಗುರಿ->ಉಳಿದ = sub_ಗಾತ್ರ;
		ಗುರಿ->working = 1;
		ಪ್ರಗತಿ_unlock();

		pthread_mutex_ಬೀಗ(&ಗುರಿ->mutex);
		ಗುರಿ->ದತ್ತಾಂಶ_ready = 1;
		pthread_cond_signal(&ಗುರಿ->cond);
		pthread_mutex_unlock(&ಗುರಿ->mutex);

		ಆದರೆ (!sub_ಗಾತ್ರ) {
			pthread_join(ಗುರಿ->thread, ಖಾಲಿ);
			pthread_cond_destroy(&ಗುರಿ->cond);
			pthread_mutex_destroy(&ಗುರಿ->mutex);
			ಸಕ್ರಿಯ_ಎಳೆಗಳು--;
		}
	}
	cleanup_threaded_ಹುಡುಕು();
	ಬಿಡುಗಡೆ(p);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ obj_is_packed(ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid)
{
	ಹಿಂತಿರುಗಿ packlist_ಕಂಡುಹಿಡಿ(&ಗಮ್ಯ_pack, oid) ||
		(reuse_packfile_bitmap &&
		 bitmap_walk_ಒಳಗೊಂಡಿದೆ(bitmap_git, reuse_packfile_bitmap, oid));
}

ಸ್ಥಿರ ಶೂನ್ಯ ಕೂಡಿಸು_ಟ್ಯಾಗ್_chain(ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid)
{
	ರಚನೆ ಟ್ಯಾಗ್ *ಟ್ಯಾಗ್;

	/*
	 * We catch duplicates already in add_object_entry(), but we'd
	 * prefer to do this extra check to avoid having to parse the
	 * tag at all if we already know that it's being packed (e.g., if
	 * it was included via bitmaps, we would not have parsed it
	 * previously).
	 */
	ಆದರೆ (obj_is_packed(oid))
		ಹಿಂತಿರುಗಿ;

	ಟ್ಯಾಗ್ = lookup_ಟ್ಯಾಗ್(the_repository, oid);
	ಆಗ (1) {
		ಆದರೆ (!ಟ್ಯಾಗ್ || parse_ಟ್ಯಾಗ್(the_repository, ಟ್ಯಾಗ್) || !ಟ್ಯಾಗ್->tagged)
			die(_("unable to pack objects reachable from tag %s"),
			    oid_ಗಮ್ಯ_hex(oid));

		ಕೂಡಿಸು_object_ನಮೂದು(&ಟ್ಯಾಗ್->object.oid, OBJ_ಟ್ಯಾಗ್, ಖಾಲಿ, 0);

		ಆದರೆ (ಟ್ಯಾಗ್->tagged->type != OBJ_ಟ್ಯಾಗ್)
			ಹಿಂತಿರುಗಿ;

		ಟ್ಯಾಗ್ = (ರಚನೆ ಟ್ಯಾಗ್ *)ಟ್ಯಾಗ್->tagged;
	}
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಕೂಡಿಸು_ref_ಟ್ಯಾಗ್(ಸ್ಥಿರಾಂಕ ರಚನೆ reference *ref, ಶೂನ್ಯ *cb_ದತ್ತಾಂಶ ಬಳಸದ)
{
	ರಚನೆ object_id peeled;

	ಆದರೆ (!reference_get_peeled_oid(the_repository, ref, &peeled) &&
	    obj_is_packed(&peeled))
		ಕೂಡಿಸು_ಟ್ಯಾಗ್_chain(ref->oid);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ should_attempt_deltas(ರಚನೆ object_ನಮೂದು *ನಮೂದು)
{
	ಆದರೆ (DELTA(ನಮೂದು))
		/* This happens if we decided to reuse existing
		 * delta from a pack. "reuse_delta &&" is implied.
		 */
		ಹಿಂತಿರುಗಿ 0;

	ಆದರೆ (!ನಮೂದು->type_valid ||
	    oe_ಗಾತ್ರ_less_than(&ಗಮ್ಯ_pack, ನಮೂದು, 50))
		ಹಿಂತಿರುಗಿ 0;

	ಆದರೆ (ನಮೂದು->no_try_delta)
		ಹಿಂತಿರುಗಿ 0;

	ಆದರೆ (!ನಮೂದು->preferred_ತಳ) {
		ಆದರೆ (oe_type(ನಮೂದು) < 0)
			die(_("unable to get type of object %s"),
				oid_ಗಮ್ಯ_hex(&ನಮೂದು->idx.oid));
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (oe_type(ನಮೂದು) < 0) {
		/*
		 * This object is not found, but we
		 * don't have to include it anyway.
		 */
		ಹಿಂತಿರುಗಿ 0;
	}

	ಹಿಂತಿರುಗಿ 1;
}

ಸ್ಥಿರ ಶೂನ್ಯ ಕಂಡುಹಿಡಿ_deltas_for_region(ರಚನೆ object_ನಮೂದು *list,
				   ರಚನೆ packing_region *region,
				   ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ *processed)
{
	ರಚನೆ object_ನಮೂದು **delta_list;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ delta_list_nr = 0;

	ALLOC_ARRAY(delta_list, region->nr);
	ಗಾಗಿ (ಗಾತ್ರ_t i = 0; i < region->nr; i++) {
		ರಚನೆ object_ನಮೂದು *ನಮೂದು = list + region->ಆರಂಭ + i;
		ಆದರೆ (should_attempt_deltas(ನಮೂದು))
			delta_list[delta_list_nr++] = ನಮೂದು;
	}

	QSORT(delta_list, delta_list_nr, type_ಗಾತ್ರ_ವಿಂಗಡಿಸು);
	ಕಂಡುಹಿಡಿ_deltas(delta_list, &delta_list_nr, ಕಿಟಕಿ, ಆಳ, processed);
	ಬಿಡುಗಡೆ(delta_list);
}

ಸ್ಥಿರ ಶೂನ್ಯ ಕಂಡುಹಿಡಿ_deltas_by_region(ರಚನೆ object_ನಮೂದು *list,
				  ರಚನೆ packing_region *regions,
				  ಗಾತ್ರ_t ಆರಂಭ, ಗಾತ್ರ_t nr)
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ processed = 0;
	ಗಾತ್ರ_t ಪ್ರಗತಿ_nr;

	ಆದರೆ (!nr)
		ಹಿಂತಿರುಗಿ;

	ಪ್ರಗತಿ_nr = regions[nr - 1].ಆರಂಭ + regions[nr - 1].nr;

	ಆದರೆ (ಪ್ರಗತಿ)
		ಪ್ರಗತಿ_state = ಆರಂಭ_ಪ್ರಗತಿ(the_repository,
						_("Compressing objects by path"),
						ಪ್ರಗತಿ_nr);

	ಆಗ (nr--)
		ಕಂಡುಹಿಡಿ_deltas_for_region(list,
				       &regions[ಆರಂಭ++],
				       &processed);

	display_ಪ್ರಗತಿ(ಪ್ರಗತಿ_state, ಪ್ರಗತಿ_nr);
	stop_ಪ್ರಗತಿ(&ಪ್ರಗತಿ_state);
}

ಸ್ಥಿರ ಶೂನ್ಯ *threaded_ಕಂಡುಹಿಡಿ_deltas_by_ಮಾರ್ಗ(ಶೂನ್ಯ *arg)
{
	ರಚನೆ thread_params *me = arg;

	ಪ್ರಗತಿ_ಬೀಗ();
	ಆಗ (me->ಉಳಿದ) {
		ಆಗ (me->ಉಳಿದ) {
			ಪ್ರಗತಿ_unlock();
			ಕಂಡುಹಿಡಿ_deltas_for_region(ಗಮ್ಯ_pack.objects,
					       me->regions,
					       me->processed);
			ಪ್ರಗತಿ_ಬೀಗ();
			me->ಉಳಿದ--;
			me->regions++;
		}

		me->working = 0;
		pthread_cond_signal(&ಪ್ರಗತಿ_cond);
		ಪ್ರಗತಿ_unlock();

		/*
		 * We must not set ->data_ready before we wait on the
		 * condition because the main thread may have set it to 1
		 * before we get here. In order to be sure that new
		 * work is available if we see 1 in ->data_ready, it
		 * was initialized to 0 before this thread was spawned
		 * and we reset it to 0 right away.
		 */
		pthread_mutex_ಬೀಗ(&me->mutex);
		ಆಗ (!me->ದತ್ತಾಂಶ_ready)
			pthread_cond_wait(&me->cond, &me->mutex);
		me->ದತ್ತಾಂಶ_ready = 0;
		pthread_mutex_unlock(&me->mutex);

		ಪ್ರಗತಿ_ಬೀಗ();
	}
	ಪ್ರಗತಿ_unlock();
	/* leave ->working 1 so that this doesn't get more work assigned */
	ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

ಸ್ಥಿರ ಶೂನ್ಯ ll_ಕಂಡುಹಿಡಿ_deltas_by_region(ರಚನೆ object_ನಮೂದು *list,
				     ರಚನೆ packing_region *regions,
				     uint32_t ಆರಂಭ, uint32_t nr)
{
	ರಚನೆ thread_params *p;
	ಪೂರ್ಣಾಂಕ i, ret, ಸಕ್ರಿಯ_ಎಳೆಗಳು = 0;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ processed = 0;
	uint32_t ಪ್ರಗತಿ_nr;
	init_threaded_ಹುಡುಕು();

	ಆದರೆ (!nr)
		ಹಿಂತಿರುಗಿ;

	ಪ್ರಗತಿ_nr =  regions[nr - 1].ಆರಂಭ + regions[nr - 1].nr;
	ಆದರೆ (delta_ಹುಡುಕು_ಎಳೆಗಳು <= 1) {
		ಕಂಡುಹಿಡಿ_deltas_by_region(list, regions, ಆರಂಭ, nr);
		cleanup_threaded_ಹುಡುಕು();
		ಹಿಂತಿರುಗಿ;
	}

	ಆದರೆ (ಪ್ರಗತಿ > pack_ಗಮ್ಯ_stdout)
		fprintf_ln(stderr,
			   Q_("Path-based delta compression using up to %d thread",
			      "Path-based delta compression using up to %d threads",
			      delta_ಹುಡುಕು_ಎಳೆಗಳು),
			   delta_ಹುಡುಕು_ಎಳೆಗಳು);
	CALLOC_ARRAY(p, delta_ಹುಡುಕು_ಎಳೆಗಳು);

	ಆದರೆ (ಪ್ರಗತಿ)
		ಪ್ರಗತಿ_state = ಆರಂಭ_ಪ್ರಗತಿ(the_repository,
						_("Compressing objects by path"),
						ಪ್ರಗತಿ_nr);
	/* Partition the work amongst work threads. */
	ಗಾಗಿ (i = 0; i < delta_ಹುಡುಕು_ಎಳೆಗಳು; i++) {
		ಚಿಹ್ನೆರಹಿತ sub_ಗಾತ್ರ = nr / (delta_ಹುಡುಕು_ಎಳೆಗಳು - i);

		p[i].ಕಿಟಕಿ = ಕಿಟಕಿ;
		p[i].ಆಳ = ಆಳ;
		p[i].processed = &processed;
		p[i].working = 1;
		p[i].ದತ್ತಾಂಶ_ready = 0;

		p[i].regions = regions;
		p[i].list_ಗಾತ್ರ = sub_ಗಾತ್ರ;
		p[i].ಉಳಿದ = sub_ಗಾತ್ರ;

		regions += sub_ಗಾತ್ರ;
		nr -= sub_ಗಾತ್ರ;
	}

	/* Start work threads. */
	ಗಾಗಿ (i = 0; i < delta_ಹುಡುಕು_ಎಳೆಗಳು; i++) {
		ಆದರೆ (!p[i].list_ಗಾತ್ರ)
			ಮುಂದುವರಿ;
		pthread_mutex_init(&p[i].mutex, ಖಾಲಿ);
		pthread_cond_init(&p[i].cond, ಖಾಲಿ);
		ret = pthread_create(&p[i].thread, ಖಾಲಿ,
				     threaded_ಕಂಡುಹಿಡಿ_deltas_by_ಮಾರ್ಗ, &p[i]);
		ಆದರೆ (ret)
			die(_("unable to create thread: %s"), strerror(ret));
		ಸಕ್ರಿಯ_ಎಳೆಗಳು++;
	}

	/*
	 * Now let's wait for work completion.  Each time a thread is done
	 * with its work, we steal half of the remaining work from the
	 * thread with the largest number of unprocessed objects and give
	 * it to that newly idle thread.  This ensure good load balancing
	 * until the remaining object list segments are simply too short
	 * to be worth splitting anymore.
	 */
	ಆಗ (ಸಕ್ರಿಯ_ಎಳೆಗಳು) {
		ರಚನೆ thread_params *ಗುರಿ = ಖಾಲಿ;
		ರಚನೆ thread_params *victim = ಖಾಲಿ;
		ಚಿಹ್ನೆರಹಿತ sub_ಗಾತ್ರ = 0;

		ಪ್ರಗತಿ_ಬೀಗ();
		ಗಾಗಿ (;;) {
			ಗಾಗಿ (i = 0; !ಗುರಿ && i < delta_ಹುಡುಕು_ಎಳೆಗಳು; i++)
				ಆದರೆ (!p[i].working)
					ಗುರಿ = &p[i];
			ಆದರೆ (ಗುರಿ)
				ನಿಲ್ಲಿಸು;
			pthread_cond_wait(&ಪ್ರಗತಿ_cond, &ಪ್ರಗತಿ_mutex);
		}

		ಗಾಗಿ (i = 0; i < delta_ಹುಡುಕು_ಎಳೆಗಳು; i++)
			ಆದರೆ (p[i].ಉಳಿದ > 2*ಕಿಟಕಿ &&
			    (!victim || victim->ಉಳಿದ < p[i].ಉಳಿದ))
				victim = &p[i];
		ಆದರೆ (victim) {
			sub_ಗಾತ್ರ = victim->ಉಳಿದ / 2;
			ಗುರಿ->regions = victim->regions + victim->ಉಳಿದ - sub_ಗಾತ್ರ;
			victim->list_ಗಾತ್ರ -= sub_ಗಾತ್ರ;
			victim->ಉಳಿದ -= sub_ಗಾತ್ರ;
		}
		ಗುರಿ->list_ಗಾತ್ರ = sub_ಗಾತ್ರ;
		ಗುರಿ->ಉಳಿದ = sub_ಗಾತ್ರ;
		ಗುರಿ->working = 1;
		ಪ್ರಗತಿ_unlock();

		pthread_mutex_ಬೀಗ(&ಗುರಿ->mutex);
		ಗುರಿ->ದತ್ತಾಂಶ_ready = 1;
		pthread_cond_signal(&ಗುರಿ->cond);
		pthread_mutex_unlock(&ಗುರಿ->mutex);

		ಆದರೆ (!sub_ಗಾತ್ರ) {
			pthread_join(ಗುರಿ->thread, ಖಾಲಿ);
			pthread_cond_destroy(&ಗುರಿ->cond);
			pthread_mutex_destroy(&ಗುರಿ->mutex);
			ಸಕ್ರಿಯ_ಎಳೆಗಳು--;
		}
	}
	cleanup_threaded_ಹುಡುಕು();
	ಬಿಡುಗಡೆ(p);

	display_ಪ್ರಗತಿ(ಪ್ರಗತಿ_state, ಪ್ರಗತಿ_nr);
	stop_ಪ್ರಗತಿ(&ಪ್ರಗತಿ_state);
}

ಸ್ಥಿರ ಶೂನ್ಯ prepare_pack(ಪೂರ್ಣಾಂಕ ಕಿಟಕಿ, ಪೂರ್ಣಾಂಕ ಆಳ)
{
	ರಚನೆ object_ನಮೂದು **delta_list;
	uint32_t i, nr_deltas;
	ಚಿಹ್ನೆರಹಿತ n;

	ಆದರೆ (use_delta_islands)
		resolve_ವೃಕ್ಷ_islands(the_repository, ಪ್ರಗತಿ, &ಗಮ್ಯ_pack);

	get_object_details();

	/*
	 * If we're locally repacking then we need to be doubly careful
	 * from now on in order to make sure no stealth corruption gets
	 * propagated to the new pack.  Clients receiving streamed packs
	 * should validate everything they get anyway so no need to incur
	 * the additional cost here in that case.
	 */
	ಆದರೆ (!pack_ಗಮ್ಯ_stdout)
		do_check_packed_object_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ = 1;

	ಆದರೆ (!ಗಮ್ಯ_pack.nr_objects || !ಕಿಟಕಿ || !ಆಳ)
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (ಮಾರ್ಗ_walk)
		ll_ಕಂಡುಹಿಡಿ_deltas_by_region(ಗಮ್ಯ_pack.objects, ಗಮ್ಯ_pack.regions,
					 0, ಗಮ್ಯ_pack.nr_regions);

	ALLOC_ARRAY(delta_list, ಗಮ್ಯ_pack.nr_objects);
	nr_deltas = n = 0;

	ಗಾಗಿ (i = 0; i < ಗಮ್ಯ_pack.nr_objects; i++) {
		ರಚನೆ object_ನಮೂದು *ನಮೂದು = ಗಮ್ಯ_pack.objects + i;

		ಆದರೆ (!should_attempt_deltas(ನಮೂದು))
			ಮುಂದುವರಿ;

		ಆದರೆ (!ನಮೂದು->preferred_ತಳ)
			nr_deltas++;

		delta_list[n++] = ನಮೂದು;
	}

	ಆದರೆ (nr_deltas && n > 1) {
		ಚಿಹ್ನೆರಹಿತ nr_ಮುಗಿದ = 0;

		ಆದರೆ (ಪ್ರಗತಿ)
			ಪ್ರಗತಿ_state = ಆರಂಭ_ಪ್ರಗತಿ(the_repository,
							_("Compressing objects"),
							nr_deltas);
		QSORT(delta_list, n, type_ಗಾತ್ರ_ವಿಂಗಡಿಸು);
		ll_ಕಂಡುಹಿಡಿ_deltas(delta_list, n, ಕಿಟಕಿ+1, ಆಳ, &nr_ಮುಗಿದ);
		stop_ಪ್ರಗತಿ(&ಪ್ರಗತಿ_state);
		ಆದರೆ (nr_ಮುಗಿದ != nr_deltas)
			die(_("inconsistency with delta count"));
	}
	ಬಿಡುಗಡೆ(delta_list);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ git_pack_config(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *k, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *v,
			   ಸ್ಥಿರಾಂಕ ರಚನೆ config_context *ctx, ಶೂನ್ಯ *cb)
{
	ಆದರೆ (!strcmp(k, "pack.window")) {
		ಕಿಟಕಿ = git_config_int(k, v, ctx->kvi);
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (!strcmp(k, "pack.windowmemory")) {
		ಕಿಟಕಿ_ಸ್ಮರಣೆ_ಎಲ್ಲೆ = git_config_ulong(k, v, ctx->kvi);
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (!strcmp(k, "pack.depth")) {
		ಆಳ = git_config_int(k, v, ctx->kvi);
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (!strcmp(k, "pack.deltacachesize")) {
		max_delta_cache_ಗಾತ್ರ = git_config_ಗಾತ್ರ_t(k, v, ctx->kvi);
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (!strcmp(k, "pack.deltacachelimit")) {
		cache_max_small_delta_ಗಾತ್ರ = git_config_int(k, v, ctx->kvi);
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (!strcmp(k, "pack.writebitmaphashcache")) {
		ಆದರೆ (git_config_bool(k, v))
			write_bitmap_options |= BITMAP_OPT_HASH_CACHE;
		ಇಲ್ಲದಿದ್ದರೆ
			write_bitmap_options &= ~BITMAP_OPT_HASH_CACHE;
	}

	ಆದರೆ (!strcmp(k, "pack.writebitmaplookuptable")) {
		ಆದರೆ (git_config_bool(k, v))
			write_bitmap_options |= BITMAP_OPT_LOOKUP_ಕೋಷ್ಟಕ;
		ಇಲ್ಲದಿದ್ದರೆ
			write_bitmap_options &= ~BITMAP_OPT_LOOKUP_ಕೋಷ್ಟಕ;
	}

	ಆದರೆ (!strcmp(k, "pack.usebitmaps")) {
		use_bitmap_ಸೂಚ್ಯಂಕ_default = git_config_bool(k, v);
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (!strcmp(k, "pack.allowpackreuse")) {
		ಪೂರ್ಣಾಂಕ res = git_parse_maybe_bool_ಪಠ್ಯ(v);
		ಆದರೆ (res < 0) {
			ಆದರೆ (!strcasecmp(v, "single"))
				allow_pack_reuse = SINGLE_PACK_REUSE;
			ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcasecmp(v, "multi"))
				allow_pack_reuse = MULTI_PACK_REUSE;
			ಇಲ್ಲದಿದ್ದರೆ
				die(_("invalid pack.allowPackReuse value: '%s'"), v);
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (res) {
			allow_pack_reuse = SINGLE_PACK_REUSE;
		} ಇಲ್ಲದಿದ್ದರೆ {
			allow_pack_reuse = NO_PACK_REUSE;
		}
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (!strcmp(k, "pack.threads")) {
		delta_ಹುಡುಕು_ಎಳೆಗಳು = git_config_int(k, v, ctx->kvi);
		ಆದರೆ (delta_ಹುಡುಕು_ಎಳೆಗಳು < 0)
			die(_("invalid number of threads specified (%d)"),
			    delta_ಹುಡುಕು_ಎಳೆಗಳು);
		ಆದರೆ (!HAVE_ಎಳೆಗಳು && delta_ಹುಡುಕು_ಎಳೆಗಳು != 1) {
			warning(_("no threads support, ignoring %s"), k);
			delta_ಹುಡುಕು_ಎಳೆಗಳು = 0;
		}
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (!strcmp(k, "pack.indexversion")) {
		pack_idx_opts.ಆವೃತ್ತಿ = git_config_int(k, v, ctx->kvi);
		ಆದರೆ (pack_idx_opts.ಆವೃತ್ತಿ > 2)
			die(_("bad pack.indexVersion=%"PRIu32),
			    pack_idx_opts.ಆವೃತ್ತಿ);
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (!strcmp(k, "pack.writereverseindex")) {
		ಆದರೆ (git_config_bool(k, v))
			pack_idx_opts.flags |= WRITE_REV;
		ಇಲ್ಲದಿದ್ದರೆ
			pack_idx_opts.flags &= ~WRITE_REV;
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (!strcmp(k, "uploadpack.blobpackfileuri")) {
		ರಚನೆ configured_exclusion *ex;
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *oid_ಕೊನೆ, *pack_ಕೊನೆ;
		/*
		 * Stores the pack hash. This is not a true object ID, but is
		 * of the same form.
		 */
		ರಚನೆ object_id pack_hash;

		ಆದರೆ (!v)
			ಹಿಂತಿರುಗಿ config_error_nonbool(k);

		ex = xmalloc(ಗಾತ್ರ(*ex));
		ಆದರೆ (parse_oid_hex(v, &ex->e.oid, &oid_ಕೊನೆ) ||
		    *oid_ಕೊನೆ != ' ' ||
		    parse_oid_hex(oid_ಕೊನೆ + 1, &pack_hash, &pack_ಕೊನೆ) ||
		    *pack_ಕೊನೆ != ' ')
			die(_("value of uploadpack.blobpackfileuri must be "
			      "of the form '<object-hash> <pack-hash> <uri>' (got '%s')"), v);
		ಆದರೆ (oidmap_get(&configured_exclusions, &ex->e.oid))
			die(_("object already configured in another "
			      "uploadpack.blobpackfileuri (got '%s')"), v);
		ex->pack_hash_hex = xcalloc(1, pack_ಕೊನೆ - oid_ಕೊನೆ);
		ಸ್ಮರಣೆ_ನಕಲು(ex->pack_hash_hex, oid_ಕೊನೆ + 1, pack_ಕೊನೆ - oid_ಕೊನೆ - 1);
		ex->uri = xstrdup(pack_ಕೊನೆ + 1);
		oidmap_put(&configured_exclusions, ex);
	}
	ಹಿಂತಿರುಗಿ git_default_config(k, v, ctx, cb);
}

/* Counters for trace2 output when in --stdin-packs mode. */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stdin_packs_ಸಿಕ್ಕಿತು_nr;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stdin_packs_hints_nr;

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಕೂಡಿಸು_object_ನಮೂದು_from_pack(ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid,
				      ರಚನೆ packed_git *p,
				      uint32_t pos,
				      ಶೂನ್ಯ *ದತ್ತಾಂಶ_2)
{
	off_t ofs;
	ರಚನೆ object_info oi = OBJECT_INFO_INIT;
	ಎಣಿಕೆ object_type type = OBJ_NONE;

	display_ಪ್ರಗತಿ(ಪ್ರಗತಿ_state, ++nr_ನೋಡಿದ);

	ಆದರೆ (have_duplicate_ನಮೂದು(oid, 0))
		ಹಿಂತಿರುಗಿ 0;

	stdin_packs_ಸಿಕ್ಕಿತು_nr++;

	ofs = nth_packed_object_ಸ್ಥಾನಾಂತರ(p, pos);

	oi.typep = &type;
	ಆದರೆ (packed_object_info(ಖಾಲಿ, p, ofs, &oi) < 0) {
		die(_("could not get type of object %s in pack %s"),
		    oid_ಗಮ್ಯ_hex(oid), p->pack_name);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (type == OBJ_COMMIT) {
		ರಚನೆ rev_info *revs = ದತ್ತಾಂಶ_2;
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
		ಕೂಡಿಸು_pending_oid(revs, ಖಾಲಿ, oid, 0);
	}

	ಆದರೆ (!want_object_ಒಳ_pack(oid, 0, &p, &ofs))
		ಹಿಂತಿರುಗಿ 0;

	create_object_ನಮೂದು(oid, type, 0, 0, 0, p, ofs);

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ show_object_pack_hint(ರಚನೆ object *object, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name,
				  ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
	ಎಣಿಕೆ stdin_packs_ಬಹುಲಕ ಬಹುಲಕ = *(ಎಣಿಕೆ stdin_packs_ಬಹುಲಕ *)ದತ್ತಾಂಶ;
	ಆದರೆ (ಬಹುಲಕ == STDIN_PACKS_ಬಹುಲಕ_FOLLOW) {
		ಆದರೆ (object->type == OBJ_BLOB &&
		    !odb_has_object(the_repository->objects, &object->oid, 0))
			ಹಿಂತಿರುಗಿ;
		ಕೂಡಿಸು_object_ನಮೂದು(&object->oid, object->type, name, 0);
	} ಇಲ್ಲದಿದ್ದರೆ {
		ರಚನೆ object_ನಮೂದು *oe = packlist_ಕಂಡುಹಿಡಿ(&ಗಮ್ಯ_pack, &object->oid);
		ಆದರೆ (!oe)
			ಹಿಂತಿರುಗಿ;

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

ಸ್ಥಿರ ಶೂನ್ಯ show_commit_pack_hint(ರಚನೆ commit *commit, ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
	ಎಣಿಕೆ stdin_packs_ಬಹುಲಕ ಬಹುಲಕ = *(ಎಣಿಕೆ stdin_packs_ಬಹುಲಕ *)ದತ್ತಾಂಶ;

	ಆದರೆ (ಬಹುಲಕ == STDIN_PACKS_ಬಹುಲಕ_FOLLOW) {
		show_object_pack_hint((ರಚನೆ object *)commit, "", ದತ್ತಾಂಶ);
		ಹಿಂತಿರುಗಿ;
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
ಎಣಿಕೆ stdin_pack_info_ಬಗೆ {
	STDIN_PACK_INCLUDE = (1<<0),
	STDIN_PACK_EXCLUDE_CLOSED = (1<<1),
	STDIN_PACK_EXCLUDE_OPEN = (1<<2),
};

ರಚನೆ stdin_pack_info {
	ರಚನೆ packed_git *p;
	ಎಣಿಕೆ stdin_pack_info_ಬಗೆ ಬಗೆ;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ pack_mtime_cmp(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *_a, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *_b)
{
	ರಚನೆ stdin_pack_info *a = ((ಸ್ಥಿರಾಂಕ ರಚನೆ string_list_ಅಂಶ*)_a)->util;
	ರಚನೆ stdin_pack_info *b = ((ಸ್ಥಿರಾಂಕ ರಚನೆ string_list_ಅಂಶ*)_b)->util;

	/*
	 * order packs by descending mtime so that objects are laid out
	 * roughly as newest-to-oldest
	 */
	ಆದರೆ (a->p->mtime < b->p->mtime)
		ಹಿಂತಿರುಗಿ 1;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (b->p->mtime < a->p->mtime)
		ಹಿಂತಿರುಗಿ -1;
	ಇಲ್ಲದಿದ್ದರೆ
		ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stdin_packs_include_check_obj(ರಚನೆ object *obj, ಶೂನ್ಯ *ದತ್ತಾಂಶ ಬಳಸದ)
{
	ಹಿಂತಿರುಗಿ !has_object_kept_pack(ಗಮ್ಯ_pack.repo, &obj->oid,
				     KEPT_PACK_ಒಳ_CORE);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stdin_packs_include_check(ರಚನೆ commit *commit, ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
	ಹಿಂತಿರುಗಿ stdin_packs_include_check_obj((ರಚನೆ object *)commit, ದತ್ತಾಂಶ);
}

ಸ್ಥಿರ ಶೂನ್ಯ stdin_packs_ಕೂಡಿಸು_pack_entries(ರಚನೆ strmap *packs,
					 ರಚನೆ rev_info *revs)
{
	ರಚನೆ string_list ಕೀಲಿಗಳು = STRING_LIST_INIT_NODUP;
	ರಚನೆ string_list_ಅಂಶ *ಅಂಶ;
	ರಚನೆ hashmap_iter iter;
	ರಚನೆ strmap_ನಮೂದು *ನಮೂದು;

	strmap_for_each_ನಮೂದು(packs, &iter, ನಮೂದು) {
		ರಚನೆ stdin_pack_info *info = ನಮೂದು->ಮೌಲ್ಯ;
		ಆದರೆ (!info->p)
			die(_("could not find pack '%s'"), ನಮೂದು->ಕೀಲಿ);

		string_list_append(&ಕೀಲಿಗಳು, ನಮೂದು->ಕೀಲಿ)->util = info;
	}

	/*
	 * Order packs by ascending mtime; use QSORT directly to access the
	 * string_list_item's ->util pointer, which string_list_sort() does not
	 * provide.
	 */
	QSORT(ಕೀಲಿಗಳು.ವಸ್ತುಗಳು, ಕೀಲಿಗಳು.nr, pack_mtime_cmp);

	for_each_string_list_ಅಂಶ(ಅಂಶ, &ಕೀಲಿಗಳು) {
		ರಚನೆ stdin_pack_info *info = ಅಂಶ->util;

		ಆದರೆ (info->ಬಗೆ & STDIN_PACK_EXCLUDE_OPEN) {
			/*
			 * When open-excluded packs ("!") are present, stop
			 * the parent walk at closed-excluded ("^") packs.
			 * Objects behind a "^" boundary are guaranteed to
			 * have closure and should not be rescued.
			 */
			revs->include_check = stdin_packs_include_check;
			revs->include_check_obj = stdin_packs_include_check_obj;
		}

		ಆದರೆ ((info->ಬಗೆ & STDIN_PACK_INCLUDE) ||
		    (info->ಬಗೆ & STDIN_PACK_EXCLUDE_OPEN))
			for_each_object_ಒಳ_pack(info->p,
						ಕೂಡಿಸು_object_ನಮೂದು_from_pack,
						revs,
						ODB_FOR_EACH_OBJECT_PACK_ಕ್ರಮ);
	}

	string_list_clear(&ಕೀಲಿಗಳು, 0);
}

ಸ್ಥಿರ ಶೂನ್ಯ stdin_packs_read_ಒಳಹರಿವು(ರಚನೆ rev_info *revs,
				   ಎಣಿಕೆ stdin_packs_ಬಹುಲಕ ಬಹುಲಕ)
{
	ರಚನೆ strbuf buf = STRBUF_INIT;
	ರಚನೆ strmap packs = STRMAP_INIT;
	ರಚನೆ packed_git *p;

	ಆಗ (strbuf_getline(&buf, stdin) != EOF) {
		ರಚನೆ stdin_pack_info *info;
		ಎಣಿಕೆ stdin_pack_info_ಬಗೆ ಬಗೆ = STDIN_PACK_INCLUDE;
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೀಲಿ = buf.buf;

		ಆದರೆ (!*ಕೀಲಿ)
			ಮುಂದುವರಿ;
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (*ಕೀಲಿ == '^')
			ಬಗೆ = STDIN_PACK_EXCLUDE_CLOSED;
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (*ಕೀಲಿ == '!' && ಬಹುಲಕ == STDIN_PACKS_ಬಹುಲಕ_FOLLOW)
			ಬಗೆ = STDIN_PACK_EXCLUDE_OPEN;

		ಆದರೆ (ಬಗೆ != STDIN_PACK_INCLUDE)
			ಕೀಲಿ++;

		info = strmap_get(&packs, ಕೀಲಿ);
		ಆದರೆ (!info) {
			CALLOC_ARRAY(info, 1);
			strmap_put(&packs, ಕೀಲಿ, info);
		}

		info->ಬಗೆ |= ಬಗೆ;

		strbuf_reset(&buf);
	}

	repo_for_each_pack(the_repository, p) {
		ರಚನೆ stdin_pack_info *info;

		info = strmap_get(&packs, pack_basename(p));
		ಆದರೆ (!info)
			ಮುಂದುವರಿ;

		ಆದರೆ (info->ಬಗೆ & STDIN_PACK_INCLUDE) {
			ಆದರೆ (exclude_promisor_objects && p->pack_promisor)
				die(_("packfile %s is a promisor but --exclude-promisor-objects was given"), p->pack_name);

			/*
			 * Arguments we got on stdin may not even be
			 * packs. First check that to avoid segfaulting
			 * later on in e.g.  pack_mtime_cmp(), excluded
			 * packs are handled below.
			 */
			ಆದರೆ (!is_pack_valid(p))
				die(_("packfile %s cannot be accessed"), p->pack_name);
		}

		ಆದರೆ (info->ಬಗೆ & STDIN_PACK_EXCLUDE_CLOSED) {
			/*
			 * Marking excluded packs as kept in-core so
			 * that later calls to add_object_entry()
			 * discards any objects that are also found in
			 * excluded packs.
			 */
			p->pack_keep_ಒಳ_core = 1;
		}

		ಆದರೆ (info->ಬಗೆ & STDIN_PACK_EXCLUDE_OPEN) {
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
			p->pack_keep_ಒಳ_core_open = 1;
		}

		info->p = p;
	}

	stdin_packs_ಕೂಡಿಸು_pack_entries(&packs, revs);

	strbuf_release(&buf);
	strmap_clear(&packs, 1);
}

ಸ್ಥಿರ ಶೂನ್ಯ ಕೂಡಿಸು_unreachable_loose_objects(ರಚನೆ rev_info *revs);

ಸ್ಥಿರ ಶೂನ್ಯ read_stdin_packs(ರಚನೆ repository *repo,
			     ಎಣಿಕೆ stdin_packs_ಬಹುಲಕ ಬಹುಲಕ, ಪೂರ್ಣಾಂಕ rev_list_unpacked)
{
	ಪೂರ್ಣಾಂಕ prev_fetch_if_missing = repo->fetch_if_missing;
	ರಚನೆ rev_info revs;

	/*
	 * The revision walk may hit objects that are promised, only. As the
	 * walk is best-effort though we don't want to perform backfill fetches
	 * for them.
	 */
	repo->fetch_if_missing = 0;

	repo_init_revisions(repo, &revs, ಖಾಲಿ);
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
	revs.keep_pack_cache_flags |= KEPT_PACK_ಒಳ_CORE;
	revs.blob_objects = 1;
	revs.ವೃಕ್ಷ_objects = 1;
	revs.ಟ್ಯಾಗ್_objects = 1;
	revs.ignore_missing_links = 1;
	revs.exclude_promisor_objects = exclude_promisor_objects;

	/* avoids adding objects in excluded packs */
	ignore_packed_keep_ಒಳ_core = 1;
	ಆದರೆ (ಬಹುಲಕ == STDIN_PACKS_ಬಹುಲಕ_FOLLOW) {
		/*
		 * In '--stdin-packs=follow' mode, additionally ignore
		 * objects in excluded-open packs to prevent them from
		 * appearing in the resulting pack.
		 */
		ignore_packed_keep_ಒಳ_core_open = 1;
	}
	stdin_packs_read_ಒಳಹರಿವು(&revs, ಬಹುಲಕ);
	ಆದರೆ (rev_list_unpacked)
		ಕೂಡಿಸು_unreachable_loose_objects(&revs);

	ಆದರೆ (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	traverse_commit_list(&revs,
			     show_commit_pack_hint,
			     show_object_pack_hint,
			     &ಬಹುಲಕ);

	release_revisions(&revs);

	trace2_ದತ್ತಾಂಶ_intmax("pack-objects", the_repository, "stdin_packs_found",
			   stdin_packs_ಸಿಕ್ಕಿತು_nr);
	trace2_ದತ್ತಾಂಶ_intmax("pack-objects", the_repository, "stdin_packs_hints",
			   stdin_packs_hints_nr);

	repo->fetch_if_missing = prev_fetch_if_missing;
}

ಸ್ಥಿರ ಶೂನ್ಯ ಕೂಡಿಸು_cruft_object_ನಮೂದು(ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid, ಎಣಿಕೆ object_type type,
				   ರಚನೆ packed_git *pack, off_t ಸ್ಥಾನಾಂತರ,
				   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, uint32_t mtime)
{
	ರಚನೆ object_ನಮೂದು *ನಮೂದು;

	display_ಪ್ರಗತಿ(ಪ್ರಗತಿ_state, ++nr_ನೋಡಿದ);

	ನಮೂದು = packlist_ಕಂಡುಹಿಡಿ(&ಗಮ್ಯ_pack, oid);
	ಆದರೆ (ನಮೂದು) {
		ಆದರೆ (name) {
			ನಮೂದು->hash = pack_name_hash_fn(name);
			ನಮೂದು->no_try_delta = no_try_delta(name);
		}
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಆದರೆ (!want_object_ಒಳ_pack_mtime(oid, 0, &pack, &ಸ್ಥಾನಾಂತರ, mtime))
			ಹಿಂತಿರುಗಿ;
		ಆದರೆ (!pack && type == OBJ_BLOB) {
			ರಚನೆ odb_ಮೂಲ_files *files =
				odb_ಮೂಲ_files_downcast(the_repository->objects->ಮೂಲ);
			ಪೂರ್ಣಾಂಕ ಸಿಕ್ಕಿತು_2 = 0;

			ಗಾಗಿ (ರಚನೆ odb_files_dir *dir = files->dirs; !ಸಿಕ್ಕಿತು_2 && dir; dir = dir->ಮುಂದಿನ)
				ಆದರೆ (!odb_ಮೂಲ_read_object_info(&dir->loose->ತಳ, oid, ಖಾಲಿ, 0, ಖಾಲಿ))
					ಸಿಕ್ಕಿತು_2 = 1;

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
			ಆದರೆ (!ಸಿಕ್ಕಿತು_2)
				ಹಿಂತಿರುಗಿ;
		}

		ನಮೂದು = create_object_ನಮೂದು(oid, type, pack_name_hash_fn(name),
					    0, name && no_try_delta(name),
					    pack, ಸ್ಥಾನಾಂತರ);
	}

	ಆದರೆ (mtime > oe_cruft_mtime(&ಗಮ್ಯ_pack, ನಮೂದು))
		oe_ಗಣ_cruft_mtime(&ಗಮ್ಯ_pack, ನಮೂದು, mtime);
	ಹಿಂತಿರುಗಿ;
}

ಸ್ಥಿರ ಶೂನ್ಯ show_cruft_object(ರಚನೆ object *obj, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಶೂನ್ಯ *ದತ್ತಾಂಶ ಬಳಸದ)
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
	ಕೂಡಿಸು_cruft_object_ನಮೂದು(&obj->oid, obj->type, ಖಾಲಿ, 0, name, cruft_expiration);
}

ಸ್ಥಿರ ಶೂನ್ಯ show_cruft_commit(ರಚನೆ commit *commit, ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
	show_cruft_object((ರಚನೆ object*)commit, ಖಾಲಿ, ದತ್ತಾಂಶ);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cruft_include_check_obj(ರಚನೆ object *obj, ಶೂನ್ಯ *ದತ್ತಾಂಶ ಬಳಸದ)
{
	ಹಿಂತಿರುಗಿ !has_object_kept_pack(ಗಮ್ಯ_pack.repo, &obj->oid, KEPT_PACK_ಒಳ_CORE);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cruft_include_check(ರಚನೆ commit *commit, ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
	ಹಿಂತಿರುಗಿ cruft_include_check_obj((ರಚನೆ object*)commit, ದತ್ತಾಂಶ);
}

ಸ್ಥಿರ ಶೂನ್ಯ ಗಣ_cruft_mtime(ಸ್ಥಿರಾಂಕ ರಚನೆ object *object,
			    ರಚನೆ packed_git *pack,
			    off_t ಸ್ಥಾನಾಂತರ, ಸಮಯ_t mtime)
{
	ಕೂಡಿಸು_cruft_object_ನಮೂದು(&object->oid, object->type, pack, ಸ್ಥಾನಾಂತರ, ಖಾಲಿ,
			       mtime);
}

ಸ್ಥಿರ ಶೂನ್ಯ mark_pack_kept_ಒಳ_core(ರಚನೆ string_list *packs, ಚಿಹ್ನೆರಹಿತ keep)
{
	ರಚನೆ string_list_ಅಂಶ *ಅಂಶ = ಖಾಲಿ;
	for_each_string_list_ಅಂಶ(ಅಂಶ, packs) {
		ರಚನೆ packed_git *p = ಅಂಶ->util;
		ಆದರೆ (!p)
			die(_("could not find pack '%s'"), ಅಂಶ->string);
		ಆದರೆ (p->is_cruft && keep)
			ignore_packed_keep_ಒಳ_core_has_cruft = 1;
		p->pack_keep_ಒಳ_core = keep;
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ ಕೂಡಿಸು_objects_ಒಳ_unpacked_packs(ಶೂನ್ಯ);

ಸ್ಥಿರ ಶೂನ್ಯ enumerate_cruft_objects(ಶೂನ್ಯ)
{
	ಆದರೆ (ಪ್ರಗತಿ)
		ಪ್ರಗತಿ_state = ಆರಂಭ_ಪ್ರಗತಿ(the_repository,
						_("Enumerating cruft objects"), 0);

	ಕೂಡಿಸು_objects_ಒಳ_unpacked_packs();
	ಕೂಡಿಸು_unreachable_loose_objects(ಖಾಲಿ);

	stop_ಪ್ರಗತಿ(&ಪ್ರಗತಿ_state);
}

ಸ್ಥಿರ ಶೂನ್ಯ enumerate_and_traverse_cruft_objects(ರಚನೆ string_list *fresh_packs)
{
	ರಚನೆ packed_git *p;
	ರಚನೆ rev_info revs;
	ಪೂರ್ಣಾಂಕ ret;

	repo_init_revisions(the_repository, &revs, ಖಾಲಿ);

	revs.ಟ್ಯಾಗ್_objects = 1;
	revs.ವೃಕ್ಷ_objects = 1;
	revs.blob_objects = 1;

	revs.include_check = cruft_include_check;
	revs.include_check_obj = cruft_include_check_obj;

	revs.ignore_missing_links = 1;

	ಆದರೆ (ಪ್ರಗತಿ)
		ಪ್ರಗತಿ_state = ಆರಂಭ_ಪ್ರಗತಿ(the_repository,
						_("Enumerating cruft objects"), 0);
	ret = ಕೂಡಿಸು_unseen_recent_objects_ಗಮ್ಯ_traversal(&revs, cruft_expiration,
						     ಗಣ_cruft_mtime, 1);
	stop_ಪ್ರಗತಿ(&ಪ್ರಗತಿ_state);

	ಆದರೆ (ret)
		die(_("unable to add cruft objects"));

	/*
	 * Re-mark only the fresh packs as kept so that objects in
	 * unknown packs do not halt the reachability traversal early.
	 */
	repo_for_each_pack(the_repository, p)
		p->pack_keep_ಒಳ_core = 0;
	mark_pack_kept_ಒಳ_core(fresh_packs, 1);

	ಆದರೆ (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	ಆದರೆ (ಪ್ರಗತಿ)
		ಪ್ರಗತಿ_state = ಆರಂಭ_ಪ್ರಗತಿ(the_repository,
						_("Traversing cruft objects"), 0);
	nr_ನೋಡಿದ = 0;
	traverse_commit_list(&revs, show_cruft_commit, show_cruft_object, ಖಾಲಿ);

	stop_ಪ್ರಗತಿ(&ಪ್ರಗತಿ_state);
	release_revisions(&revs);
}

ಸ್ಥಿರ ಶೂನ್ಯ read_cruft_objects(ಶೂನ್ಯ)
{
	ರಚನೆ strbuf buf = STRBUF_INIT;
	ರಚನೆ string_list discard_packs = STRING_LIST_INIT_DUP;
	ರಚನೆ string_list fresh_packs = STRING_LIST_INIT_DUP;
	ರಚನೆ packed_git *p;

	ignore_packed_keep_ಒಳ_core = 1;

	ಆಗ (strbuf_getline(&buf, stdin) != EOF) {
		ಆದರೆ (!buf.len)
			ಮುಂದುವರಿ;

		ಆದರೆ (*buf.buf == '-')
			string_list_append(&discard_packs, buf.buf + 1);
		ಇಲ್ಲದಿದ್ದರೆ
			string_list_append(&fresh_packs, buf.buf);
	}

	string_list_ವಿಂಗಡಿಸು(&discard_packs);
	string_list_ವಿಂಗಡಿಸು(&fresh_packs);

	repo_for_each_pack(the_repository, p) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *pack_name = pack_basename(p);
		ರಚನೆ string_list_ಅಂಶ *ಅಂಶ;

		ಅಂಶ = string_list_lookup(&fresh_packs, pack_name);
		ಆದರೆ (!ಅಂಶ)
			ಅಂಶ = string_list_lookup(&discard_packs, pack_name);

		ಆದರೆ (ಅಂಶ) {
			ಅಂಶ->util = p;
		} ಇಲ್ಲದಿದ್ದರೆ {
			/*
			 * This pack wasn't mentioned in either the "fresh" or
			 * "discard" list, so the caller didn't know about it.
			 *
			 * Mark it as kept so that its objects are ignored by
			 * add_unseen_recent_objects_to_traversal(). We'll
			 * unmark it before starting the traversal so it doesn't
			 * halt the traversal early.
			 */
			p->pack_keep_ಒಳ_core = 1;
		}
	}

	mark_pack_kept_ಒಳ_core(&fresh_packs, 1);
	mark_pack_kept_ಒಳ_core(&discard_packs, 0);

	ಆದರೆ (cruft_expiration)
		enumerate_and_traverse_cruft_objects(&fresh_packs);
	ಇಲ್ಲದಿದ್ದರೆ
		enumerate_cruft_objects();

	strbuf_release(&buf);
	string_list_clear(&discard_packs, 0);
	string_list_clear(&fresh_packs, 0);
}

ಸ್ಥಿರ ಶೂನ್ಯ read_object_list_from_stdin(ಶೂನ್ಯ)
{
	ಅಕ್ಷರ ಸಾಲು[GIT_MAX_HEXSZ + 1 + ಮಾರ್ಗ_MAX + 2];
	ರಚನೆ object_id oid;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *p;

	ಗಾಗಿ (;;) {
		ಆದರೆ (!fgets(ಸಾಲು, ಗಾತ್ರ(ಸಾಲು), stdin)) {
			ಆದರೆ (feof(stdin))
				ನಿಲ್ಲಿಸು;
			ಆದರೆ (!ferror(stdin))
				BUG("fgets returned NULL, not EOF, not error!");
			ಆದರೆ (errno != EINTR)
				die_errno("fgets");
			clearerr(stdin);
			ಮುಂದುವರಿ;
		}
		ಆದರೆ (ಸಾಲು[0] == '-') {
			ಆದರೆ (get_oid_hex(ಸಾಲು+1, &oid))
				die(_("expected edge object ID, got garbage:\n %s"),
				    ಸಾಲು);
			ಕೂಡಿಸು_preferred_ತಳ(&oid);
			ಮುಂದುವರಿ;
		}
		ಆದರೆ (parse_oid_hex(ಸಾಲು, &oid, &p))
			die(_("expected object ID, got garbage:\n %s"), ಸಾಲು);

		ಕೂಡಿಸು_preferred_ತಳ_object(p + 1);
		ಕೂಡಿಸು_object_ನಮೂದು(&oid, OBJ_NONE, p + 1, 0);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ show_commit(ರಚನೆ commit *commit, ಶೂನ್ಯ *ದತ್ತಾಂಶ ಬಳಸದ)
{
	ಕೂಡಿಸು_object_ನಮೂದು(&commit->object.oid, OBJ_COMMIT, ಖಾಲಿ, 0);

	ಆದರೆ (write_bitmap_ಸೂಚ್ಯಂಕ)
		ಸೂಚ್ಯಂಕ_commit_for_bitmap(commit);

	ಆದರೆ (use_delta_islands)
		propagate_island_marks(the_repository, commit);
}

ಸ್ಥಿರ ಶೂನ್ಯ show_object(ರಚನೆ object *obj, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name,
			ಶೂನ್ಯ *ದತ್ತಾಂಶ ಬಳಸದ)
{
	ಕೂಡಿಸು_preferred_ತಳ_object(name);
	ಕೂಡಿಸು_object_ನಮೂದು(&obj->oid, obj->type, name, 0);

	ಆದರೆ (use_delta_islands)
		ದಾಖಲೆ_ವೃಕ್ಷ_ಆಳ(&obj->oid, name);
}

ಸ್ಥಿರ ಶೂನ್ಯ show_object__ma_allow_any(ರಚನೆ object *obj, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
	ಪ್ರತಿಜ್ಞಾ(arg_missing_action == MA_ALLOW_ANY);

	/*
	 * Quietly ignore ALL missing objects.  This avoids problems with
	 * staging them now and getting an odd error later.
	 */
	ಆದರೆ (!odb_has_object(the_repository->objects, &obj->oid, 0))
		ಹಿಂತಿರುಗಿ;

	show_object(obj, name, ದತ್ತಾಂಶ);
}

ಸ್ಥಿರ ಶೂನ್ಯ show_object__ma_allow_promisor(ರಚನೆ object *obj, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
	ಪ್ರತಿಜ್ಞಾ(arg_missing_action == MA_ALLOW_PROMISOR);

	/*
	 * Quietly ignore EXPECTED missing objects.  This avoids problems with
	 * staging them now and getting an odd error later.
	 */
	ಆದರೆ (!odb_has_object(the_repository->objects, &obj->oid, 0) &&
	    is_promisor_object(ಗಮ್ಯ_pack.repo, &obj->oid))
		ಹಿಂತಿರುಗಿ;

	show_object(obj, name, ದತ್ತಾಂಶ);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ option_parse_missing_action(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt,
				       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	ರಚನೆ repository *repo = opt->ಮೌಲ್ಯ;

	ಪ್ರತಿಜ್ಞಾ(arg);
	ಪ್ರತಿಜ್ಞಾ(!unset);

	ಆದರೆ (!strcmp(arg, "error")) {
		arg_missing_action = MA_ERROR;
		fn_show_object = show_object;
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (!strcmp(arg, "allow-any")) {
		arg_missing_action = MA_ALLOW_ANY;
		repo->fetch_if_missing = 0;
		fn_show_object = show_object__ma_allow_any;
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (!strcmp(arg, "allow-promisor")) {
		arg_missing_action = MA_ALLOW_PROMISOR;
		repo->fetch_if_missing = 0;
		fn_show_object = show_object__ma_allow_promisor;
		ಹಿಂತಿರುಗಿ 0;
	}

	die(_("invalid value for '%s': '%s'"), "--missing", arg);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ show_ಅಂಚು(ರಚನೆ commit *commit)
{
	ಕೂಡಿಸು_preferred_ತಳ(&commit->object.oid);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಕೂಡಿಸು_object_ಒಳ_unpacked_pack(ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid,
				       ರಚನೆ object_info *oi,
				       ಶೂನ್ಯ *ದತ್ತಾಂಶ ಬಳಸದ)
{
	ಆದರೆ (cruft) {
		ಕೂಡಿಸು_cruft_object_ನಮೂದು(oid, OBJ_NONE, oi->ಮೂಲ_infop->u.packed.pack,
				       oi->ಮೂಲ_infop->u.packed.ಸ್ಥಾನಾಂತರ, ಖಾಲಿ,
				       *oi->mtimep);
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಕೂಡಿಸು_object_ನಮೂದು(oid, OBJ_NONE, "", 0);
	}
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ ಕೂಡಿಸು_objects_ಒಳ_unpacked_packs(ಶೂನ್ಯ)
{
	ರಚನೆ odb_ಮೂಲ_files *files =
		odb_ಮೂಲ_files_downcast(ಗಮ್ಯ_pack.repo->objects->ಮೂಲ);
	ಸಮಯ_t mtime;
	ರಚನೆ odb_for_each_object_options opts = {
		.flags = ODB_FOR_EACH_OBJECT_PACK_ಕ್ರಮ |
			 ODB_FOR_EACH_OBJECT_LOCAL_ONLY |
			 ODB_FOR_EACH_OBJECT_SKIP_ಒಳ_CORE_KEPT_PACKS |
			 ODB_FOR_EACH_OBJECT_SKIP_ON_DISK_KEPT_PACKS,
	};
	ರಚನೆ odb_ಮೂಲ_info ಮೂಲ_info;
	ರಚನೆ object_info oi = {
		.mtimep = &mtime,
		.ಮೂಲ_infop = &ಮೂಲ_info,
	};

	ಗಾಗಿ (ರಚನೆ odb_files_dir *dir = files->dirs; dir; dir = dir->ಮುಂದಿನ) {
		ಆದರೆ (!dir->local)
			ಮುಂದುವರಿ;

		ಆದರೆ (odb_ಮೂಲ_for_each_object(&dir->packed->ತಳ, &oi,
					       ಕೂಡಿಸು_object_ಒಳ_unpacked_pack, ಖಾಲಿ, &opts))
			die(_("cannot open pack index"));
	}
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಕೂಡಿಸು_loose_object(ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ,
			    ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
	ರಚನೆ rev_info *revs = ದತ್ತಾಂಶ;
	ಎಣಿಕೆ object_type type = odb_read_object_info(the_repository->objects, oid, ಖಾಲಿ);

	ಆದರೆ (type < 0) {
		warning(_("loose object at %s could not be examined"), ಮಾರ್ಗ);
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (cruft) {
		ರಚನೆ stat st;
		ಆದರೆ (stat(ಮಾರ್ಗ, &st) < 0) {
			ಆದರೆ (errno == ENOENT)
				ಹಿಂತಿರುಗಿ 0;
			ಹಿಂತಿರುಗಿ error_errno("unable to stat %s", oid_ಗಮ್ಯ_hex(oid));
		}

		ಕೂಡಿಸು_cruft_object_ನಮೂದು(oid, type, ಖಾಲಿ, 0, ಖಾಲಿ,
				       st.st_mtime);
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಕೂಡಿಸು_object_ನಮೂದು(oid, type, "", 0);
	}

	ಆದರೆ (revs && type == OBJ_COMMIT)
		ಕೂಡಿಸು_pending_oid(revs, ಖಾಲಿ, oid, 0);

	ಹಿಂತಿರುಗಿ 0;
}

/*
 * We actually don't even have to worry about reachability here.
 * add_object_entry will weed out duplicates, so we just add every
 * loose object we find.
 */
ಸ್ಥಿರ ಶೂನ್ಯ ಕೂಡಿಸು_unreachable_loose_objects(ರಚನೆ rev_info *revs)
{
	for_each_loose_file_ಒಳ_ಮೂಲ(the_repository->objects->ಮೂಲ,
				      ಕೂಡಿಸು_loose_object, ಖಾಲಿ, ಖಾಲಿ, revs);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ has_sha1_pack_kept_or_nonlocal(ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid)
{
	ಸ್ಥಿರ ರಚನೆ packed_git *ಕೊನೆಯ_ಸಿಕ್ಕಿತು = ಖಾಲಿ;
	ರಚನೆ packed_git *p;

	ಆದರೆ (ಕೊನೆಯ_ಸಿಕ್ಕಿತು && ಕಂಡುಹಿಡಿ_pack_ನಮೂದು_one(oid, ಕೊನೆಯ_ಸಿಕ್ಕಿತು))
		ಹಿಂತಿರುಗಿ 1;

	repo_for_each_pack(the_repository, p) {
		/*
		 * We have already checked `last_found`, so there is no need to
		 * re-check here.
		 */
		ಆದರೆ (p == ಕೊನೆಯ_ಸಿಕ್ಕಿತು)
			ಮುಂದುವರಿ;

		ಆದರೆ ((!p->pack_local || p->pack_keep || p->pack_keep_ಒಳ_core) &&
		    ಕಂಡುಹಿಡಿ_pack_ನಮೂದು_one(oid, p)) {
			ಕೊನೆಯ_ಸಿಕ್ಕಿತು = p;
			ಹಿಂತಿರುಗಿ 1;
		}
	}

	ಹಿಂತಿರುಗಿ 0;
}

/*
 * Store a list of sha1s that are should not be discarded
 * because they are either written too recently, or are
 * reachable from another object that was.
 *
 * This is filled by get_object_list.
 */
ಸ್ಥಿರ ರಚನೆ oid_array recent_objects;

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ loosened_object_can_be_discarded(ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid,
					    timestamp_t mtime)
{
	ಆದರೆ (!unpack_unreachable_expiration)
		ಹಿಂತಿರುಗಿ 0;
	ಆದರೆ (mtime > unpack_unreachable_expiration)
		ಹಿಂತಿರುಗಿ 0;
	ಆದರೆ (oid_array_lookup(&recent_objects, oid) >= 0)
		ಹಿಂತಿರುಗಿ 0;
	ಹಿಂತಿರುಗಿ 1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ force_object_loose(ರಚನೆ odb_ಮೂಲ *ಮೂಲ,
			      ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid,
			      ಸ್ಥಿರಾಂಕ ಸಮಯ_t *mtime)
{
	ರಚನೆ odb_ಮೂಲ_files *files = odb_ಮೂಲ_files_downcast(ಮೂಲ);
	ಸ್ಥಿರಾಂಕ ರಚನೆ git_hash_algo *compat = ಮೂಲ->odb->repo->compat_hash_algo;
	ರಚನೆ object_info oi = OBJECT_INFO_INIT;
	ರಚನೆ object_id compat_oid, *compat_oid_p = ಖಾಲಿ;
	ಎಣಿಕೆ object_type type;
	ಶೂನ್ಯ *buf = ಖಾಲಿ;
	ಗಾತ್ರ_t len;
	ಪೂರ್ಣಾಂಕ ret;

	ಗಾಗಿ (ರಚನೆ odb_files_dir *dir = files->dirs; dir; dir = dir->ಮುಂದಿನ)
		ಆದರೆ (!odb_ಮೂಲ_read_object_info(&dir->loose->ತಳ, oid, ಖಾಲಿ, 0, ಖಾಲಿ))
			ಹಿಂತಿರುಗಿ 0;

	oi.typep = &type;
	oi.sizep = &len;
	oi.contentp = &buf;
	ಆದರೆ (odb_read_object_info_extended(ಮೂಲ->odb, oid, &oi, 0)) {
		ret = error(_("cannot read object for %s"), oid_ಗಮ್ಯ_hex(oid));
		ಹೋಗು ಹೊರಹೋಗು;
	}

	ಆದರೆ (compat) {
		ಆದರೆ (repo_oid_ಗಮ್ಯ_algop(ಮೂಲ->odb->repo, oid, compat, &compat_oid)) {
			ret = error(_("cannot map object %s to %s"),
				    oid_ಗಮ್ಯ_hex(oid), compat->name);
			ಹೋಗು ಹೊರಹೋಗು;
		}

		compat_oid_p = &compat_oid;
	}

	ret = odb_ಮೂಲ_write_object(&files->dirs->loose->ತಳ, buf, len, type, oid,
				      compat_oid_p, mtime, 0);

ಹೊರಹೋಗು:
	ಬಿಡುಗಡೆ(buf);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಶೂನ್ಯ loosen_ಬಳಸದ_packed_objects(ಶೂನ್ಯ)
{
	ರಚನೆ packed_git *p;
	uint32_t i;
	uint32_t loosened_objects_nr = 0;
	ರಚನೆ object_id oid;

	repo_for_each_pack(the_repository, p) {
		ಆದರೆ (!p->pack_local || p->pack_keep || p->pack_keep_ಒಳ_core)
			ಮುಂದುವರಿ;

		ಆದರೆ (open_pack_ಸೂಚ್ಯಂಕ(p))
			die(_("cannot open pack index"));

		ಗಾಗಿ (i = 0; i < p->ಅಂಕೆ_objects; i++) {
			nth_packed_object_id(&oid, p, i);
			ಆದರೆ (!packlist_ಕಂಡುಹಿಡಿ(&ಗಮ್ಯ_pack, &oid) &&
			    !has_sha1_pack_kept_or_nonlocal(&oid) &&
			    !loosened_object_can_be_discarded(&oid, p->mtime)) {
				ಆದರೆ (force_object_loose(the_repository->objects->ಮೂಲ,
						       &oid, &p->mtime))
					die(_("unable to force loose object"));
				loosened_objects_nr++;
			}
		}
	}

	trace2_ದತ್ತಾಂಶ_intmax("pack-objects", the_repository,
			   "loosen_unused_packed_objects/loosened", loosened_objects_nr);
}

/*
 * This tracks any options which pack-reuse code expects to be on, or which a
 * reader of the pack might not understand, and which would therefore prevent
 * blind reuse of what we have on disk.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ pack_options_allow_reuse(ಶೂನ್ಯ)
{
	ಹಿಂತಿರುಗಿ allow_pack_reuse != NO_PACK_REUSE &&
	       pack_ಗಮ್ಯ_stdout &&
	       !ignore_packed_keep_on_disk &&
	       !ignore_packed_keep_ಒಳ_core &&
	       (!local || !have_non_local_packs) &&
	       !incremental;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ get_object_list_from_bitmap(ರಚನೆ rev_info *revs)
{
	ಆದರೆ (!(bitmap_git = prepare_bitmap_walk(revs, 0)))
		ಹಿಂತಿರುಗಿ -1;

	/*
	 * For now, force the name-hash version to be 1 since that
	 * is the version implied by the bitmap format. Later, the
	 * format can include this version explicitly in its format,
	 * allowing readers to know the version that was used during
	 * the bitmap write.
	 */
	name_hash_ಆವೃತ್ತಿ = 1;

	ಆದರೆ (pack_options_allow_reuse())
		reuse_partial_packfile_from_bitmap(bitmap_git,
						   &reuse_packfiles,
						   &reuse_packfiles_nr,
						   &reuse_packfile_bitmap,
						   allow_pack_reuse == MULTI_PACK_REUSE);

	ಆದರೆ (reuse_packfiles) {
		reuse_packfile_objects = bitmap_popcount(reuse_packfile_bitmap);
		ಆದರೆ (!reuse_packfile_objects)
			BUG("expected non-empty reuse bitmap");

		nr_ಫಲಿತಾಂಶ += reuse_packfile_objects;
		nr_ನೋಡಿದ += reuse_packfile_objects;
		display_ಪ್ರಗತಿ(ಪ್ರಗತಿ_state, nr_ನೋಡಿದ);
	}

	traverse_bitmap_commit_list(bitmap_git, revs,
				    &ಕೂಡಿಸು_object_ನಮೂದು_from_bitmap);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ ದಾಖಲೆ_recent_object(ರಚನೆ object *obj,
				 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name ಬಳಸದ,
				 ಶೂನ್ಯ *ದತ್ತಾಂಶ ಬಳಸದ)
{
	oid_array_append(&recent_objects, &obj->oid);
}

ಸ್ಥಿರ ಶೂನ್ಯ ದಾಖಲೆ_recent_commit(ರಚನೆ commit *commit, ಶೂನ್ಯ *ದತ್ತಾಂಶ ಬಳಸದ)
{
	oid_array_append(&recent_objects, &commit->object.oid);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ mark_bitmap_preferred_tip(ಸ್ಥಿರಾಂಕ ರಚನೆ reference *ref, ಶೂನ್ಯ *ದತ್ತಾಂಶ ಬಳಸದ)
{
	ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *maybe_peeled = ref->oid;
	ರಚನೆ object_id peeled;
	ರಚನೆ object *object;

	ಆದರೆ (!reference_get_peeled_oid(the_repository, ref, &peeled))
		maybe_peeled = &peeled;

	object = parse_object_or_die(the_repository, maybe_peeled, ref->name);
	ಆದರೆ (object->type == OBJ_COMMIT)
		object->flags |= NEEDS_BITMAP;

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ is_oid_uninteresting(ರಚನೆ repository *repo,
				       ರಚನೆ object_id *oid)
{
	ರಚನೆ object *o = lookup_object(repo, oid);
	ಹಿಂತಿರುಗಿ !o || (o->flags & UNINTERESTING);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಕೂಡಿಸು_objects_by_ಮಾರ್ಗ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ,
			       ರಚನೆ oid_array *oids,
			       ಎಣಿಕೆ object_type type,
			       ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
	ಗಾತ್ರ_t oe_ಆರಂಭ = ಗಮ್ಯ_pack.nr_objects;
	ಗಾತ್ರ_t oe_ಕೊನೆ;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ *processed = ದತ್ತಾಂಶ;

	/*
	 * First, add all objects to the packing data, including the ones
	 * marked UNINTERESTING (translated to 'exclude') as they can be
	 * used as delta bases.
	 */
	ಗಾಗಿ (ಗಾತ್ರ_t i = 0; i < oids->nr; i++) {
		ಪೂರ್ಣಾಂಕ exclude;
		ರಚನೆ object_info oi = OBJECT_INFO_INIT;
		ರಚನೆ object_id *oid = &oids->oid[i];

		/* Skip objects that do not exist locally. */
		ಆದರೆ ((exclude_promisor_objects || arg_missing_action != MA_ERROR) &&
		    odb_read_object_info_extended(the_repository->objects, oid, &oi,
						  OBJECT_INFO_FOR_PREFETCH) < 0)
			ಮುಂದುವರಿ;

		exclude = is_oid_uninteresting(the_repository, oid);

		ಆದರೆ (exclude && !thin)
			ಮುಂದುವರಿ;

		ಕೂಡಿಸು_object_ನಮೂದು(oid, type, ಮಾರ್ಗ, exclude);

		ಆದರೆ (type == OBJ_COMMIT) {
			ರಚನೆ commit *commit;

			ಆದರೆ (!write_bitmap_ಸೂಚ್ಯಂಕ && !use_delta_islands)
				ಮುಂದುವರಿ;

			commit = lookup_commit(the_repository, oid);
			ಆದರೆ (!commit)
				die(_("could not find commit %s"), oid_ಗಮ್ಯ_hex(oid));
			ಆದರೆ (write_bitmap_ಸೂಚ್ಯಂಕ)
				ಸೂಚ್ಯಂಕ_commit_for_bitmap(commit);
			/*
			 * Skip island propagation for boundary commits.
			 * The regular traversal's show_commit() is only
			 * called for interesting commits; matching that
			 * here keeps path-walk from doing extra work that
			 * would only be a no-op anyway (boundary commits
			 * are not in island_marks).
			 */
			ಆದರೆ (use_delta_islands && !exclude)
				propagate_island_marks(the_repository, commit);
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (type == OBJ_ವೃಕ್ಷ && use_delta_islands) {
			ದಾಖಲೆ_ವೃಕ್ಷ_ಆಳ(oid, ಮಾರ್ಗ);
		}
	}

	oe_ಕೊನೆ = ಗಮ್ಯ_pack.nr_objects;

	/* We can skip delta calculations if it is a no-op. */
	ಆದರೆ (oe_ಕೊನೆ == oe_ಆರಂಭ || !ಕಿಟಕಿ)
		ಹಿಂತಿರುಗಿ 0;

	ALLOC_GROW(ಗಮ್ಯ_pack.regions,
		   ಗಮ್ಯ_pack.nr_regions + 1,
		   ಗಮ್ಯ_pack.nr_regions_alloc);

	ಗಮ್ಯ_pack.regions[ಗಮ್ಯ_pack.nr_regions].ಆರಂಭ = oe_ಆರಂಭ;
	ಗಮ್ಯ_pack.regions[ಗಮ್ಯ_pack.nr_regions].nr = oe_ಕೊನೆ - oe_ಆರಂಭ;
	ಗಮ್ಯ_pack.nr_regions++;

	*processed += oids->nr;
	display_ಪ್ರಗತಿ(ಪ್ರಗತಿ_state, *processed);

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ get_object_list_ಮಾರ್ಗ_walk(ರಚನೆ rev_info *revs)
{
	ರಚನೆ ಮಾರ್ಗ_walk_info info = ಮಾರ್ಗ_WALK_INFO_INIT;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ processed = 0;
	ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ;

	info.revs = revs;
	info.ಮಾರ್ಗ_fn = ಕೂಡಿಸು_objects_by_ಮಾರ್ಗ;
	info.ಮಾರ್ಗ_fn_ದತ್ತಾಂಶ = &processed;

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
	info.ಅಂಚು_aggressive = shallow;

	trace2_region_enter("pack-objects", "path-walk", revs->repo);
	ಫಲಿತಾಂಶ = walk_objects_by_ಮಾರ್ಗ(&info);
	trace2_region_leave("pack-objects", "path-walk", revs->repo);

	ಮಾರ್ಗ_walk_info_clear(&info);

	ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

ಸ್ಥಿರ ಶೂನ್ಯ get_object_list(ರಚನೆ rev_info *revs, ರಚನೆ strvec *argv)
{
	ರಚನೆ ಸಿದ್ಧತೆ_revision_opt s_r_opt = {
		.allow_exclude_promisor_objects = 1,
	};
	ರಚನೆ repo_config_ಮೌಲ್ಯಗಳು *cfg = repo_config_ಮೌಲ್ಯಗಳು(the_repository);
	ಅಕ್ಷರ ಸಾಲು[1000];
	ಪೂರ್ಣಾಂಕ flags = 0;
	ಪೂರ್ಣಾಂಕ save_warning;

	save_commit_ಬಫರ್ = 0;
	ಸಿದ್ಧತೆ_revisions_from_strvec(argv, revs, &s_r_opt);

	/* make sure shallows are read */
	is_repository_shallow(the_repository);

	save_warning = cfg->warn_on_object_refname_ambiguity;
	cfg->warn_on_object_refname_ambiguity = 0;

	ಆಗ (fgets(ಸಾಲು, ಗಾತ್ರ(ಸಾಲು), stdin) != ಖಾಲಿ) {
		ಪೂರ್ಣಾಂಕ len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(ಸಾಲು);
		ಆದರೆ (len && ಸಾಲು[len - 1] == '\n')
			ಸಾಲು[--len] = 0;
		ಆದರೆ (!len)
			ನಿಲ್ಲಿಸು;
		ಆದರೆ (*ಸಾಲು == '-') {
			ಆದರೆ (!strcmp(ಸಾಲು, "--not")) {
				flags ^= UNINTERESTING;
				write_bitmap_ಸೂಚ್ಯಂಕ = 0;
				ಮುಂದುವರಿ;
			}
			ಆದರೆ (starts_with(ಸಾಲು, "--shallow ")) {
				ರಚನೆ object_id oid;
				ಆದರೆ (get_oid_hex(ಸಾಲು + 10, &oid))
					die("not an object name '%s'", ಸಾಲು + 10);
				ರಿಜಿಸ್ಟರ್_shallow(the_repository, &oid);
				use_bitmap_ಸೂಚ್ಯಂಕ = 0;
				ಮುಂದುವರಿ;
			}
			die(_("not a rev '%s'"), ಸಾಲು);
		}
		ಆದರೆ (handle_revision_arg(ಸಾಲು, revs, flags, REVARG_CANNOT_BE_FILENAME))
			die(_("bad revision '%s'"), ಸಾಲು);
	}

	cfg->warn_on_object_refname_ambiguity = save_warning;

	ಆದರೆ (use_bitmap_ಸೂಚ್ಯಂಕ && !get_object_list_from_bitmap(revs))
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (use_delta_islands)
		ತುಂಬು_delta_islands(the_repository, ಪ್ರಗತಿ);

	ಆದರೆ (write_bitmap_ಸೂಚ್ಯಂಕ)
		for_each_preferred_bitmap_tip(the_repository, mark_bitmap_preferred_tip,
					      ಖಾಲಿ);

	ಆದರೆ (!fn_show_object)
		fn_show_object = show_object;

	ಆದರೆ (ಮಾರ್ಗ_walk) {
		ಆದರೆ (get_object_list_ಮಾರ್ಗ_walk(revs)) {
			warning(_("failed to pack objects via path-walk"));
			ಮಾರ್ಗ_walk = 0;
		}
	}

	ಆದರೆ (!ಮಾರ್ಗ_walk) {
		ಆದರೆ (prepare_revision_walk(revs))
			die(_("revision walk setup failed"));
		mark_ಅಂಚುಗಳು_uninteresting(revs, show_ಅಂಚು, sparse);
		traverse_commit_list(revs,
				show_commit, fn_show_object,
				ಖಾಲಿ);
	}

	ಆದರೆ (unpack_unreachable_expiration) {
		revs->ignore_missing_links = 1;
		ಆದರೆ (ಕೂಡಿಸು_unseen_recent_objects_ಗಮ್ಯ_traversal(revs,
				unpack_unreachable_expiration, ಖಾಲಿ, 0))
			die(_("unable to add recent objects"));
		ಆದರೆ (prepare_revision_walk(revs))
			die(_("revision walk setup failed"));
		traverse_commit_list(revs, ದಾಖಲೆ_recent_commit,
				     ದಾಖಲೆ_recent_object, ಖಾಲಿ);
	}

	ಆದರೆ (keep_unreachable)
		ಕೂಡಿಸು_objects_ಒಳ_unpacked_packs();
	ಆದರೆ (pack_loose_unreachable)
		ಕೂಡಿಸು_unreachable_loose_objects(ಖಾಲಿ);
	ಆದರೆ (unpack_unreachable)
		loosen_ಬಳಸದ_packed_objects();

	oid_array_clear(&recent_objects);
}

ಸ್ಥಿರ ಶೂನ್ಯ ಕೂಡಿಸು_extra_kept_packs(ಸ್ಥಿರಾಂಕ ರಚನೆ string_list *ಹೆಸರುಗಳು)
{
	ರಚನೆ packed_git *p;

	ಆದರೆ (!ಹೆಸರುಗಳು->nr)
		ಹಿಂತಿರುಗಿ;

	repo_for_each_pack(the_repository, p) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name = basename(p->pack_name);
		ಪೂರ್ಣಾಂಕ i;

		ಆದರೆ (!p->pack_local)
			ಮುಂದುವರಿ;

		ಗಾಗಿ (i = 0; i < ಹೆಸರುಗಳು->nr; i++)
			ಆದರೆ (!fspathcmp(name, ಹೆಸರುಗಳು->ವಸ್ತುಗಳು[i].string))
				ನಿಲ್ಲಿಸು;

		ಆದರೆ (i < ಹೆಸರುಗಳು->nr) {
			p->pack_keep_ಒಳ_core = 1;
			ignore_packed_keep_ಒಳ_core = 1;
			ಮುಂದುವರಿ;
		}
	}
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ option_parse_quiet(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg,
			      ಪೂರ್ಣಾಂಕ unset)
{
	ಪೂರ್ಣಾಂಕ *val = opt->ಮೌಲ್ಯ;

	BUG_ON_OPT_ARG(arg);

	ಆದರೆ (!unset)
		*val = 0;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!*val)
		*val = 1;
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ option_parse_ಸೂಚ್ಯಂಕ_ಆವೃತ್ತಿ(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt,
				      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	ರಚನೆ pack_idx_option *popts = opt->ಮೌಲ್ಯ;
	ಅಕ್ಷರ *c;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *val = arg;

	BUG_ON_OPT_NEG(unset);

	popts->ಆವೃತ್ತಿ = strtoul(val, &c, 10);
	ಆದರೆ (popts->ಆವೃತ್ತಿ > 2)
		die(_("unsupported index version %s"), val);
	ಆದರೆ (*c == ',' && c[1])
		popts->off32_ಎಲ್ಲೆ = strtoul(c+1, &c, 0);
	ಆದರೆ (*c || popts->off32_ಎಲ್ಲೆ & 0x80000000)
		die(_("bad index version '%s'"), val);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ option_parse_unpack_unreachable(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt ಬಳಸದ,
					   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	ಆದರೆ (unset) {
		unpack_unreachable = 0;
		unpack_unreachable_expiration = 0;
	}
	ಇಲ್ಲದಿದ್ದರೆ {
		unpack_unreachable = 1;
		ಆದರೆ (arg)
			unpack_unreachable_expiration = approxidate(arg);
	}
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ option_parse_cruft_expiration(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt ಬಳಸದ,
					 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	ಆದರೆ (unset) {
		cruft = 0;
		cruft_expiration = 0;
	} ಇಲ್ಲದಿದ್ದರೆ {
		cruft = 1;
		ಆದರೆ (arg)
			cruft_expiration = approxidate(arg);
	}
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ is_not_ಒಳ_promisor_pack_obj(ರಚನೆ object *obj, ಶೂನ್ಯ *ದತ್ತಾಂಶ ಬಳಸದ)
{
	ರಚನೆ odb_ಮೂಲ_info ಮೂಲ_info;
	ರಚನೆ object_info info = {
		.ಮೂಲ_infop = &ಮೂಲ_info,
	};

	ಆದರೆ (odb_read_object_info_extended(the_repository->objects, &obj->oid, &info, 0))
		BUG("should_include_obj should only be called on existing objects");
	ಹಿಂತಿರುಗಿ ಮೂಲ_info.ಮೂಲ->type != ODB_ಮೂಲ_PACKED || !ಮೂಲ_info.u.packed.pack->pack_promisor;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ is_not_ಒಳ_promisor_pack(ರಚನೆ commit *commit, ಶೂನ್ಯ *ದತ್ತಾಂಶ) {
	ಹಿಂತಿರುಗಿ is_not_ಒಳ_promisor_pack_obj((ರಚನೆ object *) commit, ದತ್ತಾಂಶ);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ parse_stdin_packs_ಬಹುಲಕ(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg,
				  ಪೂರ್ಣಾಂಕ unset)
{
	ಎಣಿಕೆ stdin_packs_ಬಹುಲಕ *ಬಹುಲಕ = opt->ಮೌಲ್ಯ;

	ಆದರೆ (unset)
		*ಬಹುಲಕ = STDIN_PACKS_ಬಹುಲಕ_NONE;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!arg || !*arg)
		*ಬಹುಲಕ = STDIN_PACKS_ಬಹುಲಕ_STANDARD;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(arg, "follow"))
		*ಬಹುಲಕ = STDIN_PACKS_ಬಹುಲಕ_FOLLOW;
	ಇಲ್ಲದಿದ್ದರೆ
		die(_("invalid value for '%s': '%s'"), opt->long_name, arg);

	ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ cmd_pack_objects(ಪೂರ್ಣಾಂಕ argc,
		     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
		     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		     ರಚನೆ repository *repo)
{
	ಪೂರ್ಣಾಂಕ use_ಆಂತರಿಕ_rev_list = 0;
	ಪೂರ್ಣಾಂಕ all_ಪ್ರಗತಿ_implied = 0;
	ರಚನೆ strvec rp = STRVEC_INIT;
	ಪೂರ್ಣಾಂಕ rev_list_unpacked = 0, rev_list_all = 0, rev_list_reflog = 0;
	ಪೂರ್ಣಾಂಕ rev_list_ಸೂಚ್ಯಂಕ = 0;
	ಎಣಿಕೆ stdin_packs_ಬಹುಲಕ stdin_packs = STDIN_PACKS_ಬಹುಲಕ_NONE;
	ರಚನೆ string_list keep_pack_list = STRING_LIST_INIT_NODUP;
	ರಚನೆ list_objects_filter_options filter_options =
		LIST_OBJECTS_FILTER_INIT;
	ರಚನೆ repo_config_ಮೌಲ್ಯಗಳು *cfg = repo_config_ಮೌಲ್ಯಗಳು(the_repository);

	ರಚನೆ option pack_objects_options[] = {
		OPT_CALLBACK_F('q', "quiet", &ಪ್ರಗತಿ, ಖಾಲಿ,
			       N_("do not show progress meter"),
			       PARSE_OPT_NOARG, option_parse_quiet),
		OPT_ಗಣ_INT(0, "progress", &ಪ್ರಗತಿ,
			    N_("show progress meter"), 1),
		OPT_ಗಣ_INT(0, "all-progress", &ಪ್ರಗತಿ,
			    N_("show progress meter during object writing phase"), 2),
		OPT_BOOL(0, "all-progress-implied",
			 &all_ಪ್ರಗತಿ_implied,
			 N_("similar to --all-progress when progress meter is shown")),
		OPT_CALLBACK_F(0, "index-version", &pack_idx_opts, N_("<version>[,<offset>]"),
		  N_("write the pack index file in the specified idx format version"),
		  PARSE_OPT_NONEG, option_parse_ಸೂಚ್ಯಂಕ_ಆವೃತ್ತಿ),
		OPT_UNSIGNED(0, "max-pack-size", &pack_ಗಾತ್ರ_ಎಲ್ಲೆ,
			     N_("maximum size of each output pack file")),
		OPT_BOOL(0, "local", &local,
			 N_("ignore borrowed objects from alternate object store")),
		OPT_BOOL(0, "incremental", &incremental,
			 N_("ignore packed objects")),
		OPT_INTEGER(0, "window", &ಕಿಟಕಿ,
			    N_("limit pack window by objects")),
		OPT_UNSIGNED(0, "window-memory", &ಕಿಟಕಿ_ಸ್ಮರಣೆ_ಎಲ್ಲೆ,
			     N_("limit pack window by memory in addition to object limit")),
		OPT_INTEGER(0, "depth", &ಆಳ,
			    N_("maximum length of delta chain allowed in the resulting pack")),
		OPT_BOOL(0, "reuse-delta", &reuse_delta,
			 N_("reuse existing deltas")),
		OPT_BOOL(0, "reuse-object", &reuse_object,
			 N_("reuse existing objects")),
		OPT_BOOL(0, "delta-base-offset", &allow_ofs_delta,
			 N_("use OFS_DELTA objects")),
		OPT_INTEGER(0, "threads", &delta_ಹುಡುಕು_ಎಳೆಗಳು,
			    N_("use threads when searching for best delta matches")),
		OPT_BOOL(0, "non-empty", &non_ಖಾಲಿಗಳು,
			 N_("do not create an empty pack output")),
		OPT_BOOL(0, "revs", &use_ಆಂತರಿಕ_rev_list,
			 N_("read revision arguments from standard input")),
		OPT_ಗಣ_INT_F(0, "unpacked", &rev_list_unpacked,
			      N_("limit the objects to those that are not yet packed"),
			      1, PARSE_OPT_NONEG),
		OPT_ಗಣ_INT_F(0, "all", &rev_list_all,
			      N_("include objects reachable from any reference"),
			      1, PARSE_OPT_NONEG),
		OPT_ಗಣ_INT_F(0, "reflog", &rev_list_reflog,
			      N_("include objects referred by reflog entries"),
			      1, PARSE_OPT_NONEG),
		OPT_ಗಣ_INT_F(0, "indexed-objects", &rev_list_ಸೂಚ್ಯಂಕ,
			      N_("include objects referred to by the index"),
			      1, PARSE_OPT_NONEG),
		OPT_CALLBACK_F(0, "stdin-packs", &stdin_packs, N_("mode"),
			     N_("read packs from stdin"),
			     PARSE_OPT_OPTARG, parse_stdin_packs_ಬಹುಲಕ),
		OPT_BOOL(0, "stdout", &pack_ಗಮ್ಯ_stdout,
			 N_("output pack to stdout")),
		OPT_BOOL(0, "include-tag", &include_ಟ್ಯಾಗ್,
			 N_("include tag objects that refer to objects to be packed")),
		OPT_BOOL(0, "keep-unreachable", &keep_unreachable,
			 N_("keep unreachable objects")),
		OPT_BOOL(0, "pack-loose-unreachable", &pack_loose_unreachable,
			 N_("pack loose unreachable objects")),
		OPT_CALLBACK_F(0, "unpack-unreachable", ಖಾಲಿ, N_("time"),
		  N_("unpack unreachable objects newer than <time>"),
		  PARSE_OPT_OPTARG, option_parse_unpack_unreachable),
		OPT_BOOL(0, "cruft", &cruft, N_("create a cruft pack")),
		OPT_CALLBACK_F(0, "cruft-expiration", ಖಾಲಿ, N_("time"),
		  N_("expire cruft objects older than <time>"),
		  PARSE_OPT_OPTARG, option_parse_cruft_expiration),
		OPT_BOOL(0, "sparse", &sparse,
			 N_("use the sparse reachability algorithm")),
		OPT_BOOL(0, "thin", &thin,
			 N_("create thin packs")),
		OPT_BOOL(0, "path-walk", &ಮಾರ್ಗ_walk,
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
		OPT_BOOL(0, "use-bitmap-index", &use_bitmap_ಸೂಚ್ಯಂಕ,
			 N_("use a bitmap index if available to speed up counting objects")),
		OPT_ಗಣ_INT(0, "write-bitmap-index", &write_bitmap_ಸೂಚ್ಯಂಕ,
			    N_("write a bitmap index together with the pack index"),
			    WRITE_BITMAP_TRUE),
		OPT_ಗಣ_INT_F(0, "write-bitmap-index-quiet",
			      &write_bitmap_ಸೂಚ್ಯಂಕ,
			      N_("write a bitmap index if possible"),
			      WRITE_BITMAP_QUIET, PARSE_OPT_HIDDEN),
		OPT_PARSE_LIST_OBJECTS_FILTER(&filter_options),
		OPT_CALLBACK_F(0, "missing", repo, N_("action"),
		  N_("handling for missing objects"), PARSE_OPT_NONEG,
		  option_parse_missing_action),
		OPT_BOOL(0, "exclude-promisor-objects", &exclude_promisor_objects,
			 N_("do not pack objects in promisor packfiles")),
		OPT_BOOL(0, "exclude-promisor-objects-best-effort",
			 &exclude_promisor_objects_ಉತ್ತಮ_effort,
			 N_("implies --missing=allow-any")),
		OPT_BOOL(0, "delta-islands", &use_delta_islands,
			 N_("respect islands during delta compression")),
		OPT_STRING_LIST(0, "uri-protocol", &uri_protocols,
				N_("protocol"),
				N_("exclude any configured uploadpack.blobpackfileuri with this protocol")),
		OPT_INTEGER(0, "name-hash-version", &name_hash_ಆವೃತ್ತಿ,
			 N_("use the specified name-hash function to group similar objects")),
		OPT_ಕೊನೆ(),
	};

	ಆದರೆ (DFS_ಅಂಕೆ_ಸ್ಥಿತಿಗಳು > (1 << OE_DFS_STATE_ಬಿಟ್ಟುಗಳು))
		BUG("too many dfs states, increase OE_DFS_STATE_BITS");

	disable_replace_refs();

	sparse = git_env_bool("GIT_TEST_PACK_SPARSE", -1);
	ಆದರೆ (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		ಆದರೆ (sparse < 0)
			sparse = the_repository->settings.pack_use_sparse;
		ಆದರೆ (the_repository->settings.pack_use_multi_pack_reuse)
			allow_pack_reuse = MULTI_PACK_REUSE;
	}

	reset_pack_idx_option(&pack_idx_opts);
	pack_idx_opts.flags |= WRITE_REV;
	repo_config(the_repository, git_pack_config, ಖಾಲಿ);
	ಆದರೆ (git_env_bool(GIT_TEST_NO_WRITE_REV_ಸೂಚ್ಯಂಕ, 0))
		pack_idx_opts.flags &= ~WRITE_REV;

	ಪ್ರಗತಿ = isatty(2);
	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, pack_objects_options,
			     pack_usage, 0);

	ಆದರೆ (argc) {
		ತಳ_name = argv[0];
		argc--;
	}
	ಆದರೆ (pack_ಗಮ್ಯ_stdout != !ತಳ_name || argc)
		usage_with_options(pack_usage, pack_objects_options);

	ಆದರೆ (ಮಾರ್ಗ_walk < 0) {
		ಆದರೆ (use_bitmap_ಸೂಚ್ಯಂಕ > 0 ||
		    !use_ಆಂತರಿಕ_rev_list)
			ಮಾರ್ಗ_walk = 0;
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (the_repository->gitdir &&
			 the_repository->settings.pack_use_ಮಾರ್ಗ_walk)
			ಮಾರ್ಗ_walk = 1;
		ಇಲ್ಲದಿದ್ದರೆ
			ಮಾರ್ಗ_walk = git_env_bool("GIT_TEST_PACK_PATH_WALK", 0);
	}

	ಆದರೆ (ಆಳ < 0)
		ಆಳ = 0;
	ಆದರೆ (ಆಳ >= (1 << OE_ಆಳ_ಬಿಟ್ಟುಗಳು)) {
		warning(_("delta chain depth %d is too deep, forcing %d"),
			ಆಳ, (1 << OE_ಆಳ_ಬಿಟ್ಟುಗಳು) - 1);
		ಆಳ = (1 << OE_ಆಳ_ಬಿಟ್ಟುಗಳು) - 1;
	}
	ಆದರೆ (cache_max_small_delta_ಗಾತ್ರ >= (1U << OE_Z_DELTA_ಬಿಟ್ಟುಗಳು)) {
		warning(_("pack.deltaCacheLimit is too high, forcing %d"),
			(1U << OE_Z_DELTA_ಬಿಟ್ಟುಗಳು) - 1);
		cache_max_small_delta_ಗಾತ್ರ = (1U << OE_Z_DELTA_ಬಿಟ್ಟುಗಳು) - 1;
	}
	ಆದರೆ (ಕಿಟಕಿ < 0)
		ಕಿಟಕಿ = 0;

	strvec_ತಳ್ಳು(&rp, "pack-objects");

	ಆದರೆ (ಮಾರ್ಗ_walk) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *option = ಖಾಲಿ;
		ಆದರೆ (!ಮಾರ್ಗ_walk_filter_compatible(&filter_options))
			option = "--filter";

		ಆದರೆ (option) {
			warning(_("cannot use %s with %s"),
				option, "--path-walk");
			ಮಾರ್ಗ_walk = 0;
		}
	}
	ಆದರೆ (ಮಾರ್ಗ_walk) {
		strvec_ತಳ್ಳು(&rp, "--objects");
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (thin) {
		use_ಆಂತರಿಕ_rev_list = 1;
		strvec_ತಳ್ಳು(&rp, shallow
				? "--objects-edge-aggressive"
				: "--objects-edge");
	} ಇಲ್ಲದಿದ್ದರೆ
		strvec_ತಳ್ಳು(&rp, "--objects");

	ಆದರೆ (rev_list_all) {
		use_ಆಂತರಿಕ_rev_list = 1;
		strvec_ತಳ್ಳು(&rp, "--all");
	}
	ಆದರೆ (rev_list_reflog) {
		use_ಆಂತರಿಕ_rev_list = 1;
		strvec_ತಳ್ಳು(&rp, "--reflog");
	}
	ಆದರೆ (rev_list_ಸೂಚ್ಯಂಕ) {
		use_ಆಂತರಿಕ_rev_list = 1;
		strvec_ತಳ್ಳು(&rp, "--indexed-objects");
	}
	ಆದರೆ (rev_list_unpacked && !stdin_packs) {
		use_ಆಂತರಿಕ_rev_list = 1;
		strvec_ತಳ್ಳು(&rp, "--unpacked");
	}

	die_for_incompatible_opt2(exclude_promisor_objects,
				  "--exclude-promisor-objects",
				  exclude_promisor_objects_ಉತ್ತಮ_effort,
				  "--exclude-promisor-objects-best-effort");
	ಆದರೆ (exclude_promisor_objects) {
		repo->fetch_if_missing = 0;

		/* --stdin-packs handles promisor objects separately. */
		ಆದರೆ (!stdin_packs) {
			use_ಆಂತರಿಕ_rev_list = 1;
			strvec_ತಳ್ಳು(&rp, "--exclude-promisor-objects");
		}
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (exclude_promisor_objects_ಉತ್ತಮ_effort) {
		use_ಆಂತರಿಕ_rev_list = 1;
		arg_missing_action = MA_ALLOW_ANY;
		repo->fetch_if_missing = 0;
		fn_show_object = show_object__ma_allow_any;
		/* revs configured below */
	}
	ಆದರೆ (unpack_unreachable || keep_unreachable || pack_loose_unreachable)
		use_ಆಂತರಿಕ_rev_list = 1;

	ಆದರೆ (!reuse_object)
		reuse_delta = 0;
	ಆದರೆ (cfg->pack_compression_level == -1)
		cfg->pack_compression_level = Z_DEFAULT_COMPRESSION;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (cfg->pack_compression_level < 0 || cfg->pack_compression_level > Z_ಉತ್ತಮ_COMPRESSION)
		die(_("bad pack compression level %d"), cfg->pack_compression_level);

	ಆದರೆ (!delta_ಹುಡುಕು_ಎಳೆಗಳು)	/* --threads=0 means autodetect */
		delta_ಹುಡುಕು_ಎಳೆಗಳು = online_cpus();

	ಆದರೆ (!HAVE_ಎಳೆಗಳು && delta_ಹುಡುಕು_ಎಳೆಗಳು != 1)
		warning(_("no threads support, ignoring --threads"));
	ಆದರೆ (!pack_ಗಮ್ಯ_stdout && !pack_ಗಾತ್ರ_ಎಲ್ಲೆ)
		pack_ಗಾತ್ರ_ಎಲ್ಲೆ = pack_ಗಾತ್ರ_ಎಲ್ಲೆ_cfg;
	ಆದರೆ (pack_ಗಮ್ಯ_stdout && pack_ಗಾತ್ರ_ಎಲ್ಲೆ)
		die(_("--max-pack-size cannot be used to build a pack for transfer"));
	ಆದರೆ (pack_ಗಾತ್ರ_ಎಲ್ಲೆ && pack_ಗಾತ್ರ_ಎಲ್ಲೆ < 1024*1024) {
		warning(_("minimum pack size limit is 1 MiB"));
		pack_ಗಾತ್ರ_ಎಲ್ಲೆ = 1024*1024;
	}

	ಆದರೆ (!pack_ಗಮ್ಯ_stdout && thin)
		die(_("--thin cannot be used to build an indexable pack"));

	die_for_incompatible_opt2(keep_unreachable, "--keep-unreachable",
				  unpack_unreachable, "--unpack-unreachable");
	ಆದರೆ (!rev_list_all || !rev_list_reflog || !rev_list_ಸೂಚ್ಯಂಕ)
		unpack_unreachable_expiration = 0;

	die_for_incompatible_opt2(stdin_packs, "--stdin-packs",
				  filter_options.choice, "--filter");


	ಆದರೆ (stdin_packs && use_ಆಂತರಿಕ_rev_list)
		die(_("cannot use internal rev list with --stdin-packs"));

	ಆದರೆ (cruft) {
		ಆದರೆ (use_ಆಂತರಿಕ_rev_list)
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
	ಆದರೆ (!pack_ಗಮ್ಯ_stdout)
		use_bitmap_ಸೂಚ್ಯಂಕ_default = 0;

	ಆದರೆ (use_bitmap_ಸೂಚ್ಯಂಕ < 0)
		use_bitmap_ಸೂಚ್ಯಂಕ = use_bitmap_ಸೂಚ್ಯಂಕ_default;

	/* "hard" reasons not to use bitmaps; these just won't work at all */
	ಆದರೆ (!use_ಆಂತರಿಕ_rev_list || (!pack_ಗಮ್ಯ_stdout && write_bitmap_ಸೂಚ್ಯಂಕ) || is_repository_shallow(the_repository))
		use_bitmap_ಸೂಚ್ಯಂಕ = 0;

	ಆದರೆ (pack_ಗಮ್ಯ_stdout || !rev_list_all)
		write_bitmap_ಸೂಚ್ಯಂಕ = 0;

	ಆದರೆ (name_hash_ಆವೃತ್ತಿ < 0)
		name_hash_ಆವೃತ್ತಿ = (ಪೂರ್ಣಾಂಕ)git_env_ulong("GIT_TEST_NAME_HASH_VERSION", 1);

	validate_name_hash_ಆವೃತ್ತಿ();

	ಆದರೆ (use_delta_islands)
		strvec_ತಳ್ಳು(&rp, "--topo-order");

	ಆದರೆ (ಪ್ರಗತಿ && all_ಪ್ರಗತಿ_implied)
		ಪ್ರಗತಿ = 2;

	ಕೂಡಿಸು_extra_kept_packs(&keep_pack_list);
	ಆದರೆ (ignore_packed_keep_on_disk) {
		ರಚನೆ packed_git *p;

		repo_for_each_pack(the_repository, p)
			ಆದರೆ (p->pack_local && p->pack_keep)
				ನಿಲ್ಲಿಸು;
		ಆದರೆ (!p) /* no keep-able packs found */
			ignore_packed_keep_on_disk = 0;
	}
	ಆದರೆ (local) {
		/*
		 * unlike ignore_packed_keep_on_disk above, we do not
		 * want to unset "local" based on looking at packs, as
		 * it also covers non-local objects
		 */
		ರಚನೆ packed_git *p;

		repo_for_each_pack(the_repository, p) {
			ಆದರೆ (!p->pack_local) {
				have_non_local_packs = 1;
				ನಿಲ್ಲಿಸು;
			}
		}
	}

	trace2_region_enter("pack-objects", "enumerate-objects",
			    the_repository);
	prepare_packing_ದತ್ತಾಂಶ(the_repository, &ಗಮ್ಯ_pack);

	ಆದರೆ (ಪ್ರಗತಿ && !cruft)
		ಪ್ರಗತಿ_state = ಆರಂಭ_ಪ್ರಗತಿ(the_repository,
						_("Enumerating objects"), 0);
	ಆದರೆ (stdin_packs) {
		read_stdin_packs(repo, stdin_packs, rev_list_unpacked);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (cruft) {
		read_cruft_objects();
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!use_ಆಂತರಿಕ_rev_list) {
		read_object_list_from_stdin();
	} ಇಲ್ಲದಿದ್ದರೆ {
		ರಚನೆ rev_info revs;

		repo_init_revisions(the_repository, &revs, ಖಾಲಿ);
		list_objects_filter_copy(&revs.filter, &filter_options);
		ಆದರೆ (exclude_promisor_objects_ಉತ್ತಮ_effort) {
			revs.include_check = is_not_ಒಳ_promisor_pack;
			revs.include_check_obj = is_not_ಒಳ_promisor_pack_obj;
		}
		get_object_list(&revs, &rp);
		release_revisions(&revs);
	}
	cleanup_preferred_ತಳ();
	ಆದರೆ (include_ಟ್ಯಾಗ್ && nr_ಫಲಿತಾಂಶ)
		refs_for_each_ಟ್ಯಾಗ್_ref(get_main_ref_store(the_repository),
				      ಕೂಡಿಸು_ref_ಟ್ಯಾಗ್, ಖಾಲಿ);
	stop_ಪ್ರಗತಿ(&ಪ್ರಗತಿ_state);
	trace2_region_leave("pack-objects", "enumerate-objects",
			    the_repository);

	ಆದರೆ (non_ಖಾಲಿಗಳು && !nr_ಫಲಿತಾಂಶ)
		ಹೋಗು cleanup;
	ಆದರೆ (nr_ಫಲಿತಾಂಶ) {
		trace2_region_enter("pack-objects", "prepare-pack",
				    the_repository);
		prepare_pack(ಕಿಟಕಿ, ಆಳ);
		trace2_region_leave("pack-objects", "prepare-pack",
				    the_repository);
	}

	trace2_region_enter("pack-objects", "write-pack-file", the_repository);
	write_excluded_by_configs();
	write_pack_file();
	trace2_region_leave("pack-objects", "write-pack-file", the_repository);

	ಆದರೆ (ಪ್ರಗತಿ)
		fprintf_ln(stderr,
			   _("Total %"PRIu32" (delta %"PRIu32"),"
			     " reused %"PRIu32" (delta %"PRIu32"),"
			     " pack-reused %"PRIu32" (from %"PRIuMAX")"),
			   written, written_delta, reused, reused_delta,
			   reuse_packfile_objects,
			   (uintmax_t)reuse_packfiles_ಬಳಸಿದ_nr);

	trace2_ದತ್ತಾಂಶ_intmax("pack-objects", the_repository, "written", written);
	trace2_ದತ್ತಾಂಶ_intmax("pack-objects", the_repository, "written/delta", written_delta);
	trace2_ದತ್ತಾಂಶ_intmax("pack-objects", the_repository, "reused", reused);
	trace2_ದತ್ತಾಂಶ_intmax("pack-objects", the_repository, "reused/delta", reused_delta);
	trace2_ದತ್ತಾಂಶ_intmax("pack-objects", the_repository, "pack-reused", reuse_packfile_objects);
	trace2_ದತ್ತಾಂಶ_intmax("pack-objects", the_repository, "packs-reused", reuse_packfiles_ಬಳಸಿದ_nr);

cleanup:
	clear_packing_ದತ್ತಾಂಶ(&ಗಮ್ಯ_pack);
	list_objects_filter_release(&filter_options);
	string_list_clear(&keep_pack_list, 0);
	strvec_clear(&rp);

	ಹಿಂತಿರುಗಿ 0;
}
