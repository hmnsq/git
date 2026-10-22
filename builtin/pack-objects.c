#وصف_ڪريو USE_THE_REPOSITORY_VARIABLE
#وصف_ڪريو DISABLE_SIGN_COMPARE_WARNINGS

#ضمن "builtin.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "hex.h"
#ضمن "config.h"
#ضمن "attr.h"
#ضمن "object.h"
#ضمن "commit.h"
#ضمن "tag.h"
#ضمن "delta.h"
#ضمن "pack.h"
#ضمن "pack-revindex.h"
#ضمن "csum-file.h"
#ضمن "tree-walk.h"
#ضمن "diff.h"
#ضمن "revision.h"
#ضمن "list-objects.h"
#ضمن "list-objects-filter-options.h"
#ضمن "pack-objects.h"
#ضمن "progress.h"
#ضمن "refs.h"
#ضمن "thread-utils.h"
#ضمن "pack-bitmap.h"
#ضمن "delta-islands.h"
#ضمن "reachable.h"
#ضمن "oid-array.h"
#ضمن "strvec.h"
#ضمن "strmap.h"
#ضمن "list.h"
#ضمن "packfile.h"
#ضمن "object-file.h"
#ضمن "object-file-convert.h"
#ضمن "odb.h"
#ضمن "odb/streaming.h"
#ضمن "replace-object.h"
#ضمن "dir.h"
#ضمن "midx.h"
#ضمن "trace2.h"
#ضمن "shallow.h"
#ضمن "promisor-remote.h"
#ضمن "pack-mtimes.h"
#ضمن "parse-options.h"
#ضمن "pkt-line.h"
#ضمن "blob.h"
#ضمن "tree.h"
#ضمن "path-walk.h"

/*
 * Objects we are going to pack are collected in the `to_pack` structure.
 * It contains an array (dynamically expanded) of the object data, and a map
 * that can resolve SHA1s to their position in the array.
 */
ساكن بنية packing_معلومات منزل_pack;

ساكن مضمن بنية object_داخلا *oe_delta(
		ثابت بنية packing_معلومات *pack,
		ثابت بنية object_داخلا *e)
{
	إذا (!e->delta_idx)
		أرجع عدم;
	إذا (e->ext_بنياد)
		أرجع &pack->ext_bases[e->delta_idx - 1];
	وإلا
		أرجع &pack->objects[e->delta_idx - 1];
}

ساكن مضمن سائيز_t oe_delta_سائيز(بنية packing_معلومات *pack,
				   ثابت بنية object_داخلا *e)
{
	إذا (e->delta_سائيز_valid)
		أرجع e->delta_سائيز;

	/*
	 * pack->delta_size[] can't be NULL because oe_set_delta_size()
	 * must have been called when a new delta is saved with
	 * oe_set_delta().
	 * If oe_delta() returns NULL (i.e. default state, which means
	 * delta_size_valid is also false), then the caller must never
	 * call oe_delta_size().
	 */
	أرجع pack->delta_سائيز_2[e - pack->objects];
}

سائيز_t oe_get_سائيز_slow(بنية packing_معلومات *pack,
			ثابت بنية object_داخلا *e);

ساكن مضمن سائيز_t oe_سائيز(بنية packing_معلومات *pack,
			     ثابت بنية object_داخلا *e)
{
	إذا (e->سائيز_valid)
		أرجع e->سائيز_2;

	أرجع oe_get_سائيز_slow(pack, e);
}

ساكن مضمن فراغ oe_سيٽ_delta(بنية packing_معلومات *pack,
				بنية object_داخلا *e,
				بنية object_داخلا *delta)
{
	إذا (delta)
		e->delta_idx = (delta - pack->objects) + 1;
	وإلا
		e->delta_idx = 0;
}

ساكن مضمن بنية object_داخلا *oe_delta_sibling(
		ثابت بنية packing_معلومات *pack,
		ثابت بنية object_داخلا *e)
{
	إذا (e->delta_sibling_idx)
		أرجع &pack->objects[e->delta_sibling_idx - 1];
	أرجع عدم;
}

ساكن مضمن بنية object_داخلا *oe_delta_ٻار(
		ثابت بنية packing_معلومات *pack,
		ثابت بنية object_داخلا *e)
{
	إذا (e->delta_ٻار_idx)
		أرجع &pack->objects[e->delta_ٻار_idx - 1];
	أرجع عدم;
}

ساكن مضمن فراغ oe_سيٽ_delta_ٻار(بنية packing_معلومات *pack,
				      بنية object_داخلا *e,
				      بنية object_داخلا *delta)
{
	إذا (delta)
		e->delta_ٻار_idx = (delta - pack->objects) + 1;
	وإلا
		e->delta_ٻار_idx = 0;
}

ساكن مضمن فراغ oe_سيٽ_delta_sibling(بنية packing_معلومات *pack,
					بنية object_داخلا *e,
					بنية object_داخلا *delta)
{
	إذا (delta)
		e->delta_sibling_idx = (delta - pack->objects) + 1;
	وإلا
		e->delta_sibling_idx = 0;
}

ساكن مضمن فراغ oe_سيٽ_سائيز(بنية packing_معلومات *pack,
			       بنية object_داخلا *e,
			       سائيز_t سائيز)
{
	إذا (سائيز < pack->oe_سائيز_حد_بندي) {
		e->سائيز_2 = سائيز;
		e->سائيز_valid = 1;
	} وإلا {
		e->سائيز_valid = 0;
		إذا (oe_get_سائيز_slow(pack, e) != سائيز)
			BUG("'size' is supposed to be the object size!");
	}
}

ساكن مضمن فراغ oe_سيٽ_delta_سائيز(بنية packing_معلومات *pack,
				     بنية object_داخلا *e,
				     سائيز_t سائيز)
{
	إذا (سائيز < pack->oe_delta_سائيز_حد_بندي) {
		e->delta_سائيز = سائيز;
		e->delta_سائيز_valid = 1;
	} وإلا {
		packing_معلومات_تالو(pack);
		إذا (!pack->delta_سائيز_2)
			ALLOC_ARRAY(pack->delta_سائيز_2, pack->nr_alloc);
		packing_معلومات_unlock(pack);

		pack->delta_سائيز_2[e - pack->objects] = سائيز;
		e->delta_سائيز_valid = 0;
	}
}

#وصف_ڪريو اندريون_PACK(obj) oe_اندريون_pack(&منزل_pack, obj)
#وصف_ڪريو سائيز_3(obj) oe_سائيز(&منزل_pack, obj)
#وصف_ڪريو سيٽ_سائيز(obj,سائيز) oe_سيٽ_سائيز(&منزل_pack, obj, سائيز)
#وصف_ڪريو DELTA_سائيز(obj) oe_delta_سائيز(&منزل_pack, obj)
#وصف_ڪريو DELTA(obj) oe_delta(&منزل_pack, obj)
#وصف_ڪريو DELTA_ٻار(obj) oe_delta_ٻار(&منزل_pack, obj)
#وصف_ڪريو DELTA_SIBLING(obj) oe_delta_sibling(&منزل_pack, obj)
#وصف_ڪريو سيٽ_DELTA(obj, val) oe_سيٽ_delta(&منزل_pack, obj, val)
#وصف_ڪريو سيٽ_DELTA_EXT(obj, oid) oe_سيٽ_delta_ext(&منزل_pack, obj, oid)
#وصف_ڪريو سيٽ_DELTA_سائيز(obj, val) oe_سيٽ_delta_سائيز(&منزل_pack, obj, val)
#وصف_ڪريو سيٽ_DELTA_ٻار(obj, val) oe_سيٽ_delta_ٻار(&منزل_pack, obj, val)
#وصف_ڪريو سيٽ_DELTA_SIBLING(obj, val) oe_سيٽ_delta_sibling(&منزل_pack, obj, val)

ساكن ثابت حرف *ثابت pack_usage[] = {
	N_("git pack-objects [-q | --progress | --all-progress] [--all-progress-implied]\n"
	   "                 [--no-reuse-delta] [--delta-base-offset] [--non-empty]\n"
	   "                 [--local] [--incremental] [--window=<n>] [--depth=<n>]\n"
	   "                 [--revs [--unpacked | --all]] [--keep-pack=<pack-name>]\n"
	   "                 [--cruft] [--cruft-expiration=<time>]\n"
	   "                 [--stdout [--filter=<filter-spec>] | <base-name>]\n"
	   "                 [--shallow] [--keep-true-parents] [--[no-]sparse]\n"
	   "                 [--name-hash-version=<n>] [--path-walk] < <object-list>"),
	عدم
};

ساكن بنية pack_idx_داخلا **written_list;
ساكن uint32_t nr_نتيجو, nr_written, nr_ڏٺل;
ساكن بنية bitmap_اشاريو *bitmap_git;
ساكن uint32_t write_layer;

ساكن صحيح non_خالي_جايون;
ساكن صحيح reuse_delta = 1, reuse_object = 1;
ساكن صحيح keep_unreachable, unpack_unreachable, include_ٽيگ;
ساكن timestamp_t unpack_unreachable_expiration;
ساكن صحيح pack_loose_unreachable;
ساكن صحيح cruft;
ساكن صحيح shallow = 0;
ساكن timestamp_t cruft_expiration;
ساكن صحيح local;
ساكن صحيح have_non_local_packs;
ساكن صحيح incremental;
ساكن صحيح ignore_packed_keep_on_disk;
ساكن صحيح ignore_packed_keep_اندريون_core;
ساكن صحيح ignore_packed_keep_اندريون_core_open;
ساكن صحيح ignore_packed_keep_اندريون_core_has_cruft;
ساكن صحيح allow_ofs_delta;
ساكن بنية pack_idx_option pack_idx_opts;
ساكن ثابت حرف *بنياد_name;
ساكن صحيح ترقي = 1;
ساكن صحيح دري = 10;
ساكن بنانشان ڊگهو pack_سائيز_حد_بندي;
ساكن صحيح اونهائي = 50;
ساكن صحيح delta_ڳولا_ڌاڳا;
ساكن صحيح pack_منزل_stdout;
ساكن صحيح sparse;
ساكن صحيح thin;
ساكن صحيح رستو_walk = -1;
ساكن صحيح انگ_preferred_بنياد;
ساكن بنية ترقي *ترقي_state;

ساكن بنية bitmapped_pack *reuse_packfiles;
ساكن سائيز_t reuse_packfiles_nr;
ساكن سائيز_t reuse_packfiles_استعمال_ٿيل_nr;
ساكن uint32_t reuse_packfile_objects;
ساكن بنية bitmap *reuse_packfile_bitmap;

ساكن صحيح use_bitmap_اشاريو_default = 1;
ساكن صحيح use_bitmap_اشاريو = -1;
ساكن تعداد {
	NO_PACK_REUSE = 0,
	SINGLE_PACK_REUSE,
	MULTI_PACK_REUSE,
} allow_pack_reuse = SINGLE_PACK_REUSE;
ساكن تعداد {
	WRITE_BITMAP_FALSE = 0,
	WRITE_BITMAP_QUIET,
	WRITE_BITMAP_TRUE,
} write_bitmap_اشاريو;
ساكن uint16_t write_bitmap_options = BITMAP_OPT_HASH_CACHE;

ساكن صحيح exclude_promisor_objects;
ساكن صحيح exclude_promisor_objects_بهترين_effort;

ساكن صحيح use_delta_islands;

ساكن سائيز_t delta_cache_سائيز = 0;
ساكن سائيز_t max_delta_cache_سائيز = DEFAULT_DELTA_CACHE_سائيز;
ساكن بنانشان ڊگهو cache_max_small_delta_سائيز = 1000;

ساكن بنانشان ڊگهو دري_يادگيري_حد_بندي = 0;

ساكن بنية string_list uri_protocols = STRING_LIST_INIT_NODUP;

تعداد missing_action {
	MA_ERROR = 0,      /* fail if any missing objects are encountered */
	MA_ALLOW_ANY,      /* silently allow ALL missing objects */
	MA_ALLOW_PROMISOR, /* silently allow all missing PROMISOR objects */
};
ساكن تعداد missing_action arg_missing_action;
ساكن show_object_fn fn_show_object;

بنية configured_exclusion {
	بنية oidmap_داخلا e;
	حرف *pack_hash_hex;
	حرف *uri;
};
ساكن بنية oidmap configured_exclusions;

ساكن بنية oidset excluded_by_config;
ساكن صحيح name_hash_نسخو = -1;

تعداد stdin_packs_گهڻائي {
	STDIN_PACKS_گهڻائي_NONE,
	STDIN_PACKS_گهڻائي_STANDARD,
	STDIN_PACKS_گهڻائي_FOLLOW,
};

/**
 * Check whether the name_hash_version chosen by user input is appropriate,
 * and also validate whether it is compatible with other features.
 */
ساكن فراغ validate_name_hash_نسخو(فراغ)
{
	إذا (name_hash_نسخو < 1 || name_hash_نسخو > 2)
		die(_("invalid --name-hash-version option: %d"), name_hash_نسخو);
	إذا (write_bitmap_اشاريو && name_hash_نسخو != 1) {
		warning(_("currently, --write-bitmap-index requires --name-hash-version=1"));
		name_hash_نسخو = 1;
	}
}

ساكن مضمن uint32_t pack_name_hash_fn(ثابت حرف *name)
{
	ساكن صحيح ڏٺل_نسخو = -1;

	إذا (ڏٺل_نسخو < 0)
		ڏٺل_نسخو = name_hash_نسخو;
	وإلا إذا (ڏٺل_نسخو != name_hash_نسخو)
		BUG("name hash version changed from %d to %d mid-process",
		    ڏٺل_نسخو, name_hash_نسخو);

	بدل (name_hash_نسخو) {
	حالة 1:
		أرجع pack_name_hash(name);

	حالة 2:
		أرجع pack_name_hash_v2((ثابت بنانشان حرف *)name);

	افتراضي:
		BUG("invalid name-hash version: %d", name_hash_نسخو);
	}
}

/*
 * stats
 */
ساكن uint32_t written, written_delta;
ساكن uint32_t reused, reused_delta;

/*
 * Indexed commits
 */
ساكن بنية commit **اشاريي_وارو_commits;
ساكن بنانشان صحيح اشاريي_وارو_commits_nr;
ساكن بنانشان صحيح اشاريي_وارو_commits_alloc;

ساكن فراغ اشاريو_commit_for_bitmap(بنية commit *commit)
{
	إذا (اشاريي_وارو_commits_nr >= اشاريي_وارو_commits_alloc) {
		اشاريي_وارو_commits_alloc = (اشاريي_وارو_commits_alloc + 32) * 2;
		REALLOC_ARRAY(اشاريي_وارو_commits, اشاريي_وارو_commits_alloc);
	}

	اشاريي_وارو_commits[اشاريي_وارو_commits_nr++] = commit;
}

ساكن فراغ *get_delta(بنية object_داخلا *داخلا)
{
	بنانشان ڊگهو سائيز, بنياد_سائيز;
	سائيز_t delta_سائيز_2;
	فراغ *buf, *بنياد_buf, *delta_buf;
	تعداد object_type type;
	سائيز_t سائيز_st = 0, بنياد_سائيز_st = 0;

	buf = odb_read_object(the_repository->objects, &داخلا->idx.oid,
			      &type, &سائيز_st);
	سائيز = cast_سائيز_t_منزل_ulong(سائيز_st);
	إذا (!buf)
		die(_("unable to read %s"), oid_منزل_hex(&داخلا->idx.oid));
	بنياد_buf = odb_read_object(the_repository->objects,
				   &DELTA(داخلا)->idx.oid, &type,
				   &بنياد_سائيز_st);
	بنياد_سائيز = cast_سائيز_t_منزل_ulong(بنياد_سائيز_st);
	إذا (!بنياد_buf)
		die("unable to read %s",
		    oid_منزل_hex(&DELTA(داخلا)->idx.oid));
	delta_buf = diff_delta(بنياد_buf, بنياد_سائيز,
			       buf, سائيز, &delta_سائيز_2, 0);
	/*
	 * We successfully computed this delta once but dropped it for
	 * memory reasons. Something is very wrong if this time we
	 * recompute and create a different delta.
	 */
	إذا (!delta_buf || delta_سائيز_2 != DELTA_سائيز(داخلا))
		BUG("delta size changed");
	حرر(buf);
	حرر(بنياد_buf);
	أرجع delta_buf;
}

ساكن بنانشان ڊگهو do_compress(فراغ **pptr, بنانشان ڊگهو سائيز)
{
	git_zstream وهڪرو;
	فراغ *اندريون, *ٻاهريون;
	بنانشان ڊگهو maxsize;
	بنية repo_config_قدرون *cfg = repo_config_قدرون(the_repository);

	git_deflate_init(&وهڪرو, cfg->pack_compression_level);
	maxsize = git_deflate_bound(&وهڪرو, سائيز);

	اندريون = *pptr;
	ٻاهريون = xmalloc(maxsize);
	*pptr = ٻاهريون;

	وهڪرو.ايندڙ_اندريون = اندريون;
	وهڪرو.avail_اندريون = سائيز;
	وهڪرو.ايندڙ_ٻاهريون = ٻاهريون;
	وهڪرو.avail_ٻاهريون = maxsize;
	طالما (git_deflate(&وهڪرو, Z_پڄاڻي) == Z_OK)
		; /* nothing */
	git_deflate_پڇاڙي(&وهڪرو);

	حرر(اندريون);
	أرجع وهڪرو.ڪل_ٻاهريون;
}

ساكن بنانشان ڊگهو write_large_blob_معلومات(بنية odb_وهڪرو *st, بنية hashfile *f,
					   ثابت بنية object_id *oid)
{
	git_zstream وهڪرو;
	بنانشان حرف ibuf[1024 * 16];
	بنانشان حرف obuf[1024 * 16];
	بنانشان ڊگهو olen = 0;
	بنية repo_config_قدرون *cfg = repo_config_قدرون(the_repository);

	git_deflate_init(&وهڪرو, cfg->pack_compression_level);

	لكل (;;) {
		ssize_t readlen;
		صحيح zret = Z_OK;
		readlen = odb_وهڪرو_read(st, ibuf, حجم(ibuf));
		إذا (readlen == -1)
			die(_("unable to read %s"), oid_منزل_hex(oid));

		وهڪرو.ايندڙ_اندريون = ibuf;
		وهڪرو.avail_اندريون = readlen;
		طالما ((وهڪرو.avail_اندريون || readlen == 0) &&
		       (zret == Z_OK || zret == Z_BUF_ERROR)) {
			وهڪرو.ايندڙ_ٻاهريون = obuf;
			وهڪرو.avail_ٻاهريون = حجم(obuf);
			zret = git_deflate(&وهڪرو, readlen ? 0 : Z_پڄاڻي);
			hashwrite(f, obuf, وهڪرو.ايندڙ_ٻاهريون - obuf);
			olen += وهڪرو.ايندڙ_ٻاهريون - obuf;
		}
		إذا (وهڪرو.avail_اندريون)
			die(_("deflate error (%d)"), zret);
		إذا (readlen == 0) {
			إذا (zret != Z_وهڪرو_پڇاڙي)
				die(_("deflate error (%d)"), zret);
			اكسر;
		}
	}
	git_deflate_پڇاڙي(&وهڪرو);
	أرجع olen;
}

/*
 * we are going to reuse the existing object data as is.  make
 * sure it is not corrupt.
 */
ساكن صحيح check_pack_inflate(بنية packed_git *p,
		بنية pack_دري **w_curs,
		off_t وٿي,
		off_t len,
		سائيز_t expect)
{
	git_zstream وهڪرو;
	بنانشان حرف fakebuf[4096], *اندريون;
	صحيح st;

	memset(&وهڪرو, 0, حجم(وهڪرو));
	git_inflate_init(&وهڪرو);
	افعل {
		اندريون = use_pack(p, w_curs, وٿي, &وهڪرو.avail_اندريون);
		وهڪرو.ايندڙ_اندريون = اندريون;
		وهڪرو.ايندڙ_ٻاهريون = fakebuf;
		وهڪرو.avail_ٻاهريون = حجم(fakebuf);
		st = git_inflate(&وهڪرو, Z_پڄاڻي);
		وٿي += وهڪرو.ايندڙ_اندريون - اندريون;
	} طالما (st == Z_OK || st == Z_BUF_ERROR);
	git_inflate_پڇاڙي(&وهڪرو);
	أرجع (st == Z_وهڪرو_پڇاڙي &&
		وهڪرو.ڪل_ٻاهريون == expect &&
		وهڪرو.ڪل_اندريون == len) ? 0 : -1;
}

ساكن فراغ copy_pack_معلومات(بنية hashfile *f,
		بنية packed_git *p,
		بنية pack_دري **w_curs,
		off_t وٿي,
		off_t len)
{
	بنانشان حرف *اندريون;
	سائيز_t avail;

	طالما (len) {
		اندريون = use_pack(p, w_curs, وٿي, &avail);
		إذا (avail > len)
			avail = (بنانشان ڊگهو)len;
		hashwrite(f, اندريون, avail);
		وٿي += avail;
		len -= avail;
	}
}

ساكن مضمن صحيح oe_سائيز_greater_than(بنية packing_معلومات *pack,
				       ثابت بنية object_داخلا *lhs,
				       سائيز_t rhs)
{
	إذا (lhs->سائيز_valid)
		أرجع lhs->سائيز_2 > rhs;
	إذا (rhs < pack->oe_سائيز_حد_بندي) /* rhs < 2^x <= lhs ? */
		أرجع 1;
	أرجع oe_get_سائيز_slow(pack, lhs) > rhs;
}

/* Return 0 if we will bust the pack-size limit */
ساكن بنانشان ڊگهو write_no_reuse_object(بنية hashfile *f, بنية object_داخلا *داخلا,
					   بنانشان ڊگهو حد_بندي, صحيح usable_delta)
{
	بنانشان ڊگهو سائيز, datalen;
	بنانشان حرف سرو[MAX_PACK_OBJECT_سرو],
		      dheader[MAX_PACK_OBJECT_سرو];
	بنانشان hdrlen;
	تعداد object_type type;
	فراغ *buf;
	بنية odb_وهڪرو *st = عدم;
	ثابت بنانشان hashsz = the_hash_algo->rawsz;

	إذا (!usable_delta) {
		إذا (oe_type(داخلا) == OBJ_BLOB &&
		    oe_سائيز_greater_than(&منزل_pack, داخلا,
					 repo_settings_get_big_file_حد(the_repository)) &&
		    (st = odb_وهڪرو_from_object(the_repository->objects, &داخلا->idx.oid,
						 عدم)) != عدم) {
			buf = عدم;
			type = st->type;
			سائيز = st->سائيز;
		} وإلا {
			سائيز_t سائيز_st = 0;
			buf = odb_read_object(the_repository->objects,
					      &داخلا->idx.oid, &type,
					      &سائيز_st);
			سائيز = cast_سائيز_t_منزل_ulong(سائيز_st);
			إذا (!buf)
				die(_("unable to read %s"),
				    oid_منزل_hex(&داخلا->idx.oid));
		}
		/*
		 * make sure no cached delta data remains from a
		 * previous attempt before a pack split occurred.
		 */
		FREE_AND_NULL(داخلا->delta_معلومات);
		داخلا->z_delta_سائيز = 0;
	} وإلا إذا (داخلا->delta_معلومات) {
		سائيز = DELTA_سائيز(داخلا);
		buf = داخلا->delta_معلومات;
		داخلا->delta_معلومات = عدم;
		type = (allow_ofs_delta && DELTA(داخلا)->idx.وٿي) ?
			OBJ_OFS_DELTA : OBJ_REF_DELTA;
	} وإلا {
		buf = get_delta(داخلا);
		سائيز = DELTA_سائيز(داخلا);
		type = (allow_ofs_delta && DELTA(داخلا)->idx.وٿي) ?
			OBJ_OFS_DELTA : OBJ_REF_DELTA;
	}

	إذا (st)	/* large blob case, just assume we don't compress well */
		datalen = سائيز;
	وإلا إذا (داخلا->z_delta_سائيز)
		datalen = داخلا->z_delta_سائيز;
	وإلا
		datalen = do_compress(&buf, سائيز);

	/*
	 * The object header is a byte of 'type' followed by zero or
	 * more bytes of length.
	 */
	hdrlen = encode_اندريون_pack_object_سرو(سرو, حجم(سرو),
					      type, سائيز);

	إذا (type == OBJ_OFS_DELTA) {
		/*
		 * Deltas with relative base contain an additional
		 * encoding of the relative offset for the delta
		 * base from this object's position in the pack.
		 */
		off_t ofs = داخلا->idx.وٿي - DELTA(داخلا)->idx.وٿي;
		بنانشان pos = حجم(dheader) - 1;
		dheader[pos] = ofs & 127;
		طالما (ofs >>= 7)
			dheader[--pos] = 128 | (--ofs & 127);
		إذا (حد_بندي && hdrlen + حجم(dheader) - pos + datalen + hashsz >= حد_بندي) {
			إذا (st)
				odb_وهڪرو_close(st);
			حرر(buf);
			أرجع 0;
		}
		hashwrite(f, سرو, hdrlen);
		hashwrite(f, dheader + pos, حجم(dheader) - pos);
		hdrlen += حجم(dheader) - pos;
	} وإلا إذا (type == OBJ_REF_DELTA) {
		/*
		 * Deltas with a base reference contain
		 * additional bytes for the base object ID.
		 */
		إذا (حد_بندي && hdrlen + hashsz + datalen + hashsz >= حد_بندي) {
			إذا (st)
				odb_وهڪرو_close(st);
			حرر(buf);
			أرجع 0;
		}
		hashwrite(f, سرو, hdrlen);
		hashwrite(f, DELTA(داخلا)->idx.oid.hash, hashsz);
		hdrlen += hashsz;
	} وإلا {
		إذا (حد_بندي && hdrlen + datalen + hashsz >= حد_بندي) {
			إذا (st)
				odb_وهڪرو_close(st);
			حرر(buf);
			أرجع 0;
		}
		hashwrite(f, سرو, hdrlen);
	}
	إذا (st) {
		datalen = write_large_blob_معلومات(st, f, &داخلا->idx.oid);
		odb_وهڪرو_close(st);
	} وإلا {
		hashwrite(f, buf, datalen);
		حرر(buf);
	}

	أرجع hdrlen + datalen;
}

