#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DISABLE_SIGN_COMPARE_WARNINGS

#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hex.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "attr.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "commit.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "tag.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "delta.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "pack.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "pack-revindex.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "csum-file.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "tree-walk.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "diff.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "revision.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "list-objects.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "list-objects-filter-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "pack-objects.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "progress.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "refs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "thread-utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "pack-bitmap.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "delta-islands.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "reachable.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "oid-array.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "strvec.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "strmap.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "list.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "packfile.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object-file.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object-file-convert.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "odb.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "odb/streaming.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "replace-object.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "dir.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "midx.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "trace2.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "shallow.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "promisor-remote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "pack-mtimes.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "pkt-line.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "blob.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "tree.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "path-walk.h"

/*
 * Objects we are going to pack are collected in the `to_pack` structure.
 * It contains an array (dynamically expanded) of the object data, and a map
 * that can resolve SHA1s to their position in the array.
 */
ਸਥਿਰ ਬਣਤਰ packing_ਅੰਕੜੇ ਮੰਜ਼ਿਲ_pack;

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ object_ਇੰਦਰਾਜ *oe_delta(
		ਅਚਲ ਬਣਤਰ packing_ਅੰਕੜੇ *pack,
		ਅਚਲ ਬਣਤਰ object_ਇੰਦਰਾਜ *e)
{
	ਜੇ (!e->delta_idx)
		ਵਾਪਸ ਨਿਰਾ;
	ਜੇ (e->ext_ਅਧਾਰ)
		ਵਾਪਸ &pack->ext_bases[e->delta_idx - 1];
	ਨਹੀਂਤਾਂ
		ਵਾਪਸ &pack->objects[e->delta_idx - 1];
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਮਾਪ_t oe_delta_ਮਾਪ(ਬਣਤਰ packing_ਅੰਕੜੇ *pack,
				   ਅਚਲ ਬਣਤਰ object_ਇੰਦਰਾਜ *e)
{
	ਜੇ (e->delta_ਮਾਪ_valid)
		ਵਾਪਸ e->delta_ਮਾਪ;

	/*
	 * pack->delta_size[] can't be NULL because oe_set_delta_size()
	 * must have been called when a new delta is saved with
	 * oe_set_delta().
	 * If oe_delta() returns NULL (i.e. default state, which means
	 * delta_size_valid is also false), then the caller must never
	 * call oe_delta_size().
	 */
	ਵਾਪਸ pack->delta_ਮਾਪ_2[e - pack->objects];
}

ਮਾਪ_t oe_get_ਮਾਪ_slow(ਬਣਤਰ packing_ਅੰਕੜੇ *pack,
			ਅਚਲ ਬਣਤਰ object_ਇੰਦਰਾਜ *e);

ਸਥਿਰ ਸਤਰਅੰਦਰ ਮਾਪ_t oe_ਮਾਪ(ਬਣਤਰ packing_ਅੰਕੜੇ *pack,
			     ਅਚਲ ਬਣਤਰ object_ਇੰਦਰਾਜ *e)
{
	ਜੇ (e->ਮਾਪ_valid)
		ਵਾਪਸ e->ਮਾਪ_2;

	ਵਾਪਸ oe_get_ਮਾਪ_slow(pack, e);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ oe_ਸੈੱਟ_delta(ਬਣਤਰ packing_ਅੰਕੜੇ *pack,
				ਬਣਤਰ object_ਇੰਦਰਾਜ *e,
				ਬਣਤਰ object_ਇੰਦਰਾਜ *delta)
{
	ਜੇ (delta)
		e->delta_idx = (delta - pack->objects) + 1;
	ਨਹੀਂਤਾਂ
		e->delta_idx = 0;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ object_ਇੰਦਰਾਜ *oe_delta_sibling(
		ਅਚਲ ਬਣਤਰ packing_ਅੰਕੜੇ *pack,
		ਅਚਲ ਬਣਤਰ object_ਇੰਦਰਾਜ *e)
{
	ਜੇ (e->delta_sibling_idx)
		ਵਾਪਸ &pack->objects[e->delta_sibling_idx - 1];
	ਵਾਪਸ ਨਿਰਾ;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ object_ਇੰਦਰਾਜ *oe_delta_ਬੱਚਾ(
		ਅਚਲ ਬਣਤਰ packing_ਅੰਕੜੇ *pack,
		ਅਚਲ ਬਣਤਰ object_ਇੰਦਰਾਜ *e)
{
	ਜੇ (e->delta_ਬੱਚਾ_idx)
		ਵਾਪਸ &pack->objects[e->delta_ਬੱਚਾ_idx - 1];
	ਵਾਪਸ ਨਿਰਾ;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ oe_ਸੈੱਟ_delta_ਬੱਚਾ(ਬਣਤਰ packing_ਅੰਕੜੇ *pack,
				      ਬਣਤਰ object_ਇੰਦਰਾਜ *e,
				      ਬਣਤਰ object_ਇੰਦਰਾਜ *delta)
{
	ਜੇ (delta)
		e->delta_ਬੱਚਾ_idx = (delta - pack->objects) + 1;
	ਨਹੀਂਤਾਂ
		e->delta_ਬੱਚਾ_idx = 0;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ oe_ਸੈੱਟ_delta_sibling(ਬਣਤਰ packing_ਅੰਕੜੇ *pack,
					ਬਣਤਰ object_ਇੰਦਰਾਜ *e,
					ਬਣਤਰ object_ਇੰਦਰਾਜ *delta)
{
	ਜੇ (delta)
		e->delta_sibling_idx = (delta - pack->objects) + 1;
	ਨਹੀਂਤਾਂ
		e->delta_sibling_idx = 0;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ oe_ਸੈੱਟ_ਮਾਪ(ਬਣਤਰ packing_ਅੰਕੜੇ *pack,
			       ਬਣਤਰ object_ਇੰਦਰਾਜ *e,
			       ਮਾਪ_t ਮਾਪ)
{
	ਜੇ (ਮਾਪ < pack->oe_ਮਾਪ_ਹੱਦਬੰਦੀ) {
		e->ਮਾਪ_2 = ਮਾਪ;
		e->ਮਾਪ_valid = 1;
	} ਨਹੀਂਤਾਂ {
		e->ਮਾਪ_valid = 0;
		ਜੇ (oe_get_ਮਾਪ_slow(pack, e) != ਮਾਪ)
			BUG("'size' is supposed to be the object size!");
	}
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ oe_ਸੈੱਟ_delta_ਮਾਪ(ਬਣਤਰ packing_ਅੰਕੜੇ *pack,
				     ਬਣਤਰ object_ਇੰਦਰਾਜ *e,
				     ਮਾਪ_t ਮਾਪ)
{
	ਜੇ (ਮਾਪ < pack->oe_delta_ਮਾਪ_ਹੱਦਬੰਦੀ) {
		e->delta_ਮਾਪ = ਮਾਪ;
		e->delta_ਮਾਪ_valid = 1;
	} ਨਹੀਂਤਾਂ {
		packing_ਅੰਕੜੇ_ਤਾਲਾ(pack);
		ਜੇ (!pack->delta_ਮਾਪ_2)
			ALLOC_ARRAY(pack->delta_ਮਾਪ_2, pack->nr_alloc);
		packing_ਅੰਕੜੇ_unlock(pack);

		pack->delta_ਮਾਪ_2[e - pack->objects] = ਮਾਪ;
		e->delta_ਮਾਪ_valid = 0;
	}
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਵਿੱਚ_PACK(obj) oe_ਵਿੱਚ_pack(&ਮੰਜ਼ਿਲ_pack, obj)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਮਾਪ_3(obj) oe_ਮਾਪ(&ਮੰਜ਼ਿਲ_pack, obj)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਸੈੱਟ_ਮਾਪ(obj,ਮਾਪ) oe_ਸੈੱਟ_ਮਾਪ(&ਮੰਜ਼ਿਲ_pack, obj, ਮਾਪ)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DELTA_ਮਾਪ(obj) oe_delta_ਮਾਪ(&ਮੰਜ਼ਿਲ_pack, obj)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DELTA(obj) oe_delta(&ਮੰਜ਼ਿਲ_pack, obj)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DELTA_ਬੱਚਾ(obj) oe_delta_ਬੱਚਾ(&ਮੰਜ਼ਿਲ_pack, obj)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DELTA_SIBLING(obj) oe_delta_sibling(&ਮੰਜ਼ਿਲ_pack, obj)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਸੈੱਟ_DELTA(obj, val) oe_ਸੈੱਟ_delta(&ਮੰਜ਼ਿਲ_pack, obj, val)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਸੈੱਟ_DELTA_EXT(obj, oid) oe_ਸੈੱਟ_delta_ext(&ਮੰਜ਼ਿਲ_pack, obj, oid)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਸੈੱਟ_DELTA_ਮਾਪ(obj, val) oe_ਸੈੱਟ_delta_ਮਾਪ(&ਮੰਜ਼ਿਲ_pack, obj, val)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਸੈੱਟ_DELTA_ਬੱਚਾ(obj, val) oe_ਸੈੱਟ_delta_ਬੱਚਾ(&ਮੰਜ਼ਿਲ_pack, obj, val)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਸੈੱਟ_DELTA_SIBLING(obj, val) oe_ਸੈੱਟ_delta_sibling(&ਮੰਜ਼ਿਲ_pack, obj, val)

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਅਚਲ pack_usage[] = {
	N_("git pack-objects [-q | --progress | --all-progress] [--all-progress-implied]\n"
	   "                 [--no-reuse-delta] [--delta-base-offset] [--non-empty]\n"
	   "                 [--local] [--incremental] [--window=<n>] [--depth=<n>]\n"
	   "                 [--revs [--unpacked | --all]] [--keep-pack=<pack-name>]\n"
	   "                 [--cruft] [--cruft-expiration=<time>]\n"
	   "                 [--stdout [--filter=<filter-spec>] | <base-name>]\n"
	   "                 [--shallow] [--keep-true-parents] [--[no-]sparse]\n"
	   "                 [--name-hash-version=<n>] [--path-walk] < <object-list>"),
	ਨਿਰਾ
};

ਸਥਿਰ ਬਣਤਰ pack_idx_ਇੰਦਰਾਜ **written_list;
ਸਥਿਰ uint32_t nr_ਨਤੀਜਾ, nr_written, nr_ਵੇਖਿਆ;
ਸਥਿਰ ਬਣਤਰ bitmap_ਸੂਚਕਾਂਕ *bitmap_git;
ਸਥਿਰ uint32_t write_layer;

ਸਥਿਰ ਪੂਰਨ non_ਖਾਲੀ_ਥਾਵਾਂ;
ਸਥਿਰ ਪੂਰਨ reuse_delta = 1, reuse_object = 1;
ਸਥਿਰ ਪੂਰਨ keep_unreachable, unpack_unreachable, include_ਟੈਗ;
ਸਥਿਰ timestamp_t unpack_unreachable_expiration;
ਸਥਿਰ ਪੂਰਨ pack_loose_unreachable;
ਸਥਿਰ ਪੂਰਨ cruft;
ਸਥਿਰ ਪੂਰਨ shallow = 0;
ਸਥਿਰ timestamp_t cruft_expiration;
ਸਥਿਰ ਪੂਰਨ local;
ਸਥਿਰ ਪੂਰਨ have_non_local_packs;
ਸਥਿਰ ਪੂਰਨ incremental;
ਸਥਿਰ ਪੂਰਨ ignore_packed_keep_on_disk;
ਸਥਿਰ ਪੂਰਨ ignore_packed_keep_ਵਿੱਚ_core;
ਸਥਿਰ ਪੂਰਨ ignore_packed_keep_ਵਿੱਚ_core_open;
ਸਥਿਰ ਪੂਰਨ ignore_packed_keep_ਵਿੱਚ_core_has_cruft;
ਸਥਿਰ ਪੂਰਨ allow_ofs_delta;
ਸਥਿਰ ਬਣਤਰ pack_idx_option pack_idx_opts;
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਅਧਾਰ_name;
ਸਥਿਰ ਪੂਰਨ ਤਰੱਕੀ = 1;
ਸਥਿਰ ਪੂਰਨ ਖਿੜਕੀ = 10;
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ pack_ਮਾਪ_ਹੱਦਬੰਦੀ;
ਸਥਿਰ ਪੂਰਨ ਡੂੰਘਾਈ = 50;
ਸਥਿਰ ਪੂਰਨ delta_ਖੋਜ_ਧਾਗੇ;
ਸਥਿਰ ਪੂਰਨ pack_ਮੰਜ਼ਿਲ_stdout;
ਸਥਿਰ ਪੂਰਨ sparse;
ਸਥਿਰ ਪੂਰਨ thin;
ਸਥਿਰ ਪੂਰਨ ਰਸਤਾ_walk = -1;
ਸਥਿਰ ਪੂਰਨ ਅੰਕ_preferred_ਅਧਾਰ;
ਸਥਿਰ ਬਣਤਰ ਤਰੱਕੀ *ਤਰੱਕੀ_state;

ਸਥਿਰ ਬਣਤਰ bitmapped_pack *reuse_packfiles;
ਸਥਿਰ ਮਾਪ_t reuse_packfiles_nr;
ਸਥਿਰ ਮਾਪ_t reuse_packfiles_ਵਰਤਿਆ_nr;
ਸਥਿਰ uint32_t reuse_packfile_objects;
ਸਥਿਰ ਬਣਤਰ bitmap *reuse_packfile_bitmap;

ਸਥਿਰ ਪੂਰਨ use_bitmap_ਸੂਚਕਾਂਕ_default = 1;
ਸਥਿਰ ਪੂਰਨ use_bitmap_ਸੂਚਕਾਂਕ = -1;
ਸਥਿਰ ਗਿਣਤੀ {
	NO_PACK_REUSE = 0,
	SINGLE_PACK_REUSE,
	MULTI_PACK_REUSE,
} allow_pack_reuse = SINGLE_PACK_REUSE;
ਸਥਿਰ ਗਿਣਤੀ {
	WRITE_BITMAP_FALSE = 0,
	WRITE_BITMAP_QUIET,
	WRITE_BITMAP_TRUE,
} write_bitmap_ਸੂਚਕਾਂਕ;
ਸਥਿਰ uint16_t write_bitmap_options = BITMAP_OPT_HASH_CACHE;

ਸਥਿਰ ਪੂਰਨ exclude_promisor_objects;
ਸਥਿਰ ਪੂਰਨ exclude_promisor_objects_ਉੱਤਮ_effort;

ਸਥਿਰ ਪੂਰਨ use_delta_islands;

ਸਥਿਰ ਮਾਪ_t delta_cache_ਮਾਪ = 0;
ਸਥਿਰ ਮਾਪ_t max_delta_cache_ਮਾਪ = DEFAULT_DELTA_CACHE_ਮਾਪ;
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ cache_max_small_delta_ਮਾਪ = 1000;

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਖਿੜਕੀ_ਸਮ੍ਰਿਤੀ_ਹੱਦਬੰਦੀ = 0;

ਸਥਿਰ ਬਣਤਰ string_list uri_protocols = STRING_LIST_INIT_NODUP;

ਗਿਣਤੀ missing_action {
	MA_ERROR = 0,      /* fail if any missing objects are encountered */
	MA_ALLOW_ANY,      /* silently allow ALL missing objects */
	MA_ALLOW_PROMISOR, /* silently allow all missing PROMISOR objects */
};
ਸਥਿਰ ਗਿਣਤੀ missing_action arg_missing_action;
ਸਥਿਰ show_object_fn fn_show_object;

ਬਣਤਰ configured_exclusion {
	ਬਣਤਰ oidmap_ਇੰਦਰਾਜ e;
	ਅੱਖਰ *pack_hash_hex;
	ਅੱਖਰ *uri;
};
ਸਥਿਰ ਬਣਤਰ oidmap configured_exclusions;

ਸਥਿਰ ਬਣਤਰ oidset excluded_by_config;
ਸਥਿਰ ਪੂਰਨ name_hash_ਸੰਸਕਰਣ = -1;

ਗਿਣਤੀ stdin_packs_ਬਹੁਲਕ {
	STDIN_PACKS_ਬਹੁਲਕ_NONE,
	STDIN_PACKS_ਬਹੁਲਕ_STANDARD,
	STDIN_PACKS_ਬਹੁਲਕ_FOLLOW,
};

/**
 * Check whether the name_hash_version chosen by user input is appropriate,
 * and also validate whether it is compatible with other features.
 */
ਸਥਿਰ ਖਾਲੀ validate_name_hash_ਸੰਸਕਰਣ(ਖਾਲੀ)
{
	ਜੇ (name_hash_ਸੰਸਕਰਣ < 1 || name_hash_ਸੰਸਕਰਣ > 2)
		die(_("invalid --name-hash-version option: %d"), name_hash_ਸੰਸਕਰਣ);
	ਜੇ (write_bitmap_ਸੂਚਕਾਂਕ && name_hash_ਸੰਸਕਰਣ != 1) {
		warning(_("currently, --write-bitmap-index requires --name-hash-version=1"));
		name_hash_ਸੰਸਕਰਣ = 1;
	}
}

ਸਥਿਰ ਸਤਰਅੰਦਰ uint32_t pack_name_hash_fn(ਅਚਲ ਅੱਖਰ *name)
{
	ਸਥਿਰ ਪੂਰਨ ਵੇਖਿਆ_ਸੰਸਕਰਣ = -1;

	ਜੇ (ਵੇਖਿਆ_ਸੰਸਕਰਣ < 0)
		ਵੇਖਿਆ_ਸੰਸਕਰਣ = name_hash_ਸੰਸਕਰਣ;
	ਨਹੀਂਤਾਂ ਜੇ (ਵੇਖਿਆ_ਸੰਸਕਰਣ != name_hash_ਸੰਸਕਰਣ)
		BUG("name hash version changed from %d to %d mid-process",
		    ਵੇਖਿਆ_ਸੰਸਕਰਣ, name_hash_ਸੰਸਕਰਣ);

	ਚੋਣ (name_hash_ਸੰਸਕਰਣ) {
	ਹਾਲਤ 1:
		ਵਾਪਸ pack_name_hash(name);

	ਹਾਲਤ 2:
		ਵਾਪਸ pack_name_hash_v2((ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *)name);

	ਮੂਲ:
		BUG("invalid name-hash version: %d", name_hash_ਸੰਸਕਰਣ);
	}
}

/*
 * stats
 */
ਸਥਿਰ uint32_t written, written_delta;
ਸਥਿਰ uint32_t reused, reused_delta;

/*
 * Indexed commits
 */
ਸਥਿਰ ਬਣਤਰ commit **ਸੂਚਕਾਂਕਿਤ_commits;
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੂਚਕਾਂਕਿਤ_commits_nr;
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੂਚਕਾਂਕਿਤ_commits_alloc;

ਸਥਿਰ ਖਾਲੀ ਸੂਚਕਾਂਕ_commit_for_bitmap(ਬਣਤਰ commit *commit)
{
	ਜੇ (ਸੂਚਕਾਂਕਿਤ_commits_nr >= ਸੂਚਕਾਂਕਿਤ_commits_alloc) {
		ਸੂਚਕਾਂਕਿਤ_commits_alloc = (ਸੂਚਕਾਂਕਿਤ_commits_alloc + 32) * 2;
		REALLOC_ARRAY(ਸੂਚਕਾਂਕਿਤ_commits, ਸੂਚਕਾਂਕਿਤ_commits_alloc);
	}

	ਸੂਚਕਾਂਕਿਤ_commits[ਸੂਚਕਾਂਕਿਤ_commits_nr++] = commit;
}

ਸਥਿਰ ਖਾਲੀ *get_delta(ਬਣਤਰ object_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਮਾਪ, ਅਧਾਰ_ਮਾਪ;
	ਮਾਪ_t delta_ਮਾਪ_2;
	ਖਾਲੀ *buf, *ਅਧਾਰ_buf, *delta_buf;
	ਗਿਣਤੀ object_type type;
	ਮਾਪ_t ਮਾਪ_st = 0, ਅਧਾਰ_ਮਾਪ_st = 0;

	buf = odb_read_object(the_repository->objects, &ਇੰਦਰਾਜ->idx.oid,
			      &type, &ਮਾਪ_st);
	ਮਾਪ = cast_ਮਾਪ_t_ਮੰਜ਼ਿਲ_ulong(ਮਾਪ_st);
	ਜੇ (!buf)
		die(_("unable to read %s"), oid_ਮੰਜ਼ਿਲ_hex(&ਇੰਦਰਾਜ->idx.oid));
	ਅਧਾਰ_buf = odb_read_object(the_repository->objects,
				   &DELTA(ਇੰਦਰਾਜ)->idx.oid, &type,
				   &ਅਧਾਰ_ਮਾਪ_st);
	ਅਧਾਰ_ਮਾਪ = cast_ਮਾਪ_t_ਮੰਜ਼ਿਲ_ulong(ਅਧਾਰ_ਮਾਪ_st);
	ਜੇ (!ਅਧਾਰ_buf)
		die("unable to read %s",
		    oid_ਮੰਜ਼ਿਲ_hex(&DELTA(ਇੰਦਰਾਜ)->idx.oid));
	delta_buf = diff_delta(ਅਧਾਰ_buf, ਅਧਾਰ_ਮਾਪ,
			       buf, ਮਾਪ, &delta_ਮਾਪ_2, 0);
	/*
	 * We successfully computed this delta once but dropped it for
	 * memory reasons. Something is very wrong if this time we
	 * recompute and create a different delta.
	 */
	ਜੇ (!delta_buf || delta_ਮਾਪ_2 != DELTA_ਮਾਪ(ਇੰਦਰਾਜ))
		BUG("delta size changed");
	ਮੁਕਤ(buf);
	ਮੁਕਤ(ਅਧਾਰ_buf);
	ਵਾਪਸ delta_buf;
}

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ do_compress(ਖਾਲੀ **pptr, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਮਾਪ)
{
	git_zstream ਧਾਰਾ;
	ਖਾਲੀ *ਵਿੱਚ, *ਬਾਹਰਲਾ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ maxsize;
	ਬਣਤਰ repo_config_ਮੁੱਲਾਂ *cfg = repo_config_ਮੁੱਲਾਂ(the_repository);

	git_deflate_init(&ਧਾਰਾ, cfg->pack_compression_level);
	maxsize = git_deflate_bound(&ਧਾਰਾ, ਮਾਪ);

	ਵਿੱਚ = *pptr;
	ਬਾਹਰਲਾ = xmalloc(maxsize);
	*pptr = ਬਾਹਰਲਾ;

	ਧਾਰਾ.ਅਗਲਾ_ਵਿੱਚ = ਵਿੱਚ;
	ਧਾਰਾ.avail_ਵਿੱਚ = ਮਾਪ;
	ਧਾਰਾ.ਅਗਲਾ_ਬਾਹਰਲਾ = ਬਾਹਰਲਾ;
	ਧਾਰਾ.avail_ਬਾਹਰਲਾ = maxsize;
	ਜਦੋਂ (git_deflate(&ਧਾਰਾ, Z_ਅੰਤ) == Z_OK)
		; /* nothing */
	git_deflate_ਸਿਰਾ(&ਧਾਰਾ);

	ਮੁਕਤ(ਵਿੱਚ);
	ਵਾਪਸ ਧਾਰਾ.ਕੁੱਲ_ਬਾਹਰਲਾ;
}

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ write_large_blob_ਅੰਕੜੇ(ਬਣਤਰ odb_ਧਾਰਾ *st, ਬਣਤਰ hashfile *f,
					   ਅਚਲ ਬਣਤਰ object_id *oid)
{
	git_zstream ਧਾਰਾ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ ibuf[1024 * 16];
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ obuf[1024 * 16];
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ olen = 0;
	ਬਣਤਰ repo_config_ਮੁੱਲਾਂ *cfg = repo_config_ਮੁੱਲਾਂ(the_repository);

	git_deflate_init(&ਧਾਰਾ, cfg->pack_compression_level);

	ਲਈ (;;) {
		ssize_t readlen;
		ਪੂਰਨ zret = Z_OK;
		readlen = odb_ਧਾਰਾ_read(st, ibuf, ਆਕਾਰ(ibuf));
		ਜੇ (readlen == -1)
			die(_("unable to read %s"), oid_ਮੰਜ਼ਿਲ_hex(oid));

		ਧਾਰਾ.ਅਗਲਾ_ਵਿੱਚ = ibuf;
		ਧਾਰਾ.avail_ਵਿੱਚ = readlen;
		ਜਦੋਂ ((ਧਾਰਾ.avail_ਵਿੱਚ || readlen == 0) &&
		       (zret == Z_OK || zret == Z_BUF_ERROR)) {
			ਧਾਰਾ.ਅਗਲਾ_ਬਾਹਰਲਾ = obuf;
			ਧਾਰਾ.avail_ਬਾਹਰਲਾ = ਆਕਾਰ(obuf);
			zret = git_deflate(&ਧਾਰਾ, readlen ? 0 : Z_ਅੰਤ);
			hashwrite(f, obuf, ਧਾਰਾ.ਅਗਲਾ_ਬਾਹਰਲਾ - obuf);
			olen += ਧਾਰਾ.ਅਗਲਾ_ਬਾਹਰਲਾ - obuf;
		}
		ਜੇ (ਧਾਰਾ.avail_ਵਿੱਚ)
			die(_("deflate error (%d)"), zret);
		ਜੇ (readlen == 0) {
			ਜੇ (zret != Z_ਧਾਰਾ_ਸਿਰਾ)
				die(_("deflate error (%d)"), zret);
			ਰੋਕੋ;
		}
	}
	git_deflate_ਸਿਰਾ(&ਧਾਰਾ);
	ਵਾਪਸ olen;
}

/*
 * we are going to reuse the existing object data as is.  make
 * sure it is not corrupt.
 */
ਸਥਿਰ ਪੂਰਨ check_pack_inflate(ਬਣਤਰ packed_git *p,
		ਬਣਤਰ pack_ਖਿੜਕੀ **w_curs,
		off_t ਵਿੱਥ,
		off_t len,
		ਮਾਪ_t expect)
{
	git_zstream ਧਾਰਾ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ fakebuf[4096], *ਵਿੱਚ;
	ਪੂਰਨ st;

	memset(&ਧਾਰਾ, 0, ਆਕਾਰ(ਧਾਰਾ));
	git_inflate_init(&ਧਾਰਾ);
	ਕਰੋ {
		ਵਿੱਚ = use_pack(p, w_curs, ਵਿੱਥ, &ਧਾਰਾ.avail_ਵਿੱਚ);
		ਧਾਰਾ.ਅਗਲਾ_ਵਿੱਚ = ਵਿੱਚ;
		ਧਾਰਾ.ਅਗਲਾ_ਬਾਹਰਲਾ = fakebuf;
		ਧਾਰਾ.avail_ਬਾਹਰਲਾ = ਆਕਾਰ(fakebuf);
		st = git_inflate(&ਧਾਰਾ, Z_ਅੰਤ);
		ਵਿੱਥ += ਧਾਰਾ.ਅਗਲਾ_ਵਿੱਚ - ਵਿੱਚ;
	} ਜਦੋਂ (st == Z_OK || st == Z_BUF_ERROR);
	git_inflate_ਸਿਰਾ(&ਧਾਰਾ);
	ਵਾਪਸ (st == Z_ਧਾਰਾ_ਸਿਰਾ &&
		ਧਾਰਾ.ਕੁੱਲ_ਬਾਹਰਲਾ == expect &&
		ਧਾਰਾ.ਕੁੱਲ_ਵਿੱਚ == len) ? 0 : -1;
}

ਸਥਿਰ ਖਾਲੀ copy_pack_ਅੰਕੜੇ(ਬਣਤਰ hashfile *f,
		ਬਣਤਰ packed_git *p,
		ਬਣਤਰ pack_ਖਿੜਕੀ **w_curs,
		off_t ਵਿੱਥ,
		off_t len)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *ਵਿੱਚ;
	ਮਾਪ_t avail;

	ਜਦੋਂ (len) {
		ਵਿੱਚ = use_pack(p, w_curs, ਵਿੱਥ, &avail);
		ਜੇ (avail > len)
			avail = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ)len;
		hashwrite(f, ਵਿੱਚ, avail);
		ਵਿੱਥ += avail;
		len -= avail;
	}
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ oe_ਮਾਪ_greater_than(ਬਣਤਰ packing_ਅੰਕੜੇ *pack,
				       ਅਚਲ ਬਣਤਰ object_ਇੰਦਰਾਜ *lhs,
				       ਮਾਪ_t rhs)
{
	ਜੇ (lhs->ਮਾਪ_valid)
		ਵਾਪਸ lhs->ਮਾਪ_2 > rhs;
	ਜੇ (rhs < pack->oe_ਮਾਪ_ਹੱਦਬੰਦੀ) /* rhs < 2^x <= lhs ? */
		ਵਾਪਸ 1;
	ਵਾਪਸ oe_get_ਮਾਪ_slow(pack, lhs) > rhs;
}

/* Return 0 if we will bust the pack-size limit */
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ write_no_reuse_object(ਬਣਤਰ hashfile *f, ਬਣਤਰ object_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ,
					   ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਹੱਦਬੰਦੀ, ਪੂਰਨ usable_delta)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਮਾਪ, datalen;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ ਸਿਰਲੇਖ[MAX_PACK_OBJECT_ਸਿਰਲੇਖ],
		      dheader[MAX_PACK_OBJECT_ਸਿਰਲੇਖ];
	ਬਿਨਾਨਿਸ਼ਾਨੀ hdrlen;
	ਗਿਣਤੀ object_type type;
	ਖਾਲੀ *buf;
	ਬਣਤਰ odb_ਧਾਰਾ *st = ਨਿਰਾ;
	ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ hashsz = the_hash_algo->rawsz;

	ਜੇ (!usable_delta) {
		ਜੇ (oe_type(ਇੰਦਰਾਜ) == OBJ_BLOB &&
		    oe_ਮਾਪ_greater_than(&ਮੰਜ਼ਿਲ_pack, ਇੰਦਰਾਜ,
					 repo_settings_get_big_file_ਹੱਦ(the_repository)) &&
		    (st = odb_ਧਾਰਾ_from_object(the_repository->objects, &ਇੰਦਰਾਜ->idx.oid,
						 ਨਿਰਾ)) != ਨਿਰਾ) {
			buf = ਨਿਰਾ;
			type = st->type;
			ਮਾਪ = st->ਮਾਪ;
		} ਨਹੀਂਤਾਂ {
			ਮਾਪ_t ਮਾਪ_st = 0;
			buf = odb_read_object(the_repository->objects,
					      &ਇੰਦਰਾਜ->idx.oid, &type,
					      &ਮਾਪ_st);
			ਮਾਪ = cast_ਮਾਪ_t_ਮੰਜ਼ਿਲ_ulong(ਮਾਪ_st);
			ਜੇ (!buf)
				die(_("unable to read %s"),
				    oid_ਮੰਜ਼ਿਲ_hex(&ਇੰਦਰਾਜ->idx.oid));
		}
		/*
		 * make sure no cached delta data remains from a
		 * previous attempt before a pack split occurred.
		 */
		FREE_AND_NULL(ਇੰਦਰਾਜ->delta_ਅੰਕੜੇ);
		ਇੰਦਰਾਜ->z_delta_ਮਾਪ = 0;
	} ਨਹੀਂਤਾਂ ਜੇ (ਇੰਦਰਾਜ->delta_ਅੰਕੜੇ) {
		ਮਾਪ = DELTA_ਮਾਪ(ਇੰਦਰਾਜ);
		buf = ਇੰਦਰਾਜ->delta_ਅੰਕੜੇ;
		ਇੰਦਰਾਜ->delta_ਅੰਕੜੇ = ਨਿਰਾ;
		type = (allow_ofs_delta && DELTA(ਇੰਦਰਾਜ)->idx.ਵਿੱਥ) ?
			OBJ_OFS_DELTA : OBJ_REF_DELTA;
	} ਨਹੀਂਤਾਂ {
		buf = get_delta(ਇੰਦਰਾਜ);
		ਮਾਪ = DELTA_ਮਾਪ(ਇੰਦਰਾਜ);
		type = (allow_ofs_delta && DELTA(ਇੰਦਰਾਜ)->idx.ਵਿੱਥ) ?
			OBJ_OFS_DELTA : OBJ_REF_DELTA;
	}

	ਜੇ (st)	/* large blob case, just assume we don't compress well */
		datalen = ਮਾਪ;
	ਨਹੀਂਤਾਂ ਜੇ (ਇੰਦਰਾਜ->z_delta_ਮਾਪ)
		datalen = ਇੰਦਰਾਜ->z_delta_ਮਾਪ;
	ਨਹੀਂਤਾਂ
		datalen = do_compress(&buf, ਮਾਪ);

	/*
	 * The object header is a byte of 'type' followed by zero or
	 * more bytes of length.
	 */
	hdrlen = encode_ਵਿੱਚ_pack_object_ਸਿਰਲੇਖ(ਸਿਰਲੇਖ, ਆਕਾਰ(ਸਿਰਲੇਖ),
					      type, ਮਾਪ);

	ਜੇ (type == OBJ_OFS_DELTA) {
		/*
		 * Deltas with relative base contain an additional
		 * encoding of the relative offset for the delta
		 * base from this object's position in the pack.
		 */
		off_t ofs = ਇੰਦਰਾਜ->idx.ਵਿੱਥ - DELTA(ਇੰਦਰਾਜ)->idx.ਵਿੱਥ;
		ਬਿਨਾਨਿਸ਼ਾਨੀ pos = ਆਕਾਰ(dheader) - 1;
		dheader[pos] = ofs & 127;
		ਜਦੋਂ (ofs >>= 7)
			dheader[--pos] = 128 | (--ofs & 127);
		ਜੇ (ਹੱਦਬੰਦੀ && hdrlen + ਆਕਾਰ(dheader) - pos + datalen + hashsz >= ਹੱਦਬੰਦੀ) {
			ਜੇ (st)
				odb_ਧਾਰਾ_close(st);
			ਮੁਕਤ(buf);
			ਵਾਪਸ 0;
		}
		hashwrite(f, ਸਿਰਲੇਖ, hdrlen);
		hashwrite(f, dheader + pos, ਆਕਾਰ(dheader) - pos);
		hdrlen += ਆਕਾਰ(dheader) - pos;
	} ਨਹੀਂਤਾਂ ਜੇ (type == OBJ_REF_DELTA) {
		/*
		 * Deltas with a base reference contain
		 * additional bytes for the base object ID.
		 */
		ਜੇ (ਹੱਦਬੰਦੀ && hdrlen + hashsz + datalen + hashsz >= ਹੱਦਬੰਦੀ) {
			ਜੇ (st)
				odb_ਧਾਰਾ_close(st);
			ਮੁਕਤ(buf);
			ਵਾਪਸ 0;
		}
		hashwrite(f, ਸਿਰਲੇਖ, hdrlen);
		hashwrite(f, DELTA(ਇੰਦਰਾਜ)->idx.oid.hash, hashsz);
		hdrlen += hashsz;
	} ਨਹੀਂਤਾਂ {
		ਜੇ (ਹੱਦਬੰਦੀ && hdrlen + datalen + hashsz >= ਹੱਦਬੰਦੀ) {
			ਜੇ (st)
				odb_ਧਾਰਾ_close(st);
			ਮੁਕਤ(buf);
			ਵਾਪਸ 0;
		}
		hashwrite(f, ਸਿਰਲੇਖ, hdrlen);
	}
	ਜੇ (st) {
		datalen = write_large_blob_ਅੰਕੜੇ(st, f, &ਇੰਦਰਾਜ->idx.oid);
		odb_ਧਾਰਾ_close(st);
	} ਨਹੀਂਤਾਂ {
		hashwrite(f, buf, datalen);
		ਮੁਕਤ(buf);
	}

	ਵਾਪਸ hdrlen + datalen;
}

/* Return 0 if we will bust the pack-size limit */
ਸਥਿਰ off_t write_reuse_object(ਬਣਤਰ hashfile *f, ਬਣਤਰ object_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ,
				ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਹੱਦਬੰਦੀ, ਪੂਰਨ usable_delta)
{
	ਬਣਤਰ packed_git *p = ਵਿੱਚ_PACK(ਇੰਦਰਾਜ);
	ਬਣਤਰ pack_ਖਿੜਕੀ *w_curs = ਨਿਰਾ;
	uint32_t pos;
	off_t ਵਿੱਥ, cur;
	ਗਿਣਤੀ object_type type = oe_type(ਇੰਦਰਾਜ);
	ਗਿਣਤੀ object_type ਵਿੱਚ_pack_type;
	off_t datalen;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ ਸਿਰਲੇਖ[MAX_PACK_OBJECT_ਸਿਰਲੇਖ],
		      dheader[MAX_PACK_OBJECT_ਸਿਰਲੇਖ];
	ਬਿਨਾਨਿਸ਼ਾਨੀ hdrlen;
	ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ hashsz = the_hash_algo->rawsz;
	ਮਾਪ_t ਇੰਦਰਾਜ_ਮਾਪ;

	cur = ਇੰਦਰਾਜ->ਵਿੱਚ_pack_ਵਿੱਥ;
	ਵਿੱਚ_pack_type = unpack_object_ਸਿਰਲੇਖ(p, &w_curs, &cur, &ਇੰਦਰਾਜ_ਮਾਪ);
	ਜੇ (ਵਿੱਚ_pack_type < 0)
		die(_("write_reuse_object: unable to parse object header of %s"),
		    oid_ਮੰਜ਼ਿਲ_hex(&ਇੰਦਰਾਜ->idx.oid));

	ਜੇ (DELTA(ਇੰਦਰਾਜ))
		type = (allow_ofs_delta && DELTA(ਇੰਦਰਾਜ)->idx.ਵਿੱਥ) ?
			OBJ_OFS_DELTA : OBJ_REF_DELTA;
	hdrlen = encode_ਵਿੱਚ_pack_object_ਸਿਰਲੇਖ(ਸਿਰਲੇਖ, ਆਕਾਰ(ਸਿਰਲੇਖ),
					      type, ਇੰਦਰਾਜ_ਮਾਪ);

	ਵਿੱਥ = ਇੰਦਰਾਜ->ਵਿੱਚ_pack_ਵਿੱਥ;
	ਜੇ (ਵਿੱਥ_ਮੰਜ਼ਿਲ_pack_pos(p, ਵਿੱਥ, &pos) < 0)
		die(_("write_reuse_object: could not locate %s, expected at "
		      "offset %"PRIuMAX" in pack %s"),
		    oid_ਮੰਜ਼ਿਲ_hex(&ਇੰਦਰਾਜ->idx.oid), (uintmax_t)ਵਿੱਥ,
		    p->pack_name);
	datalen = pack_pos_ਮੰਜ਼ਿਲ_ਵਿੱਥ(p, pos + 1) - ਵਿੱਥ;
	ਜੇ (!pack_ਮੰਜ਼ਿਲ_stdout && p->ਸੂਚਕਾਂਕ_ਸੰਸਕਰਣ > 1 &&
	    check_pack_ਚੱਕਰੀ_ਜਾਂਚ(p, &w_curs, ਵਿੱਥ, datalen,
			   pack_pos_ਮੰਜ਼ਿਲ_ਸੂਚਕਾਂਕ(p, pos))) {
		error(_("bad packed object CRC for %s"),
		      oid_ਮੰਜ਼ਿਲ_hex(&ਇੰਦਰਾਜ->idx.oid));
		unuse_pack(&w_curs);
		ਵਾਪਸ write_no_reuse_object(f, ਇੰਦਰਾਜ, ਹੱਦਬੰਦੀ, usable_delta);
	}

	ਵਿੱਥ += ਇੰਦਰਾਜ->ਵਿੱਚ_pack_ਸਿਰਲੇਖ_ਮਾਪ;
	datalen -= ਇੰਦਰਾਜ->ਵਿੱਚ_pack_ਸਿਰਲੇਖ_ਮਾਪ;

	ਜੇ (!pack_ਮੰਜ਼ਿਲ_stdout && p->ਸੂਚਕਾਂਕ_ਸੰਸਕਰਣ == 1 &&
	    check_pack_inflate(p, &w_curs, ਵਿੱਥ, datalen, ਇੰਦਰਾਜ_ਮਾਪ)) {
		error(_("corrupt packed object for %s"),
		      oid_ਮੰਜ਼ਿਲ_hex(&ਇੰਦਰਾਜ->idx.oid));
		unuse_pack(&w_curs);
		ਵਾਪਸ write_no_reuse_object(f, ਇੰਦਰਾਜ, ਹੱਦਬੰਦੀ, usable_delta);
	}

	ਜੇ (type == OBJ_OFS_DELTA) {
		off_t ofs = ਇੰਦਰਾਜ->idx.ਵਿੱਥ - DELTA(ਇੰਦਰਾਜ)->idx.ਵਿੱਥ;
		ਬਿਨਾਨਿਸ਼ਾਨੀ pos = ਆਕਾਰ(dheader) - 1;
		dheader[pos] = ofs & 127;
		ਜਦੋਂ (ofs >>= 7)
			dheader[--pos] = 128 | (--ofs & 127);
		ਜੇ (ਹੱਦਬੰਦੀ && hdrlen + ਆਕਾਰ(dheader) - pos + datalen + hashsz >= ਹੱਦਬੰਦੀ) {
			unuse_pack(&w_curs);
			ਵਾਪਸ 0;
		}
		hashwrite(f, ਸਿਰਲੇਖ, hdrlen);
		hashwrite(f, dheader + pos, ਆਕਾਰ(dheader) - pos);
		hdrlen += ਆਕਾਰ(dheader) - pos;
		reused_delta++;
	} ਨਹੀਂਤਾਂ ਜੇ (type == OBJ_REF_DELTA) {
		ਜੇ (ਹੱਦਬੰਦੀ && hdrlen + hashsz + datalen + hashsz >= ਹੱਦਬੰਦੀ) {
			unuse_pack(&w_curs);
			ਵਾਪਸ 0;
		}
		hashwrite(f, ਸਿਰਲੇਖ, hdrlen);
		hashwrite(f, DELTA(ਇੰਦਰਾਜ)->idx.oid.hash, hashsz);
		hdrlen += hashsz;
		reused_delta++;
	} ਨਹੀਂਤਾਂ {
		ਜੇ (ਹੱਦਬੰਦੀ && hdrlen + datalen + hashsz >= ਹੱਦਬੰਦੀ) {
			unuse_pack(&w_curs);
			ਵਾਪਸ 0;
		}
		hashwrite(f, ਸਿਰਲੇਖ, hdrlen);
	}
	copy_pack_ਅੰਕੜੇ(f, p, &w_curs, ਵਿੱਥ, datalen);
	unuse_pack(&w_curs);
	reused++;
	ਵਾਪਸ hdrlen + datalen;
}

/* Return 0 if we will bust the pack-size limit */
ਸਥਿਰ off_t write_object(ਬਣਤਰ hashfile *f,
			  ਬਣਤਰ object_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ,
			  off_t write_ਵਿੱਥ)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਹੱਦਬੰਦੀ;
	off_t len;
	ਪੂਰਨ usable_delta, ਮੰਜ਼ਿਲ_reuse;

	ਜੇ (!pack_ਮੰਜ਼ਿਲ_stdout)
		ਚੱਕਰੀ_ਜਾਂਚ_32_begin(f);

	/* apply size limit if limited packsize and not first object */
	ਜੇ (!pack_ਮਾਪ_ਹੱਦਬੰਦੀ || !nr_written)
		ਹੱਦਬੰਦੀ = 0;
	ਨਹੀਂਤਾਂ ਜੇ (pack_ਮਾਪ_ਹੱਦਬੰਦੀ <= write_ਵਿੱਥ)
		/*
		 * the earlier object did not fit the limit; avoid
		 * mistaking this with unlimited (i.e. limit = 0).
		 */
		ਹੱਦਬੰਦੀ = 1;
	ਨਹੀਂਤਾਂ
		ਹੱਦਬੰਦੀ = pack_ਮਾਪ_ਹੱਦਬੰਦੀ - write_ਵਿੱਥ;

	ਜੇ (!DELTA(ਇੰਦਰਾਜ))
		usable_delta = 0;	/* no delta */
	ਨਹੀਂਤਾਂ ਜੇ (!pack_ਮਾਪ_ਹੱਦਬੰਦੀ)
	       usable_delta = 1;	/* unlimited packfile */
	ਨਹੀਂਤਾਂ ਜੇ (DELTA(ਇੰਦਰਾਜ)->idx.ਵਿੱਥ == (off_t)-1)
		usable_delta = 0;	/* base was written to another pack */
	ਨਹੀਂਤਾਂ ਜੇ (DELTA(ਇੰਦਰਾਜ)->idx.ਵਿੱਥ)
		usable_delta = 1;	/* base already exists in this pack */
	ਨਹੀਂਤਾਂ
		usable_delta = 0;	/* base could end up in another pack */

	ਜੇ (!reuse_object)
		ਮੰਜ਼ਿਲ_reuse = 0;	/* explicit */
	ਨਹੀਂਤਾਂ ਜੇ (!ਵਿੱਚ_PACK(ਇੰਦਰਾਜ))
		ਮੰਜ਼ਿਲ_reuse = 0;	/* can't reuse what we don't have */
	ਨਹੀਂਤਾਂ ਜੇ (oe_type(ਇੰਦਰਾਜ) == OBJ_REF_DELTA ||
		 oe_type(ਇੰਦਰਾਜ) == OBJ_OFS_DELTA)
				/* check_object() decided it for us ... */
		ਮੰਜ਼ਿਲ_reuse = usable_delta;
				/* ... but pack split may override that */
	ਨਹੀਂਤਾਂ ਜੇ (oe_type(ਇੰਦਰਾਜ) != ਇੰਦਰਾਜ->ਵਿੱਚ_pack_type)
		ਮੰਜ਼ਿਲ_reuse = 0;	/* pack has delta which is unusable */
	ਨਹੀਂਤਾਂ ਜੇ (DELTA(ਇੰਦਰਾਜ))
		ਮੰਜ਼ਿਲ_reuse = 0;	/* we want to pack afresh */
	ਨਹੀਂਤਾਂ
		ਮੰਜ਼ਿਲ_reuse = 1;	/* we have it in-pack undeltified,
				 * and we do not need to deltify it.
				 */

	ਜੇ (!ਮੰਜ਼ਿਲ_reuse)
		len = write_no_reuse_object(f, ਇੰਦਰਾਜ, ਹੱਦਬੰਦੀ, usable_delta);
	ਨਹੀਂਤਾਂ
		len = write_reuse_object(f, ਇੰਦਰਾਜ, ਹੱਦਬੰਦੀ, usable_delta);
	ਜੇ (!len)
		ਵਾਪਸ 0;

	ਜੇ (usable_delta)
		written_delta++;
	written++;
	ਜੇ (!pack_ਮੰਜ਼ਿਲ_stdout)
		ਇੰਦਰਾਜ->idx.ਚੱਕਰੀ_ਜਾਂਚ_32 = ਚੱਕਰੀ_ਜਾਂਚ_32_ਸਿਰਾ(f);
	ਵਾਪਸ len;
}

ਗਿਣਤੀ write_one_ਸਥਿਤੀ {
	WRITE_ONE_SKIP = -1, /* already written */
	WRITE_ONE_BREAK = 0, /* writing this will bust the limit; not written */
	WRITE_ONE_WRITTEN = 1, /*ਸਾਧਾਰਨ*/
	WRITE_ONE_RECURSIVE = 2 /* already scheduled to be written */
};

ਸਥਿਰ ਗਿਣਤੀ write_one_ਸਥਿਤੀ write_one(ਬਣਤਰ hashfile *f,
				       ਬਣਤਰ object_ਇੰਦਰਾਜ *e,
				       off_t *ਵਿੱਥ)
{
	off_t ਮਾਪ;
	ਪੂਰਨ recursing;

	/*
	 * we set offset to 1 (which is an impossible value) to mark
	 * the fact that this object is involved in "write its base
	 * first before writing a deltified object" recursion.
	 */
	recursing = (e->idx.ਵਿੱਥ == 1);
	ਜੇ (recursing) {
		warning(_("recursive delta detected for object %s"),
			oid_ਮੰਜ਼ਿਲ_hex(&e->idx.oid));
		ਵਾਪਸ WRITE_ONE_RECURSIVE;
	} ਨਹੀਂਤਾਂ ਜੇ (e->idx.ਵਿੱਥ || e->preferred_ਅਧਾਰ) {
		/* offset is non zero if object is written already. */
		ਵਾਪਸ WRITE_ONE_SKIP;
	}

	/* if we are deltified, write out base object first. */
	ਜੇ (DELTA(e)) {
		e->idx.ਵਿੱਥ = 1; /* now recurse */
		ਚੋਣ (write_one(f, DELTA(e), ਵਿੱਥ)) {
		ਹਾਲਤ WRITE_ONE_RECURSIVE:
			/* we cannot depend on this one */
			ਸੈੱਟ_DELTA(e, ਨਿਰਾ);
			ਰੋਕੋ;
		ਮੂਲ:
			ਰੋਕੋ;
		ਹਾਲਤ WRITE_ONE_BREAK:
			e->idx.ਵਿੱਥ = recursing;
			ਵਾਪਸ WRITE_ONE_BREAK;
		}
	}

	e->idx.ਵਿੱਥ = *ਵਿੱਥ;
	ਮਾਪ = write_object(f, e, *ਵਿੱਥ);
	ਜੇ (!ਮਾਪ) {
		e->idx.ਵਿੱਥ = recursing;
		ਵਾਪਸ WRITE_ONE_BREAK;
	}
	written_list[nr_written++] = &e->idx;

	/* make sure off_t is sufficiently large not to wrap */
	ਜੇ (signed_ਜੋੜੋ_overflows(*ਵਿੱਥ, ਮਾਪ))
		die(_("pack too large for current definition of off_t"));
	*ਵਿੱਥ += ਮਾਪ;
	ਵਾਪਸ WRITE_ONE_WRITTEN;
}

ਸਥਿਰ ਪੂਰਨ mark_tagged(ਅਚਲ ਬਣਤਰ reference *ref, ਖਾਲੀ *cb_ਅੰਕੜੇ ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ object_id peeled;
	ਬਣਤਰ object_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ = packlist_ਲੱਭੋ(&ਮੰਜ਼ਿਲ_pack, ref->oid);

	ਜੇ (ਇੰਦਰਾਜ)
		ਇੰਦਰਾਜ->tagged = 1;
	ਜੇ (!reference_get_peeled_oid(the_repository, ref, &peeled)) {
		ਇੰਦਰਾਜ = packlist_ਲੱਭੋ(&ਮੰਜ਼ਿਲ_pack, &peeled);
		ਜੇ (ਇੰਦਰਾਜ)
			ਇੰਦਰਾਜ->tagged = 1;
	}
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ oe_layer(ਬਣਤਰ packing_ਅੰਕੜੇ *pack,
				     ਬਣਤਰ object_ਇੰਦਰਾਜ *e)
{
	ਜੇ (!pack->layer)
		ਵਾਪਸ 0;
	ਵਾਪਸ pack->layer[e - pack->objects];
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ ਜੋੜੋ_ਮੰਜ਼ਿਲ_write_ਕ੍ਰਮ(ਬਣਤਰ object_ਇੰਦਰਾਜ **wo,
			       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *endp,
			       ਬਣਤਰ object_ਇੰਦਰਾਜ *e)
{
	ਜੇ (e->filled || oe_layer(&ਮੰਜ਼ਿਲ_pack, e) != write_layer)
		ਵਾਪਸ;
	wo[(*endp)++] = e;
	e->filled = 1;
}

ਸਥਿਰ ਖਾਲੀ ਜੋੜੋ_descendants_ਮੰਜ਼ਿਲ_write_ਕ੍ਰਮ(ਬਣਤਰ object_ਇੰਦਰਾਜ **wo,
					   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *endp,
					   ਬਣਤਰ object_ਇੰਦਰਾਜ *e)
{
	ਪੂਰਨ ਜੋੜੋ_ਮੰਜ਼ਿਲ_ਕ੍ਰਮ = 1;
	ਜਦੋਂ (e) {
		ਜੇ (ਜੋੜੋ_ਮੰਜ਼ਿਲ_ਕ੍ਰਮ) {
			ਬਣਤਰ object_ਇੰਦਰਾਜ *s;
			/* add this node... */
			ਜੋੜੋ_ਮੰਜ਼ਿਲ_write_ਕ੍ਰਮ(wo, endp, e);
			/* all its siblings... */
			ਲਈ (s = DELTA_SIBLING(e); s; s = DELTA_SIBLING(s)) {
				ਜੋੜੋ_ਮੰਜ਼ਿਲ_write_ਕ੍ਰਮ(wo, endp, s);
			}
		}
		/* drop down a level to add left subtree nodes if possible */
		ਜੇ (DELTA_ਬੱਚਾ(e)) {
			ਜੋੜੋ_ਮੰਜ਼ਿਲ_ਕ੍ਰਮ = 1;
			e = DELTA_ਬੱਚਾ(e);
		} ਨਹੀਂਤਾਂ {
			ਜੋੜੋ_ਮੰਜ਼ਿਲ_ਕ੍ਰਮ = 0;
			/* our sibling might have some children, it is next */
			ਜੇ (DELTA_SIBLING(e)) {
				e = DELTA_SIBLING(e);
				ਜਾਰੀ;
			}
			/* go back to our parent node */
			e = DELTA(e);
			ਜਦੋਂ (e && !DELTA_SIBLING(e)) {
				/* we're on the right side of a subtree, keep
				 * going up until we can go right again */
				e = DELTA(e);
			}
			ਜੇ (!e) {
				/* done- we hit our original root node */
				ਵਾਪਸ;
			}
			/* pass it off to sibling at this level */
			e = DELTA_SIBLING(e);
		}
	};
}

ਸਥਿਰ ਖਾਲੀ ਜੋੜੋ_family_ਮੰਜ਼ਿਲ_write_ਕ੍ਰਮ(ਬਣਤਰ object_ਇੰਦਰਾਜ **wo,
				      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *endp,
				      ਬਣਤਰ object_ਇੰਦਰਾਜ *e)
{
	ਬਣਤਰ object_ਇੰਦਰਾਜ *ਜੜ੍ਹ;

	ਲਈ (ਜੜ੍ਹ = e; DELTA(ਜੜ੍ਹ); ਜੜ੍ਹ = DELTA(ਜੜ੍ਹ))
		; /* nothing */
	ਜੋੜੋ_descendants_ਮੰਜ਼ਿਲ_write_ਕ੍ਰਮ(wo, endp, ਜੜ੍ਹ);
}

ਸਥਿਰ ਖਾਲੀ compute_layer_ਕ੍ਰਮ(ਬਣਤਰ object_ਇੰਦਰਾਜ **wo, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *wo_ਸਿਰਾ)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i, ਆਖਰੀ_untagged;
	ਬਣਤਰ object_ਇੰਦਰਾਜ *objects = ਮੰਜ਼ਿਲ_pack.objects;

	ਲਈ (i = 0; i < ਮੰਜ਼ਿਲ_pack.nr_objects; i++) {
		ਜੇ (objects[i].tagged)
			ਰੋਕੋ;
		ਜੋੜੋ_ਮੰਜ਼ਿਲ_write_ਕ੍ਰਮ(wo, wo_ਸਿਰਾ, &objects[i]);
	}
	ਆਖਰੀ_untagged = i;

	/*
	 * Then fill all the tagged tips.
	 */
	ਲਈ (; i < ਮੰਜ਼ਿਲ_pack.nr_objects; i++) {
		ਜੇ (objects[i].tagged)
			ਜੋੜੋ_ਮੰਜ਼ਿਲ_write_ਕ੍ਰਮ(wo, wo_ਸਿਰਾ, &objects[i]);
	}

	/*
	 * And then all remaining commits and tags.
	 */
	ਲਈ (i = ਆਖਰੀ_untagged; i < ਮੰਜ਼ਿਲ_pack.nr_objects; i++) {
		ਜੇ (oe_type(&objects[i]) != OBJ_COMMIT &&
		    oe_type(&objects[i]) != OBJ_ਟੈਗ)
			ਜਾਰੀ;
		ਜੋੜੋ_ਮੰਜ਼ਿਲ_write_ਕ੍ਰਮ(wo, wo_ਸਿਰਾ, &objects[i]);
	}

	/*
	 * And then all the trees.
	 */
	ਲਈ (i = ਆਖਰੀ_untagged; i < ਮੰਜ਼ਿਲ_pack.nr_objects; i++) {
		ਜੇ (oe_type(&objects[i]) != OBJ_ਰੁੱਖ)
			ਜਾਰੀ;
		ਜੋੜੋ_ਮੰਜ਼ਿਲ_write_ਕ੍ਰਮ(wo, wo_ਸਿਰਾ, &objects[i]);
	}

	/*
	 * Finally all the rest in really tight order
	 */
	ਲਈ (i = ਆਖਰੀ_untagged; i < ਮੰਜ਼ਿਲ_pack.nr_objects; i++) {
		ਜੇ (!objects[i].filled && oe_layer(&ਮੰਜ਼ਿਲ_pack, &objects[i]) == write_layer)
			ਜੋੜੋ_family_ਮੰਜ਼ਿਲ_write_ਕ੍ਰਮ(wo, wo_ਸਿਰਾ, &objects[i]);
	}
}

ਸਥਿਰ ਬਣਤਰ object_ਇੰਦਰਾਜ **compute_write_ਕ੍ਰਮ(ਖਾਲੀ)
{
	uint32_t max_layers = 1;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i, wo_ਸਿਰਾ;

	ਬਣਤਰ object_ਇੰਦਰਾਜ **wo;
	ਬਣਤਰ object_ਇੰਦਰਾਜ *objects = ਮੰਜ਼ਿਲ_pack.objects;

	ਲਈ (i = 0; i < ਮੰਜ਼ਿਲ_pack.nr_objects; i++) {
		objects[i].tagged = 0;
		objects[i].filled = 0;
		ਸੈੱਟ_DELTA_ਬੱਚਾ(&objects[i], ਨਿਰਾ);
		ਸੈੱਟ_DELTA_SIBLING(&objects[i], ਨਿਰਾ);
	}

	/*
	 * Fully connect delta_child/delta_sibling network.
	 * Make sure delta_sibling is sorted in the original
	 * recency order.
	 */
	ਲਈ (i = ਮੰਜ਼ਿਲ_pack.nr_objects; i > 0;) {
		ਬਣਤਰ object_ਇੰਦਰਾਜ *e = &objects[--i];
		ਜੇ (!DELTA(e))
			ਜਾਰੀ;
		/* Mark me as the first child */
		e->delta_sibling_idx = DELTA(e)->delta_ਬੱਚਾ_idx;
		ਸੈੱਟ_DELTA_ਬੱਚਾ(DELTA(e), e);
	}

	/*
	 * Mark objects that are at the tip of tags.
	 */
	refs_for_each_ਟੈਗ_ref(get_main_ref_store(the_repository), mark_tagged,
			      ਨਿਰਾ);

	ਜੇ (use_delta_islands) {
		max_layers = compute_pack_layers(&ਮੰਜ਼ਿਲ_pack);
		free_island_marks();
	}

	ALLOC_ARRAY(wo, ਮੰਜ਼ਿਲ_pack.nr_objects);
	wo_ਸਿਰਾ = 0;

	ਲਈ (; write_layer < max_layers; ++write_layer)
		compute_layer_ਕ੍ਰਮ(wo, &wo_ਸਿਰਾ);

	ਜੇ (wo_ਸਿਰਾ != ਮੰਜ਼ਿਲ_pack.nr_objects)
		die(_("ordered %u objects, expected %"PRIu32),
		    wo_ਸਿਰਾ, ਮੰਜ਼ਿਲ_pack.nr_objects);

	ਵਾਪਸ wo;
}


/*
 * A reused set of objects. All objects in a chunk have the same
 * relative position in the original packfile and the generated
 * packfile.
 */

ਸਥਿਰ ਬਣਤਰ reused_chunk {
	/* The offset of the first object of this chunk in the original
	 * packfile. */
	off_t original;
	/* The difference for "original" minus the offset of the first object of
	 * this chunk in the generated packfile. */
	off_t difference;
} *reused_chunks;
ਸਥਿਰ ਪੂਰਨ reused_chunks_nr;
ਸਥਿਰ ਪੂਰਨ reused_chunks_alloc;

ਸਥਿਰ ਖਾਲੀ ਰਿਕਾਰਡ_reused_object(off_t where, off_t ਵਿੱਥ)
{
	ਜੇ (reused_chunks_nr && reused_chunks[reused_chunks_nr-1].difference == ਵਿੱਥ)
		ਵਾਪਸ;

	ALLOC_GROW(reused_chunks, reused_chunks_nr + 1,
		   reused_chunks_alloc);
	reused_chunks[reused_chunks_nr].original = where;
	reused_chunks[reused_chunks_nr].difference = ਵਿੱਥ;
	reused_chunks_nr++;
}

/*
 * Binary search to find the chunk that "where" is in. Note
 * that we're not looking for an exact match, just the first
 * chunk that contains it (which implicitly ends at the start
 * of the next chunk.
 */
ਸਥਿਰ off_t ਲੱਭੋ_reused_ਵਿੱਥ(off_t where)
{
	ਪੂਰਨ lo = 0, hi = reused_chunks_nr;
	ਜਦੋਂ (lo < hi) {
		ਪੂਰਨ mi = lo + ((hi - lo) / 2);
		ਜੇ (where == reused_chunks[mi].original)
			ਵਾਪਸ reused_chunks[mi].difference;
		ਜੇ (where < reused_chunks[mi].original)
			hi = mi;
		ਨਹੀਂਤਾਂ
			lo = mi + 1;
	}

	/*
	 * The first chunk starts at zero, so we can't have gone below
	 * there.
	 */
	ਪ੍ਰਤਿਜ੍ਞਾ(lo);
	ਵਾਪਸ reused_chunks[lo-1].difference;
}

ਸਥਿਰ ਖਾਲੀ write_reused_pack_one(ਬਣਤਰ packed_git *reuse_packfile,
				  ਮਾਪ_t pos, ਬਣਤਰ hashfile *ਬਾਹਰਲਾ,
				  off_t pack_ਸ਼ੁਰੂ,
				  ਬਣਤਰ pack_ਖਿੜਕੀ **w_curs)
{
	off_t ਵਿੱਥ, ਅਗਲਾ, cur;
	ਗਿਣਤੀ object_type type;
	ਮਾਪ_t ਮਾਪ;

	ਵਿੱਥ = pack_pos_ਮੰਜ਼ਿਲ_ਵਿੱਥ(reuse_packfile, pos);
	ਅਗਲਾ = pack_pos_ਮੰਜ਼ਿਲ_ਵਿੱਥ(reuse_packfile, pos + 1);

	ਰਿਕਾਰਡ_reused_object(ਵਿੱਥ,
			     ਵਿੱਥ - (hashfile_ਕੁੱਲ(ਬਾਹਰਲਾ) - pack_ਸ਼ੁਰੂ));

	cur = ਵਿੱਥ;
	type = unpack_object_ਸਿਰਲੇਖ(reuse_packfile, w_curs, &cur, &ਮਾਪ);
	ਪ੍ਰਤਿਜ੍ਞਾ(type >= 0);

	ਜੇ (type == OBJ_OFS_DELTA) {
		off_t ਅਧਾਰ_ਵਿੱਥ;
		off_t fixup;

		ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ ਸਿਰਲੇਖ[MAX_PACK_OBJECT_ਸਿਰਲੇਖ];
		ਬਿਨਾਨਿਸ਼ਾਨੀ len;

		ਅਧਾਰ_ਵਿੱਥ = get_delta_ਅਧਾਰ(reuse_packfile, w_curs, &cur, type, ਵਿੱਥ);
		ਪ੍ਰਤਿਜ੍ਞਾ(ਅਧਾਰ_ਵਿੱਥ != 0);

		/* Convert to REF_DELTA if we must... */
		ਜੇ (!allow_ofs_delta) {
			uint32_t ਅਧਾਰ_pos;
			ਬਣਤਰ object_id ਅਧਾਰ_oid;

			ਜੇ (ਵਿੱਥ_ਮੰਜ਼ਿਲ_pack_pos(reuse_packfile, ਅਧਾਰ_ਵਿੱਥ, &ਅਧਾਰ_pos) < 0)
				die(_("expected object at offset %"PRIuMAX" "
				      "in pack %s"),
				    (uintmax_t)ਅਧਾਰ_ਵਿੱਥ,
				    reuse_packfile->pack_name);

			nth_packed_object_id(&ਅਧਾਰ_oid, reuse_packfile,
					     pack_pos_ਮੰਜ਼ਿਲ_ਸੂਚਕਾਂਕ(reuse_packfile, ਅਧਾਰ_pos));

			len = encode_ਵਿੱਚ_pack_object_ਸਿਰਲੇਖ(ਸਿਰਲੇਖ, ਆਕਾਰ(ਸਿਰਲੇਖ),
							   OBJ_REF_DELTA, ਮਾਪ);
			hashwrite(ਬਾਹਰਲਾ, ਸਿਰਲੇਖ, len);
			hashwrite(ਬਾਹਰਲਾ, ਅਧਾਰ_oid.hash, the_hash_algo->rawsz);
			copy_pack_ਅੰਕੜੇ(ਬਾਹਰਲਾ, reuse_packfile, w_curs, cur, ਅਗਲਾ - cur);
			ਵਾਪਸ;
		}

		/* Otherwise see if we need to rewrite the offset... */
		fixup = ਲੱਭੋ_reused_ਵਿੱਥ(ਵਿੱਥ) -
			ਲੱਭੋ_reused_ਵਿੱਥ(ਅਧਾਰ_ਵਿੱਥ);
		ਜੇ (fixup) {
			ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ ofs_ਸਿਰਲੇਖ[MAX_PACK_OBJECT_ਸਿਰਲੇਖ];
			ਬਿਨਾਨਿਸ਼ਾਨੀ i, ofs_len;
			off_t ofs = ਵਿੱਥ - ਅਧਾਰ_ਵਿੱਥ - fixup;

			len = encode_ਵਿੱਚ_pack_object_ਸਿਰਲੇਖ(ਸਿਰਲੇਖ, ਆਕਾਰ(ਸਿਰਲੇਖ),
							   OBJ_OFS_DELTA, ਮਾਪ);

			i = ਆਕਾਰ(ofs_ਸਿਰਲੇਖ) - 1;
			ofs_ਸਿਰਲੇਖ[i] = ofs & 127;
			ਜਦੋਂ (ofs >>= 7)
				ofs_ਸਿਰਲੇਖ[--i] = 128 | (--ofs & 127);

			ofs_len = ਆਕਾਰ(ofs_ਸਿਰਲੇਖ) - i;

			hashwrite(ਬਾਹਰਲਾ, ਸਿਰਲੇਖ, len);
			hashwrite(ਬਾਹਰਲਾ, ofs_ਸਿਰਲੇਖ + ਆਕਾਰ(ofs_ਸਿਰਲੇਖ) - ofs_len, ofs_len);
			copy_pack_ਅੰਕੜੇ(ਬਾਹਰਲਾ, reuse_packfile, w_curs, cur, ਅਗਲਾ - cur);
			ਵਾਪਸ;
		}

		/* ...otherwise we have no fixup, and can write it verbatim */
	}

	copy_pack_ਅੰਕੜੇ(ਬਾਹਰਲਾ, reuse_packfile, w_curs, ਵਿੱਥ, ਅਗਲਾ - ਵਿੱਥ);
}

ਸਥਿਰ ਮਾਪ_t write_reused_pack_verbatim(ਬਣਤਰ bitmapped_pack *reuse_packfile,
					 ਬਣਤਰ hashfile *ਬਾਹਰਲਾ,
					 ਬਣਤਰ pack_ਖਿੜਕੀ **w_curs)
{
	ਮਾਪ_t pos = 0;
	ਮਾਪ_t ਸਿਰਾ;

	ਜੇ (reuse_packfile->bitmap_pos) {
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
		ਵਾਪਸ reuse_packfile->bitmap_pos / ਬਿੱਟ_ਵਿੱਚ_EWORD;
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
	ਸਿਰਾ = reuse_packfile->bitmap_nr / ਬਿੱਟ_ਵਿੱਚ_EWORD;
	ਜੇ (reuse_packfile_bitmap->ਸ਼ਬਦ_alloc < ਸਿਰਾ)
		BUG("fewer words than expected in reuse_packfile_bitmap");

	ਜਦੋਂ (pos < ਸਿਰਾ && reuse_packfile_bitmap->ਸ਼ਬਦ[pos] == (eword_t)~0)
		pos++;

	ਜੇ (pos) {
		off_t ਮੰਜ਼ਿਲ_write;

		written = (pos * ਬਿੱਟ_ਵਿੱਚ_EWORD);
		ਮੰਜ਼ਿਲ_write = pack_pos_ਮੰਜ਼ਿਲ_ਵਿੱਥ(reuse_packfile->p, written)
			- ਆਕਾਰ(ਬਣਤਰ pack_ਸਿਰਲੇਖ);

		/* We're recording one chunk, not one object. */
		ਰਿਕਾਰਡ_reused_object(ਆਕਾਰ(ਬਣਤਰ pack_ਸਿਰਲੇਖ), 0);
		hashflush(ਬਾਹਰਲਾ);
		copy_pack_ਅੰਕੜੇ(ਬਾਹਰਲਾ, reuse_packfile->p, w_curs,
			ਆਕਾਰ(ਬਣਤਰ pack_ਸਿਰਲੇਖ), ਮੰਜ਼ਿਲ_write);

		display_ਤਰੱਕੀ(ਤਰੱਕੀ_state, written);
	}
	ਵਾਪਸ pos;
}

ਸਥਿਰ ਖਾਲੀ write_reused_pack(ਬਣਤਰ bitmapped_pack *reuse_packfile,
			      ਬਣਤਰ hashfile *f)
{
	ਮਾਪ_t i = reuse_packfile->bitmap_pos / ਬਿੱਟ_ਵਿੱਚ_EWORD;
	uint32_t ਵਿੱਥ;
	off_t pack_ਸ਼ੁਰੂ = hashfile_ਕੁੱਲ(f) - ਆਕਾਰ(ਬਣਤਰ pack_ਸਿਰਲੇਖ);
	ਬਣਤਰ pack_ਖਿੜਕੀ *w_curs = ਨਿਰਾ;

	ਜੇ (allow_ofs_delta)
		i = write_reused_pack_verbatim(reuse_packfile, f, &w_curs);

	ਲਈ (; i < reuse_packfile_bitmap->ਸ਼ਬਦ_alloc; ++i) {
		eword_t ਸ਼ਬਦ_2 = reuse_packfile_bitmap->ਸ਼ਬਦ[i];
		ਮਾਪ_t pos = (i * ਬਿੱਟ_ਵਿੱਚ_EWORD);

		ਲਈ (ਵਿੱਥ = 0; ਵਿੱਥ < ਬਿੱਟ_ਵਿੱਚ_EWORD; ++ਵਿੱਥ) {
			uint32_t pack_pos;
			ਜੇ ((ਸ਼ਬਦ_2 >> ਵਿੱਥ) == 0)
				ਰੋਕੋ;

			ਵਿੱਥ += ewah_ਦੋਆਧਾਰੀ_ਅੰਕ_ctz64(ਸ਼ਬਦ_2 >> ਵਿੱਥ);
			ਜੇ (pos + ਵਿੱਥ < reuse_packfile->bitmap_pos)
				ਜਾਰੀ;
			ਜੇ (pos + ਵਿੱਥ >= reuse_packfile->bitmap_pos + reuse_packfile->bitmap_nr)
				ਜਾਓ ਪੂਰਾ;

			ਜੇ (reuse_packfile->bitmap_pos) {
				/*
				 * When doing multi-pack reuse on a
				 * non-preferred pack, translate bit positions
				 * from the MIDX pseudo-pack order back to their
				 * pack-relative positions before attempting
				 * reuse.
				 */
				ਬਣਤਰ multi_pack_ਸੂਚਕਾਂਕ *m = reuse_packfile->from_midx;
				uint32_t midx_pos;
				off_t pack_ofs;

				ਜੇ (!m)
					BUG("non-zero bitmap position without MIDX");

				midx_pos = pack_pos_ਮੰਜ਼ਿਲ_midx(m, pos + ਵਿੱਥ);
				pack_ofs = nth_midxed_ਵਿੱਥ(m, midx_pos);

				ਜੇ (ਵਿੱਥ_ਮੰਜ਼ਿਲ_pack_pos(reuse_packfile->p,
						       pack_ofs, &pack_pos) < 0)
					BUG("could not find expected object at offset %"PRIuMAX" in pack %s",
					    (uintmax_t)pack_ofs,
					    pack_basename(reuse_packfile->p));
			} ਨਹੀਂਤਾਂ {
				/*
				 * Can use bit positions directly, even for MIDX
				 * bitmaps. See comment in try_partial_reuse()
				 * for why.
				 */
				pack_pos = pos + ਵਿੱਥ;
			}

			write_reused_pack_one(reuse_packfile->p, pack_pos, f,
					      pack_ਸ਼ੁਰੂ, &w_curs);
			display_ਤਰੱਕੀ(ਤਰੱਕੀ_state, ++written);
		}
	}

ਪੂਰਾ:
	unuse_pack(&w_curs);
}

ਸਥਿਰ ਖਾਲੀ write_excluded_by_configs(ਖਾਲੀ)
{
	ਬਣਤਰ oidset_iter iter;
	ਅਚਲ ਬਣਤਰ object_id *oid;

	oidset_iter_init(&excluded_by_config, &iter);
	ਜਦੋਂ ((oid = oidset_iter_ਅਗਲਾ(&iter))) {
		ਬਣਤਰ configured_exclusion *ex =
			oidmap_get(&configured_exclusions, oid);

		ਜੇ (!ex)
			BUG("configured exclusion wasn't configured");
		write_ਵਿੱਚ_ਭਰਿਆ(1, ex->pack_hash_hex, ਲੜੀ_ਲੰਬਾਈ(ex->pack_hash_hex));
		write_ਵਿੱਚ_ਭਰਿਆ(1, " ", 1);
		write_ਵਿੱਚ_ਭਰਿਆ(1, ex->uri, ਲੜੀ_ਲੰਬਾਈ(ex->uri));
		write_ਵਿੱਚ_ਭਰਿਆ(1, "\n", 1);
	}
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ no_ਵੰਡੋ_warning[] = N_(
"disabling bitmap writing, packs are split due to pack.packSizeLimit"
);

ਸਥਿਰ ਖਾਲੀ write_pack_file(ਖਾਲੀ)
{
	uint32_t i = 0, j;
	ਬਣਤਰ hashfile *f;
	off_t ਵਿੱਥ;
	uint32_t nr_ਬਚਿਆ = nr_ਨਤੀਜਾ;
	ਸਮਾਂ_t ਆਖਰੀ_mtime = 0;
	ਬਣਤਰ object_ਇੰਦਰਾਜ **write_ਕ੍ਰਮ;
	off_t ਬਾਈਟਾਂ_written = 0;

	ਜੇ (ਤਰੱਕੀ > pack_ਮੰਜ਼ਿਲ_stdout)
		ਤਰੱਕੀ_state = ਸ਼ੁਰੂ_ਤਰੱਕੀ(the_repository,
						_("Writing objects"), nr_ਨਤੀਜਾ);
	ALLOC_ARRAY(written_list, ਮੰਜ਼ਿਲ_pack.nr_objects);
	write_ਕ੍ਰਮ = compute_write_ਕ੍ਰਮ();

	ਕਰੋ {
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ hash[GIT_MAX_RAWSZ];
		ਅੱਖਰ *pack_tmp_name = ਨਿਰਾ;

		ਜੇ (pack_ਮੰਜ਼ਿਲ_stdout) {
			/*
			 * This command is most often invoked via
			 * git-upload-pack(1), which will typically chunk data
			 * into pktlines. As such, we use the maximum data
			 * length of them as buffer length.
			 *
			 * Note that we need to subtract one though to
			 * accommodate for the sideband byte.
			 */
			ਬਣਤਰ hashfd_options opts = {
				.ਤਰੱਕੀ = ਤਰੱਕੀ_state,
				.ਬਫਰ_len = LARGE_PACKET_ਅੰਕੜੇ_MAX - 1,
			};
			f = hashfd_ext(the_repository->hash_algo, 1,
				       "<stdout>", &opts);
		} ਨਹੀਂਤਾਂ {
			f = create_tmp_packfile(the_repository, &pack_tmp_name);
		}

		ਵਿੱਥ = write_pack_ਸਿਰਲੇਖ(f, nr_ਬਚਿਆ);

		ਜੇ (reuse_packfiles_nr) {
			ਪ੍ਰਤਿਜ੍ਞਾ(pack_ਮੰਜ਼ਿਲ_stdout);
			ਲਈ (j = 0; j < reuse_packfiles_nr; j++) {
				reused_chunks_nr = 0;
				write_reused_pack(&reuse_packfiles[j], f);
				ਜੇ (reused_chunks_nr)
					reuse_packfiles_ਵਰਤਿਆ_nr++;
			}
			ਵਿੱਥ = hashfile_ਕੁੱਲ(f);
		}

		nr_written = 0;
		ਲਈ (; i < ਮੰਜ਼ਿਲ_pack.nr_objects; i++) {
			ਬਣਤਰ object_ਇੰਦਰਾਜ *e = write_ਕ੍ਰਮ[i];
			ਜੇ (write_one(f, e, &ਵਿੱਥ) == WRITE_ONE_BREAK)
				ਰੋਕੋ;
			display_ਤਰੱਕੀ(ਤਰੱਕੀ_state, written);
		}

		ਬਾਈਟਾਂ_written += hashfile_ਕੁੱਲ(f) +
			the_repository->hash_algo->rawsz;
		ਜੇ (pack_ਮੰਜ਼ਿਲ_stdout) {
			/*
			 * We never fsync when writing to stdout since we may
			 * not be writing to an actual pack file. For instance,
			 * the upload-pack code passes a pipe here. Calling
			 * fsync on a pipe results in unnecessary
			 * synchronization with the reader on some platforms.
			 */
			finalize_hashfile(f, hash, FSYNC_COMPONENT_NONE,
					  CSUM_HASH_ਵਿੱਚ_ਧਾਰਾ | CSUM_CLOSE);
		} ਨਹੀਂਤਾਂ ਜੇ (nr_written == nr_ਬਚਿਆ) {
			finalize_hashfile(f, hash, FSYNC_COMPONENT_PACK,
					  CSUM_HASH_ਵਿੱਚ_ਧਾਰਾ | CSUM_FSYNC | CSUM_CLOSE);
		} ਨਹੀਂਤਾਂ {
			/*
			 * If we wrote the wrong number of entries in the
			 * header, rewrite it like in fast-import.
			 */

			ਪੂਰਨ fd = finalize_hashfile(f, hash, FSYNC_COMPONENT_PACK, 0);
			fixup_pack_ਸਿਰਲੇਖ_footer(the_hash_algo, fd, hash,
						 pack_tmp_name, nr_written,
						 hash, ਵਿੱਥ);
			close(fd);
			ਜੇ (write_bitmap_ਸੂਚਕਾਂਕ) {
				ਜੇ (write_bitmap_ਸੂਚਕਾਂਕ != WRITE_BITMAP_QUIET)
					warning(_(no_ਵੰਡੋ_warning));
				write_bitmap_ਸੂਚਕਾਂਕ = 0;
			}
		}

		ਜੇ (!pack_ਮੰਜ਼ਿਲ_stdout) {
			ਬਣਤਰ stat st;
			ਬਣਤਰ strbuf tmpname = STRBUF_INIT;
			ਬਣਤਰ bitmap_writer bitmap_writer;
			ਅੱਖਰ *idx_tmp_name = ਨਿਰਾ;

			/*
			 * Packs are runtime accessed in their mtime
			 * order since newer packs are more likely to contain
			 * younger objects.  So if we are creating multiple
			 * packs then we should modify the mtime of later ones
			 * to preserve this property.
			 */
			ਜੇ (stat(pack_tmp_name, &st) < 0) {
				warning_errno(_("failed to stat %s"), pack_tmp_name);
			} ਨਹੀਂਤਾਂ ਜੇ (!ਆਖਰੀ_mtime) {
				ਆਖਰੀ_mtime = st.st_mtime;
			} ਨਹੀਂਤਾਂ {
				ਬਣਤਰ utimbuf utb;
				utb.actime = st.st_atime;
				utb.modtime = --ਆਖਰੀ_mtime;
				ਜੇ (utime(pack_tmp_name, &utb) < 0)
					warning_errno(_("failed utime() on %s"), pack_tmp_name);
			}

			strbuf_addf(&tmpname, "%s-%s.", ਅਧਾਰ_name,
				    hash_ਮੰਜ਼ਿਲ_hex(hash));

			ਜੇ (write_bitmap_ਸੂਚਕਾਂਕ) {
				bitmap_writer_init(&bitmap_writer,
						   the_repository, &ਮੰਜ਼ਿਲ_pack,
						   ਨਿਰਾ);
				bitmap_writer_ਸੈੱਟ_checksum(&bitmap_writer, hash);
				bitmap_writer_build_type_ਸੂਚਕਾਂਕ(&bitmap_writer,
							       written_list);
			}

			ਜੇ (cruft)
				pack_idx_opts.flags |= WRITE_MTIMES;

			stage_tmp_packfiles(the_repository, &tmpname,
					    pack_tmp_name, written_list,
					    nr_written, &ਮੰਜ਼ਿਲ_pack,
					    &pack_idx_opts, hash,
					    &idx_tmp_name);

			ਜੇ (write_bitmap_ਸੂਚਕਾਂਕ) {
				ਮਾਪ_t tmpname_len = tmpname.len;

				strbuf_addstr(&tmpname, "bitmap");
				stop_ਤਰੱਕੀ(&ਤਰੱਕੀ_state);

				bitmap_writer_show_ਤਰੱਕੀ(&bitmap_writer,
							    ਤਰੱਕੀ);
				bitmap_writer_select_commits(&bitmap_writer,
							     ਸੂਚਕਾਂਕਿਤ_commits,
							     ਸੂਚਕਾਂਕਿਤ_commits_nr);
				ਜੇ (bitmap_writer_build(&bitmap_writer) < 0)
					die(_("failed to write bitmap index"));
				bitmap_writer_ਅੰਤ(&bitmap_writer,
						     written_list,
						     tmpname.buf, write_bitmap_options);
				bitmap_writer_free(&bitmap_writer);
				write_bitmap_ਸੂਚਕਾਂਕ = 0;
				strbuf_setlen(&tmpname, tmpname_len);
			}

			rename_tmp_packfile_idx(the_repository, &tmpname, &idx_tmp_name);

			ਮੁਕਤ(idx_tmp_name);
			strbuf_release(&tmpname);
			ਮੁਕਤ(pack_tmp_name);
			puts(hash_ਮੰਜ਼ਿਲ_hex(hash));
		}

		/* mark written objects as written to previous pack */
		ਲਈ (j = 0; j < nr_written; j++) {
			written_list[j]->ਵਿੱਥ = (off_t)-1;
		}
		nr_ਬਚਿਆ -= nr_written;
	} ਜਦੋਂ (nr_ਬਚਿਆ && i < ਮੰਜ਼ਿਲ_pack.nr_objects);

	ਮੁਕਤ(written_list);
	ਮੁਕਤ(write_ਕ੍ਰਮ);
	stop_ਤਰੱਕੀ(&ਤਰੱਕੀ_state);
	ਜੇ (written != nr_ਨਤੀਜਾ)
		die(_("wrote %"PRIu32" objects while expecting %"PRIu32),
		    written, nr_ਨਤੀਜਾ);
	trace2_ਅੰਕੜੇ_intmax("pack-objects", the_repository,
			   "write_pack_file/wrote", nr_ਨਤੀਜਾ);
	trace2_ਅੰਕੜੇ_intmax("pack-objects", the_repository,
			   "write_pack_file/wrote_bytes", ਬਾਈਟਾਂ_written);
}

ਸਥਿਰ ਪੂਰਨ no_try_delta(ਅਚਲ ਅੱਖਰ *ਰਸਤਾ)
{
	ਸਥਿਰ ਬਣਤਰ attr_check *check;

	ਜੇ (!check)
		check = attr_check_initl("delta", ਨਿਰਾ);
	git_check_attr(the_repository->ਸੂਚਕਾਂਕ, ਰਸਤਾ, check);
	ਜੇ (ATTR_FALSE(check->ਚੀਜ਼ਾਂ[0].ਮੁੱਲ))
		ਵਾਪਸ 1;
	ਵਾਪਸ 0;
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
ਸਥਿਰ ਪੂਰਨ have_duplicate_ਇੰਦਰਾਜ(ਅਚਲ ਬਣਤਰ object_id *oid,
				ਪੂਰਨ exclude)
{
	ਬਣਤਰ object_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ;

	ਜੇ (reuse_packfile_bitmap &&
	    bitmap_walk_ਸ਼ਾਮਲ_ਹੈ(bitmap_git, reuse_packfile_bitmap, oid))
		ਵਾਪਸ 1;

	ਇੰਦਰਾਜ = packlist_ਲੱਭੋ(&ਮੰਜ਼ਿਲ_pack, oid);
	ਜੇ (!ਇੰਦਰਾਜ)
		ਵਾਪਸ 0;

	ਜੇ (exclude) {
		ਜੇ (!ਇੰਦਰਾਜ->preferred_ਅਧਾਰ)
			nr_ਨਤੀਜਾ--;
		ਇੰਦਰਾਜ->preferred_ਅਧਾਰ = 1;
	}

	ਵਾਪਸ 1;
}

ਸਥਿਰ ਪੂਰਨ want_cruft_object_mtime(ਬਣਤਰ repository *r,
				   ਅਚਲ ਬਣਤਰ object_id *oid,
				   ਬਿਨਾਨਿਸ਼ਾਨੀ flags, uint32_t mtime)
{
	ਬਣਤਰ odb_ਸਰੋਤ_files *files = odb_ਸਰੋਤ_files_downcast(r->objects->ਸਰੋਤ);

	ਲਈ (ਬਣਤਰ odb_files_dir *dir = files->dirs; dir; dir = dir->ਅਗਲਾ) {
		ਬਣਤਰ packed_git **cache = packfile_store_get_kept_pack_cache(dir->packed, flags);

		ਲਈ (; *cache; cache++) {
			ਬਣਤਰ packed_git *p = *cache;
			off_t ofs;
			uint32_t candidate_mtime;

			ofs = ਲੱਭੋ_pack_ਇੰਦਰਾਜ_one(oid, p);
			ਜੇ (!ofs)
				ਜਾਰੀ;

			/*
			 * We have a copy of the object 'oid' in a non-cruft
			 * pack. We can avoid packing an additional copy
			 * regardless of what the existing copy's mtime is since
			 * it is outside of a cruft pack.
			 */
			ਜੇ (!p->is_cruft)
				ਵਾਪਸ 0;

			/*
			 * If we have a copy of the object 'oid' in a cruft
			 * pack, then either read the cruft pack's mtime for
			 * that object, or, if that can't be loaded, assume the
			 * pack's mtime itself.
			 */
			ਜੇ (!ਲੱਦੋ_pack_mtimes(p)) {
				uint32_t pos;
				ਜੇ (ਵਿੱਥ_ਮੰਜ਼ਿਲ_pack_pos(p, ofs, &pos) < 0)
					ਜਾਰੀ;
				candidate_mtime = nth_packed_mtime(p, pos);
			} ਨਹੀਂਤਾਂ {
				candidate_mtime = p->mtime;
			}

			/*
			 * We have a surviving copy of the object in a cruft
			 * pack whose mtime is greater than or equal to the one
			 * we are considering. We can thus avoid packing an
			 * additional copy of that object.
			 */
			ਜੇ (mtime <= candidate_mtime)
				ਵਾਪਸ 0;
		}
	}

	ਵਾਪਸ -1;
}

ਸਥਿਰ ਪੂਰਨ want_ਮਿਲਿਆ_object(ਅਚਲ ਬਣਤਰ object_id *oid, ਪੂਰਨ exclude,
			     ਬਣਤਰ packed_git *p, uint32_t mtime)
{
	ਜੇ (exclude)
		ਵਾਪਸ 1;
	ਜੇ (incremental)
		ਵਾਪਸ 0;

	ਜੇ (!is_pack_valid(p))
		ਵਾਪਸ -1;

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
	ਜੇ (local && !p->pack_local)
		ਵਾਪਸ 0;

	/*
	 * Then handle .keep first, as we have a fast(er) path there.
	 */
	ਜੇ (ignore_packed_keep_on_disk || ignore_packed_keep_ਵਿੱਚ_core ||
	    ignore_packed_keep_ਵਿੱਚ_core_open) {
		/*
		 * Set the flags for the kept-pack cache to be the ones we want
		 * to ignore.
		 *
		 * That is, if we are ignoring objects in on-disk keep packs,
		 * then we want to search through the on-disk keep and ignore
		 * the in-core ones.
		 */
		ਬਿਨਾਨਿਸ਼ਾਨੀ flags = 0;
		ਜੇ (ignore_packed_keep_on_disk)
			flags |= KEPT_PACK_ON_DISK;
		ਜੇ (ignore_packed_keep_ਵਿੱਚ_core)
			flags |= KEPT_PACK_ਵਿੱਚ_CORE;
		ਜੇ (ignore_packed_keep_ਵਿੱਚ_core_open)
			flags |= KEPT_PACK_ਵਿੱਚ_CORE_OPEN;

		/*
		 * If the object is in a pack that we want to ignore, *and* we
		 * don't have any cruft packs that are being retained, we can
		 * abort quickly.
		 */
		ਜੇ (!ignore_packed_keep_ਵਿੱਚ_core_has_cruft) {
			ਜੇ (ignore_packed_keep_on_disk && p->pack_keep)
				ਵਾਪਸ 0;
			ਜੇ (ignore_packed_keep_ਵਿੱਚ_core && p->pack_keep_ਵਿੱਚ_core)
				ਵਾਪਸ 0;
			ਜੇ (ignore_packed_keep_ਵਿੱਚ_core_open && p->pack_keep_ਵਿੱਚ_core_open)
				ਵਾਪਸ 0;
			ਜੇ (has_object_kept_pack(p->repo, oid, flags))
				ਵਾਪਸ 0;
		} ਨਹੀਂਤਾਂ {
			/*
			 * But if there is at least one cruft pack which
			 * is being kept, we only want to include the
			 * provided object if it has a strictly greater
			 * mtime than any existing cruft copy.
			 */
			ਜੇ (!want_cruft_object_mtime(p->repo, oid, flags,
						     mtime))
				ਵਾਪਸ 0;
		}
	}

	/*
	 * At this point we know definitively that either we don't care about
	 * keep-packs, or the object is not in one. Keep checking other
	 * conditions...
	 */
	ਜੇ (!local || !have_non_local_packs)
		ਵਾਪਸ 1;

	/* we don't know yet; keep looking for more packs */
	ਵਾਪਸ -1;
}

ਸਥਿਰ ਪੂਰਨ want_object_ਵਿੱਚ_pack_one(ਬਣਤਰ packed_git *p,
				   ਅਚਲ ਬਣਤਰ object_id *oid,
				   ਪੂਰਨ exclude,
				   ਬਣਤਰ packed_git **ਮਿਲਿਆ_pack,
				   off_t *ਮਿਲਿਆ_ਵਿੱਥ,
				   uint32_t ਮਿਲਿਆ_mtime)
{
	off_t ਵਿੱਥ;

	ਜੇ (p == *ਮਿਲਿਆ_pack)
		ਵਿੱਥ = *ਮਿਲਿਆ_ਵਿੱਥ;
	ਨਹੀਂਤਾਂ
		ਵਿੱਥ = ਲੱਭੋ_pack_ਇੰਦਰਾਜ_one(oid, p);

	ਜੇ (ਵਿੱਥ) {
		ਜੇ (!*ਮਿਲਿਆ_pack) {
			ਜੇ (!is_pack_valid(p))
				ਵਾਪਸ -1;
			*ਮਿਲਿਆ_ਵਿੱਥ = ਵਿੱਥ;
			*ਮਿਲਿਆ_pack = p;
		}
		ਵਾਪਸ want_ਮਿਲਿਆ_object(oid, exclude, p, ਮਿਲਿਆ_mtime);
	}
	ਵਾਪਸ -1;
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
ਸਥਿਰ ਪੂਰਨ want_object_ਵਿੱਚ_pack_mtime(ਅਚਲ ਬਣਤਰ object_id *oid,
				     ਪੂਰਨ exclude,
				     ਬਣਤਰ packed_git **ਮਿਲਿਆ_pack,
				     off_t *ਮਿਲਿਆ_ਵਿੱਥ,
				     uint32_t ਮਿਲਿਆ_mtime)
{
	ਬਣਤਰ odb_ਸਰੋਤ_files *files =
		odb_ਸਰੋਤ_files_downcast(the_repository->objects->ਸਰੋਤ);
	ਪੂਰਨ want;
	ਬਣਤਰ packfile_list_ਇੰਦਰਾਜ *e;

	ਜੇ (!exclude && local) {
		/*
		 * Note that we start iterating at `dirs->next` so that we
		 * skip the local object directory.
		 */
		ਲਈ (ਬਣਤਰ odb_files_dir *dir = files->dirs->ਅਗਲਾ; dir; dir = dir->ਅਗਲਾ)
			ਜੇ (!odb_ਸਰੋਤ_read_object_info(&dir->loose->ਅਧਾਰ, oid, ਨਿਰਾ, 0, ਨਿਰਾ))
				ਵਾਪਸ 0;
	}

	/*
	 * If we already know the pack object lives in, start checks from that
	 * pack - in the usual case when neither --local was given nor .keep files
	 * are present we will determine the answer right now.
	 */
	ਜੇ (*ਮਿਲਿਆ_pack) {
		want = want_ਮਿਲਿਆ_object(oid, exclude, *ਮਿਲਿਆ_pack,
					 ਮਿਲਿਆ_mtime);
		ਜੇ (want != -1)
			ਵਾਪਸ want;

		*ਮਿਲਿਆ_pack = ਨਿਰਾ;
		*ਮਿਲਿਆ_ਵਿੱਥ = 0;
	}

	ਲਈ (ਬਣਤਰ odb_files_dir *dir = files->dirs; dir; dir = dir->ਅਗਲਾ) {
		ਬਣਤਰ multi_pack_ਸੂਚਕਾਂਕ *m = get_multi_pack_ਸੂਚਕਾਂਕ(dir->packed);
		ਬਣਤਰ pack_ਇੰਦਰਾਜ e;

		ਜੇ (m && midx_fill_ਇੰਦਰਾਜ(m, oid, &e, ਨਿਰਾ) == MIDX_FILL_ਪ੍ਰਾਪਤੀ) {
			want = want_object_ਵਿੱਚ_pack_one(e.p, oid, exclude, ਮਿਲਿਆ_pack, ਮਿਲਿਆ_ਵਿੱਥ, ਮਿਲਿਆ_mtime);
			ਜੇ (want != -1)
				ਵਾਪਸ want;
		}
	}

	ਲਈ (ਬਣਤਰ odb_files_dir *dir = files->dirs; dir; dir = dir->ਅਗਲਾ) {
		ਲਈ (e = dir->packed->packs.ਸਿਰ; e; e = e->ਅਗਲਾ) {
			ਬਣਤਰ packed_git *p = e->pack;
			want = want_object_ਵਿੱਚ_pack_one(p, oid, exclude, ਮਿਲਿਆ_pack, ਮਿਲਿਆ_ਵਿੱਥ, ਮਿਲਿਆ_mtime);
			ਜੇ (!exclude && want > 0)
				packfile_list_prepend(&dir->packed->packs, p);
			ਜੇ (want != -1)
				ਵਾਪਸ want;
		}
	}

	ਜੇ (uri_protocols.nr) {
		ਬਣਤਰ configured_exclusion *ex =
			oidmap_get(&configured_exclusions, oid);
		ਪੂਰਨ i;
		ਅਚਲ ਅੱਖਰ *p;

		ਜੇ (ex) {
			ਲਈ (i = 0; i < uri_protocols.nr; i++) {
				ਜੇ (skip_ਅਗੇਤਰ(ex->uri,
						uri_protocols.ਚੀਜ਼ਾਂ[i].string,
						&p) &&
				    *p == ':') {
					oidset_ਪਾਓ(&excluded_by_config, oid);
					ਵਾਪਸ 0;
				}
			}
		}
	}

	ਵਾਪਸ 1;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ want_object_ਵਿੱਚ_pack(ਅਚਲ ਬਣਤਰ object_id *oid,
				      ਪੂਰਨ exclude,
				      ਬਣਤਰ packed_git **ਮਿਲਿਆ_pack,
				      off_t *ਮਿਲਿਆ_ਵਿੱਥ)
{
	ਵਾਪਸ want_object_ਵਿੱਚ_pack_mtime(oid, exclude, ਮਿਲਿਆ_pack, ਮਿਲਿਆ_ਵਿੱਥ,
					 0);
}

ਸਥਿਰ ਬਣਤਰ object_ਇੰਦਰਾਜ *create_object_ਇੰਦਰਾਜ(ਅਚਲ ਬਣਤਰ object_id *oid,
						ਗਿਣਤੀ object_type type,
						uint32_t hash,
						ਪੂਰਨ exclude,
						ਪੂਰਨ no_try_delta,
						ਬਣਤਰ packed_git *ਮਿਲਿਆ_pack,
						off_t ਮਿਲਿਆ_ਵਿੱਥ)
{
	ਬਣਤਰ object_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ;

	ਇੰਦਰਾਜ = packlist_alloc(&ਮੰਜ਼ਿਲ_pack, oid);
	ਇੰਦਰਾਜ->hash = hash;
	oe_ਸੈੱਟ_type(ਇੰਦਰਾਜ, type);
	ਜੇ (exclude)
		ਇੰਦਰਾਜ->preferred_ਅਧਾਰ = 1;
	ਨਹੀਂਤਾਂ
		nr_ਨਤੀਜਾ++;
	ਜੇ (ਮਿਲਿਆ_pack) {
		oe_ਸੈੱਟ_ਵਿੱਚ_pack(&ਮੰਜ਼ਿਲ_pack, ਇੰਦਰਾਜ, ਮਿਲਿਆ_pack);
		ਇੰਦਰਾਜ->ਵਿੱਚ_pack_ਵਿੱਥ = ਮਿਲਿਆ_ਵਿੱਥ;
	}

	ਇੰਦਰਾਜ->no_try_delta = no_try_delta;

	ਵਾਪਸ ਇੰਦਰਾਜ;
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ no_ਘੇਰਾ_warning[] = N_(
"disabling bitmap writing, as some objects are not being packed"
);

ਸਥਿਰ ਖਾਲੀ ਜੋੜੋ_object_ਇੰਦਰਾਜ(ਅਚਲ ਬਣਤਰ object_id *oid, ਗਿਣਤੀ object_type type,
			     ਅਚਲ ਅੱਖਰ *name, ਪੂਰਨ exclude)
{
	ਬਣਤਰ packed_git *ਮਿਲਿਆ_pack = ਨਿਰਾ;
	off_t ਮਿਲਿਆ_ਵਿੱਥ = 0;

	display_ਤਰੱਕੀ(ਤਰੱਕੀ_state, ++nr_ਵੇਖਿਆ);

	ਜੇ (have_duplicate_ਇੰਦਰਾਜ(oid, exclude))
		ਵਾਪਸ;

	ਜੇ (!want_object_ਵਿੱਚ_pack(oid, exclude, &ਮਿਲਿਆ_pack, &ਮਿਲਿਆ_ਵਿੱਥ)) {
		/* The pack is missing an object, so it will not have closure */
		ਜੇ (write_bitmap_ਸੂਚਕਾਂਕ) {
			ਜੇ (write_bitmap_ਸੂਚਕਾਂਕ != WRITE_BITMAP_QUIET)
				warning(_(no_ਘੇਰਾ_warning));
			write_bitmap_ਸੂਚਕਾਂਕ = 0;
		}
		ਵਾਪਸ;
	}

	create_object_ਇੰਦਰਾਜ(oid, type, pack_name_hash_fn(name),
			    exclude, name && no_try_delta(name),
			    ਮਿਲਿਆ_pack, ਮਿਲਿਆ_ਵਿੱਥ);
}

ਸਥਿਰ ਪੂਰਨ ਜੋੜੋ_object_ਇੰਦਰਾਜ_from_bitmap(ਅਚਲ ਬਣਤਰ object_id *oid,
					ਗਿਣਤੀ object_type type,
					ਪੂਰਨ flags ਅਣਵਰਤਿਆ, uint32_t name_hash,
					ਬਣਤਰ packed_git *pack, off_t ਵਿੱਥ,
					ਖਾਲੀ *payload ਅਣਵਰਤਿਆ)
{
	display_ਤਰੱਕੀ(ਤਰੱਕੀ_state, ++nr_ਵੇਖਿਆ);

	ਜੇ (have_duplicate_ਇੰਦਰਾਜ(oid, 0))
		ਵਾਪਸ 0;

	ਜੇ (!want_object_ਵਿੱਚ_pack(oid, 0, &pack, &ਵਿੱਥ))
		ਵਾਪਸ 0;

	create_object_ਇੰਦਰਾਜ(oid, type, name_hash, 0, 0, pack, ਵਿੱਥ);
	ਵਾਪਸ 0;
}

ਬਣਤਰ pbase_ਰੁੱਖ_cache {
	ਬਣਤਰ object_id oid;
	ਪੂਰਨ ref;
	ਪੂਰਨ temporary;
	ਖਾਲੀ *ਰੁੱਖ_ਅੰਕੜੇ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਰੁੱਖ_ਮਾਪ;
};

ਸਥਿਰ ਬਣਤਰ pbase_ਰੁੱਖ_cache *(pbase_ਰੁੱਖ_cache[256]);
ਸਥਿਰ ਪੂਰਨ pbase_ਰੁੱਖ_cache_ix(ਅਚਲ ਬਣਤਰ object_id *oid)
{
	ਵਾਪਸ oid->hash[0] % ARRAY_ਮਾਪ(pbase_ਰੁੱਖ_cache);
}
ਸਥਿਰ ਪੂਰਨ pbase_ਰੁੱਖ_cache_ix_incr(ਪੂਰਨ ix)
{
	ਵਾਪਸ (ix+1) % ARRAY_ਮਾਪ(pbase_ਰੁੱਖ_cache);
}

ਸਥਿਰ ਬਣਤਰ pbase_ਰੁੱਖ {
	ਬਣਤਰ pbase_ਰੁੱਖ *ਅਗਲਾ;
	/* This is a phony "cache" entry; we are not
	 * going to evict it or find it through _get()
	 * mechanism -- this is for the toplevel node that
	 * would almost always change with any commit.
	 */
	ਬਣਤਰ pbase_ਰੁੱਖ_cache pcache;
} *pbase_ਰੁੱਖ;

ਸਥਿਰ ਬਣਤਰ pbase_ਰੁੱਖ_cache *pbase_ਰੁੱਖ_get(ਅਚਲ ਬਣਤਰ object_id *oid)
{
	ਬਣਤਰ pbase_ਰੁੱਖ_cache *ent, *nent;
	ਖਾਲੀ *ਅੰਕੜੇ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਮਾਪ;
	ਮਾਪ_t ਮਾਪ_st = 0;
	ਗਿਣਤੀ object_type type;
	ਪੂਰਨ neigh;
	ਪੂਰਨ my_ix = pbase_ਰੁੱਖ_cache_ix(oid);
	ਪੂਰਨ ਉਪਲਬਧ_ix = -1;

	/* pbase-tree-cache acts as a limited hashtable.
	 * your object will be found at your index or within a few
	 * slots after that slot if it is cached.
	 */
	ਲਈ (neigh = 0; neigh < 8; neigh++) {
		ent = pbase_ਰੁੱਖ_cache[my_ix];
		ਜੇ (ent && oideq(&ent->oid, oid)) {
			ent->ref++;
			ਵਾਪਸ ent;
		}
		ਨਹੀਂਤਾਂ ਜੇ (((ਉਪਲਬਧ_ix < 0) && (!ent || !ent->ref)) ||
			 ((0 <= ਉਪਲਬਧ_ix) &&
			  (!ent && pbase_ਰੁੱਖ_cache[ਉਪਲਬਧ_ix])))
			ਉਪਲਬਧ_ix = my_ix;
		ਜੇ (!ent)
			ਰੋਕੋ;
		my_ix = pbase_ਰੁੱਖ_cache_ix_incr(my_ix);
	}

	/* Did not find one.  Either we got a bogus request or
	 * we need to read and perhaps cache.
	 */
	ਅੰਕੜੇ = odb_read_object(the_repository->objects, oid, &type, &ਮਾਪ_st);
	ਮਾਪ = cast_ਮਾਪ_t_ਮੰਜ਼ਿਲ_ulong(ਮਾਪ_st);
	ਜੇ (!ਅੰਕੜੇ)
		ਵਾਪਸ ਨਿਰਾ;
	ਜੇ (type != OBJ_ਰੁੱਖ) {
		ਮੁਕਤ(ਅੰਕੜੇ);
		ਵਾਪਸ ਨਿਰਾ;
	}

	/* We need to either cache or return a throwaway copy */

	ਜੇ (ਉਪਲਬਧ_ix < 0)
		ent = ਨਿਰਾ;
	ਨਹੀਂਤਾਂ {
		ent = pbase_ਰੁੱਖ_cache[ਉਪਲਬਧ_ix];
		my_ix = ਉਪਲਬਧ_ix;
	}

	ਜੇ (!ent) {
		nent = xmalloc(ਆਕਾਰ(*nent));
		nent->temporary = (ਉਪਲਬਧ_ix < 0);
	}
	ਨਹੀਂਤਾਂ {
		/* evict and reuse */
		ਮੁਕਤ(ent->ਰੁੱਖ_ਅੰਕੜੇ);
		nent = ent;
	}
	oidcpy(&nent->oid, oid);
	nent->ਰੁੱਖ_ਅੰਕੜੇ = ਅੰਕੜੇ;
	nent->ਰੁੱਖ_ਮਾਪ = ਮਾਪ;
	nent->ref = 1;
	ਜੇ (!nent->temporary)
		pbase_ਰੁੱਖ_cache[my_ix] = nent;
	ਵਾਪਸ nent;
}

ਸਥਿਰ ਖਾਲੀ pbase_ਰੁੱਖ_put(ਬਣਤਰ pbase_ਰੁੱਖ_cache *cache)
{
	ਜੇ (!cache->temporary) {
		cache->ref--;
		ਵਾਪਸ;
	}
	ਮੁਕਤ(cache->ਰੁੱਖ_ਅੰਕੜੇ);
	ਮੁਕਤ(cache);
}

ਸਥਿਰ ਮਾਪ_t name_cmp_len(ਅਚਲ ਅੱਖਰ *name)
{
	ਵਾਪਸ strcspn(name, "\n/");
}

ਸਥਿਰ ਖਾਲੀ ਜੋੜੋ_pbase_object(ਬਣਤਰ ਰੁੱਖ_desc *ਰੁੱਖ,
			     ਅਚਲ ਅੱਖਰ *name,
			     ਮਾਪ_t cmplen,
			     ਅਚਲ ਅੱਖਰ *fullname)
{
	ਬਣਤਰ name_ਇੰਦਰਾਜ ਇੰਦਰਾਜ;
	ਪੂਰਨ cmp;

	ਜਦੋਂ (ਰੁੱਖ_ਇੰਦਰਾਜ(ਰੁੱਖ,&ਇੰਦਰਾਜ)) {
		ਜੇ (S_ISGITLINK(ਇੰਦਰਾਜ.ਬਹੁਲਕ))
			ਜਾਰੀ;
		cmp = ਰੁੱਖ_ਇੰਦਰਾਜ_len(&ਇੰਦਰਾਜ) != cmplen ? 1 :
		      memcmp(name, ਇੰਦਰਾਜ.ਰਸਤਾ, cmplen);
		ਜੇ (cmp > 0)
			ਜਾਰੀ;
		ਜੇ (cmp < 0)
			ਵਾਪਸ;
		ਜੇ (name[cmplen] != '/') {
			ਜੋੜੋ_object_ਇੰਦਰਾਜ(&ਇੰਦਰਾਜ.oid,
					 object_type(ਇੰਦਰਾਜ.ਬਹੁਲਕ),
					 fullname, 1);
			ਵਾਪਸ;
		}
		ਜੇ (S_ISDIR(ਇੰਦਰਾਜ.ਬਹੁਲਕ)) {
			ਬਣਤਰ ਰੁੱਖ_desc sub;
			ਬਣਤਰ pbase_ਰੁੱਖ_cache *ਰੁੱਖ;
			ਅਚਲ ਅੱਖਰ *ਥੱਲੇ = name+cmplen+1;
			ਮਾਪ_t downlen = name_cmp_len(ਥੱਲੇ);

			ਰੁੱਖ = pbase_ਰੁੱਖ_get(&ਇੰਦਰਾਜ.oid);
			ਜੇ (!ਰੁੱਖ)
				ਵਾਪਸ;
			init_ਰੁੱਖ_desc(&sub, &ਰੁੱਖ->oid,
				       ਰੁੱਖ->ਰੁੱਖ_ਅੰਕੜੇ, ਰੁੱਖ->ਰੁੱਖ_ਮਾਪ);

			ਜੋੜੋ_pbase_object(&sub, ਥੱਲੇ, downlen, fullname);
			pbase_ਰੁੱਖ_put(ਰੁੱਖ);
		}
	}
}

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ *ਪੂਰਾ_pbase_paths;
ਸਥਿਰ ਪੂਰਨ ਪੂਰਾ_pbase_paths_ਅੰਕ;
ਸਥਿਰ ਪੂਰਨ ਪੂਰਾ_pbase_paths_alloc;
ਸਥਿਰ ਪੂਰਨ ਪੂਰਾ_pbase_ਰਸਤਾ_pos(ਬਿਨਾਨਿਸ਼ਾਨੀ hash)
{
	ਪੂਰਨ lo = 0;
	ਪੂਰਨ hi = ਪੂਰਾ_pbase_paths_ਅੰਕ;
	ਜਦੋਂ (lo < hi) {
		ਪੂਰਨ mi = lo + (hi - lo) / 2;
		ਜੇ (ਪੂਰਾ_pbase_paths[mi] == hash)
			ਵਾਪਸ mi;
		ਜੇ (ਪੂਰਾ_pbase_paths[mi] < hash)
			hi = mi;
		ਨਹੀਂਤਾਂ
			lo = mi + 1;
	}
	ਵਾਪਸ -lo-1;
}

ਸਥਿਰ ਪੂਰਨ check_pbase_ਰਸਤਾ(ਬਿਨਾਨਿਸ਼ਾਨੀ hash)
{
	ਪੂਰਨ pos = ਪੂਰਾ_pbase_ਰਸਤਾ_pos(hash);
	ਜੇ (0 <= pos)
		ਵਾਪਸ 1;
	pos = -pos - 1;
	ALLOC_GROW(ਪੂਰਾ_pbase_paths,
		   ਪੂਰਾ_pbase_paths_ਅੰਕ + 1,
		   ਪੂਰਾ_pbase_paths_alloc);
	ਪੂਰਾ_pbase_paths_ਅੰਕ++;
	ਜੇ (pos < ਪੂਰਾ_pbase_paths_ਅੰਕ)
		MOVE_ARRAY(ਪੂਰਾ_pbase_paths + pos + 1, ਪੂਰਾ_pbase_paths + pos,
			   ਪੂਰਾ_pbase_paths_ਅੰਕ - pos - 1);
	ਪੂਰਾ_pbase_paths[pos] = hash;
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ ਜੋੜੋ_preferred_ਅਧਾਰ_object(ਅਚਲ ਅੱਖਰ *name)
{
	ਬਣਤਰ pbase_ਰੁੱਖ *it;
	ਮਾਪ_t cmplen;
	ਬਿਨਾਨਿਸ਼ਾਨੀ hash = pack_name_hash_fn(name);

	ਜੇ (!ਅੰਕ_preferred_ਅਧਾਰ || check_pbase_ਰਸਤਾ(hash))
		ਵਾਪਸ;

	cmplen = name_cmp_len(name);
	ਲਈ (it = pbase_ਰੁੱਖ; it; it = it->ਅਗਲਾ) {
		ਜੇ (cmplen == 0) {
			ਜੋੜੋ_object_ਇੰਦਰਾਜ(&it->pcache.oid, OBJ_ਰੁੱਖ, ਨਿਰਾ, 1);
		}
		ਨਹੀਂਤਾਂ {
			ਬਣਤਰ ਰੁੱਖ_desc ਰੁੱਖ;
			init_ਰੁੱਖ_desc(&ਰੁੱਖ, &it->pcache.oid,
				       it->pcache.ਰੁੱਖ_ਅੰਕੜੇ, it->pcache.ਰੁੱਖ_ਮਾਪ);
			ਜੋੜੋ_pbase_object(&ਰੁੱਖ, name, cmplen, name);
		}
	}
}

ਸਥਿਰ ਖਾਲੀ ਜੋੜੋ_preferred_ਅਧਾਰ(ਬਣਤਰ object_id *oid)
{
	ਬਣਤਰ pbase_ਰੁੱਖ *it;
	ਖਾਲੀ *ਅੰਕੜੇ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਮਾਪ;
	ਮਾਪ_t ਮਾਪ_st = 0;
	ਬਣਤਰ object_id ਰੁੱਖ_oid;

	ਜੇ (ਖਿੜਕੀ <= ਅੰਕ_preferred_ਅਧਾਰ++)
		ਵਾਪਸ;

	ਅੰਕੜੇ = odb_read_object_peeled(the_repository->objects, oid,
				      OBJ_ਰੁੱਖ, &ਮਾਪ_st, &ਰੁੱਖ_oid);
	ਮਾਪ = cast_ਮਾਪ_t_ਮੰਜ਼ਿਲ_ulong(ਮਾਪ_st);
	ਜੇ (!ਅੰਕੜੇ)
		ਵਾਪਸ;

	ਲਈ (it = pbase_ਰੁੱਖ; it; it = it->ਅਗਲਾ) {
		ਜੇ (oideq(&it->pcache.oid, &ਰੁੱਖ_oid)) {
			ਮੁਕਤ(ਅੰਕੜੇ);
			ਵਾਪਸ;
		}
	}

	CALLOC_ARRAY(it, 1);
	it->ਅਗਲਾ = pbase_ਰੁੱਖ;
	pbase_ਰੁੱਖ = it;

	oidcpy(&it->pcache.oid, &ਰੁੱਖ_oid);
	it->pcache.ਰੁੱਖ_ਅੰਕੜੇ = ਅੰਕੜੇ;
	it->pcache.ਰੁੱਖ_ਮਾਪ = ਮਾਪ;
}

ਸਥਿਰ ਖਾਲੀ cleanup_preferred_ਅਧਾਰ(ਖਾਲੀ)
{
	ਬਣਤਰ pbase_ਰੁੱਖ *it;
	ਬਿਨਾਨਿਸ਼ਾਨੀ i;

	it = pbase_ਰੁੱਖ;
	pbase_ਰੁੱਖ = ਨਿਰਾ;
	ਜਦੋਂ (it) {
		ਬਣਤਰ pbase_ਰੁੱਖ *tmp = it;
		it = tmp->ਅਗਲਾ;
		ਮੁਕਤ(tmp->pcache.ਰੁੱਖ_ਅੰਕੜੇ);
		ਮੁਕਤ(tmp);
	}

	ਲਈ (i = 0; i < ARRAY_ਮਾਪ(pbase_ਰੁੱਖ_cache); i++) {
		ਜੇ (!pbase_ਰੁੱਖ_cache[i])
			ਜਾਰੀ;
		ਮੁਕਤ(pbase_ਰੁੱਖ_cache[i]->ਰੁੱਖ_ਅੰਕੜੇ);
		FREE_AND_NULL(pbase_ਰੁੱਖ_cache[i]);
	}

	FREE_AND_NULL(ਪੂਰਾ_pbase_paths);
	ਪੂਰਾ_pbase_paths_ਅੰਕ = ਪੂਰਾ_pbase_paths_alloc = 0;
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
ਸਥਿਰ ਪੂਰਨ can_reuse_delta(ਅਚਲ ਬਣਤਰ object_id *ਅਧਾਰ_oid,
			   ਬਣਤਰ object_ਇੰਦਰਾਜ *delta,
			   ਬਣਤਰ object_ਇੰਦਰਾਜ **ਅਧਾਰ_ਬਾਹਰਲਾ)
{
	ਬਣਤਰ object_ਇੰਦਰਾਜ *ਅਧਾਰ;

	/*
	 * First see if we're already sending the base (or it's explicitly in
	 * our "excluded" list).
	 */
	ਅਧਾਰ = packlist_ਲੱਭੋ(&ਮੰਜ਼ਿਲ_pack, ਅਧਾਰ_oid);
	ਜੇ (ਅਧਾਰ) {
		ਜੇ (!ਵਿੱਚ_same_island(&delta->idx.oid, &ਅਧਾਰ->idx.oid))
			ਵਾਪਸ 0;
		*ਅਧਾਰ_ਬਾਹਰਲਾ = ਅਧਾਰ;
		ਵਾਪਸ 1;
	}

	/*
	 * Otherwise, reachability bitmaps may tell us if the receiver has it,
	 * even if it was buried too deep in history to make it into the
	 * packing list.
	 */
	ਜੇ (thin && bitmap_has_oid_ਵਿੱਚ_uninteresting(bitmap_git, ਅਧਾਰ_oid)) {
		ਜੇ (use_delta_islands) {
			ਜੇ (!ਵਿੱਚ_same_island(&delta->idx.oid, ਅਧਾਰ_oid))
				ਵਾਪਸ 0;
		}
		*ਅਧਾਰ_ਬਾਹਰਲਾ = ਨਿਰਾ;
		ਵਾਪਸ 1;
	}

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ prefetch_ਮੰਜ਼ਿਲ_pack(uint32_t object_ਸੂਚਕਾਂਕ_ਸ਼ੁਰੂ) {
	ਬਣਤਰ oid_array ਮੰਜ਼ਿਲ_fetch = OID_ARRAY_INIT;
	uint32_t i;

	ਲਈ (i = object_ਸੂਚਕਾਂਕ_ਸ਼ੁਰੂ; i < ਮੰਜ਼ਿਲ_pack.nr_objects; i++) {
		ਬਣਤਰ object_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ = ਮੰਜ਼ਿਲ_pack.objects + i;

		ਜੇ (!odb_read_object_info_extended(the_repository->objects,
						   &ਇੰਦਰਾਜ->idx.oid,
						   ਨਿਰਾ,
						   OBJECT_INFO_FOR_PREFETCH))
			ਜਾਰੀ;
		oid_array_append(&ਮੰਜ਼ਿਲ_fetch, &ਇੰਦਰਾਜ->idx.oid);
	}
	promisor_remote_get_ਸਿੱਧਾ(the_repository,
				   ਮੰਜ਼ਿਲ_fetch.oid, ਮੰਜ਼ਿਲ_fetch.nr);
	oid_array_clear(&ਮੰਜ਼ਿਲ_fetch);
}

ਸਥਿਰ ਖਾਲੀ check_object(ਬਣਤਰ object_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ, uint32_t object_ਸੂਚਕਾਂਕ)
{
	ਮਾਪ_t canonical_ਮਾਪ;
	ਗਿਣਤੀ object_type type;
	ਬਣਤਰ object_info oi = {.typep = &type, .sizep = &canonical_ਮਾਪ};

	ਜੇ (ਵਿੱਚ_PACK(ਇੰਦਰਾਜ)) {
		ਬਣਤਰ packed_git *p = ਵਿੱਚ_PACK(ਇੰਦਰਾਜ);
		ਬਣਤਰ pack_ਖਿੜਕੀ *w_curs = ਨਿਰਾ;
		ਪੂਰਨ have_ਅਧਾਰ = 0;
		ਬਣਤਰ object_id ਅਧਾਰ_ref;
		ਬਣਤਰ object_ਇੰਦਰਾਜ *ਅਧਾਰ_ਇੰਦਰਾਜ;
		ਮਾਪ_t ਵਰਤਿਆ_2, ਵਰਤਿਆ_0, avail;
		off_t ofs;
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *buf, c;
		ਗਿਣਤੀ object_type type;
		ਮਾਪ_t ਵਿੱਚ_pack_ਮਾਪ;

		buf = use_pack(p, &w_curs, ਇੰਦਰਾਜ->ਵਿੱਚ_pack_ਵਿੱਥ, &avail);

		/*
		 * We want in_pack_type even if we do not reuse delta
		 * since non-delta representations could still be reused.
		 */
		ਵਰਤਿਆ_2 = unpack_object_ਸਿਰਲੇਖ_ਬਫਰ(buf, avail,
						   &type,
						   &ਵਿੱਚ_pack_ਮਾਪ);
		ਜੇ (ਵਰਤਿਆ_2 == 0)
			ਜਾਓ give_ਉੱਪਰ;

		ਜੇ (type < 0)
			BUG("invalid type %d", type);
		ਇੰਦਰਾਜ->ਵਿੱਚ_pack_type = type;

		/*
		 * Determine if this is a delta and if so whether we can
		 * reuse it or not.  Otherwise let's find out as cheaply as
		 * possible what the actual type and size for this object is.
		 */
		ਚੋਣ (ਇੰਦਰਾਜ->ਵਿੱਚ_pack_type) {
		ਮੂਲ:
			/* Not a delta hence we've already got all we need. */
			oe_ਸੈੱਟ_type(ਇੰਦਰਾਜ, ਇੰਦਰਾਜ->ਵਿੱਚ_pack_type);
			ਸੈੱਟ_ਮਾਪ(ਇੰਦਰਾਜ, ਵਿੱਚ_pack_ਮਾਪ);
			ਇੰਦਰਾਜ->ਵਿੱਚ_pack_ਸਿਰਲੇਖ_ਮਾਪ = ਵਰਤਿਆ_2;
			ਜੇ (oe_type(ਇੰਦਰਾਜ) < OBJ_COMMIT || oe_type(ਇੰਦਰਾਜ) > OBJ_BLOB)
				ਜਾਓ give_ਉੱਪਰ;
			unuse_pack(&w_curs);
			ਵਾਪਸ;
		ਹਾਲਤ OBJ_REF_DELTA:
			ਜੇ (reuse_delta && !ਇੰਦਰਾਜ->preferred_ਅਧਾਰ) {
				oidread(&ਅਧਾਰ_ref,
					use_pack(p, &w_curs,
						 ਇੰਦਰਾਜ->ਵਿੱਚ_pack_ਵਿੱਥ + ਵਰਤਿਆ_2,
						 ਨਿਰਾ),
					the_repository->hash_algo);
				have_ਅਧਾਰ = 1;
			}
			ਇੰਦਰਾਜ->ਵਿੱਚ_pack_ਸਿਰਲੇਖ_ਮਾਪ = ਵਰਤਿਆ_2 + the_hash_algo->rawsz;
			ਰੋਕੋ;
		ਹਾਲਤ OBJ_OFS_DELTA:
			buf = use_pack(p, &w_curs,
				       ਇੰਦਰਾਜ->ਵਿੱਚ_pack_ਵਿੱਥ + ਵਰਤਿਆ_2, ਨਿਰਾ);
			ਵਰਤਿਆ_0 = 0;
			c = buf[ਵਰਤਿਆ_0++];
			ofs = c & 127;
			ਜਦੋਂ (c & 128) {
				ofs += 1;
				ਜੇ (!ofs || MSB(ofs, 7)) {
					error(_("delta base offset overflow in pack for %s"),
					      oid_ਮੰਜ਼ਿਲ_hex(&ਇੰਦਰਾਜ->idx.oid));
					ਜਾਓ give_ਉੱਪਰ;
				}
				c = buf[ਵਰਤਿਆ_0++];
				ofs = (ofs << 7) + (c & 127);
			}
			ofs = ਇੰਦਰਾਜ->ਵਿੱਚ_pack_ਵਿੱਥ - ofs;
			ਜੇ (ofs <= 0 || ofs >= ਇੰਦਰਾਜ->ਵਿੱਚ_pack_ਵਿੱਥ) {
				error(_("delta base offset out of bound for %s"),
				      oid_ਮੰਜ਼ਿਲ_hex(&ਇੰਦਰਾਜ->idx.oid));
				ਜਾਓ give_ਉੱਪਰ;
			}
			ਜੇ (reuse_delta && !ਇੰਦਰਾਜ->preferred_ਅਧਾਰ) {
				uint32_t pos;
				ਜੇ (ਵਿੱਥ_ਮੰਜ਼ਿਲ_pack_pos(p, ofs, &pos) < 0)
					ਜਾਓ give_ਉੱਪਰ;
				ਜੇ (!nth_packed_object_id(&ਅਧਾਰ_ref, p,
							  pack_pos_ਮੰਜ਼ਿਲ_ਸੂਚਕਾਂਕ(p, pos)))
					have_ਅਧਾਰ = 1;
			}
			ਇੰਦਰਾਜ->ਵਿੱਚ_pack_ਸਿਰਲੇਖ_ਮਾਪ = ਵਰਤਿਆ_2 + ਵਰਤਿਆ_0;
			ਰੋਕੋ;
		}

		ਜੇ (have_ਅਧਾਰ &&
		    can_reuse_delta(&ਅਧਾਰ_ref, ਇੰਦਰਾਜ, &ਅਧਾਰ_ਇੰਦਰਾਜ)) {
			oe_ਸੈੱਟ_type(ਇੰਦਰਾਜ, ਇੰਦਰਾਜ->ਵਿੱਚ_pack_type);
			ਸੈੱਟ_ਮਾਪ(ਇੰਦਰਾਜ, ਵਿੱਚ_pack_ਮਾਪ); /* delta size */
			ਸੈੱਟ_DELTA_ਮਾਪ(ਇੰਦਰਾਜ, ਵਿੱਚ_pack_ਮਾਪ);

			ਜੇ (ਅਧਾਰ_ਇੰਦਰਾਜ) {
				ਸੈੱਟ_DELTA(ਇੰਦਰਾਜ, ਅਧਾਰ_ਇੰਦਰਾਜ);
				ਇੰਦਰਾਜ->delta_sibling_idx = ਅਧਾਰ_ਇੰਦਰਾਜ->delta_ਬੱਚਾ_idx;
				ਸੈੱਟ_DELTA_ਬੱਚਾ(ਅਧਾਰ_ਇੰਦਰਾਜ, ਇੰਦਰਾਜ);
			} ਨਹੀਂਤਾਂ {
				ਸੈੱਟ_DELTA_EXT(ਇੰਦਰਾਜ, &ਅਧਾਰ_ref);
			}

			unuse_pack(&w_curs);
			ਵਾਪਸ;
		}

		ਜੇ (oe_type(ਇੰਦਰਾਜ)) {
			off_t delta_pos;

			/*
			 * This must be a delta and we already know what the
			 * final object type is.  Let's extract the actual
			 * object size from the delta header.
			 */
			delta_pos = ਇੰਦਰਾਜ->ਵਿੱਚ_pack_ਵਿੱਥ + ਇੰਦਰਾਜ->ਵਿੱਚ_pack_ਸਿਰਲੇਖ_ਮਾਪ;
			canonical_ਮਾਪ = get_ਮਾਪ_from_delta(p, &w_curs,
							     delta_pos);
			ਜੇ (canonical_ਮਾਪ == 0)
				ਜਾਓ give_ਉੱਪਰ;
			ਸੈੱਟ_ਮਾਪ(ਇੰਦਰਾਜ, canonical_ਮਾਪ);
			unuse_pack(&w_curs);
			ਵਾਪਸ;
		}

		/*
		 * No choice but to fall back to the recursive delta walk
		 * with odb_read_object_info() to find about the object type
		 * at this point...
		 */
		give_ਉੱਪਰ:
		unuse_pack(&w_curs);
	}

	ਜੇ (odb_read_object_info_extended(the_repository->objects, &ਇੰਦਰਾਜ->idx.oid, &oi,
					  OBJECT_INFO_SKIP_FETCH_OBJECT | OBJECT_INFO_LOOKUP_REPLACE) < 0) {
		ਜੇ (repo_has_promisor_remote(the_repository)) {
			prefetch_ਮੰਜ਼ਿਲ_pack(object_ਸੂਚਕਾਂਕ);
			ਜੇ (odb_read_object_info_extended(the_repository->objects, &ਇੰਦਰਾਜ->idx.oid, &oi,
							  OBJECT_INFO_SKIP_FETCH_OBJECT | OBJECT_INFO_LOOKUP_REPLACE) < 0)
				type = -1;
		} ਨਹੀਂਤਾਂ {
			type = -1;
		}
	}
	oe_ਸੈੱਟ_type(ਇੰਦਰਾਜ, type);
	ਜੇ (ਇੰਦਰਾਜ->type_valid) {
		ਸੈੱਟ_ਮਾਪ(ਇੰਦਰਾਜ, canonical_ਮਾਪ);
	} ਨਹੀਂਤਾਂ {
		/*
		 * Bad object type is checked in prepare_pack().  This is
		 * to permit a missing preferred base object to be ignored
		 * as a preferred base.  Doing so can result in a larger
		 * pack file, but the transfer will still take place.
		 */
	}
}

ਸਥਿਰ ਪੂਰਨ pack_ਵਿੱਥ_ਛਾਂਟੋ(ਅਚਲ ਖਾਲੀ *_a, ਅਚਲ ਖਾਲੀ *_b)
{
	ਅਚਲ ਬਣਤਰ object_ਇੰਦਰਾਜ *a = *(ਬਣਤਰ object_ਇੰਦਰਾਜ **)_a;
	ਅਚਲ ਬਣਤਰ object_ਇੰਦਰਾਜ *b = *(ਬਣਤਰ object_ਇੰਦਰਾਜ **)_b;
	ਅਚਲ ਬਣਤਰ packed_git *a_ਵਿੱਚ_pack = ਵਿੱਚ_PACK(a);
	ਅਚਲ ਬਣਤਰ packed_git *b_ਵਿੱਚ_pack = ਵਿੱਚ_PACK(b);

	/* avoid filesystem trashing with loose objects */
	ਜੇ (!a_ਵਿੱਚ_pack && !b_ਵਿੱਚ_pack)
		ਵਾਪਸ oidcmp(&a->idx.oid, &b->idx.oid);

	ਜੇ (a_ਵਿੱਚ_pack < b_ਵਿੱਚ_pack)
		ਵਾਪਸ -1;
	ਜੇ (a_ਵਿੱਚ_pack > b_ਵਿੱਚ_pack)
		ਵਾਪਸ 1;
	ਵਾਪਸ a->ਵਿੱਚ_pack_ਵਿੱਥ < b->ਵਿੱਚ_pack_ਵਿੱਥ ? -1 :
			(a->ਵਿੱਚ_pack_ਵਿੱਥ > b->ਵਿੱਚ_pack_ਵਿੱਥ);
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
ਸਥਿਰ ਖਾਲੀ drop_reused_delta(ਬਣਤਰ object_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ *idx = &ਮੰਜ਼ਿਲ_pack.objects[ਇੰਦਰਾਜ->delta_idx - 1].delta_ਬੱਚਾ_idx;
	ਬਣਤਰ object_info oi = OBJECT_INFO_INIT;
	ਗਿਣਤੀ object_type type;
	ਮਾਪ_t ਮਾਪ;

	ਜਦੋਂ (*idx) {
		ਬਣਤਰ object_ਇੰਦਰਾਜ *oe = &ਮੰਜ਼ਿਲ_pack.objects[*idx - 1];

		ਜੇ (oe == ਇੰਦਰਾਜ)
			*idx = oe->delta_sibling_idx;
		ਨਹੀਂਤਾਂ
			idx = &oe->delta_sibling_idx;
	}
	ਸੈੱਟ_DELTA(ਇੰਦਰਾਜ, ਨਿਰਾ);
	ਇੰਦਰਾਜ->ਡੂੰਘਾਈ = 0;

	oi.sizep = &ਮਾਪ;
	oi.typep = &type;
	ਜੇ (packed_object_info(ਨਿਰਾ, ਵਿੱਚ_PACK(ਇੰਦਰਾਜ), ਇੰਦਰਾਜ->ਵਿੱਚ_pack_ਵਿੱਥ, &oi) < 0) {
		/*
		 * We failed to get the info from this pack for some reason;
		 * fall back to odb_read_object_info, which may find another copy.
		 * And if that fails, the error will be recorded in oe_type(entry)
		 * and dealt with in prepare_pack().
		 */
		oe_ਸੈੱਟ_type(ਇੰਦਰਾਜ,
			    odb_read_object_info(the_repository->objects,
						 &ਇੰਦਰਾਜ->idx.oid, &ਮਾਪ));
	} ਨਹੀਂਤਾਂ {
		oe_ਸੈੱਟ_type(ਇੰਦਰਾਜ, type);
	}
	ਸੈੱਟ_ਮਾਪ(ਇੰਦਰਾਜ, ਮਾਪ);
}

/*
 * Follow the chain of deltas from this entry onward, throwing away any links
 * that cause us to hit a cycle (as determined by the DFS state flags in
 * the entries).
 *
 * We also detect too-long reused chains that would violate our --depth
 * limit.
 */
ਸਥਿਰ ਖਾਲੀ break_delta_chains(ਬਣਤਰ object_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ)
{
	/*
	 * The actual depth of each object we will write is stored as an int,
	 * as it cannot exceed our int "depth" limit. But before we break
	 * changes based no that limit, we may potentially go as deep as the
	 * number of objects, which is elsewhere bounded to a uint32_t.
	 */
	uint32_t ਕੁੱਲ_ਡੂੰਘਾਈ;
	ਬਣਤਰ object_ਇੰਦਰਾਜ *cur, *ਅਗਲਾ;

	ਲਈ (cur = ਇੰਦਰਾਜ, ਕੁੱਲ_ਡੂੰਘਾਈ = 0;
	     cur;
	     cur = DELTA(cur), ਕੁੱਲ_ਡੂੰਘਾਈ++) {
		ਜੇ (cur->dfs_state == DFS_ਪੂਰਾ) {
			/*
			 * We've already seen this object and know it isn't
			 * part of a cycle. We do need to append its depth
			 * to our count.
			 */
			ਕੁੱਲ_ਡੂੰਘਾਈ += cur->ਡੂੰਘਾਈ;
			ਰੋਕੋ;
		}

		/*
		 * We break cycles before looping, so an ACTIVE state (or any
		 * other cruft which made its way into the state variable)
		 * is a bug.
		 */
		ਜੇ (cur->dfs_state != DFS_NONE)
			BUG("confusing delta dfs state in first pass: %d",
			    cur->dfs_state);

		/*
		 * Now we know this is the first time we've seen the object. If
		 * it's not a delta, we're done traversing, but we'll mark it
		 * done to save time on future traversals.
		 */
		ਜੇ (!DELTA(cur)) {
			cur->dfs_state = DFS_ਪੂਰਾ;
			ਰੋਕੋ;
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
		cur->dfs_state = DFS_ਸਰਗਰਮ;
		ਜੇ (DELTA(cur)->dfs_state == DFS_ਸਰਗਰਮ) {
			drop_reused_delta(cur);
			cur->dfs_state = DFS_ਪੂਰਾ;
			ਰੋਕੋ;
		}
	}

	/*
	 * And now that we've gone all the way to the bottom of the chain, we
	 * need to clear the active flags and set the depth fields as
	 * appropriate. Unlike the loop above, which can quit when it drops a
	 * delta, we need to keep going to look for more depth cuts. So we need
	 * an extra "next" pointer to keep going after we reset cur->delta.
	 */
	ਲਈ (cur = ਇੰਦਰਾਜ; cur; cur = ਅਗਲਾ) {
		ਅਗਲਾ = DELTA(cur);

		/*
		 * We should have a chain of zero or more ACTIVE states down to
		 * a final DONE. We can quit after the DONE, because either it
		 * has no bases, or we've already handled them in a previous
		 * call.
		 */
		ਜੇ (cur->dfs_state == DFS_ਪੂਰਾ)
			ਰੋਕੋ;
		ਨਹੀਂਤਾਂ ਜੇ (cur->dfs_state != DFS_ਸਰਗਰਮ)
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
		cur->ਡੂੰਘਾਈ = (ਕੁੱਲ_ਡੂੰਘਾਈ--) % (ਡੂੰਘਾਈ + 1);
		ਜੇ (!cur->ਡੂੰਘਾਈ)
			drop_reused_delta(cur);

		cur->dfs_state = DFS_ਪੂਰਾ;
	}
}

ਸਥਿਰ ਖਾਲੀ get_object_details(ਖਾਲੀ)
{
	uint32_t i;
	ਬਣਤਰ object_ਇੰਦਰਾਜ **sorted_by_ਵਿੱਥ;

	ਜੇ (ਤਰੱਕੀ)
		ਤਰੱਕੀ_state = ਸ਼ੁਰੂ_ਤਰੱਕੀ(the_repository,
						_("Counting objects"),
						ਮੰਜ਼ਿਲ_pack.nr_objects);

	CALLOC_ARRAY(sorted_by_ਵਿੱਥ, ਮੰਜ਼ਿਲ_pack.nr_objects);
	ਲਈ (i = 0; i < ਮੰਜ਼ਿਲ_pack.nr_objects; i++)
		sorted_by_ਵਿੱਥ[i] = ਮੰਜ਼ਿਲ_pack.objects + i;
	QSORT(sorted_by_ਵਿੱਥ, ਮੰਜ਼ਿਲ_pack.nr_objects, pack_ਵਿੱਥ_ਛਾਂਟੋ);

	ਲਈ (i = 0; i < ਮੰਜ਼ਿਲ_pack.nr_objects; i++) {
		ਬਣਤਰ object_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ = sorted_by_ਵਿੱਥ[i];
		check_object(ਇੰਦਰਾਜ, i);
		ਜੇ (ਇੰਦਰਾਜ->type_valid &&
		    oe_ਮਾਪ_greater_than(&ਮੰਜ਼ਿਲ_pack, ਇੰਦਰਾਜ,
					 repo_settings_get_big_file_ਹੱਦ(the_repository)))
			ਇੰਦਰਾਜ->no_try_delta = 1;
		display_ਤਰੱਕੀ(ਤਰੱਕੀ_state, i + 1);
	}
	stop_ਤਰੱਕੀ(&ਤਰੱਕੀ_state);

	/*
	 * This must happen in a second pass, since we rely on the delta
	 * information for the whole list being completed.
	 */
	ਲਈ (i = 0; i < ਮੰਜ਼ਿਲ_pack.nr_objects; i++)
		break_delta_chains(&ਮੰਜ਼ਿਲ_pack.objects[i]);

	ਮੁਕਤ(sorted_by_ਵਿੱਥ);
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
ਸਥਿਰ ਪੂਰਨ type_ਮਾਪ_ਛਾਂਟੋ(ਅਚਲ ਖਾਲੀ *_a, ਅਚਲ ਖਾਲੀ *_b)
{
	ਅਚਲ ਬਣਤਰ object_ਇੰਦਰਾਜ *a = *(ਬਣਤਰ object_ਇੰਦਰਾਜ **)_a;
	ਅਚਲ ਬਣਤਰ object_ਇੰਦਰਾਜ *b = *(ਬਣਤਰ object_ਇੰਦਰਾਜ **)_b;
	ਅਚਲ ਗਿਣਤੀ object_type a_type = oe_type(a);
	ਅਚਲ ਗਿਣਤੀ object_type b_type = oe_type(b);
	ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ a_ਮਾਪ = ਮਾਪ_3(a);
	ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ b_ਮਾਪ = ਮਾਪ_3(b);

	ਜੇ (a_type > b_type)
		ਵਾਪਸ -1;
	ਜੇ (a_type < b_type)
		ਵਾਪਸ 1;
	ਜੇ (a->hash > b->hash)
		ਵਾਪਸ -1;
	ਜੇ (a->hash < b->hash)
		ਵਾਪਸ 1;
	ਜੇ (a->preferred_ਅਧਾਰ > b->preferred_ਅਧਾਰ)
		ਵਾਪਸ -1;
	ਜੇ (a->preferred_ਅਧਾਰ < b->preferred_ਅਧਾਰ)
		ਵਾਪਸ 1;
	ਜੇ (use_delta_islands) {
		ਅਚਲ ਪੂਰਨ island_cmp = island_delta_cmp(&a->idx.oid, &b->idx.oid);
		ਜੇ (island_cmp)
			ਵਾਪਸ island_cmp;
	}
	ਜੇ (a_ਮਾਪ > b_ਮਾਪ)
		ਵਾਪਸ -1;
	ਜੇ (a_ਮਾਪ < b_ਮਾਪ)
		ਵਾਪਸ 1;
	ਵਾਪਸ a < b ? -1 : (a > b);  /* newest first */
}

ਬਣਤਰ unpacked {
	ਬਣਤਰ object_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ;
	ਖਾਲੀ *ਅੰਕੜੇ;
	ਬਣਤਰ delta_ਸੂਚਕਾਂਕ *ਸੂਚਕਾਂਕ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਡੂੰਘਾਈ;
};

ਸਥਿਰ ਪੂਰਨ delta_cacheable(ਮਾਪ_t src_ਮਾਪ, ਮਾਪ_t trg_ਮਾਪ,
			   ਮਾਪ_t delta_ਮਾਪ_2)
{
	ਜੇ (max_delta_cache_ਮਾਪ && delta_cache_ਮਾਪ + delta_ਮਾਪ_2 > max_delta_cache_ਮਾਪ)
		ਵਾਪਸ 0;

	ਜੇ (delta_ਮਾਪ_2 < cache_max_small_delta_ਮਾਪ)
		ਵਾਪਸ 1;

	/* cache delta, if objects are large enough compared to delta size */
	ਜੇ ((src_ਮਾਪ >> 20) + (trg_ਮਾਪ >> 21) > (delta_ਮਾਪ_2 >> 10))
		ਵਾਪਸ 1;

	ਵਾਪਸ 0;
}

/* Protect delta_cache_size */
ਸਥਿਰ pthread_mutex_t cache_mutex;
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cache_ਤਾਲਾ()		pthread_mutex_ਤਾਲਾ(&cache_mutex)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cache_unlock()		pthread_mutex_unlock(&cache_mutex)

/*
 * Protect object list partitioning (e.g. struct thread_param) and
 * progress_state
 */
ਸਥਿਰ pthread_mutex_t ਤਰੱਕੀ_mutex;
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਤਰੱਕੀ_ਤਾਲਾ()		pthread_mutex_ਤਾਲਾ(&ਤਰੱਕੀ_mutex)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਤਰੱਕੀ_unlock()	pthread_mutex_unlock(&ਤਰੱਕੀ_mutex)

/*
 * Access to struct object_entry is unprotected since each thread owns
 * a portion of the main object list. Just don't access object entries
 * ahead in the list because they can be stolen and would need
 * progress_mutex for protection.
 */

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ oe_ਮਾਪ_less_than(ਬਣਤਰ packing_ਅੰਕੜੇ *pack,
				    ਅਚਲ ਬਣਤਰ object_ਇੰਦਰਾਜ *lhs,
				    ਮਾਪ_t rhs)
{
	ਜੇ (lhs->ਮਾਪ_valid)
		ਵਾਪਸ lhs->ਮਾਪ_2 < rhs;
	ਜੇ (rhs < pack->oe_ਮਾਪ_ਹੱਦਬੰਦੀ) /* rhs < 2^x <= lhs ? */
		ਵਾਪਸ 0;
	ਵਾਪਸ oe_get_ਮਾਪ_slow(pack, lhs) < rhs;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ oe_ਸੈੱਟ_ਰੁੱਖ_ਡੂੰਘਾਈ(ਬਣਤਰ packing_ਅੰਕੜੇ *pack,
				     ਬਣਤਰ object_ਇੰਦਰਾਜ *e,
				     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਰੁੱਖ_ਡੂੰਘਾਈ)
{
	ਜੇ (!pack->ਰੁੱਖ_ਡੂੰਘਾਈ)
		CALLOC_ARRAY(pack->ਰੁੱਖ_ਡੂੰਘਾਈ, pack->nr_alloc);
	pack->ਰੁੱਖ_ਡੂੰਘਾਈ[e - pack->objects] = ਰੁੱਖ_ਡੂੰਘਾਈ;
}

ਸਥਿਰ ਖਾਲੀ ਰਿਕਾਰਡ_ਰੁੱਖ_ਡੂੰਘਾਈ(ਅਚਲ ਬਣਤਰ object_id *oid, ਅਚਲ ਅੱਖਰ *name)
{
	ਅਚਲ ਅੱਖਰ *p;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਡੂੰਘਾਈ;
	ਬਣਤਰ object_ਇੰਦਰਾਜ *ent;

	/* the empty string is a root tree, which is depth 0 */
	ਡੂੰਘਾਈ = *name ? 1 : 0;
	ਲਈ (p = strchr(name, '/'); p; p = strchr(p + 1, '/'))
		ਡੂੰਘਾਈ++;

	ent = packlist_ਲੱਭੋ(&ਮੰਜ਼ਿਲ_pack, oid);
	ਜੇ (ent && ਡੂੰਘਾਈ > oe_ਰੁੱਖ_ਡੂੰਘਾਈ(&ਮੰਜ਼ਿਲ_pack, ent))
		oe_ਸੈੱਟ_ਰੁੱਖ_ਡੂੰਘਾਈ(&ਮੰਜ਼ਿਲ_pack, ent, ਡੂੰਘਾਈ);
}

/*
 * Return the size of the object without doing any delta
 * reconstruction (so non-deltas are true object sizes, but deltas
 * return the size of the delta data).
 */
ਮਾਪ_t oe_get_ਮਾਪ_slow(ਬਣਤਰ packing_ਅੰਕੜੇ *pack,
			ਅਚਲ ਬਣਤਰ object_ਇੰਦਰਾਜ *e)
{
	ਬਣਤਰ packed_git *p;
	ਬਣਤਰ pack_ਖਿੜਕੀ *w_curs;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *buf;
	ਗਿਣਤੀ object_type type;
	ਮਾਪ_t ਵਰਤਿਆ_2, avail, ਮਾਪ;

	ਜੇ (e->type_ != OBJ_OFS_DELTA && e->type_ != OBJ_REF_DELTA) {
		ਮਾਪ_t sz;
		packing_ਅੰਕੜੇ_ਤਾਲਾ(&ਮੰਜ਼ਿਲ_pack);
		ਜੇ (odb_read_object_info(the_repository->objects,
					 &e->idx.oid, &sz) < 0)
			die(_("unable to get size of %s"),
			    oid_ਮੰਜ਼ਿਲ_hex(&e->idx.oid));
		packing_ਅੰਕੜੇ_unlock(&ਮੰਜ਼ਿਲ_pack);
		ਵਾਪਸ sz;
	}

	p = oe_ਵਿੱਚ_pack(pack, e);
	ਜੇ (!p)
		BUG("when e->type is a delta, it must belong to a pack");

	packing_ਅੰਕੜੇ_ਤਾਲਾ(&ਮੰਜ਼ਿਲ_pack);
	w_curs = ਨਿਰਾ;
	buf = use_pack(p, &w_curs, e->ਵਿੱਚ_pack_ਵਿੱਥ, &avail);
	ਵਰਤਿਆ_2 = unpack_object_ਸਿਰਲੇਖ_ਬਫਰ(buf, avail, &type, &ਮਾਪ);
	ਜੇ (ਵਰਤਿਆ_2 == 0)
		die(_("unable to parse object header of %s"),
		    oid_ਮੰਜ਼ਿਲ_hex(&e->idx.oid));

	unuse_pack(&w_curs);
	packing_ਅੰਕੜੇ_unlock(&ਮੰਜ਼ਿਲ_pack);
	ਵਾਪਸ ਮਾਪ;
}

ਸਥਿਰ ਪੂਰਨ try_delta(ਬਣਤਰ unpacked *trg, ਬਣਤਰ unpacked *src,
		     ਬਿਨਾਨਿਸ਼ਾਨੀ max_ਡੂੰਘਾਈ, ਮਾਪ_t *mem_usage)
{
	ਬਣਤਰ object_ਇੰਦਰਾਜ *trg_ਇੰਦਰਾਜ = trg->ਇੰਦਰਾਜ;
	ਬਣਤਰ object_ਇੰਦਰਾਜ *src_ਇੰਦਰਾਜ = src->ਇੰਦਰਾਜ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ trg_ਮਾਪ, src_ਮਾਪ, sizediff, max_ਮਾਪ, sz;
	ਮਾਪ_t delta_ਮਾਪ_2;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ref_ਡੂੰਘਾਈ;
	ਗਿਣਤੀ object_type type;
	ਖਾਲੀ *delta_buf;

	/* Don't bother doing diffs between different types */
	ਜੇ (oe_type(trg_ਇੰਦਰਾਜ) != oe_type(src_ਇੰਦਰਾਜ))
		ਵਾਪਸ -1;

	/*
	 * We do not bother to try a delta that we discarded on an
	 * earlier try, but only when reusing delta data.  Note that
	 * src_entry that is marked as the preferred_base should always
	 * be considered, as even if we produce a suboptimal delta against
	 * it, we will still save the transfer cost, as we already know
	 * the other side has it and we won't send src_entry at all.
	 */
	ਜੇ (reuse_delta && ਵਿੱਚ_PACK(trg_ਇੰਦਰਾਜ) &&
	    ਵਿੱਚ_PACK(trg_ਇੰਦਰਾਜ) == ਵਿੱਚ_PACK(src_ਇੰਦਰਾਜ) &&
	    !src_ਇੰਦਰਾਜ->preferred_ਅਧਾਰ &&
	    trg_ਇੰਦਰਾਜ->ਵਿੱਚ_pack_type != OBJ_REF_DELTA &&
	    trg_ਇੰਦਰਾਜ->ਵਿੱਚ_pack_type != OBJ_OFS_DELTA)
		ਵਾਪਸ 0;

	/* Let's not bust the allowed depth. */
	ਜੇ (src->ਡੂੰਘਾਈ >= max_ਡੂੰਘਾਈ)
		ਵਾਪਸ 0;

	/* Now some size filtering heuristics. */
	trg_ਮਾਪ = ਮਾਪ_3(trg_ਇੰਦਰਾਜ);
	ਜੇ (!DELTA(trg_ਇੰਦਰਾਜ)) {
		max_ਮਾਪ = trg_ਮਾਪ/2 - the_hash_algo->rawsz;
		ref_ਡੂੰਘਾਈ = 1;
	} ਨਹੀਂਤਾਂ {
		max_ਮਾਪ = DELTA_ਮਾਪ(trg_ਇੰਦਰਾਜ);
		ref_ਡੂੰਘਾਈ = trg->ਡੂੰਘਾਈ;
	}
	max_ਮਾਪ = (uint64_t)max_ਮਾਪ * (max_ਡੂੰਘਾਈ - src->ਡੂੰਘਾਈ) /
						(max_ਡੂੰਘਾਈ - ref_ਡੂੰਘਾਈ + 1);
	ਜੇ (max_ਮਾਪ == 0)
		ਵਾਪਸ 0;
	src_ਮਾਪ = ਮਾਪ_3(src_ਇੰਦਰਾਜ);
	sizediff = src_ਮਾਪ < trg_ਮਾਪ ? trg_ਮਾਪ - src_ਮਾਪ : 0;
	ਜੇ (sizediff >= max_ਮਾਪ)
		ਵਾਪਸ 0;
	ਜੇ (trg_ਮਾਪ < src_ਮਾਪ / 32)
		ਵਾਪਸ 0;

	ਜੇ (!ਵਿੱਚ_same_island(&trg->ਇੰਦਰਾਜ->idx.oid, &src->ਇੰਦਰਾਜ->idx.oid))
		ਵਾਪਸ 0;

	/* Load data if not already done */
	ਜੇ (!trg->ਅੰਕੜੇ) {
		ਮਾਪ_t sz_st = 0;
		packing_ਅੰਕੜੇ_ਤਾਲਾ(&ਮੰਜ਼ਿਲ_pack);
		trg->ਅੰਕੜੇ = odb_read_object(the_repository->objects,
					    &trg_ਇੰਦਰਾਜ->idx.oid, &type,
					    &sz_st);
		sz = cast_ਮਾਪ_t_ਮੰਜ਼ਿਲ_ulong(sz_st);
		packing_ਅੰਕੜੇ_unlock(&ਮੰਜ਼ਿਲ_pack);
		ਜੇ (!trg->ਅੰਕੜੇ)
			die(_("object %s cannot be read"),
			    oid_ਮੰਜ਼ਿਲ_hex(&trg_ਇੰਦਰਾਜ->idx.oid));
		ਜੇ (sz != trg_ਮਾਪ)
			die(_("object %s inconsistent object length (%"PRIuMAX" vs %"PRIuMAX")"),
			    oid_ਮੰਜ਼ਿਲ_hex(&trg_ਇੰਦਰਾਜ->idx.oid), (uintmax_t)sz,
			    (uintmax_t)trg_ਮਾਪ);
		*mem_usage += sz;
	}
	ਜੇ (!src->ਅੰਕੜੇ) {
		ਮਾਪ_t sz_st = 0;
		packing_ਅੰਕੜੇ_ਤਾਲਾ(&ਮੰਜ਼ਿਲ_pack);
		src->ਅੰਕੜੇ = odb_read_object(the_repository->objects,
					    &src_ਇੰਦਰਾਜ->idx.oid, &type,
					    &sz_st);
		sz = cast_ਮਾਪ_t_ਮੰਜ਼ਿਲ_ulong(sz_st);
		packing_ਅੰਕੜੇ_unlock(&ਮੰਜ਼ਿਲ_pack);
		ਜੇ (!src->ਅੰਕੜੇ) {
			ਜੇ (src_ਇੰਦਰਾਜ->preferred_ਅਧਾਰ) {
				ਸਥਿਰ ਪੂਰਨ warned = 0;
				ਜੇ (!warned++)
					warning(_("object %s cannot be read"),
						oid_ਮੰਜ਼ਿਲ_hex(&src_ਇੰਦਰਾਜ->idx.oid));
				/*
				 * Those objects are not included in the
				 * resulting pack.  Be resilient and ignore
				 * them if they can't be read, in case the
				 * pack could be created nevertheless.
				 */
				ਵਾਪਸ 0;
			}
			die(_("object %s cannot be read"),
			    oid_ਮੰਜ਼ਿਲ_hex(&src_ਇੰਦਰਾਜ->idx.oid));
		}
		ਜੇ (sz != src_ਮਾਪ)
			die(_("object %s inconsistent object length (%"PRIuMAX" vs %"PRIuMAX")"),
			    oid_ਮੰਜ਼ਿਲ_hex(&src_ਇੰਦਰਾਜ->idx.oid), (uintmax_t)sz,
			    (uintmax_t)src_ਮਾਪ);
		*mem_usage += sz;
	}
	ਜੇ (!src->ਸੂਚਕਾਂਕ) {
		src->ਸੂਚਕਾਂਕ = create_delta_ਸੂਚਕਾਂਕ(src->ਅੰਕੜੇ, src_ਮਾਪ);
		ਜੇ (!src->ਸੂਚਕਾਂਕ) {
			ਸਥਿਰ ਪੂਰਨ warned = 0;
			ਜੇ (!warned++)
				warning(_("suboptimal pack - out of memory"));
			ਵਾਪਸ 0;
		}
		*mem_usage += sizeof_delta_ਸੂਚਕਾਂਕ(src->ਸੂਚਕਾਂਕ);
	}

	delta_buf = create_delta(src->ਸੂਚਕਾਂਕ, trg->ਅੰਕੜੇ, trg_ਮਾਪ, &delta_ਮਾਪ_2, max_ਮਾਪ);
	ਜੇ (!delta_buf)
		ਵਾਪਸ 0;

	ਜੇ (DELTA(trg_ਇੰਦਰਾਜ)) {
		/* Prefer only shallower same-sized deltas. */
		ਜੇ (delta_ਮਾਪ_2 == DELTA_ਮਾਪ(trg_ਇੰਦਰਾਜ) &&
		    src->ਡੂੰਘਾਈ + 1 >= trg->ਡੂੰਘਾਈ) {
			ਮੁਕਤ(delta_buf);
			ਵਾਪਸ 0;
		}
	}

	/*
	 * Handle memory allocation outside of the cache
	 * accounting lock.  Compiler will optimize the strangeness
	 * away when NO_PTHREADS is defined.
	 */
	ਮੁਕਤ(trg_ਇੰਦਰਾਜ->delta_ਅੰਕੜੇ);
	cache_ਤਾਲਾ();
	ਜੇ (trg_ਇੰਦਰਾਜ->delta_ਅੰਕੜੇ) {
		delta_cache_ਮਾਪ -= DELTA_ਮਾਪ(trg_ਇੰਦਰਾਜ);
		trg_ਇੰਦਰਾਜ->delta_ਅੰਕੜੇ = ਨਿਰਾ;
	}
	ਜੇ (delta_cacheable(src_ਮਾਪ, trg_ਮਾਪ, delta_ਮਾਪ_2)) {
		delta_cache_ਮਾਪ += delta_ਮਾਪ_2;
		cache_unlock();
		trg_ਇੰਦਰਾਜ->delta_ਅੰਕੜੇ = xrealloc(delta_buf, delta_ਮਾਪ_2);
	} ਨਹੀਂਤਾਂ {
		cache_unlock();
		ਮੁਕਤ(delta_buf);
	}

	ਸੈੱਟ_DELTA(trg_ਇੰਦਰਾਜ, src_ਇੰਦਰਾਜ);
	ਸੈੱਟ_DELTA_ਮਾਪ(trg_ਇੰਦਰਾਜ, delta_ਮਾਪ_2);
	trg->ਡੂੰਘਾਈ = src->ਡੂੰਘਾਈ + 1;

	ਵਾਪਸ 1;
}

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ check_delta_ਹੱਦਬੰਦੀ(ਬਣਤਰ object_ਇੰਦਰਾਜ *me, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ n)
{
	ਬਣਤਰ object_ਇੰਦਰਾਜ *ਬੱਚਾ = DELTA_ਬੱਚਾ(me);
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ m = n;
	ਜਦੋਂ (ਬੱਚਾ) {
		ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ c = check_delta_ਹੱਦਬੰਦੀ(ਬੱਚਾ, n + 1);
		ਜੇ (m < c)
			m = c;
		ਬੱਚਾ = DELTA_SIBLING(ਬੱਚਾ);
	}
	ਵਾਪਸ m;
}

ਸਥਿਰ ਮਾਪ_t free_unpacked(ਬਣਤਰ unpacked *n)
{
	ਮਾਪ_t freed_mem = sizeof_delta_ਸੂਚਕਾਂਕ(n->ਸੂਚਕਾਂਕ);
	free_delta_ਸੂਚਕਾਂਕ(n->ਸੂਚਕਾਂਕ);
	n->ਸੂਚਕਾਂਕ = ਨਿਰਾ;
	ਜੇ (n->ਅੰਕੜੇ) {
		freed_mem += ਮਾਪ_3(n->ਇੰਦਰਾਜ);
		FREE_AND_NULL(n->ਅੰਕੜੇ);
	}
	n->ਇੰਦਰਾਜ = ਨਿਰਾ;
	n->ਡੂੰਘਾਈ = 0;
	ਵਾਪਸ freed_mem;
}

ਸਥਿਰ ਖਾਲੀ ਲੱਭੋ_deltas(ਬਣਤਰ object_ਇੰਦਰਾਜ **list, ਬਿਨਾਨਿਸ਼ਾਨੀ *list_ਮਾਪ,
			ਪੂਰਨ ਖਿੜਕੀ, ਪੂਰਨ ਡੂੰਘਾਈ, ਬਿਨਾਨਿਸ਼ਾਨੀ *processed)
{
	uint32_t i, idx = 0, ਗਿਣਤੀ_2 = 0;
	ਬਣਤਰ unpacked *array;
	ਮਾਪ_t mem_usage = 0;

	CALLOC_ARRAY(array, ਖਿੜਕੀ);

	ਲਈ (;;) {
		ਬਣਤਰ object_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ;
		ਬਣਤਰ unpacked *n = array + idx;
		ਪੂਰਨ j, max_ਡੂੰਘਾਈ, ਉੱਤਮ_ਅਧਾਰ = -1;

		ਤਰੱਕੀ_ਤਾਲਾ();
		ਜੇ (!*list_ਮਾਪ) {
			ਤਰੱਕੀ_unlock();
			ਰੋਕੋ;
		}
		ਇੰਦਰਾਜ = *list++;
		(*list_ਮਾਪ)--;
		ਜੇ (!ਇੰਦਰਾਜ->preferred_ਅਧਾਰ) {
			(*processed)++;
			display_ਤਰੱਕੀ(ਤਰੱਕੀ_state, *processed);
		}
		ਤਰੱਕੀ_unlock();

		mem_usage -= free_unpacked(n);
		n->ਇੰਦਰਾਜ = ਇੰਦਰਾਜ;

		ਜਦੋਂ (ਖਿੜਕੀ_ਸਮ੍ਰਿਤੀ_ਹੱਦਬੰਦੀ &&
		       mem_usage > ਖਿੜਕੀ_ਸਮ੍ਰਿਤੀ_ਹੱਦਬੰਦੀ &&
		       ਗਿਣਤੀ_2 > 1) {
			ਅਚਲ uint32_t ਪੂਛ = (idx + ਖਿੜਕੀ - ਗਿਣਤੀ_2) % ਖਿੜਕੀ;
			mem_usage -= free_unpacked(array + ਪੂਛ);
			ਗਿਣਤੀ_2--;
		}

		/* We do not compute delta to *create* objects we are not
		 * going to pack.
		 */
		ਜੇ (ਇੰਦਰਾਜ->preferred_ਅਧਾਰ)
			ਜਾਓ ਅਗਲਾ;

		/*
		 * If the current object is at pack edge, take the depth the
		 * objects that depend on the current object into account
		 * otherwise they would become too deep.
		 */
		max_ਡੂੰਘਾਈ = ਡੂੰਘਾਈ;
		ਜੇ (DELTA_ਬੱਚਾ(ਇੰਦਰਾਜ)) {
			max_ਡੂੰਘਾਈ -= check_delta_ਹੱਦਬੰਦੀ(ਇੰਦਰਾਜ, 0);
			ਜੇ (max_ਡੂੰਘਾਈ <= 0)
				ਜਾਓ ਅਗਲਾ;
		}

		j = ਖਿੜਕੀ;
		ਜਦੋਂ (--j > 0) {
			ਪੂਰਨ ret;
			uint32_t other_idx = idx + j;
			ਬਣਤਰ unpacked *m;
			ਜੇ (other_idx >= ਖਿੜਕੀ)
				other_idx -= ਖਿੜਕੀ;
			m = array + other_idx;
			ਜੇ (!m->ਇੰਦਰਾਜ)
				ਰੋਕੋ;
			ret = try_delta(n, m, max_ਡੂੰਘਾਈ, &mem_usage);
			ਜੇ (ret < 0)
				ਰੋਕੋ;
			ਨਹੀਂਤਾਂ ਜੇ (ret > 0)
				ਉੱਤਮ_ਅਧਾਰ = other_idx;
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
		ਜੇ (ਇੰਦਰਾਜ->delta_ਅੰਕੜੇ && !pack_ਮੰਜ਼ਿਲ_stdout) {
			ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਮਾਪ;

			ਮਾਪ = do_compress(&ਇੰਦਰਾਜ->delta_ਅੰਕੜੇ, DELTA_ਮਾਪ(ਇੰਦਰਾਜ));
			ਜੇ (ਮਾਪ < (1U << OE_Z_DELTA_ਬਿੱਟ)) {
				ਇੰਦਰਾਜ->z_delta_ਮਾਪ = ਮਾਪ;
				cache_ਤਾਲਾ();
				delta_cache_ਮਾਪ -= DELTA_ਮਾਪ(ਇੰਦਰਾਜ);
				delta_cache_ਮਾਪ += ਇੰਦਰਾਜ->z_delta_ਮਾਪ;
				cache_unlock();
			} ਨਹੀਂਤਾਂ {
				FREE_AND_NULL(ਇੰਦਰਾਜ->delta_ਅੰਕੜੇ);
				ਇੰਦਰਾਜ->z_delta_ਮਾਪ = 0;
			}
		}

		/* if we made n a delta, and if n is already at max
		 * depth, leaving it in the window is pointless.  we
		 * should evict it first.
		 */
		ਜੇ (DELTA(ਇੰਦਰਾਜ) && max_ਡੂੰਘਾਈ <= n->ਡੂੰਘਾਈ)
			ਜਾਰੀ;

		/*
		 * Move the best delta base up in the window, after the
		 * currently deltified object, to keep it longer.  It will
		 * be the first base object to be attempted next.
		 */
		ਜੇ (DELTA(ਇੰਦਰਾਜ)) {
			ਬਣਤਰ unpacked ਅਦਲਾ_ਬਦਲੀ_2 = array[ਉੱਤਮ_ਅਧਾਰ];
			ਪੂਰਨ dist = (ਖਿੜਕੀ + idx - ਉੱਤਮ_ਅਧਾਰ) % ਖਿੜਕੀ;
			ਪੂਰਨ dst = ਉੱਤਮ_ਅਧਾਰ;
			ਜਦੋਂ (dist--) {
				ਪੂਰਨ src = (dst + 1) % ਖਿੜਕੀ;
				array[dst] = array[src];
				dst = src;
			}
			array[dst] = ਅਦਲਾ_ਬਦਲੀ_2;
		}

		ਅਗਲਾ:
		idx++;
		ਜੇ (ਗਿਣਤੀ_2 + 1 < ਖਿੜਕੀ)
			ਗਿਣਤੀ_2++;
		ਜੇ (idx >= ਖਿੜਕੀ)
			idx = 0;
	}

	ਲਈ (i = 0; i < ਖਿੜਕੀ; ++i) {
		free_delta_ਸੂਚਕਾਂਕ(array[i].ਸੂਚਕਾਂਕ);
		ਮੁਕਤ(array[i].ਅੰਕੜੇ);
	}
	ਮੁਕਤ(array);
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

ਬਣਤਰ thread_params {
	pthread_t thread;
	ਬਣਤਰ object_ਇੰਦਰਾਜ **list;
	ਬਣਤਰ packing_region *regions;
	ਬਿਨਾਨਿਸ਼ਾਨੀ list_ਮਾਪ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਬਚਿਆ;
	ਪੂਰਨ ਖਿੜਕੀ;
	ਪੂਰਨ ਡੂੰਘਾਈ;
	ਪੂਰਨ working;
	ਪੂਰਨ ਅੰਕੜੇ_ready;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	ਬਿਨਾਨਿਸ਼ਾਨੀ *processed;
};

ਸਥਿਰ pthread_cond_t ਤਰੱਕੀ_cond;

/*
 * Mutex and conditional variable can't be statically-initialized on Windows.
 */
ਸਥਿਰ ਖਾਲੀ init_threaded_ਖੋਜ(ਖਾਲੀ)
{
	pthread_mutex_init(&cache_mutex, ਨਿਰਾ);
	pthread_mutex_init(&ਤਰੱਕੀ_mutex, ਨਿਰਾ);
	pthread_cond_init(&ਤਰੱਕੀ_cond, ਨਿਰਾ);
}

ਸਥਿਰ ਖਾਲੀ cleanup_threaded_ਖੋਜ(ਖਾਲੀ)
{
	pthread_cond_destroy(&ਤਰੱਕੀ_cond);
	pthread_mutex_destroy(&cache_mutex);
	pthread_mutex_destroy(&ਤਰੱਕੀ_mutex);
}

ਸਥਿਰ ਖਾਲੀ *threaded_ਲੱਭੋ_deltas(ਖਾਲੀ *arg)
{
	ਬਣਤਰ thread_params *me = arg;

	ਤਰੱਕੀ_ਤਾਲਾ();
	ਜਦੋਂ (me->ਬਚਿਆ) {
		ਤਰੱਕੀ_unlock();

		ਲੱਭੋ_deltas(me->list, &me->ਬਚਿਆ,
			    me->ਖਿੜਕੀ, me->ਡੂੰਘਾਈ, me->processed);

		ਤਰੱਕੀ_ਤਾਲਾ();
		me->working = 0;
		pthread_cond_signal(&ਤਰੱਕੀ_cond);
		ਤਰੱਕੀ_unlock();

		/*
		 * We must not set ->data_ready before we wait on the
		 * condition because the main thread may have set it to 1
		 * before we get here. In order to be sure that new
		 * work is available if we see 1 in ->data_ready, it
		 * was initialized to 0 before this thread was spawned
		 * and we reset it to 0 right away.
		 */
		pthread_mutex_ਤਾਲਾ(&me->mutex);
		ਜਦੋਂ (!me->ਅੰਕੜੇ_ready)
			pthread_cond_wait(&me->cond, &me->mutex);
		me->ਅੰਕੜੇ_ready = 0;
		pthread_mutex_unlock(&me->mutex);

		ਤਰੱਕੀ_ਤਾਲਾ();
	}
	ਤਰੱਕੀ_unlock();
	/* leave ->working 1 so that this doesn't get more work assigned */
	ਵਾਪਸ ਨਿਰਾ;
}

ਸਥਿਰ ਖਾਲੀ ll_ਲੱਭੋ_deltas(ਬਣਤਰ object_ਇੰਦਰਾਜ **list, ਬਿਨਾਨਿਸ਼ਾਨੀ list_ਮਾਪ,
			   ਪੂਰਨ ਖਿੜਕੀ, ਪੂਰਨ ਡੂੰਘਾਈ, ਬਿਨਾਨਿਸ਼ਾਨੀ *processed)
{
	ਬਣਤਰ thread_params *p;
	ਪੂਰਨ i, ret, ਸਰਗਰਮ_ਧਾਗੇ = 0;

	init_threaded_ਖੋਜ();

	ਜੇ (delta_ਖੋਜ_ਧਾਗੇ <= 1) {
		ਲੱਭੋ_deltas(list, &list_ਮਾਪ, ਖਿੜਕੀ, ਡੂੰਘਾਈ, processed);
		cleanup_threaded_ਖੋਜ();
		ਵਾਪਸ;
	}
	ਜੇ (ਤਰੱਕੀ > pack_ਮੰਜ਼ਿਲ_stdout)
		fprintf_ln(stderr, _("Delta compression using up to %d threads"),
			   delta_ਖੋਜ_ਧਾਗੇ);
	CALLOC_ARRAY(p, delta_ਖੋਜ_ਧਾਗੇ);

	/* Partition the work amongst work threads. */
	ਲਈ (i = 0; i < delta_ਖੋਜ_ਧਾਗੇ; i++) {
		ਬਿਨਾਨਿਸ਼ਾਨੀ sub_ਮਾਪ = list_ਮਾਪ / (delta_ਖੋਜ_ਧਾਗੇ - i);

		/* don't use too small segments or no deltas will be found */
		ਜੇ (sub_ਮਾਪ < 2*ਖਿੜਕੀ && i+1 < delta_ਖੋਜ_ਧਾਗੇ)
			sub_ਮਾਪ = 0;

		p[i].ਖਿੜਕੀ = ਖਿੜਕੀ;
		p[i].ਡੂੰਘਾਈ = ਡੂੰਘਾਈ;
		p[i].processed = processed;
		p[i].working = 1;
		p[i].ਅੰਕੜੇ_ready = 0;

		/* try to split chunks on "path" boundaries */
		ਜਦੋਂ (sub_ਮਾਪ && sub_ਮਾਪ < list_ਮਾਪ &&
		       list[sub_ਮਾਪ]->hash &&
		       list[sub_ਮਾਪ]->hash == list[sub_ਮਾਪ-1]->hash)
			sub_ਮਾਪ++;

		p[i].list = list;
		p[i].list_ਮਾਪ = sub_ਮਾਪ;
		p[i].ਬਚਿਆ = sub_ਮਾਪ;

		list += sub_ਮਾਪ;
		list_ਮਾਪ -= sub_ਮਾਪ;
	}

	/* Start work threads. */
	ਲਈ (i = 0; i < delta_ਖੋਜ_ਧਾਗੇ; i++) {
		ਜੇ (!p[i].list_ਮਾਪ)
			ਜਾਰੀ;
		pthread_mutex_init(&p[i].mutex, ਨਿਰਾ);
		pthread_cond_init(&p[i].cond, ਨਿਰਾ);
		ret = pthread_create(&p[i].thread, ਨਿਰਾ,
				     threaded_ਲੱਭੋ_deltas, &p[i]);
		ਜੇ (ret)
			die(_("unable to create thread: %s"), strerror(ret));
		ਸਰਗਰਮ_ਧਾਗੇ++;
	}

	/*
	 * Now let's wait for work completion.  Each time a thread is done
	 * with its work, we steal half of the remaining work from the
	 * thread with the largest number of unprocessed objects and give
	 * it to that newly idle thread.  This ensure good load balancing
	 * until the remaining object list segments are simply too short
	 * to be worth splitting anymore.
	 */
	ਜਦੋਂ (ਸਰਗਰਮ_ਧਾਗੇ) {
		ਬਣਤਰ thread_params *ਨਿਸ਼ਾਨਾ = ਨਿਰਾ;
		ਬਣਤਰ thread_params *victim = ਨਿਰਾ;
		ਬਿਨਾਨਿਸ਼ਾਨੀ sub_ਮਾਪ = 0;

		ਤਰੱਕੀ_ਤਾਲਾ();
		ਲਈ (;;) {
			ਲਈ (i = 0; !ਨਿਸ਼ਾਨਾ && i < delta_ਖੋਜ_ਧਾਗੇ; i++)
				ਜੇ (!p[i].working)
					ਨਿਸ਼ਾਨਾ = &p[i];
			ਜੇ (ਨਿਸ਼ਾਨਾ)
				ਰੋਕੋ;
			pthread_cond_wait(&ਤਰੱਕੀ_cond, &ਤਰੱਕੀ_mutex);
		}

		ਲਈ (i = 0; i < delta_ਖੋਜ_ਧਾਗੇ; i++)
			ਜੇ (p[i].ਬਚਿਆ > 2*ਖਿੜਕੀ &&
			    (!victim || victim->ਬਚਿਆ < p[i].ਬਚਿਆ))
				victim = &p[i];
		ਜੇ (victim) {
			sub_ਮਾਪ = victim->ਬਚਿਆ / 2;
			list = victim->list + victim->list_ਮਾਪ - sub_ਮਾਪ;
			ਜਦੋਂ (sub_ਮਾਪ && list[0]->hash &&
			       list[0]->hash == list[-1]->hash) {
				list++;
				sub_ਮਾਪ--;
			}
			ਜੇ (!sub_ਮਾਪ) {
				/*
				 * It is possible for some "paths" to have
				 * so many objects that no hash boundary
				 * might be found.  Let's just steal the
				 * exact half in that case.
				 */
				sub_ਮਾਪ = victim->ਬਚਿਆ / 2;
				list -= sub_ਮਾਪ;
			}
			ਨਿਸ਼ਾਨਾ->list = list;
			victim->list_ਮਾਪ -= sub_ਮਾਪ;
			victim->ਬਚਿਆ -= sub_ਮਾਪ;
		}
		ਨਿਸ਼ਾਨਾ->list_ਮਾਪ = sub_ਮਾਪ;
		ਨਿਸ਼ਾਨਾ->ਬਚਿਆ = sub_ਮਾਪ;
		ਨਿਸ਼ਾਨਾ->working = 1;
		ਤਰੱਕੀ_unlock();

		pthread_mutex_ਤਾਲਾ(&ਨਿਸ਼ਾਨਾ->mutex);
		ਨਿਸ਼ਾਨਾ->ਅੰਕੜੇ_ready = 1;
		pthread_cond_signal(&ਨਿਸ਼ਾਨਾ->cond);
		pthread_mutex_unlock(&ਨਿਸ਼ਾਨਾ->mutex);

		ਜੇ (!sub_ਮਾਪ) {
			pthread_join(ਨਿਸ਼ਾਨਾ->thread, ਨਿਰਾ);
			pthread_cond_destroy(&ਨਿਸ਼ਾਨਾ->cond);
			pthread_mutex_destroy(&ਨਿਸ਼ਾਨਾ->mutex);
			ਸਰਗਰਮ_ਧਾਗੇ--;
		}
	}
	cleanup_threaded_ਖੋਜ();
	ਮੁਕਤ(p);
}

ਸਥਿਰ ਪੂਰਨ obj_is_packed(ਅਚਲ ਬਣਤਰ object_id *oid)
{
	ਵਾਪਸ packlist_ਲੱਭੋ(&ਮੰਜ਼ਿਲ_pack, oid) ||
		(reuse_packfile_bitmap &&
		 bitmap_walk_ਸ਼ਾਮਲ_ਹੈ(bitmap_git, reuse_packfile_bitmap, oid));
}

ਸਥਿਰ ਖਾਲੀ ਜੋੜੋ_ਟੈਗ_chain(ਅਚਲ ਬਣਤਰ object_id *oid)
{
	ਬਣਤਰ ਟੈਗ *ਟੈਗ;

	/*
	 * We catch duplicates already in add_object_entry(), but we'd
	 * prefer to do this extra check to avoid having to parse the
	 * tag at all if we already know that it's being packed (e.g., if
	 * it was included via bitmaps, we would not have parsed it
	 * previously).
	 */
	ਜੇ (obj_is_packed(oid))
		ਵਾਪਸ;

	ਟੈਗ = lookup_ਟੈਗ(the_repository, oid);
	ਜਦੋਂ (1) {
		ਜੇ (!ਟੈਗ || parse_ਟੈਗ(the_repository, ਟੈਗ) || !ਟੈਗ->tagged)
			die(_("unable to pack objects reachable from tag %s"),
			    oid_ਮੰਜ਼ਿਲ_hex(oid));

		ਜੋੜੋ_object_ਇੰਦਰਾਜ(&ਟੈਗ->object.oid, OBJ_ਟੈਗ, ਨਿਰਾ, 0);

		ਜੇ (ਟੈਗ->tagged->type != OBJ_ਟੈਗ)
			ਵਾਪਸ;

		ਟੈਗ = (ਬਣਤਰ ਟੈਗ *)ਟੈਗ->tagged;
	}
}

ਸਥਿਰ ਪੂਰਨ ਜੋੜੋ_ref_ਟੈਗ(ਅਚਲ ਬਣਤਰ reference *ref, ਖਾਲੀ *cb_ਅੰਕੜੇ ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ object_id peeled;

	ਜੇ (!reference_get_peeled_oid(the_repository, ref, &peeled) &&
	    obj_is_packed(&peeled))
		ਜੋੜੋ_ਟੈਗ_chain(ref->oid);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ should_attempt_deltas(ਬਣਤਰ object_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ)
{
	ਜੇ (DELTA(ਇੰਦਰਾਜ))
		/* This happens if we decided to reuse existing
		 * delta from a pack. "reuse_delta &&" is implied.
		 */
		ਵਾਪਸ 0;

	ਜੇ (!ਇੰਦਰਾਜ->type_valid ||
	    oe_ਮਾਪ_less_than(&ਮੰਜ਼ਿਲ_pack, ਇੰਦਰਾਜ, 50))
		ਵਾਪਸ 0;

	ਜੇ (ਇੰਦਰਾਜ->no_try_delta)
		ਵਾਪਸ 0;

	ਜੇ (!ਇੰਦਰਾਜ->preferred_ਅਧਾਰ) {
		ਜੇ (oe_type(ਇੰਦਰਾਜ) < 0)
			die(_("unable to get type of object %s"),
				oid_ਮੰਜ਼ਿਲ_hex(&ਇੰਦਰਾਜ->idx.oid));
	} ਨਹੀਂਤਾਂ ਜੇ (oe_type(ਇੰਦਰਾਜ) < 0) {
		/*
		 * This object is not found, but we
		 * don't have to include it anyway.
		 */
		ਵਾਪਸ 0;
	}

	ਵਾਪਸ 1;
}

ਸਥਿਰ ਖਾਲੀ ਲੱਭੋ_deltas_for_region(ਬਣਤਰ object_ਇੰਦਰਾਜ *list,
				   ਬਣਤਰ packing_region *region,
				   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *processed)
{
	ਬਣਤਰ object_ਇੰਦਰਾਜ **delta_list;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ delta_list_nr = 0;

	ALLOC_ARRAY(delta_list, region->nr);
	ਲਈ (ਮਾਪ_t i = 0; i < region->nr; i++) {
		ਬਣਤਰ object_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ = list + region->ਸ਼ੁਰੂ + i;
		ਜੇ (should_attempt_deltas(ਇੰਦਰਾਜ))
			delta_list[delta_list_nr++] = ਇੰਦਰਾਜ;
	}

	QSORT(delta_list, delta_list_nr, type_ਮਾਪ_ਛਾਂਟੋ);
	ਲੱਭੋ_deltas(delta_list, &delta_list_nr, ਖਿੜਕੀ, ਡੂੰਘਾਈ, processed);
	ਮੁਕਤ(delta_list);
}

ਸਥਿਰ ਖਾਲੀ ਲੱਭੋ_deltas_by_region(ਬਣਤਰ object_ਇੰਦਰਾਜ *list,
				  ਬਣਤਰ packing_region *regions,
				  ਮਾਪ_t ਸ਼ੁਰੂ, ਮਾਪ_t nr)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ processed = 0;
	ਮਾਪ_t ਤਰੱਕੀ_nr;

	ਜੇ (!nr)
		ਵਾਪਸ;

	ਤਰੱਕੀ_nr = regions[nr - 1].ਸ਼ੁਰੂ + regions[nr - 1].nr;

	ਜੇ (ਤਰੱਕੀ)
		ਤਰੱਕੀ_state = ਸ਼ੁਰੂ_ਤਰੱਕੀ(the_repository,
						_("Compressing objects by path"),
						ਤਰੱਕੀ_nr);

	ਜਦੋਂ (nr--)
		ਲੱਭੋ_deltas_for_region(list,
				       &regions[ਸ਼ੁਰੂ++],
				       &processed);

	display_ਤਰੱਕੀ(ਤਰੱਕੀ_state, ਤਰੱਕੀ_nr);
	stop_ਤਰੱਕੀ(&ਤਰੱਕੀ_state);
}

ਸਥਿਰ ਖਾਲੀ *threaded_ਲੱਭੋ_deltas_by_ਰਸਤਾ(ਖਾਲੀ *arg)
{
	ਬਣਤਰ thread_params *me = arg;

	ਤਰੱਕੀ_ਤਾਲਾ();
	ਜਦੋਂ (me->ਬਚਿਆ) {
		ਜਦੋਂ (me->ਬਚਿਆ) {
			ਤਰੱਕੀ_unlock();
			ਲੱਭੋ_deltas_for_region(ਮੰਜ਼ਿਲ_pack.objects,
					       me->regions,
					       me->processed);
			ਤਰੱਕੀ_ਤਾਲਾ();
			me->ਬਚਿਆ--;
			me->regions++;
		}

		me->working = 0;
		pthread_cond_signal(&ਤਰੱਕੀ_cond);
		ਤਰੱਕੀ_unlock();

		/*
		 * We must not set ->data_ready before we wait on the
		 * condition because the main thread may have set it to 1
		 * before we get here. In order to be sure that new
		 * work is available if we see 1 in ->data_ready, it
		 * was initialized to 0 before this thread was spawned
		 * and we reset it to 0 right away.
		 */
		pthread_mutex_ਤਾਲਾ(&me->mutex);
		ਜਦੋਂ (!me->ਅੰਕੜੇ_ready)
			pthread_cond_wait(&me->cond, &me->mutex);
		me->ਅੰਕੜੇ_ready = 0;
		pthread_mutex_unlock(&me->mutex);

		ਤਰੱਕੀ_ਤਾਲਾ();
	}
	ਤਰੱਕੀ_unlock();
	/* leave ->working 1 so that this doesn't get more work assigned */
	ਵਾਪਸ ਨਿਰਾ;
}

ਸਥਿਰ ਖਾਲੀ ll_ਲੱਭੋ_deltas_by_region(ਬਣਤਰ object_ਇੰਦਰਾਜ *list,
				     ਬਣਤਰ packing_region *regions,
				     uint32_t ਸ਼ੁਰੂ, uint32_t nr)
{
	ਬਣਤਰ thread_params *p;
	ਪੂਰਨ i, ret, ਸਰਗਰਮ_ਧਾਗੇ = 0;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ processed = 0;
	uint32_t ਤਰੱਕੀ_nr;
	init_threaded_ਖੋਜ();

	ਜੇ (!nr)
		ਵਾਪਸ;

	ਤਰੱਕੀ_nr =  regions[nr - 1].ਸ਼ੁਰੂ + regions[nr - 1].nr;
	ਜੇ (delta_ਖੋਜ_ਧਾਗੇ <= 1) {
		ਲੱਭੋ_deltas_by_region(list, regions, ਸ਼ੁਰੂ, nr);
		cleanup_threaded_ਖੋਜ();
		ਵਾਪਸ;
	}

	ਜੇ (ਤਰੱਕੀ > pack_ਮੰਜ਼ਿਲ_stdout)
		fprintf_ln(stderr,
			   Q_("Path-based delta compression using up to %d thread",
			      "Path-based delta compression using up to %d threads",
			      delta_ਖੋਜ_ਧਾਗੇ),
			   delta_ਖੋਜ_ਧਾਗੇ);
	CALLOC_ARRAY(p, delta_ਖੋਜ_ਧਾਗੇ);

	ਜੇ (ਤਰੱਕੀ)
		ਤਰੱਕੀ_state = ਸ਼ੁਰੂ_ਤਰੱਕੀ(the_repository,
						_("Compressing objects by path"),
						ਤਰੱਕੀ_nr);
	/* Partition the work amongst work threads. */
	ਲਈ (i = 0; i < delta_ਖੋਜ_ਧਾਗੇ; i++) {
		ਬਿਨਾਨਿਸ਼ਾਨੀ sub_ਮਾਪ = nr / (delta_ਖੋਜ_ਧਾਗੇ - i);

		p[i].ਖਿੜਕੀ = ਖਿੜਕੀ;
		p[i].ਡੂੰਘਾਈ = ਡੂੰਘਾਈ;
		p[i].processed = &processed;
		p[i].working = 1;
		p[i].ਅੰਕੜੇ_ready = 0;

		p[i].regions = regions;
		p[i].list_ਮਾਪ = sub_ਮਾਪ;
		p[i].ਬਚਿਆ = sub_ਮਾਪ;

		regions += sub_ਮਾਪ;
		nr -= sub_ਮਾਪ;
	}

	/* Start work threads. */
	ਲਈ (i = 0; i < delta_ਖੋਜ_ਧਾਗੇ; i++) {
		ਜੇ (!p[i].list_ਮਾਪ)
			ਜਾਰੀ;
		pthread_mutex_init(&p[i].mutex, ਨਿਰਾ);
		pthread_cond_init(&p[i].cond, ਨਿਰਾ);
		ret = pthread_create(&p[i].thread, ਨਿਰਾ,
				     threaded_ਲੱਭੋ_deltas_by_ਰਸਤਾ, &p[i]);
		ਜੇ (ret)
			die(_("unable to create thread: %s"), strerror(ret));
		ਸਰਗਰਮ_ਧਾਗੇ++;
	}

	/*
	 * Now let's wait for work completion.  Each time a thread is done
	 * with its work, we steal half of the remaining work from the
	 * thread with the largest number of unprocessed objects and give
	 * it to that newly idle thread.  This ensure good load balancing
	 * until the remaining object list segments are simply too short
	 * to be worth splitting anymore.
	 */
	ਜਦੋਂ (ਸਰਗਰਮ_ਧਾਗੇ) {
		ਬਣਤਰ thread_params *ਨਿਸ਼ਾਨਾ = ਨਿਰਾ;
		ਬਣਤਰ thread_params *victim = ਨਿਰਾ;
		ਬਿਨਾਨਿਸ਼ਾਨੀ sub_ਮਾਪ = 0;

		ਤਰੱਕੀ_ਤਾਲਾ();
		ਲਈ (;;) {
			ਲਈ (i = 0; !ਨਿਸ਼ਾਨਾ && i < delta_ਖੋਜ_ਧਾਗੇ; i++)
				ਜੇ (!p[i].working)
					ਨਿਸ਼ਾਨਾ = &p[i];
			ਜੇ (ਨਿਸ਼ਾਨਾ)
				ਰੋਕੋ;
			pthread_cond_wait(&ਤਰੱਕੀ_cond, &ਤਰੱਕੀ_mutex);
		}

		ਲਈ (i = 0; i < delta_ਖੋਜ_ਧਾਗੇ; i++)
			ਜੇ (p[i].ਬਚਿਆ > 2*ਖਿੜਕੀ &&
			    (!victim || victim->ਬਚਿਆ < p[i].ਬਚਿਆ))
				victim = &p[i];
		ਜੇ (victim) {
			sub_ਮਾਪ = victim->ਬਚਿਆ / 2;
			ਨਿਸ਼ਾਨਾ->regions = victim->regions + victim->ਬਚਿਆ - sub_ਮਾਪ;
			victim->list_ਮਾਪ -= sub_ਮਾਪ;
			victim->ਬਚਿਆ -= sub_ਮਾਪ;
		}
		ਨਿਸ਼ਾਨਾ->list_ਮਾਪ = sub_ਮਾਪ;
		ਨਿਸ਼ਾਨਾ->ਬਚਿਆ = sub_ਮਾਪ;
		ਨਿਸ਼ਾਨਾ->working = 1;
		ਤਰੱਕੀ_unlock();

		pthread_mutex_ਤਾਲਾ(&ਨਿਸ਼ਾਨਾ->mutex);
		ਨਿਸ਼ਾਨਾ->ਅੰਕੜੇ_ready = 1;
		pthread_cond_signal(&ਨਿਸ਼ਾਨਾ->cond);
		pthread_mutex_unlock(&ਨਿਸ਼ਾਨਾ->mutex);

		ਜੇ (!sub_ਮਾਪ) {
			pthread_join(ਨਿਸ਼ਾਨਾ->thread, ਨਿਰਾ);
			pthread_cond_destroy(&ਨਿਸ਼ਾਨਾ->cond);
			pthread_mutex_destroy(&ਨਿਸ਼ਾਨਾ->mutex);
			ਸਰਗਰਮ_ਧਾਗੇ--;
		}
	}
	cleanup_threaded_ਖੋਜ();
	ਮੁਕਤ(p);

	display_ਤਰੱਕੀ(ਤਰੱਕੀ_state, ਤਰੱਕੀ_nr);
	stop_ਤਰੱਕੀ(&ਤਰੱਕੀ_state);
}

ਸਥਿਰ ਖਾਲੀ prepare_pack(ਪੂਰਨ ਖਿੜਕੀ, ਪੂਰਨ ਡੂੰਘਾਈ)
{
	ਬਣਤਰ object_ਇੰਦਰਾਜ **delta_list;
	uint32_t i, nr_deltas;
	ਬਿਨਾਨਿਸ਼ਾਨੀ n;

	ਜੇ (use_delta_islands)
		resolve_ਰੁੱਖ_islands(the_repository, ਤਰੱਕੀ, &ਮੰਜ਼ਿਲ_pack);

	get_object_details();

	/*
	 * If we're locally repacking then we need to be doubly careful
	 * from now on in order to make sure no stealth corruption gets
	 * propagated to the new pack.  Clients receiving streamed packs
	 * should validate everything they get anyway so no need to incur
	 * the additional cost here in that case.
	 */
	ਜੇ (!pack_ਮੰਜ਼ਿਲ_stdout)
		do_check_packed_object_ਚੱਕਰੀ_ਜਾਂਚ = 1;

	ਜੇ (!ਮੰਜ਼ਿਲ_pack.nr_objects || !ਖਿੜਕੀ || !ਡੂੰਘਾਈ)
		ਵਾਪਸ;

	ਜੇ (ਰਸਤਾ_walk)
		ll_ਲੱਭੋ_deltas_by_region(ਮੰਜ਼ਿਲ_pack.objects, ਮੰਜ਼ਿਲ_pack.regions,
					 0, ਮੰਜ਼ਿਲ_pack.nr_regions);

	ALLOC_ARRAY(delta_list, ਮੰਜ਼ਿਲ_pack.nr_objects);
	nr_deltas = n = 0;

	ਲਈ (i = 0; i < ਮੰਜ਼ਿਲ_pack.nr_objects; i++) {
		ਬਣਤਰ object_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ = ਮੰਜ਼ਿਲ_pack.objects + i;

		ਜੇ (!should_attempt_deltas(ਇੰਦਰਾਜ))
			ਜਾਰੀ;

		ਜੇ (!ਇੰਦਰਾਜ->preferred_ਅਧਾਰ)
			nr_deltas++;

		delta_list[n++] = ਇੰਦਰਾਜ;
	}

	ਜੇ (nr_deltas && n > 1) {
		ਬਿਨਾਨਿਸ਼ਾਨੀ nr_ਪੂਰਾ = 0;

		ਜੇ (ਤਰੱਕੀ)
			ਤਰੱਕੀ_state = ਸ਼ੁਰੂ_ਤਰੱਕੀ(the_repository,
							_("Compressing objects"),
							nr_deltas);
		QSORT(delta_list, n, type_ਮਾਪ_ਛਾਂਟੋ);
		ll_ਲੱਭੋ_deltas(delta_list, n, ਖਿੜਕੀ+1, ਡੂੰਘਾਈ, &nr_ਪੂਰਾ);
		stop_ਤਰੱਕੀ(&ਤਰੱਕੀ_state);
		ਜੇ (nr_ਪੂਰਾ != nr_deltas)
			die(_("inconsistency with delta count"));
	}
	ਮੁਕਤ(delta_list);
}

ਸਥਿਰ ਪੂਰਨ git_pack_config(ਅਚਲ ਅੱਖਰ *k, ਅਚਲ ਅੱਖਰ *v,
			   ਅਚਲ ਬਣਤਰ config_context *ctx, ਖਾਲੀ *cb)
{
	ਜੇ (!strcmp(k, "pack.window")) {
		ਖਿੜਕੀ = git_config_int(k, v, ctx->kvi);
		ਵਾਪਸ 0;
	}
	ਜੇ (!strcmp(k, "pack.windowmemory")) {
		ਖਿੜਕੀ_ਸਮ੍ਰਿਤੀ_ਹੱਦਬੰਦੀ = git_config_ulong(k, v, ctx->kvi);
		ਵਾਪਸ 0;
	}
	ਜੇ (!strcmp(k, "pack.depth")) {
		ਡੂੰਘਾਈ = git_config_int(k, v, ctx->kvi);
		ਵਾਪਸ 0;
	}
	ਜੇ (!strcmp(k, "pack.deltacachesize")) {
		max_delta_cache_ਮਾਪ = git_config_ਮਾਪ_t(k, v, ctx->kvi);
		ਵਾਪਸ 0;
	}
	ਜੇ (!strcmp(k, "pack.deltacachelimit")) {
		cache_max_small_delta_ਮਾਪ = git_config_int(k, v, ctx->kvi);
		ਵਾਪਸ 0;
	}
	ਜੇ (!strcmp(k, "pack.writebitmaphashcache")) {
		ਜੇ (git_config_bool(k, v))
			write_bitmap_options |= BITMAP_OPT_HASH_CACHE;
		ਨਹੀਂਤਾਂ
			write_bitmap_options &= ~BITMAP_OPT_HASH_CACHE;
	}

	ਜੇ (!strcmp(k, "pack.writebitmaplookuptable")) {
		ਜੇ (git_config_bool(k, v))
			write_bitmap_options |= BITMAP_OPT_LOOKUP_ਸਾਰਣੀ;
		ਨਹੀਂਤਾਂ
			write_bitmap_options &= ~BITMAP_OPT_LOOKUP_ਸਾਰਣੀ;
	}

	ਜੇ (!strcmp(k, "pack.usebitmaps")) {
		use_bitmap_ਸੂਚਕਾਂਕ_default = git_config_bool(k, v);
		ਵਾਪਸ 0;
	}
	ਜੇ (!strcmp(k, "pack.allowpackreuse")) {
		ਪੂਰਨ res = git_parse_maybe_bool_ਪਾਠ(v);
		ਜੇ (res < 0) {
			ਜੇ (!strcasecmp(v, "single"))
				allow_pack_reuse = SINGLE_PACK_REUSE;
			ਨਹੀਂਤਾਂ ਜੇ (!strcasecmp(v, "multi"))
				allow_pack_reuse = MULTI_PACK_REUSE;
			ਨਹੀਂਤਾਂ
				die(_("invalid pack.allowPackReuse value: '%s'"), v);
		} ਨਹੀਂਤਾਂ ਜੇ (res) {
			allow_pack_reuse = SINGLE_PACK_REUSE;
		} ਨਹੀਂਤਾਂ {
			allow_pack_reuse = NO_PACK_REUSE;
		}
		ਵਾਪਸ 0;
	}
	ਜੇ (!strcmp(k, "pack.threads")) {
		delta_ਖੋਜ_ਧਾਗੇ = git_config_int(k, v, ctx->kvi);
		ਜੇ (delta_ਖੋਜ_ਧਾਗੇ < 0)
			die(_("invalid number of threads specified (%d)"),
			    delta_ਖੋਜ_ਧਾਗੇ);
		ਜੇ (!HAVE_ਧਾਗੇ && delta_ਖੋਜ_ਧਾਗੇ != 1) {
			warning(_("no threads support, ignoring %s"), k);
			delta_ਖੋਜ_ਧਾਗੇ = 0;
		}
		ਵਾਪਸ 0;
	}
	ਜੇ (!strcmp(k, "pack.indexversion")) {
		pack_idx_opts.ਸੰਸਕਰਣ = git_config_int(k, v, ctx->kvi);
		ਜੇ (pack_idx_opts.ਸੰਸਕਰਣ > 2)
			die(_("bad pack.indexVersion=%"PRIu32),
			    pack_idx_opts.ਸੰਸਕਰਣ);
		ਵਾਪਸ 0;
	}
	ਜੇ (!strcmp(k, "pack.writereverseindex")) {
		ਜੇ (git_config_bool(k, v))
			pack_idx_opts.flags |= WRITE_REV;
		ਨਹੀਂਤਾਂ
			pack_idx_opts.flags &= ~WRITE_REV;
		ਵਾਪਸ 0;
	}
	ਜੇ (!strcmp(k, "uploadpack.blobpackfileuri")) {
		ਬਣਤਰ configured_exclusion *ex;
		ਅਚਲ ਅੱਖਰ *oid_ਸਿਰਾ, *pack_ਸਿਰਾ;
		/*
		 * Stores the pack hash. This is not a true object ID, but is
		 * of the same form.
		 */
		ਬਣਤਰ object_id pack_hash;

		ਜੇ (!v)
			ਵਾਪਸ config_error_nonbool(k);

		ex = xmalloc(ਆਕਾਰ(*ex));
		ਜੇ (parse_oid_hex(v, &ex->e.oid, &oid_ਸਿਰਾ) ||
		    *oid_ਸਿਰਾ != ' ' ||
		    parse_oid_hex(oid_ਸਿਰਾ + 1, &pack_hash, &pack_ਸਿਰਾ) ||
		    *pack_ਸਿਰਾ != ' ')
			die(_("value of uploadpack.blobpackfileuri must be "
			      "of the form '<object-hash> <pack-hash> <uri>' (got '%s')"), v);
		ਜੇ (oidmap_get(&configured_exclusions, &ex->e.oid))
			die(_("object already configured in another "
			      "uploadpack.blobpackfileuri (got '%s')"), v);
		ex->pack_hash_hex = xcalloc(1, pack_ਸਿਰਾ - oid_ਸਿਰਾ);
		ਸਮ੍ਰਿਤੀ_ਨਕਲ(ex->pack_hash_hex, oid_ਸਿਰਾ + 1, pack_ਸਿਰਾ - oid_ਸਿਰਾ - 1);
		ex->uri = xstrdup(pack_ਸਿਰਾ + 1);
		oidmap_put(&configured_exclusions, ex);
	}
	ਵਾਪਸ git_default_config(k, v, ctx, cb);
}

/* Counters for trace2 output when in --stdin-packs mode. */
ਸਥਿਰ ਪੂਰਨ stdin_packs_ਮਿਲਿਆ_nr;
ਸਥਿਰ ਪੂਰਨ stdin_packs_hints_nr;

ਸਥਿਰ ਪੂਰਨ ਜੋੜੋ_object_ਇੰਦਰਾਜ_from_pack(ਅਚਲ ਬਣਤਰ object_id *oid,
				      ਬਣਤਰ packed_git *p,
				      uint32_t pos,
				      ਖਾਲੀ *ਅੰਕੜੇ_2)
{
	off_t ofs;
	ਬਣਤਰ object_info oi = OBJECT_INFO_INIT;
	ਗਿਣਤੀ object_type type = OBJ_NONE;

	display_ਤਰੱਕੀ(ਤਰੱਕੀ_state, ++nr_ਵੇਖਿਆ);

	ਜੇ (have_duplicate_ਇੰਦਰਾਜ(oid, 0))
		ਵਾਪਸ 0;

	stdin_packs_ਮਿਲਿਆ_nr++;

	ofs = nth_packed_object_ਵਿੱਥ(p, pos);

	oi.typep = &type;
	ਜੇ (packed_object_info(ਨਿਰਾ, p, ofs, &oi) < 0) {
		die(_("could not get type of object %s in pack %s"),
		    oid_ਮੰਜ਼ਿਲ_hex(oid), p->pack_name);
	} ਨਹੀਂਤਾਂ ਜੇ (type == OBJ_COMMIT) {
		ਬਣਤਰ rev_info *revs = ਅੰਕੜੇ_2;
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
		ਜੋੜੋ_pending_oid(revs, ਨਿਰਾ, oid, 0);
	}

	ਜੇ (!want_object_ਵਿੱਚ_pack(oid, 0, &p, &ofs))
		ਵਾਪਸ 0;

	create_object_ਇੰਦਰਾਜ(oid, type, 0, 0, 0, p, ofs);

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ show_object_pack_hint(ਬਣਤਰ object *object, ਅਚਲ ਅੱਖਰ *name,
				  ਖਾਲੀ *ਅੰਕੜੇ)
{
	ਗਿਣਤੀ stdin_packs_ਬਹੁਲਕ ਬਹੁਲਕ = *(ਗਿਣਤੀ stdin_packs_ਬਹੁਲਕ *)ਅੰਕੜੇ;
	ਜੇ (ਬਹੁਲਕ == STDIN_PACKS_ਬਹੁਲਕ_FOLLOW) {
		ਜੇ (object->type == OBJ_BLOB &&
		    !odb_has_object(the_repository->objects, &object->oid, 0))
			ਵਾਪਸ;
		ਜੋੜੋ_object_ਇੰਦਰਾਜ(&object->oid, object->type, name, 0);
	} ਨਹੀਂਤਾਂ {
		ਬਣਤਰ object_ਇੰਦਰਾਜ *oe = packlist_ਲੱਭੋ(&ਮੰਜ਼ਿਲ_pack, &object->oid);
		ਜੇ (!oe)
			ਵਾਪਸ;

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

ਸਥਿਰ ਖਾਲੀ show_commit_pack_hint(ਬਣਤਰ commit *commit, ਖਾਲੀ *ਅੰਕੜੇ)
{
	ਗਿਣਤੀ stdin_packs_ਬਹੁਲਕ ਬਹੁਲਕ = *(ਗਿਣਤੀ stdin_packs_ਬਹੁਲਕ *)ਅੰਕੜੇ;

	ਜੇ (ਬਹੁਲਕ == STDIN_PACKS_ਬਹੁਲਕ_FOLLOW) {
		show_object_pack_hint((ਬਣਤਰ object *)commit, "", ਅੰਕੜੇ);
		ਵਾਪਸ;
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
ਗਿਣਤੀ stdin_pack_info_ਕਿਸਮ {
	STDIN_PACK_INCLUDE = (1<<0),
	STDIN_PACK_EXCLUDE_CLOSED = (1<<1),
	STDIN_PACK_EXCLUDE_OPEN = (1<<2),
};

ਬਣਤਰ stdin_pack_info {
	ਬਣਤਰ packed_git *p;
	ਗਿਣਤੀ stdin_pack_info_ਕਿਸਮ ਕਿਸਮ;
};

ਸਥਿਰ ਪੂਰਨ pack_mtime_cmp(ਅਚਲ ਖਾਲੀ *_a, ਅਚਲ ਖਾਲੀ *_b)
{
	ਬਣਤਰ stdin_pack_info *a = ((ਅਚਲ ਬਣਤਰ string_list_ਵਸਤੂ*)_a)->util;
	ਬਣਤਰ stdin_pack_info *b = ((ਅਚਲ ਬਣਤਰ string_list_ਵਸਤੂ*)_b)->util;

	/*
	 * order packs by descending mtime so that objects are laid out
	 * roughly as newest-to-oldest
	 */
	ਜੇ (a->p->mtime < b->p->mtime)
		ਵਾਪਸ 1;
	ਨਹੀਂਤਾਂ ਜੇ (b->p->mtime < a->p->mtime)
		ਵਾਪਸ -1;
	ਨਹੀਂਤਾਂ
		ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ stdin_packs_include_check_obj(ਬਣਤਰ object *obj, ਖਾਲੀ *ਅੰਕੜੇ ਅਣਵਰਤਿਆ)
{
	ਵਾਪਸ !has_object_kept_pack(ਮੰਜ਼ਿਲ_pack.repo, &obj->oid,
				     KEPT_PACK_ਵਿੱਚ_CORE);
}

ਸਥਿਰ ਪੂਰਨ stdin_packs_include_check(ਬਣਤਰ commit *commit, ਖਾਲੀ *ਅੰਕੜੇ)
{
	ਵਾਪਸ stdin_packs_include_check_obj((ਬਣਤਰ object *)commit, ਅੰਕੜੇ);
}

ਸਥਿਰ ਖਾਲੀ stdin_packs_ਜੋੜੋ_pack_entries(ਬਣਤਰ strmap *packs,
					 ਬਣਤਰ rev_info *revs)
{
	ਬਣਤਰ string_list ਕੁੰਜੀਆਂ = STRING_LIST_INIT_NODUP;
	ਬਣਤਰ string_list_ਵਸਤੂ *ਵਸਤੂ;
	ਬਣਤਰ hashmap_iter iter;
	ਬਣਤਰ strmap_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ;

	strmap_for_each_ਇੰਦਰਾਜ(packs, &iter, ਇੰਦਰਾਜ) {
		ਬਣਤਰ stdin_pack_info *info = ਇੰਦਰਾਜ->ਮੁੱਲ;
		ਜੇ (!info->p)
			die(_("could not find pack '%s'"), ਇੰਦਰਾਜ->ਕੁੰਜੀ);

		string_list_append(&ਕੁੰਜੀਆਂ, ਇੰਦਰਾਜ->ਕੁੰਜੀ)->util = info;
	}

	/*
	 * Order packs by ascending mtime; use QSORT directly to access the
	 * string_list_item's ->util pointer, which string_list_sort() does not
	 * provide.
	 */
	QSORT(ਕੁੰਜੀਆਂ.ਚੀਜ਼ਾਂ, ਕੁੰਜੀਆਂ.nr, pack_mtime_cmp);

	for_each_string_list_ਵਸਤੂ(ਵਸਤੂ, &ਕੁੰਜੀਆਂ) {
		ਬਣਤਰ stdin_pack_info *info = ਵਸਤੂ->util;

		ਜੇ (info->ਕਿਸਮ & STDIN_PACK_EXCLUDE_OPEN) {
			/*
			 * When open-excluded packs ("!") are present, stop
			 * the parent walk at closed-excluded ("^") packs.
			 * Objects behind a "^" boundary are guaranteed to
			 * have closure and should not be rescued.
			 */
			revs->include_check = stdin_packs_include_check;
			revs->include_check_obj = stdin_packs_include_check_obj;
		}

		ਜੇ ((info->ਕਿਸਮ & STDIN_PACK_INCLUDE) ||
		    (info->ਕਿਸਮ & STDIN_PACK_EXCLUDE_OPEN))
			for_each_object_ਵਿੱਚ_pack(info->p,
						ਜੋੜੋ_object_ਇੰਦਰਾਜ_from_pack,
						revs,
						ODB_FOR_EACH_OBJECT_PACK_ਕ੍ਰਮ);
	}

	string_list_clear(&ਕੁੰਜੀਆਂ, 0);
}

ਸਥਿਰ ਖਾਲੀ stdin_packs_read_ਨਿਵੇਸ਼(ਬਣਤਰ rev_info *revs,
				   ਗਿਣਤੀ stdin_packs_ਬਹੁਲਕ ਬਹੁਲਕ)
{
	ਬਣਤਰ strbuf buf = STRBUF_INIT;
	ਬਣਤਰ strmap packs = STRMAP_INIT;
	ਬਣਤਰ packed_git *p;

	ਜਦੋਂ (strbuf_getline(&buf, stdin) != EOF) {
		ਬਣਤਰ stdin_pack_info *info;
		ਗਿਣਤੀ stdin_pack_info_ਕਿਸਮ ਕਿਸਮ = STDIN_PACK_INCLUDE;
		ਅਚਲ ਅੱਖਰ *ਕੁੰਜੀ = buf.buf;

		ਜੇ (!*ਕੁੰਜੀ)
			ਜਾਰੀ;
		ਨਹੀਂਤਾਂ ਜੇ (*ਕੁੰਜੀ == '^')
			ਕਿਸਮ = STDIN_PACK_EXCLUDE_CLOSED;
		ਨਹੀਂਤਾਂ ਜੇ (*ਕੁੰਜੀ == '!' && ਬਹੁਲਕ == STDIN_PACKS_ਬਹੁਲਕ_FOLLOW)
			ਕਿਸਮ = STDIN_PACK_EXCLUDE_OPEN;

		ਜੇ (ਕਿਸਮ != STDIN_PACK_INCLUDE)
			ਕੁੰਜੀ++;

		info = strmap_get(&packs, ਕੁੰਜੀ);
		ਜੇ (!info) {
			CALLOC_ARRAY(info, 1);
			strmap_put(&packs, ਕੁੰਜੀ, info);
		}

		info->ਕਿਸਮ |= ਕਿਸਮ;

		strbuf_reset(&buf);
	}

	repo_for_each_pack(the_repository, p) {
		ਬਣਤਰ stdin_pack_info *info;

		info = strmap_get(&packs, pack_basename(p));
		ਜੇ (!info)
			ਜਾਰੀ;

		ਜੇ (info->ਕਿਸਮ & STDIN_PACK_INCLUDE) {
			ਜੇ (exclude_promisor_objects && p->pack_promisor)
				die(_("packfile %s is a promisor but --exclude-promisor-objects was given"), p->pack_name);

			/*
			 * Arguments we got on stdin may not even be
			 * packs. First check that to avoid segfaulting
			 * later on in e.g.  pack_mtime_cmp(), excluded
			 * packs are handled below.
			 */
			ਜੇ (!is_pack_valid(p))
				die(_("packfile %s cannot be accessed"), p->pack_name);
		}

		ਜੇ (info->ਕਿਸਮ & STDIN_PACK_EXCLUDE_CLOSED) {
			/*
			 * Marking excluded packs as kept in-core so
			 * that later calls to add_object_entry()
			 * discards any objects that are also found in
			 * excluded packs.
			 */
			p->pack_keep_ਵਿੱਚ_core = 1;
		}

		ਜੇ (info->ਕਿਸਮ & STDIN_PACK_EXCLUDE_OPEN) {
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
			p->pack_keep_ਵਿੱਚ_core_open = 1;
		}

		info->p = p;
	}

	stdin_packs_ਜੋੜੋ_pack_entries(&packs, revs);

	strbuf_release(&buf);
	strmap_clear(&packs, 1);
}

ਸਥਿਰ ਖਾਲੀ ਜੋੜੋ_unreachable_loose_objects(ਬਣਤਰ rev_info *revs);

ਸਥਿਰ ਖਾਲੀ read_stdin_packs(ਬਣਤਰ repository *repo,
			     ਗਿਣਤੀ stdin_packs_ਬਹੁਲਕ ਬਹੁਲਕ, ਪੂਰਨ rev_list_unpacked)
{
	ਪੂਰਨ prev_fetch_if_missing = repo->fetch_if_missing;
	ਬਣਤਰ rev_info revs;

	/*
	 * The revision walk may hit objects that are promised, only. As the
	 * walk is best-effort though we don't want to perform backfill fetches
	 * for them.
	 */
	repo->fetch_if_missing = 0;

	repo_init_revisions(repo, &revs, ਨਿਰਾ);
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
	revs.keep_pack_cache_flags |= KEPT_PACK_ਵਿੱਚ_CORE;
	revs.blob_objects = 1;
	revs.ਰੁੱਖ_objects = 1;
	revs.ਟੈਗ_objects = 1;
	revs.ignore_missing_links = 1;
	revs.exclude_promisor_objects = exclude_promisor_objects;

	/* avoids adding objects in excluded packs */
	ignore_packed_keep_ਵਿੱਚ_core = 1;
	ਜੇ (ਬਹੁਲਕ == STDIN_PACKS_ਬਹੁਲਕ_FOLLOW) {
		/*
		 * In '--stdin-packs=follow' mode, additionally ignore
		 * objects in excluded-open packs to prevent them from
		 * appearing in the resulting pack.
		 */
		ignore_packed_keep_ਵਿੱਚ_core_open = 1;
	}
	stdin_packs_read_ਨਿਵੇਸ਼(&revs, ਬਹੁਲਕ);
	ਜੇ (rev_list_unpacked)
		ਜੋੜੋ_unreachable_loose_objects(&revs);

	ਜੇ (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	traverse_commit_list(&revs,
			     show_commit_pack_hint,
			     show_object_pack_hint,
			     &ਬਹੁਲਕ);

	release_revisions(&revs);

	trace2_ਅੰਕੜੇ_intmax("pack-objects", the_repository, "stdin_packs_found",
			   stdin_packs_ਮਿਲਿਆ_nr);
	trace2_ਅੰਕੜੇ_intmax("pack-objects", the_repository, "stdin_packs_hints",
			   stdin_packs_hints_nr);

	repo->fetch_if_missing = prev_fetch_if_missing;
}

ਸਥਿਰ ਖਾਲੀ ਜੋੜੋ_cruft_object_ਇੰਦਰਾਜ(ਅਚਲ ਬਣਤਰ object_id *oid, ਗਿਣਤੀ object_type type,
				   ਬਣਤਰ packed_git *pack, off_t ਵਿੱਥ,
				   ਅਚਲ ਅੱਖਰ *name, uint32_t mtime)
{
	ਬਣਤਰ object_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ;

	display_ਤਰੱਕੀ(ਤਰੱਕੀ_state, ++nr_ਵੇਖਿਆ);

	ਇੰਦਰਾਜ = packlist_ਲੱਭੋ(&ਮੰਜ਼ਿਲ_pack, oid);
	ਜੇ (ਇੰਦਰਾਜ) {
		ਜੇ (name) {
			ਇੰਦਰਾਜ->hash = pack_name_hash_fn(name);
			ਇੰਦਰਾਜ->no_try_delta = no_try_delta(name);
		}
	} ਨਹੀਂਤਾਂ {
		ਜੇ (!want_object_ਵਿੱਚ_pack_mtime(oid, 0, &pack, &ਵਿੱਥ, mtime))
			ਵਾਪਸ;
		ਜੇ (!pack && type == OBJ_BLOB) {
			ਬਣਤਰ odb_ਸਰੋਤ_files *files =
				odb_ਸਰੋਤ_files_downcast(the_repository->objects->ਸਰੋਤ);
			ਪੂਰਨ ਮਿਲਿਆ_2 = 0;

			ਲਈ (ਬਣਤਰ odb_files_dir *dir = files->dirs; !ਮਿਲਿਆ_2 && dir; dir = dir->ਅਗਲਾ)
				ਜੇ (!odb_ਸਰੋਤ_read_object_info(&dir->loose->ਅਧਾਰ, oid, ਨਿਰਾ, 0, ਨਿਰਾ))
					ਮਿਲਿਆ_2 = 1;

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
			ਜੇ (!ਮਿਲਿਆ_2)
				ਵਾਪਸ;
		}

		ਇੰਦਰਾਜ = create_object_ਇੰਦਰਾਜ(oid, type, pack_name_hash_fn(name),
					    0, name && no_try_delta(name),
					    pack, ਵਿੱਥ);
	}

	ਜੇ (mtime > oe_cruft_mtime(&ਮੰਜ਼ਿਲ_pack, ਇੰਦਰਾਜ))
		oe_ਸੈੱਟ_cruft_mtime(&ਮੰਜ਼ਿਲ_pack, ਇੰਦਰਾਜ, mtime);
	ਵਾਪਸ;
}

ਸਥਿਰ ਖਾਲੀ show_cruft_object(ਬਣਤਰ object *obj, ਅਚਲ ਅੱਖਰ *name, ਖਾਲੀ *ਅੰਕੜੇ ਅਣਵਰਤਿਆ)
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
	ਜੋੜੋ_cruft_object_ਇੰਦਰਾਜ(&obj->oid, obj->type, ਨਿਰਾ, 0, name, cruft_expiration);
}

ਸਥਿਰ ਖਾਲੀ show_cruft_commit(ਬਣਤਰ commit *commit, ਖਾਲੀ *ਅੰਕੜੇ)
{
	show_cruft_object((ਬਣਤਰ object*)commit, ਨਿਰਾ, ਅੰਕੜੇ);
}

ਸਥਿਰ ਪੂਰਨ cruft_include_check_obj(ਬਣਤਰ object *obj, ਖਾਲੀ *ਅੰਕੜੇ ਅਣਵਰਤਿਆ)
{
	ਵਾਪਸ !has_object_kept_pack(ਮੰਜ਼ਿਲ_pack.repo, &obj->oid, KEPT_PACK_ਵਿੱਚ_CORE);
}

ਸਥਿਰ ਪੂਰਨ cruft_include_check(ਬਣਤਰ commit *commit, ਖਾਲੀ *ਅੰਕੜੇ)
{
	ਵਾਪਸ cruft_include_check_obj((ਬਣਤਰ object*)commit, ਅੰਕੜੇ);
}

ਸਥਿਰ ਖਾਲੀ ਸੈੱਟ_cruft_mtime(ਅਚਲ ਬਣਤਰ object *object,
			    ਬਣਤਰ packed_git *pack,
			    off_t ਵਿੱਥ, ਸਮਾਂ_t mtime)
{
	ਜੋੜੋ_cruft_object_ਇੰਦਰਾਜ(&object->oid, object->type, pack, ਵਿੱਥ, ਨਿਰਾ,
			       mtime);
}

ਸਥਿਰ ਖਾਲੀ mark_pack_kept_ਵਿੱਚ_core(ਬਣਤਰ string_list *packs, ਬਿਨਾਨਿਸ਼ਾਨੀ keep)
{
	ਬਣਤਰ string_list_ਵਸਤੂ *ਵਸਤੂ = ਨਿਰਾ;
	for_each_string_list_ਵਸਤੂ(ਵਸਤੂ, packs) {
		ਬਣਤਰ packed_git *p = ਵਸਤੂ->util;
		ਜੇ (!p)
			die(_("could not find pack '%s'"), ਵਸਤੂ->string);
		ਜੇ (p->is_cruft && keep)
			ignore_packed_keep_ਵਿੱਚ_core_has_cruft = 1;
		p->pack_keep_ਵਿੱਚ_core = keep;
	}
}

ਸਥਿਰ ਖਾਲੀ ਜੋੜੋ_objects_ਵਿੱਚ_unpacked_packs(ਖਾਲੀ);

ਸਥਿਰ ਖਾਲੀ enumerate_cruft_objects(ਖਾਲੀ)
{
	ਜੇ (ਤਰੱਕੀ)
		ਤਰੱਕੀ_state = ਸ਼ੁਰੂ_ਤਰੱਕੀ(the_repository,
						_("Enumerating cruft objects"), 0);

	ਜੋੜੋ_objects_ਵਿੱਚ_unpacked_packs();
	ਜੋੜੋ_unreachable_loose_objects(ਨਿਰਾ);

	stop_ਤਰੱਕੀ(&ਤਰੱਕੀ_state);
}

ਸਥਿਰ ਖਾਲੀ enumerate_and_traverse_cruft_objects(ਬਣਤਰ string_list *fresh_packs)
{
	ਬਣਤਰ packed_git *p;
	ਬਣਤਰ rev_info revs;
	ਪੂਰਨ ret;

	repo_init_revisions(the_repository, &revs, ਨਿਰਾ);

	revs.ਟੈਗ_objects = 1;
	revs.ਰੁੱਖ_objects = 1;
	revs.blob_objects = 1;

	revs.include_check = cruft_include_check;
	revs.include_check_obj = cruft_include_check_obj;

	revs.ignore_missing_links = 1;

	ਜੇ (ਤਰੱਕੀ)
		ਤਰੱਕੀ_state = ਸ਼ੁਰੂ_ਤਰੱਕੀ(the_repository,
						_("Enumerating cruft objects"), 0);
	ret = ਜੋੜੋ_unseen_recent_objects_ਮੰਜ਼ਿਲ_traversal(&revs, cruft_expiration,
						     ਸੈੱਟ_cruft_mtime, 1);
	stop_ਤਰੱਕੀ(&ਤਰੱਕੀ_state);

	ਜੇ (ret)
		die(_("unable to add cruft objects"));

	/*
	 * Re-mark only the fresh packs as kept so that objects in
	 * unknown packs do not halt the reachability traversal early.
	 */
	repo_for_each_pack(the_repository, p)
		p->pack_keep_ਵਿੱਚ_core = 0;
	mark_pack_kept_ਵਿੱਚ_core(fresh_packs, 1);

	ਜੇ (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	ਜੇ (ਤਰੱਕੀ)
		ਤਰੱਕੀ_state = ਸ਼ੁਰੂ_ਤਰੱਕੀ(the_repository,
						_("Traversing cruft objects"), 0);
	nr_ਵੇਖਿਆ = 0;
	traverse_commit_list(&revs, show_cruft_commit, show_cruft_object, ਨਿਰਾ);

	stop_ਤਰੱਕੀ(&ਤਰੱਕੀ_state);
	release_revisions(&revs);
}

ਸਥਿਰ ਖਾਲੀ read_cruft_objects(ਖਾਲੀ)
{
	ਬਣਤਰ strbuf buf = STRBUF_INIT;
	ਬਣਤਰ string_list discard_packs = STRING_LIST_INIT_DUP;
	ਬਣਤਰ string_list fresh_packs = STRING_LIST_INIT_DUP;
	ਬਣਤਰ packed_git *p;

	ignore_packed_keep_ਵਿੱਚ_core = 1;

	ਜਦੋਂ (strbuf_getline(&buf, stdin) != EOF) {
		ਜੇ (!buf.len)
			ਜਾਰੀ;

		ਜੇ (*buf.buf == '-')
			string_list_append(&discard_packs, buf.buf + 1);
		ਨਹੀਂਤਾਂ
			string_list_append(&fresh_packs, buf.buf);
	}

	string_list_ਛਾਂਟੋ(&discard_packs);
	string_list_ਛਾਂਟੋ(&fresh_packs);

	repo_for_each_pack(the_repository, p) {
		ਅਚਲ ਅੱਖਰ *pack_name = pack_basename(p);
		ਬਣਤਰ string_list_ਵਸਤੂ *ਵਸਤੂ;

		ਵਸਤੂ = string_list_lookup(&fresh_packs, pack_name);
		ਜੇ (!ਵਸਤੂ)
			ਵਸਤੂ = string_list_lookup(&discard_packs, pack_name);

		ਜੇ (ਵਸਤੂ) {
			ਵਸਤੂ->util = p;
		} ਨਹੀਂਤਾਂ {
			/*
			 * This pack wasn't mentioned in either the "fresh" or
			 * "discard" list, so the caller didn't know about it.
			 *
			 * Mark it as kept so that its objects are ignored by
			 * add_unseen_recent_objects_to_traversal(). We'll
			 * unmark it before starting the traversal so it doesn't
			 * halt the traversal early.
			 */
			p->pack_keep_ਵਿੱਚ_core = 1;
		}
	}

	mark_pack_kept_ਵਿੱਚ_core(&fresh_packs, 1);
	mark_pack_kept_ਵਿੱਚ_core(&discard_packs, 0);

	ਜੇ (cruft_expiration)
		enumerate_and_traverse_cruft_objects(&fresh_packs);
	ਨਹੀਂਤਾਂ
		enumerate_cruft_objects();

	strbuf_release(&buf);
	string_list_clear(&discard_packs, 0);
	string_list_clear(&fresh_packs, 0);
}

ਸਥਿਰ ਖਾਲੀ read_object_list_from_stdin(ਖਾਲੀ)
{
	ਅੱਖਰ ਸਤਰ[GIT_MAX_HEXSZ + 1 + ਰਸਤਾ_MAX + 2];
	ਬਣਤਰ object_id oid;
	ਅਚਲ ਅੱਖਰ *p;

	ਲਈ (;;) {
		ਜੇ (!fgets(ਸਤਰ, ਆਕਾਰ(ਸਤਰ), stdin)) {
			ਜੇ (feof(stdin))
				ਰੋਕੋ;
			ਜੇ (!ferror(stdin))
				BUG("fgets returned NULL, not EOF, not error!");
			ਜੇ (errno != EINTR)
				die_errno("fgets");
			clearerr(stdin);
			ਜਾਰੀ;
		}
		ਜੇ (ਸਤਰ[0] == '-') {
			ਜੇ (get_oid_hex(ਸਤਰ+1, &oid))
				die(_("expected edge object ID, got garbage:\n %s"),
				    ਸਤਰ);
			ਜੋੜੋ_preferred_ਅਧਾਰ(&oid);
			ਜਾਰੀ;
		}
		ਜੇ (parse_oid_hex(ਸਤਰ, &oid, &p))
			die(_("expected object ID, got garbage:\n %s"), ਸਤਰ);

		ਜੋੜੋ_preferred_ਅਧਾਰ_object(p + 1);
		ਜੋੜੋ_object_ਇੰਦਰਾਜ(&oid, OBJ_NONE, p + 1, 0);
	}
}

ਸਥਿਰ ਖਾਲੀ show_commit(ਬਣਤਰ commit *commit, ਖਾਲੀ *ਅੰਕੜੇ ਅਣਵਰਤਿਆ)
{
	ਜੋੜੋ_object_ਇੰਦਰਾਜ(&commit->object.oid, OBJ_COMMIT, ਨਿਰਾ, 0);

	ਜੇ (write_bitmap_ਸੂਚਕਾਂਕ)
		ਸੂਚਕਾਂਕ_commit_for_bitmap(commit);

	ਜੇ (use_delta_islands)
		propagate_island_marks(the_repository, commit);
}

ਸਥਿਰ ਖਾਲੀ show_object(ਬਣਤਰ object *obj, ਅਚਲ ਅੱਖਰ *name,
			ਖਾਲੀ *ਅੰਕੜੇ ਅਣਵਰਤਿਆ)
{
	ਜੋੜੋ_preferred_ਅਧਾਰ_object(name);
	ਜੋੜੋ_object_ਇੰਦਰਾਜ(&obj->oid, obj->type, name, 0);

	ਜੇ (use_delta_islands)
		ਰਿਕਾਰਡ_ਰੁੱਖ_ਡੂੰਘਾਈ(&obj->oid, name);
}

ਸਥਿਰ ਖਾਲੀ show_object__ma_allow_any(ਬਣਤਰ object *obj, ਅਚਲ ਅੱਖਰ *name, ਖਾਲੀ *ਅੰਕੜੇ)
{
	ਪ੍ਰਤਿਜ੍ਞਾ(arg_missing_action == MA_ALLOW_ANY);

	/*
	 * Quietly ignore ALL missing objects.  This avoids problems with
	 * staging them now and getting an odd error later.
	 */
	ਜੇ (!odb_has_object(the_repository->objects, &obj->oid, 0))
		ਵਾਪਸ;

	show_object(obj, name, ਅੰਕੜੇ);
}

ਸਥਿਰ ਖਾਲੀ show_object__ma_allow_promisor(ਬਣਤਰ object *obj, ਅਚਲ ਅੱਖਰ *name, ਖਾਲੀ *ਅੰਕੜੇ)
{
	ਪ੍ਰਤਿਜ੍ਞਾ(arg_missing_action == MA_ALLOW_PROMISOR);

	/*
	 * Quietly ignore EXPECTED missing objects.  This avoids problems with
	 * staging them now and getting an odd error later.
	 */
	ਜੇ (!odb_has_object(the_repository->objects, &obj->oid, 0) &&
	    is_promisor_object(ਮੰਜ਼ਿਲ_pack.repo, &obj->oid))
		ਵਾਪਸ;

	show_object(obj, name, ਅੰਕੜੇ);
}

ਸਥਿਰ ਪੂਰਨ option_parse_missing_action(ਅਚਲ ਬਣਤਰ option *opt,
				       ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	ਬਣਤਰ repository *repo = opt->ਮੁੱਲ;

	ਪ੍ਰਤਿਜ੍ਞਾ(arg);
	ਪ੍ਰਤਿਜ੍ਞਾ(!unset);

	ਜੇ (!strcmp(arg, "error")) {
		arg_missing_action = MA_ERROR;
		fn_show_object = show_object;
		ਵਾਪਸ 0;
	}

	ਜੇ (!strcmp(arg, "allow-any")) {
		arg_missing_action = MA_ALLOW_ANY;
		repo->fetch_if_missing = 0;
		fn_show_object = show_object__ma_allow_any;
		ਵਾਪਸ 0;
	}

	ਜੇ (!strcmp(arg, "allow-promisor")) {
		arg_missing_action = MA_ALLOW_PROMISOR;
		repo->fetch_if_missing = 0;
		fn_show_object = show_object__ma_allow_promisor;
		ਵਾਪਸ 0;
	}

	die(_("invalid value for '%s': '%s'"), "--missing", arg);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ show_ਕਿਨਾਰਾ(ਬਣਤਰ commit *commit)
{
	ਜੋੜੋ_preferred_ਅਧਾਰ(&commit->object.oid);
}

ਸਥਿਰ ਪੂਰਨ ਜੋੜੋ_object_ਵਿੱਚ_unpacked_pack(ਅਚਲ ਬਣਤਰ object_id *oid,
				       ਬਣਤਰ object_info *oi,
				       ਖਾਲੀ *ਅੰਕੜੇ ਅਣਵਰਤਿਆ)
{
	ਜੇ (cruft) {
		ਜੋੜੋ_cruft_object_ਇੰਦਰਾਜ(oid, OBJ_NONE, oi->ਸਰੋਤ_infop->u.packed.pack,
				       oi->ਸਰੋਤ_infop->u.packed.ਵਿੱਥ, ਨਿਰਾ,
				       *oi->mtimep);
	} ਨਹੀਂਤਾਂ {
		ਜੋੜੋ_object_ਇੰਦਰਾਜ(oid, OBJ_NONE, "", 0);
	}
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ ਜੋੜੋ_objects_ਵਿੱਚ_unpacked_packs(ਖਾਲੀ)
{
	ਬਣਤਰ odb_ਸਰੋਤ_files *files =
		odb_ਸਰੋਤ_files_downcast(ਮੰਜ਼ਿਲ_pack.repo->objects->ਸਰੋਤ);
	ਸਮਾਂ_t mtime;
	ਬਣਤਰ odb_for_each_object_options opts = {
		.flags = ODB_FOR_EACH_OBJECT_PACK_ਕ੍ਰਮ |
			 ODB_FOR_EACH_OBJECT_LOCAL_ONLY |
			 ODB_FOR_EACH_OBJECT_SKIP_ਵਿੱਚ_CORE_KEPT_PACKS |
			 ODB_FOR_EACH_OBJECT_SKIP_ON_DISK_KEPT_PACKS,
	};
	ਬਣਤਰ odb_ਸਰੋਤ_info ਸਰੋਤ_info;
	ਬਣਤਰ object_info oi = {
		.mtimep = &mtime,
		.ਸਰੋਤ_infop = &ਸਰੋਤ_info,
	};

	ਲਈ (ਬਣਤਰ odb_files_dir *dir = files->dirs; dir; dir = dir->ਅਗਲਾ) {
		ਜੇ (!dir->local)
			ਜਾਰੀ;

		ਜੇ (odb_ਸਰੋਤ_for_each_object(&dir->packed->ਅਧਾਰ, &oi,
					       ਜੋੜੋ_object_ਵਿੱਚ_unpacked_pack, ਨਿਰਾ, &opts))
			die(_("cannot open pack index"));
	}
}

ਸਥਿਰ ਪੂਰਨ ਜੋੜੋ_loose_object(ਅਚਲ ਬਣਤਰ object_id *oid, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ,
			    ਖਾਲੀ *ਅੰਕੜੇ)
{
	ਬਣਤਰ rev_info *revs = ਅੰਕੜੇ;
	ਗਿਣਤੀ object_type type = odb_read_object_info(the_repository->objects, oid, ਨਿਰਾ);

	ਜੇ (type < 0) {
		warning(_("loose object at %s could not be examined"), ਰਸਤਾ);
		ਵਾਪਸ 0;
	}

	ਜੇ (cruft) {
		ਬਣਤਰ stat st;
		ਜੇ (stat(ਰਸਤਾ, &st) < 0) {
			ਜੇ (errno == ENOENT)
				ਵਾਪਸ 0;
			ਵਾਪਸ error_errno("unable to stat %s", oid_ਮੰਜ਼ਿਲ_hex(oid));
		}

		ਜੋੜੋ_cruft_object_ਇੰਦਰਾਜ(oid, type, ਨਿਰਾ, 0, ਨਿਰਾ,
				       st.st_mtime);
	} ਨਹੀਂਤਾਂ {
		ਜੋੜੋ_object_ਇੰਦਰਾਜ(oid, type, "", 0);
	}

	ਜੇ (revs && type == OBJ_COMMIT)
		ਜੋੜੋ_pending_oid(revs, ਨਿਰਾ, oid, 0);

	ਵਾਪਸ 0;
}

/*
 * We actually don't even have to worry about reachability here.
 * add_object_entry will weed out duplicates, so we just add every
 * loose object we find.
 */
ਸਥਿਰ ਖਾਲੀ ਜੋੜੋ_unreachable_loose_objects(ਬਣਤਰ rev_info *revs)
{
	for_each_loose_file_ਵਿੱਚ_ਸਰੋਤ(the_repository->objects->ਸਰੋਤ,
				      ਜੋੜੋ_loose_object, ਨਿਰਾ, ਨਿਰਾ, revs);
}

ਸਥਿਰ ਪੂਰਨ has_sha1_pack_kept_or_nonlocal(ਅਚਲ ਬਣਤਰ object_id *oid)
{
	ਸਥਿਰ ਬਣਤਰ packed_git *ਆਖਰੀ_ਮਿਲਿਆ = ਨਿਰਾ;
	ਬਣਤਰ packed_git *p;

	ਜੇ (ਆਖਰੀ_ਮਿਲਿਆ && ਲੱਭੋ_pack_ਇੰਦਰਾਜ_one(oid, ਆਖਰੀ_ਮਿਲਿਆ))
		ਵਾਪਸ 1;

	repo_for_each_pack(the_repository, p) {
		/*
		 * We have already checked `last_found`, so there is no need to
		 * re-check here.
		 */
		ਜੇ (p == ਆਖਰੀ_ਮਿਲਿਆ)
			ਜਾਰੀ;

		ਜੇ ((!p->pack_local || p->pack_keep || p->pack_keep_ਵਿੱਚ_core) &&
		    ਲੱਭੋ_pack_ਇੰਦਰਾਜ_one(oid, p)) {
			ਆਖਰੀ_ਮਿਲਿਆ = p;
			ਵਾਪਸ 1;
		}
	}

	ਵਾਪਸ 0;
}

/*
 * Store a list of sha1s that are should not be discarded
 * because they are either written too recently, or are
 * reachable from another object that was.
 *
 * This is filled by get_object_list.
 */
ਸਥਿਰ ਬਣਤਰ oid_array recent_objects;

ਸਥਿਰ ਪੂਰਨ loosened_object_can_be_discarded(ਅਚਲ ਬਣਤਰ object_id *oid,
					    timestamp_t mtime)
{
	ਜੇ (!unpack_unreachable_expiration)
		ਵਾਪਸ 0;
	ਜੇ (mtime > unpack_unreachable_expiration)
		ਵਾਪਸ 0;
	ਜੇ (oid_array_lookup(&recent_objects, oid) >= 0)
		ਵਾਪਸ 0;
	ਵਾਪਸ 1;
}

ਸਥਿਰ ਪੂਰਨ force_object_loose(ਬਣਤਰ odb_ਸਰੋਤ *ਸਰੋਤ,
			      ਅਚਲ ਬਣਤਰ object_id *oid,
			      ਅਚਲ ਸਮਾਂ_t *mtime)
{
	ਬਣਤਰ odb_ਸਰੋਤ_files *files = odb_ਸਰੋਤ_files_downcast(ਸਰੋਤ);
	ਅਚਲ ਬਣਤਰ git_hash_algo *compat = ਸਰੋਤ->odb->repo->compat_hash_algo;
	ਬਣਤਰ object_info oi = OBJECT_INFO_INIT;
	ਬਣਤਰ object_id compat_oid, *compat_oid_p = ਨਿਰਾ;
	ਗਿਣਤੀ object_type type;
	ਖਾਲੀ *buf = ਨਿਰਾ;
	ਮਾਪ_t len;
	ਪੂਰਨ ret;

	ਲਈ (ਬਣਤਰ odb_files_dir *dir = files->dirs; dir; dir = dir->ਅਗਲਾ)
		ਜੇ (!odb_ਸਰੋਤ_read_object_info(&dir->loose->ਅਧਾਰ, oid, ਨਿਰਾ, 0, ਨਿਰਾ))
			ਵਾਪਸ 0;

	oi.typep = &type;
	oi.sizep = &len;
	oi.contentp = &buf;
	ਜੇ (odb_read_object_info_extended(ਸਰੋਤ->odb, oid, &oi, 0)) {
		ret = error(_("cannot read object for %s"), oid_ਮੰਜ਼ਿਲ_hex(oid));
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	ਜੇ (compat) {
		ਜੇ (repo_oid_ਮੰਜ਼ਿਲ_algop(ਸਰੋਤ->odb->repo, oid, compat, &compat_oid)) {
			ret = error(_("cannot map object %s to %s"),
				    oid_ਮੰਜ਼ਿਲ_hex(oid), compat->name);
			ਜਾਓ ਬਾਹਰਲਾ;
		}

		compat_oid_p = &compat_oid;
	}

	ret = odb_ਸਰੋਤ_write_object(&files->dirs->loose->ਅਧਾਰ, buf, len, type, oid,
				      compat_oid_p, mtime, 0);

ਬਾਹਰਲਾ:
	ਮੁਕਤ(buf);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਖਾਲੀ loosen_ਅਣਵਰਤਿਆ_packed_objects(ਖਾਲੀ)
{
	ਬਣਤਰ packed_git *p;
	uint32_t i;
	uint32_t loosened_objects_nr = 0;
	ਬਣਤਰ object_id oid;

	repo_for_each_pack(the_repository, p) {
		ਜੇ (!p->pack_local || p->pack_keep || p->pack_keep_ਵਿੱਚ_core)
			ਜਾਰੀ;

		ਜੇ (open_pack_ਸੂਚਕਾਂਕ(p))
			die(_("cannot open pack index"));

		ਲਈ (i = 0; i < p->ਅੰਕ_objects; i++) {
			nth_packed_object_id(&oid, p, i);
			ਜੇ (!packlist_ਲੱਭੋ(&ਮੰਜ਼ਿਲ_pack, &oid) &&
			    !has_sha1_pack_kept_or_nonlocal(&oid) &&
			    !loosened_object_can_be_discarded(&oid, p->mtime)) {
				ਜੇ (force_object_loose(the_repository->objects->ਸਰੋਤ,
						       &oid, &p->mtime))
					die(_("unable to force loose object"));
				loosened_objects_nr++;
			}
		}
	}

	trace2_ਅੰਕੜੇ_intmax("pack-objects", the_repository,
			   "loosen_unused_packed_objects/loosened", loosened_objects_nr);
}

/*
 * This tracks any options which pack-reuse code expects to be on, or which a
 * reader of the pack might not understand, and which would therefore prevent
 * blind reuse of what we have on disk.
 */
ਸਥਿਰ ਪੂਰਨ pack_options_allow_reuse(ਖਾਲੀ)
{
	ਵਾਪਸ allow_pack_reuse != NO_PACK_REUSE &&
	       pack_ਮੰਜ਼ਿਲ_stdout &&
	       !ignore_packed_keep_on_disk &&
	       !ignore_packed_keep_ਵਿੱਚ_core &&
	       (!local || !have_non_local_packs) &&
	       !incremental;
}

ਸਥਿਰ ਪੂਰਨ get_object_list_from_bitmap(ਬਣਤਰ rev_info *revs)
{
	ਜੇ (!(bitmap_git = prepare_bitmap_walk(revs, 0)))
		ਵਾਪਸ -1;

	/*
	 * For now, force the name-hash version to be 1 since that
	 * is the version implied by the bitmap format. Later, the
	 * format can include this version explicitly in its format,
	 * allowing readers to know the version that was used during
	 * the bitmap write.
	 */
	name_hash_ਸੰਸਕਰਣ = 1;

	ਜੇ (pack_options_allow_reuse())
		reuse_partial_packfile_from_bitmap(bitmap_git,
						   &reuse_packfiles,
						   &reuse_packfiles_nr,
						   &reuse_packfile_bitmap,
						   allow_pack_reuse == MULTI_PACK_REUSE);

	ਜੇ (reuse_packfiles) {
		reuse_packfile_objects = bitmap_popcount(reuse_packfile_bitmap);
		ਜੇ (!reuse_packfile_objects)
			BUG("expected non-empty reuse bitmap");

		nr_ਨਤੀਜਾ += reuse_packfile_objects;
		nr_ਵੇਖਿਆ += reuse_packfile_objects;
		display_ਤਰੱਕੀ(ਤਰੱਕੀ_state, nr_ਵੇਖਿਆ);
	}

	traverse_bitmap_commit_list(bitmap_git, revs,
				    &ਜੋੜੋ_object_ਇੰਦਰਾਜ_from_bitmap);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ ਰਿਕਾਰਡ_recent_object(ਬਣਤਰ object *obj,
				 ਅਚਲ ਅੱਖਰ *name ਅਣਵਰਤਿਆ,
				 ਖਾਲੀ *ਅੰਕੜੇ ਅਣਵਰਤਿਆ)
{
	oid_array_append(&recent_objects, &obj->oid);
}

ਸਥਿਰ ਖਾਲੀ ਰਿਕਾਰਡ_recent_commit(ਬਣਤਰ commit *commit, ਖਾਲੀ *ਅੰਕੜੇ ਅਣਵਰਤਿਆ)
{
	oid_array_append(&recent_objects, &commit->object.oid);
}

ਸਥਿਰ ਪੂਰਨ mark_bitmap_preferred_tip(ਅਚਲ ਬਣਤਰ reference *ref, ਖਾਲੀ *ਅੰਕੜੇ ਅਣਵਰਤਿਆ)
{
	ਅਚਲ ਬਣਤਰ object_id *maybe_peeled = ref->oid;
	ਬਣਤਰ object_id peeled;
	ਬਣਤਰ object *object;

	ਜੇ (!reference_get_peeled_oid(the_repository, ref, &peeled))
		maybe_peeled = &peeled;

	object = parse_object_or_die(the_repository, maybe_peeled, ref->name);
	ਜੇ (object->type == OBJ_COMMIT)
		object->flags |= NEEDS_BITMAP;

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ is_oid_uninteresting(ਬਣਤਰ repository *repo,
				       ਬਣਤਰ object_id *oid)
{
	ਬਣਤਰ object *o = lookup_object(repo, oid);
	ਵਾਪਸ !o || (o->flags & UNINTERESTING);
}

ਸਥਿਰ ਪੂਰਨ ਜੋੜੋ_objects_by_ਰਸਤਾ(ਅਚਲ ਅੱਖਰ *ਰਸਤਾ,
			       ਬਣਤਰ oid_array *oids,
			       ਗਿਣਤੀ object_type type,
			       ਖਾਲੀ *ਅੰਕੜੇ)
{
	ਮਾਪ_t oe_ਸ਼ੁਰੂ = ਮੰਜ਼ਿਲ_pack.nr_objects;
	ਮਾਪ_t oe_ਸਿਰਾ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *processed = ਅੰਕੜੇ;

	/*
	 * First, add all objects to the packing data, including the ones
	 * marked UNINTERESTING (translated to 'exclude') as they can be
	 * used as delta bases.
	 */
	ਲਈ (ਮਾਪ_t i = 0; i < oids->nr; i++) {
		ਪੂਰਨ exclude;
		ਬਣਤਰ object_info oi = OBJECT_INFO_INIT;
		ਬਣਤਰ object_id *oid = &oids->oid[i];

		/* Skip objects that do not exist locally. */
		ਜੇ ((exclude_promisor_objects || arg_missing_action != MA_ERROR) &&
		    odb_read_object_info_extended(the_repository->objects, oid, &oi,
						  OBJECT_INFO_FOR_PREFETCH) < 0)
			ਜਾਰੀ;

		exclude = is_oid_uninteresting(the_repository, oid);

		ਜੇ (exclude && !thin)
			ਜਾਰੀ;

		ਜੋੜੋ_object_ਇੰਦਰਾਜ(oid, type, ਰਸਤਾ, exclude);

		ਜੇ (type == OBJ_COMMIT) {
			ਬਣਤਰ commit *commit;

			ਜੇ (!write_bitmap_ਸੂਚਕਾਂਕ && !use_delta_islands)
				ਜਾਰੀ;

			commit = lookup_commit(the_repository, oid);
			ਜੇ (!commit)
				die(_("could not find commit %s"), oid_ਮੰਜ਼ਿਲ_hex(oid));
			ਜੇ (write_bitmap_ਸੂਚਕਾਂਕ)
				ਸੂਚਕਾਂਕ_commit_for_bitmap(commit);
			/*
			 * Skip island propagation for boundary commits.
			 * The regular traversal's show_commit() is only
			 * called for interesting commits; matching that
			 * here keeps path-walk from doing extra work that
			 * would only be a no-op anyway (boundary commits
			 * are not in island_marks).
			 */
			ਜੇ (use_delta_islands && !exclude)
				propagate_island_marks(the_repository, commit);
		} ਨਹੀਂਤਾਂ ਜੇ (type == OBJ_ਰੁੱਖ && use_delta_islands) {
			ਰਿਕਾਰਡ_ਰੁੱਖ_ਡੂੰਘਾਈ(oid, ਰਸਤਾ);
		}
	}

	oe_ਸਿਰਾ = ਮੰਜ਼ਿਲ_pack.nr_objects;

	/* We can skip delta calculations if it is a no-op. */
	ਜੇ (oe_ਸਿਰਾ == oe_ਸ਼ੁਰੂ || !ਖਿੜਕੀ)
		ਵਾਪਸ 0;

	ALLOC_GROW(ਮੰਜ਼ਿਲ_pack.regions,
		   ਮੰਜ਼ਿਲ_pack.nr_regions + 1,
		   ਮੰਜ਼ਿਲ_pack.nr_regions_alloc);

	ਮੰਜ਼ਿਲ_pack.regions[ਮੰਜ਼ਿਲ_pack.nr_regions].ਸ਼ੁਰੂ = oe_ਸ਼ੁਰੂ;
	ਮੰਜ਼ਿਲ_pack.regions[ਮੰਜ਼ਿਲ_pack.nr_regions].nr = oe_ਸਿਰਾ - oe_ਸ਼ੁਰੂ;
	ਮੰਜ਼ਿਲ_pack.nr_regions++;

	*processed += oids->nr;
	display_ਤਰੱਕੀ(ਤਰੱਕੀ_state, *processed);

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ get_object_list_ਰਸਤਾ_walk(ਬਣਤਰ rev_info *revs)
{
	ਬਣਤਰ ਰਸਤਾ_walk_info info = ਰਸਤਾ_WALK_INFO_INIT;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ processed = 0;
	ਪੂਰਨ ਨਤੀਜਾ;

	info.revs = revs;
	info.ਰਸਤਾ_fn = ਜੋੜੋ_objects_by_ਰਸਤਾ;
	info.ਰਸਤਾ_fn_ਅੰਕੜੇ = &processed;

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
	info.ਕਿਨਾਰਾ_aggressive = shallow;

	trace2_region_enter("pack-objects", "path-walk", revs->repo);
	ਨਤੀਜਾ = walk_objects_by_ਰਸਤਾ(&info);
	trace2_region_leave("pack-objects", "path-walk", revs->repo);

	ਰਸਤਾ_walk_info_clear(&info);

	ਵਾਪਸ ਨਤੀਜਾ;
}

ਸਥਿਰ ਖਾਲੀ get_object_list(ਬਣਤਰ rev_info *revs, ਬਣਤਰ strvec *argv)
{
	ਬਣਤਰ ਤਿਆਰੀ_revision_opt s_r_opt = {
		.allow_exclude_promisor_objects = 1,
	};
	ਬਣਤਰ repo_config_ਮੁੱਲਾਂ *cfg = repo_config_ਮੁੱਲਾਂ(the_repository);
	ਅੱਖਰ ਸਤਰ[1000];
	ਪੂਰਨ flags = 0;
	ਪੂਰਨ save_warning;

	save_commit_ਬਫਰ = 0;
	ਤਿਆਰੀ_revisions_from_strvec(argv, revs, &s_r_opt);

	/* make sure shallows are read */
	is_repository_shallow(the_repository);

	save_warning = cfg->warn_on_object_refname_ambiguity;
	cfg->warn_on_object_refname_ambiguity = 0;

	ਜਦੋਂ (fgets(ਸਤਰ, ਆਕਾਰ(ਸਤਰ), stdin) != ਨਿਰਾ) {
		ਪੂਰਨ len = ਲੜੀ_ਲੰਬਾਈ(ਸਤਰ);
		ਜੇ (len && ਸਤਰ[len - 1] == '\n')
			ਸਤਰ[--len] = 0;
		ਜੇ (!len)
			ਰੋਕੋ;
		ਜੇ (*ਸਤਰ == '-') {
			ਜੇ (!strcmp(ਸਤਰ, "--not")) {
				flags ^= UNINTERESTING;
				write_bitmap_ਸੂਚਕਾਂਕ = 0;
				ਜਾਰੀ;
			}
			ਜੇ (starts_with(ਸਤਰ, "--shallow ")) {
				ਬਣਤਰ object_id oid;
				ਜੇ (get_oid_hex(ਸਤਰ + 10, &oid))
					die("not an object name '%s'", ਸਤਰ + 10);
				ਰਜਿਸਟਰ_shallow(the_repository, &oid);
				use_bitmap_ਸੂਚਕਾਂਕ = 0;
				ਜਾਰੀ;
			}
			die(_("not a rev '%s'"), ਸਤਰ);
		}
		ਜੇ (handle_revision_arg(ਸਤਰ, revs, flags, REVARG_CANNOT_BE_FILENAME))
			die(_("bad revision '%s'"), ਸਤਰ);
	}

	cfg->warn_on_object_refname_ambiguity = save_warning;

	ਜੇ (use_bitmap_ਸੂਚਕਾਂਕ && !get_object_list_from_bitmap(revs))
		ਵਾਪਸ;

	ਜੇ (use_delta_islands)
		ਲੱਦੋ_delta_islands(the_repository, ਤਰੱਕੀ);

	ਜੇ (write_bitmap_ਸੂਚਕਾਂਕ)
		for_each_preferred_bitmap_tip(the_repository, mark_bitmap_preferred_tip,
					      ਨਿਰਾ);

	ਜੇ (!fn_show_object)
		fn_show_object = show_object;

	ਜੇ (ਰਸਤਾ_walk) {
		ਜੇ (get_object_list_ਰਸਤਾ_walk(revs)) {
			warning(_("failed to pack objects via path-walk"));
			ਰਸਤਾ_walk = 0;
		}
	}

	ਜੇ (!ਰਸਤਾ_walk) {
		ਜੇ (prepare_revision_walk(revs))
			die(_("revision walk setup failed"));
		mark_ਕਿਨਾਰੇ_uninteresting(revs, show_ਕਿਨਾਰਾ, sparse);
		traverse_commit_list(revs,
				show_commit, fn_show_object,
				ਨਿਰਾ);
	}

	ਜੇ (unpack_unreachable_expiration) {
		revs->ignore_missing_links = 1;
		ਜੇ (ਜੋੜੋ_unseen_recent_objects_ਮੰਜ਼ਿਲ_traversal(revs,
				unpack_unreachable_expiration, ਨਿਰਾ, 0))
			die(_("unable to add recent objects"));
		ਜੇ (prepare_revision_walk(revs))
			die(_("revision walk setup failed"));
		traverse_commit_list(revs, ਰਿਕਾਰਡ_recent_commit,
				     ਰਿਕਾਰਡ_recent_object, ਨਿਰਾ);
	}

	ਜੇ (keep_unreachable)
		ਜੋੜੋ_objects_ਵਿੱਚ_unpacked_packs();
	ਜੇ (pack_loose_unreachable)
		ਜੋੜੋ_unreachable_loose_objects(ਨਿਰਾ);
	ਜੇ (unpack_unreachable)
		loosen_ਅਣਵਰਤਿਆ_packed_objects();

	oid_array_clear(&recent_objects);
}

ਸਥਿਰ ਖਾਲੀ ਜੋੜੋ_extra_kept_packs(ਅਚਲ ਬਣਤਰ string_list *ਨਾਂ)
{
	ਬਣਤਰ packed_git *p;

	ਜੇ (!ਨਾਂ->nr)
		ਵਾਪਸ;

	repo_for_each_pack(the_repository, p) {
		ਅਚਲ ਅੱਖਰ *name = basename(p->pack_name);
		ਪੂਰਨ i;

		ਜੇ (!p->pack_local)
			ਜਾਰੀ;

		ਲਈ (i = 0; i < ਨਾਂ->nr; i++)
			ਜੇ (!fspathcmp(name, ਨਾਂ->ਚੀਜ਼ਾਂ[i].string))
				ਰੋਕੋ;

		ਜੇ (i < ਨਾਂ->nr) {
			p->pack_keep_ਵਿੱਚ_core = 1;
			ignore_packed_keep_ਵਿੱਚ_core = 1;
			ਜਾਰੀ;
		}
	}
}

ਸਥਿਰ ਪੂਰਨ option_parse_quiet(ਅਚਲ ਬਣਤਰ option *opt, ਅਚਲ ਅੱਖਰ *arg,
			      ਪੂਰਨ unset)
{
	ਪੂਰਨ *val = opt->ਮੁੱਲ;

	BUG_ON_OPT_ARG(arg);

	ਜੇ (!unset)
		*val = 0;
	ਨਹੀਂਤਾਂ ਜੇ (!*val)
		*val = 1;
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ option_parse_ਸੂਚਕਾਂਕ_ਸੰਸਕਰਣ(ਅਚਲ ਬਣਤਰ option *opt,
				      ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	ਬਣਤਰ pack_idx_option *popts = opt->ਮੁੱਲ;
	ਅੱਖਰ *c;
	ਅਚਲ ਅੱਖਰ *val = arg;

	BUG_ON_OPT_NEG(unset);

	popts->ਸੰਸਕਰਣ = strtoul(val, &c, 10);
	ਜੇ (popts->ਸੰਸਕਰਣ > 2)
		die(_("unsupported index version %s"), val);
	ਜੇ (*c == ',' && c[1])
		popts->off32_ਹੱਦਬੰਦੀ = strtoul(c+1, &c, 0);
	ਜੇ (*c || popts->off32_ਹੱਦਬੰਦੀ & 0x80000000)
		die(_("bad index version '%s'"), val);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ option_parse_unpack_unreachable(ਅਚਲ ਬਣਤਰ option *opt ਅਣਵਰਤਿਆ,
					   ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	ਜੇ (unset) {
		unpack_unreachable = 0;
		unpack_unreachable_expiration = 0;
	}
	ਨਹੀਂਤਾਂ {
		unpack_unreachable = 1;
		ਜੇ (arg)
			unpack_unreachable_expiration = approxidate(arg);
	}
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ option_parse_cruft_expiration(ਅਚਲ ਬਣਤਰ option *opt ਅਣਵਰਤਿਆ,
					 ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	ਜੇ (unset) {
		cruft = 0;
		cruft_expiration = 0;
	} ਨਹੀਂਤਾਂ {
		cruft = 1;
		ਜੇ (arg)
			cruft_expiration = approxidate(arg);
	}
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ is_not_ਵਿੱਚ_promisor_pack_obj(ਬਣਤਰ object *obj, ਖਾਲੀ *ਅੰਕੜੇ ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ odb_ਸਰੋਤ_info ਸਰੋਤ_info;
	ਬਣਤਰ object_info info = {
		.ਸਰੋਤ_infop = &ਸਰੋਤ_info,
	};

	ਜੇ (odb_read_object_info_extended(the_repository->objects, &obj->oid, &info, 0))
		BUG("should_include_obj should only be called on existing objects");
	ਵਾਪਸ ਸਰੋਤ_info.ਸਰੋਤ->type != ODB_ਸਰੋਤ_PACKED || !ਸਰੋਤ_info.u.packed.pack->pack_promisor;
}

ਸਥਿਰ ਪੂਰਨ is_not_ਵਿੱਚ_promisor_pack(ਬਣਤਰ commit *commit, ਖਾਲੀ *ਅੰਕੜੇ) {
	ਵਾਪਸ is_not_ਵਿੱਚ_promisor_pack_obj((ਬਣਤਰ object *) commit, ਅੰਕੜੇ);
}

ਸਥਿਰ ਪੂਰਨ parse_stdin_packs_ਬਹੁਲਕ(ਅਚਲ ਬਣਤਰ option *opt, ਅਚਲ ਅੱਖਰ *arg,
				  ਪੂਰਨ unset)
{
	ਗਿਣਤੀ stdin_packs_ਬਹੁਲਕ *ਬਹੁਲਕ = opt->ਮੁੱਲ;

	ਜੇ (unset)
		*ਬਹੁਲਕ = STDIN_PACKS_ਬਹੁਲਕ_NONE;
	ਨਹੀਂਤਾਂ ਜੇ (!arg || !*arg)
		*ਬਹੁਲਕ = STDIN_PACKS_ਬਹੁਲਕ_STANDARD;
	ਨਹੀਂਤਾਂ ਜੇ (!strcmp(arg, "follow"))
		*ਬਹੁਲਕ = STDIN_PACKS_ਬਹੁਲਕ_FOLLOW;
	ਨਹੀਂਤਾਂ
		die(_("invalid value for '%s': '%s'"), opt->long_name, arg);

	ਵਾਪਸ 0;
}

ਪੂਰਨ cmd_pack_objects(ਪੂਰਨ argc,
		     ਅਚਲ ਅੱਖਰ **argv,
		     ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		     ਬਣਤਰ repository *repo)
{
	ਪੂਰਨ use_ਅੰਦਰੂਨੀ_rev_list = 0;
	ਪੂਰਨ all_ਤਰੱਕੀ_implied = 0;
	ਬਣਤਰ strvec rp = STRVEC_INIT;
	ਪੂਰਨ rev_list_unpacked = 0, rev_list_all = 0, rev_list_reflog = 0;
	ਪੂਰਨ rev_list_ਸੂਚਕਾਂਕ = 0;
	ਗਿਣਤੀ stdin_packs_ਬਹੁਲਕ stdin_packs = STDIN_PACKS_ਬਹੁਲਕ_NONE;
	ਬਣਤਰ string_list keep_pack_list = STRING_LIST_INIT_NODUP;
	ਬਣਤਰ list_objects_filter_options filter_options =
		LIST_OBJECTS_FILTER_INIT;
	ਬਣਤਰ repo_config_ਮੁੱਲਾਂ *cfg = repo_config_ਮੁੱਲਾਂ(the_repository);

	ਬਣਤਰ option pack_objects_options[] = {
		OPT_CALLBACK_F('q', "quiet", &ਤਰੱਕੀ, ਨਿਰਾ,
			       N_("do not show progress meter"),
			       PARSE_OPT_NOARG, option_parse_quiet),
		OPT_ਸੈੱਟ_INT(0, "progress", &ਤਰੱਕੀ,
			    N_("show progress meter"), 1),
		OPT_ਸੈੱਟ_INT(0, "all-progress", &ਤਰੱਕੀ,
			    N_("show progress meter during object writing phase"), 2),
		OPT_BOOL(0, "all-progress-implied",
			 &all_ਤਰੱਕੀ_implied,
			 N_("similar to --all-progress when progress meter is shown")),
		OPT_CALLBACK_F(0, "index-version", &pack_idx_opts, N_("<version>[,<offset>]"),
		  N_("write the pack index file in the specified idx format version"),
		  PARSE_OPT_NONEG, option_parse_ਸੂਚਕਾਂਕ_ਸੰਸਕਰਣ),
		OPT_UNSIGNED(0, "max-pack-size", &pack_ਮਾਪ_ਹੱਦਬੰਦੀ,
			     N_("maximum size of each output pack file")),
		OPT_BOOL(0, "local", &local,
			 N_("ignore borrowed objects from alternate object store")),
		OPT_BOOL(0, "incremental", &incremental,
			 N_("ignore packed objects")),
		OPT_INTEGER(0, "window", &ਖਿੜਕੀ,
			    N_("limit pack window by objects")),
		OPT_UNSIGNED(0, "window-memory", &ਖਿੜਕੀ_ਸਮ੍ਰਿਤੀ_ਹੱਦਬੰਦੀ,
			     N_("limit pack window by memory in addition to object limit")),
		OPT_INTEGER(0, "depth", &ਡੂੰਘਾਈ,
			    N_("maximum length of delta chain allowed in the resulting pack")),
		OPT_BOOL(0, "reuse-delta", &reuse_delta,
			 N_("reuse existing deltas")),
		OPT_BOOL(0, "reuse-object", &reuse_object,
			 N_("reuse existing objects")),
		OPT_BOOL(0, "delta-base-offset", &allow_ofs_delta,
			 N_("use OFS_DELTA objects")),
		OPT_INTEGER(0, "threads", &delta_ਖੋਜ_ਧਾਗੇ,
			    N_("use threads when searching for best delta matches")),
		OPT_BOOL(0, "non-empty", &non_ਖਾਲੀ_ਥਾਵਾਂ,
			 N_("do not create an empty pack output")),
		OPT_BOOL(0, "revs", &use_ਅੰਦਰੂਨੀ_rev_list,
			 N_("read revision arguments from standard input")),
		OPT_ਸੈੱਟ_INT_F(0, "unpacked", &rev_list_unpacked,
			      N_("limit the objects to those that are not yet packed"),
			      1, PARSE_OPT_NONEG),
		OPT_ਸੈੱਟ_INT_F(0, "all", &rev_list_all,
			      N_("include objects reachable from any reference"),
			      1, PARSE_OPT_NONEG),
		OPT_ਸੈੱਟ_INT_F(0, "reflog", &rev_list_reflog,
			      N_("include objects referred by reflog entries"),
			      1, PARSE_OPT_NONEG),
		OPT_ਸੈੱਟ_INT_F(0, "indexed-objects", &rev_list_ਸੂਚਕਾਂਕ,
			      N_("include objects referred to by the index"),
			      1, PARSE_OPT_NONEG),
		OPT_CALLBACK_F(0, "stdin-packs", &stdin_packs, N_("mode"),
			     N_("read packs from stdin"),
			     PARSE_OPT_OPTARG, parse_stdin_packs_ਬਹੁਲਕ),
		OPT_BOOL(0, "stdout", &pack_ਮੰਜ਼ਿਲ_stdout,
			 N_("output pack to stdout")),
		OPT_BOOL(0, "include-tag", &include_ਟੈਗ,
			 N_("include tag objects that refer to objects to be packed")),
		OPT_BOOL(0, "keep-unreachable", &keep_unreachable,
			 N_("keep unreachable objects")),
		OPT_BOOL(0, "pack-loose-unreachable", &pack_loose_unreachable,
			 N_("pack loose unreachable objects")),
		OPT_CALLBACK_F(0, "unpack-unreachable", ਨਿਰਾ, N_("time"),
		  N_("unpack unreachable objects newer than <time>"),
		  PARSE_OPT_OPTARG, option_parse_unpack_unreachable),
		OPT_BOOL(0, "cruft", &cruft, N_("create a cruft pack")),
		OPT_CALLBACK_F(0, "cruft-expiration", ਨਿਰਾ, N_("time"),
		  N_("expire cruft objects older than <time>"),
		  PARSE_OPT_OPTARG, option_parse_cruft_expiration),
		OPT_BOOL(0, "sparse", &sparse,
			 N_("use the sparse reachability algorithm")),
		OPT_BOOL(0, "thin", &thin,
			 N_("create thin packs")),
		OPT_BOOL(0, "path-walk", &ਰਸਤਾ_walk,
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
		OPT_BOOL(0, "use-bitmap-index", &use_bitmap_ਸੂਚਕਾਂਕ,
			 N_("use a bitmap index if available to speed up counting objects")),
		OPT_ਸੈੱਟ_INT(0, "write-bitmap-index", &write_bitmap_ਸੂਚਕਾਂਕ,
			    N_("write a bitmap index together with the pack index"),
			    WRITE_BITMAP_TRUE),
		OPT_ਸੈੱਟ_INT_F(0, "write-bitmap-index-quiet",
			      &write_bitmap_ਸੂਚਕਾਂਕ,
			      N_("write a bitmap index if possible"),
			      WRITE_BITMAP_QUIET, PARSE_OPT_HIDDEN),
		OPT_PARSE_LIST_OBJECTS_FILTER(&filter_options),
		OPT_CALLBACK_F(0, "missing", repo, N_("action"),
		  N_("handling for missing objects"), PARSE_OPT_NONEG,
		  option_parse_missing_action),
		OPT_BOOL(0, "exclude-promisor-objects", &exclude_promisor_objects,
			 N_("do not pack objects in promisor packfiles")),
		OPT_BOOL(0, "exclude-promisor-objects-best-effort",
			 &exclude_promisor_objects_ਉੱਤਮ_effort,
			 N_("implies --missing=allow-any")),
		OPT_BOOL(0, "delta-islands", &use_delta_islands,
			 N_("respect islands during delta compression")),
		OPT_STRING_LIST(0, "uri-protocol", &uri_protocols,
				N_("protocol"),
				N_("exclude any configured uploadpack.blobpackfileuri with this protocol")),
		OPT_INTEGER(0, "name-hash-version", &name_hash_ਸੰਸਕਰਣ,
			 N_("use the specified name-hash function to group similar objects")),
		OPT_ਸਿਰਾ(),
	};

	ਜੇ (DFS_ਅੰਕ_ਹਾਲਤਾਂ > (1 << OE_DFS_STATE_ਬਿੱਟ))
		BUG("too many dfs states, increase OE_DFS_STATE_BITS");

	disable_replace_refs();

	sparse = git_env_bool("GIT_TEST_PACK_SPARSE", -1);
	ਜੇ (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		ਜੇ (sparse < 0)
			sparse = the_repository->settings.pack_use_sparse;
		ਜੇ (the_repository->settings.pack_use_multi_pack_reuse)
			allow_pack_reuse = MULTI_PACK_REUSE;
	}

	reset_pack_idx_option(&pack_idx_opts);
	pack_idx_opts.flags |= WRITE_REV;
	repo_config(the_repository, git_pack_config, ਨਿਰਾ);
	ਜੇ (git_env_bool(GIT_TEST_NO_WRITE_REV_ਸੂਚਕਾਂਕ, 0))
		pack_idx_opts.flags &= ~WRITE_REV;

	ਤਰੱਕੀ = isatty(2);
	argc = parse_options(argc, argv, ਅਗੇਤਰ, pack_objects_options,
			     pack_usage, 0);

	ਜੇ (argc) {
		ਅਧਾਰ_name = argv[0];
		argc--;
	}
	ਜੇ (pack_ਮੰਜ਼ਿਲ_stdout != !ਅਧਾਰ_name || argc)
		usage_with_options(pack_usage, pack_objects_options);

	ਜੇ (ਰਸਤਾ_walk < 0) {
		ਜੇ (use_bitmap_ਸੂਚਕਾਂਕ > 0 ||
		    !use_ਅੰਦਰੂਨੀ_rev_list)
			ਰਸਤਾ_walk = 0;
		ਨਹੀਂਤਾਂ ਜੇ (the_repository->gitdir &&
			 the_repository->settings.pack_use_ਰਸਤਾ_walk)
			ਰਸਤਾ_walk = 1;
		ਨਹੀਂਤਾਂ
			ਰਸਤਾ_walk = git_env_bool("GIT_TEST_PACK_PATH_WALK", 0);
	}

	ਜੇ (ਡੂੰਘਾਈ < 0)
		ਡੂੰਘਾਈ = 0;
	ਜੇ (ਡੂੰਘਾਈ >= (1 << OE_ਡੂੰਘਾਈ_ਬਿੱਟ)) {
		warning(_("delta chain depth %d is too deep, forcing %d"),
			ਡੂੰਘਾਈ, (1 << OE_ਡੂੰਘਾਈ_ਬਿੱਟ) - 1);
		ਡੂੰਘਾਈ = (1 << OE_ਡੂੰਘਾਈ_ਬਿੱਟ) - 1;
	}
	ਜੇ (cache_max_small_delta_ਮਾਪ >= (1U << OE_Z_DELTA_ਬਿੱਟ)) {
		warning(_("pack.deltaCacheLimit is too high, forcing %d"),
			(1U << OE_Z_DELTA_ਬਿੱਟ) - 1);
		cache_max_small_delta_ਮਾਪ = (1U << OE_Z_DELTA_ਬਿੱਟ) - 1;
	}
	ਜੇ (ਖਿੜਕੀ < 0)
		ਖਿੜਕੀ = 0;

	strvec_ਧੱਕੋ(&rp, "pack-objects");

	ਜੇ (ਰਸਤਾ_walk) {
		ਅਚਲ ਅੱਖਰ *option = ਨਿਰਾ;
		ਜੇ (!ਰਸਤਾ_walk_filter_compatible(&filter_options))
			option = "--filter";

		ਜੇ (option) {
			warning(_("cannot use %s with %s"),
				option, "--path-walk");
			ਰਸਤਾ_walk = 0;
		}
	}
	ਜੇ (ਰਸਤਾ_walk) {
		strvec_ਧੱਕੋ(&rp, "--objects");
	} ਨਹੀਂਤਾਂ ਜੇ (thin) {
		use_ਅੰਦਰੂਨੀ_rev_list = 1;
		strvec_ਧੱਕੋ(&rp, shallow
				? "--objects-edge-aggressive"
				: "--objects-edge");
	} ਨਹੀਂਤਾਂ
		strvec_ਧੱਕੋ(&rp, "--objects");

	ਜੇ (rev_list_all) {
		use_ਅੰਦਰੂਨੀ_rev_list = 1;
		strvec_ਧੱਕੋ(&rp, "--all");
	}
	ਜੇ (rev_list_reflog) {
		use_ਅੰਦਰੂਨੀ_rev_list = 1;
		strvec_ਧੱਕੋ(&rp, "--reflog");
	}
	ਜੇ (rev_list_ਸੂਚਕਾਂਕ) {
		use_ਅੰਦਰੂਨੀ_rev_list = 1;
		strvec_ਧੱਕੋ(&rp, "--indexed-objects");
	}
	ਜੇ (rev_list_unpacked && !stdin_packs) {
		use_ਅੰਦਰੂਨੀ_rev_list = 1;
		strvec_ਧੱਕੋ(&rp, "--unpacked");
	}

	die_for_incompatible_opt2(exclude_promisor_objects,
				  "--exclude-promisor-objects",
				  exclude_promisor_objects_ਉੱਤਮ_effort,
				  "--exclude-promisor-objects-best-effort");
	ਜੇ (exclude_promisor_objects) {
		repo->fetch_if_missing = 0;

		/* --stdin-packs handles promisor objects separately. */
		ਜੇ (!stdin_packs) {
			use_ਅੰਦਰੂਨੀ_rev_list = 1;
			strvec_ਧੱਕੋ(&rp, "--exclude-promisor-objects");
		}
	} ਨਹੀਂਤਾਂ ਜੇ (exclude_promisor_objects_ਉੱਤਮ_effort) {
		use_ਅੰਦਰੂਨੀ_rev_list = 1;
		arg_missing_action = MA_ALLOW_ANY;
		repo->fetch_if_missing = 0;
		fn_show_object = show_object__ma_allow_any;
		/* revs configured below */
	}
	ਜੇ (unpack_unreachable || keep_unreachable || pack_loose_unreachable)
		use_ਅੰਦਰੂਨੀ_rev_list = 1;

	ਜੇ (!reuse_object)
		reuse_delta = 0;
	ਜੇ (cfg->pack_compression_level == -1)
		cfg->pack_compression_level = Z_DEFAULT_COMPRESSION;
	ਨਹੀਂਤਾਂ ਜੇ (cfg->pack_compression_level < 0 || cfg->pack_compression_level > Z_ਉੱਤਮ_COMPRESSION)
		die(_("bad pack compression level %d"), cfg->pack_compression_level);

	ਜੇ (!delta_ਖੋਜ_ਧਾਗੇ)	/* --threads=0 means autodetect */
		delta_ਖੋਜ_ਧਾਗੇ = online_cpus();

	ਜੇ (!HAVE_ਧਾਗੇ && delta_ਖੋਜ_ਧਾਗੇ != 1)
		warning(_("no threads support, ignoring --threads"));
	ਜੇ (!pack_ਮੰਜ਼ਿਲ_stdout && !pack_ਮਾਪ_ਹੱਦਬੰਦੀ)
		pack_ਮਾਪ_ਹੱਦਬੰਦੀ = pack_ਮਾਪ_ਹੱਦਬੰਦੀ_cfg;
	ਜੇ (pack_ਮੰਜ਼ਿਲ_stdout && pack_ਮਾਪ_ਹੱਦਬੰਦੀ)
		die(_("--max-pack-size cannot be used to build a pack for transfer"));
	ਜੇ (pack_ਮਾਪ_ਹੱਦਬੰਦੀ && pack_ਮਾਪ_ਹੱਦਬੰਦੀ < 1024*1024) {
		warning(_("minimum pack size limit is 1 MiB"));
		pack_ਮਾਪ_ਹੱਦਬੰਦੀ = 1024*1024;
	}

	ਜੇ (!pack_ਮੰਜ਼ਿਲ_stdout && thin)
		die(_("--thin cannot be used to build an indexable pack"));

	die_for_incompatible_opt2(keep_unreachable, "--keep-unreachable",
				  unpack_unreachable, "--unpack-unreachable");
	ਜੇ (!rev_list_all || !rev_list_reflog || !rev_list_ਸੂਚਕਾਂਕ)
		unpack_unreachable_expiration = 0;

	die_for_incompatible_opt2(stdin_packs, "--stdin-packs",
				  filter_options.choice, "--filter");


	ਜੇ (stdin_packs && use_ਅੰਦਰੂਨੀ_rev_list)
		die(_("cannot use internal rev list with --stdin-packs"));

	ਜੇ (cruft) {
		ਜੇ (use_ਅੰਦਰੂਨੀ_rev_list)
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
	ਜੇ (!pack_ਮੰਜ਼ਿਲ_stdout)
		use_bitmap_ਸੂਚਕਾਂਕ_default = 0;

	ਜੇ (use_bitmap_ਸੂਚਕਾਂਕ < 0)
		use_bitmap_ਸੂਚਕਾਂਕ = use_bitmap_ਸੂਚਕਾਂਕ_default;

	/* "hard" reasons not to use bitmaps; these just won't work at all */
	ਜੇ (!use_ਅੰਦਰੂਨੀ_rev_list || (!pack_ਮੰਜ਼ਿਲ_stdout && write_bitmap_ਸੂਚਕਾਂਕ) || is_repository_shallow(the_repository))
		use_bitmap_ਸੂਚਕਾਂਕ = 0;

	ਜੇ (pack_ਮੰਜ਼ਿਲ_stdout || !rev_list_all)
		write_bitmap_ਸੂਚਕਾਂਕ = 0;

	ਜੇ (name_hash_ਸੰਸਕਰਣ < 0)
		name_hash_ਸੰਸਕਰਣ = (ਪੂਰਨ)git_env_ulong("GIT_TEST_NAME_HASH_VERSION", 1);

	validate_name_hash_ਸੰਸਕਰਣ();

	ਜੇ (use_delta_islands)
		strvec_ਧੱਕੋ(&rp, "--topo-order");

	ਜੇ (ਤਰੱਕੀ && all_ਤਰੱਕੀ_implied)
		ਤਰੱਕੀ = 2;

	ਜੋੜੋ_extra_kept_packs(&keep_pack_list);
	ਜੇ (ignore_packed_keep_on_disk) {
		ਬਣਤਰ packed_git *p;

		repo_for_each_pack(the_repository, p)
			ਜੇ (p->pack_local && p->pack_keep)
				ਰੋਕੋ;
		ਜੇ (!p) /* no keep-able packs found */
			ignore_packed_keep_on_disk = 0;
	}
	ਜੇ (local) {
		/*
		 * unlike ignore_packed_keep_on_disk above, we do not
		 * want to unset "local" based on looking at packs, as
		 * it also covers non-local objects
		 */
		ਬਣਤਰ packed_git *p;

		repo_for_each_pack(the_repository, p) {
			ਜੇ (!p->pack_local) {
				have_non_local_packs = 1;
				ਰੋਕੋ;
			}
		}
	}

	trace2_region_enter("pack-objects", "enumerate-objects",
			    the_repository);
	prepare_packing_ਅੰਕੜੇ(the_repository, &ਮੰਜ਼ਿਲ_pack);

	ਜੇ (ਤਰੱਕੀ && !cruft)
		ਤਰੱਕੀ_state = ਸ਼ੁਰੂ_ਤਰੱਕੀ(the_repository,
						_("Enumerating objects"), 0);
	ਜੇ (stdin_packs) {
		read_stdin_packs(repo, stdin_packs, rev_list_unpacked);
	} ਨਹੀਂਤਾਂ ਜੇ (cruft) {
		read_cruft_objects();
	} ਨਹੀਂਤਾਂ ਜੇ (!use_ਅੰਦਰੂਨੀ_rev_list) {
		read_object_list_from_stdin();
	} ਨਹੀਂਤਾਂ {
		ਬਣਤਰ rev_info revs;

		repo_init_revisions(the_repository, &revs, ਨਿਰਾ);
		list_objects_filter_copy(&revs.filter, &filter_options);
		ਜੇ (exclude_promisor_objects_ਉੱਤਮ_effort) {
			revs.include_check = is_not_ਵਿੱਚ_promisor_pack;
			revs.include_check_obj = is_not_ਵਿੱਚ_promisor_pack_obj;
		}
		get_object_list(&revs, &rp);
		release_revisions(&revs);
	}
	cleanup_preferred_ਅਧਾਰ();
	ਜੇ (include_ਟੈਗ && nr_ਨਤੀਜਾ)
		refs_for_each_ਟੈਗ_ref(get_main_ref_store(the_repository),
				      ਜੋੜੋ_ref_ਟੈਗ, ਨਿਰਾ);
	stop_ਤਰੱਕੀ(&ਤਰੱਕੀ_state);
	trace2_region_leave("pack-objects", "enumerate-objects",
			    the_repository);

	ਜੇ (non_ਖਾਲੀ_ਥਾਵਾਂ && !nr_ਨਤੀਜਾ)
		ਜਾਓ cleanup;
	ਜੇ (nr_ਨਤੀਜਾ) {
		trace2_region_enter("pack-objects", "prepare-pack",
				    the_repository);
		prepare_pack(ਖਿੜਕੀ, ਡੂੰਘਾਈ);
		trace2_region_leave("pack-objects", "prepare-pack",
				    the_repository);
	}

	trace2_region_enter("pack-objects", "write-pack-file", the_repository);
	write_excluded_by_configs();
	write_pack_file();
	trace2_region_leave("pack-objects", "write-pack-file", the_repository);

	ਜੇ (ਤਰੱਕੀ)
		fprintf_ln(stderr,
			   _("Total %"PRIu32" (delta %"PRIu32"),"
			     " reused %"PRIu32" (delta %"PRIu32"),"
			     " pack-reused %"PRIu32" (from %"PRIuMAX")"),
			   written, written_delta, reused, reused_delta,
			   reuse_packfile_objects,
			   (uintmax_t)reuse_packfiles_ਵਰਤਿਆ_nr);

	trace2_ਅੰਕੜੇ_intmax("pack-objects", the_repository, "written", written);
	trace2_ਅੰਕੜੇ_intmax("pack-objects", the_repository, "written/delta", written_delta);
	trace2_ਅੰਕੜੇ_intmax("pack-objects", the_repository, "reused", reused);
	trace2_ਅੰਕੜੇ_intmax("pack-objects", the_repository, "reused/delta", reused_delta);
	trace2_ਅੰਕੜੇ_intmax("pack-objects", the_repository, "pack-reused", reuse_packfile_objects);
	trace2_ਅੰਕੜੇ_intmax("pack-objects", the_repository, "packs-reused", reuse_packfiles_ਵਰਤਿਆ_nr);

cleanup:
	clear_packing_ਅੰਕੜੇ(&ਮੰਜ਼ਿਲ_pack);
	list_objects_filter_release(&filter_options);
	string_list_clear(&keep_pack_list, 0);
	strvec_clear(&rp);

	ਵਾਪਸ 0;
}