/* Return 0 if we will bust the pack-size limit */
ساكن off_t write_reuse_object(بنية hashfile *f, بنية object_داخلا *داخلا,
				بنانشان ڊگهو حد_بندي, صحيح usable_delta)
{
	بنية packed_git *p = اندريون_PACK(داخلا);
	بنية pack_دري *w_curs = عدم;
	uint32_t pos;
	off_t وٿي, cur;
	تعداد object_type type = oe_type(داخلا);
	تعداد object_type اندريون_pack_type;
	off_t datalen;
	بنانشان حرف سرو[MAX_PACK_OBJECT_سرو],
		      dheader[MAX_PACK_OBJECT_سرو];
	بنانشان hdrlen;
	ثابت بنانشان hashsz = the_hash_algo->rawsz;
	سائيز_t داخلا_سائيز;

	cur = داخلا->اندريون_pack_وٿي;
	اندريون_pack_type = unpack_object_سرو(p, &w_curs, &cur, &داخلا_سائيز);
	إذا (اندريون_pack_type < 0)
		die(_("write_reuse_object: unable to parse object header of %s"),
		    oid_منزل_hex(&داخلا->idx.oid));

	إذا (DELTA(داخلا))
		type = (allow_ofs_delta && DELTA(داخلا)->idx.وٿي) ?
			OBJ_OFS_DELTA : OBJ_REF_DELTA;
	hdrlen = encode_اندريون_pack_object_سرو(سرو, حجم(سرو),
					      type, داخلا_سائيز);

	وٿي = داخلا->اندريون_pack_وٿي;
	إذا (وٿي_منزل_pack_pos(p, وٿي, &pos) < 0)
		die(_("write_reuse_object: could not locate %s, expected at "
		      "offset %"PRIuMAX" in pack %s"),
		    oid_منزل_hex(&داخلا->idx.oid), (uintmax_t)وٿي,
		    p->pack_name);
	datalen = pack_pos_منزل_وٿي(p, pos + 1) - وٿي;
	إذا (!pack_منزل_stdout && p->اشاريو_نسخو > 1 &&
	    check_pack_چڪري_جاچ(p, &w_curs, وٿي, datalen,
			   pack_pos_منزل_اشاريو(p, pos))) {
		error(_("bad packed object CRC for %s"),
		      oid_منزل_hex(&داخلا->idx.oid));
		unuse_pack(&w_curs);
		أرجع write_no_reuse_object(f, داخلا, حد_بندي, usable_delta);
	}

	وٿي += داخلا->اندريون_pack_سرو_سائيز;
	datalen -= داخلا->اندريون_pack_سرو_سائيز;

	إذا (!pack_منزل_stdout && p->اشاريو_نسخو == 1 &&
	    check_pack_inflate(p, &w_curs, وٿي, datalen, داخلا_سائيز)) {
		error(_("corrupt packed object for %s"),
		      oid_منزل_hex(&داخلا->idx.oid));
		unuse_pack(&w_curs);
		أرجع write_no_reuse_object(f, داخلا, حد_بندي, usable_delta);
	}

	إذا (type == OBJ_OFS_DELTA) {
		off_t ofs = داخلا->idx.وٿي - DELTA(داخلا)->idx.وٿي;
		بنانشان pos = حجم(dheader) - 1;
		dheader[pos] = ofs & 127;
		طالما (ofs >>= 7)
			dheader[--pos] = 128 | (--ofs & 127);
		إذا (حد_بندي && hdrlen + حجم(dheader) - pos + datalen + hashsz >= حد_بندي) {
			unuse_pack(&w_curs);
			أرجع 0;
		}
		hashwrite(f, سرو, hdrlen);
		hashwrite(f, dheader + pos, حجم(dheader) - pos);
		hdrlen += حجم(dheader) - pos;
		reused_delta++;
	} وإلا إذا (type == OBJ_REF_DELTA) {
		إذا (حد_بندي && hdrlen + hashsz + datalen + hashsz >= حد_بندي) {
			unuse_pack(&w_curs);
			أرجع 0;
		}
		hashwrite(f, سرو, hdrlen);
		hashwrite(f, DELTA(داخلا)->idx.oid.hash, hashsz);
		hdrlen += hashsz;
		reused_delta++;
	} وإلا {
		إذا (حد_بندي && hdrlen + datalen + hashsz >= حد_بندي) {
			unuse_pack(&w_curs);
			أرجع 0;
		}
		hashwrite(f, سرو, hdrlen);
	}
	copy_pack_معلومات(f, p, &w_curs, وٿي, datalen);
	unuse_pack(&w_curs);
	reused++;
	أرجع hdrlen + datalen;
}

/* Return 0 if we will bust the pack-size limit */
ساكن off_t write_object(بنية hashfile *f,
			  بنية object_داخلا *داخلا,
			  off_t write_وٿي)
{
	بنانشان ڊگهو حد_بندي;
	off_t len;
	صحيح usable_delta, منزل_reuse;

	إذا (!pack_منزل_stdout)
		چڪري_جاچ_32_begin(f);

	/* apply size limit if limited packsize and not first object */
	إذا (!pack_سائيز_حد_بندي || !nr_written)
		حد_بندي = 0;
	وإلا إذا (pack_سائيز_حد_بندي <= write_وٿي)
		/*
		 * the earlier object did not fit the limit; avoid
		 * mistaking this with unlimited (i.e. limit = 0).
		 */
		حد_بندي = 1;
	وإلا
		حد_بندي = pack_سائيز_حد_بندي - write_وٿي;

	إذا (!DELTA(داخلا))
		usable_delta = 0;	/* no delta */
	وإلا إذا (!pack_سائيز_حد_بندي)
	       usable_delta = 1;	/* unlimited packfile */
	وإلا إذا (DELTA(داخلا)->idx.وٿي == (off_t)-1)
		usable_delta = 0;	/* base was written to another pack */
	وإلا إذا (DELTA(داخلا)->idx.وٿي)
		usable_delta = 1;	/* base already exists in this pack */
	وإلا
		usable_delta = 0;	/* base could end up in another pack */

	إذا (!reuse_object)
		منزل_reuse = 0;	/* explicit */
	وإلا إذا (!اندريون_PACK(داخلا))
		منزل_reuse = 0;	/* can't reuse what we don't have */
	وإلا إذا (oe_type(داخلا) == OBJ_REF_DELTA ||
		 oe_type(داخلا) == OBJ_OFS_DELTA)
				/* check_object() decided it for us ... */
		منزل_reuse = usable_delta;
				/* ... but pack split may override that */
	وإلا إذا (oe_type(داخلا) != داخلا->اندريون_pack_type)
		منزل_reuse = 0;	/* pack has delta which is unusable */
	وإلا إذا (DELTA(داخلا))
		منزل_reuse = 0;	/* we want to pack afresh */
	وإلا
		منزل_reuse = 1;	/* we have it in-pack undeltified,
				 * and we do not need to deltify it.
				 */

	إذا (!منزل_reuse)
		len = write_no_reuse_object(f, داخلا, حد_بندي, usable_delta);
	وإلا
		len = write_reuse_object(f, داخلا, حد_بندي, usable_delta);
	إذا (!len)
		أرجع 0;

	إذا (usable_delta)
		written_delta++;
	written++;
	إذا (!pack_منزل_stdout)
		داخلا->idx.چڪري_جاچ_32 = چڪري_جاچ_32_پڇاڙي(f);
	أرجع len;
}

تعداد write_one_حالت {
	WRITE_ONE_SKIP = -1, /* already written */
	WRITE_ONE_BREAK = 0, /* writing this will bust the limit; not written */
	WRITE_ONE_WRITTEN = 1, /*عام*/
	WRITE_ONE_RECURSIVE = 2 /* already scheduled to be written */
};

ساكن تعداد write_one_حالت write_one(بنية hashfile *f,
				       بنية object_داخلا *e,
				       off_t *وٿي)
{
	off_t سائيز;
	صحيح recursing;

	/*
	 * we set offset to 1 (which is an impossible value) to mark
	 * the fact that this object is involved in "write its base
	 * first before writing a deltified object" recursion.
	 */
	recursing = (e->idx.وٿي == 1);
	إذا (recursing) {
		warning(_("recursive delta detected for object %s"),
			oid_منزل_hex(&e->idx.oid));
		أرجع WRITE_ONE_RECURSIVE;
	} وإلا إذا (e->idx.وٿي || e->preferred_بنياد) {
		/* offset is non zero if object is written already. */
		أرجع WRITE_ONE_SKIP;
	}

	/* if we are deltified, write out base object first. */
	إذا (DELTA(e)) {
		e->idx.وٿي = 1; /* now recurse */
		بدل (write_one(f, DELTA(e), وٿي)) {
		حالة WRITE_ONE_RECURSIVE:
			/* we cannot depend on this one */
			سيٽ_DELTA(e, عدم);
			اكسر;
		افتراضي:
			اكسر;
		حالة WRITE_ONE_BREAK:
			e->idx.وٿي = recursing;
			أرجع WRITE_ONE_BREAK;
		}
	}

	e->idx.وٿي = *وٿي;
	سائيز = write_object(f, e, *وٿي);
	إذا (!سائيز) {
		e->idx.وٿي = recursing;
		أرجع WRITE_ONE_BREAK;
	}
	written_list[nr_written++] = &e->idx;

	/* make sure off_t is sufficiently large not to wrap */
	إذا (signed_جوڙيو_overflows(*وٿي, سائيز))
		die(_("pack too large for current definition of off_t"));
	*وٿي += سائيز;
	أرجع WRITE_ONE_WRITTEN;
}

ساكن صحيح mark_tagged(ثابت بنية reference *ref, فراغ *cb_معلومات اڻ_استعمال_ٿيل)
{
	بنية object_id peeled;
	بنية object_داخلا *داخلا = packlist_ڳوليو(&منزل_pack, ref->oid);

	إذا (داخلا)
		داخلا->tagged = 1;
	إذا (!reference_get_peeled_oid(the_repository, ref, &peeled)) {
		داخلا = packlist_ڳوليو(&منزل_pack, &peeled);
		إذا (داخلا)
			داخلا->tagged = 1;
	}
	أرجع 0;
}

ساكن مضمن بنانشان حرف oe_layer(بنية packing_معلومات *pack,
				     بنية object_داخلا *e)
{
	إذا (!pack->layer)
		أرجع 0;
	أرجع pack->layer[e - pack->objects];
}

ساكن مضمن فراغ جوڙيو_منزل_write_ترتيب(بنية object_داخلا **wo,
			       بنانشان صحيح *endp,
			       بنية object_داخلا *e)
{
	إذا (e->filled || oe_layer(&منزل_pack, e) != write_layer)
		أرجع;
	wo[(*endp)++] = e;
	e->filled = 1;
}

ساكن فراغ جوڙيو_descendants_منزل_write_ترتيب(بنية object_داخلا **wo,
					   بنانشان صحيح *endp,
					   بنية object_داخلا *e)
{
	صحيح جوڙيو_منزل_ترتيب = 1;
	طالما (e) {
		إذا (جوڙيو_منزل_ترتيب) {
			بنية object_داخلا *s;
			/* add this node... */
			جوڙيو_منزل_write_ترتيب(wo, endp, e);
			/* all its siblings... */
			لكل (s = DELTA_SIBLING(e); s; s = DELTA_SIBLING(s)) {
				جوڙيو_منزل_write_ترتيب(wo, endp, s);
			}
		}
		/* drop down a level to add left subtree nodes if possible */
		إذا (DELTA_ٻار(e)) {
			جوڙيو_منزل_ترتيب = 1;
			e = DELTA_ٻار(e);
		} وإلا {
			جوڙيو_منزل_ترتيب = 0;
			/* our sibling might have some children, it is next */
			إذا (DELTA_SIBLING(e)) {
				e = DELTA_SIBLING(e);
				استمر;
			}
			/* go back to our parent node */
			e = DELTA(e);
			طالما (e && !DELTA_SIBLING(e)) {
				/* we're on the right side of a subtree, keep
				 * going up until we can go right again */
				e = DELTA(e);
			}
			إذا (!e) {
				/* done- we hit our original root node */
				أرجع;
			}
			/* pass it off to sibling at this level */
			e = DELTA_SIBLING(e);
		}
	};
}

ساكن فراغ جوڙيو_family_منزل_write_ترتيب(بنية object_داخلا **wo,
				      بنانشان صحيح *endp,
				      بنية object_داخلا *e)
{
	بنية object_داخلا *پاڙ;

	لكل (پاڙ = e; DELTA(پاڙ); پاڙ = DELTA(پاڙ))
		; /* nothing */
	جوڙيو_descendants_منزل_write_ترتيب(wo, endp, پاڙ);
}

ساكن فراغ compute_layer_ترتيب(بنية object_داخلا **wo, بنانشان صحيح *wo_پڇاڙي)
{
	بنانشان صحيح i, آخري_untagged;
	بنية object_داخلا *objects = منزل_pack.objects;

	لكل (i = 0; i < منزل_pack.nr_objects; i++) {
		إذا (objects[i].tagged)
			اكسر;
		جوڙيو_منزل_write_ترتيب(wo, wo_پڇاڙي, &objects[i]);
	}
	آخري_untagged = i;

	/*
	 * Then fill all the tagged tips.
	 */
	لكل (; i < منزل_pack.nr_objects; i++) {
		إذا (objects[i].tagged)
			جوڙيو_منزل_write_ترتيب(wo, wo_پڇاڙي, &objects[i]);
	}

	/*
	 * And then all remaining commits and tags.
	 */
	لكل (i = آخري_untagged; i < منزل_pack.nr_objects; i++) {
		إذا (oe_type(&objects[i]) != OBJ_COMMIT &&
		    oe_type(&objects[i]) != OBJ_ٽيگ)
			استمر;
		جوڙيو_منزل_write_ترتيب(wo, wo_پڇاڙي, &objects[i]);
	}

	/*
	 * And then all the trees.
	 */
	لكل (i = آخري_untagged; i < منزل_pack.nr_objects; i++) {
		إذا (oe_type(&objects[i]) != OBJ_وڻ)
			استمر;
		جوڙيو_منزل_write_ترتيب(wo, wo_پڇاڙي, &objects[i]);
	}

	/*
	 * Finally all the rest in really tight order
	 */
	لكل (i = آخري_untagged; i < منزل_pack.nr_objects; i++) {
		إذا (!objects[i].filled && oe_layer(&منزل_pack, &objects[i]) == write_layer)
			جوڙيو_family_منزل_write_ترتيب(wo, wo_پڇاڙي, &objects[i]);
	}
}

ساكن بنية object_داخلا **compute_write_ترتيب(فراغ)
{
	uint32_t max_layers = 1;
	بنانشان صحيح i, wo_پڇاڙي;

	بنية object_داخلا **wo;
	بنية object_داخلا *objects = منزل_pack.objects;

	لكل (i = 0; i < منزل_pack.nr_objects; i++) {
		objects[i].tagged = 0;
		objects[i].filled = 0;
		سيٽ_DELTA_ٻار(&objects[i], عدم);
		سيٽ_DELTA_SIBLING(&objects[i], عدم);
	}

	/*
	 * Fully connect delta_child/delta_sibling network.
	 * Make sure delta_sibling is sorted in the original
	 * recency order.
	 */
	لكل (i = منزل_pack.nr_objects; i > 0;) {
		بنية object_داخلا *e = &objects[--i];
		إذا (!DELTA(e))
			استمر;
		/* Mark me as the first child */
		e->delta_sibling_idx = DELTA(e)->delta_ٻار_idx;
		سيٽ_DELTA_ٻار(DELTA(e), e);
	}

	/*
	 * Mark objects that are at the tip of tags.
	 */
	refs_for_each_ٽيگ_ref(get_main_ref_store(the_repository), mark_tagged,
			      عدم);

	إذا (use_delta_islands) {
		max_layers = compute_pack_layers(&منزل_pack);
		free_island_marks();
	}

	ALLOC_ARRAY(wo, منزل_pack.nr_objects);
	wo_پڇاڙي = 0;

	لكل (; write_layer < max_layers; ++write_layer)
		compute_layer_ترتيب(wo, &wo_پڇاڙي);

	إذا (wo_پڇاڙي != منزل_pack.nr_objects)
		die(_("ordered %u objects, expected %"PRIu32),
		    wo_پڇاڙي, منزل_pack.nr_objects);

	أرجع wo;
}


/*
 * A reused set of objects. All objects in a chunk have the same
 * relative position in the original packfile and the generated
 * packfile.
 */

ساكن بنية reused_chunk {
	/* The offset of the first object of this chunk in the original
	 * packfile. */
	off_t original;
	/* The difference for "original" minus the offset of the first object of
	 * this chunk in the generated packfile. */
	off_t difference;
} *reused_chunks;
ساكن صحيح reused_chunks_nr;
ساكن صحيح reused_chunks_alloc;

ساكن فراغ رڪارڊ_reused_object(off_t where, off_t وٿي)
{
	إذا (reused_chunks_nr && reused_chunks[reused_chunks_nr-1].difference == وٿي)
		أرجع;

	ALLOC_GROW(reused_chunks, reused_chunks_nr + 1,
		   reused_chunks_alloc);
	reused_chunks[reused_chunks_nr].original = where;
	reused_chunks[reused_chunks_nr].difference = وٿي;
	reused_chunks_nr++;
}

/*
 * Binary search to find the chunk that "where" is in. Note
 * that we're not looking for an exact match, just the first
 * chunk that contains it (which implicitly ends at the start
 * of the next chunk.
 */
ساكن off_t ڳوليو_reused_وٿي(off_t where)
{
	صحيح lo = 0, hi = reused_chunks_nr;
	طالما (lo < hi) {
		صحيح mi = lo + ((hi - lo) / 2);
		إذا (where == reused_chunks[mi].original)
			أرجع reused_chunks[mi].difference;
		إذا (where < reused_chunks[mi].original)
			hi = mi;
		وإلا
			lo = mi + 1;
	}

	/*
	 * The first chunk starts at zero, so we can't have gone below
	 * there.
	 */
	أكد(lo);
	أرجع reused_chunks[lo-1].difference;
}

ساكن فراغ write_reused_pack_one(بنية packed_git *reuse_packfile,
				  سائيز_t pos, بنية hashfile *ٻاهريون,
				  off_t pack_شروعات,
				  بنية pack_دري **w_curs)
{
	off_t وٿي, ايندڙ, cur;
	تعداد object_type type;
	سائيز_t سائيز;

	وٿي = pack_pos_منزل_وٿي(reuse_packfile, pos);
	ايندڙ = pack_pos_منزل_وٿي(reuse_packfile, pos + 1);

	رڪارڊ_reused_object(وٿي,
			     وٿي - (hashfile_ڪل(ٻاهريون) - pack_شروعات));

	cur = وٿي;
	type = unpack_object_سرو(reuse_packfile, w_curs, &cur, &سائيز);
	أكد(type >= 0);

	إذا (type == OBJ_OFS_DELTA) {
		off_t بنياد_وٿي;
		off_t fixup;

		بنانشان حرف سرو[MAX_PACK_OBJECT_سرو];
		بنانشان len;

		بنياد_وٿي = get_delta_بنياد(reuse_packfile, w_curs, &cur, type, وٿي);
		أكد(بنياد_وٿي != 0);

		/* Convert to REF_DELTA if we must... */
		إذا (!allow_ofs_delta) {
			uint32_t بنياد_pos;
			بنية object_id بنياد_oid;

			إذا (وٿي_منزل_pack_pos(reuse_packfile, بنياد_وٿي, &بنياد_pos) < 0)
				die(_("expected object at offset %"PRIuMAX" "
				      "in pack %s"),
				    (uintmax_t)بنياد_وٿي,
				    reuse_packfile->pack_name);

			nth_packed_object_id(&بنياد_oid, reuse_packfile,
					     pack_pos_منزل_اشاريو(reuse_packfile, بنياد_pos));

			len = encode_اندريون_pack_object_سرو(سرو, حجم(سرو),
							   OBJ_REF_DELTA, سائيز);
			hashwrite(ٻاهريون, سرو, len);
			hashwrite(ٻاهريون, بنياد_oid.hash, the_hash_algo->rawsz);
			copy_pack_معلومات(ٻاهريون, reuse_packfile, w_curs, cur, ايندڙ - cur);
			أرجع;
		}

		/* Otherwise see if we need to rewrite the offset... */
		fixup = ڳوليو_reused_وٿي(وٿي) -
			ڳوليو_reused_وٿي(بنياد_وٿي);
		إذا (fixup) {
			بنانشان حرف ofs_سرو[MAX_PACK_OBJECT_سرو];
			بنانشان i, ofs_len;
			off_t ofs = وٿي - بنياد_وٿي - fixup;

			len = encode_اندريون_pack_object_سرو(سرو, حجم(سرو),
							   OBJ_OFS_DELTA, سائيز);

			i = حجم(ofs_سرو) - 1;
			ofs_سرو[i] = ofs & 127;
			طالما (ofs >>= 7)
				ofs_سرو[--i] = 128 | (--ofs & 127);

			ofs_len = حجم(ofs_سرو) - i;

			hashwrite(ٻاهريون, سرو, len);
			hashwrite(ٻاهريون, ofs_سرو + حجم(ofs_سرو) - ofs_len, ofs_len);
			copy_pack_معلومات(ٻاهريون, reuse_packfile, w_curs, cur, ايندڙ - cur);
			أرجع;
		}

		/* ...otherwise we have no fixup, and can write it verbatim */
	}

	copy_pack_معلومات(ٻاهريون, reuse_packfile, w_curs, وٿي, ايندڙ - وٿي);
}

ساكن سائيز_t write_reused_pack_verbatim(بنية bitmapped_pack *reuse_packfile,
					 بنية hashfile *ٻاهريون,
					 بنية pack_دري **w_curs)
{
	سائيز_t pos = 0;
	سائيز_t پڇاڙي;

	إذا (reuse_packfile->bitmap_pos) {
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
		أرجع reuse_packfile->bitmap_pos / بٽ_اندريون_EWORD;
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
	پڇاڙي = reuse_packfile->bitmap_nr / بٽ_اندريون_EWORD;
	إذا (reuse_packfile_bitmap->لفظ_alloc < پڇاڙي)
		BUG("fewer words than expected in reuse_packfile_bitmap");

	طالما (pos < پڇاڙي && reuse_packfile_bitmap->لفظ[pos] == (eword_t)~0)
		pos++;

	إذا (pos) {
		off_t منزل_write;

		written = (pos * بٽ_اندريون_EWORD);
		منزل_write = pack_pos_منزل_وٿي(reuse_packfile->p, written)
			- حجم(بنية pack_سرو);

		/* We're recording one chunk, not one object. */
		رڪارڊ_reused_object(حجم(بنية pack_سرو), 0);
		hashflush(ٻاهريون);
		copy_pack_معلومات(ٻاهريون, reuse_packfile->p, w_curs,
			حجم(بنية pack_سرو), منزل_write);

		display_ترقي(ترقي_state, written);
	}
	أرجع pos;
}

ساكن فراغ write_reused_pack(بنية bitmapped_pack *reuse_packfile,
			      بنية hashfile *f)
{
	سائيز_t i = reuse_packfile->bitmap_pos / بٽ_اندريون_EWORD;
	uint32_t وٿي;
	off_t pack_شروعات = hashfile_ڪل(f) - حجم(بنية pack_سرو);
	بنية pack_دري *w_curs = عدم;

	إذا (allow_ofs_delta)
		i = write_reused_pack_verbatim(reuse_packfile, f, &w_curs);

	لكل (; i < reuse_packfile_bitmap->لفظ_alloc; ++i) {
		eword_t لفظ_2 = reuse_packfile_bitmap->لفظ[i];
		سائيز_t pos = (i * بٽ_اندريون_EWORD);

		لكل (وٿي = 0; وٿي < بٽ_اندريون_EWORD; ++وٿي) {
			uint32_t pack_pos;
			إذا ((لفظ_2 >> وٿي) == 0)
				اكسر;

			وٿي += ewah_ٻٽو_انگ_ctz64(لفظ_2 >> وٿي);
			إذا (pos + وٿي < reuse_packfile->bitmap_pos)
				استمر;
			إذا (pos + وٿي >= reuse_packfile->bitmap_pos + reuse_packfile->bitmap_nr)
				اذهب پورو;

			إذا (reuse_packfile->bitmap_pos) {
				/*
				 * When doing multi-pack reuse on a
				 * non-preferred pack, translate bit positions
				 * from the MIDX pseudo-pack order back to their
				 * pack-relative positions before attempting
				 * reuse.
				 */
				بنية multi_pack_اشاريو *m = reuse_packfile->from_midx;
				uint32_t midx_pos;
				off_t pack_ofs;

				إذا (!m)
					BUG("non-zero bitmap position without MIDX");

				midx_pos = pack_pos_منزل_midx(m, pos + وٿي);
				pack_ofs = nth_midxed_وٿي(m, midx_pos);

				إذا (وٿي_منزل_pack_pos(reuse_packfile->p,
						       pack_ofs, &pack_pos) < 0)
					BUG("could not find expected object at offset %"PRIuMAX" in pack %s",
					    (uintmax_t)pack_ofs,
					    pack_basename(reuse_packfile->p));
			} وإلا {
				/*
				 * Can use bit positions directly, even for MIDX
				 * bitmaps. See comment in try_partial_reuse()
				 * for why.
				 */
				pack_pos = pos + وٿي;
			}

			write_reused_pack_one(reuse_packfile->p, pack_pos, f,
					      pack_شروعات, &w_curs);
			display_ترقي(ترقي_state, ++written);
		}
	}

پورو:
	unuse_pack(&w_curs);
}

ساكن فراغ write_excluded_by_configs(فراغ)
{
	بنية oidset_iter iter;
	ثابت بنية object_id *oid;

	oidset_iter_init(&excluded_by_config, &iter);
	طالما ((oid = oidset_iter_ايندڙ(&iter))) {
		بنية configured_exclusion *ex =
			oidmap_get(&configured_exclusions, oid);

		إذا (!ex)
			BUG("configured exclusion wasn't configured");
		write_اندريون_ڀريل(1, ex->pack_hash_hex, لڙي_ڊيگهه(ex->pack_hash_hex));
		write_اندريون_ڀريل(1, " ", 1);
		write_اندريون_ڀريل(1, ex->uri, لڙي_ڊيگهه(ex->uri));
		write_اندريون_ڀريل(1, "\n", 1);
	}
}

ساكن ثابت حرف no_ورهايو_warning[] = N_(
"disabling bitmap writing, packs are split due to pack.packSizeLimit"
);

ساكن فراغ write_pack_file(فراغ)
{
	uint32_t i = 0, j;
	بنية hashfile *f;
	off_t وٿي;
	uint32_t nr_بچيل = nr_نتيجو;
	وقت_t آخري_mtime = 0;
	بنية object_داخلا **write_ترتيب;
	off_t بائيٽون_written = 0;

	إذا (ترقي > pack_منزل_stdout)
		ترقي_state = شروعات_ترقي(the_repository,
						_("Writing objects"), nr_نتيجو);
	ALLOC_ARRAY(written_list, منزل_pack.nr_objects);
	write_ترتيب = compute_write_ترتيب();

	افعل {
		بنانشان حرف hash[GIT_MAX_RAWSZ];
		حرف *pack_tmp_name = عدم;

		إذا (pack_منزل_stdout) {
			/*
			 * This command is most often invoked via
			 * git-upload-pack(1), which will typically chunk data
			 * into pktlines. As such, we use the maximum data
			 * length of them as buffer length.
			 *
			 * Note that we need to subtract one though to
			 * accommodate for the sideband byte.
			 */
			بنية hashfd_options opts = {
				.ترقي = ترقي_state,
				.بفر_len = LARGE_PACKET_معلومات_MAX - 1,
			};
			f = hashfd_ext(the_repository->hash_algo, 1,
				       "<stdout>", &opts);
		} وإلا {
			f = create_tmp_packfile(the_repository, &pack_tmp_name);
		}

		وٿي = write_pack_سرو(f, nr_بچيل);

		إذا (reuse_packfiles_nr) {
			أكد(pack_منزل_stdout);
			لكل (j = 0; j < reuse_packfiles_nr; j++) {
				reused_chunks_nr = 0;
				write_reused_pack(&reuse_packfiles[j], f);
				إذا (reused_chunks_nr)
					reuse_packfiles_استعمال_ٿيل_nr++;
			}
			وٿي = hashfile_ڪل(f);
		}

		nr_written = 0;
		لكل (; i < منزل_pack.nr_objects; i++) {
			بنية object_داخلا *e = write_ترتيب[i];
			إذا (write_one(f, e, &وٿي) == WRITE_ONE_BREAK)
				اكسر;
			display_ترقي(ترقي_state, written);
		}

		بائيٽون_written += hashfile_ڪل(f) +
			the_repository->hash_algo->rawsz;
		إذا (pack_منزل_stdout) {
			/*
			 * We never fsync when writing to stdout since we may
			 * not be writing to an actual pack file. For instance,
			 * the upload-pack code passes a pipe here. Calling
			 * fsync on a pipe results in unnecessary
			 * synchronization with the reader on some platforms.
			 */
			finalize_hashfile(f, hash, FSYNC_COMPONENT_NONE,
					  CSUM_HASH_اندريون_وهڪرو | CSUM_CLOSE);
		} وإلا إذا (nr_written == nr_بچيل) {
			finalize_hashfile(f, hash, FSYNC_COMPONENT_PACK,
					  CSUM_HASH_اندريون_وهڪرو | CSUM_FSYNC | CSUM_CLOSE);
		} وإلا {
			/*
			 * If we wrote the wrong number of entries in the
			 * header, rewrite it like in fast-import.
			 */

			صحيح fd = finalize_hashfile(f, hash, FSYNC_COMPONENT_PACK, 0);
			fixup_pack_سرو_footer(the_hash_algo, fd, hash,
						 pack_tmp_name, nr_written,
						 hash, وٿي);
			close(fd);
			إذا (write_bitmap_اشاريو) {
				إذا (write_bitmap_اشاريو != WRITE_BITMAP_QUIET)
					warning(_(no_ورهايو_warning));
				write_bitmap_اشاريو = 0;
			}
		}

		إذا (!pack_منزل_stdout) {
			بنية stat st;
			بنية strbuf tmpname = STRBUF_INIT;
			بنية bitmap_writer bitmap_writer;
			حرف *idx_tmp_name = عدم;

			/*
			 * Packs are runtime accessed in their mtime
			 * order since newer packs are more likely to contain
			 * younger objects.  So if we are creating multiple
			 * packs then we should modify the mtime of later ones
			 * to preserve this property.
			 */
			إذا (stat(pack_tmp_name, &st) < 0) {
				warning_errno(_("failed to stat %s"), pack_tmp_name);
			} وإلا إذا (!آخري_mtime) {
				آخري_mtime = st.st_mtime;
			} وإلا {
				بنية utimbuf utb;
				utb.actime = st.st_atime;
				utb.modtime = --آخري_mtime;
				إذا (utime(pack_tmp_name, &utb) < 0)
					warning_errno(_("failed utime() on %s"), pack_tmp_name);
			}

			strbuf_addf(&tmpname, "%s-%s.", بنياد_name,
				    hash_منزل_hex(hash));

			إذا (write_bitmap_اشاريو) {
				bitmap_writer_init(&bitmap_writer,
						   the_repository, &منزل_pack,
						   عدم);
				bitmap_writer_سيٽ_checksum(&bitmap_writer, hash);
				bitmap_writer_build_type_اشاريو(&bitmap_writer,
							       written_list);
			}

			إذا (cruft)
				pack_idx_opts.flags |= WRITE_MTIMES;

			stage_tmp_packfiles(the_repository, &tmpname,
					    pack_tmp_name, written_list,
					    nr_written, &منزل_pack,
					    &pack_idx_opts, hash,
					    &idx_tmp_name);

			إذا (write_bitmap_اشاريو) {
				سائيز_t tmpname_len = tmpname.len;

				strbuf_addstr(&tmpname, "bitmap");
				stop_ترقي(&ترقي_state);

				bitmap_writer_show_ترقي(&bitmap_writer,
							    ترقي);
				bitmap_writer_select_commits(&bitmap_writer,
							     اشاريي_وارو_commits,
							     اشاريي_وارو_commits_nr);
				إذا (bitmap_writer_build(&bitmap_writer) < 0)
					die(_("failed to write bitmap index"));
				bitmap_writer_پڄاڻي(&bitmap_writer,
						     written_list,
						     tmpname.buf, write_bitmap_options);
				bitmap_writer_free(&bitmap_writer);
				write_bitmap_اشاريو = 0;
				strbuf_setlen(&tmpname, tmpname_len);
			}

			rename_tmp_packfile_idx(the_repository, &tmpname, &idx_tmp_name);

			حرر(idx_tmp_name);
			strbuf_release(&tmpname);
			حرر(pack_tmp_name);
			puts(hash_منزل_hex(hash));
		}

		/* mark written objects as written to previous pack */
		لكل (j = 0; j < nr_written; j++) {
			written_list[j]->وٿي = (off_t)-1;
		}
		nr_بچيل -= nr_written;
	} طالما (nr_بچيل && i < منزل_pack.nr_objects);

	حرر(written_list);
	حرر(write_ترتيب);
	stop_ترقي(&ترقي_state);
	إذا (written != nr_نتيجو)
		die(_("wrote %"PRIu32" objects while expecting %"PRIu32),
		    written, nr_نتيجو);
	trace2_معلومات_intmax("pack-objects", the_repository,
			   "write_pack_file/wrote", nr_نتيجو);
	trace2_معلومات_intmax("pack-objects", the_repository,
			   "write_pack_file/wrote_bytes", بائيٽون_written);
}

ساكن صحيح no_try_delta(ثابت حرف *رستو)
{
	ساكن بنية attr_check *check;

	إذا (!check)
		check = attr_check_initl("delta", عدم);
	git_check_attr(the_repository->اشاريو, رستو, check);
	إذا (ATTR_FALSE(check->شيون[0].قدر))
		أرجع 1;
	أرجع 0;
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
ساكن صحيح have_duplicate_داخلا(ثابت بنية object_id *oid,
				صحيح exclude)
{
	بنية object_داخلا *داخلا;

	إذا (reuse_packfile_bitmap &&
	    bitmap_walk_شامل_آهي(bitmap_git, reuse_packfile_bitmap, oid))
		أرجع 1;

	داخلا = packlist_ڳوليو(&منزل_pack, oid);
	إذا (!داخلا)
		أرجع 0;

	إذا (exclude) {
		إذا (!داخلا->preferred_بنياد)
			nr_نتيجو--;
		داخلا->preferred_بنياد = 1;
	}

	أرجع 1;
}

ساكن صحيح want_cruft_object_mtime(بنية repository *r,
				   ثابت بنية object_id *oid,
				   بنانشان flags, uint32_t mtime)
{
	بنية odb_ذريعو_files *files = odb_ذريعو_files_downcast(r->objects->ذريعو);

	لكل (بنية odb_files_dir *dir = files->dirs; dir; dir = dir->ايندڙ) {
		بنية packed_git **cache = packfile_store_get_kept_pack_cache(dir->packed, flags);

		لكل (; *cache; cache++) {
			بنية packed_git *p = *cache;
			off_t ofs;
			uint32_t candidate_mtime;

			ofs = ڳوليو_pack_داخلا_one(oid, p);
			إذا (!ofs)
				استمر;

			/*
			 * We have a copy of the object 'oid' in a non-cruft
			 * pack. We can avoid packing an additional copy
			 * regardless of what the existing copy's mtime is since
			 * it is outside of a cruft pack.
			 */
			إذا (!p->is_cruft)
				أرجع 0;

			/*
			 * If we have a copy of the object 'oid' in a cruft
			 * pack, then either read the cruft pack's mtime for
			 * that object, or, if that can't be loaded, assume the
			 * pack's mtime itself.
			 */
			إذا (!ڀريو_pack_mtimes(p)) {
				uint32_t pos;
				إذا (وٿي_منزل_pack_pos(p, ofs, &pos) < 0)
					استمر;
				candidate_mtime = nth_packed_mtime(p, pos);
			} وإلا {
				candidate_mtime = p->mtime;
			}

			/*
			 * We have a surviving copy of the object in a cruft
			 * pack whose mtime is greater than or equal to the one
			 * we are considering. We can thus avoid packing an
			 * additional copy of that object.
			 */
			إذا (mtime <= candidate_mtime)
				أرجع 0;
		}
	}

	أرجع -1;
}

ساكن صحيح want_مليو_object(ثابت بنية object_id *oid, صحيح exclude,
			     بنية packed_git *p, uint32_t mtime)
{
	إذا (exclude)
		أرجع 1;
	إذا (incremental)
		أرجع 0;

	إذا (!is_pack_valid(p))
		أرجع -1;

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
	إذا (local && !p->pack_local)
		أرجع 0;

	/*
	 * Then handle .keep first, as we have a fast(er) path there.
	 */
	إذا (ignore_packed_keep_on_disk || ignore_packed_keep_اندريون_core ||
	    ignore_packed_keep_اندريون_core_open) {
		/*
		 * Set the flags for the kept-pack cache to be the ones we want
		 * to ignore.
		 *
		 * That is, if we are ignoring objects in on-disk keep packs,
		 * then we want to search through the on-disk keep and ignore
		 * the in-core ones.
		 */
		بنانشان flags = 0;
		إذا (ignore_packed_keep_on_disk)
			flags |= KEPT_PACK_ON_DISK;
		إذا (ignore_packed_keep_اندريون_core)
			flags |= KEPT_PACK_اندريون_CORE;
		إذا (ignore_packed_keep_اندريون_core_open)
			flags |= KEPT_PACK_اندريون_CORE_OPEN;

		/*
		 * If the object is in a pack that we want to ignore, *and* we
		 * don't have any cruft packs that are being retained, we can
		 * abort quickly.
		 */
		إذا (!ignore_packed_keep_اندريون_core_has_cruft) {
			إذا (ignore_packed_keep_on_disk && p->pack_keep)
				أرجع 0;
			إذا (ignore_packed_keep_اندريون_core && p->pack_keep_اندريون_core)
				أرجع 0;
			إذا (ignore_packed_keep_اندريون_core_open && p->pack_keep_اندريون_core_open)
				أرجع 0;
			إذا (has_object_kept_pack(p->repo, oid, flags))
				أرجع 0;
		} وإلا {
			/*
			 * But if there is at least one cruft pack which
			 * is being kept, we only want to include the
			 * provided object if it has a strictly greater
			 * mtime than any existing cruft copy.
			 */
			إذا (!want_cruft_object_mtime(p->repo, oid, flags,
						     mtime))
				أرجع 0;
		}
	}

	/*
	 * At this point we know definitively that either we don't care about
	 * keep-packs, or the object is not in one. Keep checking other
	 * conditions...
	 */
	إذا (!local || !have_non_local_packs)
		أرجع 1;

	/* we don't know yet; keep looking for more packs */
	أرجع -1;
}

ساكن صحيح want_object_اندريون_pack_one(بنية packed_git *p,
				   ثابت بنية object_id *oid,
				   صحيح exclude,
				   بنية packed_git **مليو_pack,
				   off_t *مليو_وٿي,
				   uint32_t مليو_mtime)
{
	off_t وٿي;

	إذا (p == *مليو_pack)
		وٿي = *مليو_وٿي;
	وإلا
		وٿي = ڳوليو_pack_داخلا_one(oid, p);

	إذا (وٿي) {
		إذا (!*مليو_pack) {
			إذا (!is_pack_valid(p))
				أرجع -1;
			*مليو_وٿي = وٿي;
			*مليو_pack = p;
		}
		أرجع want_مليو_object(oid, exclude, p, مليو_mtime);
	}
	أرجع -1;
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
ساكن صحيح want_object_اندريون_pack_mtime(ثابت بنية object_id *oid,
				     صحيح exclude,
				     بنية packed_git **مليو_pack,
				     off_t *مليو_وٿي,
				     uint32_t مليو_mtime)
{
	بنية odb_ذريعو_files *files =
		odb_ذريعو_files_downcast(the_repository->objects->ذريعو);
	صحيح want;
	بنية packfile_list_داخلا *e;

	إذا (!exclude && local) {
		/*
		 * Note that we start iterating at `dirs->next` so that we
		 * skip the local object directory.
		 */
		لكل (بنية odb_files_dir *dir = files->dirs->ايندڙ; dir; dir = dir->ايندڙ)
			إذا (!odb_ذريعو_read_object_info(&dir->loose->بنياد, oid, عدم, 0, عدم))
				أرجع 0;
	}

	/*
	 * If we already know the pack object lives in, start checks from that
	 * pack - in the usual case when neither --local was given nor .keep files
	 * are present we will determine the answer right now.
	 */
	إذا (*مليو_pack) {
		want = want_مليو_object(oid, exclude, *مليو_pack,
					 مليو_mtime);
		إذا (want != -1)
			أرجع want;

		*مليو_pack = عدم;
		*مليو_وٿي = 0;
	}

	لكل (بنية odb_files_dir *dir = files->dirs; dir; dir = dir->ايندڙ) {
		بنية multi_pack_اشاريو *m = get_multi_pack_اشاريو(dir->packed);
		بنية pack_داخلا e;

		إذا (m && midx_fill_داخلا(m, oid, &e, عدم) == MIDX_FILL_ڪاميابي) {
			want = want_object_اندريون_pack_one(e.p, oid, exclude, مليو_pack, مليو_وٿي, مليو_mtime);
			إذا (want != -1)
				أرجع want;
		}
	}

	لكل (بنية odb_files_dir *dir = files->dirs; dir; dir = dir->ايندڙ) {
		لكل (e = dir->packed->packs.سر; e; e = e->ايندڙ) {
			بنية packed_git *p = e->pack;
			want = want_object_اندريون_pack_one(p, oid, exclude, مليو_pack, مليو_وٿي, مليو_mtime);
			إذا (!exclude && want > 0)
				packfile_list_prepend(&dir->packed->packs, p);
			إذا (want != -1)
				أرجع want;
		}
	}

	إذا (uri_protocols.nr) {
		بنية configured_exclusion *ex =
			oidmap_get(&configured_exclusions, oid);
		صحيح i;
		ثابت حرف *p;

		إذا (ex) {
			لكل (i = 0; i < uri_protocols.nr; i++) {
				إذا (skip_اڳياڙي(ex->uri,
						uri_protocols.شيون[i].string,
						&p) &&
				    *p == ':') {
					oidset_وجهو(&excluded_by_config, oid);
					أرجع 0;
				}
			}
		}
	}

	أرجع 1;
}

ساكن مضمن صحيح want_object_اندريون_pack(ثابت بنية object_id *oid,
				      صحيح exclude,
				      بنية packed_git **مليو_pack,
				      off_t *مليو_وٿي)
{
	أرجع want_object_اندريون_pack_mtime(oid, exclude, مليو_pack, مليو_وٿي,
					 0);
}

ساكن بنية object_داخلا *create_object_داخلا(ثابت بنية object_id *oid,
						تعداد object_type type,
						uint32_t hash,
						صحيح exclude,
						صحيح no_try_delta,
						بنية packed_git *مليو_pack,
						off_t مليو_وٿي)
{
	بنية object_داخلا *داخلا;

	داخلا = packlist_alloc(&منزل_pack, oid);
	داخلا->hash = hash;
	oe_سيٽ_type(داخلا, type);
	إذا (exclude)
		داخلا->preferred_بنياد = 1;
	وإلا
		nr_نتيجو++;
	إذا (مليو_pack) {
		oe_سيٽ_اندريون_pack(&منزل_pack, داخلا, مليو_pack);
		داخلا->اندريون_pack_وٿي = مليو_وٿي;
	}

	داخلا->no_try_delta = no_try_delta;

	أرجع داخلا;
}

ساكن ثابت حرف no_گهيرو_warning[] = N_(
"disabling bitmap writing, as some objects are not being packed"
);

ساكن فراغ جوڙيو_object_داخلا(ثابت بنية object_id *oid, تعداد object_type type,
			     ثابت حرف *name, صحيح exclude)
{
	بنية packed_git *مليو_pack = عدم;
	off_t مليو_وٿي = 0;

	display_ترقي(ترقي_state, ++nr_ڏٺل);

	إذا (have_duplicate_داخلا(oid, exclude))
		أرجع;

	إذا (!want_object_اندريون_pack(oid, exclude, &مليو_pack, &مليو_وٿي)) {
		/* The pack is missing an object, so it will not have closure */
		إذا (write_bitmap_اشاريو) {
			إذا (write_bitmap_اشاريو != WRITE_BITMAP_QUIET)
				warning(_(no_گهيرو_warning));
			write_bitmap_اشاريو = 0;
		}
		أرجع;
	}

	create_object_داخلا(oid, type, pack_name_hash_fn(name),
			    exclude, name && no_try_delta(name),
			    مليو_pack, مليو_وٿي);
}

ساكن صحيح جوڙيو_object_داخلا_from_bitmap(ثابت بنية object_id *oid,
					تعداد object_type type,
					صحيح flags اڻ_استعمال_ٿيل, uint32_t name_hash,
					بنية packed_git *pack, off_t وٿي,
					فراغ *payload اڻ_استعمال_ٿيل)
{
	display_ترقي(ترقي_state, ++nr_ڏٺل);

	إذا (have_duplicate_داخلا(oid, 0))
		أرجع 0;

	إذا (!want_object_اندريون_pack(oid, 0, &pack, &وٿي))
		أرجع 0;

	create_object_داخلا(oid, type, name_hash, 0, 0, pack, وٿي);
	أرجع 0;
}

بنية pbase_وڻ_cache {
	بنية object_id oid;
	صحيح ref;
	صحيح temporary;
	فراغ *وڻ_معلومات;
	بنانشان ڊگهو وڻ_سائيز;
};

ساكن بنية pbase_وڻ_cache *(pbase_وڻ_cache[256]);
ساكن صحيح pbase_وڻ_cache_ix(ثابت بنية object_id *oid)
{
	أرجع oid->hash[0] % ARRAY_سائيز(pbase_وڻ_cache);
}
ساكن صحيح pbase_وڻ_cache_ix_incr(صحيح ix)
{
	أرجع (ix+1) % ARRAY_سائيز(pbase_وڻ_cache);
}

ساكن بنية pbase_وڻ {
	بنية pbase_وڻ *ايندڙ;
	/* This is a phony "cache" entry; we are not
	 * going to evict it or find it through _get()
	 * mechanism -- this is for the toplevel node that
	 * would almost always change with any commit.
	 */
	بنية pbase_وڻ_cache pcache;
} *pbase_وڻ;

ساكن بنية pbase_وڻ_cache *pbase_وڻ_get(ثابت بنية object_id *oid)
{
	بنية pbase_وڻ_cache *ent, *nent;
	فراغ *معلومات;
	بنانشان ڊگهو سائيز;
	سائيز_t سائيز_st = 0;
	تعداد object_type type;
	صحيح neigh;
	صحيح my_ix = pbase_وڻ_cache_ix(oid);
	صحيح موجود_ix = -1;

	/* pbase-tree-cache acts as a limited hashtable.
	 * your object will be found at your index or within a few
	 * slots after that slot if it is cached.
	 */
	لكل (neigh = 0; neigh < 8; neigh++) {
		ent = pbase_وڻ_cache[my_ix];
		إذا (ent && oideq(&ent->oid, oid)) {
			ent->ref++;
			أرجع ent;
		}
		وإلا إذا (((موجود_ix < 0) && (!ent || !ent->ref)) ||
			 ((0 <= موجود_ix) &&
			  (!ent && pbase_وڻ_cache[موجود_ix])))
			موجود_ix = my_ix;
		إذا (!ent)
			اكسر;
		my_ix = pbase_وڻ_cache_ix_incr(my_ix);
	}

	/* Did not find one.  Either we got a bogus request or
	 * we need to read and perhaps cache.
	 */
	معلومات = odb_read_object(the_repository->objects, oid, &type, &سائيز_st);
	سائيز = cast_سائيز_t_منزل_ulong(سائيز_st);
	إذا (!معلومات)
		أرجع عدم;
	إذا (type != OBJ_وڻ) {
		حرر(معلومات);
		أرجع عدم;
	}

	/* We need to either cache or return a throwaway copy */

	إذا (موجود_ix < 0)
		ent = عدم;
	وإلا {
		ent = pbase_وڻ_cache[موجود_ix];
		my_ix = موجود_ix;
	}

	إذا (!ent) {
		nent = xmalloc(حجم(*nent));
		nent->temporary = (موجود_ix < 0);
	}
	وإلا {
		/* evict and reuse */
		حرر(ent->وڻ_معلومات);
		nent = ent;
	}
	oidcpy(&nent->oid, oid);
	nent->وڻ_معلومات = معلومات;
	nent->وڻ_سائيز = سائيز;
	nent->ref = 1;
	إذا (!nent->temporary)
		pbase_وڻ_cache[my_ix] = nent;
	أرجع nent;
}

ساكن فراغ pbase_وڻ_put(بنية pbase_وڻ_cache *cache)
{
	إذا (!cache->temporary) {
		cache->ref--;
		أرجع;
	}
	حرر(cache->وڻ_معلومات);
	حرر(cache);
}

ساكن سائيز_t name_cmp_len(ثابت حرف *name)
{
	أرجع strcspn(name, "\n/");
}

ساكن فراغ جوڙيو_pbase_object(بنية وڻ_desc *وڻ,
			     ثابت حرف *name,
			     سائيز_t cmplen,
			     ثابت حرف *fullname)
{
	بنية name_داخلا داخلا;
	صحيح cmp;

	طالما (وڻ_داخلا(وڻ,&داخلا)) {
		إذا (S_ISGITLINK(داخلا.گهڻائي))
			استمر;
		cmp = وڻ_داخلا_len(&داخلا) != cmplen ? 1 :
		      memcmp(name, داخلا.رستو, cmplen);
		إذا (cmp > 0)
			استمر;
		إذا (cmp < 0)
			أرجع;
		إذا (name[cmplen] != '/') {
			جوڙيو_object_داخلا(&داخلا.oid,
					 object_type(داخلا.گهڻائي),
					 fullname, 1);
			أرجع;
		}
		إذا (S_ISDIR(داخلا.گهڻائي)) {
			بنية وڻ_desc sub;
			بنية pbase_وڻ_cache *وڻ;
			ثابت حرف *هيٺ_ڏانهن = name+cmplen+1;
			سائيز_t downlen = name_cmp_len(هيٺ_ڏانهن);

			وڻ = pbase_وڻ_get(&داخلا.oid);
			إذا (!وڻ)
				أرجع;
			init_وڻ_desc(&sub, &وڻ->oid,
				       وڻ->وڻ_معلومات, وڻ->وڻ_سائيز);

			جوڙيو_pbase_object(&sub, هيٺ_ڏانهن, downlen, fullname);
			pbase_وڻ_put(وڻ);
		}
	}
}

ساكن بنانشان *پورو_pbase_paths;
ساكن صحيح پورو_pbase_paths_انگ;
ساكن صحيح پورو_pbase_paths_alloc;
ساكن صحيح پورو_pbase_رستو_pos(بنانشان hash)
{
	صحيح lo = 0;
	صحيح hi = پورو_pbase_paths_انگ;
	طالما (lo < hi) {
		صحيح mi = lo + (hi - lo) / 2;
		إذا (پورو_pbase_paths[mi] == hash)
			أرجع mi;
		إذا (پورو_pbase_paths[mi] < hash)
			hi = mi;
		وإلا
			lo = mi + 1;
	}
	أرجع -lo-1;
}

ساكن صحيح check_pbase_رستو(بنانشان hash)
{
	صحيح pos = پورو_pbase_رستو_pos(hash);
	إذا (0 <= pos)
		أرجع 1;
	pos = -pos - 1;
	ALLOC_GROW(پورو_pbase_paths,
		   پورو_pbase_paths_انگ + 1,
		   پورو_pbase_paths_alloc);
	پورو_pbase_paths_انگ++;
	إذا (pos < پورو_pbase_paths_انگ)
		MOVE_ARRAY(پورو_pbase_paths + pos + 1, پورو_pbase_paths + pos,
			   پورو_pbase_paths_انگ - pos - 1);
	پورو_pbase_paths[pos] = hash;
	أرجع 0;
}

ساكن فراغ جوڙيو_preferred_بنياد_object(ثابت حرف *name)
{
	بنية pbase_وڻ *it;
	سائيز_t cmplen;
	بنانشان hash = pack_name_hash_fn(name);

	إذا (!انگ_preferred_بنياد || check_pbase_رستو(hash))
		أرجع;

	cmplen = name_cmp_len(name);
	لكل (it = pbase_وڻ; it; it = it->ايندڙ) {
		إذا (cmplen == 0) {
			جوڙيو_object_داخلا(&it->pcache.oid, OBJ_وڻ, عدم, 1);
		}
		وإلا {
			بنية وڻ_desc وڻ;
			init_وڻ_desc(&وڻ, &it->pcache.oid,
				       it->pcache.وڻ_معلومات, it->pcache.وڻ_سائيز);
			جوڙيو_pbase_object(&وڻ, name, cmplen, name);
		}
	}
}

ساكن فراغ جوڙيو_preferred_بنياد(بنية object_id *oid)
{
	بنية pbase_وڻ *it;
	فراغ *معلومات;
	بنانشان ڊگهو سائيز;
	سائيز_t سائيز_st = 0;
	بنية object_id وڻ_oid;

	إذا (دري <= انگ_preferred_بنياد++)
		أرجع;

	معلومات = odb_read_object_peeled(the_repository->objects, oid,
				      OBJ_وڻ, &سائيز_st, &وڻ_oid);
	سائيز = cast_سائيز_t_منزل_ulong(سائيز_st);
	إذا (!معلومات)
		أرجع;

	لكل (it = pbase_وڻ; it; it = it->ايندڙ) {
		إذا (oideq(&it->pcache.oid, &وڻ_oid)) {
			حرر(معلومات);
			أرجع;
		}
	}

	CALLOC_ARRAY(it, 1);
	it->ايندڙ = pbase_وڻ;
	pbase_وڻ = it;

	oidcpy(&it->pcache.oid, &وڻ_oid);
	it->pcache.وڻ_معلومات = معلومات;
	it->pcache.وڻ_سائيز = سائيز;
}

ساكن فراغ cleanup_preferred_بنياد(فراغ)
{
	بنية pbase_وڻ *it;
	بنانشان i;

	it = pbase_وڻ;
	pbase_وڻ = عدم;
	طالما (it) {
		بنية pbase_وڻ *tmp = it;
		it = tmp->ايندڙ;
		حرر(tmp->pcache.وڻ_معلومات);
		حرر(tmp);
	}

	لكل (i = 0; i < ARRAY_سائيز(pbase_وڻ_cache); i++) {
		إذا (!pbase_وڻ_cache[i])
			استمر;
		حرر(pbase_وڻ_cache[i]->وڻ_معلومات);
		FREE_AND_NULL(pbase_وڻ_cache[i]);
	}

	FREE_AND_NULL(پورو_pbase_paths);
	پورو_pbase_paths_انگ = پورو_pbase_paths_alloc = 0;
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
ساكن صحيح can_reuse_delta(ثابت بنية object_id *بنياد_oid,
			   بنية object_داخلا *delta,
			   بنية object_داخلا **بنياد_ٻاهريون)
{
	بنية object_داخلا *بنياد;

	/*
	 * First see if we're already sending the base (or it's explicitly in
	 * our "excluded" list).
	 */
	بنياد = packlist_ڳوليو(&منزل_pack, بنياد_oid);
	إذا (بنياد) {
		إذا (!اندريون_same_island(&delta->idx.oid, &بنياد->idx.oid))
			أرجع 0;
		*بنياد_ٻاهريون = بنياد;
		أرجع 1;
	}

	/*
	 * Otherwise, reachability bitmaps may tell us if the receiver has it,
	 * even if it was buried too deep in history to make it into the
	 * packing list.
	 */
	إذا (thin && bitmap_has_oid_اندريون_uninteresting(bitmap_git, بنياد_oid)) {
		إذا (use_delta_islands) {
			إذا (!اندريون_same_island(&delta->idx.oid, بنياد_oid))
				أرجع 0;
		}
		*بنياد_ٻاهريون = عدم;
		أرجع 1;
	}

	أرجع 0;
}

ساكن فراغ prefetch_منزل_pack(uint32_t object_اشاريو_شروعات) {
	بنية oid_array منزل_fetch = OID_ARRAY_INIT;
	uint32_t i;

	لكل (i = object_اشاريو_شروعات; i < منزل_pack.nr_objects; i++) {
		بنية object_داخلا *داخلا = منزل_pack.objects + i;

		إذا (!odb_read_object_info_extended(the_repository->objects,
						   &داخلا->idx.oid,
						   عدم,
						   OBJECT_INFO_FOR_PREFETCH))
			استمر;
		oid_array_append(&منزل_fetch, &داخلا->idx.oid);
	}
	promisor_remote_get_سڌو(the_repository,
				   منزل_fetch.oid, منزل_fetch.nr);
	oid_array_clear(&منزل_fetch);
}

ساكن فراغ check_object(بنية object_داخلا *داخلا, uint32_t object_اشاريو)
{
	سائيز_t canonical_سائيز;
	تعداد object_type type;
	بنية object_info oi = {.typep = &type, .sizep = &canonical_سائيز};

	إذا (اندريون_PACK(داخلا)) {
		بنية packed_git *p = اندريون_PACK(داخلا);
		بنية pack_دري *w_curs = عدم;
		صحيح have_بنياد = 0;
		بنية object_id بنياد_ref;
		بنية object_داخلا *بنياد_داخلا;
		سائيز_t استعمال_ٿيل_2, استعمال_ٿيل_0, avail;
		off_t ofs;
		بنانشان حرف *buf, c;
		تعداد object_type type;
		سائيز_t اندريون_pack_سائيز;

		buf = use_pack(p, &w_curs, داخلا->اندريون_pack_وٿي, &avail);

		/*
		 * We want in_pack_type even if we do not reuse delta
		 * since non-delta representations could still be reused.
		 */
		استعمال_ٿيل_2 = unpack_object_سرو_بفر(buf, avail,
						   &type,
						   &اندريون_pack_سائيز);
		إذا (استعمال_ٿيل_2 == 0)
			اذهب give_مٿي;

		إذا (type < 0)
			BUG("invalid type %d", type);
		داخلا->اندريون_pack_type = type;

		/*
		 * Determine if this is a delta and if so whether we can
		 * reuse it or not.  Otherwise let's find out as cheaply as
		 * possible what the actual type and size for this object is.
		 */
		بدل (داخلا->اندريون_pack_type) {
		افتراضي:
			/* Not a delta hence we've already got all we need. */
			oe_سيٽ_type(داخلا, داخلا->اندريون_pack_type);
			سيٽ_سائيز(داخلا, اندريون_pack_سائيز);
			داخلا->اندريون_pack_سرو_سائيز = استعمال_ٿيل_2;
			إذا (oe_type(داخلا) < OBJ_COMMIT || oe_type(داخلا) > OBJ_BLOB)
				اذهب give_مٿي;
			unuse_pack(&w_curs);
			أرجع;
		حالة OBJ_REF_DELTA:
			إذا (reuse_delta && !داخلا->preferred_بنياد) {
				oidread(&بنياد_ref,
					use_pack(p, &w_curs,
						 داخلا->اندريون_pack_وٿي + استعمال_ٿيل_2,
						 عدم),
					the_repository->hash_algo);
				have_بنياد = 1;
			}
			داخلا->اندريون_pack_سرو_سائيز = استعمال_ٿيل_2 + the_hash_algo->rawsz;
			اكسر;
		حالة OBJ_OFS_DELTA:
			buf = use_pack(p, &w_curs,
				       داخلا->اندريون_pack_وٿي + استعمال_ٿيل_2, عدم);
			استعمال_ٿيل_0 = 0;
			c = buf[استعمال_ٿيل_0++];
			ofs = c & 127;
			طالما (c & 128) {
				ofs += 1;
				إذا (!ofs || MSB(ofs, 7)) {
					error(_("delta base offset overflow in pack for %s"),
					      oid_منزل_hex(&داخلا->idx.oid));
					اذهب give_مٿي;
				}
				c = buf[استعمال_ٿيل_0++];
				ofs = (ofs << 7) + (c & 127);
			}
			ofs = داخلا->اندريون_pack_وٿي - ofs;
			إذا (ofs <= 0 || ofs >= داخلا->اندريون_pack_وٿي) {
				error(_("delta base offset out of bound for %s"),
				      oid_منزل_hex(&داخلا->idx.oid));
				اذهب give_مٿي;
			}
			إذا (reuse_delta && !داخلا->preferred_بنياد) {
				uint32_t pos;
				إذا (وٿي_منزل_pack_pos(p, ofs, &pos) < 0)
					اذهب give_مٿي;
				إذا (!nth_packed_object_id(&بنياد_ref, p,
							  pack_pos_منزل_اشاريو(p, pos)))
					have_بنياد = 1;
			}
			داخلا->اندريون_pack_سرو_سائيز = استعمال_ٿيل_2 + استعمال_ٿيل_0;
			اكسر;
		}

		إذا (have_بنياد &&
		    can_reuse_delta(&بنياد_ref, داخلا, &بنياد_داخلا)) {
			oe_سيٽ_type(داخلا, داخلا->اندريون_pack_type);
			سيٽ_سائيز(داخلا, اندريون_pack_سائيز); /* delta size */
			سيٽ_DELTA_سائيز(داخلا, اندريون_pack_سائيز);

			إذا (بنياد_داخلا) {
				سيٽ_DELTA(داخلا, بنياد_داخلا);
				داخلا->delta_sibling_idx = بنياد_داخلا->delta_ٻار_idx;
				سيٽ_DELTA_ٻار(بنياد_داخلا, داخلا);
			} وإلا {
				سيٽ_DELTA_EXT(داخلا, &بنياد_ref);
			}

			unuse_pack(&w_curs);
			أرجع;
		}

		إذا (oe_type(داخلا)) {
			off_t delta_pos;

			/*
			 * This must be a delta and we already know what the
			 * final object type is.  Let's extract the actual
			 * object size from the delta header.
			 */
			delta_pos = داخلا->اندريون_pack_وٿي + داخلا->اندريون_pack_سرو_سائيز;
			canonical_سائيز = get_سائيز_from_delta(p, &w_curs,
							     delta_pos);
			إذا (canonical_سائيز == 0)
				اذهب give_مٿي;
			سيٽ_سائيز(داخلا, canonical_سائيز);
			unuse_pack(&w_curs);
			أرجع;
		}

		/*
		 * No choice but to fall back to the recursive delta walk
		 * with odb_read_object_info() to find about the object type
		 * at this point...
		 */
		give_مٿي:
		unuse_pack(&w_curs);
	}

	إذا (odb_read_object_info_extended(the_repository->objects, &داخلا->idx.oid, &oi,
					  OBJECT_INFO_SKIP_FETCH_OBJECT | OBJECT_INFO_LOOKUP_REPLACE) < 0) {
		إذا (repo_has_promisor_remote(the_repository)) {
			prefetch_منزل_pack(object_اشاريو);
			إذا (odb_read_object_info_extended(the_repository->objects, &داخلا->idx.oid, &oi,
							  OBJECT_INFO_SKIP_FETCH_OBJECT | OBJECT_INFO_LOOKUP_REPLACE) < 0)
				type = -1;
		} وإلا {
			type = -1;
		}
	}
	oe_سيٽ_type(داخلا, type);
	إذا (داخلا->type_valid) {
		سيٽ_سائيز(داخلا, canonical_سائيز);
	} وإلا {
		/*
		 * Bad object type is checked in prepare_pack().  This is
		 * to permit a missing preferred base object to be ignored
		 * as a preferred base.  Doing so can result in a larger
		 * pack file, but the transfer will still take place.
		 */
	}
}

ساكن صحيح pack_وٿي_ترتيب_ڏيو(ثابت فراغ *_a, ثابت فراغ *_b)
{
	ثابت بنية object_داخلا *a = *(بنية object_داخلا **)_a;
	ثابت بنية object_داخلا *b = *(بنية object_داخلا **)_b;
	ثابت بنية packed_git *a_اندريون_pack = اندريون_PACK(a);
	ثابت بنية packed_git *b_اندريون_pack = اندريون_PACK(b);

	/* avoid filesystem trashing with loose objects */
	إذا (!a_اندريون_pack && !b_اندريون_pack)
		أرجع oidcmp(&a->idx.oid, &b->idx.oid);

	إذا (a_اندريون_pack < b_اندريون_pack)
		أرجع -1;
	إذا (a_اندريون_pack > b_اندريون_pack)
		أرجع 1;
	أرجع a->اندريون_pack_وٿي < b->اندريون_pack_وٿي ? -1 :
			(a->اندريون_pack_وٿي > b->اندريون_pack_وٿي);
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
ساكن فراغ drop_reused_delta(بنية object_داخلا *داخلا)
{
	بنانشان *idx = &منزل_pack.objects[داخلا->delta_idx - 1].delta_ٻار_idx;
	بنية object_info oi = OBJECT_INFO_INIT;
	تعداد object_type type;
	سائيز_t سائيز;

	طالما (*idx) {
		بنية object_داخلا *oe = &منزل_pack.objects[*idx - 1];

		إذا (oe == داخلا)
			*idx = oe->delta_sibling_idx;
		وإلا
			idx = &oe->delta_sibling_idx;
	}
	سيٽ_DELTA(داخلا, عدم);
	داخلا->اونهائي = 0;

	oi.sizep = &سائيز;
	oi.typep = &type;
	إذا (packed_object_info(عدم, اندريون_PACK(داخلا), داخلا->اندريون_pack_وٿي, &oi) < 0) {
		/*
		 * We failed to get the info from this pack for some reason;
		 * fall back to odb_read_object_info, which may find another copy.
		 * And if that fails, the error will be recorded in oe_type(entry)
		 * and dealt with in prepare_pack().
		 */
		oe_سيٽ_type(داخلا,
			    odb_read_object_info(the_repository->objects,
						 &داخلا->idx.oid, &سائيز));
	} وإلا {
		oe_سيٽ_type(داخلا, type);
	}
	سيٽ_سائيز(داخلا, سائيز);
}

/*
 * Follow the chain of deltas from this entry onward, throwing away any links
 * that cause us to hit a cycle (as determined by the DFS state flags in
 * the entries).
 *
 * We also detect too-long reused chains that would violate our --depth
 * limit.
 */
ساكن فراغ break_delta_chains(بنية object_داخلا *داخلا)
{
	/*
	 * The actual depth of each object we will write is stored as an int,
	 * as it cannot exceed our int "depth" limit. But before we break
	 * changes based no that limit, we may potentially go as deep as the
	 * number of objects, which is elsewhere bounded to a uint32_t.
	 */
	uint32_t ڪل_اونهائي;
	بنية object_داخلا *cur, *ايندڙ;

	لكل (cur = داخلا, ڪل_اونهائي = 0;
	     cur;
	     cur = DELTA(cur), ڪل_اونهائي++) {
		إذا (cur->dfs_state == DFS_پورو) {
			/*
			 * We've already seen this object and know it isn't
			 * part of a cycle. We do need to append its depth
			 * to our count.
			 */
			ڪل_اونهائي += cur->اونهائي;
			اكسر;
		}

		/*
		 * We break cycles before looping, so an ACTIVE state (or any
		 * other cruft which made its way into the state variable)
		 * is a bug.
		 */
		إذا (cur->dfs_state != DFS_NONE)
			BUG("confusing delta dfs state in first pass: %d",
			    cur->dfs_state);

		/*
		 * Now we know this is the first time we've seen the object. If
		 * it's not a delta, we're done traversing, but we'll mark it
		 * done to save time on future traversals.
		 */
		إذا (!DELTA(cur)) {
			cur->dfs_state = DFS_پورو;
			اكسر;
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
		cur->dfs_state = DFS_سرگرم;
		إذا (DELTA(cur)->dfs_state == DFS_سرگرم) {
			drop_reused_delta(cur);
			cur->dfs_state = DFS_پورو;
			اكسر;
		}
	}

	/*
	 * And now that we've gone all the way to the bottom of the chain, we
	 * need to clear the active flags and set the depth fields as
	 * appropriate. Unlike the loop above, which can quit when it drops a
	 * delta, we need to keep going to look for more depth cuts. So we need
	 * an extra "next" pointer to keep going after we reset cur->delta.
	 */
	لكل (cur = داخلا; cur; cur = ايندڙ) {
		ايندڙ = DELTA(cur);

		/*
		 * We should have a chain of zero or more ACTIVE states down to
		 * a final DONE. We can quit after the DONE, because either it
		 * has no bases, or we've already handled them in a previous
		 * call.
		 */
		إذا (cur->dfs_state == DFS_پورو)
			اكسر;
		وإلا إذا (cur->dfs_state != DFS_سرگرم)
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
		cur->اونهائي = (ڪل_اونهائي--) % (اونهائي + 1);
		إذا (!cur->اونهائي)
			drop_reused_delta(cur);

		cur->dfs_state = DFS_پورو;
	}
}

ساكن فراغ get_object_details(فراغ)
{
	uint32_t i;
	بنية object_داخلا **sorted_by_وٿي;

	إذا (ترقي)
		ترقي_state = شروعات_ترقي(the_repository,
						_("Counting objects"),
						منزل_pack.nr_objects);

	CALLOC_ARRAY(sorted_by_وٿي, منزل_pack.nr_objects);
	لكل (i = 0; i < منزل_pack.nr_objects; i++)
		sorted_by_وٿي[i] = منزل_pack.objects + i;
	QSORT(sorted_by_وٿي, منزل_pack.nr_objects, pack_وٿي_ترتيب_ڏيو);

	لكل (i = 0; i < منزل_pack.nr_objects; i++) {
		بنية object_داخلا *داخلا = sorted_by_وٿي[i];
		check_object(داخلا, i);
		إذا (داخلا->type_valid &&
		    oe_سائيز_greater_than(&منزل_pack, داخلا,
					 repo_settings_get_big_file_حد(the_repository)))
			داخلا->no_try_delta = 1;
		display_ترقي(ترقي_state, i + 1);
	}
	stop_ترقي(&ترقي_state);

	/*
	 * This must happen in a second pass, since we rely on the delta
	 * information for the whole list being completed.
	 */
	لكل (i = 0; i < منزل_pack.nr_objects; i++)
		break_delta_chains(&منزل_pack.objects[i]);

	حرر(sorted_by_وٿي);
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
ساكن صحيح type_سائيز_ترتيب_ڏيو(ثابت فراغ *_a, ثابت فراغ *_b)
{
	ثابت بنية object_داخلا *a = *(بنية object_داخلا **)_a;
	ثابت بنية object_داخلا *b = *(بنية object_داخلا **)_b;
	ثابت تعداد object_type a_type = oe_type(a);
	ثابت تعداد object_type b_type = oe_type(b);
	ثابت بنانشان ڊگهو a_سائيز = سائيز_3(a);
	ثابت بنانشان ڊگهو b_سائيز = سائيز_3(b);

	إذا (a_type > b_type)
		أرجع -1;
	إذا (a_type < b_type)
		أرجع 1;
	إذا (a->hash > b->hash)
		أرجع -1;
	إذا (a->hash < b->hash)
		أرجع 1;
	إذا (a->preferred_بنياد > b->preferred_بنياد)
		أرجع -1;
	إذا (a->preferred_بنياد < b->preferred_بنياد)
		أرجع 1;
	إذا (use_delta_islands) {
		ثابت صحيح island_cmp = island_delta_cmp(&a->idx.oid, &b->idx.oid);
		إذا (island_cmp)
			أرجع island_cmp;
	}
	إذا (a_سائيز > b_سائيز)
		أرجع -1;
	إذا (a_سائيز < b_سائيز)
		أرجع 1;
	أرجع a < b ? -1 : (a > b);  /* newest first */
}

بنية unpacked {
	بنية object_داخلا *داخلا;
	فراغ *معلومات;
	بنية delta_اشاريو *اشاريو;
	بنانشان اونهائي;
};

ساكن صحيح delta_cacheable(سائيز_t src_سائيز, سائيز_t trg_سائيز,
			   سائيز_t delta_سائيز_2)
{
	إذا (max_delta_cache_سائيز && delta_cache_سائيز + delta_سائيز_2 > max_delta_cache_سائيز)
		أرجع 0;

	إذا (delta_سائيز_2 < cache_max_small_delta_سائيز)
		أرجع 1;

	/* cache delta, if objects are large enough compared to delta size */
	إذا ((src_سائيز >> 20) + (trg_سائيز >> 21) > (delta_سائيز_2 >> 10))
		أرجع 1;

	أرجع 0;
}

/* Protect delta_cache_size */
ساكن pthread_mutex_t cache_mutex;
#وصف_ڪريو cache_تالو()		pthread_mutex_تالو(&cache_mutex)
#وصف_ڪريو cache_unlock()		pthread_mutex_unlock(&cache_mutex)

/*
 * Protect object list partitioning (e.g. struct thread_param) and
 * progress_state
 */
ساكن pthread_mutex_t ترقي_mutex;
#وصف_ڪريو ترقي_تالو()		pthread_mutex_تالو(&ترقي_mutex)
#وصف_ڪريو ترقي_unlock()	pthread_mutex_unlock(&ترقي_mutex)

/*
 * Access to struct object_entry is unprotected since each thread owns
 * a portion of the main object list. Just don't access object entries
 * ahead in the list because they can be stolen and would need
 * progress_mutex for protection.
 */

ساكن مضمن صحيح oe_سائيز_less_than(بنية packing_معلومات *pack,
				    ثابت بنية object_داخلا *lhs,
				    سائيز_t rhs)
{
	إذا (lhs->سائيز_valid)
		أرجع lhs->سائيز_2 < rhs;
	إذا (rhs < pack->oe_سائيز_حد_بندي) /* rhs < 2^x <= lhs ? */
		أرجع 0;
	أرجع oe_get_سائيز_slow(pack, lhs) < rhs;
}

ساكن مضمن فراغ oe_سيٽ_وڻ_اونهائي(بنية packing_معلومات *pack,
				     بنية object_داخلا *e,
				     بنانشان صحيح وڻ_اونهائي)
{
	إذا (!pack->وڻ_اونهائي)
		CALLOC_ARRAY(pack->وڻ_اونهائي, pack->nr_alloc);
	pack->وڻ_اونهائي[e - pack->objects] = وڻ_اونهائي;
}

ساكن فراغ رڪارڊ_وڻ_اونهائي(ثابت بنية object_id *oid, ثابت حرف *name)
{
	ثابت حرف *p;
	بنانشان اونهائي;
	بنية object_داخلا *ent;

	/* the empty string is a root tree, which is depth 0 */
	اونهائي = *name ? 1 : 0;
	لكل (p = strchr(name, '/'); p; p = strchr(p + 1, '/'))
		اونهائي++;

	ent = packlist_ڳوليو(&منزل_pack, oid);
	إذا (ent && اونهائي > oe_وڻ_اونهائي(&منزل_pack, ent))
		oe_سيٽ_وڻ_اونهائي(&منزل_pack, ent, اونهائي);
}

/*
 * Return the size of the object without doing any delta
 * reconstruction (so non-deltas are true object sizes, but deltas
 * return the size of the delta data).
 */
سائيز_t oe_get_سائيز_slow(بنية packing_معلومات *pack,
			ثابت بنية object_داخلا *e)
{
	بنية packed_git *p;
	بنية pack_دري *w_curs;
	بنانشان حرف *buf;
	تعداد object_type type;
	سائيز_t استعمال_ٿيل_2, avail, سائيز;

	إذا (e->type_ != OBJ_OFS_DELTA && e->type_ != OBJ_REF_DELTA) {
		سائيز_t sz;
		packing_معلومات_تالو(&منزل_pack);
		إذا (odb_read_object_info(the_repository->objects,
					 &e->idx.oid, &sz) < 0)
			die(_("unable to get size of %s"),
			    oid_منزل_hex(&e->idx.oid));
		packing_معلومات_unlock(&منزل_pack);
		أرجع sz;
	}

	p = oe_اندريون_pack(pack, e);
	إذا (!p)
		BUG("when e->type is a delta, it must belong to a pack");

	packing_معلومات_تالو(&منزل_pack);
	w_curs = عدم;
	buf = use_pack(p, &w_curs, e->اندريون_pack_وٿي, &avail);
	استعمال_ٿيل_2 = unpack_object_سرو_بفر(buf, avail, &type, &سائيز);
	إذا (استعمال_ٿيل_2 == 0)
		die(_("unable to parse object header of %s"),
		    oid_منزل_hex(&e->idx.oid));

	unuse_pack(&w_curs);
	packing_معلومات_unlock(&منزل_pack);
	أرجع سائيز;
}

ساكن صحيح try_delta(بنية unpacked *trg, بنية unpacked *src,
		     بنانشان max_اونهائي, سائيز_t *mem_usage)
{
	بنية object_داخلا *trg_داخلا = trg->داخلا;
	بنية object_داخلا *src_داخلا = src->داخلا;
	بنانشان ڊگهو trg_سائيز, src_سائيز, sizediff, max_سائيز, sz;
	سائيز_t delta_سائيز_2;
	بنانشان ref_اونهائي;
	تعداد object_type type;
	فراغ *delta_buf;

	/* Don't bother doing diffs between different types */
	إذا (oe_type(trg_داخلا) != oe_type(src_داخلا))
		أرجع -1;

	/*
	 * We do not bother to try a delta that we discarded on an
	 * earlier try, but only when reusing delta data.  Note that
	 * src_entry that is marked as the preferred_base should always
	 * be considered, as even if we produce a suboptimal delta against
	 * it, we will still save the transfer cost, as we already know
	 * the other side has it and we won't send src_entry at all.
	 */
	إذا (reuse_delta && اندريون_PACK(trg_داخلا) &&
	    اندريون_PACK(trg_داخلا) == اندريون_PACK(src_داخلا) &&
	    !src_داخلا->preferred_بنياد &&
	    trg_داخلا->اندريون_pack_type != OBJ_REF_DELTA &&
	    trg_داخلا->اندريون_pack_type != OBJ_OFS_DELTA)
		أرجع 0;

	/* Let's not bust the allowed depth. */
	إذا (src->اونهائي >= max_اونهائي)
		أرجع 0;

	/* Now some size filtering heuristics. */
	trg_سائيز = سائيز_3(trg_داخلا);
	إذا (!DELTA(trg_داخلا)) {
		max_سائيز = trg_سائيز/2 - the_hash_algo->rawsz;
		ref_اونهائي = 1;
	} وإلا {
		max_سائيز = DELTA_سائيز(trg_داخلا);
		ref_اونهائي = trg->اونهائي;
	}
	max_سائيز = (uint64_t)max_سائيز * (max_اونهائي - src->اونهائي) /
						(max_اونهائي - ref_اونهائي + 1);
	إذا (max_سائيز == 0)
		أرجع 0;
	src_سائيز = سائيز_3(src_داخلا);
	sizediff = src_سائيز < trg_سائيز ? trg_سائيز - src_سائيز : 0;
	إذا (sizediff >= max_سائيز)
		أرجع 0;
	إذا (trg_سائيز < src_سائيز / 32)
		أرجع 0;

	إذا (!اندريون_same_island(&trg->داخلا->idx.oid, &src->داخلا->idx.oid))
		أرجع 0;

	/* Load data if not already done */
	إذا (!trg->معلومات) {
		سائيز_t sz_st = 0;
		packing_معلومات_تالو(&منزل_pack);
		trg->معلومات = odb_read_object(the_repository->objects,
					    &trg_داخلا->idx.oid, &type,
					    &sz_st);
		sz = cast_سائيز_t_منزل_ulong(sz_st);
		packing_معلومات_unlock(&منزل_pack);
		إذا (!trg->معلومات)
			die(_("object %s cannot be read"),
			    oid_منزل_hex(&trg_داخلا->idx.oid));
		إذا (sz != trg_سائيز)
			die(_("object %s inconsistent object length (%"PRIuMAX" vs %"PRIuMAX")"),
			    oid_منزل_hex(&trg_داخلا->idx.oid), (uintmax_t)sz,
			    (uintmax_t)trg_سائيز);
		*mem_usage += sz;
	}
	إذا (!src->معلومات) {
		سائيز_t sz_st = 0;
		packing_معلومات_تالو(&منزل_pack);
		src->معلومات = odb_read_object(the_repository->objects,
					    &src_داخلا->idx.oid, &type,
					    &sz_st);
		sz = cast_سائيز_t_منزل_ulong(sz_st);
		packing_معلومات_unlock(&منزل_pack);
		إذا (!src->معلومات) {
			إذا (src_داخلا->preferred_بنياد) {
				ساكن صحيح warned = 0;
				إذا (!warned++)
					warning(_("object %s cannot be read"),
						oid_منزل_hex(&src_داخلا->idx.oid));
				/*
				 * Those objects are not included in the
				 * resulting pack.  Be resilient and ignore
				 * them if they can't be read, in case the
				 * pack could be created nevertheless.
				 */
				أرجع 0;
			}
			die(_("object %s cannot be read"),
			    oid_منزل_hex(&src_داخلا->idx.oid));
		}
		إذا (sz != src_سائيز)
			die(_("object %s inconsistent object length (%"PRIuMAX" vs %"PRIuMAX")"),
			    oid_منزل_hex(&src_داخلا->idx.oid), (uintmax_t)sz,
			    (uintmax_t)src_سائيز);
		*mem_usage += sz;
	}
	إذا (!src->اشاريو) {
		src->اشاريو = create_delta_اشاريو(src->معلومات, src_سائيز);
		إذا (!src->اشاريو) {
			ساكن صحيح warned = 0;
			إذا (!warned++)
				warning(_("suboptimal pack - out of memory"));
			أرجع 0;
		}
		*mem_usage += sizeof_delta_اشاريو(src->اشاريو);
	}

	delta_buf = create_delta(src->اشاريو, trg->معلومات, trg_سائيز, &delta_سائيز_2, max_سائيز);
	إذا (!delta_buf)
		أرجع 0;

	إذا (DELTA(trg_داخلا)) {
		/* Prefer only shallower same-sized deltas. */
		إذا (delta_سائيز_2 == DELTA_سائيز(trg_داخلا) &&
		    src->اونهائي + 1 >= trg->اونهائي) {
			حرر(delta_buf);
			أرجع 0;
		}
	}

	/*
	 * Handle memory allocation outside of the cache
	 * accounting lock.  Compiler will optimize the strangeness
	 * away when NO_PTHREADS is defined.
	 */
	حرر(trg_داخلا->delta_معلومات);
	cache_تالو();
	إذا (trg_داخلا->delta_معلومات) {
		delta_cache_سائيز -= DELTA_سائيز(trg_داخلا);
		trg_داخلا->delta_معلومات = عدم;
	}
	إذا (delta_cacheable(src_سائيز, trg_سائيز, delta_سائيز_2)) {
		delta_cache_سائيز += delta_سائيز_2;
		cache_unlock();
		trg_داخلا->delta_معلومات = xrealloc(delta_buf, delta_سائيز_2);
	} وإلا {
		cache_unlock();
		حرر(delta_buf);
	}

	سيٽ_DELTA(trg_داخلا, src_داخلا);
	سيٽ_DELTA_سائيز(trg_داخلا, delta_سائيز_2);
	trg->اونهائي = src->اونهائي + 1;

	أرجع 1;
}

ساكن بنانشان صحيح check_delta_حد_بندي(بنية object_داخلا *me, بنانشان صحيح n)
{
	بنية object_داخلا *ٻار = DELTA_ٻار(me);
	بنانشان صحيح m = n;
	طالما (ٻار) {
		ثابت بنانشان صحيح c = check_delta_حد_بندي(ٻار, n + 1);
		إذا (m < c)
			m = c;
		ٻار = DELTA_SIBLING(ٻار);
	}
	أرجع m;
}

ساكن سائيز_t free_unpacked(بنية unpacked *n)
{
	سائيز_t freed_mem = sizeof_delta_اشاريو(n->اشاريو);
	free_delta_اشاريو(n->اشاريو);
	n->اشاريو = عدم;
	إذا (n->معلومات) {
		freed_mem += سائيز_3(n->داخلا);
		FREE_AND_NULL(n->معلومات);
	}
	n->داخلا = عدم;
	n->اونهائي = 0;
	أرجع freed_mem;
}

ساكن فراغ ڳوليو_deltas(بنية object_داخلا **list, بنانشان *list_سائيز,
			صحيح دري, صحيح اونهائي, بنانشان *processed)
{
	uint32_t i, idx = 0, ڳڻپ = 0;
	بنية unpacked *array;
	سائيز_t mem_usage = 0;

	CALLOC_ARRAY(array, دري);

	لكل (;;) {
		بنية object_داخلا *داخلا;
		بنية unpacked *n = array + idx;
		صحيح j, max_اونهائي, بهترين_بنياد = -1;

		ترقي_تالو();
		إذا (!*list_سائيز) {
			ترقي_unlock();
			اكسر;
		}
		داخلا = *list++;
		(*list_سائيز)--;
		إذا (!داخلا->preferred_بنياد) {
			(*processed)++;
			display_ترقي(ترقي_state, *processed);
		}
		ترقي_unlock();

		mem_usage -= free_unpacked(n);
		n->داخلا = داخلا;

		طالما (دري_يادگيري_حد_بندي &&
		       mem_usage > دري_يادگيري_حد_بندي &&
		       ڳڻپ > 1) {
			ثابت uint32_t پڇ = (idx + دري - ڳڻپ) % دري;
			mem_usage -= free_unpacked(array + پڇ);
			ڳڻپ--;
		}

		/* We do not compute delta to *create* objects we are not
		 * going to pack.
		 */
		إذا (داخلا->preferred_بنياد)
			اذهب ايندڙ;

		/*
		 * If the current object is at pack edge, take the depth the
		 * objects that depend on the current object into account
		 * otherwise they would become too deep.
		 */
		max_اونهائي = اونهائي;
		إذا (DELTA_ٻار(داخلا)) {
			max_اونهائي -= check_delta_حد_بندي(داخلا, 0);
			إذا (max_اونهائي <= 0)
				اذهب ايندڙ;
		}

		j = دري;
		طالما (--j > 0) {
			صحيح ret;
			uint32_t other_idx = idx + j;
			بنية unpacked *m;
			إذا (other_idx >= دري)
				other_idx -= دري;
			m = array + other_idx;
			إذا (!m->داخلا)
				اكسر;
			ret = try_delta(n, m, max_اونهائي, &mem_usage);
			إذا (ret < 0)
				اكسر;
			وإلا إذا (ret > 0)
				بهترين_بنياد = other_idx;
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
		إذا (داخلا->delta_معلومات && !pack_منزل_stdout) {
			بنانشان ڊگهو سائيز;

			سائيز = do_compress(&داخلا->delta_معلومات, DELTA_سائيز(داخلا));
			إذا (سائيز < (1U << OE_Z_DELTA_بٽ)) {
				داخلا->z_delta_سائيز = سائيز;
				cache_تالو();
				delta_cache_سائيز -= DELTA_سائيز(داخلا);
				delta_cache_سائيز += داخلا->z_delta_سائيز;
				cache_unlock();
			} وإلا {
				FREE_AND_NULL(داخلا->delta_معلومات);
				داخلا->z_delta_سائيز = 0;
			}
		}

		/* if we made n a delta, and if n is already at max
		 * depth, leaving it in the window is pointless.  we
		 * should evict it first.
		 */
		إذا (DELTA(داخلا) && max_اونهائي <= n->اونهائي)
			استمر;

		/*
		 * Move the best delta base up in the window, after the
		 * currently deltified object, to keep it longer.  It will
		 * be the first base object to be attempted next.
		 */
		إذا (DELTA(داخلا)) {
			بنية unpacked مٽا_سٽا_2 = array[بهترين_بنياد];
			صحيح dist = (دري + idx - بهترين_بنياد) % دري;
			صحيح dst = بهترين_بنياد;
			طالما (dist--) {
				صحيح src = (dst + 1) % دري;
				array[dst] = array[src];
				dst = src;
			}
			array[dst] = مٽا_سٽا_2;
		}

		ايندڙ:
		idx++;
		إذا (ڳڻپ + 1 < دري)
			ڳڻپ++;
		إذا (idx >= دري)
			idx = 0;
	}

	لكل (i = 0; i < دري; ++i) {
		free_delta_اشاريو(array[i].اشاريو);
		حرر(array[i].معلومات);
	}
	حرر(array);
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

بنية thread_params {
	pthread_t thread;
	بنية object_داخلا **list;
	بنية packing_region *regions;
	بنانشان list_سائيز;
	بنانشان بچيل;
	صحيح دري;
	صحيح اونهائي;
	صحيح working;
	صحيح معلومات_ready;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	بنانشان *processed;
};

ساكن pthread_cond_t ترقي_cond;

/*
 * Mutex and conditional variable can't be statically-initialized on Windows.
 */
ساكن فراغ init_threaded_ڳولا(فراغ)
{
	pthread_mutex_init(&cache_mutex, عدم);
	pthread_mutex_init(&ترقي_mutex, عدم);
	pthread_cond_init(&ترقي_cond, عدم);
}

ساكن فراغ cleanup_threaded_ڳولا(فراغ)
{
	pthread_cond_destroy(&ترقي_cond);
	pthread_mutex_destroy(&cache_mutex);
	pthread_mutex_destroy(&ترقي_mutex);
}

ساكن فراغ *threaded_ڳوليو_deltas(فراغ *arg)
{
	بنية thread_params *me = arg;

	ترقي_تالو();
	طالما (me->بچيل) {
		ترقي_unlock();

		ڳوليو_deltas(me->list, &me->بچيل,
			    me->دري, me->اونهائي, me->processed);

		ترقي_تالو();
		me->working = 0;
		pthread_cond_signal(&ترقي_cond);
		ترقي_unlock();

		/*
		 * We must not set ->data_ready before we wait on the
		 * condition because the main thread may have set it to 1
		 * before we get here. In order to be sure that new
		 * work is available if we see 1 in ->data_ready, it
		 * was initialized to 0 before this thread was spawned
		 * and we reset it to 0 right away.
		 */
		pthread_mutex_تالو(&me->mutex);
		طالما (!me->معلومات_ready)
			pthread_cond_wait(&me->cond, &me->mutex);
		me->معلومات_ready = 0;
		pthread_mutex_unlock(&me->mutex);

		ترقي_تالو();
	}
	ترقي_unlock();
	/* leave ->working 1 so that this doesn't get more work assigned */
	أرجع عدم;
}

ساكن فراغ ll_ڳوليو_deltas(بنية object_داخلا **list, بنانشان list_سائيز,
			   صحيح دري, صحيح اونهائي, بنانشان *processed)
{
	بنية thread_params *p;
	صحيح i, ret, سرگرم_ڌاڳا = 0;

	init_threaded_ڳولا();

	إذا (delta_ڳولا_ڌاڳا <= 1) {
		ڳوليو_deltas(list, &list_سائيز, دري, اونهائي, processed);
		cleanup_threaded_ڳولا();
		أرجع;
	}
	إذا (ترقي > pack_منزل_stdout)
		fprintf_ln(stderr, _("Delta compression using up to %d threads"),
			   delta_ڳولا_ڌاڳا);
	CALLOC_ARRAY(p, delta_ڳولا_ڌاڳا);

	/* Partition the work amongst work threads. */
	لكل (i = 0; i < delta_ڳولا_ڌاڳا; i++) {
		بنانشان sub_سائيز = list_سائيز / (delta_ڳولا_ڌاڳا - i);

		/* don't use too small segments or no deltas will be found */
		إذا (sub_سائيز < 2*دري && i+1 < delta_ڳولا_ڌاڳا)
			sub_سائيز = 0;

		p[i].دري = دري;
		p[i].اونهائي = اونهائي;
		p[i].processed = processed;
		p[i].working = 1;
		p[i].معلومات_ready = 0;

		/* try to split chunks on "path" boundaries */
		طالما (sub_سائيز && sub_سائيز < list_سائيز &&
		       list[sub_سائيز]->hash &&
		       list[sub_سائيز]->hash == list[sub_سائيز-1]->hash)
			sub_سائيز++;

		p[i].list = list;
		p[i].list_سائيز = sub_سائيز;
		p[i].بچيل = sub_سائيز;

		list += sub_سائيز;
		list_سائيز -= sub_سائيز;
	}

	/* Start work threads. */
	لكل (i = 0; i < delta_ڳولا_ڌاڳا; i++) {
		إذا (!p[i].list_سائيز)
			استمر;
		pthread_mutex_init(&p[i].mutex, عدم);
		pthread_cond_init(&p[i].cond, عدم);
		ret = pthread_create(&p[i].thread, عدم,
				     threaded_ڳوليو_deltas, &p[i]);
		إذا (ret)
			die(_("unable to create thread: %s"), strerror(ret));
		سرگرم_ڌاڳا++;
	}

	/*
	 * Now let's wait for work completion.  Each time a thread is done
	 * with its work, we steal half of the remaining work from the
	 * thread with the largest number of unprocessed objects and give
	 * it to that newly idle thread.  This ensure good load balancing
	 * until the remaining object list segments are simply too short
	 * to be worth splitting anymore.
	 */
	طالما (سرگرم_ڌاڳا) {
		بنية thread_params *هدف = عدم;
		بنية thread_params *victim = عدم;
		بنانشان sub_سائيز = 0;

		ترقي_تالو();
		لكل (;;) {
			لكل (i = 0; !هدف && i < delta_ڳولا_ڌاڳا; i++)
				إذا (!p[i].working)
					هدف = &p[i];
			إذا (هدف)
				اكسر;
			pthread_cond_wait(&ترقي_cond, &ترقي_mutex);
		}

		لكل (i = 0; i < delta_ڳولا_ڌاڳا; i++)
			إذا (p[i].بچيل > 2*دري &&
			    (!victim || victim->بچيل < p[i].بچيل))
				victim = &p[i];
		إذا (victim) {
			sub_سائيز = victim->بچيل / 2;
			list = victim->list + victim->list_سائيز - sub_سائيز;
			طالما (sub_سائيز && list[0]->hash &&
			       list[0]->hash == list[-1]->hash) {
				list++;
				sub_سائيز--;
			}
			إذا (!sub_سائيز) {
				/*
				 * It is possible for some "paths" to have
				 * so many objects that no hash boundary
				 * might be found.  Let's just steal the
				 * exact half in that case.
				 */
				sub_سائيز = victim->بچيل / 2;
				list -= sub_سائيز;
			}
			هدف->list = list;
			victim->list_سائيز -= sub_سائيز;
			victim->بچيل -= sub_سائيز;
		}
		هدف->list_سائيز = sub_سائيز;
		هدف->بچيل = sub_سائيز;
		هدف->working = 1;
		ترقي_unlock();

		pthread_mutex_تالو(&هدف->mutex);
		هدف->معلومات_ready = 1;
		pthread_cond_signal(&هدف->cond);
		pthread_mutex_unlock(&هدف->mutex);

		إذا (!sub_سائيز) {
			pthread_join(هدف->thread, عدم);
			pthread_cond_destroy(&هدف->cond);
			pthread_mutex_destroy(&هدف->mutex);
			سرگرم_ڌاڳا--;
		}
	}
	cleanup_threaded_ڳولا();
	حرر(p);
}

ساكن صحيح obj_is_packed(ثابت بنية object_id *oid)
{
	أرجع packlist_ڳوليو(&منزل_pack, oid) ||
		(reuse_packfile_bitmap &&
		 bitmap_walk_شامل_آهي(bitmap_git, reuse_packfile_bitmap, oid));
}

ساكن فراغ جوڙيو_ٽيگ_chain(ثابت بنية object_id *oid)
{
	بنية ٽيگ *ٽيگ;

	/*
	 * We catch duplicates already in add_object_entry(), but we'd
	 * prefer to do this extra check to avoid having to parse the
	 * tag at all if we already know that it's being packed (e.g., if
	 * it was included via bitmaps, we would not have parsed it
	 * previously).
	 */
	إذا (obj_is_packed(oid))
		أرجع;

	ٽيگ = lookup_ٽيگ(the_repository, oid);
	طالما (1) {
		إذا (!ٽيگ || parse_ٽيگ(the_repository, ٽيگ) || !ٽيگ->tagged)
			die(_("unable to pack objects reachable from tag %s"),
			    oid_منزل_hex(oid));

		جوڙيو_object_داخلا(&ٽيگ->object.oid, OBJ_ٽيگ, عدم, 0);

		إذا (ٽيگ->tagged->type != OBJ_ٽيگ)
			أرجع;

		ٽيگ = (بنية ٽيگ *)ٽيگ->tagged;
	}
}

ساكن صحيح جوڙيو_ref_ٽيگ(ثابت بنية reference *ref, فراغ *cb_معلومات اڻ_استعمال_ٿيل)
{
	بنية object_id peeled;

	إذا (!reference_get_peeled_oid(the_repository, ref, &peeled) &&
	    obj_is_packed(&peeled))
		جوڙيو_ٽيگ_chain(ref->oid);
	أرجع 0;
}

ساكن صحيح should_attempt_deltas(بنية object_داخلا *داخلا)
{
	إذا (DELTA(داخلا))
		/* This happens if we decided to reuse existing
		 * delta from a pack. "reuse_delta &&" is implied.
		 */
		أرجع 0;

	إذا (!داخلا->type_valid ||
	    oe_سائيز_less_than(&منزل_pack, داخلا, 50))
		أرجع 0;

	إذا (داخلا->no_try_delta)
		أرجع 0;

	إذا (!داخلا->preferred_بنياد) {
		إذا (oe_type(داخلا) < 0)
			die(_("unable to get type of object %s"),
				oid_منزل_hex(&داخلا->idx.oid));
	} وإلا إذا (oe_type(داخلا) < 0) {
		/*
		 * This object is not found, but we
		 * don't have to include it anyway.
		 */
		أرجع 0;
	}

	أرجع 1;
}

ساكن فراغ ڳوليو_deltas_for_region(بنية object_داخلا *list,
				   بنية packing_region *region,
				   بنانشان صحيح *processed)
{
	بنية object_داخلا **delta_list;
	بنانشان صحيح delta_list_nr = 0;

	ALLOC_ARRAY(delta_list, region->nr);
	لكل (سائيز_t i = 0; i < region->nr; i++) {
		بنية object_داخلا *داخلا = list + region->شروعات + i;
		إذا (should_attempt_deltas(داخلا))
			delta_list[delta_list_nr++] = داخلا;
	}

	QSORT(delta_list, delta_list_nr, type_سائيز_ترتيب_ڏيو);
	ڳوليو_deltas(delta_list, &delta_list_nr, دري, اونهائي, processed);
	حرر(delta_list);
}

ساكن فراغ ڳوليو_deltas_by_region(بنية object_داخلا *list,
				  بنية packing_region *regions,
				  سائيز_t شروعات, سائيز_t nr)
{
	بنانشان صحيح processed = 0;
	سائيز_t ترقي_nr;

	إذا (!nr)
		أرجع;

	ترقي_nr = regions[nr - 1].شروعات + regions[nr - 1].nr;

	إذا (ترقي)
		ترقي_state = شروعات_ترقي(the_repository,
						_("Compressing objects by path"),
						ترقي_nr);

	طالما (nr--)
		ڳوليو_deltas_for_region(list,
				       &regions[شروعات++],
				       &processed);

	display_ترقي(ترقي_state, ترقي_nr);
	stop_ترقي(&ترقي_state);
}

ساكن فراغ *threaded_ڳوليو_deltas_by_رستو(فراغ *arg)
{
	بنية thread_params *me = arg;

	ترقي_تالو();
	طالما (me->بچيل) {
		طالما (me->بچيل) {
			ترقي_unlock();
			ڳوليو_deltas_for_region(منزل_pack.objects,
					       me->regions,
					       me->processed);
			ترقي_تالو();
			me->بچيل--;
			me->regions++;
		}

		me->working = 0;
		pthread_cond_signal(&ترقي_cond);
		ترقي_unlock();

		/*
		 * We must not set ->data_ready before we wait on the
		 * condition because the main thread may have set it to 1
		 * before we get here. In order to be sure that new
		 * work is available if we see 1 in ->data_ready, it
		 * was initialized to 0 before this thread was spawned
		 * and we reset it to 0 right away.
		 */
		pthread_mutex_تالو(&me->mutex);
		طالما (!me->معلومات_ready)
			pthread_cond_wait(&me->cond, &me->mutex);
		me->معلومات_ready = 0;
		pthread_mutex_unlock(&me->mutex);

		ترقي_تالو();
	}
	ترقي_unlock();
	/* leave ->working 1 so that this doesn't get more work assigned */
	أرجع عدم;
}

ساكن فراغ ll_ڳوليو_deltas_by_region(بنية object_داخلا *list,
				     بنية packing_region *regions,
				     uint32_t شروعات, uint32_t nr)
{
	بنية thread_params *p;
	صحيح i, ret, سرگرم_ڌاڳا = 0;
	بنانشان صحيح processed = 0;
	uint32_t ترقي_nr;
	init_threaded_ڳولا();

	إذا (!nr)
		أرجع;

	ترقي_nr =  regions[nr - 1].شروعات + regions[nr - 1].nr;
	إذا (delta_ڳولا_ڌاڳا <= 1) {
		ڳوليو_deltas_by_region(list, regions, شروعات, nr);
		cleanup_threaded_ڳولا();
		أرجع;
	}

	إذا (ترقي > pack_منزل_stdout)
		fprintf_ln(stderr,
			   Q_("Path-based delta compression using up to %d thread",
			      "Path-based delta compression using up to %d threads",
			      delta_ڳولا_ڌاڳا),
			   delta_ڳولا_ڌاڳا);
	CALLOC_ARRAY(p, delta_ڳولا_ڌاڳا);

	إذا (ترقي)
		ترقي_state = شروعات_ترقي(the_repository,
						_("Compressing objects by path"),
						ترقي_nr);
	/* Partition the work amongst work threads. */
	لكل (i = 0; i < delta_ڳولا_ڌاڳا; i++) {
		بنانشان sub_سائيز = nr / (delta_ڳولا_ڌاڳا - i);

		p[i].دري = دري;
		p[i].اونهائي = اونهائي;
		p[i].processed = &processed;
		p[i].working = 1;
		p[i].معلومات_ready = 0;

		p[i].regions = regions;
		p[i].list_سائيز = sub_سائيز;
		p[i].بچيل = sub_سائيز;

		regions += sub_سائيز;
		nr -= sub_سائيز;
	}

	/* Start work threads. */
	لكل (i = 0; i < delta_ڳولا_ڌاڳا; i++) {
		إذا (!p[i].list_سائيز)
			استمر;
		pthread_mutex_init(&p[i].mutex, عدم);
		pthread_cond_init(&p[i].cond, عدم);
		ret = pthread_create(&p[i].thread, عدم,
				     threaded_ڳوليو_deltas_by_رستو, &p[i]);
		إذا (ret)
			die(_("unable to create thread: %s"), strerror(ret));
		سرگرم_ڌاڳا++;
	}

	/*
	 * Now let's wait for work completion.  Each time a thread is done
	 * with its work, we steal half of the remaining work from the
	 * thread with the largest number of unprocessed objects and give
	 * it to that newly idle thread.  This ensure good load balancing
	 * until the remaining object list segments are simply too short
	 * to be worth splitting anymore.
	 */
	طالما (سرگرم_ڌاڳا) {
		بنية thread_params *هدف = عدم;
		بنية thread_params *victim = عدم;
		بنانشان sub_سائيز = 0;

		ترقي_تالو();
		لكل (;;) {
			لكل (i = 0; !هدف && i < delta_ڳولا_ڌاڳا; i++)
				إذا (!p[i].working)
					هدف = &p[i];
			إذا (هدف)
				اكسر;
			pthread_cond_wait(&ترقي_cond, &ترقي_mutex);
		}

		لكل (i = 0; i < delta_ڳولا_ڌاڳا; i++)
			إذا (p[i].بچيل > 2*دري &&
			    (!victim || victim->بچيل < p[i].بچيل))
				victim = &p[i];
		إذا (victim) {
			sub_سائيز = victim->بچيل / 2;
			هدف->regions = victim->regions + victim->بچيل - sub_سائيز;
			victim->list_سائيز -= sub_سائيز;
			victim->بچيل -= sub_سائيز;
		}
		هدف->list_سائيز = sub_سائيز;
		هدف->بچيل = sub_سائيز;
		هدف->working = 1;
		ترقي_unlock();

		pthread_mutex_تالو(&هدف->mutex);
		هدف->معلومات_ready = 1;
		pthread_cond_signal(&هدف->cond);
		pthread_mutex_unlock(&هدف->mutex);

		إذا (!sub_سائيز) {
			pthread_join(هدف->thread, عدم);
			pthread_cond_destroy(&هدف->cond);
			pthread_mutex_destroy(&هدف->mutex);
			سرگرم_ڌاڳا--;
		}
	}
	cleanup_threaded_ڳولا();
	حرر(p);

	display_ترقي(ترقي_state, ترقي_nr);
	stop_ترقي(&ترقي_state);
}

ساكن فراغ prepare_pack(صحيح دري, صحيح اونهائي)
{
	بنية object_داخلا **delta_list;
	uint32_t i, nr_deltas;
	بنانشان n;

	إذا (use_delta_islands)
		resolve_وڻ_islands(the_repository, ترقي, &منزل_pack);

	get_object_details();

	/*
	 * If we're locally repacking then we need to be doubly careful
	 * from now on in order to make sure no stealth corruption gets
	 * propagated to the new pack.  Clients receiving streamed packs
	 * should validate everything they get anyway so no need to incur
	 * the additional cost here in that case.
	 */
	إذا (!pack_منزل_stdout)
		do_check_packed_object_چڪري_جاچ = 1;

	إذا (!منزل_pack.nr_objects || !دري || !اونهائي)
		أرجع;

	إذا (رستو_walk)
		ll_ڳوليو_deltas_by_region(منزل_pack.objects, منزل_pack.regions,
					 0, منزل_pack.nr_regions);

	ALLOC_ARRAY(delta_list, منزل_pack.nr_objects);
	nr_deltas = n = 0;

	لكل (i = 0; i < منزل_pack.nr_objects; i++) {
		بنية object_داخلا *داخلا = منزل_pack.objects + i;

		إذا (!should_attempt_deltas(داخلا))
			استمر;

		إذا (!داخلا->preferred_بنياد)
			nr_deltas++;

		delta_list[n++] = داخلا;
	}

	إذا (nr_deltas && n > 1) {
		بنانشان nr_پورو = 0;

		إذا (ترقي)
			ترقي_state = شروعات_ترقي(the_repository,
							_("Compressing objects"),
							nr_deltas);
		QSORT(delta_list, n, type_سائيز_ترتيب_ڏيو);
		ll_ڳوليو_deltas(delta_list, n, دري+1, اونهائي, &nr_پورو);
		stop_ترقي(&ترقي_state);
		إذا (nr_پورو != nr_deltas)
			die(_("inconsistency with delta count"));
	}
	حرر(delta_list);
}

ساكن صحيح git_pack_config(ثابت حرف *k, ثابت حرف *v,
			   ثابت بنية config_context *ctx, فراغ *cb)
{
	إذا (!strcmp(k, "pack.window")) {
		دري = git_config_int(k, v, ctx->kvi);
		أرجع 0;
	}
	إذا (!strcmp(k, "pack.windowmemory")) {
		دري_يادگيري_حد_بندي = git_config_ulong(k, v, ctx->kvi);
		أرجع 0;
	}
	إذا (!strcmp(k, "pack.depth")) {
		اونهائي = git_config_int(k, v, ctx->kvi);
		أرجع 0;
	}
	إذا (!strcmp(k, "pack.deltacachesize")) {
		max_delta_cache_سائيز = git_config_سائيز_t(k, v, ctx->kvi);
		أرجع 0;
	}
	إذا (!strcmp(k, "pack.deltacachelimit")) {
		cache_max_small_delta_سائيز = git_config_int(k, v, ctx->kvi);
		أرجع 0;
	}
	إذا (!strcmp(k, "pack.writebitmaphashcache")) {
		إذا (git_config_bool(k, v))
			write_bitmap_options |= BITMAP_OPT_HASH_CACHE;
		وإلا
			write_bitmap_options &= ~BITMAP_OPT_HASH_CACHE;
	}

	إذا (!strcmp(k, "pack.writebitmaplookuptable")) {
		إذا (git_config_bool(k, v))
			write_bitmap_options |= BITMAP_OPT_LOOKUP_جدول;
		وإلا
			write_bitmap_options &= ~BITMAP_OPT_LOOKUP_جدول;
	}

	إذا (!strcmp(k, "pack.usebitmaps")) {
		use_bitmap_اشاريو_default = git_config_bool(k, v);
		أرجع 0;
	}
	إذا (!strcmp(k, "pack.allowpackreuse")) {
		صحيح res = git_parse_maybe_bool_متن(v);
		إذا (res < 0) {
			إذا (!strcasecmp(v, "single"))
				allow_pack_reuse = SINGLE_PACK_REUSE;
			وإلا إذا (!strcasecmp(v, "multi"))
				allow_pack_reuse = MULTI_PACK_REUSE;
			وإلا
				die(_("invalid pack.allowPackReuse value: '%s'"), v);
		} وإلا إذا (res) {
			allow_pack_reuse = SINGLE_PACK_REUSE;
		} وإلا {
			allow_pack_reuse = NO_PACK_REUSE;
		}
		أرجع 0;
	}
	إذا (!strcmp(k, "pack.threads")) {
		delta_ڳولا_ڌاڳا = git_config_int(k, v, ctx->kvi);
		إذا (delta_ڳولا_ڌاڳا < 0)
			die(_("invalid number of threads specified (%d)"),
			    delta_ڳولا_ڌاڳا);
		إذا (!HAVE_ڌاڳا && delta_ڳولا_ڌاڳا != 1) {
			warning(_("no threads support, ignoring %s"), k);
			delta_ڳولا_ڌاڳا = 0;
		}
		أرجع 0;
	}
	إذا (!strcmp(k, "pack.indexversion")) {
		pack_idx_opts.نسخو = git_config_int(k, v, ctx->kvi);
		إذا (pack_idx_opts.نسخو > 2)
			die(_("bad pack.indexVersion=%"PRIu32),
			    pack_idx_opts.نسخو);
		أرجع 0;
	}
	إذا (!strcmp(k, "pack.writereverseindex")) {
		إذا (git_config_bool(k, v))
			pack_idx_opts.flags |= WRITE_REV;
		وإلا
			pack_idx_opts.flags &= ~WRITE_REV;
		أرجع 0;
	}
	إذا (!strcmp(k, "uploadpack.blobpackfileuri")) {
		بنية configured_exclusion *ex;
		ثابت حرف *oid_پڇاڙي, *pack_پڇاڙي;
		/*
		 * Stores the pack hash. This is not a true object ID, but is
		 * of the same form.
		 */
		بنية object_id pack_hash;

		إذا (!v)
			أرجع config_error_nonbool(k);

		ex = xmalloc(حجم(*ex));
		إذا (parse_oid_hex(v, &ex->e.oid, &oid_پڇاڙي) ||
		    *oid_پڇاڙي != ' ' ||
		    parse_oid_hex(oid_پڇاڙي + 1, &pack_hash, &pack_پڇاڙي) ||
		    *pack_پڇاڙي != ' ')
			die(_("value of uploadpack.blobpackfileuri must be "
			      "of the form '<object-hash> <pack-hash> <uri>' (got '%s')"), v);
		إذا (oidmap_get(&configured_exclusions, &ex->e.oid))
			die(_("object already configured in another "
			      "uploadpack.blobpackfileuri (got '%s')"), v);
		ex->pack_hash_hex = xcalloc(1, pack_پڇاڙي - oid_پڇاڙي);
		يادگيري_نقل(ex->pack_hash_hex, oid_پڇاڙي + 1, pack_پڇاڙي - oid_پڇاڙي - 1);
		ex->uri = xstrdup(pack_پڇاڙي + 1);
		oidmap_put(&configured_exclusions, ex);
	}
	أرجع git_default_config(k, v, ctx, cb);
}

/* Counters for trace2 output when in --stdin-packs mode. */
ساكن صحيح stdin_packs_مليو_nr;
ساكن صحيح stdin_packs_hints_nr;

ساكن صحيح جوڙيو_object_داخلا_from_pack(ثابت بنية object_id *oid,
				      بنية packed_git *p,
				      uint32_t pos,
				      فراغ *معلومات_2)
{
	off_t ofs;
	بنية object_info oi = OBJECT_INFO_INIT;
	تعداد object_type type = OBJ_NONE;

	display_ترقي(ترقي_state, ++nr_ڏٺل);

	إذا (have_duplicate_داخلا(oid, 0))
		أرجع 0;

	stdin_packs_مليو_nr++;

	ofs = nth_packed_object_وٿي(p, pos);

	oi.typep = &type;
	إذا (packed_object_info(عدم, p, ofs, &oi) < 0) {
		die(_("could not get type of object %s in pack %s"),
		    oid_منزل_hex(oid), p->pack_name);
	} وإلا إذا (type == OBJ_COMMIT) {
		بنية rev_info *revs = معلومات_2;
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
		جوڙيو_pending_oid(revs, عدم, oid, 0);
	}

	إذا (!want_object_اندريون_pack(oid, 0, &p, &ofs))
		أرجع 0;

	create_object_داخلا(oid, type, 0, 0, 0, p, ofs);

	أرجع 0;
}

ساكن فراغ show_object_pack_hint(بنية object *object, ثابت حرف *name,
				  فراغ *معلومات)
{
	تعداد stdin_packs_گهڻائي گهڻائي = *(تعداد stdin_packs_گهڻائي *)معلومات;
	إذا (گهڻائي == STDIN_PACKS_گهڻائي_FOLLOW) {
		إذا (object->type == OBJ_BLOB &&
		    !odb_has_object(the_repository->objects, &object->oid, 0))
			أرجع;
		جوڙيو_object_داخلا(&object->oid, object->type, name, 0);
	} وإلا {
		بنية object_داخلا *oe = packlist_ڳوليو(&منزل_pack, &object->oid);
		إذا (!oe)
			أرجع;

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

ساكن فراغ show_commit_pack_hint(بنية commit *commit, فراغ *معلومات)
{
	تعداد stdin_packs_گهڻائي گهڻائي = *(تعداد stdin_packs_گهڻائي *)معلومات;

	إذا (گهڻائي == STDIN_PACKS_گهڻائي_FOLLOW) {
		show_object_pack_hint((بنية object *)commit, "", معلومات);
		أرجع;
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
تعداد stdin_pack_info_قسم {
	STDIN_PACK_INCLUDE = (1<<0),
	STDIN_PACK_EXCLUDE_CLOSED = (1<<1),
	STDIN_PACK_EXCLUDE_OPEN = (1<<2),
};

بنية stdin_pack_info {
	بنية packed_git *p;
	تعداد stdin_pack_info_قسم قسم;
};

ساكن صحيح pack_mtime_cmp(ثابت فراغ *_a, ثابت فراغ *_b)
{
	بنية stdin_pack_info *a = ((ثابت بنية string_list_شي*)_a)->util;
	بنية stdin_pack_info *b = ((ثابت بنية string_list_شي*)_b)->util;

	/*
	 * order packs by descending mtime so that objects are laid out
	 * roughly as newest-to-oldest
	 */
	إذا (a->p->mtime < b->p->mtime)
		أرجع 1;
	وإلا إذا (b->p->mtime < a->p->mtime)
		أرجع -1;
	وإلا
		أرجع 0;
}

ساكن صحيح stdin_packs_include_check_obj(بنية object *obj, فراغ *معلومات اڻ_استعمال_ٿيل)
{
	أرجع !has_object_kept_pack(منزل_pack.repo, &obj->oid,
				     KEPT_PACK_اندريون_CORE);
}

ساكن صحيح stdin_packs_include_check(بنية commit *commit, فراغ *معلومات)
{
	أرجع stdin_packs_include_check_obj((بنية object *)commit, معلومات);
}

ساكن فراغ stdin_packs_جوڙيو_pack_entries(بنية strmap *packs,
					 بنية rev_info *revs)
{
	بنية string_list ڪنجيون = STRING_LIST_INIT_NODUP;
	بنية string_list_شي *شي;
	بنية hashmap_iter iter;
	بنية strmap_داخلا *داخلا;

	strmap_for_each_داخلا(packs, &iter, داخلا) {
		بنية stdin_pack_info *info = داخلا->قدر;
		إذا (!info->p)
			die(_("could not find pack '%s'"), داخلا->ڪنجي);

		string_list_append(&ڪنجيون, داخلا->ڪنجي)->util = info;
	}

	/*
	 * Order packs by ascending mtime; use QSORT directly to access the
	 * string_list_item's ->util pointer, which string_list_sort() does not
	 * provide.
	 */
	QSORT(ڪنجيون.شيون, ڪنجيون.nr, pack_mtime_cmp);

	for_each_string_list_شي(شي, &ڪنجيون) {
		بنية stdin_pack_info *info = شي->util;

		إذا (info->قسم & STDIN_PACK_EXCLUDE_OPEN) {
			/*
			 * When open-excluded packs ("!") are present, stop
			 * the parent walk at closed-excluded ("^") packs.
			 * Objects behind a "^" boundary are guaranteed to
			 * have closure and should not be rescued.
			 */
			revs->include_check = stdin_packs_include_check;
			revs->include_check_obj = stdin_packs_include_check_obj;
		}

		إذا ((info->قسم & STDIN_PACK_INCLUDE) ||
		    (info->قسم & STDIN_PACK_EXCLUDE_OPEN))
			for_each_object_اندريون_pack(info->p,
						جوڙيو_object_داخلا_from_pack,
						revs,
						ODB_FOR_EACH_OBJECT_PACK_ترتيب);
	}

	string_list_clear(&ڪنجيون, 0);
}

ساكن فراغ stdin_packs_read_داخلا(بنية rev_info *revs,
				   تعداد stdin_packs_گهڻائي گهڻائي)
{
	بنية strbuf buf = STRBUF_INIT;
	بنية strmap packs = STRMAP_INIT;
	بنية packed_git *p;

	طالما (strbuf_getline(&buf, stdin) != EOF) {
		بنية stdin_pack_info *info;
		تعداد stdin_pack_info_قسم قسم = STDIN_PACK_INCLUDE;
		ثابت حرف *ڪنجي = buf.buf;

		إذا (!*ڪنجي)
			استمر;
		وإلا إذا (*ڪنجي == '^')
			قسم = STDIN_PACK_EXCLUDE_CLOSED;
		وإلا إذا (*ڪنجي == '!' && گهڻائي == STDIN_PACKS_گهڻائي_FOLLOW)
			قسم = STDIN_PACK_EXCLUDE_OPEN;

		إذا (قسم != STDIN_PACK_INCLUDE)
			ڪنجي++;

		info = strmap_get(&packs, ڪنجي);
		إذا (!info) {
			CALLOC_ARRAY(info, 1);
			strmap_put(&packs, ڪنجي, info);
		}

		info->قسم |= قسم;

		strbuf_reset(&buf);
	}

	repo_for_each_pack(the_repository, p) {
		بنية stdin_pack_info *info;

		info = strmap_get(&packs, pack_basename(p));
		إذا (!info)
			استمر;

		إذا (info->قسم & STDIN_PACK_INCLUDE) {
			إذا (exclude_promisor_objects && p->pack_promisor)
				die(_("packfile %s is a promisor but --exclude-promisor-objects was given"), p->pack_name);

			/*
			 * Arguments we got on stdin may not even be
			 * packs. First check that to avoid segfaulting
			 * later on in e.g.  pack_mtime_cmp(), excluded
			 * packs are handled below.
			 */
			إذا (!is_pack_valid(p))
				die(_("packfile %s cannot be accessed"), p->pack_name);
		}

		إذا (info->قسم & STDIN_PACK_EXCLUDE_CLOSED) {
			/*
			 * Marking excluded packs as kept in-core so
			 * that later calls to add_object_entry()
			 * discards any objects that are also found in
			 * excluded packs.
			 */
			p->pack_keep_اندريون_core = 1;
		}

		إذا (info->قسم & STDIN_PACK_EXCLUDE_OPEN) {
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
			p->pack_keep_اندريون_core_open = 1;
		}

		info->p = p;
	}

	stdin_packs_جوڙيو_pack_entries(&packs, revs);

	strbuf_release(&buf);
	strmap_clear(&packs, 1);
}

ساكن فراغ جوڙيو_unreachable_loose_objects(بنية rev_info *revs);

ساكن فراغ read_stdin_packs(بنية repository *repo,
			     تعداد stdin_packs_گهڻائي گهڻائي, صحيح rev_list_unpacked)
{
	صحيح prev_fetch_if_missing = repo->fetch_if_missing;
	بنية rev_info revs;

	/*
	 * The revision walk may hit objects that are promised, only. As the
	 * walk is best-effort though we don't want to perform backfill fetches
	 * for them.
	 */
	repo->fetch_if_missing = 0;

	repo_init_revisions(repo, &revs, عدم);
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
	revs.keep_pack_cache_flags |= KEPT_PACK_اندريون_CORE;
	revs.blob_objects = 1;
	revs.وڻ_objects = 1;
	revs.ٽيگ_objects = 1;
	revs.ignore_missing_links = 1;
	revs.exclude_promisor_objects = exclude_promisor_objects;

	/* avoids adding objects in excluded packs */
	ignore_packed_keep_اندريون_core = 1;
	إذا (گهڻائي == STDIN_PACKS_گهڻائي_FOLLOW) {
		/*
		 * In '--stdin-packs=follow' mode, additionally ignore
		 * objects in excluded-open packs to prevent them from
		 * appearing in the resulting pack.
		 */
		ignore_packed_keep_اندريون_core_open = 1;
	}
	stdin_packs_read_داخلا(&revs, گهڻائي);
	إذا (rev_list_unpacked)
		جوڙيو_unreachable_loose_objects(&revs);

	إذا (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	traverse_commit_list(&revs,
			     show_commit_pack_hint,
			     show_object_pack_hint,
			     &گهڻائي);

	release_revisions(&revs);

	trace2_معلومات_intmax("pack-objects", the_repository, "stdin_packs_found",
			   stdin_packs_مليو_nr);
	trace2_معلومات_intmax("pack-objects", the_repository, "stdin_packs_hints",
			   stdin_packs_hints_nr);

	repo->fetch_if_missing = prev_fetch_if_missing;
}

ساكن فراغ جوڙيو_cruft_object_داخلا(ثابت بنية object_id *oid, تعداد object_type type,
				   بنية packed_git *pack, off_t وٿي,
				   ثابت حرف *name, uint32_t mtime)
{
	بنية object_داخلا *داخلا;

	display_ترقي(ترقي_state, ++nr_ڏٺل);

	داخلا = packlist_ڳوليو(&منزل_pack, oid);
	إذا (داخلا) {
		إذا (name) {
			داخلا->hash = pack_name_hash_fn(name);
			داخلا->no_try_delta = no_try_delta(name);
		}
	} وإلا {
		إذا (!want_object_اندريون_pack_mtime(oid, 0, &pack, &وٿي, mtime))
			أرجع;
		إذا (!pack && type == OBJ_BLOB) {
			بنية odb_ذريعو_files *files =
				odb_ذريعو_files_downcast(the_repository->objects->ذريعو);
			صحيح مليو_2 = 0;

			لكل (بنية odb_files_dir *dir = files->dirs; !مليو_2 && dir; dir = dir->ايندڙ)
				إذا (!odb_ذريعو_read_object_info(&dir->loose->بنياد, oid, عدم, 0, عدم))
					مليو_2 = 1;

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
			إذا (!مليو_2)
				أرجع;
		}

		داخلا = create_object_داخلا(oid, type, pack_name_hash_fn(name),
					    0, name && no_try_delta(name),
					    pack, وٿي);
	}

	إذا (mtime > oe_cruft_mtime(&منزل_pack, داخلا))
		oe_سيٽ_cruft_mtime(&منزل_pack, داخلا, mtime);
	أرجع;
}

ساكن فراغ show_cruft_object(بنية object *obj, ثابت حرف *name, فراغ *معلومات اڻ_استعمال_ٿيل)
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
	جوڙيو_cruft_object_داخلا(&obj->oid, obj->type, عدم, 0, name, cruft_expiration);
}

ساكن فراغ show_cruft_commit(بنية commit *commit, فراغ *معلومات)
{
	show_cruft_object((بنية object*)commit, عدم, معلومات);
}

ساكن صحيح cruft_include_check_obj(بنية object *obj, فراغ *معلومات اڻ_استعمال_ٿيل)
{
	أرجع !has_object_kept_pack(منزل_pack.repo, &obj->oid, KEPT_PACK_اندريون_CORE);
}

ساكن صحيح cruft_include_check(بنية commit *commit, فراغ *معلومات)
{
	أرجع cruft_include_check_obj((بنية object*)commit, معلومات);
}

ساكن فراغ سيٽ_cruft_mtime(ثابت بنية object *object,
			    بنية packed_git *pack,
			    off_t وٿي, وقت_t mtime)
{
	جوڙيو_cruft_object_داخلا(&object->oid, object->type, pack, وٿي, عدم,
			       mtime);
}

ساكن فراغ mark_pack_kept_اندريون_core(بنية string_list *packs, بنانشان keep)
{
	بنية string_list_شي *شي = عدم;
	for_each_string_list_شي(شي, packs) {
		بنية packed_git *p = شي->util;
		إذا (!p)
			die(_("could not find pack '%s'"), شي->string);
		إذا (p->is_cruft && keep)
			ignore_packed_keep_اندريون_core_has_cruft = 1;
		p->pack_keep_اندريون_core = keep;
	}
}

ساكن فراغ جوڙيو_objects_اندريون_unpacked_packs(فراغ);

ساكن فراغ enumerate_cruft_objects(فراغ)
{
	إذا (ترقي)
		ترقي_state = شروعات_ترقي(the_repository,
						_("Enumerating cruft objects"), 0);

	جوڙيو_objects_اندريون_unpacked_packs();
	جوڙيو_unreachable_loose_objects(عدم);

	stop_ترقي(&ترقي_state);
}

ساكن فراغ enumerate_and_traverse_cruft_objects(بنية string_list *fresh_packs)
{
	بنية packed_git *p;
	بنية rev_info revs;
	صحيح ret;

	repo_init_revisions(the_repository, &revs, عدم);

	revs.ٽيگ_objects = 1;
	revs.وڻ_objects = 1;
	revs.blob_objects = 1;

	revs.include_check = cruft_include_check;
	revs.include_check_obj = cruft_include_check_obj;

	revs.ignore_missing_links = 1;

	إذا (ترقي)
		ترقي_state = شروعات_ترقي(the_repository,
						_("Enumerating cruft objects"), 0);
	ret = جوڙيو_unseen_recent_objects_منزل_traversal(&revs, cruft_expiration,
						     سيٽ_cruft_mtime, 1);
	stop_ترقي(&ترقي_state);

	إذا (ret)
		die(_("unable to add cruft objects"));

	/*
	 * Re-mark only the fresh packs as kept so that objects in
	 * unknown packs do not halt the reachability traversal early.
	 */
	repo_for_each_pack(the_repository, p)
		p->pack_keep_اندريون_core = 0;
	mark_pack_kept_اندريون_core(fresh_packs, 1);

	إذا (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	إذا (ترقي)
		ترقي_state = شروعات_ترقي(the_repository,
						_("Traversing cruft objects"), 0);
	nr_ڏٺل = 0;
	traverse_commit_list(&revs, show_cruft_commit, show_cruft_object, عدم);

	stop_ترقي(&ترقي_state);
	release_revisions(&revs);
}

ساكن فراغ read_cruft_objects(فراغ)
{
	بنية strbuf buf = STRBUF_INIT;
	بنية string_list discard_packs = STRING_LIST_INIT_DUP;
	بنية string_list fresh_packs = STRING_LIST_INIT_DUP;
	بنية packed_git *p;

	ignore_packed_keep_اندريون_core = 1;

	طالما (strbuf_getline(&buf, stdin) != EOF) {
		إذا (!buf.len)
			استمر;

		إذا (*buf.buf == '-')
			string_list_append(&discard_packs, buf.buf + 1);
		وإلا
			string_list_append(&fresh_packs, buf.buf);
	}

	string_list_ترتيب_ڏيو(&discard_packs);
	string_list_ترتيب_ڏيو(&fresh_packs);

	repo_for_each_pack(the_repository, p) {
		ثابت حرف *pack_name = pack_basename(p);
		بنية string_list_شي *شي;

		شي = string_list_lookup(&fresh_packs, pack_name);
		إذا (!شي)
			شي = string_list_lookup(&discard_packs, pack_name);

		إذا (شي) {
			شي->util = p;
		} وإلا {
			/*
			 * This pack wasn't mentioned in either the "fresh" or
			 * "discard" list, so the caller didn't know about it.
			 *
			 * Mark it as kept so that its objects are ignored by
			 * add_unseen_recent_objects_to_traversal(). We'll
			 * unmark it before starting the traversal so it doesn't
			 * halt the traversal early.
			 */
			p->pack_keep_اندريون_core = 1;
		}
	}

	mark_pack_kept_اندريون_core(&fresh_packs, 1);
	mark_pack_kept_اندريون_core(&discard_packs, 0);

	إذا (cruft_expiration)
		enumerate_and_traverse_cruft_objects(&fresh_packs);
	وإلا
		enumerate_cruft_objects();

	strbuf_release(&buf);
	string_list_clear(&discard_packs, 0);
	string_list_clear(&fresh_packs, 0);
}

ساكن فراغ read_object_list_from_stdin(فراغ)
{
	حرف سٽ[GIT_MAX_HEXSZ + 1 + رستو_MAX + 2];
	بنية object_id oid;
	ثابت حرف *p;

	لكل (;;) {
		إذا (!fgets(سٽ, حجم(سٽ), stdin)) {
			إذا (feof(stdin))
				اكسر;
			إذا (!ferror(stdin))
				BUG("fgets returned NULL, not EOF, not error!");
			إذا (errno != EINTR)
				die_errno("fgets");
			clearerr(stdin);
			استمر;
		}
		إذا (سٽ[0] == '-') {
			إذا (get_oid_hex(سٽ+1, &oid))
				die(_("expected edge object ID, got garbage:\n %s"),
				    سٽ);
			جوڙيو_preferred_بنياد(&oid);
			استمر;
		}
		إذا (parse_oid_hex(سٽ, &oid, &p))
			die(_("expected object ID, got garbage:\n %s"), سٽ);

		جوڙيو_preferred_بنياد_object(p + 1);
		جوڙيو_object_داخلا(&oid, OBJ_NONE, p + 1, 0);
	}
}

ساكن فراغ show_commit(بنية commit *commit, فراغ *معلومات اڻ_استعمال_ٿيل)
{
	جوڙيو_object_داخلا(&commit->object.oid, OBJ_COMMIT, عدم, 0);

	إذا (write_bitmap_اشاريو)
		اشاريو_commit_for_bitmap(commit);

	إذا (use_delta_islands)
		propagate_island_marks(the_repository, commit);
}

ساكن فراغ show_object(بنية object *obj, ثابت حرف *name,
			فراغ *معلومات اڻ_استعمال_ٿيل)
{
	جوڙيو_preferred_بنياد_object(name);
	جوڙيو_object_داخلا(&obj->oid, obj->type, name, 0);

	إذا (use_delta_islands)
		رڪارڊ_وڻ_اونهائي(&obj->oid, name);
}

ساكن فراغ show_object__ma_allow_any(بنية object *obj, ثابت حرف *name, فراغ *معلومات)
{
	أكد(arg_missing_action == MA_ALLOW_ANY);

	/*
	 * Quietly ignore ALL missing objects.  This avoids problems with
	 * staging them now and getting an odd error later.
	 */
	إذا (!odb_has_object(the_repository->objects, &obj->oid, 0))
		أرجع;

	show_object(obj, name, معلومات);
}

ساكن فراغ show_object__ma_allow_promisor(بنية object *obj, ثابت حرف *name, فراغ *معلومات)
{
	أكد(arg_missing_action == MA_ALLOW_PROMISOR);

	/*
	 * Quietly ignore EXPECTED missing objects.  This avoids problems with
	 * staging them now and getting an odd error later.
	 */
	إذا (!odb_has_object(the_repository->objects, &obj->oid, 0) &&
	    is_promisor_object(منزل_pack.repo, &obj->oid))
		أرجع;

	show_object(obj, name, معلومات);
}

ساكن صحيح option_parse_missing_action(ثابت بنية option *opt,
				       ثابت حرف *arg, صحيح unset)
{
	بنية repository *repo = opt->قدر;

	أكد(arg);
	أكد(!unset);

	إذا (!strcmp(arg, "error")) {
		arg_missing_action = MA_ERROR;
		fn_show_object = show_object;
		أرجع 0;
	}

	إذا (!strcmp(arg, "allow-any")) {
		arg_missing_action = MA_ALLOW_ANY;
		repo->fetch_if_missing = 0;
		fn_show_object = show_object__ma_allow_any;
		أرجع 0;
	}

	إذا (!strcmp(arg, "allow-promisor")) {
		arg_missing_action = MA_ALLOW_PROMISOR;
		repo->fetch_if_missing = 0;
		fn_show_object = show_object__ma_allow_promisor;
		أرجع 0;
	}

	die(_("invalid value for '%s': '%s'"), "--missing", arg);
	أرجع 0;
}

ساكن فراغ show_ڪنارو(بنية commit *commit)
{
	جوڙيو_preferred_بنياد(&commit->object.oid);
}

ساكن صحيح جوڙيو_object_اندريون_unpacked_pack(ثابت بنية object_id *oid,
				       بنية object_info *oi,
				       فراغ *معلومات اڻ_استعمال_ٿيل)
{
	إذا (cruft) {
		جوڙيو_cruft_object_داخلا(oid, OBJ_NONE, oi->ذريعو_infop->u.packed.pack,
				       oi->ذريعو_infop->u.packed.وٿي, عدم,
				       *oi->mtimep);
	} وإلا {
		جوڙيو_object_داخلا(oid, OBJ_NONE, "", 0);
	}
	أرجع 0;
}

ساكن فراغ جوڙيو_objects_اندريون_unpacked_packs(فراغ)
{
	بنية odb_ذريعو_files *files =
		odb_ذريعو_files_downcast(منزل_pack.repo->objects->ذريعو);
	وقت_t mtime;
	بنية odb_for_each_object_options opts = {
		.flags = ODB_FOR_EACH_OBJECT_PACK_ترتيب |
			 ODB_FOR_EACH_OBJECT_LOCAL_ONLY |
			 ODB_FOR_EACH_OBJECT_SKIP_اندريون_CORE_KEPT_PACKS |
			 ODB_FOR_EACH_OBJECT_SKIP_ON_DISK_KEPT_PACKS,
	};
	بنية odb_ذريعو_info ذريعو_info;
	بنية object_info oi = {
		.mtimep = &mtime,
		.ذريعو_infop = &ذريعو_info,
	};

	لكل (بنية odb_files_dir *dir = files->dirs; dir; dir = dir->ايندڙ) {
		إذا (!dir->local)
			استمر;

		إذا (odb_ذريعو_for_each_object(&dir->packed->بنياد, &oi,
					       جوڙيو_object_اندريون_unpacked_pack, عدم, &opts))
			die(_("cannot open pack index"));
	}
}

ساكن صحيح جوڙيو_loose_object(ثابت بنية object_id *oid, ثابت حرف *رستو,
			    فراغ *معلومات)
{
	بنية rev_info *revs = معلومات;
	تعداد object_type type = odb_read_object_info(the_repository->objects, oid, عدم);

	إذا (type < 0) {
		warning(_("loose object at %s could not be examined"), رستو);
		أرجع 0;
	}

	إذا (cruft) {
		بنية stat st;
		إذا (stat(رستو, &st) < 0) {
			إذا (errno == ENOENT)
				أرجع 0;
			أرجع error_errno("unable to stat %s", oid_منزل_hex(oid));
		}

		جوڙيو_cruft_object_داخلا(oid, type, عدم, 0, عدم,
				       st.st_mtime);
	} وإلا {
		جوڙيو_object_داخلا(oid, type, "", 0);
	}

	إذا (revs && type == OBJ_COMMIT)
		جوڙيو_pending_oid(revs, عدم, oid, 0);

	أرجع 0;
}

/*
 * We actually don't even have to worry about reachability here.
 * add_object_entry will weed out duplicates, so we just add every
 * loose object we find.
 */
ساكن فراغ جوڙيو_unreachable_loose_objects(بنية rev_info *revs)
{
	for_each_loose_file_اندريون_ذريعو(the_repository->objects->ذريعو,
				      جوڙيو_loose_object, عدم, عدم, revs);
}

ساكن صحيح has_sha1_pack_kept_or_nonlocal(ثابت بنية object_id *oid)
{
	ساكن بنية packed_git *آخري_مليو = عدم;
	بنية packed_git *p;

	إذا (آخري_مليو && ڳوليو_pack_داخلا_one(oid, آخري_مليو))
		أرجع 1;

	repo_for_each_pack(the_repository, p) {
		/*
		 * We have already checked `last_found`, so there is no need to
		 * re-check here.
		 */
		إذا (p == آخري_مليو)
			استمر;

		إذا ((!p->pack_local || p->pack_keep || p->pack_keep_اندريون_core) &&
		    ڳوليو_pack_داخلا_one(oid, p)) {
			آخري_مليو = p;
			أرجع 1;
		}
	}

	أرجع 0;
}

/*
 * Store a list of sha1s that are should not be discarded
 * because they are either written too recently, or are
 * reachable from another object that was.
 *
 * This is filled by get_object_list.
 */
ساكن بنية oid_array recent_objects;

ساكن صحيح loosened_object_can_be_discarded(ثابت بنية object_id *oid,
					    timestamp_t mtime)
{
	إذا (!unpack_unreachable_expiration)
		أرجع 0;
	إذا (mtime > unpack_unreachable_expiration)
		أرجع 0;
	إذا (oid_array_lookup(&recent_objects, oid) >= 0)
		أرجع 0;
	أرجع 1;
}

ساكن صحيح force_object_loose(بنية odb_ذريعو *ذريعو,
			      ثابت بنية object_id *oid,
			      ثابت وقت_t *mtime)
{
	بنية odb_ذريعو_files *files = odb_ذريعو_files_downcast(ذريعو);
	ثابت بنية git_hash_algo *compat = ذريعو->odb->repo->compat_hash_algo;
	بنية object_info oi = OBJECT_INFO_INIT;
	بنية object_id compat_oid, *compat_oid_p = عدم;
	تعداد object_type type;
	فراغ *buf = عدم;
	سائيز_t len;
	صحيح ret;

	لكل (بنية odb_files_dir *dir = files->dirs; dir; dir = dir->ايندڙ)
		إذا (!odb_ذريعو_read_object_info(&dir->loose->بنياد, oid, عدم, 0, عدم))
			أرجع 0;

	oi.typep = &type;
	oi.sizep = &len;
	oi.contentp = &buf;
	إذا (odb_read_object_info_extended(ذريعو->odb, oid, &oi, 0)) {
		ret = error(_("cannot read object for %s"), oid_منزل_hex(oid));
		اذهب ٻاهريون;
	}

	إذا (compat) {
		إذا (repo_oid_منزل_algop(ذريعو->odb->repo, oid, compat, &compat_oid)) {
			ret = error(_("cannot map object %s to %s"),
				    oid_منزل_hex(oid), compat->name);
			اذهب ٻاهريون;
		}

		compat_oid_p = &compat_oid;
	}

	ret = odb_ذريعو_write_object(&files->dirs->loose->بنياد, buf, len, type, oid,
				      compat_oid_p, mtime, 0);

ٻاهريون:
	حرر(buf);
	أرجع ret;
}

ساكن فراغ loosen_اڻ_استعمال_ٿيل_packed_objects(فراغ)
{
	بنية packed_git *p;
	uint32_t i;
	uint32_t loosened_objects_nr = 0;
	بنية object_id oid;

	repo_for_each_pack(the_repository, p) {
		إذا (!p->pack_local || p->pack_keep || p->pack_keep_اندريون_core)
			استمر;

		إذا (open_pack_اشاريو(p))
			die(_("cannot open pack index"));

		لكل (i = 0; i < p->انگ_objects; i++) {
			nth_packed_object_id(&oid, p, i);
			إذا (!packlist_ڳوليو(&منزل_pack, &oid) &&
			    !has_sha1_pack_kept_or_nonlocal(&oid) &&
			    !loosened_object_can_be_discarded(&oid, p->mtime)) {
				إذا (force_object_loose(the_repository->objects->ذريعو,
						       &oid, &p->mtime))
					die(_("unable to force loose object"));
				loosened_objects_nr++;
			}
		}
	}

	trace2_معلومات_intmax("pack-objects", the_repository,
			   "loosen_unused_packed_objects/loosened", loosened_objects_nr);
}

/*
 * This tracks any options which pack-reuse code expects to be on, or which a
 * reader of the pack might not understand, and which would therefore prevent
 * blind reuse of what we have on disk.
 */
ساكن صحيح pack_options_allow_reuse(فراغ)
{
	أرجع allow_pack_reuse != NO_PACK_REUSE &&
	       pack_منزل_stdout &&
	       !ignore_packed_keep_on_disk &&
	       !ignore_packed_keep_اندريون_core &&
	       (!local || !have_non_local_packs) &&
	       !incremental;
}

ساكن صحيح get_object_list_from_bitmap(بنية rev_info *revs)
{
	إذا (!(bitmap_git = prepare_bitmap_walk(revs, 0)))
		أرجع -1;

	/*
	 * For now, force the name-hash version to be 1 since that
	 * is the version implied by the bitmap format. Later, the
	 * format can include this version explicitly in its format,
	 * allowing readers to know the version that was used during
	 * the bitmap write.
	 */
	name_hash_نسخو = 1;

	إذا (pack_options_allow_reuse())
		reuse_partial_packfile_from_bitmap(bitmap_git,
						   &reuse_packfiles,
						   &reuse_packfiles_nr,
						   &reuse_packfile_bitmap,
						   allow_pack_reuse == MULTI_PACK_REUSE);

	إذا (reuse_packfiles) {
		reuse_packfile_objects = bitmap_popcount(reuse_packfile_bitmap);
		إذا (!reuse_packfile_objects)
			BUG("expected non-empty reuse bitmap");

		nr_نتيجو += reuse_packfile_objects;
		nr_ڏٺل += reuse_packfile_objects;
		display_ترقي(ترقي_state, nr_ڏٺل);
	}

	traverse_bitmap_commit_list(bitmap_git, revs,
				    &جوڙيو_object_داخلا_from_bitmap);
	أرجع 0;
}

ساكن فراغ رڪارڊ_recent_object(بنية object *obj,
				 ثابت حرف *name اڻ_استعمال_ٿيل,
				 فراغ *معلومات اڻ_استعمال_ٿيل)
{
	oid_array_append(&recent_objects, &obj->oid);
}

ساكن فراغ رڪارڊ_recent_commit(بنية commit *commit, فراغ *معلومات اڻ_استعمال_ٿيل)
{
	oid_array_append(&recent_objects, &commit->object.oid);
}

ساكن صحيح mark_bitmap_preferred_tip(ثابت بنية reference *ref, فراغ *معلومات اڻ_استعمال_ٿيل)
{
	ثابت بنية object_id *maybe_peeled = ref->oid;
	بنية object_id peeled;
	بنية object *object;

	إذا (!reference_get_peeled_oid(the_repository, ref, &peeled))
		maybe_peeled = &peeled;

	object = parse_object_or_die(the_repository, maybe_peeled, ref->name);
	إذا (object->type == OBJ_COMMIT)
		object->flags |= NEEDS_BITMAP;

	أرجع 0;
}

ساكن مضمن صحيح is_oid_uninteresting(بنية repository *repo,
				       بنية object_id *oid)
{
	بنية object *o = lookup_object(repo, oid);
	أرجع !o || (o->flags & UNINTERESTING);
}

ساكن صحيح جوڙيو_objects_by_رستو(ثابت حرف *رستو,
			       بنية oid_array *oids,
			       تعداد object_type type,
			       فراغ *معلومات)
{
	سائيز_t oe_شروعات = منزل_pack.nr_objects;
	سائيز_t oe_پڇاڙي;
	بنانشان صحيح *processed = معلومات;

	/*
	 * First, add all objects to the packing data, including the ones
	 * marked UNINTERESTING (translated to 'exclude') as they can be
	 * used as delta bases.
	 */
	لكل (سائيز_t i = 0; i < oids->nr; i++) {
		صحيح exclude;
		بنية object_info oi = OBJECT_INFO_INIT;
		بنية object_id *oid = &oids->oid[i];

		/* Skip objects that do not exist locally. */
		إذا ((exclude_promisor_objects || arg_missing_action != MA_ERROR) &&
		    odb_read_object_info_extended(the_repository->objects, oid, &oi,
						  OBJECT_INFO_FOR_PREFETCH) < 0)
			استمر;

		exclude = is_oid_uninteresting(the_repository, oid);

		إذا (exclude && !thin)
			استمر;

		جوڙيو_object_داخلا(oid, type, رستو, exclude);

		إذا (type == OBJ_COMMIT) {
			بنية commit *commit;

			إذا (!write_bitmap_اشاريو && !use_delta_islands)
				استمر;

			commit = lookup_commit(the_repository, oid);
			إذا (!commit)
				die(_("could not find commit %s"), oid_منزل_hex(oid));
			إذا (write_bitmap_اشاريو)
				اشاريو_commit_for_bitmap(commit);
			/*
			 * Skip island propagation for boundary commits.
			 * The regular traversal's show_commit() is only
			 * called for interesting commits; matching that
			 * here keeps path-walk from doing extra work that
			 * would only be a no-op anyway (boundary commits
			 * are not in island_marks).
			 */
			إذا (use_delta_islands && !exclude)
				propagate_island_marks(the_repository, commit);
		} وإلا إذا (type == OBJ_وڻ && use_delta_islands) {
			رڪارڊ_وڻ_اونهائي(oid, رستو);
		}
	}

	oe_پڇاڙي = منزل_pack.nr_objects;

	/* We can skip delta calculations if it is a no-op. */
	إذا (oe_پڇاڙي == oe_شروعات || !دري)
		أرجع 0;

	ALLOC_GROW(منزل_pack.regions,
		   منزل_pack.nr_regions + 1,
		   منزل_pack.nr_regions_alloc);

	منزل_pack.regions[منزل_pack.nr_regions].شروعات = oe_شروعات;
	منزل_pack.regions[منزل_pack.nr_regions].nr = oe_پڇاڙي - oe_شروعات;
	منزل_pack.nr_regions++;

	*processed += oids->nr;
	display_ترقي(ترقي_state, *processed);

	أرجع 0;
}

ساكن صحيح get_object_list_رستو_walk(بنية rev_info *revs)
{
	بنية رستو_walk_info info = رستو_WALK_INFO_INIT;
	بنانشان صحيح processed = 0;
	صحيح نتيجو;

	info.revs = revs;
	info.رستو_fn = جوڙيو_objects_by_رستو;
	info.رستو_fn_معلومات = &processed;

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
	info.ڪنارو_aggressive = shallow;

	trace2_region_enter("pack-objects", "path-walk", revs->repo);
	نتيجو = walk_objects_by_رستو(&info);
	trace2_region_leave("pack-objects", "path-walk", revs->repo);

	رستو_walk_info_clear(&info);

	أرجع نتيجو;
}

ساكن فراغ get_object_list(بنية rev_info *revs, بنية strvec *argv)
{
	بنية تياري_revision_opt s_r_opt = {
		.allow_exclude_promisor_objects = 1,
	};
	بنية repo_config_قدرون *cfg = repo_config_قدرون(the_repository);
	حرف سٽ[1000];
	صحيح flags = 0;
	صحيح save_warning;

	save_commit_بفر = 0;
	تياري_revisions_from_strvec(argv, revs, &s_r_opt);

	/* make sure shallows are read */
	is_repository_shallow(the_repository);

	save_warning = cfg->warn_on_object_refname_ambiguity;
	cfg->warn_on_object_refname_ambiguity = 0;

	طالما (fgets(سٽ, حجم(سٽ), stdin) != عدم) {
		صحيح len = لڙي_ڊيگهه(سٽ);
		إذا (len && سٽ[len - 1] == '\n')
			سٽ[--len] = 0;
		إذا (!len)
			اكسر;
		إذا (*سٽ == '-') {
			إذا (!strcmp(سٽ, "--not")) {
				flags ^= UNINTERESTING;
				write_bitmap_اشاريو = 0;
				استمر;
			}
			إذا (starts_with(سٽ, "--shallow ")) {
				بنية object_id oid;
				إذا (get_oid_hex(سٽ + 10, &oid))
					die("not an object name '%s'", سٽ + 10);
				رجسٽر_shallow(the_repository, &oid);
				use_bitmap_اشاريو = 0;
				استمر;
			}
			die(_("not a rev '%s'"), سٽ);
		}
		إذا (handle_revision_arg(سٽ, revs, flags, REVARG_CANNOT_BE_FILENAME))
			die(_("bad revision '%s'"), سٽ);
	}

	cfg->warn_on_object_refname_ambiguity = save_warning;

	إذا (use_bitmap_اشاريو && !get_object_list_from_bitmap(revs))
		أرجع;

	إذا (use_delta_islands)
		ڀريو_delta_islands(the_repository, ترقي);

	إذا (write_bitmap_اشاريو)
		for_each_preferred_bitmap_tip(the_repository, mark_bitmap_preferred_tip,
					      عدم);

	إذا (!fn_show_object)
		fn_show_object = show_object;

	إذا (رستو_walk) {
		إذا (get_object_list_رستو_walk(revs)) {
			warning(_("failed to pack objects via path-walk"));
			رستو_walk = 0;
		}
	}

	إذا (!رستو_walk) {
		إذا (prepare_revision_walk(revs))
			die(_("revision walk setup failed"));
		mark_ڪنارا_uninteresting(revs, show_ڪنارو, sparse);
		traverse_commit_list(revs,
				show_commit, fn_show_object,
				عدم);
	}

	إذا (unpack_unreachable_expiration) {
		revs->ignore_missing_links = 1;
		إذا (جوڙيو_unseen_recent_objects_منزل_traversal(revs,
				unpack_unreachable_expiration, عدم, 0))
			die(_("unable to add recent objects"));
		إذا (prepare_revision_walk(revs))
			die(_("revision walk setup failed"));
		traverse_commit_list(revs, رڪارڊ_recent_commit,
				     رڪارڊ_recent_object, عدم);
	}

	إذا (keep_unreachable)
		جوڙيو_objects_اندريون_unpacked_packs();
	إذا (pack_loose_unreachable)
		جوڙيو_unreachable_loose_objects(عدم);
	إذا (unpack_unreachable)
		loosen_اڻ_استعمال_ٿيل_packed_objects();

	oid_array_clear(&recent_objects);
}

ساكن فراغ جوڙيو_extra_kept_packs(ثابت بنية string_list *نالا)
{
	بنية packed_git *p;

	إذا (!نالا->nr)
		أرجع;

	repo_for_each_pack(the_repository, p) {
		ثابت حرف *name = basename(p->pack_name);
		صحيح i;

		إذا (!p->pack_local)
			استمر;

		لكل (i = 0; i < نالا->nr; i++)
			إذا (!fspathcmp(name, نالا->شيون[i].string))
				اكسر;

		إذا (i < نالا->nr) {
			p->pack_keep_اندريون_core = 1;
			ignore_packed_keep_اندريون_core = 1;
			استمر;
		}
	}
}

ساكن صحيح option_parse_quiet(ثابت بنية option *opt, ثابت حرف *arg,
			      صحيح unset)
{
	صحيح *val = opt->قدر;

	BUG_ON_OPT_ARG(arg);

	إذا (!unset)
		*val = 0;
	وإلا إذا (!*val)
		*val = 1;
	أرجع 0;
}

ساكن صحيح option_parse_اشاريو_نسخو(ثابت بنية option *opt,
				      ثابت حرف *arg, صحيح unset)
{
	بنية pack_idx_option *popts = opt->قدر;
	حرف *c;
	ثابت حرف *val = arg;

	BUG_ON_OPT_NEG(unset);

	popts->نسخو = strtoul(val, &c, 10);
	إذا (popts->نسخو > 2)
		die(_("unsupported index version %s"), val);
	إذا (*c == ',' && c[1])
		popts->off32_حد_بندي = strtoul(c+1, &c, 0);
	إذا (*c || popts->off32_حد_بندي & 0x80000000)
		die(_("bad index version '%s'"), val);
	أرجع 0;
}

ساكن صحيح option_parse_unpack_unreachable(ثابت بنية option *opt اڻ_استعمال_ٿيل,
					   ثابت حرف *arg, صحيح unset)
{
	إذا (unset) {
		unpack_unreachable = 0;
		unpack_unreachable_expiration = 0;
	}
	وإلا {
		unpack_unreachable = 1;
		إذا (arg)
			unpack_unreachable_expiration = approxidate(arg);
	}
	أرجع 0;
}

ساكن صحيح option_parse_cruft_expiration(ثابت بنية option *opt اڻ_استعمال_ٿيل,
					 ثابت حرف *arg, صحيح unset)
{
	إذا (unset) {
		cruft = 0;
		cruft_expiration = 0;
	} وإلا {
		cruft = 1;
		إذا (arg)
			cruft_expiration = approxidate(arg);
	}
	أرجع 0;
}

ساكن صحيح is_not_اندريون_promisor_pack_obj(بنية object *obj, فراغ *معلومات اڻ_استعمال_ٿيل)
{
	بنية odb_ذريعو_info ذريعو_info;
	بنية object_info info = {
		.ذريعو_infop = &ذريعو_info,
	};

	إذا (odb_read_object_info_extended(the_repository->objects, &obj->oid, &info, 0))
		BUG("should_include_obj should only be called on existing objects");
	أرجع ذريعو_info.ذريعو->type != ODB_ذريعو_PACKED || !ذريعو_info.u.packed.pack->pack_promisor;
}

ساكن صحيح is_not_اندريون_promisor_pack(بنية commit *commit, فراغ *معلومات) {
	أرجع is_not_اندريون_promisor_pack_obj((بنية object *) commit, معلومات);
}

ساكن صحيح parse_stdin_packs_گهڻائي(ثابت بنية option *opt, ثابت حرف *arg,
				  صحيح unset)
{
	تعداد stdin_packs_گهڻائي *گهڻائي = opt->قدر;

	إذا (unset)
		*گهڻائي = STDIN_PACKS_گهڻائي_NONE;
	وإلا إذا (!arg || !*arg)
		*گهڻائي = STDIN_PACKS_گهڻائي_STANDARD;
	وإلا إذا (!strcmp(arg, "follow"))
		*گهڻائي = STDIN_PACKS_گهڻائي_FOLLOW;
	وإلا
		die(_("invalid value for '%s': '%s'"), opt->long_name, arg);

	أرجع 0;
}

صحيح cmd_pack_objects(صحيح argc,
		     ثابت حرف **argv,
		     ثابت حرف *اڳياڙي,
		     بنية repository *repo)
{
	صحيح use_اندروني_rev_list = 0;
	صحيح all_ترقي_implied = 0;
	بنية strvec rp = STRVEC_INIT;
	صحيح rev_list_unpacked = 0, rev_list_all = 0, rev_list_reflog = 0;
	صحيح rev_list_اشاريو = 0;
	تعداد stdin_packs_گهڻائي stdin_packs = STDIN_PACKS_گهڻائي_NONE;
	بنية string_list keep_pack_list = STRING_LIST_INIT_NODUP;
	بنية list_objects_filter_options filter_options =
		LIST_OBJECTS_FILTER_INIT;
	بنية repo_config_قدرون *cfg = repo_config_قدرون(the_repository);

	بنية option pack_objects_options[] = {
		OPT_CALLBACK_F('q', "quiet", &ترقي, عدم,
			       N_("do not show progress meter"),
			       PARSE_OPT_NOARG, option_parse_quiet),
		OPT_سيٽ_INT(0, "progress", &ترقي,
			    N_("show progress meter"), 1),
		OPT_سيٽ_INT(0, "all-progress", &ترقي,
			    N_("show progress meter during object writing phase"), 2),
		OPT_BOOL(0, "all-progress-implied",
			 &all_ترقي_implied,
			 N_("similar to --all-progress when progress meter is shown")),
		OPT_CALLBACK_F(0, "index-version", &pack_idx_opts, N_("<version>[,<offset>]"),
		  N_("write the pack index file in the specified idx format version"),
		  PARSE_OPT_NONEG, option_parse_اشاريو_نسخو),
		OPT_UNSIGNED(0, "max-pack-size", &pack_سائيز_حد_بندي,
			     N_("maximum size of each output pack file")),
		OPT_BOOL(0, "local", &local,
			 N_("ignore borrowed objects from alternate object store")),
		OPT_BOOL(0, "incremental", &incremental,
			 N_("ignore packed objects")),
		OPT_INTEGER(0, "window", &دري,
			    N_("limit pack window by objects")),
		OPT_UNSIGNED(0, "window-memory", &دري_يادگيري_حد_بندي,
			     N_("limit pack window by memory in addition to object limit")),
		OPT_INTEGER(0, "depth", &اونهائي,
			    N_("maximum length of delta chain allowed in the resulting pack")),
		OPT_BOOL(0, "reuse-delta", &reuse_delta,
			 N_("reuse existing deltas")),
		OPT_BOOL(0, "reuse-object", &reuse_object,
			 N_("reuse existing objects")),
		OPT_BOOL(0, "delta-base-offset", &allow_ofs_delta,
			 N_("use OFS_DELTA objects")),
		OPT_INTEGER(0, "threads", &delta_ڳولا_ڌاڳا,
			    N_("use threads when searching for best delta matches")),
		OPT_BOOL(0, "non-empty", &non_خالي_جايون,
			 N_("do not create an empty pack output")),
		OPT_BOOL(0, "revs", &use_اندروني_rev_list,
			 N_("read revision arguments from standard input")),
		OPT_سيٽ_INT_F(0, "unpacked", &rev_list_unpacked,
			      N_("limit the objects to those that are not yet packed"),
			      1, PARSE_OPT_NONEG),
		OPT_سيٽ_INT_F(0, "all", &rev_list_all,
			      N_("include objects reachable from any reference"),
			      1, PARSE_OPT_NONEG),
		OPT_سيٽ_INT_F(0, "reflog", &rev_list_reflog,
			      N_("include objects referred by reflog entries"),
			      1, PARSE_OPT_NONEG),
		OPT_سيٽ_INT_F(0, "indexed-objects", &rev_list_اشاريو,
			      N_("include objects referred to by the index"),
			      1, PARSE_OPT_NONEG),
		OPT_CALLBACK_F(0, "stdin-packs", &stdin_packs, N_("mode"),
			     N_("read packs from stdin"),
			     PARSE_OPT_OPTARG, parse_stdin_packs_گهڻائي),
		OPT_BOOL(0, "stdout", &pack_منزل_stdout,
			 N_("output pack to stdout")),
		OPT_BOOL(0, "include-tag", &include_ٽيگ,
			 N_("include tag objects that refer to objects to be packed")),
		OPT_BOOL(0, "keep-unreachable", &keep_unreachable,
			 N_("keep unreachable objects")),
		OPT_BOOL(0, "pack-loose-unreachable", &pack_loose_unreachable,
			 N_("pack loose unreachable objects")),
		OPT_CALLBACK_F(0, "unpack-unreachable", عدم, N_("time"),
		  N_("unpack unreachable objects newer than <time>"),
		  PARSE_OPT_OPTARG, option_parse_unpack_unreachable),
		OPT_BOOL(0, "cruft", &cruft, N_("create a cruft pack")),
		OPT_CALLBACK_F(0, "cruft-expiration", عدم, N_("time"),
		  N_("expire cruft objects older than <time>"),
		  PARSE_OPT_OPTARG, option_parse_cruft_expiration),
		OPT_BOOL(0, "sparse", &sparse,
			 N_("use the sparse reachability algorithm")),
		OPT_BOOL(0, "thin", &thin,
			 N_("create thin packs")),
		OPT_BOOL(0, "path-walk", &رستو_walk,
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
		OPT_BOOL(0, "use-bitmap-index", &use_bitmap_اشاريو,
			 N_("use a bitmap index if available to speed up counting objects")),
		OPT_سيٽ_INT(0, "write-bitmap-index", &write_bitmap_اشاريو,
			    N_("write a bitmap index together with the pack index"),
			    WRITE_BITMAP_TRUE),
		OPT_سيٽ_INT_F(0, "write-bitmap-index-quiet",
			      &write_bitmap_اشاريو,
			      N_("write a bitmap index if possible"),
			      WRITE_BITMAP_QUIET, PARSE_OPT_HIDDEN),
		OPT_PARSE_LIST_OBJECTS_FILTER(&filter_options),
		OPT_CALLBACK_F(0, "missing", repo, N_("action"),
		  N_("handling for missing objects"), PARSE_OPT_NONEG,
		  option_parse_missing_action),
		OPT_BOOL(0, "exclude-promisor-objects", &exclude_promisor_objects,
			 N_("do not pack objects in promisor packfiles")),
		OPT_BOOL(0, "exclude-promisor-objects-best-effort",
			 &exclude_promisor_objects_بهترين_effort,
			 N_("implies --missing=allow-any")),
		OPT_BOOL(0, "delta-islands", &use_delta_islands,
			 N_("respect islands during delta compression")),
		OPT_STRING_LIST(0, "uri-protocol", &uri_protocols,
				N_("protocol"),
				N_("exclude any configured uploadpack.blobpackfileuri with this protocol")),
		OPT_INTEGER(0, "name-hash-version", &name_hash_نسخو,
			 N_("use the specified name-hash function to group similar objects")),
		OPT_پڇاڙي(),
	};

	إذا (DFS_انگ_حالتون > (1 << OE_DFS_STATE_بٽ))
		BUG("too many dfs states, increase OE_DFS_STATE_BITS");

	disable_replace_refs();

	sparse = git_env_bool("GIT_TEST_PACK_SPARSE", -1);
	إذا (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		إذا (sparse < 0)
			sparse = the_repository->settings.pack_use_sparse;
		إذا (the_repository->settings.pack_use_multi_pack_reuse)
			allow_pack_reuse = MULTI_PACK_REUSE;
	}

	reset_pack_idx_option(&pack_idx_opts);
	pack_idx_opts.flags |= WRITE_REV;
	repo_config(the_repository, git_pack_config, عدم);
	إذا (git_env_bool(GIT_TEST_NO_WRITE_REV_اشاريو, 0))
		pack_idx_opts.flags &= ~WRITE_REV;

	ترقي = isatty(2);
	argc = parse_options(argc, argv, اڳياڙي, pack_objects_options,
			     pack_usage, 0);

	إذا (argc) {
		بنياد_name = argv[0];
		argc--;
	}
	إذا (pack_منزل_stdout != !بنياد_name || argc)
		usage_with_options(pack_usage, pack_objects_options);

	إذا (رستو_walk < 0) {
		إذا (use_bitmap_اشاريو > 0 ||
		    !use_اندروني_rev_list)
			رستو_walk = 0;
		وإلا إذا (the_repository->gitdir &&
			 the_repository->settings.pack_use_رستو_walk)
			رستو_walk = 1;
		وإلا
			رستو_walk = git_env_bool("GIT_TEST_PACK_PATH_WALK", 0);
	}

	إذا (اونهائي < 0)
		اونهائي = 0;
	إذا (اونهائي >= (1 << OE_اونهائي_بٽ)) {
		warning(_("delta chain depth %d is too deep, forcing %d"),
			اونهائي, (1 << OE_اونهائي_بٽ) - 1);
		اونهائي = (1 << OE_اونهائي_بٽ) - 1;
	}
	إذا (cache_max_small_delta_سائيز >= (1U << OE_Z_DELTA_بٽ)) {
		warning(_("pack.deltaCacheLimit is too high, forcing %d"),
			(1U << OE_Z_DELTA_بٽ) - 1);
		cache_max_small_delta_سائيز = (1U << OE_Z_DELTA_بٽ) - 1;
	}
	إذا (دري < 0)
		دري = 0;

	strvec_ڌڪو(&rp, "pack-objects");

	إذا (رستو_walk) {
		ثابت حرف *option = عدم;
		إذا (!رستو_walk_filter_compatible(&filter_options))
			option = "--filter";

		إذا (option) {
			warning(_("cannot use %s with %s"),
				option, "--path-walk");
			رستو_walk = 0;
		}
	}
	إذا (رستو_walk) {
		strvec_ڌڪو(&rp, "--objects");
	} وإلا إذا (thin) {
		use_اندروني_rev_list = 1;
		strvec_ڌڪو(&rp, shallow
				? "--objects-edge-aggressive"
				: "--objects-edge");
	} وإلا
		strvec_ڌڪو(&rp, "--objects");

	إذا (rev_list_all) {
		use_اندروني_rev_list = 1;
		strvec_ڌڪو(&rp, "--all");
	}
	إذا (rev_list_reflog) {
		use_اندروني_rev_list = 1;
		strvec_ڌڪو(&rp, "--reflog");
	}
	إذا (rev_list_اشاريو) {
		use_اندروني_rev_list = 1;
		strvec_ڌڪو(&rp, "--indexed-objects");
	}
	إذا (rev_list_unpacked && !stdin_packs) {
		use_اندروني_rev_list = 1;
		strvec_ڌڪو(&rp, "--unpacked");
	}

	die_for_incompatible_opt2(exclude_promisor_objects,
				  "--exclude-promisor-objects",
				  exclude_promisor_objects_بهترين_effort,
				  "--exclude-promisor-objects-best-effort");
	إذا (exclude_promisor_objects) {
		repo->fetch_if_missing = 0;

		/* --stdin-packs handles promisor objects separately. */
		إذا (!stdin_packs) {
			use_اندروني_rev_list = 1;
			strvec_ڌڪو(&rp, "--exclude-promisor-objects");
		}
	} وإلا إذا (exclude_promisor_objects_بهترين_effort) {
		use_اندروني_rev_list = 1;
		arg_missing_action = MA_ALLOW_ANY;
		repo->fetch_if_missing = 0;
		fn_show_object = show_object__ma_allow_any;
		/* revs configured below */
	}
	إذا (unpack_unreachable || keep_unreachable || pack_loose_unreachable)
		use_اندروني_rev_list = 1;

	إذا (!reuse_object)
		reuse_delta = 0;
	إذا (cfg->pack_compression_level == -1)
		cfg->pack_compression_level = Z_DEFAULT_COMPRESSION;
	وإلا إذا (cfg->pack_compression_level < 0 || cfg->pack_compression_level > Z_بهترين_COMPRESSION)
		die(_("bad pack compression level %d"), cfg->pack_compression_level);

	إذا (!delta_ڳولا_ڌاڳا)	/* --threads=0 means autodetect */
		delta_ڳولا_ڌاڳا = online_cpus();

	إذا (!HAVE_ڌاڳا && delta_ڳولا_ڌاڳا != 1)
		warning(_("no threads support, ignoring --threads"));
	إذا (!pack_منزل_stdout && !pack_سائيز_حد_بندي)
		pack_سائيز_حد_بندي = pack_سائيز_حد_بندي_cfg;
	إذا (pack_منزل_stdout && pack_سائيز_حد_بندي)
		die(_("--max-pack-size cannot be used to build a pack for transfer"));
	إذا (pack_سائيز_حد_بندي && pack_سائيز_حد_بندي < 1024*1024) {
		warning(_("minimum pack size limit is 1 MiB"));
		pack_سائيز_حد_بندي = 1024*1024;
	}

	إذا (!pack_منزل_stdout && thin)
		die(_("--thin cannot be used to build an indexable pack"));

	die_for_incompatible_opt2(keep_unreachable, "--keep-unreachable",
				  unpack_unreachable, "--unpack-unreachable");
	إذا (!rev_list_all || !rev_list_reflog || !rev_list_اشاريو)
		unpack_unreachable_expiration = 0;

	die_for_incompatible_opt2(stdin_packs, "--stdin-packs",
				  filter_options.choice, "--filter");


	إذا (stdin_packs && use_اندروني_rev_list)
		die(_("cannot use internal rev list with --stdin-packs"));

	إذا (cruft) {
		إذا (use_اندروني_rev_list)
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
	إذا (!pack_منزل_stdout)
		use_bitmap_اشاريو_default = 0;

	إذا (use_bitmap_اشاريو < 0)
		use_bitmap_اشاريو = use_bitmap_اشاريو_default;

	/* "hard" reasons not to use bitmaps; these just won't work at all */
	إذا (!use_اندروني_rev_list || (!pack_منزل_stdout && write_bitmap_اشاريو) || is_repository_shallow(the_repository))
		use_bitmap_اشاريو = 0;

	إذا (pack_منزل_stdout || !rev_list_all)
		write_bitmap_اشاريو = 0;

	إذا (name_hash_نسخو < 0)
		name_hash_نسخو = (صحيح)git_env_ulong("GIT_TEST_NAME_HASH_VERSION", 1);

	validate_name_hash_نسخو();

	إذا (use_delta_islands)
		strvec_ڌڪو(&rp, "--topo-order");

	إذا (ترقي && all_ترقي_implied)
		ترقي = 2;

	جوڙيو_extra_kept_packs(&keep_pack_list);
	إذا (ignore_packed_keep_on_disk) {
		بنية packed_git *p;

		repo_for_each_pack(the_repository, p)
			إذا (p->pack_local && p->pack_keep)
				اكسر;
		إذا (!p) /* no keep-able packs found */
			ignore_packed_keep_on_disk = 0;
	}
	إذا (local) {
		/*
		 * unlike ignore_packed_keep_on_disk above, we do not
		 * want to unset "local" based on looking at packs, as
		 * it also covers non-local objects
		 */
		بنية packed_git *p;

		repo_for_each_pack(the_repository, p) {
			إذا (!p->pack_local) {
				have_non_local_packs = 1;
				اكسر;
			}
		}
	}

	trace2_region_enter("pack-objects", "enumerate-objects",
			    the_repository);
	prepare_packing_معلومات(the_repository, &منزل_pack);

	إذا (ترقي && !cruft)
		ترقي_state = شروعات_ترقي(the_repository,
						_("Enumerating objects"), 0);
	إذا (stdin_packs) {
		read_stdin_packs(repo, stdin_packs, rev_list_unpacked);
	} وإلا إذا (cruft) {
		read_cruft_objects();
	} وإلا إذا (!use_اندروني_rev_list) {
		read_object_list_from_stdin();
	} وإلا {
		بنية rev_info revs;

		repo_init_revisions(the_repository, &revs, عدم);
		list_objects_filter_copy(&revs.filter, &filter_options);
		إذا (exclude_promisor_objects_بهترين_effort) {
			revs.include_check = is_not_اندريون_promisor_pack;
			revs.include_check_obj = is_not_اندريون_promisor_pack_obj;
		}
		get_object_list(&revs, &rp);
		release_revisions(&revs);
	}
	cleanup_preferred_بنياد();
	إذا (include_ٽيگ && nr_نتيجو)
		refs_for_each_ٽيگ_ref(get_main_ref_store(the_repository),
				      جوڙيو_ref_ٽيگ, عدم);
	stop_ترقي(&ترقي_state);
	trace2_region_leave("pack-objects", "enumerate-objects",
			    the_repository);

	إذا (non_خالي_جايون && !nr_نتيجو)
		اذهب cleanup;
	إذا (nr_نتيجو) {
		trace2_region_enter("pack-objects", "prepare-pack",
				    the_repository);
		prepare_pack(دري, اونهائي);
		trace2_region_leave("pack-objects", "prepare-pack",
				    the_repository);
	}

	trace2_region_enter("pack-objects", "write-pack-file", the_repository);
	write_excluded_by_configs();
	write_pack_file();
	trace2_region_leave("pack-objects", "write-pack-file", the_repository);

	إذا (ترقي)
		fprintf_ln(stderr,
			   _("Total %"PRIu32" (delta %"PRIu32"),"
			     " reused %"PRIu32" (delta %"PRIu32"),"
			     " pack-reused %"PRIu32" (from %"PRIuMAX")"),
			   written, written_delta, reused, reused_delta,
			   reuse_packfile_objects,
			   (uintmax_t)reuse_packfiles_استعمال_ٿيل_nr);

	trace2_معلومات_intmax("pack-objects", the_repository, "written", written);
	trace2_معلومات_intmax("pack-objects", the_repository, "written/delta", written_delta);
	trace2_معلومات_intmax("pack-objects", the_repository, "reused", reused);
	trace2_معلومات_intmax("pack-objects", the_repository, "reused/delta", reused_delta);
	trace2_معلومات_intmax("pack-objects", the_repository, "pack-reused", reuse_packfile_objects);
	trace2_معلومات_intmax("pack-objects", the_repository, "packs-reused", reuse_packfiles_استعمال_ٿيل_nr);

cleanup:
	clear_packing_معلومات(&منزل_pack);
	list_objects_filter_release(&filter_options);
	string_list_clear(&keep_pack_list, 0);
	strvec_clear(&rp);

	أرجع 0;
}
